/* Lowering pass for OpenMP directives.  Converts OpenMP directives
   into explicit calls to the runtime library (libgomp) and data
   marshalling to implement data sharing and copying clauses.
   Contributed by Diego Novillo <dnovillo@redhat.com>

   Copyright (C) 2005 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "varray.h"
#include "splay-tree.h"
#include "tree-gimple.h"
#include "tree-inline.h"
#include "diagnostic.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "tree-flow.h"
#include "timevar.h"
#include "except.h"
#include "hashtab.h"
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "toplev.h"
#include "tree-pass.h"
#include "vec.h"

/* Data structure used for implementing all the data sharing/copying
   clauses.  */

struct remap_info_d
{
  /* Function holding the body of the OpenMP directive.  */
  tree omp_fn;

  /* Iterator where to insert code in the parent function.  Initially,
     this points to the OpenMP directive being lowered.  */
  tree_stmt_iterator *tsi;

  /* Set of local variables to be shared among the threads.  */
  bitmap shared;

  /* Structure holding addresses of shared and copy-in data to be
     passed to the children threads.  This is declared in the parent
     thread.  */
  tree omp_data;

  /* Mapping between local variables in the original function and the
     fields of OMP_DATA.  */
  splay_tree omp_data_map;

  /* Mapping between local variables in the original function and
     local variables in the new function OMP_FN.  Shared variables are
     replaced with dereferences of the DATA argument in OMP_FN,
     private variables are replaced with new local variables inside
     OMP_FN.  */
  splay_tree local_map;

  /* Pointer type to OMP_DATA.  */
  tree ptr_to_omp_data;

  /* Functions created for each omp parallel directive found.  */
  VEC(tree,heap) *omp_fn_list;

};


/* Create a new temporary of type TYPE and prefix PREFIX as a local
   variable to function FN.  */

static tree
create_tmp_var_in (tree type, const char *prefix, tree fn)
{
  struct function *f = DECL_STRUCT_FUNCTION (fn);
  tree var = create_tmp_var_raw (type, prefix);
  DECL_CONTEXT (var) = fn;
  DECL_SEEN_IN_BIND_EXPR_P (var) = 1;
  f->unexpanded_var_list = tree_cons (NULL_TREE, var, f->unexpanded_var_list);

  return var;
}


/* Add a new field for VAR inside the structure RI_P->OMP_DATA.
   Associate the new field with VAR in RI_P->OMP_DATA_MAP.  */

static void
add_omp_data_field (tree var, struct remap_info_d *ri_p)
{
  tree field;
  splay_tree_node n;

  if (ri_p->omp_data == NULL_TREE)
    {
      /* Create an empty 'struct .omp_data_s'.  */
      tree struct_type = make_node (RECORD_TYPE);
      TYPE_NAME (struct_type) = get_identifier (".omp_data_s");

      /* Create the pointer type 'struct .omp_data_s *'.  */
      ri_p->ptr_to_omp_data = build_pointer_type (struct_type);

      /* Create 'struct .omp_data_s .omp_data', a local
	 structure in the parent thread.  Note that we cannot call
	 create_tmp_var here because the structure is still not
	 finished.  */
      ri_p->omp_data = create_tmp_var_raw (struct_type, ".omp_data");
      gimple_add_tmp_var (ri_p->omp_data);
    }

  /* If VAR already has a field in .OMP_DATA_S, return.  */
  n = splay_tree_lookup (ri_p->omp_data_map, (splay_tree_key) var);
  if (n)
    return;

  /* Add a new field for VAR inside struct .omp_data_s.  */
  field = build_decl (FIELD_DECL, DECL_NAME (var), TREE_TYPE (var));
  insert_field_into_struct (TREE_TYPE (ri_p->omp_data), field);

  /* Associate FIELD with VAR in RI_P->OMP_DATA_MAP so that we can
     create the proper dereferences later.  */
  splay_tree_insert (ri_p->omp_data_map,
		     (splay_tree_key) var,
		     (splay_tree_value) field);
}


/* Get the field inside RI_P->OMP_DATA that corresponds to VAR.
   Aborts if no associaton exists (the caller should've called
   add_omp_data_field first).  */

static tree
get_omp_data_field_for (tree var, struct remap_info_d *ri_p)
{
  splay_tree_node n;
  n = splay_tree_lookup (ri_p->omp_data_map, (splay_tree_key) var);
  gcc_assert (n);
  return (tree) n->value;
}


/* Prepare and return a private replacement for VAR.  The replacement
   is a new variable local to the child function RI_P->OMP_FN.  The
   caller should replace references of VAR with the new replacement
   inside RI_P->OMP_FN.  */

static tree
get_omp_private_repl (tree var, struct remap_info_d *ri_p)
{
  tree repl;
  splay_tree_node n;

  n = splay_tree_lookup (ri_p->local_map, (splay_tree_key) var);
  if (n == NULL)
    {
      repl = create_tmp_var_in (TREE_TYPE (var), NULL, ri_p->omp_fn);
      DECL_NAME (repl) = DECL_NAME (var);
      splay_tree_insert (ri_p->local_map,
			 (splay_tree_key) var,
			 (splay_tree_value) repl);
    }
  else
    repl = (tree) n->value;


  return repl;
}


/* Prepare a shared replacement for variable VAR, appropriate to be
   substituted in RI_P->OMP_FN.  This moves the whole variable inside
   .OMP_DATA.  The parent thread then passes the address of this
   structure onto the children threads.  For instance,

   		int i, j;
		struct A x;

		#pragma omp parallel shared (A, i, j)
		  {
		    ...
		  }

   The above snippet is lowered into:

   		struct .omp_data_s {
		  int i;
		  int j;
		  struct A x;
		} .omp_data_orig;

		GOMP_parallel_start (__omp_fn.9, &.omp_data_orig, num_threads);
		__omp_fn.9 (&.omp_data_orig);
		GOMP_parallel_end ();

   All the local variables that are marked shared are grouped inside a
   structure.  All the references to the original variables is
   converted to a structure reference and the address of that
   structure is passed on to the children threads.  */

static tree
get_omp_shared_repl (tree var, struct remap_info_d *ri_p)
{
  tree ptr_type, cast, field, repl;
  splay_tree_node n;

  n = splay_tree_lookup (ri_p->local_map, (splay_tree_key) var);
  if (n == NULL)
    {
      /* Add a new field in struct .omp_data_s to hold VAR.  */
      add_omp_data_field (var, ri_p);

      /* Mark the variable in RI_P->SHARED so that it can be
	 re-written in the body of the parent function.  */
      if (ri_p->shared == NULL)
	ri_p->shared = BITMAP_ALLOC (NULL);
      bitmap_set_bit (ri_p->shared, DECL_UID (var));

      /* Build and return the replacement
	 ((struct .omp_data_s *) data)->var.  */
      ptr_type = ri_p->ptr_to_omp_data;
      cast = fold_convert (ptr_type, DECL_ARGUMENTS (ri_p->omp_fn));
      field = get_omp_data_field_for (var, ri_p);
      repl = build (COMPONENT_REF, TREE_TYPE (var),
		    build (INDIRECT_REF, TREE_TYPE (ri_p->omp_data), cast),
		    field,
		    NULL_TREE);
      splay_tree_insert (ri_p->local_map,
			 (splay_tree_key) var,
			 (splay_tree_value) repl);
    }
  else
    repl = (tree) n->value;


  return repl;
}


/* Insert firstprivate sending code for variable VAR.  This assumes
   that there is a field VAR inside RI_P->OMP_DATA.  It generates the
   following assignment before RI_P->TSI: .omp_data.var = var.  */

static void
emit_firstprivate_sending_code (tree var, struct remap_info_d *ri_p)
{
  tree field, ref, init;
  
  field = get_omp_data_field_for (var, ri_p);
  ref = build (COMPONENT_REF, TREE_TYPE (field), ri_p->omp_data, field, 0);
  init = build (MODIFY_EXPR, void_type_node, ref, var);
  tsi_link_before (ri_p->tsi, init, TSI_SAME_STMT);
}


/* Insert firstprivate receiving code for variable VAR.  This assumes
   that there is a field VAR inside RI_P->OMP_DATA.  It generates the
   following assignment at the start of RI_P->OMP_FN:
   replacement_of(var) = ((struct .omp_data_s *) data)->var.  */

static void
emit_firstprivate_receiving_code (tree var, struct remap_info_d *ri_p)
{
  tree ptr_type, cast, field, t, body;
  tree_stmt_iterator child_tsi;

  /* Build ((struct .omp_data_s *) data)  */
  ptr_type = ri_p->ptr_to_omp_data;
  cast = fold_convert (ptr_type, DECL_ARGUMENTS (ri_p->omp_fn));

  /* Build ((struct .omp_data_s *) data)->var  */
  field = get_omp_data_field_for (var, ri_p);
  t = build (COMPONENT_REF, TREE_TYPE (var),
	     build (INDIRECT_REF, TREE_TYPE (ri_p->omp_data), cast),
	     field,
	     NULL_TREE);

  /* Build replacement_of(var) = ((struct .omp_data_s *) data)->var  */
  t = build (MODIFY_EXPR, void_type_node, get_omp_private_repl (var, ri_p), t);

  /* Insert at the start of RI_P->OMP_FN.  */
  body = BIND_EXPR_BODY (DECL_SAVED_TREE (ri_p->omp_fn));
  if (TREE_CODE (body) != STATEMENT_LIST)
    {
      tree list = alloc_stmt_list ();
      append_to_statement_list (body, &list);
      BIND_EXPR_BODY (DECL_SAVED_TREE (ri_p->omp_fn)) = list;
    }

  child_tsi = tsi_start (BIND_EXPR_BODY (DECL_SAVED_TREE (ri_p->omp_fn)));
  tsi_link_before (&child_tsi, t, TSI_NEW_STMT);
}


/* Prepare a firstprivate replacement for VAR.  This is similar to the
   private case, but the initial value of the replacement is sent via
   the DATA argument from the parent to the child thread.  So, given

   	#pragma omp parallel firstprivate (a)
	  {
	    ...
	  }

   we generate in the master thread:

	struct .omp_data_s { int a; } .omp_data;
   	.omp_data.a = a;
	GOMP_parallel_start (__omp_fn.3, &.omp_data, ...);
	__omp_fn.3 (&.omp_data);
	GOMP_parallel_end ();

   and in the child thread:

	__omp_fn.3 (void *data)
	{
	  int a = ((struct .omp_data_s *) data)->a;
	  ...
	}
   */

static void
get_omp_firstprivate_repl (tree var, struct remap_info_d *ri_p)
{
  /* Add a new field in struct .omp_data_s to hold VAR.  */
  add_omp_data_field (var, ri_p);

  /* Insert .omp_data_orig.var = var in the parent thread.  */
  emit_firstprivate_sending_code (var, ri_p);

  /* Insert repl = ((struct .omp_data_s *) data)->var in the child
     thread.  */
  emit_firstprivate_receiving_code (var, ri_p);
}


/* Insert lastprivate receiving code for variable VAR.  This assumes
   that there is a field VAR inside RI_P->OMP_DATA.  It generates the
   following assignment after the OpenMP parallel directive pointed by
   RI_P->TSI: var = ((struct .omp_data_s *) data)->var.  */

static void
emit_lastprivate_receiving_code (tree var, struct remap_info_d *ri_p)
{
  tree field, ref, init;
  
  field = get_omp_data_field_for (var, ri_p);
  ref = build (COMPONENT_REF, TREE_TYPE (field), ri_p->omp_data, field, 0);
  init = build (MODIFY_EXPR, void_type_node, var, ref);
  tsi_link_after (ri_p->tsi, init, TSI_SAME_STMT);
}


/* Insert lastprivate sending code for variable VAR in the workshare
   directive pointed by TSI, which can be either 'omp for' or 'omp
   sections'.  The assignment to be inserted is:
   ((struct .omp_data_s *) data)->var = var.

   For 'omp for' this should be computed for the last iteration of the
   construct.  For 'omp sections', it should go in the lexically last
   section construct.  */

static void
emit_lastprivate_sending_code (tree var, tree_stmt_iterator *tsi,
			       struct remap_info_d *ri_p)
{
  tree ptr_type, cast, field, copy, t;
  tree ws;

  /* Build ((struct .omp_data_s *) data)  */
  ptr_type = ri_p->ptr_to_omp_data;
  cast = fold_convert (ptr_type, DECL_ARGUMENTS (ri_p->omp_fn));

  /* Build ((struct .omp_data_s *) data)->var  */
  field = get_omp_data_field_for (var, ri_p);
  t = build (COMPONENT_REF, TREE_TYPE (var),
		  build (INDIRECT_REF, TREE_TYPE (ri_p->omp_data), cast),
		  field,
		  NULL_TREE);

  /* Build ((struct .omp_data_s *) data)->var = var  */
  copy = build (MODIFY_EXPR, void_type_node, t, var);

  /* Insert at the appropriate spot.  TSI points to the header of the
     workshare directive.  For 'omp for'

     	#pragma omp for
	for (VAR = N1; VAR COND N2; VAR = VAR OP INCR)

     Since we have to update at the last iteration, we emit a
     conditional that will only trigger for the unique thread that
     has VAR with a value larger (or smaller, depending on COND) than
     N2.  So,

     	if (VAR {>=, <=} N2)
	  ((struct .omp_data_s *) data)->var = var

     The conditional is >= if COND is < or <=, and <= if COND is > or
     >=.  */
  ws = tsi_stmt (*tsi);
  if (TREE_CODE (ws) == OMP_FOR)
    {
      tree cond = OMP_FOR_COND (ws);
      tree var = TREE_OPERAND (cond, 0);
      tree n2 = TREE_OPERAND (cond, 1);
      enum tree_code code = TREE_CODE (cond);

      if (code == LT_EXPR || code == LE_EXPR)
	code = GE_EXPR;
      else if (code == GT_EXPR || code == GE_EXPR)
	code = LT_EXPR;
      else
	gcc_unreachable ();

      t = build (code, boolean_type_node, var, unshare_expr (n2));
      t = build (COND_EXPR, void_type_node, t, copy, NULL_TREE);

      tsi_link_after (tsi, t, TSI_NEW_STMT);
    }
  else if (TREE_CODE (ws) == OMP_SECTIONS)
    {
      /* For 'omp sections', just insert at the end of the lexically
	 last section.  */
      tree_stmt_iterator last = tsi_last (OMP_SECTIONS_BODY (ws));
      tree last_sec = tsi_stmt (last);
      tree last_body = OMP_SECTION_BODY (last_sec);

      /* If the section consists of only one element, replace it with
	 a new statement list.  */
      if (TREE_CODE (last_body) != STATEMENT_LIST)
	{
	  tree list = alloc_stmt_list ();
	  append_to_statement_list (last_body, &list);
	  OMP_SECTION_BODY (last_sec) = list;
	}

      last = tsi_last (OMP_SECTION_BODY (last_sec));
      tsi_link_after (&last, copy, TSI_SAME_STMT);
    }
}


/* Prepare a lastprivate replacement for VAR.  This is similar to the
   firstprivate case, but the update process is reversed.  It is the
   child thread the one updating VAR in the parent.  

   	#pragma omp parallel firstprivate (a)
	  {
	    #pragma omp [for|sections]
	      ...
	  }

   we generate in the master thread:

	struct .omp_data_s { int a; } .omp_data;
	GOMP_parallel_start (__omp_fn.3, &.omp_data, ...);
	__omp_fn.3 (&.omp_data);
	GOMP_parallel_end ();
   	a = .omp_data.a;

   
   and in the child thread

	__omp_fn.3 (void *data)
	{
	  int a;
	  ...
	  ((struct .omp_data_s *) data)->a = a;
	}
   */

static void
get_omp_lastprivate_repl (tree var, tree_stmt_iterator *tsi,
			  struct remap_info_d *ri_p)
{
  get_omp_private_repl (var, ri_p);

  /* Add a new field in struct .omp_data_s to hold VAR.  */
  add_omp_data_field (var, ri_p);

  /* Insert .omp_data_orig.var = var in the child thread.  */
  emit_lastprivate_sending_code (var, tsi, ri_p);

  /* Insert repl = ((struct .omp_data_s *) data)->var in the parent
     thread.  */
  emit_lastprivate_receiving_code (var, ri_p);
}


/* Prepare a copyin replacement for VAR.  This is exactly like
   firstprivate, except that this is done on threadprivate variables,
   so there is no need to create a new local in the child thread.
   So, given

   	#pragma omp parallel copyin (a)
	  {
	    ...
	  }

   we generate in the master thread:

	struct .omp_data_s { int a; } .omp_data;
   	.omp_data.a = a;
	GOMP_parallel_start (__omp_fn.3, &.omp_data, ...);
	__omp_fn.3 (&.omp_data);
	GOMP_parallel_end ();

   and in the child thread:

	__omp_fn.3 (void *data)
	{
	  a = ((struct .omp_data_s *) data)->a;
	  ...
	}
   */

static void
get_omp_copyin_repl (tree var, struct remap_info_d *ri_p)
{
  /* Add a new field in struct .omp_data_s to hold VAR.  */
  add_omp_data_field (var, ri_p);

  /* Insert .omp_data_orig.var = var in the parent thread.  */
  emit_firstprivate_sending_code (var, ri_p);

  /* Insert replacement_of(var) = ((struct .omp_data_s *) data)->var
     in the child thread.  */
  emit_firstprivate_receiving_code (var, ri_p);
}


/* Prepare replacements for all the variables in CLAUSE, which is
   assumed to be one of the data sharing/copying clauses (private,
   shared, firstprivate, lastprivate, copyin or copyprivate).  TSI
   points to the directive being processed.  */

static void
get_omp_sharing_replacements (tree clause, tree_stmt_iterator *tsi,
			      struct remap_info_d *ri_p)
{
  tree t;

  for (t = TREE_OPERAND (clause, 0); t; t = TREE_CHAIN (t))
    {
      tree var = TREE_VALUE (t);

      if (TREE_CODE (clause) == OMP_CLAUSE_PRIVATE)
	get_omp_private_repl (var, ri_p);
      else if (TREE_CODE (clause) == OMP_CLAUSE_SHARED)
	get_omp_shared_repl (var, ri_p);
      else if (TREE_CODE (clause) == OMP_CLAUSE_FIRSTPRIVATE)
	get_omp_firstprivate_repl (var, ri_p);
      else if (TREE_CODE (clause) == OMP_CLAUSE_LASTPRIVATE)
	get_omp_lastprivate_repl (var, tsi, ri_p);
      else if (TREE_CODE (clause) == OMP_CLAUSE_REDUCTION)
	gcc_unreachable (); /*get_omp_reduction_repl (var, ri_p);*/
      else if (TREE_CODE (clause) == OMP_CLAUSE_COPYPRIVATE)
	gcc_unreachable (); /*get_omp_copyprivate_repl (var, ri_p);*/
      else if (TREE_CODE (clause) == OMP_CLAUSE_COPYIN)
	get_omp_copyin_repl (var, ri_p);
      else
	gcc_unreachable ();
    }
}


/* Process the list of data sharing and copying specified in CLAUSES.
   All the variables found in each clause are added to the
   corresponding sets in the remap info structure pointed by RI_P.
   TSI points to the directive being processed.  */

static void
process_omp_clauses (tree clauses, tree_stmt_iterator *tsi,
		     struct remap_info_d *ri_p)
{
  tree c;

  for (c = clauses; c; c = TREE_CHAIN (c))
    {
      tree clause = TREE_VALUE (c);

      if (TREE_CODE (clause) == OMP_CLAUSE_PRIVATE
	  || TREE_CODE (clause) == OMP_CLAUSE_SHARED
	  || TREE_CODE (clause) == OMP_CLAUSE_FIRSTPRIVATE
	  || TREE_CODE (clause) == OMP_CLAUSE_LASTPRIVATE
	  || TREE_CODE (clause) == OMP_CLAUSE_REDUCTION
	  || TREE_CODE (clause) == OMP_CLAUSE_COPYPRIVATE
	  || TREE_CODE (clause) == OMP_CLAUSE_COPYIN)
      get_omp_sharing_replacements (clause, tsi, ri_p);
    }
}


/* Build and create a data sharing description structure to guide the
   remapping actions.  */

static struct remap_info_d *
build_remap_info (void)
{
  struct remap_info_d *ri_p;

  ri_p = xmalloc (sizeof (*ri_p));
  memset (ri_p, 0, sizeof (*ri_p));

  ri_p->omp_data_map = splay_tree_new (splay_tree_compare_pointers, 0, 0);
  ri_p->local_map = splay_tree_new (splay_tree_compare_pointers, 0, 0);

  return ri_p;
}


/* Free memory used by the data sharing structure RI_P.  */

static void
delete_remap_info (struct remap_info_d *ri_p)
{
  splay_tree_delete (ri_p->omp_data_map);
  splay_tree_delete (ri_p->local_map);
  BITMAP_FREE (ri_p->shared);
  VEC_free (tree, heap, ri_p->omp_fn_list);
  free (ri_p);
}


/* Callback for walk_stmts to change the context of all the local
   variables found in *TP accordingly to what is described by the
   sets in DATA.  This makes the replacements required by children
   threads.  */

static tree
remap_locals_in_child_r (tree *tp, int *ws, void *data)
{
  struct walk_stmt_info *wi = data;
  struct remap_info_d *ri_p = wi->info;
  tree t, omp_fn;
  enum tree_code code;

  omp_fn = ri_p->omp_fn;

  t = *tp;
  code = TREE_CODE (t);

  /* Remap locals in the parent function into the child function.
     Skip any symbols that we may have already generated with the
     correct scope.  */
  if ((code == PARM_DECL || code == VAR_DECL)
      && !is_global_var (t)
      && DECL_CONTEXT (t) != omp_fn)
    {
      /* Get a shared or private replacement accordingly.  */
      if (ri_p->shared
	  && bitmap_bit_p (ri_p->shared, DECL_UID (t)))
	*tp = get_omp_shared_repl (t, ri_p);
      else
	*tp = get_omp_private_repl (t, ri_p);
    }
  else if (code == LABEL_DECL)
    DECL_CONTEXT (t) = omp_fn;
  else if (code == OMP_PARALLEL
	   || code == OMP_CLAUSE_PRIVATE
	   || code == OMP_CLAUSE_SHARED
	   || code == OMP_CLAUSE_FIRSTPRIVATE
	   || code == OMP_CLAUSE_LASTPRIVATE
	   || code == OMP_CLAUSE_REDUCTION
	   || code == OMP_CLAUSE_COPYIN
	   || code == OMP_CLAUSE_COPYPRIVATE)
    {
      /* Variables may appear inside other clauses in nested OpenMP
	 directives.  Avoid rewriting nested references because they
	 need to refer to the original _DECL.  */
      *ws = 0;
    }
  else if (code == OMP_FOR)
    process_omp_clauses (OMP_FOR_CLAUSES (t), &wi->tsi, ri_p);
  else if (code == OMP_SECTIONS)
    process_omp_clauses (OMP_SECTIONS_CLAUSES (t), &wi->tsi, ri_p);

  return NULL_TREE;
}


/* Build and return a new FUNCTION_DECL to hold the body of the OpenMP
   directive OMP_EXPR.  */

static tree
create_omp_fn (tree omp_expr)
{
  tree fn_name, fn_type, fn_body, fn_decl, res_decl;
  tree fn_data_arg;

  /* Enclose the body in a BIND_EXPR, if it doesn't have one already.  */
  fn_body = OMP_PARALLEL_BODY (omp_expr);
  if (TREE_CODE (fn_body) != BIND_EXPR)
    {
      fn_body = build3 (BIND_EXPR, void_type_node, NULL, fn_body, NULL);
      TREE_SIDE_EFFECTS (fn_body) = 1;
    }

  /* Build the declaration of the new function.  */
  fn_name = create_tmp_var_name ("__omp_fn");
  fn_type = build_function_type_list (void_type_node, ptr_type_node, NULL_TREE);
  fn_decl = build_fn_decl (IDENTIFIER_POINTER (fn_name), fn_type);
  res_decl = build_decl (RESULT_DECL, NULL_TREE, void_type_node);

  /* Initialize attributes for the result and the function.  */
  DECL_ARTIFICIAL (res_decl) = 1;
  DECL_IGNORED_P (res_decl) = 1;
  DECL_RESULT (fn_decl) = res_decl;

  TREE_STATIC (fn_decl) = 0;
  TREE_USED (fn_decl) = 1;
  DECL_ARTIFICIAL (fn_decl) = 1;
  DECL_IGNORED_P (fn_decl) = 0;
  TREE_PUBLIC (fn_decl) = 0;
  DECL_UNINLINABLE (fn_decl) = 1;
  DECL_EXTERNAL (fn_decl) = 0;
  DECL_CONTEXT (fn_decl) = NULL_TREE;

  DECL_INITIAL (fn_decl) = make_node (BLOCK);
  TREE_USED (DECL_INITIAL (fn_decl)) = 1;
  BLOCK_SUPERCONTEXT (DECL_INITIAL (fn_decl)) = fn_decl;
  gcc_assert (TREE_CODE (fn_body) == BIND_EXPR);
  DECL_SAVED_TREE (fn_decl) = fn_body;

  /* Add the argument DATA.  */
  fn_data_arg = build_decl (PARM_DECL, get_identifier ("data"), ptr_type_node);
  DECL_ARG_TYPE (fn_data_arg) = TREE_TYPE (fn_data_arg);
  DECL_CONTEXT (fn_data_arg) = fn_decl;
  TREE_USED (fn_data_arg) = 1;
  DECL_ARGUMENTS (fn_decl) = fn_data_arg;

  return fn_decl;
}


/* Build and return the call GOMP_parallel_start (FN, DATA, NUM_THREADS).  */

static tree
create_gomp_parallel_start (tree fn, tree data, tree num_threads)
{
  tree lib_fn, args, type;

  gcc_assert (TREE_CODE (fn) == FUNCTION_DECL);

  type = build_function_type_list (void_type_node,
				   build_pointer_type (TREE_TYPE (fn)),
				   ptr_type_node,
				   unsigned_type_node,
				   NULL_TREE);

  fn = build_addr (fn, current_function_decl);
  lib_fn = build_fn_decl ("GOMP_parallel_start", type);
  args = tree_cons (NULL_TREE, fn,
		    tree_cons (NULL_TREE, data,
			       tree_cons (NULL_TREE, num_threads,
					  NULL_TREE)));

  return build_function_call_expr (lib_fn, args);
}


/* Build and return a call to GOMP_parallel_end().  */

static tree
create_gomp_parallel_end (void)
{
  tree fn, type;

  type = build_function_type_list (void_type_node, void_type_node, NULL_TREE);
  fn = build_fn_decl ("GOMP_parallel_end", type);
  return build_function_call_expr (fn, NULL_TREE);
}


/* Return a GIMPLE value representing EXP.  If necessary, insert a new
   assignment before TSI to create a new temporary to hold EXP (the
   iterator TSI is not affected by this insertion).

   If EXP is already a GIMPLE value its returned, otherwise the new
   temporary holding EXP is returned.  */

static tree
convert_to_gimple_val (tree exp, tree_stmt_iterator *tsi)
{
  tree tmp, stmt;

  if (is_gimple_val (exp))
    return exp;

  tmp = create_tmp_var (TREE_TYPE (exp), NULL);
  stmt = build (MODIFY_EXPR, void_type_node, tmp, exp);
  SET_EXPR_LOCUS (stmt, EXPR_LOCUS (tsi_stmt (*tsi)));
  tsi_link_before (tsi, stmt, TSI_SAME_STMT);

  return tmp;
}


/* Callback for walk_stmts to change the context of all the local
   variables found in *TP accordingly to what is described by the
   sets in DATA.
   
   This makes the replacements required by the parent thread.  It's
   mostly modelled after convert_nonlocal_reference, which needs to do
   similar rewriting when unnesting functions.  */

static tree
remap_locals_in_parent_r (tree *tp, int *ws, void *data)
{
  struct walk_stmt_info *wi = data;
  struct remap_info_d *ri_p = wi->info;
  tree t = *tp;

  switch (TREE_CODE (t))
    {
    case VAR_DECL:
    case PARM_DECL:
      if (!is_global_var (t)
	  && ri_p->shared
	  && bitmap_bit_p (ri_p->shared, DECL_UID (t)))
	{
	  tree r, field;

	  /* If the replacement needs to be a GIMPLE value, assign it
	     into a new temporary before TSI.  */
	  field = get_omp_data_field_for (t, ri_p);
	  r = build (COMPONENT_REF, TREE_TYPE (t), ri_p->omp_data, field, 0);

	  if (wi->val_only)
	    *tp = convert_to_gimple_val (r, &wi->tsi);
	  else
	    *tp = r;

	  /* We don't want to recurse into the replacement we just added.  */
	  *ws = 0;
	}
      break;

    case ADDR_EXPR:
      {
	bool save_val_only = wi->val_only;

	wi->val_only = false;
	wi->is_lhs = false;
	wi->changed = false;
	walk_tree (&TREE_OPERAND (t, 0), remap_locals_in_parent_r, wi, NULL);
	wi->val_only = true;

	if (wi->changed)
	  {
	    /* If we changed anything, then TREE_INVARIANT may be
	       wrong, since we're no longer directly referencing a
	       decl.  */
	    recompute_tree_invarant_for_addr_expr (t);

	    /* If the callback converted the address argument in a
	       context where we only accept variables (and
	       min_invariant, presumably), then compute the address
	       into a temporary.  */
	    if (save_val_only)
	      *tp = convert_to_gimple_val (t, &wi->tsi);
	  }
      }
      break;

    case REALPART_EXPR:
    case IMAGPART_EXPR:
    case COMPONENT_REF:
    case ARRAY_REF:
    case ARRAY_RANGE_REF:
    case BIT_FIELD_REF:
      /* Go down this entire nest and just look at the final prefix
	 and anything that describes the references.  Otherwise, we
	 lose track of whether a NOP_EXPR or VIEW_CONVERT_EXPR needs a
	 simple value.  */
      wi->val_only = true;
      wi->is_lhs = false;
      for (; handled_component_p (t); tp = &TREE_OPERAND (t, 0), t = *tp)
	{
	  if (TREE_CODE (t) == COMPONENT_REF)
	    walk_tree (&TREE_OPERAND (t, 2), remap_locals_in_parent_r, wi,
		       NULL);
	  else if (TREE_CODE (t) == ARRAY_REF
		   || TREE_CODE (t) == ARRAY_RANGE_REF)
	    {
	      walk_tree (&TREE_OPERAND (t, 1), remap_locals_in_parent_r, wi,
			 NULL);
	      walk_tree (&TREE_OPERAND (t, 2), remap_locals_in_parent_r, wi,
			 NULL);
	      walk_tree (&TREE_OPERAND (t, 3), remap_locals_in_parent_r, wi,
			 NULL);
	    }
	  else if (TREE_CODE (t) == BIT_FIELD_REF)
	    {
	      walk_tree (&TREE_OPERAND (t, 1), remap_locals_in_parent_r, wi,
			 NULL);
	      walk_tree (&TREE_OPERAND (t, 2), remap_locals_in_parent_r, wi,
			 NULL);
	    }
	}
      wi->val_only = false;
      walk_tree (tp, remap_locals_in_parent_r, wi, NULL);
      break;

    case OMP_PARALLEL:
    case OMP_FOR:
    case OMP_SECTIONS:
      /* No need to handle these here.  */
      *ws = 0;
      break;

    default:
      if (!IS_TYPE_OR_DECL_P (t))
	{
	  *ws = 1;
          wi->val_only = true;
	  wi->is_lhs = false;
	}
      break;
    }

  return NULL_TREE;
}


/* Emit code to specify the number of threads to use in the
   runtime.

   RI_P points to the remap structure holding clause information.

   Returns the argument that should be passed to GOMP_parallel_start.  */

static tree
emit_num_threads_setup_code (tree clauses, struct remap_info_d *ri_p)
{
  tree val, cond, c;

  /* By default, the value of NUM_THREADS is zero (selected at run time)
     and there is no conditional.  */
  cond = NULL_TREE;
  val = build_int_cst (unsigned_type_node, 0);

  for (c = clauses; c; c = TREE_CHAIN (c))
    {
      tree clause = TREE_VALUE (c);

      if (TREE_CODE (clause) == OMP_CLAUSE_IF)
	cond = OMP_IF_EXPR (clause);
      else if (TREE_CODE (clause) == OMP_CLAUSE_NUM_THREADS)
	val = OMP_NUM_THREADS_EXPR (clause);
    }

  /* If we found either of 'if (expr)' or 'num_threads (expr)', create a
     local variable and prepare a list of statements to be inserted.  */
  if (cond
      || !is_gimple_val (val)
      || !lang_hooks.types_compatible_p (unsigned_type_node, TREE_TYPE (val)))
    {
      tree stmt_list = NULL, num_threads;
      enum gimplify_status gs;

      /* Ensure 'val' is of the correct type.  */
      val = fold_convert (unsigned_type_node, val);

      /* If we found the clause 'if (cond)', build either
	 (unsigned int)(cond) or (cond ? val : 1u).  */
      if (cond)
	{
	  if (integer_zerop (val))
	    val = fold_convert (unsigned_type_node, invert_truthvalue (cond));
	  else
	    val = build3 (COND_EXPR, unsigned_type_node, cond, val,
			  build_int_cst (unsigned_type_node, 1));
	}

      /* Gimplify the emitted code.  This is necessary mostly
	 for COND and VAL, which can be arbitrary expressions.  */
      push_gimplify_context ();
      num_threads = val;
      gs = gimplify_expr (&num_threads, &stmt_list, NULL,
			  is_gimple_val, fb_rvalue);
      pop_gimplify_context (NULL);

      if (stmt_list)
	tsi_link_after (ri_p->tsi, stmt_list, TSI_CONTINUE_LINKING);

      if (gs == GS_ALL_DONE)
	val = num_threads;
      else
	val = build_int_cst (unsigned_type_node, 0);
    }

  return val;
}


/* Lower the OpenMP parallel directive pointed by TSI.  Build a new
   function with the body of the pragma and emit the appropriate
   runtime call.  RI_P points to the remap structure holding clause
   information.  */

static void
lower_omp_parallel (tree_stmt_iterator *tsi, struct remap_info_d *ri_p)
{
  tree par_stmt, call, args, num_threads, addr_data_arg, clauses;
  tree_stmt_iterator orig_tsi;
  struct function *saved_cfun;
  struct walk_stmt_info wi;

  orig_tsi = *tsi;
  par_stmt = tsi_stmt (*tsi);
  clauses = OMP_PARALLEL_CLAUSES (par_stmt);

  /* Build a new function out of the pragma's body.  */
  ri_p->omp_fn = create_omp_fn (par_stmt);
  ri_p->tsi = tsi;

  /* Allocate memory for the function structure.  We need to
     save/restore CFUN because it gets clobbered by the call to
     allocate_struct_function.  */
  saved_cfun = cfun;
  allocate_struct_function (ri_p->omp_fn);
  DECL_SOURCE_LOCATION (ri_p->omp_fn) = input_location;
  cfun->function_end_locus = input_location;
  cfun = saved_cfun;

  /* Process the clauses in the header of the omp parallel
     directive.  */
  process_omp_clauses (clauses, ri_p->tsi, ri_p);

  /* Remap local variables according to the data clauses.  This must
     be done before gimplification, because we can only gimplify the
     function after adjusting labels and local variable references.  */
  wi.callback = remap_locals_in_child_r;
  wi.info = ri_p;
  wi.tsi = *tsi;
  walk_stmts (&wi, &DECL_SAVED_TREE (ri_p->omp_fn));

  /* Once remapped, remove all the mappings.  */
  splay_tree_delete (ri_p->local_map);
  ri_p->local_map = splay_tree_new (splay_tree_compare_pointers, 0, 0);

  /* Add RI_P->OMP_FN to the call graph and to the list of functions
     to gimplify once RI_P->OMP_DATA has been finalized.  */
  cgraph_add_new_function (ri_p->omp_fn);
  VEC_safe_push (tree, heap, ri_p->omp_fn_list, ri_p->omp_fn);

  /* Take the address of RI_P->OMP_DATA to pass to __omp_fn.XXXX.  */
  if (ri_p->omp_data)
    {
      addr_data_arg = build1 (ADDR_EXPR, ri_p->ptr_to_omp_data, ri_p->omp_data);
      TREE_ADDRESSABLE (ri_p->omp_data) = 1;
    }
  else
    addr_data_arg = null_pointer_node;

  /* Emit code to set up the number of threads to use according to the
     IF and NUM_THREADS clauses.  If both are missing, set it to 0 so
     that it is dynamically selected by the runtime.  */
  num_threads = emit_num_threads_setup_code (clauses, ri_p);

  /* Emit GOMP_parallel_start (__omp_fn.XXXX ...).  */
  call = create_gomp_parallel_start (ri_p->omp_fn, addr_data_arg, num_threads);
  tsi_link_after (tsi, call, TSI_CONTINUE_LINKING);

  /* Emit __omp_fn.XXXX (&omp_data).  */
  args = tree_cons (NULL_TREE, unshare_expr (addr_data_arg), NULL_TREE);
  call = build_function_call_expr (ri_p->omp_fn, args);
  tsi_link_after (tsi, call, TSI_NEW_STMT);

  /* Emit GOMP_parallel_end ().  */
  call = create_gomp_parallel_end ();
  tsi_link_after (tsi, call, TSI_NEW_STMT);

  /* Remove the original statement.  */
  tsi_delink (&orig_tsi);
}


/* Callback for walk_stmts to lower all the unnested omp parallel
   directives in the code.  */

static tree
lower_omp_parallel_r (tree *tp, int *ws, void *data)
{
  struct walk_stmt_info *wi = data;
  struct remap_info_d *ri_p = wi->info;
  tree t = *tp;

  if (TREE_CODE (t) == OMP_PARALLEL)
    {
      /* Lower the omp parallel directive.  */
      lower_omp_parallel (&wi->tsi, ri_p);

      /* Do not walk inside the body of the directive.  */
      *ws = 0;
    }

  return NULL_TREE;
}


/* Lower OpenMP directives in CURRENT_FUNCTION_DECL.  The lowering
   process consists of two phases:
   
   1- Walk the function body lowering all the omp parallel
      directives and processing their clauses.  

      This will create the .OMP_DATA_S structure with fields for every
      variable mentioned in clauses that need data sharing and/or
      copying between the parent and children threads.

      This also creates a new function __omp_fn.XX for the body of the
      directive.  The variables referenced in that body are then
      remapped using data sharing/copying clause information.  Any
      worksharing constructs inside the body may also contribute new
      fields to the structure .OMP_DATA_S created in #1.

   2- All the references to shared variables in the parent function
      are replaced with the appropriate structure access in
      .OMP_DATA_S.  */

static void
lower_omp (void)
{
  struct walk_stmt_info wi;
  struct remap_info_d *ri_p;
  tree t, body;
  unsigned ix;

  ri_p = build_remap_info ();

  body = DECL_SAVED_TREE (current_function_decl);
  gcc_assert (TREE_CODE (body) == BIND_EXPR);

  /* 1- Collect all the unnested parallel directives.  We are not
	interested in processing nested directives here.  The strategy
	is to let them go unchanged into the child __omp_fn.XX thread
	and then lower them when compiling that function.  */
  wi.callback = lower_omp_parallel_r;
  wi.info = ri_p;
  walk_stmts (&wi, &DECL_SAVED_TREE (current_function_decl));
  
  /* 2- Remap shared variables in the parent function.  */
  wi.callback = remap_locals_in_parent_r;
  wi.info = ri_p;
  walk_stmts (&wi, &DECL_SAVED_TREE (current_function_decl));

  /* Once we have remapped all the locals, we can layout the
     structure type for RI_P->OMP_DATA.  */
  if (ri_p->omp_data)
    {
      layout_type (TREE_TYPE (ri_p->omp_data));
      layout_type (ri_p->ptr_to_omp_data);
      layout_decl (ri_p->omp_data, 0);
    }

  /* Once RI_P->OMP_DATA has been finalized, we can gimplify all the
     function bodies we added in lower_omp_parallel.  */
  for (ix = 0; VEC_iterate (tree, ri_p->omp_fn_list, ix, t); ix++)
    gimplify_function_tree (t);

  /* Free allocated memory.  */
  delete_remap_info (ri_p);
}


static bool
gate_lower_omp (void)
{
  return flag_openmp != 0;
}


struct tree_opt_pass pass_lower_omp = 
{
  "omplower",				/* name */
  gate_lower_omp,			/* gate */
  lower_omp,				/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_gimple_any,			/* properties_required */
  PROP_gimple_lomp,			/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func,			/* todo_flags_finish */
  0					/* letter */
};
