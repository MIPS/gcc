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

static void reg_set_info		PARAMS ((rtx, rtx, void *));
static int store_ops_ok			PARAMS ((rtx, basic_block));
static void find_moveable_store		PARAMS ((rtx));
static int compute_store_table		PARAMS ((struct store_global *));
static int load_kills_store		PARAMS ((rtx, rtx));
static int find_loads			PARAMS ((rtx, rtx));
static int store_killed_in_insn		PARAMS ((rtx, rtx));
static int store_killed_after		PARAMS ((rtx, rtx, basic_block));
static int store_killed_before		PARAMS ((rtx, rtx, basic_block));
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
    SET_BIT (*store_data->regvec, REGNO (dest));
}

/* Return non-zero if the register operands of expression X are killed
   anywhere in basic block BB.  */

static int
store_ops_ok (x, bb)
     rtx x;
     basic_block bb;
{
  int i;
  enum rtx_code code;
  const char * fmt;

  /* Repeat is used to turn tail-recursion into iteration.  */
 repeat:

  if (x == 0)
    return 1;

  code = GET_CODE (x);
  switch (code)
    {
    case REG:
	/* If a reg has changed after us in this
	   block, the operand has been killed.  */
	return TEST_BIT (reg_set_in_block[bb->index], REGNO (x));

    case MEM:
      x = XEXP (x, 0);
      goto repeat;

    case PRE_DEC:
    case PRE_INC:
    case POST_DEC:
    case POST_INC:
      return 0;

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
      return 1;

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
	     needed at this level, change it into iteration.
	     This function is called enough to be worth it.  */
	  if (i == 0)
	    {
	      x = tem;
	      goto repeat;
	    }

	  if (! store_ops_ok (tem, bb))
	    return 0;
	}
      else if (fmt[i] == 'E')
	{
	  int j;

	  for (j = 0; j < XVECLEN (x, i); j++)
	    {
	      if (! store_ops_ok (XVECEXP (x, i, j), bb))
		return 0;
	    }
	}
    }

  return 1;
}

/* Determine whether insn is MEM store pattern that we will consider moving.  */

static void
find_moveable_store (insn)
     rtx insn;
{
  struct ls_expr * ptr;
  rtx dest = PATTERN (insn);

  if (GET_CODE (dest) != SET
      || GET_CODE (SET_SRC (dest)) == ASM_OPERANDS)
    return;

  dest = SET_DEST (dest);

  if (GET_CODE (dest) != MEM || MEM_VOLATILE_P (dest)
      || GET_MODE (dest) == BLKmode)
    return;

  if (GET_CODE (XEXP (dest, 0)) != SYMBOL_REF)
      return;

  if (rtx_varies_p (XEXP (dest, 0), 0))
    return;

  ptr = ldst_entry (dest);
  ptr->stores = alloc_INSN_LIST (insn, ptr->stores);
}

/* Perform store motion. Much like gcse, except we move expressions the
   other way by looking at the flowgraph in reverse.  */

static int
compute_store_table (store_data)
     struct store_global *store_data;
{
  int ret;
  basic_block bb;
  unsigned regno;
  rtx insn, pat;

  max_gcse_regno = max_reg_num ();

  reg_set_in_block = (sbitmap *) sbitmap_vector_alloc (last_basic_block,
						       max_gcse_regno);
  sbitmap_vector_zero (reg_set_in_block, last_basic_block);
  pre_ldst_mems = 0;

  /* Find all the stores we care about.  */
  FOR_EACH_BB (bb)
    {
      store_data->regvec = & (reg_set_in_block[bb->index]);
      for (insn = bb->end;
	   insn && insn != PREV_INSN (bb->end);
	   insn = PREV_INSN (insn))
	{
	  /* Ignore anything that is not a normal insn.  */
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
		  SET_BIT (reg_set_in_block[bb->index], regno);
	    }

	  pat = PATTERN (insn);
	  note_stores (pat, reg_set_info, store_data);

	  /* Now that we've marked regs, look for stores.  */
	  if (GET_CODE (pat) == SET)
	    find_moveable_store (insn);
	}
    }

  ret = enumerate_ldsts ();

  if (gcse_file)
    {
      fprintf (gcse_file, "Store Motion Expressions.\n");
      print_ldst_list (gcse_file);
    }

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
   within basic block BB.  */

static int
store_killed_after (x, insn, bb)
     rtx x, insn;
     basic_block bb;
{
  rtx last = bb->end;

  if (insn == last)
    return 0;

  /* Check if the register operands of the store are OK in this block.
     Note that if registers are changed ANYWHERE in the block, we'll
     decide we can't move it, regardless of whether it changed above
     or below the store. This could be improved by checking the register
     operands while lookinng for aliasing in each insn.  */
  if (!store_ops_ok (XEXP (x, 0), bb))
    return 1;

  for ( ; insn && insn != NEXT_INSN (last); insn = NEXT_INSN (insn))
    if (store_killed_in_insn (x, insn))
      return 1;

  return 0;
}

/* Returns 1 if the expression X is loaded or clobbered on or before INSN
   within basic block BB.  */
static int
store_killed_before (x, insn, bb)
     rtx x, insn;
     basic_block bb;
{
  rtx first = bb->head;

  if (insn == first)
    return store_killed_in_insn (x, insn);

  /* Check if the register operands of the store are OK in this block.
     Note that if registers are changed ANYWHERE in the block, we'll
     decide we can't move it, regardless of whether it changed above
     or below the store. This could be improved by checking the register
     operands while lookinng for aliasing in each insn.  */
  if (!store_ops_ok (XEXP (x, 0), bb))
    return 1;

  for ( ; insn && insn != PREV_INSN (first); insn = PREV_INSN (insn))
    if (store_killed_in_insn (x, insn))
      return 1;

  return 0;
}

#define ANTIC_STORE_LIST(x)	((x)->loads)
#define AVAIL_STORE_LIST(x)	((x)->stores)

/* Given the table of available store insns at the end of blocks,
   determine which ones are not killed by aliasing, and generate
   the appropriate vectors for gen and killed.  */
static void
build_store_vectors (store_data)
     struct store_global *store_data;
{
  basic_block bb, b;
  rtx insn, st;
  struct ls_expr * ptr;

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
      /* Put all the stores into either the antic list, or the avail list,
	 or both.  */
      rtx store_list = ptr->stores;
      ptr->stores = NULL_RTX;

      for (st = store_list; st != NULL; st = XEXP (st, 1))
	{
	  insn = XEXP (st, 0);
	  bb = BLOCK_FOR_INSN (insn);

	  if (!store_killed_after (ptr->pattern, insn, bb))
	    {
	      /* If we've already seen an availale expression in this block,
		 we can delete the one we saw already (It occurs earlier in
		 the block), and replace it with this one). We'll copy the
		 old SRC expression to an unused register in case there
		 are any side effects.  */
	      if (TEST_BIT (store_data->ae.gen[bb->index], ptr->index))
		{
		  /* Find previous store.  */
		  rtx st;
		  for (st = AVAIL_STORE_LIST (ptr); st ; st = XEXP (st, 1))
		    if (BLOCK_FOR_INSN (XEXP (st, 0)) == bb)
		      break;
		  if (st)
		    {
		      rtx r = gen_reg_rtx (GET_MODE (ptr->pattern));
		      if (gcse_file)
			fprintf (gcse_file, "Removing redundant store:\n");
		      replace_store_insn (r, XEXP (st, 0), bb);
		      XEXP (st, 0) = insn;
		      continue;
		    }
		}
	      SET_BIT (store_data->ae.gen[bb->index], ptr->index);
	      AVAIL_STORE_LIST (ptr) = alloc_INSN_LIST (insn,
							AVAIL_STORE_LIST (ptr));
	    }

	  if (!store_killed_before (ptr->pattern, insn, bb))
	    {
	      SET_BIT (store_data->antloc[BLOCK_NUM (insn)], ptr->index);
	      ANTIC_STORE_LIST (ptr) = alloc_INSN_LIST (insn,
							ANTIC_STORE_LIST (ptr));
	    }
	}

      /* Free the original list of store insns.  */
      free_INSN_LIST_list (&store_list);
    }

  store_data->ae.kill = (sbitmap *) sbitmap_vector_alloc (last_basic_block, store_data->num_stores);
  sbitmap_vector_zero (store_data->ae.kill, last_basic_block);

  store_data->transp = (sbitmap *) sbitmap_vector_alloc (last_basic_block, store_data->num_stores);
  sbitmap_vector_zero (store_data->transp, last_basic_block);

  for (ptr = first_ls_expr (); ptr != NULL; ptr = next_ls_expr (ptr))
    FOR_EACH_BB (b)
      {
	if (store_killed_after (ptr->pattern, b->head, b))
	  {
	    /* The anticipatable expression is not killed if it's gen'd.  */
	    /*
	      We leave this check out for now. If we have a code sequence
	      in a block which looks like:
			ST MEMa = x
			L     y = MEMa
			ST MEMa = z
	      We should flag this as having an ANTIC expression, NOT
	      transparent, NOT killed, and AVAIL.
	      Unfortunately, since we haven't re-written all loads to
	      use the reaching reg, we'll end up doing an incorrect
	      Load in the middle here if we push the store down. It happens in
		    gcc.c-torture/execute/960311-1.c with -O3
	      If we always kill it in this case, we'll sometimes do
	      uneccessary work, but it shouldn't actually hurt anything.
	    if (!TEST_BIT (ae_gen[b], ptr->index)).  */
	    SET_BIT (store_data->ae.kill[b->index], ptr->index);
	  }
	else
	  SET_BIT (store_data->transp[b->index], ptr->index);
      }

  /* Any block with no exits calls some non-returning function, so
     we better mark the store killed here, or we might not store to
     it at all.  If we knew it was abort, we wouldn't have to store,
     but we don't know that for sure.  */
  if (gcse_file)
    {
      fprintf (gcse_file, "ST_avail and ST_antic (shown under loads..)\n");
      print_ldst_list (gcse_file);
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
  insn = gen_move_insn (expr->pattern, reg);

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


  /* If there is more than 1 store, the earlier ones will be dead,
     but it doesn't hurt to replace them here.  */
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

  if (gcse_file)
    {
      fprintf (gcse_file, "before store motion\n");
      print_rtl (gcse_file, get_insns ());
    }

  init_alias_analysis ();

  /* Find all the stores that are live to the end of their block.  */
  store_data.num_stores = compute_store_table (&store_data);
  if (store_data.num_stores == 0)
    {
      sbitmap_vector_free (reg_set_in_block);
      end_alias_analysis ();
      return;
    }

  /* Now compute whats actually available to move.  */
  add_noreturn_fake_exit_edges ();
  build_store_vectors (&store_data);

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
