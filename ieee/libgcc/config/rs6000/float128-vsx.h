/* Map __<func>tf<n> used by the standard IEEE 128-bit floating point routines
   to the name used for __float128 when used on a VSX system (KFmode).

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

#define __addtf3	__addkf3
#define __subtf3	__subkf3
#define __multf3	__mulkf3
#define __divtf3	__divkf3
#define __eqtf2		__eqkf2
#define __netf2		__nekf2
#define __getf2		__gekf2
#define __gttf2		__gtkf2
#define __letf2		__lekf2
#define __lttf2		__ltkf2
#define __unordtf2	__unordkf2
#define __negtf2	__negkf2
#define __extenddftf2	__extenddfkf2
#define __extendsftf2	__extendsfkf2
#define __trunctfdf2	__trunckfdf2
#define __trunctfsf2	__trunckfsf2
#define __fixtfsi	__fixkfsi
#define __fixtfdi	__fixkfdi
#define __fixtfti	__fixkfti
#define __fixunstfsi	__fixunskfsi
#define __fixunstfdi	__fixunskfdi
#define __fixunstfti	__fixunskfti
#define __floatsitf	__floatsikf
#define __floatditf	__floatdikf
#define __floattitf	__floattikf
#define __floatunsitf	__floatunsikf
#define __floatunditf	__floatundikf
#define __floatuntitf	__floatuntikf
