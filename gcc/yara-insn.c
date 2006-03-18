/* Allocation for insn allocnos for Yet Another Register Allocator.
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
#include "hard-reg-set.h"
#include "basic-block.h"
#include "insn-codes.h"
#include "insn-config.h"
#include "expr.h"
#include "optabs.h"
#include "recog.h"
#include "cfgloop.h"
#include "errors.h"
#include "params.h"
#include "yara-int.h"



static bool check_alternative_possibility (allocno_t, const char *, bool);
static allocno_t get_first_operand_allocno (allocno_t);
static void set_up_possible_operand_alternatives (rtx, bool);

static bool try_hard_regno_from_connected_allocno (allocno_t, enum reg_class,
						   HARD_REG_SET, bool, bool *);
static bool assign_hard_regno_to_pseudo_reg_insn_allocno (allocno_t,
							  enum reg_class,
							  HARD_REG_SET);
static bool assign_constraint (allocno_t, const char *);
static void try_insn_allocno_allocation (int);
static void change_other_allocno_container_loc (allocno_t, allocno_t,
						rtx *, rtx *);
static void find_best_alt_allocation (int);
static int get_duplication_number (const char *);
static void put_duplication_before_original (void);
static int store_best_allocation (int, bool *);
static bool find_best_allocation (rtx);



static bool
check_alternative_possibility (allocno_t a, const char *p, bool strict_p)
{
  rtx op, no_subreg_op, equiv_const;
  int c = *p;

  op = *INSN_ALLOCNO_LOC (a);
  SKIP_TO_REG (no_subreg_op, op);
  equiv_const = (ALLOCNO_REGNO (a) >= 0
		 && (! strict_p || ALLOCNO_USE_EQUIV_CONST_P (a))
		 ? reg_equiv_constant [ALLOCNO_REGNO (a)] : NULL_RTX);
  switch (c)
    {
    case '\0':
    case 'X':
      return true;

    case 'i':
      if (strict_p && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a)
	  && ! ALLOCNO_USE_EQUIV_CONST_P (a))
	return false;
      return (CONSTANT_P (op)
	      || (equiv_const != NULL_RTX && CONSTANT_P (equiv_const)));

    case 'n':
      if (strict_p && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a)
	  && ! ALLOCNO_USE_EQUIV_CONST_P (a))
	return false;
      return (GET_CODE (op) == CONST_INT
	      || (GET_CODE (op) == CONST_DOUBLE && GET_MODE (op) == VOIDmode)
	      || (equiv_const != NULL_RTX
		  && (GET_CODE (equiv_const) == CONST_INT
		      || (GET_CODE (equiv_const) == CONST_DOUBLE
			  && GET_MODE (equiv_const) == VOIDmode))));

    case 's':
      if (strict_p && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a)
	  && ! ALLOCNO_USE_EQUIV_CONST_P (a))
	return false;
      return ((CONSTANT_P (op) && GET_CODE (op) != CONST_INT
	       && (GET_CODE (op) != CONST_DOUBLE || GET_MODE (op) != VOIDmode))
	      || (equiv_const != NULL_RTX
		  && CONSTANT_P (equiv_const)
		  && GET_CODE (equiv_const) != CONST_INT
		  && (GET_CODE (equiv_const) != CONST_DOUBLE
		      || GET_MODE (equiv_const) != VOIDmode)));
	      
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
      if (strict_p && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a)
	  && ! ALLOCNO_USE_EQUIV_CONST_P (a))
	return false;
      return ((GET_CODE (op) == CONST_INT
	       && CONST_OK_FOR_CONSTRAINT_P (INTVAL (op), c, p))
	      || (equiv_const != NULL_RTX
		  && GET_CODE (equiv_const) == CONST_INT
		  && CONST_OK_FOR_CONSTRAINT_P (INTVAL (equiv_const), c, p)));

    case 'E':
    case 'F':
      if (strict_p && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a)
	  && ! ALLOCNO_USE_EQUIV_CONST_P (a))
	return false;
      return (GET_CODE (op) == CONST_DOUBLE
	      || (GET_CODE (op) == CONST_VECTOR
		  && GET_MODE_CLASS (GET_MODE (op)) == MODE_VECTOR_FLOAT)
	      || (equiv_const != NULL_RTX
		  && (GET_CODE (equiv_const) == CONST_DOUBLE
		      || (GET_CODE (equiv_const) == CONST_VECTOR
			  && (GET_MODE_CLASS (GET_MODE (equiv_const))
			      == MODE_VECTOR_FLOAT)))));
      
    case 'G':
    case 'H':
      if (strict_p && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a)
	  && ! ALLOCNO_USE_EQUIV_CONST_P (a))
	return false;
      return ((GET_CODE (op) == CONST_DOUBLE
	       && CONST_DOUBLE_OK_FOR_CONSTRAINT_P (op, c, p))
	      || (equiv_const != NULL_RTX
		  && GET_CODE (equiv_const) == CONST_DOUBLE
		  && CONST_DOUBLE_OK_FOR_CONSTRAINT_P (equiv_const, c, p)));

    case 'm':
    case 'o':
      if (strict_p)
	/* CONST_POOL_OK_P and use_equiv_const??? */
	return (ALLOCNO_MEMORY_SLOT (a) != NULL
		|| (INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a)
		    &&
		    ((MEM_P (no_subreg_op) && c == 'm')
		     || (c == 'o'
			 && offsettable_nonstrict_memref_p (no_subreg_op)))));
      else
	return ((MEM_P (no_subreg_op) && c == 'm')
		|| (c == 'o' && offsettable_nonstrict_memref_p (no_subreg_op))
		/* We could accept a constant that can be turned into
		   memory.  */
		|| CONST_POOL_OK_P (op)
		|| (equiv_const != NULL_RTX && CONST_POOL_OK_P (equiv_const))
		/* Accept a register which might be placed in memory.  */
		|| REG_P (no_subreg_op));
      
    case 'V':
      if (strict_p && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a))
	return false;
      return (MEM_P (op) && ! offsettable_nonstrict_memref_p (op));
      
    case '<':
      if (strict_p && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a))
	return false;
      return (MEM_P (op) && (GET_CODE (XEXP (op, 0)) == PRE_DEC
			     || GET_CODE (XEXP (op, 0)) == POST_DEC));

    case '>':
      if (strict_p && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a))
	return false;
      return (MEM_P (op) && (GET_CODE (XEXP (op, 0)) == PRE_INC
			     || GET_CODE (XEXP (op, 0)) == POST_INC));

    case 'p':
      /* ??? strict_p */
      /* p is used for address_operands.  We consider it in non-strict
	 sense.  */
      GO_IF_LEGITIMATE_ADDRESS (VOIDmode, op, win_p);
      break;

    win_p:
      return true;

    case 'g':
      return (! strict_p || check_alternative_possibility (a, "r", true)
	      || check_alternative_possibility (a, "m", true)
	      || check_alternative_possibility (a, "i", true));
      
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
	enum machine_mode mode;
	int hard_regno, regno;

	cl = (c == 'r' ? GENERAL_REGS : REG_CLASS_FROM_CONSTRAINT (c, p));
	if (cl != NO_REGS)
	  {
	    if (! strict_p)
	      return true;
	    hard_regno = ALLOCNO_HARD_REGNO (a);
	    mode = ALLOCNO_MODE (a);
	    regno = ALLOCNO_REGNO (a);
	    if (hard_regno < 0 && regno >= 0 && HARD_REGISTER_NUM_P (regno)
		&& INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a))
	      hard_regno = get_allocno_hard_regno (a, regno);
	    if (hard_regno >= 0
		&& hard_reg_in_set_p (hard_regno, mode,
				      reg_class_contents [cl]))
	      return true;
	  }
#ifdef EXTRA_CONSTRAINT_STR
	else if (EXTRA_CONSTRAINT_STR (op, c, p))
	  return true;
	/* EXTRA_MEMORY_CONSTRAINT && EXTRA_ADDRESS_CONSTRAINT is
	   ignored here because it means that they should be loaded
	   into a register.  */
#endif
	break;
      }

    default:
      gcc_unreachable ();
    }
  return false;
}

void
set_up_possible_allocno_alternatives (struct insn_op_info *info, allocno_t a,
				      bool strict_p)
{
  int op_num, n_alt, alts_num;
  const char *constraints;
  char c;
  alt_set_t set;

  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
  gcc_assert (op_num >= 0);
  alts_num = info->n_alts;
  if (info == NULL || alts_num == 0)
    /* We dont care about value of possible_alts in this case.  */
    return;
  CLEAR_ALT_SET (set);
  for (n_alt = 0; n_alt < alts_num; n_alt++)
    {
      constraints = info->op_constraints [op_num * alts_num + n_alt];
      if (constraints == NULL || *constraints == '\0')
	SET_ALT (set, n_alt);
      else
	{
	  for (;
	       (c = *constraints);
	       constraints += CONSTRAINT_LEN (c, constraints))
	    if (c == '#')
	      break;
	    else if (c == ' ' || c == '\t' || c == '=' || c == '+' || c == '*'
		     || c == '&' || c == '%' || c == '?' || c == '!')
	      ;
	    else if ('0' <= c && c <= '9')
	      {
		allocno_t a1;
		const char *str;
		int n = c - '0';

		gcc_assert (op_num > n);
		a1 = (insn_op_allocnos [INSN_UID (INSN_ALLOCNO_INSN (a))] [n]);
		if (! strict_p)
		  SET_ALT (set, n_alt);
		else if (INSN_ALLOCNO_TIED_ALLOCNO (a) == a1
			 && TEST_ALT (INSN_ALLOCNO_POSSIBLE_ALTS (a1), n_alt))
		  {
		    /* ???!!! We have after allocation duplication
		       first.  */
		    str = info->op_constraints [(n) * alts_num + n_alt];
		    for (; (c = *str); str += CONSTRAINT_LEN (c, str))
		      if (c == '#')
			break;
		      else if (c == ' ' || c == '\t' || c == '=' || c == '+'
			       || c == '*' || c == '&' || c == '%'
			       || c == '?' || c == '!')
			;
		      else if (check_alternative_possibility (a, str, true))
			{
			  SET_ALT (set, n_alt);
			  break;
			}
		  }
	      }
	    else if (check_alternative_possibility (a, constraints, strict_p))
	      SET_ALT (set, n_alt);
	}
    }
  INSN_ALLOCNO_POSSIBLE_ALTS (a) = set;
}

static allocno_t
get_first_operand_allocno (allocno_t start)
{
  for (;start != NULL; start = INSN_ALLOCNO_NEXT (start))
    if (INSN_ALLOCNO_TYPE (start) >= OPERAND_BASE)
      break;
  return start;
}

static void
set_up_possible_operand_alternatives (rtx insn, bool strict_p)
{
  allocno_t op;
  struct insn_op_info *info;

  info = insn_infos [INSN_UID (insn)];
  for (op = get_first_operand_allocno (insn_allocnos [INSN_UID (insn)]);
       op != NULL;
       op = get_first_operand_allocno (INSN_ALLOCNO_NEXT (op)))
    set_up_possible_allocno_alternatives (info, op, strict_p);
}

void
set_up_possible_alternatives (bool strict_p)
{
  basic_block bb;
  rtx bound, insn;

  FOR_EACH_BB (bb)
    {
      bound = NEXT_INSN (BB_END (bb));
      for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
	if (INSN_P (insn))
	  set_up_possible_operand_alternatives (insn, strict_p);
    }
}



static enum reg_class
limit_insn_allocno_class (allocno_t a, enum reg_class class)
{
#ifdef LIMIT_RELOAD_CLASS
  rtx x = *INSN_ALLOCNO_LOC (a);

  /* Make sure we use a class that can handle the actual pseudo inside
     any subreg.  For example, on the 386, QImode regs can appear
     within SImode subregs.  Although GENERAL_REGS can handle SImode,
     QImode needs a smaller class.  */
  class = LIMIT_RELOAD_CLASS (GET_MODE (x), class);
#endif

  return class;
}

static bool (*call_cross_hint_func) (allocno_t);

static void (*insn_allocno_sort_func) (allocno_t *, int, rtx, int);

static int (*provide_allocno_class_hard_reg_func) (allocno_t,
						   enum reg_class);

static bool
try_hard_regno_from_connected_allocno (allocno_t a, enum reg_class cl,
				       HARD_REG_SET possible_regs,
				       bool dst_p, bool *call_p)
{
  int hard_regno;
  allocno_t connected_a;
  copy_t cp;

  for (cp = (dst_p ? ALLOCNO_DST_COPIES (a) : ALLOCNO_SRC_COPIES (a));
       cp != NULL;
       cp = (dst_p ? COPY_NEXT_DST_COPY (cp) : COPY_NEXT_SRC_COPY (cp)))
    {
      connected_a = (dst_p ? COPY_SRC (cp) : COPY_DST (cp));
      if (connected_a == NULL
	  || (hard_regno = ALLOCNO_HARD_REGNO (connected_a)) < 0)
	{
	  if (connected_a != NULL && call_cross_hint_func != NULL
	      && (*call_cross_hint_func) (connected_a))
	    *call_p = true;
	  continue;
	}
      hard_regno = get_allocno_hard_regno (a, hard_regno);
      if (assign_allocno (a, cl, possible_regs, hard_regno))
	return true;
    }
  return false;
}

/* ??? SUBREGS */

static bool
assign_hard_regno_to_pseudo_reg_insn_allocno (allocno_t a, enum reg_class cl,
					      HARD_REG_SET possible_regs)
{
  bool call_p;
  int hard_regno;
  allocno_t tied_a;
  
  call_p = false;
  if (try_hard_regno_from_connected_allocno (a, cl, possible_regs,
					     true, &call_p))
    return true;
  if (try_hard_regno_from_connected_allocno (a, cl, possible_regs,
					     false, &call_p))
    return true;
  tied_a = INSN_ALLOCNO_TIED_ALLOCNO (a);
  if (tied_a != NULL)
    {
      if (try_hard_regno_from_connected_allocno
	  (tied_a, cl, possible_regs, true, &call_p))
	return true;
      if (try_hard_regno_from_connected_allocno
	  (tied_a, cl, possible_regs, false, &call_p))
	return true;
    }
  if (call_p)
    {
      HARD_REG_SET temp_regs;
      
      COPY_HARD_REG_SET (temp_regs, possible_regs);
      AND_COMPL_HARD_REG_SET (temp_regs, call_used_reg_set);
      if (assign_allocno (a, cl, temp_regs, -1))
	return true;
    }
  if (assign_allocno (a, cl, possible_regs, -1))
    return true;
  if (provide_allocno_class_hard_reg_func == NULL)
    return false;
  hard_regno = (*provide_allocno_class_hard_reg_func) (a, cl);
  return hard_regno >= 0;
}

static bool
assign_constraint (allocno_t a, const char *p)
{
  rtx op, no_subreg_op, equiv_const;
  bool success_p;
  int before;
  int c = *p;

  op = *INSN_ALLOCNO_LOC (a);
  SKIP_TO_REG (no_subreg_op, op);
  equiv_const = (ALLOCNO_REGNO (a) >= 0
		 ? reg_equiv_constant [ALLOCNO_REGNO (a)] : NULL_RTX);
  success_p = false;
  before = global_allocation_cost;
  switch (c)
    {
    case '\0':
    case 'X':
      if (ALLOCNO_REGNO (a) >= 0
	  && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a)))
	/* Actually such operand will be not used finally but we need
	   something instead of pseudo-register. ??? change it on
	   constant.  */
	success_p = assign_allocno (a, NO_REGS,
				    reg_class_contents [NO_REGS], -1);
      else
	success_p = assign_allocno (a, LIM_REG_CLASSES,
				    reg_class_contents [NO_REGS], -1);
      break;

    case 'i':
      if (CONSTANT_P (op)
	  || (equiv_const != NULL_RTX && CONSTANT_P (equiv_const)))
	{
	  gcc_assert (equiv_const != NULL_RTX
		      || (ALLOCNO_SRC_COPIES (a) == NULL
			  && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL));
	  success_p = assign_allocno (a, LIM_REG_CLASSES,
				      reg_class_contents [NO_REGS], -1);
	}
      break;

    case 'n':
      if (GET_CODE (op) == CONST_INT
	  || (GET_CODE (op) == CONST_DOUBLE && GET_MODE (op) == VOIDmode)
	  || (equiv_const != NULL_RTX
	      && (GET_CODE (equiv_const) == CONST_INT
		  || (GET_CODE (equiv_const) == CONST_DOUBLE
		      && GET_MODE (equiv_const) == VOIDmode))))
	{
	  gcc_assert (equiv_const != NULL_RTX
		      || (ALLOCNO_SRC_COPIES (a) == NULL
			  && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL));
	  success_p = assign_allocno (a, LIM_REG_CLASSES,
				      reg_class_contents [NO_REGS], -1);
	}
      break;

    case 's':
      if ((CONSTANT_P (op) && GET_CODE (op) != CONST_INT
	   && (GET_CODE (op) != CONST_DOUBLE || GET_MODE (op) != VOIDmode))
	  || (equiv_const != NULL_RTX
	      && CONSTANT_P (equiv_const)
	      && GET_CODE (equiv_const) != CONST_INT
	      && (GET_CODE (equiv_const) != CONST_DOUBLE
		  || GET_MODE (equiv_const) != VOIDmode)))
	{
	  gcc_assert (equiv_const != NULL_RTX
		      || (ALLOCNO_SRC_COPIES (a) == NULL
			  && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL));
	  success_p = assign_allocno (a, LIM_REG_CLASSES,
				      reg_class_contents [NO_REGS], -1);
	}
      break;

    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
      if ((GET_CODE (op) == CONST_INT
	   && CONST_OK_FOR_CONSTRAINT_P (INTVAL (op), c, p))
	  || (equiv_const != NULL_RTX
	      && GET_CODE (equiv_const) == CONST_INT
	      && CONST_OK_FOR_CONSTRAINT_P (INTVAL (equiv_const), c, p)))
	{
	  gcc_assert (equiv_const != NULL_RTX
		      || (ALLOCNO_SRC_COPIES (a) == NULL
			  && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL));
	  success_p = assign_allocno (a, LIM_REG_CLASSES,
				      reg_class_contents [NO_REGS], -1);
	}
      break;

    case 'E':
    case 'F':
      if (GET_CODE (op) == CONST_DOUBLE
	  || (GET_CODE (op) == CONST_VECTOR
	      && GET_MODE_CLASS (GET_MODE (op)) == MODE_VECTOR_FLOAT)
	  || (equiv_const != NULL_RTX
	      && (GET_CODE (equiv_const) == CONST_DOUBLE
		  || (GET_CODE (equiv_const) == CONST_VECTOR
		      && (GET_MODE_CLASS (GET_MODE (equiv_const))
			  == MODE_VECTOR_FLOAT)))))
	{
	  gcc_assert (equiv_const != NULL_RTX
		      || (ALLOCNO_SRC_COPIES (a) == NULL
			  && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL));
	  success_p = assign_allocno (a, LIM_REG_CLASSES,
				      reg_class_contents [NO_REGS], -1);
	}
      break;

    case 'G':
    case 'H':
      if ((GET_CODE (op) == CONST_DOUBLE
	   && CONST_DOUBLE_OK_FOR_CONSTRAINT_P (op, c, p))
	  || (equiv_const != NULL_RTX
	      && GET_CODE (equiv_const) == CONST_DOUBLE
	      && CONST_DOUBLE_OK_FOR_CONSTRAINT_P (equiv_const, c, p)))
	{
	  gcc_assert (equiv_const != NULL_RTX
		      || (ALLOCNO_SRC_COPIES (a) == NULL
			  && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL));
	  success_p = assign_allocno (a, LIM_REG_CLASSES,
				      reg_class_contents [NO_REGS], -1);
	}
      break;

    case 'm':
    case 'o':
      if ((MEM_P (no_subreg_op) && c == 'm')
	  || (offsettable_nonstrict_memref_p (no_subreg_op) && c == 'o'))
	{
	  /* We already have legitimate memory address in non-strict
	     sense.  */
	  gcc_assert (INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL
		      || INSN_ALLOCNO_ORIGINAL_P (a));
	  success_p = assign_allocno (a, LIM_REG_CLASSES,
				      reg_class_contents [NO_REGS], -1);
	}
      /* We could accept a constant that can be turned into mem.  */
      else if (CONST_POOL_OK_P (op)
	       || (0 && equiv_const != NULL_RTX
		   && CONST_POOL_OK_P (equiv_const)))
	{
	  gcc_assert (equiv_const != NULL_RTX
		      || (ALLOCNO_SRC_COPIES (a) == NULL
			  && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL));
	  success_p = (! INSN_ALLOCNO_ELIMINATION_PART_CONST_P (a)
		       && assign_allocno (a, NO_REGS,
					  reg_class_contents [NO_REGS], -1));
	}
      /* Accept a register which might be placed in memory.  */
      else if (REG_P (no_subreg_op))
	success_p = assign_allocno (a, NO_REGS,
				    reg_class_contents [NO_REGS], -1);
      break;
      

    case 'V':
      if (MEM_P (op) && ! offsettable_nonstrict_memref_p (op))
	{
	  gcc_assert (INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL
		      || INSN_ALLOCNO_ORIGINAL_P (a));
	  success_p = assign_allocno (a, LIM_REG_CLASSES,
				      reg_class_contents [NO_REGS], -1);
	}
      break;

    case '<':
      if (MEM_P (op) && (GET_CODE (XEXP (op, 0)) == PRE_DEC
			 || GET_CODE (XEXP (op, 0)) == POST_DEC)
	  && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL)
	success_p = assign_allocno (a, LIM_REG_CLASSES,
				    reg_class_contents [NO_REGS], -1);
      break;

    case '>':
      if (MEM_P (op) && (GET_CODE (XEXP (op, 0)) == PRE_INC
			 || GET_CODE (XEXP (op, 0)) == POST_INC)
	  && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL)
	success_p = assign_allocno (a, LIM_REG_CLASSES,
				    reg_class_contents [NO_REGS], -1);
      break;

    case 'p':
      /* p is used for address_operands.  We consider it in non-strict
	 sense.  */
      GO_IF_LEGITIMATE_ADDRESS (VOIDmode, op, win_p);
      break;

    win_p:
      success_p = assign_allocno (a, LIM_REG_CLASSES,
				  reg_class_contents [NO_REGS], -1);
      break;

    case 'g':
      /* Anything goes unless it is a REG and really has a hard reg
	 but the hard reg is not in the class GENERAL_REGS.  */
      if (CONSTANT_P (op) || MEM_P (no_subreg_op)
	  || (equiv_const != NULL_RTX && CONSTANT_P (equiv_const)))
	{
	  gcc_assert (! CONSTANT_P (op) || equiv_const != NULL_RTX
		      || ALLOCNO_SRC_COPIES (a) == NULL);
	  gcc_assert (INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL
		      || INSN_ALLOCNO_ORIGINAL_P (a));
	  success_p = assign_allocno (a, LIM_REG_CLASSES,
				      reg_class_contents [NO_REGS], -1);
	  break;
	}
      /* fall through */

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
	
	cl = (c == 'r'  || c == 'g'
	      ? GENERAL_REGS : REG_CLASS_FROM_CONSTRAINT (c, p));
	if (cl != NO_REGS)
	  {
	    gcc_assert (! REG_P (no_subreg_op) || ALLOCNO_REGNO (a) >= 0);

	    cl = limit_insn_allocno_class (a, cl);

	    if (cl == NO_REGS)
	      ;
	    else if (REG_P (no_subreg_op) && HARD_REGISTER_P (no_subreg_op)
		     && hard_reg_in_set_p (get_allocno_hard_regno
					   (a, ALLOCNO_REGNO (a)),
					   ALLOCNO_MODE (a),
					   reg_class_contents [cl]))
	      {
		gcc_assert (INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL
			    || INSN_ALLOCNO_ORIGINAL_P (a));
		success_p = assign_allocno (a, LIM_REG_CLASSES,
					    reg_class_contents [NO_REGS], -1);
		if (! success_p
		    && assign_hard_regno_to_pseudo_reg_insn_allocno
		       (a, cl, reg_class_contents [cl]))
		  success_p = true;
	      }
	    else if (! INSN_ALLOCNO_ELIMINATION_PART_CONST_P (a)
		     && assign_hard_regno_to_pseudo_reg_insn_allocno
                        (a, cl, reg_class_contents [cl]))
	      success_p = true;
	    /* Accept a register which might be placed in memory.  */
	    else if (c == 'g' && REG_P (no_subreg_op))
	      success_p = assign_allocno (a, NO_REGS,
					  reg_class_contents [NO_REGS], -1);
	    /* We can fail here because a wrong copy is active.  */
 	    break;
	  }
#ifdef EXTRA_CONSTRAINT_STR
	else if (EXTRA_CONSTRAINT_STR (op, c, p))
	  {
	    success_p = assign_allocno (a, LIM_REG_CLASSES,
					reg_class_contents [NO_REGS], -1);
	    /* EXTRA_MEMORY_CONSTRAINT && EXTRA_ADDRESS_CONSTRAINT is
	       ignored here because it means that they should be loaded
	       into a register.  */
	  }
#endif
	break;
      }

    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      {
	allocno_t another_a;
	
	another_a = (insn_op_allocnos
		     [INSN_UID (INSN_ALLOCNO_INSN (a))] [c - '0']);
	gcc_assert ((INSN_ALLOCNO_OP_MODE (a) == OP_IN
		     && INSN_ALLOCNO_OP_MODE (another_a) == OP_OUT)
		    || (INSN_ALLOCNO_OP_MODE (another_a) == OP_IN
			&& INSN_ALLOCNO_OP_MODE (a) == OP_OUT));
	create_tie (another_a, a);
	/* The cost will taken into account when the original will
	   be allocated.  */
	success_p = true;
	break;
      }

    default:
      gcc_unreachable ();
    }
  if (! success_p)
    return false;
  return true;
}

/* ??? Rename */
static int curr_allocation_alt;
static rtx curr_allocation_insn;
static bool min_alt_cost_undef_p;
static int min_alt_cost;
static const char *best_constraints [MAX_RECOG_OPERANDS];
static const char *best_alt_constraints [MAX_RECOG_OPERANDS];
static bool best_op_exchange_p [MAX_RECOG_OPERANDS];
static bool best_alt_op_exchange_p [MAX_RECOG_OPERANDS];

/* Varray containing references to the current insn allocnos for the
   current allternative.  */
static varray_type insn_allocno_varray;

/* Varray containing references to the current insn allocnos in order
   for the best allocation.  */
static varray_type best_insn_allocno_varray;

static const char *curr_alt_constraints [MAX_RECOG_OPERANDS];
static bool curr_alt_op_exchange_p [MAX_RECOG_OPERANDS];

static void
try_insn_allocno_allocation (int a_num)
{
  allocno_t a;
  const char *constraints;
  int op_num, before;
  bool ok_p;
  char c;
  struct insn_op_info *info;

  info = insn_infos [INSN_UID (curr_allocation_insn)];
  a = VARRAY_GENERIC_PTR (insn_allocno_varray, a_num);
  before = global_allocation_cost;
  start_transaction ();
  if (INSN_ALLOCNO_TYPE (a) == BASE_REG || INSN_ALLOCNO_TYPE (a) == INDEX_REG)
    {
      rtx reg = *INSN_ALLOCNO_LOC (a);
      HARD_REG_SET temp_set;
      enum reg_class cl;
      
      gcc_assert (REG_P (reg)
		  && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL
		  && (INSN_ALLOCNO_OP_MODE (a) == OP_IN
		      || INSN_ALLOCNO_OP_MODE (a) == OP_INOUT)
		  /* We assume no base and index registers in
		     subregisters.  */
		  && GET_CODE (*INSN_ALLOCNO_CONTAINER_LOC (a)) != SUBREG);
      if (INSN_ALLOCNO_TYPE (a) == BASE_REG)
	{
	  COPY_HARD_REG_SET
	    (temp_set, base_regs [GET_MODE (*INSN_ALLOCNO_CONTAINER_LOC (a))]);
	  cl = BASE_REG_CLASS;
	}
      else
	{
	  COPY_HARD_REG_SET (temp_set, index_regs);
	  cl = INDEX_REG_CLASS;
	}
      if (HARD_REGISTER_P (reg)
	  && hard_reg_in_set_p (REGNO (reg), ALLOCNO_MODE (a), temp_set))
	{
	  if (! assign_allocno (a, LIM_REG_CLASSES,
				reg_class_contents [NO_REGS], -1))
	    gcc_unreachable ();
	  find_best_alt_allocation (a_num + 1);
	}
      else if (! assign_hard_regno_to_pseudo_reg_insn_allocno (a, cl,
							       temp_set))
	;
      else
	{
	  find_best_alt_allocation (a_num + 1);
	  gcc_assert (INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL);
	}
    }
  else if (INSN_ALLOCNO_TYPE (a) == NON_OPERAND)
    {
      if (ALLOCNO_REGNO (a) < 0 || HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a)))
	{
	  if (! assign_allocno (a, LIM_REG_CLASSES,
				reg_class_contents [NO_REGS], -1))
	    gcc_unreachable ();
	  find_best_alt_allocation (a_num + 1);
	}
      /* ??? GENERAL_REGS */
      else if (assign_hard_regno_to_pseudo_reg_insn_allocno
	       (a, GENERAL_REGS, reg_class_contents [GENERAL_REGS]))
	{
	  find_best_alt_allocation (a_num + 1);
	  gcc_assert (INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL);
	}
      else
	gcc_unreachable ();
    }
  else
    {
      info = insn_infos [INSN_UID (curr_allocation_insn)];
      op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
      gcc_assert (op_num >= 0);
      if (info == NULL || info->n_alts == 0)
	{
	  curr_alt_constraints [op_num] = "\0";
	  ok_p = assign_constraint (a, "\0");
	  gcc_assert (ok_p);
	  find_best_alt_allocation (a_num + 1);
	}
      else if (! TEST_ALT (INSN_ALLOCNO_POSSIBLE_ALTS (a),
			   curr_allocation_alt))
	;
      else
	{
	  constraints = info->op_constraints [op_num * info->n_alts
					      + curr_allocation_alt];
	  if (constraints == NULL || *constraints == '\0')
	    {
	      curr_alt_constraints [op_num] = "\0";
	      ok_p = assign_constraint (a, "\0");
	      gcc_assert (ok_p);
	      find_best_alt_allocation (a_num + 1);
	    }
	  else
	    {
	      int cost;

	      for (cost = 0;
		   (c = *constraints);
		   constraints += CONSTRAINT_LEN (c, constraints))
		if (c == '#')
		  break;
	        /* We can not ignore '?' and '!' because it is needed
                   for correct work, e.g. problem of sharing FP and
                   MMX registers is solved such way in gcc.  */
		else if (c == '?')
		  cost
		    += 6 * BLOCK_FOR_INSN (INSN_ALLOCNO_INSN (a))->frequency;
		else if (c == '!')
		  cost
		    = 600 * BLOCK_FOR_INSN (INSN_ALLOCNO_INSN (a))->frequency;
		else if (c == ' ' || c == '\t' || c == '=' || c == '+'
			 || c == '*' || c == '&' || c == '%'
			 /* We ignore this ambigous hints.  The cost
			    should be defined just by move costs. */
			 || c == '?' || c == '!')
		  ;
		else if (assign_constraint (a, constraints))
		  {
		    global_allocation_cost += cost;
		    curr_alt_constraints [op_num] = constraints;
		    find_best_alt_allocation (a_num + 1);
		    undo_transaction ();
		    start_transaction ();
		  }
	    }
	}
    }
  undo_transaction ();
  gcc_assert (global_allocation_cost == before);
}

static void
change_other_allocno_container_loc (allocno_t a, allocno_t a2,
				    rtx *old_loc, rtx *old_loc2)
{
  allocno_t a1;
  
  gcc_assert (INSN_ALLOCNO_INSN (a) == INSN_ALLOCNO_INSN (a2)
	      && INSN_ALLOCNO_COMMUTATIVE (a) == a2);
  for (a1 = insn_allocnos [INSN_UID (INSN_ALLOCNO_INSN (a))];
       a1 != NULL;
       a1 = INSN_ALLOCNO_NEXT (a1))
    if (a == a1 || a2 == a1)
      ;
    else if (INSN_ALLOCNO_CONTAINER_LOC (a1) == old_loc)
      INSN_ALLOCNO_CONTAINER_LOC (a1) = INSN_ALLOCNO_LOC (a);
    else if (INSN_ALLOCNO_CONTAINER_LOC (a1) == old_loc2)
      INSN_ALLOCNO_CONTAINER_LOC (a1) = INSN_ALLOCNO_LOC (a2);
}

/* It should work only during initial allocation.  */
void
make_commutative_exchange (allocno_t a)
{
  rtx tmp_rtx;
  int temp;
  bool temp_bool;
  rtx *op_loc, *op_loc2;
  struct insn_op_info *info;
  allocno_t tied_allocno, tied_allocno2;
  allocno_t a2 = INSN_ALLOCNO_COMMUTATIVE (a);

  gcc_assert (a2 != NULL && INSN_ALLOCNO_TYPE (a) >= OPERAND_BASE
	      && ALLOCNO_HARD_REGNO (a) < 0 && ALLOCNO_HARD_REGNO (a2) < 0
	      && ALLOCNO_MEMORY_SLOT (a) == NULL
	      && ALLOCNO_MEMORY_SLOT (a2) == NULL
	      && INSN_ALLOCNO_ELIMINATION (a) == NULL
	      && INSN_ALLOCNO_ELIMINATION (a2) == NULL
	      && ! INSN_ALLOCNO_CONST_POOL_P (a)
	      && ! INSN_ALLOCNO_CONST_POOL_P (a2)
	      && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a)
	      && ! INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a2)
	      && INSN_ALLOCNO_OP_MODE (a) == INSN_ALLOCNO_OP_MODE (a2)
	      && (INSN_ALLOCNO_COMMUTATIVE (a)
		  != INSN_ALLOCNO_COMMUTATIVE (a2)));
  tmp_rtx = *INSN_ALLOCNO_LOC (a);
  *INSN_ALLOCNO_LOC (a) = *INSN_ALLOCNO_LOC (a2);
  *INSN_ALLOCNO_LOC (a2) = tmp_rtx;
  temp = INSN_ALLOCNO_TYPE (a);
  INSN_ALLOCNO_TYPE (a) = INSN_ALLOCNO_TYPE (a2);
  INSN_ALLOCNO_TYPE (a2) = temp;
  temp_bool = INSN_ALLOCNO_EARLY_CLOBBER (a);
  INSN_ALLOCNO_EARLY_CLOBBER (a) = INSN_ALLOCNO_EARLY_CLOBBER (a2);
  INSN_ALLOCNO_EARLY_CLOBBER (a2) = temp_bool;
  op_loc = INSN_ALLOCNO_LOC (a);
  op_loc2 = INSN_ALLOCNO_LOC (a2);
  INSN_ALLOCNO_LOC (a) = op_loc2;
  INSN_ALLOCNO_CONTAINER_LOC (a)
    = get_container_loc (op_loc2, &PATTERN (INSN_ALLOCNO_INSN (a)));
  INSN_ALLOCNO_LOC (a2) = op_loc;
  INSN_ALLOCNO_CONTAINER_LOC (a2)
    = get_container_loc (op_loc, &PATTERN (INSN_ALLOCNO_INSN (a2)));
  change_other_allocno_container_loc (a, a2, op_loc, op_loc2);
  tied_allocno = INSN_ALLOCNO_TIED_ALLOCNO (a);
  tied_allocno2 = INSN_ALLOCNO_TIED_ALLOCNO (a2);
  if (tied_allocno == NULL)
    INSN_ALLOCNO_TIED_ALLOCNO (a2) = NULL;
  else
    {
      INSN_ALLOCNO_TIED_ALLOCNO (a2) = tied_allocno;
      INSN_ALLOCNO_TIED_ALLOCNO (tied_allocno) = a2;
    }
  if (tied_allocno2 == NULL)
    INSN_ALLOCNO_TIED_ALLOCNO (a) = NULL;
  else
    {
      INSN_ALLOCNO_TIED_ALLOCNO (a) = tied_allocno2;
      INSN_ALLOCNO_TIED_ALLOCNO (tied_allocno2) = a;
    }
  info = insn_infos [INSN_UID (INSN_ALLOCNO_INSN (a))];
  set_up_possible_allocno_alternatives (info, a, false);
  set_up_possible_allocno_alternatives (info, a2, false);
}

static int init_insn_allocation_cost;

/* Recursive_function */
static void
find_best_alt_allocation (int a_num)
{
  allocno_t a, a2;
  int i, op_num, n;
  rtx insn;

  if (VARRAY_ACTIVE_SIZE (insn_allocno_varray) <= (unsigned) a_num)
    {
      int cost = global_allocation_cost - init_insn_allocation_cost;

      if (min_alt_cost_undef_p || cost < min_alt_cost)
	{
	  min_alt_cost_undef_p = false;
	  min_alt_cost = cost;
	  for (i = 0;
	       i < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray);
	       i++)
	    {
	      a = VARRAY_GENERIC_PTR (insn_allocno_varray, i);
	      op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
	      if (op_num >= 0)
		{
		  best_alt_constraints [op_num]
		    = curr_alt_constraints [op_num];
		  best_alt_op_exchange_p [op_num]
		    = curr_alt_op_exchange_p [op_num];
		}
	    }
	}
      return;
    }
  a = VARRAY_GENERIC_PTR (insn_allocno_varray, a_num);
  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
  for (n = 0; n < 2; n++)
     {
       if (op_num >= 0)
	 curr_alt_op_exchange_p [op_num] = n != 0;
       insn = INSN_ALLOCNO_INSN (a);
       try_insn_allocno_allocation (a_num);
       a2 = INSN_ALLOCNO_COMMUTATIVE (a);
       if (a2 == NULL)
	 break;
       for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray); i++)
	 if (VARRAY_GENERIC_PTR (insn_allocno_varray, i) == a2)
	   break;
       if (i < a_num)
	 break;
       gcc_assert (i < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray));
       make_commutative_exchange (a);
       VARRAY_GENERIC_PTR (insn_allocno_varray, a_num) = a2;
       VARRAY_GENERIC_PTR (insn_allocno_varray, i) = a;
       a = a2;
       gcc_assert (op_num == INSN_ALLOCNO_TYPE (a) - OPERAND_BASE);
     }
}

/* Return duplication number in the COSTRAINT.  Return -1 if there is
   no such number.  */
static int
get_duplication_number (const char *constraints)
{
  char c;

  if (constraints != NULL)
    for (; (c = *constraints); constraints += CONSTRAINT_LEN (c, constraints))
      switch (c)
	 {
	 case '#':
	   return -1;

	 case '0': case '1': case '2': case '3': case '4':
	 case '5': case '6': case '7': case '8': case '9':
	   return c - '0';
	 }
  return -1;
}

/* Placing of the original (or/and commutative) after the duplication
   for the current alternative of the current insn.  */
static void
put_duplication_before_original (void)
{
  int i, j, dup_n, op_num;
  const char *constraints;
  allocno_t a, original = NULL, commutative;
  struct insn_op_info *info;

  info = insn_infos [INSN_UID (curr_allocation_insn)];
  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray); i++)
    {
      a = VARRAY_GENERIC_PTR (insn_allocno_varray, i);
      if ((op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE) < 0)
	 continue;
      constraints = info->op_constraints [op_num * info->n_alts
					   + curr_allocation_alt];
      dup_n = get_duplication_number (constraints);
      if (dup_n < 0)
	 continue;
      for (j = 0; j < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray); j++)
	 {
	   original = VARRAY_GENERIC_PTR (insn_allocno_varray, j);
	   if (INSN_ALLOCNO_TYPE (original) - OPERAND_BASE == dup_n
	       || ((commutative = INSN_ALLOCNO_COMMUTATIVE (original)) != NULL
		   && ((INSN_ALLOCNO_TYPE (commutative) - OPERAND_BASE)
		       == dup_n)))
	     break;
	 }
      gcc_assert (j >= 0 && j != i
		  && j < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray));
      if (j < i)
	 {
	   if (i != j + 1)
	     memmove (&VARRAY_GENERIC_PTR (insn_allocno_varray, j + 2),
		      &VARRAY_GENERIC_PTR (insn_allocno_varray, j + 1),
		    (i - j - 1) * sizeof (allocno_t));
	   VARRAY_GENERIC_PTR (insn_allocno_varray, j) = a;
	   VARRAY_GENERIC_PTR (insn_allocno_varray, j + 1) = original;
	 }
    }
}

static int
store_best_allocation (int min_cost, bool *min_cost_undef_p)
{
  int i, op_num;
  allocno_t a;

 if (*min_cost_undef_p || min_cost > min_alt_cost)
   {
     *min_cost_undef_p = false;
     min_cost = min_alt_cost;
     VARRAY_POP_ALL (best_insn_allocno_varray);
     for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray); i++)
	{
	  a = VARRAY_GENERIC_PTR (insn_allocno_varray, i);
	  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
	  if (op_num >= 0)
	    {
	      best_constraints [op_num]	= best_alt_constraints [op_num];
	      best_op_exchange_p [op_num] = best_alt_op_exchange_p [op_num];
	    }
	  VARRAY_PUSH_GENERIC_PTR (best_insn_allocno_varray, a);
	}
   }
  return min_cost;
}

static bool
find_best_allocation (rtx insn)
{
  struct insn_op_info *info;
  int min_cost;
  bool min_cost_undef_p;
  allocno_t a;

  curr_allocation_insn = insn;
  info = insn_infos [INSN_UID (insn)];
  min_cost_undef_p = true;
  min_cost = -1;
  VARRAY_POP_ALL (insn_allocno_varray);
  for (a = insn_allocnos [INSN_UID (insn)];
       a != NULL;
       a = INSN_ALLOCNO_NEXT (a))
    VARRAY_PUSH_GENERIC_PTR (insn_allocno_varray, a);
  init_insn_allocation_cost = global_allocation_cost;
  if (info == NULL || info->n_alts == 0)
    {
      min_alt_cost_undef_p = true;
      find_best_alt_allocation (0);
      if (! min_alt_cost_undef_p)
	min_cost = store_best_allocation (min_cost, &min_cost_undef_p);
    }
  else
    {
      for (curr_allocation_alt = 0;
	   curr_allocation_alt < info->n_alts;
	   curr_allocation_alt++)
	{
	  (*insn_allocno_sort_func)
	    ((allocno_t *) &VARRAY_GENERIC_PTR (insn_allocno_varray, 0),
	     VARRAY_ACTIVE_SIZE (insn_allocno_varray),
	     insn, curr_allocation_alt);
	  put_duplication_before_original ();
	  min_alt_cost_undef_p = true;
	  start_transaction ();
	  find_best_alt_allocation (0);
	  if (! min_alt_cost_undef_p)
	    min_cost = store_best_allocation (min_cost, &min_cost_undef_p);
	  undo_transaction ();
	}
    }
  return ! min_cost_undef_p;
}

void
allocate_insn_allocnos (rtx insn, bool (*call_cross_hint) (allocno_t),
			void (*insn_allocno_sort) (allocno_t *, int, rtx, int),
			int (*provide_allocno_class_hard_reg) (allocno_t,
							       enum reg_class))
{
  int i, op_num;
  allocno_t a;

  call_cross_hint_func = call_cross_hint;
  insn_allocno_sort_func = insn_allocno_sort;
  provide_allocno_class_hard_reg_func = provide_allocno_class_hard_reg;
  VARRAY_POP_ALL (insn_allocno_varray);
  VARRAY_POP_ALL (best_insn_allocno_varray);
  if (! find_best_allocation (insn))
    gcc_unreachable ();
  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (best_insn_allocno_varray); i++)
    {
      a = VARRAY_GENERIC_PTR (best_insn_allocno_varray, i);
      if (INSN_ALLOCNO_TYPE (a) == NON_OPERAND)
	{
	  if (ALLOCNO_REGNO (a) < 0 || HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a)))
	    {
	      if (! assign_allocno (a, LIM_REG_CLASSES,
				    reg_class_contents [NO_REGS], -1))
		gcc_unreachable ();
	    }
	  else if (! assign_hard_regno_to_pseudo_reg_insn_allocno
		     (a, GENERAL_REGS, reg_class_contents [GENERAL_REGS]))
	    gcc_unreachable ();
	}
      else if (INSN_ALLOCNO_TYPE (a) == BASE_REG
	       || INSN_ALLOCNO_TYPE (a) == INDEX_REG)
	{
	  /* See analogous code in find_best_alt_allocation.  */
	  rtx reg = *INSN_ALLOCNO_LOC (a);
	  HARD_REG_SET temp_set;
	  enum reg_class cl;

	  gcc_assert (REG_P (reg)
		      && (INSN_ALLOCNO_OP_MODE (a) == OP_IN
			  || INSN_ALLOCNO_OP_MODE (a) == OP_INOUT)
		      /* We assume no base and index registers in
			 subregisters.  */
		      && GET_CODE (*INSN_ALLOCNO_CONTAINER_LOC (a)) != SUBREG);
	  if (INSN_ALLOCNO_TYPE (a) == BASE_REG)
	    {
	      COPY_HARD_REG_SET
		(temp_set,
		 base_regs [GET_MODE (*INSN_ALLOCNO_CONTAINER_LOC (a))]);
	      cl = BASE_REG_CLASS;
	    }
	  else
	    {
	      COPY_HARD_REG_SET (temp_set, index_regs);
	      cl = INDEX_REG_CLASS;
	    }
	  if (HARD_REGISTER_P (reg)
	      && hard_reg_in_set_p (REGNO (reg), ALLOCNO_MODE (a), temp_set))
	    {
	      if (! assign_allocno (a, LIM_REG_CLASSES,
				    reg_class_contents [NO_REGS], -1))
		gcc_unreachable ();
	    }
	  else if (! assign_hard_regno_to_pseudo_reg_insn_allocno (a, cl,
								   temp_set))
	    gcc_unreachable ();
	}
      else
	{
	  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
	  if (best_op_exchange_p [op_num])
	    {
	      int j;
	      allocno_t a2 = INSN_ALLOCNO_COMMUTATIVE (a);

	      for (j = i + 1;
		   j < (int) VARRAY_ACTIVE_SIZE (best_insn_allocno_varray);
		   j++)
		if (VARRAY_GENERIC_PTR (best_insn_allocno_varray, j) == a2)
		  break;
	      gcc_assert
		(j < (int) VARRAY_ACTIVE_SIZE (best_insn_allocno_varray));
	      make_commutative_exchange (a);
	      VARRAY_GENERIC_PTR (best_insn_allocno_varray, i) = a2;
	      VARRAY_GENERIC_PTR (best_insn_allocno_varray, j) = a;
	      a = a2;
	      gcc_assert (op_num == INSN_ALLOCNO_TYPE (a) - OPERAND_BASE);
	    }
	  if (! assign_constraint (a, best_constraints [op_num]))
	    gcc_unreachable ();
	}
    }
}

void
yara_reload_init (void)
{
  VARRAY_GENERIC_PTR_NOGC_INIT (insn_allocno_varray, MAX_RECOG_OPERANDS * 4,
				"insn allocnos");
  VARRAY_GENERIC_PTR_NOGC_INIT (best_insn_allocno_varray,
				MAX_RECOG_OPERANDS * 4,
				"insn allocnos in best order");
}

void
yara_reload_finish (void)
{
  VARRAY_FREE (best_insn_allocno_varray);
  VARRAY_FREE (insn_allocno_varray);
}
