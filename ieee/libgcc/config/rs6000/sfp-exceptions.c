/*
 * Copyright (C) 2015 Free Software Foundation, Inc.
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3, or (at your option) any
 * later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * Under Section 7 of GPL version 3, you are granted additional
 * permissions described in the GCC Runtime Library Exception, version
 * 3.1, as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License and
 * a copy of the GCC Runtime Library Exception along with this program;
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "sfp-machine.h"

void
__sfp_handle_exceptions (int _fex)
{
  const double fp_max = __DBL_MAX__;
  const double fp_min = __DBL_MIN__;
  const double fp_zero = (double) 0.0;
  const double fp_one = 1.0;

  if (_fex & FP_EX_INVALID)
    {
      __asm__ __volatile__ ("fdiv %0, %0, %0"
			    :
			    : "f" (fp_zero));
    }
  if (_fex & FP_EX_DIVZERO)
    {
      __asm__ __volatile__ ("fdiv %0, %0, %1"
			    :
			    : "f" (fp_one), "f" (fp_zero));
    }
  if (_fex & FP_EX_OVERFLOW)
    {
      __asm__ __volatile__ ("fadd %0, %0, %0"
			    :
			    : "f" (fp_max));
    }
  if (_fex & FP_EX_UNDERFLOW)
    {
      __asm__ __volatile__ ("fmul %0, %0, %0"
			    :
			    : "f" (fp_min));
    }
  if (_fex & FP_EX_INEXACT)
    {
      __asm__ __volatile__ ("fsub %0, %0, %1"
			    :
			    : "f" (fp_max), "f" (fp_one));
    }
}
