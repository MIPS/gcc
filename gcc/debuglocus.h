/* Routines for dealing with debuglocus
   Copyright (C) 2009 Free Software Foundation, Inc.
   Contributed by Andrew Macleod <amacleod@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef DEBUGLOCUS_H
#define DEBUGLOCUS_H

/* No entry. Used for terminating lists and returning no matches.  */
#define DEBUGLOCUS_NONE			0
#define IS_DEBUGLOCUS_P(LOCUS) 		(((LOCUS) & DEBUGLOCUS_BIT) != 0)
#define DEBUGLOCUS_INDEX(LOCUS) 	((LOCUS) & ~DEBUGLOCUS_BIT)
#define GET_DEBUGLOCUS(I)		(get_debuglocus (I))
#define GET_LOCUS_FROM_DEBUGLOCUS(DL)	(debuglocus_lookup_locus (DL))

struct debuglocus_entry_d GTY((skip)) {
  tree decl;			/* debug decl  */
  int order;			/* Linear ordering for emission sorting.  */
  source_location locus;	/* locus of statement.  */
  int prev;			/* prev link for multiple entries on a stmt.  */
  int next;			/* next link for multiple entries on a stmt.  */
};

typedef struct debuglocus_entry_d debuglocus;
typedef struct debuglocus_entry_d *debuglocus_p;


DEF_VEC_P(debuglocus_p);
DEF_VEC_ALLOC_P(debuglocus_p, heap);

/* The debuglocus table is implemented simply as an array of segments. This
   allows for simple growth of the table without having to relocate existing 
   memory in use.  The table can never shrink in size.  */

struct debuglocus_table_d GTY((skip)) {
  int size;			  /* Current number of debuglocus entries.  */
  VEC(debuglocus_p, heap) *table;   /* list of Table segments.  */
};

typedef struct debuglocus_table_d debuglocus_table_t;


typedef int debuglocus_index;

void create_debuglocus_table(void);
void destroy_debuglocus_table(void);
debuglocus_p create_debuglocus_entry (void);
debuglocus_p get_debuglocus (debuglocus_index);
source_location create_duplicate_debuglocus (source_location locus);


/* Copy a locus, performing a deep copy if it is a debuglocus.  */
static inline source_location 
copy_debuglocus (source_location locus)
{
  if (!IS_DEBUGLOCUS_P (locus))
    return locus;
  else
    return create_duplicate_debuglocus (locus);
}


/* Return the source locus associated with a debuglocus.  */
static inline source_location
debuglocus_lookup_locus (source_location dlocus)
{
  debuglocus_p ptr;

  gcc_assert (IS_DEBUGLOCUS_P (dlocus));
  ptr = get_debuglocus (DEBUGLOCUS_INDEX (dlocus));
  return ptr->locus;
}

#endif
