/* Global common subexpression elimination/Partial redundancy elimination
   and global constant/copy propagation for GNU compiler.
   Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002
   Free Software Foundation, Inc.

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
#include "toplev.h"

#include "rtl.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "real.h"
#include "insn-config.h"
#include "recog.h"
#include "basic-block.h"
#include "output.h"
#include "function.h"
#include "expr.h"
#include "except.h"
#include "ggc.h"
#include "params.h"

#include "obstack.h"

#include "gcse-globals.h"
#include "gcse-utils.h"

static void alloc_rd_mem		PARAMS ((int, int, struct rd *));
static void free_rd_mem			PARAMS ((struct rd *));
static void handle_rd_kill_set		PARAMS ((rtx, int, basic_block, struct rd *));
static void compute_kill_rd		PARAMS ((struct rd *));
static void compute_rd			PARAMS ((struct rd *));
static void alloc_avail_expr_mem	PARAMS ((int, int, struct ae *));
static void free_avail_expr_mem		PARAMS ((struct ae *));
static void compute_ae_gen		PARAMS ((struct ae *, struct hash_table *));
static void compute_ae_kill		PARAMS ((struct ae *, struct hash_table *));
static int expr_reaches_here_p_work	PARAMS ((struct occr *, struct expr *,
					  basic_block, int, char *, struct ae *));
static int expr_reaches_here_p		PARAMS ((struct occr *, struct expr *,
					  basic_block, int, struct ae *));
static rtx computing_insn		PARAMS ((struct expr *, rtx, struct ae *));
static int def_reaches_here_p		PARAMS ((rtx, rtx, struct rd *));
static int can_disregard_other_sets	PARAMS ((struct reg_set **, rtx, int,
					  struct rd *));
static int handle_avail_expr		PARAMS ((rtx, struct expr *,
					  struct ae *, struct rd *));
static int classic_gcse			PARAMS ((struct classic_global *));

/* Classic GCSE reaching definition support.  */

/* Allocate reaching def variables.  */

static void
alloc_rd_mem (n_blocks, n_insns, rd)
     int n_blocks, n_insns;
     struct rd *rd;
{
  rd->kill = (sbitmap *) sbitmap_vector_alloc (n_blocks, n_insns);
  sbitmap_vector_zero (rd->kill, n_blocks);

  rd->gen = (sbitmap *) sbitmap_vector_alloc (n_blocks, n_insns);
  sbitmap_vector_zero (rd->gen, n_blocks);

  rd->reaching_defs = (sbitmap *) sbitmap_vector_alloc (n_blocks, n_insns);
  sbitmap_vector_zero (rd->reaching_defs, n_blocks);

  rd->out = (sbitmap *) sbitmap_vector_alloc (n_blocks, n_insns);
  sbitmap_vector_zero (rd->out, n_blocks);
}

/* Free reaching def variables.  */

static void
free_rd_mem (rd)
     struct rd *rd;
{
  sbitmap_vector_free (rd->kill);
  sbitmap_vector_free (rd->gen);
  sbitmap_vector_free (rd->reaching_defs);
  sbitmap_vector_free (rd->out);
}

/* Add INSN to the kills of BB.  REGNO, set in BB, is killed by INSN.  */

static void
handle_rd_kill_set (insn, regno, bb, rd)
     rtx insn;
     int regno;
     basic_block bb;
     struct rd *rd;
{
  struct reg_set *this_reg;

  for (this_reg = reg_set_table.table[regno]; this_reg; this_reg = this_reg ->next)
    if (BLOCK_NUM (this_reg->insn) != BLOCK_NUM (insn))
      SET_BIT (rd->kill[bb->index], INSN_CUID (this_reg->insn));
}

/* Compute the set of kill's for reaching definitions.  */

static void
compute_kill_rd (rd)
     struct rd *rd;
{
  int cuid;
  unsigned int regno;
  int i;
  basic_block bb;

  /* For each block
       For each set bit in `gen' of the block (i.e each insn which
	   generates a definition in the block)
	 Call the reg set by the insn corresponding to that bit regx
	 Look at the linked list starting at reg_set_table[regx]
	 For each setting of regx in the linked list, which is not in
	     this block
	   Set the bit in `kill' corresponding to that insn.  */
  FOR_EACH_BB (bb)
    for (cuid = 0; cuid < max_cuid; cuid++)
      if (TEST_BIT (rd->gen[bb->index], cuid))
	{
	  rtx insn = CUID_INSN (cuid);
	  rtx pat = PATTERN (insn);

	  if (GET_CODE (insn) == CALL_INSN)
	    {
	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (TEST_HARD_REG_BIT (regs_invalidated_by_call, regno))
		  handle_rd_kill_set (insn, regno, bb, rd);
	    }

	  if (GET_CODE (pat) == PARALLEL)
	    {
	      for (i = XVECLEN (pat, 0) - 1; i >= 0; i--)
		{
		  enum rtx_code code = GET_CODE (XVECEXP (pat, 0, i));

		  if ((code == SET || code == CLOBBER)
		      && GET_CODE (XEXP (XVECEXP (pat, 0, i), 0)) == REG)
		    handle_rd_kill_set (insn,
					REGNO (XEXP (XVECEXP (pat, 0, i), 0)),
					bb, rd);
		}
	    }
	  else if (GET_CODE (pat) == SET && GET_CODE (SET_DEST (pat)) == REG)
	    /* Each setting of this register outside of this block
	       must be marked in the set of kills in this block.  */
	    handle_rd_kill_set (insn, REGNO (SET_DEST (pat)), bb, rd);
	}
}

/* Compute the reaching definitions as in
   Compilers Principles, Techniques, and Tools. Aho, Sethi, Ullman,
   Chapter 10.  It is the same algorithm as used for computing available
   expressions but applied to the gens and kills of reaching definitions.  */

static void
compute_rd (rd)
     struct rd *rd;
{
  int changed, passes;
  basic_block bb;

  FOR_EACH_BB (bb)
    sbitmap_copy (rd->out[bb->index] /*dst*/, rd->gen[bb->index] /*src*/);

  passes = 0;
  changed = 1;
  while (changed)
    {
      changed = 0;
      FOR_EACH_BB (bb)
	{
	  sbitmap_union_of_preds (rd->reaching_defs[bb->index], rd->out, bb->index);
	  changed |= sbitmap_union_of_diff_cg (rd->out[bb->index], rd->gen[bb->index],
					       rd->reaching_defs[bb->index], rd->kill[bb->index]);
	}
      passes++;
    }

  if (gcse_file)
    fprintf (gcse_file, "reaching def computation: %d passes\n", passes);
}

/* Classic GCSE available expression support.  */

/* Allocate memory for available expression computation.  */

static void
alloc_avail_expr_mem (n_blocks, n_exprs, ae)
     int n_blocks, n_exprs;
     struct ae *ae;
{
  ae->kill = (sbitmap *) sbitmap_vector_alloc (n_blocks, n_exprs);
  sbitmap_vector_zero (ae->kill, n_blocks);

  ae->gen = (sbitmap *) sbitmap_vector_alloc (n_blocks, n_exprs);
  sbitmap_vector_zero (ae->gen, n_blocks);

  ae->in = (sbitmap *) sbitmap_vector_alloc (n_blocks, n_exprs);
  sbitmap_vector_zero (ae->in, n_blocks);

  ae->out = (sbitmap *) sbitmap_vector_alloc (n_blocks, n_exprs);
  sbitmap_vector_zero (ae->out, n_blocks);
}

static void
free_avail_expr_mem (ae)
     struct ae *ae;
{
  sbitmap_vector_free (ae->kill);
  sbitmap_vector_free (ae->gen);
  sbitmap_vector_free (ae->in);
  sbitmap_vector_free (ae->out);
}

/* Compute the set of available expressions generated in each basic block.  */

static void
compute_ae_gen (ae, expr_hash_table)
     struct ae *ae;
     struct hash_table *expr_hash_table;
{
  unsigned int i;
  struct expr *expr;
  struct occr *occr;

  /* For each recorded occurrence of each expression, set ae->gen[bb][expr].
     This is all we have to do because an expression is not recorded if it
     is not available, and the only expressions we want to work with are the
     ones that are recorded.  */
  for (i = 0; i < expr_hash_table->size; i++)
    for (expr = expr_hash_table->table[i]; expr != 0; expr = expr->next_same_hash)
      for (occr = expr->avail_occr; occr != 0; occr = occr->next)
	SET_BIT (ae->gen[BLOCK_NUM (occr->insn)], expr->bitmap_index);
}

/* Compute the set of available expressions killed in each basic block.  */

static void
compute_ae_kill (ae, expr_hash_table)
     struct ae *ae;
     struct hash_table *expr_hash_table;
{
  basic_block bb;
  unsigned int i;
  struct expr *expr;

  FOR_EACH_BB (bb)
    for (i = 0; i < expr_hash_table->size; i++)
      for (expr = expr_hash_table->table[i]; expr; expr = expr->next_same_hash)
	{
	  /* Skip EXPR if generated in this block.  */
	  if (TEST_BIT (ae->gen[bb->index], expr->bitmap_index))
	    continue;

	  if (expr_killed_p (expr->expr, bb))
	    SET_BIT (ae->kill[bb->index], expr->bitmap_index);
	}
}

/* Actually perform the Classic GCSE optimizations.  */

/* Return non-zero if occurrence OCCR of expression EXPR reaches block BB.

   CHECK_SELF_LOOP is non-zero if we should consider a block reaching itself
   as a positive reach.  We want to do this when there are two computations
   of the expression in the block.

   VISITED is a pointer to a working buffer for tracking which BB's have
   been visited.  It is NULL for the top-level call.

   We treat reaching expressions that go through blocks containing the same
   reaching expression as "not reaching".  E.g. if EXPR is generated in blocks
   2 and 3, INSN is in block 4, and 2->3->4, we treat the expression in block
   2 as not reaching.  The intent is to improve the probability of finding
   only one reaching expression and to reduce register lifetimes by picking
   the closest such expression.  */

static int
expr_reaches_here_p_work (occr, expr, bb, check_self_loop, visited, ae)
     struct occr *occr;
     struct expr *expr;
     basic_block bb;
     int check_self_loop;
     char *visited;
     struct ae *ae;
{
  edge pred;

  for (pred = bb->pred; pred != NULL; pred = pred->pred_next)
    {
      basic_block pred_bb = pred->src;

      if (visited[pred_bb->index])
	/* This predecessor has already been visited. Nothing to do.  */
	  ;
      else if (pred_bb == bb)
	{
	  /* BB loops on itself.  */
	  if (check_self_loop
	      && TEST_BIT (ae->gen[pred_bb->index], expr->bitmap_index)
	      && BLOCK_NUM (occr->insn) == pred_bb->index)
	    return 1;

	  visited[pred_bb->index] = 1;
	}

      /* Ignore this predecessor if it kills the expression.  */
      else if (TEST_BIT (ae->kill[pred_bb->index], expr->bitmap_index))
	visited[pred_bb->index] = 1;

      /* Does this predecessor generate this expression?  */
      else if (TEST_BIT (ae->gen[pred_bb->index], expr->bitmap_index))
	{
	  /* Is this the occurrence we're looking for?
	     Note that there's only one generating occurrence per block
	     so we just need to check the block number.  */
	  if (BLOCK_NUM (occr->insn) == pred_bb->index)
	    return 1;

	  visited[pred_bb->index] = 1;
	}

      /* Neither gen nor kill.  */
      else
	{
	  visited[pred_bb->index] = 1;
	  if (expr_reaches_here_p_work (occr, expr, pred_bb, check_self_loop,
	      visited, ae))

	    return 1;
	}
    }

  /* All paths have been checked.  */
  return 0;
}

/* This wrapper for expr_reaches_here_p_work() is to ensure that any
   memory allocated for that function is returned.  */

static int
expr_reaches_here_p (occr, expr, bb, check_self_loop, ae)
     struct occr *occr;
     struct expr *expr;
     basic_block bb;
     int check_self_loop;
     struct ae *ae;
{
  int rval;
  char *visited = (char *) xcalloc (last_basic_block, 1);

  rval = expr_reaches_here_p_work (occr, expr, bb, check_self_loop, visited, ae);

  free (visited);
  return rval;
}

/* Return the instruction that computes EXPR that reaches INSN's basic block.
   If there is more than one such instruction, return NULL.

   Called only by handle_avail_expr.  */

static rtx
computing_insn (expr, insn, ae)
     struct expr *expr;
     rtx insn;
     struct ae *ae;
{
  basic_block bb = BLOCK_FOR_INSN (insn);

  if (expr->avail_occr->next == NULL)
    {
      if (BLOCK_FOR_INSN (expr->avail_occr->insn) == bb)
	/* The available expression is actually itself
	   (i.e. a loop in the flow graph) so do nothing.  */
	return NULL;

      /* (FIXME) Case that we found a pattern that was created by
	 a substitution that took place.  */
      return expr->avail_occr->insn;
    }
  else
    {
      /* Pattern is computed more than once.
	 Search backwards from this insn to see how many of these
	 computations actually reach this insn.  */
      struct occr *occr;
      rtx insn_computes_expr = NULL;
      int can_reach = 0;

      for (occr = expr->avail_occr; occr != NULL; occr = occr->next)
	{
	  if (BLOCK_FOR_INSN (occr->insn) == bb)
	    {
	      /* The expression is generated in this block.
		 The only time we care about this is when the expression
		 is generated later in the block [and thus there's a loop].
		 We let the normal cse pass handle the other cases.  */
	      if (INSN_CUID (insn) < INSN_CUID (occr->insn)
		  && expr_reaches_here_p (occr, expr, bb, 1, ae))
		{
		  can_reach++;
		  if (can_reach > 1)
		    return NULL;

		  insn_computes_expr = occr->insn;
		}
	    }
	  else if (expr_reaches_here_p (occr, expr, bb, 0, ae))
	    {
	      can_reach++;
	      if (can_reach > 1)
		return NULL;

	      insn_computes_expr = occr->insn;
	    }
	}

      if (insn_computes_expr == NULL)
	abort ();

      return insn_computes_expr;
    }
}

/* Return non-zero if the definition in DEF_INSN can reach INSN.
   Only called by can_disregard_other_sets.  */

static int
def_reaches_here_p (insn, def_insn, rd)
     rtx insn, def_insn;
     struct rd *rd;
{
  rtx reg;

  if (TEST_BIT (rd->reaching_defs[BLOCK_NUM (insn)], INSN_CUID (def_insn)))
    return 1;

  if (BLOCK_NUM (insn) == BLOCK_NUM (def_insn))
    {
      if (INSN_CUID (def_insn) < INSN_CUID (insn))
	{
	  if (GET_CODE (PATTERN (def_insn)) == PARALLEL)
	    return 1;
	  else if (GET_CODE (PATTERN (def_insn)) == CLOBBER)
	    reg = XEXP (PATTERN (def_insn), 0);
	  else if (GET_CODE (PATTERN (def_insn)) == SET)
	    reg = SET_DEST (PATTERN (def_insn));
	  else
	    abort ();

	  return ! reg_set_between_p (reg, NEXT_INSN (def_insn), insn);
	}
      else
	return 0;
    }

  return 0;
}

/* Return non-zero if *ADDR_THIS_REG can only have one value at INSN.  The
   value returned is the number of definitions that reach INSN.  Returning a
   value of zero means that [maybe] more than one definition reaches INSN and
   the caller can't perform whatever optimization it is trying.  i.e. it is
   always safe to return zero.  */

static int
can_disregard_other_sets (addr_this_reg, insn, for_combine, rd)
     struct reg_set **addr_this_reg;
     rtx insn;
     int for_combine;
     struct rd *rd;
{
  int number_of_reaching_defs = 0;
  struct reg_set *this_reg;

  for (this_reg = *addr_this_reg; this_reg != 0; this_reg = this_reg->next)
    if (def_reaches_here_p (insn, this_reg->insn, rd))
      {
	number_of_reaching_defs++;
	/* Ignore parallels for now.  */
	if (GET_CODE (PATTERN (this_reg->insn)) == PARALLEL)
	  return 0;

	if (!for_combine
	    && (GET_CODE (PATTERN (this_reg->insn)) == CLOBBER
		|| ! rtx_equal_p (SET_SRC (PATTERN (this_reg->insn)),
				  SET_SRC (PATTERN (insn)))))
	  /* A setting of the reg to a different value reaches INSN.  */
	  return 0;

	if (number_of_reaching_defs > 1)
	  {
	    /* If in this setting the value the register is being set to is
	       equal to the previous value the register was set to and this
	       setting reaches the insn we are trying to do the substitution
	       on then we are ok.  */
	    if (GET_CODE (PATTERN (this_reg->insn)) == CLOBBER)
	      return 0;
	    else if (! rtx_equal_p (SET_SRC (PATTERN (this_reg->insn)),
				    SET_SRC (PATTERN (insn))))
	      return 0;
	  }

	*addr_this_reg = this_reg;
      }

  return number_of_reaching_defs;
}

/* Expression computed by insn is available and the substitution is legal,
   so try to perform the substitution.

   The result is non-zero if any changes were made.  */

static int
handle_avail_expr (insn, expr, ae, rd)
     rtx insn;
     struct expr *expr;
     struct ae *ae;
     struct rd *rd;
{
  rtx pat, insn_computes_expr, expr_set;
  rtx to;
  struct reg_set *this_reg;
  int found_setting, use_src;
  int changed = 0;

  /* We only handle the case where one computation of the expression
     reaches this instruction.  */
  insn_computes_expr = computing_insn (expr, insn, ae);
  if (insn_computes_expr == NULL)
    return 0;
  expr_set = single_set (insn_computes_expr);
  if (!expr_set)
    abort ();

  found_setting = 0;
  use_src = 0;

  /* At this point we know only one computation of EXPR outside of this
     block reaches this insn.  Now try to find a register that the
     expression is computed into.  */
  if (GET_CODE (SET_SRC (expr_set)) == REG)
    {
      /* This is the case when the available expression that reaches
	 here has already been handled as an available expression.  */
      unsigned int regnum_for_replacing
	= REGNO (SET_SRC (expr_set));

      /* If the register was created by GCSE we can't use `reg_set_table',
	 however we know it's set only once.  */
      if (regnum_for_replacing >= max_gcse_regno
	  /* If the register the expression is computed into is set only once,
	     or only one set reaches this insn, we can use it.  */
	  || (((this_reg = reg_set_table.table[regnum_for_replacing]),
	       this_reg->next == NULL)
	      || can_disregard_other_sets (&this_reg, insn, 0, rd)))
	{
	  use_src = 1;
	  found_setting = 1;
	}
    }

  if (!found_setting)
    {
      unsigned int regnum_for_replacing
	= REGNO (SET_DEST (expr_set));

      /* This shouldn't happen.  */
      if (regnum_for_replacing >= max_gcse_regno)
	abort ();

      this_reg = reg_set_table.table[regnum_for_replacing];

      /* If the register the expression is computed into is set only once,
	 or only one set reaches this insn, use it.  */
      if (this_reg->next == NULL
	  || can_disregard_other_sets (&this_reg, insn, 0, rd))
	found_setting = 1;
    }

  if (found_setting)
    {
      pat = PATTERN (insn);
      if (use_src)
	to = SET_SRC (expr_set);
      else
	to = SET_DEST (expr_set);
      changed = validate_change (insn, &SET_SRC (pat), to, 0);

      /* We should be able to ignore the return code from validate_change but
	 to play it safe we check.  */
      if (changed)
	{
	  gcse_subst_count++;
	  if (gcse_file != NULL)
	    {
	      fprintf (gcse_file, "GCSE: Replacing the source in insn %d with",
		       INSN_UID (insn));
	      fprintf (gcse_file, " reg %d %s insn %d\n",
		       REGNO (to), use_src ? "from" : "set in",
		       INSN_UID (insn_computes_expr));
	    }
	}
    }

  /* The register that the expr is computed into is set more than once.  */
  else if (1 /*expensive_op(this_pattrn->op) && do_expensive_gcse)*/)
    {
      /* Insert an insn after insnx that copies the reg set in insnx
	 into a new pseudo register call this new register REGN.
	 From insnb until end of basic block or until REGB is set
	 replace all uses of REGB with REGN.  */
      rtx new_insn;

      to = gen_reg_rtx (GET_MODE (SET_DEST (expr_set)));

      /* Generate the new insn.  */
      /* ??? If the change fails, we return 0, even though we created
	 an insn.  I think this is ok.  */
      new_insn
	= emit_insn_after (gen_rtx_SET (VOIDmode, to,
					SET_DEST (expr_set)),
			   insn_computes_expr);

      /* Keep register set table up to date.  */
      record_one_set (REGNO (to), new_insn);

      gcse_create_count++;
      if (gcse_file != NULL)
	{
	  fprintf (gcse_file, "GCSE: Creating insn %d to copy value of reg %d",
		   INSN_UID (NEXT_INSN (insn_computes_expr)),
		   REGNO (SET_SRC (PATTERN (NEXT_INSN (insn_computes_expr)))));
	  fprintf (gcse_file, ", computed in insn %d,\n",
		   INSN_UID (insn_computes_expr));
	  fprintf (gcse_file, "      into newly allocated reg %d\n",
		   REGNO (to));
	}

      pat = PATTERN (insn);

      /* Do register replacement for INSN.  */
      changed = validate_change (insn, &SET_SRC (pat),
				 SET_DEST (PATTERN
					   (NEXT_INSN (insn_computes_expr))),
				 0);

      /* We should be able to ignore the return code from validate_change but
	 to play it safe we check.  */
      if (changed)
	{
	  gcse_subst_count++;
	  if (gcse_file != NULL)
	    {
	      fprintf (gcse_file,
		       "GCSE: Replacing the source in insn %d with reg %d ",
		       INSN_UID (insn),
		       REGNO (SET_DEST (PATTERN (NEXT_INSN
						 (insn_computes_expr)))));
	      fprintf (gcse_file, "set in insn %d\n",
		       INSN_UID (insn_computes_expr));
	    }
	}
    }

  return changed;
}

/* Perform classic GCSE.  This is called by one_classic_gcse_pass after all
   the dataflow analysis has been done.

   The result is non-zero if a change was made.  */

static int
classic_gcse (classic_data)
     struct classic_global *classic_data;
{
  int changed;
  rtx insn;
  basic_block bb;

  /* Note we start at block 1.  */

  if (ENTRY_BLOCK_PTR->next_bb == EXIT_BLOCK_PTR)
    return 0;

  changed = 0;
  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR->next_bb->next_bb, EXIT_BLOCK_PTR, next_bb)
    {
      /* Reset tables used to keep track of what's still valid [since the
	 start of the block].  */
      reset_opr_set_tables ();

      for (insn = bb->head;
	   insn != NULL && insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  /* Is insn of form (set (pseudo-reg) ...)?  */
	  if (GET_CODE (insn) == INSN
	      && GET_CODE (PATTERN (insn)) == SET
	      && GET_CODE (SET_DEST (PATTERN (insn))) == REG
	      && REGNO (SET_DEST (PATTERN (insn))) >= FIRST_PSEUDO_REGISTER)
	    {
	      rtx pat = PATTERN (insn);
	      rtx src = SET_SRC (pat);
	      struct expr *expr;

	      if (want_to_gcse_p (src)
		  /* Is the expression recorded?  */
		  && ((expr = lookup_expr (src, &classic_data->expr_hash_table)) != NULL)
		  /* Is the expression available [at the start of the
		     block]?  */
		  && TEST_BIT (classic_data->ae.in[bb->index], expr->bitmap_index)
		  /* Are the operands unchanged since the start of the
		     block?  */
		  && oprs_not_set_p (src, insn))
		changed |= handle_avail_expr (insn, expr,
				&classic_data->ae, &classic_data->rd);
	    }

	  /* Keep track of everything modified by this insn.  */
	  /* ??? Need to be careful w.r.t. mods done to INSN.  */
	  if (INSN_P (insn))
	    mark_oprs_set (insn);
	}
    }

  return changed;
}

/* Top level routine to perform one classic GCSE pass.

   Return non-zero if a change was made.  */

int
one_classic_gcse_pass (pass)
     int pass;
{
  int changed = 0;
  struct classic_global classic_data;

  gcse_subst_count = 0;
  gcse_create_count = 0;

  alloc_hash_table (max_cuid, &classic_data.expr_hash_table, 0);
  alloc_rd_mem (last_basic_block, max_cuid, &classic_data.rd);
  compute_hash_table (&classic_data.expr_hash_table);
  if (gcse_file)
    dump_hash_table (gcse_file, "Expression", &classic_data.expr_hash_table);

  if (classic_data.expr_hash_table.n_elems > 0)
    {
      compute_kill_rd (&classic_data.rd);
      compute_rd (&classic_data.rd);
      alloc_avail_expr_mem (last_basic_block, classic_data.expr_hash_table.n_elems, &classic_data.ae);
      compute_ae_gen (&classic_data.ae, &classic_data.expr_hash_table);
      compute_ae_kill (&classic_data.ae, &classic_data.expr_hash_table);
      compute_available (classic_data.ae.gen, classic_data.ae.kill, classic_data.ae.out, classic_data.ae.in);
      changed = classic_gcse (&classic_data);
      free_avail_expr_mem (&classic_data.ae);
    }

  free_rd_mem (&classic_data.rd);
  free_hash_table (&classic_data.expr_hash_table);

  if (gcse_file)
    {
      fprintf (gcse_file, "\n");
      fprintf (gcse_file, "GCSE of %s, pass %d: %d bytes needed, %d substs,",
	       current_function_name, pass, bytes_used, gcse_subst_count);
      fprintf (gcse_file, "%d insns created\n", gcse_create_count);
    }

  return changed;
}
