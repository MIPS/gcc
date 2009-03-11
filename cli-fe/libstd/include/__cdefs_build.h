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

#ifndef ____CDEFS_BUILD_H__
#define ____CDEFS_BUILD_H__

#if !defined(__LIBSTD_CRT_BUILD)
#error ""
#endif

#include "config.h"

#if defined(__GNU_CIL__)

#  define LIBSTD_LNAME(_name) _name
#  define LIBSTD_SNAME(_name) _name
#  define LIBSTD_HNAME(_name) __host__##_name

#  define LIBSTD_LPROTO(type, name, ...) \
    extern type LIBSTD_LNAME(name)(__VA_ARGS__) __attribute__((cil_strattr("Libstd")))
#  define LIBSTD_SPROTO(type, name, ...) \
    static type LIBSTD_SNAME(name)(__VA_ARGS__) __attribute__((cil_strattr("Libstd")))
#  define LIBSTD_HPROTO(type, name, ...) \
    extern type LIBSTD_HNAME(name)(__VA_ARGS__) __attribute__((assembly_name("MSCorelibWrapper"), cil_name("MSCorelibWrapper::"#name), cil_strattr("Libstd") ))

#  define LIBSTD_LPROTO_IMPL(type, name, ...) \
    type LIBSTD_LNAME(name)(__VA_ARGS__)
#  define LIBSTD_SPROTO_IMPL(type, name, ...) \
    static type LIBSTD_SNAME(name)(__VA_ARGS__)
/* LIBSTD_HPROTO_IMPL intentionally undefined: redirected to MSCorlibWrapper in the prototype */

#elif defined(__ST200) || defined(__SH4__) || defined(__arm__) || defined(__i386__)

#  define LIBSTD_LNAME(_name) Libstd_##_name
#  define LIBSTD_SNAME(_name) Libstd_##_name
#  define LIBSTD_HNAME(_name) Libstd___host__##_name

#  define LIBSTD_LPROTO(type, name, ...) \
    extern type LIBSTD_LNAME(name)(__VA_ARGS__)
#  define LIBSTD_SPROTO(type, name, ...) \
    static type LIBSTD_SNAME(name)(__VA_ARGS__)
#  define LIBSTD_HPROTO(type, name, ...) \
    extern type LIBSTD_HNAME(name)(__VA_ARGS__)

#  define LIBSTD_LPROTO_IMPL(type, name, ...) \
    type LIBSTD_LNAME(name)(__VA_ARGS__)
#  define LIBSTD_SPROTO_IMPL(type, name, ...) \
    static type LIBSTD_SNAME(name)(__VA_ARGS__)
#  define LIBSTD_HPROTO_IMPL(type, name, ...) \
    type LIBSTD_HNAME(name)(__VA_ARGS__)

#else
#  error "Sorry : <__cdefs.h> : Unknown architecture."
#endif

#endif /*____CDEFS_BUILD_H__*/
