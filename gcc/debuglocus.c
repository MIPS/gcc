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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "ggc.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "bitmap.h"
#include "tree-flow.h"
#include "timevar.h"
#include "tree-dump.h"
#include "tree-ssa-live.h"
#include "tree-pass.h"
#include "toplev.h"
#include "debuglocus.h"
#include "hashtab.h"


/* This file contains data and functions required to implement debuglocus 
   support. This is a mechanism for tracking debug information in the compiler
   by replacing the source locus field on some statements with entries into
   a debug table. This debuglocus entry also contains the original source locus
   allowing for transparent use of source locations on statements.  */

  
#define DEBUGLOCUS_VEC_SIZE	8192
#define DEBUGLOCUS_VEC_MEM	(DEBUGLOCUS_VEC_SIZE * sizeof (debuglocus))


/* Create and initialize a new debuglocus table.  */
static debuglocus_table_t *
init_debuglocus_table (void)
{
  debuglocus_table_t *tab;
  debuglocus_p dlocus;

  tab = (debuglocus_table_t *) xmalloc (sizeof (debuglocus_table_t));
  tab->table = VEC_alloc (debuglocus_p, heap, 100);
  dlocus = (debuglocus_p) xmalloc (DEBUGLOCUS_VEC_MEM);
  VEC_safe_push (debuglocus_p, heap, tab->table, dlocus);

  /* Reserve space numbner 0 for the NULL entry.  */
  tab->size = 1;
  return tab;
}


/* Create a debuglocus table for the current function.  */
void
create_debuglocus_table (void)
{
  cfun->debuglocus_table = init_debuglocus_table ();
}


/* Destroy the current function's debuglocus table.  */
void
destroy_debuglocus_table (void)
{
  cfun->debuglocus_table = NULL;
}


/* Return a pointer to the debuglocus in table TAB at index I.  */
static debuglocus_p
get_debuglocus_entry (debuglocus_table_t *tab, debuglocus_index i)
{
  int v,e;
  debuglocus_p table_vec;
  gcc_assert (tab != NULL);
  gcc_assert (i > 0 && i < tab->size);

  v = i / DEBUGLOCUS_VEC_SIZE;
  e = i % DEBUGLOCUS_VEC_SIZE;
  table_vec = VEC_index (debuglocus_p, tab->table, v);
  return &(table_vec[e]);
}


/* Return a pointer to the debuglocus entry at index I in the current table.  */
debuglocus_p
get_debuglocus (debuglocus_index i)
{
  return get_debuglocus_entry (cfun->debuglocus_table, i);
}


/* Create and return a pointer to a new debuglocus entry in table TAB. */
static debuglocus_p
new_debuglocus_entry (debuglocus_table_t *tab)
{
  int i;
  debuglocus_p dlocus;
  gcc_assert (tab != NULL);

  i = tab->size++;
  
  /* Check if we need to increase the size of the table vector.  */
  if (i % DEBUGLOCUS_VEC_SIZE == 0)
    {
      dlocus = (debuglocus_p) xmalloc (DEBUGLOCUS_VEC_MEM);
      VEC_safe_push (debuglocus_p, heap, tab->table, dlocus);
    }

  dlocus = get_debuglocus_entry (tab, i);

  /* Initialize the entry.  */
  dlocus->locus = UNKNOWN_LOCATION;
  dlocus->decl = NULL_TREE;
  dlocus->order = i;		    /* Initially the issuing index.  */
  dlocus->prev = dlocus->next = i;  /* Initialize the circular linked list.  */
  return get_debuglocus_entry (tab, i);
}


/* Create and return a pointer to a new debuglocus entry in the current 
   table.  */
debuglocus_p
create_debuglocus_entry (void)
{
  return new_debuglocus_entry (cfun->debuglocus_table);
}


/* Copy the debuglocus LOCUS. This requires duplicating the debuglocus, as well
   as any chain of debuglocus's attached to it.  */
source_location
create_duplicate_debuglocus (source_location locus)
{
  int root_i, new_i, src_i, dest_i, prev_dest_i;
  debuglocus_p root_p, new_p, src_p, dest_p, prev_dest_p;

  gcc_assert (IS_DEBUGLOCUS_P (locus));

  root_i = DEBUGLOCUS_INDEX (locus);   /* Root is the locus being copied.  */
  root_p = get_debuglocus (root_i);

  new_p = create_debuglocus_entry ();  /* New is the copy of root.  */
  /* The order field starts as the index of the new debuglocus.  */
  new_i = new_p->order;

  /* Copy the fields from the root node.  */
  new_p->decl = root_p->decl;
  new_p->locus = root_p->locus;
  new_p->order = root_p->order;

  prev_dest_p = new_p;  /* prev_dest is used for updating the links.  */
  prev_dest_i = new_i;

  /* Now duplicate and link in any other locus's which are linked to root.  */
  for (src_i = root_p->next; src_i != root_i; src_i = src_p->next)
   {
     src_p = get_debuglocus (src_i);  /* Src is the node being copied.  */
     dest_p = create_debuglocus_entry (); /* Dest is the new node.  */
     dest_i = dest_p->order;
     /* Initialize links  */
     prev_dest_p->next = dest_i;
     dest_p->prev = prev_dest_i;

     /* copy node values.  */
     dest_p->decl = src_p->decl;
     dest_p->locus = src_p->locus;
     dest_p->order = src_p->order;

     /* Now dest is the previous node in the new list.  */
     prev_dest_i = dest_i;
     prev_dest_p = dest_p;
   }

   /* Update the final link.  */
   new_p->prev = prev_dest_i;
   prev_dest_p->next = new_i;

  /* Return the debuglocus for the new copy.  */
  return new_i | DEBUGLOCUS_BIT;
}
