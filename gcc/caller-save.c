/* Save and restore call-clobbered registers which are live across a call.
   Copyright (C) 1989, 1992, 1994, 1995, 1997, 1998, 1999, 2000,
   2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software Foundation, Inc.

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
#include "cfgloop.h"
#include "df.h"
#include "ggc.h"

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

/* The number of elements in the subsequent array.  */
static int save_slots_num;

/* Allocated slots so far.  */
static rtx save_slots[FIRST_PSEUDO_REGISTER];

/* We will only make a register eligible for caller-save if it can be
   saved in its widest mode with a simple SET insn as long as the memory
   address is valid.  We record the INSN_CODE is those insns here since
   when we emit them, the addresses might not be valid, so they might not
   be recognized.  */

static int
  cached_reg_save_code[FIRST_PSEUDO_REGISTER][MAX_MACHINE_MODE];
static int
  cached_reg_restore_code[FIRST_PSEUDO_REGISTER][MAX_MACHINE_MODE];

/* Set of hard regs currently residing in save area (during insn scan).  */

static HARD_REG_SET hard_regs_saved;

/* Number of registers currently in hard_regs_saved.  */

static int n_regs_saved;

/* Computed by mark_referenced_regs, all regs referenced in a given
   insn.  */
static HARD_REG_SET referenced_regs;
/* Computed by mark_referenced_regs, all regs modified in a given insn.  */
static HARD_REG_SET modified_regs;


static int reg_save_code (int, enum machine_mode);
static int reg_restore_code (int, enum machine_mode);

struct saved_hard_reg;
static void initiate_saved_hard_regs (void);
static struct saved_hard_reg *new_saved_hard_reg (int, int);
static void finish_saved_hard_regs (void);
static int saved_hard_reg_compare_func (const void *, const void *);
static void calculate_local_save_info (void);
static bool restore_trans_fun (int);
static void restore_con_fun_0 (basic_block);
static void restore_con_fun_n (edge);
static void calculate_restore_in_out (void);
static int calculate_restore_here (void);
static bool save_trans_fun (int);
static void save_con_fun_0 (basic_block);
static void save_con_fun_n (edge);
static void calculate_save_in_out (void);
static int calculate_save_here (void);
static bool free_trans_fun (int);
static void free_con_fun_0 (basic_block);
static void free_con_fun_n (edge);
static void calculate_free_in_out (void);

static void make_global_save_analysis (void);
static void print_annotated_hard_reg_set (FILE *, HARD_REG_SET,
					  unsigned char *, int *);
static void print_hard_reg_set (FILE *, HARD_REG_SET);
static void print_save_data (FILE *);
static void set_hard_reg_saved (HARD_REG_SET, unsigned char *,
				enum machine_mode *, int *, int *);

static void mark_set_regs (rtx, const_rtx, void *);
static void add_stored_regs (rtx, const_rtx, void *);
static void mark_referenced_regs (rtx, int);
static int insert_save (struct insn_chain *, int, int, HARD_REG_SET *,
			enum machine_mode *, int *);
static int insert_restore (struct insn_chain *, int, int, int,
			   enum machine_mode *, int *);
static struct insn_chain *insert_one_insn (struct insn_chain *, int, int,
					   rtx);
static void add_stored_regs (rtx, const_rtx, void *);



static GTY(()) rtx savepat;
static GTY(()) rtx restpat;
static GTY(()) rtx test_reg;
static GTY(()) rtx test_mem;
static GTY(()) rtx saveinsn;
static GTY(()) rtx restinsn;

/* Return the INSN_CODE used to save register REG in mode MODE.  */
static int
reg_save_code (int reg, enum machine_mode mode)
{
  bool ok;
  if (cached_reg_save_code[reg][mode])
     return cached_reg_save_code[reg][mode];
  if (!HARD_REGNO_MODE_OK (reg, mode))
     {
       cached_reg_save_code[reg][mode] = -1;
       cached_reg_restore_code[reg][mode] = -1;
       return -1;
     }

  /* Update the register number and modes of the register
     and memory operand.  */
  SET_REGNO (test_reg, reg);
  PUT_MODE (test_reg, mode);
  PUT_MODE (test_mem, mode);

  /* Force re-recognition of the modified insns.  */
  INSN_CODE (saveinsn) = -1;
  INSN_CODE (restinsn) = -1;

  cached_reg_save_code[reg][mode] = recog_memoized (saveinsn);
  cached_reg_restore_code[reg][mode] = recog_memoized (restinsn);

  /* Now extract both insns and see if we can meet their
     constraints.  */
  ok = (cached_reg_save_code[reg][mode] != -1
	&& cached_reg_restore_code[reg][mode] != -1);
  if (ok)
    {
      extract_insn (saveinsn);
      ok = constrain_operands (1);
      extract_insn (restinsn);
      ok &= constrain_operands (1);
    }

  if (! ok)
    {
      cached_reg_save_code[reg][mode] = -1;
      cached_reg_restore_code[reg][mode] = -1;
    }
  gcc_assert (cached_reg_save_code[reg][mode]);
  return cached_reg_save_code[reg][mode];
}

/* Return the INSN_CODE used to restore register REG in mode MODE.  */
static int
reg_restore_code (int reg, enum machine_mode mode)
{
  if (cached_reg_restore_code[reg][mode])
     return cached_reg_restore_code[reg][mode];
  /* Populate our cache.  */
  reg_save_code (reg, mode);
  return cached_reg_restore_code[reg][mode];
}

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
	 [(int) base_reg_class (regno_save_mode[i][1], PLUS, CONST_INT)], i))
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

  saveinsn = gen_rtx_INSN (VOIDmode, 0, 0, 0, 0, 0, savepat, -1, 0);
  restinsn = gen_rtx_INSN (VOIDmode, 0, 0, 0, 0, 0, restpat, -1, 0);

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    for (j = 1; j <= MOVE_MAX_WORDS; j++)
      if (reg_save_code (i,regno_save_mode[i][j]) == -1)
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
  save_slots_num = 0;
    
}

/* The structure represents a hard register which should be saved
   through the call.  It is used when the integrated register
   allocator (IRA) is used and sharing save slots is on.  */
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
  /* Order number of the next hard register structure with the same
     slot in the chain.  -1 represents end of the chain.  */
  int next;
};

/* Map: hard register number to the corresponding structure.  */
static struct saved_hard_reg *hard_reg_map[FIRST_PSEUDO_REGISTER];

/* The number of all structures representing hard registers should be
   saved, in order words, the number of used elements in the following
   array.  */
static int saved_regs_num;

/* Pointers to all the structures.  Index is the order number of the
   corresponding structure.  */
static struct saved_hard_reg *all_saved_regs[FIRST_PSEUDO_REGISTER];

/* First called function for work with saved hard registers.  */
static void
initiate_saved_hard_regs (void)
{
  int i;

  saved_regs_num = 0;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    hard_reg_map[i] = NULL;
}

/* Allocate and return new saved hard register with given REGNO and
   CALL_FREQ.  */
static struct saved_hard_reg *
new_saved_hard_reg (int regno, int call_freq)
{
  struct saved_hard_reg *saved_reg;

  saved_reg
    = (struct saved_hard_reg *) xmalloc (sizeof (struct saved_hard_reg));
  hard_reg_map[regno] = all_saved_regs[saved_regs_num] = saved_reg;
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
    free (all_saved_regs[i]);
}

/* The function is used to sort the saved hard register structures
   according their frequency.  */
static int
saved_hard_reg_compare_func (const void *v1p, const void *v2p)
{
  const struct saved_hard_reg *p1 = *(struct saved_hard_reg * const *) v1p;
  const struct saved_hard_reg *p2 = *(struct saved_hard_reg * const *) v2p;
  
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
   saving hard registers through calls.  We build conflicts for them
   to do coloring.

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
	  = end_hard_regno (GET_MODE (regno_reg_rtx[i]), regno);
	for (r = regno; r < endregno; r++)
	  if (call_used_regs[r])
	    SET_HARD_REG_BIT (hard_regs_used, r);
      }

  if (flag_ira && optimize && flag_ira_share_save_slots)
    {
      rtx insn, slot;
      struct insn_chain *chain, *next;
      char *saved_reg_conflicts;
      unsigned int regno;
      int next_k, freq;
      struct saved_hard_reg *saved_reg, *saved_reg2, *saved_reg3;
      int call_saved_regs_num;
      struct saved_hard_reg *call_saved_regs[FIRST_PSEUDO_REGISTER];
      HARD_REG_SET hard_regs_to_save, used_regs, this_insn_sets;
      reg_set_iterator rsi;
      int best_slot_num;
      int prev_save_slots_num;
      rtx prev_save_slots[FIRST_PSEUDO_REGISTER];
      
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
	  COPY_HARD_REG_SET (used_regs, call_used_reg_set);

	  /* Record all registers set in this call insn.  These don't
	     need to be saved.  N.B. the call insn might set a subreg
	     of a multi-hard-reg pseudo; then the pseudo is considered
	     live during the call, but the subreg that is set
	     isn't.  */
	  CLEAR_HARD_REG_SET (this_insn_sets);
	  note_stores (PATTERN (insn), mark_set_regs, &this_insn_sets);
	  /* Sibcalls are considered to set the return value.  */
	  if (SIBLING_CALL_P (insn) && crtl->return_rtx)
	    mark_set_regs (crtl->return_rtx, NULL_RTX, &this_insn_sets);

	  AND_COMPL_HARD_REG_SET (used_regs, call_fixed_reg_set);
	  AND_COMPL_HARD_REG_SET (used_regs, this_insn_sets);
	  AND_HARD_REG_SET (hard_regs_to_save, used_regs);
	  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
	    if (TEST_HARD_REG_BIT (hard_regs_to_save, regno))
	      {
		if (hard_reg_map[regno] != NULL)
		  hard_reg_map[regno]->call_freq += freq;
		else
		  saved_reg = new_saved_hard_reg (regno, freq);
	      }
	  /* Look through all live pseudos, mark their hard registers.  */
	  EXECUTE_IF_SET_IN_REG_SET
	    (&chain->live_throughout, FIRST_PSEUDO_REGISTER, regno, rsi)
	    {
	      int r = reg_renumber[regno];
	      int bound;
	      
	      if (r < 0)
		continue;
	      
	      bound = r + hard_regno_nregs[r][PSEUDO_REGNO_MODE (regno)];
	      for (; r < bound; r++)
		if (TEST_HARD_REG_BIT (used_regs, r))
		  {
		    if (hard_reg_map[r] != NULL)
		      hard_reg_map[r]->call_freq += freq;
		    else
		      saved_reg = new_saved_hard_reg (r, freq);
		    SET_HARD_REG_BIT (hard_regs_to_save, r);
		  }
	    }
	}
      /* Find saved hard register conflicts.  */
      saved_reg_conflicts = (char *) xmalloc (saved_regs_num * saved_regs_num);
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
	  COPY_HARD_REG_SET (used_regs, call_used_reg_set);

	  /* Record all registers set in this call insn.  These don't
	     need to be saved.  N.B. the call insn might set a subreg
	     of a multi-hard-reg pseudo; then the pseudo is considered
	     live during the call, but the subreg that is set
	     isn't.  */
	  CLEAR_HARD_REG_SET (this_insn_sets);
	  note_stores (PATTERN (insn), mark_set_regs, &this_insn_sets);
	  /* Sibcalls are considered to set the return value,
	     compare flow.c:propagate_one_insn.  */
	  if (SIBLING_CALL_P (insn) && crtl->return_rtx)
	    mark_set_regs (crtl->return_rtx, NULL_RTX, &this_insn_sets);

	  AND_COMPL_HARD_REG_SET (used_regs, call_fixed_reg_set);
	  AND_COMPL_HARD_REG_SET (used_regs, this_insn_sets);
	  AND_HARD_REG_SET (hard_regs_to_save, used_regs);
	  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
	    if (TEST_HARD_REG_BIT (hard_regs_to_save, regno))
	      {
		gcc_assert (hard_reg_map[regno] != NULL);
		call_saved_regs[call_saved_regs_num++] = hard_reg_map[regno];
	      }
	  /* Look through all live pseudos, mark their hard registers.  */
	  EXECUTE_IF_SET_IN_REG_SET
	    (&chain->live_throughout, FIRST_PSEUDO_REGISTER, regno, rsi)
	    {
	      int r = reg_renumber[regno];
	      int bound;
	      
	      if (r < 0)
		continue;

	      bound = r + hard_regno_nregs[r][PSEUDO_REGNO_MODE (regno)];
	      for (; r < bound; r++)
		if (TEST_HARD_REG_BIT (used_regs, r))
		  call_saved_regs[call_saved_regs_num++] = hard_reg_map[r];
	    }
	  for (i = 0; i < call_saved_regs_num; i++)
	    {
	      saved_reg = call_saved_regs[i];
	      for (j = 0; j < call_saved_regs_num; j++)
		if (i != j)
		  {
		    saved_reg2 = call_saved_regs[j];
		    saved_reg_conflicts[saved_reg->num * saved_regs_num
					+ saved_reg2->num]
		      = saved_reg_conflicts[saved_reg2->num * saved_regs_num
					    + saved_reg->num]
		      = TRUE;
		  }
	    }
	}
      /* Sort saved hard regs.  */
      qsort (all_saved_regs, saved_regs_num, sizeof (struct saved_hard_reg *),
	     saved_hard_reg_compare_func);
      /* Initiate slots available from the previous reload
	 iteration.  */
      prev_save_slots_num = save_slots_num;
      memcpy (prev_save_slots, save_slots, save_slots_num * sizeof (rtx));
      save_slots_num = 0;
      /* Allocate stack slots for the saved hard registers.  */
      for (i = 0; i < saved_regs_num; i++)
	{
	  saved_reg = all_saved_regs[i];
	  regno = saved_reg->hard_regno;
	  for (j = 0; j < i; j++)
	    {
	      saved_reg2 = all_saved_regs[j];
	      if (! saved_reg2->first_p)
		continue;
	      slot = saved_reg2->slot;
	      for (k = j; k >= 0; k = next_k)
		{
		  saved_reg3 = all_saved_regs[k];
		  next_k = saved_reg3->next;
		  if (saved_reg_conflicts[saved_reg->num * saved_regs_num
					  + saved_reg3->num])
		    break;
		}
	      if (k < 0
		  && (GET_MODE_SIZE (regno_save_mode[regno][1])
		      <= GET_MODE_SIZE (regno_save_mode
					[saved_reg2->hard_regno][1])))
		{
		  saved_reg->slot
		    = adjust_address_nv
		      (slot, regno_save_mode[saved_reg->hard_regno][1], 0);
		  regno_save_mem[regno][1] = saved_reg->slot;
		  saved_reg->next = saved_reg2->next;
		  saved_reg2->next = i;
		  if (dump_file != NULL)
		    fprintf (dump_file, "%d uses slot of %d\n",
			     regno, saved_reg2->hard_regno);
		  break;
		}
	    }
	  if (j == i)
	    {
	      saved_reg->first_p = TRUE;
	      for (best_slot_num = -1, j = 0; j < prev_save_slots_num; j++)
		{
		  slot = prev_save_slots[j];
		  if (slot == NULL_RTX)
		    continue;
		  if (GET_MODE_SIZE (regno_save_mode[regno][1])
		      <= GET_MODE_SIZE (GET_MODE (slot))
		      && best_slot_num < 0)
		    best_slot_num = j;
		  if (GET_MODE (slot) == regno_save_mode[regno][1])
		    break;
		}
	      if (best_slot_num >= 0)
		{
		  saved_reg->slot = prev_save_slots[best_slot_num];
		  saved_reg->slot
		    = adjust_address_nv
		      (saved_reg->slot,
		       regno_save_mode[saved_reg->hard_regno][1], 0);
		  if (dump_file != NULL)
		    fprintf (dump_file,
			     "%d uses a slot from prev iteration\n", regno);
		  prev_save_slots[best_slot_num] = NULL_RTX;
		  if (best_slot_num + 1 == prev_save_slots_num)
		    prev_save_slots_num--;
		}
	      else
		{
		  saved_reg->slot
		    = assign_stack_local_1
		      (regno_save_mode[regno][1],
		       GET_MODE_SIZE (regno_save_mode[regno][1]), 0, true);
		  if (dump_file != NULL)
		    fprintf (dump_file, "%d uses a new slot\n", regno);
		}
	      regno_save_mem[regno][1] = saved_reg->slot;
	      save_slots[save_slots_num++] = saved_reg->slot;
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
	    
	    /* We have found an acceptable mode to store in.  Since
	       hard register is always saved in the widest mode
	       available, the mode may be wider than necessary, it is
	       OK to reduce the alignment of spill space.  We will
	       verify that it is equal to or greater than required
	       when we restore and save the hard register in
	       insert_restore and insert_save.  */
	    regno_save_mem[i][j]
	      = assign_stack_local_1 (regno_save_mode[i][j],
				      GET_MODE_SIZE (regno_save_mode[i][j]),
				      0, true);
	    
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



/* This page contains code for global analysis for better placement
   save/restore insn when IRA is used.

   First, we calculate local BB info in function calculate_local_save_info:
     o hard registers set/mentioned in basic blocks (they prevents
       moving corresponding saves/restores further up/down in CFG) --
       see comments for save_kill/restore_kill.
     o hard registers needed to be saved/restored around calls and
       which are not set/mentioned before/after the call in the basic
       block -- see comments for save_gen/restore_gen.
     o free hard registers at the basic block start -- see comments
       for free_gen.  This set can be different from save_gen.  For
       example, in the following situation

       BB:
          ...
          insn using R
          ...
          call through which R live through and should be saved/restored.
          ...

       save_gen contains R because we can put save at the BB start but
       free_gen does not contain R because we can not use R for other
       purposes from the BB start till the insn using R.
       
   Second, we calculate the global info for placement saves/restores
   in functions -- see comments for save_in, save_out, restore_in,
   restore_out, free_in, free_out.

   The equations for the global info calculation are not trivial
   because we can not put saves/restores on CFG edges as the reload
   can not work when new BBs added.  For example, it means that all
   save_out set of BB should be a subset of intersections of save_in
   of all successors of the BB.  Preventing putting the code on edges
   makes data flow problem bidirectional.  The data flow functions are
   also complicated by necessity to propagate saving modes (e.g. x86
   fp register can be saved in XFmode or DFmode).

   The equations do not permit to propagate info into the loops
   because we can not put saves/restores in more frequently executed
   points.  But if the loop does not contains references for a hard
   register, we permit to propagate info for the register into the
   loop because the info will be propagated through all the loop and
   as consequence corresponding saves/restores will be moved through
   the loop.

   Third, using this info we place saves/restores and set up member
   `saved' in function save_call_clobbered_regs.

  */

/* The following structure contains basic block data flow information
   used to optimize save/restore placement.  Data flow equations are
   bidirectional because we don't want to put save/restore code on CFG
   edges.  */
struct bb_info
{
  /* True if save_out/restore_in should be empty for this block.  */
  int empty_save_out_p, empty_restore_in_p;
  /* Hard registers set/mentioned in the BB.  */
  HARD_REG_SET save_kill;
  HARD_REG_SET restore_kill;
  /* Hard registers needed to be saved and this save not killed (see above)
     by an insn in the BB before that.  */
  HARD_REG_SET save_gen;
  /* Hard registers needed to be restored and this restore not killed
     by an insn in the BB after that.  */
  HARD_REG_SET restore_gen;
  /* Hard registers free and not killed by an insn in the BB before
     that.  */
  HARD_REG_SET free_gen;
  /* Registers needed to be saved/restored at the start and end of the
     basic block.  */
  HARD_REG_SET save_in, save_out, restore_in, restore_out;
  /* Registers free at the start and end of the basic block.  */
  HARD_REG_SET free_in, free_out;
  /* Hard registers living at the start/end of the basic block.  */
  HARD_REG_SET live_at_start, live_at_end;
  /* We don't want to generate save/restore insns on edges because it
     changes CFG during the reload.  To prevent this we use the
     following set.  This set defines what hard registers should be
     saved/restored at the start/end of basic block.  */
  HARD_REG_SET save_here, restore_here;
  /* It corresponds to set SAVE_GEN right after the call of
     calculate_local_save_info.  */
  unsigned char save_in_mode[FIRST_PSEUDO_REGISTER];
  /* Saving modes at the end of the basic block.  */
  unsigned char save_out_mode[FIRST_PSEUDO_REGISTER];
  /* Restoring modes at the start of the basic block.  */
  unsigned char restore_in_mode[FIRST_PSEUDO_REGISTER];
  /* It corresponds to set RESTORE_GEN right after the call of
     calculate_local_save_info.  */
  unsigned char restore_out_mode[FIRST_PSEUDO_REGISTER];
  /* Analogous but the corresponding pseudo-register numbers.  */
  int save_in_pseudo[FIRST_PSEUDO_REGISTER];
  int save_out_pseudo[FIRST_PSEUDO_REGISTER];
  int restore_in_pseudo[FIRST_PSEUDO_REGISTER];
  int restore_out_pseudo[FIRST_PSEUDO_REGISTER];
};

/* Macros for accessing data flow information of basic blocks.  */
#define BB_INFO(BB) ((struct bb_info *) (BB)->aux)
#define BB_INFO_BY_INDEX(N) BB_INFO (BASIC_BLOCK(N))

/* The following structure contains loop info necessary for
   save/restore placement optimization.  */
struct loop_info
{
  /* All hard registers mentioned in the loop.  If a pseudo is
     mentioned in the loop, the hard registers assigned to it are also
     believed to be mentioned in the loop.  */
  HARD_REG_SET mentioned_regs;
};

/* Macro for accessing data flow information of LOOP.  */
#define LOOP_INFO(LOOP) ((struct loop_info *) (LOOP)->aux)

/* The function calculates sets KILL, GEN, LIVE_AT_START and
   RESTORE_OUT_MODES corresponding to GEN for basic blocks.  */
static void
calculate_local_save_info (void)
{
  int i, empty_save_out_p, empty_restore_in_p;
  struct insn_chain *chain, *next;
  struct bb_info *bb_info;
  /* Computed in mark_set_regs, holds all registers set by the current
     instruction.  */
  HARD_REG_SET save_kill, restore_kill;
  HARD_REG_SET this_insn_sets, save_gen, restore_gen, free_gen, temp_set;
  unsigned regno;
  reg_set_iterator rsi;
  /* A map: hard register being saved to the mode used for its
     saving.  */
  enum machine_mode save_mode[FIRST_PSEUDO_REGISTER];
  /* A map: hard register being saved to the pseudo-register assigned
     to the hard register at given point.  */
  int save_pseudo[FIRST_PSEUDO_REGISTER];

  CLEAR_HARD_REG_SET (save_gen);
  CLEAR_HARD_REG_SET (restore_gen);
  CLEAR_HARD_REG_SET (free_gen);
  CLEAR_HARD_REG_SET (save_kill);
  CLEAR_HARD_REG_SET (restore_kill);
  empty_save_out_p = FALSE;
  empty_restore_in_p = FALSE;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      save_mode[i] = VOIDmode;
      save_pseudo[i] = -1;
    }

  for (chain = reload_insn_chain; chain != 0; chain = next)
    {
      rtx insn = chain->insn;
      enum rtx_code code = GET_CODE (insn);

      next = chain->next;

      bb_info = BB_INFO_BY_INDEX (chain->block);
      if (INSN_P (insn))
	{
	  CLEAR_HARD_REG_SET (referenced_regs);
	  CLEAR_HARD_REG_SET (modified_regs);
	  mark_referenced_regs (PATTERN (insn), FALSE);
	  AND_COMPL_HARD_REG_SET (restore_gen, referenced_regs);
	  IOR_HARD_REG_SET (restore_kill, referenced_regs);
	  IOR_HARD_REG_SET (save_kill, modified_regs);

	  if (code == CALL_INSN && find_reg_note (insn, REG_NORETURN, NULL))
	    {
	      SET_HARD_REG_SET (save_kill);
	      SET_HARD_REG_SET (restore_kill);
	    }
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
		  
		  /* Remember live_throughout can contain spilled
		     registers when IRA is used.  */
		  if (flag_ira && optimize && r < 0)
		    continue;
		  gcc_assert (r >= 0);

		  nregs = hard_regno_nregs[r][PSEUDO_REGNO_MODE (regno)];
		  mode = HARD_REGNO_CALLER_SAVE_MODE
		         (r, nregs, PSEUDO_REGNO_MODE (regno));
		  if (nregs == 1)
		    {
		      SET_HARD_REG_BIT (hard_regs_to_save, r);
		      save_mode[r] = mode;
		      save_pseudo[r] = regno;
		    }
		  else
		    {
		      while (nregs-- > 0)
			{
			  SET_HARD_REG_BIT (hard_regs_to_save, r + nregs);
			  save_mode[r + nregs]
			    = regno_save_mode[r + nregs][1];
			  save_pseudo[r + nregs] = regno;
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
	      /* Sibcalls are considered to set the return value.  */
	      if (SIBLING_CALL_P (insn) && crtl->return_rtx)
		mark_set_regs (crtl->return_rtx, NULL_RTX, &this_insn_sets);
	      
	      /* Compute which hard regs must be saved before this call.  */
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, call_fixed_reg_set);
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, this_insn_sets);
	      COPY_HARD_REG_SET (used_regs, call_used_reg_set);
	      AND_HARD_REG_SET (hard_regs_to_save, used_regs);
	      IOR_HARD_REG_SET (restore_gen, hard_regs_to_save);
	      COPY_HARD_REG_SET (temp_set, hard_regs_to_save);
	      AND_COMPL_HARD_REG_SET (temp_set, save_kill);
	      AND_COMPL_HARD_REG_SET (temp_set, save_gen);
	      IOR_HARD_REG_SET (save_gen, temp_set);
	      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
		if (TEST_HARD_REG_BIT (temp_set, i))
		  {
		    bb_info->save_in_mode[i] = save_mode[i];
		    bb_info->save_in_pseudo[i] = save_pseudo[i];
		  }
	      COPY_HARD_REG_SET (temp_set, hard_regs_to_save);
	      AND_COMPL_HARD_REG_SET (temp_set, save_kill);
	      AND_COMPL_HARD_REG_SET (temp_set, restore_kill);
	      IOR_HARD_REG_SET (free_gen, temp_set);
	    }
	}

      if (chain->next == 0 || chain->next->block != chain->block)
	{
	  basic_block bb = BASIC_BLOCK (chain->block);
	  edge e;
	  edge_iterator ei;
	  loop_p loop;

	  if (! empty_save_out_p)
	    FOR_EACH_EDGE (e, ei, bb->succs)
	      if (e->flags & EDGE_ABNORMAL)
		{
		  empty_save_out_p = TRUE;
		  break;
		}
	  bb_info->empty_save_out_p = empty_save_out_p;
	  empty_save_out_p = FALSE;
	  if (! empty_restore_in_p)
	    FOR_EACH_EDGE (e, ei, bb->preds)
	      if (e->flags & EDGE_ABNORMAL)
		{
		  empty_restore_in_p = TRUE;
		  break;
		}
	  bb_info->empty_restore_in_p = empty_restore_in_p;
	  empty_restore_in_p = FALSE;
	  COPY_HARD_REG_SET (bb_info->save_gen, save_gen);
	  COPY_HARD_REG_SET (bb_info->restore_gen, restore_gen);
	  COPY_HARD_REG_SET (bb_info->free_gen, free_gen);
	  CLEAR_HARD_REG_SET (bb_info->restore_in);
	  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	    {
	      bb_info->save_out_mode[i] = VOIDmode;
	      bb_info->save_out_pseudo[i] = -1;
	      if (!TEST_HARD_REG_BIT (save_gen, i))
		{
		  bb_info->save_in_mode[i] = VOIDmode;
		  bb_info->save_in_pseudo[i] = -1;
		}
	      bb_info->restore_in_mode[i] = VOIDmode;
	      bb_info->restore_in_pseudo[i] = -1;
	      if (TEST_HARD_REG_BIT (restore_gen, i))
		{
		  bb_info->restore_out_mode[i] = save_mode[i];
		  bb_info->restore_out_pseudo[i] = save_pseudo[i];
		}
	      else
		{
		  bb_info->restore_out_mode[i] = VOIDmode;
		  bb_info->restore_out_pseudo[i] = -1;
		}
	    }
	  COPY_HARD_REG_SET (bb_info->save_kill, save_kill);
	  COPY_HARD_REG_SET (bb_info->restore_kill, restore_kill);
	  for (loop = bb->loop_father;
	       loop != current_loops->tree_root;
	       loop = loop_outer (loop))
	    {
	      struct loop_info *loop_info = LOOP_INFO (loop);
	      
	      IOR_HARD_REG_SET (loop_info->mentioned_regs, save_kill);
	      IOR_HARD_REG_SET (loop_info->mentioned_regs, restore_kill);
	    }
	  CLEAR_HARD_REG_SET (bb_info->save_in);
	  CLEAR_HARD_REG_SET (bb_info->restore_out);
	  CLEAR_HARD_REG_SET (bb_info->free_in);
	  /* We don't use LIVE for IRA.  */
	  REG_SET_TO_HARD_REG_SET
	    (bb_info->live_at_end,
	     flag_ira ? DF_LR_OUT (bb) : DF_LIVE_OUT (bb));
	  EXECUTE_IF_SET_IN_REG_SET
	    ((flag_ira ? DF_LR_OUT (bb) : DF_LIVE_OUT (bb)),
	     FIRST_PSEUDO_REGISTER, regno, rsi)
	    {
	      int r = reg_renumber[regno];
	      int nregs;
	      
	      if (r < 0)
		continue;
	      nregs = hard_regno_nregs[r][PSEUDO_REGNO_MODE (regno)];
	      while (nregs-- > 0)
		SET_HARD_REG_BIT (bb_info->live_at_end, r + nregs);
	    }
	  REG_SET_TO_HARD_REG_SET
	    (bb_info->live_at_start,
	     flag_ira ? DF_LR_IN (bb) : DF_LIVE_IN (bb));
	  EXECUTE_IF_SET_IN_REG_SET
	    ((flag_ira ? DF_LR_IN (bb) : DF_LIVE_IN (bb)),
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
	  CLEAR_HARD_REG_SET (bb_info->restore_here);
	  CLEAR_HARD_REG_SET (save_gen);
	  CLEAR_HARD_REG_SET (restore_gen);
	  CLEAR_HARD_REG_SET (free_gen);
	  CLEAR_HARD_REG_SET (save_kill);
	  CLEAR_HARD_REG_SET (restore_kill);
	  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	    {
	      save_mode[i] = VOIDmode;
	      save_pseudo[i] = -1;
	    }
	}
    }
}

/* The transfer function used by the DF equation solver to propagate restore
   info through block with BB_INDEX according to the following
   equation:

   bb.restore_out = (bb.restore_in - bb.restore_kill) U bb.restore_gen.

   The function also propagates saving modes of the hard
   registers.  */
static bool
restore_trans_fun (int bb_index)
{
  int i;
  HARD_REG_SET temp_set;
  struct bb_info *bb_info = BB_INFO_BY_INDEX (bb_index);

  COPY_HARD_REG_SET (temp_set, bb_info->restore_in);
  AND_COMPL_HARD_REG_SET (temp_set, bb_info->restore_kill);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (temp_set, i)
	&& ! TEST_HARD_REG_BIT (bb_info->restore_gen, i))
      {
	gcc_assert (bb_info->restore_out_mode[i] == VOIDmode
		    || ((bb_info->restore_out_mode[i]
			 == bb_info->restore_in_mode[i])
			&& (bb_info->restore_out_pseudo[i]
			    == bb_info->restore_in_pseudo[i])));
	bb_info->restore_out_mode[i] = bb_info->restore_in_mode[i];
	bb_info->restore_out_pseudo[i] = bb_info->restore_in_pseudo[i];
      }
  IOR_HARD_REG_SET (temp_set, bb_info->restore_gen);
  if (! hard_reg_set_equal_p (temp_set, bb_info->restore_out))
    {
      COPY_HARD_REG_SET (bb_info->restore_out, temp_set);
      return true;
    }
  return false;
}

/* The confluence function used by the DF equation solver to set up restore info
   for a block BB without predecessor.  */
static void
restore_con_fun_0 (basic_block bb)
{
  CLEAR_HARD_REG_SET (BB_INFO (bb)->restore_in);
}

/* The confluence function used by the DF equation solver to propagate
   restore info from predecessor to successor on edge E (pred->bb)
   according to the following equation:

     bb.restore_in  = empty for entry block or one with empty_restore_in_p
                    | ^(pred.restore_out - pred.restore_here) ^ bb.live_at_start
		    
   If the edge is a loop enter we do not propagate the info because we
   don't want to put restores in more frequently executed places.

 */
static void
restore_con_fun_n (edge e)
{
  int i;
  HARD_REG_SET temp_set;
  basic_block pred = e->src;
  basic_block bb = e->dest;
  struct bb_info *bb_info = BB_INFO (bb);
  
  if (bb_info->empty_restore_in_p)
    return;

  COPY_HARD_REG_SET (temp_set, BB_INFO (pred)->restore_out);
  AND_COMPL_HARD_REG_SET (temp_set, BB_INFO (pred)->restore_here);
  if (bb->loop_depth > pred->loop_depth)
    AND_COMPL_HARD_REG_SET (temp_set,
			    LOOP_INFO (bb->loop_father)->mentioned_regs);
  AND_HARD_REG_SET (temp_set, bb_info->live_at_start);
  if (EDGE_PRED (bb, 0) == e)
    COPY_HARD_REG_SET (bb_info->restore_in, temp_set);
  else
    AND_HARD_REG_SET (bb_info->restore_in, temp_set);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (temp_set, i))
      {
	if (bb_info->restore_in_mode[i] != VOIDmode
	    && BB_INFO (pred)->restore_out_mode[i] != VOIDmode
	    && (bb_info->restore_in_mode[i]
		!= BB_INFO (pred)->restore_out_mode[i]
		|| bb_info->restore_in_pseudo[i]
		!= BB_INFO (pred)->restore_out_pseudo[i]))
	  CLEAR_HARD_REG_BIT (bb_info->restore_in, i);
	else if (BB_INFO (pred)->restore_out_mode[i] != VOIDmode)
	  {
	    bb_info->restore_in_mode[i]
	      = BB_INFO (pred)->restore_out_mode[i];
	    bb_info->restore_in_pseudo[i]
	      = BB_INFO (pred)->restore_out_pseudo[i];
	  }
      }
}

/* Basic blocks for data flow problem -- all bocks except the special
   ones.  */
static bitmap all_blocks;

/* The function calculates global restore information according
   to the following equations:

     bb.restore_in  = empty for entry block or one with empty_restore_in_p
                    | ^(pred.restore_out - pred.restore_here) ^ bb.live_at_start
     bb.restore_out = (bb.restore_in - bb.restore_kill) U bb.restore_gen.

   The function also calculates restore_in_mode and restore_out_mode.
*/
static void
calculate_restore_in_out (void)
{
  df_simple_dataflow (DF_FORWARD, NULL, restore_con_fun_0, restore_con_fun_n,
		      restore_trans_fun, all_blocks,
		      df_get_postorder (DF_FORWARD),
		      df_get_n_blocks (DF_FORWARD));
}

/* The function calculates RESTORE_HERE according to the equation
   bb.restore_here = U ((bb.restore_out - succ.restore_in)
                        ^ succ.live_at_start).  */
static int
calculate_restore_here (void)
{
  basic_block bb;
  edge e;
  edge_iterator ei;
  HARD_REG_SET restore_here, temp_set;
  int changed_p = FALSE;

  FOR_EACH_BB (bb)
    {
      CLEAR_HARD_REG_SET (restore_here);
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  basic_block dest = e->dest;

	  COPY_HARD_REG_SET (temp_set, BB_INFO (bb)->restore_out);
	  AND_COMPL_HARD_REG_SET (temp_set, BB_INFO (dest)->restore_in);
	  AND_HARD_REG_SET (temp_set, BB_INFO (dest)->live_at_start);
	  IOR_HARD_REG_SET (restore_here, temp_set);
	}

      if (! hard_reg_set_equal_p (restore_here, BB_INFO (bb)->restore_here))
	{
	  COPY_HARD_REG_SET (BB_INFO (bb)->restore_here, restore_here);
	  changed_p = TRUE;
	}
    }
  return changed_p;
}

/* The transfer function used by the DF equation solver to propagate save info
   through block with BB_INDEX according to the following
   equation:

   bb.save_in = ((bb.save_out - bb.save_kill) U bb.save_gen) - bb.restore_in.

   The function also propagates saving modes of the hard
   registers.  */

static bool
save_trans_fun (int bb_index)
{
  int i;
  HARD_REG_SET temp_set;
  struct bb_info *bb_info = BB_INFO_BY_INDEX (bb_index);

  COPY_HARD_REG_SET (temp_set, bb_info->save_out);
  AND_COMPL_HARD_REG_SET (temp_set, bb_info->save_kill);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (temp_set, i)
	&& ! TEST_HARD_REG_BIT (bb_info->save_gen, i))
      {
	gcc_assert (bb_info->save_in_mode[i] == VOIDmode
		    || ((bb_info->save_in_mode[i]
			 == bb_info->save_out_mode[i])
			&& (bb_info->save_in_pseudo[i]
			    == bb_info->save_out_pseudo[i])));
	bb_info->save_in_mode[i] = bb_info->save_out_mode[i];
	bb_info->save_in_pseudo[i] = bb_info->save_out_pseudo[i];
      }
  IOR_HARD_REG_SET (temp_set, bb_info->save_gen);
  AND_COMPL_HARD_REG_SET (temp_set, bb_info->restore_in);
  if (! hard_reg_set_equal_p (temp_set, bb_info->save_in))
    {
      COPY_HARD_REG_SET (bb_info->save_in, temp_set);
      return true;
    }
  return false;
}

/* The confluence function used by the DF equation solver to set up
   save info for a block BB without successor.  */
static void
save_con_fun_0 (basic_block bb)
{
  CLEAR_HARD_REG_SET (BB_INFO (bb)->save_out);
}

/* The confluence function used by the DF equation solver to propagate
   save info from successor to predecessor on edge E (bb->succ)
   according to the following equation:

     bb.save_out  = empty for exit block or one with empty_save_out_p
                    | ^(succ.save_in - succ.save_here) ^ bb.live_at_end

   If the edge is a loop exit we do not propagate the info because we
   don't want to put saves in more frequently executed places.
*/
static void
save_con_fun_n (edge e)
{
  int i;
  HARD_REG_SET temp_set;
  basic_block succ = e->dest;
  basic_block bb = e->src;
  struct bb_info *bb_info = BB_INFO (bb);
  
  if (bb_info->empty_save_out_p)
    return;

  COPY_HARD_REG_SET (temp_set, BB_INFO (succ)->save_in);
  AND_COMPL_HARD_REG_SET (temp_set, BB_INFO (succ)->save_here);
  if (bb->loop_depth > succ->loop_depth)
    AND_COMPL_HARD_REG_SET (temp_set,
			    LOOP_INFO (bb->loop_father)->mentioned_regs);
  AND_HARD_REG_SET (temp_set, bb_info->live_at_end);
  if (EDGE_SUCC (bb, 0) == e)
    COPY_HARD_REG_SET (bb_info->save_out, temp_set);
  else
    AND_HARD_REG_SET (bb_info->save_out, temp_set);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (temp_set, i))
      {
	if (bb_info->save_out_mode[i] != VOIDmode
	    && BB_INFO (succ)->save_in_mode[i] != VOIDmode
	    && ((bb_info->save_out_mode[i]
		 != BB_INFO (succ)->save_in_mode[i])
		|| (bb_info->save_out_pseudo[i]
		    != BB_INFO (succ)->save_in_pseudo[i])))
	  CLEAR_HARD_REG_BIT (bb_info->save_out, i);
	else if (BB_INFO (succ)->save_in_mode[i] != VOIDmode)
	  {
	    bb_info->save_out_mode[i]
	      = BB_INFO (succ)->save_in_mode[i];
	    bb_info->save_out_pseudo[i]
	      = BB_INFO (succ)->save_in_pseudo[i];
	  }
      }
}

/* The transfer function calculates global save information according
   to the following equations:

     bb.save_out  = empty for exit block or one with empty_save_out_p
                    | ^(succ.save_in - succ.save_here) ^ bb.live_at_end
     bb.save_in = ((bb.save_out - bb.save_kill) U bb.save_gen) - bb.restore_in.

   The function also calculates save_in_mode and save_out_mode.
*/
static void
calculate_save_in_out (void)
{
  df_simple_dataflow (DF_BACKWARD, NULL, save_con_fun_0, save_con_fun_n,
		      save_trans_fun, all_blocks,
		      df_get_postorder (DF_BACKWARD),
		      df_get_n_blocks (DF_BACKWARD));
}

/* The function calculates SAVE_HERE according to the equation
   bb.save_here = U ((bb.save_in - pred.save_out) ^ pred.live_at_end).  */
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
      FOR_EACH_EDGE (e, ei, bb->preds)
	{
	  basic_block src = e->src;

	  COPY_HARD_REG_SET (temp_set, BB_INFO (bb)->save_in);
	  AND_COMPL_HARD_REG_SET (temp_set, BB_INFO (src)->save_out);
	  AND_HARD_REG_SET (temp_set, BB_INFO (src)->live_at_end);
	  IOR_HARD_REG_SET (save_here, temp_set);
	}

      if (! hard_reg_set_equal_p (save_here, BB_INFO (bb)->save_here))
	{
	  COPY_HARD_REG_SET (BB_INFO (bb)->save_here, save_here);
	  changed_p = TRUE;
	}
    }
  return changed_p;
}

/* The transfer function used by the DF equation solver to propagate
   free info through block with BB_INDEX according to the following
   equation:

   bb.free_in = ((bb.free_out - bb.save_kill - bb.restore_kill) U bb.free_gen)
                 - bb.save_here)

*/
static bool
free_trans_fun (int bb_index)
{
  HARD_REG_SET temp_set;
  struct bb_info *bb_info = BB_INFO_BY_INDEX (bb_index);

  COPY_HARD_REG_SET (temp_set, bb_info->free_out);
  AND_COMPL_HARD_REG_SET (temp_set, bb_info->save_kill);
  AND_COMPL_HARD_REG_SET (temp_set, bb_info->restore_kill);
  IOR_HARD_REG_SET (temp_set, bb_info->free_gen);
  AND_COMPL_HARD_REG_SET (temp_set, bb_info->save_here);
  if (! hard_reg_set_equal_p (temp_set, bb_info->free_in))
    {
      COPY_HARD_REG_SET (bb_info->free_in, temp_set);
      return true;
    }
  return false;
}

/* The confluence function used by the DF equation solver to set up
   free info for a block BB without successor.  */
static void
free_con_fun_0 (basic_block bb)
{
  CLEAR_HARD_REG_SET (BB_INFO (bb)->free_out);
}

/* The confluence function used by the DF equation solver to propagate
   free info from successor to predecessor on edge E (bb->succ)
   according to the following equation:

     bb.free_out = ^(succ.free_in ^ bb.live_at_end)
*/
static void
free_con_fun_n (edge e)
{
  HARD_REG_SET temp_set;
  basic_block succ = e->dest;
  basic_block bb = e->src;
  struct bb_info *bb_info = BB_INFO (bb);
  
  COPY_HARD_REG_SET (temp_set, BB_INFO (succ)->free_in);
  if (EDGE_SUCC (bb, 0) == e)
    COPY_HARD_REG_SET (bb_info->free_out, temp_set);
  else
    AND_HARD_REG_SET (bb_info->free_out, temp_set);
  AND_HARD_REG_SET (bb_info->free_out, bb_info->live_at_end);
}

/* The function calculates global free information according
   to the following equations:

   bb.free_out = ^(succ.free_in ^ bb.live_at_end)
   bb.free_in = ((bb.free_out - bb.save_kill - bb.restore_kill) U bb.free_gen)
                 - bb.save_here)

   The function also calculates free_in_mode and free_out_mode.
*/
static void
calculate_free_in_out (void)
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      struct bb_info *bb_info = BB_INFO (bb);

      COPY_HARD_REG_SET (bb_info->free_in, bb_info->save_in);
      IOR_HARD_REG_SET (bb_info->free_in, bb_info->restore_in);
      COPY_HARD_REG_SET (bb_info->free_out, bb_info->save_out);
      IOR_HARD_REG_SET (bb_info->free_out, bb_info->restore_out);
    }
  df_simple_dataflow (DF_BACKWARD, NULL, free_con_fun_0, free_con_fun_n,
		      free_trans_fun, all_blocks,
		      df_get_postorder (DF_BACKWARD),
		      df_get_n_blocks (DF_BACKWARD));
}

/* The function calculates the global save/restore information used to put
   save/restore code without generating new blocks.  This is a
   bidirectional data flow problem (calculation of SAVE_IN and
   SAVE_OUT is a backward data flow problem and SAVE_HERE is forward
   one; calculation of RESTORE_IN and RESTORE_OUT is a forward data
   flow problem and RESTORE_HERE is backward one).  It is complicated
   by necessity of calculation of modes for saving/restoring callee
   clobbered hard registers.  */
static void
make_global_save_analysis (void)
{
  basic_block bb;
  int iter, changed_p;

  all_blocks = BITMAP_ALLOC (NULL);
  FOR_ALL_BB (bb)
    {
      bitmap_set_bit (all_blocks, bb->index);
    }
  calculate_local_save_info ();
  for (iter = 1;; iter++)
    {
      calculate_restore_in_out ();
      changed_p = calculate_restore_here ();
      if (! changed_p)
	break;
    }
  if (dump_file != NULL)
    fprintf (dump_file, " Number of global restore analysis iterations %d\n",
	     iter);
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
  calculate_free_in_out ();
  BITMAP_FREE (all_blocks);
}

/* Print hard registers in SET to file F.  The registers are printed
   with its mode given in MODES and corresponding pseudo given in
   PSEUDOS.  */
static void
print_annotated_hard_reg_set (FILE *f, HARD_REG_SET set,
			      unsigned char *modes, int *pseudos)
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (set, i))
      {
	fprintf (f, " %d", i);
	if (pseudos[i] >= 0)
	  fprintf (f, "(%d)", pseudos[i]);
	fprintf (f, ":%s %s", GET_MODE_NAME (modes[i]), reg_names[i]);
      }
}

/* Print hard registers in SET to file F.  */  
static void
print_hard_reg_set (FILE *f, HARD_REG_SET set)
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (set, i))
      fprintf (f, " %d %s", i, reg_names[i]);
}

/* Print the save information for each block to file F.  */  
static void
print_save_data (FILE *f)
{
  basic_block bb;
  struct bb_info *bb_info;
  edge e;
  edge_iterator ei;

  FOR_EACH_BB (bb)
    {
      bb_info = BB_INFO (bb);
      fprintf (f, "bb %d (preds", bb->index);
      FOR_EACH_EDGE (e, ei, bb->preds)
	{
	  fprintf (f, " %d", e->src->index);
	}
      fprintf (f, ") (succs");
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  fprintf (f, " %d", e->dest->index);
	}
      fprintf (f, ")\n  empty_save_out_p=%d", bb_info->empty_save_out_p);
      fprintf (f, ", empty_restore_in_p=%d", bb_info->empty_restore_in_p);
      fprintf (f, "\n  save_in:");
      print_annotated_hard_reg_set
	(f, bb_info->save_in, bb_info->save_in_mode, bb_info->save_in_pseudo);
      fprintf (f, "\n  save_out:");
      print_annotated_hard_reg_set
	(f, bb_info->save_out,
	 bb_info->save_out_mode, bb_info->save_out_pseudo);
      fprintf (f, "\n  restore_in:");
      print_annotated_hard_reg_set
	(f, bb_info->restore_in,
	 bb_info->restore_in_mode, bb_info->restore_in_pseudo);
      fprintf (f, "\n  restore_out:");
      print_annotated_hard_reg_set
	(f, bb_info->restore_out,
	 bb_info->restore_out_mode, bb_info->restore_out_pseudo);
      fprintf (f, "\n  free_in:");
      print_hard_reg_set (f, bb_info->free_in);
      fprintf (f, "\n  free_out:");
      print_hard_reg_set (f, bb_info->free_out);
      fprintf (f, "\n  live_at_start:");
      print_hard_reg_set (f, bb_info->live_at_start);
      fprintf (f, "\n  live_at_end:");
      print_hard_reg_set (f, bb_info->live_at_end);
      fprintf (f, "\n  save_here:");
      print_hard_reg_set (f, bb_info->save_here);
      fprintf (f, "\n  restore_here:");
      print_hard_reg_set (f, bb_info->restore_here);
      fprintf (f, "\n  save_kill:");
      print_hard_reg_set (f, bb_info->save_kill);
      fprintf (f, "\n  restore_kill:");
      print_hard_reg_set (f, bb_info->restore_kill);
      fprintf (f, "\n  save_gen:");
      print_hard_reg_set (f, bb_info->save_gen);
      fprintf (f, "\n  restore_gen:");
      print_hard_reg_set (f, bb_info->restore_gen);
      fprintf (f, "\n  free_gen:");
      print_hard_reg_set (f, bb_info->save_gen);
      fprintf (f, "\n\n");
    }
}

/* Print the save information for each block to stderr.  */  
void
debug_save_data (void)
{
  print_save_data (stderr);
}


/* Setup hard registers in SET to save.  Setup also their save modes
   in SAVE_MODE from FROM_SAVE_MODE and their pseudos in SAVE_PSEUDO
   from FROM_SAVE_PSEUDO.  */  
static void
set_hard_reg_saved (HARD_REG_SET set, unsigned char *from_saved_mode,
		    enum machine_mode *save_mode, int *from_saved_pseudo,
		    int *save_pseudo)
{
  int regno;

  n_regs_saved = 0;
  COPY_HARD_REG_SET (hard_regs_saved, set);
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    if (TEST_HARD_REG_BIT (set, regno))
      {
	n_regs_saved++;
	save_mode[regno] = from_saved_mode[regno];
	save_pseudo[regno] = from_saved_pseudo[regno];
      }
    else
      {
	save_mode[regno] = VOIDmode;
	save_pseudo[regno] = -1;
      }
}



/* Find the places where hard regs are live across calls and save them.  */

void
save_call_clobbered_regs (void)
{
  unsigned int regno;
  /* Computed in mark_set_regs, holds all registers set by the current
     instruction.  */
  HARD_REG_SET this_insn_sets, hard_regs_to_save, saved;
  struct insn_chain *chain, *last, *next, *prev, *last_restore_chain, *where;
  struct bb_info *bb_info;
  enum machine_mode save_mode[FIRST_PSEUDO_REGISTER];
  loop_p loop;
  loop_iterator li;
  int save_pseudo[FIRST_PSEUDO_REGISTER];
  int free_pseudo[FIRST_PSEUDO_REGISTER];
  bool do_placement_opt_p = 0 /* flag_ira && optimize */;

  if (do_placement_opt_p)
    {
      /* Do global analysis for better placement of spill code. */
      alloc_aux_for_blocks (sizeof (struct bb_info));
      FOR_EACH_LOOP (li, loop, 0)
	{
	  loop->aux = xmalloc (sizeof (struct loop_info));
	  CLEAR_HARD_REG_SET (LOOP_INFO (loop)->mentioned_regs);
	}
      memset (BB_INFO (ENTRY_BLOCK_PTR), 0, sizeof (struct bb_info));
      memset (BB_INFO (EXIT_BLOCK_PTR), 0, sizeof (struct bb_info));
      make_global_save_analysis ();
    }

  CLEAR_HARD_REG_SET (hard_regs_saved);
  n_regs_saved = 0;

  if (do_placement_opt_p && reload_insn_chain != NULL)
    {
      bb_info = BB_INFO_BY_INDEX (reload_insn_chain->block);
      set_hard_reg_saved (bb_info->restore_in,
			  bb_info->restore_in_mode, save_mode,
			  bb_info->restore_in_pseudo, save_pseudo);
    }

  last = NULL;
  for (chain = reload_insn_chain; chain != 0; chain = next)
    {
      rtx insn = chain->insn;
      enum rtx_code code = GET_CODE (insn);

      last = chain;
      next = chain->next;

      if (INSN_P (insn))
	{
	  if (n_regs_saved)
	    {
	      if (!do_placement_opt_p && code == JUMP_INSN)
		/* Restore all registers if this is a JUMP_INSN.  */
		COPY_HARD_REG_SET (referenced_regs, hard_regs_saved);
	      else
		{
		  CLEAR_HARD_REG_SET (referenced_regs);
		  CLEAR_HARD_REG_SET (modified_regs);
		  mark_referenced_regs (PATTERN (insn), FALSE);
		  AND_HARD_REG_SET (referenced_regs, hard_regs_saved);
		}

	      /* If some registers have been saved, see if INSN
		 references any of them.  We must restore them before
		 the insn if so.  */

	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (TEST_HARD_REG_BIT (referenced_regs, regno))
		  {
		    unsigned int before = regno;
		    
		    regno += insert_restore (chain, 1, regno, MOVE_MAX_WORDS,
					     save_mode, save_pseudo);
		    if (do_placement_opt_p)
		      {
			gcc_assert (before == regno);
			save_mode[before] = VOIDmode;
			save_pseudo[before] = -1;
		      }
		  }
	    }

	  if (code == CALL_INSN
	      && ! SIBLING_CALL_P (insn)
	      && ! find_reg_note (insn, REG_NORETURN, NULL))
	    {
	      HARD_REG_SET used_regs;
	      reg_set_iterator rsi;

	      /* Use the register life information in CHAIN to compute which
		 regs are live during the call.  */
	      REG_SET_TO_HARD_REG_SET (hard_regs_to_save,
				       &chain->live_throughout);
	      /* Save hard registers always in the widest mode available.  */
	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		{
		  if (TEST_HARD_REG_BIT (hard_regs_to_save, regno))
		    save_mode[regno] = regno_save_mode[regno][1];
		  else
		    save_mode[regno] = VOIDmode;
		  save_pseudo[regno] = -1;
		}

	      /* Look through all live pseudos, mark their hard registers
		 and choose proper mode for saving.  */
	      EXECUTE_IF_SET_IN_REG_SET
		(&chain->live_throughout, FIRST_PSEUDO_REGISTER, regno, rsi)
		{
		  int r = reg_renumber[regno];
		  int nregs;
		  enum machine_mode mode;

		  /* Remember live_throughout can contain spilled
		     registers when IRA is used.  */
		  if (flag_ira && optimize && r < 0)
		    continue;
		  gcc_assert (r >= 0);

		  nregs = hard_regno_nregs[r][PSEUDO_REGNO_MODE (regno)];
		  mode = HARD_REGNO_CALLER_SAVE_MODE
		         (r, nregs, PSEUDO_REGNO_MODE (regno));
		  if (GET_MODE_BITSIZE (mode)
		      > GET_MODE_BITSIZE (save_mode[r]))
		    save_mode[r] = mode;
		  while (nregs-- > 0)
		    {
		      SET_HARD_REG_BIT (hard_regs_to_save, r + nregs);
		      save_pseudo[r + nregs] = regno;
		    }
		}

	      /* Record all registers set in this call insn.  These don't need
		 to be saved.  N.B. the call insn might set a subreg of a
		 multi-hard-reg pseudo; then the pseudo is considered live
		 during the call, but the subreg that is set isn't.  */
	      CLEAR_HARD_REG_SET (this_insn_sets);
	      note_stores (PATTERN (insn), mark_set_regs, &this_insn_sets);

	      /* Compute which hard regs must be saved before this call.  */
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, call_fixed_reg_set);
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, this_insn_sets);
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, hard_regs_saved);
	      COPY_HARD_REG_SET (used_regs, call_used_reg_set);
	      AND_HARD_REG_SET (hard_regs_to_save, used_regs);

	      if (do_placement_opt_p)
		IOR_HARD_REG_SET (hard_regs_saved, hard_regs_to_save);
	      else
		{
		  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		    if (TEST_HARD_REG_BIT (hard_regs_to_save, regno))
		      regno += insert_save (chain, 1, regno,
					    &hard_regs_to_save, save_mode,
					    save_pseudo);
		  
		}
	     
	      /* Must recompute n_regs_saved.  */
	      n_regs_saved = 0;
	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (TEST_HARD_REG_BIT (hard_regs_saved, regno))
		  n_regs_saved++;
	    }
	}

      if (chain->next == 0 || chain->next->block != chain->block)
	{
	  struct bb_info *next_bb_info;

	  next_bb_info = (chain->next != NULL
			  ? BB_INFO_BY_INDEX (chain->next->block) : NULL);

	  /* At the end of the basic block, we must restore any registers that
	     remain saved.  If the last insn in the block is a JUMP_INSN, put
	     the restore before the insn, otherwise, put it after the insn.  */

	  if (do_placement_opt_p)
	    set_hard_reg_saved
	      (BB_INFO_BY_INDEX (chain->block)->restore_here,
	       BB_INFO_BY_INDEX (chain->block)->restore_out_mode, save_mode,
	       BB_INFO_BY_INDEX (chain->block)->restore_out_pseudo,
	       save_pseudo);

	  if (n_regs_saved)
	    for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
	      if (TEST_HARD_REG_BIT (hard_regs_saved, regno))
		{
		  unsigned int before = regno;

		  regno += insert_restore (chain, JUMP_P (insn),
					   regno, MOVE_MAX_WORDS, save_mode,
					   save_pseudo);
		  gcc_assert (before == regno);
		  save_mode[before] = VOIDmode;
		  save_pseudo[before] = -1;
		}

	  if (do_placement_opt_p && next_bb_info != NULL)
	    set_hard_reg_saved (next_bb_info->restore_in,
				next_bb_info->restore_in_mode, save_mode,
				next_bb_info->restore_in_pseudo, save_pseudo);

	}
    }

  if (!do_placement_opt_p)
    return;

  CLEAR_HARD_REG_SET (hard_regs_to_save);
  n_regs_saved = 0;
  last_restore_chain = NULL;
  
  if (last == NULL)
    CLEAR_HARD_REG_SET (saved);
  else
    {
      bb_info = BB_INFO_BY_INDEX (last->block);
      set_hard_reg_saved (bb_info->save_out,
			  bb_info->save_out_mode, save_mode,
			  bb_info->save_out_pseudo, save_pseudo);
      COPY_HARD_REG_SET (hard_regs_to_save, hard_regs_saved);
      COPY_HARD_REG_SET (saved, bb_info->free_out);
      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
	if (!TEST_HARD_REG_BIT (saved, regno))
	  free_pseudo[regno] = -1;
	else
	  {
	    if (TEST_HARD_REG_BIT (bb_info->save_out, regno))
	      free_pseudo [regno] = bb_info->save_out_pseudo [regno];
	    else
	      {
		gcc_assert (TEST_HARD_REG_BIT (bb_info->restore_out, regno));
		free_pseudo [regno] = bb_info->restore_out_pseudo [regno];
	      }
	  }
    }
  for (chain = last; chain != 0; chain = prev)
    {
      rtx insn = chain->insn;
      enum rtx_code code = GET_CODE (insn);
      
      prev = chain->prev;
      
      if (INSN_P (insn))
	{
	  bb_info = BB_INFO_BY_INDEX (chain->block);
	  
	  CLEAR_HARD_REG_SET (referenced_regs);
	  CLEAR_HARD_REG_SET (modified_regs);
	  mark_referenced_regs (PATTERN (insn), FALSE);
	  AND_HARD_REG_SET (modified_regs, hard_regs_to_save);
	  AND_COMPL_HARD_REG_SET (saved, referenced_regs);
	  AND_COMPL_HARD_REG_SET (saved, modified_regs);
	  
	  if (chain->is_caller_save_insn)
	    {
	      if (last_restore_chain == NULL)
		last_restore_chain = chain;
	    }
	  else
	    {
	      /* If some registers have been saved, see if INSN
		 references any of them.  We must restore them before
		 the insn if so.  */
	      
	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (TEST_HARD_REG_BIT (modified_regs, regno))
		  {
		    unsigned int before = regno;
		    
		    /* We should put save insns before restore insns
		       between the two calls because the same stack
		       slot for different hard registers can be used
		       for restoring in the first call and saving for
		       the second call.  */
		    regno += insert_save (last_restore_chain != NULL
					  ? last_restore_chain : chain,
					  last_restore_chain == NULL
					  && JUMP_P (chain->insn),
					  regno, &hard_regs_to_save,
					  save_mode, save_pseudo);
		    gcc_assert (before == regno);

		    CLEAR_HARD_REG_BIT (saved, regno);
		    CLEAR_HARD_REG_BIT (hard_regs_to_save, regno);
		    save_mode[before] = VOIDmode;
		    save_pseudo[before] = -1;
		    free_pseudo[before] = -1;
		  }
	    }

	  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
	    if (TEST_HARD_REG_BIT (saved, regno) && free_pseudo[regno] >= 0)
	      SET_REGNO_REG_SET (&chain->saved, free_pseudo[regno]);
	  if (chain->is_caller_save_insn && chain->saved_pseudo_regno >= 0)
	    {
	      int i;
	      enum machine_mode mode;

	      mode = GET_MODE (regno_reg_rtx[chain->saved_pseudo_regno]);
	      regno = reg_renumber[chain->saved_pseudo_regno];
	      add_to_hard_reg_set (&saved, mode, regno);
	      for (i = hard_regno_nregs[regno][mode] - 1; i >= 0; i--)
		free_pseudo[regno + i] = chain->saved_pseudo_regno;
	    }
	  if (code == CALL_INSN
	      && ! SIBLING_CALL_P (insn)
	      && ! find_reg_note (insn, REG_NORETURN, NULL))
	    {
	      HARD_REG_SET used_regs;
	      reg_set_iterator rsi;
	      
	      last_restore_chain = NULL;
	      
	      /* Use the register life information in CHAIN to
		 compute which regs are live during the call.  */
	      REG_SET_TO_HARD_REG_SET (hard_regs_to_save,
				       &chain->live_throughout);
	      /* Save hard registers always in the widest mode
		 available.  */
	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		{
		  if (TEST_HARD_REG_BIT (hard_regs_to_save, regno))
		    {
		      CLEAR_HARD_REG_SET (this_insn_sets);
		      note_stores (PATTERN (insn), mark_set_regs,
				   &this_insn_sets);
		      save_mode[regno] = regno_save_mode[regno][1];
		    }
		  else
		    save_mode[regno] = VOIDmode;
		  save_pseudo[regno] = -1;
		}
	      
	      /* Look through all live pseudos, mark their hard
		 registers and choose proper mode for saving.  */
	      EXECUTE_IF_SET_IN_REG_SET
		(&chain->live_throughout, FIRST_PSEUDO_REGISTER, regno, rsi)
		{
		  int r = reg_renumber[regno];
		  int nregs;
		  enum machine_mode mode;
		  
		  /* Remember live_throughout can contain spilled
		     registers when IRA is used.  */
		  if (flag_ira && r < 0)
		    continue;
		  gcc_assert (r >= 0);

		  nregs = hard_regno_nregs[r][PSEUDO_REGNO_MODE (regno)];
		  mode = HARD_REGNO_CALLER_SAVE_MODE
		         (r, nregs, PSEUDO_REGNO_MODE (regno));
		  if (GET_MODE_BITSIZE (mode)
		      > GET_MODE_BITSIZE (save_mode[r]))
		    save_mode[r] = mode;
		  while (nregs-- > 0)
		    {
		      SET_HARD_REG_BIT (hard_regs_to_save, r + nregs);
		      save_pseudo[r + nregs] = regno;
		      free_pseudo[r + nregs] = regno;
		    }
		}
	      
	      /* Record all registers set in this call insn.  These
		 don't need to be saved.  N.B. the call insn might set
		 a subreg of a multi-hard-reg pseudo; then the pseudo
		 is considered live during the call, but the subreg
		 that is set isn't.  */
	      CLEAR_HARD_REG_SET (this_insn_sets);
	      note_stores (PATTERN (insn), mark_set_regs, &this_insn_sets);
	      
	      /* Compute which hard regs must be saved before this call.  */
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, call_fixed_reg_set);
	      AND_COMPL_HARD_REG_SET (hard_regs_to_save, this_insn_sets);
	      COPY_HARD_REG_SET (used_regs, call_used_reg_set);
	      AND_HARD_REG_SET (hard_regs_to_save, used_regs);
	      COPY_HARD_REG_SET (saved, hard_regs_to_save);
	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (!TEST_HARD_REG_BIT (saved, regno))
		  {
		    free_pseudo[regno] = -1;
		    save_pseudo[regno] = -1;
		    save_mode[regno] = VOIDmode;
		  }
	    }
	}
      
      if (chain->prev == 0 || chain->prev->block != chain->block)
	{
	  struct bb_info *prev_bb_info;
	  
	  prev_bb_info = (chain->prev != NULL
			  ? BB_INFO_BY_INDEX (chain->prev->block) : NULL);
	  
	  /* At the start of the basic block, we must save any
	     registers from save_here.  */
	  
	  set_hard_reg_saved
	    (BB_INFO_BY_INDEX (chain->block)->save_here,
	     BB_INFO_BY_INDEX (chain->block)->save_in_mode, save_mode,
	     BB_INFO_BY_INDEX (chain->block)->save_in_pseudo, save_pseudo);
	  COPY_HARD_REG_SET (hard_regs_to_save, hard_regs_saved);
	  
	  where = (last_restore_chain != NULL
		   ? last_restore_chain->next : chain);
	  /* An addr_vec is placed outside any basic block but its
	     chain has the same block as the block of subsequent insn.
	     So skip to the real start of the basic block.  */
	  if (GET_CODE (where->insn) == CODE_LABEL && where->next != NULL
	      && JUMP_P (where->next->insn)
	      && (GET_CODE (PATTERN (where->next->insn)) == ADDR_DIFF_VEC
		  || GET_CODE (PATTERN (where->next->insn)) == ADDR_VEC)
	      && where->next->next != NULL)
	    where = where->next->next;
	  
	  if (!hard_reg_set_empty_p (hard_regs_to_save))
	    for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
	      if (TEST_HARD_REG_BIT (hard_regs_to_save, regno))
		{
		  unsigned int before = regno;
		  
		  regno += insert_save (where, INSN_P (where->insn), regno,
					&hard_regs_to_save, save_mode,
					save_pseudo);
		  gcc_assert (before == regno);
		  save_mode[before] = VOIDmode;
		  save_pseudo[before] = -1;
		}
	  
	  if (prev_bb_info != NULL)
	    {
	      last_restore_chain = NULL;
	      set_hard_reg_saved (prev_bb_info->save_out,
				  prev_bb_info->save_out_mode, save_mode,
				  prev_bb_info->save_out_pseudo, save_pseudo);
	      COPY_HARD_REG_SET (hard_regs_to_save, hard_regs_saved);
	      COPY_HARD_REG_SET (saved, prev_bb_info->free_out);
	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (!TEST_HARD_REG_BIT (saved, regno))
		  free_pseudo[regno] = -1;
		else
		  {
		    if (TEST_HARD_REG_BIT (prev_bb_info->save_out, regno))
		      free_pseudo [regno]
			= prev_bb_info->save_out_pseudo [regno];
		    else
		      {
			gcc_assert (TEST_HARD_REG_BIT (prev_bb_info->restore_out, regno));
			free_pseudo [regno]
			  = prev_bb_info->restore_out_pseudo [regno];
		      }
		  }
	    }
	}
    }
      
  FOR_EACH_LOOP (li, loop, 0)
    {
      free (loop->aux);
    }
  if (flag_ira && optimize)
    free_aux_for_blocks ();
}

/* Here from note_stores, or directly from save_call_clobbered_regs,
   when an insn stores a value in a register.  Set the proper bit or
   bits in this_insn_sets.  */
static void
mark_set_regs (rtx reg, const_rtx setter ATTRIBUTE_UNUSED, void *data)
{
  int regno, endregno, i;
  HARD_REG_SET *this_insn_sets = (HARD_REG_SET *) data;

  if (GET_CODE (reg) == SUBREG)
    {
      rtx inner = SUBREG_REG (reg);
      if (!REG_P (inner))
	return;
      if ((regno = REGNO (inner)) >= FIRST_PSEUDO_REGISTER)
	{
	  if (reg_renumber[regno] < 0)
	    return;
	  regno = reg_renumber[regno];
	  endregno
	    = hard_regno_nregs[regno][PSEUDO_REGNO_MODE (REGNO (inner))];
	}
      else
	{
	  regno = subreg_regno (reg);
	  endregno = regno + subreg_nregs (reg);
	}
    }
  else if (REG_P (reg))
    {
      if ((regno = REGNO (reg)) < FIRST_PSEUDO_REGISTER)
	endregno = END_HARD_REGNO (reg);
      else
	{
	  if (reg_renumber[regno] < 0)
	    return;
	  regno = reg_renumber[regno];
	  endregno
	    = hard_regno_nregs[regno][PSEUDO_REGNO_MODE (REGNO (reg))];
	}
    }
  else
    return;

  for (i = regno; i < endregno; i++)
    SET_HARD_REG_BIT (*this_insn_sets, i);
}

/* Here from note_stores when an insn stores a value in a register.
   Set the proper bit or bits in the passed regset.  All pseudos that have
   been assigned hard regs have had their register number changed already,
   so we can ignore pseudos.  */
static void
add_stored_regs (rtx reg, const_rtx setter, void *data)
{
  int regno, endregno, i;
  enum machine_mode mode = GET_MODE (reg);
  int offset = 0;

  if (GET_CODE (setter) == CLOBBER)
    return;

  if (GET_CODE (reg) == SUBREG
      && REG_P (SUBREG_REG (reg))
      && REGNO (SUBREG_REG (reg)) < FIRST_PSEUDO_REGISTER)
    {
      offset = subreg_regno_offset (REGNO (SUBREG_REG (reg)),
				    GET_MODE (SUBREG_REG (reg)),
				    SUBREG_BYTE (reg),
				    GET_MODE (reg));
      regno = REGNO (SUBREG_REG (reg)) + offset;
      endregno = regno + subreg_nregs (reg);
    }
  else
    {
      if (!REG_P (reg) || REGNO (reg) >= FIRST_PSEUDO_REGISTER)
	return;

      regno = REGNO (reg) + offset;
      endregno = end_hard_regno (mode, regno);
    }

  for (i = regno; i < endregno; i++)
    SET_REGNO_REG_SET ((regset) data, i);
}

/* Walk X and record all referenced registers in REFERENCED_REG and
   modified registers in MODIFIED_REGS.  */
static void
mark_referenced_regs (rtx x, int set_p)
{
  enum rtx_code code;
  const char *fmt;
  int i, j;
  int stop_p;
  
  for (stop_p = FALSE; !stop_p; )
    {
      while (GET_CODE (x) == STRICT_LOW_PART || GET_CODE (x) == ZERO_EXTRACT)
	x = XEXP (x, 0);
      code = GET_CODE (x);
      switch (code)
	{
	case SET:
	  gcc_assert (!set_p);
	  mark_referenced_regs (SET_DEST (x), TRUE);
	  x = SET_SRC (x);
	  break;
	case CLOBBER:
	  mark_referenced_regs (SET_DEST (x), TRUE);
	  return;
	case PRE_INC:
	case POST_INC:
	case PRE_DEC:
	case POST_DEC:
	  gcc_assert (!set_p);
	  x = XEXP (x, 0);
	  mark_referenced_regs (x, TRUE);
	  break;
	case PRE_MODIFY:
	case POST_MODIFY:
	  set_p = FALSE;
	  mark_referenced_regs (XEXP (x, 0), FALSE);
	  mark_referenced_regs (XEXP (x, 0), TRUE);
	  x = XEXP (x, 1);
	  mark_referenced_regs (x, FALSE);
	  break;
	case SUBREG:
	  x = SUBREG_REG (x);
	  break;
	case REG:
	  {
	    int regno = REGNO (x);
	    int hardregno = (regno < FIRST_PSEUDO_REGISTER ? regno
			     : reg_renumber[regno]);
	    
	    if (hardregno >= 0)
	      {
		if (set_p)
		  add_to_hard_reg_set (&modified_regs, GET_MODE (x), hardregno);
		add_to_hard_reg_set (&referenced_regs, GET_MODE (x), hardregno);
	      }
	    /* If this is a pseudo that did not get a hard register, scan
	       its memory location, since it might involve the use of
	       another register, which might be saved.  */
	    else if (reg_equiv_mem[regno] != 0)
	      mark_referenced_regs (XEXP (reg_equiv_mem[regno], 0), set_p);
	    else if (reg_equiv_address[regno] != 0)
	      mark_referenced_regs (reg_equiv_address[regno], set_p);
	    return;
	  }
	default:
	  fmt = GET_RTX_FORMAT (code);
	  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
	    {
	      if (fmt[i] == 'e')
		mark_referenced_regs (XEXP (x, i), FALSE);
	      else if (fmt[i] == 'E')
		for (j = XVECLEN (x, i) - 1; j >= 0; j--)
		  mark_referenced_regs (XVECEXP (x, i, j), FALSE);
	    }
	  stop_p = TRUE;
	  break;
	}
    }
}

/* Insert a sequence of insns to restore.  Place these insns in front
   of CHAIN if BEFORE_P is nonzero, behind the insn otherwise.
   MAXRESTORE is the maximum number of registers which should be
   restored during this call.  It should never be less than 1 since we
   only work with entire registers.  SAVE_PSEUDO maps hard registers
   into the corresponding pseudos.

   Note that we have verified in init_caller_save that we can do this
   with a simple SET, so use it.  Set INSN_CODE to what we save there
   since the address might not be valid so the insn might not be recognized.
   These insns will be reloaded and have register elimination done by
   find_reload, so we need not worry about that here.

   Return the extra number of registers saved.  */

static int
insert_restore (struct insn_chain *chain, int before_p, int regno,
		int maxrestore, enum machine_mode *save_mode,
		int *save_pseudo)
{
  int i, k;
  rtx pat = NULL_RTX;
  int code;
  unsigned int numregs = 0;
  struct insn_chain *new_chain;
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

  mem = regno_save_mem[regno][numregs];
  if (save_mode[regno] != VOIDmode
      && save_mode[regno] != GET_MODE (mem)
      && numregs == (unsigned int) hard_regno_nregs[regno][save_mode[regno]]
      && reg_save_code (regno, save_mode[regno]) >= 0)
    mem = adjust_address_nv (mem, save_mode[regno], 0);
  else
    mem = copy_rtx (mem);

  /* Verify that the alignment of spill space is equal to or greater
     than required.  */
  gcc_assert (MIN (MAX_SUPPORTED_STACK_ALIGNMENT,
		   GET_MODE_ALIGNMENT (GET_MODE (mem))) <= MEM_ALIGN (mem));

  pat = gen_rtx_SET (VOIDmode,
		     gen_rtx_REG (GET_MODE (mem),
				  regno), mem);
  code = reg_restore_code (regno, GET_MODE (mem));
  new_chain = insert_one_insn (chain, before_p, code, pat);
  new_chain->saved_pseudo_regno = save_pseudo[regno];

  if (dump_file != NULL)
    fprintf (dump_file, "inserting restore insn %u for pseudo %d %s %u\n",
	     INSN_UID (new_chain->insn), save_pseudo[regno],
	     before_p ? "before" : "after", INSN_UID (chain->insn));

  /* Clear status for all registers we restored.  */
  for (k = 0; k < i; k++)
    {
      CLEAR_HARD_REG_BIT (hard_regs_saved, regno + k);
      SET_REGNO_REG_SET (&new_chain->dead_or_set, regno + k);
      n_regs_saved--;
    }

  /* Tell our callers how many extra registers we saved/restored.  */
  return numregs - 1;
}

/* Like insert_restore above, but save registers instead.  */

static int
insert_save (struct insn_chain *chain, int before_p, int regno,
	     HARD_REG_SET (*to_save), enum machine_mode *save_mode,
	     int *save_pseudo)
{
  int i;
  unsigned int k;
  rtx pat = NULL_RTX;
  int code;
  unsigned int numregs = 0;
  struct insn_chain *new_chain;
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

  mem = regno_save_mem[regno][numregs];
  if (save_mode[regno] != VOIDmode
      && save_mode[regno] != GET_MODE (mem)
      && numregs == (unsigned int) hard_regno_nregs[regno][save_mode[regno]]
      && reg_save_code (regno, save_mode[regno]) >= 0)
    mem = adjust_address_nv (mem, save_mode[regno], 0);
  else
    mem = copy_rtx (mem);

  /* Verify that the alignment of spill space is equal to or greater
     than required.  */
  gcc_assert (MIN (MAX_SUPPORTED_STACK_ALIGNMENT,
		   GET_MODE_ALIGNMENT (GET_MODE (mem))) <= MEM_ALIGN (mem));

  pat = gen_rtx_SET (VOIDmode, mem,
		     gen_rtx_REG (GET_MODE (mem),
				  regno));
  code = reg_save_code (regno, GET_MODE (mem));
  new_chain = insert_one_insn (chain, before_p, code, pat);
  new_chain->saved_pseudo_regno = save_pseudo[regno];

  if (dump_file != NULL)
    fprintf (dump_file, "inserting save insn %u for pseudo %d %s %u\n",
	     INSN_UID (new_chain->insn), save_pseudo[regno],
	     before_p ? "before" : "after", INSN_UID (chain->insn));

  /* Set hard_regs_saved and dead_or_set for all the registers we saved.  */
  for (k = 0; k < numregs; k++)
    {
      SET_HARD_REG_BIT (hard_regs_saved, regno + k);
      SET_REGNO_REG_SET (&new_chain->dead_or_set, regno + k);
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
  struct insn_chain *new_chain;

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

  new_chain = new_insn_chain ();
  if (before_p)
    {
      rtx link;

      new_chain->prev = chain->prev;
      if (new_chain->prev != 0)
	new_chain->prev->next = new_chain;
      else
	reload_insn_chain = new_chain;

      chain->prev = new_chain;
      new_chain->next = chain;
      new_chain->insn = emit_insn_before (pat, insn);
      /* ??? It would be nice if we could exclude the already / still saved
	 registers from the live sets.  */
      COPY_REG_SET (&new_chain->live_throughout, &chain->live_throughout);
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
		SET_REGNO_REG_SET (&new_chain->live_throughout, regno + i);
	    }
	}

      /* If CHAIN->INSN is a call, then the registers which contain
	 the arguments to the function are live in the new insn.  */
      if (CALL_P (chain->insn))
	{
	  for (link = CALL_INSN_FUNCTION_USAGE (chain->insn);
	       link != NULL_RTX;
	       link = XEXP (link, 1))
	    {
	      rtx arg = XEXP (link, 0);

	      if (GET_CODE (arg) == USE)
		{
		  rtx reg = XEXP (arg, 0);

		  if (REG_P (reg))
		    {
		      int i, regno = REGNO (reg);

		      /* Registers in CALL_INSN_FUNCTION_USAGE are always
			 hard registers.  */
		      gcc_assert (regno < FIRST_PSEUDO_REGISTER);

		      for (i = hard_regno_nregs[regno][GET_MODE (reg)] - 1;
			   i >= 0; i--)
			SET_REGNO_REG_SET (&new_chain->live_throughout, regno + i);
		    }
		}
	    }
	  
	}

      CLEAR_REG_SET (&new_chain->dead_or_set);
      if (chain->insn == BB_HEAD (BASIC_BLOCK (chain->block)))
	BB_HEAD (BASIC_BLOCK (chain->block)) = new_chain->insn;
    }
  else
    {
      new_chain->next = chain->next;
      if (new_chain->next != 0)
	new_chain->next->prev = new_chain;
      chain->next = new_chain;
      new_chain->prev = chain;
      if (GET_CODE (insn) != CODE_LABEL)
	new_chain->insn = emit_insn_after (pat, insn);
      else
	{
	  /* Put the insn after bb note in a empty basic block.  */
	  gcc_assert (NEXT_INSN (insn) && NOTE_P (NEXT_INSN (insn)));
	  new_chain->insn = emit_insn_after (pat, NEXT_INSN (insn));
	}
      /* ??? It would be nice if we could exclude the already / still saved
	 registers from the live sets, and observe REG_UNUSED notes.  */
      COPY_REG_SET (&new_chain->live_throughout, &chain->live_throughout);
      /* Registers that are set in CHAIN->INSN live in the new insn.
	 (Unless there is a REG_UNUSED note for them, but we don't
	  look for them here.) */
      if (INSN_P (chain->insn))
	note_stores (PATTERN (chain->insn), add_stored_regs,
		     &new_chain->live_throughout);
      CLEAR_REG_SET (&new_chain->dead_or_set);
      if (chain->insn == BB_END (BASIC_BLOCK (chain->block)))
	BB_END (BASIC_BLOCK (chain->block)) = new_chain->insn;
    }
  new_chain->block = chain->block;
  new_chain->is_caller_save_insn = 1;

  INSN_CODE (new_chain->insn) = code;
  return new_chain;
}
#include "gt-caller-save.h"
