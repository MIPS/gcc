/* Iterator routines for GIMPLE statements.
   Copyright (C) 2007 Free Software Foundation, Inc.
   Contributed by Aldy Hernandez  <aldy@quesejoda.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "gimple.h"
#include "tree-flow.h"

/* Links a sequence of statements before the current statement.  */

void
gsi_link_seq_before (gimple_stmt_iterator *i,
    		     gimple_seq seq,
    		     enum gsi_iterator_update mode)
{
  gimple head, tail, cur;

  /* Die on looping.  */
  gcc_assert (seq != i->seq);

  head = gimple_seq_first (seq);
  tail = gimple_seq_last (seq);
  gimple_seq_init (seq);

  /* Empty sequences need no work.  */
  if (!head || !tail)
    {
      gcc_assert (head == tail);
      return;
    }

  cur = i->stmt;

  /* Link it into the sequence.  */
  if (cur)
    {
      gimple_set_prev (head, gimple_prev (cur));
      if (gimple_prev (head))
	gimple_set_next (gimple_prev (head), head);
      else
	gimple_seq_set_first (i->seq, head);
      gimple_set_next (tail, cur);
      gimple_set_prev (cur, tail);
    }
  else
    {
      gimple_set_prev (head, gimple_seq_last (i->seq));
      if (gimple_prev (head))
	gimple_set_next (gimple_prev (head), head);
      else
	gimple_seq_set_first (i->seq, head);
      gimple_seq_set_last (i->seq, tail);
    }

  /* Update the iterator, if requested.  */
  switch (mode)
    {
    case GSI_NEW_STMT:
    case GSI_CONTINUE_LINKING:
      i->stmt = head;
      break;
    case GSI_SAME_STMT:
      break;
    default:
      gcc_unreachable ();
    }
}


/* Links one gimple statement before the current statement.  */

void
gsi_link_before (gimple_stmt_iterator *i,
    		 gimple g,
		 enum gsi_iterator_update mode)
{
  struct gimple_sequence tseq;

  gimple_seq_init (&tseq);
  gimple_seq_add (&tseq, g);
  gsi_link_seq_before (i, &tseq, mode);
}


/* Links sequence SEQ after the statement pointed-to by iterator I.
   MODE is as in gsi_insert_after.  */

void
gsi_link_seq_after (gimple_stmt_iterator *i, gimple_seq seq,
		    enum gsi_iterator_update mode)
{
  gimple head, tail, cur;

  /* Die on looping.  */
  gcc_assert (seq != i->seq);

  head = gimple_seq_first (seq);
  tail = gimple_seq_last (seq);
  gimple_seq_init (seq);

  /* Empty sequences need no work.  */
  if (!head || !tail)
    {
      gcc_assert (head == tail);
      return;
    }

  cur = i->stmt;

  /* Link it into the list.  */
  if (cur)
    {
      gimple_set_next (tail, gimple_next (cur));
      if (gimple_next (tail))
	gimple_set_prev (gimple_next (tail), tail);
      else
	gimple_seq_set_last (i->seq, tail);
      gimple_set_prev (head, cur);
      gimple_set_next (cur, head);
    }
  else
    {
      gcc_assert (!gimple_seq_last (i->seq));
      gimple_seq_set_first (i->seq, head);
      gimple_seq_set_last (i->seq, tail);
    }

  /* Update the iterator, if requested.  */
  switch (mode)
    {
    case GSI_NEW_STMT:
      i->stmt = head;
      break;
    case GSI_CONTINUE_LINKING:
      i->stmt = tail;
      break;
    case GSI_SAME_STMT:
      gcc_assert (cur);
      break;
    default:
      gcc_unreachable ();
    }
}


/* Links gimple statement G after the statement pointed-to by iterator
   I.  MODE is as ing gsi_insert_after.  */

void
gsi_link_after (gimple_stmt_iterator *i, gimple g,
		enum gsi_iterator_update mode)
{
  struct gimple_sequence tseq;

  gimple_seq_init (&tseq);
  gimple_seq_add (&tseq, g);
  gsi_link_seq_after (i, &tseq, mode);
}


/* Remove a stmt from the sequence.  The iterator is updated to point to the
   next stmt.  */

void
gsi_delink (gimple_stmt_iterator *i)
{
  gimple cur, next, prev;

  cur = i->stmt;
  next = gimple_next (cur);
  prev = gimple_prev (cur);

  if (prev)
    gimple_set_next (prev, next);
  else
    gimple_seq_set_first (i->seq, next);
  if (next)
    gimple_set_prev (next, prev);
  else
    gimple_seq_set_last (i->seq, prev);

  i->stmt = next;
}

/* Move all statements in the sequence after I to a new sequence.  Return this
   new sequence.  I itself is unchanged.  */

gimple_seq
gsi_split_seq_after (const gimple_stmt_iterator *i)
{
  gimple cur, next;
  gimple_seq old_seq, new_seq;

  cur = i->stmt;
  /* How can we possibly split after the end, or before the beginning?  */
  gcc_assert (cur);
  next = gimple_next (cur);

  old_seq = i->seq;
  new_seq = gimple_seq_alloc ();

  gimple_seq_set_first (new_seq, next);
  gimple_seq_set_last (new_seq, gimple_seq_last (old_seq));
  gimple_seq_set_last (old_seq, cur);
  gimple_set_next (cur, NULL);
  gimple_set_prev (next, NULL);

  return new_seq;
}


/* Move all statements in the sequence before I to a new sequence.  Return this
   new sequence.  I is set to the head of the new list.  */

gimple_seq
gsi_split_seq_before (gimple_stmt_iterator *i)
{
  gimple cur, prev;
  gimple_seq old_seq, new_seq;

  cur = i->stmt;
  /* How can we possibly split after the end, or before the beginning?  */
  gcc_assert (cur);
  prev = gimple_prev (cur);

  old_seq = i->seq;
  new_seq = gimple_seq_alloc ();
  i->seq = new_seq;

  gimple_seq_set_first (new_seq, cur);
  gimple_seq_set_last (new_seq, gimple_seq_last (old_seq));
  gimple_seq_set_last (old_seq, prev);
  gimple_set_prev (cur, NULL);
  if (prev)
    gimple_set_next (prev, NULL);
  else
    gimple_seq_set_first (old_seq, NULL);

  return new_seq;
}


/* Replace the statement pointed-to by GSI to STMT.  If UPDATE_EH_INFO
   is true, the exception handling information of the original
   statement is moved to the new statement.  */

void
gsi_replace (gimple_stmt_iterator *gsi, gimple stmt, bool update_eh_info)
{
#if 0
  int eh_region;
#endif
  gimple orig_stmt = gsi_stmt (gsi);

  if (stmt == orig_stmt)
    return;

  gimple_set_locus (stmt, gimple_locus (orig_stmt));
  gsi_insert_before (gsi, stmt, GSI_SAME_STMT);
  gsi_remove (gsi, update_eh_info);
  gsi->stmt = stmt;

  /* FIXME tuples.  Enable after converting tree-eh.c  */
#if 0
  /* Preserve EH region information from the original statement, if
     requested by the caller.  */
  if (update_eh_info)
    {
      eh_region = lookup_stmt_eh_region (orig_stmt);
      if (eh_region >= 0)
	{
	  remove_stmt_from_eh_region (orig_stmt);
	  add_stmt_to_eh_region (stmt, eh_region);
	}
    }

  gimple_duplicate_stmt_histograms (cfun, stmt, cfun, orig_stmt);
  gimple_remove_stmt_histograms (cfun, orig_stmt);
  delink_stmt_imm_use (orig_stmt);
#endif

  /* FIXME tuples.  Enable after converting tree-ssa-operands.c.  */
#if 0
  mark_stmt_modified (stmt);
  update_modified_stmts (stmt);
#endif
}


/* Mark the statement T as modified, and update it.  */

static inline void
update_modified_stmt (gimple t)
{
  /* FIXME tuples: Perhaps we should cache the output from
     ssa_operands_active, or make it inline ??  */
  if (!ssa_operands_active ())
    return;
  update_stmt_if_modified (t);
}


/* Insert statement STMT before the statement pointed-to by iterator
   I, update STMT's basic block and scan it for new operands.  M
   specifies how to update iterator I after insertion (see enum
   gsi_iterator_update).  */

void
gsi_insert_before (gimple_stmt_iterator *i, gimple stmt,
		   enum gsi_iterator_update m)
{
  gimple_set_bb (stmt, i->bb);
  update_modified_stmt (stmt);
  gsi_link_before (i, stmt, m);
}


/* Like gsi_insert_before, but for all the statements in SEQ.  */

void
gsi_insert_seq_before (gimple_stmt_iterator *i, gimple_seq seq,
		       enum gsi_iterator_update m)
{
  gimple_stmt_iterator *gsi;

  for (gsi = gsi_start (seq); !gsi_end_p (gsi); gsi_next (gsi))
    gsi_insert_before (i, gsi_stmt (gsi), m);
}


/* Insert statement STMT after the statement pointed-to by iterator I,
   update STMT's basic block and scan it for new operands.  M
   specifies how to update iterator I after insertion (see enum
   gsi_iterator_update).  */

void
gsi_insert_after (gimple_stmt_iterator *i, gimple stmt,
		  enum gsi_iterator_update m)
{
  gimple_set_bb (stmt, i->bb);
  update_modified_stmt (stmt);
  gsi_link_after (i, stmt, m);
}


/* Like gsi_insert_after, but for all the statements in SEQ.  */

void
gsi_insert_seq_after (gimple_stmt_iterator *i, gimple_seq seq,
		      enum gsi_iterator_update m)
{
  gimple_stmt_iterator *gsi;

  for (gsi = gsi_start (seq); !gsi_end_p (gsi); gsi_next (gsi))
    gsi_insert_after (i, gsi_stmt (gsi), m);
}


/* Finds iterator for STMT.  */

gimple_stmt_iterator *
gsi_for_stmt (gimple stmt)
{
  gimple_stmt_iterator *gsi;

  for (gsi = gsi_start_bb (gimple_bb (stmt)); !gsi_end_p (gsi); gsi_next (gsi))
    if (gsi_stmt (gsi) == stmt)
      return gsi;

  gcc_unreachable ();
}


/* Move the statement at FROM so it comes right after the statement at TO.  */

void
gsi_move_after (gimple_stmt_iterator *from, gimple_stmt_iterator *to)
{
  gimple stmt = gsi_stmt (from);
  gsi_remove (from, false);

  /* We must have GSI_NEW_STMT here, as gsi_move_after is sometimes used to
     move statements to an empty block.  */
  gsi_insert_after (to, stmt, GSI_NEW_STMT);
}


/* Move the statement at FROM so it comes right before the statement at TO.  */

void
gsi_move_before (gimple_stmt_iterator *from, gimple_stmt_iterator *to)
{
  gimple stmt = gsi_stmt (from);
  gsi_remove (from, false);

  /* For consistency with gsi_move_after, it might be better to have
     GSI_NEW_STMT here; however, that breaks several places that expect
     that TO does not change.  */
  gsi_insert_before (to, stmt, GSI_SAME_STMT);
}


/* Move the statement at FROM to the end of basic block BB.  */

void
gsi_move_to_bb_end (gimple_stmt_iterator *from, basic_block bb)
{
  gimple_stmt_iterator *last = gsi_last (bb_seq (bb));

  /* Have to check gsi_end_p because it could be an empty block.  */
  if (!gsi_end_p (last) && is_ctrl_stmt (gsi_stmt (last)))
    gsi_move_before (from, last);
  else
    gsi_move_after (from, last);
}
