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

#ifndef GCC_FIXED_VALUE_H
#define GCC_FIXED_VALUE_H

#include "machmode.h"

struct fixed_value GTY(())
{
  double_int data;	/* Store data up to 2 wide integers.  */
  unsigned int mode;	/* Use machine mode to know IBIT and FBIT.  */
};

#define FIXED_VALUE_TYPE struct fixed_value

/* Constant fixed-point values 0 and 1.  */
extern FIXED_VALUE_TYPE fconst0[];
extern FIXED_VALUE_TYPE fconst1[];

/* Return a CONST_FIXED with value R and mode M.  */
#define CONST_FIXED_FROM_FIXED_VALUE(r, m) \
  const_fixed_from_fixed_value (r, m)
extern rtx const_fixed_from_fixed_value (FIXED_VALUE_TYPE, enum machine_mode);

/* Initialize from a decimal or hexadecimal string.  */
extern void fixed_from_string (FIXED_VALUE_TYPE *, const char *, tree);

/* In tree.c: wrap up a FIXED_VALUE_TYPE in a tree node.  */
extern tree build_fixed (tree, FIXED_VALUE_TYPE);

/* Extend or truncate to a new mode.  */
extern void fixed_convert (FIXED_VALUE_TYPE *, enum machine_mode,
			   const FIXED_VALUE_TYPE *, int);

/* Compare two fixed-point objects for bitwise identity.  */
extern bool fixed_identical (const FIXED_VALUE_TYPE *, const FIXED_VALUE_TYPE *);

/* Calculate a hash value.  */
extern unsigned int fixed_hash (const FIXED_VALUE_TYPE *);

#define FIXED_VALUES_IDENTICAL(x, y)	fixed_identical (&(x), &(y))

/* Render F as a decimal floating point constant.  */
extern void fixed_to_decimal (char *str, const FIXED_VALUE_TYPE *, size_t);

/* Binary or unary arithmetic on tree_code.  */
extern void fixed_arithmetic (FIXED_VALUE_TYPE *, int, const FIXED_VALUE_TYPE *,
			      const FIXED_VALUE_TYPE *, int);

/* Compare fixed-point values by tree_code.  */
extern bool fixed_compare (int, const FIXED_VALUE_TYPE *,
			   const FIXED_VALUE_TYPE *);

extern FIXED_VALUE_TYPE fixed_arithmetic2 (int, const FIXED_VALUE_TYPE *,
					   const FIXED_VALUE_TYPE *, int);

#define FIXED_VALUE_NEGATE(X, satp) \
  fixed_arithmetic2 (NEGATE_EXPR, &(X), NULL, satp)

#endif /* GCC_FIXED_VALUE_H */
