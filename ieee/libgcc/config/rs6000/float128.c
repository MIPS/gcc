/* Functions needed for software support of IEEE 128-bit floating point
   (i.e. __float128 or KFmode).
 
   Copyright (C) 2014 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#include <math.h>

#ifndef __FLOAT128__
#error "-mfloat128 is required"
#endif

#if !defined(__LONG_DOUBLE_128__) || defined(__LONG_DOUBLE_IEEE128__)
#error "long double must use the IBM long double format"
#endif

long double
__extendkftf2 (__float128 value)
{
  double high, low;

  high = (double) value;
  low = ((isnan (high) || isinf (high))
	 ? high
	 : (double) (value - ((long double)high)));

#ifdef __LITTLE_ENDIAN__
  return __builtin_pack_longdouble (low, high);
#else
  return __builtin_pack_longdouble (high, low);
#endif
}

__float128
__trunctfkf2 (long double value)
{
#ifdef __LITTLE_ENDIAN__
  double high = __builtin_unpack_longdouble (value, 0);
  double low = __builtin_unpack_longdouble (value, 1);
#else
  double high = __builtin_unpack_longdouble (value, 1);
  double low = __builtin_unpack_longdouble (value, 0);
#endif

  if (isnan (high))
    return (__float128) NAN;

  if (isinf (high))
    return (__float128) INFINITY;

  if (low == 0.0)
    return (__float128)high;

  return ((__float128)high) + ((__float128)low);
}
