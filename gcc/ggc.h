/* Garbage collection for the GNU compiler: external interface
   Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005
   Free Software Foundation, Inc.

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#ifndef GCC_GGC_H
#define GCC_GGC_H
#include "statistics.h"

/* Symbols are marked with `ggc' for `gcc gc' so as not to interfere with
   an external gc library that might be linked in.  */

/* Constants for general use.  */
extern const char empty_string[];	/* empty string */
extern const char digit_vector[];	/* "0" .. "9" */

/* Internal functions and data structures used by the GTY
   machinery.  */

/* The first parameter is a pointer to a pointer, the second a cookie.  */
typedef void (*gt_pointer_operator) (void *, void *);

#include "gtype-desc.h"

/* Return 1 if P has been marked, zero otherwise.
   P must have been allocated by the GC allocator; it mustn't point to
   static objects, stack variables, or memory allocated with malloc.  */
extern int ggc_marked_p	(const void *);

/* Initialize the string pool.  */
extern void init_stringpool (void);

/* A GC implementation must provide these functions.  They are internal
   to the GC system.  */

/* Forward declare the zone structure.  Only ggc_zone implements this.  */
struct alloc_zone;

/* Initialize the garbage collector.  */
extern void init_ggc (void);

/* Allocation.  */

/* The internal primitive.  */
extern void *ggc_alloc_stat (size_t MEM_STAT_DECL);
#define ggc_alloc(s) ggc_alloc_stat (s MEM_STAT_INFO)
/* Allocate an object of the specified type and size.  */
extern void *ggc_alloc_typed_stat (enum gt_types_enum, size_t MEM_STAT_DECL);
#define ggc_alloc_typed(s,z) ggc_alloc_typed_stat (s,z MEM_STAT_INFO)
/* Like ggc_alloc, but allocates cleared memory.  */
extern void *ggc_alloc_cleared_stat (size_t MEM_STAT_DECL);
#define ggc_alloc_cleared(s) ggc_alloc_cleared_stat (s MEM_STAT_INFO)
/* Allocate an object of the specifid type and size and clear it.  */
extern void *ggc_alloc_cleared_typed_stat (enum gt_types_enum, size_t MEM_STAT_DECL);
#define ggc_alloc_cleared_typed(s,z) ggc_alloc_cleared_typed_stat(s, z MEM_STAT_INFO)
/* Resize a block.  */
extern void *ggc_realloc_stat (void *, size_t MEM_STAT_DECL);
#define ggc_realloc(s,z) ggc_realloc_stat (s,z MEM_STAT_INFO)
/* Like ggc_alloc_cleared, but performs a multiplication.  */
extern void *ggc_calloc (size_t, size_t);
/* Free a block.  To be used when known for certain it's not reachable.  */
extern void ggc_free (void *);

extern void dump_ggc_loc_statistics (void);

/* Allocators for atomic objects.  Use these when you know that allocated
   object does not contain pointers to GC-allocated memody.  */
extern void *ggc_alloc_atomic_stat (size_t MEM_STAT_DECL);
#define ggc_alloc_atomic(s) ggc_alloc_atomic_stat (s MEM_STAT_INFO)

/* Type-safe, C++-friendly versions of ggc_alloc() and gcc_calloc().  */
#define GGC_NEW(T)		((T *) ggc_alloc (sizeof (T)))
#define GGC_NEW_ATOMIC(T)       ((T *) ggc_alloc_atomic (sizeof (T)))
#define GGC_CNEW(T)             ((T *) ggc_alloc_cleared (sizeof (T)))
#define GGC_NEWVEC(T, N)	((T *) ggc_alloc ((N) * sizeof(T)))
#define GGC_CNEWVEC(T, N)	((T *) ggc_alloc_cleared ((N) * sizeof(T)))
#define GGC_NEWVAR(T, S)	((T *) ggc_alloc ((S)))
#define GGC_CNEWVAR(T, S)	((T *) ggc_alloc_cleared ((S)))
#define GGC_RESIZEVEC(T, P, N)  ((T *) ggc_realloc ((P), (N) * sizeof (T)))

#define ggc_alloc_rtvec(NELT)						 \
  ((rtvec) ggc_alloc_zone (sizeof (struct rtvec_def) + ((NELT) - 1)	 \
			   * sizeof (rtx), &rtl_zone))

#define ggc_alloc_tree(LENGTH) (ggc_alloc_tree_node (LENGTH))

#define htab_create_ggc(SIZE, HASH, EQ, DEL) \
  htab_create_alloc (SIZE, HASH, EQ, DEL, ggc_calloc, NULL)

#define splay_tree_new_ggc(COMPARE, ALLOC_TREE, ALLOC_NODE) \
  splay_tree_new_with_separate_allocators (COMPARE, NULL, NULL, \
                                           &ALLOC_TREE, &ALLOC_NODE,    \
                                           &ggc_splay_dont_free, NULL)
extern void *ggc_splay_alloc_tree (enum gt_types_enum, int, void *);
extern void *ggc_splay_alloc_node (enum gt_types_enum, int, void *);
extern void ggc_splay_dont_free (void *, void *);

/* Allocate a gc-able string, and fill it with LENGTH bytes from CONTENTS.
   If LENGTH is -1, then CONTENTS is assumed to be a
   null-terminated string and the memory sized accordingly.  */
extern const char *ggc_alloc_string (const char *contents, int length);

/* Make a copy of S, in GC-able memory.  */
#define ggc_strdup(S) ggc_alloc_string((S), -1)

/* Invoke the collector.  Garbage collection occurs only when this
   function is called, not during allocations.  */
extern void ggc_collect	(void);

/* Write out all GCed objects to F.  */
extern void gt_pch_save (FILE *f);

/* Read objects previously saved with gt_pch_save from F.  */
extern void gt_pch_restore (FILE *f);


/* Heuristics.  */
extern void init_ggc_heuristics (void);

/* Zone collection.  */
#if defined (GGC_ZONE) && !defined (GENERATOR_FILE)

/* For regular rtl allocations.  */
extern struct alloc_zone rtl_zone;
/* For regular tree allocations.  */
extern struct alloc_zone tree_zone;
/* For IDENTIFIER_NODE allocations.  */
extern struct alloc_zone tree_id_zone;

/* Allocate an object into the specified allocation zone.  */
extern void *ggc_alloc_zone_stat (size_t, struct alloc_zone * MEM_STAT_DECL);
# define ggc_alloc_zone(s,z) ggc_alloc_zone_stat (s,z MEM_STAT_INFO)
# define ggc_alloc_zone_pass_stat(s,z) ggc_alloc_zone_stat (s,z PASS_MEM_STAT)
#else

# define ggc_alloc_zone(s, z) ggc_alloc (s)
# define ggc_alloc_zone_pass_stat(s, z) ggc_alloc_stat (s PASS_MEM_STAT)

#endif

/* Print allocation statistics.  */
extern void ggc_print_statistics (void);
extern void stringpool_statistics (void);

#endif
