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



/* REdefine the TYPE_ accessor macros to be inloine functions which only
   accept ttype * fields.  THis means tree is no longer an acceped parameter
   for any TYPE_ field accesor.  */

#undef tree_type_node
#undef func_or_method_type_node
#undef ptr_or_ref_type_node
#undef record_or_union_type_node
#undef not_record_or_union_type_node
#undef numerical_type_node
#undef aggregate_type_node
#undef not_aggregate_type_node
#undef any_integral_type_node
#undef enumeral_type_node
#undef array_type_node
#undef offset_type_node
#undef pointer_type_node
#undef reference_type_node
#undef vector_type_node


inline struct tree_type&
tree_type_node (ttype *t)
{
  return t->u.type_common;
}
inline const struct tree_type&
tree_type_node (const ttype *t)
{
  return t->u.type_common;
}

#define TYPE_BASE(NAME, CHECK)		\
inline struct tree_type&		\
NAME (ttype *t)				\
{					\
  return CHECK(t)->u.type_common;	\
}					\
inline const struct tree_type&		\
NAME (const ttype *t)			\
{					\
  return CHECK(t)->u.type_common;	\
}

TYPE_BASE (func_or_method_type_node, FUNC_OR_METHOD_CHECK)
TYPE_BASE (ptr_or_ref_type_node,  PTR_OR_REF_CHECK)
TYPE_BASE (record_or_union_type_node, RECORD_OR_UNION_CHECK)
TYPE_BASE (not_record_or_union_type_node, NOT_RECORD_OR_UNION_CHECK)
TYPE_BASE (numerical_type_node, NUMERICAL_TYPE_CHECK)
TYPE_BASE (aggregate_type_node, AGGREGATE_TYPE_CHECK)
TYPE_BASE (not_aggregate_type_node, NOT_AGGREGATE_TYPE_CHECK)
TYPE_BASE (any_integral_type_node, ANY_INTEGRAL_TYPE_CHECK)
TYPE_BASE (enumeral_type_node, ENUMERAL_TYPE_CHECK)
TYPE_BASE (array_type_node, ARRAY_TYPE_CHECK)
TYPE_BASE (offset_type_node, OFFSET_TYPE_CHECK)
TYPE_BASE (pointer_type_node, POINTER_TYPE_CHECK)
TYPE_BASE (reference_type_node, REFERENCE_TYPE_CHECK)
TYPE_BASE (vector_type_node, VECTOR_TYPE_CHECK)



/* Redefine any of the accessors which are expected to return ttype * rather
   than a tree to use the ttype_ref temporary class which can work as either
   an lvalue or an rvalue.  */

#undef TREE_TYPE
#define TREE_TYPE(NODE) \
  (ttype_ref (&(CONTAINS_STRUCT_CHECK (NODE, TS_TYPED)->u.typed.type)))

#undef TYPE_CANONICAL
#define TYPE_CANONICAL(NODE) (ttype_ref (&(tree_type_node (NODE).canonical)))
#undef TYPE_MAIN_VARIANT
#define TYPE_MAIN_VARIANT(NODE) (ttype_ref (&(tree_type_node (NODE).main_variant)))
#undef TYPE_NEXT_VARIANT
#define TYPE_NEXT_VARIANT(NODE) (ttype_ref (&(tree_type_node (NODE).next_variant)))
#undef TYPE_POINTER_TO
#define TYPE_POINTER_TO(NODE) (ttype_ref (&(tree_type_node (NODE).pointer_to)))
#undef TYPE_NEXT_PTR_TO
#define TYPE_NEXT_PTR_TO(NODE) (ttype_ref (&(pointer_type_node (NODE).minval)))
#undef TYPE_REFERENCE_TO
#define TYPE_REFERENCE_TO(NODE) (ttype_ref (&(tree_type_node (NODE).reference_to)))
#undef TYPE_NEXT_REF_TO
#define TYPE_NEXT_REF_TO(NODE) (ttype_ref (&(reference_type_node (NODE).minval)))
#undef TYPE_DOMAIN
#define TYPE_DOMAIN(NODE) (ttype_ref (&(array_type_node (NODE).values)))
#undef TYPE_METHOD_BASETYPE
#define TYPE_METHOD_BASETYPE(NODE) (ttype_ref (&(func_or_method_type_node (NODE).maxval)))
#undef TYPE_OFFSET_BASETYPE
#define TYPE_OFFSET_BASETYPE(NODE) (ttype_ref (&(offset_type_node (NODE).maxval)))


/* DECL_CONTEXT can be either a tree or a ttype*, so add a checking flag and
   accessor.  */
#define DECL_CONTEXT_TYPE_P(NODE) (TYPE_P (DECL_CONTEXT (NODE)))
#define DECL_CONTEXT_TYPE(NODE) (ttype_ref (&(DECL_CONTEXT (NODE))))



/* Redefine DECL_ORIGINAL_TYPE to return a ttype *.  */
#undef DECL_ORIGINAL_TYPE
#define DECL_ORIGINAL_TYPE(NODE) \
  (ttype_ref (&(TYPE_DECL_CHECK (NODE)->u.decl_non_common.result)))
/* And provide a SET routine.  */




#endif 
