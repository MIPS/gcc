/* Software IEEE 128-bit floating-point emulation for PowerPC.
   Copyright (C) 2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Michael Meissner (meissner@linux.vnet.ibm.com)
   Code is based on the main soft-fp library written by:
	Richard Henderson (rth@cygnus.com) and
	Jakub Jelinek (jj@ultra.linux.cz).

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   In addition to the permissions in the GNU Lesser General Public
   License, the Free Software Foundation gives you unlimited
   permission to link the compiled version of this file into
   combinations with other programs, and to distribute those
   combinations without any restriction coming from the use of this
   file.  (The Lesser General Public License restrictions do apply in
   other respects; for example, they cover modification of the file,
   and distribution when not linked into a combine executable.)

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* Convert IBM long double to IEEE 128-bit floating point.  */

#ifdef __FLOAT128_REF__
#define __trunctfkf2	_q_ttoq
#endif

extern __float128 __trunctfkf2 (__ibm128);

__float128
__trunctfkf2 (__ibm128 value)
{
#ifdef __LITTLE_ENDIAN__
  double high = __builtin_unpack_longdouble (value, 0);
  double low = __builtin_unpack_longdouble (value, 1);
#else
  double high = __builtin_unpack_longdouble (value, 1);
  double low = __builtin_unpack_longdouble (value, 0);
#endif

  /* This should catch the special cases of NAN, Infinity, and -0.0, and
     propigate the sign correctly for infinity/0.  */
  if (__builtin_isnan (high) || __builtin_isinf (high)
      || (high == 0.0 && low == 0.0))
    return (__float128) high;

  return ((__float128)low) + ((__float128)high);
}
