/* Rtl-level induction variable analysis.
   Copyright (C) 2004 Free Software Foundation, Inc.
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* This is just a very simplistic analysis of induction variables of the loop.
   The major use is for determining the number of iterations of a loop for
   loop unrolling, doloop optimization and branch prediction.  For this we
   are only interested in bivs and a fairly limited set of givs that are
   needed in the exit condition.  We also only compute the iv information on
   demand.

   The interesting registers are determined.  A register is interesting if

   -- it is set only in the blocks that dominate the latch of the current loop
   -- all its sets are simple -- i.e. in the form we understand

   We also number the insns sequentially in each basic block.  For a use of the
   interesting reg, it is now easy to find a reaching definition (there may be
   only one).

   Induction variable is then simply analysed by walking the use-def
   chains.
   
   Usage:

   iv_analysis_loop_init (loop);
   insn = iv_get_reaching_def (where, reg);
   if (iv_analyse (insn, reg, &iv))
     {
       ...
     }
   iv_analysis_done (); */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "expr.h"
#include "output.h"

/* The insn information.  */

struct insn_info
{
  /* Id of the insn.  */
  unsigned luid;

  /* The previous definition of the register defined by the single
     set in the insn.  */
  rtx prev_def;

  /* The description of the iv.  */
  struct rtx_iv iv;
};

static struct insn_info *insn_info;

/* The last definition of register.  */

static rtx *last_def;

/* The bivs.  */

static struct rtx_iv *bivs;

/* Maximal insn number for that there is place in insn_info array.  */

static unsigned max_insn_no;

/* Maximal register number for that there is place in bivs and last_def
   arrays.  */

static unsigned max_reg_no;

/* Dumps information about IV to FILE.  */

extern void dump_iv_info (FILE *, struct rtx_iv *);
void
dump_iv_info (FILE *file, struct rtx_iv *iv)
{
  if (!iv->base)
    {
      fprintf (file, "not simple");
      return;
    }

  if (iv->step == const0_rtx)
    {
      fprintf (file, "invariant ");
      print_rtl (file, iv->base);
      return;
    }

  print_rtl (file, iv->base);
  fprintf (file, " + ");
  print_rtl (file, iv->step);
  fprintf (file, " * iteration");
}

/* Assigns luids to insns in basic block BB.  */

static void
assign_luids (basic_block bb)
{
  unsigned i = 0, uid;
  rtx insn;

  FOR_BB_INSNS (bb, insn)
    {
      uid = INSN_UID (insn);
      insn_info[uid].luid = i++;
      insn_info[uid].prev_def = NULL_RTX;
      insn_info[uid].iv.analysed = false;
    }
}

/* Checks whether REG is a well-behaved register.  */

static bool
simple_reg_p (rtx reg)
{
  unsigned r;

  if (!REG_P (reg))
    return false;

  r = REGNO (reg);
  if (HARD_REGISTER_NUM_P (r))
    return false;

  if (GET_MODE_CLASS (GET_MODE (reg)) != MODE_INT)
    return false;

  if (last_def[r] == const0_rtx)
    return false;

  return true;
}

/* Checks whether assignment LHS = RHS is simple enough for us to process.  */

static bool
simple_set_p (rtx lhs, rtx rhs)
{
  rtx op0, op1;

  if (!simple_reg_p (lhs))
    return false;

  if (CONSTANT_P (rhs))
    return true;

  switch (GET_CODE (rhs))
    {
    case REG:
      return simple_reg_p (rhs);

    case NEG:
      return simple_reg_p (XEXP (rhs, 0));

    case PLUS:
    case MINUS:
    case MULT:
      op0 = XEXP (rhs, 0);
      op1 = XEXP (rhs, 1);

      if (!simple_reg_p (op0)
	  && !CONSTANT_P (op0))
	return false;

      if (!simple_reg_p (op1)
	  && !CONSTANT_P (op1))
	return false;

      if (GET_CODE (rhs) == MULT
	  && !CONSTANT_P (op0)
	  && !CONSTANT_P (op1))
	return false;

      return true;

    default:
      return false;
    }
}

/* Mark single SET in INSN.  */

static rtx
mark_single_set (rtx insn, rtx set)
{
  rtx def = SET_DEST (set), src;
  unsigned regno, uid;

  src = find_reg_equal_equiv_note (insn);
  if (!src)
    src = SET_SRC (set);

  if (!simple_set_p (SET_DEST (set), src))
    return NULL_RTX;

  regno = REGNO (def);
  uid = INSN_UID (insn);

  bivs[regno].analysed = false;
  insn_info[uid].prev_def = last_def[regno];
  last_def[regno] = insn;

  return def;
}

/* Marks set register REG as wrong, unless it is equal to EXCEPT.  */

static void
kill_sets (rtx reg, rtx by ATTRIBUTE_UNUSED, void *except)
{
  if (GET_CODE (reg) == SUBREG)
    reg = SUBREG_REG (reg);
  if (!REG_P (reg))
    return;
  if (reg == except)
    return;

  last_def[REGNO (reg)] = const0_rtx;
}

/* Marks sets in basic block BB.  If DOM is true, BB dominates the loop
   latch.  */

static void
mark_sets (basic_block bb, bool dom)
{
  rtx insn, set, def;

  FOR_BB_INSNS (bb, insn)
    {
      if (!INSN_P (insn))
	continue;

      if (dom
	  && (set = single_set (insn)))
	def = mark_single_set (insn, set);
      else
	def = NULL_RTX;

      note_stores (PATTERN (insn), kill_sets, def);
    }
}

/* Prepare the data for an induction variable analysis of a LOOP.  */

void
iv_analysis_loop_init (struct loop *loop)
{
  basic_block *body = get_loop_body_in_dom_order (loop);
  unsigned b;

  if ((unsigned) get_max_uid () >= max_insn_no)
    {
      /* Add some reserve for insns and registers produced in optimizations.  */
      max_insn_no = get_max_uid () + 100;
      if (insn_info)
	free (insn_info);
      insn_info = xmalloc (max_insn_no * sizeof (struct insn_info));
    }

  if ((unsigned) max_reg_num () >= max_reg_no)
    {
      max_reg_no = max_reg_num () + 100;
      if (last_def)
	free (last_def);
      last_def = xmalloc (max_reg_no * sizeof (rtx));
      if (bivs)
	free (bivs);
      bivs = xmalloc (max_reg_no * sizeof (struct rtx_iv));
    }

  memset (last_def, 0, max_reg_num () * sizeof (rtx));

  for (b = 0; b < loop->num_nodes; b++)
    {
      assign_luids (body[b]);
      mark_sets (body[b], just_once_each_iteration_p (loop, body[b]));
    }

  free (body);
}

/* Gets definition of REG reaching the INSN.  If REG is not simple, const0_rtx
   is returned.  If INSN is before the first def in the loop, NULL_RTX is
   returned.  */

rtx
iv_get_reaching_def (rtx insn, rtx reg)
{
  unsigned regno, luid, auid;
  rtx ainsn;
  basic_block bb, abb;

  if (!REG_P (reg))
    return NULL_RTX;

  regno = REGNO (reg);
  if (!last_def[regno]
      || last_def[regno] == const0_rtx)
    return last_def[regno];

  bb = BLOCK_FOR_INSN (insn);
  luid = insn_info[INSN_UID (insn)].luid;

  ainsn = last_def[regno];
  while (1)
    {
      abb = BLOCK_FOR_INSN (ainsn);

      if (dominated_by_p (CDI_DOMINATORS, bb, abb))
	break;

      auid = INSN_UID (ainsn);
      ainsn = insn_info[auid].prev_def;

      if (!ainsn)
	return NULL_RTX;
    }

  while (1)
    {
      abb = BLOCK_FOR_INSN (ainsn);
      if (abb != bb)
	return ainsn;

      auid = INSN_UID (ainsn);
      if (luid > insn_info[auid].luid)
	return ainsn;

      ainsn = insn_info[auid].prev_def;
      if (!ainsn)
	return NULL_RTX;
    }
}

/* Determines whether DEF is a biv and if so, stores its description
   to *IV.  */

static bool
iv_analyse_biv (rtx def, struct rtx_iv *iv)
{
  unsigned regno;
  rtx insn, set, rhs, adef, op0, op1, tmp;
  enum rtx_code code;

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Analysing ");
      print_rtl (rtl_dump_file, def);
      fprintf (rtl_dump_file, " for bivness.\n");
    }
    
  if (!REG_P (def))
    {
      if (!CONSTANT_P (def))
	return false;

      iv->analysed = true;
      iv->mode = VOIDmode;
      iv->base = def;
      iv->step = const0_rtx;
      return true;
    }

  regno = REGNO (def);
  if (last_def[regno] == const0_rtx)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "  not simple.\n");
      return false;
    }

  if (last_def[regno] && bivs[regno].analysed)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "  already analysed.\n");

      *iv = bivs[regno];
      return iv->base != NULL_RTX;
    }

  iv->analysed = true;
  iv->mode = GET_MODE (def);
  iv->base = def;
  iv->step = const0_rtx;

  if (!last_def[regno])
    goto end;

  insn = last_def[regno];
  adef = def;
  while (insn)
    {
      set = single_set (insn);
      rhs = find_reg_equal_equiv_note (insn);
      if (!rhs)
	rhs = SET_SRC (set);

      code = GET_CODE (rhs);
      switch (code)
	{
	case REG:
	  adef = rhs;
	  break;

	case PLUS:
	case MINUS:
	  op0 = XEXP (rhs, 0);
	  op1 = XEXP (rhs, 1);

	  if (code == PLUS && CONSTANT_P (op0))
	    {
	      tmp = op0; op0 = op1; op1 = tmp;
	    }

	  if (!simple_reg_p (op0)
	      || !CONSTANT_P (op1))
	    {
	      iv->base = NULL_RTX;
	      goto end;
	    }

	  iv->step = simplify_gen_binary (code, GET_MODE (rhs), iv->step, op1);
	  adef = op0;
	  break;

	default:
	  iv->base = NULL_RTX;
	  goto end;
	}

      insn = iv_get_reaching_def (insn, adef);
      if (insn == const0_rtx)
	{
	  iv->base = NULL_RTX;
	  goto end;
	}
    }

  if (adef != def)
    iv->base = NULL_RTX;

end:
  if (iv->base)

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "  ");
      dump_iv_info (rtl_dump_file, iv);
      fprintf (rtl_dump_file, "\n");
    }

  bivs[regno] = *iv;

  return iv->base != NULL_RTX;
}

/* Analyses operand OP of INSN and stores the result to *IV.  */

static bool
iv_analyse_op (rtx insn, rtx op, struct rtx_iv *iv)
{
  rtx def_insn;
  unsigned regno;
  bool inv = CONSTANT_P (op);

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Analysing operand ");
      print_rtl (rtl_dump_file, op);
      fprintf (rtl_dump_file, " of insn ");
      print_rtl_single (rtl_dump_file, insn);
    }

  if (!inv)
    {
      regno = REGNO (op);
      if (!last_def[regno])
	inv = true;
      else if (last_def[regno] == const0_rtx)
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "  not simple.\n");
	  return false;
	}
    }

  if (inv)
    {
      iv->mode = GET_MODE (op);
      iv->base = op;
      iv->step = const0_rtx;
	  
      if (rtl_dump_file)
	{
	  fprintf (rtl_dump_file, "  ");
	  dump_iv_info (rtl_dump_file, iv);
	  fprintf (rtl_dump_file, "\n");
	}
      return true;
    }

  def_insn = iv_get_reaching_def (insn, op);
  if (def_insn == const0_rtx)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "  not simple.\n");
      return false;
    }

  return iv_analyse (def_insn, op, iv);
}

/* Analyses iv DEF defined in INSN and stores the result to *IV.  */

bool
iv_analyse (rtx insn, rtx def, struct rtx_iv *iv)
{
  unsigned uid;
  rtx set, rhs;
  rtx op0 = NULL_RTX, op1 = NULL_RTX;
  struct rtx_iv iv0, iv1;
  enum machine_mode amode;
  enum rtx_code code;

  if (insn == const0_rtx)
    return false;

  if (!insn)
    return iv_analyse_biv (def, iv);

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Analysing def of ");
      print_rtl (rtl_dump_file, def);
      fprintf (rtl_dump_file, " in insn ");
      print_rtl_single (rtl_dump_file, insn);
    }

  uid = INSN_UID (insn);
  if (insn_info[uid].iv.analysed)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "  already analysed.\n");
      *iv = insn_info[uid].iv;
      return iv->base != NULL_RTX;
    }

  iv->mode = VOIDmode;
  iv->base = NULL_RTX;
  iv->step = NULL_RTX;

  set = single_set (insn);
  rhs = find_reg_equal_equiv_note (insn);
  if (!rhs)
    rhs = SET_SRC (set);
  code = GET_CODE (rhs);

  if (CONSTANT_P (rhs))
    {
      op0 = rhs;
      amode = GET_MODE (def);
    }
  else
    {
      switch (code)
	{
	case REG:
	  op0 = rhs;
	  break;

	case NEG:
	  op0 = XEXP (rhs, 0);
	  break;

	case PLUS:
	case MINUS:
	case MULT:
	  op0 = XEXP (rhs, 0);
	  op1 = XEXP (rhs, 1);
	  break;

	default:
	  abort ();
	}

      amode = GET_MODE (rhs);
    }

  if (op0)
    {
      if (!iv_analyse_op (insn, op0, &iv0))
	goto end;
	
      if (iv0.mode == VOIDmode)
	iv0.mode = amode;
    }

  if (op1)
    {
      if (!iv_analyse_op (insn, op1, &iv1))
	goto end;

      if (iv1.mode == VOIDmode)
	iv1.mode = amode;
    }

  if (CONSTANT_P (rhs)
      || code == REG)
    {
      *iv = iv0;
      goto end;
    }

  switch (code)
    {
    case NEG:
      iv->mode = iv0.mode;
      iv->base = simplify_gen_unary (NEG, amode, iv0.base, amode);
      iv->step = simplify_gen_unary (NEG, amode, iv0.step, amode);
      break;

    case PLUS:
    case MINUS:
      if (iv0.mode != iv1.mode)
	abort ();

      iv->mode = iv0.mode;
      iv->base = simplify_gen_binary (code, amode, iv0.base, iv1.base);
      iv->step = simplify_gen_binary (code, amode, iv0.step, iv1.step);
      break;

    case MULT:
      if (iv0.mode != iv1.mode)
	abort ();

      iv->mode = iv0.mode;

      if (iv0.step == const0_rtx)
	{
	  iv->base = simplify_gen_binary (MULT, amode, iv0.base, iv1.base);
	  iv->step = simplify_gen_binary (MULT, amode, iv0.base, iv1.step);
	}
      else if (iv1.step == const0_rtx)
	{
	  iv->base = simplify_gen_binary (MULT, amode, iv1.base, iv0.base);
	  iv->step = simplify_gen_binary (MULT, amode, iv1.base, iv0.step);
	}
      else
	abort ();
      break;

    default:
      abort ();
    }

end:
  iv->analysed = true;
  insn_info[uid].iv = *iv;

  if (rtl_dump_file)
    {
      print_rtl (rtl_dump_file, def);
      fprintf (rtl_dump_file, " in insn ");
      print_rtl_single (rtl_dump_file, insn);
      fprintf (rtl_dump_file, "  is ");
      dump_iv_info (rtl_dump_file, iv);
      fprintf (rtl_dump_file, "\n");
    }

  return iv->base != NULL_RTX;
}

/* Free the data for an induction variable analysis.  */

void
iv_analysis_done (void)
{
  max_insn_no = 0;
  max_reg_no = 0;
  if (insn_info)
    {
      free (insn_info);
      insn_info = NULL;
    }
  if (last_def)
    {
      free (last_def);
      last_def = NULL;
    }
  if (bivs)
    {
      free (bivs);
      bivs = NULL;
    }
}

/* Computes inverse to X modulo (1 << MOD).  */

static unsigned HOST_WIDEST_INT
inverse (unsigned HOST_WIDEST_INT x, int mod)
{
  unsigned HOST_WIDEST_INT mask =
	  ((unsigned HOST_WIDEST_INT) 1 << (mod - 1) << 1) - 1;
  unsigned HOST_WIDEST_INT rslt = 1;
  int i;

  for (i = 0; i < mod - 1; i++)
    {
      rslt = (rslt * x) & mask;
      x = (x * x) & mask;
    }

  return rslt;
}

/* Tries to estimate the maximum number of iterations.  */

static unsigned HOST_WIDEST_INT
determine_max_iter (struct niter_desc *desc)
{
  rtx niter = desc->niter_expr;
  rtx mmin, mmax, left, right;
  unsigned HOST_WIDEST_INT nmax, inc;

  if (GET_CODE (niter) == AND
      && GET_CODE (XEXP (niter, 0)) == CONST_INT)
    {
      nmax = INTVAL (XEXP (niter, 0));
      if (!(nmax & (nmax + 1)))
	{
	  desc->niter_max = nmax;
	  return nmax;
	}
    }

  get_mode_bounds (desc->mode, desc->signed_p, &mmin, &mmax);
  nmax = INTVAL (mmax) - INTVAL (mmin);

  if (GET_CODE (niter) == UDIV)
    {
      if (GET_CODE (XEXP (niter, 1)) != CONST_INT)
	{
	  desc->niter_max = nmax;
	  return nmax;
	}
      inc = INTVAL (XEXP (niter, 1));
      niter = XEXP (niter, 0);
    }
  else
    inc = 1;

  if (GET_CODE (niter) == PLUS)
    {
      left = XEXP (niter, 0);
      right = XEXP (niter, 0);

      if (GET_CODE (right) == CONST_INT)
	right = GEN_INT (-INTVAL (right));
    }
  else if (GET_CODE (niter) == MINUS)
    {
      left = XEXP (niter, 0);
      right = XEXP (niter, 0);
    }
  else
    {
      left = niter;
      right = mmin;
    }

  if (GET_CODE (left) == CONST_INT)
    mmax = left;
  if (GET_CODE (right) == CONST_INT)
    mmin = right;
  nmax = INTVAL (mmax) - INTVAL (mmin);

  desc->niter_max = nmax / inc;
  return nmax / inc;
}

/* Checks whether register *REG is in set ALT.  Callback for for_each_rtx.  */

static int
altered_reg_used (rtx *reg, void *alt)
{
  if (!REG_P (*reg))
    return 0;

  return REGNO_REG_SET_P (alt, REGNO (*reg));
}

/* Marks registers altered by EXPR in set ALT.  */

static void
mark_altered (rtx expr, rtx by ATTRIBUTE_UNUSED, void *alt)
{
  if (GET_CODE (expr) == SUBREG)
    expr = SUBREG_REG (expr);
  if (!REG_P (expr))
    return;

  SET_REGNO_REG_SET (alt, REGNO (expr));
}

/* Checks whether RHS is simple enough to process.  */

static bool
simple_rhs_p (rtx rhs)
{
  rtx op0, op1;

  if (CONSTANT_P (rhs)
      || REG_P (rhs))
    return true;

  switch (GET_CODE (rhs))
    {
    case PLUS:
    case MINUS:
      op0 = XEXP (rhs, 0);
      op1 = XEXP (rhs, 1);
      /* Allow reg + const sets only.  */
      if (REG_P (op0) && CONSTANT_P (op1))
	return true;
      if (REG_P (op1) && CONSTANT_P (op0))
	return true;

      return false;

    default:
      return false;
    }
}

/* Simplifies *EXPR using assignment in INSN.  ALTERED is the set of registers
   altered so far.  */

static void
simplify_using_assignment (rtx insn, rtx *expr, regset altered)
{
  rtx set = single_set (insn);
  rtx lhs, rhs;
  bool ret = false;

  if (set)
    {
      lhs = SET_DEST (set);
      if (GET_CODE (lhs) != REG
	  || altered_reg_used (&lhs, altered))
	ret = true;
    }
  else
    ret = true;

  note_stores (PATTERN (insn), mark_altered, altered);

  if (ret)
    return;

  rhs = find_reg_equal_equiv_note (insn);
  if (!rhs)
    rhs = SET_SRC (set);

  if (!simple_rhs_p (rhs))
    return;

  if (for_each_rtx (&rhs, altered_reg_used, altered))
    return;

  *expr = simplify_replace_rtx (*expr, lhs, rhs);
}

/* Checks whether A implies B.  */

static bool
implies_p (rtx a, rtx b)
{
  rtx op0, op1, r;

  if (GET_CODE (a) == EQ)
    {
      op0 = XEXP (a, 0);
      op1 = XEXP (a, 1);

      if (REG_P (op0))
	{
	  r = simplify_replace_rtx (b, op0, op1);
	  if (r == const_true_rtx)
	    return true;
	}

      if (REG_P (op1))
	{
	  r = simplify_replace_rtx (b, op1, op0);
	  if (r == const_true_rtx)
	    return true;
	}
    }

  return false;
}

/* Canonicalizes COND so that

   (1) If an operand is a constant, it will be the second operand.
   (2) (LE x const) will be replaced with (LT x <const+1>) and similarly
       for GE, GEU, and LEU.  */

rtx
canon_condition (rtx cond)
{
  rtx tem;
  rtx op0, op1;
  enum rtx_code code;

  code = GET_CODE (cond);
  op0 = XEXP (cond, 0);
  op1 = XEXP (cond, 1);

  /* If constant is first, put it last.  */
  if (CONSTANT_P (op0))
    {
      code = swap_condition (code);
      tem = op0;
      op0 = op1;
      op1 = tem;
    }

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

  if (op0 != XEXP (cond, 0)
      || op1 != XEXP (cond, 1)
      || code != GET_CODE (cond)
      || GET_MODE (cond) != SImode)
    cond = gen_rtx_fmt_ee (code, SImode, op0, op1);

  return cond;
}

/* Tries to use the fact that COND holds to simplify EXPR.  ALTERED is the
   set of altered regs.  */

void
simplify_using_condition (rtx cond, rtx *expr, regset altered)
{
  rtx rev, reve, exp = *expr;

  if (GET_RTX_CLASS (GET_CODE (*expr)) != '<')
    return;

  /* If some register gets altered later, we do not really speak about its
     value at the time of comparison.  */
  if (altered
      && for_each_rtx (&cond, altered_reg_used, altered))
    return;

  rev = reversed_condition (cond);
  reve = reversed_condition (exp);

  cond = canon_condition (cond);
  exp = canon_condition (exp);
  if (rev)
    rev = canon_condition (rev);
  if (reve)
    reve = canon_condition (reve);

  if (rtx_equal_p (exp, cond))
    {
      *expr = const_true_rtx;
      return;
    }


  if (rev && rtx_equal_p (exp, rev))
    {
      *expr = const0_rtx;
      return;
    }

  if (implies_p (cond, exp))
    {
      *expr = const_true_rtx;
      return;
    }
  
  if (reve && implies_p (cond, reve))
    {
      *expr = const0_rtx;
      return;
    }

  /* A proof by contradiction.  If *EXPR implies (not cond), *EXPR must
     be false.  */
  if (rev && implies_p (exp, rev))
    {
      *expr = const0_rtx;
      return;
    }

  /* Similarly, If (not *EXPR) implies (not cond), *EXPR must be true.  */
  if (rev && reve && implies_p (reve, rev))
    {
      *expr = const_true_rtx;
      return;
    }

  /* We would like to have some other tests here.  TODO.  */

  return;
}

/* Use relationship between A and *B to eventually eliminate *B.
   OP is the operation we consider.  */

static void
eliminate_implied_condition (enum rtx_code op, rtx a, rtx *b)
{
  if (op == AND)
    {
      /* If A implies *B, we may replace *B by true.  */
      if (implies_p (a, *b))
	*b = const_true_rtx;
    }
  else if (op == IOR)
    {
      /* If *B implies A, we may replace *B by false.  */
      if (implies_p (*b, a))
	*b = const0_rtx;
    }
  else
    abort ();
}

/* Eliminates the conditions in TAIL that are implied by HEAD.  OP is the
   operation we consider.  */

static void
eliminate_implied_conditions (enum rtx_code op, rtx *head, rtx tail)
{
  rtx elt;

  for (elt = tail; elt; elt = XEXP (elt, 1))
    eliminate_implied_condition (op, *head, &XEXP (elt, 0));
  for (elt = tail; elt; elt = XEXP (elt, 1))
    eliminate_implied_condition (op, XEXP (elt, 0), head);
}

/* Simplifies *EXPR using initial values at the start of the LOOP.  If *EXPR
   is a list, its elements are assumed to be combined using OP.  */

static void
simplify_using_initial_values (struct loop *loop, enum rtx_code op, rtx *expr)
{
  rtx head, tail, insn;
  rtx neutral, aggr;
  regset altered;
  regset_head altered_head;
  edge e;

  if (!*expr)
    return;

  if (CONSTANT_P (*expr))
    return;

  if (GET_CODE (*expr) == EXPR_LIST)
    {
      head = XEXP (*expr, 0);
      tail = XEXP (*expr, 1);

      eliminate_implied_conditions (op, &head, tail);

      if (op == AND)
	{
	  neutral = const_true_rtx;
	  aggr = const0_rtx;
	}
      else if (op == IOR)
	{
	  neutral = const0_rtx;
	  aggr = const_true_rtx;
	}
      else
	abort ();

      simplify_using_initial_values (loop, NIL, &head);
      if (head == aggr)
	{
	  XEXP (*expr, 0) = aggr;
	  XEXP (*expr, 1) = NULL_RTX;
	  return;
	}
      else if (head == neutral)
	{
	  *expr = tail;
	  simplify_using_initial_values (loop, op, expr);
	  return;
	}
      simplify_using_initial_values (loop, op, &tail);

      if (tail && XEXP (tail, 0) == aggr)
	{
	  *expr = tail;
	  return;
	}
  
      XEXP (*expr, 0) = head;
      XEXP (*expr, 1) = tail;
      return;
    }

  if (op != NIL)
    abort ();

  e = loop_preheader_edge (loop);
  if (e->src == ENTRY_BLOCK_PTR)
    return;

  altered = INITIALIZE_REG_SET (altered_head);

  while (1)
    {
      insn = BB_END (e->src);
      if (any_condjump_p (insn))
	{
	  /* FIXME -- slightly wrong -- what if compared register
	     gets altered between start of the condition and insn?  */
	  rtx cond = get_condition (BB_END (e->src), NULL, false);
      
	  if (cond && (e->flags & EDGE_FALLTHRU))
	    cond = reversed_condition (cond);
	  if (cond)
	    {
	      simplify_using_condition (cond, expr, altered);
	      if (CONSTANT_P (*expr))
		{
		  FREE_REG_SET (altered);
		  return;
		}
	    }
	}

      FOR_BB_INSNS_REVERSE (e->src, insn)
	{
	  if (!INSN_P (insn))
	    continue;
	    
	  simplify_using_assignment (insn, expr, altered);
	  if (CONSTANT_P (*expr))
	    {
	      FREE_REG_SET (altered);
	      return;
	    }
	}

      e = e->src->pred;
      if (e->pred_next
	  || e->src == ENTRY_BLOCK_PTR)
	break;
    }

  FREE_REG_SET (altered);
}

/* Computes number of iterations of the CONDITION in INSN in LOOP and stores
   the result into DESC.  Very similar to determine_number_of_iterations
   (basically its rtl version).  */

void
iv_number_of_iterations (struct loop *loop, rtx insn, rtx condition,
			 struct niter_desc *desc)
{
  rtx op0, op1, delta, step, bound, may_xform, def_insn, tmp;
  struct rtx_iv iv0, iv1, tmp_iv;
  rtx assumption;
  enum rtx_code cond;
  enum machine_mode mode;
  rtx mmin, mmax;
  unsigned HOST_WIDEST_INT s, size, d;
  HOST_WIDEST_INT up, down, inc;
  int was_sharp = false;

  /* The meaning of these assumptions is this:
     if !assumptions
       then the rest of information does not have to be valid
     if noloop_assumptions then the loop does not roll
     if infinite then this exit is never used
     */
  desc->assumptions = NULL_RTX;
  desc->noloop_assumptions = NULL_RTX;
  desc->infinite = NULL_RTX;
  desc->simple_p = true;

  desc->const_iter = false;
  desc->niter_expr = NULL_RTX;
  desc->niter_max = 0;

  cond = GET_CODE (condition);
  if (GET_RTX_CLASS (cond) != '<')
    abort ();

  mode = GET_MODE (XEXP (condition, 0));
  if (mode == VOIDmode)
    mode = GET_MODE (XEXP (condition, 1));
  /* The constant comparisons should be folded.  */
  if (mode == VOIDmode)
    abort ();

  /* We only handle integers or pointers.  */
  if (GET_MODE_CLASS (mode) != MODE_INT
      && GET_MODE_CLASS (mode) != MODE_PARTIAL_INT)
    goto fail;

  op0 = XEXP (condition, 0);
  def_insn = iv_get_reaching_def (insn, op0);
  if (!iv_analyse (def_insn, op0, &iv0))
    goto fail;
  
  op1 = XEXP (condition, 1);
  def_insn = iv_get_reaching_def (insn, op1);
  if (!iv_analyse (def_insn, op1, &iv1))
    goto fail;

  desc->mode = mode;

  /* Check condition and normalize it.  */

  switch (cond)
    {
      case GE:
      case GT:
      case GEU:
      case GTU:
	tmp_iv = iv0; iv0 = iv1; iv1 = tmp_iv;
	cond = swap_condition (cond);
	break;
      case NE:
      case LE:
      case LEU:
      case LT:
      case LTU:
	break;
      default:
	goto fail;
    }

  size = GET_MODE_BITSIZE (mode);
  get_mode_bounds (mode, (cond == LE || cond == LT), &mmin, &mmax);
  desc->signed_p = (cond == LE || cond == LT);

  if (GET_CODE (iv0.step) != CONST_INT || GET_CODE (iv1.step) != CONST_INT)
    goto fail;

  /* We can take care of the case of two induction variables chasing each other
     if the test is NE. I have never seen a loop using it, but still it is
     cool.  */
  if (iv0.step != const0_rtx && iv1.step != const0_rtx)
    {
      if (cond != NE)
	goto fail;

      iv0.step = simplify_gen_binary (MINUS, mode, iv0.step, iv1.step);
      iv1.step = const0_rtx;
    }

  /* This is either infinite loop or the one that ends immediately, depending
     on initial values.  Unswitching should remove this kind of conditions.  */
  if (iv0.step == const0_rtx && iv1.step == const0_rtx)
    goto fail;

  /* Ignore loops of while (i-- < 10) type.  */
  if (cond != NE
      && (INTVAL (iv0.step) < 0 || INTVAL (iv1.step) > 0))
    goto fail;

  /* Some more condition normalization.  We must record some assumptions
     due to overflows.  */
  switch (cond)
    {
      case LT:
      case LTU:
	/* We want to take care only of non-sharp relationals; this is easy,
	   as in cases the overflow would make the transformation unsafe
	   the loop does not roll.  Seemingly it would make more sense to want
	   to take care of sharp relationals instead, as NE is more simmilar to
	   them, but the problem is that here the transformation would be more
	   difficult due to possibly infinite loops.  */
	if (iv0.step == const0_rtx)
	  {
	    assumption = simplify_gen_relational (EQ, SImode, mode,
						  iv0.base, mmax);
	    if (assumption == const_true_rtx)
	      goto zero_iter;
	    iv0.base = simplify_gen_binary (PLUS, mode, iv0.base, const1_rtx);
	  }
	else
	  {
	    assumption = simplify_gen_relational (EQ, SImode, mode,
						  iv1.base, mmin);
	    if (assumption == const_true_rtx)
	      goto zero_iter;
	    iv1.base = simplify_gen_binary (PLUS, mode, iv1.base, constm1_rtx);
	  }
	if (assumption != const0_rtx)
	  desc->noloop_assumptions =
		  alloc_EXPR_LIST (0, assumption, desc->noloop_assumptions);
	cond = (cond == LT) ? LE : LEU;

	/* It will be useful to be able to tell the difference once more in
	   LE -> NE reduction.  */
	was_sharp = true;
	break;
      default: ;
    }

  /* Take care of trivially infinite loops.  */
  if (cond != NE)
    {
      if (iv0.step == const0_rtx)
	{
	  if (iv0.base == mmin)
	    {
	      desc->infinite =
		      alloc_EXPR_LIST (0, const_true_rtx, NULL_RTX);
	      goto fail;
	    }
	}
      else
	{
	  if (iv1.base == mmax)
	    {
	      desc->infinite =
		      alloc_EXPR_LIST (0, const_true_rtx, NULL_RTX);
	      goto fail;
	    }
	}
    }

  /* If we can we want to take care of NE conditions instead of size
     comparisons, as they are much more friendly (most importantly
     this takes care of special handling of loops with step 1).  We can
     do it if we first check that upper bound is greater or equal to
     lower bound, their difference is constant c modulo step and that
     there is not an overflow.  */
  if (cond != NE)
    {
      if (iv0.step == const0_rtx)
	step = simplify_gen_unary (NEG, mode, iv1.step, mode);
      else
	step = iv0.step;
      delta = simplify_gen_binary (MINUS, mode, copy_rtx (iv1.base), copy_rtx (iv0.base));
      delta = simplify_gen_binary (UMOD, mode, delta, step);
      may_xform = const0_rtx;

      if (GET_CODE (delta) == CONST_INT)
	{
	  if (was_sharp && INTVAL (delta) == INTVAL (step) - 1)
	    {
	      /* A special case.  We have transformed condition of type
		 for (i = 0; i < 4; i += 4)
		 into
		 for (i = 0; i <= 3; i += 4)
		 obviously if the test for overflow during that transformation
		 passed, we cannot overflow here.  Most importantly any
		 loop with sharp end condition and step 1 falls into this
		 cathegory, so handling this case specially is definitely
		 worth the troubles.  */
	      may_xform = const_true_rtx;
	    }
	  else if (iv0.step == const0_rtx)
	    {
	      bound = simplify_gen_binary (PLUS, mode, mmin, step);
	      bound = simplify_gen_binary (MINUS, mode, bound, delta);
	      may_xform = simplify_gen_relational (cond, SImode, mode,
						   bound, copy_rtx (iv0.base));
	    }
	  else
	    {
	      bound = simplify_gen_binary (MINUS, mode, mmax, step);
	      bound = simplify_gen_binary (PLUS, mode, bound, delta);
	      may_xform = simplify_gen_relational (cond, SImode, mode,
						   copy_rtx (iv1.base), bound);
	    }
	}

      if (may_xform != const0_rtx)
	{
	  /* We perform the transformation always provided that it is not
	     completely senseless.  This is OK, as we would need this assumption
	     to determine the number of iterations anyway.  */
	  if (may_xform != const_true_rtx)
	    desc->assumptions = alloc_EXPR_LIST (0, may_xform,
						 desc->assumptions);

	  /* We are going to lose some information about upper bound on
	     number of iterations in this step, so record the information
	     here.  */
	  inc = INTVAL (iv0.step) - INTVAL (iv1.step);
	  if (GET_CODE (iv1.base) == CONST_INT)
	    up = INTVAL (iv1.base);
	  else
	    up = INTVAL (mmax) - inc;
	  down = INTVAL (GET_CODE (iv0.base) == CONST_INT ? iv0.base : mmin);
	  desc->niter_max = (up - down) / inc + 1;

	  if (iv0.step == const0_rtx)
	    {
	      iv0.base = simplify_gen_binary (PLUS, mode, iv0.base, delta);
	      iv0.base = simplify_gen_binary (MINUS, mode, iv0.base, step);
	    }
	  else
	    {
	      iv1.base = simplify_gen_binary (MINUS, mode, iv1.base, delta);
	      iv1.base = simplify_gen_binary (PLUS, mode, iv1.base, step);
	    }

	  assumption = simplify_gen_relational (reverse_condition (cond),
						SImode, mode,
						copy_rtx (iv0.base),
						copy_rtx (iv1.base));
	  if (assumption == const_true_rtx)
	    goto zero_iter;
	  else if (assumption != const0_rtx)
	    desc->noloop_assumptions =
		    alloc_EXPR_LIST (0, assumption, desc->noloop_assumptions);
	  cond = NE;
	}
    }

  /* Count the number of iterations.  */
  if (cond == NE)
    {
      /* Everything we do here is just arithmetics modulo size of mode.  This
	 makes us able to do more involved computations of number of iterations
	 than in other cases.  First transform the condition into shape
	 s * i <> c, with s positive.  */
      iv1.base = simplify_gen_binary (MINUS, mode, iv1.base, iv0.base);
      iv0.base = const0_rtx;
      iv0.step = simplify_gen_binary (MINUS, mode, iv0.step, iv1.step);
      iv1.step = const0_rtx;
      if (INTVAL (iv0.step) < 0)
	{
	  iv0.step = simplify_gen_unary (NEG, mode, iv0.step, mode);
	  iv1.base = simplify_gen_unary (NEG, mode, iv1.base, mode);
	}

      /* Let nsd (s, size of mode) = d.  If d does not divide c, the loop
	 is infinite.  Otherwise, the number of iterations is
	 (inverse(s/d) * (c/d)) mod (size of mode/d).  */
      s = INTVAL (iv0.step); d = 1;
      while (s % 2 != 1)
	{
	  s /= 2;
	  d *= 2;
	  size--;
	}
      bound = GEN_INT (((unsigned HOST_WIDEST_INT) 1 << (size - 1 ) << 1) - 1);
      tmp = simplify_gen_binary (UMOD, mode, copy_rtx (iv1.base),
				 GEN_INT (d));
      desc->infinite = simplify_gen_relational (NE, SImode, mode,
						tmp, const0_rtx);
      tmp = simplify_gen_binary (UDIV, mode, iv1.base, GEN_INT (d));
      tmp = simplify_gen_binary (MULT, mode,
				 tmp, GEN_INT (inverse (s, size)));
      desc->niter_expr = simplify_gen_binary (AND, mode, tmp, bound);
    }
  else
    {
      if (iv1.step == const0_rtx)
	/* Condition in shape a + s * i <= b
	   We must know that b + s does not overflow and a <= b + s and then we
	   can compute number of iterations as (b + s - a) / s.  (It might
	   seem that we in fact could be more clever about testing the b + s
	   overflow condition using some information about b - a mod s,
	   but it was already taken into account during LE -> NE transform).  */
	{
	  bound = simplify_gen_binary (MINUS, mode, mmax, iv0.step);
	  assumption = simplify_gen_relational (cond, SImode, mode,
						copy_rtx (iv1.base), bound);
	  desc->assumptions =
		  alloc_EXPR_LIST (0, assumption, desc->assumptions);
	  step = iv0.step;
	  tmp = simplify_gen_binary (PLUS, mode, copy_rtx (iv1.base), iv0.step);
	  assumption = simplify_gen_relational (reverse_condition (cond),
						SImode,
						mode,
						copy_rtx (iv0.base),
						tmp);
	  delta = simplify_gen_binary (PLUS, mode, iv1.base, step);
	  delta = simplify_gen_binary (MINUS, mode, delta, iv0.base);
	}
      else
	{
	  /* Condition in shape a <= b - s * i
	     We must know that a - s does not overflow and a - s <= b and then
	     we can again compute number of iterations as (b - (a - s)) / s.  */
	  bound = simplify_gen_binary (MINUS, mode, mmin, iv1.step);
	  assumption = simplify_gen_relational (cond, SImode, mode,
						bound, copy_rtx (iv0.base));
	  desc->assumptions =
		  alloc_EXPR_LIST (0, assumption, desc->assumptions);
	  step = simplify_gen_unary (NEG, mode, iv1.step, mode);
	  tmp = simplify_gen_binary (PLUS, mode, copy_rtx (iv0.base), iv1.step);
	  assumption = simplify_gen_relational (reverse_condition (cond),
						SImode,
						mode,
						tmp,
						copy_rtx (iv1.base));
	  delta = simplify_gen_binary (MINUS, mode, iv0.base, step);
	  delta = simplify_gen_binary (MINUS, mode, iv1.base, delta);
	}
      if (assumption == const_true_rtx)
	goto zero_iter;
      else if (assumption != const0_rtx)
	desc->noloop_assumptions =
		alloc_EXPR_LIST (0, assumption, desc->noloop_assumptions);
      delta = simplify_gen_binary (UDIV, mode, delta, step);
      desc->niter_expr = delta;
    }

  simplify_using_initial_values (loop, AND, &desc->assumptions);
  if (desc->assumptions
      && XEXP (desc->assumptions, 0) == const0_rtx)
    goto fail;
  simplify_using_initial_values (loop, IOR, &desc->noloop_assumptions);
  simplify_using_initial_values (loop, NIL, &desc->niter_expr);

  /* Rerun the simplification.  Consider code (created by copying loop headers)

     i = 0;

     if (0 < n)
       {
         do
	   {
	     i++;
	   } while (i < n);
       }

    The first pass determines that i = 0, the second pass uses it to eliminate
    noloop assumption.  */

  simplify_using_initial_values (loop, AND, &desc->assumptions);
  if (desc->assumptions
      && XEXP (desc->assumptions, 0) == const0_rtx)
    goto fail;
  simplify_using_initial_values (loop, IOR, &desc->noloop_assumptions);
  simplify_using_initial_values (loop, NIL, &desc->niter_expr);

  if (GET_CODE (desc->niter_expr) == CONST_INT)
    {
      unsigned HOST_WIDEST_INT val = INTVAL (desc->niter_expr);

      desc->const_iter = true;
      desc->niter_max = desc->niter = val & GET_MODE_MASK (desc->mode);
    }
  else if (!desc->niter_max)
    desc->niter_max = determine_max_iter (desc);

  return;

fail:
  desc->simple_p = false;
  return;

zero_iter:
  desc->const_iter = true;
  desc->niter = 0;
  desc->niter_max = 0;
  desc->niter_expr = const0_rtx;
  return;
}

/* Checks whether E is a simple exit from LOOP and stores its description
   into DESC.  TODO Should replace cfgloopanal.c:simple_loop_exit_p.  */

static void
check_simple_exit (struct loop *loop, edge e, struct niter_desc *desc)
{
  basic_block exit_bb;
  rtx condition, at;
  edge ei;

  exit_bb = e->src;
  desc->simple_p = false;

  /* It must belong directly to the loop.  */
  if (exit_bb->loop_father != loop)
    return;

  /* It must be tested (at least) once during any iteration.  */
  if (!dominated_by_p (CDI_DOMINATORS, loop->latch, exit_bb))
    return;

  /* It must end in a simple conditional jump.  */
  if (!any_condjump_p (BB_END (exit_bb)))
    return;

  ei = exit_bb->succ;
  if (ei == e)
    ei = ei->succ_next;

  desc->out_edge = e;
  desc->in_edge = ei;

  /* Test whether the condition is suitable.  */
  if (!(condition = get_condition (BB_END (ei->src), &at, false)))
    return;

  if (ei->flags & EDGE_FALLTHRU)
    {
      condition = reversed_condition (condition);
      if (!condition)
	return;
    }

  /* Check that we are able to determine number of iterations and fill
     in information about it.  */
  iv_number_of_iterations (loop, at, condition, desc);
}

/* Finds a simple exit of LOOP and stores its description into DESC.
   TODO Should replace cfgloopanal.c:simple_loop_p.  */

void
find_simple_exit (struct loop *loop, struct niter_desc *desc)
{
  unsigned i;
  basic_block *body;
  edge e;
  struct niter_desc act;
  bool any = false;

  desc->simple_p = false;
  body = get_loop_body (loop);

  for (i = 0; i < loop->num_nodes; i++)
    {
      for (e = body[i]->succ; e; e = e->succ_next)
	{
	  if (flow_bb_inside_loop_p (loop, e->dest))
	    continue;
	  
	  check_simple_exit (loop, e, &act);
	  if (!act.simple_p)
	    continue;

	  /* Prefer constant iterations; the less the better.  */
	  if (!any)
	    any = true;
	  else if (!act.const_iter
		   || (desc->const_iter && act.niter >= desc->niter))
	    continue;
	  *desc = act;
	}
    }

#if 0
    {
      /* Check that we do not lose wrto older version.  */
      struct loop_desc odesc;
      if (simple_loop_p (loop, &odesc) && !odesc.strange)
	{
	  if (!desc->simple_p)
	    abort ();

	  if (odesc.const_iter)
	    {
	      if (!desc->const_iter)
		abort ();

	      if (desc->niter != odesc.niter)
		abort ();
	    }
	}
    }
#endif

  if (rtl_dump_file)
    {
      if (desc->simple_p)
	{
	  fprintf (rtl_dump_file, "Loop %d is simple:\n", loop->num);
	  fprintf (rtl_dump_file, "  simple exit %d -> %d\n",
		   desc->out_edge->src->index,
		   desc->out_edge->dest->index);
	  if (desc->assumptions)
	    {
	      fprintf (rtl_dump_file, "  assumptions: ");
	      print_rtl (rtl_dump_file, desc->assumptions);
	      fprintf (rtl_dump_file, "\n");
	    }
	  if (desc->noloop_assumptions)
	    {
	      fprintf (rtl_dump_file, "  does not roll if: ");
	      print_rtl (rtl_dump_file, desc->noloop_assumptions);
	      fprintf (rtl_dump_file, "\n");
	    }
	  if (desc->infinite != const0_rtx)
	    {
	      fprintf (rtl_dump_file, "  infinite if: ");
	      print_rtl (rtl_dump_file, desc->infinite);
	      fprintf (rtl_dump_file, "\n");
	    }

	  fprintf (rtl_dump_file, "  number of iterations: ");
	  print_rtl (rtl_dump_file, desc->niter_expr);
      	  fprintf (rtl_dump_file, "\n");

	  fprintf (rtl_dump_file, "  upper bound: ");
	  fprintf (rtl_dump_file, HOST_WIDEST_INT_PRINT_DEC, desc->niter_max);
      	  fprintf (rtl_dump_file, "\n");
	}
      else
	fprintf (rtl_dump_file, "Loop %d is not simple.\n", loop->num);
    }

  free (body);
}

/* Creates a simple loop description of LOOP if it was not computed
   already.  */

struct niter_desc *
get_simple_loop_desc (struct loop *loop)
{
  struct niter_desc *desc = simple_loop_desc (loop);

  if (desc)
    return desc;

  desc = xmalloc (sizeof (struct niter_desc));
  iv_analysis_loop_init (loop);
  find_simple_exit (loop, desc);
  loop->aux = desc;

  return desc;
}

/* Releases simple loop description for LOOP.  */

void
free_simple_loop_desc (struct loop *loop)
{
  struct niter_desc *desc = simple_loop_desc (loop);

  if (!desc)
    return;

  free (desc);
  loop->aux = NULL;
}
