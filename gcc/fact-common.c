/* Local factoring (code hoisting/sinking) on SSA trees.
   Copyright (C) 2004 Free Software Foundation, Inc.

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "resource.h"
#include "ggc.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "langhooks.h"
#include "tree-pass.h"
#include "tree-ssa-operands.h"
#include "basic-block.h"
#include "fact-common.h"

static hashval_t block_hash (const void *);
static int block_equal (const void *, const void *);
static bool compare_blocks (bb_decorator, bb_decorator, enum lfact_direction);

#define DIR_EDGES(bb, alg) (alg == LFD_HOISTING ? bb->preds : bb->succs)
#define DIR_BLOCKS(e, alg) (alg == LFD_HOISTING ? e->src : e->dest)
#define INVDIR_EDGES(bb, alg) (alg != LFD_HOISTING ? bb->preds : bb->succs)
#define INVDIR_BLOCKS(e, alg) (alg != LFD_HOISTING ? e->src : e->dest)

/* Auxiliary function for hashing basic blocks.  */
static hashval_t
block_hash (const void *item)
{
  basic_block bb = (basic_block) item;
  return (iterative_hash_object (bb, 0));
}

/* Auxiliary function for htab_create function.  */
static int
block_equal (const void *p1, const void *p2)
{
  basic_block bb1 = (basic_block) p1;
  basic_block bb2 = (basic_block) p2;

  if (bb1->index != bb2->index)
    return 0;

  gcc_assert (block_hash (p1) == block_hash (p2));

  return 1;
}

/* Compare the set of pred/succ blocks of DECORATOR1->CURR and
   DECORATOR1->CURR. If it isn't different return true.  */
static bool
compare_blocks (bb_decorator decorator1, bb_decorator decorator2,
		enum lfact_direction alg)
{
  VEC (edge, gc) * ev;
  edge e;
  edge_iterator ei;
  basic_block bb1 = decorator1->curr;
  basic_block bb2 = decorator2->curr;
  htab_t bb_table = htab_create (100, block_hash, block_equal, NULL);
  void **slot;

  if ((alg == LFD_HOISTING
       && decorator1->num_pred_bb != decorator2->num_pred_bb)
      || (alg != LFD_HOISTING
	  && decorator1->num_succ_bb != decorator2->num_succ_bb))
    return false;

  ev = DIR_EDGES (bb1, alg);
  FOR_EACH_EDGE (e, ei, ev)
  {
    slot = htab_find_slot (bb_table, DIR_BLOCKS (e, alg), INSERT);
    *slot = DIR_BLOCKS (e, alg);
  }

  ev = DIR_EDGES (bb2, alg);
  FOR_EACH_EDGE (e, ei, ev) if (!htab_find (bb_table, DIR_BLOCKS (e, alg)))
    {
      htab_delete (bb_table);
      return false;
    }

  htab_delete (bb_table);
  return true;
}

/* Collection of the blocks, which have same pred/succ set.  */
void
collect_sibling (bb_decorator decorator, enum lfact_direction alg)
{
  bb_decorator first, second;

  /* Clean up.  */
  for (first = decorator; first; first = first->next_decorator)
    {
      first->next_sibling = NULL;
      first->prev_sibling = NULL;
    }

  /* Collecting.  */
  for (first = decorator; first; first = first->next_decorator)
    {
      for (second = first->next_decorator; second;
	   second = second->next_decorator)
	{
	  if (compare_blocks (first, second, alg))
	    {
	      first->next_sibling = second;
	      second->prev_sibling = first;
	      break;
	    }
	}
    }
}

/* Deletion of the sibling relations from FIRST.  */
void
delete_siblings (bb_decorator first)
{
  bb_decorator last = NULL;
  bb_decorator temp;
  for (temp = first; temp; temp = temp->next_sibling)
    {
      if (last)
	last->next_sibling = NULL;
      last = temp;
      temp->prev_sibling = NULL;
    }
}

/* Free allocated memory.  */
void
free_bb_decorator_list (bb_decorator decorator)
{
  bb_decorator temp;
  for (temp = decorator; temp;)
    {
      bb_decorator last = temp;
      temp = temp->next_decorator;
      free (last);
    }
}

/* Selection of the siblings.  */
void
collect_full_sibling (bb_decorator decorator, enum lfact_direction alg)
{
  bb_decorator temp;
  collect_sibling (decorator, alg);

  for (temp = decorator; temp; temp = temp->next_decorator)
    {
      if (!temp->prev_sibling && temp->next_sibling)
	{
	  unsigned long num_of_sibling = 0;
	  bb_decorator temp2;
	  VEC (edge, gc) * ev;
	  VEC (edge, gc) * fv;
	  edge f;
	  edge_iterator ei;

	  for (temp2 = temp; temp2; temp2 = temp2->next_sibling)
	    num_of_sibling++;

	  ev = DIR_EDGES (temp->curr, alg);
	  FOR_EACH_EDGE (f, ei, ev)
	  {
	    fv = INVDIR_EDGES (DIR_BLOCKS (f, alg), alg);
	    if (EDGE_COUNT (fv) != num_of_sibling)
	      {
		delete_siblings (temp);
		break;
	      }
	  }
	}
    }
}

void
collect_family_sibling (bb_decorator decorator, enum lfact_direction alg)
{
  bb_decorator temp, temp2;
  htab_t bb_table = htab_create (100, block_hash, block_equal, NULL);
  void **slot;

  collect_sibling (decorator, alg);

  for (temp = decorator; temp; temp = temp->next_decorator)
    {
      if (!temp->prev_sibling && temp->next_sibling)
	{
	  htab_empty (bb_table);
	  for (temp2 = temp; temp2; temp2 = temp2->next_sibling)
	    {
	      slot = htab_find_slot (bb_table, temp2->curr, INSERT);
	      *slot = temp2->curr;
	    }

	  for (temp2 = temp; temp2; temp2 = temp2->next_sibling)
	    {
	      VEC (edge, gc) * ev;
	      edge e;
	      edge_iterator ei;
	      int bad_siblingset = 0;

	      ev = DIR_EDGES (temp2->curr, alg);
	      FOR_EACH_EDGE (e, ei, ev)
	      {
		edge e2;
		edge_iterator ei2;
		VEC (edge, gc) * ev2;

		ev2 = INVDIR_EDGES (DIR_BLOCKS (e, alg), alg);
		FOR_EACH_EDGE (e2, ei2, ev2)
		  if (!htab_find (bb_table, INVDIR_BLOCKS (e2, alg)))
		  {
		    bad_siblingset = 1;
		    break;
		  }
		if (bad_siblingset)
		  break;
	      }

	      if (bad_siblingset)
		{
		  delete_siblings (temp);
		  break;
		}
	    }
	}
    }
}

/* Selection of the siblings by cost info.  */
void
cost_analyzer_1 (bb_decorator temp, enum lfact_direction alg)
{
  unsigned long num_of_sibling = 0;
  unsigned long num_of_edge = 0;
  bb_decorator temp2;

  for (temp2 = temp; temp2; temp2 = temp2->next_sibling)
    num_of_sibling++;

  num_of_edge = EDGE_COUNT (DIR_EDGES (temp->curr, alg));

  if (num_of_edge >= num_of_sibling)
    delete_siblings (temp);
}

/* Selection of the siblings by cost info.  */
void
cost_analyzer (bb_decorator decorator, enum lfact_direction alg)
{
  bb_decorator temp;
  for (temp = decorator; temp; temp = temp->next_decorator)
    {
      if ((!temp->prev_sibling) && temp->next_sibling)
	cost_analyzer_1 (temp, alg);
    }
}

/* Initialization of the chain of bb_decorator! For each BB there is a
   bb_decorator in the chain.  */
bb_decorator
init_factoring (bb_decorator decorator)
{
  basic_block bb;
  int out_of_mem = 0;

  bb_decorator last = NULL;
  FOR_EACH_BB (bb)
  {
    bb_decorator temp;

    temp = xcalloc (1, sizeof (struct bb_decorator_def));
    if (!temp)
      {
	out_of_mem = 1;
	break;
      }

    temp->curr = bb;
    temp->num_pred_bb = EDGE_COUNT (bb->preds);
    temp->num_succ_bb = EDGE_COUNT (bb->succs);
    if (!decorator)
      decorator = temp;
    else
      {
	last->next_decorator = temp;
	temp->prev_decorator = last;
      }
    last = temp;
  }
  if (out_of_mem)
    {
      free_bb_decorator_list (decorator);
      return NULL;
    }

  return decorator;
}

void
dump_cfg_info (FILE * fp)
{
  basic_block bb;

  fprintf (fp, "CFG:\n");
  FOR_EACH_BB (bb)
  {
    edge e;
    edge_iterator ei;

    fprintf (fp, "%d:\n", bb->index);

    fprintf (fp, "  PREDS (num: %d): ", EDGE_COUNT (bb->preds));
    FOR_EACH_EDGE (e, ei, bb->preds) fprintf (fp, "%d, ", e->src->index);

    fprintf (fp, "\n  SUCCS (num: %d): ", EDGE_COUNT (bb->succs));
    FOR_EACH_EDGE (e, ei, bb->succs) fprintf (fp, "%d, ", e->dest->index);
    fprintf (fp, "\n");
  }
  fprintf (fp, "\n");
}

/* Dump function.  */
void
dump_siblings (FILE * fp, bb_decorator decorator, enum lfact_direction alg)
{
  bb_decorator temp;
  for (temp = decorator; temp; temp = temp->next_decorator)
    {
      if ((!temp->prev_sibling) && temp->next_sibling)
	{
	  bb_decorator temp2;
	  fprintf (fp,
		   "The next basic blocks may contain statements, which are %s. \n",
		   alg == LFD_HOISTING ? "hoistable" : "sinkable");
	  for (temp2 = temp; temp2; temp2 = temp2->next_sibling)
	    fprintf (fp, "%d ", temp2->curr->index);
	  fprintf (fp, "\n");
	}
    }
  fprintf (fp, "----------------\n\n\n");
}
