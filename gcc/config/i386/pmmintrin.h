/* Copyright (C) 2003 Free Software Foundation, Inc.

   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, if you include this header file into source
   files compiled by GCC, this header file does not by itself cause
   the resulting executable to be covered by the GNU General Public
   License.  This exception does not however invalidate any other
   reasons why the executable file might be covered by the GNU General
   Public License.  */

/* Implemented from the specification included in the Intel C++ Compiler
   User Guide and Reference, version 8.0.  */

#ifndef _PMMINTRIN_H_INCLUDED
#define _PMMINTRIN_H_INCLUDED

#ifdef __SSE3__

#if 0
static __inline void
_mm_monitor (void const * __P, unsigned int __E, unsigned int __H)
{
  __builtin_ia32_monitor (__P, __E, __H);
}

static __inline void
_mm_mwait (unsigned int __E, unsigned int __H)
{
  __builtin_ia32_mwait (__E, __H);
}
#else
#define _mm_monitor(P, E, H)	__builtin_ia32_monitor ((P), (E), (H))
#define _mm_mwait(E, H)		__builtin_ia32_mwait ((E), (H))
#endif

#endif /* __SSE3__ */

#endif /* _PMMINTRIN_H_INCLUDED */
