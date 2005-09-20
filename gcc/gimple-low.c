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

  data.block = DECL_INITIAL (current_function_decl);
  BLOCK_SUBBLOCKS (data.block) = NULL_TREE;
  BLOCK_CHAIN (data.block) = NULL_TREE;
  TREE_ASM_WRITTEN (data.block) = 1;

  data.return_statements = NULL_TREE;

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


/* Data structure used for remap_locals_r.  */

struct remap_info_d
{
  /* Mapping between local variables in the original function and the
     new function containing the body of the OpenMP directive.  */
  splay_tree map;

  /* Function holding the body of the OpenMP directive.  */
  tree gomp_fn;

  /* Data sharing and copying clauses associated with this directive.  */
  tree clauses;

  /* Sets of DECLs representing the different data sharing clauses.  */
  bitmap private;
  bitmap firstprivate;
  bitmap lastprivate;
  bitmap shared;

  /* Sets of DECLs representing the different data copying clauses.  */
  bitmap copyin;
  bitmap copyprivate;

  /* Structure holding addresses of shared and copy-in data to be
     passed to the children threads.  This is declared in the parent
     thread.  */
  tree data_arg_orig;

  /* Pointer to DATA_ARG_ORIG passed in as an argument to each child
     thread.  */
  tree data_arg_dest;
};
  

/* Add all the variables in LIST to the bitmap *SET_P.  Create a new
   bitmap, if necessary.  */

static void
add_decls_to_set (bitmap *set_p, tree list)
{
  tree t;

  if (*set_p == NULL)
    *set_p = BITMAP_ALLOC (NULL);

  for (t = list; t; t = TREE_CHAIN (t))
    {
      tree var = TREE_VALUE (t);
      bitmap_set_bit (*set_p, DECL_UID (var));
    }
}


/* Build and create a data sharing description structure to guide the
   remapping actions done in remap_locals_r.  GOMP_FN is the function
   holding the body of the directive to be processed.  CLAUSES is the
   list of clauses controlling data sharing and copying.  */

static struct remap_info_d *
build_remap_info (tree gomp_fn, tree clauses)
{
  tree c;
  struct remap_info_d *ri_p;

  /* Setup the mapping data.  */
  ri_p = xmalloc (sizeof (*ri_p));
  memset (ri_p, 0, sizeof (*ri_p));
  ri_p->gomp_fn = gomp_fn;
  ri_p->clauses = clauses;
  ri_p->map = splay_tree_new (splay_tree_compare_pointers, 0, 0);

  for (c = clauses; c; c = TREE_CHAIN (c))
    {
      tree clause = TREE_VALUE (c);
      bitmap *set_p = NULL;

      if (TREE_CODE (clause) == GOMP_CLAUSE_PRIVATE)
	set_p = &ri_p->private;
      else if (TREE_CODE (clause) == GOMP_CLAUSE_SHARED)
	set_p = &ri_p->shared;
      else if (TREE_CODE (clause) == GOMP_CLAUSE_FIRSTPRIVATE)
	set_p = &ri_p->firstprivate;
      else if (TREE_CODE (clause) == GOMP_CLAUSE_LASTPRIVATE)
	set_p = &ri_p->lastprivate;
      else if (TREE_CODE (clause) == GOMP_CLAUSE_COPYIN)
	set_p = &ri_p->copyin;
      else if (TREE_CODE (clause) == GOMP_CLAUSE_COPYPRIVATE)
	set_p = &ri_p->copyprivate;

      if (set_p)
	add_decls_to_set (set_p, TREE_OPERAND (clause, 0));
    }

  /* FIXME.  Add checking code to disallow variables in multiple sets.
     Variables may only appear in exactly one set, except for
     firstprivate and lastprivate.  */
  return ri_p;
}


/* Free memory used by the data sharing structure RI_P.  */

static void
delete_remap_info (struct remap_info_d *ri_p)
{
  splay_tree_delete (ri_p->map);
  BITMAP_FREE (ri_p->private);
  BITMAP_FREE (ri_p->firstprivate);
  BITMAP_FREE (ri_p->lastprivate);
  BITMAP_FREE (ri_p->shared);
  BITMAP_FREE (ri_p->copyin);
  BITMAP_FREE (ri_p->copyprivate);
  free (ri_p);
}


/* Given a private variable VAR, get the variable that will replace
   VAR inside the body of the function created for RI_P->GOMP_FN.  If
   VAR had not been mapped before, add it to the unexpanded variable
   list in ID->GOMP_FN.  */

static tree
get_gomp_private_ref (tree var, struct remap_info_d *ri_p)
{
  tree repl;
  splay_tree_node n;

  n = splay_tree_lookup (ri_p->map, (splay_tree_key) var);
  if (n == NULL)
    {
      struct function *f = DECL_STRUCT_FUNCTION (ri_p->gomp_fn);

      repl = copy_decl_for_dup (var, current_function_decl, ri_p->gomp_fn, 0);
      splay_tree_insert (ri_p->map, (splay_tree_key) var,
			 (splay_tree_value) repl);

      /* Since this is first time that we map VAR, add it to the
	 unexpanded variable list of GOMP_FN.  */
      DECL_CONTEXT (repl) = ri_p->gomp_fn;
      DECL_SEEN_IN_BIND_EXPR_P (repl) = 1;
      f->unexpanded_var_list = tree_cons (NULL_TREE, repl,
					  f->unexpanded_var_list);
    }
  else
    repl = (tree) n->value;
  
  return repl;
}


/* Given a variable VAR, marked as shared inside the OpenMP shared
   clause, return the INDIRECT_REF expression DATA->VAR.  DATA is the
   argument passed in to the function implementing the body of the
   OpenMP directive.  */

static tree
get_gomp_shared_ref (tree var, struct remap_info_d *ri_p)
{
  tree t, field;
  splay_tree_node n = splay_tree_lookup (ri_p->map, (splay_tree_key) var);

  /* If no replacement exists for VAR, create a new one.  */
  if (n == NULL)
    {
      /* Add a new field to RI_P->DATA_ARG_DEST to hold the address of VAR.  */
      tree struct_ptr_type, struct_type;

      /* Create the structure type and local pointer .GOMP_DATA.  */
      if (ri_p->data_arg_dest == NULL)
	{
	  struct function *f = DECL_STRUCT_FUNCTION (ri_p->gomp_fn);
	  struct_ptr_type = build_pointer_type (make_node (RECORD_TYPE));
	  ri_p->data_arg_dest = create_tmp_var_raw (struct_ptr_type,
						    ".gomp_data");

	  /* Add .GOMP_DATA to the unexpanded variable list of GOMP_FN.  */
	  DECL_CONTEXT (ri_p->data_arg_dest) = ri_p->gomp_fn;
	  DECL_SEEN_IN_BIND_EXPR_P (ri_p->data_arg_dest) = 1;
	  f->unexpanded_var_list = tree_cons (NULL_TREE, 
	                                      ri_p->data_arg_dest,
					      f->unexpanded_var_list);
	}
      else
	struct_ptr_type = TREE_TYPE (ri_p->data_arg_dest);

      /* Add a new field to hold the address of VAR.  */
      struct_type = TREE_TYPE (struct_ptr_type);

      field = build_decl (FIELD_DECL,
			  DECL_NAME (var),
			  build_pointer_type (TREE_TYPE (var)));
      DECL_FIELD_CONTEXT (field) = struct_type;
      TREE_CHAIN (field) = TYPE_FIELDS (struct_type);
      TYPE_FIELDS (struct_type) = field;

      /* Create a mapping between VAR and FIELD.  */
      splay_tree_insert (ri_p->map,
			 (splay_tree_key) var,
			 (splay_tree_value) field);
    }
  else
    field = (tree) n->value;

  /* Build .GOMP_DATA->VAR.  */
  t = build (COMPONENT_REF, TREE_TYPE (field),
	     build (INDIRECT_REF, TREE_TYPE (TREE_TYPE (ri_p->data_arg_dest)),
		    ri_p->data_arg_dest),
	     field,
	     NULL_TREE);

  /* Return *(.GOMP_DATA->VAR).  */
  t = build (INDIRECT_REF, TREE_TYPE (var), t);

  return t;
}


/* Callback for walk_tree to change the context of all the local
   variables found in *TP accordingly to what is described by the
   sets in DATA.  */

static tree
remap_locals_r (tree *tp, int *ws, void *data)
{
  struct remap_info_d *ri_p = (struct remap_info_d *) data;
  tree t, gomp_fn;
  enum tree_code code;

  gomp_fn = ri_p->gomp_fn;

  t = *tp;
  code = TREE_CODE (t);
  if (code == VAR_DECL && !is_global_var (t))
    {
      tree repl = NULL_TREE;

      /* Remap local variables into GOMP_FN if they appear in one of
	 the private sets.  */
      if (ri_p->private
	  && bitmap_bit_p (ri_p->private, DECL_UID (t)))
	{
	  /* Private variables just need to be declared inside GOMP_FN.  */
	  repl = get_gomp_private_ref (t, ri_p);
	}
      else if (ri_p->firstprivate
	       && bitmap_bit_p (ri_p->firstprivate, DECL_UID (t)))
	gcc_unreachable ();
      else if (ri_p->lastprivate
	       && bitmap_bit_p (ri_p->lastprivate, DECL_UID (t)))
	gcc_unreachable ();
      else if (ri_p->shared
	       && bitmap_bit_p (ri_p->shared, DECL_UID (t)))
	{
	  /* Shared variables are declared as global pointers holding
	     their address.  Each reference to T is replaced by a
	     dereference of the pointer associated with T.  */
	  repl = get_gomp_shared_ref (t, ri_p);
	}
      else if (ri_p->copyin
	       && bitmap_bit_p (ri_p->copyin, DECL_UID (t)))
	gcc_unreachable ();
      else if (ri_p->copyprivate
	       && bitmap_bit_p (ri_p->copyprivate, DECL_UID (t)))
	gcc_unreachable ();

      if (repl)
	*tp = repl;
    }
  else if (code == LABEL_DECL)
    DECL_CONTEXT (t) = gomp_fn;
  else if (code == GOMP_CLAUSE_PRIVATE
	   || code == GOMP_CLAUSE_SHARED
	   || code == GOMP_CLAUSE_FIRSTPRIVATE
	   || code == GOMP_CLAUSE_LASTPRIVATE
	   || code == GOMP_CLAUSE_REDUCTION
	   || code == GOMP_CLAUSE_COPYIN
	   || code == GOMP_CLAUSE_COPYPRIVATE)
    {
      /* Variables may appear inside other clauses in nested OpenMP
	 directives.  Avoid rewriting nested references because they
	 need to refer to the original _DECL.  */
      *ws = 0;
    }

  return NULL_TREE;
}


/* Map local variables from the current function into GOMP_FN.
   CLAUSES is the set of clauses controlling data sharing and copying.
   Return mapping information as defined in struct remap_info_d.  */

static struct remap_info_d *
remap_locals (tree gomp_fn, tree clauses)
{
  struct remap_info_d *ri_p;
  
  ri_p = build_remap_info (gomp_fn, clauses);
  walk_tree (&DECL_SAVED_TREE (gomp_fn), remap_locals_r, ri_p, NULL);

  /* Once we have remapped all the locals, we can layout the
     structure type for RI_P->DATA_ARG_DEST.  */
  if (ri_p->data_arg_dest)
    {
      layout_type (TREE_TYPE (TREE_TYPE (ri_p->data_arg_dest)));
      layout_type (TREE_TYPE (ri_p->data_arg_dest));
    }

  return ri_p;
}


/* Build and return a new FUNCTION_DECL to hold the body of the OpenMP
   directive GOMP_EXPR.  */

static tree
create_gomp_fn (tree gomp_expr)
{
  tree fn_name, fn_type, fn_body, fn_decl, res_decl;
  tree fn_data_arg;

  /* Enclose the body in a BIND_EXPR, if it doesn't have one already.  */
  fn_body = GOMP_PARALLEL_BODY (gomp_expr);
  if (TREE_CODE (fn_body) != BIND_EXPR)
    {
      fn_body = build3 (BIND_EXPR, void_type_node, NULL, fn_body, NULL);
      TREE_SIDE_EFFECTS (fn_body) = 1;
    }

  /* Build the declaration of the new function.  */
  fn_name = create_tmp_var_name ("__gomp_fn");
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


/* Emit code needed to initialize shared and copied-in variables.
   This emits code in the parent thread to store the addresses of
   shared variables into a structure whose address is pass as an
   argument to all the children threads.

   Inside the child function, it inserts the corresponding receiving
   code, which takes the incoming void * argument and casts it into a
   pointer to the structure sent by the parent thread.

   TSI points to the OpenMP directive that we are generating code for.
      The initialization code is inserted before it.

   RI_P points to the remap information computed by remap_locals.  */

static void
emit_gomp_data_setup_code (tree_stmt_iterator *tsi, struct remap_info_d *ri_p)
{
  tree t, c;

  /* Create a local structure holding all the shared data.  Since
     RI_P->DATA_ARG_DEST is a pointer to this structure, we want to
     create a local stack object in the calling function.  */
  if (ri_p->data_arg_dest)
    {
      tree type = TREE_TYPE (TREE_TYPE (ri_p->data_arg_dest));
      ri_p->data_arg_orig = create_tmp_var (type, ".gomp_data");
    }

  /* Generate the necessary setup code for each variable mentioned in
     all the clauses.  */
  for (c = ri_p->clauses; c; c = TREE_CHAIN (c))
    {
      tree clause = TREE_VALUE (c);

      if (TREE_CODE (clause) == GOMP_CLAUSE_SHARED)
	{
	  for (t = TREE_OPERAND (clause, 0); t; t = TREE_CHAIN (t))
	    {
	      tree var;
	      splay_tree_node n;

	      var = TREE_VALUE (t);
	      n = splay_tree_lookup (ri_p->map, (splay_tree_key) var);
	      if (n)
		{
		  tree ref, field, init, addr;

		  /* Emit .GOMP_DATA.VAR = &VAR  */
		  field = (tree) n->value;
		  addr = build1 (ADDR_EXPR, TREE_TYPE (field), var);
		  ref = build (COMPONENT_REF, TREE_TYPE (field),
			       ri_p->data_arg_orig, field, NULL_TREE);
		  init = build (MODIFY_EXPR, TREE_TYPE (field), ref, addr);
		  tsi_link_after (tsi, init, TSI_NEW_STMT);
		}
	    }
	}
    }

  /* Emit receiving code inside RI_P->GOMP_FN.  */
  if (ri_p->data_arg_dest)
    {
      /* If RI_P->DATA_ARG_DEST has been created, then each thread
	 needs to initialize its own copy from the function argument.
	 Emit .GOMP_DATA = (STRUCT *) DATA at the start of the
	 function.  */
      tree body = BIND_EXPR_BODY (DECL_SAVED_TREE (ri_p->gomp_fn));
      tree arg = DECL_ARGUMENTS (ri_p->gomp_fn);
      tree_stmt_iterator child_tsi = tsi_start (body);
      tree cast = build1 (NOP_EXPR, TREE_TYPE (ri_p->data_arg_dest), arg);
      tree t = build (MODIFY_EXPR, TREE_TYPE (ri_p->data_arg_dest),
		      ri_p->data_arg_dest, cast);
      tsi_link_before (&child_tsi, t, TSI_NEW_STMT);
    }
}


/* Lower the OpenMP parallel directive pointed by TSI.  Build a new
   function with the body of the pragma and emit the appropriate
   runtime call.  DATA contains locus and scope information for TSI.  */

static void
lower_gomp_parallel (tree_stmt_iterator *tsi)
{
  tree par_stmt, fn, call, args, num_threads, addr_data_arg;
  tree_stmt_iterator orig_tsi;
  struct remap_info_d *ri_p;
  struct function *saved_cfun;

  orig_tsi = *tsi;
  par_stmt = tsi_stmt (*tsi);

  /* Build a new function out of the pragma's body and add it to the
     call graph.  */
  fn = create_gomp_fn (par_stmt);

  /* Allocate memory for the function structure.  */
  saved_cfun = cfun;
  allocate_struct_function (fn);
  DECL_SOURCE_LOCATION (fn) = input_location;
  cfun->function_end_locus = input_location;

  /* Remap local variables according to the data clauses.  This must
     be done before adding FN to the call graph because adding
     the function to the call graph also gimplifies its body.  And we
     can only gimplify the function after adjusting labels and local
     variable references.  */
  ri_p = remap_locals (fn, GOMP_PARALLEL_CLAUSES (par_stmt));

  /* Add FN to the call graph.  */
  cgraph_add_new_function (fn);

  /* Restore CFUN (which was overwritten by the call to
     allocate_struct_function).  */
  cfun = saved_cfun;

  /* Emit code to setup the shared data before launching the threads.  */
  emit_gomp_data_setup_code (tsi, ri_p);

  /* Take the address of RI_P->DATA_ARG_ORIG to pass to __gomp_fn.XXXX.  */
  addr_data_arg = build1 (ADDR_EXPR,
			  TREE_TYPE (ri_p->data_arg_dest),
                          ri_p->data_arg_orig);
  TREE_ADDRESSABLE (ri_p->data_arg_orig) = 1;

  /* Emit GOMP_parallel_start (__gomp_fn.XXXX ...) to PRE_P.  FIXME,
     num_threads should only be integer_zero_node if the clause
     num_threads is not present.  */
  num_threads = integer_zero_node;
  call = create_gomp_parallel_start (fn, addr_data_arg, num_threads);
  tsi_link_after (tsi, call, TSI_CONTINUE_LINKING);

  /* Emit __gomp_fn.XXXX (&gomp_data).  */
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


/* Emit code for a parallel loop with static schedule.

   The general form is:

   GOMP_FOR <clause(s), V = N1, V {<, >, >=, <=} N2, V {+=, -=} INCR, BODY>

   The lowering process generates code to compute how many iterations
   will be assigned to each thread (CHUNK) and the local loop limits
   (V and Ni).  It then emits a sequential loop to execute BODY in
   the iteration space V to Ni.
   
   For loops of the form V = N1; V < N2; V += STEP, it generates the
   following pseudo-code (gimplified accordingly):

   	NTHREADS = omp_get_num_threads ()
	TID = omp_get_thread_num ()
	NITER = (N2 - N1 + STEP) / STEP
   	CHUNK = ((NITER + NTHREADS) / NTHREADS) * STEP
	V = N1 + TID * CHUNK
	Ni = MIN (V + CHUNK, N2)
    L0:
	if (V < Ni) goto L1 else goto L2
    L1:
	BODY
	V += STEP
	goto L0
    L2:

   The transformation is the same for '<=' with the exception that Ni
   is computed as 'Ni = MIN (Ni + CHUNK, N2 + 1)', so that N2 is
   included in the last iteration.

   For loops of the form for (V = N2; V > N1; V -= STEP), it generates
   the same setup code for computing the NTHREADS, TID and CHUNK, but
   the bound computation is basically a mirror of the previous case:

   	[ ... same as before ... ]
	V = N2 - TID * CHUNK
	Ni = MAX (V - CHUNK, N1)
    L0:
	if (V > Ni) goto L1 else goto L2
    L1:
	BODY
	V -= STEP
	goto L0
    L2:

   The transformation is the same for '>=' with the exception that Ni
   is computed as 'Ni = MAX (V - CHUNK, N1 - 1)', so that N1 is
   included in the last iteration.  */

static void
emit_gomp_for_static (tree_stmt_iterator *tsi)
{
  tree for_stmt = tsi_stmt (*tsi);
  tree nthreads, tid, chunk, ni;
  tree init, cond, incr, step, niter, body, v, n1, n2, l0, l1, l2;
  tree t;
  bool counts_up_p;
  tree stmt_list = alloc_stmt_list ();

  body = GOMP_FOR_BODY (for_stmt);

  /* Extract loop variable (V) and lower bound (N1).  */
  init = GOMP_FOR_INIT (for_stmt);
  gcc_assert (TREE_CODE (init) == MODIFY_EXPR
	      && TREE_CODE (TREE_TYPE (init)) == INTEGER_TYPE);
  v = TREE_OPERAND (init, 0);
  n1 = TREE_OPERAND (init, 1);

  /* Extract the controlling predicate (COND) and upper bound (N2).  */
  cond = GOMP_FOR_COND (for_stmt);
  gcc_assert (TREE_OPERAND (cond, 0) == v);
  n2 = TREE_OPERAND (cond, 1);

  /* Extract the increment expression (V = V [+-] STEP).  */
  incr = GOMP_FOR_INCR (for_stmt);
  gcc_assert (TREE_CODE (incr) == MODIFY_EXPR);
  gcc_assert (TREE_CODE (TREE_OPERAND (incr, 1)) == PLUS_EXPR
              || TREE_CODE (TREE_OPERAND (incr, 1)) == MINUS_EXPR);
  gcc_assert (TREE_OPERAND (TREE_OPERAND (incr, 1), 0) == v);
  step = TREE_OPERAND (TREE_OPERAND (incr, 1), 1);

  /* If the loop counts down instead of counting up, switch
     around the values for N1 and N2.  This simplifies the computation
     of CHUNK by guaranteeing N2 > N1.  */
  counts_up_p = (TREE_CODE (cond) == LT_EXPR || TREE_CODE (cond) == LE_EXPR);
  if (!counts_up_p)
    {
      t = n1;
      n1 = n2;
      n2 = t;
    }

  /* Create all the needed locals.  */
  niter = create_tmp_var (integer_type_node, ".niter");
  nthreads = create_tmp_var (integer_type_node, ".nthreads");
  tid = create_tmp_var (integer_type_node, ".tid");
  chunk = create_tmp_var (long_integer_type_node, ".chunk");
  ni = create_tmp_var (long_integer_type_node, ".ni");

  /* .nthreads = omp_get_num_threads ()  */
  t = build_function_type_list (integer_type_node, void_type_node,
				NULL_TREE);
  t = build_fn_decl ("omp_get_num_threads", t);
  t = build_function_call_expr (t, NULL_TREE);
  t = build (MODIFY_EXPR, long_integer_type_node, nthreads, t);
  tsi_link_after (tsi, t, TSI_NEW_STMT);

  /* .tid = omp_get_thread_num ()  */
  t = build_function_type_list (integer_type_node, void_type_node, NULL_TREE);
  t = build_fn_decl ("omp_get_thread_num", t);
  t = build_function_call_expr (t, NULL_TREE);
  t = build (MODIFY_EXPR, long_integer_type_node, tid, t);
  tsi_link_after (tsi, t, TSI_NEW_STMT);

  /* .niter = (n2 - n1 + step) / step.  */
  t = build (MODIFY_EXPR, long_integer_type_node,
	     niter,
	     fold_build2 (TRUNC_DIV_EXPR, long_integer_type_node,
			  build (PLUS_EXPR, long_integer_type_node,
				 build (MINUS_EXPR, long_integer_type_node,
					unshare_expr (n2), unshare_expr (n1)),
				 unshare_expr (step)),
			  unshare_expr (step)));
  append_to_statement_list (t, &stmt_list);

  /* .chunk = ((.niter + .nthreads) / .nthreads) * step.  */
  t = build (MODIFY_EXPR, long_integer_type_node,
	     chunk,
	     fold_build2 (MULT_EXPR, long_integer_type_node,
			  build (TRUNC_DIV_EXPR, long_integer_type_node,
				 build (PLUS_EXPR, long_integer_type_node,
					niter, nthreads),
				 nthreads),
			  step));
  append_to_statement_list (t, &stmt_list);

  /* For loops counting up:	V = n1 + .tid * .chunk
     For loops counting down:	V = n2 - .tid * .chunk  */
  t = build (MODIFY_EXPR, long_integer_type_node,
	     v,
	     fold_build2 ((counts_up_p) ? PLUS_EXPR : MINUS_EXPR,
			  long_integer_type_node,
			  (counts_up_p) ? unshare_expr (n1) : unshare_expr (n2),
			  fold_build2 (MULT_EXPR, long_integer_type_node,
				       tid, chunk)));
  append_to_statement_list (t, &stmt_list);

  /* For loops counting up:	.ni = MIN_EXPR <v + chunk, n2>

     If the test for the original loop was <=, we need to
     include N2 in the iteration, so we use N2 + 1 instead of N2 in
     MIN_EXPR.

     For loops counting down:	.ni = MAX_EXPR <v - chunk, n1>

     Similarly, if the loop is >=, then use N1 - 1 instead of N1 in
     MAX_EXPR.  */
  if (TREE_CODE (cond) == LT_EXPR)
    t = unshare_expr (n2);
  else if (TREE_CODE (cond) == LE_EXPR)
    t = fold_build2 (PLUS_EXPR, long_integer_type_node, unshare_expr (n2),
		     integer_one_node);
  else if (TREE_CODE (cond) == GT_EXPR)
    t = unshare_expr (n1);
  else if (TREE_CODE (cond) == GE_EXPR)
    t = fold_build2 (MINUS_EXPR, long_integer_type_node, unshare_expr (n1),
		     integer_one_node);

  t = build (MODIFY_EXPR, long_integer_type_node,
	     ni,
	     fold_build2 ((counts_up_p) ? MIN_EXPR : MAX_EXPR,
			  long_integer_type_node,
			  build ((counts_up_p) ? PLUS_EXPR : MINUS_EXPR,
				 long_integer_type_node,
				 v, chunk),
			  t));
  append_to_statement_list (t, &stmt_list);

  /* Gimplify the iteration partitioning code we just generated.  */
  push_gimplify_context ();
  gimplify_stmt (&stmt_list);
  pop_gimplify_context (NULL);

  tsi_link_after (tsi, stmt_list, TSI_CONTINUE_LINKING);

  /* L0:
	if (V < Ni) goto L1 else goto L2
     L1:
	BODY
	V += INCR
	goto L0
     L2:

     If the loop counts down, the conditional is controlled by (V > Ni)
     instead.  */
  l0 = create_artificial_label ();
  l1 = create_artificial_label ();
  l2 = create_artificial_label ();

  /* L0:  */
  t = build (LABEL_EXPR, void_type_node, l0);
  tsi_link_after (tsi, t, TSI_NEW_STMT);

  /* For loops counting up:	if (V < Ni) goto L1 else goto L2
     For loops counting down:	if (V > Ni) goto L1 else goto L2  */
  t = build (COND_EXPR, void_type_node,
	     (counts_up_p
	      ? build (LT_EXPR, boolean_type_node, v, ni)
	      : build (GT_EXPR, boolean_type_node, v, ni)),
	     build_and_jump (&l1),
	     build_and_jump (&l2));
  tsi_link_after (tsi, t, TSI_NEW_STMT);

  /* L1:  */
  t = build (LABEL_EXPR, void_type_node, l1);
  tsi_link_after (tsi, t, TSI_NEW_STMT);

  /* BODY
     V = V [+-] INCR
     goto L0  */
  tsi_link_after (tsi, body, TSI_CONTINUE_LINKING);
  tsi_link_after (tsi, incr, TSI_NEW_STMT);
  tsi_link_after (tsi, build_and_jump (&l0), TSI_NEW_STMT);

  /* L2:  */
  t = build (LABEL_EXPR, void_type_node, l2);
  tsi_link_after (tsi, t, TSI_NEW_STMT);
}


/* Lower the GOMP_FOR structure pointed-to by TSI.  GOMP_FOR is a work
   sharing construct that distributes the iteration space of the
   original loop into all the available threads.  No new parallel
   regions are created.  This construct is *only* valid inside a
   parallel region.  */

static void
lower_gomp_for (tree_stmt_iterator *tsi, struct lower_data *data)
{
  tree for_stmt = tsi_stmt (*tsi);
  tree_stmt_iterator orig_tsi;
  struct remap_info_d *ri_p;

  orig_tsi = *tsi;

  /* Lower the body of the loop.  */
  lower_stmt_body (GOMP_FOR_BODY (for_stmt), data);

  /* Remap local variables according to the data clauses.  */
  ri_p = remap_locals (current_function_decl, GOMP_FOR_CLAUSES (for_stmt));

  /* Emit code to setup shared data before executing the loop.  */
  emit_gomp_data_setup_code (tsi, ri_p);

  /* Emit code for the parallel loop according to the specified schedule.
     FIXME, only static schedules handled.  */
  emit_gomp_for_static (tsi);

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

    case GOMP_PARALLEL:
      lower_gomp_parallel (tsi);
      break;

    case GOMP_FOR:
      lower_gomp_for (tsi, data);
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
