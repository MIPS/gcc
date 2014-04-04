/* Specialized bits of code needed for the OpenMP offloading tables.
   Copyright (C) 2014 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

/* Target machine header files require this define. */
#define IN_LIBGCC2

/* FIXME: Including auto-host is incorrect, but until we have
   identified the set of defines that need to go into auto-target.h,
   this will have to do.  */
#include "auto-host.h"
#undef pid_t
#undef rlim_t
#undef ssize_t
#undef vfork
#include "tconfig.h"
#include "tsystem.h"
#include "coretypes.h"
#include "tm.h"
#include "libgcc_tm.h"

#if defined(HAVE_GAS_HIDDEN) && defined(ENABLE_OFFLOADING)
extern void __start___gnu_offload_funcs;
extern void __stop___gnu_offload_funcs;
extern void __start___gnu_offload_vars;
extern void __stop___gnu_offload_vars;
void *__OPENMP_TARGET__[] __attribute__ ((__visibility__ ("hidden"))) =
{
  &__start___gnu_offload_funcs, &__stop___gnu_offload_funcs,
  &__start___gnu_offload_vars, &__stop___gnu_offload_vars
};
#endif
