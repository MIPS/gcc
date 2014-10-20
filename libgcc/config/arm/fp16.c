/* Half-float conversion routines.

   Copyright (C) 2008-2013 Free Software Foundation, Inc.
   Contributed by CodeSourcery.

   This file is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* Note: While the EABI (Run-time ABI for the ARM (R) Architecture, IHI 0043C)
   describes the helper routine arguments and return types representing
   half-float values using the type 'short', it does not require that they be
   extended to full register width.
   The ABI normally requires that the caller sign or zero extends short (or
   unsigned short) values to 32 bits.  We use unsigned int arguments and return
   types to prevent GCC making assumptions about the high halves of the
   registers in question.  */

static inline unsigned int
__gnu_f2h_internal (unsigned int a, int ieee)
{
  unsigned int sign = (a >> 16) & 0x8000;
  int aexp = (a >> 23) & 0xff;
  unsigned int mantissa = a & 0x007fffff;
  unsigned int mask;
  unsigned int increment;

  if (aexp == 0xff)
    {
      if (!ieee)
	return sign;
      return sign | 0x7e00 | (mantissa >> 13);
    }
  
  if (aexp == 0 && mantissa == 0)
    return sign;

  aexp -= 127;

  /* Decimal point between bits 22 and 23.  */
  mantissa |= 0x00800000;
  if (aexp < -14)
    {
      mask = 0x00ffffff;
      if (aexp >= -25)
        mask >>= 25 + aexp;
    }
  else
    mask = 0x00001fff;

  /* Round.  */
  if (mantissa & mask)
    {
      increment = (mask + 1) >> 1;
      if ((mantissa & mask) == increment)
	increment = mantissa & (increment << 1);
      mantissa += increment;
      if (mantissa >= 0x01000000)
       	{
	  mantissa >>= 1;
	  aexp++;
	}
    }

  if (ieee)
    {
      if (aexp > 15)
	return sign | 0x7c00;
    }
  else
    {
      if (aexp > 16)
	return sign | 0x7fff;
    }

  if (aexp < -24)
    return sign;

  if (aexp < -14)
    {
      mantissa >>= -14 - aexp;
      aexp = -14;
    }

  /* We leave the leading 1 in the mantissa, and subtract one
     from the exponent bias to compensate.  */
  return sign | (((aexp + 14) << 10) + (mantissa >> 13));
}

static inline unsigned int
__gnu_h2f_internal (unsigned int a, int ieee)
{
  unsigned int sign = (a & 0x00008000) << 16;
  int aexp = (a >> 10) & 0x1f;
  unsigned int mantissa = a & 0x3ff;

  if (aexp == 0x1f && ieee)
    return sign | 0x7f800000 | (mantissa << 13);

  if (aexp == 0)
    {
      int shift;

      if (mantissa == 0)
	return sign;

      shift = __builtin_clz(mantissa) - 21;
      mantissa <<= shift;
      aexp = -shift;
    }

  return sign | (((aexp + 0x70) << 23) + (mantissa << 13));
}

#define ALIAS(src, dst) \
  typeof (src) dst __attribute__ ((alias (#src)));

unsigned int
__gnu_f2h_ieee (unsigned int a)
{
  return __gnu_f2h_internal (a, 1);
}
ALIAS (__gnu_f2h_ieee, __aeabi_f2h)

unsigned int
__gnu_h2f_ieee (unsigned int a)
{
  return __gnu_h2f_internal (a, 1);
}
ALIAS (__gnu_h2f_ieee, __aeabi_h2f)

unsigned int
__gnu_f2h_alternative (unsigned int x)
{
  return __gnu_f2h_internal (x, 0);
}
ALIAS (__gnu_f2h_alternative, __aeabi_f2h_alt)

unsigned int
__gnu_h2f_alternative (unsigned int a)
{
  return __gnu_h2f_internal (a, 0);
}
ALIAS (__gnu_h2f_alternative, __aeabi_h2f_alt)
