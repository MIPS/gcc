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

static void simplify_target_expr     PARAMS ((tree *, tree *, tree *));

/* Do C++-specific simplification.  Args are as for simplify_expr.  */

int
cp_simplify_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  switch (TREE_CODE (*expr_p))
    {
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
      simplify_target_expr (expr_p, pre_p, post_p);
      break;
    }

  return c_simplify_expr (expr_p, pre_p, post_p);
}

/* Simplify a TARGET_EXPR which doesn't appear on the rhs of an INIT_EXPR.  */

static void
simplify_target_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  tree temp = TARGET_EXPR_SLOT (*expr_p);
  tree decl = build_stmt (DECL_STMT, temp);
  tree init = build (INIT_EXPR, TREE_TYPE (temp), temp,
		     TARGET_EXPR_INITIAL (*expr_p));
  add_tree (decl, pre_p);
  *expr_p = init;
}
