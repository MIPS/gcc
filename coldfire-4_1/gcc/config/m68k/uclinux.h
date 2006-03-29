/* Definitions of target machine for GCC.  m68k/ColdFire based uClinux system
   using ELF objects with special linker post-processing to produce FLAT
   executables.

   Copyright (C) 2003 Free Software Foundation, Inc.

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

/* Override the default LIB_SPEC from gcc.c.  We don't currently support
   profiling, or libg.a.  */
#undef LIB_SPEC
#define LIB_SPEC "\
%{mid-shared-library:-R libc.gdb%s -elf2flt -shared-lib-id 0} -lc \
"

#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
      LINUX_TARGET_OS_CPP_BUILTINS ();		\
      builtin_define ("__uClinux__");		\
      if (TARGET_ID_SHARED_LIBRARY)		\
	builtin_define ("__ID_SHARED_LIBRARY__"); \
    }						\
  while (0)

/* Arbitrarily pick one multilib to be the default.  The true default
   is enforced by the with_cpu setting in config.gcc.  */
#undef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { "mcpu=5206" }

#undef SYSROOT_SUFFIX_SPEC
#define SYSROOT_SUFFIX_SPEC	\
  "%{mcpu=5206: /m5206 ;	\
     mcpu=5206e: /m5206e ;	\
     mcpu=5207: /m5207 ;	\
     mcpu=5211: /m5211 ;	\
     mcpu=5307: /m5307 ;	\
     mcpu=532x: /m532x ;	\
     mcpu=5407: /m5407}"

#undef SYSROOT_HEADERS_SUFFIX_SPEC
#define SYSROOT_HEADERS_SUFFIX_SPEC "/m5206"

/* The uclinux binary format relies on relocations against a segment being
   within that segment.  Conservatively apply this rule to individual
   sections.  */
#undef M68K_OFFSETS_MUST_BE_WITHIN_SECTIONS_P
#define M68K_OFFSETS_MUST_BE_WITHIN_SECTIONS_P 1
