/*

   Copyright (C) 2007-2008 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

/* If we are compiling this, we are sure this is to build C-runtime*/
#define __LIBSTD_CRT_BUILD

#include "__libinfo.h"

static const char __libinfo__version_[] = "[libstd $Revision: $.$$ $Date: $$$$/$$/$$]";
static const char __libinfo__build_[]   = "[" __DATE__ " " __TIME__ "]";
static const char __libinfo__tools_[]   = "["
#if defined(__GNU_CIL__)
  "gcc4net"
#elif defined(__open64__)
  "open64"
#elif defined(__GNUC__)
  "gcc"
#else
  "Unknown"
#endif
  "]";
static const char __libinfo__target_[]  = "["
#if defined(__cil32__)
  "CIL32"
#elif defined(__cil64__)
  "CIL64"
#elif defined(__unix)
  "Unix"
#elif defined(__ST200)
  "ST200"
#else
  "Unknown"
#endif /* target list */
  "]";

LIBSTD_LPROTO_IMPL(const char *, __libinfo__version, void)
{
    return __libinfo__version_;
}

LIBSTD_LPROTO_IMPL(const char *, __libinfo__build, void)
{
    return __libinfo__build_;
}

LIBSTD_LPROTO_IMPL(const char *, __libinfo__tools, void)
{
    return __libinfo__tools_;
}

LIBSTD_LPROTO_IMPL(const char *, __libinfo__target, void)
{
    return __libinfo__target_;
}
