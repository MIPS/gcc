/* Save/restore placement optimization based on register allocation.
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


/* This file contains code for placement save/restore insns.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "hard-reg-set.h"
#include "rtl.h"
#include "tm_p.h"
#include "regs.h"
#include "insn-config.h"
#include "recog.h"
#include "output.h"
#include "regs.h"
#include "target.h"
#include "function.h"
#include "expr.h"
#include "basic-block.h"
#include "except.h"
#include "cfgloop.h"
#include "df.h"
#include "ira.h"
#include "lra-int.h"

/* Pseudos living through calls and assigned to call used hard
   registers.  */
static bitmap consideration_pseudos;
/* Currently live pseudos which are subset of the consideration
   pseudos.  */
static bitmap live_pseudos;



/* The number elements in arrays correspondingly REFERENCED_PSEUDOS
   and MODIFIED_PSEUDOS.  */
static int referenced_pseudos_num, modified_pseudos_num;

/* Enough space to keep all operands and two address registers for an
   operand.  */
#define MAX_INSN_PSEUDOS (MAX_RECOG_OPERANDS * 3)

/* Arrays used to store pseudos referenced and modified in the current
   insn.  */
static int referenced_pseudos[MAX_INSN_PSEUDOS];
static int modified_pseudos[MAX_INSN_PSEUDOS];

/* Add pseudos referenced and modified in OP to ones referenced and
   modified in the current insn.  */
static void
mark_pseudos (rtx op, bool out_p)
{
  enum rtx_code code = GET_CODE (op);
  const char *fmt;
  int i, j, regno;

  switch (code)
    {
    case SUBREG:
      op = SUBREG_REG (op);
      if (! REG_P (op))
	{
	  mark_pseudos (op, out_p);
	  return;
	}
      /* Fall through:  */

    case REG:
      if ((regno = REGNO (op)) >= FIRST_PSEUDO_REGISTER
	  && bitmap_bit_p (consideration_pseudos, regno))
	{
	  gcc_assert (referenced_pseudos_num < MAX_INSN_PSEUDOS);
	  if (out_p)
	    modified_pseudos[modified_pseudos_num++] = regno;
	  referenced_pseudos[referenced_pseudos_num++] = regno;
	}
      return;
     
    case PRE_INC:
    case POST_INC:
    case PRE_DEC:
    case POST_DEC:
      mark_pseudos (XEXP (op, 0), true);
      return;

    case PRE_MODIFY:
    case POST_MODIFY:
      mark_pseudos (XEXP (op, 0), true);
      mark_pseudos (XEXP (op, 1), false);
      return;
    default:
      fmt = GET_RTX_FORMAT (code);
      for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
	{
	  if (fmt[i] == 'e')
	    mark_pseudos (XEXP (op, i), false);
	  else if (fmt[i] == 'E')
	    for (j = XVECLEN (op, i) - 1; j >= 0; j--)
	      mark_pseudos (XVECEXP (op, i, j), false);
	}
    }
}

/* Process INSN and record all referenced registers in
   REFERENCED_PSEUDOS and modified registers in MODIFIED_PSEUDOS.
   Only pseudos in CONSIDERATION_PSEUDOS are considered.  */
static void
mark_insn_pseudos (rtx insn)
{
  int i;
  lra_insn_recog_data_t id;
  struct lra_static_insn_data *static_id;

  modified_pseudos_num = referenced_pseudos_num = 0;
  id = lra_get_insn_recog_data (insn);
  static_id = id->insn_static_data;
  for (i = static_id->n_operands - 1; i >= 0; i--)
    mark_pseudos (*id->operand_loc[i], static_id->operand[i].type != OP_IN);
}

/* Process storing of REG to update live info.  */
static void
mark_pseudo_store (rtx reg, const_rtx setter ATTRIBUTE_UNUSED,
		   void *data ATTRIBUTE_UNUSED)
{
  int regno;

  if (GET_CODE (reg) == SUBREG)
    reg = SUBREG_REG (reg);

  if (! REG_P (reg))
    return;

  regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER
      && bitmap_bit_p (consideration_pseudos, regno))
    bitmap_set_bit (live_pseudos, regno);
}

/* Info about save/restore code for a pseudo.  */
struct save_regs
{
  /* True if MEM_REG is actually an equivalence of the corresponding
     saved pseudo.  */
  bool equiv_p;
  /* A spilled pseudo or equivalence which hold value of the
     corresponding saved/restored pseudo around calls.  */
  rtx saved_value;
  /* Saving/restoring of a pseudo could be done in a mode different
     from the pseudo mode.  The following is the pseudo or a subreg of
     the pseudo and is used in save/restore code.  */
  rtx saved_reg;
  /* Last insn in EBB referencing the pseudo.  */
  rtx last_referencing_insn;
};

/* Map: regno -> info about save/restore for REGNO.  */
static struct save_regs *save_regs;

/* Return equivalence value we can use for restoring.  NULL,
   otherwise.  ??? Should we use and is it worth to invariants with
   caller saved hard registers.  */
static rtx
equiv_for_save (int regno)
{
  return ira_reg_equiv[regno].constant;
}

/* Set to true if changes in live info are too complex to update it
   here.  */
static bool update_live_info_p;

/* Set up if necessary equiv_p, saved_value, and saved_reg for REGNO.  */
static void
setup_save_regs (int regno)
{
  if (save_regs[regno].saved_value == NULL_RTX)
    {
      enum machine_mode mode;
      int hard_regno;
      rtx equiv, saved_reg = regno_reg_rtx[regno];

      equiv = ira_reg_equiv[regno].defined_p ? equiv_for_save (regno) : NULL_RTX;
      if (equiv != NULL_RTX)
	{
	  save_regs[regno].saved_value = equiv;
	  update_live_info_p = true;
	}
      else
	{
	  hard_regno = reg_renumber[regno];
	  gcc_assert (hard_regno >= 0);
	  mode = (HARD_REGNO_CALLER_SAVE_MODE
		  (hard_regno,
		   hard_regno_nregs[hard_regno][PSEUDO_REGNO_MODE (regno)],
		   PSEUDO_REGNO_MODE (regno)));
	  if (mode != PSEUDO_REGNO_MODE (regno))
	    saved_reg = gen_rtx_SUBREG (mode, saved_reg, 0);
	  save_regs[regno].saved_value
	    = lra_create_new_reg (VOIDmode, saved_reg, NO_REGS, NULL);
	}
      save_regs[regno].saved_reg = saved_reg;
      save_regs[regno].equiv_p = equiv != NULL_RTX;
    }
}

/* Insert save (if SAVE_P) or restore code for REGNO before (if
   BEFORE_P) or after INSN.  Use equivalence for restoring if it is
   possible.  */
static void
insert_save_restore (rtx insn, bool before_p, int regno, bool save_p)
{
  rtx x, saved_value;
  int to_regno, from_regno;
  rtx saved_reg;

  setup_save_regs (regno);
  saved_value = save_regs[regno].saved_value;
  saved_reg = save_regs[regno].saved_reg;
  if (save_regs[regno].equiv_p)
    {
      if (save_p)
	/* Do nothing -- the pseudo always holds the same value.  */
	return;
      start_sequence ();
      emit_move_insn (saved_reg, saved_value);
      x = get_insns ();
      end_sequence ();
      if (lra_dump_file != NULL)
	{
	  fprintf (lra_dump_file, "Inserting %s i%u bb%d\n",
		   before_p ? "before" : "after", INSN_UID (insn),
		   BLOCK_FOR_INSN (insn)->index);
	  print_rtl_slim (lra_dump_file, x, x, -1, 0);
	}
      ira_reg_equiv[regno].init_insns
	= gen_rtx_INSN_LIST (VOIDmode, x, ira_reg_equiv[regno].init_insns);
      if (before_p)
	emit_insn_before (x, insn); 
      else
	emit_insn_after (x, insn); 
      return;
    }
  start_sequence ();
  if (save_p)
    {
      gcc_assert (REG_P (saved_value));
      to_regno = REGNO (saved_value);
      from_regno = regno;
      emit_move_insn (saved_value, saved_reg);
    }
  else
    {
      to_regno = regno;
      from_regno = REGNO (saved_value);
      emit_move_insn (saved_reg, saved_value);
    }
  x = get_insns ();
  end_sequence ();
  if (before_p || ! save_p || ! CALL_P (insn))
    {
      if (! update_live_info_p)
	add_reg_note (x, REG_DEAD, regno_reg_rtx[from_regno]);
    }
  else
    {
      /* A special case: saving a pseudo used in a call.  Put saving
	 insn before the call and attach the note to the call.  */
      before_p = true;
      if (! update_live_info_p)
	add_reg_note (insn, REG_DEAD, regno_reg_rtx[from_regno]);
    }
  ira_update_equiv_info_by_shuffle_insn (to_regno, from_regno, x);
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file, "Inserting insn %u %d:=%d %s i%u bb%d\n",
	     INSN_UID (x),
	     save_p ? (int) REGNO (saved_value) : regno,
	     save_p ? regno : (int) REGNO (saved_value),
	     before_p ? "before" : "after", INSN_UID (insn),
	     BLOCK_FOR_INSN (insn)->index);
  if (before_p)
    emit_insn_before (x, insn); 
  else
    emit_insn_after (x, insn); 
}

/* Insert save/restore code usinge solution of the data-flow
   equations.  */
static void
insert_saves_restores (void)
{
  int i;
  unsigned int regno;
  rtx insn, curr, link, first;
  rtx where = NULL_RTX;
  basic_block bb, prev_bb, start_bb, end_bb;
  edge e;
  edge_iterator ei;
  bitmap pseudos_saved, pseudos_to_restore;
  reg_set_iterator rsi;
  
  pseudos_saved = BITMAP_ALLOC (&reg_obstack);
  pseudos_to_restore = BITMAP_ALLOC (&reg_obstack);
  FOR_EACH_BB (bb)
    {
      /* Find EBB.  */
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "EBB");
      /* Form a EBB starting with BB.  */
      for (start_bb = bb;; bb = bb->next_bb)
	{
	  end_bb = bb;
	  if (lra_dump_file != NULL)
	    fprintf (lra_dump_file, " %d", bb->index);
	  if (bb->next_bb == EXIT_BLOCK_PTR || LABEL_P (BB_HEAD (bb->next_bb)))
	    break;
	  e = find_fallthru_edge (bb->succs);
	  if (! e)
	    break;
	  /* We are not interesting in frequencies, we need the
	     longest EBB because the farther BB in EBB, the smaller
	     execution frequency of restore insns.  */
	}
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "\n");
      
      bitmap_clear (pseudos_saved);
      bitmap_and (live_pseudos, DF_LR_IN (start_bb), consideration_pseudos);

      for (bb = start_bb;;)
	{
	  first = NULL_RTX;
	  FOR_BB_INSNS (bb, insn)
	    {
	      if (NONDEBUG_INSN_P (insn))
		break;
	      first = insn;
	    }
	  EXECUTE_IF_AND_COMPL_IN_BITMAP (live_pseudos, pseudos_saved,
					  FIRST_PSEUDO_REGISTER, regno, rsi)
	    save_regs[regno].last_referencing_insn = first;

	  FOR_BB_INSNS_SAFE (bb, insn, curr)
	    {
	      enum rtx_code code = GET_CODE (insn);
	      
	      if (! NONDEBUG_INSN_P (insn))
		continue;
	      
	      mark_insn_pseudos (insn);
	      for (i = 0; i < referenced_pseudos_num; i++)
		{
		  regno = referenced_pseudos[i];
		  if (bitmap_bit_p (pseudos_saved, regno)
		      && bitmap_bit_p (live_pseudos, regno))
		    {
		      insert_save_restore
			(save_regs[regno].last_referencing_insn,
			 false, regno, true);
		      insert_save_restore (insn, true, regno, false);
		      bitmap_clear_bit (pseudos_saved, regno);
		    }
		  save_regs[regno].last_referencing_insn = insn;
		}
	      for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
		if (REG_NOTE_KIND (link) == REG_DEAD)
		  bitmap_clear_bit (live_pseudos, REGNO (XEXP (link, 0)));
	      
	      if (code == CALL_INSN && ! SIBLING_CALL_P (insn)
		  && ! find_reg_note (insn, REG_NORETURN, NULL))
		bitmap_ior_into (pseudos_saved, live_pseudos);
	      
	      /* Mark any registers set in INSN as live.  */
	      note_stores (PATTERN (insn), mark_pseudo_store, NULL);
	      
#ifdef AUTO_INC_DEC
	      for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
		if (REG_NOTE_KIND (link) == REG_INC)
		  mark_pseudo_store (XEXP (link, 0), NULL_RTX, NULL);
#endif
	      for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
		if (REG_NOTE_KIND (link) == REG_UNUSED)
		  bitmap_clear_bit (live_pseudos, REGNO (XEXP (link, 0)));
	      
	      where = insn;
	    }
	  /* At the end of the basic block, we must restore any
	     registers living at all non-fall through BB.  */
	  if (bb == end_bb)
	    bitmap_copy (pseudos_to_restore, pseudos_saved);
	  else
	    {
	      bitmap_clear (pseudos_to_restore);
	      FOR_EACH_EDGE (e, ei, bb->succs)
		if (e->dest != bb->next_bb)
		  bitmap_ior_and_into (pseudos_to_restore,
				       DF_LR_IN (e->dest), pseudos_saved);
	    }
	      
	  EXECUTE_IF_SET_IN_REG_SET (pseudos_to_restore,
				     FIRST_PSEUDO_REGISTER, regno, rsi)
	    {
	      insert_save_restore (save_regs[regno].last_referencing_insn,
				   false, regno, true);
	      insert_save_restore (where, JUMP_P (where), regno, false);
	      bitmap_clear_bit (pseudos_saved, regno);
	    }
	  if (bb == end_bb)
	    break;
	  prev_bb = bb;
	  bb = bb->next_bb;
	  bitmap_and (live_pseudos, DF_LR_IN (bb), consideration_pseudos);
	  EXECUTE_IF_SET_IN_REG_SET (pseudos_saved,
				     FIRST_PSEUDO_REGISTER, regno, rsi)
	    {
	      bitmap_clear_bit (DF_LR_IN (bb), regno);
	      bitmap_clear_bit (DF_LR_OUT (prev_bb), regno);
	      setup_save_regs (regno);
	      if (update_live_info_p)
		break;
	      bitmap_set_bit (DF_LR_IN (bb),
			      REGNO (save_regs[regno].saved_value));
	      bitmap_set_bit (DF_LR_OUT (prev_bb),
			      REGNO (save_regs[regno].saved_value));
	    }
	}
    }
  BITMAP_FREE (pseudos_to_restore);
  BITMAP_FREE (pseudos_saved);
}

/* Major function to insert save/restore code.  The function needs
   correct DFA info and REG_N_REFS & REG_N_CALLS_CROSSED before the
   function work.  It keeps correct bb live info and live related insn
   notes.  Return true if we need to update live info because the
   changes are too complex to do it here.  */
bool
lra_save_restore (void)
{
  int i, hard_regno, max_regno_before;

  max_regno_before = max_reg_num ();

  if (lra_dump_file != NULL)
    fprintf (lra_dump_file, "\n********** Caller saves: **********\n\n");

  update_live_info_p = false;
  consideration_pseudos = BITMAP_ALLOC (&reg_obstack);
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno_before; i++)
    if (REG_N_REFS (i) != 0
	&& (hard_regno = lra_get_regno_hard_regno (i)) >= 0
	&& REG_N_CALLS_CROSSED (i) != 0
	&& lra_hard_reg_set_intersection_p (hard_regno, PSEUDO_REGNO_MODE (i),
					    call_used_reg_set))
      bitmap_set_bit (consideration_pseudos, i);
  if (! bitmap_empty_p (consideration_pseudos))
    {
      live_pseudos = BITMAP_ALLOC (&reg_obstack);
      save_regs = (struct save_regs *) xmalloc (sizeof (struct save_regs)
						* max_regno_before);
      memset (save_regs, 0, sizeof (struct save_regs) * max_regno_before);
      insert_saves_restores ();
      free (save_regs);
      BITMAP_FREE (live_pseudos);
    }
  BITMAP_FREE (consideration_pseudos);
  return update_live_info_p;
}
