/*

   Copyright (C) 2007-2008 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#ifndef __STDBOOL_H__
#define __STDBOOL_H__

#include <__cdefs.h>

__BEGIN_EXTERN_C

#if defined __IS_ISO_C99

#  define bool	_Bool
#  define true	1
#  define false	0
#  define __bool_true_false_are_defined	1

#else /* !__IS_ISO_C99 */

#  error  "<stdbool.h> : Sorry : This header shall only be used with a (C99) compiler supporting _Bool types."

#endif /* __IS_ISO_C99 */

__END_EXTERN_C

#endif /*__STDBOOL_H__*/
