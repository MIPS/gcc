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
#include "input.h"


/* This file contains data and functions required to implement debuglocus 
   support. This is a mechanism for tracking debug information in the compiler
   by replacing the source locus field on some statements with entries into
   a debug table. This debuglocus entry also contains the original source locus
   allowing for transparent use of source locations on statements.  */

  
#define DEBUGLOCUS_INDEX(LOCUS) ((LOCUS) & ~DEBUGLOCUS_BIT)
#define DEBUGLOCUS_VEC_MEM	(sizeof (struct debuglocus_segment_d))


/* Create and initialize a new debuglocus table.  */
static debuglocus_table_t *
init_debuglocus_table (void)
{
  debuglocus_table_t *tab;
  debuglocus_segment_p seg;

  tab = (debuglocus_table_t *) ggc_alloc (sizeof (debuglocus_table_t));
  tab->table = VEC_alloc (debuglocus_segment_p, gc, 100);
  seg = (debuglocus_segment_p) ggc_alloc_cleared (DEBUGLOCUS_VEC_MEM);
  VEC_safe_push (debuglocus_segment_p, gc, tab->table, seg);

  /* Reserve space number 0 for the NULL entry.  */
  tab->size = 1;
  return tab;
}


static GTY(()) debuglocus_table_t *debugtable = NULL;

/* Create the current debuglocus table.  */
void
create_debuglocus_table (void)
{
  if (debugtable == NULL)
    debugtable = init_debuglocus_table ();
}


/* Destroy the current debuglocus table.  */
void
destroy_debuglocus_table (void)
{
  if (debugtable)
    {
      unsigned int x;
      for (x = 0; x < VEC_length (debuglocus_segment_p, debugtable->table); x++)
	{
	  /* I wonder if it's worth it to free the segments individually, or let
	     GC do it's thing.  */
	  ggc_free (VEC_index (debuglocus_segment_p, debugtable->table, x));
	}
      VEC_free (debuglocus_segment_p, gc, debugtable->table);
      ggc_free (debugtable);
    }
  debugtable = NULL;
}


/* Return the currently active debugtable.  */
static inline debuglocus_table_t *
current_debuglocus_table (void)
{
  return debugtable;
}


/* Return a pointer to the debuglocus in table TAB at index I.  */
static debuglocus_p
get_debuglocus_entry (debuglocus_table_t *tab, unsigned int i)
{
  int v,e;
  debuglocus_segment_p seg;
  gcc_assert (tab != NULL);
  gcc_assert (i > 0 && i < tab->size);

  v = i / DEBUGLOCUS_VEC_SIZE;
  e = i % DEBUGLOCUS_VEC_SIZE;
  seg = VEC_index (debuglocus_segment_p, tab->table, v);
  return &seg->entries[e];
}


/* Return a pointer to the debuglocus entry at index I in the current table.  */
debuglocus_p
get_debuglocus (source_location dlocus)
{
  gcc_assert (is_debuglocus (dlocus));

  dlocus = DEBUGLOCUS_INDEX (dlocus);
  return get_debuglocus_entry (current_debuglocus_table (), dlocus);
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
      debuglocus_segment_p seg;
      seg = (debuglocus_segment_p) ggc_alloc_cleared (DEBUGLOCUS_VEC_MEM);
      VEC_safe_push (debuglocus_segment_p, gc, tab->table, seg);
    }

  dlocus = get_debuglocus_entry (tab, i);

  /* Initialize the entry.  */
  dlocus->locus = UNKNOWN_LOCATION;
  dlocus->decl = NULL_TREE;
  dlocus->order = i;		    /* Initially the issuing index.  */
  dlocus->prev = dlocus->next = i;  /* Initialize the circular linked list.  */
  return dlocus;
}


/* Create and return a pointer to a new debuglocus entry in the current 
   table.  */
debuglocus_p
create_debuglocus_entry (void)
{
  return new_debuglocus_entry (current_debuglocus_table ());
}


/* Copy the debuglocus LOCUS. This requires duplicating the debuglocus, as well
   as any chain of debuglocus's attached to it.  */
source_location
create_duplicate_debuglocus (source_location locus)
{
  int root_i, new_i, src_i, dest_i, prev_dest_i;
  debuglocus_p root_p, new_p, src_p, dest_p, prev_dest_p;

  gcc_assert (is_debuglocus (locus));

  root_i = DEBUGLOCUS_INDEX (locus);   /* Root is the locus being copied.  */
  root_p = get_debuglocus_entry (current_debuglocus_table (), root_i);

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
     /* Src is the node being copied. Dest is the new node.  */
     src_p = get_debuglocus_entry (current_debuglocus_table (), src_i);  
     dest_p = create_debuglocus_entry (); 
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


/* Decide whether VAR should have a debuglocus emitted.  */
bool
decl_needs_debuglocus_p (tree var)
{
  if (TREE_CODE (var) != VAR_DECL
      && TREE_CODE (var) != PARM_DECL)
    return false;

  if (DECL_IGNORED_P (var))
    return false;

  if (!DECL_NAME (var)) {
      tree origin = DECL_ABSTRACT_ORIGIN (var);

      if (!origin)
	return false;

      if (!DECL_P (origin))
	return false;

      if (!DECL_NAME (origin))
	return false;
    }

  if (!is_gimple_reg (var))
    return false;

  return true;
}


/* Return the source_location setting for the debuglocus table entry DLOCUS.  */
static source_location 
debuglocus_index_from_pointer (debuglocus_p dlocus)
{
  unsigned int x;
  source_location idx = UNKNOWN_LOCATION;
  debuglocus_table_t *tab = current_debuglocus_table();

  for (x = 0; x  < VEC_length (debuglocus_segment_p, tab->table); x++)
    {
      debuglocus_segment_p seg = VEC_index (debuglocus_segment_p, tab->table, x);
      debuglocus_p ptr = &seg->entries[0];

      /* Find the table this pointer belongs to.  */
      if (ptr <= dlocus && dlocus < ptr + DEBUGLOCUS_VEC_SIZE)
        {
          idx = ((x * DEBUGLOCUS_VEC_SIZE) + (dlocus - ptr));
	  break;
	}
    }

  gcc_assert (idx != UNKNOWN_LOCATION);
  gcc_assert (idx > 0 && idx < tab->size);

  return idx;
}


/* Return the source_location setting for the debuglocus table entry DLOCUS.  */
static source_location 
debuglocus_from_pointer (debuglocus_p dlocus)
{
  source_location res = debuglocus_index_from_pointer (dlocus);
  res = res | DEBUGLOCUS_BIT;
  gcc_assert (dlocus == get_debuglocus (res));

  return res;
}


/* Create and return a new debuglocus entry for a decl VAR.  */
debuglocus_p 
create_debuglocus_for_decl (tree var)
{
  debuglocus_p ptr = create_debuglocus_entry ();
  ptr->decl = var;
  return ptr;
}


/* Create a new debuglocus entry for decl VAR and source location LOCUS.  */
source_location
create_debuglocus_for_decl_and_locus (tree var, source_location locus)
{
  debuglocus_p ptr = create_debuglocus_for_decl (var);
  ptr->locus = locus;
  return debuglocus_from_pointer (ptr);
}


/* Replace the locus in STMT with the debuglocus entry DLOCUS.  */
void
replace_gimple_locus_with_debuglocus (gimple stmt, debuglocus_p dlocus)
{
  source_location locus = gimple_location (stmt);

  /* Conflicting locus's shouldn't occurr.  */
  gcc_assert (dlocus->locus == UNKNOWN_LOCATION || 
	      locus == UNKNOWN_LOCATION  || 
	      locus == dlocus->locus);

  if (locus != dlocus->locus)
    {
      if (dlocus->locus == UNKNOWN_LOCATION)
        dlocus->locus = locus;
    }
 
  /* If stmt already has a debuglocus, then merge this one.  Typically this
     happens when there are multiple results, as in an ASM */
  if (is_debuglocus (locus))
    {
      debuglocus_p curr = get_debuglocus (locus);
      merge_debuglocus (curr, dlocus);
    } 
  else
    gimple_set_location (stmt, debuglocus_from_pointer (dlocus));
}


/* Merge the debuglocus list FROM to TO.  */
void
merge_debuglocus (debuglocus_p to, debuglocus_p from)
{
  source_location from_last_i, to_last_i, tmp;
  debuglocus_p from_last_p, to_last_p;
  debuglocus_table_t *tab = current_debuglocus_table ();

  from_last_i = from->prev;
  from_last_p = get_debuglocus_entry (tab, from_last_i);
  to_last_i = to->prev;
  to_last_p = get_debuglocus_entry (tab, to_last_i);
  
  /* Update the circular list next fields.  */
  tmp = to_last_p->next;
  to_last_p->next = from_last_p->next;
  from_last_p->next = tmp;

  /* Update the circular list prev fields.  */
  tmp = to->prev;
  to->prev= from->prev;
  from->prev = tmp;
}


debuglocus_p
find_debuglocus (gimple stmt, tree decl, source_location locus)
{
  source_location dlocus = gimple_location (stmt);
  debuglocus_iterator iter;
  debuglocus_p ptr;

  if (!is_debuglocus (dlocus))
    return NULL;
  
  /* Find a matching entry.  */
  FOR_EACH_DEBUGLOCUS (dlocus, ptr, iter)
    if (ptr->decl == decl && ptr->locus == locus)
      break;

  return ptr;
}


/* Examine the debuglocus list on STMT, looking for an entry for DECL with
   location LOCUS.  If found, delink it from the list and return a pointer.
   Otherwise return NULL.  */
debuglocus_p 
find_and_detach_debuglocus (gimple stmt, tree decl, source_location locus)
{
  debuglocus_p ptr = find_debuglocus (stmt, decl, locus);

  /* If an entry is found, remove it from the list and return it.  */
  if (ptr)
    {
      int self = debuglocus_index_from_pointer (ptr);
      debuglocus_p prev, next;
      debuglocus_table_t * tab;

      /* If this is the original debuglocus on the stmt, leave it since the
         stmt is presumably still live and will need it.  */
      if (gimple_location (stmt) == (DEBUGLOCUS_BIT | self))
        return NULL;

      /* Make sure this isnt the only object in the list.  */
      gcc_assert (ptr->next != self || ptr->prev != self);
      gcc_assert (ptr->next != ptr->prev);

      /* Update the links for the list and the entry.  */
      tab = current_debuglocus_table ();
      prev = get_debuglocus_entry (tab, ptr->prev);
      next = get_debuglocus_entry (tab, ptr->next);
      prev->next = ptr->next;
      next->prev = ptr->prev;
      ptr->next = ptr->prev = self;
    }

  return ptr;
}


/* Examine the debuglocus list on STMT, looking for an entry for DECL with
   location DLOCUS.  If found, delink it from the list and return a pointer.
   Otherwise create a new debuglocus for the source location DLOCUS and
   return it.  */
source_location
find_and_detach_or_create_debuglocus (gimple stmt, tree decl, 
				      source_location dlocus)
{
  debuglocus_p ptr = find_and_detach_debuglocus (stmt, decl, dlocus);
  source_location locus = locus_from_debuglocus (dlocus);
  if (!ptr)
    {
      ptr = create_debuglocus_for_decl (decl);
      ptr->locus = locus;
    }

  gcc_assert (ptr != NULL);

  return debuglocus_from_pointer (ptr);
}


/* Initialize iterator ITER over the list of debuglocus for LOCUS.  Return the
   first pointer or NULL.  */
debuglocus_p
debuglocus_iter_start (debuglocus_iterator *iter, source_location locus)
{
  if (!is_debuglocus (locus))
    {
      iter->start_dlocus = NULL;
      iter->current_dlocus = NULL;
      return NULL;
    }
  iter->start_dlocus = iter->current_dlocus = get_debuglocus (locus);
  return iter->current_dlocus;
}


/* Move to the next debuglocus for ITER. Return the pointer or NULL.  */
debuglocus_p 
debuglocus_iter_next (debuglocus_iterator * iter)
{
  debuglocus_p next;

  next = get_debuglocus_entry (current_debuglocus_table (), 
			       iter->current_dlocus->next);

  /* If this matches the first one visited, the list is done.  */
  if (next == iter->start_dlocus)
    next = NULL;

  iter->current_dlocus = next;
  return next;
}


/* Initialize iterator ITER over the list of debuglocus for LOCUS.  Return the
   first decl or NULL.  */
tree
debuglocus_var_iter_start (debuglocus_iterator *iter, source_location locus)
{
  if (!is_debuglocus (locus))
    {
      iter->start_dlocus = NULL;
      iter->current_dlocus = NULL;
      return NULL;
    }
  iter->start_dlocus = iter->current_dlocus = get_debuglocus (locus);
  return iter->current_dlocus->decl;
}


/* Move to the next debuglocus for ITER. Return the pointer or NULL.  */
tree
debuglocus_var_iter_next (debuglocus_iterator * iter)
{
  debuglocus_p next;

  next = get_debuglocus_entry (current_debuglocus_table (), 
			       iter->current_dlocus->next);

  /* If this matches the first one visited, the list is done.  */
  if (next == iter->start_dlocus)
    next = NULL;
  iter->current_dlocus = next;

  if (next)
    return next->decl;
  return NULL;
}

/* Debuglocus verification routines.  */


/* Debuglocus bitmap for entire translation unit.  */
bitmap_head global_dl_bitmap;

/* Callback WI info for the walkers.  */
struct dlb_info
{
  /* The bitmap.  */
  bitmap bits;
  /* DUMPFILE to dump errors out to.  */
  FILE *dumpfile;
  /* TRUE if BITS is the bitmap for BEFORE the pass ran.  */
  bool before_pass_p;
};

/* If the given LOCUS is a debuglocus, set the corresponding bit in
   bitmap B.  Complain about shared debuglocus entries.

   DUMPFILE is the dump file to output errors to.

   BEFORE_PASS_P is true if this is the bitmap before the pass ran,
   or false if the bitmap is for after the pass completed.  */
static void
debuglocus_bitmap_populate_helper (bitmap b, location_t locus,
				   FILE *dumpfile, bool before_pass_p)
{
  if (is_debuglocus (locus))
    {
      unsigned int i = DEBUGLOCUS_INDEX (locus);

      if (bitmap_bit_p (b, i))
	fprintf (dumpfile, "Duplicated debuglocus %s this pass: %u\n",
		 before_pass_p ? "before" : "after", i);
      else
	bitmap_set_bit (b, (int) i);
    }
}

/* Callback to walk gimple statements.  Populate debuglocus bitmap
   with each statement's debuglocus.  */
static tree
debuglocus_bitmap_populate_gimple (gimple_stmt_iterator *gsi,
				   bool *handled_ops ATTRIBUTE_UNUSED,
				   struct walk_stmt_info *wi)
{
  gimple stmt = gsi_stmt (*gsi);
  struct dlb_info *dlb = (struct dlb_info *)wi->info;

  debuglocus_bitmap_populate_helper (dlb->bits, gimple_location (stmt),
				     dlb->dumpfile, dlb->before_pass_p);
  return NULL_TREE;
}

/* Same, but for gimple operands (trees).  */
static tree
debuglocus_bitmap_populate_tree (tree *tp,
				 int *walk_subtrees ATTRIBUTE_UNUSED,
				 void *data)
{
  struct walk_stmt_info *wi = (struct walk_stmt_info *) data;
  struct dlb_info *dlb = (struct dlb_info *)wi->info;

  if (CAN_HAVE_LOCATION_P (*tp))
    debuglocus_bitmap_populate_helper (dlb->bits, EXPR_LOCATION (*tp),
				       dlb->dumpfile, dlb->before_pass_p);
  return NULL_TREE;
}

/* Initialize and populate a bitmap of debuglocus entries.
   B is the bitmap to populate.
   DUMPFILE is the file to dump errors out to.
   BEFORE_PASS_P is true if this is the bitmap for before the pass ran.  */
void
debuglocus_bitmap_populate (bitmap b, FILE *dumpfile, bool before_pass_p)
{
  debuglocus_table_t *tab = current_debuglocus_table ();
  tree func;
  struct walk_stmt_info wi;
  struct dlb_info dlb;

  bitmap_initialize (b, NULL);

  if (!tab || !cfun)
    return;

  func = cfun->decl;

  dlb.bits = b;
  dlb.dumpfile = dumpfile;
  dlb.before_pass_p = before_pass_p;
  memset (&wi, 0, sizeof (wi));
  wi.info = (void *)&dlb;

  /* If we haven't lowered to gimple, ignore this pass.  */
  if (DECL_SAVED_TREE (func) != NULL)
    return;

  if (bitmap_empty_p (&global_dl_bitmap))
    bitmap_initialize (&global_dl_bitmap, NULL);

  /* If the CFG has been built, traverse the CFG.  */
  if (cfun->cfg && basic_block_info)
    {
      basic_block bb;
      gimple_stmt_iterator gsi;

      FOR_EACH_BB (bb)
	for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
	  {
	    walk_gimple_stmt (&gsi,
			      debuglocus_bitmap_populate_gimple,
			      debuglocus_bitmap_populate_tree,
			      &wi);
	  }
    }
  else
    {
      /* Otherwise, plain gimple.  Traverse the sequence of
	 statements.  */
      walk_gimple_seq (gimple_body (func),
		       debuglocus_bitmap_populate_gimple,
		       debuglocus_bitmap_populate_tree,
		       &wi);
    }

  /* If the pass has already run, update the global debugloc
     bitmap.  */
  if (before_pass_p == false)
    bitmap_ior_into (&global_dl_bitmap, b);
}

/* Dump the contents of debuglocus entry DL.
   F is the dumpfile.  DL is the debuglocus entry.
   INDEX is the debuglocus index for the entry.  */
static void
dump_debuglocus_entry (FILE *f, debuglocus_p dl, unsigned int index)
{
  expanded_location xloc = expand_location (dl->locus);

  fprintf (f, "index #%u\n", index);
  fputs   ("decl  = [", f);
  print_generic_decl (f, dl->decl, /*flags=*/0);
  fputs ("]\n", f);
  fprintf (f, "order = %d\n", dl->order);
  fprintf (f, "locus = %u {file='%s', line=%d, col=%d}\n",
	   (unsigned int) dl->locus, xloc.file, xloc.line, xloc.column);
  fprintf (f, "prev  = %d\n", dl->prev);
  fprintf (f, "next  = %d\n", dl->next);
}

/* Dump the entire contents of the debuglocus table.
   F is the dumpfile.  TAB is the table.  */
static void
dump_debuglocus_table (FILE *f, debuglocus_table_t *tab)
{
  unsigned int i;
  debuglocus_p d;

  if (!tab || tab->size <= 1)
    return;

  fputs ("\ndebuglocus table contents after pass executed\n", f);
  fputs ("-----------------------------------------------\n", f);
  fprintf (f, "number of elements=%d\n", tab->size);
  for (i = 1; i < tab->size; ++i)
    {
      d = get_debuglocus_entry (tab, i);
      dump_debuglocus_entry (f, d, i);
    }
  fputs ("-------------------------\n", f);
}

/* Verify the debuglocus entries created by the current pass.

   BEFORE is the debuglocus bitmap before the pass.  AFTER is the
   debuglocus bitmap after the pass.  F is the dumpfile to dump the
   verification information.  FLAGS is are the TDF dump flags from the
   pass.  */
void
debuglocus_bitmap_verify (FILE *f, bitmap before, bitmap after,
			  unsigned int flags)
{
  const char *comma;
  unsigned i;
  bitmap_iterator bi;
  bool first = true;
  debuglocus_table_t *tab = current_debuglocus_table ();

  if (tab && flags & TDF_DETAILS)
    dump_debuglocus_table (f, tab);

  /* Dump new debuglocus entries generated in this pass.  */
  comma = "";
  EXECUTE_IF_SET_IN_BITMAP (after, 0, i, bi)
    {
      if (!bitmap_bit_p (before, i))
	{
	  if (first)
	    {
	      fputs ("new debuglocus entries in this pass\n", f);
	      first = false;
	    }
	  fprintf (f, "%s%u", comma, i);
	  comma = ", ";
	}
    }
  if (!first)
    fputc ('\n', f);

  /* Dump debuglocus entries that went missing in this pass.  */
  first  = true;
  comma = "";
  EXECUTE_IF_SET_IN_BITMAP (before, 0, i, bi)
    {
      if (!bitmap_bit_p (after, i))
	{
	  if (first)
	    {
	      fputs ("deleted debuglocus entries in this pass:\n", f);
	      first = false;
	    }
	  fprintf (f, "%s%u", comma, i);
	  comma = ", ";
	}
    }
  if (!first)
    fputc ('\n', f);


  if (tab)
    {
      /* Dump debuglocus entries that are no longer referenced after this
	 pass.  */
      for (i = 1; i < tab->size; ++i)
	if (!bitmap_bit_p (&global_dl_bitmap, i))
	  fprintf (f, "debuglocus: orphaned debuglocus entry: %u\n", i);
    }
}

#include "gt-debuglocus.h"
