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

/* Array of implicit set patterns indexed by basic block index.  */
rtx *implicit_sets;

/* Hash tables.  */
static void compute_can_copy		PARAMS ((char []));
static int oprs_unchanged_p		PARAMS ((rtx, rtx, int, basic_block, struct reg_avail_info *));
static int oprs_anticipatable_p 	PARAMS ((rtx, rtx, basic_block, struct reg_avail_info *));
static int oprs_available_p		PARAMS ((rtx, rtx, basic_block, struct reg_avail_info *));
static void record_last_set_info 	PARAMS ((rtx, rtx, void *));
static void record_last_reg_set_info 	PARAMS ((rtx, int, basic_block, struct reg_avail_info *));
static void hash_scan_insn		PARAMS ((rtx, struct hash_table *,
					  int, basic_block, struct reg_avail_info *));
static void hash_scan_set		PARAMS ((rtx, rtx, struct hash_table *,
					  basic_block, struct reg_avail_info *));
static void hash_scan_clobber		PARAMS ((rtx, rtx, struct hash_table *));
static void hash_scan_call		PARAMS ((rtx, rtx, struct hash_table *));
static void insert_expr_in_table 	PARAMS ((rtx, enum machine_mode, rtx,
					  int, int, struct hash_table *));
static void insert_set_in_table 	PARAMS ((rtx, rtx, struct hash_table *));
static unsigned int hash_expr		PARAMS ((rtx, enum machine_mode, int *, int));
static unsigned int hash_expr_1		PARAMS ((rtx, enum machine_mode, int *));
static unsigned int hash_string_1	PARAMS ((const char *));
static unsigned int hash_set		PARAMS ((int, int));
static void compute_hash_table_work	PARAMS ((struct hash_table *));
static void canon_list_insert		PARAMS ((rtx, rtx, void *));
static void mems_conflict_for_gcse_p	PARAMS ((rtx, rtx, void *));
static int load_killed_in_block_p	PARAMS ((basic_block, int, rtx, int));

/* Local properties computation.  */
static void compute_transp		PARAMS ((rtx, int, sbitmap *, int));

/* Operands checking.  */
static void mark_call			PARAMS ((rtx));
static void mark_set			PARAMS ((rtx, rtx));
static void mark_clobber		PARAMS ((rtx, rtx));

/* Modify mem tables.  */
static void free_insn_expr_list_list	PARAMS ((rtx *));

/* Insn manipulation.  */
static void add_label_notes	PARAMS ((rtx, rtx));

/* Memory allocation.  */

/* Cover function to xmalloc to record bytes allocated.  */

char *
gmalloc (size)
     unsigned int size;
{
  bytes_used += size;
  return xmalloc (size);
}

/* Cover function to xrealloc.
   We don't record the additional size since we don't know it.
   It won't affect memory usage stats much anyway.  */

char *
grealloc (ptr, size)
     char *ptr;
     unsigned int size;
{
  return xrealloc (ptr, size);
}

/* Cover function to obstack_alloc.  */

char *
gcse_alloc (size)
     unsigned long size;
{
  bytes_used += size;
  return (char *) obstack_alloc (&gcse_obstack, size);
}

/* Hash tables -- static.  */

/* Used for communication between mems_conflict_for_gcse_p and
   load_killed_in_block_p.  Nonzero if mems_conflict_for_gcse_p finds a
   conflict between two memory references.  */
static int gcse_mems_conflict_p;

/* Used for communication between mems_conflict_for_gcse_p and
   load_killed_in_block_p.  A memory reference for a load instruction,
   mems_conflict_for_gcse_p will see if a memory store conflicts with
   this memory load.  */
static rtx gcse_mem_operand;

/* DEST is the output of an instruction.  If it is a memory reference, and
   possibly conflicts with the load found in gcse_mem_operand, then set
   gcse_mems_conflict_p to a nonzero value.  */

static void
mems_conflict_for_gcse_p (dest, setter, data)
     rtx dest, setter ATTRIBUTE_UNUSED;
     void *data ATTRIBUTE_UNUSED;
{
  while (GET_CODE (dest) == SUBREG
	 || GET_CODE (dest) == ZERO_EXTRACT
	 || GET_CODE (dest) == SIGN_EXTRACT
	 || GET_CODE (dest) == STRICT_LOW_PART)
    dest = XEXP (dest, 0);

  /* If DEST is not a MEM, then it will not conflict with the load.  Note
     that function calls are assumed to clobber memory, but are handled
     elsewhere.  */
  if (GET_CODE (dest) != MEM)
    return;

  /* If we are setting a MEM in our list of specially recognized MEMs,
     don't mark as killed this time.  */

  if (dest == gcse_mem_operand && pre_ldst_mems != NULL)
    {
      if (!find_rtx_in_ldst (dest))
	gcse_mems_conflict_p = 1;
      return;
    }

  if (true_dependence (dest, GET_MODE (dest), gcse_mem_operand,
		       rtx_addr_varies_p))
    gcse_mems_conflict_p = 1;
}

/* Return nonzero if the expression in X (a memory reference) is killed
   in block BB before or after the insn with the CUID in UID_LIMIT.
   AVAIL_P is nonzero for kills after UID_LIMIT, and zero for kills
   before UID_LIMIT.

   To check the entire block, set UID_LIMIT to max_uid + 1 and
   AVAIL_P to 0.  */

static int
load_killed_in_block_p (bb, uid_limit, x, avail_p)
     basic_block bb;
     int uid_limit;
     rtx x;
     int avail_p;
{
  rtx list_entry = modify_mem_list[bb->index];
  while (list_entry)
    {
      rtx setter;
      /* Ignore entries in the list that do not apply.  */
      if ((avail_p
	   && INSN_CUID (XEXP (list_entry, 0)) < uid_limit)
	  || (! avail_p
	      && INSN_CUID (XEXP (list_entry, 0)) > uid_limit))
	{
	  list_entry = XEXP (list_entry, 1);
	  continue;
	}

      setter = XEXP (list_entry, 0);

      /* If SETTER is a call everything is clobbered.  Note that calls
	 to pure functions are never put on the list, so we need not
	 worry about them.  */
      if (GET_CODE (setter) == CALL_INSN)
	return 1;

      /* SETTER must be an INSN of some kind that sets memory.  Call
	 note_stores to examine each hunk of memory that is modified.

	 The note_stores interface is pretty limited, so we have to
	 communicate via global variables.  Yuk.  */
      gcse_mem_operand = x;
      gcse_mems_conflict_p = 0;
      note_stores (PATTERN (setter), mems_conflict_for_gcse_p, NULL);
      if (gcse_mems_conflict_p)
	return 1;
      list_entry = XEXP (list_entry, 1);
    }
  return 0;
}

/* Return non-zero if the operands of expression X are unchanged from
   the start of INSN's basic block up to but not including INSN.  */

static int
oprs_anticipatable_p (x, insn, current_bb, reg_avail_info)
     rtx x, insn;
     basic_block current_bb;
     struct reg_avail_info *reg_avail_info;
{
  return oprs_unchanged_p (x, insn, 0, current_bb, reg_avail_info);
}

/* Return non-zero if the operands of expression X are unchanged from
   INSN to the end of INSN's basic block.  */

static int
oprs_available_p (x, insn, current_bb, reg_avail_info)
     rtx x, insn;
     basic_block current_bb;
     struct reg_avail_info *reg_avail_info;
{
  return oprs_unchanged_p (x, insn, 1, current_bb, reg_avail_info);
}

/* Return non-zero if the operands of expression X are unchanged from the
   start of INSN's basic block up to but not including INSN (if AVAIL_P == 0),
   or from INSN to the end of INSN's basic block (if AVAIL_P != 0).  */

static int
oprs_unchanged_p (x, insn, avail_p, current_bb, reg_avail_info)
     rtx x, insn;
     int avail_p;
     basic_block current_bb;
     struct reg_avail_info *reg_avail_info;
{
  int i, j;
  enum rtx_code code;
  const char *fmt;

  if (x == 0)
    return 1;

  code = GET_CODE (x);
  switch (code)
    {
    case REG:
      {
	struct reg_avail_info *info = &reg_avail_info[REGNO (x)];

	if (info->last_bb != current_bb)
	  return 1;
	if (avail_p)
	  return info->last_set < INSN_CUID (insn);
	else
	  return info->first_set >= INSN_CUID (insn);
      }

    case MEM:
      if (load_killed_in_block_p (current_bb, INSN_CUID (insn),
				  x, avail_p))
	return 0;
      else
	return oprs_unchanged_p (XEXP (x, 0), insn, avail_p, current_bb, reg_avail_info);

    case PRE_DEC:
    case PRE_INC:
    case POST_DEC:
    case POST_INC:
    case PRE_MODIFY:
    case POST_MODIFY:
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

  for (i = GET_RTX_LENGTH (code) - 1, fmt = GET_RTX_FORMAT (code); i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  /* If we are about to do the last recursive call needed at this
	     level, change it into iteration.  This function is called enough
	     to be worth it.  */
	  if (i == 0)
	    return oprs_unchanged_p (XEXP (x, i), insn, avail_p, current_bb, reg_avail_info);

	  else if (! oprs_unchanged_p (XEXP (x, i), insn, avail_p, current_bb, reg_avail_info))
	    return 0;
	}
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  if (! oprs_unchanged_p (XVECEXP (x, i, j), insn, avail_p, current_bb, reg_avail_info))
	    return 0;
    }

  return 1;
}

/* Compute which modes support reg/reg copy operations.  */

static void
compute_can_copy (can_copy_p)
     char can_copy_p[];
{
  int i;
#ifndef AVOID_CCMODE_COPIES
  rtx reg, insn;
#endif
  memset (can_copy_p, 0, NUM_MACHINE_MODES);

  start_sequence ();
  for (i = 0; i < NUM_MACHINE_MODES; i++)
    if (GET_MODE_CLASS (i) == MODE_CC)
      {
#ifdef AVOID_CCMODE_COPIES
	can_copy_p[i] = 0;
#else
	reg = gen_rtx_REG ((enum machine_mode) i, LAST_VIRTUAL_REGISTER + 1);
	insn = emit_insn (gen_rtx_SET (VOIDmode, reg, reg));
	if (recog (PATTERN (insn), insn, NULL) >= 0)
	  can_copy_p[i] = 1;
#endif
      }
    else
      can_copy_p[i] = 1;

  end_sequence ();
}

/* Called from compute_hash_table via note_stores to handle one
   SET or CLOBBER in an insn.  DATA is really the instruction in which
   the SET is taking place.  */

static basic_block rlsi_current_bb;
static struct reg_avail_info *rlsi_reg_avail_info;
static void
record_last_set_info (dest, setter, data)
     rtx dest, setter ATTRIBUTE_UNUSED;
     void *data;
{
  rtx last_set_insn = (rtx) data;

  if (GET_CODE (dest) == SUBREG)
    dest = SUBREG_REG (dest);

  if (GET_CODE (dest) == REG)
    record_last_reg_set_info (last_set_insn, REGNO (dest), rlsi_current_bb, rlsi_reg_avail_info);
  else if (GET_CODE (dest) == MEM
	   /* Ignore pushes, they clobber nothing.  */
	   && ! push_operand (dest, GET_MODE (dest)))
    record_last_mem_set_info (last_set_insn);
}

/* Record register first/last/block set information for REGNO in INSN.

   first_set records the first place in the block where the register
   is set and is used to compute "anticipatability".

   last_set records the last place in the block where the register
   is set and is used to compute "availability".

   last_bb records the block for which first_set and last_set are
   valid, as a quick test to invalidate them.

   reg_set_in_block records whether the register is set in the block
   and is used to compute "transparency".  */

static void
record_last_reg_set_info (insn, regno, current_bb, reg_avail_info)
     rtx insn;
     int regno;
     basic_block current_bb;
     struct reg_avail_info *reg_avail_info;
{
  struct reg_avail_info *info = &reg_avail_info[regno];
  int cuid = INSN_CUID (insn);

  info->last_set = cuid;
  if (info->last_bb != current_bb)
    {
      info->last_bb = current_bb;
      info->first_set = cuid;
      SET_BIT (reg_set_in_block[current_bb->index], regno);
    }
}

/* Process INSN and add hash table entries as appropriate.

   Only available expressions that set a single pseudo-reg are recorded.

   Single sets in a PARALLEL could be handled, but it's an extra complication
   that isn't dealt with right now.  The trick is handling the CLOBBERs that
   are also in the PARALLEL.  Later.

   If SET_P is non-zero, this is for the assignment hash table,
   otherwise it is for the expression hash table.
   If IN_LIBCALL_BLOCK nonzero, we are in a libcall block, and should
   not record any expressions.  */

static void
hash_scan_insn (insn, table, in_libcall_block, current_bb, reg_avail_info)
     rtx insn;
     struct hash_table *table;
     int in_libcall_block;
     basic_block current_bb;
     struct reg_avail_info *reg_avail_info;
{
  rtx pat = PATTERN (insn);
  int i;

  if (in_libcall_block)
    return;

  /* Pick out the sets of INSN and for other forms of instructions record
     what's been modified.  */

  if (GET_CODE (pat) == SET)
    hash_scan_set (pat, insn, table, current_bb, reg_avail_info);
  else if (GET_CODE (pat) == PARALLEL)
    for (i = 0; i < XVECLEN (pat, 0); i++)
      {
	rtx x = XVECEXP (pat, 0, i);

	if (GET_CODE (x) == SET)
	  hash_scan_set (x, insn, table, current_bb, reg_avail_info);
	else if (GET_CODE (x) == CLOBBER)
	  hash_scan_clobber (x, insn, table);
	else if (GET_CODE (x) == CALL)
	  hash_scan_call (x, insn, table);
      }

  else if (GET_CODE (pat) == CLOBBER)
    hash_scan_clobber (pat, insn, table);
  else if (GET_CODE (pat) == CALL)
    hash_scan_call (pat, insn, table);
}

/* Scan pattern PAT of INSN and add an entry to the hash TABLE (set or
   expression one).  */

static void
hash_scan_set (pat, insn, table, current_bb, reg_avail_info)
     rtx pat, insn;
     struct hash_table *table;
     basic_block current_bb;
     struct reg_avail_info *reg_avail_info;
{
  /* Non-zero for each mode that supports (set (reg) (reg)).
     This is trivially true for integer and floating point values.
     It may or may not be true for condition codes.  */
  static char can_copy_p[(int) NUM_MACHINE_MODES];

  /* Non-zero if can_copy_p has been initialized.  */
  static int can_copy_init_p;

  rtx src = SET_SRC (pat);
  rtx dest = SET_DEST (pat);
  rtx note;

  /* See what modes support reg/reg copy operations.  */
  if (! can_copy_init_p)
    {
      compute_can_copy (can_copy_p);
      can_copy_init_p = 1;
    }

  if (GET_CODE (src) == CALL)
    hash_scan_call (src, insn, table);

  else if (GET_CODE (dest) == REG)
    {
      unsigned int regno = REGNO (dest);
      rtx tmp;

      /* If this is a single set and we are doing constant propagation,
	 see if a REG_NOTE shows this equivalent to a constant.  */
      if (table->set_p && (note = find_reg_equal_equiv_note (insn)) != 0
	  && CONSTANT_P (XEXP (note, 0)))
	src = XEXP (note, 0), pat = gen_rtx_SET (VOIDmode, dest, src);

      /* Only record sets of pseudo-regs in the hash table.  */
      if (! table->set_p
	  && regno >= FIRST_PSEUDO_REGISTER
	  /* Don't GCSE something if we can't do a reg/reg copy.  */
	  && can_copy_p [GET_MODE (dest)]
	  /* GCSE commonly inserts instruction after the insn.  We can't
	     do that easily for EH_REGION notes so disable GCSE on these
	     for now.  */
	  && !find_reg_note (insn, REG_EH_REGION, NULL_RTX)
	  /* Is SET_SRC something we want to gcse?  */
	  && want_to_gcse_p (src)
	  /* Don't CSE a nop.  */
	  && ! set_noop_p (pat)
	  /* Don't GCSE if it has attached REG_EQUIV note.
	     At this point this only function parameters should have
	     REG_EQUIV notes and if the argument slot is used somewhere
	     explicitly, it means address of parameter has been taken,
	     so we should not extend the lifetime of the pseudo.  */
	  && ((note = find_reg_note (insn, REG_EQUIV, NULL_RTX)) == 0
	      || GET_CODE (XEXP (note, 0)) != MEM))
	{
	  /* An expression is not anticipatable if its operands are
	     modified before this insn or if this is not the only SET in
	     this insn.  */
	  int antic_p = oprs_anticipatable_p (src, insn, current_bb, reg_avail_info) && single_set (insn);
	  /* An expression is not available if its operands are
	     subsequently modified, including this insn.  It's also not
	     available if this is a branch, because we can't insert
	     a set after the branch.  */
	  int avail_p = (oprs_available_p (src, insn, current_bb, reg_avail_info)
			 && ! JUMP_P (insn));

	  insert_expr_in_table (src, GET_MODE (dest), insn, antic_p, avail_p,
				table);
	}

      /* Record sets for constant/copy propagation.  */
      else if (table->set_p
	       && regno >= FIRST_PSEUDO_REGISTER
	       && ((GET_CODE (src) == REG
		    && REGNO (src) >= FIRST_PSEUDO_REGISTER
		    && can_copy_p [GET_MODE (dest)]
		    && REGNO (src) != regno)
		   || (CONSTANT_P (src)
		       && GET_CODE (src) != CONSTANT_P_RTX))
	       /* A copy is not available if its src or dest is subsequently
		  modified.  Here we want to search from INSN+1 on, but
		  oprs_available_p searches from INSN on.  */
	       && (insn == BLOCK_END (BLOCK_NUM (insn))
		   || ((tmp = next_nonnote_insn (insn)) != NULL_RTX
		       && oprs_available_p (pat, tmp, current_bb, reg_avail_info))))
	insert_set_in_table (pat, insn, table);
    }
}

static void
hash_scan_clobber (x, insn, table)
     rtx x ATTRIBUTE_UNUSED, insn ATTRIBUTE_UNUSED;
     struct hash_table *table ATTRIBUTE_UNUSED;
{
  /* Currently nothing to do.  */
}

static void
hash_scan_call (x, insn, table)
     rtx x ATTRIBUTE_UNUSED, insn ATTRIBUTE_UNUSED;
     struct hash_table *table ATTRIBUTE_UNUSED;
{
  /* Currently nothing to do.  */
}

/* Insert expression X in INSN in the hash TABLE.
   If it is already present, record it as the last occurrence in INSN's
   basic block.

   MODE is the mode of the value X is being stored into.
   It is only used if X is a CONST_INT.

   ANTIC_P is non-zero if X is an anticipatable expression.
   AVAIL_P is non-zero if X is an available expression.  */

static void
insert_expr_in_table (x, mode, insn, antic_p, avail_p, table)
     rtx x;
     enum machine_mode mode;
     rtx insn;
     int antic_p, avail_p;
     struct hash_table *table;
{
  int found, do_not_record_p;
  unsigned int hash;
  struct expr *cur_expr, *last_expr = NULL;
  struct occr *antic_occr, *avail_occr;
  struct occr *last_occr = NULL;

  hash = hash_expr (x, mode, &do_not_record_p, table->size);

  /* Do not insert expression in table if it contains volatile operands,
     or if hash_expr determines the expression is something we don't want
     to or can't handle.  */
  if (do_not_record_p)
    return;

  cur_expr = table->table[hash];
  found = 0;

  while (cur_expr && 0 == (found = expr_equiv_p (cur_expr->expr, x)))
    {
      /* If the expression isn't found, save a pointer to the end of
	 the list.  */
      last_expr = cur_expr;
      cur_expr = cur_expr->next_same_hash;
    }

  if (! found)
    {
      cur_expr = (struct expr *) gcse_alloc (sizeof (struct expr));
      bytes_used += sizeof (struct expr);
      if (table->table[hash] == NULL)
	/* This is the first pattern that hashed to this index.  */
	table->table[hash] = cur_expr;
      else
	/* Add EXPR to end of this hash chain.  */
	last_expr->next_same_hash = cur_expr;

      /* Set the fields of the expr element.  */
      cur_expr->expr = x;
      cur_expr->bitmap_index = table->n_elems++;
      cur_expr->next_same_hash = NULL;
      cur_expr->antic_occr = NULL;
      cur_expr->avail_occr = NULL;
    }

  /* Now record the occurrence(s).  */
  if (antic_p)
    {
      antic_occr = cur_expr->antic_occr;

      /* Search for another occurrence in the same basic block.  */
      while (antic_occr && BLOCK_NUM (antic_occr->insn) != BLOCK_NUM (insn))
	{
	  /* If an occurrence isn't found, save a pointer to the end of
	     the list.  */
	  last_occr = antic_occr;
	  antic_occr = antic_occr->next;
	}

      if (antic_occr)
	/* Found another instance of the expression in the same basic block.
	   Prefer the currently recorded one.  We want the first one in the
	   block and the block is scanned from start to end.  */
	; /* nothing to do */
      else
	{
	  /* First occurrence of this expression in this basic block.  */
	  antic_occr = (struct occr *) gcse_alloc (sizeof (struct occr));
	  bytes_used += sizeof (struct occr);
	  /* First occurrence of this expression in any block?  */
	  if (cur_expr->antic_occr == NULL)
	    cur_expr->antic_occr = antic_occr;
	  else
	    last_occr->next = antic_occr;

	  antic_occr->insn = insn;
	  antic_occr->next = NULL;
	}
    }

  if (avail_p)
    {
      avail_occr = cur_expr->avail_occr;

      /* Search for another occurrence in the same basic block.  */
      while (avail_occr && BLOCK_NUM (avail_occr->insn) != BLOCK_NUM (insn))
	{
	  /* If an occurrence isn't found, save a pointer to the end of
	     the list.  */
	  last_occr = avail_occr;
	  avail_occr = avail_occr->next;
	}

      if (avail_occr)
	/* Found another instance of the expression in the same basic block.
	   Prefer this occurrence to the currently recorded one.  We want
	   the last one in the block and the block is scanned from start
	   to end.  */
	avail_occr->insn = insn;
      else
	{
	  /* First occurrence of this expression in this basic block.  */
	  avail_occr = (struct occr *) gcse_alloc (sizeof (struct occr));
	  bytes_used += sizeof (struct occr);

	  /* First occurrence of this expression in any block?  */
	  if (cur_expr->avail_occr == NULL)
	    cur_expr->avail_occr = avail_occr;
	  else
	    last_occr->next = avail_occr;

	  avail_occr->insn = insn;
	  avail_occr->next = NULL;
	}
    }
}

/* Insert pattern X in INSN in the hash table.
   X is a SET of a reg to either another reg or a constant.
   If it is already present, record it as the last occurrence in INSN's
   basic block.  */

static void
insert_set_in_table (x, insn, table)
     rtx x;
     rtx insn;
     struct hash_table *table;
{
  int found;
  unsigned int hash;
  struct expr *cur_expr, *last_expr = NULL;
  struct occr *cur_occr, *last_occr = NULL;

  if (GET_CODE (x) != SET
      || GET_CODE (SET_DEST (x)) != REG)
    abort ();

  hash = hash_set (REGNO (SET_DEST (x)), table->size);

  cur_expr = table->table[hash];
  found = 0;

  while (cur_expr && 0 == (found = expr_equiv_p (cur_expr->expr, x)))
    {
      /* If the expression isn't found, save a pointer to the end of
	 the list.  */
      last_expr = cur_expr;
      cur_expr = cur_expr->next_same_hash;
    }

  if (! found)
    {
      cur_expr = (struct expr *) gcse_alloc (sizeof (struct expr));
      bytes_used += sizeof (struct expr);
      if (table->table[hash] == NULL)
	/* This is the first pattern that hashed to this index.  */
	table->table[hash] = cur_expr;
      else
	/* Add EXPR to end of this hash chain.  */
	last_expr->next_same_hash = cur_expr;

      /* Set the fields of the expr element.
	 We must copy X because it can be modified when copy propagation is
	 performed on its operands.  */
      cur_expr->expr = copy_rtx (x);
      cur_expr->bitmap_index = table->n_elems++;
      cur_expr->next_same_hash = NULL;
      cur_expr->antic_occr = NULL;
      cur_expr->avail_occr = NULL;
    }

  /* Now record the occurrence.  */
  cur_occr = cur_expr->avail_occr;

  /* Search for another occurrence in the same basic block.  */
  while (cur_occr && BLOCK_NUM (cur_occr->insn) != BLOCK_NUM (insn))
    {
      /* If an occurrence isn't found, save a pointer to the end of
	 the list.  */
      last_occr = cur_occr;
      cur_occr = cur_occr->next;
    }

  if (cur_occr)
    /* Found another instance of the expression in the same basic block.
       Prefer this occurrence to the currently recorded one.  We want the
       last one in the block and the block is scanned from start to end.  */
    cur_occr->insn = insn;
  else
    {
      /* First occurrence of this expression in this basic block.  */
      cur_occr = (struct occr *) gcse_alloc (sizeof (struct occr));
      bytes_used += sizeof (struct occr);

      /* First occurrence of this expression in any block?  */
      if (cur_expr->avail_occr == NULL)
	cur_expr->avail_occr = cur_occr;
      else
	last_occr->next = cur_occr;

      cur_occr->insn = insn;
      cur_occr->next = NULL;
    }
}

/* Hash expression X.

   MODE is only used if X is a CONST_INT.  DO_NOT_RECORD_P is a boolean
   indicating if a volatile operand is found or if the expression contains
   something we don't want to insert in the table.

   ??? One might want to merge this with canon_hash.  Later.  */

static unsigned int
hash_expr (x, mode, do_not_record_p, hash_table_size)
     rtx x;
     enum machine_mode mode;
     int *do_not_record_p;
     int hash_table_size;
{
  unsigned int hash;

  *do_not_record_p = 0;

  hash = hash_expr_1 (x, mode, do_not_record_p);
  return hash % hash_table_size;
}

/* Hash a string.  Just add its bytes up.  */

static inline unsigned
hash_string_1 (ps)
     const char *ps;
{
  unsigned hash = 0;
  const unsigned char *p = (const unsigned char *) ps;

  if (p)
    while (*p)
      hash += *p++;

  return hash;
}

/* Subroutine of hash_expr to do the actual work.  */

static unsigned int
hash_expr_1 (x, mode, do_not_record_p)
     rtx x;
     enum machine_mode mode;
     int *do_not_record_p;
{
  int i, j;
  unsigned hash = 0;
  enum rtx_code code;
  const char *fmt;

  /* Used to turn recursion into iteration.  We can't rely on GCC's
     tail-recursion elimination since we need to keep accumulating values
     in HASH.  */

  if (x == 0)
    return hash;

 repeat:
  code = GET_CODE (x);
  switch (code)
    {
    case REG:
      hash += ((unsigned int) REG << 7) + REGNO (x);
      return hash;

    case CONST_INT:
      hash += (((unsigned int) CONST_INT << 7) + (unsigned int) mode
	       + (unsigned int) INTVAL (x));
      return hash;

    case CONST_DOUBLE:
      /* This is like the general case, except that it only counts
	 the integers representing the constant.  */
      hash += (unsigned int) code + (unsigned int) GET_MODE (x);
      if (GET_MODE (x) != VOIDmode)
	for (i = 2; i < GET_RTX_LENGTH (CONST_DOUBLE); i++)
	  hash += (unsigned int) XWINT (x, i);
      else
	hash += ((unsigned int) CONST_DOUBLE_LOW (x)
		 + (unsigned int) CONST_DOUBLE_HIGH (x));
      return hash;

    case CONST_VECTOR:
      {
	int units;
	rtx elt;

	units = CONST_VECTOR_NUNITS (x);

	for (i = 0; i < units; ++i)
	  {
	    elt = CONST_VECTOR_ELT (x, i);
	    hash += hash_expr_1 (elt, GET_MODE (elt), do_not_record_p);
	  }

	return hash;
      }

      /* Assume there is only one rtx object for any given label.  */
    case LABEL_REF:
      /* We don't hash on the address of the CODE_LABEL to avoid bootstrap
	 differences and differences between each stage's debugging dumps.  */
      hash += (((unsigned int) LABEL_REF << 7)
	       + CODE_LABEL_NUMBER (XEXP (x, 0)));
      return hash;

    case SYMBOL_REF:
      {
	/* Don't hash on the symbol's address to avoid bootstrap differences.
	   Different hash values may cause expressions to be recorded in
	   different orders and thus different registers to be used in the
	   final assembler.  This also avoids differences in the dump files
	   between various stages.  */
	unsigned int h = 0;
	const unsigned char *p = (const unsigned char *) XSTR (x, 0);

	while (*p)
	  h += (h << 7) + *p++; /* ??? revisit */

	hash += ((unsigned int) SYMBOL_REF << 7) + h;
	return hash;
      }

    case MEM:
      if (MEM_VOLATILE_P (x))
	{
	  *do_not_record_p = 1;
	  return 0;
	}

      hash += (unsigned int) MEM;
      /* We used alias set for hashing, but this is not good, since the alias
	 set may differ in -fprofile-arcs and -fbranch-probabilities compilation
	 causing the profiles to fail to match.  */
      x = XEXP (x, 0);
      goto repeat;

    case PRE_DEC:
    case PRE_INC:
    case POST_DEC:
    case POST_INC:
    case PC:
    case CC0:
    case CALL:
    case UNSPEC_VOLATILE:
      *do_not_record_p = 1;
      return 0;

    case ASM_OPERANDS:
      if (MEM_VOLATILE_P (x))
	{
	  *do_not_record_p = 1;
	  return 0;
	}
      else
	{
	  /* We don't want to take the filename and line into account.  */
	  hash += (unsigned) code + (unsigned) GET_MODE (x)
	    + hash_string_1 (ASM_OPERANDS_TEMPLATE (x))
	    + hash_string_1 (ASM_OPERANDS_OUTPUT_CONSTRAINT (x))
	    + (unsigned) ASM_OPERANDS_OUTPUT_IDX (x);

	  if (ASM_OPERANDS_INPUT_LENGTH (x))
	    {
	      for (i = 1; i < ASM_OPERANDS_INPUT_LENGTH (x); i++)
		{
		  hash += (hash_expr_1 (ASM_OPERANDS_INPUT (x, i),
					GET_MODE (ASM_OPERANDS_INPUT (x, i)),
					do_not_record_p)
			   + hash_string_1 (ASM_OPERANDS_INPUT_CONSTRAINT
					    (x, i)));
		}

	      hash += hash_string_1 (ASM_OPERANDS_INPUT_CONSTRAINT (x, 0));
	      x = ASM_OPERANDS_INPUT (x, 0);
	      mode = GET_MODE (x);
	      goto repeat;
	    }
	  return hash;
	}

    default:
      break;
    }

  hash += (unsigned) code + (unsigned) GET_MODE (x);
  for (i = GET_RTX_LENGTH (code) - 1, fmt = GET_RTX_FORMAT (code); i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  /* If we are about to do the last recursive call
	     needed at this level, change it into iteration.
	     This function is called enough to be worth it.  */
	  if (i == 0)
	    {
	      x = XEXP (x, i);
	      goto repeat;
	    }

	  hash += hash_expr_1 (XEXP (x, i), 0, do_not_record_p);
	  if (*do_not_record_p)
	    return 0;
	}

      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  {
	    hash += hash_expr_1 (XVECEXP (x, i, j), 0, do_not_record_p);
	    if (*do_not_record_p)
	      return 0;
	  }

      else if (fmt[i] == 's')
	hash += hash_string_1 (XSTR (x, i));
      else if (fmt[i] == 'i')
	hash += (unsigned int) XINT (x, i);
      else
	abort ();
    }

  return hash;
}

/* Hash a set of register REGNO.

   Sets are hashed on the register that is set.  This simplifies the PRE copy
   propagation code.

   ??? May need to make things more elaborate.  Later, as necessary.  */

static unsigned int
hash_set (regno, hash_table_size)
     int regno;
     int hash_table_size;
{
  unsigned int hash;

  hash = regno;
  return hash % hash_table_size;
}

/* Top level function to create an expression or assignment hash table.

   Expression entries are placed in the hash table if
   - they are of the form (set (pseudo-reg) src),
   - src is something we want to perform GCSE on,
   - none of the operands are subsequently modified in the block

   Assignment entries are placed in the hash table if
   - they are of the form (set (pseudo-reg) src),
   - src is something we want to perform const/copy propagation on,
   - none of the operands or target are subsequently modified in the block

   Currently src must be a pseudo-reg or a const_int.

   F is the first insn.
   TABLE is the table computed.  */

static void
compute_hash_table_work (table)
     struct hash_table *table;
{
  unsigned int i;
  basic_block current_bb;
  struct reg_avail_info *reg_avail_info;

  /* While we compute the hash table we also compute a bit array of which
     registers are set in which blocks.
     ??? This isn't needed during const/copy propagation, but it's cheap to
     compute.  Later.  */
  sbitmap_vector_zero (reg_set_in_block, last_basic_block);

  /* re-Cache any INSN_LIST nodes we have allocated.  */
  clear_modify_mem_tables ();
  /* Some working arrays used to track first and last set in each block.  */
  reg_avail_info = (struct reg_avail_info*)
    gmalloc (max_gcse_regno * sizeof (struct reg_avail_info));

  for (i = 0; i < max_gcse_regno; ++i)
    reg_avail_info[i].last_bb = NULL;

  FOR_EACH_BB (current_bb)
    {
      rtx insn;
      unsigned int regno;
      int in_libcall_block;

      /* First pass over the instructions records information used to
	 determine when registers and memory are first and last set.
	 ??? hard-reg reg_set_in_block computation
	 could be moved to compute_sets since they currently don't change.  */

      for (insn = current_bb->head;
	   insn && insn != NEXT_INSN (current_bb->end);
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
		  record_last_reg_set_info (insn, regno, current_bb, reg_avail_info);

	      mark_call (insn);
	    }

	  rlsi_current_bb = current_bb;
	  rlsi_reg_avail_info = reg_avail_info;
	  note_stores (PATTERN (insn), record_last_set_info, insn);
	}

      /* Insert implicit sets in the hash table.  */
      if (table->set_p
	  && implicit_sets[current_bb->index] != NULL_RTX)
	hash_scan_set (implicit_sets[current_bb->index],
		       current_bb->head, table, current_bb, reg_avail_info);

      /* The next pass builds the hash table.  */

      for (insn = current_bb->head, in_libcall_block = 0;
	   insn && insn != NEXT_INSN (current_bb->end);
	   insn = NEXT_INSN (insn))
	if (INSN_P (insn))
	  {
	    if (find_reg_note (insn, REG_LIBCALL, NULL_RTX))
	      in_libcall_block = 1;
	    else if (table->set_p && find_reg_note (insn, REG_RETVAL, NULL_RTX))
	      in_libcall_block = 0;
	    hash_scan_insn (insn, table, in_libcall_block, current_bb, reg_avail_info);
	    if (!table->set_p && find_reg_note (insn, REG_RETVAL, NULL_RTX))
	      in_libcall_block = 0;
	  }
    }

  free (reg_avail_info);
  reg_avail_info = NULL;
}

/* Record all of the canonicalized MEMs of record_last_mem_set_info's insn.
   Note we store a pair of elements in the list, so they have to be
   taken off pairwise.  */

static void
canon_list_insert (dest, unused1, v_insn)
     rtx    dest ATTRIBUTE_UNUSED;
     rtx    unused1 ATTRIBUTE_UNUSED;
     void * v_insn;
{
  rtx dest_addr, insn;
  int bb;

  while (GET_CODE (dest) == SUBREG
      || GET_CODE (dest) == ZERO_EXTRACT
      || GET_CODE (dest) == SIGN_EXTRACT
      || GET_CODE (dest) == STRICT_LOW_PART)
    dest = XEXP (dest, 0);

  /* If DEST is not a MEM, then it will not conflict with a load.  Note
     that function calls are assumed to clobber memory, but are handled
     elsewhere.  */

  if (GET_CODE (dest) != MEM)
    return;

  dest_addr = get_addr (XEXP (dest, 0));
  dest_addr = canon_rtx (dest_addr);
  insn = (rtx) v_insn;
  bb = BLOCK_NUM (insn);

  canon_modify_mem_list[bb] =
    alloc_EXPR_LIST (VOIDmode, dest_addr, canon_modify_mem_list[bb]);
  canon_modify_mem_list[bb] =
    alloc_EXPR_LIST (VOIDmode, dest, canon_modify_mem_list[bb]);
  bitmap_set_bit (canon_modify_mem_list_set, bb);
}

/* Hash tables -- exported.  */

/* Compute the hash TABLE for doing copy/const propagation or
   expression hash table.  */

void
compute_hash_table (table)
    struct hash_table *table;
{
  /* Initialize count of number of entries in hash table.  */
  table->n_elems = 0;
  memset ((char *) table->table, 0,
	  table->size * sizeof (struct expr *));

  compute_hash_table_work (table);
}

/* Lookup pattern PAT in the expression TABLE.
   The result is a pointer to the table entry, or NULL if not found.  */

struct expr *
lookup_expr (pat, table)
     rtx pat;
     struct hash_table *table;
{
  int do_not_record_p;
  unsigned int hash = hash_expr (pat, GET_MODE (pat), &do_not_record_p,
				 table->size);
  struct expr *expr;

  if (do_not_record_p)
    return NULL;

  expr = table->table[hash];

  while (expr && ! expr_equiv_p (expr->expr, pat))
    expr = expr->next_same_hash;

  return expr;
}

/* Lookup REGNO in the set TABLE.  The result is a pointer to the
   table entry, or NULL if not found.  */

struct expr *
lookup_set (regno, table)
     unsigned int regno;
     struct hash_table *table;
{
  unsigned int hash = hash_set (regno, table->size);
  struct expr *expr;

  expr = table->table[hash];

  while (expr && REGNO (SET_DEST (expr->expr)) != regno)
    expr = expr->next_same_hash;

  return expr;
}

/* Return the next entry for REGNO in list EXPR.  */

struct expr *
next_set (regno, expr)
     unsigned int regno;
     struct expr *expr;
{
  do
    expr = expr->next_same_hash;
  while (expr && REGNO (SET_DEST (expr->expr)) != regno);

  return expr;
}

/* Allocate space for the set/expr hash TABLE.
   N_INSNS is the number of instructions in the function.
   It is used to determine the number of buckets to use.
   SET_P determines whether set or expression table will
   be created.  */

void
alloc_hash_table (n_insns, table, set_p)
     int n_insns;
     struct hash_table *table;
     int set_p;
{
  int n;

  table->size = n_insns / 4;
  if (table->size < 11)
    table->size = 11;

  /* Attempt to maintain efficient use of hash table.
     Making it an odd number is simplest for now.
     ??? Later take some measurements.  */
  table->size |= 1;
  n = table->size * sizeof (struct expr *);
  table->table = (struct expr **) gmalloc (n);
  table->set_p = set_p;
}

/* Free things allocated by alloc_hash_table.  */

void
free_hash_table (table)
     struct hash_table *table;
{
  free (table->table);
}

void
dump_hash_table (file, name, table)
     FILE *file;
     const char *name;
     struct hash_table *table;
{
  int i;
  /* Flattened out table, so it's printed in proper order.  */
  struct expr **flat_table;
  unsigned int *hash_val;
  struct expr *expr;

  flat_table
    = (struct expr **) xcalloc (table->n_elems, sizeof (struct expr *));
  hash_val = (unsigned int *) xmalloc (table->n_elems * sizeof (unsigned int));

  for (i = 0; i < (int) table->size; i++)
    for (expr = table->table[i]; expr != NULL; expr = expr->next_same_hash)
      {
	flat_table[expr->bitmap_index] = expr;
	hash_val[expr->bitmap_index] = i;
      }

  fprintf (file, "%s hash table (%d buckets, %d entries)\n",
	   name, table->size, table->n_elems);

  for (i = 0; i < (int) table->n_elems; i++)
    if (flat_table[i] != 0)
      {
	expr = flat_table[i];
	fprintf (file, "Index %d (hash value %d)\n  ",
		 expr->bitmap_index, hash_val[i]);
	print_rtl (file, expr->expr);
	fprintf (file, "\n");
      }

  fprintf (file, "\n");

  free (flat_table);
  free (hash_val);
}

/* Hash tables -- exported support functions.  */

/* See whether X, the source of a set, is something we want to consider for
   GCSE.  */

static GTY(()) rtx test_insn;
int
want_to_gcse_p (x)
     rtx x;
{
  int num_clobbers = 0;
  int icode;

  switch (GET_CODE (x))
    {
    case REG:
    case SUBREG:
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
    case CALL:
    case CONSTANT_P_RTX:
      return 0;

    default:
      break;
    }

  /* If this is a valid operand, we are OK.  If it's VOIDmode, we aren't.  */
  if (general_operand (x, GET_MODE (x)))
    return 1;
  else if (GET_MODE (x) == VOIDmode)
    return 0;

  /* Otherwise, check if we can make a valid insn from it.  First initialize
     our test insn if we haven't already.  */
  if (test_insn == 0)
    {
      test_insn
	= make_insn_raw (gen_rtx_SET (VOIDmode,
				      gen_rtx_REG (word_mode,
						   FIRST_PSEUDO_REGISTER * 2),
				      const0_rtx));
      NEXT_INSN (test_insn) = PREV_INSN (test_insn) = 0;
    }

  /* Now make an insn like the one we would make when GCSE'ing and see if
     valid.  */
  PUT_MODE (SET_DEST (PATTERN (test_insn)), GET_MODE (x));
  SET_SRC (PATTERN (test_insn)) = x;
  return ((icode = recog (PATTERN (test_insn), test_insn, &num_clobbers)) >= 0
	  && (num_clobbers == 0 || ! added_clobbers_hard_reg_p (icode)));
}

/* Record memory modification information for INSN.  We do not actually care
   about the memory location(s) that are set, or even how they are set (consider
   a CALL_INSN).  We merely need to record which insns modify memory.  */

void
record_last_mem_set_info (insn)
     rtx insn;
{
  int bb = BLOCK_NUM (insn);

  /* load_killed_in_block_p will handle the case of calls clobbering
     everything.  */
  modify_mem_list[bb] = alloc_INSN_LIST (insn, modify_mem_list[bb]);
  bitmap_set_bit (modify_mem_list_set, bb);

  if (GET_CODE (insn) == CALL_INSN)
    {
      /* Note that traversals of this loop (other than for free-ing)
	 will break after encountering a CALL_INSN.  So, there's no
	 need to insert a pair of items, as canon_list_insert does.  */
      canon_modify_mem_list[bb] =
	alloc_INSN_LIST (insn, canon_modify_mem_list[bb]);
      bitmap_set_bit (canon_modify_mem_list_set, bb);
    }
  else
    note_stores (PATTERN (insn), canon_list_insert, (void*) insn);
}

/* Computation of local properties -- static.  */

/* For each block, compute whether X is transparent.  X is either an
   expression or an assignment [though we don't care which, for this context
   an assignment is treated as an expression].  For each block where an
   element of X is modified, set (SET_P == 1) or reset (SET_P == 0) the INDX
   bit in BMAP.  */

static void
compute_transp (x, indx, bmap, set_p)
     rtx x;
     int indx;
     sbitmap *bmap;
     int set_p;
{
  int i, j;
  basic_block bb;
  enum rtx_code code;
  reg_set *r;
  const char *fmt;

  /* repeat is used to turn tail-recursion into iteration since GCC
     can't do it when there's no return value.  */
 repeat:

  if (x == 0)
    return;

  code = GET_CODE (x);
  switch (code)
    {
    case REG:
      if (set_p)
	{
	  if (REGNO (x) < FIRST_PSEUDO_REGISTER)
	    {
	      FOR_EACH_BB (bb)
		if (TEST_BIT (reg_set_in_block[bb->index], REGNO (x)))
		  SET_BIT (bmap[bb->index], indx);
	    }
	  else
	    {
	      for (r = reg_set_table.table[REGNO (x)]; r != NULL; r = r->next)
		SET_BIT (bmap[BLOCK_NUM (r->insn)], indx);
	    }
	}
      else
	{
	  if (REGNO (x) < FIRST_PSEUDO_REGISTER)
	    {
	      FOR_EACH_BB (bb)
		if (TEST_BIT (reg_set_in_block[bb->index], REGNO (x)))
		  RESET_BIT (bmap[bb->index], indx);
	    }
	  else
	    {
	      for (r = reg_set_table.table[REGNO (x)]; r != NULL; r = r->next)
		RESET_BIT (bmap[BLOCK_NUM (r->insn)], indx);
	    }
	}

      return;

    case MEM:
      FOR_EACH_BB (bb)
	{
	  rtx list_entry = canon_modify_mem_list[bb->index];

	  while (list_entry)
	    {
	      rtx dest, dest_addr;

	      if (GET_CODE (XEXP (list_entry, 0)) == CALL_INSN)
		{
		  if (set_p)
		    SET_BIT (bmap[bb->index], indx);
		  else
		    RESET_BIT (bmap[bb->index], indx);
		  break;
		}
	      /* LIST_ENTRY must be an INSN of some kind that sets memory.
		 Examine each hunk of memory that is modified.  */

	      dest = XEXP (list_entry, 0);
	      list_entry = XEXP (list_entry, 1);
	      dest_addr = XEXP (list_entry, 0);

	      if (canon_true_dependence (dest, GET_MODE (dest), dest_addr,
					 x, rtx_addr_varies_p))
		{
		  if (set_p)
		    SET_BIT (bmap[bb->index], indx);
		  else
		    RESET_BIT (bmap[bb->index], indx);
		  break;
		}
	      list_entry = XEXP (list_entry, 1);
	    }
	}

      x = XEXP (x, 0);
      goto repeat;

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
      return;

    default:
      break;
    }

  for (i = GET_RTX_LENGTH (code) - 1, fmt = GET_RTX_FORMAT (code); i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  /* If we are about to do the last recursive call
	     needed at this level, change it into iteration.
	     This function is called enough to be worth it.  */
	  if (i == 0)
	    {
	      x = XEXP (x, i);
	      goto repeat;
	    }

	  compute_transp (XEXP (x, i), indx, bmap, set_p);
	}
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  compute_transp (XVECEXP (x, i, j), indx, bmap, set_p);
    }
}

/* Computation of local properties -- exported.  */

/* Compute the local properties of each recorded expression.

   Local properties are those that are defined by the block, irrespective of
   other blocks.

   An expression is transparent in a block if its operands are not modified
   in the block.

   An expression is computed (locally available) in a block if it is computed
   at least once and expression would contain the same value if the
   computation was moved to the end of the block.

   An expression is locally anticipatable in a block if it is computed at
   least once and expression would contain the same value if the computation
   was moved to the beginning of the block.

   We call this routine for cprop, pre and code hoisting.  They all compute
   basically the same information and thus can easily share this code.

   TRANSP, COMP, and ANTLOC are destination sbitmaps for recording local
   properties.  If NULL, then it is not necessary to compute or record that
   particular property.

   TABLE controls which hash table to look at.  If it is  set hash table,
   additionally, TRANSP is computed as ~TRANSP, since this is really cprop's
   ABSALTERED.  */

void
compute_local_properties (transp, comp, antloc, table)
     sbitmap *transp;
     sbitmap *comp;
     sbitmap *antloc;
     struct hash_table *table;
{
  unsigned int i;

  /* Initialize any bitmaps that were passed in.  */
  if (transp)
    {
      if (table->set_p)
	sbitmap_vector_zero (transp, last_basic_block);
      else
	sbitmap_vector_ones (transp, last_basic_block);
    }

  if (comp)
    sbitmap_vector_zero (comp, last_basic_block);
  if (antloc)
    sbitmap_vector_zero (antloc, last_basic_block);

  for (i = 0; i < table->size; i++)
    {
      struct expr *expr;

      for (expr = table->table[i]; expr != NULL; expr = expr->next_same_hash)
	{
	  int indx = expr->bitmap_index;
	  struct occr *occr;

	  /* The expression is transparent in this block if it is not killed.
	     We start by assuming all are transparent [none are killed], and
	     then reset the bits for those that are.  */
	  if (transp)
	    compute_transp (expr->expr, indx, transp, table->set_p);

	  /* The occurrences recorded in antic_occr are exactly those that
	     we want to set to non-zero in ANTLOC.  */
	  if (antloc)
	    for (occr = expr->antic_occr; occr != NULL; occr = occr->next)
	      {
		SET_BIT (antloc[BLOCK_NUM (occr->insn)], indx);

		/* While we're scanning the table, this is a good place to
		   initialize this.  */
		occr->deleted_p = 0;
	      }

	  /* The occurrences recorded in avail_occr are exactly those that
	     we want to set to non-zero in COMP.  */
	  if (comp)
	    for (occr = expr->avail_occr; occr != NULL; occr = occr->next)
	      {
		SET_BIT (comp[BLOCK_NUM (occr->insn)], indx);

		/* While we're scanning the table, this is a good place to
		   initialize this.  */
		occr->copied_p = 0;
	      }

	  /* While we're scanning the table, this is a good place to
	     initialize this.  */
	  expr->reaching_reg = 0;
	}
    }
}

/* Operands checking -- static.  */

/* Mark things set by a CALL.  */

static void
mark_call (insn)
     rtx insn;
{
  if (! CONST_OR_PURE_CALL_P (insn))
    record_last_mem_set_info (insn);
}

/* Mark things set by a SET.  */

static void
mark_set (pat, insn)
     rtx pat, insn;
{
  rtx dest = SET_DEST (pat);

  while (GET_CODE (dest) == SUBREG
	 || GET_CODE (dest) == ZERO_EXTRACT
	 || GET_CODE (dest) == SIGN_EXTRACT
	 || GET_CODE (dest) == STRICT_LOW_PART)
    dest = XEXP (dest, 0);

  if (GET_CODE (dest) == REG)
    SET_REGNO_REG_SET (reg_set_bitmap, REGNO (dest));
  else if (GET_CODE (dest) == MEM)
    record_last_mem_set_info (insn);

  if (GET_CODE (SET_SRC (pat)) == CALL)
    mark_call (insn);
}

/* Record things set by a CLOBBER.  */

static void
mark_clobber (pat, insn)
     rtx pat, insn;
{
  rtx clob = XEXP (pat, 0);

  while (GET_CODE (clob) == SUBREG || GET_CODE (clob) == STRICT_LOW_PART)
    clob = XEXP (clob, 0);

  if (GET_CODE (clob) == REG)
    SET_REGNO_REG_SET (reg_set_bitmap, REGNO (clob));
  else
    record_last_mem_set_info (insn);
}

/* Operands checking -- exported.  */

/* Reset tables used to keep track of what's still available [since the
   start of the block].  */

void
reset_opr_set_tables ()
{
  /* Maintain a bitmap of which regs have been set since beginning of
     the block.  */
  CLEAR_REG_SET (reg_set_bitmap);

  /* Also keep a record of the last instruction to modify memory.
     For now this is very trivial, we only record whether any memory
     location has been modified.  */
  clear_modify_mem_tables ();
}

/* Return non-zero if the operands of X are not set before INSN in
   INSN's basic block.  */

int
oprs_not_set_p (x, insn)
     rtx x, insn;
{
  int i, j;
  enum rtx_code code;
  const char *fmt;

  if (x == 0)
    return 1;

  code = GET_CODE (x);
  switch (code)
    {
    case PC:
    case CC0:
    case CONST:
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
    case SYMBOL_REF:
    case LABEL_REF:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
      return 1;

    case MEM:
      if (load_killed_in_block_p (BLOCK_FOR_INSN (insn),
				  INSN_CUID (insn), x, 0))
	return 0;
      else
	return oprs_not_set_p (XEXP (x, 0), insn);

    case REG:
      return ! REGNO_REG_SET_P (reg_set_bitmap, REGNO (x));

    default:
      break;
    }

  for (i = GET_RTX_LENGTH (code) - 1, fmt = GET_RTX_FORMAT (code); i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  /* If we are about to do the last recursive call
	     needed at this level, change it into iteration.
	     This function is called enough to be worth it.  */
	  if (i == 0)
	    return oprs_not_set_p (XEXP (x, i), insn);

	  if (! oprs_not_set_p (XEXP (x, i), insn))
	    return 0;
	}
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  if (! oprs_not_set_p (XVECEXP (x, i, j), insn))
	    return 0;
    }

  return 1;
}

/* Record things set by INSN.
   This data is used by oprs_not_set_p.  */

void
mark_oprs_set (insn)
     rtx insn;
{
  rtx pat = PATTERN (insn);
  int i;

  if (GET_CODE (pat) == SET)
    mark_set (pat, insn);
  else if (GET_CODE (pat) == PARALLEL)
    for (i = 0; i < XVECLEN (pat, 0); i++)
      {
	rtx x = XVECEXP (pat, 0, i);

	if (GET_CODE (x) == SET)
	  mark_set (x, insn);
	else if (GET_CODE (x) == CLOBBER)
	  mark_clobber (x, insn);
	else if (GET_CODE (x) == CALL)
	  mark_call (insn);
      }

  else if (GET_CODE (pat) == CLOBBER)
    mark_clobber (pat, insn);
  else if (GET_CODE (pat) == CALL)
    mark_call (insn);
}

/* Modify mem tables -- static.  */

/* Like free_INSN_LIST_list or free_EXPR_LIST_list, except that the node
   types may be mixed.  */

static void
free_insn_expr_list_list (listp)
     rtx *listp;
{
  rtx list, next;

  for (list = *listp; list ; list = next)
    {
      next = XEXP (list, 1);
      if (GET_CODE (list) == EXPR_LIST)
	free_EXPR_LIST_node (list);
      else
	free_INSN_LIST_node (list);
    }

  *listp = NULL;
}

/* Modify mem tables -- exported.  */

/* Clear canon_modify_mem_list and modify_mem_list tables.  */
void
clear_modify_mem_tables ()
{
  int i;

  EXECUTE_IF_SET_IN_BITMAP
    (modify_mem_list_set, 0, i, free_INSN_LIST_list (modify_mem_list + i));
  bitmap_clear (modify_mem_list_set);

  EXECUTE_IF_SET_IN_BITMAP
    (canon_modify_mem_list_set, 0, i,
     free_insn_expr_list_list (canon_modify_mem_list + i));
  bitmap_clear (canon_modify_mem_list_set);
}

/* Release memory used by modify_mem_list_set and canon_modify_mem_list_set.  */

void
free_modify_mem_tables ()
{
  clear_modify_mem_tables ();
  free (modify_mem_list);
  free (canon_modify_mem_list);
  modify_mem_list = 0;
  canon_modify_mem_list = 0;
}


/* Ldst handling -- exported.  */

/*  Here we provide the things required to do store motion towards
    the exit. In order for this to be effective, gcse also needed to
    be taught how to move a load when it is kill only by a store to itself.

	    int i;
	    float a[10];

	    void foo(float scale)
	    {
	      for (i=0; i<10; i++)
		a[i] *= scale;
	    }

    'i' is both loaded and stored to in the loop. Normally, gcse cannot move
    the load out since its live around the loop, and stored at the bottom
    of the loop.

      The 'Load Motion' referred to and implemented in this file is
    an enhancement to gcse which when using edge based lcm, recognizes
    this situation and allows gcse to move the load out of the loop.

      Once gcse has hoisted the load, store motion can then push this
    load towards the exit, and we end up with no loads or stores of 'i'
    in the loop.  */

/* This will search the ldst list for a matching expression. If it
   doesn't find one, we create one and initialize it.  */

struct ls_expr *
ldst_entry (x)
     rtx x;
{
  struct ls_expr * ptr;

  for (ptr = first_ls_expr(); ptr != NULL; ptr = next_ls_expr (ptr))
    if (expr_equiv_p (ptr->pattern, x))
      break;

  if (!ptr)
    {
      ptr = (struct ls_expr *) xmalloc (sizeof (struct ls_expr));

      ptr->next         = pre_ldst_mems;
      ptr->expr         = NULL;
      ptr->pattern      = x;
      ptr->pattern_regs	= NULL_RTX;
      ptr->loads        = NULL_RTX;
      ptr->stores       = NULL_RTX;
      ptr->reaching_reg = NULL_RTX;
      ptr->invalid      = 0;
      ptr->index        = 0;
      ptr->hash_index   = 0;
      pre_ldst_mems     = ptr;
    }

  return ptr;
}

/* Free up an individual ldst entry.  */

void
free_ldst_entry (ptr)
     struct ls_expr * ptr;
{
  free_INSN_LIST_list (& ptr->loads);
  free_INSN_LIST_list (& ptr->stores);

  free (ptr);
}

/* Free up all memory associated with the ldst list.  */

void
free_ldst_mems ()
{
  while (pre_ldst_mems)
    {
      struct ls_expr * tmp = pre_ldst_mems;

      pre_ldst_mems = pre_ldst_mems->next;

      free_ldst_entry (tmp);
    }

  pre_ldst_mems = NULL;
}

/* Dump debugging info about the ldst list.  */

void
print_ldst_list (file)
     FILE * file;
{
  struct ls_expr * ptr;

  fprintf (file, "LDST list: \n");

  for (ptr = first_ls_expr(); ptr != NULL; ptr = next_ls_expr (ptr))
    {
      fprintf (file, "  Pattern (%3d): ", ptr->index);

      print_rtl (file, ptr->pattern);

      fprintf (file, "\n	 Loads : ");

      if (ptr->loads)
	print_rtl (file, ptr->loads);
      else
	fprintf (file, "(nil)");

      fprintf (file, "\n	Stores : ");

      if (ptr->stores)
	print_rtl (file, ptr->stores);
      else
	fprintf (file, "(nil)");

      fprintf (file, "\n\n");
    }

  fprintf (file, "\n");
}

/* Returns 1 if X is in the list of ldst only expressions.  */

struct ls_expr *
find_rtx_in_ldst (x)
     rtx x;
{
  struct ls_expr * ptr;

  for (ptr = pre_ldst_mems; ptr != NULL; ptr = ptr->next)
    if (expr_equiv_p (ptr->pattern, x) && ! ptr->invalid)
      return ptr;

  return NULL;
}

/* Assign each element of the list of mems a monotonically increasing value.  */

int
enumerate_ldsts ()
{
  struct ls_expr * ptr;
  int n = 0;

  for (ptr = pre_ldst_mems; ptr != NULL; ptr = ptr->next)
    ptr->index = n++;

  return n;
}

/* Return first item in the list.  */

inline struct ls_expr *
first_ls_expr ()
{
  return pre_ldst_mems;
}

/* Return the next item in ther list after the specified one.  */

inline struct ls_expr *
next_ls_expr (ptr)
     struct ls_expr * ptr;
{
  return ptr->next;
}

/* Register set information -- exported.  */

/* `reg_set_table' records where each register is set or otherwise
   modified.  */

static struct obstack reg_set_obstack;

void
alloc_reg_set_mem (n_regs)
     int n_regs;
{
  unsigned int n;

  reg_set_table.size = n_regs + REG_SET_TABLE_SLOP;
  n = reg_set_table.size * sizeof (struct reg_set *);
  reg_set_table.table = (struct reg_set **) gmalloc (n);
  memset ((char *) reg_set_table.table, 0, n);

  gcc_obstack_init (&reg_set_obstack);
}

void
free_reg_set_mem ()
{
  free (reg_set_table.table);
  obstack_free (&reg_set_obstack, NULL);
}

/* Record REGNO in the reg_set table.  */

void
record_one_set (regno, insn)
     int regno;
     rtx insn;
{
  /* Allocate a new reg_set element and link it onto the list.  */
  struct reg_set *new_reg_info;

  /* If the table isn't big enough, enlarge it.  */
  if (regno >= reg_set_table.size)
    {
      int new_size = regno + REG_SET_TABLE_SLOP;

      reg_set_table.table
	= (struct reg_set **) grealloc ((char *) reg_set_table.table,
					new_size * sizeof (struct reg_set *));
      memset ((char *) (reg_set_table.table + reg_set_table.size), 0,
	      (new_size - reg_set_table.size) * sizeof (struct reg_set *));
      reg_set_table.size = new_size;
    }

  new_reg_info = (struct reg_set *) obstack_alloc (&reg_set_obstack,
						   sizeof (struct reg_set));
  bytes_used += sizeof (struct reg_set);
  new_reg_info->insn = insn;
  new_reg_info->next = reg_set_table.table[regno];
  reg_set_table.table[regno] = new_reg_info;
}

/* Called from compute_sets via note_stores to handle one SET or CLOBBER in
   an insn.  The DATA is really the instruction in which the SET is
   occurring.  */

void
record_set_info (dest, setter, data)
     rtx dest, setter ATTRIBUTE_UNUSED;
     void *data;
{
  rtx record_set_insn = (rtx) data;

  if (GET_CODE (dest) == REG && REGNO (dest) >= FIRST_PSEUDO_REGISTER)
    record_one_set (REGNO (dest), record_set_insn);
}

/* Insn manipulation -- static.  */

/* If X contains any LABEL_REF's, add REG_LABEL notes for them to INSN.
   If notes are added to an insn which references a CODE_LABEL, the
   LABEL_NUSES count is incremented.  We have to add REG_LABEL notes,
   because the following loop optimization pass requires them.  */

/* ??? This is very similar to the loop.c add_label_notes function.  We
   could probably share code here.  */

/* ??? If there was a jump optimization pass after gcse and before loop,
   then we would not need to do this here, because jump would add the
   necessary REG_LABEL notes.  */

static void
add_label_notes (x, insn)
     rtx x;
     rtx insn;
{
  enum rtx_code code = GET_CODE (x);
  int i, j;
  const char *fmt;

  if (code == LABEL_REF && !LABEL_REF_NONLOCAL_P (x))
    {
      /* This code used to ignore labels that referred to dispatch tables to
	 avoid flow generating (slighly) worse code.

	 We no longer ignore such label references (see LABEL_REF handling in
	 mark_jump_label for additional information).  */

      REG_NOTES (insn) = gen_rtx_INSN_LIST (REG_LABEL, XEXP (x, 0),
					    REG_NOTES (insn));
      if (LABEL_P (XEXP (x, 0)))
	LABEL_NUSES (XEXP (x, 0))++;
      return;
    }

  for (i = GET_RTX_LENGTH (code) - 1, fmt = GET_RTX_FORMAT (code); i >= 0; i--)
    {
      if (fmt[i] == 'e')
	add_label_notes (XEXP (x, i), insn);
      else if (fmt[i] == 'E')
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  add_label_notes (XVECEXP (x, i, j), insn);
    }
}

/* Insn manipulation -- exported.  */

/* Emit move from SRC to DEST noting the equivalence with expression computed
   in INSN.  */
rtx
gcse_emit_move_after (src, dest, insn)
     rtx src, dest, insn;
{
  rtx new;
  rtx set = single_set (insn), set2;
  rtx note;
  rtx eqv;

  /* This should never fail since we're creating a reg->reg copy
     we've verified to be valid.  */

  new = emit_insn_after (gen_move_insn (dest, src), insn);

  /* Note the equivalence for local CSE pass.  */
  set2 = single_set (new);
  if (!set2 || !rtx_equal_p (SET_DEST (set2), dest))
    return new;
  if ((note = find_reg_equal_equiv_note (insn)))
    eqv = XEXP (note, 0);
  else
    eqv = SET_SRC (set);

  set_unique_reg_note (new, REG_EQUAL, copy_insn_1 (src));

  return new;
}

/* Add EXPR to the end of basic block BB.

   This is used by both the PRE and code hoisting.

   For PRE, we want to verify that the expr is either transparent
   or locally anticipatable in the target block.  This check makes
   no sense for code hoisting.  */

void
insert_insn_end_bb (expr, bb, pre, antloc, transp)
     struct expr *expr;
     basic_block bb;
     int pre;
     sbitmap *antloc;
     sbitmap *transp;
{
  rtx insn = bb->end;
  rtx new_insn;
  rtx reg = expr->reaching_reg;
  int regno = REGNO (reg);
  rtx pat, pat_end;

  pat = process_insert_insn (expr);
  if (pat == NULL_RTX || ! INSN_P (pat))
    abort ();

  pat_end = pat;
  while (NEXT_INSN (pat_end) != NULL_RTX)
    pat_end = NEXT_INSN (pat_end);

  /* If the last insn is a jump, insert EXPR in front [taking care to
     handle cc0, etc. properly].  Similary we need to care trapping
     instructions in presence of non-call exceptions.  */

  if (GET_CODE (insn) == JUMP_INSN
      || (GET_CODE (insn) == INSN
	  && (bb->succ->succ_next || (bb->succ->flags & EDGE_ABNORMAL))))
    {
#ifdef HAVE_cc0
      rtx note;
#endif
      /* It should always be the case that we can put these instructions
	 anywhere in the basic block with performing PRE optimizations.
	 Check this.  */
      if (GET_CODE (insn) == INSN && pre
	  && !TEST_BIT (antloc[bb->index], expr->bitmap_index)
	  && !TEST_BIT (transp[bb->index], expr->bitmap_index))
	abort ();

      /* If this is a jump table, then we can't insert stuff here.  Since
	 we know the previous real insn must be the tablejump, we insert
	 the new instruction just before the tablejump.  */
      if (GET_CODE (PATTERN (insn)) == ADDR_VEC
	  || GET_CODE (PATTERN (insn)) == ADDR_DIFF_VEC)
	insn = prev_real_insn (insn);

#ifdef HAVE_cc0
      /* FIXME: 'twould be nice to call prev_cc0_setter here but it aborts
	 if cc0 isn't set.  */
      note = find_reg_note (insn, REG_CC_SETTER, NULL_RTX);
      if (note)
	insn = XEXP (note, 0);
      else
	{
	  rtx maybe_cc0_setter = prev_nonnote_insn (insn);
	  if (maybe_cc0_setter
	      && INSN_P (maybe_cc0_setter)
	      && sets_cc0_p (PATTERN (maybe_cc0_setter)))
	    insn = maybe_cc0_setter;
	}
#endif
      /* FIXME: What if something in cc0/jump uses value set in new insn?  */
      new_insn = emit_insn_before (pat, insn);
    }

  /* Likewise if the last insn is a call, as will happen in the presence
     of exception handling.  */
  else if (GET_CODE (insn) == CALL_INSN
	   && (bb->succ->succ_next || (bb->succ->flags & EDGE_ABNORMAL)))
    {
      /* Keeping in mind SMALL_REGISTER_CLASSES and parameters in registers,
	 we search backward and place the instructions before the first
	 parameter is loaded.  Do this for everyone for consistency and a
	 presumtion that we'll get better code elsewhere as well.

	 It should always be the case that we can put these instructions
	 anywhere in the basic block with performing PRE optimizations.
	 Check this.  */

      if (pre
	  && !TEST_BIT (antloc[bb->index], expr->bitmap_index)
	  && !TEST_BIT (transp[bb->index], expr->bitmap_index))
	abort ();

      /* Since different machines initialize their parameter registers
	 in different orders, assume nothing.  Collect the set of all
	 parameter registers.  */
      insn = find_first_parameter_load (insn, bb->head);

      /* If we found all the parameter loads, then we want to insert
	 before the first parameter load.

	 If we did not find all the parameter loads, then we might have
	 stopped on the head of the block, which could be a CODE_LABEL.
	 If we inserted before the CODE_LABEL, then we would be putting
	 the insn in the wrong basic block.  In that case, put the insn
	 after the CODE_LABEL.  Also, respect NOTE_INSN_BASIC_BLOCK.  */
      while (GET_CODE (insn) == CODE_LABEL
	     || NOTE_INSN_BASIC_BLOCK_P (insn))
	insn = NEXT_INSN (insn);

      new_insn = emit_insn_before (pat, insn);
    }
  else
    new_insn = emit_insn_after (pat, insn);

  while (1)
    {
      if (INSN_P (pat))
	{
	  add_label_notes (PATTERN (pat), new_insn);
	  note_stores (PATTERN (pat), record_set_info, pat);
	}
      if (pat == pat_end)
	break;
      pat = NEXT_INSN (pat);
    }

  gcse_create_count++;

  if (gcse_file)
    {
      fprintf (gcse_file, "PRE/HOIST: end of bb %d, insn %d, ",
	       bb->index, INSN_UID (new_insn));
      fprintf (gcse_file, "copying expression %d to reg %d\n",
	       expr->bitmap_index, regno);
    }
}

/* Insert an instruction at the begining of a basic block, and update
   the BLOCK_HEAD if needed.  */

void
insert_insn_start_bb (insn, bb)
     rtx insn;
     basic_block bb;
{
  /* Insert at start of successor block.  */
  rtx prev = PREV_INSN (bb->head);
  rtx before = bb->head;
  while (before != 0)
    {
      if (GET_CODE (before) != CODE_LABEL
	  && (GET_CODE (before) != NOTE
	      || NOTE_LINE_NUMBER (before) != NOTE_INSN_BASIC_BLOCK))
	break;
      prev = before;
      if (prev == bb->end)
	break;
      before = NEXT_INSN (before);
    }

  insn = emit_insn_after (insn, prev);

  if (gcse_file)
    {
      fprintf (gcse_file, "STORE_MOTION  insert store at start of BB %d:\n",
	       bb->index);
      print_inline_rtx (gcse_file, insn, 6);
      fprintf (gcse_file, "\n");
    }
}

/* Given an expr, generate RTL which we can insert at the end of a BB,
   or on an edge.  Set the block number of any insns generated to
   the value of BB.  */

rtx
process_insert_insn (expr)
     struct expr *expr;
{
  rtx reg = expr->reaching_reg;
  rtx exp = copy_rtx (expr->expr);
  rtx pat;

  start_sequence ();

  /* If the expression is something that's an operand, like a constant,
     just copy it to a register.  */
  if (general_operand (exp, GET_MODE (reg)))
    emit_move_insn (reg, exp);

  /* Otherwise, make a new insn to compute this expression and make sure the
     insn will be recognized (this also adds any needed CLOBBERs).  Copy the
     expression to make sure we don't have any sharing issues.  */
  else if (insn_invalid_p (emit_insn (gen_rtx_SET (VOIDmode, reg, exp))))
    abort ();

  pat = get_insns ();
  end_sequence ();

  return pat;
}

/* Miscelaneous.  */

/* Return non-zero if exp1 is equivalent to exp2.
   ??? Borrowed from cse.c.  Might want to remerge with cse.c.  Later.  */

int
expr_equiv_p (x, y)
     rtx x, y;
{
  int i, j;
  enum rtx_code code;
  const char *fmt;

  if (x == y)
    return 1;

  if (x == 0 || y == 0)
    return x == y;

  code = GET_CODE (x);
  if (code != GET_CODE (y))
    return 0;

  /* (MULT:SI x y) and (MULT:HI x y) are NOT equivalent.  */
  if (GET_MODE (x) != GET_MODE (y))
    return 0;

  switch (code)
    {
    case PC:
    case CC0:
      return x == y;

    case CONST_INT:
      return INTVAL (x) == INTVAL (y);

    case LABEL_REF:
      return XEXP (x, 0) == XEXP (y, 0);

    case SYMBOL_REF:
      return XSTR (x, 0) == XSTR (y, 0);

    case REG:
      return REGNO (x) == REGNO (y);

    case MEM:
      /* Can't merge two expressions in different alias sets, since we can
	 decide that the expression is transparent in a block when it isn't,
	 due to it being set with the different alias set.  */
      if (MEM_ALIAS_SET (x) != MEM_ALIAS_SET (y))
	return 0;
      break;

    /*  For commutative operations, check both orders.  */
    case PLUS:
    case MULT:
    case AND:
    case IOR:
    case XOR:
    case NE:
    case EQ:
      return ((expr_equiv_p (XEXP (x, 0), XEXP (y, 0))
	       && expr_equiv_p (XEXP (x, 1), XEXP (y, 1)))
	      || (expr_equiv_p (XEXP (x, 0), XEXP (y, 1))
		  && expr_equiv_p (XEXP (x, 1), XEXP (y, 0))));

    case ASM_OPERANDS:
      /* We don't use the generic code below because we want to
	 disregard filename and line numbers.  */

      /* A volatile asm isn't equivalent to any other.  */
      if (MEM_VOLATILE_P (x) || MEM_VOLATILE_P (y))
	return 0;

      if (GET_MODE (x) != GET_MODE (y)
	  || strcmp (ASM_OPERANDS_TEMPLATE (x), ASM_OPERANDS_TEMPLATE (y))
	  || strcmp (ASM_OPERANDS_OUTPUT_CONSTRAINT (x),
		     ASM_OPERANDS_OUTPUT_CONSTRAINT (y))
	  || ASM_OPERANDS_OUTPUT_IDX (x) != ASM_OPERANDS_OUTPUT_IDX (y)
	  || ASM_OPERANDS_INPUT_LENGTH (x) != ASM_OPERANDS_INPUT_LENGTH (y))
	return 0;

      if (ASM_OPERANDS_INPUT_LENGTH (x))
	{
	  for (i = ASM_OPERANDS_INPUT_LENGTH (x) - 1; i >= 0; i--)
	    if (! expr_equiv_p (ASM_OPERANDS_INPUT (x, i),
				ASM_OPERANDS_INPUT (y, i))
		|| strcmp (ASM_OPERANDS_INPUT_CONSTRAINT (x, i),
			   ASM_OPERANDS_INPUT_CONSTRAINT (y, i)))
	      return 0;
	}

      return 1;

    default:
      break;
    }

  /* Compare the elements.  If any pair of corresponding elements
     fail to match, return 0 for the whole thing.  */

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      switch (fmt[i])
	{
	case 'e':
	  if (! expr_equiv_p (XEXP (x, i), XEXP (y, i)))
	    return 0;
	  break;

	case 'E':
	  if (XVECLEN (x, i) != XVECLEN (y, i))
	    return 0;
	  for (j = 0; j < XVECLEN (x, i); j++)
	    if (! expr_equiv_p (XVECEXP (x, i, j), XVECEXP (y, i, j)))
	      return 0;
	  break;

	case 's':
	  if (strcmp (XSTR (x, i), XSTR (y, i)))
	    return 0;
	  break;

	case 'i':
	  if (XINT (x, i) != XINT (y, i))
	    return 0;
	  break;

	case 'w':
	  if (XWINT (x, i) != XWINT (y, i))
	    return 0;
	break;

	case '0':
	  break;

	default:
	  abort ();
	}
    }

  return 1;
}

/* Return non-zero if expression X is killed in BB.  */

int
expr_killed_p (x, bb)
     rtx x;
     basic_block bb;
{
  int i, j;
  enum rtx_code code;
  const char *fmt;

  if (x == 0)
    return 1;

  code = GET_CODE (x);
  switch (code)
    {
    case REG:
      return TEST_BIT (reg_set_in_block[bb->index], REGNO (x));

    case MEM:
      if (load_killed_in_block_p (bb, get_max_uid () + 1, x, 0))
	return 1;
      else
	return expr_killed_p (XEXP (x, 0), bb);

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
      return 0;

    default:
      break;
    }

  for (i = GET_RTX_LENGTH (code) - 1, fmt = GET_RTX_FORMAT (code); i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  /* If we are about to do the last recursive call
	     needed at this level, change it into iteration.
	     This function is called enough to be worth it.  */
	  if (i == 0)
	    return expr_killed_p (XEXP (x, i), bb);
	  else if (expr_killed_p (XEXP (x, i), bb))
	    return 1;
	}
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  if (expr_killed_p (XVECEXP (x, i, j), bb))
	    return 1;
    }

  return 0;
}

#include "gt-gcse-utils.h"
