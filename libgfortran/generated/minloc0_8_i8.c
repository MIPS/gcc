/* Implementation of the MINLOC intrinsic
   Copyright 2002 Free Software Foundation, Inc.
   Contributed by Paul Brook <paul@nowt.org>

This file is part of the GNU Fortran 95 runtime library (libgfor).

GNU G95 is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

GNU G95 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with libgfor; see the file COPYING.LIB.  If not,
write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include "libgfortran.h"



void
__minloc0_8_i8 (g95_array_i8 * retarray, g95_array_i8 *array)
{
  index_type count[G95_MAX_DIMENSIONS];
  index_type extent[G95_MAX_DIMENSIONS];
  index_type sstride[G95_MAX_DIMENSIONS];
  index_type dstride;
  G95_INTEGER_8 *base;
  G95_INTEGER_8 *dest;
  index_type rank;
  index_type n;

  rank = G95_DESCRIPTOR_RANK (array);
  assert (rank > 0);
  assert (G95_DESCRIPTOR_RANK (retarray) == 1);
  assert (retarray->dim[0].ubound + 1 - retarray->dim[0].lbound == rank);
  if (array->dim[0].stride == 0)
    array->dim[0].stride = 1;
  if (retarray->dim[0].stride == 0)
    retarray->dim[0].stride = 1;

  dstride = retarray->dim[0].stride;
  for (n = 0; n < rank; n++)
    {
      sstride[n] = array->dim[n].stride;
      extent[n] = array->dim[n].ubound + 1 - array->dim[n].lbound;
      count[n] = 0;
      if (extent[n] <= 0)
        return;
    }

  base = array->data;
  dest = retarray->data;
  {

  G95_INTEGER_8 minval;

  /* Initialize the return value.  */
  for (n = 0; n < rank; n++)
    dest[n * dstride] = 0;
  minval = G95_INTEGER_8_HUGE;

  while (base)
    {
      {
        /* Implementation start.  */

  if (*base < minval)
    {
      minval = *base;
      for (n = 0; n < rank; n++)
        dest[n * dstride] = count[n] + 1;
    }
        /* Implementation end.  */
      }
      /* Advance to the next element.  */
      count[0]++;
      base += sstride[0];
      n = 0;
      while (count[n] == extent[n])
        {
          /* When we get to the end of a dimension, reset it and increment
             the next dimension.  */
          count[n] = 0;
          /* We could precalculate these products, but this is a less
             frequently used path so proabably not worth it.  */
          base -= sstride[n] * extent[n];
          n++;
          if (n == rank)
            {
              /* Break out of the loop.  */
              base = NULL;
              break;
            }
          else
            {
              count[n]++;
              base += sstride[n];
            }
        }
    }
  }
}

void
__mminloc0_8_i8 (g95_array_i8 * retarray, g95_array_i8 *array, g95_array_l4 * mask)
{
  index_type count[G95_MAX_DIMENSIONS];
  index_type extent[G95_MAX_DIMENSIONS];
  index_type sstride[G95_MAX_DIMENSIONS];
  index_type mstride[G95_MAX_DIMENSIONS];
  index_type dstride;
  G95_INTEGER_8 *dest;
  G95_INTEGER_8 *base;
  G95_LOGICAL_4 *mbase;
  int rank;
  index_type n;

  rank = G95_DESCRIPTOR_RANK (array);
  assert (rank > 0);
  assert (G95_DESCRIPTOR_RANK (retarray) == 1);
  assert (retarray->dim[0].ubound + 1 - retarray->dim[0].lbound == rank);
  assert (G95_DESCRIPTOR_RANK (mask) == rank);

  if (array->dim[0].stride == 0)
    array->dim[0].stride = 1;
  if (retarray->dim[0].stride == 0)
    retarray->dim[0].stride = 1;
  if (retarray->dim[0].stride == 0)
    retarray->dim[0].stride = 1;

  dstride = retarray->dim[0].stride;
  for (n = 0; n < rank; n++)
    {
      sstride[n] = array->dim[n].stride;
      mstride[n] = mask->dim[n].stride;
      extent[n] = array->dim[n].ubound + 1 - array->dim[n].lbound;
      count[n] = 0;
      if (extent[n] <= 0)
        return;
    }

  dest = retarray->data;
  base = array->data;
  mbase = mask->data;

  if (G95_DESCRIPTOR_SIZE (mask) != 4)
    {
      /* This allows the same loop to be used for all logical types.  */
      assert (G95_DESCRIPTOR_SIZE (mask) == 8);
      for (n = 0; n < rank; n++)
        mstride[n] <<= 1;
      mbase = (GFOR_POINTER_L8_TO_L4 (mbase));
    }

  {

  G95_INTEGER_8 minval;

  /* Initialize the return value.  */
  for (n = 0; n < rank; n++)
    dest[n * dstride] = 0;
  minval = G95_INTEGER_8_HUGE;

  while (base)
    {
      {
        /* Implementation start.  */

  if (*mbase && *base < minval)
    {
      minval = *base;
      for (n = 0; n < rank; n++)
        dest[n * dstride] = count[n] + 1;
    }
        /* Implementation end.  */
      }
      /* Advance to the next element.  */
      count[0]++;
      base += sstride[0];
      mbase += mstride[0];
      n = 0;
      while (count[n] == extent[n])
        {
          /* When we get to the end of a dimension, reset it and increment
             the next dimension.  */
          count[n] = 0;
          /* We could precalculate these products, but this is a less
             frequently used path so proabably not worth it.  */
          base -= sstride[n] * extent[n];
          mbase -= mstride[n] * extent[n];
          n++;
          if (n == rank)
            {
              /* Break out of the loop.  */
              base = NULL;
              break;
            }
          else
            {
              count[n]++;
              base += sstride[n];
              mbase += mstride[n];
            }
        }
    }
  }
}
