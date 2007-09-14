/* Iterator routines for GIMPLE statements.
   Copyright (C) 2007 Free Software Foundation, Inc.
   Contributed by Aldy Hernandez  <aldyh@redhat.com>

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

#ifndef GCC_SEQ_ITERATOR_H
#define GCC_SEQ_ITERATOR_H

#include "ggc.h"

/* Iterator object for GIMPLE statement sequences.  */

typedef struct {
    gimple stmt;
    gimple_seq seq;
} gimple_stmt_iterator;

/* Return a new iterator initially pointing to GIMPLE_SEQ's first statement.  */

static inline gimple_stmt_iterator *
gsi_start (gimple_seq seq)
{
  gimple_stmt_iterator *i = ggc_alloc_cleared (sizeof (gimple_stmt_iterator));

  if (seq)
    {
      i->stmt = gimple_seq_first (seq);
      i->seq = seq;
    }

  return i;
}

/* Return a new iterator initially pointing to GIMPLE_SEQ's last statement.  */

static inline gimple_stmt_iterator *
gsi_last (gimple_seq seq)
{
  gimple_stmt_iterator *i = ggc_alloc_cleared (sizeof (gimple_stmt_iterator));

  i->stmt = gimple_seq_last (seq);
  i->seq = seq;

  return i;
}

/* Return TRUE if at the end of I.  */

static inline bool
gsi_end_p (gimple_stmt_iterator *i)
{
  return i->stmt == NULL;
}

/* Return TRUE if we're one statement before the end of I.  */

static inline bool
gsi_one_before_end_p (gimple_stmt_iterator *i)
{
  return i->stmt == gimple_seq_last (i->seq);
}

/* Return the next gimple statement in I.  */

static inline void
gsi_next (gimple_stmt_iterator *i)
{
#if defined ENABLE_GIMPLE_CHECKING
  /* The last statement of the sequence should not have anything
     chained after it.  */
  gimple next = gimple_next (i->stmt);
  if (i->stmt == gimple_seq_last (i->seq))
    gcc_assert (next == NULL);
#endif
  i->stmt = gimple_next (i->stmt);
}

/* Return the previous gimple statement in I.  */

static inline void
gsi_prev (gimple_stmt_iterator *i)
{
#if defined ENABLE_GIMPLE_CHECKING
  /* The first statement of the sequence should not have anything
     chained before it.  */
  gimple prev = gimple_prev (i->stmt);
  if (i->stmt == gimple_seq_first (i->seq))
    gcc_assert (prev == NULL);
#endif
  i->stmt = gimple_prev (i->stmt);
}

/* Return the current stmt.  */

static inline gimple
gsi_stmt (gimple_stmt_iterator *i)
{
  return i->stmt;
}


/* Remove the current stmt from the sequence.  The iterator is updated to
   point to the next statement.

   When REMOVE_EH_INFO is true we remove the statement pointed to by
   iterator I from the EH tables.  Otherwise we do not modify the EH
   tables.

   Generally, REMOVE_EH_INFO should be true when the statement is going to
   be removed from the IL and not reinserted elsewhere.  */

static inline void
gsi_remove (gimple_stmt_iterator *i, bool remove_eh_info)
{
  gimple stmt = i->stmt;
  gimple_seq seq = i->seq;
  gsi_next (i);
  gimple_remove (stmt, seq, remove_eh_info);
}

enum gsi_iterator_update
{
  GSI_NEW_STMT,		/* Only valid when single statement is added, move
			   iterator to it.  */
  GSI_SAME_STMT,	/* Leave the iterator at the same statement.  */
  GSI_CONTINUE_LINKING	/* Move iterator to whatever position is suitable
			   for linking other statements in the same
			   direction.  */
};

void gsi_link_seq_before (gimple_stmt_iterator *, gimple_seq,
			  enum gsi_iterator_update);
void gsi_link_before (gimple_stmt_iterator *, gimple,
    		      enum gsi_iterator_update);
void gsi_link_seq_after (gimple_stmt_iterator *, gimple_seq,
			 enum gsi_iterator_update);
void gsi_link_after (gimple_stmt_iterator *, gimple, enum gsi_iterator_update);
gimple_seq gsi_split_seq_after (const gimple_stmt_iterator *);
gimple_seq gsi_split_seq_before (gimple_stmt_iterator *);
void gsi_replace (gimple_stmt_iterator *, gimple, bool);
void gsi_insert_before (gimple_stmt_iterator *, gimple,
			enum gsi_iterator_update);
void gsi_insert_seq_before (gimple_stmt_iterator *, gimple_seq,
		       enum gsi_iterator_update);
void gsi_insert_after (gimple_stmt_iterator *, gimple,
		       enum gsi_iterator_update);
void gsi_insert_seq_after (gimple_stmt_iterator *, gimple_seq,
			   enum gsi_iterator_update);
gimple_stmt_iterator *gsi_for_stmt (gimple);
void gsi_move_after (gimple_stmt_iterator *, gimple_stmt_iterator *);
void gsi_move_before (gimple_stmt_iterator *, gimple_stmt_iterator *);
void gsi_move_to_bb_end (gimple_stmt_iterator *, struct basic_block_def *);

#endif /* GCC_SEQ_ITERATOR_H */
