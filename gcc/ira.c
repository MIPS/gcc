/* Integrated Register Allocator (IRA) entry point.
   Copyright (C) 2006, 2007, 2008
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

/* The integrated register allocator (IRA) is called integrated
   because register coalescing, register live range splitting, and
   choosing a better hard register are done on-the-fly during
   coloring.  Register coalescing and choosing a cheaper hard register
   is done by hard register preferencing during hard register
   assigning.  The live range splitting is a byproduct of the regional
   register allocation.

   The regional allocation is top-down process.  The first we do
   allocation for all function then we improve it for loops then their
   subloops and so on.  To reduce register shuffling, the same
   mechanism of hard register preferencing is used.  This approach
   works as good as Callahan-Koblentz algorithm but it is simpler.  We
   use Chaitin-Briggs coloring for each region (loop or all function).
   If pseudo-registers got different location on loop borders we
   rename them inside the loop and generate pseudo-register move
   insns.  Some optimizations (like removing redundant stores, moving
   register shuffling to less frequent points, and code duplication
   reducing) to minimize effect of register shuffling are done.

   If we don't improve register allocation for loops (or there are no
   loops at all) we get classic Chaitin-Briggs coloring (only instead
   of separate pass of coalescing, we use hard register preferencing).
   In any case, before the reload work, we have one region IRA
   internal representation.

   IRA also has better integration with the reload pass than the old
   register allocator.  Pseudo-registers spilled by IRA or the reload
   have still a chance to get hard-registers when the reload evicts
   some pseudo-registers from hard-registers.  IRA helps to choose
   better pseudos for spilling based on their live ranges and to
   coalesce stack slots allocated for the spilled pseudo-registers.

   Literature is worth to read for better understanding the code:

   o Preston Briggs, Keith D. Cooper, Linda Torczon.  Improvements to
     Graph Coloring Register Allocation.

   o David Callahan, Brian Koblenz.  Register allocation via
     hierarchical graph coloring.

   o Keith Cooper, Anshuman Dasgupta, Jason Eckhardt. Revisiting Graph
     Coloring Register Allocation: A Study of the Chaitin-Briggs and
     Callahan-Koblenz Algorithms.

   o Guei-Yuan Lueh, Thomas Gross, and Ali-Reza Adl-Tabatabai. Global
     Register Allocation Based on Graph Fusion.

   o Vladimir Makarov. The Integrated Register Allocator for GCC.

*/


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "regs.h"
#include "rtl.h"
#include "tm_p.h"
#include "target.h"
#include "flags.h"
#include "obstack.h"
#include "bitmap.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "expr.h"
#include "recog.h"
#include "params.h"
#include "timevar.h"
#include "tree-pass.h"
#include "output.h"
#include "reload.h"
#include "errors.h"
#include "integrate.h"
#include "df.h"
#include "ggc.h"
#include "ira-int.h"

static void setup_reg_mode_hard_regset (void);
static void setup_class_hard_regs (void);
static void setup_available_class_regs (void);
static void setup_alloc_regs (int);
static void setup_class_subset_and_memory_move_costs (void);
static void setup_reg_subclasses (void);
#ifdef IRA_COVER_CLASSES
static void setup_cover_and_important_classes (void);
static void setup_class_translate (void);
static void setup_reg_class_intersect_union (void);
#endif
static void print_class_cover (FILE *);
static void find_reg_class_closure (void);
static void setup_reg_class_nregs (void);
static void setup_prohibited_class_mode_regs (void);
static void free_register_move_costs (void);
static void setup_prohibited_mode_move_regs (void);
static int insn_contains_asm_1 (rtx *, void *);
static int insn_contains_asm (rtx);
static void compute_regs_asm_clobbered (char *);
static void setup_eliminable_regset (void);
static void find_reg_equiv_invariant_const (void);
static void setup_reg_renumber (void);
static void setup_allocno_assignment_flags (void);
static void calculate_allocation_cost (void);
#ifdef ENABLE_IRA_CHECKING
static void check_allocation (void);
#endif
static void fix_reg_equiv_init (void);
#ifdef ENABLE_IRA_CHECKING
static void print_redundant_copies (void);
#endif
static void setup_preferred_alternate_classes (void);
static void expand_reg_info (int);
static int chain_freq_compare (const void *, const void *);
static int chain_bb_compare (const void *, const void *);

static void ira (FILE *);
static bool gate_ira (void);
static unsigned int rest_of_handle_ira (void);

/* A modified value of flag `-fira-verbose' used internally.  */
int internal_flag_ira_verbose;

/* Dump file of the allocator if it is not NULL.  */
FILE *ira_dump_file;

/* Pools for allocnos, copies, allocno live ranges.  */
alloc_pool allocno_pool, copy_pool, allocno_live_range_pool;

/* The number of elements in the following array.  */
int spilled_reg_stack_slots_num;

/* The following array contains info about spilled pseudo-registers
   stack slots used in current function so far.  */
struct spilled_reg_stack_slot *spilled_reg_stack_slots;

/* Correspondingly overall cost of the allocation, cost of the
   allocnos assigned to hard-registers, cost of the allocnos assigned
   to memory, cost of loads, stores and register move insns generated
   for pseudo-register live range splitting (see ira-emit.c).  */
int overall_cost;
int reg_cost, mem_cost;
int load_cost, store_cost, shuffle_cost;
int move_loops_num, additional_jumps_num;

/* Map: hard regs X modes -> set of hard registers for storing value
   of given mode starting with given hard register.  */
HARD_REG_SET reg_mode_hard_regset [FIRST_PSEUDO_REGISTER] [NUM_MACHINE_MODES];

/* The following two variables are array analog of macros
   MEMORY_MOVE_COST and REGISTER_MOVE_COST.  */
short int memory_move_cost [MAX_MACHINE_MODE] [N_REG_CLASSES] [2];
move_table *register_move_cost [MAX_MACHINE_MODE];

/* Similar to may_move_in_cost but it is calculated in IRA instead of
   regclass.  Another difference we take only available hard registers
   into account to figure out that one register class is a subset of
   the another one.  */
move_table *register_may_move_in_cost [MAX_MACHINE_MODE];

/* Similar to may_move_out_cost but it is calculated in IRA instead of
   regclass.  Another difference we take only available hard registers
   into account to figure out that one register class is a subset of
   the another one.  */
move_table *register_may_move_out_cost [MAX_MACHINE_MODE];

/* Register class subset relation: TRUE if the first class is a subset
   of the second one considering only hard registers available for the
   allocation.  */
int class_subset_p [N_REG_CLASSES] [N_REG_CLASSES];

/* Temporary hard reg set used for different calculation.  */
static HARD_REG_SET temp_hard_regset;



/* The function sets up map REG_MODE_HARD_REGSET.  */
static void
setup_reg_mode_hard_regset (void)
{
  int i, m, hard_regno;

  for (m = 0; m < NUM_MACHINE_MODES; m++)
    for (hard_regno = 0; hard_regno < FIRST_PSEUDO_REGISTER; hard_regno++)
      {
	CLEAR_HARD_REG_SET (reg_mode_hard_regset [hard_regno] [m]);
	for (i = hard_regno_nregs [hard_regno] [m] - 1; i >= 0; i--)
	  if (hard_regno + i < FIRST_PSEUDO_REGISTER)
	    SET_HARD_REG_BIT (reg_mode_hard_regset [hard_regno] [m],
			      hard_regno + i);
      }
}



/* Hard registers that can not be used for the register allocator for
   all functions of the current compilation unit.  */
static HARD_REG_SET no_unit_alloc_regs;

/* Array of number of hard registers of given class which are
   available for the allocation.  The order is defined by the
   allocation order.  */
short class_hard_regs [N_REG_CLASSES] [FIRST_PSEUDO_REGISTER];

/* The number of elements of the above array for given register
   class.  */
int class_hard_regs_num [N_REG_CLASSES];

/* Index (in class_hard_regs) for given register class and hard
   register (in general case a hard register can belong to several
   register classes).  The index is negative for hard registers
   unavailable for the allocation. */
short class_hard_reg_index [N_REG_CLASSES] [FIRST_PSEUDO_REGISTER];

/* The function sets up the three arrays declared above.  */
static void
setup_class_hard_regs (void)
{
  int cl, i, hard_regno, n;
  HARD_REG_SET processed_hard_reg_set;

  ira_assert (SHRT_MAX >= FIRST_PSEUDO_REGISTER);
  /* We could call ORDER_REGS_FOR_LOCAL_ALLOC here (it is usually
     putting hard callee-used hard registers first).  But our
     heuristics work better.  */
  for (cl = (int) N_REG_CLASSES - 1; cl >= 0; cl--)
    {
      COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [cl]);
      AND_COMPL_HARD_REG_SET (temp_hard_regset, no_unit_alloc_regs);
      CLEAR_HARD_REG_SET (processed_hard_reg_set);
      for (n = 0, i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	{
#ifdef REG_ALLOC_ORDER
	  hard_regno = reg_alloc_order [i];
#else
	  hard_regno = i;
#endif	  
	  if (TEST_HARD_REG_BIT (processed_hard_reg_set, hard_regno))
	    continue;
	  SET_HARD_REG_BIT (processed_hard_reg_set, hard_regno);
      	  if (! TEST_HARD_REG_BIT (temp_hard_regset, hard_regno))
	    class_hard_reg_index [cl] [hard_regno] = -1;
	  else
	    {
	      class_hard_reg_index [cl] [hard_regno] = n;
	      class_hard_regs [cl] [n++] = hard_regno;
	    }
	}
      class_hard_regs_num [cl] = n;
    }
}

/* Number of given class hard registers available for the register
   allocation for given classes.  */
int available_class_regs [N_REG_CLASSES];

/* Function setting up AVAILABLE_CLASS_REGS.  */
static void
setup_available_class_regs (void)
{
  int i, j;

  memset (available_class_regs, 0, sizeof (available_class_regs));
  for (i = 0; i < N_REG_CLASSES; i++)
    {
      COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [i]);
      AND_COMPL_HARD_REG_SET (temp_hard_regset, no_unit_alloc_regs);
      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
	if (TEST_HARD_REG_BIT (temp_hard_regset, j))
	  available_class_regs [i]++;
    }
}

/* The function setting up different global variables defining info
   about hard registers for the allocation.  It depends on
   USE_HARD_FRAME_P whose nonzero value means that we can use hard
   frame pointer for the allocation.  */
static void
setup_alloc_regs (int use_hard_frame_p)
{
  COPY_HARD_REG_SET (no_unit_alloc_regs, fixed_reg_set);
  if (! use_hard_frame_p)
    SET_HARD_REG_BIT (no_unit_alloc_regs, HARD_FRAME_POINTER_REGNUM);
  setup_class_hard_regs ();
  setup_available_class_regs ();
}



/* The function sets up MEMORY_MOVE_COST, REGISTER_MOVE_COST.  */
static void
setup_class_subset_and_memory_move_costs (void)
{
  int cl, cl2;
  enum machine_mode mode;
  HARD_REG_SET temp_hard_regset2;

  for (cl = (int) N_REG_CLASSES - 1; cl >= 0; cl--)
    {
      for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
	{
	  memory_move_cost [mode] [cl] [0] = MEMORY_MOVE_COST (mode, cl, 0);
	  memory_move_cost [mode] [cl] [1] = MEMORY_MOVE_COST (mode, cl, 1);
	}

      for (cl2 = (int) N_REG_CLASSES - 1; cl2 >= 0; cl2--)
	{
	  COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [cl]);
	  AND_COMPL_HARD_REG_SET (temp_hard_regset, no_unit_alloc_regs);
	  COPY_HARD_REG_SET (temp_hard_regset2, reg_class_contents [cl2]);
	  AND_COMPL_HARD_REG_SET (temp_hard_regset2, no_unit_alloc_regs);
	  class_subset_p [cl] [cl2]
	    = hard_reg_set_subset_p (temp_hard_regset, temp_hard_regset2);
	}
    }
}



/* Define the following macro if allocation through malloc if
   preferable.  */
#define IRA_NO_OBSTACK

#ifndef IRA_NO_OBSTACK
/* Obstack used for storing all dynamic data (except bitmaps) of the
   IRA.  */
static struct obstack ira_obstack;
#endif

/* Obstack used for storing all bitmaps of the IRA.  */
static struct bitmap_obstack ira_bitmap_obstack;

/* The function allocates memory of size LEN for IRA data.  */
void *
ira_allocate (size_t len)
{
  void *res;

#ifndef IRA_NO_OBSTACK
  res = obstack_alloc (&ira_obstack, len);
#else
  res = xmalloc (len);
#endif
  return res;
}

/* The function reallocates memory PTR of size LEN for IRA data.  */
void *
ira_reallocate (void *ptr, size_t len)
{
  void *res;

#ifndef IRA_NO_OBSTACK
  res = obstack_alloc (&ira_obstack, len);
#else
  res = xrealloc (ptr, len);
#endif
  return res;
}

/* The function free memory ADDR allocated for IRA data.  */
void
ira_free (void *addr ATTRIBUTE_UNUSED)
{
#ifndef IRA_NO_OBSTACK
  /* do nothing */
#else
  free (addr);
#endif
}


/* The function allocates and returns bitmap for IRA.  */
bitmap
ira_allocate_bitmap (void)
{
  return BITMAP_ALLOC (&ira_bitmap_obstack);
}

/* The function frees bitmap B allocated for IRA.  */
void
ira_free_bitmap (bitmap b ATTRIBUTE_UNUSED)
{
  /* do nothing */
}



/* The function outputs information about allocation of all allocnos
   into file F.  */
void
print_disposition (FILE *f)
{
  int i, n, max_regno;
  allocno_t a;
  basic_block bb;

  fprintf (f, "Disposition:");
  max_regno = max_reg_num ();
  for (n = 0, i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    for (a = regno_allocno_map [i];
	 a != NULL;
	 a = ALLOCNO_NEXT_REGNO_ALLOCNO (a))
      {
	if (n % 4 == 0)
	  fprintf (f, "\n");
	n++;
	fprintf (f, " %4d:r%-4d", ALLOCNO_NUM (a), ALLOCNO_REGNO (a));
	if ((bb = ALLOCNO_LOOP_TREE_NODE (a)->bb) != NULL)
	  fprintf (f, "b%-3d", bb->index);
	else
	  fprintf (f, "l%-3d", ALLOCNO_LOOP_TREE_NODE (a)->loop->num);
	if (ALLOCNO_HARD_REGNO (a) >= 0)
	  fprintf (f, " %3d", ALLOCNO_HARD_REGNO (a));
	else
	  fprintf (f, " mem");
      }
  fprintf (f, "\n");
}

/* The function outputs information about allocation of all allocnos
   into stderr.  */
void
debug_disposition (void)
{
  print_disposition (stderr);
}



/* For each reg class, table listing all the classes contained in it
   (excluding the class itself.  Non-allocatable registers are
   excluded from the consideration).  */
static enum reg_class alloc_reg_class_subclasses[N_REG_CLASSES][N_REG_CLASSES];

/* The function initializes the table of subclasses of each reg
   class.  */
static void
setup_reg_subclasses (void)
{
  int i, j;
  HARD_REG_SET temp_hard_regset2;

  for (i = 0; i < N_REG_CLASSES; i++)
    for (j = 0; j < N_REG_CLASSES; j++)
      alloc_reg_class_subclasses [i] [j] = LIM_REG_CLASSES;

  for (i = 0; i < N_REG_CLASSES; i++)
    {
      if (i == (int) NO_REGS)
	continue;

      COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [i]);
      AND_COMPL_HARD_REG_SET (temp_hard_regset, no_unit_alloc_regs);
      if (hard_reg_set_equal_p (temp_hard_regset, zero_hard_reg_set))
	continue;
      for (j = 0; j < N_REG_CLASSES; j++)
	if (i != j)
	  {
	    enum reg_class *p;

	    COPY_HARD_REG_SET (temp_hard_regset2, reg_class_contents [j]);
	    AND_COMPL_HARD_REG_SET (temp_hard_regset2, no_unit_alloc_regs);
	    if (! hard_reg_set_subset_p (temp_hard_regset,
					 temp_hard_regset2))
	      continue;
	    p = &alloc_reg_class_subclasses [j] [0];
	    while (*p != LIM_REG_CLASSES) p++;
	    *p = (enum reg_class) i;
	  }
    }
}



/* Number of cover classes.  Cover classes is non-intersected register
   classes containing all hard-registers available for the
   allocation.  */
int reg_class_cover_size;

/* The array containing cover classes (see also comments for macro
   IRA_COVER_CLASSES).  Only first REG_CLASS_COVER_SIZE elements are
   used for this.  */
enum reg_class reg_class_cover [N_REG_CLASSES];

/* The value is number of elements in the subsequent array.  */
int important_classes_num;

/* The array containing classes (including cover classes) which are
   subclasses of cover classes.  Such classes is important for
   calculation of the hard register usage costs.  */
enum reg_class important_classes [N_REG_CLASSES];

/* The array containing indexes of important classes in the previous
   array.  The array elements are defined only for important
   classes.  */
int important_class_nums [N_REG_CLASSES];

#ifdef IRA_COVER_CLASSES

/* The function checks IRA_COVER_CLASSES and sets the four global
   variables defined above.  */
static void
setup_cover_and_important_classes (void)
{
  int i, j;
  enum reg_class cl;
  static enum reg_class classes [] = IRA_COVER_CLASSES;
  HARD_REG_SET temp_hard_regset2;

  reg_class_cover_size = 0;
  for (i = 0; (cl = classes [i]) != LIM_REG_CLASSES; i++)
    {
      for (j = 0; j < i; j++)
	if (reg_classes_intersect_p (cl, classes [j]))
	  gcc_unreachable ();
      COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [cl]);
      AND_COMPL_HARD_REG_SET (temp_hard_regset, no_unit_alloc_regs);
      if (! hard_reg_set_equal_p (temp_hard_regset, zero_hard_reg_set))
	reg_class_cover [reg_class_cover_size++] = cl;
    }
  important_classes_num = 0;
  for (cl = 0; cl < N_REG_CLASSES; cl++)
    {
      COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [cl]);
      AND_COMPL_HARD_REG_SET (temp_hard_regset, no_unit_alloc_regs);
      if (! hard_reg_set_equal_p (temp_hard_regset, zero_hard_reg_set))
	for (j = 0; j < reg_class_cover_size; j++)
	  {
	    COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [cl]);
	    AND_COMPL_HARD_REG_SET (temp_hard_regset, no_unit_alloc_regs);
	    COPY_HARD_REG_SET (temp_hard_regset2,
			       reg_class_contents [reg_class_cover [j]]);
	    AND_COMPL_HARD_REG_SET (temp_hard_regset2, no_unit_alloc_regs);
	    if (cl == reg_class_cover [j]
		|| (hard_reg_set_subset_p (temp_hard_regset, temp_hard_regset2)
		    && ! hard_reg_set_equal_p (temp_hard_regset,
					       temp_hard_regset2)))
	      {
		important_class_nums [cl] = important_classes_num;
		important_classes [important_classes_num++] = cl;
	      }
	  }
    }
}
#endif

/* Map of all register classes to corresponding cover class containing
   the given class.  If given class is not a subset of a cover class,
   we translate it into the cheapest cover class.  */
enum reg_class class_translate [N_REG_CLASSES];

#ifdef IRA_COVER_CLASSES

/* The function sets up array CLASS_TRANSLATE.  */
static void
setup_class_translate (void)
{
  enum reg_class cl, cover_class, best_class, *cl_ptr;
  enum machine_mode mode;
  int i, cost, min_cost, best_cost;

  for (cl = 0; cl < N_REG_CLASSES; cl++)
    class_translate [cl] = NO_REGS;
  for (i = 0; i < reg_class_cover_size; i++)
    {
      cover_class = reg_class_cover [i];
      for (cl_ptr = &alloc_reg_class_subclasses [cover_class] [0];
	   (cl = *cl_ptr) != LIM_REG_CLASSES;
	   cl_ptr++)
	{
	  if (class_translate [cl] == NO_REGS)
	    class_translate [cl] = cover_class;
#ifdef ENABLE_IRA_CHECKING
	  else
	    {
	      COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [cl]);
	      AND_COMPL_HARD_REG_SET (temp_hard_regset, no_unit_alloc_regs);
	      if (! hard_reg_set_subset_p (temp_hard_regset,
					   zero_hard_reg_set))
		gcc_unreachable ();
	    }
#endif
	}
      class_translate [cover_class] = cover_class;
    }
  /* For classes which are not fully covered by a cover class (in
     other words covered by more one cover class), use the cheapest
     cover class.  */
  for (cl = 0; cl < N_REG_CLASSES; cl++)
    {
      if (cl == NO_REGS || class_translate [cl] != NO_REGS)
	continue;
      best_class = NO_REGS;
      best_cost = INT_MAX;
      for (i = 0; i < reg_class_cover_size; i++)
	{
	  cover_class = reg_class_cover [i];
	  COPY_HARD_REG_SET (temp_hard_regset,
			     reg_class_contents [cover_class]);
	  AND_HARD_REG_SET (temp_hard_regset, reg_class_contents [cl]);
	  AND_COMPL_HARD_REG_SET (temp_hard_regset, no_unit_alloc_regs);
	  if (! hard_reg_set_equal_p (temp_hard_regset, zero_hard_reg_set))
	    {
	      min_cost = INT_MAX;
	      for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
		{
		  cost = (memory_move_cost [mode] [cl] [0]
			  + memory_move_cost [mode] [cl] [1]);
		  if (min_cost > cost)
		    min_cost = cost;
		}
	      if (best_class == NO_REGS || best_cost > min_cost)
		{
		  best_class = cover_class;
		  best_cost = min_cost;
		}
	    }
	}
      class_translate [cl] = best_class;
    }
}
#endif

/* The biggest important class inside of intersection of the two
   classes (that is calculated taking only hard registers available
   for allocation into account).  */
enum reg_class reg_class_intersect [N_REG_CLASSES] [N_REG_CLASSES];

/* The biggest important class inside of union of the two classes
   (that is calculated taking only hard registers available for
   allocation into account).  */
enum reg_class reg_class_union [N_REG_CLASSES] [N_REG_CLASSES];

#ifdef IRA_COVER_CLASSES

/* The function sets up REG_CLASS_INTERSECT and REG_CLASS_UNION.  */
static void
setup_reg_class_intersect_union (void)
{
  int i, cl1, cl2, cl3;
  HARD_REG_SET intersection_set, union_set, temp_set2;

  for (cl1 = 0; cl1 < N_REG_CLASSES; cl1++)
    {
      for (cl2 = 0; cl2 < N_REG_CLASSES; cl2++)
	{
	  reg_class_intersect [cl1] [cl2] = NO_REGS;
	  reg_class_union [cl1] [cl2] = NO_REGS;
	  COPY_HARD_REG_SET (intersection_set, reg_class_contents [cl1]);
	  AND_HARD_REG_SET (intersection_set, reg_class_contents [cl2]);
	  AND_COMPL_HARD_REG_SET (intersection_set, no_unit_alloc_regs);
	  COPY_HARD_REG_SET (union_set, reg_class_contents [cl1]);
	  IOR_HARD_REG_SET (union_set, reg_class_contents [cl2]);
	  AND_COMPL_HARD_REG_SET (union_set, no_unit_alloc_regs);
	  for (i = 0; i < important_classes_num; i++)
	    {
	      cl3 = important_classes [i];
	      COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [cl3]);
	      AND_COMPL_HARD_REG_SET (temp_hard_regset, no_unit_alloc_regs);
	      if (hard_reg_set_subset_p (temp_hard_regset, intersection_set))
		{
		  COPY_HARD_REG_SET
		    (temp_set2,
		     reg_class_contents
		     [(int) reg_class_intersect [cl1] [cl2]]);
		  AND_COMPL_HARD_REG_SET (temp_set2, no_unit_alloc_regs);
	 	  if (! hard_reg_set_subset_p (temp_hard_regset, temp_set2))
		    reg_class_intersect [cl1] [cl2] = (enum reg_class) cl3;
		}
	      if (hard_reg_set_subset_p (temp_hard_regset, union_set))
		{
		  COPY_HARD_REG_SET
		    (temp_set2,
		     reg_class_contents
		     [(int) reg_class_union [cl1] [cl2]]);
		  AND_COMPL_HARD_REG_SET (temp_set2, no_unit_alloc_regs);
	 	  if (hard_reg_set_subset_p (temp_set2, temp_hard_regset))
		    reg_class_union [cl1] [cl2] = (enum reg_class) cl3;
		}
	    }
	}
    }
}

#endif

/* The function outputs all cover classes and the translation map into
   file F.  */
static void
print_class_cover (FILE *f)
{
  static const char *const reg_class_names[] = REG_CLASS_NAMES;
  int i;

  fprintf (f, "Class cover:\n");
  for (i = 0; i < reg_class_cover_size; i++)
    fprintf (f, " %s", reg_class_names [reg_class_cover [i]]);
  fprintf (f, "\nClass translation:\n");
  for (i = 0; i < N_REG_CLASSES; i++)
    fprintf (f, " %s -> %s\n", reg_class_names [i],
	     reg_class_names [class_translate [i]]);
}

/* The function outputs all cover classes and the translation map into
   stderr.  */
void
debug_class_cover (void)
{
  print_class_cover (stderr);
}

/* Function setting up different arrays concerning class subsets,
   cover and important classes.  */
static void
find_reg_class_closure (void)
{
  setup_reg_subclasses ();
#ifdef IRA_COVER_CLASSES
  setup_cover_and_important_classes ();
  setup_class_translate ();
  setup_reg_class_intersect_union ();
#endif
}



/* Map: register class x machine mode -> number of hard registers of
   given class needed to store value of given mode.  If the number is
   different, the size will be negative.  */
int reg_class_nregs [N_REG_CLASSES] [MAX_MACHINE_MODE];

/* Maximal value of the previous array elements.  */
int max_nregs;

/* Function forming REG_CLASS_NREGS map.  */
static void
setup_reg_class_nregs (void)
{
  int m;
  enum reg_class cl;

  max_nregs = -1;
  for (cl = 0; cl < N_REG_CLASSES; cl++)
    for (m = 0; m < MAX_MACHINE_MODE; m++)
      {
	reg_class_nregs [cl] [m] = CLASS_MAX_NREGS (cl, m);
	if (max_nregs < reg_class_nregs [cl] [m])
	  max_nregs = reg_class_nregs [cl] [m];
      }
}



/* Array whose values are hard regset of hard registers available for
   the allocation of given register class whose HARD_REGNO_MODE_OK
   values for given mode are zero.  */
HARD_REG_SET prohibited_class_mode_regs [N_REG_CLASSES] [NUM_MACHINE_MODES];

/* The function setting up PROHIBITED_CLASS_MODE_REGS.  */
static void
setup_prohibited_class_mode_regs (void)
{
  int i, j, k, hard_regno;
  enum reg_class cl;

  for (i = 0; i < reg_class_cover_size; i++)
    {
      cl = reg_class_cover [i];
      for (j = 0; j < NUM_MACHINE_MODES; j++)
	{
	  CLEAR_HARD_REG_SET (prohibited_class_mode_regs [cl] [j]);
	  for (k = class_hard_regs_num [cl] - 1; k >= 0; k--)
	    {
	      hard_regno = class_hard_regs [cl] [k];
	      if (! HARD_REGNO_MODE_OK (hard_regno, j))
		SET_HARD_REG_BIT (prohibited_class_mode_regs [cl] [j],
				  hard_regno);
	    }
	}
    }
}



/* The function allocates and initializes REGISTER_MOVE_COST,
   REGISTER_MAY_MOVE_IN_COST, and REGISTER_MAY_MOVE_OUT_COST for MODE
   if it is not done yet.  */
void
init_register_move_cost (enum machine_mode mode)
{
  int cl1, cl2;

  ira_assert (register_move_cost [mode] == NULL
	      && register_may_move_in_cost [mode] == NULL
	      && register_may_move_out_cost [mode] == NULL);
  if (move_cost [mode] == NULL)
    init_move_cost (mode);
  register_move_cost [mode] = move_cost [mode];
  /* Don't use ira_allocate because the tables exist out of scope of a
     IRA call.  */
  register_may_move_in_cost [mode]
    = (move_table *) xmalloc (sizeof (move_table) * N_REG_CLASSES);
  memcpy (register_may_move_in_cost [mode], may_move_in_cost [mode],
	  sizeof (move_table) * N_REG_CLASSES);
  register_may_move_out_cost [mode]
    = (move_table *) xmalloc (sizeof (move_table) * N_REG_CLASSES);
  memcpy (register_may_move_out_cost [mode], may_move_out_cost [mode],
	  sizeof (move_table) * N_REG_CLASSES);
  for (cl1 = 0; cl1 < N_REG_CLASSES; cl1++)
    {
      for (cl2 = 0; cl2 < N_REG_CLASSES; cl2++)
	{
	  if (class_subset_p [cl1] [cl2])
	    register_may_move_in_cost [mode] [cl1] [cl2] = 0;
	  if (class_subset_p [cl2] [cl1])
	    register_may_move_out_cost [mode] [cl1] [cl2] = 0;
	}
    }
}



/* Hard regsets whose all bits are correspondingly zero or one.  */
HARD_REG_SET zero_hard_reg_set;
HARD_REG_SET one_hard_reg_set;

/* The function called once during compiler work.  It sets up
   different arrays whose values don't depend on the compiled
   function.  */
void
init_ira_once (void)
{
  enum machine_mode mode;

  CLEAR_HARD_REG_SET (zero_hard_reg_set);
  SET_HARD_REG_SET (one_hard_reg_set);
  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
    {
      register_move_cost [mode] = NULL;
      register_may_move_in_cost [mode] = NULL;
      register_may_move_out_cost [mode] = NULL;
    }
  init_ira_costs_once ();
}

/* The function frees register_move_cost, register_may_move_in_cost,
   and register_may_move_out_cost for each mode.  */
static void
free_register_move_costs (void)
{
  enum machine_mode mode;

  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
    {
      if (register_may_move_in_cost [mode] != NULL)
	free (register_may_move_in_cost [mode]);
      if (register_may_move_out_cost [mode] != NULL)
	free (register_may_move_out_cost [mode]);
      register_move_cost [mode] = NULL;
      register_may_move_in_cost [mode] = NULL;
      register_may_move_out_cost [mode] = NULL;
    }
}

/* The function called every time when register related information is
   changed.  */
void
init_ira (void)
{
  free_register_move_costs ();
  setup_reg_mode_hard_regset ();
  setup_alloc_regs (flag_omit_frame_pointer != 0);
  setup_class_subset_and_memory_move_costs ();
  find_reg_class_closure ();
  setup_reg_class_nregs ();
  setup_prohibited_class_mode_regs ();
  init_ira_costs ();
}

/* Function called once at the end of compiler work.  */
void
finish_ira_once (void)
{
  finish_ira_costs_once ();
  free_register_move_costs ();
}



/* Array whose values are hard regset of hard registers for which
   move of the hard register in given mode into itself is
   prohibited.  */
HARD_REG_SET prohibited_mode_move_regs [NUM_MACHINE_MODES];

/* Flag of that the above array has been initialized.  */
static int prohibited_mode_move_regs_initialized_p = FALSE;

/* The function setting up PROHIBITED_MODE_MOVE_REGS.  */
static void
setup_prohibited_mode_move_regs (void)
{
  int i, j;
  rtx test_reg1, test_reg2, move_pat, move_insn;

  if (prohibited_mode_move_regs_initialized_p)
    return;
  prohibited_mode_move_regs_initialized_p = TRUE;
  test_reg1 = gen_rtx_REG (VOIDmode, 0);
  test_reg2 = gen_rtx_REG (VOIDmode, 0);
  move_pat = gen_rtx_SET (VOIDmode, test_reg1, test_reg2);
  move_insn = gen_rtx_INSN (VOIDmode, 0, 0, 0, 0, 0, move_pat, -1, 0);
  for (i = 0; i < NUM_MACHINE_MODES; i++)
    {
      SET_HARD_REG_SET (prohibited_mode_move_regs [i]);
      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
	{
	  if (! HARD_REGNO_MODE_OK (j, i))
	    continue;
	  SET_REGNO (test_reg1, j);
	  PUT_MODE (test_reg1, i);
	  SET_REGNO (test_reg2, j);
	  PUT_MODE (test_reg2, i);
	  INSN_CODE (move_insn) = -1;
	  recog_memoized (move_insn);
	  if (INSN_CODE (move_insn) < 0)
	    continue;
	  extract_insn (move_insn);
	  if (! constrain_operands (1))
	    continue;
	  CLEAR_HARD_REG_BIT (prohibited_mode_move_regs [i], j);
	}
    }
}



/* Function specific hard registers that can not be used for the
   register allocation.  */
HARD_REG_SET no_alloc_regs;

/* Return TRUE if *LOC contains an asm.  */
static int
insn_contains_asm_1 (rtx *loc, void *data ATTRIBUTE_UNUSED)
{
  if ( !*loc)
    return 0;
  if (GET_CODE (*loc) == ASM_OPERANDS)
    return TRUE;
  return FALSE;
}


/* Return true if INSN contains an ASM.  */
static int
insn_contains_asm (rtx insn)
{
  return for_each_rtx (&insn, insn_contains_asm_1, NULL);
}

/* Set up regs_asm_clobbered.  */
static void
compute_regs_asm_clobbered (char *regs_asm_clobbered)
{
  basic_block bb;

  memset (regs_asm_clobbered, 0, sizeof (char) * FIRST_PSEUDO_REGISTER);
  
  FOR_EACH_BB (bb)
    {
      rtx insn;
      FOR_BB_INSNS_REVERSE (bb, insn)
	{
	  struct df_ref **def_rec;

	  if (insn_contains_asm (insn))
	    for (def_rec = DF_INSN_DEFS (insn); *def_rec; def_rec++)
	      {
		struct df_ref *def = *def_rec;
		unsigned int dregno = DF_REF_REGNO (def);
		if (dregno < FIRST_PSEUDO_REGISTER)
		  {
		    unsigned int i;
		    enum machine_mode mode = GET_MODE (DF_REF_REAL_REG (def));
		    unsigned int end = dregno 
		      + hard_regno_nregs [dregno] [mode] - 1;

		    for (i = dregno; i <= end; ++i)
		      regs_asm_clobbered [i] = 1;
		  }
	      }
	}
    }
}


/* The function sets up ELIMINABLE_REGSET, NO_ALLOC_REGS, and
   REGS_EVER_LIVE.  */
static void
setup_eliminable_regset (void)
{
  int i;
  /* Like regs_ever_live, but 1 if a reg is set or clobbered from an
     asm.  Unlike regs_ever_live, elements of this array corresponding
     to eliminable regs (like the frame pointer) are set if an asm
     sets them.  */
  char *regs_asm_clobbered = alloca (FIRST_PSEUDO_REGISTER * sizeof (char));
#ifdef ELIMINABLE_REGS
  static const struct {const int from, to; } eliminables[] = ELIMINABLE_REGS;
#endif
  int need_fp
    = (! flag_omit_frame_pointer
       || (current_function_calls_alloca && EXIT_IGNORE_STACK)
       || FRAME_POINTER_REQUIRED);

  COPY_HARD_REG_SET (no_alloc_regs, no_unit_alloc_regs);
  CLEAR_HARD_REG_SET (eliminable_regset);

  compute_regs_asm_clobbered (regs_asm_clobbered);
  /* Build the regset of all eliminable registers and show we can't
     use those that we already know won't be eliminated.  */
#ifdef ELIMINABLE_REGS
  for (i = 0; i < (int) ARRAY_SIZE (eliminables); i++)
    {
      bool cannot_elim
	= (! CAN_ELIMINATE (eliminables [i].from, eliminables [i].to)
	   || (eliminables [i].to == STACK_POINTER_REGNUM && need_fp));

      if (! regs_asm_clobbered [eliminables [i].from])
	{
	    SET_HARD_REG_BIT (eliminable_regset, eliminables [i].from);

	    if (cannot_elim)
	      SET_HARD_REG_BIT (no_alloc_regs, eliminables[i].from);
	}
      else if (cannot_elim)
	error ("%s cannot be used in asm here",
	       reg_names [eliminables [i].from]);
      else
	df_set_regs_ever_live (eliminables[i].from, true);
    }
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
  if (! regs_asm_clobbered [HARD_FRAME_POINTER_REGNUM])
    {
      SET_HARD_REG_BIT (eliminable_regset, HARD_FRAME_POINTER_REGNUM);
      if (need_fp)
	SET_HARD_REG_BIT (no_alloc_regs, HARD_FRAME_POINTER_REGNUM);
    }
  else if (need_fp)
    error ("%s cannot be used in asm here",
	   reg_names [HARD_FRAME_POINTER_REGNUM]);
  else
    df_set_regs_ever_live (HARD_FRAME_POINTER_REGNUM, true);
#endif

#else
  if (! regs_asm_clobbered [FRAME_POINTER_REGNUM])
    {
      SET_HARD_REG_BIT (eliminable_regset, FRAME_POINTER_REGNUM);
      if (need_fp)
	SET_HARD_REG_BIT (no_alloc_regs, FRAME_POINTER_REGNUM);
    }
  else if (need_fp)
    error ("%s cannot be used in asm here", reg_names [FRAME_POINTER_REGNUM]);
  else
    df_set_regs_ever_live (FRAME_POINTER_REGNUM, true);
#endif
}



/* The length of the following two arrays.  */
int reg_equiv_len;

/* The element value is TRUE if the corresponding regno value is
   invariant.  */
int *reg_equiv_invariant_p;

/* The element value is equiv constant of given pseudo-register or
   NULL_RTX.  */
rtx *reg_equiv_const;

/* The function sets up the two array declared above.  */
static void
find_reg_equiv_invariant_const (void)
{
  int i, invariant_p;
  rtx list, insn, note, constant, x;

  for (i = FIRST_PSEUDO_REGISTER; i < reg_equiv_init_size; i++)
    {
      constant = NULL_RTX;
      invariant_p = FALSE;
      for (list = reg_equiv_init [i]; list != NULL_RTX; list = XEXP (list, 1))
	{
	  insn = XEXP (list, 0);
	  note = find_reg_note (insn, REG_EQUIV, NULL_RTX);
	  
	  if (note == NULL_RTX)
	    continue;

	  x = XEXP (note, 0);
	  
	  if (! function_invariant_p (x)
	      || ! flag_pic
	      /* A function invariant is often CONSTANT_P but may
		 include a register.  We promise to only pass CONSTANT_P
		 objects to LEGITIMATE_PIC_OPERAND_P.  */
	      || (CONSTANT_P (x) && LEGITIMATE_PIC_OPERAND_P (x)))
	    {
	      /* It can happen that a REG_EQUIV note contains a MEM
		 that is not a legitimate memory operand.  As later
		 stages of the reload assume that all addresses found
		 in the reg_equiv_* arrays were originally legitimate,
		 we ignore such REG_EQUIV notes.  */
	      if (memory_operand (x, VOIDmode))
		invariant_p = MEM_READONLY_P (x);
	      else if (function_invariant_p (x))
		{
		  if (GET_CODE (x) == PLUS
		      || x == frame_pointer_rtx || x == arg_pointer_rtx)
		    invariant_p = TRUE;
		  else
		    constant = x;
		}
	    }
	}
      reg_equiv_invariant_p [i] = invariant_p;
      reg_equiv_const [i] = constant;
    }
}



/* The function sets up REG_RENUMBER and CALLER_SAVE_NEEDED (used by
   the reload) from the allocation found by IRA.  */
static void
setup_reg_renumber (void)
{
  int regno, hard_regno;
  allocno_t a;
  allocno_iterator ai;

  caller_save_needed = 0;
  FOR_EACH_ALLOCNO (a, ai)
    {
      /* There are no caps at this point.  */
      ira_assert (ALLOCNO_CAP_MEMBER (a) == NULL);
      if (! ALLOCNO_ASSIGNED_P (a))
	/* It can happen if A is not referenced but partially anticipated
	   somewhere in a region.  */
	ALLOCNO_ASSIGNED_P (a) = TRUE;
      free_allocno_updated_costs (a);
      hard_regno = ALLOCNO_HARD_REGNO (a);
      regno = (int) REGNO (ALLOCNO_REG (a));
      reg_renumber [regno] = (hard_regno < 0 ? -1 : hard_regno);
      if (hard_regno >= 0 && ALLOCNO_CALLS_CROSSED_NUM (a) != 0
	  && ! hard_reg_not_in_set_p (hard_regno, ALLOCNO_MODE (a),
				      call_used_reg_set))
	{
	  ira_assert (flag_caller_saves || regno >= reg_equiv_len
		      || reg_equiv_const [regno]
		      || reg_equiv_invariant_p [regno]);
	  caller_save_needed = 1;
	}
    }
}

/* The function sets up allocno assignment flags for further
   allocation improvements.  */
static void
setup_allocno_assignment_flags (void)
{
  int hard_regno;
  allocno_t a;
  allocno_iterator ai;

  FOR_EACH_ALLOCNO (a, ai)
    {
      if (! ALLOCNO_ASSIGNED_P (a))
	/* It can happen if A is not referenced but partially anticipated
	   somewhere in a region.  */
	free_allocno_updated_costs (a);
      hard_regno = ALLOCNO_HARD_REGNO (a);
      /* Don't assign hard registers to allocnos which are destination
	 of removed store at the end of loop.  It has no sense to keep
	 the same value in different hard registers.  It is also
	 impossible to assign hard registers correctly to such
	 allocnos because the cost info and info about intersected
	 calls are incorrect for them.  */
      ALLOCNO_ASSIGNED_P (a) = (hard_regno >= 0
				|| ALLOCNO_MEM_OPTIMIZED_DEST_P (a));
      ira_assert (hard_regno < 0
		  || ! hard_reg_not_in_set_p (hard_regno, ALLOCNO_MODE (a),
					      reg_class_contents
					      [ALLOCNO_COVER_CLASS (a)]));
    }
}

/* The function evaluates overall allocation cost and costs for using
   hard registers and memory for allocnos.  */
static void
calculate_allocation_cost (void)
{
  int hard_regno, cost;
  allocno_t a;
  allocno_iterator ai;

  overall_cost = reg_cost = mem_cost = 0;
  FOR_EACH_ALLOCNO (a, ai)
    {
      hard_regno = ALLOCNO_HARD_REGNO (a);
      ira_assert (hard_regno < 0
		  || ! hard_reg_not_in_set_p
		       (hard_regno, ALLOCNO_MODE (a),
			reg_class_contents [ALLOCNO_COVER_CLASS (a)])); 
      if (hard_regno < 0)
	{
	  cost = ALLOCNO_MEMORY_COST (a);
	  mem_cost += cost;
	}
      else if (ALLOCNO_HARD_REG_COSTS (a) != NULL)
	{
	  cost = (ALLOCNO_HARD_REG_COSTS (a)
		  [class_hard_reg_index
		   [ALLOCNO_COVER_CLASS (a)] [hard_regno]]);
	  reg_cost += cost;
	}
      else
	{
	  cost = ALLOCNO_COVER_CLASS_COST (a);
	  reg_cost += cost;
	}
      overall_cost += cost;
    }

  if (internal_flag_ira_verbose > 0 && ira_dump_file != NULL)
    {
      fprintf (ira_dump_file,
	       "+++Costs: overall %d, reg %d, mem %d, ld %d, st %d, move %d\n",
	       overall_cost, reg_cost, mem_cost,
	       load_cost, store_cost, shuffle_cost);
      fprintf (ira_dump_file, "+++       move loops %d, new jumps %d\n",
	       move_loops_num, additional_jumps_num);
    }

}

#ifdef ENABLE_IRA_CHECKING
/* The function checks correctness of the allocation.  We do need this
   because of complicated code to transform more one region internal
   representation into one region representation.  */
static void
check_allocation (void)
{
  allocno_t a, conflict_a;
  int hard_regno, conflict_hard_regno, nregs, conflict_nregs;
  allocno_conflict_iterator aci;
  allocno_iterator ai;

  FOR_EACH_ALLOCNO (a, ai)
    {
      if (ALLOCNO_CAP_MEMBER (a) != NULL
	  || (hard_regno = ALLOCNO_HARD_REGNO (a)) < 0)
	continue;
      nregs = hard_regno_nregs [hard_regno] [ALLOCNO_MODE (a)];
      FOR_EACH_ALLOCNO_CONFLICT (a, conflict_a, aci)
	if ((conflict_hard_regno = ALLOCNO_HARD_REGNO (conflict_a)) >= 0)
	  {
	    conflict_nregs
	      = (hard_regno_nregs
		 [conflict_hard_regno] [ALLOCNO_MODE (conflict_a)]);
	    if ((conflict_hard_regno <= hard_regno
		 && hard_regno < conflict_hard_regno + conflict_nregs)
		|| (hard_regno <= conflict_hard_regno
		    && conflict_hard_regno < hard_regno + nregs))
	      {
		fprintf (stderr, "bad allocation for %d and %d\n",
			 ALLOCNO_REGNO (a), ALLOCNO_REGNO (conflict_a));
		gcc_unreachable ();
	      }
	  }
    }
}
#endif

/* The function fixes values of array REG_EQUIV_INIT after live range
   splitting done by IRA.  */
static void
fix_reg_equiv_init (void)
{
  int max_regno = max_reg_num ();
  int i, new_regno;
  rtx x, prev, next, insn, set;
  
  
  if (reg_equiv_init_size < max_regno)
    {
      reg_equiv_init = ggc_realloc (reg_equiv_init, max_regno * sizeof (rtx));
      while (reg_equiv_init_size < max_regno)
	reg_equiv_init [reg_equiv_init_size++] = NULL_RTX;
      for (i = FIRST_PSEUDO_REGISTER; i < reg_equiv_init_size; i++)
	for (prev = NULL_RTX, x = reg_equiv_init [i]; x != NULL_RTX; x = next)
	  {
	    next = XEXP (x, 1);
	    insn = XEXP (x, 0);
	    set = single_set (insn);
	    ira_assert (set != NULL_RTX
			&& (REG_P (SET_DEST (set)) || REG_P (SET_SRC (set))));
	    if (REG_P (SET_DEST (set))
		&& ((int) REGNO (SET_DEST (set)) == i
		    || (int) ORIGINAL_REGNO (SET_DEST (set)) == i))
	      new_regno = REGNO (SET_DEST (set));
	    else if (REG_P (SET_SRC (set))
		     && ((int) REGNO (SET_SRC (set)) == i
			 || (int) ORIGINAL_REGNO (SET_SRC (set)) == i))
	      new_regno = REGNO (SET_SRC (set));
	    else
 	      gcc_unreachable ();
	    if (new_regno == i)
	      prev = x;
	    else
	      {
		if (prev == NULL_RTX)
		  reg_equiv_init [i] = next;
		else
		  XEXP (prev, 1) = next;
		XEXP (x, 1) = reg_equiv_init [new_regno];
		reg_equiv_init [new_regno] = x;
	      }
	  }
    }
}

#ifdef ENABLE_IRA_CHECKING
/* The function prints redundant memory-memory copies.  */
static void
print_redundant_copies (void)
{
  int hard_regno;
  allocno_t a;
  copy_t cp, next_cp;
  allocno_iterator ai;
  
  FOR_EACH_ALLOCNO (a, ai)
    {
      if (ALLOCNO_CAP_MEMBER (a) != NULL)
	/* It is a cap. */
	continue;
      hard_regno = ALLOCNO_HARD_REGNO (a);
      if (hard_regno >= 0)
	continue;
      for (cp = ALLOCNO_COPIES (a); cp != NULL; cp = next_cp)
	if (cp->first == a)
	  next_cp = cp->next_first_allocno_copy;
	else
	  {
	    next_cp = cp->next_second_allocno_copy;
	    if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL
		&& cp->insn != NULL_RTX
		&& ALLOCNO_HARD_REGNO (cp->first) == hard_regno)
	      fprintf (ira_dump_file,
		       "        Redundant move from %d(freq %d):%d\n",
		       INSN_UID (cp->insn), cp->freq, hard_regno);
	  }
    }
}
#endif

/* Setup preferred and alternative classes for pseudo-registers for
   other passes.  */
static void
setup_preferred_alternate_classes (void)
{
  enum reg_class cover_class;
  allocno_t a;
  allocno_iterator ai;

  FOR_EACH_ALLOCNO (a, ai)
    {
      cover_class = ALLOCNO_COVER_CLASS (a);
      if (cover_class == NO_REGS)
	cover_class = GENERAL_REGS;
      setup_reg_classes (ALLOCNO_REGNO (a), cover_class, NO_REGS);
    }
}



/* Regional allocation can create new pseudo-registers.  The function
   expands some arrays for pseudo-registers.  */
static void
expand_reg_info (int old_size)
{
  int i;
  int size = max_reg_num ();

  resize_reg_info ();
  for (i = old_size; i < size; i++)
    {
      reg_renumber [i] = -1;
      setup_reg_classes (i, GENERAL_REGS, ALL_REGS);
    }
}



/* This page contains code for sorting the insn chain used by the
   reload.  In old register allocator, the insn chain order
   corresponds to the order of insns in RTL.  By putting insns with
   higher execution frequency first, the reload has a better chance to
   generate less expensive operand reloads for such insns.  */

/* Map bb index -> order number in the BB chain in RTL code.  */
static int *basic_block_order_nums;

/* The function is used to sort insn chain according insn execution
   frequencies.  */
static int
chain_freq_compare (const void *v1p, const void *v2p)
{
  struct insn_chain *c1 = *(struct insn_chain **)v1p;
  struct insn_chain *c2 = *(struct insn_chain **)v2p;
  int diff;

  diff = (BASIC_BLOCK (c2->block)->frequency
	  - BASIC_BLOCK (c1->block)->frequency);
  if (diff)
    return diff;
  return (char *) v1p - (char *) v2p;
}

/* The function is used to sort insn chain according insn original
   order.  */
static int
chain_bb_compare (const void *v1p, const void *v2p)
{
  struct insn_chain *c1 = *(struct insn_chain **)v1p;
  struct insn_chain *c2 = *(struct insn_chain **)v2p;
  int diff;

  diff = (basic_block_order_nums [c1->block]
	  - basic_block_order_nums [c2->block]);
  if (diff)
    return diff;
  return (char *) v1p - (char *) v2p;
}

/* The function sorts insn chain according to insn frequencies if
   FREQ_P or according to insn original order otherwise.  */
void
sort_insn_chain (int freq_p)
{
  struct insn_chain *chain, **chain_arr;
  basic_block bb;
  int i, n;
  
  for (n = 0, chain = reload_insn_chain; chain != 0; chain = chain->next)
    n++;
  if (n <= 1)
    return;
  chain_arr = ira_allocate (n * sizeof (struct insn_chain *));
  basic_block_order_nums = ira_allocate (sizeof (int) * last_basic_block);
  n = 0;
  FOR_EACH_BB (bb)
    {
      basic_block_order_nums [bb->index] = n++;
    }
  for (n = 0, chain = reload_insn_chain; chain != 0; chain = chain->next)
    chain_arr [n++] = chain;
  qsort (chain_arr, n, sizeof (struct insn_chain *),
	 freq_p ? chain_freq_compare : chain_bb_compare);
  for (i = 1; i < n - 1; i++)
    {
      chain_arr [i]->next = chain_arr [i + 1];
      chain_arr [i]->prev = chain_arr [i - 1];
    }
  chain_arr [i]->next = NULL;
  chain_arr [i]->prev = chain_arr [i - 1];
  reload_insn_chain = chain_arr [0];
  reload_insn_chain->prev = NULL;
  reload_insn_chain->next = chain_arr [1];
  ira_free (basic_block_order_nums);
  ira_free (chain_arr);
}



/* All natural loops.  */
struct loops ira_loops;

/* This is the main entry of IRA.  */
static void
ira (FILE *f)
{
  int overall_cost_before, loops_p, allocated_reg_info_size;
  int max_regno_before_ira, max_point_before_emit;
  int rebuild_p, saved_flag_ira_algorithm;
  basic_block bb;

  if (flag_ira_verbose < 10)
    {
      internal_flag_ira_verbose = flag_ira_verbose;
      ira_dump_file = f;
    }
  else
    {
      internal_flag_ira_verbose = flag_ira_verbose - 10;
      ira_dump_file = stderr;
    }

  setup_prohibited_mode_move_regs ();

  df_note_add_problem ();

  if (optimize == 1)
    {
      df_live_add_problem ();
      df_live_set_all_dirty ();
    }
  df_analyze ();

  df_clear_flags (DF_NO_INSN_RESCAN);

  regstat_init_n_sets_and_refs ();
  regstat_compute_ri ();

  /* If we are not optimizing, then this is the only place before
     register allocation where dataflow is done.  And that is needed
     to generate these warnings.  */
  if (warn_clobbered)
    generate_setjmp_warnings ();

  rebuild_p = update_equiv_regs ();
  regstat_free_n_sets_and_refs ();
  regstat_free_ri ();
    
#ifndef IRA_NO_OBSTACK
  gcc_obstack_init (&ira_obstack);
#endif
  bitmap_obstack_initialize (&ira_bitmap_obstack);

  max_regno = max_reg_num ();
  reg_equiv_len = max_regno;
  reg_equiv_invariant_p = ira_allocate (max_regno * sizeof (int));
  memset (reg_equiv_invariant_p, 0, max_regno * sizeof (int));
  reg_equiv_const = ira_allocate (max_regno * sizeof (rtx));
  memset (reg_equiv_const, 0, max_regno * sizeof (rtx));
  find_reg_equiv_invariant_const ();
  if (rebuild_p)
    {
      timevar_push (TV_JUMP);
      rebuild_jump_labels (get_insns ());
      purge_all_dead_edges ();
      timevar_pop (TV_JUMP);
    }
  max_regno_before_ira = allocated_reg_info_size = max_reg_num ();
  allocate_reg_info ();
  setup_eliminable_regset ();

  overall_cost = reg_cost = mem_cost = 0;
  load_cost = store_cost = shuffle_cost = 0;
  move_loops_num = additional_jumps_num = 0;

  ira_assert (current_loops == NULL);
  flow_loops_find (&ira_loops);
  current_loops = &ira_loops;
  saved_flag_ira_algorithm = flag_ira_algorithm;
  if (number_of_loops () > (unsigned) IRA_MAX_LOOPS_NUM)
    flag_ira_algorithm = IRA_ALGORITHM_CB;

  if (internal_flag_ira_verbose > 0 && ira_dump_file != NULL)
    fprintf (ira_dump_file, "Building IRA IR\n");
  loops_p = ira_build (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL
		       || flag_ira_algorithm == IRA_ALGORITHM_MIXED);
  ira_color ();

  max_point_before_emit = max_point;

  ira_emit (loops_p);

  max_regno = max_reg_num ();
  
  if (! loops_p)
    initiate_ira_assign ();
  else
    {
      expand_reg_info (allocated_reg_info_size);
      allocated_reg_info_size = max_regno;
 
      if (internal_flag_ira_verbose > 0 && ira_dump_file != NULL)
	fprintf (ira_dump_file, "Flattening IR\n");
      ira_flattening (max_regno_before_ira, max_point_before_emit);
      /* New insns were generated: add notes and recalculate live
	 info.  */
      df_analyze ();

      {
	basic_block bb;
	
	FOR_ALL_BB (bb)
	  bb->loop_father = NULL;
	current_loops = NULL;
      }

      setup_allocno_assignment_flags ();
      initiate_ira_assign ();
      reassign_conflict_allocnos (max_regno);
    }

  setup_reg_renumber ();

  calculate_allocation_cost ();

#ifdef ENABLE_IRA_CHECKING
  check_allocation ();
#endif

  setup_preferred_alternate_classes ();

  delete_trivially_dead_insns (get_insns (), max_reg_num ());
  max_regno = max_reg_num ();
  
  /* Determine if the current function is a leaf before running IRA
     since this can impact optimizations done by the prologue and
     epilogue thus changing register elimination offsets.  */
  current_function_is_leaf = leaf_function_p ();
  
  /* And the reg_equiv_memory_loc array.  */
  VEC_safe_grow (rtx, gc, reg_equiv_memory_loc_vec, max_regno);
  memset (VEC_address (rtx, reg_equiv_memory_loc_vec), 0,
	  sizeof (rtx) * max_regno);
  reg_equiv_memory_loc = VEC_address (rtx, reg_equiv_memory_loc_vec);
  
  allocate_initial_values (reg_equiv_memory_loc);
  
  regstat_init_n_sets_and_refs ();
  regstat_compute_ri ();

  fix_reg_equiv_init ();

#ifdef ENABLE_IRA_CHECKING
  print_redundant_copies ();
#endif

  overall_cost_before = overall_cost;

  spilled_reg_stack_slots_num = 0;
  spilled_reg_stack_slots
    = ira_allocate (max_regno * sizeof (struct spilled_reg_stack_slot));
  memset (spilled_reg_stack_slots, 0,
	  max_regno * sizeof (struct spilled_reg_stack_slot));

  df_set_flags (DF_NO_INSN_RESCAN);
  build_insn_chain ();
  sort_insn_chain (TRUE);
  reload_completed = ! reload (get_insns (), 1);

  ira_free (spilled_reg_stack_slots);

  finish_ira_assign ();

  if (internal_flag_ira_verbose > 0 && ira_dump_file != NULL
      && overall_cost_before != overall_cost)
    fprintf (ira_dump_file, "+++Overall after reload %d\n", overall_cost);

  ira_destroy ();

  flow_loops_free (&ira_loops);
  free_dominance_info (CDI_DOMINATORS);
  FOR_ALL_BB (bb)
    bb->loop_father = NULL;
  current_loops = NULL;

  flag_ira_algorithm = saved_flag_ira_algorithm;

  cleanup_cfg (CLEANUP_EXPENSIVE);

  regstat_free_ri ();
  regstat_free_n_sets_and_refs ();

  ira_free (reg_equiv_invariant_p);
  ira_free (reg_equiv_const);

  bitmap_obstack_release (&ira_bitmap_obstack);
#ifndef IRA_NO_OBSTACK
  obstack_free (&ira_obstack, NULL);
#endif
  
  /* The code after the reload has changed so much that at this point
     we might as well just rescan everything.  Not that
     df_rescan_all_insns is not going to help here because it does not
     touch the artificial uses and defs.  */
  df_finish_pass (true);
  if (optimize > 1)
    df_live_add_problem ();
  df_scan_alloc (NULL);
  df_scan_blocks ();

  if (optimize)
    df_analyze ();
}



static bool
gate_ira (void)
{
  return flag_ira != 0;
}

/* Run the integrated register allocator.  */
static unsigned int
rest_of_handle_ira (void)
{
  ira (dump_file);
  return 0;
}

struct tree_opt_pass pass_ira =
{
  "ira",                                /* name */
  gate_ira,                             /* gate */
  rest_of_handle_ira,		        /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_IRA,	                        /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'y'                                   /* letter */
};
