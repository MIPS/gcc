/* The tracer pass for the GNU compiler.
   Contributed by Jan Hubicka, SuSE Labs.
   Copyright (C) 2001 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

/* This pass performs the tail duplication needed for superblock formation.
   For more information see:

     Design and Analysis of Profile-Based Optimization in Compaq's
     Compilation Tools for Alpha; Journal of Instruction-Level
     Parallelism 3 (2000) 1-25

   Unlike Compaq's implementation we don't do the loop peeling as most
   probably a better job can be done by a special pass and we don't
   need to worry too much about the code size implications as the tail
   duplicates are crossjumped again if optimizations are not
   performed.  */


#include "config.h"
#include "system.h"
#include "tree.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "cfglayout.h"
#include "fibheap.h"

static int count_insns			PARAMS ((basic_block));
static bool better_p			PARAMS ((edge, edge));
static bool is_trace_head_p		PARAMS ((basic_block));
int find_trace				PARAMS ((basic_block, basic_block *));
static void tail_duplicate		PARAMS ((void));
static void layout_superblocks		PARAMS ((void));

/* Return true if BB has been seen - it is connected to some trace
   already.  */

#define seen(bb) (RBI (bb)->visited || RBI (bb)->next)

/* Return number of instructions in the block.  */

static int
count_insns (bb)
     basic_block bb;
{
  rtx insn;
  int n = 0;

  for (insn = bb->head; insn != NEXT_INSN (bb->end); insn = NEXT_INSN (insn))
    if (active_insn_p (insn))
      n++;
  return n;
}

/* Return true if E1 is more frequent than E2.  */
static bool
better_p (e1, e2)
     edge e1, e2;
{
  if (EDGE_FREQUENCY (e1) != EDGE_FREQUENCY (e2))
    return EDGE_FREQUENCY (e1) > EDGE_FREQUENCY (e2);
  if (e1->count != e2->count)
    return e1->count > e2->count;
  /* This is needed to avoid changes in the decision after
     CFG is modified.  */
  if (e1->src->index != e2->src->index)
    return e1->src->index > e2->src->index;
  return e1->dest->index > e2->dest->index;
}

/* Return most frequent successor of basic block BB.  */

static edge
find_best_successor (basic_block bb)
{
  edge e;
  edge best = NULL;

  for (e = bb->succ; e; e = e->succ_next)
    if (!best || better_p (e, best))
      best = e;
  return best;
}

/* Return most frequent predecessor of basic block BB.  */

static edge
find_best_predecessor (basic_block bb)
{
  edge e;
  edge best = NULL;

  for (e = bb->pred; e; e = e->pred_next)
    if (!best || better_p (e, best))
      best = e;
  return best;
}

static bool
is_trace_head_p (bb)
    basic_block bb;
{
  edge e = find_best_predecessor (bb);
  basic_block bb2;

  if (!e)
    return true;
  bb2 = e->src;
  if (find_best_successor (bb2) != e
      || bb2 == ENTRY_BLOCK_PTR
      || seen (bb2)
      || (e->flags & (EDGE_DFS_BACK | EDGE_COMPLEX)))
    return true;
  return false;
}

/* Find the trace using bb and record it in the TRACE array.
   Return number of basic blocks recorded.  */

int
find_trace (bb, trace)
     basic_block bb;
     basic_block *trace;
{
  int i = 0;
  edge e;

  trace [i++] = bb;
  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Trace: %i [%i]", bb->index, bb->frequency);
  /* Follow the trace in forward direction.  */
  while ((e = find_best_successor (bb)) != NULL)
    {
      bb = e->dest;
      if (find_best_predecessor (bb) != e
	  || bb == EXIT_BLOCK_PTR
	  || seen (bb)
	  || (e->flags & (EDGE_DFS_BACK | EDGE_COMPLEX)))
	break;
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ",%i [%i]", bb->index, bb->frequency);
      trace [i++] = bb;
    }
  if (rtl_dump_file)
    fprintf (rtl_dump_file, "\n");
  return i;
}

/* Look for basic blocks in frequency order, construct traces and tail duplicate
   if profitable.  */

static void
tail_duplicate ()
{
  fibnode_t *blocks = xmalloc (sizeof (fibnode_t) * n_basic_blocks);
  basic_block *trace = xmalloc (sizeof (basic_block) * n_basic_blocks);
  int ninsns = 0, nduplicated = 0;
  gcov_type weighted_insns = 0, traced_insns = 0;
  fibheap_t heap = fibheap_new ();
  int i;

  for (i = 0; i < n_basic_blocks; i++)
    blocks[i] = fibheap_insert (heap, -BASIC_BLOCK (i)->frequency,
				BASIC_BLOCK (i));

  for (i = 0; i < n_basic_blocks; i++)
    {
      int n = count_insns (BASIC_BLOCK (i));

      ninsns += n;
      weighted_insns += n * BASIC_BLOCK (i)->frequency;
    }

  for (; traced_insns * 100 < weighted_insns * 90;)
    {
      basic_block bb = fibheap_extract_min (heap);
      int n, pos;

      if (!bb)
	break;
      blocks[bb->index] = NULL;

      if (!is_trace_head_p (bb))
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "Basic block %i [%i] is not trace head.\n",
		     bb->index, bb->frequency);
	  continue;
	}

      if (seen (bb))
	abort ();
      n = find_trace (bb, trace);

      bb = trace[0];
      traced_insns += bb->frequency * count_insns (bb);
      if (blocks[bb->index])
	{
	  fibheap_delete_node (heap, blocks[bb->index]);
	  blocks[bb->index] = NULL;
	}

      for (pos = 1; pos < n; pos++)
	{
	  basic_block bb2 = trace[pos];

	  if (blocks[bb2->index])
	    {
	      fibheap_delete_node (heap, blocks[bb2->index]);
	      blocks[bb2->index] = NULL;
	    }
	  if (bb2->pred && bb2->pred->pred_next
	      && cfg_layout_can_duplicate_bb_p (bb2))
	    {
	      edge e = bb2->pred;
	      basic_block old = bb2;

	      while (e->src != bb)
		e = e->pred_next;
	      bb2 = cfg_layout_duplicate_bb (bb2, e);
	      nduplicated += count_insns (bb2);

	      /* Reconsider the original copy of block we've duplicated.
	         Removing the most common predecesor may make it to be
	         head.  */
	      blocks[old->index] = fibheap_insert (heap, -old->frequency, old);

	      /* Similary all sucessors possibly changed the most
	         common predecesor.  Reconsider them.  */
	      for (e = old->succ; e; e = e->succ_next)
		if (e->dest != EXIT_BLOCK_PTR && !seen (e->dest)
		    && e->dest != bb && !blocks[e->dest->index])
		  {
		    blocks[e->dest->index] =
		      fibheap_insert (heap, -e->dest->frequency, e->dest);
		  }
	      if (rtl_dump_file)
		fprintf (rtl_dump_file, "Duplicated %i as %i [%i]\n",
			 old->index, bb2->index, bb2->frequency);
	    }
	  RBI (bb)->next = bb2;
	  RBI (bb2)->visited = 1;
	  traced_insns += bb2->frequency * count_insns (bb2);
	  bb = bb2;
	}
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "\n");
    }
  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Duplicated %i insns (%i%%)\n", nduplicated,
	     nduplicated * 100 / ninsns);

  free (blocks);
  free (trace);
  fibheap_delete (heap);
}

/* Connect the superblocks into linear seuqence.  At the moment we attempt to keep
   the original order as much as possible, but the algorithm may be made smarter
   later if needed.  BB reordering pass should void most of the benefits of such
   change though.  */

static void
layout_superblocks ()
{
  basic_block end = BASIC_BLOCK (0);
  int i = 1;

  while (i < n_basic_blocks)
    {
      edge e, best = NULL;
      while (RBI (end)->next)
	end = RBI (end)->next;

      for (e = end->succ; e; e = e->succ_next)
	if (e->dest != EXIT_BLOCK_PTR
	    && ! RBI (e->dest)->visited
	    && (!best || EDGE_FREQUENCY (e) > EDGE_FREQUENCY (best)))
	  best = e;

      if (e)
	{
	  RBI (end)->next = e->dest;
	  RBI (e->dest)->visited = 1;
	}
      else
	for (; i < n_basic_blocks; i++)
	  {
	    basic_block bb = BASIC_BLOCK (i);

	    if (!RBI (bb)->visited)
	      {
		RBI (end)->next = bb;
		RBI (bb)->visited = 1;
		break;
	      }
	  }
    }
}

/* Main entry point to this file.  */

void
tracer ()
{
  if (n_basic_blocks <= 1)
    return;
  cfg_layout_initialize ();
  mark_dfs_back_edges ();
  if (rtl_dump_file)
    dump_flow_info (rtl_dump_file);
  tail_duplicate ();
  layout_superblocks ();
  if (rtl_dump_file)
    dump_flow_info (rtl_dump_file);
  cfg_layout_finalize ();
  /* Merge basic blocks in duplicated traces.  */
  cleanup_cfg (CLEANUP_EXPENSIVE);
}
