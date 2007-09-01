/* Tree lowering pass.  This pass converts the GENERIC functions-as-trees
   tree representation into the GIMPLE form.
   Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007
   Free Software Foundation, Inc.
   Major work done by Sebastian Pop <s.pop@laposte.net>,
   Diego Novillo <dnovillo@redhat.com> and Jason Merrill <jason@redhat.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "varray.h"
#include "tree-gimple.h"
#include "tree-inline.h"
#include "diagnostic.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "tree-flow.h"
#include "cgraph.h"
#include "timevar.h"
#include "except.h"
#include "hashtab.h"
#include "flags.h"
#include "real.h"
#include "function.h"
#include "output.h"
#include "expr.h"
#include "ggc.h"
#include "toplev.h"
#include "target.h"
#include "optabs.h"
#include "pointer-set.h"
#include "splay-tree.h"
#include "vec.h"
#include "gimple.h"


enum gimplify_omp_var_data
{
  GOVD_SEEN = 1,
  GOVD_EXPLICIT = 2,
  GOVD_SHARED = 4,
  GOVD_PRIVATE = 8,
  GOVD_FIRSTPRIVATE = 16,
  GOVD_LASTPRIVATE = 32,
  GOVD_REDUCTION = 64,
  GOVD_LOCAL = 128,
  GOVD_DEBUG_PRIVATE = 256,
  GOVD_DATA_SHARE_CLASS = (GOVD_SHARED | GOVD_PRIVATE | GOVD_FIRSTPRIVATE
			   | GOVD_LASTPRIVATE | GOVD_REDUCTION | GOVD_LOCAL)
};


struct gimplify_omp_ctx
{
  struct gimplify_omp_ctx *outer_context;
  splay_tree variables;
  struct pointer_set_t *privatized_types;
  location_t location;
  enum omp_clause_default_kind default_kind;
  bool is_parallel;
  bool is_combined_parallel;
};

struct gimplify_ctx
{
  struct gimplify_ctx *prev_context;

  VEC(gimple,heap) *bind_expr_stack;
  tree temps;
  struct gimple_sequence conditional_cleanups;
  tree exit_label;
  tree return_temp;
  
  VEC(tree,heap) *case_labels;
  /* The formal temporary table.  Should this be persistent?  */
  htab_t temp_htab;

  int conditions;
  bool save_stack;
  bool into_ssa;
};

static struct gimplify_ctx *gimplify_ctxp;
static struct gimplify_omp_ctx *gimplify_omp_ctxp;


/* Formal (expression) temporary table handling: Multiple occurrences of
   the same scalar expression are evaluated into the same temporary.  */

typedef struct gimple_temp_hash_elt
{
  tree val;   /* Key */
  tree temp;  /* Value */
} elt_t;

/* Forward declarations.  */
static enum gimplify_status gimplify_compound_expr (tree *, gimple_seq, bool);

/* Mark X addressable.  Unlike the langhook we expect X to be in gimple
   form and we don't do any syntax checking.  */
static void
mark_addressable (tree x)
{
  while (handled_component_p (x))
    x = TREE_OPERAND (x, 0);
  if (TREE_CODE (x) != VAR_DECL && TREE_CODE (x) != PARM_DECL)
    return ;
  TREE_ADDRESSABLE (x) = 1;
}

/* Return a hash value for a formal temporary table entry.  */

static hashval_t
gimple_tree_hash (const void *p)
{
  tree t = ((const elt_t *) p)->val;
  return iterative_hash_expr (t, 0);
}

/* Compare two formal temporary table entries.  */

static int
gimple_tree_eq (const void *p1, const void *p2)
{
  tree t1 = ((const elt_t *) p1)->val;
  tree t2 = ((const elt_t *) p2)->val;
  enum tree_code code = TREE_CODE (t1);

  if (TREE_CODE (t2) != code
      || TREE_TYPE (t1) != TREE_TYPE (t2))
    return 0;

  if (!operand_equal_p (t1, t2, 0))
    return 0;

  /* Only allow them to compare equal if they also hash equal; otherwise
     results are nondeterminate, and we fail bootstrap comparison.  */
  gcc_assert (gimple_tree_hash (p1) == gimple_tree_hash (p2));

  return 1;
}

/* Set up a context for the gimplifier.  */

void
push_gimplify_context (void)
{
  struct gimplify_ctx *c;

  c = (struct gimplify_ctx *) xcalloc (1, sizeof (struct gimplify_ctx));
  c->prev_context = gimplify_ctxp;
  c->bind_expr_stack = VEC_alloc (gimple, heap, 8);
  if (optimize)
    c->temp_htab = htab_create (1000, gimple_tree_hash, gimple_tree_eq, free);

  gimplify_ctxp = c;
}

/* Tear down a context for the gimplifier.  If BODY is non-null, then
   put the temporaries into the outer BIND_EXPR.  Otherwise, put them
   in the unexpanded_var_list.  */

void
pop_gimplify_context (gimple body)
{
  struct gimplify_ctx *c = gimplify_ctxp;
  tree t;

  gcc_assert (c && VEC_empty (gimple, c->bind_expr_stack));
  gimplify_ctxp = c->prev_context;

  for (t = c->temps; t ; t = TREE_CHAIN (t))
    DECL_GIMPLE_FORMAL_TEMP_P (t) = 0;

  if (body)
    declare_vars (c->temps, body, false);
  else
    record_vars (c->temps);

  if (optimize)
    htab_delete (c->temp_htab);
  free (c);
}

static void
gimple_push_bind_expr (gimple gimple_bind)
{
  VEC_safe_push (gimple, heap, gimplify_ctxp->bind_expr_stack, gimple_bind);
}

static void
gimple_pop_bind_expr (void)
{
  VEC_pop (gimple, gimplify_ctxp->bind_expr_stack);
}

gimple
gimple_current_bind_expr (void)
{
  return VEC_last (gimple, gimplify_ctxp->bind_expr_stack);
}

/* Returns true iff there is a COND_EXPR between us and the innermost
   CLEANUP_POINT_EXPR.  This info is used by gimple_push_cleanup.  */

/* FIXME tuples.  */
#if 0
static bool
gimple_conditional_context (void)
{
  return gimplify_ctxp->conditions > 0;
}
#endif

/* Note that we've entered a COND_EXPR.  */

static void
gimple_push_condition (void)
{
#ifdef ENABLE_GIMPLE_CHECKING
  if (gimplify_ctxp->conditions == 0)
    gcc_assert (gimple_seq_empty_p (&gimplify_ctxp->conditional_cleanups));
#endif
  ++(gimplify_ctxp->conditions);
}

/* Note that we've left a COND_EXPR.  If we're back at unconditional scope
   now, add any conditional cleanups we've seen to the prequeue.  */

static void
gimple_pop_condition (gimple_seq pre_p)
{
  int conds = --(gimplify_ctxp->conditions);

  gcc_assert (conds >= 0);
  if (conds == 0)
    {
      gimple_seq_append (pre_p, &gimplify_ctxp->conditional_cleanups);
      gimple_seq_init (&gimplify_ctxp->conditional_cleanups);
    }
}

/* A stable comparison routine for use with splay trees and DECLs.  */

static int
splay_tree_compare_decl_uid (splay_tree_key xa, splay_tree_key xb)
{
  tree a = (tree) xa;
  tree b = (tree) xb;

  return DECL_UID (a) - DECL_UID (b);
}

/* Create a new omp construct that deals with variable remapping.  */

static struct gimplify_omp_ctx *
new_omp_context (bool is_parallel, bool is_combined_parallel)
{
  struct gimplify_omp_ctx *c;

  c = XCNEW (struct gimplify_omp_ctx);
  c->outer_context = gimplify_omp_ctxp;
  c->variables = splay_tree_new (splay_tree_compare_decl_uid, 0, 0);
  c->privatized_types = pointer_set_create ();
  c->location = input_location;
  c->is_parallel = is_parallel;
  c->is_combined_parallel = is_combined_parallel;
  c->default_kind = OMP_CLAUSE_DEFAULT_SHARED;

  return c;
}

/* Destroy an omp construct that deals with variable remapping.  */

static void
delete_omp_context (struct gimplify_omp_ctx *c)
{
  splay_tree_delete (c->variables);
  pointer_set_destroy (c->privatized_types);
  XDELETE (c);
}

static void omp_add_variable (struct gimplify_omp_ctx *, tree, unsigned int);
static bool omp_notice_variable (struct gimplify_omp_ctx *, tree, bool);

/* A subroutine of append_to_statement_list{,_force}.  T is not NULL.  */

static void
append_to_statement_list_1 (tree t, tree *list_p)
{
  tree list = *list_p;
  tree_stmt_iterator i;

  if (!list)
    {
      if (t && TREE_CODE (t) == STATEMENT_LIST)
	{
	  *list_p = t;
	  return;
	}
      *list_p = list = alloc_stmt_list ();
    }

  i = tsi_last (list);
  tsi_link_after (&i, t, TSI_CONTINUE_LINKING);
}

/* Add T to the end of the list container pointed to by LIST_P.
   If T is an expression with no effects, it is ignored.  */

void
append_to_statement_list (tree t, tree *list_p)
{
  if (t && TREE_SIDE_EFFECTS (t))
    append_to_statement_list_1 (t, list_p);
}

/* Similar, but the statement is always added, regardless of side effects.  */

void
append_to_statement_list_force (tree t, tree *list_p)
{
  if (t != NULL_TREE)
    append_to_statement_list_1 (t, list_p);
}

/* Both gimplify the statement T and append it to SEQ.  */
/* This function behaves exactly as gimplify_stmt, but you don't have to pass
   T as a reference.  */

void
gimplify_and_add (tree t, gimple_seq seq)
{
  gimplify_stmt (&t, seq);
}

/* Strip off a legitimate source ending from the input string NAME of
   length LEN.  Rather than having to know the names used by all of
   our front ends, we strip off an ending of a period followed by
   up to five characters.  (Java uses ".class".)  */

static inline void
remove_suffix (char *name, int len)
{
  int i;

  for (i = 2;  i < 8 && len > i;  i++)
    {
      if (name[len - i] == '.')
	{
	  name[len - i] = '\0';
	  break;
	}
    }
}

/* Subroutine for find_single_pointer_decl.  */

static tree
find_single_pointer_decl_1 (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
			    void *data)
{
  tree *pdecl = (tree *) data;

  if (DECL_P (*tp) && POINTER_TYPE_P (TREE_TYPE (*tp)))
    {
      if (*pdecl)
	{
	  /* We already found a pointer decl; return anything other
	     than NULL_TREE to unwind from walk_tree signalling that
	     we have a duplicate.  */
	  return *tp;
	}
      *pdecl = *tp;
    }

  return NULL_TREE;
}

/* Find the single DECL of pointer type in the tree T and return it.
   If there are zero or more than one such DECLs, return NULL.  */

static tree
find_single_pointer_decl (tree t)
{
  tree decl = NULL_TREE;

  if (walk_tree (&t, find_single_pointer_decl_1, &decl, NULL))
    {
      /* find_single_pointer_decl_1 returns a nonzero value, causing
	 walk_tree to return a nonzero value, to indicate that it
	 found more than one pointer DECL.  */
      return NULL_TREE;
    }

  return decl;
}

/* Create a new temporary name with PREFIX.  Returns an identifier.  */

static GTY(()) unsigned int tmp_var_id_num;

tree
create_tmp_var_name (const char *prefix)
{
  char *tmp_name;

  if (prefix)
    {
      char *preftmp = ASTRDUP (prefix);

      remove_suffix (preftmp, strlen (preftmp));
      prefix = preftmp;
    }

  ASM_FORMAT_PRIVATE_NAME (tmp_name, prefix ? prefix : "T", tmp_var_id_num++);
  return get_identifier (tmp_name);
}


/* Create a new temporary variable declaration of type TYPE.
   Does NOT push it into the current binding.  */

tree
create_tmp_var_raw (tree type, const char *prefix)
{
  tree tmp_var;
  tree new_type;

  /* Make the type of the variable writable.  */
  new_type = build_type_variant (type, 0, 0);
  TYPE_ATTRIBUTES (new_type) = TYPE_ATTRIBUTES (type);

  tmp_var = build_decl (VAR_DECL, prefix ? create_tmp_var_name (prefix) : NULL,
			type);

  /* The variable was declared by the compiler.  */
  DECL_ARTIFICIAL (tmp_var) = 1;
  /* And we don't want debug info for it.  */
  DECL_IGNORED_P (tmp_var) = 1;

  /* Make the variable writable.  */
  TREE_READONLY (tmp_var) = 0;

  DECL_EXTERNAL (tmp_var) = 0;
  TREE_STATIC (tmp_var) = 0;
  TREE_USED (tmp_var) = 1;

  return tmp_var;
}

/* Create a new temporary variable declaration of type TYPE.  DOES push the
   variable into the current binding.  Further, assume that this is called
   only from gimplification or optimization, at which point the creation of
   certain types are bugs.  */

tree
create_tmp_var (tree type, const char *prefix)
{
  tree tmp_var;

  /* We don't allow types that are addressable (meaning we can't make copies),
     or incomplete.  We also used to reject every variable size objects here,
     but now support those for which a constant upper bound can be obtained.
     The processing for variable sizes is performed in gimple_add_tmp_var,
     point at which it really matters and possibly reached via paths not going
     through this function, e.g. after direct calls to create_tmp_var_raw.  */
  gcc_assert (!TREE_ADDRESSABLE (type) && COMPLETE_TYPE_P (type));

  tmp_var = create_tmp_var_raw (type, prefix);
  gimple_add_tmp_var (tmp_var);
  return tmp_var;
}

/* Create a temporary with a name derived from VAL.  Subroutine of
   lookup_tmp_var; nobody else should call this function.  */

static inline tree
create_tmp_from_val (tree val)
{
  return create_tmp_var (TYPE_MAIN_VARIANT (TREE_TYPE (val)), get_name (val));
}

/* Create a temporary to hold the value of VAL.  If IS_FORMAL, try to reuse
   an existing expression temporary.  */

static tree
lookup_tmp_var (tree val, bool is_formal)
{
  tree ret;

  /* If not optimizing, never really reuse a temporary.  local-alloc
     won't allocate any variable that is used in more than one basic
     block, which means it will go into memory, causing much extra
     work in reload and final and poorer code generation, outweighing
     the extra memory allocation here.  */
  if (!optimize || !is_formal || TREE_SIDE_EFFECTS (val))
    ret = create_tmp_from_val (val);
  else
    {
      elt_t elt, *elt_p;
      void **slot;

      elt.val = val;
      slot = htab_find_slot (gimplify_ctxp->temp_htab, (void *)&elt, INSERT);
      if (*slot == NULL)
	{
	  elt_p = XNEW (elt_t);
	  elt_p->val = val;
	  elt_p->temp = ret = create_tmp_from_val (val);
	  *slot = (void *) elt_p;
	}
      else
	{
	  elt_p = (elt_t *) *slot;
          ret = elt_p->temp;
	}
    }

  if (is_formal)
    DECL_GIMPLE_FORMAL_TEMP_P (ret) = 1;

  return ret;
}

/* Returns a formal temporary variable initialized with VAL.  PRE_P is as
   in gimplify_expr.  Only use this function if:

   1) The value of the unfactored expression represented by VAL will not
      change between the initialization and use of the temporary, and
   2) The temporary will not be otherwise modified.

   For instance, #1 means that this is inappropriate for SAVE_EXPR temps,
   and #2 means it is inappropriate for && temps.

   For other cases, use get_initialized_tmp_var instead.  */

static tree
internal_get_tmp_var (tree val, gimple_seq pre_p, gimple_seq post_p,
                      bool is_formal)
{
  tree t, mod;

  gimplify_expr (&val, pre_p, post_p, is_gimple_formal_tmp_rhs, fb_rvalue);

  t = lookup_tmp_var (val, is_formal);

  if (is_formal)
    {
      tree u = find_single_pointer_decl (val);

      if (u && TREE_CODE (u) == VAR_DECL && DECL_BASED_ON_RESTRICT_P (u))
	u = DECL_GET_RESTRICT_BASE (u);
      if (u && TYPE_RESTRICT (TREE_TYPE (u)))
	{
	  if (DECL_BASED_ON_RESTRICT_P (t))
	    gcc_assert (u == DECL_GET_RESTRICT_BASE (t));
	  else
	    {
	      DECL_BASED_ON_RESTRICT_P (t) = 1;
	      SET_DECL_RESTRICT_BASE (t, u);
	    }
	}
    }

  if (TREE_CODE (TREE_TYPE (t)) == COMPLEX_TYPE
      || TREE_CODE (TREE_TYPE (t)) == VECTOR_TYPE)
    DECL_GIMPLE_REG_P (t) = 1;

  mod = build2 (INIT_EXPR, TREE_TYPE (t), t, unshare_expr (val));

  if (EXPR_HAS_LOCATION (val))
    SET_EXPR_LOCUS (mod, EXPR_LOCUS (val));
  else
    SET_EXPR_LOCATION (mod, input_location);

  /* gimplify_modify_expr might want to reduce this further.  */
  gimplify_and_add (mod, pre_p);

  /* If we're gimplifying into ssa, gimplify_modify_expr will have
     given our temporary an ssa name.  Find and return it.  */
  if (gimplify_ctxp->into_ssa)
    t = TREE_OPERAND (mod, 0);

  return t;
}

/* Returns a formal temporary variable initialized with VAL.  PRE_P
   points to a sequence where side-effects needed to compute VAL should be
   stored.  */

tree
get_formal_tmp_var (tree val, gimple_seq pre_p)
{
  return internal_get_tmp_var (val, pre_p, NULL, true);
}

/* Returns a temporary variable initialized with VAL.  PRE_P and POST_P
   are as in gimplify_expr.  */

tree
get_initialized_tmp_var (tree val, gimple_seq pre_p, gimple_seq post_p)
{
  return internal_get_tmp_var (val, pre_p, post_p, false);
}

/* Create a temporary variable to be used as the LHS of the given GIMPLE
   statement STMT. STMT must be GIMPLE_ASSIGN or GIMPLE_CALL.  If IS_FORMAL is
   true, try to return a temporary from the formal temporaries table
   (which will attempt to return the same temporary created for a
   previous statement identical to STMT.  */

static tree
get_tmp_var_for (gimple stmt)
{
  tree lhs;
  enum gimple_code code = gimple_code (stmt);

  /* FIXME tuples, add support for formal temporaries (worth it?)  */
  if (code == GIMPLE_ASSIGN)
    lhs = create_tmp_from_val (gimple_assign_operand (stmt, 1));
  else if (code == GIMPLE_CALL)
    lhs = create_tmp_var (gimple_call_return_type (stmt),
			   get_name (gimple_call_fn (stmt)));
  else
    gcc_unreachable ();

  return lhs;
}


/* Declares all the variables in VARS in SCOPE.  If DEBUG_INFO is
   true, generate debug info for them; otherwise don't.  */

void
declare_vars (tree vars, gimple scope, bool debug_info)
{
  tree last = vars;
  if (last)
    {
      tree temps, block;

      gcc_assert (gimple_code (scope) == GIMPLE_BIND);

      temps = nreverse (last);

      block = gimple_block (scope);
      gcc_assert (!block || TREE_CODE (block) == BLOCK);
      if (!block || !debug_info)
	{
	  TREE_CHAIN (last) = gimple_bind_vars (scope);
	  gimple_bind_set_vars (scope, temps);
	}
      else
	{
	  /* We need to attach the nodes both to the BIND_EXPR and to its
	     associated BLOCK for debugging purposes.  The key point here
	     is that the BLOCK_VARS of the BIND_EXPR_BLOCK of a BIND_EXPR
	     is a subchain of the BIND_EXPR_VARS of the BIND_EXPR.  */
	  if (BLOCK_VARS (block))
	    BLOCK_VARS (block) = chainon (BLOCK_VARS (block), temps);
	  else
	    {
	      gimple_bind_set_vars (scope,
	      			    chainon (gimple_bind_vars (scope), temps));
	      BLOCK_VARS (block) = temps;
	    }
	}
    }
}

/* For VAR a VAR_DECL of variable size, try to find a constant upper bound
   for the size and adjust DECL_SIZE/DECL_SIZE_UNIT accordingly.  Abort if
   no such upper bound can be obtained.  */

static void
force_constant_size (tree var)
{
  /* The only attempt we make is by querying the maximum size of objects
     of the variable's type.  */

  HOST_WIDE_INT max_size;

  gcc_assert (TREE_CODE (var) == VAR_DECL);

  max_size = max_int_size_in_bytes (TREE_TYPE (var));

  gcc_assert (max_size >= 0);

  DECL_SIZE_UNIT (var)
    = build_int_cst (TREE_TYPE (DECL_SIZE_UNIT (var)), max_size);
  DECL_SIZE (var)
    = build_int_cst (TREE_TYPE (DECL_SIZE (var)), max_size * BITS_PER_UNIT);
}

void
gimple_add_tmp_var (tree tmp)
{
  gcc_assert (!TREE_CHAIN (tmp) && !DECL_SEEN_IN_BIND_EXPR_P (tmp));

  /* Later processing assumes that the object size is constant, which might
     not be true at this point.  Force the use of a constant upper bound in
     this case.  */
  if (!host_integerp (DECL_SIZE_UNIT (tmp), 1))
    force_constant_size (tmp);

  DECL_CONTEXT (tmp) = current_function_decl;
  DECL_SEEN_IN_BIND_EXPR_P (tmp) = 1;

  if (gimplify_ctxp)
    {
      TREE_CHAIN (tmp) = gimplify_ctxp->temps;
      gimplify_ctxp->temps = tmp;

      /* Mark temporaries local within the nearest enclosing parallel.  */
      if (gimplify_omp_ctxp)
	{
	  struct gimplify_omp_ctx *ctx = gimplify_omp_ctxp;
	  while (ctx && !ctx->is_parallel)
	    ctx = ctx->outer_context;
	  if (ctx)
	    omp_add_variable (ctx, tmp, GOVD_LOCAL | GOVD_SEEN);
	}
    }
  else if (cfun)
    record_vars (tmp);
  else
    {
      gimple_seq body_seq;

      /* This case is for nested functions.  We need to expose the locals
	 they create.  */
      body_seq = gimple_body (current_function_decl);
      declare_vars (tmp, gimple_seq_first (body_seq), false);
    }
}

/* Determines whether to assign a locus to the statement GS.  */

static bool
should_carry_locus_p (gimple gs)
{
  /* Don't emit a line note for a label.  We particularly don't want to
     emit one for the break label, since it doesn't actually correspond
     to the beginning of the loop/switch.  */
  if (gimple_code (gs) == GIMPLE_LABEL)
    return false;

  return true;
}

static void
annotate_one_with_locus (gimple gs, location_t locus)
{
  /* All gimple statements have location.  */
  if (gimple_locus_empty_p (gs) && should_carry_locus_p (gs))
    set_gimple_locus (gs, locus);
}

void
annotate_all_with_locus (gimple_seq stmt_p, location_t locus)
{
  gimple_stmt_iterator *i;

  if (gimple_seq_empty_p (stmt_p))
    return;

  for (i = gsi_start (stmt_p); !gsi_end_p (i); gsi_next (i))
    {
      gimple gs = gsi_stmt (i);
      annotate_one_with_locus (gs, locus);
    }
}

/* Similar to copy_tree_r() but do not copy SAVE_EXPR or TARGET_EXPR nodes.
   These nodes model computations that should only be done once.  If we
   were to unshare something like SAVE_EXPR(i++), the gimplification
   process would create wrong code.  */

static tree
mostly_copy_tree_r (tree *tp, int *walk_subtrees, void *data)
{
  enum tree_code code = TREE_CODE (*tp);
  /* Don't unshare types, decls, constants and SAVE_EXPR nodes.  */
  if (TREE_CODE_CLASS (code) == tcc_type
      || TREE_CODE_CLASS (code) == tcc_declaration
      || TREE_CODE_CLASS (code) == tcc_constant
      || code == SAVE_EXPR || code == TARGET_EXPR
      /* We can't do anything sensible with a BLOCK used as an expression,
	 but we also can't just die when we see it because of non-expression
	 uses.  So just avert our eyes and cross our fingers.  Silly Java.  */
      || code == BLOCK)
    *walk_subtrees = 0;
  else
    {
      gcc_assert (code != BIND_EXPR);
      copy_tree_r (tp, walk_subtrees, data);
    }

  return NULL_TREE;
}

/* Callback for walk_tree to unshare most of the shared trees rooted at
   *TP.  If *TP has been visited already (i.e., TREE_VISITED (*TP) == 1),
   then *TP is deep copied by calling copy_tree_r.

   This unshares the same trees as copy_tree_r with the exception of
   SAVE_EXPR nodes.  These nodes model computations that should only be
   done once.  If we were to unshare something like SAVE_EXPR(i++), the
   gimplification process would create wrong code.  */

static tree
copy_if_shared_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		  void *data ATTRIBUTE_UNUSED)
{
  tree t = *tp;
  enum tree_code code = TREE_CODE (t);

  /* Skip types, decls, and constants.  But we do want to look at their
     types and the bounds of types.  Mark them as visited so we properly
     unmark their subtrees on the unmark pass.  If we've already seen them,
     don't look down further.  */
  if (TREE_CODE_CLASS (code) == tcc_type
      || TREE_CODE_CLASS (code) == tcc_declaration
      || TREE_CODE_CLASS (code) == tcc_constant)
    {
      if (TREE_VISITED (t))
	*walk_subtrees = 0;
      else
	TREE_VISITED (t) = 1;
    }

  /* If this node has been visited already, unshare it and don't look
     any deeper.  */
  else if (TREE_VISITED (t))
    {
      walk_tree (tp, mostly_copy_tree_r, NULL, NULL);
      *walk_subtrees = 0;
    }

  /* Otherwise, mark the tree as visited and keep looking.  */
  else
    TREE_VISITED (t) = 1;

  return NULL_TREE;
}

static tree
unmark_visited_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		  void *data ATTRIBUTE_UNUSED)
{
  if (TREE_VISITED (*tp))
    TREE_VISITED (*tp) = 0;
  else
    *walk_subtrees = 0;

  return NULL_TREE;
}

/* Unshare all the trees in BODY_P, a pointer into the body of FNDECL, and the
   bodies of any nested functions if we are unsharing the entire body of
   FNDECL.  */

static void
unshare_body (tree *body_p, tree fndecl)
{
  struct cgraph_node *cgn = cgraph_node (fndecl);

  walk_tree (body_p, copy_if_shared_r, NULL, NULL);
  if (body_p == &DECL_SAVED_TREE (fndecl))
    for (cgn = cgn->nested; cgn; cgn = cgn->next_nested)
      unshare_body (&DECL_SAVED_TREE (cgn->decl), cgn->decl);
}

/* Likewise, but mark all trees as not visited.  */

static void
unvisit_body (tree *body_p, tree fndecl)
{
  struct cgraph_node *cgn = cgraph_node (fndecl);

  walk_tree (body_p, unmark_visited_r, NULL, NULL);
  if (body_p == &DECL_SAVED_TREE (fndecl))
    for (cgn = cgn->nested; cgn; cgn = cgn->next_nested)
      unvisit_body (&DECL_SAVED_TREE (cgn->decl), cgn->decl);
}

/* Unconditionally make an unshared copy of EXPR.  This is used when using
   stored expressions which span multiple functions, such as BINFO_VTABLE,
   as the normal unsharing process can't tell that they're shared.  */

tree
unshare_expr (tree expr)
{
  walk_tree (&expr, mostly_copy_tree_r, NULL, NULL);
  return expr;
}

/* A terser interface for building a representation of an exception
   specification.  */

tree
build_gimple_eh_filter_tree (tree body, tree allowed, tree failure)
{
  tree t;

  /* FIXME should the allowed types go in TREE_TYPE?  */
  t = build2 (EH_FILTER_EXPR, void_type_node, allowed, NULL_TREE);
  append_to_statement_list (failure, &EH_FILTER_FAILURE (t));

  t = build2 (TRY_CATCH_EXPR, void_type_node, NULL_TREE, t);
  append_to_statement_list (body, &TREE_OPERAND (t, 0));

  return t;
}


/* WRAPPER is a code such as BIND_EXPR or CLEANUP_POINT_EXPR which can both
   contain statements and have a value.  Assign its value to a temporary
   and give it void_type_node.  Returns the temporary, or NULL_TREE if
   WRAPPER was already void.  */

tree
voidify_wrapper_expr (tree wrapper, tree temp)
{
  tree type = TREE_TYPE (wrapper);
  if (type && !VOID_TYPE_P (type))
    {
      tree *p;

      /* Set p to point to the body of the wrapper.  Loop until we find
	 something that isn't a wrapper.  */
      for (p = &wrapper; p && *p; )
	{
	  switch (TREE_CODE (*p))
	    {
	    case BIND_EXPR:
	      TREE_SIDE_EFFECTS (*p) = 1;
	      TREE_TYPE (*p) = void_type_node;
	      /* For a BIND_EXPR, the body is operand 1.  */
	      p = &BIND_EXPR_BODY (*p);
	      break;

	    case CLEANUP_POINT_EXPR:
	    case TRY_FINALLY_EXPR:
	    case TRY_CATCH_EXPR:
	      TREE_SIDE_EFFECTS (*p) = 1;
	      TREE_TYPE (*p) = void_type_node;
	      p = &TREE_OPERAND (*p, 0);
	      break;

	    case STATEMENT_LIST:
	      {
		tree_stmt_iterator i = tsi_last (*p);
		TREE_SIDE_EFFECTS (*p) = 1;
		TREE_TYPE (*p) = void_type_node;
		p = tsi_end_p (i) ? NULL : tsi_stmt_ptr (i);
	      }
	      break;

	    case COMPOUND_EXPR:
	      /* Advance to the last statement.  Set all container types to void.  */
	      for (; TREE_CODE (*p) == COMPOUND_EXPR; p = &TREE_OPERAND (*p, 1))
		{
		  TREE_SIDE_EFFECTS (*p) = 1;
		  TREE_TYPE (*p) = void_type_node;
		}
	      break;

	    default:
	      goto out;
	    }
	}

    out:
      if (p == NULL || IS_EMPTY_STMT (*p))
	temp = NULL_TREE;
      else if (temp)
	{
	  /* The wrapper is on the RHS of an assignment that we're pushing
	     down.  */
	  gcc_assert (TREE_CODE (temp) == INIT_EXPR
		      || TREE_CODE (temp) == GIMPLE_MODIFY_STMT
		      || TREE_CODE (temp) == MODIFY_EXPR);
	  GENERIC_TREE_OPERAND (temp, 1) = *p;
	  *p = temp;
	}
      else
	{
	  temp = create_tmp_var (type, "retval");
	  *p = build2 (INIT_EXPR, type, temp, *p);
	}

      return temp;
    }

  return NULL_TREE;
}

/* Prepare calls to builtins to SAVE and RESTORE the stack as well as
   a temporary through which they communicate.  */

static void
build_stack_save_restore (gimple *save, gimple *restore)
{
  tree tmp_var;

  *save = build_gimple_call (implicit_built_in_decls[BUILT_IN_STACK_SAVE], 0);
  tmp_var = create_tmp_var (ptr_type_node, "saved_stack");
  gimple_call_set_lhs (*save, tmp_var);

  *restore = build_gimple_call (implicit_built_in_decls[BUILT_IN_STACK_RESTORE],
			    1, tmp_var);
}

/* Gimplify a BIND_EXPR.  Just voidify and recurse.  */

static enum gimplify_status
gimplify_bind_expr (tree *expr_p, gimple_seq pre_p)
{
  tree bind_expr = *expr_p;
  bool old_save_stack = gimplify_ctxp->save_stack;
  tree t;
  gimple gimple_bind;
  struct gimple_sequence empty_seq;

  tree temp = voidify_wrapper_expr (bind_expr, NULL);

  /* Mark variables seen in this bind expr.  */
  for (t = BIND_EXPR_VARS (bind_expr); t ; t = TREE_CHAIN (t))
    {
      if (TREE_CODE (t) == VAR_DECL)
	{
	  struct gimplify_omp_ctx *ctx = gimplify_omp_ctxp;

	  /* Mark variable as local.  */
	  if (ctx && !is_global_var (t)
	      && (! DECL_SEEN_IN_BIND_EXPR_P (t)
		  || splay_tree_lookup (ctx->variables,
					(splay_tree_key) t) == NULL))
	    omp_add_variable (gimplify_omp_ctxp, t, GOVD_LOCAL | GOVD_SEEN);

	  DECL_SEEN_IN_BIND_EXPR_P (t) = 1;
	}

      /* Preliminarily mark non-addressed complex variables as eligible
	 for promotion to gimple registers.  We'll transform their uses
	 as we find them.  */
      if ((TREE_CODE (TREE_TYPE (t)) == COMPLEX_TYPE
	   || TREE_CODE (TREE_TYPE (t)) == VECTOR_TYPE)
	  && !TREE_THIS_VOLATILE (t)
	  && (TREE_CODE (t) == VAR_DECL && !DECL_HARD_REGISTER (t))
	  && !needs_to_live_in_memory (t))
	DECL_GIMPLE_REG_P (t) = 1;
    }

  gimple_bind = build_gimple_bind (BIND_EXPR_VARS (bind_expr), NULL);
  gimple_push_bind_expr (gimple_bind);

  gimplify_ctxp->save_stack = false;

  /* Gimplify the body into the GIMPLE_BIND tuple's body.  */
  gimplify_stmt (&BIND_EXPR_BODY (bind_expr), gimple_bind_body (gimple_bind));

  if (gimplify_ctxp->save_stack)
    {
      gimple stack_save, stack_restore, gs;

      /* Save stack on entry and restore it on exit.  Add a try_finally
	 block to achieve this.  Note that mudflap depends on the
	 format of the emitted code: see mx_register_decls().  */
      build_stack_save_restore (&stack_save, &stack_restore);

      gs = build_gimple_try (gimple_bind_body (gimple_bind), NULL,
	  		     GIMPLE_TRY_FINALLY);
      gimple_seq_add (gimple_try_cleanup (gs), stack_restore);

      gimple_seq_init (&empty_seq);
      gimple_bind_set_body (gimple_bind, &empty_seq);
      gimple_seq_add (gimple_bind_body (gimple_bind), stack_save);
      gimple_seq_add (gimple_bind_body (gimple_bind), gs);
    }

  gimplify_ctxp->save_stack = old_save_stack;
  gimple_pop_bind_expr ();

  gimple_seq_add (pre_p, gimple_bind);

  if (temp)
    {
      *expr_p = temp;
      return GS_OK;
    }
  else
    return GS_ALL_DONE;
}

/* Gimplify a RETURN_EXPR.  If the expression to be returned is not a
   GIMPLE value, it is assigned to a new temporary and the statement is
   re-written to return the temporary.

   PRE_P points to the sequence where side effects that must happen before
   STMT should be stored.  */

static enum gimplify_status
gimplify_return_expr (tree stmt, gimple_seq pre_p)
{
  tree ret_expr = TREE_OPERAND (stmt, 0);
  tree result_decl, result;

  if (!ret_expr
      || TREE_CODE (ret_expr) == RESULT_DECL
      || ret_expr == error_mark_node)
    {
      bool use_return_decl_p = ret_expr && TREE_CODE (ret_expr) == RESULT_DECL;
      gimple ret = build_gimple_return (use_return_decl_p, ret_expr);
      gimple_seq_add (pre_p, ret);
      return GS_ALL_DONE;
    }

  if (VOID_TYPE_P (TREE_TYPE (TREE_TYPE (current_function_decl))))
    result_decl = NULL_TREE;
  else
    {
      result_decl = GENERIC_TREE_OPERAND (ret_expr, 0);

      /* See through a return by reference.  */
      if (TREE_CODE (result_decl) == INDIRECT_REF)
	result_decl = TREE_OPERAND (result_decl, 0);

      gcc_assert ((TREE_CODE (ret_expr) == MODIFY_EXPR
	    	   || TREE_CODE (ret_expr) == GIMPLE_MODIFY_STMT
		   || TREE_CODE (ret_expr) == INIT_EXPR)
		  && TREE_CODE (result_decl) == RESULT_DECL);
    }

  /* If aggregate_value_p is true, then we can return the bare RESULT_DECL.
     Recall that aggregate_value_p is FALSE for any aggregate type that is
     returned in registers.  If we're returning values in registers, then
     we don't want to extend the lifetime of the RESULT_DECL, particularly
     across another call.  In addition, for those aggregates for which
     hard_function_value generates a PARALLEL, we'll die during normal
     expansion of structure assignments; there's special code in expand_return
     to handle this case that does not exist in expand_expr.  */
  if (!result_decl
      || aggregate_value_p (result_decl, TREE_TYPE (current_function_decl)))
    result = result_decl;
  else if (gimplify_ctxp->return_temp)
    result = gimplify_ctxp->return_temp;
  else
    {
      result = create_tmp_var (TREE_TYPE (result_decl), NULL);
      if (TREE_CODE (TREE_TYPE (result)) == COMPLEX_TYPE
          || TREE_CODE (TREE_TYPE (result)) == VECTOR_TYPE)
        DECL_GIMPLE_REG_P (result) = 1;

      /* ??? With complex control flow (usually involving abnormal edges),
	 we can wind up warning about an uninitialized value for this.  Due
	 to how this variable is constructed and initialized, this is never
	 true.  Give up and never warn.  */
      TREE_NO_WARNING (result) = 1;

      gimplify_ctxp->return_temp = result;
    }

  /* Smash the lhs of the GIMPLE_MODIFY_STMT to the temporary we plan to use.
     Then gimplify the whole thing.  */
  if (result != result_decl)
    GENERIC_TREE_OPERAND (ret_expr, 0) = result;

  gimplify_and_add (TREE_OPERAND (stmt, 0), pre_p);

  gimple_seq_add (pre_p, build_gimple_return (result == result_decl, result));

  return GS_ALL_DONE;
}

/* Gimplifies a DECL_EXPR node *STMT_P by making any necessary allocation
   and initialization explicit.  */

static enum gimplify_status
gimplify_decl_expr (tree *stmt_p, gimple_seq seq_p)
{
  tree stmt = *stmt_p;
  tree decl = DECL_EXPR_DECL (stmt);

  *stmt_p = NULL_TREE;

  if (TREE_TYPE (decl) == error_mark_node)
    return GS_ERROR;

  if ((TREE_CODE (decl) == TYPE_DECL
       || TREE_CODE (decl) == VAR_DECL)
      && !TYPE_SIZES_GIMPLIFIED (TREE_TYPE (decl)))
    gimplify_type_sizes (TREE_TYPE (decl), seq_p);

  if (TREE_CODE (decl) == VAR_DECL && !DECL_EXTERNAL (decl))
    {
      tree init = DECL_INITIAL (decl);

      if (TREE_CODE (DECL_SIZE (decl)) != INTEGER_CST)
	{
	  /* This is a variable-sized decl.  Simplify its size and mark it
	     for deferred expansion.  Note that mudflap depends on the format
	     of the emitted code: see mx_register_decls().  */
	  tree t, addr, ptr_type;

	  gimplify_one_sizepos (&DECL_SIZE (decl), seq_p);
	  gimplify_one_sizepos (&DECL_SIZE_UNIT (decl), seq_p);

	  /* All occurrences of this decl in final gimplified code will be
	     replaced by indirection.  Setting DECL_VALUE_EXPR does two
	     things: First, it lets the rest of the gimplifier know what
	     replacement to use.  Second, it lets the debug info know
	     where to find the value.  */
	  ptr_type = build_pointer_type (TREE_TYPE (decl));
	  addr = create_tmp_var (ptr_type, get_name (decl));
	  DECL_IGNORED_P (addr) = 0;
	  t = build_fold_indirect_ref (addr);
	  SET_DECL_VALUE_EXPR (decl, t);
	  DECL_HAS_VALUE_EXPR_P (decl) = 1;

	  t = built_in_decls[BUILT_IN_ALLOCA];
	  t = build_call_expr (t, 1, DECL_SIZE_UNIT (decl));
	  t = fold_convert (ptr_type, t);
	  t = build_gimple_modify_stmt (addr, t);

	  gimplify_and_add (t, seq_p);

	  /* Indicate that we need to restore the stack level when the
	     enclosing BIND_EXPR is exited.  */
	  gimplify_ctxp->save_stack = true;
	}

      if (init && init != error_mark_node)
	{
	  if (!TREE_STATIC (decl))
	    {
	      DECL_INITIAL (decl) = NULL_TREE;
	      init = build2 (INIT_EXPR, void_type_node, decl, init);
	      gimplify_and_add (init, seq_p);
	    }
	  else
	    /* We must still examine initializers for static variables
	       as they may contain a label address.  */
	    walk_tree (&init, force_labels_r, NULL, NULL);
	}

      /* Some front ends do not explicitly declare all anonymous
	 artificial variables.  We compensate here by declaring the
	 variables, though it would be better if the front ends would
	 explicitly declare them.  */
      if (!DECL_SEEN_IN_BIND_EXPR_P (decl)
	  && DECL_ARTIFICIAL (decl) && DECL_NAME (decl) == NULL_TREE)
	gimple_add_tmp_var (decl);
    }

  return GS_ALL_DONE;
}

/* Gimplify a LOOP_EXPR.  Normally this just involves gimplifying the body
   and replacing the LOOP_EXPR with goto, but if the loop contains an
   EXIT_EXPR, we need to append a label for it to jump to.  */

static enum gimplify_status
gimplify_loop_expr (tree *expr_p, gimple_seq pre_p)
{
  tree saved_label = gimplify_ctxp->exit_label;
  tree start_label = create_artificial_label ();

  gimple_seq_add (pre_p, build_gimple_label (start_label));

  gimplify_ctxp->exit_label = NULL_TREE;

  gimplify_and_add (LOOP_EXPR_BODY (*expr_p), pre_p);

  gimple_seq_add (pre_p, build_gimple_goto (start_label));

  if (gimplify_ctxp->exit_label)
    gimple_seq_add (pre_p, build_gimple_label (gimplify_ctxp->exit_label));

  gimplify_ctxp->exit_label = saved_label;

  *expr_p = NULL;
  return GS_ALL_DONE;
}

/* Gimplifies a statement list onto a sequence.  These may be created either
   by an enlightened front-end, or by shortcut_cond_expr.  */

static enum gimplify_status
gimplify_statement_list (tree *expr_p, gimple_seq pre_p)
{
  tree temp = voidify_wrapper_expr (*expr_p, NULL);

  tree_stmt_iterator i = tsi_start (*expr_p);

  while (!tsi_end_p (i))
    {
      gimplify_stmt (tsi_stmt_ptr (i), pre_p);
      tsi_delink (&i);
    }

  if (temp)
    {
      /* FIXME tuples.  Not clear how this should be handled.  */
      gcc_unreachable ();
#if 0
      append_to_statement_list (*expr_p, pre_p);
      *expr_p = temp;
      return GS_OK;
#endif
    }

  return GS_ALL_DONE;
}

/* Compare two case labels.  Because the front end should already have
   made sure that case ranges do not overlap, it is enough to only compare
   the CASE_LOW values of each case label.  */

static int
compare_case_labels (const void *p1, const void *p2)
{
  const_tree const case1 = *(const_tree const*)p1;
  const_tree const case2 = *(const_tree const*)p2;

  return tree_int_cst_compare (CASE_LOW (case1), CASE_LOW (case2));
}

/* Sort the case labels in LABEL_VEC in place in ascending order.  */

void
sort_case_labels (VEC(tree,heap)* label_vec)
{
  size_t len = VEC_length (tree, label_vec);
  tree default_case = VEC_index (tree, label_vec, len - 1);

  if (CASE_LOW (default_case))
    {
      size_t i;

      /* The last label in the vector should be the default case
         but it is not.  */
      for (i = 0; i < len; ++i)
        {
          tree t = VEC_index (tree, label_vec, i);
          if (!CASE_LOW (t))
            {
              default_case = t;
              tree index = VEC_index (tree, label_vec, len - 1);
              VEC_replace (tree, label_vec, i, index);
              VEC_replace (tree, label_vec, len - 1, default_case);
              break;
            }
        }
    }

  qsort (VEC_address (tree, label_vec), len - 1, sizeof (tree),
         compare_case_labels);
}


/* Gimplify a SWITCH_EXPR, and collect a TREE_VEC of the labels it can
   branch to.  */

static enum gimplify_status
gimplify_switch_expr (tree *expr_p, gimple_seq pre_p)
{
  tree switch_expr;
  switch_expr = *expr_p;
  struct gimple_sequence switch_body_seq;
  
  gimple_seq_init (&switch_body_seq);
  gimplify_expr (&SWITCH_COND (switch_expr), pre_p, NULL, is_gimple_val,
                 fb_rvalue);
  
  if (SWITCH_BODY (switch_expr))
    {
      VEC (tree,heap) *labels;
      VEC (tree,heap) *saved_labels;
      tree default_case = NULL_TREE;
      size_t i, len;
      gimple gimple_switch;
      
      /* If someone can be bothered to fill in the labels, they can
	 be bothered to null out the body too.  */
      gcc_assert (!SWITCH_LABELS (switch_expr));

      /* save old labels, get new ones from body, then restore the old 
         labels.  Save all the things from the switch body to append after.  */
      saved_labels = gimplify_ctxp->case_labels;
      gimplify_ctxp->case_labels = VEC_alloc (tree, heap, 8);

      gimplify_stmt (&SWITCH_BODY (switch_expr), &switch_body_seq);
      labels = gimplify_ctxp->case_labels;
      gimplify_ctxp->case_labels = saved_labels;
 
      i = 0;
      while (i < VEC_length (tree, labels))
	{
	  tree elt = VEC_index (tree, labels, i);
	  tree low = CASE_LOW (elt);
	  bool remove_element = FALSE;

	  if (low)
	    {
	      /* Discard empty ranges.  */
	      tree high = CASE_HIGH (elt);
	      if (high && INT_CST_LT (high, low))
	        remove_element = TRUE;
	    }
	  else
	    {
	      /* The default case must be the last label in the list.  */
	      gcc_assert (!default_case);
	      default_case = elt;
	      remove_element = TRUE;
	    }

	  if (remove_element)
	    VEC_ordered_remove (tree, labels, i);
	  else
	    i++;
	}
      len = i;

      if (! default_case)
	{
	  /* If the switch has no default label, add one, so that we jump
	     around the switch body.  */
	  tree def_lab = build3 (CASE_LABEL_EXPR, void_type_node, NULL_TREE,
	                         NULL_TREE, create_artificial_label ());
	  gimple new_default = build_gimple_label (def_lab);
	  gimple_seq_add (&switch_body_seq, new_default);
	  default_case = gimple_label_label (new_default);
	}

      if (!VEC_empty (tree, labels))
	sort_case_labels (labels);

      gimple_switch = build_gimple_switch_vec (SWITCH_COND (switch_expr), 
                                               default_case, labels);
      gimple_seq_add (pre_p, gimple_switch);
      gimple_seq_append (pre_p, &switch_body_seq);
      VEC_free(tree, heap, labels);
    }
  else
    gcc_assert (SWITCH_LABELS (switch_expr));

  return GS_ALL_DONE;
}


static enum gimplify_status
gimplify_case_label_expr (tree *expr_p, gimple_seq pre_p)
{
  struct gimplify_ctx *ctxp;

  /* Invalid OpenMP programs can play Duff's Device type games with
     #pragma omp parallel.  At least in the C front end, we don't
     detect such invalid branches until after gimplification.  */
  for (ctxp = gimplify_ctxp; ; ctxp = ctxp->prev_context)
    if (ctxp->case_labels)
      break;

  gimple gimple_label = build_gimple_label (*expr_p);
  VEC_safe_push (tree, heap, ctxp->case_labels,
                 gimple_label_label (gimple_label));
  gimple_seq_add (pre_p, gimple_label);

  return GS_ALL_DONE;
}

/* Build a GOTO to the LABEL_DECL pointed to by LABEL_P, building it first
   if necessary.  */

tree
build_and_jump (tree *label_p)
{
  if (label_p == NULL)
    /* If there's nowhere to jump, just fall through.  */
    return NULL_TREE;

  if (*label_p == NULL_TREE)
    {
      tree label = create_artificial_label ();
      *label_p = label;
    }

  return build1 (GOTO_EXPR, void_type_node, *label_p);
}

/* Gimplify an EXIT_EXPR by converting to a GOTO_EXPR inside a COND_EXPR.
   This also involves building a label to jump to and communicating it to
   gimplify_loop_expr through gimplify_ctxp->exit_label.  */

static enum gimplify_status
gimplify_exit_expr (tree *expr_p)
{
  tree cond = TREE_OPERAND (*expr_p, 0);
  tree expr;

  expr = build_and_jump (&gimplify_ctxp->exit_label);
  expr = build3 (COND_EXPR, void_type_node, cond, expr, NULL_TREE);
  *expr_p = expr;

  return GS_OK;
}

/* A helper function to be called via walk_tree.  Mark all labels under *TP
   as being forced.  To be called for DECL_INITIAL of static variables.  */

tree
force_labels_r (tree *tp, int *walk_subtrees, void *data ATTRIBUTE_UNUSED)
{
  if (TYPE_P (*tp))
    *walk_subtrees = 0;
  if (TREE_CODE (*tp) == LABEL_DECL)
    FORCED_LABEL (*tp) = 1;

  return NULL_TREE;
}

/* *EXPR_P is a COMPONENT_REF being used as an rvalue.  If its type is
   different from its canonical type, wrap the whole thing inside a
   NOP_EXPR and force the type of the COMPONENT_REF to be the canonical
   type.

   The canonical type of a COMPONENT_REF is the type of the field being
   referenced--unless the field is a bit-field which can be read directly
   in a smaller mode, in which case the canonical type is the
   sign-appropriate type corresponding to that mode.  */

static void
canonicalize_component_ref (tree *expr_p)
{
  tree expr = *expr_p;
  tree type;

  gcc_assert (TREE_CODE (expr) == COMPONENT_REF);

  if (INTEGRAL_TYPE_P (TREE_TYPE (expr)))
    type = TREE_TYPE (get_unwidened (expr, NULL_TREE));
  else
    type = TREE_TYPE (TREE_OPERAND (expr, 1));

  if (TREE_TYPE (expr) != type)
    {
      tree old_type = TREE_TYPE (expr);

      /* Set the type of the COMPONENT_REF to the underlying type.  */
      TREE_TYPE (expr) = type;

      /* And wrap the whole thing inside a NOP_EXPR.  */
      expr = build1 (NOP_EXPR, old_type, expr);

      *expr_p = expr;
    }
}

/* If a NOP conversion is changing a pointer to array of foo to a pointer
   to foo, embed that change in the ADDR_EXPR by converting
      T array[U];
      (T *)&array
   ==>
      &array[L]
   where L is the lower bound.  For simplicity, only do this for constant
   lower bound.  */

static void
canonicalize_addr_expr (tree *expr_p)
{
  tree expr = *expr_p;
  tree ctype = TREE_TYPE (expr);
  tree addr_expr = TREE_OPERAND (expr, 0);
  tree atype = TREE_TYPE (addr_expr);
  tree dctype, datype, ddatype, otype, obj_expr;

  /* Both cast and addr_expr types should be pointers.  */
  if (!POINTER_TYPE_P (ctype) || !POINTER_TYPE_P (atype))
    return;

  /* The addr_expr type should be a pointer to an array.  */
  datype = TREE_TYPE (atype);
  if (TREE_CODE (datype) != ARRAY_TYPE)
    return;

  /* Both cast and addr_expr types should address the same object type.  */
  dctype = TREE_TYPE (ctype);
  ddatype = TREE_TYPE (datype);
  if (!useless_type_conversion_p (dctype, ddatype))
    return;

  /* The addr_expr and the object type should match.  */
  obj_expr = TREE_OPERAND (addr_expr, 0);
  otype = TREE_TYPE (obj_expr);
  if (!useless_type_conversion_p (datype, otype))
    return;

  /* The lower bound and element sizes must be constant.  */
  if (!TYPE_SIZE_UNIT (dctype)
      || TREE_CODE (TYPE_SIZE_UNIT (dctype)) != INTEGER_CST
      || !TYPE_DOMAIN (datype) || !TYPE_MIN_VALUE (TYPE_DOMAIN (datype))
      || TREE_CODE (TYPE_MIN_VALUE (TYPE_DOMAIN (datype))) != INTEGER_CST)
    return;

  /* All checks succeeded.  Build a new node to merge the cast.  */
  *expr_p = build4 (ARRAY_REF, dctype, obj_expr,
		    TYPE_MIN_VALUE (TYPE_DOMAIN (datype)),
		    NULL_TREE, NULL_TREE);
  *expr_p = build1 (ADDR_EXPR, ctype, *expr_p);
}

/* *EXPR_P is a NOP_EXPR or CONVERT_EXPR.  Remove it and/or other conversions
   underneath as appropriate.  */

static enum gimplify_status
gimplify_conversion (tree *expr_p)
{
  tree tem;
  gcc_assert (TREE_CODE (*expr_p) == NOP_EXPR
	      || TREE_CODE (*expr_p) == CONVERT_EXPR);
  
  /* Then strip away all but the outermost conversion.  */
  STRIP_SIGN_NOPS (TREE_OPERAND (*expr_p, 0));

  /* And remove the outermost conversion if it's useless.  */
  if (tree_ssa_useless_type_conversion (*expr_p))
    *expr_p = TREE_OPERAND (*expr_p, 0);

  /* Attempt to avoid NOP_EXPR by producing reference to a subtype.
     For example this fold (subclass *)&A into &A->subclass avoiding
     a need for statement.  */
  if (TREE_CODE (*expr_p) == NOP_EXPR
      && POINTER_TYPE_P (TREE_TYPE (*expr_p))
      && POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (*expr_p, 0)))
      && (tem = maybe_fold_offset_to_reference
		  (TREE_OPERAND (*expr_p, 0),
		   integer_zero_node, TREE_TYPE (TREE_TYPE (*expr_p)))))
    {
      tree ptr_type = build_pointer_type (TREE_TYPE (tem));
      if (useless_type_conversion_p (TREE_TYPE (*expr_p), ptr_type))
        *expr_p = build_fold_addr_expr_with_type (tem, ptr_type);
    }

  /* If we still have a conversion at the toplevel,
     then canonicalize some constructs.  */
  if (TREE_CODE (*expr_p) == NOP_EXPR || TREE_CODE (*expr_p) == CONVERT_EXPR)
    {
      tree sub = TREE_OPERAND (*expr_p, 0);

      /* If a NOP conversion is changing the type of a COMPONENT_REF
	 expression, then canonicalize its type now in order to expose more
	 redundant conversions.  */
      if (TREE_CODE (sub) == COMPONENT_REF)
	canonicalize_component_ref (&TREE_OPERAND (*expr_p, 0));

      /* If a NOP conversion is changing a pointer to array of foo
	 to a pointer to foo, embed that change in the ADDR_EXPR.  */
      else if (TREE_CODE (sub) == ADDR_EXPR)
	canonicalize_addr_expr (expr_p);
    }

  return GS_OK;
}

/* Gimplify a VAR_DECL or PARM_DECL.  Returns GS_OK if we expanded a 
   DECL_VALUE_EXPR, and it's worth re-examining things.  */

static enum gimplify_status
gimplify_var_or_parm_decl (tree *expr_p)
{
  tree decl = *expr_p;

  /* ??? If this is a local variable, and it has not been seen in any
     outer BIND_EXPR, then it's probably the result of a duplicate
     declaration, for which we've already issued an error.  It would
     be really nice if the front end wouldn't leak these at all.
     Currently the only known culprit is C++ destructors, as seen
     in g++.old-deja/g++.jason/binding.C.  */
  if (TREE_CODE (decl) == VAR_DECL
      && !DECL_SEEN_IN_BIND_EXPR_P (decl)
      && !TREE_STATIC (decl) && !DECL_EXTERNAL (decl)
      && decl_function_context (decl) == current_function_decl)
    {
      gcc_assert (errorcount || sorrycount);
      return GS_ERROR;
    }

  /* When within an OpenMP context, notice uses of variables.  */
  if (gimplify_omp_ctxp && omp_notice_variable (gimplify_omp_ctxp, decl, true))
    return GS_ALL_DONE;

  /* If the decl is an alias for another expression, substitute it now.  */
  if (DECL_HAS_VALUE_EXPR_P (decl))
    {
      *expr_p = unshare_expr (DECL_VALUE_EXPR (decl));
      return GS_OK;
    }

  return GS_ALL_DONE;
}


/* Gimplify the COMPONENT_REF, ARRAY_REF, REALPART_EXPR or IMAGPART_EXPR
   node *EXPR_P.

      compound_lval
	      : min_lval '[' val ']'
	      | min_lval '.' ID
	      | compound_lval '[' val ']'
	      | compound_lval '.' ID

   This is not part of the original SIMPLE definition, which separates
   array and member references, but it seems reasonable to handle them
   together.  Also, this way we don't run into problems with union
   aliasing; gcc requires that for accesses through a union to alias, the
   union reference must be explicit, which was not always the case when we
   were splitting up array and member refs.

   PRE_P points to the sequence where side effects that must happen before
     *EXPR_P should be stored.

   POST_P points to the sequence where side effects that must happen after
     *EXPR_P should be stored.  */

static enum gimplify_status
gimplify_compound_lval (tree *expr_p, gimple_seq pre_p,
    			gimple_seq post_p, fallback_t fallback)
{
  tree *p;
  VEC(tree,heap) *stack;
  enum gimplify_status ret = GS_OK, tret;
  int i;

  /* Create a stack of the subexpressions so later we can walk them in
     order from inner to outer.  */
  stack = VEC_alloc (tree, heap, 10);

  /* We can handle anything that get_inner_reference can deal with.  */
  for (p = expr_p; ; p = &TREE_OPERAND (*p, 0))
    {
    restart:
      /* Fold INDIRECT_REFs now to turn them into ARRAY_REFs.  */
      if (TREE_CODE (*p) == INDIRECT_REF)
	*p = fold_indirect_ref (*p);

      if (handled_component_p (*p))
	;
      /* Expand DECL_VALUE_EXPR now.  In some cases that may expose
	 additional COMPONENT_REFs.  */
      else if ((TREE_CODE (*p) == VAR_DECL || TREE_CODE (*p) == PARM_DECL)
	       && gimplify_var_or_parm_decl (p) == GS_OK)
	goto restart;
      else
	break;
	       
      VEC_safe_push (tree, heap, stack, *p);
    }

  gcc_assert (VEC_length (tree, stack));

  /* Now STACK is a stack of pointers to all the refs we've walked through
     and P points to the innermost expression.

     Java requires that we elaborated nodes in source order.  That
     means we must gimplify the inner expression followed by each of
     the indices, in order.  But we can't gimplify the inner
     expression until we deal with any variable bounds, sizes, or
     positions in order to deal with PLACEHOLDER_EXPRs.

     So we do this in three steps.  First we deal with the annotations
     for any variables in the components, then we gimplify the base,
     then we gimplify any indices, from left to right.  */
  for (i = VEC_length (tree, stack) - 1; i >= 0; i--)
    {
      tree t = VEC_index (tree, stack, i);

      if (TREE_CODE (t) == ARRAY_REF || TREE_CODE (t) == ARRAY_RANGE_REF)
	{
	  /* Gimplify the low bound and element type size and put them into
	     the ARRAY_REF.  If these values are set, they have already been
	     gimplified.  */
	  if (TREE_OPERAND (t, 2) == NULL_TREE)
	    {
	      tree low = unshare_expr (array_ref_low_bound (t));
	      if (!is_gimple_min_invariant (low))
		{
		  TREE_OPERAND (t, 2) = low;
		  tret = gimplify_expr (&TREE_OPERAND (t, 2), pre_p,
					post_p, is_gimple_formal_tmp_reg,
					fb_rvalue);
		  ret = MIN (ret, tret);
		}
	    }

	  if (!TREE_OPERAND (t, 3))
	    {
	      tree elmt_type = TREE_TYPE (TREE_TYPE (TREE_OPERAND (t, 0)));
	      tree elmt_size = unshare_expr (array_ref_element_size (t));
	      tree factor = size_int (TYPE_ALIGN_UNIT (elmt_type));

	      /* Divide the element size by the alignment of the element
		 type (above).  */
	      elmt_size = size_binop (EXACT_DIV_EXPR, elmt_size, factor);

	      if (!is_gimple_min_invariant (elmt_size))
		{
		  TREE_OPERAND (t, 3) = elmt_size;
		  tret = gimplify_expr (&TREE_OPERAND (t, 3), pre_p,
					post_p, is_gimple_formal_tmp_reg,
					fb_rvalue);
		  ret = MIN (ret, tret);
		}
	    }
	}
      else if (TREE_CODE (t) == COMPONENT_REF)
	{
	  /* Set the field offset into T and gimplify it.  */
	  if (!TREE_OPERAND (t, 2))
	    {
	      tree offset = unshare_expr (component_ref_field_offset (t));
	      tree field = TREE_OPERAND (t, 1);
	      tree factor
		= size_int (DECL_OFFSET_ALIGN (field) / BITS_PER_UNIT);

	      /* Divide the offset by its alignment.  */
	      offset = size_binop (EXACT_DIV_EXPR, offset, factor);

	      if (!is_gimple_min_invariant (offset))
		{
		  TREE_OPERAND (t, 2) = offset;
		  tret = gimplify_expr (&TREE_OPERAND (t, 2), pre_p,
					post_p, is_gimple_formal_tmp_reg,
					fb_rvalue);
		  ret = MIN (ret, tret);
		}
	    }
	}
    }

  /* Step 2 is to gimplify the base expression.  Make sure lvalue is set
     so as to match the min_lval predicate.  Failure to do so may result
     in the creation of large aggregate temporaries.  */
  tret = gimplify_expr (p, pre_p, post_p, is_gimple_min_lval,
			fallback | fb_lvalue);
  ret = MIN (ret, tret);

  /* And finally, the indices and operands to BIT_FIELD_REF.  During this
     loop we also remove any useless conversions.  */
  for (; VEC_length (tree, stack) > 0; )
    {
      tree t = VEC_pop (tree, stack);

      if (TREE_CODE (t) == ARRAY_REF || TREE_CODE (t) == ARRAY_RANGE_REF)
	{
	  /* Gimplify the dimension.
	     Temporary fix for gcc.c-torture/execute/20040313-1.c.
	     Gimplify non-constant array indices into a temporary
	     variable.
	     FIXME - The real fix is to gimplify post-modify
	     expressions into a minimal gimple lvalue.  However, that
	     exposes bugs in alias analysis.  The alias analyzer does
	     not handle &PTR->FIELD very well.  Will fix after the
	     branch is merged into mainline (dnovillo 2004-05-03).  */
	  if (!is_gimple_min_invariant (TREE_OPERAND (t, 1)))
	    {
	      tret = gimplify_expr (&TREE_OPERAND (t, 1), pre_p, post_p,
				    is_gimple_formal_tmp_reg, fb_rvalue);
	      ret = MIN (ret, tret);
	    }
	}
      else if (TREE_CODE (t) == BIT_FIELD_REF)
	{
	  tret = gimplify_expr (&TREE_OPERAND (t, 1), pre_p, post_p,
				is_gimple_val, fb_rvalue);
	  ret = MIN (ret, tret);
	  tret = gimplify_expr (&TREE_OPERAND (t, 2), pre_p, post_p,
				is_gimple_val, fb_rvalue);
	  ret = MIN (ret, tret);
	}

      STRIP_USELESS_TYPE_CONVERSION (TREE_OPERAND (t, 0));

      /* The innermost expression P may have originally had
	 TREE_SIDE_EFFECTS set which would have caused all the outer
	 expressions in *EXPR_P leading to P to also have had
	 TREE_SIDE_EFFECTS set.  */
      recalculate_side_effects (t);
    }


  /* FIXME tuples.  Is this needed at all?  We've already gimplified
     P.  */
#if 0
  tret = gimplify_expr (p, pre_p, post_p, is_gimple_min_lval, fallback);
  ret = MIN (ret, tret);
#endif

  /* If the outermost expression is a COMPONENT_REF, canonicalize its type.  */
  if ((fallback & fb_rvalue) && TREE_CODE (*expr_p) == COMPONENT_REF)
    {
      canonicalize_component_ref (expr_p);
      ret = MIN (ret, GS_OK);
    }

  VEC_free (tree, heap, stack);

  return ret;
}

/*  Gimplify the self modifying expression pointed to by EXPR_P
    (++, --, +=, -=).

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
	*EXPR_P should be stored.

    WANT_VALUE is nonzero iff we want to use the value of this expression
	in another expression.  */

static enum gimplify_status
gimplify_self_mod_expr (tree *expr_p, gimple_seq pre_p, gimple_seq post_p,
			bool want_value)
{
  enum tree_code code;
  tree lhs, lvalue, rhs, t1;
  struct gimple_sequence post;
  gimple_seq orig_post_p = post_p;
  bool postfix;
  enum tree_code arith_code;
  enum gimplify_status ret;

  code = TREE_CODE (*expr_p);

  gcc_assert (code == POSTINCREMENT_EXPR || code == POSTDECREMENT_EXPR
	      || code == PREINCREMENT_EXPR || code == PREDECREMENT_EXPR);

  /* Prefix or postfix?  */
  if (code == POSTINCREMENT_EXPR || code == POSTDECREMENT_EXPR)
    /* Faster to treat as prefix if result is not used.  */
    postfix = want_value;
  else
    postfix = false;

  /* For postfix, make sure the inner expression's post side effects
     are executed after side effects from this expression.  */
  gimple_seq_init (&post);
  if (postfix)
    post_p = &post;

  /* Add or subtract?  */
  if (code == PREINCREMENT_EXPR || code == POSTINCREMENT_EXPR)
    arith_code = PLUS_EXPR;
  else
    arith_code = MINUS_EXPR;

  /* Gimplify the LHS into a GIMPLE lvalue.  */
  lvalue = TREE_OPERAND (*expr_p, 0);
  ret = gimplify_expr (&lvalue, pre_p, post_p, is_gimple_lvalue, fb_lvalue);
  if (ret == GS_ERROR)
    return ret;

  /* Extract the operands to the arithmetic operation.  */
  lhs = lvalue;
  rhs = TREE_OPERAND (*expr_p, 1);

  /* For postfix operator, we evaluate the LHS to an rvalue and then use
     that as the result value and in the postqueue operation.  */
  if (postfix)
    {
      ret = gimplify_expr (&lhs, pre_p, post_p, is_gimple_val, fb_rvalue);
      if (ret == GS_ERROR)
	return ret;
    }

  /* For POINTERs increment, use POINTER_PLUS_EXPR.  */
  if (POINTER_TYPE_P (TREE_TYPE (lhs)))
    {
      rhs = fold_convert (sizetype, rhs);
      if (arith_code == MINUS_EXPR)
	rhs = fold_build1 (NEGATE_EXPR, TREE_TYPE (rhs), rhs);
      arith_code = POINTER_PLUS_EXPR;
    }

  t1 = build2 (arith_code, TREE_TYPE (*expr_p), lhs, rhs);
  t1 = build_gimple_modify_stmt (lvalue, t1);

  if (postfix)
    {
      gimplify_and_add (t1, orig_post_p);
      gimple_seq_append (orig_post_p, &post);
      *expr_p = lhs;
      return GS_ALL_DONE;
    }
  else
    {
      *expr_p = t1;
      return GS_OK;
    }
}


/* If *EXPR_P has a variable sized type, wrap it in a WITH_SIZE_EXPR.  */

static void
maybe_with_size_expr (tree *expr_p)
{
  tree expr = *expr_p;
  tree type = TREE_TYPE (expr);
  tree size;

  /* If we've already wrapped this or the type is error_mark_node, we can't do
     anything.  */
  if (TREE_CODE (expr) == WITH_SIZE_EXPR
      || type == error_mark_node)
    return;

  /* If the size isn't known or is a constant, we have nothing to do.  */
  size = TYPE_SIZE_UNIT (type);
  if (!size || TREE_CODE (size) == INTEGER_CST)
    return;

  /* Otherwise, make a WITH_SIZE_EXPR.  */
  size = unshare_expr (size);
  size = SUBSTITUTE_PLACEHOLDER_IN_EXPR (size, expr);
  *expr_p = build2 (WITH_SIZE_EXPR, type, expr, size);
}


/* Helper for gimplify_call_expr.  Gimplify a single argument *ARG_P
   Store any side-effects in PRE_P.  */

static enum gimplify_status
gimplify_arg (tree *arg_p, gimple_seq pre_p)
{
  bool (*test) (tree);
  fallback_t fb;

  /* In general, we allow lvalues for function arguments to avoid
     extra overhead of copying large aggregates out of even larger
     aggregates into temporaries only to copy the temporaries to
     the argument list.  Make optimizers happy by pulling out to
     temporaries those types that fit in registers.  */
  if (is_gimple_reg_type (TREE_TYPE (*arg_p)))
    test = is_gimple_val, fb = fb_rvalue;
  else
    test = is_gimple_lvalue, fb = fb_either;

  /* If this is a variable sized type, we must remember the size.  */
  maybe_with_size_expr (arg_p);

  /* There is a sequence point before a function call.  Side effects in
     the argument list must occur before the actual call. So, when
     gimplifying arguments, force gimplify_expr to use an internal
     post queue which is then appended to the end of PRE_P.  */
  return gimplify_expr (arg_p, pre_p, NULL, test, fb);
}


/* Gimplify the CALL_EXPR node *EXPR_P into the GIMPLE sequence PRE_P.
   WANT_VALUE is true if the result of the call is desired.  */

static enum gimplify_status
gimplify_call_expr (tree *expr_p, gimple_seq pre_p, bool want_value)
{
  tree fndecl, parms, p;
  enum gimplify_status ret;
  int i, nargs;
  VEC(tree, gc) *args = NULL;
  gimple call;

  gcc_assert (TREE_CODE (*expr_p) == CALL_EXPR);

  /* For reliable diagnostics during inlining, it is necessary that
     every call_expr be annotated with file and line.  */
  if (! EXPR_HAS_LOCATION (*expr_p))
    SET_EXPR_LOCATION (*expr_p, input_location);

  /* This may be a call to a builtin function.

     Builtin function calls may be transformed into different
     (and more efficient) builtin function calls under certain
     circumstances.  Unfortunately, gimplification can muck things
     up enough that the builtin expanders are not aware that certain
     transformations are still valid.

     So we attempt transformation/gimplification of the call before
     we gimplify the CALL_EXPR.  At this time we do not manage to
     transform all calls in the same manner as the expanders do, but
     we do transform most of them.  */
  fndecl = get_callee_fndecl (*expr_p);
  if (fndecl && DECL_BUILT_IN (fndecl))
    {
      tree new = fold_call_expr (*expr_p, !want_value);

      if (new && new != *expr_p)
	{
	  /* There was a transformation of this call which computes the
	     same value, but in a more efficient way.  Return and try
	     again.  */
	  *expr_p = new;
	  return GS_OK;
	}

      if (DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
	  && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_VA_START)
        {
	  if (call_expr_nargs (*expr_p) < 2)
	    {
	      error ("too few arguments to function %<va_start%>");
	      *expr_p = build_empty_stmt ();
	      return GS_OK;
	    }
	  
	  if (fold_builtin_next_arg (*expr_p, true))
	    {
	      *expr_p = build_empty_stmt ();
	      return GS_OK;
	    }
	  /* Avoid gimplifying the second argument to va_start, which needs
	     to be the plain PARM_DECL.  */
	  return gimplify_arg (&CALL_EXPR_ARG (*expr_p, 0), pre_p);
	}
    }

  /* There is a sequence point before the call, so any side effects in
     the calling expression must occur before the actual call.  Force
     gimplify_expr to use an internal post queue.  */
  ret = gimplify_expr (&CALL_EXPR_FN (*expr_p), pre_p, NULL,
		       is_gimple_call_addr, fb_rvalue);

  nargs = call_expr_nargs (*expr_p);

  /* Get argument types for verification.  */
  fndecl = get_callee_fndecl (*expr_p);
  parms = NULL_TREE;
  if (fndecl)
    parms = TYPE_ARG_TYPES (TREE_TYPE (fndecl));
  else if (POINTER_TYPE_P (TREE_TYPE (CALL_EXPR_FN (*expr_p))))
    parms = TYPE_ARG_TYPES (TREE_TYPE (TREE_TYPE (CALL_EXPR_FN (*expr_p))));

  /* Verify if the type of the argument matches that of the function
     declaration.  If we cannot verify this or there is a mismatch,
     mark the call expression so it doesn't get inlined later.  */
  if (fndecl && DECL_ARGUMENTS (fndecl))
    {
      for (i = 0, p = DECL_ARGUMENTS (fndecl); i < nargs;
	   i++, p = TREE_CHAIN (p))
	{
	  /* We cannot distinguish a varargs function from the case
	     of excess parameters, still deferring the inlining decision
	     to the callee is possible.  */
	  if (!p)
	    break;
	  if (p == error_mark_node
	      || CALL_EXPR_ARG (*expr_p, i) == error_mark_node
	      || !fold_convertible_p (DECL_ARG_TYPE (p),
				      CALL_EXPR_ARG (*expr_p, i)))
	    {
	      CALL_CANNOT_INLINE_P (*expr_p) = 1;
	      break;
	    }
	}
    }
  else if (parms)
    {
      for (i = 0, p = parms; i < nargs; i++, p = TREE_CHAIN (p))
	{
	  /* If this is a varargs function defer inlining decision
	     to callee.  */
	  if (!p)
	    break;
	  if (TREE_VALUE (p) == error_mark_node
	      || CALL_EXPR_ARG (*expr_p, i) == error_mark_node
	      || TREE_CODE (TREE_VALUE (p)) == VOID_TYPE
	      || !fold_convertible_p (TREE_VALUE (p),
				      CALL_EXPR_ARG (*expr_p, i)))
	    {
	      CALL_CANNOT_INLINE_P (*expr_p) = 1;
	      break;
	    }
	}
    }
  else if (nargs != 0)
    CALL_CANNOT_INLINE_P (*expr_p) = 1;

  /* Finally, gimplify the function arguments.  */
  for (i = (PUSH_ARGS_REVERSED ? nargs - 1 : 0);
       PUSH_ARGS_REVERSED ? i >= 0 : i < nargs;
       PUSH_ARGS_REVERSED ? i-- : i++)
    {
      enum gimplify_status t;

      t = gimplify_arg (&CALL_EXPR_ARG (*expr_p, i), pre_p);

      if (t == GS_ERROR)
	ret = GS_ERROR;

      VEC_safe_push (tree, gc, args, CALL_EXPR_ARG (*expr_p, i));
    }

  /* Try this again in case gimplification exposed something.  */
  if (ret != GS_ERROR)
    {
      tree new = fold_call_expr (*expr_p, !want_value);

      if (new && new != *expr_p)
	{
	  /* There was a transformation of this call which computes the
	     same value, but in a more efficient way.  Return and try
	     again.  */
	  *expr_p = new;
	  return GS_OK;
	}
    }

  /* If the function is "const" or "pure", then clear
     TREE_SIDE_EFFECTS on its decl.  This allows us to eliminate
     redundant or useless calls to "const" functions.  */
  /* FIXME tuples.  Must handle TREE_SIDE_EFFECTS on GIMPLE_CALL too.  */
  if (TREE_CODE (*expr_p) == CALL_EXPR
      && (call_expr_flags (*expr_p) & (ECF_CONST | ECF_PURE)))
    TREE_SIDE_EFFECTS (*expr_p) = 0;

  /* Now add the GIMPLE call to PRE_P.  If WANT_VALUE is set, we need
     to create the appropriate temporary for the call's LHS.  */
  call = build_gimple_call_vec (fndecl ? fndecl : CALL_EXPR_FN (*expr_p), args);
  gimple_seq_add (pre_p, call);
  if (want_value)
    {
      tree lhs = get_tmp_var_for (call);
      gimple_call_set_lhs (call, lhs);
      *expr_p = lhs;
    }
  else
    *expr_p = NULL_TREE;

  return ret;
}

/* Handle shortcut semantics in the predicate operand of a COND_EXPR by
   rewriting it into multiple COND_EXPRs, and possibly GOTO_EXPRs.

   TRUE_LABEL_P and FALSE_LABEL_P point to the labels to jump to if the
   condition is true or false, respectively.  If null, we should generate
   our own to skip over the evaluation of this specific expression.

   This function is the tree equivalent of do_jump.

   shortcut_cond_r should only be called by shortcut_cond_expr.  */

static tree
shortcut_cond_r (tree pred, tree *true_label_p, tree *false_label_p)
{
  tree local_label = NULL_TREE;
  tree t, expr = NULL;

  /* OK, it's not a simple case; we need to pull apart the COND_EXPR to
     retain the shortcut semantics.  Just insert the gotos here;
     shortcut_cond_expr will append the real blocks later.  */
  if (TREE_CODE (pred) == TRUTH_ANDIF_EXPR)
    {
      /* Turn if (a && b) into

	 if (a); else goto no;
	 if (b) goto yes; else goto no;
	 (no:) */

      if (false_label_p == NULL)
	false_label_p = &local_label;

      t = shortcut_cond_r (TREE_OPERAND (pred, 0), NULL, false_label_p);
      append_to_statement_list (t, &expr);

      t = shortcut_cond_r (TREE_OPERAND (pred, 1), true_label_p,
			   false_label_p);
      append_to_statement_list (t, &expr);
    }
  else if (TREE_CODE (pred) == TRUTH_ORIF_EXPR)
    {
      /* Turn if (a || b) into

	 if (a) goto yes;
	 if (b) goto yes; else goto no;
	 (yes:) */

      if (true_label_p == NULL)
	true_label_p = &local_label;

      t = shortcut_cond_r (TREE_OPERAND (pred, 0), true_label_p, NULL);
      append_to_statement_list (t, &expr);

      t = shortcut_cond_r (TREE_OPERAND (pred, 1), true_label_p,
			   false_label_p);
      append_to_statement_list (t, &expr);
    }
  else if (TREE_CODE (pred) == COND_EXPR)
    {
      /* As long as we're messing with gotos, turn if (a ? b : c) into
	 if (a)
	   if (b) goto yes; else goto no;
	 else
	   if (c) goto yes; else goto no;  */
      expr = build3 (COND_EXPR, void_type_node, TREE_OPERAND (pred, 0),
		     shortcut_cond_r (TREE_OPERAND (pred, 1), true_label_p,
				      false_label_p),
		     shortcut_cond_r (TREE_OPERAND (pred, 2), true_label_p,
				      false_label_p));
    }
  else
    {
      expr = build3 (COND_EXPR, void_type_node, pred,
		     build_and_jump (true_label_p),
		     build_and_jump (false_label_p));
    }

  if (local_label)
    {
      t = build1 (LABEL_EXPR, void_type_node, local_label);
      append_to_statement_list (t, &expr);
    }

  return expr;
}

/* Given a conditional expression EXPR with short-circuit boolean
   predicates using TRUTH_ANDIF_EXPR or TRUTH_ORIF_EXPR, break the
   predicate appart into the equivalent sequence of conditionals.  */

static tree
shortcut_cond_expr (tree expr)
{
  tree pred = TREE_OPERAND (expr, 0);
  tree then_ = TREE_OPERAND (expr, 1);
  tree else_ = TREE_OPERAND (expr, 2);
  tree true_label, false_label, end_label, t;
  tree *true_label_p;
  tree *false_label_p;
  bool emit_end, emit_false, jump_over_else;
  bool then_se = then_ && TREE_SIDE_EFFECTS (then_);
  bool else_se = else_ && TREE_SIDE_EFFECTS (else_);

  /* First do simple transformations.  */
  if (!else_se)
    {
      /* If there is no 'else', turn (a && b) into if (a) if (b).  */
      while (TREE_CODE (pred) == TRUTH_ANDIF_EXPR)
	{
	  TREE_OPERAND (expr, 0) = TREE_OPERAND (pred, 1);
	  then_ = shortcut_cond_expr (expr);
	  then_se = then_ && TREE_SIDE_EFFECTS (then_);
	  pred = TREE_OPERAND (pred, 0);
	  expr = build3 (COND_EXPR, void_type_node, pred, then_, NULL_TREE);
	}
    }

  if (!then_se)
    {
      /* If there is no 'then', turn
	   if (a || b); else d
	 into
	   if (a); else if (b); else d.  */
      while (TREE_CODE (pred) == TRUTH_ORIF_EXPR)
	{
	  TREE_OPERAND (expr, 0) = TREE_OPERAND (pred, 1);
	  else_ = shortcut_cond_expr (expr);
	  else_se = else_ && TREE_SIDE_EFFECTS (else_);
	  pred = TREE_OPERAND (pred, 0);
	  expr = build3 (COND_EXPR, void_type_node, pred, NULL_TREE, else_);
	}
    }

  /* If we're done, great.  */
  if (TREE_CODE (pred) != TRUTH_ANDIF_EXPR
      && TREE_CODE (pred) != TRUTH_ORIF_EXPR)
    return expr;

  /* Otherwise we need to mess with gotos.  Change
       if (a) c; else d;
     to
       if (a); else goto no;
       c; goto end;
       no: d; end:
     and recursively gimplify the condition.  */

  true_label = false_label = end_label = NULL_TREE;

  /* If our arms just jump somewhere, hijack those labels so we don't
     generate jumps to jumps.  */

  if (then_
      && TREE_CODE (then_) == GOTO_EXPR
      && TREE_CODE (GOTO_DESTINATION (then_)) == LABEL_DECL)
    {
      true_label = GOTO_DESTINATION (then_);
      then_ = NULL;
      then_se = false;
    }

  if (else_
      && TREE_CODE (else_) == GOTO_EXPR
      && TREE_CODE (GOTO_DESTINATION (else_)) == LABEL_DECL)
    {
      false_label = GOTO_DESTINATION (else_);
      else_ = NULL;
      else_se = false;
    }

  /* If we aren't hijacking a label for the 'then' branch, it falls through.  */
  if (true_label)
    true_label_p = &true_label;
  else
    true_label_p = NULL;

  /* The 'else' branch also needs a label if it contains interesting code.  */
  if (false_label || else_se)
    false_label_p = &false_label;
  else
    false_label_p = NULL;

  /* If there was nothing else in our arms, just forward the label(s).  */
  if (!then_se && !else_se)
    return shortcut_cond_r (pred, true_label_p, false_label_p);

  /* If our last subexpression already has a terminal label, reuse it.  */
  if (else_se)
    expr = expr_last (else_);
  else if (then_se)
    expr = expr_last (then_);
  else
    expr = NULL;
  if (expr && TREE_CODE (expr) == LABEL_EXPR)
    end_label = LABEL_EXPR_LABEL (expr);

  /* If we don't care about jumping to the 'else' branch, jump to the end
     if the condition is false.  */
  if (!false_label_p)
    false_label_p = &end_label;

  /* We only want to emit these labels if we aren't hijacking them.  */
  emit_end = (end_label == NULL_TREE);
  emit_false = (false_label == NULL_TREE);

  /* We only emit the jump over the else clause if we have to--if the
     then clause may fall through.  Otherwise we can wind up with a
     useless jump and a useless label at the end of gimplified code,
     which will cause us to think that this conditional as a whole
     falls through even if it doesn't.  If we then inline a function
     which ends with such a condition, that can cause us to issue an
     inappropriate warning about control reaching the end of a
     non-void function.  */
  jump_over_else = block_may_fallthru (then_);

  pred = shortcut_cond_r (pred, true_label_p, false_label_p);

  expr = NULL;
  append_to_statement_list (pred, &expr);

  append_to_statement_list (then_, &expr);
  if (else_se)
    {
      if (jump_over_else)
	{
	  t = build_and_jump (&end_label);
	  append_to_statement_list (t, &expr);
	}
      if (emit_false)
	{
	  t = build1 (LABEL_EXPR, void_type_node, false_label);
	  append_to_statement_list (t, &expr);
	}
      append_to_statement_list (else_, &expr);
    }
  if (emit_end && end_label)
    {
      t = build1 (LABEL_EXPR, void_type_node, end_label);
      append_to_statement_list (t, &expr);
    }

  return expr;
}

/* EXPR is used in a boolean context; make sure it has BOOLEAN_TYPE.  */

tree
gimple_boolify (tree expr)
{
  tree type = TREE_TYPE (expr);

  if (TREE_CODE (type) == BOOLEAN_TYPE)
    return expr;

  switch (TREE_CODE (expr))
    {
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
      /* Also boolify the arguments of truth exprs.  */
      TREE_OPERAND (expr, 1) = gimple_boolify (TREE_OPERAND (expr, 1));
      /* FALLTHRU */

    case TRUTH_NOT_EXPR:
      TREE_OPERAND (expr, 0) = gimple_boolify (TREE_OPERAND (expr, 0));
      /* FALLTHRU */

    case EQ_EXPR: case NE_EXPR:
    case LE_EXPR: case GE_EXPR: case LT_EXPR: case GT_EXPR:
      /* These expressions always produce boolean results.  */
      TREE_TYPE (expr) = boolean_type_node;
      return expr;

    default:
      /* Other expressions that get here must have boolean values, but
	 might need to be converted to the appropriate mode.  */
      return fold_convert (boolean_type_node, expr);
    }
}

/*  Convert the conditional expression pointed to by EXPR_P '(p) ? a : b;'
    into

    if (p)			if (p)
      t1 = a;			  a;
    else		or	else
      t1 = b;			  b;
    t1;

    The second form is used when *EXPR_P is of type void.

    PRE_P points to the list where side effects that must happen before
      *EXPR_P should be stored.  */

static enum gimplify_status
gimplify_cond_expr (tree *expr_p, gimple_seq pre_p, fallback_t fallback)
{
  tree expr = *expr_p;
  tree tmp, type, arm1, arm2;
  enum gimplify_status ret;
  tree label_true, label_false, label_cont;
  bool have_then_clause_p, have_else_clause_p;
  gimple gimple_cond;
  enum gimple_cond pred;

  type = TREE_TYPE (expr);

  /* If this COND_EXPR has a value, copy the values into a temporary within
     the arms.  */
  if (! VOID_TYPE_P (type))
    {
      tree result;

      if ((fallback & fb_lvalue) == 0)
	result = tmp = create_tmp_var (TREE_TYPE (expr), "iftmp");
      else
	{
	  tree type = build_pointer_type (TREE_TYPE (expr));

	  if (TREE_TYPE (TREE_OPERAND (expr, 1)) != void_type_node)
	    TREE_OPERAND (expr, 1) =
	      build_fold_addr_expr (TREE_OPERAND (expr, 1));

	  if (TREE_TYPE (TREE_OPERAND (expr, 2)) != void_type_node)
	    TREE_OPERAND (expr, 2) =
	      build_fold_addr_expr (TREE_OPERAND (expr, 2));
	  
	  tmp = create_tmp_var (type, "iftmp");

	  expr = build3 (COND_EXPR, void_type_node, TREE_OPERAND (expr, 0),
			 TREE_OPERAND (expr, 1), TREE_OPERAND (expr, 2));

	  result = build_fold_indirect_ref (tmp);
	}

      /* Build the then clause, 't1 = a;'.  But don't build an assignment
	 if this branch is void; in C++ it can be, if it's a throw.  */
      if (TREE_TYPE (TREE_OPERAND (expr, 1)) != void_type_node)
	TREE_OPERAND (expr, 1)
	  = build_gimple_modify_stmt (tmp, TREE_OPERAND (expr, 1));

      /* Build the else clause, 't1 = b;'.  */
      if (TREE_TYPE (TREE_OPERAND (expr, 2)) != void_type_node)
	TREE_OPERAND (expr, 2)
	  = build_gimple_modify_stmt (tmp, TREE_OPERAND (expr, 2));

      TREE_TYPE (expr) = void_type_node;
      recalculate_side_effects (expr);

      /* Move the COND_EXPR to the prequeue.  */
      gimplify_stmt (&expr, pre_p);

      *expr_p = result;
      return GS_ALL_DONE;
    }

  /* Make sure the condition has BOOLEAN_TYPE.  */
  TREE_OPERAND (expr, 0) = gimple_boolify (TREE_OPERAND (expr, 0));

  /* Break apart && and || conditions.  */
  if (TREE_CODE (TREE_OPERAND (expr, 0)) == TRUTH_ANDIF_EXPR
      || TREE_CODE (TREE_OPERAND (expr, 0)) == TRUTH_ORIF_EXPR)
    {
      expr = shortcut_cond_expr (expr);

      if (expr != *expr_p)
	{
	  *expr_p = expr;

	  /* We can't rely on gimplify_expr to re-gimplify the expanded
	     form properly, as cleanups might cause the target labels to be
	     wrapped in a TRY_FINALLY_EXPR.  To prevent that, we need to
	     set up a conditional context.  */
	  gimple_push_condition ();
	  gimplify_stmt (expr_p, pre_p);
	  gimple_pop_condition (pre_p);

	  return GS_ALL_DONE;
	}
    }

  /* Now do the normal gimplification.  */

  /* Gimplify condition.  */
  ret = gimplify_expr (&TREE_OPERAND (expr, 0), pre_p, NULL, is_gimple_condexpr,
		       fb_rvalue);
  gcc_assert (TREE_OPERAND (expr, 0) != NULL_TREE);

  gimple_push_condition ();

  label_true = create_artificial_label ();
  label_false = create_artificial_label ();

  arm1 = arm2 = NULL_TREE;
  switch (TREE_CODE (TREE_OPERAND (expr, 0)))
    {
    case LT_EXPR:
      pred = GIMPLE_COND_LT;
      break;
    case LE_EXPR:
      pred = GIMPLE_COND_LE;
      break;
    case GT_EXPR:
      pred = GIMPLE_COND_GT;
      break;
    case GE_EXPR:
      pred = GIMPLE_COND_GE;
      break;
    case EQ_EXPR:
      pred = GIMPLE_COND_EQ;
      break;
    case NE_EXPR:
      pred = GIMPLE_COND_NE;
      break;
    default:
      pred = GIMPLE_COND_NE;
      arm1 = TREE_OPERAND (expr, 0);
      arm2 = boolean_false_node;
      break;
    }
  if (arm1 == NULL_TREE)
    {
      arm1 = TREE_OPERAND (TREE_OPERAND (expr, 0), 0);
      arm2 = TREE_OPERAND (TREE_OPERAND (expr, 0), 1);
      gimple_cond = build_gimple_cond (pred, arm1, arm2, label_true,
				       label_false);
    }
  else
    {
      /* Use canonical comparison with false.  */
      gimple_cond = build_gimple_cond (pred, arm1, arm2, label_true,
                                       label_false);
    }

  gimple_seq_add (pre_p, gimple_cond);
  gimple_seq_add (pre_p, build_gimple_label (label_true));
  have_then_clause_p = gimplify_stmt (&TREE_OPERAND (expr, 1), pre_p);
  label_cont = create_artificial_label ();
  gimple_seq_add (pre_p, build_gimple_goto (label_cont));
  gimple_seq_add (pre_p, build_gimple_label (label_false));
  have_else_clause_p = gimplify_stmt (&TREE_OPERAND (expr, 2), pre_p);
  gimple_seq_add (pre_p, build_gimple_label (label_cont));

  gimple_pop_condition (pre_p);

  if (ret == GS_ERROR)
    ;
  else if (have_then_clause_p)
    ret = GS_ALL_DONE;
  else if (have_else_clause_p)
    {
      /* Rewrite "if (a); else b" into "if (!a) b"  */
      gimple_cond_invert (gimple_cond);
    }
  else
    {
      /* Both arms are empty; replace the COND_EXPR with its predicate.  */
      expr = TREE_OPERAND (expr, 0);
      gimplify_stmt (&expr, pre_p);
    }

  *expr_p = NULL;
  return ret;
}

/* A subroutine of gimplify_modify_expr.  Replace a MODIFY_EXPR with
   a call to __builtin_memcpy.  */

static enum gimplify_status
gimplify_modify_expr_to_memcpy (tree *expr_p, tree size, bool want_value,
    				gimple_seq seq_p)
{
  tree t, to, to_ptr, from, from_ptr;
  gimple gs;

  to = GENERIC_TREE_OPERAND (*expr_p, 0);
  from = GENERIC_TREE_OPERAND (*expr_p, 1);

  from_ptr = build_fold_addr_expr (from);
  to_ptr = build_fold_addr_expr (to);
  t = implicit_built_in_decls[BUILT_IN_MEMCPY];

  gs = build_gimple_call (t, 3, to_ptr, from_ptr, size);

  if (want_value)
    {
      /* tmp = memcpy() */
      t = create_tmp_var (TREE_TYPE (to_ptr), NULL);
      gimple_call_set_lhs (gs, t);
      gimple_seq_add (seq_p, gs);

      *expr_p = build1 (INDIRECT_REF, TREE_TYPE (to), t);
      return GS_ALL_DONE;
    }

  gimple_seq_add (seq_p, gs);
  *expr_p = NULL;
  return GS_ALL_DONE;
}

/* A subroutine of gimplify_modify_expr.  Replace a MODIFY_EXPR with
   a call to __builtin_memset.  In this case we know that the RHS is
   a CONSTRUCTOR with an empty element list.  */

static enum gimplify_status
gimplify_modify_expr_to_memset (tree *expr_p, tree size, bool want_value,
    				gimple_seq seq_p)
{
  tree t, to, to_ptr;
  gimple gs;

  to = GENERIC_TREE_OPERAND (*expr_p, 0);

  to_ptr = build_fold_addr_expr (to);
  t = implicit_built_in_decls[BUILT_IN_MEMSET];

  gs = build_gimple_call (t, 3, to_ptr, integer_zero_node, size);

  if (want_value)
    {
      /* tmp = memset() */
      t = create_tmp_var (TREE_TYPE (to_ptr), NULL);
      gimple_call_set_lhs (gs, t);
      gimple_seq_add (seq_p, gs);

      *expr_p = build1 (INDIRECT_REF, TREE_TYPE (to), t);
      return GS_ALL_DONE;
    }

  gimple_seq_add (seq_p, gs);
  *expr_p = NULL;
  return GS_ALL_DONE;
}

/* A subroutine of gimplify_init_ctor_preeval.  Called via walk_tree,
   determine, cautiously, if a CONSTRUCTOR overlaps the lhs of an
   assignment.  Returns non-null if we detect a potential overlap.  */

struct gimplify_init_ctor_preeval_data
{
  /* The base decl of the lhs object.  May be NULL, in which case we
     have to assume the lhs is indirect.  */
  tree lhs_base_decl;

  /* The alias set of the lhs object.  */
  alias_set_type lhs_alias_set;
};

static tree
gimplify_init_ctor_preeval_1 (tree *tp, int *walk_subtrees, void *xdata)
{
  struct gimplify_init_ctor_preeval_data *data
    = (struct gimplify_init_ctor_preeval_data *) xdata;
  tree t = *tp;

  /* If we find the base object, obviously we have overlap.  */
  if (data->lhs_base_decl == t)
    return t;

  /* If the constructor component is indirect, determine if we have a
     potential overlap with the lhs.  The only bits of information we
     have to go on at this point are addressability and alias sets.  */
  if (TREE_CODE (t) == INDIRECT_REF
      && (!data->lhs_base_decl || TREE_ADDRESSABLE (data->lhs_base_decl))
      && alias_sets_conflict_p (data->lhs_alias_set, get_alias_set (t)))
    return t;

  /* If the constructor component is a call, determine if it can hide a
     potential overlap with the lhs through an INDIRECT_REF like above.  */
  if (TREE_CODE (t) == CALL_EXPR)
    {
      tree type, fntype = TREE_TYPE (TREE_TYPE (CALL_EXPR_FN (t)));

      for (type = TYPE_ARG_TYPES (fntype); type; type = TREE_CHAIN (type))
	if (POINTER_TYPE_P (TREE_VALUE (type))
	    && (!data->lhs_base_decl || TREE_ADDRESSABLE (data->lhs_base_decl))
	    && alias_sets_conflict_p (data->lhs_alias_set,
				      get_alias_set
				        (TREE_TYPE (TREE_VALUE (type)))))
	  return t;
    }

  if (IS_TYPE_OR_DECL_P (t))
    *walk_subtrees = 0;
  return NULL;
}

/* A subroutine of gimplify_init_constructor.  Pre-evaluate EXPR,
   force values that overlap with the lhs (as described by *DATA)
   into temporaries.  */

static void
gimplify_init_ctor_preeval (tree *expr_p, gimple_seq pre_p, gimple_seq post_p,
			    struct gimplify_init_ctor_preeval_data *data)
{
  enum gimplify_status one;

  /* If the value is invariant, then there's nothing to pre-evaluate.
     But ensure it doesn't have any side-effects since a SAVE_EXPR is
     invariant but has side effects and might contain a reference to
     the object we're initializing.  */
  if (TREE_INVARIANT (*expr_p) && !TREE_SIDE_EFFECTS (*expr_p))
    return;

  /* If the type has non-trivial constructors, we can't pre-evaluate.  */
  if (TREE_ADDRESSABLE (TREE_TYPE (*expr_p)))
    return;

  /* Recurse for nested constructors.  */
  if (TREE_CODE (*expr_p) == CONSTRUCTOR)
    {
      unsigned HOST_WIDE_INT ix;
      constructor_elt *ce;
      VEC(constructor_elt,gc) *v = CONSTRUCTOR_ELTS (*expr_p);

      for (ix = 0; VEC_iterate (constructor_elt, v, ix, ce); ix++)
	gimplify_init_ctor_preeval (&ce->value, pre_p, post_p, data);

      return;
    }

  /* If this is a variable sized type, we must remember the size.  */
  maybe_with_size_expr (expr_p);

  /* Gimplify the constructor element to something appropriate for the rhs
     of a MODIFY_EXPR.  Given that we know the lhs is an aggregate, we know
     the gimplifier will consider this a store to memory.  Doing this
     gimplification now means that we won't have to deal with complicated
     language-specific trees, nor trees like SAVE_EXPR that can induce
     exponential search behavior.  */
  one = gimplify_expr (expr_p, pre_p, post_p, is_gimple_mem_rhs, fb_rvalue);
  if (one == GS_ERROR)
    {
      *expr_p = NULL;
      return;
    }

  /* If we gimplified to a bare decl, we can be sure that it doesn't overlap
     with the lhs, since "a = { .x=a }" doesn't make sense.  This will
     always be true for all scalars, since is_gimple_mem_rhs insists on a
     temporary variable for them.  */
  if (DECL_P (*expr_p))
    return;

  /* If this is of variable size, we have no choice but to assume it doesn't
     overlap since we can't make a temporary for it.  */
  if (TREE_CODE (TYPE_SIZE (TREE_TYPE (*expr_p))) != INTEGER_CST)
    return;

  /* Otherwise, we must search for overlap ...  */
  if (!walk_tree (expr_p, gimplify_init_ctor_preeval_1, data, NULL))
    return;

  /* ... and if found, force the value into a temporary.  */
  *expr_p = get_formal_tmp_var (*expr_p, pre_p);
}

/* A subroutine of gimplify_init_ctor_eval.  Create a loop for
   a RANGE_EXPR in a CONSTRUCTOR for an array.

      var = lower;
    loop_entry:
      object[var] = value;
      if (var == upper)
	goto loop_exit;
      var = var + 1;
      goto loop_entry;
    loop_exit:

   We increment var _after_ the loop exit check because we might otherwise
   fail if upper == TYPE_MAX_VALUE (type for upper).

   Note that we never have to deal with SAVE_EXPRs here, because this has
   already been taken care of for us, in gimplify_init_ctor_preeval().  */

static void gimplify_init_ctor_eval (tree, VEC(constructor_elt,gc) *,
				     gimple_seq, bool);

static void
gimplify_init_ctor_eval_range (tree object, tree lower, tree upper,
			       tree value, tree array_elt_type,
			       gimple_seq pre_p, bool cleared)
{
  tree loop_entry_label, loop_exit_label;
  tree var, var_type, cref, tmp;

  loop_entry_label = create_artificial_label ();
  loop_exit_label = create_artificial_label ();

  /* Create and initialize the index variable.  */
  var_type = TREE_TYPE (upper);
  var = create_tmp_var (var_type, NULL);
  gimple_seq_add (pre_p, build_gimple_assign (var, lower));

  /* Add the loop entry label.  */
  gimple_seq_add (pre_p, build_gimple_label (loop_entry_label));

  /* Build the reference.  */
  cref = build4 (ARRAY_REF, array_elt_type, unshare_expr (object),
		 var, NULL_TREE, NULL_TREE);

  /* If we are a constructor, just call gimplify_init_ctor_eval to do
     the store.  Otherwise just assign value to the reference.  */

  if (TREE_CODE (value) == CONSTRUCTOR)
    /* NB we might have to call ourself recursively through
       gimplify_init_ctor_eval if the value is a constructor.  */
    gimplify_init_ctor_eval (cref, CONSTRUCTOR_ELTS (value),
			     pre_p, cleared);
  else
    gimple_seq_add (pre_p, build_gimple_assign (cref, value));

  /* We exit the loop when the index var is equal to the upper bound.  */
  gimple_seq_add (pre_p,
	  build_gimple_cond (GIMPLE_COND_EQ, var, upper, loop_exit_label,
	                     NULL_TREE));

  /* Otherwise, increment the index var...  */
  tmp = build2 (PLUS_EXPR, var_type, var,
		fold_convert (var_type, integer_one_node));
  gimple_seq_add (pre_p, build_gimple_assign (var, tmp));

  /* ...and jump back to the loop entry.  */
  gimple_seq_add (pre_p, build_gimple_goto (loop_entry_label));

  /* Add the loop exit label.  */
  gimple_seq_add (pre_p, build_gimple_label (loop_exit_label));
}

/* Return true if FDECL is accessing a field that is zero sized.  */
   
static bool
zero_sized_field_decl (const_tree fdecl)
{
  if (TREE_CODE (fdecl) == FIELD_DECL && DECL_SIZE (fdecl) 
      && integer_zerop (DECL_SIZE (fdecl)))
    return true;
  return false;
}

/* Return true if TYPE is zero sized.  */
   
static bool
zero_sized_type (const_tree type)
{
  if (AGGREGATE_TYPE_P (type) && TYPE_SIZE (type)
      && integer_zerop (TYPE_SIZE (type)))
    return true;
  return false;
}

/* A subroutine of gimplify_init_constructor.  Generate individual
   MODIFY_EXPRs for a CONSTRUCTOR.  OBJECT is the LHS against which the
   assignments should happen.  ELTS is the CONSTRUCTOR_ELTS of the
   CONSTRUCTOR.  CLEARED is true if the entire LHS object has been
   zeroed first.  */

static void
gimplify_init_ctor_eval (tree object, VEC(constructor_elt,gc) *elts,
			 gimple_seq pre_p, bool cleared)
{
  tree array_elt_type = NULL;
  unsigned HOST_WIDE_INT ix;
  tree purpose, value;

  if (TREE_CODE (TREE_TYPE (object)) == ARRAY_TYPE)
    array_elt_type = TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (object)));

  FOR_EACH_CONSTRUCTOR_ELT (elts, ix, purpose, value)
    {
      tree cref;

      /* NULL values are created above for gimplification errors.  */
      if (value == NULL)
	continue;

      if (cleared && initializer_zerop (value))
	continue;

      /* ??? Here's to hoping the front end fills in all of the indices,
	 so we don't have to figure out what's missing ourselves.  */
      gcc_assert (purpose);

      /* Skip zero-sized fields, unless value has side-effects.  This can
	 happen with calls to functions returning a zero-sized type, which
	 we shouldn't discard.  As a number of downstream passes don't
	 expect sets of zero-sized fields, we rely on the gimplification of
	 the MODIFY_EXPR we make below to drop the assignment statement.  */
      if (! TREE_SIDE_EFFECTS (value) && zero_sized_field_decl (purpose))
	continue;

      /* If we have a RANGE_EXPR, we have to build a loop to assign the
	 whole range.  */
      if (TREE_CODE (purpose) == RANGE_EXPR)
	{
	  tree lower = TREE_OPERAND (purpose, 0);
	  tree upper = TREE_OPERAND (purpose, 1);

	  /* If the lower bound is equal to upper, just treat it as if
	     upper was the index.  */
	  if (simple_cst_equal (lower, upper))
	    purpose = upper;
	  else
	    {
	      gimplify_init_ctor_eval_range (object, lower, upper, value,
					     array_elt_type, pre_p, cleared);
	      continue;
	    }
	}

      if (array_elt_type)
	{
	  cref = build4 (ARRAY_REF, array_elt_type, unshare_expr (object),
			 purpose, NULL_TREE, NULL_TREE);
	}
      else
	{
	  gcc_assert (TREE_CODE (purpose) == FIELD_DECL);
	  cref = build3 (COMPONENT_REF, TREE_TYPE (purpose),
			 unshare_expr (object), purpose, NULL_TREE);
	}

      if (TREE_CODE (value) == CONSTRUCTOR
	  && TREE_CODE (TREE_TYPE (value)) != VECTOR_TYPE)
	gimplify_init_ctor_eval (cref, CONSTRUCTOR_ELTS (value),
				 pre_p, cleared);
      else
	gimple_seq_add (pre_p, build_gimple_assign (cref, value));
    }
}

/* A subroutine of gimplify_modify_expr.  Break out elements of a
   CONSTRUCTOR used as an initializer into separate MODIFY_EXPRs.

   Note that we still need to clear any elements that don't have explicit
   initializers, so if not all elements are initialized we keep the
   original MODIFY_EXPR, we just remove all of the constructor elements.  */

static enum gimplify_status
gimplify_init_constructor (tree *expr_p, gimple_seq pre_p, gimple_seq post_p,
			   bool want_value)
{
  tree object;
  tree ctor = GENERIC_TREE_OPERAND (*expr_p, 1);
  tree type = TREE_TYPE (ctor);
  enum gimplify_status ret;
  VEC(constructor_elt,gc) *elts;

  if (TREE_CODE (ctor) != CONSTRUCTOR)
    return GS_UNHANDLED;

  ret = gimplify_expr (&GENERIC_TREE_OPERAND (*expr_p, 0), pre_p, post_p,
		       is_gimple_lvalue, fb_lvalue);
  if (ret == GS_ERROR)
    return ret;

  object = GENERIC_TREE_OPERAND (*expr_p, 0);
  elts = CONSTRUCTOR_ELTS (ctor);
  ret = GS_ALL_DONE;

  switch (TREE_CODE (type))
    {
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
    case ARRAY_TYPE:
      {
	struct gimplify_init_ctor_preeval_data preeval_data;
	HOST_WIDE_INT num_type_elements, num_ctor_elements;
	HOST_WIDE_INT num_nonzero_elements;
	bool cleared, valid_const_initializer;

	/* Aggregate types must lower constructors to initialization of
	   individual elements.  The exception is that a CONSTRUCTOR node
	   with no elements indicates zero-initialization of the whole.  */
	if (VEC_empty (constructor_elt, elts))
	  break;
 
	/* Fetch information about the constructor to direct later processing.
	   We might want to make static versions of it in various cases, and
	   can only do so if it known to be a valid constant initializer.  */
	valid_const_initializer
	  = categorize_ctor_elements (ctor, &num_nonzero_elements,
				      &num_ctor_elements, &cleared);

	/* If a const aggregate variable is being initialized, then it
	   should never be a lose to promote the variable to be static.  */
	if (valid_const_initializer
	    && num_nonzero_elements > 1
	    && TREE_READONLY (object)
	    && TREE_CODE (object) == VAR_DECL)
	  {
	    DECL_INITIAL (object) = ctor;
	    TREE_STATIC (object) = 1;
	    if (!DECL_NAME (object))
	      DECL_NAME (object) = create_tmp_var_name ("C");
	    walk_tree (&DECL_INITIAL (object), force_labels_r, NULL, NULL);

	    /* ??? C++ doesn't automatically append a .<number> to the
	       assembler name, and even when it does, it looks a FE private
	       data structures to figure out what that number should be,
	       which are not set for this variable.  I suppose this is
	       important for local statics for inline functions, which aren't
	       "local" in the object file sense.  So in order to get a unique
	       TU-local symbol, we must invoke the lhd version now.  */
	    lhd_set_decl_assembler_name (object);

	    *expr_p = NULL_TREE;
	    break;
	  }

	/* If there are "lots" of initialized elements, even discounting
	   those that are not address constants (and thus *must* be
	   computed at runtime), then partition the constructor into
	   constant and non-constant parts.  Block copy the constant
	   parts in, then generate code for the non-constant parts.  */
	/* TODO.  There's code in cp/typeck.c to do this.  */

	num_type_elements = count_type_elements (type, true);

	/* If count_type_elements could not determine number of type elements
	   for a constant-sized object, assume clearing is needed.
	   Don't do this for variable-sized objects, as store_constructor
	   will ignore the clearing of variable-sized objects.  */
	if (num_type_elements < 0 && int_size_in_bytes (type) >= 0)
	  cleared = true;
	/* If there are "lots" of zeros, then block clear the object first.  */
	else if (num_type_elements - num_nonzero_elements > CLEAR_RATIO
		 && num_nonzero_elements < num_type_elements/4)
	  cleared = true;
	/* ??? This bit ought not be needed.  For any element not present
	   in the initializer, we should simply set them to zero.  Except
	   we'd need to *find* the elements that are not present, and that
	   requires trickery to avoid quadratic compile-time behavior in
	   large cases or excessive memory use in small cases.  */
	else if (num_ctor_elements < num_type_elements)
	  cleared = true;

	/* If there are "lots" of initialized elements, and all of them
	   are valid address constants, then the entire initializer can
	   be dropped to memory, and then memcpy'd out.  Don't do this
	   for sparse arrays, though, as it's more efficient to follow
	   the standard CONSTRUCTOR behavior of memset followed by
	   individual element initialization.  */
	if (valid_const_initializer && !cleared)
	  {
	    HOST_WIDE_INT size = int_size_in_bytes (type);
	    unsigned int align;

	    /* ??? We can still get unbounded array types, at least
	       from the C++ front end.  This seems wrong, but attempt
	       to work around it for now.  */
	    if (size < 0)
	      {
		size = int_size_in_bytes (TREE_TYPE (object));
		if (size >= 0)
		  TREE_TYPE (ctor) = type = TREE_TYPE (object);
	      }

	    /* Find the maximum alignment we can assume for the object.  */
	    /* ??? Make use of DECL_OFFSET_ALIGN.  */
	    if (DECL_P (object))
	      align = DECL_ALIGN (object);
	    else
	      align = TYPE_ALIGN (type);

	    if (size > 0 && !can_move_by_pieces (size, align))
	      {
		tree new = create_tmp_var_raw (type, "C");

		gimple_add_tmp_var (new);
		TREE_STATIC (new) = 1;
		TREE_READONLY (new) = 1;
		DECL_INITIAL (new) = ctor;
		if (align > DECL_ALIGN (new))
		  {
		    DECL_ALIGN (new) = align;
		    DECL_USER_ALIGN (new) = 1;
		  }
	        walk_tree (&DECL_INITIAL (new), force_labels_r, NULL, NULL);

		GENERIC_TREE_OPERAND (*expr_p, 1) = new;

		/* This is no longer an assignment of a CONSTRUCTOR, but
		   we still may have processing to do on the LHS.  So
		   pretend we didn't do anything here to let that happen.  */
		return GS_UNHANDLED;
	      }
	  }

	/* If there are nonzero elements, pre-evaluate to capture elements
	   overlapping with the lhs into temporaries.  We must do this before
	   clearing to fetch the values before they are zeroed-out.  */
	if (num_nonzero_elements > 0)
	  {
	    preeval_data.lhs_base_decl = get_base_address (object);
	    if (!DECL_P (preeval_data.lhs_base_decl))
	      preeval_data.lhs_base_decl = NULL;
	    preeval_data.lhs_alias_set = get_alias_set (object);

	    gimplify_init_ctor_preeval (&GENERIC_TREE_OPERAND (*expr_p, 1),
					pre_p, post_p, &preeval_data);
	  }

	if (cleared)
	  {
	    /* Zap the CONSTRUCTOR element list, which simplifies this case.
	       Note that we still have to gimplify, in order to handle the
	       case of variable sized types.  Avoid shared tree structures.  */
	    CONSTRUCTOR_ELTS (ctor) = NULL;
	    object = unshare_expr (object);
	    gimplify_stmt (expr_p, pre_p);
	  }

	/* If we have not block cleared the object, or if there are nonzero
	   elements in the constructor, add assignments to the individual
	   scalar fields of the object.  */
	if (!cleared || num_nonzero_elements > 0)
	  gimplify_init_ctor_eval (object, elts, pre_p, cleared);

	*expr_p = NULL_TREE;
      }
      break;

    case COMPLEX_TYPE:
      {
	tree r, i;

	/* Extract the real and imaginary parts out of the ctor.  */
	gcc_assert (VEC_length (constructor_elt, elts) == 2);
	r = VEC_index (constructor_elt, elts, 0)->value;
	i = VEC_index (constructor_elt, elts, 1)->value;
	if (r == NULL || i == NULL)
	  {
	    tree zero = fold_convert (TREE_TYPE (type), integer_zero_node);
	    if (r == NULL)
	      r = zero;
	    if (i == NULL)
	      i = zero;
	  }

	/* Complex types have either COMPLEX_CST or COMPLEX_EXPR to
	   represent creation of a complex value.  */
	if (TREE_CONSTANT (r) && TREE_CONSTANT (i))
	  {
	    ctor = build_complex (type, r, i);
	    TREE_OPERAND (*expr_p, 1) = ctor;
	  }
	else
	  {
	    ctor = build2 (COMPLEX_EXPR, type, r, i);
	    TREE_OPERAND (*expr_p, 1) = ctor;
	    ret = gimplify_expr (&TREE_OPERAND (*expr_p, 1),
				 pre_p,
				 post_p,
				 rhs_predicate_for (TREE_OPERAND (*expr_p, 0)),
				 fb_rvalue);
	  }
      }
      break;

    case VECTOR_TYPE:
      {
	unsigned HOST_WIDE_INT ix;
	constructor_elt *ce;

	/* Go ahead and simplify constant constructors to VECTOR_CST.  */
	if (TREE_CONSTANT (ctor))
	  {
	    bool constant_p = true;
	    tree value;

	    /* Even when ctor is constant, it might contain non-*_CST
	      elements (e.g. { 1.0/0.0 - 1.0/0.0, 0.0 }) and those don't
	      belong into VECTOR_CST nodes.  */
	    FOR_EACH_CONSTRUCTOR_VALUE (elts, ix, value)
	      if (!CONSTANT_CLASS_P (value))
		{
		  constant_p = false;
		  break;
		}

	    if (constant_p)
	      {
		TREE_OPERAND (*expr_p, 1) = build_vector_from_ctor (type, elts);
		break;
	      }

	    /* Don't reduce a TREE_CONSTANT vector ctor even if we can't
	       make a VECTOR_CST.  It won't do anything for us, and it'll
	       prevent us from representing it as a single constant.  */
	    break;
	  }

	/* Vector types use CONSTRUCTOR all the way through gimple
	  compilation as a general initializer.  */
	for (ix = 0; VEC_iterate (constructor_elt, elts, ix, ce); ix++)
	  {
	    enum gimplify_status tret;
	    tret = gimplify_expr (&ce->value, pre_p, post_p, is_gimple_val,
				  fb_rvalue);
	    if (tret == GS_ERROR)
	      ret = GS_ERROR;
	  }
	if (!is_gimple_reg (GENERIC_TREE_OPERAND (*expr_p, 0)))
	  GENERIC_TREE_OPERAND (*expr_p, 1) = get_formal_tmp_var (ctor, pre_p);
      }
      break;

    default:
      /* So how did we get a CONSTRUCTOR for a scalar type?  */
      gcc_unreachable ();
    }

  if (ret == GS_ERROR)
    return GS_ERROR;
  else if (want_value)
    {
      *expr_p = object;
      return GS_OK;
    }
  else
    {
      /* If we have gimplified both sides of the initializer but have
	 not emitted an assignment, do so now.  */
      if (*expr_p)
	{
	  tree lhs = TREE_OPERAND (*expr_p, 0);
	  tree rhs = TREE_OPERAND (*expr_p, 1);
	  gimple init = build_gimple_assign (lhs, rhs);
	  gimple_seq_add (pre_p, init);
	  *expr_p = NULL;
	}

      return GS_ALL_DONE;
    }
}

/* Given a pointer value OP0, return a simplified version of an
   indirection through OP0, or NULL_TREE if no simplification is
   possible.  This may only be applied to a rhs of an expression.
   Note that the resulting type may be different from the type pointed
   to in the sense that it is still compatible from the langhooks
   point of view. */

static tree
fold_indirect_ref_rhs (tree t)
{
  tree type = TREE_TYPE (TREE_TYPE (t));
  tree sub = t;
  tree subtype;

  STRIP_USELESS_TYPE_CONVERSION (sub);
  subtype = TREE_TYPE (sub);
  if (!POINTER_TYPE_P (subtype))
    return NULL_TREE;

  if (TREE_CODE (sub) == ADDR_EXPR)
    {
      tree op = TREE_OPERAND (sub, 0);
      tree optype = TREE_TYPE (op);
      /* *&p => p */
      if (useless_type_conversion_p (type, optype))
        return op;
      /* *(foo *)&fooarray => fooarray[0] */
      else if (TREE_CODE (optype) == ARRAY_TYPE
	       && useless_type_conversion_p (type, TREE_TYPE (optype)))
       {
         tree type_domain = TYPE_DOMAIN (optype);
         tree min_val = size_zero_node;
         if (type_domain && TYPE_MIN_VALUE (type_domain))
           min_val = TYPE_MIN_VALUE (type_domain);
         return build4 (ARRAY_REF, type, op, min_val, NULL_TREE, NULL_TREE);
       }
    }

  /* *(foo *)fooarrptr => (*fooarrptr)[0] */
  if (TREE_CODE (TREE_TYPE (subtype)) == ARRAY_TYPE
      && useless_type_conversion_p (type, TREE_TYPE (TREE_TYPE (subtype))))
    {
      tree type_domain;
      tree min_val = size_zero_node;
      tree osub = sub;
      sub = fold_indirect_ref_rhs (sub);
      if (! sub)
	sub = build1 (INDIRECT_REF, TREE_TYPE (subtype), osub);
      type_domain = TYPE_DOMAIN (TREE_TYPE (sub));
      if (type_domain && TYPE_MIN_VALUE (type_domain))
        min_val = TYPE_MIN_VALUE (type_domain);
      return build4 (ARRAY_REF, type, sub, min_val, NULL_TREE, NULL_TREE);
    }

  return NULL_TREE;
}

/* Subroutine of gimplify_modify_expr to do simplifications of MODIFY_EXPRs
   based on the code of the RHS.  We loop for as long as something changes.  */

static enum gimplify_status
gimplify_modify_expr_rhs (tree *expr_p, tree *from_p, tree *to_p,
			  gimple_seq pre_p, gimple_seq post_p, bool want_value)
{
  enum gimplify_status ret = GS_OK;

  while (ret != GS_UNHANDLED)
    switch (TREE_CODE (*from_p))
      {
      case INDIRECT_REF:
	{
	  /* If we have code like 

	        *(const A*)(A*)&x

	     where the type of "x" is a (possibly cv-qualified variant
	     of "A"), treat the entire expression as identical to "x".
	     This kind of code arises in C++ when an object is bound
	     to a const reference, and if "x" is a TARGET_EXPR we want
	     to take advantage of the optimization below.  */
	  tree t = fold_indirect_ref_rhs (TREE_OPERAND (*from_p, 0));
	  if (t)
	    {
	      *from_p = t;
	      ret = GS_OK;
	    }
	  else
	    ret = GS_UNHANDLED;
	  break;
	}

      case TARGET_EXPR:
	{
	  /* If we are initializing something from a TARGET_EXPR, strip the
	     TARGET_EXPR and initialize it directly, if possible.  This can't
	     be done if the initializer is void, since that implies that the
	     temporary is set in some non-trivial way.

	     ??? What about code that pulls out the temp and uses it
	     elsewhere? I think that such code never uses the TARGET_EXPR as
	     an initializer.  If I'm wrong, we'll die because the temp won't
	     have any RTL.  In that case, I guess we'll need to replace
	     references somehow.  */
	  tree init = TARGET_EXPR_INITIAL (*from_p);

	  if (!VOID_TYPE_P (TREE_TYPE (init)))
	    {
	      *from_p = init;
	      ret = GS_OK;
	    }
	  else
	    ret = GS_UNHANDLED;
	}
	break;

      case COMPOUND_EXPR:
	/* Remove any COMPOUND_EXPR in the RHS so the following cases will be
	   caught.  */
	gimplify_compound_expr (from_p, pre_p, true);
	ret = GS_OK;
	break;

      case CONSTRUCTOR:
	/* If we're initializing from a CONSTRUCTOR, break this into
	   individual MODIFY_EXPRs.  */
	return gimplify_init_constructor (expr_p, pre_p, post_p, want_value);

      case COND_EXPR:
	/* If we're assigning to a non-register type, push the assignment
	   down into the branches.  This is mandatory for ADDRESSABLE types,
	   since we cannot generate temporaries for such, but it saves a
	   copy in other cases as well.  */
	if (!is_gimple_reg_type (TREE_TYPE (*from_p)))
	  {
	    /* This code should mirror the code in gimplify_cond_expr. */
	    enum tree_code code = TREE_CODE (*expr_p);
	    tree cond = *from_p;
	    tree result = *to_p;

	    ret = gimplify_expr (&result, pre_p, post_p, is_gimple_min_lval,
				 fb_lvalue);
	    if (ret != GS_ERROR)
	      ret = GS_OK;

	    if (TREE_TYPE (TREE_OPERAND (cond, 1)) != void_type_node)
	      TREE_OPERAND (cond, 1)
		= build2 (code, void_type_node, result,
			  TREE_OPERAND (cond, 1));
	    if (TREE_TYPE (TREE_OPERAND (cond, 2)) != void_type_node)
	      TREE_OPERAND (cond, 2)
		= build2 (code, void_type_node, unshare_expr (result),
			  TREE_OPERAND (cond, 2));

	    TREE_TYPE (cond) = void_type_node;
	    recalculate_side_effects (cond);

	    if (want_value)
	      {
		gimplify_and_add (cond, pre_p);
		*expr_p = unshare_expr (result);
	      }
	    else
	      *expr_p = cond;
	    return ret;
	  }
	else
	  ret = GS_UNHANDLED;
	break;

      case CALL_EXPR:
	/* For calls that return in memory, give *to_p as the CALL_EXPR's
	   return slot so that we don't generate a temporary.  */
	if (!CALL_EXPR_RETURN_SLOT_OPT (*from_p)
	    && aggregate_value_p (*from_p, *from_p))
	  {
	    bool use_target;

	    if (!(rhs_predicate_for (*to_p))(*from_p))
	      /* If we need a temporary, *to_p isn't accurate.  */
	      use_target = false;
	    else if (TREE_CODE (*to_p) == RESULT_DECL
		     && DECL_NAME (*to_p) == NULL_TREE
		     && needs_to_live_in_memory (*to_p))
	      /* It's OK to use the return slot directly unless it's an NRV. */
	      use_target = true;
	    else if (is_gimple_reg_type (TREE_TYPE (*to_p))
		     || (DECL_P (*to_p) && DECL_REGISTER (*to_p)))
	      /* Don't force regs into memory.  */
	      use_target = false;
	    else if (TREE_CODE (*to_p) == VAR_DECL
		     && DECL_GIMPLE_FORMAL_TEMP_P (*to_p))
	      /* Don't use the original target if it's a formal temp; we
		 don't want to take their addresses.  */
	      use_target = false;
	    else if (TREE_CODE (*expr_p) == INIT_EXPR)
	      /* It's OK to use the target directly if it's being
		 initialized. */
	      use_target = true;
	    else if (!is_gimple_non_addressable (*to_p))
	      /* Don't use the original target if it's already addressable;
		 if its address escapes, and the called function uses the
		 NRV optimization, a conforming program could see *to_p
		 change before the called function returns; see c++/19317.
		 When optimizing, the return_slot pass marks more functions
		 as safe after we have escape info.  */
	      use_target = false;
	    else
	      use_target = true;

	    if (use_target)
	      {
		CALL_EXPR_RETURN_SLOT_OPT (*from_p) = 1;
		mark_addressable (*to_p);
	      }
	  }

	ret = GS_UNHANDLED;
	break;

	/* If we're initializing from a container, push the initialization
	   inside it.  */
      case CLEANUP_POINT_EXPR:
      case BIND_EXPR:
      case STATEMENT_LIST:
	{
	  tree wrap = *from_p;
	  tree t;

	  ret = gimplify_expr (to_p, pre_p, post_p, is_gimple_min_lval,
			       fb_lvalue);
	  if (ret != GS_ERROR)
	    ret = GS_OK;

	  t = voidify_wrapper_expr (wrap, *expr_p);
	  gcc_assert (t == *expr_p);

	  if (want_value)
	    {
	      gimplify_and_add (wrap, pre_p);
	      *expr_p = unshare_expr (*to_p);
	    }
	  else
	    *expr_p = wrap;
	  return GS_OK;
	}
	
      default:
	ret = GS_UNHANDLED;
	break;
      }

  return ret;
}

/* Destructively convert the TREE pointer in TP into a gimple tuple if
   appropriate.  */

static void
tree_to_gimple_tuple (tree *tp)
{

  switch (TREE_CODE (*tp))
    {
    case GIMPLE_MODIFY_STMT:
      return;
    case MODIFY_EXPR:
      {
        struct gimple_stmt *gs;
	tree lhs = TREE_OPERAND (*tp, 0);
	bool def_stmt_self_p = false;

	if (TREE_CODE (lhs) == SSA_NAME)
	  {
	    if (SSA_NAME_DEF_STMT (lhs) == *tp)
	      def_stmt_self_p = true;
	  }

        gs = &make_node (GIMPLE_MODIFY_STMT)->gstmt;
        gs->base = (*tp)->base;
        /* The set to base above overwrites the CODE.  */
        TREE_SET_CODE ((tree) gs, GIMPLE_MODIFY_STMT);

        gs->locus = EXPR_LOCUS (*tp);
        gs->operands[0] = TREE_OPERAND (*tp, 0);
        gs->operands[1] = TREE_OPERAND (*tp, 1);
        gs->block = TREE_BLOCK (*tp);
        *tp = (tree)gs;

	/* If we re-gimplify a set to an SSA_NAME, we must change the
	   SSA name's DEF_STMT link.  */
	if (def_stmt_self_p)
	  SSA_NAME_DEF_STMT (GIMPLE_STMT_OPERAND (*tp, 0)) = *tp;

        return;
      }
    default:
      break;
    }
}

/* Promote partial stores to COMPLEX variables to total stores.  *EXPR_P is
   a MODIFY_EXPR with a lhs of a REAL/IMAGPART_EXPR of a variable with
   DECL_GIMPLE_REG_P set.  */

static enum gimplify_status
gimplify_modify_expr_complex_part (tree *expr_p, gimple_seq pre_p,
                                   bool want_value)
{
  enum tree_code code, ocode;
  tree lhs, rhs, new_rhs, other, realpart, imagpart;

  lhs = GENERIC_TREE_OPERAND (*expr_p, 0);
  rhs = GENERIC_TREE_OPERAND (*expr_p, 1);
  code = TREE_CODE (lhs);
  lhs = TREE_OPERAND (lhs, 0);

  ocode = code == REALPART_EXPR ? IMAGPART_EXPR : REALPART_EXPR;
  other = build1 (ocode, TREE_TYPE (rhs), lhs);
  other = get_formal_tmp_var (other, pre_p);

  realpart = code == REALPART_EXPR ? rhs : other;
  imagpart = code == REALPART_EXPR ? other : rhs;

  if (TREE_CONSTANT (realpart) && TREE_CONSTANT (imagpart))
    new_rhs = build_complex (TREE_TYPE (lhs), realpart, imagpart);
  else
    new_rhs = build2 (COMPLEX_EXPR, TREE_TYPE (lhs), realpart, imagpart);

  gimple_seq_add (pre_p, build_gimple_assign (lhs, new_rhs));
  *expr_p = (want_value) ? rhs : NULL_TREE;

  return GS_ALL_DONE;
}

/* Gimplify the MODIFY_EXPR node pointed to by EXPR_P.

      modify_expr
	      : varname '=' rhs
	      | '*' ID '=' rhs

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
	*EXPR_P should be stored.

    WANT_VALUE is nonzero iff we want to use the value of this expression
	in another expression.  */

static enum gimplify_status
gimplify_modify_expr (tree *expr_p, gimple_seq pre_p, gimple_seq post_p,
		      bool want_value)
{
  tree *from_p = &GENERIC_TREE_OPERAND (*expr_p, 1);
  tree *to_p = &GENERIC_TREE_OPERAND (*expr_p, 0);
  enum gimplify_status ret = GS_UNHANDLED;

  gcc_assert (TREE_CODE (*expr_p) == MODIFY_EXPR
	      || TREE_CODE (*expr_p) == GIMPLE_MODIFY_STMT
	      || TREE_CODE (*expr_p) == INIT_EXPR);

  /* For zero sized types only gimplify the left hand side and right hand side
     as statements and throw away the assignment.  */
  if (zero_sized_type (TREE_TYPE (*from_p)))
    {
      gimplify_stmt (from_p, pre_p);
      gimplify_stmt (to_p, pre_p);
      *expr_p = NULL_TREE;
      return GS_ALL_DONE;
    }

  /* See if any simplifications can be done based on what the RHS is.  */
  ret = gimplify_modify_expr_rhs (expr_p, from_p, to_p, pre_p, post_p,
				  want_value);
  if (ret != GS_UNHANDLED)
    return ret;

  /* If the value being copied is of variable width, compute the length
     of the copy into a WITH_SIZE_EXPR.   Note that we need to do this
     before gimplifying any of the operands so that we can resolve any
     PLACEHOLDER_EXPRs in the size.  Also note that the RTL expander uses
     the size of the expression to be copied, not of the destination, so
     that is what we must here.  */
  maybe_with_size_expr (from_p);

  ret = gimplify_expr (to_p, pre_p, post_p, is_gimple_lvalue, fb_lvalue);
  if (ret == GS_ERROR)
    return ret;

  ret = gimplify_expr (from_p, pre_p, post_p, rhs_predicate_for (*to_p),
		       fb_rvalue);
  if (ret == GS_ERROR)
    return ret;

  /* Now see if the above changed *from_p to something we handle specially.  */
  ret = gimplify_modify_expr_rhs (expr_p, from_p, to_p, pre_p, post_p,
				  want_value);
  if (ret != GS_UNHANDLED)
    return ret;

  /* If we've got a variable sized assignment between two lvalues (i.e. does
     not involve a call), then we can make things a bit more straightforward
     by converting the assignment to memcpy or memset.  */
  if (TREE_CODE (*from_p) == WITH_SIZE_EXPR)
    {
      tree from = TREE_OPERAND (*from_p, 0);
      tree size = TREE_OPERAND (*from_p, 1);

      if (TREE_CODE (from) == CONSTRUCTOR)
	return gimplify_modify_expr_to_memset (expr_p, size, want_value, pre_p);
      if (is_gimple_addressable (from))
	{
	  *from_p = from;
	  return gimplify_modify_expr_to_memcpy (expr_p, size, want_value,
	      					 pre_p);
	}
    }

  /* Transform partial stores to non-addressable complex variables into
     total stores.  This allows us to use real instead of virtual operands
     for these variables, which improves optimization.  */
  if ((TREE_CODE (*to_p) == REALPART_EXPR
       || TREE_CODE (*to_p) == IMAGPART_EXPR)
      && is_gimple_reg (TREE_OPERAND (*to_p, 0)))
    return gimplify_modify_expr_complex_part (expr_p, pre_p, want_value);

  if (gimplify_ctxp->into_ssa && is_gimple_reg (*to_p))
    {
      /* If we've somehow already got an SSA_NAME on the LHS, then
	 we're probably modified it twice.  Not good.  */
      gcc_assert (TREE_CODE (*to_p) != SSA_NAME);
      *to_p = make_ssa_name (*to_p, *expr_p);
    }

  /* Try to alleviate the effects of the gimplification creating artificial
     temporaries (see for example is_gimple_reg_rhs) on the debug info.  */
  if (!gimplify_ctxp->into_ssa
      && DECL_P (*from_p) && DECL_IGNORED_P (*from_p)
      && DECL_P (*to_p) && !DECL_IGNORED_P (*to_p))
    {
      if (!DECL_NAME (*from_p) && DECL_NAME (*to_p))
	DECL_NAME (*from_p)
	  = create_tmp_var_name (IDENTIFIER_POINTER (DECL_NAME (*to_p)));
      DECL_DEBUG_EXPR_IS_FROM (*from_p) = 1;
      SET_DECL_DEBUG_EXPR (*from_p, *to_p);
    }

  gimple_seq_add (pre_p, build_gimple_assign (*to_p, *from_p));

  if (want_value)
    {
      *expr_p = *to_p;
      return GS_OK;
    }
  else
    *expr_p = NULL;

  return GS_ALL_DONE;
}

/*  Gimplify a comparison between two variable-sized objects.  Do this
    with a call to BUILT_IN_MEMCMP.  */

static enum gimplify_status
gimplify_variable_sized_compare (tree *expr_p)
{
  tree op0 = TREE_OPERAND (*expr_p, 0);
  tree op1 = TREE_OPERAND (*expr_p, 1);
  tree t, arg, dest, src;

  arg = TYPE_SIZE_UNIT (TREE_TYPE (op0));
  arg = unshare_expr (arg);
  arg = SUBSTITUTE_PLACEHOLDER_IN_EXPR (arg, op0);
  src = build_fold_addr_expr (op1);
  dest = build_fold_addr_expr (op0);
  t = implicit_built_in_decls[BUILT_IN_MEMCMP];
  t = build_call_expr (t, 3, dest, src, arg);
  *expr_p
    = build2 (TREE_CODE (*expr_p), TREE_TYPE (*expr_p), t, integer_zero_node);

  return GS_OK;
}

/*  Gimplify a comparison between two aggregate objects of integral scalar
    mode as a comparison between the bitwise equivalent scalar values.  */

static enum gimplify_status
gimplify_scalar_mode_aggregate_compare (tree *expr_p)
{
  tree op0 = TREE_OPERAND (*expr_p, 0);
  tree op1 = TREE_OPERAND (*expr_p, 1);

  tree type = TREE_TYPE (op0);
  tree scalar_type = lang_hooks.types.type_for_mode (TYPE_MODE (type), 1);

  op0 = fold_build1 (VIEW_CONVERT_EXPR, scalar_type, op0);
  op1 = fold_build1 (VIEW_CONVERT_EXPR, scalar_type, op1);

  *expr_p
    = fold_build2 (TREE_CODE (*expr_p), TREE_TYPE (*expr_p), op0, op1);

  return GS_OK;
}

/*  Gimplify TRUTH_ANDIF_EXPR and TRUTH_ORIF_EXPR expressions.  EXPR_P
    points to the expression to gimplify.

    Expressions of the form 'a && b' are gimplified to:

	a && b ? true : false

    gimplify_cond_expr will do the rest.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.  */

static enum gimplify_status
gimplify_boolean_expr (tree *expr_p)
{
  /* Preserve the original type of the expression.  */
  tree type = TREE_TYPE (*expr_p);

  *expr_p = build3 (COND_EXPR, type, *expr_p,
		    fold_convert (type, boolean_true_node),
		    fold_convert (type, boolean_false_node));

  return GS_OK;
}

/* Gimplifies an expression sequence.  This function gimplifies each
   expression and re-writes the original expression with the last
   expression of the sequence in GIMPLE form.

   PRE_P points to the list where the side effects for all the
       expressions in the sequence will be emitted.

   WANT_VALUE is true when the result of the last COMPOUND_EXPR is used.  */

static enum gimplify_status
gimplify_compound_expr (tree *expr_p, gimple_seq pre_p, bool want_value)
{
  tree t = *expr_p;

  do
    {
      tree *sub_p = &TREE_OPERAND (t, 0);

      if (TREE_CODE (*sub_p) == COMPOUND_EXPR)
	gimplify_compound_expr (sub_p, pre_p, false);
      else
	gimplify_stmt (sub_p, pre_p);

      t = TREE_OPERAND (t, 1);
    }
  while (TREE_CODE (t) == COMPOUND_EXPR);

  *expr_p = t;
  if (want_value)
    return GS_OK;
  else
    {
      gimplify_stmt (expr_p, pre_p);
      return GS_ALL_DONE;
    }
}


/* Gimplify a SAVE_EXPR node.  EXPR_P points to the expression to
   gimplify.  After gimplification, EXPR_P will point to a new temporary
   that holds the original value of the SAVE_EXPR node.

   PRE_P points to the list where side effects that must happen before
      *EXPR_P should be stored.  */

static enum gimplify_status
gimplify_save_expr (tree *expr_p, gimple_seq pre_p, gimple_seq post_p)
{
  enum gimplify_status ret = GS_ALL_DONE;
  tree val;

  gcc_assert (TREE_CODE (*expr_p) == SAVE_EXPR);
  val = TREE_OPERAND (*expr_p, 0);

  /* If the SAVE_EXPR has not been resolved, then evaluate it once.  */
  if (!SAVE_EXPR_RESOLVED_P (*expr_p))
    {
      /* The operand may be a void-valued expression such as SAVE_EXPRs
	 generated by the Java frontend for class initialization.  It is
	 being executed only for its side-effects.  */
      if (TREE_TYPE (val) == void_type_node)
	{
	  ret = gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_gimple_stmt, fb_none);
	  val = NULL;
	}
      else
	val = get_initialized_tmp_var (val, pre_p, post_p);

      TREE_OPERAND (*expr_p, 0) = val;
      SAVE_EXPR_RESOLVED_P (*expr_p) = 1;
    }

  *expr_p = val;

  return ret;
}

/*  Re-write the ADDR_EXPR node pointed to by EXPR_P

      unary_expr
	      : ...
	      | '&' varname
	      ...

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
	*EXPR_P should be stored.  */

static enum gimplify_status
gimplify_addr_expr (tree *expr_p, gimple_seq pre_p, gimple_seq post_p)
{
  tree expr = *expr_p;
  tree op0 = TREE_OPERAND (expr, 0);
  enum gimplify_status ret;

  switch (TREE_CODE (op0))
    {
    case INDIRECT_REF:
    case MISALIGNED_INDIRECT_REF:
    do_indirect_ref:
      /* Check if we are dealing with an expression of the form '&*ptr'.
	 While the front end folds away '&*ptr' into 'ptr', these
	 expressions may be generated internally by the compiler (e.g.,
	 builtins like __builtin_va_end).  */
      /* Caution: the silent array decomposition semantics we allow for
	 ADDR_EXPR means we can't always discard the pair.  */
      /* Gimplification of the ADDR_EXPR operand may drop
	 cv-qualification conversions, so make sure we add them if
	 needed.  */
      {
	tree op00 = TREE_OPERAND (op0, 0);
	tree t_expr = TREE_TYPE (expr);
	tree t_op00 = TREE_TYPE (op00);

        if (!useless_type_conversion_p (t_expr, t_op00))
	  op00 = fold_convert (TREE_TYPE (expr), op00);
        *expr_p = op00;
        ret = GS_OK;
      }
      break;

    case VIEW_CONVERT_EXPR:
      /* Take the address of our operand and then convert it to the type of
	 this ADDR_EXPR.

	 ??? The interactions of VIEW_CONVERT_EXPR and aliasing is not at
	 all clear.  The impact of this transformation is even less clear.  */

      /* If the operand is a useless conversion, look through it.  Doing so
	 guarantees that the ADDR_EXPR and its operand will remain of the
	 same type.  */
      if (tree_ssa_useless_type_conversion (TREE_OPERAND (op0, 0)))
	op0 = TREE_OPERAND (op0, 0);

      *expr_p = fold_convert (TREE_TYPE (expr),
			      build_fold_addr_expr (TREE_OPERAND (op0, 0)));
      ret = GS_OK;
      break;

    default:
      /* We use fb_either here because the C frontend sometimes takes
	 the address of a call that returns a struct; see
	 gcc.dg/c99-array-lval-1.c.  The gimplifier will correctly make
	 the implied temporary explicit.  */

      /* Mark the RHS addressable.  */
      ret = gimplify_expr (&TREE_OPERAND (expr, 0), pre_p, post_p,
			   is_gimple_addressable, fb_either);
      if (ret != GS_ERROR)
	{
	  op0 = TREE_OPERAND (expr, 0);

	  /* For various reasons, the gimplification of the expression
	     may have made a new INDIRECT_REF.  */
	  if (TREE_CODE (op0) == INDIRECT_REF)
	    goto do_indirect_ref;

	  /* Make sure TREE_INVARIANT, TREE_CONSTANT, and TREE_SIDE_EFFECTS
	     is set properly.  */
	  recompute_tree_invariant_for_addr_expr (expr);

	  mark_addressable (TREE_OPERAND (expr, 0));
	}
      break;
    }

  return ret;
}

/* Gimplify the operands of an ASM_EXPR.  Input operands should be a gimple
   value; output operands should be a gimple lvalue.  */

static enum gimplify_status
gimplify_asm_expr (tree *expr_p, gimple_seq pre_p, gimple_seq post_p)
{
  tree expr;
  int noutputs;
  const char **oconstraints;
  int i;
  tree link;
  const char *constraint;
  bool allows_mem, allows_reg, is_inout;
  enum gimplify_status ret, tret;
  gimple stmt;
  VEC(tree, gc) *inputs;
  VEC(tree, gc) *outputs;
  VEC(tree, gc) *clobbers;
  
  expr = *expr_p;
  noutputs = list_length (ASM_OUTPUTS (expr));
  oconstraints = (const char **) alloca ((noutputs) * sizeof (const char *));

  inputs = VEC_alloc (tree, gc, 5);
  outputs = VEC_alloc (tree, gc, 5);
  clobbers = VEC_alloc (tree, gc, 5);
  
  ret = GS_ALL_DONE;
  for (i = 0, link = ASM_OUTPUTS (expr); link; ++i, link = TREE_CHAIN (link))
    {
      size_t constraint_len;

      oconstraints[i]
	= constraint
	= TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      constraint_len = strlen (constraint);
      if (constraint_len == 0)
        continue;

      parse_output_constraint (&constraint, i, 0, 0, &allows_mem, &allows_reg,
	                       &is_inout);

      if (!allows_reg && allows_mem)
	mark_addressable (TREE_VALUE (link));

      tret = gimplify_expr (&TREE_VALUE (link), pre_p, post_p,
			    is_inout ? is_gimple_min_lval : is_gimple_lvalue,
			    fb_lvalue | fb_mayfail);
      if (tret == GS_ERROR)
	{
	  error ("invalid lvalue in asm output %d", i);
	  ret = tret;
	}

      VEC_safe_push (tree, gc, outputs, link);

      if (is_inout)
	{
	  /* An input/output operand.  To give the optimizers more
	     flexibility, split it into separate input and output
 	     operands.  */
	  tree input;
	  char buf[10];

	  /* Turn the in/out constraint into an output constraint.  */
	  char *p = xstrdup (constraint);
	  p[0] = '=';
	  TREE_VALUE (TREE_PURPOSE (link)) = build_string (constraint_len, p);

	  /* And add a matching input constraint.  */
	  if (allows_reg)
	    {
	      sprintf (buf, "%d", i);

	      /* If there are multiple alternatives in the constraint,
		 handle each of them individually.  Those that allow register
		 will be replaced with operand number, the others will stay
		 unchanged.  */
	      if (strchr (p, ',') != NULL)
		{
		  size_t len = 0, buflen = strlen (buf);
		  char *beg, *end, *str, *dst;

		  for (beg = p + 1;;)
		    {
		      end = strchr (beg, ',');
		      if (end == NULL)
			end = strchr (beg, '\0');
		      if ((size_t) (end - beg) < buflen)
			len += buflen + 1;
		      else
			len += end - beg + 1;
		      if (*end)
			beg = end + 1;
		      else
			break;
		    }

		  str = (char *) alloca (len);
		  for (beg = p + 1, dst = str;;)
		    {
		      const char *tem;
		      bool mem_p, reg_p, inout_p;

		      end = strchr (beg, ',');
		      if (end)
			*end = '\0';
		      beg[-1] = '=';
		      tem = beg - 1;
		      parse_output_constraint (&tem, i, 0, 0,
					       &mem_p, &reg_p, &inout_p);
		      if (dst != str)
			*dst++ = ',';
		      if (reg_p)
			{
			  memcpy (dst, buf, buflen);
			  dst += buflen;
			}
		      else
			{
			  if (end)
			    len = end - beg;
			  else
			    len = strlen (beg);
			  memcpy (dst, beg, len);
			  dst += len;
			}
		      if (end)
			beg = end + 1;
		      else
			break;
		    }
		  *dst = '\0';
		  input = build_string (dst - str, str);
		}
	      else
		input = build_string (strlen (buf), buf);
	    }
	  else
	    input = build_string (constraint_len - 1, constraint + 1);

	  free (p);

	  input = build_tree_list (build_tree_list (NULL_TREE, input),
				   unshare_expr (TREE_VALUE (link)));
	  ASM_INPUTS (expr) = chainon (ASM_INPUTS (expr), input);
	}
    }

  for (link = ASM_INPUTS (expr); link; ++i, link = TREE_CHAIN (link))
    {
      constraint
	= TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      parse_input_constraint (&constraint, 0, 0, noutputs, 0,
			      oconstraints, &allows_mem, &allows_reg);

      /* If we can't make copies, we can only accept memory.  */
      if (TREE_ADDRESSABLE (TREE_TYPE (TREE_VALUE (link))))
	{
	  if (allows_mem)
	    allows_reg = 0;
	  else
	    {
	      error ("impossible constraint in %<asm%>");
	      error ("non-memory input %d must stay in memory", i);
	      return GS_ERROR;
	    }
	}

      /* If the operand is a memory input, it should be an lvalue.  */
      if (!allows_reg && allows_mem)
	{
	  tret = gimplify_expr (&TREE_VALUE (link), pre_p, post_p,
				is_gimple_lvalue, fb_lvalue | fb_mayfail);
	  mark_addressable (TREE_VALUE (link));
	  if (tret == GS_ERROR)
	    {
	      error ("memory input %d is not directly addressable", i);
	      ret = tret;
	    }
	}
      else
	{
	  tret = gimplify_expr (&TREE_VALUE (link), pre_p, post_p,
				is_gimple_asm_val, fb_rvalue);
	  if (tret == GS_ERROR)
	    ret = tret;
	}

      VEC_safe_push (tree, gc, inputs, link);
    }
  
  for (link = ASM_CLOBBERS (expr); link; ++i, link = TREE_CHAIN (link))
      VEC_safe_push (tree, gc, clobbers, link);
    
  stmt = build_gimple_asm_vec (TREE_STRING_POINTER (ASM_STRING(expr)),
                               inputs, outputs, clobbers);
  gimple_seq_add (pre_p, stmt);
  return ret;
}

/* Gimplify a CLEANUP_POINT_EXPR.  Currently this works by adding
   WITH_CLEANUP_EXPRs to the prequeue as we encounter cleanups while
   gimplifying the body, and converting them to TRY_FINALLY_EXPRs when we
   return to this function.

   FIXME should we complexify the prequeue handling instead?  Or use flags
   for all the cleanups and let the optimizer tighten them up?  The current
   code seems pretty fragile; it will break on a cleanup within any
   non-conditional nesting.  But any such nesting would be broken, anyway;
   we can't write a TRY_FINALLY_EXPR that starts inside a nesting construct
   and continues out of it.  We can do that at the RTL level, though, so
   having an optimizer to tighten up try/finally regions would be a Good
   Thing.  */

#if 0
/* FIXME tuples */
static enum gimplify_status
gimplify_cleanup_point_expr (tree *expr_p, tree *pre_p)
{
  tree_stmt_iterator iter;
  tree body;

  tree temp = voidify_wrapper_expr (*expr_p, NULL);

  /* We only care about the number of conditions between the innermost
     CLEANUP_POINT_EXPR and the cleanup.  So save and reset the count and
     any cleanups collected outside the CLEANUP_POINT_EXPR.  */
  int old_conds = gimplify_ctxp->conditions;
  struct gimple_sequence old_cleanups = gimplify_ctxp->conditional_cleanups;
  gimplify_ctxp->conditions = 0;
  gimple_seq_init (&gimplify_ctxp->conditional_cleanups);

  body = TREE_OPERAND (*expr_p, 0);
  gimplify_stmt (&body, pre_p);

  gimplify_ctxp->conditions = old_conds;
  gimplify_ctxp->conditional_cleanups = old_cleanups;

  for (iter = tsi_start (body); !tsi_end_p (iter); )
    {
      tree *wce_p = tsi_stmt_ptr (iter);
      tree wce = *wce_p;

      if (TREE_CODE (wce) == WITH_CLEANUP_EXPR)
	{
	  if (tsi_one_before_end_p (iter))
	    {
	      tsi_link_before (&iter, TREE_OPERAND (wce, 0), TSI_SAME_STMT);
	      tsi_delink (&iter);
	      break;
	    }
	  else
	    {
	      tree sl, tfe;
	      enum tree_code code;

	      if (CLEANUP_EH_ONLY (wce))
		code = TRY_CATCH_EXPR;
	      else
		code = TRY_FINALLY_EXPR;

	      sl = tsi_split_statement_list_after (&iter);
	      tfe = build2 (code, void_type_node, sl, NULL_TREE);
	      append_to_statement_list (TREE_OPERAND (wce, 0),
				        &TREE_OPERAND (tfe, 1));
	      *wce_p = tfe;
	      iter = tsi_start (sl);
	    }
	}
      else
	tsi_next (&iter);
    }

  if (temp)
    {
      *expr_p = temp;
      append_to_statement_list (body, pre_p);
      return GS_OK;
    }
  else
    {
      *expr_p = body;
      return GS_ALL_DONE;
    }
}
#endif

/* Insert a cleanup marker for gimplify_cleanup_point_expr.  CLEANUP
   is the cleanup action required.  */

static void
gimple_push_cleanup (tree var ATTRIBUTE_UNUSED, tree cleanup ATTRIBUTE_UNUSED, bool eh_only ATTRIBUTE_UNUSED, gimple_seq pre_p ATTRIBUTE_UNUSED)
{
/* FIXME tuples */
#if 0 /* FIXME tuples */
  tree wce;

  /* Errors can result in improperly nested cleanups.  Which results in
     confusion when trying to resolve the WITH_CLEANUP_EXPR.  */
  if (errorcount || sorrycount)
    return;

  if (gimple_conditional_context ())
    {
      /* If we're in a conditional context, this is more complex.  We only
	 want to run the cleanup if we actually ran the initialization that
	 necessitates it, but we want to run it after the end of the
	 conditional context.  So we wrap the try/finally around the
	 condition and use a flag to determine whether or not to actually
	 run the destructor.  Thus

	   test ? f(A()) : 0

	 becomes (approximately)

	   flag = 0;
	   try {
	     if (test) { A::A(temp); flag = 1; val = f(temp); }
	     else { val = 0; }
	   } finally {
	     if (flag) A::~A(temp);
	   }
	   val
      */

      tree flag = create_tmp_var (boolean_type_node, "cleanup");
      gimple ffalse = build_gimple_assign (flag, boolean_false_node);
      gimple ftrue = build_gimple_assign (flag, boolean_true_node);
      cleanup = build3 (COND_EXPR, void_type_node, flag, cleanup, NULL);
      wce = build1 (WITH_CLEANUP_EXPR, void_type_node, cleanup);
      gimple_seq_add (&gimplify_ctxp->conditional_cleanups, ffalse);
      gimple_seq_append (&gimplify_ctxp->conditional_cleanups, wce);
      gimple_seq_add (pre_p, ftrue);

      /* Because of this manipulation, and the EH edges that jump
	 threading cannot redirect, the temporary (VAR) will appear
	 to be used uninitialized.  Don't warn.  */
      TREE_NO_WARNING (var) = 1;
    }
  else
    {
      wce = build1 (WITH_CLEANUP_EXPR, void_type_node, cleanup);
      CLEANUP_EH_ONLY (wce) = eh_only;
      append_to_statement_list (wce, pre_p);
    }

  gimplify_stmt (&TREE_OPERAND (wce, 0), pre_p);
#endif
}

/* Gimplify a TARGET_EXPR which doesn't appear on the rhs of an INIT_EXPR.  */

static enum gimplify_status
gimplify_target_expr (tree *expr_p, gimple_seq pre_p, gimple_seq post_p)
{
  tree targ = *expr_p;
  tree temp = TARGET_EXPR_SLOT (targ);
  tree init = TARGET_EXPR_INITIAL (targ);
  enum gimplify_status ret;

  if (init)
    {
      /* TARGET_EXPR temps aren't part of the enclosing block, so add it
	 to the temps list.  */
      gimple_add_tmp_var (temp);

      /* If TARGET_EXPR_INITIAL is void, then the mere evaluation of the
	 expression is supposed to initialize the slot.  */
      if (VOID_TYPE_P (TREE_TYPE (init)))
	ret = gimplify_expr (&init, pre_p, post_p, is_gimple_stmt, fb_none);
      else
	{
	  init = build2 (INIT_EXPR, void_type_node, temp, init);
	  ret = gimplify_expr (&init, pre_p, post_p, is_gimple_stmt, fb_none);
	  init = NULL;
	}
      if (ret == GS_ERROR)
	{
	  /* PR c++/28266 Make sure this is expanded only once. */
	  TARGET_EXPR_INITIAL (targ) = NULL_TREE;
	  return GS_ERROR;
	}
      if (init)
	gimplify_and_add (init, pre_p);

      /* If needed, push the cleanup for the temp.  */
      if (TARGET_EXPR_CLEANUP (targ))
	{
	  gimplify_stmt (&TARGET_EXPR_CLEANUP (targ), pre_p);
	  gimple_push_cleanup (temp, TARGET_EXPR_CLEANUP (targ),
			       CLEANUP_EH_ONLY (targ), pre_p);
	}

      /* Only expand this once.  */
      TREE_OPERAND (targ, 3) = init;
      TARGET_EXPR_INITIAL (targ) = NULL_TREE;
    }
  else
    /* We should have expanded this before.  */
    gcc_assert (DECL_SEEN_IN_BIND_EXPR_P (temp));

  *expr_p = temp;
  return GS_OK;
}

/* Gimplification of expression trees.  */

/* Gimplify an expression which appears at statement context.  The
   corresponding GIMPLE statements are added to SEQ_P.  Note that the
   sequence is not initialized.  It is the caller's responsibility to
   initialize it, if required. 

   Return true if we actually added a statement to the queue.  */

bool
gimplify_stmt (tree *stmt_p, gimple_seq seq_p)
{
  gimple last = gimple_seq_last (seq_p);
  gimplify_expr (stmt_p, seq_p, NULL, is_gimple_stmt, fb_none);
  return last != gimple_seq_last (seq_p);
}


/* Add FIRSTPRIVATE entries for DECL in the OpenMP the surrounding parallels
   to CTX.  If entries already exist, force them to be some flavor of private.
   If there is no enclosing parallel, do nothing.  */

void
omp_firstprivatize_variable (struct gimplify_omp_ctx *ctx, tree decl)
{
  splay_tree_node n;

  if (decl == NULL || !DECL_P (decl))
    return;

  do
    {
      n = splay_tree_lookup (ctx->variables, (splay_tree_key)decl);
      if (n != NULL)
	{
	  if (n->value & GOVD_SHARED)
	    n->value = GOVD_FIRSTPRIVATE | (n->value & GOVD_SEEN);
	  else
	    return;
	}
      else if (ctx->is_parallel)
	omp_add_variable (ctx, decl, GOVD_FIRSTPRIVATE);

      ctx = ctx->outer_context;
    }
  while (ctx);
}

/* Similarly for each of the type sizes of TYPE.  */

static void
omp_firstprivatize_type_sizes (struct gimplify_omp_ctx *ctx, tree type)
{
  if (type == NULL || type == error_mark_node)
    return;
  type = TYPE_MAIN_VARIANT (type);

  if (pointer_set_insert (ctx->privatized_types, type))
    return;

  switch (TREE_CODE (type))
    {
    case INTEGER_TYPE:
    case ENUMERAL_TYPE:
    case BOOLEAN_TYPE:
    case REAL_TYPE:
    case FIXED_POINT_TYPE:
      omp_firstprivatize_variable (ctx, TYPE_MIN_VALUE (type));
      omp_firstprivatize_variable (ctx, TYPE_MAX_VALUE (type));
      break;

    case ARRAY_TYPE:
      omp_firstprivatize_type_sizes (ctx, TREE_TYPE (type));
      omp_firstprivatize_type_sizes (ctx, TYPE_DOMAIN (type));
      break;

    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      {
	tree field;
	for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
	  if (TREE_CODE (field) == FIELD_DECL)
	    {
	      omp_firstprivatize_variable (ctx, DECL_FIELD_OFFSET (field));
	      omp_firstprivatize_type_sizes (ctx, TREE_TYPE (field));
	    }
      }
      break;

    case POINTER_TYPE:
    case REFERENCE_TYPE:
      omp_firstprivatize_type_sizes (ctx, TREE_TYPE (type));
      break;

    default:
      break;
    }

  omp_firstprivatize_variable (ctx, TYPE_SIZE (type));
  omp_firstprivatize_variable (ctx, TYPE_SIZE_UNIT (type));
  lang_hooks.types.omp_firstprivatize_type_sizes (ctx, type);
}

/* Add an entry for DECL in the OpenMP context CTX with FLAGS.  */

static void
omp_add_variable (struct gimplify_omp_ctx *ctx, tree decl, unsigned int flags)
{
  splay_tree_node n;
  unsigned int nflags;
  tree t;

  if (decl == error_mark_node || TREE_TYPE (decl) == error_mark_node)
    return;

  /* Never elide decls whose type has TREE_ADDRESSABLE set.  This means
     there are constructors involved somewhere.  */
  if (TREE_ADDRESSABLE (TREE_TYPE (decl))
      || TYPE_NEEDS_CONSTRUCTING (TREE_TYPE (decl)))
    flags |= GOVD_SEEN;

  n = splay_tree_lookup (ctx->variables, (splay_tree_key)decl);
  if (n != NULL)
    {
      /* We shouldn't be re-adding the decl with the same data
	 sharing class.  */
      gcc_assert ((n->value & GOVD_DATA_SHARE_CLASS & flags) == 0);
      /* The only combination of data sharing classes we should see is
	 FIRSTPRIVATE and LASTPRIVATE.  */
      nflags = n->value | flags;
      gcc_assert ((nflags & GOVD_DATA_SHARE_CLASS)
		  == (GOVD_FIRSTPRIVATE | GOVD_LASTPRIVATE));
      n->value = nflags;
      return;
    }

  /* When adding a variable-sized variable, we have to handle all sorts
     of additional bits of data: the pointer replacement variable, and 
     the parameters of the type.  */
  if (DECL_SIZE (decl) && TREE_CODE (DECL_SIZE (decl)) != INTEGER_CST)
    {
      /* Add the pointer replacement variable as PRIVATE if the variable
	 replacement is private, else FIRSTPRIVATE since we'll need the
	 address of the original variable either for SHARED, or for the
	 copy into or out of the context.  */
      if (!(flags & GOVD_LOCAL))
	{
	  nflags = flags & GOVD_PRIVATE ? GOVD_PRIVATE : GOVD_FIRSTPRIVATE;
	  nflags |= flags & GOVD_SEEN;
	  t = DECL_VALUE_EXPR (decl);
	  gcc_assert (TREE_CODE (t) == INDIRECT_REF);
	  t = TREE_OPERAND (t, 0);
	  gcc_assert (DECL_P (t));
	  omp_add_variable (ctx, t, nflags);
	}

      /* Add all of the variable and type parameters (which should have
	 been gimplified to a formal temporary) as FIRSTPRIVATE.  */
      omp_firstprivatize_variable (ctx, DECL_SIZE_UNIT (decl));
      omp_firstprivatize_variable (ctx, DECL_SIZE (decl));
      omp_firstprivatize_type_sizes (ctx, TREE_TYPE (decl));

      /* The variable-sized variable itself is never SHARED, only some form
	 of PRIVATE.  The sharing would take place via the pointer variable
	 which we remapped above.  */
      if (flags & GOVD_SHARED)
	flags = GOVD_PRIVATE | GOVD_DEBUG_PRIVATE
		| (flags & (GOVD_SEEN | GOVD_EXPLICIT));

      /* We're going to make use of the TYPE_SIZE_UNIT at least in the 
	 alloca statement we generate for the variable, so make sure it
	 is available.  This isn't automatically needed for the SHARED
	 case, since we won't be allocating local storage then.
	 For local variables TYPE_SIZE_UNIT might not be gimplified yet,
	 in this case omp_notice_variable will be called later
	 on when it is gimplified.  */
      else if (! (flags & GOVD_LOCAL))
	omp_notice_variable (ctx, TYPE_SIZE_UNIT (TREE_TYPE (decl)), true);
    }
  else if (lang_hooks.decls.omp_privatize_by_reference (decl))
    {
      gcc_assert ((flags & GOVD_LOCAL) == 0);
      omp_firstprivatize_type_sizes (ctx, TREE_TYPE (decl));

      /* Similar to the direct variable sized case above, we'll need the
	 size of references being privatized.  */
      if ((flags & GOVD_SHARED) == 0)
	{
	  t = TYPE_SIZE_UNIT (TREE_TYPE (TREE_TYPE (decl)));
	  if (TREE_CODE (t) != INTEGER_CST)
	    omp_notice_variable (ctx, t, true);
	}
    }

  splay_tree_insert (ctx->variables, (splay_tree_key)decl, flags);
}

/* Record the fact that DECL was used within the OpenMP context CTX.
   IN_CODE is true when real code uses DECL, and false when we should
   merely emit default(none) errors.  Return true if DECL is going to
   be remapped and thus DECL shouldn't be gimplified into its
   DECL_VALUE_EXPR (if any).  */

static bool
omp_notice_variable (struct gimplify_omp_ctx *ctx, tree decl, bool in_code)
{
  splay_tree_node n;
  unsigned flags = in_code ? GOVD_SEEN : 0;
  bool ret = false, shared;

  if (decl == error_mark_node || TREE_TYPE (decl) == error_mark_node)
    return false;

  /* Threadprivate variables are predetermined.  */
  if (is_global_var (decl))
    {
      if (DECL_THREAD_LOCAL_P (decl))
	return false;

      if (DECL_HAS_VALUE_EXPR_P (decl))
	{
	  tree value = get_base_address (DECL_VALUE_EXPR (decl));

	  if (value && DECL_P (value) && DECL_THREAD_LOCAL_P (value))
	    return false;
	}
    }

  n = splay_tree_lookup (ctx->variables, (splay_tree_key)decl);
  if (n == NULL)
    {
      enum omp_clause_default_kind default_kind, kind;

      if (!ctx->is_parallel)
	goto do_outer;

      /* ??? Some compiler-generated variables (like SAVE_EXPRs) could be
	 remapped firstprivate instead of shared.  To some extent this is
	 addressed in omp_firstprivatize_type_sizes, but not effectively.  */
      default_kind = ctx->default_kind;
      kind = lang_hooks.decls.omp_predetermined_sharing (decl);
      if (kind != OMP_CLAUSE_DEFAULT_UNSPECIFIED)
	default_kind = kind;

      switch (default_kind)
	{
	case OMP_CLAUSE_DEFAULT_NONE:
	  error ("%qs not specified in enclosing parallel",
		 IDENTIFIER_POINTER (DECL_NAME (decl)));
	  error ("%Henclosing parallel", &ctx->location);
	  /* FALLTHRU */
	case OMP_CLAUSE_DEFAULT_SHARED:
	  flags |= GOVD_SHARED;
	  break;
	case OMP_CLAUSE_DEFAULT_PRIVATE:
	  flags |= GOVD_PRIVATE;
	  break;
	default:
	  gcc_unreachable ();
	}

      omp_add_variable (ctx, decl, flags);

      shared = (flags & GOVD_SHARED) != 0;
      ret = lang_hooks.decls.omp_disregard_value_expr (decl, shared);
      goto do_outer;
    }

  shared = ((flags | n->value) & GOVD_SHARED) != 0;
  ret = lang_hooks.decls.omp_disregard_value_expr (decl, shared);

  /* If nothing changed, there's nothing left to do.  */
  if ((n->value & flags) == flags)
    return ret;
  flags |= n->value;
  n->value = flags;

 do_outer:
  /* If the variable is private in the current context, then we don't
     need to propagate anything to an outer context.  */
  if (flags & GOVD_PRIVATE)
    return ret;
  if (ctx->outer_context
      && omp_notice_variable (ctx->outer_context, decl, in_code))
    return true;
  return ret;
}

/* Verify that DECL is private within CTX.  If there's specific information
   to the contrary in the innermost scope, generate an error.  */

static bool
omp_is_private (struct gimplify_omp_ctx *ctx, tree decl)
{
  splay_tree_node n;

  n = splay_tree_lookup (ctx->variables, (splay_tree_key)decl);
  if (n != NULL)
    {
      if (n->value & GOVD_SHARED)
	{
	  if (ctx == gimplify_omp_ctxp)
	    {
	      error ("iteration variable %qs should be private",
		     IDENTIFIER_POINTER (DECL_NAME (decl)));
	      n->value = GOVD_PRIVATE;
	      return true;
	    }
	  else
	    return false;
	}
      else if ((n->value & GOVD_EXPLICIT) != 0
	       && (ctx == gimplify_omp_ctxp
		   || (ctx->is_combined_parallel
		       && gimplify_omp_ctxp->outer_context == ctx)))
	{
	  if ((n->value & GOVD_FIRSTPRIVATE) != 0)
	    error ("iteration variable %qs should not be firstprivate",
		   IDENTIFIER_POINTER (DECL_NAME (decl)));
	  else if ((n->value & GOVD_REDUCTION) != 0)
	    error ("iteration variable %qs should not be reduction",
		   IDENTIFIER_POINTER (DECL_NAME (decl)));
	}
      return true;
    }

  if (ctx->is_parallel)
    return false;
  else if (ctx->outer_context)
    return omp_is_private (ctx->outer_context, decl);
  else
    return !is_global_var (decl);
}

/* Return true if DECL is private within a parallel region
   that binds to the current construct's context or in parallel
   region's REDUCTION clause.  */

static bool
omp_check_private (struct gimplify_omp_ctx *ctx, tree decl)
{
  splay_tree_node n;

  do
    {
      ctx = ctx->outer_context;
      if (ctx == NULL)
	return !(is_global_var (decl)
		 /* References might be private, but might be shared too.  */
		 || lang_hooks.decls.omp_privatize_by_reference (decl));

      n = splay_tree_lookup (ctx->variables, (splay_tree_key) decl);
      if (n != NULL)
	return (n->value & GOVD_SHARED) == 0;
    }
  while (!ctx->is_parallel);
  return false;
}

/* Scan the OpenMP clauses in *LIST_P, installing mappings into a new
   and previous omp contexts.  */

static void
gimplify_scan_omp_clauses (tree *list_p, gimple_seq pre_p, bool in_parallel,
			   bool in_combined_parallel)
{
  struct gimplify_omp_ctx *ctx, *outer_ctx;
  tree c;

  ctx = new_omp_context (in_parallel, in_combined_parallel);
  outer_ctx = ctx->outer_context;

  while ((c = *list_p) != NULL)
    {
      enum gimplify_status gs;
      bool remove = false;
      bool notice_outer = true;
      const char *check_non_private = NULL;
      unsigned int flags;
      tree decl;

      switch (OMP_CLAUSE_CODE (c))
	{
	case OMP_CLAUSE_PRIVATE:
	  flags = GOVD_PRIVATE | GOVD_EXPLICIT;
	  notice_outer = false;
	  goto do_add;
	case OMP_CLAUSE_SHARED:
	  flags = GOVD_SHARED | GOVD_EXPLICIT;
	  goto do_add;
	case OMP_CLAUSE_FIRSTPRIVATE:
	  flags = GOVD_FIRSTPRIVATE | GOVD_EXPLICIT;
	  check_non_private = "firstprivate";
	  goto do_add;
	case OMP_CLAUSE_LASTPRIVATE:
	  flags = GOVD_LASTPRIVATE | GOVD_SEEN | GOVD_EXPLICIT;
	  check_non_private = "lastprivate";
	  goto do_add;
	case OMP_CLAUSE_REDUCTION:
	  flags = GOVD_REDUCTION | GOVD_SEEN | GOVD_EXPLICIT;
	  check_non_private = "reduction";
	  goto do_add;

	do_add:
	  decl = OMP_CLAUSE_DECL (c);
	  if (decl == error_mark_node || TREE_TYPE (decl) == error_mark_node)
	    {
	      remove = true;
	      break;
	    }
	  omp_add_variable (ctx, decl, flags);
	  if (OMP_CLAUSE_CODE (c) == OMP_CLAUSE_REDUCTION
	      && OMP_CLAUSE_REDUCTION_PLACEHOLDER (c))
	    {
	      omp_add_variable (ctx, OMP_CLAUSE_REDUCTION_PLACEHOLDER (c),
				GOVD_LOCAL | GOVD_SEEN);
	      gimplify_omp_ctxp = ctx;
	      push_gimplify_context ();
	      gimplify_stmt (&OMP_CLAUSE_REDUCTION_INIT (c), pre_p);
	      /* FIXME tuples.  */
#if 0
	      pop_gimplify_context (OMP_CLAUSE_REDUCTION_INIT (c));
	      push_gimplify_context ();
	      gimplify_stmt (&OMP_CLAUSE_REDUCTION_MERGE (c), pre_p);
	      pop_gimplify_context (OMP_CLAUSE_REDUCTION_MERGE (c));
#else
	      gcc_unreachable ();
#endif
	      gimplify_omp_ctxp = outer_ctx;
	    }
	  if (notice_outer)
	    goto do_notice;
	  break;

	case OMP_CLAUSE_COPYIN:
	case OMP_CLAUSE_COPYPRIVATE:
	  decl = OMP_CLAUSE_DECL (c);
	  if (decl == error_mark_node || TREE_TYPE (decl) == error_mark_node)
	    {
	      remove = true;
	      break;
	    }
	do_notice:
	  if (outer_ctx)
	    omp_notice_variable (outer_ctx, decl, true);
	  if (check_non_private
	      && !in_parallel
	      && omp_check_private (ctx, decl))
	    {
	      error ("%s variable %qs is private in outer context",
		     check_non_private, IDENTIFIER_POINTER (DECL_NAME (decl)));
	      remove = true;
	    }
	  break;

	case OMP_CLAUSE_IF:
	  OMP_CLAUSE_OPERAND (c, 0)
	    = gimple_boolify (OMP_CLAUSE_OPERAND (c, 0));
	  /* Fall through.  */

	case OMP_CLAUSE_SCHEDULE:
	case OMP_CLAUSE_NUM_THREADS:
	  gs = gimplify_expr (&OMP_CLAUSE_OPERAND (c, 0), pre_p, NULL,
			      is_gimple_val, fb_rvalue);
	  if (gs == GS_ERROR)
	    remove = true;
	  break;

	case OMP_CLAUSE_NOWAIT:
	case OMP_CLAUSE_ORDERED:
	  break;

	case OMP_CLAUSE_DEFAULT:
	  ctx->default_kind = OMP_CLAUSE_DEFAULT_KIND (c);
	  break;

	default:
	  gcc_unreachable ();
	}

      if (remove)
	*list_p = OMP_CLAUSE_CHAIN (c);
      else
	list_p = &OMP_CLAUSE_CHAIN (c);
    }

  gimplify_omp_ctxp = ctx;
}

/* For all variables that were not actually used within the context,
   remove PRIVATE, SHARED, and FIRSTPRIVATE clauses.  */

static int
gimplify_adjust_omp_clauses_1 (splay_tree_node n, void *data)
{
  tree *list_p = (tree *) data;
  tree decl = (tree) n->key;
  unsigned flags = n->value;
  enum omp_clause_code code;
  tree clause;
  bool private_debug;

  if (flags & (GOVD_EXPLICIT | GOVD_LOCAL))
    return 0;
  if ((flags & GOVD_SEEN) == 0)
    return 0;
  if (flags & GOVD_DEBUG_PRIVATE)
    {
      gcc_assert ((flags & GOVD_DATA_SHARE_CLASS) == GOVD_PRIVATE);
      private_debug = true;
    }
  else
    private_debug
      = lang_hooks.decls.omp_private_debug_clause (decl,
						   !!(flags & GOVD_SHARED));
  if (private_debug)
    code = OMP_CLAUSE_PRIVATE;
  else if (flags & GOVD_SHARED)
    {
      if (is_global_var (decl))
	{
	  struct gimplify_omp_ctx *ctx = gimplify_omp_ctxp->outer_context;
	  while (ctx != NULL)
	    {
	      splay_tree_node on
		= splay_tree_lookup (ctx->variables, (splay_tree_key) decl);
	      if (on && (on->value & (GOVD_FIRSTPRIVATE | GOVD_LASTPRIVATE
				      | GOVD_PRIVATE | GOVD_REDUCTION)) != 0)
		break;
	      ctx = ctx->outer_context;
	    }
	  if (ctx == NULL)
	    return 0;
	}
      code = OMP_CLAUSE_SHARED;
    }
  else if (flags & GOVD_PRIVATE)
    code = OMP_CLAUSE_PRIVATE;
  else if (flags & GOVD_FIRSTPRIVATE)
    code = OMP_CLAUSE_FIRSTPRIVATE;
  else
    gcc_unreachable ();

  clause = build_omp_clause (code);
  OMP_CLAUSE_DECL (clause) = decl;
  OMP_CLAUSE_CHAIN (clause) = *list_p;
  if (private_debug)
    OMP_CLAUSE_PRIVATE_DEBUG (clause) = 1;
  *list_p = clause;

  return 0;
}

static void
gimplify_adjust_omp_clauses (tree *list_p)
{
  struct gimplify_omp_ctx *ctx = gimplify_omp_ctxp;
  tree c, decl;

  while ((c = *list_p) != NULL)
    {
      splay_tree_node n;
      bool remove = false;

      switch (OMP_CLAUSE_CODE (c))
	{
	case OMP_CLAUSE_PRIVATE:
	case OMP_CLAUSE_SHARED:
	case OMP_CLAUSE_FIRSTPRIVATE:
	  decl = OMP_CLAUSE_DECL (c);
	  n = splay_tree_lookup (ctx->variables, (splay_tree_key) decl);
	  remove = !(n->value & GOVD_SEEN);
	  if (! remove)
	    {
	      bool shared = OMP_CLAUSE_CODE (c) == OMP_CLAUSE_SHARED;
	      if ((n->value & GOVD_DEBUG_PRIVATE)
		  || lang_hooks.decls.omp_private_debug_clause (decl, shared))
		{
		  gcc_assert ((n->value & GOVD_DEBUG_PRIVATE) == 0
			      || ((n->value & GOVD_DATA_SHARE_CLASS)
				  == GOVD_PRIVATE));
		  OMP_CLAUSE_SET_CODE (c, OMP_CLAUSE_PRIVATE);
		  OMP_CLAUSE_PRIVATE_DEBUG (c) = 1;
		}
	    }
	  break;

	case OMP_CLAUSE_LASTPRIVATE:
	  /* Make sure OMP_CLAUSE_LASTPRIVATE_FIRSTPRIVATE is set to
	     accurately reflect the presence of a FIRSTPRIVATE clause.  */
	  decl = OMP_CLAUSE_DECL (c);
	  n = splay_tree_lookup (ctx->variables, (splay_tree_key) decl);
	  OMP_CLAUSE_LASTPRIVATE_FIRSTPRIVATE (c)
	    = (n->value & GOVD_FIRSTPRIVATE) != 0;
	  break;
	  
	case OMP_CLAUSE_REDUCTION:
	case OMP_CLAUSE_COPYIN:
	case OMP_CLAUSE_COPYPRIVATE:
	case OMP_CLAUSE_IF:
	case OMP_CLAUSE_NUM_THREADS:
	case OMP_CLAUSE_SCHEDULE:
	case OMP_CLAUSE_NOWAIT:
	case OMP_CLAUSE_ORDERED:
	case OMP_CLAUSE_DEFAULT:
	  break;

	default:
	  gcc_unreachable ();
	}

      if (remove)
	*list_p = OMP_CLAUSE_CHAIN (c);
      else
	list_p = &OMP_CLAUSE_CHAIN (c);
    }

  /* Add in any implicit data sharing.  */
  splay_tree_foreach (ctx->variables, gimplify_adjust_omp_clauses_1, list_p);
  
  gimplify_omp_ctxp = ctx->outer_context;
  delete_omp_context (ctx);
}

/* Gimplify the contents of an OMP_PARALLEL statement.  This involves
   gimplification of the body, as well as scanning the body for used
   variables.  We need to do this scan now, because variable-sized
   decls will be decomposed during gimplification.  */

static enum gimplify_status
gimplify_omp_parallel (tree *expr_p, gimple_seq pre_p)
{
  tree expr = *expr_p;

  gimplify_scan_omp_clauses (&OMP_PARALLEL_CLAUSES (expr), pre_p,
			     true, OMP_PARALLEL_COMBINED (expr));

  push_gimplify_context ();

  gimplify_stmt (&OMP_PARALLEL_BODY (expr), pre_p);

  /* FIXME tuples.  */
#if 0
  if (TREE_CODE (OMP_PARALLEL_BODY (expr)) == BIND_EXPR)
    pop_gimplify_context (OMP_PARALLEL_BODY (expr));
  else
    pop_gimplify_context (NULL);
#else
  gcc_unreachable ();
#endif

  gimplify_adjust_omp_clauses (&OMP_PARALLEL_CLAUSES (expr));

  return GS_ALL_DONE;
}

/* Gimplify the gross structure of an OMP_FOR statement.  */

static enum gimplify_status
gimplify_omp_for (tree *expr_p, gimple_seq pre_p)
{
  tree for_stmt, decl, t;
  enum gimplify_status ret = GS_OK;

  for_stmt = *expr_p;

  gimplify_scan_omp_clauses (&OMP_FOR_CLAUSES (for_stmt), pre_p, false, false);

  t = OMP_FOR_INIT (for_stmt);
  gcc_assert (TREE_CODE (t) == MODIFY_EXPR
	      || TREE_CODE (t) == GIMPLE_MODIFY_STMT);
  decl = GENERIC_TREE_OPERAND (t, 0);
  gcc_assert (DECL_P (decl));
  gcc_assert (INTEGRAL_TYPE_P (TREE_TYPE (decl)));

  /* Make sure the iteration variable is private.  */
  if (omp_is_private (gimplify_omp_ctxp, decl))
    omp_notice_variable (gimplify_omp_ctxp, decl, true);
  else
    omp_add_variable (gimplify_omp_ctxp, decl, GOVD_PRIVATE | GOVD_SEEN);

#if 0
/* FIXME tuples */
  ret |= gimplify_expr (&GENERIC_TREE_OPERAND (t, 1),
			&OMP_FOR_PRE_BODY (for_stmt), NULL,
			false, is_gimple_val, fb_rvalue);
#endif
  

  tree_to_gimple_tuple (&OMP_FOR_INIT (for_stmt));

  t = OMP_FOR_COND (for_stmt);
  gcc_assert (COMPARISON_CLASS_P (t));
  gcc_assert (GENERIC_TREE_OPERAND (t, 0) == decl);

#if 0
/* FIXME tuples */
  ret |= gimplify_expr (&GENERIC_TREE_OPERAND (t, 1),
			&OMP_FOR_PRE_BODY (for_stmt), NULL, false,
			is_gimple_val, fb_rvalue);
#endif

  tree_to_gimple_tuple (&OMP_FOR_INCR (for_stmt));
  t = OMP_FOR_INCR (for_stmt);
  switch (TREE_CODE (t))
    {
    case PREINCREMENT_EXPR:
    case POSTINCREMENT_EXPR:
      t = build_int_cst (TREE_TYPE (decl), 1);
      t = build2 (PLUS_EXPR, TREE_TYPE (decl), decl, t);
      t = build_gimple_modify_stmt (decl, t);
      OMP_FOR_INCR (for_stmt) = t;
      break;

    case PREDECREMENT_EXPR:
    case POSTDECREMENT_EXPR:
      t = build_int_cst (TREE_TYPE (decl), -1);
      t = build2 (PLUS_EXPR, TREE_TYPE (decl), decl, t);
      t = build_gimple_modify_stmt (decl, t);
      OMP_FOR_INCR (for_stmt) = t;
      break;
      
    case GIMPLE_MODIFY_STMT:
      gcc_assert (GIMPLE_STMT_OPERAND (t, 0) == decl);
      t = GIMPLE_STMT_OPERAND (t, 1);
      switch (TREE_CODE (t))
	{
	case PLUS_EXPR:
	  if (TREE_OPERAND (t, 1) == decl)
	    {
	      TREE_OPERAND (t, 1) = TREE_OPERAND (t, 0);
	      TREE_OPERAND (t, 0) = decl;
	      break;
	    }
	case MINUS_EXPR:
	  gcc_assert (TREE_OPERAND (t, 0) == decl);
	  break;
	default:
	  gcc_unreachable ();
	}

#if 0
/* FIXME tuples */
      ret |= gimplify_expr (&TREE_OPERAND (t, 1),
			    &OMP_FOR_PRE_BODY (for_stmt), NULL, false,
			    is_gimple_val, fb_rvalue);
#endif
      break;

    default:
      gcc_unreachable ();
    }

#if 0
  /* FIXME tuples.  Need to gimplify into a sequence using
     gimplify_stmt and create the GIMPLE OMP_FOR with that sequence.  */
  gimplify_to_stmt_list (&OMP_FOR_BODY (for_stmt));
#endif
  gimplify_adjust_omp_clauses (&OMP_FOR_CLAUSES (for_stmt));

  return ret == GS_ALL_DONE ? GS_ALL_DONE : GS_ERROR;
}

/* Gimplify the gross structure of other OpenMP worksharing constructs.
   In particular, OMP_SECTIONS and OMP_SINGLE.  */

static enum gimplify_status
gimplify_omp_workshare (tree *expr_p, gimple_seq pre_p)
{
  tree stmt = *expr_p;

  gimplify_scan_omp_clauses (&OMP_CLAUSES (stmt), pre_p, false, false);
#if 0
  /* FIXME tuples.  Need to gimplify into a sequence using
     gimplify_stmt and create the GIMPLE OMP_FOR with that sequence.  */
  gimplify_to_stmt_list (&OMP_BODY (stmt));
#endif
  gimplify_adjust_omp_clauses (&OMP_CLAUSES (stmt));

  return GS_ALL_DONE;
}

/* A subroutine of gimplify_omp_atomic.  The front end is supposed to have
   stabilized the lhs of the atomic operation as *ADDR.  Return true if 
   EXPR is this stabilized form.  */

static bool
goa_lhs_expr_p (const_tree expr, const_tree addr)
{
  /* Also include casts to other type variants.  The C front end is fond
     of adding these for e.g. volatile variables.  This is like 
     STRIP_TYPE_NOPS but includes the main variant lookup.  */
  while ((TREE_CODE (expr) == NOP_EXPR
          || TREE_CODE (expr) == CONVERT_EXPR
          || TREE_CODE (expr) == NON_LVALUE_EXPR)
         && TREE_OPERAND (expr, 0) != error_mark_node
         && (TYPE_MAIN_VARIANT (TREE_TYPE (expr))
             == TYPE_MAIN_VARIANT (TREE_TYPE (TREE_OPERAND (expr, 0)))))
    expr = TREE_OPERAND (expr, 0);

  if (TREE_CODE (expr) == INDIRECT_REF && TREE_OPERAND (expr, 0) == addr)
    return true;
  if (TREE_CODE (addr) == ADDR_EXPR && expr == TREE_OPERAND (addr, 0))
    return true;
  return false;
}

/* A subroutine of gimplify_omp_atomic.  Attempt to implement the atomic
   operation as a __sync_fetch_and_op builtin.  INDEX is log2 of the
   size of the data type, and thus usable to find the index of the builtin
   decl.  Returns GS_UNHANDLED if the expression is not of the proper form.  */

static enum gimplify_status
gimplify_omp_atomic_fetch_op (tree *expr_p, tree addr, tree rhs, int index)
{
  enum built_in_function base;
  tree decl, itype;
  enum insn_code *optab;

  /* Check for one of the supported fetch-op operations.  */
  switch (TREE_CODE (rhs))
    {
    case POINTER_PLUS_EXPR:
    case PLUS_EXPR:
      base = BUILT_IN_FETCH_AND_ADD_N;
      optab = sync_add_optab;
      break;
    case MINUS_EXPR:
      base = BUILT_IN_FETCH_AND_SUB_N;
      optab = sync_add_optab;
      break;
    case BIT_AND_EXPR:
      base = BUILT_IN_FETCH_AND_AND_N;
      optab = sync_and_optab;
      break;
    case BIT_IOR_EXPR:
      base = BUILT_IN_FETCH_AND_OR_N;
      optab = sync_ior_optab;
      break;
    case BIT_XOR_EXPR:
      base = BUILT_IN_FETCH_AND_XOR_N;
      optab = sync_xor_optab;
      break;
    default:
      return GS_UNHANDLED;
    }

  /* Make sure the expression is of the proper form.  */
  if (goa_lhs_expr_p (TREE_OPERAND (rhs, 0), addr))
    rhs = TREE_OPERAND (rhs, 1);
  else if (commutative_tree_code (TREE_CODE (rhs))
	   && goa_lhs_expr_p (TREE_OPERAND (rhs, 1), addr))
    rhs = TREE_OPERAND (rhs, 0);
  else
    return GS_UNHANDLED;

  decl = built_in_decls[base + index + 1];
  itype = TREE_TYPE (TREE_TYPE (decl));

  if (optab[TYPE_MODE (itype)] == CODE_FOR_nothing)
    return GS_UNHANDLED;

  *expr_p = build_call_expr (decl, 2, addr, fold_convert (itype, rhs));
  return GS_OK;
}

/* A subroutine of gimplify_omp_atomic_pipeline.  Walk *EXPR_P and replace
   appearances of *LHS_ADDR with LHS_VAR.  If an expression does not involve
   the lhs, evaluate it into a temporary.  Return 1 if the lhs appeared as
   a subexpression, 0 if it did not, or -1 if an error was encountered.  */

static int
goa_stabilize_expr (tree *expr_p, gimple_seq pre_p, tree lhs_addr, tree lhs_var)
{
  tree expr = *expr_p;
  int saw_lhs;

  if (goa_lhs_expr_p (expr, lhs_addr))
    {
      *expr_p = lhs_var;
      return 1;
    }
  if (is_gimple_val (expr))
    return 0;
 
  saw_lhs = 0;
  switch (TREE_CODE_CLASS (TREE_CODE (expr)))
    {
    case tcc_binary:
      saw_lhs |= goa_stabilize_expr (&TREE_OPERAND (expr, 1), pre_p, lhs_addr,
				     lhs_var);
    case tcc_unary:
      saw_lhs |= goa_stabilize_expr (&TREE_OPERAND (expr, 0), pre_p, lhs_addr,
				     lhs_var);
      break;
    default:
      break;
    }

  if (saw_lhs == 0)
    {
      enum gimplify_status gs;
      gs = gimplify_expr (expr_p, pre_p, NULL, is_gimple_val, fb_rvalue);
      if (gs != GS_ALL_DONE)
	saw_lhs = -1;
    }

  return saw_lhs;
}

/* A subroutine of gimplify_omp_atomic.  Implement the atomic operation as:

	oldval = *addr;
      repeat:
	newval = rhs;	// with oldval replacing *addr in rhs
	oldval = __sync_val_compare_and_swap (addr, oldval, newval);
	if (oldval != newval)
	  goto repeat;

   INDEX is log2 of the size of the data type, and thus usable to find the
   index of the builtin decl.  */

static enum gimplify_status
gimplify_omp_atomic_pipeline (tree *expr_p, gimple_seq pre_p,
			      tree addr, tree rhs, int index)
{
  tree oldval, oldival, oldival2, newval, newival, label;
  tree type, itype, cmpxchg, x, iaddr;

  cmpxchg = built_in_decls[BUILT_IN_VAL_COMPARE_AND_SWAP_N + index + 1];
  type = TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (addr)));
  itype = TREE_TYPE (TREE_TYPE (cmpxchg));

  if (sync_compare_and_swap[TYPE_MODE (itype)] == CODE_FOR_nothing)
    return GS_UNHANDLED;

  oldval = create_tmp_var (type, NULL);
  newval = create_tmp_var (type, NULL);

  /* Precompute as much of RHS as possible.  In the same walk, replace
     occurrences of the lhs value with our temporary.  */
  if (goa_stabilize_expr (&rhs, pre_p, addr, oldval) < 0)
    return GS_ERROR;

  x = build_fold_indirect_ref (addr);
  x = build_gimple_modify_stmt (oldval, x);
  gimplify_and_add (x, pre_p);

  /* For floating-point values, we'll need to view-convert them to integers
     so that we can perform the atomic compare and swap.  Simplify the 
     following code by always setting up the "i"ntegral variables.  */
  if (INTEGRAL_TYPE_P (type) || POINTER_TYPE_P (type))
    {
      oldival = oldval;
      newival = newval;
      iaddr = addr;
    }
  else
    {
      oldival = create_tmp_var (itype, NULL);
      newival = create_tmp_var (itype, NULL);

      x = build1 (VIEW_CONVERT_EXPR, itype, oldval);
      x = build_gimple_modify_stmt (oldival, x);
      gimplify_and_add (x, pre_p);
      iaddr = fold_convert (build_pointer_type (itype), addr);
    }

  oldival2 = create_tmp_var (itype, NULL);

  label = create_artificial_label ();
  x = build1 (LABEL_EXPR, void_type_node, label);
  gimplify_and_add (x, pre_p);

  x = build_gimple_modify_stmt (newval, rhs);
  gimplify_and_add (x, pre_p);

  if (newval != newival)
    {
      x = build1 (VIEW_CONVERT_EXPR, itype, newval);
      x = build_gimple_modify_stmt (newival, x);
      gimplify_and_add (x, pre_p);
    }

  x = build_gimple_modify_stmt (oldival2, fold_convert (itype, oldival));
  gimplify_and_add (x, pre_p);

  x = build_call_expr (cmpxchg, 3, iaddr, fold_convert (itype, oldival),
		       fold_convert (itype, newival));
  if (oldval == oldival)
    x = fold_convert (type, x);
  x = build_gimple_modify_stmt (oldival, x);
  gimplify_and_add (x, pre_p);

  /* For floating point, be prepared for the loop backedge.  */
  if (oldval != oldival)
    {
      x = build1 (VIEW_CONVERT_EXPR, type, oldival);
      x = build_gimple_modify_stmt (oldval, x);
      gimplify_and_add (x, pre_p);
    }

  /* Note that we always perform the comparison as an integer, even for
     floating point.  This allows the atomic operation to properly 
     succeed even with NaNs and -0.0.  */
  x = build3 (COND_EXPR, void_type_node,
	      build2 (NE_EXPR, boolean_type_node,
		      fold_convert (itype, oldival), oldival2),
	      build1 (GOTO_EXPR, void_type_node, label), NULL);
  gimplify_and_add (x, pre_p);

  *expr_p = NULL;
  return GS_ALL_DONE;
}

/* A subroutine of gimplify_omp_atomic.  Implement the atomic operation as:

	GOMP_atomic_start ();
	*addr = rhs;
	GOMP_atomic_end ();

   The result is not globally atomic, but works so long as all parallel
   references are within #pragma omp atomic directives.  According to
   responses received from omp@openmp.org, appears to be within spec.
   Which makes sense, since that's how several other compilers handle
   this situation as well.  */

static enum gimplify_status
gimplify_omp_atomic_mutex (tree *expr_p, gimple_seq pre_p, tree addr, tree rhs)
{
  tree t;

  t = built_in_decls[BUILT_IN_GOMP_ATOMIC_START];
  t = build_call_expr (t, 0);
  gimplify_and_add (t, pre_p);

  t = build_fold_indirect_ref (addr);
  t = build_gimple_modify_stmt (t, rhs);
  gimplify_and_add (t, pre_p);
  
  t = built_in_decls[BUILT_IN_GOMP_ATOMIC_END];
  t = build_call_expr (t, 0);
  gimplify_and_add (t, pre_p);

  *expr_p = NULL;
  return GS_ALL_DONE;
}

/* Gimplify an OMP_ATOMIC statement.  */

static enum gimplify_status
gimplify_omp_atomic (tree *expr_p, gimple_seq pre_p)
{
  tree addr = TREE_OPERAND (*expr_p, 0);
  tree rhs = TREE_OPERAND (*expr_p, 1);
  tree type = TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (addr)));
  HOST_WIDE_INT index;

  /* Make sure the type is one of the supported sizes.  */
  index = tree_low_cst (TYPE_SIZE_UNIT (type), 1);
  index = exact_log2 (index);
  if (index >= 0 && index <= 4)
    {
      enum gimplify_status gs;
      unsigned int align;

      if (DECL_P (TREE_OPERAND (addr, 0)))
	align = DECL_ALIGN_UNIT (TREE_OPERAND (addr, 0));
      else if (TREE_CODE (TREE_OPERAND (addr, 0)) == COMPONENT_REF
	       && TREE_CODE (TREE_OPERAND (TREE_OPERAND (addr, 0), 1))
		  == FIELD_DECL)
	align = DECL_ALIGN_UNIT (TREE_OPERAND (TREE_OPERAND (addr, 0), 1));
      else
	align = TYPE_ALIGN_UNIT (type);

      /* __sync builtins require strict data alignment.  */
      if (exact_log2 (align) >= index)
	{
	  /* When possible, use specialized atomic update functions.  */
	  if (INTEGRAL_TYPE_P (type) || POINTER_TYPE_P (type))
	    {
	      gs = gimplify_omp_atomic_fetch_op (expr_p, addr, rhs, index);
	      if (gs != GS_UNHANDLED)
		return gs;
	    }

	  /* If we don't have specialized __sync builtins, try and implement
	     as a compare and swap loop.  */
	  gs = gimplify_omp_atomic_pipeline (expr_p, pre_p, addr, rhs, index);
	  if (gs != GS_UNHANDLED)
	    return gs;
	}
    }

  /* The ultimate fallback is wrapping the operation in a mutex.  */
  return gimplify_omp_atomic_mutex (expr_p, pre_p, addr, rhs);
}


/* Converts the GENERIC expression tree *EXPR_P to GIMPLE.  If the
   expression produces a value to be used as an operand inside a GIMPLE
   statement, the value will be stored back in *EXPR_P.  This value will
   be a tree of class tcc_declaration, tcc_constant, tcc_reference or
   an SSA_NAME.  The corresponding sequence of GIMPLE statements is
   emitted in PRE_P and POST_P.

   Additionally, this process may overwrite parts of the input
   expression during gimplification.  FIXME tuples, it should be
   possible to do non-destructive gimplification.

   EXPR_P points to the GENERIC expression to convert to GIMPLE.  If
      the expression needs to evaluate to a value to be used as
      an operand in a GIMPLE statement, this value will be stored in
      *EXPR_P on exit.  This happens when the caller specifies one
      of fb_lvalue or fb_rvalue fallback flags.

   PRE_P will contain the sequence of GIMPLE statements corresponding
       to the evaluation of EXPR and all the side-effects that must
       be executed before the main expression.

   POST_P will contain the sequence of GIMPLE statements corresponding
       to the evaluation of all the side-effects that must be executed
       after the main expression.  If this is NULL, the post
       side-effects are stored at the end of PRE_P.

       The reason why the output is split in two is to handle post
       side-effects explicitly.  In some cases, an expression may have
       inner and outer post side-effects which need to be emitted in
       an order different from the one given by the recursive
       traversal.  For instance, for the expression (*p--)++ the post
       side-effects of '--' must actually occur *after* the post
       side-effects of '++'.  However, gimplification will first visit
       the inner expression, so if a separate POST sequence was not
       used, the resulting sequence would be:

       	    1	t.1 = *p
       	    2	p = p - 1
       	    3	t.2 = t.1 + 1
       	    4	*p = t.2

       However, the post-decrement operation in line #2 must not be
       evaluated until after the store to *p at line #4, so the
       correct sequence should be:

       	    1	t.1 = *p
       	    2	t.2 = t.1 + 1
       	    3	*p = t.2
       	    4	p = p - 1

       So, by specifying a separate post queue, it is possible
       to emit the post side-effects in the correct order.
       If POST_P is NULL, an internal queue will be used.  Before
       returning to the caller, the sequence POST_P is appended to
       the main output sequence PRE_P.

   GIMPLE_TEST_F points to a function that takes a tree T and
       returns nonzero if T is in the GIMPLE form requested by the
       caller.  The GIMPLE predicates are in tree-gimple.c.

   FALLBACK tells the function what sort of a temporary we want if
       gimplification cannot produce an expression that complies with
       GIMPLE_TEST_F.

       fb_none means that no temporary should be generated
       fb_rvalue means that an rvalue is OK to generate
       fb_lvalue means that an lvalue is OK to generate
       fb_either means that either is OK, but an lvalue is preferable.
       fb_mayfail means that gimplification may fail (in which case
       GS_ERROR will be returned)

   The return value is either GS_ERROR or GS_ALL_DONE, since this
   function iterates until EXPR is completely gimplified or an error
   occurs.  */

enum gimplify_status
gimplify_expr (tree *expr_p, gimple_seq pre_p, gimple_seq post_p,
	       bool (*gimple_test_f) (tree), fallback_t fallback)
{
  tree tmp;
  struct gimple_sequence internal_pre;
  struct gimple_sequence internal_post;
  tree save_expr;
  bool is_statement;
  location_t saved_location;
  enum gimplify_status ret;

  save_expr = *expr_p;
  if (save_expr == NULL_TREE)
    return GS_ALL_DONE;

  /* If we are gimplifying a top-level statement, PRE_P must be valid.  */
  is_statement = gimple_test_f == is_gimple_stmt;
  if (is_statement)
    gcc_assert (pre_p);

  /* Consistency checks.  FIXME tuples, FALLBACK should not be needed.
     We can figure it out from GIMPLE_TEST_F.  */
  if (gimple_test_f == is_gimple_val
      || gimple_test_f == is_gimple_reg
      || gimple_test_f == is_gimple_formal_tmp_rhs
      || gimple_test_f == is_gimple_formal_tmp_reg
      || gimple_test_f == is_gimple_formal_tmp_var
      || gimple_test_f == is_gimple_call_addr
      || gimple_test_f == is_gimple_condexpr
      || gimple_test_f == is_gimple_mem_rhs
      || gimple_test_f == is_gimple_reg_rhs
      || gimple_test_f == is_gimple_asm_val)
    gcc_assert (fallback & fb_rvalue);
  else if (gimple_test_f == is_gimple_min_lval
	   || gimple_test_f == is_gimple_lvalue)
    gcc_assert (fallback & fb_lvalue);
  else if (gimple_test_f == is_gimple_addressable)
    gcc_assert (fallback & fb_either);
  else if (gimple_test_f == is_gimple_stmt)
    gcc_assert (fallback == fb_none);
  else
    {
      /* We should have recognized the GIMPLE_TEST_F predicate to
	 know what kind of fallback to use in case a temporary is
	 needed to hold the value or address of *EXPR_P.  */
      gcc_unreachable ();
    }


  /* We used to check the predicate here and return immediately if it
     succeeds.  This is wrong; the design is for gimplification to be
     idempotent, and for the predicates to only test for valid forms, not
     whether they are fully simplified.  */

  /* Set up our internal queues if needed.  */
  gimple_seq_init (&internal_pre);
  gimple_seq_init (&internal_post);

  if (pre_p == NULL)
    pre_p = &internal_pre;

  if (post_p == NULL)
    post_p = &internal_post;

  saved_location = input_location;
  if (save_expr != error_mark_node
      && EXPR_HAS_LOCATION (*expr_p))
    input_location = EXPR_LOCATION (*expr_p);

  /* Loop over the specific gimplifiers until the toplevel node
     remains the same.  */
  do
    {
      /* Strip away as many useless type conversions as possible
	 at the toplevel.  */
      STRIP_USELESS_TYPE_CONVERSION (*expr_p);

      /* Remember the expr.  */
      save_expr = *expr_p;

      /* Die, die, die, my darling.  */
      if (save_expr == error_mark_node
	  || (!GIMPLE_STMT_P (save_expr)
	      && TREE_TYPE (save_expr)
	      && TREE_TYPE (save_expr) == error_mark_node))
	{
	  ret = GS_ERROR;
	  break;
	}

      /* Do any language-specific gimplification.  */
      ret = lang_hooks.gimplify_expr (expr_p, pre_p, post_p);
      if (ret == GS_OK)
	{
	  if (*expr_p == NULL_TREE)
	    break;
	  if (*expr_p != save_expr)
	    continue;
	}
      else if (ret != GS_UNHANDLED)
	break;

      ret = GS_OK;
      switch (TREE_CODE (*expr_p))
	{
	  /* First deal with the special cases.  */

	case POSTINCREMENT_EXPR:
	case POSTDECREMENT_EXPR:
	case PREINCREMENT_EXPR:
	case PREDECREMENT_EXPR:
	  ret = gimplify_self_mod_expr (expr_p, pre_p, post_p,
					fallback != fb_none);
	  break;

	case ARRAY_REF:
	case ARRAY_RANGE_REF:
	case REALPART_EXPR:
	case IMAGPART_EXPR:
	case COMPONENT_REF:
	case VIEW_CONVERT_EXPR:
	  ret = gimplify_compound_lval (expr_p, pre_p, post_p,
					fallback ? fallback : fb_rvalue);
	  break;

	case COND_EXPR:
	  ret = gimplify_cond_expr (expr_p, pre_p, fallback);

	  /* C99 code may assign to an array in a structure value of a
	     conditional expression, and this has undefined behavior
	     only on execution, so create a temporary if an lvalue is
	     required.  */
	  if (fallback == fb_lvalue)
	    {
	      *expr_p = get_initialized_tmp_var (*expr_p, pre_p, post_p);
	      mark_addressable (*expr_p);
	    }
	  break;

	case CALL_EXPR:
	  ret = gimplify_call_expr (expr_p, pre_p, fallback != fb_none);

	  /* C99 code may assign to an array in a structure returned
	     from a function, and this has undefined behavior only on
	     execution, so create a temporary if an lvalue is
	     required.  */
	  if (fallback == fb_lvalue)
	    {
	      *expr_p = get_initialized_tmp_var (*expr_p, pre_p, post_p);
	      mark_addressable (*expr_p);
	    }
	  break;

	case TREE_LIST:
	  gcc_unreachable ();

	case COMPOUND_EXPR:
	  ret = gimplify_compound_expr (expr_p, pre_p, fallback != fb_none);
	  break;

	case MODIFY_EXPR:
	case GIMPLE_MODIFY_STMT:
	case INIT_EXPR:
	  ret = gimplify_modify_expr (expr_p, pre_p, post_p,
				      fallback != fb_none);
	  break;

	case TRUTH_ANDIF_EXPR:
	case TRUTH_ORIF_EXPR:
	  ret = gimplify_boolean_expr (expr_p);
	  break;

	case TRUTH_NOT_EXPR:
	  TREE_OPERAND (*expr_p, 0) = gimple_boolify (TREE_OPERAND (*expr_p, 0));
	  ret = gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_gimple_val, fb_rvalue);
	  recalculate_side_effects (*expr_p);
	  break;

	case ADDR_EXPR:
	  ret = gimplify_addr_expr (expr_p, pre_p, post_p);
	  break;

	case VA_ARG_EXPR:
	  ret = gimplify_va_arg_expr (expr_p, pre_p, post_p);
	  break;

	case CONVERT_EXPR:
	case NOP_EXPR:
	  if (IS_EMPTY_STMT (*expr_p))
	    {
	      ret = GS_ALL_DONE;
	      break;
	    }

	  if (VOID_TYPE_P (TREE_TYPE (*expr_p))
	      || fallback == fb_none)
	    {
	      /* Just strip a conversion to void (or in void context) and
		 try again.  */
	      *expr_p = TREE_OPERAND (*expr_p, 0);
	      break;
	    }

	  ret = gimplify_conversion (expr_p);
	  if (ret == GS_ERROR)
	    break;
	  if (*expr_p != save_expr)
	    break;
	  /* FALLTHRU */

	case FIX_TRUNC_EXPR:
	  /* unary_expr: ... | '(' cast ')' val | ...  */
	  ret = gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_gimple_val, fb_rvalue);
	  recalculate_side_effects (*expr_p);
	  break;

	case INDIRECT_REF:
	  *expr_p = fold_indirect_ref (*expr_p);
	  if (*expr_p != save_expr)
	    break;
	  /* else fall through.  */
	case ALIGN_INDIRECT_REF:
	case MISALIGNED_INDIRECT_REF:
	  ret = gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_gimple_reg, fb_rvalue);
	  recalculate_side_effects (*expr_p);
	  break;

	  /* Constants need not be gimplified.  */
	case INTEGER_CST:
	case REAL_CST:
	case FIXED_CST:
	case STRING_CST:
	case COMPLEX_CST:
	case VECTOR_CST:
	  ret = GS_ALL_DONE;
	  break;

	case CONST_DECL:
	  /* If we require an lvalue, such as for ADDR_EXPR, retain the
	     CONST_DECL node.  Otherwise the decl is replaceable by its
	     value.  */
	  /* ??? Should be == fb_lvalue, but ADDR_EXPR passes fb_either.  */
	  if (fallback & fb_lvalue)
	    ret = GS_ALL_DONE;
	  else
	    *expr_p = DECL_INITIAL (*expr_p);
	  break;

	case DECL_EXPR:
	  ret = gimplify_decl_expr (expr_p, pre_p);
	  break;

	case EXC_PTR_EXPR:
	  /* FIXME make this a decl.  */
	  ret = GS_ALL_DONE;
	  break;

	case BIND_EXPR:
	  ret = gimplify_bind_expr (expr_p, pre_p);
	  break;

	case LOOP_EXPR:
	  ret = gimplify_loop_expr (expr_p, pre_p);
	  break;

	case SWITCH_EXPR:
	  ret = gimplify_switch_expr (expr_p, pre_p);
	  break;

	case EXIT_EXPR:
	  ret = gimplify_exit_expr (expr_p);
	  break;

	case GOTO_EXPR:
	  /* If the target is not LABEL, then it is a computed jump
	     and the target needs to be gimplified.  */
	  if (TREE_CODE (GOTO_DESTINATION (*expr_p)) != LABEL_DECL)
	    ret = gimplify_expr (&GOTO_DESTINATION (*expr_p), pre_p,
				 NULL, is_gimple_val, fb_rvalue);
	  gimple_seq_add (pre_p, build_gimple_goto (GOTO_DESTINATION (*expr_p)));
	  break;

	case LABEL_EXPR:
	  ret = GS_ALL_DONE;
	  gcc_assert (decl_function_context (LABEL_EXPR_LABEL (*expr_p))
		      == current_function_decl);
	  gimple_seq_add (pre_p, build_gimple_label (LABEL_EXPR_LABEL (*expr_p)));
	  break;

	case CASE_LABEL_EXPR:
	  ret = gimplify_case_label_expr (expr_p, pre_p);
	  break;

	case RETURN_EXPR:
	  ret = gimplify_return_expr (*expr_p, pre_p);
	  break;

	case CONSTRUCTOR:
	  /* Don't reduce this in place; let gimplify_init_constructor work its
	     magic.  Buf if we're just elaborating this for side effects, just
	     gimplify any element that has side-effects.  */
	  if (fallback == fb_none)
	    {
	      unsigned HOST_WIDE_INT ix;
	      constructor_elt *ce;
	      tree temp = NULL_TREE;
	      for (ix = 0;
		   VEC_iterate (constructor_elt, CONSTRUCTOR_ELTS (*expr_p),
				ix, ce);
		   ix++)
		if (TREE_SIDE_EFFECTS (ce->value))
		  append_to_statement_list (ce->value, &temp);

	      *expr_p = temp;
	      ret = GS_OK;
	    }
	  /* C99 code may assign to an array in a constructed
	     structure or union, and this has undefined behavior only
	     on execution, so create a temporary if an lvalue is
	     required.  */
	  else if (fallback == fb_lvalue)
	    {
	      *expr_p = get_initialized_tmp_var (*expr_p, pre_p, post_p);
	      mark_addressable (*expr_p);
	    }
	  else
	    ret = GS_ALL_DONE;
	  break;

	  /* The following are special cases that are not handled by the
	     original GIMPLE grammar.  */

	  /* SAVE_EXPR nodes are converted into a GIMPLE identifier and
	     eliminated.  */
	case SAVE_EXPR:
	  ret = gimplify_save_expr (expr_p, pre_p, post_p);
	  break;

	case BIT_FIELD_REF:
	  {
	    enum gimplify_status r0, r1, r2;

	    r0 = gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p,
				post_p, is_gimple_lvalue, fb_either);
	    r1 = gimplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p,
				post_p, is_gimple_val, fb_rvalue);
	    r2 = gimplify_expr (&TREE_OPERAND (*expr_p, 2), pre_p,
				post_p, is_gimple_val, fb_rvalue);
	    recalculate_side_effects (*expr_p);

	    ret = MIN (r0, MIN (r1, r2));
	  }
	  break;

	case NON_LVALUE_EXPR:
	  /* This should have been stripped above.  */
	  gcc_unreachable ();

	case ASM_EXPR:
	  ret = gimplify_asm_expr (expr_p, pre_p, post_p);
	  break;

	case TRY_FINALLY_EXPR:
	case TRY_CATCH_EXPR:
	  {
	    gimple try
	      = build_gimple_try (NULL, NULL,
				  TREE_CODE (*expr_p) == TRY_FINALLY_EXPR ?
				  GIMPLE_TRY_FINALLY : GIMPLE_TRY_CATCH);

	    gimplify_and_add (TREE_OPERAND (*expr_p, 0), gimple_try_eval (try));
	    gimplify_and_add (TREE_OPERAND (*expr_p, 1),
			      gimple_try_cleanup (try));
	    gimple_seq_add (pre_p, try);

	    ret = GS_ALL_DONE;
	    break;
	  }

	case CLEANUP_POINT_EXPR:
	  gcc_unreachable();
#if 0
/* FIXME tuples */
	  ret = gimplify_cleanup_point_expr (expr_p, pre_p);
#endif
	  break;

	case TARGET_EXPR:
	  ret = gimplify_target_expr (expr_p, pre_p, post_p);
	  break;

	case CATCH_EXPR:
	  gimplify_stmt (&CATCH_BODY (*expr_p), pre_p);
	  ret = GS_ALL_DONE;
	  break;

	case EH_FILTER_EXPR:
	  gimplify_stmt (&EH_FILTER_FAILURE (*expr_p), pre_p);
	  ret = GS_ALL_DONE;
	  break;

	case CHANGE_DYNAMIC_TYPE_EXPR:
	  ret = gimplify_expr (&CHANGE_DYNAMIC_TYPE_LOCATION (*expr_p),
			       pre_p, post_p, is_gimple_reg, fb_lvalue);
	  break;

	case OBJ_TYPE_REF:
	  {
	    enum gimplify_status r0, r1;
	    r0 = gimplify_expr (&OBJ_TYPE_REF_OBJECT (*expr_p), pre_p,
				post_p, is_gimple_val, fb_rvalue);
	    r1 = gimplify_expr (&OBJ_TYPE_REF_EXPR (*expr_p), pre_p,
				post_p, is_gimple_val, fb_rvalue);
	    ret = MIN (r0, r1);
	  }
	  break;

	case LABEL_DECL:
	  /* We get here when taking the address of a label.  We mark
	     the label as "forced"; meaning it can never be removed and
	     it is a potential target for any computed goto.  */
	  FORCED_LABEL (*expr_p) = 1;
	  ret = GS_ALL_DONE;
	  break;

	case STATEMENT_LIST:
	  ret = gimplify_statement_list (expr_p, pre_p);
	  break;

	case WITH_SIZE_EXPR:
	  {
	    gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p,
			   post_p == &internal_post ? NULL : post_p,
			   gimple_test_f, fallback);
	    gimplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p,
			   is_gimple_val, fb_rvalue);
	  }
	  break;

	case VAR_DECL:
	case PARM_DECL:
	  ret = gimplify_var_or_parm_decl (expr_p);
	  break;

	case RESULT_DECL:
	  /* When within an OpenMP context, notice uses of variables.  */
	  if (gimplify_omp_ctxp)
	    omp_notice_variable (gimplify_omp_ctxp, *expr_p, true);
	  ret = GS_ALL_DONE;
	  break;

	case SSA_NAME:
	  /* Allow callbacks into the gimplifier during optimization.  */
	  ret = GS_ALL_DONE;
	  break;

	case OMP_PARALLEL:
	  ret = gimplify_omp_parallel (expr_p, pre_p);
	  break;

	case OMP_FOR:
	  ret = gimplify_omp_for (expr_p, pre_p);
	  break;

	case OMP_SECTIONS:
	case OMP_SINGLE:
	  ret = gimplify_omp_workshare (expr_p, pre_p);
	  break;

	case OMP_SECTION:
	case OMP_MASTER:
	case OMP_ORDERED:
	case OMP_CRITICAL:
	  gimplify_stmt (&OMP_BODY (*expr_p), pre_p);
	  break;

	case OMP_ATOMIC:
	  ret = gimplify_omp_atomic (expr_p, pre_p);
	  break;

	case OMP_RETURN:
	case OMP_CONTINUE:
	  ret = GS_ALL_DONE;
	  break;

	case POINTER_PLUS_EXPR:
          /* Convert ((type *)A)+offset into &A->field_of_type_and_offset.
	     The second is gimple immediate saving a need for extra statement.
	   */
	  if (TREE_CODE (TREE_OPERAND (*expr_p, 1)) == INTEGER_CST
	      && (tmp = maybe_fold_offset_to_reference
			 (TREE_OPERAND (*expr_p, 0), TREE_OPERAND (*expr_p, 1),
		   	  TREE_TYPE (TREE_TYPE (*expr_p)))))
	     {
	       tree ptr_type = build_pointer_type (TREE_TYPE (tmp));
	       if (useless_type_conversion_p (TREE_TYPE (*expr_p), ptr_type))
		 {
                   *expr_p = build_fold_addr_expr_with_type (tmp, ptr_type);
		   break;
		 }
	     }
	  /* Convert (void *)&a + 4 into (void *)&a[1].  */
	  if (TREE_CODE (TREE_OPERAND (*expr_p, 0)) == NOP_EXPR
	      && TREE_CODE (TREE_OPERAND (*expr_p, 1)) == INTEGER_CST
	      && POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (TREE_OPERAND (*expr_p,
									0),0)))
	      && (tmp = maybe_fold_offset_to_reference
			 (TREE_OPERAND (TREE_OPERAND (*expr_p, 0), 0),
			  TREE_OPERAND (*expr_p, 1),
		   	  TREE_TYPE (TREE_TYPE
				  (TREE_OPERAND (TREE_OPERAND (*expr_p, 0),
						 0))))))
	     {
               tmp = build_fold_addr_expr (tmp);
               *expr_p = fold_convert (TREE_TYPE (*expr_p), tmp);
	       break;
	     }
          /* FALLTHRU */

	default:
	  switch (TREE_CODE_CLASS (TREE_CODE (*expr_p)))
	    {
	    case tcc_comparison:
	      /* Handle comparison of objects of non scalar mode aggregates
	     	 with a call to memcmp.  It would be nice to only have to do
	     	 this for variable-sized objects, but then we'd have to allow
	     	 the same nest of reference nodes we allow for MODIFY_EXPR and
	     	 that's too complex.

		 Compare scalar mode aggregates as scalar mode values.  Using
		 memcmp for them would be very inefficient at best, and is
		 plain wrong if bitfields are involved.  */
		{
		  tree type = TREE_TYPE (TREE_OPERAND (*expr_p, 1));

		  if (!AGGREGATE_TYPE_P (type))
		    goto expr_2;
		  else if (TYPE_MODE (type) != BLKmode)
		    ret = gimplify_scalar_mode_aggregate_compare (expr_p);
		  else
		    ret = gimplify_variable_sized_compare (expr_p);

		  break;
		}

	    /* If *EXPR_P does not need to be special-cased, handle it
	       according to its class.  */
	    case tcc_unary:
	      ret = gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p,
				   post_p, is_gimple_val, fb_rvalue);
	      break;

	    case tcc_binary:
	    expr_2:
	      {
		enum gimplify_status r0, r1;

		r0 = gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p,
		                    post_p, is_gimple_val, fb_rvalue);
		r1 = gimplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p,
				    post_p, is_gimple_val, fb_rvalue);

		ret = MIN (r0, r1);
		break;
	      }

	    case tcc_declaration:
	    case tcc_constant:
	      ret = GS_ALL_DONE;
	      goto dont_recalculate;

	    default:
	      gcc_assert (TREE_CODE (*expr_p) == TRUTH_AND_EXPR
			  || TREE_CODE (*expr_p) == TRUTH_OR_EXPR
			  || TREE_CODE (*expr_p) == TRUTH_XOR_EXPR);
	      goto expr_2;
	    }

	  recalculate_side_effects (*expr_p);

	dont_recalculate:
	  break;
	}

      /* If we replaced *expr_p, gimplify again.  */
      if (ret == GS_OK && (*expr_p == NULL || *expr_p == save_expr))
	ret = GS_ALL_DONE;
    }
  while (ret == GS_OK);

  /* If we encountered an error_mark somewhere nested inside, either
     stub out the statement or propagate the error back out.  */
  if (ret == GS_ERROR)
    {
      if (is_statement)
	*expr_p = NULL;
      goto out;
    }

  /* This was only valid as a return value from the langhook, which
     we handled.  Make sure it doesn't escape from any other context.  */
  gcc_assert (ret != GS_UNHANDLED);

  if (fallback == fb_none && *expr_p && !is_gimple_stmt (*expr_p))
    {
      /* We aren't looking for a value, and we don't have a valid
	 statement.  If it doesn't have side-effects, throw it away.  */
      if (!TREE_SIDE_EFFECTS (*expr_p))
	*expr_p = NULL;
      else if (!TREE_THIS_VOLATILE (*expr_p))
	{
	  /* This is probably a _REF that contains something nested that
	     has side effects.  Recurse through the operands to find it.  */
	  enum tree_code code = TREE_CODE (*expr_p);

	  switch (code)
	    {
	    case COMPONENT_REF:
	    case REALPART_EXPR:
	    case IMAGPART_EXPR:
	    case VIEW_CONVERT_EXPR:
	      gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			     gimple_test_f, fallback);
	      break;

	    case ARRAY_REF:
	    case ARRAY_RANGE_REF:
	      gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			     gimple_test_f, fallback);
	      gimplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p,
			     gimple_test_f, fallback);
	      break;

	    default:
	       /* Anything else with side-effects must be converted to
		  a valid statement before we get here.  */
	      gcc_unreachable ();
	    }

	  *expr_p = NULL;
	}
      else if (COMPLETE_TYPE_P (TREE_TYPE (*expr_p))
	       && TYPE_MODE (TREE_TYPE (*expr_p)) != BLKmode)
	{
	  /* Historically, the compiler has treated a bare reference
	     to a non-BLKmode volatile lvalue as forcing a load.  */
	  tree type = TYPE_MAIN_VARIANT (TREE_TYPE (*expr_p));

	  /* Normally, we do not want to create a temporary for a
	     TREE_ADDRESSABLE type because such a type should not be
	     copied by bitwise-assignment.  However, we make an
	     exception here, as all we are doing here is ensuring that
	     we read the bytes that make up the type.  We use
	     create_tmp_var_raw because create_tmp_var will abort when
	     given a TREE_ADDRESSABLE type.  */
	  tree tmp = create_tmp_var_raw (type, "vol");
	  gimple_add_tmp_var (tmp);
	  *expr_p = build_gimple_modify_stmt (tmp, *expr_p);
	}
      else
	/* We can't do anything useful with a volatile reference to
	   an incomplete type, so just throw it away.  Likewise for
	   a BLKmode type, since any implicit inner load should
	   already have been turned into an explicit one by the
	   gimplification process.  */
	*expr_p = NULL;
    }

  /* If we are gimplifying at the statement level, we're done.  Tack
     everything together and return.  */
  if (fallback == fb_none || is_statement)
    {
      if (!gimple_seq_empty_p (&internal_pre) ||
          !gimple_seq_empty_p (&internal_post))
	{
	  gimple_seq_append (&internal_pre, &internal_post);
	  gimple_seq_append (pre_p, &internal_pre);
	}

      if (!gimple_seq_empty_p (&internal_pre))
	annotate_all_with_locus (&internal_pre, input_location);

      goto out;
    }

#ifdef ENABLE_GIMPLE_CHECKING
  if (*expr_p)
    {
      enum tree_code code = TREE_CODE (*expr_p);
      /* These expressions should already be in gimple IR form.  */
      gcc_assert (code != MODIFY_EXPR
	  	  && code != GIMPLE_MODIFY_STMT
		  && code != ASM_EXPR
		  && code != BIND_EXPR
		  && code != CATCH_EXPR
		  && code != COND_EXPR
		  && code != EH_FILTER_EXPR
		  && code != GOTO_EXPR
		  && code != LABEL_EXPR
		  && code != LOOP_EXPR
		  && code != PHI_NODE
		  && code != RESX_EXPR
		  && code != SWITCH_EXPR
		  && code != TRY_FINALLY_EXPR
		  && code != OMP_CONTINUE
		  && code != OMP_CRITICAL
		  && code != OMP_FOR
		  && code != OMP_MASTER
		  && code != OMP_ORDERED
		  && code != OMP_PARALLEL
		  && code != OMP_RETURN
		  && code != OMP_SECTIONS
		  && code != OMP_SECTION
		  && code != OMP_SINGLE);
    }
#endif

  /* Otherwise we're gimplifying a subexpression, so the resulting value is
     interesting.  */

  /* If it's sufficiently simple already, we're done.  Unless we are
     handling some post-effects internally; if that's the case, we need to
     copy into a temp before adding the post-effects to the tree.  */
  if (gimple_seq_empty_p (&internal_post) && (*gimple_test_f) (*expr_p))
    goto out;

  /* Otherwise, we need to create a new temporary for the gimplified
     expression.  */

  /* We can't return an lvalue if we have an internal postqueue.  The
     object the lvalue refers to would (probably) be modified by the
     postqueue; we need to copy the value out first, which means an
     rvalue.  */
  if ((fallback & fb_lvalue)
      && gimple_seq_empty_p (&internal_post)
      && is_gimple_addressable (*expr_p))
    {
      /* An lvalue will do.  Take the address of the expression, store it
	 in a temporary, and replace the expression with an INDIRECT_REF of
	 that temporary.  */
      tmp = build_fold_addr_expr (*expr_p);
      gimplify_expr (&tmp, pre_p, post_p, is_gimple_reg, fb_rvalue);
      *expr_p = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (tmp)), tmp);
    }
  else if ((fallback & fb_rvalue) && is_gimple_formal_tmp_rhs (*expr_p))
    {
      gcc_assert (!VOID_TYPE_P (TREE_TYPE (*expr_p)));

      /* An rvalue will do.  Assign the gimplified expression into a new
	 temporary TMP and replace the original expression with TMP.  */

      if (!gimple_seq_empty_p (&internal_post) || (fallback & fb_lvalue))
	/* The postqueue might change the value of the expression between
	   the initialization and use of the temporary, so we can't use a
	   formal temp.  FIXME do we care?  */
	*expr_p = get_initialized_tmp_var (*expr_p, pre_p, post_p);
      else
	*expr_p = get_formal_tmp_var (*expr_p, pre_p);

      if (TREE_CODE (*expr_p) != SSA_NAME)
	DECL_GIMPLE_FORMAL_TEMP_P (*expr_p) = 1;
    }
  else
    {
#ifdef ENABLE_GIMPLE_CHECKING
      if (!(fallback & fb_mayfail))
	{
	  fprintf (stderr, "gimplification failed:\n");
	  print_generic_expr (stderr, *expr_p, 0);
	  debug_tree (*expr_p);
	  internal_error ("gimplification failed");
	}
#endif
      gcc_assert (fallback & fb_mayfail);

      /* If this is an asm statement, and the user asked for the
	 impossible, don't die.  Fail and let gimplify_asm_expr
	 issue an error.  */
      ret = GS_ERROR;
      goto out;
    }

  /* Make sure the temporary matches our predicate.  */
  gcc_assert ((*gimple_test_f) (*expr_p));

  if (!gimple_seq_empty_p (&internal_post))
    {
      annotate_all_with_locus (&internal_post, input_location);
      gimple_seq_append (pre_p, &internal_post);
    }

 out:
  input_location = saved_location;
  return ret;
}

/* Look through TYPE for variable-sized objects and gimplify each such
   size that we find.  Add to LIST_P any statements generated.  */

void
gimplify_type_sizes (tree type, gimple_seq list_p)
{
  tree field, t;

  if (type == NULL || type == error_mark_node)
    return;

  /* We first do the main variant, then copy into any other variants.  */
  type = TYPE_MAIN_VARIANT (type);

  /* Avoid infinite recursion.  */
  if (TYPE_SIZES_GIMPLIFIED (type))
    return;

  TYPE_SIZES_GIMPLIFIED (type) = 1;

  switch (TREE_CODE (type))
    {
    case INTEGER_TYPE:
    case ENUMERAL_TYPE:
    case BOOLEAN_TYPE:
    case REAL_TYPE:
    case FIXED_POINT_TYPE:
      gimplify_one_sizepos (&TYPE_MIN_VALUE (type), list_p);
      gimplify_one_sizepos (&TYPE_MAX_VALUE (type), list_p);

      for (t = TYPE_NEXT_VARIANT (type); t; t = TYPE_NEXT_VARIANT (t))
	{
	  TYPE_MIN_VALUE (t) = TYPE_MIN_VALUE (type);
	  TYPE_MAX_VALUE (t) = TYPE_MAX_VALUE (type);
	}
      break;

    case ARRAY_TYPE:
      /* These types may not have declarations, so handle them here.  */
      gimplify_type_sizes (TREE_TYPE (type), list_p);
      gimplify_type_sizes (TYPE_DOMAIN (type), list_p);
      break;

    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
	if (TREE_CODE (field) == FIELD_DECL)
	  {
	    gimplify_one_sizepos (&DECL_FIELD_OFFSET (field), list_p);
	    gimplify_type_sizes (TREE_TYPE (field), list_p);
	  }
      break;

    case POINTER_TYPE:
    case REFERENCE_TYPE:
	/* We used to recurse on the pointed-to type here, which turned out to
	   be incorrect because its definition might refer to variables not
	   yet initialized at this point if a forward declaration is involved.

	   It was actually useful for anonymous pointed-to types to ensure
	   that the sizes evaluation dominates every possible later use of the
	   values.  Restricting to such types here would be safe since there
	   is no possible forward declaration around, but would introduce an
	   undesirable middle-end semantic to anonymity.  We then defer to
	   front-ends the responsibility of ensuring that the sizes are
	   evaluated both early and late enough, e.g. by attaching artificial
	   type declarations to the tree.  */
      break;

    default:
      break;
    }

  gimplify_one_sizepos (&TYPE_SIZE (type), list_p);
  gimplify_one_sizepos (&TYPE_SIZE_UNIT (type), list_p);

  for (t = TYPE_NEXT_VARIANT (type); t; t = TYPE_NEXT_VARIANT (t))
    {
      TYPE_SIZE (t) = TYPE_SIZE (type);
      TYPE_SIZE_UNIT (t) = TYPE_SIZE_UNIT (type);
      TYPE_SIZES_GIMPLIFIED (t) = 1;
    }
}

/* A subroutine of gimplify_type_sizes to make sure that *EXPR_P,
   a size or position, has had all of its SAVE_EXPRs evaluated.
   We add any required statements to STMT_P.  */

void
gimplify_one_sizepos (tree *expr_p, gimple_seq stmt_p)
{
  tree type, expr = *expr_p;

  /* We don't do anything if the value isn't there, is constant, or contains
     A PLACEHOLDER_EXPR.  We also don't want to do anything if it's already
     a VAR_DECL.  If it's a VAR_DECL from another function, the gimplifier
     will want to replace it with a new variable, but that will cause problems
     if this type is from outside the function.  It's OK to have that here.  */
  if (expr == NULL_TREE || TREE_CONSTANT (expr)
      || TREE_CODE (expr) == VAR_DECL
      || CONTAINS_PLACEHOLDER_P (expr))
    return;

  type = TREE_TYPE (expr);
  *expr_p = unshare_expr (expr);

  gimplify_expr (expr_p, NULL, NULL, is_gimple_val, fb_rvalue);
  expr = *expr_p;

  /* Verify that we've an exact type match with the original expression.
     In particular, we do not wish to drop a "sizetype" in favour of a
     type of similar dimensions.  We don't want to pollute the generic
     type-stripping code with this knowledge because it doesn't matter
     for the bulk of GENERIC/GIMPLE.  It only matters that TYPE_SIZE_UNIT
     and friends retain their "sizetype-ness".  */
  if (TREE_TYPE (expr) != type
      && TREE_CODE (type) == INTEGER_TYPE
      && TYPE_IS_SIZETYPE (type))
    {
      tree tmp;

      *expr_p = create_tmp_var (type, NULL);
      tmp = build1 (NOP_EXPR, type, expr);
      tmp = build_gimple_modify_stmt (*expr_p, tmp);
      if (EXPR_HAS_LOCATION (expr))
        SET_EXPR_LOCUS (tmp, EXPR_LOCUS (expr));
      else
        SET_EXPR_LOCATION (tmp, input_location);

      gimplify_and_add (tmp, stmt_p);
    }
}


/* Gimplify the body of statements pointed to by BODY_P and return a
   GIMPLE_BIND containing the sequence of GIMPLE statements
   corresponding to BODY_P.  FNDECL is the function decl containing
   *BODY_P.  */

gimple
gimplify_body (tree *body_p, tree fndecl, bool do_parms)
{
  location_t saved_location = input_location;
  struct gimple_sequence parm_stmts, seq;
  gimple outer_bind;

  timevar_push (TV_TREE_GIMPLIFY);

  gcc_assert (gimplify_ctxp == NULL);
  push_gimplify_context ();

  /* Unshare most shared trees in the body and in that of any nested functions.
     It would seem we don't have to do this for nested functions because
     they are supposed to be output and then the outer function gimplified
     first, but the g++ front end doesn't always do it that way.  */
  unshare_body (body_p, fndecl);
  unvisit_body (body_p, fndecl);

  /* Make sure input_location isn't set to something wierd.  */
  input_location = DECL_SOURCE_LOCATION (fndecl);

  /* Resolve callee-copies.  This has to be done before processing
     the body so that DECL_VALUE_EXPR gets processed correctly.  */
  if (do_parms)
    parm_stmts = gimplify_parameters ();
  else
    gimple_seq_init (&parm_stmts);

  /* Gimplify the function's body.  */
  gimple_seq_init (&seq);
  gimplify_stmt (body_p, &seq);

  outer_bind = gimple_seq_first (&seq);
  if (!outer_bind)
    {
      outer_bind = build_gimple_nop ();
      gimple_seq_add (&seq, outer_bind);
    }

  /* If there isn't an outer GIMPLE_BIND, add one.  */
  if (gimple_code (outer_bind) != GIMPLE_BIND)
    outer_bind = build_gimple_bind (NULL_TREE, &seq);

  *body_p = NULL_TREE;

  /* If we had callee-copies statements, insert them at the beginning
     of the function.  */
  if (!gimple_seq_empty_p (&parm_stmts))
    {
      gimple_seq_append (&parm_stmts, gimple_bind_body (outer_bind));
      gimple_bind_set_body (outer_bind, &parm_stmts);
    }

  pop_gimplify_context (outer_bind);
  gcc_assert (gimplify_ctxp == NULL);

#ifdef ENABLE_TYPES_CHECKING
  if (!errorcount && !sorrycount)
    verify_types_in_gimple_seq (gimple_bind_body (outer_bind));
#endif

  timevar_pop (TV_TREE_GIMPLIFY);
  input_location = saved_location;

  return outer_bind;
}

/* Entry point to the gimplification pass.  FNDECL is the FUNCTION_DECL
   node for the function we want to gimplify.
   
   Returns the sequence of GIMPLE statements corresponding to the body
   of FNDECL.  */

void
gimplify_function_tree (tree fndecl)
{
  tree oldfn, parm, ret;
  gimple_seq seq;
  gimple bind;

  oldfn = current_function_decl;
  current_function_decl = fndecl;
  cfun = DECL_STRUCT_FUNCTION (fndecl);
  if (cfun == NULL)
    allocate_struct_function (fndecl);

  for (parm = DECL_ARGUMENTS (fndecl); parm ; parm = TREE_CHAIN (parm))
    {
      /* Preliminarily mark non-addressed complex variables as eligible
         for promotion to gimple registers.  We'll transform their uses
         as we find them.  */
      if ((TREE_CODE (TREE_TYPE (parm)) == COMPLEX_TYPE
	   || TREE_CODE (TREE_TYPE (parm)) == VECTOR_TYPE)
          && !TREE_THIS_VOLATILE (parm)
          && !needs_to_live_in_memory (parm))
        DECL_GIMPLE_REG_P (parm) = 1;
    }

  ret = DECL_RESULT (fndecl);
  if ((TREE_CODE (TREE_TYPE (ret)) == COMPLEX_TYPE
	   || TREE_CODE (TREE_TYPE (ret)) == VECTOR_TYPE)
      && !needs_to_live_in_memory (ret))
    DECL_GIMPLE_REG_P (ret) = 1;

  bind = gimplify_body (&DECL_SAVED_TREE (fndecl), fndecl, true);
  seq = gimple_bind_body (bind);

  /* If we're instrumenting function entry/exit, then prepend the call to
     the entry hook and wrap the whole function in a TRY_FINALLY_EXPR to
     catch the exit hook.  */
  /* ??? Add some way to ignore exceptions for this TFE.  */
  if (flag_instrument_function_entry_exit
      && !DECL_NO_INSTRUMENT_FUNCTION_ENTRY_EXIT (fndecl)
      && !flag_instrument_functions_exclude_p (fndecl))
    {
      tree x;
      gimple tf;

      tf = build_gimple_try (seq, NULL, GIMPLE_TRY_FINALLY);
      x = implicit_built_in_decls[BUILT_IN_PROFILE_FUNC_EXIT];
      gimple_seq_add (gimple_try_cleanup (tf), build_gimple_call (x, 0));

      bind = build_gimple_bind (NULL, NULL);
      x = implicit_built_in_decls[BUILT_IN_PROFILE_FUNC_ENTER];
      gimple_seq_add (gimple_bind_body (bind), build_gimple_call (x, 0));
      gimple_seq_add (gimple_bind_body (bind), tf);
    }

  /* The tree body of the function is no longer needed, replace it
     with the new GIMPLE body.  */
  seq = gimple_seq_alloc ();
  gimple_seq_add (seq, bind);
  set_gimple_body (fndecl, seq);
  DECL_SAVED_TREE (fndecl) = NULL_TREE;

  current_function_decl = oldfn;
  cfun = oldfn ? DECL_STRUCT_FUNCTION (oldfn) : NULL;
}


/* Expands EXPR to list of gimple statements STMTS.  If SIMPLE is true,
   force the result to be either ssa_name or an invariant, otherwise
   just force it to be a rhs expression.  If VAR is not NULL, make the
   base variable of the final destination be VAR if suitable.  */

tree
force_gimple_operand (tree expr, gimple_seq stmts, bool simple, tree var)
{
  tree t;
  enum gimplify_status ret;
  gimple_predicate gimple_test_f;

  gimple_seq_init (stmts);

  if (is_gimple_val (expr))
    return expr;

  gimple_test_f = simple ? is_gimple_val : is_gimple_reg_rhs;

  push_gimplify_context ();
  gimplify_ctxp->into_ssa = gimple_in_ssa_p (cfun);

  if (var)
    expr = build_gimple_modify_stmt (var, expr);

  ret = gimplify_expr (&expr, stmts, NULL, gimple_test_f, fb_rvalue);
  gcc_assert (ret != GS_ERROR);

  if (gimple_referenced_vars (cfun))
    {
      for (t = gimplify_ctxp->temps; t ; t = TREE_CHAIN (t))
	add_referenced_var (t);
    }

  pop_gimplify_context (NULL);

  return expr;
}

/* Invokes force_gimple_operand for EXPR with parameters SIMPLE_P and VAR.  If
   some statements are produced, emits them at BSI.  If BEFORE is true.
   the statements are appended before BSI, otherwise they are appended after
   it.  M specifies the way BSI moves after insertion (BSI_SAME_STMT or
   BSI_CONTINUE_LINKING are the usual values).  */

tree
force_gimple_operand_bsi (block_stmt_iterator *bsi ATTRIBUTE_UNUSED, tree expr,
			  bool simple_p, tree var, bool before ATTRIBUTE_UNUSED,
			  enum bsi_iterator_update m ATTRIBUTE_UNUSED)
{
  struct gimple_sequence stmts;

  expr = force_gimple_operand (expr, &stmts, simple_p, var);

  if (!gimple_seq_empty_p (&stmts))
    {
      if (gimple_in_ssa_p (cfun))
	{
	  gimple_stmt_iterator *gsi;

	  for (gsi = gsi_start (&stmts); !gsi_end_p (gsi); gsi_next (gsi))
	    mark_symbols_for_renaming (gsi_stmt (gsi));
	}

      if (before)
	bsi_insert_seq_before (bsi, &stmts, m);
      else
	bsi_insert_seq_after (bsi, &stmts, m);
    }

  return expr;
}

#include "gt-gimplify.h"
