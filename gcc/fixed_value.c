/* Fixed-point arithmetic support.
   Copyright (C) 2006 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "toplev.h"
#include "fixed_value.h"
#include "real.h"

/* Compare two fixed objects for bitwise identity.  */

bool
fixed_identical (const FIXED_VALUE_TYPE *a, const FIXED_VALUE_TYPE *b)
{
  return a->mode == b->mode && a->data.high == b->data.high
	 && a->data.low == b->data.low;
}

/* Calculate a hash value.  */

unsigned int
fixed_hash (const FIXED_VALUE_TYPE *f)
{
  return (unsigned int) (f->data.low ^ f->data.high);
}

/* Initialize from a decimal or hexadecimal string.  */

void
fixed_from_string (FIXED_VALUE_TYPE *f, const char *str, tree type)
{
  REAL_VALUE_TYPE real_value, fixed_value, base_value;
  char base_string[10];
  unsigned int fbit;

  f->mode = TYPE_MODE (type);
  fbit = TYPE_FBIT (type);

  sprintf (base_string, "0x1.0p%d", fbit);
  real_from_string (&real_value, str);
  real_from_string (&base_value, base_string);
  real_arithmetic (&fixed_value, MULT_EXPR, &real_value, &base_value);
  real_to_integer2 (&f->data.low, &f->data.high,  &fixed_value);
}
