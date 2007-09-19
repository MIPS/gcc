/* IRA allocation based on graph coloring.
   Copyright (C) 2006, 2007
   Free Software Foundation, Inc.
   Contributed by Vladimir Makarov <vmakarov@redhat.com>.

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
#include "expr.h"
#include "toplev.h"
#include "reload.h"
#include "params.h"
#include "df.h"
#include "ira-int.h"

/* We use optimistic colouring with optional biased colouring.  */

static void update_copy_costs (allocno_t, int);
static int assign_hard_reg (allocno_t, int);

static void add_allocno_to_bucket (allocno_t, allocno_t *);
static void add_allocno_to_ordered_bucket (allocno_t, allocno_t *);
static void delete_allocno_from_bucket (allocno_t, allocno_t *);
static void push_allocno_to_stack (allocno_t);
static void remove_allocno_from_bucket_and_push (allocno_t, int);
static void push_only_colorable (void);
static void push_allocno_to_spill (allocno_t);
static int loop_edge_freq (struct ira_loop_tree_node *, int, int);
static int calculate_allocno_spill_cost (allocno_t);
static void push_allocnos_to_stack (void);
static void pop_allocnos_from_stack (void);
static void setup_allocno_available_regs_num (allocno_t);
static void setup_allocno_left_conflicts_num (allocno_t);
static void put_allocno_into_bucket (allocno_t);
static void color_allocnos (void);

static void print_loop_title (struct ira_loop_tree_node *);
static void color_pass (struct ira_loop_tree_node *);
static int allocno_priority_compare_func (const void *, const void *);
static void priority_coloring (void);
static void do_coloring (void);

static void move_spill_restore (void);

/* Bitmap of allocnos which should be colored.  */
static bitmap coloring_allocno_bitmap;

/* Bitmap of allocnos which should be taken into account during
   coloring.  In general case it contains allocnos from
   coloring_allocno_bitmap plus other already colored conflicting
   allocnos.  */
static bitmap consideration_allocno_bitmap;

/* All allocnos sorted accoring their priorities.  */
static allocno_t *sorted_allocnos;



/* This page contains function to choose hard register for allocnos.  */

/* Array whose element value is TRUE if the corresponding hard
   register already allocated for a allocno.  */
static int allocated_hardreg_p [FIRST_PSEUDO_REGISTER];

/* The function updates costs (decrease if DECR_P) of the allocnos
   connected by copies with ALLOCNO.  */
static void
update_copy_costs (allocno_t allocno, int decr_p)
{
  int i, hard_regno, cost;
  enum machine_mode mode;
  enum reg_class class;
  allocno_t another_allocno;
  copy_t cp, next_cp;

  if (ALLOCNO_COVER_CLASS (allocno) == NO_REGS)
    return;
  hard_regno = ALLOCNO_HARD_REGNO (allocno);
  ira_assert (hard_regno >= 0 && ALLOCNO_COVER_CLASS (allocno) != NO_REGS);
  i = class_hard_reg_index [ALLOCNO_COVER_CLASS (allocno)] [hard_regno];
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
      if (ALLOCNO_COVER_CLASS (allocno)
	  != ALLOCNO_COVER_CLASS (another_allocno))
	continue;
      cost = (cp->second == allocno
	      ? register_move_cost [mode] [class]
	        [ALLOCNO_COVER_CLASS (another_allocno)]
	      : register_move_cost [mode]
	        [ALLOCNO_COVER_CLASS (another_allocno)] [class]);
      if (decr_p)
	cost = -cost;
      ALLOCNO_CURR_HARD_REG_COSTS (another_allocno) [i] += cp->freq * cost;
      ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (another_allocno) [i]
	+= cp->freq * cost;
    }
}

/* Varray representing the stack of allocnos used during coloring.  */
static varray_type allocno_stack_varray;

/* Function choosing a hard register for ALLOCNO.  If RETRY_P is
   nonzero, it means that the function called from
   `retry_ira_color'. */
static int
assign_hard_reg (allocno_t allocno, int retry_p)
{
  HARD_REG_SET conflicting_regs, biased_regs;
  int i, j, hard_regno, best_hard_regno, class_size;
  int cost, mem_cost, min_cost, full_cost, min_full_cost, add_cost;
  int *costs;
  int *conflict_costs;
  enum reg_class cover_class, class;
  enum machine_mode mode;
  allocno_t conflict_allocno, conflict_allocno2;
  allocno_t *allocno_vec, *allocno_vec2;
  allocno_t another_allocno;
  copy_t cp, next_cp;
  static int full_costs [FIRST_PSEUDO_REGISTER];
#ifdef STACK_REGS
  int no_stack_reg_p;
#endif

  ira_assert (! ALLOCNO_ASSIGNED_P (allocno));
  cover_class = ALLOCNO_COVER_CLASS (allocno);
  class_size = class_hard_regs_num [cover_class];
  CLEAR_HARD_REG_SET (biased_regs);
  COPY_HARD_REG_SET (conflicting_regs, no_alloc_regs);
  IOR_COMPL_HARD_REG_SET (conflicting_regs, reg_class_contents [cover_class]);
  best_hard_regno = -1;
  mode = ALLOCNO_MODE (allocno);
  memset (full_costs, 0, sizeof (int) * class_size);
  mem_cost = ALLOCNO_MEMORY_COST (allocno);
  allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (allocno);
  IOR_HARD_REG_SET (conflicting_regs, ALLOCNO_CONFLICT_HARD_REGS (allocno));
  costs = ALLOCNO_CURR_HARD_REG_COSTS (allocno);
  memcpy (full_costs, costs, sizeof (int) * class_size);
#ifdef STACK_REGS
  no_stack_reg_p = ALLOCNO_NO_STACK_REG_P (allocno);
#endif
  for (i = 0; (conflict_allocno = allocno_vec [i]) != NULL; i++)
    /* Reload can give another class so we need to check all
       allocnos.  */
    if (retry_p
	|| (cover_class == ALLOCNO_COVER_CLASS (conflict_allocno)
	    && bitmap_bit_p (consideration_allocno_bitmap,
			     ALLOCNO_NUM (conflict_allocno))))
      {
	if (ALLOCNO_ASSIGNED_P (conflict_allocno))
	  {
	    if ((hard_regno = ALLOCNO_HARD_REGNO (conflict_allocno)) >= 0)
	      {
		IOR_HARD_REG_SET
		  (conflicting_regs,
		   reg_mode_hard_regset
		   [hard_regno] [ALLOCNO_MODE (conflict_allocno)]);
		if (hard_reg_set_subset_p (reg_class_contents [cover_class],
					   conflicting_regs))
		  goto fail;
	      }
	    continue;
	  }
	else if (! ALLOCNO_MAY_BE_SPILLED_P (conflict_allocno))
	  {
	    conflict_costs
	      = ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (conflict_allocno);
	    if (conflict_costs != NULL)
	      for (j = class_size - 1; j >= 0; j--)
		full_costs [j] -= conflict_costs [j];
	  }
	if (retry_p || ! flag_ira_biased_coloring)
	  continue;
	allocno_vec2 = ALLOCNO_CONFLICT_ALLOCNO_VEC (conflict_allocno);
	for (j = 0; (conflict_allocno2 = allocno_vec2 [j]) != NULL; j++)
	  if (cover_class == ALLOCNO_COVER_CLASS (conflict_allocno2)
	      && ALLOCNO_ASSIGNED_P (conflict_allocno2)
	      && (hard_regno = ALLOCNO_HARD_REGNO (conflict_allocno2)) >= 0
	      && (retry_p
		  || bitmap_bit_p (consideration_allocno_bitmap,
				   ALLOCNO_NUM (conflict_allocno2))))
	    IOR_HARD_REG_SET (biased_regs,
			      reg_mode_hard_regset
			      [hard_regno] [ALLOCNO_MODE (conflict_allocno2)]);
      }
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
      if (cover_class != ALLOCNO_COVER_CLASS (another_allocno)
	  || ALLOCNO_ASSIGNED_P (another_allocno))
	continue;
      conflict_costs = ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (another_allocno);
      if (conflict_costs != NULL
	  && ! ALLOCNO_MAY_BE_SPILLED_P (another_allocno))
	for (j = class_size - 1; j >= 0; j--)
 	  full_costs [j] += conflict_costs [j];
    }
  min_cost = min_full_cost = INT_MAX;
  /* We don't care about giving callee saved registers to allocnos no
     living through calls because call used register are allocated
     first (it is usual practice to put them first in
     REG_ALLOC_ORDER).  */
  for (i = 0; i < class_size; i++)
    {
      hard_regno = class_hard_regs [cover_class] [i];
#ifdef STACK_REGS
      if (no_stack_reg_p
	  && FIRST_STACK_REG <= hard_regno && hard_regno <= LAST_STACK_REG)
	continue;
#endif
      if (TEST_HARD_REG_BIT (prohibited_class_mode_regs
			     [cover_class] [mode], hard_regno))
	continue;
      if (hard_reg_not_in_set_p (hard_regno, mode, conflicting_regs))
	{
	  cost = costs [i];
	  full_cost = full_costs [i];
	  if (! allocated_hardreg_p [hard_regno]
	      && hard_reg_not_in_set_p (hard_regno, mode, call_used_reg_set))
	    /* We need to save/restore the register in
	       epilogue/prologue.  Therefore we increase the cost.  */
	    {
	      /* ??? If only part is call clobbered.  */
	      class = REGNO_REG_CLASS (hard_regno);
	      add_cost = (memory_move_cost [mode] [class] [0]
			  + memory_move_cost [mode] [class] [1] - 1);
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
	  else if (min_full_cost == full_cost && flag_ira_biased_coloring != 0
		   && TEST_HARD_REG_BIT (biased_regs, hard_regno)
		   && ! TEST_HARD_REG_BIT (biased_regs, best_hard_regno))
	    best_hard_regno = hard_regno;
	}
    }
  if (min_cost > mem_cost)
    best_hard_regno = -1;
 fail:
  if (best_hard_regno >= 0)
    allocated_hardreg_p [best_hard_regno] = TRUE;
  ALLOCNO_HARD_REGNO (allocno) = best_hard_regno;
  ALLOCNO_ASSIGNED_P (allocno) = TRUE;
  if (best_hard_regno >= 0)
    update_copy_costs (allocno, TRUE);
  return best_hard_regno >= 0;
}



/* This page contains allocator based on Chaitin algorithm.  */

/* Bucket of allocnos allocno be colored currently without spilling.  */
static allocno_t colorable_allocno_bucket;

/* Bucket of allocnos allocno might be not colored currently without
   spilling.  */
static allocno_t uncolorable_allocno_bucket;

/* Add ALLOCNO to *BUCKET_PTR bucket.  ALLOCNO should be not in a bucket
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

/* Add ALLOCNO to *BUCKET_PTR bucket maintaining the order according
   their frequency.  ALLOCNO should be not in a bucket before the
   call.  */
static void
add_allocno_to_ordered_bucket (allocno_t allocno, allocno_t *bucket_ptr)
{
  allocno_t before, after;
  enum reg_class cover_class;
  int freq, nregs;

  freq = ALLOCNO_FREQ (allocno);
  cover_class = ALLOCNO_COVER_CLASS (allocno);
  nregs = reg_class_nregs [cover_class] [ALLOCNO_MODE (allocno)];
  for (before = *bucket_ptr, after = NULL;
       before != NULL;
       after = before, before = ALLOCNO_NEXT_BUCKET_ALLOCNO (before))
    if (ALLOCNO_FREQ (before) > freq)
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

/* Delete ALLOCNO from *BUCKET_PTR bucket.  It should be there before
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
   it from the bucket.  Such action can result in moving conflicting
   allocnos from the uncolorable bucket to the colorable one.  */
static void
push_allocno_to_stack (allocno_t allocno)
{
  int i, conflicts_num, conflict_size, size;
  allocno_t conflict_allocno;
  allocno_t *allocno_vec;
  enum reg_class cover_class;
  
  ALLOCNO_IN_GRAPH_P (allocno) = FALSE;
  VARRAY_PUSH_GENERIC_PTR (allocno_stack_varray, allocno);
  cover_class = ALLOCNO_COVER_CLASS (allocno);
  if (cover_class == NO_REGS)
    return;
  size = reg_class_nregs [cover_class] [ALLOCNO_MODE (allocno)];
  allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (allocno);
  for (i = 0; (conflict_allocno = allocno_vec [i]) != NULL; i++)
    if (cover_class == ALLOCNO_COVER_CLASS (conflict_allocno)
	&& bitmap_bit_p (coloring_allocno_bitmap,
			 ALLOCNO_NUM (conflict_allocno)))
      {
	if (ALLOCNO_IN_GRAPH_P (conflict_allocno)
	    && ! ALLOCNO_ASSIGNED_P (conflict_allocno))
	  {
	    conflicts_num = ALLOCNO_LEFT_CONFLICTS_NUM (conflict_allocno);
	    conflict_size
	      = (reg_class_nregs
		 [cover_class] [ALLOCNO_MODE (conflict_allocno)]);
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
}

/* The function puts ALLOCNO onto the coloring stack and removes it
   from the bucket.  The allocno is in the colorable bucket if
   COLORABLE_P is nonzero.  */
static void
remove_allocno_from_bucket_and_push (allocno_t allocno, int colorable_p)
{
  enum reg_class cover_class;
  allocno_t *bucket_ptr;

  bucket_ptr = (colorable_p
		? &colorable_allocno_bucket : &uncolorable_allocno_bucket);
  delete_allocno_from_bucket (allocno, bucket_ptr);
  if (ira_dump_file != NULL)
    {
      fprintf (ira_dump_file, "  Pushing");
      print_expanded_allocno (allocno);
      fprintf (ira_dump_file, "%s\n", colorable_p ? "" : "(potential spill)");
    }
  cover_class = ALLOCNO_COVER_CLASS (allocno);
  ira_assert ((colorable_p
	       && (ALLOCNO_LEFT_CONFLICTS_NUM (allocno)
		   + reg_class_nregs [cover_class] [ALLOCNO_MODE (allocno)]
		   <= ALLOCNO_AVAILABLE_REGS_NUM (allocno)))
	      || (! colorable_p
		  && (ALLOCNO_LEFT_CONFLICTS_NUM (allocno)
		      + reg_class_nregs [cover_class] [ALLOCNO_MODE (allocno)]
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
  if (ira_dump_file != NULL)
    fprintf (ira_dump_file, "  Pushing p%d(%d) (potential spill)\n",
	     ALLOCNO_NUM (allocno), ALLOCNO_REGNO (allocno));
  push_allocno_to_stack (allocno);
}

/* The function returns frequency of exit edges (if EXIT_P) or enter
   from/to the loop given by its LOOP_NODE.  */ 
static int
loop_edge_freq (struct ira_loop_tree_node *loop_node, int regno, int exit_p)
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
  struct ira_loop_tree_node *father_node, *loop_node;

  regno = ALLOCNO_REGNO (a);
  cost = ALLOCNO_MEMORY_COST (a) - ALLOCNO_COVER_CLASS_COST (a);
  if (ALLOCNO_CAP (a) != NULL)
    return cost;
  loop_node = ALLOCNO_LOOP_TREE_NODE (a);
  if ((father_node = loop_node->father) == NULL)
    return cost;
  if ((father_allocno = father_node->regno_allocno_map [regno]) == NULL)
    return cost;
  mode = ALLOCNO_MODE (a);
  class = ALLOCNO_COVER_CLASS (a);
  if (ALLOCNO_HARD_REGNO (father_allocno) < 0)
    cost -= (memory_move_cost [mode] [class] [0]
	     * loop_edge_freq (loop_node, regno, TRUE)
	     + memory_move_cost [mode] [class] [1]
	     * loop_edge_freq (loop_node, regno, FALSE));
  else
    cost += ((memory_move_cost [mode] [class] [1]
	      * loop_edge_freq (loop_node, regno, TRUE)
	      + memory_move_cost [mode] [class] [0]
	      * loop_edge_freq (loop_node, regno, FALSE))
	     - (register_move_cost [mode] [class] [class]
		* (loop_edge_freq (loop_node, regno, FALSE)
		   + loop_edge_freq (loop_node, regno, TRUE))));
  return cost;
}

/* Push allocnos on the coloring stack.  The order of allocnos in the
   stack defines the order for the subsequent coloring.  */
static void
push_allocnos_to_stack (void)
{
  int i, j;
  double allocno_pri, i_allocno_pri;
  allocno_t allocno, i_allocno;
  allocno_t *allocno_vec;
  enum reg_class cover_class;
  int num, cover_class_allocnos_num [N_REG_CLASSES];
  allocno_t *cover_class_allocnos [N_REG_CLASSES];

  /* Initialize.  */
  for (i = 0; i < reg_class_cover_size; i++)
    {
      cover_class = reg_class_cover [i];
      cover_class_allocnos_num [cover_class] = 0;
      cover_class_allocnos [cover_class] = NULL;
    }
  /* Calculate uncolorable allocnos of each cover class.  */
  for (allocno = uncolorable_allocno_bucket;
       allocno != NULL;
       allocno = ALLOCNO_NEXT_BUCKET_ALLOCNO (allocno))
    if ((cover_class = ALLOCNO_COVER_CLASS (allocno)) != NO_REGS)
      {
	cover_class_allocnos_num [cover_class]++;
	ALLOCNO_TEMP (allocno) = INT_MAX;
      }
  /* Define place where to put uncolorable allocnos of the same cover
     class.  */
  for (num = i = 0; i < reg_class_cover_size; i++)
    {
      cover_class = reg_class_cover [i];
      if (cover_class_allocnos_num [cover_class] != 0)
	{
	  cover_class_allocnos [cover_class] = sorted_allocnos + num;
	  num += cover_class_allocnos_num [cover_class];
	  cover_class_allocnos_num [cover_class] = 0;
	}
    }
  ira_assert (num <= allocnos_num);
  /* Put uncolorable allocnos of the same cover class together.  */
  for (allocno = uncolorable_allocno_bucket;
       allocno != NULL;
       allocno = ALLOCNO_NEXT_BUCKET_ALLOCNO (allocno))
    if ((cover_class = ALLOCNO_COVER_CLASS (allocno)) != NO_REGS)
      cover_class_allocnos
	[cover_class] [cover_class_allocnos_num [cover_class]++] = allocno;
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
      ira_assert (reg_class_nregs [cover_class] [ALLOCNO_MODE (allocno)] > 0);
      num = cover_class_allocnos_num [cover_class];
      ira_assert (num > 0);
      allocno_vec = cover_class_allocnos [cover_class];
      allocno = NULL;
      allocno_pri = 0;
      /* Sort uncolorable allocno to find the one with the lowest spill
	 cost.  */
      for (i = 0, j = num - 1; i <= j;)
	{
	  i_allocno = allocno_vec [i];
	  if (! ALLOCNO_IN_GRAPH_P (i_allocno)
	      && ALLOCNO_IN_GRAPH_P (allocno_vec [j]))
	    {
	      i_allocno = allocno_vec [j];
	      allocno_vec [j] = allocno_vec [i];
	      allocno_vec [i] = i_allocno;
	    }
	  if (ALLOCNO_IN_GRAPH_P (i_allocno))
	    {
	      i++;
	      if (ALLOCNO_TEMP (i_allocno) == INT_MAX)
		ALLOCNO_TEMP (i_allocno)
		  = calculate_allocno_spill_cost (i_allocno);
	      i_allocno_pri
		= ((double) ALLOCNO_TEMP (i_allocno)
		   / (ALLOCNO_LEFT_CONFLICTS_NUM (i_allocno)
		      * reg_class_nregs [ALLOCNO_COVER_CLASS (i_allocno)]
		                        [ALLOCNO_MODE (i_allocno)] + 1));
	      if (allocno == NULL || allocno_pri > i_allocno_pri
		  || (allocno_pri == i_allocno_pri
		      && (ALLOCNO_NUM (allocno) > ALLOCNO_NUM (i_allocno))))
		{
		  allocno = i_allocno;
		  allocno_pri = i_allocno_pri;
		}
	    }
	  if (! ALLOCNO_IN_GRAPH_P (allocno_vec [j]))
	    j--;
	}
      ira_assert (allocno != NULL && j >= 0);
      cover_class_allocnos_num [cover_class] = j + 1;
      ira_assert (ALLOCNO_IN_GRAPH_P (allocno)
		  && ALLOCNO_COVER_CLASS (allocno) == cover_class
		  && (ALLOCNO_LEFT_CONFLICTS_NUM (allocno)
		      + reg_class_nregs [cover_class] [ALLOCNO_MODE (allocno)]
		      > ALLOCNO_AVAILABLE_REGS_NUM (allocno)));
      remove_allocno_from_bucket_and_push (allocno, FALSE);
    }
}

/* Assign hard registers to allocnos on the coloring stack.  */
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
      if (ira_dump_file != NULL)
	{
	  fprintf (ira_dump_file, "  Popping");
	  print_expanded_allocno (allocno);
	  fprintf (ira_dump_file, "  -- ");
	}
      if (cover_class == NO_REGS)
	{
	  ALLOCNO_HARD_REGNO (allocno) = -1;
	  ALLOCNO_ASSIGNED_P (allocno) = TRUE;
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file, "assign memory\n");
	}
      else if (assign_hard_reg (allocno, FALSE))
	{
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file, "assign reg %d\n",
		     ALLOCNO_HARD_REGNO (allocno));
	}
      else
	{
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file, "spill\n");
	}
      ALLOCNO_IN_GRAPH_P (allocno) = TRUE;
    }
}

/* Set up number of avaliable hard registers for ALLOCNO.  */
static void
setup_allocno_available_regs_num (allocno_t allocno)
{
  int i, n;
  enum reg_class cover_class;
  HARD_REG_SET temp_set;

  cover_class = ALLOCNO_COVER_CLASS (allocno);
  ALLOCNO_AVAILABLE_REGS_NUM (allocno) = available_class_regs [cover_class];
  if (cover_class == NO_REGS)
    return;
  COPY_HARD_REG_SET (temp_set, ALLOCNO_CONFLICT_HARD_REGS (allocno));
  for (n = 0, i = class_hard_regs_num [cover_class] - 1; i >= 0; i--)
    if (TEST_HARD_REG_BIT (temp_set, class_hard_regs [cover_class] [i]))
      n++;
  if (n > 0 && ira_dump_file != NULL)
    fprintf (ira_dump_file, "reg %d of %s has %d regs less\n",
	     ALLOCNO_REGNO (allocno), reg_class_names [cover_class], n);
  ALLOCNO_AVAILABLE_REGS_NUM (allocno) -= n;
}

/* Set up ALLOCNO_LEFT_CONFLICTS_NUM for ALLOCNO.  */
static void
setup_allocno_left_conflicts_num (allocno_t allocno)
{
  int i, hard_regs_num, hard_regno, conflict_allocnos_size;
  allocno_t conflict_allocno;
  allocno_t *allocno_vec;
  enum reg_class cover_class;
  HARD_REG_SET temp_set;

  cover_class = ALLOCNO_COVER_CLASS (allocno);
  hard_regs_num = class_hard_regs_num [cover_class];
  if (hard_regs_num != 0)
    {
      memcpy (ALLOCNO_CURR_HARD_REG_COSTS (allocno),
	      ALLOCNO_HARD_REG_COSTS (allocno), sizeof (int) * hard_regs_num);
      memcpy (ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (allocno),
	      ALLOCNO_CONFLICT_HARD_REG_COSTS (allocno),
	      sizeof (int) * hard_regs_num);
    }
  COPY_HARD_REG_SET (temp_set, ALLOCNO_CONFLICT_HARD_REGS (allocno));
  AND_HARD_REG_SET (temp_set, reg_class_contents [cover_class]);
  AND_COMPL_HARD_REG_SET (temp_set, no_alloc_regs);
  conflict_allocnos_size = 0;
  if (! hard_reg_set_equal_p (temp_set, zero_hard_reg_set))
    for (i = 0; i < (int) hard_regs_num; i++)
      {
	hard_regno = class_hard_regs [cover_class] [i];
	if (TEST_HARD_REG_BIT (temp_set, hard_regno))
	  {
	    conflict_allocnos_size++;
	    CLEAR_HARD_REG_BIT (temp_set, hard_regno);
	    if (hard_reg_set_equal_p (temp_set, zero_hard_reg_set))
	      break;
	  }
      }
  CLEAR_HARD_REG_SET (temp_set);
  allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (allocno);
  if (cover_class != NO_REGS)
    for (i = 0; (conflict_allocno = allocno_vec [i]) != NULL; i++)
      if (cover_class == ALLOCNO_COVER_CLASS (conflict_allocno)
	  && bitmap_bit_p (consideration_allocno_bitmap,
			   ALLOCNO_NUM (conflict_allocno)))
	{
	  if (! ALLOCNO_ASSIGNED_P (conflict_allocno))
	    conflict_allocnos_size
	      += (reg_class_nregs
		  [cover_class] [ALLOCNO_MODE (conflict_allocno)]);
	  else if ((hard_regno = ALLOCNO_HARD_REGNO (conflict_allocno)) >= 0)
	    {
	      int last = (hard_regno
			  + hard_regno_nregs
                            [hard_regno] [ALLOCNO_MODE (conflict_allocno)]);

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
  hard_regs_num = class_hard_regs_num [cover_class];
  if (hard_regs_num != 0)
    {
      memcpy (ALLOCNO_CURR_HARD_REG_COSTS (allocno),
	      ALLOCNO_HARD_REG_COSTS (allocno), sizeof (int) * hard_regs_num);
      memcpy (ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (allocno),
	      ALLOCNO_CONFLICT_HARD_REG_COSTS (allocno),
	      sizeof (int) * hard_regs_num);
    }
  ALLOCNO_IN_GRAPH_P (allocno) = TRUE;
  setup_allocno_left_conflicts_num (allocno);
  setup_allocno_available_regs_num (allocno);
  if (ALLOCNO_LEFT_CONFLICTS_NUM (allocno)
      + reg_class_nregs [cover_class] [ALLOCNO_MODE (allocno)]
      <= ALLOCNO_AVAILABLE_REGS_NUM (allocno))
    add_allocno_to_ordered_bucket (allocno, &colorable_allocno_bucket);
  else
    add_allocno_to_bucket (allocno, &uncolorable_allocno_bucket);
}

/* Function implements Chaitin-Briggs coloring for allocnos in
   COLORING_ALLOCNO_BITMAP taking into account allocnos in
   CONSIDERATION_ALLOCNO_BITMAP.  */
static void
color_allocnos (void)
{
  unsigned int i;
  bitmap_iterator bi;

  /* Put the allocnos into the corresponding buckets.  */
  colorable_allocno_bucket = NULL;
  uncolorable_allocno_bucket = NULL;
  EXECUTE_IF_SET_IN_BITMAP (coloring_allocno_bitmap, 0, i, bi)
    put_allocno_into_bucket (allocnos [i]);
  push_allocnos_to_stack ();
  pop_allocnos_from_stack ();
}



/* The function outputs information about the loop given by its
   LOOP_TREE_NODE. */
static void
print_loop_title (struct ira_loop_tree_node *loop_tree_node)
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
    fprintf (ira_dump_file, " %dr%d", j, ALLOCNO_REGNO (allocnos [j]));
  fprintf (ira_dump_file, "\n    modified regnos:");
  EXECUTE_IF_SET_IN_BITMAP (loop_tree_node->modified_regnos, 0, j, bi)
    fprintf (ira_dump_file, " %d", j);
  fprintf (ira_dump_file, "\n    border:");
  EXECUTE_IF_SET_IN_BITMAP (loop_tree_node->border_allocnos, 0, j, bi)
    fprintf (ira_dump_file, " %dr%d", j, ALLOCNO_REGNO (allocnos [j]));
  fprintf (ira_dump_file, "\n    Pressure:");
  for (j = 0; (int) j < reg_class_cover_size; j++)
    {
      enum reg_class cover_class;
      
      cover_class = reg_class_cover [j];
      if (loop_tree_node->reg_pressure [cover_class] == 0)
	continue;
      fprintf (ira_dump_file, " %s=%d", reg_class_names [cover_class],
	       loop_tree_node->reg_pressure [cover_class]);
    }
  fprintf (ira_dump_file, "\n");
}

/* The function implements Chaitin-Briggs coloring for allocnos inside
   loop (in extreme case it can be all function) given by the
   corresponding LOOP_TREE_NODE.  */
static void
color_pass (struct ira_loop_tree_node *loop_tree_node)
{
  int regno, hard_regno, index = -1;
  int cost, exit_freq, enter_freq;
  unsigned int j;
  bitmap_iterator bi;
  enum machine_mode mode;
  enum reg_class class;
  allocno_t a, subloop_allocno;
  struct ira_loop_tree_node *subloop_node;

  if (loop_tree_node->loop == NULL)
    return;
  if (ira_dump_file != NULL)
    print_loop_title (loop_tree_node);

  bitmap_copy (coloring_allocno_bitmap, loop_tree_node->mentioned_allocnos);
  bitmap_ior_into (coloring_allocno_bitmap, loop_tree_node->border_allocnos);
  bitmap_copy (consideration_allocno_bitmap, coloring_allocno_bitmap);
  EXECUTE_IF_SET_IN_BITMAP (consideration_allocno_bitmap, 0, j, bi)
    {
      a = allocnos [j];
      if (! ALLOCNO_ASSIGNED_P (a))
	continue;
      bitmap_clear_bit (coloring_allocno_bitmap, ALLOCNO_NUM (a));
    }
  /* Color all mentioned including transparent.  */
  color_allocnos ();
  /* Update costs for subloops.  */
  for (subloop_node = loop_tree_node->inner;
       subloop_node != NULL;
       subloop_node = subloop_node->next)
    if (subloop_node->bb == NULL)
      EXECUTE_IF_SET_IN_BITMAP (consideration_allocno_bitmap, 0, j, bi)
        {
	  a = allocnos [j];
	  mode = ALLOCNO_MODE (a);
	  class = ALLOCNO_COVER_CLASS (a);
	  hard_regno = ALLOCNO_HARD_REGNO (a);
	  if (hard_regno >= 0)
	    {
	      index = class_hard_reg_index [class] [hard_regno];
	      ira_assert (index >= 0);
	    }
	  regno = ALLOCNO_REGNO (a);
	  /* ??? conflict costs */
	  if (ALLOCNO_CAP_MEMBER (a) == NULL)
	    {
	      subloop_allocno = subloop_node->regno_allocno_map [regno];
	      if (subloop_allocno == NULL)
		continue;
	      if ((flag_ira_algorithm == IRA_ALGORITHM_MIXED
		   && subloop_node->reg_pressure [class]
		      <= available_class_regs [class]))
		{
		  if (! ALLOCNO_ASSIGNED_P (subloop_allocno))
		    {
		      ALLOCNO_HARD_REGNO (subloop_allocno) = hard_regno;
		      ALLOCNO_ASSIGNED_P (subloop_allocno) = TRUE;
		      if (hard_regno >= 0)
			update_copy_costs (subloop_allocno, TRUE);
		    }
		  continue;
		}
	      exit_freq = loop_edge_freq (subloop_node, regno, TRUE);
	      enter_freq = loop_edge_freq (subloop_node, regno, FALSE);
	      if (reg_equiv_invariant_p [regno]
		  || reg_equiv_const [regno] != NULL_RTX)
		{
		  if (! ALLOCNO_ASSIGNED_P (subloop_allocno))
		    {
		      ALLOCNO_HARD_REGNO (subloop_allocno) = hard_regno;
		      ALLOCNO_ASSIGNED_P (subloop_allocno) = TRUE;
		      if (hard_regno >= 0)
			update_copy_costs (subloop_allocno, TRUE);
		    }
		}
	      else if (hard_regno < 0)
		{
		  ALLOCNO_MEMORY_COST (subloop_allocno)
		    -= ((memory_move_cost [mode] [class] [1] * enter_freq)
			+ (memory_move_cost [mode] [class] [0] * exit_freq));
		}
	      else
		{
		  cost = (register_move_cost [mode] [class] [class] 
			  * (exit_freq + enter_freq));
		  ALLOCNO_HARD_REG_COSTS (subloop_allocno) [index] -= cost;
		  ALLOCNO_CONFLICT_HARD_REG_COSTS (subloop_allocno) [index]
		    -= cost;
		  ALLOCNO_MEMORY_COST (subloop_allocno)
		    += (memory_move_cost [mode] [class] [0] * enter_freq
			+ memory_move_cost [mode] [class] [1] * exit_freq);
		  if (ALLOCNO_COVER_CLASS_COST (subloop_allocno)
		      > ALLOCNO_HARD_REG_COSTS (subloop_allocno) [index])
		    ALLOCNO_COVER_CLASS_COST (subloop_allocno)
		      = ALLOCNO_HARD_REG_COSTS (subloop_allocno) [index];
		}
	    }
	  else
	    {
	      if ((flag_ira_algorithm == IRA_ALGORITHM_MIXED
		   && subloop_node->reg_pressure [class]
		      <= available_class_regs [class]))
		{
		  subloop_allocno = ALLOCNO_CAP_MEMBER (a);
		  if (! ALLOCNO_ASSIGNED_P (subloop_allocno))
		    {
		      ALLOCNO_HARD_REGNO (subloop_allocno) = hard_regno;
		      ALLOCNO_ASSIGNED_P (subloop_allocno) = TRUE;
		      if (hard_regno >= 0)
			update_copy_costs (subloop_allocno, TRUE);
		    }
		}
	      else if (flag_ira_propagate_cost && hard_regno >= 0)
		{
		  exit_freq = loop_edge_freq (subloop_node, -1, TRUE);
		  enter_freq = loop_edge_freq (subloop_node, -1, FALSE);
		  cost = (register_move_cost [mode] [class] [class] 
			  * (exit_freq + enter_freq));
		  subloop_allocno = ALLOCNO_CAP_MEMBER (a);
		  ALLOCNO_HARD_REG_COSTS (subloop_allocno) [index] -= cost;
		  ALLOCNO_CONFLICT_HARD_REG_COSTS (subloop_allocno) [index]
		    -= cost;
		  ALLOCNO_MEMORY_COST (subloop_allocno)
		    += (memory_move_cost [mode] [class] [0] * enter_freq
			+ memory_move_cost [mode] [class] [1] * exit_freq);
		  if (ALLOCNO_COVER_CLASS_COST (subloop_allocno)
		      > ALLOCNO_HARD_REG_COSTS (subloop_allocno) [index])
		    ALLOCNO_COVER_CLASS_COST (subloop_allocno)
		      = ALLOCNO_HARD_REG_COSTS (subloop_allocno) [index];
		}
	    }
	}
}

/* The function is used to sort allocnos according to their priorities
   which are calculated analogous to ones in file `global.c'.  */
static int
allocno_priority_compare_func (const void *v1p, const void *v2p)
{
  allocno_t p1 = *(const allocno_t *) v1p, p2 = *(const allocno_t *) v2p;
  int pri1, pri2;

#if 0
  pri1 = (((double) (floor_log2 (REG_N_REFS (ALLOCNO_REGNO (p1)))
		     * ALLOCNO_FREQ (p1))
	   / REG_LIVE_LENGTH (ALLOCNO_REGNO (p1)))
	  * (10000 / REG_FREQ_MAX) * PSEUDO_REGNO_SIZE (ALLOCNO_REGNO (p1)));
  pri2 = (((double) (floor_log2 (REG_N_REFS (ALLOCNO_REGNO (p2)))
		     * ALLOCNO_FREQ (p2))
	   / REG_LIVE_LENGTH (ALLOCNO_REGNO (p2)))
	  * (10000 / REG_FREQ_MAX) * PSEUDO_REGNO_SIZE (ALLOCNO_REGNO (p2)));
#else
  pri1 = (ALLOCNO_FREQ (p1) * (10000 / REG_FREQ_MAX)
	  * PSEUDO_REGNO_SIZE (ALLOCNO_REGNO (p1)));
  pri2 = (ALLOCNO_FREQ (p2) * (10000 / REG_FREQ_MAX)
	  * PSEUDO_REGNO_SIZE (ALLOCNO_REGNO (p2)));
#endif
  if (pri2 - pri1)
    return pri2 - pri1;

  /* If regs are equally good, sort by allocnos, so that the results of
     qsort leave nothing to chance.  */
  return ALLOCNO_NUM (p1) - ALLOCNO_NUM (p2);
}

/* The function implements Chow's prioity-based coloring.  */
static void
priority_coloring (void)
{
  int i, hard_regs_num;
  allocno_t a;

  memcpy (sorted_allocnos, allocnos, allocnos_num * sizeof (allocno_t));
  for (i = 0; i < allocnos_num; i++)
    {
      bitmap_set_bit (coloring_allocno_bitmap, i);
      a = allocnos [i];
      hard_regs_num = class_hard_regs_num [ALLOCNO_COVER_CLASS (a)];
      if (hard_regs_num == 0)
	continue;
      memcpy (ALLOCNO_CURR_HARD_REG_COSTS (a),
	      ALLOCNO_HARD_REG_COSTS (a), sizeof (int) * hard_regs_num);
      memcpy (ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (a),
	      ALLOCNO_CONFLICT_HARD_REG_COSTS (a),
	      sizeof (int) * hard_regs_num);
    }
  bitmap_copy (consideration_allocno_bitmap, coloring_allocno_bitmap);
  qsort (sorted_allocnos, allocnos_num, sizeof (allocno_t),
	 allocno_priority_compare_func);
  for (i = 0; i < allocnos_num; i++)
    {
      a = sorted_allocnos [i];
      if (ira_dump_file != NULL)
	{
	  fprintf (ira_dump_file, "  ");
	  print_expanded_allocno (a);
	  fprintf (ira_dump_file, "  -- ");
	}
      if (assign_hard_reg (a, FALSE))
	{
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file, "assign reg %d\n",
		     ALLOCNO_HARD_REGNO (a));
	}
      else
	{
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file, "spill\n");
	}
      ALLOCNO_IN_GRAPH_P (a) = TRUE;
    }
}

/* The function initialized common data for cloring and calls
   functions to do Chaitin-Briggs, regional, and Chow's priority-based
   coloring.  */
static void
do_coloring (void)
{
  coloring_allocno_bitmap = ira_allocate_bitmap ();
  consideration_allocno_bitmap = ira_allocate_bitmap ();

  if (flag_ira_algorithm == IRA_ALGORITHM_PRIORITY)
    priority_coloring ();
  else
    {
      if (ira_dump_file != NULL)
	fprintf (ira_dump_file, "\n**** Allocnos coloring:\n\n");
      
      traverse_loop_tree (ira_loop_tree_root, color_pass, NULL);
    }

  if (ira_dump_file != NULL)
    print_disposition (ira_dump_file);

  ira_free_bitmap (consideration_allocno_bitmap);
  ira_free_bitmap (coloring_allocno_bitmap);
}



/* The functions moves future spill/restore code to less frequent
   points (if it is profitable) by reassigning some allocnos to memory
   which means make longer live-range where the corresponding
   pseudo-registers will be in memory.  */
static void
move_spill_restore (void)
{
  int i, cost, changed_p, regno, hard_regno, hard_regno2, index;
  int enter_freq, exit_freq;
  enum machine_mode mode;
  enum reg_class class;
  allocno_t a, father_allocno, subloop_allocno;
  struct ira_loop_tree_node *father, *loop_node, *subloop_node;

  for (;;)
    {
      changed_p = FALSE;
      if (ira_dump_file != NULL)
	fprintf (ira_dump_file, "New iteration of spill/restore move\n");
      for (i = 0; i < allocnos_num; i++)
	{
	  a = allocnos [i];
	  regno = ALLOCNO_REGNO (a);
	  loop_node = ALLOCNO_LOOP_TREE_NODE (a);
	  if (ALLOCNO_CAP_MEMBER (a) != NULL
	      || (hard_regno = ALLOCNO_HARD_REGNO (a)) < 0
	      || loop_node->inner == NULL)
	    continue;
	  mode = ALLOCNO_MODE (a);
	  class = ALLOCNO_COVER_CLASS (a);
	  index = class_hard_reg_index [class] [hard_regno];
	  ira_assert (index >= 0);
	  cost = (ALLOCNO_ORIGINAL_MEMORY_COST (a)
		  - ALLOCNO_HARD_REG_COSTS (a) [index]);
	  for (subloop_node = loop_node->inner;
	       subloop_node != NULL;
	       subloop_node = subloop_node->next)
	    {
	      if (subloop_node->bb != NULL)
		continue;
	      subloop_allocno = subloop_node->regno_allocno_map [regno];
	      if (subloop_allocno == NULL)
		continue;
	      cost -= (ALLOCNO_ORIGINAL_MEMORY_COST (subloop_allocno)
		       - ALLOCNO_HARD_REG_COSTS (subloop_allocno) [index]);
	      exit_freq = loop_edge_freq (subloop_node, regno, TRUE);
	      enter_freq = loop_edge_freq (subloop_node, regno, FALSE);
	      if ((hard_regno2 = ALLOCNO_HARD_REGNO (subloop_allocno)) < 0)
		cost -= (memory_move_cost [mode] [class] [0] * exit_freq
			 + memory_move_cost [mode] [class] [1] * enter_freq);
	      else
		{
		  cost += (memory_move_cost [mode] [class] [0] * exit_freq
			   + memory_move_cost [mode] [class] [1] * enter_freq);
		  if (hard_regno2 != hard_regno)
		    cost -= (register_move_cost [mode] [class] [class]
			     * (exit_freq + enter_freq));
		}
	    }
	  if ((father = loop_node->father) != NULL
	      && (father_allocno = father->regno_allocno_map [regno]) != NULL)
	    {
	      exit_freq	= loop_edge_freq (loop_node, regno, TRUE);
	      enter_freq = loop_edge_freq (loop_node, regno, FALSE);
	      if ((hard_regno2 = ALLOCNO_HARD_REGNO (father_allocno)) < 0)
		cost -= (memory_move_cost [mode] [class] [0] * exit_freq
			 + memory_move_cost [mode] [class] [1] * enter_freq);
	      else
		{
		  cost += (memory_move_cost [mode] [class] [1] * exit_freq
			   + memory_move_cost [mode] [class] [0] * enter_freq);
		  if (hard_regno2 != hard_regno)
		    cost -= (register_move_cost [mode] [class] [class]
			     * (exit_freq + enter_freq));
		}
	    }
	  if (cost < 0)
	    {
	      ALLOCNO_HARD_REGNO (a) = -1;
	      if (ira_dump_file != NULL)
		fprintf (ira_dump_file,
			 "Moving spill/restore for a%dr%d up from loop %d - profit %d\n",
			 ALLOCNO_NUM (a), regno, loop_node->loop->num, -cost);
	      changed_p = TRUE;
	    }
	}
      if (! changed_p)
	break;
    }
}



/* Set up current hard reg costs and current conflict hard reg costs
   for allocno A.  */
static void
setup_curr_costs (allocno_t a)
{
  int i, hard_regno, cost, hard_regs_num;
  enum machine_mode mode;
  enum reg_class cover_class, class;
  allocno_t another_a;
  copy_t cp, next_cp;

  ira_assert (! ALLOCNO_ASSIGNED_P (a));
  cover_class = ALLOCNO_COVER_CLASS (a);
  if (cover_class == NO_REGS)
    return;
  hard_regs_num = class_hard_regs_num [cover_class];
  if (hard_regs_num == 0)
    return;
  mode = ALLOCNO_MODE (a);
  memcpy (ALLOCNO_CURR_HARD_REG_COSTS (a),
	  ALLOCNO_HARD_REG_COSTS (a),
	  sizeof (int) * hard_regs_num);
  memcpy (ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (a),
	  ALLOCNO_CONFLICT_HARD_REG_COSTS (a),
	  sizeof (int) * hard_regs_num);
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
      i = class_hard_reg_index [cover_class] [hard_regno];
      ira_assert (i >= 0);
      cost = (cp->first == a
	      ? register_move_cost [mode] [class] [cover_class]
	      : register_move_cost [mode] [cover_class] [class]);
      ALLOCNO_CURR_HARD_REG_COSTS (a) [i] -= cp->freq * cost;
      ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (a) [i] -= cp->freq * cost;
    }
}

/* Try to assign hard registers to the unassigned allocnos and allocnos
   conflicting with them or conflicting with allocnos whose regno >=
   START_REGNO.  We only try to assign a hard register to allocnos
   which do not live across calls if NO_CALL_CROSS_P.  */
void
reassign_conflict_allocnos (int start_regno, int no_call_cross_p)
{
  int i, j, allocnos_to_color_num;
  allocno_t a, conflict_a, *allocno_vec;
  enum reg_class cover_class;
  bitmap allocnos_to_color;

  sorted_allocnos = ira_allocate (sizeof (allocno_t) * allocnos_num);
  allocnos_to_color = ira_allocate_bitmap ();
  allocnos_to_color_num = 0;
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (! ALLOCNO_ASSIGNED_P (a)
	  && ! bitmap_bit_p (allocnos_to_color, ALLOCNO_NUM (a)))
	{
	  if (ALLOCNO_COVER_CLASS (a) != NO_REGS
	      && (! no_call_cross_p || ALLOCNO_CALLS_CROSSED_NUM (a) == 0))
	    sorted_allocnos [allocnos_to_color_num++] = a;
	  else
	    {
	      ALLOCNO_ASSIGNED_P (a) = TRUE;
	      ALLOCNO_HARD_REGNO (a) = -1;
	    }
	  bitmap_set_bit (allocnos_to_color, ALLOCNO_NUM (a));
	}
      if (ALLOCNO_REGNO (a) < start_regno)
	continue;
      allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (a);
      for (j = 0; (conflict_a = allocno_vec [j]) != NULL; j++)
	{
	  if ((cover_class = ALLOCNO_COVER_CLASS (conflict_a)) == NO_REGS
	      || (no_call_cross_p
		  && ALLOCNO_CALLS_CROSSED_NUM (conflict_a) != 0)
	      || bitmap_bit_p (allocnos_to_color, ALLOCNO_NUM (conflict_a)))
	    continue;
	  bitmap_set_bit (allocnos_to_color, ALLOCNO_NUM (conflict_a));
	  sorted_allocnos [allocnos_to_color_num++] = conflict_a;
	}
    }
  ira_free_bitmap (allocnos_to_color);
  if (allocnos_to_color_num > 1)
    qsort (sorted_allocnos, allocnos_to_color_num, sizeof (allocno_t),
	   allocno_priority_compare_func);
  for (i = 0; i < allocnos_to_color_num; i++)
    {
      a = sorted_allocnos [i];
      ALLOCNO_ASSIGNED_P (a) = FALSE;
      setup_curr_costs (a);
    }
  for (i = 0; i < allocnos_to_color_num; i++)
    {
      a = sorted_allocnos [i];
      if (assign_hard_reg (a, TRUE))
	{
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file,
		     "after call opt: assign hard reg %d to reg %d\n",
		     ALLOCNO_HARD_REGNO (a), ALLOCNO_REGNO (a));
	}
    }
  ira_free (sorted_allocnos);
}



/* The function called from the reload to mark changes in the
   allocation of REGNO made by the reload.  */
void
mark_allocation_change (int regno)
{
  allocno_t a = regno_allocno_map [regno];
  int old_hard_regno, hard_regno, cost;
  enum reg_class cover_class = ALLOCNO_COVER_CLASS (a);

  ira_assert (a != NULL);
  hard_regno = reg_renumber [regno];
  if ((old_hard_regno = ALLOCNO_HARD_REGNO (a)) == hard_regno)
    return;
  if (old_hard_regno < 0)
    cost = -ALLOCNO_MEMORY_COST (a);
  else
    {
      ira_assert (class_hard_reg_index [cover_class] [old_hard_regno] >= 0);
      cost = -(ALLOCNO_HARD_REG_COSTS (a)
	       [class_hard_reg_index [cover_class] [old_hard_regno]]);
      update_copy_costs (a, FALSE);
    }
  overall_cost -= cost;
  ALLOCNO_HARD_REGNO (a) = hard_regno;
  if (hard_regno < 0)
    cost += ALLOCNO_MEMORY_COST (a);
  else if (class_hard_reg_index [cover_class] [hard_regno] >= 0)
    {
      cost += (ALLOCNO_HARD_REG_COSTS (a)
	       [class_hard_reg_index [cover_class] [hard_regno]]);
      update_copy_costs (a, TRUE);
    }
  else
    /* Reload chages class of the allocno.  */
    cost = 0;
  overall_cost += cost;
}

/* The function is analog of function `retry_global_alloc'.  It is
   called by reload to try to put spilled pseudo-register REGNO into a
   hard register which is not in FORBIDDEN_REGS.  */
void
retry_ira_color (int regno, HARD_REG_SET forbidden_regs)
{
  allocno_t a;
  int hard_regno;
  enum reg_class cover_class;

  a = regno_allocno_map [regno];
  mark_allocation_change (regno);
  ira_assert (reg_renumber [regno] < 0);
  if (ira_dump_file != NULL)
    fprintf (ira_dump_file, "spill %d(a%d), cost=%d", regno, ALLOCNO_NUM (a),
	     ALLOCNO_MEMORY_COST (a) - ALLOCNO_COVER_CLASS_COST (a));
  IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a), forbidden_regs);
  if ((! flag_caller_saves || flag_ira_split_around_calls)
      && ALLOCNO_CALLS_CROSSED_NUM (a) != 0)
    IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a), call_used_reg_set);
  ALLOCNO_ASSIGNED_P (a) = FALSE;
  cover_class = ALLOCNO_COVER_CLASS (a);
  setup_curr_costs (a);
  assign_hard_reg (a, TRUE);
  hard_regno = ALLOCNO_HARD_REGNO (a);
  reg_renumber [regno] = hard_regno;
  if (hard_regno >= 0)
    {
      ira_assert (class_hard_reg_index [cover_class] [hard_regno] >= 0);
      overall_cost -= (ALLOCNO_MEMORY_COST (a)
		       - (ALLOCNO_HARD_REG_COSTS (a)
			  [class_hard_reg_index
			   [cover_class] [hard_regno]]));
      if (ALLOCNO_CALLS_CROSSED_NUM (a) != 0
	  && ! hard_reg_not_in_set_p (hard_regno, ALLOCNO_MODE (a),
				      call_used_reg_set))
	{
	  ira_assert (flag_caller_saves && ! flag_ira_split_around_calls);
	  caller_save_needed = 1;
	}
    }

  /* If we found a register, modify the RTL for the register to show
     the hard register, and mark that register live.  */
  if (reg_renumber[regno] >= 0)
    {
      if (ira_dump_file != NULL)
	fprintf (ira_dump_file, ": reassign to %d", reg_renumber[regno]);
      SET_REGNO (regno_reg_rtx[regno], reg_renumber[regno]);
      mark_home_live (regno);
    }
  else if (ira_dump_file != NULL && original_regno_call_crossed_p [regno]
	   && ! flag_caller_saves && flag_ira_split_around_calls
	   && get_around_calls_regno (regno) >= 0)
    fprintf (ira_dump_file, "+++spilling %d\n", regno);

  if (ira_dump_file != NULL)
    fprintf (ira_dump_file, "\n");
}



/* The function called by the reload returns already allocated stack
   slot (if any) for REGNO with given INHERENT_SIZE and
   TOTAL_SIZE.  */
rtx
reuse_stack_slot (int regno, unsigned int inherent_size,
		  unsigned int total_size)
{
  unsigned int i;
  int n;
  int freq, best_freq = -1;
  struct spilled_reg_stack_slot *best_slot = NULL;
  allocno_t another_allocno, allocno = regno_allocno_map [regno];
  copy_t cp, next_cp;
  rtx x;
  bitmap_iterator bi;
  struct spilled_reg_stack_slot *slot = NULL;

  ira_assert (flag_ira && inherent_size == PSEUDO_REGNO_BYTES (regno)
	      && inherent_size <= total_size);
  if (! flag_ira_share_spill_slots)
    return NULL_RTX;
  x = NULL_RTX;
  if (flag_omit_frame_pointer)
    n = spilled_reg_stack_slots_num - 1;
  else
    n = 0;
  if (x == NULL_RTX)
    {
      for (;;)
	{
	  if (flag_omit_frame_pointer)
	    {
	      if (n < 0)
		break;
	      slot = &spilled_reg_stack_slots [n--];
	    }
	  else if (n >= spilled_reg_stack_slots_num)
	    break;
	  else
	    slot = &spilled_reg_stack_slots [n++];
	  if (slot->width < total_size
	      || GET_MODE_SIZE (GET_MODE (slot->mem)) < inherent_size)
	    continue;
	  
	  EXECUTE_IF_SET_IN_BITMAP (&slot->spilled_regs,
				    FIRST_PSEUDO_REGISTER, i, bi)
	    {
	      if (allocno_reg_conflict_p (regno, i))
		goto cont;
	    }
	  for (freq = 0, cp = ALLOCNO_COPIES (allocno);
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
	      if (bitmap_bit_p (&slot->spilled_regs,
				ALLOCNO_REGNO (another_allocno)))
		freq += cp->freq;
	    }
	  if (freq > best_freq)
	    {
	      best_freq = freq;
	      best_slot = slot;
	    }
	cont:
	  ;
	}
      if (best_freq >= 0)
	{
	  SET_REGNO_REG_SET (&best_slot->spilled_regs, regno);
	  x = best_slot->mem;
	}
    }
  if (x)
    {
      if (ira_dump_file)
	{
	  fprintf (ira_dump_file, " Assigning %d slot of", regno);
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

/* The function called by the reload when a new stack slot X with
   TOTAL_SIZE was allocated for REGNO.  */
void
mark_new_stack_slot (rtx x, int regno, unsigned int total_size)
{
  struct spilled_reg_stack_slot *slot;

  ira_assert (flag_ira && PSEUDO_REGNO_BYTES (regno) <= total_size);
  slot = &spilled_reg_stack_slots [spilled_reg_stack_slots_num++];
  INIT_REG_SET (&slot->spilled_regs);
  SET_REGNO_REG_SET (&slot->spilled_regs, regno);
  slot->mem = x;
  slot->width = total_size;
  if (ira_dump_file)
    fprintf (ira_dump_file, " Assigning %d a new slot\n", regno);
}



/* The function returns (through CALL_CLOBBERED_REGS) hard registers
   changed by all function calls in REGNO live range.  */
void
collect_pseudo_call_clobbered_regs (int regno,
				    HARD_REG_SET (*call_clobbered_regs))
{
  int i;
  allocno_t a;
  HARD_REG_SET clobbered_regs;
  rtx call, *allocno_calls;

  a = regno_allocno_map [regno];
  CLEAR_HARD_REG_SET (*call_clobbered_regs);
  allocno_calls = (VEC_address (rtx, regno_calls [regno])
		   + ALLOCNO_CALLS_CROSSED_START (a));
  for (i = ALLOCNO_CALLS_CROSSED_NUM (a) - 1; i >= 0; i--)
    {
      call = allocno_calls [i];
      get_call_invalidated_used_regs (call, &clobbered_regs, FALSE);
      IOR_HARD_REG_SET (*call_clobbered_regs, clobbered_regs);
    }
}



/* Entry function doing color-based register allocation.  */
void
ira_color (void)
{
  sorted_allocnos = ira_allocate (sizeof (allocno_t) * allocnos_num);
  VARRAY_GENERIC_PTR_NOGC_INIT (allocno_stack_varray, allocnos_num,
				"stack of allocnos");
  memset (allocated_hardreg_p, 0, sizeof (allocated_hardreg_p));
  do_coloring ();
  VARRAY_FREE (allocno_stack_varray);
  ira_free (sorted_allocnos);
  move_spill_restore ();
}
