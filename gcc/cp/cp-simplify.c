/* C++-specific tree lowering bits; see also c-simplify.c and tree-simple.c.

   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Jason Merrill <jason@redhat.com>

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
#include "tree.h"
#include "cp-tree.h"
#include "c-common.h"
#include "toplev.h"
#include "tree-simple.h"

static void simplify_target_expr     PARAMS ((tree *, tree *));
static void maybe_fixup_loop_cond    PARAMS ((tree *, tree *, tree *));

/* Do C++-specific simplification.  Args are as for simplify_expr.  */

int
cp_simplify_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  switch (TREE_CODE (*expr_p))
    {
    case FOR_STMT:
      maybe_fixup_loop_cond (&FOR_COND (*expr_p), &FOR_BODY (*expr_p),
			     pre_p);
      break;
    case WHILE_STMT:
      maybe_fixup_loop_cond (&WHILE_COND (*expr_p), &WHILE_BODY (*expr_p),
			     pre_p);
      break;
      
    case INIT_EXPR:
      {
	/* If we are initializing something from a TARGET_EXPR, strip the
	   TARGET_EXPR and initialize it directly.  */
	tree *from = &TREE_OPERAND (*expr_p, 1);
	if (TREE_CODE (*from) == TARGET_EXPR)
	  *from = TARGET_EXPR_INITIAL (*from);
	/* And then fall through to the default code.  */
	break;
      }
    case TARGET_EXPR:
      simplify_target_expr (expr_p, pre_p);
      break;

    default:
      break;
    }

  return c_simplify_expr (expr_p, pre_p, post_p);
}

/* Simplify a TARGET_EXPR which doesn't appear on the rhs of an INIT_EXPR.  */

static void
simplify_target_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  tree temp = TARGET_EXPR_SLOT (*expr_p);
  tree decl = build_stmt (DECL_STMT, temp);
  tree init = build (INIT_EXPR, TREE_TYPE (temp), temp,
		     TARGET_EXPR_INITIAL (*expr_p));
  add_tree (decl, pre_p);
  *expr_p = init;
}

/* Given pointers to the condition and body of a for or while loop,
   reorganize things so that the condition is just an expression.

   COND_P and BODY_P are pointers to the condition and body of the loop.
   PRE_P is a list to which we can add effects that need to happen before
   the loop begins (i.e. as part of the for-init-stmt).  */

static void
maybe_fixup_loop_cond (cond_p, body_p, pre_p)
     tree *cond_p;
     tree *body_p;
     tree *pre_p;
{
  if (TREE_CODE (*cond_p) == TREE_LIST)
    {
      /* In C++ a condition can be a declaration; to express this,
	 the FOR_COND is a TREE_LIST.  The TREE_PURPOSE contains the
	 SCOPE_STMT and DECL_STMT, if any, and the TREE_VALUE contains
	 the actual value to test.  We want to transform

	   for (; T u = init; ) { ... }

	 into

	   for (tmp = 1; tmp; )
	     { T u = init; tmp = u; if (tmp) { ... } }

	 The wackiest part of this is handling the SCOPE_STMTs; at first,
	 one is in the FOR_COND and the other is after the COMPOUND_STMT in
	 the body.  We will use the same SCOPE_STMTs to wrap our new
	 block.  */

      tree scope = TREE_PURPOSE (*cond_p);
      tree val = TREE_VALUE (*cond_p);

      if (TREE_CODE (scope) != SCOPE_STMT)
	abort ();
      if (TREE_CHAIN (scope) == NULL_TREE)
	{
	  /* There isn't actually a decl.  Just move the SCOPE_STMT
	     down into the FOR_BODY.  */
	  *cond_p = val;
	  TREE_CHAIN (scope) = *body_p;
	}
      else
	{
	  /* There is a decl.  Do the transformation described
	     above.  */

	  tree if_s, mod, close_scope;
	  /* tmp = 1;  */
	  *cond_p = get_initialized_tmp_var (boolean_true_node,
					     pre_p);
	  /* tmp = u; */
	  mod = build_modify_expr (*cond_p, NOP_EXPR, val);
	  mod = build_stmt (EXPR_STMT, mod);

	  /* Separate the actual body block from the SCOPE_STMT.  */
	  close_scope = TREE_CHAIN (*body_p);
	  TREE_CHAIN (*body_p) = NULL_TREE;

	  /* if (tmp) { ... } */
	  if_s = build_stmt (IF_STMT, *cond_p, *body_p, NULL_TREE);

	  /* Finally, tack it all together.  */
	  chainon (scope, mod);
	  TREE_CHAIN (mod) = if_s;
	  TREE_CHAIN (if_s) = close_scope;
	}
      *body_p = build_stmt (COMPOUND_STMT, scope);
    }
}
