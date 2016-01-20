/* Definitions to poison tree macros for ttype. 

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

#ifdef TTYPE_DEVELOPING
#undef TREE_TTYPE

#undef TYPE_HASH
#undef TYPE_ARTIFICIAL
#undef TYPE_REF_CAN_ALIAS_ALL
#undef TYPE_CACHED_VALUES_P
#undef TYPE_SIZES_GIMPLIFIED
#undef TYPE_UNSIGNED
#undef TYPE_SIGN
#undef TYPE_OVERFLOW_WRAPS
#undef TYPE_OVERFLOW_UNDEFINED
#undef TYPE_OVERFLOW_TRAPS
#undef TYPE_OVERFLOW_SANITIZED
#undef TYPE_ALIGN_OK
#undef TYPE_REF_IS_RVALUE
#undef TYPE_REVERSE_STORAGE_ORDER
#undef TYPE_SATURATING
#undef TYPE_UID
#undef TYPE_SIZE
#undef TYPE_SIZE_UNIT
#undef TYPE_POINTER_TO
#undef TYPE_REFERENCE_TO
#undef TYPE_PRECISION
#undef TYPE_NAME
#undef TYPE_NEXT_VARIANT
#undef TYPE_MAIN_VARIANT
#undef TYPE_CONTEXT
#undef TYPE_MODE_RAW
#undef TYPE_MODE
#undef TYPE_CANONICAL
#undef TYPE_STRUCTURAL_EQUALITY_P
#undef TYPE_IBIT
#undef TYPE_FBIT
#undef TYPE_ALIAS_SET
#undef TYPE_ALIAS_SET_KNOWN_P
#undef TYPE_ATTRIBUTES
#undef TYPE_ALIGN
#undef TYPE_USER_ALIGN
#undef TYPE_ALIGN_UNIT
#undef TYPE_STUB_DECL
#undef TYPE_NO_FORCE_BLK
#undef TYPE_VOLATILE
#undef TYPE_ATOMIC
#undef TYPE_READONLY
#undef TYPE_RESTRICT
#undef TYPE_NAMELESS
#undef TYPE_ADDR_SPACE
#undef TYPE_QUALS
#undef TYPE_QUALS_NO_ADDR_SPACE
#undef TYPE_QUALS_NO_ADDR_SPACE_NO_ATOMIC
#undef TYPE_LANG_FLAG_0
#undef TYPE_LANG_FLAG_1
#undef TYPE_LANG_FLAG_2
#undef TYPE_LANG_FLAG_3
#undef TYPE_LANG_FLAG_4
#undef TYPE_LANG_FLAG_5
#undef TYPE_LANG_FLAG_6
#undef TYPE_STRING_FLAG
#undef TYPE_VECTOR_SUBPARTS
#undef TYPE_VECTOR_OPAQUE
#undef TYPE_NEEDS_CONSTRUCTING
#undef TYPE_TRANSPARENT_AGGR
#undef TYPE_NONALIASED_COMPONENT
#undef TYPE_PACKED
#undef TYPE_CONTAINS_PLACEHOLDER_INTERNAL
#undef TYPE_FINAL_P
#undef TYPE_SYMTAB_ADDRESS
#undef TYPE_SYMTAB_POINTER
#undef TYPE_SYMTAB_DIE
#undef TYPE_SYMTAB_IS_ADDRESS 
#undef TYPE_SYMTAB_IS_POINTER 
#undef TYPE_SYMTAB_IS_DIE 
#undef TYPE_LANG_SPECIFIC
#undef TYPE_VALUES
#undef TYPE_DOMAIN
#undef TYPE_FIELDS
#undef TYPE_CACHED_VALUES
#undef TYPE_ARG_TYPES
#undef TYPE_VALUES_RAW
#undef TYPE_METHODS
#undef TYPE_VFIELD
#undef TYPE_METHOD_BASETYPE
#undef TYPE_OFFSET_BASETYPE
#undef TYPE_MAXVAL
#undef TYPE_MINVAL
#undef TYPE_NEXT_PTR_TO
#undef TYPE_NEXT_REF_TO
#undef TYPE_MIN_VALUE
#undef TYPE_MAX_VALUE
#undef TYPE_ARRAY_MAX_SIZE
#undef TYPE_BINFO
#undef TYPE_LANG_SLOT_1
#undef TYPE_IDENTIFIER
#undef TYPE_FILE_SCOPE_P
#undef TYPE_DECL_SUPPRESS_DEBUG

#undef VECTOR_TYPE_P
#undef VECTOR_BOOLEAN_TYPE_P
#undef INTEGRAL_TYPE_P
#undef ANY_INTEGRAL_TYPE_P
#undef NON_SAT_FIXED_POINT_TYPE_P
#undef SAT_FIXED_POINT_TYPE_P
#undef FIXED_POINT_TYPE_P
#undef SCALAR_FLOAT_TYPE_P
#undef COMPLEX_FLOAT_TYPE_P
#undef VECTOR_INTEGER_TYPE_P
#undef VECTOR_FLOAT_TYPE_P
#undef FLOAT_TYPE_P
#undef DECIMAL_FLOAT_TYPE_P
#undef RECORD_OR_UNION_TYPE_P
#undef AGGREGATE_TYPE_P
#undef POINTER_TYPE_P
#undef FUNCTION_POINTER_TYPE_P
#undef COMPLETE_TYPE_P
#undef POINTER_BOUNDS_TYPE_P
#undef BOUNDED_TYPE_P
#undef VOID_TYPE_P
#undef COMPLETE_OR_VOID_TYPE_P
#undef COMPLETE_OR_UNBOUND_ARRAY_TYPE_P
#undef FUNC_OR_METHOD_TYPE_P


#undef TREE_CODE
static inline enum tree_code TREE_CODE (tree n) { return n->u.base.code; }
static inline enum tree_code TREE_CODE (const_tree n) { return n->u.base.code; }
enum tree_code TREE_CODE (ttype *node) __attribute__((error(" Fix use of TREE_CODE(ttype *)")));
enum tree_code TREE_CODE (ttype_p node) __attribute__((error(" Fix use of TREE_CODE(ttype_p)")));

#undef TREE_SET_CODE
static inline void TREE_SET_CODE (tree n, enum tree_code v)
							{ n->u.base.code = v; }
void TREE_SET_CODE (ttype *node, enum tree_code v) __attribute__((error(" Fix use of TREE_CODE(ttype *)")));
void TREE_SET_CODE (ttype_p node, enum tree_code v) __attribute__((error(" Fix use of TREE_CODE(ttype_p)")));



#endif

/* These macros are defined using one of the above, and thus need to be
   redefined in tree.h as they are here once this comes into full production
   and this file is removed. */

#undef VECTOR_CST_NELTS
#define VECTOR_CST_NELTS(NODE)  (TREE_TYPE (NODE)->vector_subparts())
#define DECL_CONTEXT_TYPE_P(NODE) (TYPE_P (DECL_CONTEXT (NODE)))
#define DECL_CONTEXT_TYPE(NODE) (TTYPE (DECL_CONTEXT (NODE)))

#undef TREE_TYPE

static inline ttype *TREE_TYPE (const_tree NODE) 
    { return TTYPE ((CONTAINS_STRUCT_CHECK (NODE, TS_TYPED)->u.typed.type)); }
static inline ttype *TREE_TYPE (tree NODE) 
    { return TTYPE ((CONTAINS_STRUCT_CHECK (NODE, TS_TYPED)->u.typed.type)); }

ttype *TREE_TYPE (ttype *node)  __attribute__((error(" Fix use of TREE_TTYPE(ttype *)")));

ttype *TREE_TYPE (ttype_p node)  __attribute__((error(" Fix use of TREE_TTYPE(ttype_p)")));

#undef TREE_TYPE_PTR
static inline ttype **TREE_TYPE_PTR (tree NODE)
{ 
  return TTYPE_PTR (&(CONTAINS_STRUCT_CHECK (NODE, TS_TYPED)->u.typed.type));
}
ttype **TREE_TYPE_PTR (ttype *node)  __attribute__((error(" Fix use of TREE_TTYPE_PTR(ttype *)")));

ttype **TREE_TYPE_PTR (ttype_p node)  __attribute__((error(" Fix use of TREE_TTYPE_PTR(ttype_p)")));


#endif 


