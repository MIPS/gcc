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

/** @file wctype.h
 *  This is a Standard C++ Library file.  You should @c \#include this file
 *  in your programs, rather than any of the @a *.h implementation files.
 *
 *  This is the C++ wrapper for the Standard C Library header @c math.h,
 *  and it merely replaces names defined as macros in C.
 */

//
// ISO C++ 14882: <cwctype>
//

#pragma GCC system_header

#ifdef __need_iswxxx
// Skip this header if specific interfaces are being requested.
#include_next <wctype.h>
#else

#ifndef _GLIBCXX_WCTYPE_H
#define _GLIBCXX_WCTYPE_H 1

// Only do any customizations if we're compiling C++ code. We handle this case
// in the event this header is found by header search while compiling C code.
#if !__cplusplus
#include_next <wctype.h>
#else // C++

// Since this is a "C" header, it may have been included in an extern "C"
// context. We explicitly put the internal libstdc++ headers back into an extern
// "C++" context in that event.
extern "C++" {
#include <bits/c++config.h>
}

#if _GLIBCXX_HAVE_WCTYPE_H

#if __GLIBC__ == 2 && __GLIBC_MINOR__ < 10
// Work around glibc BZ 9694
#include <stddef.h>
#endif

#include_next <wctype.h>

#endif // _GLIBCXX_HAVE_WCTYPE_H

// Get rid of those macros defined in <wctype.h> in lieu of real functions.
#undef iswalnum
#undef iswalpha
#if _GLIBCXX_HAVE_ISWBLANK
# undef iswblank
#endif
#undef iswcntrl
#undef iswctype
#undef iswdigit
#undef iswgraph
#undef iswlower
#undef iswprint
#undef iswpunct
#undef iswspace
#undef iswupper
#undef iswxdigit
#undef towctrans
#undef towlower
#undef towupper
#undef wctrans
#undef wctype

#endif // C++

#endif // _GLIBCXX_WCTYPE_H

#endif // !__need_iswxxx
