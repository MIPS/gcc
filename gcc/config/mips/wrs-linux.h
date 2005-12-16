/* Wind River GNU/Linux Configuration.
   Copyright (C) 2005
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
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* We do not need to provide an explicit big-endian multilib.  */
#undef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS \
  { "meb" }

/* The GLIBC headers are in /usr/include, relative to the sysroot; the
   uClibc headers are in /uclibc/usr/include.  */
#undef SYSROOT_HEADERS_SUFFIX_SPEC
#define SYSROOT_HEADERS_SUFFIX_SPEC		\
  "%{muclibc:/uclibc}" 

/* The various C libraries each have their own subdirectory.  */
#undef SYSROOT_SUFFIX_SPEC
#define SYSROOT_SUFFIX_SPEC			\
  "%{muclibc:%{mel:/uclibc/mel ;		\
               :/uclibc} ;			\
     mel:/mel}"
