/* Target macros for mips*-elf targets.
   Copyright (C) 1994-2017 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* MIPS assemblers don't have the usual .set foo,bar construct;
   .set is used for assembler options instead.  */
#undef SET_ASM_OP
#define ASM_OUTPUT_DEF(FILE, LABEL1, LABEL2)			\
  do								\
    {								\
      fputc ('\t', FILE);					\
      assemble_name (FILE, LABEL1);				\
      fputs (" = ", FILE);					\
      assemble_name (FILE, LABEL2);				\
      fputc ('\n', FILE);					\
    }								\
  while (0)

#undef ASM_DECLARE_OBJECT_NAME
#define ASM_DECLARE_OBJECT_NAME mips_declare_object_name

#undef ASM_FINISH_DECLARE_OBJECT
#define ASM_FINISH_DECLARE_OBJECT mips_finish_declare_object

/* Leave the linker script to choose the appropriate libraries.  */
#undef  LIB_SPEC
#define LIB_SPEC ""

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "crti%O%s crtbegin%O%s"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtend%O%s crtn%O%s"

#ifndef LINK_COMMAND_SPEC
#define LINK_COMMAND_SPEC "\
%{!fsyntax-only:%{!c:%{!M:%{!MM:%{!E:%{!S:\
    %(linker) " \
    LINK_PLUGIN_SPEC \
   "%{flto|flto=*:%<fcompare-debug*} \
    %{flto} %{fno-lto} %{flto=*} %l " LINK_PIE_SPEC \
   "%{fuse-ld=*:-fuse-ld=%*} " LINK_COMPRESS_DEBUG_SPEC \
   "%X %{o*} %{e*} %{N} %{n} %{r}\
    %{s} %{t} %{u*} %{z} %{Z} %{!nostdlib:%{!nostartfiles:%S}} \
    %{static|no-pie:} %{L*} %(mfwrap) %(link_libgcc) " \
    VTABLE_VERIFICATION_SPEC " " SANITIZER_EARLY_SPEC " %o " CHKP_SPEC " \
    %{fopenacc|fopenmp|%:gt(%{ftree-parallelize-loops=*:%*} 1):\
	%:include(libgomp.spec)%(link_gomp)}\
    %{fcilkplus:%:include(libcilkrts.spec)%(link_cilkrts)}\
    %{fgnu-tm:%:include(libitm.spec)%(link_itm)}\
    %(mflib) " STACK_SPLIT_SPEC "\
    %{fprofile-arcs|fprofile-generate*|coverage:-lgcov} " SANITIZER_SPEC " \
    %{!nostdlib:%{!nodefaultlibs:%(link_ssp) %(link_gcc_c_sequence)}}\
    %{T*} %{!nostdlib:%{!nostartfiles:%E}}  \n%(post_link) }}}}}}"
#endif

#define NO_IMPLICIT_EXTERN_C 1
