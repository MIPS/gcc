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

#define ANTIC_STORE_LIST(x)	((x)->loads)
#define AVAIL_STORE_LIST(x)	((x)->stores)
#define LAST_AVAIL_CHECK_FAILURE(x)	((x)->reaching_reg)

static void reg_set_info		PARAMS ((rtx, rtx, void *));
static int store_ops_ok			PARAMS ((rtx, int *));
static rtx extract_mentioned_regs	PARAMS ((rtx));
static rtx extract_mentioned_regs_helper PARAMS ((rtx, rtx));
static void find_moveable_store		PARAMS ((rtx, int *, int *));
static int compute_store_table		PARAMS ((struct store_global *));
static int load_kills_store		PARAMS ((rtx, rtx));
static int find_loads			PARAMS ((rtx, rtx));
static int store_killed_in_insn		PARAMS ((rtx, rtx));
static int store_killed_after		PARAMS ((rtx, rtx, rtx, basic_block,
						 int *, rtx *));
static int store_killed_before		PARAMS ((rtx, rtx, rtx, basic_block,
						 int *));
static void build_store_vectors		PARAMS ((struct store_global *));
static int insert_store			PARAMS ((struct ls_expr *, edge,
					  struct store_global *));
static void replace_store_insn		PARAMS ((rtx, rtx, basic_block));
static void delete_store		PARAMS ((struct ls_expr *,
					  basic_block));
static void free_store_memory		PARAMS ((struct store_global *));

/* Checks to set if we need to mark a register set. Called from note_stores.  */

static void
reg_set_info (dest, setter, data)
     rtx dest, setter ATTRIBUTE_UNUSED;
     void * data;
{
  struct store_global *store_data = (struct store_global *) data;
  if (GET_CODE (dest) == SUBREG)
    dest = SUBREG_REG (dest);

  if (GET_CODE (dest) == REG)
    store_data->regvec[REGNO (dest)] = INSN_UID (store_data->current_insn);
}

/* Return zero if some of the registers in list X are killed
   due to set of registers in bitmap REGS_SET.  */

static int
store_ops_ok (x, regs_set)
     rtx x;
     int *regs_set;
{
  rtx reg;

  for (; x; x = XEXP (x, 1))
    {
      reg = XEXP (x, 0);
      if (regs_set[REGNO(reg)])
	return 0; 
    }

  return 1;
}

/* Returns a list of registers mentioned in X.  */
static rtx
extract_mentioned_regs (x)
     rtx x;
{
  return extract_mentioned_regs_helper (x, NULL_RTX);
}

/* Helper for extract_mentioned_regs; ACCUM is used to accumulate used
   registers.  */
static rtx
extract_mentioned_regs_helper (x, accum)
     rtx x;
     rtx accum;
{
  int i;
  enum rtx_code code;
  const char * fmt;

  /* Repeat is used to turn tail-recursion into iteration.  */
 repeat:

  if (x == 0)
    return accum;

  code = GET_CODE (x);
  switch (code)
    {
    case REG:
      return alloc_EXPR_LIST (0, x, accum);

    case MEM:
      x = XEXP (x, 0);
      goto repeat;

    case PRE_DEC:
    case PRE_INC:
    case POST_DEC:
    case POST_INC:
      /* We do not run this function with arguments having side effects.  */
      abort ();

    case PC:
    case CC0: /*FIXME*/
    case CONST:
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
    case SYMBOL_REF:
    case LABEL_REF:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
      return accum;

    default:
      break;
    }

  i = GET_RTX_LENGTH (code) - 1;
  fmt = GET_RTX_FORMAT (code);

  for (; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  rtx tem = XEXP (x, i);

	  /* If we are about to do the last recursive call
	     needed at this level, change it into iteration.  */
	  if (i == 0)
	    {
	      x = tem;
	      goto repeat;
	    }

	  accum = extract_mentioned_regs_helper (tem, accum);
	}
      else if (fmt[i] == 'E')
	{
	  int j;

	  for (j = 0; j < XVECLEN (x, i); j++)
	    accum = extract_mentioned_regs_helper (XVECEXP (x, i, j), accum);
	}
    }

  return accum;
}

/* Determine whether INSN is MEM store pattern that we will consider moving.
   REGS_SET_BEFORE is bitmap of registers set before (and including) the
   current insn, REGS_SET_AFTER is bitmap of registers set after (and
   including) the insn in this basic block.  We must be passing through BB from
   head to end, as we are using this fact to speed things up.
   
   The results are stored this way:

   -- the first anticipatable expression is added into ANTIC_STORE_LIST
   -- if the processed expression is not anticipatable, NULL_RTX is added
      there instead, so that we can use it as indicator that no further
      expression of this type may be anticipatable
   -- if the expression is available, it is added as head of AVAIL_STORE_LIST;
      consequently, all of them but this head are dead and may be deleted.
   -- if the expression is not available, the insn due to that it fails to be
      available is stored in reaching_reg.

   The things are complicated a bit by fact that there already may be stores
   to the same MEM from other blocks; also caller must take care of the neccessary
   cleanup of the temporary markers after end of the basic block.
   */

static void
find_moveable_store (insn, regs_set_before, regs_set_after)
     rtx insn;
     int *regs_set_before;
     int *regs_set_after;
{
  struct ls_expr * ptr;
  rtx dest, set, tmp;
  int check_anticipatable, check_available;
  basic_block bb = BLOCK_FOR_INSN (insn);

  set = single_set (insn);
  if (!set)
    return;

  dest = SET_DEST (set);

  if (GET_CODE (dest) != MEM || MEM_VOLATILE_P (dest)
      || GET_MODE (dest) == BLKmode)
    return;

  if (side_effects_p (dest))
    return;

  /* If we are handling exceptions, we must be careful with memory references
     that may trap. If we are not, the behavior is undefined, so we may just
     continue.  */
  if (flag_exceptions && may_trap_p (dest))
    return;

  /* Do not consider MEMs that mention stack pointer; in the following
     we rely on that constant functions do not read memory, which of course
     does not include their arguments if passed on stack.  */
  if (reg_mentioned_p (stack_pointer_rtx, dest))
    return;

  ptr = ldst_entry (dest);
  if (!ptr->pattern_regs)
    ptr->pattern_regs = extract_mentioned_regs (dest);

  /* Do not check for anticipatability if we either found one anticipatable
     store already, or tested for one and found out that it was killed.  */
  check_anticipatable = 0;
  if (!ANTIC_STORE_LIST (ptr))
    check_anticipatable = 1;
  else
    {
      tmp = XEXP (ANTIC_STORE_LIST (ptr), 0);
      if (tmp != NULL_RTX
	  && BLOCK_FOR_INSN (tmp) != bb)
	check_anticipatable = 1;
    }
  if (check_anticipatable)
    {
      if (store_killed_before (dest, ptr->pattern_regs, insn, bb, regs_set_before))
	tmp = NULL_RTX;
      else
	tmp = insn;
      ANTIC_STORE_LIST (ptr) = alloc_INSN_LIST (tmp,
						ANTIC_STORE_LIST (ptr));
    }

  /* It is not neccessary to check whether store is available if we did
     it successfully before; if we failed before, do not bother to check
     until we reach the insn that caused us to fail.  */
  check_available = 0;
  if (!AVAIL_STORE_LIST (ptr))
    check_available = 1;
  else
    {
      tmp = XEXP (AVAIL_STORE_LIST (ptr), 0);
      if (BLOCK_FOR_INSN (tmp) != bb)
	check_available = 1;
    }
  if (check_available)
    {
      /* Check that we have already reached the insn at that the check
	 failed last time.  */
      if (LAST_AVAIL_CHECK_FAILURE (ptr))
	{
	  for (tmp = bb->end;
	       tmp != insn && tmp != LAST_AVAIL_CHECK_FAILURE (ptr);
	       tmp = PREV_INSN (tmp))
	    continue;
	  if (tmp == insn)
	    check_available = 0;
	}
      else
	check_available = store_killed_after (dest, ptr->pattern_regs, insn,
					      bb, regs_set_after,
					      &LAST_AVAIL_CHECK_FAILURE (ptr));
    }
  if (!check_available)
    AVAIL_STORE_LIST (ptr) = alloc_INSN_LIST (insn, AVAIL_STORE_LIST (ptr));
}

/* Find available and anticipatable stores.  */

static int
compute_store_table (store_data)
     struct store_global *store_data;
{
  int ret;
  basic_block bb;
  unsigned regno;
  rtx insn, pat, tmp;
  int *last_set_in, *already_set;
  struct ls_expr * ptr, **prev_next_ptr_ptr;

  max_gcse_regno = max_reg_num ();

  reg_set_in_block = (sbitmap *) sbitmap_vector_alloc (last_basic_block,
						       max_gcse_regno);
  sbitmap_vector_zero (reg_set_in_block, last_basic_block);
  pre_ldst_mems = 0;
  last_set_in = xmalloc (sizeof (int) * max_gcse_regno);
  already_set = xmalloc (sizeof (int) * max_gcse_regno);

  /* Find all the stores we care about.  */
  FOR_EACH_BB (bb)
    {
      /* First compute the registers set in this block.  */
      memset (last_set_in, 0, sizeof (int) * max_gcse_regno);
      store_data->regvec = last_set_in;

      for (insn = bb->head;
	   insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  if (! INSN_P (insn))
	    continue;

	  if (GET_CODE (insn) == CALL_INSN)
	    {
	      bool clobbers_all = false;
#ifdef NON_SAVING_SETJMP
	      if (NON_SAVING_SETJMP
		  && find_reg_note (insn, REG_SETJMP, NULL_RTX))
		clobbers_all = true;
#endif

	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (clobbers_all
		    || TEST_HARD_REG_BIT (regs_invalidated_by_call, regno))
		  last_set_in[regno] = INSN_UID (insn);
	    }

	  pat = PATTERN (insn);
	  store_data->current_insn = insn;
	  note_stores (pat, reg_set_info, store_data);
	}

      /* Record the set registers.  */
      for (regno = 0; regno < max_gcse_regno; regno++)
	if (last_set_in[regno])
	  SET_BIT (reg_set_in_block[bb->index], regno);

      /* Now find the stores.  */
      memset (already_set, 0, sizeof (int) * max_gcse_regno);
      store_data->regvec = already_set;
      for (insn = bb->head;
	   insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  if (! INSN_P (insn))
	    continue;

	  if (GET_CODE (insn) == CALL_INSN)
	    {
	      bool clobbers_all = false;
#ifdef NON_SAVING_SETJMP
	      if (NON_SAVING_SETJMP
		  && find_reg_note (insn, REG_SETJMP, NULL_RTX))
		clobbers_all = true;
#endif

	      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (clobbers_all
		    || TEST_HARD_REG_BIT (regs_invalidated_by_call, regno))
		  already_set[regno] = 1;
	    }

	  pat = PATTERN (insn);
	  note_stores (pat, reg_set_info, store_data);

	  /* Now that we've marked regs, look for stores.  */
	  find_moveable_store (insn, already_set, last_set_in);

	  /* Unmark regs that are no longer set.  */
	  for (regno = 0; regno < max_gcse_regno; regno++)
	    if (last_set_in[regno] == INSN_UID (insn))
	      last_set_in[regno] = 0;
	}

      /* Clear temporary marks.  */
      for (ptr = first_ls_expr (); ptr != NULL; ptr = next_ls_expr (ptr))
	{
	  LAST_AVAIL_CHECK_FAILURE(ptr) = NULL_RTX;
	  if (ANTIC_STORE_LIST (ptr)
	      && (tmp = XEXP (ANTIC_STORE_LIST (ptr), 0)) == NULL_RTX)
	    ANTIC_STORE_LIST (ptr) = XEXP (ANTIC_STORE_LIST (ptr), 1);
	}
    }

  /* Remove the stores that are not available anywhere, as there will
     be no opportunity to optimize them.  */
  for (ptr = pre_ldst_mems, prev_next_ptr_ptr = &pre_ldst_mems;
       ptr != NULL;
       ptr = *prev_next_ptr_ptr)
    {
      if (!AVAIL_STORE_LIST (ptr))
	{
	  *prev_next_ptr_ptr = ptr->next;
	  free_ldst_entry (ptr);
	}
      else
	prev_next_ptr_ptr = &ptr->next;
    }

  ret = enumerate_ldsts ();

  if (gcse_file)
    {
      fprintf (gcse_file, "ST_avail and ST_antic (shown under loads..)\n");
      print_ldst_list (gcse_file);
    }

  free (last_set_in);
  free (already_set);
  return ret;
}

/* Check to see if the load X is aliased with STORE_PATTERN.  */

static int
load_kills_store (x, store_pattern)
     rtx x, store_pattern;
{
  if (true_dependence (x, GET_MODE (x), store_pattern, rtx_addr_varies_p))
    return 1;
  return 0;
}

/* Go through the entire insn X, looking for any loads which might alias
   STORE_PATTERN.  Return 1 if found.  */

static int
find_loads (x, store_pattern)
     rtx x, store_pattern;
{
  const char * fmt;
  int i, j;
  int ret = 0;

  if (!x)
    return 0;

  if (GET_CODE (x) == SET)
    x = SET_SRC (x);

  if (GET_CODE (x) == MEM)
    {
      if (load_kills_store (x, store_pattern))
	return 1;
    }

  /* Recursively process the insn.  */
  fmt = GET_RTX_FORMAT (GET_CODE (x));

  for (i = GET_RTX_LENGTH (GET_CODE (x)) - 1; i >= 0 && !ret; i--)
    {
      if (fmt[i] == 'e')
	ret |= find_loads (XEXP (x, i), store_pattern);
      else if (fmt[i] == 'E')
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  ret |= find_loads (XVECEXP (x, i, j), store_pattern);
    }
  return ret;
}

/* Check if INSN kills the store pattern X (is aliased with it).
   Return 1 if it it does.  */

static int
store_killed_in_insn (x, insn)
     rtx x, insn;
{
  if (GET_RTX_CLASS (GET_CODE (insn)) != 'i')
    return 0;

  if (GET_CODE (insn) == CALL_INSN)
    {
      /* A normal or pure call might read from pattern,
	 but a const call will not.  */
      return ! CONST_OR_PURE_CALL_P (insn) || pure_call_p (insn);
    }

  if (GET_CODE (PATTERN (insn)) == SET)
    {
      rtx pat = PATTERN (insn);
      /* Check for memory stores to aliased objects.  */
      if (GET_CODE (SET_DEST (pat)) == MEM && !expr_equiv_p (SET_DEST (pat), x))
	/* pretend its a load and check for aliasing.  */
	if (find_loads (SET_DEST (pat), x))
	  return 1;
      return find_loads (SET_SRC (pat), x);
    }
  else
    return find_loads (PATTERN (insn), x);
}

/* Returns 1 if the expression X is loaded or clobbered on or after INSN
   within basic block BB.  REGS_SET_AFTER is bitmap of registers set in
   or after the insn.  X_REGS is list of registers mentioned in X. If the store
   is killed, return the last insn in that it occurs in FAIL_INSN.  */

static int
store_killed_after (x, x_regs, insn, bb, regs_set_after, fail_insn)
     rtx x, x_regs, insn;
     basic_block bb;
     int *regs_set_after;
     rtx *fail_insn;
{
  rtx last = bb->end, act;

  if (!store_ops_ok (x_regs, regs_set_after))
    { 
      /* We do not know where it will happen.  */
      if (fail_insn)
	*fail_insn = NULL_RTX;
      return 1;
    }

  /* Scan from the end, so that fail_insn is determined correctly.  */
  for (act = last; act != PREV_INSN (insn); act = PREV_INSN (act))
    if (store_killed_in_insn (x, act))
      {
	if (fail_insn)
	  *fail_insn = act;
	return 1;
      }

  return 0;
}

/* Returns 1 if the expression X is loaded or clobbered on or before INSN
   within basic block BB. X_REGS is list of registers mentioned in X.
   REGS_SET_BEFORE is bitmap of registers set before or in this insn.  */
static int
store_killed_before (x, x_regs, insn, bb, regs_set_before)
     rtx x, x_regs, insn;
     basic_block bb;
     int *regs_set_before;
{
  rtx first = bb->head;

  if (!store_ops_ok (x_regs, regs_set_before))
    return 1;

  for ( ; insn != PREV_INSN (first); insn = PREV_INSN (insn))
    if (store_killed_in_insn (x, insn))
      return 1;

  return 0;
}

/* Fill in available, anticipatable, transparent and kill vectors in
   STORE_DATA, based on lists of available and anticipatable stores.  */
static void
build_store_vectors (store_data)
     struct store_global *store_data;
{
  basic_block bb;
  int *regs_set_in_block;
  rtx insn, st;
  struct ls_expr * ptr;
  unsigned regno;

  /* Build the gen_vector. This is any store in the table which is not killed
     by aliasing later in its block.  */
  store_data->ae.gen = (sbitmap *) sbitmap_vector_alloc (last_basic_block,
					store_data->num_stores);
  sbitmap_vector_zero (store_data->ae.gen, last_basic_block);

  store_data->antloc = (sbitmap *) sbitmap_vector_alloc (last_basic_block,
					store_data->num_stores);
  sbitmap_vector_zero (store_data->antloc, last_basic_block);

  for (ptr = first_ls_expr (); ptr != NULL; ptr = next_ls_expr (ptr))
    {
      for (st = AVAIL_STORE_LIST (ptr); st != NULL; st = XEXP (st, 1))
	{
	  insn = XEXP (st, 0);
	  bb = BLOCK_FOR_INSN (insn);

	  /* If we've already seen an available expression in this block,
    	     we can delete this one (It occurs earlier in the block). We'll
	     copy the SRC expression to an unused register in case there
	     are any side effects.  */
	  if (TEST_BIT (store_data->ae.gen[bb->index], ptr->index))
	    {
	      rtx r = gen_reg_rtx (GET_MODE (ptr->pattern));
	      if (gcse_file)
		fprintf (gcse_file, "Removing redundant store:\n");
	      replace_store_insn (r, XEXP (st, 0), bb);
	      continue;
	    }
	  SET_BIT (store_data->ae.gen[bb->index], ptr->index);
	}

      for (st = ANTIC_STORE_LIST (ptr); st != NULL; st = XEXP (st, 1))
	{
	  insn = XEXP (st, 0);
	  bb = BLOCK_FOR_INSN (insn);
	  SET_BIT (store_data->antloc[bb->index], ptr->index);
	}
    }

  store_data->ae.kill = (sbitmap *) sbitmap_vector_alloc (last_basic_block, store_data->num_stores);
  sbitmap_vector_zero (store_data->ae.kill, last_basic_block);

  store_data->transp = (sbitmap *) sbitmap_vector_alloc (last_basic_block, store_data->num_stores);
  sbitmap_vector_zero (store_data->transp, last_basic_block);
  regs_set_in_block = xmalloc (sizeof (int) * max_gcse_regno);

  FOR_EACH_BB (bb)
    {
      for (regno = 0; regno < max_gcse_regno; regno++)
	regs_set_in_block[regno] = TEST_BIT (reg_set_in_block[bb->index], regno);

      for (ptr = first_ls_expr (); ptr != NULL; ptr = next_ls_expr (ptr))
	{
	  if (store_killed_after (ptr->pattern, ptr->pattern_regs, bb->head,
				  bb, regs_set_in_block, NULL))
	    {
	      /* It should not be neccessary to consider the expression
		 killed if it is both anticipatable and available.  */
	      if (!TEST_BIT (store_data->antloc[bb->index], ptr->index)
		  || !TEST_BIT (store_data->ae.gen[bb->index], ptr->index))
		SET_BIT (store_data->ae.kill[bb->index], ptr->index);
  	    }
  	  else
  	    SET_BIT (store_data->transp[bb->index], ptr->index);
       	}
    }

  free (regs_set_in_block);

  if (gcse_file)
    {
      dump_sbitmap_vector (gcse_file, "st_antloc", "", store_data->antloc, last_basic_block);
      dump_sbitmap_vector (gcse_file, "st_kill", "", store_data->ae.kill, last_basic_block);
      dump_sbitmap_vector (gcse_file, "Transpt", "", store_data->transp, last_basic_block);
      dump_sbitmap_vector (gcse_file, "st_avloc", "", store_data->ae.gen, last_basic_block);
    }
}

/* This routine will insert a store on an edge. EXPR is the ldst entry for
   the memory reference, and E is the edge to insert it on.  Returns non-zero
   if an edge insertion was performed.  */

static int
insert_store (expr, e, store_data)
     struct ls_expr * expr;
     edge e;
     struct store_global *store_data;
{
  rtx reg, insn;
  basic_block bb;
  edge tmp;

  /* We did all the deleted before this insert, so if we didn't delete a
     store, then we haven't set the reaching reg yet either.  */
  if (expr->reaching_reg == NULL_RTX)
    return 0;

  reg = expr->reaching_reg;
  insn = gen_move_insn (copy_rtx (expr->pattern), reg);

  /* If we are inserting this expression on ALL predecessor edges of a BB,
     insert it at the start of the BB, and reset the insert bits on the other
     edges so we don't try to insert it on the other edges.  */
  bb = e->dest;
  for (tmp = e->dest->pred; tmp ; tmp = tmp->pred_next)
    {
      int index = EDGE_INDEX (store_data->edge_list, tmp->src, tmp->dest);
      if (index == EDGE_INDEX_NO_EDGE)
	abort ();
      if (! TEST_BIT (store_data->insert_map[index], expr->index))
	break;
    }

  /* If tmp is NULL, we found an insertion on every edge, blank the
     insertion vector for these edges, and insert at the start of the BB.  */
  if (!tmp && bb != EXIT_BLOCK_PTR)
    {
      for (tmp = e->dest->pred; tmp ; tmp = tmp->pred_next)
	{
	  int index = EDGE_INDEX (store_data->edge_list, tmp->src, tmp->dest);
	  RESET_BIT (store_data->insert_map[index], expr->index);
	}
      insert_insn_start_bb (insn, bb);
      return 0;
    }

  /* We can't insert on this edge, so we'll insert at the head of the
     successors block.  See Morgan, sec 10.5.  */
  if ((e->flags & EDGE_ABNORMAL) == EDGE_ABNORMAL)
    {
      insert_insn_start_bb (insn, bb);
      return 0;
    }

  insert_insn_on_edge (insn, e);

  if (gcse_file)
    {
      fprintf (gcse_file, "STORE_MOTION  insert insn on edge (%d, %d):\n",
	       e->src->index, e->dest->index);
      print_inline_rtx (gcse_file, insn, 6);
      fprintf (gcse_file, "\n");
    }

  return 1;
}

/* This routine will replace a store with a SET to a specified register.  */

static void
replace_store_insn (reg, del, bb)
     rtx reg, del;
     basic_block bb;
{
  rtx insn;

  insn = gen_move_insn (reg, SET_SRC (PATTERN (del)));
  insn = emit_insn_after (insn, del);

  if (gcse_file)
    {
      fprintf (gcse_file,
	       "STORE_MOTION  delete insn in BB %d:\n      ", bb->index);
      print_inline_rtx (gcse_file, del, 6);
      fprintf (gcse_file, "\nSTORE MOTION  replaced with insn:\n      ");
      print_inline_rtx (gcse_file, insn, 6);
      fprintf (gcse_file, "\n");
    }

  delete_insn (del);
}

/* Delete a store, but copy the value that would have been stored into
   the reaching_reg for later storing.  */

static void
delete_store (expr, bb)
     struct ls_expr * expr;
     basic_block bb;
{
  rtx reg, i, del;

  if (expr->reaching_reg == NULL_RTX)
    expr->reaching_reg = gen_reg_rtx (GET_MODE (expr->pattern));


  reg = expr->reaching_reg;

  for (i = AVAIL_STORE_LIST (expr); i; i = XEXP (i, 1))
    {
      del = XEXP (i, 0);
      if (BLOCK_FOR_INSN (del) == bb)
	{
	  /* We know there is only one since we deleted redundant
	     ones during the available computation.  */
	  replace_store_insn (reg, del, bb);
	  break;
	}
    }
}

/* Free memory used by store motion.  */

static void
free_store_memory (store_data)
     struct store_global *store_data;
{
  free_ldst_mems ();

  if (store_data->ae.gen)
    sbitmap_vector_free (store_data->ae.gen);
  if (store_data->ae.kill)
    sbitmap_vector_free (store_data->ae.kill);
  if (store_data->transp)
    sbitmap_vector_free (store_data->transp);
  if (store_data->antloc)
    sbitmap_vector_free (store_data->antloc);
  if (store_data->insert_map)
    sbitmap_vector_free (store_data->insert_map);
  if (store_data->delete_map)
    sbitmap_vector_free (store_data->delete_map);
  if (reg_set_in_block)
    sbitmap_vector_free (reg_set_in_block);
  
  store_data->ae.gen = store_data->ae.kill =
    store_data->transp = store_data->antloc = NULL;
  store_data->insert_map = store_data->delete_map =
    reg_set_in_block = NULL;
}

/* Perform store motion. Much like gcse, except we move expressions the
   other way by looking at the flowgraph in reverse.  */

void
store_motion ()
{
  basic_block bb;
  int x;
  struct ls_expr * ptr;
  int update_flow = 0;
  struct store_global store_data;

  gcse_file = rtl_dump_file;
  if (gcse_file)
    {
      fprintf (gcse_file, "before store motion\n");
      print_rtl (gcse_file, get_insns ());

      FOR_EACH_BB (bb)
	{
	  edge e;

	  fprintf (gcse_file, "%d ->", bb->index);
	  for (e = bb->succ; e; e = e->succ_next)
	    fprintf (gcse_file, " %d", e->dest->index);
	  fprintf (gcse_file, "\n");
	}
    }

  init_alias_analysis ();

  /* Find all the available and anticipatable stores.  */
  store_data.num_stores = compute_store_table (&store_data);
  if (store_data.num_stores == 0)
    {
      sbitmap_vector_free (reg_set_in_block);
      end_alias_analysis ();
      return;
    }

  /* Now compute kill & transp vectors.  */
  build_store_vectors (&store_data);

  add_noreturn_fake_exit_edges ();

  store_data.edge_list = pre_edge_rev_lcm (gcse_file, store_data.num_stores,
				store_data.transp, store_data.ae.gen, 
				store_data.antloc, store_data.ae.kill,
				&store_data.insert_map, &store_data.delete_map);

  /* Now we want to insert the new stores which are going to be needed.  */
  for (ptr = first_ls_expr (); ptr != NULL; ptr = next_ls_expr (ptr))
    {
      FOR_EACH_BB (bb)
	if (TEST_BIT (store_data.delete_map[bb->index], ptr->index))
	  delete_store (ptr, bb);

      for (x = 0; x < NUM_EDGES (store_data.edge_list); x++)
	if (TEST_BIT (store_data.insert_map[x], ptr->index))
	  update_flow |= insert_store (ptr, INDEX_EDGE (store_data.edge_list, x),
				       &store_data);
    }

  if (update_flow)
    commit_edge_insertions ();

  free_store_memory (&store_data);
  free_edge_list (store_data.edge_list);
  remove_fake_edges ();
  end_alias_analysis ();
}
