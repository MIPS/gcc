/* Conditional (or predicated) execution support.
   Copyright (C) 2000 Free Software Foundation, Inc.

   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"

#include "rtl.h"
#include "regs.h"
#include "function.h"
#include "flags.h"
#include "insn-config.h"
#include "recog.h"
#include "basic-block.h"
#include "expr.h"
#include "output.h"
#include "tm_p.h"


#ifndef MAX_CONDITIONAL_EXECUTE
#define MAX_CONDITIONAL_EXECUTE   (BRANCH_COST + 1)
#endif

#define EDGE_COMPLEX	(EDGE_ABNORMAL | EDGE_ABNORMAL_CALL | EDGE_EH)

#define NULL_EDGE	((struct edge_def *)NULL)
#define NULL_BLOCK	((struct basic_block_def *)NULL)

/* Flags placed in bb->aux.  */
#define AUX_UPDATE_LIFE		((void *) 1)

/* # of IF-THEN or IF-THEN-ELSE blocks we looked at  */
static int num_possible_if_blocks;

/* # of IF-THEN or IF-THEN-ELSE blocks were converted to conditional
   execution.  */
static int num_updated_if_blocks;

/* # of insns that were successfully modified.  */
static int num_modified_insns;

/* # of basic blocks that were removed.  */
static int num_removed_blocks;

/* Forward references.  */
#ifdef HAVE_conditional_execution
static int count_bb_insns		PARAMS ((basic_block));
static int cond_exec_process_insns	PARAMS ((rtx, rtx, rtx, int,
						 varray_type));
static void cond_exec_unprocess_insns	PARAMS ((varray_type));
static int cond_exec_process_if_block	PARAMS ((basic_block, basic_block,
						 basic_block, basic_block));
#endif

static int noce_process_if_block	PARAMS ((basic_block, basic_block,
						 basic_block, basic_block));

static int find_if_block		PARAMS ((basic_block));
static int process_if_block		PARAMS ((basic_block, basic_block,
						 basic_block, basic_block));
static void merge_if_block		PARAMS ((basic_block, basic_block,
						 basic_block, basic_block));

#ifdef HAVE_conditional_execution
/* Count the number of non-jump active insns in BB.  */

static int
count_bb_insns (bb)
     basic_block bb;
{
  int count = 0;
  rtx insn = bb->head;

  while (1)
    {
      if (GET_CODE (insn) == CALL_INSN || GET_CODE (insn) == INSN)
	count++;

      if (insn == bb->end)
	break;
      insn = NEXT_INSN (insn);
    }

  return count;
}
#endif /* HAVE_conditional_execution */

/* Return the first non-jump active insn in the basic block.  */

static rtx
first_active_insn (bb)
     basic_block bb;
{
  rtx insn = bb->head;

  if (GET_CODE (insn) == CODE_LABEL)
    {
      if (insn == bb->end)
	return NULL_RTX;
      insn = NEXT_INSN (insn);
    }

  while (GET_CODE (insn) == NOTE)
    {
      if (insn == bb->end)
	return NULL_RTX;
      insn = NEXT_INSN (insn);
    }

  if (GET_CODE (insn) == JUMP_INSN)
    return NULL_RTX;

  return insn;
}

/* Return true if INSN is the last active non-jump insn in BB.  */

static int
last_active_insn_p (bb, insn)
     basic_block bb;
     rtx insn;
{
  do
    {
      if (insn == bb->end)
	return TRUE;
      insn = NEXT_INSN (insn);
    }
  while (GET_CODE (insn) == NOTE);

  return GET_CODE (insn) == JUMP_INSN;
}

#ifdef HAVE_conditional_execution

/* Go through a bunch of insns, converting them to conditional
   execution format if possible.  Return TRUE if all of the non-note
   insns were processed.  */

static int
cond_exec_process_insns (start, end, test, call_ok, undo)
     rtx start;			/* first insn to look at */
     rtx end;			/* last insn to look at */
     rtx test;			/* conditional execution test */
     int call_ok;		/* != 0 if cond. calls are ok as last insn */
     varray_type undo;		/* undo buffer */
{
  int must_be_last = FALSE;
  rtx insn;
  rtx seq;
  rtx set;
  rtx *p_set;

  for (insn = start; ; insn = NEXT_INSN (insn))
    {
      if (GET_CODE (insn) == NOTE)
	goto insn_done;

      if (GET_CODE (insn) != INSN && GET_CODE (insn) != CALL_INSN)
	abort ();

      /* Last insn wasn't last?  */
      if (must_be_last)
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "Insn not last at %d.\n",
		     INSN_UID (insn));
	  return FALSE;
	}

      /* See if we need to split the current insn.  */
      while ((seq = split_insns (PATTERN (insn), insn)) != NULL_RTX)
	{
	  rtx p = insn;
	  emit_insn_after (seq, insn);
	  insn = NEXT_INSN (insn);
	  delete_insn (p);
	}

      if (GET_CODE (insn) == CALL_INSN)
	{
	  if (call_ok)
	    must_be_last = TRUE;
	  else
	    {
	      if (rtl_dump_file)
		fprintf (rtl_dump_file,
			 "Call insn %d terminates conditional execution.\n",
			 INSN_UID (insn));
	      return FALSE;
	    }
	}
      else if (modified_in_p (test, PATTERN (insn)))
	must_be_last = TRUE;

      set = single_set (insn);
      if (! set)
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "Insn %d is not a single_set.\n",
		     (int)INSN_UID (insn));
	  return FALSE;
	}

      /* Now build the conditional form of the instruction.  */
      if (GET_CODE (PATTERN (insn)) == SET)
	p_set = &PATTERN (insn);
      else if (GET_CODE (PATTERN (insn)) == PARALLEL
	       && XVECEXP (PATTERN (insn), 0, 0) == set)
	p_set = &XVECEXP (PATTERN (insn), 0, 0);
      else
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "Insn %d is not a canonical single set.\n",
		     (int)INSN_UID (insn));
	  return FALSE;
	}

      *p_set = gen_rtx_COND_EXEC (VOIDmode, copy_rtx (test), set);

      /* Remember the insn so we can undo it later.  */
      VARRAY_PUSH_RTX (undo, insn);

      /* Ensure recog will be rerun.  */
      INSN_CODE (insn) = -1;
      if (recog_memoized (insn) < 0)
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file,
		     "Insn %d can not be conditionally executed.\n",
		     INSN_UID (insn));
	  return FALSE;
	}

    insn_done:
      if (insn == end)
	break;
    }

  return TRUE;
}

/* Revert changes made in cond_exec_process_insns.  */

static void
cond_exec_unprocess_insns (undo)
     varray_type undo;
{
  while (VARRAY_ACTIVE_SIZE (undo) > 0)
    {
      rtx insn = VARRAY_TOP_RTX (undo);
      rtx pattern = PATTERN (insn);

      VARRAY_POP (undo);

      /* Ensure recog will be rerun.  */
      INSN_CODE (insn) = -1;

      if (GET_CODE (pattern) == COND_EXEC)
	PATTERN (insn) = COND_EXEC_CODE (pattern);
      else if (GET_CODE (pattern) == PARALLEL
	       && GET_CODE (XVECEXP (pattern, 0, 0)) == COND_EXEC)
	XVECEXP (pattern, 0, 0)
	  = COND_EXEC_CODE (XVECEXP (pattern, 0, 0));
      else
	abort ();

      if (rtl_dump_file)
	fprintf (rtl_dump_file, "Undoing conditional execution to insn %d\n",
		 INSN_UID (insn));
    }
}

/* Given a simple IF-THEN or IF-THEN-ELSE block, attempt to convert it
   to conditional execution.  Return TRUE if we were successful at
   converting the the block. 

   ??? Future work: Convert

      (set pc (if_then_else test over pc))
      (foo)
      (bar)
      (set pc elsewhere)
      (code_label over)

   into

      (cond_exec !test foo)
      (cond_exec !test bar)
      (set pc (if_then_else !test elsewhere pc))

   This is surprisingly common, especially when "elsewhere" is the
   return label and "foo" sets up the return value.  */

static int
cond_exec_process_if_block (test_bb, then_bb, else_bb, join_bb)
     basic_block test_bb;	/* Basic block test is in */
     basic_block then_bb;	/* Basic block for THEN block */
     basic_block else_bb;	/* Basic block for ELSE block */
     basic_block join_bb;	/* Basic block the join label is in */
{
  rtx test_jump;		/* JUMP_INSN of IF_THEN_ELSE */
  rtx test_if;			/* IF_THEN_ELSE extracted from test_jump */
  rtx test_expr;		/* expression in IF_THEN_ELSE that is tested */
  rtx then_start;		/* first insn in THEN block */
  rtx then_end;			/* last insn + 1 in THEN block */
  rtx else_start;		/* first insn in ELSE block or NULL */
  rtx else_end;			/* last insn + 1 in ELSE block */
  int max;			/* max # of insns to convert. */
  int then_call_ok;		/* whether conditional calls are ok in THEN */
  rtx true_expr;		/* test for else block insns */
  rtx false_expr;		/* test for then block insns */
  varray_type undo;		/* undo buffer for conversion */
  int n_insns;

  /* Find the conditional jump to the ELSE or JOIN part, and isolate
     the test.  */
  test_jump = test_bb->end;
  if (GET_CODE (test_jump) == NOTE)
    test_jump = prev_nonnote_insn (test_jump);

  if (condjump_p (test_jump))
    test_if = XEXP (PATTERN (test_jump), 1);
  else if (condjump_in_parallel_p (test_jump))
    test_if = XEXP (XVECEXP (PATTERN (test_jump), 0, 0), 1);
  else
    {
      if (rtl_dump_file)
	fputs ("IF jump is not recognized.\n", rtl_dump_file);
      return FALSE;
    }

  test_expr = XEXP (test_if, 0);
  if (GET_RTX_CLASS (GET_CODE (test_expr)) != '<')
    {
      if (rtl_dump_file)
	fputs ("Non-relational operator in test.\n", rtl_dump_file);
      return FALSE;
    }

  /* Collect the bounds of where we're to search.  */
  then_start = then_bb->head;
  then_end = then_bb->end;
  if (else_bb)
    {
      /* Don't process the jump at the end of the THEN block.  */
      then_end = PREV_INSN (then_bb->end);

      /* Skip the ELSE block's label.  */
      else_start = NEXT_INSN (else_bb->head);
      else_end = else_bb->end;
    }

  /* How many instructions should we convert in total?  */
  n_insns = 0;
  if (else_bb)
    {
      max = 2 * MAX_CONDITIONAL_EXECUTE;
      n_insns = count_bb_insns (else_bb);
    }
  else
    max = MAX_CONDITIONAL_EXECUTE;
  n_insns += count_bb_insns (then_bb);
  if (n_insns > max)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "Too many insns (%d > %d)\n", n_insns, max);
      return FALSE;
    }

  /* Map test_expr/test_jump into the appropriate MD tests to use on
     the conditionally executed code.  Note, we need to reverse the
     sense of the true and false tests if the optimizer has swapped
     the pc and label.  */
  
  true_expr = test_expr;
  false_expr = gen_rtx_fmt_ee (reverse_condition (GET_CODE (true_expr)),
			       GET_MODE (true_expr), XEXP (true_expr, 0),
			       XEXP (true_expr, 1));
  if (GET_CODE (XEXP (test_if, 1)) == PC)
    {
      rtx tmp = true_expr;
      true_expr = false_expr;
      false_expr = tmp;
    }

  /* For IF-THEN-ELSE blocks, we don't allow calls on then THEN block to
     make sure that the conditional code flag is not modified by the call.  */
  /* ??? IA-64 has somewhere over 50 call-saved predicate registers.
     Figure out how to relax this.  */
  then_call_ok = (else_bb == NULL_BLOCK);

  /* Go through the THEN and ELSE blocks converting the insns if possible
     to conditional execution.  */

  VARRAY_RTX_INIT (undo, n_insns, "cond_exec undo");

  if (then_end
      && ! cond_exec_process_insns (then_start, then_end,
				    false_expr, then_call_ok, undo))
    goto fail;

  if (else_bb
      && ! cond_exec_process_insns (else_start, else_end,
				    true_expr, TRUE, undo))
    goto fail;

  /* Conversion succeeded.  */
  num_modified_insns += n_insns;
  if (rtl_dump_file)
    fprintf (rtl_dump_file, "%d insn%s converted to conditional execution.\n",
	     n_insns, (n_insns == 1) ? " was" : "s were");

  VARRAY_FREE (undo);

  /* Merge the blocks!  */
  merge_if_block (test_bb, then_bb, else_bb, join_bb);

  return TRUE;

 fail:
  cond_exec_unprocess_insns (undo);
  VARRAY_FREE (undo);
  return FALSE;
}
#endif /* HAVE_conditional_execution */

/* Used by noce_process_if_block to communicate with its subroutines. 

   The subroutines know that A and B may be evaluated freely.  They
   know that X is a register.  They should insert new instructions 
   before cond_earliest.  */

struct noce_if_info
{
  rtx insn_a, insn_b;
  rtx x, a, b;
  rtx jump, cond, cond_earliest;
};

static int noce_try_store_flag		PARAMS ((struct noce_if_info *));
static int noce_try_store_flag_arith	PARAMS ((struct noce_if_info *));
#ifdef HAVE_conditional_move
static int noce_try_cmove		PARAMS ((struct noce_if_info *));
static int noce_try_cmove_arith		PARAMS ((struct noce_if_info *));
#endif

/* Only try 0 and STORE_FLAG_VALUE here.  Other combinations will be
   tried in noce_try_store_flag_arith after noce_try_cmove has had a
   go at the conversion.  */

static int
noce_try_store_flag (if_info)
     struct noce_if_info *if_info;
{
  enum rtx_code code;
  rtx target, seq;

  if (GET_CODE (if_info->b) == CONST_INT
      && INTVAL (if_info->b) == STORE_FLAG_VALUE
      && if_info->a == const0_rtx)
    code = GET_CODE (if_info->cond);
  else if (if_info->b == const0_rtx
	   && GET_CODE (if_info->a) == CONST_INT
	   && INTVAL (if_info->a) == STORE_FLAG_VALUE
	   && can_reverse_comparison_p (if_info->cond, if_info->jump))
    code = reverse_condition (GET_CODE (if_info->cond));
  else
    return FALSE;

  start_sequence ();

  target = emit_store_flag (if_info->x, code, XEXP (if_info->cond, 0),
			    XEXP (if_info->cond, 1), VOIDmode,
			    (code == LTU || code == LEU
			     || code == GEU || code == GTU), 0);

  if (target)
    {
      if (target != if_info->x)
	emit_move_insn (if_info->x, target);

      seq = get_insns ();
      end_sequence ();
      emit_insns_before (seq, if_info->cond_earliest);

      return TRUE;
    }
  else
    {
      end_sequence ();
      return FALSE;
    }
}

/* Try more complex cases involving store_flag.  */

static int
noce_try_store_flag_arith (if_info)
     struct noce_if_info *if_info;
{
  enum rtx_code code;
  rtx target, seq;
  int reversep;

  /* Convert "if (test) foo++" into "foo += (test != 0)", and 
     similarly for "foo--".  */

  if (! no_new_pseudos
      && (BRANCH_COST >= 2
#ifdef HAVE_incscc
	  || HAVE_incscc
#endif
#ifdef HAVE_decscc
	  || HAVE_decscc
#endif
	  )
      /* Should be no `else' case to worry about.  */
      && if_info->b == if_info->x
      && GET_CODE (if_info->a) == PLUS
      && (XEXP (if_info->a, 1) == const1_rtx
	  || XEXP (if_info->a, 1) == constm1_rtx)
      && rtx_equal_p (XEXP (if_info->a, 0), if_info->x)
      && can_reverse_comparison_p (if_info->cond, if_info->jump))
    {
      int subtract, normalize;

      code = reverse_condition (GET_CODE (if_info->cond));

      if (STORE_FLAG_VALUE == INTVAL (XEXP (if_info->a, 1)))
	subtract = 0, normalize = 0;
      else if (-STORE_FLAG_VALUE == INTVAL (XEXP (if_info->a, 1)))
	subtract = 1, normalize = 0;
      else
	subtract = 0, normalize = INTVAL (XEXP (if_info->a, 1));
      
      start_sequence ();

      target = emit_store_flag (gen_reg_rtx (GET_MODE (if_info->x)), code, 
				XEXP (if_info->cond, 0),
				XEXP (if_info->cond, 1), VOIDmode,
				(code == LTU || code == LEU
				 || code == GTU || code == GEU),
				normalize);

      if (target)
	target = expand_binop (GET_MODE (if_info->x),
			       subtract ? sub_optab : add_optab,
			       if_info->x, target, if_info->x, 0, OPTAB_WIDEN);
      if (target)
	{
	  if (target != if_info->x)
	    emit_move_insn (if_info->x, target);

	  seq = get_insns ();
	  end_sequence ();
	  emit_insns_before (seq, if_info->cond_earliest);

	  return TRUE;
	}

      end_sequence ();
      return FALSE;
    }
				
  /* Convert A and B constant, but not 0 and STORE_TEST_FLAG, which was
     taken care of earlier.  Branches have to be moderately expensive.  */

  if (! no_new_pseudos
      && GET_CODE (if_info->a) == CONST_INT
      && GET_CODE (if_info->b) == CONST_INT)
    {
      HOST_WIDE_INT ai = INTVAL (if_info->a);
      HOST_WIDE_INT bi = INTVAL (if_info->b);
      HOST_WIDE_INT diff = ai - bi;
      HOST_WIDE_INT tmp;
      int normalize, can_reverse;

      can_reverse = can_reverse_comparison_p (if_info->cond, if_info->jump);

      reversep = 0;
      if (diff == STORE_FLAG_VALUE || diff == -STORE_FLAG_VALUE)
	normalize = 0;
      else if (bi == 0 && exact_log2 (ai) >= 0
	       && (STORE_FLAG_VALUE == 1
		   || BRANCH_COST >= 2))
	normalize = 1;
      else if (ai == 0 && exact_log2 (bi) >= 0 && can_reverse
	       && (STORE_FLAG_VALUE == 1 || BRANCH_COST >= 2))
	normalize = 1, reversep = 1;
      else if (ai == -1
	       && (STORE_FLAG_VALUE == -1
		   || BRANCH_COST >= 2))
	normalize = -1;
      else if (bi == -1 && can_reverse
	       && (STORE_FLAG_VALUE == -1 || BRANCH_COST >= 2))
	normalize = -1, reversep = 1;
      else if ((BRANCH_COST >= 2 && STORE_FLAG_VALUE == -1)
	       || BRANCH_COST >= 3)
	normalize = -1;
      else
	return FALSE;

      code = GET_CODE (if_info->cond);
      if (reversep)
      	{
	  code = reverse_condition (code);
	  tmp = ai, ai = bi, bi = tmp;
	  diff = -diff;
	}

      start_sequence ();
      target = emit_store_flag (if_info->x, code, XEXP (if_info->cond, 0),
				XEXP (if_info->cond, 1), VOIDmode,
				(code == LTU || code == LEU
				 || code == GEU || code == GTU), 0);
      if (! target)
	{
	  end_sequence ();
	  return FALSE;
	}

      /* if (test) x = 3; else x = 4;
	 =>   x = 4 - (test != 0);  */
      if (diff == STORE_FLAG_VALUE || diff == -STORE_FLAG_VALUE)
	{
	  target = expand_binop (GET_MODE (if_info->x),
				 (diff == STORE_FLAG_VALUE
				  ? sub_optab : add_optab),
				 if_info->a, target, if_info->x, 0,
				 OPTAB_WIDEN);
	}

      /* if (test) x = 8; else x = 0;
	 =>   x = (test != 0) << 3;  */
      else if (bi == 0 && (tmp = exact_log2 (ai)) >= 0)
	{
	  target = expand_binop (GET_MODE (if_info->x), ashl_optab,
				 target, GEN_INT (tmp), if_info->x, 0,
				 OPTAB_WIDEN);
	}

      /* if (test) x = a; else x = b;
	 =>   x = (-(test != 0) & (a - b)) + b;  */
      else
	{
	  target = expand_binop (GET_MODE (if_info->x), and_optab,
				 target, GEN_INT (diff), if_info->x, 0,
				 OPTAB_WIDEN);
	  target = expand_binop (GET_MODE (if_info->x), add_optab,
				 target, GEN_INT (bi), if_info->x, 0,
				 OPTAB_WIDEN);
	}

      if (! target)
	{
	  end_sequence ();
	  return FALSE;
	}

      if (target != if_info->x)
	emit_move_insn (if_info->x, target);

      seq = get_insns ();
      end_sequence ();
      emit_insns_before (seq, if_info->cond_earliest);

      return TRUE;
    }

  /* if (test) x = 0;
     =>   x &= -(test == 0);  */

  reversep = 0;
  if (! no_new_pseudos
      && (BRANCH_COST >= 2
	  || STORE_FLAG_VALUE == -1)
      && ((if_info->b == const0_rtx
	   && rtx_equal_p (if_info->a, if_info->x))
	  || ((reversep = can_reverse_comparison_p (if_info->cond,
						    if_info->jump))
	      && if_info->a == const0_rtx
	      && rtx_equal_p (if_info->b, if_info->x))))
    {
      code = GET_CODE (if_info->cond);
      if (reversep)
	code = reverse_condition (code);

      start_sequence ();
      target = emit_store_flag (gen_reg_rtx (GET_MODE (if_info->x)), code,
				XEXP (if_info->cond, 0),
				XEXP (if_info->cond, 1), VOIDmode,
				(code == LTU || code == LEU
				 || code == GEU || code == GTU), -1);
      if (! target)
	{
	  end_sequence ();
	  return FALSE;
	}

      target = expand_binop (GET_MODE (if_info->x), and_optab,
			     if_info->x, target, if_info->x, 0,
			     OPTAB_WIDEN);

      if (target != if_info->x)
	emit_move_insn (if_info->x, target);

      seq = get_insns ();
      end_sequence ();
      emit_insns_before (seq, if_info->cond_earliest);

      return TRUE;
    }

  return FALSE;
}

#ifdef HAVE_conditional_move
/* Try only simple constants and registers here.  More complex cases
   are handled in noce_try_cmove_arith after noce_try_store_flag_arith
   has had a go at it.  */

static int
noce_try_cmove (if_info)
     struct noce_if_info *if_info;
{
  enum rtx_code code;
  rtx target, seq;

  if ((CONSTANT_P (if_info->a) || register_operand (if_info->a, VOIDmode))
      && (CONSTANT_P (if_info->b) || register_operand (if_info->b, VOIDmode)))
    {
      start_sequence ();

      code = GET_CODE (if_info->cond);
      target = emit_conditional_move (if_info->x, code,
				      XEXP (if_info->cond, 0),
				      XEXP (if_info->cond, 1), VOIDmode,
				      if_info->b, if_info->a,
				      GET_MODE (if_info->x),
				      (code == LTU || code == GEU
				       || code == LEU || code == GTU));

      if (target)
	{
	  if (target != if_info->x)
	    emit_move_insn (if_info->x, target);

	  seq = get_insns ();
	  end_sequence ();
	  emit_insns_before (seq, if_info->cond_earliest);
	  return TRUE;
	}
      else
	{
	  end_sequence ();
	  return FALSE;
	}
    }

  return FALSE;
}

/* Try more complex cases involving conditional_move.  */

static int
noce_try_cmove_arith (if_info)
     struct noce_if_info *if_info;
{
  rtx a = if_info->a;
  rtx b = if_info->b;
  rtx x = if_info->x;
  rtx insn_a, insn_b;
  rtx tmp, target;
  int is_mem = 0;
  enum rtx_code code;

  /* A conditional move from two memory sources is equivalent to a
     conditional on their addresses followed by a load.  Don't do this
     early because it'll screw alias analysis.  Note that we've
     already checked for no side effects.  */
  if (! no_new_pseudos && cse_not_expected
      && GET_CODE (a) == MEM && GET_CODE (b) == MEM
      && BRANCH_COST >= 5)
    {
      a = XEXP (a, 0);
      b = XEXP (b, 0);
      x = gen_reg_rtx (Pmode);
      is_mem = 1;
    }

  /* ??? We could handle this if we knew that a load from A or B could
     not fault.  This is true of stack memories or if we've already loaded
     from the address along the path from ENTRY.  */
  else if (GET_CODE (a) == MEM || GET_CODE (b) == MEM)
    return FALSE;

  /* if (test) x = a + b; else x = c - d;
     => y = a + b;
        x = c - d;
	if (test)
	  x = y;
  */
  
  code = GET_CODE (if_info->cond);
  insn_a = if_info->insn_a;
  insn_b = if_info->insn_b;

  /* Possibly rearrange operands to make things come out more natural.  */
  if (can_reverse_comparison_p (if_info->cond, if_info->jump))
    {
      int reversep = 0;
      if (rtx_equal_p (b, x))
	reversep = 1;
      else if (general_operand (b, GET_MODE (b)))
	reversep = 1;

      if (reversep)
	{
	  code = reverse_condition (code);
	  tmp = a, a = b, b = tmp;
	  tmp = insn_a, insn_a = insn_b, insn_b = tmp;
	}
    }

  start_sequence ();

  /* If either operand is complex, load it into a register first.
     The best way to do this is to copy the original insn.  In this
     way we preserve any clobbers etc that the insn may have had.  
     This is of course not possible in the IS_MEM case.  */
  if (! general_operand (a, GET_MODE (a)))
    {
      rtx set;

      if (no_new_pseudos)
	goto end_seq_and_fail;

      if (is_mem)
	{
	  tmp = gen_reg_rtx (GET_MODE (a));
	  tmp = emit_insn (gen_rtx_SET (VOIDmode, tmp, a));
	}
      else
	{
	  a = gen_reg_rtx (GET_MODE (a));
	  tmp = copy_rtx (insn_a);
	  set = single_set (tmp);
	  SET_DEST (set) = a;
	  tmp = emit_insn (PATTERN (tmp));
	}
      if (recog_memoized (tmp) < 0)
	goto end_seq_and_fail;
    }
  if (! general_operand (b, GET_MODE (b)))
    {
      rtx set;

      if (no_new_pseudos)
	goto end_seq_and_fail;

      if (is_mem)
	{
          tmp = gen_reg_rtx (GET_MODE (b));
	  tmp = emit_insn (gen_rtx_SET (VOIDmode, tmp, b));
	}
      else if (! insn_b)
	goto end_seq_and_fail;
      else
	{
          b = gen_reg_rtx (GET_MODE (b));
	  tmp = copy_rtx (insn_b);
	  set = single_set (tmp);
	  SET_DEST (set) = b;
	  tmp = emit_insn (PATTERN (tmp));
	}
      if (recog_memoized (tmp) < 0)
	goto end_seq_and_fail;
    }

  target = emit_conditional_move (x, code, XEXP (if_info->cond, 0),
				  XEXP (if_info->cond, 1), VOIDmode,
				  b, a, GET_MODE (x),
				  (code == LTU || code == GEU
				   || code == LEU || code == GTU));

  if (! target)
    goto end_seq_and_fail;

  /* If we're handling a memory for above, emit the load now.  */
  if (is_mem)
    {
      tmp = gen_rtx_MEM (GET_MODE (if_info->x), target);

      /* Copy over flags as appropriate.  */
      if (MEM_VOLATILE_P (if_info->a) || MEM_VOLATILE_P (if_info->b))
	MEM_VOLATILE_P (tmp) = 1;
      if (MEM_IN_STRUCT_P (if_info->a) && MEM_IN_STRUCT_P (if_info->b))
	MEM_IN_STRUCT_P (tmp) = 1;
      if (MEM_SCALAR_P (if_info->a) && MEM_SCALAR_P (if_info->b))
	MEM_SCALAR_P (tmp) = 1;
      if (MEM_ALIAS_SET (if_info->a) == MEM_ALIAS_SET (if_info->b))
	MEM_ALIAS_SET (tmp) = MEM_ALIAS_SET (if_info->a);

      emit_move_insn (if_info->x, tmp);
    }
  else if (target != x)
    emit_move_insn (x, target);

  tmp = get_insns ();
  end_sequence ();
  emit_insns_before (tmp, if_info->cond_earliest);
  return TRUE;

 end_seq_and_fail:
  end_sequence ();
  return FALSE;
}
#endif /* HAVE_conditional_move */

/* Given a simple IF-THEN or IF-THEN-ELSE block, attempt to convert it
   without using conditional execution.  Return TRUE if we were
   successful at converting the the block.  */

static int
noce_process_if_block (test_bb, then_bb, else_bb, join_bb)
     basic_block test_bb;	/* Basic block test is in */
     basic_block then_bb;	/* Basic block for THEN block */
     basic_block else_bb;	/* Basic block for ELSE block */
     basic_block join_bb;	/* Basic block the join label is in */
{
  /* We're looking for patterns of the form

     (1) if (...) x = a; else x = b;
     (2) x = b; if (...) x = a;
     (3) if (...) x = a;   // as if with an initial x = x.

     The later patterns require jumps to be more expensive.

     ??? For future expansion, look for multiple X in such patterns.  */

  struct noce_if_info if_info;
  rtx insn_a, insn_b;
  rtx set_a, set_b;
  rtx orig_x, x, a, b;
  rtx jump, cond;

  /* Look for the condition for the jump first.  We'd prefer to avoid
     get_condition if we can -- it tries to look back for the contents
     of an original compare.  On targets that use normal integers for
     comparisons, e.g. alpha, this is wasteful.  */

  /* If this is not a standard conditional jump, we can't parse it.  */
  jump = test_bb->end;
  if (GET_CODE (jump) != JUMP_INSN
      || ! condjump_p (jump) || simplejump_p (jump))
    return FALSE;

  /* If the condition variable is a register and is MODE_INT, accept it.
     Otherwise, fall back on get_condition.  */

  cond = XEXP (SET_SRC (PATTERN (jump)), 0);
  if (GET_CODE (XEXP (cond, 0)) == REG
      && GET_MODE_CLASS (GET_MODE (XEXP (cond, 0))) == MODE_INT)
    {
      if_info.cond_earliest = jump;

      /* If this branches to JUMP_LABEL when the condition is false,
	 reverse the condition.  */
      if (GET_CODE (XEXP (SET_SRC (PATTERN (jump)), 2)) == LABEL_REF
	  && XEXP (XEXP (SET_SRC (PATTERN (jump)), 2), 0) == JUMP_LABEL (jump))
	cond = gen_rtx_fmt_ee (reverse_condition (GET_CODE (cond)),
			       GET_MODE (cond), XEXP (cond, 0),
			       XEXP (cond, 1));
    }
  else 
    {
      cond = get_condition (test_bb->end, &if_info.cond_earliest);
      if (! cond)
	return FALSE;
    }

  /* We must be comparing objects whose modes imply the size.  */
  if (GET_MODE (XEXP (cond, 0)) == BLKmode)
    return FALSE;

  /* Look for one of the potential sets.  */
  insn_a = first_active_insn (then_bb);
  if (! insn_a
      || ! last_active_insn_p (then_bb, insn_a)
      || (set_a = single_set (insn_a)) == NULL_RTX)
    return FALSE;

  x = SET_DEST (set_a);
  a = SET_SRC (set_a);

  /* Look for the other potential set.  Make sure we've got equivalent
     destinations.  */
  /* ??? This is overconservative.  Storing to two different mems is
     as easy as conditionally computing the address.  Storing to a
     single mem merely requires a scratch memory to use as one of the
     destination addresses; often the memory immediately below the
     stack pointer is available for this.  */
  if (else_bb)
    {
      insn_b = first_active_insn (else_bb);
      if (! insn_b
	  || ! last_active_insn_p (else_bb, insn_b)
	  || (set_b = single_set (insn_b)) == NULL_RTX
	  || ! rtx_equal_p (x, SET_DEST (set_b)))
	return FALSE;
    }
  else
    {
      insn_b = prev_nonnote_insn (if_info.cond_earliest);
      if (GET_CODE (insn_b) != INSN
	  || (set_b = single_set (insn_b)) == NULL_RTX
	  || ! rtx_equal_p (x, SET_DEST (set_b)))
	insn_b = set_b = NULL_RTX;
    }
  b = (set_b ? SET_SRC (set_b) : x);

  /* Only operate on register destinations, and even then avoid extending
     the lifetime of hard registers on small register class machines.  */
  orig_x = x;
  if (GET_CODE (x) != REG
      || (SMALL_REGISTER_CLASSES
	  && REGNO (x) < FIRST_PSEUDO_REGISTER))
    {
      if (no_new_pseudos)
	return FALSE;
      x = gen_reg_rtx (GET_MODE (x));
    }

  /* Don't operate on sources that may trap or are volatile.  */
  if (side_effects_p (a) || side_effects_p (b)
      || (GET_CODE (a) != MEM && may_trap_p (a))
      || (GET_CODE (b) != MEM && may_trap_p (b)))
    return FALSE;

  /* Set up the info block for our subroutines.  */
  if_info.cond = cond;
  if_info.jump = jump;
  if_info.insn_a = insn_a;
  if_info.insn_b = insn_b;
  if_info.x = x;
  if_info.a = a;
  if_info.b = b;

  /* Try optimizations in some approximation of a useful order.  */
  /* ??? Should first look to see if X is live incoming at all.  If it
     isn't, we don't need anything but an unconditional set.  */

  /* Look and see if A and B are really the same.  Avoid creating silly
     cmove constructs that no one will fix up later.  */
  if (rtx_equal_p (a, b))
    {
      /* If we have an INSN_B, we don't have to create any new rtl.  Just
	 move the instruction that we already have.  If we don't have an
	 INSN_B, that means that A == X, and we've got a noop move.  In
	 that case don't do anything and let the code below delete INSN_A.  */
      if (insn_b && else_bb)
	{
	  if (else_bb && insn_b == else_bb->end)
	    else_bb->end = PREV_INSN (insn_b);
	  reorder_insns (insn_b, insn_b, PREV_INSN (if_info.cond_earliest));
	  insn_b = NULL_RTX;
	  x = orig_x;
	}
      goto success;
    }

  if (noce_try_store_flag (&if_info))
    goto success;
#ifdef HAVE_conditional_move
  if (noce_try_cmove (&if_info))
    goto success;
#endif
  if (noce_try_store_flag_arith (&if_info))
    goto success;
#ifdef HAVE_conditional_move
  if (noce_try_cmove_arith (&if_info))
    goto success;
#endif

  return FALSE;

 success:
  /* The original sets may now be killed.  */
  if (insn_a == then_bb->end)
    then_bb->end = PREV_INSN (insn_a);
  flow_delete_insn (insn_a);

  /* Several special cases here: First, we may have reused insn_b above,
     in which case insn_b is now NULL.  Second, we want to delete insn_b
     if it came from the ELSE block, because follows the now correct
     write that appears in the TEST block.  However, if we got insn_b from
     the TEST block, it may in fact be loading data needed for the comparison.
     We'll let life_analysis remove the insn if it's really dead.  */
  if (insn_b && else_bb)
    {
      if (insn_b == else_bb->end)
	else_bb->end = PREV_INSN (insn_b);
      flow_delete_insn (insn_b);
    }

  /* The new insns will have been inserted before cond_earliest.  We should
     be able to remove cond_earliest through the jump with impunity.  */
  insn_a = prev_nonnote_insn (if_info.cond_earliest);
  flow_delete_insn_chain (if_info.cond_earliest, test_bb->end);
  test_bb->end = insn_a;

  /* If we used a temporary, fix it up now.  */
  if (orig_x != x)
    {
      start_sequence ();
      emit_move_insn (orig_x, x);
      insn_b = gen_sequence ();
      end_sequence ();

      test_bb->end = emit_insn_after (insn_b, insn_a);
    }

  /* Merge the blocks!  */
  merge_if_block (test_bb, then_bb, else_bb, join_bb);

  return TRUE;
}

/* Determine if a given basic block heads a simple IF-THEN or IF-THEN-ELSE
   block.  If so, we'll try to convert the insns to not require the branch.
   Return TRUE if we were successful at converting the the block.  */

static int
find_if_block (test_bb)
     basic_block test_bb;	/* basic block to check out */
{
  basic_block then_bb = NULL_BLOCK;
  basic_block else_bb = NULL_BLOCK;
  basic_block join_bb = NULL_BLOCK;
  edge then_edge;
  edge else_edge;
  edge then_succ;
  edge else_succ;
  int next_index;

  /* The kind of block we're looking for has exactly two successors.  */
  if ((then_edge = test_bb->succ) == NULL_EDGE
      || (else_edge = then_edge->succ_next) == NULL_EDGE
      || else_edge->succ_next != NULL_EDGE)
    return FALSE;

  /* Neither edge should be abnormal.  */
  if ((then_edge->flags & EDGE_COMPLEX)
      || (else_edge->flags & EDGE_COMPLEX))
    return FALSE;

  /* The THEN edge is canonically the one that falls through.  */
  if (then_edge->flags & EDGE_FALLTHRU)
    ;
  else if (else_edge->flags & EDGE_FALLTHRU)
    {
      edge e = else_edge;
      else_edge = then_edge;
      then_edge = e;
    }
  else
    /* Otherwise this must be a multiway branch of some sort.  */
    return FALSE;

  then_bb = then_edge->dest;
  else_bb = else_edge->dest;
  then_succ = then_bb->succ;
  else_succ = else_bb->succ;

  /* The THEN block of an IF-THEN combo must have exactly one predecessor.  */
  if (then_edge->pred_next != NULL_EDGE)
    return FALSE;

  /* The THEN block of an IF-THEN combo must have exactly one successor.  */
  if (then_succ == NULL_EDGE
      || then_succ->succ_next != NULL_EDGE
      || (then_succ->flags & EDGE_COMPLEX))
    return FALSE;

  /* The THEN block may not start with a label, as might happen with an
     unused user label that has had its address taken.  */
  if (GET_CODE (then_bb->head) == CODE_LABEL)
    return FALSE;

  /* If the THEN block's successor is the other edge out of the TEST block,
     then we have an IF-THEN combo without an ELSE.  */
  if (then_succ->dest == else_bb)
    {
      join_bb = else_bb;
      else_bb = NULL_BLOCK;
    }

  /* If the THEN and ELSE block meet in a subsequent block, and the ELSE
     has exactly one predecessor and one successor, and the outgoing edge
     is not complex, then we have an IF-THEN-ELSE combo.  */
  else if (else_succ != NULL_EDGE
	   && then_succ->dest == else_succ->dest
	   && else_edge->pred_next == NULL_EDGE
	   && else_succ->succ_next == NULL_EDGE
	   && ! (else_succ->flags & EDGE_COMPLEX))
    join_bb = else_succ->dest;

  /* Otherwise it is not an IF-THEN or IF-THEN-ELSE combination.  */
  else
    return FALSE;	   

  num_possible_if_blocks++;

  if (rtl_dump_file)
    {
      if (else_bb)
	fprintf (rtl_dump_file,
		 "\nIF-THEN-ELSE block found, start %d, then %d, else %d, join %d\n",
		 test_bb->index, then_bb->index, else_bb->index,
		 join_bb->index);
      else
	fprintf (rtl_dump_file,
		 "\nIF-THEN block found, start %d, then %d, join %d\n",
		 test_bb->index, then_bb->index, join_bb->index);
    }

  /* Make sure IF, THEN, ELSE, and JOIN blocks are adjacent.  */
  /* ??? As an enhancement, move them.  This also relaxes the current
     fallthru constraint on the else block.  All we'd have to do is
     preserve the jump out of the else block to reach the final
     destination.  */

  next_index = test_bb->index;
  if (++next_index != then_bb->index)
    {
      if (rtl_dump_file)
	fputs ("IF and THEN blocks are not adjacent.\n", rtl_dump_file);
      return FALSE;
    }
  if (else_bb && ++next_index != else_bb->index)
    {
      if (rtl_dump_file)
	  fputs ("THEN and ELSE blocks are not adjacent.\n", rtl_dump_file);
	return FALSE;
    }
  if (++next_index != join_bb->index)
    {
      if (rtl_dump_file)
	fputs ("THEN or ELSE block is not adjacent to JOIN block.\n",
	       rtl_dump_file);
      return FALSE;
    }

  /* Do the real work.  */
  return process_if_block (test_bb, then_bb, else_bb, join_bb);
}

/* Attempt to convert an IF-THEN or IF-THEN-ELSE block into
   straight line code.  Return true if successful.  */

static int
process_if_block (test_bb, then_bb, else_bb, join_bb)
     basic_block test_bb;	/* Basic block test is in */
     basic_block then_bb;	/* Basic block for THEN block */
     basic_block else_bb;	/* Basic block for ELSE block */
     basic_block join_bb;	/* Basic block the join label is in */
{
  if (! reload_completed
      && noce_process_if_block (test_bb, then_bb, else_bb, join_bb))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "Conversion succeeded.\n");
      return TRUE;
    }

#ifdef HAVE_conditional_execution
  if (reload_completed
      && cond_exec_process_if_block (test_bb, then_bb, else_bb, join_bb))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "Conversion succeeded.\n");
      return TRUE;
    }
#endif

  return FALSE;
}

/* Merge the blocks and mark for local life update.  */

static void
merge_if_block (test_bb, then_bb, else_bb, join_bb)
     basic_block test_bb;	/* Basic block test is in */
     basic_block then_bb;	/* Basic block for THEN block */
     basic_block else_bb;	/* Basic block for ELSE block */
     basic_block join_bb;	/* Basic block the join label is in */
{
  basic_block combo_bb;

  /* All block merging is done into the lower block numbers.  */

  combo_bb = test_bb;

  /* First merge TEST block into THEN block.  This is a no-brainer since
     the THEN block did not have a code label to begin with.  */

  merge_blocks_nomove (combo_bb, then_bb);
  num_removed_blocks++;

  /* The ELSE block, if it existed, had a label.  That label count
     will almost always be zero, but odd things can happen when labels
     get their addresses taken.  */
  if (else_bb)
    {
      if (LABEL_NUSES (else_bb->head) == 0
	  && ! LABEL_PRESERVE_P (else_bb->head)
	  && ! LABEL_NAME (else_bb->head))
	{
	  /* We can merge the ELSE.  */
	  merge_blocks_nomove (combo_bb, else_bb);
	  num_removed_blocks++;
	}
      else
	{
	  /* We cannot merge the ELSE.  */

	  /* Properly rewire the edge out of the now combined
	     TEST-THEN block to point here.  */
	  remove_edge (combo_bb->succ);
	  if (combo_bb->succ || else_bb->pred)
	    abort ();
	  make_edge (NULL, combo_bb, else_bb, EDGE_FALLTHRU);

	  /* Remove the jump and cruft from the end of the TEST-THEN block.  */
	  tidy_fallthru_edge (combo_bb->succ, combo_bb, else_bb);

	  /* Make sure we update life info properly.  */
	  combo_bb->aux = AUX_UPDATE_LIFE;

	  /* The ELSE is the new combo block.  */
	  combo_bb = else_bb;
	}
    }

  /* The JOIN block had a label.  It may have had quite a number
     of other predecessors too, but probably not.  See if we can
     merge this with the others.  */
  if (LABEL_NUSES (join_bb->head) == 0
      && ! LABEL_PRESERVE_P (join_bb->head)
      && ! LABEL_NAME (join_bb->head))
    {
      /* We can merge the JOIN.  */
      merge_blocks_nomove (combo_bb, join_bb);
      num_removed_blocks++;
    }
  else
    {
      /* We cannot merge the JOIN.  */

      /* The outgoing edge for the current COMBO block should already
	 be correct.  Verify this.  */
      if (combo_bb->succ->succ_next != NULL_EDGE
	  || combo_bb->succ->dest != join_bb)
	abort ();

      /* Remove the jump and cruft from the end of the COMBO block.  */
      tidy_fallthru_edge (combo_bb->succ, combo_bb, join_bb);
    }

  /* Make sure we update life info properly.  */
  combo_bb->aux = AUX_UPDATE_LIFE;

  num_updated_if_blocks++;
}


/* Convert simple if-then and if-then-else statements into the appropriate
   conditional execution instructions.  */

void
if_convert ()
{
  int block_num;

#ifndef HAVE_conditional_execution
  /* Without conditional execution, nothing applies after reload.  */
  if (reload_completed)
    return;
#endif

  num_possible_if_blocks = 0;
  num_updated_if_blocks = 0;
  num_modified_insns = 0;
  num_removed_blocks = 0;

  /* Free up basic_block_for_insn so that we don't have to keep it 
     up to date, either here or in merge_blocks_nomove.  */
  free_basic_block_vars (1);

  /* Zero bb->aux.  */
  for (block_num = 0; block_num < n_basic_blocks; block_num++)
    BASIC_BLOCK (block_num)->aux = NULL;

  /* Go through each of the basic blocks looking for an IF-THEN or
     IF-THEN-ELSE combination, and convert them if we can.  */
  for (block_num = 0; block_num < n_basic_blocks; )
    {
      if (find_if_block (BASIC_BLOCK (block_num)))
	;
      else 
	block_num++;
    }

  if (rtl_dump_file)
    fflush (rtl_dump_file);

  /* Rebuild basic_block_for_insn for update_life_info and for gcse.  */
  compute_bb_for_insn (get_max_uid ());

  /* Rebuild life info for basic blocks that require it.  */
  if (num_removed_blocks
      && ENTRY_BLOCK_PTR->global_live_at_end != NULL)
    {
      sbitmap update_life_blocks = sbitmap_alloc (n_basic_blocks);
      sbitmap_zero (update_life_blocks);

      for (block_num = 0; block_num < n_basic_blocks; block_num++)
	if (BASIC_BLOCK (block_num)->aux == AUX_UPDATE_LIFE)
	  SET_BIT (update_life_blocks, block_num);

      count_or_remove_death_notes (update_life_blocks, 1);
      update_life_info (update_life_blocks, UPDATE_LIFE_LOCAL,
			PROP_DEATH_NOTES);

      sbitmap_free (update_life_blocks);
    }

  /* Write the final stats.  */
  if (rtl_dump_file && num_possible_if_blocks > 0)
    {
      fprintf (rtl_dump_file,
	       "\n%d possible IF-THEN/IF-THEN-ELSE block%s searched.\n",
	       num_possible_if_blocks,
	       (num_possible_if_blocks == 1) ? " was" : "s were");

      fprintf (rtl_dump_file,
	       "%d IF-THEN/IF-THEN-ELSE block%s converted.\n",
	       num_updated_if_blocks,
	       (num_updated_if_blocks == 1) ? " was" : "s were");

      fprintf (rtl_dump_file,
	       "%d basic block%s deleted.\n",
	       num_removed_blocks,
	       (num_removed_blocks == 1) ? " was" : "s were");

      fprintf (rtl_dump_file,
	       "%d insn%s modified.\n\n",
	       num_modified_insns,
	       (num_modified_insns == 1) ? " was" : "s were");
    }

  verify_flow_info ();
}
