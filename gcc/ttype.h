/* Definitions to poison tree macros for ttype. 
   Copyright (C) 2016 Free Software Foundation, Inc.
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

#ifndef GCC_TTYPE_H
#define GCC_TTYPE_H


/* Change the default for the type field in the ttype_field union.  */
#undef TTYPE_FIELD
#define TTYPE_FIELD  _type

/* Redfine TREE_TYPE as an inline function to return a ttype * for various
   types of parameters. */
#undef TREE_TYPE
static inline ttype *
TREE_TYPE (const_tree node) 
{
  return CONTAINS_STRUCT_CHECK (node, TS_TYPED)->u.typed.type._type;
}

static inline ttype *&
TREE_TYPE (tree node) 
{
  return CONTAINS_STRUCT_CHECK (node, TS_TYPED)->u.typed.type._type;
}

static inline ttype *
TREE_TYPE (const ttype *node)
{
  return node->u.typed.type._type;
}
static inline ttype *&
TREE_TYPE (ttype *node)
{
  return node->u.typed.type._type;
}


/* DECL_CONTEXT can be either a tree or a ttype*, so add a checking flag and
   accessor.  */
#define DECL_CONTEXT_TYPE_P(NODE) (TYPE_P (DECL_CONTEXT (NODE)))
#define DECL_CONTEXT_TYPE(NODE) (TTYPE (DECL_CONTEXT (NODE)))

/* Redefine DECL_ORIGINAL_TYPE to return a ttype *.  */
#undef DECL_ORIGINAL_TYPE
#define DECL_ORIGINAL_TYPE(NODE) \
  TTYPE ((TYPE_DECL_CHECK (NODE)->u.decl_non_common.result))
/* And provide a SET routine.  */
static inline void
SET_DECL_ORIGINAL_TYPE(tree node, ttype *val)
{
  TYPE_DECL_CHECK (node)->u.decl_non_common.result = val;
}

#endif 
