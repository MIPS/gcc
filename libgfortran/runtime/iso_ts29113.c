/* iso_ts29113.c. of GCC's GNU Fortran compiler.
   Copyright (C) 2013 Free Software Foundation, Inc.

This file is part of the GNU Fortran runtime library (libgfortran)
Libgfortran is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libgfortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libquadmath; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
Boston, MA 02110-1301, USA.  */


/* Functions as defined by ISO/IEC Technical Specification TS 29113:2012
   on Further Interoperability of Fortran with C.  */

#include <stdlib.h>	/* For malloc and free.  */
#include "ISO_Fortran_binding.h"

#include <stdio.h>

#ifndef __GNUC__
# define __attribute__(x)
# define likely(x)       (x)
# define unlikely(x)     (x)
#else
# define likely(x)       __builtin_expect(!!(x), 1)
# define unlikely(x)     __builtin_expect(!!(x), 0)
#endif


void *
CFI_address (const CFI_cdesc_t *dv, const CFI_index_t subscripts[])
{
  int i;
  CFI_index_t offset;

  if (!dv->base_addr || dv->rank == 0)
    return dv->base_addr;

  offset = 0;

  /* Notes: For nonalloctables, the lower_bound == 0; the caller ensures that.
     No bounds check is done.  */
  for (i = 0; i < dv->rank; i++)
    offset += (subscripts[i] - dv->dim[i].lower_bound) * dv->dim[i].sm;

  return dv->base_addr + offset;
}


/* ALLOCATE. The caller's descriptor already contains dv->base_addr == NULL,
   dv->rank, dv->attribute and dv->type; the allocation size is taken from
   the dv->elem_len, except for characters; for those it uses elem_len.  */

int
CFI_allocate (CFI_cdesc_t *dv, const CFI_index_t lower_bounds[],
	      const CFI_index_t upper_bounds[], size_t elem_len)
{
  int i;
  size_t size;
  intptr_t extent;

  if (unlikely (dv->attribute == CFI_attribute_other))
    return CFI_INVALID_ATTRIBUTE;

  if (unlikely (dv->base_addr))
    return CFI_ERROR_BASE_ADDR_NOT_NULL;

  if (dv->type == CFI_type_char || dv->type == CFI_type_ucs4_char)
    dv->elem_len = elem_len;

  size = dv->elem_len;

  for (i = 0; i < dv->rank; i++)
    {
      dv->dim[i].lower_bound = lower_bounds[i];
      extent = upper_bounds[i] - lower_bounds[i] + 1;
      dv->dim[i].extent = unlikely (extent < 0)
			  ? 0 : extent;
      dv->dim[i].sm = size;
      size *= dv->dim[i].extent;
    }

  if (unlikely (size == 0))
    size = 1;

  dv->base_addr = malloc (size);

  if (unlikely (!dv->base_addr))
    return CFI_ERROR_MEM_ALLOCATION;

  return CFI_SUCCESS;
}


int
CFI_deallocate (CFI_cdesc_t *dv)
{
  if (unlikely (dv->attribute == CFI_attribute_other))
    return CFI_INVALID_ATTRIBUTE;

  if (unlikely (!dv->base_addr))
    return CFI_ERROR_BASE_ADDR_NULL;

  /* FIXME: This function shall return an error if a pointer is
     not allocated via CFI_allocate/ALLOCATE, i.e. if it is
     associated with a named target. RFC: What happens if it is
     only the tailing part of a pointer alloc?  */

  free (dv->base_addr);
  dv->base_addr = NULL;
  return CFI_SUCCESS;
}


int
CFI_establish (CFI_cdesc_t *dv, void *base_addr, CFI_attribute_t attribute,
	       CFI_type_t type, size_t elem_len, CFI_rank_t rank,
	       const CFI_index_t extents[])
{
  int i;
  size_t sm;

  if (unlikely (attribute != CFI_attribute_pointer
		&& attribute != CFI_attribute_allocatable
		&& attribute != CFI_attribute_other))
    return CFI_INVALID_ATTRIBUTE;

  if (unlikely (attribute == CFI_attribute_allocatable && base_addr))
    return CFI_ERROR_BASE_ADDR_NOT_NULL;

  if (unlikely (rank > CFI_MAX_RANK))
    return CFI_INVALID_RANK;

  if (rank > 0 && base_addr)
    for (i = 0; i < rank; i++)
      if (unlikely (extents[i] < 0))
	return CFI_INVALID_EXTENT;

  dv->base_addr = base_addr;
  dv->rank = rank;
  dv->version = CFI_VERSION;
  dv->type = type;
  dv->attribute = attribute;

  if (type == CFI_type_char || type == CFI_type_ucs4_char
      || type == CFI_type_struct || type == CFI_type_other)
    dv->elem_len = elem_len;
  else if (type == CFI_type_Real10)
    dv->elem_len = 16;
  else if (type == CFI_type_Complex10)
    dv->elem_len = 32;
  else if ((type & CFI_type_mask) == CFI_type_Complex)
    dv->elem_len = 2*(type >> CFI_type_kind_shift);
  else
    dv->elem_len = type >> CFI_type_kind_shift;

  if (!base_addr)
    return CFI_SUCCESS;

  sm = dv->elem_len;
  for (i = 0; i < rank; i++)
    {
      dv->dim[i].lower_bound = 0;
      dv->dim[i].extent = extents[i];
      dv->dim[i].sm = sm;
      sm *= extents[i];
    }

  return CFI_SUCCESS;
}


int
CFI_is_contiguous (const CFI_cdesc_t *dv)
{
  int i;
  ptrdiff_t size;

  /* Claim that scalars and (invalid!) null pointers are contiguous.  */
  if (dv->rank == 0 || !dv->base_addr)
    return 1;

  /* Assumed-size array are contiguous.  */
  if (dv->dim[dv->rank-1].extent < 0)
    return 1;

  size = dv->elem_len;
  for (i = 1; i < dv->rank; i++)
    {
      if (size < dv->dim[i].sm || dv->dim[i].sm < 0)
	return 0;
      size *= dv->dim[i].sm;
    }

  return 1;
}


int
CFI_section (CFI_cdesc_t *result, const CFI_cdesc_t *source,
	     const CFI_index_t lower_bounds[],
	     const CFI_index_t upper_bounds[],
	     const CFI_index_t strides[])
{
  int i, j;
  CFI_index_t offset;

  if (unlikely (result->attribute == CFI_attribute_allocatable))
    return CFI_INVALID_ATTRIBUTE;

  if (unlikely (result->elem_len != source->elem_len))
    return CFI_INVALID_ELEM_LEN;

  if (unlikely (result->type != source->type))
    return CFI_INVALID_TYPE;

  if (strides == NULL && unlikely (result->rank != source->rank))
    return CFI_INVALID_RANK;

  if (strides != NULL)
    {
      int zero_strides = 0;

      for (i = 0; i < source->rank; i++)
	if (strides[i] == 0)
	  {
	    CFI_index_t lb = lower_bounds
			     ? lower_bounds[i] : source->dim[i].lower_bound;
	    CFI_index_t ub = upper_bounds
			     ? upper_bounds[i]
			     : source->dim[i].extent + lb - 1;
	    if (unlikely ((lb != ub)))
	      return CFI_INVALID_EXTENT;
	    zero_strides++;
	  }

      if (unlikely (result->rank + zero_strides != source->rank))
	return CFI_INVALID_RANK;
    }

  offset = 0;

  for (i = 0, j = 0; i < source->rank; i++)
    {
      if (lower_bounds)
	offset += (lower_bounds[i]-source->dim[i].lower_bound)*source->dim[i].sm;

      if (strides && strides[i] == 0)
	continue;

      result->dim[j].lower_bound = lower_bounds
				   ? lower_bounds[i] : source->dim[i].lower_bound;
      if (upper_bounds)
	{
	  CFI_index_t extent;
	  extent = upper_bounds[i] - result->dim[j].lower_bound + 1;
          result->dim[j].extent = extent < 0 ? 0 : extent;
	}
      else
	result->dim[j].extent = source->dim[i].extent;

      if (!strides)
	result->dim[j].sm = source->dim[i].sm;
      else
	{
	  result->dim[j].sm = source->dim[i].sm * strides[i];
	  result->dim[j].extent /= strides[i];
	}

      j++;
    }

  result->base_addr = source->base_addr + offset;

  return CFI_SUCCESS;
}


int
CFI_select_part (CFI_cdesc_t *result, const CFI_cdesc_t *source,
		 size_t displacement, size_t elem_len)
{
  int i;

  if (unlikely (result->attribute == CFI_attribute_allocatable))
    return CFI_INVALID_ATTRIBUTE;

  if (unlikely (!source->base_addr))
    return CFI_ERROR_BASE_ADDR_NULL;

  if (unlikely (result->rank != source->rank))
    return CFI_INVALID_RANK;

  if (unlikely (displacement >= source->elem_len))
    return CFI_INVALID_ELEM_LEN;

  if (result->type == CFI_type_char || result->type == CFI_type_ucs4_char)
    {
      if (unlikely (elem_len > source->elem_len))
        return CFI_INVALID_ELEM_LEN;
      result->elem_len = elem_len;
    }

  result->base_addr = source->base_addr + displacement;

  for (i = 1; i < result->rank; i++)
   {
      result->dim[i].lower_bound = source->dim[i].lower_bound;
      result->dim[i].extent = source->dim[i].extent;
      result->dim[i].sm = source->dim[i].sm;
    }

  return CFI_SUCCESS;
}


int
CFI_setpointer (CFI_cdesc_t *result, CFI_cdesc_t *source,
		const CFI_index_t lower_bounds[])
{
  int i;

  if (unlikely (result->attribute != CFI_attribute_pointer))
    return CFI_INVALID_ATTRIBUTE;

  if (!source->base_addr)
    {
      result->base_addr = NULL;
      return CFI_SUCCESS;
    }

  if (unlikely (result->elem_len != source->elem_len))
    return CFI_INVALID_ELEM_LEN;

  if (unlikely (result->type != source->type))
    return CFI_INVALID_TYPE;

  if (unlikely (result->rank != source->rank))
    return CFI_INVALID_RANK;

  for (i = 0; i < result->rank; i++)
    {
      result->dim[i].lower_bound = lower_bounds[i];
      result->dim[i].extent = source->dim[i].extent;
      result->dim[i].sm = source->dim[i].sm;
    }

  return CFI_SUCCESS;
}
