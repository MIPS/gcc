/* Code for RTL transformations to satisfy insn constraints.
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


/* This code selects alternatives for insns based on register
   allocation.  It has also a mode to do some simple code
   transformations.  */

#undef REG_OK_STRICT

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "hard-reg-set.h"
#include "rtl.h"
#include "tm_p.h"
#include "regs.h"
#include "insn-config.h"
#include "insn-codes.h"
#include "recog.h"
#include "output.h"
#include "addresses.h"
#include "target.h"
#include "function.h"
#include "expr.h"
#include "basic-block.h"
#include "except.h"
#include "optabs.h"
#include "df.h"
#include "ira.h"
#include "rtl-error.h"
#include "lra-int.h"

/* Value of LRA_CURR_RELOAD_NUM at the beginning of BB of the current
   insn.  Remember that LRA_CURR_RELOAD_NUM is the number of emitted
   reload insns.  */
static int bb_reload_num;

/* Array whose element is (MEM:MODE BASE_REG) corresponding to the
   mode (index) and where BASE_REG is a base hard register for given
   memory mode.  */
static rtx indirect_mem[MAX_MACHINE_MODE];

/* Return indrect memory for MODE.  */
static rtx
get_indirect_mem (enum machine_mode mode)
{
  if (indirect_mem[mode] == NULL_RTX)
    {
      enum reg_class rclass = base_reg_class (mode, ADDR_SPACE_GENERIC,
					      MEM, SCRATCH);

      indirect_mem[mode]
	 = gen_rtx_MEM (mode, regno_reg_rtx [ira_class_hard_regs[rclass][0]]);
    }
  return indirect_mem[mode];
}

/* Initiate INDIECT_MEM.  */
static void
init_indirect_mem (void)
{
  int i;

  for (i = 0; i < MAX_MACHINE_MODE; i++)
    indirect_mem[i] = NULL_RTX;
}



/* The current insn being processed and corresponding its data (basic
   block, the insn data, and the insn static data.  */
static rtx curr_insn;
static basic_block curr_bb;
static lra_insn_recog_data_t curr_id;
static struct lra_static_insn_data *curr_static_id;



/* Start numbers for new registers and insns at the current constraints
   pass start.  */
static int new_regno_start;
static int new_insn_uid_start;

/* Return hard regno of REGNO or if it is was not assigned to a hard
   register, use a hard register from its allocno class.  */
static int
get_try_hard_regno (int regno)
{
  int hard_regno;
  enum reg_class rclass;

  if ((hard_regno = regno) >= FIRST_PSEUDO_REGISTER)
    hard_regno = lra_get_regno_hard_regno (regno);
  if (hard_regno >= 0)
    return hard_regno;
  rclass = lra_get_allocno_class (regno);
  if (rclass == NO_REGS)
    return -1;
  return ira_class_hard_regs[rclass][0];
}

/* Return class of hard regno of REGNO or if it is was not assigned to
   a hard register, return its allocno class but only for
   non-inherited pseudos created on the current constraint pass.
   Otherwise, return NO_REGS.  */
static enum reg_class
get_reg_class (int regno)
{
  int hard_regno;

  if ((hard_regno = regno) >= FIRST_PSEUDO_REGISTER)
    hard_regno = lra_get_regno_hard_regno (regno);
  if (hard_regno >= 0)
    return REGNO_REG_CLASS (hard_regno);
  if (regno >= new_regno_start)
    return lra_get_allocno_class (regno);
  return NO_REGS;
}

/* Return true if REGNO satisfies reg class constraint CL.  For new
   non-inherited pseudos we should make more accurate class *NEW_CLASS
   (we set up it if it is not NULL) to satisfy the constraints.
   Otherwise, set up NEW_CLASS to NO_REGS.  */
static bool
in_class_p (int regno, enum reg_class cl, enum reg_class *new_class)
{
  enum reg_class rclass, common_class;

  if (new_class != NULL)
    *new_class = NO_REGS;
  if (regno < FIRST_PSEUDO_REGISTER)
    return TEST_HARD_REG_BIT (reg_class_contents[cl], regno);
  rclass = get_reg_class (regno);
  if (regno < new_regno_start
      /* Do not make more accurate class from reloads generated.  They
	 are mostly moves with a lot of constraints.  Making more
	 accurate class may results in very narrow class and
	 impossibility of find registers for several reloads of one
	 insn.  */
      || INSN_UID (curr_insn) >= new_insn_uid_start)
    return (rclass != NO_REGS && ira_class_subset_p[rclass][cl]
	     && ! hard_reg_set_subset_p (reg_class_contents[cl],
					 lra_no_alloc_regs));
  else
    {
      common_class = ira_reg_class_subset[rclass][cl];
      if (new_class != NULL)
	*new_class = common_class;
      return ! hard_reg_set_subset_p (reg_class_contents[common_class],
				      lra_no_alloc_regs);
    }
}

/* Return true if REGNO satisfies a memory constraint.  */
static bool
in_mem_p (int regno)
{
  return get_reg_class (regno) == NO_REGS;
}

/* Return the defined and profitable equiv substition of reg X, return
   X otherwise.  */
static rtx
get_equiv_substitution (rtx x)
{
  int regno;
  rtx res;

  if (! REG_P (x) || (regno = REGNO (x)) < FIRST_PSEUDO_REGISTER
      || ! ira_reg_equiv[regno].defined_p
      || ! ira_reg_equiv[regno].profitable_p
      || lra_get_regno_hard_regno (regno) >= 0)
    return x;
  if ((res = ira_reg_equiv[regno].memory) != NULL_RTX)
    return res;
  if ((res = ira_reg_equiv[regno].constant) != NULL_RTX)
    return res;
  if ((res = ira_reg_equiv[regno].invariant) != NULL_RTX)
    return res;
  gcc_unreachable ();
}



/* The page contains code to reuse input reloads.  */

/* Structure describes input reload of the current insns.  */
struct input_reload
{
  /* Reloaded value.  */
  rtx input;
  /* Reload pseudo used.  */
  rtx reg;
};

/* The number of elements in the following array.  */
static int curr_insn_input_reloads_num;
/* Array containing info about input reloads.  It is used to find the
   same input reload and reuse the reload pseudo in this case.  */
static struct input_reload curr_insn_input_reloads[LRA_MAX_INSN_RELOADS];

/* Initiate data concerning reusage of input reloads for the current
   insn.  */
static void
init_curr_insn_input_reloads (void)
{
  curr_insn_input_reloads_num = 0;
}

/* Change class of pseudo REGNO to NEW_CLASS.  Print info about it
   using TITLE.  Output a new line if NL_P.  */
static void
change_class (int regno, enum reg_class new_class,
	      const char *title, bool nl_p)
{
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file, "%s to class %s for r%d",
	     title, reg_class_names[new_class], regno);
  setup_reg_classes (regno, new_class, NO_REGS, new_class);
  if (lra_dump_file != NULL && nl_p)
    fprintf (lra_dump_file, "\n");
}

/* Create a new pseudo using MODE, RCLASS, ORIGINAL, TITLE or reuse
   already created input reload pseudo (only if TYPE is not OP_OUT).
   The result pseudo is returned through RESULT_REG.  Return TRUE if
   we created a new pseudo, FALSE if we reused the already created
   input reload pseudo.  */
static bool
get_reload_reg (enum op_type type, enum machine_mode mode, rtx original,
		enum reg_class rclass, const char *title, rtx *result_reg)
{
  bool res_p;
  int i, regno;
  enum reg_class new_class;

  if (type == OP_OUT)
    {
      *result_reg
	= lra_create_new_reg_with_unique_value (mode, original, rclass, title);
      return true;
    }
  for (i = 0; i < curr_insn_input_reloads_num; i++)
    if (rtx_equal_p (curr_insn_input_reloads[i].input, original))
      break;
  if (i >= curr_insn_input_reloads_num
      || ! in_class_p (REGNO (curr_insn_input_reloads[i].reg),
			rclass, &new_class))
    {
      res_p = true;
      *result_reg = lra_create_new_reg (mode, original, rclass, title);
    }
  else
    {
      gcc_assert (! side_effects_p (original));
      res_p = false;
      *result_reg = curr_insn_input_reloads[i].reg;
      regno = REGNO (*result_reg);
      if (lra_dump_file != NULL)
	 {
	   fprintf (lra_dump_file, "     Reuse r%d for reload ", regno);
	   print_value_slim (lra_dump_file, original, 1);
	 }
      if (rclass != new_class)
	 change_class (regno, new_class, ", change", false);
      if (lra_dump_file != NULL)
	 fprintf (lra_dump_file, "\n");
    }
  gcc_assert (curr_insn_input_reloads_num < LRA_MAX_INSN_RELOADS);
  curr_insn_input_reloads[curr_insn_input_reloads_num].input = original;
  curr_insn_input_reloads[curr_insn_input_reloads_num++].reg = *result_reg;
  return res_p;
}



/* The page contains code to extract memory address parts.  */

/* Info about base and index regs of an address.  */
struct address
{
  rtx *base_reg_loc;  /* NULL if there is no a base register.  */
  rtx *base_reg_loc2; /* Second location of {post/pre}_modify, NULL
			 otherwise. */
  rtx *index_reg_loc; /* NULL if there is no an index register.  */
  rtx *index_loc; /* location of index reg * scale or index_reg_loc
		      otherwise. */
  rtx *disp_loc; /* NULL if there is no a displacement.  */
  /* Defined if base_reg_loc is not NULL.  */
  enum rtx_code base_outer_code, index_code;
  /* True if the base register is modified in the address, for
     example, in PRE_INC.  */
  bool base_modify_p;
};

/* Wrapper around REGNO_OK_FOR_INDEX_P, to allow pseudo registers.  */
static inline bool
ok_for_index_p_nonstrict (rtx reg)
{
  unsigned regno = REGNO (reg);
  
  return regno >= FIRST_PSEUDO_REGISTER || REGNO_OK_FOR_INDEX_P (regno);
}

/* A version of regno_ok_for_base_p for use here, when all
   pseudo-registers should count as OK.  Arguments as for
   regno_ok_for_base_p.  */
static inline bool
ok_for_base_p_nonstrict (rtx reg, enum machine_mode mode, addr_space_t as,
			 enum rtx_code outer_code, enum rtx_code index_code)
{
  unsigned regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER)
    return true;
  return ok_for_base_p_1 (regno, mode, as, outer_code, index_code);
}

/* Process address part in space AS (or all address if TOP_P) with
   location *LOC to extract address characteristics.

   If CONTEXT_P is false, we are looking at the base part of an
   address, otherwise we are looking at the index part.

   MODE is the mode of the memory reference; OUTER_CODE and INDEX_CODE
   give the context that the rtx appears in; MODIFY_P if *LOC is
   modified.  */
static void
extract_loc_address_regs (bool top_p, enum machine_mode mode, addr_space_t as,
			  rtx *loc, bool context_p, enum rtx_code outer_code,
			  enum rtx_code index_code,
			  bool modify_p, struct address *ad)
{
  rtx x = *loc;
  enum rtx_code code = GET_CODE (x);
  bool base_ok_p;

  switch (code)
    {
    case CONST_INT:
    case CONST:
    case SYMBOL_REF:
    case LABEL_REF:
      if (! context_p)
	ad->disp_loc = loc;
      return;

    case CC0:
    case PC:
      return;

    case PLUS:
      /* When we have an address that is a sum, we must determine
	 whether registers are "base" or "index" regs.  If there is a
	 sum of two registers, we must choose one to be the
	 "base".  */
      {
	rtx *arg0_loc = &XEXP (x, 0);
	rtx *arg1_loc = &XEXP (x, 1);
	rtx arg0 = *arg0_loc;
	rtx arg1 = *arg1_loc;
	enum rtx_code code0 = GET_CODE (arg0);
	enum rtx_code code1 = GET_CODE (arg1);

	/* Look inside subregs.  */
	if (code0 == SUBREG)
	  {
	    arg0_loc = &SUBREG_REG (arg0);
	    arg0 = *arg0_loc;
	    code0 = GET_CODE (arg0);
	  }
	if (code1 == SUBREG)
	  {
	    arg1_loc = &SUBREG_REG (arg1);
	    arg1 = *arg1_loc;
	    code1 = GET_CODE (arg1);
	  }

	/* If this machine only allows one register per address, it
	   must be in the first operand.  */
	if (MAX_REGS_PER_ADDRESS == 1)
	  {
	    extract_loc_address_regs (false, mode, as, arg0_loc, false, PLUS,
				      code1, modify_p, ad);
	    gcc_assert (CONSTANT_P (arg1)); /* It should be a displacement.  */
	    ad->disp_loc = arg1_loc;
	  }
	/* If index and base registers are the same on this machine,
	   just record registers in any non-constant operands.  We
	   assume here, as well as in the tests below, that all
	   addresses are in canonical form.  */
	else if (INDEX_REG_CLASS
		 == base_reg_class (VOIDmode, as, PLUS, SCRATCH))
	  {
	    extract_loc_address_regs (false, mode, as, arg0_loc, false, PLUS,
				      code1, modify_p, ad);
	    if (! CONSTANT_P (arg1))
	      extract_loc_address_regs (false, mode, as, arg1_loc, true, PLUS,
					code0, modify_p, ad);
	    else
	      ad->disp_loc = arg1_loc;
	  }

	/* If the second operand is a constant integer, it doesn't
	   change what class the first operand must be.  */
	else if (code1 == CONST_INT || code1 == CONST_DOUBLE)
	  {
	    extract_loc_address_regs (false, mode, as, arg0_loc, context_p,
				      PLUS, code1, modify_p, ad);
	    ad->disp_loc = arg1_loc;
	  }
	/* If the second operand is a symbolic constant, the first
	   operand must be an index register but only if this part is
	   all the address.  */
	else if (code1 == SYMBOL_REF || code1 == CONST || code1 == LABEL_REF)
	  {
	    extract_loc_address_regs (false, mode, as, arg0_loc,
				      top_p ? true : context_p, PLUS, code1,
				      modify_p, ad);
	    ad->disp_loc = arg1_loc;
	  }
	/* If both operands are registers but one is already a hard
	   register of index or reg-base class, give the other the
	   class that the hard register is not.  */
	else if (code0 == REG && code1 == REG
		 && REGNO (arg0) < FIRST_PSEUDO_REGISTER
		 && ((base_ok_p
		      = ok_for_base_p_nonstrict (arg0, mode, as, PLUS, REG))
		     || ok_for_index_p_nonstrict (arg0)))
	  {
	    extract_loc_address_regs (false, mode, as, arg0_loc, ! base_ok_p,
				      PLUS, REG, modify_p, ad);
	    extract_loc_address_regs (false, mode, as, arg1_loc, base_ok_p,
				      PLUS, REG, modify_p, ad);
	  }
	else if (code0 == REG && code1 == REG
		 && REGNO (arg1) < FIRST_PSEUDO_REGISTER
		 && ((base_ok_p
		      = ok_for_base_p_nonstrict (arg1, mode, as, PLUS, REG))
		     || ok_for_index_p_nonstrict (arg1)))
	  {
	    extract_loc_address_regs (false, mode, as, arg0_loc, base_ok_p,
				      PLUS, REG, modify_p, ad);
	    extract_loc_address_regs (false, mode, as, arg1_loc, ! base_ok_p,
				      PLUS, REG, modify_p, ad);
	  }
	/* If one operand is known to be a pointer, it must be the
	   base with the other operand the index.  Likewise if the
	   other operand is a MULT.  */
	else if ((code0 == REG && REG_POINTER (arg0)) || code1 == MULT)
	  {
	    extract_loc_address_regs (false, mode, as, arg0_loc, false, PLUS,
				      code1, modify_p, ad);
	    if (code1 == MULT)
	      ad->index_loc = arg1_loc;
	    extract_loc_address_regs (false, mode, as, arg1_loc, true, PLUS,
				      code0, modify_p, ad);
	  }
	else if ((code1 == REG && REG_POINTER (arg1)) || code0 == MULT)
	  {
	    extract_loc_address_regs (false, mode, as, arg0_loc, true, PLUS,
				      code1, modify_p, ad);
	    if (code0 == MULT)
	      ad->index_loc = arg0_loc;
	    extract_loc_address_regs (false, mode, as, arg1_loc, false, PLUS,
				      code0, modify_p, ad);
	  }
	/* Otherwise, count equal chances that each might be a base or
	   index register.  This case should be rare.  */
	else
	  {
	    extract_loc_address_regs (false, mode, as, arg0_loc, false, PLUS,
				      code1, modify_p, ad);
	    extract_loc_address_regs (false, mode, as, arg1_loc, true, PLUS,
				      code0, modify_p, ad);
	  }
      }
      break;

      /* Double the importance of a pseudo that is incremented or
	 decremented, since it would take two extra insns if it ends
	 up in the wrong place.  */
    case POST_MODIFY:
    case PRE_MODIFY:
      extract_loc_address_regs (false, mode, as, &XEXP (x, 0), false,
				code, GET_CODE (XEXP (XEXP (x, 1), 1)),
				true, ad);
      gcc_assert (rtx_equal_p (XEXP (XEXP (x, 1), 0), XEXP (x, 0)));
      ad->base_reg_loc2 = &XEXP (XEXP (x, 1), 0);
      if (REG_P (XEXP (XEXP (x, 1), 1)))
	extract_loc_address_regs (false, mode, as, &XEXP (XEXP (x, 1), 1),
				  true, code, REG, modify_p, ad);
      break;

    case POST_INC:
    case PRE_INC:
    case POST_DEC:
    case PRE_DEC:
      extract_loc_address_regs (false, mode, as, &XEXP (x, 0), false, code,
				SCRATCH, true, ad);
      break;

    case REG:
      if (context_p)
	ad->index_reg_loc = loc;
      else
	{
	  ad->base_reg_loc = loc;
	  ad->base_outer_code = outer_code;
	  ad->index_code = index_code;
	  ad->base_modify_p = modify_p;
	}
      break;

    default:
      {
	const char *fmt = GET_RTX_FORMAT (code);
	int i;

	for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
	  if (fmt[i] == 'e')
	    extract_loc_address_regs (false, mode, as, &XEXP (x, i), context_p,
				      code, SCRATCH, modify_p, ad);
      }
    }
}


/* Extract address characteristics in address with location *LOC in
   space AS.  Return them in AD.  Parameter OUTER_CODE for MEM should
   be MEM.  Parameter OUTER_CODE for 'p' constraint should be ADDRESS
   and MEM_MODE should be VOIDmode.  */
static void
extract_address_regs (enum machine_mode mem_mode, addr_space_t as,
		      rtx *loc, enum rtx_code outer_code, struct address *ad)
{
  ad->base_reg_loc = ad->base_reg_loc2
    = ad->index_reg_loc = ad->index_loc = ad->disp_loc = NULL;
  ad->base_outer_code = SCRATCH;
  ad->index_code = SCRATCH;
  ad->base_modify_p = false;
  extract_loc_address_regs (true, mem_mode, as, loc, false, outer_code,
			    SCRATCH, false, ad);  
  if (ad->index_loc == NULL)
    /* SUBREG ??? */
    ad->index_loc = ad->index_reg_loc;
}



/* The page constains major code to choose the current insn
   alternative and do reloads for it.  */

/* Return start register offset of hard register REGNO in MODE.  */
int
lra_constraint_offset (int regno, enum machine_mode mode)
{
  gcc_assert (regno < FIRST_PSEUDO_REGISTER);
  /* On a WORDS_BIG_ENDIAN machine, point to the last register of a
     multiple hard register group of scalar integer registers, so that
     for example (reg:DI 0) and (reg:SI 1) will be considered the same
     register.  */
  if (WORDS_BIG_ENDIAN && GET_MODE_SIZE (mode) > UNITS_PER_WORD
      && SCALAR_INT_MODE_P (mode))
    return hard_regno_nregs[regno][mode] - 1;
  return 0;
}

/* Like rtx_equal_p except that it allows a REG and a SUBREG to match
   if they are the same hard reg, and has special hacks for
   autoincrement and autodecrement.  This is specifically intended for
   process_alt_operands to use in determining whether two operands
   match.  X is the operand whose number is the lower of the two.

   It is supposed that X is the output operand and Y is the input
   operand.  */
static bool
operands_match_p (rtx x, rtx y, int y_hard_regno)
{
  int i, offset;
  RTX_CODE code = GET_CODE (x);
  const char *fmt;

  if (x == y)
    return true;
  if ((code == REG || (code == SUBREG && REG_P (SUBREG_REG (x))))
      && (REG_P (y) || (GET_CODE (y) == SUBREG && REG_P (SUBREG_REG (y)))))
    {
      int j;
      
      lra_get_hard_regno_and_offset (x, &i, &offset);
      if (i < 0)
	goto slow;
      i += offset;

      if ((j = y_hard_regno) < 0)
	goto slow;

      i += lra_constraint_offset (i, GET_MODE (x));
      j += lra_constraint_offset (j, GET_MODE (y));

      return i == j;
    }

  /* If two operands must match, because they are really a single
     operand of an assembler insn, then two postincrements are invalid
     because the assembler insn would increment only once.  On the
     other hand, a postincrement matches ordinary indexing if the
     postincrement is the output operand.  */
  if (code == POST_DEC || code == POST_INC || code == POST_MODIFY)
    return operands_match_p (XEXP (x, 0), y, y_hard_regno);

  /* Two preincrements are invalid because the assembler insn would
     increment only once.  On the other hand, a preincrement matches
     ordinary indexing if the preincrement is the input operand.  */
  if (GET_CODE (y) == PRE_DEC || GET_CODE (y) == PRE_INC
      || GET_CODE (y) == PRE_MODIFY)
    return operands_match_p (x, XEXP (y, 0), y_hard_regno);
  
 slow:

  if (code == REG && GET_CODE (y) == SUBREG && REG_P (SUBREG_REG (y))
      && x == SUBREG_REG (y))
    return true;
  if (GET_CODE (y) == REG && code == SUBREG && REG_P (SUBREG_REG (x))
      && SUBREG_REG (x) == y)
    return true;

  /* Now we have disposed of all the cases in which different rtx
     codes can match.  */
  if (code != GET_CODE (y))
    return false;

  /* (MULT:SI x y) and (MULT:HI x y) are NOT equivalent.  */
  if (GET_MODE (x) != GET_MODE (y))
    return false;

  switch (code)
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_FIXED:
      return false;

    case LABEL_REF:
      return XEXP (x, 0) == XEXP (y, 0);
    case SYMBOL_REF:
      return XSTR (x, 0) == XSTR (y, 0);

    default:
      break;
    }

  /* Compare the elements.  If any pair of corresponding elements fail
     to match, return false for the whole things.  */

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      int val, j;
      switch (fmt[i])
	{
	case 'w':
	  if (XWINT (x, i) != XWINT (y, i))
	    return false;
	  break;

	case 'i':
	  if (XINT (x, i) != XINT (y, i))
	    return false;
	  break;

	case 'e':
	  val = operands_match_p (XEXP (x, i), XEXP (y, i), y_hard_regno);
	  if (val == 0)
	    return false;
	  break;

	case '0':
	  break;

	case 'E':
	  if (XVECLEN (x, i) != XVECLEN (y, i))
	    return false;
	  for (j = XVECLEN (x, i) - 1; j >= 0; --j)
	    {
	      val = operands_match_p (XVECEXP (x, i, j), XVECEXP (y, i, j),
				      y_hard_regno);
	      if (val == 0)
		return false;
	    }
	  break;

	  /* It is believed that rtx's at this level will never
	     contain anything but integers and other rtx's, except for
	     within LABEL_REFs and SYMBOL_REFs.  */
	default:
	  gcc_unreachable ();
	}
    }
  return true;
}

/* Reload pseudos which created for input and output(or early
   clobber) reloads which should have the same hard register.  Such
   pseudos as input operands should be not inherited because the
   output rewrite the value in any away.  */
bitmap_head lra_matched_pseudos;

/* Reload pseudos created for matched input and output reloads whose
   mode are different.  Such pseudos has a modified rules for finding
   their living ranges, e.g. assigning to subreg of such pseudo means
   changing all pseudo value. */
bitmap_head lra_bound_pseudos;

/* True if X is a constant that can be forced into the constant pool.
   MODE is the mode of the operand, or VOIDmode if not known.  */
#define CONST_POOL_OK_P(MODE, X)		\
  ((MODE) != VOIDmode				\
   && CONSTANT_P (X)				\
   && GET_CODE (X) != HIGH			\
   && !targetm.cannot_force_const_mem (MODE, X))

/* True if C is a non-empty register class that has too few registers
   to be safely used as a reload target class.  */
#define SMALL_REGISTER_CLASS_P(C)					\
  (reg_class_size [(C)] == 1						\
   || (reg_class_size [(C)] >= 1 && targetm.class_likely_spilled_p (C)))

/* Return mode of WHAT inside of WHERE whose mode of the context is
   OUTER_MODE.  If WHERE does not contain WHAT, return VOIDmode.  */
static enum machine_mode
find_mode (rtx *where, enum machine_mode outer_mode, rtx *what)
{
  int i, j;
  enum machine_mode mode;
  rtx x;
  const char *fmt;
  enum rtx_code code;

  if (where == what)
    return outer_mode;
  if (*where == NULL_RTX)
    return VOIDmode;
  x = *where;
  code = GET_CODE (x);
  outer_mode = GET_MODE (x);
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  if ((mode = find_mode (&XEXP (x, i), outer_mode, what)) != VOIDmode)
	    return mode;
	}
      else if (fmt[i] == 'E')
	{
	  for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  if ((mode = find_mode (&XVECEXP (x, i, j), outer_mode, what))
	      != VOIDmode)
	    return mode;
	}
    }
  return VOIDmode;
}

/* Return mode for operand NOP of the current insn.  */
static inline enum machine_mode
get_op_mode (int nop)
{
  rtx *loc = curr_id->operand_loc[nop];
  enum machine_mode mode = GET_MODE (*loc);

  /* Take mode from the operand first.  */
  if (mode != VOIDmode)
    return mode;
  /* Take mode from the machine description second.  */
  if ((mode = curr_static_id->operand[nop].mode) != VOIDmode)
    return mode;
  /* Here is a very rare case.  Take mode from the context.  */
  return find_mode (&PATTERN (curr_insn), VOIDmode, loc);
}

/* If REG is a reload pseudo, try to make its class satisfying CL.  */
static void
narrow_reload_pseudo_class (rtx reg, enum reg_class cl)
{
  int regno;
  enum reg_class rclass;

  /* Do not make more accurate class from reloads generated.  They are
     mostly moves with a lot of constraints.  Making more accurate
     class may results in very narrow class and impossibility of find
     registers for several reloads of one insn.  */
  if (INSN_UID (curr_insn) >= new_insn_uid_start)
    return;
  if (GET_CODE (reg) == SUBREG)
    reg = SUBREG_REG (reg);
  if (! REG_P (reg) || (regno = REGNO (reg)) < new_regno_start)
    return;
  rclass = get_reg_class (regno);
  rclass = ira_reg_class_subset[rclass][cl];
  if (rclass == NO_REGS)
    return;
  change_class (regno, rclass, "      Change", true);
}

/* Generate reloads for matching OUT and INS (array of input operand
   numbers with end marker -1) with reg class GOAL_CLASS.  Add input
   and output reloads correspondingly to the lists *BEFORE and
   *AFTER.  */
static void
match_reload (signed char out, signed char *ins, enum reg_class goal_class,
	      rtx *before, rtx *after)
{
  int i, in;
  rtx new_in_reg, new_out_reg, reg;
  enum machine_mode inmode, outmode;
  rtx in_rtx = *curr_id->operand_loc[ins[0]];
  rtx out_rtx = *curr_id->operand_loc[out];

  outmode = get_op_mode (out);
  inmode = get_op_mode (ins[0]);
  if (inmode != outmode)
    {
      if (GET_MODE_SIZE (inmode) > GET_MODE_SIZE (outmode))
	{
	  reg = new_in_reg
	    = lra_create_new_reg_with_unique_value (inmode, in_rtx,
						    goal_class, "");
	  new_out_reg = gen_lowpart_SUBREG (outmode, reg);
	}
      else
	{
	  reg = new_out_reg
	    = lra_create_new_reg_with_unique_value (outmode, out_rtx,
						    goal_class, "");
	  new_in_reg = gen_lowpart_SUBREG (inmode, new_out_reg);
	}
      bitmap_set_bit (&lra_matched_pseudos, REGNO (reg));
      bitmap_set_bit (&lra_bound_pseudos, REGNO (reg));
    }
  else
    {
      /* We create pseudo for out rtx because we always should keep
	 registers with the same original regno have synchronized
	 value (it is not true for out register but it will be
	 corrected by the next insn).
	 
	 Do not reuse register because of the following situation: a <-
	 a op b, and b should be the same as a.
	 
	 Don't generate inheritance for the new register because we
	 can not use the same hard register for the corresponding
	 inheritance pseudo for input reload.  */
      new_in_reg = new_out_reg
	= lra_create_new_reg_with_unique_value (outmode, out_rtx,
						goal_class, "");
      bitmap_set_bit (&lra_matched_pseudos, REGNO (new_in_reg));
    }
  /* In and out operand can be got from transformations before
     processing constraints.  So the pseudos might have inaccurate
     class and we should make their classes more accurate.  */
  narrow_reload_pseudo_class (in_rtx, goal_class);
  narrow_reload_pseudo_class (out_rtx, goal_class);
  push_to_sequence (*before);
  lra_emit_move (new_in_reg, in_rtx);
  *before = get_insns ();
  end_sequence ();
  for (i = 0; (in = ins[i]) >= 0; i++)
    *curr_id->operand_loc[in] = new_in_reg;
  lra_update_dups (curr_id, ins);
  if (find_reg_note (curr_insn, REG_UNUSED, out_rtx) == NULL_RTX)
    {
      push_to_sequence (*after);
      lra_emit_move (out_rtx, new_out_reg);
      *after = get_insns ();
      end_sequence ();
    }
  *curr_id->operand_loc[out] = new_out_reg;
  lra_update_dup (curr_id, out);
}

/* Return register class which is union of all reg classes in insn
   constraint alternative string starting with P.  */
static enum reg_class
reg_class_from_constraints (const char *p)
{
  int c, len;
  enum reg_class op_class = NO_REGS;

  do
    switch ((c = *p, len = CONSTRAINT_LEN (c, p)), c)
      {
      case '#':
      case ',':
	return op_class;

      case 'p':
	op_class = (reg_class_subunion
		    [op_class][base_reg_class (VOIDmode, ADDR_SPACE_GENERIC,
					       ADDRESS, SCRATCH)]);
	break;
	
      case 'g':
      case 'r':
	op_class = reg_class_subunion[op_class][GENERAL_REGS];
	break;
	
      default:
	if (REG_CLASS_FROM_CONSTRAINT (c, p) == NO_REGS)
	  {
#ifdef EXTRA_CONSTRAINT_STR
	    if (EXTRA_ADDRESS_CONSTRAINT (c, p))
	      op_class
		= (reg_class_subunion
		   [op_class][base_reg_class (VOIDmode, ADDR_SPACE_GENERIC,
					      ADDRESS, SCRATCH)]);
#endif
	    break;
	  }
	
	op_class
	  = reg_class_subunion[op_class][REG_CLASS_FROM_CONSTRAINT (c, p)];
	break;
      }
  while ((p += len), c);
  return op_class;
}

/* Return final hard regno (plus offset) which will be after
   elimination.  We do this for matching constraints because the final
   hard regno could have a different class.  */
static int
get_final_hard_regno (int hard_regno, int offset)
{
  if (hard_regno < 0)
    return hard_regno;
  hard_regno += offset;
  return lra_get_elimation_hard_regno (hard_regno);
}

/* Return register class of OP.  That is a class of the hard register
   itself (if OP is a hard register), or class of assigned hard
   register to the pseudo (if OP is pseudo or its subregister), or
   class of unassigned pseudo (if OP is reload pseudo or its
   subregister).  Return NO_REGS otherwise.  */
static enum reg_class
get_op_class (rtx op)
{
  int regno, hard_regno, offset;

  if (! REG_P (op) && (GET_CODE (op) != SUBREG || ! REG_P (SUBREG_REG (op))))
    return NO_REGS;
  lra_get_hard_regno_and_offset (op, &hard_regno, &offset);
  if (hard_regno >= 0)
    {
      hard_regno = get_final_hard_regno (hard_regno, offset);
      return REGNO_REG_CLASS (hard_regno);
    }
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);
  /* Reload pseudo will get a hard register in any case.  */
  if ((regno = REGNO (op)) >= new_regno_start)
    return lra_get_allocno_class (regno);
  return NO_REGS;
}

/* Return generated insn mem_pseudo:=val if TO_P or val:=mem_pseudo
   otherwise.  If modes of MEM_PSEUDO and VAL are different, use
   SUBREG for VAL to make them equal.  Assign CODE to the insn if it
   is not recognized.

   We can not use emit_move_insn in some cases because of used bad
   practice in machine descriptions.  For example, power can use only
   base+index addressing for altivec move insns and it is checked by
   insn predicates.  On the other hand, the same move insn constraints
   permit to use offsetable memory for moving vector mode values
   from/to general registers to/from memory.  emit_move_insn will
   transform offsetable address to one with base+index addressing
   which is rejected by the constraint.  So sometimes we need to
   generate move insn without modifications and assign the code
   explicitly because the generated move can be unrecognizable because
   of the predicates.  */
static rtx
emit_spill_move (bool to_p, rtx mem_pseudo, rtx val, int code)
{
  rtx insn, after;

  start_sequence ();
  if (GET_MODE (mem_pseudo) != GET_MODE (val))
    val = gen_rtx_SUBREG (GET_MODE (mem_pseudo),
			  GET_CODE (val) == SUBREG ? SUBREG_REG (val) : val,
			  0);
  if (to_p)
    insn = gen_move_insn (mem_pseudo, val);
  else
    insn = gen_move_insn (val, mem_pseudo);
  if (recog_memoized (insn) < 0)
    INSN_CODE (insn) = code;
  emit_insn (insn);
  after = get_insns ();
  end_sequence ();
  return after;
}

/* Process a special case insn (register move), return true if we
   don't need to process it anymore.  Return that RTL was changed
   through CHANGE_P and macro SECONDARY_MEMORY_NEEDED says to use
   secondary memory through SEC_MEM_P.  */
static bool
check_and_process_move (bool *change_p, bool *sec_mem_p)
{
  int regno;
  rtx set, dest, src, dreg, sr, dr, sreg, new_reg, before, x, scratch_reg;
  enum reg_class dclass, sclass, xclass, rclass, secondary_class;
  secondary_reload_info sri;
  bool in_p, temp_assign_p;

  *sec_mem_p = *change_p = false;
  if ((set = single_set (curr_insn)) == NULL || side_effects_p (set))
    return false;
  dreg = dest = SET_DEST (set);
  sreg = src = SET_SRC (set);
  /* Quick check on the right move insn which does not need
     reloads.  */
  dclass = get_op_class (dest);
  if (dclass != NO_REGS)
    {
      sclass = get_op_class (src);
      if (sclass != NO_REGS
	  && targetm.register_move_cost (GET_MODE (src), dclass, sclass) == 2)
	return true;
    }
  if (GET_CODE (dest) == SUBREG)
    dreg = SUBREG_REG (dest);
  if (GET_CODE (src) == SUBREG)
    sreg = SUBREG_REG (src);
  if (! REG_P (dreg) || ! REG_P (sreg))
    return false;
  sclass = dclass = NO_REGS;
  dr = get_equiv_substitution (dreg);
  if (dr != dreg)
    dreg = copy_rtx (dr);
  if (REG_P (dreg))
    dclass = get_reg_class (REGNO (dreg));
  if (dclass == ALL_REGS)
    /* We don't know what class we will use -- let it be figured out
       by curr_insn_transform function.  Remember some targets does not
       work with such classes through their implementation of
       machine-dependent hooks like secondary_memory_needed.  */
    return false;
  sr = get_equiv_substitution (sreg);
  if (sr != sreg)
    sreg = copy_rtx (sr);
  if (REG_P (sreg))
    sclass = get_reg_class (REGNO (sreg));
  if (sclass == ALL_REGS)
    /* See comments above.  */
    return false;
#ifdef SECONDARY_MEMORY_NEEDED
  if (dclass != NO_REGS && sclass != NO_REGS
      && SECONDARY_MEMORY_NEEDED (sclass, dclass, GET_MODE (src)))
    {
      *sec_mem_p = true;
      return false;
    }
#endif
  sri.prev_sri = NULL;
  sri.icode = CODE_FOR_nothing;
  sri.extra_cost = 0;
  if (dclass != NO_REGS)
    {
      in_p = true;
      rclass = dclass;
      x = sreg;
      xclass = sclass;
    }
  else if (sclass != NO_REGS)
    {
      in_p = false;
      rclass = sclass;
      x = dreg;
      xclass = dclass;
    }
  else
    return false;
  temp_assign_p = false;
  /* Set up hard register for a reload pseudo for hook
     secondary_reload because some targets just ignore pseudos in the
     hook.  */
  if (xclass != NO_REGS
      && REG_P (x) && (regno = REGNO (x)) >= new_regno_start
      && lra_get_regno_hard_regno (regno) < 0)
    {
      reg_renumber[regno] = ira_class_hard_regs[xclass][0];
      temp_assign_p = true;
    }
  secondary_class
    = (enum reg_class) targetm.secondary_reload (in_p, x, (reg_class_t) rclass,
						 GET_MODE (src), &sri);
  if (temp_assign_p)
    reg_renumber [REGNO (x)] = -1;
  if (secondary_class == NO_REGS && sri.icode == CODE_FOR_nothing)
    return false;
  *change_p = true;
  new_reg = NULL_RTX;
  if (secondary_class != NO_REGS)
    new_reg = lra_create_new_reg_with_unique_value (GET_MODE (sreg), NULL_RTX,
						    secondary_class,
						    "secondary");
  start_sequence ();
  if (sri.icode == CODE_FOR_nothing)
    lra_emit_move (new_reg, sreg);
  else
    {
      enum reg_class scratch_class;

      scratch_class = (reg_class_from_constraints
		       (insn_data[sri.icode].operand[2].constraint));
      scratch_reg = (lra_create_new_reg_with_unique_value
		     (insn_data[sri.icode].operand[2].mode, NULL_RTX,
		      scratch_class, "scratch"));
      emit_insn (GEN_FCN (sri.icode) (new_reg != NULL_RTX ? new_reg : dest,
				      sreg, scratch_reg));
    }
  before = get_insns ();
  end_sequence ();
  lra_process_new_insns (curr_insn, before, NULL_RTX, "Inserting the move");
  if (new_reg != NULL_RTX)
    {
      if (GET_CODE (SET_SRC (set)) == SUBREG)
	SUBREG_REG (SET_SRC (set)) = new_reg;
      else
	SET_SRC (set) = new_reg;
    }
  else
    {
      if (lra_dump_file != NULL)
	{
	  fprintf (lra_dump_file, "Deleting move %u\n", INSN_UID (curr_insn));
	  print_rtl_slim (lra_dump_file, curr_insn, curr_insn, -1, 0);
	}
      lra_set_insn_deleted (curr_insn);
      return true;
    }
  return false;
}

/* The following data describe the result of process_alt operands.
   The data are used in curr_insn_transform to generate reloads.  */

/* The chosen reg class which should be used for the corresponding
   operands.  */
static enum reg_class goal_alt[MAX_RECOG_OPERANDS];
/* True if the operand should be the same as another operand and the
   another operand does not need a reload.  */
static bool goal_alt_match_win[MAX_RECOG_OPERANDS];
/* True if the operand does not need a reload.  */
static bool goal_alt_win[MAX_RECOG_OPERANDS];
/* True if the operand can be offsetable memory.  */
static bool goal_alt_offmemok[MAX_RECOG_OPERANDS];
/* The number of operand to which given operand can be matched to.  */
static int goal_alt_matches[MAX_RECOG_OPERANDS];
/* The number of elements in the following array.  */
static int goal_alt_dont_inherit_ops_num;
/* Numbers of operands whose reload pseudos should not be inherited.  */
static int goal_alt_dont_inherit_ops[MAX_RECOG_OPERANDS];
/* True if the insn commutative operands should be swapped.  */
static bool goal_alt_swapped;
/* The chosen insn alternative.  */
static int goal_alt_number;

/* The following four variables are used to choose the best insn
   alternative.  They reflect finally characteristics of the best
   alternative.  */

/* Number of necessary reloads and overall cost reflecting the
   previous value and other unplesantness of the best alternative.  */
static int best_losers, best_overall;
/* Number of small register classes used for operands of the best
   alternative.  */
static int best_small_class_operands_num;
/* Overall number hard registers used for reloads.  For example, on
   some targets we need 2 general registers to reload DFmode and only
   one floating point register.  */
static int best_reload_nregs;
/* Overall number reflecting distances of previous reloading the same
   value.  It is used to improve inheritance chances.  */
static int best_reload_sum;

/* True if the current insn should have no correspondingly input or
   output reloads.  */
static bool no_input_reloads_p, no_output_reloads_p;

/* True if swapped the commutative operands in the current insn.  */
static int curr_swapped;

/* Make reloads for addr register in LOC which should be of class CL,
   add reloads to list BEFORE. If AFTER is not null emit insns to set
   the register up after the insn (it is case of inc/dec, modify).  */
static bool
process_addr_reg (rtx *loc, rtx *before, rtx *after, enum reg_class cl)
{
  int regno, final_regno;
  enum reg_class rclass, new_class;
  rtx reg = *loc;
  rtx new_reg;
  enum machine_mode mode;
  bool change_p = false;

  gcc_assert (REG_P (reg));
  final_regno = regno = REGNO (reg);
  if (regno < FIRST_PSEUDO_REGISTER)
    {
      rtx final_reg = reg;
      rtx *final_loc = &final_reg;

      lra_eliminate_reg_if_possible (final_loc);
      final_regno = REGNO (*final_loc);
    }
  /* Use class of hard register after elimination because some targets
     do not recognize virtual hard registers as valid address
     registers.  */
  rclass = get_reg_class (final_regno);
  if ((*loc = get_equiv_substitution (reg)) != reg)
    {
      if (lra_dump_file != NULL)
	{
	  fprintf (lra_dump_file,
		   "Changing pseudo %d in address of insn %u on equiv ",
		   REGNO (reg), INSN_UID (curr_insn));
	  print_value_slim (lra_dump_file, *loc, 1);
	  fprintf (lra_dump_file, "\n");
	}
      *loc = copy_rtx (*loc);
      change_p = true;
    }
  if (*loc != reg || ! in_class_p (final_regno, cl, &new_class))
    {
      mode = GET_MODE (reg);
      reg = *loc;
      if (get_reload_reg (OP_IN, mode, reg, cl, "address", &new_reg))
	{
	  push_to_sequence (*before);
	  lra_emit_move (new_reg, reg);
	  *before = get_insns ();
	  end_sequence ();
	}
      *loc = new_reg;
      if (after != NULL)
	{
	  push_to_sequence (*after);
	  lra_emit_move (reg, new_reg);
	  *after = get_insns ();
	  end_sequence ();
	}
      change_p = true;
    }
  else if (new_class != NO_REGS && rclass != new_class)
    change_class (regno, new_class, "      Change", true);
  return change_p;
}

#ifndef SLOW_UNALIGNED_ACCESS
#define SLOW_UNALIGNED_ACCESS(mode, align) 0
#endif

/* Make reloads for subreg in operand NOP with internal subreg mode
   REG_MODE, add new reloads for further processing.  Return true if
   any reload was generated.  */
static bool
simplify_operand_subreg (int nop, enum machine_mode reg_mode)
{
  int hard_regno;
  rtx before, after;
  enum machine_mode mode;
  rtx reg, new_reg;
  rtx operand = *curr_id->operand_loc[nop];

  /* Remove subregs of memory.  */
  before = after = NULL_RTX;

  if (GET_CODE (operand) != SUBREG)
    return false;
  
  mode = GET_MODE (operand);
  reg = SUBREG_REG (operand);
  /* If we change address for paradoxical subreg of memory, the
     address might violate the necessary alignment or the access might
     be slow.  So take this into consideration.  */
  if ((MEM_P (reg)
       && ((! STRICT_ALIGNMENT
	    && ! SLOW_UNALIGNED_ACCESS (mode, MEM_ALIGN (reg)))
	   || MEM_ALIGN (reg) >= GET_MODE_ALIGNMENT (mode)))
      || (REG_P (reg) && REGNO (reg) < FIRST_PSEUDO_REGISTER))
    {
      alter_subreg (curr_id->operand_loc[nop]);
      return true;
    }
  /* Force reload if this is a constant or PLUS or if there may be a
     problem accessing OPERAND in the outer mode.  */
  if ((REG_P (reg)
       && REGNO (reg) >= FIRST_PSEUDO_REGISTER
       && (hard_regno = lra_get_regno_hard_regno (REGNO (reg))) >= 0
       /* Don't reload paradoxical subregs because we could looping
	  having repeatedly final regno out of hard regs range.  */
       && (hard_regno_nregs[hard_regno][GET_MODE (reg)]
	   >= hard_regno_nregs[hard_regno][mode])
       && simplify_subreg_regno (hard_regno, GET_MODE (reg),
				 SUBREG_BYTE (operand), mode) < 0)
      || CONSTANT_P (reg) || GET_CODE (reg) == PLUS || MEM_P (reg))
    {
      /* Constant mode ???? */
      enum op_type type = curr_static_id->operand[nop].type;
      /* The class will be defined later in curr_insn_transform.  */
      enum reg_class rclass
	= (enum reg_class) targetm.preferred_reload_class (reg, ALL_REGS);

      if (get_reload_reg (type, reg_mode, reg, rclass, "subreg reg", &new_reg)
	  && type != OP_OUT)
	{
	  push_to_sequence (before);
	  lra_emit_move (new_reg, reg);
	  before = get_insns ();
	  end_sequence ();
	}
      if (type != OP_IN)
	{
	  push_to_sequence (after);
	  lra_emit_move (reg, new_reg);
	  after = get_insns ();
	  end_sequence ();
	}
      SUBREG_REG (operand) = new_reg;
      lra_process_new_insns (curr_insn, before, after,
			     "Inserting subreg reload");
      return true;
    }
  return false;
}

/* Return TRUE if *LOC refers for a hard register from SET.  */
static bool
uses_hard_regs_p (rtx *loc, HARD_REG_SET set)
{
  int i, j, x_hard_regno, offset;
  enum machine_mode mode;
  rtx x;
  const char *fmt;
  enum rtx_code code;

  if (*loc == NULL_RTX)
    return false;
  x = *loc;
  code = GET_CODE (x);
  mode = GET_MODE (x);
  if (code == SUBREG)
    {
      loc = &SUBREG_REG (x);
      x = SUBREG_REG (x);
      code = GET_CODE (x);
      if (GET_MODE_SIZE (GET_MODE (x)) > GET_MODE_SIZE (mode))
	mode = GET_MODE (x);
    }
  
  if (REG_P (x))
    {
      lra_get_hard_regno_and_offset (x, &x_hard_regno, &offset);
      /* The real hard regno of the operand after the allocation.  It
	 can be negative only for registers.  */
      x_hard_regno = get_final_hard_regno (x_hard_regno, offset);
      return (x_hard_regno >= 0
	      && lra_hard_reg_set_intersection_p (x_hard_regno, mode, set));
    }
  if (MEM_P (x))
    {
      struct address ad;
      enum machine_mode mode = GET_MODE (x);
      rtx *addr_loc = &XEXP (x, 0);

      extract_address_regs (mode, MEM_ADDR_SPACE (x), addr_loc, MEM, &ad);
      if (ad.base_reg_loc != NULL)
	{
	  if (uses_hard_regs_p (ad.base_reg_loc, set))
	    return true;
	}
      if (ad.index_reg_loc != NULL)
	{
	  if (uses_hard_regs_p (ad.index_reg_loc, set))
	    return true;
	}
    }
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  if (uses_hard_regs_p (&XEXP (x, i), set))
	    return true;
	}
      else if (fmt[i] == 'E')
	{
	  for (j = XVECLEN (*loc, i) - 1; j >= 0; j--)
	    if (uses_hard_regs_p (&XVECEXP (*loc, i, j), set))
	      return true;
	}
    }
  return false;
}

/* Cost factor for each additional reload and maximal cost bound for
   insn reloads.  One might ask about such strange numbers.  Their
   values occured historically from former reload pass. In some way,
   even machine descriptions.  */
#define LOSER_COST_FACTOR 6
#define MAX_OVERALL_COST_BOUND 600

/* Major function to choose the current insn alternative and what
   operands should be reload and how.  If ONLY_ALTERNATIVE is not
   negative we should consider only this alternative.  Return false if
   we can not choose the alternative or find how to reload the
   operands.  */
static bool
process_alt_operands (int only_alternative)
{
  bool ok_p = false;
  int nop, small_class_operands_num, overall, nalt, offset;
  int n_alternatives = curr_static_id->n_alternatives;
  int n_operands = curr_static_id->n_operands;
  /* LOSERS counts those that don't fit this alternative and would
     require loading.  */
  int losers;
  /* REJECT is a count of how undesirable this alternative says it is
     if any reloading is required.  If the alternative matches exactly
     then REJECT is ignored, but otherwise it gets this much counted
     against it in addition to the reloading needed.  Each ? counts
     three times here since we want the disparaging caused by a bad
     register class to only count 1/3 as much.  */
  int reject;
  /* The number of elements in the following array.  */
  int early_clobbered_regs_num;
  /* Numbers of operands which are early clobber registers.  */
  int early_clobbered_nops[MAX_RECOG_OPERANDS];
  enum reg_class curr_alt[MAX_RECOG_OPERANDS];
  HARD_REG_SET curr_alt_set[MAX_RECOG_OPERANDS];
  bool curr_alt_match_win[MAX_RECOG_OPERANDS];
  bool curr_alt_win[MAX_RECOG_OPERANDS];
  bool curr_alt_offmemok[MAX_RECOG_OPERANDS];
  int curr_alt_matches[MAX_RECOG_OPERANDS];
  /* The number of elements in the following array.  */
  int curr_alt_dont_inherit_ops_num;
  /* Numbers of operands whose reload pseudos should not be inherited.  */
  int curr_alt_dont_inherit_ops[MAX_RECOG_OPERANDS];
  rtx op;
  rtx no_subreg_operand[MAX_RECOG_OPERANDS], operand_reg[MAX_RECOG_OPERANDS];
  int hard_regno[MAX_RECOG_OPERANDS];
  enum machine_mode biggest_mode[MAX_RECOG_OPERANDS];
  int reload_nregs, reload_sum;
  bool costly_p;
  enum reg_class cl;

  /* Calculate some data common for all alternatives to speed up the
     function.  */
  for (nop = 0; nop < n_operands; nop++)
    {
      op = no_subreg_operand[nop] = *curr_id->operand_loc[nop];
      lra_get_hard_regno_and_offset (op, &hard_regno[nop], &offset);
      /* The real hard regno of the operand after the allocation.  It
	 can be negative only for registers.  */
      hard_regno[nop] = get_final_hard_regno (hard_regno[nop], offset);
      
      operand_reg[nop] = op;
      biggest_mode[nop] = GET_MODE (operand_reg[nop]);
      if (GET_CODE (operand_reg[nop]) == SUBREG)
	{
	  operand_reg[nop] = SUBREG_REG (operand_reg[nop]);
	  if (GET_MODE_SIZE (biggest_mode[nop])
	      < GET_MODE_SIZE (GET_MODE (operand_reg[nop])))
	    biggest_mode[nop] = GET_MODE (operand_reg[nop]);
	}
      if (REG_P (operand_reg[nop]))
	no_subreg_operand[nop] = operand_reg[nop];
      else
	operand_reg[nop] = NULL_RTX;
    }

  /* The constraints are made of several alternatives.  Each operand's
     constraint looks like foo,bar,... with commas separating the
     alternatives.  The first alternatives for all operands go
     together, the second alternatives go together, etc.

     First loop over alternatives.  */
  for (nalt = 0; nalt < n_alternatives; nalt++)
    {
      /* Loop over operands for one constraint alternative.  */
      if (
#ifdef HAVE_ATTR_enabled
	  (curr_id->alternative_enabled_p != NULL
	   && ! curr_id->alternative_enabled_p[nalt])
	  ||
#endif
	  (only_alternative >= 0 && nalt != only_alternative))
	continue;

      overall = losers = reject = reload_nregs = reload_sum = 0;
      for (nop = 0; nop < n_operands; nop++)
	reject += (curr_static_id
		   ->operand_alternative[nalt * n_operands + nop].reject);
      early_clobbered_regs_num = 0;

      for (nop = 0; nop < n_operands; nop++)
	{
	  const char *p;
	  char *end;
	  int len, c, m, i, opalt_num, this_alternative_matches;
	  bool win, did_match, offmemok, early_clobber_p;
	  /* false => this operand can be reloaded somehow for this
	     alternative.  */
	  bool badop;
	  /* false => this operand can be reloaded if the alternative
	     allows regs.  */
	  bool winreg;
	  /* False if a constant forced into memory would be OK for
	     this operand.  */
	  bool constmemok;
	  enum reg_class this_alternative, this_costly_alternative;
	  HARD_REG_SET this_alternative_set, this_costly_alternative_set;
	  bool this_alternative_match_win, this_alternative_win;
	  bool this_alternative_offmemok;
	  int invalidate_m;
	  enum machine_mode mode;

	  opalt_num = nalt * n_operands + nop;
	  if (curr_static_id->operand_alternative[opalt_num].anything_ok)
	    {
	      /* Fast track for no constraints at all.  */
	      curr_alt[nop] = NO_REGS;
	      CLEAR_HARD_REG_SET (curr_alt_set[nop]);
	      curr_alt_win[nop] = true;
	      curr_alt_match_win[nop] = false;
	      curr_alt_offmemok[nop] = false;
	      curr_alt_matches[nop] = -1;
	      continue;
	    }
      
	  op = no_subreg_operand[nop];
	  mode = GET_MODE (*curr_id->operand_loc[nop]);

	  win = did_match = winreg = offmemok = constmemok = false;
	  badop = true;
      
	  early_clobber_p = false;
	  p = curr_static_id->operand_alternative[opalt_num].constraint;
      
	  this_costly_alternative = this_alternative = NO_REGS;
	  /* We update set of possible hard regs besides its class
	     because reg class might be inaccurate.  For example,
	     union of LO_REGS (l), HI_REGS(h), and STACK_REG(k) in ARM
	     is translated in HI_REGS because classes are merged by
	     pairs and there is no accurate intermediate class.  */
	  CLEAR_HARD_REG_SET (this_alternative_set);
	  CLEAR_HARD_REG_SET (this_costly_alternative_set);
	  this_alternative_win = false;
	  this_alternative_match_win = false;
	  this_alternative_offmemok = false;
	  this_alternative_matches = -1;
  
	  invalidate_m = -1;

	  /* An empty constraint should be excluded by the fast
	     track.  */
	  gcc_assert (*p != 0 && *p != ',');
  
	  /* Scan this alternative's specs for this operand; set WIN
	     if the operand fits any letter in this alternative.
	     Otherwise, clear BADOP if this operand could fit some
	     letter after reloads, or set WINREG if this operand could
	     fit after reloads provided the constraint allows some
	     registers.  */
	  costly_p = false;
	  do
	    {
	      switch ((c = *p, len = CONSTRAINT_LEN (c, p)), c)
		{
		case '\0':
		  len = 0;
		  break;
		case ',':
		  c = '\0';
		  break;
	
		case '=':  case '+': case '?': case '*': case '!':
		case ' ': case '\t':
		  break;
		  
		case '%':
		  /* We only support one commutative marker, the first
		     one.  We already set commutative above.  */
		  break;
		  
		case '&':
		  early_clobber_p = true;
		  break;
		  
		case '#':
		  /* Ignore rest of this alternative.  */
		  c = '\0';
		  break;
		  
		case '0':  case '1':  case '2':  case '3':  case '4':
		case '5':  case '6':  case '7':  case '8':  case '9':
		  {
		    int m_hregno, m_offset;
		    bool match_p;
		    
		    m = strtoul (p, &end, 10);
		    p = end;
		    len = 0;
		    gcc_assert (nop > m);
		    
		    this_alternative_matches = m;
		    lra_get_hard_regno_and_offset (*curr_id->operand_loc[m],
						   &m_hregno, &m_offset);
		    m_hregno = get_final_hard_regno (m_hregno, m_offset);
		    /* We are supposed to match a previous operand.
		       If we do, we win if that one did.  If we do
		       not, count both of the operands as losers.
		       (This is too conservative, since most of the
		       time only a single reload insn will be needed
		       to make the two operands win.  As a result,
		       this alternative may be rejected when it is
		       actually desirable.)  */
		    /* If it conflicts with others.  */
		    match_p = false;
		    if (operands_match_p (*curr_id->operand_loc[nop],
					  *curr_id->operand_loc[m], m_hregno))
		      {
			int i;
			
			for (i = 0; i < early_clobbered_regs_num; i++)
			  if (early_clobbered_nops[i] == m)
			    break;
			/* We should reject matching of an early
			   clobber operand if the matching operand is
			   not dying in the insn.  */
			if (i >= early_clobbered_regs_num
			    || operand_reg[nop] == NULL_RTX
			    || (find_regno_note (curr_insn, REG_DEAD,
						 REGNO (operand_reg[nop]))
				!= NULL_RTX))
			  match_p = true;
		      }
		    if (match_p)
		      {
			/* If we are matching a non-offsettable
			   address where an offsettable address was
			   expected, then we must reject this
			   combination, because we can't reload
			   it.  */
			if (curr_alt_offmemok[m]
			    && MEM_P (*curr_id->operand_loc[m])
			    && curr_alt[m] == NO_REGS && ! curr_alt_win[m])
			  continue;
			
			did_match = curr_alt_win[m];
		      }
		    else
		      {
			/* Operands don't match.  */
			/* Retroactively mark the operand we had to
			   match as a loser, if it wasn't already and
			   it wasn't matched to a register constraint
			   (e.g it might be matched by memory).  */
			if (curr_alt_win[m]
			    && (operand_reg[m] == NULL_RTX || hard_regno[m] < 0))
			  {
			    losers++;
			    if (curr_alt[m] != NO_REGS)
			      reload_nregs
				+= (ira_reg_class_max_nregs[curr_alt[m]]
				    [GET_MODE (*curr_id->operand_loc[m])]);
			}
			invalidate_m = m;
			if (curr_alt[m] == NO_REGS)
			  continue;
			
			/* We prefer no matching alternatives because
			   it gives more freedom in RA.  */
			if (operand_reg[nop] == NULL_RTX
			    || (find_regno_note (curr_insn, REG_DEAD,
						 REGNO (operand_reg[nop]))
				== NULL_RTX))
			  reject += 2;
		      }
		    /* This can be fixed with reloads if the operand
		       we are supposed to match can be fixed with
		       reloads.  */
		    badop = false;
		    this_alternative = curr_alt[m];
		    COPY_HARD_REG_SET (this_alternative_set, curr_alt_set[m]);
		    
		    /* If we have to reload this operand and some
		       previous operand also had to match the same
		       thing as this operand, we don't know how to do
		       that.  So reject this alternative.  */
		    if (! did_match)
		      for (i = 0; i < nop; i++)
			if (curr_alt_matches[i] == this_alternative_matches)
			  badop = true;
		    
		    break;
		  }
		  
		case 'p':
		  cl = base_reg_class (VOIDmode, ADDR_SPACE_GENERIC,
				       ADDRESS, SCRATCH);
		  this_alternative = reg_class_subunion[this_alternative][cl];
		  IOR_HARD_REG_SET (this_alternative_set, reg_class_contents[cl]);
		  if (costly_p)
		    {
		      this_costly_alternative
			= reg_class_subunion[this_costly_alternative][cl];
		      IOR_HARD_REG_SET (this_costly_alternative_set,
					reg_class_contents[cl]);
		    }
		  win = true;
		  badop = false;
		  break;
		  
		case TARGET_MEM_CONSTRAINT:
		  if (MEM_P (op)
		      || (REG_P (op)
			  && REGNO (op) >= FIRST_PSEUDO_REGISTER
			  && in_mem_p (REGNO (op))))
		    win = true;
		  if (CONST_POOL_OK_P (mode, op))
		    badop = false;
		  constmemok = true;
		  break;
		  
		case '<':
		  if (MEM_P (op)
		      && (GET_CODE (XEXP (op, 0)) == PRE_DEC
			  || GET_CODE (XEXP (op, 0)) == POST_DEC))
		    win = true;
		  break;
		  
		case '>':
		  if (MEM_P (op)
		      && (GET_CODE (XEXP (op, 0)) == PRE_INC
			  || GET_CODE (XEXP (op, 0)) == POST_INC))
		    win = true;
		  break;
		  
		/* Memory op whose address is not offsettable.  */
		case 'V':
		  if (MEM_P (op)
		      && ! offsettable_nonstrict_memref_p (op))
		    win = true;
		  break;
		  
		  /* Memory operand whose address is offsettable.  */
		case 'o':
		  if ((MEM_P (op)
		       && offsettable_nonstrict_memref_p (op))
		      || (REG_P (op)
			  && REGNO (op) >= FIRST_PSEUDO_REGISTER
			  && in_mem_p (REGNO (op))))
		    win = true;
		  if (CONST_POOL_OK_P (mode, op) || MEM_P (op))
		    badop = false;
		  constmemok = true;
		  offmemok = true;
		  break;
		  
		case 'E':
		case 'F':
		  if (GET_CODE (op) == CONST_DOUBLE
		      || (GET_CODE (op) == CONST_VECTOR
			  && (GET_MODE_CLASS (mode) == MODE_VECTOR_FLOAT)))
		    win = true;
		  break;
		  
		case 'G':
		case 'H':
		  if (GET_CODE (op) == CONST_DOUBLE
		      && CONST_DOUBLE_OK_FOR_CONSTRAINT_P (op, c, p))
		    win = true;
		  break;
		  
		case 's':
		  if (CONST_INT_P (op)
		      || (GET_CODE (op) == CONST_DOUBLE && mode == VOIDmode))
		    break;
		case 'i':
		  if (CONSTANT_P (op)
		      && (! flag_pic || LEGITIMATE_PIC_OPERAND_P (op)))
		    win = true;
		  break;
		  
		case 'n':
		  if (CONST_INT_P (op)
		      || (GET_CODE (op) == CONST_DOUBLE && mode == VOIDmode))
		    win = true;
		  break;
	
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		  if (CONST_INT_P (op)
		      && CONST_OK_FOR_CONSTRAINT_P (INTVAL (op), c, p))
		    win = true;
		  break;
		  
		case 'X':
		  /* This constraint should be excluded by the fast
		     track.  */
		  gcc_unreachable ();
		  break;
		  
		case 'g':
		  if (/* A PLUS is never a valid operand, but LRA can
			 make it from a register when eliminating
			 registers.  */
		      GET_CODE (op) != PLUS
		      && (! CONSTANT_P (op) || ! flag_pic
			  || LEGITIMATE_PIC_OPERAND_P (op))
		      && (! REG_P (op)
			  || (REGNO (op) >= FIRST_PSEUDO_REGISTER
			      && in_mem_p (REGNO (op)))))
		    win = true;
		  /* Drop through into 'r' case.  */
		  
		case 'r':
		  this_alternative
		    = reg_class_subunion[this_alternative][GENERAL_REGS];
		  IOR_HARD_REG_SET (this_alternative_set,
				    reg_class_contents[GENERAL_REGS]);
		  if (costly_p)
		    {
		      this_costly_alternative
			= reg_class_subunion[this_costly_alternative][GENERAL_REGS];
		      IOR_HARD_REG_SET (this_costly_alternative_set,
					reg_class_contents[GENERAL_REGS]);
		    }
		  goto reg;
		  
		default:
		  if (REG_CLASS_FROM_CONSTRAINT (c, p) == NO_REGS)
		    {
#ifdef EXTRA_CONSTRAINT_STR
		      if (EXTRA_MEMORY_CONSTRAINT (c, p))
			{
			  if (EXTRA_CONSTRAINT_STR (op, c, p))
			    win = true;
			  /* For regno_equiv_mem_loc we have to
			     check.  */
			  else if (REG_P (op)
				   && REGNO (op) >= FIRST_PSEUDO_REGISTER
				   && in_mem_p (REGNO (op)))
			    {
			      /* We could transform spilled memory
				 finally to indirect memory.  */
			      if (EXTRA_CONSTRAINT_STR (get_indirect_mem (mode),
							c, p))
				win = true;
			    }
			  
			  /* If we didn't already win, we can reload
			     constants via force_const_mem, and other
			     MEMs by reloading the address like for
			     'o'.  */
			  if (CONST_POOL_OK_P (mode, op) || MEM_P (op))
			    badop = false;
			  constmemok = true;
			  offmemok = true;
			  break;
			}
		      if (EXTRA_ADDRESS_CONSTRAINT (c, p))
			{
			  if (EXTRA_CONSTRAINT_STR (op, c, p))
			    win = true;
			  
			  /* If we didn't already win, we can reload
			     the address into a base register.  */
			  cl = base_reg_class (VOIDmode, ADDR_SPACE_GENERIC,
					       ADDRESS, SCRATCH);
			  this_alternative
			    = reg_class_subunion[this_alternative][cl];
			  IOR_HARD_REG_SET (this_alternative_set,
					    reg_class_contents[cl]);
			  if (costly_p)
			    {
			      this_costly_alternative
				= reg_class_subunion[this_costly_alternative][cl];
			      IOR_HARD_REG_SET (this_costly_alternative_set,
						reg_class_contents[cl]);
			    }
			  badop = false;
			  break;
			}
		      
		      if (EXTRA_CONSTRAINT_STR (op, c, p))
			win = true;
		      else if (REG_P (op)
			       && REGNO (op) >= FIRST_PSEUDO_REGISTER
			       && in_mem_p (REGNO (op)))
			{
			  /* We could transform spilled memory finally
			     to indirect memory.  */
			  if (EXTRA_CONSTRAINT_STR (get_indirect_mem (mode),
						    c, p))
			    win = true;
			}
#endif
		      break;
		    }
		  
		  cl = REG_CLASS_FROM_CONSTRAINT (c, p);
		  this_alternative = reg_class_subunion[this_alternative][cl];
		  IOR_HARD_REG_SET (this_alternative_set, reg_class_contents[cl]);
		  if (costly_p)
		    {
		      this_costly_alternative
			= reg_class_subunion[this_costly_alternative][cl];
		      IOR_HARD_REG_SET (this_costly_alternative_set,
					reg_class_contents[cl]);
		    }
		reg:
		  if (mode == BLKmode)
		    break;
		  winreg = true;
		  if (REG_P (op))
		    {
		      if (hard_regno[nop] >= 0
			  && in_hard_reg_set_p (this_alternative_set,
						mode, hard_regno[nop]))
			win = true;
		      else if (hard_regno[nop] < 0
			       && in_class_p (REGNO (op),
					      this_alternative, NULL))
			win = true;
		    }
		  break;
		}
	      if (c != ' ' && c != '\t')
		costly_p = c == '*';
	    }
	  while ((p += len), c);
  
	  /* If this operand could be handled with a reg, and some reg
	     is allowed, then this operand can be handled.  */
	  if (winreg && this_alternative != NO_REGS)
	    badop = false;
  
	  /* Record which operands fit this alternative.  */
	  if (win)
	    {
	      this_alternative_win = true;
	      if (operand_reg[nop] != NULL_RTX)
		{
		  if (hard_regno[nop] >= 0)
		    {
		      if (in_hard_reg_set_p (this_costly_alternative_set,
					     mode, hard_regno[nop]))
			reject++;
		    }
		  else
		    {
		      /* Prefer won reg to spilled pseudo under other equal
			 conditions.  */
		      reject++;
		      if (in_class_p (REGNO (operand_reg[nop]),
				      this_costly_alternative, NULL))
			reject++;
		    }
		}
	    }
	  else if (did_match)
	    this_alternative_match_win = true;
	  else
	    {
	      int const_to_mem = 0;
	      bool no_regs_p;

	      no_regs_p
		= (this_alternative == NO_REGS
		   || (hard_reg_set_subset_p
		       (reg_class_contents[this_alternative],
			lra_no_alloc_regs)));
	      this_alternative_offmemok = offmemok;
	      if (this_costly_alternative != NO_REGS)
		reject++;
	      /* If the operand is dying, has a matching constraint,
		 and satisfies constraints of the matched operand
		 which failed to satisfy the own constraints, we do
		 not need to generate a reload insn for this
		 operand.  */
	      if (this_alternative_matches < 0
		  || curr_alt_win[this_alternative_matches]
		  || ! REG_P (op)
		  || find_regno_note (curr_insn, REG_DEAD,
				      REGNO (op)) == NULL_RTX
		  || ((hard_regno[nop] < 0
		       || ! in_hard_reg_set_p (this_alternative_set,
					       mode, hard_regno[nop]))
		      && (hard_regno[nop] >= 0
			  || ! in_class_p (REGNO (op),
					   this_alternative, NULL))))
		losers++;
	      if (operand_reg[nop] != NULL_RTX)
		{
		  /* ??? */
		  int last_reload = (lra_reg_info[ORIGINAL_REGNO
						  (operand_reg[nop])]
				     .last_reload);

		  if (last_reload > bb_reload_num)
		    reload_sum += last_reload;
		  else
		    reload_sum += bb_reload_num;
		}
	      if (badop
		  /* Alternative loses if it has no regs for a reg
		     operand.  */
		  || (REG_P (op) && no_regs_p
		      && this_alternative_matches < 0))
		goto fail;
      
	      /* If this is a constant that is reloaded into the
		 desired class by copying it to memory first, count
		 that as another reload.  This is consistent with
		 other code and is required to avoid choosing another
		 alternative when the constant is moved into memory.
		 Note that the test here is precisely the same as in
		 the code below that calls force_const_mem.  */
	      if (CONST_POOL_OK_P (mode, op)
		  && ((targetm.preferred_reload_class
		       (op, this_alternative) == NO_REGS)
		      || no_input_reloads_p)
		  && get_op_mode (nop) != VOIDmode)
		{
		  const_to_mem = 1;
		  if (! no_regs_p)
		    losers++;
		}
      
	      /* Alternative loses if it requires a type of reload not
		 permitted for this insn.  We can always reload
		 objects with a REG_UNUSED note.  */
	      if ((curr_static_id->operand[nop].type != OP_IN
		   && no_output_reloads_p
		   && ! find_reg_note (curr_insn, REG_UNUSED, op))
		  || (curr_static_id->operand[nop].type != OP_OUT
		      && no_input_reloads_p && ! const_to_mem))
		goto fail;
      
	      /* If we can't reload this value at all, reject this
		 alternative.  Note that we could also lose due to
		 LIMIT_RELOAD_CLASS, but we don't check that here.  */
	      if (! CONSTANT_P (op) && ! no_regs_p)
		{
		  if (targetm.preferred_reload_class
		      (op, this_alternative) == NO_REGS)
		    reject = MAX_OVERALL_COST_BOUND;
	  
		  if (curr_static_id->operand[nop].type == OP_OUT
		      && (targetm.preferred_output_reload_class
			  (op, this_alternative) == NO_REGS))
		    reject = MAX_OVERALL_COST_BOUND;
		}
      
	      /* We prefer to reload pseudos over reloading other
		 things, since such reloads may be able to be
		 eliminated later.  So bump REJECT in other cases.
		 Don't do this in the case where we are forcing a
		 constant into memory and it will then win since we
		 don't want to have a different alternative match
		 then.  */
	      if (! (REG_P (op)
		     && REGNO (op) >= FIRST_PSEUDO_REGISTER)
		  && ! (const_to_mem && constmemok)
		  /* We can reload the address instead of memory (so
		     do not punish it).  It is preferable to do to
		     avoid cycling in some cases.  */
		  && ! (MEM_P (op) && offmemok))
		reject += 2;
      
	      /* Input reloads can be inherited more often than output
		 reloads can be removed, so penalize output
		 reloads.  */
	      if (!REG_P (op) || curr_static_id->operand[nop].type != OP_IN)
		reject++;
	      /* SUBREGS ??? */
	      if (this_alternative_matches >= 0)
		{
		  /* ?!? */
		}
	      else if (no_regs_p && ! this_alternative_offmemok && ! constmemok)
		goto fail;

	      if (! no_regs_p)
		reload_nregs += ira_reg_class_max_nregs[this_alternative][mode];
	    }
  
	  if (early_clobber_p)
	    reject++;
	  /* ??? Should we update the cost because early clobber
	     register reloads or it is a rare thing to be worth to do
	     it.  */
	  overall = losers * LOSER_COST_FACTOR + reject;
	  if ((best_losers == 0 || losers != 0) && best_overall < overall)
	    goto fail;

	  curr_alt[nop] = this_alternative;
	  COPY_HARD_REG_SET (curr_alt_set[nop], this_alternative_set);
	  curr_alt_win[nop] = this_alternative_win;
	  curr_alt_match_win[nop] = this_alternative_match_win;
	  curr_alt_offmemok[nop] = this_alternative_offmemok;
	  curr_alt_matches[nop] = this_alternative_matches;
  
	  if (invalidate_m >= 0 && ! this_alternative_win)
	    curr_alt_win[invalidate_m] = false;
  
	  if (early_clobber_p && operand_reg[nop] != NULL_RTX)
	    early_clobbered_nops[early_clobbered_regs_num++] = nop;
	}
      ok_p = true;
      curr_alt_dont_inherit_ops_num = 0;
      for (nop = 0; nop < early_clobbered_regs_num; nop++)
	{
	  int i, j, clobbered_hard_regno;
	  HARD_REG_SET temp_set;

	  i = early_clobbered_nops[nop];
	  if ((! curr_alt_win[i] && ! curr_alt_match_win[i])
	      || hard_regno[i] < 0)
	    continue;
	  clobbered_hard_regno = hard_regno[i];
	  CLEAR_HARD_REG_SET (temp_set);
	  for (j = hard_regno_nregs[clobbered_hard_regno][biggest_mode[i]] - 1;
	       j >= 0;
	       j--)
	    SET_HARD_REG_BIT (temp_set, clobbered_hard_regno + j);
	  for (j = 0; j < n_operands; j++)
	    if (j == i
		/* We don't want process insides of match_operator and
		   match_parallel because otherwise we would process
		   their operands once again generating a wrong
		   code.  */
		|| curr_static_id->operand[j].is_operator)
	      continue;
	    else if (curr_alt_matches[j] == i && curr_alt_match_win[j])
	      {
		/* This is a trick.  Such operands don't conflict and
		   don't need a reload.  But it is hard to transfer
		   this information to the assignment pass which
		   spills one operand without this info.  We avoid the
		   conflict by forcing to use the same pseudo for the
		   operands hoping that the pseudo gets the same hard
		   regno as the operands and the reloads are gone.  */
		curr_alt_win[i] = false;
		curr_alt_match_win[j] = false;
		continue;
	      }
	    else if (curr_alt_matches[i] == j && curr_alt_match_win[i])
	      {
		/* See the comment for the previous case. */
		curr_alt_win[j] = false;
		curr_alt_match_win[i] = false;
		continue;
	      }
	    else if (uses_hard_regs_p (curr_id->operand_loc[j], temp_set))
	      break;
	  if (j >= n_operands)
	    continue;
	  /* We need to reload early clobbered register.  */
	  for (j = 0; j < n_operands; j++)
	    if (curr_alt_matches[j] == i)
	      {
		curr_alt_match_win[j] = false;
		losers++;
		overall += LOSER_COST_FACTOR;
	      }
	  if (! curr_alt_match_win[i])
	    curr_alt_dont_inherit_ops[curr_alt_dont_inherit_ops_num++] = i;
	  else
	    {
	      /* Remember pseudos used for match reloads are never inherited.  */
	      gcc_assert (curr_alt_matches[i] >= 0);
	      curr_alt_win[curr_alt_matches[i]] = false;
	    }
	  curr_alt_win[i] = curr_alt_match_win[i] = false;
	  losers++;
	  overall += LOSER_COST_FACTOR;
	}
      small_class_operands_num = 0;
      for (nop = 0; nop < n_operands; nop++)
	/* If this alternative can be made to work by reloading, and
	   it needs less reloading than the others checked so far,
	   record it as the chosen goal for reloading.  */
	small_class_operands_num
	  += SMALL_REGISTER_CLASS_P (curr_alt[nop]) ? 1 : 0;

      if ((best_losers != 0 && losers == 0)
	  || (((best_losers == 0 && losers == 0)
	       || (best_losers != 0 && losers != 0))
	      && (best_overall > overall
		  || (best_overall == overall
		      /* If the cost of the reloads is the same,
			 prefer alternative which requires minimal
			 number of small register classes for the
			 operands.  This improves chances of reloads
			 for insn requiring small register
			 classes.  */
		      && (small_class_operands_num
			  < best_small_class_operands_num
			  || (small_class_operands_num
			      == best_small_class_operands_num
			      && (reload_nregs < best_reload_nregs
				  || (reload_nregs == best_reload_nregs
				      && best_reload_sum < reload_sum))))))))
	{
	  for (nop = 0; nop < n_operands; nop++)
	    {
	      goal_alt_win[nop] = curr_alt_win[nop];
	      goal_alt_match_win[nop] = curr_alt_match_win[nop];
	      goal_alt_matches[nop] = curr_alt_matches[nop];
	      goal_alt[nop] = curr_alt[nop];
	      goal_alt_offmemok[nop] = curr_alt_offmemok[nop];
	    }
	  goal_alt_dont_inherit_ops_num = curr_alt_dont_inherit_ops_num;
	  for (nop = 0; nop < curr_alt_dont_inherit_ops_num; nop++)
	    goal_alt_dont_inherit_ops[nop] = curr_alt_dont_inherit_ops[nop];
	  goal_alt_swapped = curr_swapped;
	  best_overall = overall;
	  best_losers = losers;
	  best_small_class_operands_num = small_class_operands_num;
	  best_reload_nregs = reload_nregs;
	  best_reload_sum = reload_sum;
	  goal_alt_number = nalt;
	}
      if (losers == 0)
	/* Everything is satisfied.  Do not process alternatives
	   anymore.  */ 
	break;
    fail:
      ;
    }
  return ok_p;
}

/* Return 1 if ADDR is a valid memory address for mode MODE
   in address space AS, and check that each pseudo reg has the
   proper kind of hard reg.  */
static int
valid_address_p (enum machine_mode mode ATTRIBUTE_UNUSED,
		 rtx addr, addr_space_t as)
{
#ifdef GO_IF_LEGITIMATE_ADDRESS
  gcc_assert (ADDR_SPACE_GENERIC_P (as));
  GO_IF_LEGITIMATE_ADDRESS (mode, addr, win);
  return 0;
  
 win:
  return 1;
#else
  return targetm.addr_space.legitimate_address_p (mode, addr, 0, as);
#endif
}

/* Make reload base reg + disp from address AD in space AS of memory
   with MODE into a new pseudo.  Return the new pseudo.  */
static rtx
base_plus_disp_to_reg (enum machine_mode mode, addr_space_t as,
		       struct address *ad)
{
  enum reg_class cl;
  rtx new_reg;

  gcc_assert (ad->base_reg_loc != NULL && ad->disp_loc != NULL);
  cl = base_reg_class (mode, as, ad->base_outer_code, ad->index_code);
  new_reg = lra_create_new_reg (Pmode, NULL_RTX, cl, "base + disp");
  lra_emit_add (new_reg, *ad->base_reg_loc, *ad->disp_loc);
  return new_reg;
}

/* Make substitution in address AD in space AS with location ADDR_LOC.
   Update AD and ADDR_LOC if it is necessary.  Return true if a
   substitution was made.  */
static bool
equiv_address_substitution (struct address *ad, rtx *addr_loc,
			    enum machine_mode mode, addr_space_t as,
			    enum rtx_code code)
{
  rtx base_reg, new_base_reg, index_reg, new_index_reg;
  HOST_WIDE_INT disp, scale;
  bool change_p;

  if (ad->base_reg_loc == NULL)
    base_reg = new_base_reg = NULL_RTX;
  else
    {
      base_reg = *ad->base_reg_loc;
      new_base_reg = get_equiv_substitution (base_reg);
    }
  if (ad->index_reg_loc == NULL)
    index_reg = new_index_reg = NULL_RTX;
  else
    {
      index_reg = *ad->index_reg_loc;
      new_index_reg = get_equiv_substitution (index_reg);
    }
  if (base_reg == new_base_reg && index_reg == new_index_reg)
    return false;
  disp = 0;
  change_p = false;
  if (lra_dump_file != NULL)
    {
      fprintf (lra_dump_file, "Changing address in insn %d ",
	       INSN_UID (curr_insn));
      print_value_slim (lra_dump_file, *addr_loc, 1);
    }
  if (base_reg != new_base_reg)
    {
      if (REG_P (new_base_reg))
	{
	  *ad->base_reg_loc = new_base_reg;
	  change_p = true;
	}
      else if (GET_CODE (new_base_reg) == PLUS
	       && REG_P (XEXP (new_base_reg, 0))
	       && CONST_INT_P (XEXP (new_base_reg, 1)))
	{
	  disp += INTVAL (XEXP (new_base_reg, 1));
	  *ad->base_reg_loc = XEXP (new_base_reg, 0);
	  change_p = true;
	}
      if (ad->base_reg_loc2 != NULL)
	*ad->base_reg_loc2 = *ad->base_reg_loc;
    }
  scale = 1;
  if (ad->index_loc != NULL && GET_CODE (*ad->index_loc) == MULT)
    {
      gcc_assert (CONST_INT_P (XEXP (*ad->index_loc, 1)));
      scale = INTVAL (XEXP (*ad->index_loc, 1));
    }
  if (index_reg != new_index_reg)
    {
      if (REG_P (new_index_reg))
	{
	  *ad->index_reg_loc = new_index_reg;
	  change_p = true;
	}
      else if (GET_CODE (new_index_reg) == PLUS
	       && REG_P (XEXP (new_index_reg, 0))
	       && CONST_INT_P (XEXP (new_index_reg, 1)))
	{
	  disp += INTVAL (XEXP (new_index_reg, 1)) * scale;
	  *ad->index_reg_loc = XEXP (new_index_reg, 0);
	  change_p = true;
	}
    }
  if (disp != 0)
    {
      if (ad->disp_loc != NULL)
	*ad->disp_loc = plus_constant (*ad->disp_loc, disp);
      else
	{
	  *addr_loc = gen_rtx_PLUS (Pmode, *addr_loc, GEN_INT (disp));
	  extract_address_regs (mode, as, addr_loc, code, ad);
	}
      change_p = true;
    }
  if (lra_dump_file != NULL)
    {
      if (! change_p)
	fprintf (lra_dump_file, " -- no change\n");
      else
	{
	  fprintf (lra_dump_file, " on equiv ");
	  print_value_slim (lra_dump_file, *addr_loc, 1);
	  fprintf (lra_dump_file, "\n");
	}
    }
  return change_p;
}

/* Exchange operands of plus X. */
static void
exchange_plus_ops (rtx x)
{
  rtx op0;

  gcc_assert (GET_CODE (x) == PLUS);
  op0 = XEXP (x, 0);
  XEXP (x, 0) = XEXP (x, 1);
  XEXP (x, 1) = op0;
}

/* Major function to make reloads for address in operand NOP.  Add to
   reloads to the list *BEFORE and *AFTER.  We might need to add
   reloads to *AFTER because of inc/dec, {pre, post} modify in the
   address.  Return true for any RTL change.  */
static bool
process_address (int nop, rtx *before, rtx *after)
{
  struct address ad;
  enum machine_mode mode;
  rtx new_reg, *addr_loc, saved_index_reg, saved_base_reg, saved_base_reg2;
  bool ok_p;
  addr_space_t as;
  rtx op = *curr_id->operand_loc[nop];
  const char *constraint = curr_static_id->operand[nop].constraint;
  bool change_p;
  enum rtx_code code;

  if (constraint[0] == 'p'
      || EXTRA_ADDRESS_CONSTRAINT (constraint[0], constraint))
    {
      mode = VOIDmode;
      addr_loc = curr_id->operand_loc[nop];
      as = ADDR_SPACE_GENERIC;
      code = ADDRESS;
    }
  else if (MEM_P (op))
    {
      mode = GET_MODE (op);
      addr_loc = &XEXP (op, 0);
      as = MEM_ADDR_SPACE (op);
      code = MEM;
    }
  else if (GET_CODE (op) == SUBREG
	   && MEM_P (SUBREG_REG (op)))
    {
      mode = GET_MODE (SUBREG_REG (op));
      addr_loc = &XEXP (SUBREG_REG (op), 0);
      as = MEM_ADDR_SPACE (SUBREG_REG (op));
      code = MEM;
    }
  else
    return false;
  if (GET_CODE (*addr_loc) == AND)
    addr_loc = &XEXP (*addr_loc, 0);
  extract_address_regs (mode, as, addr_loc, code, &ad);
  saved_base_reg = saved_base_reg2 = saved_index_reg = NULL_RTX;
  change_p = equiv_address_substitution (&ad, addr_loc, mode, as, code);
  if (ad.base_reg_loc != NULL)
    {
      if (process_addr_reg (ad.base_reg_loc, before,
			    (ad.base_modify_p
			     && find_regno_note (curr_insn, REG_DEAD,
						 REGNO (*ad.base_reg_loc)) == NULL
			     ? after : NULL),
			    base_reg_class (mode, as, ad.base_outer_code,
					    ad.index_code)))
	change_p = true;
      if (ad.base_reg_loc2 != NULL)
	*ad.base_reg_loc2 = *ad.base_reg_loc;
      saved_base_reg = *ad.base_reg_loc;
      lra_eliminate_reg_if_possible (ad.base_reg_loc);
      if (ad.base_reg_loc2 != NULL)
	{
	  saved_base_reg2 = *ad.base_reg_loc2;
	  lra_eliminate_reg_if_possible (ad.base_reg_loc2);
	}
    }
  if (ad.index_reg_loc != NULL)
    {
      if (process_addr_reg (ad.index_reg_loc, before, NULL, INDEX_REG_CLASS))
	change_p = true;
      saved_index_reg = *ad.index_reg_loc;
      lra_eliminate_reg_if_possible (ad.index_reg_loc);
    }
  /* Some ports do not check displacements for virtual registers -- so
     we substitute them temporarily by real registers.  */
  ok_p = valid_address_p (mode, *addr_loc, as);
  if (saved_base_reg != NULL_RTX)
    {
      *ad.base_reg_loc = saved_base_reg;
      if (saved_base_reg2 != NULL_RTX)
	*ad.base_reg_loc = saved_base_reg2;
    }
  if (saved_index_reg != NULL_RTX)
    *ad.index_reg_loc = saved_index_reg;
  if (ok_p
      /* The following addressing is checked by constraints and
	 usually target specific legitimate address hooks do not
	 consider them valid.  */
      || GET_CODE (*addr_loc) == POST_DEC || GET_CODE (*addr_loc) == POST_INC
      || GET_CODE (*addr_loc) == PRE_DEC || GET_CODE (*addr_loc) == PRE_DEC
      || GET_CODE (*addr_loc) == PRE_MODIFY
      || GET_CODE (*addr_loc) == POST_MODIFY
      /* In this case we can not do anything becuase if it is wrong
	 that is because of wrong displacement.  Remember that any
	 address was legitimate in non-strict sense before LRA.  */
      || ad.disp_loc == NULL)
    return change_p;

  /* Addresses were legitimate before LRA.  So if the address has
     two registers than it can have two of them.  We should also
     not worry about scale for the same reason.  */
  push_to_sequence (*before);
  if (ad.base_reg_loc == NULL)
    {
      if (ad.index_reg_loc == NULL)
	{
	  /* disp => new_base  */
	  enum reg_class cl = base_reg_class (mode, as, SCRATCH, SCRATCH);
	  
	  new_reg = lra_create_new_reg (Pmode, NULL_RTX, cl, "disp");
	  lra_emit_move (new_reg, *ad.disp_loc);
	  *ad.disp_loc = new_reg;
	}
      else
	{
	  /* index * scale + disp => new base + index * scale  */
	  enum reg_class cl = base_reg_class (mode, as, SCRATCH, SCRATCH);

	  gcc_assert (INDEX_REG_CLASS != NO_REGS);
	  new_reg = lra_create_new_reg (Pmode, NULL_RTX, cl, "disp");
	  gcc_assert (GET_CODE (*addr_loc) == PLUS);
	  lra_emit_move (new_reg, *ad.disp_loc);
	  if (CONSTANT_P (XEXP (*addr_loc, 1)))
	    XEXP (*addr_loc, 1) = XEXP (*addr_loc, 0);
	  XEXP (*addr_loc, 0) = new_reg;
	  /* Some targets like ARM, accept address operands in
	     specific order -- try exchange them if necessary.  */
	  if (! valid_address_p (mode, *addr_loc, as))
	    {
	      exchange_plus_ops (*addr_loc);
	      if (! valid_address_p (mode, *addr_loc, as))
		exchange_plus_ops (*addr_loc);
	    }
	}
    }
  else if (ad.index_reg_loc == NULL)
    {
      /* We don't use transformation 'base + disp => base + new index'
	 because of some bad practice used in machine descriptions
	 (see comments for emit_spill_move).  */
      /* base + disp => new base  */
      new_reg = base_plus_disp_to_reg (mode, as, &ad);
      *addr_loc = new_reg;
    }
  else
    {
      /* base + scale * index + disp => new base + scale * index  */
      new_reg = base_plus_disp_to_reg (mode, as, &ad);
      *addr_loc = gen_rtx_PLUS (Pmode, new_reg, *ad.index_loc);
      if (! valid_address_p (mode, *addr_loc, as))
	{
	  /* Some targets like ARM, accept address operands in
	     specific order -- try exchange them if necessary.  */
	  exchange_plus_ops (*addr_loc);
	  if (! valid_address_p (mode, *addr_loc, as))
	    exchange_plus_ops (*addr_loc);
	}
    }
  *before = get_insns ();
  end_sequence ();
  return true;
}

/* Emit insns to reload VALUE into a new register.  VALUE is an
   autoincrement or autodecrement RTX whose operand is a register or
   memory location; so reloading involves incrementing that location.
   IN is either identical to VALUE, or some cheaper place to reload
   value being incremented/decremented from.

   INC_AMOUNT is the number to increment or decrement by (always
   positive and ignored for POST_MODIFY/PRE_MODIFY).

   Return pseudo containing the result.  */
static rtx
emit_inc (enum reg_class new_rclass, rtx in, rtx value, int inc_amount)
{
  /* REG or MEM to be copied and incremented.  */
  rtx incloc = XEXP (value, 0);
  /* Nonzero if increment after copying.  */
  int post = (GET_CODE (value) == POST_DEC || GET_CODE (value) == POST_INC
              || GET_CODE (value) == POST_MODIFY);
  rtx last;
  rtx inc;
  rtx add_insn;
  int code;
  rtx real_in = in == value ? incloc : in;
  rtx result;
  bool plus_p = true;

  if (GET_CODE (value) == PRE_MODIFY || GET_CODE (value) == POST_MODIFY)
    {
      gcc_assert (GET_CODE (XEXP (value, 1)) == PLUS
		  || GET_CODE (XEXP (value, 1)) == MINUS);
      gcc_assert (rtx_equal_p (XEXP (XEXP (value, 1), 0), XEXP (value, 0)));
      plus_p = GET_CODE (XEXP (value, 1)) == PLUS;
      inc = XEXP (XEXP (value, 1), 1);
    }
  else
    {
      if (GET_CODE (value) == PRE_DEC || GET_CODE (value) == POST_DEC)
        inc_amount = -inc_amount;

      inc = GEN_INT (inc_amount);
    }

  if (! post && REG_P (incloc))
    result = incloc;
  else
    result = lra_create_new_reg (GET_MODE (value), value, new_rclass,
				 "INC/DEC result");

  /* If this is post-increment, first copy the location to the reload reg.  */
  if (post && real_in != result)
    emit_insn (gen_move_insn (result, real_in));

  /* We suppose that there are insns to add/sub with the constant
     increment permitted in {PRE/POST)_{DEC/INC/MODIFY}.  At least the
     old reload worked with this assumption.  If the assumption
     becomes wrong, we should use approach in function
     base_plus_disp_to_reg.  */
  if (in == value)
    {
      /* See if we can directly increment INCLOC.  */
      last = get_last_insn ();
      add_insn = emit_insn (plus_p
			    ? gen_add2_insn (incloc, inc)
			    : gen_sub2_insn (incloc, inc));

      code = recog_memoized (add_insn);
      /* We should restore recog_data for the current insn.  */
      if (code >= 0)
	{
	  if (! post && result != incloc)
	    emit_insn (gen_move_insn (result, incloc));
	  return result;
	}
      delete_insns_since (last);
    }

  /* If couldn't do the increment directly, must increment in RESULT.
     The way we do this depends on whether this is pre- or
     post-increment.  For pre-increment, copy INCLOC to the reload
     register, increment it there, then save back.  */
  if (! post)
    {
      if (real_in != result)
	emit_insn (gen_move_insn (result, real_in));
      if (plus_p)
	emit_insn (gen_add2_insn (result, inc));
      else
	emit_insn (gen_sub2_insn (result, inc));
      if (result != incloc)
	emit_insn (gen_move_insn (incloc, result));
    }
  else
    {
      /* Postincrement.

	 Because this might be a jump insn or a compare, and because
	 RESULT may not be available after the insn in an input
	 reload, we must do the incrementation before the insn being
	 reloaded for.

	 We have already copied IN to RESULT.  Increment the copy in
	 RESULT, save that back, then decrement RESULT so it has
	 the original value.  */
      if (plus_p)
	emit_insn (gen_add2_insn (result, inc));
      else
	emit_insn (gen_sub2_insn (result, inc));
      emit_insn (gen_move_insn (incloc, result));
      /* Restore non-modified value for the result.  We prefer this
	 way because it does not require an addition hard
	 register.  */
      if (plus_p)
	{
	  if (CONST_INT_P (inc))
	    emit_insn (gen_add2_insn (result, GEN_INT (-INTVAL (inc))));
	  else
	    emit_insn (gen_sub2_insn (result, inc));
	}
      else if (CONST_INT_P (inc))
	emit_insn (gen_add2_insn (result, inc));
      else
	emit_insn (gen_add2_insn (result, inc));
    }
  return result;
}

/* Main entry point of this file: search the body of the current insn
   to choose the best alternative.  It is mimicking insn alternative
   cost calculation model of former reload pass.  That is because
   machine descriptions were written to use this model.  This model
   can be changed in future.  Make commutative operand exchange if it
   is chosen.

   Return true if some RTL changes happened during function call.  */
static bool
curr_insn_transform (void)
{
  int i, j, k;
  int n_operands;
  int n_alternatives;
  int n_dups;
  int commutative;
  signed char goal_alt_matched[MAX_RECOG_OPERANDS][MAX_RECOG_OPERANDS];
  rtx before, after;
  bool alt_p = false;
  /* Flag that the insn has been changed through a transformation.  */
  bool change_p;
  bool sec_mem_p;
#ifdef SECONDARY_MEMORY_NEEDED
  bool use_sec_mem_p;
#endif
  int max_regno_before;
  int reused_alternative_num;

  no_input_reloads_p = no_output_reloads_p = false;
  goal_alt_number = -1;

  if (check_and_process_move (&change_p, &sec_mem_p))
    return change_p;

  /* JUMP_INSNs and CALL_INSNs are not allowed to have any output
     reloads; neither are insns that SET cc0.  Insns that use CC0 are
     not allowed to have any input reloads.  */
  if (JUMP_P (curr_insn) || CALL_P (curr_insn))
    no_output_reloads_p = true;

#ifdef HAVE_cc0
  if (reg_referenced_p (cc0_rtx, PATTERN (curr_insn)))
    no_input_reloads_p = true;
  if (reg_set_p (cc0_rtx, PATTERN (curr_insn)))
    no_output_reloads_p = true;
#endif

  n_operands = curr_static_id->n_operands;
  n_alternatives = curr_static_id->n_alternatives;
  n_dups = curr_static_id->n_dups;

  /* Just return "no reloads" if insn has no operands with
     constraints.  */
  if (n_operands == 0 || n_alternatives == 0)
    return false;

  max_regno_before = max_reg_num ();

  for (i = 0; i < n_operands; i++)
    {
      goal_alt_matched[i][0] = -1;
      goal_alt_matches[i] = -1;
    }

  commutative = curr_static_id->commutative;

  /* Now see what we need for pseudo-regs that didn't get hard regs or
     got the wrong kind of hard reg.  For this, we must consider all
     the operands together against the register constraints.  */

  best_losers = best_overall = MAX_RECOG_OPERANDS * 2 + MAX_OVERALL_COST_BOUND;
  best_small_class_operands_num = best_reload_sum = 0;

  curr_swapped = false;
  goal_alt_swapped = false;

  /* Make equivalence substitution and memory subreg elimination
     before address processing because an address legitimacy can
     depend on memory mode.  */
  for (i = 0; i < n_operands; i++)
    {
      rtx op = *curr_id->operand_loc[i];
      rtx subst, old = op;
      bool op_change_p = false;

      if (GET_CODE (old) == SUBREG)
	old = SUBREG_REG (old);
      subst = get_equiv_substitution (old);
      if (subst != old)
	{
	  subst = copy_rtx (subst);
	  gcc_assert (REG_P (old));
	  if (GET_CODE (op) == SUBREG)
	    SUBREG_REG (op) = subst;
	  else
	    *curr_id->operand_loc[i] = subst;
	  if (lra_dump_file != NULL)
	    {
	      fprintf (lra_dump_file,
		       "Changing pseudo %d in operand %i of insn %u on equiv ",
		       REGNO (old), i, INSN_UID (curr_insn));
	      print_value_slim (lra_dump_file, subst, 1);
	      fprintf (lra_dump_file, "\n");
	    }
	  op_change_p = change_p = true;
	}
      if (simplify_operand_subreg (i, GET_MODE (old)) || op_change_p)
	{
	  change_p = true;
	  lra_update_dup (curr_id, i);
	}
    }

  /* Reload address registers and displacements.  We do it before
     finding an alternative because of memory constraints.  */
  before = after = NULL_RTX;
  for (i = 0; i < n_operands; i++)
    if (process_address (i, &before, &after))
      {
	change_p = true;
	lra_update_dup (curr_id, i);
      }
  
  if (change_p)
    /* Changes in the insn might result in that we can not satisfy
       constraints in lately used alternative of the insn.  */
    lra_set_used_insn_alternative (curr_insn, -1);

 try_swapped:

  reused_alternative_num = curr_id->used_insn_alternative;
  if (lra_dump_file != NULL && reused_alternative_num >= 0)
    fprintf (lra_dump_file, "Reusing alternative %d for insn #%u\n",
	     reused_alternative_num, INSN_UID (curr_insn));

  if (process_alt_operands (reused_alternative_num))
    alt_p = true;

  /* If insn is commutative (it's safe to exchange a certain pair of
     operands) then we need to try each alternative twice, the second
     time matching those two operands as if we had exchanged them.  To
     do this, really exchange them in operands.

     If we have just tried the alternatives the second time, return
     operands to normal and drop through.  */

  if (reused_alternative_num < 0 && commutative >= 0)
    {
      rtx x;

      curr_swapped = !curr_swapped;
      if (curr_swapped)
	{
	  x = *curr_id->operand_loc[commutative];
	  *curr_id->operand_loc[commutative]
	    = *curr_id->operand_loc[commutative + 1];
	  *curr_id->operand_loc[commutative + 1] = x;
	  /* Swap the duplicates too.  */
	  lra_update_dup (curr_id, commutative);
	  lra_update_dup (curr_id, commutative + 1);
	  goto try_swapped;
	}
      else
	{
	  x = *curr_id->operand_loc[commutative];
	  *curr_id->operand_loc[commutative]
	    = *curr_id->operand_loc[commutative + 1];
	  *curr_id->operand_loc[commutative + 1] = x;
	  lra_update_dup (curr_id, commutative);
	  lra_update_dup (curr_id, commutative + 1);
	}
    }

  /* The operands don't meet the constraints.  goal_alt describes the
     alternative that we could reach by reloading the fewest operands.
     Reload so as to fit it.  */

  if (! alt_p && ! sec_mem_p)
    {
      /* No alternative works with reloads??  */
      if (INSN_CODE (curr_insn) >= 0)
	fatal_insn ("unable to generate reloads for:", curr_insn);
      error_for_asm (curr_insn,
		     "inconsistent operand constraints in an %<asm%>");
      /* Avoid further trouble with this insn.  */
      PATTERN (curr_insn) = gen_rtx_USE (VOIDmode, const0_rtx);
      lra_invalidate_insn_data (curr_insn);
      return true;
    }

  /* If the best alternative is with operands 1 and 2 swapped, swap
     them.  Update the operand numbers of any reloads already
     pushed.  */

  if (goal_alt_swapped)
    {
      rtx tem;
      rtx x;
      int dup1, dup2;

      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "  Commutative operand exchange in insn %u\n",
		 INSN_UID (curr_insn));

      tem = *curr_id->operand_loc[commutative];
      *curr_id->operand_loc[commutative]
	= *curr_id->operand_loc[commutative + 1];
      *curr_id->operand_loc[commutative + 1] = tem;

      /* Swap the duplicates too.  */
      for (dup1 = dup2 = -1, i = 0; i < n_dups; i++)
	if (curr_static_id->dup_num[i] == commutative)
	  dup1 = i;
	else if  (curr_static_id->dup_num[i] == commutative + 1)
	  dup2 = i;
      
      if (dup1 >= 0 && dup2 >= 0)
	{
	  x = *curr_id->dup_loc[dup1];
	  *curr_id->dup_loc[dup1] = *curr_id->dup_loc[dup2];
	  *curr_id->dup_loc[dup2] = x;
	}

      change_p = true;
    }

#ifdef SECONDARY_MEMORY_NEEDED
  /* Some target macros SECONDARY_MEMORY_NEEDED (e.g. x86) are defined
     too conservatively.  So we use the secondary memory only if there
     is no any alternative without reloads.  */
  use_sec_mem_p = false;
  if (! alt_p)
    use_sec_mem_p = true;
  else if (sec_mem_p)
    {
      for (i = 0; i < n_operands; i++)
	if (! goal_alt_win[i] && ! goal_alt_match_win[i])
	  break;
      use_sec_mem_p = i < n_operands;
    }

  if (use_sec_mem_p)
    {
      rtx new_reg, set, src, dest;
      enum machine_mode sec_mode;

      gcc_assert (sec_mem_p);
      set = single_set (curr_insn);
      gcc_assert (set != NULL_RTX && ! side_effects_p (set));
      dest = SET_DEST (set);
      src = SET_SRC (set);
#ifdef SECONDARY_MEMORY_NEEDED_MODE
      sec_mode = SECONDARY_MEMORY_NEEDED_MODE (GET_MODE (src));
#else
      sec_mode = GET_MODE (src);
#endif
      new_reg = lra_create_new_reg (sec_mode, NULL_RTX,
				    NO_REGS, "secondary");
      /* If the mode is changed, it should be wider.  */
      gcc_assert (GET_MODE_SIZE (GET_MODE (new_reg))
		  >= GET_MODE_SIZE (GET_MODE (src)));
      after = emit_spill_move (false, new_reg, dest, INSN_CODE (curr_insn));
      lra_process_new_insns (curr_insn, NULL_RTX, after,
			     "Inserting the sec. move");
      before = emit_spill_move (true, new_reg, src, INSN_CODE (curr_insn));
      lra_process_new_insns (curr_insn, before, NULL_RTX, "Changing on");
      lra_set_insn_deleted (curr_insn);
      return true;
    }
#endif

  gcc_assert (goal_alt_number >= 0);
  lra_set_used_insn_alternative (curr_insn, goal_alt_number);

  if (lra_dump_file != NULL)
    {
      const char *p;

      fprintf (lra_dump_file, "  Choosing alt %d in insn %u:",
	       goal_alt_number, INSN_UID (curr_insn));
      for (i = 0; i < n_operands; i++)
	{
	  p = (curr_static_id->operand_alternative
	       [goal_alt_number * n_operands + i].constraint);
	  if (*p == '\0')
	    continue;
	  fprintf (lra_dump_file, "  (%d) ", i);
	  for (; *p != '\0' && *p != ',' && *p != '#'; p++)
	    fputc (*p, lra_dump_file);
	}
      fprintf (lra_dump_file, "\n");
    }

  /* Right now, for any pair of operands I and J that are required to
     match, with J < I, goal_alt_matches[I] is J.  Add I to
     goal_alt_matched[J].  */
  
  for (i = 0; i < n_operands; i++)
    if ((j = goal_alt_matches[i]) >= 0)
      {
	for (k = 0; goal_alt_matched[j][k] >= 0; k++)
	  ;
	/* We allows matching one output operand and several input
	   operands.  */
	gcc_assert (k == 0
		    || (curr_static_id->operand[j].type == OP_OUT
			&& curr_static_id->operand[i].type == OP_IN
			&& (curr_static_id->operand
			    [goal_alt_matched[j][0]].type == OP_IN)));
	goal_alt_matched[j][k] = i;
	goal_alt_matched[j][k + 1] = -1;
      }
  
  for (i = 0; i < n_operands; i++)
    goal_alt_win[i] |= goal_alt_match_win[i];
  
  /* Any constants that aren't allowed and can't be reloaded into
     registers are here changed into memory references.  */
  for (i = 0; i < n_operands; i++)
    if (goal_alt_win[i])
      {
	int regno;
	enum reg_class new_class;
	rtx reg = *curr_id->operand_loc[i];

	if (GET_CODE (reg) == SUBREG)
	  reg = SUBREG_REG (reg);
	    
	if (REG_P (reg) && (regno = REGNO (reg)) >= FIRST_PSEUDO_REGISTER)
	  {
	    bool ok_p = in_class_p (regno, goal_alt[i], &new_class);

	    if (new_class != NO_REGS && get_reg_class (regno) != new_class)
	      {
		gcc_assert (ok_p);
		change_class (regno, new_class, "      Change", true);
	      }
	  }
      }
    else
      {
	const char *constraint;
	char c;
	rtx op = *curr_id->operand_loc[i];
	rtx subreg = NULL_RTX;
	rtx plus = NULL_RTX;
	enum machine_mode mode = get_op_mode (i);
	
	if (GET_CODE (op) == SUBREG)
	  {
	    subreg = op;
	    op = SUBREG_REG (op);
	    mode = GET_MODE (op);
	  }
	
	if (GET_CODE (op) == PLUS)
	  {
	    plus = op;
	    op = XEXP (op, 1);
	  }
	
	if (CONST_POOL_OK_P (mode, op)
	    && ((targetm.preferred_reload_class
		 (op, (enum reg_class) goal_alt[i]) == NO_REGS)
		|| no_input_reloads_p)
	    && mode != VOIDmode)
	  {
	    rtx tem = force_const_mem (mode, op);
	    
	    change_p = true;
	    /* If we stripped a SUBREG or a PLUS above add it back.  */
	    if (plus != NULL_RTX)
	      tem = gen_rtx_PLUS (mode, XEXP (plus, 0), tem);
	    
	    if (subreg != NULL_RTX)
	      tem = gen_rtx_SUBREG (mode, tem, SUBREG_BYTE (subreg));
	    
	    *curr_id->operand_loc[i] = tem;
	    lra_update_dup (curr_id, i);
	    process_address (i, &before, &after);
	    
	    /* If the alternative accepts constant pool refs directly
	       there will be no reload needed at all.  */
	    if (plus != NULL_RTX || subreg != NULL_RTX)
	      continue;
	    /* Skip alternatives before the one requested.  */
	    constraint = (curr_static_id->operand_alternative
			  [goal_alt_number * n_operands + i].constraint);
	    for (;
		 (c = *constraint) && c != ',' && c != '#';
		 constraint += CONSTRAINT_LEN (c, constraint))
	      {
		if (c == TARGET_MEM_CONSTRAINT || c == 'o')
		  break;
#ifdef EXTRA_CONSTRAINT_STR
		if (EXTRA_MEMORY_CONSTRAINT (c, constraint)
		    && EXTRA_CONSTRAINT_STR (tem, c, constraint))
		  break;
#endif
	      }
	    if (c == '\0' || c == ',' || c == '#')
	      continue;
	    
	    goal_alt_win[i] = true;
	  }
      }
  
  for (i = 0; i < n_operands; i++)
    {
      rtx old, new_reg;
      rtx op = *curr_id->operand_loc[i];

      if (goal_alt_win[i])
	{
	  if (goal_alt[i] == NO_REGS
	      && REG_P (op)
	      && lra_former_scratch_operand_p (curr_insn, i))
	    change_class (REGNO (op), NO_REGS, "      Change", true);
	  continue;
	}
      
      /* Operands that match previous ones have already been handled.  */
      if (goal_alt_matches[i] >= 0)
	continue;

      /* We should not have an operand with a nonoffsettable address
	 appearing where an offsettable address will do.  It also may
	 be a case when the address should be special in other words
	 not a general one (e.g. it needs no index reg).  */
      if (goal_alt_matched[i][0] == -1 && goal_alt_offmemok[i] && MEM_P (op))
	{
	  enum reg_class rclass;
	  rtx *loc = &XEXP (op, 0);
	  enum rtx_code code = GET_CODE (*loc);

	  push_to_sequence (before);
	  rclass = base_reg_class (GET_MODE (op), MEM_ADDR_SPACE (op),
				   MEM, SCRATCH);
	  if (code == PRE_DEC || code == POST_DEC
	      || code == PRE_INC || code == POST_INC
	      || code == PRE_MODIFY || code == POST_MODIFY)
	    new_reg = emit_inc (rclass, *loc, *loc,
				/* This value does not matter for MODIFY.  */
				GET_MODE_SIZE (GET_MODE (op)));
	  else if (get_reload_reg (OP_IN, Pmode, *loc, rclass,
				   "offsetable address", &new_reg))
	    lra_emit_move (new_reg, *loc);
	  before = get_insns ();
	  end_sequence ();
	  *loc = new_reg;
	  lra_update_dup (curr_id, i);
	}
      else if (goal_alt_matched[i][0] == -1)
	{
	  enum machine_mode mode;
	  rtx reg, *loc;
	  int hard_regno, byte;
	  enum op_type type = curr_static_id->operand[i].type;

	  loc = curr_id->operand_loc[i];
	  mode = get_op_mode (i);
	  if (GET_CODE (*loc) == SUBREG)
	    {
	      reg = SUBREG_REG (*loc);
	      byte = SUBREG_BYTE (*loc);
	      if (REG_P (reg)
		  /* Strict_low_part requires reload the register not
		     the sub-register.  */
		  && (curr_static_id->operand[i].strict_low
		      || (GET_MODE_SIZE (mode)
			  <= GET_MODE_SIZE (GET_MODE (reg))
			  && (hard_regno
			      = get_try_hard_regno (REGNO (reg))) >= 0
			  && (simplify_subreg_regno
			      (hard_regno,
			       GET_MODE (reg), byte, mode) < 0)
			  && (goal_alt[i] == NO_REGS
			      || (simplify_subreg_regno
				  (ira_class_hard_regs[goal_alt[i]][0],
				   GET_MODE (reg), byte, mode) >= 0)))))
		{
		  loc = &SUBREG_REG (*loc);
		  mode = GET_MODE (*loc);
		}
	    }
	  old = *loc;
	  if (get_reload_reg (type, mode, old, goal_alt[i], "", &new_reg)
	      && type != OP_OUT)
	    {
	      push_to_sequence (before);
	      lra_emit_move (new_reg, *loc);
	      before = get_insns ();
	      end_sequence ();
	    }
	  *loc = new_reg;
	  if (type != OP_IN)
	    {
	      if (find_reg_note (curr_insn, REG_UNUSED, old) == NULL_RTX)
		{
		  push_to_sequence (after);
		  lra_emit_move (old, new_reg);
		  after = get_insns ();
		  end_sequence ();
		}
	      *loc = new_reg;
	    }
	  for (j = 0; j < goal_alt_dont_inherit_ops_num; j++)
	    if (goal_alt_dont_inherit_ops[j] == i)
	      {
		lra_set_regno_unique_value (REGNO (new_reg));
		break;
	      }
	  lra_update_dup (curr_id, i);
	}
      else if (curr_static_id->operand[i].type == OP_IN
	       && curr_static_id->operand[goal_alt_matched[i][0]].type == OP_OUT)
	{
	  signed char arr[2];

	  arr[0] = i;
	  arr[1] = -1;
	  match_reload (goal_alt_matched[i][0], arr,
			goal_alt[i], &before, &after);
	}
      else if (curr_static_id->operand[i].type == OP_OUT
	       && curr_static_id->operand[goal_alt_matched[i][0]].type == OP_IN)
	match_reload (i, goal_alt_matched[i], goal_alt[i], &before, &after);
      else
	{
	  gcc_assert (INSN_CODE (curr_insn) < 0);
	  error_for_asm (curr_insn,
			 "inconsistent operand constraints in an %<asm%>");
	  /* Avoid further trouble with this insn.  */
	  PATTERN (curr_insn) = gen_rtx_USE (VOIDmode, const0_rtx);
	  return false;
	}
    }
  if (before != NULL_RTX || after != NULL_RTX
      || max_regno_before != max_reg_num ())
    change_p = true;
  if (change_p)
    {
      /* Process operator duplications.  We do it here to guarantee
	 their processing after operands processing.  Generally
	 speaking, we could do this probably in the previous loop
	 because a common practice is to enumerate the operators after
	 their operands.  */
      for (i = 0; i < n_dups; i++)
	{
	  int ndup = curr_static_id->dup_num[i];

	  if (curr_static_id->operand[ndup].is_operator)
	    *curr_id->dup_loc[i] = *curr_id->operand_loc[ndup];
	}
      /* Something changes -- process the insn.  */
      lra_update_insn_regno_info (curr_insn);
    }
  lra_process_new_insns (curr_insn, before, after, "Inserting insn reload");
  return change_p;
}

/* Return true i X is in LIST.  */
static bool
in_list_p (rtx x, rtx list)
{
  for (; list != NULL_RTX; list = XEXP (list, 1))
    if (XEXP (list, 0) == x)
      return true;
  return false;
}

/* Return true if X contains an allocatable hard register (if
   HARD_REG_P) or a (spilled if SPILLED_P) pseudo.  */
static bool
contains_reg_p (rtx x, bool hard_reg_p, bool spilled_p)
{
  int i, j;
  const char *fmt;
  enum rtx_code code;

  code = GET_CODE (x);
  if (REG_P (x))
    {
      int regno = REGNO (x);
      HARD_REG_SET alloc_regs;

      if (hard_reg_p)
	{
	  if (regno >= FIRST_PSEUDO_REGISTER)
	    regno = lra_get_regno_hard_regno (regno);
	  if (regno < 0)
	    return false;
	  COMPL_HARD_REG_SET (alloc_regs, lra_no_alloc_regs);
	  return lra_hard_reg_set_intersection_p (regno, GET_MODE (x),
						  alloc_regs);
	}
      else
	{
	  if (regno < FIRST_PSEUDO_REGISTER)
	    return false;
	  if (! spilled_p)
	    return true;
	  return lra_get_regno_hard_regno (regno) < 0;
	}
    }
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  if (contains_reg_p (XEXP (x, i), hard_reg_p, spilled_p))
	    return true;
	}
      else if (fmt[i] == 'E')
	{
	  for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	    if (contains_reg_p (XVECEXP (x, i, j), hard_reg_p, spilled_p))
	      return true;
	}
    }
  return false;
}

/* Process all regs in debug location *LOC and change them on
   equivalent substitution.  Return true if any change was done.  */
static bool
debug_loc_equivalence_change_p (rtx *loc)
{
  rtx subst, reg, x = *loc;
  bool result = false;
  enum rtx_code code = GET_CODE (x);
  const char *fmt;
  int i, j;

  if (code == SUBREG)
    {
      reg = SUBREG_REG (x);
      if ((subst = get_equiv_substitution (reg)) != reg
	  && GET_MODE (subst) == VOIDmode)
	{
	  /* We cannot reload debug location.  Simplify subreg here
	     while we know the inner mode.  */
	  *loc = simplify_gen_subreg (GET_MODE (x), subst,
				      GET_MODE (reg), SUBREG_BYTE (x));
	  return true;
	}
    }
  if (code == REG && (subst = get_equiv_substitution (x)) != x)
    {
      *loc = subst;
      return true;
    }

  /* Scan all the operand sub-expressions.  */
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	result = debug_loc_equivalence_change_p (&XEXP (x, i)) || result;
      else if (fmt[i] == 'E')
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  result
	    = debug_loc_equivalence_change_p (&XVECEXP (x, i, j)) || result;
    }
  return result;
}

/* Maximum number of constraint pass iteration number.  It is for
   preventing all LRA cycling.  */
#define MAX_CONSTRAINT_ITERATION_NUMBER 15

/* Maximum number of generated reload insns per an insn.  It is for
   preventing this pass cycling.  */
#define MAX_RELOAD_INSNS_NUMBER LRA_MAX_INSN_RELOADS

/* The current iteration number of this LRA pass.  */
int lra_constraint_iter;

/* True if we substituted equiv which needs checking register
   allocation correctness because the equivalent value contains
   allocatiable hard registers or when we restore multi-register
   pseudo.  */
bool lra_risky_transformations_p;

/* Entry function of LRA constraint pass.  Return true if the
   constraint pass did change the code.  */
bool
lra_constraints (bool first_p)
{
  bool changed_p;
  int i, hard_regno, new_insns_num;
  unsigned int min_len;
  rtx set, x, dest_reg;
  basic_block last_bb;

  lra_constraint_iter++;
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file, "\n********** Local #%d: **********\n\n",
	     lra_constraint_iter);
  if (lra_constraint_iter > MAX_CONSTRAINT_ITERATION_NUMBER)
    internal_error
      ("Maximum number of LRA constraint passes is achieved (%d)\n",
       MAX_CONSTRAINT_ITERATION_NUMBER);
  changed_p = false;
  lra_risky_transformations_p = false;
  new_insn_uid_start = get_max_uid ();
  new_regno_start = first_p ? lra_constraint_new_regno_start : max_reg_num ();
  for (i = FIRST_PSEUDO_REGISTER; i < new_regno_start; i++)
    ira_reg_equiv[i].profitable_p = true;
  for (i = FIRST_PSEUDO_REGISTER; i < new_regno_start; i++)
    if (lra_reg_info[i].nrefs != 0)
      {
	if ((hard_regno = lra_get_regno_hard_regno (i)) >= 0)
	  {
	    int j, nregs = hard_regno_nregs[hard_regno][PSEUDO_REGNO_MODE (i)];
	    
	    for (j = 0; j < nregs; j++)
	      df_set_regs_ever_live (hard_regno + j, true);
	  }
	else if ((x = get_equiv_substitution (regno_reg_rtx[i])) != NULL_RTX)
	  {
	    if (! first_p && contains_reg_p (x, false, false))
	      /* After RTL transformation, we can not guarantee that
		 pseudo in the susbtitution was not reloaded which
		 might make equivalence invalid.  For example, in
		 reverse equiv of p0

		 p0 <- ...
		 ...
		 equiv_mem <- p0

		 the memory address register was reloaded before the
		 2nd insn.  */
	      ira_reg_equiv[i].defined_p = false;
	    if (contains_reg_p (x, false, true))
	      ira_reg_equiv[i].profitable_p = false;
	  }
      }
  lra_eliminate (false);
  min_len = VEC_length (rtx, lra_constraint_insn_stack);
  new_insns_num = 0;
  last_bb = NULL;
  changed_p = false;
  for (;VEC_length (rtx, lra_constraint_insn_stack) != 0;)
    {
      curr_insn = VEC_pop (rtx, lra_constraint_insn_stack);
      bitmap_clear_bit (&lra_constraint_insn_stack_bitmap,
			INSN_UID (curr_insn));
      curr_bb = BLOCK_FOR_INSN (curr_insn); 
      if (curr_bb != last_bb)
	{
	  last_bb = curr_bb;
	  bb_reload_num = lra_curr_reload_num;
	}
      if (min_len > VEC_length (rtx, lra_constraint_insn_stack))
	{
	  min_len = VEC_length (rtx, lra_constraint_insn_stack);
	  new_insns_num = 0;
	}
      if (new_insns_num > MAX_RELOAD_INSNS_NUMBER)
	internal_error
	  ("Max. number of generated reload insns per insn is achieved (%d)\n",
	   MAX_RELOAD_INSNS_NUMBER);
      new_insns_num++;
      if (DEBUG_INSN_P (curr_insn))
	{
	  /* We need to check equivalence in debug insn and change
	     pseudo to the equivalent value if necessary.  */
	  curr_id = lra_get_insn_recog_data (curr_insn);
	  if (debug_loc_equivalence_change_p (curr_id->operand_loc[0]))
	    changed_p = true;
	}
      else if (INSN_P (curr_insn))
	{
	  if ((set = single_set (curr_insn)) != NULL_RTX)
	    {
	      dest_reg = SET_DEST (set);
	      /* The equivalence pseudo could be set up as SUBREG in a
		 case when it is a call restore insn in a mode
		 different from the pseudo mode.  */
	      if (GET_CODE (dest_reg) == SUBREG)
		dest_reg = SUBREG_REG (dest_reg);
	      if ((REG_P (dest_reg)
		   && (x = get_equiv_substitution (dest_reg)) != dest_reg
		   /* Remove insns which set up a pseudo whose value
		      can not be changed.  Such insns might be not in
		      init_insns because we don't update equiv data
		      during insn transformations.
			  
		      As an example, let suppose that a pseudo got
		      hard register and on the 1st pass was not
		      changed to equivalent constant.  We generate an
		      additional insn setting up the pseudo because of
		      secondary memory movement.  Then the pseudo is
		      spilled and we use the equiv constant.  In this
		      case we should remove the additional insn and
		      this insn is not init_insns list.  */
		   && (! MEM_P (x) || MEM_READONLY_P (x)
		       || in_list_p (curr_insn,
				     ira_reg_equiv
				     [REGNO (dest_reg)].init_insns)))
		  || ((x = get_equiv_substitution (SET_SRC (set))) != SET_SRC (set)
		      && in_list_p (curr_insn,
				    ira_reg_equiv
				    [REGNO (SET_SRC (set))].init_insns)))
		{
		  /* This is equiv init insn of pseudo which did not get a
		     hard register -- remove the insn.  */
		  if (lra_dump_file != NULL)
		    {
		      fprintf (lra_dump_file,
			       "      Removing equiv init insn %i (freq=%d)\n",
			       INSN_UID (curr_insn),
			       BLOCK_FOR_INSN (curr_insn)->frequency);
		      print_rtl_slim (lra_dump_file, curr_insn, curr_insn, -1, 0);
		    }
		  if (contains_reg_p (x, true, false))
		    lra_risky_transformations_p = true;
		  lra_set_insn_deleted (curr_insn);
		  continue;
		}
	    }
	  curr_id = lra_get_insn_recog_data (curr_insn);
	  curr_static_id = curr_id->insn_static_data;
	  init_curr_insn_input_reloads ();
	  if (curr_insn_transform ())
	    changed_p = true;
	}
    }
  /* If we used a new hard regno, changed_p should be true because the
     hard reg is assigned to a new pseudo.  */
#ifdef ENABLE_CHECKING
  if (! changed_p)
    {
      for (i = FIRST_PSEUDO_REGISTER; i < new_regno_start; i++)
	if (lra_reg_info[i].nrefs != 0
	    && (hard_regno = lra_get_regno_hard_regno (i)) >= 0)
	  {
	    int j, nregs = hard_regno_nregs[hard_regno][PSEUDO_REGNO_MODE (i)];
	    
	    for (j = 0; j < nregs; j++)
	      gcc_assert (df_regs_ever_live_p (hard_regno + j));
	  }
    }
#endif
  return changed_p;
}

/* Initiate the LRA constraint pass.  It is done once per
   function.  */
void
lra_contraints_init (void)
{
  init_indirect_mem ();
  bitmap_initialize (&lra_matched_pseudos, &reg_obstack);
  bitmap_initialize (&lra_bound_pseudos, &reg_obstack);
}

/* Finalize the LRA constraint pass.  It is done once per
   function.  */
void
lra_contraints_finish (void)
{
  bitmap_clear (&lra_bound_pseudos);
  bitmap_clear (&lra_matched_pseudos);
}



/* This page contains code to do inheritance/split
   transformations.  */

/* Number of reloads passed so far in current EBB.  */
static int reloads_num;

/* Number of calls passed so far in current EBB.  */
static int calls_num;

/* Current reload pseudo check for validity of elements in
   USAGE_INSNS.  */
static int curr_usage_insns_check;

/* Info about last usage of pseudos in EBB to do inheritance/split
   transformation.  Inheritance transformation is done from a spilled
   pseudo and split transformations from a pseudo to assigned to a
   hard register.  */
struct usage_insns
{
  /* If the value is equal to the above variable value, then the INSNS
     is valid.  The insns is chain of optional debug insns and a
     finishing non-debug insn using the corresponding pseudo.  */
  int check;
  /* Value of global reloads_num at the corresponding next insns.  */
  int reloads_num;
  /* Value of global reloads_num at the corresponding next insns.  */
  int calls_num;
  /* It can be true only for splitting.  And it means that the restore
     insn should be put after insn give by the following member.  */
  bool after_p;
  /* Next insns in the current EBB which use the original pseudo and
     the original pseudo value is not changed between the current insn
     and the next insns.  In order words, if we need to use the
     original pseudo value again in the next insns we can try to use
     the value in a hard register from a reload insn of the current
     insn.  */
  rtx insns;
};

/* Map: pseudo regno -> corresponding pseudo usage insns.  */
static struct usage_insns *usage_insns;

/* Process all regs OLD_REGNO in location *LOC and change them on the
   reload pseudo NEW_REG.  Return true if any change was done.  */
static bool
substitute_pseudo (rtx *loc, int old_regno, rtx new_reg)
{
  rtx x = *loc;
  bool result = false;
  enum rtx_code code;
  const char *fmt;
  int i, j;

  if (x == NULL_RTX)
    return false;

  code = GET_CODE (x);
  if (code == REG && (int) REGNO (x) == old_regno)
    {
      *loc = new_reg;
      return true;
    }

  /* Scan all the operand sub-expressions.  */
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  if (substitute_pseudo (&XEXP (x, i), old_regno, new_reg))
	    result = true;
	}
      else if (fmt[i] == 'E')
	{
	  for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	    if (substitute_pseudo (&XVECEXP (x, i, j), old_regno, new_reg))
	      result = true;
	}
    }
  return result;
}

/* Pseudos involved in inheritance/split in the current EBB
   (inheritance/split and original pseudos).  */
static bitmap_head check_only_pseudos;

/* Do inheritance transformation for insn INSN defining (if DEF_P) or
   using ORIGINAL_REGNO where the subsequent insn(s) in EBB (remember
   we traverse insns in the backward direction) for the original regno
   is NEXT_USAGE_INSNS.  The transformations look like

     p <- ...             i <- ...
     ...                  p <- i    (new insn)
     ...             =>
     <- ... p ...         <- ... i ...
   or
     ...                  i <- p    (new insn)
     <- ... p ...         <- ... i ...
     ...             =>
     <- ... p ...         <- ... i ...
   where p is spilled original pseudo and i is a new inheritance pseudo.
   
   The inheritance pseudo has the smallest class of two classes CL and
   class of ORIGINAL REGNO.  It will have unique value if UNIQ_P.  The
   unique value is necessary for correct assignment to inheritance
   pseudo for input of an insn which should be the same as output
   (bound pseudos).  Return true if we succeed in such
   transformation.  */
static bool
inherit_reload_reg (bool def_p, bool uniq_p, int original_regno,
		    enum reg_class cl, rtx insn, rtx next_usage_insns)
{
  enum reg_class rclass = lra_get_allocno_class (original_regno);
  rtx original_reg = regno_reg_rtx[original_regno];
  rtx new_reg, new_insns, usage_insn;

  gcc_assert (! usage_insns[original_regno].after_p);
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file,
	     "    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
  if (! ira_reg_classes_intersect_p[cl][rclass])
    {
      if (lra_dump_file != NULL)
	{
	  fprintf (lra_dump_file,
		   "    Rejecting inheritance for %d "
		   "because of too different classes %s and %s\n",
		   original_regno, reg_class_names[cl],
		   reg_class_names[rclass]);
	  fprintf (lra_dump_file,
		   "    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	}
      return false;
    }
  if ((ira_class_subset_p[cl][rclass] && cl != rclass)
      || ira_available_class_regs[cl] < ira_available_class_regs[rclass])
    {
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "    Use smallest class of %s and %s\n",
		 reg_class_names[cl], reg_class_names[rclass]);
      
      rclass = cl;
    }
  if (uniq_p)
    new_reg = lra_create_new_reg_with_unique_value (GET_MODE (original_reg),
						    original_reg,
						    rclass, "inheritance");
  else
    new_reg = lra_create_new_reg (GET_MODE (original_reg), original_reg,
				  rclass, "inheritance");
  start_sequence ();
  if (def_p)
    emit_move_insn (original_reg, new_reg);
  else
    emit_move_insn (new_reg, original_reg);
  new_insns = get_insns ();
  end_sequence ();
  if (NEXT_INSN (new_insns) != NULL_RTX)
    {
      if (lra_dump_file != NULL)
	{
	  fprintf (lra_dump_file,
		   "    Rejecting inheritance %d->%d "
		   "as it results in 2 or more insns:\n",
		   original_regno, REGNO (new_reg));
	  print_rtl_slim (lra_dump_file, new_insns, NULL_RTX, -1, 0);
	  fprintf (lra_dump_file,
		   "    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	}
      return false;
    }
  substitute_pseudo (&insn, original_regno, new_reg);
  lra_update_insn_regno_info (insn);
  if (! def_p)
    {
      /* We now have a new usage insn for original regno.  */
      usage_insns[original_regno].check = curr_usage_insns_check;
      usage_insns[original_regno].insns = new_insns;
      usage_insns[original_regno].reloads_num = reloads_num;
      usage_insns[original_regno].calls_num = calls_num;
      usage_insns[original_regno].after_p = false;
    }
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file, "    Original reg change %d->%d:\n",
	     original_regno, REGNO (new_reg));
  lra_reg_info[REGNO (new_reg)].restore_regno = original_regno;
  bitmap_set_bit (&check_only_pseudos, REGNO (new_reg));
  bitmap_set_bit (&check_only_pseudos, original_regno);
  bitmap_set_bit (&lra_inheritance_pseudos, REGNO (new_reg));
  if (def_p)
    lra_process_new_insns (insn, NULL_RTX, new_insns,
			   "Add original<-inheritance");
  else
    lra_process_new_insns (insn, new_insns, NULL_RTX,
			   "Add inheritance<-pseudo");
  while (next_usage_insns != NULL_RTX)
    {
      if (GET_CODE (next_usage_insns) != INSN_LIST)
	{
	  usage_insn = next_usage_insns;
	  gcc_assert (NONDEBUG_INSN_P (usage_insn));
	  next_usage_insns = NULL;
	}
      else
	{
	  usage_insn = XEXP (next_usage_insns, 0);
	  gcc_assert (DEBUG_INSN_P (usage_insn));
	  next_usage_insns = XEXP (next_usage_insns, 1);
	}
      substitute_pseudo (&usage_insn, original_regno, new_reg);
      lra_update_insn_regno_info (usage_insn);
      if (lra_dump_file != NULL)
	{
	  fprintf (lra_dump_file, "    Inheritance reuse change %d->%d:\n",
		   original_regno, REGNO (new_reg));
	  print_rtl_slim (lra_dump_file, usage_insn, usage_insn,
			  -1, 0);
	}
    }
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file,
	     "    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  return true;
}

/* Return true if we need a caller save/restore for pseudo REGNO which
   was assigned to a hard register.  */
static inline bool
need_for_call_save_p (int regno)
{
  gcc_assert (reg_renumber[regno] >= 0);
  return (usage_insns[regno].calls_num < calls_num
	  && (lra_hard_reg_set_intersection_p
	      (reg_renumber[regno], PSEUDO_REGNO_MODE (regno),
	       call_used_reg_set)));
}

/* Return true if we need a split for pseudo REGNO which was assigned
   to a hard register.  POTENTIAL_RELOAD_HARD_REGS contains hard
   registers which might be used for reloads since the EBB end.  It is
   an approximation of the used hard registers in the split range.
   The exact value would require expensive calculations.  If we were
   aggressive with splitting because of the approximation, the split
   pseudo will save the same hard register assignment and will be
   removed in the undo pass.  We still need the approximation because
   too aggressive splitting would result in too inaccurate cost
   calculation in the assignment pass because of too many generated
   moves which will be probably removed in the undo pass.  */
static inline bool
need_for_split_p (HARD_REG_SET potential_reload_hard_regs, int regno)
{
  gcc_assert (reg_renumber[regno] >= 0);
  return ((TEST_HARD_REG_BIT (potential_reload_hard_regs, reg_renumber[regno])
	   && usage_insns[regno].reloads_num + 1 < reloads_num)
	  || need_for_call_save_p (regno));
}

/* Return class for the split pseudo created from original pseudo with
   ALLOCNO_CLASS and MODE which got a hard register HARD_REGNO We
   choose subclass of ALLOCNO_CLASS which contains HARD_REGNO and
   results in no secondary memory movements.  */
static enum reg_class
choose_split_class (enum reg_class allocno_class,
		    int hard_regno ATTRIBUTE_UNUSED,
		    enum machine_mode mode ATTRIBUTE_UNUSED)
{
#ifndef SECONDARY_MEMORY_NEEDED
  return allocno_class;
#else
  int i;
  enum reg_class cl, best_cl = NO_REGS;
  enum reg_class hard_reg_class = REGNO_REG_CLASS (hard_regno);
  
  if (! SECONDARY_MEMORY_NEEDED (allocno_class, allocno_class, mode)
      && TEST_HARD_REG_BIT (reg_class_contents[allocno_class], hard_regno))
    return allocno_class;
  for (i = 0;
       (cl = reg_class_subclasses[allocno_class][i]) != LIM_REG_CLASSES;
       i++)
    if (! SECONDARY_MEMORY_NEEDED (cl, hard_reg_class, mode)
	&& ! SECONDARY_MEMORY_NEEDED (hard_reg_class, cl, mode)
	&& TEST_HARD_REG_BIT (reg_class_contents[cl], hard_regno)
	&& (best_cl == NO_REGS
	    || (hard_reg_set_subset_p (reg_class_contents[best_cl],
				       reg_class_contents[cl])
		&& ! hard_reg_set_equal_p (reg_class_contents[best_cl],
					   reg_class_contents[cl]))))
      best_cl = cl;
  return best_cl;
#endif
}

/* Do split transformation for insn INSN defining or
   using ORIGINAL_REGNO where the subsequent insn(s) in EBB (remember
   we traverse insns in the backward direction) for the original regno
   is NEXT_USAGE_INSNS.  The transformations look like

     p <- ...             p <- ...
     ...                  s <- p    (new insn -- save)
     ...             =>
     ...                  p <- s    (new insn -- restore)
     <- ... p ...         <- ... p ...
   or
     <- ... p ...         <- ... p ...
     ...                  s <- p    (new insn -- save)
     ...             =>
     ...                  p <- s    (new insn -- restore)
     <- ... p ...         <- ... p ...

   where p is an original pseudo got a hard register and s is a new
   split pseudo.  The save is put before INSN if BEFORE_P is true.
   Return true if we succeed in such transformation.  */
static bool
split_pseudo (bool before_p, int original_regno, rtx insn, rtx next_usage_insns)
{
  enum reg_class rclass = lra_get_allocno_class (original_regno);
  rtx original_reg = regno_reg_rtx[original_regno];
  rtx new_reg, save, restore, usage_insn;
  bool after_p;
  bool call_save_p = need_for_call_save_p (original_regno);

  gcc_assert (reg_renumber[original_regno] >= 0);
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file,
	     "    ((((((((((((((((((((((((((((((((((((((((((((((((\n");
  if (call_save_p)
    {
      enum machine_mode sec_mode;
      
#ifdef SECONDARY_MEMORY_NEEDED_MODE
      sec_mode = SECONDARY_MEMORY_NEEDED_MODE (GET_MODE (original_reg));
#else
      sec_mode = GET_MODE (original_reg);
#endif
      new_reg = lra_create_new_reg (sec_mode, NULL_RTX,
				    NO_REGS, "save");
    }
  else
    {
      rclass = choose_split_class (rclass,
				   reg_renumber[original_regno],
				   GET_MODE (original_reg));
      if (rclass == NO_REGS)
	{
	  if (lra_dump_file != NULL)
	    {
	      fprintf (lra_dump_file,
		       "    Rejecting split of %d(%s): "
		       "no good reg class for %d(%s)\n",
		       original_regno,
		       reg_class_names[lra_get_allocno_class (original_regno)],
		       reg_renumber[original_regno],
		       reg_class_names[REGNO_REG_CLASS
				       (reg_renumber[original_regno])]);
	      fprintf (lra_dump_file,
		       "    ))))))))))))))))))))))))))))))))))))))))))))))))\n");
	    }
	  return false;
	}
      new_reg = lra_create_new_reg (GET_MODE (original_reg), original_reg,
				    rclass, "split");
      reg_renumber[REGNO (new_reg)] = reg_renumber[original_regno];
    }
  if (call_save_p)
    save = emit_spill_move (true, new_reg, original_reg, -1);
  else
    {
      start_sequence ();
      emit_move_insn (new_reg, original_reg);
      save = get_insns ();
      end_sequence ();
    }
  if (NEXT_INSN (save) != NULL_RTX)
    {
      gcc_assert (! call_save_p);
      if (lra_dump_file != NULL)
	{
	  fprintf (lra_dump_file,
		   "    Rejecting split %d->%d resulting in > 2 %s save insns:\n",
		   original_regno, REGNO (new_reg), call_save_p ? "call" : "");
	  print_rtl_slim (lra_dump_file, save, NULL_RTX, -1, 0);
	  fprintf (lra_dump_file,
		   "    ))))))))))))))))))))))))))))))))))))))))))))))))\n");
	}
      return false;
    }
  if (call_save_p)
    restore = emit_spill_move (false, new_reg, original_reg, -1);
  else
    {
      start_sequence ();
      emit_move_insn (original_reg, new_reg);
      restore = get_insns ();
      end_sequence ();
    }
  if (NEXT_INSN (restore) != NULL_RTX)
    {
      gcc_assert (! call_save_p);
      if (lra_dump_file != NULL)
	{
	  fprintf (lra_dump_file,
		   "    Rejecting split %d->%d "
		   "resulting in > 2 %s restore insns:\n",
		   original_regno, REGNO (new_reg), call_save_p ? "call" : "");
	  print_rtl_slim (lra_dump_file, restore, NULL_RTX, -1, 0);
	  fprintf (lra_dump_file,
		   "    ))))))))))))))))))))))))))))))))))))))))))))))))\n");
	}
      return false;
    }
  after_p = usage_insns[original_regno].after_p;
  lra_reg_info[REGNO (new_reg)].restore_regno = original_regno;
  bitmap_set_bit (&check_only_pseudos, REGNO (new_reg));
  bitmap_set_bit (&check_only_pseudos, original_regno);
  bitmap_set_bit (&lra_split_pseudos, REGNO (new_reg));
  for (;;)
    {
      if (GET_CODE (next_usage_insns) != INSN_LIST)
	{
	  usage_insn = next_usage_insns;
	  break;
	}
      usage_insn = XEXP (next_usage_insns, 0);
      gcc_assert (DEBUG_INSN_P (usage_insn));
      next_usage_insns = XEXP (next_usage_insns, 1);
      substitute_pseudo (&usage_insn, original_regno, new_reg);
      lra_update_insn_regno_info (usage_insn);
      if (lra_dump_file != NULL)
	{
	  fprintf (lra_dump_file, "    Split reuse change %d->%d:\n",
		   original_regno, REGNO (new_reg));
	  print_rtl_slim (lra_dump_file, usage_insn, usage_insn,
			  -1, 0);
	}
    }
  gcc_assert (NONDEBUG_INSN_P (usage_insn));
  gcc_assert (usage_insn != insn || (after_p && before_p));
  lra_process_new_insns (usage_insn, after_p ? NULL_RTX : restore,
			 after_p ? restore : NULL_RTX,
			 call_save_p
			 ?  "Add pseudo<-save" : "Add pseudo<-split");
  lra_process_new_insns (insn, before_p ? save : NULL_RTX,
			 before_p ? NULL_RTX : save,
			 call_save_p
			 ?  "Add save<-pseudo" : "Add split<-pseudo");
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file,
	     "    ))))))))))))))))))))))))))))))))))))))))))))))))\n");
  return true;
}

/* Check only pseudos living at the current program point in the
   current EBB.  */
static bitmap_head live_pseudos;

/* Update live info in EBB given by its HEAD and TAIL insns after
   inheritance/split transformation.  The function removes dead moves
   too.  */
static void
update_ebb_live_info (rtx head, rtx tail)
{
  unsigned int j;
  int regno;
  bool live_p;
  rtx prev_insn, set;
  bool remove_p;
  basic_block last_bb, prev_bb, curr_bb;
  bitmap_iterator bi;
  struct lra_insn_reg *reg;
  edge e;
  edge_iterator ei;

  last_bb = BLOCK_FOR_INSN (tail);
  prev_bb = NULL;
  for (curr_insn = tail;
       curr_insn != PREV_INSN (head);
       curr_insn = prev_insn)
    {
      prev_insn = PREV_INSN (curr_insn);
      if (! NONDEBUG_INSN_P (curr_insn))
	continue;
      curr_bb = BLOCK_FOR_INSN (curr_insn);
      if (curr_bb != prev_bb)
	{
	  if (prev_bb != NULL)
	    {
	      /* Udpate DF_LR_IN (prev_bb):  */
	      EXECUTE_IF_SET_IN_BITMAP (&check_only_pseudos, 0, j, bi)
		if (bitmap_bit_p (&live_pseudos, j))
		  bitmap_set_bit (DF_LR_IN (prev_bb), j);
		else
		  bitmap_clear_bit (DF_LR_IN (prev_bb), j);
	    }
	  if (curr_bb != last_bb)
	    {
	      /* Update DF_LR_OUT (curr_bb):  */
	      EXECUTE_IF_SET_IN_BITMAP (&check_only_pseudos, 0, j, bi)
		{
		  live_p = bitmap_bit_p (&live_pseudos, j);
		  if (! live_p)
		    FOR_EACH_EDGE (e, ei, curr_bb->succs)
		      if (bitmap_bit_p (DF_LR_IN (e->dest), j))
			{
			  live_p = true;
			  break;
			}
		  if (live_p)
		    bitmap_set_bit (DF_LR_OUT (curr_bb), j);
		  else
		    bitmap_clear_bit (DF_LR_OUT (curr_bb), j);
		}
	    }
	  prev_bb = curr_bb;
	  bitmap_and (&live_pseudos,
		      &check_only_pseudos, DF_LR_OUT (curr_bb));
	}
      curr_id = lra_get_insn_recog_data (curr_insn);
      remove_p = false;
      if ((set = single_set (curr_insn)) != NULL_RTX && REG_P (SET_DEST (set))
	  && (regno = REGNO (SET_DEST (set))) >= FIRST_PSEUDO_REGISTER
	  && bitmap_bit_p (&check_only_pseudos, regno)
	  && ! bitmap_bit_p (&live_pseudos, regno))
	remove_p = true;
      /* See which defined values die here.  */
      for (reg = curr_id->regs; reg != NULL; reg = reg->next)
	if (reg->type == OP_OUT && ! reg->early_clobber
	    && (! reg->subreg_p
		|| bitmap_bit_p (&lra_bound_pseudos, reg->regno)))
	  bitmap_clear_bit (&live_pseudos, reg->regno);
      /* Mark each used value as live.  */
      for (reg = curr_id->regs; reg != NULL; reg = reg->next)
	if (reg->type == OP_IN
	    && bitmap_bit_p (&check_only_pseudos, reg->regno))
	  bitmap_set_bit (&live_pseudos, reg->regno);
      /* Mark early clobber outputs dead.  */
      for (reg = curr_id->regs; reg != NULL; reg = reg->next)
	if (reg->type == OP_OUT && reg->early_clobber && ! reg->subreg_p)
	  bitmap_clear_bit (&live_pseudos, reg->regno);
      /* It is quite important to remove dead move insns because it
	 means removing dead store, we don't need to process them for
	 constraints, and unfortunately some subsequent optimizations
	 (like shrink-wrapping) currently based on assumption that
	 there are no trivial dead insns.  */
      if (remove_p)
	{
	  if (lra_dump_file != NULL)
	    {
	      fprintf (lra_dump_file, "     Removing dead insn:\n ");
	      print_rtl_slim (lra_dump_file, curr_insn, curr_insn, -1, 0);
	    }
	  lra_set_insn_deleted (curr_insn);
	}
    }
}

/* The structure describes info to do an inheritance for the current
   insn.  We need to collect such info first before doing the
   transformations because the transformations change the insn
   internal representation.  */
struct to_inherit
{
  /* Original regno.  */
  int regno;
  /* Subsequent insns which can inherit original reg value.  */
  rtx insns;
};

/* Array containing all info for doing inheritance from the current
   insn.  */
static struct to_inherit to_inherit[LRA_MAX_INSN_RELOADS];

/* Number elements in the previous array.  */
static int to_inherit_num;

/* Add inheritance info REGNO and INSNS.  */
static void
add_to_inherit (int regno, rtx insns)
{
  int i;

  for (i = 0; i < to_inherit_num; i++)
    if (to_inherit[i].regno == regno)
      return;
  gcc_assert (to_inherit_num < LRA_MAX_INSN_RELOADS);
  to_inherit[to_inherit_num].regno = regno;
  to_inherit[to_inherit_num++].insns = insns;
}

/* Set up RES by registers living on edges FROM except edege (FROM,
   TO).  */
static void
get_live_on_other_edges (basic_block from, basic_block to, bitmap res)
{
  edge e;
  edge_iterator ei;

  gcc_assert (to != NULL);
  bitmap_clear (res);
  FOR_EACH_EDGE (e, ei, from->succs)
    if (e->dest != to)
      bitmap_ior_into (res, DF_LR_IN (e->dest));
}
	
/* Return first (if FIRST_P) or last non-debug insn in basic block BB.
   Return null if there are no non-debug insns in the block.  */
static rtx
get_non_debug_insn (bool first_p, basic_block bb)
{
  rtx insn;

  for (insn = first_p ? BB_HEAD (bb) : BB_END (bb);
       insn != NULL_RTX && ! NONDEBUG_INSN_P (insn);
       insn = first_p ? NEXT_INSN (insn) : PREV_INSN (insn))
    ;
  if (insn != NULL_RTX && BLOCK_FOR_INSN (insn) != bb)
    insn = NULL_RTX;
  return insn;
}

/* Used as a temporary results of some bitmap calculations.  */
static bitmap_head temp_bitmap;

/* The function is used to form list REGNO usages which consists of
   optional debug insns finished by a non-debug insn using REGNO.
   RELOADS_NUM is current number of reload insns processed so far.  */
static void
add_next_usage_insn (int regno, rtx insn, int reloads_num)
{
  rtx next_usage_insns;
  
  if (usage_insns[regno].check == curr_usage_insns_check
      && (next_usage_insns = usage_insns[regno].insns) != NULL_RTX
      && DEBUG_INSN_P (insn))
    {
      /* Check that we did not add the debug insn yet.  */
      if (next_usage_insns != insn
	  && (GET_CODE (next_usage_insns) != INSN_LIST
	      || XEXP (next_usage_insns, 0) != insn))
	usage_insns[regno].insns = gen_rtx_INSN_LIST (VOIDmode, insn,
						      next_usage_insns);
    }
  else if (NONDEBUG_INSN_P (insn))
    {
      usage_insns[regno].check = curr_usage_insns_check;
      usage_insns[regno].insns = insn;
      usage_insns[regno].reloads_num = reloads_num;
      usage_insns[regno].calls_num = calls_num;
      usage_insns[regno].after_p = false;
    }
  else
    usage_insns[regno].check = 0;
}
  
/* Do inheritance/split transformations in EBB starting with HEAD and
   finishing on TAIL.  We process EBB insns in the reverse order.
   Return true if we did any inheritance/split transformation in the
   EBB.

   We should avoid excessive splitting which results in worse code
   because of inaccurate cost calculations for spilling new split
   pseudos in such case.  To achieve this we do splitting only if
   register pressure is high in given basic block and there reload
   pseudos requiring hard registers.  We could do more register
   pressure calculations at any given program point to avoid necessary
   splitting even more but it is to expensive and the current approach
   is well enough.  */
static bool
inherit_in_ebb (rtx head, rtx tail)
{
  int i, src_regno, dst_regno;
  bool change_p, succ_p;
  rtx prev_insn, next_usage_insns, set,  first_insn, last_insn;
  enum reg_class cl;
  struct lra_insn_reg *reg;
  basic_block last_processed_bb, curr_bb = NULL;
  HARD_REG_SET potential_reload_hard_regs, live_hard_regs;
  bitmap to_process;
  unsigned int j;
  bitmap_iterator bi;
  bool head_p, after_p;


  change_p = false;
  curr_usage_insns_check++;
  reloads_num = calls_num = 0;
  /* Remeber: we can remove the current insn.  */
  bitmap_clear (&check_only_pseudos);
  last_processed_bb = NULL;
  CLEAR_HARD_REG_SET (potential_reload_hard_regs);
  CLEAR_HARD_REG_SET (live_hard_regs);
  /* We don't process new insns generated in the loop.  */
  for (curr_insn = tail; curr_insn != PREV_INSN (head); curr_insn = prev_insn)
    {
      prev_insn = PREV_INSN (curr_insn);
      if (BLOCK_FOR_INSN (curr_insn) != NULL)
	curr_bb = BLOCK_FOR_INSN (curr_insn);
      if (last_processed_bb != curr_bb)
	{
	  /* We are at the end of BB.  Add qualified living
	     pseudos for potential splitting.  */
	  to_process = DF_LR_OUT (curr_bb);
	  if (last_processed_bb != NULL)
	    {	
	      /* We are somewhere in the middle of EBB. */
	      get_live_on_other_edges (curr_bb, last_processed_bb, &temp_bitmap);
	      to_process = &temp_bitmap;
	    }
	  last_processed_bb = curr_bb;
	  last_insn = get_non_debug_insn (false, curr_bb);
	  after_p = (last_insn != NULL_RTX && ! JUMP_P (last_insn)
		     && (! CALL_P (last_insn)
			 || find_reg_note (last_insn,
					   REG_NORETURN, NULL) == NULL_RTX));
	  REG_SET_TO_HARD_REG_SET (live_hard_regs, DF_LR_OUT (curr_bb));
	  IOR_HARD_REG_SET (live_hard_regs, eliminable_regset);
	  IOR_HARD_REG_SET (live_hard_regs, lra_no_alloc_regs);
	  CLEAR_HARD_REG_SET (potential_reload_hard_regs);
	  EXECUTE_IF_SET_IN_BITMAP (to_process,
				    FIRST_PSEUDO_REGISTER, j, bi)
	    if ((int) j < lra_constraint_new_regno_start
		&& reg_renumber[j] >= 0)
	      {
		lra_add_hard_reg_set (reg_renumber[j],
				      PSEUDO_REGNO_MODE (j),
				      &live_hard_regs);
		usage_insns[j].check = curr_usage_insns_check;
		usage_insns[j].insns = last_insn;
		usage_insns[j].reloads_num = reloads_num;
		usage_insns[j].calls_num = calls_num;
		usage_insns[j].after_p = after_p;
	      }
	}
      src_regno = dst_regno = -1;
      if (NONDEBUG_INSN_P (curr_insn)
	  && (set = single_set (curr_insn)) != NULL_RTX
	  && REG_P (SET_DEST (set)) && REG_P (SET_SRC (set)))
	{
	  src_regno = REGNO (SET_SRC (set));
	  dst_regno = REGNO (SET_DEST (set));
	}
      if (src_regno < lra_constraint_new_regno_start
	  && src_regno >= FIRST_PSEUDO_REGISTER
	  && reg_renumber[src_regno] < 0
	  && dst_regno >= lra_constraint_new_regno_start
	  && (cl = lra_get_allocno_class (dst_regno)) != NO_REGS)
	{
	  /* 'reload_pseudo <- original_pseudo'.  */
	  reloads_num++;
	  succ_p = false;
	  if (usage_insns[src_regno].check == curr_usage_insns_check
	      && (next_usage_insns = usage_insns[src_regno].insns) != NULL_RTX)
	    succ_p = inherit_reload_reg (false,
					 bitmap_bit_p (&lra_matched_pseudos,
						       dst_regno),
					 src_regno, cl,
					 curr_insn, next_usage_insns);
	  if (succ_p)
	    change_p = true;
	  else
	    {
	      usage_insns[src_regno].check = curr_usage_insns_check;
	      usage_insns[src_regno].insns = curr_insn;
	      usage_insns[src_regno].reloads_num = reloads_num;
	      usage_insns[src_regno].calls_num = calls_num;
	      usage_insns[src_regno].after_p = false;
	    }
	  if (cl != NO_REGS
	      && hard_reg_set_subset_p (reg_class_contents[cl],
					live_hard_regs))
	    IOR_HARD_REG_SET (potential_reload_hard_regs, reg_class_contents[cl]);
	}
      else if (src_regno >= lra_constraint_new_regno_start
	       && dst_regno < lra_constraint_new_regno_start
	       && dst_regno >= FIRST_PSEUDO_REGISTER
	       && reg_renumber[dst_regno] < 0
	       && (cl = lra_get_allocno_class (src_regno)) != NO_REGS
	       && usage_insns[dst_regno].check == curr_usage_insns_check
	       && (next_usage_insns = usage_insns[dst_regno].insns) != NULL_RTX)
	{
	  reloads_num++;
	  /* 'original_pseudo <- reload_pseudo'.  */
	  if (! JUMP_P (curr_insn)
	      && inherit_reload_reg (true, false, dst_regno, cl,
				     curr_insn, next_usage_insns))
	    change_p = true;
	  /* Invalidate.  */
	  usage_insns[dst_regno].check = 0;
	  if (cl != NO_REGS
	      && hard_reg_set_subset_p (reg_class_contents[cl], live_hard_regs))
	    IOR_HARD_REG_SET (potential_reload_hard_regs, reg_class_contents[cl]);
	}
      else if (INSN_P (curr_insn))
	{
	  int max_uid = get_max_uid ();

	  curr_id = lra_get_insn_recog_data (curr_insn);
	  to_inherit_num = 0;
	  /* Process insn definitions.  */
	  for (reg = curr_id->regs; reg != NULL; reg = reg->next)
	    if (reg->type != OP_IN
		&& (dst_regno = reg->regno) >= FIRST_PSEUDO_REGISTER
		&& dst_regno < lra_constraint_new_regno_start
		&& usage_insns[dst_regno].check == curr_usage_insns_check
		&& (next_usage_insns
		    = usage_insns[dst_regno].insns) != NULL_RTX)
	      {
		if (reg->type == OP_OUT
		    && reg_renumber[dst_regno] < 0 && ! reg->subreg_p)
		  {
		    struct lra_insn_reg *r;
		    
		    for (r = curr_id->regs; r != NULL; r = r->next)
		      if (r->type != OP_OUT && r->regno == dst_regno)
			break;
		    /* Don't do inheritance if the pseudo is also
		       used in the insn.  */
		    if (r == NULL)
		      /* We can not do inheritance right now
			 because the current insn reg info (chain
			 regs) can change after that.  */
		      add_to_inherit (dst_regno, next_usage_insns);
		  }
		/* We can not process one pseudo twice here
		   because of usage_insns invalidation.  */
		if (reg_renumber[dst_regno] >= 0 && ! reg->subreg_p
		    && reg->type == OP_OUT)
		  {
		    HARD_REG_SET s;
		    
		    if (need_for_split_p (potential_reload_hard_regs,
					  dst_regno)
			&& split_pseudo (false, dst_regno, curr_insn,
					 next_usage_insns))
		      change_p = true;
		    CLEAR_HARD_REG_SET (s);
		    lra_add_hard_reg_set (reg_renumber[dst_regno],
					  PSEUDO_REGNO_MODE (dst_regno), &s);
		    AND_COMPL_HARD_REG_SET (live_hard_regs, s);
		  }
		if (reg_renumber[dst_regno] < 0
		    || (reg->type == OP_OUT && ! reg->subreg_p))
		/* Invalidate.  */
		usage_insns[dst_regno].check = 0;
	      }
	  if (! JUMP_P (curr_insn))
	    for (i = 0; i < to_inherit_num; i++)
	      if (inherit_reload_reg (true, false, to_inherit[i].regno,
				      ALL_REGS, curr_insn,
				      to_inherit[i].insns))
	      change_p = true;
	  if (CALL_P (curr_insn))
	    calls_num++;
	  to_inherit_num = 0;
	  /* Process insn usages.  */
	  for (reg = curr_id->regs; reg != NULL; reg = reg->next)
	    if ((reg->type != OP_OUT
		 || (reg->type == OP_OUT && reg->subreg_p))
		&& (src_regno = reg->regno) >= FIRST_PSEUDO_REGISTER
		&& src_regno < lra_constraint_new_regno_start)
	      {
		if (reg_renumber[src_regno] < 0 && reg->type == OP_IN)
		  {
		    if (usage_insns[src_regno].check == curr_usage_insns_check
			&& (next_usage_insns
			    = usage_insns[src_regno].insns) != NULL_RTX
			&& NONDEBUG_INSN_P (curr_insn))
		      add_to_inherit (src_regno, next_usage_insns);
		    else
		      /* Add usages.  */
		      add_next_usage_insn (src_regno, curr_insn, reloads_num);
		  }
		else if (reg_renumber[src_regno] >= 0)
		  {
		    bool before_p;
		    rtx use_insn = curr_insn;

		    before_p = (JUMP_P (curr_insn)
				|| (CALL_P (curr_insn) && reg->type == OP_IN));
		    if (usage_insns[src_regno].check == curr_usage_insns_check
			&& (next_usage_insns
			    = usage_insns[src_regno].insns) != NULL_RTX
			/* To avoid processing the pseudo twice or
			   more.  */
			&& ((GET_CODE (next_usage_insns) != INSN_LIST
			     && INSN_UID (next_usage_insns) < max_uid)
			    || (GET_CODE (next_usage_insns) == INSN_LIST
				&& (INSN_UID (XEXP (next_usage_insns, 0))
				    < max_uid)))
			&& need_for_split_p (potential_reload_hard_regs,
					     src_regno)
			&& NONDEBUG_INSN_P (curr_insn)
			&& split_pseudo (before_p, src_regno, curr_insn,
					 next_usage_insns))
		      {
			if (reg->subreg_p)
			  lra_risky_transformations_p = true;
			change_p = true;
			/* Invalidate.  */
			usage_insns[src_regno].check = 0;
			if (before_p)
			  use_insn = PREV_INSN (curr_insn);
		      }
		    if (NONDEBUG_INSN_P (curr_insn))
		      lra_add_hard_reg_set (reg_renumber[src_regno],
					    PSEUDO_REGNO_MODE (src_regno),
					    &live_hard_regs);
		      add_next_usage_insn (src_regno, use_insn, reloads_num);
		  }
	      }
	  for (i = 0; i < to_inherit_num; i++)
	    {
	      src_regno = to_inherit[i].regno;
	      if (inherit_reload_reg (false, false, src_regno, ALL_REGS,
				      curr_insn, to_inherit[i].insns))
		change_p = true;
	      else
		{
		  usage_insns[src_regno].check = curr_usage_insns_check;
		  usage_insns[src_regno].insns = curr_insn;
		  usage_insns[src_regno].reloads_num = reloads_num;
		  usage_insns[src_regno].calls_num = calls_num;
		  usage_insns[src_regno].after_p = false;
		}
	    }
	}
      /* We reached the start of the current basic block.  */
      if (prev_insn == NULL_RTX || prev_insn == PREV_INSN (head)
	  || BLOCK_FOR_INSN (prev_insn) != curr_bb)
	{
	  /* We reached the beginning of the current block -- do
	     rest of spliting in the current BB.  */
	  first_insn = get_non_debug_insn (true, curr_bb);
	  to_process = DF_LR_IN (curr_bb);
	  if (BLOCK_FOR_INSN (head) != curr_bb)
	    {	
	      /* We are somewhere in the middle of EBB.  */
	      get_live_on_other_edges (EDGE_PRED (curr_bb, 0)->src,
				       curr_bb, &temp_bitmap);
	      to_process = &temp_bitmap;
	    }
	  head_p = true;
	  EXECUTE_IF_SET_IN_BITMAP (to_process,
				    FIRST_PSEUDO_REGISTER, j, bi)
	    if ((int) j < lra_constraint_new_regno_start
		&& reg_renumber[j] >= 0
		&& usage_insns[j].check == curr_usage_insns_check
		&& (next_usage_insns = usage_insns[j].insns) != NULL_RTX)
	      {
		if (first_insn != NULL_RTX
		    && need_for_split_p (potential_reload_hard_regs, j))
		  {
		    if (lra_dump_file != NULL && head_p)
		      {
			fprintf (lra_dump_file,
				 "  ----------------------------------\n");
			head_p = false;
		      }
		    if (split_pseudo (true, j, first_insn,
				      next_usage_insns))
		      change_p = true;
		  }
		usage_insns[j].check = 0;
	      }
	}
    }
  return change_p;
}

/* This value affects EBB forming.  If probability of edge from EBB to
   a BB is not greater than the following value, we don't add the BB
   to EBB.  */ 
#define EBB_PROBABILITY_CUTOFF (REG_BR_PROB_BASE / 2)

/* Current number of inheritance/split iteration.  */
int lra_inheritance_iter;

/* Entry function for inheritance/split pass.  */
void
lra_inheritance (void)
{
  int i;
  basic_block bb, start_bb;
  edge e;

  lra_inheritance_iter++;
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file, "\n********** Inheritance #%d: **********\n\n",
	     lra_inheritance_iter);
  curr_usage_insns_check = 0;
  usage_insns
    = (struct usage_insns *) xmalloc (sizeof (struct usage_insns)
				      * lra_constraint_new_regno_start);
  for (i = 0; i < lra_constraint_new_regno_start; i++)
    usage_insns[i].check = 0;
  bitmap_initialize (&check_only_pseudos, &reg_obstack);
  bitmap_initialize (&live_pseudos, &reg_obstack);
  bitmap_initialize (&temp_bitmap, &reg_obstack);
  FOR_EACH_BB (bb)
    {
      start_bb = bb;
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "EBB");
      /* Form a EBB starting with BB.  */
      for (;;)
	{
	  if (lra_dump_file != NULL)
	    fprintf (lra_dump_file, " %d", bb->index);
	  if (bb->next_bb == EXIT_BLOCK_PTR || LABEL_P (BB_HEAD (bb->next_bb)))
	    break;
	  e = find_fallthru_edge (bb->succs);
	  if (! e)
	    break;
	  if (e->probability <= EBB_PROBABILITY_CUTOFF)
	    break;
	  bb = bb->next_bb;
	}
      if (lra_dump_file != NULL)
	fprintf (lra_dump_file, "\n");
      if (inherit_in_ebb (BB_HEAD (start_bb), BB_END (bb)))
	/* Remember that the EBB head and tail can change in
	   inherit_in_ebb.  */
	update_ebb_live_info (BB_HEAD (start_bb), BB_END (bb));
    }
  bitmap_clear (&temp_bitmap);
  bitmap_clear (&live_pseudos);
  bitmap_clear (&check_only_pseudos);
  free (usage_insns);
}



/* This page contains code to undo failed inheritance/split
   transformations.  */

/* Current number of iteration undoing inheritance/split.  */
int lra_undo_inheritance_iter;

/* Temporary bitmaps used during calls of FIX_BB_LIVE_INFO.  */
static bitmap_head temp_bitmap_head;

/* Fix BB live info LIVE after removing pseudos created on pass doing
   inheritance/split which are REMOVED_PSEUDOS.  */
static void
fix_bb_live_info (bitmap live, bitmap removed_pseudos)
{
  unsigned int regno;
  bitmap_iterator bi;

  bitmap_and (&temp_bitmap_head, removed_pseudos, live);
  EXECUTE_IF_SET_IN_BITMAP (&temp_bitmap_head, 0, regno, bi)
    {
      bitmap_clear_bit (live, regno);
      bitmap_set_bit (live, lra_reg_info[regno].restore_regno);
    }
}

/* Return regno of the (subreg of) pseudo REG. Otherise, return
   a negative number.  */
static int
get_pseudo_regno (rtx reg)
{
  int regno;

  if (GET_CODE (reg) == SUBREG)
    reg = SUBREG_REG (reg);
  if (REG_P (reg)
      && (regno = REGNO (reg)) >= FIRST_PSEUDO_REGISTER)
    return regno;
  return -1;
}

/* Remove inheritance/split pseudos which are in REMOVE_PSEUDOS and
   return true if we did any change.  The undo transformations for
   inheritance looks like
      i <- i2
      p <- i      =>   p <- i2
   or removing
      p <- i, i <- p, and i <- i3
   where p is original pseudo from which inheritance pseudo i was
   created, i and i3 are removed inheritance pseudos, i2 is another
   not removed inheritance pseudo.  All split pseudos or other
   occurrences of removed inheritance pseudos are changed on the
   corresponding original pseudos.  */
static bool
remove_inheritance_pseudos (bitmap remove_pseudos)
{
  basic_block bb;
  int regno, sregno, prev_sregno, dregno, restore_regno;
  rtx set, prev_set, prev_insn;
  bool change_p, done_p;

  change_p = ! bitmap_empty_p (remove_pseudos);
  bitmap_initialize (&temp_bitmap_head, &reg_obstack);
  FOR_EACH_BB (bb)
    {
      fix_bb_live_info (DF_LR_IN (bb), remove_pseudos);
      fix_bb_live_info (DF_LR_OUT (bb), remove_pseudos);
      FOR_BB_INSNS_REVERSE (bb, curr_insn)
	{
	  if (! INSN_P (curr_insn))
	    continue;
	  done_p = false;
	  sregno = dregno = -1;
	  if (change_p && NONDEBUG_INSN_P (curr_insn)
	      && (set = single_set (curr_insn)) != NULL_RTX)
	    {
	      dregno = get_pseudo_regno (SET_DEST (set));
	      sregno = get_pseudo_regno (SET_SRC (set));
	    }
	  
	  if (sregno >= 0 && dregno >= 0)
	    {
	      if ((bitmap_bit_p (remove_pseudos, sregno)
		   && (lra_reg_info[sregno].restore_regno == dregno
		       || (bitmap_bit_p (remove_pseudos, dregno)
			   && (lra_reg_info[sregno].restore_regno
			       == lra_reg_info[dregno].restore_regno))))
		  || (bitmap_bit_p (remove_pseudos, dregno)
		      && lra_reg_info[dregno].restore_regno == sregno))
		/* One of the following cases:
		     original <- removed inheritance pseudo
                     removed inherit pseudo <- another removed inherit pseudo
                     removed inherit pseudo <- original pseudo
		   Or
		     removed_split_pseudo <- original_pseudo
                     original_pseudo <- removed_split_pseudo */
		{
		  if (lra_dump_file != NULL)
		    {
		      fprintf (lra_dump_file, "    Removing %s:\n",
			       bitmap_bit_p (&lra_split_pseudos, sregno)
			       || bitmap_bit_p (&lra_split_pseudos, dregno)
			       ? "split" : "inheritance");
		      print_rtl_slim (lra_dump_file,
				      curr_insn, curr_insn, -1, 0);
		    }
		  lra_set_insn_deleted (curr_insn);
		  done_p = true;
		}
	      else if (bitmap_bit_p (remove_pseudos, sregno)
		       && bitmap_bit_p (&lra_inheritance_pseudos, sregno))
		{
		  /* Search the following pattern:
		       inherit_or_split_pseudo1 <- inherit_or_split_pseudo2
                       original_pseudo <- inherit_or_split_pseudo1
		    where the 2nd insn is the current insn and
		    inherit_or_split_pseudo2 is not removed.  If it is found,
		    change the current insn onto:
		       original_pseudo1 <- inherit_or_split_pseudo2.  */
		  for (prev_insn = PREV_INSN (curr_insn);
		       prev_insn != NULL_RTX && ! NONDEBUG_INSN_P (prev_insn);
		       prev_insn = PREV_INSN (prev_insn))
		    ;
		  if (prev_insn != NULL_RTX && BLOCK_FOR_INSN (prev_insn) == bb
		      && (prev_set = single_set (prev_insn)) != NULL_RTX
		      /* There should be no subregs in insn we are
			 searching because only the original reg might
			 be in subreg when we changed the mode of
			 load/store for splitting.  */
		      && REG_P (SET_DEST (prev_set))
		      && REG_P (SET_SRC (prev_set))
		      && (int) REGNO (SET_DEST (prev_set)) == sregno
		      && ((prev_sregno = REGNO (SET_SRC (prev_set)))
			  >= FIRST_PSEUDO_REGISTER)
		      && (lra_reg_info[sregno].restore_regno
			  == lra_reg_info[prev_sregno].restore_regno)
		      && ! bitmap_bit_p (remove_pseudos, prev_sregno))
		    {
		      gcc_assert (GET_MODE (SET_SRC (prev_set))
				  == GET_MODE (regno_reg_rtx[sregno]));
		      if (GET_CODE (SET_SRC (set)) == SUBREG)
			SUBREG_REG (SET_SRC (set)) = SET_SRC (prev_set);
		      else
			SET_SRC (set) = SET_SRC (prev_set);
		      lra_push_insn_and_update_insn_regno_info (curr_insn);
		      lra_set_used_insn_alternative_by_uid (INSN_UID (curr_insn), -1);
		      done_p = true;
		      if (lra_dump_file != NULL)
			{
			  fprintf (lra_dump_file, "    Change reload insn:\n");
			  print_rtl_slim (lra_dump_file,
					  curr_insn, curr_insn, -1, 0);
			}
		    }
		}
	    }
	  if (! done_p)
	    {
	      struct lra_insn_reg *reg;
	      bool insn_change_p = false;

	      curr_id = lra_get_insn_recog_data (curr_insn);
	      for (reg = curr_id->regs; reg != NULL; reg = reg->next)
		if ((regno = reg->regno) >= lra_constraint_new_regno_start
		    && lra_reg_info[regno].restore_regno >= 0)
		  {
		    if (change_p && bitmap_bit_p (remove_pseudos, regno))
		      {
			restore_regno = lra_reg_info[regno].restore_regno;
			gcc_assert (restore_regno >= FIRST_PSEUDO_REGISTER);
			substitute_pseudo (&curr_insn, regno,
					   regno_reg_rtx[restore_regno]);
			insn_change_p = true;
		      }
		    else if (NONDEBUG_INSN_P (curr_insn))
		      {
			lra_push_insn_and_update_insn_regno_info (curr_insn);
			lra_set_used_insn_alternative_by_uid
			  (INSN_UID (curr_insn), -1);
		      }
		  }
	      if (insn_change_p)
		{
		  lra_update_insn_regno_info (curr_insn);
		  if (lra_dump_file != NULL)
		    {
		      fprintf (lra_dump_file, "    Restore original insn:\n");
		      print_rtl_slim (lra_dump_file,
				      curr_insn, curr_insn, -1, 0);
		    }
		}
	    }
	}
    }
  bitmap_clear (&temp_bitmap_head);
  return change_p;
}

/* Entry function for undoing inheritance transformation.  Return true
   if we did any RTL change in this pass.  */
bool
lra_undo_inheritance (void)
{
  unsigned int regno;
  int restore_regno;
  int n_all_inherit, n_inherit, n_all_split, n_split;
  bitmap_head remove_pseudos;
  bitmap_iterator bi;
  bool change_p;

  lra_undo_inheritance_iter++;
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file,
	     "\n********** Undoing inheritance #%d: **********\n\n",
	     lra_undo_inheritance_iter);
  bitmap_initialize (&remove_pseudos, &reg_obstack);
  n_inherit = n_all_inherit = 0;
  EXECUTE_IF_SET_IN_BITMAP (&lra_inheritance_pseudos, 0, regno, bi)
    if (lra_reg_info[regno].restore_regno >= 0)
      {
	n_all_inherit++;
	if (reg_renumber[regno] < 0)
	  bitmap_set_bit (&remove_pseudos, regno);
	else
	  n_inherit++;
      }
  if (lra_dump_file != NULL && n_all_inherit != 0)
    fprintf (lra_dump_file, "Inherit %d out of %d (%.2f%%)\n",
	     n_inherit, n_all_inherit,
	     (double) n_inherit / n_all_inherit * 100);
  n_split = n_all_split = 0;
  EXECUTE_IF_SET_IN_BITMAP (&lra_split_pseudos, 0, regno, bi)
    if ((restore_regno = lra_reg_info[regno].restore_regno) >= 0)
      {
	n_all_split++;
	if (reg_renumber[restore_regno] < 0
	    || reg_renumber[regno] == reg_renumber[restore_regno])
	  bitmap_set_bit (&remove_pseudos, regno);
	else
	  {
	    n_split++;
	    if (lra_dump_file != NULL)
	      fprintf (lra_dump_file, "      Keep split r%d (orig=r%d)\n",
		       regno, restore_regno);
	  }
      }
  if (lra_dump_file != NULL && n_all_split != 0)
    fprintf (lra_dump_file, "Split %d out of %d (%.2f%%)\n",
	     n_split, n_all_split,
	     (double) n_split / n_all_split * 100);
  change_p = remove_inheritance_pseudos (&remove_pseudos);
  bitmap_clear (&remove_pseudos);
  /* Clear restore_regnos.  */
  EXECUTE_IF_SET_IN_BITMAP (&lra_inheritance_pseudos, 0, regno, bi)
    lra_reg_info[regno].restore_regno = -1;
  EXECUTE_IF_SET_IN_BITMAP (&lra_split_pseudos, 0, regno, bi)
    lra_reg_info[regno].restore_regno = -1;
  return change_p;
}
