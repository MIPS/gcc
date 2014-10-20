/* Copyright (C) 2013,14 Bluwireless Technologies Ltd */
/* Target macros for mips*-elf targets.
   Copyright (C) 1994, 1997, 1999, 2000, 2002, 2003, 2004, 2007, 2010
   Free Software Foundation, Inc.

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

       /* Bluwireless Technologies Ltd GCC customisations */

/* A definition of a compiler setup suitable for Bluwireless devices */

      /* default runtime */
#undef  LIB_SPEC
#define LIB_SPEC  "-T bwt.ld -lbwt" 

#undef ENDFILE_SPEC
#define ENDFILE_SPEC ""

#undef STARTFILE_SPEC
#define STARTFILE_SPEC ""

      /* default predefined macro definitions */

#define TARGET_OS_CPP_BUILTINS()                            \
    do {                                                    \
          builtin_define ("__bwt__");                       \
          builtin_assert ("system=bwt");                    \
        if (TARGET_BWT_INTRINSICS) {                      \
                builtin_define ("__bwt_intrinsics__");      \
              if (bwt_model_var == BWT_INTRINSICS_I2)     \
                    builtin_define ("__bwt_intrinsics_i2__"); \
         }                                                  \
} while (0)

      /* cpu specific default parameterisations */

#undef SUBTARGET_ASM_SPEC
#define SUBTARGET_ASM_SPEC " %{!march:-march=m14k}         \
                             %{!mmicromips:-mno-micromips} \
                                %{!mhard-float:-msoft-float}"

#undef SUBTARGET_CC1_SPEC
#define SUBTARGET_CC1_SPEC " %{!march:-march=m14k}         \
                             %{!mmicromips:-mno-micromips} \
                             %{!mgpopt:-mno-gpopt}         \
                                %{!mhard-float:-msoft-float}"

#undef STARTFILE_SPE
#define STARTFILE_SPEC ""

#undef ENDFILE_SPE
#define ENDFILE_SPEC ""

#undef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS \
  { "mbwt=", MULTILIB_ENDIAN_DEFAULT, MULTILIB_ISA_DEFAULT, \
    MULTILIB_ABI_DEFAULT }

