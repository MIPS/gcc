/* Transformations and transactions of Yet Another Register Allocator.
   Contributed by Vladimir Makarov.
   Copyright (C) 2005, 2006 Free Software Foundation, Inc.

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
#include "tm_p.h"
#include "target.h"
#include "varray.h"
#include "regs.h"
#include "flags.h"
#include "sbitmap.h"
#include "bitmap.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "insn-codes.h"
#include "insn-config.h"
#include "expr.h"
#include "optabs.h"
#include "recog.h"
#include "cfgloop.h"
#include "hashtab.h"
#include "errors.h"
#include "ggc.h"
#include "params.h"
#include "toplev.h"
#include "yara-int.h"

/* Round a value to the lowest integer less than it that is a multiple of
   the required alignment.  Avoid using division in case the value is
   negative.  Assume the alignment is a power of two.  */
#define FLOOR_ROUND(VALUE,ALIGN) ((VALUE) & ~((ALIGN) - 1))

/* Similar, but round to the next highest integer that meets the
   alignment.  */
#define CEIL_ROUND(VALUE,ALIGN)	(((VALUE) + (ALIGN) - 1) & ~((ALIGN)- 1))

/* Cost of the current allocation.  */
int global_allocation_cost;

/* Current size and alignment of slot memory.  */
int slot_memory_size;
int slot_memory_alignment;

HARD_REG_SET base_regs [MAX_MACHINE_MODE];
HARD_REG_SET index_regs;



static void set_ever_live_regs (void);
static void mark_regno_allocation (int, enum machine_mode);
static void mark_regno_release (int, enum machine_mode);

static void initiate_stack_memory (void);
static void free_all_stack_memory (void);
static void reserve_stack_memory (int, int);
static int find_free_stack_memory (int, int);
static void finish_stack_memory (void);

static void initiate_copy_changes (void);
static void free_copy_change (struct copy_change *);
static struct copy_change *get_free_copy_change (void);
static void finish_copy_changes (void);

static struct memory_slot *get_free_memory_slot_structure (void);
static void free_memory_slot_structure (struct memory_slot *);
static void switch_on_pending_memory_slot_structures (void);
static void free_pending_memory_slot_structures (void);
static void add_memory_slot_end (int);
static void remove_memory_slot_end (int);
static void increase_align_count (int);
static void decrease_align_count (int);
static void initiate_memory_slots (void);
static void register_slot_start_change (int, struct memory_slot *);
static void register_memory_slot_usage (struct memory_slot *, int);
static void unregister_memory_slot_usage (struct memory_slot *, int);
static void allocate_allocno_memory_slot (allocno_t);
static void deallocate_allocno_memory_slot (allocno_t);
#ifdef SECONDARY_MEMORY_NEEDED
static void allocate_copy_memory_slot (copy_t);
static void deallocate_copy_memory_slot (copy_t);
#endif
static int can_compare (const void *, const void *);
static void finish_memory_slots (void);

static void setup_temp_mems_and_addresses (void);
static rtx get_temp_const_int (HOST_WIDE_INT);
static rtx get_temp_disp (rtx, HOST_WIDE_INT);
static rtx get_temp_stack_memory_slot_rtx (enum machine_mode, HOST_WIDE_INT,
					   int);

static enum reg_class get_allocno_reg_class (allocno_t);
static int non_pseudo_allocno_copy_cost (allocno_t);

static bool check_hard_reg (int, allocno_t, HARD_REG_SET, bool);
static int find_hard_reg (allocno_t, enum reg_class, HARD_REG_SET, int *, int);
static int find_hard_reg_for_mode (enum reg_class, enum machine_mode,
				   HARD_REG_SET);
#ifdef SECONDARY_MEMORY_NEEDED
static bool allocate_copy_secondary_memory (bool, copy_t, int, enum reg_class,
					    enum reg_class, enum machine_mode);
#endif
static bool allocno_change_mode_ok_p (allocno_t, allocno_t);
static bool assign_copy_interm_equiv_const_hard_reg (copy_t);
static void unassign_copy_interm_equiv_const_hard_reg (copy_t);
static bool assign_copy (copy_t);
static bool assign_allocno_copies (allocno_t);
static void unassign_allocno_copies (allocno_t a);
static bool check_hard_regno_for_a (allocno_t, int, HARD_REG_SET);
static bool collect_conflict_hard_regs (allocno_t, HARD_REG_SET *);
static bool assign_allocno_hard_regno (allocno_t, int, HARD_REG_SET);
static bool assign_one_allocno (allocno_t, enum reg_class, HARD_REG_SET);
static bool assign_allocno_pair (allocno_t, allocno_t, enum reg_class,
				 HARD_REG_SET, int);
static void unassign_one_allocno (allocno_t);

static void possible_alt_reg_intersection (allocno_t, HARD_REG_SET *);
static bool all_alt_offset_ok_p (allocno_t, HOST_WIDE_INT);
static bool find_interm_elimination_reg (allocno_t, enum reg_class,
					 HARD_REG_SET);
static struct log_entry *get_free_log_entry (void);
static void free_log_entry (struct log_entry *);
static void free_all_log_entries (void);
static struct transaction *get_free_transaction (void);
static void free_transaction (struct transaction *);
static void free_all_transactions (void);
static void initiate_transactions (void);
static void log_allocno (allocno_t);
static void undo_allocno_change (struct allocno_log_entry *);
static void log_copy (copy_t);
static void undo_copy_change (struct copy_log_entry *);
static void log_memory_slot (struct memory_slot *);
static void undo_memory_slot_change (struct memory_slot_log_entry *);
static void undo_change (struct log_entry *, bool);
static void stop_transaction (bool);
static void finish_transactions (void);



static int hard_reg_alocation_counts [FIRST_PSEUDO_REGISTER];

static void
set_ever_live_regs (void)
{
  int i;

  if (! stack_frame_pointer_can_be_eliminated_p
      || ! obligatory_stack_frame_pointer_elimination_p)
    regs_ever_live [HARD_FRAME_POINTER_REGNUM] = 1;

  /* A function that receives a nonlocal goto must save all call-saved
     registers.  */
  if (current_function_has_nonlocal_label)
    for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
      if (! call_used_regs[i] && ! fixed_regs[i] && ! LOCAL_REGNO (i))
	regs_ever_live[i] = 1;
  
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    hard_reg_alocation_counts [i] = (regs_ever_live [i] ? 1 : 0);
  update_elim_offsets ();
}

static void
mark_regno_allocation (int hard_regno, enum machine_mode mode)
{
  int i, n;

  for (i = hard_regno_nregs [hard_regno] [mode] - 1; i >= 0; i--)
    {
      n = hard_regno + i;
      if (hard_reg_alocation_counts [n] == 0)
	{
	  regs_ever_live [n] = 1;
	  if (! call_used_regs [n])
	    update_elim_offsets ();
	}
      hard_reg_alocation_counts [n]++;
    }
}

static void
mark_regno_release (int hard_regno, enum machine_mode mode)
{
  int i, n;

  for (i = hard_regno_nregs [hard_regno] [mode] - 1; i >= 0; i--)
    {
      n = hard_regno + i;
      yara_assert (hard_reg_alocation_counts [n] > 0);
      --hard_reg_alocation_counts [n];
      if (hard_reg_alocation_counts [n] == 0)
	{
	  regs_ever_live [n] = 0;
	  if (! call_used_regs [n])
	    update_elim_offsets ();
	}
    }
}



/* This page contains functions to simulate stack memory and to
   reserve stack slots for spilled registers.  */

/* The stack is represented by the following bit map.  If the stack
   grows downward bigger index in the bitmap corresponds to smaller
   address in real memory space.  Usually, the length of the bitmap is
   small and have a lot of ones. */
static sbitmap memory_stack_sbitmap;
static int memory_stack_sbitmap_size;

#define MIN_STACK_SBITMAP_SIZE 128

static void
initiate_stack_memory (void)
{
  memory_stack_sbitmap_size = MIN_STACK_SBITMAP_SIZE;
  memory_stack_sbitmap = sbitmap_alloc (memory_stack_sbitmap_size);
}

static void
free_all_stack_memory (void)
{
  sbitmap_ones (memory_stack_sbitmap);
}

static void
reserve_stack_memory (int start, int size)
{
  int i;
  int begin, bound;

  yara_assert (start >= 0 && size > 0);
#ifdef FRAME_GROWS_DOWNWARD
  begin = start - size + 1;
  yara_assert (begin >= 0);
  bound = start + 1;
#else
  begin = start;
  bound = start + size;
#endif
  if (bound >= memory_stack_sbitmap_size)
    {
      memory_stack_sbitmap_size = bound + bound / 2;
      memory_stack_sbitmap = sbitmap_resize (memory_stack_sbitmap,
					     memory_stack_sbitmap_size, 1);
    }
  for (i = begin; i < bound; i++)
    RESET_BIT (memory_stack_sbitmap, i);
}

static int
find_free_stack_memory (int size, int align)
{
  sbitmap_iterator sbi;
  unsigned int k = 0;
  int j, start;
  bool cont_p;

  yara_assert (size > 0 && align > 0);
  start = 0;
#ifdef FRAME_GROWS_DOWNWARD
  EXECUTE_IF_SET_IN_SBITMAP (memory_stack_sbitmap, 0, k, sbi)
    {
      start = CEIL_ROUND (k, (unsigned) (align));
      for (j = 0;
	   (cont_p = j < size && j + start < memory_stack_sbitmap_size);
	   j++)
	if (! TEST_BIT (memory_stack_sbitmap, j + start))
	  break;
      if (! cont_p)
	return start + size - 1;
    }
  start += size - 1;
#else
  EXECUTE_IF_SET_IN_SBITMAP (memory_stack_sbitmap, 0, k, sbi)
    {
      start = CEIL_ROUND (k, (unsigned) align);
      for (j = 0;
	   cont_p = j < size && j + start < memory_stack_sbitmap_size;
	   j++)
	if (! TEST_BIT (memory_stack_sbitmap, j + start))
	  break;
      if (! cont_p)
	return start;
    }
#endif
  return start;
}

static void
finish_stack_memory (void)
{
  sbitmap_free (memory_stack_sbitmap);
}



static struct copy_change *free_copy_changes;
static varray_type copy_change_varray;

static void
initiate_copy_changes (void)
{
  free_copy_changes = NULL;
  VARRAY_GENERIC_PTR_NOGC_INIT (copy_change_varray, 2000, "all copy changes");
}

static void
free_copy_change (struct copy_change *change)
{
  *(struct copy_change **) change = free_copy_changes;
  free_copy_changes = change;
}

static struct copy_change *
get_free_copy_change (void)
{
  struct copy_change *result;

  if (free_copy_changes == NULL)
    {
      result = yara_allocate (sizeof (struct copy_change));
      VARRAY_PUSH_GENERIC_PTR (copy_change_varray, result);
    }
  else
    {
      result = free_copy_changes;
      free_copy_changes
	= *(struct copy_change **) free_copy_changes;
    }
#ifdef HAVE_SECONDARY_RELOADS
  result->icode = CODE_FOR_nothing;
  result->interm_mode = result->scratch_mode = VOIDmode;
  result->interm_regno = result->scratch_regno = -1;
#endif
  result->interm_equiv_const_regno = -1;
  result->interm_equiv_const_mode = VOIDmode;
#ifdef SECONDARY_MEMORY_NEEDED
  result->memory_mode = VOIDmode;
  result->user_defined_memory = NULL_RTX;
  result->memory_slot = NULL;
#endif
  CLEAR_HARD_REG_SET (result->interm_scratch_hard_regset);
  return result;
}

static void
finish_copy_changes (void)
{
  int i;

  for (i = VARRAY_ACTIVE_SIZE (copy_change_varray) - 1;
       i >= 0;
       i--)
    yara_free (VARRAY_GENERIC_PTR (copy_change_varray, i));
  VARRAY_FREE (copy_change_varray);
}



/* This page contains functions to allocate/deallocate memory slots for
   allocnos.  They works on regno level to guarantee that the same
   allocnos with the same register number will get the same memory slot
   and as consequence copies of memory into memory will be eliminated.
   Otherwise, allocation for allocnos without taking corresponding
   registers into account would be too complicated and the result is
   not obviously better (additional memory-memory copies might
   overweight memory slot economy).  */


/* Slot num -> the corresponding memory slot or NULL if there is no
   currently memory slot for the can.  */
static struct memory_slot **can_memory_slots;

/* The free structures that could be reused.  */
static struct memory_slot *free_memory_slot_structures;

/* If the following value is true the memory slot being freed put into
   the following array instead of freeing them for subsequent
   reuse.  */
static bool pending_free_memory_slot_pending_p;
static varray_type pending_free_memory_slot_varray;

/* Each element value is number of allocnos whose end byte (in
   simulated stack memory) is the element index.  */
static varray_type end_slot_numbers_varray;

/* Array elements contain number of slots with aligment given as the
   index.  */
static int *align_counts;

#ifdef SECONDARY_MEMORY_NEEDED
/* Copies which needs secondary memory.  */
static bitmap secondary_memory_copies;
#endif

static struct memory_slot *
get_free_memory_slot_structure (void)
{
  struct memory_slot *slot;

  if (free_memory_slot_structures == NULL)
    slot = yara_allocate (sizeof (struct memory_slot));
  else
    {
      slot = free_memory_slot_structures;
      free_memory_slot_structures
	= free_memory_slot_structures->next_free_slot;
    }
  slot->allocnos_num = 0;
  slot->start = -1;
  slot->mem = NULL;
  slot->mem_index = -1;
  slot->size = 0;
  return slot;
}

static void
free_memory_slot_structure (struct memory_slot *slot)
{
  yara_assert (slot->allocnos_num == 0);
  if (pending_free_memory_slot_pending_p)
    VARRAY_PUSH_GENERIC_PTR (pending_free_memory_slot_varray, slot);
  else
    {
      yara_assert (slot->start >= 0);
      /* Mark it not to free it more one time.  */
      slot->start = -1;
      slot->next_free_slot = free_memory_slot_structures;
      free_memory_slot_structures = slot;
    }
}

static void
switch_on_pending_memory_slot_structures (void)
{
  yara_assert (! pending_free_memory_slot_pending_p);
  pending_free_memory_slot_pending_p = true;
}

static void
free_pending_memory_slot_structures (void)
{
  int i;
  struct memory_slot *slot;

  yara_assert (pending_free_memory_slot_pending_p);
  pending_free_memory_slot_pending_p = false;
  for (i = VARRAY_ACTIVE_SIZE (pending_free_memory_slot_varray) - 1;
       i >= 0;
       i--)
    {
      slot = VARRAY_GENERIC_PTR (pending_free_memory_slot_varray, i);
      if (slot->allocnos_num == 0 && slot->start >= 0)
	free_memory_slot_structure (slot);
    }
  VARRAY_POP_ALL (pending_free_memory_slot_varray);
}

static void
add_memory_slot_end (int end)
{
  if (VARRAY_ACTIVE_SIZE (end_slot_numbers_varray) <= (unsigned) end)
    {
      while (VARRAY_ACTIVE_SIZE (end_slot_numbers_varray) <= (unsigned) end)
	VARRAY_PUSH_INT (end_slot_numbers_varray, 0);
      slot_memory_size = end + 1;
    }
  VARRAY_INT (end_slot_numbers_varray, end)
    = VARRAY_INT (end_slot_numbers_varray, end) + 1;
  yara_assert (VARRAY_ACTIVE_SIZE (end_slot_numbers_varray)
	       == (unsigned) slot_memory_size);
}

static void
remove_memory_slot_end (int end)
{
  VARRAY_INT (end_slot_numbers_varray, end)--;
  yara_assert ((VARRAY_ACTIVE_SIZE (end_slot_numbers_varray)
		== (unsigned) slot_memory_size)
	       && slot_memory_size > end
	       && VARRAY_INT (end_slot_numbers_varray, end) >= 0);
  if (end + 1 == slot_memory_size)
    {
      while (end >= 0 && VARRAY_INT (end_slot_numbers_varray, end) == 0)
	{
	  end--;
	  VARRAY_POP (end_slot_numbers_varray);
	}
      slot_memory_size = VARRAY_ACTIVE_SIZE (end_slot_numbers_varray);
    }
}

static void
increase_align_count (int align)
{
  if (align > slot_memory_alignment)
    slot_memory_alignment = align;
  align_counts [align]++;
}

static void
decrease_align_count (int align)
{
  int i;

  align_counts [align]--;
  yara_assert (align_counts [align] >= 0);
  if (align_counts [align] == 0 && align == slot_memory_alignment)
    {
      for (i = align - 1; i > 0 && align_counts [i] == 0; i--)
	;
      slot_memory_alignment = i;
    }
}

/* Return size of the aligned simulated stack area.  */
HOST_WIDE_INT
rounded_slot_memory_size (void)
{
  int align;

  align = slot_memory_alignment;
  /* Ignore alignment we can't do with expected alignment of the
     boundary.  */
  if ((unsigned) align * BITS_PER_UNIT > PREFERRED_STACK_BOUNDARY)
    align = PREFERRED_STACK_BOUNDARY / BITS_PER_UNIT;
  
  return CEIL_ROUND (slot_memory_size, (unsigned) align);
}

/* Return the displacement of the simulated stack area start relative
   to frame pointer.  */
HOST_WIDE_INT
get_stack_memory_start_frame_offset (void)
{
  int align, size;
  int frame_off, frame_alignment, frame_phase;
  HOST_WIDE_INT offset;

  align = slot_memory_alignment;
  /* Ignore alignment we can't do with expected alignment of the
     boundary.  */
  if ((unsigned) align * BITS_PER_UNIT > PREFERRED_STACK_BOUNDARY)
    align = PREFERRED_STACK_BOUNDARY / BITS_PER_UNIT;
  
#ifdef FRAME_GROWS_DOWNWARD
  /* We assume that the simulated stack is properly aligned.  It means
     that the first byte after the cell is aligned too.  */
  size = CEIL_ROUND (slot_memory_size, (unsigned) align);
  offset = cfun->x_frame_offset - size;
#else
  size = slot_memory_size;
  offset = cfun->x_frame_offset;
#endif

  /* Calculate how many bytes the start of local variables is off from
     stack alignment.  */
  frame_alignment = PREFERRED_STACK_BOUNDARY / BITS_PER_UNIT;
  frame_off = STARTING_FRAME_OFFSET % frame_alignment;
  frame_phase = frame_off ? frame_alignment - frame_off : 0;

#ifdef FRAME_GROWS_DOWNWARD
  offset = (FLOOR_ROUND (offset - frame_phase,
			 (unsigned HOST_WIDE_INT) align) + frame_phase);
#else
  offset = (CEIL_ROUND (offset - frame_phase,
			(unsigned HOST_WIDE_INT) align) + frame_phase);
#endif

#ifdef FRAME_GROWS_DOWNWARD
  offset += size - 1;
#endif

  /* ??? trunc_int_for_mode */
  return offset + STARTING_FRAME_OFFSET;
}


static void
initiate_memory_slots (void)
{
  VARRAY_INT_INIT (end_slot_numbers_varray, 1000,
		   "number of slots whose end is at given byte");
  pending_free_memory_slot_pending_p = false;
  VARRAY_GENERIC_PTR_NOGC_INIT (pending_free_memory_slot_varray, 1000,
				"memory slot structures postponed for reuse");
  slot_memory_size = 0;
  initiate_stack_memory ();
  free_memory_slot_structures = NULL;
  slot_memory_alignment = 0;
  align_counts = yara_allocate (sizeof (int)
				* (BIGGEST_ALIGNMENT / BITS_PER_UNIT + 1));
  memset (align_counts, 0,
	  sizeof (int) * (BIGGEST_ALIGNMENT / BITS_PER_UNIT + 1));
  can_memory_slots
    = yara_allocate (sizeof (struct memory_slot *) * allocnos_num);
  memset (can_memory_slots, 0, sizeof (struct memory_slot *) * allocnos_num);
#ifdef SECONDARY_MEMORY_NEEDED
  secondary_memory_copies = yara_allocate_bitmap ();
#endif
}


#ifdef SECONDARY_MEMORY_NEEDED
static bool
can_copy_conflict_p (can_t can, copy_t cp)
{
  int n;
  int i;
  allocno_t a, *can_allocnos;
  copy_t *copy_vec;
  copy_t conflict_cp;

  can_allocnos = CAN_ALLOCNOS (can);
  for (n = 0; (a = can_allocnos [n]) != NULL; n++)
    {
      copy_vec = ALLOCNO_COPY_CONFLICT_VEC (a);
      for (i = 0; (conflict_cp = copy_vec [i]) != NULL; i++)
	{
	  if (conflict_cp == cp)
	    return true;
	}
    }
  return false;
}
#endif

static void
register_slot_start_change (int new, struct memory_slot *slot)
{
  log_memory_slot (slot);
#ifdef FRAME_GROWS_DOWNWARD
  remove_memory_slot_end (slot->start);
  add_memory_slot_end (new);
#else
  remove_memory_slot_end (slot->start + slot->size - 1);
  add_memory_slot_end (new + slot->size - 1);
#endif
}

void
print_memory_slot (FILE *f, const char *head, int indent,
		   struct memory_slot *slot)
{
  fprintf (f, "%s", head);
  if (slot->mem != NULL_RTX)
    {
      fprintf (f, " rtx=");
      print_inline_rtx (f, slot->mem, 5 + indent + strlen (head));
      fprintf (f, ", ");
    }
  fprintf (f, "start=%d, size=%d", slot->start, slot->size);
}

static void
register_memory_slot_usage (struct memory_slot *slot, int align)
{
  if (slot->allocnos_num == 0)
    {
      yara_assert (slot->size > 0);
#ifdef FRAME_GROWS_DOWNWARD
      add_memory_slot_end (slot->start);
#else
      add_memory_slot_end (slot->start + slot->size - 1);
#endif
    }
  increase_align_count (align);
  slot->allocnos_num++;
}

static void
unregister_memory_slot_usage (struct memory_slot *slot, int align)
{
  yara_assert (slot != NULL && slot->allocnos_num > 0);
  slot->allocnos_num--;
  decrease_align_count (align);
  if (slot->allocnos_num == 0)
    {
#ifdef FRAME_GROWS_DOWNWARD
      remove_memory_slot_end (slot->start);
#else
      remove_memory_slot_end (slot->start + slot->size - 1);
#endif
      free_memory_slot_structure (slot);
    }
}

/* We have (SUBREG:mode of MEMORY_SLOT_SIZE (RMODE: memory) 0), return
   offset of the memory in all the paradoxical subreg.  */
int
get_paradoxical_subreg_memory_offset (int memory_slot_size,
				      enum machine_mode rmode)
{
  int offset = 0;
  int difference = memory_slot_size - GET_MODE_SIZE (rmode);

  yara_assert (difference >= 0);
  if (WORDS_BIG_ENDIAN)
    offset += (difference / UNITS_PER_WORD) * UNITS_PER_WORD;
  if (BYTES_BIG_ENDIAN)
    offset += difference % UNITS_PER_WORD;
  return offset;
}

static enum machine_mode
choose_cp_mode (int hard_regno,
		enum machine_mode smode, enum machine_mode rmode)
{
  enum machine_mode mode;

  if (GET_MODE_SIZE (rmode) < GET_MODE_SIZE (smode))
    {
      if (! HARD_REGNO_MODE_OK (hard_regno, rmode))
	{
	  for (mode = mode_inner_mode [smode];
	       mode != VOIDmode
		 && GET_MODE_SIZE (mode) != GET_MODE_SIZE (rmode);
	       mode = mode_inner_mode [mode])
	    ;
	  if (mode != VOIDmode)
	    {
	      if (HARD_REGNO_MODE_OK (hard_regno, mode))
		rmode = mode;
	      else if (HARD_REGNO_MODE_OK (hard_regno, smode))
		rmode = smode; /* try bigger mode */
	    }
	}
      return rmode;
    }
  else
    {
      if (! HARD_REGNO_MODE_OK (hard_regno, smode))
	{
	  for (mode = rmode;
	       mode != VOIDmode
		 && GET_MODE_SIZE (mode) != GET_MODE_SIZE (smode);
	       mode = mode_inner_mode [mode])
	    ;
	  if (mode != VOIDmode)
	    {
	      if (HARD_REGNO_MODE_OK (hard_regno, mode))
		smode = mode;
	      else if (HARD_REGNO_MODE_OK (hard_regno, rmode))
		smode = rmode; /* try bigger mode */
	    }
	}
      return smode;
    }
}

enum machine_mode
get_copy_mode (copy_t cp)
{
  int hard_regno;
  allocno_t src, dst;
  rtx x = NULL_RTX;
  enum machine_mode cp_mode, rmode, amode;
  
  src = COPY_SRC (cp);
  dst = COPY_DST (cp);
  if (src != NULL && ALLOCNO_TYPE (src) == INSN_ALLOCNO)
    {
      cp_mode = amode = ALLOCNO_MODE (src);
      SKIP_TO_SUBREG (x, *INSN_ALLOCNO_LOC (src));
      if (GET_CODE (x) == SUBREG)
        {
	  rmode = GET_MODE (SUBREG_REG (x));
	  if (((hard_regno = ALLOCNO_HARD_REGNO (src)) >= 0
	       || (dst != NULL
		   && (hard_regno = ALLOCNO_HARD_REGNO (dst)) >= 0)
	       || (dst == NULL && (hard_regno = ALLOCNO_REGNO (src)) >= 0)))
	    return choose_cp_mode (hard_regno, amode, rmode);
	}
      return cp_mode;
    }
  else if (dst != NULL && ALLOCNO_TYPE (dst) == INSN_ALLOCNO)
    {
      cp_mode = amode = ALLOCNO_MODE (dst);
      SKIP_TO_SUBREG (x, *INSN_ALLOCNO_LOC (dst));
      if (GET_CODE (x) == SUBREG)
	{
	  rmode = GET_MODE (SUBREG_REG (x));
	  if (((hard_regno = ALLOCNO_HARD_REGNO (dst)) >= 0
	       || (src != NULL
		   && (hard_regno = ALLOCNO_HARD_REGNO (src)) >= 0)
	       || (src == NULL && (hard_regno = ALLOCNO_REGNO (dst)) >= 0)))
	    return choose_cp_mode (hard_regno, amode, rmode);
	}
      return cp_mode;
    }
  else
    {
      yara_assert (src != NULL && dst != NULL);
      return ALLOCNO_MODE (src);
    }
}

void
get_copy_loc (copy_t cp, bool src_p, enum machine_mode *mode,
	      int *hard_regno, struct memory_slot **memory_slot, int *offset)
{
  int a_hard_regno, a2_hard_regno, byte;
  allocno_t src, dst, a, a2;
  rtx x = NULL_RTX;
  enum machine_mode amode;

  src = COPY_SRC (cp);
  dst = COPY_DST (cp);
  *offset = 0;
  if (src_p)
    {
      a = src;
      a2 = dst;
    }
  else
    {
      a = dst;
      a2 = src;
    }
  byte = 0;
  *mode = get_copy_mode (cp);
  if (a != NULL && ALLOCNO_TYPE (a) == INSN_ALLOCNO)
    {
      amode = ALLOCNO_MODE (a);
      SKIP_TO_SUBREG (x, *INSN_ALLOCNO_LOC (a));
      if (GET_CODE (x) == SUBREG)
	byte = SUBREG_BYTE (x);
    }
  else if (a2 != NULL && ALLOCNO_TYPE (a2) == INSN_ALLOCNO)
    {
      amode = (a != NULL ? ALLOCNO_MODE (a) : ALLOCNO_MODE (a2));
      SKIP_TO_SUBREG (x, *INSN_ALLOCNO_LOC (a2));
      if (GET_CODE (x) == SUBREG)
	byte = SUBREG_BYTE (x);
    }
  else
    {
      yara_assert (a != NULL && a2 != NULL);
      amode = ALLOCNO_MODE (a);
    }
  if (a != NULL)
    {
      *memory_slot = NULL;
      if ((src_p  && (a_hard_regno = COPY_SUBST_SRC_HARD_REGNO (cp)) >= 0)
	   || (a_hard_regno = ALLOCNO_HARD_REGNO (a)) >= 0)
	{
	  if (ALLOCNO_TYPE (a) == INSN_ALLOCNO
	      && GET_MODE_SIZE (*mode) < GET_MODE_SIZE (amode)
	      && (! src_p || COPY_SUBST_SRC_HARD_REGNO (cp) < 0))
	    /* Paradoxical */
	    *hard_regno = (a_hard_regno
			   - (int) subreg_regno_offset (a_hard_regno, *mode,
							byte, amode));
	  else if (ALLOCNO_TYPE (a) == INSN_ALLOCNO
		   || (src_p && COPY_SUBST_SRC_HARD_REGNO (cp) >= 0))
	    *hard_regno = a_hard_regno;
	  else
	    *hard_regno = (a_hard_regno
			   + subreg_regno_offset (a_hard_regno, amode,
						  byte, *mode));
	}
      else if (ALLOCNO_MEMORY_SLOT (a) != NULL)
	{
	  *hard_regno = -1;
	  *memory_slot = ALLOCNO_MEMORY_SLOT (a);
	  *offset = ALLOCNO_MEMORY_SLOT_OFFSET (a);
	  if (ALLOCNO_TYPE (a) == INSN_ALLOCNO
	      && GET_MODE_SIZE (*mode) < GET_MODE_SIZE (amode))
	    *offset
	      += get_paradoxical_subreg_memory_offset (GET_MODE_SIZE (amode),
						       *mode);
	  else if (ALLOCNO_TYPE (a) != INSN_ALLOCNO)
	    *offset += byte;
	}
      else
	*hard_regno = -1;
    }
  else
    {
      yara_assert (a2 != NULL && x != NULL_RTX);
      *hard_regno = -1;
      *memory_slot = NULL;
      if ((a2_hard_regno = ALLOCNO_REGNO (a2)) >= 0)
	{
	  yara_assert (HARD_REGISTER_NUM_P (a2_hard_regno));
	  if (GET_MODE_SIZE (*mode) < GET_MODE_SIZE (amode))
	    *hard_regno = (a2_hard_regno
			   - (int) subreg_regno_offset (a2_hard_regno, *mode,
							byte, amode));
	  else if (amode != *mode)
	    *hard_regno = (a2_hard_regno
			   + (int) subreg_regno_offset (a2_hard_regno, *mode,
							byte, amode));
	  else
	    *hard_regno = a2_hard_regno;
	}
      else if (amode != *mode && MEM_P (SUBREG_REG (x)))
	{
	  if (GET_MODE_SIZE (*mode) < GET_MODE_SIZE (amode))
	    *offset
	      = get_paradoxical_subreg_memory_offset (GET_MODE_SIZE (amode),
						      *mode);
	  else
	    *offset = byte;
	}
    }
}

static void
allocate_allocno_memory_slot (allocno_t a)
{
  bitmap_iterator bi;
  unsigned int i;
  int align, num, slot_size, *vec, no;
  int regno = ALLOCNO_REGNO (a);
  struct memory_slot *slot, *conflict_slot;
  can_t can;

  yara_assert (regno >= 0);
  can = ALLOCNO_CAN (a);
  yara_assert (can != NULL);
  num = CAN_SLOTNO (can);
  align = slotno_max_ref_align [num];
  if (can_memory_slots [num] != NULL)
    slot = ALLOCNO_MEMORY_SLOT (a) = can_memory_slots [num];
  else
    {
      slot = ALLOCNO_MEMORY_SLOT (a) = get_free_memory_slot_structure ();
      if (reg_equiv_memory_loc [regno] != NULL_RTX
	  && (slotno_max_ref_size [num]
	      <= GET_MODE_SIZE (GET_MODE (reg_equiv_memory_loc [regno]))))
	{
	  slot->mem_index = reg_equiv_memory_index [regno];
	  slot->mem = reg_equiv_memory_loc [regno];
	}
      else
	{
	  slot->size = slotno_max_ref_size [num];
	  free_all_stack_memory ();
	  vec = slotno_conflicts [CAN_SLOTNO (can)];
	  if (vec != NULL)
	    for (i = 0; (no = vec [i]) >= 0; i++)
	      if ((conflict_slot = can_memory_slots [no]) != NULL
		  && conflict_slot->mem == NULL_RTX)
		reserve_stack_memory (conflict_slot->start,
				      conflict_slot->size);
#ifdef SECONDARY_MEMORY_NEEDED
	  EXECUTE_IF_SET_IN_BITMAP (secondary_memory_copies, 0, i, bi)
	    {
	      if (can_copy_conflict_p (can, copies [i]))
		{
		  yara_assert (COPY_CHANGE_ADDR (copies [i]) != NULL);
		  conflict_slot = COPY_MEMORY_SLOT (copies [i]);
		  yara_assert (conflict_slot != NULL);
		  reserve_stack_memory (conflict_slot->start,
					conflict_slot->size);
		}
	    }
#endif
	  slot->start = find_free_stack_memory (slot->size, align);
	  yara_assert (slot->size > 0);
	}
      can_memory_slots [num] = slot;
    }
  slot_size = (slot->mem != NULL
	       ? GET_MODE_SIZE (GET_MODE (slot->mem)) : slot->size);

  ALLOCNO_MEMORY_SLOT_OFFSET (a)
    = get_paradoxical_subreg_memory_offset (slot_size, ALLOCNO_MODE (a));
  if (ALLOCNO_TYPE (a) == INSN_ALLOCNO)
    {
      rtx x;

      SKIP_TO_SUBREG (x, *INSN_ALLOCNO_LOC (a));
      if (GET_CODE (x) == SUBREG)
	ALLOCNO_MEMORY_SLOT_OFFSET (a) = SUBREG_BYTE (x);
    }
  if (slot->mem == NULL_RTX)
    register_memory_slot_usage (slot, align);
}

static void
deallocate_allocno_memory_slot (allocno_t a)
{
  struct memory_slot *slot = ALLOCNO_MEMORY_SLOT (a);
  can_t can = ALLOCNO_CAN (a);
  int num, align;

  yara_assert (ALLOCNO_REGNO (a) >= 0 && can != NULL);
#ifdef REGNO_SLOT
  yara_assert (slot == regno_memory_slots [ALLOCNO_REGNO (a)]);
  align = reg_max_ref_align [ALLOCNO_REGNO (a)]
#else
  num = CAN_SLOTNO (can);
  yara_assert (slot == can_memory_slots [num]);
  align = slotno_max_ref_align [num];
#endif
  ALLOCNO_MEMORY_SLOT (a) = NULL;
  if (slot->mem != NULL_RTX)
    return;
  unregister_memory_slot_usage (slot, align);
  if (slot->allocnos_num == 0)
    {
#ifdef REGNO_SLOT
      regno_memory_slots [ALLOCNO_REGNO (a)] = NULL;
#else
      can_memory_slots [num] = NULL;
#endif
    }
  ALLOCNO_MEMORY_SLOT_OFFSET (a) = 0;
}

#ifdef SECONDARY_MEMORY_NEEDED

static void
allocate_copy_memory_slot (copy_t cp)
{
  int i, align;
  allocno_t a;
  can_t can;
  struct memory_slot *slot, *conflict_slot;
  allocno_t *vec;

  yara_assert (COPY_CHANGE_ADDR (cp) != NULL && COPY_MEMORY_SLOT (cp) == NULL);
  slot = COPY_MEMORY_SLOT (cp) = get_free_memory_slot_structure ();
  bitmap_set_bit (secondary_memory_copies, COPY_NUM (cp));
  slot->size = GET_MODE_SIZE (COPY_MEMORY_MODE (cp));
  align = get_stack_align (COPY_MEMORY_MODE (cp)) / BITS_PER_UNIT;
  free_all_stack_memory ();
  vec = COPY_ALLOCNO_CONFLICT_VEC (cp);
  for (i = 0; (a = vec [i]) != NULL; i++)
    {
      can = ALLOCNO_CAN (a);
      if (can == NULL)
	continue;
      if ((conflict_slot = can_memory_slots [CAN_SLOTNO (can)]) != NULL
	  && conflict_slot->mem == NULL_RTX)
	reserve_stack_memory (conflict_slot->start, conflict_slot->size);
    }
  slot->start = find_free_stack_memory (slot->size, align);
  yara_assert (slot->size > 0);
  register_memory_slot_usage (slot, align);
}

static void
deallocate_copy_memory_slot (copy_t cp)
{
  struct memory_slot *slot;
  int align;

  yara_assert (COPY_CHANGE_ADDR (cp) != NULL);
  slot = COPY_MEMORY_SLOT (cp);
  align = get_stack_align (COPY_MEMORY_MODE (cp)) / BITS_PER_UNIT;
  yara_assert (slot->mem == NULL_RTX);
  unregister_memory_slot_usage (slot, align);
  COPY_MEMORY_SLOT (cp) = NULL;
  bitmap_clear_bit (secondary_memory_copies, COPY_NUM (cp));
}
#endif

/* The function is used to sort cans in order to put cans with higher
   memory cost first.  */
static int
can_compare (const void *c1p, const void *c2p)
{
  can_t c1 = *(can_t *) c1p;
  can_t c2 = *(can_t *) c2p;
  int diff;

  diff = (CAN_MEMORY_COST (c2) - CAN_MEMORY_COST (c1));
  if (diff != 0)
    return diff;
  return CAN_NUM (c1) - CAN_NUM (c2);
}

/* The following function tries to compact stack.  It might decrease
   (but never increase) the displacement from the stack start.  */
void
compact_stack (void)
{
  int i, j, n, slot_no;
  unsigned int k;
  can_t can;
  int *vec;
  int start, align;
  allocno_t a, *a_vec;
  copy_t cp;
  struct memory_slot *slot, *conflict_slot;
  bitmap_iterator bi;
  varray_type slot_memory_can_varray;

  /* Sort cans to consider more higher cost can moves first.  */
  VARRAY_GENERIC_PTR_NOGC_INIT (slot_memory_can_varray,
				cans_num, "cans with slot memory");
  for (i = 0; i < cans_num; i++)
    {
      can = cans [i];
      if (can == NULL)
	continue;
      slot_no = CAN_SLOTNO (can);
      if ((slot = can_memory_slots [slot_no]) == NULL || slot->mem != NULL_RTX)
	continue;
      VARRAY_PUSH_GENERIC_PTR (slot_memory_can_varray, can);
    }
  if (VARRAY_ACTIVE_SIZE (slot_memory_can_varray) != 0)
    qsort (&VARRAY_GENERIC_PTR (slot_memory_can_varray, 0),
	   VARRAY_ACTIVE_SIZE (slot_memory_can_varray),
	   sizeof (can_t), can_compare);
  /* Try to move can slots to closer stack start.  */
  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (slot_memory_can_varray); i++)
    {
      can = VARRAY_GENERIC_PTR (slot_memory_can_varray, i);
      if (can == NULL)
	continue;
      slot_no = CAN_SLOTNO (can);
      if ((slot = can_memory_slots [slot_no]) == NULL || slot->mem != NULL_RTX)
	continue;
      align = slotno_max_ref_align [slot_no];
      free_all_stack_memory ();
      vec = slotno_conflicts [slot_no];
      if (vec != NULL)
	for (j = 0; (n = vec [j]) >= 0; j++)
	  if ((conflict_slot = can_memory_slots [n]) != NULL
	      && conflict_slot->mem == NULL_RTX)
	    reserve_stack_memory (conflict_slot->start, conflict_slot->size);
#ifdef SECONDARY_MEMORY_NEEDED
      EXECUTE_IF_SET_IN_BITMAP (secondary_memory_copies, 0, k, bi)
	{
	  if (can_copy_conflict_p (can, copies [k]))
	    {
	      yara_assert (COPY_CHANGE_ADDR (copies [k]) != NULL);
	      conflict_slot = COPY_MEMORY_SLOT (copies [k]);
	      yara_assert (conflict_slot != NULL
			   && conflict_slot->mem == NULL_RTX);
	      reserve_stack_memory (conflict_slot->start, conflict_slot->size);
	    }
	}
#endif
      start = find_free_stack_memory (slot->size, align);
      yara_assert (slot->start >= start);
      if (start == slot->start)
	continue;
      register_slot_start_change (start, slot);
      slot->start = start;
    }
  VARRAY_FREE (slot_memory_can_varray);
#ifdef SECONDARY_MEMORY_NEEDED
  /* Try to move slots used for secondary memory closer to the stack
     start.  */
  EXECUTE_IF_SET_IN_BITMAP (secondary_memory_copies, 0, k, bi)
    {
      cp = copies [k];
      slot = COPY_MEMORY_SLOT (cp);
      free_all_stack_memory ();
      align = get_stack_align (COPY_MEMORY_MODE (cp)) / BITS_PER_UNIT;
      a_vec = COPY_ALLOCNO_CONFLICT_VEC (cp);
      for (j = 0; (a = a_vec [j]) != NULL; j++)
	{
	  can = ALLOCNO_CAN (a);
	  if (can == NULL)
	    continue;
	  if ((conflict_slot = can_memory_slots [CAN_SLOTNO (can)]) != NULL
	      && conflict_slot->mem == NULL_RTX)
	    reserve_stack_memory (conflict_slot->start, conflict_slot->size);
	}
      start = find_free_stack_memory (slot->size, align);
      yara_assert (slot->start >= start);
      if (start == slot->start)
	continue;
      register_slot_start_change (start, slot);
      slot->start = start;
    }
#endif
}

static void
finish_memory_slots (void)
{
  int i;
  struct memory_slot *slot;

  yara_assert (! pending_free_memory_slot_pending_p
	       && VARRAY_ACTIVE_SIZE (pending_free_memory_slot_varray) == 0);
  VARRAY_FREE (pending_free_memory_slot_varray);
  yara_free (align_counts);
  for (i = 0; i < allocnos_num; i++)
    if (can_memory_slots [i] != NULL)
      yara_free (can_memory_slots [i]);
  while (free_memory_slot_structures != NULL)
    {
      slot = free_memory_slot_structures->next_free_slot;
      yara_free (free_memory_slot_structures);
      free_memory_slot_structures = slot;
    }
#ifdef SECONDARY_MEMORY_NEEDED
  yara_free_bitmap (secondary_memory_copies);
#endif
  yara_free (can_memory_slots);
  finish_stack_memory ();
}



bool
hard_reg_in_set_p (int hard_regno, enum machine_mode mode,
		   HARD_REG_SET hard_regset)
{
  int i;

  yara_assert (hard_regno >= 0);
  for (i = hard_regno_nregs [hard_regno] [mode] - 1; i >= 0; i--)
    if (! TEST_HARD_REG_BIT (hard_regset, hard_regno + i))
      return false;
  return true;
}

bool
hard_reg_not_in_set_p (int hard_regno, enum machine_mode mode,
		       HARD_REG_SET hard_regset)
{
  int i;

  yara_assert (hard_regno >= 0);
  for (i = hard_regno_nregs [hard_regno] [mode] - 1; i >= 0; i--)
    if (TEST_HARD_REG_BIT (hard_regset, hard_regno + i))
      return false;
  return true;
}



static GTY(()) rtx temp_const_int;
static GTY(()) rtx temp_const;
static GTY(()) rtx temp_plus;
static GTY(()) rtx temp_reg [MAX_MACHINE_MODE];
static GTY(()) rtx temp_stack_disp_mem [MAX_MACHINE_MODE];
static GTY(()) rtx temp_hard_frame_disp_mem [MAX_MACHINE_MODE];

static void
setup_temp_mems_and_addresses (void)
{
  int mode;

  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
    {
      temp_reg [mode] = gen_raw_REG ((enum machine_mode) mode, 0);
      temp_stack_disp_mem [mode]
        = gen_rtx_MEM (mode, gen_rtx_PLUS (Pmode, stack_pointer_rtx,
					   const0_rtx));
      temp_hard_frame_disp_mem [mode]
	= gen_rtx_MEM (mode, gen_rtx_PLUS (Pmode, hard_frame_pointer_rtx,
					   const0_rtx));
    }
  temp_const_int = gen_rtx_raw_CONST_INT (VOIDmode, 0);
  temp_plus = gen_rtx_PLUS (Pmode, gen_rtx_REG (Pmode, 0), const0_rtx);
  temp_const = gen_rtx_CONST (Pmode, temp_plus);
}

static rtx
get_temp_const_int (HOST_WIDE_INT disp)
{
  if (disp >= -MAX_SAVED_CONST_INT && disp <= MAX_SAVED_CONST_INT)
    return gen_rtx_CONST_INT (VOIDmode, disp);
  INTVAL (temp_const_int) = disp;
  return temp_const_int;
}

static rtx
get_temp_disp (rtx disp, HOST_WIDE_INT offset)
{
  enum rtx_code code;

  if (offset == 0)
    ;
  else if (disp == NULL_RTX)
    disp = get_temp_const_int (offset);
  else if ((code = GET_CODE (disp)) == CONST_INT)
    disp = get_temp_const_int (INTVAL (disp) + offset);
  else if (code == SYMBOL_REF || code == LABEL_REF)
    {
      XEXP (temp_plus, 0) = disp;
      XEXP (temp_plus, 1) = get_temp_const_int (offset);
      disp = temp_const;
      XEXP (disp, 0) = temp_plus;
    }
  else if (code == CONST)
    {
      code = GET_CODE (XEXP (disp, 0));
      if (code == PLUS
	  && GET_CODE (XEXP (XEXP (disp, 0), 1)) == CONST_INT)
	{
	  /* Minus is not used when the second operand is
	     CONST_INT.  */
	  XEXP (temp_plus, 0) = XEXP (XEXP (disp, 0), 0);
	  XEXP (temp_plus, 1)
	    = get_temp_const_int (INTVAL (XEXP (XEXP (disp, 0), 1)) + offset);
	}
      else
	{
	  XEXP (temp_plus, 0) = XEXP (disp, 0);
	  XEXP (temp_plus, 1) = get_temp_const_int (offset);
	}
      disp = temp_const;
      XEXP (disp, 0) = temp_plus;
    }
  else
    gcc_unreachable ();
  return disp;
}

static rtx
get_temp_stack_memory_slot_rtx (enum machine_mode mode, HOST_WIDE_INT disp,
				int size)
{
  rtx mem;
  HOST_WIDE_INT offset;

  if (stack_frame_pointer_can_be_eliminated_p
      && obligatory_stack_frame_pointer_elimination_p)
    {
      /* disp is addressed from the stack bottom in this case.  */
      mem = temp_stack_disp_mem [mode];
      offset = (frame_stack_pointer_offset + disp
		- rounded_slot_memory_size ());
#ifdef FRAME_GROWS_DOWNWARD
      offset += get_stack_memory_start_frame_offset () - size + 2;
#else
      offset += size - get_stack_memory_start_frame_offset ();
#endif
    }
  else
    {
      mem = temp_hard_frame_disp_mem [mode];
      offset = (get_stack_memory_start_frame_offset ()
		+ frame_hard_frame_pointer_offset);
#ifdef FRAME_GROWS_DOWNWARD
      offset -= disp;
#else
      offset += disp;
#endif
    }
  XEXP (XEXP (mem, 0), 1) = get_temp_const_int (disp);
  return mem;
}



#ifndef REGNO_MODE_OK_FOR_BASE_P
#define REGNO_MODE_OK_FOR_BASE_P(REGNO, MODE) REGNO_OK_FOR_BASE_P (REGNO)
#endif

/* The function sets up sets of hard registers which can be used for
   base and index registers.  */
static void
set_base_index_reg_sets (void)
{
  int i, mode;
  short *saved_reg_renumber;
  short temp_array [FIRST_PSEUDO_REGISTER];

  /* REGNO_OK_FOR_INDEX_P and REGNO_MODE_OK_FOR_BASE_P can use
     reg_renumber so we need to define it temporarily.  */
  saved_reg_renumber = reg_renumber;
  reg_renumber = temp_array;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    temp_array [i] = -1;
  CLEAR_HARD_REG_SET (index_regs);
  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
    CLEAR_HARD_REG_SET (base_regs [mode]);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (REGNO_OK_FOR_INDEX_P (i))
	SET_HARD_REG_BIT (index_regs, i);
      
      for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
	if (REGNO_MODE_OK_FOR_BASE_P (i, mode))
	  SET_HARD_REG_BIT (base_regs [mode], i);
    }
  saved_reg_renumber = reg_renumber;
}

int minimal_memory_load_cost [MAX_MACHINE_MODE];
int minimal_memory_store_cost [MAX_MACHINE_MODE];

static void
setup_move_costs (void)
{
  int mode, cl, cost;
  
  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
    {
      minimal_memory_store_cost [mode] = minimal_memory_load_cost [mode] = -1;
      for (cl = (int) N_REG_CLASSES - 1; cl >= 0; cl--)
	{
	  cost = memory_move_cost [mode] [cl] [0];
	  if (cost > 0 &&  (minimal_memory_store_cost [mode] < 0
			    || minimal_memory_store_cost [mode] > cost))
	    minimal_memory_store_cost [mode] = cost;
	  cost = memory_move_cost [mode] [cl] [1];
	  if (cost > 0 &&  (minimal_memory_load_cost [mode] < 0
			    || minimal_memory_load_cost [mode] > cost))
	    minimal_memory_load_cost [mode] = cost;
	}
    }
}

/* Return class of hard register gotten by A, return NO_REGS
   otherwise.  */
static enum reg_class
get_allocno_reg_class (allocno_t a)
{
  enum reg_class cl;

  if (ALLOCNO_HARD_REGNO (a) >= 0)
    cl = REGNO_REG_CLASS (ALLOCNO_HARD_REGNO (a));
  else
    cl = NO_REGS; /* memory or no allocation yet.  */
  return cl;
}

/* This function is called only when an allocation is done for non
   pseudo-register (insn) allocno A.  It returns the cost of this
   allocation (reload in old sense).  */
static int
non_pseudo_allocno_copy_cost (allocno_t a)
{
  int regno;
  enum machine_mode mode;
  rtx op;
  copy_t src_copy, dst_copy;
  int cost;
  enum reg_class cl;
  struct memory_slot *memory_slot;

  yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
  regno = ALLOCNO_REGNO (a);
  yara_assert (regno < 0 || HARD_REGISTER_NUM_P (regno));
  mode = ALLOCNO_MODE (a);
  src_copy = ALLOCNO_SRC_COPIES (a);
  dst_copy = ALLOCNO_DST_COPIES (a);
  yara_assert ((src_copy == NULL || COPY_DST (src_copy) == NULL)
	       && (dst_copy == NULL || COPY_SRC (dst_copy) == NULL));
  cl = get_allocno_reg_class (a);
  SKIP_TO_REG (op, *INSN_ALLOCNO_LOC (a));
  memory_slot = ALLOCNO_MEMORY_SLOT (a);
  if (regno >= 0)
    {
      /* It is a hard register.  */
      enum reg_class regno_class = REGNO_REG_CLASS (regno);
      
      yara_assert (REG_P (op));
      if (memory_slot != NULL)
	cost = ((src_copy == NULL
		 ? 0 : memory_move_cost [mode] [regno_class] [1])
		+ (dst_copy == NULL
		   ? 0 : memory_move_cost [mode] [regno_class] [0]));
      else if (cl != NO_REGS)
	cost = ((src_copy == NULL
		 ? 0 : register_move_cost [mode] [cl] [regno_class])
		+ (dst_copy == NULL
		   ? 0 : register_move_cost [mode] [regno_class] [cl]));
      else
	/* This allocno did not get memory or a hard register because
	   it is ok for constraints.  */
	cost = 0;
      cost *= COST_FACTOR;
    }
  else if (MEM_P (op) || CONST_POOL_OK_P (op))
    {
      if (cl != NO_REGS)
	cost = (COST_FACTOR
		* ((src_copy == NULL ? 0 : memory_move_cost [mode] [cl] [0])
		   + (dst_copy == NULL
		      ? 0 : memory_move_cost [mode] [cl] [1])));
      else if (memory_slot != NULL)
	cost = 0;
      else if (INSN_ALLOCNO_CONST_POOL_P (a))
	return COST_FACTOR * minimal_memory_load_cost [mode];
      else if (MEM_P (op) && INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a))
	/* If even memory did not get register memory, it is still
	   costly to access but less than loading it in hard register.  */
	cost = (COST_FACTOR
		* ((src_copy == NULL ? 0 : minimal_memory_store_cost [mode])
		   + (dst_copy == NULL ? 0 : minimal_memory_load_cost [mode]))
		- COST_FACTOR / 2);
      else /* case for non-allocated allocno is here too.  */
	cost = 0;
    }
  else if (GET_CODE (op) == SCRATCH)
    {
      /* It might be scratch without constraint so CL==NO_REGS is
	 possible.  */
      cost = 0;
    }
  else
    {
      /* an operation and others ??? */
      yara_assert (cl == NO_REGS && memory_slot == NULL);
      cost = 0;
    }
  return cost * (src_copy != NULL ? COPY_FREQ (src_copy)
		 : dst_copy != NULL ? COPY_FREQ (dst_copy)
		 : BLOCK_FOR_INSN (INSN_ALLOCNO_INSN (a))->frequency);
}

/* Cost of copy insns of allocno A which has been allocated.  We don't
   use information about secondary reloads or memory because it is
   hard to say the cost of the reload patterns (how many insns are
   generated by a reload pattern).  The cost of secondary reloads and
   used memory are included and should be included in
   REGISTER_MOVE_COST and MEMORY_MOVE_COST.  */
int
allocno_copy_cost (allocno_t a)
{
  enum machine_mode mode;
  copy_t cp, src_copy, dst_copy;
  int cost;

  mode = ALLOCNO_MODE (a);
  src_copy = ALLOCNO_SRC_COPIES (a);
  dst_copy = ALLOCNO_DST_COPIES (a);
  yara_assert (ALLOCNO_TYPE (a) != INSN_ALLOCNO
	       || src_copy == NULL || COPY_NEXT_SRC_COPY (src_copy) == NULL
	       || dst_copy == NULL || dst_copy->next_dst_copy == NULL);
  if (ALLOCNO_REGNO (a) < 0 || HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a)))
    cost = non_pseudo_allocno_copy_cost (a);
  else
    {
      cost = 0;
      for (cp = dst_copy; cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
	if (COPY_SRC (cp) != COPY_DST (cp))
	  cost += (*pseudo_reg_copy_cost_func) (cp);
      for (cp = src_copy; cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
	if (COPY_SRC (cp) != COPY_DST (cp))
	  cost += (*pseudo_reg_copy_cost_func) (cp);
    }
  yara_assert (cost >= 0);
  return cost;
}



/* Check that HARD_REGNO of class CL is ok for A, i.e. maximal part is
   not in PROHIBITED_HARD_REGS and MODE is ok for HARD_REGNO.  */
static bool
check_hard_reg (int hard_regno, allocno_t a,
		HARD_REG_SET prohibited_hard_regs, bool no_alloc_reg_p)
{
  int start;
  enum machine_mode allocation_mode;

  /* ??? apply no_alloc_regs reg_class_contents.  */
  if (! no_alloc_reg_p)
    IOR_HARD_REG_SET (prohibited_hard_regs, no_alloc_regs);
  if (! HARD_REGNO_MODE_OK (hard_regno, ALLOCNO_MODE (a)))
    return false;
  /* ??? it is calclulated each time when it is called from
     find_hard_reg.  */
  allocation_mode = get_allocation_mode (a);
  start = get_maximal_part_start_hard_regno (hard_regno, a);
  if (start < 0)
    return false;
  if (! HARD_REGNO_MODE_OK (start, allocation_mode))
    return false;
  /* All allocated registers should be not prohibited.  */
  if (! hard_reg_not_in_set_p (start, allocation_mode,
			       prohibited_hard_regs))
    return false;
  return true;
}

/* Find hard regno of class CL for A.  */
static int
find_hard_reg (allocno_t a, enum reg_class cl,
	       HARD_REG_SET prohibited_hard_regs,
	       int *possible_hard_regnos, int possible_hard_regnos_num)
{
  int i, hard_regno;
  HARD_REG_SET temp_set;

  COMPL_HARD_REG_SET (temp_set, reg_class_contents [cl]);
  for (i = 0; i < possible_hard_regnos_num; i++)
    {
      hard_regno = possible_hard_regnos [i];
      if (check_hard_reg (hard_regno, a, temp_set, false))
	{
	  yara_assert (hard_reg_in_set_p (hard_regno, ALLOCNO_MODE (a),
					  reg_class_contents [cl]));
	  return hard_regno;
	}
    }
  for (i = 0; i < (int) class_hard_regs_num [cl]; i++)
    {
      hard_regno = class_hard_regs [cl] [i];
      if (check_hard_reg (hard_regno, a, prohibited_hard_regs, false))
	{
	  yara_assert (hard_reg_in_set_p (hard_regno, ALLOCNO_MODE (a),
					  reg_class_contents [cl]));
	  return hard_regno;
	}
    }
  return -1;
}

/* Find hard regno of class CL for MODE.  */
static int
find_hard_reg_for_mode (enum reg_class cl, enum machine_mode mode,
			HARD_REG_SET prohibited_hard_regs)
{
  int i, hard_regno;

  for (i = 0; i < (int) class_hard_regs_num [cl]; i++)
    {
      hard_regno = class_hard_regs [cl] [i];
      if (HARD_REGNO_MODE_OK (hard_regno, mode)
	  && hard_reg_not_in_set_p (hard_regno, mode, prohibited_hard_regs))
	return hard_regno;
    }
  return -1;
}

#ifdef SECONDARY_MEMORY_NEEDED

/* Allocate secondary memory for copy CP if we need it for copying
   HARD_REGNO (if it is not negative) of MODE into/from (depending
   from IN_P) a hard register in a hard register of class CL and
   further into a hard register of class NEXT_CLASS if it is not
   NO_REGS.  Return false if we fail (because we need secondary memory
   for reload allocno containing an eliminated reg).  */
static bool
allocate_copy_secondary_memory (bool in_p, copy_t cp, int hard_regno,
				enum reg_class cl, enum reg_class next_class,
				enum machine_mode mode)
{
  if ((in_p && ((hard_regno >= 0
		 && SECONDARY_MEMORY_NEEDED (REGNO_REG_CLASS (hard_regno),
					     cl, mode))
		|| (next_class != NO_REGS
		    && SECONDARY_MEMORY_NEEDED (cl, next_class, mode))))
      || (! in_p && ((hard_regno >= 0
		      && SECONDARY_MEMORY_NEEDED (cl,
						  REGNO_REG_CLASS (hard_regno),
						  mode))
		     || (next_class != NO_REGS
			 && SECONDARY_MEMORY_NEEDED (next_class, cl, mode)))))
    {
      /* We don't allocate secondary memory for allocno containing
	 eliminated reg.  Remeber type of allocno with an eliminated
	 reg is always input.  */
      if (COPY_DST (cp) != NULL && ALLOCNO_TYPE (COPY_DST (cp)) == INSN_ALLOCNO
	  && INSN_ALLOCNO_ELIMINATION_P (COPY_DST (cp)))
	return false;
      if (COPY_CHANGE_ADDR (cp) == NULL)
	COPY_CHANGE_ADDR (cp) = get_free_copy_change ();
      COPY_MEMORY_MODE (cp) = mode;
      allocate_copy_memory_slot (cp);
    }
  return true;
}

#endif

/* The function checks (and returns true in case of success) that copy
   CP with source allocno using equivalent constant can be successfully
   generated and, if necessary, assigns an intermediate hard register
   will be used to assign the constant to the copy destination.  */
static bool
assign_copy_interm_equiv_const_hard_reg (copy_t cp)
{
  allocno_t src, dst;

  src = COPY_SRC (cp);
  dst = COPY_DST (cp);
  yara_assert (src != NULL && dst != NULL);
  if (ALLOCNO_USE_EQUIV_CONST_P (dst))
    return true;
  yara_assert (ALLOCNO_USE_EQUIV_CONST_P (src));
  /* ??? We need better code for all possible cases including usage of
     an intermediate register.*/
  if (ALLOCNO_MEMORY_SLOT (dst) != NULL)
    return false;
  return true;
}

/* The function frees an intermediate hard register (if any) used to
   assign the constant to the copy destination.  */
static void
unassign_copy_interm_equiv_const_hard_reg (copy_t cp)
{
  allocno_t src, dst;

  src = COPY_SRC (cp);
  dst = COPY_DST (cp);
  yara_assert (src != NULL && dst != NULL);
  if (ALLOCNO_USE_EQUIV_CONST_P (dst))
    return;
  yara_assert (ALLOCNO_USE_EQUIV_CONST_P (src));
}

/* The following function returns TRUE if moving SRC to DST is not
   prohibited with the point of changing mode when a subregister is
   involved.  */
static bool
allocno_change_mode_ok_p (allocno_t src, allocno_t dst)
{
  allocno_t insn_a, another_a;
  rtx op, x;
  int hard_regno;

  if (((insn_a = dst) != NULL && ALLOCNO_TYPE (dst) == INSN_ALLOCNO)
      || ((insn_a = src) != NULL && ALLOCNO_TYPE (src) == INSN_ALLOCNO))
    {
      op = *INSN_ALLOCNO_LOC (insn_a);
      if (GET_CODE (op) == SUBREG)
	{
	  SKIP_TO_REG (x, op);
	  another_a = (src == insn_a ? dst : src);
	  if (REG_P (x) &&
	      ((another_a != NULL
		&& (hard_regno = ALLOCNO_HARD_REGNO (another_a)) >= 0)
	       || (another_a == NULL
		   && (hard_regno = ALLOCNO_REGNO (insn_a)) >= 0))
	      && CANNOT_CHANGE_MODE_CLASS (GET_MODE (x), GET_MODE (op),
					   REGNO_REG_CLASS (hard_regno)))
	    return false;
	}
    }
  return true;
}

static bool
assign_copy (copy_t cp)
{
  bool in_p;
  enum reg_class cl;
  allocno_t a, a2;
  enum machine_mode mode;
  int regno, hard_regno;
  bool logged_p;

  in_p = false;
  a = COPY_SRC (cp);
  a2 = COPY_DST (cp);
  if (! allocno_change_mode_ok_p (a, a2))
    return false;
  if (a != NULL && ALLOCNO_USE_EQUIV_CONST_P (a)
      && ! assign_copy_interm_equiv_const_hard_reg (cp))
    return false;
  if (a == NULL)
    {
      a = COPY_DST (cp);
      a2 = NULL;
      in_p = true;
    }
  yara_assert (a != NULL);
  regno = ALLOCNO_REGNO (a);
  if (a2 != NULL)
    {
      yara_assert (regno >= 0 && ! HARD_REGISTER_NUM_P (regno)
		   && ALLOCNO_REGNO (a2) >= 0
		   && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a2)));
      if ((hard_regno = ALLOCNO_HARD_REGNO (a)) < 0)
	{
	  allocno_t tmp = a;

	  a = a2;
	  a2 = tmp;
	  in_p = ! in_p;
	  hard_regno = ALLOCNO_HARD_REGNO (a);
	}
      yara_assert (ALLOCNO_TYPE (a2) != INSN_ALLOCNO
		   || (! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a2)
		       && ! INSN_ALLOCNO_CONST_POOL_P (a2)));
      if (hard_regno < 0 || (ALLOCNO_HARD_REGNO (a2) < 0
			     && ALLOCNO_MEMORY_SLOT (a2) == NULL))
	return true; /* not assigned yet.  */
    }
  else if ((hard_regno = ALLOCNO_HARD_REGNO (a)) >= 0)
    {
      yara_assert (regno < 0 || HARD_REGISTER_NUM_P (regno));
      yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO
		   && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a)
		   && ! INSN_ALLOCNO_CONST_POOL_P (a));
    }
  else if (regno >= 0)
    {
      yara_assert (HARD_REGISTER_NUM_P (regno)
		   && ALLOCNO_TYPE (a) == INSN_ALLOCNO
		   && ! INSN_ALLOCNO_CONST_POOL_P (a));
      if (ALLOCNO_MEMORY_SLOT (a) == NULL
	  && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a))
	return true; /* not assigned yet.  */
      hard_regno = regno;
    }
  else
    return true; /* no hard register is involved.  */
  yara_assert (hard_regno >= 0);
  cl = REGNO_REG_CLASS (hard_regno);
  mode = get_copy_mode (cp);
  if (mode != ALLOCNO_MODE (a))
    {
      /* Subregisters are involed.  Make hard_regno more accurate.  */
      enum machine_mode cp_mode;
      struct memory_slot *memory_slot;
      int offset;

      get_copy_loc (cp, ! in_p, &cp_mode, &hard_regno, &memory_slot, &offset);
      yara_assert (cp_mode == mode && hard_regno >= 0 && memory_slot == NULL);
      cl = REGNO_REG_CLASS (hard_regno);
    }
  yara_assert (cl != NO_REGS);
  logged_p = false;
#ifdef HAVE_SECONDARY_RELOADS
  {
    int i;
    rtx x;
    enum machine_mode interm_mode = VOIDmode;
    enum machine_mode scratch_mode = VOIDmode;
    enum reg_class interm_class = NO_REGS;
    enum reg_class scratch_class = NO_REGS;
    int interm_hard_regno, scratch_hard_regno;
    HARD_REG_SET prohibited_hard_regs;
    enum insn_code icode = CODE_FOR_nothing;
    allocno_t conflict_a;
    allocno_t *vec;

    COPY_HARD_REG_SET (prohibited_hard_regs, COPY_HARD_REG_CONFLICTS (cp));
    if (regno < FIRST_PSEUDO_REGISTER)
      {
	yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
	x = *INSN_ALLOCNO_LOC (a);
      }
    else
      {
	struct memory_slot *slot;
	
	yara_assert (a2 != NULL);
	if (ALLOCNO_HARD_REGNO (a2) >= 0)
	  {
	    if (ALLOCNO_HARD_REGNO (a) == ALLOCNO_HARD_REGNO (a2))
	      return true;
	    x = temp_reg [(int) mode];
	    REGNO (x) = regno;
	    reg_renumber [regno] = ALLOCNO_HARD_REGNO (a2);
	    IOR_HARD_REG_SET
	      (prohibited_hard_regs,
	       reg_mode_hard_regset [ALLOCNO_HARD_REGNO (a2)] [mode]);
	  }
	else if ((slot = ALLOCNO_MEMORY_SLOT (a2)) != NULL)
	  {
	    x = slot->mem;
	    if (x == NULL)
	      x = get_temp_stack_memory_slot_rtx (mode, slot->start,
						  slot->size);
	  }
	else
	  gcc_unreachable ();
      }
    if (REG_P (x))
      interm_class = NO_REGS; /* we move register into register.  */
    else if (in_p)
      {
#ifdef SECONDARY_INPUT_RELOAD_CLASS
	interm_class = SECONDARY_INPUT_RELOAD_CLASS (cl, mode, x);
#endif
      }
    else
      {
#ifdef SECONDARY_OUTPUT_RELOAD_CLASS
	interm_class = SECONDARY_OUTPUT_RELOAD_CLASS (cl, mode, x);
#endif
      }
    if (interm_class != NO_REGS && regno >= 0 && ! HARD_REGISTER_NUM_P (regno)
	&& mode != spill_mode [mode])
      {
	/* Try another mode.  */
	enum reg_class temp_class = NO_REGS;

	if (in_p)
	  {
#ifdef SECONDARY_INPUT_RELOAD_CLASS
	    temp_class
	      = SECONDARY_INPUT_RELOAD_CLASS (cl, spill_mode [mode], x);
#endif
	  }
	else
	  {
#ifdef SECONDARY_OUTPUT_RELOAD_CLASS
	    temp_class
	      = SECONDARY_OUTPUT_RELOAD_CLASS (cl, spill_mode [mode], x);
#endif
	  }
	if (temp_class == NO_REGS)
	  {
	    mode = spill_mode [mode];
	    interm_class = temp_class;
	  }
      }
    if (interm_class != NO_REGS)
      {
#ifdef SECONDARY_RELOAD_MODE_P
	yara_assert (SECONDARY_RELOAD_MODE_P (mode));
#endif
	if (ALLOCNO_TYPE (a) == INSN_ALLOCNO && INSN_ALLOCNO_ELIMINATION_P (a))
	  return false;
	yara_assert (COPY_CHANGE_ADDR (cp) == NULL);
	logged_p = true;
	log_copy (cp);
	vec = COPY_ALLOCNO_CONFLICT_VEC (cp);
	for (i = 0; (conflict_a = vec [i]) != NULL; i++)
	  {
	    IOR_HARD_REG_SET (prohibited_hard_regs,
			      ALLOCNO_HARD_REGSET (conflict_a));
	    if (ALLOCNO_TYPE (conflict_a) == INSN_ALLOCNO)
	      IOR_HARD_REG_SET
		(prohibited_hard_regs,
		 INSN_ALLOCNO_INTERM_ELIMINATION_REGSET (conflict_a));
	  }
	interm_mode = mode;
	icode = (in_p
		 ? reload_in_optab [(int) mode]
		 : reload_out_optab [(int) mode]);
	if (icode != CODE_FOR_nothing
	    && insn_data [(int) icode].operand [in_p].predicate
	    && (! (insn_data [(int) icode].operand [in_p].predicate) (x,
								      mode)))
	  icode = CODE_FOR_nothing;
	if (icode == CODE_FOR_nothing)
	  {
#ifdef SECONDARY_MEMORY_NEEDED
	    hard_regno = (a2 == NULL
			  ? ALLOCNO_REGNO (a) : ALLOCNO_HARD_REGNO (a2));
	    if (! allocate_copy_secondary_memory (in_p, cp, hard_regno,
						  interm_class, cl, mode))
	      gcc_unreachable ();
#endif
	  }
	else
	  {
	    enum reg_class insn_class;
	    const char *str;
	    
	    if (insn_data [(int) icode].operand [!in_p].constraint [0] == 0)
	      insn_class = ALL_REGS;
	    else
	      {
		str = (&insn_data [(int) icode].operand [!in_p]
		       .constraint [in_p]);
		insn_class
		  = (*str == 'r' ? GENERAL_REGS
		     : REG_CLASS_FROM_CONSTRAINT ((unsigned char) *str, str));
		
		yara_assert (insn_class != NO_REGS);
		yara_assert
		  (!in_p
		   || (insn_data [(int) icode].operand [0].constraint [0]
		       == '='));
	      }
	    yara_assert
	      (insn_data [(int) icode].operand [2].constraint [0] == '='
	       && insn_data [(int) icode].operand [2].constraint [1] == '&');
	    str = &insn_data [(int) icode].operand [2].constraint [2];
	    if (class_subset_p [cl] [insn_class])
	      interm_mode = insn_data [(int) icode].operand [2].mode;
	    else
	      {
		interm_class = insn_class;
		scratch_class
		  = (*str == 'r' ? GENERAL_REGS
		     : REG_CLASS_FROM_CONSTRAINT ((unsigned char) *str,
						  str));
		scratch_mode = insn_data [(int) icode].operand [2].mode;
#ifdef SECONDARY_MEMORY_NEEDED
		if (! allocate_copy_secondary_memory (in_p, cp, -1,
						      interm_class, cl, mode))
		  gcc_unreachable ();
#endif
	      }
	  }
#ifdef ENABLE_YARA_CHECKING
	if (COPY_CHANGE_ADDR (cp) != NULL)
	  {
	    GO_IF_HARD_REG_EQUAL (COPY_INTERM_SCRATCH_HARD_REGSET (cp),
				  zero_hard_reg_set, ok);
	    gcc_unreachable ();
	  }
      ok:
#endif
	interm_hard_regno = scratch_hard_regno = -1;
	COPY_CHANGE_ADDR (cp) = get_free_copy_change ();
	if (interm_class != NO_REGS)
	  {
	    interm_hard_regno = find_hard_reg_for_mode (interm_class,
							interm_mode,
							prohibited_hard_regs);
	    if (interm_hard_regno < 0)
	      return false;
	    mark_regno_allocation (interm_hard_regno, interm_mode);
	    IOR_HARD_REG_SET
	      (COPY_INTERM_SCRATCH_HARD_REGSET (cp),
	       reg_mode_hard_regset [interm_hard_regno] [interm_mode]);
	    IOR_HARD_REG_SET
	      (prohibited_hard_regs,
	       reg_mode_hard_regset [interm_hard_regno] [interm_mode]);
	  }
	if (scratch_class != NO_REGS)
	  {
	    scratch_hard_regno = find_hard_reg_for_mode (scratch_class,
							 scratch_mode,
							 prohibited_hard_regs);
	    if (scratch_hard_regno < 0)
	      {
		if (interm_hard_regno >= 0)
		  mark_regno_release (interm_hard_regno, interm_mode);
		return false;
	      }
	    mark_regno_allocation (scratch_hard_regno, scratch_mode);
	    IOR_HARD_REG_SET
	      (COPY_INTERM_SCRATCH_HARD_REGSET (cp),
	       reg_mode_hard_regset [scratch_hard_regno] [scratch_mode]);
	  }
	COPY_ICODE (cp) = icode;
	COPY_INTERM_MODE (cp) = interm_mode;
	COPY_INTERM_REGNO (cp) = interm_hard_regno;
	COPY_SCRATCH_MODE (cp) = scratch_mode;
	COPY_SCRATCH_REGNO (cp) = scratch_hard_regno;
	COPY_IN_P (cp) = in_p;
	return true;
      }
  }
#endif
#ifdef SECONDARY_MEMORY_NEEDED
  hard_regno = (a2 == NULL ? ALLOCNO_REGNO (a) : ALLOCNO_HARD_REGNO (a2));
  yara_assert (hard_regno < 0 || HARD_REGISTER_NUM_P (hard_regno));
  if (! logged_p)
    log_copy (cp);
  if (! allocate_copy_secondary_memory (in_p, cp, hard_regno, cl,
					NO_REGS, mode))
    {
      yara_assert ((a != NULL && ALLOCNO_TYPE (a) == INSN_ALLOCNO
		    && INSN_ALLOCNO_ELIMINATION_P (a))
		   || (a2 != NULL && ALLOCNO_TYPE (a2) == INSN_ALLOCNO
		       && INSN_ALLOCNO_ELIMINATION_P (a2)));
      return false;
    }
#endif
  return true;
}

static bool
assign_allocno_copies (allocno_t a)
{
  copy_t cp;
  bool succ_p = true;

  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
    if (! (succ_p = assign_copy (cp)))
      break;
  if (succ_p)
    for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
      if (! (succ_p = assign_copy (cp)))
	break;
  if (succ_p)
    return true;
  /* Fail: restore the allocation state.  */
  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
    {
      if (COPY_CHANGE_ADDR (cp) == NULL)
	continue;
      log_copy (cp);
#ifdef SECONDARY_MEMORY_NEEDED
      if (COPY_MEMORY_SLOT (cp) != NULL)
	deallocate_copy_memory_slot (cp);
#endif
      COPY_CHANGE_ADDR (cp) = NULL;
    }
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    {
      if (COPY_CHANGE_ADDR (cp) == NULL)
	continue;
      log_copy (cp);
#ifdef SECONDARY_MEMORY_NEEDED
      if (COPY_MEMORY_SLOT (cp) != NULL)
	deallocate_copy_memory_slot (cp);
#endif
      COPY_CHANGE_ADDR (cp) = NULL;
    }
  return false;
}

void
unassign_copy (copy_t cp)
{
  if (COPY_CHANGE_ADDR (cp) == NULL)
    return;
  log_copy (cp);
#ifdef HAVE_SECONDARY_RELOADS
  if (COPY_INTERM_REGNO (cp) >= 0)
    mark_regno_release (COPY_INTERM_REGNO (cp), COPY_INTERM_MODE (cp));
  if (COPY_SCRATCH_REGNO (cp) >= 0)
    mark_regno_release (COPY_SCRATCH_REGNO (cp), COPY_SCRATCH_MODE (cp));
#endif
#ifdef SECONDARY_MEMORY_NEEDED
  if (COPY_MEMORY_SLOT (cp) != NULL)
    deallocate_copy_memory_slot (cp);
#endif
  if (COPY_SRC (cp) != NULL && ALLOCNO_USE_EQUIV_CONST_P (COPY_SRC (cp)))
    unassign_copy_interm_equiv_const_hard_reg (cp);
  free_copy_change (COPY_CHANGE_ADDR (cp));
  COPY_CHANGE_ADDR (cp) = NULL;
}

static void
unassign_allocno_copies (allocno_t a)
{
  copy_t cp;

  yara_assert (ALLOCNO_HARD_REGNO (a) >= 0 || ALLOCNO_MEMORY_SLOT (a) != NULL);
  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
    unassign_copy (cp);
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    unassign_copy (cp);
}

static bool
check_hard_regno_for_a (allocno_t a, int hard_regno,
			HARD_REG_SET possible_regs)
{
  int i, check_regno, conflict_hard_regno, start;
  int regno, reg_hard_regno, conflict_reg_hard_regno;
  enum machine_mode allocation_mode;
  allocno_t tied_a, conflict_a, check_a;
  copy_t cp;
  allocno_t *vec;
  copy_t *copy_vec;
  bool check_p;
  HARD_REG_SET prohibited_hard_regs;

  if (! HARD_REGNO_MODE_OK (hard_regno, ALLOCNO_MODE (a)))
    return false;
  allocation_mode = get_allocation_mode (a);
  start = get_maximal_part_start_hard_regno (hard_regno, a);
  if (start < 0)
    return false;
  if (! HARD_REGNO_MODE_OK (start, allocation_mode))
    return false;
  if (ALLOCNO_CALL_CROSS_P (a))
    {
      if (flag_ipra)
	COPY_HARD_REG_SET (prohibited_hard_regs, ALLOCNO_CLOBBERED_REGS (a));
      else
	COPY_HARD_REG_SET (prohibited_hard_regs, call_used_reg_set);
      IOR_HARD_REG_SET (prohibited_hard_regs, ALLOCNO_HARD_REG_CONFLICTS (a));
    }
  else
    COPY_HARD_REG_SET (prohibited_hard_regs, ALLOCNO_HARD_REG_CONFLICTS (a));
  if (! TEST_HARD_REG_BIT (no_alloc_regs, hard_regno))
    IOR_HARD_REG_SET (prohibited_hard_regs, no_alloc_regs);
  IOR_COMPL_HARD_REG_SET (prohibited_hard_regs, possible_regs);
  if (! hard_reg_not_in_set_p (start, allocation_mode, prohibited_hard_regs))
    return false;
  if (ALLOCNO_TYPE (a) == INSN_ALLOCNO)
    {
      tied_a = INSN_ALLOCNO_TIED_ALLOCNO (a);
      if (tied_a != NULL && (INSN_ALLOCNO_OP_MODE (tied_a) == OP_OUT
			     || INSN_ALLOCNO_OP_MODE (tied_a) == OP_INOUT))
	check_a = tied_a;
      else
	check_a = a;
    }
  else
    {
      check_a = a;
      tied_a = NULL;
    }
  check_regno = ALLOCNO_REGNO (check_a);
  vec = ALLOCNO_CONFLICT_VEC (a);
  reg_hard_regno = get_allocno_reg_hard_regno (a, hard_regno);
  regno = ALLOCNO_REGNO (a);
  check_p = regno >= 0 && ! HARD_REGISTER_NUM_P (regno);
  for (i = 0; (conflict_a = vec [i]) != NULL; i++)
    {
      if (conflict_a == tied_a)
	continue;
      COPY_HARD_REG_SET (prohibited_hard_regs,
			 ALLOCNO_HARD_REGSET (conflict_a));
      if (ALLOCNO_TYPE (conflict_a) == INSN_ALLOCNO)
	IOR_HARD_REG_SET (prohibited_hard_regs,
			  INSN_ALLOCNO_INTERM_ELIMINATION_REGSET (conflict_a));
      if (check_p
	  && regno == ALLOCNO_REGNO (conflict_a)
	  && (ALLOCNO_TYPE (a) != INSN_ALLOCNO
	      || ALLOCNO_TYPE (conflict_a) != INSN_ALLOCNO
	      || (! INSN_ALLOCNO_EARLY_CLOBBER (a)
		  && ! INSN_ALLOCNO_EARLY_CLOBBER (conflict_a)))
	  && (conflict_hard_regno = ALLOCNO_HARD_REGNO (conflict_a)) >= 0)
	{
	  conflict_reg_hard_regno
	    = get_allocno_reg_hard_regno (conflict_a, conflict_hard_regno);
	  if (conflict_reg_hard_regno != reg_hard_regno
	      && ! hard_reg_not_in_set_p (start, allocation_mode,
					  prohibited_hard_regs))
	    return false;
	}
      else
	{
	  if (! hard_reg_not_in_set_p (start, allocation_mode,
				       prohibited_hard_regs))
	    return false;
	}
    }
  copy_vec = ALLOCNO_COPY_CONFLICT_VEC (a);
  for (i = 0; (cp = copy_vec [i]) != NULL; i++)
    {
      if (COPY_CHANGE_ADDR (cp) == NULL)
	continue;
      if (! hard_reg_not_in_set_p (start, allocation_mode,
				   COPY_INTERM_SCRATCH_HARD_REGSET (cp)))
	return false;
    }
  return true;
}

static int possible_hard_regnos [FIRST_PSEUDO_REGISTER];
static int possible_hard_regnos_num;

static bool
collect_conflict_hard_regs (allocno_t a, HARD_REG_SET *prohibited_hard_regs)
{
  int i, j, regno, hard_regno = 0, conflict_hard_regno, temp_regno, start;
  enum machine_mode allocation_mode;
  allocno_t tied_a, conflict_a;
  copy_t cp;
  HARD_REG_SET conflict_set, temp_set;
  allocno_t *vec;
  copy_t *copy_vec;
  bool check_p;

  COPY_HARD_REG_SET (conflict_set, ALLOCNO_HARD_REG_CONFLICTS (a));
  IOR_HARD_REG_SET (conflict_set, *prohibited_hard_regs);
  tied_a = (ALLOCNO_TYPE (a) == INSN_ALLOCNO
	    ? INSN_ALLOCNO_TIED_ALLOCNO (a) : NULL);
  vec = ALLOCNO_CONFLICT_VEC (a);
  possible_hard_regnos_num = 0;
  allocation_mode = get_allocation_mode (a);
  regno = ALLOCNO_REGNO (a);
  check_p = regno >= 0 && ! HARD_REGISTER_NUM_P (regno);
  for (i = 0; (conflict_a = vec [i]) != NULL; i++)
    {
      if (conflict_a == tied_a)
	continue;
      COPY_HARD_REG_SET (temp_set, ALLOCNO_HARD_REGSET (conflict_a));
      if (ALLOCNO_TYPE (conflict_a) == INSN_ALLOCNO)
	IOR_HARD_REG_SET (temp_set,
			  INSN_ALLOCNO_INTERM_ELIMINATION_REGSET (conflict_a));
      hard_regno = -1;
      if (check_p
	  && regno == ALLOCNO_REGNO (conflict_a)
	  && (conflict_hard_regno = ALLOCNO_HARD_REGNO (conflict_a)) >= 0)
	{
	  conflict_hard_regno
	    = get_allocno_reg_hard_regno (conflict_a, conflict_hard_regno);
	  if (conflict_hard_regno >= 0)
	    {
	      hard_regno = get_allocno_hard_regno (a, conflict_hard_regno);
	      if (hard_regno >= 0)
		{
		  for (j = 0; j < possible_hard_regnos_num; j++)
		    if (possible_hard_regnos [j] == hard_regno)
		      break;
		  if (j == possible_hard_regnos_num)
		    {
		      start
			= get_maximal_part_start_hard_regno (hard_regno, a);
		      if (start >= 0
			  && hard_reg_not_in_set_p (start, allocation_mode,
						    conflict_set))
			possible_hard_regnos [possible_hard_regnos_num++]
			  = hard_regno;
		    }
		}
	    }
	}
      IOR_HARD_REG_SET (conflict_set, temp_set);
      for (j = 0; j < possible_hard_regnos_num; j++)
	{
	  temp_regno = possible_hard_regnos [j];
	  if (temp_regno == hard_regno)
	    continue;
	  start = get_maximal_part_start_hard_regno (temp_regno, a);
	  yara_assert (start >= 0);
	  if (! hard_reg_not_in_set_p (start, allocation_mode, conflict_set))
	    {
	      possible_hard_regnos [j]
		= possible_hard_regnos [possible_hard_regnos_num - 1];
	      possible_hard_regnos_num--;
	      j--;
	    }
	}
      GO_IF_HARD_REG_EQUAL (conflict_set, one_hard_reg_set, ok);
      continue;
    ok:
      if (possible_hard_regnos_num == 0)
	return false;
    }
  CLEAR_HARD_REG_SET (temp_set);
  copy_vec = ALLOCNO_COPY_CONFLICT_VEC (a);
  for (i = 0; (cp = copy_vec [i]) != NULL; i++)
    {
      if (COPY_CHANGE_ADDR (cp) == NULL)
	continue;
      IOR_HARD_REG_SET (temp_set, COPY_INTERM_SCRATCH_HARD_REGSET (cp));
    }
  for (j = 0; j < possible_hard_regnos_num; j++)
    {
      temp_regno = possible_hard_regnos [j];
      if (temp_regno == hard_regno)
	continue;
      start = get_maximal_part_start_hard_regno (temp_regno, a);
      if (! hard_reg_not_in_set_p (start, allocation_mode, temp_set))
	{
	  possible_hard_regnos [j]
	    = possible_hard_regnos [possible_hard_regnos_num - 1];
	  possible_hard_regnos_num--;
	  j--;
	}
    }
  IOR_HARD_REG_SET (conflict_set, temp_set);
  COPY_HARD_REG_SET (*prohibited_hard_regs, conflict_set);
  return true;
}

/* Return mode for allocated hard reg for A.  */
enum machine_mode
get_allocation_mode (allocno_t a)
{
  if (ALLOCNO_TYPE (a) != INSN_ALLOCNO)
    return ALLOCNO_MODE (a);
  return INSN_ALLOCNO_BIGGEST_MODE (a);
}

/* We know that allocno register got HARD_REGNO (allocno may contain
   subregister of the register).  Return hard_regno of the
   allocno.  */
int
get_allocno_hard_regno (allocno_t a, int hard_regno)
{
  rtx x;
  enum machine_mode smode, rmode;

  yara_assert (hard_regno >= 0);
  if (ALLOCNO_TYPE (a) != INSN_ALLOCNO)
    return hard_regno;
  SKIP_TO_SUBREG (x, *INSN_ALLOCNO_LOC (a));
  if (GET_CODE (x) != SUBREG)
    return hard_regno;
  smode = GET_MODE (x);
  yara_assert (GET_MODE (x) == ALLOCNO_MODE (a));
  rmode = GET_MODE (SUBREG_REG (x));
  return (hard_regno
	  + (int) subreg_regno_offset (hard_regno, rmode,
				       SUBREG_BYTE (x), smode));
}

/* This function is opposite the previous one.  We know that allocno
   got A_HARD_REGNO (allocno may contain subregister of the register).
   Return hard_regno of the cooresponding register.  */
int
get_allocno_reg_hard_regno (allocno_t a, int a_hard_regno)
{
  rtx x;
  enum machine_mode smode, rmode;

  yara_assert (a_hard_regno >= 0);
  if (ALLOCNO_TYPE (a) != INSN_ALLOCNO)
    return a_hard_regno;
  SKIP_TO_SUBREG (x, *INSN_ALLOCNO_LOC (a));
  if (GET_CODE (x) != SUBREG)
    return a_hard_regno;
  smode = GET_MODE (x);
  yara_assert (GET_MODE (x) == ALLOCNO_MODE (a));
  rmode = GET_MODE (SUBREG_REG (x));
  return (a_hard_regno
	  - (int) subreg_regno_offset (a_hard_regno, rmode,
				       SUBREG_BYTE (x), smode));
}

/* Return start hard regno of maximal (allocated) part.  */
int
get_maximal_part_start_hard_regno (int hard_regno, allocno_t a)
{
  rtx container;
  enum machine_mode smode, rmode;

  yara_assert (hard_regno >= 0);
  if (ALLOCNO_TYPE (a) != INSN_ALLOCNO)
    return hard_regno;
  container = *INSN_ALLOCNO_CONTAINER_LOC (a);
  if (GET_CODE (container) != SUBREG)
    return hard_regno;
  smode = GET_MODE (container);
  rmode = ALLOCNO_MODE (a);
  if (GET_MODE_SIZE (smode) > GET_MODE_SIZE (rmode))
    hard_regno += (int) subreg_regno_offset (hard_regno, rmode,
					     SUBREG_BYTE (container), smode);
  yara_assert (hard_regno >= 0);
  return hard_regno;
}

bool
check_insns_added_since (rtx last)
{
  rtx since, insn;

  if (last == NULL_RTX)
    since = get_insns ();
  else
    since = NEXT_INSN (last);
  for (insn = since; insn != NULL_RTX; insn = NEXT_INSN (insn))
    if (recog_memoized (insn) < 0)
      break;
    else
      {
	extract_insn (insn);
	/* It might be memory in which pseudo-registers are not
	   changed by hard registers yet.  ??? Strict -- we need implement
	   substitution of all pseudos in memory.  */
	if (! constrain_operands (1))
	  break;
      }
  return insn == NULL_RTX;
}

/* HARD_REGNO should be tried as hard regno for A.  */
static bool
assign_allocno_hard_regno (allocno_t a, int hard_regno,
			   HARD_REG_SET possible_regs)
{
  int start;
  enum machine_mode allocation_mode;

  yara_assert (hard_regno >= 0 && ALLOCNO_HARD_REGNO (a) < 0);
  if (ALLOCNO_REGNO (a) == hard_regno)
    {
      rtx x;

      yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
      SKIP_TO_SUBREG (x, *INSN_ALLOCNO_LOC (a));
      if (REG_P (x))
	{
	  log_allocno (a);
	  INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a) = true;
	  global_allocation_cost += allocno_copy_cost (a);
	  yara_assert (assign_allocno_copies (a));
	  return true;
	}
    }
  if (! check_hard_regno_for_a (a, hard_regno, possible_regs))
    return false;
  log_allocno (a);
  ALLOCNO_HARD_REGNO (a) = hard_regno;
  if (! assign_allocno_copies (a))
    {
      ALLOCNO_HARD_REGNO (a) = -1;
      return false;
    }
  allocation_mode = get_allocation_mode (a);
  start = get_maximal_part_start_hard_regno (hard_regno, a);
  yara_assert (start >= 0);
  mark_regno_allocation (start, allocation_mode);
  IOR_HARD_REG_SET
    (ALLOCNO_HARD_REGSET (a), reg_mode_hard_regset [start] [allocation_mode]);
  global_allocation_cost += allocno_copy_cost (a);
  return true;
}

static bool
assign_one_allocno (allocno_t a, enum reg_class cl, HARD_REG_SET possible_regs)
{
  int hard_regno, start;
  HARD_REG_SET prohibited_hard_regs;
  enum machine_mode allocation_mode;

  if (cl == LIM_REG_CLASSES)
    {
      rtx equiv_const = (ALLOCNO_REGNO (a) >= 0
			 ? reg_equiv_constant [ALLOCNO_REGNO (a)] : NULL_RTX);

      yara_assert (equiv_const != NULL_RTX
		   || (ALLOCNO_TYPE (a) == INSN_ALLOCNO
		       && (ALLOCNO_REGNO (a) < 0
			   || HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a)))));
      log_allocno (a);
      if (equiv_const == NULL_RTX)
	INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a) = true;
      else
	ALLOCNO_USE_EQUIV_CONST_P (a) = true;
      global_allocation_cost += allocno_copy_cost (a);
      if (! assign_allocno_copies (a))
	{
	  if (equiv_const == NULL_RTX)
	    INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a) = false;
	  else
	    ALLOCNO_USE_EQUIV_CONST_P (a) = false;
	  return false;
	}
      return true;
    }
  else if (cl == NO_REGS)
    {
      if (ALLOCNO_TYPE (a) == INSN_ALLOCNO && INSN_ALLOCNO_ELIMINATION_P (a))
	/* We never allocate memory for allocnos containing eliminated
	   regs.  */
	return false;
      log_allocno (a);
      if (ALLOCNO_REGNO (a) >= 0)
	allocate_allocno_memory_slot (a);
      else
	{
	  yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO
		       && CONST_POOL_OK_P (*INSN_ALLOCNO_LOC (a)));
	  /* ??? Implement elimination register if the address is not
	     legitimate.  */
	  INSN_ALLOCNO_CONST_POOL_P (a) = true;
	}
      global_allocation_cost += allocno_copy_cost (a);
      if (! assign_allocno_copies (a))
	{
	  if (ALLOCNO_MEMORY_SLOT (a) != NULL)
	    deallocate_allocno_memory_slot (a);
	  else
	    INSN_ALLOCNO_CONST_POOL_P (a) = false;
	  return false;
	}
      return true;
    }
  yara_assert (ALLOCNO_HARD_REGNO (a) < 0);
#ifdef ENABLE_YARA_CHECKING
  GO_IF_HARD_REG_SUBSET (possible_regs, reg_class_contents [cl], ok);
  gcc_unreachable ();
 ok:
#endif
  if (ALLOCNO_CALL_CROSS_P (a))
    {
      if (flag_ipra)
	COPY_HARD_REG_SET (prohibited_hard_regs, ALLOCNO_CLOBBERED_REGS (a));
      else
	COPY_HARD_REG_SET (prohibited_hard_regs, call_used_reg_set);
    }
  else
    CLEAR_HARD_REG_SET (prohibited_hard_regs);
  IOR_COMPL_HARD_REG_SET (prohibited_hard_regs, possible_regs);
  if (! collect_conflict_hard_regs (a, &prohibited_hard_regs))
    return false;
  hard_regno = find_hard_reg (a, cl, prohibited_hard_regs,
			      possible_hard_regnos, possible_hard_regnos_num);
  if (hard_regno < 0)
    return false;
  log_allocno (a);
  ALLOCNO_HARD_REGNO (a) = hard_regno;
  if (! assign_allocno_copies (a))
    {
      ALLOCNO_HARD_REGNO (a) = -1;
      return false;
    }
  allocation_mode = get_allocation_mode (a);
  start = get_maximal_part_start_hard_regno (hard_regno, a);
  yara_assert (start >= 0);
  mark_regno_allocation (start, allocation_mode);
  IOR_HARD_REG_SET
    (ALLOCNO_HARD_REGSET (a), reg_mode_hard_regset [start] [allocation_mode]);
  global_allocation_cost += allocno_copy_cost (a);
  return true;
}

static void
unassign_one_allocno (allocno_t a)
{
  log_allocno (a);
  global_allocation_cost -= allocno_copy_cost (a);
  if (ALLOCNO_HARD_REGNO (a) >= 0)
    {
      unassign_allocno_copies (a);
      mark_regno_release
	(get_maximal_part_start_hard_regno (ALLOCNO_HARD_REGNO (a), a),
	 get_allocation_mode (a));
      ALLOCNO_HARD_REGNO (a) = -1;
      CLEAR_HARD_REG_SET (ALLOCNO_HARD_REGSET (a));
    }
  else if (ALLOCNO_MEMORY_SLOT (a) != NULL)
    {
      unassign_allocno_copies (a);
      deallocate_allocno_memory_slot (a);
    }
  else if (ALLOCNO_USE_EQUIV_CONST_P (a))
    {
      unassign_allocno_copies (a);
      ALLOCNO_USE_EQUIV_CONST_P (a) = false;
    }
  else if (ALLOCNO_TYPE (a) == INSN_ALLOCNO)
    {
      if (INSN_ALLOCNO_CONST_POOL_P (a))
	INSN_ALLOCNO_CONST_POOL_P (a) = false;
      else if (INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a))
	INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a) = false;
      else
	gcc_unreachable ();
    }
  else
    gcc_unreachable ();
}

/* Allocate A1 than A2.  */
static bool
assign_allocno_pair (allocno_t a1, allocno_t a2, enum reg_class cl,
		     HARD_REG_SET possible_regs, int start)
{
  int regno;

  yara_assert (a1 == INSN_ALLOCNO_TIED_ALLOCNO (a2)
	       && a2 == INSN_ALLOCNO_TIED_ALLOCNO (a1));
  if (cl == LIM_REG_CLASSES)
    {
      yara_assert (start < 0);
      regno = ALLOCNO_REGNO (a1);
      if (regno >= 0 && HARD_REGISTER_NUM_P (regno))
	{
	  yara_assert (REG_P (*INSN_ALLOCNO_LOC (a1)));
	  if (! assign_one_allocno (a1, cl, possible_regs))
	    gcc_unreachable ();
	  if (! assign_allocno_hard_regno (a2, regno, possible_regs))
	    {
	      unassign_one_allocno (a1);
	      return false;
	    }
	  return true;
	}
      if (! rtx_equal_p (*INSN_ALLOCNO_LOC (a1), *INSN_ALLOCNO_LOC (a2)))
	return false;
      if (! assign_one_allocno (a1, cl, possible_regs))
	gcc_unreachable ();
      if (! assign_one_allocno (a2, cl, possible_regs))
	gcc_unreachable ();
      return true;
    }
  else if (cl == NO_REGS)
    {
      yara_assert (start < 0);
      /* We don't want move memory into memory because it needs
	 additional register (but if the insn had identical memory as
	 the two operands than they can still use them).  So we
	 believe that target machine has insn with duplications which
	 works on registers.  */
      /* ??? Use memory slots */
      if (ALLOCNO_CAN (a1) == NULL || ALLOCNO_CAN (a1) != ALLOCNO_CAN (a2))
	return false;
      if (ALLOCNO_MEMORY_SLOT_OFFSET (a1) != ALLOCNO_MEMORY_SLOT_OFFSET (a2))
	return false;
      if (! assign_one_allocno (a1, cl, possible_regs))
	gcc_unreachable ();
      if (! assign_one_allocno (a2, cl, possible_regs))
	gcc_unreachable ();
      return true;
    }
  if ((start < 0 && ! assign_one_allocno (a1, cl, possible_regs))
      || (start >= 0
	  && ! assign_allocno_hard_regno (a1, start, possible_regs)))
    return false;
  if (INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a1))
    start = ALLOCNO_REGNO (a1);
  else
    start = ALLOCNO_HARD_REGNO (a1);
  yara_assert (start >= 0);
  if (! assign_allocno_hard_regno (a2, start, possible_regs))
    {
      unassign_one_allocno (a1);
      return false;
    }
  yara_assert (ALLOCNO_CAN (a1) == ALLOCNO_CAN (a2)
	       || (ALLOCNO_TYPE (a1) == INSN_ALLOCNO
		   && ALLOCNO_TYPE (a2) == INSN_ALLOCNO));
  return true;
}

bool
assign_allocno (allocno_t a, enum reg_class cl, HARD_REG_SET possible_regs,
		int start)
{
  allocno_t another_a, tmp;

  if (ALLOCNO_TYPE (a) != INSN_ALLOCNO
      || (another_a = INSN_ALLOCNO_TIED_ALLOCNO (a)) == NULL)
    return (start < 0 ? assign_one_allocno (a, cl, possible_regs)
	    : assign_allocno_hard_regno (a, start, possible_regs));
  if (INSN_ALLOCNO_ORIGINAL_P (a))
    {
      tmp = a;
      a = another_a;
      another_a = tmp;
    }
  if (assign_allocno_pair (a, another_a, cl, possible_regs, start))
    return true;
  return assign_allocno_pair (another_a, a, cl, possible_regs, start);
}

bool
assign_elimination_reg (allocno_t a, enum reg_class cl,
			HARD_REG_SET possible_regs, int hard_regno)
{
  HARD_REG_SET prohibited_hard_regs;

  yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
#ifdef ENABLE_YARA_CHECKING
  GO_IF_HARD_REG_SUBSET (possible_regs, reg_class_contents [cl], ok);
  gcc_unreachable ();
 ok:
#endif
  CLEAR_HARD_REG_SET (prohibited_hard_regs);
  IOR_COMPL_HARD_REG_SET (prohibited_hard_regs, possible_regs);
  if (! collect_conflict_hard_regs (a, &prohibited_hard_regs))
    return false;
  /* We set up possible_hard_regnos only for pseudo-registers.  */
  yara_assert (possible_hard_regnos_num == 0);
  /* We assume that eliminated registers are not in subregisters.
     Otherwise we could use function get_allocation_mode.  */
  yara_assert (GET_CODE (*INSN_ALLOCNO_CONTAINER_LOC (a)) != SUBREG
	       && REG_P (*INSN_ALLOCNO_LOC (a)));
  if (hard_regno < 0)
    hard_regno = find_hard_reg (a, cl, prohibited_hard_regs, NULL, 0);
  else
    {
      if (! check_hard_reg (hard_regno, a, prohibited_hard_regs, false))
	hard_regno = -1;
      else
	{
	  yara_assert (hard_reg_in_set_p (hard_regno, ALLOCNO_MODE (a),
					  reg_class_contents [cl]));
	}
    }
  if (hard_regno < 0)
    return false;
  log_allocno (a);
  /* ??? Is it right to use BASE_REGS as the class of eliminated
     register (it can be a virtual register).
     ??? Is it right to make  addition cost to register move cost. */
  global_allocation_cost
    += COST_FACTOR * register_move_cost [Pmode] [BASE_REG_CLASS] [cl];
  INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (a) = hard_regno;
  mark_regno_allocation (hard_regno, Pmode);
  IOR_HARD_REG_SET (INSN_ALLOCNO_INTERM_ELIMINATION_REGSET (a),
		    reg_mode_hard_regset [hard_regno] [Pmode]);
  return true;
}

void
create_tie (allocno_t original, allocno_t duplicate)
{
  yara_assert (ALLOCNO_TYPE (original) == INSN_ALLOCNO
	       && ALLOCNO_TYPE (duplicate) == INSN_ALLOCNO
	       && INSN_ALLOCNO_TIED_ALLOCNO (original) == NULL
	       && INSN_ALLOCNO_TIED_ALLOCNO (duplicate) == NULL);
  log_allocno (original);
  log_allocno (duplicate);
  INSN_ALLOCNO_TIED_ALLOCNO (duplicate) = original;
  INSN_ALLOCNO_ORIGINAL_P (duplicate) = false;
  INSN_ALLOCNO_TIED_ALLOCNO (original) = duplicate;
  INSN_ALLOCNO_ORIGINAL_P (original) = true;
}

void
break_tie (allocno_t a)
{
  allocno_t another_a;

  yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
  if ((another_a = INSN_ALLOCNO_TIED_ALLOCNO (a)) != NULL)
    {
      log_allocno (a);
      log_allocno (another_a);
      INSN_ALLOCNO_TIED_ALLOCNO (a) = NULL;
      INSN_ALLOCNO_ORIGINAL_P (a) = false;
      INSN_ALLOCNO_TIED_ALLOCNO (another_a) = NULL;
      INSN_ALLOCNO_ORIGINAL_P (another_a) = false;
    }
}

void
unassign_allocno (allocno_t a)
{
  allocno_t another_a;

  unassign_one_allocno (a);
  if (ALLOCNO_TYPE (a) == INSN_ALLOCNO
      && (another_a = INSN_ALLOCNO_TIED_ALLOCNO (a)) != NULL)
    {
      yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO
		   && ALLOCNO_TYPE (another_a) == INSN_ALLOCNO);
      unassign_one_allocno (another_a);
    }
}

bool
memory_slot_intersected (struct memory_slot *slot1, struct memory_slot *slot2)
{
  int start1, start2;

  if (slot1 == NULL || slot2 == NULL
      || slot1->mem != NULL_RTX || slot2->mem != NULL_RTX)
    return false;
#ifdef FRAME_GROWS_DOWNWARD
  start1 = slot1->start - slot1->size + 1;
  start2 = slot2->start - slot2->size + 1;
#else
  start1 = slot1->start;
  start2 = slot2->start;
#endif
  if (start1 <= start2)
    return start2 < start1 + slot1->size;
  else
    return start1 < start2 + slot2->size;
}



enum reg_class
smallest_superset_class (HARD_REG_SET set)
{
  int cl, result;
  HARD_REG_SET super_set;
  
  result = ALL_REGS;
  COPY_HARD_REG_SET (super_set, reg_class_contents [ALL_REGS]);
  for (cl = (int) N_REG_CLASSES - 1; cl >= 0; cl--)
    {
      GO_IF_HARD_REG_SUBSET (reg_class_contents [cl], super_set, smaller);
      continue;
    smaller:
      GO_IF_HARD_REG_SUBSET (set, reg_class_contents [cl], ok);
      continue;
    ok:
      COPY_HARD_REG_SET (super_set, reg_class_contents [cl]);
      result = cl;
    }
  return result;
}

static void
possible_alt_reg_intersection (allocno_t a, HARD_REG_SET *regs)
{
  int op_num, n_alt, c;
  const char *constraints;
  struct insn_op_info *info;
  HARD_REG_SET alt_regs;

  yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
  yara_assert (op_num >= 0);
  info = insn_infos [INSN_UID (INSN_ALLOCNO_INSN (a))];
  COPY_HARD_REG_SET (*regs, reg_class_contents [ALL_REGS]);
  for (n_alt = 0; n_alt < info->n_alts; n_alt++)
    {
      if (! TEST_ALT (INSN_ALLOCNO_POSSIBLE_ALTS (a), n_alt))
	continue;
      constraints = info->op_constraints [op_num * info->n_alts + n_alt];
      if (constraints != NULL && *constraints != '\0')
	{
	  COPY_HARD_REG_SET (alt_regs, reg_class_contents [NO_REGS]);
	  for (;
	       (c = *constraints);
	       constraints += CONSTRAINT_LEN (c, constraints))
	    {
	      if (c == '*')
		{
		  constraints += CONSTRAINT_LEN (c, constraints);
		  continue;
		}
	      else if (c == ' ' || c == '\t' || c == '=' || c == '+'
		       || c == '*' || c == '&' || c == '%' || c == '?'
		       || c == '!')
		continue;
	      
	      switch (c)
		{
		case '\0':
		case 'X':
		  break;
		  
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8':	case '9':
		  break;

		case 'i': case 'n': case 's': case 'I':	case 'J':
		case 'K': case 'L': case 'M': case 'N':	case 'O':
		case 'P': case 'E': case 'F': case 'G':	case 'H':
		  /* constants -- ignore */
		  break;

		case 'm': case 'o': case 'V': case '<':	case '>':
		  /* memory -- ignore */
		  break;

		case 'p':
		  IOR_HARD_REG_SET (alt_regs,
				    reg_class_contents [BASE_REG_CLASS]);
		  break;
		  
		case 'g':
		  IOR_HARD_REG_SET (alt_regs, reg_class_contents [ALL_REGS]);
		  break;
		  
		case 'r':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		case 'h': case 'j': case 'k': case 'l':
		case 'q': case 't': case 'u':
		case 'v': case 'w': case 'x': case 'y': case 'z':
		case 'A': case 'B': case 'C': case 'D':
		case 'Q': case 'R': case 'S': case 'T': case 'U':
		case 'W': case 'Y': case 'Z':
		  {
		    enum reg_class cl;
		    
		    cl = (c == 'r'
			  ? GENERAL_REGS
			  : REG_CLASS_FROM_CONSTRAINT (c, constraints));
		    IOR_HARD_REG_SET (alt_regs, reg_class_contents [cl]);
		    break;
		  }
		  
		default:
		  gcc_unreachable ();
		}
	    }
	  GO_IF_HARD_REG_EQUAL (alt_regs, reg_class_contents [NO_REGS], skip);
	  AND_HARD_REG_SET (*regs, alt_regs);
	skip:
	  ;
	}
    }
}

static bool
all_alt_offset_ok_p (allocno_t a, HOST_WIDE_INT val)
{
  int op_num, n_alt, c;
  struct insn_op_info *info;
  const char *constraints;
  bool const_p, in_range_p;

  yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
  yara_assert (op_num >= 0);
  info = insn_infos [INSN_UID (INSN_ALLOCNO_INSN (a))];
  for (n_alt = 0; n_alt < info->n_alts; n_alt++)
    {
      if (! TEST_ALT (INSN_ALLOCNO_POSSIBLE_ALTS (a), n_alt))
	continue;
      constraints = info->op_constraints [op_num * info->n_alts + n_alt];
      if (constraints != NULL && *constraints != '\0')
	{
	  const_p = false;
	  in_range_p = false;
	  for (;
	       (c = *constraints);
	       constraints += CONSTRAINT_LEN (c, constraints))
	    {
	      if (c == ' ' || c == '\t' || c == '=' || c == '+'
		  || c == '*' || c == '&' || c == '%' || c == '?'
		  || c == '!')
		continue;
	      
	      switch (c)
		{
		case '\0':
		case 'X':
		  break;
		  
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8':	case '9':
		  /* Ignore because it should be memory or
		     register.  */
		  break;

		case 'i': case 'n': case 's':
		  /* generic constants -- ignore */
		  break;

		case 'I': case 'J': case 'K': case 'L':
		case 'M': case 'N': case 'O': case 'P':
		  const_p = true;
		  in_range_p = CONST_OK_FOR_CONSTRAINT_P (val, c, constraints);
		  break;

		case 'E': case 'F': case 'G': case 'H':
		  /* floating point constants -- ignore */
		  break;

		case 'm': case 'o': case 'V': case '<':	case '>':
		  /* memory -- ignore */
		  break;

		case 'p':
		  /* address - ignore  */
		  break;
		  
		case 'g': case 'r':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		case 'h': case 'j': case 'k': case 'l':
		case 'q': case 't': case 'u':
		case 'v': case 'w': case 'x': case 'y': case 'z':
		case 'A': case 'B': case 'C': case 'D':
		case 'Q': case 'R': case 'S': case 'T': case 'U':
		case 'W': case 'Y': case 'Z':
		  /* register -- ignore  */
		  break;
		  
		default:
		  gcc_unreachable ();
		}
	    }
	  if (const_p && ! in_range_p)
	    return false;
	}
    }
  return true;
}

static int (*provide_allocno_elimination_class_hard_reg_func) (allocno_t,
							       enum reg_class,
							       HARD_REG_SET);

static bool
find_interm_elimination_reg (allocno_t a, enum reg_class cl,
			     HARD_REG_SET possible_regs)
{
  int interm_elimination_regno, hard_regno;
  
  /* We assume that eliminated registers are not in subregisters.
     Otherwise we could use function get_allocation_mode.  */
  yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO
	       && GET_CODE (*INSN_ALLOCNO_CONTAINER_LOC (a)) != SUBREG
	       && REG_P (*INSN_ALLOCNO_LOC (a)));
  if ((interm_elimination_regno = ALLOCNO_HARD_REGNO (a)) >= 0
      && mode_size [ALLOCNO_MODE (a)] >= mode_size [Pmode]
      && hard_reg_in_set_p (interm_elimination_regno, Pmode, possible_regs))
    {
      INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (a) = interm_elimination_regno;
      mark_regno_allocation (interm_elimination_regno, Pmode);
      IOR_HARD_REG_SET
	(INSN_ALLOCNO_INTERM_ELIMINATION_REGSET (a),
	 reg_mode_hard_regset [interm_elimination_regno] [Pmode]);
      return true;
    }
  if (assign_elimination_reg (a, cl, possible_regs, -1))
    return true;
  if (provide_allocno_elimination_class_hard_reg_func == NULL)
    return false;
  hard_regno
    = (*provide_allocno_elimination_class_hard_reg_func) (a, cl,
							  possible_regs);
  return hard_regno >= 0;
}

/* The function checks that address *ADDRESS_LOC of CONTAINER_LOC can
   be eliminated and return elimination info and what should be
   eliminated through BASE_REG_P.  The function do substitution of
   register CHANGE in the address by SUBST_REGNO before the check.
   ADDITION is an additional offset after the elimination.  */
struct reg_eliminate *
check_elimination_in_addr (int change, int subst_regno, HOST_WIDE_INT addition,
			   rtx *address_loc, rtx *container_loc, bool *base_p)
{
  bool ok_p;
  int base_regno, index_regno, saved_regno;
  HOST_WIDE_INT offset, scale;
  rtx *base_reg_loc, *disp_loc, *index_reg_loc, *temp_container_loc;
  rtx addr, new_disp, saved_disp;
  struct reg_eliminate *elim;
  enum machine_mode mode;
 
  temp_container_loc = container_loc;
  if (! decode_address (address_loc, &temp_container_loc, &base_reg_loc,
			&disp_loc, &index_reg_loc, &scale, true))
    gcc_unreachable ();
  yara_assert (temp_container_loc == container_loc);
  base_regno = index_regno = -1;
  if (base_reg_loc != NULL)
    {
      base_regno = REGNO (*base_reg_loc);
      if (base_regno == change)
	base_regno = subst_regno;
    }
  if (index_reg_loc != NULL)
    {
      index_regno = REGNO (*index_reg_loc);
      if (index_regno == change)
	index_regno = subst_regno;
    }
  yara_assert (base_regno >= 0 || index_regno >= 0);
  if (base_regno >= 0 && HARD_REGISTER_NUM_P (base_regno)
      && reg_eliminate [base_regno] != NULL)
    {
      yara_assert (index_regno < 0 || ! HARD_REGISTER_NUM_P (index_regno)
		   || reg_eliminate [index_regno] == NULL);
      *base_p = true;
    }
  else
    {
      yara_assert (index_regno >= 0 && HARD_REGISTER_NUM_P (index_regno)
		   && reg_eliminate [index_regno] != NULL);
      yara_assert (base_regno < 0 || ! HARD_REGISTER_NUM_P (base_regno)
		   || reg_eliminate [base_regno] == NULL);
      *base_p = false;
    }
  mode = (GET_CODE (*container_loc) == MEM
	  ? GET_MODE (*container_loc) : VOIDmode);
  for (elim = reg_eliminate [*base_p ? base_regno : index_regno];
       elim != NULL;
       elim = elim->next)
    {
      offset = elim->offset + addition;
      if (elim->to == STACK_POINTER_REGNUM)
	offset += slot_memory_size;
      if (*base_p)
	{
	  saved_regno = REGNO (*base_reg_loc);
	  REGNO (*base_reg_loc) = elim->to;
	}
      else
	{
	  saved_regno = REGNO (*index_reg_loc);
	  REGNO (*index_reg_loc) = elim->to;
	  offset *= scale;
	}
      new_disp
	= get_temp_disp ((disp_loc == NULL ? NULL_RTX : *disp_loc), offset);
      if (disp_loc == NULL)
	{
	  saved_disp = NULL_RTX;
	  if (new_disp == NULL)
	    addr = *address_loc;
	  else
	    {
	      XEXP (temp_plus, 0) = *address_loc;
	      XEXP (temp_plus, 1) = new_disp;
	      addr = temp_plus;
	    }
	}
      else
	{
	  saved_disp = *disp_loc;
	  *disp_loc = new_disp;
	  addr = *address_loc;
	}
      ok_p = true;
      GO_IF_LEGITIMATE_ADDRESS (mode, addr, ok);
      ok_p = false;
    ok:
      if (*base_p)
	REGNO (*base_reg_loc) = saved_regno;
      else
	REGNO (*index_reg_loc) = saved_regno;
      if (saved_disp != NULL_RTX)
	*disp_loc = saved_disp;
      if (ok_p)
	return elim;
    }
  return NULL;
}

bool
eliminate_reg (allocno_t a)
{
  int regno = ALLOCNO_REGNO (a);
  allocno_t disp_a;
  HOST_WIDE_INT offset;
  rtx *container_loc;
  struct reg_eliminate *elim;
  bool base_p, ok_p;
  HARD_REG_SET possible_regs;
  enum reg_class cl;

  container_loc = INSN_ALLOCNO_CONTAINER_LOC (a);
  yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO
	       /* We assume that eliminated registers are not in
		  subregisters.  Otherwise we could use function
		  get_allocation_mode.  */
	       && GET_CODE (*container_loc) != SUBREG
	       && REG_P (*INSN_ALLOCNO_LOC (a))
	       && mode_size [Pmode] >= mode_size [ALLOCNO_MODE (a)]
	       && INSN_ALLOCNO_OP_MODE (a) == OP_IN
	       && (regno < 0 || (HARD_REGISTER_NUM_P (regno)
				 && reg_eliminate [regno] != NULL)));
  if (regno < 0)
    {
      /* It is a non-register allocno.  */
      base_p = true;
      elim = check_elimination_in_addr (-1, -1, 0, INSN_ALLOCNO_LOC (a),
					container_loc, &base_p);
      possible_alt_reg_intersection (a, &possible_regs);
      if (base_p)
	{
	  GO_IF_HARD_REG_SUBSET (reg_class_contents [BASE_REG_CLASS],
				 possible_regs,  ok);
	}
      else
	{
	  GO_IF_HARD_REG_SUBSET (reg_class_contents [INDEX_REG_CLASS],
				 possible_regs,  ok);
	}
      goto skip;
    ok:
      if (elim != NULL)
	{
	  INSN_ALLOCNO_ELIMINATION (a) = elim;
	  return true;
	}
    skip:
      cl = smallest_superset_class (possible_regs);
      ok_p = find_interm_elimination_reg (a, cl, possible_regs);
    }
  else if (INSN_ALLOCNO_TYPE (a) == BASE_REG
	   || INSN_ALLOCNO_TYPE (a) == INDEX_REG)
    {
      rtx *address_loc;

      address_loc = (GET_CODE (*container_loc) == MEM
		     ? &XEXP (*container_loc, 0) : container_loc);
      elim = check_elimination_in_addr (-1, -1, 0,
					address_loc, container_loc, &base_p);
      yara_assert ((base_p && INSN_ALLOCNO_TYPE (a) == BASE_REG)
		   || (! base_p && INSN_ALLOCNO_TYPE (a) == INDEX_REG));
      if (elim != NULL)
	{
	  INSN_ALLOCNO_ELIMINATION (a) = elim;
	  return true;
	}
      if (base_p)
	{
	  enum machine_mode mode;

	  mode = (GET_CODE (*container_loc) == MEM
		  ? GET_MODE (*container_loc) : VOIDmode);
	  ok_p = find_interm_elimination_reg (a, BASE_REG_CLASS,
					      base_regs [mode]);
	}
      else
	ok_p = find_interm_elimination_reg (a, INDEX_REG_CLASS, index_regs);
    }
  else
    {
      rtx insn = INSN_ALLOCNO_INSN (a);

      if (GET_CODE (*container_loc) == PLUS
	  && GET_CODE (XEXP (*container_loc, 1)) == CONST_INT
	  && ((disp_a = insn_allocno (XEXP (*container_loc, 1), insn)) == NULL
	      || INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (disp_a)))
	{
	  /* The register is in operator PLUS.  */
	  rtx temp_const_int;
	  
	  yara_assert (XEXP (*container_loc, 0) == *INSN_ALLOCNO_LOC (a));
	  temp_const_int = XEXP (*container_loc, 1);
	  for (elim = reg_eliminate [regno]; elim != NULL; elim = elim->next)
	    {
	      offset = elim->offset;
	      if (elim->to == STACK_POINTER_REGNUM)
		offset += slot_memory_size;
	      offset += INTVAL (temp_const_int);
	      if (all_alt_offset_ok_p (a, offset))
		{
		  INSN_ALLOCNO_ELIMINATION (a) = elim;
		  return true;
		}
	    }
	}
      possible_alt_reg_intersection (a, &possible_regs);
      cl = smallest_superset_class (possible_regs);
      /* ??? zero displacement */
      ok_p = find_interm_elimination_reg (a, cl, possible_regs);
    }
  if (ok_p)
    /* ??? 1st elimination */
    INSN_ALLOCNO_ELIMINATION (a) = reg_eliminate [regno];
  return ok_p;
}

void
uneliminate_reg (allocno_t a ATTRIBUTE_UNUSED)
{
  yara_assert (ALLOCNO_REGNO (a) < FIRST_VIRTUAL_REGISTER
	       || ALLOCNO_REGNO (a) > LAST_VIRTUAL_REGISTER);
}

/* The function returns hard register and offset through paarmeters TO
   and OFFSET of function invariant address X.  */
void
get_equiv_const_addr_info (rtx x, int *to, HOST_WIDE_INT *offset)
{
  if (GET_CODE (x) != PLUS)
    *offset = 0;
  else
    {
      /* ??? Fixme: In general it should be a general constant.  */
      yara_assert (GET_CODE (XEXP (x, 1)) == CONST_INT);
      *offset = INTVAL (XEXP (x, 1));
      x = XEXP (x, 0);
    }
  yara_assert (x == frame_pointer_rtx || x == arg_pointer_rtx);
  *to = REGNO (x);
}

/* The function returns final hard register and final offset through
   paarmeters TO and OFFSET of function invariant address X after
   elimination of virtual register in X.  */
void
get_equiv_const_elimination_info (rtx x, int *to, HOST_WIDE_INT *offset)
{
  struct reg_eliminate *elim;

  if (GET_CODE (x) != PLUS)
    *offset = 0;
  else
    {
      /* ??? Fixme: In general it should be a constant.  */
      yara_assert (GET_CODE (XEXP (x, 1)) == CONST_INT);
      *offset = INTVAL (XEXP (x, 1));
      x = XEXP (x, 0);
    }
  yara_assert (x == frame_pointer_rtx || x == arg_pointer_rtx);
  if ((elim = reg_eliminate [REGNO (x)]) == NULL)
    *to = REGNO (x);
  else
    {
      /* ??? only 1st elimination.  */
      *to = elim->to;
      *offset += elim->offset;
      if (*to == STACK_POINTER_REGNUM)
	*offset += slot_memory_size;
   }
}



enum log_type {ALLOCNO_LOG, COPY_LOG, MEMORY_SLOT_LOG};

struct log_entry
{
  enum log_type type;
  union
  {
    struct allocno_log_entry a;
    struct copy_log_entry c;
    struct memory_slot_log_entry m;
  } u;
  struct log_entry *next_free_entry;
};

static varray_type log_varray;
static struct log_entry *free_log_entries;

struct transaction
{
  int log_varray_start;
  int saved_global_allocation_cost;
  struct transaction *next_free_entry;
};

static varray_type transaction_varray;
static struct transaction *free_transactions;

static struct log_entry *
get_free_log_entry (void)
{
  struct log_entry *l;

  if (free_log_entries == NULL)
    l = yara_allocate (sizeof (struct log_entry));
  else
    {
      l = free_log_entries;
      free_log_entries = free_log_entries->next_free_entry;
    }
  return l;
}

static void
free_log_entry (struct log_entry *l)
{
  l->next_free_entry = free_log_entries;
  free_log_entries = l;
}

static void
free_all_log_entries (void)
{
  struct log_entry *l, *next_l;
  
  for (l = free_log_entries; l != NULL; l = next_l)
    {
      next_l = l->next_free_entry;
      yara_free (l);
    }
}

static struct transaction *
get_free_transaction (void)
{
  struct transaction *trans;

  if (free_transactions == NULL)
    trans = yara_allocate (sizeof (struct transaction));
  else
    {
      trans = free_transactions;
      free_transactions = free_transactions->next_free_entry;
    }
  return trans;
}

static void
free_transaction (struct transaction *trans)
{
  trans->next_free_entry = free_transactions;
  free_transactions = trans;
}

static void
free_all_transactions (void)
{
  struct transaction *tr, *next_tr;

  for (tr = free_transactions; tr != NULL; tr = next_tr)
    {
      next_tr = tr->next_free_entry;
      yara_free (tr);
    }
}

static void
initiate_transactions (void)
{
  free_log_entries = NULL;
  free_transactions = NULL;
  VARRAY_GENERIC_PTR_NOGC_INIT (log_varray,
				yara_max_uid / 10 + 1, "log enries");
  VARRAY_GENERIC_PTR_NOGC_INIT (transaction_varray, 10, "transaction stack");
}

void
start_transaction (void)
{
  struct transaction *trans;

  if (VARRAY_ACTIVE_SIZE (transaction_varray) == 0)
    switch_on_pending_memory_slot_structures ();
  trans = get_free_transaction ();
  trans->log_varray_start = VARRAY_ACTIVE_SIZE (log_varray);
  trans->saved_global_allocation_cost = global_allocation_cost;
  VARRAY_PUSH_GENERIC_PTR (transaction_varray, trans);
}

static void
log_allocno (allocno_t a)
{
  struct log_entry *l;

  if (VARRAY_ACTIVE_SIZE (transaction_varray) == 0)
    return;
  l = get_free_log_entry ();
  l->type = ALLOCNO_LOG;
  l->u.a.allocno = a;
  l->u.a.change = ALLOCNO_CHANGE (a);
  if (ALLOCNO_TYPE (a) == INSN_ALLOCNO)
    l->u.a.insn_change = INSN_ALLOCNO_CHANGE (a);
  VARRAY_PUSH_GENERIC_PTR (log_varray, l);
}

static void
undo_allocno_change (struct allocno_log_entry *al)
{
  int regno, align = 0;
  int num = 0;
  allocno_t a;
  can_t can;

  a = al->allocno;
  regno = ALLOCNO_REGNO (a);
  if (ALLOCNO_HARD_REGNO (a) != al->change.hard_regno)
    {
      if (al->change.hard_regno < 0)
	mark_regno_release
	  (get_maximal_part_start_hard_regno (ALLOCNO_HARD_REGNO (a), a),
	   get_allocation_mode (a));
      else if (ALLOCNO_HARD_REGNO (a) < 0)
	mark_regno_allocation
	  (get_maximal_part_start_hard_regno (al->change.hard_regno, a),
	   get_allocation_mode (a));
      else
	gcc_unreachable ();
    }
  can = ALLOCNO_CAN (a);
  if (can != NULL)
    {
      num = CAN_SLOTNO (can);
      align = slotno_max_ref_align [num];
    }
  if (ALLOCNO_MEMORY_SLOT (a) != al->change.memory_slot)
    {
      yara_assert (regno >= 0 && can != NULL);
      if (ALLOCNO_MEMORY_SLOT (a) != NULL
	  && ALLOCNO_MEMORY_SLOT (a)->mem == NULL_RTX)
	unregister_memory_slot_usage (ALLOCNO_MEMORY_SLOT (a), align);
      if (al->change.memory_slot != NULL
	  && al->change.memory_slot->mem == NULL_RTX)
	register_memory_slot_usage (al->change.memory_slot, align);
    }
  ALLOCNO_CHANGE (a) = al->change;
  if (can != NULL
      && (ALLOCNO_MEMORY_SLOT (a) != can_memory_slots [CAN_SLOTNO (can)]))
    {
      if (can_memory_slots [num] == NULL)
	{
	  can_memory_slots [num] = ALLOCNO_MEMORY_SLOT (a);
	  yara_assert (can_memory_slots [num]->mem != NULL_RTX
		       || can_memory_slots [num]->allocnos_num != 0);
	}
      else if (ALLOCNO_MEMORY_SLOT (a) == NULL)
	{
	  if (can_memory_slots [num]->mem == NULL_RTX
	      && can_memory_slots [num]->allocnos_num == 0)
	    can_memory_slots [num] = NULL;
	}
      else
	gcc_unreachable ();
    }
  if (ALLOCNO_TYPE (a) == INSN_ALLOCNO)
    {
      if (INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (a)
	  != al->insn_change.interm_elimination_regno)
	{
	  if (al->insn_change.interm_elimination_regno < 0)
	    mark_regno_release
	      (INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (a), Pmode);
	  else if (INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (a) < 0)
	    mark_regno_allocation
	      (al->insn_change.interm_elimination_regno, Pmode);
	  else
	    gcc_unreachable ();
	}
      INSN_ALLOCNO_CHANGE (a) = al->insn_change;
    }
}

static void
log_copy (copy_t cp)
{
  struct log_entry *l;

  if (VARRAY_ACTIVE_SIZE (transaction_varray) == 0)
    return;
  l = get_free_log_entry ();
  l->type = COPY_LOG;
  l->u.c.copy = cp;
  l->u.c.copy_change = COPY_CHANGE_ADDR (cp);
  if (COPY_CHANGE_ADDR (cp) != NULL)
    {
      l->u.c.copy_change = get_free_copy_change ();
      memcpy (l->u.c.copy_change, COPY_CHANGE_ADDR (cp),
	      sizeof (struct copy_change));
    }
  VARRAY_PUSH_GENERIC_PTR (log_varray, l);
}

static void
undo_copy_change (struct copy_log_entry *cl)
{
  copy_t cp = cl->copy;
  struct memory_slot *copy_slot, *log_slot;
  int copy_regno, log_regno;
  bool new_p;

#ifdef SECONDARY_MEMORY_NEEDED
  copy_slot = (COPY_CHANGE_ADDR (cp) == NULL ? NULL : COPY_MEMORY_SLOT (cp));
  log_slot = (cl->copy_change == NULL ? NULL : cl->copy_change->memory_slot);
  if (copy_slot != log_slot)
    {
      int align;
      
      if (copy_slot != NULL)
	{
	  align = get_stack_align (COPY_MEMORY_MODE (cp)) / BITS_PER_UNIT;
	  yara_assert (copy_slot->mem == NULL_RTX);
	  unregister_memory_slot_usage (copy_slot, align);
	  bitmap_clear_bit (secondary_memory_copies, COPY_NUM (cp));
	}
      if (log_slot != NULL)
	{
	  align = (get_stack_align (cl->copy_change->memory_mode)
		   / BITS_PER_UNIT);
	  yara_assert (log_slot->mem == NULL_RTX);
	  register_memory_slot_usage (log_slot, align);
	  bitmap_set_bit (secondary_memory_copies, COPY_NUM (cp));
	}
    }
#endif
#ifdef HAVE_SECONDARY_RELOADS
  copy_regno = (COPY_CHANGE_ADDR (cp) == NULL ? -1 : COPY_INTERM_REGNO (cp));
  log_regno = (cl->copy_change == NULL ? -1 : cl->copy_change->interm_regno);
  if (copy_regno != log_regno)
    {
      if (log_regno < 0)
	mark_regno_release (copy_regno, COPY_INTERM_MODE (cp));
      else if (copy_regno < 0)
	mark_regno_allocation (log_regno, cl->copy_change->interm_mode);
      else
	gcc_unreachable ();
    }
  copy_regno = (COPY_CHANGE_ADDR (cp) == NULL ? -1 : COPY_SCRATCH_REGNO (cp));
  log_regno = (cl->copy_change == NULL ? -1 : cl->copy_change->scratch_regno);
  if (copy_regno != log_regno)
    {
      if (log_regno < 0)
	mark_regno_release (copy_regno, COPY_SCRATCH_MODE (cp));
      else if (copy_regno < 0)
	mark_regno_allocation (log_regno, cl->copy_change->scratch_mode);
      else
	gcc_unreachable ();
    }
#endif
  copy_regno = (COPY_CHANGE_ADDR (cp) == NULL
		? -1 : COPY_INTERM_EQUIV_CONST_REGNO (cp));
  log_regno = (cl->copy_change == NULL
	       ? -1 : cl->copy_change->interm_equiv_const_regno);
  if (copy_regno != log_regno)
    {
      if (log_regno < 0)
	mark_regno_release (copy_regno, COPY_INTERM_EQUIV_CONST_MODE (cp));
      else if (copy_regno < 0)
	mark_regno_allocation
	  (log_regno, cl->copy_change->interm_equiv_const_mode);
      else
	gcc_unreachable ();
    }
  if (COPY_CHANGE_ADDR (cp) != NULL && cl->copy_change == NULL)
    free_copy_change (COPY_CHANGE_ADDR (cp));
  new_p = (COPY_CHANGE_ADDR (cp) == NULL && cl->copy_change != NULL);
  COPY_CHANGE_ADDR (cp) = cl->copy_change;
  if (new_p)
    {
      COPY_CHANGE_ADDR (cp) = get_free_copy_change ();
      memcpy (COPY_CHANGE_ADDR (cp), cl->copy_change,
	      sizeof (struct copy_change));
    }
}

static void
log_memory_slot (struct memory_slot *slot)
{
  struct log_entry *l;

  if (VARRAY_ACTIVE_SIZE (transaction_varray) == 0)
    return;
  l = get_free_log_entry ();
  l->type = MEMORY_SLOT_LOG;
  l->u.m.memory_slot = slot;
  l->u.m.start = slot->start;
  VARRAY_PUSH_GENERIC_PTR (log_varray, l);
}

static void
undo_memory_slot_change (struct memory_slot_log_entry *sl)
{
  struct memory_slot *slot = sl->memory_slot;

  if (sl->start != slot->start)
    {
#ifdef FRAME_GROWS_DOWNWARD
      remove_memory_slot_end (slot->start);
      add_memory_slot_end (sl->start);
#else
      remove_memory_slot_end (slot->start + slot->size - 1);
      add_memory_slot_end (sl->start + slot->size - 1);
#endif
      slot->start = sl->start;
    }
}

static void
undo_change (struct log_entry *l, bool accept_change_p)
{
  if (! accept_change_p)
    {
      if (l->type == ALLOCNO_LOG)
	undo_allocno_change (&l->u.a);
      else if (l->type == COPY_LOG)
	undo_copy_change (&l->u.c);
      else if (l->type == MEMORY_SLOT_LOG)
	undo_memory_slot_change (&l->u.m);
      else
	gcc_unreachable ();
    }
  if (l->type == COPY_LOG && l->u.c.copy_change != NULL)
    free_copy_change (l->u.c.copy_change);
  free_log_entry (l);
}

static void
stop_transaction (bool accept_change_p)
{
  int i, len;
  struct log_entry *l;
  struct transaction *trans;

  len = VARRAY_ACTIVE_SIZE (transaction_varray);
  yara_assert (len != 0);
  trans = VARRAY_GENERIC_PTR (transaction_varray, len - 1);
  /* We don't end transaction until the top transaction.  Otherwise we
     would be not able to undo the top transaction.  */
  if (! accept_change_p || len == 1)
    {
      for (i = (int) VARRAY_ACTIVE_SIZE (log_varray) - 1;
	   i >= trans->log_varray_start;
	   i--)
	{
	  l = VARRAY_GENERIC_PTR (log_varray, i);
	  undo_change (l, accept_change_p);
	  VARRAY_POP (log_varray);
	}
      if (! accept_change_p)
	global_allocation_cost = trans->saved_global_allocation_cost;
    }
  free_transaction (trans);
  VARRAY_POP (transaction_varray);
  if (VARRAY_ACTIVE_SIZE (transaction_varray) == 0)
    free_pending_memory_slot_structures ();
}

void
undo_transaction (void)
{
  stop_transaction (false);
}

void
end_transaction (void)
{
  stop_transaction (true);
}

static void
finish_transactions (void)
{
  yara_assert (VARRAY_ACTIVE_SIZE (transaction_varray) == 0);
  VARRAY_FREE (transaction_varray);
  VARRAY_FREE (log_varray);
  free_all_transactions ();
  free_all_log_entries ();
}



bool
check_hard_regno_memory_on_constraint (allocno_t a, bool use_equiv_const_p,
				       int hard_regno)
{
  alt_set_t temp_alt_set, saved_alt_set;
  bool saved_use_equiv_const_p;
  int op_num, saved_hard_regno;
  struct memory_slot *saved_memory_slot, temp_memory_slot;
  struct insn_op_info *info;
  allocno_t curr_a;

  yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
  yara_assert (! use_equiv_const_p || hard_regno < 0);
  if (INSN_ALLOCNO_TYPE (a) == NON_OPERAND)
    /* ???? */
    return hard_regno >= 0;
  if (INSN_ALLOCNO_TYPE (a) == BASE_REG || INSN_ALLOCNO_TYPE (a) == INDEX_REG)
    {
      HARD_REG_SET temp;

      if (INSN_ALLOCNO_TYPE (a) == BASE_REG)
	COPY_HARD_REG_SET
	  (temp, base_regs [GET_MODE (*INSN_ALLOCNO_CONTAINER_LOC (a))]);
      else
	COPY_HARD_REG_SET (temp, index_regs);
      if (! use_equiv_const_p)
	return hard_regno >= 0 && TEST_HARD_REG_BIT (temp, hard_regno);
      else
	{
	  rtx equiv_const, *container_loc, *address_loc;
	  int to;
	  HOST_WIDE_INT offset;
	  bool base_p;
	  
	  equiv_const = reg_equiv_constant [ALLOCNO_REGNO (a)];
	  yara_assert (equiv_cosnt != NULL_RTX);
	  if (CONSTANT_P (equiv_const))
	    return false;
	  get_equiv_const_addr_info (equiv_const, &to, &offset);
	  container_loc = INSN_ALLOCNO_CONTAINER_LOC (a);
	  address_loc = (GET_CODE (*container_loc) == MEM
			 ? &XEXP (*container_loc, 0) : container_loc);
	  /* ??? Allocate interm equiv constant register. Do we need
	     this?  */
	  return check_elimination_in_addr (ALLOCNO_REGNO (a), to, offset,
					    address_loc, container_loc,
					    &base_p) != NULL;
	}
    }
  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
  yara_assert (op_num >= 0);
  info = insn_infos [INSN_UID (INSN_ALLOCNO_INSN (a))];
  COPY_ALT_SET (saved_alt_set, INSN_ALLOCNO_POSSIBLE_ALTS (a));
  saved_use_equiv_const_p = ALLOCNO_USE_EQUIV_CONST_P (a);
  saved_hard_regno = ALLOCNO_HARD_REGNO (a);
  saved_memory_slot = ALLOCNO_MEMORY_SLOT (a);
  /* ??? SUBREG */
  ALLOCNO_HARD_REGNO (a) = hard_regno;
  ALLOCNO_MEMORY_SLOT (a) = NULL;
  if (use_equiv_const_p)
    ALLOCNO_USE_EQUIV_CONST_P (a) = true;
  else if (hard_regno < 0)
    ALLOCNO_MEMORY_SLOT (a) = &temp_memory_slot;
  setup_possible_allocno_alternatives (info, a, true);
  ALLOCNO_USE_EQUIV_CONST_P (a) = saved_use_equiv_const_p;
  ALLOCNO_HARD_REGNO (a) = saved_hard_regno;
  ALLOCNO_MEMORY_SLOT (a) = saved_memory_slot;
  for (curr_a = insn_allocnos [INSN_UID (INSN_ALLOCNO_INSN (a))];
       curr_a != NULL;
       curr_a = INSN_ALLOCNO_NEXT (curr_a))
    if (INSN_ALLOCNO_TYPE (curr_a) >= OPERAND_BASE)
      {
	COPY_ALT_SET (temp_alt_set, INSN_ALLOCNO_POSSIBLE_ALTS (curr_a));
	AND_ALT_SET (temp_alt_set, INSN_ALLOCNO_POSSIBLE_ALTS (a));
	if (EQ_ALT_SET (temp_alt_set, ZERO_ALT_SET))
	  break;
      }
  COPY_ALT_SET (INSN_ALLOCNO_POSSIBLE_ALTS (a), saved_alt_set);
  return curr_a == NULL;
}



void
eliminate_virtual_registers (int (*func) (allocno_t, enum reg_class,
					  HARD_REG_SET))
{
  int i;
  allocno_t a;

  provide_allocno_elimination_class_hard_reg_func = func;
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (a == NULL)
	continue;
      if (ALLOCNO_TYPE (a) == INSN_ALLOCNO && INSN_ALLOCNO_ELIMINATION_P (a))
	{
	  if (! eliminate_reg (a))
	    gcc_unreachable ();
	}
    }
}



/* The function is called once for each compilation file.  It does
   initialization of data common for all RTL functions.  */
void
yara_trans_init_once (void)
{
  setup_temp_mems_and_addresses ();
  setup_move_costs ();
  set_base_index_reg_sets ();
}

/* The function initializes local file data used for each RTL
   function.  */
void
yara_trans_init (void)
{
  set_ever_live_regs ();
  initiate_copy_changes ();
  initiate_memory_slots ();
  initiate_transactions ();
}

/* The function frees local file data used for each RTL function.  */
void
yara_trans_finish (void)
{
  finish_memory_slots ();
  finish_transactions ();
  finish_copy_changes ();
}

#include "gt-yara-trans.h"
