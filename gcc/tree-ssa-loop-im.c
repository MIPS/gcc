/* Loop invariant motion.
   Copyright (C) 2003 Free Software Foundation, Inc.
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
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
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "timevar.h"
#include "cfgloop.h"
#include "domwalk.h"
#include "params.h"
#include "tree-pass.h"

/* A list of dependencies.  */

struct depend
{
  tree stmt;
  struct depend *next;
};

/* The auxiliary data kept for each statement.  */
struct lim_aux_data
{
  struct loop *max_loop;	/* The outermost loop in that the statement
				   is invariant.  */

  struct loop *tgt_loop;	/* The loop out of that we want to move the
				   invariant.  */

  unsigned cost;		/* Cost of the computation of the value.  */

  struct depend *depends;	/* List of statements that must be moved as
				   well.  */
};

#define LIM_DATA(STMT) ((struct lim_aux_data *) (stmt_ann (STMT)->aux))

/* Minimum cost of an expensive expression.  */
#define LIM_EXPENSIVE ((unsigned) PARAM_VALUE (PARAM_LIM_EXPENSIVE))

/* The outermost loop for that execution of the header guarantees that the
   block will be executed.  */
#define ALWAYS_EXECUTED_IN(BB) ((struct loop *) (BB)->aux)

/* The possibilities of statement movement.  */
enum move_pos
{
  MOVE_IMPOSSIBLE,		/* No movement -- side effect expression.  */
  MOVE_PRESERVE_EXECUTION,	/* Must not cause the non-executed statement
				   become executed -- memory accesses, ... */
  MOVE_POSSIBLE			/* Unlimited movement.  */
};

/* Checks whether MEM is a memory access that might fail.  */

static bool
unsafe_memory_access_p (tree mem)
{
  tree base, idx;

  switch (TREE_CODE (mem))
    {
    case ADDR_EXPR:
      return false;

    case COMPONENT_REF:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
      return unsafe_memory_access_p (TREE_OPERAND (mem, 0));

    case ARRAY_REF:
      base = TREE_OPERAND (mem, 0);
      idx = TREE_OPERAND (mem, 1);
      if (unsafe_memory_access_p (base))
	return true;

      if (TREE_CODE_CLASS (TREE_CODE (idx)) != 'c')
	return true;

      return !in_array_bounds_p (base, idx);

    case INDIRECT_REF:
      return true;

    default:
      return false;
    }
}

/* Determines whether it is possible to move the statement STMT.  */

static enum move_pos
movement_possibility (tree stmt)
{
  tree lhs, rhs;

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return MOVE_IMPOSSIBLE;

  if (stmt_ends_bb_p (stmt))
    return MOVE_IMPOSSIBLE;

  get_stmt_operands (stmt);

  if (stmt_ann (stmt)->has_volatile_ops)
    return MOVE_IMPOSSIBLE;

  lhs = TREE_OPERAND (stmt, 0);
  rhs = TREE_OPERAND (stmt, 1);

  if (TREE_SIDE_EFFECTS (rhs))
    return MOVE_IMPOSSIBLE;

  if (TREE_CODE (lhs) != SSA_NAME
      || tree_could_trap_p (rhs)
      || unsafe_memory_access_p (rhs))
    return MOVE_PRESERVE_EXECUTION;

  return MOVE_POSSIBLE;
}

/* Adds a dependency on DEF to DATA on statement inside LOOP.  If ADD_COST is
   true, add the cost of the computation to the cost in DATA.  */

static bool
add_dependency (tree def, struct lim_aux_data *data, struct loop *loop,
		bool add_cost)
{
  tree def_stmt = SSA_NAME_DEF_STMT (def);
  basic_block def_bb = bb_for_stmt (def_stmt);
  struct loop *max_loop;
  struct depend *dep;

  if (!def_bb)
    return true;

  max_loop = find_common_loop (loop, def_bb->loop_father);

  if (LIM_DATA (def_stmt))
    max_loop = find_common_loop (max_loop,
				 LIM_DATA (def_stmt)->max_loop->outer);
  if (max_loop == loop)
    return false;
  max_loop = superloop_at_depth (loop, max_loop->depth + 1);

  if (flow_loop_nested_p (data->max_loop, max_loop))
    data->max_loop = max_loop;
 
  if (add_cost
      && def_bb->loop_father == loop)
    data->cost += LIM_DATA (def_stmt)->cost;

  dep = xmalloc (sizeof (struct depend));
  dep->stmt = def_stmt;
  dep->next = data->depends;
  data->depends = dep;

  return true;
}

/* Estimates a cost of statement STMT.  TODO -- the values here are just ad-hoc
   constants.  The estimates should be based on target-specific values.  */

static unsigned
stmt_cost (tree stmt)
{
  tree lhs, rhs;
  unsigned cost = 1;

  lhs = TREE_OPERAND (stmt, 0);
  rhs = TREE_OPERAND (stmt, 1);

  /* Hoisting memory references out should almost surely be a win.  */
  if (!is_gimple_variable (lhs))
    cost += 20;
  if (is_gimple_addr_expr_arg (rhs) && !is_gimple_variable (rhs))
    cost += 20;

  switch (TREE_CODE (rhs))
    {
    case CALL_EXPR:
      /* So should be hoisting calls.  */

      /* Unless the call is a builtin_constant_p; this always folds to a
	 constant, so moving it is useless.  */
      rhs = get_callee_fndecl (rhs);
      if (DECL_BUILT_IN (rhs)
	  && DECL_FUNCTION_CODE (rhs) == BUILT_IN_CONSTANT_P)
	return 0;

      cost += 20;
      break;

    case MULT_EXPR:
    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case EXACT_DIV_EXPR:
    case CEIL_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case ROUND_MOD_EXPR:
    case TRUNC_MOD_EXPR:
      /* Division and multiplication are usually expensive.  */
      cost += 20;
      break;

    default:
      break;
    }

  return cost;
}

/* Determine maximal level to that it is possible to move a statement STMT.
   If MUST_PRESERVE_EXEC is true, we must preserve the fact whether the
   statement is executed.  */

static bool
determine_max_movement (tree stmt, bool must_preserve_exec)
{
  basic_block bb = bb_for_stmt (stmt);
  struct loop *loop = bb->loop_father;
  struct loop *level;
  struct lim_aux_data *lim_data = LIM_DATA (stmt);
  use_optype uses;
  vuse_optype vuses;
  vdef_optype vdefs;
  stmt_ann_t ann = stmt_ann (stmt);
  unsigned i;
  
  if (must_preserve_exec)
    level = ALWAYS_EXECUTED_IN (bb);
  else
    level = superloop_at_depth (loop, 1);
  lim_data->max_loop = level;

  uses = USE_OPS (ann);
  for (i = 0; i < NUM_USES (uses); i++)
    if (!add_dependency (USE_OP (uses, i), lim_data, loop, true))
      return false;

  vuses = VUSE_OPS (ann);
  for (i = 0; i < NUM_VUSES (vuses); i++)
    if (!add_dependency (VUSE_OP (vuses, i), lim_data, loop, false))
      return false;

  vdefs = VDEF_OPS (ann);
  for (i = 0; i < NUM_VDEFS (vdefs); i++)
    if (!add_dependency (VDEF_OP (vdefs, i), lim_data, loop, false))
      return false;

  lim_data->cost += stmt_cost (stmt);

  return true;
}

/* Sets a level to that the statement STMT is moved to LEVEL due to moving of
   statement from ORIG_LOOP and update levels of all dependencies.  */

static void
set_level (tree stmt, struct loop *orig_loop, struct loop *level)
{
  struct loop *stmt_loop = bb_for_stmt (stmt)->loop_father;
  struct depend *dep;

  stmt_loop = find_common_loop (orig_loop, stmt_loop);
  if (LIM_DATA (stmt) && LIM_DATA (stmt)->tgt_loop)
    stmt_loop = find_common_loop (stmt_loop,
				  LIM_DATA (stmt)->tgt_loop->outer);
  if (flow_loop_nested_p (stmt_loop, level))
    return;

  LIM_DATA (stmt)->tgt_loop = level;
  for (dep = LIM_DATA (stmt)->depends; dep; dep = dep->next)
    set_level (dep->stmt, orig_loop, level);
}

/* Determines a level to that really hoist the statement STMT.  TODO -- use
   profiling information to set it more sanely.  */

static void
set_profitable_level (tree stmt)
{
  set_level (stmt, bb_for_stmt (stmt)->loop_father, LIM_DATA (stmt)->max_loop);
}

/* Checks whether STMT is a nonpure call.  */

static bool
nonpure_call_p (tree stmt)
{
  if (TREE_CODE (stmt) == MODIFY_EXPR)
    stmt = TREE_OPERAND (stmt, 1);

  return (TREE_CODE (stmt) == CALL_EXPR
	  && TREE_SIDE_EFFECTS (stmt));
}

/* Releases the memory occupied by DATA.  */

static void
free_lim_aux_data (struct lim_aux_data *data)
{
  struct depend *dep, *next;

  for (dep = data->depends; dep; dep = next)
    {
      next = dep->next;
      free (dep);
    }
  free (data);
}

/* Determine invariantness of statements in basic block BB.  Callback
   for walk_dominator_tree.  */

static void
determine_invariantness_stmt (struct dom_walk_data *dw_data ATTRIBUTE_UNUSED,
			      basic_block bb)
{
  enum move_pos pos;
  block_stmt_iterator bsi;
  tree stmt;
  bool maybe_never = ALWAYS_EXECUTED_IN (bb) == NULL;

  if (!bb->loop_father->outer)
    return;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "Basic block %d (loop %d -- depth %d):\n\n",
	     bb->index, bb->loop_father->num, bb->loop_father->depth);

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      stmt = bsi_stmt (bsi);

      pos = movement_possibility (stmt);
      if (pos == MOVE_IMPOSSIBLE)
	{
	  if (nonpure_call_p (stmt))
	    maybe_never = true;
	  continue;
	}

      if (maybe_never && pos == MOVE_PRESERVE_EXECUTION)
	continue;

      stmt_ann (stmt)->aux = xcalloc (1, sizeof (struct lim_aux_data));
      if (!determine_max_movement (stmt, pos == MOVE_PRESERVE_EXECUTION))
	{
	  free_lim_aux_data (LIM_DATA (stmt));
	  stmt_ann (stmt)->aux = NULL;
	  continue;
	}

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  print_generic_stmt_indented (tree_dump_file, stmt, 0, 2);
	  fprintf (tree_dump_file, "  invariant up to level %d, cost %d.\n\n",
		   LIM_DATA (stmt)->max_loop->depth,
		   LIM_DATA (stmt)->cost);
	}

      if (LIM_DATA (stmt)->cost >= LIM_EXPENSIVE)
	set_profitable_level (stmt);
    }
}

/* For each statement determines the outermost loop in that it is invariant,
   statements on whose motion it depends and the cost of the computation.  */

static void
determine_invariantness (void)
{
  struct dom_walk_data walk_data;

  memset (&walk_data, 0, sizeof (struct dom_walk_data));
  walk_data.before_dom_children_before_stmts = determine_invariantness_stmt;

  init_walk_dominator_tree (&walk_data);
  walk_dominator_tree (&walk_data, ENTRY_BLOCK_PTR);
  fini_walk_dominator_tree (&walk_data);
}

/* Moves the statements in basic block BB to the right place.  Callback
   for walk_dominator_tree.  */

static void
move_computations_stmt (struct dom_walk_data *dw_data ATTRIBUTE_UNUSED,
			basic_block bb)
{
  struct loop *level;
  block_stmt_iterator bsi;
  tree stmt;
  unsigned cost = 0;

  if (!bb->loop_father->outer)
    return;

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); )
    {
      stmt = bsi_stmt (bsi);

      if (!LIM_DATA (stmt))
	level = NULL;
      else
	{
	  cost = LIM_DATA (stmt)->cost;
	  level = LIM_DATA (stmt)->tgt_loop;
	  free_lim_aux_data (LIM_DATA (stmt));
	  stmt_ann (stmt)->aux = NULL;
	}

      if (!level)
	{
	  bsi_next (&bsi);
	  continue;
	}

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "Moving statement\n");
	  print_generic_stmt (tree_dump_file, stmt, 0);
	  fprintf (tree_dump_file, "(cost %u) out of loop %d.\n\n",
		   cost, level->num);
	}
      bsi_insert_on_edge (loop_preheader_edge (level), stmt);
      bsi_remove (&bsi);
    }
}

/* Moves the statements to the requiered level.  */

static void
move_computations (void)
{
  struct dom_walk_data walk_data;
  int old_last_basic_block, i;

  memset (&walk_data, 0, sizeof (struct dom_walk_data));
  walk_data.before_dom_children_before_stmts = move_computations_stmt;

  init_walk_dominator_tree (&walk_data);
  walk_dominator_tree (&walk_data, ENTRY_BLOCK_PTR);
  fini_walk_dominator_tree (&walk_data);

  old_last_basic_block = last_basic_block;
  bsi_commit_edge_inserts (NULL);
  for (i = old_last_basic_block; i < last_basic_block; i++)
    {
      basic_block bb = BASIC_BLOCK (i);
      add_bb_to_loop (bb,
		      find_common_loop (bb->succ->dest->loop_father,
					bb->pred->src->loop_father));
    }
}

/* Fills ALWAYS_EXECUTED_IN for basic blocks in LOOP.  CONTAINS_CALL is
   the bitmap of blocks that contain a call.  */

static void
fill_always_executed_in (struct loop *loop, sbitmap contains_call)
{
  basic_block bb = NULL, *bbs, last = NULL;
  unsigned i;
  edge e;

  if (!loop->header->aux)
    {
      bbs = get_loop_body_in_dom_order (loop);

      for (i = 0; i < loop->num_nodes; i++)
	{
	  bb = bbs[i];

	  if (dominated_by_p (CDI_DOMINATORS, loop->latch, bb))
	    last = bb;

	  if (TEST_BIT (contains_call, bb->index))
	    break;

	  for (e = bb->succ; e; e = e->succ_next)
	    if (!flow_bb_inside_loop_p (loop, e->dest))
	      break;
	  if (e)
	    break;
	}

      while (1)
	{
	  last->aux = loop;
	  if (last == loop->header)
	    break;
	  last = get_immediate_dominator (CDI_DOMINATORS, last);
	}

      free (bbs);
    }

  for (loop = loop->inner; loop; loop = loop->next)
    fill_always_executed_in (loop, contains_call);
}

/* Compute information needed by the pass.  LOOPS is the loop tree.  */

static void
tree_ssa_lim_initialize (struct loops *loops)
{
  sbitmap contains_call = sbitmap_alloc (last_basic_block);
  block_stmt_iterator bsi;
  struct loop *loop;
  basic_block bb;

  /* Set ALWAYS_EXECUTED_IN.  Quadratic, can be improved.  */
  
  sbitmap_zero (contains_call);
  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  if (nonpure_call_p (bsi_stmt (bsi)))
	    break;
	}

      if (!bsi_end_p (bsi))
	SET_BIT (contains_call, bb->index);
    }

  for (loop = loops->tree_root->inner; loop; loop = loop->next)
    fill_always_executed_in (loop, contains_call);

  sbitmap_free (contains_call);
}

/* Cleans up after the invariant motion pass.  */

static void
tree_ssa_lim_finalize (void)
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      bb->aux = NULL;
    }
}

/* Moves invariants from LOOPS.  Only "expensive" invariants are moved out --
   i.e. those that are likely to be win regardless of the register presure.  */

void
tree_ssa_lim (struct loops *loops)
{
  tree_ssa_lim_initialize (loops);

  /* For each statement determine the outermost loop in that it is
     invariant and cost for computing the invariant.  */
  determine_invariantness ();

  /* Move the expressions that are expensive enough.  */
  move_computations ();

  tree_ssa_lim_finalize ();
}
