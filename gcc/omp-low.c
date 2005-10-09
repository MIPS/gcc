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
#include "tree-gimple.h"
#include "tree-inline.h"
#include "langhooks.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "timevar.h"
#include "except.h"
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "toplev.h"
#include "tree-pass.h"


/* Lowering of OpenMP parallel and workshare constructs proceeds in two 
   phases.  The first phase scans the function looking for OMP statements
   and then for variables that must be replaced to satisfy data sharing
   clauses.  The second phase expands code for the constructs, as well as
   re-gimplifing things when variables have been replaced with complex
   expressions.

   Lowering of a parallel statement results in the contents of the 
   parallel being moved to a new function, to be invoked by the thread
   library.  The variable remapping process is complex enough that only
   one level of parallel statement is handled at one time.  If there are
   nested parallel statements, those nested statements are handled when
   the new function is lowered and optimized.  The result is not 100%
   optimal, but lexically nested parallels effectively only happens in
   test suites.  */


typedef struct omp_context
{
  /* This field must be at the beginning, as we do "inheritence".  */
  copy_body_data cb;

  /* The tree of contexts corresponding to the encountered constructs.  */
  struct omp_context *outer;
  tree stmt;

  /* Map variables to fields in a structure that allows communication 
     between sending and receiving threads.  */
  splay_tree field_map;
  tree record_type;
  tree sender_decl;
  tree receiver_decl;

  /* A chain of variables to add to the top-level block surrounding the
     construct.  In the case of a parallel, this is in the child function.  */
  tree block_vars;

  /* What to do with variables with implicitly determined sharing
     attributes.  */
  enum omp_clause_default_kind default_kind;

  bool is_parallel;
} omp_context;

static splay_tree all_contexts;
static int parallel_nesting_level;

static void scan_omp (tree *, omp_context *);
static void expand_omp (tree *, omp_context *);


/* Find an OpenMP clause of type KIND within CLAUSES.  */

tree
find_omp_clause (tree clauses, enum tree_code kind)
{
  for (; clauses ; clauses = OMP_CLAUSE_CHAIN (clauses))
    if (TREE_CODE (clauses) == kind)
      return clauses;

  return NULL_TREE;
}

/* Return true if CTX is for an omp parallel.  */

static inline bool
is_parallel_ctx (omp_context *ctx)
{
  return ctx->is_parallel;
}

/* Lookup variables in the decl or field splay trees.  The "maybe" form
   allows for the variable form to not have been entered, otherwise we
   assert that the variable must have been entered.  */

static inline tree
lookup_decl (tree var, omp_context *ctx)
{
  splay_tree_node n;
  n = splay_tree_lookup (ctx->cb.decl_map, (splay_tree_key) var);
  return (tree) n->value;
}

static inline tree
maybe_lookup_decl (tree var, omp_context *ctx)
{
  splay_tree_node n;
  n = splay_tree_lookup (ctx->cb.decl_map, (splay_tree_key) var);
  return n ? (tree) n->value : NULL_TREE;
}

static inline tree
lookup_field (tree var, omp_context *ctx)
{
  splay_tree_node n;
  n = splay_tree_lookup (ctx->field_map, (splay_tree_key) var);
  return (tree) n->value;
}

static inline tree
maybe_lookup_field (tree var, omp_context *ctx)
{
  splay_tree_node n;
  n = splay_tree_lookup (ctx->field_map, (splay_tree_key) var);
  return n ? (tree) n->value : NULL_TREE;
}

/* Return true if DECL should be copied by pointer.  */

static bool
use_pointer_for_field (tree decl, bool shared_p)
{
  if (AGGREGATE_TYPE_P (TREE_TYPE (decl)))
    return true;

  /* We can only use copy-in/copy-out semantics for shared varibles
     when we know the value is not accessible from an outer scope.  */
  if (shared_p)
    {
      /* ??? Trivially accessible from anywhere.  But why would we even
	 be passing an address in this case?  Should we simply assert
	 this to be false, or should we have a cleanup pass that removes
	 these from the list of mappings?  */
      if (TREE_STATIC (decl) || DECL_EXTERNAL (decl))
	return true;

      /* For variables with DECL_HAS_VALUE_EXPR_P set, we cannot tell
	 without analyzing the expression whether or not its location
	 is accessible to anyone else.  In the case of nested parallel
	 regions it certainly may be.  */
      if (DECL_HAS_VALUE_EXPR_P (decl))
	return true;
    }

  return false;
}

/* Construct a new automatic decl similar to VAR.  */

static tree
omp_copy_decl_1 (tree var, omp_context *ctx)
{
  tree copy = build_decl (VAR_DECL, DECL_NAME (var), TREE_TYPE (var));

  TREE_ADDRESSABLE (copy) = TREE_ADDRESSABLE (var);
  DECL_COMPLEX_GIMPLE_REG_P (copy) = DECL_COMPLEX_GIMPLE_REG_P (var);
  DECL_ARTIFICIAL (copy) = DECL_ARTIFICIAL (var);
  DECL_IGNORED_P (copy) = DECL_IGNORED_P (var);
  DECL_ABSTRACT_ORIGIN (copy) = DECL_ORIGIN (var);
  TREE_USED (copy) = 1;
  DECL_CONTEXT (copy) = ctx->cb.dst_fn;
  DECL_SEEN_IN_BIND_EXPR_P (copy) = 1;

  TREE_CHAIN (copy) = ctx->block_vars;
  ctx->block_vars = copy;

  return copy;
}

/* Build tree nodes to access the field for VAR on the receiver side.  */

static tree
build_receiver_ref (tree var, bool by_ref, omp_context *ctx)
{
  tree x, field = lookup_field (var, ctx);

  x = build_fold_indirect_ref (ctx->receiver_decl);
  x = build3 (COMPONENT_REF, TREE_TYPE (field), x, field, NULL);
  if (by_ref)
    x = build_fold_indirect_ref (x);

  return x;
}

/* Build tree nodes to access VAR in the scope outer to CTX.  In the case
   of a parallel, this is a component reference; for workshare constructs
   this is some variable.  */

static tree
build_outer_var_ref (tree var, omp_context *ctx)
{
  if (is_parallel_ctx (ctx))
    {
      bool by_ref = use_pointer_for_field (var, false);
      return build_receiver_ref (var, by_ref, ctx);
    }
  else if (ctx->outer)
    return lookup_decl (var, ctx->outer);
  else
    return var;
}

/* Build tree nodes to access the field for VAR on the sender side.  */

static tree
build_sender_ref (tree var, omp_context *ctx)
{
  tree field = lookup_field (var, ctx);
  return build3 (COMPONENT_REF, TREE_TYPE (field),
		 ctx->sender_decl, field, NULL);
}

/* Add a new field for VAR inside the structure CTX->SENDER_DECL.  */

static void
install_var_field (tree var, bool by_ref, omp_context *ctx)
{
  tree field, type;

  /* We can have both firstprivate and lastprivate on a parallel.
     Avoid creating two fields.  */
  if (maybe_lookup_field (var, ctx) != NULL)
    return;

  type = TREE_TYPE (var);
  if (by_ref)
    type = build_pointer_type (type);

  field = build_decl (FIELD_DECL, DECL_NAME (var), type);
  insert_field_into_struct (ctx->record_type, field);

  splay_tree_insert (ctx->field_map, (splay_tree_key) var,
		     (splay_tree_value) field);
}

static tree
install_var_new (tree new_var, tree var, omp_context *ctx)
{
  insert_decl_map (&ctx->cb, var, new_var);
  return new_var;
}

/* Install in CTX a new variable to stand as a local private for VAR.  */

static tree
install_var_private (tree var, omp_context *ctx)
{
  tree new_var = omp_copy_decl_1 (var, ctx);
  return install_var_new (new_var, var, ctx);
}

/* Install in CTX a substitution to implement shared semantics for VAR.  */

static tree
install_var_shared (tree var, bool by_ref, omp_context *ctx)
{
  tree x = build_receiver_ref (var, by_ref, ctx);
  tree new_var = omp_copy_decl_1 (var, ctx);
  SET_DECL_VALUE_EXPR (new_var, x);
  DECL_HAS_VALUE_EXPR_P (new_var) = 1;
  return install_var_new (new_var, var, ctx);
}

/* Return true if VAR is private in the context of the enclosing parallel.  */

static bool
is_private_var (tree var, omp_context *ctx)
{
  tree new_var;

  while (1)
    {
      new_var = maybe_lookup_decl (var, ctx);
      if (new_var)
	return !DECL_HAS_VALUE_EXPR_P (new_var);
      if (is_parallel_ctx (ctx))
	break;
      ctx = ctx->outer;

      /* If we reached the top of the contexts without hitting a parallel,
	 then the original context is not lexically nested within a parallel
 	 and any local variable is automatically private.  */
      if (ctx == NULL)
	return !is_global_var (var);
    }

  if (DECL_ARTIFICIAL (var))
    return false;
  else
    return ctx->default_kind == OMP_CLAUSE_DEFAULT_PRIVATE;
}

/* The callback for remap_decl.  Search all containing contexts for a
   mapping of the variable; this avoids having to duplicate the splay
   tree ahead of time.  We know a mapping doesn't already exist in the
   given context.  Create new mappings to implement default semantics.  */

static tree
omp_copy_decl (tree var, copy_body_data *cb)
{
  omp_context *ctx = (omp_context *) cb;
  enum omp_clause_default_kind default_kind;
  tree new_var;
  bool by_ref;

  if (decl_function_context (var) != ctx->cb.src_fn)
    return var;

  while (!is_parallel_ctx (ctx))
    {
      ctx = ctx->outer;
      if (ctx == NULL)
	return var;
      new_var = maybe_lookup_decl (var, ctx);
      if (new_var)
	return new_var;
    }

  if (TREE_CODE (var) == LABEL_DECL)
    {
      new_var = create_artificial_label ();
      DECL_CONTEXT (new_var) = ctx->cb.dst_fn;
      insert_decl_map (&ctx->cb, var, new_var);
      return new_var;
    }

  /* ??? Some compiler-generated variables (like TYPE_SIZE_UNIT SAVE_EXPRs)
     could be remapped firstprivate instead of shared.  */
  default_kind = ctx->default_kind;
  if (DECL_ARTIFICIAL (var))
    default_kind = OMP_CLAUSE_DEFAULT_SHARED;

  switch (default_kind)
    {
    case OMP_CLAUSE_DEFAULT_NONE:
      error ("%qE not specified in enclosing parallel", var);
      error ("%Henclosing parallel", EXPR_LOCUS (ctx->stmt));
      /* FALLTHRU */

    case OMP_CLAUSE_DEFAULT_SHARED:
      by_ref = use_pointer_for_field (var, true);
      install_var_field (var, by_ref, ctx);
      return install_var_shared (var, by_ref, ctx);

    case OMP_CLAUSE_DEFAULT_PRIVATE:
      return install_var_private (var, ctx);

    default:
      gcc_unreachable ();
    }
}

/* Create a new context, with OUTER_CTX being the surrounding context.  */

static omp_context *
new_omp_context (tree stmt, omp_context *outer_ctx)
{
  omp_context *ctx = XCNEW (omp_context);

  splay_tree_insert (all_contexts, (splay_tree_key) stmt,
		     (splay_tree_value) ctx);
  ctx->stmt = stmt;

  if (outer_ctx)
    {
      ctx->outer = outer_ctx;
      ctx->cb = outer_ctx->cb;
      ctx->cb.block = NULL;
    }
  else
    {
      ctx->cb.src_fn = current_function_decl;
      ctx->cb.dst_fn = current_function_decl;
      ctx->cb.src_node = cgraph_node (current_function_decl);
      ctx->cb.dst_node = ctx->cb.src_node;
      ctx->cb.src_cfun = cfun;
      ctx->cb.copy_decl = omp_copy_decl;
      ctx->cb.eh_region = -1;
      ctx->cb.transform_call_graph_edges = CB_CGE_MOVE;
    }

  ctx->cb.decl_map = splay_tree_new (splay_tree_compare_pointers, 0, 0);

  return ctx;
}

/* Destroy a omp_context data structures.  Called through the splay tree
   value delete callback.  */

static void
delete_omp_context (splay_tree_value value)
{
  omp_context *ctx = (omp_context *) value;

  splay_tree_delete (ctx->cb.decl_map);
  if (ctx->field_map)
    splay_tree_delete (ctx->field_map);
  XDELETE (ctx);
}

/* Instantiate decls as necessary in CTX to satisfy the data sharing
   specified by CLAUSES.  */

static void
scan_sharing_clauses (tree *pclauses, omp_context *ctx)
{
  tree c, *pc;

  for (pc = pclauses; (c = *pc) != NULL ; pc = &OMP_CLAUSE_CHAIN (c))
    {
      tree decl;
      bool by_ref;

      switch (TREE_CODE (c))
	{
	case OMP_CLAUSE_PRIVATE:
	  install_var_private (OMP_CLAUSE_DECL (c), ctx);
	  break;

	case OMP_CLAUSE_FIRSTPRIVATE:
	case OMP_CLAUSE_LASTPRIVATE:
	case OMP_CLAUSE_REDUCTION:
	  decl = OMP_CLAUSE_DECL (c);
	  if (is_parallel_ctx (ctx))
	    {
	      by_ref = use_pointer_for_field (decl, false);
	      install_var_field (decl, by_ref, ctx);
	    }
	  /* If we're in a nested context, force a remapping in the outer
	     context.  We'll need this mapping later when we emit code.  */
	  else if (ctx->outer)
	    omp_copy_decl (decl, &ctx->outer->cb);
	  install_var_private (decl, ctx);
	  break;

	case OMP_CLAUSE_SHARED:
	  gcc_assert (is_parallel_ctx (ctx));
	  decl = OMP_CLAUSE_DECL (c);
	  by_ref = use_pointer_for_field (decl, true);
	  install_var_field (decl, by_ref, ctx);
	  install_var_shared (decl, by_ref, ctx);
	  break;

	case OMP_CLAUSE_COPYPRIVATE:
	  scan_omp (pc, ctx);
	  /* FALLTHRU */

	case OMP_CLAUSE_COPYIN:
	  decl = OMP_CLAUSE_DECL (c);
	  by_ref = use_pointer_for_field (decl, false);
	  install_var_field (decl, by_ref, ctx);
	  break;

	case OMP_CLAUSE_DEFAULT:
	  ctx->default_kind = OMP_CLAUSE_DEFAULT_KIND (c);
	  break;

	case OMP_CLAUSE_IF:
	case OMP_CLAUSE_NUM_THREADS:
	case OMP_CLAUSE_SCHEDULE:
	  scan_omp (pc, ctx);
	  break;

	case OMP_CLAUSE_NOWAIT:
	case OMP_CLAUSE_ORDERED:
	  break;

	default:
	  gcc_unreachable ();
	}
    }
}

/* Build a decl for the omp child function.  It'll not contain a body
   yet, just the bare decl.  */

static void
create_omp_child_function (omp_context *ctx)
{
  tree decl, ptype, type, name, t;

  ptype = build_pointer_type (ctx->record_type);

  name = create_tmp_var_name ("__omp_fn");
  type = build_function_type_list (void_type_node, ptype, NULL_TREE);

  decl = build_decl (FUNCTION_DECL, name, type);
  ctx->cb.dst_fn = decl;

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
  ctx->receiver_decl = t;

  /* Allocate memory for the function structure.  The call to 
     allocate_struct_function clobbers cfun, so we need to restore
     it afterward.  */
  allocate_struct_function (decl);
  DECL_SOURCE_LOCATION (decl) = input_location;
  cfun->function_end_locus = input_location;
  cfun = ctx->cb.src_cfun;
}

/* Scan an OpenMP parallel directive.  */

static void
scan_omp_parallel (tree *stmt_p, omp_context *outer_ctx)
{
  omp_context *ctx;

  ctx = new_omp_context (*stmt_p, outer_ctx);
  ctx->field_map = splay_tree_new (splay_tree_compare_pointers, 0, 0);
  ctx->is_parallel = true;
  ctx->default_kind = OMP_CLAUSE_DEFAULT_SHARED;
  ctx->record_type = make_node (RECORD_TYPE);
  TYPE_NAME (ctx->record_type) = create_tmp_var_name (".omp_data_s");
  create_omp_child_function (ctx);

  scan_sharing_clauses (&OMP_PARALLEL_CLAUSES (*stmt_p), ctx);
  scan_omp (&OMP_PARALLEL_BODY (*stmt_p), ctx);

  if (TYPE_FIELDS (ctx->record_type) == NULL)
    ctx->record_type = ctx->receiver_decl = NULL;
  else
    layout_type (ctx->record_type);
}

/* Scan an OpenMP loop directive.  */

static void
scan_omp_for (tree *stmt_p, omp_context *outer_ctx)
{
  omp_context *ctx;
  tree iter, stmt = *stmt_p;

  ctx = new_omp_context (stmt, outer_ctx);
  scan_sharing_clauses (&OMP_FOR_CLAUSES (stmt), ctx);

  /* Make sure the iteration variable is private.  */
  iter = TREE_OPERAND (OMP_FOR_INIT (stmt), 0);
  if (!is_private_var (iter, ctx))
    {
      omp_context *pctx;
      for (pctx = ctx; !is_parallel_ctx (pctx); pctx = pctx->outer)
	continue;
      install_var_private (iter, pctx);
    }

  scan_omp (&OMP_FOR_INIT (stmt), ctx);
  scan_omp (&OMP_FOR_COND (stmt), ctx);
  scan_omp (&OMP_FOR_INCR (stmt), ctx);
  scan_omp (&OMP_FOR_BODY (stmt), ctx);
}

/* Scan an OpenMP sections directive.  */

static void
scan_omp_sections (tree *stmt_p, omp_context *outer_ctx)
{
  tree stmt = *stmt_p;
  omp_context *ctx;

  ctx = new_omp_context (stmt, outer_ctx);
  scan_sharing_clauses (&OMP_SECTIONS_CLAUSES (stmt), ctx);
  scan_omp (&OMP_SECTIONS_BODY (stmt), ctx);
}

/* Scan an OpenMP single directive.  */

static void
scan_omp_single (tree *stmt_p, omp_context *outer_ctx)
{
  tree stmt = *stmt_p;
  omp_context *ctx;

  ctx = new_omp_context (stmt, outer_ctx);
  ctx->field_map = splay_tree_new (splay_tree_compare_pointers, 0, 0);
  ctx->record_type = make_node (RECORD_TYPE);
  TYPE_NAME (ctx->record_type) = create_tmp_var_name (".omp_copy_s");

  scan_sharing_clauses (&OMP_SINGLE_CLAUSES (stmt), ctx);
  scan_omp (&OMP_SINGLE_BODY (stmt), ctx);

  if (TYPE_FIELDS (ctx->record_type) == NULL)
    ctx->record_type = NULL;
  else
    layout_type (ctx->record_type);
}

/* Lower OpenMP directives in CURRENT_FUNCTION_DECL.  */

static tree
scan_omp_1 (tree *tp, int *walk_subtrees, void *data)
{
  struct walk_stmt_info *wi = data;
  omp_context *ctx = wi->info;
  tree t = *tp;

  if (EXPR_HAS_LOCATION (t))
    input_location = EXPR_LOCATION (t);

  *walk_subtrees = 0;
  switch (TREE_CODE (t))
    {
    case OMP_PARALLEL:
      switch (parallel_nesting_level)
	{
	case 0:
	  parallel_nesting_level = 1;
	  scan_omp_parallel (tp, ctx);
	  parallel_nesting_level = 0;
	  break;
	case 1:
	  parallel_nesting_level = 2;
	  walk_tree (&OMP_PARALLEL_CLAUSES (t), scan_omp_1, wi, NULL);
	  walk_tree (&OMP_PARALLEL_BODY (t), scan_omp_1, wi, NULL);
	  parallel_nesting_level = 1;
	  break;
	default:
	  *walk_subtrees = 1;
	  break;
	}
      break;

    case OMP_FOR:
      if (parallel_nesting_level <= 1)
	scan_omp_for (tp, ctx);
      else
	*walk_subtrees = 1;
      break;

    case OMP_SECTIONS:
      if (parallel_nesting_level <= 1)
	scan_omp_sections (tp, ctx);
      else
	*walk_subtrees = 1;
      break;

    case OMP_SINGLE:
      if (parallel_nesting_level <= 1)
	scan_omp_single (tp, ctx);
      else
	*walk_subtrees = 1;
      break;

    case BIND_EXPR:
      {
	tree var;
	*walk_subtrees = 1;

	for (var = BIND_EXPR_VARS (t); var ; var = TREE_CHAIN (var))
	  {
	    if (DECL_CONTEXT (var) == ctx->cb.src_fn)
	      DECL_CONTEXT (var) = ctx->cb.dst_fn;
	    insert_decl_map (&ctx->cb, var, var);
	  }
      }
      break;

    case VAR_DECL:
    case PARM_DECL:
    case LABEL_DECL:
      if (ctx)
	*tp = remap_decl (t, &ctx->cb);
      break;

    default:
      if (ctx && TYPE_P (t))
	*tp = remap_type (t, &ctx->cb);
      else if (!DECL_P (t))
	*walk_subtrees = 1;
      break;
    }

  return NULL_TREE;
}

static void
scan_omp (tree *stmt_p, omp_context *ctx)
{
  location_t saved_location;
  struct walk_stmt_info wi;

  memset (&wi, 0, sizeof (wi));
  wi.callback = scan_omp_1;
  wi.info = ctx;
  wi.val_only = true;
  wi.want_bind_expr = (ctx != NULL);
  wi.want_locations = true;

  saved_location = input_location;
  walk_stmts (&wi, stmt_p);
  input_location = saved_location;
}

/* Re-gimplification and code generation routines.  */

/* If a context was created for STMT when it was scanned, return it.  */

static omp_context *
maybe_lookup_ctx (tree stmt)
{
  splay_tree_node n;
  n = splay_tree_lookup (all_contexts, (splay_tree_key) stmt);
  return n ? (omp_context *) n->value : NULL;
}

/* Construct the initialization value for reduction CLAUSE.  */

static tree
build_reduction_init (tree clause)
{
  tree type = TREE_TYPE (OMP_CLAUSE_DECL (clause));

  switch (OMP_CLAUSE_REDUCTION_CODE (clause))
    {
    case PLUS_EXPR:
    case MINUS_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_ORIF_EXPR:
    case TRUTH_XOR_EXPR:
      return fold_convert (type, integer_zero_node);

    case MULT_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_ANDIF_EXPR:
      return fold_convert (type, integer_one_node);

    case BIT_AND_EXPR:
      return fold_convert (type, integer_minus_one_node);

    case MAX_EXPR:
      if (SCALAR_FLOAT_TYPE_P (type))
	gcc_unreachable (); /* FIXME */
      else
	{
	  gcc_assert (INTEGRAL_TYPE_P (type));
	  return TYPE_MIN_VALUE (type);
	}

    case MIN_EXPR:
      if (SCALAR_FLOAT_TYPE_P (type))
	gcc_unreachable (); /* FIXME */
      else
	{
	  gcc_assert (INTEGRAL_TYPE_P (type));
	  return TYPE_MAX_VALUE (type);
	}

    default:
      gcc_unreachable ();
    }
}

/* Generate code to implement the input clauses, FIRSTPRIVATE and COPYIN,
   from the receiver (aka child) side.  */

static void
expand_rec_input_clauses (tree clauses, tree *stmt_list, omp_context *ctx)
{
  tree c;

  for (c = clauses; c ; c = OMP_CLAUSE_CHAIN (c))
    {
      tree var, new_var, x;
      bool by_ref;

      switch (TREE_CODE (c))
	{
	case OMP_CLAUSE_FIRSTPRIVATE:
	  var = OMP_CLAUSE_DECL (c);
	  new_var = lookup_decl (var, ctx);
	  x = build_outer_var_ref (var, ctx);
	  break;

	case OMP_CLAUSE_COPYIN:
	  new_var = var = OMP_CLAUSE_DECL (c);
	  by_ref = use_pointer_for_field (var, false);
	  x = build_receiver_ref (var, by_ref, ctx);
	  break;

	case OMP_CLAUSE_REDUCTION:
	  var = OMP_CLAUSE_DECL (c);
	  new_var = lookup_decl (var, ctx);
	  x = build_reduction_init (c);
	  continue;
	  
	default:
	  continue;
	}

      x = build2 (MODIFY_EXPR, void_type_node, new_var, x);
      gimplify_and_add (x, stmt_list);
    }
}

/* Generate code to implement the LASTPRIVATE clauses.  This is used for
   both parallel and workshare constructs.  PREDICATE may be NULL if it's
   always true.   */

static void
expand_lastprivate_clauses (tree clauses, tree predicate, tree *stmt_list,
			    omp_context *ctx)
{
  tree sub_list, x, c;

  /* Early exit if there are no lastprivate clauses.  */
  clauses = find_omp_clause (clauses, OMP_CLAUSE_LASTPRIVATE);
  if (clauses == NULL)
    {
      /* If this was a workshare clause, see if it had been combined
	 with its parallel.  In that case, look for the clauses on the
	 parallel statement itself.  */
      if (is_parallel_ctx (ctx))
	return;

      ctx = ctx->outer;
      if (ctx == NULL || !is_parallel_ctx (ctx))
	return;

      clauses = find_omp_clause (OMP_PARALLEL_CLAUSES (ctx->stmt),
				 OMP_CLAUSE_LASTPRIVATE);
      if (clauses == NULL)
	return;
    }

  sub_list = alloc_stmt_list ();

  for (c = clauses; c ; c = OMP_CLAUSE_CHAIN (c))
    {
      tree var, new_var;

      if (TREE_CODE (c) != OMP_CLAUSE_LASTPRIVATE)
	continue;

      var = OMP_CLAUSE_DECL (c);
      new_var = lookup_decl (var, ctx);
      x = build_outer_var_ref (var, ctx);
      x = build2 (MODIFY_EXPR, void_type_node, x, new_var);
      append_to_statement_list (x, &sub_list);
    }

  if (predicate)
    x = build3 (COND_EXPR, void_type_node, predicate, sub_list, NULL);
  else
    x = sub_list;
  gimplify_and_add (x, stmt_list);
}

/* Generate code to implement the REDUCTION clauses.  */

static void
expand_reduction_clauses (tree clauses, tree *stmt_list, omp_context *ctx)
{
  tree sub_list = NULL, x, c;

  for (c = clauses; c ; c = OMP_CLAUSE_CHAIN (c))
    {
      tree var, ref, new_var;

      if (TREE_CODE (c) != OMP_CLAUSE_REDUCTION)
	continue;

      var = OMP_CLAUSE_DECL (c);
      new_var = lookup_decl (var, ctx);

      ref = build_outer_var_ref (var, ctx);
      x = build2 (OMP_CLAUSE_REDUCTION_CODE (c),
		  TREE_TYPE (ref), ref, new_var);
      ref = build_outer_var_ref (var, ctx);
      x = build2 (MODIFY_EXPR, void_type_node, ref, x);
      append_to_statement_list (x, &sub_list);
    }

  if (sub_list == NULL)
    return;

  /* ??? Maybe transform to atomic operations.  */

  x = built_in_decls[BUILT_IN_GOMP_CRITICAL_START];
  x = build_function_call_expr (x, NULL);
  gimplify_and_add (x, stmt_list);

  gimplify_and_add (sub_list, stmt_list);

  x = built_in_decls[BUILT_IN_GOMP_CRITICAL_END];
  x = build_function_call_expr (x, NULL);
  gimplify_and_add (x, stmt_list);
}

/* Generate code to implement the COPYPRIVATE clauses.  */

static void
expand_copyprivate_clauses (tree clauses, tree *slist, tree *rlist,
			    omp_context *ctx)
{
  tree c;

  for (c = clauses; c ; c = OMP_CLAUSE_CHAIN (c))
    {
      tree var, ref, x;
      bool by_ref;

      if (TREE_CODE (c) != OMP_CLAUSE_COPYPRIVATE)
	continue;

      var = OMP_CLAUSE_DECL (c);
      by_ref = use_pointer_for_field (var, false);

      ref = build_sender_ref (var, ctx);
      x = by_ref ? build_fold_addr_expr (var) : var;
      x = build2 (MODIFY_EXPR, void_type_node, ref, x);
      gimplify_and_add (x, slist);

      ref = build_receiver_ref (var, by_ref, ctx);
      x = build2 (MODIFY_EXPR, void_type_node, var, ref);
      gimplify_and_add (x, rlist);
    }
}

/* Generate code to implement the clauses, FIRSTPRIVATE, COPYIN, LASTPRIVATE,
   and REDUCTION from the sender (aka parent) side.  */

static void
expand_send_clauses (tree clauses, tree *ilist, tree *olist, omp_context *ctx)
{
  tree c;

  for (c = clauses; c ; c = OMP_CLAUSE_CHAIN (c))
    {
      tree val, ref, x;
      bool by_ref, do_in = false, do_out = false;

      switch (TREE_CODE (c))
	{
	case OMP_CLAUSE_FIRSTPRIVATE:
	case OMP_CLAUSE_COPYIN:
	  val = OMP_CLAUSE_DECL (c);
	  by_ref = use_pointer_for_field (val, false);
	  do_in = true;
	  break;

	case OMP_CLAUSE_LASTPRIVATE:
	  val = OMP_CLAUSE_DECL (c);
	  by_ref = use_pointer_for_field (val, false);
	  if (by_ref)
	    do_in = true;
	  else
	    do_out = true;
	  break;

	case OMP_CLAUSE_REDUCTION:
	  val = OMP_CLAUSE_DECL (c);
	  by_ref = use_pointer_for_field (val, false);
	  do_in = true;
	  do_out = !by_ref;
	  break;
	default:
	  continue;
	}

      if (do_in)
	{
	  ref = build_sender_ref (val, ctx);
	  x = by_ref ? build_fold_addr_expr (val) : val;
	  x = build2 (MODIFY_EXPR, void_type_node, ref, x);
	  gimplify_and_add (x, ilist);
	}
      if (do_out)
	{
	  ref = build_sender_ref (val, ctx);
	  x = build2 (MODIFY_EXPR, void_type_node, val, ref);
	  gimplify_and_add (x, olist);
	}
    }
}

/* Generate code to implement SHARED from the sender (aka parent) side.
   This is trickier, since OMP_PARALLEL_CLAUSES doesn't list things that
   got automatically shared.  */

static int
expand_send_shared_vars_1 (splay_tree_node n, void *data)
{
  void **xdata = data;
  omp_context *ctx = xdata[0];
  tree *ilist = xdata[1];
  tree *olist = xdata[2];
  tree ovar, nvar, x;
  
  ovar = (tree) n->key;
  nvar = maybe_lookup_decl (ovar, ctx);
  if (nvar && DECL_HAS_VALUE_EXPR_P (nvar))
    {
      if (use_pointer_for_field (ovar, true))
	{
	  x = build_sender_ref (ovar, ctx);
	  ovar = build_fold_addr_expr (ovar);
	  x = build2 (MODIFY_EXPR, void_type_node, x, ovar);
	  gimplify_and_add (x, ilist);
	}
      else
	{
	  x = build_sender_ref (ovar, ctx);
	  x = build2 (MODIFY_EXPR, void_type_node, x, ovar);
	  gimplify_and_add (x, ilist);

	  x = build_sender_ref (ovar, ctx);
	  x = build2 (MODIFY_EXPR, void_type_node, ovar, x);
	  gimplify_and_add (x, olist);
	}
    }
  return 0;
}

static void
expand_send_shared_vars (tree *ilist, tree *olist, omp_context *ctx)
{
  void *xdata[3];

  xdata[0] = ctx;
  xdata[1] = ilist;
  xdata[2] = olist;
  splay_tree_foreach (ctx->field_map, expand_send_shared_vars_1, xdata);
}

/* Build the function calls to GOMP_parallel_start etc to actually 
   generate the parallel operation.  */

static void
build_parallel_call (tree clauses, tree *stmt_list, omp_context *ctx)
{
  tree t, args, val, cond, c;

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

  args = tree_cons (NULL, val, NULL);
  t = ctx->sender_decl;
  if (t == NULL)
    t = null_pointer_node;
  else
    t = build_fold_addr_expr (t);
  args = tree_cons (NULL, t, args);
  t = build_fold_addr_expr (ctx->cb.dst_fn);
  args = tree_cons (NULL, t, args);
  t = built_in_decls[BUILT_IN_GOMP_PARALLEL_START];
  t = build_function_call_expr (t, args);
  gimplify_and_add (t, stmt_list);

  t = ctx->sender_decl;
  if (t == NULL)
    t = null_pointer_node;
  else
    t = build_fold_addr_expr (t);
  args = tree_cons (NULL, t, NULL);
  t = build_function_call_expr (ctx->cb.dst_fn, args);
  gimplify_and_add (t, stmt_list);

  t = built_in_decls[BUILT_IN_GOMP_PARALLEL_END];
  t = build_function_call_expr (t, NULL);
  gimplify_and_add (t, stmt_list);
}

/* Expand an OpenMP parallel directive.  */

static void
expand_omp_parallel (tree *stmt_p, omp_context *ctx)
{
  tree clauses, block, bind, body, olist;

  current_function_decl = ctx->cb.dst_fn;
  cfun = DECL_STRUCT_FUNCTION (current_function_decl);

  push_gimplify_context ();

  expand_omp (&OMP_PARALLEL_BODY (*stmt_p), ctx);

  clauses = OMP_PARALLEL_CLAUSES (*stmt_p);
  bind = OMP_PARALLEL_BODY (*stmt_p);
  block = BIND_EXPR_BLOCK (bind);
  body = BIND_EXPR_BODY (bind);
  BIND_EXPR_BODY (bind) = alloc_stmt_list ();

  expand_rec_input_clauses (clauses, &BIND_EXPR_BODY (bind), ctx);
  append_to_statement_list (body, &BIND_EXPR_BODY (bind));
  expand_reduction_clauses (clauses, &BIND_EXPR_BODY (bind), ctx);

  pop_gimplify_context (bind);
  BIND_EXPR_VARS (bind) = chainon (BIND_EXPR_VARS (bind), ctx->block_vars);
  BLOCK_VARS (block) = BIND_EXPR_VARS (bind);

  DECL_INITIAL (ctx->cb.dst_fn) = block;
  DECL_SAVED_TREE (ctx->cb.dst_fn) = bind;
  cgraph_add_new_function (ctx->cb.dst_fn);

  current_function_decl = ctx->cb.src_fn;
  cfun = DECL_STRUCT_FUNCTION (current_function_decl);

  block = make_node (BLOCK);
  bind = build3 (BIND_EXPR, void_type_node, NULL, NULL, block);
  *stmt_p = bind;

  push_gimplify_context ();

  /* Build the sender decl now that we're in the correct context.  */
  if (ctx->record_type)
    ctx->sender_decl = create_tmp_var (ctx->record_type, ".omp_data_o");

  olist = NULL;
  expand_send_clauses (clauses, &BIND_EXPR_BODY (bind), &olist, ctx);
  expand_send_shared_vars (&BIND_EXPR_BODY (bind), &olist, ctx);
  build_parallel_call (clauses, &BIND_EXPR_BODY (bind), ctx);
  append_to_statement_list (olist, &BIND_EXPR_BODY (bind));

  pop_gimplify_context (bind);
  BLOCK_VARS (block) = BIND_EXPR_VARS (bind);
}

/* Build a call to GOMP_barrier.  */

static void
build_omp_barrier (tree *stmt_list)
{
  tree t;

  t = built_in_decls[BUILT_IN_GOMP_BARRIER];
  t = build_function_call_expr (t, NULL);
  gimplify_and_add (t, stmt_list);
}

/* A structure to communicate between the various subroutines of 
   expand_omp_for_1.  */

struct expand_omp_for_data
{
  tree v, n1, n2, step, chunk_size, for_stmt;
  enum tree_code cond_code;
  tree pre;
  omp_context *ctx;
};

/* A subroutine of expand_omp_for_1.  Generate code to emit the
   for for a lastprivate clause.  Given a loop control predicate
   of (V cond N2), we gate the clause on (!(V cond N2)).  */

static void
expand_omp_for_lastprivate (struct expand_omp_for_data *fd)
{
  tree clauses, cond;
  enum tree_code cond_code;
  
  cond_code = fd->cond_code;
  cond_code = cond_code == LT_EXPR ? GE_EXPR : LE_EXPR;

  /* When possible, use a strict equality expression.  This can let VRP
     type optimizations deduce the value and remove a copy.  */
  if (host_integerp (fd->step, 0))
    {
      HOST_WIDE_INT step = TREE_INT_CST_LOW (fd->step);
      if (step == 1 || step == -1)
	cond_code = EQ_EXPR;
    }

  cond = build2 (cond_code, boolean_type_node, fd->v, fd->n2);

  clauses = OMP_FOR_CLAUSES (fd->for_stmt);
  expand_lastprivate_clauses (clauses, cond, &fd->pre, fd->ctx);
}

/* A subroutine of expand_omp_for_1.  Generate code for a parallel
   loop with any schedule.  Given parameters:

	for (V = N1; V cond N2; V += STEP) BODY;

   where COND is "<" or ">", we generate pseudocode

	more = GOMP_loop_foo_start (N1, N2, STEP, CHUNK, &istart0, &iend0);
	if (more) goto L0; else goto L3;
    L0:
	V = istart0;
	iend = iend0;
    L1:
	BODY;
	V += STEP;
	if (V cond iend) goto L1;
	more = GOMP_loop_foo_next (&istart0, &iend0);
	if (more) goto L0;
	lastprivate;
    L3:
*/

static void
expand_omp_for_generic (struct expand_omp_for_data *fd,
			enum built_in_function start_fn,
			enum built_in_function next_fn)
{
  tree l0, l1, l3;
  tree type, istart0, iend0, iend;
  tree t, args;

  type = TREE_TYPE (fd->v);

  istart0 = create_tmp_var (long_integer_type_node, ".istart0");
  iend0 = create_tmp_var (long_integer_type_node, ".istart0");

  l0 = create_artificial_label ();
  l1 = create_artificial_label ();
  l3 = create_artificial_label ();
  iend = create_tmp_var (type, NULL);

  t = build_fold_addr_expr (iend0);
  args = tree_cons (NULL, t, NULL);
  t = build_fold_addr_expr (istart0);
  args = tree_cons (NULL, t, args);
  if (fd->chunk_size)
    {
      t = fold_convert (long_integer_type_node, fd->chunk_size);
      args = tree_cons (NULL, t, args);
    }
  t = fold_convert (long_integer_type_node, fd->step);
  args = tree_cons (NULL, t, args);
  t = fold_convert (long_integer_type_node, fd->n2);
  args = tree_cons (NULL, t, args);
  t = fold_convert (long_integer_type_node, fd->n1);
  args = tree_cons (NULL, t, args);
  t = build_function_call_expr (built_in_decls[start_fn], args);
  t = build3 (COND_EXPR, void_type_node, t,
	      build_and_jump (&l0), build_and_jump (&l3));
  gimplify_and_add (t, &fd->pre);

  t = build1 (LABEL_EXPR, void_type_node, l0);
  gimplify_and_add (t, &fd->pre);

  t = fold_convert (type, istart0);
  t = build2 (MODIFY_EXPR, void_type_node, fd->v, t);
  gimplify_and_add (t, &fd->pre);

  t = fold_convert (type, iend0);
  t = build2 (MODIFY_EXPR, void_type_node, iend, t);
  gimplify_and_add (t, &fd->pre);

  t = build1 (LABEL_EXPR, void_type_node, l1);
  gimplify_and_add (t, &fd->pre);

  append_to_statement_list (OMP_FOR_BODY (fd->for_stmt), &fd->pre);

  t = build2 (PLUS_EXPR, type, fd->v, fd->step);
  t = build2 (MODIFY_EXPR, void_type_node, fd->v, t);
  gimplify_and_add (t, &fd->pre);
  
  t = build2 (fd->cond_code, boolean_type_node, fd->v, iend);
  t = build3 (COND_EXPR, void_type_node, t, build_and_jump (&l1), NULL);
  gimplify_and_add (t, &fd->pre);

  t = build_fold_addr_expr (iend0);
  args = tree_cons (NULL, t, NULL);
  t = build_fold_addr_expr (istart0);
  args = tree_cons (NULL, t, args);
  t = build_function_call_expr (built_in_decls[next_fn], args);
  t = build3 (COND_EXPR, void_type_node, t, build_and_jump (&l0), NULL);
  gimplify_and_add (t, &fd->pre);

  expand_omp_for_lastprivate (fd);
  
  t = build1 (LABEL_EXPR, void_type_node, l3);
  gimplify_and_add (t, &fd->pre);
}

/* A subroutine of expand_omp_for_1.  Generate code for a parallel
   loop with static schedule and no specified chunk size.  Given parameters:

	for (V = N1; V cond N2; V += STEP) BODY;

   where COND is "<" or ">", we generate pseudocode

	if (cond is <)
	  adj = STEP - 1;
	else
	  adj = STEP + 1;
	n = (adj + N2 - N1) / STEP;
	q = n / nthreads;
	q += (q * nthreads != n);
	s0 = q * threadid;
	e0 = min(s0 + q, n);
	if (s0 >= e0) goto L2; else goto L0;
    L0:
	V = s0 * STEP + N1;
	e = e0 * STEP + N1;
    L1:
	BODY;
	V += STEP;
	if (V cond e) goto L1;
	lastprivate;
    L2:
*/

static void
expand_omp_for_static_nochunk (struct expand_omp_for_data *fd)
{
  tree l0, l1, l2, n, q, s0, e0, e, t, nthreads, threadid;
  tree type, utype;

  l0 = create_artificial_label ();
  l1 = create_artificial_label ();
  l2 = create_artificial_label ();
  
  type = TREE_TYPE (fd->v);
  utype = lang_hooks.types.unsigned_type (type);

  t = built_in_decls[BUILT_IN_OMP_GET_NUM_THREADS];
  t = build_function_call_expr (t, NULL);
  t = fold_convert (utype, t);
  nthreads = get_formal_tmp_var (t, &fd->pre);
  
  t = built_in_decls[BUILT_IN_OMP_GET_THREAD_NUM];
  t = build_function_call_expr (t, NULL);
  t = fold_convert (utype, t);
  threadid = get_formal_tmp_var (t, &fd->pre);

  fd->n1 = fold_convert (type, fd->n1);
  if (!is_gimple_val (fd->n1))
    fd->n1 = get_formal_tmp_var (fd->n1, &fd->pre);

  fd->n2 = fold_convert (type, fd->n2);
  if (!is_gimple_val (fd->n2))
    fd->n2 = get_formal_tmp_var (fd->n2, &fd->pre);

  fd->step = fold_convert (type, fd->step);
  if (!is_gimple_val (fd->step))
    fd->step = get_formal_tmp_var (fd->step, &fd->pre);

  t = build_int_cst (type, (fd->cond_code == LT_EXPR ? -1 : 1));
  t = fold_build2 (PLUS_EXPR, type, fd->step, t);
  t = fold_build2 (PLUS_EXPR, type, t, fd->n2);
  t = fold_build2 (MINUS_EXPR, type, t, fd->n1);
  t = fold_build2 (TRUNC_DIV_EXPR, type, t, fd->step);
  t = fold_convert (utype, t);
  if (is_gimple_val (t))
    n = t;
  else
    n = get_formal_tmp_var (t, &fd->pre);

  t = build2 (TRUNC_DIV_EXPR, utype, n, nthreads);
  q = get_formal_tmp_var (t, &fd->pre);

  t = build2 (MULT_EXPR, utype, q, nthreads);
  t = build2 (NE_EXPR, utype, t, n);
  t = build2 (PLUS_EXPR, utype, q, t);
  q = get_formal_tmp_var (t, &fd->pre);

  t = build2 (MULT_EXPR, utype, q, threadid);
  s0 = get_formal_tmp_var (t, &fd->pre);

  t = build2 (PLUS_EXPR, utype, s0, q);
  t = build2 (MIN_EXPR, utype, t, n);
  e0 = get_formal_tmp_var (t, &fd->pre);

  t = build2 (GE_EXPR, boolean_type_node, s0, e0);
  t = build3 (COND_EXPR, void_type_node, t,
	      build_and_jump (&l2), build_and_jump (&l0));
  gimplify_and_add (t, &fd->pre);

  t = build1 (LABEL_EXPR, void_type_node, l0);
  gimplify_and_add (t, &fd->pre);

  t = fold_convert (type, s0);
  t = build2 (MULT_EXPR, type, t, fd->step);
  t = build2 (PLUS_EXPR, type, t, fd->n1);
  t = build2 (MODIFY_EXPR, void_type_node, fd->v, t);
  gimplify_and_add (t, &fd->pre);

  t = fold_convert (type, e0);
  t = build2 (MULT_EXPR, type, t, fd->step);
  t = build2 (PLUS_EXPR, type, t, fd->n1);
  e = get_formal_tmp_var (t, &fd->pre);

  t = build1 (LABEL_EXPR, void_type_node, l1);
  gimplify_and_add (t, &fd->pre);

  append_to_statement_list (OMP_FOR_BODY (fd->for_stmt), &fd->pre);

  t = build2 (PLUS_EXPR, type, fd->v, fd->step);
  t = build2 (MODIFY_EXPR, void_type_node, fd->v, t);
  gimplify_and_add (t, &fd->pre);

  t = build2 (fd->cond_code, boolean_type_node, fd->v, e);
  t = build3 (COND_EXPR, void_type_node, t, build_and_jump (&l1), NULL);
  gimplify_and_add (t, &fd->pre);

  expand_omp_for_lastprivate (fd);
  
  t = build1 (LABEL_EXPR, void_type_node, l2);
  gimplify_and_add (t, &fd->pre);
}

/* A subroutine of expand_omp_for_1.  Generate code for a parallel
   loop with static schedule and a specified chunk size.  Given parameters:

	for (V = N1; V cond N2; V += STEP) BODY;

   where COND is "<" or ">", we generate pseudocode

	if (cond is <)
	  adj = STEP - 1;
	else
	  adj = STEP + 1;
	n = (adj + N2 - N1) / STEP;
	trip = 0;
    L0:
	s0 = (trip * nthreads + threadid) * CHUNK;
	e0 = min(s0 + CHUNK, n);
	if (s0 < n) goto L1; else goto L4;
    L1:
	V = s0 * STEP + N1;
	e = e0 * STEP + N1;
    L2:
	BODY;
	V += STEP;
	if (V cond e) goto L2; else goto L3;
    L3:
	trip += 1;
	goto L0;
    L4:
	if (trip == 0) goto L5;
	lastprivate;
    L5:
*/

static void
expand_omp_for_static_chunk (struct expand_omp_for_data *fd)
{
  tree l0, l1, l2, l3, l4, l5, n, s0, e0, e, t;
  tree trip, nthreads, threadid;
  tree type, utype;

  l0 = create_artificial_label ();
  l1 = create_artificial_label ();
  l2 = create_artificial_label ();
  l3 = create_artificial_label ();
  l4 = create_artificial_label ();
  l5 = create_artificial_label ();
  
  type = TREE_TYPE (fd->v);
  utype = lang_hooks.types.unsigned_type (type);

  t = built_in_decls[BUILT_IN_OMP_GET_NUM_THREADS];
  t = build_function_call_expr (t, NULL);
  t = fold_convert (utype, t);
  nthreads = get_formal_tmp_var (t, &fd->pre);
  
  t = built_in_decls[BUILT_IN_OMP_GET_THREAD_NUM];
  t = build_function_call_expr (t, NULL);
  t = fold_convert (utype, t);
  threadid = get_formal_tmp_var (t, &fd->pre);

  fd->n1 = fold_convert (type, fd->n1);
  if (!is_gimple_val (fd->n1))
    fd->n1 = get_formal_tmp_var (fd->n1, &fd->pre);

  fd->n2 = fold_convert (type, fd->n2);
  if (!is_gimple_val (fd->n2))
    fd->n2 = get_formal_tmp_var (fd->n2, &fd->pre);

  fd->step = fold_convert (type, fd->step);
  if (!is_gimple_val (fd->step))
    fd->step = get_formal_tmp_var (fd->step, &fd->pre);

  fd->chunk_size = fold_convert (utype, fd->chunk_size);
  if (!is_gimple_val (fd->chunk_size))
    fd->chunk_size = get_formal_tmp_var (fd->chunk_size, &fd->pre);

  t = build_int_cst (type, (fd->cond_code == LT_EXPR ? -1 : 1));
  t = fold_build2 (PLUS_EXPR, type, fd->step, t);
  t = fold_build2 (PLUS_EXPR, type, t, fd->n2);
  t = fold_build2 (MINUS_EXPR, type, t, fd->n1);
  t = fold_build2 (TRUNC_DIV_EXPR, type, t, fd->step);
  t = fold_convert (utype, t);
  if (is_gimple_val (t))
    n = t;
  else
    n = get_formal_tmp_var (t, &fd->pre);

  t = build_int_cst (utype, 0);
  trip = get_initialized_tmp_var (t, &fd->pre, NULL);

  t = build1 (LABEL_EXPR, void_type_node, l0);
  gimplify_and_add (t, &fd->pre);

  t = build2 (MULT_EXPR, utype, trip, nthreads);
  t = build2 (PLUS_EXPR, utype, t, threadid);
  t = build2 (MULT_EXPR, utype, t, fd->chunk_size);
  s0 = get_formal_tmp_var (t, &fd->pre);

  t = build2 (PLUS_EXPR, utype, s0, fd->chunk_size);
  t = build2 (MIN_EXPR, utype, t, n);
  e0 = get_formal_tmp_var (t, &fd->pre);

  t = build2 (LT_EXPR, boolean_type_node, s0, n);
  t = build3 (COND_EXPR, void_type_node, t,
	      build_and_jump (&l1), build_and_jump (&l4));
  gimplify_and_add (t, &fd->pre);

  t = build1 (LABEL_EXPR, void_type_node, l1);
  gimplify_and_add (t, &fd->pre);

  t = fold_convert (type, s0);
  t = build2 (MULT_EXPR, type, t, fd->step);
  t = build2 (PLUS_EXPR, type, t, fd->n1);
  t = build2 (MODIFY_EXPR, void_type_node, fd->v, t);
  gimplify_and_add (t, &fd->pre);

  t = fold_convert (type, e0);
  t = build2 (MULT_EXPR, type, t, fd->step);
  t = build2 (PLUS_EXPR, type, t, fd->n1);
  e = get_formal_tmp_var (t, &fd->pre);

  t = build1 (LABEL_EXPR, void_type_node, l2);
  gimplify_and_add (t, &fd->pre);

  append_to_statement_list (OMP_FOR_BODY (fd->for_stmt), &fd->pre);

  t = build2 (PLUS_EXPR, type, fd->v, fd->step);
  t = build2 (MODIFY_EXPR, void_type_node, fd->v, t);
  gimplify_and_add (t, &fd->pre);

  t = build2 (fd->cond_code, boolean_type_node, fd->v, e);
  t = build3 (COND_EXPR, void_type_node, t,
	      build_and_jump (&l2), build_and_jump (&l3));
  gimplify_and_add (t, &fd->pre);

  t = build1 (LABEL_EXPR, void_type_node, l3);
  gimplify_and_add (t, &fd->pre);

  t = build_int_cst (utype, 1);
  t = build2 (PLUS_EXPR, utype, trip, t);
  t = build2 (MODIFY_EXPR, void_type_node, trip, t);
  gimplify_and_add (t, &fd->pre);

  t = build1 (GOTO_EXPR, void_type_node, l0);
  gimplify_and_add (t, &fd->pre);

  t = build1 (LABEL_EXPR, void_type_node, l4);
  gimplify_and_add (t, &fd->pre);

  t = build_int_cst (utype, 0);
  t = build2 (EQ_EXPR, boolean_type_node, trip, t);
  t = build3 (COND_EXPR, void_type_node, t, build_and_jump (&l5), NULL);

  expand_omp_for_lastprivate (fd);
  
  t = build1 (LABEL_EXPR, void_type_node, l5);
  gimplify_and_add (t, &fd->pre);
}

/* A subroutine of expand_omp_for.  Expand the logic of the loop itself.  */

static tree
expand_omp_for_1 (tree *stmt_p, omp_context *ctx)
{
  struct expand_omp_for_data fd;
  bool have_nowait, have_ordered;
  enum omp_clause_schedule_kind sched_kind;
  tree t;

  fd.for_stmt = *stmt_p;
  fd.pre = NULL;
  fd.ctx = ctx;

  t = OMP_FOR_INIT (fd.for_stmt);
  gcc_assert (TREE_CODE (t) == MODIFY_EXPR);
  fd.v = TREE_OPERAND (t, 0);
  gcc_assert (DECL_P (fd.v));
  gcc_assert (TREE_CODE (TREE_TYPE (fd.v)) == INTEGER_TYPE);
  fd.n1 = TREE_OPERAND (t, 1);

  t = OMP_FOR_COND (fd.for_stmt);
  fd.cond_code = TREE_CODE (t);
  gcc_assert (TREE_OPERAND (t, 0) == fd.v);
  fd.n2 = TREE_OPERAND (t, 1);
  switch (fd.cond_code)
    {
    case LT_EXPR:
    case GT_EXPR:
      break;
    case LE_EXPR:
      fd.n2 = fold_build2 (PLUS_EXPR, TREE_TYPE (fd.n2), fd.n2,
			   build_int_cst (TREE_TYPE (fd.n2), 1));
      fd.cond_code = LT_EXPR;
      break;
    case GE_EXPR:
      fd.n2 = fold_build2 (MINUS_EXPR, TREE_TYPE (fd.n2), fd.n2,
			   build_int_cst (TREE_TYPE (fd.n2), 1));
      fd.cond_code = GT_EXPR;
      break;
    default:
      gcc_unreachable ();
    }

  t = OMP_FOR_INCR (fd.for_stmt);
  gcc_assert (TREE_CODE (t) == MODIFY_EXPR);
  gcc_assert (TREE_OPERAND (t, 0) == fd.v);
  t = TREE_OPERAND (t, 1);
  gcc_assert (TREE_OPERAND (t, 0) == fd.v);
  switch (TREE_CODE (t))
    {
    case PLUS_EXPR:
      fd.step = TREE_OPERAND (t, 1);
      break;
    case MINUS_EXPR:
      fd.step = TREE_OPERAND (t, 1);
      fd.step = fold_build1 (NEGATE_EXPR, TREE_TYPE (fd.step), fd.step);
      break;
    default:
      gcc_unreachable ();
    }

  have_nowait = have_ordered = false;
  sched_kind = OMP_CLAUSE_SCHEDULE_STATIC;
  fd.chunk_size = NULL_TREE;

  for (t = OMP_FOR_CLAUSES (fd.for_stmt); t ; t = OMP_CLAUSE_CHAIN (t))
    switch (TREE_CODE (t))
      {
      case OMP_CLAUSE_NOWAIT:
	have_nowait = true;
	break;
      case OMP_CLAUSE_ORDERED:
	have_ordered = true;
	break;
      case OMP_CLAUSE_SCHEDULE:
	sched_kind = OMP_CLAUSE_SCHEDULE_KIND (t);
	fd.chunk_size = OMP_CLAUSE_SCHEDULE_CHUNK_EXPR (t);
	break;
      default:
	break;
      }

  expand_rec_input_clauses (OMP_FOR_CLAUSES (fd.for_stmt), &fd.pre, ctx);

  if (sched_kind == OMP_CLAUSE_SCHEDULE_STATIC && !have_ordered)
    {
      if (fd.chunk_size == NULL)
	expand_omp_for_static_nochunk (&fd);
      else
	expand_omp_for_static_chunk (&fd);
    }
  else
    {
      int fn_index;

      if (sched_kind == OMP_CLAUSE_SCHEDULE_RUNTIME)
	gcc_assert (fd.chunk_size == NULL);
      else if (fd.chunk_size == NULL)
	fd.chunk_size = integer_zero_node;

      fn_index = sched_kind + have_ordered * 4;

      expand_omp_for_generic (&fd, BUILT_IN_GOMP_LOOP_STATIC_START + fn_index,
			     BUILT_IN_GOMP_LOOP_STATIC_NEXT + fn_index);
    }

  expand_reduction_clauses (OMP_FOR_CLAUSES (fd.for_stmt), &fd.pre, ctx);

  if (!have_nowait)
    build_omp_barrier (&fd.pre);

  return fd.pre;
}

/* Expand code for an OpenMP loop directive.  */

static void
expand_omp_for (tree *stmt_p, omp_context *ctx)
{
  tree bind, block, stmt_list;

  push_gimplify_context ();

  expand_omp (&OMP_FOR_BODY (*stmt_p), ctx);

  stmt_list = expand_omp_for_1 (stmt_p, ctx);
  block = make_node (BLOCK);
  bind = build3 (BIND_EXPR, void_type_node, NULL, stmt_list, block);
  *stmt_p = bind;

  pop_gimplify_context (bind);
  BIND_EXPR_VARS (bind) = chainon (BIND_EXPR_VARS (bind), ctx->block_vars);
  BLOCK_VARS (block) = BIND_EXPR_VARS (bind);
}

/* Expand code for an OpenMP sections directive.  In pseudo code, we generate

	firstprivate;
	v = GOMP_sections_start (n);
    L0:
	switch (v)
	  {
	  case 0:
	    goto L2;
	  case 1:
	    section 1;
	    goto L1;
	  case 2:
	    ...
	  case n:
	    ...
	    lastprivate;
	  default:
	    abort ();
	  }
    L1:
	v = GOMP_sections_next ();
	goto L0;
    L2:
	reduction;
*/

static void
expand_omp_sections (tree *stmt_p, omp_context *ctx)
{
  tree sec_stmt, label_vec, bind, block, stmt_list, l0, l1, l2, t, u, v;
  tree_stmt_iterator tsi;
  unsigned i, len;

  sec_stmt = *stmt_p;
  stmt_list = NULL;

  push_gimplify_context ();

  expand_rec_input_clauses (OMP_SECTIONS_CLAUSES (sec_stmt), &stmt_list, ctx);

  tsi = tsi_start (OMP_SECTIONS_BODY (sec_stmt));
  for (len = 0; !tsi_end_p (tsi); len++, tsi_next (&tsi))
    continue;

  l0 = create_artificial_label ();
  l1 = create_artificial_label ();
  l2 = create_artificial_label ();
  v = create_tmp_var (unsigned_type_node, ".section");
  label_vec = make_tree_vec (len + 2);

  t = build_int_cst (unsigned_type_node, len);
  t = tree_cons (NULL, t, NULL);
  u = built_in_decls[BUILT_IN_GOMP_SECTIONS_START];
  t = build_function_call_expr (u, t);
  t = build2 (MODIFY_EXPR, void_type_node, v, t);
  gimplify_and_add (t, &stmt_list);

  t = build1 (LABEL_EXPR, void_type_node, l0);
  gimplify_and_add (t, &stmt_list);

  t = build3 (SWITCH_EXPR, void_type_node, v, NULL, label_vec);
  gimplify_and_add (t, &stmt_list);

  t = build3 (CASE_LABEL_EXPR, void_type_node,
	      build_int_cst (unsigned_type_node, 0), NULL, l2);
  TREE_VEC_ELT (label_vec, 0) = t;
  
  tsi = tsi_start (OMP_SECTIONS_BODY (sec_stmt));
  for (i = 0; i < len; i++, tsi_next (&tsi))
    {
      t = create_artificial_label ();
      u = build_int_cst (unsigned_type_node, i + 1);
      u = build3 (CASE_LABEL_EXPR, void_type_node, u, NULL, t);
      TREE_VEC_ELT (label_vec, i + 1) = u;
      t = build1 (LABEL_EXPR, void_type_node, t);
      gimplify_and_add (t, &stmt_list);
  
      t = tsi_stmt (tsi);
      expand_omp (&OMP_SECTION_BODY (t), ctx);
      append_to_statement_list (OMP_SECTION_BODY (t), &stmt_list);

      if (i == len - 1)
	expand_lastprivate_clauses (OMP_SECTIONS_CLAUSES (sec_stmt),
				    NULL, &stmt_list, ctx);

      t = build1 (GOTO_EXPR, void_type_node, l1);
      gimplify_and_add (t, &stmt_list);
    }

  t = create_artificial_label ();
  u = build3 (CASE_LABEL_EXPR, void_type_node, NULL, NULL, t);
  TREE_VEC_ELT (label_vec, len + 1) = u;
  t = build1 (LABEL_EXPR, void_type_node, t);
  gimplify_and_add (t, &stmt_list);

  t = built_in_decls[BUILT_IN_TRAP];
  t = build_function_call_expr (t, NULL);
  gimplify_and_add (t, &stmt_list);

  t = build1 (LABEL_EXPR, void_type_node, l1);
  gimplify_and_add (t, &stmt_list);

  t = built_in_decls[BUILT_IN_GOMP_SECTIONS_NEXT];
  t = build_function_call_expr (t, NULL);
  t = build2 (MODIFY_EXPR, void_type_node, v, t);
  gimplify_and_add (t, &stmt_list);

  t = build1 (GOTO_EXPR, void_type_node, l0);
  gimplify_and_add (t, &stmt_list);

  t = build1 (LABEL_EXPR, void_type_node, l2);
  gimplify_and_add (t, &stmt_list);

  expand_reduction_clauses (OMP_SECTIONS_CLAUSES (sec_stmt), &stmt_list, ctx);

  /* Unless there's a nowait clause, add a barrier afterward.  */
  if (!find_omp_clause (OMP_SECTIONS_CLAUSES (sec_stmt), OMP_CLAUSE_NOWAIT))
    build_omp_barrier (&stmt_list);

  block = make_node (BLOCK);
  bind = build3 (BIND_EXPR, void_type_node, NULL, stmt_list, block);
  *stmt_p = bind;

  pop_gimplify_context (bind);
  BIND_EXPR_VARS (bind) = chainon (BIND_EXPR_VARS (bind), ctx->block_vars);
  BLOCK_VARS (block) = BIND_EXPR_VARS (bind);
}

/* A subroutine of expand_omp_single.  Expand the simple form of
   an OMP_SINGLE, without a copyprivate clause:

     	if (GOMP_single_start ())
	  BODY;
	[ GOMP_barrier (); ]	-> unless 'nowait' is present.
*/

static void
expand_omp_single_simple (tree single_stmt, tree *pre_p)
{
  tree t;

  t = built_in_decls[BUILT_IN_GOMP_SINGLE_START];
  t = build_function_call_expr (t, NULL);
  t = build3 (COND_EXPR, void_type_node, t,
	      OMP_SINGLE_BODY (single_stmt), NULL);
  gimplify_and_add (t, pre_p);

  if (!find_omp_clause (OMP_SINGLE_CLAUSES (single_stmt), OMP_CLAUSE_NOWAIT))
    build_omp_barrier (pre_p);
}

/* A subroutine of expand_omp_single.  Expand the simple form of
   an OMP_SINGLE, with a copyprivate clause:

	#pragma omp single copyprivate (a, b, c)

   Create a new structure to hold copies of 'a', 'b' and 'c' and emit:

      {
	if ((copyout_p = GOMP_single_copy_start ()) == NULL)
	  {
	    BODY;
	    copyout.a = a;
	    copyout.b = b;
	    copyout.c = c;
	    GOMP_single_copy_end (&copyout);
	  }
	else
	  {
	    a = copyout_p->a;
	    b = copyout_p->b;
	    c = copyout_p->c;
	  }
	GOMP_barrier ();
      }
*/

static void
expand_omp_single_copy (tree single_stmt, tree *pre_p, omp_context *ctx)
{
  tree ptr_type, t, args, l0, l1, l2, copyin_seq;

  ctx->sender_decl = create_tmp_var (ctx->record_type, ".omp_copy_o");

  ptr_type = build_pointer_type (ctx->record_type);
  ctx->receiver_decl = create_tmp_var (ptr_type, ".omp_copy_i");

  l0 = create_artificial_label ();
  l1 = create_artificial_label ();
  l2 = create_artificial_label ();

  /* Handle firstprivate clauses.  */
  expand_rec_input_clauses (OMP_SINGLE_CLAUSES (single_stmt), pre_p, ctx);

  t = built_in_decls[BUILT_IN_GOMP_SINGLE_COPY_START];
  t = build_function_call_expr (t, NULL);
  t = fold_convert (ptr_type, t);
  t = build2 (MODIFY_EXPR, void_type_node, ctx->receiver_decl, t);
  gimplify_and_add (t, pre_p);

  t = build (EQ_EXPR, boolean_type_node, ctx->receiver_decl,
	     build_int_cst (ptr_type, 0));
  t = build (COND_EXPR, void_type_node, t,
	     build_and_jump (&l0), build_and_jump (&l1));
  gimplify_and_add (t, pre_p);

  t = build1 (LABEL_EXPR, void_type_node, l0);
  gimplify_and_add (t, pre_p);

  append_to_statement_list (OMP_SINGLE_BODY (single_stmt), pre_p);

  copyin_seq = NULL;
  expand_copyprivate_clauses (OMP_SINGLE_CLAUSES (single_stmt), pre_p,
			      &copyin_seq, ctx);

  t = build_fold_addr_expr (ctx->sender_decl);
  args = tree_cons (NULL, t, NULL);
  t = built_in_decls[BUILT_IN_GOMP_SINGLE_COPY_END];
  t = build_function_call_expr (t, args);
  gimplify_and_add (t, pre_p);

  t = build_and_jump (&l2);
  gimplify_and_add (t, pre_p);

  t = build1 (LABEL_EXPR, void_type_node, l1);
  gimplify_and_add (t, pre_p);

  append_to_statement_list (copyin_seq, pre_p);

  t = build1 (LABEL_EXPR, void_type_node, l2);
  gimplify_and_add (t, pre_p);

  build_omp_barrier (pre_p);
}

/* Expand code for an OpenMP single directive.  */

static void
expand_omp_single (tree *stmt_p, omp_context *ctx)
{
  tree bind, block, single_stmt = *stmt_p;

  push_gimplify_context ();

  expand_omp (&OMP_SINGLE_BODY (single_stmt), ctx);

  block = make_node (BLOCK);
  bind = build3 (BIND_EXPR, void_type_node, NULL, NULL, block);
  *stmt_p = bind;

  if (ctx->record_type)
    expand_omp_single_copy (single_stmt, &BIND_EXPR_BODY (bind), ctx);
  else
    expand_omp_single_simple (single_stmt, &BIND_EXPR_BODY (bind));

  pop_gimplify_context (bind);
  BIND_EXPR_VARS (bind) = chainon (BIND_EXPR_VARS (bind), ctx->block_vars);
  BLOCK_VARS (block) = BIND_EXPR_VARS (bind);
}

/* Pass *TP back through the gimplifier within the context determined by WI.
   This handles replacement of DECL_VALUE_EXPR, as well as adjusting the 
   flags on ADDR_EXPR.  */

static void
expand_regimplify (tree *tp, struct walk_stmt_info *wi)
{
  enum gimplify_status gs;
  tree pre = NULL;

  if (wi->is_lhs)
    gs = gimplify_expr (tp, &pre, NULL, is_gimple_lvalue, fb_lvalue);
  else if (wi->val_only)
    gs = gimplify_expr (tp, &pre, NULL, is_gimple_val, fb_rvalue);
  else
    gs = gimplify_expr (tp, &pre, NULL, is_gimple_formal_tmp_var, fb_rvalue);
  gcc_assert (gs == GS_ALL_DONE);

  if (pre)
    tsi_link_before (&wi->tsi, pre, TSI_SAME_STMT);
}

static tree
expand_omp_1 (tree *tp, int *walk_subtrees, void *data)
{
  struct walk_stmt_info *wi = data;
  omp_context *ctx = wi->info;
  tree t = *tp;

  switch (TREE_CODE (*tp))
    {
    case OMP_PARALLEL:
      ctx = maybe_lookup_ctx (t);
      if (ctx)
	{
	  expand_omp_parallel (tp, ctx);
	  *walk_subtrees = 0;
	}
      break;

    case OMP_FOR:
      ctx = maybe_lookup_ctx (t);
      if (ctx)
	{
	  expand_omp_for (tp, ctx);
	  *walk_subtrees = 0;
	}
      break;

    case OMP_SECTIONS:
      ctx = maybe_lookup_ctx (t);
      if (ctx)
	{
	  expand_omp_sections (tp, ctx);
	  *walk_subtrees = 0;
	}
      break;

    case OMP_SINGLE:
      ctx = maybe_lookup_ctx (t);
      if (ctx)
	{
	  expand_omp_single (tp, ctx);
	  *walk_subtrees = 0;
	}
      break;

    case VAR_DECL:
      *walk_subtrees = 0;
      if (ctx && DECL_HAS_VALUE_EXPR_P (t))
	expand_regimplify (tp, wi);
      break;

    case ADDR_EXPR:
      *walk_subtrees = 0;
      if (ctx)
	expand_regimplify (tp, wi);
      break;

    case ARRAY_REF:
    case ARRAY_RANGE_REF:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
    case COMPONENT_REF:
    case VIEW_CONVERT_EXPR:
      *walk_subtrees = 0;
      if (ctx)
	expand_regimplify (tp, wi);
      break;

    default:
      if (TYPE_P (t) || DECL_P (t))
	*walk_subtrees = 0;
      break;
    }

  return NULL_TREE;
}

static void
expand_omp (tree *stmt_p, omp_context *ctx)
{
  struct walk_stmt_info wi;

  memset (&wi, 0, sizeof (wi));
  wi.callback = expand_omp_1;
  wi.info = ctx;
  wi.val_only = true;

  walk_stmts (&wi, stmt_p);
}

/* Main entry point.  */

static void
execute_lower_omp (void)
{
  all_contexts = splay_tree_new (splay_tree_compare_pointers, 0,
				 delete_omp_context);

  scan_omp (&DECL_SAVED_TREE (current_function_decl), NULL);
  gcc_assert (parallel_nesting_level == 0);

  if (all_contexts->root)
    expand_omp (&DECL_SAVED_TREE (current_function_decl), NULL);

  splay_tree_delete (all_contexts);
  all_contexts = NULL;
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
  execute_lower_omp,			/* execute */
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
