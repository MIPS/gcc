/* Integrated Register Allocator entry point.
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

/* The integrated register allocator (IRA) is called integrated
   because register coalescing and register live range splitting are
   done on-the-fly during coloring.  Register coalescing is done by
   hard register preferencing during hard register assigning.  The
   live range splitting is a byproduct of the regional register
   allocation.

   The regional allocation is top-down process.  The first we do
   allocation for all function then we improve it for loops then their
   subloops and so on.  To reduce register shuffling, the same
   mechanism of hard register prefrencing is used.  This approach
   works as good as Callahan-Koblentz algorithm but it is simpler.
   We use Chaitin-Briggs coloring for each loop (or function) with
   optional biased coloring.  If pseudo-registers got different
   location on loop borders we rename them inside the loop and
   generate pseudo-register move insns.  Some optimizations (like
   removing redundant stores, moving register shuffling to less
   frequent points, and code duplication reducing) to minimize effect
   of register shuffling is done

   If we don't improve register allocation for loops we get classic
   Chaitin-Briggs coloring (only instead of separate pass of
   coalescing, we use hard register preferencing).

   Optionally we implements Chow's priority coloring only for all
   function.  It is quite analogous to the current gcc global register
   allocator only we use more sophisticated hard register
   preferencing.

   Literature is worth to read for better understanding the code:

   o Preston Briggs, Keith D. Cooper, Linda Torczon.  Improvements to
     Graph Coloring Register Allocation.

   o David Callahan, Brian Koblenz.  Register allocation via
     hierarchical graph coloring

   o Keith Cooper, Anshuman Dasgupta, Jason Eckhardt. Revisiting Graph
     Coloring Register Allocation: A Study of the Chaitin-Briggs and
     Callahan-Koblenz Algorithms.

   o Guei-Yuan Lueh, Thomas Gross, and Ali-Reza Adl-Tabatabai. Global
     Register Allocation Based on Graph Fusion.

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

static void setup_inner_mode (void);
static void setup_reg_mode_hard_regset (void);
static void setup_class_subset_and_move_costs (void);
static void setup_class_hard_regs (void);
static void setup_available_class_regs (void);
static void setup_alloc_regs (int);
static void setup_reg_subclasses (void);
static void setup_cover_classes (void);
static void setup_class_translate (void);
static void print_class_cover (FILE *);
static void find_reg_class_closure (void);
static void setup_reg_class_nregs (void);
static void setup_prohibited_class_mode_regs (void);
static void setup_eliminable_regset (void);
static void find_reg_equiv_invariant_const (void);
static void setup_reg_renumber (void);
static void calculate_allocation_cost (void);
#ifdef ENABLE_IRA_CHECKING
static void check_allocation (void);
#endif
static void fix_reg_equiv_init (void);
#ifdef ENABLE_IRA_CHECKING
static void print_redundant_copies (void);
#endif
static void setup_preferred_alternate_classes (void);

static bool gate_ira (void);
static unsigned int rest_of_handle_ira (void);

/* Dump file for IRA.  */
FILE *ira_dump_file;

/* The number of elements in the following array.  */
int spilled_reg_stack_slots_num;

/* The following array contains description of spilled registers stack
   slots have been used in the current function so far.  */
struct spilled_reg_stack_slot *spilled_reg_stack_slots;

/* The following variable values are correspondingly overall cost of
   the allocation, cost of hard register usage for the pseudos, cost
   of memory usage for the pseudos, cost of loads, stores and register
   move insns generated for register live range splitting.  */
int overall_cost;
int reg_cost, mem_cost;
int load_cost, store_cost, shuffle_cost;
int move_loops_num, additional_jumps_num;

/* A mode whose value is immediately contained in given mode
   value.  */
unsigned char mode_inner_mode [NUM_MACHINE_MODES];

/* The following array is a map hard regs X modes -> number registers
   for store value of given mode starting with given hard
   register.  */
HARD_REG_SET reg_mode_hard_regset [FIRST_PSEUDO_REGISTER] [NUM_MACHINE_MODES];

/* The following two variables are array analog of macros
   MEMORY_MOVE_COST and REGISTER_MOVE_COST.  */
int memory_move_cost [MAX_MACHINE_MODE] [N_REG_CLASSES] [2];
int register_move_cost [MAX_MACHINE_MODE] [N_REG_CLASSES] [N_REG_CLASSES];

/* Nonzero value of element of the following array means that the
   1st class is a subset of the 2nd class.  */
int class_subset_p [N_REG_CLASSES] [N_REG_CLASSES];

/* Temporary hard reg set used for different calculation.  */
static HARD_REG_SET temp_hard_regset;



/* The function sets up mode_inner_mode array.  */
static void
setup_inner_mode (void)
{
  int i;
  enum machine_mode wider;

  for (i = 0; i < NUM_MACHINE_MODES; i++)
    mode_inner_mode [i] = VOIDmode;
  for (i = 0; i < NUM_MACHINE_MODES; i++)
    {
      wider = GET_MODE_WIDER_MODE (i);
      if (wider != VOIDmode)
	{
	  ira_assert (mode_inner_mode [wider] == VOIDmode);
	  mode_inner_mode [wider] = i;
	}
    }
}



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



/* The function sets up MEMORY_MOVE_COST, REGISTER_MOVE_COST and
   CLASS_SUBSET_P.  */
static void
setup_class_subset_and_move_costs (void)
{
  int cl, cl2;
  enum machine_mode mode;

  for (cl = (int) N_REG_CLASSES - 1; cl >= 0; cl--)
    {
      for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
	{
	  memory_move_cost [mode] [cl] [0] = MEMORY_MOVE_COST (mode, cl, 0);
	  memory_move_cost [mode] [cl] [1] = MEMORY_MOVE_COST (mode, cl, 1);
	}

      for (cl2 = (int) N_REG_CLASSES - 1; cl2 >= 0; cl2--)
	{
	  if (cl != NO_REGS && cl2 != NO_REGS)
	    for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
	      register_move_cost [mode] [cl] [cl2]
		= REGISTER_MOVE_COST (mode, cl, cl2);
	  GO_IF_HARD_REG_SUBSET (reg_class_contents[cl],
				 reg_class_contents[cl2],
				 subset);
	  class_subset_p [cl] [cl2] = FALSE;
	  continue;
	  
	subset:
	  class_subset_p [cl] [cl2] = TRUE;
	}
    }
}



/* Hard registers which can be used for the allocation of given
   register class.  The order is defined by the allocation order.  */
short class_hard_regs [N_REG_CLASSES] [FIRST_PSEUDO_REGISTER];

/* The size of the above array for given register class.  */
int class_hard_regs_num [N_REG_CLASSES];

/* Index (in class_hard_regs) for given register class and hard
   register (in general case a hard register can belong to several
   register classes).  */
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
      AND_COMPL_HARD_REG_SET (temp_hard_regset, no_alloc_regs);
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

/* Number of class hard registers available for the register
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
      AND_COMPL_HARD_REG_SET (temp_hard_regset, no_alloc_regs);
      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
	if (TEST_HARD_REG_BIT (temp_hard_regset, j))
	  available_class_regs [i]++;
    }
}

/* Hard registers can not be used for the register allocator.  */
HARD_REG_SET no_alloc_regs;

/* The function setting up different global variables defining hard
   registers for the allocation.  It depends on USE_HARD_FRAME_P whose
   nonzero value means that we can use hard frame pointer for the
   allocation.  */
static void
setup_alloc_regs (int use_hard_frame_p)
{
  COPY_HARD_REG_SET (no_alloc_regs, fixed_reg_set);
  if (! use_hard_frame_p)
    SET_HARD_REG_BIT (no_alloc_regs, HARD_FRAME_POINTER_REGNUM);
  setup_class_hard_regs ();
  setup_available_class_regs ();
}



/* Define the following macro if allocation through malloc if
   preferable.  */
/*#define IRA_NO_OBSTACK*/

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


/* The function allocates bitmap for IRA.  */
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

/* The function allocates regset for IRA.  */
regset
ira_allocate_regset (void)
{
  return ALLOC_REG_SET (&ira_bitmap_obstack);
}

/* The function frees regset R allocated for IRA.  */
void
ira_free_regset (regset r ATTRIBUTE_UNUSED)
{
  /* do nothing */
}



/* The function returns nonzero if hard registers starting with
   HARD_REGNO and containing value of MODE are not in set
   HARD_REGSET.  */
int
hard_reg_not_in_set_p (int hard_regno, enum machine_mode mode,
		       HARD_REG_SET hard_regset)
{
  int i;

  ira_assert (hard_regno >= 0);
  for (i = hard_regno_nregs [hard_regno] [mode] - 1; i >= 0; i--)
    if (TEST_HARD_REG_BIT (hard_regset, hard_regno + i))
      return FALSE;
  return TRUE;
}



/* The function outputs information about allocation of all pseudos
   into file F.  */
void
print_disposition (FILE *f)
{
  int i, n, max_regno;
  pseudo_t p;
  basic_block bb;

  fprintf (f, "Disposition:");
  max_regno = max_reg_num ();
  for (n = 0, i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    for (p = regno_pseudo_map [i]; p != NULL; p = PSEUDO_NEXT_REGNO_PSEUDO (p))
      {
	if (n % 4 == 0)
	  fprintf (f, "\n");
	n++;
	fprintf (f, " %4d:r%-4d", PSEUDO_NUM (p), PSEUDO_REGNO (p));
	if ((bb = PSEUDO_LOOP_TREE_NODE (p)->bb) != NULL)
	  fprintf (f, "b%-3d", bb->index);
	else
	  fprintf (f, "l%-3d", PSEUDO_LOOP_TREE_NODE (p)->loop->num);
	if (PSEUDO_HARD_REGNO (p) >= 0)
	  fprintf (f, " %3d", PSEUDO_HARD_REGNO (p));
	else
	  fprintf (f, " mem");
      }
  fprintf (f, "\n");
}

/* The function outputs information about allocation of all pseudos
   into stderr.  */
void
debug_disposition (void)
{
  print_disposition (stderr);
}



/* For each reg class, table listing all the classes contained in it
   (excluding the class itself.  Fixed registers are excluded from the
   consideration).  */
static enum reg_class alloc_reg_class_subclasses[N_REG_CLASSES][N_REG_CLASSES];

/* The function initializes the tables of subclasses of each reg
   class.  */
static void
setup_reg_subclasses (void)
{
  int i, j;

  for (i = 0; i < N_REG_CLASSES; i++)
    for (j = 0; j < N_REG_CLASSES; j++)
      alloc_reg_class_subclasses [i] [j] = LIM_REG_CLASSES;

  for (i = 0; i < N_REG_CLASSES; i++)
    {
      if (i == (int) NO_REGS)
	continue;

      COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [i]);
      AND_COMPL_HARD_REG_SET (temp_hard_regset, fixed_reg_set);
      GO_IF_HARD_REG_EQUAL (temp_hard_regset, zero_hard_reg_set, cont);
      goto ok;
    cont:
      continue;
    ok:
      for (j = 0; j < N_REG_CLASSES; j++)
	if (i != j)
	  {
	    enum reg_class *p;

	    GO_IF_HARD_REG_SUBSET (reg_class_contents [i],
				   reg_class_contents [j], subclass);
	    continue;
	  subclass:
	    p = &alloc_reg_class_subclasses [j] [0];
	    while (*p != LIM_REG_CLASSES) p++;
	    *p = (enum reg_class) i;
	  }
    }
}



/* The value is size of the subsequent array.  */
int reg_class_cover_size;

/* The array containing cover classes whose hard registers are used
   for the allocation -- see also comments for macro
   IRA_COVER_CLASSES.  */
enum reg_class reg_class_cover [N_REG_CLASSES];


/* The function checks IRA_COVER_CLASSES and sets the two global
   variables defined above.  */
static void
setup_cover_classes (void)
{
  int i, j;
  enum reg_class cl;
  static enum reg_class classes [] = IRA_COVER_CLASSES;

  reg_class_cover_size = 0;
  for (i = 0; (cl = classes [i]) != LIM_REG_CLASSES; i++)
    {
      for (j = 0; j < i; j++)
	if (reg_classes_intersect_p (cl, classes [j]))
	  gcc_unreachable ();
      COPY_HARD_REG_SET (temp_hard_regset, reg_class_contents [cl]);
      AND_COMPL_HARD_REG_SET (temp_hard_regset, fixed_reg_set);
      GO_IF_HARD_REG_EQUAL (temp_hard_regset, zero_hard_reg_set, cont);
      reg_class_cover [reg_class_cover_size++] = cl;
    cont:
      ;
    }
}

/* Map of register classes to corresponding cover class containing the
   given class.  If given class is not a subset of a cover class, we
   translate it into the cheapest cover class.  */
enum reg_class class_translate [N_REG_CLASSES];

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
	      AND_COMPL_HARD_REG_SET (temp_hard_regset, fixed_reg_set);
	      GO_IF_HARD_REG_SUBSET (temp_hard_regset, zero_hard_reg_set, ok);
	      gcc_unreachable ();
	    ok:
	      ;
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
	  GO_IF_HARD_REG_EQUAL (temp_hard_regset, zero_hard_reg_set,
				no_intersection);
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
	no_intersection:
	  ;
	}
      class_translate [cl] = best_class;
    }
}

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

/* Function setting up different arrays concerning class subsets and
   cover classes.  */
static void
find_reg_class_closure (void)
{
  setup_reg_subclasses ();
  setup_cover_classes ();
  setup_class_translate ();
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



/* Array whose values are hard regset of hard registers of given
   register class whose HARD_REGNO_MODE_OK values are zero.  */
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



/* Hard regsets whose all bits are correspondingly zero or one.  */
HARD_REG_SET zero_hard_reg_set;
HARD_REG_SET one_hard_reg_set;

/* Function called once during compiler work.  It sets up different
   arrays whose values don't depend on the compiled function.  */
void
init_ira_once (void)
{
  CLEAR_HARD_REG_SET (zero_hard_reg_set);
  SET_HARD_REG_SET (one_hard_reg_set);
  setup_inner_mode ();
  setup_reg_mode_hard_regset ();
  setup_class_subset_and_move_costs ();
  setup_alloc_regs (flag_omit_frame_pointer != 0);
  find_reg_class_closure ();
  setup_reg_class_nregs ();
  setup_prohibited_class_mode_regs ();
  init_ira_costs_once ();
}



/* The function sets up ELIMINABLE_REGSET and REGS_EVER_LIVE.  */
static void
setup_eliminable_regset (void)
{
  int i;
#ifdef ELIMINABLE_REGS
  static const struct {const int from, to; } eliminables[] = ELIMINABLE_REGS;
#endif
  int need_fp
    = (! flag_omit_frame_pointer
       || (current_function_calls_alloca && EXIT_IGNORE_STACK)
       || FRAME_POINTER_REQUIRED);

  CLEAR_HARD_REG_SET (eliminable_regset);
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
	  if (! cannot_elim)
	    SET_HARD_REG_BIT (eliminable_regset, eliminables [i].from);
	}
      else if (cannot_elim)
	error ("%s cannot be used in asm here",
	       reg_names [eliminables [i].from]);
      else
	regs_ever_live [eliminables [i].from] = 1;
    }
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
  if (! regs_asm_clobbered [HARD_FRAME_POINTER_REGNUM])
    {
      if (! need_fp)
	SET_HARD_REG_BIT (eliminable_regset, HARD_FRAME_POINTER_REGNUM);
    }
  else if (need_fp)
    error ("%s cannot be used in asm here",
	   reg_names [HARD_FRAME_POINTER_REGNUM]);
  else
    regs_ever_live [HARD_FRAME_POINTER_REGNUM] = 1;
#endif

#else
  if (! regs_asm_clobbered [FRAME_POINTER_REGNUM])
    {
      if (! need_fp)
	SET_HARD_REG_BIT (eliminable_regset, FRAME_POINTER_REGNUM);
    }
  else if (need_fp)
    error ("%s cannot be used in asm here", reg_names [FRAME_POINTER_REGNUM]);
  else
    regs_ever_live [FRAME_POINTER_REGNUM] = 1;
#endif
}



/* The element value is nonzero if the corresponding regno value is
   invariant.  */
int *reg_equiv_invariant_p;

/* The element value is equiv constant or NULL_RTX.  */
rtx *reg_equiv_const;

/* The function sets up the two array declaraed above.  */
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
	      /* It can happen that a REG_EQUIV note contains a MEM that
		 is not a legitimate memory operand.  As later stages of
		 reload assume that all addresses found in the
		 reg_equiv_* arrays were originally legitimate, we
		 ignore such REG_EQUIV notes.  */
	      if (memory_operand (x, VOIDmode))
		continue;
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



/* The function sets up REG_RENUMBER and CALLER_SAVE_NEEDED used by
   reload from the allocation found by IRA.  */
static void
setup_reg_renumber (void)
{
  int i, hard_regno;
  pseudo_t p;

  for (i = 0; i < pseudos_num; i++)
    {
      p = pseudos [i];
      if (PSEUDO_REGNO (p) < 0)
	/* It is a cap. */
	continue;
      hard_regno = PSEUDO_HARD_REGNO (p);
      reg_renumber [REGNO (PSEUDO_REG (p))]
	= (hard_regno < 0 ? -1 : hard_regno);
      if (hard_regno >= 0 && PSEUDO_CALLS_CROSSED_NUM (p) != 0
	  && ! hard_reg_not_in_set_p (hard_regno, PSEUDO_MODE (p),
				      call_used_reg_set))
	{
	  ira_assert (flag_caller_saves);
	  caller_save_needed = 1;
	}
    }
}

/* The function calculates cost of the found register allocation.  */
static void
calculate_allocation_cost (void)
{
  int i, hard_regno, cost;
  pseudo_t p;

  overall_cost = reg_cost = mem_cost = 0;
  for (i = 0; i < pseudos_num; i++)
    {
      p = pseudos [i];
      hard_regno = PSEUDO_HARD_REGNO (p) = reg_renumber [PSEUDO_REGNO (p)];
      PSEUDO_ASSIGNED_P (p) = TRUE;
      if (hard_regno < 0)
	{
	  cost = PSEUDO_MEMORY_COST (p);
	  mem_cost += cost;
	}
      else
	{
	  cost = (PSEUDO_HARD_REG_COSTS (p)
		  [class_hard_reg_index
		   [PSEUDO_COVER_CLASS (p)] [hard_regno]]);
	  reg_cost += cost;
	}
      overall_cost += cost;
    }

  if (ira_dump_file != NULL)
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
/* The function checks correctness of the allocation.  */
static void
check_allocation (void)
{
  pseudo_t p, conflict_p, *pseudo_vec;
  int i, hard_regno, conflict_hard_regno, j, nregs, conflict_nregs;
  
  for (i = 0; i < pseudos_num; i++)
    {
      p = pseudos [i];
      if (PSEUDO_REGNO (p) < 0 || (hard_regno = PSEUDO_HARD_REGNO (p)) < 0)
	continue;
      nregs = hard_regno_nregs [hard_regno] [PSEUDO_MODE (p)];
      pseudo_vec = PSEUDO_CONFLICT_PSEUDO_VEC (p);
      for (j = 0; (conflict_p = pseudo_vec [j]) != NULL; j++)
	if ((conflict_hard_regno = PSEUDO_HARD_REGNO (conflict_p)) >= 0)
	  {
	    conflict_nregs
	      = (hard_regno_nregs
		 [conflict_hard_regno] [PSEUDO_MODE (conflict_p)]);
	    if ((conflict_hard_regno <= hard_regno
		 && hard_regno < conflict_hard_regno + conflict_nregs)
		|| (hard_regno <= conflict_hard_regno
		    && conflict_hard_regno < hard_regno + nregs))
	      {
		fprintf (stderr, "bad allocation for %d and %d\n",
			 PSEUDO_REGNO (p), PSEUDO_REGNO (conflict_p));
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
/* The function prints redundant memory memory copies. */
static void
print_redundant_copies (void)
{
  int i, hard_regno;
  pseudo_t p;
  struct pseudo_copy *cp, *next_cp;
  
  for (i = 0; i < pseudos_num; i++)
    {
      p = pseudos [i];
      if (PSEUDO_REGNO (p) < 0)
	/* It is a cap. */
	continue;
      hard_regno = PSEUDO_HARD_REGNO (p);
      if (hard_regno >= 0)
	continue;
      for (cp = PSEUDO_COPIES (p); cp != NULL; cp = next_cp)
	if (cp->first == p)
	  next_cp = cp->next_first_pseudo_copy;
	else
	  {
	    next_cp = cp->next_second_pseudo_copy;
	    if (ira_dump_file != NULL && cp->move_insn != NULL_RTX
		&& PSEUDO_HARD_REGNO (cp->first) == hard_regno)
	      fprintf (ira_dump_file, "move %d(freq %d):%d\n",
		       INSN_UID (cp->move_insn), cp->freq, hard_regno);
	  }
    }
}
#endif

/* Setup preferred and alternative classes for pseudo-registers for
   other passes.  */
static void
setup_preferred_alternate_classes (void)
{
  int i;
  enum reg_class cover_class;
  pseudo_t p;

  for (i = 0; i < pseudos_num; i++)
    {
      p = pseudos [i];
      cover_class = PSEUDO_COVER_CLASS (p);
      if (cover_class == NO_REGS)
	cover_class = GENERAL_REGS;
      setup_reg_classes (PSEUDO_REGNO (p), cover_class, NO_REGS);
    }
}



/* This is the main entry of IRA.  */
void
ira (FILE *f)
{
  int overall_cost_before, loops_p;
  int rebuild_p;

  ira_dump_file = f;

  no_new_pseudos = 0;

  rebuild_p = update_equiv_regs ();
    
#ifndef IRA_NO_OBSTACK
  gcc_obstack_init (&ira_obstack);
#endif
  bitmap_obstack_initialize (&ira_bitmap_obstack);

  reg_equiv_invariant_p = ira_allocate (max_reg_num () * sizeof (int));
  memset (reg_equiv_invariant_p, 0, max_reg_num () * sizeof (int));
  reg_equiv_const = ira_allocate (max_reg_num () * sizeof (rtx));
  memset (reg_equiv_const, 0, max_reg_num () * sizeof (rtx));
  find_reg_equiv_invariant_const ();
  if (rebuild_p)
    {
      rebuild_jump_labels (get_insns ());
      purge_all_dead_edges ();
      delete_unreachable_blocks ();
    }
  setup_eliminable_regset ();

  overall_cost = reg_cost = mem_cost = 0;
  load_cost = store_cost = shuffle_cost = 0;
  move_loops_num = additional_jumps_num = 0;
  loops_p = ira_build (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL);
  ira_color ();

  ira_emit ();

  max_regno = max_reg_num ();
  
  /* Allocate the reg_renumber array.  */
  allocate_reg_info (max_regno, FALSE, TRUE);
  
  setup_reg_renumber ();
  no_new_pseudos = 1;

  if (loops_p)
    {
      /* Even if new registers are not created rebuild IRA internal
	 representation to use correct regno pseudo map.  */
      ira_destroy ();
      ira_build (FALSE);
    }

  calculate_allocation_cost ();

#ifdef ENABLE_IRA_CHECKING
  check_allocation ();
#endif

  max_regno = max_reg_num ();
  delete_trivially_dead_insns (get_insns (), max_regno);
  /* The allocator makes live register information inaccurate.  */
  life_analysis (PROP_DEATH_NOTES | PROP_LOG_LINKS | PROP_REG_INFO);
  max_regno = max_reg_num ();
  
  /* Determine if the current function is a leaf before running IRA
     since this can impact optimizations done by the prologue and
     epilogue thus changing register elimination offsets.  */
  current_function_is_leaf = leaf_function_p ();
  
  /* Allocate the reg_renumber array.  */
  allocate_reg_info (max_regno, FALSE, TRUE);
  
  /* And the reg_equiv_memory_loc array.  */
  VEC_safe_grow (rtx, gc, reg_equiv_memory_loc_vec, max_regno);
  memset (VEC_address (rtx, reg_equiv_memory_loc_vec), 0,
	  sizeof (rtx) * max_regno);
  reg_equiv_memory_loc = VEC_address (rtx, reg_equiv_memory_loc_vec);
  
  allocate_initial_values (reg_equiv_memory_loc);
  
  fix_reg_equiv_init ();

  setup_preferred_alternate_classes ();

#ifdef ENABLE_IRA_CHECKING
  print_redundant_copies ();
#endif

  overall_cost_before = overall_cost;

  spilled_reg_stack_slots_num = 0;
  spilled_reg_stack_slots
    = ira_allocate (max_regno * sizeof (struct spilled_reg_stack_slot));

  build_insn_chain (get_insns ());
  reload_completed = ! reload (get_insns (), 1);

  ira_free (spilled_reg_stack_slots);

  if (ira_dump_file != NULL && overall_cost_before != overall_cost)
    fprintf (ira_dump_file, "+++Overall after reload %d\n", overall_cost);

  ira_destroy ();

  cleanup_cfg (CLEANUP_EXPENSIVE);

  ira_free (reg_equiv_invariant_p);
  ira_free (reg_equiv_const);
  
  bitmap_obstack_release (&ira_bitmap_obstack);
#ifndef IRA_NO_OBSTACK
  obstack_free (&ira_obstack, NULL);
#endif
  
  reload_completed = 1;
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
  "ira",                               /* name */
  gate_ira,                            /* gate */
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
