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
#include "tree.h"
#include "gimple.h"

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
      set_gimple_prev (head, gimple_prev (cur));
      if (gimple_prev (head))
	set_gimple_next (gimple_prev (head), head);
      else
	gimple_seq_set_first (i->seq, head);
      set_gimple_next (tail, cur);
      set_gimple_prev (cur, tail);
    }
  else
    {
      set_gimple_prev (head, gimple_seq_last (i->seq));
      if (gimple_prev (head))
	set_gimple_next (gimple_prev (head), head);
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


/* Links a sequence after the current statement.  */

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
      set_gimple_next (tail, gimple_next (cur));
      if (gimple_next (tail))
	set_gimple_prev (gimple_next (tail), tail);
      else
	gimple_seq_set_last (i->seq, tail);
      set_gimple_prev (head, cur);
      set_gimple_next (cur, head);
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


/* Links one gimple statement after the current statement.  */

void
gsi_link_after (gimple_stmt_iterator *i,
    		gimple g,
		enum gsi_iterator_update mode)
{
  struct gimple_sequence tseq;

  gimple_seq_init (&tseq);
  gimple_seq_add (&tseq, g);
  gsi_link_seq_after (i, &tseq, mode);
}


/* Remove the current stmt from the sequence.  The iterator is updated to
   point to the next statement.  */

void
gsi_delink (gimple_stmt_iterator *i)
{
  gimple cur = i->stmt;
  gimple next = gimple_next (cur);
  gimple prev = gimple_prev (cur);

  if (prev)
    set_gimple_next (prev, next);
  else
    gimple_seq_set_first (i->seq, next);
  if (next)
    set_gimple_prev (next, prev);
  else
    gimple_seq_set_last (i->seq, prev);

  /* Clear any links this statement may have, just in case someone is
     still using it.  */
  set_gimple_next (i->stmt, NULL);
  set_gimple_prev (i->stmt, NULL);

  i->stmt = next;
}


/* Move all statements in the sequence after I to a new sequence.  I
   itself is unchanged.  */

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
  set_gimple_next (cur, NULL);
  set_gimple_prev (next, NULL);

  return new_seq;
}


/* Move all statements in the sequence before I to a new sequence.  I
   is set to the head of the new list.  */

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
  set_gimple_prev (cur, NULL);
  if (prev)
    set_gimple_next (prev, NULL);
  else
    gimple_seq_set_first (old_seq, NULL);

  return new_seq;
}


/* Replace the statement pointed-to by GSI to STMT.  If UPDATE_EH_INFO
   is true, the exception handling information of the original
   statement is moved to the new statement.  */

void
gsi_replace (gimple_stmt_iterator *gsi, gimple stmt, bool update_eh_info ATTRIBUTE_UNUSED)
{
#if 0
  int eh_region;
#endif
  gimple orig_stmt = gsi_stmt (gsi);

  if (stmt == orig_stmt)
    return;

  set_gimple_locus (stmt, gimple_locus (orig_stmt));
  set_gimple_bb (stmt, gimple_bb (orig_stmt));

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

  gsi->stmt = stmt;
}
