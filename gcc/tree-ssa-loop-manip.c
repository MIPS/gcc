/* High-level loop manipulation functions.
   Copyright (C) 2004 Free Software Foundation, Inc.
   
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
#include "tree-pass.h"
#include "cfglayout.h"
#include "tree-scalar-evolution.h"

static basic_block lv_adjust_loop_entry_edge (basic_block, basic_block, edge, 
					      tree); 
static void lv_update_pending_stmts (edge e);
static void lv_adjust_loop_header_phi (basic_block, basic_block, basic_block, 
				       edge);

/* Copies phi node arguments for duplicated blocks.  The index of the first
   duplicated block is FIRST_NEW_BLOCK.  */

static void
copy_phi_node_args (unsigned first_new_block)
{
  unsigned i;

  for (i = first_new_block; i < (unsigned) last_basic_block; i++)
    BASIC_BLOCK (i)->rbi->duplicated = 1;

  for (i = first_new_block; i < (unsigned) last_basic_block; i++)
    add_phi_args_after_copy_bb (BASIC_BLOCK (i));

  for (i = first_new_block; i < (unsigned) last_basic_block; i++)
    BASIC_BLOCK (i)->rbi->duplicated = 0;
}

/* Renames variables in the area copied by tree_duplicate_loop_to_header_edge.
   FIRST_NEW_BLOCK is the first block in the copied area.   DEFINITIONS is
   a bitmap of all ssa names defined inside the loop.  */

static void
rename_variables (unsigned first_new_block, bitmap definitions)
{
  unsigned i, copy_number = 0;
  basic_block bb;
  htab_t ssa_name_map = NULL;

  for (i = first_new_block; i < (unsigned) last_basic_block; i++)
    {
      bb = BASIC_BLOCK (i);

      /* We assume that first come all blocks from the first copy, then all
	 blocks from the second copy, etc.  */
      if (copy_number != (unsigned) bb->rbi->copy_number)
	{
	  allocate_ssa_names (definitions, &ssa_name_map);
	  copy_number = bb->rbi->copy_number;
	}

      rewrite_to_new_ssa_names_bb (bb, ssa_name_map);
    }

  htab_delete (ssa_name_map);
}

/* Sets SSA_NAME_DEF_STMT for results of all phi nodes in BB.  */

static void
set_phi_def_stmts (basic_block bb)
{
  tree phi;

  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    SSA_NAME_DEF_STMT (PHI_RESULT (phi)) = phi;
}

/* The same ad cfgloopmanip.c:duplicate_loop_to_header_edge, but also updates
   ssa.  In order to achieve this, only loops whose exits all lead to the same
   location are handled.
   
   FIXME: we create some degenerate phi nodes that could be avoided by copy
   propagating them instead.  Unfortunately this is not completely
   straightforward due to problems with constant folding.  */

bool
tree_duplicate_loop_to_header_edge (struct loop *loop, edge e,
				    struct loops *loops,
				    unsigned int ndupl, sbitmap wont_exit,
				    edge orig, edge *to_remove,
				    unsigned int *n_to_remove, int flags)
{
  unsigned first_new_block;
  basic_block bb;
  unsigned i;
  tree phi, arg, map, def;
  bitmap definitions;

  if (!(loops->state & LOOPS_HAVE_SIMPLE_LATCHES))
    return false;
  if (!(loops->state & LOOPS_HAVE_PREHEADERS))
    return false;

#ifdef ENABLE_CHECKING
  verify_loop_closed_ssa ();
#endif

  if (any_marked_for_rewrite_p ())
    abort ();

  first_new_block = last_basic_block;
  if (!duplicate_loop_to_header_edge (loop, e, loops, ndupl, wont_exit,
				      orig, to_remove, n_to_remove, flags))
    return false;

  /* Readd the removed phi args for e.  */
  map = PENDING_STMT (e);
  PENDING_STMT (e) = NULL;

  for (phi = phi_nodes (e->dest), arg = map;
       phi;
       phi = TREE_CHAIN (phi), arg = TREE_CHAIN (arg))
    {
      def = TREE_VALUE (arg);
      add_phi_arg (&phi, def, e);
    }
  if (arg)
    abort ();

  /* Copy the phi node arguments.  */
  copy_phi_node_args (first_new_block);

  /* Rename the variables.  */
  definitions = marked_ssa_names ();
  rename_variables (first_new_block, definitions);
  unmark_all_for_rewrite ();
  BITMAP_XFREE (definitions);

  /* For some time we have the identical ssa names as results in multiple phi
     nodes.  When phi node is resized, it sets SSA_NAME_DEF_STMT of its result
     to the new copy.  This means that we cannot easily ensure that the ssa
     names defined in those phis are pointing to the right one -- so just
     recompute SSA_NAME_DEF_STMT for them.  */ 

  for (i = first_new_block; i < (unsigned) last_basic_block; i++)
    {
      bb = BASIC_BLOCK (i);
      set_phi_def_stmts (bb);
      if (bb->rbi->copy_number == 1)
  	set_phi_def_stmts (bb->rbi->original);
    }

  scev_reset ();
#ifdef ENABLE_CHECKING
  verify_loop_closed_ssa ();
#endif

  return true;
}

/*---------------------------------------------------------------------------
  Loop versioning
  ---------------------------------------------------------------------------*/
 
/* Adjust entry edge for lv.
   
  e is a incoming edge. 

  --- edge e ---- > [second_head]

  Split it and insert new conditional expression and adjust edges.
   
   --- edge e ---> [cond expr] ---> [first_head]
                        |
                        +---------> [second_head]

*/
   
static basic_block
lv_adjust_loop_entry_edge (basic_block first_head,
			   basic_block second_head,
			   edge e,
			   tree cond_expr)
{ 
  block_stmt_iterator bsi;
  basic_block orig_head = e->src;
  basic_block new_head = NULL;
  tree goto1 = NULL_TREE;
  tree goto2 = NULL_TREE;
  tree new_cond_expr = NULL_TREE;
  edge e0, e1;

  /* Split edge 'e'. This will create a new basic block, where we can
     insert conditional expr.  */
  new_head = split_edge (e);
  set_immediate_dominator (CDI_DOMINATORS, new_head, orig_head);

  /* Build new conditional expr */
  goto1 = build1 (GOTO_EXPR, void_type_node, tree_block_label (first_head));
  goto2 = build1 (GOTO_EXPR, void_type_node, tree_block_label (second_head));
  new_cond_expr = build (COND_EXPR, void_type_node, cond_expr, goto1, goto2);

  /* Add new cond. in new head.  */ 
  bsi = bsi_start (new_head); 
  bsi_insert_after (&bsi, new_cond_expr, BSI_NEW_STMT);

  /* Adjust edges appropriately to connect new head with first head
     as well as second head.  */
  e1 = make_edge (new_head, first_head, EDGE_TRUE_VALUE);
  set_immediate_dominator (CDI_DOMINATORS, first_head, new_head);
  make_edge (new_head, second_head, EDGE_FALSE_VALUE);
  set_immediate_dominator (CDI_DOMINATORS, second_head, new_head);

  /* Adjust loop header phi nodes.  */
  lv_adjust_loop_header_phi (first_head, second_head, new_head, e1);

  /* When edge 'e' was split, it created a fall through edge
      from new head to second head. Above created FALSE edge
      from new head to second head and now we do not need the
      fall through edge.  */
  for (e0 = new_head->succ; e0; e0 = e0->succ_next)
    if (e0->dest == second_head)
      e0->flags &= ~EDGE_FALLTHRU;

  return new_head;
}

/* Add phi args using PENDINT_STMT list.  */

static void
lv_update_pending_stmts (edge e)
{
  basic_block dest;
  tree phi, arg, def;

  if (!PENDING_STMT (e))
    return;

  dest = e->dest;

  for (phi = phi_nodes (dest), arg = PENDING_STMT (e);
       phi;
       phi = TREE_CHAIN (phi), arg = TREE_CHAIN (arg))
    {
      def = TREE_VALUE (arg);
      add_phi_arg (&phi, def, e);
    }

  PENDING_STMT (e) = NULL;
}

/* Adjust phi nodes for 'first' basic block.  'second' basic block is a copy
   of 'first'. Both of them are dominated by 'new_head' basic block. When
   'new_head' was created by 'second's incoming edge it received phi arguments
   on the edge by split_edge(). Later, additional edge 'e' was created to
   connect 'new_head' and 'first'. Now this routine adds phi args on this 
   additional edge 'e' that new_head to second edge received as part of edge 
   splitting.
*/

static void
lv_adjust_loop_header_phi (basic_block first, basic_block second,
			   basic_block new_head, edge e)
{
  tree phi1, phi2;

  /* Browse all 'second' basic block phi nodes and add phi args to
     edge 'e' for 'first' head. PHI args are always in correct order.  */

  for (phi2 = phi_nodes (second), phi1 = phi_nodes (first); 
       phi2 && phi1; 
       phi2 = TREE_CHAIN (phi2),  phi1 = TREE_CHAIN (phi1))
    {
      int i;
      for (i = 0; i < PHI_NUM_ARGS (phi2); i++)
	{
	  if (PHI_ARG_EDGE (phi2, i)->src == new_head)
	    {
	      tree def = PHI_ARG_DEF (phi2, i);
	      add_phi_arg (&phi1, def, e);
	    }
	}
    }
}


/* Main entry point for Loop Versioning transformation.
   
This transformation given a condition and a loop, creates
-if (condition) { loop_copy1 } else { loop_copy2 },
where loop_copy1 is the loop transformed in one way, and loop_copy2
is the loop transformed in another way (or unchanged). 'condition'
may be a run time test for things that were not resolved by static
analysis (overlapping ranges (anti-aliasing), alignment, etc.).  */

struct loop *
tree_ssa_loop_version (struct loops *loops, struct loop * loop, 
		       tree cond_expr, basic_block *condition_bb)
{
  edge entry, latch_edge, exit;
  basic_block first_head, second_head;
  int irred_flag;
  struct loop *nloop;

  /* CHECKME: Loop versioning does not handle nested loop at this point.  */
  if (loop->inner)
    return NULL;

  /* Record entry and latch edges for the loop */
  entry = loop_preheader_edge (loop);

  /* Note down head of loop as first_head.  */
  first_head = entry->dest;

  /* Duplicate loop.  */
  irred_flag = entry->flags & EDGE_IRREDUCIBLE_LOOP;
  entry->flags &= ~EDGE_IRREDUCIBLE_LOOP;
  if (!tree_duplicate_loop_to_header_edge (loop, entry, loops, 1,
					   NULL, NULL, NULL, NULL, 0))
    {
      entry->flags |= irred_flag;
      return NULL;
    }

  /* After duplication entry edge now points to new loop head block.
     Note down new head as second_head.  */
  second_head = entry->dest;

  /* Split loop entry edge and insert new block with cond expr.  */
  *condition_bb = lv_adjust_loop_entry_edge (first_head, second_head, entry, 
					    cond_expr); 

  latch_edge = loop->latch->rbi->copy->succ;
  nloop = loopify (loops, 
		   latch_edge,
		   loop->header->rbi->copy->pred,
		   *condition_bb,
		   false /* Do not redirect all edges.  */);

  exit = loop->single_exit;
  if (exit)
    nloop->single_exit = find_edge (exit->src->rbi->copy, exit->dest);

  /* loopify redirected latch_edge. Update its PENDING_STMTS.  */ 
  lv_update_pending_stmts (latch_edge);

  /* loopify redirected condition_bb's succ edge. Update its PENDING_STMTS.  */ 
  lv_update_pending_stmts (FALLTHRU_EDGE (*condition_bb));

  /* Adjust irreducible flag.  */
  if (irred_flag)
    {
      (*condition_bb)->flags |= BB_IRREDUCIBLE_LOOP;
      loop_preheader_edge (loop)->flags |= EDGE_IRREDUCIBLE_LOOP;
      loop_preheader_edge (nloop)->flags |= EDGE_IRREDUCIBLE_LOOP;
      (*condition_bb)->pred->flags |= EDGE_IRREDUCIBLE_LOOP;
    }

  /* At this point condition_bb is loop predheader with two successors, 
     first_head and second_head.   Make sure that loop predheader has only 
     one successor. */
  loop_split_edge_with (loop_preheader_edge (loop), NULL);
  loop_split_edge_with (loop_preheader_edge (nloop), NULL);

  return nloop;
}

/* Update loop versioning condition.
   This is used by other optimizations/transformations to disable
   one loop version.  */

void
update_lv_condition (basic_block *bb, tree new_cond)
{
  tree stmt;
  block_stmt_iterator bsi = bsi_last (*bb);

  stmt = bsi_stmt (bsi);

  if (TREE_CODE (stmt) == COND_EXPR)
    {
      TREE_OPERAND (stmt, 0) = new_cond;
      modify_stmt (stmt);
    }
  else
    abort ();
}

/* Add exit phis for the USE on EXIT.  */

static void
add_exit_phis_edge (basic_block exit, tree use)
{
  tree phi, def_stmt = SSA_NAME_DEF_STMT (use);
  basic_block def_bb = bb_for_stmt (def_stmt);
  struct loop *def_loop;
  edge e;

  /* Check that some of the edges entering the EXIT block exits a loop in
     that USE is defined.  */
  for (e = exit->pred; e; e = e->pred_next)
    {
      def_loop = find_common_loop (def_bb->loop_father, e->src->loop_father);
      if (!flow_bb_inside_loop_p (def_loop, e->dest))
	break;
    }

  if (!e)
    return;

  phi = create_phi_node (use, exit);

  for (e = exit->pred; e; e = e->pred_next)
    add_phi_arg (&phi, use, e);

  SSA_NAME_DEF_STMT (use) = def_stmt;
}

/* Add exit phis for VAR that is used in LIVEIN.
   Exits of the loops are stored in EXITS.  */

static void
add_exit_phis_var (tree var, bitmap livein, bitmap exits)
{
  bitmap def;
  int index;
  basic_block def_bb = bb_for_stmt (SSA_NAME_DEF_STMT (var));

  bitmap_clear_bit (livein, def_bb->index);

  def = BITMAP_XMALLOC ();
  bitmap_set_bit (def, def_bb->index);
  compute_global_livein (livein, def);
  BITMAP_XFREE (def);

  EXECUTE_IF_AND_IN_BITMAP (exits, livein, 0, index,
			    add_exit_phis_edge (BASIC_BLOCK (index), var));
}

/* Add exit phis for the names marked in NAMES_TO_RENAME.
   Exits of the loops are stored in EXITS.  Sets of blocks where the ssa
   names are used are stored in USE_BLOCKS.  */

static void
add_exit_phis (bitmap names_to_rename, bitmap *use_blocks, bitmap loop_exits)
{
  unsigned i;

  EXECUTE_IF_SET_IN_BITMAP (names_to_rename, 0, i,
    {
      add_exit_phis_var (ssa_name (i), use_blocks[i], loop_exits);
    });
}

/* Returns a bitmap of all loop exit edge targets.  */

static bitmap
get_loops_exits (void)
{
  bitmap exits = BITMAP_XMALLOC ();
  basic_block bb;
  edge e;

  FOR_EACH_BB (bb)
    {
      for (e = bb->pred; e; e = e->pred_next)
	if (e->src != ENTRY_BLOCK_PTR
	    && !flow_bb_inside_loop_p (e->src->loop_father, bb))
	  {
	    bitmap_set_bit (exits, bb->index);
	    break;
	  }
    }

  return exits;
}

/* For USE in BB, if it is used outside of the loop it is defined in,
   mark it for rewrite.  Record basic block BB where it is used
   to USE_BLOCKS.  */

static void
find_uses_to_rename_use (basic_block bb, tree use, bitmap *use_blocks)
{
  unsigned ver;
  basic_block def_bb;
  struct loop *def_loop;

  if (TREE_CODE (use) != SSA_NAME)
    return;

  ver = SSA_NAME_VERSION (use);
  def_bb = bb_for_stmt (SSA_NAME_DEF_STMT (use));
  if (!def_bb)
    return;
  def_loop = def_bb->loop_father;

  /* If the definition is not inside loop, it is not interesting.  */
  if (!def_loop->outer)
    return;

  if (!use_blocks[ver])
    use_blocks[ver] = BITMAP_XMALLOC ();
  bitmap_set_bit (use_blocks[ver], bb->index);

  if (!flow_bb_inside_loop_p (def_loop, bb))
    mark_for_rewrite (use);
}

/* For uses in STMT, mark names that are used outside of the loop they are
   defined to rewrite.  Record the set of blocks in that the ssa
   names are defined to USE_BLOCKS.  */

static void
find_uses_to_rename_stmt (tree stmt, bitmap *use_blocks)
{
  use_optype uses;
  vuse_optype vuses;
  v_may_def_optype v_may_defs;
  stmt_ann_t ann;
  unsigned i;
  basic_block bb = bb_for_stmt (stmt);

  get_stmt_operands (stmt);
  ann = stmt_ann (stmt);

  uses = USE_OPS (ann);
  for (i = 0; i < NUM_USES (uses); i++)
    find_uses_to_rename_use (bb, USE_OP (uses, i), use_blocks);

  vuses = VUSE_OPS (ann);
  for (i = 0; i < NUM_VUSES (vuses); i++)
    find_uses_to_rename_use (bb, VUSE_OP (vuses, i),use_blocks);

  v_may_defs = V_MAY_DEF_OPS (ann);
  for (i = 0; i < NUM_V_MAY_DEFS (v_may_defs); i++)
    find_uses_to_rename_use (bb, V_MAY_DEF_OP (v_may_defs, i), use_blocks);
}

/* Marks names that are used outside of the loop they are defined in
   for rewrite.  Records the set of blocks in that the ssa
   names are defined to USE_BLOCKS.  */

static void
find_uses_to_rename (bitmap *use_blocks)
{
  basic_block bb;
  block_stmt_iterator bsi;
  tree phi;
  unsigned i;

  FOR_EACH_BB (bb)
    {
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	for (i = 0; i < (unsigned) PHI_NUM_ARGS (phi); i++)
	  find_uses_to_rename_use (PHI_ARG_EDGE (phi, i)->src,
				   PHI_ARG_DEF (phi, i), use_blocks);

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	find_uses_to_rename_stmt (bsi_stmt (bsi), use_blocks);
    }
}

/* Rewrites the program into a loop closed ssa form -- i.e. inserts extra
   phi nodes to ensure that no variable is used outside the loop it is
   defined in.

   This strengthening of the basic ssa form has several advantages:

   1) Updating it during unrolling/peeling/versioning is trivial, since
      we do not need to care about the uses outside of the loop.
   2) The behavior of all uses of an induction variable is the same.
      Without this, you need to distinguish the case when the variable
      is used outside of the loop it is defined in, for example

      for (i = 0; i < 100; i++)
	{
	  for (j = 0; j < 100; j++)
	    {
	      k = i + j;
	      use1 (k);
	    }
	  use2 (k);
	}

      Looking from the outer loop with the normal SSA form, the first use of k
      is not well-behaved, while the second one is an induction variable with
      base 99 and step 1.  */

void
rewrite_into_loop_closed_ssa (void)
{
  bitmap loop_exits = get_loops_exits ();
  bitmap *use_blocks;
  unsigned i;
  bitmap names_to_rename;

  tree_ssa_dce_no_cfg_changes ();

  if (any_marked_for_rewrite_p ())
    abort ();

  use_blocks = xcalloc (num_ssa_names, sizeof (bitmap));

  /* Find the uses outside loops.  */
  find_uses_to_rename (use_blocks);

  /* Add the phi nodes on exits of the loops for the names we need to
     rewrite.  */
  names_to_rename = marked_ssa_names ();
  add_exit_phis (names_to_rename, use_blocks, loop_exits);

  for (i = 0; i < num_ssa_names; i++)
    BITMAP_XFREE (use_blocks[i]);
  free (use_blocks);
  BITMAP_XFREE (loop_exits);
  BITMAP_XFREE (names_to_rename);

  /* Do the rewriting.  */
  rewrite_ssa_into_ssa ();
}

/* Check invariants of the loop closed ssa form for the USE in BB.  */

static void
check_loop_closed_ssa_use (basic_block bb, tree use)
{
  tree def;
  basic_block def_bb;
  
  if (TREE_CODE (use) != SSA_NAME)
    return;

  def = SSA_NAME_DEF_STMT (use);
  def_bb = bb_for_stmt (def);
  if (def_bb
      && !flow_bb_inside_loop_p (def_bb->loop_father, bb))
    abort ();
}

/* Checks invariants of loop closed ssa form in statement STMT in BB.  */

static void
check_loop_closed_ssa_stmt (basic_block bb, tree stmt)
{
  use_optype uses;
  vuse_optype vuses;
  v_may_def_optype v_may_defs;
  stmt_ann_t ann;
  unsigned i;

  get_stmt_operands (stmt);
  ann = stmt_ann (stmt);

  uses = USE_OPS (ann);
  for (i = 0; i < NUM_USES (uses); i++)
    check_loop_closed_ssa_use (bb, USE_OP (uses, i));

  vuses = VUSE_OPS (ann);
  for (i = 0; i < NUM_VUSES (vuses); i++)
    check_loop_closed_ssa_use (bb, VUSE_OP (vuses, i));

  v_may_defs = V_MAY_DEF_OPS (ann);
  for (i = 0; i < NUM_V_MAY_DEFS (v_may_defs); i++)
    check_loop_closed_ssa_use (bb, V_MAY_DEF_OP (v_may_defs, i));
}

/* Checks that invariants of the loop closed ssa form are preserved.  */

void
verify_loop_closed_ssa (void)
{
  basic_block bb;
  block_stmt_iterator bsi;
  tree phi;
  unsigned i;

  verify_ssa ();

  FOR_EACH_BB (bb)
    {
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	for (i = 0; i < (unsigned) PHI_NUM_ARGS (phi); i++)
	  check_loop_closed_ssa_use (PHI_ARG_EDGE (phi, i)->src,
				     PHI_ARG_DEF (phi, i));

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	check_loop_closed_ssa_stmt (bb, bsi_stmt (bsi));
    }
}


/* Duplicates LOOP to the exit edge EXIT.  The original loops exit condition
   is changed to EXIT_ON.  The newly created loop is returned.  In case
   duplication fails, NULL is returned instead.

   I.e.

   while (1)
     {
       before;
       if (cond)
         break;
       after;
     }

   is transformed into

   while (1)
     {
       before;
       if (exit_on)
         break;
       after;
     }

   while (!cond)
     {
       after;
       before;
     } */

struct loop *
tree_split_loop_iterations (struct loop *loop, edge exit, tree exit_on,
			    struct loops *loops)
{
  basic_block *bbs, *new_bbs, *doms, new_latch_orig;
  struct loop *new_loop, *aloop;
  block_stmt_iterator bsi;
  tree stmt;
  edge new_exit, e;
  unsigned n_bbs, n_doms, i;
  tree phi;
  bitmap definitions_before, all_definitions;
  unsigned exit_bb_index;
  htab_t ssa_name_map = NULL;

  if (any_marked_for_rewrite_p ())
    abort ();

  /* Check whether duplication is possible.  */

  if (!just_once_each_iteration_p (loop, exit->src))
    return NULL;

  if (loop->inner)
    return NULL;

  if (!can_duplicate_loop_p (loop))
    return NULL;

  /* Ensure that the exit is in a separate basic block.  */
  bsi = bsi_last (exit->src);
  if (bsi_end_p (bsi))
    return NULL;
  stmt = bsi_stmt (bsi);
  if (TREE_CODE (stmt) != COND_EXPR)
    return NULL;
  bsi_prev (&bsi);
  if (!bsi_end_p (bsi))
    {
      stmt = bsi_stmt (bsi);
      split_loop_bb (exit->src, stmt);
    }

  /* And that it has just a single predecessor that has just a single
     successor, and that it contains no phi nodes.  */
  if (exit->src->pred->pred_next
      || exit->src->pred->src->succ->succ_next
      || phi_nodes (exit->src))
    split_loop_bb (exit->src, NULL);
  new_latch_orig = exit->src->pred->src;

  /* The duplication itself.  */
  n_bbs = loop->num_nodes;
  bbs = get_loop_body_in_dom_order (loop);
  for (exit_bb_index = 0; bbs[exit_bb_index] != exit->src; exit_bb_index++)
    continue;

  if (loops->state & LOOPS_HAVE_MARKED_SINGLE_EXITS)
    {
      /* Exclude the EXIT from consideration for now, since it is not going to
	 be really duplicated.  */
      update_single_exits_after_duplication (bbs, exit_bb_index,
					     loop->outer);
      update_single_exits_after_duplication (bbs + exit_bb_index + 1,
					     n_bbs - exit_bb_index - 1,
					     loop->outer);
    }

  new_loop = duplicate_loop (loops, loop, loop->outer);
  new_bbs = xmalloc (sizeof (basic_block) * n_bbs);

  copy_bbs (bbs, exit_bb_index, new_bbs, NULL, 0, NULL, loop->outer);
  definitions_before = marked_ssa_names ();
  copy_bbs (bbs + exit_bb_index, n_bbs - exit_bb_index, new_bbs + exit_bb_index,
	    &exit, 1, &new_exit, loop->outer);
  all_definitions = marked_ssa_names ();

  /* Due to copying the loop in two parts some edges were not redirected.  */
  redirect_edge_and_branch_force (new_latch_orig->rbi->copy->succ,
				  new_exit->src);
  redirect_edge_and_branch_force (loop->latch->rbi->copy->succ,
				  loop->header->rbi->copy);

  add_phi_args_after_copy (new_bbs, n_bbs);

  set_immediate_dominator (CDI_DOMINATORS, new_exit->src, exit->src);
  set_immediate_dominator (CDI_DOMINATORS, new_loop->header, new_loop->latch);
  new_loop->header = new_exit->src;
  new_loop->latch = new_latch_orig->rbi->copy;

  if (loops->state & LOOPS_HAVE_MARKED_SINGLE_EXITS)
    {
      if (loop->single_exit == exit)
	new_loop->single_exit = new_exit;

      for (aloop = loop->outer;
	   !flow_bb_inside_loop_p (aloop, exit->dest);
	   aloop = aloop->outer)
	if (aloop->single_exit == exit)
	  aloop->single_exit = new_exit;
    }

  redirect_edge_and_branch_force (exit, new_exit->src);
  /* Forget the phi nodes on the exit of the original loop, since they are no
     longer used.  */
  PENDING_STMT (exit) = NULL;

  stmt = last_stmt (exit->src);
  COND_EXPR_COND (stmt) = exit_on;
  if (exit->flags & EDGE_FALSE_VALUE)
    {
      exit->flags &= ~EDGE_FALSE_VALUE;
      exit->flags |= EDGE_TRUE_VALUE;

      e = exit->src->succ;
      if (e == exit)
	e = e->succ_next;

      exit->flags &= ~EDGE_TRUE_VALUE;
      exit->flags |= EDGE_FALSE_VALUE;
    }

  /* Update the dominators.  */
  doms = xmalloc (sizeof (basic_block) * n_basic_blocks);
  n_doms = get_dominated_by_region (CDI_DOMINATORS, bbs, n_bbs, doms);
  iterate_fix_dominators (CDI_DOMINATORS, doms, n_doms);
  free (doms);

  /* Rewrite the ssa names in the copy of the loop.  But first create the phi
     nodes that transfer values from LOOP to NEW_LOOP.  TODO -- for now we
     transfer all values defined before the exit, it is sufficient to transfer
     those that are used after the exit.  */
  EXECUTE_IF_SET_IN_BITMAP (definitions_before, 0, i,
    {
      phi = create_phi_node (ssa_name (i), new_loop->header);
      add_phi_arg (&phi, ssa_name (i), new_loop->header->pred);
      add_phi_arg (&phi, ssa_name (i), new_loop->header->pred->pred_next);
    });
  allocate_ssa_names (all_definitions, &ssa_name_map);
  rewrite_to_new_ssa_names (new_bbs, exit_bb_index, ssa_name_map);
  allocate_ssa_names (definitions_before, &ssa_name_map);
  rewrite_to_new_ssa_names (new_bbs + exit_bb_index, n_bbs - exit_bb_index,
			    ssa_name_map);

  /* Ensure that NEW_LOOP has a simple preheader.  */
  split_loop_exit_edge (exit);

#ifdef ENABLE_CHECKING
  verify_dominators (CDI_DOMINATORS);
  verify_loop_structure (loops);
  verify_loop_closed_ssa ();
#endif

  free (new_bbs);
  free (bbs);

  unmark_all_for_rewrite ();
  BITMAP_XFREE (definitions_before);
  BITMAP_XFREE (all_definitions);
  htab_delete (ssa_name_map);

  return new_loop;
}

/* Split loop exit edge EXIT.  The things are a bit complicated by a need to
   preserve the loop closed ssa form.  */

void
split_loop_exit_edge (edge exit)
{
  basic_block dest = exit->dest;
  basic_block bb = loop_split_edge_with (exit, NULL);
  tree phi, new_phi, new_name;
  use_operand_p op_p;

  for (phi = phi_nodes (dest); phi; phi = TREE_CHAIN (phi))
    {
      op_p = PHI_ARG_DEF_PTR_FROM_EDGE (phi, bb->succ);

      new_name = duplicate_ssa_name (USE_FROM_PTR (op_p), NULL);
      new_phi = create_phi_node (new_name, bb);
      SSA_NAME_DEF_STMT (new_name) = new_phi;
      add_phi_arg (&new_phi, USE_FROM_PTR (op_p), exit);
      SET_USE (op_p, new_name);
    }
}

/* Insert statement STMT to the edge E and update the loop structures.
   Returns the newly created block (if any).  */

basic_block
bsi_insert_on_edge_immediate_loop (edge e, tree stmt)
{
  basic_block src, dest, new_bb;
  struct loop *loop_c;

  src = e->src;
  dest = e->dest;

  loop_c = find_common_loop (src->loop_father, dest->loop_father);

  new_bb = bsi_insert_on_edge_immediate (e, stmt);

  if (!new_bb)
    return NULL;

  add_bb_to_loop (new_bb, loop_c);
  if (dest->loop_father->latch == src)
    dest->loop_father->latch = new_bb;

  return new_bb;
}

/* Ensure that the number of iterations of LOOP is divisible by MOD,
   by moving a few last iterations to the new loop.  EXIT is the exit edge
   on that the new loop is created.  NITER is the number of iterations
   of the LOOP before it exits through EXIT.  LOOPS is the loop tree.
   MOD must be a power of two.
   
   For example
   
   while (1)
     {
       before;
       if (cond)
         break;
       after;
     }

   is transformed into

   niter1 = (niter + 1) & ~(mod - 1);
   if (niter < mod - 1)
     before;
   else
     {
       while (1)
         {
            before;
	    if (--niter1 == 0)
	      break;
	    after;
	 }
     }
   while (!cond)
     {
       after;
       before;
     }
   */ 

struct loop *
tree_align_loop_iterations (struct loop *loop, edge exit, tree niter,
			    unsigned mod, struct loops *loops)
{
  tree niter1, stmts, type, tmod, niter_count, exit_cond_stmt;
  tree var_base, exit_cond, init_cond, always_exit;
  struct loop *new_loop;
  block_stmt_iterator bsi;
  bool after;
  basic_block init_cond_bb;

  /* MOD must be a power of two.  */
  if (mod & (mod - 1))
    abort ();

  /* Prepare the expression for # of iterations.  */
  type = TREE_TYPE (niter);
  var_base = create_tmp_var (type, "unaligned_niter_tmp");
  add_referenced_tmp_var (var_base);

  niter = force_gimple_operand (unshare_expr (niter), &stmts, false, var_base);
  if (stmts)
    bsi_insert_on_edge_immediate_loop (loop_preheader_edge (loop),
				       stmts);

  niter1 = build2 (PLUS_EXPR, type,
		   unshare_expr (niter), build_int_cst (type, 1));
  tmod = fold (build1 (BIT_NOT_EXPR, type, build_int_cst (type, mod - 1)));
  niter1 = build2 (BIT_AND_EXPR, type, niter1, tmod);
  niter1 = fold (niter1);

  var_base = create_tmp_var (type, "aligned_niter_tmp");
  add_referenced_tmp_var (var_base);
  niter1 = force_gimple_operand (niter1, &stmts, false, var_base);
  if (stmts)
    bsi_insert_on_edge_immediate_loop (loop_preheader_edge (loop),
				       stmts);

  /* Prepare the counter of iterations of the loop.  */
  var_base = create_tmp_var (type, "align_niter_iv");
  add_referenced_tmp_var (var_base);
  standard_iv_increment_position (loop, &bsi, &after);
  create_iv (niter1, build_int_cst (type, (HOST_WIDE_INT) -1),
	     var_base, loop, &bsi, after, &niter_count, NULL);

  /* Split out the new loop and insert the niter1 != 1 exit condition to
     the original loop.  */
  exit_cond = build2 (EQ_EXPR, boolean_type_node,
		      niter_count, build_int_cst (type, 1));
  new_loop = tree_split_loop_iterations (loop, exit, exit_cond, loops);
  if (!new_loop)
    return NULL;

  /* Use loop versioning to create the niter < mod - 1 guard.  */
  init_cond = build2 (GE_EXPR, boolean_type_node,
		      niter,  build_int_cst (type, mod - 1));
  tree_ssa_loop_version (loops, loop, init_cond, &init_cond_bb);
  
  /* Make the "after" part of the second version of the loop
     unreachable.  */
  if (exit->flags & EDGE_TRUE_VALUE)
    always_exit = boolean_true_node;
  else
    always_exit = boolean_false_node;
  exit_cond_stmt = last_stmt (exit->src->rbi->copy);
  COND_EXPR_COND (exit_cond_stmt) = always_exit;

  return new_loop;
}
