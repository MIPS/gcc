/* Type related function prototypes and declarations.

   Copyright (C) 2006-2008 Free Software Foundation, Inc.

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
02110-1301, USA.

Authors:
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Erven Rohou             <erven.rohou@st.com>
*/

#ifndef CIL_TYPES_H
#define CIL_TYPES_H

#include "coretypes.h"

/******************************************************************************
 * Misc types                                                                 *
 ******************************************************************************/

/* Referenced string entry.  */

struct str_ref_d GTY(())
{
  /* STRING_CST node.  */
  tree cst;

  /* Unique ID.  */
  size_t id;
};

typedef struct str_ref_d *str_ref;

/* Mapping for label's whose addresses have been taken.  */

struct label_addr_d GTY(())
{
  /* LABEL_DECL node.  */
  tree label;

  /* Unique id as an INTEGER_CST of type intSI_type_node.  */
  tree id;
};

typedef struct label_addr_d *label_addr;

#endif /* !CIL_TYPES_H */
