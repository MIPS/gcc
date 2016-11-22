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

/** @file time.h
 *  This is a Standard C++ Library file.  You should @c \#include this file
 *  in your programs, rather than any of the @a *.h implementation files.
 *
 *  This is the C++ wrapper for the Standard C Library header @c time.h,
 *  and it merely replaces names defined as macros in C.
 */

//
// ISO C++ 14882: 20.5  Date and time
//

#pragma GCC system_header

#if (defined(__need_time_t) || defined(__need_clock_t) || \
     defined(__need_timespec))
// Skip this header if specific interfaces are being requested.
#include_next <time.h>
#else

#ifndef _GLIBCXX_TIME_H
#define _GLIBCXX_TIME_H 1

#include_next <time.h>

// Only do any customizations if we're compiling C++ code. We handle this case
// in the event this header is found by header search while compiling C code.
#if __cplusplus

// Get rid of those macros defined in <time.h> in lieu of real functions.
#undef clock
#undef difftime
#undef mktime
#undef time
#undef asctime
#undef ctime
#undef gmtime
#undef localtime
#undef strftime

#endif // C++

#endif

#endif // (! defined __need_time_t && !defined __need_clock_t &&
       //  ! defined __need_timespec)
