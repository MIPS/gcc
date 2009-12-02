/*
 * Copyright (C) 2009, Free Software Foundation, Inc.
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

#ifndef ____LIBSTD_INITFINI_H__
#define ____LIBSTD_INITFINI_H__

#if !defined(__LIBSTD_CRT_BUILD)
#error "__libstd_initfini.h : Sorry : This header is intended for building C runtime only."
#endif

#include <__cdefs.h>

__BEGIN_EXTERN_C

LIBSTD_LPROTO(void,_libstd_init,void);
LIBSTD_LPROTO(void,_libstd_fini,void);

__END_EXTERN_C

#endif /*____LIBSTD_INITFINI_H__*/
