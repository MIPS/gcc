/* RTL dead code elimination.
   Copyright (C) 2005 Free Software Foundation, Inc.

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "hashtab.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "df.h"
#include "cselib.h"
#include "dce.h"
#include "timevar.h"
#include "tree-pass.h"
#include "dbgcnt.h"

DEF_VEC_I(int);
DEF_VEC_ALLOC_I(int,heap);


/* -------------------------------------------------------------------------
   Core mark/delete routines
   ------------------------------------------------------------------------- */

/* The data-flow information needed by this pass.  */
static bool df_in_progress = false;

/* True if we deleted at least one instruction.  */
static bool something_changed;

/* Instructions that have been marked but whose dependencies have not
   yet been processed.  */
static VEC(rtx,heap) *worklist;

static bitmap marked = NULL;

/* Return true if INSN a normal instruction that can be deleted by the
   DCE pass.  */

static bool
deletable_insn_p (rtx insn)
{
  rtx x;

  switch (GET_CODE (PATTERN (insn)))
    {
    case USE:
    case PREFETCH:
    case TRAP_IF:
      /* The UNSPEC case was added here because the ia-64 claims that
	 USEs do not work after reload and generates UNSPECS rather
	 than USEs.  Since dce/dse is run after reload we need to
	 avoid deleting these even if they are dead.  If it turns out
	 that USEs really do work after reload, the ia-64 should be
	 changed, and the UNSPEC case can be removed.  */
    case UNSPEC:
      return false;

    case CLOBBER:
      /* A CLOBBER of a dead pseudo register serves no purpose.
	 That is not necessarily true for hard registers until
	 after reload.  */
      x = XEXP (PATTERN (insn), 0);
      return REG_P (x) && (!HARD_REGISTER_P (x) || reload_completed);

    default:
      if (!NONJUMP_INSN_P (insn))
	return false;

      if (volatile_insn_p (PATTERN (insn)))
	return false;

      if (flag_non_call_exceptions && may_trap_p (PATTERN (insn)))
	return false;

      return true;
    }
}


/* Return true if INSN has not been marked as needed.  */

static inline bool
marked_insn_p (rtx insn)
{
  if (insn)
    return bitmap_bit_p (marked, INSN_UID (insn));
  else 
    /* Artificial defs are always needed and they do not have an
       insn.  */
    return true;
}


/* If INSN has not yet been marked as needed, mark it now, and add it to
   the worklist.  */

static void
mark_insn (rtx insn, bool fast)
{
  if (!marked_insn_p (insn))
    {
      if (!fast)
	VEC_safe_push (rtx, heap, worklist, insn);
      bitmap_set_bit (marked, INSN_UID (insn));
      if (dump_file)
	fprintf (dump_file, "  Adding insn %d to worklist\n", INSN_UID (insn));
    }
}


/* A note_stores callback used by mark_nonreg_stores.  DATA is the
   instruction containing DEST.  */

static void
mark_nonreg_stores_1 (rtx dest, rtx pattern, void *data)
{
  if (GET_CODE (pattern) != CLOBBER && !REG_P (dest))
    mark_insn ((rtx) data, true);
}


/* A note_stores callback used by mark_nonreg_stores.  DATA is the
   instruction containing DEST.  */

static void
mark_nonreg_stores_2 (rtx dest, rtx pattern, void *data)
{
  if (GET_CODE (pattern) != CLOBBER && !REG_P (dest))
    mark_insn ((rtx) data, false);
}


/* Mark INSN if BODY stores to a non-register destination.  */

static void
mark_nonreg_stores (rtx body, rtx insn, bool fast)
{
  if (fast)
    note_stores (body, mark_nonreg_stores_1, insn);
  else
    note_stores (body, mark_nonreg_stores_2, insn);
}


/* Initialize global variables for a new DCE pass.  */

static void
init_dce (bool fast)
{
  if (!df_in_progress)
    {
      if (!fast)
	df_chain_add_problem (DF_UD_CHAIN);
      df_analyze ();
    }

  if (dump_file)
    df_dump (dump_file);

  marked = BITMAP_ALLOC (NULL);
}


/* Delete all REG_EQUAL notes of the registers INSN writes, to prevent
   bad dangling REG_EQUAL notes. */

static void
delete_corresponding_reg_eq_notes (rtx insn)
{
  struct df_ref *def;
  for (def = DF_INSN_DEFS (insn); def; def = DF_REF_NEXT_REF (def))
    {
      unsigned int regno = DF_REF_REGNO (def);
      /* This loop is a little tricky.  We cannot just go down the
	 chain because it is being modified by the actions in the
	 loop.  So we just get the head.  We plan to drain the list
	 anyway.  */
      while (DF_REG_EQ_USE_CHAIN (regno))
	{
	  struct df_ref *eq_use = DF_REG_EQ_USE_CHAIN (regno);
	  rtx noted_insn = DF_REF_INSN (eq_use);
	  rtx note = find_reg_note (noted_insn, REG_EQUAL, NULL_RTX);
	  if (!note)
	    note = find_reg_note (noted_insn, REG_EQUIV, NULL_RTX);
	  gcc_assert (note);
	  remove_note (noted_insn, note);
	}
    }
}


/* Delete every instruction that hasn't been marked.  Clear the insn
   from DCE_DF if DF_DELETE is true.  */

static void
delete_unmarked_insns (void)
{
  basic_block bb;
  rtx insn, next;

  something_changed = false;
  FOR_EACH_BB (bb)
    FOR_BB_INSNS_SAFE (bb, insn, next)
      if (INSN_P (insn) 
	&& ((!marked_insn_p (insn)) || noop_move_p (insn))
        && dbg_cnt (new_dce))
	{
	  if (dump_file)
	    fprintf (dump_file, "DCE: Deleting insn %d\n", INSN_UID (insn));

          /* Before we delete the insn, we have to delete
             REG_EQUAL of the destination regs of the deleted insn
             to prevent dangling REG_EQUAL. */
          delete_corresponding_reg_eq_notes (insn);

	  /* XXX: This may need to be changed to delete_insn_and_edges */
	  delete_insn (insn);
	  something_changed = true;
	}
}


/* Mark all insns using DELETE_PARM in the libcall that contains
   START_INSN.  */
static void 
mark_libcall (rtx start_insn, bool delete_parm)
{
  rtx note = find_reg_note (start_insn, REG_LIBCALL_ID, NULL_RTX);
  int id = INTVAL (XEXP (note, 0));
  rtx insn;

  mark_insn (start_insn, delete_parm);
  insn = NEXT_INSN (start_insn);

  /* There are tales, long ago and far away, of the mystical nested
     libcall.  No one alive has actually seen one, but other parts of
     the compiler support them so we will here.  */
  for (insn = NEXT_INSN (start_insn); insn; insn = NEXT_INSN (insn))
    {
      if (INSN_P (insn))
	{
	  /* Stay in the loop as long as we are in any libcall.  */
	  if ((note = find_reg_note (insn, REG_LIBCALL_ID, NULL_RTX)))
	    {
	      if (id == INTVAL (XEXP (note, 0)))
		{
		  mark_insn (insn, delete_parm);
		  if (dump_file)
		    fprintf (dump_file, "matching forward libcall %d[%d]\n",
			     INSN_UID (insn), id);
		}
	    }
	  else 
	    break;
	}
    }
  
  for (insn = PREV_INSN (start_insn); insn; insn = PREV_INSN (insn))
    {
      if (INSN_P (insn))
	{
	  /* Stay in the loop as long as we are in any libcall.  */
	  if ((note = find_reg_note (insn, REG_LIBCALL_ID, NULL_RTX)))
	    {
	      if (id == INTVAL (XEXP (note, 0)))
		{
		  mark_insn (insn, delete_parm);
		  if (dump_file)
		    fprintf (dump_file, "matching backward libcall %d[%d]\n",
			     INSN_UID (insn), id);
		}
	    }
	  else 
	    break;
	}
    }
}


/* Go through the instructions and mark those whose necessity is not
   dependent on inter-instruction information.  Make sure all other
   instructions are not marked.  */

static void
prescan_insns_for_dce (void)
{
  basic_block bb;
  rtx insn;
  
  if (dump_file)
    fprintf (dump_file, "Finding needed instructions:\n");
  
  FOR_EACH_BB (bb)
    FOR_BB_INSNS (bb, insn)
    if (INSN_P (insn))
      {
	if (deletable_insn_p (insn))
	  mark_nonreg_stores (PATTERN (insn), insn, true);
	else
	  {
	    rtx note = find_reg_note (insn, REG_LIBCALL_ID, NULL_RTX);
	    if (note)
	      mark_libcall (insn, true);
	    else
	      mark_insn (insn, true);
	  }
      }

  if (dump_file)
    fprintf (dump_file, "Finished finding needed instructions:\n");
}


/* Free the data allocated by init_dce.  */

static void
end_dce (void)
{
  BITMAP_FREE (marked);
  df_in_progress = false;
}


/* -------------------------------------------------------------------------
   Code for handling register dependencies
   ------------------------------------------------------------------------- */


/* Mark instructions that define artifically-used registers, such as
   the frame pointer and the stack pointer.  */

static void
mark_artificial_uses (void)
{
  basic_block bb;
  struct df_link *defs;
  struct df_ref *use;

  FOR_ALL_BB (bb)
    {
      for (use = df_get_artificial_uses (bb->index); 
	   use; use = use->next_ref)
	for (defs = DF_REF_CHAIN (use); defs; defs = defs->next)
	  mark_insn (DF_REF_INSN (defs->ref), false);
    }
}

/* Mark every instruction that defines a register value that INSN uses.  */

static void
mark_reg_dependencies (rtx insn)
{
  struct df_link *defs;
  struct df_ref *use;

  /* If this is part of a libcall, mark the entire libcall.  */
  if (find_reg_note (insn, REG_LIBCALL_ID, NULL_RTX))
    mark_libcall (insn, false);

  for (use = DF_INSN_USES (insn); use; use = use->next_ref)
    {
      if (dump_file)
	{
	  fprintf (dump_file, "Processing use of ");
	  print_simple_rtl (dump_file, DF_REF_REG (use));
	  fprintf (dump_file, " in insn %d:\n", INSN_UID (insn));
	}
      for (defs = DF_REF_CHAIN (use); defs; defs = defs->next)
	mark_insn (DF_REF_INSN (defs->ref), false);
    }
}

/* -------------------------------------------------------------------------
   Fast DCE functions
   ------------------------------------------------------------------------- */


/* Free the data allocated by init_dce.  */

static void
end_fast_dce (void)
{
  BITMAP_FREE (marked);
  df_in_progress = false;
}


/* Process basic block BB.  Return true if the live_in set has
   changed.  */

static bool
dce_process_block (basic_block bb, bool redo_out)
{
  bitmap local_live = BITMAP_ALLOC (NULL);
  rtx insn;
  bool block_changed;
  struct df_ref *def, *use;
  unsigned int bb_index = bb->index;

  if (redo_out)
    {
      /* Need to redo the live_out set of this block if when one of
	 the succs of this block has had a change in it live in
	 set.  */
      edge e;
      edge_iterator ei;
      df_confluence_function_n con_fun_n = df_lr->problem->con_fun_n;
      FOR_EACH_EDGE (e, ei, bb->succs)
	(*con_fun_n) (e);
    }

  bitmap_copy (local_live, DF_LR_OUT (bb));

  /* Process the artificial defs and uses at the bottom of the block.  */
  for (def = df_get_artificial_defs (bb_index); 
       def; def = def->next_ref)
    if (((DF_REF_FLAGS (def) & DF_REF_AT_TOP) == 0)
	&& (!(DF_REF_FLAGS (def) & (DF_REF_PARTIAL | DF_REF_CONDITIONAL))))
      bitmap_clear_bit (local_live, DF_REF_REGNO (def));

  for (use = df_get_artificial_uses (bb_index); 
       use; use = use->next_ref)
    if ((DF_REF_FLAGS (use) & DF_REF_AT_TOP) == 0)
      bitmap_set_bit (local_live, DF_REF_REGNO (use));

  FOR_BB_INSNS_REVERSE (bb, insn)
    if (INSN_P (insn))
      {
	/* If this is a recursive call, the libcall will have already
	   been marked.  */
	if (!marked_insn_p (insn))
	  {	
	    bool needed = false;

	    for (def = DF_INSN_DEFS (insn); 
		 def; def = def->next_ref)
	      if (bitmap_bit_p (local_live, DF_REF_REGNO (def)))
		{
		  needed = true;
		  break;
		}
	    
	    if (needed)
	      {
		rtx note = find_reg_note (insn, REG_LIBCALL_ID, NULL_RTX);

		/* If we need to mark an insn in the middle of a
		   libcall, we need to back up to mark the entire
		   libcall.  Given that libcalls are rare, rescanning
		   the block should be a reasonable solution to trying
		   to figure out how to back up.  */
		if (note)
		  {
		    if (dump_file)
		      fprintf (dump_file, "needed libcall %d\n", INSN_UID (insn));
		    mark_libcall (insn, true);
		    BITMAP_FREE (local_live);
		    return dce_process_block (bb, false);
		  }
		else
		  mark_insn (insn, true);
	      }
	  }
	
	/* No matter if the instruction is needed or not, we remove
	   any regno in the defs from the live set.  */
	for (def = DF_INSN_DEFS (insn); def; def = def->next_ref)
	  {
	    unsigned int regno = DF_REF_REGNO (def);
	    if (!(DF_REF_FLAGS (def) & (DF_REF_PARTIAL | DF_REF_CONDITIONAL)))
	      bitmap_clear_bit (local_live, regno);
	  }
	if (marked_insn_p (insn))
	  for (use = DF_INSN_USES (insn); 
	       use; use = use->next_ref)
	    {
	      unsigned int regno = DF_REF_REGNO (use);
	      bitmap_set_bit (local_live, regno);
	    }
      }
  
  for (def = df_get_artificial_defs (bb_index); def; def = def->next_ref)
    if ((DF_REF_FLAGS (def) & DF_REF_AT_TOP)
	&& (!(DF_REF_FLAGS (def) & (DF_REF_PARTIAL | DF_REF_CONDITIONAL))))
      bitmap_clear_bit (local_live, DF_REF_REGNO (def));

#ifdef EH_USES
  /* Process the uses that are live into an exception handler.  */
  for (use = df_get_artificial_uses (bb_index); use; use = use->next_ref)
    /* Add use to set of uses in this BB.  */
    if (DF_REF_FLAGS (use) & DF_REF_AT_TOP)
      bitmap_set_bit (local_live, DF_REF_REGNO (use));
#endif

  block_changed = !bitmap_equal_p (local_live, DF_LR_IN (bb));
  if (block_changed)
    {
      BITMAP_FREE (DF_LR_IN (bb));
      DF_LR_IN (bb) = local_live;
    }
  else
    BITMAP_FREE (local_live);

  return block_changed;
}

static void
fast_dce (void)
{
  int *postorder = df_get_postorder ();
  int n_blocks = df_get_n_blocks ();
  int i;
  /* The set of blocks that have been seen on this iteration.  */
  bitmap processed = BITMAP_ALLOC (NULL);
  /* The set of blocks that will have to be rescaned because some
     instructions were deleted.  */
  bitmap changed = BITMAP_ALLOC (NULL);
  /* The set of blocks that need to have the out vectors reset because
     the in of one of their successors has changed.  */
  bitmap redo_out = BITMAP_ALLOC (NULL);
  bitmap all_blocks = BITMAP_ALLOC (NULL);
  bool global_changed = true;

  int loop_count = 0;

  prescan_insns_for_dce ();

  for (i = 0; i < n_blocks; i++)
    bitmap_set_bit (all_blocks, postorder[i]);

  while (global_changed)
    {
      global_changed = false;
      for (i = 0; i < n_blocks; i++)
	{
	  int index = postorder [i];
	  basic_block bb = BASIC_BLOCK (index);
	  bool local_changed 
	    = dce_process_block (bb, bitmap_bit_p (redo_out, index));
	  bitmap_set_bit (processed, index);
	  
	  if (local_changed)
	    {
	      edge e;
	      edge_iterator ei;
	      bitmap_set_bit (changed, index);
	      FOR_EACH_EDGE (e, ei, bb->preds)
		if (bitmap_bit_p (processed, e->src->index))
		  /* Be very tricky about when we need to iterate the
		     analysis.  We only have redo the analysis if we
		     delete an instrution from a block that used
		     something that was live on entry to the block and
		     we have already processed the pred of that block.

		     Since we are processing the blocks postorder,
		     that will only happen if the block is at the top
		     of a loop and the pred is inside the loop.  */
		  global_changed = true;
		else
		  bitmap_set_bit (redo_out, e->src->index);
	    }
	}
      
      if (global_changed)
	{
	  /* Turn off the RUN_DCE flag to prevent recursive calls to
	     dce.  */
	  int old_flag = df_clear_flags (DF_LR_RUN_DCE);

	  /* So something was deleted that requires a redo.  Do it on
	     the cheap.  */
	  delete_unmarked_insns ();
	  bitmap_clear (marked);
	  bitmap_clear (processed);
	  bitmap_clear (redo_out);
	  
	  /* We do not need to rescan any instructions.  We only need
	     to redo the dataflow equations for the blocks that had a
	     change at the top of the block.  Then we need to redo the
	     iteration.  */ 
	  df_analyze_problem (df_lr, all_blocks, changed, postorder, n_blocks, false);

	  if (old_flag & DF_LR_RUN_DCE)
	    df_set_flags (DF_LR_RUN_DCE);
	  bitmap_clear (changed);
	  prescan_insns_for_dce ();
	}
      loop_count++;
    }

  delete_unmarked_insns ();

  BITMAP_FREE (processed);
  BITMAP_FREE (changed);
  BITMAP_FREE (redo_out);
  BITMAP_FREE (all_blocks);
}


/* Callback for running pass_rtl_dce.  */

static unsigned int
rest_of_handle_fast_dce (void)
{
  init_dce (true);
  fast_dce ();
  end_fast_dce ();
  return 0;
}


/* This is an internal call that is used by the df live register
   problem to run fast dce as a side effect of creating the live
   information.  The stack is organized so that the lr problem is run,
   this pass is run, which updates the live info and the df scanning
   info, and then returns to allow the rest of the problems to be run.

   This can be called by elsewhere but it will not update the bit
   vectors for any other problems than LR.
*/

void
run_fast_df_dce (void)
{
  df_in_progress = true;
  init_dce (true);
  fast_dce ();
  BITMAP_FREE (marked);
  df_in_progress = false;
}


static bool
gate_fast_dce (void)
{
  return optimize > 0 && flag_new_dce;
}


/* Run a fast DCE pass and return true if any instructions were
   deleted.  */

bool
run_fast_dce (void)
{
  return gate_fast_dce () && (rest_of_handle_fast_dce (), something_changed);
}


struct tree_opt_pass pass_fast_rtl_dce =
{
  "dce",                                /* name */
  gate_fast_dce,                        /* gate */
  rest_of_handle_fast_dce,              /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_DCE,                               /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_df_finish |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'w'                                   /* letter */
};

/* -------------------------------------------------------------------------
   Code for handling store dependencies
   -------------------------------------------------------------------------

   This code eliminates simple store instructions if it can determine that
   the result of the stores is never needed.  We say that:

     - Instruction I "kills" store S if we know that instruction
       I unconditionally writes to every byte that store S does.

     - Store S "reaches" some point if we think the bytes written by
       store S might still be intact when instruction I is executed.

     - Instruction I "uses" store S if we think instruction I might
       depend on the result of store S.  (This implies that store S
       reaches instruction I.)

   In other words, "kills" is a "must" relationship whereas "reaches"
   and "uses" are "may" relationships.

   If an instruction in block B kills store S and has an LUID > I, let
   next(B,S,I) be the lowest such LUID, otherwise let it be higher than
   any valid LUID.  Let gen(S) be the LUID of the instruction that
   performs ("generates") store S.

   The instructions reached by store S in block B are then given by
   the following ranges:

     (a) (-1, next(B,S,-1)], if store S reaches the beginning of block B.

     (b) (gen(S), next(B,S,gen(S))], if block B generates store S.

   Range (a) requires global reaching information whereas range (b) only
   needs local information.  We can therefore handle two types of store:

     - Stores to non-invariant addresses that are killed later in the
       same basic block.  We only need to track range (b) for these stores.

     - Stores to invariant addresses that are killed at arbitrary
       points in the function.  We need to track ranges (a) and (b)
       for these stores.

   We use the "kills" information to compute the set of reaching store
   definitions, or "reaching stores" for short.  This is done in just
   the same way as for registers, with the store identifiers taking the
   place of register numbers.  The result is stored in RS_DFLOW.

   Instruction I uses store S if:

     (a) store S reaches instruction I, as determined above.
     (b) either:
	 (i) instruction I reads from unspecified memory locations; or
	 (ii) instruction I reads from a memory location that is
	      truly dependent on store S.

   We can then use this store information in much the same way as
   for registers.  The worklist algorithm itself is identical.  */

/* This structure stores information about a candidate store.  */
struct store_info {
  /* The target of the store (a MEM rtx).  */
  rtx mem;

  /* The instruction that performs the store.  */
  rtx insn;

  /* The basic block to which INSN belongs.  */
  int bb;

  /* INSN's df.c luid.  */
  int luid;
  
  /* The luid of the last instruction after INSN that this store
     reaches.  (This is next(B,S,gen(S)) in the commentary above.)  */
  int max_gen_luid;
};
typedef struct store_info store_info;

/* Vector definitions for the above.  */
DEF_VEC_O(store_info);
DEF_VEC_ALLOC_O(store_info,heap);

/* Each instance of this structure is attached to some store_base_info
   object SB and records that store STORE addresses bytes in the range
   [SB->BASE + BEGIN, SB->BASE + END).  */
struct store_offset_info {
  unsigned int store;
  HOST_WIDE_INT begin, end;
};
typedef struct store_offset_info store_offset_info;

/* Vector definitions for the above.  */
DEF_VEC_O(store_offset_info);
DEF_VEC_ALLOC_O(store_offset_info,heap);

/* Information about a base address.  Addresses are either function
   invariants or cselib values, depending on context.  */
union base_address {
  rtx invariant;
  cselib_val *value;
};

/* A structure for grouping together stores that share the same base address.
   STORES is a list of stores with base address BASE.  */
struct store_base_info {
  union base_address base;
  VEC(store_offset_info,heap) *stores;
};
typedef struct store_base_info store_base_info;

/* This structure holds a group of stores.  Each group has a separate
   identifier space.  */
struct store_group_info {
  /* A vector of all the stores in the group, indexed by store identifier.  */
  VEC(store_info,heap) *stores;
  
  /* A table of store_base_info structures, hashed by base value.  */
  htab_t bases;
};
typedef struct store_group_info store_group_info;

/* All candidate stores.  */
static store_group_info stores;

/* The identifiers of stores for which we haven't found a potential use.  */
static VEC(int,heap) *unmarked_stores;

/* Used while processing a basic block.  LOCAL_INVARIANT_STORES contains
   stores with invariant addresses; LOCAL_VALUE_STORES contains stores
   whose base addresses are cselib_vals.  */
static store_group_info local_invariant_stores, local_value_stores;

/* max_in_luid[B][S] is the luid of the last instruction in basic block B
   that is reached by the incoming value of store S.  It is -1 if store S
   does not reach the beginning of block B.  (When nonnegative, this is
   next(B,S,-1) in the commentary above.)  */
static int **max_in_luid;

/* Hashtable callbacks for maintaining the "bases" field of
   store_group_info, given that the addresses are function invariants.  */

static int
invariant_store_base_eq (const void *p1, const void *p2)
{
  const store_base_info *sb1 = (const store_base_info *) p1;
  const store_base_info *sb2 = (const store_base_info *) p2;
  return rtx_equal_p (sb1->base.invariant, sb2->base.invariant);
}

static hashval_t
invariant_store_base_hash (const void *p)
{
  const store_base_info *sb = (const store_base_info *) p;
  int do_not_record;
  return hash_rtx (sb->base.invariant, Pmode, &do_not_record, NULL, false);
}

/* Hashtable callbacks for maintaining the "bases" field of
   store_group_info, given that the addresses are cselib_vals.  */

static int
value_store_base_eq (const void *p1, const void *p2)
{
  const store_base_info *sb1 = (const store_base_info *) p1;
  const store_base_info *sb2 = (const store_base_info *) p2;
  return sb1->base.value == sb2->base.value;
}

static hashval_t
value_store_base_hash (const void *p)
{
  const store_base_info *sb = (const store_base_info *) p;
  return sb->base.value->value;
}

/* Hash table callback for the "bases" field of store_group_info.  */

static void
store_base_del (void *p)
{
  store_base_info *sb = (store_base_info *) p;
  VEC_free (store_offset_info, heap, sb->stores);
  XDELETE (sb);
}

static bitmap * in_vec;
static bitmap * out_vec;
static bitmap * gen_vec;
static bitmap * kill_vec;
static int * postorder;
static int n_blocks;
static bitmap iterating;
 

/* Functions used to compute the reaching stores sets.  */
static void 
rs_init (bitmap all_blocks)
{
  unsigned int bb_index;
  bitmap_iterator bi;

  EXECUTE_IF_SET_IN_BITMAP (all_blocks, 0, bb_index, bi)
    {
      bitmap_copy (out_vec[bb_index], gen_vec[bb_index]);
    }
}

static void
rs_confluence (edge e)
{
  bitmap op1 = in_vec[e->dest->index];
  bitmap op2 = out_vec[e->src->index];
  bitmap_ior_into (op1, op2);
}

static bool
rs_transfer_function (int bb_index)
{
  bitmap in = in_vec[bb_index];
  bitmap out = out_vec[bb_index];
  bitmap gen = gen_vec[bb_index];
  bitmap kill = kill_vec[bb_index];
  
  return bitmap_ior_and_compl (out, gen, in, kill);
}
 
/* Initialize store group *GROUP so that it can hold invariant addresses.  */
 
static void
init_invariant_store_group (store_group_info *group)
{
  group->stores = NULL;
  group->bases = htab_create (11, invariant_store_base_hash,
			      invariant_store_base_eq, store_base_del);
}

/* Initialize store group *GROUP so that it can hold cselib_val addresses.  */
 
static void
init_value_store_group (store_group_info *group)
{
  group->stores = NULL;
  group->bases = htab_create (11, value_store_base_hash,
			      value_store_base_eq, store_base_del);
}

/* Forget about all stores in *GROUP.  */

static void
empty_store_group (store_group_info *group)
{
  htab_empty (group->bases);
  VEC_truncate (store_info, group->stores, 0);
}

/* Free the memory used by store *GROUP.  */

static void
end_store_group (store_group_info *group)
{
  htab_delete (group->bases);
  VEC_free (store_info, heap, group->stores);
}

/* Initialize the data structures related to the global reaching store
   information.  STORES has already been initialized.  */


static void
init_rs_dflow (void)
{
  int i, bb;
  unsigned int j, num_stores;

  in_vec = XNEWVEC (bitmap, last_basic_block);
  out_vec = XNEWVEC (bitmap, last_basic_block);
  gen_vec = XNEWVEC (bitmap, last_basic_block);
  kill_vec = XNEWVEC (bitmap, last_basic_block);
  n_blocks = df_get_n_blocks ();
  postorder = df_get_postorder ();
  iterating = BITMAP_ALLOC (NULL);

  num_stores = VEC_length (store_info, stores.stores);
  max_in_luid = XCNEWVEC (int *, last_basic_block);

  for (i = 0; i < n_blocks; i++)
    {
      bb = postorder[i];

      in_vec[bb] = BITMAP_ALLOC (NULL);
      out_vec[bb] = BITMAP_ALLOC (NULL);
      gen_vec[bb] = BITMAP_ALLOC (NULL);
      kill_vec[bb] = BITMAP_ALLOC (NULL);
      bitmap_set_bit (iterating, bb);

      max_in_luid[bb] = XNEWVEC (int, num_stores);
      for (j = 0; j < num_stores; j++)
	max_in_luid[bb][j] = INT_MAX;
    }
}

/* Free the data allocated by init_rs_dflow.  */

static void
end_rs_dflow (void)
{
  int i, bb;

  for (i = 0; i < n_blocks; i++)
    {
      bb = postorder[i];

      XDELETE (max_in_luid[bb]);

      BITMAP_FREE (kill_vec[bb]);
      BITMAP_FREE (gen_vec[bb]);
      BITMAP_FREE (out_vec[bb]);
      BITMAP_FREE (in_vec[bb]);
    }

  XDELETE (max_in_luid);

  BITMAP_FREE (iterating);
  XDELETE (kill_vec);
  XDELETE (gen_vec);
  XDELETE (out_vec);
  XDELETE (in_vec);
}

/* Initialize UNMARKED_STORES so that it contains all the stores that
   have not yet been marked.  */

static void
init_unmarked_stores (void)
{
  store_info *store;
  unsigned int i;

  gcc_assert (unmarked_stores == NULL);
  for (i = 0; VEC_iterate (store_info, stores.stores, i, store); i++)
    if (!marked_insn_p (store->insn))
      VEC_safe_push (int, heap, unmarked_stores, i);
}

/* Free the memory used by UNMARKED_STORES.  */

static void
end_unmarked_stores (void)
{
  VEC_free (int, heap, unmarked_stores);
}

/* Initialize the data structures that DSE needs but normal DCE doesn't.  */

static void
init_dse (void)
{
  init_invariant_store_group (&stores);
}

/* Free the structures used by DSE.  */

static void
end_dse (void)
{
  end_store_group (&stores);
}

/* Print information about the store candidates to FILE.  */

static void
dump_stores (FILE *file)
{
  store_info *store;
  basic_block bb;
  rtx end;
  int i;

  for (i = 0; VEC_iterate (store_info, stores.stores, i, store); i++)
    {
      fprintf (file, "Store %d: ", i);
      print_simple_rtl (file, store->mem);
      if (store->max_gen_luid == INT_MAX)
	fprintf (file, " gen range (%d, end]\n", INSN_UID (store->insn));
      else
	{
	  end = store->insn;
	  while (DF_INSN_LUID (end) != store->max_gen_luid)
	    end = NEXT_INSN (end);
	  fprintf (file, " gen range (%d, %d]\n",
		   INSN_UID (store->insn), INSN_UID (end));
	}
    }

  FOR_ALL_BB (bb)
    {
      fprintf (file, "Block %d\n", bb->index);
      fprintf (file, "  in   : ");
      dump_bitmap (file, in_vec[bb->index]);
      fprintf (file, "  out  : ");
      dump_bitmap (file, out_vec[bb->index]);
      fprintf (file, "  gen  : ");
      dump_bitmap (file, gen_vec[bb->index]);
      fprintf (file, "  kill : ");
      dump_bitmap (file, kill_vec[bb->index]);
    }
}

/* Split address X into a base term and a constant offset, storing them
   in *BASE_OUT and *OFFSET_OUT respectively.  */

static void
split_address (rtx x, rtx *base_out, HOST_WIDE_INT *offset_out)
{
  *offset_out = 0;
  if (GET_CODE (x) == CONST)
    x = XEXP (x, 0);
  if (GET_CODE (x) == PLUS && GET_CODE (XEXP (x, 1)) == CONST_INT)
    {
      *offset_out = INTVAL (XEXP (x, 1));
      x = XEXP (x, 0);
    }
  *base_out = x;
}

/* Add a store_offset_info structure to GROUP, given that the next
   store to be added to it will span bytes [BASE + BEGIN, BASE + END).  */

static void
add_store_offset (store_group_info *group, union base_address *base,
		  HOST_WIDE_INT begin, HOST_WIDE_INT end)
{
  store_base_info *sb, tmp_sb;
  store_offset_info *so;
  void **slot;

  /* Find the store_base_info structure for BASE, creating a new one
     if necessary.  */
  tmp_sb.base = *base;
  slot = htab_find_slot (group->bases, &tmp_sb, INSERT);
  sb = (store_base_info *) *slot;
  if (sb == NULL)
    {
      *slot = sb = XNEW (store_base_info);
      sb->base = *base;
      sb->stores = NULL;
    }

  /* Add offset information to SB.  */
  so = VEC_safe_push (store_offset_info, heap, sb->stores, NULL);
  so->store = VEC_length (store_info, group->stores);
  so->begin = begin;
  so->end = end;
}

/* BODY is an instruction pattern that belongs to INSN.  Return true
   if it is a candidate store, adding it to the appropriate local
   store group if so.  */

static bool
record_store (rtx body, rtx insn)
{
  store_group_info *group;
  store_info *store;
  rtx mem;
  union base_address base;
  HOST_WIDE_INT offset;

  /* Check whether INSN sets a single value.  */
  if (GET_CODE (body) != SET)
    return false;

  /* ...and whether that value is a suitable memory location.  */
  mem = SET_DEST (body);
  if (!MEM_P (mem)
      || GET_MODE (mem) == BLKmode
      || MEM_VOLATILE_P (mem))
    return false;

  /* Split the address into canonical BASE + OFFSET terms.  */
  group = &local_invariant_stores;
  split_address (canon_rtx (XEXP (mem, 0)), &base.invariant, &offset);
  if (rtx_varies_p (base.invariant, false))
    {
      group = &local_value_stores;
      base.value = cselib_lookup (base.invariant, Pmode, true);
      if (base.value == NULL)
	return false;
    }

  /* Add a store_offset_info structure for the store.  */
  add_store_offset (group, &base, offset,
		    offset + GET_MODE_SIZE (GET_MODE (mem)));

  /* Record the store itself.  */
  store = VEC_safe_push (store_info, heap, group->stores, NULL);
  store->mem = mem;
  store->insn = insn;
  store->bb = BLOCK_NUM (insn);
  store->luid = DF_INSN_LUID (insn);
  store->max_gen_luid = INT_MAX;
  return true;
}

/* Apply record_store to all candidate stores in INSN.  Mark INSN
   if some part of it is not a candidate store and assigns to a
   non-register target.  */

static void
record_stores (rtx insn)
{
  rtx body;
  int i;

  body = PATTERN (insn);
  if (GET_CODE (body) == PARALLEL)
    for (i = 0; i < XVECLEN (body, 0); i++)
      {
	if (!record_store (XVECEXP (body, 0, i), insn))
	  mark_nonreg_stores (XVECEXP (body, 0, i), insn, false);
      }
  else
    if (!record_store (body, insn))
      mark_nonreg_stores (body, insn, false);
}

/* A qsort callback used to sort store_offset_info structures into
   ascending order of starting offset.  If two stores have the same
   offset, put those with higher identifiers first.  */

static int
store_offset_compare (const void *p1, const void *p2)
{
  const store_offset_info *offset1 = (const store_offset_info *) p1;
  const store_offset_info *offset2 = (const store_offset_info *) p2;
  if (offset1->begin - offset2->begin < 0)
    return -1;
  else if (offset1->begin - offset2->begin > 0)
    return 1;
  else
    return offset2->store - offset1->store;
}

/* *SLOT is a store_base_info structure that belongs to store group DATA.
   INVARIANT_P is true if all base addresses are function invariants; it is
   false if all addresses are cselib_vals.

   Work out the max_gen_luid of the stores, all of which are known to be
   in the same block.  Decide whether we can safely compute the reaching
   information for each store.  Add it to STORES if we can, otherwise
   mark it as needed.  Return true.  */

static inline int
store_base_local (void **slot, void *data, bool invariant_p)
{
  store_base_info *sb = (store_base_info *) *slot;
  store_group_info *group = (store_group_info *) data;
  store_offset_info *so;
  store_info *s, *storei, *storej;
  unsigned int i, j, length;

  /* Sort the stores into increasing offset order, putting later stores
     before earlier stores if they have the same offset.  */
  so = VEC_address (store_offset_info, sb->stores);
  length = VEC_length (store_offset_info, sb->stores);
  qsort (so, length, sizeof (store_offset_info), store_offset_compare);

  s = VEC_address (store_info, group->stores);
  for (j = 0; j < length; j++)
    {
      storej = &s[so[j].store];

      /* Find out whether STOREJ kills any earlier stores.  We can start
	 the search at J + 1 because stores with the same "begin" field
	 are ordered in decreasing order of "store" field, and thus will
	 be in descending order of luid.  */
      for (i = j + 1; i < length && so[i].begin < so[j].end; i++)
	{
	  storei = &s[so[i].store];
	  if (so[j].end >= so[i].end
	      && storej->luid > storei->luid
	      && storej->luid < storei->max_gen_luid)
	    storei->max_gen_luid = storej->luid;
	}

      if (invariant_p)
	{
	  /* An invariant address.  We can track STOREJ globally if reaches
	     the end of the block.  We will also want to know whether STOREJ
	     kills stores in other basic blocks.  */
	  add_store_offset (&stores, &sb->base, so[j].begin, so[j].end);
	  VEC_safe_push (store_info, heap, stores.stores, storej);
	}
      else
	{
	  /* A varying address.  We can't treat STOREJ as a candidate
	     store if it reaches the end of the block.  */
	  if (storej->max_gen_luid == INT_MAX)
	    mark_insn (storej->insn, false);
	  else
	    VEC_safe_push (store_info, heap, stores.stores, storej);
	}
    }
  return true;
}

/* Like store_base_local, but for invariant base addresses only.  */

static int
invariant_store_base_local (void **slot, void *data)
{
  return store_base_local (slot, data, true);
}

/* Like store_base_local, but for cselib_val base addresses only.  */

static int
value_store_base_local (void **slot, void *data)
{
  return store_base_local (slot, data, false);
}

/* Like value_store_base_local, but only handle values that have
   no elt_list_locs left.  Remove all such entries from the hash table.  */

static int
value_store_base_useless (void **slot, void *data)
{
  store_base_info *sb = (store_base_info *) *slot;
  store_group_info *group = (store_group_info *) data;

  if (sb->base.value->locs == NULL)
    {
      value_store_base_local (slot, data);
      htab_clear_slot (group->bases, slot);
    }
  return true;
}

/* Process and remove all entries in local_value_stores that cselib
   considers useless.  The associated cselib_vals are about to be freed.  */

static void
remove_useless_values (void)
{
  htab_traverse (local_value_stores.bases,
		 value_store_base_useless, &local_value_stores);
}

/* A htab_traverse callback in which *SLOT is a store_base_info structure
   attached to STORES.  Work out the values of gen_vec, kill_vec
   and max_in_luid.  */

static int
store_base_global (void **slot, void *data ATTRIBUTE_UNUSED)
{
  store_base_info *sb = (store_base_info *) *slot;
  store_offset_info *so;
  store_info *s, *storej;
  unsigned int starti, i, j, length;
  int *bb_max_in_luid;

  /* Sort the stores into increasing offset order.  */
  so = VEC_address (store_offset_info, sb->stores);
  length = VEC_length (store_offset_info, sb->stores);
  qsort (so, length, sizeof (store_offset_info), store_offset_compare);

  s = VEC_address (store_info, stores.stores);
  starti = 0;
  for (j = 0; j < length; j++)
    {
      storej = &s[so[j].store];
      bb_max_in_luid = max_in_luid[storej->bb];
      while (starti < length && so[starti].begin < so[j].begin)
	starti++;
      for (i = starti; i < length && so[i].begin < so[j].end; i++)
	if (so[j].end >= so[i].end)
	  {
	    if (storej->luid < bb_max_in_luid[so[i].store])
	      bb_max_in_luid[so[i].store] = storej->luid;
	    bitmap_set_bit ((bitmap) kill_vec[storej->bb], so[i].store);
	  }
      if (storej->max_gen_luid == INT_MAX)
	bitmap_set_bit ((bitmap) gen_vec[storej->bb], so[j].store);
    }

  return true;
}

/* Set max_in_luid[B][S] to -1 if store S doesn't reach the beginning of
   block B.  */

static void
finish_max_in_luid (void)
{
  bitmap_iterator bi;
  bitmap in;
  int i, bb;
  unsigned int j, start, num_stores;
  int *bb_max_in_luid;

  num_stores = VEC_length (store_info, stores.stores);
  for (i = 0; i < n_blocks; i++)
    {
      bb = postorder[i];
      start = 0;
      in = (bitmap) in_vec[bb];
      bb_max_in_luid = max_in_luid[bb];
      EXECUTE_IF_SET_IN_BITMAP (in, 0, j, bi)
	{
	  while (start < j)
	    bb_max_in_luid[start++] = -1;
	  start++;
	}
      while (start < num_stores)
	bb_max_in_luid[start++] = -1;
    }
}

/* Calculate the reaching stores information.  Called after all
   candidate stores have been added to STORES and after RS_DFLOW
   has been initialized.  */

static void
calculate_reaching_stores (void)
{
  htab_traverse (stores.bases, store_base_global, NULL);
  df_simple_iterative_dataflow (DF_FORWARD, rs_init, NULL, 
				rs_confluence, rs_transfer_function, 
				iterating, postorder, n_blocks);
  finish_max_in_luid ();
  if (dump_file)
    dump_stores (dump_file);
}

/* Return true if stores based on the frame pointer might be live even
   after the function has returned.  */

static bool
frame_stores_escape_p (void)
{
  if (TREE_CODE (TREE_TYPE (current_function_decl)) == FUNCTION_TYPE
      && TYPE_RETURNS_STACK_DEPRESSED (TREE_TYPE (current_function_decl)))
    return true;

  if (current_function_calls_eh_return)
    return true;

  return false;
}

/* A htab_traverse callback in which *SLOT is a global store_base_info
   structure and DATA is a bitmap containing all global stores that survive
   until the end of the function.  Remove any stores that do not survive
   after the function has returned.  */

static int
store_base_prune_needed (void **slot, void *data)
{
  store_base_info *sb = (store_base_info *) *slot;
  bitmap needed = (bitmap) data;
  store_offset_info *so;
  unsigned int i;

  if (sb->base.invariant == frame_pointer_rtx && !frame_stores_escape_p ())
    for (i = 0; VEC_iterate (store_offset_info, sb->stores, i, so); i++)
      bitmap_clear_bit (needed, so->store);

  return true;
}

/* Mark all global stores that might be used after the function has
   returned.  */

static void
mark_escaping_stores (void)
{
  bitmap needed;
  basic_block bb;
  bitmap_iterator bi;
  unsigned int i;

  /* Work out which global stores are live after the function has returned.  */
  needed = BITMAP_ALLOC (NULL);
  FOR_ALL_BB (bb)
    if (EDGE_COUNT (bb->succs) == 0)
      bitmap_ior_into (needed, (bitmap) out_vec[bb->index]);
  htab_traverse (stores.bases, store_base_prune_needed, needed);

  /* Mark all such stores as needed.  */
  if (dump_file)
    fprintf (dump_file, "Marking stores that are needed after"
	     " the function has exited:\n");
  EXECUTE_IF_SET_IN_BITMAP (needed, 0, i, bi)
    mark_insn (VEC_index (store_info, stores.stores, i)->insn, false);

  BITMAP_FREE (needed);
}

/* A for_each_rtx callback in which DATA is an rtx memory reference
   or pc_rtx.  In the former case, return true if *LOC has a true
   dependence on DATA; in the latter case, return true if *LOC is
   any kind of memory reference.  */

static int
insn_might_read_mem_rtx (rtx *loc, void *data)
{
  rtx mem = (rtx) data;
  if (*loc && MEM_P (*loc))
    {
      if (mem == pc_rtx)
	return 1;
      if (true_dependence (mem, VOIDmode, *loc, rtx_varies_p))
	return 1;
    }
  return 0;
}

/* A for_each_rtx callback in which DATA points to the same kind of rtx
   as insn_might_read_mem_rtx.  Nullify the pointer if i_m_r_m_r returns
   true for any part of *LOC.  */

static void
insn_might_read_mem_use (rtx *loc, void *data)
{
  rtx *mem_ptr = (rtx *) data;
  if (*mem_ptr && for_each_rtx (loc, insn_might_read_mem_rtx, *mem_ptr))
    *mem_ptr = NULL;
}

/* If MEM is pc_rtx, return true if INSN might read memory.
   Otherwise return true if INSN might have a true dependence
   on memory reference MEM.  */

static bool
insn_might_read_mem_p (rtx insn, rtx mem)
{
  /* We don't have enough information to tell how calls use memory.  */
  if (CALL_P (insn))
    return true;

  /* We can't predict how volatile instructions use memory.  */
  if (volatile_insn_p (PATTERN (insn)))
    return true;

  /* Look for an explicit memory read that has a true dependence on ID.  */
  note_uses (&PATTERN (insn), insn_might_read_mem_use, &mem);
  if (mem == NULL)
    return true;

  /* Look for a similar memory reference in the notes.  */
  if (for_each_rtx (&REG_NOTES (insn), insn_might_read_mem_rtx, mem))
    return true;

  return false;
}

/* Mark all stores that might be used by INSN.  */

static void
mark_dependent_stores (rtx insn)
{
  store_info *s, *store;
  int bb, luid, i, id, *bb_max_in_luid;

  if (VEC_length (int, unmarked_stores) == 0)
    return;

  /* Quick exit if INSN doesn't read memory at all.  */
  if (!insn_might_read_mem_p (insn, pc_rtx))
    return;

  if (dump_file)
    fprintf (dump_file, "Processing memory use in insn %d\n", INSN_UID (insn));

  /* Look at the local stores that haven't been marked yet.  */
  bb = BLOCK_NUM (insn);
  luid = DF_INSN_LUID (insn);
  bb_max_in_luid = max_in_luid[bb];
  s = VEC_address (store_info, stores.stores);
  for (i = 0; VEC_iterate (int, unmarked_stores, i, id); i++)
    {
      store = &s[id];
      if ((luid <= bb_max_in_luid[id]
	   || (bb == store->bb
	       && luid > store->luid
	       && luid <= store->max_gen_luid))
	  && insn_might_read_mem_p (insn, store->mem))
	{
	  mark_insn (store->insn, false);
	  VEC_unordered_remove (int, unmarked_stores, i);
	  i--;
	}
    }
}

/* -------------------------------------------------------------------------
   DSE pass functions
   ------------------------------------------------------------------------- */

/* Like prescan_insns_for_dce, but also record store candidates.  */

static void
prescan_insns_for_dse (void)
{
  basic_block bb;
  rtx insn;

  if (dump_file)
    fprintf (dump_file, "Finding stores and needed instructions:\n");

  cselib_init (false);
  init_invariant_store_group (&local_invariant_stores);
  init_value_store_group (&local_value_stores);
  FOR_EACH_BB (bb)
    {
      df_recompute_luids (bb);
      cselib_clear_table ();
      cselib_discard_hook = remove_useless_values;
      FOR_BB_INSNS (bb, insn)
	{
	  if (INSN_P (insn))
	    {
	      if (!deletable_insn_p (insn))
		{
		  rtx note = find_reg_note (insn, REG_LIBCALL_ID, NULL_RTX);
		  if (note)
		    mark_libcall (insn, false);
		  else
		    mark_insn (insn, false);
		}
	      else
		record_stores (insn);
	    }
	  cselib_process_insn (insn);
	}
      cselib_discard_hook = NULL;
      htab_traverse (local_invariant_stores.bases,
		     invariant_store_base_local, &local_invariant_stores);
      htab_traverse (local_value_stores.bases,
		     value_store_base_local, &local_value_stores);
      empty_store_group (&local_invariant_stores);
      empty_store_group (&local_value_stores);
    }
  end_store_group (&local_invariant_stores);
  end_store_group (&local_value_stores);
  cselib_finish ();
}

/* Callback for running pass_rtl_dse.  */

static unsigned int
rest_of_handle_dse (void)
{
  rtx insn;

  df_in_progress = false;
  init_dce (false);
  init_alias_analysis ();
  init_dse ();

  prescan_insns_for_dse ();
  if (stores.stores)
    {
      init_rs_dflow ();
      calculate_reaching_stores ();
      mark_escaping_stores ();
      init_unmarked_stores ();
    }
  mark_artificial_uses ();
  while (VEC_length (rtx, worklist) > 0)
    {
      insn = VEC_pop (rtx, worklist);
      mark_reg_dependencies (insn);
      mark_dependent_stores (insn);
    }
  /* Before any insns are deleted, we must remove the chains since
     they are not bidirectional.  */
  df_remove_problem (df_chain);
  delete_unmarked_insns ();
  if (stores.stores)
    {
      end_unmarked_stores ();
      end_rs_dflow ();
    }

  end_dse ();
  end_alias_analysis ();
  end_dce ();
  return 0;
}

static bool
gate_dse (void)
{
  return optimize > 0 && flag_new_dce;
}

struct tree_opt_pass pass_rtl_dse1 =
{
  "dse1",                               /* name */
  gate_dse,                             /* gate */
  rest_of_handle_dse,                   /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_DSE1,                              /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_df_finish |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'w'                                   /* letter */
};

struct tree_opt_pass pass_rtl_dse2 =
{
  "dse2",                               /* name */
  gate_dse,                             /* gate */
  rest_of_handle_dse,                   /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_DSE2,                              /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_df_finish |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'w'                                   /* letter */
};

struct tree_opt_pass pass_rtl_dse3 =
{
  "dse3",                               /* name */
  gate_dse,                             /* gate */
  rest_of_handle_dse,                   /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_DSE3,                              /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_df_finish |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'w'                                   /* letter */
};
