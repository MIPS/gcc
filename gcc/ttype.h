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

#undef TREE_TYPE
#define TREE_TYPE(NODE) \
      TTYPE ((CONTAINS_STRUCT_CHECK (NODE, TS_TYPED)->u.typed.type))

#ifndef TTYPE_DEVELOPING
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

#endif


#endif 


