// -*- C++ -*- forwarding header.

// Copyright (C) 1997-2014 Free Software Foundation, Inc.
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

/** @file stdlib.h
 *  This is a Standard C++ Library file.  You should @c \#include this file
 *  in your programs, rather than any of the @a *.h implementation files.
 *
 *  This is the C++ wrapper for the Standard C Library header @c stdlib.h,
 *  and it merely replaces names defined as macros in C.
 */

//
// ISO C++ 14882: 20.4.6  C library
//

#pragma GCC system_header

#ifdef __need_malloc_and_calloc
// We have to skip this header if stdlib.h is being included for specific
// declarations rather than as a full fledged header.
#include_next <stdlib.h>
#else

#ifndef _GLIBCXX_STDLIB_H
#define _GLIBCXX_STDLIB_H 1

// Only do any customizations if we're compiling C++ code. We handle this case
// in the event this header is found by header search while compiling C code.
#if !__cplusplus
#include_next <stdlib.h>
#else // C++

// Since this is a "C" header, it may have been included in an extern "C"
// context. We explicitly put the internal libstdc++ headers back into an extern
// "C++" context in that event.
extern "C++" {
#include <bits/c++config.h>
}

#if !_GLIBCXX_HOSTED
// The C standard does not require a freestanding implementation to provide
// <stdlib.h>.  However, the C++ standard does still require <cstdlib> and for
// compatibility <stdlib.h> -- but only the functionality mentioned in
// [lib.support.start.term].

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

extern "C" void abort(void) throw () _GLIBCXX_NORETURN;
extern "C" int atexit(void (*)(void)) throw ();
extern "C" void exit(int) throw () _GLIBCXX_NORETURN;
#if __cplusplus >= 201103L
# ifdef _GLIBCXX_HAVE_AT_QUICK_EXIT
extern "C" int at_quick_exit(void (*)(void)) throw ();
# endif
# ifdef _GLIBCXX_HAVE_QUICK_EXIT
extern "C" void quick_exit(int) throw() _GLIBCXX_NORETURN;
# endif
#endif

#else

#include_next <stdlib.h>

// Get rid of those macros defined in <stdlib.h> in lieu of real functions.
#undef abort
#undef abs
#undef atexit
#if __cplusplus >= 201103L
# ifdef _GLIBCXX_HAVE_AT_QUICK_EXIT
#  undef at_quick_exit
# endif
#endif
#undef atof
#undef atoi
#undef atol
#undef bsearch
#undef calloc
#undef div
#undef exit
#undef free
#undef getenv
#undef labs
#undef ldiv
#undef malloc
#undef mblen
#undef mbstowcs
#undef mbtowc
#undef qsort
#if __cplusplus >= 201103L
# ifdef _GLIBCXX_HAVE_QUICK_EXIT
#  undef quick_exit
# endif
#endif
#undef rand
#undef realloc
#undef srand
#undef strtod
#undef strtol
#undef strtoul
#undef system
#undef wcstombs
#undef wctomb

#if _GLIBCXX_USE_C99

#undef _Exit
#undef llabs
#undef lldiv
#undef atoll
#undef strtoll
#undef strtoull
#undef strtof
#undef strtold

#endif // _GLIBCXX_USE_C99

#endif // !_GLIBCXX_HOSTED

#endif // C++

#endif

#endif // !__need_malloc_and_calloc
