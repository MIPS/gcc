/* MIPS SourceryG++ GNU/Linux Configuration.
   Copyright (C) 2006
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

/* We do not need to provide an explicit big-endian multilib.  */
#undef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS \
  { "EB" }

/* The various C libraries each have their own subdirectory.  */
#undef SYSROOT_SUFFIX_SPEC
#define SYSROOT_SUFFIX_SPEC			\
  "%{mel|EL:%{msoft-float:/soft-float/el ;	\
	      :/el} ;				\
     msoft-float:/soft-float}"
