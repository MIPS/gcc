/* Configuration file for ARM GNU/Linux EABI targets.
   Copyright (C) 2004
   Free Software Foundation, Inc.
   Contributed by CodeSourcery, LLC   

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* On EABI GNU/Linux, we want both the BPABI builtins and the
   GNU/Linux builtins.  */
#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS() 		\
  do 						\
    {						\
      TARGET_BPABI_CPP_BUILTINS();		\
      LINUX_TARGET_OS_CPP_BUILTINS();		\
    }						\
  while (false)

/* We default to a soft-float ABI so that binaries can run on all
   target hardware.  */
#undef TARGET_DEFAULT_FLOAT_ABI
#define TARGET_DEFAULT_FLOAT_ABI ARM_FLOAT_ABI_SOFT

#undef SUBTARGET_EXTRA_LINK_SPEC
#define SUBTARGET_EXTRA_LINK_SPEC "-m armelf_linux_eabi"

/* Use ld-linux.so.3 so that it will be possible to run "classic"
   GNU/Linux binaries on an EABI system.  */
#undef LINUX_TARGET_INTERPRETER
#define LINUX_TARGET_INTERPRETER "/lib/ld-linux.so.3"

/* At this point, bpabi.h will have clobbered LINK_SPEC.  We want to
   use the GNU/Linux version, not the generic BPABI version.  */
#undef LINK_SPEC
#define LINK_SPEC LINUX_TARGET_LINK_SPEC

/* Use the default LIBGCC_SPEC, not the version in linux-elf.h, as we
   do not use -lfloat.  */
#undef LIBGCC_SPEC

/* Do not use short enums for the GNU/Linux platform.  */
#define DEFAULT_SHORT_ENUMS 0
