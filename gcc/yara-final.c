/* Final part of Yet Another Register Allocator.
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
#include "obstack.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "insn-codes.h"
#include "expr.h"
#include "optabs.h"
#include "recog.h"
#include "cfgloop.h"
#include "errors.h"
#include "output.h"
#include "params.h"
#include "yara-int.h"

static void get_reg_set_and_memory_slots (copy_t, HARD_REG_SET *,
					  struct memory_slot **,
					  struct memory_slot **,
					  struct memory_slot **);
static bool copy_can_be_moved_through_copy (copy_t, copy_t);
static bool copy_can_be_moved_through_insn_allocno_p (copy_t, allocno_t);
static bool copy_can_be_moved_through_insn_p (rtx, copy_t);
static rtx get_allocno_memory_slot_rtx (struct memory_slot *, int,
					enum machine_mode, allocno_t);
static rtx gen_allocno_reg_rtx (enum machine_mode, int, allocno_t);
static void emit_move (rtx, rtx);
static bool emit_secondary_memory_mode_move (rtx, rtx, copy_t,
					     enum machine_mode);
static void emit_secondary_memory_move (rtx, rtx, copy_t, enum machine_mode,
					enum machine_mode, bool);
static rtx get_eliminate_subst_rtx (rtx *, int, int, HOST_WIDE_INT);
static void emit_plus_assign (int, allocno_t, rtx, HOST_WIDE_INT);
static void emit_copy (copy_t);
static rtx copy_insns (rtx);
static void emit_insns_at_bb_start (rtx, basic_block);
static void emit_insns_at_bb_end (rtx, basic_block, rtx, bool);
static void add_copy_list (copy_t);
static void eliminate_reg_allocno (allocno_t, int, int, HOST_WIDE_INT);
static void modify_insn (rtx, bool);
static void clean_insn (rtx);

static void allocate_stack_memory (void);

static bitmap insn_to_be_removed_p;

extern int n;




/* Return true if the copy is not necessary. It means that source and
   destination got the same place.  */
bool
unnecessary_copy_p (copy_t cp)
{
  allocno_t src = COPY_SRC (cp), dst = COPY_DST (cp);

  if (src == dst)
    return true;
  if (src == NULL) 
    {
      /* DST is non pseudo register allocno.  */
      yara_assert (ALLOCNO_TYPE (dst) == INSN_ALLOCNO);
      return (ALLOCNO_MEMORY_SLOT (dst) == NULL
	      && ALLOCNO_HARD_REGNO (dst) < 0
	      && INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (dst) < 0);
    }
  else if (dst == NULL)
    {
      /* SRC is non pseudo register allocno.  */
      yara_assert (ALLOCNO_TYPE (src) == INSN_ALLOCNO);
      /* SRC is non pseudo register allocno.  */
      return (ALLOCNO_MEMORY_SLOT (src) == NULL
	      && ALLOCNO_HARD_REGNO (src) < 0);
    }
  else if (ALLOCNO_USE_EQUIV_CONST_P (dst))
    return true;
  else if (ALLOCNO_MEMORY_SLOT (dst) != NULL
	   && ALLOCNO_MEMORY_SLOT (dst)->mem != NULL_RTX
	   && reg_equiv_set_p [ALLOCNO_REGNO (dst)])
    return true;
  else if (ALLOCNO_MEMORY_SLOT (src) != NULL
	   && ALLOCNO_MEMORY_SLOT (dst) != NULL)
    {
      yara_assert ((ALLOCNO_MEMORY_SLOT (src)->mem
		    == ALLOCNO_MEMORY_SLOT (dst)->mem)
		   && (ALLOCNO_MEMORY_SLOT (src)->start
		       == ALLOCNO_MEMORY_SLOT (dst)->start)
		   && (ALLOCNO_MEMORY_SLOT (src)->size
		       == ALLOCNO_MEMORY_SLOT (dst)->size));
      return true;
    }
  else
    {
      int src_hard_regno = ALLOCNO_HARD_REGNO (src);
      int dst_hard_regno = ALLOCNO_HARD_REGNO (dst);

      if (dst_hard_regno >= 0 && ALLOCNO_TYPE (src) == INSN_ALLOCNO)
	dst_hard_regno = get_allocno_hard_regno (src, dst_hard_regno);
      if (src_hard_regno >= 0 && ALLOCNO_TYPE (dst) == INSN_ALLOCNO)
	src_hard_regno = get_allocno_hard_regno (dst, src_hard_regno);
      if (src_hard_regno != dst_hard_regno)
	return false;
      if (dst_hard_regno < 0)
	return ! ALLOCNO_USE_EQUIV_CONST_P (src);
      else
	return true;
    }
}



static int live_call_used_regs [FIRST_PSEUDO_REGISTER];
static int live_call_used_regs_num;

static void (*process_reg_reg_loc_copy_func) (int dst_hard_regno,
					      int dst_nregs,
					      int src_hard_regno,
					      int src_nregs);
static void (*process_reg_mem_loc_copy_func) (bool dst_reg_p, int hard_regno,
					      struct memory_slot *memory_slot,
					      int offset, int len);
static void (*process_reg_loc_set_func) (int hard_regno);
static void (*process_mem_loc_set_func) (struct memory_slot *memory_slot,
					 int offset, int len);
static void (*process_pre_copy_func) (rtx insn, copy_t copy);
static void (*process_all_reg_memory_copy_func) (copy_t, enum machine_mode,
						 int, struct memory_slot *,
						 int);
static void (*process_post_copy_func) (rtx insn, copy_t copy);

static void
process_allocno_locs (allocno_t dst, allocno_t src, copy_t cp, rtx insn)
{
  int src_hard_regno, dst_hard_regno, src_offset, dst_offset;
  int i, nregs, part_size;
  struct memory_slot *src_memory_slot, *dst_memory_slot;
  enum machine_mode cp_mode;
  
  if (process_pre_copy_func != NULL)
    (*process_pre_copy_func) (insn, cp);
  if (cp == NULL)
    {
      yara_assert (insn != NULL && src != NULL && dst != NULL);
      cp_mode = ALLOCNO_MODE (src);
      src_hard_regno = ALLOCNO_HARD_REGNO (src);
      src_memory_slot = ALLOCNO_MEMORY_SLOT (src);
      src_offset = ALLOCNO_MEMORY_SLOT_OFFSET (src);
      dst_hard_regno = ALLOCNO_HARD_REGNO (dst);
      dst_memory_slot = ALLOCNO_MEMORY_SLOT (dst);
      dst_offset = ALLOCNO_MEMORY_SLOT_OFFSET (dst);
    }
  else
    {
      get_copy_loc (cp, true, &cp_mode, &src_hard_regno, &src_memory_slot,
		    &src_offset);
      get_copy_loc (cp, false, &cp_mode, &dst_hard_regno, &dst_memory_slot,
		    &dst_offset);
    }
  if (cp != NULL && dst != NULL 
      && ALLOCNO_TYPE (dst) == INSN_ALLOCNO
      && INSN_ALLOCNO_ELIMINATION (dst) != NULL)
    {
      int hard_regno;

      yara_assert (src == NULL);
      if ((hard_regno = INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (dst)) >= 0)
	{
	  if (process_reg_loc_set_func)
	    for (i = hard_regno_nregs [hard_regno] [Pmode] - 1; i >= 0; i--)
	      (*process_reg_loc_set_func) (hard_regno + i);
	}
      else if (dst_hard_regno >= 0)
	{
	  if (process_reg_loc_set_func)
	    for (i = hard_regno_nregs [dst_hard_regno] [cp_mode] - 1;
		 i >= 0;
		 i--)
	      (*process_reg_loc_set_func) (dst_hard_regno + i);
	}
      else
	{
	  yara_assert (dst_memory_slot != NULL);
	  (*process_mem_loc_set_func)
	    (dst_memory_slot, 0, GET_MODE_SIZE (cp_mode));
	}
    }
  else if (src_hard_regno >= 0)
    {
      if (dst_hard_regno >= 0)
	{
	  if (hard_regno_nregs [dst_hard_regno] [cp_mode]
	      != hard_regno_nregs [src_hard_regno] [cp_mode])
	    (*process_reg_reg_loc_copy_func)
	      (dst_hard_regno, hard_regno_nregs [dst_hard_regno] [cp_mode],
	       src_hard_regno, hard_regno_nregs [src_hard_regno] [cp_mode]);
	  else
	    {
	      nregs = hard_regno_nregs [dst_hard_regno] [cp_mode];
	      /* Check sync order in case if the source and
		 destination registers are intersected.  */
	      if (dst_hard_regno < src_hard_regno)
		{
		  for (i = 0; i < nregs; i++)
		    (*process_reg_reg_loc_copy_func)
		      (dst_hard_regno + i, 1, src_hard_regno + i, 1);
		}
	      else
		{
		  for (i = nregs - 1; i >= 0; i--)
		    (*process_reg_reg_loc_copy_func)
		      (dst_hard_regno + i, 1, src_hard_regno + i, 1);
		}
	    }
	}
      else if (dst_memory_slot != NULL)
	{
	  nregs = hard_regno_nregs [src_hard_regno] [cp_mode];
	  part_size = GET_MODE_SIZE (cp_mode) / nregs;
	  yara_assert (GET_MODE_SIZE (cp_mode) % nregs == 0);
	  for (i = nregs - 1; i >= 0; i--)
	    (*process_reg_mem_loc_copy_func)
	      (false, src_hard_regno + i, dst_memory_slot,
	       dst_offset + i * part_size, part_size);
	}
    }
  else if (dst_hard_regno >= 0)
    {
      if (src_memory_slot != NULL)
	{
	  nregs = hard_regno_nregs [dst_hard_regno] [cp_mode];
	  part_size = GET_MODE_SIZE (cp_mode) / nregs;
	  yara_assert (GET_MODE_SIZE (cp_mode) % nregs == 0);
	  for (i = nregs - 1; i >= 0; i--)
	    (*process_reg_mem_loc_copy_func)
	      (true, dst_hard_regno + i, src_memory_slot,
	       src_offset + i * part_size, part_size);
	  if (process_all_reg_memory_copy_func != NULL)
	    (*process_all_reg_memory_copy_func) (cp, cp_mode, dst_hard_regno,
						 src_memory_slot, src_offset);
	}
      else if (process_reg_loc_set_func != NULL)
	{
	  for (i = hard_regno_nregs [dst_hard_regno] [cp_mode] - 1;
	       i >= 0;
	       i--)
	    (*process_reg_loc_set_func) (dst_hard_regno + i);
	}
    }
  else if (src != NULL && ALLOCNO_USE_EQUIV_CONST_P (src))
    {
      yara_assert ((cp == NULL && ALLOCNO_USE_EQUIV_CONST_P (dst))
		   || dst_memory_slot != NULL);
      if (! ALLOCNO_USE_EQUIV_CONST_P (dst))
	(*process_mem_loc_set_func)
	  (dst_memory_slot, 0, GET_MODE_SIZE (cp_mode));
    }
  else
    gcc_unreachable ();
  if (process_post_copy_func != NULL)
    (*process_post_copy_func) (insn, cp);
}

static void
process_copy_locs (copy_t copy_list)
{
  copy_t cp;
  int i, hard_regno;
  struct memory_slot *memory_slot;

  for (cp = copy_list; cp != NULL; cp = COPY_NEXT_COPY (cp))
    if (! unnecessary_copy_p (cp))
      {
	process_allocno_locs (COPY_DST (cp), COPY_SRC (cp), cp, NULL_RTX);
#ifdef HAVE_SECONDARY_RELOADS
	if (COPY_CHANGE_ADDR (cp) != NULL && process_reg_loc_set_func != NULL)
	  {
	    if ((hard_regno = COPY_INTERM_REGNO (cp)) >= 0)
	      for (i = hard_regno_nregs [hard_regno]
		                        [COPY_INTERM_MODE (cp)] - 1;
		   i >= 0;
		   i--)
		(*process_reg_loc_set_func) (hard_regno + i);
	    if ((hard_regno = COPY_SCRATCH_REGNO (cp)) >= 0)
	      for (i = hard_regno_nregs [hard_regno]
		        [COPY_SCRATCH_MODE (cp)] - 1;
		   i >= 0;
		   i--)
		(*process_reg_loc_set_func) (hard_regno + i);
	  }
#endif
#ifdef SECONDARY_MEMORY_NEEDED
	if (COPY_CHANGE_ADDR (cp) != NULL)
	  {
	    if ((memory_slot = COPY_MEMORY_SLOT (cp)) != NULL)
	      (*process_mem_loc_set_func)
		(memory_slot, 0, GET_MODE_SIZE (get_copy_mode (cp)));
	  }
#endif
      }
}

static void
process_bb_locs (basic_block bb)
{
  int i, hard_regno;
  rtx insn, bound, set;
  enum machine_mode mode;
  allocno_t a, a2;
  struct memory_slot *memory_slot;
  bool cont_p;

  bound = NEXT_INSN (BB_END (bb));
  process_copy_locs (at_bb_start_copies [bb->index]);
  for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
    {
      if (! INSN_P (insn))
	continue;
      process_copy_locs (before_insn_copies [INSN_UID (insn)]);
      cont_p = true;
      if ((set = single_set (insn)) != NULL_RTX
	  && REG_P (SET_DEST (set)) && REG_P (SET_SRC (set))
	  && ! HARD_REGISTER_P (SET_DEST (set))
	  && ! HARD_REGISTER_P (SET_SRC (set))
	  && ! side_effects_p (set)
	  && find_reg_note (insn, REG_RETVAL, NULL_RTX) == NULL_RTX)
	{
	  a = insn_allocno (SET_SRC (set), insn);
	  a2 = insn_allocno (SET_DEST (set), insn);
	  cont_p = (ALLOCNO_MEMORY_SLOT (a) != NULL
		    && ALLOCNO_MEMORY_SLOT (a2) != NULL);
	  if (! cont_p)
	    process_allocno_locs (a2, a, NULL, insn);
	}
      if (cont_p)
	{
	  for (a = insn_allocnos [INSN_UID (insn)];
	       a != NULL;
	       a = INSN_ALLOCNO_NEXT (a))
	    if ((INSN_ALLOCNO_OP_MODE (a) == OP_OUT
		 || INSN_ALLOCNO_OP_MODE (a) == OP_INOUT))
	      {
		if ((hard_regno = ALLOCNO_HARD_REGNO (a)) >= 0)
		  {
		    mode = get_allocation_mode (a);
		    hard_regno
		      = get_maximal_part_start_hard_regno (hard_regno, a);
		    if (process_reg_loc_set_func != NULL)
		      for (i = hard_regno_nregs [hard_regno] [mode] - 1;
			   i >= 0;
			   i--)
			(*process_reg_loc_set_func) (hard_regno + i);
		  }
		else if ((memory_slot = ALLOCNO_MEMORY_SLOT (a)) != NULL)
		  {
		    mode = get_allocation_mode (a);
		    (*process_mem_loc_set_func)
		      (memory_slot, ALLOCNO_MEMORY_SLOT_OFFSET (a),
		       GET_MODE_SIZE (mode));
		  }
		else if ((hard_regno = ALLOCNO_REGNO (a)) >= 0)
		  {
		    yara_assert (HARD_REGISTER_NUM_P (hard_regno));
		    hard_regno = get_allocno_hard_regno (a, hard_regno);
		    hard_regno
		      = get_maximal_part_start_hard_regno (hard_regno, a);
		    mode = get_allocation_mode (a);
		    if (process_reg_loc_set_func != NULL)
		      for (i = hard_regno_nregs [hard_regno] [mode] - 1;
			   i >= 0;
			   i--)
			(*process_reg_loc_set_func) (hard_regno + i);
		  }
	      }
	  if (CALL_P (insn) && process_reg_loc_set_func != NULL)
	    for (i = 0; i < live_call_used_regs_num; i++)
	      {
		hard_regno = live_call_used_regs [i];
		(*process_reg_loc_set_func) (hard_regno);
	      }
	}
      process_copy_locs (after_insn_copies [INSN_UID (insn)]);
    }
  process_copy_locs (at_bb_end_copies [bb->index]);
}



struct loc_conflict;

struct loc
{
  int loc_num;
  int start, len;
  bool reg_p;
  struct loc *next_start_loc;
  struct loc_conflict *loc_conflicts;
};

/* Varray containing references to locs.  */
static varray_type loc_varray;
static struct loc **locs;
static int locs_num, reg_locs_num;

static struct loc *reg_locs [FIRST_PSEUDO_REGISTER];
static struct loc **mem_locs;

static int max_mem_loc_len;

struct loc_conflict
{
  struct loc *loc1, *loc2;
  struct loc_conflict *next_loc1_conflict, *next_loc2_conflict;
};

/* Varray containing references to location conflicts.  */
static varray_type loc_conflict_varray;

static void
create_loc_conflict (struct loc *loc1, struct loc *loc2)
{
  struct loc_conflict *c;

  c = yara_allocate (sizeof (struct loc_conflict));
  VARRAY_PUSH_GENERIC_PTR (loc_conflict_varray, c);
  c->loc1 = loc1;
  c->loc2 = loc2;
  c->next_loc1_conflict = loc1->loc_conflicts;
  loc1->loc_conflicts = c;
  c->next_loc2_conflict = loc2->loc_conflicts;
  loc2->loc_conflicts = c;
}

static struct loc *
find_reg_loc (int hard_regno, int len)
{
  struct loc *loc;

  for (loc = reg_locs [hard_regno]; loc != NULL; loc = loc->next_start_loc)
    {
      yara_assert (loc->start == hard_regno);
      if (loc->len == len)
	return loc;
    }
  return NULL;
}

static void
add_conflicts (struct loc *loc)
{
  struct loc *curr_loc, **locs;
  int i, disp, bound, start;

  if (loc->reg_p)
    {
      disp = 1;
      bound = FIRST_PSEUDO_REGISTER;
      locs = reg_locs;
    }
  else
    {
      disp = max_mem_loc_len;
      bound = slot_memory_size;
      locs = mem_locs;
    }
  start = loc->start - disp;
  if (start < 0)
    start = 0;
  for (i = start; i < loc->start + disp && i < bound; i++)
    for (curr_loc = locs [i];
	 curr_loc != NULL;
	 curr_loc = curr_loc->next_start_loc)
      if (curr_loc != loc)
	{
	  yara_assert (curr_loc->start == i);
	  if ((i >= loc->start && i < loc->start + loc->len)
	      || (i < loc->start && i + curr_loc->len > loc->start))
	    create_loc_conflict (loc, curr_loc);
	}
}

static void
add_reg_loc (int hard_regno, int len)
{
  struct loc *loc;

  if (find_reg_loc (hard_regno, len) != NULL)
    return;
  loc = yara_allocate (sizeof (struct loc));
  VARRAY_PUSH_GENERIC_PTR (loc_varray, loc);
  loc->reg_p = true;
  loc->start = hard_regno;
  loc->len = len;
  loc->next_start_loc = reg_locs [hard_regno];
  reg_locs [hard_regno] = loc;
  loc->loc_conflicts = NULL;
  add_conflicts (loc);
  reg_locs_num++;
}

static int
get_slot_start (struct memory_slot *slot, int offset)
{
  if (slot->mem == NULL_RTX)
    {
      if (FRAME_GROWS_DOWNWARD)
	return slot->start + offset - slot->size + 1;
      else
	return slot->start + offset;
    }
  else
    {
      return slot_memory_size + slot->mem_index + offset;
    }
}

static struct loc *
find_mem_loc (struct memory_slot *slot, int offset, int len)
{
  struct loc *loc;
  int start;

  start = get_slot_start (slot, offset);
  for (loc = mem_locs [start]; loc != NULL; loc = loc->next_start_loc)
    {
      yara_assert (loc->start == start);
      if (loc->len == len)
	return loc;
    }
  return NULL;
}

static void
add_mem_loc (struct memory_slot *slot, int offset, int len)
{
  struct loc *loc;
  int start;

  if (find_mem_loc (slot, offset, len) != NULL)
    return;
  start = get_slot_start (slot, offset);
  loc = yara_allocate (sizeof (struct loc));
  VARRAY_PUSH_GENERIC_PTR (loc_varray, loc);
  loc->reg_p = false;
  loc->start = start;
  loc->len = len;
  loc->next_start_loc = mem_locs [start];
  mem_locs [start] = loc;
  loc->loc_conflicts = NULL;
  if (len > max_mem_loc_len)
    max_mem_loc_len = len;
  add_conflicts (loc);
}

static int
loc_compare (const void *l1p, const void *l2p)
{
  struct loc *l1 = *(struct loc **) l1p;
  struct loc *l2 = *(struct loc **) l2p;
  int diff;

  if (l1->reg_p && ! l2->reg_p)
    return -1;
  else if (! l1->reg_p && l2->reg_p)
    return 1;
  else if ((diff = l1->start - l2->start) != 0)
    return diff;
  else
    return l1->len - l2->len;
}

static void
process_reg_reg_loc_copy_for_locs (int dst_hard_regno, int dst_nregs,
				   int src_hard_regno, int src_nregs)
{
  if (dst_nregs != 1)
    add_reg_loc (dst_hard_regno, dst_nregs);
  if (src_nregs != 1)
    add_reg_loc (src_hard_regno, src_nregs);
}

static void
process_reg_mem_loc_copy_for_locs (bool dst_reg_p ATTRIBUTE_UNUSED,
				   int hard_regno ATTRIBUTE_UNUSED,
				   struct memory_slot *memory_slot,
				   int offset, int len)
{
  add_mem_loc (memory_slot, offset, len);
}

static void
initiate_locations (void)
{
  basic_block bb;
  edge_iterator ei;
  edge e;
  int i;

  VARRAY_GENERIC_PTR_NOGC_INIT
    (loc_varray, (FIRST_PSEUDO_REGISTER + slot_memory_size) * 4, "locations");
  VARRAY_GENERIC_PTR_NOGC_INIT
    (loc_conflict_varray, (FIRST_PSEUDO_REGISTER + slot_memory_size) * 8,
     "location conflicts");
  memset (reg_locs, 0, sizeof (reg_locs));
  mem_locs = yara_allocate (sizeof (struct loc *)
			    * (slot_memory_size + equiv_memory_num + 16));
  memset (mem_locs, 0,
	  sizeof (struct loc *) * (slot_memory_size + equiv_memory_num + 16));
  max_mem_loc_len = -1;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    reg_locs [i] = NULL;
  live_call_used_regs_num = 0;
  reg_locs_num = 0;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (regs_ever_live [i])
      {
	if (call_used_regs [i])
	  live_call_used_regs [live_call_used_regs_num++] = i;
	add_reg_loc (i, 1);
      }

  process_reg_reg_loc_copy_func = process_reg_reg_loc_copy_for_locs;
  process_reg_mem_loc_copy_func = process_reg_mem_loc_copy_for_locs;
  process_reg_loc_set_func = NULL;
  process_mem_loc_set_func = add_mem_loc;
  process_pre_copy_func = NULL;
  process_all_reg_memory_copy_func = NULL;
  process_post_copy_func = NULL;
  FOR_EACH_BB (bb)
    {
      process_bb_locs (bb);
      FOR_EACH_EDGE (e, ei, bb->preds)
	{
	  if (e->src != ENTRY_BLOCK_PTR)
	    {
	      process_copy_locs (at_edge_start_copies [(size_t) e->aux]);
	      process_copy_locs (at_edge_end_copies [(size_t) e->aux]);
	    }
	}
    }

  locs = (struct loc **) &VARRAY_GENERIC_PTR (loc_varray, 0);
  locs_num = VARRAY_ACTIVE_SIZE (loc_varray);
  qsort (locs, locs_num, sizeof (struct loc *), loc_compare);
  for (i = 0; i < locs_num; i++)
    locs [i]->loc_num = i;
}

static void
finish_locations (void)
{
  int i;

  for (i = 0; i < locs_num; i++)
    yara_free (locs [i]);
  for (i = VARRAY_ACTIVE_SIZE (loc_conflict_varray) - 1; i >= 0; i--)
    yara_free (VARRAY_GENERIC_PTR (loc_conflict_varray, i));
  yara_free (mem_locs);
  VARRAY_FREE (loc_conflict_varray);
  VARRAY_FREE (loc_varray);
}



/* take secondary memory and reload for copies into account ???? */

/* This page contains code to make live information more accurate.
   The accurate register liveness at program point P means:
     o there is a path from P to usage of the register and the
       register is not redefined or killed on the path.
     o register at P is partially available, i.e. there is a path from
       a register definition to the point P and the register is not
       killed (clobbered) on the path

   The standard GCC live information means only the first condition.
   Without the partial availability, there will be more register
   conflicts and as a consequence worse register allocation.  The
   typical example where the information can be different is a
   register initialized in the loop at the basic block preceding the
   loop in CFG.  */

/* The following structure contains basic block data flow information
   used to calculate partial availability of registers.  */

struct bb_info
{
  /* The basic block reverse post-order number.  */
  int rts_number;
  /* ??? */
  bool syncin_defined_p, syncout_defined_p;
#ifdef SYNC_BITMAP
  /* Registers correspondingly killed (clobbered) and defined but not
     killed afterward in the basic block.  */
  bitmap killed, syncloc;
  /* Registers partially available correspondingly at the start and
     end of the basic block.  */
  bitmap syncin, syncout;
#else
  /* Registers correspondingly killed (clobbered) and defined but not
     killed afterward in the basic block.  */
  sbitmap killed, syncloc;
  /* Registers partially available correspondingly at the start and
     end of the basic block.  */
  sbitmap syncin, syncout;
#endif
};

/* Macros for accessing data flow information of basic blocks.  */

#define BB_INFO(BB) ((struct bb_info *) (BB)->aux)
#define BB_INFO_BY_INDEX(N) BB_INFO (BASIC_BLOCK(N))

#ifdef SYNC_BITMAP
static bitmap temp_sync;
#else
static sbitmap temp_sync;
#endif

static void
initiate_reg_sync_info (void)
{
  basic_block bb;
  struct bb_info *bb_info;

  alloc_aux_for_blocks (sizeof (struct bb_info));
  FOR_EACH_BB (bb)
    {
      bb_info = bb->aux;
      bb_info->syncin_defined_p = false;
      bb_info->syncout_defined_p = false;
#ifdef SYNC_BITMAP
      bb_info->syncin = yara_allocate_bitmap ();
      bb_info->syncout = yara_allocate_bitmap ();
      bb_info->killed = yara_allocate_bitmap ();
      bb_info->syncloc = yara_allocate_bitmap ();
#else
      bb_info->syncin = sbitmap_alloc (reg_locs_num * locs_num);
      sbitmap_zero (bb_info->syncin);
      bb_info->syncout = sbitmap_alloc (reg_locs_num * locs_num);
      sbitmap_zero (bb_info->syncout);
      bb_info->killed = sbitmap_alloc (reg_locs_num * locs_num);
      sbitmap_zero (bb_info->killed);
      bb_info->syncloc = sbitmap_alloc (reg_locs_num * locs_num);
      sbitmap_zero (bb_info->syncloc);
#endif
    }
#ifdef SYNC_BITMAP
  temp_sync = yara_allocate_bitmap ();
#else
  temp_sync = sbitmap_alloc (reg_locs_num * locs_num);
#endif
}

static void
finish_reg_sync_info (void)
{
  basic_block bb;
  struct bb_info *bb_info;

#ifdef SYNC_BITMAP
  yara_free_bitmap (temp_sync);
#else
  sbitmap_free (temp_sync);
#endif
  FOR_EACH_BB (bb)
    {
      bb_info = bb->aux;
#ifdef SYNC_BITMAP
      yara_free_bitmap (bb_info->syncloc);
      yara_free_bitmap (bb_info->killed);
      yara_free_bitmap (bb_info->syncout);
      yara_free_bitmap (bb_info->syncin);
#else
      sbitmap_free (bb_info->syncloc);
      sbitmap_free (bb_info->killed);
      sbitmap_free (bb_info->syncout);
      sbitmap_free (bb_info->syncin);
#endif
    }
  free_aux_for_blocks ();
}

static void
#ifdef SYNC_BITMAP
set_sync (bitmap sync, int index1, int index2)
#else
set_sync (sbitmap sync, int index1, int index2)
#endif
{
  yara_assert (index1 < reg_locs_num);
#ifdef SYNC_BITMAP
  bitmap_set_bit (sync, index1 * locs_num + index2);
  if (index2 < reg_locs_num)
    bitmap_set_bit (sync, index2 * locs_num + index1);
#else
  SET_BIT (sync, index1 * locs_num + index2);
  if (index2 < reg_locs_num)
    SET_BIT (sync, index2 * locs_num + index1);
#endif
}

static void
#ifdef SYNC_BITMAP
clear_sync (bitmap sync, int index1, int index2)
#else
clear_sync (sbitmap sync, int index1, int index2)
#endif
{
  yara_assert (index1 < reg_locs_num);
#ifdef SYNC_BITMAP
  bitmap_clear_bit (sync, index1 * locs_num + index2);
  if (index2 < reg_locs_num)
    bitmap_clear_bit (sync, index2 * locs_num + index1);
#else
  RESET_BIT (sync, index1 * locs_num + index2);
  if (index2 < reg_locs_num)
    RESET_BIT (sync, index2 * locs_num + index1);
#endif
}

static void
#ifdef SYNC_BITMAP
clear_sync_for_one_loc (bitmap sync, struct loc *loc, bool set_p)
#else
clear_sync_for_one_loc (sbitmap sync, struct loc *loc, bool set_p)
#endif
{
  int index, i;

  index = loc->loc_num;
  if (index < reg_locs_num)
    {
      for (i = 0; i < locs_num; i++)
	if (set_p)
	  set_sync (sync, index, i);
	else
	  clear_sync (sync, index, i);
    }
  else
    {
      for (i = 0; i < reg_locs_num; i++)
	if (set_p)
	  set_sync (sync, i, index);
	else
	  clear_sync (sync, i, index);
    }

}

static void
#ifdef SYNC_BITMAP
clear_sync_for_loc (bitmap sync, struct loc *loc, bool set_p)
#else
clear_sync_for_loc (sbitmap sync, struct loc *loc, bool set_p)
#endif
{
  struct loc *conflict_loc;
  struct loc_conflict *c, *next_c;

  yara_assert (loc != NULL);
  clear_sync_for_one_loc (sync, loc, set_p);
  for (c = loc->loc_conflicts; c != NULL; c = next_c)
    {
      if (c->loc1 == loc)
	{
	  next_c = c->next_loc1_conflict;
	  conflict_loc = c->loc2;
	}
      else
	{
	  next_c = c->next_loc2_conflict;
	  conflict_loc = c->loc1;
	}
      clear_sync_for_one_loc (sync, conflict_loc, set_p);
    }
}

static void
#ifdef SYNC_BITMAP
clear_sync_for_reg (bitmap sync, int hard_regno, bool set_p)
#else
clear_sync_for_reg (sbitmap sync, int hard_regno, bool set_p)
#endif
{
  clear_sync_for_loc (sync, find_reg_loc (hard_regno, 1), set_p);
}

static void
#ifdef SYNC_BITMAP
set_sync_for_reg (bitmap sync, bitmap killed, int hard_regno, int len,
		  struct loc *loc)
#else
set_sync_for_reg (sbitmap sync, sbitmap killed, int hard_regno, int len,
		  struct loc *loc)
#endif
{
  int i, index, reg_index;
  struct loc *reg_loc;

  for (i = 0; i < len; i++)
    {
      clear_sync_for_reg (sync, hard_regno + i, false);
      if (killed != NULL)
	clear_sync_for_reg (killed, hard_regno + i, true);
    }
  reg_loc = find_reg_loc (hard_regno, len);
  yara_assert (loc != NULL && reg_loc != NULL);
  index = loc->loc_num;
  reg_index = reg_loc->loc_num;
  if (index >= reg_locs_num)
    {
      for (i = 0; i < reg_locs_num; i++)
#ifdef SYNC_BITMAP
	if (bitmap_bit_p (sync, i * locs_num + index))
#else
	if (TEST_BIT (sync, i * locs_num + index))
#endif
	  set_sync (sync, reg_index, i);
    }
  else
    {
      for (i = 0; i < locs_num; i++)
#ifdef SYNC_BITMAP
	if (bitmap_bit_p (sync, index * locs_num + i))
#else
	if (TEST_BIT (sync, index * locs_num + i))
#endif
	  set_sync (sync, reg_index, i);
    }
  set_sync (sync, reg_index, index);
}

static void
#ifdef SYNC_BITMAP
clear_sync_for_mem (bitmap sync, struct memory_slot *slot, int offset,
		    int len, bool set_p)
#else
clear_sync_for_mem (sbitmap sync, struct memory_slot *slot, int offset,
		    int len, bool set_p)
#endif
{
  clear_sync_for_loc (sync, find_mem_loc (slot, offset, len), set_p);
}

static void
#ifdef SYNC_BITMAP
set_sync_for_mem (bitmap sync, bitmap killed, int hard_regno,
		  struct memory_slot *slot, int offset, int len)
#else
set_sync_for_mem (sbitmap sync, sbitmap killed, int hard_regno,
		  struct memory_slot *slot, int offset, int len)
#endif
{
  int i, reg_index, mem_index;
  struct loc *reg_loc, *mem_loc;

  clear_sync_for_mem (sync, slot, offset, len, false);
  if (killed != NULL)
    clear_sync_for_mem (killed, slot, offset, len, true);
  reg_loc = find_reg_loc (hard_regno, 1);
  mem_loc = find_mem_loc (slot, offset, len);
  yara_assert (reg_loc != NULL && mem_loc != NULL);
  reg_index = reg_loc->loc_num;
  mem_index = mem_loc->loc_num;
  for (i = 0; i < reg_locs_num; i++)
#ifdef SYNC_BITMAP
    if (bitmap_bit_p (sync, reg_index * locs_num + i))
#else
    if (TEST_BIT (sync, reg_index * locs_num + i))
#endif
      set_sync (sync, i, mem_index);
  set_sync (sync, reg_index, mem_index);
}

static bool
#ifdef SYNC_BITMAP
check_sync_and_setup_copy_sync (bitmap sync,
				struct loc *loc1, struct loc *loc2,
				bool mem_subst_p)
#else
check_sync_and_setup_copy_sync (sbitmap sync,
				struct loc *loc1, struct loc *loc2,
				bool mem_subst_p ATTRIBUTE_UNUSED)
#endif
{
  int index1, index2;

  yara_assert (loc1 != NULL && loc2 != NULL);
  index1 = loc1->loc_num;
  index2 = loc2->loc_num;
  yara_assert (index1 < reg_locs_num);
#ifdef SYNC_BITMAP
  if (bitmap_bit_p (sync, index1 * locs_num + index2))
#else
  if (TEST_BIT (sync, index1 * locs_num + index2))
#endif
    return true;
  return false;
}

static bool set_copy_sync_p, synced_p, processed_p;
static int subst_hard_regno;
#ifdef SYNC_BITMAP
static bitmap sync_set, killed_set;
#else
static sbitmap sync_set, killed_set;
#endif

static void
process_reg_reg_loc_copy_for_sync (int dst_hard_regno, int dst_nregs,
				   int src_hard_regno, int src_nregs)
{
  struct loc *src_loc;

  processed_p = true;
  src_loc = find_reg_loc (src_hard_regno, src_nregs);
  if (set_copy_sync_p)
    synced_p = check_sync_and_setup_copy_sync
               (sync_set, find_reg_loc (dst_hard_regno, dst_nregs),
		src_loc, false) && synced_p;
  set_sync_for_reg (sync_set, killed_set, dst_hard_regno, dst_nregs, src_loc);
}

static void
process_reg_mem_loc_copy_for_sync (bool dst_reg_p, int hard_regno,
				   struct memory_slot *memory_slot,
				   int offset, int len)
{
  processed_p = true;
  if (set_copy_sync_p)
    synced_p = check_sync_and_setup_copy_sync
               (sync_set, find_reg_loc (hard_regno, 1),
		find_mem_loc (memory_slot, offset, len), false) && synced_p;
  if (dst_reg_p)
    set_sync_for_reg (sync_set, killed_set, hard_regno, 1,
		      find_mem_loc (memory_slot, offset, len));
  else
    set_sync_for_mem (sync_set, killed_set, hard_regno,
		      memory_slot, offset, len);
}

static void
process_reg_loc_set_for_sync (int hard_regno)
{
  clear_sync_for_reg (sync_set, hard_regno, false);
  if (killed_set != NULL)
    clear_sync_for_reg (killed_set, hard_regno, true);
}

static void
process_mem_loc_set_for_sync (struct memory_slot *memory_slot, int offset,
			      int len)
{
  clear_sync_for_mem (sync_set, memory_slot, offset, len, false);
  if (killed_set != NULL)
    clear_sync_for_mem (killed_set, memory_slot, offset, len, true);
}

static void
process_pre_copy_for_sync (rtx insn ATTRIBUTE_UNUSED,
			   copy_t cp ATTRIBUTE_UNUSED)
{
  processed_p = false;
  synced_p = set_copy_sync_p;
  subst_hard_regno = -1;
}

static void
process_all_reg_memory_copy_for_sync (copy_t cp, enum machine_mode cp_mode,
				      int dst_hard_regno,
				      struct memory_slot *src_memory_slot,
				      int src_offset)
{
  int hard_regno, nregs, i, part_size, best_cost = 0, cost;
  int reg_index, mem_index, index;
  struct loc *mem_loc, *reg_loc;
  enum reg_class dst_class, src_class;
  allocno_t src;

  processed_p = true;
  if (! set_copy_sync_p || synced_p || cp == NULL)
    return;
  src = COPY_SRC (cp);
  yara_assert (src != NULL && ALLOCNO_MEMORY_SLOT (src) != NULL);
  if (ALLOCNO_TYPE (src) == INSN_ALLOCNO
      && INSN_ALLOCNO_TIED_ALLOCNO (src) != NULL)
    return;
  dst_class = REGNO_REG_CLASS (dst_hard_regno);
  nregs = hard_regno_nregs [dst_hard_regno] [cp_mode];
  part_size = GET_MODE_SIZE (cp_mode) / nregs;
  yara_assert (GET_MODE_SIZE (cp_mode) % nregs == 0);
  mem_loc = find_mem_loc (src_memory_slot, src_offset, part_size);
  mem_index = mem_loc->loc_num;
  for (reg_index = 0; reg_index < reg_locs_num; reg_index++)
#ifdef SYNC_BITMAP
    if (bitmap_bit_p (sync_set, reg_index * locs_num + mem_index))
#else
    if (TEST_BIT (sync_set, reg_index * locs_num + mem_index))
#endif
      {
	hard_regno = locs [reg_index]->start;
	if (dst_hard_regno == hard_regno)
	  continue;
	for (i = 1; i < nregs; i++)
	  {
	    reg_loc = find_reg_loc (dst_hard_regno + i, 1);
	    index = reg_loc->loc_num;
	    mem_loc = find_mem_loc (src_memory_slot,
				    src_offset + i * part_size, part_size);
	    mem_index = mem_loc->loc_num;
#ifdef SYNC_BITMAP
	    if (bitmap_bit_p (sync_set, index * locs_num + mem_index))
	      break;
#else
	    if (TEST_BIT (sync_set, index * locs_num + mem_index))
	      break;
#endif
	  }
	if (i < nregs)
	  continue;
	if (! HARD_REGNO_MODE_OK (hard_regno, cp_mode))
	  continue;
	src_class = REGNO_REG_CLASS (hard_regno);
#ifdef SECONDARY_MEMORY_NEEDED
	if (SECONDARY_MEMORY_NEEDED (src_class, dst_class, cp_mode))
	  continue;
#endif
	/* secondary reload not necessary ??? */
	cost = REGISTER_MOVE_COST (cp_mode, src_class, dst_class);
	if (subst_hard_regno < 0 || best_cost > cost)
	  {
	    subst_hard_regno = hard_regno;
	    best_cost = cost;
	  }
      }
}

static void
process_post_copy_for_sync (rtx insn, copy_t cp)
{
  if (! processed_p)
    return;
  if (synced_p)
    {
      if (insn != NULL_RTX)
	{
	  if ((YARA_PARAMS & YARA_NO_MOVE_SYNC) == 0)
	    {
	      if (yara_dump_file != NULL)
		fprintf (yara_dump_file,
			 "Removing sync insn %d\n", INSN_UID (insn));
	      bitmap_set_bit (insn_to_be_removed_p, INSN_UID (insn));
	    }
	}
      else
	{
	  COPY_SYNC_P (cp) = true;
	  if (yara_dump_file != NULL)
	    {
	      fprintf (yara_dump_file, "copy %d (", COPY_NUM (cp));
	      print_point (yara_dump_file, &COPY_POINT (cp));
	      if (COPY_DST (cp) == NULL)
		fprintf (yara_dump_file, ")   nil<-");
	      else if (ALLOCNO_HARD_REGNO (COPY_DST (cp)) >= 0)
		fprintf (yara_dump_file, ")%5dr%d<-",
			 ALLOCNO_NUM (COPY_DST (cp)),
			 ALLOCNO_HARD_REGNO (COPY_DST (cp)));
	      else if (ALLOCNO_MEMORY_SLOT (COPY_DST (cp)) != NULL)
		fprintf (yara_dump_file, "%5dm%d<-",
			 ALLOCNO_NUM (COPY_DST (cp)),
			 ALLOCNO_MEMORY_SLOT (COPY_DST (cp))->start
			 + ALLOCNO_MEMORY_SLOT_OFFSET (COPY_DST (cp)));
	      if (COPY_SRC (cp) == NULL)
		fprintf (yara_dump_file, "   nil");
	      else if (ALLOCNO_HARD_REGNO (COPY_SRC (cp)) >= 0)
		fprintf (yara_dump_file, "%5dr%d", ALLOCNO_NUM (COPY_SRC (cp)),
			 ALLOCNO_HARD_REGNO (COPY_SRC (cp)));
	      else if (ALLOCNO_MEMORY_SLOT (COPY_SRC (cp)) != NULL)
		fprintf (yara_dump_file, "%5dm%d", ALLOCNO_NUM (COPY_SRC (cp)),
			 ALLOCNO_MEMORY_SLOT (COPY_SRC (cp))->start
			 + ALLOCNO_MEMORY_SLOT_OFFSET (COPY_SRC (cp)));
	      fprintf (yara_dump_file, " is synchronizied\n");
	    }
	}
    }
  else if (subst_hard_regno >= 0)
    {
      yara_assert (cp != NULL);
      if (yara_dump_file != NULL)
	{
	  fprintf (yara_dump_file, "In copy %d (", COPY_NUM (cp));
	  print_point (yara_dump_file, &COPY_POINT (cp));
	  if (COPY_DST (cp) == NULL)
	    fprintf (yara_dump_file, ")   nil(%3d)<-",
		     ALLOCNO_REGNO (COPY_SRC (cp)));
	  else
	    fprintf (yara_dump_file, ")%5d(%3d)<-",
		     ALLOCNO_NUM (COPY_DST (cp)),
		     ALLOCNO_HARD_REGNO (COPY_DST (cp)));
	  fprintf (yara_dump_file, "%5d", ALLOCNO_NUM (COPY_SRC (cp)));
	  fprintf (yara_dump_file, " memory is changed by %d\n",
		   subst_hard_regno);
	}
      unassign_copy (cp);
      COPY_SUBST_SRC_HARD_REGNO (cp) = subst_hard_regno;
    }
}

/* The function calculates local info for each basic block.  */

static void
calculate_local_reg_bb_info (void)
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      set_copy_sync_p = false;
      sync_set = BB_INFO (bb)->syncloc;
      killed_set = BB_INFO (bb)->killed;
      process_bb_locs (bb);
    }
}

/* The function sets up reverse post-order number of each basic
   block.  */

static void
setup_bb_rts_numbers (void)
{
  int i;
  int *rts_order;
  
  rts_order = xmalloc (sizeof (int) * (n_basic_blocks - NUM_FIXED_BLOCKS));
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

/* The function calculates memory register synchronization.  The
   function calculates sync info at the start and end of basic block
   E->dst by propagating synch info at end of predecessor basic block
   E->src.  The function returns true if the partial availability at
   the end of BB has been changed or if CHANGED_P.  We have the
   following equations:

     bb.syncin = empty for entry block | intersection (syncout of predecessors)
     bb.syncout = union (bb.syncin - b.killed, bb.syncloc)  */

static bool
modify_bb_reg_sync (edge e, bool changed_p)
{
  basic_block bb, pred;
  struct bb_info *bb_info;
#ifdef SYNC_BITMAP
  bitmap bb_syncin, bb_syncout;
#else
  sbitmap bb_syncin, bb_syncout;
#endif

  bb = e->dest;
  pred = e->src;
  bb_info = BB_INFO (bb);
  bb_syncin = bb_info->syncin;
  bb_syncout = bb_info->syncout;
  if (pred->index != ENTRY_BLOCK && BB_INFO (pred)->syncout_defined_p)
    {
#ifdef SYNC_BITMAP
      bitmap_copy (temp_sync, BB_INFO (pred)->syncout);
#else
      sbitmap_copy (temp_sync, BB_INFO (pred)->syncout);
#endif
      process_copy_locs (at_edge_start_copies [(size_t) e->aux]);
      process_copy_locs (at_edge_end_copies [(size_t) e->aux]);
      if (bb_info->syncin_defined_p)
#ifdef SYNC_BITMAP
	bitmap_and_into (bb_syncin, temp_sync);
#else
	sbitmap_a_and_b (bb_syncin, bb_syncin, temp_sync);
#endif
      else
#ifdef SYNC_BITMAP
	bitmap_copy (bb_syncin, temp_sync);
#else
        sbitmap_copy (bb_syncin, temp_sync);
#endif
      bb_info->syncin_defined_p = true;
    }
#ifdef SYNC_BITMAP
  changed_p |= (bitmap_ior_and_compl (bb_syncout, bb_info->syncloc,
				      bb_syncin, bb_info->killed)
		|| ! bb_info->syncout_defined_p);
#else
  changed_p |= (sbitmap_union_of_diff_cg (bb_syncout, bb_info->syncloc,
					  bb_syncin, bb_info->killed)
		|| ! bb_info->syncout_defined_p);
#endif
  bb_info->syncout_defined_p = true;
  return changed_p;
}

/* The function calculates register sync.  */

static void
calculate_reg_sync (void)
{
  basic_block bb, succ;
  edge_iterator ei;
  edge e;
  bool changed_p;
  int i, nel;
  varray_type bbs, new_bbs, temp;
  basic_block *bb_array;
  sbitmap wset;

  set_copy_sync_p = false;
  sync_set = temp_sync;
  killed_set = NULL;
  VARRAY_BB_INIT (bbs, n_basic_blocks, "basic blocks");
  VARRAY_BB_INIT (new_bbs, n_basic_blocks, "basic blocks for the next iter.");
  FOR_EACH_BB (bb)
    {
      VARRAY_PUSH_BB (bbs, bb);
    }
  wset = sbitmap_alloc (n_basic_blocks + 1);
  while (VARRAY_ACTIVE_SIZE (bbs))
    {
      bb_array = &VARRAY_BB (bbs, 0);
      nel = VARRAY_ACTIVE_SIZE (bbs);
      qsort (bb_array, nel, sizeof (basic_block), rpost_cmp);
      sbitmap_zero (wset);
      for (i = 0; i < nel; i++)
	{
	  bb = bb_array [i];
	  changed_p = 0;
	  FOR_EACH_EDGE (e, ei, bb->preds)
	    changed_p = modify_bb_reg_sync (e, changed_p);
	  if (changed_p)
	    FOR_EACH_EDGE (e, ei, bb->succs)
	      {
		succ = e->dest;
		if (succ->index != EXIT_BLOCK && !TEST_BIT (wset, succ->index))
		  {
		    SET_BIT (wset, succ->index);
		    VARRAY_PUSH_BB (new_bbs, succ);
		  }
	      }
	}
      temp = bbs;
      bbs = new_bbs;
      new_bbs = temp;
      VARRAY_POP_ALL (new_bbs);
    }
  sbitmap_free (wset);
}

static void
setup_copy_sync_p (void)
{
  basic_block bb;
  edge_iterator ei;
  edge e;

  FOR_EACH_BB (bb)
    {
#ifdef SYNC_BITMAP
      bitmap_copy (temp_sync, BB_INFO (bb)->syncin);
#else
      sbitmap_copy (temp_sync, BB_INFO (bb)->syncin);
#endif
      
      set_copy_sync_p = true;
      sync_set = temp_sync;
      killed_set = NULL;
      process_bb_locs (bb);
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
#ifdef SYNC_BITMAP
	  bitmap_copy (temp_sync, BB_INFO (bb)->syncout);
#else
	  sbitmap_copy (temp_sync, BB_INFO (bb)->syncout);
#endif
	  process_copy_locs (at_edge_start_copies [(size_t) e->aux]);
	  process_copy_locs (at_edge_end_copies [(size_t) e->aux]);
	}
    }
}

/* The following function makes live information more accurate by
   modifying global_live_at_start and global_live_at_end of basic
   blocks.  After the function call a register lives at a program
   point only if it is initialized on a path from CFG entry to the
   program point.  The standard GCC life analysis permits registers to
   live uninitialized.  */

static void
find_sync_copies (void)
{
  process_reg_reg_loc_copy_func = process_reg_reg_loc_copy_for_sync;
  process_reg_mem_loc_copy_func = process_reg_mem_loc_copy_for_sync;
  process_reg_loc_set_func = process_reg_loc_set_for_sync;
  process_mem_loc_set_func = process_mem_loc_set_for_sync;
  process_pre_copy_func = process_pre_copy_for_sync;
  process_all_reg_memory_copy_func = process_all_reg_memory_copy_for_sync;
  process_post_copy_func = process_post_copy_for_sync;
  initiate_reg_sync_info ();
  calculate_local_reg_bb_info ();
  setup_bb_rts_numbers ();
  calculate_reg_sync ();
  setup_copy_sync_p ();
  finish_reg_sync_info ();
}



static void
get_reg_set_and_memory_slots (copy_t cp, HARD_REG_SET *regs,
			      struct memory_slot **slot1,
			      struct memory_slot **slot2,
			      struct memory_slot **slot3)
{
  allocno_t src, dst;

  src = COPY_SRC (cp);
  dst = COPY_DST (cp);
  if (src == NULL)
    CLEAR_HARD_REG_SET (*regs);
  else
    {
      COPY_HARD_REG_SET (*regs, ALLOCNO_HARD_REGSET (src));
      if (ALLOCNO_TYPE (src) == INSN_ALLOCNO)
	{
	  IOR_HARD_REG_SET (*regs,
			    INSN_ALLOCNO_INTERM_ELIMINATION_REGSET (src));
	  if (INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (src)
	      && ALLOCNO_REGNO (src) >= 0)
	    {
	      yara_assert (HARD_REGISTER_NUM_P (ALLOCNO_REGNO (src)));
	      IOR_HARD_REG_SET (*regs,
				reg_mode_hard_regset 
				[ALLOCNO_REGNO (src)] [ALLOCNO_MODE (src)]);
	    }
	}
    }
  if (dst == NULL)
    {
      IOR_HARD_REG_SET (*regs, ALLOCNO_HARD_REGSET (dst));
      if (ALLOCNO_TYPE (dst) == INSN_ALLOCNO)
	{
	  IOR_HARD_REG_SET (*regs,
			    INSN_ALLOCNO_INTERM_ELIMINATION_REGSET (dst));
	  if (INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (dst)
	      && ALLOCNO_REGNO (dst) >= 0)
	    {
	      yara_assert (HARD_REGISTER_NUM_P (ALLOCNO_REGNO (dst)));
	      IOR_HARD_REG_SET (*regs,
				reg_mode_hard_regset 
				[ALLOCNO_REGNO (dst)] [ALLOCNO_MODE (dst)]);
	    }
	}
    }
  if (COPY_CHANGE_ADDR (cp) != NULL)
    IOR_HARD_REG_SET (*regs, COPY_INTERM_SCRATCH_HARD_REGSET (cp));
  *slot1 = (src == NULL ? NULL : ALLOCNO_MEMORY_SLOT (src));
  *slot2 = (dst == NULL ? NULL : ALLOCNO_MEMORY_SLOT (dst));
  *slot3 = NULL;
#ifdef SECONDARY_MEMORY_NEEDED
  if (COPY_CHANGE_ADDR (cp) != NULL)
    *slot3 = COPY_MEMORY_SLOT (cp);
#endif
}

static bool
copy_can_be_moved_through_copy (copy_t cp, copy_t another_cp)
{
  HARD_REG_SET regs, another_regs;
  struct memory_slot *slot1, *slot2, *slot3;
  struct memory_slot *another_slot1, *another_slot2, *another_slot3;

  get_reg_set_and_memory_slots (cp, &regs, &slot1, &slot2, &slot3);
#ifdef SECONDARY_MEMORY_NEEDED
  /* It is rare case -- don't worry about improving the code for
     that.  */
  if (COPY_CHANGE_ADDR (cp) == NULL || COPY_USER_DEFINED_MEMORY(cp) != NULL)
    return false;
#endif
  get_reg_set_and_memory_slots (another_cp, &another_regs, &another_slot1,
				&another_slot2, &another_slot3);
  AND_HARD_REG_SET (another_regs, regs);
  GO_IF_HARD_REG_EQUAL (another_regs, zero_hard_reg_set, ok);
  return false;
 ok:
  if (slot1 != NULL
      && ((another_slot1 != NULL
	   && memory_slot_intersected (slot1, another_slot1))
	  || (another_slot2 != NULL
	      && memory_slot_intersected (slot1, another_slot2))
	  || (another_slot3 != NULL
	      && memory_slot_intersected (slot1, another_slot3))))
    return false;
  if (slot2 != NULL
      && ((another_slot1 != NULL
	   && memory_slot_intersected (slot2, another_slot1))
	  || (another_slot2 != NULL
	      && memory_slot_intersected (slot2, another_slot2))
	  || (another_slot3 != NULL
	      && memory_slot_intersected (slot2, another_slot3))))
    return false;
  if (slot3 != NULL
      && ((another_slot1 != NULL
	   && memory_slot_intersected (slot3, another_slot1))
	  || (another_slot2 != NULL
	      && memory_slot_intersected (slot3, another_slot2))
	  || (another_slot3 != NULL
	      && memory_slot_intersected (slot3, another_slot3))))
    return false;
  return true;
}

static bool
copy_can_be_moved_through_insn_allocno_p (copy_t cp, allocno_t a)
{
  HARD_REG_SET regs, a_regs;
  struct memory_slot *slot1, *slot2, *slot3;
  struct memory_slot *a_slot;

  yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
  get_reg_set_and_memory_slots (cp, &regs, &slot1, &slot2, &slot3);
  COPY_HARD_REG_SET (a_regs, ALLOCNO_HARD_REGSET (a));
  IOR_HARD_REG_SET (a_regs, INSN_ALLOCNO_INTERM_ELIMINATION_REGSET (a));
  if (INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a) && ALLOCNO_REGNO (a) >= 0)
    {
      yara_assert (HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a)));
      IOR_HARD_REG_SET
	(a_regs, reg_mode_hard_regset [ALLOCNO_REGNO (a)] [ALLOCNO_MODE (a)]);
    }
  a_slot = ALLOCNO_MEMORY_SLOT (a);
  AND_HARD_REG_SET (a_regs, regs);
  GO_IF_HARD_REG_EQUAL (a_regs, zero_hard_reg_set, ok);
  return false;
 ok:
  if (a_slot != NULL
      && ((slot1 != NULL
	   && memory_slot_intersected (a_slot, slot1))
	  || (slot2 != NULL
	      && memory_slot_intersected (a_slot, slot2))
	  || (slot3 != NULL
	      && memory_slot_intersected (a_slot, slot3))))
    return false;
  return true;
}

/* Return true if TARGET_CP can be moved up through INSN.  */
static bool
copy_can_be_moved_through_insn_p (rtx insn, copy_t target_cp)
{
  copy_t cp;
  allocno_t a;

  yara_assert (target_cp != NULL);
  for (a = insn_allocnos [INSN_UID (insn)];
       a != NULL;
       a = INSN_ALLOCNO_NEXT (a))
    {
      if (! copy_can_be_moved_through_insn_allocno_p (target_cp, a))
	return false;
      for (cp = ALLOCNO_SRC_COPIES (a);
	   cp != NULL;
	   cp = COPY_NEXT_SRC_COPY (cp))
	if (! copy_can_be_moved_through_copy (target_cp, cp))
	  return false;
      for (cp = ALLOCNO_DST_COPIES (a);
	   cp != NULL;
	   cp = COPY_NEXT_DST_COPY (cp))
	if (! copy_can_be_moved_through_copy (target_cp, cp))
	  return false;
    }
  return true;
}

static rtx
eliminate_regs (rtx x)
{
  int base_regno, index_regno;
  HOST_WIDE_INT scale;
  rtx *loc, *base_reg_loc, *disp_loc, *index_reg_loc, *temp_loc;

  if (GET_CODE (x) == MEM)
    loc = &XEXP (x, 0);
  else
    loc = &x;
  temp_loc = loc;
  if (! decode_address (loc, &temp_loc,	&base_reg_loc,
			&disp_loc, &index_reg_loc, &scale, true))
    gcc_unreachable ();
  if (base_reg_loc != NULL)
    {
      base_regno = REGNO (*base_reg_loc);
      if (HARD_REGISTER_NUM_P (base_regno)
	  && reg_eliminate [base_regno] != NULL)
	*loc = get_eliminate_subst_rtx (loc, base_regno,
					reg_eliminate [base_regno]->to,
					reg_eliminate [base_regno]->offset);
    }
  if (index_reg_loc != NULL)
    {
      index_regno = REGNO (*index_reg_loc);
      if (HARD_REGISTER_NUM_P (index_regno)
	  && reg_eliminate [index_regno] != NULL)
	*loc = get_eliminate_subst_rtx (loc, index_regno,
					reg_eliminate [index_regno]->to,
					reg_eliminate [index_regno]->offset);
    }
  return x;
}

/* Offset between frame and stack memory slot area after the
   allocation.  */
static HOST_WIDE_INT final_stack_memory_start_frame_offset;

/* Size of stack slots after allocation of all necessary stack
   memory.  */
static int final_rounded_slot_memory_size;

static rtx
get_allocno_memory_slot_rtx (struct memory_slot *memory_slot, int offset,
			     enum machine_mode mode,
			     allocno_t for_a ATTRIBUTE_UNUSED)
{
  rtx mem;

  mem = memory_slot->mem;
  if (mem != NULL_RTX)
    {
      mem = eliminate_regs (mem);
      if (mode != GET_MODE (mem))
	{
	  mem = gen_rtx_SUBREG (mode, mem, offset);
	  alter_subreg (&mem);
	}
    }
  else
    {
      HOST_WIDE_INT disp;

      if (FRAME_GROWS_DOWNWARD)
	{
	  if (stack_frame_pointer_can_be_eliminated_p
	      && obligatory_stack_frame_pointer_elimination_p)
	    {
	      /* Prefer slot arragement with smaller displacement for
		 slots with smaller start (because they have higher
		 prioirity).  */
	      if (final_stack_memory_start_frame_offset
		  + frame_stack_pointer_offset <= 1)
		disp = (final_stack_memory_start_frame_offset
			+ frame_stack_pointer_offset - memory_slot->start);
	      else
		disp = (final_stack_memory_start_frame_offset
			+ frame_stack_pointer_offset + memory_slot->start
			- memory_slot->size
			- final_rounded_slot_memory_size + 2);
	    }
	  else
	    disp = (final_stack_memory_start_frame_offset
		    + frame_hard_frame_pointer_offset - memory_slot->start);
	}
      else
	{
	  if (stack_frame_pointer_can_be_eliminated_p
	      && obligatory_stack_frame_pointer_elimination_p)
	    {
	      /* Prefer slot arragement with smaller displacement for
		 slots with smaller start (because they have higher
		 prioirity).  */
	      if (final_stack_memory_start_frame_offset
		  + frame_stack_pointer_offset >= -1)
		disp = (final_stack_memory_start_frame_offset
			+ frame_stack_pointer_offset + memory_slot->start);
	      else
		disp = (final_stack_memory_start_frame_offset
			+ frame_stack_pointer_offset - memory_slot->start
			- memory_slot->size + final_rounded_slot_memory_size);
	    }
	  else
	    disp = (final_stack_memory_start_frame_offset
		    + frame_hard_frame_pointer_offset + memory_slot->start);
	}
      mem = gen_rtx_MEM (mode,
			 gen_rtx_PLUS
			 (Pmode,
			  (stack_frame_pointer_can_be_eliminated_p
			   && obligatory_stack_frame_pointer_elimination_p
			   ? stack_pointer_rtx : hard_frame_pointer_rtx),
			  gen_rtx_CONST_INT (VOIDmode, disp + offset)));
    }
#if 0
  if (for_a != NULL)
    {
      int regno;
      
      if ((regno = ALLOCNO_REGNO (for_a)) >= 0
	  && ! HARD_REGISTER_NUM_P (regno))
	set_mem_attrs_from_reg (mem, regno_reg_rtx [regno]);
      else if (ALLOCNO_TYPE (for_a) == INSN_ALLOCNO
	       && REG_P (*INSN_ALLOCNO_LOC (for_a)))
	set_mem_attrs_from_reg (mem, *INSN_ALLOCNO_LOC (for_a));
    }
#endif
  return mem;
}

static rtx
gen_allocno_reg_rtx (enum machine_mode mode, int hard_regno, allocno_t for_a)
{
  rtx new_reg;

  new_reg = gen_rtx_REG (mode, hard_regno);
  /* We don't set attributes from MEM because it is used temporarily
     in the insn.  */
  if (for_a != NULL && (ALLOCNO_TYPE (for_a) != INSN_ALLOCNO
			|| ! MEM_P (*INSN_ALLOCNO_LOC (for_a))))
    {
      tree expr;
      int regno;
      rtx x, from_reg = NULL_RTX;

      if ((regno = ALLOCNO_REGNO (for_a)) >= 0
	  && ! HARD_REGISTER_NUM_P (regno))
	from_reg = regno_reg_rtx [regno];
      else if (ALLOCNO_TYPE (for_a) == INSN_ALLOCNO)
	{
	  SKIP_TO_REG (x, *INSN_ALLOCNO_LOC (for_a));
	  if (REG_P (x))
	    from_reg = x;
	}
      if (from_reg != NULL_RTX)
	{
	  REG_USERVAR_P (new_reg) = REG_USERVAR_P (from_reg);
	  REG_POINTER (new_reg) = REG_POINTER (from_reg);
	  REG_ATTRS (new_reg) = REG_ATTRS (from_reg);
	  ORIGINAL_REGNO (new_reg) = ORIGINAL_REGNO (from_reg);
	  if ((expr = REG_EXPR (from_reg)) != NULL
	      && DECL_P (expr) && DECL_RTL (expr) != NULL
	      && REG_P (DECL_RTL (expr)))
	    SET_DECL_RTL (expr, new_reg);
	}
    }
  return new_reg;
}

#ifdef ENABLE_YARA_CHECKING
static allocno_t curr_emit_copy_dst, curr_emit_copy_src;
static HARD_REG_SET emit_copy_dst_hard_regs;
#endif

static void
emit_move (rtx dst, rtx src)
{
  enum machine_mode mode;

  mode = GET_MODE (src);
  yara_assert (mode == VOIDmode || GET_MODE (dst) == VOIDmode
	       || GET_MODE_SIZE (mode) == GET_MODE_SIZE (GET_MODE (dst)));
#if 0
#ifdef ENABLE_YARA_CHECKING
 {
   int i, dst_regno, src_regno;

   if (GET_CODE (src) == REG && (src_regno = REGNO (src)) >= 0
       && HARD_REGISTER_NUM_P (src_regno))
     for (i = hard_regno_nregs [src_regno] [mode] - 1; i >= 0; i--)
       if (TEST_HARD_REG_BIT (emit_copy_dst_hard_regs, src_regno + i)
	   && (hard_regno_emit_copy_dst_map [src_regno + i]
	       != curr_emit_copy_src))
	 gcc_unreachable ();
   if (GET_CODE (dst) == REG && (dst_regno = REGNO (dst)) >= 0
       && HARD_REGISTER_NUM_P (dst_regno))
     for (i = hard_regno_nregs [dst_regno] [mode] - 1; i >= 0; i--)
       {
	 SET_HARD_REG_BIT (emit_copy_dst_hard_regs, dst_regno + i);
	 hard_regno_emit_copy_dst_map [dst_regno + i] = curr_emit_copy_dst;
       }
 }
#endif
#endif
  emit_move_insn (dst, src);
}

static bool
emit_secondary_memory_mode_move (rtx dst_rtx, rtx src_rtx, copy_t cp,
				 enum machine_mode mode)
{
  rtx last = get_last_insn ();

  yara_assert (mode != VOIDmode
	       && (GET_MODE (src_rtx) == VOIDmode
		   || GET_MODE (dst_rtx) == VOIDmode
		   || GET_MODE (src_rtx) == GET_MODE (dst_rtx)));
  /* ??? Another offset for memory if using nother mode.  */
  if (GET_MODE (src_rtx) != mode && GET_MODE (src_rtx) != VOIDmode)
    {
      src_rtx = copy_rtx (src_rtx);
      PUT_MODE (src_rtx, mode);
    }
  if (GET_MODE (dst_rtx) != mode && GET_MODE (dst_rtx) != VOIDmode)
    {
      dst_rtx = copy_rtx (dst_rtx);
      PUT_MODE (dst_rtx, mode);
    }
#ifdef SECONDARY_MEMORY_NEEDED
  /* Although we see that we have allocated secondary memory, it is
     not enough that we need a secondary memory move because when the
     secondary reload register needed we have two possible register
     moves possibly through secondary memory.  */
  if (REG_P (src_rtx) && REG_P (dst_rtx)
      && SECONDARY_MEMORY_NEEDED (REGNO_REG_CLASS (REGNO (src_rtx)),
				  REGNO_REG_CLASS (REGNO (dst_rtx)),
				  GET_MODE (src_rtx)))
    {
      rtx mem;

      yara_assert (COPY_CHANGE_ADDR (cp) != NULL);
      if (COPY_USER_DEFINED_MEMORY (cp) != NULL_RTX)
	mem = COPY_USER_DEFINED_MEMORY (cp);
      else if (COPY_MEMORY_SLOT (cp) != NULL)
	mem = get_allocno_memory_slot_rtx (COPY_MEMORY_SLOT (cp), 0,
					   mode, NULL);
      else
	gcc_unreachable ();
      if (GET_MODE (mem) != mode)
	{
	  mem = copy_rtx (mem);
	  PUT_MODE (mem, mode);
	}
      emit_move (mem, src_rtx);
      emit_move (dst_rtx, mem);
    }
  else
#endif
    emit_move (dst_rtx, src_rtx);
  if (check_insns_added_since (last))
    return true;
  delete_insns_since (last);
  return false;
}

static void
emit_secondary_memory_move (rtx dst_rtx, rtx src_rtx, copy_t cp,
			    enum machine_mode alt_mode_1,
			    enum machine_mode alt_mode_2,
			    bool try_spill_mode_p)
{
  enum machine_mode mode = GET_MODE (dst_rtx);

  if (emit_secondary_memory_mode_move (dst_rtx, src_rtx, cp, mode))
    return;
  if (alt_mode_1 != VOIDmode && mode != alt_mode_1)
    {
      if (emit_secondary_memory_mode_move (dst_rtx, src_rtx, cp, alt_mode_1))
	return;
    }
  if (alt_mode_2 != VOIDmode && mode != alt_mode_2
      && alt_mode_2 != alt_mode_1)
    {
      if (emit_secondary_memory_mode_move (dst_rtx, src_rtx, cp, alt_mode_2))
	return;
    }
  if (try_spill_mode_p && mode != spill_mode [mode]
      && emit_secondary_memory_mode_move (dst_rtx, src_rtx, cp,
					  spill_mode [mode]))
    return;
  gcc_unreachable ();
}


static rtx
get_eliminate_subst_rtx (rtx *loc, int from, int to, HOST_WIDE_INT offset)
{
  int base_regno, index_regno;
  HOST_WIDE_INT scale;
  rtx *base_reg_loc, *disp_loc, disp, old_disp, *index_reg_loc;
  rtx *temp_loc, reg;
  bool base_p;
  enum rtx_code code;

  temp_loc = loc;
  if (! decode_address (loc, &temp_loc, &base_reg_loc, &disp_loc,
			&index_reg_loc, &scale, true))
    gcc_unreachable ();
  old_disp = disp = (disp_loc == NULL ? NULL_RTX : *disp_loc);
  yara_assert (temp_loc == loc);
  base_regno = index_regno = -1;
  if (base_reg_loc != NULL)
    base_regno = REGNO (*base_reg_loc);
  if (index_reg_loc != NULL)
    index_regno = REGNO (*index_reg_loc);
  yara_assert (base_regno >= 0 || index_regno >= 0);
  if (base_regno == from)
    {
      base_p = true;
      base_regno = to;
    }
  else if (index_regno == from)
    {
      base_p = false;
      index_regno = to;
      offset *= scale;
    }
  else
    {
      /* ??? should we better to unshare all rtx.  */
      /* it happens when the address has been changed already because
	 the address is shared.  */
      yara_assert (base_regno == to || index_regno == to);
      return *loc;
    }
  if (offset == 0)
    ;
  else if (disp == NULL_RTX)
    disp = gen_rtx_CONST_INT (VOIDmode, offset);
  else if ((code = GET_CODE (disp)) == CONST_INT)
    disp = gen_rtx_CONST_INT (VOIDmode, INTVAL (disp) + offset);
  else if (code == SYMBOL_REF || code == LABEL_REF)
    disp = gen_rtx_CONST (Pmode,
			  gen_rtx_PLUS (Pmode, disp,
					gen_rtx_CONST_INT (VOIDmode, offset)));
  else if (code == CONST)
    {
      code = GET_CODE (XEXP (disp, 0));
      if (code == PLUS && GET_CODE (XEXP (XEXP (disp, 0), 1)) == CONST_INT)
	XEXP (XEXP (disp, 0), 1)
	  = gen_rtx_CONST_INT (VOIDmode,
			       INTVAL (XEXP (XEXP (disp, 0), 1)) + offset);
      else
	XEXP (disp, 0) = gen_rtx_PLUS (Pmode, XEXP (disp, 0),
				       gen_rtx_CONST_INT (VOIDmode, offset));
    }
  else
    gcc_unreachable ();
  reg = gen_rtx_REG (Pmode, base_p ? base_regno : index_regno);
  if (base_p)
    *base_reg_loc = reg;
  else
    *index_reg_loc = reg;
  /* We need to save locations for base and index registers because
     there might be a reference from another allocno.  */
  if (base_regno < 0 || index_regno < 0)
    {
      yara_assert ((base_regno < 0 && scale != 1) || index_regno < 0);
      if (disp == NULL_RTX)
	{
	  yara_assert (old_disp == NULL_RTX);
	  return *loc;
	}
      else if (old_disp == NULL_RTX)
	return gen_rtx_PLUS (Pmode, *loc, disp);
      else
	{
	  yara_assert (GET_CODE (XEXP (*loc, 1)) == CONST_INT);
	  *disp_loc = disp;
	  return *loc;
	}
    }
  else if (old_disp == NULL_RTX)
    {
      yara_assert (GET_CODE (*loc) == PLUS);
      return (disp == NULL_RTX ? *loc : gen_rtx_PLUS (Pmode, *loc, disp));
    }
  else
    {
      yara_assert (GET_CODE (*loc) == PLUS);
      *disp_loc = disp;
      return *loc;
    }
}

/* The following function emits correct code assigning hard register
   value (value of SRC_REG_RTX) plus OFFSET to hard register DST_REGNO
   which denotes allocno DST.  */

static void
emit_plus_assign (int dst_regno, allocno_t dst, rtx src_reg_rtx,
		  HOST_WIDE_INT offset)
{
  rtx last = get_last_insn ();
  rtx dst_reg_rtx, offset_rtx;
  
  dst_reg_rtx = gen_allocno_reg_rtx (Pmode, dst_regno, dst);
  offset_rtx = gen_rtx_CONST_INT (VOIDmode, offset);
  emit_move (dst_reg_rtx, gen_rtx_PLUS (Pmode, src_reg_rtx, offset_rtx));
  if (check_insns_added_since (last))
    return;
  delete_insns_since (last);
  emit_move (dst_reg_rtx, offset_rtx);
  emit_move (dst_reg_rtx, gen_rtx_PLUS (Pmode, dst_reg_rtx, src_reg_rtx));
  if (check_insns_added_since (last))
    return;
  gcc_unreachable ();
}

static void
emit_copy (copy_t cp)
{
  rtx src_rtx, dst_rtx;
  enum machine_mode alt_mode_1, alt_mode_2;
  bool try_spill_mode_p = false;
  allocno_t src = COPY_SRC (cp);
  allocno_t dst = COPY_DST (cp);

#ifdef ENABLE_YARA_CHECKING
  curr_emit_copy_dst = dst;
  curr_emit_copy_src = src;
#endif
  alt_mode_1 = alt_mode_2 = VOIDmode;
  if (src == NULL)
    {
      int regno, hard_regno, interm_elimination_regno;
      struct reg_eliminate *elim;
      rtx elimination_subst_reg;
      HOST_WIDE_INT offset;
      rtx *loc;

      yara_assert (ALLOCNO_TYPE (dst) == INSN_ALLOCNO
		   && COPY_SUBST_SRC_HARD_REGNO (cp) < 0
		   && (INSN_ALLOCNO_OP_MODE (dst) == OP_IN
		       || INSN_ALLOCNO_OP_MODE (dst) == OP_INOUT));
      src_rtx = *INSN_ALLOCNO_LOC (dst);
      yara_assert (! REG_P (src_rtx) || HARD_REGISTER_NUM_P (REGNO (src_rtx)));
      hard_regno = ALLOCNO_HARD_REGNO (dst);
      elim = INSN_ALLOCNO_ELIMINATION (dst);
      if (elim != NULL)
	{
	  yara_assert (ALLOCNO_MEMORY_SLOT (dst) == NULL);
#ifdef HAVE_SECONDARY_RELOADS
	  yara_assert (COPY_CHANGE_ADDR (cp) == NULL
		       || (COPY_INTERM_REGNO (cp) < 0
			   && COPY_SCRATCH_REGNO (cp) < 0));
#endif
#ifdef SECONDARY_MEMORY_NEEDED
	  yara_assert (COPY_CHANGE_ADDR (cp) == NULL
		       || (COPY_MEMORY_SLOT (cp) == NULL
			   && COPY_USER_DEFINED_MEMORY (cp) == NULL_RTX));
#endif
	  elimination_subst_reg = gen_allocno_reg_rtx (Pmode, elim->to, dst);
	  offset = elim->offset;
	  interm_elimination_regno
	    = INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (dst);
	  regno = ALLOCNO_REGNO (dst);
	  loc = INSN_ALLOCNO_LOC (dst);
	  if (hard_regno < 0)
	    {
	      yara_assert (interm_elimination_regno >= 0);
	      emit_plus_assign (interm_elimination_regno, dst,
				elimination_subst_reg, offset);
	      return;
	    }
	  else if (interm_elimination_regno < 0)
	    {
	      if (regno >= 0)
		src_rtx = gen_rtx_PLUS (Pmode, elimination_subst_reg,
					gen_rtx_CONST_INT (VOIDmode, offset));
	      else
		src_rtx = get_eliminate_subst_rtx (loc, elim->from,
						   elim->to, offset);
	      dst_rtx = gen_allocno_reg_rtx (Pmode, hard_regno, dst);
	    }
	  else if (hard_regno == interm_elimination_regno && regno >= 0)
	    {
	      emit_plus_assign (hard_regno, dst,
				elimination_subst_reg, offset);
	      return;
	    }
	  else
	    {
	      emit_plus_assign (interm_elimination_regno, dst,
				elimination_subst_reg, offset);
	      dst_rtx = gen_allocno_reg_rtx (Pmode, hard_regno, dst);
	      if (regno >= 0)
		src_rtx
		  = gen_allocno_reg_rtx (Pmode, interm_elimination_regno, dst);
	      else
		src_rtx = get_eliminate_subst_rtx (loc, elim->from,
						   interm_elimination_regno,
						   offset);
	    }
	}
      else
	{
	  int hard_regno, offset;
	  struct memory_slot *memory_slot;
	  enum machine_mode cp_mode;

	  get_copy_loc (cp, true, &cp_mode, &hard_regno,
			&memory_slot, &offset);
	  if (hard_regno >= 0)
	    src_rtx = gen_allocno_reg_rtx (cp_mode, hard_regno, dst);
	  else if (memory_slot != NULL)
	    src_rtx = get_allocno_memory_slot_rtx (memory_slot, offset,
						   cp_mode, dst);
	  else if (offset != 0)
	    src_rtx = gen_rtx_SUBREG (cp_mode,
				      *INSN_ALLOCNO_LOC (dst), offset);
	  else
	    src_rtx = *INSN_ALLOCNO_LOC (dst);
	  get_copy_loc (cp, false, &cp_mode, &hard_regno,
			&memory_slot, &offset);
	  if (hard_regno >= 0)
	    dst_rtx = gen_allocno_reg_rtx (cp_mode, hard_regno, dst);
	  else if (memory_slot != NULL)
	    dst_rtx = get_allocno_memory_slot_rtx (memory_slot, offset,
						   cp_mode, dst);
	  else
	    gcc_unreachable ();
	}
    }
  else if (dst == NULL)
    {
      int hard_regno, offset;
      struct memory_slot *memory_slot;
      enum machine_mode cp_mode;

      /* Memory and substitution ??? */
      yara_assert (ALLOCNO_TYPE (src) == INSN_ALLOCNO
		   && COPY_SUBST_SRC_HARD_REGNO (cp) < 0
		   && (INSN_ALLOCNO_OP_MODE (src) == OP_OUT
		       || INSN_ALLOCNO_OP_MODE (src) == OP_INOUT));
      get_copy_loc (cp, true, &cp_mode, &hard_regno, &memory_slot, &offset);
      if (hard_regno >= 0)
	src_rtx = gen_allocno_reg_rtx (cp_mode, hard_regno, src);
      else if (memory_slot != NULL)
	src_rtx = get_allocno_memory_slot_rtx (memory_slot, offset, cp_mode,
					       src);
      else
	gcc_unreachable ();

      get_copy_loc (cp, false, &cp_mode, &hard_regno, &memory_slot, &offset);
      if (hard_regno >= 0)
	dst_rtx = gen_allocno_reg_rtx (cp_mode, hard_regno, dst);
      else if (memory_slot != NULL)
	dst_rtx = get_allocno_memory_slot_rtx (memory_slot, offset, cp_mode,
					       dst);
      else if (offset != 0)
	dst_rtx = gen_rtx_SUBREG (cp_mode, *INSN_ALLOCNO_LOC (src), offset);
      else
	dst_rtx = *INSN_ALLOCNO_LOC (src);
    }
  else
    {
      int hard_regno, to, offset;
      HOST_WIDE_INT elim_offset;
      struct memory_slot *memory_slot;
      enum machine_mode cp_mode;
      rtx x;

      try_spill_mode_p = true;
      yara_assert (ALLOCNO_REGNO (src) >= 0
		   && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (src))
		   && ALLOCNO_REGNO (dst) >= 0
		   && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (dst)));
      yara_assert (! ALLOCNO_USE_EQUIV_CONST_P (dst));
      if (ALLOCNO_USE_EQUIV_CONST_P (src))
	{
	  x = reg_equiv_constant [ALLOCNO_REGNO (dst)];
	  if (CONSTANT_P (x))
	    src_rtx = x;
	  else
	    {
	      yara_assert (GET_MODE_SIZE (GET_MODE (x))
			   == GET_MODE_SIZE (Pmode));
	      get_equiv_const_elimination_info (x, &to, &elim_offset);
	      src_rtx
		= gen_rtx_PLUS (GET_MODE (x),
				gen_allocno_reg_rtx (GET_MODE (x), to, dst),
				gen_rtx_CONST_INT (VOIDmode, elim_offset));
	    }
	}
      else
	{
	  get_copy_loc (cp, true, &cp_mode, &hard_regno,
			&memory_slot, &offset);
	  if (hard_regno >= 0)
	    src_rtx = gen_allocno_reg_rtx (cp_mode, hard_regno, src);
	  else if (memory_slot != NULL)
	    src_rtx = get_allocno_memory_slot_rtx (memory_slot, offset,
						   cp_mode, src);
	  else
	    gcc_unreachable ();
	}
      get_copy_loc (cp, false, &cp_mode, &hard_regno, &memory_slot, &offset);
      if (hard_regno >= 0)
	dst_rtx = gen_allocno_reg_rtx (cp_mode, hard_regno, dst);
      else if (memory_slot != NULL)
	dst_rtx = get_allocno_memory_slot_rtx (memory_slot, offset, cp_mode,
					       dst);
      else
	gcc_unreachable ();
    }
#ifdef HAVE_SECONDARY_RELOADS
  if (COPY_CHANGE_ADDR (cp) == NULL || COPY_INTERM_REGNO (cp) < 0)
    emit_secondary_memory_move (dst_rtx, src_rtx, cp, alt_mode_1, alt_mode_2,
				try_spill_mode_p);
  else 
    {
      rtx interm_rtx;
      rtx scratch_rtx;

      interm_rtx = gen_allocno_reg_rtx (COPY_INTERM_MODE (cp),
					COPY_INTERM_REGNO (cp), NULL);
      if (COPY_ICODE (cp) == CODE_FOR_nothing)
	{
	  yara_assert (COPY_SCRATCH_REGNO (cp) < 0);
	  emit_secondary_memory_move (interm_rtx, src_rtx, cp,
				      alt_mode_1, alt_mode_2, false);
	  emit_secondary_memory_move (dst_rtx, interm_rtx, cp,
				      alt_mode_1, alt_mode_2, false);
	}
      else if (COPY_SCRATCH_REGNO (cp) < 0)
	emit_insn (GEN_FCN (COPY_ICODE (cp)) (dst_rtx, src_rtx, interm_rtx));
      else
	{
	  scratch_rtx
	    = gen_allocno_reg_rtx (COPY_SCRATCH_MODE (cp),
				   COPY_SCRATCH_REGNO (cp), NULL);
	  if (COPY_IN_P (cp))
	    {
	      emit_insn (GEN_FCN (COPY_ICODE (cp)) (interm_rtx, src_rtx,
						    scratch_rtx));
	      emit_secondary_memory_move (dst_rtx, interm_rtx, cp,
					  alt_mode_1, alt_mode_2, false);
	    }
	  else
	    {
	      emit_secondary_memory_move (interm_rtx, src_rtx, cp,
					  alt_mode_1, alt_mode_2, false);
	      emit_insn (GEN_FCN (COPY_ICODE (cp)) (dst_rtx, interm_rtx,
						    scratch_rtx));
	    }
	}
    }
#else
  emit_secondary_memory_move (dst_rtx, src_rtx, cp,
			      alt_mode_1, alt_mode_2, false);
#endif
}

static rtx
copy_insns (rtx insns)
{
  rtx result;

  start_sequence ();
  for (; insns != NULL_RTX; insns = NEXT_INSN (insns))
    emit_insn (copy_insn (PATTERN (insns)));
  result = get_insns ();
  end_sequence ();
  return result;
}

static void
emit_insns_at_bb_start (rtx insns, basic_block bb)
{
  rtx tmp;

  /* Get the location correct wrt a code label, and "nice" wrt a
     basic block note, and before everything else.  */
  tmp = BB_HEAD (bb);
  if (LABEL_P (tmp))
    tmp = NEXT_INSN (tmp);
  if (NOTE_INSN_BASIC_BLOCK_P (tmp))
    tmp = NEXT_INSN (tmp);
#if 0
  if (tmp
      && NOTE_P (tmp)
      && NOTE_LINE_NUMBER (tmp) == NOTE_INSN_UNLIKELY_EXECUTED_CODE)
    tmp = NEXT_INSN (tmp);
#endif
  if (tmp == BB_HEAD (bb))
    emit_insn_before (insns, tmp);
  else if (tmp != NULL_RTX)
    emit_insn_after (insns, PREV_INSN (tmp));
  else
    emit_insn_after (insns, get_last_insn ());
}

static void
emit_insns_at_bb_end (rtx insns, basic_block bb, rtx jmp, bool before_jump_p)
{
  edge_iterator ei;
  edge e;
  rtx tmp;

  if (jmp == NULL_RTX)
    emit_insn_after (insns, BB_END (bb));
  else if (before_jump_p)
    {
      for (tmp = jmp;
	   NOTE_P (PREV_INSN (tmp))
	     && NOTE_LINE_NUMBER (PREV_INSN (tmp)) == NOTE_INSN_LOOP_BEG;
	   tmp = PREV_INSN (tmp))
	;
      emit_insn_before (insns, tmp);
    }
  else
    {
      /* It is originated from the jump so we need to place it
	 after the jump.  It means that we put the copy on all
	 output edges.  */
      /* Remember we process copies on out edges of the basic
	 block after copies of at the end of the basic block.  */
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  yara_assert ((e->flags & EDGE_COMPLEX) == 0);
	  tmp = (ei.index == 0 ? insns : copy_insns (insns));
	  insert_insn_on_edge (tmp, e);
	}
    }
}

/* The function generates insns representing copies of the list given
   by CP.  */
static void
add_copy_list (copy_t cp)
{
  rtx insns, jmp;
  struct point p;
  enum point_type pt;
  bool before_jump_p;

  if (cp == NULL)
    return;
  p = COPY_POINT (cp);
  pt = p.point_type;
  jmp = (pt == AT_BB_END && control_flow_insn_p (BB_END (p.u.bb))
	 ? BB_END (p.u.bb)
	 : pt == AFTER_INSN && control_flow_insn_p (p.u.insn)
	 ? p.u.insn : NULL_RTX);
  before_jump_p = true;
  start_sequence ();
#ifdef ENABLE_YARA_CHECKING
  CLEAR_HARD_REG_SET (emit_copy_dst_hard_regs);
#endif
  for (; cp != NULL; cp = COPY_NEXT_COPY (cp))
    {
      yara_assert (COPY_POINT (cp).point_type == pt);
      yara_assert (((pt == BEFORE_INSN || pt == AFTER_INSN)
		    && COPY_POINT (cp).u.insn == p.u.insn)
		   || ((pt == ON_EDGE_SRC || pt == ON_EDGE_DST)
		       && COPY_POINT (cp).u.e.e == p.u.e.e)
		   || ((pt == AT_BB_START || pt == AT_BB_END)
		       && COPY_POINT (cp).u.bb == p.u.bb));
      if (! unnecessary_copy_p (cp)
	  && ((YARA_PARAMS & YARA_NO_COPY_SYNC) != 0 || ! COPY_SYNC_P (cp)))
	{
	  emit_copy (cp);
	  if (jmp != NULL_RTX && before_jump_p)
	    before_jump_p = copy_can_be_moved_through_insn_p (jmp, cp);
	}
    }
  insns = get_insns ();
  end_sequence ();
  if (insns == NULL_RTX)
    return;
  switch (pt)
    {
    case BEFORE_INSN:
      emit_insn_before (insns, p.u.insn);
      break;
      
    case AFTER_INSN:
      if (jmp == NULL_RTX)
	emit_insn_after (insns, p.u.insn);
      else
	{
	  gcc_assert (! before_jump_p);
	  emit_insns_at_bb_end (insns, BLOCK_FOR_INSN (jmp), jmp, false);
	}
      break;
      
    case AT_BB_START:
      emit_insns_at_bb_start (insns, p.u.bb);
      break;
      
    case AT_BB_END:
      emit_insns_at_bb_end (insns, p.u.bb, jmp, before_jump_p);
      break;
      
    case ON_EDGE_SRC:
    case ON_EDGE_DST:
      yara_assert ((p.u.e.e->flags & EDGE_COMPLEX) == 0);
      insert_insn_on_edge (insns, p.u.e.e);
      break;
      
    default:
      gcc_unreachable ();
    }
}

/* Compute the sum of X and Y, making canonicalizations assumed in an
   address, namely: sum constant integers, surround the sum of two
   constants with a CONST, put the constant as the second operand, and
   group the constant on the outermost sum.

   This routine assumes both inputs are already in canonical form.  */

static rtx
form_sum (rtx x, rtx y)
{
  rtx tem;
  enum machine_mode mode = GET_MODE (x);

  if (mode == VOIDmode)
    mode = GET_MODE (y);

  if (mode == VOIDmode)
    mode = Pmode;

  if (GET_CODE (x) == CONST_INT)
    return plus_constant (y, INTVAL (x));
  else if (GET_CODE (y) == CONST_INT)
    return plus_constant (x, INTVAL (y));
  else if (CONSTANT_P (x))
    tem = x, x = y, y = tem;

  if (GET_CODE (x) == PLUS && CONSTANT_P (XEXP (x, 1)))
    return form_sum (XEXP (x, 0), form_sum (XEXP (x, 1), y));

  /* Note that if the operands of Y are specified in the opposite
     order in the recursive calls below, infinite recursion will occur.  */
  if (GET_CODE (y) == PLUS && CONSTANT_P (XEXP (y, 1)))
    return form_sum (form_sum (x, XEXP (y, 0)), XEXP (y, 1));

  /* If both constant, encapsulate sum.  Otherwise, just form sum.  A
     constant will have been placed second.  */
  if (CONSTANT_P (x) && CONSTANT_P (y))
    {
      if (GET_CODE (x) == CONST)
	x = XEXP (x, 0);
      if (GET_CODE (y) == CONST)
	y = XEXP (y, 0);

      return gen_rtx_CONST (VOIDmode, gen_rtx_PLUS (mode, x, y));
    }

  return gen_rtx_PLUS (mode, x, y);
}

/* Scan X and replace any eliminable registers (such as fp) with a
   replacement (such as sp), plus an offset.

   MEM_MODE is the mode of an enclosing MEM.  We need this to know how
   much to adjust a register for, e.g., PRE_DEC.  Also, if we are inside a
   MEM, we are allowed to replace a sum of a register and the constant zero
   with the register, which we cannot do outside a MEM.  In addition, we need
   to record the fact that a register is referenced outside a MEM.

   If INSN is an insn, it is the insn containing X.  If we replace a REG
   in a SET_DEST with an equivalent MEM and INSN is nonzero, write a
   CLOBBER of the pseudo after INSN so find_equiv_regs will know that
   the REG is being modified.

   Alternatively, INSN may be a note (an EXPR_LIST or INSN_LIST).
   That's used when we eliminate in expressions stored in notes.
   This means, do not set ref_outside_mem even if the reference
   is outside of MEMs.

   REG_EQUIV_MEM and REG_EQUIV_ADDRESS contain address that have had
   replacements done assuming all offsets are at their initial values.  If
   they are not, or if REG_EQUIV_ADDRESS is nonzero for a pseudo we
   encounter, return the actual location so that find_reloads will do
   the proper thing.  */

rtx
yara_eliminate_regs (rtx x, enum machine_mode mem_mode)
{
  enum rtx_code code = GET_CODE (x);
  int regno;
  rtx new;
  int i, j;
  const char *fmt;
  int copied = 0;

  if (! current_function_decl)
    return x;

  switch (code)
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
    case CONST:
    case SYMBOL_REF:
    case CODE_LABEL:
    case PC:
    case CC0:
    case ASM_INPUT:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
    case RETURN:
      return x;

    case REG:
      regno = REGNO (x);

      /* First handle the case where we encounter a bare register that
	 is eliminable.  Replace it with a PLUS.  */
      if (regno < FIRST_PSEUDO_REGISTER && reg_eliminate [regno] != NULL)
	return plus_constant (gen_rtx_REG (Pmode, reg_eliminate [regno]->to),
			      reg_eliminate [regno]->offset);
      
      return x;

    /* You might think handling MINUS in a manner similar to PLUS is a
       good idea.  It is not.  It has been tried multiple times and every
       time the change has had to have been reverted.

       Other parts of reload know a PLUS is special (gen_reload for example)
       and require special code to handle code a reloaded PLUS operand.

       Also consider backends where the flags register is clobbered by a
       MINUS, but we can emit a PLUS that does not clobber flags (IA-32,
       lea instruction comes to mind).  If we try to reload a MINUS, we
       may kill the flags register that was holding a useful value.

       So, please before trying to handle MINUS, consider reload as a
       whole instead of this little section as well as the backend issues.  */
    case PLUS:
      /* If this is the sum of an eliminable register and a constant, rework
	 the sum.  */
      if (REG_P (XEXP (x, 0))
	  && (regno = REGNO (XEXP (x, 0))) < FIRST_PSEUDO_REGISTER
	  && CONSTANT_P (XEXP (x, 1)))
	{
	  if (reg_eliminate [regno] != NULL)
	    {
	      /* The only time we want to replace a PLUS with a REG
		 (this occurs when the constant operand of the PLUS is
		 the negative of the offset) is when we are inside a
		 MEM.  We won't want to do so at other times because
		 that would change the structure of the insn in a way
		 that reload can't handle.  We special-case the
		 commonest situation in yara_eliminate_regs_in_insn, so
		 just replace a PLUS with a PLUS here, unless inside a
		 MEM.  */
	      if (mem_mode != 0 && GET_CODE (XEXP (x, 1)) == CONST_INT
		  && INTVAL (XEXP (x, 1)) == - reg_eliminate [regno]->offset)
		return gen_rtx_REG (Pmode, reg_eliminate [regno]->to);
	      else
		return gen_rtx_PLUS (Pmode,
				     gen_rtx_REG (Pmode,
						  reg_eliminate [regno]->to),
				     plus_constant
				     (XEXP (x, 1),
				      reg_eliminate [regno]->offset));
	    }
	  /* If the register is not eliminable, we are done since the other
	     operand is a constant.  */
	  return x;
	}

      /* If this is part of an address, we want to bring any constant to the
	 outermost PLUS.  We will do this by doing register replacement in
	 our operands and seeing if a constant shows up in one of them.

	 Note that there is no risk of modifying the structure of the insn,
	 since we only get called for its operands, thus we are either
	 modifying the address inside a MEM, or something like an address
	 operand of a load-address insn.  */

      {
	rtx new0 = yara_eliminate_regs (XEXP (x, 0), mem_mode);
	rtx new1 = yara_eliminate_regs (XEXP (x, 1), mem_mode);

	if (reg_renumber && (new0 != XEXP (x, 0) || new1 != XEXP (x, 1)))
	  {
	    /* If one side is a PLUS and the other side is a pseudo that
	       didn't get a hard register but has a reg_equiv_constant,
	       we must replace the constant here since it may no longer
	       be in the position of any operand.  */
	    if (GET_CODE (new0) == PLUS && REG_P (new1)
		&& REGNO (new1) >= FIRST_PSEUDO_REGISTER
		&& reg_renumber[REGNO (new1)] < 0
		&& reg_equiv_constant != 0
		&& reg_equiv_constant[REGNO (new1)] != 0)
	      new1 = reg_equiv_constant[REGNO (new1)];
	    else if (GET_CODE (new1) == PLUS && REG_P (new0)
		     && REGNO (new0) >= FIRST_PSEUDO_REGISTER
		     && reg_renumber[REGNO (new0)] < 0
		     && reg_equiv_constant[REGNO (new0)] != 0)
	      new0 = reg_equiv_constant[REGNO (new0)];

	    new = form_sum (new0, new1);

	    /* As above, if we are not inside a MEM we do not want to
	       turn a PLUS into something else.  We might try to do so here
	       for an addition of 0 if we aren't optimizing.  */
	    if (! mem_mode && GET_CODE (new) != PLUS)
	      return gen_rtx_PLUS (GET_MODE (x), new, const0_rtx);
	    else
	      return new;
	  }
      }
      return x;

    case MULT:
      /* If this is the product of an eliminable register and a
	 constant, apply the distribute law and move the constant out
	 so that we have (plus (mult ..) ..).  This is needed in order
	 to keep load-address insns valid.   This case is pathological.
	 We ignore the possibility of overflow here.  */
      if (REG_P (XEXP (x, 0))
	  && (regno = REGNO (XEXP (x, 0))) < FIRST_PSEUDO_REGISTER
	  && GET_CODE (XEXP (x, 1)) == CONST_INT)
	{
	  if (reg_eliminate [regno] != NULL)
	    {
	      return
		plus_constant (gen_rtx_MULT
			       (Pmode,
				gen_rtx_REG (Pmode, reg_eliminate [regno]->to),
					     XEXP (x, 1)),
			       reg_eliminate [regno]->offset
			       * INTVAL (XEXP (x, 1)));
	    }
	}

      /* ... fall through ...  */

    case CALL:
    case COMPARE:
    /* See comments before PLUS about handling MINUS.  */
    case MINUS:
    case DIV:      case UDIV:
    case MOD:      case UMOD:
    case AND:      case IOR:      case XOR:
    case ROTATERT: case ROTATE:
    case ASHIFTRT: case LSHIFTRT: case ASHIFT:
    case NE:       case EQ:
    case GE:       case GT:       case GEU:    case GTU:
    case LE:       case LT:       case LEU:    case LTU:
      {
	rtx new0 = yara_eliminate_regs (XEXP (x, 0), mem_mode);
	rtx new1
	  = XEXP (x, 1) ? yara_eliminate_regs (XEXP (x, 1), mem_mode) : 0;

	if (new0 != XEXP (x, 0) || new1 != XEXP (x, 1))
	  return gen_rtx_fmt_ee (code, GET_MODE (x), new0, new1);
      }
      return x;

    case EXPR_LIST:
      /* If we have something in XEXP (x, 0), the usual case, eliminate it.  */
      if (XEXP (x, 0))
	{
	  new = yara_eliminate_regs (XEXP (x, 0), mem_mode);
	  if (new != XEXP (x, 0))
	    {
	      /* If this is a REG_DEAD note, it is not valid anymore.
		 Using the eliminated version could result in creating a
		 REG_DEAD note for the stack or frame pointer.  */
	      if (GET_MODE (x) == REG_DEAD)
		return (XEXP (x, 1)
			? yara_eliminate_regs (XEXP (x, 1), mem_mode)
			: NULL_RTX);

	      x = gen_rtx_EXPR_LIST (REG_NOTE_KIND (x), new, XEXP (x, 1));
	    }
	}

      /* ... fall through ...  */

    case INSN_LIST:
      /* Now do eliminations in the rest of the chain.  If this was
	 an EXPR_LIST, this might result in allocating more memory than is
	 strictly needed, but it simplifies the code.  */
      if (XEXP (x, 1))
	{
	  new = yara_eliminate_regs (XEXP (x, 1), mem_mode);
	  if (new != XEXP (x, 1))
	    return
	      gen_rtx_fmt_ee (GET_CODE (x), GET_MODE (x), XEXP (x, 0), new);
	}
      return x;

    case PRE_INC:
    case POST_INC:
    case PRE_DEC:
    case POST_DEC:
    case STRICT_LOW_PART:
    case NEG:          case NOT:
    case SIGN_EXTEND:  case ZERO_EXTEND:
    case TRUNCATE:     case FLOAT_EXTEND: case FLOAT_TRUNCATE:
    case FLOAT:        case FIX:
    case UNSIGNED_FIX: case UNSIGNED_FLOAT:
    case ABS:
    case SQRT:
    case FFS:
    case CLZ:
    case CTZ:
    case POPCOUNT:
    case PARITY:
      new = yara_eliminate_regs (XEXP (x, 0), mem_mode);
      if (new != XEXP (x, 0))
	return gen_rtx_fmt_e (code, GET_MODE (x), new);
      return x;

    case SUBREG:
      /* Similar to above processing, but preserve SUBREG_BYTE.
	 Convert (subreg (mem)) to (mem) if not paradoxical.
	 Also, if we have a non-paradoxical (subreg (pseudo)) and the
	 pseudo didn't get a hard reg, we must replace this with the
	 eliminated version of the memory location because push_reload
	 may do the replacement in certain circumstances.  */
      if (REG_P (SUBREG_REG (x))
	  && (GET_MODE_SIZE (GET_MODE (x))
	      <= GET_MODE_SIZE (GET_MODE (SUBREG_REG (x))))
	  && reg_equiv_memory_loc != 0
	  && reg_equiv_memory_loc [REGNO (SUBREG_REG (x))] != 0)
	{
	  new = SUBREG_REG (x);
	}
      else
	new = yara_eliminate_regs (SUBREG_REG (x), mem_mode);

      if (new != SUBREG_REG (x))
	{
	  int x_size = GET_MODE_SIZE (GET_MODE (x));
	  int new_size = GET_MODE_SIZE (GET_MODE (new));

	  if (MEM_P (new)
	      && ((x_size < new_size
#ifdef WORD_REGISTER_OPERATIONS
		   /* On these machines, combine can create rtl of the form
		      (set (subreg:m1 (reg:m2 R) 0) ...)
		      where m1 < m2, and expects something interesting to
		      happen to the entire word.  Moreover, it will use the
		      (reg:m2 R) later, expecting all bits to be preserved.
		      So if the number of words is the same, preserve the
		      subreg so that push_reload can see it.  */
		   && ! ((x_size - 1) / UNITS_PER_WORD
			 == (new_size -1 ) / UNITS_PER_WORD)
#endif
		   )
		  || x_size == new_size)
	      )
	    return adjust_address_nv (new, GET_MODE (x), SUBREG_BYTE (x));
	  else
	    return gen_rtx_SUBREG (GET_MODE (x), new, SUBREG_BYTE (x));
	}

      return x;

    case MEM:
      /* Our only special processing is to pass the mode of the MEM to our
	 recursive call and copy the flags.  While we are here, handle this
	 case more efficiently.  */
      return
	replace_equiv_address_nv (x,
				  yara_eliminate_regs (XEXP (x, 0),
						       GET_MODE (x)));

    case USE:
      /* Handle insn_list USE that a call to a pure function may generate.  */
      new = yara_eliminate_regs (XEXP (x, 0), 0);
      if (new != XEXP (x, 0))
	return gen_rtx_USE (GET_MODE (x), new);
      return x;

    case CLOBBER:
    case ASM_OPERANDS:
    case SET:
      gcc_unreachable ();

    default:
      break;
    }

  /* Process each of our operands recursively.  If any have changed, make a
     copy of the rtx.  */
  fmt = GET_RTX_FORMAT (code);
  for (i = 0; i < GET_RTX_LENGTH (code); i++, fmt++)
    {
      if (*fmt == 'e')
	{
	  new = yara_eliminate_regs (XEXP (x, i), mem_mode);
	  if (new != XEXP (x, i) && ! copied)
	    {
	      rtx new_x = rtx_alloc (code);
	      memcpy (new_x, x, RTX_CODE_SIZE (code));
	      x = new_x;
	      copied = 1;
	    }
	  XEXP (x, i) = new;
	}
      else if (*fmt == 'E')
	{
	  int copied_vec = 0;
	  for (j = 0; j < XVECLEN (x, i); j++)
	    {
	      new = yara_eliminate_regs (XVECEXP (x, i, j), mem_mode);
	      if (new != XVECEXP (x, i, j) && ! copied_vec)
		{
		  rtvec new_v = gen_rtvec_v (XVECLEN (x, i),
					     XVEC (x, i)->elem);
		  if (! copied)
		    {
		      rtx new_x = rtx_alloc (code);
		      memcpy (new_x, x, RTX_CODE_SIZE (code));
		      x = new_x;
		      copied = 1;
		    }
		  XVEC (x, i) = new_v;
		  copied_vec = 1;
		}
	      XVECEXP (x, i, j) = new;
	    }
	}
    }

  return x;
}

/* The function does elimination of register FROM to register TO for
   allocno A with an addition OFFSET (beside the usual elimination
   offset).  */
static void
eliminate_reg_allocno (allocno_t a, int from, int to, HOST_WIDE_INT offset)
{
  rtx *container_loc;

  container_loc = INSN_ALLOCNO_CONTAINER_LOC (a);
  if (INSN_ALLOCNO_TYPE (a) == BASE_REG
      || INSN_ALLOCNO_TYPE (a) == INDEX_REG)
    {
      /* ??? Another register allocno */
      if (GET_CODE (*container_loc) == MEM)
	XEXP (*container_loc, 0)
	  = get_eliminate_subst_rtx (&XEXP (*container_loc, 0),
				     from, to, offset);
      else
	*container_loc
	  = get_eliminate_subst_rtx (container_loc, from, to, offset);
    }
  else
    {
      yara_assert
	(GET_CODE (*container_loc) == PLUS
	 && REG_P (XEXP (*container_loc, 0))
	 && (int) REGNO (XEXP (*container_loc, 0)) == from
	 && GET_CODE (XEXP (*container_loc, 1)) == CONST_INT);
      XEXP (*container_loc, 0) = gen_rtx_REG (Pmode, to);
      XEXP (*container_loc, 1)
	= gen_rtx_CONST_INT (VOIDmode,
			     INTVAL (XEXP (*container_loc, 1)) + offset);
    }
}

static void
modify_insn (rtx insn, bool non_operand_p)
{
  allocno_t a, origin;
  int i, regno, hard_regno, offset;
  struct memory_slot *memory_slot;
  rtx *loc;
  
  for (a = insn_allocnos [INSN_UID (insn)];
       a != NULL;
       a = INSN_ALLOCNO_NEXT (a))
    {
      if ((INSN_ALLOCNO_TYPE (a) >= OPERAND_BASE && non_operand_p)
	  || (INSN_ALLOCNO_TYPE (a) < OPERAND_BASE && ! non_operand_p))
	continue;
      loc = INSN_ALLOCNO_LOC (a);
      regno = ALLOCNO_REGNO (a);
      hard_regno = ALLOCNO_HARD_REGNO (a);
      memory_slot = ALLOCNO_MEMORY_SLOT (a);
      if ((origin = INSN_ALLOCNO_TIED_ALLOCNO (a)) != NULL
	  && ! INSN_ALLOCNO_ORIGINAL_P (a))
	{
	  yara_assert (((hard_regno < 0
			&& (regno < 0 || ! HARD_REGISTER_NUM_P (regno))
			 && ALLOCNO_HARD_REGNO (origin) < 0)
			|| ((hard_regno >= 0
			     || (regno >= 0 && HARD_REGISTER_NUM_P (regno)))
			    && (ALLOCNO_HARD_REGNO (origin) >= 0
				|| (ALLOCNO_REGNO (origin) >= 0
				    && HARD_REGISTER_NUM_P (ALLOCNO_REGNO
							    (origin)))))
			|| (memory_slot != NULL
			    && ALLOCNO_MEMORY_SLOT (origin) != NULL
			    && regno == ALLOCNO_REGNO (origin))));
	  /* Remember that the origin is a hard register or already
	     changed by a hard register.  */
	  yara_assert (hard_regno < 0
		       || GET_CODE (*INSN_ALLOCNO_LOC (origin)) == SCRATCH
		       || (REG_P (*INSN_ALLOCNO_LOC (origin))
			   && (hard_regno
			       == (int) REGNO (*INSN_ALLOCNO_LOC (origin)))));
	  if (hard_regno >= 0)
	    *loc = gen_allocno_reg_rtx (ALLOCNO_MODE (a), hard_regno, a);
	  else if (memory_slot != NULL)
	    {
	      offset = ALLOCNO_MEMORY_SLOT_OFFSET (a);
	      *loc = get_allocno_memory_slot_rtx (memory_slot, offset,
						  ALLOCNO_MODE (a), a);
	    }
	  else if (INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a))
	    ;
	  else
	    *loc = copy_rtx (*INSN_ALLOCNO_LOC (origin));
	}
      else if (ALLOCNO_USE_EQUIV_CONST_P (a))
	{
	  int to;
 	  HOST_WIDE_INT offset;
	  rtx x;

	  yara_assert (ALLOCNO_REGNO (a) >= 0
		       && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a)));
	  yara_assert (REG_P (*INSN_ALLOCNO_LOC (a)));
	  x = reg_equiv_constant [ALLOCNO_REGNO (a)];
	  if (CONSTANT_P (x))
	    *loc = x;
	  else
	    {
	      get_equiv_const_elimination_info (x, &to, &offset);
	      eliminate_reg_allocno (a, regno, to, offset);
	    }
	  if (yara_dump_file != NULL)
	    {
	      fprintf (yara_dump_file, "Using equiv constant %d:",
		       ALLOCNO_NUM (a));
	      print_inline_rtx (yara_dump_file, *loc, 26);
	      fprintf (yara_dump_file, "\n");
	    }
	}
      else if (INSN_ALLOCNO_CONST_POOL_P (a))
	{
	  yara_assert (CONST_POOL_OK_P (*loc));
	  *loc = force_const_mem (ALLOCNO_MODE (a), *loc);
	}
      else if (hard_regno < 0 && memory_slot == NULL)
	{
	  struct reg_eliminate *elim;
	  int interm_regno, to;
	  HOST_WIDE_INT offset;

	  if ((elim = INSN_ALLOCNO_ELIMINATION (a)) == NULL)
	    {
	      yara_assert (INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a));
	      continue;
	    }
	  interm_regno = INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (a);
	  offset = elim->offset;
	  if (interm_regno < 0)
	    to = elim->to;
	  else
	    {
	      to = interm_regno;
	      offset = 0;
	    }
	  if (regno < 0 || interm_regno >= 0)
	    /* We have address allocno or just using the intermediate
	       register.  */
	    *loc = get_eliminate_subst_rtx (loc, elim->from, to, offset);
	  else
	    eliminate_reg_allocno (a, elim->from, to, offset);
	}
      else
	{
	  enum machine_mode mode = ALLOCNO_MODE (a);

	  if (hard_regno >= 0)
	    *loc = gen_allocno_reg_rtx (mode, hard_regno, a);
	  else if (memory_slot != NULL)
	    {
	      offset = ALLOCNO_MEMORY_SLOT_OFFSET (a);
	      *loc = get_allocno_memory_slot_rtx (memory_slot, offset,
						  mode, a);
	    }
	  else
	    gcc_unreachable ();
	}
    }
  if (non_operand_p)
    return;
  extract_insn (insn);
  for (i = recog_data.n_dups - 1; i >= 0; i--)
    *recog_data.dup_loc [i]
      = *recog_data.operand_loc [(int) recog_data.dup_num [i]];
}

static void
clean_insn (rtx insn)
{
  rtx *pnote;

  yara_assert (INSN_P (insn));
  if ((GET_CODE (PATTERN (insn)) == USE
       && find_reg_note (insn, REG_EQUAL, NULL_RTX) != NULL_RTX)
      || (GET_CODE (PATTERN (insn)) == CLOBBER
	  && (! MEM_P (XEXP (PATTERN (insn), 0))
	      || GET_MODE (XEXP (PATTERN (insn), 0)) != BLKmode
	      || (GET_CODE (XEXP (XEXP (PATTERN (insn), 0), 0)) != SCRATCH
		  && XEXP (XEXP (PATTERN (insn), 0), 0) != stack_pointer_rtx))
	  && (! REG_P (XEXP (PATTERN (insn), 0))
	      || ! REG_FUNCTION_VALUE_P (XEXP (PATTERN (insn), 0)))))
    {
      delete_insn (insn);
      return;
    }
  
  pnote = &REG_NOTES (insn);
  while (*pnote != 0)
    {
      if (REG_NOTE_KIND (*pnote) == REG_DEAD
	  || REG_NOTE_KIND (*pnote) == REG_UNUSED
	  || REG_NOTE_KIND (*pnote) == REG_INC
	  || REG_NOTE_KIND (*pnote) == REG_RETVAL
	  || REG_NOTE_KIND (*pnote) == REG_LIBCALL)
	*pnote = XEXP (*pnote, 1);
      else
	pnote = &XEXP (*pnote, 1);
    }
  
#ifdef AUTO_INC_DEC
  add_auto_inc_notes (insn, PATTERN (insn));
#endif
}

/* Allign stack or allocate memory on the stack for all stack slots
   currently existing.  */
static void
allocate_stack_memory (void)
{
  int align;

  if (slot_memory_size == 0)
    return;
  align = slot_memory_alignment;
  /* Ignore alignment we can't do with expected alignment of the
     boundary.  */
  if ((unsigned) align * BITS_PER_UNIT > PREFERRED_STACK_BOUNDARY)
    align = (int) PREFERRED_STACK_BOUNDARY / BITS_PER_UNIT;
  (void) assign_stack_local (BLKmode, 0, align * BITS_PER_UNIT);
  final_stack_memory_start_frame_offset
    = get_stack_memory_start_frame_offset ();
  (void) assign_stack_local (BLKmode, slot_memory_size,
			     align * BITS_PER_UNIT);
  final_rounded_slot_memory_size = rounded_slot_memory_size ();
  update_elim_offsets ();
}


void
yara_rewrite (void)
{
  basic_block bb;
  edge_iterator ei;
  edge e;
  rtx bound, insn, next_insn, set;
  bool remove_p;

  if ((YARA_PARAMS & YARA_NO_COPY_SYNC) == 0)
    {
      insn_to_be_removed_p = yara_allocate_bitmap ();
      bitmap_clear (insn_to_be_removed_p);
      initiate_locations ();
      find_sync_copies ();
      finish_locations ();
    }
  no_new_pseudos = 0;
  allocate_stack_memory ();
  FOR_EACH_BB (bb)
    {
      bound = NEXT_INSN (BB_END (bb));
      for (insn = BB_HEAD (bb); insn != bound; insn = next_insn)
	{
	  next_insn = NEXT_INSN (insn);
	  if (INSN_P (insn) && INSN_UID (insn) < yara_max_uid)
	    {
	      remove_p = ((YARA_PARAMS & YARA_NO_COPY_SYNC) == 0
			  && bitmap_bit_p (insn_to_be_removed_p,
					   INSN_UID (insn)));
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
	      /* Special case frame = bp.  Change it.  */
	      if ((set = single_set (insn)) != NULL_RTX
		  && SET_DEST (set) == frame_pointer_rtx
		  && SET_SRC (set) == hard_frame_pointer_rtx)
		{
		  rtx new_insns;

		  yara_assert (GET_MODE (hard_frame_pointer_rtx) == Pmode);
		  start_sequence ();
		  emit_insn (gen_rtx_SET
			     (VOIDmode, hard_frame_pointer_rtx,
			      gen_rtx_PLUS
			      (Pmode, hard_frame_pointer_rtx,
			       GEN_INT (-frame_hard_frame_pointer_offset))));
		  new_insns = get_insns ();
		  end_sequence ();
		  emit_insn_before (new_insns, insn);
      		  delete_insn (insn);
		  continue;
		}
#endif
	      if (! remove_p)
		modify_insn (insn, true);
	      /* Make copy before insn modifications because it changes
		 values in insn operand allocno locations.  */
	      add_copy_list (before_insn_copies [INSN_UID (insn)]);
	      add_copy_list (after_insn_copies [INSN_UID (insn)]);
	      if (remove_p)
		delete_insn (insn);
	      else
		{
		  modify_insn (insn, false);
		  clean_insn (insn);
		}
	    }
	}
      add_copy_list (at_bb_start_copies [bb->index]);
      add_copy_list (at_bb_end_copies [bb->index]);
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  /* The order of calls is important.  */
	  add_copy_list (at_edge_start_copies [(size_t) e->aux]);
	  add_copy_list (at_edge_end_copies [(size_t) e->aux]);
	}
    }
  commit_edge_insertions ();
  no_new_pseudos = 1;
  if ((YARA_PARAMS & YARA_NO_COPY_SYNC) == 0)
    yara_free_bitmap (insn_to_be_removed_p);
}
