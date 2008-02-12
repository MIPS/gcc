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

/* Links the sequence of statements SEQ before the statement pointed
   by iterator I.  MODE indicates what to do with the iterator after
   insertion (see enum gsi_iterator_update).  */

void
gsi_link_seq_before (gimple_stmt_iterator *i, gimple_seq seq,
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


/* Links statement G before the statement pointed-to by iterator I.
   Updates iterator I according to MODE.  */

void
gsi_link_before (gimple_stmt_iterator *i, gimple g,
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


/* Move all statements in the sequence after I to a new sequence.  Return this
   new sequence.  */

gimple_seq
gsi_split_seq_after (gimple_stmt_iterator i)
{
  gimple cur, next;
  gimple_seq old_seq, new_seq;

  cur = i.stmt;

  /* How can we possibly split after the end, or before the beginning?  */
  gcc_assert (cur);
  next = gimple_next (cur);

  old_seq = i.seq;
  new_seq = gimple_seq_alloc ();

  gimple_seq_set_first (new_seq, next);
  gimple_seq_set_last (new_seq, gimple_seq_last (old_seq));
  gimple_seq_set_last (old_seq, cur);
  gimple_set_next (cur, NULL);
  gimple_set_prev (next, NULL);

  return new_seq;
}


/* Move all statements in the sequence before I to a new sequence.
   Return this new sequence.  I is set to the head of the new list.  */

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
  /* FIXME tuples.  */
#if 0
  int eh_region;
#endif
  gimple orig_stmt = gsi_stmt (*gsi);

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
  gimple_set_modified (stmt, true);
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
  gimple_stmt_iterator gsi;

  for (gsi = gsi_start (seq); !gsi_end_p (gsi); gsi_next (&gsi))
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
  /* Note that we cannot use statement iterators here because we are
     moving each statement from one list to the other, which confuses
     gsi_next.  */
  gimple stmt = gimple_seq_first (seq);
  while (stmt)
    {
      gimple next = gimple_next (stmt);
      gsi_insert_after (i, stmt, m);
      stmt = next;
    }
}


/* Finds iterator for STMT.  */

gimple_stmt_iterator
gsi_for_stmt (gimple stmt)
{
  gimple_stmt_iterator gsi;

  for (gsi = gsi_start_bb (gimple_bb (stmt)); !gsi_end_p (gsi); gsi_next (&gsi))
    if (gsi_stmt (gsi) == stmt)
      return gsi;

  gcc_unreachable ();
}


/* Move the statement at FROM so it comes right after the statement at TO.  */

void
gsi_move_after (gimple_stmt_iterator *from, gimple_stmt_iterator *to)
{
  gimple stmt = gsi_stmt (*from);
  gsi_remove (from, false);

  /* We must have GSI_NEW_STMT here, as gsi_move_after is sometimes used to
     move statements to an empty block.  */
  gsi_insert_after (to, stmt, GSI_NEW_STMT);
}


/* Move the statement at FROM so it comes right before the statement at TO.  */

void
gsi_move_before (gimple_stmt_iterator *from, gimple_stmt_iterator *to)
{
  gimple stmt = gsi_stmt (*from);
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
  gimple_stmt_iterator last = gsi_last (bb_seq (bb));

  /* Have to check gsi_end_p because it could be an empty block.  */
  if (!gsi_end_p (last) && is_ctrl_stmt (gsi_stmt (last)))
    gsi_move_before (from, &last);
  else
    gsi_move_after (from, &last);
}


/* Add STMT to the pending list of edge E.  No actual insertion is
   made until a call to gsi_commit_edge_inserts () is made.  */

void
gsi_insert_on_edge (edge e, gimple stmt)
{
  if (PENDING_STMT (e) == NULL)
    PENDING_STMT (e) = gimple_seq_alloc ();

  gimple_seq_add (PENDING_STMT (e), stmt);
}

/* Add the sequence of statements SEQ to the pending list of edge E.
   No actual insertion is made until a call to gsi_commit_edge_inserts
   is made.  */

void
gsi_insert_seq_on_edge (edge e, gimple_seq seq)
{
  gimple_stmt_iterator gsi;

  for (gsi = gsi_start (seq); !gsi_end_p (gsi); gsi_next (&gsi))
    gsi_insert_on_edge (e, gsi_stmt (gsi));
}


/* Insert the statement pointed-to by GSI into edge E.  Every attempt
   is made to place the statement in an existing basic block, but
   sometimes that isn't possible.  When it isn't possible, the edge is
   split and the statement is added to the new block.

   In all cases, the returned *GSI points to the correct location.  The
   return value is true if insertion should be done after the location,
   or false if it should be done before the location.  If new basic block
   has to be created, it is stored in *NEW_BB.  */

static bool
gimple_find_edge_insert_loc (edge e, gimple_stmt_iterator *gsi,
			     basic_block *new_bb)
{
  basic_block dest, src;
  gimple tmp;

  dest = e->dest;

  /* If the destination has one predecessor which has no PHI nodes,
     insert there.  Except for the exit block.

     The requirement for no PHI nodes could be relaxed.  Basically we
     would have to examine the PHIs to prove that none of them used
     the value set by the statement we want to insert on E.  That
     hardly seems worth the effort.  */
restart:
  if (single_pred_p (dest)
      && ! phi_nodes (dest)
      && dest != EXIT_BLOCK_PTR)
    {
      *gsi = gsi_start_bb (dest);
      if (gsi_end_p (*gsi))
	return true;

      /* Make sure we insert after any leading labels.  */
      tmp = gsi_stmt (*gsi);
      while (gimple_code (tmp) == GIMPLE_LABEL)
	{
	  gsi_next (gsi);
	  if (gsi_end_p (*gsi))
	    break;
	  tmp = gsi_stmt (*gsi);
	}

      if (gsi_end_p (*gsi))
	{
	  *gsi = gsi_last (bb_seq (dest));
	  return true;
	}
      else
	return false;
    }

  /* If the source has one successor, the edge is not abnormal and
     the last statement does not end a basic block, insert there.
     Except for the entry block.  */
  src = e->src;
  if ((e->flags & EDGE_ABNORMAL) == 0
      && single_succ_p (src)
      && src != ENTRY_BLOCK_PTR)
    {
      *gsi = gsi_last (bb_seq (src));
      if (gsi_end_p (*gsi))
	return true;

      tmp = gsi_stmt (*gsi);
      if (!stmt_ends_bb_p (tmp))
	return true;

      if (gimple_code (tmp) == GIMPLE_RETURN)
        {
	  gsi_prev (gsi);
	  return true;
        }
    }

  /* Otherwise, create a new basic block, and split this edge.  */
  dest = split_edge (e);
  if (new_bb)
    *new_bb = dest;
  e = single_pred_edge (dest);
  goto restart;
}


/* Similar to gsi_insert_on_edge+gsi_commit_edge_inserts.  If a new
   block has to be created, it is returned.  */

basic_block
gsi_insert_on_edge_immediate (edge e, gimple stmt)
{
  gimple_stmt_iterator gsi;
  basic_block new_bb = NULL;

  gcc_assert (!PENDING_STMT (e));

  if (gimple_find_edge_insert_loc (e, &gsi, &new_bb))
    gsi_insert_after (&gsi, stmt, GSI_NEW_STMT);
  else
    gsi_insert_before (&gsi, stmt, GSI_NEW_STMT);

  return new_bb;
}


/* This routine will commit all pending edge insertions, creating any new
   basic blocks which are necessary.  */

void
gsi_commit_edge_inserts (void)
{
  basic_block bb;
  edge e;
  edge_iterator ei;

  gsi_commit_one_edge_insert (single_succ_edge (ENTRY_BLOCK_PTR), NULL);

  FOR_EACH_BB (bb)
    FOR_EACH_EDGE (e, ei, bb->succs)
      gsi_commit_one_edge_insert (e, NULL);
}


/* Commit insertions pending at edge E. If a new block is created, set NEW_BB
   to this block, otherwise set it to NULL.  */

void
gsi_commit_one_edge_insert (edge e, basic_block *new_bb)
{
  if (new_bb)
    *new_bb = NULL;

  if (PENDING_STMT (e))
    {
      gimple_stmt_iterator gsi;
      gimple_seq seq = PENDING_STMT (e);

      PENDING_STMT (e) = NULL;

      if (gimple_find_edge_insert_loc (e, &gsi, new_bb))
	gsi_link_seq_after (&gsi, seq, GSI_NEW_STMT);
      else
	gsi_link_seq_before (&gsi, seq, GSI_NEW_STMT);
    }
}
