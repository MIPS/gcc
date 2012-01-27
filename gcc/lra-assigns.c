/* Assign reload pseudos.
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


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "hard-reg-set.h"
#include "rtl.h"
#include "tm_p.h"
#include "target.h"
#include "insn-config.h"
#include "recog.h"
#include "output.h"
#include "regs.h"
#include "function.h"
#include "expr.h"
#include "basic-block.h"
#include "except.h"
#include "df.h"
#include "ira.h"
#include "sparseset.h"
#include "lra-int.h"

/* Array representation of lra_get_allocno_class.  It is used to speed
   up the code.  */
static enum reg_class *regno_allocno_class_array;

/* Info about pseudo used during the assignment sub-pass.  Thread is a
   set of connected reload and inheritance pseudos with the same set
   of available hard reg set.  Thread is a pseudo itself for other
   cases.  */
struct regno_assign_info
{
  /* First/next pseudo of the same thread.  */
  int first, next;
  /* Frequency of the thread (frequency of the reload pseudos only in
     the thread when the thread contains a reload pseudo).  Defined
     only for the first thread pseudo.  */
  int freq;
};

/* Map regno to the corresponding assignment info.  */
static struct regno_assign_info *regno_assign_info;

/* Process a pseudo copy with frequency COPY_FREQ connecting REGNO1
   and REGNO2 to form threads.  */
static void
process_copy_to_form_thread (int regno1, int regno2, int copy_freq)
{
  int last, regno1_first, regno2_first;

  gcc_assert (regno1 >= lra_constraint_new_regno_start
	      && regno2 >= lra_constraint_new_regno_start);
  regno1_first = regno_assign_info[regno1].first;
  regno2_first = regno_assign_info[regno2].first;
  if (regno1_first != regno2_first)
    {
      for (last = regno2_first;
	   regno_assign_info[last].next >= 0;
	   last = regno_assign_info[last].next)
	regno_assign_info[last].first = regno1_first;
      regno_assign_info[last].next = regno_assign_info[regno1_first].next;
      regno_assign_info[regno1_first].first = regno2_first;
      regno_assign_info[regno1_first].freq
	+= regno_assign_info[regno2_first].freq;
    }
  regno_assign_info[regno1_first].freq -= 2 * copy_freq;
  gcc_assert (regno_assign_info[regno1_first].freq >= 0);
}

/* Initialize REGNO_ASSIGN_INFO and form threads.  */
static void
init_regno_assign_info (void)
{
  int i, regno1, regno2;
  lra_copy_t cp;

  regno_assign_info
    = (struct regno_assign_info *) xmalloc (sizeof (struct regno_assign_info)
				       * max_reg_num ());
  for (i = FIRST_PSEUDO_REGISTER; i < max_reg_num (); i++)
    {
      regno_assign_info[i].first = i;
      regno_assign_info[i].next = -1;
      regno_assign_info[i].freq = lra_reg_info[i].freq;
    }
  /* Form the threads.  */
  for (i = 0; (cp = lra_get_copy (i)) != NULL; i++)
    if ((regno1 = cp->regno1) >= lra_constraint_new_regno_start
	&& (regno2 = cp->regno2) >= lra_constraint_new_regno_start
	&& reg_renumber[regno1] < 0 && lra_reg_info[regno1].nrefs != 0
	&& reg_renumber[regno2] < 0 && lra_reg_info[regno2].nrefs != 0
	&& (ira_available_class_regs[regno_allocno_class_array[regno1]]
	    == ira_available_class_regs[regno_allocno_class_array[regno2]]))
      process_copy_to_form_thread (regno1, regno2, cp->freq);
}

/* Free REGNO_ASSIGN_INFO.  */
static void
finish_regno_assign_info (void)
{
  free (regno_assign_info);
}

/* The function is used to sort *reload* and *inheritance* pseudos to
   try to assign them hard registers.  We put pseudos from the same
   thread always nearby.  */
static int
reload_pseudo_compare_func (const void *v1p, const void *v2p)
{
  int r1 = *(const int *) v1p, r2 = *(const int *) v2p;
  enum reg_class cl1 = regno_allocno_class_array[r1];
  enum reg_class cl2 = regno_allocno_class_array[r2];
  int diff;
  
  gcc_assert (r1 >= lra_constraint_new_regno_start
	      && r2 >= lra_constraint_new_regno_start);
  
  /* Prefer to assign reload registers with smaller classes first to
     guarantee assignment to all reload registers.  */
  if ((diff = (ira_available_class_regs[cl1]
	       - ira_available_class_regs[cl2])) != 0)
    return diff;
  if ((diff = (regno_assign_info[regno_assign_info[r2].first].freq
	       - regno_assign_info[regno_assign_info[r1].first].freq)) != 0)
    return diff;
  /* Put pseudos from the thread nearby.  */
  if ((diff = regno_assign_info[r1].first - regno_assign_info[r2].first) != 0)
    return diff;
  /* If regs are equally good, sort by their numbers, so that the
     results of qsort leave nothing to chance.  */
  return r1 - r2;
}

/* The function is used to sort *non-reload* pseudos to try to assign
   them hard registers.  The order calculation is simpler than in the
   previous function and based on the pseudo frequency usage.  */
static int
pseudo_compare_func (const void *v1p, const void *v2p)
{
  int r1 = *(const int *) v1p, r2 = *(const int *) v2p;
  int diff;

  /* Prefer to assign more frequently used registers first.  */
  if ((diff = lra_reg_info[r2].freq - lra_reg_info[r1].freq) != 0)
    return diff;
  
  /* If regs are equally good, sort by their numbers, so that the
     results of qsort leave nothing to chance.  */
  return r1 - r2;
}

/* Map: program point -> bitmap of all pseudos living at the point and
   assigned to hard registers.  */
static bitmap_head *live_hard_reg_pseudos;

/* reg_renumber corresponding to live_hard_reg_pseudos.  reg_renumber
   might differ from information in live_hard_reg_pseudos but
   live_pseudos_reg_renumber always reflects the info.  */
static int *live_pseudos_reg_renumber;

/* Sparseset used to calculate living hard reg pseudos for some program
   point range.  */
static sparseset live_range_hard_reg_pseudos;

/* Sparseset used to calculate living reload pseudos for some program
   point range.  */
static sparseset live_range_reload_pseudos;

/* Allocate and initialize the data about living pseudos at program
   points.  */
static void
init_lives (void)
{
  int i;

  live_range_hard_reg_pseudos = sparseset_alloc (max_reg_num ());
  live_range_reload_pseudos = sparseset_alloc (max_reg_num ());
  live_hard_reg_pseudos = (bitmap_head *) xmalloc (sizeof (bitmap_head)
						   * lra_live_max_point);
  for (i = 0; i < lra_live_max_point; i++)
    bitmap_initialize (&live_hard_reg_pseudos[i], &reg_obstack);
  live_pseudos_reg_renumber
    = (int *) xmalloc (sizeof (int) * max_reg_num ());
  for (i = 0; i < max_reg_num (); i++)
    live_pseudos_reg_renumber[i] = -1;
}

/* Free the data about living pseudos at program points.  */
static void
finish_lives (void)
{
  int i;

  sparseset_free (live_range_hard_reg_pseudos);
  sparseset_free (live_range_reload_pseudos);
  for (i = 0; i < lra_live_max_point; i++)
    bitmap_clear (&live_hard_reg_pseudos[i]);
  free (live_hard_reg_pseudos);
  free (live_pseudos_reg_renumber);
}

/* Update LIVE_HARD_REG_PSEUDOS and LIVE_PSEUDOS_REG_RENUMBER by
   pseudo REGNO assigment or by the pseudo spilling if FREE_P.  */
static void
update_lives (int regno, bool free_p)
{
  int p;
  lra_live_range_t r;

  if (reg_renumber[regno] < 0)
    return;
  live_pseudos_reg_renumber[regno] = free_p ? -1 : reg_renumber[regno];
  for (r = lra_reg_info[regno].live_ranges; r != NULL; r = r->next)
    {
      for (p = r->start; p <= r->finish; p++)
	if (free_p)
	  bitmap_clear_bit (&live_hard_reg_pseudos[p], regno);
	else
	  bitmap_set_bit (&live_hard_reg_pseudos[p], regno);
    }
}

/* Sparseset used to calculate reload pseudos conflicting with a given
   pseudo when we are trying to find a hard register for the given
   pseudo.  */
static sparseset conflict_reload_and_inheritance_pseudos;

/* Map: program point -> bitmap of all reload and inheritance pseudos
   living at the point.  */
static bitmap_head *live_reload_and_inheritance_pseudos;

/* Allocate and initialize data about living reload pseudos at any
   given program point.  */
static void
init_live_reload_and_inheritance_pseudos (void)
{
  int i, p;
  lra_live_range_t r;
  
  conflict_reload_and_inheritance_pseudos = sparseset_alloc (max_reg_num ());
  live_reload_and_inheritance_pseudos
    = (bitmap_head *) xmalloc (sizeof (bitmap_head) * lra_live_max_point);
  for (p = 0; p < lra_live_max_point; p++)
    bitmap_initialize (&live_reload_and_inheritance_pseudos[p], &reg_obstack);
  for (i = lra_constraint_new_regno_start; i < max_reg_num (); i++)
    for (r = lra_reg_info[i].live_ranges; r != NULL; r = r->next)
      for (p = r->start; p <= r->finish; p++)
	bitmap_set_bit (&live_reload_and_inheritance_pseudos[p], i);
}

/* Finalize data about living reload pseudos at any given program
   point.  */
static void
finish_live_reload_and_inheritance_pseudos (void)
{
  int p;

  sparseset_free (conflict_reload_and_inheritance_pseudos);
  for (p = 0; p < lra_live_max_point; p++)
    bitmap_clear (&live_reload_and_inheritance_pseudos[p]);
  free (live_reload_and_inheritance_pseudos);
}

/* The value used to check that cost of given hard reg is defined
   currently.  */
static int curr_hard_regno_costs_check = 0;
/* Array used to check that cost of the corresponding hard reg (the
   array element index) is defined currently.  */
static int hard_regno_costs_check[FIRST_PSEUDO_REGISTER];
/* The current costs of allocation of hard regs.  Defined only if the
   value of the corresponding element of the previous array is equal to
   CURR_HARD_REGNO_COSTS_CHECK.  */
static int hard_regno_costs[FIRST_PSEUDO_REGISTER];

/* Find and return best (or TRY_ONLY_HARD_REGNO) free hard register
   for pseudo REGNO.  In failure case, return a negative number.
   Return through *COST the cost of usage of the hard register for the
   pseudo.  Best free hard register has smallest cost of usage for
   REGNO or smallest register bank if the cost is the same.  */
static int
find_hard_regno_for (int regno, int *cost, int try_only_hard_regno)
{
  HARD_REG_SET conflict_set;
  int best_cost = INT_MAX, best_bank = INT_MAX, best_usage = INT_MAX;
  lra_live_range_t r;
  int p, i, j, rclass_size, best_hard_regno, bank, hard_regno;
  unsigned int k, conflict_regno;
  int val;
  enum reg_class rclass;
  bitmap_iterator bi;
  bool *rclass_intersect_p;

  COPY_HARD_REG_SET (conflict_set, lra_no_alloc_regs);
  rclass = regno_allocno_class_array[regno];
  rclass_intersect_p = ira_reg_classes_intersect_p[rclass];
  curr_hard_regno_costs_check++;
  sparseset_clear (conflict_reload_and_inheritance_pseudos);
  sparseset_clear (live_range_hard_reg_pseudos);
  IOR_HARD_REG_SET (conflict_set, lra_reg_info[regno].conflict_hard_regs);
  for (r = lra_reg_info[regno].live_ranges; r != NULL; r = r->next)
    {
      EXECUTE_IF_SET_IN_BITMAP (&live_hard_reg_pseudos[r->start], 0, k, bi)
	if (rclass_intersect_p[regno_allocno_class_array[k]])
	  sparseset_set_bit (live_range_hard_reg_pseudos, k);
      EXECUTE_IF_SET_IN_BITMAP (&live_reload_and_inheritance_pseudos[r->start],
				0, k, bi)
	if (lra_reg_info[k].preferred_hard_regno1 >= 0
	    && live_pseudos_reg_renumber[k] < 0
	    && rclass_intersect_p[regno_allocno_class_array[k]])
	  sparseset_set_bit (conflict_reload_and_inheritance_pseudos, k);
      for (p = r->start + 1; p <= r->finish; p++)
	{
	  lra_live_range_t r2;
	  
	  for (r2 = lra_start_point_ranges[p]; r2 != NULL; r2 = r2->start_next)
	    {
	      if (r2->regno >= lra_constraint_new_regno_start
		  && lra_reg_info[r2->regno].preferred_hard_regno1 >= 0
		  && live_pseudos_reg_renumber[r2->regno] < 0
		  && rclass_intersect_p[regno_allocno_class_array[r2->regno]])
		sparseset_set_bit (conflict_reload_and_inheritance_pseudos,
				   r2->regno);
	      if (live_pseudos_reg_renumber[r2->regno] >= 0
		  && rclass_intersect_p[regno_allocno_class_array[r2->regno]])
		sparseset_set_bit (live_range_hard_reg_pseudos, r2->regno);
	    }
	}
    }
  if ((hard_regno = lra_reg_info[regno].preferred_hard_regno1) >= 0)
    {
      if (hard_regno_costs_check[hard_regno] != curr_hard_regno_costs_check)
	hard_regno_costs[hard_regno] = 0;
      hard_regno_costs_check[hard_regno] = curr_hard_regno_costs_check;
      hard_regno_costs[hard_regno]
	-= lra_reg_info[regno].preferred_hard_regno_profit1;
      if ((hard_regno = lra_reg_info[regno].preferred_hard_regno2) >= 0)
	{
	  if (hard_regno_costs_check[hard_regno]
	      != curr_hard_regno_costs_check)
	    hard_regno_costs[hard_regno] = 0;
	  hard_regno_costs_check[hard_regno] = curr_hard_regno_costs_check;
	  hard_regno_costs[hard_regno]
	    -= lra_reg_info[regno].preferred_hard_regno_profit2;
	}
    }
#ifdef STACK_REGS
  if (lra_reg_info[regno].no_stack_p)
    for (i = FIRST_STACK_REG; i <= LAST_STACK_REG; i++)
      SET_HARD_REG_BIT (conflict_set, i);
#endif
  sparseset_clear_bit (conflict_reload_and_inheritance_pseudos, regno);
  val = lra_reg_info[regno].val;
  EXECUTE_IF_SET_IN_SPARSESET (live_range_hard_reg_pseudos, conflict_regno)
    if (val != lra_reg_info[conflict_regno].val)
      {
	lra_add_hard_reg_set (live_pseudos_reg_renumber[conflict_regno],
			      lra_reg_info[conflict_regno].biggest_mode,
			      &conflict_set);
	if (hard_reg_set_subset_p (reg_class_contents[rclass],
				   conflict_set))
	  return -1;
      }
  EXECUTE_IF_SET_IN_SPARSESET (conflict_reload_and_inheritance_pseudos,
			       conflict_regno)
    if (val != lra_reg_info[conflict_regno].val)
      {
	gcc_assert (live_pseudos_reg_renumber[conflict_regno] < 0);
	if ((hard_regno
	     = lra_reg_info[conflict_regno].preferred_hard_regno1) >= 0)
	  {
	    if (hard_regno_costs_check[hard_regno]
		!= curr_hard_regno_costs_check)
	      hard_regno_costs[hard_regno] = 0;
	    hard_regno_costs_check[hard_regno] = curr_hard_regno_costs_check;
	    hard_regno_costs[hard_regno]
	      += lra_reg_info[conflict_regno].preferred_hard_regno_profit1;
	    if ((hard_regno
		 = lra_reg_info[conflict_regno].preferred_hard_regno2) >= 0)
	      {
		if (hard_regno_costs_check[hard_regno]
		    != curr_hard_regno_costs_check)
		  hard_regno_costs[hard_regno] = 0;
		hard_regno_costs_check[hard_regno]
		  = curr_hard_regno_costs_check;
		hard_regno_costs[hard_regno]
		  += lra_reg_info[conflict_regno].preferred_hard_regno_profit2;
	      }
	  }
      }
  /* That is important for allocation of multi-word pseudos.  */
  IOR_COMPL_HARD_REG_SET (conflict_set, reg_class_contents[rclass]);
  /* ?!? may be cover class for the old.  */
  gcc_assert (rclass != NO_REGS);
  rclass_size = ira_class_hard_regs_num[rclass];
  best_hard_regno = -1;
  for (i = 0; i < rclass_size; i++)
    {
      if (try_only_hard_regno >= 0)
	hard_regno = try_only_hard_regno;
      else
	hard_regno = ira_class_hard_regs[rclass][i];
      if (! lra_hard_reg_set_intersection_p (hard_regno,
					     PSEUDO_REGNO_MODE (regno),
					     conflict_set)
	  /* We can not use prohibited_class_mode_regs because it is
	     defined not for all classes.  */
	  && HARD_REGNO_MODE_OK (hard_regno, PSEUDO_REGNO_MODE (regno)))
	{
	  if (hard_regno_costs_check[hard_regno]
	      != curr_hard_regno_costs_check)
	    {
	      hard_regno_costs_check[hard_regno] = curr_hard_regno_costs_check;
	      hard_regno_costs[hard_regno] = 0;
	    }
	  for (j = 0;
	       j < hard_regno_nregs[hard_regno][PSEUDO_REGNO_MODE (regno)];
	       j++)
	    if (! TEST_HARD_REG_BIT (call_used_reg_set, hard_regno + j)
		&& ! df_regs_ever_live_p (hard_regno + j))
	      /* It needs save restore.  */
	      hard_regno_costs[hard_regno]
		+= 2 * ENTRY_BLOCK_PTR->next_bb->frequency;
	  bank = targetm.register_bank (hard_regno);
	  if (best_hard_regno < 0 || hard_regno_costs[hard_regno] < best_cost
	      || (hard_regno_costs[hard_regno] == best_cost
		  && (bank < best_bank
		      /* Hard register usage leveling actually results
			 in bigger code for targets with conditional
			 execution like ARM because it reduces chance
			 of if-conversion after LRA.  */
		      || (! targetm.have_conditional_execution ()
			  && bank == best_bank
			  && best_usage > lra_hard_reg_usage[hard_regno]))))
	    {
	      best_hard_regno = hard_regno;
	      best_cost = hard_regno_costs[hard_regno];
	      best_bank = bank;
	      best_usage = lra_hard_reg_usage[hard_regno];
	    }
	}
      if (try_only_hard_regno >= 0)
	break;
    }
  if (best_hard_regno >= 0)
    *cost = best_cost - lra_reg_info[regno].freq;
  return best_hard_regno;
}

/* Current value used for checking elements in
   update_hard_regno_preference_check.  */
static int curr_update_hard_regno_preference_check;
/* If an element value is equal to the above variable value, then the
   corresponding regno has been processed for preference
   propagation.  */
static int *update_hard_regno_preference_check;

/* Update HARD_REGNO preference for pseudos connected (directly or
   indirectly) to a pseudo with REGNO.  Use divisor DIV to the
   corresponding copy frequency for the hard regno cost preference
   calculation.  The more indirectly a pseudo connected, the less the
   cost preference.  It is achieved by increasing the divisor for each
   next recursive level move.  */
static void
update_hard_regno_preference (int regno, int hard_regno, int div)
{
  int another_regno, cost;
  lra_copy_t cp, next_cp;

  /* Search depth 5 seems to be enough.  */
  if (div > (1 << 5))
    return;
  for (cp = lra_reg_info[regno].copies; cp != NULL; cp = next_cp)
    {
      if (cp->regno1 == regno)
	{
	  next_cp = cp->regno1_next;
	  another_regno = cp->regno2;
	}
      else if (cp->regno2 == regno)
	{
	  next_cp = cp->regno2_next;
	  another_regno = cp->regno1;
	}
      else
	gcc_unreachable ();
      if (reg_renumber[another_regno] < 0
	  && (update_hard_regno_preference_check[another_regno]
	      != curr_update_hard_regno_preference_check))
	{
	  update_hard_regno_preference_check[another_regno]
	    = curr_update_hard_regno_preference_check;
	  cost = cp->freq < div ? 1 : cp->freq / div;
	  lra_setup_reload_pseudo_preferenced_hard_reg
	    (another_regno, hard_regno, cost);
	  update_hard_regno_preference (another_regno, hard_regno, div * 2);
	}
    }
}

/* Update REG_RENUMBER and other pseudo preferences by assignment of
   HARD_REGNO to pseudo REGNO and print about it if PRINT_P.  */
void
lra_setup_reg_renumber (int regno, int hard_regno, bool print_p)
{
  int i, hr;

  if ((hr = hard_regno) < 0)
    hr = reg_renumber[regno];
  reg_renumber[regno] = hard_regno;
  gcc_assert (hr >= 0);
  for (i = 0; i < hard_regno_nregs[hr][PSEUDO_REGNO_MODE (regno)]; i++)
    if (hard_regno < 0)
      lra_hard_reg_usage[hr + i] -= lra_reg_info[regno].freq;
    else
      lra_hard_reg_usage[hr + i] += lra_reg_info[regno].freq;
  if (print_p && lra_dump_file != NULL)
    fprintf (lra_dump_file, "      Assign %d to %sr%d (freq=%d)\n",
	     reg_renumber[regno],
	     regno < lra_constraint_new_regno_start
	     ? ""
	     : bitmap_bit_p (&lra_inheritance_pseudos, regno) ? "inheritance "
	     : bitmap_bit_p (&lra_split_pseudos, regno) ? "split " : "reload ",
	     regno, lra_reg_info[regno].freq);
  if (hard_regno >= 0)
    {
      curr_update_hard_regno_preference_check++;
      update_hard_regno_preference (regno, hard_regno, 1);
    }
}

/* Pseudos which should be ignored for spilling for a particular
   pseudo. */
static bitmap_head ignore_pseudos_bitmap;

/* Bitmaps used to contain spill pseudos for given pseudo hard regno
   and best spill pseudos for given pseudo (and best hard regno).  */
static bitmap_head spill_pseudos_bitmap, best_spill_pseudos_bitmap;

/* Current pseudo check for validity of elements in
   TRY_HARD_REG_PSEUDOS.  */
static int curr_pseudo_check;
/* Array used for validity of elements in TRY_HARD_REG_PSEUDOS.  */
static int try_hard_reg_pseudos_check[FIRST_PSEUDO_REGISTER];
/* Pseudos who hold given hard register at corresponding points.  */
static bitmap_head try_hard_reg_pseudos[FIRST_PSEUDO_REGISTER];

/* Set up try_hard_reg_pseudos for given program point P and class
   RCLASS.  Those are pseudos living at P and assigned to a hard
   register of RCLASS.  In other words, those are pseudos which can be
   spilled to assign a hard register of RCLASS to a pseudo living at
   P.  */
static void
setup_try_hard_regno_pseudos (int p, enum reg_class rclass)
{
  int i, hard_regno;
  enum machine_mode mode;
  unsigned int spill_regno;
  bitmap_iterator bi;

  /* Find what pseudos could be spilled.  */
  EXECUTE_IF_SET_IN_BITMAP (&live_hard_reg_pseudos[p], 0, spill_regno, bi)
    {
      mode = PSEUDO_REGNO_MODE (spill_regno);
      if (lra_hard_reg_set_intersection_p (live_pseudos_reg_renumber[spill_regno], mode,
					   reg_class_contents[rclass]))
	{
	  hard_regno = live_pseudos_reg_renumber[spill_regno];
	  for (i = hard_regno_nregs[hard_regno][mode] - 1; i >= 0; i--)
	    {
	      if (try_hard_reg_pseudos_check[hard_regno + i]
		  != curr_pseudo_check)
		{
		  try_hard_reg_pseudos_check[hard_regno + i]
		    = curr_pseudo_check;
		  bitmap_clear (&try_hard_reg_pseudos[hard_regno + i]);
		}
	      bitmap_set_bit (&try_hard_reg_pseudos[hard_regno + i],
			      spill_regno);
	    }
	}
    }
}

/* Assign temporarily HARD_REGNO to pseudo REGNO.  Temporary
   assignment means that we might undo the data change.  */
static void
assign_temporarily (int regno, int hard_regno)
{
  int p;
  lra_live_range_t r;

  for (r = lra_reg_info[regno].live_ranges; r != NULL; r = r->next)
    {
      for (p = r->start; p <= r->finish; p++)
	if (hard_regno < 0)
	  bitmap_clear_bit (&live_hard_reg_pseudos[p], regno);
	else
	  bitmap_set_bit (&live_hard_reg_pseudos[p], regno);
    }
  live_pseudos_reg_renumber[regno] = hard_regno;
}

/* Array used for sorting reload pseudos for subsequent allocation
   after some spilling a pseudo.  */
static int *sorted_reload_pseudos;

/* Spill for a reload pseudo REGNO and return hard register which
   should be used for pseudo after spilling.  The function adds
   spilled pseudos to SPILLED_PSEUDO_BITMAP.  When we choose hard
   register (and pseudos occupying the hard registers and to be
   spilled), we take into account not only how REGNO will from the
   spills but also how other reload pseudos not assigned to hard
   registers yet benefit from the spills too.  */
static int
spill_for (int regno, bitmap spilled_pseudo_bitmap)
{
  int i, j, n, p, hard_regno, best_hard_regno, cost, best_cost, rclass_size;
  int reload_hard_regno, reload_cost;
  enum machine_mode mode, mode2;
  enum reg_class rclass;
  HARD_REG_SET spilled_hard_regs;
  unsigned int k, spill_regno, reload_regno, uid;
  int insn_pseudos_num, best_insn_pseudos_num;
  lra_live_range_t r;
  bitmap_iterator bi, bi2;

  rclass = regno_allocno_class_array[regno];
  gcc_assert (reg_renumber[regno] < 0 && rclass != NO_REGS);
  bitmap_clear (&ignore_pseudos_bitmap);
  bitmap_clear (&best_spill_pseudos_bitmap);
  EXECUTE_IF_SET_IN_BITMAP (&lra_reg_info[regno].insn_bitmap, 0, uid, bi)
    {
      struct lra_insn_reg *ir;
      
      for (ir = lra_get_insn_regs (uid); ir != NULL; ir = ir->next)
	if (ir->regno >= FIRST_PSEUDO_REGISTER)
	  bitmap_set_bit (&ignore_pseudos_bitmap, ir->regno);
    }
  best_hard_regno = -1;
  best_cost = INT_MAX;
  best_insn_pseudos_num = INT_MAX;
  rclass_size = ira_class_hard_regs_num[rclass];
  mode = PSEUDO_REGNO_MODE (regno);
  curr_pseudo_check++; /* Invalidate try_hard_reg_pseudos elements.  */
  for (r = lra_reg_info[regno].live_ranges; r != NULL; r = r->next)
    for (p = r->start; p <= r->finish; p++)
      setup_try_hard_regno_pseudos (p, rclass);
  for (i = 0; i < rclass_size; i++)
    {
      hard_regno = ira_class_hard_regs[rclass][i];
      bitmap_clear (&spill_pseudos_bitmap);
      for (j = hard_regno_nregs[hard_regno][mode] - 1; j >= 0; j--)
	{
	  if (try_hard_reg_pseudos_check[hard_regno + j] != curr_pseudo_check)
	    continue;
	  gcc_assert (! bitmap_empty_p (&try_hard_reg_pseudos[hard_regno + j]));
	  bitmap_ior_into (&spill_pseudos_bitmap,
			   &try_hard_reg_pseudos[hard_regno + j]);
	}
      /* Spill pseudos.  */
      CLEAR_HARD_REG_SET (spilled_hard_regs);
      EXECUTE_IF_SET_IN_BITMAP (&spill_pseudos_bitmap, 0, spill_regno, bi)
	if ((int) spill_regno >= lra_constraint_new_regno_start
	    /* ??? */
	    && ! bitmap_bit_p (&lra_inheritance_pseudos, spill_regno)
	    && ! bitmap_bit_p (&lra_split_pseudos, spill_regno))
	  goto fail;
      insn_pseudos_num = 0;
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "        Trying %d:", hard_regno);
      sparseset_clear (live_range_reload_pseudos);
      EXECUTE_IF_SET_IN_BITMAP (&spill_pseudos_bitmap, 0, spill_regno, bi)
	{
	  if (bitmap_bit_p (&ignore_pseudos_bitmap, spill_regno))
	    insn_pseudos_num++;
	  mode2 = PSEUDO_REGNO_MODE (spill_regno);
	  update_lives (spill_regno, true);
	  if (lra_dump_file != NULL)
	    fprintf (lra_dump_file, " spill %d(freq=%d)",
		     spill_regno, lra_reg_info[spill_regno].freq);
	  lra_add_hard_reg_set (reg_renumber[spill_regno], mode2,
				&spilled_hard_regs);
	  for (r = lra_reg_info[spill_regno].live_ranges; r != NULL; r = r->next)
	    {
	      EXECUTE_IF_SET_IN_BITMAP (&live_hard_reg_pseudos[r->start], 0, k, bi2)
		sparseset_set_bit (live_range_hard_reg_pseudos, k);
	      for (p = r->start + 1; p <= r->finish; p++)
		{
		  lra_live_range_t r2;
		  
		  for (r2 = lra_start_point_ranges[p]; r2 != NULL; r2 = r2->start_next)
		    if (r2->regno >= lra_constraint_new_regno_start)
		      sparseset_set_bit (live_range_reload_pseudos, r2->regno);
		}
	    }
	}
      /* We are trying to spill reload pseudo.  That is wrong we
	 should assign all reload pseudos, otherwise we cannot reuse
	 selected alternativies.  */
      hard_regno = find_hard_regno_for (regno, &cost, -1);
      if (hard_regno >= 0)
	{
	  assign_temporarily (regno, hard_regno);
	  n = 0;
	  EXECUTE_IF_SET_IN_SPARSESET (live_range_reload_pseudos, reload_regno)
	    if (live_pseudos_reg_renumber[reload_regno] < 0
		&& (hard_reg_set_intersect_p
		    (reg_class_contents[regno_allocno_class_array[reload_regno]],
		     spilled_hard_regs)))
	      sorted_reload_pseudos[n++] = reload_regno;
	  qsort (sorted_reload_pseudos, n, sizeof (int),
		 reload_pseudo_compare_func);
	  for (j = 0; j < n; j++)
	    {
	      reload_regno = sorted_reload_pseudos[j];
	      if (live_pseudos_reg_renumber[reload_regno] < 0
		  && (reload_hard_regno
		      = find_hard_regno_for (reload_regno,
					     &reload_cost, -1)) >= 0
		  && (lra_hard_reg_set_intersection_p
		      (reload_hard_regno, PSEUDO_REGNO_MODE (reload_regno),
		       spilled_hard_regs)))
		{
		  if (lra_dump_file != NULL)
		    fprintf (lra_dump_file, " assign %d(cost=%d)",
			     reload_regno, reload_cost);
		  assign_temporarily (reload_regno, reload_hard_regno);
		  cost += reload_cost;
		}
	    }
	  EXECUTE_IF_SET_IN_BITMAP (&spill_pseudos_bitmap, 0, spill_regno, bi)
	    {
	      rtx x;
	      
	      cost += lra_reg_info[spill_regno].freq;
	      if (ira_reg_equiv[spill_regno].memory != NULL
		  || ira_reg_equiv[spill_regno].constant != NULL)
		for (x = ira_reg_equiv[spill_regno].init_insns;
		     x != NULL;
		     x = XEXP (x, 1))
		  cost -= REG_FREQ_FROM_BB (BLOCK_FOR_INSN (XEXP (x, 0)));
	    }
	  if (best_insn_pseudos_num > insn_pseudos_num
	      || (best_insn_pseudos_num == insn_pseudos_num && best_cost > cost))
	    {
	      best_insn_pseudos_num = insn_pseudos_num;
	      best_cost = cost;
	      best_hard_regno = hard_regno;
	      bitmap_copy (&best_spill_pseudos_bitmap, &spill_pseudos_bitmap);
	      if (lra_dump_file != NULL)
		fprintf (lra_dump_file, "        Now best %d(cost=%d)\n",
			 hard_regno, cost);
	    }
	  assign_temporarily (regno, -1);
	  for (j = 0; j < n; j++)
	    {
	      reload_regno = sorted_reload_pseudos[j];
	      if (live_pseudos_reg_renumber[reload_regno] >= 0)
		assign_temporarily (reload_regno, -1);
	    }
	}
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "\n");
      /* Restore the live hard reg pseudo info for spilled pseudos.  */
      EXECUTE_IF_SET_IN_BITMAP (&spill_pseudos_bitmap, 0, spill_regno, bi)
	update_lives (spill_regno, false);
    fail:
      ;
    }
  /* Spill: */
  EXECUTE_IF_SET_IN_BITMAP (&best_spill_pseudos_bitmap, 0, spill_regno, bi)
    {
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "      Spill %sr%d(hr=%d, freq=%d) for r%d\n",
		 ((int) spill_regno < lra_constraint_new_regno_start
		  ? ""
		  : bitmap_bit_p (&lra_inheritance_pseudos, spill_regno)
		  ? "inheritance "
		  : bitmap_bit_p (&lra_split_pseudos, spill_regno)
		  ? "split " : "reload "),
		 spill_regno, reg_renumber[spill_regno],
		 lra_reg_info[spill_regno].freq, regno);
      update_lives (spill_regno, true);
      lra_setup_reg_renumber (spill_regno, -1, false);
    }
  bitmap_ior_into (spilled_pseudo_bitmap, &best_spill_pseudos_bitmap);
  return best_hard_regno;
}

/* Assign HARD_REGNO to REGNO.  */
static void
assign_hard_regno (int hard_regno, int regno)
{
  int i;

  gcc_assert (hard_regno >= 0);
  lra_setup_reg_renumber (regno, hard_regno, true);
  update_lives (regno, false);
  for (i = 0;
       i < hard_regno_nregs[hard_regno][PSEUDO_REGNO_MODE (regno)];
       i++)
    df_set_regs_ever_live (hard_regno + i, true);
}

/* Array used for sorting different pseudos.  */
static int *sorted_pseudos;

/* Constraint transformation can use equivalences and they can
   contains pseudos assigned to hard registers.  Such equivalence
   usage might create new conflicts of pseudos with hard registers
   (like ones used for parameter passing or call clobbered ones) or
   other pseudos assigned to the same hard registers.  Another very
   rare risky transformation is restoring whole multi-register pseudo
   when only one subreg lives and unused hard register is used already
   for something else.

   Process pseudos assigned to hard registers (most frequently used
   first), spill if a conflict is found, and mark the spilled pseudos
   in SPILLED_PSEUDO_BITMAP.  Set up LIVE_HARD_REG_PSEUDOS from
   pseudos, assigned to hard registers. */
static void
setup_live_pseudos_and_spill_after_risky_transforms (bitmap spilled_pseudo_bitmap)
{
  int p, i, j, n, regno, hard_regno;
  unsigned int k, conflict_regno;
  int val;
  HARD_REG_SET conflict_set;
  enum machine_mode mode;
  lra_live_range_t r;
  bitmap_iterator bi;

  for (n = 0, i = FIRST_PSEUDO_REGISTER; i < max_reg_num (); i++)
    if (reg_renumber[i] >= 0 && lra_reg_info[i].nrefs > 0)
      {
	if (lra_risky_transformations_p)
	  sorted_pseudos[n++] = i;
	else
	  update_lives (i, false);
      }
  if (! lra_risky_transformations_p)
    return;
  qsort (sorted_pseudos, n, sizeof (int), pseudo_compare_func);
  for (i = 0; i < n; i++)
    {
      regno = sorted_pseudos[i];
      hard_regno = reg_renumber[regno];
      gcc_assert (hard_regno >= 0);
      mode = lra_reg_info[regno].biggest_mode;
      sparseset_clear (live_range_hard_reg_pseudos);
      for (r = lra_reg_info[regno].live_ranges; r != NULL; r = r->next)
	{
	  EXECUTE_IF_SET_IN_BITMAP (&live_hard_reg_pseudos[r->start], 0, k, bi)
	    sparseset_set_bit (live_range_hard_reg_pseudos, k);
	  for (p = r->start + 1; p <= r->finish; p++)
	    {
	      lra_live_range_t r2;
	      
	      for (r2 = lra_start_point_ranges[p]; r2 != NULL; r2 = r2->start_next)
		if (live_pseudos_reg_renumber[r2->regno] >= 0)
		  sparseset_set_bit (live_range_hard_reg_pseudos, r2->regno);
	    }
	}
      COPY_HARD_REG_SET (conflict_set, lra_no_alloc_regs);
      val = lra_reg_info[regno].val;
      EXECUTE_IF_SET_IN_SPARSESET (live_range_hard_reg_pseudos, conflict_regno)
	if (val != lra_reg_info[conflict_regno].val)
	  lra_add_hard_reg_set (reg_renumber[conflict_regno],
				lra_reg_info[conflict_regno].biggest_mode,
				&conflict_set);
      if (! lra_hard_reg_set_intersection_p (hard_regno, mode, conflict_set))
	{
	  update_lives (regno, false);
	  continue;
	}
      bitmap_set_bit (spilled_pseudo_bitmap, regno);
      hard_regno = reg_renumber[regno];
      for (j = 0;
	   j < hard_regno_nregs[hard_regno][PSEUDO_REGNO_MODE (regno)];
	   j++)
	lra_hard_reg_usage[hard_regno + j] -= lra_reg_info[regno].freq;
      reg_renumber[regno] = -1;
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "    Spill r%d after risky transformations\n",
		 regno);
    }
}

/* Improve allocation by assigning the same hard regno of inheritance
   pseudos to the connected pseudos.  We need this because inheritance
   pseudos are allocated after reload pseudos in the thread and when
   we assign a hard register to a reload pseudo we don't know yet that
   the connected inheritance pseudos can get the same hard
   register.  */
static void
improve_inheritance (void)
{
  unsigned int k;
  int regno, another_regno, hard_regno, another_hard_regno, cost, i, n;
  lra_copy_t cp, next_cp;
  bitmap_iterator bi;

  n = 0;
  EXECUTE_IF_SET_IN_BITMAP (&lra_inheritance_pseudos, 0, k, bi)
    if (reg_renumber[k] >= 0 && lra_reg_info[k].nrefs != 0)
      sorted_pseudos[n++] = k;
  qsort (sorted_pseudos, n, sizeof (int), pseudo_compare_func);
  for (i = 0; i < n; i++)
    {
      regno = sorted_pseudos[i];
      hard_regno = reg_renumber[regno];
      gcc_assert (hard_regno >= 0);
      for (cp = lra_reg_info[regno].copies; cp != NULL; cp = next_cp)
	{
	  if (cp->regno1 == regno)
	    {
	      next_cp = cp->regno1_next;
	      another_regno = cp->regno2;
	    }
	  else if (cp->regno2 == regno)
	    {
	      next_cp = cp->regno2_next;
	      another_regno = cp->regno1;
	    }
	  else
	    gcc_unreachable ();
	  if ((another_hard_regno = reg_renumber[another_regno]) >= 0
	      && another_hard_regno != hard_regno)
	    {
	      if (lra_dump_file != NULL)
		fprintf (lra_dump_file,
			 "    Improving inheritance for %d(%d) and %d(%d)...\n",
			 regno, hard_regno, another_regno, another_hard_regno);
	      update_lives (another_regno, true);
	      lra_setup_reg_renumber (another_regno, -1, false);
	      if (hard_regno
		  == find_hard_regno_for (another_regno, &cost, hard_regno))
		assign_hard_regno (hard_regno, another_regno);
	      else
		assign_hard_regno (another_hard_regno, another_regno);
	    }
	}
    }
}


/* Bitmap finaly containing all pseudos spilled on this assignment
   pass.  */
static bitmap_head all_spilled_pseudos;
/* All pseudos whose allocation was changed.  */
static bitmap_head changed_pseudo_bitmap;

/* Assign hard registers to reload pseudos and other pseudos.  */
static void
assign_by_spills (void)
{
  int i, n, nfails, iter, regno, hard_regno, cost, restore_regno;
  rtx insn;
  basic_block bb;
  bitmap_head changed_insns, do_not_assign_nonreload_pseudos;
  unsigned int u;
  bitmap_iterator bi;

  for (n = 0, i = lra_constraint_new_regno_start; i < max_reg_num (); i++)
    if (reg_renumber[i] < 0 && lra_reg_info[i].nrefs != 0
	&& regno_allocno_class_array[i] != NO_REGS)
      sorted_pseudos[n++] = i;
  bitmap_initialize (&ignore_pseudos_bitmap, &reg_obstack);
  bitmap_initialize (&spill_pseudos_bitmap, &reg_obstack);
  bitmap_initialize (&best_spill_pseudos_bitmap, &reg_obstack);
  update_hard_regno_preference_check = (int *) xmalloc (sizeof (int)
							* max_reg_num ());
  memset (update_hard_regno_preference_check, 0,
	  sizeof (int) * max_reg_num ());
  curr_update_hard_regno_preference_check = 0;
  memset (try_hard_reg_pseudos_check, 0, sizeof (try_hard_reg_pseudos_check));
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    bitmap_initialize (&try_hard_reg_pseudos[i], &reg_obstack);
  curr_pseudo_check = 0;
  bitmap_initialize (&changed_insns, &reg_obstack);
  for (iter = 0; iter <= 1; iter++)
    {
      qsort (sorted_pseudos, n, sizeof (int), reload_pseudo_compare_func);
      nfails = 0;
      for (i = 0; i < n; i++)
	{
	  regno = sorted_pseudos[i];
	  if (lra_dump_file != NULL)
	    fprintf (lra_dump_file, "    Assigning to %d "
		     "(cl=%s, orig=%d, freq=%d, tfirst=%d, tfreq=%d)...\n",
		     regno, reg_class_names[regno_allocno_class_array[regno]],
		     ORIGINAL_REGNO (regno_reg_rtx[regno]),
		     lra_reg_info[regno].freq, regno_assign_info[regno].first,
		     regno_assign_info[regno_assign_info[regno].first].freq);
	  hard_regno = find_hard_regno_for (regno, &cost, -1);
	  if (hard_regno < 0
	      && ! bitmap_bit_p (&lra_inheritance_pseudos, regno)
	      && ! bitmap_bit_p (&lra_split_pseudos, regno))
	    hard_regno = spill_for (regno, &all_spilled_pseudos);
	  if (hard_regno < 0)
	    {
	      if (! bitmap_bit_p (&lra_inheritance_pseudos, regno)
		  && ! bitmap_bit_p (&lra_split_pseudos, regno))
		sorted_pseudos[nfails++] = regno;
	    }
	  else
	    {
	      /* Remember that reload pseudos can be spilled on the
		 1st pass.  */
	      bitmap_clear_bit (&all_spilled_pseudos, regno);
	      assign_hard_regno (hard_regno, regno);
	    }
	}
      if (nfails == 0)
	break;
      gcc_assert (iter == 0);
      /* This is a very rare event.  We can not assign a hard
	 register to reload pseudo because the hard register was
	 assigned to another reload pseudo on a previous
	 assignment pass.  For x86 example, on the 1st pass we
	 assigned CX (although another hard register could be used
	 for this) to reload pseudo in an insn, on the 2nd pass we
	 need CX (and only this) hard register for a new reload
	 pseudo in the same insn.  */
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "  2nd iter for reload pseudo assignments:\n");
      for (i = 0; i < nfails; i++)
	{
	  if (lra_dump_file != NULL)
	    fprintf (lra_dump_file, "    Reload r%d assignment failure\n",
		     sorted_pseudos[i]);
	  bitmap_ior_into (&changed_insns,
			   &lra_reg_info[sorted_pseudos[i]].insn_bitmap);
	}
      FOR_EACH_BB (bb)
	FOR_BB_INSNS (bb, insn)
	if (bitmap_bit_p (&changed_insns, INSN_UID (insn)))
	  {
	    lra_insn_recog_data_t data;
	    struct lra_insn_reg *r;
	      
	    data = lra_get_insn_recog_data (insn);
	    for (r = data->regs; r != NULL; r = r->next)
	      {
		regno = r->regno;
		if (regno < lra_constraint_new_regno_start
		    || bitmap_bit_p (&lra_inheritance_pseudos, regno)
		    || reg_renumber[regno] < 0)
		  continue;
		sorted_pseudos[nfails++] = regno;
		if (lra_dump_file != NULL)
		  fprintf (lra_dump_file,
			   "      Spill reload r%d(hr=%d, freq=%d)\n",
			   regno, reg_renumber[regno],
			   lra_reg_info[regno].freq);
		update_lives (regno, true);
		lra_setup_reg_renumber (regno, -1, false);
	      }
	  }
      n = nfails;
    }
  improve_inheritance ();
  bitmap_clear (&changed_insns);
  /* We can not assign to inherited pseudos if any its inheritance
     pseudo did not get hard register because undo inheritance pass
     will extend live range of such inherited pseudos.  */
  bitmap_initialize (&do_not_assign_nonreload_pseudos, &reg_obstack);
  EXECUTE_IF_SET_IN_BITMAP (&lra_inheritance_pseudos, 0, u, bi)
    if ((restore_regno = lra_reg_info[u].restore_regno) >= 0
	&& ((reg_renumber[u] < 0
	     && bitmap_bit_p (&lra_inheritance_pseudos, u))
	    || (reg_renumber[u] >= 0
		&& bitmap_bit_p (&lra_split_pseudos, u))))
      bitmap_set_bit (&do_not_assign_nonreload_pseudos, restore_regno);
  for (n = 0, i = FIRST_PSEUDO_REGISTER; i < max_reg_num (); i++)
    if (((i < lra_constraint_new_regno_start
	  && ! bitmap_bit_p (&do_not_assign_nonreload_pseudos, i))
	 || (bitmap_bit_p (&lra_inheritance_pseudos, i)
	     && lra_reg_info[i].restore_regno >= 0)
	 || (bitmap_bit_p (&lra_split_pseudos, i)
	     && lra_reg_info[i].restore_regno >= 0))
	&& reg_renumber[i] < 0 && lra_reg_info[i].nrefs != 0
	&& regno_allocno_class_array[i] != NO_REGS)
      sorted_pseudos[n++] = i;
  bitmap_clear (&do_not_assign_nonreload_pseudos);
  if (n != 0 && lra_dump_file != NULL)
    fprintf (lra_dump_file, "  Reassing non-reload pseudos\n");
  qsort (sorted_pseudos, n, sizeof (int), pseudo_compare_func);
  for (i = 0; i < n; i++)
    {
      regno = sorted_pseudos[i];
      hard_regno = find_hard_regno_for (regno, &cost, -1);
      if (hard_regno >= 0)
	{
	  bitmap_set_bit (&changed_pseudo_bitmap, regno);
	  assign_hard_regno (hard_regno, regno);
	  /* We change allocation for non-reload pseudo on
	     this iteration -- mark it for invalidation used
	     alternative of insns containing the pseudo.  */
	  bitmap_set_bit (&changed_pseudo_bitmap, regno);
	}
    }
  free (update_hard_regno_preference_check);
  bitmap_clear (&best_spill_pseudos_bitmap);
  bitmap_clear (&spill_pseudos_bitmap);
  bitmap_clear (&ignore_pseudos_bitmap);
}


/* Entry function to assign hard registers to new reload pseudos
   starting with LRA_CONSTRAINT_NEW_REGNO_START (by possible spilling
   of old pseudos) and possibly to the old pseudos.  The function adds
   what insns to process for the next constraint pass.  Those are all
   insns who contains non-reload and non-inheritance pseudos with
   changed allocation.

   Return true if we did not spill any non-reload and non-inheritance
   pseudos.  */
bool
lra_assign (void)
{
  int i;
  unsigned int u;
  bitmap_iterator bi;
  bitmap_head insns_to_process;
  bool no_spills_p;

  init_lives ();
  sorted_pseudos = (int *) xmalloc (sizeof (int) * max_reg_num ());
  sorted_reload_pseudos = (int *) xmalloc (sizeof (int) * max_reg_num ());
  regno_allocno_class_array
    = (enum reg_class *) xmalloc (sizeof (enum reg_class) * max_reg_num ());
  for (i = FIRST_PSEUDO_REGISTER; i < max_reg_num (); i++)
    regno_allocno_class_array[i] = lra_get_allocno_class (i);
  init_regno_assign_info ();
  bitmap_initialize (&all_spilled_pseudos, &reg_obstack);
  setup_live_pseudos_and_spill_after_risky_transforms (&all_spilled_pseudos);
  /* Setup insns to process.  */
  bitmap_initialize (&changed_pseudo_bitmap, &reg_obstack);
  init_live_reload_and_inheritance_pseudos ();
  assign_by_spills ();
  finish_live_reload_and_inheritance_pseudos ();
  bitmap_ior_into (&changed_pseudo_bitmap, &all_spilled_pseudos);
  no_spills_p = true;
  EXECUTE_IF_SET_IN_BITMAP (&all_spilled_pseudos, 0, u, bi)
    /* We ignore spilled pseudos created on last inheritance pass
       because they will be removed.  */
    if (lra_reg_info[u].restore_regno < 0)
      {
	no_spills_p = false;
	break;
      }
  bitmap_clear (&all_spilled_pseudos);
  bitmap_initialize (&insns_to_process, &reg_obstack);
  EXECUTE_IF_SET_IN_BITMAP (&changed_pseudo_bitmap, 0, u, bi)
    bitmap_ior_into (&insns_to_process, &lra_reg_info[u].insn_bitmap);
  bitmap_clear (&changed_pseudo_bitmap);
  EXECUTE_IF_SET_IN_BITMAP (&insns_to_process, 0, u, bi)
    {
      lra_push_insn_by_uid (u);
      /* Invalidate alternatives for insn should be processed.  */
      lra_set_used_insn_alternative_by_uid (u, -1);
    }
  bitmap_clear (&insns_to_process);
  finish_regno_assign_info ();
  free (regno_allocno_class_array);
  free (sorted_pseudos);
  free (sorted_reload_pseudos);
  finish_lives ();
  return no_spills_p;
}
