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
#include "toplev.h"
#include "yara-int.h"



static bool check_alternative_possibility (allocno_t, const char *, bool);
static void setup_possible_operand_alternatives (rtx, bool);

static bool try_hard_regno_from_connected_allocno (allocno_t, enum reg_class,
						   HARD_REG_SET, bool, bool *);
static bool assign_hard_regno_to_pseudo_reg_insn_allocno (allocno_t,
							  enum reg_class,
							  HARD_REG_SET);
static bool assign_constraint (allocno_t, const char *);
static void find_best_alt_allocation_1 (void);
static void find_best_alt_allocation (void);
static int get_duplication_number (const char *);
static void put_duplication_before_original (void);
static bool find_best_allocation (rtx);
static bool assign_pseudo_allocno_from_connected_allocno (allocno_t);
static bool assign_insn_allocnos_without_copy (rtx insn);



/* The function checks that constraint *P is possible (in strict sense
   if STRICT_P) for allocno A.  The function returns true for the
   successful try.  */
static bool
check_alternative_possibility (allocno_t a, const char *p, bool strict_p)
{
  rtx x, op, no_subreg_op, equiv_const;
  allocno_t another_a;
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
      else if ((MEM_P (no_subreg_op) && c == 'm')
	       || (c == 'o' && offsettable_nonstrict_memref_p (no_subreg_op)))
	return true;
      /* Accept a register which might be placed in memory.  */
      else if (REG_P (no_subreg_op)
	       && (! strict_p || ALLOCNO_MEMORY_SLOT (a) != NULL))
	return true;
      /* Accept a register which might be placed in memory.  */
      else if (CONST_POOL_OK_P (op)
	       || (equiv_const != NULL_RTX && CONST_POOL_OK_P (equiv_const)))
	{
	  if (! INSN_ALLOCNO_ELIMINATION_PART_CONST_P (a))
	    return true;
	  x = *INSN_ALLOCNO_CONTAINER_LOC (a);
	  yara_assert (GET_CODE (x) == PLUS && XEXP (x, 1) == op);
	  another_a = insn_allocno (XEXP (x, 0), INSN_ALLOCNO_INSN (a));
	  yara_assert (another_a != NULL);
	  return ! ALLOCNO_USE_EQUIV_CONST_P (another_a);
	}
      else
	return false;
      
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
	    if (INSN_ALLOCNO_ELIMINATION_PART_CONST_P (a))
	      {
		x = *INSN_ALLOCNO_CONTAINER_LOC (a);
		yara_assert (GET_CODE (x) == PLUS && XEXP (x, 1) == op);
		another_a = insn_allocno (XEXP (x, 0),
					  INSN_ALLOCNO_INSN (a));
		yara_assert (another_a != NULL);
		if (! ALLOCNO_USE_EQUIV_CONST_P (another_a))
		  return true;
	      }
	    else if (equiv_const != NULL_RTX && op == no_subreg_op
		     && ! CONSTANT_P (equiv_const))
	      {
		x = *INSN_ALLOCNO_CONTAINER_LOC (a);
		if (GET_CODE (x) == PLUS)
		  {
		    another_a
		      = insn_allocno (XEXP (x, 1), INSN_ALLOCNO_INSN (a));
		    if (INSN_ALLOCNO_ELIMINATION_PART_CONST_P (another_a)
			&& ALLOCNO_HARD_REGNO (another_a) < 0
			&& ALLOCNO_MEMORY_SLOT (another_a) == NULL
			&& hard_reg_in_set_p (REGNO (XEXP (equiv_const, 0)),
					      ALLOCNO_MODE (a),
					      reg_class_contents [cl]))
		      return true;
		  }
	      }
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

/* The function sets up possible alternatives (in strict sense if
   STRICT_P) for allocno A representing an operand of insn given by
   INFO.  */
void
setup_possible_allocno_alternatives (struct insn_op_info *info, allocno_t a,
				      bool strict_p)
{
  int op_num, n_alt, alts_num;
  const char *constraints;
  char c;
  alt_set_t set;

  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
  yara_assert (op_num >= 0);
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
	    {
	      switch (c)
		{
		case ' ': case '\t': case '=': case '+': case '*':
		case '&': case '%': case '?': case '!':
		  continue;
		  
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		  {
		    allocno_t a1;
		    const char *str;
		    int n = c - '0';
		    
		    yara_assert (op_num > n);
		    a1 = insn_op_allocnos [INSN_UID (INSN_ALLOCNO_INSN (a))]
                                          [n];
		    if (! strict_p)
		      SET_ALT (set, n_alt);
		    else if (INSN_ALLOCNO_TIED_ALLOCNO (a) == a1
			     && TEST_ALT (INSN_ALLOCNO_POSSIBLE_ALTS (a1),
					  n_alt))
		      {
			/* ???!!! We have after allocation duplication
			   first.  */
			str = info->op_constraints [(n) * alts_num + n_alt];
			for (; (c = *str); str += CONSTRAINT_LEN (c, str))
			  {
			    switch (c)
			      {
			      case ' ': case '\t': case '=': case '+':
			      case '*': case '&': case '%': case '?': case '!':
				break;
			      default:
				yara_assert (c != '#');
				if (check_alternative_possibility (a, str,
								   true))
				  SET_ALT (set, n_alt);
				break;
			      }
			    if (TEST_ALT (set, n_alt))
			      break;
			  }
		      }
		    break;
		  }

		default:
		  yara_assert (c != '#');
		  if (check_alternative_possibility (a, constraints, strict_p))
		    SET_ALT (set, n_alt);
		  break;
		}
	      if (TEST_ALT (set, n_alt))
		break;
	    }
	}
    }
  INSN_ALLOCNO_POSSIBLE_ALTS (a) = set;
}

/* The function sets up possible alternatives (in strict sense if
   STRICT_P) for allocnos representing operands of INSN.  */
static void
setup_possible_operand_alternatives (rtx insn, bool strict_p)
{
  allocno_t op;
  struct insn_op_info *info;

  info = insn_infos [INSN_UID (insn)];
  for (op = insn_allocnos [INSN_UID (insn)];
       op != NULL;
       op = INSN_ALLOCNO_NEXT (op))
    if (INSN_ALLOCNO_TYPE (op) >= OPERAND_BASE)
      setup_possible_allocno_alternatives (info, op, strict_p);
}

void
setup_possible_alternatives (bool strict_p)
{
  basic_block bb;
  rtx insn;

  FOR_EACH_BB (bb)
    {
      FOR_BB_INSNS (bb, insn)
	if (INSN_P (insn))
	  setup_possible_operand_alternatives (insn, strict_p);
    }
}



static enum reg_class
limit_insn_allocno_class (allocno_t a ATTRIBUTE_UNUSED, enum reg_class class)
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
  rtx x, op, no_subreg_op, equiv_const;
  allocno_t another_a;
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
	  yara_assert (equiv_const != NULL_RTX
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
	  yara_assert (equiv_const != NULL_RTX
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
	  yara_assert (equiv_const != NULL_RTX
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
	  yara_assert (equiv_const != NULL_RTX
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
	  yara_assert (equiv_const != NULL_RTX
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
	  yara_assert (equiv_const != NULL_RTX
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
	  yara_assert (INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL
		       || INSN_ALLOCNO_ORIGINAL_P (a));
	  success_p = assign_allocno (a, LIM_REG_CLASSES,
				      reg_class_contents [NO_REGS], -1);
	}
      /* We could accept a constant that can be turned into mem.  */
      else if (CONST_POOL_OK_P (op)
	       || (equiv_const != NULL_RTX
		   && CONST_POOL_OK_P (equiv_const)))
	{
	  rtx x;

	  yara_assert (equiv_const != NULL_RTX
		       || (ALLOCNO_SRC_COPIES (a) == NULL
			   && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL));
	  if (! INSN_ALLOCNO_ELIMINATION_PART_CONST_P (a))
	    success_p = true;
	  else
	    {
	      x = *INSN_ALLOCNO_CONTAINER_LOC (a);
	      yara_assert (GET_CODE (x) == PLUS && XEXP (x, 1) == op);
	      another_a = insn_allocno (XEXP (x, 0), INSN_ALLOCNO_INSN (a));
	      yara_assert (another_a != NULL);
	      success_p = ! ALLOCNO_USE_EQUIV_CONST_P (another_a);
	    }
	  success_p
	    = success_p && assign_allocno (a, NO_REGS,
					   reg_class_contents [NO_REGS], -1);
	}
      /* Accept a register which might be placed in memory.  */
      else if (REG_P (no_subreg_op))
	success_p = assign_allocno (a, NO_REGS,
				    reg_class_contents [NO_REGS], -1);
      break;
      

    case 'V':
      if (MEM_P (op) && ! offsettable_nonstrict_memref_p (op))
	{
	  yara_assert (INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL
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
	  yara_assert (! CONSTANT_P (op) || equiv_const != NULL_RTX
		       || ALLOCNO_SRC_COPIES (a) == NULL);
	  yara_assert (INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL
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
	    yara_assert (! REG_P (no_subreg_op) || ALLOCNO_REGNO (a) >= 0);

	    cl = limit_insn_allocno_class (a, cl);

	    if (cl == NO_REGS)
	      ;
	    else if (REG_P (no_subreg_op) && HARD_REGISTER_P (no_subreg_op)
		     && hard_reg_in_set_p (get_allocno_hard_regno
					   (a, ALLOCNO_REGNO (a)),
					   ALLOCNO_MODE (a),
					   reg_class_contents [cl])
		     && hard_reg_not_in_set_p (get_allocno_hard_regno
					       (a, ALLOCNO_REGNO (a)),
					       ALLOCNO_MODE (a),
					       ALLOCNO_HARD_REG_CONFLICTS (a)))
	      {
		yara_assert (INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL
			     || INSN_ALLOCNO_ORIGINAL_P (a));
		success_p = assign_allocno (a, LIM_REG_CLASSES,
					    reg_class_contents [NO_REGS], -1);
		if (! success_p
		    && assign_hard_regno_to_pseudo_reg_insn_allocno
		       (a, cl, reg_class_contents [cl]))
		  success_p = true;
	      }
	    else
	      {
		if (INSN_ALLOCNO_ELIMINATION_PART_CONST_P (a))
		  {
		    x = *INSN_ALLOCNO_CONTAINER_LOC (a);
		    yara_assert (GET_CODE (x) == PLUS && XEXP (x, 1) == op);
		    another_a = insn_allocno (XEXP (x, 0),
					      INSN_ALLOCNO_INSN (a));
		    yara_assert (another_a != NULL);
		    if (ALLOCNO_USE_EQUIV_CONST_P (another_a))
		      break;
		  }
		else if (equiv_const != NULL_RTX && op == no_subreg_op
			 && ! CONSTANT_P (equiv_const))
		  {
		    x = *INSN_ALLOCNO_CONTAINER_LOC (a);
		    if (GET_CODE (x) == PLUS)
		      {
			another_a
			  = insn_allocno (XEXP (x, 1), INSN_ALLOCNO_INSN (a));
			if (INSN_ALLOCNO_ELIMINATION_PART_CONST_P (another_a)
			    && ALLOCNO_HARD_REGNO (another_a) < 0
			    && ALLOCNO_MEMORY_SLOT (another_a) == NULL
			    && hard_reg_in_set_p (REGNO (XEXP (equiv_const,
							       0)),
						  ALLOCNO_MODE (a),
						  reg_class_contents [cl])
			    && assign_allocno (a, LIM_REG_CLASSES,
					       reg_class_contents [NO_REGS],
					       -1))
			  {
			    success_p = true;
			    break;
			  }
		      }
		  }
		if (assign_hard_regno_to_pseudo_reg_insn_allocno
		    (a, cl, reg_class_contents [cl]))
		  success_p = true;
		/* Accept a register which might be placed in memory.  */
		else if (c == 'g' && REG_P (no_subreg_op))
		  success_p
		    = assign_allocno (a, NO_REGS,
				      reg_class_contents [NO_REGS], -1);
	      }
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
	another_a = (insn_op_allocnos
		     [INSN_UID (INSN_ALLOCNO_INSN (a))] [c - '0']);
	yara_assert ((INSN_ALLOCNO_OP_MODE (a) == OP_IN
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

/* The number of the current insn alternative in the
   consideration.  */
static int curr_allocation_alt;
/* Info about the current insn.  */
struct insn_op_info *curr_allocation_insn_info;
/* The variable value is true when we found an allocation of allocnos
   of the current insn.  */
static bool min_insn_cost_undef_p;
/* Cost of the best allocation of allocnos of the current insn.  */
static int min_insn_cost;
/* Contraints used for the best allocation.  */
static const char *best_insn_constraints [MAX_RECOG_OPERANDS];
/* Allocno should be exchanged (if any) to get the best
   allocation.  */
static allocno_t best_insn_exchange_allocno;

/* Varray used as a stack to try all operand constraints.  */
static varray_type op_try_varray;

/* Varray containing references to the current insn allocnos for the
   current alternative.  */
static varray_type insn_allocno_varray;

/* Varray containing references to the current insn allocnos in order
   for the best allocation.  */
static varray_type best_insn_allocno_varray;

/* Constraints currently being tried for the current insn allocnos for
   the current alternative.  */
static const char *curr_alt_constraints [MAX_RECOG_OPERANDS];
/* Allocno (if any) exchanged for the current insn allocnos for the
   current alternative.  */
static allocno_t curr_alt_exchange_allocno;

/* The function exchange container locations of commutative allocnos A
   and A2 with the corresponding locations OLD_LOC and OLD_LOC2 if
   they coincide with container locs.  */
static void
change_other_op_allocno_container_loc (allocno_t a, allocno_t a2,
				       rtx *old_loc, rtx *old_loc2)
{
  allocno_t a1;
  
  yara_assert (INSN_ALLOCNO_INSN (a) == INSN_ALLOCNO_INSN (a2)
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

/* The function exchanges insn allocno A with the corresponding
   commutative allocno without changing possible alternatives.  */
void
make_commutative_exchange (allocno_t a)
{
  rtx tmp_rtx;
  int temp, uid;
  bool temp_bool;
  rtx *op_loc, *op_loc2;
  allocno_t tied_allocno, tied_allocno2;
  allocno_t a2 = INSN_ALLOCNO_COMMUTATIVE (a);

  yara_assert (a2 != NULL && INSN_ALLOCNO_TYPE (a) >= OPERAND_BASE
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
  uid = INSN_UID (INSN_ALLOCNO_INSN (a));
  insn_op_allocnos [uid] [temp - OPERAND_BASE] = a2;
  insn_op_allocnos [uid] [INSN_ALLOCNO_TYPE (a) - OPERAND_BASE] = a;
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
  change_other_op_allocno_container_loc (a, a2, op_loc, op_loc2);
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
}

/* Cost of all allocation before allocating insn allocnos of the
   current insn.  */
static int init_insn_allocation_cost;

/* The major function searching for the best allocation of insn
   allocnos of the current insn for the current alternative without
   making commutative operand exchange.  */
static void
find_best_alt_allocation_1 (void)
{
  allocno_t a;
  int a_num, op_num, cost, start;
  const char *constraints, *next_constraints;
  bool ok_p;
  char c;
  int op_alt_cost [MAX_RECOG_OPERANDS];
  const char *op_alt_constraints [MAX_RECOG_OPERANDS];
  
  if (VARRAY_ACTIVE_SIZE (insn_allocno_varray) == 0)
    {
      min_insn_cost_undef_p = false;
      min_insn_cost = 0;
      return;
    }
  start_transaction ();
  VARRAY_POP_ALL (op_try_varray);
  VARRAY_PUSH_INT (op_try_varray, 0);
  a = VARRAY_GENERIC_PTR (insn_allocno_varray, 0);
  if ((op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE) >= 0
      && curr_allocation_insn_info->op_constraints != NULL)
    {
      op_alt_constraints [op_num]
	= curr_allocation_insn_info->op_constraints
	  [op_num * curr_allocation_insn_info->n_alts + curr_allocation_alt];
      op_alt_cost [op_num] = 0;
    }
  for (;;)
    {
      start = VARRAY_INT (op_try_varray,
			  VARRAY_ACTIVE_SIZE (op_try_varray) - 1);
      VARRAY_POP (op_try_varray);
      for (a_num = start;
	   a_num < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray);
	   a_num++)
	{
	  if (! min_insn_cost_undef_p
	      && (global_allocation_cost - init_insn_allocation_cost
		  > min_insn_cost))
	    break;
	  a = VARRAY_GENERIC_PTR (insn_allocno_varray, a_num);
	  if (INSN_ALLOCNO_TYPE (a) == BASE_REG
	      || INSN_ALLOCNO_TYPE (a) == INDEX_REG)
	    {
	      rtx reg = *INSN_ALLOCNO_LOC (a);
	      HARD_REG_SET temp_set;
	      enum reg_class cl;
	      
	      yara_assert (REG_P (reg)
			   && INSN_ALLOCNO_TIED_ALLOCNO (a) == NULL
			   && (INSN_ALLOCNO_OP_MODE (a) == OP_IN
			       || INSN_ALLOCNO_OP_MODE (a) == OP_INOUT)
			   /* We assume no base and index registers in
			      subregisters.  */
			   && (GET_CODE (*INSN_ALLOCNO_CONTAINER_LOC (a))
			       != SUBREG));
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
		  && hard_reg_in_set_p (REGNO (reg), ALLOCNO_MODE (a),
					temp_set))
		{
		  if (! assign_allocno (a, LIM_REG_CLASSES,
					reg_class_contents [NO_REGS], -1))
		    gcc_unreachable ();
		}
	      else
		{
		  int to;
		  bool base_p;
		  HOST_WIDE_INT offset;
		  bool cont_p = true;
		  rtx x, *address_loc, *container_loc;

		  if ((x = reg_equiv_constant [ALLOCNO_REGNO (a)]) != NULL_RTX
		      && ! CONSTANT_P (x))
		    {
		      get_equiv_const_elimination_info (x, &to, &offset);
		      get_equiv_const_addr_info (x, &to, &offset);
		      container_loc = INSN_ALLOCNO_CONTAINER_LOC (a);
		      address_loc = (GET_CODE (*container_loc) == MEM
				     ? &XEXP (*container_loc, 0)
				     : container_loc);
		      /* ??? Allocate interm equiv constant register.  Do
			 we need this?  */
		      if (check_elimination_in_addr (ALLOCNO_REGNO (a), to,
						     offset, address_loc,
						     container_loc,
						     &base_p) != NULL
			  && assign_allocno (a, LIM_REG_CLASSES,
					     reg_class_contents [NO_REGS], -1))
			cont_p = false;
		    }
		  if (cont_p
		      && ! assign_hard_regno_to_pseudo_reg_insn_allocno
		           (a, cl, temp_set))
		    break;
		}
	    }
	  else if (INSN_ALLOCNO_TYPE (a) == NON_OPERAND)
	    {
	      if (ALLOCNO_REGNO (a) < 0
		  || HARD_REGISTER_NUM_P (ALLOCNO_REGNO (a)))
		{
		  if (! assign_allocno (a, LIM_REG_CLASSES,
					reg_class_contents [NO_REGS], -1))
		    gcc_unreachable ();
		}
	      /* ??? GENERAL_REGS */
	      else if (! assign_hard_regno_to_pseudo_reg_insn_allocno
		         (a, GENERAL_REGS, reg_class_contents [GENERAL_REGS]))
		gcc_unreachable ();
	    }
	  else
	    {
	      op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
	      yara_assert (op_num >= 0);
	      if (a_num == start)
		constraints = op_alt_constraints [op_num];
	      else
		constraints
		  = curr_allocation_insn_info->op_constraints
		    [op_num * curr_allocation_insn_info->n_alts
		     + curr_allocation_alt];
	      if (constraints == NULL)
		{
		  curr_alt_constraints [op_num] = "\0";
		  ok_p = assign_constraint (a, "\0");
		  yara_assert (ok_p);
		}
	      else if (! TEST_ALT (INSN_ALLOCNO_POSSIBLE_ALTS (a),
				   curr_allocation_alt))
		break;
	      else
		{
		  if (a_num == start)
		    cost = op_alt_cost [op_num];
		  else
		    cost = 0;
		  for (; (c = *constraints); constraints = next_constraints)
		    {
		      next_constraints
			= constraints + CONSTRAINT_LEN (c, constraints);
		      switch (c)
			{
			  /* We can not ignore '?' and '!' because it is needed
			     for correct work, e.g. problem of sharing FP and
			     MMX registers is solved such way in gcc.  */
			case '?':
			  cost += (6 * (BLOCK_FOR_INSN (INSN_ALLOCNO_INSN (a))
					->frequency));
			  continue;

			case '!':
			  cost = (600 * (BLOCK_FOR_INSN (INSN_ALLOCNO_INSN (a))
					 ->frequency));
			  continue;

			case ' ': case '\t': case '=': case '+':
			case '*': case '&': case '%':
			  continue;

			default:
			  yara_assert (c != '#');
			  if (*next_constraints != '\0')
			    start_transaction ();
			  if (assign_constraint (a, constraints))
			    {
			      global_allocation_cost += cost;
			      curr_alt_constraints [op_num] = constraints;
			      if (*next_constraints != '\0')
				{
				  op_alt_cost [op_num] = cost;
				  op_alt_constraints [op_num]
				    = next_constraints;
				  VARRAY_PUSH_INT (op_try_varray, a_num);
				}
			    }
			  else
			    {
			      if (*next_constraints != '\0')
				undo_transaction ();
			      continue;
			    }
			}
		      break;
		    }
		  if (c == '\0')
		    break;
		}
	    }
	}
      if (VARRAY_ACTIVE_SIZE (insn_allocno_varray) == (unsigned) a_num)
	{
	  int cost = global_allocation_cost - init_insn_allocation_cost;
	  
	  if (min_insn_cost_undef_p || cost < min_insn_cost)
	    {
	      min_insn_cost_undef_p = false;
	      min_insn_cost = cost;
	      best_insn_exchange_allocno = curr_alt_exchange_allocno;
	      VARRAY_POP_ALL (best_insn_allocno_varray);
	      for (a_num = 0;
		   a_num < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray);
		   a_num++)
		{
		  a = VARRAY_GENERIC_PTR (insn_allocno_varray, a_num);
		  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
		  if (op_num >= 0)
		    best_insn_constraints [op_num]
		      = curr_alt_constraints [op_num];
		  VARRAY_PUSH_GENERIC_PTR (best_insn_allocno_varray, a);
		}
	    }
	}
      if (VARRAY_ACTIVE_SIZE (op_try_varray) == 0)
	break;
      undo_transaction ();
    }
  undo_transaction ();
}

/* The function searches for the best allocation of insn allocnos of
   the current insn for the current alternative making commutative
   operand exchange if it is possible.  */
static void
find_best_alt_allocation (void)
{
  int i, j, op_num;
  allocno_t a, a2;
  alt_set_t saved_alt_set, saved_alt_set2;

  curr_alt_exchange_allocno = NULL;
  find_best_alt_allocation_1 ();
  if (curr_allocation_insn_info->commutative_op_p)
    {
      a = a2 = NULL;
      for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray); i++)
	{
	  a = VARRAY_GENERIC_PTR (insn_allocno_varray, i);
	  a2 = INSN_ALLOCNO_COMMUTATIVE (a);
	  if (a2 != NULL)
	    break;
	}
      yara_assert (i < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray));
      for (j = i + 1; j < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray); j++)
	if (VARRAY_GENERIC_PTR (insn_allocno_varray, j) == a2)
	  break;
      yara_assert (j < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray));
      op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
      yara_assert (op_num >= 0);
      COPY_ALT_SET (saved_alt_set, INSN_ALLOCNO_POSSIBLE_ALTS (a));
      COPY_ALT_SET (saved_alt_set2, INSN_ALLOCNO_POSSIBLE_ALTS (a2));
      make_commutative_exchange (a);
      setup_possible_allocno_alternatives
	(curr_allocation_insn_info, a, false);
      setup_possible_allocno_alternatives
	(curr_allocation_insn_info, a2, false);
      VARRAY_GENERIC_PTR (insn_allocno_varray, i) = a2;
      VARRAY_GENERIC_PTR (insn_allocno_varray, j) = a;
      curr_alt_exchange_allocno = a;
      find_best_alt_allocation_1 ();
      make_commutative_exchange (a);
      COPY_ALT_SET (INSN_ALLOCNO_POSSIBLE_ALTS (a), saved_alt_set);
      COPY_ALT_SET (INSN_ALLOCNO_POSSIBLE_ALTS (a2), saved_alt_set2);
      VARRAY_GENERIC_PTR (insn_allocno_varray, i) = a;
      VARRAY_GENERIC_PTR (insn_allocno_varray, j) = a2;
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
	 case '0': case '1': case '2': case '3': case '4':
	 case '5': case '6': case '7': case '8': case '9':
	   return c - '0';
	 default:
	   yara_assert (c != '#');
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

  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (insn_allocno_varray); i++)
    {
      a = VARRAY_GENERIC_PTR (insn_allocno_varray, i);
      if ((op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE) < 0)
	 continue;
      constraints
	= curr_allocation_insn_info->op_constraints
	  [op_num * curr_allocation_insn_info->n_alts + curr_allocation_alt];
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
      yara_assert (j >= 0 && j != i
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

/* The function searches best allocation for allocnos in INSN.  It
   returns true if an allocation found.  The best allocation is stored
   in best_insn_allocno_varray, best_insn_exchange_allocno,
   best_insn_constraints.  */
static bool
find_best_allocation (rtx insn)
{
  allocno_t a;

  min_insn_cost_undef_p = true;
  min_insn_cost = -1;
  VARRAY_POP_ALL (insn_allocno_varray);
  for (a = insn_allocnos [INSN_UID (insn)];
       a != NULL;
       a = INSN_ALLOCNO_NEXT (a))
    VARRAY_PUSH_GENERIC_PTR (insn_allocno_varray, a);
  init_insn_allocation_cost = global_allocation_cost;
  if (curr_allocation_insn_info == NULL
      || curr_allocation_insn_info->n_alts == 0)
    find_best_alt_allocation ();
  else
    {
      for (curr_allocation_alt = 0;
	   curr_allocation_alt < curr_allocation_insn_info->n_alts;
	   curr_allocation_alt++)
	{
	  (*insn_allocno_sort_func)
	    ((allocno_t *) &VARRAY_GENERIC_PTR (insn_allocno_varray, 0),
	     VARRAY_ACTIVE_SIZE (insn_allocno_varray),
	     insn, curr_allocation_alt);
	  put_duplication_before_original ();
	  find_best_alt_allocation ();
	}
    }
  return ! min_insn_cost_undef_p;
}

/* The function assigns hard register or memory to allocno A.  The
   hard register or memory is taken from allocnos connected to A.  The
   function returns true in the case of success, otherwise it returns
   false.  */
static bool
assign_pseudo_allocno_from_connected_allocno (allocno_t a)
{
  int hard_regno;
  allocno_t connected_a;
  copy_t cp;

  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
    {
      connected_a = COPY_SRC (cp);
      yara_assert (connected_a != NULL);
      if ((hard_regno = ALLOCNO_HARD_REGNO (connected_a)) >= 0)
	{
	  hard_regno = get_allocno_hard_regno (a, hard_regno);
	  if (assign_allocno (a, ALL_REGS, reg_class_contents [ALL_REGS],
			      hard_regno))
	    return true;
	}
      else if (ALLOCNO_MEMORY_SLOT (connected_a) != NULL)
	{
	  if (assign_allocno (a, NO_REGS, reg_class_contents [NO_REGS], -1))
	    return true;
	}
    }
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    {
      connected_a = COPY_DST (cp);
      if ((hard_regno = ALLOCNO_HARD_REGNO (connected_a)) >= 0)
	{
	  hard_regno = get_allocno_hard_regno (a, hard_regno);
	  if (assign_allocno (a, ALL_REGS, reg_class_contents [ALL_REGS],
			      hard_regno))
	    return true;
	}
      else if (ALLOCNO_MEMORY_SLOT (connected_a) != NULL)
	{
	  if (assign_allocno (a, NO_REGS, reg_class_contents [NO_REGS], -1))
	    return true;
	}
    }
  return false;
}

/* The function assigns allocnos of INSN without actual copying.  It
   means that pseudo register allocno gets the same hard register or
   memory as the connected allocno has, non-pseudo register allocnos
   will stay in the insn without change.  The function returns true in
   the case of success, otherwise it returns false.  */
static bool
assign_insn_allocnos_without_copy (rtx insn)
{
  int regno;
  allocno_t a;
  alt_set_t possible_alts;

  start_transaction ();
  SET_ALT_SET (possible_alts);
  for (a = insn_allocnos [INSN_UID (insn)];
       a != NULL;
       a = INSN_ALLOCNO_NEXT (a))
    {
      if ((regno = ALLOCNO_REGNO (a)) >= 0 && ! HARD_REGISTER_NUM_P (regno))
	{
	  /* If connected allocnos use equiv. constants we try them in
	     find_best_allocation.  */
	  if (! assign_pseudo_allocno_from_connected_allocno (a))
	    break;
	  if (INSN_ALLOCNO_TYPE (a) == BASE_REG
	      && (ALLOCNO_HARD_REGNO (a) < 0
		  || ! hard_reg_in_set_p
                       (ALLOCNO_HARD_REGNO (a), ALLOCNO_MODE (a),
			base_regs [GET_MODE
				   (*INSN_ALLOCNO_CONTAINER_LOC (a))])))
	    break;
	  if (INSN_ALLOCNO_TYPE (a) == INDEX_REG
	      && (ALLOCNO_HARD_REGNO (a) < 0
		  || ! hard_reg_in_set_p (ALLOCNO_HARD_REGNO (a),
					  ALLOCNO_MODE (a), index_regs)))
	    break;
	}
      else if (! assign_allocno (a, LIM_REG_CLASSES,
				 reg_class_contents [NO_REGS], -1))
	/* We assume that if hard register is used as a base or index
	   register it already has a right class.  */
	break;
      if (INSN_ALLOCNO_TYPE (a) >= OPERAND_BASE)
	{
	  setup_possible_allocno_alternatives
	    (curr_allocation_insn_info, a, true);
	  AND_ALT_SET (possible_alts, INSN_ALLOCNO_POSSIBLE_ALTS (a));
	  if (EQ_ALT_SET (possible_alts, ZERO_ALT_SET))
	    break;
	}
    }
  if (a != NULL)
    {
      undo_transaction ();
      return false;
    }
  end_transaction ();
  return true;
}

/* Function makes assignment for all allocnos of INSN.  It may spill
   allocnos living through the insn by calling hook
   PROVIDE_ALLOCNO_CLASS_HARD_REG.  It also may use hook
   CALL_CROSS_HINT to define that given allocno lives through a call.
   The insn allocnos are processed in order provided by hook
   INSN_ALLOCNO_SORT (e.g. to process allocnos with bigger constraints
   first).  */
void
allocate_insn_allocnos (rtx insn, bool (*call_cross_hint) (allocno_t),
			void (*insn_allocno_sort) (allocno_t *, int, rtx, int),
			int (*provide_allocno_class_hard_reg) (allocno_t,
							       enum reg_class))
{
  int i, op_num;
  allocno_t a;

  curr_allocation_insn_info = insn_infos [INSN_UID (insn)];
  if (assign_insn_allocnos_without_copy (insn))
    return;
  call_cross_hint_func = call_cross_hint;
  insn_allocno_sort_func = insn_allocno_sort;
  provide_allocno_class_hard_reg_func = provide_allocno_class_hard_reg;
  VARRAY_POP_ALL (insn_allocno_varray);
  VARRAY_POP_ALL (best_insn_allocno_varray);
  best_insn_exchange_allocno = NULL;
  if (! find_best_allocation (insn))
    gcc_unreachable ();
  if (best_insn_exchange_allocno != NULL)
    {
      make_commutative_exchange (best_insn_exchange_allocno);
      setup_possible_allocno_alternatives
	(curr_allocation_insn_info, best_insn_exchange_allocno, false);
      setup_possible_allocno_alternatives
	(curr_allocation_insn_info,
	 INSN_ALLOCNO_COMMUTATIVE (best_insn_exchange_allocno), false);
    }
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

	  yara_assert (REG_P (reg)
		       && (INSN_ALLOCNO_OP_MODE (a) == OP_IN
			   || INSN_ALLOCNO_OP_MODE (a) == OP_INOUT)
		       /* We assume no base and index registers in
			  subregisters.  */
		       && GET_CODE (*INSN_ALLOCNO_CONTAINER_LOC (a))
		          != SUBREG);
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
	  else
	    {
	      int to;
	      bool base_p;
	      HOST_WIDE_INT offset;
	      bool cont_p = true;
	      rtx x, *address_loc, *container_loc;
	      
	      if ((x = reg_equiv_constant [ALLOCNO_REGNO (a)]) != NULL_RTX
		  && ! CONSTANT_P (x))
		{
		  get_equiv_const_addr_info (x, &to, &offset);
		  container_loc = INSN_ALLOCNO_CONTAINER_LOC (a);
		  address_loc = (GET_CODE (*container_loc) == MEM
				 ? &XEXP (*container_loc, 0) : container_loc);
		  /* ??? Allocate interm equiv constant register.  Do
		     we need this?  */
		  if (check_elimination_in_addr (ALLOCNO_REGNO (a), to, offset,
						 address_loc, container_loc,
						 &base_p) != NULL
		      && assign_allocno (a, LIM_REG_CLASSES,
					 reg_class_contents [NO_REGS], -1))
		    cont_p = false;
		}
	      if (cont_p
		  && ! assign_hard_regno_to_pseudo_reg_insn_allocno (a, cl,
								     temp_set))
		gcc_unreachable ();
	    }
	}
      else
	{
	  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
	  if (! assign_constraint (a, best_insn_constraints [op_num]))
	    gcc_unreachable ();
	}
    }
}

void
yara_insn_init (void)
{
  VARRAY_INT_INIT (op_try_varray, MAX_RECOG_OPERANDS,
		   "op constraint try stack");
  VARRAY_GENERIC_PTR_NOGC_INIT (insn_allocno_varray, MAX_RECOG_OPERANDS * 4,
				"insn allocnos");
  VARRAY_GENERIC_PTR_NOGC_INIT (best_insn_allocno_varray,
				MAX_RECOG_OPERANDS * 4,
				"insn allocnos in best order");
}

void
yara_insn_finish (void)
{
  VARRAY_FREE (best_insn_allocno_varray);
  VARRAY_FREE (insn_allocno_varray);
}
