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
#include "coretypes.h"
#include "tm.h"
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

static void alloc_pre_mem		PARAMS ((int, struct pre_global *));
static void free_pre_mem		PARAMS ((struct pre_global *));
static void compute_pre_data		PARAMS ((struct pre_global *));
static int pre_expr_reaches_here_p_work PARAMS ((basic_block, struct expr *,
					  basic_block, char *, struct pre_global *));
static int pre_expr_reaches_here_p 	PARAMS ((basic_block, struct expr *,
 					  basic_block, struct pre_global *));
static void pre_insert_copy_insn	PARAMS ((struct expr *, rtx));
static void pre_insert_copies		PARAMS ((struct pre_global *));
static int pre_delete			PARAMS ((struct pre_global *));
static int pre_edge_insert		PARAMS ((struct expr **, struct pre_global *));
static int simple_mem			PARAMS ((rtx));
static void invalidate_any_buried_refs	PARAMS ((rtx));
static void compute_ld_motion_mems	PARAMS ((void));
static void trim_ld_motion_mems		PARAMS ((struct pre_global *));
static void update_ld_motion_stores	PARAMS ((struct expr *));
static int pre_gcse			PARAMS ((struct pre_global *));

/* Compute PRE+LCM working variables.  */

/* Allocate vars used for PRE analysis.  */

static void
alloc_pre_mem (n_blocks, pre_data)
     int n_blocks;
     struct pre_global *pre_data;
{
  pre_data->transp = sbitmap_vector_alloc (n_blocks, pre_data->expr_hash_table.n_elems);
  pre_data->comp = sbitmap_vector_alloc (n_blocks, pre_data->expr_hash_table.n_elems);
  pre_data->antloc = sbitmap_vector_alloc (n_blocks, pre_data->expr_hash_table.n_elems);

  pre_data->insert_map = NULL;
  pre_data->delete_map = NULL;
  pre_data->ae.in = NULL;
  pre_data->ae.out = NULL;
  pre_data->ae.kill = sbitmap_vector_alloc (n_blocks, pre_data->expr_hash_table.n_elems);

  /* pre_insert_map and pre_delete_map are allocated later.  */
}

/* Free vars used for PRE analysis.  */

static void
free_pre_mem (pre_data)
     struct pre_global *pre_data;
{
  sbitmap_vector_free (pre_data->transp);
  sbitmap_vector_free (pre_data->comp);

  /* ANTLOC and AE_KILL are freed just after pre_lcm finishes.  */

  if (pre_data->insert_map)
    sbitmap_vector_free (pre_data->insert_map);
  if (pre_data->delete_map)
    sbitmap_vector_free (pre_data->delete_map);
  if (pre_data->ae.in)
    sbitmap_vector_free (pre_data->ae.in);
  if (pre_data->ae.out)
    sbitmap_vector_free (pre_data->ae.out);

  pre_data->transp = pre_data->comp = NULL;
  pre_data->insert_map = pre_data->delete_map = NULL;
  pre_data->ae.in = pre_data->ae.out = NULL;
}

/* Top level routine to do the dataflow analysis needed by PRE.  */

static void
compute_pre_data (pre_data)
     struct pre_global *pre_data;
{
  sbitmap trapping_expr;
  basic_block bb;
  unsigned int ui;

  compute_local_properties (pre_data->transp, pre_data->comp,
			    pre_data->antloc, &pre_data->expr_hash_table);
  if (gcse_file)
    {
      dump_sbitmap_vector (gcse_file, "antloc", "", pre_data->antloc, last_basic_block);
      dump_sbitmap_vector (gcse_file, "comp", "", pre_data->comp, last_basic_block);
      dump_sbitmap_vector (gcse_file, "transp", "", pre_data->transp, last_basic_block);
    }
  sbitmap_vector_zero (pre_data->ae.kill, last_basic_block);

  /* Collect expressions which might trap.  */
  trapping_expr = sbitmap_alloc (pre_data->expr_hash_table.n_elems);
  sbitmap_zero (trapping_expr);
  for (ui = 0; ui < pre_data->expr_hash_table.size; ui++)
    {
      struct expr *e;
      for (e = pre_data->expr_hash_table.table[ui]; e != NULL; e = e->next_same_hash)
	if (may_trap_p (e->expr))
	  SET_BIT (trapping_expr, e->bitmap_index);
    }

  /* Compute ae.kill for each basic block using:

     ~(TRANSP | COMP)

     This is significantly faster than compute_ae_kill.  */

  FOR_EACH_BB (bb)
    {
      edge e;

      /* If the current block is the destination of an abnormal edge, we
	 kill all trapping expressions because we won't be able to properly
	 place the instruction on the edge.  So make them neither
	 anticipatable nor transparent.  This is fairly conservative.  */
      for (e = bb->pred; e ; e = e->pred_next)
	if (e->flags & EDGE_ABNORMAL)
	  {
	    sbitmap_difference (pre_data->antloc[bb->index],
				pre_data->antloc[bb->index], trapping_expr);
	    sbitmap_difference (pre_data->transp[bb->index],
				pre_data->transp[bb->index], trapping_expr);
	    break;
	  }

      sbitmap_a_or_b (pre_data->ae.kill[bb->index], pre_data->transp[bb->index],
		      pre_data->comp[bb->index]);
      sbitmap_not (pre_data->ae.kill[bb->index], pre_data->ae.kill[bb->index]);
    }
  if (gcse_file)
    dump_sbitmap_vector (gcse_file, "kill", "", pre_data->ae.kill, last_basic_block);

  pre_data->edge_list =
	  pre_edge_lcm (gcse_file, pre_data->expr_hash_table.n_elems,
			pre_data->transp, pre_data->comp, pre_data->antloc,
			pre_data->ae.kill,
			&pre_data->insert_map, &pre_data->delete_map);
  sbitmap_vector_free (pre_data->antloc);
  pre_data->antloc = NULL;
  sbitmap_vector_free (pre_data->ae.kill);
  pre_data->ae.kill = NULL; 
  sbitmap_free (trapping_expr);
}

/* PRE utilities */

/* Return non-zero if an occurrence of expression EXPR in OCCR_BB would reach
   block BB.

   VISITED is a pointer to a working buffer for tracking which BB's have
   been visited.  It is NULL for the top-level call.

   We treat reaching expressions that go through blocks containing the same
   reaching expression as "not reaching".  E.g. if EXPR is generated in blocks
   2 and 3, INSN is in block 4, and 2->3->4, we treat the expression in block
   2 as not reaching.  The intent is to improve the probability of finding
   only one reaching expression and to reduce register lifetimes by picking
   the closest such expression.  */

static int
pre_expr_reaches_here_p_work (occr_bb, expr, bb, visited, pre_data)
     basic_block occr_bb;
     struct expr *expr;
     basic_block bb;
     char *visited;
     struct pre_global *pre_data;
{
  edge pred;

  for (pred = bb->pred; pred != NULL; pred = pred->pred_next)
    {
      basic_block pred_bb = pred->src;

      if (pred->src == ENTRY_BLOCK_PTR
	  /* Has predecessor has already been visited?  */
	  || visited[pred_bb->index])
	;/* Nothing to do.  */

      /* Does this predecessor generate this expression?  */
      else if (TEST_BIT (pre_data->comp[pred_bb->index], expr->bitmap_index))
	{
	  /* Is this the occurrence we're looking for?
	     Note that there's only one generating occurrence per block
	     so we just need to check the block number.  */
	  if (occr_bb == pred_bb)
	    return 1;

	  visited[pred_bb->index] = 1;
	}
      /* Ignore this predecessor if it kills the expression.  */
      else if (! TEST_BIT (pre_data->transp[pred_bb->index], expr->bitmap_index))
	visited[pred_bb->index] = 1;

      /* Neither gen nor kill.  */
      else
	{
	  visited[pred_bb->index] = 1;
	  if (pre_expr_reaches_here_p_work (occr_bb, expr, pred_bb, visited, pre_data))
	    return 1;
	}
    }

  /* All paths have been checked.  */
  return 0;
}

/* The wrapper for pre_expr_reaches_here_work that ensures that any
   memory allocated for that function is returned.  */

static int
pre_expr_reaches_here_p (occr_bb, expr, bb, pre_data)
     basic_block occr_bb;
     struct expr *expr;
     basic_block bb;
     struct pre_global *pre_data;
{
  int rval;
  char *visited = (char *) xcalloc (last_basic_block, 1);

  rval = pre_expr_reaches_here_p_work (occr_bb, expr, bb, visited, pre_data);

  free (visited);
  return rval;
}


/* Insert partially redundant expressions on edges in the CFG to make
   the expressions fully redundant.  */

static int
pre_edge_insert (index_map, pre_data)
     struct expr **index_map;
     struct pre_global *pre_data;
{
  int e, i, j, num_edges, set_size, did_insert = 0;
  sbitmap *inserted;

  /* Where PRE_INSERT_MAP is nonzero, we add the expression on that edge
     if it reaches any of the deleted expressions.  */

  set_size = pre_data->insert_map[0]->size;
  num_edges = NUM_EDGES (pre_data->edge_list);
  inserted = sbitmap_vector_alloc (num_edges, pre_data->expr_hash_table.n_elems);
  sbitmap_vector_zero (inserted, num_edges);

  for (e = 0; e < num_edges; e++)
    {
      int indx;
      basic_block bb = INDEX_EDGE_PRED_BB (pre_data->edge_list, e);

      for (i = indx = 0; i < set_size; i++, indx += SBITMAP_ELT_BITS)
	{
	  SBITMAP_ELT_TYPE insert = pre_data->insert_map[e]->elms[i];

	  for (j = indx; insert && j < (int) pre_data->expr_hash_table.n_elems; j++, insert >>= 1)
	    if ((insert & 1) != 0 && index_map[j]->reaching_reg != NULL_RTX)
	      {
		struct expr *expr = index_map[j];
		struct occr *occr;

		/* Now look at each deleted occurrence of this expression.  */
		for (occr = expr->antic_occr; occr != NULL; occr = occr->next)
		  {
		    if (! occr->deleted_p)
		      continue;

		    /* Insert this expression on this edge if if it would
		       reach the deleted occurrence in BB.  */
		    if (!TEST_BIT (inserted[e], j))
		      {
			rtx insn;
			edge eg = INDEX_EDGE (pre_data->edge_list, e);

			/* We can't insert anything on an abnormal and
			   critical edge, so we insert the insn at the end of
			   the previous block. There are several alternatives
			   detailed in Morgans book P277 (sec 10.5) for
			   handling this situation.  This one is easiest for
			   now.  */

			if ((eg->flags & EDGE_ABNORMAL) == EDGE_ABNORMAL)
			  insert_insn_end_bb (index_map[j], bb, 0,
					pre_data->antloc, pre_data->transp);
			else
			  {
			    insn = process_insert_insn (index_map[j]);
			    insert_insn_on_edge (insn, eg);
			  }

			if (gcse_file)
			  {
			    fprintf (gcse_file, "PRE/HOIST: edge (%d,%d), ",
				     bb->index,
				     INDEX_EDGE_SUCC_BB (pre_data->edge_list,
							 e)->index);
			    fprintf (gcse_file, "copy expression %d\n",
				     expr->bitmap_index);
			  }

			update_ld_motion_stores (expr);
			SET_BIT (inserted[e], j);
			did_insert = 1;
			gcse_create_count++;
		      }
		  }
	      }
	}
    }

  sbitmap_vector_free (inserted);
  return did_insert;
}

/* Copy the result of INSN to REG.  INDX is the expression number.  */

static void
pre_insert_copy_insn (expr, insn)
     struct expr *expr;
     rtx insn;
{
  rtx reg = expr->reaching_reg;
  int regno = REGNO (reg);
  int indx = expr->bitmap_index;
  rtx set = single_set (insn);
  rtx new_insn;

  if (!set)
    abort ();

  new_insn = emit_insn_after (gen_move_insn (reg, copy_rtx (SET_DEST (set))), insn);

  /* Keep register set table up to date.  */
  record_one_set (regno, new_insn);

  gcse_create_count++;

  if (gcse_file)
    fprintf (gcse_file,
	     "PRE: bb %d, insn %d, copy expression %d in insn %d to reg %d\n",
	      BLOCK_NUM (insn), INSN_UID (new_insn), indx,
	      INSN_UID (insn), regno);
  update_ld_motion_stores (expr);
}

/* Copy available expressions that reach the redundant expression
   to `reaching_reg'.  */

static void
pre_insert_copies (pre_data)
     struct pre_global *pre_data;
{
  unsigned int i;
  struct expr *expr;
  struct occr *occr;
  struct occr *avail;

  /* For each available expression in the table, copy the result to
     `reaching_reg' if the expression reaches a deleted one.

     ??? The current algorithm is rather brute force.
     Need to do some profiling.  */

  for (i = 0; i < pre_data->expr_hash_table.size; i++)
    for (expr = pre_data->expr_hash_table.table[i]; expr != NULL; expr = expr->next_same_hash)
      {
	/* If the basic block isn't reachable, PPOUT will be TRUE.  However,
	   we don't want to insert a copy here because the expression may not
	   really be redundant.  So only insert an insn if the expression was
	   deleted.  This test also avoids further processing if the
	   expression wasn't deleted anywhere.  */
	if (expr->reaching_reg == NULL)
	  continue;

	for (occr = expr->antic_occr; occr != NULL; occr = occr->next)
	  {
	    if (! occr->deleted_p)
	      continue;

	    for (avail = expr->avail_occr; avail != NULL; avail = avail->next)
	      {
		rtx insn = avail->insn;

		/* No need to handle this one if handled already.  */
		if (avail->copied_p)
		  continue;

		/* Don't handle this one if it's a redundant one.  */
		if (TEST_BIT (pre_data->redundant_insns, INSN_CUID (insn)))
		  continue;

		/* Or if the expression doesn't reach the deleted one.  */
		if (! pre_expr_reaches_here_p (BLOCK_FOR_INSN (avail->insn),
					       expr,
					       BLOCK_FOR_INSN (occr->insn),
					       pre_data))
		  continue;

		/* Copy the result of avail to reaching_reg.  */
		pre_insert_copy_insn (expr, insn);
		avail->copied_p = 1;
	      }
	  }
      }
}

/* Delete redundant computations.
   Deletion is done by changing the insn to copy the `reaching_reg' of
   the expression into the result of the SET.  It is left to later passes
   (cprop, cse2, flow, combine, regmove) to propagate the copy or eliminate it.

   Returns non-zero if a change is made.  */

static int
pre_delete (pre_data)
     struct pre_global *pre_data;
{
  unsigned int i;
  int changed;
  struct expr *expr;
  struct occr *occr;

  changed = 0;
  for (i = 0; i < pre_data->expr_hash_table.size; i++)
    for (expr = pre_data->expr_hash_table.table[i]; expr != NULL; expr = expr->next_same_hash)
      {
	int indx = expr->bitmap_index;

	/* We only need to search antic_occr since we require
	   ANTLOC != 0.  */

	for (occr = expr->antic_occr; occr != NULL; occr = occr->next)
	  {
	    rtx insn = occr->insn;
	    rtx set;
	    basic_block bb = BLOCK_FOR_INSN (insn);

	    if (TEST_BIT (pre_data->delete_map[bb->index], indx))
	      {
		set = single_set (insn);
		if (! set)
		  abort ();

		/* Create a pseudo-reg to store the result of reaching
		   expressions into.  Get the mode for the new pseudo from
		   the mode of the original destination pseudo.  */
		if (expr->reaching_reg == NULL)
		  expr->reaching_reg
		    = gen_reg_rtx (GET_MODE (SET_DEST (set)));

		gcse_emit_move_after (expr->reaching_reg, SET_DEST (set), insn);
		delete_insn (insn);
		occr->deleted_p = 1;
		SET_BIT (pre_data->redundant_insns, INSN_CUID (insn));
		changed = 1;
		gcse_subst_count++;

		if (gcse_file)
		  {
		    fprintf (gcse_file,
			     "PRE: redundant insn %d (expression %d) in ",
			       INSN_UID (insn), indx);
		    fprintf (gcse_file, "bb %d, reaching reg is %d\n",
			     bb->index, REGNO (expr->reaching_reg));
		  }
	      }
	  }
      }

  return changed;
}

/* Load Motion for loads which only kill themselves.  */

/* Return true if x is a simple MEM operation, with no registers or
   side effects. These are the types of loads we consider for the
   ld_motion list, otherwise we let the usual aliasing take care of it.  */

static int
simple_mem (x)
     rtx x;
{
  if (GET_CODE (x) != MEM)
    return 0;

  if (MEM_VOLATILE_P (x))
    return 0;

  if (GET_MODE (x) == BLKmode)
    return 0;

  if (side_effects_p (x))
    return 0;

  /* Do not consider function arguments passed on stack.  */
  if (reg_mentioned_p (stack_pointer_rtx, x))
    return 0;

  return 1;
}

/* Make sure there isn't a buried reference in this pattern anywhere.
   If there is, invalidate the entry for it since we're not capable
   of fixing it up just yet.. We have to be sure we know about ALL
   loads since the aliasing code will allow all entries in the
   ld_motion list to not-alias itself.  If we miss a load, we will get
   the wrong value since gcse might common it and we won't know to
   fix it up.  */

static void
invalidate_any_buried_refs (x)
     rtx x;
{
  const char * fmt;
  int i, j;
  struct ls_expr * ptr;

  /* Invalidate it in the list.  */
  if (GET_CODE (x) == MEM && simple_mem (x))
    {
      ptr = ldst_entry (x);
      ptr->invalid = 1;
    }

  /* Recursively process the insn.  */
  fmt = GET_RTX_FORMAT (GET_CODE (x));

  for (i = GET_RTX_LENGTH (GET_CODE (x)) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	invalidate_any_buried_refs (XEXP (x, i));
      else if (fmt[i] == 'E')
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  invalidate_any_buried_refs (XVECEXP (x, i, j));
    }
}

/* Find all the 'simple' MEMs which are used in LOADs and STORES. Simple
   being defined as MEM loads and stores to symbols, with no
   side effects and no registers in the expression. If there are any
   uses/defs which don't match this criteria, it is invalidated and
   trimmed out later.  */

static void
compute_ld_motion_mems ()
{
  struct ls_expr * ptr;
  basic_block bb;
  rtx insn;

  pre_ldst_mems = NULL;

  FOR_EACH_BB (bb)
    {
      for (insn = bb->head;
	   insn && insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  if (GET_RTX_CLASS (GET_CODE (insn)) == 'i')
	    {
	      if (GET_CODE (PATTERN (insn)) == SET)
		{
		  rtx src = SET_SRC (PATTERN (insn));
		  rtx dest = SET_DEST (PATTERN (insn));

		  /* Check for a simple LOAD...  */
		  if (GET_CODE (src) == MEM && simple_mem (src))
		    {
		      ptr = ldst_entry (src);
		      if (GET_CODE (dest) == REG)
			ptr->loads = alloc_INSN_LIST (insn, ptr->loads);
		      else
			ptr->invalid = 1;
		    }
		  else
		    {
		      /* Make sure there isn't a buried load somewhere.  */
		      invalidate_any_buried_refs (src);
		    }

		  /* Check for stores. Don't worry about aliased ones, they
		     will block any movement we might do later. We only care
		     about this exact pattern since those are the only
		     circumstance that we will ignore the aliasing info.  */
		  if (GET_CODE (dest) == MEM && simple_mem (dest))
		    {
		      ptr = ldst_entry (dest);

		      if (GET_CODE (src) != MEM
			  && GET_CODE (src) != ASM_OPERANDS)
			ptr->stores = alloc_INSN_LIST (insn, ptr->stores);
		      else
			ptr->invalid = 1;
		    }
		}
	      else
		invalidate_any_buried_refs (PATTERN (insn));
	    }
	}
    }
}

/* Remove any references that have been either invalidated or are not in the
   expression list for pre gcse.  */

static void
trim_ld_motion_mems (pre_data)
     struct pre_global *pre_data;
{
  struct ls_expr * last = NULL;
  struct ls_expr * ptr = first_ls_expr ();

  while (ptr != NULL)
    {
      int del = ptr->invalid;
      struct expr * expr = NULL;

      /* Delete if entry has been made invalid.  */
      if (!del)
	{
	  unsigned int i;

	  del = 1;
	  /* Delete if we cannot find this mem in the expression list.  */
	  for (i = 0; i < pre_data->expr_hash_table.size && del; i++)
	    {
	      for (expr = pre_data->expr_hash_table.table[i]; 
		   expr != NULL;
		   expr = expr->next_same_hash)
		if (expr_equiv_p (expr->expr, ptr->pattern))
		  {
		    del = 0;
		    break;
		  }
	    }
	}

      if (del)
	{
	  if (last != NULL)
	    {
	      last->next = ptr->next;
	      free_ldst_entry (ptr);
	      ptr = last->next;
	    }
	  else
	    {
	      pre_ldst_mems = pre_ldst_mems->next;
	      free_ldst_entry (ptr);
	      ptr = pre_ldst_mems;
	    }
	}
      else
	{
	  /* Set the expression field if we are keeping it.  */
	  last = ptr;
	  ptr->expr = expr;
	  ptr = ptr->next;
	}
    }

  /* Show the world what we've found.  */
  if (gcse_file && pre_ldst_mems != NULL)
    print_ldst_list (gcse_file);
}

/* This routine will take an expression which we are replacing with
   a reaching register, and update any stores that are needed if
   that expression is in the ld_motion list.  Stores are updated by
   copying their SRC to the reaching register, and then storeing
   the reaching register into the store location. These keeps the
   correct value in the reaching register for the loads.  */

static void
update_ld_motion_stores (expr)
     struct expr * expr;
{
  struct ls_expr * mem_ptr;

  if ((mem_ptr = find_rtx_in_ldst (expr->expr)))
    {
      /* We can try to find just the REACHED stores, but is shouldn't
	 matter to set the reaching reg everywhere...  some might be
	 dead and should be eliminated later.  */

      /* We replace  SET mem = expr   with
	   SET reg = expr
	   SET mem = reg , where reg is the
	   reaching reg used in the load.  */
      rtx list = mem_ptr->stores;

      for ( ; list != NULL_RTX; list = XEXP (list, 1))
	{
	  rtx insn = XEXP (list, 0);
	  rtx pat = PATTERN (insn);
	  rtx src = SET_SRC (pat);
	  rtx reg = expr->reaching_reg;
	  rtx copy, new;

	  /* If we've already copied it, continue.  */
	  if (expr->reaching_reg == src)
	    continue;

	  if (gcse_file)
	    {
	      fprintf (gcse_file, "PRE:  store updated with reaching reg ");
	      print_rtl (gcse_file, expr->reaching_reg);
	      fprintf (gcse_file, ":\n	");
	      print_inline_rtx (gcse_file, insn, 8);
	      fprintf (gcse_file, "\n");
	    }

	  copy = gen_move_insn ( reg, copy_rtx (SET_SRC (pat)));
	  new = emit_insn_before (copy, insn);
	  record_one_set (REGNO (reg), new);
	  SET_SRC (pat) = reg;

	  /* un-recognize this pattern since it's probably different now.  */
	  INSN_CODE (insn) = -1;
	  gcse_create_count++;
	}
    }
}

/* Perform GCSE optimizations using PRE.
   This is called by one_pre_gcse_pass after all the dataflow analysis
   has been done.

   This is based on the original Morel-Renvoise paper Fred Chow's thesis, and
   lazy code motion from Knoop, Ruthing and Steffen as described in Advanced
   Compiler Design and Implementation.

   ??? A new pseudo reg is created to hold the reaching expression.  The nice
   thing about the classical approach is that it would try to use an existing
   reg.  If the register can't be adequately optimized [i.e. we introduce
   reload problems], one could add a pass here to propagate the new register
   through the block.

   ??? We don't handle single sets in PARALLELs because we're [currently] not
   able to copy the rest of the parallel when we insert copies to create full
   redundancies from partial redundancies.  However, there's no reason why we
   can't handle PARALLELs in the cases where there are no partial
   redundancies.  */

static int
pre_gcse (pre_data)
     struct pre_global *pre_data;
{
  unsigned int i;
  int did_insert, changed;
  struct expr **index_map;
  struct expr *expr;

  /* Compute a mapping from expression number (`bitmap_index') to
     hash table entry.  */

  index_map = (struct expr **) xcalloc (pre_data->expr_hash_table.n_elems, sizeof (struct expr *));
  for (i = 0; i < pre_data->expr_hash_table.size; i++)
    for (expr = pre_data->expr_hash_table.table[i]; expr != NULL; expr = expr->next_same_hash)
      index_map[expr->bitmap_index] = expr;

  /* Reset bitmap used to track which insns are redundant.  */
  pre_data->redundant_insns = sbitmap_alloc (max_cuid);
  sbitmap_zero (pre_data->redundant_insns);

  /* Delete the redundant insns first so that
     - we know what register to use for the new insns and for the other
       ones with reaching expressions
     - we know which insns are redundant when we go to create copies  */

  changed = pre_delete (pre_data);
  
  did_insert = pre_edge_insert (index_map, pre_data);

  /* In other places with reaching expressions, copy the expression to the
     specially allocated pseudo-reg that reaches the redundant expr.  */
  pre_insert_copies (pre_data);
  if (did_insert)
    {
      commit_edge_insertions ();
      changed = 1;
    }

  free (index_map);
  sbitmap_free (pre_data->redundant_insns);
  return changed;
}

/* Top level routine to perform one PRE GCSE pass.

   Return non-zero if a change was made.  */

int
one_pre_gcse_pass (pass)
     int pass;
{
  int changed = 0;
  struct pre_global pre_data;

  gcse_subst_count = 0;
  gcse_create_count = 0;

  alloc_hash_table (max_cuid, &pre_data.expr_hash_table, 0);
  add_noreturn_fake_exit_edges ();
  if (flag_gcse_lm)
    compute_ld_motion_mems ();

  compute_hash_table (&pre_data.expr_hash_table);
  trim_ld_motion_mems (&pre_data);
  if (gcse_file)
    dump_hash_table (gcse_file, "Expression", &pre_data.expr_hash_table);

  if (pre_data.expr_hash_table.n_elems > 0)
    {
      alloc_pre_mem (last_basic_block, &pre_data);
      compute_pre_data (&pre_data);
      changed |= pre_gcse (&pre_data);
      free_edge_list (pre_data.edge_list);
      free_pre_mem (&pre_data);
    }

  free_ldst_mems ();
  remove_fake_edges ();
  free_hash_table (&pre_data.expr_hash_table);

  if (gcse_file)
    {
      fprintf (gcse_file, "\nPRE GCSE of %s, pass %d: %d bytes needed, ",
	       current_function_name, pass, bytes_used);
      fprintf (gcse_file, "%d substs, %d insns created\n",
	       gcse_subst_count, gcse_create_count);
    }

  return changed;
}
