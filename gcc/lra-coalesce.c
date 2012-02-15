/* Coalesce spilled pseudos.
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


/* This pass makes some simple RTL code transformations by coalescing
   pseudo-registers to remove some move insns and by remove dead moves
   whose destination is not used.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tm_p.h"
#include "insn-config.h"
#include "recog.h"
#include "output.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "basic-block.h"
#include "except.h"
#include "timevar.h"
#include "ira.h"
#include "lra-int.h"
#include "df.h"

/* Arrays whose elements represent the first and the next pseudo
   (regno) in the coalesced pseudos group to which given pseudo (its
   regno is the index) belongs.  The next of the last pseudo in the
   group refers to the first pseudo in the group, in other words the
   group is represented by a cyclic list.  */
static int *first_coalesced_pseudo, *next_coalesced_pseudo;

/* The function is used to sort moves according to their execution
   frequencies.  */
static int
move_freq_compare_func (const void *v1p, const void *v2p)
{
  rtx mv1 = *(const rtx *) v1p;
  rtx mv2 = *(const rtx *) v2p;
  int pri1, pri2;
  
  pri1 = BLOCK_FOR_INSN (mv1)->frequency;
  pri2 = BLOCK_FOR_INSN (mv2)->frequency;
  if (pri2 - pri1)
    return pri2 - pri1;

  /* If freqencies are equal, sort by moves, so that the results of
     qsort leave nothing to chance.  */
  return (int) INSN_UID (mv1) - (int) INSN_UID (mv2);
}

/* Pseudos which go away after coalescing.  */
static bitmap_head coalesced_pseudos_bitmap;

/* Merge two sets of coalesced pseudos given correspondingly by
   pseudos REGNO1 and REGNO2 (more accurately merging REGNO2 group
   into REGNO1 group).  Set up COALESCED_PSEUDOS_BITMAP.  */
static void
merge_pseudos (int regno1, int regno2)
{
  int regno, first, first2, last, next;

  first = first_coalesced_pseudo[regno1];
  if ((first2 = first_coalesced_pseudo[regno2]) == first)
    return;
  for (last = regno2, regno = next_coalesced_pseudo[regno2];;
       regno = next_coalesced_pseudo[regno])
    {
      first_coalesced_pseudo[regno] = first;
      bitmap_set_bit (&coalesced_pseudos_bitmap, regno);
      if (regno == regno2)
	break;
      last = regno;
    }
  next = next_coalesced_pseudo[first];
  next_coalesced_pseudo[first] = regno2;
  next_coalesced_pseudo[last] = next;
  lra_reg_info[first].live_ranges
    = (lra_merge_live_ranges
       (lra_reg_info[first].live_ranges,
	lra_copy_live_range_list (lra_reg_info[first2].live_ranges)));
  if (GET_MODE_SIZE (lra_reg_info[first].biggest_mode)
      < GET_MODE_SIZE (lra_reg_info[first2].biggest_mode))
    lra_reg_info[first].biggest_mode = lra_reg_info[first2].biggest_mode;
}

/* Change pseudos in *LOC on their coalescing group
   representatives.  */
static bool
substitute (rtx *loc)
{
  int i, regno;
  const char *fmt;
  enum rtx_code code;
  bool res;

  if (*loc == NULL_RTX)
    return false;
  code = GET_CODE (*loc);
  if (code == REG)
    {
      regno = REGNO (*loc);
      if (regno < FIRST_PSEUDO_REGISTER
	  || first_coalesced_pseudo[regno] == regno)
	return false;
      *loc = regno_reg_rtx[first_coalesced_pseudo[regno]];
      return true;
    }

  res = false;
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  if (substitute (&XEXP (*loc, i)))
	    res = true;
	}
      else if (fmt[i] == 'E')
	{
	  int j;

	  for (j = XVECLEN (*loc, i) - 1; j >= 0; j--)
	    if (substitute (&XVECEXP (*loc, i, j)))
	      res = true;
	}
    }
  return res;
}

/* The current iteration (1, 2, ...) of the coalecsing pass.  */
int lra_coalesce_iter;

/* Return true if the move involving REGNO1 and REGNO2 is a
   memory-memory move.  */
static bool
mem_move_p (int regno1, int regno2)
{
  return reg_renumber[regno1] < 0 && reg_renumber[regno2] < 0;
}


/* Pseudos which go away after coalescing and pseudos used instead of
   the removed pseudos.  */
static bitmap_head removed_pseudos_bitmap, used_pseudos_bitmap;

/* Set up REMOVED_PSEUDOS_BITMAP and USED_PSEUDOS_BITMAP, and update
   LR_BITMAP (a BB live info bitmap).  */
static void
update_live_info (bitmap lr_bitmap)
{
  unsigned int j;
  bitmap_iterator bi;

  bitmap_clear (&removed_pseudos_bitmap);
  bitmap_clear (&used_pseudos_bitmap);
  EXECUTE_IF_AND_IN_BITMAP (&coalesced_pseudos_bitmap, lr_bitmap,
			    FIRST_PSEUDO_REGISTER, j, bi)
    {
      bitmap_set_bit (&removed_pseudos_bitmap, j);
      bitmap_set_bit (&used_pseudos_bitmap, first_coalesced_pseudo[j]);
    }
  if (! bitmap_empty_p (&removed_pseudos_bitmap))
    {
      bitmap_and_compl_into (lr_bitmap, &removed_pseudos_bitmap);
      bitmap_ior_into (lr_bitmap, &used_pseudos_bitmap);
    }
}

/* The major function for aggressive pseudo coalescing of moves only
   if the both pseudos were spilled and not bound.  */
bool
lra_coalesce (void)
{
  basic_block bb;
  rtx mv, set, insn, next, *sorted_moves;
  int i, n, mv_num, sregno, dregno, restore_regno;
  unsigned int regno;
  int coalesced_moves;
  int max_regno = max_reg_num ();
  bitmap_head involved_insns_bitmap, split_origin_bitmap;
  bitmap_iterator bi;

  if (lra_dump_file != NULL)
    fprintf (lra_dump_file,
	     "\n********** Pseudos coalescing #%d: **********\n\n",
	     ++lra_coalesce_iter);
  first_coalesced_pseudo = (int *) xmalloc (sizeof (int) * max_regno);
  next_coalesced_pseudo = (int *) xmalloc (sizeof (int) * max_regno);
  for (i = 0; i < max_regno; i++)
    first_coalesced_pseudo[i] = next_coalesced_pseudo[i] = i;
  sorted_moves = (rtx *) xmalloc (get_max_uid () * sizeof (rtx));
  mv_num = 0;
  /* Collect pseudos whose live ranges were split.  */
  bitmap_initialize (&split_origin_bitmap, &reg_obstack);
  EXECUTE_IF_SET_IN_BITMAP (&lra_split_pseudos, 0, regno, bi)
    if ((restore_regno = lra_reg_info[regno].restore_regno) >= 0)
      bitmap_set_bit (&split_origin_bitmap, restore_regno);
  /* Collect moves.  */
  coalesced_moves = 0;
  FOR_EACH_BB (bb)
    {
      FOR_BB_INSNS_SAFE (bb, insn, next)
	if (INSN_P (insn)
	    && (set = single_set (insn)) != NULL_RTX
	    && REG_P (SET_DEST (set)) && REG_P (SET_SRC (set))
	    && (sregno = REGNO (SET_SRC (set))) >= FIRST_PSEUDO_REGISTER
	    && (dregno = REGNO (SET_DEST (set))) >= FIRST_PSEUDO_REGISTER
	    && mem_move_p (sregno, dregno)
	    /* Don't coalesce inheritance pseudos because spilled
	       inheritance pseudos will be removed in subsequent 'undo
	       inheritance' pass.  */
	    && lra_reg_info[sregno].restore_regno < 0
	    && lra_reg_info[dregno].restore_regno < 0
	    /* We undo splits for spilled pseudos whose live ranges
	       were split.  So don't coalesce them, it is not
	       necessary and the undo transformations would be
	       wrong.  */
	    && ! bitmap_bit_p (&split_origin_bitmap, sregno)
	    && ! bitmap_bit_p (&split_origin_bitmap, dregno)
	    && ! side_effects_p (set)
	    /* Don't coalesces bound pseudos.  Bound pseudos has own
	       rules for finding live ranges.  It is hard to maintain
	       this info with coalescing and it is not worth to do
	       it.  */
	    && ! bitmap_bit_p (&lra_bound_pseudos, sregno)
	    && ! bitmap_bit_p (&lra_bound_pseudos, dregno)
	    /* We don't want to coalesce regnos with equivalences,
	       at least without updating this info.  */
	    && ira_reg_equiv[sregno].constant == NULL_RTX
	    && ira_reg_equiv[sregno].memory == NULL_RTX
	    && ira_reg_equiv[sregno].invariant == NULL_RTX
	    && ira_reg_equiv[dregno].constant == NULL_RTX
	    && ira_reg_equiv[dregno].memory == NULL_RTX
	    && ira_reg_equiv[dregno].invariant == NULL_RTX
	    && ! lra_intersected_live_ranges_p (lra_reg_info[sregno].live_ranges,
						lra_reg_info[dregno].live_ranges))
	  sorted_moves[mv_num++] = insn;
    }
  bitmap_clear (&removed_pseudos_bitmap);
  qsort (sorted_moves, mv_num, sizeof (rtx), move_freq_compare_func);
  /* Coalesced copies, most frequently executed first.  */
  bitmap_initialize (&coalesced_pseudos_bitmap, &reg_obstack);
  bitmap_initialize (&involved_insns_bitmap, &reg_obstack);
  for (; mv_num != 0;)
    {
      for (i = 0; i < mv_num; i++)
	{
	  mv = sorted_moves[i];
	  set = single_set (mv);
	  gcc_assert (set != NULL && REG_P (SET_SRC (set))
		      && REG_P (SET_DEST (set)));
	  sregno = REGNO (SET_SRC (set));
	  dregno = REGNO (SET_DEST (set));
	  if (! lra_intersected_live_ranges_p
	        (lra_reg_info[first_coalesced_pseudo[sregno]].live_ranges,
		 lra_reg_info[first_coalesced_pseudo[dregno]].live_ranges))
	    {
	      coalesced_moves++;
	      if (lra_dump_file != NULL)
		fprintf
		  (lra_dump_file,
		   "      Coalescing move %i:r%d(%d)-r%d(%d) (freq=%d)\n",
		   INSN_UID (mv), sregno, ORIGINAL_REGNO (SET_SRC (set)),
		   dregno, ORIGINAL_REGNO (SET_DEST (set)),
		   BLOCK_FOR_INSN (mv)->frequency);
	      bitmap_ior_into (&involved_insns_bitmap,
			       &lra_reg_info[sregno].insn_bitmap);
	      bitmap_ior_into (&involved_insns_bitmap,
			       &lra_reg_info[dregno].insn_bitmap);
	      merge_pseudos (sregno, dregno);
	      i++;
	      break;
	    }
	}
      /* Collect the rest of copies.  */
      for (n = 0; i < mv_num; i++)
	{
	  mv = sorted_moves[i];
	  set = single_set (mv);
	  gcc_assert (set != NULL && REG_P (SET_SRC (set))
		      && REG_P (SET_DEST (set)));
	  sregno = REGNO (SET_SRC (set));
	  dregno = REGNO (SET_DEST (set));
	  if (first_coalesced_pseudo[sregno] != first_coalesced_pseudo[dregno])
	    sorted_moves[n++] = mv;
	  else if (lra_dump_file != NULL)
	    {
	      coalesced_moves++;
	      fprintf
		(lra_dump_file,
		 "      Coalescing move %i:r%d-r%d (freq=%d)\n",
		 INSN_UID (mv), sregno, dregno, BLOCK_FOR_INSN (mv)->frequency);
	    }
	}
      mv_num = n;
    }
  bitmap_initialize (&removed_pseudos_bitmap, &reg_obstack);
  bitmap_initialize (&used_pseudos_bitmap, &reg_obstack);
  FOR_EACH_BB (bb)
    {
      update_live_info (DF_LR_IN (bb));
      update_live_info (DF_LR_OUT (bb));
      FOR_BB_INSNS_SAFE (bb, insn, next)
	if (INSN_P (insn)
	    && bitmap_bit_p (&involved_insns_bitmap, INSN_UID (insn)))
	  {
	    if (! substitute (&insn))
	      continue;
	    lra_update_insn_regno_info (insn);
	    if ((set = single_set (insn)) != NULL_RTX
		&& REG_P (SET_DEST (set)) && REG_P (SET_SRC (set))
		&& REGNO (SET_SRC (set)) == REGNO (SET_DEST (set))
		&& ! side_effects_p (set))
	      {
		/* Coalesced move. */
		if (lra_dump_file != NULL)
		  fprintf (lra_dump_file, "      Removing move %i (freq=%d)\n",
			 INSN_UID (insn), BLOCK_FOR_INSN (insn)->frequency);
		lra_set_insn_deleted (insn);
	      }
	  }
    }
  bitmap_clear (&removed_pseudos_bitmap);
  bitmap_clear (&used_pseudos_bitmap);
  bitmap_clear (&involved_insns_bitmap);
  bitmap_clear (&coalesced_pseudos_bitmap);
  if (lra_dump_file != NULL && coalesced_moves != 0)
    fprintf (lra_dump_file, "Coalesced Moves = %d\n", coalesced_moves);
  free (sorted_moves);
  free (next_coalesced_pseudo);
  free (first_coalesced_pseudo);
  return coalesced_moves != 0;
}
