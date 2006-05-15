/* Wind River GNU/Linux Configuration.
   Copyright (C) 2005
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
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

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
