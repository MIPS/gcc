/* Transformations based on profile information for values.
   Copyright (C) 1987, 1988, 1989, 1991, 1992, 1993, 1994, 1995, 1996, 1997,
   1998, 1999, 2000, 2001, 2002 Free Software Foundation, Inc.

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
#include "rtl.h"
#include "expr.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "profile.h"
#include "vpt.h"
#include "output.h"
#include "flags.h"
#include "insn-config.h"
#include "recog.h"
#include "optabs.h"

/* For speculative prefetching, the range in that we do not prefetch (because
   we assume that it will be in cache anyway).  */
#ifndef NOPREFETCH_RANGE_MIN
#define NOPREFETCH_RANGE_MIN (-16)
#endif
#ifndef NOPREFETCH_RANGE_MAX
#define NOPREFETCH_RANGE_MAX 32
#endif

static void insn_values_to_profile	PARAMS ((rtx, unsigned *,
					    	 struct histogram_value *));
static bool insn_divmod_values_to_profile PARAMS ((rtx, unsigned *,
						   struct histogram_value *));
#ifdef HAVE_prefetch
static bool insn_prefetch_values_to_profile PARAMS ((rtx, unsigned *,
						     struct histogram_value *));
static int find_mem_reference_1		PARAMS ((rtx *, void *));
static void find_mem_reference_2	PARAMS ((rtx, rtx, void *));
static bool find_mem_reference		PARAMS ((rtx, rtx *, int *));
#endif
static rtx gen_divmod_fixed_value	PARAMS ((enum machine_mode,
						 enum rtx_code,
						 rtx, rtx, rtx, gcov_type));
static rtx gen_mod_pow2			PARAMS ((enum machine_mode,
						 enum rtx_code,
						 rtx, rtx, rtx));
static rtx gen_mod_subtract		PARAMS ((enum machine_mode,
						 enum rtx_code,
						 rtx, rtx, rtx, int));
#ifdef HAVE_prefetch
static rtx gen_speculative_prefetch	PARAMS ((rtx, gcov_type, int));
#endif
static int divmod_fixed_value_transform	PARAMS ((rtx));
static int mod_pow2_value_transform	PARAMS ((rtx));
static int mod_subtract_transform	PARAMS ((rtx));
#ifdef HAVE_prefetch
static int speculative_prefetching_transform	PARAMS ((rtx));
#endif


/* Release the list of values for that we want to measure histograms.  */
void
free_profiled_values (n_values, values)
     unsigned n_values;
     struct histogram_value *values;
{
  unsigned i;

  for (i = 0; i < n_values; i++)
    {
      switch (values[i].type)
	{
	case HIST_TYPE_RANGE:
	  free (values[i].hdata.range.ranges);
	  break;

	default:
	  break;
	}
    }
  free (values);
}

/* Find values inside INSN for that we want to measure histograms for
   division/modulo optimization.  */
static bool
insn_divmod_values_to_profile (insn, n_values, values)
     rtx insn;
     unsigned *n_values;
     struct histogram_value *values;
{
  rtx set, set_src, op1, op2;
  enum machine_mode mode;

  if (!INSN_P (insn))
    return false;

  set = single_set_1 (insn);
  if (!set)
    return false;

  mode = GET_MODE (SET_DEST (set));
  if (!INTEGRAL_MODE_P (mode))
    return false;

  set_src = SET_SRC (set);
  switch (GET_CODE (set_src))
    {
    case DIV:
    case MOD:
    case UDIV:
    case UMOD:
      op1 = XEXP (set_src, 0);
      op2 = XEXP (set_src, 1);
      if (side_effects_p (op2))
	return false;

      /* Check for a special case where the divisor is power of 2.  */
      if ((GET_CODE (set_src) == UMOD) && !CONSTANT_P (op2))
	{
	  if (values)
	    {
	      values[*n_values].value = op2;
	      values[*n_values].seq = NULL_RTX;
	      values[*n_values].mode = mode;
	      values[*n_values].insn = insn;
	      values[*n_values].type = HIST_TYPE_POW2;
	      values[*n_values].hdata.pow2.may_be_other = 1;
	    }
	  (*n_values)++;
	}

      /* Check whether the divisor is not in fact a constant.  */
      if (!CONSTANT_P (op2))
	{
	  if (values)
	    {
	      values[*n_values].value = op2;
	      values[*n_values].mode = mode;
	      values[*n_values].seq = NULL_RTX;
	      values[*n_values].insn = insn;
	      values[*n_values].type = HIST_TYPE_ONE_VALUE;
	    }
	  (*n_values)++;
	}

      /* For mod, check whether it is not often a noop (or replacable by
	 a few subtractions).  */
      if (GET_CODE (set_src) == UMOD && !side_effects_p (op1))
	{
	  if (values)
	    {
	      rtx tmp;

	      start_sequence ();
	      tmp = simplify_gen_binary (DIV, mode, copy_rtx (op1), copy_rtx (op2));
	      values[*n_values].value = force_operand (tmp, NULL_RTX);
	      values[*n_values].seq = get_insns ();
	      end_sequence ();
	      values[*n_values].mode = mode;
	      values[*n_values].insn = insn;
	      values[*n_values].type = HIST_TYPE_INTERVAL;
	      values[*n_values].hdata.intvl.int_start = 0;
	      values[*n_values].hdata.intvl.steps = 2;
	      values[*n_values].hdata.intvl.may_be_less = 1;
	      values[*n_values].hdata.intvl.may_be_more = 1;
	    }
	  (*n_values)++;
	}
      return true;

    default:
      return false;
    }
}

#ifdef HAVE_prefetch
/* Called form find_mem_reference through for_each_rtx, finds a memory
   reference.  */
static int
find_mem_reference_1 (expr, ret)
     rtx *expr;
     void *ret;
{
  rtx *mem = ret;

  if (GET_CODE (*expr) == MEM)
    {
      *mem = *expr;
      return 1;
    }
  return 0;
}

/* Called form find_mem_reference through note_stores, finds out whether
   the memory reference is a store.  */
static int fmr2_write;
static void
find_mem_reference_2 (expr, pat, mem)
     rtx expr;
     rtx pat ATTRIBUTE_UNUSED;
     void *mem;
{
  if (expr == mem)
    fmr2_write = true;
}


/* Find a memory reference inside INSN, return it in MEM. Set WRITE to true
   if it is a write of the mem.  Return false if no mem is found, true
   otherwise.  */
static bool
find_mem_reference (insn, mem, write)
     rtx insn;
     rtx *mem;
     int *write;
{
  *mem = NULL_RTX;
  for_each_rtx (&PATTERN (insn), find_mem_reference_1, mem);

  if (!*mem)
    return false;
  
  fmr2_write = false;
  note_stores (PATTERN (insn), find_mem_reference_2, *mem);
  *write = fmr2_write;
  return true;
}

/* Find values inside INSN for that we want to measure histograms for
   a speculative prefetching.  */
static bool
insn_prefetch_values_to_profile (insn, n_values, values)
     rtx insn;
     unsigned *n_values;
     struct histogram_value *values;
{
  rtx mem, address;
  int write;

  if (!INSN_P (insn))
    return false;

  if (!find_mem_reference (insn, &mem, &write))
    return false;

  address = XEXP (mem, 0);
  if (side_effects_p (address))
    return false;
      
  if (!CONSTANT_P (address))
    {
      if (values)
	{
	  values[*n_values].value = address;
	  values[*n_values].mode = GET_MODE (address);
	  values[*n_values].seq = NULL_RTX;
	  values[*n_values].insn = insn;
	  values[*n_values].type = HIST_TYPE_CONST_DELTA;
	}
      (*n_values)++;
    }

  return true;
}
#endif

/* Find values inside INSN for that we want to measure histograms.  */
static void
insn_values_to_profile (insn, n_values, values)
     rtx insn;
     unsigned *n_values;
     struct histogram_value *values;
{
  if (flag_value_profile_transformations)
    insn_divmod_values_to_profile (insn, n_values, values);

#ifdef HAVE_prefetch
  if (flag_speculative_prefetching)
    insn_prefetch_values_to_profile (insn, n_values, values);
#endif
}

/* Find list of values for that we want to measure histograms.  */
void
find_values_to_profile (n_values, values)
     unsigned *n_values;
     struct histogram_value **values;
{
  rtx insn;
  unsigned i;

  *n_values = 0;
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    insn_values_to_profile (insn, n_values, NULL);
  *values = xmalloc (*n_values * sizeof (struct histogram_value));

  (*n_values) = 0;
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    insn_values_to_profile (insn, n_values, *values);

  for (i = 0; i < *n_values; i++)
    {
      switch ((*values)[i].type)
	{
	case HIST_TYPE_INTERVAL:
	  (*values)[i].n_counters = (*values)[i].hdata.intvl.steps +
		  ((*values)[i].hdata.intvl.may_be_less ? 1 : 0) +
		  ((*values)[i].hdata.intvl.may_be_more ? 1 : 0);
	  break;

	case HIST_TYPE_RANGE:
	  (*values)[i].n_counters = (*values)[i].hdata.range.n_ranges + 1;
	  break;

	case HIST_TYPE_POW2:
	  (*values)[i].n_counters = GET_MODE_BITSIZE ((*values)[i].mode) +
		  ((*values)[i].hdata.pow2.may_be_other ? 1 : 0);
	  break;

	case HIST_TYPE_ONE_VALUE:
	  (*values)[i].n_counters = 3;
	  break;

	case HIST_TYPE_CONST_DELTA:
	  (*values)[i].n_counters = 4;
	  break;

	default:
	  abort ();
	}
    }
}

/* Main entry point.  Finds REG_VALUE_HISTOGRAM notes from profiler and uses
   them to identify and exploit properties of values that are hard to analyze
   statically.

   We do following transformations:

   1)

   x = a / b;

   where b is almost always a constant N is transformed to

   if (b == N)
     x = a / N;
   else
     x = a / b;

   Analogically with %

   2)

   x = a % b

   where b is almost always a power of 2 and the division is unsigned
   TODO -- handle signed case as well

   if ((b & (b - 1)) == 0)
     x = a & (b - 1);
   else
     x = x % b;

   Note that when b = 0, no error will occur and x = a; this is correct,
   as result of such operation is undefined.

   3)

   x = a % b

   where a is almost always less then b and the division is unsigned
   TODO -- handle signed case as well

   x = a;
   if (x >= b)
     x %= b;

   4)

   x = a % b

   where a is almost always less then 2 * b and the division is unsigned
   TODO -- handle signed case as well

   x = a;
   if (x >= b)
     x -= b;
   if (x >= b)
     x %= b;

   It would be possible to continue analogically for K * b for other small
   K's, but I am not sure whether it is worth that.

   5)

   Read or write of mem[address], where the value of address changes usually
   by a constant C > 0 between the following accesses to the computation; with
   -fspeculative-prefetching we then add a prefetch of address + C before
   the insn.  This handles prefetching of several interesting cases in addition
   to a simple prefetching for addresses that are induction variables, e. g.
   linked lists allocated sequentially (even in case they are processed
   recursively).

   TODO -- we should also check whether there is not (usually) a small
	   difference with the adjacent memory references, so that we do
	   not issue overlapping prefetches.  Also we should employ some
	   heuristics to eliminate cases where prefetching evidently spoils
	   the code.
	-- it should somehow cooperate with the loop optimizer prefetching,
	   so that the mem is not prefetched twice

   TODO:

   There are other useful cases that could be handled by a similar mechanism,
   for example:
   
   for (i = 0; i < n; i++)
     ...
   
   transform to (for constant N):
   
   if (n == N)
     for (i = 0; i < N; i++)
       ...
   else
     for (i = 0; i < n; i++)
       ...
   making unroller happy.

   */

int
value_profile_transformations ()
{
  rtx insn, next;
  int changed = false;

  for (insn = get_insns (); insn; insn = next)
    {
      next = NEXT_INSN (insn);

      /* Analyze the insn.  */
      if (!INSN_P (insn))
	continue;

      /* Scan for insn carrying a histogram.  */
      if (!find_reg_note (insn, REG_VALUE_HISTOGRAM, 0))
	continue;

      /* Ignore cold areas -- we are growing a code.  */
      if (!maybe_hot_bb_p (BLOCK_FOR_INSN (insn)))
	continue;

      if (rtl_dump_file)
	{
	  fprintf (rtl_dump_file, "Trying transformations on insn %d\n",
		   INSN_UID (insn));
	  print_rtl_single (rtl_dump_file, insn);
	}

      /* Transformations:  */
      if (flag_value_profile_transformations
	  && (mod_subtract_transform (insn)
	      || divmod_fixed_value_transform (insn)
	      || mod_pow2_value_transform (insn)))
	changed = true;
#ifdef HAVE_prefetch
      if (flag_speculative_prefetching
	  && speculative_prefetching_transform (insn))
	changed = true;
#endif
    }

  if (changed)
    commit_edge_insertions ();

  return changed;
}

/* Generate code for transformation 1 (with MODE and OPERATION, operands OP1
   and OP2 whose value is expected to be VALUE and result TARGET).  */
static rtx
gen_divmod_fixed_value (mode, operation, target, op1, op2, value)
     enum machine_mode mode;
     enum rtx_code operation;
     rtx target;
     rtx op1;
     rtx op2;
     gcov_type value;
{
  rtx tmp, tmp1;
  rtx neq_label = gen_label_rtx ();
  rtx end_label = gen_label_rtx ();
  rtx sequence;

  start_sequence ();
  
  if (!REG_P (op2))
    {
      tmp = gen_reg_rtx (mode);
      emit_move_insn (tmp, copy_rtx (op2));
    }
  else
    tmp = op2;

  do_compare_rtx_and_jump (tmp, GEN_INT (value), NE, 0, mode, NULL_RTX,
			   NULL_RTX, neq_label);
  tmp1 = simplify_gen_binary (operation, mode, copy_rtx (op1), GEN_INT (value));
  tmp1 = force_operand (tmp1, target);
  if (tmp1 != target)
    emit_move_insn (copy_rtx (target), copy_rtx (tmp1));

  emit_jump_insn (gen_jump (end_label));
  emit_barrier ();

  emit_label (neq_label);
  tmp1 = simplify_gen_binary (operation, mode, copy_rtx (op1), copy_rtx (tmp));
  tmp1 = force_operand (tmp1, target);
  if (tmp1 != target)
    emit_move_insn (copy_rtx (target), copy_rtx (tmp1));
  
  emit_label (end_label);

  sequence = get_insns ();
  end_sequence ();
  rebuild_jump_labels (sequence);
  return sequence;
}

/* Do transform 1) on INSN if applicable.  */
static int
divmod_fixed_value_transform (insn)
     rtx insn;
{
  rtx set, set_src, set_dest, op1, op2, value, histogram;
  enum rtx_code code;
  enum machine_mode mode;
  gcov_type val, count, all;
  edge e;

  set = single_set_1 (insn);
  if (!set)
    return 0;

  set_src = SET_SRC (set);
  set_dest = SET_DEST (set);
  code = GET_CODE (set_src);
  mode = GET_MODE (set_dest);
  
  if (code != DIV && code != MOD && code != UDIV && code != UMOD)
    return 0;
  op1 = XEXP (set_src, 0);
  op2 = XEXP (set_src, 1);

  for (histogram = REG_NOTES (insn);
       histogram;
       histogram = XEXP (histogram, 1))
    if (REG_NOTE_KIND (histogram) == REG_VALUE_HISTOGRAM
	&& XEXP (XEXP (histogram, 0), 0) == GEN_INT (HIST_TYPE_ONE_VALUE))
      break;

  if (!histogram)
    return 0;

  histogram = XEXP (XEXP (histogram, 0), 1);
  value = XEXP (histogram, 0);
  histogram = XEXP (histogram, 1);
  val = INTVAL (XEXP (histogram, 0));
  histogram = XEXP (histogram, 1);
  count = INTVAL (XEXP (histogram, 0));
  histogram = XEXP (histogram, 1);
  all = INTVAL (XEXP (histogram, 0));

  /* We requiere that count is at least half of all; this means
     that for the transformation to fire the value must be constant
     at least 50% of time (and 75% gives the garantee of usage).  */
  if (!rtx_equal_p (op2, value) || 2 * count < all)
    return 0;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Div/mod by constant transformation on insn %d\n",
	     INSN_UID (insn));

  e = split_block (BLOCK_FOR_INSN (insn), PREV_INSN (insn));
  delete_insn (insn);
  
  insert_insn_on_edge (
	gen_divmod_fixed_value (mode, code, set_dest, op1, op2, val), e);

  return 1;
}

/* Generate code for transformation 2 (with MODE and OPERATION, operands OP1
   and OP2 and result TARGET).  */
static rtx
gen_mod_pow2 (mode, operation, target, op1, op2)
     enum machine_mode mode;
     enum rtx_code operation;
     rtx target;
     rtx op1;
     rtx op2;
{
  rtx tmp, tmp1, tmp2, tmp3;
  rtx neq_label = gen_label_rtx ();
  rtx end_label = gen_label_rtx ();
  rtx sequence;

  start_sequence ();
  
  if (!REG_P (op2))
    {
      tmp = gen_reg_rtx (mode);
      emit_move_insn (tmp, copy_rtx (op2));
    }
  else
    tmp = op2;

  tmp1 = expand_simple_binop (mode, PLUS, tmp, constm1_rtx, NULL_RTX,
			      0, OPTAB_WIDEN);
  tmp2 = expand_simple_binop (mode, AND, tmp, tmp1, NULL_RTX,
			      0, OPTAB_WIDEN);
  do_compare_rtx_and_jump (tmp2, const0_rtx, NE, 0, mode, NULL_RTX,
			   NULL_RTX, neq_label);
  tmp3 = expand_simple_binop (mode, AND, op1, tmp1, target,
			      0, OPTAB_WIDEN);
  if (tmp3 != target)
    emit_move_insn (copy_rtx (target), tmp3);
  emit_jump_insn (gen_jump (end_label));
  emit_barrier ();

  emit_label (neq_label);
  tmp1 = simplify_gen_binary (operation, mode, copy_rtx (op1), copy_rtx (tmp));
  tmp1 = force_operand (tmp1, target);
  if (tmp1 != target)
    emit_move_insn (target, tmp1);
  
  emit_label (end_label);

  sequence = get_insns ();
  end_sequence ();
  rebuild_jump_labels (sequence);
  return sequence;
}

/* Do transform 2) on INSN if applicable.  */
static int
mod_pow2_value_transform (insn)
     rtx insn;
{
  rtx set, set_src, set_dest, op1, op2, value, histogram;
  enum rtx_code code;
  enum machine_mode mode;
  gcov_type wrong_values, count;
  edge e;
  int i;

  set = single_set_1 (insn);
  if (!set)
    return 0;

  set_src = SET_SRC (set);
  set_dest = SET_DEST (set);
  code = GET_CODE (set_src);
  mode = GET_MODE (set_dest);
  
  if (code != UMOD)
    return 0;
  op1 = XEXP (set_src, 0);
  op2 = XEXP (set_src, 1);

  for (histogram = REG_NOTES (insn);
       histogram;
       histogram = XEXP (histogram, 1))
    if (REG_NOTE_KIND (histogram) == REG_VALUE_HISTOGRAM
	&& XEXP (XEXP (histogram, 0), 0) == GEN_INT (HIST_TYPE_POW2))
      break;

  if (!histogram)
    return 0;

  histogram = XEXP (XEXP (histogram, 0), 1);
  value = XEXP (histogram, 0);
  histogram = XEXP (histogram, 1);
  wrong_values =INTVAL (XEXP (histogram, 0));
  histogram = XEXP (histogram, 1);

  count = 0;
  for (i = 0; i < GET_MODE_BITSIZE (mode); i++)
    {
      count += INTVAL (XEXP (histogram, 0));
      histogram = XEXP (histogram, 1);
    }

  if (!rtx_equal_p (op2, value))
    return 0;

  /* We require that we hit a power of two at least half of all evaluations.  */
  if (count < wrong_values)
    return 0;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Mod power of 2 transformation on insn %d\n",
	     INSN_UID (insn));

  e = split_block (BLOCK_FOR_INSN (insn), PREV_INSN (insn));
  delete_insn (insn);
  
  insert_insn_on_edge (
	gen_mod_pow2 (mode, code, set_dest, op1, op2), e);

  return 1;
}

/* Generate code for transformations 3 and 4 (with MODE and OPERATION,
   operands OP1 and OP2, result TARGET and at most SUB subtractions).  */
static rtx
gen_mod_subtract (mode, operation, target, op1, op2, sub)
     enum machine_mode mode;
     enum rtx_code operation;
     rtx target;
     rtx op1;
     rtx op2;
     int sub;
{
  rtx tmp, tmp1;
  rtx end_label = gen_label_rtx ();
  rtx sequence;
  int i;

  start_sequence ();
  
  if (!REG_P (op2))
    {
      tmp = gen_reg_rtx (mode);
      emit_move_insn (tmp, copy_rtx (op2));
    }
  else
    tmp = op2;

  emit_move_insn (target, copy_rtx (op1));
  do_compare_rtx_and_jump (target, tmp, LTU, 0, mode, NULL_RTX,
			   NULL_RTX, end_label);
  

  for (i = 0; i < sub; i++)
    {
      tmp1 = expand_simple_binop (mode, MINUS, target, tmp, target,
	    			  0, OPTAB_WIDEN);
      if (tmp1 != target)
	emit_move_insn (target, tmp1);
      do_compare_rtx_and_jump (target, tmp, LTU, 0, mode, NULL_RTX,
    			       NULL_RTX, end_label);
    }

  tmp1 = simplify_gen_binary (operation, mode, copy_rtx (target), copy_rtx (tmp));
  tmp1 = force_operand (tmp1, target);
  if (tmp1 != target)
    emit_move_insn (target, tmp1);
  
  emit_label (end_label);

  sequence = get_insns ();
  end_sequence ();
  rebuild_jump_labels (sequence);
  return sequence;
}

/* Do transforms 3) and 4) on INSN if applicable.  */
static int
mod_subtract_transform (insn)
     rtx insn;
{
  rtx set, set_src, set_dest, op1, op2, value, histogram;
  enum rtx_code code;
  enum machine_mode mode;
  gcov_type wrong_values, counts[2], count, all;
  edge e;
  int i;

  set = single_set_1 (insn);
  if (!set)
    return 0;

  set_src = SET_SRC (set);
  set_dest = SET_DEST (set);
  code = GET_CODE (set_src);
  mode = GET_MODE (set_dest);
  
  if (code != UMOD)
    return 0;
  op1 = XEXP (set_src, 0);
  op2 = XEXP (set_src, 1);

  for (histogram = REG_NOTES (insn);
       histogram;
       histogram = XEXP (histogram, 1))
    if (REG_NOTE_KIND (histogram) == REG_VALUE_HISTOGRAM
	&& XEXP (XEXP (histogram, 0), 0) == GEN_INT (HIST_TYPE_INTERVAL))
      break;

  if (!histogram)
    return 0;

  histogram = XEXP (XEXP (histogram, 0), 1);
  value = XEXP (histogram, 0);
  histogram = XEXP (histogram, 1);

  all = 0;
  for (i = 0; i < 2; i++)
    {
      counts[i] = INTVAL (XEXP (histogram, 0));
      all += counts[i];
      histogram = XEXP (histogram, 1);
    }
  wrong_values = INTVAL (XEXP (histogram, 0));
  histogram = XEXP (histogram, 1);
  wrong_values += INTVAL (XEXP (histogram, 0));
  all += wrong_values;

  /* We require that we use just subtractions in at least 50% of all evaluations.  */
  count = 0;
  for (i = 0; i < 2; i++)
    {
      count += counts[i];
      if (count * 2 >= all)
	break;
    }
  
  if (i == 2)
    return 0;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Mod subtract transformation on insn %d\n",
	     INSN_UID (insn));

  e = split_block (BLOCK_FOR_INSN (insn), PREV_INSN (insn));
  delete_insn (insn);
  
  insert_insn_on_edge (
	gen_mod_subtract (mode, code, set_dest, op1, op2, i), e);

  return 1;
}

#ifdef HAVE_prefetch
/* Generate code for transformation 5 for mem with ADDRESS and a constant
   step DELTA.  WRITE is true if the reference is a store to mem.  */
static rtx
gen_speculative_prefetch (address, delta, write)
     rtx address;
     gcov_type delta;
     int write;
{
  rtx tmp;
  rtx sequence;

  start_sequence ();
  if (offsettable_address_p (0, VOIDmode, address))
    tmp = plus_constant (copy_rtx (address), delta);
  else
    {
      tmp = simplify_gen_binary (PLUS, Pmode,
				 copy_rtx (address), GEN_INT (delta));
      tmp = force_operand (tmp, NULL);
    }
  if (! (*insn_data[(int)CODE_FOR_prefetch].operand[0].predicate)
      (tmp, insn_data[(int)CODE_FOR_prefetch].operand[0].mode))
    tmp = force_reg (Pmode, tmp);
  emit_insn (gen_prefetch (tmp, GEN_INT (write), GEN_INT (3)));
  sequence = get_insns ();
  end_sequence ();

  return sequence;
}

/* Do transform 5) on INSN if applicable.  */
static int
speculative_prefetching_transform (insn)
     rtx insn;
{
  rtx histogram, value;
  gcov_type val, count, all;
  edge e;
  rtx mem, address;
  int write;

  if (!find_mem_reference (insn, &mem, &write))
    return false;

  address = XEXP (mem, 0);
  if (side_effects_p (address))
    return false;
      
  if (CONSTANT_P (address))
    return false;

  for (histogram = REG_NOTES (insn);
       histogram;
       histogram = XEXP (histogram, 1))
    if (REG_NOTE_KIND (histogram) == REG_VALUE_HISTOGRAM
	&& XEXP (XEXP (histogram, 0), 0) == GEN_INT (HIST_TYPE_CONST_DELTA))
      break;

  if (!histogram)
    return 0;

  histogram = XEXP (XEXP (histogram, 0), 1);
  value = XEXP (histogram, 0);
  histogram = XEXP (histogram, 1);
  /* Skip last value referenced.  */
  histogram = XEXP (histogram, 1);
  val = INTVAL (XEXP (histogram, 0));
  histogram = XEXP (histogram, 1);
  count = INTVAL (XEXP (histogram, 0));
  histogram = XEXP (histogram, 1);
  all = INTVAL (XEXP (histogram, 0));

  /* We require that count is at least half of all; this means
     that for the transformation to fire the value must be constant
     at least 50% of time (and 75% gives the garantee of usage).  */
  if (!rtx_equal_p (address, value) || 2 * count < all)
    return 0;

  /* If the difference is too small, it does not make too much sense to
     prefetch, as the memory is probably already in cache.  */
  if (val >= NOPREFETCH_RANGE_MIN && val <= NOPREFETCH_RANGE_MAX)
    return 0;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Speculative prefetching for insn %d\n",
	     INSN_UID (insn));

  e = split_block (BLOCK_FOR_INSN (insn), PREV_INSN (insn));
  
  insert_insn_on_edge (
	gen_speculative_prefetch (address, val, write), e);

  return 1;
}
#endif  /* HAVE_prefetch */
