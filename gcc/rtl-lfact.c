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
#include "flags.h"
#include "fact-common.h"
#include "df.h"

static int gain;

/* Local factoring functions in rtl - hoisting.  */
static void insert_insn_end_of_basic_block (rtx, basic_block);
static int rtl_hoisting (int, bb_decorator);
static rtx find_first_from_begin (bb_decorator decorator, int);
static int moveable_from_begin (rtx);
static int find_insn_from_begin (rtx, bb_decorator, int);

/* Local factoring functions in rtl - sinking.  */
static void insert_insn_begin_of_basic_block (rtx, basic_block);
static int rtl_sinking (int, bb_decorator);
static rtx find_first_from_end (bb_decorator, int);
static int find_insn_from_end (rtx, bb_decorator, int);
static int moveable_from_end (rtx);
static void modifying_concatenation (bb_decorator, bb_decorator);
static void modify_jump_labels (basic_block, bb_decorator, rtx);
static int find_abnormal_edge (bb_decorator);
static basic_block create_basic_block_by_factoring (bb_decorator);
static bb_decorator insert_new_bb_decorator (bb_decorator, basic_block);

/* Common functions.  */
static void insn_dependent_p_1 (rtx, rtx, void *);
static bool insn_dependent_p (rtx, rtx);
static void delete_analogous (bb_decorator);

/* Return true if anything in insn X is (anti,output,true) dependent on
   anything in insn Y.  */
bool
insn_dependent_p (rtx x, rtx y)
{
  rtx tmp;

  if (!INSN_P (x) || !INSN_P (y))
    abort ();

  tmp = PATTERN (y);
  note_stores (PATTERN (x), insn_dependent_p_1, &tmp);
  if (tmp == NULL_RTX)
    return true;

  tmp = PATTERN (x);
  note_stores (PATTERN (y), insn_dependent_p_1, &tmp);
  if (tmp == NULL_RTX)
    return true;

  return false;
}

/* Auxiliary routine for insn_dependent_p called through note_stores.  */
static void
insn_dependent_p_1 (rtx x, rtx pat ATTRIBUTE_UNUSED, void *data)
{
  rtx *pinsn = (rtx *) data;

  if (*pinsn && reg_mentioned_p (x, *pinsn))
    *pinsn = NULL_RTX;
}

/* Deletion of the 'analogous' relations from FIRST.  */
void
delete_analogous (bb_decorator first)
{
  bb_decorator last = NULL;
  bb_decorator di;
  for (di = first; di; di = di->next_analogous)
    {
      if (last)
	last->next_analogous = NULL;
      last = di;
      di->prev_analogous = NULL;
    }
}

/* Insert insn into bb. If bb's last instruction is jump, insn will be before
   the jump instruction.  */
static void
insert_insn_end_of_basic_block (rtx insn, basic_block bb)
{
  rtx last = BB_END (bb);

  if (JUMP_P (last))
    {
      if (reg_set_p (cc0_rtx, PREV_INSN (last)))
	add_insn_before (insn, PREV_INSN (last));
      else
	add_insn_before (insn, last);
    }
  else
    add_insn_after (insn, last);
}

/* Local factoring 'up' function: it returns with 1, if the insn is moveable 
   from the block. */
static int
moveable_from_begin (rtx insn)
{
  basic_block bb = BLOCK_FOR_INSN (insn);
  rtx head = BB_HEAD (bb);
  rtx prev;

  if (insn == head)
    return 1;

  prev = PREV_INSN (insn);

  while (prev)
    {
      if (INSN_P (prev))
	if (insn_dependent_p (insn, prev))
	  return 0;
      if (prev == head)
	prev = NULL;
      else
	prev = PREV_INSN (prev);
    }
  return 1;
}

/* Finding of the first not examined insn in the block, which would be good to
   move into the pred bb. If we find a jump or call insn, this function return
   with NULL_RTX, and there is no more investigation in this block to try
   moving an other insn.  */
static rtx
find_first_from_begin (bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  rtx insn;
  int depth = 0;
  decorator->moveable_insn = NULL_RTX;
  for (insn = BB_HEAD (bb); insn != BB_END (bb); insn = NEXT_INSN (insn))
    {
      if (!INSN_P (insn))
	continue;
      if (depth < decorator->last_depth)
	{
	  depth++;
	  continue;
	}

      if (GET_CODE (insn) == CALL_INSN || volatile_refs_p (PATTERN (insn)))
	return NULL_RTX;
      if (reg_set_p (cc0_rtx, insn))
	{
	  depth++;
	  decorator->last_depth = depth;
	  continue;
	}
      if (moveable_from_begin (insn))
	{
	  decorator->moveable_insn = insn;
	  return (insn);
	}
      depth++;
      decorator->last_depth = depth;


      if (depth == max_depth - 1)
	break;
    }
  return NULL_RTX;
}

/* Finding of an instruction in the basic block decorator->curr, which is equal
   with 'insn' and which is outsourcable from the bb. Direction 'up'.  */
static int
find_insn_from_begin (rtx insn, bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  rtx tmp;
  int depth = 0;

  for (tmp = BB_HEAD (bb); tmp != BB_END (bb); tmp = NEXT_INSN (tmp))
    {
      if (!INSN_P (tmp))
	continue;
      if (rtx_equal_p (PATTERN (tmp), PATTERN (insn)))
	{
	  decorator->moveable_insn = tmp;
	  return 1;
	}
      depth++;
      if (depth == max_depth)
	return 0;

      if (insn_dependent_p (insn, tmp))
	return 0;
    }
  return 0;
}

/* Main function of local factoring hoisting.  */
static int
rtl_hoisting (int depth, bb_decorator decorator)
{
  bb_decorator di = decorator;
  int change = 0;

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
	  bb_decorator di2;
	  rtx insn = NULL;
	  int not_hoistable = 0;
	  edge e;
	  edge_iterator ei;
	  insn = find_first_from_begin (di, depth);

	  if (!insn)
	    {
	      di = di->next_decorator;
	      continue;
	    }

	  /* Finding equal instruction in the concatenated basic blocks.  */
	  for (di2 = di->next_sibling; di2; di2 = di2->next_sibling)
	    if (!find_insn_from_begin (insn, di2, depth))
	      {
		not_hoistable = 1;
		di->last_depth++;
		break;
	      }

	  if (not_hoistable)
	    continue;

	  /* Verifying the insn if it is insertable after the end of pred
	     functions.  */ 
	    FOR_EACH_EDGE (e, ei, (di->curr)->preds)
	  {
	    if (!JUMP_P (BB_END (e->src)))
	      continue;
	    if (insn_dependent_p (insn, BB_END (e->src))
		|| (reg_set_p (cc0_rtx, PREV_INSN (BB_END (e->src)))
		    && insn_dependent_p (insn, PREV_INSN (BB_END (e->src)))))
	      {
		not_hoistable = 1;
		break;
	      }
	  }

	  if (not_hoistable)
	    {
	      di->last_depth++;
	      continue;
	    }

	  if (!not_hoistable)
	    {
	      /* Moving the insn.  */
	      edge e;
	      edge_iterator ei;

	      FOR_EACH_EDGE (e, ei, (di->curr)->preds)
	      {
		rtx new_insn = make_insn_raw (PATTERN (di->moveable_insn));
		insert_insn_end_of_basic_block (new_insn, e->src);
		gain--;

	      }

	      for (di2 = di; di2; di2 = di2->next_sibling)
		{
		  rtx rinsn = di2->moveable_insn;
		  di2->moveable_insn = NULL;
		  remove_insn (rinsn);
		  gain++;
		}
	      change = 1;
	    }
	}
      else
	di = di->next_decorator;
    }
  return change;
}

/* Insert insn into bb to the first possible place.  */
static void
insert_insn_begin_of_basic_block (rtx insn, basic_block bb)
{
  rtx first = BB_HEAD (bb);
  while (!INSN_P (first))
    first = NEXT_INSN (first);

  add_insn_before (insn, first);
}

/* Local factoring 'down' function: it returns with 1, if the insn is moveable
   from the block.  */
static int
moveable_from_end (rtx insn)
{
  basic_block bb = BLOCK_FOR_INSN (insn);
  rtx end = BB_END (bb);
  rtx next;

  if (insn == end)
    return 1;

  next = NEXT_INSN (insn);

  while (next)
    {
      if (INSN_P (next) && (insn_dependent_p (next, insn)))
	return 0;
      if (next == end)
	next = NULL;
      else
	next = NEXT_INSN (next);
    }
  return 1;
}

/* Like find_first_from_begin in case of sinking.  */
static rtx
find_first_from_end (bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  rtx insn;
  int depth = 0;
  decorator->moveable_insn = NULL_RTX;
  for (insn = BB_END (bb); insn != BB_HEAD (bb); insn = PREV_INSN (insn))
    {
      if (!INSN_P (insn) || JUMP_P (insn))
	continue;

      if (depth < decorator->last_depth)
	{
	  depth++;
	  continue;
	}

      if ((GET_CODE (insn) == CALL_INSN) || volatile_refs_p (PATTERN (insn)))
	return NULL_RTX;

      if (moveable_from_end (insn))
	{
	  decorator->moveable_insn = insn;
	  return (insn);
	}
      depth++;
      decorator->last_depth++;
      if (depth == max_depth)
	break;
    }
  return NULL_RTX;
}

/* Like find_insn_from_begin just in the case of direction 'down'.  */
static int
find_insn_from_end (rtx insn, bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  rtx tmp;
  int depth = 0;
  for (tmp = BB_END (bb); tmp != BB_HEAD (bb); tmp = PREV_INSN (tmp))
    {
      if (!INSN_P (tmp) || JUMP_P (tmp))
	continue;
      if (GET_CODE (tmp) == CALL_INSN)
	return 0;
      if (rtx_equal_p (PATTERN (tmp), PATTERN (insn)))
	{
	  decorator->moveable_insn = tmp;
	  return (1);
	}
      depth++;
      if (depth == max_depth)
	return 0;
      if (insn_dependent_p (insn, tmp))
	return 0;
    }
  return 0;
}

static void
sinking_rtl_stmt (bb_decorator first) 
{
  edge e;
  edge_iterator ei;
  bb_decorator di;
  
  FOR_EACH_EDGE (e, ei, (first->curr)->succs)
  {
    rtx new_insn = make_insn_raw (PATTERN (first->moveable_insn));
    insert_insn_begin_of_basic_block (new_insn, e->dest);
    gain--;
  }

  for (di = first; di; di = di->next_analogous)
    {
      rtx rinsn = di->moveable_insn;
      di->moveable_insn = NULL;
      remove_insn (rinsn);
      gain++;
    }

  delete_analogous (first);
}

/* If we insert a new basic block, the relations between the basic blocks will
   be changed, so we must modifying some concatenation.  */
static void
modifying_concatenation (bb_decorator first, bb_decorator new_bb)
{
  bb_decorator last_sibling = first->prev_sibling;
  bb_decorator first_sibling = NULL;
  bb_decorator last_analogous = first;
  bb_decorator di;

  if (first->prev_sibling)
    {
      (first->prev_sibling)->next_sibling = NULL;
      first->prev_sibling = NULL;
    }
  for (di = first->next_sibling; di; di = di->next_sibling)
    {
      if (di->prev_sibling == last_analogous)
	{
	  if (last_analogous->next_analogous != di)
	    {
	      di->prev_sibling->next_sibling = NULL;
	      di->prev_sibling = last_sibling;
	      if (last_sibling)
		{
		  if (last_sibling->next_sibling)
		    last_sibling->next_sibling->prev_sibling = NULL;
		  last_sibling->next_sibling = di;
		}
	      last_sibling = di;
	      if (!first_sibling)
		first_sibling = di;
	    }
	  else
	    last_analogous = di;
	}
      else if (di->prev_sibling == last_sibling)
	{
	  if (last_analogous->next_analogous != di)
	    {
	      last_sibling = di;
	      if (!first_sibling)
		first_sibling = di;
	    }
	  else
	    last_analogous = di;
	}
    }
  if (last_sibling)
    {
      last_sibling->next_sibling = new_bb;
      new_bb->prev_sibling = last_sibling;
    }

  delete_siblings (first);
  for (di = first; di; di = di->next_analogous)
    {
      di->prev_sibling = di->prev_analogous;
      di->next_sibling = di->next_analogous;
    }
  delete_analogous (first);
}

/* Modification of the edges and jump labels of basic blocks, if it's necessary
   after the insertion of the new basic block.  */
static void
modify_jump_labels (basic_block new_bb, bb_decorator first, rtx label)
{
  bb_decorator di;
  for (di = first; di; di = di->next_analogous)
    {
      rtx jump = BB_END (di->curr);
      if (JUMP_P (jump))
	{
	  redirect_jump (jump, label, 0);
	  redirect_edge_succ_nodup (EDGE_SUCC (di->curr, 0), new_bb);
	}
      else
	redirect_edge_succ_nodup (EDGE_SUCC (di->curr, 0), new_bb);
    }
}


/* It is not safe to modify the destination of edge of a bb, if this edge
   is ABNORMAL.  */
static int
find_abnormal_edge (bb_decorator first) 
{
  int num_of_not_abnormal_succ = 0;
  bb_decorator di;
  for (di = first; di; di = di->next_analogous)
    {
      if (EDGE_SUCC (di->curr, 0)->flags & EDGE_ABNORMAL)
	{
	  if (di->prev_analogous)
	    di->prev_analogous->next_analogous = di->next_analogous;
	  if (di->next_analogous)
	    di->next_analogous->prev_analogous = di->prev_analogous;
	  di->prev_analogous = di->next_analogous = NULL;
	}
      else
	num_of_not_abnormal_succ++;
    }
  return (num_of_not_abnormal_succ > 1 ? 1 : 0);
}

/* Creation of new basic block.  */
static basic_block
create_basic_block_by_factoring (bb_decorator first)
{
  bb_decorator di;
  basic_block next_bb = EDGE_SUCC (first->curr, 0)->dest;
  basic_block prev = next_bb->prev_bb;
  basic_block new_bb;
  rtx next;
  rtx new_insn;
  rtx label = gen_label_rtx ();
  edge f, new_edge;
  edge_iterator ei;
  rtx insn = first->moveable_insn;
  int find = 0;
  edge e = NULL;
  int find2 = 0;
  rtx last = get_last_insn ();

  next = BB_END (next_bb->prev_bb);
  while (BARRIER_P (NEXT_INSN (next)))
    next = NEXT_INSN (next);

  new_bb = create_basic_block (NULL, NULL, prev);
  PREV_INSN (BB_HEAD (new_bb)) = next;
  NEXT_INSN (BB_HEAD (new_bb)) = NEXT_INSN (next);
  PREV_INSN (NEXT_INSN (next)) = BB_HEAD (new_bb);
  NEXT_INSN (next) = BB_HEAD (new_bb);
  NEXT_INSN (last) = NULL;
  set_last_insn (last);

  /* Creation of the new instructions for the new bb.  */
  new_insn = make_insn_raw (PATTERN (insn));
  gain--;
  PUT_MODE (new_insn, GET_MODE (insn));
  INSN_LOCATOR (new_insn) = INSN_LOCATOR (insn);

  emit_label_before (label, BB_HEAD (new_bb));
  BB_HEAD (new_bb) = label;
  set_block_for_insn (label, new_bb);
  add_insn_after (new_insn, BB_END (new_bb));
  set_block_for_insn (new_insn, new_bb);
  BB_END (new_bb) = new_insn;

  /* Creation of the edge of the new bb.  */
  new_edge = make_edge (new_bb, next_bb, EDGE_FALLTHRU);
  if (new_edge)
    new_edge->probability = REG_BR_PROB_BASE;

  FOR_EACH_EDGE (f, ei, prev->succs)
  {
    if (f->dest == next_bb)
      {
	e = f;
	for (di = first; di; di = di->next_analogous)
	  if (prev == di->curr)
	    {
	      find = 1;
	      break;
	    }
	find2 = 1;
	break;
      }
  }

  /* Modifications in the environment.  */
  if (JUMP_P (BB_END (prev)) && !find && find2 && e)
    {
      e->flags &= ~EDGE_FALLTHRU;
      emit_barrier_after (BB_END (prev));
    }
  if (!JUMP_P (BB_END (prev)) && !find && find2 && e)
    {
      edge new_edge_;
      remove_edge (e);
      emit_jump_insn_after (gen_jump (BB_HEAD (next_bb)), BB_END (prev));
      gain--;
      emit_barrier_after (BB_END (prev));
      new_edge_ = make_edge (prev, next_bb, 0);
      if (new_edge_)
	new_edge_->probability = REG_BR_PROB_BASE;
      JUMP_LABEL (BB_END (prev)) = block_label (next_bb);
      LABEL_NUSES (block_label (next_bb))++;
    }

  for (di = first; di; di = di->next_analogous)
    {
      rtx rinsn = di->moveable_insn;
      di->moveable_insn = NULL;
      remove_insn (rinsn);
      gain++;
    }

  modify_jump_labels (new_bb, first, label);
  return (new_bb);
}


/* If we create a new basic block, we must complement it with a 
   bbDecorator.  */
static bb_decorator
insert_new_bb_decorator (bb_decorator decorator, basic_block new_bb)
{
  bb_decorator last_di;
  bb_decorator new_di = NULL;

  last_di = decorator;
  while (last_di->next_decorator)
    last_di = last_di->next_decorator;

  new_di = xcalloc (1, sizeof (struct bb_decorator_def));
  new_di->num_pred_bb = EDGE_COUNT (new_bb->preds);
  new_di->num_succ_bb = EDGE_COUNT (new_bb->succs);
  new_di->curr = new_bb;
  new_di->prev_decorator = last_di;
  last_di->next_decorator = new_di;
  return new_di;
}

/* Main function of local factoring sinking.  */
static int
rtl_sinking (int depth, bb_decorator decorator)
{
  int change = 0;
  bb_decorator di = decorator;
  while (di)
    {
      di->last_depth = 0;
      di = di->next_decorator;
    }

  di = decorator;
  while (di)
    {
      if (di->next_sibling)
	{
	  bb_decorator di2;
	  rtx insn = NULL;
	  int insert_bb = 0;
	  unsigned long num_of_next_bb = di->num_succ_bb;
	  unsigned long num_of_good_bb = 0;
	  bb_decorator last_sibling;

	  if (di->prev_sibling)
	    insert_bb = 1;
	  insn = find_first_from_end (di, depth);

	  if (!insn)
	    {
	      di = di->next_sibling;
	      continue;
	    }

	  last_sibling = di;

	  /* Finding equal instruction in the concatenated basic blocks.  */
	  for (di2 = di->next_sibling; di2; di2 = di2->next_sibling)
	    {
	      if (find_insn_from_end (insn, di2, depth))
		{
		  last_sibling->next_analogous = di2;
		  di2->prev_analogous = last_sibling;
		  last_sibling = di2;
		  num_of_good_bb++;
		}
	      else
		insert_bb = 1;
	    }
	  /* Cost analizing.  */
	  if (num_of_good_bb <= num_of_next_bb)
	    {
	      di->last_depth++;
	      delete_analogous (di);
	      continue;
	    }

	  if (!insert_bb && EDGE_SUCC (di->curr, 0))
	    {
	      if (num_of_good_bb <
		  EDGE_COUNT ((EDGE_SUCC (di->curr, 0))->dest->preds))
		insert_bb = 1;
	    }

	  if (!insert_bb)
	    {
	      sinking_rtl_stmt (di);
	      change = 1;
	      continue;
	    }
	  else if (di->num_succ_bb == 1
		   && (EDGE_SUCC (di->curr, 0)->dest != EXIT_BLOCK_PTR))
	    {
	      basic_block new_bb;
	      bb_decorator new_bb_decorator;


	      if (optimize_size
		  && find_abnormal_edge (di)
		  && !any_condjump_p (
		       BB_END (EDGE_SUCC (di->curr, 0)->dest->prev_bb)))
		{
		  new_bb = create_basic_block_by_factoring (di);
		  new_bb_decorator =
		    insert_new_bb_decorator (decorator, new_bb);
		  modifying_concatenation (di, new_bb_decorator);
		  change = 1;
		}
	    }
	  else
	    di->last_depth++;

	  delete_analogous (di);
	}
      else
	di = di->next_sibling;
    }
  return change;
}

/* The main entry point of factoring algorithms.  */
int
rtl_lfact (int depth, enum lfact_direction alg)
{
  bb_decorator decorator = NULL;
  int result = 0;
  gain = 0;

  decorator = init_factoring (decorator);

  switch (alg)
    {
    case LFD_HOISTING:
      collect_full_sibling (decorator, alg);
      cost_analyzer (decorator, alg);
      while (rtl_hoisting (depth, decorator))
	result = 1;
      break;
    case LFD_SINKING:
    default:
      collect_sibling (decorator, alg);
      cost_analyzer (decorator, alg);
      while (rtl_sinking (depth, decorator))
	result = 1;
      break;
    }

  free_bb_decorator_list (decorator);

  return (result);
}


