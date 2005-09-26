/* Tree lowering pass.  Lowers GIMPLE into unstructured form.

   Copyright (C) 2003, 2005 Free Software Foundation, Inc.

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

struct lower_data
{
  /* Block the current statement belongs to.  */
  tree block;

  /* A TREE_LIST of label and return statements to be moved to the end
     of the function.  */
  tree return_statements;
};

static void lower_stmt (tree_stmt_iterator *, struct lower_data *);
static void lower_bind_expr (tree_stmt_iterator *, struct lower_data *);
static void lower_cond_expr (tree_stmt_iterator *, struct lower_data *);
static void lower_return_expr (tree_stmt_iterator *, struct lower_data *);
static bool expand_var_p (tree);

/* Lowers the body of current_function_decl.  */

static void
lower_function_body (void)
{
  struct lower_data data;
  tree *body_p = &DECL_SAVED_TREE (current_function_decl);
  tree bind = *body_p;
  tree_stmt_iterator i;
  tree t, x;

  gcc_assert (TREE_CODE (bind) == BIND_EXPR);

  memset (&data, 0, sizeof (data));
  data.block = DECL_INITIAL (current_function_decl);
  BLOCK_SUBBLOCKS (data.block) = NULL_TREE;
  BLOCK_CHAIN (data.block) = NULL_TREE;
  TREE_ASM_WRITTEN (data.block) = 1;

  *body_p = alloc_stmt_list ();
  i = tsi_start (*body_p);
  tsi_link_after (&i, bind, TSI_NEW_STMT);
  lower_bind_expr (&i, &data);

  i = tsi_last (*body_p);

  /* If the function falls off the end, we need a null return statement.
     If we've already got one in the return_statements list, we don't
     need to do anything special.  Otherwise build one by hand.  */
  if (block_may_fallthru (*body_p)
      && (data.return_statements == NULL
          || TREE_OPERAND (TREE_VALUE (data.return_statements), 0) != NULL))
    {
      x = build (RETURN_EXPR, void_type_node, NULL);
      SET_EXPR_LOCATION (x, cfun->function_end_locus);
      tsi_link_after (&i, x, TSI_CONTINUE_LINKING);
    }

  /* If we lowered any return statements, emit the representative
     at the end of the function.  */
  for (t = data.return_statements ; t ; t = TREE_CHAIN (t))
    {
      x = build (LABEL_EXPR, void_type_node, TREE_PURPOSE (t));
      tsi_link_after (&i, x, TSI_CONTINUE_LINKING);

      /* Remove the line number from the representative return statement.
	 It now fills in for many such returns.  Failure to remove this
	 will result in incorrect results for coverage analysis.  */
      x = TREE_VALUE (t);
#ifdef USE_MAPPED_LOCATION
      SET_EXPR_LOCATION (x, UNKNOWN_LOCATION);
#else
      SET_EXPR_LOCUS (x, NULL);
#endif
      tsi_link_after (&i, x, TSI_CONTINUE_LINKING);
    }

  gcc_assert (data.block == DECL_INITIAL (current_function_decl));
  BLOCK_SUBBLOCKS (data.block)
    = blocks_nreverse (BLOCK_SUBBLOCKS (data.block));

  clear_block_marks (data.block);
}

struct tree_opt_pass pass_lower_cf = 
{
  "lower",				/* name */
  NULL,					/* gate */
  lower_function_body,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_gimple_any,			/* properties_required */
  PROP_gimple_lcf,			/* properties_provided */
  PROP_gimple_any,			/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func,			/* todo_flags_finish */
  0					/* letter */
};


/* Lowers the EXPR.  Unlike gimplification the statements are not relowered
   when they are changed -- if this has to be done, the lowering routine must
   do it explicitly.  DATA is passed through the recursion.  */

static void
lower_stmt_body (tree expr, struct lower_data *data)
{
  tree_stmt_iterator tsi;

  for (tsi = tsi_start (expr); !tsi_end_p (tsi); )
    lower_stmt (&tsi, data);
}


/* Data structure used for implementing all the data sharing/copying
   clauses.  */

struct remap_info_d
{
  /* Mapping between local variables in the original function and the
     new function containing the body of the OpenMP directive.  */
  splay_tree map;

  /* Function holding the body of the OpenMP directive.  */
  tree omp_fn;

  /* Iterator where to insert code in the parent function.  Initially,
     this points at the OpenMP directive being lowered.  */
  tree_stmt_iterator *tsi;

  /* Data sharing and copying clauses associated with this directive.  */
  tree clauses;

  /* Set of local variables to be shared among the threads.  */
  bitmap shared;

  /* Structure holding addresses of shared and copy-in data to be
     passed to the children threads.  This is declared in the parent
     thread.  */
  tree omp_data;

  /* Pointer type to OMP_DATA.  */
  tree ptr_to_omp_data;

  /* Symbols that have DECL_VALUE_EXPR set.  */
  VEC(tree,heap) *syms_with_value_expr;
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
   Associate the new field with VAR in RI_P->MAP.  */

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

  /* Add a new field for VAR inside struct .omp_data_s.  */
  field = build_decl (FIELD_DECL, DECL_NAME (var), TREE_TYPE (var));
  insert_field_into_struct (TREE_TYPE (ri_p->omp_data), field);

  /* Associate FIELD with VAR in RI_P->MAP so that we can create the
     proper dereferences later.  */
  n = splay_tree_lookup (ri_p->map, (splay_tree_key) var);
  gcc_assert (n == NULL);
  splay_tree_insert (ri_p->map, (splay_tree_key) var, (splay_tree_value) field);
}


/* Get the field inside RI_P->OMP_DATA that corresponds to VAR.
   Aborts if no associaton exists (the caller should've called
   add_omp_data_field first).  */

static tree
get_omp_data_field_for (tree var, struct remap_info_d *ri_p)
{
  splay_tree_node n = splay_tree_lookup (ri_p->map, (splay_tree_key) var);
  gcc_assert (n);
  return (tree) n->value;
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
   var = ((struct .omp_data_s *) data)->var.  */

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

  /* Build var = ((struct .omp_data_s *) data)->var  */
  t = build (MODIFY_EXPR, void_type_node, var, t);

  /* Insert at the start of RI_P->OMP_FN.  */
  body = BIND_EXPR_BODY (DECL_SAVED_TREE (ri_p->omp_fn));
  child_tsi = tsi_start (body);
  tsi_link_before (&child_tsi, t, TSI_NEW_STMT);
}


/* Prepare a private replacement for VAR.  This simply creates a
   variable local to the child thread function RI_P->OMP_FN and sets
   VAR's DECL_VALUE_EXPR to be the new local.  When RI_P->OMP_FN is
   gimplified, all references to VAR will be replaced with the new
   local.  */

static tree
get_omp_private_repl (tree var, struct remap_info_d *ri_p)
{
  tree repl;

  if (DECL_HAS_VALUE_EXPR_P (var))
    return DECL_VALUE_EXPR (var);

  repl = create_tmp_var_in (TREE_TYPE (var), NULL, ri_p->omp_fn);

  DECL_NAME (repl) = DECL_NAME (var);
  SET_DECL_VALUE_EXPR (var, repl);
  DECL_HAS_VALUE_EXPR_P (var) = 1;
  VEC_safe_push (tree, heap, ri_p->syms_with_value_expr, var);

  return repl;
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
  get_omp_private_repl (var, ri_p);

  /* Add a new field in struct .omp_data_s to hold VAR.  */
  add_omp_data_field (var, ri_p);

  /* Insert .omp_data_orig.var = var in the parent thread.  */
  emit_firstprivate_sending_code (var, ri_p);

  /* Insert repl = ((struct .omp_data_s *)data)->var in the child
     thread.  */
  emit_firstprivate_receiving_code (var, ri_p);
}


/* Prepare a shared replacement for variable VAR.  This moves the
   whole variable inside .OMP_DATA.  The parent thread then passes the
   address of this structure onto the children threads.  For instance,

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

static void
get_omp_shared_repl (tree var, struct remap_info_d *ri_p)
{
  tree ptr_type, cast, field, repl;

  if (DECL_HAS_VALUE_EXPR_P (var))
    return;

  /* Add a new field in struct .omp_data_s to hold VAR.  */
  add_omp_data_field (var, ri_p);

  /* Mark the variable in RI_P->SHARED so that it can be re-written in
     the body of the parent function.  */
  if (ri_p->shared == NULL)
    ri_p->shared = BITMAP_ALLOC (NULL);
  bitmap_set_bit (ri_p->shared, DECL_UID (var));

  /* When the child thread RI_P->OMP_FN is gimplified, all references
     to VAR will be converted to ((struct .omp_data_s *)data)->var.  */
  ptr_type = ri_p->ptr_to_omp_data;
  cast = fold_convert (ptr_type, DECL_ARGUMENTS (ri_p->omp_fn));
  field = get_omp_data_field_for (var, ri_p);
  repl = build (COMPONENT_REF, TREE_TYPE (var),
		build (INDIRECT_REF, TREE_TYPE (ri_p->omp_data), cast),
		field,
		NULL_TREE);

  SET_DECL_VALUE_EXPR (var, repl);
  DECL_HAS_VALUE_EXPR_P (var) = 1;
  VEC_safe_push (tree, heap, ri_p->syms_with_value_expr, var);
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

  /* Insert repl = ((struct .omp_data_s *)data)->var in the parent
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

  /* Insert repl = ((struct .omp_data_s *)data)->var in the child
     thread.  */
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

  /* FIXME.  Add checking code to disallow variables in multiple clauses.
     Variables may only appear in exactly one clause, except for
     firstprivate and lastprivate.  */
}


/* Build and create a data sharing description structure to guide the
   remapping actions done in remap_locals_in_child_r.  OMP_FN is the
   function holding the body of the directive to be processed.
   CLAUSES is the list of clauses controlling data sharing and
   copying.  TSI points to the OpenMP directive being lowered.  */

static struct remap_info_d *
build_remap_info (tree omp_fn, tree clauses, tree_stmt_iterator *tsi)
{
  struct remap_info_d *ri_p;

  /* Setup the mapping data.  */
  ri_p = xmalloc (sizeof (*ri_p));
  memset (ri_p, 0, sizeof (*ri_p));
  ri_p->omp_fn = omp_fn;
  ri_p->clauses = clauses;
  ri_p->map = splay_tree_new (splay_tree_compare_pointers, 0, 0);
  ri_p->tsi = tsi;
  ri_p->syms_with_value_expr = NULL;

  return ri_p;
}


/* Free memory used by the data sharing structure RI_P.  */

static void
delete_remap_info (struct remap_info_d *ri_p)
{
  splay_tree_delete (ri_p->map);
  BITMAP_FREE (ri_p->shared);
  VEC_free (tree, heap, ri_p->syms_with_value_expr);
  free (ri_p);
}


/* Callback for walk_tree to change the context of all the local
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

  if ((code == PARM_DECL || code == VAR_DECL)
      && DECL_CONTEXT (t) != omp_fn
      && !is_global_var (t))
    {
      /* Remap locals in the parent function into the child function.
	 Skip any symbols that we may have already generated with the
	 correct scope.  By default, ask the FE what the implicit
	 sharing rules should be.  FIXME, add langhooks.  Currently,
	 this just uses C/C++ rules (every local is private).  */
      if (!DECL_VALUE_EXPR (t))
	*tp = get_omp_private_repl (t, ri_p);
      else
	*tp = DECL_VALUE_EXPR (t);
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



/* Map local variables from the current function into RI_P->OMP_FN.
   RI_P points to an instance of struct remap_info_d describing how
   local variables should be mapped into the new function.  */

static void
remap_locals_in_child (struct remap_info_d *ri_p)
{
  struct walk_stmt_info wi;

  /* Process the clauses in the header of the omp parallel directive.  */
  process_omp_clauses (ri_p->clauses, ri_p->tsi, ri_p);

  /* Walk all statements in RI_P->OMP_FN doing the mappings.  */
  wi.callback = remap_locals_in_child_r;
  wi.info = ri_p;
  walk_stmts (&wi, &DECL_SAVED_TREE (ri_p->omp_fn));

  /* Once we have remapped all the locals, we can layout the
     structure type for RI_P->OMP_DATA.  */
  if (ri_p->omp_data)
    {
      layout_type (TREE_TYPE (ri_p->omp_data));
      layout_type (ri_p->ptr_to_omp_data);
      layout_decl (ri_p->omp_data, 0);
    }
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


/* Callback for walk_tree to change the context of all the local
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

   DATA points to GIMPLE lowering information.

   Returns the argument that should be passed to GOMP_parallel_start.  */

static tree
emit_num_threads_setup_code (struct remap_info_d *ri_p, struct lower_data *data)
{
  tree val, cond, c;
  tree clauses = ri_p->clauses;

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

      /* Gimplify and lower the emitted code.  This is necessary mostly
	 for COND and VAL, which can be arbitrary expressions.  */
      push_gimplify_context ();
      num_threads = val;
      gs = gimplify_expr (&num_threads, &stmt_list, NULL,
			  is_gimple_val, fb_rvalue);
      pop_gimplify_context (NULL);

      if (stmt_list)
	{
	  lower_stmt_body (stmt_list, data);
	  tsi_link_after (ri_p->tsi, stmt_list, TSI_CONTINUE_LINKING);
	}

      if (gs == GS_ALL_DONE)
	val = num_threads;
      else
	val = build_int_cst (unsigned_type_node, 0);
    }

  return val;
}


/* Lower the OpenMP parallel directive pointed by TSI.  Build a new
   function with the body of the pragma and emit the appropriate
   runtime call.  DATA contains locus and scope information for TSI.  */

static void
lower_omp_parallel (tree_stmt_iterator *tsi, struct lower_data *data)
{
  tree t, par_stmt, fn, call, args, num_threads, addr_data_arg;
  tree_stmt_iterator orig_tsi;
  struct remap_info_d *ri_p;
  struct function *saved_cfun;
  struct walk_stmt_info wi;
  unsigned ix;

  orig_tsi = *tsi;
  par_stmt = tsi_stmt (*tsi);

  /* Build a new function out of the pragma's body.  */
  fn = create_omp_fn (par_stmt);

  /* Allocate memory for the function structure.  We need to
     save/restore CFUN because it gets clobbered by the call to
     allocate_struct_function.  */
  saved_cfun = cfun;
  allocate_struct_function (fn);
  DECL_SOURCE_LOCATION (fn) = input_location;
  cfun->function_end_locus = input_location;
  cfun = saved_cfun;

  /* Remap local variables according to the data clauses.  This must
     be done before gimplification, because we can only gimplify the
     function after adjusting labels and local variable references.  */
  ri_p = build_remap_info (fn, OMP_PARALLEL_CLAUSES (par_stmt), tsi);
  remap_locals_in_child (ri_p);

  /* Add FN to the call graph.  */
  gimplify_function_tree (fn);
  cgraph_add_new_function (fn);

  /* Remove DECL_VALUE_EXPR from every symbol we processed.  */
  for (ix = 0; VEC_iterate (tree, ri_p->syms_with_value_expr, ix, t); ix++)
    {
      SET_DECL_VALUE_EXPR (t, NULL_TREE);
      DECL_HAS_VALUE_EXPR_P (t) = 0;
    }

  /* Remap shared variables in the parent function.  */
  wi.callback = remap_locals_in_parent_r;
  wi.info = ri_p;
  walk_stmts (&wi, &DECL_SAVED_TREE (current_function_decl));

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
  num_threads = emit_num_threads_setup_code (ri_p, data);

  /* Emit GOMP_parallel_start (__omp_fn.XXXX ...).  */
  call = create_gomp_parallel_start (fn, addr_data_arg, num_threads);
  tsi_link_after (tsi, call, TSI_CONTINUE_LINKING);

  /* Emit __omp_fn.XXXX (&omp_data).  */
  args = tree_cons (NULL_TREE, unshare_expr (addr_data_arg), NULL_TREE);
  call = build_function_call_expr (fn, args);
  tsi_link_after (tsi, call, TSI_NEW_STMT);

  /* Emit GOMP_parallel_end ().  */
  call = create_gomp_parallel_end ();
  tsi_link_after (tsi, call, TSI_NEW_STMT);

  /* Remove the original statement and free memory used by the mappings.  */
  tsi_delink (&orig_tsi);
  delete_remap_info (ri_p);
}


/* Lowers statement TSI.  DATA is passed through the recursion.  */

static void
lower_stmt (tree_stmt_iterator *tsi, struct lower_data *data)
{
  tree stmt = tsi_stmt (*tsi);

  if (EXPR_HAS_LOCATION (stmt) && data)
    TREE_BLOCK (stmt) = data->block;

  switch (TREE_CODE (stmt))
    {
    case BIND_EXPR:
      lower_bind_expr (tsi, data);
      return;
    case COND_EXPR:
      lower_cond_expr (tsi, data);
      return;
    case RETURN_EXPR:
      lower_return_expr (tsi, data);
      return;

    case TRY_FINALLY_EXPR:
    case TRY_CATCH_EXPR:
      lower_stmt_body (TREE_OPERAND (stmt, 0), data);
      lower_stmt_body (TREE_OPERAND (stmt, 1), data);
      break;
    case CATCH_EXPR:
      lower_stmt_body (CATCH_BODY (stmt), data);
      break;
    case EH_FILTER_EXPR:
      lower_stmt_body (EH_FILTER_FAILURE (stmt), data);
      break;
      
    case NOP_EXPR:
    case ASM_EXPR:
    case MODIFY_EXPR:
    case CALL_EXPR:
    case GOTO_EXPR:
    case LABEL_EXPR:
    case SWITCH_EXPR:
      break;

    case OMP_PARALLEL:
      lower_omp_parallel (tsi, data);
      break;

    default:
#ifdef ENABLE_CHECKING
      print_node_brief (stderr, "", stmt, 0);
      internal_error ("unexpected node");
#endif
    case COMPOUND_EXPR:
      gcc_unreachable ();
    }

  tsi_next (tsi);
}

/* Lowers a bind_expr TSI.  DATA is passed through the recursion.  */

static void
lower_bind_expr (tree_stmt_iterator *tsi, struct lower_data *data)
{
  tree old_block = data->block;
  tree stmt = tsi_stmt (*tsi);
  tree new_block = BIND_EXPR_BLOCK (stmt);

  if (new_block)
    {
      if (new_block == old_block)
	{
	  /* The outermost block of the original function may not be the
	     outermost statement chain of the gimplified function.  So we
	     may see the outermost block just inside the function.  */
	  gcc_assert (new_block == DECL_INITIAL (current_function_decl));
	  new_block = NULL;
	}
      else
	{
	  /* We do not expect to handle duplicate blocks.  */
	  gcc_assert (!TREE_ASM_WRITTEN (new_block));
	  TREE_ASM_WRITTEN (new_block) = 1;

	  /* Block tree may get clobbered by inlining.  Normally this would
	     be fixed in rest_of_decl_compilation using block notes, but
	     since we are not going to emit them, it is up to us.  */
	  BLOCK_CHAIN (new_block) = BLOCK_SUBBLOCKS (old_block);
	  BLOCK_SUBBLOCKS (old_block) = new_block;
	  BLOCK_SUBBLOCKS (new_block) = NULL_TREE;
	  BLOCK_SUPERCONTEXT (new_block) = old_block;

	  data->block = new_block;
	}
    }

  record_vars (BIND_EXPR_VARS (stmt));
  lower_stmt_body (BIND_EXPR_BODY (stmt), data);

  if (new_block)
    {
      gcc_assert (data->block == new_block);

      BLOCK_SUBBLOCKS (new_block)
	= blocks_nreverse (BLOCK_SUBBLOCKS (new_block));
      data->block = old_block;
    }

  /* The BIND_EXPR no longer carries any useful information -- kill it.  */
  tsi_link_before (tsi, BIND_EXPR_BODY (stmt), TSI_SAME_STMT);
  tsi_delink (tsi);
}

/* Try to determine whether a TRY_CATCH expression can fall through.
   This is a subroutine of block_may_fallthru.  */

static bool
try_catch_may_fallthru (tree stmt)
{
  tree_stmt_iterator i;

  /* If the TRY block can fall through, the whole TRY_CATCH can
     fall through.  */
  if (block_may_fallthru (TREE_OPERAND (stmt, 0)))
    return true;

  i = tsi_start (TREE_OPERAND (stmt, 1));
  switch (TREE_CODE (tsi_stmt (i)))
    {
    case CATCH_EXPR:
      /* We expect to see a sequence of CATCH_EXPR trees, each with a
	 catch expression and a body.  The whole TRY_CATCH may fall
	 through iff any of the catch bodies falls through.  */
      for (; !tsi_end_p (i); tsi_next (&i))
	{
	  if (block_may_fallthru (CATCH_BODY (tsi_stmt (i))))
	    return true;
	}
      return false;

    case EH_FILTER_EXPR:
      /* The exception filter expression only matters if there is an
	 exception.  If the exception does not match EH_FILTER_TYPES,
	 we will execute EH_FILTER_FAILURE, and we will fall through
	 if that falls through.  If the exception does match
	 EH_FILTER_TYPES, the stack unwinder will continue up the
	 stack, so we will not fall through.  We don't know whether we
	 will throw an exception which matches EH_FILTER_TYPES or not,
	 so we just ignore EH_FILTER_TYPES and assume that we might
	 throw an exception which doesn't match.  */
      return block_may_fallthru (EH_FILTER_FAILURE (tsi_stmt (i)));

    default:
      /* This case represents statements to be executed when an
	 exception occurs.  Those statements are implicitly followed
	 by a RESX_EXPR to resume execution after the exception.  So
	 in this case the TRY_CATCH never falls through.  */
      return false;
    }
}

/* Try to determine if we can fall out of the bottom of BLOCK.  This guess
   need not be 100% accurate; simply be conservative and return true if we
   don't know.  This is used only to avoid stupidly generating extra code.
   If we're wrong, we'll just delete the extra code later.  */

bool
block_may_fallthru (tree block)
{
  tree stmt = expr_last (block);

  switch (stmt ? TREE_CODE (stmt) : ERROR_MARK)
    {
    case GOTO_EXPR:
    case RETURN_EXPR:
    case RESX_EXPR:
      /* Easy cases.  If the last statement of the block implies 
	 control transfer, then we can't fall through.  */
      return false;

    case SWITCH_EXPR:
      /* If SWITCH_LABELS is set, this is lowered, and represents a
	 branch to a selected label and hence can not fall through.
	 Otherwise SWITCH_BODY is set, and the switch can fall
	 through.  */
      return SWITCH_LABELS (stmt) == NULL_TREE;

    case COND_EXPR:
      if (block_may_fallthru (COND_EXPR_THEN (stmt)))
	return true;
      return block_may_fallthru (COND_EXPR_ELSE (stmt));

    case BIND_EXPR:
      return block_may_fallthru (BIND_EXPR_BODY (stmt));

    case TRY_CATCH_EXPR:
      return try_catch_may_fallthru (stmt);

    case TRY_FINALLY_EXPR:
      /* The finally clause is always executed after the try clause,
	 so if it does not fall through, then the try-finally will not
	 fall through.  Otherwise, if the try clause does not fall
	 through, then when the finally clause falls through it will
	 resume execution wherever the try clause was going.  So the
	 whole try-finally will only fall through if both the try
	 clause and the finally clause fall through.  */
      return (block_may_fallthru (TREE_OPERAND (stmt, 0))
	      && block_may_fallthru (TREE_OPERAND (stmt, 1)));

    case MODIFY_EXPR:
      if (TREE_CODE (TREE_OPERAND (stmt, 1)) == CALL_EXPR)
	stmt = TREE_OPERAND (stmt, 1);
      else
	return true;
      /* FALLTHRU */

    case CALL_EXPR:
      /* Functions that do not return do not fall through.  */
      return (call_expr_flags (stmt) & ECF_NORETURN) == 0;
    
    case CLEANUP_POINT_EXPR:
      return block_may_fallthru (TREE_OPERAND (stmt, 0));

    default:
      return true;
    }
}

/* Lowers a cond_expr TSI.  DATA is passed through the recursion.  */

static void
lower_cond_expr (tree_stmt_iterator *tsi, struct lower_data *data)
{
  tree stmt = tsi_stmt (*tsi);
  bool then_is_goto, else_is_goto;
  tree then_branch, else_branch;
  tree then_goto, else_goto;
  
  then_branch = COND_EXPR_THEN (stmt);
  else_branch = COND_EXPR_ELSE (stmt);

  lower_stmt_body (then_branch, data);
  lower_stmt_body (else_branch, data);

  then_goto = expr_only (then_branch);
  then_is_goto = then_goto && simple_goto_p (then_goto);

  else_goto = expr_only (else_branch);
  else_is_goto = else_goto && simple_goto_p (else_goto);

  if (!then_is_goto || !else_is_goto)
    {
      tree then_label, else_label, end_label, t;

      then_label = NULL_TREE;
      else_label = NULL_TREE;
      end_label = NULL_TREE;
 
      /* Replace the cond_expr with explicit gotos.  */
      if (!then_is_goto)
	{
	  t = build1 (LABEL_EXPR, void_type_node, NULL_TREE);
	  if (TREE_SIDE_EFFECTS (then_branch))
	    then_label = t;
	  else
	    end_label = t;
	  then_goto = build_and_jump (&LABEL_EXPR_LABEL (t));
	}

      if (!else_is_goto)
	{
	  t = build1 (LABEL_EXPR, void_type_node, NULL_TREE);
	  if (TREE_SIDE_EFFECTS (else_branch))
	    else_label = t;
	  else
	    {
	      /* Both THEN and ELSE can be no-ops if one or both contained an
	         empty BIND_EXPR that was associated with the toplevel block
	         of an inlined function.  In that case remove_useless_stmts
	         can't have cleaned things up for us; kill the whole 
	         conditional now.  */
	      if (end_label)
		{
		  tsi_delink (tsi);
		  return;
		}
	      else
		end_label = t;
	    }
	  else_goto = build_and_jump (&LABEL_EXPR_LABEL (t));
	}

      if (then_label)
	{
	  bool may_fallthru = block_may_fallthru (then_branch);

	  tsi_link_after (tsi, then_label, TSI_CONTINUE_LINKING);
	  tsi_link_after (tsi, then_branch, TSI_CONTINUE_LINKING);
  
	  if (else_label && may_fallthru)
	    {
	      end_label = build1 (LABEL_EXPR, void_type_node, NULL_TREE);
	      t = build_and_jump (&LABEL_EXPR_LABEL (end_label));
	      tsi_link_after (tsi, t, TSI_CONTINUE_LINKING);
	    }
	}
  
      if (else_label)
	{
	  tsi_link_after (tsi, else_label, TSI_CONTINUE_LINKING);
	  tsi_link_after (tsi, else_branch, TSI_CONTINUE_LINKING);
	}

      if (end_label)
	tsi_link_after (tsi, end_label, TSI_CONTINUE_LINKING);
    }

  COND_EXPR_THEN (stmt) = then_goto;
  COND_EXPR_ELSE (stmt) = else_goto;

  tsi_next (tsi);
}

static void
lower_return_expr (tree_stmt_iterator *tsi, struct lower_data *data)
{
  tree stmt = tsi_stmt (*tsi);
  tree value, t, label;

  /* Extract the value being returned.  */
  value = TREE_OPERAND (stmt, 0);
  if (value && TREE_CODE (value) == MODIFY_EXPR)
    value = TREE_OPERAND (value, 1);

  /* Match this up with an existing return statement that's been created.  */
  for (t = data->return_statements; t ; t = TREE_CHAIN (t))
    {
      tree tvalue = TREE_OPERAND (TREE_VALUE (t), 0);
      if (tvalue && TREE_CODE (tvalue) == MODIFY_EXPR)
	tvalue = TREE_OPERAND (tvalue, 1);

      if (value == tvalue)
	{
	  label = TREE_PURPOSE (t);
	  goto found;
	}
    }

  /* Not found.  Create a new label and record the return statement.  */
  label = create_artificial_label ();
  data->return_statements = tree_cons (label, stmt, data->return_statements);

  /* Generate a goto statement and remove the return statement.  */
 found:
  t = build (GOTO_EXPR, void_type_node, label);
  SET_EXPR_LOCUS (t, EXPR_LOCUS (stmt));
  tsi_link_before (tsi, t, TSI_SAME_STMT);
  tsi_delink (tsi);
}


/* Record the variables in VARS.  */

void
record_vars (tree vars)
{
  for (; vars; vars = TREE_CHAIN (vars))
    {
      tree var = vars;

      /* BIND_EXPRs contains also function/type/constant declarations
         we don't need to care about.  */
      if (TREE_CODE (var) != VAR_DECL)
	continue;
      /* Nothing to do in this case.  */
      if (DECL_EXTERNAL (var))
	continue;

      /* Record the variable.  */
      cfun->unexpanded_var_list = tree_cons (NULL_TREE, var,
					     cfun->unexpanded_var_list);
    }
}

/* Check whether to expand a variable VAR.  */

static bool
expand_var_p (tree var)
{
  struct var_ann_d *ann;

  if (TREE_CODE (var) != VAR_DECL)
    return true;

  /* Leave statics and externals alone.  */
  if (TREE_STATIC (var) || DECL_EXTERNAL (var))
    return true;

  /* Remove all unused local variables.  */
  ann = var_ann (var);
  if (!ann || !ann->used)
    return false;

  return true;
}

/* Throw away variables that are unused.  */

static void
remove_useless_vars (void)
{
  tree var, *cell;
  FILE *df = NULL;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      df = dump_file;
      fputs ("Discarding as unused:\n", df);
    }

  for (cell = &cfun->unexpanded_var_list; *cell; )
    {
      var = TREE_VALUE (*cell);

      if (!expand_var_p (var))
	{
	  if (df)
	    {
	      fputs ("  ", df);
	      print_generic_expr (df, var, dump_flags);
	      fputc ('\n', df);
	    }

	  *cell = TREE_CHAIN (*cell);
	  continue;
	}

      cell = &TREE_CHAIN (*cell);
    }

  if (df)
    fputc ('\n', df);
}

struct tree_opt_pass pass_remove_useless_vars = 
{
  "vars",				/* name */
  NULL,					/* gate */
  remove_useless_vars,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  0,					/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func,			/* todo_flags_finish */
  0					/* letter */
};

/* Mark BLOCK used if it has a used variable in it, then recurse over its
   subblocks.  */

static void
mark_blocks_with_used_vars (tree block)
{
  tree var;
  tree subblock;

  if (!TREE_USED (block))
    {
      for (var = BLOCK_VARS (block);
	   var;
	   var = TREE_CHAIN (var))
	{
	  if (TREE_USED (var))
	    {
	      TREE_USED (block) = true;
	      break;
	    }
	}
    }
  for (subblock = BLOCK_SUBBLOCKS (block);
       subblock;
       subblock = BLOCK_CHAIN (subblock))
    mark_blocks_with_used_vars (subblock);
}

/* Mark the used attribute on blocks correctly.  */
  
static void
mark_used_blocks (void)
{  
  mark_blocks_with_used_vars (DECL_INITIAL (current_function_decl));
}


struct tree_opt_pass pass_mark_used_blocks = 
{
  "blocks",				/* name */
  NULL,					/* gate */
  mark_used_blocks,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  0,					/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func,			/* todo_flags_finish */
  0					/* letter */
};
