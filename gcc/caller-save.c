/* Save and restore call-clobbered registers which are live across a call.
   Copyright (C) 1989, 1992, 1994, 1995, 1997, 1998,
   1999, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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
#include "regs.h"
#include "insn-config.h"
#include "flags.h"
#include "hard-reg-set.h"
#include "recog.h"
#include "basic-block.h"
#include "reload.h"
#include "function.h"
#include "expr.h"
#include "toplev.h"
#include "tm_p.h"
#include "addresses.h"
#include "output.h"
#include "ira.h"

/* Call used hard registers which can not be saved because there is no
   insn for this.  */
HARD_REG_SET no_caller_save_reg_set;

#ifndef MAX_MOVE_MAX
#define MAX_MOVE_MAX MOVE_MAX
#endif

#ifndef MIN_UNITS_PER_WORD
#define MIN_UNITS_PER_WORD UNITS_PER_WORD
#endif

#define MOVE_MAX_WORDS (MOVE_MAX / UNITS_PER_WORD)

/* Modes for each hard register that we can save.  The smallest mode is wide
   enough to save the entire contents of the register.  When saving the
   register because it is live we first try to save in multi-register modes.
   If that is not possible the save is done one register at a time.  */

static enum machine_mode
  regno_save_mode[FIRST_PSEUDO_REGISTER][MAX_MOVE_MAX / MIN_UNITS_PER_WORD + 1];

/* For each hard register, a place on the stack where it can be saved,
   if needed.  */

static rtx
  regno_save_mem[FIRST_PSEUDO_REGISTER][MAX_MOVE_MAX / MIN_UNITS_PER_WORD + 1];

/* We will only make a register eligible for caller-save if it can be
   saved in its widest mode with a simple SET insn as long as the memory
   address is valid.  We record the INSN_CODE is those insns here since
   when we emit them, the addresses might not be valid, so they might not
   be recognized.  */

static int
  reg_save_code[FIRST_PSEUDO_REGISTER][MAX_MACHINE_MODE];
static int
  reg_restore_code[FIRST_PSEUDO_REGISTER][MAX_MACHINE_MODE];

/* Set of hard regs currently residing in save area (during insn scan).  */

static HARD_REG_SET hard_regs_saved;

/* Number of registers currently in hard_regs_saved.  */

static int n_regs_saved;

/* Computed by mark_referenced_regs, all regs referenced in a given
   insn.  */
static HARD_REG_SET referenced_regs;


static void mark_set_regs (rtx, rtx, void *);
static void mark_referenced_regs (rtx);
static int insert_save (struct insn_chain *, int, int, HARD_REG_SET *,
			enum machine_mode *);
static int insert_restore (struct insn_chain *, int, int, int,
			   enum machine_mode *);
static struct insn_chain *insert_one_insn (struct insn_chain *, int, int,
					   rtx);
static void add_stored_regs (rtx, rtx, void *);

/* Initialize for caller-save.

   Look at all the hard registers that are used by a call and for which
   regclass.c has not already excluded from being used across a call.

   Ensure that we can find a mode to save the register and that there is a
   simple insn to save and restore the register.  This latter check avoids
   problems that would occur if we tried to save the MQ register of some
   machines directly into memory.  */

void
init_caller_save (void)
{
  rtx addr_reg;
  int offset;
  rtx address;
  int i, j;
  enum machine_mode mode;
  rtx savepat, restpat;
  rtx test_reg, test_mem;
  rtx saveinsn, restinsn;

  CLEAR_HARD_REG_SET (no_caller_save_reg_set);
  /* First find all the registers that we need to deal with and all
     the modes that they can have.  If we can't find a mode to use,
     we can't have the register live over calls.  */

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (call_used_regs[i] && ! call_fixed_regs[i])
	{
	  for (j = 1; j <= MOVE_MAX_WORDS; j++)
	    {
	      regno_save_mode[i][j] = HARD_REGNO_CALLER_SAVE_MODE (i, j,
								   VOIDmode);
	      if (regno_save_mode[i][j] == VOIDmode && j == 1)
		{
		  call_fixed_regs[i] = 1;
		  SET_HARD_REG_BIT (call_fixed_reg_set, i);
		}
	    }
	}
      else
	regno_save_mode[i][1] = VOIDmode;
    }

  /* The following code tries to approximate the conditions under which
     we can easily save and restore a register without scratch registers or
     other complexities.  It will usually work, except under conditions where
     the validity of an insn operand is dependent on the address offset.
     No such cases are currently known.

     We first find a typical offset from some BASE_REG_CLASS register.
     This address is chosen by finding the first register in the class
     and by finding the smallest power of two that is a valid offset from
     that register in every mode we will use to save registers.  */

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT
	(reg_class_contents
	 [(int) base_reg_class (regno_save_mode [i][1], PLUS, CONST_INT)], i))
      break;

  gcc_assert (i < FIRST_PSEUDO_REGISTER);

  addr_reg = gen_rtx_REG (Pmode, i);

  for (offset = 1 << (HOST_BITS_PER_INT / 2); offset; offset >>= 1)
    {
      address = gen_rtx_PLUS (Pmode, addr_reg, GEN_INT (offset));

      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if (regno_save_mode[i][1] != VOIDmode
	  && ! strict_memory_address_p (regno_save_mode[i][1], address))
	  break;

      if (i == FIRST_PSEUDO_REGISTER)
	break;
    }

  /* If we didn't find a valid address, we must use register indirect.  */
  if (offset == 0)
    address = addr_reg;

  /* Next we try to form an insn to save and restore the register.  We
     see if such an insn is recognized and meets its constraints.

     To avoid lots of unnecessary RTL allocation, we construct all the RTL
     once, then modify the memory and register operands in-place.  */

  test_reg = gen_rtx_REG (VOIDmode, 0);
  test_mem = gen_rtx_MEM (VOIDmode, address);
  savepat = gen_rtx_SET (VOIDmode, test_mem, test_reg);
  restpat = gen_rtx_SET (VOIDmode, test_reg, test_mem);

  saveinsn = gen_rtx_INSN (VOIDmode, 0, 0, 0, 0, 0, savepat, -1, 0, 0);
  restinsn = gen_rtx_INSN (VOIDmode, 0, 0, 0, 0, 0, restpat, -1, 0, 0);

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    for (mode = 0 ; mode < MAX_MACHINE_MODE; mode++)
      if (HARD_REGNO_MODE_OK (i, mode))
	{
	  int ok;

	  /* Update the register number and modes of the register
	     and memory operand.  */
	  REGNO (test_reg) = i;
	  PUT_MODE (test_reg, mode);
	  PUT_MODE (test_mem, mode);

	  /* Force re-recognition of the modified insns.  */
	  INSN_CODE (saveinsn) = -1;
	  INSN_CODE (restinsn) = -1;

	  reg_save_code[i][mode] = recog_memoized (saveinsn);
	  reg_restore_code[i][mode] = recog_memoized (restinsn);

	  /* Now extract both insns and see if we can meet their
	     constraints.  */
	  ok = (reg_save_code[i][mode] != -1
		&& reg_restore_code[i][mode] != -1);
	  if (ok)
	    {
	      extract_insn (saveinsn);
	      ok = constrain_operands (1);
	      extract_insn (restinsn);
	      ok &= constrain_operands (1);
	    }

	  if (! ok)
	    {
	      reg_save_code[i][mode] = -1;
	      reg_restore_code[i][mode] = -1;
	    }
	}
      else
	{
	  reg_save_code[i][mode] = -1;
	  reg_restore_code[i][mode] = -1;
	}

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    for (j = 1; j <= MOVE_MAX_WORDS; j++)
      if (reg_save_code [i][regno_save_mode[i][j]] == -1)
	{
	  regno_save_mode[i][j] = VOIDmode;
	  if (j == 1)
	    {
	      call_fixed_regs[i] = 1;
	      SET_HARD_REG_BIT (call_fixed_reg_set, i);
	      if (call_used_regs[i])
		SET_HARD_REG_BIT (no_caller_save_reg_set, i);
	    }
	}
}



/* Initialize save areas by showing that we haven't allocated any yet.  */

void
init_save_areas (void)
{
  int i, j;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    for (j = 1; j <= MOVE_MAX_WORDS; j++)
      regno_save_mem[i][j] = 0;
}

/* The structure represents a hard register which should be saved
   through the call.  */
struct saved_hard_reg
{
  /* Order number starting with 0.  */
  int num;
  /* The hard regno.  */
  int hard_regno;
  /* Execution frequency of all calls through which given hard
     register should be saved.  */
  int call_freq;
  /* Stack slot reserved to save the hard register through calls.  */
  rtx slot;
  /* True if it is first hard register in the chain of hard registers
     sharing the same stack slot.  */
  int first_p;
  /* Order number of the next hard register with the same slot in the
     chain.  -1 represents end of the chain.  */
  int next;
};

/* Map: hard register number to the corresponding structure.  */
static struct saved_hard_reg *hard_reg_map [FIRST_PSEUDO_REGISTER];

/* The number of all structures representing hard register should be
   saved.  */
static int saved_regs_num;

/* Pointers to all the structures.  Index is the order number of the
   structure.  */
static struct saved_hard_reg *all_saved_regs [FIRST_PSEUDO_REGISTER];

/* First called function for work with saved hard registers.  */
static void
initiate_saved_hard_regs (void)
{
  int i;

  saved_regs_num = 0;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    hard_reg_map [i] = NULL;
}

/* Allocate and return new saved hard register with given REGNO and
   CALL_FREQ.  */
static struct saved_hard_reg *
new_saved_hard_reg (int regno, int call_freq)
{
  struct saved_hard_reg *saved_reg;

  saved_reg = xmalloc (sizeof (struct saved_hard_reg));
  hard_reg_map [regno] = all_saved_regs [saved_regs_num] = saved_reg;
  saved_reg->num = saved_regs_num++;
  saved_reg->hard_regno = regno;
  saved_reg->call_freq = call_freq;
  saved_reg->first_p = FALSE;
  saved_reg->next = -1;
  return saved_reg;
}

/* Free memory allocated for the saved hard registers.  */
static void
finish_saved_hard_regs (void)
{
  int i;

  for (i = 0; i < saved_regs_num; i++)
    free (all_saved_regs [i]);
}

/* The function is used to sort the saved hard registers according
   their frequency.  */
static int
saved_hard_reg_compare_func (const void *v1p, const void *v2p)
{
  struct saved_hard_reg *p1 = *(struct saved_hard_reg **) v1p;
  struct saved_hard_reg *p2 = *(struct saved_hard_reg **) v2p;
  
  if (flag_omit_frame_pointer)
    {
      if (p1->call_freq - p2->call_freq != 0)
	return p1->call_freq - p2->call_freq;
    }
  else if (p2->call_freq - p1->call_freq != 0)
    return p2->call_freq - p1->call_freq;

  return p1->num - p2->num;
}

/* Allocate save areas for any hard registers that might need saving.
   We take a conservative approach here and look for call-clobbered hard
   registers that are assigned to pseudos that cross calls.  This may
   overestimate slightly (especially if some of these registers are later
   used as spill registers), but it should not be significant.

   For IRA we use priority coloring to decrease stack slots needed for
   saving hard registers through calls.

   Future work:

     In the fallback case we should iterate backwards across all possible
     modes for the save, choosing the largest available one instead of
     falling back to the smallest mode immediately.  (eg TF -> DF -> SF).

     We do not try to use "move multiple" instructions that exist
     on some machines (such as the 68k moveml).  It could be a win to try
     and use them when possible.  The hard part is doing it in a way that is
     machine independent since they might be saving non-consecutive
     registers. (imagine caller-saving d0,d1,a0,a1 on the 68k) */

void
setup_save_areas (void)
{
  int i, j, k;
  unsigned int r;
  HARD_REG_SET hard_regs_used;

  /* Allocate space in the save area for the largest multi-register
     pseudos first, then work backwards to single register
     pseudos.  */

  /* Find and record all call-used hard-registers in this function.  */
  CLEAR_HARD_REG_SET (hard_regs_used);
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    if (reg_renumber[i] >= 0 && REG_N_CALLS_CROSSED (i) > 0)
      {
	unsigned int regno = reg_renumber[i];
	unsigned int endregno
	  = regno + hard_regno_nregs[regno][GET_MODE (regno_reg_rtx[i])];
	if (flag_ira && flag_ipra)
	  {
	    HARD_REG_SET clobbered_regs;
	    
	    collect_pseudo_call_clobbered_regs (i, &clobbered_regs);
	    for (r = regno; r < endregno; r++)
	      if (TEST_HARD_REG_BIT (clobbered_regs, r))
		SET_HARD_REG_BIT (hard_regs_used, r);
	  }
	else
	  for (r = regno; r < endregno; r++)
	    if (call_used_regs[r])
	      SET_HARD_REG_BIT (hard_regs_used, r);
      }

  if (flag_ira)
    {
      rtx insn, slot;
      struct insn_chain *chain, *next;
      char *saved_reg_conflicts;
      unsigned int regno;
      int next_k, freq;
      struct saved_hard_reg *saved_reg, *saved_reg2, *saved_reg3;
      int call_saved_regs_num;
      struct saved_hard_reg *call_saved_regs [FIRST_PSEUDO_REGISTER];
      HARD_REG_SET hard_regs_to_save, used_regs, this_insn_sets;
      reg_set_iterator rsi;
      
      initiate_saved_hard_regs ();
      /* Create hard reg saved regs.  */
      for (chain = reload_insn_chain; chain != 0; chain = next)
	{
	  insn = chain->insn;
	  next = chain->next;
	  if (GET_CODE (insn) != CALL_INSN
	      || find_reg_note (insn, REG_NORETURN, NULL))
	    continue;
	  freq = REG_FREQ_FROM_BB (BLOCK_FOR_INSN (insn));
	  REG_SET_TO_HARD_REG_SET (hard_regs_to_save,
				   &chain->live_throughout);
	  get_call_invalidated_used_regs (insn, &used_regs, false);

	  /* Record all registers set in this call insn.  These don't
	     need to be saved.  N.B. the call insn might set a subreg
	     of a multi-hard-reg pseudo; then the pseudo is considered
	     live during the call, but the subreg that is set
	     isn't.  */
	  CLEAR_HARD_REG_SET (this_insn_sets);
	  note_stores (PATTERN (insn), mark_set_regs, &this_insn_sets);
	  /* Sibcalls are considered to set the return value,
	     compare flow.c:propagate_one_insn.  */
	  if (SIBLING_CALL_P (insn) && current_function_return_rtx)
	    mark_set_regs (current_function_return_rtx, NULL_RTX,
			   &this_insn_sets);

	  AND_COMPL_HARD_REG_SET (used_regs, call_fixed_reg_set);
	  AND_COMPL_HARD_REG_SET (used_regs, this_insn_sets);
	  AND_HARD_REG_SET (hard_regs_to_save, used_regs);
	  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
	    if (TEST_HARD_REG_BIT (hard_regs_to_save, regno))
	      {
		if (hard_reg_map [regno] != NULL)
		  hard_reg_map [regno]->call_freq += freq;
		else
		  saved_reg = new_saved_hard_reg (regno, freq);
	      }
	  /* Look through all live pseudos, mark their hard registers.  */
	  EXECUTE_IF_SET_IN_REG_SET
	    (&chain->live_throughout, FIRST_PSEUDO_REGISTER, regno, rsi)
	    {
	      int r = reg_renumber [regno];
	      int bound;
	      
	      if (r < 0)
		continue;
	      
	      bound = r + hard_regno_nregs [r] [PSEUDO_REGNO_MODE (regno)];
	      for (; r < bound; r++)
		if (TEST_HARD_REG_BIT (used_regs, r))
		  {
		    if (hard_reg_map [r] != NULL)
		      hard_reg_map [r]->call_freq += freq;
		    else
		      saved_reg = new_saved_hard_reg (r, freq);
		  }
	    }
	}
      /* Find saved hard register conflicts.  */
      saved_reg_conflicts = xmalloc (saved_regs_num * saved_regs_num);
      memset (saved_reg_conflicts, 0, saved_regs_num * saved_regs_num);
      for (chain = reload_insn_chain; chain != 0; chain = next)
	{
	  call_saved_regs_num = 0;
	  insn = chain->insn;
	  next = chain->next;
	  if (GET_CODE (insn) != CALL_INSN
	      || find_reg_note (insn, REG_NORETURN, NULL))
	    continue;
	  REG_SET_TO_HARD_REG_SET (hard_regs_to_save,
				   &chain->live_throughout);
	  get_call_invalidated_used_regs (insn, &used_regs, false);

	  /* Record all registers set in this call insn.  These don't
	     need to be saved.  N.B. the call insn might set a subreg
	     of a multi-hard-reg pseudo; then the pseudo is considered
	     live during the call, but the subreg that is set
	     isn't.  */
	  CLEAR_HARD_REG_SET (this_insn_sets);
	  note_stores (PATTERN (insn), mark_set_regs, &this_insn_sets);
	  /* Sibcalls are considered to set the return value,
	     compare flow.c:propagate_one_insn.  */
	  if (SIBLING_CALL_P (insn) && current_function_return_rtx)
	    mark_set_regs (current_function_return_rtx, NULL_RTX,
			   &this_insn_sets);

	  AND_COMPL_HARD_REG_SET (used_regs, call_fixed_reg_set);
	  AND_COMPL_HARD_REG_SET (used_regs, this_insn_sets);
	  AND_HARD_REG_SET (hard_regs_to_save, used_regs);
	  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
	    if (TEST_HARD_REG_BIT (hard_regs_to_save, regno))
	      {
		gcc_assert (hard_reg_map [regno] != NULL);
		call_saved_regs [call_saved_regs_num++] = hard_reg_map [regno];
	      }
	  /* Look through all live pseudos, mark their hard registers.  */
	  EXECUTE_IF_SET_IN_REG_SET
	    (&chain->live_throughout, FIRST_PSEUDO_REGISTER, regno, rsi)
	    {
	      int r = reg_renumber[regno];
	      int bound;
	      
	      if (r < 0)
		continue;
	      bound = r + hard_regno_nregs [r] [PSEUDO_REGNO_MODE (regno)];
	      for (; r < bound; r++)
		if (TEST_HARD_REG_BIT (used_regs, r))
		  call_saved_regs [call_saved_regs_num++] = hard_reg_map [r];
	    }
	  for (i = 0; i < call_saved_regs_num; i++)
	    {
	      saved_reg = call_saved_regs [i];
	      for (j = 0; j < call_saved_regs_num; j++)
		if (i != j)
		  {
		    saved_reg2 = call_saved_regs [j];
		    saved_reg_conflicts [saved_reg->num * saved_regs_num
					 + saved_reg2->num]
		      = saved_reg_conflicts [saved_reg2->num * saved_regs_num
					     + saved_reg->num]
		      = TRUE;
		  }
	    }
	}
      /* Sort saved hard regs.  */
      qsort (all_saved_regs, saved_regs_num, sizeof (struct saved_hard_reg *),
	     saved_hard_reg_compare_func);
      /* Allocate stack slots for the saved hard registers.  */
      for (i = 0; i < saved_regs_num; i++)
	{
	  saved_reg = all_saved_regs [i];
	  for (j = 0; j < i; j++)
	    {
	      saved_reg2 = all_saved_regs [j];
	      if (! saved_reg2->first_p)
		continue;
	      slot = saved_reg2->slot;
	      for (k = j; k >= 0; k = next_k)
		{
		  saved_reg3 = all_saved_regs [k];
		  next_k = saved_reg3->next;
		  if (saved_reg_conflicts [saved_reg->num * saved_regs_num
					   + saved_reg3->num])
		    break;
		}
	      if (k < 0
		  && (GET_MODE_SIZE (regno_save_mode
				     [saved_reg->hard_regno] [1])
		      <= GET_MODE_SIZE (regno_save_mode
					[saved_reg2->hard_regno] [1])))
		{
		  saved_reg->slot = slot;
		  regno_save_mem [saved_reg->hard_regno] [1] = slot;
		  saved_reg->next = saved_reg2->next;
		  saved_reg2->next = i;
		  if (dump_file != NULL)
		    fprintf (dump_file, "%d uses slot of %d\n",
			     saved_reg->hard_regno, saved_reg2->hard_regno);
		  break;
		}
	    }
	  if (j == i)
	    {
	      saved_reg->first_p = TRUE;
	      if (regno_save_mem [saved_reg->hard_regno] [1] != NULL_RTX)
		{
		  saved_reg->slot = regno_save_mem [saved_reg->hard_regno] [1];
		  if (dump_file != NULL)
		    fprintf (dump_file,
			     "%d uses slot from prev iteration\n",
			     saved_reg->hard_regno);
		}
	      else
		{
		  saved_reg->slot
		    = assign_stack_local
		      (regno_save_mode [saved_reg->hard_regno] [1],
		       GET_MODE_SIZE (regno_save_mode
				      [saved_reg->hard_regno] [1]), 0);
		  regno_save_mem [saved_reg->hard_regno] [1] = saved_reg->slot;
		  if (dump_file != NULL)
		    fprintf (dump_file, "%d uses a new slot\n",
			     saved_reg->hard_regno);
		}
	    }
	}
      free (saved_reg_conflicts);
      finish_saved_hard_regs ();
    }
  else
    {
      /* Now run through all the call-used hard-registers and allocate
	 space for them in the caller-save area.  Try to allocate space
	 in a manner which allows multi-register saves/restores to be done.  */
      
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	for (j = MOVE_MAX_WORDS; j > 0; j--)
	  {
	    int do_save = 1;
	    
	    /* If no mode exists for this size, try another.  Also break out
	       if we have already saved this hard register.  */
	    if (regno_save_mode[i][j] == VOIDmode || regno_save_mem[i][1] != 0)
	      continue;
	    
	    /* See if any register in this group has been saved.  */
	    for (k = 0; k < j; k++)
	      if (regno_save_mem[i + k][1])
		{
		  do_save = 0;
		  break;
		}
	    if (! do_save)
	      continue;
	    
	    for (k = 0; k < j; k++)
	      if (! TEST_HARD_REG_BIT (hard_regs_used, i + k))
		{
		  do_save = 0;
		  break;
		}
	    if (! do_save)
	      continue;
	    
	    /* We have found an acceptable mode to store in.  */
	    regno_save_mem[i][j]
	      = assign_stack_local (regno_save_mode[i][j],
				    GET_MODE_SIZE (regno_save_mode[i][j]), 0);
	    
	    /* Setup single word save area just in case...  */
	    for (k = 0; k < j; k++)
	      /* This should not depend on WORDS_BIG_ENDIAN.
		 The order of words in regs is the same as in memory.  */
	      regno_save_mem[i + k][1]
		= adjust_address_nv (regno_save_mem[i][j],
				     regno_save_mode[i + k][1],
				     k * UNITS_PER_WORD);
	  }
    }

  /* Now loop again and set the alias set of any save areas we made to
     the alias set used to represent frame objects.  */
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    for (j = MOVE_MAX_WORDS; j > 0; j--)
      if (regno_save_mem[i][j] != 0)
	set_mem_alias_set (regno_save_mem[i][j], get_frame_alias_set ());
}


/* The following structure contains basic block data flow information
   used to calculate hard registers to save at BB ends.  Data flow
   equation is bidirectional because we don't want to put save/restore
   code on CFG edges.  */
struct bb_info
{
  /* The basic block reverse post-order number.  */
  int rts_number;
  /* True if save_in should empty for this block.  */
  int empty_save_in_p;
  /* Hard registers correspondingly used (or set) and should be saved but
     not used (or used) afterward in the basic block.  */
  HARD_REG_SET kill, gen;
  /* Registers needed to be saved at the start and end of the basic
     block.  */
  HARD_REG_SET save_in, save_out;
  /* Hard registers living at the start of the basic block.  */
  HARD_REG_SET live_at_start;
  /* We don't want to generate save/restore insns on edges because it
     changes CFG during reload.  To prevent this we use the following
     set.  This set defines what hard registers should be saved at the
     end of basic block. */
  HARD_REG_SET save_here;
  /* Saving modes at the start and end of the basic block.  */
  unsigned char save_in_mode [FIRST_PSEUDO_REGISTER];
  /* It corresponds to set GEN right after the call of
     calculate_local_save_info.  */
  unsigned char save_out_mode [FIRST_PSEUDO_REGISTER];
};

/* Macros for accessing data flow information of basic blocks.  */
#define BB_INFO(BB) ((struct bb_info *) (BB)->aux)
#define BB_INFO_BY_INDEX(N) BB_INFO (BASIC_BLOCK(N))

/* The function calculates sets KILL, GEN, LIVE_AT_START and
   SAVE_OUT_MODES corresponding to GEN for basic blocks.  */
static void
calculate_local_save_info (void)
{
  int i, empty_save_in_p;
  struct insn_chain *chain, *next;
  struct bb_info *bb_info;
  /* Computed in mark_set_regs, holds all registers set by the current
     instruction.  */
  HARD_REG_SET this_insn_sets, gen, kill;
  unsigned regno;
  reg_set_iterator rsi;
  enum machine_mode save_mode [FIRST_PSEUDO_REGISTER];

  CLEAR_HARD_REG_SET (gen);
  CLEAR_HARD_REG_SET (kill);
  empty_save_in_p = FALSE;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    save_mode [i] = VOIDmode;

  for (chain = reload_insn_chain; chain != 0; chain = next)
    {
      rtx insn = chain->insn;
      enum rtx_code code = GET_CODE (insn);

      next = chain->next;

      gcc_assert (!chain->is_caller_save_insn);

      bb_info = BB_INFO_BY_INDEX (chain->block);
      if (INSN_P (insn))
	{
	  if (JUMP_P (insn)
	      && (GET_CODE (PATTERN (insn)) == ADDR_DIFF_VEC
		  || GET_CODE (PATTERN (insn)) == ADDR_VEC))
	    empty_save_in_p = TRUE;
	  
	  CLEAR_HARD_REG_SET (referenced_regs);
	  mark_referenced_regs (PATTERN (insn));
	  IOR_HARD_REG_SET (kill, referenced_regs);
	  AND_COMPL_HARD_REG_SET (gen, referenced_regs);
	  
	  if (code == CALL_INSN && find_reg_note (insn, REG_NORETURN, NULL))
	    SET_HARD_REG_SET (kill);
	  else if (code == CALL_INSN)
	    {
	      HARD_REG_SET hard_regs_to_save, used_regs;
	      
	      /* Use the register life information in CHAIN to compute
		 which regs are live during the call.  */
	      REG_SET_TO_HARD_REG_SET (hard_regs_to_save,
				       &chain->live_throughout);
	      /* Look through all live pseudos, mark their hard registers
		 and choose proper mode for saving.  */
	      EXECUTE_IF_SET_IN_REG_SET
		(&chain->live_throughout, FIRST_PSEUDO_REGISTER, regno, rsi)
		{
		  int r = reg_renumber[regno];
		  int nregs;
		  enum machine_mode mode;
		  
		  gcc_assert (r >= 0);
		  nregs = hard_regno_nregs[r][PSEUDO_REGNO_MODE (regno)];
		  mode = HARD_REGNO_CALLER_SAVE_MODE
		    (r, nregs, PSEUDO_REGNO_MODE (regno));
		  if (nregs == 1)
		    {
		      SET_HARD_REG_BIT (hard_regs_to_save, r);
		      save_mode[r] = mode;
		    }
		  else
		    {
		      while (nregs-- > 0)
			{
			  SET_HARD_REG_BIT (hard_regs_to_save, r + nregs);
			  save_mode [r + nregs]
			    = regno_save_mode [r + nregs] [1];
			}
		    }
		}
	      
	      /* Record all registers set in this call insn.  These
		 don't need to be saved.  N.B. the call insn might set
		 a subreg of a multi-hard-reg pseudo; then the pseudo
		 is considered live during the call, but the subreg
		 that is set isn't.  */
	      CLEAR_HARD_REG_SET (this_insn_sets);
	      note_stores (PATTERN (insn), mark_set_regs, &this_insn_sets);
	      /* Sibcalls are considered to set the return value,
		 compare flow.c:propagate_one_insn.  */
	      if (SIBLING_CALL_P (insn) && current_function_return_rtx)
		mark_set_regs (current_function_return_rtx, NULL_RTX,
			       &this_insn_sets);
	      
	      /* Compute which hard regs must be saved before this call.  */
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, call_fixed_reg_set);
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, this_insn_sets);
	      get_call_invalidated_used_regs (insn, &used_regs, false);
	      AND_HARD_REG_SET (hard_regs_to_save, used_regs);
	      IOR_HARD_REG_SET (gen, hard_regs_to_save);
	    }
	}

      if (chain->next == 0 || chain->next->block != chain->block)
	{
	  basic_block bb = BASIC_BLOCK (chain->block);

	  bb_info->empty_save_in_p = empty_save_in_p;
	  empty_save_in_p = FALSE;
	  CLEAR_HARD_REG_SET (bb_info->save_in);
	  COPY_HARD_REG_SET (bb_info->gen, gen);
	  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	    {
	      bb_info->save_in_mode [i] = VOIDmode;
	      if (TEST_HARD_REG_BIT (gen, i))
		bb_info->save_out_mode [i] = save_mode [i];
	      else
		bb_info->save_out_mode [i] = VOIDmode;
	    }
	  COPY_HARD_REG_SET (bb_info->kill, kill);
	  SET_HARD_REG_SET (bb_info->save_out);
	  REG_SET_TO_HARD_REG_SET (bb_info->live_at_start,
				   bb->il.rtl->global_live_at_start);
	  EXECUTE_IF_SET_IN_REG_SET
	    (bb->il.rtl->global_live_at_start,
	     FIRST_PSEUDO_REGISTER, regno, rsi)
	    {
	      int r = reg_renumber[regno];
	      int nregs;
	      
	      if (r < 0)
		continue;
	      nregs = hard_regno_nregs[r][PSEUDO_REGNO_MODE (regno)];
	      while (nregs-- > 0)
		SET_HARD_REG_BIT (bb_info->live_at_start, r + nregs);
	    }
	  CLEAR_HARD_REG_SET (bb_info->save_here);
	  CLEAR_HARD_REG_SET (gen);
	  CLEAR_HARD_REG_SET (kill);
	  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	    save_mode [i] = VOIDmode;
	}
    }
}

/* The function sets up reverse post-order number of each basic
   block.  */
static void
set_up_bb_rts_numbers (void)
{
  int i;
  int *rts_order;
  
  rts_order = XNEWVEC (int, n_basic_blocks - NUM_FIXED_BLOCKS);
  post_order_compute (rts_order, false);
  for (i = 0; i < n_basic_blocks - NUM_FIXED_BLOCKS; i++)
    BB_INFO_BY_INDEX (rts_order [i])->rts_number = i;
  free (rts_order);
}

/* Compare function for sorting blocks in reverse postorder.  */
static int
rpost_cmp (const void *bb1, const void *bb2)
{
  basic_block b1 = *(basic_block *) bb1, b2 = *(basic_block *) bb2;

  return BB_INFO (b2)->rts_number - BB_INFO (b1)->rts_number;
}

/* The function calculates global save information according
   to the following equations:

     bb.save_in  = empty for entry block or one with empty_save_in_p
                 | ^ (pred.save_out - pred.save_here) ^ bb.live_at_start
     bb.save_out = (bb.save_in - bb.kill) U bb.gen.

   See function calculate_save_here to know how SAVE_HERE is
   calculated  */
static void
calculate_save_in_out (void)
{
  basic_block bb, succ;
  edge e;
  int i, j, nel;
  VEC(basic_block,heap) *bbs, *new_bbs, *temp;
  basic_block *bb_array;
  HARD_REG_SET temp_set;
  sbitmap wset;

  bbs = VEC_alloc (basic_block, heap, last_basic_block);
  new_bbs = VEC_alloc (basic_block, heap, last_basic_block);
  FOR_EACH_BB (bb)
    {
      VEC_quick_push (basic_block, bbs, bb);
    }
  wset = sbitmap_alloc (last_basic_block);
  while (VEC_length (basic_block, bbs))
    {
      bb_array = VEC_address (basic_block, bbs);
      nel = VEC_length (basic_block, bbs);
      qsort (bb_array, nel, sizeof (basic_block), rpost_cmp);
      sbitmap_zero (wset);
      for (i = 0; i < nel; i++)
	{
	  int first_p;
	  edge_iterator ei;
	  struct bb_info *bb_info;
	      
	  bb = bb_array [i];
	  bb_info = BB_INFO (bb);
	  first_p = TRUE;
	  if (! bb_info->empty_save_in_p)
	    FOR_EACH_EDGE (e, ei, bb->preds)
	      {
		basic_block pred = e->src;
		
		if (e->flags & EDGE_ABNORMAL)
		  {
		    CLEAR_HARD_REG_SET (bb_info->save_in);
		    break;
		  }
		if (pred->index != ENTRY_BLOCK)
		  {
		    COPY_HARD_REG_SET (temp_set, BB_INFO (pred)->save_out);
		    AND_COMPL_HARD_REG_SET (temp_set,
					    BB_INFO (pred)->save_here);
		    AND_HARD_REG_SET (temp_set, bb_info->live_at_start);
		    if (first_p)
		      COPY_HARD_REG_SET (bb_info->save_in, temp_set);
		    else
		      AND_HARD_REG_SET (bb_info->save_in, temp_set);
		    for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
		      if (TEST_HARD_REG_BIT (temp_set, j))
			{
			  gcc_assert
			    (bb_info->save_in_mode [j] == VOIDmode
			     || BB_INFO (pred)->save_out_mode [j] == VOIDmode
			     || (bb_info->save_in_mode [j]
				 == BB_INFO (pred)->save_out_mode [j]));
			  if (BB_INFO (pred)->save_out_mode [j] != VOIDmode)
			    bb_info->save_in_mode [j]
			      = BB_INFO (pred)->save_out_mode [j];
			}
		    first_p = FALSE;
		  }
	      }
	  COPY_HARD_REG_SET (temp_set, bb_info->save_in);
	  AND_COMPL_HARD_REG_SET (temp_set, bb_info->kill);
	  for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
	    if (TEST_HARD_REG_BIT (temp_set, j)
		&& ! TEST_HARD_REG_BIT (bb_info->gen, j))
	      {
		gcc_assert (bb_info->save_out_mode [j] == VOIDmode
			    || (bb_info->save_out_mode [j]
				== bb_info->save_in_mode [j]));
		bb_info->save_out_mode [j] = bb_info->save_in_mode [j];
	      }
	  IOR_HARD_REG_SET (temp_set, bb_info->gen);

	  GO_IF_HARD_REG_EQUAL (temp_set, bb_info->save_out, cont);
	  COPY_HARD_REG_SET (bb_info->save_out, temp_set);
	  FOR_EACH_EDGE (e, ei, bb->succs)
	    {
	      succ = e->dest;
	      if (succ->index != EXIT_BLOCK && ! TEST_BIT (wset, succ->index))
		{
		  SET_BIT (wset, succ->index);
		  VEC_quick_push (basic_block, new_bbs, succ);
		}
	    }
	cont:
	  ;
	}
      temp = bbs;
      bbs = new_bbs;
      new_bbs = temp;
      VEC_truncate (basic_block, new_bbs, 0);
    }
  sbitmap_free (wset);
  VEC_free (basic_block, heap, new_bbs);
  VEC_free (basic_block, heap, bbs);
}

/* The function calculates SAVE_HERE according to the equation
   bb.save_here = U ((bb.save_out - succ.save_in) ^ succ.live_at_start).  */
static int
calculate_save_here (void)
{
  basic_block bb;
  edge e;
  edge_iterator ei;
  HARD_REG_SET save_here, temp_set;
  int changed_p = FALSE;

  FOR_EACH_BB (bb)
    {
      CLEAR_HARD_REG_SET (save_here);
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  basic_block dest = e->dest;

	  COPY_HARD_REG_SET (temp_set, BB_INFO (bb)->save_out);
	  AND_COMPL_HARD_REG_SET (temp_set, BB_INFO (dest)->save_in);
	  AND_HARD_REG_SET (temp_set, BB_INFO (dest)->live_at_start);
	  IOR_HARD_REG_SET (save_here, temp_set);
	}

      GO_IF_HARD_REG_EQUAL (save_here, BB_INFO (bb)->save_here, cont);
      COPY_HARD_REG_SET (BB_INFO (bb)->save_here, save_here);
      changed_p = TRUE;
    cont:
      ;
    }
  return changed_p;
}

/* The function calculates global save information used to put
   save/restore code without generating new blocks.  This is a
   bidirectional data flow problem (calculation of SAVE_IN and
   SAVE_OUT is a forward data flow problem and SAVE_HERE is a backward
   one).  It is complicated by calculation of modes for
   saving/restoring call used hard registers.  */
static void
make_global_save_analysis (void)
{
  int iter, changed_p;

  calculate_local_save_info ();
  set_up_bb_rts_numbers ();
  for (iter = 1;; iter++)
    {
      calculate_save_in_out ();
      changed_p = calculate_save_here ();
      if (! changed_p)
	break;
    }
  if (dump_file != NULL)
    fprintf (dump_file, " Number of global save analysis iterations %d\n",
	     iter);
}

/* Print hard registers in SET to file F.  The registers are printed
   with its mode given in MODES.  */  
static void
print_hard_reg_set_and_mode (FILE *f, HARD_REG_SET set, unsigned char *modes)
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (set, i))
      fprintf (f, " %d:%s %s", i, GET_MODE_NAME (modes [i]), reg_names [i]);
}

/* Print hard registers in SET to file F.  */  
static void
print_hard_reg_set (FILE *f, HARD_REG_SET set)
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (set, i))
      fprintf (f, " %d %s", i, reg_names [i]);
}

/* Print save information for each block to file F.  */  
static void
print_save_data (FILE *f)
{
  basic_block bb;
  struct bb_info *bb_info;

  FOR_EACH_BB (bb)
    {
      bb_info = BB_INFO (bb);
      fprintf (f, "bb %d:\n  save_in:", bb->index);
      print_hard_reg_set_and_mode (f, bb_info->save_in,
				   bb_info->save_in_mode);
      fprintf (f, "\n  save_out:");
      print_hard_reg_set_and_mode (f, bb_info->save_out,
				   bb_info->save_out_mode);
      fprintf (f, "\n  live_at_start:");
      print_hard_reg_set (f, bb_info->live_at_start);
      fprintf (f, "\n  save_here:");
      print_hard_reg_set (f, bb_info->save_here);
      fprintf (f, "\n  kill:");
      print_hard_reg_set (f, bb_info->kill);
      fprintf (f, "\n  gen:");
      print_hard_reg_set (f, bb_info->gen);
      fprintf (f, "\n\n");
    }
}

/* Print save information for each block to stderr.  */  
void
debug_save_data (void)
{
  print_save_data (stderr);
}

/* Setup hard registers in SET to save.  Setup also their save modes
   in SAVE_MODE from FROM_SAVE_MODE.  */  
static void
set_hard_reg_saved (HARD_REG_SET set, unsigned char *from_saved_mode,
		    enum machine_mode *save_mode)
{
  int regno;

  n_regs_saved = 0;
  COPY_HARD_REG_SET (hard_regs_saved, set);
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    if (TEST_HARD_REG_BIT (set, regno))
      {
	n_regs_saved++;
	save_mode [regno] = from_saved_mode [regno];
      }
    else
      save_mode [regno] = VOIDmode;
}

/* Find the places where hard regs are live across calls and save them.  */

void
save_call_clobbered_regs (void)
{
  struct insn_chain *chain, *next;
  enum machine_mode save_mode [FIRST_PSEUDO_REGISTER];

  if (flag_ira)
    {
      alloc_aux_for_blocks (sizeof (struct bb_info));
      make_global_save_analysis ();
    }

  /* Computed in mark_set_regs, holds all registers set by the current
     instruction.  */
  HARD_REG_SET this_insn_sets;

  CLEAR_HARD_REG_SET (hard_regs_saved);
  n_regs_saved = 0;

  for (chain = reload_insn_chain; chain != 0; chain = next)
    {
      rtx insn = chain->insn;
      enum rtx_code code = GET_CODE (insn);

      next = chain->next;

      gcc_assert (!chain->is_caller_save_insn);

      if (INSN_P (insn))
	{
	  /* If some registers have been saved, see if INSN references
	     any of them.  We must restore them before the insn if so.  */

	  if (n_regs_saved)
	    {
	      int regno;

	      if (code == JUMP_INSN)
		/* Restore all registers if this is a JUMP_INSN.  */
		COPY_HARD_REG_SET (referenced_regs, hard_regs_saved);
	      else
		{
		  CLEAR_HARD_REG_SET (referenced_regs);
		  mark_referenced_regs (PATTERN (insn));
		  AND_HARD_REG_SET (referenced_regs, hard_regs_saved);
		}

	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (TEST_HARD_REG_BIT (referenced_regs, regno))
		  {
		    int before = regno;

		    regno += insert_restore (chain, 1, regno, MOVE_MAX_WORDS,
					     save_mode);
		    if (flag_ira)
		      {
			gcc_assert (before == regno);
			save_mode [before] = VOIDmode;
		      }
		  }
	    }

	  if (code == CALL_INSN && ! find_reg_note (insn, REG_NORETURN, NULL))
	    {
	      unsigned regno;
	      HARD_REG_SET hard_regs_to_save, used_regs;
	      reg_set_iterator rsi;

	      /* Use the register life information in CHAIN to compute which
		 regs are live during the call.  */
	      REG_SET_TO_HARD_REG_SET (hard_regs_to_save,
				       &chain->live_throughout);
	      /* Save hard registers always in the widest mode available.  */
	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (TEST_HARD_REG_BIT (hard_regs_to_save, regno))
		  {
		    CLEAR_HARD_REG_SET (this_insn_sets);
		    note_stores (PATTERN (insn), mark_set_regs,
				 &this_insn_sets);
		    save_mode [regno] = regno_save_mode [regno] [1];
		  }
		else
		  save_mode [regno] = VOIDmode;

	      /* Look through all live pseudos, mark their hard registers
		 and choose proper mode for saving.  */
	      EXECUTE_IF_SET_IN_REG_SET
		(&chain->live_throughout, FIRST_PSEUDO_REGISTER, regno, rsi)
		{
		  int r = reg_renumber[regno];
		  int nregs;
		  enum machine_mode mode;

		  gcc_assert (r >= 0);
		  nregs = hard_regno_nregs[r][PSEUDO_REGNO_MODE (regno)];
		  mode = HARD_REGNO_CALLER_SAVE_MODE
		    (r, nregs, PSEUDO_REGNO_MODE (regno));
		  if (GET_MODE_BITSIZE (mode)
		      > GET_MODE_BITSIZE (save_mode[r]))
		    save_mode[r] = mode;
		  while (nregs-- > 0)
		    SET_HARD_REG_BIT (hard_regs_to_save, r + nregs);
		}

	      /* Record all registers set in this call insn.  These don't need
		 to be saved.  N.B. the call insn might set a subreg of a
		 multi-hard-reg pseudo; then the pseudo is considered live
		 during the call, but the subreg that is set isn't.  */
	      CLEAR_HARD_REG_SET (this_insn_sets);
	      note_stores (PATTERN (insn), mark_set_regs, &this_insn_sets);
	      /* Sibcalls are considered to set the return value,
		 compare flow.c:propagate_one_insn.  */
	      if (SIBLING_CALL_P (insn) && current_function_return_rtx)
		mark_set_regs (current_function_return_rtx, NULL_RTX,
			       &this_insn_sets);

	      /* Compute which hard regs must be saved before this call.  */
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, call_fixed_reg_set);
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, this_insn_sets);
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, hard_regs_saved);
	      get_call_invalidated_used_regs (insn, &used_regs, false);
	      AND_HARD_REG_SET (hard_regs_to_save, used_regs);

	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (TEST_HARD_REG_BIT (hard_regs_to_save, regno))
		  regno += insert_save (chain, 1, regno, &hard_regs_to_save,
					save_mode);

	      /* Must recompute n_regs_saved.  */
	      n_regs_saved = 0;
	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (TEST_HARD_REG_BIT (hard_regs_saved, regno))
		  n_regs_saved++;
	    }
	}

      if (chain->next == 0 || chain->next->block != chain->block)
	{
	  int regno;
	  struct bb_info *next_bb_info;

	  next_bb_info = (chain->next != NULL
			  ? BB_INFO_BY_INDEX (chain->next->block) : NULL);

	  /* At the end of the basic block, we must restore any registers that
	     remain saved.  If the last insn in the block is a JUMP_INSN, put
	     the restore before the insn, otherwise, put it after the insn.  */

	  if (flag_ira)
	    set_hard_reg_saved (BB_INFO_BY_INDEX (chain->block)->save_here,
				BB_INFO_BY_INDEX (chain->block)->save_out_mode,
				save_mode);

	  if (n_regs_saved)
	    for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
	      if (TEST_HARD_REG_BIT (hard_regs_saved, regno))
		{
		  int before = regno;

		  regno += insert_restore (chain, JUMP_P (insn),
					   regno, MOVE_MAX_WORDS, save_mode);
		  if (flag_ira)
		    {
		      gcc_assert (before == regno);
		      save_mode [before] = VOIDmode;
		    }
		}

	  if (flag_ira && next_bb_info != NULL)
	    set_hard_reg_saved (next_bb_info->save_in,
				next_bb_info->save_in_mode, save_mode);

	}
    }

  if (flag_ira)
    free_aux_for_blocks ();
}

/* Here from note_stores, or directly from save_call_clobbered_regs, when
   an insn stores a value in a register.
   Set the proper bit or bits in this_insn_sets.  All pseudos that have
   been assigned hard regs have had their register number changed already,
   so we can ignore pseudos.  */
static void
mark_set_regs (rtx reg, rtx setter ATTRIBUTE_UNUSED, void *data)
{
  int regno, endregno, i;
  enum machine_mode mode = GET_MODE (reg);
  HARD_REG_SET *this_insn_sets = data;

  if (GET_CODE (reg) == SUBREG)
    {
      rtx inner = SUBREG_REG (reg);
      if (!REG_P (inner) || REGNO (inner) >= FIRST_PSEUDO_REGISTER)
	return;
      regno = subreg_regno (reg);
    }
  else if (REG_P (reg)
	   && REGNO (reg) < FIRST_PSEUDO_REGISTER)
    regno = REGNO (reg);
  else
    return;

  endregno = regno + hard_regno_nregs[regno][mode];

  for (i = regno; i < endregno; i++)
    SET_HARD_REG_BIT (*this_insn_sets, i);
}

/* Here from note_stores when an insn stores a value in a register.
   Set the proper bit or bits in the passed regset.  All pseudos that have
   been assigned hard regs have had their register number changed already,
   so we can ignore pseudos.  */
static void
add_stored_regs (rtx reg, rtx setter, void *data)
{
  int regno, endregno, i;
  enum machine_mode mode = GET_MODE (reg);
  int offset = 0;

  if (GET_CODE (setter) == CLOBBER)
    return;

  if (GET_CODE (reg) == SUBREG && REG_P (SUBREG_REG (reg)))
    {
      offset = subreg_regno_offset (REGNO (SUBREG_REG (reg)),
				    GET_MODE (SUBREG_REG (reg)),
				    SUBREG_BYTE (reg),
				    GET_MODE (reg));
      reg = SUBREG_REG (reg);
    }

  if (!REG_P (reg) || REGNO (reg) >= FIRST_PSEUDO_REGISTER)
    return;

  regno = REGNO (reg) + offset;
  endregno = regno + hard_regno_nregs[regno][mode];

  for (i = regno; i < endregno; i++)
    SET_REGNO_REG_SET ((regset) data, i);
}

/* Walk X and record all referenced registers in REFERENCED_REGS.  */
static void
mark_referenced_regs (rtx x)
{
  enum rtx_code code = GET_CODE (x);
  const char *fmt;
  int i, j;

  if (code == SET)
    mark_referenced_regs (SET_SRC (x));
  if (code == SET || code == CLOBBER)
    {
      x = SET_DEST (x);
      code = GET_CODE (x);
      if ((code == REG && REGNO (x) < FIRST_PSEUDO_REGISTER)
	  || code == PC || code == CC0
	  || (code == SUBREG && REG_P (SUBREG_REG (x))
	      && REGNO (SUBREG_REG (x)) < FIRST_PSEUDO_REGISTER
	      /* If we're setting only part of a multi-word register,
		 we shall mark it as referenced, because the words
		 that are not being set should be restored.  */
	      && ((GET_MODE_SIZE (GET_MODE (x))
		   >= GET_MODE_SIZE (GET_MODE (SUBREG_REG (x))))
		  || (GET_MODE_SIZE (GET_MODE (SUBREG_REG (x)))
		      <= UNITS_PER_WORD))))
	return;
    }
  if (code == MEM || code == SUBREG)
    {
      x = XEXP (x, 0);
      code = GET_CODE (x);
    }

  if (code == REG)
    {
      int regno = REGNO (x);
      int hardregno = (regno < FIRST_PSEUDO_REGISTER ? regno
		       : reg_renumber[regno]);

      if (hardregno >= 0)
	{
	  int nregs = hard_regno_nregs[hardregno][GET_MODE (x)];
	  while (nregs-- > 0)
	    SET_HARD_REG_BIT (referenced_regs, hardregno + nregs);
	}
      /* If this is a pseudo that did not get a hard register, scan its
	 memory location, since it might involve the use of another
	 register, which might be saved.  */
      else if (reg_equiv_mem[regno] != 0)
	mark_referenced_regs (XEXP (reg_equiv_mem[regno], 0));
      else if (reg_equiv_address[regno] != 0)
	mark_referenced_regs (reg_equiv_address[regno]);
      return;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	mark_referenced_regs (XEXP (x, i));
      else if (fmt[i] == 'E')
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  mark_referenced_regs (XVECEXP (x, i, j));
    }
}

/* Insert a sequence of insns to restore.  Place these insns in front of
   CHAIN if BEFORE_P is nonzero, behind the insn otherwise.  MAXRESTORE is
   the maximum number of registers which should be restored during this call.
   It should never be less than 1 since we only work with entire registers.

   Note that we have verified in init_caller_save that we can do this
   with a simple SET, so use it.  Set INSN_CODE to what we save there
   since the address might not be valid so the insn might not be recognized.
   These insns will be reloaded and have register elimination done by
   find_reload, so we need not worry about that here.

   Return the extra number of registers saved.  */

static int
insert_restore (struct insn_chain *chain, int before_p, int regno,
		int maxrestore, enum machine_mode *save_mode)
{
  int i, k;
  rtx pat = NULL_RTX;
  int code;
  unsigned int numregs = 0;
  struct insn_chain *new;
  rtx mem;

  /* A common failure mode if register status is not correct in the
     RTL is for this routine to be called with a REGNO we didn't
     expect to save.  That will cause us to write an insn with a (nil)
     SET_DEST or SET_SRC.  Instead of doing so and causing a crash
     later, check for this common case here instead.  This will remove
     one step in debugging such problems.  */
  gcc_assert (regno_save_mem[regno][1]);

  /* Get the pattern to emit and update our status.

     See if we can restore `maxrestore' registers at once.  Work
     backwards to the single register case.  */
  for (i = maxrestore; i > 0; i--)
    {
      int j;
      int ok = 1;

      if (regno_save_mem[regno][i] == 0)
	continue;

      for (j = 0; j < i; j++)
	if (! TEST_HARD_REG_BIT (hard_regs_saved, regno + j))
	  {
	    ok = 0;
	    break;
	  }
      /* Must do this one restore at a time.  */
      if (! ok)
	continue;

      numregs = i;
      break;
    }

  mem = regno_save_mem [regno][numregs];
  if (save_mode [regno] != VOIDmode
      && save_mode [regno] != GET_MODE (mem)
      && numregs == (unsigned int) hard_regno_nregs[regno][save_mode [regno]])
    mem = adjust_address (mem, save_mode[regno], 0);
  else
    mem = copy_rtx (mem);
  pat = gen_rtx_SET (VOIDmode,
		     gen_rtx_REG (GET_MODE (mem),
				  regno), mem);
  code = reg_restore_code[regno][GET_MODE (mem)];
  new = insert_one_insn (chain, before_p, code, pat);

  /* Clear status for all registers we restored.  */
  for (k = 0; k < i; k++)
    {
      CLEAR_HARD_REG_BIT (hard_regs_saved, regno + k);
      SET_REGNO_REG_SET (&new->dead_or_set, regno + k);
      n_regs_saved--;
    }

  /* Tell our callers how many extra registers we saved/restored.  */
  return numregs - 1;
}

/* Like insert_restore above, but save registers instead.  */

static int
insert_save (struct insn_chain *chain, int before_p, int regno,
	     HARD_REG_SET (*to_save), enum machine_mode *save_mode)
{
  int i;
  unsigned int k;
  rtx pat = NULL_RTX;
  int code;
  unsigned int numregs = 0;
  struct insn_chain *new;
  rtx mem;

  /* A common failure mode if register status is not correct in the
     RTL is for this routine to be called with a REGNO we didn't
     expect to save.  That will cause us to write an insn with a (nil)
     SET_DEST or SET_SRC.  Instead of doing so and causing a crash
     later, check for this common case here.  This will remove one
     step in debugging such problems.  */
  gcc_assert (regno_save_mem[regno][1]);

  /* Get the pattern to emit and update our status.

     See if we can save several registers with a single instruction.
     Work backwards to the single register case.  */
  for (i = MOVE_MAX_WORDS; i > 0; i--)
    {
      int j;
      int ok = 1;
      if (regno_save_mem[regno][i] == 0)
	continue;

      for (j = 0; j < i; j++)
	if (! TEST_HARD_REG_BIT (*to_save, regno + j))
	  {
	    ok = 0;
	    break;
	  }
      /* Must do this one save at a time.  */
      if (! ok)
	continue;

      numregs = i;
      break;
    }

  mem = regno_save_mem [regno][numregs];
  if (save_mode [regno] != VOIDmode
      && save_mode [regno] != GET_MODE (mem)
      && numregs == (unsigned int) hard_regno_nregs[regno][save_mode [regno]])
    mem = adjust_address (mem, save_mode[regno], 0);
  else
    mem = copy_rtx (mem);
  pat = gen_rtx_SET (VOIDmode, mem,
		     gen_rtx_REG (GET_MODE (mem),
				  regno));
  code = reg_save_code[regno][GET_MODE (mem)];
  new = insert_one_insn (chain, before_p, code, pat);

  /* Set hard_regs_saved and dead_or_set for all the registers we saved.  */
  for (k = 0; k < numregs; k++)
    {
      SET_HARD_REG_BIT (hard_regs_saved, regno + k);
      SET_REGNO_REG_SET (&new->dead_or_set, regno + k);
      n_regs_saved++;
    }

  /* Tell our callers how many extra registers we saved/restored.  */
  return numregs - 1;
}

/* Emit a new caller-save insn and set the code.  */
static struct insn_chain *
insert_one_insn (struct insn_chain *chain, int before_p, int code, rtx pat)
{
  rtx insn = chain->insn;
  struct insn_chain *new;

#ifdef HAVE_cc0
  /* If INSN references CC0, put our insns in front of the insn that sets
     CC0.  This is always safe, since the only way we could be passed an
     insn that references CC0 is for a restore, and doing a restore earlier
     isn't a problem.  We do, however, assume here that CALL_INSNs don't
     reference CC0.  Guard against non-INSN's like CODE_LABEL.  */

  if ((NONJUMP_INSN_P (insn) || JUMP_P (insn))
      && before_p
      && reg_referenced_p (cc0_rtx, PATTERN (insn)))
    chain = chain->prev, insn = chain->insn;
#endif

  new = new_insn_chain ();
  if (before_p)
    {
      rtx link;

      new->prev = chain->prev;
      if (new->prev != 0)
	new->prev->next = new;
      else
	reload_insn_chain = new;

      chain->prev = new;
      new->next = chain;
      new->insn = emit_insn_before (pat, insn);
      /* ??? It would be nice if we could exclude the already / still saved
	 registers from the live sets.  */
      COPY_REG_SET (&new->live_throughout, &chain->live_throughout);
      /* Registers that die in CHAIN->INSN still live in the new insn.  */
      for (link = REG_NOTES (chain->insn); link; link = XEXP (link, 1))
	{
	  if (REG_NOTE_KIND (link) == REG_DEAD)
	    {
	      rtx reg = XEXP (link, 0);
	      int regno, i;

	      gcc_assert (REG_P (reg));
	      regno = REGNO (reg);
	      if (regno >= FIRST_PSEUDO_REGISTER)
		regno = reg_renumber[regno];
	      if (regno < 0)
		continue;
	      for (i = hard_regno_nregs[regno][GET_MODE (reg)] - 1;
		   i >= 0; i--)
		SET_REGNO_REG_SET (&new->live_throughout, regno + i);
	    }
	}
      CLEAR_REG_SET (&new->dead_or_set);
      if (chain->insn == BB_HEAD (BASIC_BLOCK (chain->block)))
	BB_HEAD (BASIC_BLOCK (chain->block)) = new->insn;
    }
  else
    {
      new->next = chain->next;
      if (new->next != 0)
	new->next->prev = new;
      chain->next = new;
      new->prev = chain;
      if (GET_CODE (insn) != CODE_LABEL)
	new->insn = emit_insn_after (pat, insn);
      else
	{
	  /* Put the insn after bb note in a empty basic block.  */
	  gcc_assert (NEXT_INSN (insn) && NOTE_P (NEXT_INSN (insn)));
	  new->insn = emit_insn_after (pat, NEXT_INSN (insn));
	}
      /* ??? It would be nice if we could exclude the already / still saved
	 registers from the live sets, and observe REG_UNUSED notes.  */
      COPY_REG_SET (&new->live_throughout, &chain->live_throughout);
      /* Registers that are set in CHAIN->INSN live in the new insn.
	 (Unless there is a REG_UNUSED note for them, but we don't
	  look for them here.) */
      if (INSN_P (chain->insn))
	note_stores (PATTERN (chain->insn), add_stored_regs,
		     &new->live_throughout);
      CLEAR_REG_SET (&new->dead_or_set);
      if (chain->insn == BB_END (BASIC_BLOCK (chain->block)))
	BB_END (BASIC_BLOCK (chain->block)) = new->insn;
    }
  new->block = chain->block;
  new->is_caller_save_insn = 1;

  INSN_CODE (new->insn) = code;
  return new;
}
