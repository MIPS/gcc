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


#define TYPE_HASH(NODE) ((NODE)->hash ())
#define TYPE_ARTIFICIAL(NODE)  ((NODE)->artificial_p ())
#define TYPE_REF_CAN_ALIAS_ALL(NODE) ((NODE)->ref_can_alias_all_p ())
#define TYPE_CACHED_VALUES_P(NODE) ((NODE)->cached_values_p ())
#define TYPE_SIZES_GIMPLIFIED(NODE) ((NODE)->sizes_gimplified ())
#define TYPE_UNSIGNED(NODE) ((NODE)->unsigned_p ())
#define TYPE_SIGN(NODE) ((NODE)->sign ())
#define TYPE_OVERFLOW_WRAPS(NODE) ((NODE)->overflow_wraps_p ())
#define TYPE_OVERFLOW_UNDEFINED(NODE) ((NODE)->overflow_undefined_p ())
#define TYPE_OVERFLOW_TRAPS(NODE) ((NODE)->overflow_traps_p ())
#define TYPE_OVERFLOW_SANITIZED(NODE) ((NODE)->overflow_sanitized_p ())
#define TYPE_ALIGN_OK(NODE) ((NODE)->align_ok_p ())
#define TYPE_REF_IS_RVALUE(NODE) ((NODE)->ref_is_rvalue_p ())
#define TYPE_REVERSE_STORAGE_ORDER(NODE) ((NODE)->reverse_storage_order_p ())
#define TYPE_SATURATING(NODE) ((NODE)->saturating_p ())
#define TYPE_UID(NODE) ((NODE)->uid ())
#define TYPE_SIZE(NODE) ((NODE)->size ())
#define TYPE_SIZE_UNIT(NODE) ((NODE)->size_unit ())
#define TYPE_POINTER_TO(NODE) ((NODE)->pointer_to ())
#define TYPE_REFERENCE_TO(NODE) ((NODE)->reference_to ())
#define TYPE_PRECISION(NODE) ((NODE)->precision ())
#define TYPE_NAME(NODE) ((NODE)->name ())
#define TYPE_NEXT_VARIANT(NODE) ((NODE)->next_variant ())
#define TYPE_MAIN_VARIANT(NODE) ((NODE)->main_variant ())
#define TYPE_CONTEXT(NODE) ((NODE)->context ())
#define TYPE_MODE_RAW(NODE) ((NODE)->mode_raw ())
#define TYPE_MODE(NODE) ((NODE)->mode ())
#define TYPE_CANONICAL(NODE) ((NODE)->canonical ())
#define TYPE_STRUCTURAL_EQUALITY_P(NODE) ((NODE)->structural_equality_p ())
#define TYPE_IBIT(NODE) ((NODE)->ibit ())
#define TYPE_FBIT(NODE) ((NODE)->fbit ())
#define TYPE_ALIAS_SET(NODE) ((NODE)->alias_set ())
#define TYPE_ALIAS_SET_KNOWN_P(NODE) ((NODE)->alias_set_known_p ())
#define TYPE_ATTRIBUTES(NODE) ((NODE)->attributes ())
#define TYPE_ALIGN(NODE) ((NODE)->align ())
#define TYPE_USER_ALIGN(NODE) ((NODE)->user_align_p ())
#define TYPE_ALIGN_UNIT(NODE) ((NODE)->align_unit ())
#define TYPE_STUB_DECL(NODE) ((NODE)->stub_decl ())
#define TYPE_NO_FORCE_BLK(NODE) ((NODE)->no_force_blk ())
#define TYPE_VOLATILE(NODE) ((NODE)->volatile_p ())
#define TYPE_ATOMIC(NODE) ((NODE)->atomic_p ())
#define TYPE_READONLY(NODE) ((NODE)->readonly_p ())
#define TYPE_RESTRICT(NODE) ((NODE)->restrict_p ())
#define TYPE_NAMELESS(NODE) ((NODE)->nameless_p ())
#define TYPE_ADDR_SPACE(NODE) ((NODE)->addr_space ())
#define TYPE_QUALS(NODE) ((NODE)->quals ())
#define TYPE_QUALS_NO_ADDR_SPACE(NODE) ((NODE)->quals_no_addr_space ())
#define TYPE_QUALS_NO_ADDR_SPACE_NO_ATOMIC(NODE) ((NODE)->quals_no_addr_space_no_atomic ())
#define TYPE_LANG_FLAG_0(NODE) ((NODE)->lang_flag_0_p ())
#define TYPE_LANG_FLAG_1(NODE) ((NODE)->lang_flag_1_p ())
#define TYPE_LANG_FLAG_2(NODE) ((NODE)->lang_flag_2_p ())
#define TYPE_LANG_FLAG_3(NODE) ((NODE)->lang_flag_3_p ())
#define TYPE_LANG_FLAG_4(NODE) ((NODE)->lang_flag_4_p ())
#define TYPE_LANG_FLAG_5(NODE) ((NODE)->lang_flag_5_p ())
#define TYPE_LANG_FLAG_6(NODE) ((NODE)->lang_flag_6_p ())
#define TYPE_STRING_FLAG(NODE) ((NODE)->string_flag_p ())
#define TYPE_VECTOR_SUBPARTS(NODE) ((NODE)->vector_subparts ())
#define TYPE_VECTOR_OPAQUE(NODE) ((NODE)->vector_opaque_p ())
#define TYPE_NEEDS_CONSTRUCTING(NODE) ((NODE)->needs_constructing_p ())
#define TYPE_TRANSPARENT_AGGR(NODE) ((NODE)->transparent_aggr_p ())
#define TYPE_NONALIASED_COMPONENT(NODE) ((NODE)->nonaliased_component_p ())
#define TYPE_PACKED(NODE) ((NODE)->packed_p ())
#define TYPE_CONTAINS_PLACEHOLDER_INTERNAL(NODE) ((NODE)->contains_placeholder_internal_p ())
#define TYPE_FINAL_P(NODE) ((NODE)->final_p ())
#define TYPE_SYMTAB_ADDRESS(NODE) ((NODE)->symtab_address ())
#define TYPE_SYMTAB_POINTER(NODE) ((NODE)->symtab_pointer ())
#define TYPE_SYMTAB_DIE(NODE) ((NODE)->symtab_die ())
#define TYPE_SYMTAB_IS_ADDRESS (NODE) ((NODE)->symtab_is_address_p ())
#define TYPE_SYMTAB_IS_POINTER (NODE) ((NODE)->symtab_is_pointer_p ())
#define TYPE_SYMTAB_IS_DIE (NODE) ((NODE)->symtab_is_die_p ())
#define TYPE_LANG_SPECIFIC(NODE) ((NODE)->lang_specific ())
#define TYPE_VALUES(NODE) ((NODE)->values ())
#define TYPE_DOMAIN(NODE) ((NODE)->domain ())
#define TYPE_FIELDS(NODE) ((NODE)->fields ())
#define TYPE_CACHED_VALUES(NODE) ((NODE)->cached_values_p ())
#define TYPE_ARG_TYPES(NODE) ((NODE)->arg_types ())
#define TYPE_VALUES_RAW(NODE) ((NODE)->values_raw ())
#define TYPE_METHODS(NODE) ((NODE)->methods ())
#define TYPE_VFIELD(NODE) ((NODE)->vfield ())
#define TYPE_METHOD_BASETYPE(NODE) ((NODE)->method_basetype ())
#define TYPE_OFFSET_BASETYPE(NODE) ((NODE)->offset_basetype ())
#define TYPE_MAXVAL(NODE) ((NODE)->maxval ())
#define TYPE_MINVAL(NODE) ((NODE)->minval ())
#define TYPE_NEXT_PTR_TO(NODE) ((NODE)->next_ptr_to ())
#define TYPE_NEXT_REF_TO(NODE) ((NODE)->next_ref_to ())
#define TYPE_MIN_VALUE(NODE) ((NODE)->min_value ())
#define TYPE_MAX_VALUE(NODE) ((NODE)->max_value ())
#define TYPE_ARRAY_MAX_SIZE(NODE) ((NODE)->array_max_size ())
#define TYPE_BINFO(NODE) ((NODE)->binfo ())
#define TYPE_LANG_SLOT_1(NODE) ((NODE)->lang_slot_1 ())
#define TYPE_IDENTIFIER(NODE) ((NODE)->identifier ())
#define TYPE_FILE_SCOPE_P(NODE) ((NODE)->file_scope_p ())
#define TYPE_DECL_SUPPRESS_DEBUG(NODE) ((NODE)->decl_surpress_debug ())

#define VECTOR_TYPE_P(NODE) ((NODE)->vector_p ())
#define VECTOR_BOOLEAN_TYPE_P(NODE) ((NODE)->vector_boolean_p ())
#define INTEGRAL_TYPE_P(NODE) ((NODE)->integral_p ())
#define ANY_INTEGRAL_TYPE_P(NODE) ((NODE)->any_integral_p ())
#define NON_SAT_FIXED_POINT_TYPE_P(NODE) ((NODE)->non_sat_fixed_point_p ())
#define SAT_FIXED_POINT_TYPE_P(NODE) ((NODE)->sat_fixed_point_p ())
#define FIXED_POINT_TYPE_P(NODE) ((NODE)->fixed_point_p ())
#define SCALAR_FLOAT_TYPE_P(NODE) ((NODE)->scalar_float_p ())
#define COMPLEX_FLOAT_TYPE_P(NODE) ((NODE)->complex_float_p ())
#define VECTOR_INTEGER_TYPE_P(NODE) ((NODE)->vector_integer_p ())
#define VECTOR_FLOAT_TYPE_P(NODE) ((NODE)->vector_float_p ())
#define FLOAT_TYPE_P(NODE) ((NODE)->float_p ())
#define DECIMAL_FLOAT_TYPE_P(NODE) ((NODE)->decimal_float_p ())
#define RECORD_OR_UNION_TYPE_P(NODE) ((NODE)->record_or_union_p ())
#define AGGREGATE_TYPE_P(NODE) ((NODE)->aggregate_p ())
#define POINTER_TYPE_P(NODE) ((NODE)->pointer_p ())
#define FUNCTION_POINTER_TYPE_P(NODE) ((NODE)->function_pointer_p ())
#define COMPLETE_TYPE_P(NODE) ((NODE)->complete_p ())
#define POINTER_BOUNDS_TYPE_P(NODE) ((NODE)->pointer_bounds_p ())
#define BOUNDED_TYPE_P(NODE) ((NODE)->bounded_p ())
#define VOID_TYPE_P(NODE) ((NODE)->void_p ())
#define COMPLETE_OR_VOID_TYPE_P(NODE) ((NODE)->complete_or_void_p ())
#define COMPLETE_OR_UNBOUND_ARRAY_TYPE_P(NODE) ((NODE)->complete_or_bounded_array_p ())
#define FUNC_OR_METHOD_TYPE_P(NODE) ((NODE)->func_or_method_p ())
 

#define TYPE_SET_HASH(NODE, VAL) ((NODE)->set_hash (VAL))
#define TYPE_SET_ARTIFICIAL(NODE, VAL)  ((NODE)->set_artificial_p (VAL))
#define TYPE_SET_REF_CAN_ALIAS_ALL(NODE, VAL) ((NODE)->set_ref_can_alias_all_p (VAL))
#define TYPE_SET_CACHED_VALUES_P(NODE, VAL) ((NODE)->set_cached_values_p (VAL))
#define TYPE_SET_SIZES_GIMPLIFIED(NODE, VAL) ((NODE)->set_sizes_gimplified (VAL))
#define TYPE_SET_UNSIGNED(NODE, VAL) ((NODE)->set_unsigned_p (VAL))
#define TYPE_SET_SIGN(NODE, VAL) ((NODE)->set_sign (VAL))
#define TYPE_SET_OVERFLOW_WRAPS(NODE, VAL) ((NODE)->set_overflow_wraps_p (VAL))
#define TYPE_SET_OVERFLOW_UNDEFINED(NODE, VAL) ((NODE)->set_overflow_undefined_p (VAL))
#define TYPE_SET_OVERFLOW_TRAPS(NODE, VAL) ((NODE)->set_overflow_traps_p (VAL))
#define TYPE_SET_OVERFLOW_SANITIZED(NODE, VAL) ((NODE)->set_overflow_sanitized_p (VAL))
#define TYPE_SET_ALIGN_OK(NODE, VAL) ((NODE)->set_align_ok_p (VAL))
#define TYPE_SET_REF_IS_RVALUE(NODE, VAL) ((NODE)->set_ref_is_rvalue_p (VAL))
#define TYPE_SET_REVERSE_STORAGE_ORDER(NODE, VAL) ((NODE)->set_reverse_storage_order_p (VAL))
#define TYPE_SET_SATURATING(NODE, VAL) ((NODE)->set_saturating_p (VAL))
#define TYPE_SET_UID(NODE, VAL) ((NODE)->set_uid (VAL))
#define TYPE_SET_SIZE(NODE, VAL) ((NODE)->set_size (VAL))
#define TYPE_SET_SIZE_UNIT(NODE, VAL) ((NODE)->set_size_unit (VAL))
#define TYPE_SET_POINTER_TO(NODE, VAL) ((NODE)->set_pointer_to (VAL))
#define TYPE_SET_REFERENCE_TO(NODE, VAL) ((NODE)->set_reference_to (VAL))
#define TYPE_SET_PRECISION(NODE, VAL) ((NODE)->set_precision (VAL))
#define TYPE_SET_NAME(NODE, VAL) ((NODE)->set_name (VAL))
#define TYPE_SET_NEXT_VARIANT(NODE, VAL) ((NODE)->set_next_variant (VAL))
#define TYPE_SET_MAIN_VARIANT(NODE, VAL) ((NODE)->set_main_variant (VAL))
#define TYPE_SET_CONTEXT(NODE, VAL) ((NODE)->set_context (VAL))
#define TYPE_SET_MODE_RAW(NODE, VAL) ((NODE)->set_mode_raw (VAL))
#define TYPE_SET_MODE(NODE, VAL) ((NODE)->set_mode (VAL))
#define TYPE_SET_CANONICAL(NODE, VAL) ((NODE)->set_canonical (VAL))
#define TYPE_SET_STRUCTURAL_EQUALITY_P(NODE, VAL) ((NODE)->set_structural_equality_p (VAL))
#define TYPE_SET_IBIT(NODE, VAL) ((NODE)->set_ibit (VAL))
#define TYPE_SET_FBIT(NODE, VAL) ((NODE)->set_fbit (VAL))
#define TYPE_SET_ALIAS_SET(NODE, VAL) ((NODE)->set_alias_set (VAL))
#define TYPE_SET_ALIAS_SET_KNOWN_P(NODE, VAL) ((NODE)->set_alias_set_known_p (VAL))
#define TYPE_SET_ATTRIBUTES(NODE, VAL) ((NODE)->set_attributes (VAL))
#define TYPE_SET_ALIGN(NODE, VAL) ((NODE)->set_align (VAL))
#define TYPE_SET_USER_ALIGN(NODE, VAL) ((NODE)->set_user_align_p (VAL))
#define TYPE_SET_ALIGN_UNIT(NODE, VAL) ((NODE)->set_align_unit (VAL))
#define TYPE_SET_STUB_DECL(NODE, VAL) ((NODE)->set_stub_decl (VAL))
#define TYPE_SET_NO_FORCE_BLK(NODE, VAL) ((NODE)->set_no_force_blk (VAL))
#define TYPE_SET_VOLATILE(NODE, VAL) ((NODE)->set_volatile_p (VAL))
#define TYPE_SET_ATOMIC(NODE, VAL) ((NODE)->set_atomic_p (VAL))
#define TYPE_SET_READONLY(NODE, VAL) ((NODE)->set_readonly_p (VAL))
#define TYPE_SET_RESTRICT(NODE, VAL) ((NODE)->set_restrict_p (VAL))
#define TYPE_SET_NAMELESS(NODE, VAL) ((NODE)->set_nameless_p (VAL))
#define TYPE_SET_ADDR_SPACE(NODE, VAL) ((NODE)->set_addr_space (VAL))
#define TYPE_SET_QUALS(NODE, VAL) ((NODE)->set_quals (VAL))
#define TYPE_SET_QUALS_NO_ADDR_SPACE(NODE, VAL) ((NODE)->set_quals_no_addr_space (VAL))
#define TYPE_SET_QUALS_NO_ADDR_SPACE_NO_ATOMIC(NODE, VAL) ((NODE)->set_quals_no_addr_space_no_atomic (VAL))
#define TYPE_SET_LANG_FLAG_0(NODE, VAL) ((NODE)->set_lang_flag_0_p (VAL))
#define TYPE_SET_LANG_FLAG_1(NODE, VAL) ((NODE)->set_lang_flag_1_p (VAL))
#define TYPE_SET_LANG_FLAG_2(NODE, VAL) ((NODE)->set_lang_flag_2_p (VAL))
#define TYPE_SET_LANG_FLAG_3(NODE, VAL) ((NODE)->set_lang_flag_3_p (VAL))
#define TYPE_SET_LANG_FLAG_4(NODE, VAL) ((NODE)->set_lang_flag_4_p (VAL))
#define TYPE_SET_LANG_FLAG_5(NODE, VAL) ((NODE)->set_lang_flag_5_p (VAL))
#define TYPE_SET_LANG_FLAG_6(NODE, VAL) ((NODE)->set_lang_flag_6_p (VAL))
#define TYPE_SET_STRING_FLAG(NODE, VAL) ((NODE)->set_string_flag_p (VAL))
#define TYPE_SET_VECTOR_SUBPARTS(NODE, VAL) ((NODE)->set_vector_subparts (VAL))
#define TYPE_SET_VECTOR_OPAQUE(NODE, VAL) ((NODE)->set_vector_opaque_p (VAL))
#define TYPE_SET_NEEDS_CONSTRUCTING(NODE, VAL) ((NODE)->set_needs_constructing_p (VAL))
#define TYPE_SET_TRANSPARENT_AGGR(NODE, VAL) ((NODE)->set_transparent_aggr_p (VAL))
#define TYPE_SET_NONALIASED_COMPONENT(NODE, VAL) ((NODE)->set_nonaliased_component_p (VAL))
#define TYPE_SET_PACKED(NODE, VAL) ((NODE)->set_packed_p (VAL))
#define TYPE_SET_CONTAINS_PLACEHOLDER_INTERNAL(NODE, VAL) ((NODE)->set_contains_placeholder_internal_p (VAL))
#define TYPE_SET_FINAL_P(NODE, VAL) ((NODE)->set_final_p (VAL))
#define TYPE_SET_SYMTAB_ADDRESS(NODE, VAL) ((NODE)->set_symtab_address (VAL))
#define TYPE_SET_SYMTAB_POINTER(NODE, VAL) ((NODE)->set_symtab_pointer (VAL))
#define TYPE_SET_SYMTAB_DIE(NODE, VAL) ((NODE)->set_symtab_die (VAL))
#define TYPE_SET_SYMTAB_IS_ADDRESS (NODE, VAL) ((NODE)->set_symtab_is_address_p (VAL))
#define TYPE_SET_SYMTAB_IS_POINTER (NODE, VAL) ((NODE)->set_symtab_is_pointer_p (VAL))
#define TYPE_SET_SYMTAB_IS_DIE (NODE, VAL) ((NODE)->set_symtab_is_die_p (VAL))
#define TYPE_SET_LANG_SPECIFIC(NODE, VAL) ((NODE)->set_lang_specific (VAL))
#define TYPE_SET_VALUES(NODE, VAL) ((NODE)->set_values (VAL))
#define TYPE_SET_DOMAIN(NODE, VAL) ((NODE)->set_domain (VAL))
#define TYPE_SET_FIELDS(NODE, VAL) ((NODE)->set_fields (VAL))
#define TYPE_SET_CACHED_VALUES(NODE, VAL) ((NODE)->set_cached_values_p (VAL))
#define TYPE_SET_ARG_TYPES(NODE, VAL) ((NODE)->set_arg_types (VAL))
#define TYPE_SET_VALUES_RAW(NODE, VAL) ((NODE)->set_values_raw (VAL))
#define TYPE_SET_METHODS(NODE, VAL) ((NODE)->set_methods (VAL))
#define TYPE_SET_VFIELD(NODE, VAL) ((NODE)->set_vfield (VAL))
#define TYPE_SET_METHOD_BASETYPE(NODE, VAL) ((NODE)->set_method_basetype (VAL))
#define TYPE_SET_OFFSET_BASETYPE(NODE, VAL) ((NODE)->set_offset_basetype (VAL))
#define TYPE_SET_MAXVAL(NODE, VAL) ((NODE)->set_maxval (VAL))
#define TYPE_SET_MINVAL(NODE, VAL) ((NODE)->set_minval (VAL))
#define TYPE_SET_NEXT_PTR_TO(NODE, VAL) ((NODE)->set_next_ptr_to (VAL))
#define TYPE_SET_NEXT_REF_TO(NODE, VAL) ((NODE)->set_next_ref_to (VAL))
#define TYPE_SET_MIN_VALUE(NODE, VAL) ((NODE)->set_min_value (VAL))
#define TYPE_SET_MAX_VALUE(NODE, VAL) ((NODE)->set_max_value (VAL))
#define TYPE_SET_ARRAY_MAX_SIZE(NODE, VAL) ((NODE)->set_array_max_size (VAL))
#define TYPE_SET_BINFO(NODE, VAL) ((NODE)->set_binfo (VAL))
#define TYPE_SET_LANG_SLOT_1(NODE, VAL) ((NODE)->set_lang_slot_1 (VAL))
#define TYPE_SET_IDENTIFIER(NODE, VAL) ((NODE)->set_identifier (VAL))
#define TYPE_SET_FILE_SCOPE_P(NODE, VAL) ((NODE)->set_file_scope_p (VAL))
#define TYPE_SET_DECL_SUPPRESS_DEBUG(NODE, VAL) ((NODE)->set_decl_surpress_debug (VAL))



#define TYPE_HASH_PTR(NODE) ((NODE)->hash_ptr ())
#define TYPE_SIZES_GIMPLIFIED_PTR(NODE) ((NODE)->sizes_gimplified_ptr ())
#define TYPE_SIGN_PTR(NODE) ((NODE)->sign_ptr ())
#define TYPE_UID_PTR(NODE) ((NODE)->uid_ptr ())
#define TYPE_SIZE_PTR(NODE) ((NODE)->size_ptr ())
#define TYPE_SIZE_UNIT_PTR(NODE) ((NODE)->size_unit_ptr ())
#define TYPE_POINTER_TO_PTR(NODE) ((NODE)->pointer_to_ptr ())
#define TYPE_REFERENCE_TO_PTR(NODE) ((NODE)->reference_to_ptr ())
#define TYPE_PRECISION_PTR(NODE) ((NODE)->precision_ptr ())
#define TYPE_NAME_PTR(NODE) ((NODE)->name_ptr ())
#define TYPE_NEXT_VARIANT_PTR(NODE) ((NODE)->next_variant_ptr ())
#define TYPE_MAIN_VARIANT_PTR(NODE) ((NODE)->main_variant_ptr ())
#define TYPE_CONTEXT_PTR(NODE) ((NODE)->context_ptr ())
#define TYPE_MODE_RAW_PTR(NODE) ((NODE)->mode_raw_ptr ())
#define TYPE_MODE_PTR(NODE) ((NODE)->mode_ptr ())
#define TYPE_CANONICAL_PTR(NODE) ((NODE)->canonical_ptr ())
#define TYPE_IBIT_PTR(NODE) ((NODE)->ibit_ptr ())
#define TYPE_FBIT_PTR(NODE) ((NODE)->fbit_ptr ())
#define TYPE_ALIAS_SET_PTR(NODE) ((NODE)->alias_set_ptr ())
#define TYPE_ATTRIBUTES_PTR(NODE) ((NODE)->attributes_ptr ())
#define TYPE_ALIGN_PTR(NODE) ((NODE)->align_ptr ())
#define TYPE_ALIGN_UNIT_PTR(NODE) ((NODE)->align_unit_ptr ())
#define TYPE_STUB_DECL_PTR(NODE) ((NODE)->stub_decl_ptr ())
#define TYPE_NO_FORCE_BLK_PTR(NODE) ((NODE)->no_force_blk_ptr ())
#define TYPE_ADDR_SPACE_PTR(NODE) ((NODE)->addr_space_ptr ())
#define TYPE_VECTOR_SUBPARTS_PTR(NODE) ((NODE)->vector_subparts_ptr ())
#define TYPE_SYMTAB_ADDRESS_PTR(NODE) ((NODE)->symtab_address_ptr ())
#define TYPE_SYMTAB_POINTER_PTR(NODE) ((NODE)->symtab_pointer_ptr ())
#define TYPE_SYMTAB_DIE_PTR(NODE) ((NODE)->symtab_die_ptr ())
#define TYPE_LANG_SPECIFIC_PTR(NODE) ((NODE)->lang_specific_ptr ())
#define TYPE_VALUES_PTR(NODE) ((NODE)->values_ptr ())
#define TYPE_DOMAIN_PTR(NODE) ((NODE)->domain_ptr ())
#define TYPE_FIELDS_PTR(NODE) ((NODE)->fields_ptr ())
#define TYPE_ARG_TYPES_PTR(NODE) ((NODE)->arg_types_ptr ())
#define TYPE_VALUES_RAW_PTR(NODE) ((NODE)->values_raw_ptr ())
#define TYPE_METHODS_PTR(NODE) ((NODE)->methods_ptr ())
#define TYPE_VFIELD_PTR(NODE) ((NODE)->vfield_ptr ())
#define TYPE_METHOD_BASETYPE_PTR(NODE) ((NODE)->method_basetype_ptr ())
#define TYPE_OFFSET_BASETYPE_PTR(NODE) ((NODE)->offset_basetype_ptr ())
#define TYPE_MAXVAL_PTR(NODE) ((NODE)->maxval_ptr ())
#define TYPE_MINVAL_PTR(NODE) ((NODE)->minval_ptr ())
#define TYPE_NEXT_PTR_TO_PTR(NODE) ((NODE)->next_ptr_to_ptr ())
#define TYPE_NEXT_REF_TO_PTR(NODE) ((NODE)->next_ref_to_ptr ())
#define TYPE_MIN_VALUE_PTR(NODE) ((NODE)->min_value_ptr ())
#define TYPE_MAX_VALUE_PTR(NODE) ((NODE)->max_value_ptr ())
#define TYPE_ARRAY_MAX_SIZE_PTR(NODE) ((NODE)->array_max_size_ptr ())
#define TYPE_BINFO_PTR(NODE) ((NODE)->binfo_ptr ())
#define TYPE_LANG_SLOT_1_PTR(NODE) ((NODE)->lang_slot_1_ptr ())
#define TYPE_IDENTIFIER_PTR(NODE) ((NODE)->identifier_ptr ())
#define TYPE_DECL_SUPPRESS_DEBUG_PTR(NODE) ((NODE)->decl_surpress_debug_ptr ())


/* DECL_CONTEXT can be either a tree or a ttype*, so add a checking flag and
   accessor.  */
#define DECL_CONTEXT_TYPE_P(NODE) (TYPE_P (DECL_CONTEXT (NODE)))
#define DECL_CONTEXT_TYPE(NODE) (TTYPE (DECL_CONTEXT (NODE)))

#undef TREE_TYPE
static inline ttype *
TREE_TYPE (const_tree node) 
{
  return reinterpret_cast<ttype *>
	   (CONTAINS_STRUCT_CHECK (node, TS_TYPED)->u.typed.type);
}
static inline ttype *
TREE_TYPE (tree node) 
{
  return reinterpret_cast<ttype *>
	   (CONTAINS_STRUCT_CHECK (node, TS_TYPED)->u.typed.type);
}
static inline ttype *
TREE_TYPE (const ttype *node)
{
  return reinterpret_cast<ttype *>(node->u.typed.type);
}
static inline ttype *
TREE_TYPE (const ttype_p node)
{
  return reinterpret_cast<ttype *>(node->u.typed.type);
}

/* Once a file is converted, the type field should only be settable with a 
   ttype * node.  */
#undef TREE_SET_TYPE
static inline void
TREE_SET_TYPE (tree node, ttype *t)
{
  (CONTAINS_STRUCT_CHECK (node, TS_TYPED)->u.typed.type) = t;
}

#undef TREE_TYPE_PTR
static inline ttype **TREE_TYPE_PTR (tree node)
{ 
  return reinterpret_cast<ttype **>
	   (&(CONTAINS_STRUCT_CHECK (node, TS_TYPED)->u.typed.type));
}

static inline ttype **
TREE_TYPE_PTR (ttype *node)
{
  return node->type_ptr ();
}

static inline ttype **
TREE_TYPE_PTR (ttype_p node)
{
  return node->type_ptr ();
} 

#endif 
