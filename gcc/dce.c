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

DEF_VEC_P(rtx);
DEF_VEC_ALLOC_P(rtx,heap);

DEF_VEC_I(int);
DEF_VEC_ALLOC_I(int,heap);

/* -------------------------------------------------------------------------
   Core mark/delete routines
   ------------------------------------------------------------------------- */

/* The data-flow information needed by this pass.  */
static struct df *dce_df;

/* True if we deleted at least one instruction.  */
static bool something_changed;

/* Instructions that have been marked but whose dependencies have not
   yet been processed.  */
static VEC(rtx,heap) *worklist;

static bitmap marked = NULL;

/* Initialize global variables for a new DCE pass.  */

static void
init_dce (void)
{
  dce_df = df_init (DF_HARD_REGS);
  df_chain_add_problem (dce_df, DF_UD_CHAIN);

  marked = BITMAP_ALLOC (NULL);
  df_analyze (dce_df);
  if (dump_file)
    df_dump (dce_df, dump_file);

}

/* Free the data allocated by init_dce.  */

static void
end_dce (void)
{
  BITMAP_FREE (marked);
  df_finish (dce_df);
  dce_df = NULL;

  /* People who call dce expect the core data flow to be updated.  */
  if (rtl_df)
    {
      df_rescan_blocks (rtl_df, NULL);
      df_analyze (rtl_df);
    }
}

/* Return true if INSN a normal instruction that can be deleted by the
   DCE pass.  */

static bool
deletable_insn_p (rtx insn)
{
  rtx x;

  switch (GET_CODE (PATTERN (insn)))
    {
    case USE:
    case TRAP_IF:
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
  return bitmap_bit_p (marked, INSN_UID (insn));
/*   return GET_MODE (insn) == SImode; */
}

static bool in_libcall = 0;
/* If INSN has not yet been marked as needed, mark it now, and add it to
   the worklist.  */

static void
mark_insn (rtx insn)
{
  if (!marked_insn_p (insn))
    {
      unsigned int id = INSN_UID (insn);
      VEC_safe_push (rtx, heap, worklist, insn);
      bitmap_set_bit (marked, INSN_UID (insn));
/*       PUT_MODE (insn, SImode); */
      if (dump_file)
	fprintf (dump_file, "  Adding insn %d to worklist\n",
		 id);

      /* The skeptical programmer may wonder what happens if one of
	 the middle instructions is the one that needs to be marked
	 live.  The answer is that this should never happen.  */
      if (!in_libcall && find_reg_note (insn, REG_LIBCALL, NULL_RTX))
	{
	  in_libcall = 1;
	  if (dump_file)
	    fprintf (dump_file, "Marking rest of libcall starting at insn %d\n", id);
	  while (!find_reg_note (insn, REG_RETVAL, NULL_RTX))
	    {
	      insn = NEXT_INSN (insn);
	      mark_insn (insn);
	    }
	  in_libcall = 0;
	}
      else if (!in_libcall && find_reg_note (insn, REG_RETVAL, NULL_RTX))
	{
	  in_libcall = 1;
	  if (dump_file)
	    fprintf (dump_file, "Marking rest of libcall ending at insn %d\n", id);
	  while (!find_reg_note (insn, REG_LIBCALL, NULL_RTX))
	    {
	      insn = PREV_INSN (insn);
	      mark_insn (insn);
	    }
	  in_libcall = 0;
	}
    }
}

/* A note_stores callback used by mark_nonreg_stores.  DATA is the
   instruction containing DEST.  */

static void
mark_nonreg_stores_1 (rtx dest, rtx pattern, void *data)
{
  if (GET_CODE (pattern) != CLOBBER && !REG_P (dest))

    mark_insn ((rtx) data);
}

/* Mark INSN if BODY stores to a non-register destination.  */

static void
mark_nonreg_stores (rtx body, rtx insn)
{
  note_stores (body, mark_nonreg_stores_1, insn);
}

/* Delete every instruction that hasn't been marked.  */

static void
delete_unmarked_insns (void)
{
  basic_block bb;
  rtx insn, next;

  something_changed = false;
  FOR_EACH_BB (bb)
    FOR_BB_INSNS_SAFE (bb, insn, next)
      if (INSN_P (insn) && !marked_insn_p (insn))
	{
	  if (dump_file)
	    fprintf (dump_file, "DCE: Deleting insn %d\n", INSN_UID (insn));
	  /* XXX: This may need to be changed to delete_insn_and_edges */
	  delete_insn (insn);
	  something_changed = true;
	}
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
      for (use = df_get_artificial_uses (dce_df, bb->index); 
	   use; use = use->next_ref)
	for (defs = DF_REF_CHAIN (use); defs; defs = defs->next)
	  mark_insn (DF_REF_INSN (defs->ref));
    }
}

/* Mark every instruction that defines a register value that INSN uses.  */

static void
mark_reg_dependencies (rtx insn)
{
  struct df_link *defs;
  struct df_ref * use;

  for (use = DF_INSN_USES (dce_df, insn); use; use = use->next_ref)
    {
      if (dump_file)
	{
	  fprintf (dump_file, "Processing use of ");
	  print_simple_rtl (dump_file, DF_REF_REG (use));
	  fprintf (dump_file, " in insn %d:\n", INSN_UID (insn));
	}
      for (defs = DF_REF_CHAIN (use); defs; defs = defs->next)
	mark_insn (DF_REF_INSN (defs->ref));
    }
}

/* -------------------------------------------------------------------------
   DCE pass functions
   ------------------------------------------------------------------------- */

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
	PUT_MODE (insn, VOIDmode);
	if (!deletable_insn_p (insn))
	  mark_insn (insn);
	else
	  mark_nonreg_stores (PATTERN (insn), insn);
      }
}

/* Callback for running pass_rtl_dce.  */

static void
rest_of_handle_dce (void)
{
  init_dce ();
  
  prescan_insns_for_dce ();
  mark_artificial_uses ();
  while (VEC_length (rtx, worklist) > 0)
    mark_reg_dependencies (VEC_pop (rtx, worklist));
  delete_unmarked_insns ();
  
  end_dce ();
}

static bool
gate_dce (void)
{
  return optimize > 0 && flag_new_dce;
}

/* Run a DCE pass and return true if any instructions were deleted.  */

bool
run_dce (void)
{
  return gate_dce () && (rest_of_handle_dce (), something_changed);
}

struct tree_opt_pass pass_rtl_dce =
{
  "dce",                                /* name */
  gate_dce,                             /* gate */
  rest_of_handle_dce,                   /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_DCE,                               /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
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

/* A structure for grouping together stores that share the same base address.
   STORES is a list of stores with base address BASE.  */
struct store_base_info {
  rtx base;
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

/* Used to store the in, out, gen and kill sets of reaching stores.
   Bit S of in_vec[B] is set if store S reaches the beginning
   of block B, etc.  */
static struct dataflow rs_dflow;

/* max_in_luid[B][S] is the luid of the last instruction in basic block B
   that is reached by the incoming value of store S.  It is -1 if store S
   does not reach the beginning of block B.  (When nonnegative, this is
   next(B,S,-1) in the commentary above.)  */
static int **max_in_luid;

/* Hashtable callbacks for maintaining the "bases" field of
   store_group_info.  */

static int
store_base_eq (const void *p1, const void *p2)
{
  const store_base_info *sb1 = (const store_base_info *) p1;
  const store_base_info *sb2 = (const store_base_info *) p2;
  return (GET_CODE (sb1->base) == VALUE
	  ? sb1->base == sb2->base
	  : rtx_equal_p (sb1->base, sb2->base));
}

static hashval_t
store_base_hash (const void *p)
  {
  const store_base_info *sb = (const store_base_info *) p;
  int do_not_record;
  return (GET_CODE (sb->base) == VALUE
	  ? htab_hash_pointer (sb->base)
	  : hash_rtx (sb->base, Pmode, &do_not_record, NULL, false));
}

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
rs_init (struct dataflow * dflow ATTRIBUTE_UNUSED, bitmap all_blocks)
{
  unsigned int bb_index;
  bitmap_iterator bi;

  EXECUTE_IF_SET_IN_BITMAP (all_blocks, 0, bb_index, bi)
    {
      bitmap_copy (out_vec[bb_index], gen_vec[bb_index]);
    }
}

static void
rs_confluence (struct dataflow * dflow ATTRIBUTE_UNUSED, edge e)
{
  bitmap op1 = in_vec[e->dest->index];
  bitmap op2 = out_vec[e->src->index];
  bitmap_ior_into (op1, op2);
}

static bool
rs_transfer_function (struct dataflow * dflow ATTRIBUTE_UNUSED, int bb_index)
{
  bitmap in = in_vec[bb_index];
  bitmap out = out_vec[bb_index];
  bitmap gen = gen_vec[bb_index];
  bitmap kill = kill_vec[bb_index];
  
  return bitmap_ior_and_compl (out, gen, in, kill);
}
 
/* The problem to be solved by rs_dflow.  */

static struct df_problem reaching_stores_problem =
{
  0,                          /* Problem id.  */
  DF_FORWARD,                 /* Direction.  */
  NULL,                       /* Allocate the problem specific data.  */
  NULL,                       /* Reset global information.  */
  NULL,                       /* Delete the basic block data.  */
  NULL,                       /* Local compute function.  */
  rs_init,                    /* Init the solution specific data.  */
  df_iterative_dataflow,      /* Iterative solver.  */
  NULL,                       /* Confluence operator 0.  */ 
  rs_confluence,              /* Confluence operator n.  */ 
  rs_transfer_function,       /* Transfer function.  */
  NULL,                       /* Finalize function.  */
  NULL,                       /* Free all of the problem information.  */
  NULL,                       /* Debugging.  */
  NULL                        /* Dependent problem.  */
};

/* Initialize store group *GROUP.  */
 
 static void
   init_store_group (store_group_info *group)
{
  group->stores = NULL;
  group->bases = htab_create (11, store_base_hash,
			      store_base_eq, store_base_del);
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
  postorder = XNEWVEC (int, last_basic_block);
  n_blocks = post_order_compute (postorder, true);
  iterating = BITMAP_ALLOC (NULL);

  rs_dflow.problem = &reaching_stores_problem;

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
  XDELETE (postorder);
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
  init_store_group (&stores);
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
	  while (DF_INSN_LUID (dce_df, end) != store->max_gen_luid)
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

/* Try to convert the address of memory reference MEM into a canonical
   base + offset expression.  Return true on success, storing the two
   components in *BASE_OUT and *OFFSET_OUT respectively.  *BASE_OUT
   will be either a nonvarying address or a VALUE rtx.  */

static bool
get_canonical_address (rtx mem, rtx *base_out, HOST_WIDE_INT *offset_out)
{
  cselib_val *val;

  split_address (canon_rtx (XEXP (mem, 0)), base_out, offset_out);
  if (!rtx_varies_p (*base_out, false))
    return true;

  val = cselib_lookup (*base_out, Pmode, true);
  if (val == NULL)
    return false;

  *base_out = val->u.val_rtx;
  return true;
}

/* Add a store_offset_info structure to GROUP, given that the next
   store to be added to it will span bytes [BASE + BEGIN, BASE + END).  */

static void
add_store_offset (store_group_info *group, rtx base,
		  HOST_WIDE_INT begin, HOST_WIDE_INT end)
{
  store_base_info *sb, tmp_sb;
  store_offset_info *so;
  void **slot;

  /* Find the store_base_info structure for BASE, creating a new one
     if necessary.  */
  tmp_sb.base = base;
  slot = htab_find_slot (group->bases, &tmp_sb, INSERT);
  sb = (store_base_info *) *slot;
  if (sb == NULL)
    {
      *slot = sb = XNEW (store_base_info);
      sb->base = base;
      sb->stores = NULL;
    }

  /* Add offset information to SB.  */
  so = VEC_safe_push (store_offset_info, heap, sb->stores, NULL);
  so->store = VEC_length (store_info, group->stores);
  so->begin = begin;
  so->end = end;
}

/* BODY is an instruction pattern that belongs to INSN.  Return true
   if it is a candidate store, adding it to GROUP if so.  */

static bool
record_store (store_group_info *group, rtx body, rtx insn)
{
  store_info *store;
  rtx base, mem;
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
  if (!get_canonical_address (mem, &base, &offset))
    return false;

  /* Add a store_offset_info structure for the sture.  */
  add_store_offset (group, base, offset,
		    offset + GET_MODE_SIZE (GET_MODE (mem)));

  /* Record the store itself.  */
  store = VEC_safe_push (store_info, heap, group->stores, NULL);
  store->mem = mem;
  store->insn = insn;
  store->bb = BLOCK_NUM (insn);
  store->luid = DF_INSN_LUID (dce_df, insn);
  store->max_gen_luid = INT_MAX;
  return true;
}

/* Add all candidate stores in INSN to GROUP.  Mark INSN if some part
   of it is not a candidate store and assigns to a non-register target.  */

static void
record_stores (store_group_info *group, rtx insn)
{
  rtx body;
  int i;

  body = PATTERN (insn);
  if (GET_CODE (body) == PARALLEL)
    for (i = 0; i < XVECLEN (body, 0); i++)
      {
	if (!record_store (group, XVECEXP (body, 0, i), insn))
	  mark_nonreg_stores (XVECEXP (body, 0, i), insn);
      }
  else
    if (!record_store (group, body, insn))
      mark_nonreg_stores (body, insn);
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

/* A htab_traverse callback in which *SLOT is a store_base_info structure
   that belongs to store group DATA.  Work out the max_gen_luid of the
   stores, all of which are known to be in the same block.  Decide whether
   we can safely compute the reaching information for each store.
   Add it to STORES if we can, otherwise mark it as needed.  */

static int
store_base_local (void **slot, void *data)
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

      if (GET_CODE (sb->base) != VALUE)
	{
	  /* An invariant address.  We can track STOREJ globally if reaches
	     the end of the block.  We will also want to know whether STOREJ
	     kills stores in other basic blocks.  */
	  add_store_offset (&stores, sb->base, so[j].begin, so[j].end);
	  VEC_safe_push (store_info, heap, stores.stores, storej);
	}
      else
	{
	  /* A varying address.  We can't treat STOREJ as a candidate
	     store if it reaches the end of the block.  */
	  if (storej->max_gen_luid == INT_MAX)
	    mark_insn (storej->insn);
	  else
	    VEC_safe_push (store_info, heap, stores.stores, storej);
	}
    }
  return true;
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
  df_iterative_dataflow (&rs_dflow, iterating, iterating, 
			 postorder, n_blocks, false);
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

  if (sb->base == frame_pointer_rtx && !frame_stores_escape_p ())
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
    mark_insn (VEC_index (store_info, stores.stores, i)->insn);

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
  luid = DF_INSN_LUID (dce_df, insn);
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
	  mark_insn (store->insn);
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
  store_group_info group;
  basic_block bb;
  rtx insn;

  if (dump_file)
    fprintf (dump_file, "Finding stores and needed instructions:\n");

  /* If we haven't added a store to this group yet, initialize it now.  */
  cselib_init (false);
  init_store_group (&group);
  FOR_EACH_BB (bb)
    {
      cselib_clear_table ();
      FOR_BB_INSNS (bb, insn)
	{
	  if (INSN_P (insn))
	    {
	      PUT_MODE (insn, VOIDmode);
	      if (!deletable_insn_p (insn))
		mark_insn (insn);
	      else
		record_stores (&group, insn);
	    }
	  cselib_process_insn (insn);
	}
      htab_traverse (group.bases, store_base_local, &group);
      empty_store_group (&group);
    }
  end_store_group (&group);
  cselib_finish ();
}

/* Callback for running pass_rtl_dse.  */

static void
rest_of_handle_dse (void)
{
  rtx insn;

  init_dce ();
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
  delete_unmarked_insns ();
  if (stores.stores)
    {
      end_unmarked_stores ();
      end_rs_dflow ();
    }

  end_dse ();
  end_alias_analysis ();
  end_dce ();
}

static bool
gate_dse (void)
{
  return optimize > 0 && flag_new_dce;
}

struct tree_opt_pass pass_rtl_dse =
{
  "dse",                                /* name */
  gate_dse,                             /* gate */
  rest_of_handle_dse,                   /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_DSE,                               /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'w'                                   /* letter */
};
