/* Definitions for MIPS systems using GNU userspace.
   Copyright (C) 1998-2018 Free Software Foundation, Inc.

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

#undef WCHAR_TYPE
#define WCHAR_TYPE "int"

#undef WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 32

#undef ASM_DECLARE_OBJECT_NAME
#define ASM_DECLARE_OBJECT_NAME mips_declare_object_name

#ifndef NANOMIPS_SUPPORT
/* If we don't set MASK_ABICALLS, we can't default to PIC.  */
#undef TARGET_DEFAULT
#define TARGET_DEFAULT MASK_ABICALLS
#endif

#define TARGET_OS_CPP_BUILTINS()				\
  do {								\
    GNU_USER_TARGET_OS_CPP_BUILTINS();				\
    /* The GNU C++ standard library requires this.  */		\
    if (c_dialect_cxx ())					\
      builtin_define ("_GNU_SOURCE");				\
  } while (0)

#undef SUBTARGET_CPP_SPEC
#define SUBTARGET_CPP_SPEC "%{posix:-D_POSIX_SOURCE} %{pthread:-D_REENTRANT}"

/* A standard GNU/Linux mapping.  On most targets, it is included in
   CC1_SPEC itself by config/linux.h, but mips.h overrides CC1_SPEC
   and provides this hook instead.  */
#undef SUBTARGET_CC1_SPEC
#define SUBTARGET_CC1_SPEC GNU_USER_TARGET_CC1_SPEC

/* -G is incompatible with -KPIC which is the default, so only allow objects
   in the small data section if the user explicitly asks for it.  */
#undef MIPS_DEFAULT_GVALUE
#define MIPS_DEFAULT_GVALUE 0

#ifdef NANOMIPS_SUPPORT
#undef GNU_USER_TARGET_LINK_SPEC
#define GNU_USER_TARGET_LINK_SPEC "\
  %{EB} %{EL} %{shared} \
  %{!shared: \
    %{!static: \
      %{rdynamic:-export-dynamic} \
      %{m64: -dynamic-linker " GNU_USER_DYNAMIC_LINKER64 "} \
      %{m32: -dynamic-linker " GNU_USER_DYNAMIC_LINKER32 "}} \
    %{static}} \
  %{m64:-m" GNU_USER_LINK_EMULATION64 "} \
  %{m32:-m" GNU_USER_LINK_EMULATION32 "}"
#else
#undef GNU_USER_TARGET_LINK_SPEC
#define GNU_USER_TARGET_LINK_SPEC "\
  %{G*} %{EB} %{EL} %{mips*} %{shared} \
  %{!shared: \
    %{!static: \
      %{rdynamic:-export-dynamic} \
      %{mabi=n32: -dynamic-linker " GNU_USER_DYNAMIC_LINKERN32 "} \
      %{mabi=64: -dynamic-linker " GNU_USER_DYNAMIC_LINKER64 "} \
      %{mabi=32: -dynamic-linker " GNU_USER_DYNAMIC_LINKER32 "}} \
    %{static}} \
  %{mabi=n32:-m" GNU_USER_LINK_EMULATIONN32 "} \
  %{mabi=64:-m" GNU_USER_LINK_EMULATION64 "} \
  %{mabi=32:-m" GNU_USER_LINK_EMULATION32 "}"
#endif

#undef LINK_SPEC
#define LINK_SPEC GNU_USER_TARGET_LINK_SPEC

#ifndef NANOMIPS_SUPPORT
#undef SUBTARGET_ASM_SPEC
#define SUBTARGET_ASM_SPEC \
  "%{!mno-abicalls:%{mplt:-call_nonpic;:-KPIC}}"
#endif

/* The MIPS assembler has different syntax for .set. We set it to
   .dummy to trap any errors.  */
#undef SET_ASM_OP
#define SET_ASM_OP "\t.dummy\t"

#undef ASM_OUTPUT_DEF
#define ASM_OUTPUT_DEF(FILE,LABEL1,LABEL2)				\
 do {									\
	fputc ( '\t', FILE);						\
	assemble_name (FILE, LABEL1);					\
	fputs ( " = ", FILE);						\
	assemble_name (FILE, LABEL2);					\
	fputc ( '\n', FILE);						\
 } while (0)

#ifndef NANOMIPS_SUPPORT
/* The glibc _mcount stub will save $v0 for us.  Don't mess with saving
   it, since ASM_OUTPUT_REG_PUSH/ASM_OUTPUT_REG_POP do not work in the
   presence of $gp-relative calls.  */
#undef ASM_OUTPUT_REG_PUSH
#undef ASM_OUTPUT_REG_POP
#endif

#undef LIB_SPEC
#define LIB_SPEC GNU_USER_TARGET_LIB_SPEC

#if defined(HAVE_AS_NO_SHARED) && !defined(NANOMIPS_SUPPORT)
/* Default to -mno-shared for non-PIC.  */
# define NO_SHARED_SPECS \
  " %{mshared|mno-shared:;:%{" NO_FPIE_AND_FPIC_SPEC ":-mno-shared}}"
#else
# define NO_SHARED_SPECS ""
#endif

/* -march=native handling only makes sense with compiler running on
   a MIPS chip.  */
#if defined(__mips)
extern const char *host_detect_local_cpu (int argc, const char **argv);
# define EXTRA_SPEC_FUNCTIONS \
  { "local_cpu_detect", host_detect_local_cpu },

# define MARCH_MTUNE_NATIVE_SPECS				\
  " %{march=native:%<march=native %:local_cpu_detect(arch)}"	\
  " %{mtune=native:%<mtune=native %:local_cpu_detect(tune)}"
#else
# define MARCH_MTUNE_NATIVE_SPECS ""
#endif

#if DEFAULT_LIBC == LIBC_MUSL
# define DEFAULT_LIBC_SPECS "%{!mglibc:-mmusl}"
#else
# define DEFAULT_LIBC_SPECS ""
#endif

#ifdef NANOMIPS_SUPPORT
#define LINUX_DRIVER_SELF_SPECS \
  NO_SHARED_SPECS							\
  MARCH_MTUNE_NATIVE_SPECS,						\
  DEFAULT_LIBC_SPECS,							\
  /* -mplt has no effect without -mno-shared.  Simplify later		\
     specs handling by removing a redundant option.  */			\
  "%{!mno-shared:%<mplt}",						\
  /* -mplt likewise has no effect for -mabi=64 without -msym32.  */	\
  "%{mabi=64:%{!msym32:%<mplt}}",					\
  "%{!EB:%{!EL:%(endian_spec)}}",					\
  "%{!mabi=*: -" MULTILIB_ABI_DEFAULT "}"
#else
#define LINUX_DRIVER_SELF_SPECS \
  NO_SHARED_SPECS							\
  MARCH_MTUNE_NATIVE_SPECS,						\
  "%{!EB:%{!EL:%(endian_spec)}}",					\
  "%{!m32:%{!m64: -" MULTILIB_ABI_DEFAULT "}}"
#endif

#undef DRIVER_SELF_SPECS
#define DRIVER_SELF_SPECS \
  MIPS_ISA_LEVEL_SPEC,    \
  BASE_DRIVER_SELF_SPECS, \
  LINUX_DRIVER_SELF_SPECS

/* Similar to standard Linux, but adding -ffast-math support.  */
#undef	GNU_USER_TARGET_MATHFILE_SPEC
#define GNU_USER_TARGET_MATHFILE_SPEC \
  "%{Ofast|ffast-math|funsafe-math-optimizations:crtfastmath.o%s}"
#undef  ENDFILE_SPEC
#define ENDFILE_SPEC \
  GNU_USER_TARGET_MATHFILE_SPEC " " \
  GNU_USER_TARGET_ENDFILE_SPEC

#undef LOCAL_LABEL_PREFIX
#define LOCAL_LABEL_PREFIX (TARGET_OLDABI ? "$" : ".")
