/* Allocation based on graph coloring.
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
#include "expr.h"
#include "optabs.h"
#include "recog.h"
#include "cfgloop.h"
#include "errors.h"
#include "toplev.h"
#include "params.h"
#include "yara-int.h"

/* We use approach similar to one described in Robert Morgan's book
   "Building an optimizing compiler".  In brief it can be considered
   as combination of the following algorithms:

   o one iteration of global optimistic colouring with the biased
     colouring, optimistic coalescing and live range splitting on demand;
   o then local register allocation based on heuristic spilling
     pseudos with the latest usage and splitting live ranges of global
     pseudos.
   o coalescing.  */

/* ??? more debug information - disposition/assigning, coalescing */
/* ??? More accurate place for changing reload_completed and
   reload_in_progress.  */



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
  HARD_REG_SET temp_set1, temp_set2;

  for (i = 0; i < N_REG_CLASSES; i++)
    for (j = 0; j < N_REG_CLASSES; j++)
      alloc_reg_class_subclasses [i] [j] = LIM_REG_CLASSES;

  for (i = 0; i < N_REG_CLASSES; i++)
    {
      if (i == (int) NO_REGS)
	continue;

      COPY_HARD_REG_SET (temp_set1, reg_class_contents [i]);
      for (j = 0; j < N_REG_CLASSES; j++)
	if (i != j)
	  {
	    enum reg_class *p;
	    
	    COPY_HARD_REG_SET (temp_set2, reg_class_contents [j]);
	    GO_IF_HARD_REG_SUBSET (temp_set1, temp_set2, subclass);
	    continue;
	  subclass:
	    p = &alloc_reg_class_subclasses [j] [0];
	    while (*p != LIM_REG_CLASSES) p++;
	    *p = (enum reg_class) i;
	  }
    }
}

/* The following is true if moving any hard register of the class into
   any hard register of the same class is cheaper than load or store
   any hard register of the class and the class is not sub-class of
   another class with corresponding true value.  */
static bool class_closure_p [N_REG_CLASSES];

/* The following defines array of the closure classes.  */
static enum reg_class closure_classes [N_REG_CLASSES];
static int closure_classes_size;

static void
set_up_closure_classes (void)
{
  enum machine_mode mode;
  enum reg_class cl, *sub_cl_ptr;
  int cost, min_cost;

  for (cl = 0; cl < N_REG_CLASSES; cl++)
    {
      class_closure_p [cl] = cl != NO_REGS;
      for (sub_cl_ptr = &alloc_reg_class_subclasses [cl] [0];
	   *sub_cl_ptr != LIM_REG_CLASSES;
	   sub_cl_ptr++)
	{
	  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
	    if (mode != VOIDmode && mode != BLKmode
		&& contains_reg_of_mode [*sub_cl_ptr] [mode]
		&& contains_reg_of_mode [cl] [mode])
	      {
		min_cost = memory_move_cost [mode] [*sub_cl_ptr] [0];
		cost = memory_move_cost [mode] [*sub_cl_ptr] [1];
		if (cost < min_cost)
		  min_cost = cost;
		if (min_cost < move_cost [mode] [cl] [cl])
		  break;
	      }
	  if (mode < MAX_MACHINE_MODE)
	    {
	      class_closure_p [cl] = false;
	      break;
	    }
	}
    }
  for (cl = 0; cl < N_REG_CLASSES; cl++)
    if (class_closure_p [cl])
      {
	for (sub_cl_ptr = &alloc_reg_class_subclasses [cl] [0];
	     *sub_cl_ptr != LIM_REG_CLASSES;
	     sub_cl_ptr++)
	  class_closure_p [*sub_cl_ptr] = false;
      }
  closure_classes_size = 0;
  for (cl = 0; cl < N_REG_CLASSES; cl++)
    if (class_closure_p [cl])
      closure_classes [closure_classes_size++] = cl;
}      


static int reg_class_cover_size, final_reg_class_cover_size;
static enum reg_class reg_class_cover [N_REG_CLASSES];
static enum reg_class final_reg_class_cover [N_REG_CLASSES];
static HARD_REG_SET reg_class_cover_set;
static HARD_REG_SET temp_hard_reg_set;

static bool
extend_reg_class_cover (void)
{
  bool result;
  int i;
  enum reg_class cl;

  COPY_HARD_REG_SET (temp_hard_reg_set, reg_class_contents [ALL_REGS]);
  AND_COMPL_HARD_REG_SET (temp_hard_reg_set, fixed_reg_set);
  AND_COMPL_HARD_REG_SET (temp_hard_reg_set, reg_class_cover_set);
  GO_IF_HARD_REG_EQUAL (temp_hard_reg_set, zero_hard_reg_set,
			done);
  if (reg_class_cover_size >= final_reg_class_cover_size)
    return false;
  result = false;
  for (i = 0; i < closure_classes_size; i++)
    {
      cl = closure_classes [i];
      gcc_assert (class_closure_p [cl]);
      GO_IF_HARD_REG_EQUAL (reg_class_contents [cl], zero_hard_reg_set,
			    halt);
      COPY_HARD_REG_SET (temp_hard_reg_set, reg_class_cover_set);
      AND_HARD_REG_SET (temp_hard_reg_set, reg_class_contents [cl]);
      GO_IF_HARD_REG_EQUAL (temp_hard_reg_set, zero_hard_reg_set,
			    non_intersected);
      continue;
    non_intersected:
      IOR_HARD_REG_SET (reg_class_cover_set, reg_class_contents [cl]);
      reg_class_cover [reg_class_cover_size++] = cl;
      result = extend_reg_class_cover () || result;
      AND_COMPL_HARD_REG_SET (reg_class_cover_set, reg_class_contents [cl]);
      reg_class_cover_size--;
    }
  return result;

 halt:
  gcc_unreachable ();

 done:
  if (final_reg_class_cover_size > reg_class_cover_size)
    {
      final_reg_class_cover_size = reg_class_cover_size;
      memcpy (final_reg_class_cover, reg_class_cover,
	      reg_class_cover_size * sizeof (enum reg_class));
    }
  return true;
}

static enum reg_class class_translate [N_REG_CLASSES];

static void
set_up_class_translate (void)
{
  enum reg_class cl, *cl_ptr;
  int i;

  for (cl = 0; cl < N_REG_CLASSES; cl++)
    class_translate [cl] = NO_REGS;
  for (i = 0; i < final_reg_class_cover_size; i++)
    {
      cl = final_reg_class_cover [i];
      for (cl_ptr = &alloc_reg_class_subclasses [cl] [0];
	   *cl_ptr != LIM_REG_CLASSES;
	   cl_ptr++)
	{
	  if (class_translate [*cl_ptr] == NO_REGS)
	    class_translate [*cl_ptr] = cl;
#ifdef ENABLE_YARA_CHECKING
	  else
	    {
	      HARD_REG_SET temp_set;
	      
	      COPY_HARD_REG_SET (temp_set, reg_class_contents [*cl_ptr]);
	      AND_COMPL_HARD_REG_SET (temp_set, fixed_reg_set);
	      GO_IF_HARD_REG_SUBSET (temp_set, zero_hard_reg_set, ok);
	      gcc_unreachable ();
	    ok:
	      ;
	    }
#endif
	}
      class_translate [cl] = cl;
    }
}

static void
print_class_cover (FILE *f)
{
  static const char *const reg_class_names[] = REG_CLASS_NAMES;
  int i;

  fprintf (f, "Class cover:\n");
  for (i = 0; i < final_reg_class_cover_size; i++)
    fprintf (f, " %s", reg_class_names [final_reg_class_cover [i]]);
  fprintf (f, "\nClass translation:\n");
  for (i = 0; i < N_REG_CLASSES; i++)
    fprintf (f, " %s -> %s\n", reg_class_names [i],
	     reg_class_names [class_translate [i]]);
}

static void ATTRIBUTE_UNUSED
debug_class_cover (void)
{
  print_class_cover (stderr);
}

static void
find_reg_class_closure (void)
{
  bool ok_p;

  setup_reg_subclasses ();
  set_up_closure_classes ();
  final_reg_class_cover_size = N_REG_CLASSES;
  reg_class_cover_size = 0;
  CLEAR_HARD_REG_SET (reg_class_cover_set);
  ok_p = extend_reg_class_cover ();
  gcc_assert (ok_p);
  set_up_class_translate ();
}


static int init_class_cost [N_REG_CLASSES];

/* Minimal costs of usage the current insn alternative operand placed
   in memory or register of given class.  */
static int min_op_memory_cost [MAX_RECOG_OPERANDS];
static int min_op_class_cost [MAX_RECOG_OPERANDS] [N_REG_CLASSES];

/* Set up minimal op costs when any operand is ok.  */
static void
set_up_minimal_op_costs (allocno_t a)
{
  int op_num;
  enum reg_class cl;

  gcc_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
  gcc_assert (op_num >= 0);
  /* ??? If we have insn alt cost infrastructure we could just set it
     to 0.  */
  /* We still need read it from memory which is usually slower than
     register.  */
  if (min_op_memory_cost [op_num] > 1)
    min_op_memory_cost [op_num] = 1;
  for (cl = 0; cl < N_REG_CLASSES; cl++)
    min_op_class_cost [op_num] [cl] = 0;
}

/* Function updating costs for operand insn allocno A which should be
   in hard register of CLASS or in memory if MEM_P.  */
static void
update_min_op_costs (allocno_t a, enum reg_class class, bool mem_p)
{
  int op_num, cost;
  enum reg_class cl;
  enum machine_mode mode;
  enum op_type op_mode;

  gcc_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
  gcc_assert (class != NO_REGS || mem_p);
  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
  gcc_assert (op_num >= 0);
  op_mode = INSN_ALLOCNO_OP_MODE (a);
  mode = ALLOCNO_MODE (a);
  /* ??? If we have insn alt cost infrastructure we could set it to 0.  */
  if (mem_p)
    cost = 1; /* We still need read it from memory which is usually
		 slower than register.  */
  else
    cost = ((op_mode == OP_IN || op_mode == OP_INOUT
	     ? memory_move_cost [mode] [class] [1] : 0)
	    + (op_mode == OP_OUT || op_mode == OP_INOUT
	       ? memory_move_cost [mode] [class] [0] : 0));
  gcc_assert (cost >= 0);
  if (cost < min_op_memory_cost [op_num])
    min_op_memory_cost [op_num] = cost;
  for (cl = 0; cl < N_REG_CLASSES; cl++)
    {
      /* ??? Tables to speed up.  */
      if (mem_p)
	cost = ((op_mode == OP_IN || op_mode == OP_INOUT
		 ? memory_move_cost [mode] [cl] [0] : 0)
		+ (op_mode == OP_OUT || op_mode == OP_INOUT
		   ? memory_move_cost [mode] [cl] [1] : 0));
      else
	{
	  cost = 0;
	  if (! class_subset_p [cl] [class])
	    {
	      if (op_mode == OP_IN || op_mode == OP_INOUT)
		cost += register_move_cost [mode] [cl] [class];
	      if (op_mode == OP_OUT || op_mode == OP_INOUT)
		cost += register_move_cost [mode] [class] [cl];
	    }
	}
      gcc_assert (cost >= 0);
      if (cost < min_op_class_cost [op_num] [cl])
	min_op_class_cost [op_num] [cl] = cost;
    }
}

/* The current insn and its alternative.  */
static rtx curr_preference_insn;
static int curr_preference_alt;

/* Minimal costs of usage the current insn operand placed in memory or
   register of given class.  */
static int min_alt_memory_cost [MAX_RECOG_OPERANDS];
static int min_alt_class_cost [MAX_RECOG_OPERANDS] [N_REG_CLASSES];

/* Recursive function processing insn allocno A for CONSTRAINT.  */
static void
process_insn_allocno_constraint (allocno_t a, const char *constraint,
				 int *add_cost)
{
  enum reg_class cl;
  enum machine_mode mode;
  enum op_type op_mode;
  int add_op_cost;
  rtx op, no_subreg_op;
  char c;

  for (; (c = *constraint); constraint += CONSTRAINT_LEN (c, constraint))
    if (c == '*')
      /* That is really important for better performance.  */
      constraint += CONSTRAINT_LEN (c, constraint);
    else if (c == ' ' || c == '\t' || c == '=' || c == '+' || c == '*'
	     || c == '&' || c == '%'
	     /* We ignore this ambigous hints.  The cost should be
		defined just by move costs. */
	     || c == '?' || c == '!')
      ;
    else
      {
	gcc_assert (c != '#');
	add_op_cost = 0;
	op = *INSN_ALLOCNO_LOC (a);
	SKIP_TO_REG (no_subreg_op, op);
	op_mode = INSN_ALLOCNO_OP_MODE (a);
	mode = ALLOCNO_MODE (a);
	switch (c)
	  {
	  case '\0':
	  case 'X':
	    set_up_minimal_op_costs (a);
	    break;
	    
	  case 'i':
	    if (! CONSTANT_P (op))
	      add_op_cost = -1;
	    break;
	    
	  case 'n':
	    if (GET_CODE (op) != CONST_INT && (GET_CODE (op) != CONST_DOUBLE
					       || GET_MODE (op) != VOIDmode))
	      add_op_cost = -1;
	    break;
	    
	  case 's':
	    if (! CONSTANT_P (op) || GET_CODE (op) == CONST_INT
		|| (GET_CODE (op) == CONST_DOUBLE
		    && GET_MODE (op) == VOIDmode))
	      add_op_cost = -1;
	    break;
	    
	  case 'I':
	  case 'J':
	  case 'K':
	  case 'L':
	  case 'M':
	  case 'N':
	  case 'O':
	  case 'P':
	    if (GET_CODE (op) != CONST_INT
		|| ! CONST_OK_FOR_CONSTRAINT_P (INTVAL (op), c, constraint))
	      add_op_cost = -1;
	    break;

	  case 'E':
	  case 'F':
	    if (GET_CODE (op) != CONST_DOUBLE
		&& (GET_CODE (op) != CONST_VECTOR
		    || GET_MODE_CLASS (GET_MODE (op)) != MODE_VECTOR_FLOAT))
	      add_op_cost = -1;
	    break;

	  case 'G':
	  case 'H':
	    if (GET_CODE (op) != CONST_DOUBLE
		|| ! CONST_DOUBLE_OK_FOR_CONSTRAINT_P (op, c, constraint))
	      add_op_cost = -1;
	    break;

	  case 'm':
	  case 'o':
	    if ((MEM_P (no_subreg_op) && c == 'm')
		|| (offsettable_nonstrict_memref_p (no_subreg_op) && c == 'o'))
	      ;
	    /* We could accept a constant that can be turned into
	       memory.  */
	    else if (CONSTANT_P (op))
	      add_op_cost = minimal_memory_load_cost [mode];
	    else if (REG_P (no_subreg_op))
	      {
		/* Accept a register which might be placed in memory.  */
		if (HARD_REGISTER_P (no_subreg_op))
		  {
		    enum reg_class class;

		    class = REGNO_REG_CLASS (ALLOCNO_REGNO (a));
		    add_op_cost
		      = ((op_mode == OP_IN || op_mode == OP_INOUT
			  ? memory_move_cost [mode] [class] [0] : 0)
			 + (op_mode == OP_OUT || op_mode == OP_INOUT
			    ? memory_move_cost [mode] [class] [1] : 0));
		  }
		else
		  update_min_op_costs (a, NO_REGS, true);
	      }
	    else
	      add_op_cost = -1;
	    break;
	    
	  case 'V':
	    if (! MEM_P (op) || offsettable_nonstrict_memref_p (op))
	      add_op_cost = -1;
	    break;
	    
	  case '<':
	    if (! MEM_P (op) || (GET_CODE (XEXP (op, 0)) != PRE_DEC
				 && GET_CODE (XEXP (op, 0)) != POST_DEC))
	      add_op_cost = -1;
	    break;

	  case '>':
	    if (! MEM_P (op) || (GET_CODE (XEXP (op, 0)) != PRE_INC
				 && GET_CODE (XEXP (op, 0)) != POST_INC))
	      add_op_cost = -1;
	    break;
	    
	  case 'p':
	    /* ??? strict_p */
	    /* p is used for address_operands.  We consider it in non-strict
	       sense.  */
	    GO_IF_LEGITIMATE_ADDRESS (VOIDmode, op, win_p);
	    add_op_cost = -1;
	    
	  win_p:
	    break;
	    
	  case 'g':
	    if (MEM_P (no_subreg_op))
	      ;
	    /* We could accept a constant that can be turned into
	       memory.  */
	    else if (CONSTANT_P (op))
	      add_op_cost = minimal_memory_load_cost [mode];
	    else if (REG_P (no_subreg_op))
	      {
		/* Accept a register which might be placed in memory.  */
		if (HARD_REGISTER_P (no_subreg_op))
		  {
		    enum reg_class class;

		    class = REGNO_REG_CLASS (ALLOCNO_REGNO (a));
		    add_op_cost
		      = ((op_mode == OP_IN || op_mode == OP_INOUT
			  ? memory_move_cost [mode] [class] [0] : 0)
			 + (op_mode == OP_OUT || op_mode == OP_INOUT
			    ? memory_move_cost [mode] [class] [1] : 0));
		  }
		else
		  update_min_op_costs (a, GENERAL_REGS, true);
	      }
	    else if (GET_CODE (op) != SCRATCH)
	      add_op_cost = -1;
	    break;
	    
	  case 'r':
	  case 'a': case 'b': case 'c': case 'd': case 'e':
	  case 'f': case 'h': case 'j': case 'k': case 'l':
	  case 'q': case 't': case 'u':
	  case 'v': case 'w': case 'x': case 'y': case 'z':
	  case 'A': case 'B': case 'C': case 'D':
	  case 'Q': case 'R': case 'S': case 'T': case 'U':
	  case 'W': case 'Y': case 'Z':
	    cl = (c == 'r'
		  ? GENERAL_REGS
		  : REG_CLASS_FROM_CONSTRAINT (c, constraint));
	    if (cl == NO_REGS)
	      add_op_cost = -1;
	    else if (MEM_P (op))
	      add_op_cost = ((op_mode == OP_IN || op_mode == OP_INOUT
			      ? memory_move_cost [mode] [cl] [1] : 0)
			     + (op_mode == OP_OUT || op_mode == OP_INOUT
				? memory_move_cost [mode] [cl] [0] : 0));
	    else if (REG_P (no_subreg_op))
	      {
		/* Accept a register which might be placed in memory.  */
		if (HARD_REGISTER_P (no_subreg_op))
		  {
		    enum reg_class class;

		    class = REGNO_REG_CLASS (ALLOCNO_REGNO (a));
		    if (! hard_reg_in_set_p (REGNO (no_subreg_op), mode,
					     reg_class_contents [cl]))
		      add_op_cost
			= ((op_mode == OP_IN || op_mode == OP_INOUT
			    ? register_move_cost [mode] [class] [cl] : 0)
			   + (op_mode == OP_OUT || op_mode == OP_INOUT
			      ? register_move_cost [mode] [cl] [class] : 0));
		  }
		else
		  update_min_op_costs (a, cl, false);
	      }
	    else if (GET_CODE (op) != SCRATCH)
	      add_op_cost = -1;
	    break;
	    
	  case '0': case '1': case '2': case '3': case '4':
	  case '5': case '6': case '7': case '8': case '9':
	    {
	      struct insn_op_info *info;

	      info = insn_infos [INSN_UID (curr_preference_insn)];
	      process_insn_allocno_constraint
		(a, info->op_constraints [(c - '0') * info->n_alts
					  + curr_preference_alt], add_cost);
	      break;
	    }
	  }
	if (add_op_cost < 0)
	  continue;
	if (*add_cost < 0 || *add_cost > add_op_cost)
	  *add_cost = add_op_cost;
      }
}

/* Recursive function processing insn allocno A and the subsequent
   insn allocnos to find the minimal costs.  */
static void
process_insn_allocno_for_costs (allocno_t a, int add_cost)
{
  allocno_t a2, next_a, prev_a, prev_a2;
  int op_num, n, add_op_cost;
  enum reg_class cl;
  const char *str;
  struct insn_op_info *info;
  /* If the element value is true, than the corresponding operand
     actually represents the commutative operand.  */
  static bool op_exchange_p [MAX_RECOG_OPERANDS];

  if (a == NULL)
    {
      int add_op, add_step, pseudo_op_num;

      pseudo_op_num = 0;
      for (a = insn_allocnos [INSN_UID (curr_preference_insn)];
	   a != NULL;
	   a = INSN_ALLOCNO_NEXT (a))
	if (ALLOCNO_REGNO (a) >= 0 && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a))
	    && INSN_ALLOCNO_TYPE (a) >= OPERAND_BASE)
	  pseudo_op_num++;
      if (pseudo_op_num == 0)
	return;
      add_step = (add_cost + pseudo_op_num - 1) / pseudo_op_num;
      for (a = insn_allocnos [INSN_UID (curr_preference_insn)];
	   a != NULL;
	   a = INSN_ALLOCNO_NEXT (a))
	{
	  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
	  if (op_num < 0 || ALLOCNO_REGNO (a) < 0
	      || HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a)))
	    continue;
	  if (op_exchange_p [op_num])
	    {
	      a2 = INSN_ALLOCNO_COMMUTATIVE (a);
	      op_num = INSN_ALLOCNO_TYPE (a2) - OPERAND_BASE;
	    }
	  gcc_assert (min_op_memory_cost [op_num] >= 0);
	  if (add_step >= add_cost)
	    {
	      add_op = add_cost;
	      add_cost = 0;
	    }
	  else
	    {
	      add_op = add_step;
	      add_cost -= add_op;
	    }
	  if (min_op_memory_cost [op_num] != INT_MAX
	      && (min_alt_memory_cost [op_num]
		  > min_op_memory_cost [op_num] + add_op))
	    min_alt_memory_cost [op_num]
	      = min_op_memory_cost [op_num] + add_op;
	  for (cl = 0; cl < N_REG_CLASSES; cl++)
	    {
	      gcc_assert (min_op_class_cost [op_num] [cl] >= 0);
	      if (min_op_class_cost [op_num] [cl] != INT_MAX
		  && (min_alt_class_cost [op_num] [cl]
		      > min_op_class_cost [op_num] [cl] + add_op))
		{
		  min_alt_class_cost [op_num] [cl]
		    = min_op_class_cost [op_num] [cl] + add_op;
		}
	    }
	}
      return;
    }
  info = insn_infos [INSN_UID (curr_preference_insn)];
  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
  for (n = 0; n < 2; n++)
     {
       a2 = INSN_ALLOCNO_COMMUTATIVE (a);
       if (op_num >= 0)
	 {
	   op_exchange_p [op_num] = n != 0;
	   if (a2 != NULL) 
	     op_exchange_p [INSN_ALLOCNO_TYPE (a2) - OPERAND_BASE] = n != 0;
	 }
       if (INSN_ALLOCNO_TYPE (a) == BASE_REG
	   || INSN_ALLOCNO_TYPE (a) == INDEX_REG
	   || INSN_ALLOCNO_TYPE (a) == NON_OPERAND)
	 process_insn_allocno_for_costs (INSN_ALLOCNO_NEXT (a), add_cost);
       else
	 {
	   gcc_assert (op_num >= 0);
	   if (info == NULL || info->n_alts == 0)
	     {
	       set_up_minimal_op_costs (a);
	       process_insn_allocno_for_costs (INSN_ALLOCNO_NEXT (a),
					       add_cost);
	     }
	   else if (! TEST_ALT (INSN_ALLOCNO_POSSIBLE_ALTS (a),
				curr_preference_alt))
	     ;
	   else
	     {
	       str = info->op_constraints [op_num * info->n_alts
					   + curr_preference_alt];
	       add_op_cost = -1; /* undef */
	       if (str == NULL || *str == '\0')
		 {
		   add_op_cost = 0;
		   set_up_minimal_op_costs (a);
		 }
	       else
		 process_insn_allocno_constraint (a, str, &add_op_cost);
	       if (add_op_cost >= 0)
		 process_insn_allocno_for_costs (INSN_ALLOCNO_NEXT (a),
						 add_cost + add_op_cost);
	     }
	 }
       if (a2 == NULL)
	 break;
       for (prev_a2 = a, next_a = INSN_ALLOCNO_NEXT (a);
	    next_a != NULL;
	    prev_a2 = next_a, next_a = INSN_ALLOCNO_NEXT (next_a))
	 if (next_a == a2)
	   break;
       if (next_a == NULL)
	 break;
       for (prev_a = NULL,
	      next_a = insn_allocnos [INSN_UID (curr_preference_insn)];
	    next_a != NULL;
	    prev_a = next_a, next_a = INSN_ALLOCNO_NEXT (next_a))
	 if (next_a == a)
	   break;
       gcc_assert (next_a != NULL);
       make_commutative_exchange (a);
       next_a = INSN_ALLOCNO_NEXT (a);
       INSN_ALLOCNO_NEXT (a) = INSN_ALLOCNO_NEXT (a2);
       if (prev_a == NULL)
	 insn_allocnos [INSN_UID (curr_preference_insn)] = a2;
       else
	 INSN_ALLOCNO_NEXT (prev_a) = a2;
       if (next_a == a2)
	 INSN_ALLOCNO_NEXT (a2) = a;
       else
	 {
	   INSN_ALLOCNO_NEXT (prev_a2) = a;
	   INSN_ALLOCNO_NEXT (a2) = next_a;
	 }
       a = a2;
       gcc_assert (op_num == INSN_ALLOCNO_TYPE (a) - OPERAND_BASE);
     }
}

/* Function processing all insn operand allocnos to update the minimal
   costs for the current alternative.  */
static void
update_min_alt_costs (void)
{
  int op_num;
  allocno_t a;
  
  for (a = insn_allocnos [INSN_UID (curr_preference_insn)];
       a != NULL;
       a = INSN_ALLOCNO_NEXT (a))
    {
      op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
      if (op_num < 0)
	continue;
      min_op_memory_cost [op_num] = INT_MAX;
      memcpy (min_op_class_cost [op_num], init_class_cost,
	      sizeof (init_class_cost));
    }
  process_insn_allocno_for_costs
    (insn_allocnos [INSN_UID (curr_preference_insn)], 0);
}

/* Function setting up reg class preferences and cover class for
   cans.  */
static void
set_up_cover_classes_and_reg_costs (void)
{
  int i, j, min_cost, cl, cl1, freq, op_num, can_num, hard_regno;
  enum reg_class best_class, cover_class;
  rtx insn, bound;
  enum machine_mode mode;
  enum op_type op_mode;
  basic_block bb;
  allocno_t a;
  can_t can;
  struct insn_op_info *info;
  int *can_class_cost, *can_memory_cost, *cost_ptr;

  can_class_cost = yara_allocate (cans_num * N_REG_CLASSES * sizeof (int));
  memset (can_class_cost, 0, cans_num * N_REG_CLASSES * sizeof (int));
  can_memory_cost = yara_allocate (cans_num * sizeof (int));
  memset (can_memory_cost, 0, cans_num * sizeof (int));
  FOR_EACH_BB (bb)
    {
      bound = NEXT_INSN (BB_END (bb));
      freq = bb->frequency;
      if (freq == 0)
	freq = 1;
      for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
	if (INSN_P (insn))
	  {
	    curr_preference_insn = insn;
	    for (a = insn_allocnos [INSN_UID (insn)];
		 a != NULL;
		 a = INSN_ALLOCNO_NEXT (a))
	      {
		op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
		if (op_num < 0)
		  continue;
		min_alt_memory_cost [op_num] = INT_MAX;
		memcpy (min_alt_class_cost [op_num], init_class_cost,
			sizeof (init_class_cost));
	      }
	    info = insn_infos [INSN_UID (insn)];
	    if (info == NULL || info->n_alts == 0)
	      update_min_alt_costs ();
	    else
	      {
		for (curr_preference_alt = info->n_alts - 1;
		     curr_preference_alt >= 0;
		     curr_preference_alt--)
		  update_min_alt_costs ();
	      }
	    for (a = insn_allocnos [INSN_UID (insn)];
		 a != NULL;
		 a = INSN_ALLOCNO_NEXT (a))
	      {
		if ((hard_regno = ALLOCNO_REGNO (a)) < 0
		    || HARD_REGISTER_NUM_P (hard_regno))
		  continue;
		can = ALLOCNO_CAN (a);
		if (can == NULL)
		  continue;
		can_num = CAN_NUM (can);
		op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
		if (op_num >= 0)
		  {
		    if (min_alt_memory_cost [op_num] == INT_MAX)
		      can_memory_cost [can_num] = INT_MAX;
		    else
		      can_memory_cost [can_num]
			+= min_alt_memory_cost [op_num] * freq;
		    for (cl = 0; cl < N_REG_CLASSES; cl++)
		      if (min_alt_class_cost [op_num] [cl] == INT_MAX)
			can_class_cost [can_num * N_REG_CLASSES + cl]
			  = INT_MAX;
		      else
			can_class_cost [can_num * N_REG_CLASSES + cl]
			  += min_alt_class_cost [op_num] [cl] * freq;
		  }
		else if (INSN_ALLOCNO_TYPE (a) == BASE_REG
			 || INSN_ALLOCNO_TYPE (a) == INDEX_REG)
		  {
		    cl = (INSN_ALLOCNO_TYPE (a) == BASE_REG
			  ? BASE_REG_CLASS :  INDEX_REG_CLASS);
		    op_mode = INSN_ALLOCNO_OP_MODE (a);
		    mode = ALLOCNO_MODE (a);
		    can_memory_cost [can_num]
		      += (((op_mode == OP_IN || op_mode == OP_INOUT
			    ? memory_move_cost [mode] [cl] [1] : 0)
			   + (op_mode == OP_OUT || op_mode == OP_INOUT
			      ? memory_move_cost [mode] [cl] [0] : 0))
			  * freq);
		    for (cl1 = 0; cl1 < N_REG_CLASSES; cl1++)
		      {
			/* ??? Tables to speed up.  */
			if ((op_mode == OP_IN || op_mode == OP_INOUT)
			    && ! class_subset_p [cl1] [cl])
			  can_class_cost [can_num * N_REG_CLASSES + cl1]
			    += register_move_cost [mode] [cl1] [cl] * freq;
			if ((op_mode == OP_OUT || op_mode == OP_INOUT)
			    && ! class_subset_p [cl] [cl1])
			  can_class_cost [can_num * N_REG_CLASSES + cl1]
			    += register_move_cost [mode] [cl] [cl1] * freq;
		      }
		  }
	      }	
	  }
    }
  for (i = 0; i < cans_num; i++)
    {
      can = cans [i];
      mode = CAN_MODE (can);
      gcc_assert (i == CAN_NUM (can));
      cost_ptr = &can_class_cost [i * N_REG_CLASSES];
      best_class = NO_REGS;
      CAN_MEMORY_COST (can) = min_cost = can_memory_cost [i];
      /* Skip NO_REGS.  */
      for (cl = 1; cl < N_REG_CLASSES; cl++)
	/* ??? What happens if the first register of the class is ok
	   but not second one (we could assign memory in
	   assign_global_can_allocnos or better build
	   CLASS_MODE_OK).  */
	if (HARD_REGNO_MODE_OK (class_hard_regs [cl] [0], mode)
	    && (min_cost > cost_ptr [cl]
		|| (min_cost == cost_ptr [cl]
		    && class_subset_p [best_class] [cl])))
	  {
	    min_cost = cost_ptr [cl];
	    best_class = cl;
	  }
      cover_class = CAN_COVER_CLASS (can) = class_translate [best_class];
      if (CAN_COVER_CLASS (can) != NO_REGS)
	{
	  CAN_COVER_CLASS_COST (can) = cost_ptr [cover_class];
	  CAN_HARD_REG_COSTS (can)
	    = yara_allocate (sizeof (int) * class_hard_regs_num [cover_class]);
	  for (j = 0; j < (int) class_hard_regs_num [cover_class]; j++)
	    {
	      hard_regno = class_hard_regs [cover_class] [j];
	      CAN_HARD_REG_COSTS (can) [j]
		= cost_ptr [REGNO_REG_CLASS (hard_regno)] - min_cost;
	    }
	}
    }
  yara_free (can_memory_cost);
  yara_free (can_class_cost);
}

static bool
add_can_copy (can_t can, can_t another_can, bool to_p, int freq)
{
  struct can_copy *cp;

  if (CAN_COVER_CLASS (can) == NO_REGS
      || CAN_COVER_CLASS (another_can) == NO_REGS
      || CAN_COVER_CLASS (can) != CAN_COVER_CLASS (another_can))
    return false;
  cp = yara_allocate (sizeof (struct can_copy));
  cp->to_p = to_p;
  cp->can = another_can;
  cp->freq = freq;
  cp->prev_can_copy = NULL;
  cp->next_can_copy = CAN_COPIES (can);
  if (cp->next_can_copy != NULL)
    cp->next_can_copy->prev_can_copy = cp;
  CAN_COPIES (can) = cp;
  return true;
}

static void
add_can_copies (can_t dst_can, can_t src_can, int freq)
{
  if (! add_can_copy (src_can, dst_can, true, freq))
    return;
  if (! add_can_copy (dst_can, src_can, false, freq))
    gcc_unreachable ();
  CAN_COPIES (src_can)->tied_can_copy = CAN_COPIES (dst_can);
  CAN_COPIES (dst_can)->tied_can_copy = CAN_COPIES (src_can);
}

static void
add_move_costs (void)
{
  int freq, i, hard_regno;
  rtx insn, bound, set, dst, src;
  basic_block bb;
  allocno_t a, src_a, dst_a;
  can_t can, src_can, dst_can;
  bool to_p;
  enum machine_mode mode;
  enum reg_class class, cover_class;

  FOR_EACH_BB (bb)
    {
      bound = NEXT_INSN (BB_END (bb));
      freq = bb->frequency;
      if (freq == 0)
	freq = 1;
      for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
	if (INSN_P (insn) && (set = single_set (insn)) != NULL_RTX
	    && REG_P (SET_DEST (set))
	    && REG_P (SET_SRC (set))
	    && find_reg_note (insn, REG_DEAD, SET_SRC (set)) != NULL
	    && ! side_effects_p (set))
	  {
	    dst = SET_DEST (set);
	    src = SET_SRC (set);
	    if (HARD_REGISTER_P (dst) || HARD_REGISTER_P (src))
	      {
		if (! HARD_REGISTER_P (dst))
		  {
		    to_p = true;
		    a = insn_allocno (dst, insn);
		    hard_regno = REGNO (src);
		  }
		else if (! HARD_REGISTER_P (src))
		  {
		    to_p = false;
		    a = insn_allocno (src, insn);
		    hard_regno = REGNO (dst);
		  }
		else
		  continue;
		can = ALLOCNO_CAN (a);
		if (can == NULL)
		  continue;
		cover_class = CAN_COVER_CLASS (can);
		if (can == NULL
		    || ! TEST_HARD_REG_BIT (reg_class_contents [cover_class],
					    hard_regno))
		  continue;
		gcc_assert (cover_class != NO_REGS);
		mode = CAN_MODE (can);
		class = REGNO_REG_CLASS (hard_regno);
		if (TEST_HARD_REG_BIT (no_alloc_regs, hard_regno))
		  continue;
		i = class_hard_reg_index [cover_class] [hard_regno];
		CAN_HARD_REG_COSTS (can) [i]
		  -= freq
		     * (to_p
			? register_move_cost [mode] [class] [cover_class]
			: register_move_cost [mode] [cover_class] [class]);
	      }
	    else
	      {
		src_a = insn_allocno (src, insn);
		dst_a = insn_allocno (dst, insn);
		src_can = ALLOCNO_CAN (src_a);
		dst_can = ALLOCNO_CAN (dst_a);
		add_can_copies (dst_can, src_can, freq);
	      }
	  }
    }
}



/* Bitmap of allocnos in the next varray.  */
static bitmap split_allocno_bitmap;

/* Varray for allocnos of splitted can.  */
static varray_type new_can_allocno_varray;

/* Split CAN and return the new can (if any).  CAN will contain
   allocnos in split_allocno_bitmap, the new can will contain the
   rest.  */
static can_t
split_can_by_split_allocno_bitmap (can_t can, bool global_p)
{
  allocno_t a, *can_allocnos;
  can_t new_can, *can_vec, c;
  int i, n, last, freq;

  VARRAY_POP_ALL (new_can_allocno_varray);
  can_allocnos = CAN_ALLOCNOS (can);
  for (last = i = 0; (a = can_allocnos [i]) != NULL; i++)
    {
      if (bitmap_bit_p (split_allocno_bitmap, ALLOCNO_NUM (a)))
	VARRAY_PUSH_GENERIC_PTR (new_can_allocno_varray, a);
      else
	can_allocnos [last++] = a;
    }
  gcc_assert (last != 0);
  if ((n = VARRAY_ACTIVE_SIZE (new_can_allocno_varray)) == 0)
    return NULL;
  can_allocnos [last] = NULL;
  /* Create a new can.  */
  new_can = create_can ();
  CAN_SLOTNO (new_can) = CAN_SLOTNO (can);
  can_allocnos = CAN_ALLOCNOS (new_can)
    = yara_allocate ((n + 1) * sizeof (allocno_t));
  memcpy (can_allocnos, &VARRAY_GENERIC_PTR (new_can_allocno_varray, 0),
	  n * sizeof (allocno_t));
  can_allocnos [n] = NULL;
  CAN_MODE (new_can) = CAN_MODE (can);
  CAN_COVER_CLASS (new_can) = CAN_COVER_CLASS (can);
  for (i = 0; (a = can_allocnos [i]) != NULL; i++)
    ALLOCNO_CAN (a) = new_can;
  freq = can_freq (new_can);
  CAN_FREQ (new_can) = freq;
  CAN_FREQ (can) -= freq;
  gcc_assert (CAN_FREQ (can) >= 0);
  if (CAN_CALL_P (can))
    {
      set_up_can_call_info (can);
      set_up_can_call_info (new_can);
    }
  if (global_p)
    CAN_GLOBAL_P (new_can) = true;
  can_vec = CAN_CONFLICT_CAN_VEC (can);
  gcc_assert (can_vec != NULL);
  for (i = 0; (c = can_vec [i]) != NULL; i++)
    {
      yara_free (CAN_CONFLICT_CAN_VEC (c));
      CAN_CONFLICT_CAN_VEC (c) = NULL;
      create_can_conflicts (c);
    }
  yara_free (can_vec);
  CAN_CONFLICT_CAN_VEC (can) = NULL;
  /* ??? internal_cost, dividing_nodes is needed for cluing.  */
  /* ??? can_copies  */
  /* cover_class_cost, memory_cost is not used by Chaitin??? */
  /* ??? hard_reg_costs */
  n = class_hard_regs_num [CAN_COVER_CLASS (new_can)];
  CAN_HARD_REG_COSTS (new_can) = yara_allocate (sizeof (int) * n);
  memset (CAN_HARD_REG_COSTS (new_can), 0, sizeof (int) * n);
  create_can_conflicts (can);
  create_can_conflicts (new_can);
  if (yara_dump_file != NULL)
    {
      fprintf (yara_dump_file, "+++Splitting %scan#%d into:\n",
	       (CAN_GLOBAL_P (can) ? "g" : ""), CAN_NUM (can));
      print_can (yara_dump_file, can);
      print_can (yara_dump_file, new_can);
    }
  return new_can;
}



/* ??? Call preferences of conflict cans.  */

/* This page contains function to choose hard register for cans.  */

/* Used for saving conflict_cans.  */
static can_t *saved_conflict_cans;

/* ??? */
static bitmap conflict_can_bitmap;
static bitmap biased_can_bitmap;

/* Function setting up conflicting, biased hard registers used for
   implementing biased coloring.  It also picks up conflicting can
   preference data.  */
static void
find_conflicting_biased_regs_and_costs (can_t can,
					HARD_REG_SET *conflicting_regs,
					int *conflicting_reg_costs,
					HARD_REG_SET *biased_regs)
{
  bitmap_iterator bi;
  unsigned int i;
  int j, size;
  enum reg_class cover_class;
  can_t conflict_can, conflict_can2;
  can_t *can_vec, *can_vec2;

  COPY_HARD_REG_SET (*conflicting_regs, CAN_CONFLICT_HARD_REGS (can));
  cover_class = CAN_COVER_CLASS (can);
  gcc_assert (cover_class != NO_REGS);
  memset (conflicting_reg_costs, 0,
	  sizeof (int) * class_hard_regs_num [cover_class]);
  bitmap_clear (biased_can_bitmap);
  bitmap_clear (conflict_can_bitmap);
  can_vec = CAN_CONFLICT_CAN_VEC (can);
  for (i = 0; (conflict_can = can_vec [i]) != NULL; i++)
    {
      if (cover_class != CAN_COVER_CLASS (conflict_can))
	continue;
      bitmap_set_bit (conflict_can_bitmap, CAN_NUM (conflict_can));
      if (! CAN_ASSIGNED_P (conflict_can))
	{
	  for (j = (int) class_hard_regs_num [cover_class] - 1; j >= 0; j--)
	    conflicting_reg_costs [j] += CAN_HARD_REG_COSTS (conflict_can) [j];
	  can_vec2 = CAN_CONFLICT_CAN_VEC (conflict_can);
	  for (j = 0; (conflict_can2 = can_vec2 [j]) != NULL; j++)
	    if (cover_class == CAN_COVER_CLASS (conflict_can2))
	      bitmap_set_bit (biased_can_bitmap, CAN_NUM (conflict_can2));
	}
      else if (CAN_HARD_REGNO (conflict_can) >= 0)
	{
	  size = (reg_class_nregs [cover_class] [CAN_MODE (conflict_can)]);
	  gcc_assert (size > 0);
	  for (j = 0; j < size; j++)
	    SET_HARD_REG_BIT (*conflicting_regs,
			      CAN_HARD_REGNO (conflict_can) + j);
	}
    }
  CLEAR_HARD_REG_SET (*biased_regs);
  EXECUTE_IF_SET_IN_BITMAP (biased_can_bitmap, 0, i, bi)
    {
      conflict_can = cans [i];
      if (CAN_HARD_REGNO (conflict_can) >= 0
	  && ! bitmap_bit_p (conflict_can_bitmap, i))
	{
	  size = reg_class_nregs [cover_class] [CAN_MODE (conflict_can)];
	  gcc_assert (size > 0);
	  for (j = 0; j < size; j++)
	    SET_HARD_REG_BIT (*biased_regs, CAN_HARD_REGNO (conflict_can) + j);
	}
    }
}

/* Function choosing a hard register for CAN.  */
static bool
choose_global_hard_reg (can_t can)
{
  HARD_REG_SET conflicting_regs, biased_regs;
  int conflicting_reg_costs [FIRST_PSEUDO_REGISTER];
  int i, hard_regno, best_hard_regno, cost, min_cost, class_size, *costs;
  short *hard_regs;
  bool call_p;
  enum reg_class cover_class, class;
  enum machine_mode mode;
  int j;
  can_t conflict_can;
  can_t *can_vec;
  int *conflict_costs;

  mode = CAN_MODE (can);
  cover_class = CAN_COVER_CLASS (can);
  class_size = class_hard_regs_num [cover_class];
  call_p = CAN_CALL_P (can);
  costs = CAN_HARD_REG_COSTS (can);
  if ((YARA_PARAMS & YARA_NO_UPDATE_COSTS) == 0)
    {

      COPY_HARD_REG_SET (conflicting_regs, CAN_CONFLICT_HARD_REGS (can));
      COPY_HARD_REG_SET (biased_regs, CAN_BIASED_HARD_REGS (can));
      can_vec = CAN_CONFLICT_CAN_VEC (can);
      for (i = 0; (conflict_can = can_vec [i]) != NULL; i++)
	{
	  if (cover_class != CAN_COVER_CLASS (conflict_can)
	      || CAN_ASSIGNED_P (conflict_can))
	    continue;
	  conflict_costs = CAN_HARD_REG_COSTS (conflict_can);
	  for (j = class_size - 1; j >= 0; j--)
	    costs [j] -= conflict_costs [j];
	}
    }
  else
    find_conflicting_biased_regs_and_costs
      (can, &conflicting_regs, conflicting_reg_costs, &biased_regs);
  IOR_HARD_REG_SET (conflicting_regs, no_alloc_regs);
  min_cost = -1;
  best_hard_regno = -1;
  hard_regs = class_hard_regs [cover_class];
  for (i = 0; i < class_size; i++)
    {
      hard_regno = hard_regs [i];
      if (hard_reg_not_in_set_p (hard_regno, mode, conflicting_regs))
	{
	  if ((YARA_PARAMS & YARA_NO_UPDATE_COSTS) == 0)
	    cost = costs [i];
	  else
	    cost = costs [i] - conflicting_reg_costs [i];
	  if (call_p
	      && ! hard_reg_not_in_set_p (hard_regno, mode, call_used_reg_set))
	    {
	      /* ??? If only part is call clobbered.  */
	      class = REGNO_REG_CLASS (hard_regno);
	      cost += (CAN_CALL_FREQ (can)
		       * (memory_move_cost [mode] [class] [0]
			  + memory_move_cost [mode] [class] [1]));
	    }
	  if (best_hard_regno < 0 || min_cost > cost)
	    {
	      min_cost = cost;
	      best_hard_regno = hard_regno;
	      gcc_assert (hard_regno >= 0);
	    }
	  else if (min_cost == cost
		   && TEST_HARD_REG_BIT (biased_regs, hard_regno)
		   && ! TEST_HARD_REG_BIT (biased_regs, best_hard_regno))
	    best_hard_regno = hard_regno;
	}
    }
  CAN_HARD_REGNO (can) = best_hard_regno;
  CAN_ASSIGNED_P (can) = true;
  if ((YARA_PARAMS & YARA_NO_UPDATE_COSTS) == 0)
    if (best_hard_regno >= 0)
      {
	can_t conflict_can2;
	can_t *can_vec2;
	
	can_vec = CAN_CONFLICT_CAN_VEC (can);
	for (i = 0; (conflict_can = can_vec [i]) != NULL; i++)
	  {
	    if (cover_class != CAN_COVER_CLASS (conflict_can)
		|| CAN_ASSIGNED_P (conflict_can))
	      continue;
	    IOR_HARD_REG_SET (CAN_CONFLICT_HARD_REGS (conflict_can),
			      reg_mode_hard_regset [best_hard_regno] [mode]);
	    can_vec2 = CAN_CONFLICT_CAN_VEC (conflict_can);
	    for (j = 0; (conflict_can2 = can_vec2 [j]) != NULL; j++)
	      {
		if (cover_class != CAN_COVER_CLASS (conflict_can2)
		    || CAN_ASSIGNED_P (conflict_can2))
		  continue;
		IOR_HARD_REG_SET (CAN_BIASED_HARD_REGS (conflict_can2),
				  reg_mode_hard_regset 
				  [best_hard_regno] [mode]);
	      }
	  }
    }
  return best_hard_regno >= 0;
}

/* The function updates costs of uncolored cans connected by copies of
   CAN.  */
static void
update_copy_costs (can_t can)
{
  int i, hard_regno, cost;
  enum machine_mode mode;
  enum reg_class class;
  can_t another_can;
  struct can_copy *cp;

  hard_regno = CAN_HARD_REGNO (can);
  gcc_assert (hard_regno >= 0 && CAN_COVER_CLASS (can) != NO_REGS);
  i = class_hard_reg_index [CAN_COVER_CLASS (can)] [hard_regno];
  class = REGNO_REG_CLASS (hard_regno);
  mode = CAN_MODE (can);
  for (cp = CAN_COPIES (can); cp != NULL; cp = cp->next_can_copy)
    {
      another_can = cp->can;
      if (CAN_ASSIGNED_P (another_can))
	continue;
      gcc_assert (CAN_COVER_CLASS (can) == CAN_COVER_CLASS (another_can));
      cost = (cp->to_p
	      ? register_move_cost [mode] [class]
	        [CAN_COVER_CLASS (another_can)]
	      : register_move_cost [mode] [CAN_COVER_CLASS (another_can)]
	        [class]);
      CAN_HARD_REG_COSTS (another_can) [i] -= cp->freq * cost;
    }
}



static can_t *sorted_cans;

/* This page contains global allocator based on Chaitin allgorithm.  */

/* Number of global cans.  */
static int global_cans_num;

/* Bucket of cans can be colored currently without spilling.  */
static can_t colorable_can_bucket;

/* Array of all buckets used for colouring.  I-th bucket contains cans
   conflicting with I other cans uncoloured yet.  */
static varray_type bucket_varray;

/* Add CAN to *BUCKET_PTR bucket.  CAN should be not in a bucket
   before the call.  */
static void
add_can_to_bucket (can_t can, can_t *bucket_ptr)
{
  can_t first_can;

  first_can = *bucket_ptr;
  CAN_NEXT_BUCKET_CAN (can) = first_can;
  CAN_PREV_BUCKET_CAN (can) = NULL;
  if (first_can != NULL)
    CAN_PREV_BUCKET_CAN (first_can) = can;
  *bucket_ptr = can;
}

/* Delete CAN from *BUCKET_PTR bucket.  It should be there before
   the call.  */
static void
delete_can_from_bucket (can_t can, can_t *bucket_ptr)
{
  can_t prev_can, next_can;

  prev_can = CAN_PREV_BUCKET_CAN (can);
  next_can = CAN_NEXT_BUCKET_CAN (can);
  if (prev_can != NULL)
    CAN_NEXT_BUCKET_CAN (prev_can) = next_can;
  else
    {
      gcc_assert (*bucket_ptr == can);
      *bucket_ptr = next_can;
    }
  if (next_can != NULL)
    CAN_PREV_BUCKET_CAN (next_can) = prev_can;
}

/* Varray representing the stack of global cans used during
   colouring.  */
static varray_type global_stack_varray;

/* Function used to sort cans according to their priorities to choose
   one for spilling.  */
static int
global_can_compare (const void *c1p, const void *c2p)
{
  can_t c1 = *(can_t *) c1p;
  can_t c2 = *(can_t *) c2p;
  int diff, size1, size2;

  if (CAN_IN_GRAPH_P (c1) && ! CAN_IN_GRAPH_P (c2))
    return -1;
  else if (! CAN_IN_GRAPH_P (c1) && CAN_IN_GRAPH_P (c2))
    return 1;
  else
    {
      gcc_assert (CAN_COVER_CLASS (c1) == CAN_COVER_CLASS (c2));
      size1 = reg_class_nregs [CAN_COVER_CLASS (c1)] [CAN_MODE (c1)];
      size2 = reg_class_nregs [CAN_COVER_CLASS (c2)] [CAN_MODE (c2)];
      diff = ((CAN_MEMORY_COST (c1) - CAN_COVER_CLASS_COST (c1))
	      / (CAN_LEFT_CONFLICTS_NUM (c1) + 1) * size1
	      - (CAN_MEMORY_COST (c2) - CAN_COVER_CLASS_COST (c2))
	      / (CAN_LEFT_CONFLICTS_NUM (c2) + 1) * size2);
      if (diff != 0)
	return diff;
      return CAN_NUM (c1) - CAN_NUM (c2);
    }
}

/* Push global cans on the stack.  The order of cans in the stack
   defines the order for the subsequent colouring.  */
static void
push_globals_to_stack (void)
{
  int i, size = 0, conflicts_num, conflict_size;
  int first_non_empty_bucket_num;
  can_t can, conflict_can;
  can_t *can_vec, *sorted_global_cans, *bucket_ptr;
  enum reg_class cover_class;
  int num, cover_class_cans_num [N_REG_CLASSES];
  can_t *cover_class_cans [N_REG_CLASSES];

  sorted_global_cans = yara_allocate (sizeof (can_t) * cans_num);
  for (i = 0; i < final_reg_class_cover_size; i++)
    {
      cover_class = final_reg_class_cover [i];
      cover_class_cans_num [cover_class] = 0;
      cover_class_cans [cover_class] = NULL;
    }
  for (i = 0; i < cans_num; i++)
    if (CAN_GLOBAL_P (cans [i])
	&& (cover_class = CAN_COVER_CLASS (cans [i])) != NO_REGS)
      cover_class_cans_num [cover_class]++;
  for (num = i = 0; i < final_reg_class_cover_size; i++)
    {
      cover_class = final_reg_class_cover [i];
      if (cover_class_cans_num [cover_class] != 0)
	{
	  cover_class_cans [cover_class] = sorted_global_cans + num;
	  num += cover_class_cans_num [cover_class];
	  cover_class_cans_num [cover_class] = 0;
	}
    }
  gcc_assert (num <= cans_num);
  for (i = 0; i < cans_num; i++)
    if (CAN_GLOBAL_P (cans [i])
	&& (cover_class = CAN_COVER_CLASS (cans [i])) != NO_REGS)
      cover_class_cans [cover_class] [cover_class_cans_num [cover_class]++]
	= cans [i];
  first_non_empty_bucket_num = 0;
  for (;;)
    {
      if (colorable_can_bucket != NULL)
	{
	  can = colorable_can_bucket;
	  cover_class = CAN_COVER_CLASS (can);
	  if (cover_class != NO_REGS)
	    size = reg_class_nregs [cover_class] [CAN_MODE (can)];
	  gcc_assert (CAN_LEFT_CONFLICTS_NUM (can)
		      + reg_class_nregs [cover_class] [CAN_MODE (can)]
		      <= available_class_regs [cover_class]);
	  bucket_ptr = &colorable_can_bucket;
	}
      else
	{
	  for (i = first_non_empty_bucket_num;
	       i < (int) VARRAY_ACTIVE_SIZE (bucket_varray);
	       i++)
	    if (VARRAY_GENERIC_PTR (bucket_varray, i) != NULL)
	      break;
	  if (i >= (int) VARRAY_ACTIVE_SIZE (bucket_varray))
	    break;
	  first_non_empty_bucket_num = i;
	  bucket_ptr = (can_t *) &VARRAY_GENERIC_PTR (bucket_varray, i);
	  can = *bucket_ptr;
	  gcc_assert (CAN_LEFT_CONFLICTS_NUM (can) == i);
	  cover_class = CAN_COVER_CLASS (can);
	  if (cover_class != NO_REGS)
	    {
	      size = reg_class_nregs [cover_class] [CAN_MODE (can)];
	      gcc_assert (size > 0);
	      if (i + size > available_class_regs [cover_class])
		{
		  num = cover_class_cans_num [cover_class];
		  gcc_assert (num > 0);
		  can_vec = cover_class_cans [cover_class];
		  qsort (can_vec, num, sizeof (can_t), global_can_compare);
		  for (num--; ! CAN_IN_GRAPH_P (can_vec [num]); num--)
		    ;
		  gcc_assert (num >= 0);
		  cover_class_cans_num [cover_class] = num;
		  can = can_vec [0];
		  cover_class_cans [cover_class]++;
		  size = reg_class_nregs [cover_class] [CAN_MODE (can)];
		  gcc_assert (CAN_IN_GRAPH_P (can)
			      && CAN_COVER_CLASS (can) == cover_class
			      && (CAN_LEFT_CONFLICTS_NUM (can) + size
				  > available_class_regs [cover_class]));
		  bucket_ptr = ((can_t *) &VARRAY_GENERIC_PTR
				(bucket_varray, CAN_LEFT_CONFLICTS_NUM (can)));
		}
	    }
	}
      delete_can_from_bucket (can, bucket_ptr);
      CAN_IN_GRAPH_P (can) = false;
      VARRAY_PUSH_GENERIC_PTR (global_stack_varray, can);
      if (cover_class == NO_REGS)
	continue;
      can_vec = CAN_CONFLICT_CAN_VEC (can);
      for (i = 0; (conflict_can = can_vec [i]) != NULL; i++)
	{
	  if (cover_class != CAN_COVER_CLASS (conflict_can))
	    continue;
	  if (CAN_IN_GRAPH_P (conflict_can))
	    {
	      conflicts_num = CAN_LEFT_CONFLICTS_NUM (conflict_can);
	      conflict_size
		= reg_class_nregs [cover_class] [CAN_MODE (conflict_can)];
	      gcc_assert (CAN_LEFT_CONFLICTS_NUM (conflict_can) >= size);
	      CAN_LEFT_CONFLICTS_NUM (conflict_can) -= size;
	      if (conflicts_num + conflict_size
		  <= available_class_regs [cover_class])
		continue;
	      delete_can_from_bucket
		(conflict_can,
		 (can_t *) &VARRAY_GENERIC_PTR (bucket_varray, conflicts_num));
	      conflicts_num = CAN_LEFT_CONFLICTS_NUM (conflict_can);
	      if (conflicts_num + conflict_size
		  <= available_class_regs [cover_class])
		add_can_to_bucket (conflict_can, &colorable_can_bucket);
	      else
		{
		  if (first_non_empty_bucket_num > conflicts_num)
		    first_non_empty_bucket_num = conflicts_num;
		  add_can_to_bucket
		    (conflict_can,
		     (can_t *) &VARRAY_GENERIC_PTR (bucket_varray,
						    conflicts_num));
		}
	    }
	}
    }
  yara_free (sorted_global_cans);
}

/* Varray implementing stack used to find allocnos forming a can for
   spilling.  */
static varray_type split_allocno_stack_varray;
/* Varray of allocnos forming a can which will be spilled.  */
static varray_type split_allocno_varray;

/* Varray of insn allocnos of can to be splitted.  */
static varray_type split_can_insn_allocno_varray;

static HARD_REG_SET split_allocno_available_regs;

/* Function putting allocno A onto stack for spilling.  Return true if
   it is sucessfull.  */
static bool
put_allocno_on_split_stack_for_reg (allocno_t a)
{
  int i, j, size, conflict_size, hard_regno;
  can_t can, conflict_can;
  enum reg_class cover_class;
  HARD_REG_SET used_hard_regs;

  if (bitmap_bit_p (split_allocno_bitmap, ALLOCNO_NUM (a)))
    return true;
  can = ALLOCNO_CAN (a);
  cover_class = CAN_COVER_CLASS (can);
  gcc_assert (cover_class != NO_REGS);
  size = reg_class_nregs [cover_class] [CAN_MODE (can)];
  gcc_assert (size > 0);
  find_allocno_conflicting_cans (a, true);
  COPY_HARD_REG_SET (used_hard_regs, no_alloc_regs);
  for (i = 0; i < conflict_cans_num; i++)
    {
      conflict_can = conflict_cans [i];
      if (cover_class != CAN_COVER_CLASS (conflict_can))
	continue;
      if (CAN_IN_GRAPH_P (conflict_can))
	{
	  if (CAN_HARD_REGNO (conflict_can) >= 0)
	    {
	      conflict_size
		= reg_class_nregs [cover_class][CAN_MODE (conflict_can)];
	      gcc_assert (conflict_size > 0);
	      for (j = 0; j < conflict_size; j++)
		SET_HARD_REG_BIT (used_hard_regs,
				  CAN_HARD_REGNO (conflict_can) + j);
	    }
	}
    }
  COPY_HARD_REG_SET (temp_hard_reg_set, split_allocno_available_regs);
  AND_COMPL_HARD_REG_SET (temp_hard_reg_set, used_hard_regs);
  AND_COMPL_HARD_REG_SET (temp_hard_reg_set, ALLOCNO_HARD_REG_CONFLICTS (a));
  if (reg_class_nregs [cover_class] [CAN_MODE (can)] == 1)
    {
      GO_IF_HARD_REG_EQUAL (temp_hard_reg_set, zero_hard_reg_set, fail);
    }
  else
    {
      for (i = (int) class_hard_regs_num [cover_class] - 1; i >= 0; i--)
	{
	  hard_regno = class_hard_regs [cover_class] [i];
	  if (! TEST_HARD_REG_BIT (temp_hard_reg_set, hard_regno))
	    continue;
	  for (j = reg_class_nregs [cover_class][CAN_MODE (can)] - 1;
	       j >= 1;
	       j--)
	  if (! TEST_HARD_REG_BIT (temp_hard_reg_set, hard_regno + j))
	    break;
	  if (j == 0)
	    break;
	}
      if (i < 0)
	goto fail;
    }
  COPY_HARD_REG_SET (split_allocno_available_regs, temp_hard_reg_set);
  VARRAY_PUSH_GENERIC_PTR (split_allocno_stack_varray, a);
  VARRAY_PUSH_GENERIC_PTR (split_allocno_varray, a);
  bitmap_set_bit (split_allocno_bitmap, ALLOCNO_NUM (a));
  return true;
 fail:
  return false;
}

/* Function trying to split global CAN.  CAN presenting the part of
   the input can is supposed to be spilled.  The new can (if any) will
   be put on the global stack.  If splitting occurs, the function
   returns true.  */
static bool
split_global_can (can_t can)
{
  allocno_t a, *can_allocnos;
  int i, stack_size, usage_freq, border_freq, start;
  int best_usage_freq, best_border_freq, best_start;
  copy_t cp;
  can_t new_can;

  gcc_assert (CAN_GLOBAL_P (can));
  can_allocnos = CAN_ALLOCNOS (can);
  for (i = 0; (a = can_allocnos [i]) != NULL; i++)
    {
      VARRAY_POP_ALL (split_allocno_stack_varray);
      VARRAY_POP_ALL (split_allocno_varray);
      COPY_HARD_REG_SET (split_allocno_available_regs,
			 reg_class_contents [CAN_COVER_CLASS (can)]);
      bitmap_clear (split_allocno_bitmap);
      if (! put_allocno_on_split_stack_for_reg (a))
	continue;
      best_usage_freq = usage_freq = 0;
      best_border_freq = border_freq = 0;
      best_start = start = 0;
      /* Split: find connected allocnos for splitting as many as
	 possible.  */
      for (;;)
	{
	  stack_size = VARRAY_ACTIVE_SIZE (split_allocno_stack_varray);
	  if (stack_size == 0)
	    break;
	  a = VARRAY_GENERIC_PTR (split_allocno_stack_varray,
				  stack_size - 1);
	  VARRAY_POP (split_allocno_stack_varray);
	  if (ALLOCNO_TYPE (a) == INSN_ALLOCNO)
	    usage_freq += ALLOCNO_FREQ (a);
	  for (cp = ALLOCNO_DST_COPIES (a);
	       cp != NULL;
	       cp = COPY_NEXT_DST_COPY (cp))
	    if (COPY_SRC (cp) != a)
	      {
		if (bitmap_bit_p (split_allocno_bitmap,
				  ALLOCNO_NUM (COPY_SRC (cp))))
		  border_freq -= COPY_FREQ (cp);
		else
		  border_freq += COPY_FREQ (cp);
	      }
	  for (cp = ALLOCNO_SRC_COPIES (a);
	       cp != NULL;
	       cp = COPY_NEXT_SRC_COPY (cp))
	    if (COPY_DST (cp) != a)
	      {
		if (bitmap_bit_p (split_allocno_bitmap,
				  ALLOCNO_NUM (COPY_DST (cp))))
		  border_freq -= COPY_FREQ (cp);
		else
		  border_freq += COPY_FREQ (cp);
	      }
	  for (cp = ALLOCNO_DST_COPIES (a);
	       cp != NULL;
	       cp = COPY_NEXT_DST_COPY (cp))
	    if (COPY_SRC (cp) != a && ALLOCNO_CAN (COPY_SRC (cp)) == can)
	      put_allocno_on_split_stack_for_reg (COPY_SRC (cp));
	  for (cp = ALLOCNO_SRC_COPIES (a);
	       cp != NULL;
	       cp = COPY_NEXT_SRC_COPY (cp))
	    if (COPY_DST (cp) != a && ALLOCNO_CAN (COPY_DST (cp)) == can)
	      put_allocno_on_split_stack_for_reg (COPY_DST (cp));
	}
      gcc_assert (border_freq >= 0);
      if (usage_freq <= YARA_SPLIT_THRESHOLD * border_freq)
	continue;
      new_can = split_can_by_split_allocno_bitmap (can, true);
      gcc_assert (new_can != NULL);
      VARRAY_PUSH_GENERIC_PTR (global_stack_varray, new_can);
      global_cans_num++;
      return true;
    }
  return false;
}

/* Assign hard registers to cans on the stack (colour the cans).  */
static void
pop_globals_from_stack (void)
{
  can_t can;
  int stack_size;
  enum reg_class cover_class;

  VARRAY_GENERIC_PTR_NOGC_INIT (split_allocno_stack_varray, allocnos_num,
				"stack used for splitting global cans");
  VARRAY_GENERIC_PTR_NOGC_INIT
    (split_allocno_varray, allocnos_num,
     "allocnos of can which forms new can for splitting");
  VARRAY_GENERIC_PTR_NOGC_INIT (split_can_insn_allocno_varray, allocnos_num,
				"insn allocnos of can to be splitted");
  for (;;)
    {
      stack_size = VARRAY_ACTIVE_SIZE (global_stack_varray);
      if (stack_size == 0)
	break;
      can = VARRAY_GENERIC_PTR (global_stack_varray, stack_size - 1);
      VARRAY_POP (global_stack_varray);
      cover_class = CAN_COVER_CLASS (can);
      if (cover_class == NO_REGS)
	{
	  CAN_IN_GRAPH_P (can) = true;
	  CAN_HARD_REGNO (can) = -1;
	  continue;
	}
      if (choose_global_hard_reg (can))
	{
	  CAN_IN_GRAPH_P (can) = true;
	  update_copy_costs (can);
	  continue;
	}
      gcc_assert (CAN_SLOTNO (can) == CAN_NUM (can));
      if (flag_split && split_global_can (can))
	{
	  /* Process CAN again.  */
	  VARRAY_PUSH_GENERIC_PTR (global_stack_varray, can);
	  continue;
	}
      /* ??? local alloc for conflicting and preferenced.  */
      CAN_IN_GRAPH_P (can) = true;
      CAN_HARD_REGNO (can) = -1;
    }
  VARRAY_FREE (split_can_insn_allocno_varray);
  VARRAY_FREE (split_allocno_stack_varray);
  VARRAY_FREE (split_allocno_varray);
}

/* Function assigning hard-registers to all global cans (colouring
   the global cans).  */
static void
global_can_alloc (void)
{
  int i, j, hard_regno, conflict_cans_size;
  can_t can, conflict_can;
  can_t *can_vec;
  enum reg_class cover_class;
  HARD_REG_SET temp_set;

  global_cans_num = 0;
  /* Put the global cans into the corresponding buckets.  */
  colorable_can_bucket = NULL;
  VARRAY_GENERIC_PTR_NOGC_INIT (bucket_varray, cans_num,
				"buckets of global cans");
  for (i = 0; i < cans_num; i++)
    {
      can = cans [i];
      if (! CAN_GLOBAL_P (can))
	continue;
      global_cans_num++;
      cover_class = CAN_COVER_CLASS (can);
      COPY_HARD_REG_SET (temp_set, CAN_CONFLICT_HARD_REGS (can));
      AND_HARD_REG_SET (temp_set, reg_class_contents [cover_class]);
      AND_COMPL_HARD_REG_SET (temp_set, no_alloc_regs);
      conflict_cans_size = 0;
      for (j = 0; j < (int) class_hard_regs_num [cover_class]; j++)
	{
	  hard_regno = class_hard_regs [cover_class] [j];
	  if (TEST_HARD_REG_BIT (temp_set, hard_regno))
	    {
	      conflict_cans_size++;
	      CLEAR_HARD_REG_BIT (temp_set, hard_regno);
	      GO_IF_HARD_REG_EQUAL (temp_set, zero_hard_reg_set, skip);
	    }
	}
    skip:
      can_vec = CAN_CONFLICT_CAN_VEC (can);
      for (j = 0; (conflict_can = can_vec [j]) != NULL; j++)
	{
	  if (cover_class != CAN_COVER_CLASS (conflict_can))
	    continue;
	  conflict_cans_size
	    += reg_class_nregs [cover_class] [CAN_MODE (conflict_can)];
	}
      CAN_IN_GRAPH_P (can) = true;
      CAN_LEFT_CONFLICTS_NUM (can) = conflict_cans_size;
      while (conflict_cans_size >= (int) VARRAY_ACTIVE_SIZE (bucket_varray))
	VARRAY_PUSH_GENERIC_PTR (bucket_varray, NULL);
      if (conflict_cans_size + reg_class_nregs [cover_class] [CAN_MODE (can)]
	  <= available_class_regs [cover_class])
	add_can_to_bucket (can, &colorable_can_bucket);
      else
	add_can_to_bucket (can,
			   (can_t *) &VARRAY_GENERIC_PTR (bucket_varray,
							  conflict_cans_size));
    }
  VARRAY_GENERIC_PTR_NOGC_INIT (global_stack_varray, global_cans_num,
				"stack of global cans");
  push_globals_to_stack ();
  pop_globals_from_stack ();
  VARRAY_FREE (global_stack_varray);
  VARRAY_FREE (bucket_varray);
}



static int *global_can_priorities;

/* Function used to sort cans according to their priorities to choose
   one for spilling.  */
static int
global_can_priority_compare (const void *c1p, const void *c2p)
{
  can_t c1 = *(can_t *) c1p;
  can_t c2 = *(can_t *) c2p;
  int diff;
  int c1_num = CAN_NUM (c1);
  int c2_num = CAN_NUM (c2);

  if ((diff = global_can_priorities [c2_num] - global_can_priorities [c1_num])
      != 0)
    return diff;
  return c1_num - c2_num;
}

static void
priority_color (void)
{
  int i, n, k, conflicts_num, insn_allocnos_num;
  allocno_t a, *can_allocnos;
  can_t can;
  can_t *sorted_global_cans, *can_vec;

  sorted_global_cans = yara_allocate (sizeof (can_t) * cans_num);
  global_can_priorities = yara_allocate (sizeof (int) * cans_num);
  for (n = i = 0; i < cans_num; i++)
    if (CAN_GLOBAL_P (cans [i]))
      {
	can = cans [i];
	sorted_global_cans [n++] = can;
	can_vec = CAN_CONFLICT_CAN_VEC (can);
	for (conflicts_num = 0;
	     can_vec [conflicts_num] != NULL;
	     conflicts_num++)
	  ;
	can_allocnos = CAN_ALLOCNOS (can);
	for (insn_allocnos_num = k = 0; (a = can_allocnos [k]) != NULL; k++)
	  if (ALLOCNO_TYPE (a) == INSN_ALLOCNO)
	    insn_allocnos_num++;
	if (insn_allocnos_num == 0)
	  {
	    /* It might be because of inaccurate live information.  */
	    global_can_priorities [i] = 0;
	    continue;
	  }
	global_can_priorities [i]
	  = (((double) (floor_log2 (insn_allocnos_num + 1)
			* (CAN_MEMORY_COST (can) - CAN_COVER_CLASS_COST (can)))
	      / (conflicts_num + 1))
	     * GET_MODE_SIZE (CAN_MODE (can)));
      }
  global_cans_num = n;
  qsort (sorted_global_cans, n, sizeof (can_t), global_can_priority_compare);
  for (i = 0; i < n; i++)
    {
      can = sorted_global_cans [i];
      if (choose_global_hard_reg (can))
	update_copy_costs (can);
    }
  yara_free (global_can_priorities);
  yara_free (sorted_global_cans);
}



/* Function used to sort cans according to their priorities to assign
   memory.  */
static int
mem_can_assignment_compare (const void *c1p, const void *c2p)
{
  can_t c1 = *(can_t *) c1p;
  can_t c2 = *(can_t *) c2p;
  int diff;

  diff = (CAN_MEMORY_COST (c2) - CAN_MEMORY_COST (c1));
  if (diff != 0)
    return diff;
  return CAN_NUM (c1) - CAN_NUM (c2);
}

/* Propagate can allocation to the corresponding allocnos.  */
static void
assign_global_can_allocnos (void)
{
  int i, n, hard_regno;
  enum reg_class cover_class;
  allocno_t a, *can_allocnos;
  can_t can;
  bool ok_p;

  sorted_cans = yara_allocate (sizeof (can_t) * cans_num);
  memcpy (sorted_cans, cans, cans_num * sizeof (can_t));
  /* If we assign memory (or hard register) to higher priority first
     we might get shorter displacement (or more preferred hard
     register) and get smaller and faster code.  */
  qsort (sorted_cans, cans_num, sizeof (can_t), mem_can_assignment_compare);
  for (i = 0; i < cans_num; i++)
    {
      can = sorted_cans [i];
      if (! CAN_GLOBAL_P (can))
	continue;
      cover_class = CAN_COVER_CLASS (can);
      hard_regno = CAN_HARD_REGNO (can);
      can_allocnos = CAN_ALLOCNOS (can);
      for (n = 0; (a = can_allocnos [n]) != NULL; n++)
	if (ALLOCNO_TYPE (a) != INSN_ALLOCNO)
	  {
	    if (hard_regno < 0
		|| (ALLOCNO_CALL_CROSS_P (a)
		    && ! hard_reg_not_in_set_p (hard_regno, CAN_MODE (can),
						call_used_reg_set)))
	      ok_p = assign_allocno (a, NO_REGS, reg_class_contents [NO_REGS],
				     -1);
	    else
	      ok_p = assign_allocno (a, cover_class,
				     reg_class_contents [cover_class],
				     hard_regno);
#if 1
	    if (! ok_p)
	      ok_p = assign_allocno (a, NO_REGS, reg_class_contents [NO_REGS],
				     -1);
#endif
	    gcc_assert (ok_p);
	  }
    }
  yara_free (sorted_cans);
}



/* This page contains local allocator.  */

/* The current basic block for which we do local allocation.  */
static basic_block curr_bb;

/* Map: BB position -> the corresponding insn.  */
static GTY(()) varray_type pos_insn_map;

/* Map: insn -> the corresponding BB position.  */
static int *insn_pos_map;

/* Function to set up liveness information of cans for the current
   basic block.  */
static void
set_up_bb_pos_insn_maps (void)
{
  rtx insn, bound;

  bound = NEXT_INSN (BB_END (curr_bb));
  VARRAY_POP_ALL (pos_insn_map);
  for (insn = BB_HEAD (curr_bb); insn != bound; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      {
	VARRAY_PUSH_RTX (pos_insn_map, insn);
	insn_pos_map [INSN_UID (insn)] = VARRAY_ACTIVE_SIZE (pos_insn_map);
      }
}

/* ??? */
static void
assign_dst_if_necessary (copy_t cp)
{
  int hard_regno;
  allocno_t src, dst;
  can_t can;
  enum reg_class cover_class;

  src = COPY_SRC (cp);
  dst = COPY_DST (cp);
  if (src == NULL || dst == NULL)
    return;
  gcc_assert (ALLOCNO_REGNO (dst) >= 0
	      && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (dst)));
  hard_regno = ALLOCNO_HARD_REGNO (src);
  if (ALLOCNO_HARD_REGNO (dst) >= 0 || ALLOCNO_MEMORY_SLOT (dst) != NULL)
    return;
  can = ALLOCNO_CAN (dst);
  cover_class = CAN_COVER_CLASS (can);
  if (! (hard_regno >= 0
	 && assign_allocno (dst, cover_class,
			    reg_class_contents [cover_class],
			    hard_regno))
      && ! assign_allocno (dst, cover_class,
			   reg_class_contents [cover_class], -1))
    {
      if (! assign_allocno (dst, NO_REGS, reg_class_contents [NO_REGS], -1))
	gcc_unreachable ();
    }
}



struct allocno_list_elem
{
  allocno_t a;
  struct allocno_list_elem *next;
};

static struct allocno_list_elem *free_allocno_list_elems;

/* Map hard register to allocnos using it.  There might be more one
   allocnos because of allocnos with the same value do not
   conflict.  */
static struct allocno_list_elem *
  hard_regno_to_allocno_map [FIRST_PSEUDO_REGISTER];

static void
initiate_hard_regno_to_allocno_map (void)
{
  int i;

  free_allocno_list_elems = NULL;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    hard_regno_to_allocno_map [i] = NULL;
}

static void
add_allocno_list_elem (int hard_regno, allocno_t a)
{
  struct allocno_list_elem *elem;

  for (elem = hard_regno_to_allocno_map [hard_regno];
       elem != NULL;
       elem = elem->next)
    if (elem->a == a)
      return;
  if (free_allocno_list_elems == NULL)
    elem = yara_allocate (sizeof (struct allocno_list_elem));
  else
    {
      elem = free_allocno_list_elems;
      free_allocno_list_elems = free_allocno_list_elems->next;
    }
  elem->a = a;
  elem->next = hard_regno_to_allocno_map [hard_regno];
  hard_regno_to_allocno_map [hard_regno] = elem;
}

static void
free_allocno_list (int hard_regno)
{
  struct allocno_list_elem *list, *last;
  
  list = hard_regno_to_allocno_map [hard_regno];
  if (list == NULL)
    return;
  for (last = list; last->next != NULL; last = last->next)
    ;
  last->next = free_allocno_list_elems;
  free_allocno_list_elems = list;
  hard_regno_to_allocno_map [hard_regno] = NULL;
}

static void
finish_hard_regno_to_allocno_map (void)
{
  int i;
  struct allocno_list_elem *next;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    free_allocno_list (i);
  for (; free_allocno_list_elems != NULL; free_allocno_list_elems = next)
    {
      next = free_allocno_list_elems->next;
      yara_free (free_allocno_list_elems);
    }
}



/* Function providing HARD_REGNO for allocno A by spilling allocnos
   using the hard registers.  In case of success, it returns true and
   sum of positions of the next usage of the spilled allocnos trough
   POS.  */
static bool
spill_hard_regno_for_allocno (allocno_t a, int hard_regno, int *pos)
{
  int i, size;
  enum machine_mode mode;
  rtx insn;
  allocno_t conflict_a;
  struct allocno_list_elem *curr;

  mode = ALLOCNO_MODE (a);
  size = hard_regno_nregs [hard_regno][mode];
  if (INSN_ALLOCNO_TIED_ALLOCNO (a) != NULL)
    {
      mode = ALLOCNO_MODE (INSN_ALLOCNO_TIED_ALLOCNO (a));
      if (size < hard_regno_nregs [hard_regno][mode])
	size = hard_regno_nregs [hard_regno][mode];
    }
  *pos = 0;
  for (i = 0; i < size; i++)
    {
      for (curr = hard_regno_to_allocno_map [hard_regno + i];
	   curr != NULL;
	   curr = curr->next)
	{
	  conflict_a = curr->a;
	  if (ALLOCNO_HARD_REGNO (conflict_a) < 0)
	    continue; /* already processed. */
	  gcc_assert (ALLOCNO_TYPE (conflict_a) != INSN_ALLOCNO);
	  if (ALLOCNO_TYPE (conflict_a) == REGION_ALLOCNO
	      || (insn = RANGE_ALLOCNO_STOP_INSN (conflict_a)) == NULL_RTX)
	    *pos += VARRAY_ACTIVE_SIZE (pos_insn_map) + 1;
	  else
	    *pos += insn_pos_map [INSN_UID (insn)];
	  unassign_allocno (conflict_a);
	  if (! assign_allocno (conflict_a, NO_REGS,
				reg_class_contents [NO_REGS], -1))
	    return false;
	}
    }
  return true;
}


/* Function assigning hard registers of CLASS starting with HARD_REGNO
   to allocno A with spilling allocnos using the hard registers.  In
   case of sucess the function returns true and sum of positions of
   the next usage of the spilled allocnos.  */
static bool
assign_with_spill (allocno_t a, enum reg_class class, int hard_regno, int *pos)
{
  copy_t cp;

  if (! spill_hard_regno_for_allocno (a, hard_regno, pos))
    return false;
  if (! assign_allocno (a, class, reg_class_contents [class], hard_regno))
    return false;
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    assign_dst_if_necessary (cp);
  return true;
}

/* ??? */
static void
set_up_hard_regno_to_allocno_map (allocno_t a, HARD_REG_SET *prohibited_regs)
{
  int i, j, hard_regno, size;
  enum machine_mode conflict_mode;
  allocno_t conflict_a;
  allocno_t *vec;

  /* ??? assigning another hard reg, swap  */
  vec = ALLOCNO_CONFLICT_VEC (a);
  for (j = 0; (conflict_a = vec [j]) != NULL; j++)
    {
      hard_regno = ALLOCNO_HARD_REGNO (conflict_a);
      conflict_mode = ALLOCNO_MODE (conflict_a);
      if (hard_regno < 0
	  || hard_reg_in_set_p (hard_regno, conflict_mode, *prohibited_regs))
	continue;
      if (ALLOCNO_TYPE (conflict_a) == INSN_ALLOCNO)
	{
	  IOR_HARD_REG_SET (*prohibited_regs,
			    ALLOCNO_HARD_REGSET (conflict_a));
	  IOR_HARD_REG_SET
	    (*prohibited_regs,
	     INSN_ALLOCNO_INTERM_ELIMINATION_REGSET (conflict_a));
	  continue;
	}
      size = hard_regno_nregs [hard_regno] [conflict_mode];
      for (i = 0; i < size; i++)
	add_allocno_list_elem (hard_regno + i, conflict_a);
    }
}

/* This function is called by YARA reload to provide a hard register of
   CLASS to allocno A.  The function returns the hard register number
   and negative value in case of failure. */
static int
provide_allocno_class_hard_reg (allocno_t a, enum reg_class class)
{
  int i, hard_regno, best_hard_regno, cost, best_cost, pos, best_pos, add_cost;
  enum machine_mode mode;
  HARD_REG_SET prohibited;
  can_t can;
  enum reg_class cover_class;

  gcc_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
  mode = ALLOCNO_MODE (a);
  best_pos = best_cost = best_hard_regno = -1;
  SET_HARD_REG_SET (prohibited);
  AND_COMPL_HARD_REG_SET (prohibited, reg_class_contents [class]);
  IOR_HARD_REG_SET (prohibited, no_alloc_regs);
  for (i = 0; i < (int) class_hard_regs_num [class]; i++)
    free_allocno_list (class_hard_regs [class] [i]);
  set_up_hard_regno_to_allocno_map (a, &prohibited);
  if (INSN_ALLOCNO_TIED_ALLOCNO (a) != NULL)
    set_up_hard_regno_to_allocno_map (INSN_ALLOCNO_TIED_ALLOCNO (a),
				      &prohibited);
  can = (ALLOCNO_REGNO (a) >= 0 && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a))
	 ? ALLOCNO_CAN (a) : NULL);
  cover_class = (can == NULL ? NO_REGS : CAN_COVER_CLASS (can));
  for (i = 0; i < (int) class_hard_regs_num [class]; i++)
    {
      hard_regno = class_hard_regs [class] [i];
      if (! hard_reg_not_in_set_p (hard_regno, mode, prohibited))
	continue;
      start_transaction ();
      cost = global_allocation_cost;
      if (assign_with_spill (a, class, hard_regno, &pos))
	{
	  add_cost = 0;
	  if (cover_class != NO_REGS
	      && hard_reg_in_set_p (hard_regno, mode,
				    reg_class_contents [cover_class]))
	    add_cost = CAN_HARD_REG_COSTS (can) [class_hard_reg_index
						 [cover_class] [hard_regno]];
	  cost = global_allocation_cost - cost + add_cost;
	  if (best_hard_regno < 0 || best_cost > cost)
	    {
	      best_hard_regno = hard_regno;
	      best_cost = cost;
	      best_pos = pos;
	    }
	  else if (best_cost == cost && best_pos < pos)
	    {
	      best_hard_regno = hard_regno;
	      best_pos = pos;
	    }
	}
      undo_transaction ();
    }
  if (best_hard_regno < 0)
    return -1;
  if (! assign_with_spill (a, class, best_hard_regno, &pos))
    gcc_unreachable ();
  return best_hard_regno;
}

/* The function returns true if the can of A crosses a call.  */
static bool
call_cross_hint (allocno_t a)
{
  gcc_assert (ALLOCNO_TYPE (a) != INSN_ALLOCNO);
  /* ??? can we improve it or should we prohibit it.  */
  return CAN_CALL_P (ALLOCNO_CAN (a));
}

static rtx curr_allocation_insn;
static int curr_allocation_insn_alt;

/* Recursive functions picking up all registers *SP in
   CONSTRAINTS.  */
static void
pick_up_hard_regs_from_constraints (const char *constraints,
				    HARD_REG_SET *sp, bool *mem_too_p)
{
  char c;
  bool flag;
  enum reg_class cl;
  struct insn_op_info *info;

  *mem_too_p = false;
  if (constraints == NULL || *constraints == '\0')
    {
      SET_HARD_REG_SET (*sp);
      return;
    }
  for (; (c = *constraints); constraints += CONSTRAINT_LEN (c, constraints))
    switch (c)
      {
	 /* ??? what about constants and other memory. */
      case 'm':
      case 'o':
	*mem_too_p = true;
	break;

      case 'g':
	*mem_too_p = true;
	/* Fall through: */
      case 'r':
      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
      case 'h': case 'j': case 'k': case 'l':
      case 'q': case 't': case 'u':
      case 'v': case 'w': case 'x': case 'y': case 'z':
      case 'A': case 'B': case 'C': case 'D':
      case 'Q': case 'R': case 'S': case 'T': case 'U':
      case 'W': case 'Y': case 'Z':
	 cl = (c == 'g' || c == 'r'
	       ? GENERAL_REGS : REG_CLASS_FROM_CONSTRAINT (c, constraints));
	 if (cl != NO_REGS)
	   IOR_HARD_REG_SET (*sp, reg_class_contents [cl]);
	 break;

      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
	 info = insn_infos [INSN_UID (curr_allocation_insn)];
	 pick_up_hard_regs_from_constraints
	   (info->op_constraints [(c - '0') * info->n_alts
				  + curr_allocation_insn_alt], sp, &flag);
	 *mem_too_p = *mem_too_p || flag;
	 break;
      default:
	gcc_assert (c != '#');
      }
}

static void
find_constraint_hard_reg_set (allocno_t a, HARD_REG_SET *sp,
			      bool *mem_too_p, bool *constraint_p)
{
  int n;
  struct insn_op_info *info;

  *mem_too_p = false;
  info = insn_infos [INSN_UID (curr_allocation_insn)];
  if (INSN_ALLOCNO_TYPE (a) == BASE_REG)
    COPY_HARD_REG_SET
      (*sp, base_regs [GET_MODE (*INSN_ALLOCNO_CONTAINER_LOC (a))]);
  else if (INSN_ALLOCNO_TYPE (a) == INDEX_REG)
    COPY_HARD_REG_SET (*sp, index_regs);
  else if (INSN_ALLOCNO_TYPE (a) == NON_OPERAND)
    /* ??? GENERAL_REGS */
    COPY_HARD_REG_SET (*sp, reg_class_contents [GENERAL_REGS]);
  else if (info == NULL || info->n_alts == 0)
    SET_HARD_REG_SET (*sp);
  else
    {
      CLEAR_HARD_REG_SET (*sp);
      n = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
      pick_up_hard_regs_from_constraints
	 (info->op_constraints [n * info->n_alts + curr_allocation_insn_alt],
	  sp, mem_too_p);
    }
  *constraint_p = true;
#ifdef NO_TRY_CONNECTION
  {
    copy_t cp;

    CLEAR_HARD_REG_SET (temp_hard_reg_set);
    for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
      if (COPY_SRC (cp) != NULL && ALLOCNO_HARD_REGNO (COPY_SRC (cp)) >= 0)
	SET_HARD_REG_BIT (temp_hard_reg_set,
			  ALLOCNO_HARD_REGNO (COPY_SRC (cp)));
    for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
      if (COPY_DST (cp) != NULL && ALLOCNO_HARD_REGNO (COPY_DST (cp)) >= 0)
	SET_HARD_REG_BIT (temp_hard_reg_set,
			  ALLOCNO_HARD_REGNO (COPY_DST (cp)));
    AND_HARD_REG_SET (temp_hard_reg_set, *sp);
    GO_IF_HARD_REG_EQUAL (temp_hard_reg_set, zero_hard_reg_set, eq);
    GO_IF_HARD_REG_EQUAL (temp_hard_reg_set, *sp, eq);
    *constraint_p = false;
    COPY_HARD_REG_SET (*sp, temp_hard_reg_set);
  eq:
    ;
  }
#endif
}

static int
connected_assigned_allocnos (allocno_t a)
{
  copy_t cp;
  int num = 0;

  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
    if (COPY_SRC (cp) != COPY_DST (cp) && COPY_SRC (cp) != NULL
	&& (ALLOCNO_HARD_REGNO (COPY_SRC (cp)) >= 0
	    || ALLOCNO_MEMORY_SLOT (COPY_SRC (cp)) != NULL))
      num++;
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    if (COPY_SRC (cp) != COPY_DST (cp) && COPY_DST (cp) != NULL
	&& (ALLOCNO_HARD_REGNO (COPY_DST (cp)) >= 0
	    || ALLOCNO_MEMORY_SLOT (COPY_DST (cp)) != NULL))
      num++;
  return num;
}

static int
insn_allocno_order_compare (const void *a1p, const void *a2p)
{
  allocno_t a1 = *(allocno_t *) a1p;
  allocno_t a2 = *(allocno_t *) a2p;
  HARD_REG_SET hard_regset1, hard_regset2;
  bool mem_too_p1, mem_too_p2, constraint_p1, constraint_p2;

  find_constraint_hard_reg_set (a1, &hard_regset1,
				&mem_too_p1, &constraint_p1);
  find_constraint_hard_reg_set (a2, &hard_regset2,
				&mem_too_p2, &constraint_p2);
  GO_IF_HARD_REG_EQUAL (hard_regset1, hard_regset2, eq);
  GO_IF_HARD_REG_SUBSET (hard_regset1, hard_regset2, strict_incl1);
  GO_IF_HARD_REG_SUBSET (hard_regset2, hard_regset1, strict_incl2);

 eq:
  if (! mem_too_p1 && mem_too_p2)
    return -1;
  else if (mem_too_p1 && ! mem_too_p2)
    return 1;
  else if (constraint_p1 && ! constraint_p2)
    return -1;
  else if (! constraint_p1 && constraint_p2)
    return 1;

#ifndef NO_TRY_CONNECTION
    {
      int diff
	= connected_assigned_allocnos (a2) - connected_assigned_allocnos(a1);

      if (diff)
	return diff;
    }
#endif

  /* Use the same order as in the list insn_allocnos.  */
  return ALLOCNO_NUM (a2) - ALLOCNO_NUM (a1);

 strict_incl1:
  /* We should process A1 first because it is strict subset of A2.  */
  return -1;

 strict_incl2:
  /* We should process A2 first because it is strict subset of A1.  */
  return 1;
}

static void
insn_allocno_sort (allocno_t *arr, int size, rtx insn, int alt)
{
  curr_allocation_insn = insn;
  curr_allocation_insn_alt = alt;
  qsort (arr, size, sizeof (allocno_t), insn_allocno_order_compare);
}

/* Function doing allocation of the rest local cans spilling a can
   with the latest usage and splitting live ranges of global cans.  */ 
static void
one_pass_allocate (void)
{
  int i;
  rtx insn;
  copy_t cp;

  initiate_hard_regno_to_allocno_map ();
  for (i = 1; i <= (int) VARRAY_ACTIVE_SIZE (pos_insn_map); i++)
    {
      insn = VARRAY_RTX (pos_insn_map, i - 1);
      allocate_insn_allocnos (insn, call_cross_hint, insn_allocno_sort,
			      provide_allocno_class_hard_reg);
      for (cp = after_insn_copies [INSN_UID (insn)];
	   cp != NULL;
	   cp = COPY_NEXT_COPY (cp))
	assign_dst_if_necessary (cp);
    }
  finish_hard_regno_to_allocno_map ();
}

/* Start point of register allocation in the current basic block.  */
static void
bb_local_can_alloc (void)
{
  set_up_bb_pos_insn_maps ();
  one_pass_allocate ();
}

/* Function used to sort basic block in order of decreasing their
   frequencies.  */
static int
bb_freq_compare (const void *bb1p, const void *bb2p)
{
  basic_block bb1 = *(basic_block *) bb1p;
  basic_block bb2 = *(basic_block *) bb2p;
  
  return bb2->frequency - bb1->frequency;
}

/* Function doing register allocation in scope of basic blocks
   (register allocation of cans living only in one BB).  */
static void
local_can_alloc (void)
{
  int i;
  basic_block bb;
  varray_type bb_varray;

  VARRAY_GENERIC_PTR_NOGC_INIT (bb_varray, n_basic_blocks, "basic blocks");
  FOR_EACH_BB (bb)
    {
      VARRAY_PUSH_GENERIC_PTR (bb_varray, bb);
    }
  qsort (&VARRAY_GENERIC_PTR (bb_varray, 0), VARRAY_ACTIVE_SIZE (bb_varray),
	 sizeof (basic_block), bb_freq_compare);
  VARRAY_RTX_INIT (pos_insn_map, yara_max_uid, "map: bb position -> insn");
  insn_pos_map = yara_allocate (sizeof (int) * yara_max_uid);
  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (bb_varray); i++)
    {
      curr_bb = VARRAY_GENERIC_PTR (bb_varray, i);
      bb_local_can_alloc ();
    }
  yara_free (insn_pos_map);
  VARRAY_FREE (bb_varray);
}



/* Function assigning elimination hard registers of CLASS starting
   with HARD_REGNO to allocno A with spilling allocnos using the hard
   registers.  In case of success the function returns true and sum of
   positions of the next usage of the spilled allocnos.  */
static bool
assign_elimination_reg_with_spill (allocno_t a, enum reg_class class,
				   HARD_REG_SET possible_regs,
				   int hard_regno, int *pos)
{
  if (! spill_hard_regno_for_allocno (a, hard_regno, pos))
    return false;
  return assign_elimination_reg (a, class, possible_regs, hard_regno);
}

/* This function is called by YARA transactions to provide elimination
   hard registers of CLASS in set POSSIBLE_REGS to allocno A.  The
   function returns the hard register number and negative value in
   case of failure. */
static int
provide_allocno_elimination_class_hard_reg (allocno_t a, enum reg_class class,
					    HARD_REG_SET possible_regs)
{
  int i, hard_regno, best_hard_regno, cost, best_cost, pos, best_pos;
  enum machine_mode mode;
  HARD_REG_SET prohibited;

  gcc_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
  mode = ALLOCNO_MODE (a);
  best_pos = best_cost = best_hard_regno = -1;
  CLEAR_HARD_REG_SET (prohibited);
  IOR_COMPL_HARD_REG_SET (prohibited, possible_regs);
  IOR_HARD_REG_SET (prohibited, no_alloc_regs);
  for (i = 0; i < (int) class_hard_regs_num [class]; i++)
    free_allocno_list (class_hard_regs [class] [i]);
  set_up_hard_regno_to_allocno_map (a, &prohibited);
  for (i = 0; i < (int) class_hard_regs_num [class]; i++)
    {
      hard_regno = class_hard_regs [class] [i];
      if (! hard_reg_not_in_set_p (hard_regno, mode, prohibited))
	continue;
      start_transaction ();
      cost = global_allocation_cost;
      if (assign_elimination_reg_with_spill (a, class, possible_regs,
					     hard_regno, &pos))
	{
	  cost = global_allocation_cost - cost;
	  if (best_hard_regno < 0 || best_cost > cost)
	    {
	      best_hard_regno = hard_regno;
	      best_cost = cost;
	      best_pos = pos;
	    }
	  else if (best_cost == cost && best_pos < pos)
	    {
	      best_hard_regno = hard_regno;
	      best_pos = pos;
	    }
	}
      undo_transaction ();
    }
  if (best_hard_regno < 0)
    return -1;
  if (! assign_elimination_reg_with_spill (a, class, possible_regs,
					   best_hard_regno, &pos))
    gcc_unreachable ();
  return best_hard_regno;
}



static void
print_can_disposition (FILE *f, bool global_p)
{
  int i, num;
  can_t can;

  for (num = i = 0; i < cans_num; i++)
    {
      can = cans [i];
      if (CAN_GLOBAL_P (can) ^ global_p)
	continue;
      fprintf (f, "  %4d:%2d", CAN_NUM (can), CAN_HARD_REGNO (can));
      num++;
      if (num % 8 == 0)
	fprintf (f, "\n");
    }
  if (num % 8 != 0)
    fprintf (f, "\n");
}



static varray_type changed_allocnos, new_allocnos_to_do;
static int curr_coalesce_tick, curr_allocno_hard_regno_tick;
static int *coalesce_allocno_check;
static int allocno_hard_regno_check [FIRST_PSEUDO_REGISTER + 2];

static void
try_change_allocno (allocno_t old, bool use_equiv_const_p,
		    int new_reg_hard_regno)
{
  HARD_REG_SET regs;
  int before;
  int old_reg_hard_regno, new_allocno_hard_regno;
  enum reg_class cl;

  old_reg_hard_regno = ALLOCNO_HARD_REGNO (old);
  if (old_reg_hard_regno >= 0)
    old_reg_hard_regno = get_allocno_reg_hard_regno (old, old_reg_hard_regno);
  gcc_assert (! use_equiv_const_p || new_reg_hard_regno < 0);
  gcc_assert (new_reg_hard_regno >= -1);
  new_allocno_hard_regno
    = (new_reg_hard_regno < 0
       ? -1 : get_allocno_hard_regno (old, new_reg_hard_regno));
  if (new_reg_hard_regno == old_reg_hard_regno
      || (ALLOCNO_TYPE (old) == INSN_ALLOCNO
	  && ! check_hard_regno_memory_on_contraint (old, use_equiv_const_p,
						     new_allocno_hard_regno)))
    return;
  before = global_allocation_cost;
  start_transaction ();
  unassign_allocno (old); /* ??? implement breaking ties.  */
  CLEAR_HARD_REG_SET (regs);
  cl = NO_REGS;
  if (new_allocno_hard_regno >= 0)
    {
      int start;

      start = get_maximal_part_start_hard_regno (new_allocno_hard_regno, old);
      gcc_assert (start >= 0);
      IOR_HARD_REG_SET
	(regs, reg_mode_hard_regset [start] [get_allocation_mode (old)]);
      AND_COMPL_HARD_REG_SET (regs, no_alloc_regs);
      cl = smallest_superset_class (regs);
    }
  if ((use_equiv_const_p && assign_allocno (old, LIM_REG_CLASSES, regs, -1))
      || (new_reg_hard_regno < 0 && assign_allocno (old, NO_REGS, regs, -1))
      || (new_reg_hard_regno >= 0 && cl != NO_REGS
	  && assign_allocno (old, cl, regs, new_allocno_hard_regno)))
    {
      VARRAY_PUSH_GENERIC_PTR (changed_allocnos, old);
      end_transaction ();
      if (ALLOCNO_TYPE (old) == INSN_ALLOCNO
	  && INSN_ALLOCNO_TYPE (old) >= OPERAND_BASE)
	set_up_possible_allocno_alternatives
	  (insn_infos [INSN_UID (INSN_ALLOCNO_INSN (old))], old, true);
      return;
    }
  undo_transaction ();
}

static int
allocno_copy_freq (allocno_t a)
{
  int freq;
  copy_t cp;

  freq = 0;
  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
    if (COPY_SRC (cp) != NULL && ! unnecessary_copy_p (cp))
      freq += COPY_FREQ (cp);
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    if (COPY_DST (cp) != NULL && ! unnecessary_copy_p (cp))
      freq += COPY_FREQ (cp);
  return freq;
}

static int *allocno_copy_freq_vec;

static int
allocno_freq_compare (const void *v1p, const void *v2p)
{
  allocno_t a1 = *(allocno_t *) v1p;
  allocno_t a2 = *(allocno_t *) v2p;
  int a1_num = ALLOCNO_NUM (a1), a2_num = ALLOCNO_NUM (a2);
  int diff = allocno_copy_freq_vec [a1_num] - allocno_copy_freq_vec [a2_num];

  if (diff != 0)
    return diff;
  return a1_num - a2_num;
}

static int best_coalesce_cost, best_coalesce_reg_hard_regno;
static bool best_coalesce_use_equiv_const_p;

static void
coalesce_allocnos_with_hard_regno (allocno_t a, bool use_equiv_const_p,
				   int reg_hard_regno, bool restore_p)
{
  copy_t cp;

  gcc_assert (! use_equiv_const_p || reg_hard_regno < 0);
  gcc_assert (reg_hard_regno >= -1);
  if (use_equiv_const_p)
    {
      if (allocno_hard_regno_check [0] == curr_allocno_hard_regno_tick)
	return;
      allocno_hard_regno_check [0] = curr_allocno_hard_regno_tick;
    }
  else
    {
      if (allocno_hard_regno_check [reg_hard_regno + 2]
	  == curr_allocno_hard_regno_tick)
	return;
      allocno_hard_regno_check [reg_hard_regno + 2]
	= curr_allocno_hard_regno_tick;
    }
  if (restore_p)
    start_transaction ();
  try_change_allocno (a, use_equiv_const_p, reg_hard_regno);
  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
    try_change_allocno (COPY_SRC (cp), use_equiv_const_p, reg_hard_regno);
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    try_change_allocno (COPY_DST (cp), use_equiv_const_p, reg_hard_regno);
  if (best_coalesce_cost < 0 || best_coalesce_cost > global_allocation_cost)
    {
      best_coalesce_cost = global_allocation_cost;
      best_coalesce_reg_hard_regno = reg_hard_regno;
      best_coalesce_use_equiv_const_p = use_equiv_const_p;
    }
  if (restore_p)
    undo_transaction ();
  return;
}

static void
coalesce_allocno_copies (allocno_t a)
{
  copy_t cp;
  allocno_t changed_a, conflict_a, src, dst;
  int i, j, before, reg_hard_regno;
  allocno_t *vec;

  best_coalesce_cost = -1;
  best_coalesce_use_equiv_const_p = false;
  before = global_allocation_cost;
  curr_allocno_hard_regno_tick++;
  reg_hard_regno = ALLOCNO_HARD_REGNO (a);
  if (reg_hard_regno >= 0)
    {
      reg_hard_regno = get_allocno_reg_hard_regno (a, reg_hard_regno);
      if (reg_hard_regno < 0)
	return;
    }
  coalesce_allocnos_with_hard_regno (a, ALLOCNO_USE_EQUIV_CONST_P (a),
				     reg_hard_regno, true);
  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
    {
      src = COPY_SRC (cp);
      reg_hard_regno = ALLOCNO_HARD_REGNO (src);
      if (reg_hard_regno >= 0)
	{
	  reg_hard_regno = get_allocno_reg_hard_regno (src, reg_hard_regno);
	  if (reg_hard_regno < 0)
	    continue;
	}
      coalesce_allocnos_with_hard_regno
	(a, ALLOCNO_USE_EQUIV_CONST_P (src), reg_hard_regno, true);
    }
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    {
      dst = COPY_DST (cp);
      reg_hard_regno = ALLOCNO_HARD_REGNO (dst);
      if (reg_hard_regno >= 0)
	{
	  reg_hard_regno = get_allocno_reg_hard_regno (dst, reg_hard_regno);
	  if (reg_hard_regno < 0)
	    continue;
	}
      coalesce_allocnos_with_hard_regno
	(a, ALLOCNO_USE_EQUIV_CONST_P (dst), reg_hard_regno, true);
    }
  gcc_assert (before == global_allocation_cost);
  if (best_coalesce_cost < 0 || best_coalesce_cost >= global_allocation_cost)
    return;
  curr_allocno_hard_regno_tick++;
  VARRAY_POP_ALL (changed_allocnos);
  coalesce_allocnos_with_hard_regno (a, best_coalesce_use_equiv_const_p,
				     best_coalesce_reg_hard_regno, false);
  if (yara_dump_file != NULL)
    {
      if (best_coalesce_use_equiv_const_p)
	fprintf (yara_dump_file, "Coalescing for %d%s to equiv const:",
		 ALLOCNO_NUM (a), allocno_type_name (a));
      else
	fprintf (yara_dump_file, "Coalescing for %d%s to %d:",
		 ALLOCNO_NUM (a), allocno_type_name (a),
		 best_coalesce_reg_hard_regno);
    }
  for (i = VARRAY_ACTIVE_SIZE (changed_allocnos) - 1; i >= 0; i--)
    {
      changed_a = VARRAY_GENERIC_PTR (changed_allocnos, i);
      if (yara_dump_file != NULL)
	fprintf (yara_dump_file, "%4d%s ",
		 ALLOCNO_NUM (changed_a), allocno_type_name (changed_a));
      if (coalesce_allocno_check [ALLOCNO_NUM (changed_a)]
	  != curr_coalesce_tick)
	{
	  coalesce_allocno_check [ALLOCNO_NUM (changed_a)]
	    = curr_coalesce_tick;
	  VARRAY_PUSH_GENERIC_PTR (new_allocnos_to_do, changed_a);
	  vec = ALLOCNO_CONFLICT_VEC (a);
	  for (j = 0; (conflict_a = vec [j]) != NULL; j++)
	    {
	      if (ALLOCNO_REGNO (conflict_a) < 0
		  || HARD_REGISTER_NUM_P (ALLOCNO_REGNO (conflict_a)))
		continue;
	      if (coalesce_allocno_check [ALLOCNO_NUM (conflict_a)]
		  != curr_coalesce_tick)
		{
		  coalesce_allocno_check [ALLOCNO_NUM (conflict_a)]
		    = curr_coalesce_tick;
		  VARRAY_PUSH_GENERIC_PTR (new_allocnos_to_do, conflict_a);
		}
	    }
	}
    }
  if (yara_dump_file != NULL)
    fprintf (yara_dump_file, "\n");
}

static void
coalesce_allocnos (void)
{
  int i;
  allocno_t a;
  varray_type allocnos_to_do;
  varray_type temp_varray;

  if (allocnos_num == 0)
    return;
  set_up_possible_alternatives (true);
  VARRAY_GENERIC_PTR_NOGC_INIT (allocnos_to_do, allocnos_num,
				"allocnos to do");
  VARRAY_GENERIC_PTR_NOGC_INIT (new_allocnos_to_do, allocnos_num,
				"new allocnos to do");
  VARRAY_GENERIC_PTR_NOGC_INIT (changed_allocnos, allocnos_num,
				"allocnos changed during allocno coalescing");
  VARRAY_POP_ALL (new_allocnos_to_do);
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (a == NULL)
	continue;
      if (ALLOCNO_REGNO (a) >= 0 && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a)))
	VARRAY_PUSH_GENERIC_PTR (new_allocnos_to_do, a);
    }
  curr_coalesce_tick = curr_allocno_hard_regno_tick = 0;
  coalesce_allocno_check = yara_allocate (sizeof (int) * allocnos_num);
  allocno_copy_freq_vec = yara_allocate (sizeof (int) * allocnos_num);
  memset (coalesce_allocno_check, 0, sizeof (int) * allocnos_num);
  memset (allocno_hard_regno_check, 0, sizeof (allocno_hard_regno_check));
  do
    {
      if (yara_dump_file != NULL)
	fprintf (yara_dump_file, "Starting iteration\n");
      curr_coalesce_tick++;
      temp_varray = allocnos_to_do;
      allocnos_to_do = new_allocnos_to_do;
      new_allocnos_to_do = temp_varray;
      for (i = VARRAY_ACTIVE_SIZE (allocnos_to_do) - 1; i >= 0; i--)
	{
	  a = VARRAY_GENERIC_PTR (allocnos_to_do, i);
	  allocno_copy_freq_vec [ALLOCNO_NUM (a)] = allocno_copy_freq (a);
	}
      qsort (&VARRAY_GENERIC_PTR (allocnos_to_do, 0),
	     VARRAY_ACTIVE_SIZE (allocnos_to_do),
	     sizeof (allocno_t), allocno_freq_compare);
      VARRAY_POP_ALL (new_allocnos_to_do);
      for (i = VARRAY_ACTIVE_SIZE (allocnos_to_do) - 1; i >= 0; i--)
	{
	  a = VARRAY_GENERIC_PTR (allocnos_to_do, i);
	  if (coalesce_allocno_check [ALLOCNO_NUM (a)] != curr_coalesce_tick)
	    coalesce_allocno_copies (a);
	}
    }
  while (VARRAY_ACTIVE_SIZE (new_allocnos_to_do) != 0);
  yara_free (allocno_copy_freq_vec);
  yara_free (coalesce_allocno_check);
  VARRAY_FREE (changed_allocnos);
  VARRAY_FREE (new_allocnos_to_do);
  VARRAY_FREE (allocnos_to_do);
}



/* Return cost of copy CP of two pseudo-registers.  */
static int
pseudo_reg_copy_cost (copy_t cp)
{
  enum reg_class cl;
  allocno_t dst, src;
  int dst_regno, src_regno;
  int dst_hard_regno, src_hard_regno;
  int dst_offset, src_offset;
  enum machine_mode dst_mode, src_mode;
  struct memory_slot *dst_memory_slot, *src_memory_slot;
  int cost;

  dst = COPY_DST (cp);
  src = COPY_SRC (cp);
  if (dst == src)
    return 0;
  gcc_assert (dst != NULL && src != NULL);
  dst_hard_regno = ALLOCNO_HARD_REGNO (dst);
  dst_memory_slot = ALLOCNO_MEMORY_SLOT (dst);
  dst_offset = ALLOCNO_MEMORY_SLOT_OFFSET (dst);
  src_hard_regno = ALLOCNO_HARD_REGNO (src);
  src_memory_slot = ALLOCNO_MEMORY_SLOT (src);
  src_offset = ALLOCNO_MEMORY_SLOT_OFFSET (src);
  /* ??? Subreg of multireg */
  dst_mode = ALLOCNO_MODE (dst);
  src_mode = ALLOCNO_MODE (src);
  cl = NO_REGS;
  cost = 0;
  if (dst_hard_regno < 0 && dst_memory_slot == NULL
      && ! ALLOCNO_USE_EQUIV_CONST_P (dst))

    {
      /* Allocation for DST has not been done yet.  */
      if (ALLOCNO_TYPE (dst) != INSN_ALLOCNO
	  && ALLOCNO_CAN (dst) != NULL)
	cl = CAN_COVER_CLASS (ALLOCNO_CAN (dst));
      if (cl != NO_REGS)
	/* ??? Accurate cost for loading constant.  */
	cost = (src_hard_regno < 0 && ! ALLOCNO_USE_EQUIV_CONST_P (src)
		? memory_move_cost [dst_mode] [cl] [1]
		: hard_reg_in_set_p (src_hard_regno, dst_mode,
				     reg_class_contents [cl])
		? 0
		: register_move_cost [dst_mode]
		                     [REGNO_REG_CLASS (src_hard_regno)] [cl]);
    }
  else if (src_hard_regno < 0 && src_memory_slot == NULL
	   && ! ALLOCNO_USE_EQUIV_CONST_P (src))
    {
      /* Allocation for SRC has not been done yet.  */
      if (ALLOCNO_TYPE (src) != INSN_ALLOCNO && ALLOCNO_CAN (src) != NULL)
	cl = CAN_COVER_CLASS (ALLOCNO_CAN (src));
      if (cl != NO_REGS && ! ALLOCNO_USE_EQUIV_CONST_P (dst))
	cost = (dst_hard_regno < 0
		? memory_move_cost [dst_mode] [cl] [0]
		: hard_reg_in_set_p (dst_hard_regno, dst_mode,
				     reg_class_contents [cl])
		? 0
		: register_move_cost [dst_mode] [cl]
		                     [REGNO_REG_CLASS (dst_hard_regno)]);
    }
  else
    {
      dst_regno = ALLOCNO_REGNO (dst);
      src_regno = ALLOCNO_REGNO (src);
      gcc_assert (dst_regno >= 0 && ! HARD_REGISTER_NUM_P (dst_regno)
		  && src_regno >= 0 && ! HARD_REGISTER_NUM_P (src_regno));
      if (dst_hard_regno < 0)
	{
	  if (ALLOCNO_USE_EQUIV_CONST_P (dst)) /* We don't need an insn.  */
	    ;
	  else if (src_hard_regno >= 0)
	    cost = memory_move_cost [src_mode]
	                            [REGNO_REG_CLASS (src_hard_regno)] [0];
	  else if (src_regno != dst_regno
		   && (src_memory_slot->mem != dst_memory_slot->mem
		       || src_memory_slot->start != dst_memory_slot->start
		       || src_offset != dst_offset
		       || src_memory_slot->size != dst_memory_slot->size))
	    {
	      /* ??? Better evaluation: scracth register and pattern.  */
#ifdef HAVE_SECONDARY_RELOADS
	      int interm_regno = COPY_INTERM_REGNO (cp);

	      gcc_assert (interm_regno >= 0);
	      cost = (memory_move_cost [dst_mode]
                                       [REGNO_REG_CLASS (interm_regno)] [0]
		      + memory_move_cost [dst_mode]
                                         [REGNO_REG_CLASS (interm_regno)] [1]);
#else
	      gcc_unreachable ();
#endif
	    }
	}
      else if (src_hard_regno < 0)
	{
	  if (! ALLOCNO_USE_EQUIV_CONST_P (src))
	    cost = memory_move_cost [dst_mode]
                                    [REGNO_REG_CLASS (dst_hard_regno)] [1];
	  else
	    /* ??? constant lodaing */
	    cost = register_move_cost [dst_mode]
                                      [REGNO_REG_CLASS (dst_hard_regno)]
	                              [REGNO_REG_CLASS (dst_hard_regno)];
	}
      else
	{
	  gcc_assert (src_hard_regno >= 0);
	  /* ??? Subregs to get real hard regs.  */
	  if (src_hard_regno != dst_hard_regno)
	    cost = register_move_cost [dst_mode]
                                      [REGNO_REG_CLASS (src_hard_regno)]
	                              [REGNO_REG_CLASS (dst_hard_regno)];
	}
    }
  return cost * COPY_FREQ (cp) * COST_FACTOR;
}



void
yara_color_init_once (void)
{
  int i;

  for (i = 0; i < N_REG_CLASSES; i++)
    init_class_cost [i] = INT_MAX;
  find_reg_class_closure ();
}

int (*pseudo_reg_copy_cost_func) (copy_t);

/* Start point of register allocation.  */
void
yara_color (void)
{
  set_up_possible_alternatives (false);
  saved_conflict_cans = yara_allocate (sizeof (can_t) * allocnos_num);
  split_allocno_bitmap = yara_allocate_bitmap ();
  VARRAY_GENERIC_PTR_NOGC_INIT (new_can_allocno_varray, allocnos_num,
				"allocnos for new can");
  biased_can_bitmap = yara_allocate_bitmap ();
  conflict_can_bitmap = yara_allocate_bitmap ();
  set_up_cover_classes_and_reg_costs ();
  add_move_costs ();
  if (yara_dump_file != NULL)
    print_cans (yara_dump_file);
  if (yara_dump_file != NULL)
    fprintf (yara_dump_file, "\n**** Global Allocation:\n\n");
  if ((YARA_PARAMS & YARA_PRIORITY_COLORING))
    priority_color ();
  else
    global_can_alloc ();
  if (yara_dump_file != NULL)
    print_can_disposition (yara_dump_file, true);
  reload_in_progress = 1;
  pseudo_reg_copy_cost_func = pseudo_reg_copy_cost;
  assign_global_can_allocnos ();
  local_can_alloc ();
  eliminate_virtual_registers (provide_allocno_elimination_class_hard_reg);
  if ((YARA_PARAMS & YARA_NO_ALLOCNO_COALESCING) == 0)
    coalesce_allocnos ();
  compact_stack ();
  reload_in_progress = 0;
  yara_free (saved_conflict_cans);
  yara_free_bitmap (conflict_can_bitmap);
  yara_free_bitmap (biased_can_bitmap);
  VARRAY_FREE (new_can_allocno_varray);
  yara_free_bitmap (split_allocno_bitmap);
}
