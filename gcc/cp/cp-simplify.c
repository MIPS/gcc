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
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cp-tree.h"
#include "c-common.h"
#include "toplev.h"
#include "tree-simple.h"

static void genericize_try_block	PARAMS ((tree *));

/* Genericize a C++ _STMT.  Called from c_simplify_stmt.  */

int
cp_simplify_stmt (stmt_p, next_p)
     tree *stmt_p;
     tree *next_p ATTRIBUTE_UNUSED;
{
  tree stmt = *stmt_p;
  switch (TREE_CODE (stmt))
    {
    case TRY_BLOCK:
      genericize_try_block (stmt_p);
      return 1;

    default:
      break;
    }
  return 0;
}

/* Genericize a TRY_BLOCK.  */

static void
genericize_try_block (stmt_p)
     tree *stmt_p;
{
  tree stmt = *stmt_p;

  if (CLEANUP_P (stmt))
    {
      /* Just convert to a TRY_CATCH_EXPR.  */
      tree body = TRY_STMTS (stmt);
      tree cleanup = TRY_HANDLERS (stmt);

      c_simplify_stmt (&body);

      *stmt_p = build (TRY_CATCH_EXPR, void_type_node, body, cleanup);
    }
  else
    /* FIXME a real try block.  */
    abort ();
}

/* Do C++-specific simplification.  Args are as for simplify_expr.  */

int
cp_simplify_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  switch (TREE_CODE (*expr_p))
    {
    case AGGR_INIT_EXPR:
      simplify_aggr_init_expr (expr_p);
      break;

    default:
      break;
    }

  return c_simplify_expr (expr_p, pre_p, post_p);
}

/* Replace the AGGR_INIT_EXPR at *TP with an equivalent CALL_EXPR.  */

void
simplify_aggr_init_expr (tp)
     tree *tp;
{
  tree aggr_init_expr = *tp;

  /* Form an appropriate CALL_EXPR.  */
  tree fn = TREE_OPERAND (aggr_init_expr, 0);
  tree args = TREE_OPERAND (aggr_init_expr, 1);
  tree slot = TREE_OPERAND (aggr_init_expr, 2);
  tree type = TREE_TYPE (aggr_init_expr);

  tree call_expr;
  enum style_t { ctor, arg, pcc } style;

  if (AGGR_INIT_VIA_CTOR_P (aggr_init_expr))
    style = ctor;
#ifdef PCC_STATIC_STRUCT_RETURN
  else if (1)
    style = pcc;
#endif
  else if (TREE_ADDRESSABLE (type))
    style = arg;
  else
    /* We shouldn't build an AGGR_INIT_EXPR if we don't need any special
       handling.  See build_cplus_new.  */
    abort ();

  if (style == ctor || style == arg)
    {
      /* Pass the address of the slot.  If this is a constructor, we
	 replace the first argument; otherwise, we tack on a new one.  */
      if (style == ctor)
	args = TREE_CHAIN (args);

      cxx_mark_addressable (slot);
      args = tree_cons (NULL_TREE, 
			build1 (ADDR_EXPR, 
				build_pointer_type (TREE_TYPE (slot)),
				slot),
			args);
    }

  call_expr = build (CALL_EXPR, 
		     TREE_TYPE (TREE_TYPE (TREE_TYPE (fn))),
		     fn, args, NULL_TREE);
  TREE_SIDE_EFFECTS (call_expr) = 1;

  if (style == arg)
    {
      /* Tell the backend that we've added our return slot to the argument
	 list.  */
      CALL_EXPR_HAS_RETURN_SLOT_ADDR (call_expr) = 1;
      /* And don't let anyone use the value of the call directly in a
	 larger expression.  */
      TREE_TYPE (call_expr) = void_type_node;
    }
  else if (style == pcc)
    {
      /* If we're using the non-reentrant PCC calling convention, then we
	 need to copy the returned value out of the static buffer into the
	 SLOT.  */
      int old_ac = flag_access_control;

      flag_access_control = 0;
      call_expr = build_aggr_init (slot, call_expr,
				   DIRECT_BIND | LOOKUP_ONLYCONVERTING);
      flag_access_control = old_ac;
    }

  /* We want to use the value of the initialized location as the
     result.  */
  call_expr = build (COMPOUND_EXPR, type,
		     call_expr, slot);

  /* Replace the AGGR_INIT_EXPR with the CALL_EXPR.  */
  TREE_CHAIN (call_expr) = TREE_CHAIN (aggr_init_expr);
  *tp = call_expr;
}
