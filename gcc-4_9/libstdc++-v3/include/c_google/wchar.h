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

/** @file wchar.h
 *  This is a Standard C++ Library file.  You should @c \#include this file
 *  in your programs, rather than any of the @a *.h implementation files.
 *
 *  This is the C++ wrapper for the Standard C Library header @c wchar.h,
 *  and it merely replaces names defined as macros in C.
 */

//
// ISO C++ 14882: 21.4
//

#pragma GCC system_header

#if defined(__need_mbstate_t) || defined(__need_wint_t)
// Skip this header if specific interfaces are being requested.
#include_next <wchar.h>
#else

#ifndef _GLIBCXX_WCHAR_H
#define _GLIBCXX_WCHAR_H 1

// Only do any customizations if we're compiling C++ code. We handle this case
// in the event this header is found by header search while compiling C code.
#if !__cplusplus
#include_next <wchar.h>
#else // C++

// Since this is a "C" header, it may have been included in an extern "C"
// context. We explicitly put the internal libstdc++ headers back into an extern
// "C++" context in that event.
extern "C++" {
#include <bits/c++config.h>
}

#if _GLIBCXX_HAVE_WCHAR_H
#include_next <wchar.h>
#endif

// Need to do a bit of trickery here with mbstate_t as char_traits
// assumes it is in wchar.h, regardless of wchar_t specializations.
#ifndef _GLIBCXX_HAVE_MBSTATE_T
extern "C"
{
  typedef struct
  {
    int __fill[6];
  } mbstate_t;
}
#endif

// Get rid of those macros defined in <wchar.h> in lieu of real functions.
#undef btowc
#undef fgetwc
#undef fgetws
#undef fputwc
#undef fputws
#undef fwide
#undef fwprintf
#undef fwscanf
#undef getwc
#undef getwchar
#undef mbrlen
#undef mbrtowc
#undef mbsinit
#undef mbsrtowcs
#undef putwc
#undef putwchar
#undef swprintf
#undef swscanf
#undef ungetwc
#undef vfwprintf
#if _GLIBCXX_HAVE_VFWSCANF
# undef vfwscanf
#endif
#undef vswprintf
#if _GLIBCXX_HAVE_VSWSCANF
# undef vswscanf
#endif
#undef vwprintf
#if _GLIBCXX_HAVE_VWSCANF
# undef vwscanf
#endif
#undef wcrtomb
#undef wcscat
#undef wcschr
#undef wcscmp
#undef wcscoll
#undef wcscpy
#undef wcscspn
#undef wcsftime
#undef wcslen
#undef wcsncat
#undef wcsncmp
#undef wcsncpy
#undef wcspbrk
#undef wcsrchr
#undef wcsrtombs
#undef wcsspn
#undef wcsstr
#undef wcstod
#if _GLIBCXX_HAVE_WCSTOF
# undef wcstof
#endif
#undef wcstok
#undef wcstol
#undef wcstoul
#undef wcsxfrm
#undef wctob
#undef wmemchr
#undef wmemcmp
#undef wmemcpy
#undef wmemmove
#undef wmemset
#undef wprintf
#undef wscanf

#if _GLIBCXX_USE_WCHAR_T

#if _GLIBCXX_USE_C99

#undef wcstold
#undef wcstoll
#undef wcstoull

#endif

#endif //_GLIBCXX_USE_WCHAR_T

#endif // C++

#endif

#endif // !defined __need_mbstate_t && !defined __need_wint_t
