/* Wind River GNU/Linux Configuration.
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

/* Add a -te500v2 option for convenience in generating multilibs.  */
#undef CC1_EXTRA_SPEC
#define CC1_EXTRA_SPEC "%{te500v2: -mcpu=8548 -mfloat-gprs=double -mspe=yes -mabi=spe}"

#undef ASM_DEFAULT_SPEC
#define ASM_DEFAULT_SPEC			\
  "%{te500v2:-mppc -mspe -me500 ;		\
     :-mppc}"

/* The various C libraries each have their own subdirectory.  */
#undef SYSROOT_SUFFIX_SPEC
#define SYSROOT_SUFFIX_SPEC			\
  "%{msoft-float:/soft-float ;			\
     te500v2:/te500v2}"

#undef TARGET_SPE_ABI
#undef TARGET_SPE
#undef TARGET_E500
#undef TARGET_ISEL
#undef TARGET_FPRS
#undef TARGET_E500_SINGLE
#undef TARGET_E500_DOUBLE

#define TARGET_SPE_ABI rs6000_spe_abi
#define TARGET_SPE rs6000_spe
#define TARGET_E500 (rs6000_cpu == PROCESSOR_PPC8540)
#define TARGET_ISEL rs6000_isel
#define TARGET_FPRS (rs6000_float_gprs == 0)
#define TARGET_E500_SINGLE (TARGET_HARD_FLOAT && rs6000_float_gprs == 1)
#define TARGET_E500_DOUBLE (TARGET_HARD_FLOAT && rs6000_float_gprs == 2)
