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

struct debuglocus_entry_d GTY((skip)) {
  tree decl;			/* debug decl  */
  int order;			/* Ordering for emission sorting.  */
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
  unsigned int size;		    /* Current number of debuglocus entries.  */
  VEC(debuglocus_p, heap) *table;   /* list of Table segments.  */
};

typedef struct debuglocus_table_d debuglocus_table_t;


void create_debuglocus_table(void);
void destroy_debuglocus_table(void);
debuglocus_p create_debuglocus_entry (void);
debuglocus_p get_debuglocus (source_location locus);
source_location create_duplicate_debuglocus (source_location locus);
bool decl_needs_debuglocus_p (tree);
debuglocus_p create_debuglocus_for_decl (tree);
source_location create_debuglocus_for_decl_and_locus (tree, source_location);
void replace_gimple_locus_with_debuglocus (gimple, debuglocus_p);
void merge_debuglocus (debuglocus_p, debuglocus_p);
debuglocus_p find_debuglocus (gimple, tree, source_location);
debuglocus_p find_and_detach_debuglocus (gimple, tree, source_location);
source_location find_and_detach_or_create_debuglocus (gimple, tree, source_location);

/* Iterate over the different debuglocus's in a single list. No iterations are 
   performed if ther locus is just a regular source location.  

   usage:
   source_location locus = gimple_location (stmt);
   debuglocus_iterator iter;
   debuglocus_p ptr;
   FOR_EACH_DEBUGLOCUS (locus, ptr, iter)
     dump_debuglocus_pointer (ptr);		
   if (!ptr)
     printf ("loop was exited early\n");
     
   ptr is guaranteed to be NULL if the loop is allowed to execute to the
   end, otherwise it will have the value when the exits happens.  
   
   This is an 'unsafe' iteration, meaning the list cannot be modified on the
   fly while traversing the list.  */

typedef struct {
  debuglocus_p start_dlocus;
  debuglocus_p current_dlocus;
} debuglocus_iterator;

debuglocus_p debuglocus_iter_start (debuglocus_iterator *, source_location);
debuglocus_p debuglocus_iter_next (debuglocus_iterator *);

#define FOR_EACH_DEBUGLOCUS(LOCUS, DLOCUS_P, ITER)		\
  for (DLOCUS_P = debuglocus_iter_start (&(ITER), (LOCUS));	\
       (ITER).current_dlocus != NULL;				\
       DLOCUS_P = debuglocus_iter_next (&(ITER)))

tree debuglocus_var_iter_start (debuglocus_iterator *, source_location);
tree debuglocus_var_iter_next (debuglocus_iterator *);

/* Another iterator, onoly this just returns the DECL in each debuglocus.  */
#define FOR_EACH_DEBUGLOCUS_VAR(LOCUS, VAR, ITER)		\
  for (VAR = debuglocus_var_iter_start (&(ITER), (LOCUS));	\
       (ITER).current_dlocus != NULL;				\
       VAR = debuglocus_var_iter_next (&(ITER)))


/* Return true if LOCUS is a debuglocus.  */
static inline bool
is_debuglocus (source_location locus)
{
  return (locus & DEBUGLOCUS_BIT) != 0;
}


/* Copy a locus, performing a deep copy if it is a debuglocus.  */
static inline source_location 
copy_debuglocus (source_location locus)
{
  if (!is_debuglocus (locus))
    return locus;
  else
    return create_duplicate_debuglocus (locus);
}


/* Return the source locus associated with a debuglocus.  */
static inline source_location
locus_from_debuglocus (source_location dlocus)
{
  if (is_debuglocus (dlocus))
    {
      debuglocus_p ptr = get_debuglocus (dlocus);
      return ptr->locus;
    }
  else
    return dlocus;
}
#endif
