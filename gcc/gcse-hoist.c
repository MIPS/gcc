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

static void compute_transpout		PARAMS ((struct hoist_global *));
static void alloc_code_hoist_mem	PARAMS ((int, struct hoist_global *));
static void free_code_hoist_mem		PARAMS ((struct hoist_global *));
static void compute_code_hoist_vbeinout	PARAMS (( struct hoist_global *));
static void compute_code_hoist_data	PARAMS (( struct hoist_global *));
static int hoist_expr_reaches_here_p	PARAMS ((basic_block, int, basic_block,
 					  char *, struct hoist_global *));
static void hoist_code			PARAMS ((struct hoist_global *));

/* Compute transparent outgoing information for each block.

   An expression is transparent to an edge unless it is killed by
   the edge itself.  This can only happen with abnormal control flow,
   when the edge is traversed through a call.  This happens with
   non-local labels and exceptions.

   This would not be necessary if we split the edge.  While this is
   normally impossible for abnormal critical edges, with some effort
   it should be possible with exception handling, since we still have
   control over which handler should be invoked.  But due to increased
   EH table sizes, this may not be worthwhile.  */

static void
compute_transpout (hoist_data)
     struct hoist_global *hoist_data;
{
  basic_block bb;
  unsigned int i;
  struct expr *expr;

  sbitmap_vector_ones (hoist_data->transpout, last_basic_block);

  FOR_EACH_BB (bb)
    {
      /* Note that flow inserted a nop a the end of basic blocks that
	 end in call instructions for reasons other than abnormal
	 control flow.  */
      if (GET_CODE (bb->end) != CALL_INSN)
	continue;

      for (i = 0; i < hoist_data->expr_hash_table.size; i++)
	for (expr = hoist_data->expr_hash_table.table[i]; expr ; expr = expr->next_same_hash)
	  if (GET_CODE (expr->expr) == MEM)
	    {
	      if (GET_CODE (XEXP (expr->expr, 0)) == SYMBOL_REF
		  && CONSTANT_POOL_ADDRESS_P (XEXP (expr->expr, 0)))
		continue;

	      /* ??? Optimally, we would use interprocedural alias
		 analysis to determine if this mem is actually killed
		 by this call.  */
	      RESET_BIT (hoist_data->transpout[bb->index], expr->bitmap_index);
	    }
    }
}

/* Code Hoisting variables and subroutines.  */

/* ??? We could compute post dominators and run this algorithm in
   reverse to perform tail merging, doing so would probably be
   more effective than the tail merging code in jump.c.

   It's unclear if tail merging could be run in parallel with
   code hoisting.  It would be nice.  */

/* Allocate vars used for code hoisting analysis.  */

static void
alloc_code_hoist_mem (n_blocks, hoist_data)
     int n_blocks;
     struct hoist_global *hoist_data;
{
  hoist_data->antloc = sbitmap_vector_alloc (n_blocks, hoist_data->expr_hash_table.n_elems);
  hoist_data->transp = sbitmap_vector_alloc (n_blocks, hoist_data->expr_hash_table.n_elems);
  hoist_data->comp = sbitmap_vector_alloc (n_blocks, hoist_data->expr_hash_table.n_elems);

  hoist_data->vbein = sbitmap_vector_alloc (n_blocks, hoist_data->expr_hash_table.n_elems);
  hoist_data->vbeout = sbitmap_vector_alloc (n_blocks, hoist_data->expr_hash_table.n_elems);
  hoist_data->exprs = sbitmap_vector_alloc (n_blocks, hoist_data->expr_hash_table.n_elems);
  hoist_data->transpout = sbitmap_vector_alloc (n_blocks, hoist_data->expr_hash_table.n_elems);
}

/* Free vars used for code hoisting analysis.  */

static void
free_code_hoist_mem (hoist_data)
     struct hoist_global *hoist_data;
{
  sbitmap_vector_free (hoist_data->antloc);
  sbitmap_vector_free (hoist_data->transp);
  sbitmap_vector_free (hoist_data->comp);

  sbitmap_vector_free (hoist_data->vbein);
  sbitmap_vector_free (hoist_data->vbeout);
  sbitmap_vector_free (hoist_data->exprs);
  sbitmap_vector_free (hoist_data->transpout);

  free_dominance_info (hoist_data->dominators);
}

/* Compute the very busy expressions at entry/exit from each block.

   An expression is very busy if all paths from a given point
   compute the expression.  */

static void
compute_code_hoist_vbeinout (hoist_data)
     struct hoist_global *hoist_data;
{
  int changed, passes;
  basic_block bb;

  sbitmap_vector_zero (hoist_data->vbeout, last_basic_block);
  sbitmap_vector_zero (hoist_data->vbein, last_basic_block);

  passes = 0;
  changed = 1;

  while (changed)
    {
      changed = 0;

      /* We scan the blocks in the reverse order to speed up
	 the convergence.  */
      FOR_EACH_BB_REVERSE (bb)
	{
	  changed |= sbitmap_a_or_b_and_c_cg (hoist_data->vbein[bb->index],
	  				      hoist_data->antloc[bb->index],
					      hoist_data->vbeout[bb->index],
					      hoist_data->transp[bb->index]);
	  if (bb->next_bb != EXIT_BLOCK_PTR)
	    sbitmap_intersection_of_succs (hoist_data->vbeout[bb->index],
	    				   hoist_data->vbein, bb->index);
	}

      passes++;
    }

  if (gcse_file)
    fprintf (gcse_file, "hoisting vbeinout computation: %d passes\n", passes);
}

/* Top level routine to do the dataflow analysis needed by code hoisting.  */

static void
compute_code_hoist_data (hoist_data)
     struct hoist_global *hoist_data;
{
  compute_local_properties (hoist_data->transp, hoist_data->comp, hoist_data->antloc, &hoist_data->expr_hash_table);
  compute_transpout (hoist_data);
  compute_code_hoist_vbeinout (hoist_data);
  hoist_data->dominators = calculate_dominance_info (CDI_DOMINATORS);
  if (gcse_file)
    fprintf (gcse_file, "\n");
}

/* Determine if the expression identified by EXPR_INDEX would
   reach BB unimpared if it was placed at the end of EXPR_BB.

   It's unclear exactly what Muchnick meant by "unimpared".  It seems
   to me that the expression must either be computed or transparent in
   *every* block in the path(s) from EXPR_BB to BB.  Any other definition
   would allow the expression to be hoisted out of loops, even if
   the expression wasn't a loop invariant.

   Contrast this to reachability for PRE where an expression is
   considered reachable if *any* path reaches instead of *all*
   paths.  */

static int
hoist_expr_reaches_here_p (expr_bb, expr_index, bb, visited, hoist_data)
     basic_block expr_bb;
     int expr_index;
     basic_block bb;
     char *visited;
     struct hoist_global *hoist_data;
{
  edge pred;
  int visited_allocated_locally = 0;


  if (visited == NULL)
    {
      visited_allocated_locally = 1;
      visited = xcalloc (last_basic_block, 1);
    }

  for (pred = bb->pred; pred != NULL; pred = pred->pred_next)
    {
      basic_block pred_bb = pred->src;

      if (pred->src == ENTRY_BLOCK_PTR)
	break;
      else if (pred_bb == expr_bb)
	continue;
      else if (visited[pred_bb->index])
	continue;

      /* Does this predecessor generate this expression?  */
      else if (TEST_BIT (hoist_data->comp[pred_bb->index], expr_index))
	break;
      else if (! TEST_BIT (hoist_data->transp[pred_bb->index], expr_index))
	break;

      /* Not killed.  */
      else
	{
	  visited[pred_bb->index] = 1;
	  if (! hoist_expr_reaches_here_p (expr_bb, expr_index,
					   pred_bb, visited, hoist_data))
	    break;
	}
    }
  if (visited_allocated_locally)
    free (visited);

  return (pred == NULL);
}

/* Actually perform code hoisting.  */

static void
hoist_code (hoist_data)
     struct hoist_global *hoist_data;
{
  basic_block bb, dominated;
  basic_block *domby;
  unsigned int domby_len;
  unsigned int i,j;
  struct expr **index_map;
  struct expr *expr;

  sbitmap_vector_zero (hoist_data->exprs, last_basic_block);

  /* Compute a mapping from expression number (`bitmap_index') to
     hash table entry.  */

  index_map = (struct expr **) xcalloc (hoist_data->expr_hash_table.n_elems, sizeof (struct expr *));
  for (i = 0; i < hoist_data->expr_hash_table.size; i++)
    for (expr = hoist_data->expr_hash_table.table[i]; expr != NULL; expr = expr->next_same_hash)
      index_map[expr->bitmap_index] = expr;

  /* Walk over each basic block looking for potentially hoistable
     expressions, nothing gets hoisted from the entry block.  */
  FOR_EACH_BB (bb)
    {
      int found = 0;
      int insn_inserted_p;

      domby_len = get_dominated_by (hoist_data->dominators, bb, &domby);
      /* Examine each expression that is very busy at the exit of this
	 block.  These are the potentially hoistable expressions.  */
      for (i = 0; i < hoist_data->vbeout[bb->index]->n_bits; i++)
	{
	  int hoistable = 0;

	  if (TEST_BIT (hoist_data->vbeout[bb->index], i) &&
	      TEST_BIT (hoist_data->transpout[bb->index], i))
	    {
	      /* We've found a potentially hoistable expression, now
		 we look at every block BB dominates to see if it
		 computes the expression.  */
	      for (j = 0; j < domby_len; j++)
		{
		  dominated = domby[j];
		  /* Ignore self dominance.  */
		  if (bb == dominated)
		    continue;
		  /* We've found a dominated block, now see if it computes
		     the busy expression and whether or not moving that
		     expression to the "beginning" of that block is safe.  */
		  if (!TEST_BIT (hoist_data->antloc[dominated->index], i))
		    continue;

		  /* Note if the expression would reach the dominated block
		     unimpared if it was placed at the end of BB.

		     Keep track of how many times this expression is hoistable
		     from a dominated block into BB.  */
		  if (hoist_expr_reaches_here_p (bb, i, dominated, NULL, hoist_data))
		    hoistable++;
		}

	      /* If we found more than one hoistable occurrence of this
		 expression, then note it in the bitmap of expressions to
		 hoist.  It makes no sense to hoist things which are computed
		 in only one BB, and doing so tends to pessimize register
		 allocation.  One could increase this value to try harder
		 to avoid any possible code expansion due to register
		 allocation issues; however experiments have shown that
		 the vast majority of hoistable expressions are only movable
		 from two successors, so raising this threshhold is likely
		 to nullify any benefit we get from code hoisting.  */
	      if (hoistable > 1)
		{
		  SET_BIT (hoist_data->exprs[bb->index], i);
		  found = 1;
		}
	    }
	}
      /* If we found nothing to hoist, then quit now.  */
      if (! found)
        {
  	  free (domby);
	continue;
	}

      /* Loop over all the hoistable expressions.  */
      for (i = 0; i < hoist_data->exprs[bb->index]->n_bits; i++)
	{
	  /* We want to insert the expression into BB only once, so
	     note when we've inserted it.  */
	  insn_inserted_p = 0;

	  /* These tests should be the same as the tests above.  */
	  if (TEST_BIT (hoist_data->vbeout[bb->index], i))
	    {
	      /* We've found a potentially hoistable expression, now
		 we look at every block BB dominates to see if it
		 computes the expression.  */
	      for (j = 0; j < domby_len; j++)
		{
		  dominated = domby[j];
		  /* Ignore self dominance.  */
		  if (bb == dominated)
		    continue;

		  /* We've found a dominated block, now see if it computes
		     the busy expression and whether or not moving that
		     expression to the "beginning" of that block is safe.  */
		  if (!TEST_BIT (hoist_data->antloc[dominated->index], i))
		    continue;

		  /* The expression is computed in the dominated block and
		     it would be safe to compute it at the start of the
		     dominated block.  Now we have to determine if the
		     expression would reach the dominated block if it was
		     placed at the end of BB.  */
		  if (hoist_expr_reaches_here_p (bb, i, dominated, NULL, hoist_data))
		    {
		      struct expr *expr = index_map[i];
		      struct occr *occr = expr->antic_occr;
		      rtx insn;
		      rtx set;

		      /* Find the right occurrence of this expression.  */
		      while (BLOCK_FOR_INSN (occr->insn) != dominated && occr)
			occr = occr->next;

		      /* Should never happen.  */
		      if (!occr)
			abort ();

		      insn = occr->insn;

		      set = single_set (insn);
		      if (! set)
			abort ();

		      /* Create a pseudo-reg to store the result of reaching
			 expressions into.  Get the mode for the new pseudo
			 from the mode of the original destination pseudo.  */
		      if (expr->reaching_reg == NULL)
			expr->reaching_reg
			  = gen_reg_rtx (GET_MODE (SET_DEST (set)));

		      gcse_emit_move_after (expr->reaching_reg, SET_DEST (set), insn);
		      delete_insn (insn);
		      occr->deleted_p = 1;
		      if (!insn_inserted_p)
			{
			  insert_insn_end_bb (index_map[i], bb, 0,
				hoist_data->antloc, hoist_data->transp);
			  insn_inserted_p = 1;
			}
		    }
		}
	    }
	}
      free (domby);
    }

  free (index_map);
}

/* Top level routine to perform one code hoisting (aka unification) pass

   Return non-zero if a change was made.  */

int
one_code_hoisting_pass ()
{
  int changed = 0;
  struct hoist_global hoist_data;

  alloc_hash_table (max_cuid, &hoist_data.expr_hash_table, 0);
  compute_hash_table (&hoist_data.expr_hash_table);
  if (gcse_file)
    dump_hash_table (gcse_file, "Code Hosting Expressions", &hoist_data.expr_hash_table);

  if (hoist_data.expr_hash_table.n_elems > 0)
    {
      alloc_code_hoist_mem (last_basic_block, &hoist_data);
      compute_code_hoist_data (&hoist_data);
      hoist_code (&hoist_data);
      free_code_hoist_mem (&hoist_data);
    }

  free_hash_table (&hoist_data.expr_hash_table);

  return changed;
}
