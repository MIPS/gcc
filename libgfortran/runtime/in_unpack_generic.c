/* Generic helper function for repacking arrays.
   Copyright (C) 2003-2016 Free Software Foundation, Inc.
   Contributed by Paul Brook <paul@nowt.org>

This file is part of the GNU Fortran runtime library (libgfortran).

Libgfortran is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

Libgfortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#include "libgfortran.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

extern void internal_unpack (gfc_array_char *, const void *);
export_proto(internal_unpack);

void
internal_unpack (gfc_array_char * d, const void * s)
{
  index_type count[GFC_MAX_DIMENSIONS];
  index_type extent[GFC_MAX_DIMENSIONS];
  index_type sm[GFC_MAX_DIMENSIONS];
  index_type sm0;
  index_type dim;
  index_type dsize;
  char *dest;
  const char *src;
  int n;
  int size;
  CFI_type_t type;

  dest = d->base_addr;
  /* This check may be redundant, but do it anyway.  */
  if (s == dest || !s)
    return;

  type = GFC_DESCRIPTOR_TYPE (d);
  if ((type == CFI_type_struct || type == CFI_type_other)
      && GFC_DESCRIPTOR_ELEM_LEN (d) == 1)
    type = CFI_type_Integer1;

  switch (type)
    {
    case CFI_type_Integer1:
    case CFI_type_Logical1:
      internal_unpack_1 ((gfc_array_i1 *) d, (const GFC_INTEGER_1 *) s);
      return;

    case CFI_type_Integer2:
    case CFI_type_Logical2:
      internal_unpack_2 ((gfc_array_i2 *) d, (const GFC_INTEGER_2 *) s);
      return;

    case CFI_type_Integer4:
    case CFI_type_Logical4:
      internal_unpack_4 ((gfc_array_i4 *) d, (const GFC_INTEGER_4 *) s);
      return;

    case CFI_type_Integer8:
    case CFI_type_Logical8:
      internal_unpack_8 ((gfc_array_i8 *) d, (const GFC_INTEGER_8 *) s);
      return;

#if defined (HAVE_GFC_INTEGER_16)
    case CFI_type_Integer16:
    case CFI_type_Logical16:
      internal_unpack_16 ((gfc_array_i16 *) d, (const GFC_INTEGER_16 *) s);
      return;
#endif

    case CFI_type_Real4:
      internal_unpack_r4 ((gfc_array_r4 *) d, (const GFC_REAL_4 *) s);
      return;

    case CFI_type_Real8:
      internal_unpack_r8 ((gfc_array_r8 *) d, (const GFC_REAL_8 *) s);
      return;

# if defined(HAVE_GFC_REAL_10)
    case CFI_type_Real10:
      internal_unpack_r10 ((gfc_array_r10 *) d, (const GFC_REAL_10 *) s);
      return;
# endif

# if defined(HAVE_GFC_REAL_16)
    case CFI_type_Real16:
      internal_unpack_r16 ((gfc_array_r16 *) d, (const GFC_REAL_16 *) s);
      return;
# endif

    case CFI_type_Complex4:
      internal_unpack_c4 ((gfc_array_c4 *)d, (const GFC_COMPLEX_4 *)s);
      return;

    case CFI_type_Complex8:
      internal_unpack_c8 ((gfc_array_c8 *)d, (const GFC_COMPLEX_8 *)s);
      return;

# if defined(HAVE_GFC_COMPLEX_10)
    case CFI_type_Complex10:
      internal_unpack_c10 ((gfc_array_c10 *) d, (const GFC_COMPLEX_10 *) s);
      return;
# endif

# if defined(HAVE_GFC_COMPLEX_16)
    case CFI_type_Complex16:
      internal_unpack_c16 ((gfc_array_c16 *) d, (const GFC_COMPLEX_16 *) s);
      return;
# endif

    case CFI_type_struct:
    case CFI_type_other:
      switch (GFC_DESCRIPTOR_ELEM_LEN(d))
	{
	case 2:
	  if (GFC_UNALIGNED_2(d->base_addr) || GFC_UNALIGNED_2(s))
	    break;
	  else
	    {
	      internal_unpack_2 ((gfc_array_i2 *) d, (const GFC_INTEGER_2 *) s);
	      return;
	    }

	case 4:
	  if (GFC_UNALIGNED_4(d->base_addr) || GFC_UNALIGNED_4(s))
	    break;
	  else
	    {
	      internal_unpack_4 ((gfc_array_i4 *) d, (const GFC_INTEGER_4 *) s);
	      return;
	    }

	case 8:
	  if (GFC_UNALIGNED_8(d->base_addr) || GFC_UNALIGNED_8(s))
	    break;
	  else
	    {
	      internal_unpack_8 ((gfc_array_i8 *) d, (const GFC_INTEGER_8 *) s);
	      return;
	    }

#ifdef HAVE_GFC_INTEGER_16
	case 16:
	  if (GFC_UNALIGNED_16(d->base_addr) || GFC_UNALIGNED_16(s))
	    break;
	  else
	    {
	      internal_unpack_16 ((gfc_array_i16 *) d,
				  (const GFC_INTEGER_16 *) s);
	      return;
	    }
#endif
	}
      break;

    default:
      break;
    }

  dim = GFC_DESCRIPTOR_RANK (d);
  size = GFC_DESCRIPTOR_ELEM_LEN (d);
  dsize = size;
  for (n = 0; n < dim; n++)
    {
      count[n] = 0;
      sm[n] = GFC_DESCRIPTOR_SM(d,n);
      extent[n] = GFC_DESCRIPTOR_EXTENT(d,n);
      if (extent[n] <= 0)
	return;

      if (dsize == sm[n])
	dsize *= extent[n];
      else
	dsize = 0;
    }

  src = s;

  if (dsize != 0)
    {
      memcpy (dest, src, dsize);
      return;
    }

  sm0 = sm[0];

  while (dest)
    {
      /* Copy the data.  */
      memcpy (dest, src, size);
      /* Advance to the next element.  */
      src += size;
      dest += sm0;
      count[0]++;
      /* Advance to the next source element.  */
      n = 0;
      while (count[n] == extent[n])
        {
          /* When we get to the end of a dimension, reset it and increment
             the next dimension.  */
          count[n] = 0;
          /* We could precalculate these products, but this is a less
             frequently used path so probably not worth it.  */
          dest -= sm[n] * extent[n];
          n++;
          if (n == dim)
            {
              dest = NULL;
              break;
            }
          else
            {
              count[n]++;
              dest += sm[n];
            }
        }
    }
}
