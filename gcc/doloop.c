/* Perform doloop optimizations
   Copyright (C) 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
   Contributed by Michael P. Hayes (m.hayes@elec.canterbury.ac.nz)

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
#include "flags.h"
#include "expr.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "toplev.h"
#include "tm_p.h"
#include "cfgloop.h"
#include "output.h"
#include "params.h"


/* This module is used to modify loops with a determinable number of
   iterations to use special low-overhead looping instructions.

   It first validates whether the loop is well behaved and has a
   determinable number of iterations (either at compile or run-time).
   It then modifies the loop to use a low-overhead looping pattern as
   follows:

   1. A pseudo register is allocated as the loop iteration counter.

   2. The number of loop iterations is calculated and is stored
      in the loop counter.

   3. At the end of the loop, the jump insn is replaced by the
      doloop_end pattern.  The compare must remain because it might be
      used elsewhere.  If the loop-variable or condition register are
      used elsewhere, they will be eliminated by flow.

   4. An optional doloop_begin pattern is inserted at the top of the
      loop.
*/


#ifdef HAVE_doloop_end

static bool doloop_optimize
  PARAMS ((struct loops *loops, struct loop *loop));
static rtx doloop_condition_get
  PARAMS ((rtx));
static unsigned HOST_WIDE_INT doloop_iterations_max
  PARAMS ((const struct loop *));
static bool doloop_valid_p
  PARAMS ((struct loops *, struct loop *));
static bool doloop_modify
  PARAMS ((const struct loop *, rtx, rtx, rtx, rtx));
static bool doloop_modify_runtime
  PARAMS ((struct loop *, rtx, rtx, rtx));
static int get_loop_level
  PARAMS ((const struct loop *));

/* Returns the maximum level of nesting of subloops of LOOP.  */
static int
get_loop_level (loop)
     const struct loop *loop;
{
  const struct loop *ploop;
  int mx = -1, l;

  for (ploop = loop->inner; ploop; ploop = ploop->next)
    {
      l = get_loop_level (ploop);
      if (l > mx)
	mx = l;
    }
  return mx + 1;
}

/* Return the loop termination condition for PATTERN or zero
   if it is not a decrement and branch jump insn.  */
static rtx
doloop_condition_get (pattern)
     rtx pattern;
{
  rtx cmp;
  rtx inc;
  rtx reg;
  rtx condition;

  /* The canonical doloop pattern we expect is:

     (parallel [(set (pc) (if_then_else (condition)
                                        (label_ref (label))
                                        (pc)))
                (set (reg) (plus (reg) (const_int -1)))
                (additional clobbers and uses)])

     Some machines (IA-64) make the decrement conditional on
     the condition as well, so we don't bother verifying the
     actual decrement.  In summary, the branch must be the
     first entry of the parallel (also required by jump.c),
     and the second entry of the parallel must be a set of
     the loop counter register.  */

  if (GET_CODE (pattern) != PARALLEL)
    return 0;

  cmp = XVECEXP (pattern, 0, 0);
  inc = XVECEXP (pattern, 0, 1);

  /* Check for (set (reg) (something)).  */
  if (GET_CODE (inc) != SET || ! REG_P (SET_DEST (inc)))
    return 0;

  /* Extract loop counter register.  */
  reg = SET_DEST (inc);

  /* Check for (set (pc) (if_then_else (condition)
                                       (label_ref (label))
                                       (pc))).  */
  if (GET_CODE (cmp) != SET
      || SET_DEST (cmp) != pc_rtx
      || GET_CODE (SET_SRC (cmp)) != IF_THEN_ELSE
      || GET_CODE (XEXP (SET_SRC (cmp), 1)) != LABEL_REF
      || XEXP (SET_SRC (cmp), 2) != pc_rtx)
    return 0;

  /* Extract loop termination condition.  */
  condition = XEXP (SET_SRC (cmp), 0);

  if ((GET_CODE (condition) != GE && GET_CODE (condition) != NE)
      || GET_CODE (XEXP (condition, 1)) != CONST_INT)
    return 0;

  if (XEXP (condition, 0) == reg)
    return condition;

  if (GET_CODE (XEXP (condition, 0)) == PLUS
      && XEXP (XEXP (condition, 0), 0) == reg)
    return condition;

  /* ??? If a machine uses a funny comparison, we could return a
     canonicalised form here.  */

  return 0;
}


/* Return an estimate of the maximum number of loop iterations for the
   loop specified by LOOP.  */
static unsigned HOST_WIDE_INT
doloop_iterations_max (loop)
     const struct loop *loop;
{
  unsigned HOST_WIDE_INT n_iterations_max;
  enum rtx_code code;
  rtx min_value;
  rtx max_value;
  HOST_WIDE_INT abs_inc;
  int neg_inc;
  enum machine_mode mode = GET_MODE (loop->desc.var);

  neg_inc = 0;
  abs_inc = INTVAL (loop->desc.stride);
  if (abs_inc < 0)
    {
      abs_inc = -abs_inc;
      neg_inc = 1;
    }

  if (neg_inc)
    {
      code = swap_condition (loop->desc.cond);
      min_value = XEXP (loop->desc.lim_alts, 0);
      max_value = XEXP (loop->desc.var_alts, 0);
    }
  else
    {
      code = loop->desc.cond;
      min_value = XEXP (loop->desc.var_alts, 0);
      max_value = XEXP (loop->desc.lim_alts, 0);
    }
  if (loop->desc.neg)
    code = reverse_condition (code);

  /* The difference should always be positive and the code must be
     LT, LE, LTU, LEU, or NE.  Otherwise the loop is not normal, e.g.,
     `for (i = 0; i < 10; i--)', and it would not get here.  */
  switch (code)
    {
    case LTU:
    case LEU:
      {
	unsigned HOST_WIDE_INT umax;
	unsigned HOST_WIDE_INT umin;

	if (GET_CODE (min_value) == CONST_INT)
	  umin = INTVAL (min_value);
	else
	  umin = 0;

	if (GET_CODE (max_value) == CONST_INT)
	  umax = INTVAL (max_value);
	else
	  umax = ((unsigned) 2 << (GET_MODE_BITSIZE (mode) - 1)) - 1;

	n_iterations_max = umax - umin;
	break;
      }

    case LT:
    case LE:
      {
	HOST_WIDE_INT smax;
	HOST_WIDE_INT smin;

	if (GET_CODE (min_value) == CONST_INT)
	  smin = INTVAL (min_value);
	else
	  smin = -((unsigned) 1 << (GET_MODE_BITSIZE (mode) - 1));

	if (GET_CODE (max_value) == CONST_INT)
	  smax = INTVAL (max_value);
	else
	  smax = ((unsigned) 1 << (GET_MODE_BITSIZE (mode) - 1)) - 1;

	n_iterations_max = smax - smin;
	break;
      }

    case NE:
      if (GET_CODE (min_value) == CONST_INT
	  && GET_CODE (max_value) == CONST_INT)
	n_iterations_max = INTVAL (max_value) - INTVAL (min_value);
      else
	/* We need to conservatively assume that we might have the maximum
	   number of iterations without any additional knowledge.  */
	n_iterations_max = ((unsigned) 2 << (GET_MODE_BITSIZE (mode) - 1)) - 1;
      break;

    default:
      abort ();
    }

  n_iterations_max /= abs_inc;

  return n_iterations_max;
}


/* Return nonzero if the loop specified by LOOP is suitable for
   the use of special low-overhead looping instructions.  */
static bool
doloop_valid_p (loops, loop)
     struct loops *loops;
     struct loop *loop;
{
  basic_block *body = get_loop_body (loop), bb;
  rtx insn;
  unsigned i;
  enum rtx_code cond;

  for (i = 0; i < loop->num_nodes; i++)
    {
      bb = body[i];

      for (insn = bb->head;
	   insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  /* A called function may clobber any special registers required for
	     low-overhead looping.  */
	  if (GET_CODE (insn) == CALL_INSN)
	    {
	      if (rtl_dump_file)
		fprintf (rtl_dump_file,
			 "Doloop: Function call in loop.\n");
	      return false;
	    }

	  /* Some targets (eg, PPC) use the count register for branch on table
	     instructions.  ??? This should be a target specific check.  */
	  if (GET_CODE (insn) == JUMP_INSN
	      && (GET_CODE (PATTERN (insn)) == ADDR_DIFF_VEC
		  || GET_CODE (PATTERN (insn)) == ADDR_VEC))
	    {
	      if (rtl_dump_file)
		fprintf (rtl_dump_file,
			 "Doloop: Computed branch in the loop.\n");
	      return false;
	    }
	}
    }
  free (body);

  /* Check whether the loop is simple.  */
  if (!loop->has_desc)
    loop->simple = simple_loop_p (loops, loop, &loop->desc);
  if (!loop->simple)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 "Doloop: The loop is not simple.\n");
      return false;
    }

  /* Check for loops that may not terminate under special conditions.  */
  cond = loop->desc.cond;
  if (loop->desc.neg)
    cond = reverse_condition (cond);
  if (! loop->desc.const_iter
      && (cond == LEU
	  || cond == GEU
	  || (cond == LTU && INTVAL (loop->desc.stride) > 1)
	  || (cond == GTU && INTVAL (loop->desc.stride) < -1)))
    {
      /* If the comparison is LEU and the comparison value is UINT_MAX
	 then the loop will not terminate.  Similarly, if the
	 comparison code is GEU and the comparison value is 0, the
	 loop will not terminate.

	 If the absolute increment is not 1, the loop can be infinite
	 even with LTU/GTU, e.g. for (i = 3; i > 0; i -= 2)

	 Note that with LE and GE, the loop behavior is undefined
	 (C++ standard section 5 clause 5) if an overflow occurs, say
	 between INT_MAX and INT_MAX + 1.  We thus don't have to worry
	 about these two cases.

	 ??? We could compute these conditions at run-time and have a
	 additional jump around the loop to ensure an infinite loop.
	 However, it is very unlikely that this is the intended
	 behavior of the loop and checking for these rare boundary
	 conditions would pessimize all other code.

	 If the loop is executed only a few times an extra check to
	 restart the loop could use up most of the benefits of using a
	 count register loop.  Note however, that normally, this
	 restart branch would never execute, so it could be predicted
	 well by the CPU.  We should generate the pessimistic code by
	 default, and have an option, e.g. -funsafe-loops that would
	 enable count-register loops in this case.  */
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 "Doloop: Possible infinite iteration case ignored.\n");
    }

  return true;
}


/* Modify the loop to use the low-overhead looping insn where LOOP
   describes the loop, ITERATIONS is an RTX containing the desired
   number of loop iterations, ITERATIONS_MAX is a CONST_INT specifying
   the maximum number of loop iterations, and DOLOOP_INSN is the
   low-overhead looping insn to emit at the end of the loop.  This
   returns nonzero if it was successful.  */
static bool
doloop_modify (loop, iterations, iterations_max, doloop_seq, condition)
     const struct loop *loop;
     rtx iterations;
     rtx iterations_max;
     rtx doloop_seq;
     rtx condition;
{
  rtx counter_reg;
  rtx count;
  rtx sequence;
  rtx jump_insn;
  rtx jump_label;
  int nonneg = 0;
  int increment_count;
  basic_block loop_end = loop->desc.out_edge->src;

  jump_insn = loop_end->end;

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Doloop: Inserting doloop pattern (");
      if (GET_CODE (iterations) == CONST_INT)
	fprintf (rtl_dump_file, HOST_WIDE_INT_PRINT_DEC,
		 INTVAL (iterations));
      else
	fputs ("runtime", rtl_dump_file);
      fputs (" iterations).", rtl_dump_file);
    }

  /* Discard original jump to continue loop.  The original compare
     result may still be live, so it cannot be discarded explicitly.  */
  delete_insn (jump_insn);

  counter_reg = XEXP (condition, 0);
  if (GET_CODE (counter_reg) == PLUS)
    counter_reg = XEXP (counter_reg, 0);

  start_sequence ();

  count = iterations;
  increment_count = 0;
  switch (GET_CODE (condition))
    {
    case NE:
      /* Currently only NE tests against zero and one are supported.  */
      if (XEXP (condition, 1) == const1_rtx)
	increment_count = 1;
      else if (XEXP (condition, 1) != const0_rtx)
	abort ();
      break;

    case GE:
      /* Currently only GE tests against zero are supported.  */
      if (XEXP (condition, 1) != const0_rtx)
	abort ();

      /* The iteration count does not needs incrementing for a GE test.  */
      increment_count = 0;

      /* Determine if the iteration counter will be non-negative.
	 Note that the maximum value loaded is iterations_max - 1.  */
      if ((unsigned HOST_WIDE_INT) INTVAL (iterations_max)
	  <= ((unsigned) 1 << (GET_MODE_BITSIZE (GET_MODE (counter_reg)) - 1)))
	nonneg = 1;
      break;

      /* Abort if an invalid doloop pattern has been generated.  */
    default:
      abort ();
    }

  if (increment_count)
    {
      if (GET_CODE (count) == CONST_INT)
	count = GEN_INT (INTVAL (count) + 1);
      else
	count = expand_simple_binop (GET_MODE (counter_reg), PLUS,
				     count, const1_rtx,
				     0, 0, OPTAB_LIB_WIDEN);
    }

  /* Insert initialization of the count register into the loop header.  */
  convert_move (counter_reg, count, 1);
  sequence = get_insns ();
  end_sequence ();
  emit_insn_after (sequence, loop_preheader_edge (loop)->src->end);

  /* Some targets (eg, C4x) need to initialize special looping
     registers.  */
#ifdef HAVE_doloop_begin
  {
    rtx init;

    init = gen_doloop_begin (counter_reg,
			     GET_CODE (iterations) == CONST_INT
			     ? iterations : const0_rtx, iterations_max,
			     GEN_INT (level));
    if (init)
      {
	start_sequence ();
	emit_insn (init);
	sequence = get_insns ();
	end_sequence ();
	emit_insn_after (sequence, loop_preheader_edge (loop)->src->end);
      }
  }
#endif

  /* Insert the new low-overhead looping insn.  */
  emit_jump_insn_after (doloop_seq, loop_end->end);
  jump_insn = loop_end->end;
  jump_label = block_label (loop->desc.in_edge->dest);
  JUMP_LABEL (jump_insn) = jump_label;
  LABEL_NUSES (jump_label)++;

  /* Ensure the right fallthru edge is marked, for case we have reversed
     the condition.  */
  loop->desc.in_edge->flags &= ~EDGE_FALLTHRU;
  loop->desc.out_edge->flags |= EDGE_FALLTHRU;

  /* Add a REG_NONNEG note if the actual or estimated maximum number
     of iterations is non-negative.  */
  if (nonneg)
    {
      REG_NOTES (jump_insn)
	= gen_rtx_EXPR_LIST (REG_NONNEG, NULL_RTX, REG_NOTES (jump_insn));
    }
  return true;
}


/* Handle the more complex case, where the bounds are not known at
   compile time.  In this case we generate a run_time calculation of
   the number of iterations.  We rely on the existence of a run-time
   guard to ensure that the loop executes at least once, i.e.,
   initial_value obeys the loop comparison condition.  If a guard is
   not present, we emit one.  The loop to modify is described by LOOP.
   ITERATIONS_MAX is a CONST_INT specifying the estimated maximum
   number of loop iterations.  DOLOOP_SEQ is the low-overhead looping
   insn to insert.  Returns nonzero if loop successfully modified.  */
static bool
doloop_modify_runtime (loop, iterations_max, doloop_seq, condition)
     struct loop *loop;
     rtx iterations_max;
     rtx doloop_seq;
     rtx condition;
{
  rtx sequence;
  rtx iterations;

  start_sequence ();
  iterations = count_loop_iterations (&loop->desc, NULL, NULL);
  if (!iterations)
    abort ();
  sequence = get_insns ();
  end_sequence ();
  emit_insn_after (sequence, loop_preheader_edge (loop)->src->end);

  return doloop_modify (loop, iterations, iterations_max,
			doloop_seq, condition);
}


/* Process loop described by LOOP validating that the loop is suitable for
   conversion to use a low overhead looping instruction, replacing the jump
   insn where suitable.  We distinguish between loops with compile-time bounds
   and those with run-time bounds.  Information from LOOP is used to compute
   the number of iterations and to determine whether the loop is a candidate
   for this optimization.  Information on the whole loop tree is stored in
   LOOPS.  Returns true if the loop was successfully modified.  */
static bool
doloop_optimize (loops, loop)
     struct loops *loops;
     struct loop *loop;
{
  enum machine_mode mode;
  unsigned HOST_WIDE_INT n_iterations;
  unsigned HOST_WIDE_INT n_iterations_max;
  rtx doloop_seq, doloop_pat, doloop_reg;
  rtx iterations;
  rtx iterations_max;
  rtx start_label;
  rtx condition;
  int level;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Doloop: Processing loop %d.\n", loop->num);

  /* Ignore large loops.  */
  if (loop->ninsns > (unsigned) PARAM_VALUE (PARAM_MAX_DOLOOP_INSNS))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 "Doloop: The loop is too large.\n");
      return false;
    }

  /* Check that loop is a candidate for a low-overhead looping insn.  */
  if (! doloop_valid_p (loops, loop))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 "Doloop: The loop is not suitable.\n");
      return false;
    }
  mode = GET_MODE (loop->desc.var);

  /* Determine or estimate the maximum number of loop iterations.  */
  if (loop->desc.const_iter)
    {
      /* This is the simple case where the exact number of iterations may
	 be determined.  */
      n_iterations_max = n_iterations = loop->desc.niter;
    }
  else
    {
      n_iterations = 0;

      /* This is the harder case where the initial and final loop
	 values may not be constants.  */
      n_iterations_max = doloop_iterations_max (loop);
    }

  if (n_iterations && n_iterations < 3)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 "Doloop: Too few iterations (%ld) to be profitable.\n",
		 (long int) n_iterations);
      return false;
    }

  iterations = GEN_INT (n_iterations);
  iterations_max = GEN_INT (n_iterations_max);
  level = get_loop_level (loop);

  /* Generate looping insn.  If the pattern FAILs then give up trying
     to modify the loop since there is some aspect the back-end does
     not like.  */
  start_label = block_label (loop->desc.in_edge->dest);
  doloop_reg = gen_reg_rtx (mode);
  doloop_seq = gen_doloop_end (doloop_reg, iterations, iterations_max,
			       GEN_INT (level), start_label);
  if (! doloop_seq && mode != word_mode)
    {
      PUT_MODE (doloop_reg, word_mode);
      doloop_seq = gen_doloop_end (doloop_reg, iterations, iterations_max,
				   GEN_INT (level), start_label);
    }
  if (! doloop_seq)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 "Doloop: Target unwilling to use doloop pattern!\n");
      return false;
    }

  /* If multiple instructions were created, the last must be the
     jump instruction.  Also, a raw define_insn may yield a plain
     pattern.  */
  doloop_pat = doloop_seq;
  if (INSN_P (doloop_pat))
    {
      while (NEXT_INSN (doloop_pat) != NULL_RTX)
	doloop_pat = NEXT_INSN (doloop_pat);
      if (GET_CODE (doloop_pat) == JUMP_INSN)
	doloop_pat = PATTERN (doloop_pat);
      else
	doloop_pat = NULL_RTX;
    }

  if (! doloop_pat
      || ! (condition = doloop_condition_get (doloop_pat)))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 "Doloop: Unrecognizable doloop pattern!\n");
      return false;
    }

  if (loop->desc.const_iter)
    /* Handle the simpler case, where we know the iteration count at
       compile time.  */
    return doloop_modify (loop, iterations, iterations_max, doloop_seq,
			  condition);
  else
    /* Handle the harder case, where we must add additional runtime tests.  */
    return doloop_modify_runtime (loop, iterations_max, doloop_seq, condition);
}

/* This is the main entry point.  Process all LOOPS using doloop_optimize.  */
void
doloop_optimize_loops (loops)
     struct loops *loops;
{
  unsigned i;
  struct loop *loop;

  for (i = 1; i < loops->num; i++)
    {
      loop = loops->parray[i];
      if (!loop)
	continue;

      doloop_optimize (loops, loop);
    }
#ifdef ENABLE_CHECKING
  verify_dominators (loops->cfg.dom);
  verify_loop_structure (loops);
#endif
}
#endif /* HAVE_doloop_end */
