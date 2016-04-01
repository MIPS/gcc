/* Definitions for tree types in GCC>
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


inline bool
error_operand_p (const ttype *t)
{
  return ((t == error_type_node) || (t && (TREE_TYPE (t) == error_type_node)));
}


inline struct tree_base&
TREE_BASE (ttype *t)
{
  return t->u.type_common.common.typed.base;
}

inline const struct tree_base&
TREE_BASE (const ttype* t)
{
  return t->u.type_common.common.typed.base;
}

inline tree&
TREE_CHAIN (ttype *t)
{
  return t->u.type_common.common.chain;
}

inline tree
TREE_CHAIN (const ttype *t)
{
  return t->u.type_common.common.chain;
}

#undef TYPE_CHECK
inline ttype *
TYPE_CHECK (ttype *t)
{
  return TTYPE (TREE_CLASS_CHECK (t, tcc_type));
}
inline const ttype *
TYPE_CHECK (const ttype *t)
{
  return TTYPE (TREE_CLASS_CHECK (t, tcc_type));
}



#undef TYPE_CODE_CHECK
inline ttype *
TYPE_CODE_CHECK (ttype *t, enum tree_code c)
{
  return TTYPE (TREE_CHECK(t, c));
}
inline const ttype *
TYPE_CODE_CHECK (const ttype *t, enum tree_code c)
{
  return TTYPE (TREE_CHECK(t, c));
}

#undef RECORD_OR_UNION_CHECK
inline ttype *
RECORD_OR_UNION_CHECK (ttype *t)
{
  return TTYPE (TREE_CHECK3 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE));
}
inline const ttype *
RECORD_OR_UNION_CHECK (const ttype *t)
{
  return TTYPE (TREE_CHECK3 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE));
}

#undef NOT_RECORD_OR_UNION_CHECK
inline ttype *
NOT_RECORD_OR_UNION_CHECK (ttype *t)
{
  return TTYPE (TREE_NOT_CHECK3 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE));
}
inline const ttype *
NOT_RECORD_OR_UNION_CHECK (const ttype *t)
{
  return TTYPE (TREE_NOT_CHECK3 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE));
}

#undef FUNC_OR_METHOD_CHECK
inline ttype *
FUNC_OR_METHOD_CHECK (ttype *t)
{
  return TTYPE (TREE_CHECK2 (t, FUNCTION_TYPE, METHOD_TYPE));
}
inline const ttype *
FUNC_OR_METHOD_CHECK (const ttype *t)
{
  return TTYPE (TREE_CHECK2 (t, FUNCTION_TYPE, METHOD_TYPE));
}
#undef NUMERICAL_TYPE_CHECK
inline ttype *
NUMERICAL_TYPE_CHECK (ttype *t)
{
  return TTYPE (TREE_CHECK5 (t, INTEGER_TYPE, ENUMERAL_TYPE, BOOLEAN_TYPE,
			     REAL_TYPE, FIXED_POINT_TYPE));
}
inline const ttype *
NUMERICAL_TYPE_CHECK (const ttype *t)
{
  return TTYPE (TREE_CHECK5 (t, INTEGER_TYPE, ENUMERAL_TYPE, BOOLEAN_TYPE,
			     REAL_TYPE, FIXED_POINT_TYPE));
}

#undef AGGREGATE_TYPE_CHECK
inline ttype *
AGGREGATE_TYPE_CHECK (ttype *t)
{
  return TTYPE (TREE_CHECK4 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE,
			     ARRAY_TYPE));
}
inline const ttype *
AGGREGATE_TYPE_CHECK (const ttype *t)
{
  return TTYPE (TREE_CHECK4 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE,
			     ARRAY_TYPE));
}

#undef NOT_AGGREGATE_TYPE_CHECK
inline ttype *
NOT_AGGREGATE_TYPE_CHECK (ttype *t)
{
  return TTYPE (TREE_NOT_CHECK4 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE,
				 ARRAY_TYPE));
}
inline const ttype *
NOT_AGGREGATE_TYPE_CHECK (const ttype *t)
{
  return TTYPE (TREE_NOT_CHECK4 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE,
				 ARRAY_TYPE));
}

#undef REVERSE_STORAGE_TYPE_CHECK
inline ttype *
REVERSE_STORAGE_TYPE_CHECK (ttype *t)
{
  return TTYPE (TREE_CHECK4 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE,
			     ARRAY_TYPE));
}
inline const ttype *
REVERSE_STORAGE_TYPE_CHECK (const ttype *t)
{
  return TTYPE (TREE_CHECK4 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE,
			     ARRAY_TYPE));
}

#undef SATURATING_TYPE_CHECK
inline ttype *
SATURATING_TYPE_CHECK (ttype *t)
{
  return TTYPE (TREE_NOT_CHECK4 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE,
				 ARRAY_TYPE));
}
inline const ttype *
SATURATING_TYPE_CHECK (const ttype *t)
{
  return TTYPE (TREE_NOT_CHECK4 (t, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE,
				 ARRAY_TYPE));
}

#undef ANY_INTEGRAL_TYPE_CHECK
#define ANY_INTEGRAL_TYPE_CHECK(T)  \
  (any_integral_ttype_check ((T), __FILE__, __LINE__, __FUNCTION__))
inline ttype *
any_integral_ttype_check (ttype *__t, const char *__f, int __l, const char *__g)
{
  return TTYPE (any_integral_type_check (__t, __f, __l, __g));
}
inline const ttype *
any_integral_ttype_check (const ttype *__t, const char *__f, int __l,
			  const char *__g)
{
  return TTYPE (any_integral_type_check (__t, __f, __l, __g));
}

/* Redefine any of the accessors which are expected to return ttype * rather
   than a tree to use the ttype_ref temporary class which can work as either
   an lvalue or an rvalue.  */

#undef TREE_TYPE
#define TREE_TYPE(NODE) \
  (CONTAINS_STRUCT_CHECK (NODE, TS_TYPED)->u.ttyped.type)

#undef TYPE_CANONICAL
#define TYPE_CANONICAL(NODE) (TYPE_CHECK (NODE)->u.ttype_common.canonical)
#undef TYPE_MAIN_VARIANT
#define TYPE_MAIN_VARIANT(NODE) (TYPE_CHECK (NODE)->u.ttype_common.main_variant)
#undef TYPE_NEXT_VARIANT
#define TYPE_NEXT_VARIANT(NODE) (TYPE_CHECK (NODE)->u.ttype_common.next_variant)
#undef TYPE_POINTER_TO
#define TYPE_POINTER_TO(NODE) (TYPE_CHECK (NODE)->u.ttype_common.pointer_to)
#undef TYPE_NEXT_PTR_TO
#define TYPE_NEXT_PTR_TO(NODE) (POINTER_TYPE_CHECK (NODE)->u.ttype_common.minval)
#undef TYPE_REFERENCE_TO
#define TYPE_REFERENCE_TO(NODE) (TYPE_CHECK (NODE)->u.ttype_common.reference_to)
#undef TYPE_NEXT_REF_TO
#define TYPE_NEXT_REF_TO(NODE) (REFERENCE_TYPE_CHECK (NODE)->u.ttype_common.minval)
#undef TYPE_DOMAIN
#define TYPE_DOMAIN(NODE) (ARRAY_TYPE_CHECK (NODE)->u.ttype_common.values)
#undef TYPE_METHOD_BASETYPE
#define TYPE_METHOD_BASETYPE(NODE) (FUNC_OR_METHOD_CHECK (NODE)->u.ttype_common.maxval)
#undef TYPE_OFFSET_BASETYPE
#define TYPE_OFFSET_BASETYPE(NODE) (OFFSET_TYPE_CHECK (NODE)->u.ttype_common.maxval)


/* DECL_CONTEXT can be either a tree or a ttype*, so add a checking flag and
   accessor.  */
#undef DECL_CONTEXT_TYPE
#define DECL_CONTEXT_TYPE(NODE) (DECL_MINIMAL_CHECK (NODE)->u.decl_minimal_ttype.context)



/* Redefine DECL_ORIGINAL_TYPE to return a ttype *.  */
#undef DECL_ORIGINAL_TYPE
#define DECL_ORIGINAL_TYPE(NODE) \
  (TYPE_DECL_CHECK (NODE)->u.decl_non_common_ttype.result)
/* And provide a SET routine.  */

#undef DECL_BIT_FIELD_TYPE
#define DECL_BIT_FIELD_TYPE(NODE) \
  (FIELD_DECL_CHECK (NODE)->u.field_decl_ttype.bit_field_type)

#undef DECL_ARG_TYPE
#define DECL_ARG_TYPE(NODE) (PARM_DECL_CHECK (NODE)->u.decl_common_ttype.initial)

/* ------------------------------------------------------------------------ */
/* The remaining routines are simply macors that are redefined to be inline
   functions which should only be called on types, so do the typechecking.  */
/* ------------------------------------------------------------------------ */

#undef POINTER_TYPE_P
inline bool
POINTER_TYPE_P (const ttype *t)
{
  enum tree_code code = TREE_CODE (t);
  return (code == POINTER_TYPE || code == REFERENCE_TYPE);
}

#undef FUNC_OR_METHOD_TYPE_P
inline bool
FUNC_OR_METHOD_TYPE_P (const ttype *t)
{
  enum tree_code code = TREE_CODE (t);
  return (code == FUNCTION_TYPE || code == METHOD_TYPE);
}

#undef RECORD_OR_UNION_TYPE_P
inline bool
RECORD_OR_UNION_TYPE_P (const ttype *t)
{
  enum tree_code code = TREE_CODE (t);
  return (code == RECORD_TYPE || code == UNION_TYPE || code == QUAL_UNION_TYPE);
}

#undef SCALAR_FLOAT_TYPE_P
inline bool
SCALAR_FLOAT_TYPE_P (const ttype *t)
{
  return (TREE_CODE (t) == REAL_TYPE);
}

#undef FIXED_POINT_TYPE_P
inline bool
FIXED_POINT_TYPE_P (const ttype *t)
{
  return (TREE_CODE (t) == FIXED_POINT_TYPE);
}

#undef VECTOR_TYPE_P
inline bool
VECTOR_TYPE_P (const ttype *t)
{
  return (TREE_CODE (t) == VECTOR_TYPE);
}

#undef INTEGRAL_TYPE_P
inline bool
INTEGRAL_TYPE_P (const ttype *t)
{
  enum tree_code code = TREE_CODE (t);
  return (code == ENUMERAL_TYPE || code == BOOLEAN_TYPE
	  || code ==  INTEGER_TYPE);
}

#undef COMPLEX_FLOAT_TYPE_P
inline bool
COMPLEX_FLOAT_TYPE_P (const ttype *t)
{
  return (TREE_CODE (t) == COMPLEX_TYPE && SCALAR_FLOAT_TYPE_P (TREE_TYPE (t)));
}

#undef POINTER_BOUNDS_TYPE_P
inline bool
POINTER_BOUNDS_TYPE_P (const ttype *t)
{
  return (TREE_CODE (t) == POINTER_BOUNDS_TYPE);
}

#undef VOID_TYPE_P
inline bool
VOID_TYPE_P (const ttype *t)
{
  return (TREE_CODE (t) == VOID_TYPE);
}

#undef VECTOR_BOOLEAN_TYPE_P
inline bool
VECTOR_BOOLEAN_TYPE_P (const ttype *t)
{
  return (TREE_CODE (t) == VECTOR_TYPE
	  && TREE_CODE (TREE_TYPE (t)) == BOOLEAN_TYPE);
}
#endif 
