/* Tree lowering pass.  This pass simplifies the GENERIC functions-as-trees
   tree representation to the GIMPLE form.

   Copyright (C) 2002 Free Software Foundation, Inc.
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

static void simplify_constructor     PARAMS ((tree, tree *, tree *));
static void simplify_array_ref       PARAMS ((tree *, tree *, tree *));
static void simplify_array_ref_to_plus   PARAMS ((tree *, tree *, tree *));
static void simplify_compound_lval   PARAMS ((tree *, tree *, tree *));
static void simplify_component_ref   PARAMS ((tree *, tree *, tree *));
static void simplify_call_expr       PARAMS ((tree *, tree *, tree *));
static void simplify_tree_list       PARAMS ((tree *, tree *, tree *));
static void simplify_modify_expr     PARAMS ((tree *, tree *, tree *, int));
static void simplify_compound_expr   PARAMS ((tree *, tree *));
static void simplify_save_expr       PARAMS ((tree *, tree *));
static void simplify_addr_expr       PARAMS ((tree *, tree *, tree *));
static void simplify_self_mod_expr   PARAMS ((tree *, tree *, tree *));
static void simplify_cond_expr       PARAMS ((tree *, tree *, tree));
static void simplify_boolean_expr    PARAMS ((tree *, tree *));
static void simplify_return_expr     PARAMS ((tree, tree *));
static tree build_addr_expr	     PARAMS ((tree));
static tree build_addr_expr_with_type   PARAMS ((tree, tree));
static tree add_stmt_to_compound	PARAMS ((tree, tree));
static void simplify_asm_expr		PARAMS ((tree, tree *));
static void simplify_bind_expr		PARAMS ((tree *, tree *));
static inline void remove_suffix	PARAMS ((char *, int));
static void push_gimplify_context	PARAMS ((void));
static void pop_gimplify_context	PARAMS ((void));
static void annotate_stmt_with_file_line	PARAMS ((tree *));
static tree copy_if_shared_r		PARAMS ((tree *, int *, void *));
static tree unmark_visited_r		PARAMS ((tree *, int *, void *));
static tree mostly_copy_tree_r		PARAMS ((tree *, int *, void *));
static void simplify_target_expr	PARAMS ((tree *, tree *, tree *));
static void simplify_cleanup_point_expr PARAMS ((tree *, tree *));
static bool gimple_conditional_context	PARAMS ((void));
static void gimple_push_condition	PARAMS ((void));
static void gimple_pop_condition	PARAMS ((tree *));
static void gimple_push_cleanup		PARAMS ((tree, tree *));
static void gimplify_loop_expr		PARAMS ((tree *));
static void gimplify_exit_expr		PARAMS ((tree *, tree *));

static struct gimplify_ctx
{
  tree current_bind_expr;
  tree temps;
  tree conditional_cleanups;
  int conditions;
  tree exit_label;
} *gimplify_ctxp;

static void
push_gimplify_context ()
{
  if (gimplify_ctxp)
    abort ();
  gimplify_ctxp = (struct gimplify_ctx *) xcalloc (1, sizeof (struct gimplify_ctx));
}

static void
pop_gimplify_context ()
{
  if (!gimplify_ctxp || gimplify_ctxp->current_bind_expr)
    abort ();
  free (gimplify_ctxp);
  gimplify_ctxp = NULL;
}

void
gimple_push_bind_expr (bind)
     tree bind;
{
  TREE_CHAIN (bind) = gimplify_ctxp->current_bind_expr;
  gimplify_ctxp->current_bind_expr = bind;
}

void
gimple_pop_bind_expr ()
{
  gimplify_ctxp->current_bind_expr
    = TREE_CHAIN (gimplify_ctxp->current_bind_expr);
}

tree
gimple_current_bind_expr ()
{
  return gimplify_ctxp->current_bind_expr;
}

/* Returns true iff there is a COND_EXPR between us and the innermost
   CLEANUP_POINT_EXPR.  This info is used by gimple_push_cleanup.  */

static bool
gimple_conditional_context ()
{
  return gimplify_ctxp->conditions > 0;
}

/* Note that we've entered a COND_EXPR.  */

static void
gimple_push_condition ()
{
  ++(gimplify_ctxp->conditions);
}

/* Note that we've left a COND_EXPR.  If we're back at unconditional scope
   now, add any conditional cleanups we've seen to the prequeue.  */

static void
gimple_pop_condition (pre_p)
     tree *pre_p;
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

/*  Entry point to the simplification pass.  FNDECL is the FUNCTION_DECL
    node for the function we want to simplify.  */

int
simplify_function_tree (fndecl)
     tree fndecl;
{
  tree fnbody;
  int done;
  tree oldfn;
  tree tmp;

  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return 0;
  
  /* FIXME.  Hack.  If this front end does not support simplification,
     do nothing.  */
  if (lang_hooks.simplify_expr == lhd_simplify_expr)
    return 0;

  fnbody = DECL_SAVED_TREE (fndecl);
  if (fnbody == NULL_TREE)
    return 0;

  timevar_push (TV_TREE_GIMPLIFY);

  oldfn = current_function_decl;
  current_function_decl = fndecl;

  push_gimplify_context ();

  /* Unshare most shared trees in the body.  */
  unshare_all_trees (fnbody);

  /* Simplify the function's body.  */
  done = simplify_stmt (&fnbody);

  /* Unshare again, in case simplification was sloppy.  */
  unshare_all_trees (fnbody);

  /* If there isn't an outer BIND_EXPR, add one.  */
  tmp = fnbody;
  if (TREE_CODE (tmp) != BIND_EXPR)
    {
      fnbody = build (BIND_EXPR, void_type_node, NULL_TREE, fnbody, NULL_TREE);
      TREE_SIDE_EFFECTS (fnbody) = 1;
    }

  DECL_SAVED_TREE (fndecl) = fnbody;

  /* Declare the new temporary variables.  */
  declare_tmp_vars (gimplify_ctxp->temps, fnbody);

  pop_gimplify_context ();

  current_function_decl = oldfn;

  timevar_pop (TV_TREE_GIMPLIFY);

  return done;
}

/* Simplification of expression trees.  */

/* Simplify an expression which appears at statement context; usually, this
   means replacing it with a suitably simple COMPOUND_EXPR.  */

int
simplify_stmt (stmt_p)
     tree *stmt_p;
{
  return simplify_expr (stmt_p, NULL, NULL, is_simple_stmt, fb_none);
}

/*  Simplifies the expression tree pointed by EXPR_P.  Return 0 if
    simplification failed.

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
	EXPR should be stored, or NULL if there is no suitable list.  In
	that case, we copy the result to a temporary, emit the
	post-effects, and then return the temporary.

    SIMPLE_TEST_F points to a function that takes a tree T and
	returns nonzero if T is in the SIMPLE form requested by the
	caller.  The SIMPLE predicates are in tree-simple.c.

	This test is used twice.  Before simplification, the test is
	invoked to determine whether *EXPR_P is already simple enough.  If
	that fails, *EXPR_P is simplified according to its code and
	SIMPLE_TEST_F is called again.  If the test still fails, then a new
	temporary variable is created and assigned the value of the
	simplified expression.

    FALLBACK tells the function what sort of a temporary we want.  If the 1
        bit is set, an rvalue is OK.  If the 2 bit is set, an lvalue is OK.
        If both are set, either is OK, but an lvalue is preferable.  */

int
simplify_expr (expr_p, pre_p, post_p, simple_test_f, fallback)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
     int (*simple_test_f) PARAMS ((tree));
     fallback_t fallback;
{
  tree tmp;
  tree internal_pre = NULL_TREE;
  tree internal_post = NULL_TREE;
  tree save_expr;
  int is_statement = (pre_p == NULL);
  char class;
  location_t *locus;
  const char *saved_input_filename;
  int saved_lineno;

  if (*expr_p == NULL_TREE)
    return 1;

  saved_input_filename = NULL;	/* [GIMPLE] Avoid uninitialized use warning.  */
  saved_lineno = -1;		/* [GIMPLE] Avoid uninitialized use warning.  */

  /* Set up our internal queues if needed.  */
  if (pre_p == NULL)
    pre_p = &internal_pre;
  if (post_p == NULL)
    post_p = &internal_post;

  /* Strip any uselessness.  */
  STRIP_TYPE_NOPS (*expr_p);

  class = TREE_CODE_CLASS (TREE_CODE (*expr_p));
  locus = TREE_LOCUS (*expr_p);

  if (locus
      && (IS_EXPR_CODE_CLASS (class)
	  || class == 'r'
          || class == 's'))
    {
      saved_input_filename = input_filename;
      saved_lineno = lineno;

      input_filename = TREE_FILENAME (*expr_p);
      lineno = TREE_LINENO (*expr_p);

    }

  /* Loop over the specific simplifiers until the toplevel node remains the
     same.  */
  do
    {
      /* Remember the expr.  */
      save_expr = *expr_p;

      /* Do any language-specific simplification.  */
      (*lang_hooks.simplify_expr) (expr_p, pre_p, post_p);
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
	  simplify_self_mod_expr (expr_p, pre_p, post_p);
	  break;

	case ARRAY_REF:
	  simplify_array_ref (expr_p, pre_p, post_p);
	  break;

	case COMPONENT_REF:
	  simplify_component_ref (expr_p, pre_p, post_p);
	  break;
      
	case COND_EXPR:
	  simplify_cond_expr (expr_p, pre_p, NULL_TREE);
	  break;

	case CALL_EXPR:
	  simplify_call_expr (expr_p, pre_p, post_p);
	  break;

	case TREE_LIST:
	  simplify_tree_list (expr_p, pre_p, post_p);
	  break;

	case COMPOUND_EXPR:
	  if (is_statement)
	    foreach_stmt (expr_p, (foreach_stmt_fn *)simplify_stmt);
	  else
	    simplify_compound_expr (expr_p, pre_p);
	  break;

	case REALPART_EXPR:
	case IMAGPART_EXPR:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			 simple_test_f, fallback);
	  break;

	case MODIFY_EXPR:
	case INIT_EXPR:
	  simplify_modify_expr (expr_p, pre_p, post_p, fallback != fb_none);
	  break;

	case TRUTH_ANDIF_EXPR:
	case TRUTH_ORIF_EXPR:
	  simplify_boolean_expr (expr_p, pre_p);
	  break;

	case TRUTH_NOT_EXPR:
	  tmp = TREE_OPERAND (*expr_p, 0);
	  simplify_expr (&tmp, pre_p, post_p, is_simple_id, fb_rvalue);
	  *expr_p = build (EQ_EXPR, TREE_TYPE (*expr_p), tmp, integer_zero_node);
	  recalculate_side_effects (*expr_p);
	  break;

	case ADDR_EXPR:
	  simplify_addr_expr (expr_p, pre_p, post_p);
	  break;

	/* va_arg expressions are in GIMPLE form already.  */
	case VA_ARG_EXPR:
	  break;

	case CONVERT_EXPR:
	  if (*expr_p == empty_stmt_node)
	    break;

	case NOP_EXPR:
	  if (VOID_TYPE_P (TREE_TYPE (*expr_p)))
	    {
	      /* Just strip a conversion to void and try again.  */
	      *expr_p = TREE_OPERAND (*expr_p, 0);
	      break;
	    }

	case FIX_TRUNC_EXPR:
	case FIX_CEIL_EXPR:
	case FIX_FLOOR_EXPR:
	case FIX_ROUND_EXPR:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			 is_simple_varname, fb_rvalue);
	  recalculate_side_effects (*expr_p);
	  break;

	case INDIRECT_REF:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			 is_simple_id, fb_rvalue);
	  recalculate_side_effects (*expr_p);
	  break;

	  /* Constants need not be simplified.  */
	case INTEGER_CST:
	case REAL_CST:
	case STRING_CST:
	case COMPLEX_CST:
	  break;

	  /* FIXME make this a decl.  */
	case EXC_PTR_EXPR:
	  break;

	case BIND_EXPR:
	  simplify_bind_expr (expr_p, pre_p);
	  break;

	case LOOP_EXPR:
	  gimplify_loop_expr (expr_p);
	  break;

	case SWITCH_EXPR:
	  simplify_expr (&SWITCH_COND (*expr_p), pre_p, NULL,
			 is_simple_val, fb_rvalue);
	  simplify_stmt (&SWITCH_BODY (*expr_p));
	  break;

	case LABELED_BLOCK_EXPR:
	  simplify_stmt (&LABELED_BLOCK_BODY (*expr_p));
	  recalculate_side_effects (*expr_p);
	  break;

	case EXIT_EXPR:
	  gimplify_exit_expr (expr_p, pre_p);
	  break;

	case GOTO_EXPR:
	  {
	    tree dest = GOTO_DESTINATION (*expr_p);

	    /* If this label is in a different context (function), then
	       mark it as a nonlocal label and mark its context as
	       receiving nonlocal gotos.  */
	    if (TREE_CODE (dest) == LABEL_DECL
		&& current_function_decl != decl_function_context (dest))
	      {
		tree context = decl_function_context (dest);

		NONLOCAL_LABEL (dest) = 1;
		FUNCTION_RECEIVES_NONLOCAL_GOTO (context) = 1;
	      }
	
	    /* If the target is not LABEL, then it is a computed jump
	       and the target needs to be simplified.  */
	    if (TREE_CODE (GOTO_DESTINATION (*expr_p)) != LABEL_DECL)
	      simplify_expr (&GOTO_DESTINATION (*expr_p), pre_p, NULL,
			     is_simple_val, fb_rvalue);

	    break;
	  }

	case LABEL_EXPR:
	case CASE_LABEL_EXPR:
	  break;

	case RETURN_EXPR:
	  simplify_return_expr (*expr_p, pre_p);
	  break;

	case CONSTRUCTOR:
	  simplify_constructor (*expr_p, pre_p, post_p);
	  break;

	  /* The following are special cases that are not handled by the
	     original SIMPLE grammar.  */

	  /* SAVE_EXPR nodes are converted into a SIMPLE identifier and
	     eliminated.  */
	case SAVE_EXPR:
	  simplify_save_expr (expr_p, pre_p);
	  break;

	case BIT_FIELD_REF:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
		         is_simple_min_lval, fb_either);
	  simplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p,
			 is_simple_val, fb_rvalue);
	  simplify_expr (&TREE_OPERAND (*expr_p, 2), pre_p, post_p,
			 is_simple_val, fb_rvalue);
	  break;

	case NON_LVALUE_EXPR:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			 simple_test_f, fb_rvalue);
	  recalculate_side_effects (*expr_p);
	  break;

	case ASM_EXPR:
	  simplify_asm_expr (*expr_p, pre_p);
	  break;

	case TRY_FINALLY_EXPR:
	case TRY_CATCH_EXPR:
	  simplify_stmt (&TREE_OPERAND (*expr_p, 0));
	  simplify_stmt (&TREE_OPERAND (*expr_p, 1));
	  break;

	case CLEANUP_POINT_EXPR:
	  simplify_cleanup_point_expr (expr_p, pre_p);
	  break;

	case TARGET_EXPR:
	  simplify_target_expr (expr_p, pre_p, post_p);
	  break;

	case CATCH_EXPR:
	  simplify_stmt (&CATCH_BODY (*expr_p));
	  break;

	case EH_FILTER_EXPR:
	  simplify_stmt (&EH_FILTER_FAILURE (*expr_p));
	  break;

	case VTABLE_REF:
	  /* This moves much of the actual computation out of the
	     VTABLE_REF.  Perhaps this should be revisited once we want to
	     do clever things with VTABLE_REFs.  */
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			 is_simple_min_lval, fb_lvalue);
	  break;

	  /* FIXME
	case MIN_EXPR:
	case MAX_EXPR:
	  simplify_minimax_expr (expr_p, pre_p);
	  break; */

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
		simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_simple_val, fb_rvalue);
		recalculate_side_effects (*expr_p);
	      }
	    else if (TREE_CODE_CLASS (TREE_CODE (*expr_p)) == '2'
		     || TREE_CODE_CLASS (TREE_CODE (*expr_p)) == '<'
		     || TREE_CODE (*expr_p) == TRUTH_AND_EXPR
		     || TREE_CODE (*expr_p) == TRUTH_OR_EXPR
		     || TREE_CODE (*expr_p) == TRUTH_XOR_EXPR)
	      {
		simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_simple_val, fb_rvalue);

		simplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p,
			       is_simple_val, fb_rvalue);
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
  /* If we replaced *expr_p, simplify again.  */
  while (*expr_p && *expr_p != save_expr);

  /* If we are simplifying at the statement level, we're done.  Tack
     everything together and replace the original statement with the
     simplified form.  */
  if (is_statement)
    {
      add_tree (*expr_p, pre_p);
      annotate_all_with_file_line (&internal_post, input_filename, lineno);
      add_tree (internal_post, pre_p);
      tmp = rationalize_compound_expr (internal_pre);
      annotate_all_with_file_line (&tmp, input_filename, lineno);
      *expr_p = tmp;

      if (locus
	  && (IS_EXPR_CODE_CLASS (class)
	      || class == 'r'
              || class == 's'))
	{
	  input_filename = saved_input_filename;
	  lineno = saved_lineno;
	}
      return 1;
    }

  /* Otherwise we're simplifying a subexpression, so the resulting value is
     interesting.  */

  /* If it's sufficiently simple already, we're done.  Unless we are
     handling some post-effects internally; if that's the case, we need to
     copy into a temp before adding the post-effects to the tree.  */
  if (!internal_post && (*simple_test_f) (*expr_p))
    {
      if (locus
	  && (IS_EXPR_CODE_CLASS (class)
	      || class == 'r'
              || class == 's'))
        {
          input_filename = saved_input_filename;
          lineno = saved_lineno;
        }
      return 1;
    }

  /* Otherwise, we need to create a new temporary for the simplified
     expression.  */

  /* We can't return an lvalue if we have an internal postqueue.  The
     object the lvalue refers to would (probably) be modified by the
     postqueue; we need to copy the value out first, which means an
     rvalue.  */
  if ((fallback & fb_lvalue) && !internal_post && is_simple_varname (*expr_p))
    {
      /* An lvalue will do.  Take the address of the expression, store it
	 in a temporary, and replace the expression with an INDIRECT_REF of
	 that temporary.  */
      tmp = build_addr_expr (*expr_p);
      simplify_expr (&tmp, pre_p, post_p, is_simple_id, fb_rvalue);
      *expr_p = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (tmp)), tmp);
    }
  else if ((fallback & fb_rvalue) && is_simple_rhs (*expr_p))
    {
#if defined ENABLE_CHECKING
      if (VOID_TYPE_P (TREE_TYPE (*expr_p)))
	abort ();
#endif

      /* An rvalue will do.  Assign the simplified expression into a new
	 temporary TMP and replace the original expression with TMP.  */
      *expr_p = get_initialized_tmp_var (*expr_p, pre_p);
    }
  else
    {
      fprintf (stderr, "simplification failed:\n");
      print_generic_expr (stderr, *expr_p, 0);
      debug_tree (*expr_p);
      abort ();
    }

#if defined ENABLE_CHECKING
  /* Make sure the temporary matches our predicate.  */
  if (!(*simple_test_f) (*expr_p))
    abort ();
#endif

  if (internal_post)
    {
      annotate_all_with_file_line (&internal_post, input_filename, lineno);
      add_tree (internal_post, pre_p);
    }

  if (locus
      && (IS_EXPR_CODE_CLASS (class)
	  || class == 'r'
          || class == 's'))
    {
      input_filename = saved_input_filename;
      lineno = saved_lineno;
    }

  return 1;
}

/* WRAPPER is a code such as BIND_EXPR or CLEANUP_POINT_EXPR which can both
   contain statements and have a value.  Assign its value to a temporary
   and give it void_type_node.  Returns the temporary, or NULL_TREE if
   WRAPPER was already void.  */

tree
voidify_wrapper_expr (wrapper)
     tree wrapper;
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
	}
      else
	{
	  temp = create_tmp_var (TREE_TYPE (wrapper), "retval");
	  if (*p != empty_stmt_node)
	    *p = build (MODIFY_EXPR, TREE_TYPE (temp), temp, *p);
	}

      TREE_TYPE (wrapper) = void_type_node;
      return temp;
    }

  return NULL_TREE;
}

/* Simplify a BIND_EXPR.  Just voidify and recurse.  */

static void
simplify_bind_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  tree bind_expr = *expr_p;
  tree temp = voidify_wrapper_expr (bind_expr);

  gimple_push_bind_expr (bind_expr);
  simplify_stmt (&BIND_EXPR_BODY (bind_expr));
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

/* Simplify a RETURN_EXPR.  If the expression to be returned is not a
   SIMPLE value, it is assigned to a new temporary and the statement is
   re-written to return the temporary.

   PRE_P points to the list where side effects that must happen before
       STMT should be stored.  */

static void
simplify_return_expr (stmt, pre_p)
     tree stmt;
     tree *pre_p;
{
  tree ret_expr = TREE_OPERAND (stmt, 0);

  if (ret_expr)
    {
      /* We need to pass the full MODIFY_EXPR down so that special handling
	 can replace it with something else.  FIXME this code is way too
	 complicated.  */
      if (VOID_TYPE_P (TREE_TYPE (TREE_TYPE (current_function_decl))))
	{
	  /* We are trying to return an expression in a void function; move
	     the expression to before the return.   Note that RET_EXPR
	     might be a POSTINCREMENT_EXPR or similar expression which
	     requires even more special handling.  Ugh.  */
	  simplify_expr (&ret_expr, pre_p, NULL, is_simple_stmt, fb_either);
	  add_tree (ret_expr, pre_p);
	  TREE_OPERAND (stmt, 0) = NULL_TREE;
	  return;
	}

      simplify_expr (&ret_expr, pre_p, NULL, is_simple_stmt, fb_none);
      if (TREE_CODE (ret_expr) != MODIFY_EXPR)
	{
	  /* We're returning a value that is not necessarily a bitwise
	     copy.  As in the previous case, move the expression to before
	     the return.  */
	  add_tree (ret_expr, pre_p);
	  TREE_OPERAND (stmt, 0) = NULL_TREE;
	}
      else
	{
	  /* Make sure the RHS is really simple.  */
	  simplify_expr (&TREE_OPERAND (ret_expr, 1), pre_p, NULL,
			 is_simple_rhs, fb_rvalue);
	  TREE_OPERAND (stmt, 0) = ret_expr;
	}
    }
}

/* Simplify a LOOP_EXPR.  Normally this just involves simplifying the body,
   but if the loop contains an EXIT_EXPR, we need to append a label for it
   to jump to.  */

static void
gimplify_loop_expr (expr_p)
     tree *expr_p;
{
  tree saved_label = gimplify_ctxp->exit_label;
  gimplify_ctxp->exit_label = NULL_TREE;

  simplify_stmt (&LOOP_EXPR_BODY (*expr_p));

  if (gimplify_ctxp->exit_label)
    {
      tree expr = build1 (LABEL_EXPR, void_type_node,
			  gimplify_ctxp->exit_label);
      add_tree (expr, expr_p);
      gimplify_ctxp->exit_label = saved_label;
    }
}

/* Simplify an EXIT_EXPR by converting to a GOTO_EXPR inside a COND_EXPR.
   This also involves building a label to jump to and communicating it to
   gimplify_loop_expr through gimplify_ctxp->exit_label.  */

static void
gimplify_exit_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  tree cond = TREE_OPERAND (*expr_p, 0);
  tree label = gimplify_ctxp->exit_label;
  tree expr;

  if (label == NULL_TREE)
    {
      label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
      DECL_ARTIFICIAL (label) = 1;
      TREE_USED (label) = 1;
      DECL_CONTEXT (label) = current_function_decl;
      gimplify_ctxp->exit_label = label;
    }

  simplify_expr (&cond, pre_p, NULL, is_simple_condexpr, fb_rvalue);

  expr = build1 (GOTO_EXPR, void_type_node, label);
  expr = build (COND_EXPR, void_type_node, cond, expr, empty_stmt_node);
  *expr_p = expr;
}

/* Simplifies a CONSTRUCTOR node T.

   FIXME: Should dynamic initializations from a CONSTRUCTOR be broken
   up into multiple assignments?  */

static void
simplify_constructor (t, pre_p, post_p)
     tree t;
     tree *pre_p;
     tree *post_p;
{
  tree elt_list;

  for (elt_list = CONSTRUCTOR_ELTS (t); elt_list;
       elt_list = TREE_CHAIN (elt_list))
    simplify_expr (&TREE_VALUE (elt_list), pre_p, post_p,
		   is_simple_constructor_elt, fb_rvalue);
}

/*  Build an expression for the address of T.  Folds away INDIRECT_REF to
    avoid confusing the simplify process.  */

static tree
build_addr_expr_with_type (t, ptrtype)
     tree t;
     tree ptrtype;
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
build_addr_expr (t)
     tree t;
{
  return build_addr_expr_with_type (t, build_pointer_type (TREE_TYPE (t)));
}

  
/*  Re-write the ARRAY_REF node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
	*EXPR_P should be stored.

    FIXME: ARRAY_REF currently doesn't accept a pointer as the array
    argument, so this simplification uses an INDIRECT_REF of ARRAY_TYPE.
    ARRAY_REF should be extended.  */

static void
simplify_array_ref (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
#if 1
  tree elttype = TREE_TYPE (TREE_TYPE (TREE_OPERAND (*expr_p, 0)));
  if (!TREE_CONSTANT (TYPE_SIZE_UNIT (elttype)))
    /* If the size of the array elements is not constant,
       computing the offset is non-trivial, so expose it.  */
    simplify_array_ref_to_plus (expr_p, pre_p, post_p);
  else
    /* Handle array and member refs together for now.  When alias analysis
       improves, we may want to go back to handling them separately.  */
    simplify_compound_lval (expr_p, pre_p, post_p);
#else
  tree *p;
  varray_type dim_stack;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != ARRAY_REF)
    abort ();
#endif

  VARRAY_GENERIC_PTR_INIT (dim_stack, 10, "dim_stack");

  /* Create a stack with all the dimensions of the array so that they can
     be simplified from left to right.  */
  for (p = expr_p; TREE_CODE (*p) == ARRAY_REF; p = &TREE_OPERAND (*p, 0))
    VARRAY_PUSH_GENERIC_PTR (dim_stack, (PTR) &TREE_OPERAND (*p, 1));

  /* After the loop above, 'p' points to the first non-ARRAY_REF,
     and 'dim_stack' is a stack of pointers to all the dimensions in left
     to right order (the leftmost dimension is at the top of the stack).

     Simplify the base, and then each of the dimensions from left to
     right.  */

  simplify_expr (p, pre_p, post_p, is_simple_min_lval, fb_lvalue);

  for (; VARRAY_ACTIVE_SIZE (dim_stack) > 0; VARRAY_POP (dim_stack))
    {
      tree *dim_p = (tree *)VARRAY_TOP_GENERIC_PTR (dim_stack);
      simplify_expr (dim_p, pre_p, post_p, is_simple_val, fb_rvalue);
    }
#endif
}

/* Subroutine of simplify_compound_lval.  Converts an ARRAY_REF to the
   equivalent *(&array + offset) form.  */

static void
simplify_array_ref_to_plus (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  tree array = TREE_OPERAND (*expr_p, 0);
  tree idx = convert (sizetype, TREE_OPERAND (*expr_p, 1));
  tree elttype = TREE_TYPE (TREE_TYPE (array));
  tree size = size_in_bytes (elttype);
  tree ptrtype = build_pointer_type (elttype);
  tree offset = size_binop (MULT_EXPR, size, idx);
  tree addr, result;

  simplify_expr (&array, pre_p, post_p, is_simple_min_lval, fb_lvalue);

  addr = build_addr_expr_with_type (array, ptrtype);
  result = fold (build (PLUS_EXPR, ptrtype, addr, offset));
  *expr_p = build1 (INDIRECT_REF, elttype, result);
}

/* Simplify the COMPONENT_REF or ARRAY_REF node pointed by EXPR_P.

   PRE_P points to the list where side effects that must happen before
     *EXPR_P should be stored.

   POST_P points to the list where side effects that must happen after
     *EXPR_P should be stored.  */

static void
simplify_compound_lval (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  tree *p;
  enum tree_code code;
  varray_type dim_stack;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != ARRAY_REF && TREE_CODE (*expr_p) != COMPONENT_REF)
    abort ();
#endif

  code = ERROR_MARK;	/* [GIMPLE] Avoid uninitialized use warning.  */

  /* Create a stack with all the array dimensions so that they can be
     simplified from left to right (to match user expectations).  */
  VARRAY_GENERIC_PTR_INIT (dim_stack, 10, "dim_stack");

  for (p = expr_p;
       TREE_CODE (*p) == ARRAY_REF || TREE_CODE (*p) == COMPONENT_REF;
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
	  VARRAY_PUSH_GENERIC_PTR (dim_stack, (PTR) &TREE_OPERAND (*p, 1));
	}
    }

  /* Now 'p' points to the first bit that isn't an ARRAY_REF or
     COMPONENT_REF, 'code' is the TREE_CODE of the last bit that was, and
     'dim_stack' is a stack of pointers to all the dimensions in left to
     right order (the leftmost dimension is at the top of the stack).

     Simplify the base, and then each of the dimensions from left to
     right.  */
  simplify_expr (p, pre_p, post_p, is_simple_min_lval,
		 code == COMPONENT_REF ? fb_either : fb_lvalue);

  for (; VARRAY_ACTIVE_SIZE (dim_stack) > 0; VARRAY_POP (dim_stack))
    {
      tree *dim_p = (tree *)VARRAY_TOP_GENERIC_PTR (dim_stack);
      simplify_expr (dim_p, pre_p, post_p, is_simple_val, fb_rvalue);
    }

  recalculate_side_effects (*expr_p);
}

/*  Simplify the self modifying expression pointed by EXPR_P (++, --, +=, -=).

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_self_mod_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  enum tree_code code;
  tree lhs, lvalue, rhs, t1;

  code = TREE_CODE (*expr_p);

#if defined ENABLE_CHECKING
  if (code != POSTINCREMENT_EXPR
      && code != POSTDECREMENT_EXPR
      && code != PREINCREMENT_EXPR
      && code != PREDECREMENT_EXPR)
    abort ();
#endif

  /* Simplify the LHS into a SIMPLE lvalue.  */
  lvalue = TREE_OPERAND (*expr_p, 0);
  simplify_expr (&lvalue, pre_p, post_p, is_simple_modify_expr_lhs,
		 fb_lvalue);

  /* Extract the operands to the arithmetic operation, including an rvalue
     version of our LHS.  */
  lhs = lvalue;
  /* And reduce it to an ID.  */
  simplify_expr (&lhs, pre_p, post_p, is_simple_id, fb_rvalue);
  rhs = TREE_OPERAND (*expr_p, 1);
  simplify_expr (&rhs, pre_p, post_p, is_simple_val, fb_rvalue);

  /* Determine whether we need to create a PLUS or a MINUS operation.  */
  if (code == PREINCREMENT_EXPR || code == POSTINCREMENT_EXPR)
    t1 = build (PLUS_EXPR, TREE_TYPE (*expr_p), lhs, rhs);
  else
    t1 = build (MINUS_EXPR, TREE_TYPE (*expr_p), lhs, rhs);

#if defined ENABLE_CHECKING
  if (!is_simple_binary_expr (t1))
    abort ();
#endif

  t1 = build (MODIFY_EXPR, TREE_TYPE (lvalue), lvalue, t1);
  if (TREE_LOCUS (*expr_p))
    TREE_LOCUS (t1) = TREE_LOCUS (*expr_p);
  else
    annotate_with_file_line (t1, input_filename, lineno);

  /* Determine whether the new assignment should go before or after
     the simplified expression.  */
  if (code == PREINCREMENT_EXPR || code == PREDECREMENT_EXPR)
    add_tree (t1, pre_p);
  else
    add_tree (t1, post_p);

  /* Replace the original expression with the LHS of the assignment.  */
  *expr_p = lvalue;
}


/*  Simplify the COMPONENT_REF node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_component_ref (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
#if 1
  /* Handle array and member refs together for now.  When alias analysis
     improves, we may want to go back to handling them separately.  */
  simplify_compound_lval (expr_p, pre_p, post_p);
#else
  tree *p;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != COMPONENT_REF)
    abort ();
#endif

  for (p = expr_p; TREE_CODE (*p) == COMPONENT_REF; p = &TREE_OPERAND (*p, 0))
    if (! is_simple_id (TREE_OPERAND (*p, 1)))
      /* The RHS of a COMPONENT_REF should always be a FIELD_DECL.  */
      abort ();

  /* Now we're down to the first bit that isn't a COMPONENT_REF.  */
  simplify_expr (p, pre_p, post_p, is_simple_min_lval, fb_either);
#endif
}


/*  Simplify the CALL_EXPR node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
    	*EXPR_P should be stored.  */

static void
simplify_call_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != CALL_EXPR)
    abort ();
#endif

  /* Some builtins cannot be simplified because they require specific
     arguments (e.g., MD builtins).  */
  if (!is_simplifiable_builtin (*expr_p))
    {
      /* Mark the whole expression not simplifiable.  */
      mark_not_simple (expr_p);
      return;
    }

  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p, is_simple_id,
                 fb_rvalue);
  simplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p, is_simple_arglist,
                 fb_rvalue);
}

/*  Simplify the TREE_LIST node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_tree_list (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  tree op;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != TREE_LIST)
    abort ();
#endif

  for (op = *expr_p; op; op = TREE_CHAIN (op))
    simplify_expr (&TREE_VALUE (op), pre_p, post_p, is_simple_val,
		   fb_rvalue);
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
simplify_cond_expr (expr_p, pre_p, target)
     tree *expr_p;
     tree *pre_p;
     tree target;
{
  tree tmp = NULL_TREE;
  tree expr = *expr_p;

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
      }

  /* Turn if (a && b) into if (a) if (b).  This only works if there is no
     'else'.  */
  if (! TREE_SIDE_EFFECTS (TREE_OPERAND (expr, 2)))
    while (TREE_CODE (TREE_OPERAND (expr, 0)) == TRUTH_ANDIF_EXPR)
      {
	tree pred = TREE_OPERAND (expr, 0);
	TREE_OPERAND (expr, 0) = TREE_OPERAND (pred, 1);
	expr = build (COND_EXPR, void_type_node, TREE_OPERAND (pred, 0),
		      expr, empty_stmt_node);
      }

  /* Now do the normal simplification.  */
  simplify_expr (&TREE_OPERAND (expr, 0), pre_p, NULL,
		 is_simple_condexpr, fb_rvalue);

  gimple_push_condition ();
  
  simplify_stmt (&TREE_OPERAND (expr, 1));
  simplify_stmt (&TREE_OPERAND (expr, 2));

  gimple_pop_condition (pre_p);

  /* If we had a value, move the COND_EXPR to the prequeue and use the temp
     in its place.  */
  if (tmp)
    {
      add_tree (expr, pre_p);
      *expr_p = tmp;
    }
  else
    *expr_p = expr;
}


/*  Simplify the MODIFY_EXPR node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_modify_expr (expr_p, pre_p, post_p, want_value)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
     int want_value;
{
  tree *from_p = &TREE_OPERAND (*expr_p, 1);
  tree *to_p = &TREE_OPERAND (*expr_p, 0);
  tree type = TREE_TYPE (*to_p);
  
#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != MODIFY_EXPR
      && TREE_CODE (*expr_p) != INIT_EXPR)
    abort ();
#endif

  simplify_expr (to_p, pre_p, post_p, is_simple_modify_expr_lhs, fb_lvalue);

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
      simplify_cond_expr (from_p, pre_p, *to_p);
      *expr_p = *from_p;
      /* Try again.  */
      return;
    }

  /* The distinction between MODIFY_EXPR and INIT_EXPR is no longer
     useful.  */
  if (TREE_CODE (*expr_p) == INIT_EXPR)
    TREE_SET_CODE (*expr_p, MODIFY_EXPR);

  /* Break out elements of a constructor into separate MODIFY_EXPRs.  FIXME
     should also handle vectors.

     Note that we still need to clear any elements that don't have explicit
     initializers, so we keep the original MODIFY_EXPR, we just remove all
     of the constructor elements.  FIXME should try to avoid clearing
     initialized elts, a la store_constructor.  */
  if (TREE_CODE (*from_p) == CONSTRUCTOR
      && (TREE_CODE (type) == RECORD_TYPE || TREE_CODE (type) == UNION_TYPE
	  || TREE_CODE (type) == QUAL_UNION_TYPE
	  || TREE_CODE (type) == ARRAY_TYPE))
    {
      tree elt_list = CONSTRUCTOR_ELTS (*from_p);
      int i;

      CONSTRUCTOR_ELTS (*from_p) = NULL_TREE;
      add_tree (*expr_p, pre_p);

      for (i = 0; elt_list; i++, elt_list = TREE_CHAIN (elt_list))
	{
	  tree purpose, value, cref, init;

	  purpose = TREE_PURPOSE (elt_list);
	  value = TREE_VALUE (elt_list);

	  if (TREE_CODE (type) == ARRAY_TYPE)
	    {
	      tree type = TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (*to_p)));
	      cref = build (ARRAY_REF, type, *to_p, build_int_2 (i, 0));
	    }
	  else
	    {
	      cref = build (COMPONENT_REF, TREE_TYPE (purpose), *to_p, purpose);
	    }

	  init = build (MODIFY_EXPR, TREE_TYPE (purpose), cref, value);
	  simplify_expr (&init, pre_p, NULL, is_simple_stmt, fb_none);
	  add_tree (init, pre_p);
	}
      *expr_p = TREE_OPERAND (*expr_p, 0);
      return;
    }

  /* If this is for a RETURN_EXPR, we can't have any posteffects.  */
  if (!want_value)
    post_p = NULL;

  simplify_expr (from_p, pre_p, post_p, is_simple_rhs, fb_rvalue);

  if (want_value)
    {
      add_tree (*expr_p, pre_p);
      *expr_p = TREE_OPERAND (*expr_p, 0);
    }
}


/*  Simplify TRUTH_ANDIF_EXPR and TRUTH_ORIF_EXPR expressions.  EXPR_P
    points to the expression to simplify.

    Expressions of the form 'a && b' are simplified to:

    	T = a;
	if (T)
	  T = b;

    Expressions of the form 'a || b' are simplified to:

	T = a;
	if (!T)
	  T = b;

    In both cases, the expression is re-written as 'T != 0'.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_boolean_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  enum tree_code code;
  tree t, lhs, rhs, if_body, if_cond, if_stmt;

  code = TREE_CODE (*expr_p);

#if defined ENABLE_CHECKING
  if (code != TRUTH_ANDIF_EXPR && code != TRUTH_ORIF_EXPR)
    abort ();
#endif

  /* Make this expression right-associative so that simplification will
     produce nested ifs.  */
  *expr_p = right_assocify_expr (*expr_p);

  /* First, make sure that our operands are truthvalues.  This should
     already be the case, but they may have the wrong type.  */
  lhs = (*lang_hooks.truthvalue_conversion) (TREE_OPERAND (*expr_p, 0));
  rhs = (*lang_hooks.truthvalue_conversion) (TREE_OPERAND (*expr_p, 1));

  /* Build 'T = a'  */
  t = get_initialized_tmp_var (lhs, pre_p);

  /* Build the body for the if() statement that conditionally evaluates the
     RHS of the expression.  Note that we first build the assignment
     surrounded by a new scope so that its simplified form is computed
     inside the new scope.  */
  if_body = build (MODIFY_EXPR, TREE_TYPE (t), t, rhs);

  /* Build the statement 'if (T = a <comp> 0) T = b;'.  Where <comp> is
     NE_EXPR if we are processing && and EQ_EXPR if we are processing ||.

     Note that we are deliberately creating a non SIMPLE statement to
     explicitly expose the sequence points to the simplifier.  When the
     resulting if() statement is simplified, the side effects for the LHS
     of 'a && b' will be inserted before the evaluation of 'b'.  */
  if (code == TRUTH_ANDIF_EXPR)
    if_cond = t;
  else
    if_cond = build (EQ_EXPR, TREE_TYPE (t), t, integer_zero_node);

  if_stmt = build (COND_EXPR, void_type_node, if_cond, if_body,
		   empty_stmt_node);
  /* Simplify the IF_STMT and insert it in the PRE_P chain.  */
  simplify_stmt (&if_stmt);
  add_tree (if_stmt, pre_p);

  /* If we're not actually looking for a boolean result, convert now.  */
  if (TREE_TYPE (t) != TREE_TYPE (*expr_p))
    t = convert (TREE_TYPE (*expr_p), t);

  /* Re-write the original expression to use T.  */
  *expr_p = t;
}


/*  Simplifies an expression sequence.  This function simplifies each
    expression and re-writes the original expression with the last
    expression of the sequence in SIMPLE form.

    PRE_P points to the list where the side effects for all the expressions
	in the sequence will be emitted.

    POST_P points to the list where the post side effects for the last
        expression in the sequence are emitted.  */

static void
simplify_compound_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  tree t;
  for (t = *expr_p; TREE_CODE (t) == COMPOUND_EXPR; t = TREE_OPERAND (t, 1))
    {
      tree sub = TREE_OPERAND (t, 0);
      simplify_stmt (&sub);
      add_tree (sub, pre_p);
    }

  *expr_p = t;
}


/*  Simplify a SAVE_EXPR node.  EXPR_P points to the expression to
    simplify.  After simplification, EXPR_P will point to a new temporary
    that holds the original value of the SAVE_EXPR node.

    PRE_P points to the list where side effects that must happen before
        *EXPR_P should be stored.  */

static void
simplify_save_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != SAVE_EXPR)
    abort ();
#endif

  /* If the operand is already a SIMPLE temporary, just re-write the
     SAVE_EXPR node.  */
  if (is_simple_tmp_var (TREE_OPERAND (*expr_p, 0)))
    *expr_p = TREE_OPERAND (*expr_p, 0);
  else
    {
      TREE_OPERAND (*expr_p, 0) =
	get_initialized_tmp_var (TREE_OPERAND (*expr_p, 0), pre_p);
      *expr_p = TREE_OPERAND (*expr_p, 0);
    }
}

/*  Re-write the ADDR_EXPR node pointed by EXPR_P

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
	*EXPR_P should be stored.  */

static void
simplify_addr_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  /* Check if we are dealing with an expression of the form '&*ptr'.
     While the front end folds away '&*ptr' into 'ptr', these
     expressions may be generated internally by the compiler (e.g.,
     builtins like __builtin_va_end).  */
  if (TREE_CODE (TREE_OPERAND (*expr_p, 0)) != INDIRECT_REF)
    {
      simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p, 
		     is_simple_addr_expr_arg, fb_lvalue);
      TREE_SIDE_EFFECTS (*expr_p)
	= TREE_SIDE_EFFECTS (TREE_OPERAND (*expr_p, 0));

      /* Mark the RHS addressable.  */
      (*lang_hooks.mark_addressable) (TREE_OPERAND (*expr_p, 0));
    }
  else
    /* Fold &*EXPR into EXPR.  simplify_expr will re-simplify EXPR.  */
    *expr_p = TREE_OPERAND (TREE_OPERAND (*expr_p, 0), 0);
}

/* Simplify the operands of an ASM_EXPR.  Input operands should be a simple
   value; output operands should be a simple lvalue.  */

static void
simplify_asm_expr (expr, pre_p)
     tree expr;
     tree *pre_p;
{
  tree link;

  for (link = ASM_INPUTS (expr); link; link = TREE_CHAIN (link))
    simplify_expr (&TREE_VALUE (link), pre_p, NULL,
		   is_simple_val, fb_rvalue);

  for (link = ASM_OUTPUTS (expr); link; link = TREE_CHAIN (link))
    simplify_expr (&TREE_VALUE (link), pre_p, NULL,
		   is_simple_modify_expr_lhs, fb_lvalue);
}

/* Apply FN to each statement under *STMT_P, which may be a COMPOUND_EXPR
   or a single statement expr.  */

void
foreach_stmt (stmt_p, fn)
     tree *stmt_p;
     foreach_stmt_fn *fn;
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

static const char *wfl_filename;
static int wfl_lineno;

static void
annotate_stmt_with_file_line (stmt_p)
     tree *stmt_p;
{
  /* Don't emit a line note for a label.  We particularly don't want to
     emit one for the break label, since it doesn't actually correspond
     to the beginning of the loop/switch.  */;
  if (TREE_CODE (*stmt_p) != LABEL_EXPR
      && ! TREE_LOCUS (*stmt_p))
    annotate_with_file_line (*stmt_p, wfl_filename, wfl_lineno);
}

void
annotate_all_with_file_line (stmt_p, file, line)
     tree *stmt_p;
     const char *file;
     int line;
{
  wfl_filename = file;
  wfl_lineno = line;
  foreach_stmt (stmt_p, annotate_stmt_with_file_line);
}

/* Add STMT to EXISTING if possible, otherwise create a new
   COMPOUND_EXPR and add STMT to it. */

static tree
add_stmt_to_compound (existing, stmt)
     tree existing, stmt;
{
  if (!stmt || !TREE_SIDE_EFFECTS (stmt))
    return existing;
  else if (existing)
    return build (COMPOUND_EXPR, void_type_node, existing, stmt);
  else
    return stmt;
}

/*  Add T to the list container pointed by LIST_P.  If T is a TREE_LIST
    node, it is linked-in directly.  If T is an expression with no effects,
    it is ignored.
    
    Return the newly added list node or NULL_TREE if T was not added to
    LIST_P.  */

tree
add_tree (t, list_p)
    tree t;
    tree *list_p;
{
  if (t == NULL_TREE || !TREE_SIDE_EFFECTS (t))
    return NULL_TREE;

  *list_p = add_stmt_to_compound (*list_p, t);
  return t;
}


/*  Create a new temporary variable declaration of type TYPE.  Returns the
    newly created decl and pushes it into the current binding.  */

tree
create_tmp_var (type, prefix)
     tree type;
     const char *prefix;
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

#if defined ENABLE_CHECKING
  /* If the type is an array or a type which must be created by the
     frontend, something is wrong.  */
  if (TREE_CODE (type) == ARRAY_TYPE || TREE_ADDRESSABLE (type))
    abort ();
#endif

  /* Make the type of the variable writable.  */
  type = build_type_variant (type, 0, 0);

  tmp_var = build_decl (VAR_DECL, get_identifier (tmp_name), type);

  /* The variable was declared by the compiler.  */
  DECL_ARTIFICIAL (tmp_var) = 1;

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
create_tmp_alias_var (type, prefix)
     tree type;
     const char *prefix;
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

#if defined ENABLE_CHECKING
  /* If the type is an array, something is wrong.  */
  if (TREE_CODE (type) == ARRAY_TYPE)
    abort ();
#endif

  /* Make the type of the variable writable.  */
  type = build_type_variant (type, 0, 0);

  tmp_var = build_decl (VAR_DECL, get_identifier (tmp_name), type);

  /* The variable was declared by the compiler.  */
  DECL_ARTIFICIAL (tmp_var) = 1;

  /* Make the variable writable.  */
  TREE_READONLY (tmp_var) = 0;

  DECL_EXTERNAL (tmp_var) = 0;
  TREE_STATIC (tmp_var) = 0;
  TREE_USED (tmp_var) = 1;


  return tmp_var;
}

/*  Given a tree, try to return a useful variable name that we can use
    to prefix a temporary that is being assigned the value of the tree. 
    I.E. given  <temp> = &A, return A.  */

const char *
get_name (t)
     tree t;
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


/*  Returns a new temporary variable, initialized with VAL.  PRE_P and STMT
    are as in simplify_expr.  */

tree
get_initialized_tmp_var (val, pre_p)
     tree val;
     tree *pre_p;
{
  tree t, mod;
  const char *prefix = NULL;
  
  prefix = get_name (val);
  simplify_expr (&val, pre_p, NULL, is_simple_rhs, fb_rvalue);
  t = create_tmp_var (TREE_TYPE (val), prefix);
  mod = build (MODIFY_EXPR, TREE_TYPE (t), t, val);
  if (TREE_LOCUS (val))
    TREE_LOCUS (mod) = TREE_LOCUS (val);
  else
    annotate_with_file_line (mod, input_filename, lineno);
  add_tree (mod, pre_p);

  return t;
}

/*  Returns true if T is a SIMPLE temporary variable, false otherwise.  */

bool
is_simple_tmp_var (t)
     tree t;
{
  /* FIXME this could trigger for other local artificials, too.  */
  return (TREE_CODE (t) == VAR_DECL && DECL_ARTIFICIAL (t)
	  && !TREE_STATIC (t) && !DECL_EXTERNAL (t));
}

/*  Declares all the variables in VARS in SCOPE.  Returns the last
    DECL_STMT emitted.  */

void
declare_tmp_vars (vars, scope)
     tree vars;
     tree scope;
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
gimple_add_tmp_var (tmp)
     tree tmp;
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
remove_suffix (name, len)
     char *name;
     int len;
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
unshare_all_trees (t)
     tree t;
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
   simplification process would create wrong code.  */

static tree
copy_if_shared_r (tp, walk_subtrees, data)
    tree *tp;
    int *walk_subtrees ATTRIBUTE_UNUSED;
    void *data ATTRIBUTE_UNUSED;
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
unmark_visited_r (tp, walk_subtrees, data)
    tree *tp;
    int *walk_subtrees ATTRIBUTE_UNUSED;
    void *data ATTRIBUTE_UNUSED;
{
  if (TREE_VISITED (*tp))
    TREE_VISITED (*tp) = 0;
  else
    *walk_subtrees = 0;

  return NULL_TREE;
}

/* Similar to copy_tree_r() but do not copy SAVE_EXPR nodes.  These nodes
   model computations that should only be done once.  If we were to unshare
   something like SAVE_EXPR(i++), the simplification process would create
   wrong code.  */

static tree
mostly_copy_tree_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees;
     void *data;
{
  enum tree_code code = TREE_CODE (*tp);
  /* Don't unshare types and SAVE_EXPR nodes.  */
  if (TREE_CODE_CLASS (code) == 't'
      || code == SAVE_EXPR
      || *tp == empty_stmt_node)
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
unshare_expr (expr)
     tree expr;
{
  walk_tree (&expr, mostly_copy_tree_r, NULL, NULL);
  return expr;
}

void
mark_not_simple (expr_p)
     tree *expr_p;
{
  TREE_NOT_GIMPLE (*expr_p) = 1;
}

/* A terser interface for building a representation of a exception
   specification.  */

tree
gimple_build_eh_filter (body, allowed, failure)
     tree body, allowed, failure;
{
  /* FIXME should the allowed types go in TREE_TYPE?  */
  tree filter = build (EH_FILTER_EXPR, void_type_node, allowed, failure);
  return build (TRY_CATCH_EXPR, void_type_node, body, filter);
}

/* Simplify a CLEANUP_POINT_EXPR.  Currently this works by adding
   WITH_CLEANUP_EXPRs to the prequeue as we encounter cleanups while
   simplifying the body, and converting them to TRY_FINALLY_EXPRs when we
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
simplify_cleanup_point_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  tree_stmt_iterator iter;
  tree body;

  tree temp = voidify_wrapper_expr (*expr_p);

  /* We only care about the number of conditions between the innermost
     CLEANUP_POINT_EXPR and the cleanup.  So save and reset the count.  */
  int old_conds = gimplify_ctxp->conditions;
  gimplify_ctxp->conditions = 0;

  body = TREE_OPERAND (*expr_p, 0);
  simplify_stmt (&body);

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
	    next = empty_stmt_node;

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

/* If the language requires some sort of exception protection of cleanup
   actions (i.e. calling terminate in C++), modify the cleanup to reflect
   that.  */

tree
maybe_protect_cleanup (cleanup)
     tree cleanup;
{
#if 0
  /* Ack!  We only want to protect the cleanup if it's run in the EH path.
     There's currently no reasonable way to express this in GENERIC.  FIXME
     FIXME FIXME!  Don't mess with this for now; it will still get handled
     properly in the expander.  */
  tree protect_cleanup_actions
    = (lang_protect_cleanup_actions
       ? (*lang_protect_cleanup_actions) ()
       : NULL_TREE);

  if (protect_cleanup_actions)
    /* Wrap cleanups in MUST_NOT_THROW equivalent if appropriate.  FIXME
       perhaps this should be done earlier in the frontend so we don't need
       a hook.  */
    cleanup = gimple_build_eh_filter (cleanup, NULL_TREE,
				      protect_cleanup_actions);
#endif
  return cleanup;
}

/* Insert a cleanup marker for simplify_cleanup_point_expr.  CLEANUP
   is the cleanup action required.  */

static void
gimple_push_cleanup (cleanup, pre_p)
     tree cleanup;
     tree *pre_p;
{
  tree wce;

  cleanup = maybe_protect_cleanup (cleanup);

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

      tree flag = create_tmp_var (integer_type_node, "cleanup");
      tree ffalse = build (MODIFY_EXPR, void_type_node, flag, integer_zero_node);
      tree ftrue = build (MODIFY_EXPR, void_type_node, flag, integer_one_node);
      cleanup = build (COND_EXPR, void_type_node, flag, cleanup, empty_stmt_node);
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

/* Simplify a TARGET_EXPR which doesn't appear on the rhs of an INIT_EXPR.  */

static void
simplify_target_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  tree targ = *expr_p;
  tree temp = TARGET_EXPR_SLOT (targ);
  tree init = TARGET_EXPR_INITIAL (targ);

  /* TARGET_EXPR temps aren't part of the enclosing block, so add it to the
     temps list.  */
  gimple_add_tmp_var (temp);

  /* Build up the initialization and add it to pre_p.  */
  init = build (MODIFY_EXPR, void_type_node, temp, init);
  simplify_expr (&init, pre_p, post_p, is_simple_stmt, fb_none);
  add_tree (init, pre_p);

  /* If needed, push the cleanup for the temp.  */
  if (TARGET_EXPR_CLEANUP (targ))
    gimple_push_cleanup (TARGET_EXPR_CLEANUP (targ), pre_p);

  *expr_p = temp;
}
