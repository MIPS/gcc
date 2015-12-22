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
have_hw_p (void)
{
  static int have_hw = -1;

  if (have_hw < 0)
    {
      char *p = (char *) getauxval (AT_PLATFORM);

      have_hw = (p && strncmp (p, "power", 5) == 0
		 && strtoul (p + 5, (char **)0, 0) >= 9);
    }

  return have_hw;
}

#define SW_OR_HW(SW, HW) (have_hw_p () ? HW : SW)
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

/* Finish writing asm versions of these functions.  For now, always use the
   software emulation function.  */
static void *
__eqkf2_resolve (void)
{
  return (void *) __eqkf2_sw;
}

static void *
__gekf2_resolve (void)
{
  return (void *) __gekf2_sw;
}

static void *
__lekf2_resolve (void)
{
  return (void *) __lekf2_sw;
}

static void *
__unordkf2_resolve (void)
{
  return (void *) __unordkf2_sw;
}

static void *
__extendsfkf2_resolve (void)
{
  return (void *) __extendsfkf2_sw;
}

static void *
__extenddfkf2_resolve (void)
{
  return (void *) __extenddfkf2_sw;
}

static void *
__trunckfsf2_resolve (void)
{
  return (void *) __trunckfsf2_sw;
}

static void *
__trunckfdf2_resolve (void)
{
  return (void *) __trunckfdf2_sw;
}

static void *
__fixkfsi_resolve (void)
{
  return (void *) __fixkfsi_sw;
}

static void *
__fixkfdi_resolve (void)
{
  return (void *) __fixkfdi_sw;
}

static void *
__fixunskfsi_resolve (void)
{
  return (void *) __fixunskfsi_sw;
}

static void *
__fixunskfdi_resolve (void)
{
  return (void *) __fixunskfdi_sw;
}

static void *
__floatsikf_resolve (void)
{
  return (void *) __floatsikf_sw;
}

static void *
__floatdikf_resolve (void)
{
  return (void *) __floatdikf_sw;
}

static void *
__floatunsikf_resolve (void)
{
  return (void *) __floatunsikf_sw;
}

static void *
__floatundikf_resolve (void)
{
  return (void *) __floatundikf_sw;
}

#ifdef _ARCH_PPC64
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

