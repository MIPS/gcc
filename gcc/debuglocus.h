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

/* This is an entry in the debuglocus table.
   DECL indicates the user variable that the entry is for. 
   LOCUS is the location the debug value is associated with.
   ORIGINAL_LOCUS is the location the current statement.
   if EXPRESSION is NULL, then it defaults to the value of the 
   expression/statement on which the debuglocus it attached, otherwise it is
   defined by the espression.  Initially this is just CONST or DECL.
   PREV and NEXT are links within the table for chaining entries.  */

struct debuglocus_entry_d GTY(()) {
  tree decl;			  /* Debug decl  */
  source_location locus;	  /* Locus of statement.  */
  source_location original_locus; /* Original locus of current statement.  */
  tree expression;                /* Optional expression node.  */
  int prev;			  /* Prev link for multiple entries.  */
  int next;			  /* Next link for multiple entries.  */
};


/* There are 2 source location fields associated with a debuglocus.
   LOCUS is the source_location for the statement with which the debug
   entry belong. Initially ORIGINAL_LOCUS is this same value. 
   They can diverge when a statement is deleted which has a debuglocus, and 
   that debuglocus is moved to another statement which has a different source 
   location, but NOT a debuglocus of it's own.   ie
     [ a.c : 8 ]  t_4 = foo ()
     <...>
     [ a.c : 44, a ]  a_8 = t_4
    if coalescing removes the second statement, the debuglocus will be attached
    to the first statement.  When moved, ORIG_LOCUS will be set to the 
    [ a.c : 8 ] location, and LOCUS will still have [ a.c : 44, a].  This means
    the call to foo can still have it's correct location entry, and the correct
    location for the debug assignemnt to 'a' is also still available.

    Note that when debuglocus's are chained together, the ORIGINAL_LOCUS field
    for everything except the initial one in the list is ignored. Only the
    initial debuglocus is considered to contain a LOCUS for a statement.  */



typedef struct debuglocus_entry_d debuglocus;
typedef struct debuglocus_entry_d *debuglocus_p;

/* Number of debuglocus entries per segment.  */
#define DEBUGLOCUS_VEC_SIZE	8192

/* Debuglocus segments for use in the debuglocus table, pathetically
   laid out so GTY can understand what's inside the vector that's
   inside the table.  */
struct debuglocus_segment_d GTY(()) {
  debuglocus entries[DEBUGLOCUS_VEC_SIZE];
};

typedef struct debuglocus_segment_d *debuglocus_segment_p;

DEF_VEC_P(debuglocus_segment_p);
DEF_VEC_ALLOC_P(debuglocus_segment_p, gc);

/* The debuglocus table is implemented simply as an array of segments. This
   allows for simple growth of the table without having to relocate existing 
   memory in use.  The table can never shrink in size.  */

struct debuglocus_table_d GTY(()) {
  /* Total number of debuglocus entries.  */
  unsigned int size;
  /* Index of the first item in the free list.

     The free list is a singly linked list of debuglocus's that have
     been freed and can thus be recycled.  For each entry, DECL must
     be NULL.  The items are linked by the NEXT field and the PREV
     field is ignored.  */
  int free_list_idx;
  /* Vector of debuglocus segments.  */
  VEC(debuglocus_segment_p, gc) *table;
};

typedef struct debuglocus_table_d debuglocus_table_t;


void create_debuglocus_table(void);
void destroy_debuglocus_table(void);
debuglocus_p create_debuglocus_entry (void);
void debuglocus_free_entry (debuglocus_p);
debuglocus_p get_debuglocus (source_location locus);
source_location create_duplicate_debuglocus (source_location locus);
bool decl_needs_debuglocus_p (tree);
debuglocus_p create_debuglocus_for_decl (tree);
source_location create_debuglocus_for_decl_and_locus (tree, source_location);
void replace_gimple_locus_with_debuglocus (gimple, debuglocus_p);
void replace_phiarg_locus_with_debuglocus (gimple, int, debuglocus_p);
void merge_debuglocus (debuglocus_p, debuglocus_p);
bitmap current_debuglocus_bitmap (void);
void debuglocus_bitmap_populate (bitmap, FILE *, bool);
void debuglocus_bitmap_verify (FILE *, bitmap, bitmap, unsigned int);
void set_debuglocus_expr(source_location, tree);
void orphan_debuglocus (gimple);



/* Iterate over the different debuglocus's in a single list. No iterations are 
   performed if their locus is just a regular source location.  

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

/* Another iterator, only this just returns the DECL in each debuglocus.  */
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
      return ptr->original_locus;
    }
  else
    return dlocus;
}
#endif
