/* IRA allocation based on graph coloring.
   Copyright (C) 2006, 2007, 2008
   Free Software Foundation, Inc.
   Contributed by Vladimir Makarov <vmakarov@redhat.com>.

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
#include "tm_p.h"
#include "target.h"
#include "varray.h"
#include "regs.h"
#include "flags.h"
#include "sbitmap.h"
#include "bitmap.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "expr.h"
#include "toplev.h"
#include "reload.h"
#include "params.h"
#include "df.h"
#include "ira-int.h"

/* This file contains code for regional graph coloring, spill/restore
   code placement optimization, and code helping the reload pass to do
   a better job.  */

static void initiate_cost_update (void);
static void finish_cost_update (void);
static void update_copy_costs_1 (allocno_t, int, int, int);
static void update_copy_costs (allocno_t, int);
static int allocno_cost_compare_func (const void *, const void *);
static void print_coalesced_allocno (allocno_t);
static int assign_hard_reg (allocno_t, int);

static void add_allocno_to_bucket (allocno_t, allocno_t *);
static void get_coalesced_allocnos_attributes (allocno_t, int *, int *);
static int bucket_allocno_compare_func (const void *, const void *);
static void sort_bucket (allocno_t *);
static void add_allocno_to_ordered_bucket (allocno_t, allocno_t *);
static void delete_allocno_from_bucket (allocno_t, allocno_t *);
static void push_allocno_to_stack (allocno_t);
static void remove_allocno_from_bucket_and_push (allocno_t, int);
static void push_only_colorable (void);
static void push_allocno_to_spill (allocno_t);
static int calculate_allocno_spill_cost (allocno_t);
static void push_allocnos_to_stack (void);
static void pop_allocnos_from_stack (void);
static void setup_allocno_available_regs_num (allocno_t);
static void setup_allocno_left_conflicts_num (allocno_t);
static void put_allocno_into_bucket (allocno_t);
static int copy_freq_compare_func (const void *, const void *);
static void merge_allocnos (allocno_t, allocno_t);
static int coalesced_allocno_conflict_p (allocno_t, allocno_t, int);
static void coalesce_allocnos (int);
static void color_allocnos (void);

static void print_loop_title (loop_tree_node_t);
static void color_pass (loop_tree_node_t);
static void do_coloring (void);

static void move_spill_restore (void);

static void update_curr_costs (allocno_t);
static void start_allocno_priorities (allocno_t *, int);
static int allocno_priority_compare_func (const void *, const void *);

static int coalesced_pseudo_reg_freq_compare (const void *, const void *);
static int coalesced_pseudo_reg_slot_compare (const void *, const void *);
static void setup_coalesced_allocno_costs_and_nums (int *, int);
static int collect_spilled_coalesced_allocnos (int *, int, allocno_t *);
static int coalesce_spill_slots (allocno_t *, int);

static int allocno_reload_assign (allocno_t, HARD_REG_SET);
static int pseudo_reg_compare (const void *, const void *);

static int calculate_spill_cost (int *, rtx, rtx, rtx,
				 int *, int *, int *, int*);

/* Bitmap of allocnos which should be colored.  */
static bitmap coloring_allocno_bitmap;

/* Bitmap of allocnos which should be taken into account during
   coloring.  In general case it contains allocnos from
   coloring_allocno_bitmap plus other already colored conflicting
   allocnos.  */
static bitmap consideration_allocno_bitmap;

/* TRUE if we coalesced some allocnos.  In other words, if we got
   loops formed by members first_coalesced_allocno and
   next_coalesced_allocno containing more one allocno.  */
static int allocno_coalesced_p;

/* Bitmap used to prevent a repeated allocno processing because of
   coalescing.  */
static bitmap processed_coalesced_allocno_bitmap;

/* All allocnos sorted according their priorities.  */
static allocno_t *sorted_allocnos;

/* Array used to sort allocnos to choose an allocno for spilling.  */
static allocno_t *sorted_allocnos_for_spilling;

/* Varray representing the stack of allocnos used during coloring.  */
static varray_type allocno_stack_varray;



/* This page contains functions used to choose hard registers for
   allocnos.  */

/* Array whose element value is TRUE if the corresponding hard
   register was already allocated for an allocno.  */
static int allocated_hardreg_p[FIRST_PSEUDO_REGISTER];

/* Array used to check already processed allocnos during the current
   update_copy_costs call.  */
static int *allocno_update_cost_check;

/* The current value of update_copy_cost call count.  */
static int update_cost_check;

/* Allocate and initialize data necessary for function
   update_copy_costs.  */
static void
initiate_cost_update (void)
{
  allocno_update_cost_check = ira_allocate (allocnos_num * sizeof (int));
  memset (allocno_update_cost_check, 0, allocnos_num * sizeof (int));
  update_cost_check = 0;
}

/* Deallocate data used by function update_copy_costs.  */
static void
finish_cost_update (void)
{
  ira_free (allocno_update_cost_check);
}

/* This recursive function updates costs (decrease if DECR_P) of the
   unassigned allocnos connected by copies with ALLOCNO.  This update
   increases chances to remove some copies.  Copy cost is proportional
   the copy frequency divided by DIVISOR.  */
static void
update_copy_costs_1 (allocno_t allocno, int hard_regno,
		     int decr_p, int divisor)
{
  int i, cost, update_cost;
  enum machine_mode mode;
  enum reg_class class, cover_class;
  allocno_t another_allocno;
  copy_t cp, next_cp;

  cover_class = ALLOCNO_COVER_CLASS (allocno);
  if (cover_class == NO_REGS)
    return;
  if (allocno_update_cost_check[ALLOCNO_NUM (allocno)] == update_cost_check)
    return;
  allocno_update_cost_check[ALLOCNO_NUM (allocno)] = update_cost_check;
  ira_assert (hard_regno >= 0);
  i = class_hard_reg_index[cover_class][hard_regno];
  ira_assert (i >= 0);
  class = REGNO_REG_CLASS (hard_regno);
  mode = ALLOCNO_MODE (allocno);
  for (cp = ALLOCNO_COPIES (allocno); cp != NULL; cp = next_cp)
    {
      if (cp->first == allocno)
	{
	  next_cp = cp->next_first_allocno_copy;
	  another_allocno = cp->second;
	}
      else if (cp->second == allocno)
	{
	  next_cp = cp->next_second_allocno_copy;
	  another_allocno = cp->first;
	}
      else
	gcc_unreachable ();
      if (cover_class
	  != ALLOCNO_COVER_CLASS (another_allocno)
	  || ALLOCNO_ASSIGNED_P (another_allocno))
	continue;
      cost = (cp->second == allocno
	      ? register_move_cost[mode][class]
	        [ALLOCNO_COVER_CLASS (another_allocno)]
	      : register_move_cost[mode]
	        [ALLOCNO_COVER_CLASS (another_allocno)][class]);
      if (decr_p)
	cost = -cost;
      allocate_and_set_or_copy_costs
	(&ALLOCNO_UPDATED_HARD_REG_COSTS (another_allocno), cover_class,
	 ALLOCNO_COVER_CLASS_COST (another_allocno),
	 ALLOCNO_HARD_REG_COSTS (another_allocno));
      allocate_and_set_or_copy_costs
	(&ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (another_allocno),
	 cover_class, 0,
	 ALLOCNO_CONFLICT_HARD_REG_COSTS (another_allocno));
      update_cost = cp->freq * cost / divisor;
      ALLOCNO_UPDATED_HARD_REG_COSTS (another_allocno)[i] += update_cost;
      ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (another_allocno)[i]
	+= update_cost;
      if (update_cost != 0)
	update_copy_costs_1 (another_allocno, hard_regno,
			     decr_p, divisor * 4);
    }
}

/* Entry function to update costs of allocnos to increase chances to
   remove some copies as the result of subsequent assignment.  */
static void
update_copy_costs (allocno_t allocno, int decr_p)
{
  update_cost_check++;  
  update_copy_costs_1 (allocno, ALLOCNO_HARD_REGNO (allocno), decr_p, 1);
}

/* The function is used to sort allocnos according to the profit of
   usage of a hard register instead of memory for them. */
static int
allocno_cost_compare_func (const void *v1p, const void *v2p)
{
  allocno_t p1 = *(const allocno_t *) v1p, p2 = *(const allocno_t *) v2p;
  int c1, c2;

  c1 = ALLOCNO_UPDATED_MEMORY_COST (p1) - ALLOCNO_COVER_CLASS_COST (p1);
  c2 = ALLOCNO_UPDATED_MEMORY_COST (p2) - ALLOCNO_COVER_CLASS_COST (p2);
  if (c1 - c2)
    return c1 - c2;

  /* If regs are equally good, sort by allocno numbers, so that the
     results of qsort leave nothing to chance.  */
  return ALLOCNO_NUM (p1) - ALLOCNO_NUM (p2);
}

/* Print all allocnos coalesced with ALLOCNO.  */
static void
print_coalesced_allocno (allocno_t allocno)
{
  allocno_t a;

  for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
    {
      print_expanded_allocno (a);
      if (a == allocno)
	break;
      fprintf (ira_dump_file, "+");
    }
}

/* Function choosing a hard register for ALLOCNO (or for all coalesced
   allocnos represented by ALLOCNO).  If RETRY_P is TRUE, it means
   that the function called from function `reassign_conflict_allocnos'
   and `allocno_reload_assign'.  The function implements the
   optimistic coalescing too: if we failed to assign a hard register
   to set of the coalesced allocnos, we put them onto the coloring
   stack for subsequent separate assigning.  */
static int
assign_hard_reg (allocno_t allocno, int retry_p)
{
  HARD_REG_SET conflicting_regs;
  int i, j, hard_regno, best_hard_regno, class_size;
  int cost, mem_cost, min_cost, full_cost, min_full_cost, add_cost;
  int *a_costs;
  int *conflict_costs;
  enum reg_class cover_class, class;
  enum machine_mode mode;
  allocno_t a, conflict_allocno;
  allocno_t another_allocno;
  allocno_conflict_iterator aci;
  copy_t cp, next_cp;
  static int costs[FIRST_PSEUDO_REGISTER], full_costs[FIRST_PSEUDO_REGISTER];
#ifdef STACK_REGS
  int no_stack_reg_p;
#endif

  ira_assert (! ALLOCNO_ASSIGNED_P (allocno));
  cover_class = ALLOCNO_COVER_CLASS (allocno);
  class_size = class_hard_regs_num[cover_class];
  mode = ALLOCNO_MODE (allocno);
  COPY_HARD_REG_SET (conflicting_regs, no_alloc_regs);
  IOR_COMPL_HARD_REG_SET (conflicting_regs, reg_class_contents[cover_class]);
  best_hard_regno = -1;
  memset (full_costs, 0, sizeof (int) * class_size);
  mem_cost = 0;
  if (allocno_coalesced_p)
    bitmap_clear (processed_coalesced_allocno_bitmap);
  memset (costs, 0, sizeof (int) * class_size);
  memset (full_costs, 0, sizeof (int) * class_size);
#ifdef STACK_REGS
  no_stack_reg_p = FALSE;
#endif
  for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
    {
      mem_cost += ALLOCNO_UPDATED_MEMORY_COST (a);
      IOR_HARD_REG_SET (conflicting_regs,
			ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
      allocate_and_copy_costs (&ALLOCNO_UPDATED_HARD_REG_COSTS (a),
			       cover_class, ALLOCNO_HARD_REG_COSTS (a));
      a_costs = ALLOCNO_UPDATED_HARD_REG_COSTS (a);
#ifdef STACK_REGS
      no_stack_reg_p = no_stack_reg_p || ALLOCNO_TOTAL_NO_STACK_REG_P (a);
#endif
      for (cost = ALLOCNO_COVER_CLASS_COST (a), i = 0; i < class_size; i++)
	if (a_costs != NULL)
	  {
	    costs[i] += a_costs[i];
	    full_costs[i] += a_costs[i];
	  }
	else
	  {
	    costs[i] += cost;
	    full_costs[i] += cost;
	  }
      /* Take preferences of conflicting allocnos into account.  */
      FOR_EACH_ALLOCNO_CONFLICT (a, conflict_allocno, aci)
	/* Reload can give another class so we need to check all
	   allocnos.  */
	if (retry_p || bitmap_bit_p (consideration_allocno_bitmap,
				     ALLOCNO_NUM (conflict_allocno)))
	  {
	    ira_assert (cover_class == ALLOCNO_COVER_CLASS (conflict_allocno));
	    if (allocno_coalesced_p)
	      {
		if (bitmap_bit_p (processed_coalesced_allocno_bitmap,
				  ALLOCNO_NUM (conflict_allocno)))
		  continue;
		bitmap_set_bit (processed_coalesced_allocno_bitmap,
				ALLOCNO_NUM (conflict_allocno));
	      }
	    if (ALLOCNO_ASSIGNED_P (conflict_allocno))
	      {
		if ((hard_regno = ALLOCNO_HARD_REGNO (conflict_allocno)) >= 0)
		  {
		    IOR_HARD_REG_SET
		      (conflicting_regs,
		       reg_mode_hard_regset
		       [hard_regno][ALLOCNO_MODE (conflict_allocno)]);
		    if (hard_reg_set_subset_p (reg_class_contents[cover_class],
					       conflicting_regs))
		      goto fail;
		  }
		continue;
	      }
	    else if (! ALLOCNO_MAY_BE_SPILLED_P (conflict_allocno))
	      {
		allocate_and_copy_costs
		  (&ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (conflict_allocno),
		   cover_class,
		   ALLOCNO_CONFLICT_HARD_REG_COSTS (conflict_allocno));
		conflict_costs
		  = ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (conflict_allocno);
		if (conflict_costs != NULL)
		  for (j = class_size - 1; j >= 0; j--)
		    full_costs[j] -= conflict_costs[j];
	      }
	  }
      if (a == allocno)
	break;
    }
  /* Take copies into account.  */
  for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
    {
      for (cp = ALLOCNO_COPIES (a); cp != NULL; cp = next_cp)
	{
	  if (cp->first == a)
	    {
	      next_cp = cp->next_first_allocno_copy;
	      another_allocno = cp->second;
	    }
	  else if (cp->second == a)
	    {
	      next_cp = cp->next_second_allocno_copy;
	      another_allocno = cp->first;
	    }
	  else
	    gcc_unreachable ();
	  if (cover_class != ALLOCNO_COVER_CLASS (another_allocno)
	      || ALLOCNO_ASSIGNED_P (another_allocno))
	    continue;
	  allocate_and_copy_costs
	    (&ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (another_allocno),
	     cover_class, ALLOCNO_CONFLICT_HARD_REG_COSTS (another_allocno));
	  conflict_costs
	    = ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (another_allocno);
	  if (conflict_costs != NULL
	      && ! ALLOCNO_MAY_BE_SPILLED_P (another_allocno))
	    for (j = class_size - 1; j >= 0; j--)
	      full_costs[j] += conflict_costs[j];
	}
      if (a == allocno)
	break;
    }
  min_cost = min_full_cost = INT_MAX;
  /* We don't care about giving callee saved registers to allocnos no
     living through calls because call clobbered registers are
     allocated first (it is usual practice to put them first in
     REG_ALLOC_ORDER).  */
  for (i = 0; i < class_size; i++)
    {
      hard_regno = class_hard_regs[cover_class][i];
#ifdef STACK_REGS
      if (no_stack_reg_p
	  && FIRST_STACK_REG <= hard_regno && hard_regno <= LAST_STACK_REG)
	continue;
#endif
      if (! hard_reg_not_in_set_p (hard_regno, mode, conflicting_regs)
	  || TEST_HARD_REG_BIT (prohibited_class_mode_regs[cover_class][mode],
				hard_regno))
	continue;
      cost = costs[i];
      full_cost = full_costs[i];
      if (! allocated_hardreg_p[hard_regno]
	  && hard_reg_not_in_set_p (hard_regno, mode, call_used_reg_set))
	/* We need to save/restore the hard register in
	   epilogue/prologue.  Therefore we increase the cost.  */
	{
	  /* ??? If only part is call clobbered.  */
	  class = REGNO_REG_CLASS (hard_regno);
	  add_cost = (memory_move_cost[mode][class][0]
		      + memory_move_cost[mode][class][1] - 1);
	  cost += add_cost;
	  full_cost += add_cost;
	}
      if (min_cost > cost)
	min_cost = cost;
      if (min_full_cost > full_cost)
	{
	  min_full_cost = full_cost;
	  best_hard_regno = hard_regno;
	  ira_assert (hard_regno >= 0);
	}
    }
  if (min_full_cost > mem_cost)
    {
      if (! retry_p && internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	fprintf (ira_dump_file, "(memory is more profitable %d vs %d) ",
		 mem_cost, min_full_cost);
      best_hard_regno = -1;
    }
 fail:
  if (best_hard_regno < 0
      && ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno) != allocno)
    {
      for (j = 0, a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
	   a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
	{
	  sorted_allocnos[j++] = a;
	  if (a == allocno)
	    break;
	}
      qsort (sorted_allocnos, j, sizeof (allocno_t), 
	     allocno_cost_compare_func);
      for (i = 0; i < j; i++)
	{
	  a = sorted_allocnos[i];
	  ALLOCNO_FIRST_COALESCED_ALLOCNO (a) = a;
	  ALLOCNO_NEXT_COALESCED_ALLOCNO (a) = a;
	  VARRAY_PUSH_GENERIC_PTR (allocno_stack_varray, a);
	  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	    {
	      fprintf (ira_dump_file, "        Pushing");
	      print_coalesced_allocno (a);
	      fprintf (ira_dump_file, "\n");
	    }
	}
      return FALSE;
    }
  if (best_hard_regno >= 0)
    allocated_hardreg_p[best_hard_regno] = TRUE;
  for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
    {
      ALLOCNO_HARD_REGNO (a) = best_hard_regno;
      ALLOCNO_ASSIGNED_P (a) = TRUE;
      if (best_hard_regno >= 0)
	update_copy_costs (a, TRUE);
      ira_assert (ALLOCNO_COVER_CLASS (a) == cover_class);
      /* We don't need updated costs anymore: */
      free_allocno_updated_costs (a);
      if (a == allocno)
	break;
    }
  return best_hard_regno >= 0;
}



/* This page contains allocator based on Chaitin-Briggs algorithm.  */

/* Bucket of allocnos that can colored currently without spilling.  */
static allocno_t colorable_allocno_bucket;

/* Bucket of allocnos that might be not colored currently without
   spilling.  */
static allocno_t uncolorable_allocno_bucket;

/* Add ALLOCNO to bucket *BUCKET_PTR.  ALLOCNO should be not in a bucket
   before the call.  */
static void
add_allocno_to_bucket (allocno_t allocno, allocno_t *bucket_ptr)
{
  allocno_t first_allocno;

  first_allocno = *bucket_ptr;
  ALLOCNO_NEXT_BUCKET_ALLOCNO (allocno) = first_allocno;
  ALLOCNO_PREV_BUCKET_ALLOCNO (allocno) = NULL;
  if (first_allocno != NULL)
    ALLOCNO_PREV_BUCKET_ALLOCNO (first_allocno) = allocno;
  *bucket_ptr = allocno;
}

/* The function returns frequency and number of available hard
   registers for allocnos coalesced with ALLOCNO.  */
static void
get_coalesced_allocnos_attributes (allocno_t allocno, int *freq, int *num)
{
  allocno_t a;

  *freq = 0;
  *num = 0;
  for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
    {
      *freq += ALLOCNO_FREQ (a);
      *num += ALLOCNO_AVAILABLE_REGS_NUM (a);
      if (a == allocno)
	break;
    }
}

/* The function compares two allocnos to define what allocno should be
   pushed first into the coloring stack.  If the function returns a
   negative number, the allocno given by the first parameter will be
   pushed first.  In this case such allocno has less priority than the
   second one and the hard register will be assigned to it after
   assignment to the second one.  As the result of such assignment
   order, the second allocno has a better chance to get the best hard
   register.  */
static int
bucket_allocno_compare_func (const void *v1p, const void *v2p)
{
  allocno_t a1 = *(const allocno_t *) v1p, a2 = *(const allocno_t *) v2p;
  int diff, a1_freq, a2_freq, a1_num, a2_num;

  if ((diff = (int) ALLOCNO_COVER_CLASS (a2) - ALLOCNO_COVER_CLASS (a1)) != 0)
    return diff;
  get_coalesced_allocnos_attributes (a1, &a1_freq, &a1_num);
  get_coalesced_allocnos_attributes (a2, &a2_freq, &a2_num);
  if ((diff = a2_num - a1_num) != 0)
    return diff;
  else if ((diff = a1_freq - a2_freq) != 0)
    return diff;
  return ALLOCNO_NUM (a2) - ALLOCNO_NUM (a1);
}

/* Function sorts bucket *BUCKET_PTR and returns the result through
   BUCKET_PTR.  */
static void
sort_bucket (allocno_t *bucket_ptr)
{
  allocno_t a, head;
  int n;

  for (n = 0, a = *bucket_ptr; a != NULL; a = ALLOCNO_NEXT_BUCKET_ALLOCNO (a))
    sorted_allocnos[n++] = a;
  if (n <= 1)
    return;
  qsort (sorted_allocnos, n, sizeof (allocno_t), bucket_allocno_compare_func);
  head = NULL;
  for (n--; n >= 0; n--)
    {
      a = sorted_allocnos[n];
      ALLOCNO_NEXT_BUCKET_ALLOCNO (a) = head;
      ALLOCNO_PREV_BUCKET_ALLOCNO (a) = NULL;
      if (head != NULL)
	ALLOCNO_PREV_BUCKET_ALLOCNO (head) = a;
      head = a;
    }
  *bucket_ptr = head;
}

/* Add ALLOCNO to bucket *BUCKET_PTR maintaining the order according
   their priority.  ALLOCNO should be not in a bucket before the
   call.  */
static void
add_allocno_to_ordered_bucket (allocno_t allocno, allocno_t *bucket_ptr)
{
  allocno_t before, after;

  for (before = *bucket_ptr, after = NULL;
       before != NULL;
       after = before, before = ALLOCNO_NEXT_BUCKET_ALLOCNO (before))
    if (bucket_allocno_compare_func (&allocno, &before) < 0)
      break;
  ALLOCNO_NEXT_BUCKET_ALLOCNO (allocno) = before;
  ALLOCNO_PREV_BUCKET_ALLOCNO (allocno) = after;
  if (after == NULL)
    *bucket_ptr = allocno;
  else
    ALLOCNO_NEXT_BUCKET_ALLOCNO (after) = allocno;
  if (before != NULL)
    ALLOCNO_PREV_BUCKET_ALLOCNO (before) = allocno;
}

/* Delete ALLOCNO from bucket *BUCKET_PTR.  It should be there before
   the call.  */
static void
delete_allocno_from_bucket (allocno_t allocno, allocno_t *bucket_ptr)
{
  allocno_t prev_allocno, next_allocno;

  prev_allocno = ALLOCNO_PREV_BUCKET_ALLOCNO (allocno);
  next_allocno = ALLOCNO_NEXT_BUCKET_ALLOCNO (allocno);
  if (prev_allocno != NULL)
    ALLOCNO_NEXT_BUCKET_ALLOCNO (prev_allocno) = next_allocno;
  else
    {
      ira_assert (*bucket_ptr == allocno);
      *bucket_ptr = next_allocno;
    }
  if (next_allocno != NULL)
    ALLOCNO_PREV_BUCKET_ALLOCNO (next_allocno) = prev_allocno;
}

/* The function puts ALLOCNO onto the coloring stack without removing
   it from its bucket.  Pushing allocno to the coloring stack can
   result in moving conflicting allocnos from the uncolorable bucket
   to the colorable one.  */
static void
push_allocno_to_stack (allocno_t allocno)
{
  int conflicts_num, conflict_size, size;
  allocno_t a, conflict_allocno;
  enum reg_class cover_class;
  allocno_conflict_iterator aci;
  
  ALLOCNO_IN_GRAPH_P (allocno) = FALSE;
  VARRAY_PUSH_GENERIC_PTR (allocno_stack_varray, allocno);
  cover_class = ALLOCNO_COVER_CLASS (allocno);
  if (cover_class == NO_REGS)
    return;
  size = reg_class_nregs[cover_class][ALLOCNO_MODE (allocno)];
  if (allocno_coalesced_p)
    bitmap_clear (processed_coalesced_allocno_bitmap);
  for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
    {
      FOR_EACH_ALLOCNO_CONFLICT (a, conflict_allocno, aci)
	if (bitmap_bit_p (coloring_allocno_bitmap,
			  ALLOCNO_NUM (conflict_allocno)))
	  {
	    ira_assert (cover_class == ALLOCNO_COVER_CLASS (conflict_allocno));
	    if (allocno_coalesced_p)
	      {
		if (bitmap_bit_p (processed_coalesced_allocno_bitmap,
				  ALLOCNO_NUM (conflict_allocno)))
		  continue;
		bitmap_set_bit (processed_coalesced_allocno_bitmap,
				ALLOCNO_NUM (conflict_allocno));
	      }
	    if (ALLOCNO_IN_GRAPH_P (conflict_allocno)
		&& ! ALLOCNO_ASSIGNED_P (conflict_allocno))
	      {
		conflicts_num = ALLOCNO_LEFT_CONFLICTS_NUM (conflict_allocno);
		conflict_size
		  = (reg_class_nregs
		     [cover_class][ALLOCNO_MODE (conflict_allocno)]);
		ira_assert
		  (ALLOCNO_LEFT_CONFLICTS_NUM (conflict_allocno) >= size);
		ALLOCNO_LEFT_CONFLICTS_NUM (conflict_allocno) -= size;
		if (conflicts_num + conflict_size
		    <= ALLOCNO_AVAILABLE_REGS_NUM (conflict_allocno))
		  continue;
		conflicts_num = ALLOCNO_LEFT_CONFLICTS_NUM (conflict_allocno);
		if (conflicts_num + conflict_size
		    <= ALLOCNO_AVAILABLE_REGS_NUM (conflict_allocno))
		  {
		    delete_allocno_from_bucket
		      (conflict_allocno, &uncolorable_allocno_bucket);
		    add_allocno_to_ordered_bucket (conflict_allocno,
						   &colorable_allocno_bucket);
		  }
	      }
	  }
      if (a == allocno)
	break;
    }
}

/* The function puts ALLOCNO onto the coloring stack and removes it
   from its bucket.  The allocno is in the colorable bucket if
   COLORABLE_P is TRUE.  */
static void
remove_allocno_from_bucket_and_push (allocno_t allocno, int colorable_p)
{
  enum reg_class cover_class;
  allocno_t *bucket_ptr;

  bucket_ptr = (colorable_p
		? &colorable_allocno_bucket : &uncolorable_allocno_bucket);
  delete_allocno_from_bucket (allocno, bucket_ptr);
  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
    {
      fprintf (ira_dump_file, "      Pushing");
      print_coalesced_allocno (allocno);
      fprintf (ira_dump_file, "%s\n", colorable_p ? "" : "(potential spill)");
    }
  cover_class = ALLOCNO_COVER_CLASS (allocno);
  ira_assert ((colorable_p
	       && (ALLOCNO_LEFT_CONFLICTS_NUM (allocno)
		   + reg_class_nregs[cover_class][ALLOCNO_MODE (allocno)]
		   <= ALLOCNO_AVAILABLE_REGS_NUM (allocno)))
	      || (! colorable_p
		  && (ALLOCNO_LEFT_CONFLICTS_NUM (allocno)
		      + reg_class_nregs[cover_class][ALLOCNO_MODE (allocno)]
		      > ALLOCNO_AVAILABLE_REGS_NUM (allocno))));
  if (! colorable_p)
    ALLOCNO_MAY_BE_SPILLED_P (allocno) = TRUE;
  push_allocno_to_stack (allocno);
}

/* The function puts all allocnos from colorable bucket onto the
   coloring stack.  */
static void
push_only_colorable (void)
{
  sort_bucket (&colorable_allocno_bucket);
  for (;colorable_allocno_bucket != NULL;)
    remove_allocno_from_bucket_and_push (colorable_allocno_bucket, TRUE);
}

/* The function puts ALLOCNO chosen for potential spilling onto the
   coloring stack.  */
static void
push_allocno_to_spill (allocno_t allocno)
{
  delete_allocno_from_bucket (allocno, &uncolorable_allocno_bucket);
  ALLOCNO_MAY_BE_SPILLED_P (allocno) = TRUE;
  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
    fprintf (ira_dump_file, "      Pushing p%d(%d) (potential spill)\n",
	     ALLOCNO_NUM (allocno), ALLOCNO_REGNO (allocno));
  push_allocno_to_stack (allocno);
}

/* The function returns frequency of exit edges (if EXIT_P) or entry
   from/to the loop given by its LOOP_NODE.  */ 
int
loop_edge_freq (loop_tree_node_t loop_node, int regno, int exit_p)
{
  int freq, i;
  edge_iterator ei;
  edge e;
  VEC (edge, heap) *edges;

  ira_assert (loop_node->loop != NULL
	      && (regno < 0 || regno >= FIRST_PSEUDO_REGISTER));
  freq = 0;
  if (! exit_p)
    {
      FOR_EACH_EDGE (e, ei, loop_node->loop->header->preds)
	if (e->src != loop_node->loop->latch
	    && (regno < 0
		|| (bitmap_bit_p (DF_LR_OUT (e->src), regno)
		    && bitmap_bit_p (DF_LR_IN (e->dest), regno))))
	  freq += EDGE_FREQUENCY (e);
    }
  else
    {
      edges = get_loop_exit_edges (loop_node->loop);
      for (i = 0; VEC_iterate (edge, edges, i, e); i++)
	if (regno < 0
	    || (bitmap_bit_p (DF_LR_OUT (e->src), regno)
		&& bitmap_bit_p (DF_LR_IN (e->dest), regno)))
	  freq += EDGE_FREQUENCY (e);
      VEC_free (edge, heap, edges);
    }

  return REG_FREQ_FROM_EDGE_FREQ (freq);
}

/* The function calculates and returns cost of putting allocno A into
   memory.  */
static int
calculate_allocno_spill_cost (allocno_t a)
{
  int regno, cost;
  enum machine_mode mode;
  enum reg_class class;
  allocno_t father_allocno;
  loop_tree_node_t father_node, loop_node;

  regno = ALLOCNO_REGNO (a);
  cost = ALLOCNO_UPDATED_MEMORY_COST (a) - ALLOCNO_COVER_CLASS_COST (a);
  if (ALLOCNO_CAP (a) != NULL)
    return cost;
  loop_node = ALLOCNO_LOOP_TREE_NODE (a);
  if ((father_node = loop_node->father) == NULL)
    return cost;
  if ((father_allocno = father_node->regno_allocno_map[regno]) == NULL)
    return cost;
  mode = ALLOCNO_MODE (a);
  class = ALLOCNO_COVER_CLASS (a);
  if (ALLOCNO_HARD_REGNO (father_allocno) < 0)
    cost -= (memory_move_cost[mode][class][0]
	     * loop_edge_freq (loop_node, regno, TRUE)
	     + memory_move_cost[mode][class][1]
	     * loop_edge_freq (loop_node, regno, FALSE));
  else
    cost += ((memory_move_cost[mode][class][1]
	      * loop_edge_freq (loop_node, regno, TRUE)
	      + memory_move_cost[mode][class][0]
	      * loop_edge_freq (loop_node, regno, FALSE))
	     - (register_move_cost[mode][class][class]
		* (loop_edge_freq (loop_node, regno, FALSE)
		   + loop_edge_freq (loop_node, regno, TRUE))));
  return cost;
}

/* Push allocnos to the coloring stack.  The order of allocnos in the
   stack defines the order for the subsequent coloring.  */
static void
push_allocnos_to_stack (void)
{
  int i, j;
  int allocno_pri, i_allocno_pri;
  int allocno_cost, i_allocno_cost;
  allocno_t allocno, i_allocno;
  allocno_t *allocno_vec;
  enum reg_class cover_class;
  int num, cover_class_allocnos_num[N_REG_CLASSES];
  allocno_t *cover_class_allocnos[N_REG_CLASSES];

  /* Initialize.  */
  for (i = 0; i < reg_class_cover_size; i++)
    {
      cover_class = reg_class_cover[i];
      cover_class_allocnos_num[cover_class] = 0;
      cover_class_allocnos[cover_class] = NULL;
    }
  /* Calculate uncolorable allocnos of each cover class.  */
  for (allocno = uncolorable_allocno_bucket;
       allocno != NULL;
       allocno = ALLOCNO_NEXT_BUCKET_ALLOCNO (allocno))
    if ((cover_class = ALLOCNO_COVER_CLASS (allocno)) != NO_REGS)
      {
	cover_class_allocnos_num[cover_class]++;
	ALLOCNO_TEMP (allocno) = INT_MAX;
      }
  /* Define place where to put uncolorable allocnos of the same cover
     class.  */
  for (num = i = 0; i < reg_class_cover_size; i++)
    {
      cover_class = reg_class_cover[i];
      if (cover_class_allocnos_num[cover_class] != 0)
	{
	  cover_class_allocnos[cover_class]
	    = sorted_allocnos_for_spilling + num;
	  num += cover_class_allocnos_num[cover_class];
	  cover_class_allocnos_num[cover_class] = 0;
	}
    }
  ira_assert (num <= allocnos_num);
  /* Put uncolorable allocnos of the same cover class together.  */
  for (allocno = uncolorable_allocno_bucket;
       allocno != NULL;
       allocno = ALLOCNO_NEXT_BUCKET_ALLOCNO (allocno))
    if ((cover_class = ALLOCNO_COVER_CLASS (allocno)) != NO_REGS)
      cover_class_allocnos
	[cover_class][cover_class_allocnos_num[cover_class]++] = allocno;
  for (;;)
    {
      push_only_colorable ();
      allocno = uncolorable_allocno_bucket;
      if (allocno == NULL)
	break;
      cover_class = ALLOCNO_COVER_CLASS (allocno);
      if (cover_class == NO_REGS)
	{
	  push_allocno_to_spill (allocno);
	  continue;
	}
      /* Potential spilling.  */
      ira_assert (reg_class_nregs[cover_class][ALLOCNO_MODE (allocno)] > 0);
      num = cover_class_allocnos_num[cover_class];
      ira_assert (num > 0);
      allocno_vec = cover_class_allocnos[cover_class];
      allocno = NULL;
      allocno_pri = allocno_cost = 0;
      /* Sort uncolorable allocno to find the one with the lowest spill
	 cost.  */
      for (i = 0, j = num - 1; i <= j;)
	{
	  i_allocno = allocno_vec[i];
	  if (! ALLOCNO_IN_GRAPH_P (i_allocno)
	      && ALLOCNO_IN_GRAPH_P (allocno_vec[j]))
	    {
	      i_allocno = allocno_vec[j];
	      allocno_vec[j] = allocno_vec[i];
	      allocno_vec[i] = i_allocno;
	    }
	  if (ALLOCNO_IN_GRAPH_P (i_allocno))
	    {
	      i++;
	      if (ALLOCNO_TEMP (i_allocno) == INT_MAX)
		{
		  allocno_t a;
		  int cost = 0;

		  for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (i_allocno);;
		       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
		    {
		      cost += calculate_allocno_spill_cost (i_allocno);
		      if (a == i_allocno)
			break;
		    }
		  /* ??? Remove cost of copies between the coalesced
		     allocnos.  */
		  ALLOCNO_TEMP (i_allocno) = cost;
		}
	      i_allocno_cost = ALLOCNO_TEMP (i_allocno);
	      i_allocno_pri
		= (i_allocno_cost
		   / (ALLOCNO_LEFT_CONFLICTS_NUM (i_allocno)
		      * reg_class_nregs[ALLOCNO_COVER_CLASS (i_allocno)]
		                       [ALLOCNO_MODE (i_allocno)] + 1));
	      if (allocno == NULL || allocno_pri > i_allocno_pri
		  || (allocno_pri == i_allocno_pri
		      && (allocno_cost > i_allocno_cost
			  || (allocno_cost == i_allocno_cost 
			      && (ALLOCNO_NUM (allocno)
				  > ALLOCNO_NUM (i_allocno))))))
		{
		  allocno = i_allocno;
		  allocno_cost = i_allocno_cost;
		  allocno_pri = i_allocno_pri;
		}
	    }
	  if (! ALLOCNO_IN_GRAPH_P (allocno_vec[j]))
	    j--;
	}
      ira_assert (allocno != NULL && j >= 0);
      cover_class_allocnos_num[cover_class] = j + 1;
      ira_assert (ALLOCNO_IN_GRAPH_P (allocno)
		  && ALLOCNO_COVER_CLASS (allocno) == cover_class
		  && (ALLOCNO_LEFT_CONFLICTS_NUM (allocno)
		      + reg_class_nregs[cover_class][ALLOCNO_MODE (allocno)]
		      > ALLOCNO_AVAILABLE_REGS_NUM (allocno)));
      remove_allocno_from_bucket_and_push (allocno, FALSE);
    }
}

/* Pop the coloring stack and assign hard registers to the popped
   allocnos.  */
static void
pop_allocnos_from_stack (void)
{
  allocno_t allocno;
  enum reg_class cover_class;

  for (;VARRAY_ACTIVE_SIZE (allocno_stack_varray) != 0;)
    {
      allocno = VARRAY_TOP_GENERIC_PTR (allocno_stack_varray);
      VARRAY_POP (allocno_stack_varray);
      cover_class = ALLOCNO_COVER_CLASS (allocno);
      if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	{
	  fprintf (ira_dump_file, "      Popping");
	  print_coalesced_allocno (allocno);
	  fprintf (ira_dump_file, "  -- ");
	}
      if (cover_class == NO_REGS)
	{
	  ALLOCNO_HARD_REGNO (allocno) = -1;
	  ALLOCNO_ASSIGNED_P (allocno) = TRUE;
	  ira_assert (ALLOCNO_UPDATED_HARD_REG_COSTS (allocno) == NULL);
	  ira_assert
	    (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (allocno) == NULL);
	  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	    fprintf (ira_dump_file, "assign memory\n");
	}
      else if (assign_hard_reg (allocno, FALSE))
	{
	  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	    fprintf (ira_dump_file, "assign reg %d\n",
		     ALLOCNO_HARD_REGNO (allocno));
	}
      else if (ALLOCNO_ASSIGNED_P (allocno))
	{
	  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	    fprintf (ira_dump_file, "spill\n");
	}
      ALLOCNO_IN_GRAPH_P (allocno) = TRUE;
    }
}

/* Set up number of available hard registers for ALLOCNO.  */
static void
setup_allocno_available_regs_num (allocno_t allocno)
{
  int i, n, hard_regs_num;
  enum reg_class cover_class;
  allocno_t a;
  HARD_REG_SET temp_set;

  cover_class = ALLOCNO_COVER_CLASS (allocno);
  ALLOCNO_AVAILABLE_REGS_NUM (allocno) = available_class_regs[cover_class];
  if (cover_class == NO_REGS)
    return;
  CLEAR_HARD_REG_SET (temp_set);
  ira_assert (ALLOCNO_FIRST_COALESCED_ALLOCNO (allocno) == allocno);
  hard_regs_num = class_hard_regs_num[cover_class];
  for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
    {
      IOR_HARD_REG_SET (temp_set, ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
      if (a == allocno)
	break;
    }
  for (n = 0, i = hard_regs_num - 1; i >= 0; i--)
    if (TEST_HARD_REG_BIT (temp_set, class_hard_regs[cover_class][i]))
      n++;
  if (internal_flag_ira_verbose > 2 && n > 0 && ira_dump_file != NULL)
    fprintf (ira_dump_file, "    Reg %d of %s has %d regs less\n",
	     ALLOCNO_REGNO (allocno), reg_class_names[cover_class], n);
  ALLOCNO_AVAILABLE_REGS_NUM (allocno) -= n;
}

/* Set up ALLOCNO_LEFT_CONFLICTS_NUM for ALLOCNO.  */
static void
setup_allocno_left_conflicts_num (allocno_t allocno)
{
  int i, hard_regs_num, hard_regno, conflict_allocnos_size;
  allocno_t a, conflict_allocno;
  enum reg_class cover_class;
  HARD_REG_SET temp_set;
  allocno_conflict_iterator aci;

  cover_class = ALLOCNO_COVER_CLASS (allocno);
  hard_regs_num = class_hard_regs_num[cover_class];
  CLEAR_HARD_REG_SET (temp_set);
  ira_assert (ALLOCNO_FIRST_COALESCED_ALLOCNO (allocno) == allocno);
  for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
    {
      IOR_HARD_REG_SET (temp_set, ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
      if (a == allocno)
	break;
    }
  AND_HARD_REG_SET (temp_set, reg_class_contents[cover_class]);
  AND_COMPL_HARD_REG_SET (temp_set, no_alloc_regs);
  conflict_allocnos_size = 0;
  if (! hard_reg_set_equal_p (temp_set, zero_hard_reg_set))
    for (i = 0; i < (int) hard_regs_num; i++)
      {
	hard_regno = class_hard_regs[cover_class][i];
	if (TEST_HARD_REG_BIT (temp_set, hard_regno))
	  {
	    conflict_allocnos_size++;
	    CLEAR_HARD_REG_BIT (temp_set, hard_regno);
	    if (hard_reg_set_equal_p (temp_set, zero_hard_reg_set))
	      break;
	  }
      }
  CLEAR_HARD_REG_SET (temp_set);
  if (allocno_coalesced_p)
    bitmap_clear (processed_coalesced_allocno_bitmap);
  if (cover_class != NO_REGS)
    for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
	 a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
      {
	FOR_EACH_ALLOCNO_CONFLICT (a, conflict_allocno, aci)
	  if (bitmap_bit_p (consideration_allocno_bitmap,
			    ALLOCNO_NUM (conflict_allocno)))
	    {
	      ira_assert (cover_class
			  == ALLOCNO_COVER_CLASS (conflict_allocno));
	      if (allocno_coalesced_p)
		{
		  if (bitmap_bit_p (processed_coalesced_allocno_bitmap,
				    ALLOCNO_NUM (conflict_allocno)))
		    continue;
		  bitmap_set_bit (processed_coalesced_allocno_bitmap,
				  ALLOCNO_NUM (conflict_allocno));
		}
	      if (! ALLOCNO_ASSIGNED_P (conflict_allocno))
		conflict_allocnos_size
		  += (reg_class_nregs
		      [cover_class][ALLOCNO_MODE (conflict_allocno)]);
	      else if ((hard_regno = ALLOCNO_HARD_REGNO (conflict_allocno))
		       >= 0)
		{
		  int last = (hard_regno
			      + hard_regno_nregs
			        [hard_regno][ALLOCNO_MODE (conflict_allocno)]);
		  
		  while (hard_regno < last)
		    {
		      if (! TEST_HARD_REG_BIT (temp_set, hard_regno))
			{
			  conflict_allocnos_size++;
			  SET_HARD_REG_BIT (temp_set, hard_regno);
			}
		      hard_regno++;
		    }
		}
	    }
        if (a == allocno)
	  break;
      }
  ALLOCNO_LEFT_CONFLICTS_NUM (allocno) = conflict_allocnos_size;
}

/* The function put ALLOCNO in a bucket corresponding to its number and
   size of its conflicting allocnos and hard registers.  */
static void
put_allocno_into_bucket (allocno_t allocno)
{
  int hard_regs_num;
  enum reg_class cover_class;

  cover_class = ALLOCNO_COVER_CLASS (allocno);
  hard_regs_num = class_hard_regs_num[cover_class];
  if (ALLOCNO_FIRST_COALESCED_ALLOCNO (allocno) != allocno)
    return;
  ALLOCNO_IN_GRAPH_P (allocno) = TRUE;
  setup_allocno_left_conflicts_num (allocno);
  setup_allocno_available_regs_num (allocno);
  if (ALLOCNO_LEFT_CONFLICTS_NUM (allocno)
      + reg_class_nregs[cover_class][ALLOCNO_MODE (allocno)]
      <= ALLOCNO_AVAILABLE_REGS_NUM (allocno))
    add_allocno_to_bucket (allocno, &colorable_allocno_bucket);
  else
    add_allocno_to_bucket (allocno, &uncolorable_allocno_bucket);
}

/* The function is used to sort allocnos according to their execution
   frequencies.  */
static int
copy_freq_compare_func (const void *v1p, const void *v2p)
{
  copy_t cp1 = *(const copy_t *) v1p, cp2 = *(const copy_t *) v2p;
  int pri1, pri2;

  pri1 = cp1->freq;
  pri2 = cp2->freq;
  if (pri2 - pri1)
    return pri2 - pri1;

  /* If freqencies are equal, sort by copies, so that the results of
     qsort leave nothing to chance.  */
  return cp1->num - cp2->num;
}

/* The function merges two sets of coalesced allocnos given
   correspondingly by allocnos A1 and A2 (more accurately merging A2
   set into A1 set).  */
static void
merge_allocnos (allocno_t a1, allocno_t a2)
{
  allocno_t a, first, last, next;

  first = ALLOCNO_FIRST_COALESCED_ALLOCNO (a1);
  if (first == ALLOCNO_FIRST_COALESCED_ALLOCNO (a2))
    return;
  for (last = a2, a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a2);;
       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
    {
      ALLOCNO_FIRST_COALESCED_ALLOCNO (a) = first;
      if (a == a2)
	break;
      last = a;
    }
  next = ALLOCNO_NEXT_COALESCED_ALLOCNO (first);
  ALLOCNO_NEXT_COALESCED_ALLOCNO (first) = a2;
  ALLOCNO_NEXT_COALESCED_ALLOCNO (last) = next;
}

/* The function returns TRUE if there are conflicting allocnos from
   two sets of coalesced allocnos given correspondingly by allocnos
   A1 and A2.  If RELOAD_P is TRUE, we use live ranges to find
   conflicts because conflicts are represented only for allocnos of
   the same cover class and during the reload pass we coalesce
   allocnos for sharing stack memory slots.  */
static int
coalesced_allocno_conflict_p (allocno_t a1, allocno_t a2, int reload_p)
{
  allocno_t a, conflict_allocno;
  allocno_conflict_iterator aci;

  if (allocno_coalesced_p)
    {
      bitmap_clear (processed_coalesced_allocno_bitmap);
      for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a1);;
	   a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
	{
	  bitmap_set_bit (processed_coalesced_allocno_bitmap, ALLOCNO_NUM (a));
	  if (a == a1)
	    break;
	}
    }
  for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a2);;
       a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
    {
      if (reload_p)
	{
	  for (conflict_allocno = ALLOCNO_NEXT_COALESCED_ALLOCNO (a1);;
	       conflict_allocno
		 = ALLOCNO_NEXT_COALESCED_ALLOCNO (conflict_allocno))
	    {
	      if (allocno_live_ranges_intersect_p (a, conflict_allocno))
		return TRUE;
	      if (conflict_allocno == a1)
		break;
	    }
	}
      else
	{
	  FOR_EACH_ALLOCNO_CONFLICT (a, conflict_allocno, aci)
	    if (conflict_allocno == a1
		|| (allocno_coalesced_p
		    && bitmap_bit_p (processed_coalesced_allocno_bitmap,
				     ALLOCNO_NUM (conflict_allocno))))
	      return TRUE;
	}
      if (a == a2)
	break;
    }
  return FALSE;
}

/* The major function for aggressive allocno coalescing.  For the
   reload pass (RELOAD_P) we coalesce only spilled allocnos.  If some
   allocnos have been coalesced, we set up flag
   allocno_coalesced_p.  */
static void
coalesce_allocnos (int reload_p)
{
  allocno_t a;
  copy_t cp, next_cp, *sorted_copies;
  enum reg_class cover_class;
  enum machine_mode mode;
  unsigned int j;
  int i, n, cp_num, regno;
  bitmap_iterator bi;

  sorted_copies = ira_allocate (copies_num * sizeof (copy_t));
  cp_num = 0;
  /* Collect copies.  */
  EXECUTE_IF_SET_IN_BITMAP (coloring_allocno_bitmap, 0, j, bi)
    {
      a = allocnos[j];
      regno = ALLOCNO_REGNO (a);
      if ((! reload_p && ALLOCNO_ASSIGNED_P (a))
	  || (reload_p
	      && (! ALLOCNO_ASSIGNED_P (a) || ALLOCNO_HARD_REGNO (a) >= 0
		  || (regno < reg_equiv_len
		      && (reg_equiv_const[regno] != NULL_RTX
			  || reg_equiv_invariant_p[regno])))))
	continue;
      cover_class = ALLOCNO_COVER_CLASS (a);
      mode = ALLOCNO_MODE (a);
      for (cp = ALLOCNO_COPIES (a); cp != NULL; cp = next_cp)
	{
	  if (cp->first == a)
	    {
	      next_cp = cp->next_first_allocno_copy;
	      regno = ALLOCNO_REGNO (cp->second);
	      if ((reload_p
		   || (ALLOCNO_COVER_CLASS (cp->second) == cover_class
		       && ALLOCNO_MODE (cp->second) == mode))
		  && cp->insn != NULL
		  && ((! reload_p && ! ALLOCNO_ASSIGNED_P (cp->second))
		      || (reload_p
			  && ALLOCNO_ASSIGNED_P (cp->second)
			  && ALLOCNO_HARD_REGNO (cp->second) < 0
			  && (regno >= reg_equiv_len
			      || (! reg_equiv_invariant_p[regno]
				  && reg_equiv_const[regno] == NULL_RTX)))))
		sorted_copies[cp_num++] = cp;
	    }
	  else if (cp->second == a)
	    next_cp = cp->next_second_allocno_copy;
	  else
	    gcc_unreachable ();
	}
    }
  qsort (sorted_copies, cp_num, sizeof (copy_t), copy_freq_compare_func);
  /* Coalesced copies, most frequently executed first.  */
  for (; cp_num != 0;)
    {
      for (i = 0; i < cp_num; i++)
	{
	  cp = sorted_copies[i];
	  if (! coalesced_allocno_conflict_p (cp->first, cp->second, reload_p))
	    {
	      allocno_coalesced_p = TRUE;
	      if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
		fprintf
		  (ira_dump_file,
		   "      Coalescing copy %d:a%dr%d-a%dr%d (freq=%d)\n",
		   cp->num, ALLOCNO_NUM (cp->first), ALLOCNO_REGNO (cp->first),
		   ALLOCNO_NUM (cp->second), ALLOCNO_REGNO (cp->second),
		   cp->freq);
	      merge_allocnos (cp->first, cp->second);
	      i++;
	      break;
	    }
	}
      /* Collect the rest of copies.  */
      for (n = 0; i < cp_num; i++)
	{
	  cp = sorted_copies[i];
	  if (ALLOCNO_FIRST_COALESCED_ALLOCNO (cp->first)
	      != ALLOCNO_FIRST_COALESCED_ALLOCNO (cp->second))
	    sorted_copies[n++] = cp;
	}
      cp_num = n;
    }
  ira_free (sorted_copies);
}

/* Function implements Chaitin-Briggs coloring for allocnos in
   COLORING_ALLOCNO_BITMAP taking into account allocnos in
   CONSIDERATION_ALLOCNO_BITMAP.  */
static void
color_allocnos (void)
{
  unsigned int i;
  bitmap_iterator bi;
  allocno_t a;

  allocno_coalesced_p = FALSE;
  processed_coalesced_allocno_bitmap = ira_allocate_bitmap ();
  if (flag_ira_coalesce)
    coalesce_allocnos (FALSE);
  /* Put the allocnos into the corresponding buckets.  */
  colorable_allocno_bucket = NULL;
  uncolorable_allocno_bucket = NULL;
  EXECUTE_IF_SET_IN_BITMAP (coloring_allocno_bitmap, 0, i, bi)
    {
      a = allocnos[i];
      if (ALLOCNO_COVER_CLASS (a) == NO_REGS)
	{
	  ALLOCNO_HARD_REGNO (a) = -1;
	  ALLOCNO_ASSIGNED_P (a) = TRUE;
	  ira_assert (ALLOCNO_UPDATED_HARD_REG_COSTS (a) == NULL);
	  ira_assert (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) == NULL);
	  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	    {
	      fprintf (ira_dump_file, "      Spill");
	      print_coalesced_allocno (a);
	      fprintf (ira_dump_file, "\n");
	    }
	  continue;
	}
      put_allocno_into_bucket (a);
    }
  push_allocnos_to_stack ();
  pop_allocnos_from_stack ();
  if (flag_ira_coalesce)
    /* We don't need coalesced allocnos for reassign_pseudos.  */
    EXECUTE_IF_SET_IN_BITMAP (coloring_allocno_bitmap, 0, i, bi)
      {
	a = allocnos[i];
	ALLOCNO_FIRST_COALESCED_ALLOCNO (a) = a;
	ALLOCNO_NEXT_COALESCED_ALLOCNO (a) = a;
      }
  ira_free_bitmap (processed_coalesced_allocno_bitmap);
  allocno_coalesced_p = FALSE;
}



/* The function outputs information about the loop given by its
   LOOP_TREE_NODE. */
static void
print_loop_title (loop_tree_node_t loop_tree_node)
{
  unsigned int j;
  bitmap_iterator bi;

  ira_assert (loop_tree_node->loop != NULL);
  fprintf (ira_dump_file,
	   "\n  Loop %d (father %d, header bb%d, depth %d)\n    ref:",
	   loop_tree_node->loop->num,
	   (loop_tree_node->father == NULL
	    ? -1 : loop_tree_node->father->loop->num),
	   loop_tree_node->loop->header->index,
	   loop_depth (loop_tree_node->loop));
  EXECUTE_IF_SET_IN_BITMAP (loop_tree_node->mentioned_allocnos, 0, j, bi)
    fprintf (ira_dump_file, " %dr%d", j, ALLOCNO_REGNO (allocnos[j]));
  fprintf (ira_dump_file, "\n    modified regnos:");
  EXECUTE_IF_SET_IN_BITMAP (loop_tree_node->modified_regnos, 0, j, bi)
    fprintf (ira_dump_file, " %d", j);
  fprintf (ira_dump_file, "\n    border:");
  EXECUTE_IF_SET_IN_BITMAP (loop_tree_node->border_allocnos, 0, j, bi)
    fprintf (ira_dump_file, " %dr%d", j, ALLOCNO_REGNO (allocnos[j]));
  fprintf (ira_dump_file, "\n    Pressure:");
  for (j = 0; (int) j < reg_class_cover_size; j++)
    {
      enum reg_class cover_class;
      
      cover_class = reg_class_cover[j];
      if (loop_tree_node->reg_pressure[cover_class] == 0)
	continue;
      fprintf (ira_dump_file, " %s=%d", reg_class_names[cover_class],
	       loop_tree_node->reg_pressure[cover_class]);
    }
  fprintf (ira_dump_file, "\n");
}

/* The function does coloring for allocnos inside loop (in extreme
   case it can be all function) given by the corresponding
   LOOP_TREE_NODE.  The function is called for each loop during
   top-down traverse of the loop tree.  */
static void
color_pass (loop_tree_node_t loop_tree_node)
{
  int regno, hard_regno, index = -1;
  int cost, exit_freq, enter_freq;
  unsigned int j;
  bitmap_iterator bi;
  enum machine_mode mode;
  enum reg_class class, cover_class;
  allocno_t a, subloop_allocno;
  loop_tree_node_t subloop_node;

  if (loop_tree_node->loop == NULL)
    return;
  if (internal_flag_ira_verbose > 1 && ira_dump_file != NULL)
    print_loop_title (loop_tree_node);

  bitmap_copy (coloring_allocno_bitmap, loop_tree_node->mentioned_allocnos);
  bitmap_ior_into (coloring_allocno_bitmap, loop_tree_node->border_allocnos);
  bitmap_copy (consideration_allocno_bitmap, coloring_allocno_bitmap);
  EXECUTE_IF_SET_IN_BITMAP (consideration_allocno_bitmap, 0, j, bi)
    {
      a = allocnos[j];
      if (! ALLOCNO_ASSIGNED_P (a))
	continue;
      bitmap_clear_bit (coloring_allocno_bitmap, ALLOCNO_NUM (a));
    }
  /* Color all mentioned allocnos including transparent ones.  */
  color_allocnos ();
  /* Update costs of the corresponding allocnos in the subloops.  */
  for (subloop_node = loop_tree_node->children;
       subloop_node != NULL;
       subloop_node = subloop_node->next)
    if (subloop_node->bb == NULL)
      EXECUTE_IF_SET_IN_BITMAP (consideration_allocno_bitmap, 0, j, bi)
        {
	  a = allocnos[j];
	  mode = ALLOCNO_MODE (a);
	  class = ALLOCNO_COVER_CLASS (a);
	  hard_regno = ALLOCNO_HARD_REGNO (a);
	  if (hard_regno >= 0)
	    {
	      index = class_hard_reg_index[class][hard_regno];
	      ira_assert (index >= 0);
	    }
	  regno = ALLOCNO_REGNO (a);
	  /* ??? conflict costs */
	  if (ALLOCNO_CAP_MEMBER (a) == NULL)
	    {
	      subloop_allocno = subloop_node->regno_allocno_map[regno];
	      if (subloop_allocno == NULL)
		continue;
	      if ((flag_ira_algorithm == IRA_ALGORITHM_MIXED
		   && (loop_tree_node->reg_pressure[class]
		       <= available_class_regs[class]))
		  || (hard_regno < 0
		      && ! bitmap_bit_p (subloop_node->mentioned_allocnos,
					 ALLOCNO_NUM (subloop_allocno))))
		{
		  if (! ALLOCNO_ASSIGNED_P (subloop_allocno))
		    {
		      ALLOCNO_HARD_REGNO (subloop_allocno) = hard_regno;
		      ALLOCNO_ASSIGNED_P (subloop_allocno) = TRUE;
		      if (hard_regno >= 0)
			update_copy_costs (subloop_allocno, TRUE);
		      /* We don't need updated costs anymore: */
		      free_allocno_updated_costs (subloop_allocno);
		    }
		  continue;
		}
	      exit_freq = loop_edge_freq (subloop_node, regno, TRUE);
	      enter_freq = loop_edge_freq (subloop_node, regno, FALSE);
	      ira_assert (regno < reg_equiv_len);
	      if (reg_equiv_invariant_p[regno]
		  || reg_equiv_const[regno] != NULL_RTX)
		{
		  if (! ALLOCNO_ASSIGNED_P (subloop_allocno))
		    {
		      ALLOCNO_HARD_REGNO (subloop_allocno) = hard_regno;
		      ALLOCNO_ASSIGNED_P (subloop_allocno) = TRUE;
		      if (hard_regno >= 0)
			update_copy_costs (subloop_allocno, TRUE);
		      /* We don't need updated costs anymore: */
		      free_allocno_updated_costs (subloop_allocno);
		    }
		}
	      else if (hard_regno < 0)
		{
		  ALLOCNO_UPDATED_MEMORY_COST (subloop_allocno)
		    -= ((memory_move_cost[mode][class][1] * enter_freq)
			+ (memory_move_cost[mode][class][0] * exit_freq));
		}
	      else
		{
		  cover_class = ALLOCNO_COVER_CLASS (subloop_allocno);
		  allocate_and_set_costs
		    (&ALLOCNO_HARD_REG_COSTS (subloop_allocno), cover_class,
		     ALLOCNO_COVER_CLASS_COST (subloop_allocno));
		  allocate_and_set_costs
		    (&ALLOCNO_CONFLICT_HARD_REG_COSTS (subloop_allocno),
		     cover_class, 0);
		  cost = (register_move_cost[mode][class][class] 
			  * (exit_freq + enter_freq));
		  ALLOCNO_HARD_REG_COSTS (subloop_allocno)[index] -= cost;
		  ALLOCNO_CONFLICT_HARD_REG_COSTS (subloop_allocno)[index]
		    -= cost;
		  ALLOCNO_UPDATED_MEMORY_COST (subloop_allocno)
		    += (memory_move_cost[mode][class][0] * enter_freq
			+ memory_move_cost[mode][class][1] * exit_freq);
		  if (ALLOCNO_COVER_CLASS_COST (subloop_allocno)
		      > ALLOCNO_HARD_REG_COSTS (subloop_allocno)[index])
		    ALLOCNO_COVER_CLASS_COST (subloop_allocno)
		      = ALLOCNO_HARD_REG_COSTS (subloop_allocno)[index];
		}
	    }
	  else
	    {
	      subloop_allocno = ALLOCNO_CAP_MEMBER (a);
	      if (ALLOCNO_LOOP_TREE_NODE (subloop_allocno) != subloop_node)
		continue;
	      if ((flag_ira_algorithm == IRA_ALGORITHM_MIXED
		   && loop_tree_node->reg_pressure[class]
		      <= available_class_regs[class])
		  || (hard_regno < 0
		      && ! bitmap_bit_p (subloop_node->mentioned_allocnos,
					 ALLOCNO_NUM (subloop_allocno))))
		{
		  if (! ALLOCNO_ASSIGNED_P (subloop_allocno))
		    {
		      ALLOCNO_HARD_REGNO (subloop_allocno) = hard_regno;
		      ALLOCNO_ASSIGNED_P (subloop_allocno) = TRUE;
		      if (hard_regno >= 0)
			update_copy_costs (subloop_allocno, TRUE);
		      /* We don't need updated costs anymore: */
		      free_allocno_updated_costs (subloop_allocno);
		    }
		}
	      else if (flag_ira_propagate_cost && hard_regno >= 0)
		{
		  exit_freq = loop_edge_freq (subloop_node, -1, TRUE);
		  enter_freq = loop_edge_freq (subloop_node, -1, FALSE);
		  cost = (register_move_cost[mode][class][class] 
			  * (exit_freq + enter_freq));
		  cover_class = ALLOCNO_COVER_CLASS (subloop_allocno);
		  allocate_and_set_costs
		    (&ALLOCNO_HARD_REG_COSTS (subloop_allocno), cover_class,
		     ALLOCNO_COVER_CLASS_COST (subloop_allocno));
		  allocate_and_set_costs
		    (&ALLOCNO_CONFLICT_HARD_REG_COSTS (subloop_allocno),
		     cover_class, 0);
		  ALLOCNO_HARD_REG_COSTS (subloop_allocno)[index] -= cost;
		  ALLOCNO_CONFLICT_HARD_REG_COSTS (subloop_allocno)[index]
		    -= cost;
		  ALLOCNO_UPDATED_MEMORY_COST (subloop_allocno)
		    += (memory_move_cost[mode][class][0] * enter_freq
			+ memory_move_cost[mode][class][1] * exit_freq);
		  if (ALLOCNO_COVER_CLASS_COST (subloop_allocno)
		      > ALLOCNO_HARD_REG_COSTS (subloop_allocno)[index])
		    ALLOCNO_COVER_CLASS_COST (subloop_allocno)
		      = ALLOCNO_HARD_REG_COSTS (subloop_allocno)[index];
		}
	    }
	}
}

/* The function initialized common data for coloring and calls
   functions to do Chaitin-Briggs and regional coloring.  */
static void
do_coloring (void)
{
  coloring_allocno_bitmap = ira_allocate_bitmap ();

  if (internal_flag_ira_verbose > 0 && ira_dump_file != NULL)
    fprintf (ira_dump_file, "\n**** Allocnos coloring:\n\n");
  
  traverse_loop_tree (FALSE, ira_loop_tree_root, color_pass, NULL);

  if (internal_flag_ira_verbose > 1 && ira_dump_file != NULL)
    print_disposition (ira_dump_file);

  ira_free_bitmap (coloring_allocno_bitmap);
}



/* The functions moves spill/restore code, which are to be generated
   in ira-emit.c, to less frequent points (if it is profitable) by
   reassigning some allocnos (in loop with subloops containing in
   another loop) to memory which results in longer live-range where
   the corresponding pseudo-registers will be in memory.  */
static void
move_spill_restore (void)
{
  int cost, changed_p, regno, hard_regno, hard_regno2, index;
  int enter_freq, exit_freq;
  enum machine_mode mode;
  enum reg_class class;
  allocno_t a, father_allocno, subloop_allocno;
  loop_tree_node_t father, loop_node, subloop_node;
  allocno_iterator ai;

  for (;;)
    {
      changed_p = FALSE;
      if (internal_flag_ira_verbose > 0 && ira_dump_file != NULL)
	fprintf (ira_dump_file, "New iteration of spill/restore move\n");
      FOR_EACH_ALLOCNO (a, ai)
	{
	  regno = ALLOCNO_REGNO (a);
	  loop_node = ALLOCNO_LOOP_TREE_NODE (a);
	  if (ALLOCNO_CAP_MEMBER (a) != NULL
	      || (hard_regno = ALLOCNO_HARD_REGNO (a)) < 0
	      || loop_node->children == NULL
	      /* don't do the optimization because it can create
		 copies and the reload pass can spill the allocno set
		 by copy although the allocno will not get memory
		 slot.  */
	      || reg_equiv_invariant_p[regno]
	      || reg_equiv_const[regno] != NULL_RTX)
	    continue;
	  mode = ALLOCNO_MODE (a);
	  class = ALLOCNO_COVER_CLASS (a);
	  index = class_hard_reg_index[class][hard_regno];
	  ira_assert (index >= 0);
	  cost = (ALLOCNO_MEMORY_COST (a)
		  - (ALLOCNO_HARD_REG_COSTS (a) == NULL
		     ? ALLOCNO_COVER_CLASS_COST (a)
		     : ALLOCNO_HARD_REG_COSTS (a)[index]));
	  for (subloop_node = loop_node->children;
	       subloop_node != NULL;
	       subloop_node = subloop_node->next)
	    {
	      if (subloop_node->bb != NULL)
		continue;
	      subloop_allocno = subloop_node->regno_allocno_map[regno];
	      if (subloop_allocno == NULL)
		continue;
	      /* We have accumulated cost.  To get the real cost of
		 allocno usage in the loop we should subtract costs of
		 the subloop allocnos.  */
	      cost -= (ALLOCNO_MEMORY_COST (subloop_allocno)
		       - (ALLOCNO_HARD_REG_COSTS (subloop_allocno) == NULL
			  ? ALLOCNO_COVER_CLASS_COST (subloop_allocno)
			  : ALLOCNO_HARD_REG_COSTS (subloop_allocno)[index]));
	      exit_freq = loop_edge_freq (subloop_node, regno, TRUE);
	      enter_freq = loop_edge_freq (subloop_node, regno, FALSE);
	      if ((hard_regno2 = ALLOCNO_HARD_REGNO (subloop_allocno)) < 0)
		cost -= (memory_move_cost[mode][class][0] * exit_freq
			 + memory_move_cost[mode][class][1] * enter_freq);
	      else
		{
		  cost += (memory_move_cost[mode][class][0] * exit_freq
			   + memory_move_cost[mode][class][1] * enter_freq);
		  if (hard_regno2 != hard_regno)
		    cost -= (register_move_cost[mode][class][class]
			     * (exit_freq + enter_freq));
		}
	    }
	  if ((father = loop_node->father) != NULL
	      && (father_allocno = father->regno_allocno_map[regno]) != NULL)
	    {
	      exit_freq	= loop_edge_freq (loop_node, regno, TRUE);
	      enter_freq = loop_edge_freq (loop_node, regno, FALSE);
	      if ((hard_regno2 = ALLOCNO_HARD_REGNO (father_allocno)) < 0)
		cost -= (memory_move_cost[mode][class][0] * exit_freq
			 + memory_move_cost[mode][class][1] * enter_freq);
	      else
		{
		  cost += (memory_move_cost[mode][class][1] * exit_freq
			   + memory_move_cost[mode][class][0] * enter_freq);
		  if (hard_regno2 != hard_regno)
		    cost -= (register_move_cost[mode][class][class]
			     * (exit_freq + enter_freq));
		}
	    }
	  if (cost < 0)
	    {
	      ALLOCNO_HARD_REGNO (a) = -1;
	      if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
		{
		  fprintf
		    (ira_dump_file,
		     "      Moving spill/restore for a%dr%d up from loop %d",
		     ALLOCNO_NUM (a), regno, loop_node->loop->num);
		  fprintf (ira_dump_file, " - profit %d\n", -cost);
		}
	      changed_p = TRUE;
	    }
	}
      if (! changed_p)
	break;
    }
}



/* Update current hard reg costs and current conflict hard reg costs
   for allocno A.  It is done by processing its copies containing
   other allocnos already assigned.  */
static void
update_curr_costs (allocno_t a)
{
  int i, hard_regno, cost;
  enum machine_mode mode;
  enum reg_class cover_class, class;
  allocno_t another_a;
  copy_t cp, next_cp;

  ira_assert (! ALLOCNO_ASSIGNED_P (a));
  cover_class = ALLOCNO_COVER_CLASS (a);
  if (cover_class == NO_REGS)
    return;
  mode = ALLOCNO_MODE (a);
  for (cp = ALLOCNO_COPIES (a); cp != NULL; cp = next_cp)
    {
      if (cp->first == a)
	{
	  next_cp = cp->next_first_allocno_copy;
	  another_a = cp->second;
	}
      else if (cp->second == a)
	{
	  next_cp = cp->next_second_allocno_copy;
	  another_a = cp->first;
	}
      else
	gcc_unreachable ();
      if (cover_class != ALLOCNO_COVER_CLASS (another_a)
	  || ! ALLOCNO_ASSIGNED_P (another_a)
	  || (hard_regno = ALLOCNO_HARD_REGNO (another_a)) < 0)
	continue;
      class = REGNO_REG_CLASS (hard_regno);
      i = class_hard_reg_index[cover_class][hard_regno];
      ira_assert (i >= 0);
      cost = (cp->first == a
	      ? register_move_cost[mode][class][cover_class]
	      : register_move_cost[mode][cover_class][class]);
      allocate_and_set_or_copy_costs
	(&ALLOCNO_UPDATED_HARD_REG_COSTS (a),
	 cover_class, ALLOCNO_COVER_CLASS_COST (a),
	 ALLOCNO_HARD_REG_COSTS (a));
      allocate_and_set_or_copy_costs
	(&ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a),
	 cover_class, 0, ALLOCNO_CONFLICT_HARD_REG_COSTS (a));
      ALLOCNO_UPDATED_HARD_REG_COSTS (a)[i] -= cp->freq * cost;
      ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a)[i] -= cp->freq * cost;
    }
}

/* Map: allocno number -> allocno priority.  */
static int *allocno_priorities;

/* Allocate array ALLOCNO_PRIORITIES and set up priorities for N allocnos in
   array CONSIDERATION_ALLOCNOS.  */
static void
start_allocno_priorities (allocno_t *consideration_allocnos, int n)
{
  int i, length;
  allocno_t a;
  allocno_live_range_t r;

  for (i = 0; i < n; i++)
    {
      a = consideration_allocnos[i];
      for (length = 0, r = ALLOCNO_LIVE_RANGES (a); r != NULL; r = r->next)
	length += r->finish - r->start + 1;
      if (length == 0)
	{
	  allocno_priorities[ALLOCNO_NUM (a)] = 0;
	  continue;
	}
      ira_assert (length > 0 && ALLOCNO_NREFS (a) >= 0);
      allocno_priorities[ALLOCNO_NUM (a)]
	= (((double) (floor_log2 (ALLOCNO_NREFS (a)) * ALLOCNO_FREQ (a))
	    / length)
	   * (10000 / REG_FREQ_MAX) * PSEUDO_REGNO_SIZE (ALLOCNO_REGNO (a)));
    }
}

/* The function is used to sort allocnos according to their priorities
   which are calculated analogous to ones in file `global.c'.  */
static int
allocno_priority_compare_func (const void *v1p, const void *v2p)
{
  allocno_t a1 = *(const allocno_t *) v1p, a2 = *(const allocno_t *) v2p;
  int pri1, pri2;

  pri1 = allocno_priorities[ALLOCNO_NUM (a1)];
  pri2 = allocno_priorities[ALLOCNO_NUM (a2)];
  if (pri2 - pri1)
    return pri2 - pri1;

  /* If regs are equally good, sort by allocnos, so that the results of
     qsort leave nothing to chance.  */
  return ALLOCNO_NUM (a1) - ALLOCNO_NUM (a2);
}

/* Try to assign hard registers to the unassigned allocnos and
   allocnos conflicting with them or conflicting with allocnos whose
   regno >= START_REGNO.  The function is called after spill/restore
   placement optimization and ira_flattening, so more allocnos
   (including ones created in ira-emit.c) will have a chance to get a
   hard register.  We use simple assignment algorithm based on
   priorities.  */
void
reassign_conflict_allocnos (int start_regno)
{
  int i, allocnos_to_color_num;
  allocno_t a, conflict_a;
  allocno_conflict_iterator aci;
  enum reg_class cover_class;
  bitmap allocnos_to_color;
  allocno_iterator ai;

  allocnos_to_color = ira_allocate_bitmap ();
  allocnos_to_color_num = 0;
  FOR_EACH_ALLOCNO (a, ai)
    {
      if (! ALLOCNO_ASSIGNED_P (a)
	  && ! bitmap_bit_p (allocnos_to_color, ALLOCNO_NUM (a)))
	{
	  if (ALLOCNO_COVER_CLASS (a) != NO_REGS)
	    sorted_allocnos[allocnos_to_color_num++] = a;
	  else
	    {
	      ALLOCNO_ASSIGNED_P (a) = TRUE;
	      ALLOCNO_HARD_REGNO (a) = -1;
	      ira_assert (ALLOCNO_UPDATED_HARD_REG_COSTS (a) == NULL);
	      ira_assert (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) == NULL);
	    }
	  bitmap_set_bit (allocnos_to_color, ALLOCNO_NUM (a));
	}
      if (ALLOCNO_REGNO (a) < start_regno
	  || (cover_class = ALLOCNO_COVER_CLASS (a)) == NO_REGS)
	continue;
      FOR_EACH_ALLOCNO_CONFLICT (a, conflict_a, aci)
	{
	  ira_assert (cover_class == ALLOCNO_COVER_CLASS (conflict_a));
	  if (bitmap_bit_p (allocnos_to_color, ALLOCNO_NUM (conflict_a)))
	    continue;
	  bitmap_set_bit (allocnos_to_color, ALLOCNO_NUM (conflict_a));
	  sorted_allocnos[allocnos_to_color_num++] = conflict_a;
	}
    }
  ira_free_bitmap (allocnos_to_color);
  if (allocnos_to_color_num > 1)
    {
      start_allocno_priorities (sorted_allocnos, allocnos_to_color_num);
      qsort (sorted_allocnos, allocnos_to_color_num, sizeof (allocno_t),
	     allocno_priority_compare_func);
    }
  for (i = 0; i < allocnos_to_color_num; i++)
    {
      a = sorted_allocnos[i];
      ALLOCNO_ASSIGNED_P (a) = FALSE;
      ira_assert (ALLOCNO_UPDATED_HARD_REG_COSTS (a) == NULL);
      ira_assert (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) == NULL);
      update_curr_costs (a);
    }
  for (i = 0; i < allocnos_to_color_num; i++)
    {
      a = sorted_allocnos[i];
      if (assign_hard_reg (a, TRUE))
	{
	  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	    fprintf
	      (ira_dump_file,
	       "      Secondary allocation: assign hard reg %d to reg %d\n",
	       ALLOCNO_HARD_REGNO (a), ALLOCNO_REGNO (a));
	}
    }
}



/* This page contains code to coalesce memory stack slots used by
   spilled allocnos.  This results in smaller stack frame, better data
   locality, and in smaller code for some architectures like
   x86/x86_64 where insn size depends on address displacement value.
   On the other hand, it can worsen insn scheduling after the RA but
   in practice it is less important than smaller stack frames.  */

/* Usage cost and order number of coalesced allocno set to which
   given pseudo register belongs to.  */
static int *regno_coalesced_allocno_cost;
static int *regno_coalesced_allocno_num;

/* The function is used to sort pseudos according frequencies of
   coalesced allocno sets they belong to (putting most frequently ones
   first), and according to coalesced allocno set order numbers.  */
static int
coalesced_pseudo_reg_freq_compare (const void *v1p, const void *v2p)
{
  int regno1 = *(int *) v1p;
  int regno2 = *(int *) v2p;
  int diff;

  if ((diff = (regno_coalesced_allocno_cost[regno2]
	       - regno_coalesced_allocno_cost[regno1])) != 0)
    return diff;
  if ((diff = (regno_coalesced_allocno_num[regno1]
	       - regno_coalesced_allocno_num[regno2])) != 0)
    return diff;
  return regno1 - regno2;
}

/* Widest width in which each pseudo reg is referred to (via subreg).
   It is used for sorting pseudo registers.  */
static unsigned int *regno_max_ref_width;

/* The function is used to sort pseudos according their slot numbers
  (putting ones with smaller numbers first, or last when the frame
  pointer is not needed).  */
static int
coalesced_pseudo_reg_slot_compare (const void *v1p, const void *v2p)
{
  int regno1 = *(int *) v1p;
  int regno2 = *(int *) v2p;
  allocno_t a1 = regno_allocno_map[regno1];
  allocno_t a2 = regno_allocno_map[regno2];
  int diff, slot_num1, slot_num2;
  int total_size1, total_size2;

  if (a1 == NULL || ALLOCNO_HARD_REGNO (a1) >= 0)
    {
      if (a2 == NULL || ALLOCNO_HARD_REGNO (a2) >= 0)
	return (int *) v1p - (int *) v2p; /* Save the order. */
      return 1;
    }
  else if (a2 == NULL || ALLOCNO_HARD_REGNO (a2) >= 0)
    return -1;
  slot_num1 = -ALLOCNO_HARD_REGNO (a1);
  slot_num2 = -ALLOCNO_HARD_REGNO (a2);
  if ((diff = slot_num1 - slot_num2) != 0)
    return frame_pointer_needed ? diff : -diff;
  total_size1 = MAX (PSEUDO_REGNO_BYTES (regno1), regno_max_ref_width[regno1]);
  total_size2 = MAX (PSEUDO_REGNO_BYTES (regno2), regno_max_ref_width[regno2]);
  if ((diff = total_size2 - total_size1) != 0)
    return diff;
  return (int *) v1p - (int *) v2p; /* Save the order. */
}

/* Setup REGNO_COALESCED_ALLOCNO_COST and REGNO_COALESCED_ALLOCNO_NUM
   for coalesced allocno sets containing allocnos with their regnos
   given in array PSEUDO_REGNOS of length N.  */
static void
setup_coalesced_allocno_costs_and_nums (int *pseudo_regnos, int n)
{
  int i, num, regno, cost;
  allocno_t allocno, a;

  for (num = i = 0; i < n; i++)
    {
      regno = pseudo_regnos[i];
      allocno = regno_allocno_map[regno];
      if (allocno == NULL)
	{
	  regno_coalesced_allocno_cost[regno] = 0;
	  regno_coalesced_allocno_num[regno] = ++num;
	  continue;
	}
      if (ALLOCNO_FIRST_COALESCED_ALLOCNO (allocno) != allocno)
	continue;
      num++;
      for (cost = 0, a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
	   a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
	{
	  cost += ALLOCNO_FREQ (a);
	  if (a == allocno)
	    break;
	}
      for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
	   a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
	{
	  regno_coalesced_allocno_num[ALLOCNO_REGNO (a)] = num;
	  regno_coalesced_allocno_cost[ALLOCNO_REGNO (a)] = cost;
	  if (a == allocno)
	    break;
	}
    }
}

/* The function collects spilled allocnos representing coalesced
   allocno sets (the first coalesced allocno).  The collected allocnos
   are returned through array SPILLED_COALESCED_ALLOCNOS.  The
   function returns the number of the collected allocnos.  The
   allocnos are given by their regnos in array PSEUDO_REGNOS of length
   N.  */
static int
collect_spilled_coalesced_allocnos (int *pseudo_regnos, int n,
				    allocno_t *spilled_coalesced_allocnos)
{
  int i, num, regno;
  allocno_t allocno;

  for (num = i = 0; i < n; i++)
    {
      regno = pseudo_regnos[i];
      allocno = regno_allocno_map[regno];
      if (allocno == NULL || ALLOCNO_HARD_REGNO (allocno) >= 0
	  || ALLOCNO_FIRST_COALESCED_ALLOCNO (allocno) != allocno)
	continue;
      spilled_coalesced_allocnos[num++] = allocno;
    }
  return num;
}

/* We have coalesced allocnos involving in copies.  This function
   coalesces allocnos further in order to share the same memory stack
   slot.  Allocnos representing sets of allocnos coalesced before the
   call are given in array SPILLED_COALESCED_ALLOCNOS of length NUM.
   Return TRUE if some allocnos were coalesced in the function.  */
static int
coalesce_spill_slots (allocno_t *spilled_coalesced_allocnos, int num)
{
  int i, j;
  allocno_t allocno, a;
  int merged_p = FALSE;

  /* Coalesce non-conflicting spilled allocnos preferring most
     frequently used.  */
  for (i = 0; i < num; i++)
    {
      allocno = spilled_coalesced_allocnos[i];
      if (ALLOCNO_FIRST_COALESCED_ALLOCNO (allocno) != allocno
	  || (ALLOCNO_REGNO (allocno) < reg_equiv_len
	      && (reg_equiv_invariant_p[ALLOCNO_REGNO (allocno)]
		  || reg_equiv_const[ALLOCNO_REGNO (allocno)] != NULL_RTX)))
	continue;
      for (j = 0; j < i; j++)
	{
	  a = spilled_coalesced_allocnos[j];
	  if (ALLOCNO_FIRST_COALESCED_ALLOCNO (a) != a
	      || (ALLOCNO_REGNO (a) < reg_equiv_len
		  && (reg_equiv_invariant_p[ALLOCNO_REGNO (a)]
		      || reg_equiv_const[ALLOCNO_REGNO (a)] != NULL_RTX))
	      || coalesced_allocno_conflict_p (allocno, a, TRUE))
	    continue;
	  allocno_coalesced_p = TRUE;
	  merged_p = TRUE;
	  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	    fprintf (ira_dump_file,
		     "      Coalescing spilled allocnos a%dr%d->a%dr%d\n",
		     ALLOCNO_NUM (allocno), ALLOCNO_REGNO (allocno),
		     ALLOCNO_NUM (a), ALLOCNO_REGNO (a));
	  merge_allocnos (a, allocno);
	  ira_assert (ALLOCNO_FIRST_COALESCED_ALLOCNO (a) == a);
	}
    }
  return merged_p;
}

/* The function sorts pseudo-register numbers in array PSEUDO_REGNOS
   of length N for subsequent assigning stack slots to them in the
   reload pass.  To do this we coalesce spilled allocnos first to
   decrease the number of memory-memory move insns.  This function is
   called by the reload.  */
void
sort_regnos_for_alter_reg (int *pseudo_regnos, int n,
			   unsigned int *reg_max_ref_width)
{
  int max_regno = max_reg_num ();
  int i, regno, num, slot_num;
  allocno_t allocno, a;
  allocno_iterator ai;
  allocno_t *spilled_coalesced_allocnos;

  processed_coalesced_allocno_bitmap = ira_allocate_bitmap ();
  /* Set up allocnos can be coalesced.  */
  coloring_allocno_bitmap = ira_allocate_bitmap ();
  for (i = 0; i < n; i++)
    {
      regno = pseudo_regnos[i];
      allocno = regno_allocno_map[regno];
      if (allocno != NULL)
	bitmap_set_bit (coloring_allocno_bitmap,
			ALLOCNO_NUM (allocno));
    }
  allocno_coalesced_p = FALSE;
  coalesce_allocnos (TRUE);
  ira_free_bitmap (coloring_allocno_bitmap);
  regno_coalesced_allocno_cost = ira_allocate (max_regno * sizeof (int));
  regno_coalesced_allocno_num = ira_allocate (max_regno * sizeof (int));
  memset (regno_coalesced_allocno_num, 0, max_regno * sizeof (int));
  setup_coalesced_allocno_costs_and_nums (pseudo_regnos, n);
  /* Sort regnos according frequencies of the corresponding coalesced
     allocno sets.  */
  qsort (pseudo_regnos, n, sizeof (int), coalesced_pseudo_reg_freq_compare);
  spilled_coalesced_allocnos
    = ira_allocate (allocnos_num * sizeof (allocno_t));
  /* Collect allocnos representing the spilled coalesced allocno
     sets.  */
  num = collect_spilled_coalesced_allocnos (pseudo_regnos, n,
					    spilled_coalesced_allocnos);
  if (flag_ira_share_spill_slots
      && coalesce_spill_slots (spilled_coalesced_allocnos, num))
    {
      setup_coalesced_allocno_costs_and_nums (pseudo_regnos, n);
      qsort (pseudo_regnos, n, sizeof (int), coalesced_pseudo_reg_freq_compare);
      num = collect_spilled_coalesced_allocnos (pseudo_regnos, n,
						spilled_coalesced_allocnos);
    }
  ira_free_bitmap (processed_coalesced_allocno_bitmap);
  allocno_coalesced_p = FALSE;
  /* Assign stack slot numbers to spilled allocno sets, use smaller
     numbers for most frequently used coalesced allocnos.  -1 is
     reserved for dynamic search of stack slots for pseudos spilled by
     the reload.  */
  slot_num = 1;
  for (i = 0; i < num; i++)
    {
      allocno = spilled_coalesced_allocnos[i];
      if (ALLOCNO_FIRST_COALESCED_ALLOCNO (allocno) != allocno
	  || ALLOCNO_HARD_REGNO (allocno) >= 0
	  || (ALLOCNO_REGNO (allocno) < reg_equiv_len
	      && (reg_equiv_invariant_p[ALLOCNO_REGNO (allocno)]
		  || reg_equiv_const[ALLOCNO_REGNO (allocno)] != NULL_RTX)))
	continue;
      if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	fprintf (ira_dump_file, "      Slot %d (freq,size):", slot_num);
      slot_num++;
      for (a = ALLOCNO_NEXT_COALESCED_ALLOCNO (allocno);;
	   a = ALLOCNO_NEXT_COALESCED_ALLOCNO (a))
	{
	  ira_assert (ALLOCNO_HARD_REGNO (a) < 0);
	  ALLOCNO_HARD_REGNO (a) = -slot_num;
	  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	    fprintf (ira_dump_file, " a%dr%d(%d,%d)",
		     ALLOCNO_NUM (a), ALLOCNO_REGNO (a), ALLOCNO_FREQ (a),
		     MAX (PSEUDO_REGNO_BYTES (ALLOCNO_REGNO (a)),
			  reg_max_ref_width[ALLOCNO_REGNO (a)]));
	      
	  if (a == allocno)
	    break;
	}
      if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	fprintf (ira_dump_file, "\n");
    }
  spilled_reg_stack_slots_num = slot_num - 1;
  ira_free (spilled_coalesced_allocnos);
  /* Sort regnos according the slot numbers.  */
  regno_max_ref_width = reg_max_ref_width;
  qsort (pseudo_regnos, n, sizeof (int), coalesced_pseudo_reg_slot_compare);
  /* Uncoalesce allocnos which is necessary for (re)assigning during
     the reload pass.  */
  FOR_EACH_ALLOCNO (a, ai)
    {
      ALLOCNO_FIRST_COALESCED_ALLOCNO (a) = a;
      ALLOCNO_NEXT_COALESCED_ALLOCNO (a) = a;
    }
  ira_free (regno_coalesced_allocno_num);
  ira_free (regno_coalesced_allocno_cost);
}



/* This page contains code used by the reload pass to improve the
   final code.  */

/* The function is called from the reload pass to mark changes in the
   allocation of REGNO made by the reload.  Remember that reg_renumber
   reflects the change result.  */
void
mark_allocation_change (int regno)
{
  allocno_t a = regno_allocno_map[regno];
  int old_hard_regno, hard_regno, cost;
  enum reg_class cover_class = ALLOCNO_COVER_CLASS (a);

  ira_assert (a != NULL);
  hard_regno = reg_renumber[regno];
  if ((old_hard_regno = ALLOCNO_HARD_REGNO (a)) == hard_regno)
    return;
  if (old_hard_regno < 0)
    cost = -ALLOCNO_MEMORY_COST (a);
  else
    {
      ira_assert (class_hard_reg_index[cover_class][old_hard_regno] >= 0);
      cost = -(ALLOCNO_HARD_REG_COSTS (a) == NULL
	       ? ALLOCNO_COVER_CLASS_COST (a)
	       : ALLOCNO_HARD_REG_COSTS (a)
	         [class_hard_reg_index[cover_class][old_hard_regno]]);
      update_copy_costs (a, FALSE);
    }
  overall_cost -= cost;
  ALLOCNO_HARD_REGNO (a) = hard_regno;
  if (hard_regno < 0)
    {
      ALLOCNO_HARD_REGNO (a) = -1;
      cost += ALLOCNO_MEMORY_COST (a);
    }
  else if (class_hard_reg_index[cover_class][hard_regno] >= 0)
    {
      cost += (ALLOCNO_HARD_REG_COSTS (a) == NULL
	       ? ALLOCNO_COVER_CLASS_COST (a)
	       : ALLOCNO_HARD_REG_COSTS (a)
	         [class_hard_reg_index[cover_class][hard_regno]]);
      update_copy_costs (a, TRUE);
    }
  else
    /* Reload changed class of the allocno.  */
    cost = 0;
  overall_cost += cost;
}

/* This function is called when the reload deletes memory-memory move.
   In this case we marks that the allocation of the corresponding
   allocnos should be not changed in future.  Otherwise we risk to get
   a wrong code.  */
void
mark_memory_move_deletion (int dst_regno, int src_regno)
{
  allocno_t dst = regno_allocno_map[dst_regno];
  allocno_t src = regno_allocno_map[src_regno];

  ira_assert (dst != NULL && src != NULL
	      && ALLOCNO_HARD_REGNO (dst) < 0
	      && ALLOCNO_HARD_REGNO (src) < 0);
  ALLOCNO_DONT_REASSIGN_P (dst) = TRUE;
  ALLOCNO_DONT_REASSIGN_P (src) = TRUE;
}

/* The function tries to assign a hard register (except for
   FORBIDDEN_REGS) to allocno A and return TRUE in the case of
   success.  That is an analog of retry_global_alloc for IRA.  */
static int
allocno_reload_assign (allocno_t a, HARD_REG_SET forbidden_regs)
{
  int hard_regno;
  enum reg_class cover_class;
  int regno = ALLOCNO_REGNO (a);

  IOR_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a), forbidden_regs);
  if (! flag_caller_saves && ALLOCNO_CALLS_CROSSED_NUM (a) != 0)
    IOR_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a), call_used_reg_set);
  ALLOCNO_ASSIGNED_P (a) = FALSE;
  ira_assert (ALLOCNO_UPDATED_HARD_REG_COSTS (a) == NULL);
  ira_assert (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) == NULL);
  cover_class = ALLOCNO_COVER_CLASS (a);
  update_curr_costs (a);
  assign_hard_reg (a, TRUE);
  hard_regno = ALLOCNO_HARD_REGNO (a);
  reg_renumber[regno] = hard_regno;
  if (hard_regno < 0)
    ALLOCNO_HARD_REGNO (a) = -1;
  else
    {
      ira_assert (class_hard_reg_index[cover_class][hard_regno] >= 0);
      overall_cost -= (ALLOCNO_MEMORY_COST (a)
		       - (ALLOCNO_HARD_REG_COSTS (a) == NULL
			  ? ALLOCNO_COVER_CLASS_COST (a)
			  : ALLOCNO_HARD_REG_COSTS (a)
			    [class_hard_reg_index[cover_class][hard_regno]]));
      if (ALLOCNO_CALLS_CROSSED_NUM (a) != 0
	  && ! hard_reg_not_in_set_p (hard_regno, ALLOCNO_MODE (a),
				      call_used_reg_set))
	{
	  ira_assert (flag_caller_saves);
	  caller_save_needed = 1;
	}
    }

  /* If we found a hard register, modify the RTL for the pseudo
     register to show the hard register, and mark the pseudo register
     live.  */
  if (reg_renumber[regno] >= 0)
    {
      if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	fprintf (ira_dump_file, ": reassign to %d\n", reg_renumber[regno]);
      SET_REGNO (regno_reg_rtx[regno], reg_renumber[regno]);
      mark_home_live (regno);
    }
  else if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
    fprintf (ira_dump_file, "\n");

  return reg_renumber[regno] >= 0;
}

/* The function is used to sort pseudos according their usage
   frequencies (putting most frequently ones first).  */
static int
pseudo_reg_compare (const void *v1p, const void *v2p)
{
  int regno1 = *(int *) v1p;
  int regno2 = *(int *) v2p;
  int diff;

  if ((diff = REG_FREQ (regno2) - REG_FREQ (regno1)) != 0)
    return diff;
  return regno1 - regno2;
}

/* The function tries to allocate hard registers to
   SPILLED_PSEUDO_REGS (there are NUM of them) or spilled pseudos
   conflicting with pseudos in SPILLED_PSEUDO_REGS.  It returns TRUE
   and update SPILLED, if the allocation has been changed.  The
   function doesn't use BAD_SPILL_REGS and hard registers in
   PSEUDO_FORBIDDEN_REGS and PSEUDO_PREVIOUS_REGS for the
   corresponding pseudos.  The function is called by the reload pass
   at the end of each reload iteration.  */
int
reassign_pseudos (int *spilled_pseudo_regs, int num,
		  HARD_REG_SET bad_spill_regs,
		  HARD_REG_SET *pseudo_forbidden_regs,
		  HARD_REG_SET *pseudo_previous_regs,  bitmap spilled)
{
  int i, m, n, regno, changed_p;
  allocno_t a, conflict_a;
  HARD_REG_SET forbidden_regs;
  allocno_conflict_iterator aci;

  if (num > 1)
    qsort (spilled_pseudo_regs, num, sizeof (int), pseudo_reg_compare);
  changed_p = FALSE;
  /* Try to assign hard registers to pseudos from
     SPILLED_PSEUDO_REGS.  */
  for (m = i = 0; i < num; i++)
    {
      regno = spilled_pseudo_regs[i];
      COPY_HARD_REG_SET (forbidden_regs, bad_spill_regs);
      IOR_HARD_REG_SET (forbidden_regs, pseudo_forbidden_regs[regno]);
      IOR_HARD_REG_SET (forbidden_regs, pseudo_previous_regs[regno]);
      gcc_assert (reg_renumber[regno] < 0);
      a = regno_allocno_map[regno];
      mark_allocation_change (regno);
      ira_assert (reg_renumber[regno] < 0);
      if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	fprintf (ira_dump_file,
		 "      Spill %d(a%d), cost=%d", regno, ALLOCNO_NUM (a),
		 ALLOCNO_MEMORY_COST (a)
		 - ALLOCNO_COVER_CLASS_COST (a));
      allocno_reload_assign (a, forbidden_regs);
      if (reg_renumber[regno] >= 0)
	{
	  CLEAR_REGNO_REG_SET (spilled, regno);
	  changed_p = TRUE;
	}
      else
	spilled_pseudo_regs[m++] = regno;
    }
  if (m == 0)
    return changed_p;
  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
    {
      fprintf (ira_dump_file, "      Spilled regs");
      for (i = 0; i < m; i++)
	fprintf (ira_dump_file, " %d", spilled_pseudo_regs[i]);
      fprintf (ira_dump_file, "\n");
    }
  /* Try to assign hard registers to pseudos conflicting with ones
     from SPILLED_PSEUDO_REGS.  */
  for (i = n = 0; i < m; i++)
    {
      regno = spilled_pseudo_regs[i];
      a = regno_allocno_map[regno];
      FOR_EACH_ALLOCNO_CONFLICT (a, conflict_a, aci)
	if (ALLOCNO_HARD_REGNO (conflict_a) < 0
	    && ! ALLOCNO_DONT_REASSIGN_P (conflict_a)
	    && ! bitmap_bit_p (consideration_allocno_bitmap,
			       ALLOCNO_NUM (conflict_a)))
	  {
	    sorted_allocnos[n++] = conflict_a;
	    bitmap_set_bit (consideration_allocno_bitmap,
			    ALLOCNO_NUM (conflict_a));
	  }
    }
  if (n != 0)
    {
      start_allocno_priorities (sorted_allocnos, n);
      qsort (sorted_allocnos, n, sizeof (allocno_t),
	     allocno_priority_compare_func);
      for (i = 0; i < n; i++)
	{
	  a = sorted_allocnos[i];
	  regno = ALLOCNO_REGNO (a);
	  COPY_HARD_REG_SET (forbidden_regs, bad_spill_regs);
	  IOR_HARD_REG_SET (forbidden_regs, pseudo_forbidden_regs[regno]);
	  IOR_HARD_REG_SET (forbidden_regs, pseudo_previous_regs[regno]);
	  if (internal_flag_ira_verbose > 3 && ira_dump_file != NULL)
	    fprintf (ira_dump_file,
		     "        Try assign %d(a%d), cost=%d",
		     regno, ALLOCNO_NUM (a),
		     ALLOCNO_MEMORY_COST (a)
		     - ALLOCNO_COVER_CLASS_COST (a));
	  if (allocno_reload_assign (a, forbidden_regs))
	    {
	      changed_p = TRUE;
	      bitmap_clear_bit (spilled, regno);
	    }
	}
    }
  return changed_p;
}

/* The function is called by the reload pass and returns already
   allocated stack slot (if any) for REGNO with given INHERENT_SIZE
   and TOTAL_SIZE.  In the case of failure to find a slot which can be
   used for REGNO, the function returns NULL.  */
rtx
reuse_stack_slot (int regno, unsigned int inherent_size,
		  unsigned int total_size)
{
  unsigned int i;
  int slot_num, best_slot_num;
  int cost, best_cost;
  copy_t cp, next_cp;
  allocno_t another_allocno, allocno = regno_allocno_map[regno];
  rtx x;
  bitmap_iterator bi;
  struct spilled_reg_stack_slot *slot = NULL;

  ira_assert (flag_ira && inherent_size == PSEUDO_REGNO_BYTES (regno)
	      && inherent_size <= total_size
	      && ALLOCNO_HARD_REGNO (allocno) < 0);
  if (! flag_ira_share_spill_slots)
    return NULL_RTX;
  slot_num = -ALLOCNO_HARD_REGNO (allocno) - 2;
  if (slot_num != -1)
    {
      slot = &spilled_reg_stack_slots[slot_num];
      x = slot->mem;
    }
  else
    {
      best_cost = best_slot_num = -1;
      x = NULL_RTX;
      /* It means that the pseudo was spilled in the reload pass, try
	 to reuse a slot.  */
      for (slot_num = 0; slot_num < spilled_reg_stack_slots_num; slot_num++)
	{
	  slot = &spilled_reg_stack_slots[slot_num];
	  if (slot->mem == NULL_RTX)
	    continue;
	  if (slot->width < total_size
	      || GET_MODE_SIZE (GET_MODE (slot->mem)) < inherent_size)
	    continue;
	  
	  EXECUTE_IF_SET_IN_BITMAP (&slot->spilled_regs,
				    FIRST_PSEUDO_REGISTER, i, bi)
	    {
	      another_allocno = regno_allocno_map[i];
	      if (allocno_live_ranges_intersect_p (allocno, another_allocno))
		goto cont;
	    }
	  for (cost = 0, cp = ALLOCNO_COPIES (allocno);
	       cp != NULL;
	       cp = next_cp)
	    {
	      if (cp->first == allocno)
		{
		  next_cp = cp->next_first_allocno_copy;
		  another_allocno = cp->second;
		}
	      else if (cp->second == allocno)
		{
		  next_cp = cp->next_second_allocno_copy;
		  another_allocno = cp->first;
		}
	      else
		gcc_unreachable ();
	      if (cp->insn == NULL_RTX)
		continue;
	      if (bitmap_bit_p (&slot->spilled_regs,
				ALLOCNO_REGNO (another_allocno)))
		cost += cp->freq;
	    }
	  if (cost > best_cost)
	    {
	      best_cost = cost;
	      best_slot_num = slot_num;
	    }
	cont:
	  ;
	}
      if (best_cost >= 0)
	{
	  slot = &spilled_reg_stack_slots[best_slot_num];
	  SET_REGNO_REG_SET (&slot->spilled_regs, regno);
	  x = slot->mem;
	  ALLOCNO_HARD_REGNO (allocno) = -best_slot_num - 2;
	}
    }
  if (x != NULL_RTX)
    {
      ira_assert (slot->width >= total_size);
      EXECUTE_IF_SET_IN_BITMAP (&slot->spilled_regs,
				FIRST_PSEUDO_REGISTER, i, bi)
	{
	  ira_assert (! pseudo_live_ranges_intersect_p (regno, i));
	}
      SET_REGNO_REG_SET (&slot->spilled_regs, regno);
      if (internal_flag_ira_verbose > 3 && ira_dump_file)
	{
	  fprintf (ira_dump_file, "      Assigning %d(freq=%d) slot %d of",
		   regno, REG_FREQ (regno), slot_num);
	  EXECUTE_IF_SET_IN_BITMAP (&slot->spilled_regs,
				    FIRST_PSEUDO_REGISTER, i, bi)
	    {
	      if ((unsigned) regno != i)
		fprintf (ira_dump_file, " %d", i);
	    }
	  fprintf (ira_dump_file, "\n");
	}
    }
  return x;
}

/* The function is called by the reload pass every time when a new
   stack slot X with TOTAL_SIZE was allocated for REGNO.  We store
   this info for subsequent reuse_stack_slot calls.  */
void
mark_new_stack_slot (rtx x, int regno, unsigned int total_size)
{
  struct spilled_reg_stack_slot *slot;
  int slot_num;
  allocno_t allocno;

  ira_assert (flag_ira && PSEUDO_REGNO_BYTES (regno) <= total_size);
  allocno = regno_allocno_map[regno];
  slot_num = -ALLOCNO_HARD_REGNO (allocno) - 2;
  if (slot_num == -1)
    {
      slot_num = spilled_reg_stack_slots_num++;
      ALLOCNO_HARD_REGNO (allocno) = -slot_num - 2;
    }
  slot = &spilled_reg_stack_slots[slot_num];
  INIT_REG_SET (&slot->spilled_regs);
  SET_REGNO_REG_SET (&slot->spilled_regs, regno);
  slot->mem = x;
  slot->width = total_size;
  if (internal_flag_ira_verbose > 3 && ira_dump_file)
    fprintf (ira_dump_file, "      Assigning %d(freq=%d) a new slot %d\n",
	     regno, REG_FREQ (regno), slot_num);
}


/* Return spill cost for pseudo-registers whose numbers are in array
   REGNOS (with a negative number as an end marker) for reload with
   given IN and OUT for INSN.  Return also number points (through
   EXCESS_PRESSURE_LIVE_LENGTH) where the pseudo-register lives and
   the register pressure is high, number of references of the
   pseudo-registers (through NREFS), number of callee-clobbered
   hard-registers occupied by the pseudo-registers (through
   CALL_USED_COUNT), and the first hard regno occupied by the
   pseudo-registers (through FIRST_HARD_REGNO).  */
static int
calculate_spill_cost (int *regnos, rtx in, rtx out, rtx insn,
		      int *excess_pressure_live_length,
		      int *nrefs, int *call_used_count, int *first_hard_regno)
{
  int i, cost, regno, hard_regno, j, count, saved_cost, nregs, in_p, out_p;
  int length;
  allocno_t a;

  *nrefs = 0;
  for (length = count = cost = i = 0;; i++)
    {
      regno = regnos[i];
      if (regno < 0)
	break;
      *nrefs += REG_N_REFS (regno);
      hard_regno = reg_renumber[regno];
      ira_assert (hard_regno >= 0);
      a = regno_allocno_map[regno];
      length += ALLOCNO_EXCESS_PRESSURE_POINTS_NUM (a);
      cost += ALLOCNO_MEMORY_COST (a) - ALLOCNO_COVER_CLASS_COST (a);
      nregs = hard_regno_nregs[hard_regno][ALLOCNO_MODE (a)];
      for (j = 0; j < nregs; j++)
	if (! TEST_HARD_REG_BIT (call_used_reg_set, hard_regno + j))
	  break;
      if (j == nregs)
	count++;
      in_p = in && REG_P (in) && (int) REGNO (in) == hard_regno;
      out_p = out && REG_P (out) && (int) REGNO (out) == hard_regno;
      if ((in_p || out_p)
	  && find_regno_note (insn, REG_DEAD, hard_regno) != NULL_RTX)
	{
	  saved_cost = 0;
	  if (in_p)
	    saved_cost += memory_move_cost
	                  [ALLOCNO_MODE (a)][ALLOCNO_COVER_CLASS (a)][1];
	  if (out_p)
	    saved_cost
	      += memory_move_cost
	         [ALLOCNO_MODE (a)][ALLOCNO_COVER_CLASS (a)][0];
	  cost -= REG_FREQ_FROM_BB (BLOCK_FOR_INSN (insn)) * saved_cost;
	}
    }
  *excess_pressure_live_length = length;
  *call_used_count = count;
  hard_regno = -1;
  if (regnos[0] >= 0)
    {
      hard_regno = reg_renumber[regnos[0]];
    }
  *first_hard_regno = hard_regno;
  return cost;
}

/* Return TRUE if spilling pseudo-registers whose numbers are in array
   REGNOS is better than spilling pseudo-registers with numbers in
   OTHER_REGNOS for reload with given IN and OUT for INSN.  The
   function used by the reload pass to make better register spilling
   decisions.  */
int
better_spill_reload_regno_p (int *regnos, int *other_regnos,
			     rtx in, rtx out, rtx insn)
{
  int cost, other_cost;
  int length, other_length;
  int nrefs, other_nrefs;
  int call_used_count, other_call_used_count;
  int hard_regno, other_hard_regno;

  cost = calculate_spill_cost (regnos, in, out, insn, 
			       &length, &nrefs, &call_used_count, &hard_regno);
  other_cost = calculate_spill_cost (other_regnos, in, out, insn,
				     &other_length, &other_nrefs,
				     &other_call_used_count,
				     &other_hard_regno);
  if (cost != other_cost)
    return cost < other_cost;
  if (length != other_length)
    return length > other_length;
#ifdef REG_ALLOC_ORDER
  if (hard_regno >= 0 && other_hard_regno >= 0)
    return (inv_reg_alloc_order[hard_regno]
	    < inv_reg_alloc_order[other_hard_regno]);
#else
  if (call_used_count != other_call_used_count)
    return call_used_count > other_call_used_count;
#endif
  return FALSE;
}



/* The function returns (through CALL_CLOBBERED_REGS) hard registers
   changed by all function calls inside REGNO live range.  The
   function is used to improve code for saving/restore callee-clobbered
   hard registers around calls (see caller-saves.c).  */
void
collect_pseudo_call_clobbered_regs (int regno,
				    HARD_REG_SET (*call_clobbered_regs))
{
  int i;
  allocno_t a;
  HARD_REG_SET clobbered_regs;
  rtx call, *allocno_calls;

  a = regno_allocno_map[regno];
  CLEAR_HARD_REG_SET (*call_clobbered_regs);
  allocno_calls = (VEC_address (rtx, regno_calls[regno])
		   + ALLOCNO_CALLS_CROSSED_START (a));
  for (i = ALLOCNO_CALLS_CROSSED_NUM (a) - 1; i >= 0; i--)
    {
      call = allocno_calls[i];
      get_call_invalidated_used_regs (call, &clobbered_regs, FALSE);
      IOR_HARD_REG_SET (*call_clobbered_regs, clobbered_regs);
    }
}



/* Allocate and initialize data necessary for assign_hard_reg.  */
void
initiate_ira_assign (void)
{
  sorted_allocnos = ira_allocate (sizeof (allocno_t) * allocnos_num);
  sorted_allocnos_for_spilling
    = ira_allocate (sizeof (allocno_t) * allocnos_num);
  consideration_allocno_bitmap = ira_allocate_bitmap ();
  initiate_cost_update ();
  allocno_priorities = ira_allocate (sizeof (int) * allocnos_num);
}

/* Deallocate data used by assign_hard_reg.  */
void
finish_ira_assign (void)
{
  ira_free (sorted_allocnos_for_spilling);
  ira_free (sorted_allocnos);
  ira_free_bitmap (consideration_allocno_bitmap);
  finish_cost_update ();
  ira_free (allocno_priorities);
}



/* Entry function doing color-based register allocation.  */
void
ira_color (void)
{
  VARRAY_GENERIC_PTR_NOGC_INIT (allocno_stack_varray, allocnos_num,
				"stack of allocnos");
  memset (allocated_hardreg_p, 0, sizeof (allocated_hardreg_p));
  initiate_ira_assign ();
  do_coloring ();
  finish_ira_assign ();
  VARRAY_FREE (allocno_stack_varray);
  move_spill_restore ();
}
