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

#undef STARTFILE_SPEC
#define STARTFILE_SPEC "crt1.o%s crti.o%s crtbegin.o%s"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtend.o%s crtn.o%s"

/* Override the default LIB_SPEC from gcc.c.  We don't currently support
   profiling, or libg.a.  */
#undef LIB_SPEC
#define LIB_SPEC "\
%{mid-shared-library:-R libc.gdb%s -elf2flt -shared-lib-id 0} -lc \
"

/* we don't want a .eh_frame section.  */
#define EH_FRAME_IN_DATA_SECTION

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
	builtin_assert ("system=linux");	\
	builtin_assert ("system=unix");		\
	builtin_assert ("system=posix");	\
	if (TARGET_ID_SHARED_LIBRARY)		\
	  builtin_define ("__ID_SHARED_LIBRARY__"); \
    }						\
  while (0)

/* uclibc uses comments to hide section attributes.  So setting
   ASM_FILE_START_APP_OFF breaks it.  */
#define M68K_NO_ASM_FILE_START_APP_OFF 1
