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
#include "hashtab.h"
#include "ggc.h"

/* Statistics about the allocation.  */
static ggc_statistics *ggc_stats;

static int ggc_htab_delete PARAMS ((void **, void *));

/* Maintain global roots that are preserved during GC.  */

/* Process a slot of an htab by deleting it if it has not been marked.  */

static int
ggc_htab_delete (slot, info)
     void **slot;
     void *info;
{
  const struct ggc_cache_tab *r = (const struct ggc_cache_tab *) info;

  if (! (*r->marked_p) (*slot))
    htab_clear_slot (*r->base, slot);
  else
    (*r->cb) (*slot);

  return 1;
}

/* Iterate through all registered roots and mark each element.  */

void
ggc_mark_roots ()
{
  struct ggc_root *x;
  const struct ggc_root_tab *const *rt;
  const struct ggc_root_tab *rti;
  const struct ggc_cache_tab *const *ct;
  const struct ggc_cache_tab *cti;
  size_t i;

  for (rt = gt_ggc_deletable_rtab; *rt; rt++)
    for (rti = *rt; rti->base != NULL; rti++)
      memset (rti->base, 0, rti->stride);

  for (rt = gt_ggc_rtab; *rt; rt++)
    for (rti = *rt; rti->base != NULL; rti++)
      for (i = 0; i < rti->nelt; i++)
	(*rti->cb)(*(void **)((char *)rti->base + rti->stride * i));

  ggc_mark_stringpool ();

  /* Now scan all hash tables that have objects which are to be deleted if
     they are not already marked.  */
  for (ct = gt_ggc_cache_rtab; *ct; ct++)
    for (cti = *ct; cti->base != NULL; cti++)
      htab_traverse (*cti->base, ggc_htab_delete, (PTR) cti);
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

/* These are for splay_tree_new_ggc.  */
PTR 
ggc_splay_alloc (sz, nl)
     int sz;
     PTR nl;
{
  if (nl != NULL)
    abort ();
  ggc_alloc (sz);
}

void
ggc_splay_dont_free (x, nl)
     PTR x;
     PTR nl ATTRIBUTE_UNUSED;
{
  if (nl != NULL)
    abort ();
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

  /* At present, we don't really gather any interesting statistics.  */

  /* Don't gather statistics any more.  */
  ggc_stats = NULL;
}
