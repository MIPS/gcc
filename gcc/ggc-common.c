/* Simple garbage collection for the GNU compiler.
   Copyright (C) 1999, 2000, 2001, 2002 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* Generic garbage collection (GC) functions and data, not specific to
   any particular GC implementation.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "hash.h"
#include "hashtab.h"
#include "varray.h"
#include "ggc.h"
#include "langhooks.h"

/* Statistics about the allocation.  */
static ggc_statistics *ggc_stats;

static void ggc_mark_rtx_children_1 PARAMS ((rtx));
static void ggc_mark_rtx_varray_ptr PARAMS ((void *));
static void ggc_mark_tree_varray_ptr PARAMS ((void *));
static void ggc_mark_tree_hash_table_ptr PARAMS ((void *));
static int ggc_htab_delete PARAMS ((void **, void *));
static bool ggc_mark_tree_hash_table_entry PARAMS ((struct hash_entry *,
						    hash_table_key));

/* Maintain global roots that are preserved during GC.  */

/* Global roots that are preserved during calls to gc.  */

struct ggc_root
{
  struct ggc_root *next;
  void *base;
  int nelt;
  int size;
  void (*cb) PARAMS ((void *));
};

static struct ggc_root *roots;

/* Add BASE as a new garbage collection root.  It is an array of
   length NELT with each element SIZE bytes long.  CB is a 
   function that will be called with a pointer to each element
   of the array; it is the intention that CB call the appropriate
   routine to mark gc-able memory for that element.  */

void
ggc_add_root (base, nelt, size, cb)
     void *base;
     int nelt, size;
     void (*cb) PARAMS ((void *));
{
  struct ggc_root *x = (struct ggc_root *) xmalloc (sizeof (*x));

  x->next = roots;
  x->base = base;
  x->nelt = nelt;
  x->size = size;
  x->cb = cb;

  roots = x;
}

/* Register a varray of rtxs as a GC root.  */

void
ggc_add_rtx_varray_root (base, nelt)
     varray_type *base;
     int nelt;
{
  ggc_add_root (base, nelt, sizeof (varray_type), 
		ggc_mark_rtx_varray_ptr);
}

/* Register a varray of trees as a GC root.  */

void
ggc_add_tree_varray_root (base, nelt)
     varray_type *base;
     int nelt;
{
  ggc_add_root (base, nelt, sizeof (varray_type), 
		ggc_mark_tree_varray_ptr);
}

/* Register a hash table of trees as a GC root.  */

void
ggc_add_tree_hash_table_root (base, nelt)
     struct hash_table **base;
     int nelt;
{
  ggc_add_root (base, nelt, sizeof (struct hash_table *), 
		ggc_mark_tree_hash_table_ptr);
}

/* Add a hash table to be scanned when all roots have been processed.  We
   delete any entry in the table that has not been marked.  */

struct d_htab_root
{
  struct d_htab_root *next;
  htab_t htab;
  ggc_htab_marked_p marked_p;
  ggc_htab_mark mark;
};

static struct d_htab_root *d_htab_roots;

/* Add X, an htab, to a list of htabs that contain objects which are allocated
   from GC memory.  Once all other roots are marked, we check each object in
   the htab to see if it has already been marked.  If not, it is deleted.

   MARKED_P, if specified, is a function that returns 1 if the entry is to
   be considered as "marked".  If not present, the data structure pointed to
   by the htab slot is tested.  This function should be supplied if some
   other object (such as something pointed to by that object) should be tested
   in which case the function tests whether that object (or objects) are
   marked (using ggc_marked_p) and returns nonzero if it is.

   MARK, if specified, is a function that is passed the contents of a slot
   that has been determined to have been "marked" (via the above function)
   and marks any other objects pointed to by that object.  For example,
   we might have a hash table of memory attribute blocks, which are pointed
   to by a MEM RTL but have a pointer to a DECL.  MARKED_P in that case will
   not be specified because we want to know if the attribute block is pointed
   to by the MEM, but MARK must be specified because if the block has been
   marked, we need to mark the DECL.  */

void
ggc_add_deletable_htab (x, marked_p, mark)
     PTR x;
     ggc_htab_marked_p marked_p;
     ggc_htab_mark mark;
{
  struct d_htab_root *r
    = (struct d_htab_root *) xmalloc (sizeof (struct d_htab_root));

  r->next = d_htab_roots;
  r->htab = (htab_t) x;
  r->marked_p = marked_p ? marked_p : ggc_marked_p;
  r->mark = mark;
  d_htab_roots = r;
}

/* Process a slot of an htab by deleting it if it has not been marked.  */

static int
ggc_htab_delete (slot, info)
     void **slot;
     void *info;
{
  struct d_htab_root *r = (struct d_htab_root *) info;

  if (! (*r->marked_p) (*slot))
    htab_clear_slot (r->htab, slot);
  else if (r->mark)
    (*r->mark) (*slot);

  return 1;
}

/* Iterate through all registered roots and mark each element.  */

void
ggc_mark_roots ()
{
  struct ggc_root *x;
  struct d_htab_root *y;
  const struct ggc_root_tab *const *rt;
  const struct ggc_root_tab *rti;
  size_t i;
  
  for (rt = gt_ggc_deletable_rtab; *rt; rt++)
    for (rti = *rt; rti->base != NULL; rti++)
      memset (rti->base, 0, rti->stride);

  for (rt = gt_ggc_rtab; *rt; rt++)
    for (rti = *rt; rti->base != NULL; rti++)
      for (i = 0; i < rti->nelt; i++)
	(*rti->cb)(*(void **)((char *)rti->base + rti->stride * i));

  for (x = roots; x != NULL; x = x->next)
    {
      char *elt = x->base;
      int s = x->size, n = x->nelt;
      void (*cb) PARAMS ((void *)) = x->cb;
      int i;

      for (i = 0; i < n; ++i, elt += s)
	(*cb)(elt);
    }

  /* Now scan all hash tables that have objects which are to be deleted if
     they are not already marked.  */
  for (y = d_htab_roots; y != NULL; y = y->next)
    htab_traverse (y->htab, ggc_htab_delete, (PTR) y);
}

/* R had not been previously marked, but has now been marked via
   ggc_set_mark.  Now recurse and process the children.  */

void
ggc_mark_rtx_children (r)
     rtx r;
{
  rtx i, last;

  /* Special case the instruction chain.  This is a data structure whose
     chain length is potentially unbounded, and which contain references
     within the chain (e.g. label_ref and insn_list).  If do nothing here,
     we risk blowing the stack recursing through a long chain of insns.

     Combat this by marking all of the instructions in the chain before
     marking the contents of those instructions.  */

  switch (GET_CODE (r))
    {
    case INSN:
    case JUMP_INSN:
    case CALL_INSN:
    case NOTE:
    case CODE_LABEL:
    case BARRIER:
      for (i = NEXT_INSN (r); ; i = NEXT_INSN (i))
	if (! ggc_test_and_set_mark (i))
	  break;
      last = i;

      for (i = NEXT_INSN (r); i != last; i = NEXT_INSN (i))
	ggc_mark_rtx_children_1 (i);

    default:
      break;
    }

  ggc_mark_rtx_children_1 (r);
}

static void
ggc_mark_rtx_children_1 (r)
     rtx r;
{
  const char *fmt;
  int i;
  rtx next_rtx;

  do 
    {
      enum rtx_code code = GET_CODE (r);
      /* This gets set to a child rtx to eliminate tail recursion.  */
      next_rtx = NULL;

      /* Collect statistics, if appropriate.  */
      if (ggc_stats)
	{
	  ++ggc_stats->num_rtxs[(int) code];
	  ggc_stats->size_rtxs[(int) code] += ggc_get_size (r);
	}

      /* ??? If (some of) these are really pass-dependent info, do we
	 have any right poking our noses in?  */
      switch (code)
	{
	case MEM:
	  gt_ggc_m_mem_attrs (MEM_ATTRS (r));
	  break;
	case JUMP_INSN:
	  ggc_mark_rtx (JUMP_LABEL (r));
	  break;
	case CODE_LABEL:
	  ggc_mark_rtx (LABEL_REFS (r));
	  break;
	case LABEL_REF:
	  ggc_mark_rtx (LABEL_NEXTREF (r));
	  ggc_mark_rtx (CONTAINING_INSN (r));
	  break;
	case ADDRESSOF:
	  ggc_mark_tree (ADDRESSOF_DECL (r));
	  break;
	case CONST_DOUBLE:
	  ggc_mark_rtx (CONST_DOUBLE_CHAIN (r));
	  break;
	case NOTE:
	  switch (NOTE_LINE_NUMBER (r))
	    {
	    case NOTE_INSN_RANGE_BEG:
	    case NOTE_INSN_RANGE_END:
	    case NOTE_INSN_LIVE:
	    case NOTE_INSN_EXPECTED_VALUE:
	      ggc_mark_rtx (NOTE_RANGE_INFO (r));
	      break;

	    case NOTE_INSN_BLOCK_BEG:
	    case NOTE_INSN_BLOCK_END:
	      ggc_mark_tree (NOTE_BLOCK (r));
	      break;

	    default:
	      break;
	    }
	  break;

	default:
	  break;
	}

      for (fmt = GET_RTX_FORMAT (GET_CODE (r)), i = 0; *fmt ; ++fmt, ++i)
	{
	  rtx exp;
	  switch (*fmt)
	    {
	    case 'e': case 'u':
	      exp = XEXP (r, i);
	      if (ggc_test_and_set_mark (exp))
		{ 
		  if (next_rtx == NULL) 
		    next_rtx = exp; 
		  else 
		    ggc_mark_rtx_children (exp);
		} 
	      break;
	    case 'V': case 'E':
	      gt_ggc_m_rtvec_def (XVEC (r, i));
	      break;
	    }
	}
    }
  while ((r = next_rtx) != NULL);
}

/* Mark all the elements of the varray V, which contains rtxs.  */

void
ggc_mark_rtx_varray (v)
     varray_type v;
{
  int i;

  if (v)
    for (i = v->num_elements - 1; i >= 0; --i) 
      ggc_mark_rtx (VARRAY_RTX (v, i));
}

/* Mark all the elements of the varray V, which contains trees.  */

void
ggc_mark_tree_varray (v)
     varray_type v;
{
  int i;

  if (v)
    for (i = v->num_elements - 1; i >= 0; --i) 
      ggc_mark_tree (VARRAY_TREE (v, i));
}

/* Mark the hash table-entry HE.  Its key field is really a tree.  */

static bool
ggc_mark_tree_hash_table_entry (he, k)
     struct hash_entry *he;
     hash_table_key k ATTRIBUTE_UNUSED;
{
  ggc_mark_tree ((tree) he->key);
  return true;
}

/* Mark all the elements of the hash-table H, which contains trees.  */

void
ggc_mark_tree_hash_table (ht)
     struct hash_table *ht;
{
  hash_traverse (ht, ggc_mark_tree_hash_table_entry, /*info=*/0);
}

/* Type-correct function to pass to ggc_add_root.  It just forwards
   ELT (which is really a varray_type *) to ggc_mark_rtx_varray.  */

static void
ggc_mark_rtx_varray_ptr (elt)
     void *elt;
{
  ggc_mark_rtx_varray (*(varray_type *) elt);
}

/* Type-correct function to pass to ggc_add_root.  It just forwards
   ELT (which is really a varray_type *) to ggc_mark_tree_varray.  */

static void
ggc_mark_tree_varray_ptr (elt)
     void *elt;
{
  ggc_mark_tree_varray (*(varray_type *) elt);
}

/* Type-correct function to pass to ggc_add_root.  It just forwards
   ELT (which is really a struct hash_table **) to
   ggc_mark_tree_hash_table.  */

static void
ggc_mark_tree_hash_table_ptr (elt)
     void *elt;
{
  ggc_mark_tree_hash_table (*(struct hash_table **) elt);
}

/* Various adaptor functions.  */
void
gt_ggc_m_rtx_def (x)
     void *x;
{
  ggc_mark_rtx((rtx)x);
}

/* Allocate a block of memory, then clear it.  */
void *
ggc_alloc_cleared (size)
     size_t size;
{
  void *buf = ggc_alloc (size);
  memset (buf, 0, size);
  return buf;
}

/* Resize a block of memory, possibly re-allocating it.  */
void *
ggc_realloc (x, size)
     void *x;
     size_t size;
{
  void *r;
  size_t old_size;

  if (x == NULL)
    return ggc_alloc (size);

  old_size = ggc_get_size (x);
  if (size <= old_size)
    return x;

  r = ggc_alloc (size);
  memcpy (r, x, old_size);
  return r;
}

/* Like ggc_alloc_cleared, but performs a multiplication.  */
void *
ggc_calloc (s1, s2)
     size_t s1, s2;
{
  return ggc_alloc_cleared (s1 * s2);
}

/* Print statistics that are independent of the collector in use.  */
#define SCALE(x) ((unsigned long) ((x) < 1024*10 \
		  ? (x) \
		  : ((x) < 1024*1024*10 \
		     ? (x) / 1024 \
		     : (x) / (1024*1024))))
#define LABEL(x) ((x) < 1024*10 ? ' ' : ((x) < 1024*1024*10 ? 'k' : 'M'))

void
ggc_print_common_statistics (stream, stats)
     FILE *stream;
     ggc_statistics *stats;
{
  int code;

  /* Set the pointer so that during collection we will actually gather
     the statistics.  */
  ggc_stats = stats;

  /* Then do one collection to fill in the statistics.  */
  ggc_collect ();

  /* Total the statistics.  */
  for (code = 0; code < MAX_TREE_CODES; ++code)
    {
      stats->total_num_trees += stats->num_trees[code];
      stats->total_size_trees += stats->size_trees[code];
    }
  for (code = 0; code < NUM_RTX_CODE; ++code)
    {
      stats->total_num_rtxs += stats->num_rtxs[code];
      stats->total_size_rtxs += stats->size_rtxs[code];
    }

  /* Print the statistics for trees.  */
  fprintf (stream, "\n%-17s%10s %16s %10s\n", "Tree", 
	   "Number", "Bytes", "% Total");
  for (code = 0; code < MAX_TREE_CODES; ++code)
    if (ggc_stats->num_trees[code]) 
      {
	fprintf (stream, "%-17s%10u%16ld%c %10.3f\n",
		 tree_code_name[code],
		 ggc_stats->num_trees[code],
		 SCALE (ggc_stats->size_trees[code]),
		 LABEL (ggc_stats->size_trees[code]),
		 (100 * ((double) ggc_stats->size_trees[code]) 
		  / ggc_stats->total_size_trees));
      }
  fprintf (stream,
	   "%-17s%10u%16ld%c\n", "Total",
	   ggc_stats->total_num_trees,
	   SCALE (ggc_stats->total_size_trees),
	   LABEL (ggc_stats->total_size_trees));

  /* Print the statistics for RTL.  */
  fprintf (stream, "\n%-17s%10s %16s %10s\n", "RTX", 
	   "Number", "Bytes", "% Total");
  for (code = 0; code < NUM_RTX_CODE; ++code)
    if (ggc_stats->num_rtxs[code]) 
      {
	fprintf (stream, "%-17s%10u%16ld%c %10.3f\n",
		 rtx_name[code],
		 ggc_stats->num_rtxs[code],
		 SCALE (ggc_stats->size_rtxs[code]),
		 LABEL (ggc_stats->size_rtxs[code]),
		 (100 * ((double) ggc_stats->size_rtxs[code]) 
		  / ggc_stats->total_size_rtxs));
      }
  fprintf (stream,
	   "%-17s%10u%16ld%c\n", "Total",
	   ggc_stats->total_num_rtxs,
	   SCALE (ggc_stats->total_size_rtxs),
	   LABEL (ggc_stats->total_size_rtxs));

  /* Don't gather statistics any more.  */
  ggc_stats = NULL;
}
