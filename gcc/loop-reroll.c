/* Loop rerolling.
   Copyright (C) 2002 Free Software Foundation, Inc.

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

/* We revert effects of unrolling in case we were unable to use it
   for any optimizations.  We detect this situation using the following
   algorithm:
 
   We detect natural loops (just detecting the headers, not actually
   building a structure from cfgloop.h).  For each innermost loop, we count
   number of blocks that dominate its latch and try to reroll it by every
   divisor of this number (from highest to lowest).  We split these
   dominators into blocks of the same size and verify that they are equal.
   If they indeed are, we redirect the edge at end of the first of them
   to the header, thus making rest of the loop unreachable.  The unreachable
   blocks are removed at the end of the pass.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "cfgloop.h"
#include "expr.h"

static bool try_reroll_loop	PARAMS ((dominance_info, edge, basic_block *,
					 int, int));
static bool unrolled_equivalent_p PARAMS ((basic_block *, edge,
					   basic_block *, edge, int));
static bool equivalent_block_p	PARAMS ((basic_block, edge, basic_block, edge));

/* The main entry point.  Rerolls all possible loops.  */
bool
reroll_loops ()
{
  dominance_info dom = calculate_dominance_info (CDI_DOMINATORS);
  int n_blocks = n_basic_blocks;
  basic_block bb, abb, *body = xmalloc (n_blocks * sizeof (basic_block));
  edge latch;
  int size, i, reroll_size, changed = false;

  FOR_EACH_BB (bb)
    {
      bb->aux = 0;
      update_forwarder_flag (bb);
    }

  FOR_EACH_BB (bb)
    {
      for (latch = bb->pred; latch; latch = latch->pred_next)
	if (dominated_by_p (dom, latch->src, bb))
	  break;
      if (!latch)
	continue;
      
      /* We only handle a simple case where the body of a loop is a simple
	 chain of basic blocks.  */
      for (size = 1, abb = latch->src;
	   abb != bb && !abb->pred->pred_next;
	   abb = abb->pred->src, size++)
	body[n_blocks - size] = abb;
      if (abb != bb
	  || size == 1)
	continue;
      body[n_blocks - size] = abb;

      if (rtl_dump_file)
	{
	  fprintf (rtl_dump_file,
		   "Attempting to reroll loop with latch %d --> %d\n",
		   latch->src->index, bb->index);
	  fprintf (rtl_dump_file, "  size %d\n", size);
	  fprintf (rtl_dump_file, "  body");
	  for (i = n_blocks - size; i < n_blocks; i++)
	    fprintf (rtl_dump_file, " %d", body[i]->index);
	  fprintf (rtl_dump_file, "\n");
	}

      for (reroll_size = 1; reroll_size <= size / 2; reroll_size++)
	{
	  if (size % reroll_size)
	    continue;

	  if (try_reroll_loop (dom, latch, body + n_blocks - size,
			       size, reroll_size))
	    {
	      changed = true;
	      break;
	    }
	}
      if (rtl_dump_file)
	{
	  if (2 * reroll_size > size)
	    fprintf (rtl_dump_file, "  failed\n");
	  fprintf (rtl_dump_file, "\n");
	}
    }

  free (body);
  free_dominance_info (dom);

  if (changed)
    delete_unreachable_blocks ();

  return changed;
}

/* Tries to reroll a single loop with given LATCH, BODY and SIZE so that
   the resulting loop has size REROLL_SIZE.  The loop is special in that
   every block in it has just one predecessor, and blocks are recorded in
   BODY in the natural order.  DOM is an object representing the dominators
   relation.  */
static bool
try_reroll_loop (dom, latch, body, size, reroll_size)
     dominance_info dom;
     edge latch;
     basic_block *body;
     int size;
     int reroll_size;
{
  int n_copies = size / reroll_size, i;
  basic_block *last_copy = body + size - reroll_size;
  edge new_latch = body[reroll_size]->pred;
  basic_block new_bb;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "  rerolled size %d (%d copies)\n",
	     reroll_size, n_copies);

  for (i = 0; i < n_copies - 1; i++)
    if (!unrolled_equivalent_p (body + i * reroll_size,
				body[(i + 1) * reroll_size]->pred,
				last_copy, latch,
				reroll_size))
      {
	if (rtl_dump_file)
	  fprintf (rtl_dump_file, "    does not match in %d-th copy\n", i);
	return false;
      }
  if (rtl_dump_file)
    fprintf (rtl_dump_file, "  succeeded, making a new latch from edge %d --> %d\n",
	     new_latch->src->index, new_latch->dest->index);

  new_bb = redirect_edge_and_branch_force (new_latch, latch->dest);
  if (new_bb)
    {
      add_to_dominance_info (dom, new_bb);
      set_immediate_dominator (dom, new_bb, new_bb->pred->src);
      new_bb->aux = 0;
      update_forwarder_flag (new_bb);
    }
  return true;
}

/* Checks whether COPY1, LATCH1 and COPY2, LATCH2 (of size REROLL_SIZE) are
   copies of the same loop body created by unrolling.  */
static bool
unrolled_equivalent_p (copy1, latch1, copy2, latch2, reroll_size)
     basic_block *copy1;
     edge latch1;
     basic_block *copy2;
     edge latch2;
     int reroll_size;
{
  int i;

  for (i = 0; i < reroll_size - 1; i++)
    if (!equivalent_block_p (copy1[i], copy1[i]->pred,
			     copy2[i], copy2[i]->pred))
      return false;
  return equivalent_block_p (copy1[i], latch1,
			     copy2[i], latch2);
}

/* Checks whether BLOCK1 and BLOCK2 are equivalent provided that we consider
   destinations of NEXT1 and NEXT2 equivalent.  */
static bool
equivalent_block_p (block1, next1, block2, next2)
     basic_block block1;
     edge next1;
     basic_block block2;
     edge next2;
{
  rtx insn1, insn2;

  /* Let's borrow some code from crossjumping.  */
  if (!outgoing_edges_match (0, block1, block2, next1, next2, false))
    return false;

  if (!flow_find_cross_jump (0, block1, block2, &insn1, &insn2))
    return false;

  return insn1 == block1->head && insn2 == block2->head;
}
