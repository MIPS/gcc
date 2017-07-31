/* Target macros for nanomips*-*-linux* targets.
   Copyright (C) 2012-2016 Free Software Foundation, Inc.

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

/* This target is a multilib target, specify the sysroot paths.
   MIPS_SYSVERSION_SPEC defaults to 'r6' (32r6 or 64r6).  */

#undef MULTILIB_DEFAULTS

#define MIPS_SYSVERSION_SPEC \
    "r6"

#undef SYSROOT_SUFFIX_SPEC
#define SYSROOT_SUFFIX_SPEC						\
    "/nanomips%{meb|EB:eb}-"MIPS_SYSVERSION_SPEC			\
    "%{msoft-float:-soft;:-hard}"					\
    "%{muclibc:-uclibc}"						\
    "%{mmusl:-musl}"

#define SYSROOT_HEADERS_SUFFIX_SPEC SYSROOT_SUFFIX_SPEC

#undef STARTFILE_PREFIX_SPEC
#define STARTFILE_PREFIX_SPEC                      \
  "%{m32: /usr/local/lib/ /lib/ /usr/lib/}         \
   %{m64: /usr/local/lib64/ /lib64/ /usr/lib64/}"

#undef DRIVER_SELF_SPECS
#define DRIVER_SELF_SPECS						\
  /* Set the ISA for the default multilib.  */				\
  MIPS_DEFAULT_ISA_LEVEL_SPEC,						\
									\
  /* Infer the default float setting from -march.  */			\
  MIPS_ARCH_FLOAT_SPEC,							\
									\
  /* Base SPECs.  */							\
  BASE_DRIVER_SELF_SPECS						\
									\
  /* Use the standard linux specs for everything else.  */		\
  LINUX_DRIVER_SELF_SPECS
