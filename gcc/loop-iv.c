/* Induction variable analysis.
   Copyright (C) 2003 Free Software Foundation, Inc.

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
#include "hard-reg-set.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "cfglayout.h"
#include "params.h"
#include "output.h"
#include "expr.h"
#include "function.h"
#include "df.h"

/* We perform induction variable analysis here.  We expect the loops to be
   in the normal form for the loop optimizer -- i.e. with preheaders and latches
   with exactly one successor (therefore the entry edge to a loop always leads
   from its immediate superloop and exit edges lead to superloops of the loop).
   For end of each basic block BB we express the values of registers in form
     REG = (PLUS base (MULT step ITERATION))
   where
     LOOP is the innermost loop containing BB
     base and step are invariant in LOOP. They may use INITIAL_VALUE
      operation and no registers may occur inside the expressions outside
      of INITIAL_VALUE.  The registers in step must also be unchanging.
     ITERATION is the number of executions of LOOP's latch.
*/

/* Maximal register number.  */
static unsigned max_regno;

/* Dataflow informations.  */
static struct df *df;

/* The induction variables at loop entries.  */
rtx **loop_entry_values;

/* The induction variables at loop exits.  */
static rtx **loop_end_values;

/* The values of registers at entries to the loops.  */
rtx **initial_values;

/* Bitmaps of registers modified in the loops.  */
static sbitmap *modified_regs;

/* Bitmap of insns that we already processed.  */
static sbitmap insn_processed;

/* The dominator relation.  */
static fast_dominance_info dom;

/* Values of registers at current point.  This array is never cleared,
   so the user is responsible for initializing those fields that he's going
   to need.  */
rtx *iv_register_values;

/* Similar to reaching defs, but limiting ourselves only to inside of loops.  */
bitmap *loop_rd_in;

/* Only the defs that are marked here are already computed.  */
sbitmap loop_rd_in_ok;

/* Sbitmap of registers that are interesting for us (pseudoregisters
   in integer modes).  */
sbitmap interesting_reg;

/* Shared rtxes.  */
static rtx *initial_value_rtx;
static rtx iteration_rtx[NUM_MACHINE_MODES];

static sbitmap suitable_code;		/* Bitmap of rtl codes we are able
					   to handle.  */

#define good_constant_p(EXPR) \
  (GET_CODE (EXPR) == CONST_INT \
   || GET_CODE (EXPR) == SYMBOL_REF)

#define expr_mentions_iteration_p(EXPR) 		\
  (GET_CODE(EXPR) == ITERATION				\
   || (GET_RTX_CLASS (GET_CODE (EXPR)) == 'c'		\
       && GET_CODE(XEXP (EXPR, 1)) == ITERATION)	\
   || (!good_constant_p (EXPR)				\
       && GET_CODE(EXPR) != VALUE_AT			\
       && expr_mentions_code_p ((EXPR), ITERATION)))
     
static rtx gen_initial_value		PARAMS ((unsigned));
static rtx gen_iteration		PARAMS ((enum machine_mode));
static rtx gen_value_at			PARAMS ((unsigned, rtx, int));
static void record_def_value		PARAMS ((rtx, unsigned, rtx));
static void record_use_value		PARAMS ((rtx, unsigned, rtx))
     ATTRIBUTE_UNUSED;
static bool invariant_wrto_ivs_p	PARAMS ((rtx, rtx *));
static void init_suitable_codes		PARAMS ((void));
static void simulate_set		PARAMS ((rtx, rtx, void *));
static rtx straighten_ops		PARAMS ((rtx));
static rtx sort_ops			PARAMS ((rtx));
static int iv_compare_rtx		PARAMS ((rtx, rtx));
static rtx use_distributive_law		PARAMS ((rtx));
static rtx iv_simplify_mult		PARAMS ((rtx));
static bool combine_constants		PARAMS ((rtx, rtx, enum machine_mode,
						 rtx *));
static rtx iv_simplify_plus		PARAMS ((rtx));
static rtx compare_with_mode_bounds	PARAMS ((enum rtx_code,
						 rtx,
						 enum machine_mode,
						 enum machine_mode));
static rtx iv_simplify_relational	PARAMS ((rtx, enum machine_mode));
static void simplify_ivs_using_values	PARAMS ((rtx *, rtx *));
static void clear_reg_values		PARAMS ((int));
static rtx earliest_value_at_for	PARAMS ((basic_block, int));
static rtx get_reg_value_at		PARAMS ((basic_block, rtx,
						 struct ref *));
static int iv_omit_initial_values_1	PARAMS ((rtx *, void *));
static void compute_reg_values		PARAMS ((basic_block, rtx));
static void compute_register_values	PARAMS ((int));
static void simplify_reg_values		PARAMS ((basic_block, rtx));
static void simplify_register_values	PARAMS ((void));
static void compute_loop_end_values	PARAMS ((struct loop *, rtx *));
static void compute_initial_values	PARAMS ((struct loop *));
static void fill_loop_rd_in_for_def	PARAMS ((struct ref *));
extern void dump_equations		PARAMS ((FILE *, rtx *));
extern void dump_insn_ivs		PARAMS ((FILE *, rtx));

/* Dump equations for induction variables in VALUES to FILE.  */
void
dump_equations (file, values)
     FILE *file;
     rtx *values;
{
  unsigned regno;

  fprintf (file, "   unchanged:");
  for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
    if (TEST_BIT (interesting_reg, regno)
	&& values[regno] == gen_initial_value (regno))
      fprintf (file, " %d", regno);
  fprintf (file, "\n");

  for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
    if (TEST_BIT (interesting_reg, regno)
	&& values[regno] != gen_initial_value (regno))
      {
	fprintf (file, "   reg %d:\n", regno);
	print_rtl (file, values[regno]);
	fprintf (file, "\n");
      }
  fprintf (file, "\n\n");
}

/* Dump equations for induction variables at INSN to FILE.  */
void
dump_insn_ivs (file, insn)
     FILE *file;
     rtx insn;
{
  struct df_link *def = DF_INSN_DEFS (df, insn);
  struct df_link *use = DF_INSN_USES (df, insn);

  fprintf (file, "USES:\n");
  for (; use; use = use->next)
    {
      if (!DF_REF_REG_USE_P (use->ref))
	continue;
      if (!TEST_BIT (interesting_reg, DF_REF_REGNO (use->ref)))
	continue;
      fprintf (file, " reg %d:\n", DF_REF_REGNO (use->ref));
      print_rtl (file, use->ref->aux);
      fprintf (file, "\n");
    }

  fprintf (file, "DEFS:\n");
  for (; def; def = def->next)
    {
      if (!DF_REF_REG_DEF_P (def->ref))
	continue;
      if (!TEST_BIT (interesting_reg, DF_REF_REGNO (def->ref)))
	continue;
      fprintf (file, " reg %d:\n", DF_REF_REGNO (def->ref));
      print_rtl (file, def->ref->aux);
      fprintf (file, "\n");
    }
}

/* Generate INITIAL_VALUE for register REGNO (they are shared, so just return
   the rtx).  */
static rtx
gen_initial_value (regno)
     unsigned regno;
{
  return initial_value_rtx[regno];
}

/* Generate ITERATION in MODE (they are shared, so just return the rtx).  */
static rtx
gen_iteration (mode)
     enum machine_mode mode;
{
  return iteration_rtx[mode];
}

/* Generate a VALUE_AT rtx for register REGNO at INSN (if AFTER, immediatelly
   after it).  */
static rtx
gen_value_at (regno, insn, after)
     unsigned regno;
     rtx insn;
     int after;
{
  int place_number;

  if (!after)
    place_number = INSN_UID (insn);
  else if (NEXT_INSN (insn))
    place_number = INSN_UID (NEXT_INSN (insn));
  else
    place_number = -INSN_UID (insn);
  return gen_rtx_fmt_ii (VALUE_AT, GET_MODE (regno_reg_rtx[regno]), regno,
			 place_number);
}

/* Record VALUE to def of register REGNO in INSN.  */
static void
record_def_value (insn, regno, value)
     rtx insn;
     unsigned regno;
     rtx value;
{
  struct df_link *def = DF_INSN_DEFS (df, insn);

  for (; def; def = def->next)
    {
      if (!DF_REF_REG_DEF_P (def->ref))
	continue;
      if (DF_REF_REGNO (def->ref) == regno)
	break;
    }
  if (!def)
    abort ();

  def->ref->aux = value;
}

/* Record VALUE to use of register REGNO in INSN.  */
static void
record_use_value (insn, regno, value)
     rtx insn;
     unsigned regno;
     rtx value;
{
  struct df_link *use = DF_INSN_USES (df, insn);

  for (; use; use = use->next)
    {
      if (!DF_REF_REG_USE_P (use->ref))
	continue;
      if (DF_REF_REGNO (use->ref) == regno)
	break;
    }
  if (!use)
    abort ();

  use->ref->aux = value;
}

/* Get value from def of register REGNO in INSN.  */
rtx
get_def_value (insn, regno)
     rtx insn;
     unsigned regno;
{
  struct df_link *def = DF_INSN_DEFS (df, insn);

  for (; def; def = def->next)
    {
      if (!DF_REF_REG_DEF_P (def->ref))
	continue;
      if (DF_REF_REGNO (def->ref) == regno)
	break;
    }
  if (!def)
    abort ();

  return (rtx) def->ref->aux;
}

/* Get VALUE from use of register REGNO in INSN.  */
rtx
get_use_value (insn, regno)
     rtx insn;
     unsigned regno;
{
  struct df_link *use = DF_INSN_USES (df, insn);

  for (; use; use = use->next)
    {
      if (!DF_REF_REG_USE_P (use->ref))
	continue;
      if (DF_REF_REGNO (use->ref) == regno)
	break;
    }
  if (!use)
    abort ();

  return (rtx) use->ref->aux;
}

/* Returns EXPR1 <=> EXPR2 in order given by lexicografical extension
   of the following ordering:

   integer constants by value < other constants < regs by number <
   value_at by register and insn id < other rtxes by code < iteration.
   Note that the fact that we return 0 does not neccessarily imply that
   expressions are equal.  */
static int
iv_compare_rtx (expr1, expr2)
     rtx expr1;
     rtx expr2;
{
  int i, length, cmp;
  const char *format;

  if (rtx_equal_p (expr1, expr2))
    return 0;

  if (GET_CODE (expr1) == CONST_INT)
    {
      if (GET_CODE (expr2) != CONST_INT)
	return -1;
      return INTVAL (expr1) < INTVAL (expr2) ? -1 : 1;
    }
  if (GET_CODE (expr2) == CONST_INT)
    return 1;

  if (CONSTANT_P (expr1))
    return CONSTANT_P (expr2) ? 0 : -1;
  if  (CONSTANT_P (expr2))
    return 1;

  if (REG_P (expr1))
    {
      if (!REG_P (expr2))
	return -1;
      return REGNO (expr1) < REGNO (expr2) ? -1 : 1;
    }
  if (REG_P (expr2))
    return 1;

  if (GET_CODE (expr1) == VALUE_AT)
    {
      if (GET_CODE (expr2) != VALUE_AT)
	return -1;
      if (XINT (expr1, 0) < XINT (expr2, 0))
	return -1;
      if (XINT (expr1, 0) > XINT (expr2, 0))
	return 1;
      return XINT (expr1, 0) < XINT (expr2, 0) ? -1 : 1;
    }

  if (GET_CODE (expr2) == VALUE_AT)
    return 1;

  if (GET_CODE (expr1) == ITERATION)
    return 1;
  if (GET_CODE (expr2) == ITERATION)
    return -1;

  if (GET_CODE (expr1) < GET_CODE (expr2))
    return -1;
  if (GET_CODE (expr1) > GET_CODE (expr2))
    return 1;

  /* We prefer rightmost operand here, so that expressions that differ
     only in the value of constant term are sorted adjacent in
     iv_simplify_plus.  */
  length = GET_RTX_LENGTH (GET_CODE (expr1));
  format = GET_RTX_FORMAT (GET_CODE (expr1));
  for (i = length - 1; i >= 0; i--)
    {
      switch (format[i])
	{
	case 'e':
	  cmp = iv_compare_rtx (XEXP (expr1, i), XEXP (expr2, i));
	  if (cmp)
	    return cmp;

	default:
	  break;
	}
    }
  return 0;
}

/* Given expression in shape (((((a op b) op c) op d) op e) op f), return
   expression in the same shape but the operands sorted according to
   iv_compare_rtx.  The sorting is done in-place using bubble sort.  */
static rtx
sort_ops (expr)
     rtx expr;
{
  rtx pos, *op1, *op2, tmp;
  enum rtx_code code = GET_CODE (expr);
  int changed = true;

  if (code != PLUS
      && code != MULT)
    return expr;

  while (changed)
    {
      changed = false;
      pos = expr;
      while (GET_CODE (pos) == code)
	{
	  op1 = &XEXP (pos, 1);
	  op2 = GET_CODE (XEXP (pos, 0)) == code
	    ? &XEXP (XEXP (pos, 0), 1)
	    : &XEXP (pos, 0);

	  if (iv_compare_rtx (*op1, *op2) < 0)
	    {
	      tmp = *op1;
	      *op1 = *op2;
	      *op2 = tmp;
	      changed = true;
	    }
	  pos = XEXP (pos, 0);
	}
    }
  return expr;
}

/* Given expression in shape ((a op b) op c) op ((d op e) op f),
   transform it into (((((a op b) op c) op d) op e) op f).  The straightening
   is done in-place.  */
static rtx
straighten_ops (expr)
     rtx expr;
{
  rtx tmp;
  enum rtx_code code = GET_CODE (expr);

  if (code != PLUS
      && code != MULT)
    return expr;

  while (GET_CODE (XEXP (expr, 1)) == code)
    {
      tmp = expr;
      expr = XEXP (expr, 1);
      XEXP (tmp, 1) = XEXP (expr, 1);
      XEXP (expr, 1) = XEXP (expr, 0);
      XEXP (expr, 0) = tmp;
    }

  return expr;
}

/* EXPR is product of two expressions in the format described at
   iv_simplify_rtx.  Use distributive law to express it as sum of
   products.  The transformation is partially done in-place.  */
static rtx
use_distributive_law (expr)
     rtx expr;
{
  rtx exprl, exprr, aexprr, ml, mr, prod, tmp, *act;
  rtx result = NULL_RTX, *ares = &result;
  enum machine_mode mode = GET_MODE (expr);
  
  exprl = XEXP (expr, 0);
  exprr = XEXP (expr, 1);

  if (GET_CODE (exprr) != PLUS)
    {
      if (GET_CODE (exprl) != PLUS)
	return straighten_ops (expr);
      tmp = exprl;
      exprl = exprr;
      exprr = tmp;
    }

  while (GET_CODE (exprl) == PLUS)
    {
      aexprr = exprr;
      while (1)
	{
	  ml = copy_rtx (XEXP (exprl, 1));
	  mr = GET_CODE (aexprr) == PLUS
		  ? copy_rtx (XEXP (aexprr, 1))
		  : copy_rtx (aexprr);
	  prod = straighten_ops (gen_rtx_fmt_ee (MULT, mode, ml, mr));
	  *ares = gen_rtx_fmt_ee (PLUS, mode, NULL_RTX, prod);
	  ares = &XEXP (*ares, 0);

	  if (GET_CODE (aexprr) != PLUS)
	    break;
	  aexprr = XEXP (aexprr, 0);
	}
      exprl = XEXP (exprl, 0);
    }

  act = &exprr;
  while (GET_CODE (*act) == PLUS)
    {
      ml = copy_rtx (exprl);
      mr = XEXP (*act, 1);
      tmp = straighten_ops (gen_rtx_fmt_ee (MULT, mode, ml, mr));
      XEXP (*act, 1) = tmp;
      act = &XEXP (*act, 0);
    }
  *act = straighten_ops (gen_rtx_fmt_ee (MULT, mode, exprl, *act));
  *ares = exprr;
  
  return result;
}

/* Simplify expression EXPR (some product).  We just fold constants and
   sort operands according to iv_compare_rtx here.  The simplification
   is done in-place.  */
static rtx
iv_simplify_mult (expr)
     rtx expr;
{
  enum machine_mode mode = GET_MODE (expr);
  rtx folded_constant = const1_rtx;
  rtx *act = &expr, *last = NULL;
  rtx op;

  if (GET_CODE (expr) != MULT)
    return expr;

  while (GET_CODE (*act) == MULT)
    {
      op = XEXP (*act, 1);
      if (GET_CODE (op) == CONST_INT)
	{
	  folded_constant = simplify_gen_binary (MULT, mode,
						 folded_constant, op);
	  *act = XEXP (*act, 0);
	}
      else
	{
	  last = act;
	  act = &XEXP (*act, 0);
	}
    }
  if (GET_CODE (*act) == CONST_INT)
    {
      folded_constant = simplify_gen_binary (MULT, mode,
					     folded_constant, *act);
      if (!last || folded_constant == const0_rtx)
	return folded_constant;
      if (folded_constant == const1_rtx)
	*last = XEXP (*last, 1);
      else
	*act = folded_constant;
    }
  else if (folded_constant == const0_rtx)
    return const0_rtx;
  else if (folded_constant != const1_rtx)
    *act = gen_rtx_fmt_ee (MULT, mode, folded_constant, *act);
  
  if (GET_CODE (expr) == MULT)
    expr = sort_ops (expr);

  return expr;
}

/* If EXPR1 and EXPR2 differ only by a multiplicative constant,
   multiply FOLDED_CONSTANT by the one that is inside EXPR2.  */
static bool
combine_constants (expr1, expr2, mode, folded_constant)
     rtx expr1;
     rtx expr2;
     enum machine_mode mode;
     rtx *folded_constant;
{
  rtx cnst;

  while (GET_CODE (expr1) == MULT && GET_CODE (expr2) == MULT)
    {
      if (!rtx_equal_p (XEXP (expr1, 1), XEXP (expr2, 1)))
	return false;
      expr1 = XEXP (expr1, 0);
      expr2 = XEXP (expr2, 0);
    }

  if (GET_CODE (expr1) == MULT)
    {
      if (GET_CODE (XEXP (expr1, 0)) != CONST_INT)
	return false;
      if (!rtx_equal_p (XEXP (expr1, 1), expr2))
	return false;
      cnst = const1_rtx;
    }
  else if (GET_CODE (expr2) == MULT)
    {
      cnst = XEXP (expr2, 0);
      if (GET_CODE (cnst) != CONST_INT)
	return false;
      if (!rtx_equal_p (expr1, XEXP (expr2, 1)))
	return false;
    }
  else if (GET_CODE (expr1) == CONST_INT && GET_CODE (expr2) == CONST_INT)
    cnst = expr2;
  else if (rtx_equal_p (expr1, expr2))
    cnst = const1_rtx;
  else return false;

  *folded_constant = simplify_gen_binary (PLUS, mode, cnst, *folded_constant);
  return true;
}

/* Simplify expression EXPR (sum of products in iv_simplify_rtx shape).  We
   fold constants, transform c1 * x + c2 * x into (result of c1 + c2) * x,
   (where c_i are constants) and sort operands according to
   iv_compare_rtx.  The simplification is done in-place.  */
static rtx
iv_simplify_plus (expr)
     rtx expr;
{
  enum machine_mode mode = GET_MODE (expr);
  rtx *act1, *pact2, *act2, op, *tmp, *last;
  rtx old_folded_constant, folded_constant;

  if (GET_CODE (expr) != PLUS)
    return expr;

  /* Transform c1 * x + c2 * x -> (c1 + c2) * x.  */
  act1 = &expr;
  while (GET_CODE (*act1) == PLUS)
    {
      if (GET_CODE (XEXP (*act1, 1)) == CONST_INT)
	{
	  act1 = &XEXP (*act1, 0);
	  continue;
	}
      op = XEXP (*act1, 1);
      while (GET_CODE (op) == MULT)
	op = XEXP (op, 0);
      if (GET_CODE (op) == CONST_INT)
	folded_constant = op;
      else
	folded_constant = const1_rtx;
      op = XEXP (*act1, 1);
      old_folded_constant = folded_constant;
	
      pact2 = &XEXP (*act1, 0);
      while (pact2)
	{
	  if (GET_CODE (*pact2) == PLUS)
	    {
	      act2 = &XEXP (*pact2, 1);
	      pact2 = &XEXP (*pact2, 0);
	    }
	  else
	    {
	      act2 = pact2;
	      pact2 = NULL;
	    }
	  if (GET_CODE (*act2) == CONST_INT)
	    continue;
	  if (combine_constants (op, *act2, mode, &folded_constant))
	    *act2 = const0_rtx;
	}

      if (folded_constant == const0_rtx)
	XEXP (*act1, 1) = const0_rtx;
      else if (INTVAL (folded_constant) != INTVAL (old_folded_constant))
	{
	  last = NULL;
	  tmp = &XEXP (*act1, 1);
	  while (GET_CODE (*tmp) == MULT)
	    {
	      last = tmp;
	      tmp = &XEXP (*tmp, 0);
	    }
	  if (folded_constant == const1_rtx)
	    {
	      if (!last)
		abort ();
	      *last = XEXP (*last, 1);
	    }
	  else if (GET_CODE (*tmp) == CONST_INT)
	    *tmp = folded_constant;
	  else
	    *tmp = gen_rtx_fmt_ee (MULT, mode, folded_constant, *tmp);
	}
      act1 = &XEXP (*act1, 0);
    }

  /* Fold constants.  */
  if (GET_CODE (expr) != PLUS)
    return expr;
  act1 = &expr;
  folded_constant = const0_rtx;
  last = NULL;

  while (GET_CODE (*act1) == PLUS)
    {
      op = XEXP (*act1, 1);
      if (GET_CODE (op) == CONST_INT)
	{
	  folded_constant = simplify_gen_binary (PLUS, mode,
						 folded_constant, op);
	  *act1 = XEXP (*act1, 0);
	}
      else
	{
	  last = act1;
	  act1 = &XEXP (*act1, 0);
	}
    }
  if (GET_CODE (*act1) == CONST_INT)
    {
      folded_constant = simplify_gen_binary (PLUS, mode,
					     folded_constant, *act1);
      if (!last)
	return folded_constant;
      if (folded_constant == const0_rtx)
	*last = XEXP (*last, 1);
      else
	*act1 = folded_constant;
    }
  else if (folded_constant != const0_rtx)
    *act1 = gen_rtx_fmt_ee (PLUS, mode, folded_constant, *act1);

  if (GET_CODE (expr) == PLUS)
    expr = sort_ops (expr);

  return expr;
}

/* Tries to simplify MODE subreg of EXPR of mode EXPR_MODE (important for
   constants).  The simplification is done in place.  */
rtx
iv_simplify_subreg (expr, expr_mode, mode)
     rtx expr;
     enum machine_mode expr_mode;
     enum machine_mode mode;
{
  enum machine_mode inner_mode;

  if (expr_mode == mode)
    return expr;
  switch (GET_CODE (expr))
    {
    case PLUS:
    case MULT:
      XEXP (expr, 0) = iv_simplify_subreg (XEXP (expr, 0), expr_mode, mode);
      XEXP (expr, 1) = iv_simplify_subreg (XEXP (expr, 1), expr_mode, mode);
      PUT_MODE (expr, mode);
      return expr;

    case ITERATION:
      PUT_MODE (expr, mode);
      return expr;

    case CONST_INT:
      return simplify_gen_subreg (mode, expr, expr_mode, 0);

    case SIGN_EXTEND:
    case ZERO_EXTEND:
      inner_mode = GET_MODE (XEXP (expr, 0));
      if (GET_MODE_SIZE (inner_mode) < GET_MODE_SIZE (mode))
	{
	  PUT_MODE (expr, mode);
	  return expr;
	}
      return iv_simplify_subreg (XEXP (expr, 0), inner_mode, mode);

    case SUBREG:
      inner_mode = GET_MODE (XEXP (expr, 0));
      if (GET_MODE_SIZE (expr_mode) >= GET_MODE_SIZE (inner_mode)
	  || XINT (expr, 1) != 0)
	break;

      return iv_simplify_subreg (XEXP (expr, 0), inner_mode, mode);

    default:
      break;
    }

  return gen_rtx_fmt_ei (SUBREG, mode, expr, 0);
}

/* Return const0_rtx if the comparison CODE with constant argument PAR is
   always false for operand in INNER_MODE extended to MODE, if the PAR is
   on right side.  If it is always true, return const_true_rtx.  Otherwise
   return NULL.  */
static rtx
compare_with_mode_bounds (code, par, mode, inner_mode)
     enum rtx_code code;
     rtx par;
     enum machine_mode mode;
     enum machine_mode inner_mode;
{
  rtx mmin, mmax, rlow, rhigh;
  int sign;

  switch (code)
    {
    case LTU:
    case GTU:
    case LEU:
    case GEU:
      sign = 0;
      break;
    default:
      sign = 1;
    }
  get_mode_bounds (inner_mode, sign, &mmin, &mmax);
  switch (code)
    {
    case EQ:
      if (simplify_gen_relational (LT, SImode, mode,
				   par, mmin) == const_true_rtx
	  || simplify_gen_relational (GT, SImode, mode,
				      par, mmax) == const_true_rtx)
	return const0_rtx;
      break;
    case NE:
      if (simplify_gen_relational (LT, SImode, mode,
				   par, mmin) == const_true_rtx
	  || simplify_gen_relational (GT, SImode, mode,
				      par, mmax) == const_true_rtx)
	return const_true_rtx;
      break;
      
    default:
      rlow = simplify_gen_relational (code, SImode, mode, par, mmin);
      rhigh = simplify_gen_relational (code, SImode, mode, par, mmax);
      if (rlow == rhigh)
	return rlow;
    }

  return NULL_RTX;
}

/* Attempt to simplify relational expression EXPR, whose operands are
   in mode INNER_MODE.  The simplification is done in place.  */
static rtx
iv_simplify_relational (expr, inner_mode)
     rtx expr;
     enum machine_mode inner_mode;
{
  rtx left, right, comp;
  enum rtx_code code, left_code, right_code, extend;
  enum machine_mode result_mode, extended_mode;

  /* If one of operands of comparison is {sign,zero}_extend and the other
     one is either constant or the same extend, we simplify them to work
     on the inner mode of extend instead.  */

  if (GET_MODE_CLASS (inner_mode) != MODE_INT
      && GET_MODE_CLASS (inner_mode) != MODE_PARTIAL_INT)
    return expr;

  code = GET_CODE (expr);
  if (!TEST_BIT (suitable_code, code))
      return NULL_RTX;
  result_mode = GET_MODE (expr);
  left = XEXP (expr, 0);
  left_code = GET_CODE (left);
  right = XEXP (expr, 1);
  right_code = GET_CODE (right);

  switch (left_code)
    {
    case CONST_INT:
      extend = NIL;
      extended_mode = VOIDmode;
      break;

    case SIGN_EXTEND:
    case ZERO_EXTEND:
      extend = left_code;
      extended_mode = GET_MODE (XEXP (left, 0));
      break;

    default:
     return expr; 
    }

  switch (right_code)
    {
    case CONST_INT:
      if (extend == NIL)
	return simplify_gen_relational (code, result_mode, VOIDmode,
					left, right);
      comp = compare_with_mode_bounds (swap_condition (code),
				       right, inner_mode, extended_mode);
      if (comp)
	return comp;
      XEXP (expr, 0) = XEXP (left, 0);
      return expr;

    case SIGN_EXTEND:
    case ZERO_EXTEND:
      if (extend == NIL)
	{
	  extended_mode = GET_MODE (XEXP (right, 0));
	  comp = compare_with_mode_bounds (code, left,
					   inner_mode, extended_mode);
	  if (comp)
	    return comp;
	  XEXP (expr, 1) = XEXP (right, 0);
	  return expr;
	}

      if (extend != right_code
	  || extended_mode != GET_MODE (XEXP (right, 0)))
	return expr;
      break;

    default:
     return expr; 
    }

  XEXP (expr, 0) = XEXP (left, 0);
  XEXP (expr, 1) = XEXP (right, 0);
  return expr;
}

/* Attempt to bring EXPR into canonical shape described below.  It would
   be nice if we could use simplify_rtx for it; but it is too low level
   for our purposes, and does basically the reverse of transformations
   we want.  The simplification is partly done in-place.  */
rtx
iv_simplify_rtx (expr)
     rtx expr;
{
  enum machine_mode mode, inner_mode = VOIDmode;
  HOST_WIDE_INT val;
  int i, length;
  const char *format;
  rtx tmp, *current;
  enum rtx_code code;

  if (!expr)
    return expr;
  mode = GET_MODE (expr);

  /* The shape of the resulting expression is

     expr = mexp | expr + mexp
     mexp = whatever | nexp * whatever

     Operands to commutative operations and comparisons are ordered
     according to iv_compare_rtx inside whatevers.  The list of mexp
     operands in expr, as well as list of whatever operands in mexp
     is in this order as well.
     
     We do also a few other optimizations on subregs and extends.  */

  /* Do some canonicalization.  */
  code = GET_CODE (expr);
  switch (code)
    {
    case MINUS:
      /* (MINUS x y) == (PLUS x (MULT (-1) y))  */
      tmp = XEXP (expr, 1);
      tmp = gen_rtx_fmt_ee (MULT, mode, constm1_rtx, tmp);
      expr = gen_rtx_fmt_ee (PLUS, mode, XEXP (expr, 0), tmp);
      break;
      
    case NEG:
      /* (NEG x) == (MULT (-1) x)  */
      tmp = XEXP (expr, 0);
      expr = gen_rtx_fmt_ee (MULT, mode, constm1_rtx, tmp);
      break;

    case ASHIFT:
      /* (ASHIFT x const) == (MULT (2^const) x)  */
      tmp = XEXP (expr, 1);
      if (GET_CODE (tmp) != CONST_INT)
	break;
      val = INTVAL (tmp);
      expr = gen_rtx_fmt_ee (MULT, mode, GEN_INT (1 << val), XEXP (expr, 0));
      break;

    case NOT:
      /* (NOT x) == (PLUS (-1) (NEG x)) */
      tmp = XEXP (expr, 0);
      tmp = gen_rtx_fmt_e (NEG, mode, tmp);
      expr = gen_rtx_fmt_ee (PLUS, mode, constm1_rtx, tmp);
      break;

    case CONST:
      /* Throw away consts so that we see what is inside.  */
      return iv_simplify_rtx (XEXP (expr, 0));

    case CONST_INT:
    case SYMBOL_REF:
    case INITIAL_VALUE:
    case REG:
    case ITERATION:
      return expr;

    case SUBREG:
    case ZERO_EXTEND:
    case SIGN_EXTEND:
      /* Remember mode, as it is lost when the inner expression is simplified
	 to constant.  */
      inner_mode = GET_MODE (XEXP (expr, 0));
      break;

    default: ;
    }
  code = GET_CODE (expr);

  if (GET_RTX_CLASS (code) == '<')
    {
      /* Remember the mode of operands, for purposes of constant folding.  */
      inner_mode = GET_MODE (XEXP (expr, 0));
      if (inner_mode == VOIDmode)
	inner_mode = GET_MODE (XEXP (expr, 1));
    }

  /* Simplify subexpressions.  */
  length = GET_RTX_LENGTH (code);
  format = GET_RTX_FORMAT (code);
  for (i = 0; i < length; i++)
    {
      switch (format[i])
	{
	case 'e':
	  XEXP (expr, i) = iv_simplify_rtx (XEXP (expr, i));
	  break;

	default:
	  break;
	}
    }

  switch (code)
    {
    case MULT:
      expr = use_distributive_law (expr);
      current = &expr;
      while (GET_CODE (*current) == PLUS)
	{
	  XEXP (*current, 1) = iv_simplify_mult (XEXP (*current, 1));
	  current = &XEXP (*current, 0);
	}
      *current = iv_simplify_mult (*current);

      if (GET_CODE (expr) != PLUS)
	return expr;

      /* Fallthru.  */
    case PLUS:
      /* Straighten the list of summands.  */
      expr = straighten_ops (expr);
      expr = iv_simplify_plus (expr);
      return expr;

    case SUBREG:
      if (GET_MODE_SIZE (mode) >= GET_MODE_SIZE (inner_mode)
	  || XINT (expr, 1) != 0)
	break;

      expr = iv_simplify_subreg (XEXP (expr, 0), inner_mode, mode);
      if (GET_CODE (expr) != SUBREG)
	expr = iv_simplify_rtx (expr);
      return expr;

    case DIV:
    case UDIV:
      if (XEXP (expr, 1) == const1_rtx)
	expr = XEXP (expr, 0);
      break;

    case MOD:
    case UMOD:
      if (XEXP (expr, 1) == const1_rtx)
	expr = const0_rtx;
      break;

    default:
      if (GET_RTX_CLASS (code) == '<')
	expr = iv_simplify_relational (expr, inner_mode);
    }
  code = GET_CODE (expr);

  /* Fold constants.  */
  length = GET_RTX_LENGTH (code);
  format = GET_RTX_FORMAT (code);
  for (i = 0; i < length; i++)
    switch (format[i])
      {
      case 'e':
	if (GET_CODE (XEXP (expr, i)) != CONST_INT)
	  i = length + 1;
	break;
      case 'i':
	break;
      default:
	i = length + 1;
      }
  if (i == length)
    {
      switch (code)
	{
	case SUBREG:
	  tmp = simplify_gen_subreg (mode, XEXP (expr, 0),
				     inner_mode, XINT (expr, 1));
	  break;
	case SIGN_EXTEND:
	case ZERO_EXTEND:
	  tmp = simplify_gen_unary (code, mode, XEXP (expr, 0), inner_mode);
	  break;
	default:
	    tmp = simplify_rtx (expr);
	}
      if (tmp && GET_CODE (tmp) == CONST_INT)
	return tmp;
    }

  /* Sort the operands if possible.  */
  if (GET_RTX_CLASS (code) == 'c')
    {
      if (iv_compare_rtx (XEXP (expr, 0), XEXP (expr, 1)) > 0)
	{
     	  tmp = XEXP (expr, 0);
	  XEXP (expr, 0) = XEXP (expr, 1);
	  XEXP (expr, 1) = tmp;
	}
    }
  else if (GET_RTX_CLASS (code) == '<')
    {
      code = swap_condition (code);
      if (code != UNKNOWN
	  && iv_compare_rtx (XEXP (expr, 0), XEXP (expr, 1)) > 0)
	{
	  PUT_CODE (expr, code);
     	  tmp = XEXP (expr, 0);
	  XEXP (expr, 0) = XEXP (expr, 1);
	  XEXP (expr, 1) = tmp;
	}
    }

  return expr;
}

/* Initialize table of codes we are able to process.  */
static void
init_suitable_codes ()
{
  if (suitable_code)
    return;

  suitable_code = sbitmap_alloc (NUM_RTX_CODE);
  sbitmap_zero (suitable_code);
  SET_BIT (suitable_code, CONST_INT);
  SET_BIT (suitable_code, CONST);
  SET_BIT (suitable_code, REG);
  SET_BIT (suitable_code, SYMBOL_REF);
  SET_BIT (suitable_code, IF_THEN_ELSE);
  SET_BIT (suitable_code, PLUS);
  SET_BIT (suitable_code, MINUS);
  SET_BIT (suitable_code, NEG);
  SET_BIT (suitable_code, MULT);
  SET_BIT (suitable_code, DIV);
  SET_BIT (suitable_code, MOD);
  SET_BIT (suitable_code, UMOD);
  SET_BIT (suitable_code, UDIV);
  SET_BIT (suitable_code, AND);
  SET_BIT (suitable_code, IOR);
  SET_BIT (suitable_code, NOT);
  SET_BIT (suitable_code, ASHIFT);
  SET_BIT (suitable_code, ASHIFTRT);
  SET_BIT (suitable_code, LSHIFTRT);
  SET_BIT (suitable_code, NE);
  SET_BIT (suitable_code, EQ);
  SET_BIT (suitable_code, GE);
  SET_BIT (suitable_code, GT);
  SET_BIT (suitable_code, LE);
  SET_BIT (suitable_code, LT);
  SET_BIT (suitable_code, GEU);
  SET_BIT (suitable_code, GTU);
  SET_BIT (suitable_code, LEU);
  SET_BIT (suitable_code, LTU);
  SET_BIT (suitable_code, SUBREG);
  SET_BIT (suitable_code, SIGN_EXTEND);
  SET_BIT (suitable_code, ZERO_EXTEND);
  SET_BIT (suitable_code, INITIAL_VALUE);
  SET_BIT (suitable_code, VALUE_AT);
  SET_BIT (suitable_code, ITERATION);
}

/* Substitutes values from SUBSTITUTION into EXPR.  If SIMPLIFY is true,
   also simplify the resulting expression.  RESULT_MODE indicates the
   real mode of target of resulting expression (used when simplifying).  */
rtx
substitute_into_expr (expr, substitution, simplify)
     rtx expr;
     rtx *substitution;
     int simplify;
{
  rtx new_expr, sub_expr;
  unsigned regno;
  int i, length;
  const char *format;
  enum machine_mode mode, inner_mode = VOIDmode;
  enum rtx_code code;
 
  if (!expr)
    return NULL_RTX;

  mode = GET_MODE (expr);
  if (mode != VOIDmode
      && GET_MODE_CLASS (mode) != MODE_INT
      && GET_MODE_CLASS (mode) != MODE_PARTIAL_INT)
    return NULL_RTX;

  if (GET_CODE (expr) == INITIAL_VALUE)
    expr = XEXP (expr, 0);

  code = GET_CODE (expr);
  if (code == REG)
    {
      rtx val;
      regno = REGNO (expr);
      if (!TEST_BIT (interesting_reg, regno))
	return NULL_RTX;

      val = substitution[regno];
      if (!val)
	return NULL_RTX;

      /* Optimize some common cases that may be shared.  */
      switch (GET_CODE (val))
	{
	case INITIAL_VALUE:
	case VALUE_AT:
	case CONST_INT:
	case SYMBOL_REF:
	  return val;

	default:
	  return copy_rtx (val);
	}
    }
  
  /* Just ignore the codes that do not seem to be good for further
     processing.  */
  if (!TEST_BIT (suitable_code, code))
      return NULL_RTX;

  /* Ensure that these constants are shared.  */
  if (good_constant_p (expr))
    return expr;

  if (code == SUBREG || code == ZERO_EXTEND || code == SIGN_EXTEND)
    inner_mode = GET_MODE (XEXP (expr, 0));

  length = GET_RTX_LENGTH (code);
  format = GET_RTX_FORMAT (code);
  new_expr = shallow_copy_rtx (expr);
  for (i = 0; i < length; i++)
    {
      switch (format[i])
	{
	case 'e':
	  sub_expr = substitute_into_expr (XEXP (expr, i), substitution, false);
	  if (!sub_expr)
	    return NULL_RTX;
	  XEXP (new_expr, i) = sub_expr;
	  break;

	case 'V':
	case 'E':
	  return NULL_RTX;

	default:
	  /* Nothing to do.  */
	  break;
	}
    }

  if ((code == SUBREG || code == ZERO_EXTEND || code == SIGN_EXTEND)
      && inner_mode != VOIDmode
      && GET_MODE (XEXP (new_expr, 0)) == VOIDmode)
    {
      if (code == SUBREG)
	new_expr = simplify_gen_subreg (mode, XEXP (new_expr, 0),
					inner_mode, XINT (expr, 1));
      else
	new_expr = simplify_gen_unary (code, mode,
				       XEXP (new_expr, 0), inner_mode);
    }

  if (simplify)
    new_expr = iv_simplify_rtx (new_expr);

  return new_expr;
}

/* Called through for_each_rtx from iv_omit_initial_values.  */
static int
iv_omit_initial_values_1 (expr, data)
     rtx *expr;
     void *data ATTRIBUTE_UNUSED;
{
  if (GET_CODE (*expr) == INITIAL_VALUE)
    {
      *expr = XEXP (*expr, 0);
      return -1;
    }
  return 0;
}

/* Omits initial_values from the expression EXPR.  */
rtx
iv_omit_initial_values (expr)
     rtx expr;
{
  expr = copy_rtx (expr);
  for_each_rtx (&expr, iv_omit_initial_values_1, NULL);
  return expr;
}

/* Splits expression for induction variable into BASE and STEP.  We expect
   EXPR to come from iv_simplify_rtx.  EXPR is not modified, but part of
   it may be shared as BASE or STEP.  */
void
iv_split (expr, base, step)
     rtx expr;
     rtx *base;
     rtx *step;
{
  rtx abase, astep, *pabase = &abase, *pastep = &astep;
  rtx next, *act, tmp;
  enum machine_mode mode;

  *base = *step = NULL_RTX;
  if (!expr)
    return;
  mode = GET_MODE (expr);

  if (good_constant_p (expr)
      || GET_CODE (expr) == VALUE_AT
      || GET_CODE (expr) == INITIAL_VALUE
      || !expr_mentions_iteration_p (expr))
    {
      *base = expr;
      *step = const0_rtx;
      return;
    }

  /* Avoid copying in the most common cases:  */
  if (GET_CODE (expr) != PLUS
      || !expr_mentions_iteration_p (XEXP (expr, 0)))
    {
      if (GET_CODE (expr) == PLUS)
	{
	  *base = XEXP (expr, 0);
	  tmp = XEXP (expr, 1);
	}
      else
	{
	  *base = const0_rtx;
	  tmp = expr;
	}

      if (GET_CODE (tmp) == ITERATION)
	*step = const1_rtx;
      else if (GET_CODE (tmp) == MULT
	       && GET_CODE (XEXP (tmp, 0)) == CONST_INT
	       && GET_CODE (XEXP (tmp, 1)) == ITERATION)
	*step = XEXP (tmp, 0);
      else
	*base = NULL_RTX;

      if (*base)
	return;
    }

  expr = copy_rtx (expr);
  while (GET_CODE (expr) == PLUS)
    {
      next = XEXP (expr, 0);
      act = &XEXP (expr, 1);
      if (GET_CODE (*act) == MULT)
	act = &XEXP (*act, 1);
      if (GET_CODE (*act) == ITERATION)
	{
	  *act = const1_rtx;
	  *pastep = expr;
	  pastep = &XEXP (expr, 0);
	}
      else
	{
	  *pabase = expr;
	  pabase = &XEXP (expr, 0);
	}
      expr = next;
    }
  if (GET_CODE (expr) == ITERATION)
    {
      *pastep = const1_rtx;
      *pabase = const0_rtx;
    }
  else if (GET_CODE (expr) == MULT
	   && GET_CODE (XEXP (expr, 1)) == ITERATION)
    {
      *pastep = expr;
      XEXP (expr, 1) = const1_rtx;
      *pabase = const0_rtx;
    }
  else
    {
      *pastep = const0_rtx;
      *pabase = expr;
    }

  if (expr_mentions_iteration_p (astep)
      || expr_mentions_iteration_p (abase))
    return;

  *base = iv_simplify_rtx (abase);
  *step = iv_simplify_rtx (astep);
}

/* Return step of induction variable EXPR, NULL_RTX if the shape of EXPR
   is not recognized as induction variable.  */
rtx
iv_step (expr)
     rtx expr;
{
  rtx base, step;
  iv_split (expr, &base, &step);
  return step;
}

/* Return base of induction variable EXPR, NULL_RTX if the shape of EXPR
   is not recognized as induction variable.  */
rtx
iv_base (expr)
     rtx expr;
{
  rtx base, step;
  iv_split (expr, &base, &step);
  return base;
}

/* Check whether EXPR is in shape corresponding to induction variable.  */
bool
iv_simple_p (expr)
     rtx expr;
{
  rtx base, step;
  iv_split (expr, &base, &step);
  if (!base)
    return false;

  return true;
}

/* Checks whether all registers used to compute EXPR are unchanging according
   to VALUES.  */
static bool
invariant_wrto_ivs_p (expr, values)
     rtx expr;
     rtx *values;
{
  unsigned regno;
  int i, length;
  const char *format;
 
  if (!expr)
    return false;

  switch (GET_CODE (expr))
    {
    case INITIAL_VALUE:
      regno = REGNO (XEXP (expr, 0));
      return (TEST_BIT (interesting_reg, regno)
	      && values[regno] == gen_initial_value (regno));

    case VALUE_AT:
      return false;

    default: ;
    }

  length = GET_RTX_LENGTH (GET_CODE (expr));
  format = GET_RTX_FORMAT (GET_CODE (expr));
  for (i = 0; i < length; i++)
    {
      switch (format[i])
	{
	case 'e':
	  if (!invariant_wrto_ivs_p (XEXP (expr, i), values))
	    return false;
	  break;

	case 'V':
	case 'E':
	  abort ();

	default:
	  /* Nothing to do.  */
	  break;
	}
    }

  return true;
}

/* Simulate effect of one set on values passed in DATA.  Called through
   note_stores.  The global variable CURRENT_INSN is used to provide the
   insn inside that the set occurs.  In ACT_MODIFIED_REGS a bitmap of registers
   whose values were altered is stored.  */
static rtx current_insn;
static sbitmap act_modified_regs;

static void
simulate_set (reg, set, data)
     rtx reg;
     rtx set;
     void *data;
{
  rtx *values = (rtx *) data;
  rtx src, value;
  unsigned regno;

  if (!REG_P (reg))
    return;
  regno = REGNO (reg);

  if (!TEST_BIT (interesting_reg, regno))
    return;

  SET_BIT (act_modified_regs, regno);
  if (GET_CODE (set) == CLOBBER)
    value = NULL_RTX;
  else
    {
      src = SET_SRC (set);
      value = substitute_into_expr (src, values, true);
    }
  if (!value)
    value = gen_value_at (regno, current_insn, true);
  record_def_value (current_insn, regno, value);
}

/* Try to simplify induction variable VAR using register initial values stored
   in INITIAL_VALUES.  Returns the simplified form of VAR or VAR if no
   simplification is possible.  */
rtx
simplify_iv_using_values (var, initial_values)
     rtx var;
     rtx *initial_values;
{
  rtx base, step, sbase, sstep, wrap = NULL_RTX;
  int changed;
  enum machine_mode mode;

  if (good_constant_p (var)
      || GET_CODE (var) == VALUE_AT)
    return var;

  if (GET_CODE (var) == SIGN_EXTEND || GET_CODE (var) == ZERO_EXTEND)
    {
      wrap = var;
      var = XEXP (var, 0);
    }
  mode = GET_MODE (var);

  iv_split (var, &base, &step);
  if (!base)
    return NULL_RTX;

  changed = false;
  if (!good_constant_p (base))
    {
      sbase = substitute_into_expr (base, initial_values, true);
      if (sbase && good_constant_p (sbase))
	{
	  base = sbase;
	  changed = true;
	}
    }
  if (!good_constant_p (step))
    {
      sstep = substitute_into_expr (step, initial_values, true);
      if (sstep && good_constant_p (sstep))
	{
	  step = sstep;
	  changed = true;
	}
    }

  if (changed)
    {
      if (step == const0_rtx)
	var = base;
      else
	{
	  var = gen_iteration (mode);
	  if (step != const1_rtx)
	    var = gen_rtx_fmt_ee (MULT, mode, step, var);
	  if (base != const0_rtx)
	    var = gen_rtx_fmt_ee (PLUS, mode, base, var);
	}
    }

  if (wrap && GET_MODE (var) == mode)
    {
      XEXP (wrap, 0) = var;
      var = wrap;
    }
  return var;
}

/* Try to substitute initial values of registers (INITIAL_VALUES) into
   induction variables (VALUES) and keep those that result in constants.  */
static void
simplify_ivs_using_values (values, initial_values)
     rtx *values;
     rtx *initial_values;
{
  unsigned regno;
  rtx value, svalue;

  for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
    if (TEST_BIT (interesting_reg, regno))
      {
	value = values[regno];
	svalue = simplify_iv_using_values (value, initial_values);
	if (svalue && svalue != value)
	  values[regno] = svalue;
      }
}

/* Attempt to simplify the expression EXPR using initial values at entry of
   the LOOP.  If EXPR is list, then its elements are combined together through
   operation OP.  */
rtx
iv_simplify_using_initial_values (op, expr, loop)
     enum rtx_code op;
     rtx expr;
     struct loop *loop;
{
  rtx l, r, tmp;

  if (!expr || good_constant_p (expr))
    return expr;

  if (GET_CODE (expr) == EXPR_LIST)
    {
      l = iv_simplify_using_initial_values (NIL, XEXP (expr, 0), loop);
      r = iv_simplify_using_initial_values (op, XEXP (expr, 1), loop);

      switch (op)
	{
	case AND:
	  if (l == const0_rtx)
	    expr = alloc_EXPR_LIST (0, const0_rtx, NULL_RTX);
	  else if (l == const_true_rtx)
	    expr = r;
	  else if (r && XEXP (r, 0) == const0_rtx)
	    expr = r;
	  break;

	case IOR:
	  if (l == const_true_rtx)
	    expr = alloc_EXPR_LIST (0, const_true_rtx, NULL_RTX);
	  else if (l == const0_rtx)
	    expr = r;
	  else if (r && XEXP (r, 0) == const_true_rtx)
	    expr = r;
	  break;

	default:
	  abort ();
	}
      return expr;
    }

  tmp = substitute_into_expr (expr, initial_values[loop->num], true);
  if (tmp && good_constant_p (tmp))
    return tmp;

  return expr;
}

/* Clears stored values of registers (unless INCLUDING_TOP, ignore values
   in the fake outermost loop).  */
static void
clear_reg_values (including_top)
     int including_top;
{
  unsigned i;
  struct ref *ref;

  for (i = 0; i < df->n_defs; i++)
    {
      ref = df->defs[i];
      if (!including_top && !DF_REF_BB (ref)->loop_father->outer)
	continue;
      ref->aux = NULL_RTX;
    }

  for (i = 0; i < df->n_uses; i++)
    {
      ref = df->uses[i];
      if (!including_top && !DF_REF_BB (ref)->loop_father->outer)
	continue;
      ref->aux = NULL_RTX;
    }
}

/* Generates VALUE_AT for register REGNO as near to entry as possible, startin
   at basic block BB.  */
static rtx
earliest_value_at_for (bb, regno)
     basic_block bb;
     int regno;
{
  /* We may continue backwards as long as we have an unique predecessor;
     if the register was altered in any such block, it would have just this
     single definition and we would not be called.  */
  while (bb->pred && !bb->pred->pred_next)
    bb = bb->pred->src;
  return gen_value_at (regno, bb->head, false);
}

/* Computes value of register referenced by REF immediately before INSN in
   basic block BB. */
static rtx
get_reg_value_at (bb, insn, ref)
     basic_block bb;
     rtx insn;
     struct ref *ref;
{
  struct df_link *def;
  struct loop *loop = bb->loop_father, *def_loop;
  basic_block def_bb;
  rtx def_insn;
  unsigned regno = DF_REF_REGNO (ref);
  unsigned defno;

  /* There are three cases:
     -- a single definition inside loop strictly dominates us, and is not
	part of any subloop -- then this is the value we want.
     -- a definition inside loop that reaches us, or definition from inside
	of subloop -- then the value is unknown
     -- any number of other definitions (outside, or inside of loop but
	only reaching us through latch/outside of loop) -- then the
	value is the initial one.  */

  for (def = DF_REF_CHAIN (ref); def; def = def->next)
    {
      def_insn = DF_REF_INSN (def->ref);
      def_bb = BLOCK_FOR_INSN (def_insn);
      def_loop = def_bb->loop_father;
      defno = DF_REF_ID (def->ref);

      if (loop != def_loop)
	{
	  if (flow_loop_nested_p (loop, def_loop))
	    return earliest_value_at_for (bb, regno);
	  else
	    continue;
	}

      if (!TEST_BIT (loop_rd_in_ok, defno))
	fill_loop_rd_in_for_def (def->ref);

      /* The definition that dominates us.  */
      if (def_bb == bb
	  && DF_INSN_LUID (df, def_insn) < DF_INSN_LUID (df, insn))
	break;
      if (def_bb != bb && fast_dominated_by_p (dom, bb, def_bb))
	break;

      /* The definition that does not dominate us, but reaches us.  */
      if (bitmap_bit_p (loop_rd_in[bb->index], defno))
	return earliest_value_at_for (bb, regno);
    }

  if (!def)
    return loop_entry_values[loop->num][regno];

  /* The def dominates us.  If it is not the last one,  the remaining
     def(s) must reach us from inside of the loop and cannot dominate us.  */
  if (def->next)
    return earliest_value_at_for (bb, regno);

  /* Make sure the value is computed.  Strictly said the depth of recusion
     is unbounded, but usually the def chains are short and refer backwards
     to values we have already computed, so it should not cause problems.  */
  compute_reg_values (def_bb, def_insn);

  return get_def_value (def_insn, regno);
}

/* Store values of registers used in INSN to VALUES.  */
extern void
iv_load_used_values (insn, values)
     rtx insn;
     rtx *values;
{
  struct df_link *use = DF_INSN_USES (df, insn);
  unsigned regno;

  use = DF_INSN_USES (df, insn);
  for (; use; use = use->next)
    {
      if (!DF_REF_REG_USE_P (use->ref))
	continue;
      regno = DF_REF_REGNO (use->ref);
      values[regno] = (rtx) use->ref->aux;
    }
}

/* Computes values of registers stored at INSN in basic block BB.  */
static void
compute_reg_values (bb, insn)
     basic_block bb;
     rtx insn;
{
  struct df_link *use = DF_INSN_USES (df, insn);
  unsigned regno;

  if (!INSN_P (insn))
    return;

  if (TEST_BIT (insn_processed, INSN_UID (insn)))
    return;
  SET_BIT (insn_processed, INSN_UID (insn));

  /* First compute the values of used registers.  */
  for (; use; use = use->next)
    {
      if (!DF_REF_REG_USE_P (use->ref))
	continue;
      regno = DF_REF_REGNO (use->ref);
      if (!TEST_BIT (interesting_reg, regno))
	continue;

      use->ref->aux = (void *) get_reg_value_at (bb, insn, use->ref);
    }

  /* Now simulate the computation to fill in the values of defs.  */
  iv_load_used_values (insn, iv_register_values);
  current_insn = insn;
  act_modified_regs = modified_regs[bb->loop_father->num];
  note_stores (PATTERN (insn), simulate_set, (void *) iv_register_values);
}

/* Computes value of registers used/defined in all insns wrto the innermost
   loop they belong to; values of registers at loop entries are assumed
   to be stored in loop_entry_values.  Unless INCLUDING_TOP, fake loop
   around the function is ignored.  */
static void
compute_register_values (including_top)
     int including_top;
{
  basic_block bb;
  rtx insn;

  clear_reg_values (including_top);

  sbitmap_zero (insn_processed);
  FOR_EACH_BB (bb)
    {
      if (!including_top && !bb->loop_father->outer)
	continue;

      for (insn = bb->head;
	   insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	compute_reg_values (bb, insn);
    }
}

/* Simplifies values of registers stored at INSN in basic block BB.  */
static void
simplify_reg_values (bb, insn)
     basic_block bb;
     rtx insn;
{
  struct df_link *use = DF_INSN_USES (df, insn);
  struct df_link *def = DF_INSN_DEFS (df, insn);
  rtx svalue;
  rtx *values = initial_values[bb->loop_father->num];

  if (!INSN_P (insn))
    return;

  for (; use; use = use->next)
    {
      if (!use->ref->aux)
	continue;
      svalue = simplify_iv_using_values (use->ref->aux, values);
      if (svalue)
	use->ref->aux = svalue;
    }
  for (; def; def = def->next)
    {
      if (!def->ref->aux)
	continue;
      svalue = simplify_iv_using_values (def->ref->aux, values);
      if (svalue)
	def->ref->aux = svalue;
    }
}

/* Simplifies values of registers used/defined in all insns using the initial
   values of the loop they belong to.  */
static void
simplify_register_values ()
{
  basic_block bb;
  rtx insn;

  FOR_EACH_BB (bb)
    {
      if (!bb->loop_father->outer)
	continue;

      for (insn = bb->head;
	   insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	simplify_reg_values (bb, insn);
    }
}

/* Fill in loop_rd_in for a given DEF.  */
static void
fill_loop_rd_in_for_def (def)
     struct ref *def;
{
  unsigned defno = DF_REF_ID (def);
  basic_block def_bb = DF_REF_BB (def), dest;
  struct loop *def_loop = def_bb->loop_father;
  edge *stack, act;
  int stack_top = 0;
  edge latch = def_loop->outer ? loop_latch_edge (def_loop) : NULL;

  SET_BIT (loop_rd_in_ok, defno);
  if (!DF_REF_REG_DEF_P (def)
      || !TEST_BIT (interesting_reg, DF_REF_REGNO (def))
      || bitmap_bit_p (DF_BB_INFO (df, def_bb)->rd_kill, defno)
      || !def_bb->succ)
    return;

  stack = xmalloc (sizeof (edge) * n_basic_blocks);
  act = def_bb->succ;

  while (1)
    {
      for (; act; act = act->succ_next)
	{
	  dest = act->dest;
	  if (dest == EXIT_BLOCK_PTR
	      || act == latch
	      || !flow_bb_inside_loop_p (def_loop, dest)
	      || bitmap_bit_p (loop_rd_in[dest->index], defno))
	    continue;

	  bitmap_set_bit (loop_rd_in[dest->index], defno);
	  if (!bitmap_bit_p (DF_BB_INFO (df, dest)->rd_kill, defno))
	    break;
	}

      if (act)
	{
	  if (act->succ_next)
	    stack[stack_top++] = act->succ_next;
	  act = dest->succ;
	}
      else if (stack_top == 0)
	break;
      else act = stack[--stack_top];
    }
  free (stack);
}

/* Initialize variables used by the analysis.  */
void
initialize_iv_analysis (loops)
     struct loops *loops;
{
  unsigned i;
  basic_block bb;
  enum machine_mode mode;

  df = df_init ();
  df_analyse (df, 0, DF_UD_CHAIN | DF_RD);
  loop_rd_in = xmalloc (sizeof (bitmap) * last_basic_block);
  FOR_EACH_BB (bb)
    {
      loop_rd_in[bb->index] = BITMAP_XMALLOC ();
      bitmap_zero (loop_rd_in[bb->index]);
    }
  loop_rd_in_ok = sbitmap_alloc (df->n_defs);
  sbitmap_zero (loop_rd_in_ok);

  dom = create_fq_dominators (loops->cfg.dom);

  init_suitable_codes ();
  max_regno = max_reg_num ();

  interesting_reg = sbitmap_alloc (max_regno);
  sbitmap_zero (interesting_reg);
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    {
      mode = GET_MODE (regno_reg_rtx[i]);
      if (GET_MODE_CLASS (mode) == MODE_INT
	  || GET_MODE_CLASS (mode) == MODE_PARTIAL_INT)
	SET_BIT (interesting_reg, i);
    }

  initial_value_rtx = xmalloc (sizeof (rtx) * max_regno);
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    {
      mode = GET_MODE (regno_reg_rtx[i]);

      if (!TEST_BIT (interesting_reg, i))
	initial_value_rtx[i] = NULL_RTX;
      else
	initial_value_rtx[i] = gen_rtx_fmt_e (INITIAL_VALUE,
					      GET_MODE (regno_reg_rtx[i]),
					      regno_reg_rtx[i]);
    }
  for (i = 0; i < NUM_MACHINE_MODES; i++)
    iteration_rtx[i] = gen_rtx (ITERATION, i);

  modified_regs = sbitmap_vector_alloc (loops->num, max_regno);
  insn_processed = sbitmap_alloc (get_max_uid () + 1);
  iv_register_values = xmalloc (max_regno * sizeof (rtx));

  initial_values = xmalloc (sizeof (rtx *) * loops->num);
  loop_entry_values = xmalloc (sizeof (rtx *) * loops->num);
  loop_end_values = xmalloc (sizeof (rtx *) * loops->num);
  for (i = 0; i < loops->num; i++)
    if (loops->parray[i])
      {
	initial_values[i] = xmalloc (sizeof (rtx) * max_regno);
	loop_entry_values[i] = xmalloc (sizeof (rtx) * max_regno);
	loop_end_values[i] = xmalloc (sizeof (rtx) * max_regno);
      }
}

/* Free variables used by the analysis.  */
void
finalize_iv_analysis (loops)
     struct loops *loops;
{
  unsigned i;
  basic_block bb;

  sbitmap_vector_free (modified_regs);
  sbitmap_free (insn_processed);
  free (iv_register_values);

  sbitmap_free (interesting_reg);

  for (i = 0; i < loops->num; i++)
    if (loops->parray[i])
      {
	free (initial_values[i]);
	free (loop_entry_values[i]);
	free (loop_end_values[i]);
      }
  free (initial_values);
  free (loop_entry_values);
  free (loop_end_values);

  free (initial_value_rtx);

  df_finish (df);
  FOR_EACH_BB (bb)
    {
      BITMAP_XFREE (loop_rd_in[bb->index]);
    }
  free (loop_rd_in);
  sbitmap_free (loop_rd_in_ok);

  release_fq_dominators (dom);
}

/* Computes values of modified registers at end of LOOP, putting the result
   into VALUES.  */
static void
compute_loop_end_values (loop, values)
     struct loop *loop;
     rtx *values;
{
  unsigned regno;
  int defno;
  struct ref *def;
  basic_block def_bb;
  struct ref **found_def = xcalloc (max_regno, sizeof (struct ref *));
  sbitmap invalid = sbitmap_alloc (max_regno);
  sbitmap_zero (invalid);

  /* There must be exactly one definition of reg coming from inside of the
     loop that dominates the loop latch and belongs directly to the loop.  */

  EXECUTE_IF_SET_IN_BITMAP (DF_BB_INFO (df, loop->latch)->rd_out, 0, defno,
    {
      def = df->defs[defno];
      def_bb = DF_REF_BB (def);
      if (DF_REF_REG_DEF_P (def))
	{
	  regno = DF_REF_REGNO (def);
	  if (TEST_BIT (modified_regs[loop->num], regno)
	      && flow_bb_inside_loop_p (loop, def_bb))
	    {
	      if (def_bb->loop_father == loop
		  && fast_dominated_by_p (dom, loop->latch, def_bb))
		found_def[regno] = def;
	      else
		SET_BIT (invalid, regno);
	    }
	}
    });

  EXECUTE_IF_SET_IN_SBITMAP (modified_regs[loop->num], 0, regno,
    {
      if (!TEST_BIT (invalid, regno))
	{
	  if (found_def[regno])
	    values[regno] = found_def[regno]->aux;
	  else
	    values[regno] = gen_initial_value (regno);
  	}
      else
	values[regno] = NULL_RTX;
    });
  sbitmap_free (invalid);
  free (found_def);
}

/* Compute values of registers at entry to the LOOP, using the values
   already computed for its superloops.  */
static void
compute_initial_values (loop)
     struct loop *loop;
{
  rtx *values = initial_values[loop->num];
  unsigned defno, regno;
  struct ref **found_def = xcalloc (max_regno, sizeof (struct ref *));
  sbitmap invalid = sbitmap_alloc (max_regno);
  basic_block preheader = loop_preheader_edge (loop)->src;
  struct loop *outer = loop->outer;
  rtx outer_preheader_end;
  int outer_preheader_end_after;
  rtx *outer_values = initial_values[outer->num];
  struct ref *def;
  basic_block def_bb;

  if (outer && outer->outer)
    {
      outer_preheader_end = loop_preheader_edge (loop)->src->end;
      outer_preheader_end_after = true;
    }
  else
    {
      outer_preheader_end = ENTRY_BLOCK_PTR->succ->dest->head;
      outer_preheader_end_after = false;
    }
  sbitmap_zero (invalid);

  /* Check definitions reaching the end of the loop's preheader.  We are able
     to determine values of registers that are either only defined outside of
     the outer loop (using the initial values of the outer loop), or have
     exactly one definition in the outer loop that dominates the preheader.  */
  EXECUTE_IF_SET_IN_BITMAP (DF_BB_INFO (df, preheader)->rd_out, 0, defno,
    {
      def = df->defs[defno];
      def_bb = DF_REF_BB (def);
      if (DF_REF_REG_DEF_P (def))
	{
	  regno = DF_REF_REGNO (def);
	  if (flow_bb_inside_loop_p (outer, def_bb))
	    {
	      if (!fast_dominated_by_p (dom, preheader, def_bb))
		SET_BIT (invalid, regno);
	      else if (found_def[regno])
		abort ();
	      else
		found_def[regno] = def;
	    }
	}
    });

  for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
    {
      def = found_def[regno];
      if (!TEST_BIT (interesting_reg, regno))
	values[regno] = NULL_RTX;
      else if (TEST_BIT (invalid, regno))
	values[regno] = gen_value_at (regno, preheader->end, true);
      else if (def)
	{
	  if (DF_REF_BB (def)->loop_father != outer
	      && expr_mentions_iteration_p ((rtx) def->aux))
	    values[regno] = gen_value_at (regno, def->insn, true);
	  else
	    values[regno] = def->aux;
	}
      else if (outer_values[regno]
	       && expr_mentions_iteration_p (outer_values[regno]))
	values[regno] = gen_value_at (regno, outer_preheader_end,
				      outer_preheader_end_after);
      else
	values[regno] = outer_values[regno];
    }
  free (found_def);
  sbitmap_free (invalid);
}

/* The main entry point.  Run the analysis for all LOOPS starting from
   innermost ones.  */
void
analyse_induction_variables (loops)
     struct loops *loops;
{
  unsigned i, regno;
  rtx eq;
  struct loop *loop;
  basic_block bb;
  rtx insn, value;
  enum machine_mode mode, extended_mode;
  enum rtx_code extend;

  /* Compute register values in the first iteration.  */
  for (i = 0; i < loops->num; i++)
    if (loops->parray[i])
      {
	sbitmap_zero (modified_regs[i]);
	for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
	  loop_entry_values[i][regno] = gen_initial_value (regno);
      }
  compute_register_values (true);

  /* Now identify the induction variables.  */
  for (i = 1; i < loops->num; i++)
    {
      loop = loops->parray[i];
      if (!loop)
	continue;

      compute_loop_end_values (loop, loop_end_values[i]);
      EXECUTE_IF_SET_IN_SBITMAP (modified_regs[i], 0, regno,
	{
	  value = loop_end_values[i][regno];
	  if (!value)
	    eq = NULL_RTX;
	  else
	    {
	      mode = GET_MODE (value);
	      extend = GET_CODE (value);

	      /* We try to handle induction variables that in fact operate
		 in different mode than their register modes suggest.  I am
		 not really 100% sure that what we do here is correct (in
		 fact I am sure that it is not, just I don't know whether we
		 ever produce code that could cause us to behave incorrectly).

		 So what we do: if we for example see that from
		 INITIAL_VALUE:SI we got to SIGN_EXTEND:SI (something:HI),
		 we check whether something:HI - SUBREG:HI (INITIAL_VALUE:SI, 0)
		 is constant.  If it is, we assume this is such an induction
		 variable.  This is almost right, except for that in the
		 first iteration of the cycle, we did not have to have a value
		 in range of the narrower mode.  For now we ignore this
		 possibility; it should be easy to handle if it caused
		 problems -- we would just have to be a bit more cautious
		 about this first iteration.  */
		 
	      if (extend == SIGN_EXTEND || extend == ZERO_EXTEND)
		extended_mode = GET_MODE (XEXP (value, 0));
	      else
		extended_mode = mode;
	      eq = gen_rtx_fmt_ee (MINUS, mode,
				   copy_rtx (value),
				   gen_initial_value (regno));
	      if (extended_mode != mode)
		{
		  eq = gen_rtx_fmt_ei (SUBREG, extended_mode, eq, 0);
		  eq = gen_rtx_fmt_e (extend, mode, eq);
		}
	      eq = iv_simplify_rtx (eq);
	      if (!invariant_wrto_ivs_p (eq, loop_end_values[i]))
		eq = NULL_RTX;
	      else
		{
		  eq = gen_rtx_fmt_ee (MULT, mode, eq, gen_iteration (mode));
		  eq = gen_rtx_fmt_ee (PLUS, mode,
				       gen_initial_value (regno), eq);
		  if (extended_mode != mode)
		    {
		      eq = gen_rtx_fmt_ei (SUBREG, extended_mode, eq, 0);
		      eq = gen_rtx_fmt_e (extend, mode, eq);
		    }
		  eq = iv_simplify_rtx (eq);
		  if (!eq)
		    abort ();
		}
	    }
	  if (!eq)
	    eq = gen_value_at (regno, loop->header->head, false);
	  loop_entry_values[i][regno] = eq;
	});
    }

  /* Compute register values again, now including the ITERATION marks.  */
  compute_register_values (false);

  /* Compute the loop initial value information using the data computed above
     and simplify loop entry information using it.  */
  insn = ENTRY_BLOCK_PTR->succ->dest->head;
  for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
    initial_values[0][regno] = gen_value_at (regno, insn, false);
  loop = loops->tree_root->inner;
  while (loop)
    {
      compute_initial_values (loop);
      simplify_ivs_using_values (loop_entry_values[loop->num],
				 initial_values[loop->num]);

      if (loop->inner)
	loop = loop->inner;
      else
	{
	  while (loop->outer && !loop->next)
	    loop = loop->outer;
	  loop = loop->next;
	}
    }

  /* Simplify values stored at insns using this knowledge.  */
  simplify_register_values ();

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, ";; Induction variables:\n\n");

      for (i = 1; i < loops->num; i++)
	{
	  loop = loops->parray[i];
	  if(!loop)
	    continue;

	  fprintf (rtl_dump_file, ";; Loop %d:\n", i);
      	  fprintf (rtl_dump_file, ";;  initial values:\n");
	  dump_equations (rtl_dump_file, initial_values[loop->num]);
      	  fprintf (rtl_dump_file, ";;  induction variables at entry:\n");
	  dump_equations (rtl_dump_file, loop_entry_values[loop->num]);
	}
      fprintf (rtl_dump_file, "\n\n");

      FOR_EACH_BB (bb)
	{
	  fprintf (rtl_dump_file, ";; Basic block: %d (loop %d)\n\n",
		   bb->index, bb->loop_father->num);
	  for (insn = bb->head;
	       insn != NEXT_INSN (bb->end);
	       insn = NEXT_INSN (insn))
	    {
	      print_rtl_single (rtl_dump_file, insn);
	      if (INSN_P (insn))
		dump_insn_ivs (rtl_dump_file, insn);
	    }
	}
    }
}
