/* Tree lowering pass.  This pass converts the GENERIC functions-as-trees
   tree representation into the GIMPLE form.

   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   Major work done by Sebastian Pop <s.pop@laposte.net>,
   Diego Novillo <dnovillo@redhat.com> and Jason Merrill <jason@redhat.com>.

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "errors.h"
#include "varray.h"
#include "tree-simple.h"
#include "tree-inline.h"
#include "diagnostic.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "tree-flow.h"
#include "timevar.h"
#include "except.h"
#include "hashtab.h"
#include "flags.h"
#include "real.h"

static void gimplify_constructor (tree, tree *, tree *);
static void gimplify_array_ref (tree *, tree *, tree *, int);
static void gimplify_array_ref_to_plus (tree *, tree *, tree *);
static void gimplify_compound_lval (tree *, tree *, tree *, int);
static void gimplify_call_expr (tree *, tree *, tree *, int (*) (tree));
static void gimplify_modify_expr (tree *, tree *, tree *, int);
static void gimplify_compound_expr (tree *, tree *);
static void gimplify_save_expr (tree *, tree *, tree *);
static void gimplify_addr_expr (tree *, tree *, tree *);
static void gimplify_self_mod_expr (tree *, tree *, tree *, int);
static void gimplify_cond_expr (tree *, tree *, tree);
static void gimplify_boolean_expr (tree *);
static void gimplify_return_expr (tree, tree *);
static tree build_addr_expr (tree);
static tree build_addr_expr_with_type (tree, tree);
static tree add_stmt_to_compound (tree, tree);
static void gimplify_asm_expr (tree *, tree *, tree *);
static void gimplify_bind_expr (tree *, tree *);
static inline void remove_suffix (char *, int);
static void push_gimplify_context (void);
static void pop_gimplify_context (void);
static tree copy_if_shared_r (tree *, int *, void *);
static tree unmark_visited_r (tree *, int *, void *);
static tree mostly_copy_tree_r (tree *, int *, void *);
static void gimplify_target_expr (tree *, tree *, tree *);
static void gimplify_cleanup_point_expr (tree *, tree *);
static bool gimple_conditional_context (void);
static void gimple_push_condition (void);
static void gimple_pop_condition (tree *);
static void gimple_push_cleanup (tree, tree *);
static void gimplify_loop_expr (tree *);
static void gimplify_exit_expr (tree *);
static void gimplify_switch_expr (tree *, tree *);
static void gimple_add_case_label (tree);
static void gimplify_labeled_block_expr (tree *);
static void gimplify_exit_block_expr (tree *);
static hashval_t gimple_tree_hash (const void *);
static int gimple_tree_eq (const void *, const void *);
static tree lookup_tmp_var (tree, bool);
static tree internal_get_tmp_var (tree, tree *, tree *, bool);
static tree shortcut_cond_expr (tree);
static tree gimple_boolify (tree);
static void gimplify_conversion (tree *);
static int gimplify_init_constructor (tree *, tree *, int);
static void gimplify_minimax_expr (tree *, tree *, tree *);
static void build_stack_save_restore (tree *, tree *);

static struct gimplify_ctx
{
  tree current_bind_expr;
  bool save_stack;
  tree temps;
  tree conditional_cleanups;
  int conditions;
  tree exit_label;
  varray_type case_labels;
  /* The formal temporary table.  Should this be persistent?  */
  htab_t temp_htab;
} *gimplify_ctxp;

static void
push_gimplify_context (void)
{
  if (gimplify_ctxp)
    abort ();
  gimplify_ctxp
    = (struct gimplify_ctx *) xcalloc (1, sizeof (struct gimplify_ctx));
  gimplify_ctxp->temp_htab
    = htab_create (1000, gimple_tree_hash, gimple_tree_eq, free);
}

static void
pop_gimplify_context (void)
{
  if (!gimplify_ctxp || gimplify_ctxp->current_bind_expr)
    abort ();
#if 0
  if (!quiet_flag)
    fprintf (stderr, " collisions: %f ",
	     htab_collisions (gimplify_ctxp->temp_htab));
#endif
  htab_delete (gimplify_ctxp->temp_htab);
  free (gimplify_ctxp);
  gimplify_ctxp = NULL;
}

void
gimple_push_bind_expr (tree bind)
{
  TREE_CHAIN (bind) = gimplify_ctxp->current_bind_expr;
  gimplify_ctxp->current_bind_expr = bind;
}

void
gimple_pop_bind_expr (void)
{
  gimplify_ctxp->current_bind_expr
    = TREE_CHAIN (gimplify_ctxp->current_bind_expr);
}

tree
gimple_current_bind_expr (void)
{
  return gimplify_ctxp->current_bind_expr;
}

/* Returns true iff there is a COND_EXPR between us and the innermost
   CLEANUP_POINT_EXPR.  This info is used by gimple_push_cleanup.  */

static bool
gimple_conditional_context (void)
{
  return gimplify_ctxp->conditions > 0;
}

/* Note that we've entered a COND_EXPR.  */

static void
gimple_push_condition (void)
{
  ++(gimplify_ctxp->conditions);
}

/* Note that we've left a COND_EXPR.  If we're back at unconditional scope
   now, add any conditional cleanups we've seen to the prequeue.  */

static void
gimple_pop_condition (tree *pre_p)
{
  int conds = --(gimplify_ctxp->conditions);
  if (conds == 0)
    {
      add_tree (gimplify_ctxp->conditional_cleanups, pre_p);
      gimplify_ctxp->conditional_cleanups = NULL_TREE;
    }
  else if (conds < 0)
    abort ();
}

/* Return nonzero if we should keep FNDECL in gimple form during inlining.  */

int
keep_function_tree_in_gimple_form (tree fndecl)
{
  tree fnbody;

  /* If the program has had errors, then keeping gimple form is not
     necessary.  */
  if (errorcount || sorrycount)
    return 0;

  /* If the front-end does not support gimplification, then this
     function was never in gimple form to begin with.  */
  if (lang_hooks.gimplify_expr == lhd_gimplify_expr)
    return 0;

  /* If the function has no body, then we consider it not in gimple
     form.  */
  fnbody = DECL_SAVED_TREE (fndecl);
  if (fnbody == NULL_TREE)
    return 0;

  return 1;
}

/*  Entry point to the gimplification pass.  FNDECL is the FUNCTION_DECL
    node for the function we want to gimplify.  */

int
gimplify_function_tree (tree fndecl)
{
  int done;
  tree fnbody;
  tree oldfn;

  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return 0;

  /* FIXME.  Hack.  If this front end does not support gimplification,
     do nothing.  */
  if (lang_hooks.gimplify_expr == lhd_gimplify_expr)
    return 0;

  fnbody = DECL_SAVED_TREE (fndecl);
  if (fnbody == NULL_TREE)
    return 0;

  oldfn = current_function_decl;
  current_function_decl = fndecl;

  done = gimplify_body (&fnbody, fndecl);

  DECL_SAVED_TREE (fndecl) = fnbody;
  current_function_decl = oldfn;

  return done;
}


/* Gimplify the body of statements pointed by BODY_P.  FNDECL is the
   function decl containing BODY.  */

int
gimplify_body (tree *body_p, tree fndecl)
{
  int done;
  location_t saved_location = input_location;

  timevar_push (TV_TREE_GIMPLIFY);
  push_gimplify_context ();

  /* Unshare most shared trees in the body.  */
  unshare_all_trees (*body_p);

  /* Make sure input_location isn't set to something wierd.  */
  input_location = DECL_SOURCE_LOCATION (fndecl);

  /* Gimplify the function's body.  */
  done = gimplify_stmt (body_p);

  /* Unshare again, in case gimplification was sloppy.  */
  unshare_all_trees (*body_p);

  /* If there isn't an outer BIND_EXPR, add one.  */
  if (TREE_CODE (*body_p) != BIND_EXPR)
    {
      *body_p = build (BIND_EXPR, void_type_node, NULL_TREE,
		       *body_p, NULL_TREE);
      TREE_SIDE_EFFECTS (*body_p) = 1;
    }

  /* Declare the new temporary variables.  */
  declare_tmp_vars (gimplify_ctxp->temps, *body_p);

  pop_gimplify_context ();
  timevar_pop (TV_TREE_GIMPLIFY);
  input_location = saved_location;

  return done;
}


/* Gimplification of expression trees.  */

/* Gimplify an expression which appears at statement context; usually, this
   means replacing it with a suitably gimple COMPOUND_EXPR.  */

int
gimplify_stmt (tree *stmt_p)
{
  return gimplify_expr (stmt_p, NULL, NULL, is_gimple_stmt, fb_none);
}

/*  Gimplifies the expression tree pointed by EXPR_P.  Return 0 if
    gimplification failed.

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
	EXPR should be stored, or NULL if there is no suitable list.  In
	that case, we copy the result to a temporary, emit the
	post-effects, and then return the temporary.

    GIMPLE_TEST_F points to a function that takes a tree T and
	returns nonzero if T is in the GIMPLE form requested by the
	caller.  The GIMPLE predicates are in tree-simple.c.

	This test is used twice.  Before gimplification, the test is
	invoked to determine whether *EXPR_P is already gimple enough.  If
	that fails, *EXPR_P is gimplified according to its code and
	GIMPLE_TEST_F is called again.  If the test still fails, then a new
	temporary variable is created and assigned the value of the
	gimplified expression.

    FALLBACK tells the function what sort of a temporary we want.  If the 1
        bit is set, an rvalue is OK.  If the 2 bit is set, an lvalue is OK.
        If both are set, either is OK, but an lvalue is preferable.  */

bool
gimplify_expr (tree *expr_p, tree *pre_p, tree *post_p,
	       int (* gimple_test_f) (tree), fallback_t fallback)
{
  tree tmp;
  tree internal_pre = NULL_TREE;
  tree internal_post = NULL_TREE;
  tree save_expr;
  int is_statement = (pre_p == NULL);
  location_t *locus;
  location_t saved_location;
  bool ret_ok = true;

  if (*expr_p == NULL_TREE)
    return true;

  /* Go ahead and strip type nops before we test our predicate.  */
  STRIP_MAIN_TYPE_NOPS (*expr_p);

  /* We used to check the predicate here and return immediately if it
     succeeds.  This is wrong; the design is for gimplification to be
     idempotent, and for the predicates to only test for valid forms, not
     whether they are fully simplified.  */

  /* Set up our internal queues if needed.  */
  if (pre_p == NULL)
    pre_p = &internal_pre;
  if (post_p == NULL)
    post_p = &internal_post;

  saved_location = input_location;
  locus = EXPR_LOCUS (*expr_p);
  if (locus)
    input_location = *locus;

  /* Loop over the specific gimplifiers until the toplevel node remains the
     same.  */
  do
    {
      /* Strip any uselessness.  */
      STRIP_MAIN_TYPE_NOPS (*expr_p);

      /* Remember the expr.  */
      save_expr = *expr_p;

      /* Do any language-specific gimplification.  */
      (*lang_hooks.gimplify_expr) (expr_p, pre_p, post_p);
      if (*expr_p == NULL_TREE)
	break;
      if (*expr_p != save_expr)
	continue;

      switch (TREE_CODE (*expr_p))
	{
	  /* First deal with the special cases.  */

	case POSTINCREMENT_EXPR:
	case POSTDECREMENT_EXPR:
	case PREINCREMENT_EXPR:
	case PREDECREMENT_EXPR:
	  gimplify_self_mod_expr (expr_p, pre_p, post_p, fallback != fb_none);
	  break;

	case ARRAY_REF:
	  gimplify_array_ref (expr_p, pre_p, post_p, fallback & fb_lvalue);
	  break;

	case COMPONENT_REF:
	  gimplify_compound_lval (expr_p, pre_p, post_p, fallback & fb_lvalue);
	  break;

	case COND_EXPR:
	  gimplify_cond_expr (expr_p, pre_p, NULL_TREE);
	  break;

	case CALL_EXPR:
	  gimplify_call_expr (expr_p, pre_p, post_p, gimple_test_f);
	  break;

	case TREE_LIST:
	  abort ();

	case COMPOUND_EXPR:
	  if (is_statement)
	    foreach_stmt (expr_p, (foreach_stmt_fn *)gimplify_stmt);
	  else
	    gimplify_compound_expr (expr_p, pre_p);
	  break;

	case REALPART_EXPR:
	case IMAGPART_EXPR:
	  gimplify_compound_lval (expr_p, pre_p, post_p, fallback & fb_lvalue);
	  break;

	case MODIFY_EXPR:
	case INIT_EXPR:
	  gimplify_modify_expr (expr_p, pre_p, post_p, fallback != fb_none);
	  break;

	case TRUTH_ANDIF_EXPR:
	case TRUTH_ORIF_EXPR:
	  gimplify_boolean_expr (expr_p);
	  break;

	case TRUTH_NOT_EXPR:
	  tmp = gimple_boolify (TREE_OPERAND (*expr_p, 0));
	  *expr_p = build (EQ_EXPR, TREE_TYPE (*expr_p), tmp,
			   boolean_false_node);
	  recalculate_side_effects (*expr_p);
	  break;

	case ADDR_EXPR:
	  gimplify_addr_expr (expr_p, pre_p, post_p);
	  break;

	/* va_arg expressions are in GIMPLE form already.  */
	case VA_ARG_EXPR:
	  break;

	case CONVERT_EXPR:
	case NOP_EXPR:
	  if (IS_EMPTY_STMT (*expr_p))
	    break;

	  if (VOID_TYPE_P (TREE_TYPE (*expr_p)))
	    {
	      /* Just strip a conversion to void and try again.  */
	      *expr_p = TREE_OPERAND (*expr_p, 0);
	      break;
	    }

	  gimplify_conversion (expr_p);
	  if (*expr_p != save_expr)
	    break;

	case FIX_TRUNC_EXPR:
	case FIX_CEIL_EXPR:
	case FIX_FLOOR_EXPR:
	case FIX_ROUND_EXPR:
	  /* unary_expr: ... | '(' cast ')' val | ...  */
	  gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			 is_gimple_val, fb_rvalue);
	  recalculate_side_effects (*expr_p);
	  break;

	case INDIRECT_REF:
	  gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			 is_gimple_reg, fb_rvalue);

	  recalculate_side_effects (*expr_p);
	  break;

	  /* Constants need not be gimplified.  */
	case INTEGER_CST:
	case REAL_CST:
	case STRING_CST:
	case COMPLEX_CST:
	case VECTOR_CST:
	  break;

	case CONST_DECL:
	  *expr_p = DECL_INITIAL (*expr_p);
	  break;

	  /* FIXME make this a decl.  */
	case EXC_PTR_EXPR:
	  break;

	case BIND_EXPR:
	  gimplify_bind_expr (expr_p, pre_p);
	  break;

	case LOOP_EXPR:
	  gimplify_loop_expr (expr_p);
	  break;

	case SWITCH_EXPR:
	  gimplify_switch_expr (expr_p, pre_p);
	  break;

	case LABELED_BLOCK_EXPR:
	  gimplify_labeled_block_expr (expr_p);
	  break;

	case EXIT_BLOCK_EXPR:
	  gimplify_exit_block_expr (expr_p);
	  break;

	case EXIT_EXPR:
	  gimplify_exit_expr (expr_p);
	  break;

	case GOTO_EXPR:
	  {
	    tree dest = GOTO_DESTINATION (*expr_p);

	    /* If the target is not LABEL, then it is a computed jump
	       and the target needs to be gimplified.  */
	    if (TREE_CODE (GOTO_DESTINATION (*expr_p)) != LABEL_DECL)
	      gimplify_expr (&GOTO_DESTINATION (*expr_p), pre_p, NULL,
			     is_gimple_val, fb_rvalue);

	    /* If this label is in a different context (function), then
	       mark it as a nonlocal label and mark its context as
	       receiving nonlocal gotos.  */
	    else if (current_function_decl != decl_function_context (dest))
	      {
		tree context = decl_function_context (dest);

		NONLOCAL_LABEL (dest) = 1;
		FUNCTION_RECEIVES_NONLOCAL_GOTO (context) = 1;
	      }

	    break;
	  }

	case LABEL_EXPR:
	  break;

	case CASE_LABEL_EXPR:
	  gimple_add_case_label (*expr_p);
	  break;

	case RETURN_EXPR:
	  gimplify_return_expr (*expr_p, pre_p);
	  break;

	case CONSTRUCTOR:
	  /* Don't reduce this in place; let gimplify_init_constructor work
	     its magic.  */
	  break;

	  /* The following are special cases that are not handled by the
	     original GIMPLE grammar.  */

	  /* SAVE_EXPR nodes are converted into a GIMPLE identifier and
	     eliminated.  */
	case SAVE_EXPR:
	  gimplify_save_expr (expr_p, pre_p, post_p);
	  break;

	case BIT_FIELD_REF:
	  gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
		         is_gimple_min_lval, fb_either);
	  gimplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p,
			 is_gimple_val, fb_rvalue);
	  gimplify_expr (&TREE_OPERAND (*expr_p, 2), pre_p, post_p,
			 is_gimple_val, fb_rvalue);
	  recalculate_side_effects (*expr_p);
	  break;

	case NON_LVALUE_EXPR:
	  /* This should have been stripped above.  */
	  abort ();
	  break;

	case ASM_EXPR:
	  gimplify_asm_expr (expr_p, pre_p, post_p);
	  break;

	case TRY_FINALLY_EXPR:
	case TRY_CATCH_EXPR:
	  gimplify_stmt (&TREE_OPERAND (*expr_p, 0));
	  gimplify_stmt (&TREE_OPERAND (*expr_p, 1));
	  break;

	case CLEANUP_POINT_EXPR:
	  gimplify_cleanup_point_expr (expr_p, pre_p);
	  break;

	case TARGET_EXPR:
	  gimplify_target_expr (expr_p, pre_p, post_p);
	  break;

	case CATCH_EXPR:
	  gimplify_stmt (&CATCH_BODY (*expr_p));
	  break;

	case EH_FILTER_EXPR:
	  gimplify_stmt (&EH_FILTER_FAILURE (*expr_p));
	  break;

	case VTABLE_REF:
	  /* This moves much of the actual computation out of the
	     VTABLE_REF.  Perhaps this should be revisited once we want to
	     do clever things with VTABLE_REFs.  */
	  gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			 is_gimple_min_lval, fb_lvalue);
	  break;

	case MIN_EXPR:
	case MAX_EXPR:
	  gimplify_minimax_expr (expr_p, pre_p, post_p);

	case LABEL_DECL:
	  /* We get here when taking the address of a label.  We mark
	     the label as "forced"; meaning it can never be removed and
	     it is a potential target for any computed goto.  */
	  FORCED_LABEL (*expr_p) = 1;
	  break;

	default:
	  /* If *EXPR_P does not need to be special-cased, handle it
	     according to its class.  */
	  {
	    if (TREE_CODE_CLASS (TREE_CODE (*expr_p)) == '1')
	      {
		gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_gimple_val, fb_rvalue);
		recalculate_side_effects (*expr_p);
	      }
	    else if (TREE_CODE_CLASS (TREE_CODE (*expr_p)) == '2'
		     || TREE_CODE_CLASS (TREE_CODE (*expr_p)) == '<'
		     || TREE_CODE (*expr_p) == TRUTH_AND_EXPR
		     || TREE_CODE (*expr_p) == TRUTH_OR_EXPR
		     || TREE_CODE (*expr_p) == TRUTH_XOR_EXPR)
	      {
		/* binary_expr : val binop val  */
		gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_gimple_val, fb_rvalue);

		gimplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p,
			       is_gimple_val, fb_rvalue);
		recalculate_side_effects (*expr_p);
	      }
	    else if (TREE_CODE_CLASS (TREE_CODE (*expr_p)) == 'd'
		     || TREE_CODE_CLASS (TREE_CODE (*expr_p)) == 'c')
	      /* OK */;
	    else
	      /* Fail if we don't know how to handle this tree code.  */
	      abort ();
	  }
	}
    }
  /* If we replaced *expr_p, gimplify again.  */
  while (*expr_p && *expr_p != save_expr);

  if (fallback == fb_none && !is_gimple_stmt (*expr_p))
    {
      /* We aren't looking for a value, and we don't have a valid
	 statement.  If it doesn't have side-effects, throw it away.  */
      if (!TREE_SIDE_EFFECTS (*expr_p))
	*expr_p = build_empty_stmt ();
      else if (!TREE_THIS_VOLATILE (*expr_p))
	/* We only handle volatiles here; anything else with side-effects
	   must be converted to a valid statement before we get here.  */
	abort ();
      else if (COMPLETE_TYPE_P (TREE_TYPE (*expr_p)))
	{
	  /* Historically, the compiler has treated a bare
	     reference to a volatile lvalue as forcing a load.  */
	  tree tmp = create_tmp_var (TREE_TYPE (*expr_p), "vol");
	  *expr_p = build (MODIFY_EXPR, TREE_TYPE (tmp), tmp, *expr_p);
	}
      else
	/* We can't do anything useful with a volatile reference to
	   incomplete type, so just throw it away.  */
	*expr_p = build_empty_stmt ();
    }

  /* If we are gimplifying at the statement level, we're done.  Tack
     everything together and replace the original statement with the
     gimplified form.  */
  if (is_statement)
    {
      add_tree (*expr_p, pre_p);
      annotate_all_with_locus (&internal_post, input_location);
      add_tree (internal_post, pre_p);
      tmp = rationalize_compound_expr (internal_pre);
      annotate_all_with_locus (&tmp, input_location);
      *expr_p = tmp;

      goto out;
    }

  /* Otherwise we're gimplifying a subexpression, so the resulting value is
     interesting.  */

  /* If it's sufficiently simple already, we're done.  Unless we are
     handling some post-effects internally; if that's the case, we need to
     copy into a temp before adding the post-effects to the tree.  */
  if (!internal_post && (*gimple_test_f) (*expr_p))
    goto out;

  /* Otherwise, we need to create a new temporary for the gimplified
     expression.  */

  /* We can't return an lvalue if we have an internal postqueue.  The
     object the lvalue refers to would (probably) be modified by the
     postqueue; we need to copy the value out first, which means an
     rvalue.  */
  if ((fallback & fb_lvalue) && !internal_post
      && is_gimple_addr_expr_arg (*expr_p))
    {
      /* An lvalue will do.  Take the address of the expression, store it
	 in a temporary, and replace the expression with an INDIRECT_REF of
	 that temporary.  */
      tmp = build_addr_expr (*expr_p);
      gimplify_expr (&tmp, pre_p, post_p, is_gimple_reg, fb_rvalue);
      *expr_p = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (tmp)), tmp);
    }
  else if ((fallback & fb_rvalue) && is_gimple_rhs (*expr_p))
    {
#if defined ENABLE_CHECKING
      if (VOID_TYPE_P (TREE_TYPE (*expr_p)))
	abort ();
#endif

      /* An rvalue will do.  Assign the gimplified expression into a new
	 temporary TMP and replace the original expression with TMP.  */

      if (internal_post || (fallback & fb_lvalue))
	/* The postqueue might change the value of the expression between
	   the initialization and use of the temporary, so we can't use a
	   formal temp.  FIXME do we care?  */
	*expr_p = get_initialized_tmp_var (*expr_p, pre_p, post_p);
      else
	*expr_p = get_formal_tmp_var (*expr_p, pre_p);
    }
  else if (fallback & fb_mayfail)
    {
      /* If this is an asm statement, and the user asked for the impossible,
	 don't abort.  Fail and let gimplify_asm_expr issue an error.  */
      ret_ok = false;
      goto out;
    }
  else
    {
      fprintf (stderr, "gimplification failed:\n");
      print_generic_expr (stderr, *expr_p, 0);
      debug_tree (*expr_p);
      abort ();
    }

#if defined ENABLE_CHECKING
  /* Make sure the temporary matches our predicate.  */
  if (!(*gimple_test_f) (*expr_p))
    abort ();
#endif

  if (internal_post)
    {
      annotate_all_with_locus (&internal_post, input_location);
      add_tree (internal_post, pre_p);
    }

 out:
  input_location = saved_location;
  return ret_ok;
}

/* WRAPPER is a code such as BIND_EXPR or CLEANUP_POINT_EXPR which can both
   contain statements and have a value.  Assign its value to a temporary
   and give it void_type_node.  Returns the temporary, or NULL_TREE if
   WRAPPER was already void.  */

tree
voidify_wrapper_expr (tree wrapper)
{
  if (!VOID_TYPE_P (TREE_TYPE (wrapper)))
    {
      tree *p;
      tree temp;

      /* Set p to point to the body of the wrapper.  */
      switch (TREE_CODE (wrapper))
	{
	case BIND_EXPR:
	  /* For a BIND_EXPR, the body is operand 1.  */
	  p = &BIND_EXPR_BODY (wrapper);
	  break;

	default:
	  p = &TREE_OPERAND (wrapper, 0);
	  break;
	}

      for (; TREE_CODE (*p) == COMPOUND_EXPR;
	   p = &TREE_OPERAND (*p, 1))
	{
	  /* Advance.  Set up the COMPOUND_EXPRs appropriately for what we
	     will have when we're done.  */
	  TREE_SIDE_EFFECTS (*p) = 1;
	  TREE_TYPE (*p) = void_type_node;
	}

      if (TREE_CODE (*p) == INIT_EXPR)
	{
	  /* The C++ frontend already did this for us.  */;
	  temp = TREE_OPERAND (*p, 0);
	}
      else if (TREE_CODE (*p) == INDIRECT_REF)
	{
	  /* If we're returning a dereference, move the dereference outside
	     the wrapper.  */
	  tree ptr = TREE_OPERAND (*p, 0);
	  temp = create_tmp_var (TREE_TYPE (ptr), "retval");
	  *p = build (MODIFY_EXPR, TREE_TYPE (ptr), temp, ptr);
	  temp = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (temp)), temp);
	  /* If this is a BIND_EXPR for a const inline function, it might not
	     have TREE_SIDE_EFFECTS set.  That is no longer accurate.  */
	  TREE_SIDE_EFFECTS (wrapper) = 1;
	}
      else
	{
	  temp = create_tmp_var (TREE_TYPE (wrapper), "retval");
	  if (!IS_EMPTY_STMT (*p))
	    {
	      *p = build (MODIFY_EXPR, TREE_TYPE (temp), temp, *p);
	      TREE_SIDE_EFFECTS (wrapper) = 1;
	    }
	}

      TREE_TYPE (wrapper) = void_type_node;
      return temp;
    }

  return NULL_TREE;
}

/* Gimplify a BIND_EXPR.  Just voidify and recurse.  */

static void
gimplify_bind_expr (tree *expr_p, tree *pre_p)
{
  tree bind_expr = *expr_p;
  tree temp = voidify_wrapper_expr (bind_expr);
  bool old_save_stack = gimplify_ctxp->save_stack;

  gimple_push_bind_expr (bind_expr);
  gimplify_ctxp->save_stack = false;

  gimplify_stmt (&BIND_EXPR_BODY (bind_expr));
  if (gimplify_ctxp->save_stack)
    {
      tree stack_save, stack_restore;

      /* Save stack on entry and restore it on exit.  Add a try_finally
	 block to achieve this.  */
      build_stack_save_restore (&stack_save, &stack_restore);
      BIND_EXPR_BODY (bind_expr) =
	  build (COMPOUND_EXPR, void_type_node,
		 stack_save,
		 build (TRY_FINALLY_EXPR, void_type_node,
			BIND_EXPR_BODY (bind_expr),
			stack_restore));
    }

  gimplify_ctxp->save_stack = old_save_stack;
  gimple_pop_bind_expr ();

  if (temp)
    {
      *expr_p = temp;
      add_tree (bind_expr, pre_p);
    }

#if 0
  if (!BIND_EXPR_VARS (bind_expr))
    *expr_p = BIND_EXPR_BODY (bind_expr);
  else
    recalculate_side_effects (bind_expr);
#endif
}

/* Gimplify a RETURN_EXPR.  If the expression to be returned is not a
   GIMPLE value, it is assigned to a new temporary and the statement is
   re-written to return the temporary.

   PRE_P points to the list where side effects that must happen before
   STMT should be stored.  */

static void
gimplify_return_expr (tree stmt, tree *pre_p)
{
  tree ret_expr = TREE_OPERAND (stmt, 0);
  tree_stmt_iterator si;

  if (ret_expr && TREE_CODE (ret_expr) != RESULT_DECL)
    {
      tree result;

      if (VOID_TYPE_P (TREE_TYPE (TREE_TYPE (current_function_decl))))
	result = NULL_TREE;
      else
	{
	  result = TREE_OPERAND (ret_expr, 0);
#ifdef ENABLE_CHECKING
	  if ((TREE_CODE (ret_expr) != MODIFY_EXPR
	       && TREE_CODE (ret_expr) != INIT_EXPR)
	      || TREE_CODE (result) != RESULT_DECL)
	    abort ();
#endif
	}

      /* We need to pass the full MODIFY_EXPR down so that special handling
	 can replace it with something else.  */
      gimplify_stmt (&ret_expr);

      if (result == NULL_TREE)
	TREE_OPERAND (stmt, 0) = NULL_TREE;
      else if (ret_expr == TREE_OPERAND (stmt, 0))
	/* It was already GIMPLE.  */
	return;
      else
	{
	  /* If there's still a MODIFY_EXPR of the RESULT_DECL after
	     gimplification, find it so we can put it in the RETURN_EXPR.  */
	  tree ret = NULL_TREE;

	  for (si = tsi_start (&ret_expr); !tsi_end_p (si); tsi_next (&si))
	    {
	      tree sub = tsi_stmt (si);
	      if (TREE_CODE (sub) == MODIFY_EXPR
		  && TREE_OPERAND (sub, 0) == result)
		{
		  ret = sub;
		  break;
		}
	    }

	  if (ret)
	    {
	      if (tsi_one_before_end_p (si))
		/* If the MODIFY_EXPR was the last thing in the GIMPLE
		   form, pull it out.  */
		tsi_delink (&si);
	      else
		{
		  /* If there were posteffects after the MODIFY_EXPR, we
		     need a temporary.  */
		  tree tmp = create_tmp_var (TREE_TYPE (result), "retval");
		  TREE_OPERAND (ret, 0) = tmp;
		  ret = build (MODIFY_EXPR, TREE_TYPE (result), result, tmp);
		}
	    }

	  if (ret)
	    TREE_OPERAND (stmt, 0) = ret;
	  else
	    /* The return value must be set up some other way.  Just tell
	       expand_return that we're returning the RESULT_DECL.  */
	    TREE_OPERAND (stmt, 0) = result;
	}

      add_tree (ret_expr, pre_p);
    }
}

/* Gimplify a LOOP_EXPR.  Normally this just involves gimplifying the body
   and replacing the LOOP_EXPR with goto, but if the loop contains an EXIT_EXPR,
   we need to append a label for it to jump to.  */

static void
gimplify_loop_expr (tree *expr_p)
{
  tree saved_label = gimplify_ctxp->exit_label;
  tree start_label = build1 (LABEL_EXPR, void_type_node, NULL_TREE);

  gimplify_ctxp->exit_label = NULL_TREE;

  gimplify_stmt (&LOOP_EXPR_BODY (*expr_p));
  *expr_p = LOOP_EXPR_BODY (*expr_p);

  add_tree (build_and_jump (&LABEL_EXPR_LABEL (start_label)), expr_p);
  *expr_p = add_stmt_to_compound (start_label, *expr_p);
  if (gimplify_ctxp->exit_label)
    {
      tree expr = build1 (LABEL_EXPR, void_type_node,
			  gimplify_ctxp->exit_label);
      add_tree (expr, expr_p);
    }

  gimplify_ctxp->exit_label = saved_label;
}

/* Gimplify a SWITCH_EXPR, and collect a TREE_VEC of the labels it can
   branch to.  */

static void
gimplify_switch_expr (tree *expr_p, tree *pre_p)
{
  tree label_vec;
  int i, len;
  varray_type labels;
  tree switch_expr = *expr_p;

  varray_type saved_labels = gimplify_ctxp->case_labels;
  if (SWITCH_LABELS (switch_expr))
    /* Don't do this work again.  */
    gimplify_ctxp->case_labels = NULL;
  else
    VARRAY_TREE_INIT (gimplify_ctxp->case_labels, 8, "case_labels");

  /* We don't want to risk changing the type of the switch condition,
     lest stmt.c get the wrong impression about enumerations.  */
  if (TREE_CODE (SWITCH_COND (switch_expr)) == NOP_EXPR)
    gimplify_expr (&TREE_OPERAND (SWITCH_COND (switch_expr), 0), pre_p,
		   NULL, is_gimple_val, fb_rvalue);
  else
    gimplify_expr (&SWITCH_COND (switch_expr), pre_p, NULL,
		   is_gimple_val, fb_rvalue);

  gimplify_stmt (&SWITCH_BODY (switch_expr));

  labels = gimplify_ctxp->case_labels;
  if (labels)
    {
      len = VARRAY_ACTIVE_SIZE (labels);
      label_vec = make_tree_vec (len);
      for (i = 0; i < len; ++i)
	TREE_VEC_ELT (label_vec, i) = VARRAY_TREE (labels, i);
      SWITCH_LABELS (*expr_p) = label_vec;
    }
  gimplify_ctxp->case_labels = saved_labels;
}

static void
gimple_add_case_label (tree expr)
{
  if (gimplify_ctxp->case_labels)
    VARRAY_PUSH_TREE (gimplify_ctxp->case_labels, CASE_LABEL (expr));
}

/* Gimplify a LABELED_BLOCK_EXPR into a LABEL_EXPR following
   a (possibly empty) body.  */

static void
gimplify_labeled_block_expr (tree *expr_p)
{
  tree body = LABELED_BLOCK_BODY (*expr_p);
  tree label = LABELED_BLOCK_LABEL (*expr_p);
  tree t;

  DECL_CONTEXT (label) = current_function_decl;
  t = build (LABEL_EXPR, void_type_node, label);
  if (body != NULL_TREE)
    t = build (COMPOUND_EXPR, void_type_node, body, t);
  *expr_p = t;
}

/* Gimplify a EXIT_BLOCK_EXPR into a GOTO_EXPR.  */

static void
gimplify_exit_block_expr (tree *expr_p)
{
  tree labeled_block = TREE_OPERAND (*expr_p, 0);
  tree label;

  /* First operand must be a LABELED_BLOCK_EXPR, which should
     already be lowered (or partially lowered) when we get here.  */
#if defined ENABLE_CHECKING
  if (TREE_CODE (labeled_block) != LABELED_BLOCK_EXPR)
    abort ();
#endif

  label = LABELED_BLOCK_LABEL (labeled_block);
  *expr_p = build1 (GOTO_EXPR, void_type_node, label);
}

/* Build a GOTO to the LABEL_DECL pointed to by LABEL_P, building it first
   if necessary.  */

tree
build_and_jump (tree *label_p)
{
  if (label_p == NULL)
    /* If there's nowhere to jump, just fall through.  */
    return build_empty_stmt ();

  if (*label_p == NULL_TREE)
    {
      tree label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
      DECL_ARTIFICIAL (label) = 1;
      DECL_CONTEXT (label) = current_function_decl;
      *label_p = label;
    }

  return build1 (GOTO_EXPR, void_type_node, *label_p);
}

/* Gimplify an EXIT_EXPR by converting to a GOTO_EXPR inside a COND_EXPR.
   This also involves building a label to jump to and communicating it to
   gimplify_loop_expr through gimplify_ctxp->exit_label.  */

static void
gimplify_exit_expr (tree *expr_p)
{
  tree cond = TREE_OPERAND (*expr_p, 0);
  tree expr;

  expr = build_and_jump (&gimplify_ctxp->exit_label);
  expr = build (COND_EXPR, void_type_node, cond, expr, build_empty_stmt ());
  *expr_p = expr;
}

/* Gimplifies a CONSTRUCTOR node at *EXPR_P.

     aggr_init: '{' vals '}'
     vals: aggr_init_elt | vals ',' aggr_init_elt
     aggr_init_elt: val | aggr_init  */

static void
gimplify_constructor (tree t, tree *pre_p, tree *post_p)
{
  tree elt_list;

  for (elt_list = CONSTRUCTOR_ELTS (t); elt_list;
       elt_list = TREE_CHAIN (elt_list))
    gimplify_expr (&TREE_VALUE (elt_list), pre_p, post_p,
		   is_gimple_constructor_elt, fb_rvalue);
}

/* Break out elements of a constructor used as an initializer into separate
   MODIFY_EXPRs.

   Note that we still need to clear any elements that don't have explicit
   initializers, so if not all elements are initialized we keep the
   original MODIFY_EXPR, we just remove all of the constructor
   elements.

   FIXME should also handle vectors.  */

static int
gimplify_init_constructor (tree *expr_p, tree *pre_p, int want_value)
{
  tree object = TREE_OPERAND (*expr_p, 0);
  tree ctor = TREE_OPERAND (*expr_p, 1);
  tree type = TREE_TYPE (ctor);

  if (TREE_CODE (ctor) != CONSTRUCTOR)
    return 0;

  if (TREE_CODE (type) == RECORD_TYPE || TREE_CODE (type) == UNION_TYPE
      || TREE_CODE (type) == QUAL_UNION_TYPE
      || TREE_CODE (type) == ARRAY_TYPE)
    {
      tree elt_list = CONSTRUCTOR_ELTS (ctor);

      if (elt_list)
	{
	  int cleared = 0;
	  int len = list_length (elt_list);
	  int i;

	  if (mostly_zeros_p (ctor))
	    cleared = 1;
	  else if (TREE_CODE (type) == ARRAY_TYPE)
	    {
	      tree nelts = array_type_nelts (type);
	      if (TREE_CODE (nelts) != INTEGER_CST
		  || (unsigned)len != TREE_INT_CST_LOW (nelts)+1)
		cleared = 1;
	    }
	  else if (len != fields_length (type))
	    cleared = 1;

	  if (cleared)
	    {
	      CONSTRUCTOR_ELTS (ctor) = NULL_TREE;
	      add_tree (*expr_p, pre_p);
	    }

	  for (i = 0; elt_list; i++, elt_list = TREE_CHAIN (elt_list))
	    {
	      tree purpose, value, cref, init;

	      purpose = TREE_PURPOSE (elt_list);
	      value = TREE_VALUE (elt_list);

	      if (cleared && initializer_zerop (value))
		continue;

	      if (TREE_CODE (type) == ARRAY_TYPE)
		{
		  tree type = TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (object)));
		  cref = build (ARRAY_REF, type, object, build_int_2 (i, 0));
		}
	      else
		{
		  cref = build (COMPONENT_REF, TREE_TYPE (purpose), object, purpose);
		}

	      init = build (MODIFY_EXPR, TREE_TYPE (purpose), cref, value);
	      /* Each member initialization is a full-expression.  */
	      gimplify_stmt (&init);
	      add_tree (init, pre_p);
	    }
	  if (want_value)
	    *expr_p = object;
	  else
	    *expr_p = build_empty_stmt ();

	  return 1;
	}
    }
  else
    gimplify_constructor (ctor, pre_p, NULL);
  return 0;
}

/* *EXPR_P is a NOP_EXPR or CONVERT_EXPR.  Remove it and/or other conversions
   underneath as appropriate.  */

static void
gimplify_conversion (tree *expr_p)
{  
  /* If a NOP conversion is changing the type of a COMPONENT_REF
     expression, then it is safe to force the type of the
     COMPONENT_REF to be the same as the type of the field the
     COMPONENT_REF is accessing.

     This is a profitable thing to do as canonicalization of
     types on COMPONENT_REFs exposes more redundant COMPONENT_REFs.  */
  if (TREE_CODE (TREE_OPERAND (*expr_p, 0)) == COMPONENT_REF)
    {
      TREE_TYPE (TREE_OPERAND (*expr_p, 0))
	= TREE_TYPE (TREE_OPERAND (TREE_OPERAND (*expr_p, 0), 1));
    }

  /* Strip away as many useless type conversions as possible
     at the toplevel.  */
  while (tree_ssa_useless_type_conversion (*expr_p))
    *expr_p = TREE_OPERAND (*expr_p, 0);

  /* If we still have a conversion at the toplevel, then strip
     away all but the outermost conversion.  */
  if (TREE_CODE (*expr_p) == NOP_EXPR
      || TREE_CODE (*expr_p) == CONVERT_EXPR)
    {
      STRIP_SIGN_NOPS (TREE_OPERAND (*expr_p, 0));

      /* And remove the outermost conversion if it's useless.  */
      if (TYPE_MAIN_VARIANT (TREE_TYPE (*expr_p))
	  == TYPE_MAIN_VARIANT (TREE_TYPE (TREE_OPERAND (*expr_p, 0))))
	*expr_p = TREE_OPERAND (*expr_p, 0);
    }
}

/* Reduce MIN/MAX_EXPR to a COND_EXPR for further gimplification.  */

static void
gimplify_minimax_expr (tree *expr_p, tree *pre_p, tree *post_p)
{
  tree op1 = TREE_OPERAND (*expr_p, 0);
  tree op2 = TREE_OPERAND (*expr_p, 1);
  enum tree_code code;

  if (TREE_CODE (*expr_p) == MIN_EXPR)
    code = LE_EXPR;
  else
    code = GE_EXPR;

  gimplify_expr (&op1, pre_p, post_p, is_gimple_val, fb_rvalue);
  gimplify_expr (&op2, pre_p, post_p, is_gimple_val, fb_rvalue);

  *expr_p = build (COND_EXPR, TREE_TYPE (*expr_p),
		   build (code, boolean_type_node, op1, op2),
		   op1, op2);
}

/*  Build an expression for the address of T.  Folds away INDIRECT_REF to
    avoid confusing the gimplify process.  */

static tree
build_addr_expr_with_type (tree t, tree ptrtype)
{
  if (TREE_CODE (t) == INDIRECT_REF)
    {
      t = TREE_OPERAND (t, 0);
      if (TREE_TYPE (t) != ptrtype)
	t = build1 (NOP_EXPR, ptrtype, t);
    }
  else
    t = build1 (ADDR_EXPR, ptrtype, t);

  return t;
}

static tree
build_addr_expr (tree t)
{
  return build_addr_expr_with_type (t, build_pointer_type (TREE_TYPE (t)));
}


/*  Re-write the ARRAY_REF node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
	*EXPR_P should be stored.

    FIXME: ARRAY_REF currently doesn't accept a pointer as the array
    argument, so this gimplification uses an INDIRECT_REF of ARRAY_TYPE.
    ARRAY_REF should be extended.  */

static void
gimplify_array_ref (tree *expr_p, tree *pre_p,
		    tree *post_p, int want_lvalue)
{
  tree elttype = TREE_TYPE (TREE_TYPE (TREE_OPERAND (*expr_p, 0)));
  if (!TREE_CONSTANT (TYPE_SIZE_UNIT (elttype)))
    /* If the size of the array elements is not constant,
       computing the offset is non-trivial, so expose it.  */
    gimplify_array_ref_to_plus (expr_p, pre_p, post_p);
  else
    /* Handle array and member refs together for now.  When alias analysis
       improves, we may want to go back to handling them separately.  */
    gimplify_compound_lval (expr_p, pre_p, post_p, want_lvalue);
}

/* Subroutine of gimplify_compound_lval.  Converts an ARRAY_REF to the
   equivalent *(&array + offset) form.  */

static void
gimplify_array_ref_to_plus (tree *expr_p, tree *pre_p, tree *post_p)
{
  tree array = TREE_OPERAND (*expr_p, 0);
  tree arrtype = TREE_TYPE (array);
  tree elttype = TREE_TYPE (arrtype);
  tree size = size_in_bytes (elttype);
  tree ptrtype = build_pointer_type (elttype);
  enum tree_code add_code = PLUS_EXPR;
  tree idx = TREE_OPERAND (*expr_p, 1);
  tree minidx, offset, addr, result;

  /* If the array domain does not start at zero, apply the offset.  */
  minidx = TYPE_DOMAIN (arrtype);
  if (minidx)
    {
      minidx = TYPE_MIN_VALUE (minidx);
      if (minidx && !integer_zerop (minidx))
	{
	  idx = convert (TREE_TYPE (minidx), idx);
	  idx = fold (build (MINUS_EXPR, TREE_TYPE (minidx), idx, minidx));
	}
    }

  /* If the index is negative -- a technically invalid situation now
     that we've biased the index back to zero -- then casting it to
     unsigned has ill effects.  In particular, -1*4U/4U != -1.
     Represent this as a subtraction of a positive rather than addition
     of a negative.  This will prevent any conversion back to ARRAY_REF
     from getting the wrong results from the division.  */
  if (TREE_CODE (idx) == INTEGER_CST && tree_int_cst_sgn (idx) < 0)
    {
      idx = fold (build1 (NEGATE_EXPR, TREE_TYPE (idx), idx));
      add_code = MINUS_EXPR;
    }

  /* Pointer arithmetic must be done in sizetype.  */
  idx = convert (sizetype, idx);

  /* Convert the index to a byte offset.  */
  offset = size_binop (MULT_EXPR, size, idx);

  gimplify_expr (&array, pre_p, post_p, is_gimple_min_lval, fb_lvalue);

  addr = build_addr_expr_with_type (array, ptrtype);
  result = fold (build (add_code, ptrtype, addr, offset));
  *expr_p = build1 (INDIRECT_REF, elttype, result);
}

/* Gimplify the COMPONENT_REF, ARRAY_REF, REALPART_EXPR or IMAGPART_EXPR
   node pointed by EXPR_P.

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

   PRE_P points to the list where side effects that must happen before
     *EXPR_P should be stored.

   POST_P points to the list where side effects that must happen after
     *EXPR_P should be stored.  */

static void
gimplify_compound_lval (tree *expr_p, tree *pre_p,
			tree *post_p, int want_lvalue)
{
  tree *p;
  enum tree_code code;
  varray_type stack;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != ARRAY_REF && TREE_CODE (*expr_p) != COMPONENT_REF
      && TREE_CODE (*expr_p) != REALPART_EXPR
      && TREE_CODE (*expr_p) != IMAGPART_EXPR)
    abort ();
#endif

  code = ERROR_MARK;	/* [GIMPLE] Avoid uninitialized use warning.  */

  /* Create a stack of the subexpressions so later we can walk them in
     order from inner to outer.  */
  VARRAY_TREE_INIT (stack, 10, "stack");

  for (p = expr_p;
       TREE_CODE (*p) == ARRAY_REF || TREE_CODE (*p) == COMPONENT_REF
	 || TREE_CODE (*p) == REALPART_EXPR || TREE_CODE (*p) == IMAGPART_EXPR;
       p = &TREE_OPERAND (*p, 0))
    {
      code = TREE_CODE (*p);
      if (code == ARRAY_REF)
	{
	  tree elttype = TREE_TYPE (TREE_TYPE (TREE_OPERAND (*p, 0)));
	  if (!TREE_CONSTANT (TYPE_SIZE_UNIT (elttype)))
	    /* If the size of the array elements is not constant,
	       computing the offset is non-trivial, so expose it.  */
	    break;
	}
      VARRAY_PUSH_TREE (stack, *p);
    }

  /* Now 'p' points to the first bit that isn't a ref, 'code' is the
     TREE_CODE of the last bit that was, and 'stack' is a stack of pointers
     to all the refs we've walked through.

     Gimplify the base, and then process each of the outer nodes from left
     to right.  */
  gimplify_expr (p, pre_p, post_p, is_gimple_min_lval,
		 code != ARRAY_REF ? fb_either : fb_lvalue);

  for (; VARRAY_ACTIVE_SIZE (stack) > 0; VARRAY_POP (stack))
    {
      tree t = VARRAY_TOP_TREE (stack);
      if (TREE_CODE (t) == ARRAY_REF)
	/* Gimplify the dimension.  */
	gimplify_expr (&TREE_OPERAND (t, 1), pre_p, post_p, is_gimple_val,
		       fb_rvalue);
      /* Update TREE_SIDE_EFFECTS.  */
      recalculate_side_effects (t);
    }

  /* Now look at the toplevel expression.  If it is a COMPONENT_REF and
     the type of the COMPONENT_REF is different than the field being
     referenced, then wrap the whole thing inside a NOP_EXPR and force
     the type of the COMPONENT_REF to be the same as the field being
     referenced.  */
  if (! want_lvalue
      && TREE_CODE (*expr_p) == COMPONENT_REF
      && TREE_TYPE (*expr_p) != TREE_TYPE (TREE_OPERAND (*expr_p, 1)))
    {
      tree type_for_nop_expr = TREE_TYPE (*expr_p);

      /* Set the type of the COMPONENT_REF to the type of the field
	 being referenced.  */
      TREE_TYPE (*expr_p) = TREE_TYPE (TREE_OPERAND (*expr_p, 1));

      /* And wrap the whole thing inside a NOP_EXPR.  */
      *expr_p = build1 (NOP_EXPR, type_for_nop_expr, *expr_p);
      recalculate_side_effects (*expr_p);
    }
}

/*  Gimplify the self modifying expression pointed by EXPR_P (++, --, +=, -=).

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.

    WANT_VALUE is nonzero iff we want to use the value of this expression
        in another expression.  */

static void
gimplify_self_mod_expr (tree *expr_p, tree *pre_p, tree *post_p,
			int want_value)
{
  enum tree_code code;
  tree lhs, lvalue, rhs, t1;
  bool postfix;
  enum tree_code arith_code;

  code = TREE_CODE (*expr_p);

#if defined ENABLE_CHECKING
  if (code != POSTINCREMENT_EXPR
      && code != POSTDECREMENT_EXPR
      && code != PREINCREMENT_EXPR
      && code != PREDECREMENT_EXPR)
    abort ();
#endif

  /* Prefix or postfix?  */
  if (code == POSTINCREMENT_EXPR || code == POSTDECREMENT_EXPR)
    /* Faster to treat as prefix if result is not used.  */
    postfix = want_value;
  else
    postfix = false;

  /* Add or subtract?  */
  if (code == PREINCREMENT_EXPR || code == POSTINCREMENT_EXPR)
    arith_code = PLUS_EXPR;
  else
    arith_code = MINUS_EXPR;

  /* Gimplify the LHS into a GIMPLE lvalue.  */
  lvalue = TREE_OPERAND (*expr_p, 0);
  gimplify_expr (&lvalue, pre_p, post_p, is_gimple_lvalue, fb_lvalue);

  /* Extract the operands to the arithmetic operation.  */
  lhs = lvalue;
  rhs = TREE_OPERAND (*expr_p, 1);

  if (postfix)
    /* We want to return the original value.  */
    gimplify_expr (&lhs, pre_p, post_p, is_gimple_val, fb_rvalue);

  t1 = build (arith_code, TREE_TYPE (*expr_p), lhs, rhs);
  t1 = build (MODIFY_EXPR, TREE_TYPE (lvalue), lvalue, t1);

  if (postfix)
    /* If this is a postfix operator, put the assignment in the postqueue
       and replace the original expression with the (rvalue) LHS.  */
    {
      gimplify_stmt (&t1);
      add_tree (t1, post_p);
      *expr_p = lhs;
    }
  else
    /* Otherwise, just plug in the assignment and continue.  */
    *expr_p = t1;
}

/*  Gimplify the CALL_EXPR node pointed by EXPR_P.

      call_expr
	      : ID '(' arglist ')'

      arglist
	      : arglist ',' val
	      | val

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
	*EXPR_P should be stored.  */

static void
gimplify_call_expr (tree *expr_p, tree *pre_p, tree *post_p,
		    int (*gimple_test_f) (tree))
{
  tree decl;
  tree arglist;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != CALL_EXPR)
    abort ();
#endif

  /* For reliable diagnostics during inlining, it is necessary that 
     every call_expr be annotated with file and line.  */
  if (!EXPR_LOCUS (*expr_p))
    annotate_with_locus (*expr_p, input_location);

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
  decl = get_callee_fndecl (*expr_p);
  if (decl && DECL_BUILT_IN (decl))
    {
      tree new;

      /* Some builtins cannot be gimplified because the require specific
	 arguments (e.g., MD builtins).  */
      if (DECL_BUILT_IN_CLASS (decl) == BUILT_IN_MD
	  /* But we don't care if the call has no arguments.  */
	  && TREE_OPERAND (*expr_p, 1) != NULL_TREE)
	{
	  /* Mark the CALL_EXPR not gimplifiable so that optimizers don't
	     assume anything about it.  FIXME: Maybe we should add a target
	     hook for allowing this in the future?  */
	  mark_not_gimple (expr_p);
	  return;
	}

      /* If it is allocation of stack, record the need to restore the memory
	 when the enclosing bind_expr is exited.  */
      if (DECL_FUNCTION_CODE (decl) == BUILT_IN_STACK_ALLOC)
	gimplify_ctxp->save_stack = true;

      /* If it is restore of the stack, reset it, since it means we are
	 regimplifying the bind_expr.  Note that we use the fact that
	 for try_finally_expr, try part is processed first.  */
      if (DECL_FUNCTION_CODE (decl) == BUILT_IN_STACK_RESTORE)
	gimplify_ctxp->save_stack = false;

      new = simplify_builtin (*expr_p, gimple_test_f == is_gimple_stmt);

      if (new && new != *expr_p)
	{
	  /* There was a transformation of this call which computes the
	     same value, but in a more efficient way.  Return and try
	     again.  */
	  *expr_p = new;
	  return;
	}
    }

  gimplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
		 is_gimple_val, fb_rvalue);

  if (PUSH_ARGS_REVERSED)
    TREE_OPERAND (*expr_p, 1) = nreverse (TREE_OPERAND (*expr_p, 1));
  for (arglist = TREE_OPERAND (*expr_p, 1); arglist;
       arglist = TREE_CHAIN (arglist))
    gimplify_expr (&TREE_VALUE (arglist), pre_p, post_p, is_gimple_val,
		   fb_rvalue);
  if (PUSH_ARGS_REVERSED)
    TREE_OPERAND (*expr_p, 1) = nreverse (TREE_OPERAND (*expr_p, 1));

  /* Try this again in case gimplification exposed something.  */
  if (decl && DECL_BUILT_IN (decl))
    {
      tree new = simplify_builtin (*expr_p, gimple_test_f == is_gimple_stmt);

      if (new && new != *expr_p)
	{
	  /* There was a transformation of this call which computes the
	     same value, but in a more efficient way.  Return and try
	     again.  */
	  *expr_p = new;
	  return;
	}
    }

  /* If the function is "const", then clear TREE_SIDE_EFFECTS on its
     decl.  This allows us to eliminate redundant or useless
     calls to "const" functions.  */
  if (TREE_CODE (*expr_p) == CALL_EXPR
      && (call_expr_flags (*expr_p) & ECF_CONST))
    TREE_SIDE_EFFECTS (*expr_p) = 0;
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
  tree one, two, expr;

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

      one = shortcut_cond_r (TREE_OPERAND (pred, 0), NULL, false_label_p);
      two = shortcut_cond_r (TREE_OPERAND (pred, 1), true_label_p,
			     false_label_p);
      expr = add_stmt_to_compound (one, two);
    }
  else if (TREE_CODE (pred) == TRUTH_ORIF_EXPR)
    {
      /* Turn if (a || b) into

         if (a) goto yes;
	 if (b) goto yes; else goto no;
         (yes:) */

      if (true_label_p == NULL)
	true_label_p = &local_label;

      one = shortcut_cond_r (TREE_OPERAND (pred, 0), true_label_p, NULL);
      two = shortcut_cond_r (TREE_OPERAND (pred, 1), true_label_p,
			     false_label_p);
      expr = add_stmt_to_compound (one, two);
    }
  else if (TREE_CODE (pred) == COND_EXPR)
    {
      /* As long as we're messing with gotos, turn if (a ? b : c) into
	 if (a)
	   if (b) goto yes; else goto no;
	 else
	   if (c) goto yes; else goto no;  */
      one = shortcut_cond_r (TREE_OPERAND (pred, 1), true_label_p,
			     false_label_p);
      two = shortcut_cond_r (TREE_OPERAND (pred, 2), true_label_p,
			     false_label_p);
      expr = build (COND_EXPR, void_type_node, TREE_OPERAND (pred, 0),
		    one, two);
    }
  else
    {
      expr = build (COND_EXPR, void_type_node, pred,
		    build_and_jump (true_label_p),
		    build_and_jump (false_label_p));
    }

  if (local_label)
    add_tree (build1 (LABEL_EXPR, void_type_node, local_label), &expr);

  return expr;
}

static tree
shortcut_cond_expr (tree expr)
{
  tree pred = TREE_OPERAND (expr, 0);
  tree then_ = TREE_OPERAND (expr, 1);
  tree else_ = TREE_OPERAND (expr, 2);
  tree true_label, false_label, end_label;
  tree *true_label_p;
  tree *false_label_p;
  bool emit_end, emit_false;
  tree_stmt_iterator si;

  /* First do simple transformations.  */
  if (!TREE_SIDE_EFFECTS (else_))
    {
      /* If there is no 'else', turn (a && b) into if (a) if (b).  */
      while (TREE_CODE (pred) == TRUTH_ANDIF_EXPR)
	{
	  TREE_OPERAND (expr, 0) = TREE_OPERAND (pred, 1);
	  then_ = shortcut_cond_expr (expr);
	  pred = TREE_OPERAND (pred, 0);
	  expr = build (COND_EXPR, void_type_node, pred, then_,
			build_empty_stmt ());
	}
    }
  if (!TREE_SIDE_EFFECTS (then_))
    {
      /* If there is no 'then', turn
	   if (a || b); else d
	 into
	   if (a); else if (b); else d.  */
      while (TREE_CODE (pred) == TRUTH_ORIF_EXPR)
	{
	  TREE_OPERAND (expr, 0) = TREE_OPERAND (pred, 1);
	  else_ = shortcut_cond_expr (expr);
	  pred = TREE_OPERAND (pred, 0);
	  expr = build (COND_EXPR, void_type_node, pred,
			build_empty_stmt (), else_);
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
  si = tsi_start (&then_);
  expr = tsi_stmt (si);
  if (TREE_CODE (expr) == GOTO_EXPR
      && TREE_CODE (GOTO_DESTINATION (expr)) == LABEL_DECL)
    {
      true_label = GOTO_DESTINATION (expr);
      tsi_delink (&si);
    }
  si = tsi_start (&else_);
  expr = tsi_stmt (si);
  if (TREE_CODE (expr) == GOTO_EXPR
      && TREE_CODE (GOTO_DESTINATION (expr)) == LABEL_DECL)
    {
      false_label = GOTO_DESTINATION (expr);
      tsi_delink (&si);
    }

  /* If we aren't hijacking a label for the 'then' branch, it falls through. */
  if (true_label)
    true_label_p = &true_label;
  else
    true_label_p = NULL;

  /* The 'else' branch also needs a label if it contains interesting code.  */
  if (false_label || TREE_SIDE_EFFECTS (else_))
    false_label_p = &false_label;
  else
    false_label_p = NULL;

  /* If there was nothing else in our arms, just forward the label(s).  */
  if (!TREE_SIDE_EFFECTS (then_) && !TREE_SIDE_EFFECTS (else_))
    return shortcut_cond_r (pred, true_label_p, false_label_p);

  /* If our last subexpression already has a terminal label, reuse it.  */
  if (TREE_SIDE_EFFECTS (else_))
    expr = expr_last (else_);
  else
    expr = expr_last (then_);
  if (TREE_CODE (expr) == LABEL_EXPR)
    end_label = LABEL_EXPR_LABEL (expr);

  /* If we don't care about jumping to the 'else' branch, jump to the end
     if the condition is false.  */
  if (!false_label_p)
    false_label_p = &end_label;

  /* We only want to emit these labels if we aren't hijacking them.  */
  emit_end = (end_label == NULL_TREE);
  emit_false = (false_label == NULL_TREE);

  expr = shortcut_cond_r (pred, true_label_p, false_label_p);

  add_tree (then_, &expr);
  if (TREE_SIDE_EFFECTS (else_))
    {
      add_tree (build_and_jump (&end_label), &expr);
      if (emit_false)
	add_tree (build1 (LABEL_EXPR, void_type_node, false_label), &expr);
      add_tree (else_, &expr);
    }
  if (emit_end && end_label)
    add_tree (build1 (LABEL_EXPR, void_type_node, end_label), &expr);

  return expr;
}


/*  Convert the conditional expression pointed by EXPR_P '(p) ? a : b;'
    into

    if (p)			if (p)
      t1 = a;			  a;
    else		or	else
      t1 = b;			  b;
    t1;

    The second form is used when *EXPR_P is of type void.

    PRE_P points to the list where side effects that must happen before
        *EXPR_P should be stored.  */

static void
gimplify_cond_expr (tree *expr_p, tree *pre_p, tree target)
{
  tree expr = *expr_p;
  tree tmp;

  /* If this COND_EXPR has a value, copy the values into a temporary within
     the arms.  */
  if (! VOID_TYPE_P (TREE_TYPE (expr)))
    {
      if (target)
	tmp = target;
      else
	tmp = create_tmp_var (TREE_TYPE (expr), "iftmp");

      /* Build the then clause, 't1 = a;'.  But don't build an assignment
	 if this branch is void; in C++ it can be, if it's a throw.  */
      if (TREE_TYPE (TREE_OPERAND (expr, 1)) != void_type_node)
	TREE_OPERAND (expr, 1)
	  = build (MODIFY_EXPR, void_type_node, tmp, TREE_OPERAND (expr, 1));

      /* Build the else clause, 't1 = b;'.  */
      if (TREE_TYPE (TREE_OPERAND (expr, 2)) != void_type_node)
	TREE_OPERAND (expr, 2)
	  = build (MODIFY_EXPR, void_type_node, tmp, TREE_OPERAND (expr, 2));

      TREE_TYPE (expr) = void_type_node;
      recalculate_side_effects (expr);

      /* Move the COND_EXPR to the prequeue and use the temp in its place.  */
      gimplify_stmt (&expr);
      add_tree (expr, pre_p);
      *expr_p = tmp;

      return;
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
	  gimplify_stmt (expr_p);
	  gimple_pop_condition (pre_p);
	  return;
	}
    }

  /* Now do the normal gimplification.  */
  gimplify_expr (&TREE_OPERAND (expr, 0), pre_p, NULL,
		 is_gimple_condexpr, fb_rvalue);

  gimple_push_condition ();

  gimplify_stmt (&TREE_OPERAND (expr, 1));
  gimplify_stmt (&TREE_OPERAND (expr, 2));

  gimple_pop_condition (pre_p);

  if (TREE_SIDE_EFFECTS (TREE_OPERAND (expr, 1)))
    /* OK */;
  else if (TREE_SIDE_EFFECTS (TREE_OPERAND (expr, 2)))
    /* Rewrite "if (a); else b" to "if (!a) b"  */
    {
      TREE_OPERAND (expr, 0) = invert_truthvalue (TREE_OPERAND (expr, 0));
      gimplify_expr (&TREE_OPERAND (expr, 0), pre_p, NULL,
		     is_gimple_condexpr, fb_rvalue);

      tmp = TREE_OPERAND (expr, 1);
      TREE_OPERAND (expr, 1) = TREE_OPERAND (expr, 2);
      TREE_OPERAND (expr, 2) = tmp;
    }
  else
    /* Both arms are empty; replace the COND_EXPR with its predicate.  */
    expr = TREE_OPERAND (expr, 0);

  *expr_p = expr;
}

/*  Gimplify the MODIFY_EXPR node pointed by EXPR_P.

      modify_expr
	      : varname '=' rhs
	      | '*' ID '=' rhs

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.

    WANT_VALUE is nonzero iff we want to use the value of this expression
        in another expression.  */

static void
gimplify_modify_expr (tree *expr_p, tree *pre_p, tree *post_p, int want_value)
{
  tree *from_p = &TREE_OPERAND (*expr_p, 1);
  tree *to_p = &TREE_OPERAND (*expr_p, 0);

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != MODIFY_EXPR
      && TREE_CODE (*expr_p) != INIT_EXPR)
    abort ();
#endif

  gimplify_expr (to_p, pre_p, post_p, is_gimple_lvalue, fb_lvalue);

  /* If we are initializing something from a TARGET_EXPR, strip the
     TARGET_EXPR and initialize it directly.  */
  /* What about code that pulls out the temp and uses it elsewhere?  I
     think that such code never uses the TARGET_EXPR as an initializer.  If
     I'm wrong, we'll abort because the temp won't have any RTL.  In that
     case, I guess we'll need to replace references somehow.  */
  if (TREE_CODE (*from_p) == TARGET_EXPR)
    *from_p = TARGET_EXPR_INITIAL (*from_p);

  /* If we're assigning from a ?: expression with ADDRESSABLE type, push
     the assignment down into the branches, since we can't generate a
     temporary of such a type.  */
  if (TREE_CODE (*from_p) == COND_EXPR
      && TREE_ADDRESSABLE (TREE_TYPE (*from_p)))
    {
      gimplify_cond_expr (from_p, pre_p, *to_p);
      *expr_p = *from_p;
      /* Try again.  */
      return;
    }

  /* The distinction between MODIFY_EXPR and INIT_EXPR is no longer
     useful.  */
  if (TREE_CODE (*expr_p) == INIT_EXPR)
    TREE_SET_CODE (*expr_p, MODIFY_EXPR);

  gimplify_expr (from_p, pre_p, post_p, is_gimple_rhs, fb_rvalue);

  if (gimplify_init_constructor (expr_p, pre_p, want_value))
    return;

  /* If the RHS of the MODIFY_EXPR may throw or make a nonlocal goto and
     the LHS is a user variable, then we need to introduce a temporary.
     ie temp = RHS; LHS = temp.

     This way the optimizers can determine that the user variable is
     only modified if evaluation of the RHS does not throw.

     FIXME this should be handled by the is_gimple_rhs predicate.  */
  if (! is_gimple_tmp_var (*to_p)
      && (TREE_CODE (*from_p) == CALL_EXPR
	  || (flag_non_call_exceptions && tree_could_trap_p (*from_p))))
    gimplify_expr (from_p, pre_p, post_p, is_gimple_val, fb_rvalue);

  if (want_value)
    {
      add_tree (*expr_p, pre_p);
      *expr_p = *to_p;
    }
}


/*  Gimplify TRUTH_ANDIF_EXPR and TRUTH_ORIF_EXPR expressions.  EXPR_P
    points to the expression to gimplify.

    Expressions of the form 'a && b' are gimplified to:

        a && b ? true : false

    gimplify_cond_expr will do the rest.

    PRE_P points to the list where side effects that must happen before
        *EXPR_P should be stored.  */

static void
gimplify_boolean_expr (tree *expr_p)
{
  /* Preserve the original type of the expression.  */
  tree type = TREE_TYPE (*expr_p);
  *expr_p = build (COND_EXPR, type, *expr_p,
		   convert (type, boolean_true_node),
		   convert (type, boolean_false_node));
}


/*  Gimplifies an expression sequence.  This function gimplifies each
    expression and re-writes the original expression with the last
    expression of the sequence in GIMPLE form.

    PRE_P points to the list where the side effects for all the expressions
	in the sequence will be emitted.

    POST_P points to the list where the post side effects for the last
        expression in the sequence are emitted.  */

static void
gimplify_compound_expr (tree *expr_p, tree *pre_p)
{
  tree t;
  for (t = *expr_p; TREE_CODE (t) == COMPOUND_EXPR; t = TREE_OPERAND (t, 1))
    {
      tree sub = TREE_OPERAND (t, 0);
      gimplify_stmt (&sub);
      add_tree (sub, pre_p);
    }

  *expr_p = t;
}


/*  Gimplify a SAVE_EXPR node.  EXPR_P points to the expression to
    gimplify.  After gimplification, EXPR_P will point to a new temporary
    that holds the original value of the SAVE_EXPR node.

    PRE_P points to the list where side effects that must happen before
        *EXPR_P should be stored.  */

static void
gimplify_save_expr (tree *expr_p, tree *pre_p, tree *post_p)
{
  tree val;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != SAVE_EXPR)
    abort ();
#endif

  val = TREE_OPERAND (*expr_p, 0);

  /* If the operand is already a GIMPLE temporary, just re-write the
     SAVE_EXPR node.  */
  if (is_gimple_tmp_var (val))
    *expr_p = val;
  /* The operand may be a void-valued expression such as SAVE_EXPRs
     generated by the Java frontend for class initialization.  It is
     being executed only for its side-effects.  */
  else if (TREE_TYPE (val) == void_type_node)
    {
      tree body = TREE_OPERAND (*expr_p, 0);
      gimplify_expr (& body, pre_p, post_p, is_gimple_stmt, fb_none);
      add_tree (body, pre_p);
      *expr_p = build_empty_stmt ();
    }
  else
    *expr_p = TREE_OPERAND (*expr_p, 0)
      = get_initialized_tmp_var (val, pre_p, post_p);
}

/*  Re-write the ADDR_EXPR node pointed by EXPR_P

      unary_expr
	      : ...
	      | '&' varname
	      ...

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
	*EXPR_P should be stored.  */

static void
gimplify_addr_expr (tree *expr_p, tree *pre_p, tree *post_p)
{
  tree expr = *expr_p;
  tree op0 = TREE_OPERAND (expr, 0);

  switch (TREE_CODE (op0))
    {
    case INDIRECT_REF:
      /* Check if we are dealing with an expression of the form '&*ptr'.
	 While the front end folds away '&*ptr' into 'ptr', these
	 expressions may be generated internally by the compiler (e.g.,
	 builtins like __builtin_va_end).  */
      *expr_p = TREE_OPERAND (op0, 0);
      break;

    case ARRAY_REF:
      /* Fold &a[6] to (&a + 6).  */
      gimplify_array_ref_to_plus (&TREE_OPERAND (expr, 0), pre_p, post_p);

      /* This added an INDIRECT_REF.  Fold it away.  */
      op0 = TREE_OPERAND (TREE_OPERAND (expr, 0), 0);

      /* ??? The Fortran front end does questionable things with types here,
	 wanting to create a pointer to an array by taking the address of
	 an element of the array.  I think we're trying to create some sort
	 of array slice or something.  Anyway, notice that the type of the
	 ADDR_EXPR doesn't match the type of the current pointer and add a
	 cast if necessary.  */
      if (TYPE_MAIN_VARIANT (TREE_TYPE (expr))
	  != TYPE_MAIN_VARIANT (TREE_TYPE (op0)))
	op0 = build1 (NOP_EXPR, TREE_TYPE (expr), op0);

      *expr_p = op0;
      break;

    default:
      /* We use fb_either here because the C frontend sometimes takes
	 the address of a call that returns a struct.  */
      gimplify_expr (&TREE_OPERAND (expr, 0), pre_p, post_p,
		     is_gimple_addr_expr_arg, fb_either);
      TREE_SIDE_EFFECTS (expr)
	= TREE_SIDE_EFFECTS (TREE_OPERAND (expr, 0));

      /* Mark the RHS addressable.  */
      (*lang_hooks.mark_addressable) (TREE_OPERAND (expr, 0));
      break;
    }
}

/* Gimplify the operands of an ASM_EXPR.  Input operands should be a gimple
   value; output operands should be a gimple lvalue.  */

static void
gimplify_asm_expr (tree *expr_p, tree *pre_p, tree *post_p)
{
  tree expr = *expr_p;
  int noutputs = list_length (ASM_OUTPUTS (expr));
  const char **oconstraints
    = (const char **) alloca ((noutputs) * sizeof (const char *));
  int i;
  tree link;
  const char *constraint;
  bool allows_mem, allows_reg, is_inout, success;

  ASM_STRING (expr)
    = resolve_asm_operand_names (ASM_STRING (expr), ASM_OUTPUTS (expr),
				 ASM_INPUTS (expr));

  success = true;
  for (i = 0, link = ASM_OUTPUTS (expr); link; ++i, link = TREE_CHAIN (link))
    {
      oconstraints[i] = constraint
	= TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));

      parse_output_constraint (&constraint, i, 0, 0,
			       &allows_mem, &allows_reg, &is_inout);

      if (!allows_reg && allows_mem)
	(*lang_hooks.mark_addressable) (TREE_VALUE (link));

      if (!gimplify_expr (&TREE_VALUE (link), pre_p, post_p,
			  is_gimple_lvalue, fb_lvalue | fb_mayfail))
	{
	  error ("invalid lvalue in asm output %d", i);
	  success = false;
	}

      if (is_inout && allows_reg)
	{
	  /* An input/output operand that allows a register.  To give the
	     optimizers more flexibility, split it into separate input and
	     output operands.  */
	  tree input;
	  char buf[10];

	  /* Turn the in/out constraint into an output constraint.  */
	  char *p = xstrdup (constraint);
	  p[0] = '=';
	  TREE_VALUE (TREE_PURPOSE (link)) = build_string (strlen (p), p);

	  /* And add a matching input constraint.  */
	  sprintf (buf, "%d", i);
	  input = build_string (strlen (buf), buf);
	  input = build_tree_list (build_tree_list (NULL_TREE, input),
				   unshare_expr (TREE_VALUE (link)));
	  ASM_INPUTS (expr) = chainon (input, ASM_INPUTS (expr));
	}
    }

  for (link = ASM_INPUTS (expr); link; ++i, link = TREE_CHAIN (link))
    {
      constraint
	= TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      parse_input_constraint (&constraint, 0, 0, noutputs, 0,
			      oconstraints, &allows_mem, &allows_reg);

      /* If the operand is a memory input, it should be an lvalue.  */
      if (!allows_reg && allows_mem)
	{
	  (*lang_hooks.mark_addressable) (TREE_VALUE (link));
	  if (!gimplify_expr (&TREE_VALUE (link), pre_p, post_p,
			      is_gimple_lvalue, fb_lvalue | fb_mayfail))
	    {
	      error ("memory input %d is not directly addressable", i);
	      success = false;
	    }
	}
      else
	gimplify_expr (&TREE_VALUE (link), pre_p, post_p,
		       is_gimple_val, fb_rvalue);
    }

  /* If we encountered an invalid asm, don't hork the optimizers.  */
  if (!success)
    *expr_p = build_empty_stmt ();
}

/* If EXPR is a boolean expression, make sure it has BOOLEAN_TYPE.  */

static tree
gimple_boolify (tree expr)
{
  tree type = TREE_TYPE (expr);

  if (TREE_CODE (type) == BOOLEAN_TYPE)
    return expr;

  /* If this is the predicate of a COND_EXPR, it might not even be a
     truthvalue yet.  */
  expr = (*lang_hooks.truthvalue_conversion) (expr);

  switch (TREE_CODE (expr))
    {
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
      /* Also boolify the arguments of truth exprs.  */
      TREE_OPERAND (expr, 1) = gimple_boolify (TREE_OPERAND (expr, 1));
    case TRUTH_NOT_EXPR:
      TREE_OPERAND (expr, 0) = gimple_boolify (TREE_OPERAND (expr, 0));
    case EQ_EXPR: case NE_EXPR:
    case LE_EXPR: case GE_EXPR: case LT_EXPR: case GT_EXPR:
      /* These expressions always produce boolean results.  */
      TREE_TYPE (expr) = boolean_type_node;
      return expr;
      
    default:
      /* Other expressions that get here must have boolean values, but
	 might need to be converted to the appropriate mode.  */
      return convert (boolean_type_node, expr);
    }
}

/* Apply FN to each statement under *STMT_P, which may be a COMPOUND_EXPR
   or a single statement expr.  */

void
foreach_stmt (tree *stmt_p, foreach_stmt_fn *fn)
{
  if (*stmt_p == NULL_TREE)
    return;

  for (; TREE_CODE (*stmt_p) == COMPOUND_EXPR;
       stmt_p = &TREE_OPERAND (*stmt_p, 1))
    {
      tree *sub_p = &TREE_OPERAND (*stmt_p, 0);
      if (TREE_CODE (*sub_p) == COMPOUND_EXPR)
	foreach_stmt (sub_p, fn);
      else
	fn (sub_p);
    }
  fn (stmt_p);
}

static location_t wfl_locus;

static void
annotate_all_with_locus_1 (tree *stmt_p)
{
  /* Don't emit a line note for a label.  We particularly don't want to
     emit one for the break label, since it doesn't actually correspond
     to the beginning of the loop/switch.  */;
  if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (TREE_CODE (*stmt_p)))
      && TREE_CODE (*stmt_p) != LABEL_EXPR
      && ! EXPR_LOCUS (*stmt_p))
    annotate_with_locus (*stmt_p, wfl_locus);
}

void
annotate_all_with_locus (tree *stmt_p, location_t locus)
{
  wfl_locus = locus;
  foreach_stmt (stmt_p, annotate_all_with_locus_1);
}

/* Add STMT to EXISTING if possible, otherwise create a new
   COMPOUND_EXPR and add STMT to it. */

static tree
add_stmt_to_compound (tree existing, tree stmt)
{
  /* If we previously had nothing, allow an empty statement.  */
  if (!existing && stmt && IS_EMPTY_STMT (stmt))
    return stmt;
  if (!stmt || !TREE_SIDE_EFFECTS (stmt))
    return existing;
  else if (existing && TREE_SIDE_EFFECTS (existing))
    return build (COMPOUND_EXPR, void_type_node, existing, stmt);
  else
    return stmt;
}

/*  Add T to the list container pointed by LIST_P.  If T is a TREE_LIST
    node, it is linked-in directly.  If T is an expression with no effects,
    it is ignored.

    Return the newly added list node or NULL_TREE if T was not added to
    LIST_P.  */

void
add_tree (tree t, tree *list_p)
{
  *list_p = add_stmt_to_compound (*list_p, t);
}


/*  Create a new temporary variable declaration of type TYPE.  Returns the
    newly created decl and pushes it into the current binding.  */

tree
create_tmp_var (tree type, const char *prefix)
{
  static unsigned int id_num = 1;
  char *tmp_name;
  char *preftmp = NULL;
  tree tmp_var;
  tree new_type;

  if (prefix)
    {
      preftmp = ASTRDUP (prefix);
      remove_suffix (preftmp, strlen (preftmp));
      prefix = preftmp;
    }

  ASM_FORMAT_PRIVATE_NAME (tmp_name, (prefix ? prefix : "T"), id_num++);

#if defined ENABLE_CHECKING
  /* If the type is an array or a type which must be created by the
     frontend, something is wrong.  */
  if (TREE_CODE (type) == ARRAY_TYPE || TREE_ADDRESSABLE (type))
    abort ();
  if (!COMPLETE_TYPE_P (type))
    abort ();
#endif

  /* Make the type of the variable writable.  */
  new_type = build_type_variant (type, 0, 0);
  TYPE_ATTRIBUTES (new_type) = TYPE_ATTRIBUTES (type);

  tmp_var = build_decl (VAR_DECL, get_identifier (tmp_name), type);

  /* The variable was declared by the compiler.  */
  DECL_ARTIFICIAL (tmp_var) = 1;
  /* And we don't want debug info for it.  */
  DECL_IGNORED_P (tmp_var) = 1;

  /* Make the variable writable.  */
  TREE_READONLY (tmp_var) = 0;

  DECL_EXTERNAL (tmp_var) = 0;
  TREE_STATIC (tmp_var) = 0;
  TREE_USED (tmp_var) = 1;

  gimple_add_tmp_var (tmp_var);

  return tmp_var;
}

/* Create a new temporary alias variable declaration of type TYPE.  Returns
   the newly created decl.  Does NOT push it into the current binding.  */

tree
create_tmp_alias_var (tree type, const char *prefix)
{
  static unsigned int id_num = 1;
  char *tmp_name;
  char *preftmp = NULL;
  tree tmp_var;

  if (prefix)
    {
      preftmp = ASTRDUP (prefix);
      remove_suffix (preftmp, strlen (preftmp));
      prefix = preftmp;
    }

  ASM_FORMAT_PRIVATE_NAME (tmp_name, (prefix ? prefix : "T"), id_num++);

#if 0
  /* FIXME: build_decl tries to layout the decl again.  This is causing a
     miscompilation of g++.dg/debug/debug5.C because at this point CFUN
     doesn't exist anymore.  Besides, laying the decl again seems to be
     unnecessary work.  */
  tmp_var = build_decl (VAR_DECL, get_identifier (tmp_name), type);
#endif
  tmp_var = make_node (VAR_DECL);
  DECL_NAME (tmp_var) = get_identifier (tmp_name);
  TREE_TYPE (tmp_var) = type;

  /* The variable was declared by the compiler.  */
  DECL_ARTIFICIAL (tmp_var) = 1;

  /* Make the variable writable.  */
  TREE_READONLY (tmp_var) = 0;

  DECL_EXTERNAL (tmp_var) = 0;
  DECL_CONTEXT (tmp_var) = current_function_decl;
  TREE_STATIC (tmp_var) = 0;
  TREE_USED (tmp_var) = 1;
  TREE_THIS_VOLATILE (tmp_var) = TYPE_VOLATILE (type);


  return tmp_var;
}

/*  Given a tree, try to return a useful variable name that we can use
    to prefix a temporary that is being assigned the value of the tree.
    I.E. given  <temp> = &A, return A.  */

const char *
get_name (tree t)
{
  tree stripped_decl;

  stripped_decl = t;
  STRIP_NOPS (stripped_decl);
  if (DECL_P (stripped_decl) && DECL_NAME (stripped_decl))
    return IDENTIFIER_POINTER (DECL_NAME (stripped_decl));
  else
    {
      switch (TREE_CODE (stripped_decl))
	{
	case ADDR_EXPR:
	  return get_name (TREE_OPERAND (stripped_decl, 0));
	  break;
	default:
	  return NULL;
	}
    }
}

/* Formal (expression) temporary table handling: Multiple occurrences of
   the same scalar expression are evaluated into the same temporary.  */

typedef struct gimple_temp_hash_elt
{
  tree val;   /* Key */
  tree temp;  /* Value */
} elt_t;

/* Return a hash value for a formal temporary table entry.  */

static hashval_t
gimple_tree_hash (const void *p)
{
  tree t = ((const elt_t *)p)->val;
  return iterative_hash_expr (t, 0);
}

/* Compare two formal temporary table entries.  */

static int
gimple_tree_eq (const void *p1, const void *p2)
{
  tree t1 = ((const elt_t *)p1)->val;
  tree t2 = ((const elt_t *)p2)->val;
  enum tree_code code = TREE_CODE (t1);

  if (TREE_CODE (t2) != code
      || TREE_TYPE (t1) != TREE_TYPE (t2))
    return 0;

  if (!operand_equal_p (t1, t2, 0))
    return 0;

  /* Only allow them to compare equal if they also hash equal; otherwise
     results are nondeterminate, and we fail bootstrap comparison.  */
  if (gimple_tree_hash (p1) != gimple_tree_hash (p2))
    abort ();

  return 1;
}

/* Create a temporary with a name derived from VAL.  Subroutine of
   lookup_tmp_var; nobody else should call this function.  */

static inline tree
create_tmp_from_val (tree val)
{
  return create_tmp_var (TREE_TYPE (val), get_name (val));
}

/* Create a temporary to hold the value of VAL.  If IS_FORMAL, try to reuse
   an existing expression temporary.  */

static tree
lookup_tmp_var (tree val, bool is_formal)
{
  if (!is_formal || TREE_SIDE_EFFECTS (val))
    return create_tmp_from_val (val);
  else
    {
      elt_t elt, *elt_p;
      void **slot;

      elt.val = val;
      slot = htab_find_slot (gimplify_ctxp->temp_htab, (void *)&elt, INSERT);
      if (*slot == NULL)
	{
	  elt_p = xmalloc (sizeof (*elt_p));
	  elt_p->val = val;
	  elt_p->temp = create_tmp_from_val (val);
	  *slot = (void *)elt_p;
	}
      else
	elt_p = (elt_t *) *slot;

      return elt_p->temp;
    }
}

/* Returns a formal temporary variable initialized with VAL.  PRE_P is as
   in gimplify_expr.  Only use this function if:

   1) The value of the unfactored expression represented by VAL will not
      change between the initialization and use of the temporary, and
   2) The temporary will not be otherwise modified.

   For instance, #1 means that this is inappropriate for SAVE_EXPR temps,
   and #2 means it is inappropriate for && temps.

   For other cases, use get_initialized_tmp_var instead.  */

tree
get_formal_tmp_var (tree val, tree *pre_p)
{
  return internal_get_tmp_var (val, pre_p, NULL, true);
}

/* Returns a temporary variable initialized with VAL.  PRE_P and POST_P
   are as in gimplify_expr.  */

tree
get_initialized_tmp_var (tree val, tree *pre_p, tree *post_p)
{
  return internal_get_tmp_var (val, pre_p, post_p, false);
}

static tree
internal_get_tmp_var (tree val, tree *pre_p, tree *post_p, bool is_formal)
{
  tree t, mod;
  char class;

  gimplify_expr (&val, pre_p, post_p, is_gimple_rhs, fb_rvalue);

  t = lookup_tmp_var (val, is_formal);

  mod = build (MODIFY_EXPR, TREE_TYPE (t), t, val);

  class = TREE_CODE_CLASS (TREE_CODE (val));
  if (EXPR_LOCUS (val))
    SET_EXPR_LOCUS (mod, EXPR_LOCUS (val));
  else
    annotate_with_locus (mod, input_location);
  /* gimplify_modify_expr might want to reduce this further.  */
  gimplify_stmt (&mod);
  add_tree (mod, pre_p);

  return t;
}

/*  Returns true if T is a GIMPLE temporary variable, false otherwise.  */

bool
is_gimple_tmp_var (tree t)
{
  /* FIXME this could trigger for other local artificials, too.  */
  return (TREE_CODE (t) == VAR_DECL && DECL_ARTIFICIAL (t)
	  && !TREE_STATIC (t) && !DECL_EXTERNAL (t));
}

/*  Declares all the variables in VARS in SCOPE.  Returns the last
    DECL_STMT emitted.  */

void
declare_tmp_vars (tree vars, tree scope)
{
  tree last = vars;
  if (last)
    {
      tree temps;

      /* C99 mode puts the default 'return 0;' for main() outside the outer
	 braces.  So drill down until we find an actual scope.  */
      while (TREE_CODE (scope) == COMPOUND_EXPR)
	scope = TREE_OPERAND (scope, 0);

      if (TREE_CODE (scope) != BIND_EXPR)
	abort ();

      temps = nreverse (last);
      TREE_CHAIN (last) = BIND_EXPR_VARS (scope);
      BIND_EXPR_VARS (scope) = temps;

      /* We don't add the temps to the block for this BIND_EXPR, as we're
	 not interested in debugging info for them.  */
    }
}

void
gimple_add_tmp_var (tree tmp)
{
  if (TREE_CHAIN (tmp))
    abort ();

  DECL_CONTEXT (tmp) = current_function_decl;
  if (gimplify_ctxp)
    {
      TREE_CHAIN (tmp) = gimplify_ctxp->temps;
      gimplify_ctxp->temps = tmp;
    }
  else
    declare_tmp_vars (tmp, DECL_SAVED_TREE (current_function_decl));
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


/* Unshare T and all the trees reached from T via TREE_CHAIN.  */

void
unshare_all_trees (tree t)
{
  walk_tree (&t, copy_if_shared_r, NULL, NULL);
  walk_tree (&t, unmark_visited_r, NULL, NULL);
}


/* Callback for walk_tree to unshare most of the shared trees rooted at
   *TP.  If *TP has been visited already (i.e., TREE_VISITED (*TP) == 1),
   then *TP is deep copied by calling copy_tree_r.

   This unshares the same trees as copy_tree_r with the exception of
   SAVE_EXPR nodes.  These nodes model computations that should only be
   done once.  If we were to unshare something like SAVE_EXPR(i++), the
   gimplification process would create wrong code.  */

static tree
copy_if_shared_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED, void *data ATTRIBUTE_UNUSED)
{
  /* If this node has been visited already, unshare it and don't look
     any deeper.  */
  if (TREE_VISITED (*tp))
    {
      walk_tree (tp, mostly_copy_tree_r, NULL, NULL);
      *walk_subtrees = 0;
    }
  else
    /* Otherwise, mark the tree as visited and keep looking.  */
    TREE_VISITED (*tp) = 1;

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

/* Similar to copy_tree_r() but do not copy SAVE_EXPR nodes.  These nodes
   model computations that should only be done once.  If we were to unshare
   something like SAVE_EXPR(i++), the gimplification process would create
   wrong code.  */

static tree
mostly_copy_tree_r (tree *tp, int *walk_subtrees, void *data)
{
  enum tree_code code = TREE_CODE (*tp);
  /* Don't unshare types, constants and SAVE_EXPR nodes.  */
  if (TREE_CODE_CLASS (code) == 't'
      || TREE_CODE_CLASS (code) == 'c'
      || code == SAVE_EXPR)
    *walk_subtrees = 0;
  else if (code == BIND_EXPR)
    abort ();
  else
    copy_tree_r (tp, walk_subtrees, data);

  return NULL_TREE;
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

void
mark_not_gimple (tree *expr_p)
{
  TREE_NOT_GIMPLE (*expr_p) = 1;
}

/* A terser interface for building a representation of a exception
   specification.  */

tree
gimple_build_eh_filter (tree body, tree allowed, tree failure)
{
  /* FIXME should the allowed types go in TREE_TYPE?  */
  tree filter = build (EH_FILTER_EXPR, void_type_node, allowed, failure);
  return build (TRY_CATCH_EXPR, void_type_node, body, filter);
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

static void
gimplify_cleanup_point_expr (tree *expr_p, tree *pre_p)
{
  tree_stmt_iterator iter;
  tree body;

  tree temp = voidify_wrapper_expr (*expr_p);

  /* We only care about the number of conditions between the innermost
     CLEANUP_POINT_EXPR and the cleanup.  So save and reset the count.  */
  int old_conds = gimplify_ctxp->conditions;
  gimplify_ctxp->conditions = 0;

  body = TREE_OPERAND (*expr_p, 0);
  gimplify_stmt (&body);

  gimplify_ctxp->conditions = old_conds;

  for (iter = tsi_start (&body); !tsi_end_p (iter); )
    {
      tree wce = tsi_stmt (iter);
      if (wce && TREE_CODE (wce) == WITH_CLEANUP_EXPR)
	{
	  tree *container = tsi_container (iter);
	  tree next, tfe;

	  if (TREE_CODE (*container) == COMPOUND_EXPR)
	    next = TREE_OPERAND (*container, 1);
	  else
	    next = build_empty_stmt ();

	  tfe = build (TRY_FINALLY_EXPR, void_type_node,
		       next, TREE_OPERAND (wce, 1));
	  *container = tfe;
	  iter = tsi_start (&TREE_OPERAND (tfe, 0));
	}
      else
	tsi_next (&iter);
    }

  if (temp)
    {
      *expr_p = temp;
      add_tree (body, pre_p);
    }
  else
    *expr_p = body;
}

/* Insert a cleanup marker for gimplify_cleanup_point_expr.  CLEANUP
   is the cleanup action required.  */

static void
gimple_push_cleanup (tree cleanup, tree *pre_p)
{
  tree wce;

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
      tree ffalse = build (MODIFY_EXPR, void_type_node, flag,
			   boolean_false_node);
      tree ftrue = build (MODIFY_EXPR, void_type_node, flag,
			  boolean_true_node);
      cleanup = build (COND_EXPR, void_type_node, flag, cleanup,
		       build_empty_stmt ());
      wce = build (WITH_CLEANUP_EXPR, void_type_node, NULL_TREE,
		   cleanup, NULL_TREE);
      add_tree (ffalse, &gimplify_ctxp->conditional_cleanups);
      add_tree (wce, &gimplify_ctxp->conditional_cleanups);
      add_tree (ftrue, pre_p);
    }
  else
    {
      wce = build (WITH_CLEANUP_EXPR, void_type_node, NULL_TREE,
		   cleanup, NULL_TREE);
      add_tree (wce, pre_p);
    }
}

/* Gimplify a TARGET_EXPR which doesn't appear on the rhs of an INIT_EXPR.  */

static void
gimplify_target_expr (tree *expr_p, tree *pre_p, tree *post_p)
{
  tree targ = *expr_p;
  tree temp = TARGET_EXPR_SLOT (targ);
  tree init = TARGET_EXPR_INITIAL (targ);

  /* TARGET_EXPR temps aren't part of the enclosing block, so add it to the
     temps list.  */
  gimple_add_tmp_var (temp);

  /* Build up the initialization and add it to pre_p.  */
  init = build (MODIFY_EXPR, void_type_node, temp, init);
  gimplify_expr (&init, pre_p, post_p, is_gimple_stmt, fb_none);
  add_tree (init, pre_p);

  /* If needed, push the cleanup for the temp.  */
  if (TARGET_EXPR_CLEANUP (targ))
    {
      gimplify_stmt (&TARGET_EXPR_CLEANUP (targ));
      gimple_push_cleanup (TARGET_EXPR_CLEANUP (targ), pre_p);
    }

  *expr_p = temp;
}

/* Prepare calls to builtins to SAVE and RESTORE the stack as well as temporary
   through that they comunicate.  */

static void
build_stack_save_restore (tree *save, tree *restore)
{
  tree save_call, tmp_var;

  save_call =
      build_function_call_expr (implicit_built_in_decls[BUILT_IN_STACK_SAVE],
				NULL_TREE);
  tmp_var = create_tmp_var (ptr_type_node, "saved_stack");

  *save = build (MODIFY_EXPR, ptr_type_node, tmp_var, save_call);
  *restore =
      build_function_call_expr (implicit_built_in_decls[BUILT_IN_STACK_RESTORE],
				tree_cons (NULL_TREE, tmp_var, NULL_TREE));
}
