/* Definitions of target machine for GCC.  sh based uClinux system
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

#undef STARTFILE_SPEC
#define STARTFILE_SPEC "%{!shared:crt1.o%s} crti.o%s %{!shared:crtbegin.o%s} %{shared:crtbeginS.o%s}"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "%{!shared:crtend.o%s} %{shared:crtendS.o%s} crtn.o%s"

#undef SUBTARGET_LINK_SPEC
#define SUBTARGET_LINK_SPEC "%{shared:-shared} %{static:-static}"
#undef SUBTARGET_LINK_EMUL_SUFFIX
#define SUBTARGET_LINK_EMUL_SUFFIX "_uclinux"

/* we have init/fini section. */
#define HAS_INIT_SECTION

/* Bring in standard linux defines */
#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
	builtin_define ("__uClinux__");		\
	builtin_define_std ("linux");		\
	builtin_define_std ("unix");		\
	builtin_define ("__gnu_linux__");	\
        if (flag_pic)				\
	  {					\
	    builtin_define ("__PIC__");		\
	    builtin_define ("__pic__");		\
	  }					\
	builtin_assert ("system=linux");	\
	builtin_assert ("system=unix");		\
	builtin_assert ("system=posix");	\
    }						\
  while (0)

/* The GNU C++ standard library requires that these macros be defined.  */
#undef CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC "-D_GNU_SOURCE %(cpp)"

/* Since libgcc is compiled with -fpic for this target, we can't use
   __sdivsi3_1 as the division strategy for -O0 and -Os.  */
#undef SH_DIV_STRATEGY_DEFAULT
#define SH_DIV_STRATEGY_DEFAULT SH_DIV_CALL2
#undef SH_DIV_STR_FOR_SIZE
#define SH_DIV_STR_FOR_SIZE "call2"

/* The uclinux binary format relies on relocations against a segment being
   within that segment.  Conservatively apply this rule to individual
   sections.  */
#undef SH_OFFSETS_MUST_BE_WITHIN_SECTIONS_P
#define SH_OFFSETS_MUST_BE_WITHIN_SECTIONS_P 1

