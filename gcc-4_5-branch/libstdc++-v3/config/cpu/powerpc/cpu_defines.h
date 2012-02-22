// Specific definitions for generic platforms  -*- C++ -*-

// Copyright (C) 2005, 2009 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file cpu_defines.h
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#ifndef _GLIBCXX_CPU_DEFINES
#define _GLIBCXX_CPU_DEFINES 1

// Integer divide instructions don't trap on PowerPC.
#define __glibcxx_integral_traps false

#ifdef IN_LIBSTDCXX
#ifdef __PPC64__
#define __rs6000_atomic_fetch_and_add_acquire_32(mem, constant) \
({							\
  __typeof (*mem) __tmpval, __tmp0, __tmp1;		\
  __asm __volatile (					\
    "1: lwarx %2,0,%4\n"				\
    "   addi %1,%2,%5\n"				\
    "   stwcx. %1,0,%4\n"				\
    "   bne- 1b\n"					\
    "   extsw %0,%2\n"					\
    "   isync\n"					\
    : "=&r" (__tmpval), "=&r" (__tmp0), "=&b" (__tmp1), "=m" (*mem) \
    : "b" (mem), "i" (constant), "m" (*mem)		\
    : "cr0", "memory");					\
  __tmpval;						\
})
#define __rs6000_atomic_fetch_and_add_release_32(mem, constant) \
({							\
  __typeof (*mem) __tmpval, __tmp0, __tmp1;		\
  __asm __volatile (					\
    "   lwsync\n"					\
    "1: lwarx %2,0,%4\n"				\
    "   addi %1,%2,%5\n"				\
    "   stwcx. %1,0,%4\n"				\
    "   bne- 1b\n"					\
    "   extsw %0,%2\n"					\
    : "=&r" (__tmpval), "=&r" (__tmp0), "=&b" (__tmp1), "=m" (*mem) \
    : "b" (mem), "i" (constant), "m" (*mem)		\
    : "cr0", "memory");					\
  __tmpval;						\
})
#else
#define __rs6000_atomic_fetch_and_add_acquire_32(mem, constant) \
({							\
  __typeof (*mem) __tmpval, __tmp0;			\
  __asm __volatile (					\
    "1: lwarx %0,0,%3\n"				\
    "   addi %1,%0,%4\n"				\
    "   stwcx. %1,0,%3\n"				\
    "   bne- 1b\n"					\
    "   isync\n"					\
    : "=&b" (__tmpval), "=&r" (__tmp0), "=m" (*mem)	\
    : "b" (mem), "i" (constant), "m" (*mem)		\
    : "cr0", "memory");					\
  __tmpval;						\
})
#define __rs6000_atomic_fetch_and_add_release_32(mem, constant) \
({							\
  __typeof (*mem) __tmpval, __tmp0;			\
  __asm __volatile (					\
    "   lwsync\n"					\
    "1: lwarx %0,0,%3\n"				\
    "   addi %1,%0,%4\n"				\
    "   stwcx. %1,0,%3\n"				\
    "   bne- 1b\n"					\
    : "=&b" (__tmpval), "=&r" (__tmp0), "=m" (*mem)	\
    : "b" (mem), "i" (constant), "m" (*mem)		\
    : "cr0", "memory");					\
  __tmpval;						\
})
#endif /* __PPC64__ */
#endif /* IN_LIBSTDCXX */

#endif
