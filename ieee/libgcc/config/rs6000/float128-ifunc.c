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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/auxv.h>

#if defined(FLOAT128_HW_INSNS) && defined(AT_PLATFORM)
static int
have_ieee_hw_p (void)
{
  static int ieee_hw_p = -1;

  if (ieee_hw_p < 0)
    {
      char *p = (char *) getauxval (AT_PLATFORM);

      ieee_hw_p = 0;

      /* Use __builtin_cpu_supports once it is supported by the PowerPC.  */
      if (p && p[0] == 'p' && p[1] == 'o' && p[2] == 'w' && p[3] == 'e'
	  && p[4] == 'r')
	{
	  long n = 0;
	  char ch;

	  /* Don't use atoi/strtol/etc.  These require the normal environment
	     to be setup to set errno to 0, and the ifunc resolvers run before
	     the whole glibc environment is initialized.  */
	  p += 5;
	  while ((ch = *p++) >= '0' && (ch <= '9'))
	    n = (n * 10) + (ch - '0');

	  if (n >= 9)
	    ieee_hw_p = 1;
	}
    }

  return ieee_hw_p;
}

#define SW_OR_HW(SW, HW) (have_ieee_hw_p () ? HW : SW)
#else
#define SW_OR_HW(SW, HW) (SW)
#endif	/* ISA 3.0 hardware available.  */

/* Resolvers.  */
static void *__addkf3_resolve (void);
static void *__subkf3_resolve (void);
static void *__mulkf3_resolve (void);
static void *__divkf3_resolve (void);
static void *__negkf2_resolve (void);
static void *__eqkf2_resolve (void);
static void *__gekf2_resolve (void);
static void *__lekf2_resolve (void);
static void *__unordkf2_resolve (void);
static void *__extendsfkf2_resolve (void);
static void *__extenddfkf2_resolve (void);
static void *__trunckfsf2_resolve (void);
static void *__trunckfdf2_resolve (void);
static void *__fixkfsi_resolve (void);
static void *__fixkfdi_resolve (void);
static void *__fixunskfsi_resolve (void);
static void *__fixunskfdi_resolve (void);
static void *__floatsikf_resolve (void);
static void *__floatdikf_resolve (void);
static void *__floatunsikf_resolve (void);
static void *__floatundikf_resolve (void);

#ifdef _ARCH_PPC64
static void *__fixkfti_resolve (void);
static void *__fixunskfti_resolve (void);
static void *__floattikf_resolve (void);
static void *__floatuntikf_resolve (void);
#endif	/* _ARCH_PPC64.  */

static void *
__addkf3_resolve (void)
{
  return (void *) SW_OR_HW (__addkf3_sw, __addkf3_hw);
}

static void *
__subkf3_resolve (void)
{
  return (void *) SW_OR_HW (__subkf3_sw, __subkf3_hw);
}

static void *
__mulkf3_resolve (void)
{
  return (void *) SW_OR_HW (__mulkf3_sw, __mulkf3_hw);
}

static void *
__divkf3_resolve (void)
{
  return (void *) SW_OR_HW (__divkf3_sw, __divkf3_hw);
}

static void *
__negkf2_resolve (void)
{
  return (void *) SW_OR_HW (__negkf2_sw, __negkf2_hw);
}

static void *
__floatsikf_resolve (void)
{
  return (void *) SW_OR_HW (__floatsikf_sw, __floatsikf_hw);
}

static void *
__floatdikf_resolve (void)
{
  return (void *) SW_OR_HW (__floatdikf_sw, __floatdikf_hw);
}

static void *
__floatunsikf_resolve (void)
{
  return (void *) SW_OR_HW (__floatunsikf_sw, __floatunsikf_hw);
}

static void *
__floatundikf_resolve (void)
{
  return (void *) SW_OR_HW (__floatundikf_sw, __floatundikf_hw);
}

static void *
__fixkfsi_resolve (void)
{
  return (void *) SW_OR_HW (__fixkfsi_sw, __fixkfsi_hw);
}

static void *
__fixkfdi_resolve (void)
{
  return (void *) SW_OR_HW (__fixkfdi_sw, __fixkfdi_hw);
}

static void *
__fixunskfsi_resolve (void)
{
  return (void *) SW_OR_HW (__fixunskfsi_sw, __fixunskfsi_hw);
}

static void *
__fixunskfdi_resolve (void)
{
  return (void *) SW_OR_HW (__fixunskfdi_sw, __fixunskfdi_hw);
}

static void *
__extendsfkf2_resolve (void)
{
  return (void *) SW_OR_HW (__extendsfkf2_sw, __extendsfkf2_hw);
}

static void *
__extenddfkf2_resolve (void)
{
  return (void *) SW_OR_HW (__extenddfkf2_sw, __extenddfkf2_hw);
}

static void *
__trunckfsf2_resolve (void)
{
  return (void *) SW_OR_HW (__trunckfsf2_sw, __trunckfsf2_hw);
}

static void *
__trunckfdf2_resolve (void)
{
  return (void *) SW_OR_HW (__trunckfdf2_sw, __trunckfdf2_hw);
}

static void *
__eqkf2_resolve (void)
{
  return (void *) SW_OR_HW (__eqkf2_sw, __eqkf2_hw);
}

static void *
__gekf2_resolve (void)
{
  return (void *) SW_OR_HW (__gekf2_sw, __gekf2_hw);
}

static void *
__lekf2_resolve (void)
{
  return (void *) SW_OR_HW (__lekf2_sw, __lekf2_hw);
}

static void *
__unordkf2_resolve (void)
{
  return (void *) SW_OR_HW (__unordkf2_sw, __unordkf2_hw);
}

#ifdef _ARCH_PPC64
/* For now, just use the emulator for 128-bit integer types.  */
static void *
__fixkfti_resolve (void)
{
  return (void *) __fixkfti_sw;
}

static void *
__fixunskfti_resolve (void)
{
  return (void *) __fixunskfti_sw;
}

static void *
__floattikf_resolve (void)
{
  return (void *) __floattikf_sw;
}

static void *
__floatuntikf_resolve (void)
{
  return (void *) __floatuntikf_sw;
}
#endif	/* _ARCH_PPC64.  */


/* Ifunc definitions.  */
TFtype __addkf3 (TFtype, TFtype)
  __attribute__ ((__ifunc__ ("__addkf3_resolve")));

TFtype __subkf3 (TFtype, TFtype)
  __attribute__ ((__ifunc__ ("__subkf3_resolve")));

TFtype __mulkf3 (TFtype, TFtype)
  __attribute__ ((__ifunc__ ("__mulkf3_resolve")));

TFtype __divkf3 (TFtype, TFtype)
  __attribute__ ((__ifunc__ ("__divkf3_resolve")));

TFtype __negkf2 (TFtype)
  __attribute__ ((__ifunc__ ("__negkf2_resolve")));

CMPtype __eqkf2 (TFtype, TFtype)
  __attribute__ ((__ifunc__ ("__eqkf2_resolve")));

CMPtype __gekf2 (TFtype, TFtype)
  __attribute__ ((__ifunc__ ("__gekf2_resolve")));

CMPtype __lekf2 (TFtype, TFtype)
  __attribute__ ((__ifunc__ ("__lekf2_resolve")));

CMPtype __unordkf2 (TFtype, TFtype)
  __attribute__ ((__ifunc__ ("__unordkf2_resolve")));

TFtype __extendsfkf2 (float)
  __attribute__ ((__ifunc__ ("__extendsfkf2_resolve")));

TFtype __extenddfkf2 (double)
  __attribute__ ((__ifunc__ ("__extenddfkf2_resolve")));

float __trunckfsf2 (TFtype)
  __attribute__ ((__ifunc__ ("__trunckfsf2_resolve")));

double __trunckfdf2 (TFtype)
  __attribute__ ((__ifunc__ ("__trunckfdf2_resolve")));

SItype_ppc __fixkfsi (TFtype)
  __attribute__ ((__ifunc__ ("__fixkfsi_resolve")));

DItype_ppc __fixkfdi (TFtype)
  __attribute__ ((__ifunc__ ("__fixkfdi_resolve")));

USItype_ppc __fixunskfsi (TFtype)
  __attribute__ ((__ifunc__ ("__fixunskfsi_resolve")));

UDItype_ppc __fixunskfdi (TFtype)
  __attribute__ ((__ifunc__ ("__fixunskfdi_resolve")));

TFtype __floatsikf (SItype_ppc)
  __attribute__ ((__ifunc__ ("__floatsikf_resolve")));

TFtype __floatdikf (DItype_ppc)
  __attribute__ ((__ifunc__ ("__floatdikf_resolve")));

TFtype __floatunsikf (USItype_ppc)
  __attribute__ ((__ifunc__ ("__floatunsikf_resolve")));

TFtype __floatundikf (UDItype_ppc)
  __attribute__ ((__ifunc__ ("__floatundikf_resolve")));

#ifdef _ARCH_PPC64
TItype_ppc __fixkfti (TFtype)
  __attribute__ ((__ifunc__ ("__fixkfti_resolve")));

UTItype_ppc __fixunskfti (TFtype)
  __attribute__ ((__ifunc__ ("__fixunskfti_resolve")));

TFtype __floattikf (TItype_ppc)
  __attribute__ ((__ifunc__ ("__floattikf_resolve")));

TFtype __floatuntikf (UTItype_ppc)
  __attribute__ ((__ifunc__ ("__floatuntikf_resolve")));
#endif

