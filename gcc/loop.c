/* Remnant of the old glory of the loop optimizer.  Some condition manipulation
   functions.
   Copyright (C) 1987, 1988, 1989, 1991, 1992, 1993, 1994, 1995, 1996, 1997,
   1998, 1999, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "expr.h"
#include "insn-config.h"


/* Given an insn INSN and condition COND, return the condition in a
   canonical form to simplify testing by callers.  Specifically:

   (1) The code will always be a comparison operation (EQ, NE, GT, etc.).
   (2) Both operands will be machine operands; (cc0) will have been replaced.
   (3) If an operand is a constant, it will be the second operand.
   (4) (LE x const) will be replaced with (LT x <const+1>) and similarly
       for GE, GEU, and LEU.

   If the condition cannot be understood, or is an inequality floating-point
   comparison which needs to be reversed, 0 will be returned.

   If REVERSE is nonzero, then reverse the condition prior to canonizing it.

   If EARLIEST is nonzero, it is a pointer to a place where the earliest
   insn used in locating the condition was found.  If a replacement test
   of the condition is desired, it should be placed in front of that
   insn and we will be sure that the inputs are still valid.

   If WANT_REG is nonzero, we wish the condition to be relative to that
   register, if possible.  Therefore, do not canonicalize the condition
   further.  */

rtx
canonicalize_condition (insn, cond, reverse, earliest, want_reg)
     rtx insn;
     rtx cond;
     int reverse;
     rtx *earliest;
     rtx want_reg;
{
  enum rtx_code code;
  rtx prev = insn;
  rtx set;
  rtx tem;
  rtx op0, op1;
  int reverse_code = 0;
  enum machine_mode mode;

  code = GET_CODE (cond);
  mode = GET_MODE (cond);
  op0 = XEXP (cond, 0);
  op1 = XEXP (cond, 1);

  if (reverse)
    code = reversed_comparison_code (cond, insn);
  if (code == UNKNOWN)
    return 0;

  if (earliest)
    *earliest = insn;

  /* If we are comparing a register with zero, see if the register is set
     in the previous insn to a COMPARE or a comparison operation.  Perform
     the same tests as a function of STORE_FLAG_VALUE as find_comparison_args
     in cse.c  */

  while (GET_RTX_CLASS (code) == '<'
	 && op1 == CONST0_RTX (GET_MODE (op0))
	 && op0 != want_reg)
    {
      /* Set nonzero when we find something of interest.  */
      rtx x = 0;

#ifdef HAVE_cc0
      /* If comparison with cc0, import actual comparison from compare
	 insn.  */
      if (op0 == cc0_rtx)
	{
	  if ((prev = prev_nonnote_insn (prev)) == 0
	      || GET_CODE (prev) != INSN
	      || (set = single_set (prev)) == 0
	      || SET_DEST (set) != cc0_rtx)
	    return 0;

	  op0 = SET_SRC (set);
	  op1 = CONST0_RTX (GET_MODE (op0));
	  if (earliest)
	    *earliest = prev;
	}
#endif

      /* If this is a COMPARE, pick up the two things being compared.  */
      if (GET_CODE (op0) == COMPARE)
	{
	  op1 = XEXP (op0, 1);
	  op0 = XEXP (op0, 0);
	  continue;
	}
      else if (GET_CODE (op0) != REG)
	break;

      /* Go back to the previous insn.  Stop if it is not an INSN.  We also
	 stop if it isn't a single set or if it has a REG_INC note because
	 we don't want to bother dealing with it.  */

      if ((prev = prev_nonnote_insn (prev)) == 0
	  || GET_CODE (prev) != INSN
	  || FIND_REG_INC_NOTE (prev, NULL_RTX))
	break;

      set = set_of (op0, prev);

      if (set
	  && (GET_CODE (set) != SET
	      || !rtx_equal_p (SET_DEST (set), op0)))
	break;

      /* If this is setting OP0, get what it sets it to if it looks
	 relevant.  */
      if (set)
	{
	  enum machine_mode inner_mode = GET_MODE (SET_DEST (set));
#ifdef FLOAT_STORE_FLAG_VALUE
	  REAL_VALUE_TYPE fsfv;
#endif

	  /* ??? We may not combine comparisons done in a CCmode with
	     comparisons not done in a CCmode.  This is to aid targets
	     like Alpha that have an IEEE compliant EQ instruction, and
	     a non-IEEE compliant BEQ instruction.  The use of CCmode is
	     actually artificial, simply to prevent the combination, but
	     should not affect other platforms.

	     However, we must allow VOIDmode comparisons to match either
	     CCmode or non-CCmode comparison, because some ports have
	     modeless comparisons inside branch patterns.

	     ??? This mode check should perhaps look more like the mode check
	     in simplify_comparison in combine.  */

	  if ((GET_CODE (SET_SRC (set)) == COMPARE
	       || (((code == NE
		     || (code == LT
			 && GET_MODE_CLASS (inner_mode) == MODE_INT
			 && (GET_MODE_BITSIZE (inner_mode)
			     <= HOST_BITS_PER_WIDE_INT)
			 && (STORE_FLAG_VALUE
			     & ((HOST_WIDE_INT) 1
				<< (GET_MODE_BITSIZE (inner_mode) - 1))))
#ifdef FLOAT_STORE_FLAG_VALUE
		     || (code == LT
			 && GET_MODE_CLASS (inner_mode) == MODE_FLOAT
			 && (fsfv = FLOAT_STORE_FLAG_VALUE (inner_mode),
			     REAL_VALUE_NEGATIVE (fsfv)))
#endif
		     ))
		   && GET_RTX_CLASS (GET_CODE (SET_SRC (set))) == '<'))
	      && (((GET_MODE_CLASS (mode) == MODE_CC)
		   == (GET_MODE_CLASS (inner_mode) == MODE_CC))
		  || mode == VOIDmode || inner_mode == VOIDmode))
	    x = SET_SRC (set);
	  else if (((code == EQ
		     || (code == GE
			 && (GET_MODE_BITSIZE (inner_mode)
			     <= HOST_BITS_PER_WIDE_INT)
			 && GET_MODE_CLASS (inner_mode) == MODE_INT
			 && (STORE_FLAG_VALUE
			     & ((HOST_WIDE_INT) 1
				<< (GET_MODE_BITSIZE (inner_mode) - 1))))
#ifdef FLOAT_STORE_FLAG_VALUE
		     || (code == GE
			 && GET_MODE_CLASS (inner_mode) == MODE_FLOAT
			 && (fsfv = FLOAT_STORE_FLAG_VALUE (inner_mode),
			     REAL_VALUE_NEGATIVE (fsfv)))
#endif
		     ))
		   && GET_RTX_CLASS (GET_CODE (SET_SRC (set))) == '<'
		   && (((GET_MODE_CLASS (mode) == MODE_CC)
			== (GET_MODE_CLASS (inner_mode) == MODE_CC))
		       || mode == VOIDmode || inner_mode == VOIDmode))

	    {
	      reverse_code = 1;
	      x = SET_SRC (set);
	    }
	  else
	    break;
	}

      else if (reg_set_p (op0, prev))
	/* If this sets OP0, but not directly, we have to give up.  */
	break;

      if (x)
	{
	  if (GET_RTX_CLASS (GET_CODE (x)) == '<')
	    code = GET_CODE (x);
	  if (reverse_code)
	    {
	      code = reversed_comparison_code (x, prev);
	      if (code == UNKNOWN)
		return 0;
	      reverse_code = 0;
	    }

	  op0 = XEXP (x, 0), op1 = XEXP (x, 1);
	  if (earliest)
	    *earliest = prev;
	}
    }

  /* If constant is first, put it last.  */
  if (CONSTANT_P (op0))
    code = swap_condition (code), tem = op0, op0 = op1, op1 = tem;

  /* If OP0 is the result of a comparison, we weren't able to find what
     was really being compared, so fail.  */
  if (GET_MODE_CLASS (GET_MODE (op0)) == MODE_CC)
    return 0;

  /* Canonicalize any ordered comparison with integers involving equality
     if we can do computations in the relevant mode and we do not
     overflow.  */

  if (GET_CODE (op1) == CONST_INT
      && GET_MODE (op0) != VOIDmode
      && GET_MODE_BITSIZE (GET_MODE (op0)) <= HOST_BITS_PER_WIDE_INT)
    {
      HOST_WIDE_INT const_val = INTVAL (op1);
      unsigned HOST_WIDE_INT uconst_val = const_val;
      unsigned HOST_WIDE_INT max_val
	= (unsigned HOST_WIDE_INT) GET_MODE_MASK (GET_MODE (op0));

      switch (code)
	{
	case LE:
	  if ((unsigned HOST_WIDE_INT) const_val != max_val >> 1)
	    code = LT, op1 = gen_int_mode (const_val + 1, GET_MODE (op0));
	  break;

	/* When cross-compiling, const_val might be sign-extended from
	   BITS_PER_WORD to HOST_BITS_PER_WIDE_INT */
	case GE:
	  if ((HOST_WIDE_INT) (const_val & max_val)
	      != (((HOST_WIDE_INT) 1
		   << (GET_MODE_BITSIZE (GET_MODE (op0)) - 1))))
	    code = GT, op1 = gen_int_mode (const_val - 1, GET_MODE (op0));
	  break;

	case LEU:
	  if (uconst_val < max_val)
	    code = LTU, op1 = gen_int_mode (uconst_val + 1, GET_MODE (op0));
	  break;

	case GEU:
	  if (uconst_val != 0)
	    code = GTU, op1 = gen_int_mode (uconst_val - 1, GET_MODE (op0));
	  break;

	default:
	  break;
	}
    }

  /* Never return CC0; return zero instead.  */
  if (CC0_P (op0))
    return 0;

  return gen_rtx_fmt_ee (code, VOIDmode, op0, op1);
}

/* Given a jump insn JUMP, return the condition that will cause it to branch
   to its JUMP_LABEL.  If the condition cannot be understood, or is an
   inequality floating-point comparison which needs to be reversed, 0 will
   be returned.

   If EARLIEST is nonzero, it is a pointer to a place where the earliest
   insn used in locating the condition was found.  If a replacement test
   of the condition is desired, it should be placed in front of that
   insn and we will be sure that the inputs are still valid.  */

rtx
get_condition (jump, earliest)
     rtx jump;
     rtx *earliest;
{
  rtx cond;
  int reverse;
  rtx set;

  /* If this is not a standard conditional jump, we can't parse it.  */
  if (GET_CODE (jump) != JUMP_INSN
      || ! any_condjump_p (jump))
    return 0;
  set = pc_set (jump);

  cond = XEXP (SET_SRC (set), 0);

  /* If this branches to JUMP_LABEL when the condition is false, reverse
     the condition.  */
  reverse
    = GET_CODE (XEXP (SET_SRC (set), 2)) == LABEL_REF
      && XEXP (XEXP (SET_SRC (set), 2), 0) == JUMP_LABEL (jump);

  return canonicalize_condition (jump, cond, reverse, earliest, NULL_RTX);
}
