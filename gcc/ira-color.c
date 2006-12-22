/* IRA allocation based on graph coloring.
   Contributed by Vladimir Makarov.
   Copyright (C) 2006 Free Software Foundation, Inc.

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

static void update_copy_costs (pseudo_t, int);
static int assign_hard_reg (pseudo_t, int);

static void add_pseudo_to_bucket (pseudo_t, pseudo_t *);
static void add_pseudo_to_ordered_bucket (pseudo_t, pseudo_t *);
static void delete_pseudo_from_bucket (pseudo_t, pseudo_t *);
static void push_pseudo_to_stack (pseudo_t);
static void remove_pseudo_from_bucket_and_push (pseudo_t, int);
static void push_only_colorable (void);
static void push_pseudo_to_spill (pseudo_t);
static int loop_edge_freq (struct ira_loop_tree_node *, int, int);
static int calculate_pseudo_spill_cost (pseudo_t);
static void push_pseudos_to_stack (void);
static void pop_pseudos_from_stack (void);
static void put_pseudo_into_bucket (pseudo_t);
static void color_pseudos (void);

static void print_loop_title (struct ira_loop_tree_node *);
static void color_pass (struct ira_loop_tree_node *);
static int pseudo_priority_compare_func (const void *, const void *);
static void priority_coloring (void);
static void do_coloring (void);

static void move_spill_restore (void);

static void update_pseudo_avaialable_regs (void);

/* Bitmap of pseudos which should be colored.  */
static bitmap coloring_pseudo_bitmap;

/* Bitmap of pseudos which should be taken into account during
   coloring.  In general case it contains pseudos from
   coloring_pseudo_bitmap plus other already colored conflicting
   pseudos.  */
static bitmap consideration_pseudo_bitmap;



/* This page contains function to choose hard register for pseudos.  */

/* Array whose element value is TRUE if the corresponding hard
   register already allocated for a pseudo.  */
static int allocated_hardreg_p [FIRST_PSEUDO_REGISTER];

/* The function updates costs (decrease if DECR_P) of the pseudos
   connected by copies with PSEUDO.  */
static void
update_copy_costs (pseudo_t pseudo, int decr_p)
{
  int i, hard_regno, cost;
  enum machine_mode mode;
  enum reg_class class;
  pseudo_t another_pseudo;
  struct pseudo_copy *cp, *next_cp;

  if (PSEUDO_COVER_CLASS (pseudo) == NO_REGS)
    return;
  hard_regno = PSEUDO_HARD_REGNO (pseudo);
  ira_assert (hard_regno >= 0 && PSEUDO_COVER_CLASS (pseudo) != NO_REGS);
  i = class_hard_reg_index [PSEUDO_COVER_CLASS (pseudo)] [hard_regno];
  ira_assert (i >= 0);
  class = REGNO_REG_CLASS (hard_regno);
  mode = PSEUDO_MODE (pseudo);
  for (cp = PSEUDO_COPIES (pseudo); cp != NULL; cp = next_cp)
    {
      if (cp->first == pseudo)
	{
	  next_cp = cp->next_first_pseudo_copy;
	  another_pseudo = cp->second;
	}
      else if (cp->second == pseudo)
	{
	  next_cp = cp->next_second_pseudo_copy;
	  another_pseudo = cp->first;
	}
      else
	gcc_unreachable ();
      if (PSEUDO_COVER_CLASS (pseudo) != PSEUDO_COVER_CLASS (another_pseudo))
	continue;
      cost = (cp->second == pseudo
	      ? register_move_cost [mode] [class]
	        [PSEUDO_COVER_CLASS (another_pseudo)]
	      : register_move_cost [mode] [PSEUDO_COVER_CLASS (another_pseudo)]
	        [class]);
      if (decr_p)
	cost = -cost;
      PSEUDO_CURR_HARD_REG_COSTS (another_pseudo) [i] += cp->freq * cost;
      PSEUDO_CURR_CONFLICT_HARD_REG_COSTS (another_pseudo) [i]
	+= cp->freq * cost;
    }
}

/* Function choosing a hard register for PSEUDO.  If RETRY_P is
   nonzero, it means that the function called from
   `retry_ira_color'. */
static int
assign_hard_reg (pseudo_t pseudo, int retry_p)
{
  HARD_REG_SET conflicting_regs, biased_regs;
  int i, j, hard_regno, best_hard_regno, class_size;
  int cost, mem_cost, min_cost, full_cost, min_full_cost, add_cost, *costs;
  int *conflict_costs;
  enum reg_class cover_class, class;
  enum machine_mode mode;
  pseudo_t conflict_pseudo, conflict_pseudo2;
  pseudo_t *pseudo_vec, *pseudo_vec2;
  pseudo_t another_pseudo;
  struct pseudo_copy *cp, *next_cp;
  static int full_costs [FIRST_PSEUDO_REGISTER];

  ira_assert (! PSEUDO_ASSIGNED_P (pseudo));
  cover_class = PSEUDO_COVER_CLASS (pseudo);
  class_size = class_hard_regs_num [cover_class];
  CLEAR_HARD_REG_SET (biased_regs);
  memset (full_costs, 0, sizeof (int) * class_size);
  mem_cost = PSEUDO_MEMORY_COST (pseudo);
  mode = PSEUDO_MODE (pseudo);
  costs = PSEUDO_CURR_HARD_REG_COSTS (pseudo);
  memcpy (full_costs, costs, sizeof (int) * class_size);
  COPY_HARD_REG_SET (conflicting_regs, PSEUDO_CONFLICT_HARD_REGS (pseudo));
  pseudo_vec = PSEUDO_CONFLICT_PSEUDO_VEC (pseudo);
  for (i = 0; (conflict_pseudo = pseudo_vec [i]) != NULL; i++)
    /* Reload can give another class so we need to check all
       pseudos.  */
    if (retry_p
	|| (cover_class == PSEUDO_COVER_CLASS (conflict_pseudo)
	    && bitmap_bit_p (consideration_pseudo_bitmap,
			     PSEUDO_NUM (conflict_pseudo))))
      {
	if (PSEUDO_ASSIGNED_P (conflict_pseudo))
	  {
	    if ((hard_regno = PSEUDO_HARD_REGNO (conflict_pseudo)) >= 0)
	      IOR_HARD_REG_SET
		(conflicting_regs,
		 reg_mode_hard_regset
		 [hard_regno] [PSEUDO_MODE (conflict_pseudo)]);
	    continue;
	  }
	else
	  {
	    conflict_costs
	      = PSEUDO_CURR_CONFLICT_HARD_REG_COSTS (conflict_pseudo);
	    if (conflict_costs != NULL)
	      for (j = class_size - 1; j >= 0; j--)
		full_costs [j] -= conflict_costs [j];
	  }
	if (retry_p || ! flag_ira_biased_coloring)
	  continue;
	pseudo_vec2 = PSEUDO_CONFLICT_PSEUDO_VEC (conflict_pseudo);
	for (j = 0; (conflict_pseudo2 = pseudo_vec2 [j]) != NULL; j++)
	  if (cover_class == PSEUDO_COVER_CLASS (conflict_pseudo2)
	      && PSEUDO_ASSIGNED_P (conflict_pseudo2)
	      && (hard_regno = PSEUDO_HARD_REGNO (conflict_pseudo2)) >= 0
	      && (retry_p
		  || bitmap_bit_p (consideration_pseudo_bitmap,
				   PSEUDO_NUM (conflict_pseudo2))))
	    IOR_HARD_REG_SET (biased_regs,
			      reg_mode_hard_regset
			      [hard_regno] [PSEUDO_MODE (conflict_pseudo2)]);
      }
  for (cp = PSEUDO_COPIES (pseudo); cp != NULL; cp = next_cp)
    {
      if (cp->first == pseudo)
	{
	  next_cp = cp->next_first_pseudo_copy;
	  another_pseudo = cp->second;
	}
      else if (cp->second == pseudo)
	{
	  next_cp = cp->next_second_pseudo_copy;
	  another_pseudo = cp->first;
	}
      else
	gcc_unreachable ();
      if (PSEUDO_ASSIGNED_P (another_pseudo))
	continue;
      conflict_costs = PSEUDO_CURR_CONFLICT_HARD_REG_COSTS (another_pseudo);
      if (conflict_costs != NULL)
	for (j = class_size - 1; j >= 0; j--)
	  full_costs [j] += conflict_costs [j];
    }
  IOR_HARD_REG_SET (conflicting_regs, no_alloc_regs);
  IOR_COMPL_HARD_REG_SET (conflicting_regs, reg_class_contents [cover_class]);
  best_hard_regno = -1;
  GO_IF_HARD_REG_SUBSET (reg_class_contents [cover_class], conflicting_regs,
			 fail);
  min_cost = min_full_cost = INT_MAX;
  /* We don't care about giving callee saved registers to pseudos no
     living through calls because call used register are allocated
     first (it is usual practice to put them first in
     REG_ALLOC_ORDER).  */
  for (i = 0; i < class_size; i++)
    {
      hard_regno = class_hard_regs [cover_class] [i];
#ifdef STACK_REGS
      if (PSEUDO_NO_STACK_REG_P (pseudo)
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
  PSEUDO_HARD_REGNO (pseudo) = best_hard_regno;
  PSEUDO_ASSIGNED_P (pseudo) = TRUE;
  if (best_hard_regno >= 0)
    {
      allocated_hardreg_p [best_hard_regno] = TRUE;
      update_copy_costs (pseudo, TRUE);
    }
  return best_hard_regno >= 0;
}



/* This page contains allocator based on Chaitin algorithm.  */

/* Bucket of pseudos pseudo be colored currently without spilling.  */
static pseudo_t colorable_pseudo_bucket;

/* Bucket of pseudos pseudo might be not colored currently without
   spilling.  */
static pseudo_t uncolorable_pseudo_bucket;

/* Add PSEUDO to *BUCKET_PTR bucket.  PSEUDO should be not in a bucket
   before the call.  */
static void
add_pseudo_to_bucket (pseudo_t pseudo, pseudo_t *bucket_ptr)
{
  pseudo_t first_pseudo;

  first_pseudo = *bucket_ptr;
  PSEUDO_NEXT_BUCKET_PSEUDO (pseudo) = first_pseudo;
  PSEUDO_PREV_BUCKET_PSEUDO (pseudo) = NULL;
  if (first_pseudo != NULL)
    PSEUDO_PREV_BUCKET_PSEUDO (first_pseudo) = pseudo;
  *bucket_ptr = pseudo;
}

/* Add PSEUDO to *BUCKET_PTR bucket maintaining the order according
   their frequency.  PSEUDO should be not in a bucket before the
   call.  */
static void
add_pseudo_to_ordered_bucket (pseudo_t pseudo, pseudo_t *bucket_ptr)
{
  pseudo_t before, after;
  enum reg_class cover_class;
  int freq, nregs;

  freq = PSEUDO_FREQ (pseudo);
  cover_class = PSEUDO_COVER_CLASS (pseudo);
  nregs = reg_class_nregs [cover_class] [PSEUDO_MODE (pseudo)];
  for (before = *bucket_ptr, after = NULL;
       before != NULL;
       after = before, before = PSEUDO_NEXT_BUCKET_PSEUDO (before))
    if (PSEUDO_FREQ (before) > freq)
      break;
  PSEUDO_NEXT_BUCKET_PSEUDO (pseudo) = before;
  PSEUDO_PREV_BUCKET_PSEUDO (pseudo) = after;
  if (after == NULL)
    *bucket_ptr = pseudo;
  else
    PSEUDO_NEXT_BUCKET_PSEUDO (after) = pseudo;
  if (before != NULL)
    PSEUDO_PREV_BUCKET_PSEUDO (before) = pseudo;
}

/* Delete PSEUDO from *BUCKET_PTR bucket.  It should be there before
   the call.  */
static void
delete_pseudo_from_bucket (pseudo_t pseudo, pseudo_t *bucket_ptr)
{
  pseudo_t prev_pseudo, next_pseudo;

  prev_pseudo = PSEUDO_PREV_BUCKET_PSEUDO (pseudo);
  next_pseudo = PSEUDO_NEXT_BUCKET_PSEUDO (pseudo);
  if (prev_pseudo != NULL)
    PSEUDO_NEXT_BUCKET_PSEUDO (prev_pseudo) = next_pseudo;
  else
    {
      ira_assert (*bucket_ptr == pseudo);
      *bucket_ptr = next_pseudo;
    }
  if (next_pseudo != NULL)
    PSEUDO_PREV_BUCKET_PSEUDO (next_pseudo) = prev_pseudo;
}

/* Varray representing the stack of pseudos used during coloring.  */
static varray_type pseudo_stack_varray;

/* The function puts PSEUDO onto the coloring stack without removing
   it from the bucket.  Such action can result in moving conflicting
   pseudos from the uncolorable bucket to the colorable one.  */
static void
push_pseudo_to_stack (pseudo_t pseudo)
{
  int i, conflicts_num, conflict_size, size;
  pseudo_t conflict_pseudo;
  pseudo_t *pseudo_vec;
  enum reg_class cover_class;
  
  PSEUDO_IN_GRAPH_P (pseudo) = FALSE;
  VARRAY_PUSH_GENERIC_PTR (pseudo_stack_varray, pseudo);
  cover_class = PSEUDO_COVER_CLASS (pseudo);
  if (cover_class == NO_REGS)
    return;
  size = reg_class_nregs [cover_class] [PSEUDO_MODE (pseudo)];
  pseudo_vec = PSEUDO_CONFLICT_PSEUDO_VEC (pseudo);
  for (i = 0; (conflict_pseudo = pseudo_vec [i]) != NULL; i++)
    if (cover_class == PSEUDO_COVER_CLASS (conflict_pseudo)
	&& bitmap_bit_p (coloring_pseudo_bitmap, PSEUDO_NUM (conflict_pseudo)))
      {
	if (PSEUDO_IN_GRAPH_P (conflict_pseudo)
	    && ! PSEUDO_ASSIGNED_P (conflict_pseudo))
	  {
	    conflicts_num = PSEUDO_LEFT_CONFLICTS_NUM (conflict_pseudo);
	    conflict_size
	      = (reg_class_nregs
		 [cover_class] [PSEUDO_MODE (conflict_pseudo)]);
	    ira_assert
	      (PSEUDO_LEFT_CONFLICTS_NUM (conflict_pseudo) >= size);
	    PSEUDO_LEFT_CONFLICTS_NUM (conflict_pseudo) -= size;
	    if (conflicts_num + conflict_size
		<= PSEUDO_AVAILABLE_REGS_NUM (conflict_pseudo))
	      continue;
	    conflicts_num = PSEUDO_LEFT_CONFLICTS_NUM (conflict_pseudo);
	    if (conflicts_num + conflict_size
		<= PSEUDO_AVAILABLE_REGS_NUM (conflict_pseudo))
	      {
		delete_pseudo_from_bucket
		  (conflict_pseudo, &uncolorable_pseudo_bucket);
		add_pseudo_to_ordered_bucket (conflict_pseudo,
					      &colorable_pseudo_bucket);
	      }
	  }
      }
}

/* The function puts PSEUDO onto the coloring stack and removes it
   from the bucket.  The pseudo is in the colorable bucket if
   COLORABLE_P is nonzero.  */
static void
remove_pseudo_from_bucket_and_push (pseudo_t pseudo, int colorable_p)
{
  enum reg_class cover_class;
  pseudo_t *bucket_ptr;

  bucket_ptr = (colorable_p
		? &colorable_pseudo_bucket : &uncolorable_pseudo_bucket);
  delete_pseudo_from_bucket (pseudo, bucket_ptr);
  if (ira_dump_file != NULL)
    {
      fprintf (ira_dump_file, "  Pushing");
      print_expanded_pseudo (pseudo);
      fprintf (ira_dump_file, "%s\n", colorable_p ? "" : "(potential spill)");
    }
  cover_class = PSEUDO_COVER_CLASS (pseudo);
  ira_assert ((colorable_p
	       && (PSEUDO_LEFT_CONFLICTS_NUM (pseudo)
		   + reg_class_nregs [cover_class] [PSEUDO_MODE (pseudo)]
		   <= PSEUDO_AVAILABLE_REGS_NUM (pseudo)))
	      || (! colorable_p
		  && (PSEUDO_LEFT_CONFLICTS_NUM (pseudo)
		      + reg_class_nregs [cover_class] [PSEUDO_MODE (pseudo)]
		      > PSEUDO_AVAILABLE_REGS_NUM (pseudo))));
  if (! colorable_p)
    PSEUDO_MAY_BE_SPILLED_P (pseudo) = TRUE;
  push_pseudo_to_stack (pseudo);
}

/* The function puts all pseudos from colorable bucket onto the
   coloring stack.  */
static void
push_only_colorable (void)
{
  for (;colorable_pseudo_bucket != NULL;)
    remove_pseudo_from_bucket_and_push (colorable_pseudo_bucket, TRUE);
}

/* The function puts PSEUDO chosen for potential spilling onto the
   coloring stack.  */
static void
push_pseudo_to_spill (pseudo_t pseudo)
{
  delete_pseudo_from_bucket (pseudo, &uncolorable_pseudo_bucket);
  PSEUDO_MAY_BE_SPILLED_P (pseudo) = TRUE;
  if (ira_dump_file != NULL)
    fprintf (ira_dump_file, "  Pushing p%d(%d) (potential spill)\n",
	     PSEUDO_NUM (pseudo), PSEUDO_REGNO (pseudo));
  push_pseudo_to_stack (pseudo);
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
		|| (bitmap_bit_p (DF_UPWARD_LIVE_OUT (build_df, e->src), regno)
		    && bitmap_bit_p (DF_UPWARD_LIVE_IN (build_df, e->dest),
				     regno))))
	  freq += EDGE_FREQUENCY (e);
    }
  else
    {
      edges = get_loop_exit_edges (loop_node->loop);
      for (i = 0; VEC_iterate (edge, edges, i, e); i++)
	if (regno < 0
	    || (bitmap_bit_p (DF_UPWARD_LIVE_OUT (build_df, e->src), regno)
		&& bitmap_bit_p (DF_UPWARD_LIVE_IN (build_df, e->dest),
				 regno)))
	  freq += EDGE_FREQUENCY (e);
      VEC_free (edge, heap, edges);
    }

  return REG_FREQ_FROM_EDGE_FREQ (freq);
}

/* The function calculates and returns cost of putting pseudo P into
   memory.  */
static int
calculate_pseudo_spill_cost (pseudo_t p)
{
  int regno, cost;
  enum machine_mode mode;
  enum reg_class class;
  pseudo_t subloop_pseudo;
  struct ira_loop_tree_node *subloop_node;

  regno = PSEUDO_REGNO (p);
  cost = PSEUDO_MEMORY_COST (p) - PSEUDO_COVER_CLASS_COST (p);
  if (regno < 0)
    return cost;
  mode = PSEUDO_MODE (p);
  class = PSEUDO_COVER_CLASS (p);
  for (subloop_node = PSEUDO_LOOP_TREE_NODE (p)->inner;
       subloop_node != NULL;
       subloop_node = subloop_node->next)
    if (subloop_node->bb == NULL)
      {
	subloop_pseudo = subloop_node->regno_pseudo_map [regno];
	if (subloop_pseudo == NULL)
	  continue;
	if (PSEUDO_HARD_REGNO (subloop_pseudo) < 0)
	  cost -= (memory_move_cost [mode] [class] [0]
		   * loop_edge_freq (subloop_node, regno, TRUE)
		   + memory_move_cost [mode] [class] [1]
		   * loop_edge_freq (subloop_node, regno, FALSE));
	else
	  /* ??? move register to register.  How often do we have
	     different hard registers.  */
	  cost += 2 * (memory_move_cost [mode] [class] [1]
		   * loop_edge_freq (subloop_node, regno, FALSE)
		   + memory_move_cost [mode] [class] [0]
		   * loop_edge_freq (subloop_node, regno, TRUE));
      }
  return cost;
}

/* All pseudos sorted accoring their priorities.  */
static pseudo_t *sorted_pseudos;

/* Push pseudos on the coloring stack.  The order of pseudos in the
   stack defines the order for the subsequent coloring.  */
static void
push_pseudos_to_stack (void)
{
  int i, j;
  double pseudo_pri, i_pseudo_pri;
  pseudo_t pseudo, i_pseudo;
  pseudo_t *pseudo_vec;
  enum reg_class cover_class;
  int num, cover_class_pseudos_num [N_REG_CLASSES];
  pseudo_t *cover_class_pseudos [N_REG_CLASSES];

  /* Initialize.  */
  for (i = 0; i < reg_class_cover_size; i++)
    {
      cover_class = reg_class_cover [i];
      cover_class_pseudos_num [cover_class] = 0;
      cover_class_pseudos [cover_class] = NULL;
    }
  /* Calculate uncolorable pseudos of each cover class.  */
  for (pseudo = uncolorable_pseudo_bucket;
       pseudo != NULL;
       pseudo = PSEUDO_NEXT_BUCKET_PSEUDO (pseudo))
    if ((cover_class = PSEUDO_COVER_CLASS (pseudo)) != NO_REGS)
      {
	cover_class_pseudos_num [cover_class]++;
	PSEUDO_TEMP (pseudo) = INT_MAX;
      }
  /* Define place where to put uncolorable pseudos of the same cover
     class.  */
  for (num = i = 0; i < reg_class_cover_size; i++)
    {
      cover_class = reg_class_cover [i];
      if (cover_class_pseudos_num [cover_class] != 0)
	{
	  cover_class_pseudos [cover_class] = sorted_pseudos + num;
	  num += cover_class_pseudos_num [cover_class];
	  cover_class_pseudos_num [cover_class] = 0;
	}
    }
  ira_assert (num <= pseudos_num);
  /* Put uncolorable pseudos of the same cover class together.  */
  for (pseudo = uncolorable_pseudo_bucket;
       pseudo != NULL;
       pseudo = PSEUDO_NEXT_BUCKET_PSEUDO (pseudo))
    if ((cover_class = PSEUDO_COVER_CLASS (pseudo)) != NO_REGS)
      cover_class_pseudos
	[cover_class] [cover_class_pseudos_num [cover_class]++] = pseudo;
  for (;;)
    {
      push_only_colorable ();
      pseudo = uncolorable_pseudo_bucket;
      if (pseudo == NULL)
	break;
      cover_class = PSEUDO_COVER_CLASS (pseudo);
      if (cover_class == NO_REGS)
	{
	  push_pseudo_to_spill (pseudo);
	  continue;
	}
      /* Potential spilling.  */
      ira_assert (reg_class_nregs [cover_class] [PSEUDO_MODE (pseudo)] > 0);
      num = cover_class_pseudos_num [cover_class];
      ira_assert (num > 0);
      pseudo_vec = cover_class_pseudos [cover_class];
      pseudo = NULL;
      pseudo_pri = 0;
      /* Sort uncolorable pseudo to find the one with the lowest spill
	 cost.  */
      for (i = 0, j = num - 1; i <= j;)
	{
	  i_pseudo = pseudo_vec [i];
	  if (! PSEUDO_IN_GRAPH_P (i_pseudo)
	      && PSEUDO_IN_GRAPH_P (pseudo_vec [j]))
	    {
	      i_pseudo = pseudo_vec [j];
	      pseudo_vec [j] = pseudo_vec [i];
	      pseudo_vec [i] = i_pseudo;
	    }
	  if (PSEUDO_IN_GRAPH_P (i_pseudo))
	    {
	      i++;
	      if (PSEUDO_TEMP (i_pseudo) == INT_MAX)
		PSEUDO_TEMP (i_pseudo)
		  = calculate_pseudo_spill_cost (i_pseudo);
	      i_pseudo_pri
		= ((double) PSEUDO_TEMP (i_pseudo)
		   / (PSEUDO_LEFT_CONFLICTS_NUM (i_pseudo)
		      * reg_class_nregs [PSEUDO_COVER_CLASS (i_pseudo)]
		                        [PSEUDO_MODE (i_pseudo)] + 1));
	      if (pseudo == NULL || pseudo_pri > i_pseudo_pri
		  || (pseudo_pri == i_pseudo_pri
		      && (PSEUDO_NUM (pseudo) > PSEUDO_NUM (i_pseudo))))
		{
		  pseudo = i_pseudo;
		  pseudo_pri = i_pseudo_pri;
		}
	    }
	  if (! PSEUDO_IN_GRAPH_P (pseudo_vec [j]))
	    j--;
	}
      ira_assert (pseudo != NULL && j >= 0);
      cover_class_pseudos_num [cover_class] = j + 1;
      ira_assert (PSEUDO_IN_GRAPH_P (pseudo)
		  && PSEUDO_COVER_CLASS (pseudo) == cover_class
		  && (PSEUDO_LEFT_CONFLICTS_NUM (pseudo)
		      + reg_class_nregs [cover_class] [PSEUDO_MODE (pseudo)]
		      > PSEUDO_AVAILABLE_REGS_NUM (pseudo)));
      remove_pseudo_from_bucket_and_push (pseudo, FALSE);
    }
}

/* Assign hard registers to pseudos on the coloring stack.  */
static void
pop_pseudos_from_stack (void)
{
  pseudo_t pseudo;
  enum reg_class cover_class;

  for (;VARRAY_ACTIVE_SIZE (pseudo_stack_varray) != 0;)
    {
      pseudo = VARRAY_TOP_GENERIC_PTR (pseudo_stack_varray);
      VARRAY_POP (pseudo_stack_varray);
      cover_class = PSEUDO_COVER_CLASS (pseudo);
      if (ira_dump_file != NULL)
	{
	  fprintf (ira_dump_file, "  Popping");
	  print_expanded_pseudo (pseudo);
	  fprintf (ira_dump_file, "  -- ");
	}
      if (cover_class == NO_REGS)
	{
	  PSEUDO_HARD_REGNO (pseudo) = -1;
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file, "assign memory\n");
	}
      else if (assign_hard_reg (pseudo, FALSE))
	{
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file, "assign reg %d\n",
		     PSEUDO_HARD_REGNO (pseudo));
	}
      else
	{
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file, "spill\n");
	}
      PSEUDO_IN_GRAPH_P (pseudo) = TRUE;
    }
}

/* The function put PSEUDO in a bucket corresponding to its number and
   size of its conflicting pseudos and hard registers.  */
static void
put_pseudo_into_bucket (pseudo_t pseudo)
{
  int i, hard_regno, conflict_pseudos_size, hard_regs_num;
  pseudo_t conflict_pseudo;
  pseudo_t *pseudo_vec;
  enum reg_class cover_class;
  HARD_REG_SET temp_set;

  cover_class = PSEUDO_COVER_CLASS (pseudo);
  hard_regs_num = class_hard_regs_num [cover_class];
  if (hard_regs_num != 0)
    {
      memcpy (PSEUDO_CURR_HARD_REG_COSTS (pseudo),
	      PSEUDO_HARD_REG_COSTS (pseudo), sizeof (int) * hard_regs_num);
      memcpy (PSEUDO_CURR_CONFLICT_HARD_REG_COSTS (pseudo),
	      PSEUDO_CONFLICT_HARD_REG_COSTS (pseudo),
	      sizeof (int) * hard_regs_num);
    }
  COPY_HARD_REG_SET (temp_set, PSEUDO_CONFLICT_HARD_REGS (pseudo));
  AND_HARD_REG_SET (temp_set, reg_class_contents [cover_class]);
  AND_COMPL_HARD_REG_SET (temp_set, no_alloc_regs);
  conflict_pseudos_size = 0;
  GO_IF_HARD_REG_EQUAL (temp_set, zero_hard_reg_set, skip);
  for (i = 0; i < (int) hard_regs_num; i++)
    {
      hard_regno = class_hard_regs [cover_class] [i];
      if (TEST_HARD_REG_BIT (temp_set, hard_regno))
	{
	  conflict_pseudos_size++;
	  CLEAR_HARD_REG_BIT (temp_set, hard_regno);
	  GO_IF_HARD_REG_EQUAL (temp_set, zero_hard_reg_set, skip);
	}
    }
 skip:
  pseudo_vec = PSEUDO_CONFLICT_PSEUDO_VEC (pseudo);
  CLEAR_HARD_REG_SET (temp_set);
  if (cover_class != NO_REGS)
    for (i = 0; (conflict_pseudo = pseudo_vec [i]) != NULL; i++)
      if (cover_class == PSEUDO_COVER_CLASS (conflict_pseudo)
	  && bitmap_bit_p (consideration_pseudo_bitmap,
			   PSEUDO_NUM (conflict_pseudo)))
	{
	  if (! PSEUDO_ASSIGNED_P (conflict_pseudo))
	    conflict_pseudos_size
	      += reg_class_nregs [cover_class] [PSEUDO_MODE (conflict_pseudo)];
	  else if ((hard_regno = PSEUDO_HARD_REGNO (conflict_pseudo)) >= 0)
	    {
	      int last = (hard_regno
			  + hard_regno_nregs
                            [hard_regno] [PSEUDO_MODE (conflict_pseudo)]);

	      while (hard_regno < last)
		{
		  if (! TEST_HARD_REG_BIT (temp_set, hard_regno))
		    {
		      conflict_pseudos_size++;
		      SET_HARD_REG_BIT (temp_set, hard_regno);
		    }
		  hard_regno++;
		}
	    }
	}
  PSEUDO_IN_GRAPH_P (pseudo) = TRUE;
  PSEUDO_LEFT_CONFLICTS_NUM (pseudo) = conflict_pseudos_size;
  if (conflict_pseudos_size
      + reg_class_nregs [cover_class] [PSEUDO_MODE (pseudo)]
      <= PSEUDO_AVAILABLE_REGS_NUM (pseudo))
    add_pseudo_to_ordered_bucket (pseudo, &colorable_pseudo_bucket);
  else
    add_pseudo_to_bucket (pseudo, &uncolorable_pseudo_bucket);
}

/* Function implements Chaitin-Briggs coloring for pseudos in
   COLORING_PSEUDO_BITMAP taking into account pseudos in
   CONSIDERATION_PSEUDO_BITMAP.  */
static void
color_pseudos (void)
{
  unsigned int i;
  bitmap_iterator bi;

  /* Put the pseudos into the corresponding buckets.  */
  colorable_pseudo_bucket = NULL;
  uncolorable_pseudo_bucket = NULL;
  EXECUTE_IF_SET_IN_BITMAP (coloring_pseudo_bitmap, 0, i, bi)
    put_pseudo_into_bucket (pseudos [i]);
  push_pseudos_to_stack ();
  pop_pseudos_from_stack ();
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
	   loop_tree_node->loop->depth);
  EXECUTE_IF_SET_IN_BITMAP (loop_tree_node->mentioned_pseudos, 0, j, bi)
    fprintf (ira_dump_file, " %dr%d", j, PSEUDO_REGNO (pseudos [j]));
  fprintf (ira_dump_file, "\n    modified regnos:");
  EXECUTE_IF_SET_IN_BITMAP (loop_tree_node->modified_regnos, 0, j, bi)
    fprintf (ira_dump_file, " %d", j);
  fprintf (ira_dump_file, "\n    border:");
  EXECUTE_IF_SET_IN_BITMAP (loop_tree_node->border_pseudos, 0, j, bi)
    fprintf (ira_dump_file, " %dr%d", j, PSEUDO_REGNO (pseudos [j]));
  fprintf (ira_dump_file, "\n");
}

/* The function implements Chaitin-Briggs coloring for pseudos inside
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
  pseudo_t p, subloop_pseudo;
  struct ira_loop_tree_node *subloop_node;

  if (loop_tree_node->loop == NULL)
    return;
  if (ira_dump_file != NULL)
    print_loop_title (loop_tree_node);

  bitmap_copy (coloring_pseudo_bitmap, loop_tree_node->mentioned_pseudos);
  bitmap_ior_into (coloring_pseudo_bitmap, loop_tree_node->border_pseudos);
  bitmap_copy (consideration_pseudo_bitmap, coloring_pseudo_bitmap);
  EXECUTE_IF_SET_IN_BITMAP (consideration_pseudo_bitmap, 0, j, bi)
    {
      p = pseudos [j];
      if (! PSEUDO_ASSIGNED_P (p))
	continue;
      bitmap_clear_bit (coloring_pseudo_bitmap, PSEUDO_NUM (p));
    }
  /* Color all mentioned including transparent.  */
  color_pseudos ();
  /* Update costs for subloops.  */
  for (subloop_node = loop_tree_node->inner;
       subloop_node != NULL;
       subloop_node = subloop_node->next)
    if (subloop_node->bb == NULL)
      EXECUTE_IF_SET_IN_BITMAP (consideration_pseudo_bitmap, 0, j, bi)
        {
	  p = pseudos [j];
	  mode = PSEUDO_MODE (p);
	  class = PSEUDO_COVER_CLASS (p);
	  hard_regno = PSEUDO_HARD_REGNO (p);
	  if (hard_regno >= 0)
	    {
	      index = class_hard_reg_index [class] [hard_regno];
	      ira_assert (index >= 0);
	    }
	  regno = PSEUDO_REGNO (p);
	  /* ??? conflict costs */
	  if (regno >= 0)
	    {
	      subloop_pseudo = subloop_node->regno_pseudo_map [regno];
	      if (subloop_pseudo == NULL)
		continue;
	      exit_freq = loop_edge_freq (subloop_node, regno, TRUE);
	      enter_freq = loop_edge_freq (subloop_node, regno, FALSE);
	      if (reg_equiv_invariant_p [regno]
		  || reg_equiv_const [regno] != NULL_RTX)
		{
		  if (! PSEUDO_ASSIGNED_P (subloop_pseudo))
		    {
		      PSEUDO_HARD_REGNO (subloop_pseudo) = hard_regno;
		      PSEUDO_ASSIGNED_P (subloop_pseudo) = TRUE;
		      if (hard_regno >= 0)
			update_copy_costs (subloop_pseudo, TRUE);
		    }
		}
	      else if (hard_regno < 0)
		{
		  PSEUDO_MEMORY_COST (subloop_pseudo)
		    -= ((memory_move_cost [mode] [class] [1] * enter_freq)
			+ (memory_move_cost [mode] [class] [0] * exit_freq));
		}
	      else
		{
		  cost = (register_move_cost [mode] [class] [class] 
			  * (exit_freq + enter_freq));
		  PSEUDO_HARD_REG_COSTS (subloop_pseudo) [index] -= cost;
		  PSEUDO_CONFLICT_HARD_REG_COSTS (subloop_pseudo) [index]
		    -= cost;
		  PSEUDO_MEMORY_COST (subloop_pseudo)
		    += (memory_move_cost [mode] [class] [0] * enter_freq
			+ memory_move_cost [mode] [class] [1] * exit_freq);
		  if (PSEUDO_COVER_CLASS_COST (subloop_pseudo)
		      > PSEUDO_HARD_REG_COSTS (subloop_pseudo) [index])
		    PSEUDO_COVER_CLASS_COST (subloop_pseudo)
		      = PSEUDO_HARD_REG_COSTS (subloop_pseudo) [index];
		}
	    }
	  else
	    {
	      exit_freq = loop_edge_freq (subloop_node, -1, TRUE);
	      enter_freq = loop_edge_freq (subloop_node, -1, FALSE);
	      cost = (register_move_cost [mode] [class] [class] 
		      * (exit_freq + enter_freq));
	    }
	}
}

/* The function is used to sort pseudos according to their priorities
   which are calculated analogous to ones in file `global.c'.  */
static int
pseudo_priority_compare_func (const void *v1p, const void *v2p)
{
  pseudo_t p1 = *(const pseudo_t *) v1p, p2 = *(const pseudo_t *) v2p;
  int pri1, pri2;

  pri1 = (((double) (floor_log2 (REG_N_REFS (PSEUDO_REGNO (p1)))
		     * PSEUDO_FREQ (p1))
	   / REG_LIVE_LENGTH (PSEUDO_REGNO (p1)))
	  * (10000 / REG_FREQ_MAX) * PSEUDO_REGNO_SIZE (PSEUDO_REGNO (p1)));
  pri2 = (((double) (floor_log2 (REG_N_REFS (PSEUDO_REGNO (p2)))
		     * PSEUDO_FREQ (p2))
	   / REG_LIVE_LENGTH (PSEUDO_REGNO (p2)))
	  * (10000 / REG_FREQ_MAX) * PSEUDO_REGNO_SIZE (PSEUDO_REGNO (p2)));
  if (pri2 - pri1)
    return pri2 - pri1;

  /* If regs are equally good, sort by pseudos, so that the results of
     qsort leave nothing to chance.  */
  return PSEUDO_NUM (p1) - PSEUDO_NUM (p2);
}

/* The function implements Chow's prioity-based coloring.  */
static void
priority_coloring (void)
{
  int i, hard_regs_num;
  pseudo_t p;

  memcpy (sorted_pseudos, pseudos, pseudos_num * sizeof (pseudo_t));
  for (i = 0; i < pseudos_num; i++)
    {
      bitmap_set_bit (coloring_pseudo_bitmap, i);
      p = pseudos [i];
      hard_regs_num = class_hard_regs_num [PSEUDO_COVER_CLASS (p)];
      if (hard_regs_num == 0)
	continue;
      memcpy (PSEUDO_CURR_HARD_REG_COSTS (p),
	      PSEUDO_HARD_REG_COSTS (p), sizeof (int) * hard_regs_num);
      memcpy (PSEUDO_CURR_CONFLICT_HARD_REG_COSTS (p),
	      PSEUDO_CONFLICT_HARD_REG_COSTS (p),
	      sizeof (int) * hard_regs_num);
    }
  bitmap_copy (consideration_pseudo_bitmap, coloring_pseudo_bitmap);
  qsort (sorted_pseudos, pseudos_num, sizeof (pseudo_t),
	 pseudo_priority_compare_func);
  for (i = 0; i < pseudos_num; i++)
    {
      p = sorted_pseudos [i];
      if (ira_dump_file != NULL)
	{
	  fprintf (ira_dump_file, "  ");
	  print_expanded_pseudo (p);
	  fprintf (ira_dump_file, "  -- ");
	}
      if (assign_hard_reg (p, FALSE))
	{
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file, "assign reg %d\n",
		     PSEUDO_HARD_REGNO (p));
	}
      else
	{
	  if (ira_dump_file != NULL)
	    fprintf (ira_dump_file, "spill\n");
	}
      PSEUDO_IN_GRAPH_P (p) = TRUE;
    }
}

/* The function initialized common data for cloring and calls
   functions to do Chaitin-Briggs, regional, and Chow's priority-based
   coloring.  */
static void
do_coloring (void)
{
  coloring_pseudo_bitmap = ira_allocate_bitmap ();
  consideration_pseudo_bitmap = ira_allocate_bitmap ();

  if (flag_ira_algorithm == IRA_ALGORITHM_PRIORITY)
    priority_coloring ();
  else
    {
      if (ira_dump_file != NULL)
	fprintf (ira_dump_file, "\n**** Pseudos coloring:\n\n");
      
      traverse_loop_tree (ira_loop_tree_root, color_pass, NULL);
    }

  if (ira_dump_file != NULL)
    print_disposition (ira_dump_file);

  ira_free_bitmap (consideration_pseudo_bitmap);
  ira_free_bitmap (coloring_pseudo_bitmap);
}



/* The functions moves future spill/restore code to less frequent
   points (if it is profitable) by reassigning some pseudos to memory
   which means make longer live-range where the corresponding
   pseudo-registers will be in memory.  */
static void
move_spill_restore (void)
{
  int i, cost, changed_p, regno, hard_regno, index;
  enum machine_mode mode;
  enum reg_class class;
  pseudo_t p, father_pseudo, grandfather_pseudo;
  struct ira_loop_tree_node *father, *grandfather;

  for (;;)
    {
      changed_p = FALSE;
      if (ira_dump_file != NULL)
	fprintf (ira_dump_file, "New iteration of spill/restore move\n");
      for (i = 0; i < pseudos_num; i++)
	{
	  p = pseudos [i];
	  regno = PSEUDO_REGNO (p);
	  if (regno < 0
	      || PSEUDO_HARD_REGNO (p) >= 0
	      || (father = PSEUDO_LOOP_TREE_NODE (p)->father) == NULL)
	  continue;
	  father_pseudo = father->regno_pseudo_map [regno];
	  if (father_pseudo == NULL)
	    continue;
	  if ((hard_regno = PSEUDO_HARD_REGNO (father_pseudo)) < 0)
	    continue;
	  mode = PSEUDO_MODE (p);
	  class = PSEUDO_COVER_CLASS (father_pseudo);
	  index = class_hard_reg_index [class] [hard_regno];
	  ira_assert (index >= 0);
	  cost = (PSEUDO_ORIGINAL_MEMORY_COST (father_pseudo)
		  - PSEUDO_HARD_REG_COSTS (p) [index]);
	  cost -= (memory_move_cost [mode] [class] [0]
		   * loop_edge_freq (PSEUDO_LOOP_TREE_NODE (p), regno, FALSE)
		   + memory_move_cost [mode] [class] [1]
		   * loop_edge_freq (PSEUDO_LOOP_TREE_NODE (p), regno, TRUE));
	  if ((grandfather = father->father) != NULL)
	    {
	      grandfather_pseudo = father->regno_pseudo_map [regno];
	      if (grandfather_pseudo != NULL)
		{
		  if (PSEUDO_HARD_REGNO (grandfather_pseudo) < 0)
		    cost -= (memory_move_cost [mode] [class] [1]
			     * loop_edge_freq (father, regno, FALSE)
			     + memory_move_cost [mode] [class] [0]
			     * loop_edge_freq (father, regno, TRUE));
		  else
		    cost += (memory_move_cost [mode] [class] [0]
			     * loop_edge_freq (father, regno, FALSE)
			     + memory_move_cost [mode] [class] [1]
			     * loop_edge_freq (father, regno, TRUE));
		}
	    }
	  if (cost < 0)
	    {
	      PSEUDO_HARD_REGNO (father_pseudo) = -1;
	      changed_p = TRUE;
	    }
	}
      if (! changed_p)
	break;
    }
}



/* The function called from the reload to mark changes in the
   allocation of REGNO made by the reload.  */
void
mark_allocation_change (int regno)
{
  pseudo_t p = regno_pseudo_map [regno];
  int old_hard_regno, hard_regno, cost;
  enum reg_class cover_class = PSEUDO_COVER_CLASS (p);

  ira_assert (p != NULL);
  hard_regno = reg_renumber [regno];
  if ((old_hard_regno = PSEUDO_HARD_REGNO (p)) == hard_regno)
    return;
  if (old_hard_regno < 0)
    cost = -PSEUDO_MEMORY_COST (p);
  else
    {
      ira_assert (class_hard_reg_index [cover_class] [old_hard_regno] >= 0);
      cost = -(PSEUDO_HARD_REG_COSTS (p)
	       [class_hard_reg_index [cover_class] [old_hard_regno]]);
      update_copy_costs (p, FALSE);
    }
  overall_cost -= cost;
  PSEUDO_HARD_REGNO (p) = hard_regno;
  if (hard_regno < 0)
    cost += PSEUDO_MEMORY_COST (p);
  else if (class_hard_reg_index [cover_class] [hard_regno] >= 0)
    {
      cost += (PSEUDO_HARD_REG_COSTS (p)
	       [class_hard_reg_index [cover_class] [hard_regno]]);
      update_copy_costs (p, TRUE);
    }
  else
    /* Reload chages class of the pseudo.  */
    cost = 0;
  overall_cost += cost;
}

/* The function is analog of function `retry_global_alloc'.  It is
   called by reload to try to put spilled pseudo-register REGNO into a
   hard register which is not in FORBIDDEN_REGS.  */
void
retry_ira_color (int regno, HARD_REG_SET forbidden_regs)
{
  pseudo_t p;
  int hard_regno, hard_regs_num;
  enum reg_class cover_class;

  p = regno_pseudo_map [regno];
  mark_allocation_change (regno);
  ira_assert (reg_renumber [regno] < 0);
  if (ira_dump_file != NULL)
    fprintf (ira_dump_file, "spill %d(p%d), cost=%d", regno, PSEUDO_NUM (p),
	     PSEUDO_MEMORY_COST (p) - PSEUDO_COVER_CLASS_COST (p));
  IOR_HARD_REG_SET (PSEUDO_CONFLICT_HARD_REGS (p), forbidden_regs);
  PSEUDO_ASSIGNED_P (p) = FALSE;
  cover_class = PSEUDO_COVER_CLASS (p);
  hard_regs_num = class_hard_regs_num [cover_class];
  if (hard_regs_num != 0)
    {
      memcpy (PSEUDO_CURR_HARD_REG_COSTS (p), PSEUDO_HARD_REG_COSTS (p),
	      sizeof (int) * hard_regs_num);
      memcpy (PSEUDO_CURR_CONFLICT_HARD_REG_COSTS (p),
	      PSEUDO_CONFLICT_HARD_REG_COSTS (p),
	      sizeof (int) * hard_regs_num);
    }
  assign_hard_reg (p, TRUE);
  hard_regno = PSEUDO_HARD_REGNO (p);
  reg_renumber [regno] = hard_regno;
  if (hard_regno >= 0)
    {
      ira_assert (class_hard_reg_index [cover_class] [hard_regno] >= 0);
      overall_cost -= (PSEUDO_MEMORY_COST (p)
		       - (PSEUDO_HARD_REG_COSTS (p)
			  [class_hard_reg_index
			   [cover_class] [hard_regno]]));
      if (PSEUDO_CALLS_CROSSED_NUM (p) != 0
	  && ! hard_reg_not_in_set_p (hard_regno, PSEUDO_MODE (p),
				      call_used_reg_set))
	{
	  ira_assert (flag_caller_saves);
	  caller_save_needed = 1;
	}
    }

  /* If we found a register, modify the RTL for the register to show
     the hard register, and mark that register live.  */
  if (reg_renumber[regno] >= 0)
    {
      if (ira_dump_file != NULL)
	fprintf (ira_dump_file, ": reassign to %d", reg_renumber[regno]);
      REGNO (regno_reg_rtx[regno]) = reg_renumber[regno];
      mark_home_live (regno);
    }
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
  rtx x;
  bitmap_iterator bi;
  struct spilled_reg_stack_slot *slot;

  ira_assert (flag_ira && inherent_size == PSEUDO_REGNO_BYTES (regno)
	      && inherent_size <= total_size);
  x = NULL_RTX;
  if (flag_omit_frame_pointer)
    n = spilled_reg_stack_slots_num - 1;
  else
    n = 0;
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
	  if (pseudo_reg_conflict_p (regno, i))
	    goto cont;
	}
      
      SET_REGNO_REG_SET (&slot->spilled_regs, regno);
      x = slot->mem;
      break;
    cont:
      ;
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



/* Update avaliable hard registers for each pseudo.  */
static void
update_pseudo_avaialable_regs (void)
{
  int i, j, n, cost;
  enum reg_class cover_class;
  pseudo_t p;
  
  for (i = 0; i < pseudos_num; i++)
    {
      p = pseudos [i];
      cover_class = PSEUDO_COVER_CLASS (p);
      PSEUDO_AVAILABLE_REGS_NUM (p) = available_class_regs [cover_class];
      if (cover_class == NO_REGS)
	continue;
      cost = PSEUDO_MEMORY_COST (p);
      for (n = 0, j = class_hard_regs_num [cover_class] - 1; j >= 0; j--)
	if (TEST_HARD_REG_BIT (PSEUDO_CONFLICT_HARD_REGS (p),
			       class_hard_regs [cover_class] [j]))
	  n++;
      if (n > 0 && ira_dump_file != NULL)
	fprintf (ira_dump_file, "reg %d of %s has %d regs less\n",
		 PSEUDO_REGNO (p), reg_class_names [cover_class], n);
      PSEUDO_AVAILABLE_REGS_NUM (p) -= n;
    }
}

/* Entry function doing color-based register allocation.  */
void
ira_color (void)
{
  update_pseudo_avaialable_regs ();
  sorted_pseudos = ira_allocate (sizeof (pseudo_t) * pseudos_num);
  VARRAY_GENERIC_PTR_NOGC_INIT (pseudo_stack_varray, pseudos_num,
				"stack of pseudos");
  memset (allocated_hardreg_p, 0, sizeof (allocated_hardreg_p));
  do_coloring ();
  VARRAY_FREE (pseudo_stack_varray);
  ira_free (sorted_pseudos);
  move_spill_restore ();
}
