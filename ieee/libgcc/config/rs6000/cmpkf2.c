/* Software IEEE 128-bit floating-point emulation for PowerPC.
   Copyright (C) 2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Michael Meissner (meissner@linux.vnet.ibm.com)
   Code is based on the main soft-fp library written by:
	Richard Henderson (rth@cygnus.com) and
	Jakub Jelinek (jj@ultra.linux.cz).

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   In addition to the permissions in the GNU Lesser General Public
   License, the Free Software Foundation gives you unlimited
   permission to link the compiled version of this file into
   combinations with other programs, and to distribute those
   combinations without any restriction coming from the use of this
   file.  (The Lesser General Public License restrictions do apply in
   other respects; for example, they cover modification of the file,
   and distribution when not linked into a combine executable.)

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "soft-fp.h"
#include "double.h"
#include "single.h"
#include "quad.h"

/* PowerPC condition register bits.  */
#define PPC_UNORDERED		0x1		/* isnan (a) || isnan (b).  */
#define PPC_EQUAL		0x2		/* a == b.  */
#define PPC_GREATER_THEN	0x4		/* a > b.  */
#define PPC_LESS_THEN		0x8		/* a < b.  */

/* Map FP_CMP_Q output to PowerPC condition register bits.  */
#define CMP_UNORDERED		(-2)		/* isnan (a) || isnan (b).  */
#define CMP_LESS_THEN		(-1)		/* a < b.  */
#define CMP_EQUAL		0		/* a == b.  */
#define CMP_GREATER_THEN	1		/* a < b.  */
#define CMP_INVALID		2		/* raise invalid exception.  */

#define CMP_LOW			CMP_UNORDERED	/* comparison low value.  */
#define CMP_HIGH		CMP_INVALID	/* comparison high value.  */

static const unsigned char ppc_cr_map[] = {
  PPC_UNORDERED,				/* -2: unordered.  */
  PPC_LESS_THEN,				/* -1: a < b.  */
  PPC_EQUAL,					/*  0: a == b.  */
  PPC_GREATER_THEN,				/*  1: a > b.  */
  PPC_UNORDERED,				/*  2: invalid.  */
};

/* Compare two IEEE 128-bit floating point values and return the status.  We
   return the status as a 4-bit value that can be copied into an appropriate
   PowerPC conditon code register.  */

CMPtype
__cmpkf2 (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r < CMP_LOW || r > CMP_HIGH) ? PPC_UNORDERED : ppc_cr_map[r-CMP_LOW];
}
