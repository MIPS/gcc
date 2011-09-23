/* Change pseudos by memory.
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


/* This code contains functions to change spilled pseudos into memory
   and to eliminate virtual registers.  */

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
#include "target.h"
#include "lra-int.h"
#include "df.h"


/* Max regno at the start of the pass.  */
static int regs_num;

/* The structure describes stack slot of a spilled pseudo.  */
struct pseudo_slot
{
  /* Number of stack slot to which given pseudo belongs.  */
  int slot_num;
  /* First or next slot with the same slot number.  */
  struct pseudo_slot *next, *first;
  /* Memory representing the spilled pseudo.  */
  rtx mem;
};

/* The stack slots for each spilled pseudo.  Indexed by regnos.  */
static struct pseudo_slot *pseudo_slots;

/* The structure describes a stack slot which can be used for several
   spilled pseudos.  */
struct slot
{
  /* First pseudo with given stack slot.  */
  int regno;
  /* Memory representing the all stack slot.  It can be different from
     memory representing a pseudo belonging to give stack slot because
     pseudo can be placed in a part of the corresponding stack
     slot.  */
  rtx mem;
  /* Combined live ranges of all pseudos belonging to give slot.  It
     is used to define can a new spilled pseudo use given stack
     slot.  */
  lra_live_range_t live_ranges;
};

/* Array containing info about the stack slots.  The array element is
   indexed by the stack slot number in the range [0..slost_num).  */
static struct slot *slots;
/* The number of the slots currently existing.  */
static int slots_num;

/* Set up memory of the spilled pseudo I.  The function can allocate
   the corresponding stack slot if it is not done yet.  */
static void
assign_slot (int i)
{
  rtx x = NULL_RTX;
  enum machine_mode mode = GET_MODE (regno_reg_rtx[i]);
  unsigned int inherent_size = PSEUDO_REGNO_BYTES (i);
  unsigned int inherent_align = GET_MODE_ALIGNMENT (mode);
  unsigned int max_ref_width = GET_MODE_SIZE (lra_reg_info[i].biggest_mode);
  unsigned int total_size = MAX (inherent_size, max_ref_width);
  unsigned int min_align = max_ref_width * BITS_PER_UNIT;
  int adjust = 0;

  gcc_assert (regno_reg_rtx[i] != NULL_RTX && REG_P (regno_reg_rtx[i])
	      && lra_reg_info[i].nrefs != 0 && reg_renumber[i] < 0);
  
  x = slots[pseudo_slots[i].slot_num].mem;
  
  if (x)
    ;
  /* Each pseudo reg has an inherent size which comes from its own
     mode, and a total size which provides room for paradoxical
     subregs which refer to the pseudo reg in wider modes.
     
     We can use a slot already allocated if it provides both enough
     inherent space and enough total space.  Otherwise, we allocate a
     new slot, making sure that it has no less inherent space, and no
     less total space, then the previous slot.  */
  else
    {
      rtx stack_slot;

      /* No known place to spill from => no slot to reuse.  */
      x = assign_stack_local (mode, total_size,
			      min_align > inherent_align
			      || total_size > inherent_size ? -1 : 0);
      x = lra_eliminate_regs_1 (x, GET_MODE (x), false, false, true);
      stack_slot = x;
      /* Cancel the big-endian correction done in assign_stack_local.
	 Get the address of the beginning of the slot.  This is so we
	 can do a big-endian correction unconditionally below.  */
      if (BYTES_BIG_ENDIAN)
	{
	  adjust = inherent_size - total_size;
	  if (adjust)
	    stack_slot
	      = adjust_address_nv (x,
				   mode_for_size (total_size * BITS_PER_UNIT,
						  MODE_INT, 1),
				   adjust);
	}
      slots[pseudo_slots[i].slot_num].mem = stack_slot;
    }
      
  /* On a big endian machine, the "address" of the slot is the address
     of the low part that fits its inherent mode.  */
  if (BYTES_BIG_ENDIAN && inherent_size < total_size)
    adjust += (total_size - inherent_size);
  
  /* If we have any adjustment to make, or if the stack slot is the
     wrong mode, make a new stack slot.  */
  x = adjust_address_nv (x, GET_MODE (regno_reg_rtx[i]), adjust);
  
  /* Set all of the memory attributes as appropriate for a spill.  */
  set_mem_attrs_for_spill (x);
  pseudo_slots[i].mem = x;
}

/* Sort pseudos according their frequencies.  */
static int
regno_freq_compare (const void *v1p, const void *v2p)
{
  const int regno1 = *(const int *) v1p;
  const int regno2 = *(const int *) v2p;
  int diff;

  if ((diff = lra_reg_info[regno2].freq - lra_reg_info[regno1].freq) != 0)
    return diff;
  return regno1 - regno2;
}

/* Redefine STACK_GROWS_DOWNWARD in terms of 0 or 1.  */
#ifdef STACK_GROWS_DOWNWARD
# undef STACK_GROWS_DOWNWARD
# define STACK_GROWS_DOWNWARD 1
#else
# define STACK_GROWS_DOWNWARD 0
#endif

/* Sort pseudos according their slot numbers putting ones with smaller
   numbers first, or last when the frame pointer is not needed.  So
   the first slot will be finally addressed with smaller address
   displacement.  */
static int
pseudo_reg_slot_compare (const void *v1p, const void *v2p)
{
  const int regno1 = *(const int *) v1p;
  const int regno2 = *(const int *) v2p;
  int diff, slot_num1, slot_num2;
  int total_size1, total_size2;

  slot_num1 = pseudo_slots[regno1].slot_num;
  slot_num2 = pseudo_slots[regno2].slot_num;
  if ((diff = slot_num1 - slot_num2) != 0)
    return (frame_pointer_needed
	    || !FRAME_GROWS_DOWNWARD == STACK_GROWS_DOWNWARD ? diff : -diff);
  total_size1 = MAX (PSEUDO_REGNO_BYTES (regno1),
		     GET_MODE_SIZE (lra_reg_info[regno1].biggest_mode));
  total_size2 = MAX (PSEUDO_REGNO_BYTES (regno2),
		     GET_MODE_SIZE (lra_reg_info[regno2].biggest_mode));
  if ((diff = total_size2 - total_size1) != 0)
    return diff;
  return regno1 - regno2;
}

/* Assign stack slot number to them pseudo-register numbers in array
   PSEUDO_REGNOS of length N.  Sort them for subsequent assigning
   stack slots.  */
static void
assign_stack_slot_num_and_sort_pseudos (int *pseudo_regnos, int n)
{
  int i, j, regno;
  struct pseudo_slot *first;
  bitmap set_jump_crosses = regstat_get_setjmp_crosses ();

  /* Sort regnos according their frequencies.  */
  qsort (pseudo_regnos, n, sizeof (int), regno_freq_compare);
  /* Assign stack slot numbers to spilled pseudos, use smaller numbers
     for most frequently used pseudos.  */
  slots_num = 0;
  for (i = 0; i < n; i++)
    {
      regno = pseudo_regnos[i];
      j = -1;
      if (! flag_ira_share_spill_slots)
	j = slots_num;
      else
	{
	  if (bitmap_bit_p (set_jump_crosses, regno))
	    j = slots_num; /* assign a new slot */
	}
      if (j < 0)
	for (j = 0; j < slots_num; j++)
	  if (! lra_intersected_live_ranges_p (slots[j].live_ranges,
					       lra_reg_info[regno].live_ranges))
	    break;
      if (j >= slots_num)
	{
	  /* New slot.  */
	  slots[j].live_ranges = NULL;
	  slots[j].regno = -1;
	  slots[j].mem = NULL_RTX;
	  slots_num++;
	}
      pseudo_slots[regno].mem = NULL_RTX;
      pseudo_slots[regno].slot_num = j;
      if (slots[j].regno < 0)
	{
	  slots[j].regno = regno;
	  pseudo_slots[regno].first = &pseudo_slots[regno];
	  pseudo_slots[regno].next = NULL;
	}
      else
	{
	  first = pseudo_slots[regno].first = &pseudo_slots[slots[j].regno];
	  pseudo_slots[regno].next = pseudo_slots[slots[j].regno].next;
	  first->next = &pseudo_slots[regno];
	}
      slots[j].live_ranges = (lra_merge_live_ranges
			      (slots[j].live_ranges,
			       lra_copy_live_range_list
			       (lra_reg_info[regno].live_ranges)));
    }
  /* Sort regnos according to their slot numbers.  */
  qsort (pseudo_regnos, n, sizeof (int), pseudo_reg_slot_compare);
}

/* Recursively process LOC in INSN and change spilled pseudos to the
   corresponding memory.  Ignore spilled pseudos created from the
   scratches.  */
static bool
remove_pseudos (rtx *loc, rtx insn)
{
  int i;
  bool res;
  const char *fmt;
  enum rtx_code code;

  if (*loc == NULL_RTX)
    return false;
  code = GET_CODE (*loc);
  if (code == REG && (i = REGNO (*loc)) >= FIRST_PSEUDO_REGISTER
      && lra_get_regno_hard_regno (i) < 0
      /* We do not want to assign memory for former scratches because
	 it might result in an address reload for some targets.  In
	 any case we transform such pseudos not getting hard registers
	 into scratches back.  */
      && ! lra_former_scratch_p (i))
    {
      *loc = copy_rtx (pseudo_slots[i].mem);
      return true;
    }

  res = false;
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	res = remove_pseudos (&XEXP (*loc, i), insn) || res;
      else if (fmt[i] == 'E')
	{
	  int j;

	  for (j = XVECLEN (*loc, i) - 1; j >= 0; j--)
	    res = remove_pseudos (&XVECEXP (*loc, i, j), insn) || res;
	}
    }
  return res;
}

/* Convert pseudos got memory into their stack slots, return true if
   any such conversion was done, put insns to process on the stack
   (that is all insns in which pseudos were changed to memory)  */
static bool
spill_pseudos (void)
{
  basic_block bb;
  rtx insn;
  int i;
  bitmap_head spilled_pseudos, changed_insns;

  bitmap_initialize (&spilled_pseudos, &reg_obstack);
  bitmap_initialize (&changed_insns, &reg_obstack);
  for (i = FIRST_PSEUDO_REGISTER; i < regs_num; i++)
    {
      if (lra_reg_info[i].nrefs != 0 && lra_get_regno_hard_regno (i) < 0
	  && ! lra_former_scratch_p (i))
	{
	  bitmap_set_bit (&spilled_pseudos, i);
	  bitmap_ior_into (&changed_insns, &lra_reg_info[i].insn_bitmap);
	}
    }
  FOR_EACH_BB (bb)
    {
      FOR_BB_INSNS (bb, insn)
	if (bitmap_bit_p (&changed_insns, INSN_UID (insn)))
	  {
	    remove_pseudos (&PATTERN (insn), insn);
	    if (lra_dump_file != NULL)
	      fprintf (lra_dump_file,
		       "Changing spilled pseudos to memory in insn #%u\n",
		       INSN_UID (insn));
	    lra_push_insn (insn);
	  }
      bitmap_and_compl_into (DF_LR_IN (bb), &spilled_pseudos);
      bitmap_and_compl_into (DF_LR_OUT (bb), &spilled_pseudos);
    }
  bitmap_clear (&spilled_pseudos);
  if (bitmap_empty_p (&changed_insns))
    return false;
  bitmap_clear (&changed_insns);
  return true;
}

/* Change spilled pseudos into memory.  Returns true if we change some
   pseudos into memory.  The function put changed insns on the stack.
   That is all insns in which pseudos were changed to memory.  */
bool
lra_spill (void)
{
  int i, n, curr_regno;
  int *pseudo_regnos;
  bool changed_p;

  regs_num = max_reg_num ();
  pseudo_regnos = (int *) xmalloc (sizeof (int) * regs_num);
  for (n = 0, i = FIRST_PSEUDO_REGISTER; i < regs_num; i++)
    if (lra_reg_info[i].nrefs != 0 && lra_get_regno_hard_regno (i) < 0)
      pseudo_regnos[n++] = i;
  pseudo_slots = (struct pseudo_slot *) xmalloc (sizeof (struct pseudo_slot)
						 * regs_num);
  slots = (struct slot *) xmalloc (sizeof (struct slot) * regs_num);
  assign_stack_slot_num_and_sort_pseudos (pseudo_regnos, n);
  for (i = 0; i < n; i++)
    /* We do not want to assign memory for former scratches.  */
    if (! lra_former_scratch_p (pseudo_regnos[i]))
      assign_slot (pseudo_regnos[i]);
  if (lra_dump_file != NULL)
    {
      for (i = 0; i < slots_num; i++)
	{
	  if (slots[i].mem == NULL_RTX)
	    /* It can happen for scratch registers.  */
	    fprintf (lra_dump_file, "  Slot %d regnos (no memory):", i);
	  else
	    fprintf (lra_dump_file, "  Slot %d regnos (width = %d):", i,
		     GET_MODE_SIZE (GET_MODE (slots[i].mem)));
	  for (curr_regno = slots[i].regno;;
	       curr_regno = pseudo_slots[curr_regno].next - pseudo_slots)
	    {
	      fprintf (lra_dump_file, "  %d", curr_regno);
	      if (pseudo_slots[curr_regno].next == NULL)
		break;
	    }
	  fprintf (lra_dump_file, "\n");
	}
    }
  changed_p = spill_pseudos ();
  free (slots);
  free (pseudo_slots);
  free (pseudo_regnos);
  /* Checking that elimination does not changed since last one.  */
  return changed_p;
}

/* Final change of pseudos got hard registers into the corresponding
   hard registers. */
void
lra_hard_reg_substitution (void)
{
  int i, hard_regno;
  basic_block bb;
  rtx insn;

  for (i = FIRST_PSEUDO_REGISTER; i < regs_num; i++)
    if (lra_reg_info[i].nrefs != 0
	&& (hard_regno = lra_get_regno_hard_regno (i)) >= 0)
      SET_REGNO (regno_reg_rtx[i], hard_regno);
  FOR_EACH_BB (bb)
    FOR_BB_INSNS (bb, insn)
      if (INSN_P (insn))
	{
	  lra_insn_recog_data_t id;
	  
	  id = lra_get_insn_recog_data (insn);
	  for (i = id->insn_static_data->n_operands - 1; i >= 0; i--)
	    {
	      rtx op = *id->operand_loc[i];

	      if (GET_CODE (op) == SUBREG && REG_P (SUBREG_REG (op)))
		{
		  gcc_assert (REGNO (SUBREG_REG (op)) < FIRST_PSEUDO_REGISTER);
		  alter_subreg (id->operand_loc[i]);
		  lra_update_dup (id, i);
		}
	    }
	}
}
