/* SourceryG++ GNU/Linux Configuration.
   Copyright (C) 2009.
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

/* Add -te500v1 and -te500v2 options for convenience in generating
   multilibs.  */
#undef CC1_EXTRA_SPEC
#define CC1_EXTRA_SPEC \
  "%{te500v1: -mcpu=8540 -mfloat-gprs=single -mspe=yes -mabi=spe} " \
  "%{te500v2: -mcpu=8548 -mfloat-gprs=double -mspe=yes -mabi=spe} " \
  "%{te600: -mcpu=7400 -maltivec -mabi=altivec}" \
  "%{te500mc: -mcpu=e500mc}" \
  "%{te6500: -mcpu=e6500 -maltivec -mabi=altivec}"

#undef ASM_DEFAULT_SPEC
#define ASM_DEFAULT_SPEC			\
  "%{te500v1:-mppc -mspe -me500 ;		\
     te500v2:-mppc -mspe -me500 ;		\
     te600:-mppc -maltivec ;			\
     te500mc:-me500mc ;				\
     te6500:-me6500 ;				\
     :-mppc%{m64:64}}"

/* The various C libraries each have their own subdirectory.  */
#undef SYSROOT_SUFFIX_SPEC
#define SYSROOT_SUFFIX_SPEC			\
  "%{msoft-float:/nof ;				\
     te600:/te600 ;				\
     te500v1:/te500v1 ;				\
     te500v2:/te500v2 ;				\
     te500mc:/te500mc ;				\
     te6500:/te6500}"

/* MULTILIB_OSDIRNAMES provides directory names used in two ways:
   relative to $target/lib/ in the GCC installation, and relative to
   lib/ and usr/lib/ in a sysroot.  For the latter, we want names such
   as plain ../lib64, but these cannot be used outside the sysroot
   because different multilibs would be mapped to the same directory.
   Directories are searched both with and without the multilib suffix,
   so it suffices if the directory without the suffix is correct
   within the sysroot while the directory with the suffix doesn't
   exist.  We use STARTFILE_PREFIX_SPEC to achieve the desired
   effect.  */
#undef STARTFILE_PREFIX_SPEC
#define STARTFILE_PREFIX_SPEC				\
  "%{m64: /usr/local/lib64/ /lib64/ /usr/lib64/ ;	\
     : /usr/local/lib/ /lib/ /usr/lib/}"
