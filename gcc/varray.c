/* Virtual array support.
   Copyright (C) 1998, 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
   Contributed by Cygnus Solutions.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA.  */

#include "config.h"
#include "errors.h"
#include "system.h"
#include "varray.h"
#include "ggc.h"
#include "hashtab.h"

#define VARRAY_HDR_SIZE (sizeof (struct varray_head_tag) - sizeof (varray_data))

static const size_t element_size[NUM_VARRAY_DATA] = {
  sizeof (char),
  sizeof (unsigned char),
  sizeof (short),
  sizeof (unsigned short),
  sizeof (int),
  sizeof (unsigned int),
  sizeof (long),
  sizeof (unsigned long),
  sizeof (HOST_WIDE_INT),
  sizeof (unsigned HOST_WIDE_INT),
  sizeof (PTR),
  sizeof (char *),
  sizeof (struct rtx_def *),
  sizeof (struct rtvec_def *),
  sizeof (union tree_node *),
  sizeof (struct bitmap_head_def *),
  sizeof (struct reg_info_def *),
  sizeof (struct const_equiv_data),
  sizeof (struct basic_block_def *),
  sizeof (struct elt_list *)
};

static const int uses_ggc[NUM_VARRAY_DATA] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* unsigned HOST_WIDE_INT */
  1, /* PTR */
  1, 1, 1, 1, 1, /* bitmap_head_def */
  0, 0, 0, 1
};

#ifdef GATHER_STATISTICS

/* Store infromation about each particular varray.  */
struct varray_descriptor
{
  const char *name;
  int allocated;
  int created;
  int resized;
  int copied;
};

/* Hashtable mapping varray names to descriptors.  */
static htab_t varray_hash;

/* Hashtable helpers.  */
static hashval_t
hash_descriptor (const void *p)
{
  const struct varray_descriptor *d = p;
  return htab_hash_pointer (d->name);
}
static int
eq_descriptor (const void *p1, const void *p2)
{
  const struct varray_descriptor *d = p1;
  return d->name == p2;
}

/* For given name, return descriptor, create new if needed.  */
static struct varray_descriptor *
varray_descriptor (const char *name)
{
  struct varray_descriptor **slot;

  if (!varray_hash)
    varray_hash = htab_create (10, hash_descriptor, eq_descriptor, NULL);

  slot = (struct varray_descriptor **)
    htab_find_slot_with_hash (varray_hash, name,
		    	      htab_hash_pointer (name),
			      1);
  if (*slot)
    return *slot;
  *slot = xcalloc (sizeof (**slot), 1);
  (*slot)->name = name;
  return *slot;
}
#endif

/* Allocate a virtual array with NUM_ELEMENT elements, each of which is
   ELEMENT_SIZE bytes long, named NAME.  Array elements are zeroed.  */
varray_type
varray_init (num_elements, element_kind, name)
     size_t num_elements;
     enum varray_data_enum element_kind;
     const char *name;
{
  size_t data_size = num_elements * element_size[element_kind];
  varray_type ptr;
#ifdef GATHER_STATISTICS
  struct varray_descriptor *desc = varray_descriptor (name);

  desc->created++;
  desc->allocated += data_size + VARRAY_HDR_SIZE;
#endif
  if (uses_ggc [element_kind])
    ptr = (varray_type) ggc_alloc_cleared (VARRAY_HDR_SIZE + data_size);
  else
    ptr = (varray_type) xcalloc (VARRAY_HDR_SIZE + data_size, 1);

  ptr->num_elements = num_elements;
  ptr->elements_used = 0;
  ptr->type = element_kind;
  ptr->name = name;
  return ptr;
}

/* Grow/shrink the virtual array VA to N elements.  Zero any new elements
   allocated.  */
varray_type
varray_grow (va, n)
     varray_type va;
     size_t n;
{
  size_t old_elements = va->num_elements;

  if (n != old_elements)
    {
      size_t elem_size = element_size[va->type];
      size_t old_data_size = old_elements * elem_size;
      size_t data_size = n * elem_size;
#ifdef GATHER_STATISTICS
      struct varray_descriptor *desc = varray_descriptor (va->name);
      varray_type oldva = va;

      if (data_size > old_data_size)
        desc->allocated += data_size - old_data_size;
      desc->resized ++;
#endif


      if (uses_ggc[va->type])
	va = (varray_type) ggc_realloc (va, VARRAY_HDR_SIZE + data_size);
      else
	va = (varray_type) xrealloc ((char *) va, VARRAY_HDR_SIZE + data_size);
      va->num_elements = n;
      if (n > old_elements)
	memset (&va->data.c[old_data_size], 0, data_size - old_data_size);
#ifdef GATHER_STATISTICS
      if (oldva != va)
        desc->copied++;
#endif
    }

  return va;
}

/* Reset a varray to its original state.  */
void
varray_clear (va)
     varray_type va;
{
  size_t data_size = element_size[va->type] * va->num_elements;

  memset (va->data.c, 0, data_size);
  va->elements_used = 0;
}

/* Check the bounds of a varray access.  */

#if defined ENABLE_CHECKING && (GCC_VERSION >= 2007)

extern void error PARAMS ((const char *, ...))	ATTRIBUTE_PRINTF_1;

void
varray_check_failed (va, n, file, line, function)
     varray_type va;
     size_t n;
     const char *file;
     int line;
     const char *function;
{
  internal_error ("virtual array %s[%lu]: element %lu out of bounds in %s, at %s:%d",
		  va->name, (unsigned long) va->num_elements, (unsigned long) n,
		  function, trim_filename (file), line);
}

#endif

/* Output per-varray statistics.  */
#ifdef GATHER_STATISTICS
struct output_info
{
  int count;
  int size;
};
static int
print_statistics (void **slot, void *b)
{
  struct varray_descriptor *d = (struct varray_descriptor *) *slot;
  struct output_info *i = (struct output_info *) b;

  if (d->allocated)
    {
      fprintf (stderr, "%-21s %6d %10d %7d %7d\n", d->name,
	       d->created, d->allocated, d->resized, d->copied);
      i->size += d->allocated;
      i->count += d->created;
    }
  return 1;
}
#endif
void dump_varray_statistics (void)
{
#ifdef GATHER_STATISTICS
  struct output_info info;

  fprintf (stderr, "\nVARRAY Kind            Count      Bytes  Resized copied\n");
  fprintf (stderr, "-------------------------------------------------------\n");
  info.count = 0;
  info.size = 0;
  htab_traverse (varray_hash, print_statistics, &info);
  fprintf (stderr, "-------------------------------------------------------\n");
  fprintf (stderr, "%-20s %7d %10d\n",
	   "Total", info.count, info.size);
  fprintf (stderr, "-------------------------------------------------------\n");
#endif
}
