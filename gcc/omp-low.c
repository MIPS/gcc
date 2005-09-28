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
  /* Mapping between a DECL in the original function and the field in
     the structure used in the interface between the sender and receiver.  */
  splay_tree map;

  /* True for those DECLs in MAP that are shared.  */
  bitmap shared;

  /* True for those DECLs in MAP that are being copied in.  */
  bitmap copyin;

  /* True for those DECLs in MAP that are being copied out.  */
  bitmap copyout;

  /* Structure variable holding addresses and values of shared, copy-in,
     and copy-out data to be passed to the other threads.  This is
     declared in the sending thread.  */
  tree omp_data_send;

  /* The child function for the omp parallel.  */
  tree omp_fn;

  /* Pointer variable holding the address of that structure as visible to
     the receiving threads.  */
  tree omp_data_receive;

  tree_stmt_iterator tsi;
};


/* Build and create a data sharing description structure to guide the
   remapping actions done in remap_locals_in_child_r.  */

static struct remap_info_d *
new_remap_info (void)
{
  struct remap_info_d *ri_p;

  ri_p = xcalloc (1, sizeof (*ri_p));
  ri_p->map = splay_tree_new (splay_tree_compare_pointers, 0, 0);
  ri_p->shared = BITMAP_ALLOC (&bitmap_default_obstack);
  ri_p->copyin = BITMAP_ALLOC (&bitmap_default_obstack);
  ri_p->copyout = BITMAP_ALLOC (&bitmap_default_obstack);

  return ri_p;
}


/* Free memory used by the data sharing structure RI_P.  */

static void
delete_remap_info (struct remap_info_d *ri_p)
{
  splay_tree_delete (ri_p->map);
  BITMAP_FREE (ri_p->shared);
  BITMAP_FREE (ri_p->copyin);
  BITMAP_FREE (ri_p->copyout);
  free (ri_p);
}


/* Find an OpenMP clause of type KIND within CLAUSES.  */

tree
find_omp_clause (tree clauses, enum tree_code kind)
{
  for (; clauses ; clauses = OMP_CLAUSE_CHAIN (clauses))
    if (TREE_CODE (clauses) == kind)
      return clauses;

  return NULL_TREE;
}

/* Return true if DECL should be copied by pointer.  */

bool
use_pointer_for_field (tree decl, bool shared_p)
{
  if (AGGREGATE_TYPE_P (TREE_TYPE (decl)))
    return true;

  if (shared_p && (TREE_STATIC (decl) || DECL_EXTERNAL (decl)))
    return true;

  return false;
}


/* Retrieve the sender remapping variable for RI_P.  */

static void
create_data_decl (struct remap_info_d *ri_p)
{
  tree decl, type;

  type = make_node (RECORD_TYPE);
  TYPE_NAME (type) = create_tmp_var_name (".omp_data_s");

  decl = create_tmp_var_raw (type, ".omp_data_o");
  ri_p->omp_data_send = decl;
  gimple_add_tmp_var (decl);

  TREE_ADDRESSABLE (decl) = 1;
}


/* Add a new field for VAR inside the structure RI_P->OMP_DATA.
   Associate the new field with VAR in RI_P->MAP.  */

static void
add_omp_data_field (tree var, bool by_ref, struct remap_info_d *ri_p)
{
  tree field, type;

  /* We can have both firstprivate and lastprivate on a parallel.
     Avoid creating two fields.  */
  if (splay_tree_lookup (ri_p->map, (splay_tree_key)var) != NULL)
    return;

  type = TREE_TYPE (var);
  if (by_ref)
    type = build_pointer_type (type);

  field = build_decl (FIELD_DECL, DECL_NAME (var), type);

  if (ri_p->omp_data_send == NULL)
    create_data_decl (ri_p);
  insert_field_into_struct (TREE_TYPE (ri_p->omp_data_send), field);

  splay_tree_insert (ri_p->map, (splay_tree_key)var, (splay_tree_value)field);
}


/* Prepare replacements for all the variables in the clauses of PAR_STMT,
   which is assumed to be one of the data sharing/copying clauses (private,
   shared, firstprivate, lastprivate, copyin or copyprivate).  */

static void
setup_data_fields (tree par_stmt, struct remap_info_d *ri_p)
{
  tree c;

  for (c = OMP_PARALLEL_CLAUSES (par_stmt); c ; c = OMP_CLAUSE_CHAIN (c))
    {
      enum tree_code kind = TREE_CODE (c);
      tree outer = OMP_CLAUSE_OUTER_DECL (c);
      bool use_ptr;

      switch (kind)
	{
	case OMP_CLAUSE_SHARED:
	  use_ptr = use_pointer_for_field (outer, true);
	  bitmap_set_bit (ri_p->shared, DECL_UID (outer));
	  bitmap_set_bit (ri_p->copyin, DECL_UID (outer));
	  bitmap_set_bit (ri_p->copyout, DECL_UID (outer));
	  break;

	case OMP_CLAUSE_FIRSTPRIVATE:
	case OMP_CLAUSE_COPYIN:
	  use_ptr = use_pointer_for_field (outer, false);
	  bitmap_set_bit (ri_p->copyin, DECL_UID (outer));
	  break;

	case OMP_CLAUSE_COPYPRIVATE:
	case OMP_CLAUSE_LASTPRIVATE:
	case OMP_CLAUSE_REDUCTION:
	  use_ptr = use_pointer_for_field (outer, false);
	  bitmap_set_bit (ri_p->copyout, DECL_UID (outer));
	  break;

	default:
	  continue;
	}

      add_omp_data_field (outer, use_ptr, ri_p);
    }

  if (ri_p->omp_data_send)
    {
      layout_type (TREE_TYPE (ri_p->omp_data_send));
      layout_decl (ri_p->omp_data_send, 0);
    }
}


/* Build a decl for the child function for RI_P.  It'll not contain a body
   yet, just the bare decl.  */

static void
create_child_function (struct remap_info_d *ri_p)
{
  tree decl, type, ptype, name, t;
  struct function *saved_cfun;

  name = create_tmp_var_name ("__omp_fn");

  /* If we don't have any data to send, create a dummy void* parameter.
     Otherwise kinda cheat and make the type of the passed data pointer
     match the type we actually need.  */
  if (ri_p->omp_data_send)
    ptype = build_pointer_type (TREE_TYPE (ri_p->omp_data_send));
  else
    ptype = ptr_type_node;

  type = build_function_type_list (void_type_node, ptype, NULL_TREE);

  decl = build_decl (FUNCTION_DECL, name, type);

  TREE_STATIC (decl) = 1;
  TREE_USED (decl) = 1;
  DECL_ARTIFICIAL (decl) = 1;
  DECL_IGNORED_P (decl) = 0;
  TREE_PUBLIC (decl) = 0;
  DECL_UNINLINABLE (decl) = 1;
  DECL_EXTERNAL (decl) = 0;
  DECL_CONTEXT (decl) = NULL_TREE;

  t = build_decl (RESULT_DECL, NULL_TREE, void_type_node);
  DECL_ARTIFICIAL (t) = 1;
  DECL_IGNORED_P (t) = 1;
  DECL_RESULT (decl) = t;

  t = build_decl (PARM_DECL, get_identifier (".omp_data_i"), ptype);
  DECL_ARG_TYPE (t) = ptype;
  DECL_CONTEXT (t) = decl;
  TREE_USED (t) = 1;
  DECL_ARGUMENTS (decl) = t;

  /* Allocate memory for the function structure.  We need to
     save/restore CFUN because it gets clobbered by the call to
     allocate_struct_function.  */
  saved_cfun = cfun;
  allocate_struct_function (decl);
  DECL_SOURCE_LOCATION (decl) = input_location;
  cfun->function_end_locus = input_location;
  cfun = saved_cfun;

  ri_p->omp_fn = decl;
  ri_p->omp_data_receive = t;
}


/* A subroutine of split_out_parallel_function.  For all variables in the
   block tree rooted at BLOCK with DECL_CONTEXT of the current function,
   update the context to FN.  */

static void
recontext_vars_in_block (tree block, tree fn)
{
  tree t;

  while (block)
    {
      for (t = BLOCK_VARS (block); t ; t = TREE_CHAIN (t))
	if (DECL_CONTEXT (t) == current_function_decl)
	  DECL_CONTEXT (t) = fn;

      recontext_vars_in_block (BLOCK_SUBBLOCKS (block), fn);
      block = BLOCK_CHAIN (block);
    }
}


/* Splice the body of an OMP_PARALLEL into a new function.  On return,
   the parallel at *STMT_P will have been replaced with an empty BIND_EXPR.

   The child function will have been created, with a BIND_EXPR as the
   DECL_SAVED_TREE.  This bind will contain only another BIND_EXPR that
   contains the old body of the parallel.  This makes it easy to insert
   new code before or after that body.  */

static tree
split_out_parallel_function (tree stmt, struct remap_info_d *ri_p)
{
  tree child_bind, parent_bind, root_bind;
  tree child_block, parent_block;

  /* The OMP_PARALLEL is expected to have exactly one BIND_EXPR as its body. */
  child_bind = expr_only (OMP_PARALLEL_BODY (stmt));
  child_block = BIND_EXPR_BLOCK (child_bind);

  /* Create the empty replacement for the statement in the parent.  */
  parent_block = make_node (BLOCK);
  parent_bind = build3 (BIND_EXPR, void_type_node, NULL, NULL, parent_block);
  TREE_SIDE_EFFECTS (parent_bind) = 1;
  SET_EXPR_LOCUS (parent_bind, EXPR_LOCUS (stmt));

  if (ri_p->omp_data_send)
    {
      BLOCK_VARS (parent_block) = ri_p->omp_data_send;
      BIND_EXPR_VARS (parent_bind) = ri_p->omp_data_send;
    }

  /* Stuff the child bind_expr inside another bind_expr that forms the true
     body of the child function.  */
  root_bind = build3 (BIND_EXPR, void_type_node,
		      BIND_EXPR_VARS (child_bind), NULL,
		      BIND_EXPR_BLOCK (child_bind));
  TREE_SIDE_EFFECTS (root_bind) = 1;
  SET_EXPR_LOCUS (root_bind, EXPR_LOCUS (stmt));
  append_to_statement_list_force (child_bind, &BIND_EXPR_BODY (root_bind));
  BIND_EXPR_VARS (child_bind) = NULL;
  BIND_EXPR_BLOCK (child_bind) = NULL;
  DECL_SAVED_TREE (ri_p->omp_fn) = root_bind;

  /* Move everything in the child block tree to the new function.  */
  BLOCK_SUPERCONTEXT (child_block) = ri_p->omp_fn;
  DECL_INITIAL (ri_p->omp_fn) = child_block;
  recontext_vars_in_block (child_block, ri_p->omp_fn);

  return parent_bind;
}


/* Based on the contents of RI_P, generate a code sequence to initialize
   the OMP_DATA variable with any addresses, and with copy-in data.  The
   code is appended to *STMT_LIST, and is not gimplified.  */

static int
emit_sender_copyin_1 (splay_tree_node n, void *data)
{
  struct remap_info_d *ri_p = data;
  tree var, field, ompd, ref, t;
  bool shared, use_ptr, copyin;

  var = (tree) n->key;
  field = (tree) n->value;

  shared = bitmap_bit_p (ri_p->shared, DECL_UID (var));
  use_ptr = use_pointer_for_field (var, shared);
  copyin = use_ptr || bitmap_bit_p (ri_p->copyin, DECL_UID (var));

  if (copyin)
    {
      ompd = ri_p->omp_data_send;
      ref = build (COMPONENT_REF, TREE_TYPE (field), ompd, field, NULL);

      t = use_ptr ? build_fold_addr_expr (var) : var;
      t = build2 (MODIFY_EXPR, void_type_node, ref, t);
      tsi_link_after (&ri_p->tsi, t, TSI_CONTINUE_LINKING);
    }

  return 0;
}

static void
emit_sender_copyin (struct remap_info_d *ri_p)
{
  splay_tree_foreach (ri_p->map, emit_sender_copyin_1, ri_p);
}

/* Based on the contents of RI_P, generate a code sequence to copy-out
   variables from the OMP_DATA variable.  The code is appended to
   *STMT_LIST, and is not gimplified.  */

static int
emit_sender_copyout_1 (splay_tree_node n, void *data)
{
  struct remap_info_d *ri_p = data;
  tree var, field, ompd, ref, t;
  bool shared, use_ptr, copyout;

  var = (tree) n->key;
  field = (tree) n->value;

  shared = bitmap_bit_p (ri_p->shared, DECL_UID (var));
  use_ptr = use_pointer_for_field (var, shared);
  copyout = !use_ptr && bitmap_bit_p (ri_p->copyout, DECL_UID (var));

  if (copyout)
    {
      ompd = ri_p->omp_data_send;
      ref = build (COMPONENT_REF, TREE_TYPE (field), ompd, field, NULL);

      t = build2 (MODIFY_EXPR, void_type_node, var, ref);
      tsi_link_after (&ri_p->tsi, t, TSI_CONTINUE_LINKING);
    }

  return 0;
}

static void
emit_sender_copyout (struct remap_info_d *ri_p)
{
  splay_tree_foreach (ri_p->map, emit_sender_copyout_1, ri_p);
}


/* Build an expression to specify the number of threads to use in the runtime.
   Returns the argument that should be passed to GOMP_parallel_start.  */

static tree
compute_num_threads (tree par_stmt)
{
  tree clauses = OMP_PARALLEL_CLAUSES (par_stmt);
  tree val, cond, c;

  /* By default, the value of NUM_THREADS is zero (selected at run time)
     and there is no conditional.  */
  cond = NULL_TREE;
  val = build_int_cst (unsigned_type_node, 0);

  c = find_omp_clause (clauses, OMP_CLAUSE_IF);
  if (c)
    cond = OMP_CLAUSE_IF_EXPR (c);

  c = find_omp_clause (clauses, OMP_CLAUSE_NUM_THREADS);
  if (c)
    val = OMP_CLAUSE_NUM_THREADS_EXPR (c);

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

  return val;
}


/* Emit the sequence for launching a thread team.  */

static void
emit_parallel_start_end (tree par_stmt, struct remap_info_d *ri_p)
{
  tree t, args;

  t = compute_num_threads (par_stmt);
  args = tree_cons (NULL, t, NULL);
  t = ri_p->omp_data_send;
  if (t == NULL)
    t = null_pointer_node;
  else
    t = build_fold_addr_expr (t);
  args = tree_cons (NULL, t, args);
  t = build_fold_addr_expr (ri_p->omp_fn);
  args = tree_cons (NULL, t, args);
  t = built_in_decls[BUILT_IN_GOMP_PARALLEL_START];
  t = build_function_call_expr (t, args);
  tsi_link_after (&ri_p->tsi, t, TSI_CONTINUE_LINKING);
  
  t = ri_p->omp_data_send;
  if (t == NULL)
    t = null_pointer_node;
  else
    t = build_fold_addr_expr (t);
  args = tree_cons (NULL, t, NULL);
  t = build_function_call_expr (ri_p->omp_fn, args);
  tsi_link_after (&ri_p->tsi, t, TSI_CONTINUE_LINKING);

  t = built_in_decls[BUILT_IN_GOMP_PARALLEL_END];
  t = build_function_call_expr (t, NULL);
  tsi_link_after (&ri_p->tsi, t, TSI_CONTINUE_LINKING);
}
  

static void
emit_omp_parallel_parent (tree par_stmt, tree bind_expr,
			  struct remap_info_d *ri_p)
{
  BIND_EXPR_BODY (bind_expr) = alloc_stmt_list ();
  ri_p->tsi = tsi_last (BIND_EXPR_BODY (bind_expr));

  emit_sender_copyin (ri_p);
  emit_parallel_start_end (par_stmt, ri_p);
  emit_sender_copyout (ri_p);
}


/* Prepare DECL_VALUE_EXPR for the shared variables in the child.  */

static void
setup_decl_value_expr_child (tree par_stmt, struct remap_info_d *ri_p)
{
  tree c;

  for (c = OMP_PARALLEL_CLAUSES (par_stmt); c ; c = OMP_CLAUSE_CHAIN (c))
    {
      tree outer, inner;
      splay_tree_node n;
      tree field, ref;

      if (TREE_CODE (c) != OMP_CLAUSE_SHARED)
	continue;

      outer = OMP_CLAUSE_OUTER_DECL (c);
      inner = OMP_CLAUSE_INNER_DECL (c);

      n = splay_tree_lookup (ri_p->map, (splay_tree_key) outer);
      field = (tree) n->value;

      ref = build_fold_indirect_ref (ri_p->omp_data_receive);
      ref = build (COMPONENT_REF, TREE_TYPE (field), ref, field, NULL);
      if (use_pointer_for_field (outer, true))
	ref = build_fold_indirect_ref (ref);

      SET_DECL_VALUE_EXPR (inner, ref);
      DECL_HAS_VALUE_EXPR_P (inner) = 1;
    }
}


/* Walk all statements in STMT_LIST replacing variables as specified in
   RI_P->MAP as for the receiver.  */

static tree
remap_variables_receiver (tree *tp, int *walk_subtrees, void *data)
{
  struct remap_info_d *ri_p = data;
  tree var = *tp;
  splay_tree_node n;
  tree field, x;

  if (TYPE_P (var))
    {
      *walk_subtrees = 0;
      return NULL_TREE;
    }
  if (!DECL_P (var))
    return NULL_TREE;

  *walk_subtrees = 0;

  n = splay_tree_lookup (ri_p->map, (splay_tree_key) var);
  if (n == NULL)
    return NULL_TREE;

  field = (tree) n->value;
  x = build_fold_indirect_ref (ri_p->omp_data_receive);
  x = build (COMPONENT_REF, TREE_TYPE (field), x, field, NULL);
  if (use_pointer_for_field (var, false))
    x = build_fold_indirect_ref (x);
  *tp = x;

  return NULL_TREE;
}

/* Grr.  All variables in the child function are present in the BLOCK tree,
   and we re-contexted them in recontext_vars_in_block.  But the labels
   don't appear anywhere but the body of the child, so we have to walk it
   just for that.  And to make things worse, the child function has not been
   gimplified, so we can't use walk_stmts.  */

static tree
remap_labels_child_1 (tree *tp, int *walk_subtrees, void *data)
{
  tree child_fn = data;
  tree t = *tp;

  if (TREE_CODE (t) == LABEL_DECL
      && DECL_CONTEXT (t) == current_function_decl)
    DECL_CONTEXT (t) = child_fn;

  if (DECL_P (t) || TYPE_P (t))
    *walk_subtrees = 0;

  return NULL_TREE;
}

static void
remap_labels_child (tree fn)
{
  walk_tree_without_duplicates (&DECL_SAVED_TREE (fn),
				remap_labels_child_1, fn);
}


/* Grr.  The sequence for lastprivate on a parallel for is not in the
   parallel node itself.  Nor should it be, since it needs to be placed
   by the loop code.  But we need to invoke remap_variables_receiver on it.

   Grovel it out.  At this point the child function consists of a root
   BIND_EXPR, and within that the original BIND_EXPR of the parallel.  So
   if this is a combined parallel for, the single component of that second
   BIND_EXPR should be an OMP_FOR node.  */

static tree *
get_lastprivate_sequence (tree_stmt_iterator *tsi)
{
  tree t;

  t = tsi_stmt (*tsi);
  t = expr_only (BIND_EXPR_BODY (t));

  if (t == NULL)
    return NULL;
  if (TREE_CODE (t) == OMP_FOR)
    return &OMP_FOR_VAR_LAST (t);
  else if (TREE_CODE (t) == OMP_SECTIONS)
    return &OMP_SECTIONS_VAR_LAST (t);

  return NULL;
}

/* Lower the OpenMP parallel directive pointed by TSI.  Build a new
   function with the body of the pragma and emit the appropriate
   runtime call.  */

static void
lower_omp_parallel (tree *stmt_p)
{
  tree par_stmt = *stmt_p;
  tree bind_stmt, fn, *lastpriv;
  struct remap_info_d *ri_p;
  tree_stmt_iterator fn_tsi;

  ri_p = new_remap_info ();

  setup_data_fields (par_stmt, ri_p);
  create_child_function (ri_p);

  *stmt_p = bind_stmt = split_out_parallel_function (par_stmt, ri_p);
  emit_omp_parallel_parent (par_stmt, bind_stmt, ri_p);
  push_gimplify_context ();
  gimplify_to_stmt_list (&BIND_EXPR_BODY (bind_stmt));
  pop_gimplify_context (bind_stmt);

  fn = ri_p->omp_fn;
  fn_tsi = tsi_start (BIND_EXPR_BODY (DECL_SAVED_TREE (fn)));

  lastpriv = get_lastprivate_sequence (&fn_tsi);
  if (lastpriv)
    walk_tree (lastpriv, remap_variables_receiver, ri_p, NULL);

  if (OMP_PARALLEL_VAR_INIT (par_stmt))
    {
      walk_tree (&OMP_PARALLEL_VAR_INIT (par_stmt),
		 remap_variables_receiver, ri_p, NULL);
      tsi_link_before (&fn_tsi, OMP_PARALLEL_VAR_INIT (par_stmt),
		       TSI_SAME_STMT);
    }

  if (OMP_PARALLEL_VAR_REDUC (par_stmt))
    {
      tree reduc;

      walk_tree (&OMP_PARALLEL_VAR_REDUC (par_stmt),
		 remap_variables_receiver, ri_p, NULL);

      /* ??? Need to do something about the atomic-ness of the reduction.
	 For now assume we'll be fine re-using a critical section.  */
      reduc = OMP_PARALLEL_VAR_REDUC (par_stmt);
      reduc = build2 (OMP_CRITICAL, void_type_node, NULL, reduc);
      tsi_link_after (&fn_tsi, reduc, TSI_SAME_STMT);
    }

  remap_labels_child (fn);
  setup_decl_value_expr_child (par_stmt, ri_p);

  delete_remap_info (ri_p);

  cgraph_add_new_function (fn);
}


/* Lower OpenMP directives in CURRENT_FUNCTION_DECL.  */

static void
lower_omp_1 (tree *stmt_p)
{
  switch (TREE_CODE (*stmt_p))
    {
    case OMP_PARALLEL:
      lower_omp_parallel (stmt_p);
      break;

    case STATEMENT_LIST:
      {
	tree_stmt_iterator i;
	for (i = tsi_start (*stmt_p); !tsi_end_p (i); tsi_next (&i))
	  lower_omp_1 (tsi_stmt_ptr (i));
      }
      break;

    case BIND_EXPR:
      lower_omp_1 (&BIND_EXPR_BODY (*stmt_p));
      break;
    case COND_EXPR:
      lower_omp_1 (&COND_EXPR_THEN (*stmt_p));
      lower_omp_1 (&COND_EXPR_ELSE (*stmt_p));
      break;
    case TRY_FINALLY_EXPR:
    case TRY_CATCH_EXPR:
      lower_omp_1 (&TREE_OPERAND (*stmt_p, 0));
      lower_omp_1 (&TREE_OPERAND (*stmt_p, 1));
      break;
    case CATCH_EXPR:
      lower_omp_1 (&CATCH_BODY (*stmt_p));
      break;
    case EH_FILTER_EXPR:
      lower_omp_1 (&EH_FILTER_FAILURE (*stmt_p));
      break;

    case RETURN_EXPR:
    case NOP_EXPR:
    case ASM_EXPR:
    case MODIFY_EXPR:
    case CALL_EXPR:
    case GOTO_EXPR:
    case LABEL_EXPR:
    case SWITCH_EXPR:
      break;

    default:
      gcc_unreachable ();
    }
}

static void
lower_omp (void)
{
  lower_omp_1 (&DECL_SAVED_TREE (current_function_decl));
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
