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

/* Iterator object for GIMPLE statement sequences.  */

typedef struct {
    gimple *ptr;
    gs_seq seq;
} gimple_stmt_iterator;

/* Return a new iterator initially pointing to GS_SEQ's first statement.  */

static inline gimple_stmt_iterator
gsi_start (gs_seq seq)
{
  gimple_stmt_iterator i;

  i.ptr = &GS_SEQ_FIRST (seq);
  i.seq = seq;

  return i;
}

/* Return a new iterator initially pointing to GS_SEQ's last statement.  */

static inline gimple_stmt_iterator
gsi_last (gs_seq seq)
{
  gimple_stmt_iterator i;

  i.ptr = &GS_SEQ_LAST (seq);
  i.seq = seq;

  return i;
}

/* Return TRUE if at the end of I.  */

static inline bool
gsi_end_p (gimple_stmt_iterator i)
{
  return *i.ptr == NULL;
}

/* Return TRUE if we're one statement before the end of I.  */

static inline bool
gsi_one_before_end_p (gimple_stmt_iterator i)
{
  return i.ptr == &GS_SEQ_LAST (i.seq);
}

/* Return the next gimple statement in I.  */

static inline void
gsi_next (gimple_stmt_iterator *i)
{
  i->ptr = &GS_NEXT (*i->ptr);
}

/* Return the previous gimple statement in I.  */

static inline void
gsi_prev (gimple_stmt_iterator *i)
{
  i->ptr = &GS_PREV (*i->ptr);
}

/* Return a pointer to the current stmt.  */

static inline gimple *
gsi_stmt_ptr (gimple_stmt_iterator i)
{
  return i.ptr;
}

/* Return the current stmt.  */

static inline gimple
gsi_stmt (gimple_stmt_iterator i)
{
  return *i.ptr;
}

#endif /* GCC_SEQ_ITERATOR_H */
