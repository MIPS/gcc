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

static int cmpblocks			PARAMS ((const void *a, const void *b));
static void layout_traces		PARAMS ((void));
static void construct_traces		PARAMS ((void));
static int count_insns			PARAMS ((basic_block));

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

/* Return true if BB has been seen - it is connected to some trace
   already.  */

#define seen(bb) (RBI (bb)->visited || RBI (bb)->next)

/* Compare blocks according to their frequency.  */

static int
cmpblocks (const void *a, const void *b)
{
  basic_block b1 = *(basic_block *) a;
  basic_block b2 = *(basic_block *) b;
  if (b1->frequency != b2->frequency)
    return b2->frequency - b1->frequency;
  return b1->count - b2->count;
}

/* Return most frequent successor of basic block BB.  */

static edge
find_best_successor (basic_block bb)
{
  edge e;
  edge best = NULL;

  for (e = bb->succ; e; e = e->succ_next)
    if (!best || best->dest->frequency < e->dest->frequency
	|| best->dest->count < e->dest->count)
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
    if (!best || best->src->frequency < e->src->frequency
	|| best->src->count < e->dest->count)
      best = e;
  return best;
}

/* Look for basic blocks in frequency order, construct traces and tail duplicate
   if profitable.  */

static void
construct_traces ()
{
  basic_block *blocks = xmalloc (sizeof (basic_block) * n_basic_blocks);
  int i;
  int numblocks = n_basic_blocks;
  int ninsns = 0, nduplicated = 0;

  for (i = 0; i < numblocks; i++)
    blocks [i] = BASIC_BLOCK (i);

  for (i = 0; i < n_basic_blocks; i++)
    ninsns += count_insns (BASIC_BLOCK (i));
 
  qsort (blocks, numblocks, sizeof (basic_block), cmpblocks);

  for (i = 0; i < numblocks; i++)
    {
      basic_block seed = blocks [i];

      while (!seen (seed))
	{
	  edge e;
	  bool found = false;
	  basic_block bb = seed;

	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "Trace seed %i", bb->index);

	  /* Look backward for the start of trace.  */
	  while ((e = find_best_predecessor (bb)) != NULL)
	    {
	      basic_block bb2 = e->src;
	      if (find_best_successor (bb2) != e
		  || bb2 == ENTRY_BLOCK_PTR
		  || seen (bb2)
		  || (e->flags & (EDGE_DFS_BACK | EDGE_COMPLEX)))
		break;
	      if (rtl_dump_file)
		fprintf (rtl_dump_file, " %i", bb2->index);
	      bb = bb2;
	    }
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "\n");

	  /* Follow the trace doing duplication during process if needed.
	     ??? This may trace terminate earlier than the loop above
	     expects.  The Hwu algorithm first finds whole trace and then
	     duplicates it, but this looks like more fair heuristics
	     (definitly resulting in fewer duplicates).  */
	  while ((e = find_best_successor (bb)) != NULL)
	    {
	      basic_block bb2 = e->dest;
	      if (find_best_predecessor (bb2) != e
		  || bb2 == EXIT_BLOCK_PTR
		  || seen (bb2)
		  || (e->flags & (EDGE_DFS_BACK | EDGE_COMPLEX)))
		break;
	      if (rtl_dump_file)
		fprintf (rtl_dump_file, "   %i", bb2->index);
	      if (bb2->pred && bb2->pred->pred_next
		  && cfg_layout_can_duplicate_bb_p (bb2))
		{
		  if (rtl_dump_file)
		    fprintf (rtl_dump_file, " (duplicated)\n");
		  bb2 = cfg_layout_duplicate_bb (bb2, e);
		}
	      else if (rtl_dump_file)
		fprintf (rtl_dump_file, "\n");
	      found = true;
	      RBI (bb)->next = bb2;
	      RBI (bb2)->visited = 1;
	      bb = bb2;
	      nduplicated += count_insns (bb2);
	    }
	  if (!found)
	    break;
	}
    }
  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Duplicated %i insns (%i%%)\n", nduplicated,
	     nduplicated * 100 / ninsns);

  free (blocks);
}

/* Connect the traces into linear seuqence.  At the moment we attempt to keep
   the original order as much as possible, but the algorithm may be made smarter
   later if needed.  BB reordering pass should void most of the benefits of such
   change though.  */

static void
layout_traces ()
{
  basic_block end = BASIC_BLOCK (0);
  int i;

  for (i = 1; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);

      if (!RBI (bb)->visited)
	{
	  while (RBI (end)->next)
	    end = RBI (end)->next;
	  RBI (end)->next = bb;
	  RBI (bb)->visited = 1;
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
  construct_traces ();
  layout_traces ();
  if (rtl_dump_file)
    dump_flow_info (rtl_dump_file);
  cfg_layout_finalize ();
  /* Merge basic blocks in duplicated traces.  */
  cleanup_cfg (CLEANUP_EXPENSIVE);
}
