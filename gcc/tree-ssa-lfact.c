/* Local factoring (code hoisting/sinking) on SSA trees.
   Copyright (C) 2004 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "basic-block.h"
#include "resource.h"
#include "ggc.h"
#include "flags.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "langhooks.h"
#include "tree-pass.h"
#include "tree-ssa-operands.h"
#include "fact-common.h"

/* This file provides the statement hoisting on TREE SSA.
   The algorithm is:
  1) Collect basic blocks that have the same parents into sibling sets.
      e.g. block0
          if (...) { block1 } else { block2 }
      Block's 1 and 2 are in a sibling set.
  2) Try to hoist statements from a sibling set.
  3) Do steps 2 for each sibling set. 
  4) Repeat steps 2-3 until there is no more gain.

  TODO:
   * Extend the number of the hoistable statements.
   * Hoist similar statements (not exactly same).
*/

#define MAX_HTAB 1000

static int gain;
static tree global_tree;

/* Local factoring - hoisting  */
static tree tree_find_first_from_begin (bb_decorator, int);
static int tree_find_insn_from_begin (tree, bb_decorator, int);
static int stmt_hoisting (bb_decorator);
static int tree_moveable_from_begin (basic_block, tree);

/* Local factoring - sinking  */
static tree tree_find_first_from_end (bb_decorator, int);
static int tree_find_insn_from_end (tree, bb_decorator, int);
static int stmt_sinking (bb_decorator);
static int tree_moveable_from_end (basic_block, tree);

/* Common functions  */
static tree tree_find_first (bb_decorator decorator, int,
			     enum lfact_direction);
static int tree_find_insn (tree, bb_decorator, int, enum lfact_direction);
static int stmt_move (bb_decorator, enum lfact_direction);

static int tree_factoring (bb_decorator, enum lfact_direction);
static int simple_stmt (tree);
static int commutable_stmts (tree, tree);
static void get_stmts_of_bb (bb_decorator);
static int compare_vars (tree, tree, bool);
static int compare_stmts (tree, tree);

static void replace_immediate_uses (tree, def_optype, tree);
static int find_stmt_in_set (htab_t *, tree);
static void replace_def_vars (bb_decorator, tree);
static bool has_vuse (tree);
static bool has_v_may_def (tree);
static bool has_v_must_def (tree);

static bool tree_ssa_local_factoring_1 (bb_decorator, enum lfact_direction);


/* Global variables  */
static htab_t stmt_table1;
static htab_t *table1_p;
static htab_t stmt_table2;
static htab_t *table2_p;

/* Auxiliary functions for hashing statements  */
static hashval_t
stmt_hash (const void *item)
{
  tree stmt = (tree) item;
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return (iterative_hash_expr (stmt, 0));
  return (iterative_hash_expr (TREE_OPERAND (stmt, 2), 0));
}

/* Return 1 if P1 and P2 is equal.  */
static int
stmt_equal (const void *p1, const void *p2)
{
  tree t1 = (tree) p1;
  tree t2 = (tree) p2;

  if ((TREE_CODE (t2) != TREE_CODE (t1))
      || (TREE_TYPE (t1) != TREE_TYPE (t2)))
    return 0;

  if (simple_cst_equal (t1, t2) != 1)
    return 0;
  if (stmt_hash (p1) != stmt_hash (p2))
    abort ();

  return 1;
}

/* Looking for STMT node in the TABLE.  */
static int
find_stmt_in_set (htab_t * table, tree stmt)
{
  if (TREE_CODE (stmt) == MODIFY_EXPR
      && htab_find (*table, TREE_OPERAND (stmt, 1)))
    return 1;
  return 0;
}

/* Collection of the statments, which are included by all basic blocks
   chained from decorator.  The DECORATOR is the first element of the
   sibling set, and we can travel the whole set by the help of the
   'next_sibling' field of bb_decorator.  In the result *table1_p
   contains the common statements of the investigated sibling set.  */
static void
get_stmts_of_bb (bb_decorator decorator)
{
  basic_block bb = decorator->curr;
  bb_decorator di;
  block_stmt_iterator si;
  void **slot;

  htab_empty (stmt_table1);
  htab_empty (stmt_table2);

  /* Collection of the statements of the first block.  */
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      tree stmt = bsi_stmt (si);
      if (simple_stmt (stmt))
	{
	  if (!htab_find (stmt_table1, TREE_OPERAND (stmt, 1)))
	    {
	      slot =
		htab_find_slot (stmt_table1, TREE_OPERAND (stmt, 1), INSERT);
	      *slot = TREE_OPERAND (stmt, 1);
	    }
	}
    }
  table1_p = &stmt_table1;
  table2_p = &stmt_table2;

  /* Collection of the common statements of the next block and till now
     investigated blocks.  */
  for (di = decorator->next_sibling; di; di = di->next_sibling)
    {
      htab_t *temp_table;
      for (si = bsi_start (di->curr); !bsi_end_p (si); bsi_next (&si))
	{
	  tree stmt = bsi_stmt (si);
	  if (TREE_CODE (stmt) == MODIFY_EXPR)
	    {
	      /* if we find a stmt, which is included by the table pointed by
	         table1_p, we insert it into the other table.  */
	      if (htab_find (*table1_p, TREE_OPERAND (stmt, 1)))
		{
		  slot =
		    htab_find_slot (*table2_p, TREE_OPERAND (stmt, 1),
				    INSERT);
		  *slot = TREE_OPERAND (stmt, 1);
		}
	    }
	}

      htab_empty (*table1_p);
      temp_table = table1_p;
      table1_p = table2_p;
      table2_p = temp_table;
    }
}

/* It returns with true, if stmt has any virtual use operand.  */
static bool
has_vuse (tree stmt)
{
  vuse_optype vuses;
  stmt_ann_t ann;
  ann = get_stmt_ann (stmt);
  vuses = VUSE_OPS (ann);
  return ((int) NUM_VUSES (vuses) != 0);
}

/* It returns with true, if stmt has any v_must_def operand.  */
static bool
has_v_must_def (tree stmt)
{
  v_must_def_optype v_must_defs;
  stmt_ann_t ann;
  ann = get_stmt_ann (stmt);
  v_must_defs = V_MUST_DEF_OPS (ann);
  return ((int) NUM_V_MUST_DEFS (v_must_defs) != 0);
}

/* It returns with true, if stmt has any v_may_def operand.  */
static bool
has_v_may_def (tree stmt)
{
  v_may_def_optype v_may_defs;
  stmt_ann_t ann;
  ann = get_stmt_ann (stmt);
  v_may_defs = V_MAY_DEF_OPS (ann);
  return ((int) NUM_V_MAY_DEFS (v_may_defs) != 0);
}

/* If the STMT1 and STMT2 nodes are commutable, this function returns
   with 1.  */
static int
commutable_stmts (tree stmt1, tree stmt2)
{
  int i;
  dataflow_t df = get_immediate_uses (stmt1);

  int num_imm_uses = num_immediate_uses (df);

  /* Now we suppose that a CALL_EXPR is not moveable.  */
  if (TREE_CODE (stmt1) == CALL_EXPR)
    return 0;

  for (i = 0; i < num_imm_uses; i++)
    if (simple_cst_equal (immediate_use (df, i), stmt2) == 1)
      return 0;
  return 1;
}

/* Auxiliary function for 'tree_find_first_from_begin' and
   'tree_find_insn_from_begin'.  */
static int
tree_moveable_from_begin (basic_block bb, tree stmt)
{
  block_stmt_iterator si;
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      tree stmt2 = bsi_stmt (si);
      if (simple_cst_equal (stmt2, stmt) == 1)
	return 1;
      if (!commutable_stmts (stmt2, stmt))
	return 0;
    }
  return 0;
}

/* Auxiliary function for 'tree_find_first_from_end' and
   'tree_find_insn_from_end'.  */
static int
tree_moveable_from_end (basic_block bb, tree stmt)
{
  block_stmt_iterator si;
  for (si = bsi_last (bb); !bsi_end_p (si); bsi_prev (&si))
    {
      tree stmt2 = bsi_stmt (si);
      if (simple_cst_equal (stmt2, stmt) == 1)
	return 1;
      if (!commutable_stmts (stmt, stmt2))
	return 0;
    }
  return 0;
}

/* Common function for calling tree_find_first_from_begin and 
   tree_find_first_from_end.  */
static tree
tree_find_first (bb_decorator decorator, int max_depth,
		 enum lfact_direction alg)
{
  return (alg == LFD_HOISTING ?
	  tree_find_first_from_begin (decorator, max_depth) :
	  tree_find_first_from_end (decorator, max_depth));
}

/* Common function for calling tree_find_insn_from_begin and 
   tree_find_insn_from_end.  */
static int
tree_find_insn (tree insn,
		bb_decorator decorator,
		int max_depth, enum lfact_direction alg)
{
  return (alg == LFD_HOISTING ?
	  tree_find_insn_from_begin (insn, decorator, max_depth) :
	  tree_find_insn_from_end (insn, decorator, max_depth));
}

/* Common function for calling stmt_hoisting and stmt_sinking.  */
static int
stmt_move (bb_decorator decorator, enum lfact_direction alg)
{
  return (alg == LFD_HOISTING ?
	  stmt_hoisting (decorator) : stmt_sinking (decorator));
}

/* Finding the first hoistable stmt of the basic block of DECORATOR. If this
   stmt is existing, the moveable_node field of decorator will be set. With
   parameter MAX_DEPTH we can modulate the depth of the search.  If MAX_DEPTH
   is 0, there is no restriction.  */
static tree
tree_find_first_from_begin (bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  block_stmt_iterator bsi = bsi_start (bb);
  tree stmt = NULL;
  int depth = 0;

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      stmt = bsi_stmt (bsi);

      if (depth < decorator->last_depth)
	{
	  depth++;
	  continue;
	}

      if ((TREE_CODE (stmt) == RETURN_EXPR)
	  /* || (TREE_CODE (stmt) == CALL_EXPR) */  || is_ctrl_stmt (stmt))
	{
	  /* stmt is not moveable.  */
	  depth++;
	  decorator->last_depth = depth;
	  return (NULL_TREE);
	}

      if (!find_stmt_in_set (table1_p, stmt))
	{
	  /* stmt is not a potantial hoistable stmt.  */
	  depth++;
	  decorator->last_depth = depth;
	  continue;
	}

      if (tree_moveable_from_begin (bb, stmt)
	  && commutable_stmts (stmt,
			       bsi_stmt (bsi_last (EDGE_PRED (bb, 0)->src))))
	{
	  /* stmt is moveable.  */
	  decorator->moveable_node = bsi;
	  return stmt;
	}

      depth++;
      decorator->last_depth = depth;

      if (depth == max_depth - 1)
	return (NULL_TREE);
    }
  if (bsi_end_p (bsi))
    return (NULL_TREE);
  return stmt;
}

/* Finding the first hoistable stmt of the basic block of DECORATOR. If this
   stmt is existing, the moveable_node field of decorator will be set. With
   parameter MAX_DEPTH we can modulate the depth of the search.  If MAX_DEPTH
   is 0, there is no restriction.  */
static tree
tree_find_first_from_end (bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  block_stmt_iterator bsi = bsi_last (bb);
  tree stmt = NULL;
  int depth = 0;

  for (bsi = bsi_last (bb); !bsi_end_p (bsi); bsi_prev (&bsi))
    {
      stmt = bsi_stmt (bsi);

      if (depth < decorator->last_depth)
	{
	  depth++;
	  continue;
	}

      if ((TREE_CODE (stmt) == RETURN_EXPR)
	  || (!find_stmt_in_set (table1_p, stmt)) || is_ctrl_stmt (stmt))
	{
	  depth++;
	  decorator->last_depth = depth;
	  continue;
	}

      if (tree_moveable_from_end (bb, stmt))
	{
	  decorator->moveable_node = bsi;
	  return stmt;
	}

      depth++;
      decorator->last_depth = depth;

      if (depth == max_depth - 1)
	return (NULL_TREE);
    }
  if (bsi_end_p (bsi))
    return (NULL_TREE);
  return stmt;
}

/* Auxiliary function for 'compare_stmts'.  */
static int
compare_vars (tree var1, tree var2, bool flag)
{
  int same = 1;

  if (TREE_CODE (var1) != TREE_CODE (var2))
    return 0;
  if (TREE_TYPE (var1) != TREE_TYPE (var2))
    return 0;

  /* if (INDIRECT_REF == TREE_CODE (var1)) same = operand_equal_p
     (SSA_NAME_VAR (TREE_OPERAND (var1, 0)), SSA_NAME_VAR (TREE_OPERAND
     (var2, 0)), 0); */
  if (TREE_CODE (var1) != SSA_NAME)
    return 0;

  if (flag)
    same = (SSA_NAME_VAR (var1) == SSA_NAME_VAR (var2));

  return same;
}

/* The equality between assignment statements is the following: if the rhs of
   the statements are equal, and the lhs of the stamtements are derived from
   the same variable, the statements are equal. In other cases the equality
   relation is the familiar equality. It returns with 1, if STMT1 and STMT2
   are equal. */
static int
compare_stmts (tree stmt1, tree stmt2)
{
  if (TREE_CODE (stmt1) != TREE_CODE (stmt2))
    return 0;
  if (TREE_CODE (stmt1) == MODIFY_EXPR)
    {
      bool flag = has_vuse (stmt1);
      return (compare_vars (TREE_OPERAND (stmt1, 0),
			    TREE_OPERAND (stmt2, 0), flag) +
	      simple_cst_equal (TREE_OPERAND (stmt1, 1),
				TREE_OPERAND (stmt2, 1)) == 2 ? 1 : 0);
      return 0;
    }
  else
    return (simple_cst_equal (stmt1, stmt2));
}

/* Finding INSN in the basic block pointed by decorator->curr. If we are find
   such insn in the basic block, the decorator->moveable_node will be set, and
   if it hoistable, too, the function returns with 1.  */
static int
tree_find_insn_from_begin (tree insn, bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  block_stmt_iterator bsi;
  int depth = 0;

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      tree stmt = bsi_stmt (bsi);

      if (compare_stmts (insn, stmt))
	{
	  decorator->moveable_node = bsi;
	  if (!tree_moveable_from_begin (bb, stmt))
	    return 0;
	  return 1;
	}
      depth++;
      if (depth == max_depth)
	return 0;
    }
  return 0;
}

/* Finding INSN in the basic block pointed by decorator->curr. If we are find
   such insn in the basic block, the decorator->moveable_node will be set, and
   if it sinkable, too, the function returns with 1.  */
static int
tree_find_insn_from_end (tree insn, bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  block_stmt_iterator bsi;
  int depth = 0;

  for (bsi = bsi_last (bb); !bsi_end_p (bsi); bsi_prev (&bsi))
    {
      tree stmt = bsi_stmt (bsi);
      if (compare_stmts (insn, stmt))
	{
	  decorator->moveable_node = bsi;
	  if (!tree_moveable_from_end (bb, stmt))
	    return 0;
	  return 1;
	}
      depth++;
      if (depth == max_depth)
	return 0;
    }
  return 0;
}

/* Try to hoist only the simple statements.  */
static int
simple_stmt (tree stmt)
{
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return 0;
  get_stmt_operands (stmt);

  if (((TREE_CODE (TREE_OPERAND (stmt, 0)) == SSA_NAME))
      && (!has_v_may_def (stmt)) && (!has_v_must_def (stmt))
      && (TREE_CODE (TREE_OPERAND (stmt, 1)) != COMPONENT_REF))
    return 1;

  return 0;
}

/* We investigate STMT, and if it uses the DEF operand, we replace it to the
   global_tree operand, and we modify the definition statement of this
   operand.  */
static void
replace_immediate_uses (tree stmt, def_optype def, tree new_def)
{
  use_optype uses;
  vuse_optype vuses;
  v_may_def_optype v_may_defs;
  stmt_ann_t ann;
  size_t j;

  if (TREE_CODE (stmt) != PHI_NODE)
    {
      /* STMT is a real statement.  */
      ann = stmt_ann (stmt);
      uses = USE_OPS (ann);
      vuses = VUSE_OPS (ann);
      v_may_defs = V_MAY_DEF_OPS (ann);

      for (j = 0; j < NUM_USES (uses); j++)
	{
	  if (simple_cst_equal (DEF_OP (def, 0), USE_OP (uses, j)) == 1)
	    {
	      tree use_var = USE_OP (uses, j);
	      use_operand_p use_p = USE_OP_PTR (uses, j);
	      replace_exp (use_p, global_tree);
	      SSA_NAME_DEF_STMT (use_var) = new_def;

	    }
	}

      for (j = 0; j < NUM_VUSES (vuses); j++)
	{
	  if (simple_cst_equal (DEF_OP (def, 0), VUSE_OP (vuses, j)) == 1)
	    {
	      tree vuse_var = VUSE_OP (vuses, j);
	      use_operand_p vuse_p = VUSE_OP_PTR (vuses, j);
	      replace_exp (vuse_p, global_tree);
	      SSA_NAME_DEF_STMT (vuse_var) = new_def;
	    }
	}
      for (j = 0; j < NUM_V_MAY_DEFS (v_may_defs); j++)
	{
	  if (simple_cst_equal (DEF_OP (def, 0), V_MAY_DEF_OP (v_may_defs, j))
	      == 1)
	    {
	      tree v_may_def_var = V_MAY_DEF_OP (v_may_defs, j);
	      use_operand_p v_may_def_p = V_MAY_DEF_OP_PTR (v_may_defs, j);
	      replace_exp (v_may_def_p, global_tree);
	      SSA_NAME_DEF_STMT (v_may_def_var) = new_def;
	    }
	}
      modify_stmt (stmt);
    }
  else
    {
      /* STMT is a phi node.  */
      size_t j;
      for (j = 0; j < (unsigned) PHI_NUM_ARGS (stmt); j++)
	{
	  if ((PHI_ARG_DEF (stmt, j) == DEF_OP (def, 0)))
	    {
	      SET_PHI_ARG_DEF (stmt, j, global_tree);
	      if (TREE_CODE (global_tree) == SSA_NAME
		  && PHI_ARG_EDGE (stmt, j)->flags & EDGE_ABNORMAL)
		SSA_NAME_OCCURS_IN_ABNORMAL_PHI (global_tree) = 1;
	      SSA_NAME_DEF_STMT (PHI_ARG_DEF (stmt, j)) = new_def;
	    }
	}
    }
}

/* Auxiliary function for 'compute_immediate_uses' function */
static bool
stmt_selector (tree node)
{
  return (simple_cst_equal (node, global_tree) == 1 ? true : false);
}

/* We must rename the decent use operand of the immediate uses of the hoisted
   assignment statement. Starting from FIRST, we can travel the whole
   sibling set. NEW_NODE is the newly created node.  */
static void
replace_def_vars (bb_decorator first, tree new_node)
{
  bb_decorator di;
  def_optype new_def;
  stmt_ann_t new_ann;
  new_ann = stmt_ann (new_node);
  new_def = DEF_OPS (new_ann);
  global_tree = TREE_OPERAND (new_node, 0);

  for (di = first; di; di = di->next_sibling)
    {
      tree stmt = bsi_stmt (di->moveable_node);
      dataflow_t df = get_immediate_uses (stmt);
      int num_imm_uses = num_immediate_uses (df);
      def_optype def;
      stmt_ann_t ann_stmt;

      ann_stmt = stmt_ann (stmt);
      def = DEF_OPS (ann_stmt);

      if (num_imm_uses > 0)
	{
	  int i = 0;
	  for (i = 0; i < num_imm_uses; i++)
	    replace_immediate_uses (immediate_use (df, i), def, new_node);
	}
    }
}

/* If there is a hoistable stmt, then all moveable_node fields of bb_decorator
   of the sibling set members are set. We must create new node(s), and modify
   the immediate uses, and remove this stmts.  */
static int
stmt_hoisting (bb_decorator first)
{
  edge e = EDGE_PRED (first->curr, 0);
  if (first->num_pred_bb == 1)
    {
      bb_decorator di;
      basic_block bb = e->src;
      block_stmt_iterator last = bsi_last (bb);
      tree new_node;
      tree stmt = bsi_stmt (first->moveable_node);
      if (TREE_CODE (stmt) != MODIFY_EXPR)
	{
	  new_node = copy_node (stmt);
	  gain--;
	}
      else
	{
	  /* Creation of a new temporaly variable.  */
	  tree temp_var =
	    create_tmp_var (TREE_TYPE (TREE_OPERAND (stmt, 0)), "T");
	  add_referenced_tmp_var (temp_var);

	  create_var_ann (temp_var);

	  /* Creation of a new node.  */
	  new_node = build (TREE_CODE (stmt),
			    TREE_TYPE (TREE_OPERAND (stmt, 0)),
			    temp_var, TREE_OPERAND (stmt, 1));

	  temp_var = make_ssa_name (temp_var, new_node);
	  SSA_NAME_VAR (temp_var) = SSA_NAME_VAR (TREE_OPERAND (stmt, 0));
	  modify_stmt (new_node);
	  TREE_OPERAND (new_node, 0) = temp_var;
	  copy_virtual_operands (new_node, stmt);
	  get_stmt_operands (new_node);
	  gain--;

	}
      bsi_insert_before (&last, new_node, BSI_NEW_STMT);
      replace_def_vars (first, new_node);

      for (di = first; di; di = di->next_sibling)
	{
	  gain++;
	  redirect_immediate_uses (bsi_stmt (di->moveable_node), new_node);
	  bsi_remove (&di->moveable_node);
	}
      compute_immediate_uses (TDFA_USE_OPS | TDFA_USE_VOPS, stmt_selector);
      return 1;
    }

  /* We increment 'first->last_depth', that we can avoid an infinite
     loop. So we can investigate further the basic block after a
     unsuccsesful hoist. This branch is a feasible improvment for
     hoisting.  */
  first->last_depth++;
  return 0;

}

static int
stmt_sinking (bb_decorator first)
{
  if (first->num_succ_bb == 1)
    {
      /* Match.  */
      first->last_depth++;
    }
  else
    first->last_depth++;

  return 0;
}

/* Main function of local factoring  */
static int
tree_factoring (bb_decorator decorator, enum lfact_direction alg)
{
  bb_decorator di = decorator;
  int change = 0;
  int not_hoistable = 1;

  while (di)
    {
      di->last_depth = 0;
      di = di->next_decorator;
    }

  di = decorator;

  while (di)
    {
      if ((!di->prev_sibling) && (di->next_sibling))
	{
	  /* DI is the first member of an real sibling set. While we can
	     hoist any statement from this sibling set, we try to hoist
	     further statements from this set, and DI is fixed.  */
	  bb_decorator di2;
	  tree insn = NULL;

	  if (not_hoistable)
	    {
	      get_stmts_of_bb (di);
	      not_hoistable = 0;
	    }

	  insn = tree_find_first (di, 0, alg);

	  if (!insn)
	    {
	      not_hoistable = 1;
	      di = di->next_decorator;
	      continue;
	    }

	  for (di2 = di->next_sibling; di2; di2 = di2->next_sibling)
	    {
	      if (!tree_find_insn (insn, di2, 0, alg))
		{
		  not_hoistable = 1;
		  di->last_depth++;
		  break;
		}
	    }

	  if (!not_hoistable)
	    {
	      if (stmt_move (di, alg))
		change = 1;
	    }
	}
      else
	di = di->next_decorator;
    }
  return change;
}

/* Call the local factoring algorithms in a parameter direction.  */
static bool
tree_ssa_local_factoring_1 (bb_decorator decorator, enum lfact_direction alg)
{
  gain = 0;
  collect_full_sibling (decorator, alg);

  cost_analyzer (decorator, alg);
  if (dump_file)
    dump_siblings (dump_file, decorator);

  compute_immediate_uses (TDFA_USE_OPS | TDFA_USE_VOPS, NULL);
  while (tree_factoring (decorator, alg));

  free_df ();
  if (dump_file && gain)
    fprintf (dump_file, "After %s the gain is %d instruction\n",
	     alg == LFD_HOISTING ? "hoisting" : "sinking", gain);
  return (gain > 0);
}

/* Main function of local factoring.  */
void
tree_ssa_local_factoring (void)
{
  bool has_gain = true;
  bb_decorator decorator = NULL;
  stmt_table1 = htab_create (MAX_HTAB, stmt_hash, stmt_equal, NULL);
  stmt_table2 = htab_create (MAX_HTAB, stmt_hash, stmt_equal, NULL);
  decorator = init_factoring (decorator);

  while (has_gain)
    {
      has_gain = tree_ssa_local_factoring_1 (decorator, LFD_SINKING);
      has_gain = tree_ssa_local_factoring_1 (decorator, LFD_HOISTING)
	|| has_gain;
    }

  free_bb_decorator_list (decorator);
  htab_delete (stmt_table1);
  htab_delete (stmt_table2);
}

/* The gate of local factoring.  */
bool
is_factoring (void)
{
  return ((flag_tree_lfact == 1) ? 1 : 0);
}

struct tree_opt_pass pass_tree_lfact = {
  "factoring",			/* name */
  is_factoring,			/* gate */
  tree_ssa_local_factoring,	/* execute */
  NULL,				/* sub */
  NULL,				/* next */
  0,				/* static_pass_number */
  0,				/* tv_id */
  0,				/* properties_required */
  0,				/* properties_provided */
  0,				/* properties_destroyed */
  0,				/* todo_flags_start */
  TODO_dump_func,		/* todo_flags_finish */ 
  0				/* letter */
};
