/* Various things used to propagate source location information
   in CIL statements.

   Copyright (C) 2009 Free Software Foundation, Inc.

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
   Thierry Lafage

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
Thierry Lafage          <thierry.lafage@inria.fr>
*/

#ifndef SOURCE_LOCATION_H
#define SOURCE_LOCATION_H

/******************************************************************************
 * Source location inlined functions                                          *
 ******************************************************************************/

/* Return the source location of tree node of any. */

static inline source_location
source_location_from_tree (tree node)
{
  source_location sloc;
  if (EXPR_P (node))
    sloc = EXPR_LOCATION (node);
  else if (DECL_P (node))
    sloc = DECL_SOURCE_LOCATION (node);
  else
    sloc = UNKNOWN_LOCATION;
  return sloc;
}

#endif /* !SOURCE_LOCATION_H */
