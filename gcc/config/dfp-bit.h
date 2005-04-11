/* Header file for dfp-bit.c.
   Copyright (C) 2005 Free Software Foundation, Inc.

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

#ifndef _DFPBIT_H
#define _DFPBIT_H

#include "decNumber.h"

#if WIDTH == 32
#include "decimal32.h"
#define DFP_TYPE	decimal32
#define TO_INTERNAL	decimal32ToNumber
#define TO_ENCODED	decimal32FromNumber
#elif WIDTH == 64
#include "decimal64.h"
#define DFP_TYPE	decimal64
#define TO_INTERNAL	decimal64ToNumber
#define TO_ENCODED	decimal64FromNumber
#elif WIDTH == 128
#include "decimal128.h"
#define DFP_TYPE	decimal128
#define TO_INTERNAL	decimal128ToNumber
#define TO_ENCODED	decimal128FromNumber
#else
#error invalid decimal float width
#endif

/* Multiplication.  */
#if defined(L_mul_sd)
#define DFP_MULTIPLY	dfp_multiply32
#elif defined(L_mul_dd)
#define DFP_MULTIPLY	dfp_multiply46
#elif defined(L_mul_td)
#define DFP_MULTIPLY	dfp_multiply128
#endif

/* Prototypes.  */

#if defined(L_mul_sd) || defined(L_mul_dd) || defined(L_mul_td)
extern DFP_TYPE DFP_MULTIPLY (DFP_TYPE, DFP_TYPE);
#endif

#endif /* DFPBIT_H */
