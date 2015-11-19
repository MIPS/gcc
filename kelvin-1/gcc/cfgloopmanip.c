/* Loop manipulation code for GNU compiler.
   Copyright (C) 2002-2015 Free Software Foundation, Inc.

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

#define KELVIN_PATCH
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "cfghooks.h"
#include "tree.h"
#include "gimple.h"
#include "rtl.h"
#include "cfganal.h"
#include "cfgloop.h"
#include "fold-const.h"
#include "internal-fn.h"
#include "gimple-iterator.h"
#include "gimplify-me.h"
#include "tree-ssa-loop-manip.h"
#include "dumpfile.h"
#ifdef KELVIN_PATCH
#include "diagnostic-core.h"

/* Use a command-line option to set the flag_checking option to enforce
   the following run-time checks.
   a. The sum of outgoing edge frequencies for the loop equals the
      sum of incoming edge frequencies for the loop header block.
   b. The sum of predecessor edge frequencies for every block
      in the loop equals the frequency of that block. */
#define KELVIN_NOISE
#ifdef KELVIN_NOISE
#include "kelvin-debugs.c"
#endif
#endif

static void copy_loops_to (struct loop **, int,
			   struct loop *);
static void loop_redirect_edge (edge, basic_block);
static void remove_bbs (basic_block *, int);
static bool rpe_enum_p (const_basic_block, const void *);
static int find_path (edge, basic_block **);
static void fix_loop_placements (struct loop *, bool *);
static bool fix_bb_placement (basic_block);
static void fix_bb_placements (basic_block, bool *, bitmap);

#ifdef KELVIN_PATCH
/* Return true iff BLOCK is considered to reside within the loop
   represented by LOOP_PTR. */
bool
in_loop_p (basic_block block, struct loop *loop_ptr)
{
  basic_block *bbs = get_loop_body (loop_ptr);
  bool result = false;

  for (unsigned int i = 0; i < loop_ptr->num_nodes; i++)
    {
      if (bbs[i] == block)
	result = true;
    }
  free (bbs);
  return result;
}

/* Zero all frequencies associated with loop LOOP_PTR. */
void 
zero_loop_frequencies (struct loop *loop_ptr)
{
  basic_block *bbs = get_loop_body (loop_ptr);
  for (unsigned i = 0; i < loop_ptr->num_nodes; ++i)
    {
      bbs[i]->frequency = 0;
    }
  free (bbs);
}

/* A list of block_ladder_rung structs is used to keep track of all
   the blocks visited in a depth-first recursive traversal of a control-flow
   graph.  This list is used to detect and prevent attempts to revisit
   a block that is already being visited in the recursive traversal. */
typedef struct block_ladder_rung {
  basic_block block;
  struct block_ladder_rung *lower_rung;
} *ladder_rung_p;

/* Return true iff AN_EDGE matches one of the nodes that is already
   present within SET_OF_EDGES. */
static bool 
in_edge_set_p (edge an_edge, vec<edge> set_of_edges)
{
  unsigned int j;
  edge e;

  FOR_EACH_VEC_ELT (set_of_edges, j, e)
    {
      if (e == an_edge)
	return true;
    }
  return false;
}

/* Return true iff AN_EDGE->dest is already represented within
   the LADDER_RUNG list. */
static bool 
in_call_chain_p (edge an_edge, ladder_rung_p ladder_rung)
{
  while (ladder_rung != NULL)
    {
      if (an_edge->dest == ladder_rung->block)
	return true;
      else
	ladder_rung = ladder_rung->lower_rung;
    }
  return FALSE;
}

/* This recursive function visits all of the blocks contained within the
   loop represented by LOOP_PTR and reachable from INCOMING_EDGE
   and zeroes the frequency field of each block.  The recursion
   terminates if INCOMING_EDGE is known to exit this loop, or
   if the destination of INCOMING_EDGE has already been visited
   in this recursive traversal, or if the destination of INCOMING_EDGE
   is the loop header. */
static void
recursively_zero_frequency (struct loop *loop_ptr, vec<edge> exit_edges,
			    ladder_rung_p ladder_rung,
			    edge incoming_edge)
{
  if (incoming_edge->dest == loop_ptr->header)
    return;
  else if (in_edge_set_p (incoming_edge, exit_edges))
    return;
  else if (in_call_chain_p (incoming_edge, ladder_rung))
    return;
  else
    {
      struct block_ladder_rung a_rung;
      basic_block block = incoming_edge->dest;
      
      a_rung.block = block;
      a_rung.lower_rung = ladder_rung;
      block->frequency = 0;
      
      edge_iterator ei;
      edge successor;
      FOR_EACH_EDGE (successor, ei, block->succs)
	recursively_zero_frequency (loop_ptr, exit_edges,
				    &a_rung, successor);
    }
}
				     
/* Return true iff the CANDIDATE block is found within the linked
   list represented by LOWER_STEPS, which would indicate that this
   block has already been visited by a recursive traversal. */
static bool 
recursion_detected_p (basic_block candidate, ladder_rung_p lower_steps)
{
  while (lower_steps != NULL)
    {
      if (lower_steps->block == candidate)
	return true;
      lower_steps = lower_steps->lower_rung;
    }
  /* we iterated through the entire list and did not find candidate */
  return false;
}

/* Return true iff CANDIDATE is contained within the loop represented
   by LOOP_HEADER and LOOP_LATCH.

   We consider the block to be within the loop if there exists a path
   within the control flow graph from this node to the loop's latch
   which does not pass through the loop's header.  (If all paths to
   the latch pass through the loop header, then the node is contained
   within an outer-nested loop but not within this loop.)
  
   Note that if a CANDIDATE's successor is in the loop and the successor
   is not the loop header, then the candidate itself is also in the loop.
   If none of the successors of a candidate are in the loop, then the
   candidate itself is not in the loop. */
static bool 
in_loop_of_header_p (basic_block candidate, basic_block loop_header,
		      basic_block loop_latch, bool start_of_recursion,
		      ladder_rung_p lower_steps)
{
  if (candidate == loop_latch)
    return true;
  else if (candidate == loop_header) 
    return start_of_recursion;
  else if (!start_of_recursion 
	   && recursion_detected_p (candidate, lower_steps))
    {
      /* if recursion revisits a node already visited and the loop latch
         was not visited in the call chain, then we are traversing an
         iterative path that belongs to an outer-nested loop. */
      return false;
    }
  else
    {
      struct block_ladder_rung new_step;
      
      new_step.block = candidate;
      new_step.lower_rung = lower_steps;
      
      edge_iterator ei;
      edge successor_edge;
      FOR_EACH_EDGE (successor_edge, ei, candidate->succs)
	{
	  basic_block successor = successor_edge->dest;
	  if (in_loop_of_header_p (successor, loop_header,
				   loop_latch, false, &new_step))
	    return true;
	}
      /* None of the successors was in loop */
      return false;
    }
}

/* Add CANDIDATE into the RESULTS vector if CANDIDATE
   is in the loop identified by the LOOP_HEADER and LOOP_LATCH
   arguments and it is not already contained within the RESULTS
   vector. 

   We consider the block to be within the loop if there exists a path
   within the control flow graph from this node to the loop's latch
   which does not pass through the loop's header.  (If all paths to
   the latch pass through the loop header, then the node is contained
   within an outer-nested loop but not within this loop.)
 
   If and only if CANDIDATE is added to the RESULTS vector, recursively
   do the same for each successor of the CANDIDATE block.
  
   Return the potentially modified RESULTS vector. */
static vec<basic_block> 
recursively_get_loop_blocks (basic_block candidate, vec<basic_block> results,
			     basic_block loop_header, basic_block loop_latch)
{
  basic_block bb;
  unsigned int u;

  /* if candidate is already in the results vector, then we're done */
  FOR_EACH_VEC_ELT (results, u, bb)
    {
      if (bb == candidate)
	return results;
    }

  if (in_loop_of_header_p (candidate, loop_header, loop_latch, true, NULL))
    {
      results.safe_push (candidate);

      edge_iterator ei;
      edge successor;
      FOR_EACH_EDGE (successor, ei, candidate->succs)
	{
	  if (successor->probability != 0)
	    {
	      results = recursively_get_loop_blocks (successor->dest, results, 
						     loop_header, loop_latch);
	    }
	}
    }
  return results;
}

/* Return a vector containing all of the blocks contained within the
   loop identified by LOOP_PTR. */
static vec<basic_block> 
get_loop_blocks (struct loop *loop_ptr)
{
  vec<basic_block> results;

  results = vNULL;
  results = recursively_get_loop_blocks (loop_ptr->header, results,
					 loop_ptr->header, loop_ptr->latch);
  return results;
}

/* Return true iff BB is contained within the loop represented by LOOP_PTR */
static bool 
in_block_set_p (basic_block bb, struct loop* loop_ptr)
{
  return (bb->loop_father == loop_ptr)
    || flow_loop_nested_p (loop_ptr, bb->loop_father);
}

/* Zero all frequencies for all blocks contained within the loop
   represented by LOOP_PTR which are reachable from BLOCK without
   passing through the loop header. If BLOCK is not within the loop,
   this has no effect. The behavior is as outlined by the following
   algorithm:
  
   If BLOCK is contained within loop:
     Set BLOCK's frequency to zero
     Using a depth-first traversal, do the same for each successor
     transitively, stopping the recursive traversal if:
 	the current block is the loop header, or
  	the current block resides outside the loop, or
  	the current block has already been visited in this depth-first
  	  traversal. */
static void
zero_partial_loop_frequencies (struct loop *loop_ptr, basic_block block)
{
  if (in_block_set_p (block, loop_ptr))
    {
      struct block_ladder_rung ladder_rung;
      ladder_rung.block = block;
      ladder_rung.lower_rung = NULL;
      vec<edge> exit_edges = get_loop_exit_edges (loop_ptr);
      block->frequency = 0;
      edge_iterator ei;
      edge successor;
      FOR_EACH_EDGE (successor, ei, block->succs)
	{
	  if (successor->probability != 0)
	    {
	      recursively_zero_frequency (loop_ptr, exit_edges,
					  &ladder_rung, successor);
	    }
	}
      exit_edges.release ();
    }
}

/* This recursive function visits all of the blocks contained within the
   loop represented by LOOP_PTR and reachable from INCOMING_EDGE,
   and increments the frequency field of each block by an
   appropriate scaling of FREQUENCY_INCREMENT.  The
   FREQUENCY_INCREMENT value is scaled in recursive invocations of
   this function by the probability associated with the edge
   corresponding to the particular recursive call.  The recursion
   terminates if INCOMING_EDGE is known to exit this loop, or
   if the destination of INCOMING_EDGE has already been visited
   in this recursive traversal, or if the destination of INCOMING_EDGE
   is the loop header. */
static void
recursively_increment_frequency (struct loop *loop_ptr, vec<edge> exit_edges,
				 ladder_rung_p ladder_rung, edge incoming_edge,
				 int frequency_increment)
{
  if (incoming_edge->dest == loop_ptr->header)
    return;
  else if (in_edge_set_p (incoming_edge, exit_edges))
    return;
  else if (in_call_chain_p (incoming_edge, ladder_rung))
    return;
  else
    {
      struct block_ladder_rung a_rung;
      basic_block block = incoming_edge->dest;
      
      a_rung.block = block;
      a_rung.lower_rung = ladder_rung;
      block->frequency += frequency_increment;

      edge_iterator ei;
      edge successor;
      FOR_EACH_EDGE (successor, ei, block->succs)
	{
	  int successor_increment =
	    (frequency_increment * successor->probability) / REG_BR_PROB_BASE;
	  recursively_increment_frequency (loop_ptr, exit_edges,
					   &a_rung, successor,
					   successor_increment);
	}
    }
}
 
/* If BLOCK is contained within loop LOOP_PTR, we do the following:
   Add INCREMENTAL_FREQUENCY (which may be negative) to
   BLOCK->frequency and propogate this change to all successors of
   BLOCK that reside within the loop, transitively.   The
   FREQUENCY_INCREMENT value is scaled before adding the value
   to successor nodes by the probability factor associated with
   the edges along all paths to the successor.  Use a depth-first
   tree traversal, stopping the recursion at the loop header, at any
   successor block that resides outside the loop, and at any block
   that is already part of the current depth-first traversal. */
void 
increment_loop_frequencies (struct loop *loop_ptr, basic_block block,
			    int frequency_increment)
{
  if (in_block_set_p (block, loop_ptr))
    {
      struct block_ladder_rung ladder_rung;
      ladder_rung.block = block;
      ladder_rung.lower_rung = NULL;

      vec<edge> exit_edges = get_loop_exit_edges (loop_ptr);
      block->frequency += frequency_increment;
      edge_iterator ei;
      edge successor;
      FOR_EACH_EDGE (successor, ei, block->succs)
	{
	  if (successor->probability != 0)
	    {
	      int successor_increment =
		((frequency_increment * successor->probability)
		 / REG_BR_PROB_BASE);
	      recursively_increment_frequency (loop_ptr, exit_edges,
					       &ladder_rung, successor,
					       successor_increment);
	    }
	}
      exit_edges.release ();
    }
}

/* check_loop_frequency_integrity enforces that:
    a. The sum of outgoing edge frequencies for loop LOOP_PTR equals the
       sum of incoming edge frequencies for the loop's header block.
    b. The sum of predecessor edge frequencies for every block
       in the loop equals the frequency of that block.
   Consistency of edge frequencies is enforced to within a programmed
   tolerance value.  The objective of allowing some variance from
   strict enforcement of equality is to allow for the accumulation of
   round-off errors. */
static void 
check_loop_frequency_integrity (struct loop *loop_ptr)
{
  unsigned int i, k;
  basic_block a_block;
  vec<basic_block> loop_body = get_loop_blocks (loop_ptr);
  basic_block header;

  FOR_EACH_VEC_ELT (loop_body, k, a_block)
    {
      int delta;
      int predecessor_frequencies = 0;
      edge_iterator ei;
      edge a_predecessor;

      FOR_EACH_EDGE (a_predecessor, ei, a_block->preds)
	predecessor_frequencies += EDGE_FREQUENCY (a_predecessor);
      delta = predecessor_frequencies - a_block->frequency;

      /* Enforce tolerance to within 0.2%. */
      int tolerance = predecessor_frequencies / 500;  
      if (tolerance < 10)
	tolerance = 10;
      if (delta < 0)
	delta = -delta;
      if (delta > tolerance)
	fatal_error (input_location,
		     "Inconsistent predecessor frequencies "
		     " while unrolling loop.");
    }

  header = loop_ptr->header;
  int incoming_frequency = 0;

  edge_iterator ei;
  edge a_predecessor;
  FOR_EACH_EDGE (a_predecessor, ei, header->preds)
    if (!in_block_set_p (a_predecessor->src, loop_ptr))
      incoming_frequency += EDGE_FREQUENCY (a_predecessor);

  int outgoing_frequency = 0;
  vec<edge> exit_edges = get_loop_exit_edges (loop_ptr);
  edge edge;
  FOR_EACH_VEC_ELT (exit_edges, i, edge)
    outgoing_frequency += EDGE_FREQUENCY (edge);

  /* enforce tolerance to within 0.2% */
  int tolerance = incoming_frequency / 500;
  if (tolerance < 10)
    tolerance = 10;
  int delta = incoming_frequency - outgoing_frequency;
  if (delta < 0)
    delta = -delta;
  if (delta > tolerance)
    fatal_error (input_location,
		 "Inconsistent enter/exit frequencies "
		 "while unrolling loop.");
  loop_body.release ();
  exit_edges.release ();
}
#endif

/* Checks whether basic block BB is dominated by DATA.  */
static bool
rpe_enum_p (const_basic_block bb, const void *data)
{
  return dominated_by_p (CDI_DOMINATORS, bb, (const_basic_block) data);
}

/* Remove basic blocks BBS.  NBBS is the number of the basic blocks.  */

static void
remove_bbs (basic_block *bbs, int nbbs)
{
  int i;

  for (i = 0; i < nbbs; i++)
    delete_basic_block (bbs[i]);
}

/* Find path -- i.e. the basic blocks dominated by edge E and put them
   into array BBS, that will be allocated large enough to contain them.
   E->dest must have exactly one predecessor for this to work (it is
   easy to achieve and we do not put it here because we do not want to
   alter anything by this function).  The number of basic blocks in the
   path is returned.  */
static int
find_path (edge e, basic_block **bbs)
{
  gcc_assert (EDGE_COUNT (e->dest->preds) <= 1);

  /* Find bbs in the path.  */
  *bbs = XNEWVEC (basic_block, n_basic_blocks_for_fn (cfun));
  return dfs_enumerate_from (e->dest, 0, rpe_enum_p, *bbs,
			     n_basic_blocks_for_fn (cfun), e->dest);
}

/* Fix placement of basic block BB inside loop hierarchy --
   Let L be a loop to that BB belongs.  Then every successor of BB must either
     1) belong to some superloop of loop L, or
     2) be a header of loop K such that K->outer is superloop of L
   Returns true if we had to move BB into other loop to enforce this condition,
   false if the placement of BB was already correct (provided that placements
   of its successors are correct).  */
static bool
fix_bb_placement (basic_block bb)
{
  edge e;
  edge_iterator ei;
  struct loop *loop = current_loops->tree_root, *act;

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      if (e->dest == EXIT_BLOCK_PTR_FOR_FN (cfun))
	continue;

      act = e->dest->loop_father;
      if (act->header == e->dest)
	act = loop_outer (act);

      if (flow_loop_nested_p (loop, act))
	loop = act;
    }

  if (loop == bb->loop_father)
    return false;

  remove_bb_from_loops (bb);
  add_bb_to_loop (bb, loop);

  return true;
}

/* Fix placement of LOOP inside loop tree, i.e. find the innermost superloop
   of LOOP to that leads at least one exit edge of LOOP, and set it
   as the immediate superloop of LOOP.  Return true if the immediate superloop
   of LOOP changed.

   IRRED_INVALIDATED is set to true if a change in the loop structures might
   invalidate the information about irreducible regions.  */

static bool
fix_loop_placement (struct loop *loop, bool *irred_invalidated)
{
  unsigned i;
  edge e;
  vec<edge> exits = get_loop_exit_edges (loop);
  struct loop *father = current_loops->tree_root, *act;
  bool ret = false;

  FOR_EACH_VEC_ELT (exits, i, e)
    {
      act = find_common_loop (loop, e->dest->loop_father);
      if (flow_loop_nested_p (father, act))
	father = act;
    }

  if (father != loop_outer (loop))
    {
      for (act = loop_outer (loop); act != father; act = loop_outer (act))
	act->num_nodes -= loop->num_nodes;
      flow_loop_tree_node_remove (loop);
      flow_loop_tree_node_add (father, loop);

      /* The exit edges of LOOP no longer exits its original immediate
	 superloops; remove them from the appropriate exit lists.  */
      FOR_EACH_VEC_ELT (exits, i, e)
	{
	  /* We may need to recompute irreducible loops.  */
	  if (e->flags & EDGE_IRREDUCIBLE_LOOP)
	    *irred_invalidated = true;
	  rescan_loop_exit (e, false, false);
	}

      ret = true;
    }

  exits.release ();
  return ret;
}

/* Fix placements of basic blocks inside loop hierarchy stored in loops; i.e.
   enforce condition stated in description of fix_bb_placement. We
   start from basic block FROM that had some of its successors removed, so that
   his placement no longer has to be correct, and iteratively fix placement of
   its predecessors that may change if placement of FROM changed.  Also fix
   placement of subloops of FROM->loop_father, that might also be altered due
   to this change; the condition for them is similar, except that instead of
   successors we consider edges coming out of the loops.

   If the changes may invalidate the information about irreducible regions,
   IRRED_INVALIDATED is set to true.  

   If LOOP_CLOSED_SSA_INVLIDATED is non-zero then all basic blocks with
   changed loop_father are collected there. */

static void
fix_bb_placements (basic_block from,
		   bool *irred_invalidated,
		   bitmap loop_closed_ssa_invalidated)
{
  sbitmap in_queue;
  basic_block *queue, *qtop, *qbeg, *qend;
  struct loop *base_loop, *target_loop;
  edge e;

  /* We pass through blocks back-reachable from FROM, testing whether some
     of their successors moved to outer loop.  It may be necessary to
     iterate several times, but it is finite, as we stop unless we move
     the basic block up the loop structure.  The whole story is a bit
     more complicated due to presence of subloops, those are moved using
     fix_loop_placement.  */

  base_loop = from->loop_father;
  /* If we are already in the outermost loop, the basic blocks cannot be moved
     outside of it.  If FROM is the header of the base loop, it cannot be moved
     outside of it, either.  In both cases, we can end now.  */
  if (base_loop == current_loops->tree_root
      || from == base_loop->header)
    return;

  in_queue = sbitmap_alloc (last_basic_block_for_fn (cfun));
  bitmap_clear (in_queue);
  bitmap_set_bit (in_queue, from->index);
  /* Prevent us from going out of the base_loop.  */
  bitmap_set_bit (in_queue, base_loop->header->index);

  queue = XNEWVEC (basic_block, base_loop->num_nodes + 1);
  qtop = queue + base_loop->num_nodes + 1;
  qbeg = queue;
  qend = queue + 1;
  *qbeg = from;

  while (qbeg != qend)
    {
      edge_iterator ei;
      from = *qbeg;
      qbeg++;
      if (qbeg == qtop)
	qbeg = queue;
      bitmap_clear_bit (in_queue, from->index);

      if (from->loop_father->header == from)
	{
	  /* Subloop header, maybe move the loop upward.  */
	  if (!fix_loop_placement (from->loop_father, irred_invalidated))
	    continue;
	  target_loop = loop_outer (from->loop_father);
	  if (loop_closed_ssa_invalidated)
	    {
	      basic_block *bbs = get_loop_body (from->loop_father);
	      for (unsigned i = 0; i < from->loop_father->num_nodes; ++i)
		bitmap_set_bit (loop_closed_ssa_invalidated, bbs[i]->index);
	      free (bbs);
	    }
	}
      else
	{
	  /* Ordinary basic block.  */
	  if (!fix_bb_placement (from))
	    continue;
	  target_loop = from->loop_father;
	  if (loop_closed_ssa_invalidated)
	    bitmap_set_bit (loop_closed_ssa_invalidated, from->index);
	}

      FOR_EACH_EDGE (e, ei, from->succs)
	{
	  if (e->flags & EDGE_IRREDUCIBLE_LOOP)
	    *irred_invalidated = true;
	}

      /* Something has changed, insert predecessors into queue.  */
      FOR_EACH_EDGE (e, ei, from->preds)
	{
	  basic_block pred = e->src;
	  struct loop *nca;

	  if (e->flags & EDGE_IRREDUCIBLE_LOOP)
	    *irred_invalidated = true;

	  if (bitmap_bit_p (in_queue, pred->index))
	    continue;

	  /* If it is subloop, then it either was not moved, or
	     the path up the loop tree from base_loop do not contain
	     it.  */
	  nca = find_common_loop (pred->loop_father, base_loop);
	  if (pred->loop_father != base_loop
	      && (nca == base_loop
		  || nca != pred->loop_father))
	    pred = pred->loop_father->header;
	  else if (!flow_loop_nested_p (target_loop, pred->loop_father))
	    {
	      /* If PRED is already higher in the loop hierarchy than the
		 TARGET_LOOP to that we moved FROM, the change of the position
		 of FROM does not affect the position of PRED, so there is no
		 point in processing it.  */
	      continue;
	    }

	  if (bitmap_bit_p (in_queue, pred->index))
	    continue;

	  /* Schedule the basic block.  */
	  *qend = pred;
	  qend++;
	  if (qend == qtop)
	    qend = queue;
	  bitmap_set_bit (in_queue, pred->index);
	}
    }
  free (in_queue);
  free (queue);
}

/* Removes path beginning at edge E, i.e. remove basic blocks dominated by E
   and update loop structures and dominators.  Return true if we were able
   to remove the path, false otherwise (and nothing is affected then).  */
bool
remove_path (edge e)
{
  edge ae;
  basic_block *rem_bbs, *bord_bbs, from, bb;
  vec<basic_block> dom_bbs;
  int i, nrem, n_bord_bbs;
  sbitmap seen;
  bool irred_invalidated = false;
  edge_iterator ei;
  struct loop *l, *f;

  if (!can_remove_branch_p (e))
    return false;

  /* Keep track of whether we need to update information about irreducible
     regions.  This is the case if the removed area is a part of the
     irreducible region, or if the set of basic blocks that belong to a loop
     that is inside an irreducible region is changed, or if such a loop is
     removed.  */
  if (e->flags & EDGE_IRREDUCIBLE_LOOP)
    irred_invalidated = true;

  /* We need to check whether basic blocks are dominated by the edge
     e, but we only have basic block dominators.  This is easy to
     fix -- when e->dest has exactly one predecessor, this corresponds
     to blocks dominated by e->dest, if not, split the edge.  */
  if (!single_pred_p (e->dest))
    e = single_pred_edge (split_edge (e));

  /* It may happen that by removing path we remove one or more loops
     we belong to.  In this case first unloop the loops, then proceed
     normally.   We may assume that e->dest is not a header of any loop,
     as it now has exactly one predecessor.  */
  for (l = e->src->loop_father; loop_outer (l); l = f)
    {
      f = loop_outer (l);
      if (dominated_by_p (CDI_DOMINATORS, l->latch, e->dest))
        unloop (l, &irred_invalidated, NULL);
    }

  /* Identify the path.  */
  nrem = find_path (e, &rem_bbs);

  n_bord_bbs = 0;
  bord_bbs = XNEWVEC (basic_block, n_basic_blocks_for_fn (cfun));
  seen = sbitmap_alloc (last_basic_block_for_fn (cfun));
  bitmap_clear (seen);

  /* Find "border" hexes -- i.e. those with predecessor in removed path.  */
  for (i = 0; i < nrem; i++)
    bitmap_set_bit (seen, rem_bbs[i]->index);
  if (!irred_invalidated)
    FOR_EACH_EDGE (ae, ei, e->src->succs)
      if (ae != e && ae->dest != EXIT_BLOCK_PTR_FOR_FN (cfun)
	  && !bitmap_bit_p (seen, ae->dest->index)
	  && ae->flags & EDGE_IRREDUCIBLE_LOOP)
	{
	  irred_invalidated = true;
	  break;
	}

  for (i = 0; i < nrem; i++)
    {
      bb = rem_bbs[i];
      FOR_EACH_EDGE (ae, ei, rem_bbs[i]->succs)
	if (ae->dest != EXIT_BLOCK_PTR_FOR_FN (cfun)
	    && !bitmap_bit_p (seen, ae->dest->index))
	  {
	    bitmap_set_bit (seen, ae->dest->index);
	    bord_bbs[n_bord_bbs++] = ae->dest;

	    if (ae->flags & EDGE_IRREDUCIBLE_LOOP)
	      irred_invalidated = true;
	  }
    }

  /* Remove the path.  */
  from = e->src;
  remove_branch (e);
  dom_bbs.create (0);

  /* Cancel loops contained in the path.  */
  for (i = 0; i < nrem; i++)
    if (rem_bbs[i]->loop_father->header == rem_bbs[i])
      cancel_loop_tree (rem_bbs[i]->loop_father);

  remove_bbs (rem_bbs, nrem);
  free (rem_bbs);

  /* Find blocks whose dominators may be affected.  */
  bitmap_clear (seen);
  for (i = 0; i < n_bord_bbs; i++)
    {
      basic_block ldom;

      bb = get_immediate_dominator (CDI_DOMINATORS, bord_bbs[i]);
      if (bitmap_bit_p (seen, bb->index))
	continue;
      bitmap_set_bit (seen, bb->index);

      for (ldom = first_dom_son (CDI_DOMINATORS, bb);
	   ldom;
	   ldom = next_dom_son (CDI_DOMINATORS, ldom))
	if (!dominated_by_p (CDI_DOMINATORS, from, ldom))
	  dom_bbs.safe_push (ldom);
    }

  free (seen);

  /* Recount dominators.  */
  iterate_fix_dominators (CDI_DOMINATORS, dom_bbs, true);
  dom_bbs.release ();
  free (bord_bbs);

  /* Fix placements of basic blocks inside loops and the placement of
     loops in the loop tree.  */
  fix_bb_placements (from, &irred_invalidated, NULL);
  fix_loop_placements (from->loop_father, &irred_invalidated);

  if (irred_invalidated
      && loops_state_satisfies_p (LOOPS_HAVE_MARKED_IRREDUCIBLE_REGIONS))
    mark_irreducible_loops ();

  return true;
}

/* Creates place for a new LOOP in loops structure of FN.  */

void
place_new_loop (struct function *fn, struct loop *loop)
{
  loop->num = number_of_loops (fn);
  vec_safe_push (loops_for_fn (fn)->larray, loop);
}

/* Given LOOP structure with filled header and latch, find the body of the
   corresponding loop and add it to loops tree.  Insert the LOOP as a son of
   outer.  */

void
add_loop (struct loop *loop, struct loop *outer)
{
  basic_block *bbs;
  int i, n;
  struct loop *subloop;
  edge e;
  edge_iterator ei;

  /* Add it to loop structure.  */
  place_new_loop (cfun, loop);
  flow_loop_tree_node_add (outer, loop);

  /* Find its nodes.  */
  bbs = XNEWVEC (basic_block, n_basic_blocks_for_fn (cfun));
  n = get_loop_body_with_size (loop, bbs, n_basic_blocks_for_fn (cfun));

  for (i = 0; i < n; i++)
    {
      if (bbs[i]->loop_father == outer)
	{
	  remove_bb_from_loops (bbs[i]);
	  add_bb_to_loop (bbs[i], loop);
	  continue;
	}

      loop->num_nodes++;

      /* If we find a direct subloop of OUTER, move it to LOOP.  */
      subloop = bbs[i]->loop_father;
      if (loop_outer (subloop) == outer
	  && subloop->header == bbs[i])
	{
	  flow_loop_tree_node_remove (subloop);
	  flow_loop_tree_node_add (loop, subloop);
	}
    }

  /* Update the information about loop exit edges.  */
  for (i = 0; i < n; i++)
    {
      FOR_EACH_EDGE (e, ei, bbs[i]->succs)
	{
	  rescan_loop_exit (e, false, false);
	}
    }

  free (bbs);
}

/* Multiply all frequencies in LOOP by NUM/DEN.  */

void
scale_loop_frequencies (struct loop *loop, int num, int den)
{
  basic_block *bbs;

  bbs = get_loop_body (loop);
  scale_bbs_frequencies_int (bbs, loop->num_nodes, num, den);
  free (bbs);
}

/* Multiply all frequencies in LOOP by SCALE/REG_BR_PROB_BASE.
   If ITERATION_BOUND is non-zero, scale even further if loop is predicted
   to iterate too many times.  */

void
scale_loop_profile (struct loop *loop, int scale, gcov_type iteration_bound)
{
  gcov_type iterations = expected_loop_iterations_unbounded (loop);
  edge e;
  edge_iterator ei;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, ";; Scaling loop %i with scale %f, "
	     "bounding iterations to %i from guessed %i\n",
	     loop->num, (double)scale / REG_BR_PROB_BASE,
	     (int)iteration_bound, (int)iterations);

  /* See if loop is predicted to iterate too many times.  */
  if (iteration_bound && iterations > 0
      && apply_probability (iterations, scale) > iteration_bound)
    {
      /* Fixing loop profile for different trip count is not trivial; the exit
	 probabilities has to be updated to match and frequencies propagated down
	 to the loop body.

	 We fully update only the simple case of loop with single exit that is
	 either from the latch or BB just before latch and leads from BB with
	 simple conditional jump.   This is OK for use in vectorizer.  */
      e = single_exit (loop);
      if (e)
	{
	  edge other_e;
	  int freq_delta;
	  gcov_type count_delta;

          FOR_EACH_EDGE (other_e, ei, e->src->succs)
	    if (!(other_e->flags & (EDGE_ABNORMAL | EDGE_FAKE))
		&& e != other_e)
	      break;

	  /* Probability of exit must be 1/iterations.  */
	  freq_delta = EDGE_FREQUENCY (e);
	  e->probability = REG_BR_PROB_BASE / iteration_bound;
	  other_e->probability = inverse_probability (e->probability);
	  freq_delta -= EDGE_FREQUENCY (e);

	  /* Adjust counts accordingly.  */
	  count_delta = e->count;
	  e->count = apply_probability (e->src->count, e->probability);
	  other_e->count = apply_probability (e->src->count, other_e->probability);
	  count_delta -= e->count;

	  /* If latch exists, change its frequency and count, since we changed
	     probability of exit.  Theoretically we should update everything from
	     source of exit edge to latch, but for vectorizer this is enough.  */
	  if (loop->latch
	      && loop->latch != e->src)
	    {
	      loop->latch->frequency += freq_delta;
	      if (loop->latch->frequency < 0)
		loop->latch->frequency = 0;
	      loop->latch->count += count_delta;
	      if (loop->latch->count < 0)
		loop->latch->count = 0;
	    }
	}

      /* Roughly speaking we want to reduce the loop body profile by the
	 the difference of loop iterations.  We however can do better if
	 we look at the actual profile, if it is available.  */
      scale = RDIV (iteration_bound * scale, iterations);
      if (loop->header->count)
	{
	  gcov_type count_in = 0;

	  FOR_EACH_EDGE (e, ei, loop->header->preds)
	    if (e->src != loop->latch)
	      count_in += e->count;

	  if (count_in != 0)
	    scale = GCOV_COMPUTE_SCALE (count_in * iteration_bound,
                                        loop->header->count);
	}
      else if (loop->header->frequency)
	{
	  int freq_in = 0;

	  FOR_EACH_EDGE (e, ei, loop->header->preds)
	    if (e->src != loop->latch)
	      freq_in += EDGE_FREQUENCY (e);

	  if (freq_in != 0)
	    scale = GCOV_COMPUTE_SCALE (freq_in * iteration_bound,
                                        loop->header->frequency);
	}
      if (!scale)
	scale = 1;
    }

  if (scale == REG_BR_PROB_BASE)
    return;

  /* Scale the actual probabilities.  */
  scale_loop_frequencies (loop, scale, REG_BR_PROB_BASE);
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, ";; guessed iterations are now %i\n",
	     (int)expected_loop_iterations_unbounded (loop));
}

/* Recompute dominance information for basic blocks outside LOOP.  */

static void
update_dominators_in_loop (struct loop *loop)
{
  vec<basic_block> dom_bbs = vNULL;
  sbitmap seen;
  basic_block *body;
  unsigned i;

  seen = sbitmap_alloc (last_basic_block_for_fn (cfun));
  bitmap_clear (seen);
  body = get_loop_body (loop);

  for (i = 0; i < loop->num_nodes; i++)
    bitmap_set_bit (seen, body[i]->index);

  for (i = 0; i < loop->num_nodes; i++)
    {
      basic_block ldom;

      for (ldom = first_dom_son (CDI_DOMINATORS, body[i]);
	   ldom;
	   ldom = next_dom_son (CDI_DOMINATORS, ldom))
	if (!bitmap_bit_p (seen, ldom->index))
	  {
	    bitmap_set_bit (seen, ldom->index);
	    dom_bbs.safe_push (ldom);
	  }
    }

  iterate_fix_dominators (CDI_DOMINATORS, dom_bbs, false);
  free (body);
  free (seen);
  dom_bbs.release ();
}

/* Creates an if region as shown above. CONDITION is used to create
   the test for the if.

   |
   |     -------------                 -------------
   |     |  pred_bb  |                 |  pred_bb  |
   |     -------------                 -------------
   |           |                             |
   |           |                             | ENTRY_EDGE
   |           | ENTRY_EDGE                  V
   |           |             ====>     -------------
   |           |                       |  cond_bb  |
   |           |                       | CONDITION |
   |           |                       -------------
   |           V                        /         \
   |     -------------         e_false /           \ e_true
   |     |  succ_bb  |                V             V
   |     -------------         -----------       -----------
   |                           | false_bb |      | true_bb |
   |                           -----------       -----------
   |                                   \           /
   |                                    \         /
   |                                     V       V
   |                                   -------------
   |                                   |  join_bb  |
   |                                   -------------
   |                                         | exit_edge (result)
   |                                         V
   |                                    -----------
   |                                    | succ_bb |
   |                                    -----------
   |
 */

edge
create_empty_if_region_on_edge (edge entry_edge, tree condition)
{

  basic_block cond_bb, true_bb, false_bb, join_bb;
  edge e_true, e_false, exit_edge;
  gcond *cond_stmt;
  tree simple_cond;
  gimple_stmt_iterator gsi;

  cond_bb = split_edge (entry_edge);

  /* Insert condition in cond_bb.  */
  gsi = gsi_last_bb (cond_bb);
  simple_cond =
    force_gimple_operand_gsi (&gsi, condition, true, NULL,
			      false, GSI_NEW_STMT);
  cond_stmt = gimple_build_cond_from_tree (simple_cond, NULL_TREE, NULL_TREE);
  gsi = gsi_last_bb (cond_bb);
  gsi_insert_after (&gsi, cond_stmt, GSI_NEW_STMT);

  join_bb = split_edge (single_succ_edge (cond_bb));

  e_true = single_succ_edge (cond_bb);
  true_bb = split_edge (e_true);

  e_false = make_edge (cond_bb, join_bb, 0);
  false_bb = split_edge (e_false);

  e_true->flags &= ~EDGE_FALLTHRU;
  e_true->flags |= EDGE_TRUE_VALUE;
  e_false->flags &= ~EDGE_FALLTHRU;
  e_false->flags |= EDGE_FALSE_VALUE;

  set_immediate_dominator (CDI_DOMINATORS, cond_bb, entry_edge->src);
  set_immediate_dominator (CDI_DOMINATORS, true_bb, cond_bb);
  set_immediate_dominator (CDI_DOMINATORS, false_bb, cond_bb);
  set_immediate_dominator (CDI_DOMINATORS, join_bb, cond_bb);

  exit_edge = single_succ_edge (join_bb);

  if (single_pred_p (exit_edge->dest))
    set_immediate_dominator (CDI_DOMINATORS, exit_edge->dest, join_bb);

  return exit_edge;
}

/* create_empty_loop_on_edge
   |
   |    - pred_bb -                   ------ pred_bb ------
   |   |           |                 | iv0 = initial_value |
   |    -----|-----                   ---------|-----------
   |         |                       ______    | entry_edge
   |         | entry_edge           /      |   |
   |         |             ====>   |      -V---V- loop_header -------------
   |         V                     |     | iv_before = phi (iv0, iv_after) |
   |    - succ_bb -                |      ---|-----------------------------
   |   |           |               |         |
   |    -----------                |      ---V--- loop_body ---------------
   |                               |     | iv_after = iv_before + stride   |
   |                               |     | if (iv_before < upper_bound)    |
   |                               |      ---|--------------\--------------
   |                               |         |               \ exit_e
   |                               |         V                \
   |                               |       - loop_latch -      V- succ_bb -
   |                               |      |              |     |           |
   |                               |       /-------------       -----------
   |                                \ ___ /

   Creates an empty loop as shown above, the IV_BEFORE is the SSA_NAME
   that is used before the increment of IV. IV_BEFORE should be used for
   adding code to the body that uses the IV.  OUTER is the outer loop in
   which the new loop should be inserted.

   Both INITIAL_VALUE and UPPER_BOUND expressions are gimplified and
   inserted on the loop entry edge.  This implies that this function
   should be used only when the UPPER_BOUND expression is a loop
   invariant.  */

struct loop *
create_empty_loop_on_edge (edge entry_edge,
			   tree initial_value,
			   tree stride, tree upper_bound,
			   tree iv,
			   tree *iv_before,
			   tree *iv_after,
			   struct loop *outer)
{
  basic_block loop_header, loop_latch, succ_bb, pred_bb;
  struct loop *loop;
  gimple_stmt_iterator gsi;
  gimple_seq stmts;
  gcond *cond_expr;
  tree exit_test;
  edge exit_e;
  int prob;

  gcc_assert (entry_edge && initial_value && stride && upper_bound && iv);

  /* Create header, latch and wire up the loop.  */
  pred_bb = entry_edge->src;
  loop_header = split_edge (entry_edge);
  loop_latch = split_edge (single_succ_edge (loop_header));
  succ_bb = single_succ (loop_latch);
  make_edge (loop_header, succ_bb, 0);
  redirect_edge_succ_nodup (single_succ_edge (loop_latch), loop_header);

  /* Set immediate dominator information.  */
  set_immediate_dominator (CDI_DOMINATORS, loop_header, pred_bb);
  set_immediate_dominator (CDI_DOMINATORS, loop_latch, loop_header);
  set_immediate_dominator (CDI_DOMINATORS, succ_bb, loop_header);

  /* Initialize a loop structure and put it in a loop hierarchy.  */
  loop = alloc_loop ();
  loop->header = loop_header;
  loop->latch = loop_latch;
  add_loop (loop, outer);

  /* TODO: Fix frequencies and counts.  */
  prob = REG_BR_PROB_BASE / 2;

  scale_loop_frequencies (loop, REG_BR_PROB_BASE - prob, REG_BR_PROB_BASE);

  /* Update dominators.  */
  update_dominators_in_loop (loop);

  /* Modify edge flags.  */
  exit_e = single_exit (loop);
  exit_e->flags = EDGE_LOOP_EXIT | EDGE_FALSE_VALUE;
  single_pred_edge (loop_latch)->flags = EDGE_TRUE_VALUE;

  /* Construct IV code in loop.  */
  initial_value = force_gimple_operand (initial_value, &stmts, true, iv);
  if (stmts)
    {
      gsi_insert_seq_on_edge (loop_preheader_edge (loop), stmts);
      gsi_commit_edge_inserts ();
    }

  upper_bound = force_gimple_operand (upper_bound, &stmts, true, NULL);
  if (stmts)
    {
      gsi_insert_seq_on_edge (loop_preheader_edge (loop), stmts);
      gsi_commit_edge_inserts ();
    }

  gsi = gsi_last_bb (loop_header);
  create_iv (initial_value, stride, iv, loop, &gsi, false,
	     iv_before, iv_after);

  /* Insert loop exit condition.  */
  cond_expr = gimple_build_cond
    (LT_EXPR, *iv_before, upper_bound, NULL_TREE, NULL_TREE);

  exit_test = gimple_cond_lhs (cond_expr);
  exit_test = force_gimple_operand_gsi (&gsi, exit_test, true, NULL,
					false, GSI_NEW_STMT);
  gimple_cond_set_lhs (cond_expr, exit_test);
  gsi = gsi_last_bb (exit_e->src);
  gsi_insert_after (&gsi, cond_expr, GSI_NEW_STMT);

  split_block_after_labels (loop_header);

  return loop;
}

/* Make area between HEADER_EDGE and LATCH_EDGE a loop by connecting
   latch to header and update loop tree and dominators
   accordingly. Everything between them plus LATCH_EDGE destination must
   be dominated by HEADER_EDGE destination, and back-reachable from
   LATCH_EDGE source.  HEADER_EDGE is redirected to basic block SWITCH_BB,
   FALSE_EDGE of SWITCH_BB to original destination of HEADER_EDGE and
   TRUE_EDGE of SWITCH_BB to original destination of LATCH_EDGE.
   Returns the newly created loop.  Frequencies and counts in the new loop
   are scaled by FALSE_SCALE and in the old one by TRUE_SCALE.  */

struct loop *
loopify (edge latch_edge, edge header_edge,
	 basic_block switch_bb, edge true_edge, edge false_edge,
	 bool redirect_all_edges, unsigned true_scale, unsigned false_scale)
{
  basic_block succ_bb = latch_edge->dest;
  basic_block pred_bb = header_edge->src;
  struct loop *loop = alloc_loop ();
  struct loop *outer = loop_outer (succ_bb->loop_father);
  int freq;
  gcov_type cnt;
  edge e;
  edge_iterator ei;

  loop->header = header_edge->dest;
  loop->latch = latch_edge->src;

  freq = EDGE_FREQUENCY (header_edge);
  cnt = header_edge->count;

  /* Redirect edges.  */
  loop_redirect_edge (latch_edge, loop->header);
  loop_redirect_edge (true_edge, succ_bb);

  /* During loop versioning, one of the switch_bb edge is already properly
     set. Do not redirect it again unless redirect_all_edges is true.  */
  if (redirect_all_edges)
    {
      loop_redirect_edge (header_edge, switch_bb);
      loop_redirect_edge (false_edge, loop->header);

      /* Update dominators.  */
      set_immediate_dominator (CDI_DOMINATORS, switch_bb, pred_bb);
      set_immediate_dominator (CDI_DOMINATORS, loop->header, switch_bb);
    }

  set_immediate_dominator (CDI_DOMINATORS, succ_bb, switch_bb);

  /* Compute new loop.  */
  add_loop (loop, outer);

  /* Add switch_bb to appropriate loop.  */
  if (switch_bb->loop_father)
    remove_bb_from_loops (switch_bb);
  add_bb_to_loop (switch_bb, outer);

  /* Fix frequencies.  */
  if (redirect_all_edges)
    {
      switch_bb->frequency = freq;
      switch_bb->count = cnt;
      FOR_EACH_EDGE (e, ei, switch_bb->succs)
	{
	  e->count = apply_probability (switch_bb->count, e->probability);
	}
    }
  scale_loop_frequencies (loop, false_scale, REG_BR_PROB_BASE);
  scale_loop_frequencies (succ_bb->loop_father, true_scale, REG_BR_PROB_BASE);
  update_dominators_in_loop (loop);

  return loop;
}

/* Remove the latch edge of a LOOP and update loops to indicate that
   the LOOP was removed.  After this function, original loop latch will
   have no successor, which caller is expected to fix somehow.

   If this may cause the information about irreducible regions to become
   invalid, IRRED_INVALIDATED is set to true.  

   LOOP_CLOSED_SSA_INVALIDATED, if non-NULL, is a bitmap where we store
   basic blocks that had non-trivial update on their loop_father.*/

void
unloop (struct loop *loop, bool *irred_invalidated,
	bitmap loop_closed_ssa_invalidated)
{
  basic_block *body;
  struct loop *ploop;
  unsigned i, n;
  basic_block latch = loop->latch;
  bool dummy = false;

  if (loop_preheader_edge (loop)->flags & EDGE_IRREDUCIBLE_LOOP)
    *irred_invalidated = true;

  /* This is relatively straightforward.  The dominators are unchanged, as
     loop header dominates loop latch, so the only thing we have to care of
     is the placement of loops and basic blocks inside the loop tree.  We
     move them all to the loop->outer, and then let fix_bb_placements do
     its work.  */

  body = get_loop_body (loop);
  n = loop->num_nodes;
  for (i = 0; i < n; i++)
    if (body[i]->loop_father == loop)
      {
	remove_bb_from_loops (body[i]);
	add_bb_to_loop (body[i], loop_outer (loop));
      }
  free (body);

  while (loop->inner)
    {
      ploop = loop->inner;
      flow_loop_tree_node_remove (ploop);
      flow_loop_tree_node_add (loop_outer (loop), ploop);
    }

  /* Remove the loop and free its data.  */
  delete_loop (loop);

  remove_edge (single_succ_edge (latch));

  /* We do not pass IRRED_INVALIDATED to fix_bb_placements here, as even if
     there is an irreducible region inside the cancelled loop, the flags will
     be still correct.  */
  fix_bb_placements (latch, &dummy, loop_closed_ssa_invalidated);
}

/* Fix placement of superloops of LOOP inside loop tree, i.e. ensure that
   condition stated in description of fix_loop_placement holds for them.
   It is used in case when we removed some edges coming out of LOOP, which
   may cause the right placement of LOOP inside loop tree to change.

   IRRED_INVALIDATED is set to true if a change in the loop structures might
   invalidate the information about irreducible regions.  */

static void
fix_loop_placements (struct loop *loop, bool *irred_invalidated)
{
  struct loop *outer;

  while (loop_outer (loop))
    {
      outer = loop_outer (loop);
      if (!fix_loop_placement (loop, irred_invalidated))
	break;

      /* Changing the placement of a loop in the loop tree may alter the
	 validity of condition 2) of the description of fix_bb_placement
	 for its preheader, because the successor is the header and belongs
	 to the loop.  So call fix_bb_placements to fix up the placement
	 of the preheader and (possibly) of its predecessors.  */
      fix_bb_placements (loop_preheader_edge (loop)->src,
			 irred_invalidated, NULL);
      loop = outer;
    }
}

/* Duplicate loop bounds and other information we store about
   the loop into its duplicate.  */

void
copy_loop_info (struct loop *loop, struct loop *target)
{
  gcc_checking_assert (!target->any_upper_bound && !target->any_estimate);
  target->any_upper_bound = loop->any_upper_bound;
  target->nb_iterations_upper_bound = loop->nb_iterations_upper_bound;
  target->any_estimate = loop->any_estimate;
  target->nb_iterations_estimate = loop->nb_iterations_estimate;
  target->estimate_state = loop->estimate_state;
  target->warned_aggressive_loop_optimizations
    |= loop->warned_aggressive_loop_optimizations;
}

/* Copies copy of LOOP as subloop of TARGET loop, placing newly
   created loop into loops structure.  */
struct loop *
duplicate_loop (struct loop *loop, struct loop *target)
{
  struct loop *cloop;
  cloop = alloc_loop ();
  place_new_loop (cfun, cloop);
 
  copy_loop_info (loop, cloop);

  /* Mark the new loop as copy of LOOP.  */
  set_loop_copy (loop, cloop);

  /* Add it to target.  */
  flow_loop_tree_node_add (target, cloop);

  return cloop;
}

/* Copies structure of subloops of LOOP into TARGET loop, placing
   newly created loops into loop tree.  */
void
duplicate_subloops (struct loop *loop, struct loop *target)
{
  struct loop *aloop, *cloop;

  for (aloop = loop->inner; aloop; aloop = aloop->next)
    {
      cloop = duplicate_loop (aloop, target);
      duplicate_subloops (aloop, cloop);
    }
}

/* Copies structure of subloops of N loops, stored in array COPIED_LOOPS,
   into TARGET loop, placing newly created loops into loop tree.  */
static void
copy_loops_to (struct loop **copied_loops, int n, struct loop *target)
{
  struct loop *aloop;
  int i;

  for (i = 0; i < n; i++)
    {
      aloop = duplicate_loop (copied_loops[i], target);
      duplicate_subloops (copied_loops[i], aloop);
    }
}

/* Redirects edge E to basic block DEST.  */
static void
loop_redirect_edge (edge e, basic_block dest)
{
  if (e->dest == dest)
    return;

  redirect_edge_and_branch_force (e, dest);
}

/* Check whether LOOP's body can be duplicated.  */
bool
can_duplicate_loop_p (const struct loop *loop)
{
  int ret;
  basic_block *bbs = get_loop_body (loop);

  ret = can_copy_bbs_p (bbs, loop->num_nodes);
  free (bbs);

  return ret;
}

static void
#ifdef KELVIN_PATCH
/* Sets probability and count of edge E to zero.  The probability and count
   is redistributed evenly to the remaining edges coming from E->src
   and is propagated transitively to all nodes contained within the
   loop identified by LOOP_PTR and reachable from E->src.  */
set_zero_probability (struct loop* loop_ptr, edge e)
#else
/* Sets probability and count of edge E to zero.  The probability and count
   is redistributed evenly to the remaining edges coming from E->src.  */
set_zero_probability (edge e)
#endif
{
  basic_block bb = e->src;
  edge_iterator ei;
  edge ae, last = NULL;
  unsigned n = EDGE_COUNT (bb->succs);
  gcov_type cnt = e->count, cnt1;
  unsigned prob = e->probability, prob1;
#ifdef KELVIN_PATCH
  int original_edge_frequency;
  int new_edge_frequency;
  int change_in_edge_frequency;
  bool edge_originates_in_loop = in_loop_p (bb, loop_ptr);
#endif

  gcc_assert (n > 1);
  cnt1 = cnt / (n - 1);
  prob1 = prob / (n - 1);

  FOR_EACH_EDGE (ae, ei, bb->succs)
    {
      if (ae == e)
	continue;

#ifdef KELVIN_PATCH
      if (edge_originates_in_loop)
	{
	  original_edge_frequency = EDGE_FREQUENCY (ae);
	  ae->probability += prob1;
	  ae->count += cnt1;
	  new_edge_frequency = EDGE_FREQUENCY (ae);
	  change_in_edge_frequency =
	    new_edge_frequency - original_edge_frequency;
	  increment_loop_frequencies (loop_ptr, ae->dest,
				      change_in_edge_frequency);
	}
      else
	{
	  ae->probability += prob1;
	  ae->count += cnt1;
	}
#else
      ae->probability += prob1;
      ae->count += cnt1;
#endif
      last = ae;
    }
    
  /* Move the rest to one of the edges.  */
#ifdef KELVIN_PATCH
  if (edge_originates_in_loop)
    {
      original_edge_frequency = EDGE_FREQUENCY (last);
      last->probability += prob % (n - 1);
      last->count += cnt % (n - 1);
      new_edge_frequency = EDGE_FREQUENCY (last);
      change_in_edge_frequency = new_edge_frequency - original_edge_frequency;
      if (change_in_edge_frequency != 0)
	increment_loop_frequencies (loop_ptr, last->dest,
				    change_in_edge_frequency);
    }
  else
    {
      last->probability += prob % (n - 1);
      last->count += cnt % (n - 1);
    }
#else
  last->probability += prob % (n - 1);
  last->count += cnt % (n - 1);
#endif
#ifdef KELVIN_PATCH
  if (edge_originates_in_loop)
    {
      original_edge_frequency = EDGE_FREQUENCY (e);
      e->probability = 0;
      e->count = 0;
      new_edge_frequency = EDGE_FREQUENCY (e);
      change_in_edge_frequency =
	new_edge_frequency - original_edge_frequency;
      increment_loop_frequencies (loop_ptr, e->dest,
				  change_in_edge_frequency);
    }
  else
    {
      e->probability = 0;
      e->count = 0;
    }
#else
  e->probability = 0;
  e->count = 0;
#endif
}

/* Duplicates body of LOOP to given edge E NDUPL times.  Takes care of updating
   loop structure and dominators.  E's destination must be LOOP header for
   this to work, i.e. it must be entry or latch edge of this loop; these are
   unique, as the loops must have preheaders for this function to work
   correctly (in case E is latch, the function unrolls the loop, if E is entry
   edge, it peels the loop).  Store edges created by copying ORIG edge from
   copies corresponding to set bits in WONT_EXIT bitmap (bit 0 corresponds to
   original LOOP body, the other copies are numbered in order given by control
   flow through them) into TO_REMOVE array.  Returns false if duplication is
   impossible.  */

bool
duplicate_loop_to_header_edge (struct loop *loop, edge e,
			       unsigned int ndupl, sbitmap wont_exit,
			       edge orig, vec<edge> *to_remove,
			       int flags)
{
  struct loop *target, *aloop;
  struct loop **orig_loops;
  unsigned n_orig_loops;
  basic_block header = loop->header, latch = loop->latch;
  basic_block *new_bbs, *bbs, *first_active;
  basic_block new_bb, bb, first_active_latch = NULL;
  edge ae, latch_edge;
  edge spec_edges[2], new_spec_edges[2];
#define SE_LATCH 0
#define SE_ORIG 1
  unsigned i, j, n;
  int is_latch = (latch == e->src);
  int scale_act = 0, *scale_step = NULL, scale_main = 0;
  int scale_after_exit = 0;
  int p, freq_in, freq_le, freq_out_orig;
  int prob_pass_thru, prob_pass_wont_exit, prob_pass_main;
  int add_irreducible_flag;
  basic_block place_after;
  bitmap bbs_to_scale = NULL;
  bitmap_iterator bi;

#ifdef KELVIN_PATCH
  /* Remember the initial ratio between frequency of edge into loop
     header and the frequency of the loop header. Preserve this ratio
     when we make adjustments within the loop. This distinction is
     necessary because different flavors of loops are subject to
     different heuristics.  In particular, loops bounded by run-time
     constants assume that branches exiting a loop have probability
     9%.  Loops bounded by compile-time constants assume branches
     exiting a loop have probability 1%. There may be other
     circumstances that assume different behaviors. 
   
     TODO: For loops that have a single exit, the exit ratio is the
     same as the ratio between the sum of the frequency of the
     header's incoming edges and the frequency of the header itself.
     For loops that have multiple exits, investigate.  */
  int header_frequency = header->frequency;
  int preheader_frequency = 0;
  
  /* Sum the EDGE frequencies for all predecessor edges that originate
     outside the loop. */
  edge_iterator ei;
  edge predecessor;
  FOR_EACH_EDGE (predecessor, ei, header->preds)
    if (!in_loop_p (predecessor->src, loop))
      preheader_frequency += EDGE_FREQUENCY (predecessor);
  int exit_ratio = (header_frequency * 10000 - 5000) / preheader_frequency;
#ifdef KELVIN_NOISE
  {
    fprintf(stderr,
	    "duplicate_loop_to_header_edge(ndupl: %d)\n", ndupl);
    kdn_dump_copy_flags(stderr, " flags: ", flags);
    kdn_dump_sbitmap(stderr, " wont_exit: ", wont_exit);
    fprintf(stderr, " header edge: ");
    kdn_dump_edge(stderr, e);
    fprintf(stderr, " original edge: ");
    kdn_dump_edge(stderr, orig);
    fprintf(stderr, " to_remove edges:\n");
    unsigned int i;
    edge edge_to_remove;
    FOR_EACH_VEC_ELT (*to_remove, i, edge_to_remove)
      kdn_dump_edge(stderr, edge_to_remove);
    fprintf(stderr,
	    " preheader frequency: %d, header frequency: %d, exit_ratio: %d\n",
	    preheader_frequency, header_frequency, exit_ratio);
    fprintf(stderr, "The loop context:");
    kdn_dump_loop(stderr, loop);
    kdn_dump_all_blocks(stderr, loop);
  }
#endif
#endif
  gcc_assert (e->dest == loop->header);
  gcc_assert (ndupl > 0);

  if (orig)
    {
      /* Orig must be edge out of the loop.  */
      gcc_assert (flow_bb_inside_loop_p (loop, orig->src));
      gcc_assert (!flow_bb_inside_loop_p (loop, orig->dest));
    }

  n = loop->num_nodes;
  bbs = get_loop_body_in_dom_order (loop);
  gcc_assert (bbs[0] == loop->header);
  gcc_assert (bbs[n  - 1] == loop->latch);

  /* Check whether duplication is possible.  */
  if (!can_copy_bbs_p (bbs, loop->num_nodes))
    {
      free (bbs);
      return false;
    }
  new_bbs = XNEWVEC (basic_block, loop->num_nodes);

  /* In case we are doing loop peeling and the loop is in the middle of
     irreducible region, the peeled copies will be inside it too.  */
  add_irreducible_flag = e->flags & EDGE_IRREDUCIBLE_LOOP;
  gcc_assert (!is_latch || !add_irreducible_flag);

  /* Find edge from latch.  */
  latch_edge = loop_latch_edge (loop);

  if (flags & DLTHE_FLAG_UPDATE_FREQ)
    {
      /* Calculate coefficients by that we have to scale frequencies
	 of duplicated loop bodies.  */
      freq_in = header->frequency;
      freq_le = EDGE_FREQUENCY (latch_edge);
      if (freq_in == 0)
	freq_in = 1;
      if (freq_in < freq_le)
	freq_in = freq_le;
      freq_out_orig = orig ? EDGE_FREQUENCY (orig) : freq_in - freq_le;
      if (freq_out_orig > freq_in - freq_le)
	freq_out_orig = freq_in - freq_le;
      prob_pass_thru = RDIV (REG_BR_PROB_BASE * freq_le, freq_in);
      prob_pass_wont_exit =
	      RDIV (REG_BR_PROB_BASE * (freq_le + freq_out_orig), freq_in);

      if (orig
	  && REG_BR_PROB_BASE - orig->probability != 0)
	{
	  /* The blocks that are dominated by a removed exit edge ORIG have
	     frequencies scaled by this.  */
	  scale_after_exit
              = GCOV_COMPUTE_SCALE (REG_BR_PROB_BASE,
                                    REG_BR_PROB_BASE - orig->probability);
	  bbs_to_scale = BITMAP_ALLOC (NULL);
	  for (i = 0; i < n; i++)
	    {
	      if (bbs[i] != orig->src
		  && dominated_by_p (CDI_DOMINATORS, bbs[i], orig->src))
		bitmap_set_bit (bbs_to_scale, i);
	    }
	}

      scale_step = XNEWVEC (int, ndupl);

      for (i = 1; i <= ndupl; i++)
	scale_step[i - 1] = bitmap_bit_p (wont_exit, i)
				? prob_pass_wont_exit
				: prob_pass_thru;

      /* Complete peeling is special as the probability of exit in last
	 copy becomes 1.  */
      if (flags & DLTHE_FLAG_COMPLETTE_PEEL)
	{
	  int wanted_freq = EDGE_FREQUENCY (e);

	  if (wanted_freq > freq_in)
	    wanted_freq = freq_in;

	  gcc_assert (!is_latch);
	  /* First copy has frequency of incoming edge.  Each subsequent
	     frequency should be reduced by prob_pass_wont_exit.  Caller
	     should've managed the flags so all except for original loop
	     has won't exist set.  */
	  scale_act = GCOV_COMPUTE_SCALE (wanted_freq, freq_in);
#ifdef KELVIN_NOISE
	  fprintf(stderr, "scale_act computed from GCOV_COMPUTE_SCALE: %d\n",
		  scale_act);
#endif
	  /* Now simulate the duplication adjustments and compute header
	     frequency of the last copy.  */
	  for (i = 0; i < ndupl; i++)
	    wanted_freq = combine_probabilities (wanted_freq, scale_step[i]);
	  scale_main = GCOV_COMPUTE_SCALE (wanted_freq, freq_in);
	}
      else if (is_latch)
	{
	  prob_pass_main = bitmap_bit_p (wont_exit, 0)
				? prob_pass_wont_exit
				: prob_pass_thru;
	  p = prob_pass_main;
	  scale_main = REG_BR_PROB_BASE;
	  for (i = 0; i < ndupl; i++)
	    {
	      scale_main += p;
	      p = combine_probabilities (p, scale_step[i]);
	    }
	  scale_main = GCOV_COMPUTE_SCALE (REG_BR_PROB_BASE, scale_main);
	  scale_act = combine_probabilities (scale_main, prob_pass_main);
#ifdef KELVIN_NOISE
	  fprintf(stderr,
		  "scale_act computed from combine_probabilities: %d\n",
		  scale_act);
#endif
	}
      else
	{
	  scale_main = REG_BR_PROB_BASE;
	  for (i = 0; i < ndupl; i++)
	    scale_main = combine_probabilities (scale_main, scale_step[i]);
	  scale_act = REG_BR_PROB_BASE - prob_pass_thru;
#ifdef KELVIN_NOISE
	  fprintf(stderr, "scale_act computed from prob_pass_thru: %d\n",
		  scale_act);
#endif
	}
      for (i = 0; i < ndupl; i++)
	gcc_assert (scale_step[i] >= 0 && scale_step[i] <= REG_BR_PROB_BASE);
      gcc_assert (scale_main >= 0 && scale_main <= REG_BR_PROB_BASE
		  && scale_act >= 0  && scale_act <= REG_BR_PROB_BASE);
    }

  /* Loop the new bbs will belong to.  */
  target = e->src->loop_father;

  /* Original loops.  */
  n_orig_loops = 0;
  for (aloop = loop->inner; aloop; aloop = aloop->next)
    n_orig_loops++;
  orig_loops = XNEWVEC (struct loop *, n_orig_loops);
  for (aloop = loop->inner, i = 0; aloop; aloop = aloop->next, i++)
    orig_loops[i] = aloop;

  set_loop_copy (loop, target);

  first_active = XNEWVEC (basic_block, n);
  if (is_latch)
    {
      memcpy (first_active, bbs, n * sizeof (basic_block));
      first_active_latch = latch;
    }

  spec_edges[SE_ORIG] = orig;
  spec_edges[SE_LATCH] = latch_edge;

#ifdef KELVIN_PATCH
  /* Recompute the loop body frequencies. */
  basic_block my_header = loop->header;
  int sum_incoming_frequencies = 0;

  zero_loop_frequencies (loop);
  FOR_EACH_EDGE(predecessor, ei, my_header->preds)
    {
      /* exit_ratio is computed based on remembered circumstances upon
	 entry into this function.  Note that loops bounded by a
	 compile-time constant have different exit ratio than loops
	 bounded by a run-time value. */ 
      if (!in_loop_p (predecessor->src, loop))
	sum_incoming_frequencies +=
	  (int) (EDGE_FREQUENCY (predecessor) * exit_ratio + 5000) / 10000;
    }
  increment_loop_frequencies (loop, my_header, sum_incoming_frequencies);
#endif
  place_after = e->src;
  for (j = 0; j < ndupl; j++)
    {
      /* Copy loops.  */
      copy_loops_to (orig_loops, n_orig_loops, target);

      /* Copy bbs.  */
      copy_bbs (bbs, n, new_bbs, spec_edges, 2, new_spec_edges, loop,
                place_after, true);

#ifdef KELVIN_NOISE
      for (unsigned int w = 0; w < n; w++)
	{
	  fprintf(stderr,
		  "new_bbs[%d] is basic block %d, with frequency: %d\n",
		  w, new_bbs[w]->index, new_bbs[w]->frequency);
	}
#endif

#ifdef KELVIN_PATCH
      int place_after_frequency = place_after->frequency;
      basic_block saved_place_after = place_after;
#endif
      place_after = new_spec_edges[SE_LATCH]->src;

      if (flags & DLTHE_RECORD_COPY_NUMBER)
	for (i = 0; i < n; i++)
	  {
	    gcc_assert (!new_bbs[i]->aux);
	    new_bbs[i]->aux = (void *)(size_t)(j + 1);
	  }

      /* Note whether the blocks and edges belong to an irreducible loop.  */
      if (add_irreducible_flag)
	{
	  for (i = 0; i < n; i++)
	    new_bbs[i]->flags |= BB_DUPLICATED;
	  for (i = 0; i < n; i++)
	    {
	      edge_iterator ei;
	      new_bb = new_bbs[i];
	      if (new_bb->loop_father == target)
		new_bb->flags |= BB_IRREDUCIBLE_LOOP;

	      FOR_EACH_EDGE (ae, ei, new_bb->succs)
		if ((ae->dest->flags & BB_DUPLICATED)
		    && (ae->src->loop_father == target
			|| ae->dest->loop_father == target))
		  ae->flags |= EDGE_IRREDUCIBLE_LOOP;
	    }
	  for (i = 0; i < n; i++)
	    new_bbs[i]->flags &= ~BB_DUPLICATED;
	}
      /* Redirect the special edges.  */
      if (is_latch)
	{
	  redirect_edge_and_branch_force (latch_edge, new_bbs[0]);
	  redirect_edge_and_branch_force (new_spec_edges[SE_LATCH],
					  loop->header);
	  set_immediate_dominator (CDI_DOMINATORS, new_bbs[0], latch);
	  latch = loop->latch = new_bbs[n - 1];
	  e = latch_edge = new_spec_edges[SE_LATCH];
	}
      else
	{
	  redirect_edge_and_branch_force (new_spec_edges[SE_LATCH],
					  loop->header);
	  redirect_edge_and_branch_force (e, new_bbs[0]);
	  set_immediate_dominator (CDI_DOMINATORS, new_bbs[0], e->src);
	  e = new_spec_edges[SE_LATCH];
	}

#ifdef KELVIN_PATCH
      zero_partial_loop_frequencies (loop, saved_place_after);
      increment_loop_frequencies (loop,
				  saved_place_after, place_after_frequency);
#endif
      /* Record exit edge in this copy.  */
      if (orig && bitmap_bit_p (wont_exit, j + 1))
	{
	  if (to_remove)
	    to_remove->safe_push (new_spec_edges[SE_ORIG]);
#ifdef KELVIN_PATCH
	  set_zero_probability (loop, new_spec_edges[SE_ORIG]);
#else
	  set_zero_probability (new_spec_edges[SE_ORIG]);
#endif
	  /* Scale the frequencies of the blocks dominated by the exit.  */
	  if (bbs_to_scale)
	    {
	      EXECUTE_IF_SET_IN_BITMAP (bbs_to_scale, 0, i, bi)
		{
		  scale_bbs_frequencies_int (new_bbs + i, 1, scale_after_exit,
					     REG_BR_PROB_BASE);
		}
	    }
	}

      /* Record the first copy in the control flow order if it is not
	 the original loop (i.e. in case of peeling).  */
      if (!first_active_latch)
	{
	  memcpy (first_active, new_bbs, n * sizeof (basic_block));
	  first_active_latch = new_bbs[n - 1];
	}

      /* Set counts and frequencies.  */
      if (flags & DLTHE_FLAG_UPDATE_FREQ)
	{
#ifdef KELVIN_NOISE
	  fprintf(stderr, "(flags & DLTHE_FLAG_UPDATE_FREQ): scale_act: %d\n",
		  scale_act);
#endif
	  scale_bbs_frequencies_int (new_bbs, n, scale_act, REG_BR_PROB_BASE);
	  scale_act = combine_probabilities (scale_act, scale_step[j]);

#ifdef KELVIN_NOISE
	  for (unsigned int w = 0; w < n; w++)
	    {
	      fprintf(stderr,
		      "after scaling, new_bbs[%d] has frequency: %d\n",
		      w, new_bbs[w]->frequency);
	    }
#endif
	}
    }
  free (new_bbs);
  free (orig_loops);

  /* Record the exit edge in the original loop body, and update the frequencies.  */
  if (orig && bitmap_bit_p (wont_exit, 0))
    {
      if (to_remove)
	to_remove->safe_push (orig);
      set_zero_probability (loop, orig);

      /* Scale the frequencies of the blocks dominated by the exit.  */
      if (bbs_to_scale)
	{
	  EXECUTE_IF_SET_IN_BITMAP (bbs_to_scale, 0, i, bi)
	    {
#ifdef KELVIN_NOISE
	      fprintf(stderr, " scaling bbs[%d] because in set from %d to ",
		      i, bbs[i]->frequency);
#endif
	      scale_bbs_frequencies_int (bbs + i, 1, scale_after_exit,
					 REG_BR_PROB_BASE);
#ifdef KELVIN_NOISE
	      fprintf(stderr, " %d, scale factor: %d\n",
		      bbs[i]->frequency, scale_after_exit);
#endif
	    }
	}
    }

  /* Update the original loop.  */
  if (!is_latch)
    set_immediate_dominator (CDI_DOMINATORS, e->dest, e->src);
  if (flags & DLTHE_FLAG_UPDATE_FREQ)
    {
      scale_bbs_frequencies_int (bbs, n, scale_main, REG_BR_PROB_BASE);
#ifdef KELVIN_NOISE
      fprintf(stderr, 
	      " because DLTHE_FLAG_UPDATE, blocks scaled by scale_main:  %d\n",
	      scale_main);
      for (unsigned int w = 0; w < n; w++)
	fprintf(stderr, " block[%d] with index %d, scaled to freq %d\n",
		w, bbs[w]->index, bbs[w]->frequency);
#endif
      free (scale_step);
    }

  /* Update dominators of outer blocks if affected.  */
  for (i = 0; i < n; i++)
    {
      basic_block dominated, dom_bb;
      vec<basic_block> dom_bbs;
      unsigned j;

      bb = bbs[i];
      bb->aux = 0;

      dom_bbs = get_dominated_by (CDI_DOMINATORS, bb);
      FOR_EACH_VEC_ELT (dom_bbs, j, dominated)
	{
	  if (flow_bb_inside_loop_p (loop, dominated))
	    continue;
	  dom_bb = nearest_common_dominator (
			CDI_DOMINATORS, first_active[i], first_active_latch);
	  set_immediate_dominator (CDI_DOMINATORS, dominated, dom_bb);
	}
      dom_bbs.release ();
    }
  free (first_active);

  free (bbs);
  BITMAP_FREE (bbs_to_scale);

#ifdef KELVIN_PATCH
#ifdef KELVIN_NOISE
  fprintf(stderr, "At bottom of duplicate_loop_to_header_edge()\n");
  kdn_dump_all_blocks (stderr, loop);
#endif

  /* The call to check_loop_frequency_integrity checks for consistency
     of predecessor frequencies with the frequency of the node they
     precede. */
  if (flag_checking)
    check_loop_frequency_integrity (loop);
#endif
  
  return true;
}

/* A callback for make_forwarder block, to redirect all edges except for
   MFB_KJ_EDGE to the entry part.  E is the edge for that we should decide
   whether to redirect it.  */

edge mfb_kj_edge;
bool
mfb_keep_just (edge e)
{
  return e != mfb_kj_edge;
}

/* True when a candidate preheader BLOCK has predecessors from LOOP.  */

static bool
has_preds_from_loop (basic_block block, struct loop *loop)
{
  edge e;
  edge_iterator ei;

  FOR_EACH_EDGE (e, ei, block->preds)
    if (e->src->loop_father == loop)
      return true;
  return false;
}

/* Creates a pre-header for a LOOP.  Returns newly created block.  Unless
   CP_SIMPLE_PREHEADERS is set in FLAGS, we only force LOOP to have single
   entry; otherwise we also force preheader block to have only one successor.
   When CP_FALLTHRU_PREHEADERS is set in FLAGS, we force the preheader block
   to be a fallthru predecessor to the loop header and to have only
   predecessors from outside of the loop.
   The function also updates dominators.  */

basic_block
create_preheader (struct loop *loop, int flags)
{
  edge e, fallthru;
  basic_block dummy;
  int nentry = 0;
  bool irred = false;
  bool latch_edge_was_fallthru;
  edge one_succ_pred = NULL, single_entry = NULL;
  edge_iterator ei;

  FOR_EACH_EDGE (e, ei, loop->header->preds)
    {
      if (e->src == loop->latch)
	continue;
      irred |= (e->flags & EDGE_IRREDUCIBLE_LOOP) != 0;
      nentry++;
      single_entry = e;
      if (single_succ_p (e->src))
	one_succ_pred = e;
    }
  gcc_assert (nentry);
  if (nentry == 1)
    {
      bool need_forwarder_block = false;

      /* We do not allow entry block to be the loop preheader, since we
	     cannot emit code there.  */
      if (single_entry->src == ENTRY_BLOCK_PTR_FOR_FN (cfun))
        need_forwarder_block = true;
      else
        {
          /* If we want simple preheaders, also force the preheader to have
             just a single successor.  */
          if ((flags & CP_SIMPLE_PREHEADERS)
              && !single_succ_p (single_entry->src))
            need_forwarder_block = true;
          /* If we want fallthru preheaders, also create forwarder block when
             preheader ends with a jump or has predecessors from loop.  */
          else if ((flags & CP_FALLTHRU_PREHEADERS)
                   && (JUMP_P (BB_END (single_entry->src))
                       || has_preds_from_loop (single_entry->src, loop)))
            need_forwarder_block = true;
        }
      if (! need_forwarder_block)
	return NULL;
    }

  mfb_kj_edge = loop_latch_edge (loop);
  latch_edge_was_fallthru = (mfb_kj_edge->flags & EDGE_FALLTHRU) != 0;
  fallthru = make_forwarder_block (loop->header, mfb_keep_just, NULL);
  dummy = fallthru->src;
  loop->header = fallthru->dest;

  /* Try to be clever in placing the newly created preheader.  The idea is to
     avoid breaking any "fallthruness" relationship between blocks.

     The preheader was created just before the header and all incoming edges
     to the header were redirected to the preheader, except the latch edge.
     So the only problematic case is when this latch edge was a fallthru
     edge: it is not anymore after the preheader creation so we have broken
     the fallthruness.  We're therefore going to look for a better place.  */
  if (latch_edge_was_fallthru)
    {
      if (one_succ_pred)
	e = one_succ_pred;
      else
	e = EDGE_PRED (dummy, 0);

      move_block_after (dummy, e->src);
    }

  if (irred)
    {
      dummy->flags |= BB_IRREDUCIBLE_LOOP;
      single_succ_edge (dummy)->flags |= EDGE_IRREDUCIBLE_LOOP;
    }

  if (dump_file)
    fprintf (dump_file, "Created preheader block for loop %i\n",
	     loop->num);

  if (flags & CP_FALLTHRU_PREHEADERS)
    gcc_assert ((single_succ_edge (dummy)->flags & EDGE_FALLTHRU)
                && !JUMP_P (BB_END (dummy)));

  return dummy;
}

/* Create preheaders for each loop; for meaning of FLAGS see create_preheader.  */

void
create_preheaders (int flags)
{
  struct loop *loop;

  if (!current_loops)
    return;

  FOR_EACH_LOOP (loop, 0)
    create_preheader (loop, flags);
  loops_state_set (LOOPS_HAVE_PREHEADERS);
}

/* Forces all loop latches to have only single successor.  */

void
force_single_succ_latches (void)
{
  struct loop *loop;
  edge e;

  FOR_EACH_LOOP (loop, 0)
    {
      if (loop->latch != loop->header && single_succ_p (loop->latch))
	continue;

      e = find_edge (loop->latch, loop->header);
      gcc_checking_assert (e != NULL);

      split_edge (e);
    }
  loops_state_set (LOOPS_HAVE_SIMPLE_LATCHES);
}

/* This function is called from loop_version.  It splits the entry edge
   of the loop we want to version, adds the versioning condition, and
   adjust the edges to the two versions of the loop appropriately.
   e is an incoming edge. Returns the basic block containing the
   condition.

   --- edge e ---- > [second_head]

   Split it and insert new conditional expression and adjust edges.

    --- edge e ---> [cond expr] ---> [first_head]
			|
			+---------> [second_head]

  THEN_PROB is the probability of then branch of the condition.  */

static basic_block
lv_adjust_loop_entry_edge (basic_block first_head, basic_block second_head,
			   edge e, void *cond_expr, unsigned then_prob)
{
  basic_block new_head = NULL;
  edge e1;

  gcc_assert (e->dest == second_head);

  /* Split edge 'e'. This will create a new basic block, where we can
     insert conditional expr.  */
  new_head = split_edge (e);

  lv_add_condition_to_bb (first_head, second_head, new_head,
			  cond_expr);

  /* Don't set EDGE_TRUE_VALUE in RTL mode, as it's invalid there.  */
  e = single_succ_edge (new_head);
  e1 = make_edge (new_head, first_head,
		  current_ir_type () == IR_GIMPLE ? EDGE_TRUE_VALUE : 0);
  e1->probability = then_prob;
  e->probability = REG_BR_PROB_BASE - then_prob;
  e1->count = apply_probability (e->count, e1->probability);
  e->count = apply_probability (e->count, e->probability);

  set_immediate_dominator (CDI_DOMINATORS, first_head, new_head);
  set_immediate_dominator (CDI_DOMINATORS, second_head, new_head);

  /* Adjust loop header phi nodes.  */
  lv_adjust_loop_header_phi (first_head, second_head, new_head, e1);

  return new_head;
}

/* Main entry point for Loop Versioning transformation.

   This transformation given a condition and a loop, creates
   -if (condition) { loop_copy1 } else { loop_copy2 },
   where loop_copy1 is the loop transformed in one way, and loop_copy2
   is the loop transformed in another way (or unchanged). 'condition'
   may be a run time test for things that were not resolved by static
   analysis (overlapping ranges (anti-aliasing), alignment, etc.).

   THEN_PROB is the probability of the then edge of the if.  THEN_SCALE
   is the ratio by that the frequencies in the original loop should
   be scaled.  ELSE_SCALE is the ratio by that the frequencies in the
   new loop should be scaled.

   If PLACE_AFTER is true, we place the new loop after LOOP in the
   instruction stream, otherwise it is placed before LOOP.  */

struct loop *
loop_version (struct loop *loop,
	      void *cond_expr, basic_block *condition_bb,
	      unsigned then_prob, unsigned then_scale, unsigned else_scale,
	      bool place_after)
{
  basic_block first_head, second_head;
  edge entry, latch_edge, true_edge, false_edge;
  int irred_flag;
  struct loop *nloop;
  basic_block cond_bb;

  /* Record entry and latch edges for the loop */
  entry = loop_preheader_edge (loop);
  irred_flag = entry->flags & EDGE_IRREDUCIBLE_LOOP;
  entry->flags &= ~EDGE_IRREDUCIBLE_LOOP;

  /* Note down head of loop as first_head.  */
  first_head = entry->dest;

  /* Duplicate loop.  */
  if (!cfg_hook_duplicate_loop_to_header_edge (loop, entry, 1,
					       NULL, NULL, NULL, 0))
    {
      entry->flags |= irred_flag;
      return NULL;
    }

  /* After duplication entry edge now points to new loop head block.
     Note down new head as second_head.  */
  second_head = entry->dest;

  /* Split loop entry edge and insert new block with cond expr.  */
  cond_bb =  lv_adjust_loop_entry_edge (first_head, second_head,
					entry, cond_expr, then_prob);
  if (condition_bb)
    *condition_bb = cond_bb;

  if (!cond_bb)
    {
      entry->flags |= irred_flag;
      return NULL;
    }

  latch_edge = single_succ_edge (get_bb_copy (loop->latch));

  extract_cond_bb_edges (cond_bb, &true_edge, &false_edge);
  nloop = loopify (latch_edge,
		   single_pred_edge (get_bb_copy (loop->header)),
		   cond_bb, true_edge, false_edge,
		   false /* Do not redirect all edges.  */,
		   then_scale, else_scale);

  copy_loop_info (loop, nloop);

  /* loopify redirected latch_edge. Update its PENDING_STMTS.  */
  lv_flush_pending_stmts (latch_edge);

  /* loopify redirected condition_bb's succ edge. Update its PENDING_STMTS.  */
  extract_cond_bb_edges (cond_bb, &true_edge, &false_edge);
  lv_flush_pending_stmts (false_edge);
  /* Adjust irreducible flag.  */
  if (irred_flag)
    {
      cond_bb->flags |= BB_IRREDUCIBLE_LOOP;
      loop_preheader_edge (loop)->flags |= EDGE_IRREDUCIBLE_LOOP;
      loop_preheader_edge (nloop)->flags |= EDGE_IRREDUCIBLE_LOOP;
      single_pred_edge (cond_bb)->flags |= EDGE_IRREDUCIBLE_LOOP;
    }

  if (place_after)
    {
      basic_block *bbs = get_loop_body_in_dom_order (nloop), after;
      unsigned i;

      after = loop->latch;

      for (i = 0; i < nloop->num_nodes; i++)
	{
	  move_block_after (bbs[i], after);
	  after = bbs[i];
	}
      free (bbs);
    }

  /* At this point condition_bb is loop preheader with two successors,
     first_head and second_head.   Make sure that loop preheader has only
     one successor.  */
  split_edge (loop_preheader_edge (loop));
  split_edge (loop_preheader_edge (nloop));

  return nloop;
}
