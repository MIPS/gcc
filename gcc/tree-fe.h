/* Definitions for the accesing the front end specific aspects of the
   'tree' data structures for GNU compiler Front ends.
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

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_TREE_FE_H
#define GCC_TREE_FE_H

#include "tree.h"

#undef DECL_STRUCT_FUNCTION

/* For FUNCTION_DECL, this holds a pointer to a structure ("struct function")
   that describes the status of this function.  */
#define DECL_STRUCT_FUNCTION(NODE)		\
  fe_function_ptr ((FUNCTION_DECL_CHECK (NODE)->function_decl.f))
#define SET_DECL_STRUCT_FUNCTION(NODE,V)	\
  ((FUNCTION_DECL_CHECK (NODE)->function_decl.f) = V)


#endif  /* GCC_TREE_FE_H  */
