/* Build live ranges for pseudos.
   Copyright (C) 2010, 2011
   Free Software Foundation, Inc.

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


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "hard-reg-set.h"
#include "rtl.h"
#include "tm_p.h"
#include "insn-config.h"
#include "recog.h"
#include "output.h"
#include "regs.h"
#include "function.h"
#include "expr.h"
#include "basic-block.h"
#include "except.h"
#include "df.h"
#include "ira.h"
#include "sparseset.h"
#include "lra-int.h"

/* Program points are enumerated by numbers from range
   0..LRA_LIVE_MAX_POINT-1.  There are approximately two times more
   program points than insns.  Program points are places in the
   program where liveness info can be changed.  In most general case
   (there are more complicated cases too) some program points
   correspond to places where input operand dies and other ones
   correspond to places where output operands are born.  */
int lra_live_max_point;

/* Arrays of size LRA_LIVE_MAX_POINT mapping a program point to the
   pseudo live ranges with given start/finish point.  */
lra_live_range_t *lra_start_point_ranges, *lra_finish_point_ranges;

/* Accumulated execution frequency of all references for each hard
   register.  */
int lra_hard_reg_usage[FIRST_PSEUDO_REGISTER];

/* A global flag whose true value says to build live ranges for all
   pseudos, otherwise the live ranges only for pseudos got memory is
   build.  True value means also building copies and setting up hard
   register preferences.  The complete info is necessary only for the
   assignment pass.  */
static bool complete_info_p;

/* Number of the current program point.  */
static int curr_point;

/* Pseudos live at current point in the scan.  */
static sparseset pseudos_live;

/* Set of hard regs (except eliminable ones) currently live.  */
static HARD_REG_SET hard_regs_live;

/* Set of pseudos and hard registers start living/dying.  */
static sparseset start_living, start_dying;

/* Set of pseudos and hard regs dead and unused in the current
   insn.  */
static sparseset unused_set, dead_set;

/* Pools for pseudo live ranges.  */
static alloc_pool live_range_pool;

/* Free live range LR.  */
static void
free_live_range (lra_live_range_t lr)
{
  pool_free (live_range_pool, lr);
}

/* Free live range list LR.  */
static void
free_live_range_list (lra_live_range_t lr)
{
  lra_live_range_t next;

  while (lr != NULL)
    {
      next = lr->next;
      free_live_range (lr);
      lr = next;
    }
}

/* Create and return pseudo live range with given attributes.  */
static lra_live_range_t
create_live_range (int regno, int start, int finish, lra_live_range_t next)
{
  lra_live_range_t p;

  p = (lra_live_range_t) pool_alloc (live_range_pool);
  p->regno = regno;
  p->start = start;
  p->finish = finish;
  p->next = next;
  return p;
}

/* Copy live range R and return the result.  */
static lra_live_range_t
copy_live_range (lra_live_range_t r)
{
  lra_live_range_t p;

  p = (lra_live_range_t) pool_alloc (live_range_pool);
  *p = *r;
  return p;
}

/* Copy live range list given by its head R and return the result.  */
lra_live_range_t
lra_copy_live_range_list (lra_live_range_t r)
{
  lra_live_range_t p, first, last;

  if (r == NULL)
    return NULL;
  for (first = last = NULL; r != NULL; r = r->next)
    {
      p = copy_live_range (r);
      if (first == NULL)
	first = p;
      else
	last->next = p;
      last = p;
    }
  return first;
}

/* Merge ranges R1 and R2 and returns the result.  The function
   maintains the order of ranges and tries to minimize number of the
   result ranges.  */
lra_live_range_t 
lra_merge_live_ranges (lra_live_range_t r1, lra_live_range_t r2)
{
  lra_live_range_t first, last, temp;

  if (r1 == NULL)
    return r2;
  if (r2 == NULL)
    return r1;
  for (first = last = NULL; r1 != NULL && r2 != NULL;)
    {
      if (r1->start < r2->start)
	{
	  temp = r1;
	  r1 = r2;
	  r2 = temp;
	}
      if (r1->start <= r2->finish + 1)
	{
	  /* Intersected ranges: merge r1 and r2 into r1.  */
	  r1->start = r2->start;
	  if (r1->finish < r2->finish)
	    r1->finish = r2->finish;
	  temp = r2;
	  r2 = r2->next;
	  pool_free (live_range_pool, temp);
	  if (r2 == NULL)
	    {
	      /* To try to merge with subsequent ranges in r1.  */
	      r2 = r1->next;
	      r1->next = NULL;
	    }
	}
      else
	{
	  /* Add r1 to the result.  */
	  if (first == NULL)
	    first = last = r1;
	  else
	    {
	      last->next = r1;
	      last = r1;
	    }
	  r1 = r1->next;
	  if (r1 == NULL)
	    {
	      /* To try to merge with subsequent ranges in r2.  */
	      r1 = r2->next;
	      r2->next = NULL;
	    }
	}
    }
  if (r1 != NULL)
    {
      if (first == NULL)
	first = r1;
      else
	last->next = r1;
      gcc_assert (r1->next == NULL);
    }
  else if (r2 != NULL)
    {
      if (first == NULL)
	first = r2;
      else
	last->next = r2;
      gcc_assert (r2->next == NULL);
    }
  else
    {
      gcc_assert (last->next == NULL);
    }
  return first;
}

/* Return TRUE if live ranges R1 and R2 intersect.  */
bool
lra_intersected_live_ranges_p (lra_live_range_t r1, lra_live_range_t r2)
{
  /* Remember the live ranges are always kept ordered.  */
  while (r1 != NULL && r2 != NULL)
    {
      if (r1->start > r2->finish)
	r1 = r1->next;
      else if (r2->start > r1->finish)
	r2 = r2->next;
      else
	return true;
    }
  return false;
}

/* Return TRUE if live range R1 is in R2.  */
bool
lra_live_range_in_p (lra_live_range_t r1, lra_live_range_t r2)
{
  /* Remember the live ranges are always kept ordered.  */
  while (r1 != NULL && r2 != NULL)
    {
      /* R1's element is in R2's element.  */
      if (r2->start <= r1->start && r1->finish <= r2->finish)
	r1 = r1->next;
      /* Intersection: R1's start is in R2.  */
      else if (r2->start <= r1->start && r1->start <= r2->finish)
	return false;
      /* Intersection: R1's finish is in R2.  */
      else if (r2->start <= r1->finish && r1->finish <= r2->finish)
	return false;
      else if (r1->start > r2->finish)
	return false; /* No covering R2's element for R1's one.  */
      else
	r2 = r2->next;
    }
  return r1 == NULL;
}

/* The function processing birth of hard_register REGNO.  It updates
   living hard regs and conflict hard regs for living pseudos.  */
static void
make_hard_regno_born (int regno)
{
  unsigned int i;

  gcc_assert (regno < FIRST_PSEUDO_REGISTER);
  if (TEST_HARD_REG_BIT (lra_no_alloc_regs, regno)
      || TEST_HARD_REG_BIT (hard_regs_live, regno))
    return;
  SET_HARD_REG_BIT (hard_regs_live, regno);
  sparseset_set_bit (start_living, regno);
  EXECUTE_IF_SET_IN_SPARSESET (pseudos_live, i)
    SET_HARD_REG_BIT (lra_reg_info[i].conflict_hard_regs, regno);
}

/* Process the death of hard register REGNO.  This updates
   hard_regs_live.  */
static void
make_hard_regno_dead (int regno)
{
  if (TEST_HARD_REG_BIT (lra_no_alloc_regs, regno)
      || ! TEST_HARD_REG_BIT (hard_regs_live, regno))
    return;
  gcc_assert (regno < FIRST_PSEUDO_REGISTER);
  sparseset_set_bit (start_dying, regno);
  CLEAR_HARD_REG_BIT (hard_regs_live, regno);
}

/* Mark pseudo REGNO as currently living, update conflicting hard
   registers of the pseudo, and start a new live range for the pseudo
   corresponding to REGNO if it is necessary.  */
static void
mark_pseudo_live (int regno)
{
  lra_live_range_t p;

  gcc_assert (regno >= FIRST_PSEUDO_REGISTER);
  gcc_assert (! sparseset_bit_p (pseudos_live, regno));
  sparseset_set_bit (pseudos_live, regno);
  IOR_HARD_REG_SET (lra_reg_info[regno].conflict_hard_regs, hard_regs_live);
  
  if ((complete_info_p || lra_get_regno_hard_regno (regno) < 0)
      && ((p = lra_reg_info[regno].live_ranges) == NULL
	  || (p->finish != curr_point && p->finish + 1 != curr_point)))
    lra_reg_info[regno].live_ranges
      = create_live_range (regno, curr_point, -1, p);
  sparseset_set_bit (start_living, regno);
}

/* Mark pseudo REGNO as currently not living. This finishes the
   current live range for the pseudo corresponding to REGNO.  */
static void
mark_pseudo_dead (int regno)
{
  lra_live_range_t p;

  gcc_assert (regno >= FIRST_PSEUDO_REGISTER);
  gcc_assert (sparseset_bit_p (pseudos_live, regno));
  sparseset_clear_bit (pseudos_live, regno);
  sparseset_set_bit (start_dying, regno);
  if (complete_info_p || lra_get_regno_hard_regno (regno) < 0)
    {
      p = lra_reg_info[regno].live_ranges;
      gcc_assert (p != NULL);
      p->finish = curr_point;
    }
}

/* Mark register REGNO in MODE as live.  */
static void
mark_regno_live (int regno, enum machine_mode mode)
{
  int last;

  if (regno < FIRST_PSEUDO_REGISTER)
    {
      for (last = regno + hard_regno_nregs[regno][mode];
	   regno < last;
	   regno++)
	make_hard_regno_born (regno);
    }
  else if (! sparseset_bit_p (pseudos_live, regno))
    mark_pseudo_live (regno);
}


/* Mark register REGNO in MODE as dead.  */
static void
mark_regno_dead (int regno, enum machine_mode mode)
{
  int last;

  if (regno < FIRST_PSEUDO_REGISTER)
    {
      for (last = regno + hard_regno_nregs[regno][mode];
	   regno < last;
	   regno++)
	make_hard_regno_dead (regno);
    }
  else if (sparseset_bit_p (pseudos_live, regno))
    mark_pseudo_dead (regno);
}

/* Insn currently scanned. */
static rtx curr_insn;
/* The insn data.  */
static lra_insn_recog_data_t curr_id;
/* The insn static data.  */
static struct lra_static_insn_data *curr_static_id;

/* Return true when one of the predecessor edges of BB is marked with
   EDGE_ABNORMAL_CALL or EDGE_EH.  */
static bool
bb_has_abnormal_call_pred (basic_block bb)
{
  edge e;
  edge_iterator ei;
  
  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      if (e->flags & (EDGE_ABNORMAL_CALL | EDGE_EH))
	return true;
    }
  return false;
}

/* Vec containing frequencies of program points.  */
static VEC(int,heap) *point_freq_vec;

/* The start of the above vector elements.  */
int *lra_point_freq;

/* Increment the current program point to the next point with
   execution frequency FREQ.  */
static void
incr_curr_point (int freq)
{
  VEC_safe_push (int, heap, point_freq_vec, freq);
  lra_point_freq = VEC_address (int, point_freq_vec);
  curr_point++;
}

/* Update the preference of HARD_REGNO for pseudo REGNO by PROFIT.  */
void
lra_setup_reload_pseudo_preferenced_hard_reg (int regno,
					      int hard_regno, int profit)
{
  gcc_assert (regno >= lra_constraint_new_regno_start);
  if (lra_reg_info[regno].preferred_hard_regno1 == hard_regno)
    lra_reg_info[regno].preferred_hard_regno_profit1 += profit;
  else if (lra_reg_info[regno].preferred_hard_regno2 == hard_regno)
    lra_reg_info[regno].preferred_hard_regno_profit2 += profit;
  else if (lra_reg_info[regno].preferred_hard_regno1 < 0)
    {
      lra_reg_info[regno].preferred_hard_regno1 = hard_regno;
      lra_reg_info[regno].preferred_hard_regno_profit1 = profit;
    }
  else if (lra_reg_info[regno].preferred_hard_regno2 < 0
	   || profit > lra_reg_info[regno].preferred_hard_regno_profit2)
    {
      lra_reg_info[regno].preferred_hard_regno2 = hard_regno;
      lra_reg_info[regno].preferred_hard_regno_profit2 = profit;
    }
  else
    return;
  /* Keep the 1st hard regno as more profitable.  */
  if (lra_reg_info[regno].preferred_hard_regno1 >= 0
      && lra_reg_info[regno].preferred_hard_regno2 >= 0
      && (lra_reg_info[regno].preferred_hard_regno_profit2
	  > lra_reg_info[regno].preferred_hard_regno_profit1))
    {
      int temp;

      temp = lra_reg_info[regno].preferred_hard_regno1;
      lra_reg_info[regno].preferred_hard_regno1
	= lra_reg_info[regno].preferred_hard_regno2;
      lra_reg_info[regno].preferred_hard_regno2 = temp;
      temp = lra_reg_info[regno].preferred_hard_regno_profit1;
      lra_reg_info[regno].preferred_hard_regno_profit1
	= lra_reg_info[regno].preferred_hard_regno_profit2;
      lra_reg_info[regno].preferred_hard_regno_profit2 = temp;
    }
  if (lra_dump_file != NULL)
    {
      if ((hard_regno = lra_reg_info[regno].preferred_hard_regno1) >= 0)
	fprintf (lra_dump_file,
		 "      Hard reg %d is preferable by r%d with profit %d\n",
		 hard_regno, regno,
		 lra_reg_info[regno].preferred_hard_regno_profit1);
      if ((hard_regno = lra_reg_info[regno].preferred_hard_regno2) >= 0)
	fprintf (lra_dump_file,
		 "      Hard reg %d is preferable by r%d with profit %d\n",
		 hard_regno, regno,
		 lra_reg_info[regno].preferred_hard_regno_profit2);
    }
}

/* Process insns of the basic block BB to update pseudo live ranges,
   pseudo hard register conflicts.  */
static void
process_bb_lives (basic_block bb)
{
  int i, regno, freq;
  unsigned int j;
  bitmap_iterator bi;
  bitmap reg_live_out;
  unsigned int px;
  rtx link, *link_loc;

  reg_live_out = DF_LR_OUT (bb);
  sparseset_clear (pseudos_live);
  REG_SET_TO_HARD_REG_SET (hard_regs_live, reg_live_out);
  AND_COMPL_HARD_REG_SET (hard_regs_live, eliminable_regset);
  AND_COMPL_HARD_REG_SET (hard_regs_live, lra_no_alloc_regs);
  EXECUTE_IF_SET_IN_BITMAP (reg_live_out, 0, j, bi)
    if (j >= FIRST_PSEUDO_REGISTER)
      mark_pseudo_live (j);
      
  freq = REG_FREQ_FROM_BB (bb);

  if (lra_dump_file != NULL)
    fprintf (lra_dump_file, "  BB %d\n", bb->index);

  /* Scan the code of this basic block, noting which pseudos and hard
     regs are born or die.

     Note that this loop treats uninitialized values as live until the
     beginning of the block.  For example, if an instruction uses
     (reg:DI foo), and only (subreg:SI (reg:DI foo) 0) is ever set,
     FOO will remain live until the beginning of the block.  Likewise
     if FOO is not set at all.  This is unnecessarily pessimistic, but
     it probably doesn't matter much in practice.  */
  FOR_BB_INSNS_REVERSE (bb, curr_insn)
    {
      bool call_p;
      int dst_regno, src_regno;
      rtx set;
      struct lra_insn_reg *reg;

      if (!NONDEBUG_INSN_P (curr_insn))
	continue;
      
      curr_id = lra_get_insn_recog_data (curr_insn);
      curr_static_id = curr_id->insn_static_data;
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "   Insn %u: point = %d\n",
		 INSN_UID (curr_insn), curr_point);

      /* Update max ref width and hard reg usage.  */
      for (reg = curr_id->regs; reg != NULL; reg = reg->next)
	if (reg->regno >= FIRST_PSEUDO_REGISTER
	    && (GET_MODE_SIZE (reg->biggest_mode)
		> GET_MODE_SIZE (lra_reg_info[reg->regno].biggest_mode)))
	  lra_reg_info[reg->regno].biggest_mode = reg->biggest_mode;
	else if (reg->regno < FIRST_PSEUDO_REGISTER)
	  lra_hard_reg_usage[reg->regno] += freq;

      call_p = CALL_P (curr_insn);
      if (complete_info_p
	  && (set = single_set (curr_insn)) != NULL_RTX
	  && REG_P (SET_DEST (set)) && REG_P (SET_SRC (set))
	  /* Check that source regno does not conflict with
	     destination regno to exclude most impossible
	     preferences.  */
	  && ((((src_regno = REGNO (SET_SRC (set))) >= FIRST_PSEUDO_REGISTER
		&& ! sparseset_bit_p (pseudos_live, src_regno))
	       || (src_regno < FIRST_PSEUDO_REGISTER
		   && ! TEST_HARD_REG_BIT (hard_regs_live, src_regno)))
	      /* It might be 'inheritance pseudo <- reload pseudo'.  */
	      || (src_regno >= lra_constraint_new_regno_start
		  && ((int) REGNO (SET_DEST (set))
		      >= lra_constraint_new_regno_start))))
	{
	  int hard_regno = -1, regno = -1;

	  dst_regno = REGNO (SET_DEST (set));
	  if (dst_regno >= lra_constraint_new_regno_start
	      && src_regno >= lra_constraint_new_regno_start)
	    lra_create_copy (dst_regno, src_regno, freq);
	  else if (dst_regno >= lra_constraint_new_regno_start)
	    {
	      if ((hard_regno = src_regno) >= FIRST_PSEUDO_REGISTER)
		hard_regno = reg_renumber[src_regno];
	      regno = dst_regno;
	    }
	  else if (src_regno >= lra_constraint_new_regno_start)
	    {
	      if ((hard_regno = dst_regno) >= FIRST_PSEUDO_REGISTER)
		hard_regno = reg_renumber[dst_regno];
	      regno = src_regno;
	    }
	  if (regno >= 0 && hard_regno >= 0)
	    lra_setup_reload_pseudo_preferenced_hard_reg
	      (regno, hard_regno, freq);
	}

      sparseset_clear (start_living);
      /* Mark each defined value as live.  We need to do this for
	 unused values because they still conflict with quantities
	 that are live at the time of the definition.  */
      for (reg = curr_id->regs; reg != NULL; reg = reg->next)
	if (reg->type != OP_IN)
	  mark_regno_live (reg->regno, reg->biggest_mode);

      for (reg = curr_static_id->hard_regs; reg != NULL; reg = reg->next)
	if (reg->type != OP_IN)
	  make_hard_regno_born (reg->regno);

      sparseset_copy (unused_set, start_living);
      
      sparseset_clear (start_dying);

      /* See which defined values die here.  */
      for (reg = curr_id->regs; reg != NULL; reg = reg->next)
	if (reg->type == OP_OUT && ! reg->early_clobber
	    && (! reg->subreg_p
		|| bitmap_bit_p (&lra_bound_pseudos, reg->regno)))
	  mark_regno_dead (reg->regno, reg->biggest_mode);

      for (reg = curr_static_id->hard_regs; reg != NULL; reg = reg->next)
	if (reg->type == OP_OUT && ! reg->early_clobber
	    && (! reg->subreg_p
		|| bitmap_bit_p (&lra_bound_pseudos, reg->regno)))
	  make_hard_regno_dead (reg->regno);

      if (call_p)
	{
	  /* The current set of live pseudos are live across the
	     call.  */
	  EXECUTE_IF_SET_IN_SPARSESET (pseudos_live, i)
	    {
	      /* Don't allocate pseudos that cross setjmps or any
		 call, if this function receives a nonlocal goto.  */
	      if (cfun->has_nonlocal_label
		  || find_reg_note (curr_insn, REG_SETJMP,
				    NULL_RTX) != NULL_RTX)
		SET_HARD_REG_SET (lra_reg_info[i].conflict_hard_regs);
	      IOR_HARD_REG_SET (lra_reg_info[i].conflict_hard_regs,
				call_used_reg_set);
	    }
	}
      
      incr_curr_point (freq);
      
      sparseset_clear (start_living);

      /* Mark each used value as live.  */
      for (reg = curr_id->regs; reg != NULL; reg = reg->next)
	if (reg->type == OP_IN)
	  mark_regno_live (reg->regno, reg->biggest_mode);

      for (reg = curr_static_id->hard_regs; reg != NULL; reg = reg->next)
	if (reg->type == OP_IN)
	  make_hard_regno_born (reg->regno);

      if (curr_id->arg_hard_regs != NULL)
	/* Make argument hard registers live.  */
	for (i = 0; (regno = curr_id->arg_hard_regs[i]) >= 0; i++)
	  make_hard_regno_born (regno);

      sparseset_and_compl (dead_set, start_living, start_dying);
      
      /* Mark early clobber outputs dead.  */
      for (reg = curr_id->regs; reg != NULL; reg = reg->next)
	if (reg->type == OP_OUT && reg->early_clobber && ! reg->subreg_p)
	  mark_regno_dead (reg->regno, reg->biggest_mode);

      for (reg = curr_static_id->hard_regs; reg != NULL; reg = reg->next)
	if (reg->type == OP_OUT && reg->early_clobber && ! reg->subreg_p)
	  make_hard_regno_dead (reg->regno);

      incr_curr_point (freq);

      /* Update notes.  */
      for (link_loc = &REG_NOTES (curr_insn); (link = *link_loc) != NULL_RTX;)
	{
	  if (REG_NOTE_KIND (link) != REG_DEAD
	      && REG_NOTE_KIND (link) != REG_UNUSED)
	    ;
	  else if (REG_P (XEXP (link, 0)))
	    {
	      regno = REGNO (XEXP (link, 0));
	      if ((REG_NOTE_KIND (link) == REG_DEAD
		   && ! sparseset_bit_p (dead_set, regno))
		  || (REG_NOTE_KIND (link) == REG_UNUSED
		      && ! sparseset_bit_p (unused_set, regno)))
		{
		  *link_loc = XEXP (link, 1);
		  continue;
		}
	      if (REG_NOTE_KIND (link) == REG_DEAD)
		sparseset_clear_bit (dead_set, regno);
	      else if (REG_NOTE_KIND (link) == REG_UNUSED)
		sparseset_clear_bit (unused_set, regno);
	    }
	  link_loc = &XEXP (link, 1);  
	}
      EXECUTE_IF_SET_IN_SPARSESET (dead_set, j)
	add_reg_note (curr_insn, REG_DEAD, regno_reg_rtx[j]);
      EXECUTE_IF_SET_IN_SPARSESET (unused_set, j)
	add_reg_note (curr_insn, REG_UNUSED, regno_reg_rtx[j]);
    }
  
#ifdef EH_RETURN_DATA_REGNO
  if (bb_has_eh_pred (bb))
    for (j = 0; ; ++j)
      {
	unsigned int regno = EH_RETURN_DATA_REGNO (j);

	if (regno == INVALID_REGNUM)
	  break;
	make_hard_regno_born (regno);
      }
#endif
  
  /* Pseudos can't go in stack regs at the start of a basic block that
     is reached by an abnormal edge. Likewise for call clobbered regs,
     because caller-save, fixup_abnormal_edges and possibly the table
     driven EH machinery are not quite ready to handle such pseudos
     live across such edges.  */
  if (bb_has_abnormal_pred (bb))
    {
#ifdef STACK_REGS
      EXECUTE_IF_SET_IN_SPARSESET (pseudos_live, px)
	lra_reg_info[px].no_stack_p = true;
      for (px = FIRST_STACK_REG; px <= LAST_STACK_REG; px++)
	make_hard_regno_born (px);
#endif
      /* No need to record conflicts for call clobbered regs if we
	 have nonlocal labels around, as we don't ever try to
	 allocate such regs in this case.  */
      if (!cfun->has_nonlocal_label && bb_has_abnormal_call_pred (bb))
	for (px = 0; px < FIRST_PSEUDO_REGISTER; px++)
	  if (call_used_regs[px])
	    make_hard_regno_born (px);
    }
  
  EXECUTE_IF_SET_IN_SPARSESET (pseudos_live, i)
    mark_pseudo_dead (i);
  
  incr_curr_point (freq);
}

/* Create and set up LRA_START_POINT_RANGES and
   LRA_FINISH_POINT_RANGES.  */
static void
create_start_finish_chains (void)
{
  int i, max_regno;
  lra_live_range_t r;

  lra_start_point_ranges
    = (lra_live_range_t *) xmalloc (lra_live_max_point
				    * sizeof (lra_live_range_t));
  memset (lra_start_point_ranges, 0,
	  lra_live_max_point * sizeof (lra_live_range_t));
  lra_finish_point_ranges
    = (lra_live_range_t *) xmalloc (lra_live_max_point
				    * sizeof (lra_live_range_t));
  memset (lra_finish_point_ranges, 0,
	  lra_live_max_point * sizeof (lra_live_range_t));
  max_regno = max_reg_num ();
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    {
      for (r = lra_reg_info[i].live_ranges; r != NULL; r = r->next)
	{
	  r->start_next = lra_start_point_ranges[r->start];
	  lra_start_point_ranges[r->start] = r;
	  r->finish_next = lra_finish_point_ranges[r->finish];
 	  lra_finish_point_ranges[r->finish] = r;
	}
    }
}

/* Rebuild LRA_START_POINT_RANGES and LRA_FINISH_POINT_RANGES after
   new live ranges and program points were added as a result of new
   insn generation.  */
static void
rebuild_start_finish_chains (void)
{
  free (lra_finish_point_ranges);
  free (lra_start_point_ranges);
  create_start_finish_chains ();
}

/* Compress pseudo live ranges by removing program points where
   nothing happens.  */
static void
remove_some_program_points_and_update_live_ranges (void)
{
  unsigned i;
  int n, max_regno;
  int *map;
  lra_live_range_t r, prev_r, next_r;
  sbitmap born_or_dead, born, dead;
  sbitmap_iterator sbi;
  bool born_p, dead_p, prev_born_p, prev_dead_p;
  
  born = sbitmap_alloc (lra_live_max_point);
  dead = sbitmap_alloc (lra_live_max_point);
  sbitmap_zero (born);
  sbitmap_zero (dead);
  max_regno = max_reg_num ();
  for (i = FIRST_PSEUDO_REGISTER; i < (unsigned) max_regno; i++)
    {
      for (r = lra_reg_info[i].live_ranges; r != NULL; r = r->next)
	{
	  gcc_assert (r->start <= r->finish);
	  SET_BIT (born, r->start);
	  SET_BIT (dead, r->finish);
	}
    }
  born_or_dead = sbitmap_alloc (lra_live_max_point);
  sbitmap_a_or_b (born_or_dead, born, dead);
  map = (int *) xmalloc (sizeof (int) * lra_live_max_point);
  n = -1;
  prev_born_p = prev_dead_p = false;
  EXECUTE_IF_SET_IN_SBITMAP (born_or_dead, 0, i, sbi)
    {
      born_p = TEST_BIT (born, i);
      dead_p = TEST_BIT (dead, i);
      if ((prev_born_p && ! prev_dead_p && born_p && ! dead_p)
	  || (prev_dead_p && ! prev_born_p && dead_p && ! born_p))
	{
	  map[i] = n;
	  lra_point_freq[n] = MAX (lra_point_freq[n], lra_point_freq[i]);
	}
      else
	{
	  map[i] = ++n;
	  lra_point_freq[n] = lra_point_freq[i];
	}
      prev_born_p = born_p;
      prev_dead_p = dead_p;
    }
  sbitmap_free (born_or_dead);
  sbitmap_free (born);
  sbitmap_free (dead);
  n++;
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file, "Compressing live ranges: from %d to %d - %d%%\n",
	     lra_live_max_point, n, 100 * n / lra_live_max_point);
  lra_live_max_point = n;
  for (i = FIRST_PSEUDO_REGISTER; i < (unsigned) max_regno; i++)
    {
      for (prev_r = NULL, r = lra_reg_info[i].live_ranges;
	   r != NULL;
	   r = next_r)
	{
	  next_r = r->next;
	  r->start = map[r->start];
	  r->finish = map[r->finish];
	  if (prev_r == NULL || prev_r->start > r->finish + 1)
	    {
	      prev_r = r;
	      continue;
	    }
	  prev_r->start = r->start;
	  prev_r->next = next_r;
	  free_live_range (r);
	}
    }
  free (map);
}

/* Print live ranges R to file F.  */
void
lra_print_live_range_list (FILE *f, lra_live_range_t r)
{
  for (; r != NULL; r = r->next)
    fprintf (f, " [%d..%d]", r->start, r->finish);
  fprintf (f, "\n");
}

/* Print live ranges R to stderr.  */
void
lra_debug_live_range_list (lra_live_range_t r)
{
  lra_print_live_range_list (stderr, r);
}

/* Print live ranges of pseudo REGNO to file F.  */
static void
print_pseudo_live_ranges (FILE *f, int regno)
{
  if (lra_reg_info[regno].live_ranges == NULL)
    return;
  fprintf (f, " r%d:", regno);
  lra_print_live_range_list (f, lra_reg_info[regno].live_ranges);
}

/* Print live ranges of pseudo REGNO to stderr.  */
void
lra_debug_pseudo_live_ranges (int regno)
{
  print_pseudo_live_ranges (stderr, regno);
}

/* Print live ranges of all pseudos to file F.  */
static void
print_live_ranges (FILE *f)
{
  int i, max_regno;

  max_regno = max_reg_num ();
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    print_pseudo_live_ranges (f, i);
}

/* Print live ranges of all pseudos to stderr.  */
void
lra_debug_live_ranges (void)
{
  print_live_ranges (stderr);
}

/* Compress pseudo live ranges.  */
static void
compress_live_ranges (void)
{
  remove_some_program_points_and_update_live_ranges ();
  rebuild_start_finish_chains ();
  if (lra_dump_file != NULL)
    {
      fprintf (lra_dump_file, "Ranges after the compression:\n");
      print_live_ranges (lra_dump_file);
    }
}

/* The number of the current live range pass.  */
int lra_live_range_iter;

/* The main entry function creates live ranges only for memory pseudos
   (or for all ones if ALL_P), set up CONFLICT_HARD_REGS for
   the pseudos.  */
void
lra_create_live_ranges (bool all_p)
{
  basic_block bb;
  int i, hard_regno, max_regno = max_reg_num ();

  complete_info_p = all_p;
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file,
	     "\n********** Pseudo live ranges #%d: **********\n\n",
	     ++lra_live_range_iter);
  memset (lra_hard_reg_usage, 0, sizeof (lra_hard_reg_usage));
  for (i = 0; i < max_regno; i++)
    {
      lra_reg_info[i].live_ranges = NULL;
      CLEAR_HARD_REG_SET (lra_reg_info[i].conflict_hard_regs);
      lra_reg_info[i].preferred_hard_regno1 = -1;
      lra_reg_info[i].preferred_hard_regno2 = -1;
      lra_reg_info[i].preferred_hard_regno_profit1 = 0;
      lra_reg_info[i].preferred_hard_regno_profit2 = 0;
#ifdef STACK_REGS
      lra_reg_info[i].no_stack_p = false;
#endif
      if (regno_reg_rtx[i] != NULL_RTX)
	lra_reg_info[i].biggest_mode = GET_MODE (regno_reg_rtx[i]);
      else
	lra_reg_info[i].biggest_mode = VOIDmode;
      if (i >= FIRST_PSEUDO_REGISTER
	  && lra_reg_info[i].nrefs != 0 && (hard_regno = reg_renumber[i]) >= 0)
	lra_hard_reg_usage[hard_regno] += lra_reg_info[i].freq;
    }
  lra_free_copies ();
  pseudos_live = sparseset_alloc (max_regno);
  start_living = sparseset_alloc (max_regno);
  start_dying = sparseset_alloc (max_regno);
  dead_set = sparseset_alloc (max_regno);
  unused_set = sparseset_alloc (max_regno);
  curr_point = 0;
  point_freq_vec = VEC_alloc (int, heap, get_max_uid () * 2);
  lra_point_freq = VEC_address (int, point_freq_vec);
  FOR_EACH_BB (bb)
    process_bb_lives (bb);
  lra_live_max_point = curr_point;
  create_start_finish_chains ();
  if (lra_dump_file != NULL)
    print_live_ranges (lra_dump_file);
  /* Clean up.  */
  sparseset_free (unused_set);
  sparseset_free (dead_set);
  sparseset_free (start_dying);
  sparseset_free (start_living);
  sparseset_free (pseudos_live);
  compress_live_ranges ();
}

/* Finish all live ranges.  */
void
lra_clear_live_ranges (void)
{
  int i;

  if (lra_finish_point_ranges != NULL)
    {
      free (lra_finish_point_ranges);
      lra_finish_point_ranges = NULL;
    }
  if (lra_start_point_ranges != NULL)
    {
      free (lra_start_point_ranges);
      lra_start_point_ranges = NULL;
    }
  for (i = 0; i < max_reg_num (); i++)
    free_live_range_list (lra_reg_info[i].live_ranges);
  VEC_free (int, heap, point_freq_vec);
}

/* Initialize live ranges data once per function.  */
void
lra_live_ranges_init (void)
{
  live_range_pool = create_alloc_pool ("live ranges",
				       sizeof (struct lra_live_range), 100);
}

/* Finish live ranges data once per function.  */
void
lra_live_ranges_finish (void)
{
  free_alloc_pool (live_range_pool);
}
