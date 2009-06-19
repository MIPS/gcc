/* Declarations and definitions of codes relating to the
   encoding of gimple into the object files.

   Copyright (C) 2009 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

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

#ifndef GCC_LTO_TAGS_H
#define GCC_LTO_TAGS_H

#include "tree.h"
#include "sbitmap.h"
#include "lto-streamer.h"

enum LTO_tags 
{
  LTO_null = 0,

  /* The 1 variant indicates that the basic block is not empty.  */
  LTO_bb0 = 1,
  LTO_bb1,

  /* Variant 1 is used to set region to a nonzero value.  */
  LTO_set_eh0,
  LTO_set_eh1,

  /* All the supported GIMPLE statement codes.  Note that we only
     support Low GIMPLE codes.  */
  LTO_gimple_asm,
  LTO_gimple_assign,
  LTO_gimple_call,
  LTO_gimple_change_dynamic_type,
  LTO_gimple_cond,
  LTO_gimple_goto,
  LTO_gimple_label,
  LTO_gimple_phi,
  LTO_gimple_predict,
  LTO_gimple_return,
  LTO_gimple_resx,
  LTO_gimple_switch,

  /* All of the expression types that we can see.  */
  LTO_abs_expr,
  LTO_addr_expr,
  LTO_align_indirect_ref,
  LTO_array_range_ref,
  LTO_array_ref,
  LTO_asm_expr,
  LTO_assert_expr,
  LTO_bit_and_expr,
  LTO_bit_ior_expr,

  /* Variant 1 is used if both operands 1 and 2 are constant ints.  */
  LTO_bit_field_ref0,
  LTO_bit_field_ref1,
  LTO_bit_not_expr,
  LTO_bit_xor_expr,

  LTO_block,

  /* Call_exprs are terminated by a 0 to indicate the end of the
     parameter list.  Variant 1 indicates the presence of a call
     chain.  */
  LTO_call_expr0,
  LTO_call_expr1,

  /* Variant 1 and 3 are if CASE_LOW exists and variant 2 and 3 are if
     CASE_HIGH exists.  */
  LTO_case_label_expr0,
  LTO_case_label_expr1,
  LTO_case_label_expr2,
  LTO_case_label_expr3,
  LTO_ceil_div_expr,
  LTO_ceil_mod_expr,
  LTO_change_dynamic_type_expr,

  /* Variant 1 if the elements are reals and 0 if the elements are ints.  */
  LTO_complex_cst0,
  LTO_complex_cst1,
  LTO_complex_expr,
  LTO_component_ref,
  LTO_compound_expr,
  LTO_compound_literal_expr,

  LTO_cond_expr,
  LTO_conj_expr,
  LTO_const_decl,

  /* This form is terminated by a zero.  */
  LTO_constructor,
  LTO_convert_expr,
  LTO_decl_expr,
  LTO_dot_prod_expr,
  LTO_eq_expr,
  LTO_exact_div_expr,
  LTO_exc_ptr_expr,
  LTO_field_decl,
  LTO_filter_expr,
  LTO_fixed_convert_expr,
  LTO_fixed_cst,
  LTO_fix_ceil_expr,
  LTO_fix_floor_expr,
  LTO_fix_round_expr,
  LTO_fix_trunc_expr,
  LTO_float_expr,
  LTO_floor_div_expr,
  LTO_floor_mod_expr,

  /* 1 for built-in functions, 0 for regular functions.  */
  LTO_function_decl0,
  LTO_function_decl1,

  LTO_ge_expr,
  LTO_goto_expr,
  LTO_gt_expr,
  LTO_imagpart_expr,
  LTO_imported_decl,
  LTO_identifier_node,
  LTO_indirect_ref,
  LTO_integer_cst,

  /* 1 for DECL_NONLOCAL labels, 0 for local labels.  */
  LTO_label_decl0,
  LTO_label_decl1,

  LTO_label_expr,
  LTO_le_expr,
  LTO_lrotate_expr,
  LTO_lshift_expr,
  LTO_lt_expr,
  LTO_ltgt_expr,
  LTO_max_expr,
  LTO_min_expr,
  LTO_minus_expr,
  LTO_misaligned_indirect_ref,
  LTO_modify_expr,
  LTO_mult_expr,
  LTO_ne_expr,
  LTO_negate_expr,
  LTO_non_lvalue_expr,
  LTO_nop_expr,
  LTO_obj_type_ref,
  LTO_ordered_expr,
  LTO_parm_decl,
  LTO_pointer_plus_expr,
  LTO_paren_expr,
  LTO_plus_expr,
  LTO_predict_expr,
  LTO_range_expr,
  LTO_rdiv_expr,
  LTO_real_cst,
  LTO_realign_load_expr,
  LTO_realpart_expr,
  LTO_reduc_max_expr,
  LTO_reduc_min_expr,
  LTO_reduc_plus_expr,
  LTO_result_decl,

  /* Form "return;"  */
  LTO_return_expr0,

  /* Form "return x;"  */
  LTO_return_expr1,

  LTO_resx_expr,
  LTO_round_div_expr,
  LTO_round_mod_expr,
  LTO_rrotate_expr,
  LTO_rshift_expr,
  LTO_ssa_name,
  LTO_string_cst,

  /* Cases are terminated with a zero.  */
  LTO_switch_expr,
  LTO_tree_list,
  LTO_tree_vec,
  LTO_trunc_div_expr,
  LTO_trunc_mod_expr,
  LTO_truth_and_expr,
  LTO_truth_not_expr,
  LTO_truth_or_expr,
  LTO_truth_xor_expr,
  LTO_type_decl,
  LTO_namespace_decl,
  LTO_translation_unit_decl,
  LTO_uneq_expr,
  LTO_unge_expr,
  LTO_ungt_expr,
  LTO_unle_expr,
  LTO_unlt_expr,
  LTO_unordered_expr,

/* 1 for static or extern and 0 for local.  */
  LTO_var_decl0,
  LTO_var_decl1,
  LTO_vec_cond_expr,
  LTO_vec_lshift_expr,
  LTO_vec_rshift_expr,

/* 1 if the elements are reals and 0 if the elements are ints.  */
  LTO_vector_cst0,
  LTO_vector_cst1,
  LTO_view_convert_expr,
  LTO_widen_mult_expr,
  LTO_widen_sum_expr,
  LTO_with_size_expr,

/* All of the statement types that do not also appear as
   expressions.  */
  LTO_asm_inputs,
  LTO_asm_outputs,
  LTO_asm_clobbers,

  LTO_function,

/* Type reference used in cgraph.  */
  LTO_type,

/* Types */
  LTO_void_type,
  LTO_integer_type,
  LTO_real_type,
  LTO_fixed_point_type,
  LTO_complex_type,
  LTO_boolean_type,
  LTO_offset_type,
  LTO_enumeral_type,
  LTO_pointer_type,
  LTO_reference_type,
  LTO_vector_type,
  LTO_array_type,
  LTO_record_type,
  LTO_union_type,
  LTO_qual_union_type,
  LTO_function_type,
  LTO_method_type,

  LTO_eh_table,

  /* There are 2 variants for each EH region type.  Variant 0 is for
     may_contain_throw == false, variant 1 is for may_contain_throw ==
     true.  */
  LTO_eh_table_cleanup0,
  LTO_eh_table_cleanup1,
  LTO_eh_table_try0,
  LTO_eh_table_try1,
  LTO_eh_table_catch0,
  LTO_eh_table_catch1,
  LTO_eh_table_allowed0,
  LTO_eh_table_allowed1,
  LTO_eh_table_must_not_throw0,
  LTO_eh_table_must_not_throw1,
  LTO_eh_table_throw0,
  LTO_eh_table_throw1,
  LTO_eh_table_shared_region,

/* Base info, e.g., for C++ */
  LTO_tree_binfo,

/* Special for global streamer. Reference to previously-streamed node.  */
  LTO_tree_pickle_reference,

  LTO_type_ref,

/* There are 16 variants of the following decl bodies depending on the
   subtrees that may or may not be there in the decl_common part of
   the tree.
      variant |= DECL_ATTRIBUTES (decl) != NULL_TREE ? 0x01 : 0;
      variant |= DECL_SIZE_UNIT (decl)  != NULL_TREE ? 0x02 : 0;
      variant |= needs_backing_var                   ? 0x04 : 0;
      variant |= ABSTRACT_ORIGIN (decl) != NULL_TREE ? 0x08 : 0;

   These next two tags must have their last hex digit be 0. */
  LTO_local_var_decl_body0 = 0x0C0,
  LTO_parm_decl_body0      = 0x0D0,

/* This is a special literal that must always appear last! */
  LTO_tree_last_tag        = 0x0E0
};

/* This bitmap is indexed by gimple type codes and contains a 1 if the 
   tree type needs to have the type written.  */
extern sbitmap lto_types_needed_for;

/* This bitmap is indexed by gimple type codes and contains a 1 if the 
   tree type needs to have the flags written.  */
extern sbitmap lto_flags_needed_for;

void lto_static_init (void);

/* Indicators for the presence of locus information on expressions and
   symbols.  */
#define LTO_SOURCE_FILE    (1 << 0)
#define LTO_SOURCE_LINE    (1 << 1)
#define LTO_SOURCE_COL     (1 << 2)
#define LTO_SOURCE_HAS_LOC (1 << 3)
#define LTO_SOURCE_LOC_BITS 4

/* The VAR_DECL tree code has more than 32 bits in flags.  On some hosts,
   HOST_WIDE_INT is not wide enough.  */
typedef unsigned HOST_WIDEST_INT 	lto_flags_type;
#define	BITS_PER_LTO_FLAGS_TYPE		HOST_BITS_PER_WIDEST_INT

#if BITS_PER_LTO_FLAGS_TYPE <= 32
#  error "Your host should support integer types wider than 32 bits."
#endif

#endif /* GCC_LTO_TAGS_H  */
