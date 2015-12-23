/* Automatic switching between software and hardware IEEE 128-bit
   floating-point emulation for PowerPC.

   Copyright (C) 2015 Free Software Foundation, Inc.
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

#include <soft-fp.h>
#include <quad-float128.h>

TFtype
__addkf3_hw (TFtype a, TFtype b)
{
  __float128 ret;
  __asm__ ("xsaddqp %0,%1,%2" : "=v" (ret) : "v" (a), "v" (b));
  return ret;
}

TFtype
__subkf3_hw (TFtype a, TFtype b)
{
  __float128 ret;
  __asm__ ("xssubqp %0,%1,%2" : "=v" (ret) : "v" (a), "v" (b));
  return ret;
}

TFtype
__mulkf3_hw (TFtype a, TFtype b)
{
  __float128 ret;
  __asm__ ("xsmulqp %0,%1,%2" : "=v" (ret) : "v" (a), "v" (b));
  return ret;
}

TFtype
__divkf3_hw (TFtype a, TFtype b)
{
  __float128 ret;
  __asm__ ("xsdivqp %0,%1,%2" : "=v" (ret) : "v" (a), "v" (b));
  return ret;
}

TFtype
__negkf2_hw (TFtype a)
{
  __float128 ret;
  __asm__ ("xsnegqp %0,%1" : "=v" (ret) : "v" (a));
  return ret;
}

TFtype
__floatsikf_hw (SItype_ppc a)
{
  __float128 ret;
  __asm__ ("mtvsrwa %x0,%1\n\txscvsdqp %0,%0" : "=v" (ret) : "r" (a));

  return ret;
}

TFtype
__floatunsikf_hw (USItype_ppc a)
{
  __float128 ret;
  __asm__ ("mtvsrwz %x0,%1\n\txscvudqp %0,%0" : "=v" (ret) : "r" (a));

  return ret;
}

#ifdef _ARCH_PPC64
TFtype
__floatdikf_hw (DItype_ppc a)
{
  __float128 ret;
  __asm__ ("mtvsrd %x0,%1\n\txscvsdqp %0,%0" : "=v" (ret) : "r" (a));
  return ret;
}

TFtype
__floatundikf_hw (UDItype_ppc a)
{
  __float128 ret;
  __asm__ ("mtvsrd %x0,%1\n\txscvudqp %0,%0" : "=v" (ret) : "r" (a));
  return ret;
}
#endif

SItype_ppc
__fixkfsi_hw (TFtype a)
{
  SItype_ppc ret;
  __float128 tmp;
  __asm__ ("xscvqpswz %1,%2\n\tmfvsrwz %0,%x1\n\textsw %0,%0"
	   : "=r" (ret), "=v" (tmp)
	   : "v" (a));

  return ret;
}

USItype_ppc
__fixunskfsi_hw (TFtype a)
{
  USItype_ppc ret;
  __float128 tmp;
  __asm__ ("xscvqpuwz %1,%2\n\tmfvsrwz %0,%x1"
	   : "=r" (ret), "=v" (tmp)
	   : "v" (a));

  return ret;
}

#ifdef _ARCH_PPC64

DItype_ppc
__fixkfdi_hw (TFtype a)
{
  DItype_ppc ret;
  __float128 tmp;
  __asm__ ("xscvqpsdz %1,%2\n\tmfvsrd %0,%x1"
	   : "=r" (ret), "=v" (tmp)
	   : "v" (a));

  return ret;
}

UDItype_ppc
__fixunskfdi_hw (TFtype a)
{
  UDItype_ppc ret;
  __float128 tmp;
  __asm__ ("xscvqpsdz %1,%2\n\tmfvsrd %0,%x1"
	   : "=r" (ret), "=v" (tmp)
	   : "v" (a));

  return ret;
}
#endif

