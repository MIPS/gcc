/* Declarations and definitions of codes relating to the
   encoding of gimple into the object files.

   Copyright (C) 2006, 2007, 2008 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */

#ifndef GCC_LTO_TAGS_H
#define GCC_LTO_TAGS_H

#include "tree.h"
#include "sbitmap.h"
#include "lto-header.h"

/* This file is one header in a collection of files that write the
   gimple intermediate code for a function into the assembly stream
   and read it back.

   This comment describes, in rough terms the methods used to encode
   that gimple stream.  This does not describe gimple itself.

   The encoding for a function consists of 8 (9 in debugging mode),
   sections of information.

   1)    The header.
   2)    FIELD_DECLS.
   3)    FUNCTION_DECLS.
   4)    global VAR_DECLS.
   5)    type_decls
   6)    types.
   7)    Names for the labels that have names
   8)    The SSA names.
   9)    The control flow graph.
   10-11)Gimple for local decls.
   12)   Gimple for the function.
   13)   Strings.
   14)   Redundant information to aid in debugging the stream.
         This is only present if the compiler is built with
         LTO_STREAM_DEBUGGING defined.

   Sections 1-5 are in plain text that can easily be read in the
   assembly file.  Sections 6-8 will be zlib encoded in the future.

   1) THE HEADER.
*/

/* The is the first part of the record for a function or constructor
   in the .o file.  */
struct lto_function_header
{
  struct lto_header lto_header;   /* The header for all types of sections. */
  int32_t num_local_decls;        /* Number of local VAR_DECLS and
				     PARM_DECLS.  */
  int32_t num_named_labels;       /* Number of labels with names.  */
  int32_t num_unnamed_labels;     /* Number of labels without names.  */
  int32_t compressed_size;        /* Size compressed or 0 if not compressed.  */
  int32_t named_label_size;       /* Size of names for named labels.  */
  int32_t ssa_names_size;         /* Size of the SSA_NAMES table.  */
  int32_t cfg_size;               /* Size of the cfg.  */
  int32_t local_decls_index_size; /* Size of local parm and var decl index 
				     region. */
  int32_t local_decls_size;       /* Size of local parm and var decl region. */
  int32_t main_size;              /* Size of main gimple body of function.  */
  int32_t string_size;            /* Size of the string table.  */
  int32_t debug_decl_index_size;  /* Size of local decl index debugging 
				     information.  */
  int32_t debug_decl_size;        /* Size of local decl debugging 
				     information.  */
  int32_t debug_label_size;       /* Size of label stream debugging 
				     information.  */
  int32_t debug_ssa_names_size;   /* Size of ssa_names stream debugging 
				     information.  */
  int32_t debug_cfg_size;         /* Size of cfg stream debugging 
				     information.  */
  int32_t debug_main_size;        /* Size of main stream debugging 
				     information.  */
};

/* 2-6) THE GLOBAL DECLS AND TYPES.

      The global decls and types are encoded in the same way.  For each
      entry, there is a pair of words.  The first is the debugging
      section number and the second is the offset within the section to
      the entry.

      FIXME: This encoding may change change so that the first word is
      a label for the debugging section.  This will cause more work for
      the linker but will make ln -r work properly.

   7) THE LABEL NAMES.  

      Since most labels do not have names, this section my be of zero
      length.  It consists of an array of string table references, one
      per label.  In the lto code, the labels are given either
      positive or negative indexes.  the positive ones have names and
      the negative ones do not.  The positive index can be used to
      find the name in this array.

   8) THE ACTIVE SSA NAMES.  

      The SSA_NAME_VERSION in an SSA_NAME in used as an index into
      this table.

   9) THE CFG. 

   10) Index into the local decls.  Since local decls can have local
      decls inside them, they must be read in randomly in order to
      properly restore them.  

   11-12) GIMPLE FOR THE LOCAL DECLS AND THE FUNCTION BODY.

     The gimple consists of a set of records.

       Each record starts with a "LTO_" tag defined in this file.  The
       mapping between gimple tree codes and LTO code described in
       lto-tree-tags.def along with useful macros that allow the
       simple cases to be processed mechanically.

       For the tree types that can be processed mechanically, the form
       of the output is:
         tag          - The LTO_ tag.

	 type         - If the tree code has a bit set in
                        lto_types_needed_for, a reference to the type
			is generated.  This reference is an index into
                        (7).  The index is encoded in uleb128 form.

	 flags        - The set of flags defined for this tree code
		        packed into a word where the low order 2 bits
		        are used to encode the presence or absense of
		        a file and line number respectively. The word
		        is output in uleb128 form. The encoding and
		        decoding of the flags is controlled by
		        lto-tree-flags.def which is designed so that
		        it is useful on both sides.
			
	 file         - If the file of this gimple record is different
                        from the file of the previous record, the file
                        is encoded into the string table and the
                        offset of that entry in uleb128 is output in
                        the stream.  The flags are ored with 0x2 to
                        indicate the presence of this.

	 line number  - If the line number of this gimple record is
                        different from the line number of the previous
			record, the line number in uleb128 is output
			in the stream.  The flags are ored with 0x1 to
			indicate the presence of this.

	 children     - If this gimple record has children defined,
                        they follow here.  For automaticly generated
			gimple forms, TREE_CODE_LENGTH sepcifies the
                        number of children.

     Many gimple forms cannot be automatically processed.  There are
     two reasons for this: either extra fields must be processed
     beyond those defined in the gimple core or some of the children
     are not always there.

     For these cases, the best reference is the code.  The the
     greatest extent possible, the automatic mechanisms are used to
     processes the pieces of such trees.  But the spec is really the
     code and will change as gimple evolves.

     THE FUNCTION
	
     At the top level of (8) is the function. It consists of five
     pieces:

     LTO_function     - The tag.
     eh tree          - This is all of the exception handling regions
                        put out in a post order traversial of the
                        tree.  Siblings are output as lists terminated
			by a 0.  The set of fields matches the fields
			defined in except.c.

     last_basic_block - in uleb128 form.

     basic blocks     - This is the set of basic blocks.

     zero             - The termination of the basic blocks.

     FIXME: I am sure that there is a lot of random stuff for a
     function that needs to be put out and is not here.  This will be
     fixed if someone steps up and enumerates what is necessary or
     when we really try to make this work.  I do not know where to
     look for everything.

     BASIC BLOCKS

     There are two forms of basic blocks depending on if they are
     empty or not.

     The basic block consists of:

     LTO_bb1 or LTO_bb0 - The tag.

     bb->index          - the index in uleb128 form.

     #succs             - The number of successors un uleb128 form.

     the successors     - For each edge, a pair.  The first of the
                          pair is the index of the successor in
                          uleb128 form and the second are the flags in
                          uleb128 form.

     the statements     - A gimple tree, as described above.
                          These are only present for LTO_BB1.
                          Following each statement is an optional
                          exception handling record LTO_set_eh1 or
                          LTO_set_eh0 if the exception handling for
                          this statement differed from the last region
                          output.  LTO_set_eh0 is a special case that
                          sets the region to 0. LTO_set_eh1 contains
			  the region number in sleb128 form.
			
     zero               - This is only present for LTO_BB1 and is used
			  to terminate the statements and exception
			  regions within this block.

   12) STRINGS

     String are represented in the table as pairs, a length in ULEB128
     form followed by the data for the string.

   13) STREAM DEBUGGING
     
     If the preprocessor symbol LTO_STREAM_DEBUGGING is defined, the
     gimple is encoded into .o file as two streams.  The first stream
     is the normal data stream that is also created when the symbol is
     undefined.  The second stream is a human readable character
     string that describes a trace of the operations used to encode
     the data stream.  This stream is created by calls in the 
     code to LTO_DEBUG_* functions.

     The lto reader uses the same set of functions when it reads the
     data stream.  However, it's version of the lowest level compares
     the debugging stream character by character with the one produced
     by the writer.  When the reader sees a character that is not the
     same one as produced by the writer, it dumps the stream to stderr
     along with a pointer to the offending character.  At this point
     it is easy to see if the bug is in the encoding or the decoding.
*/

/* When we get a strongly typed gimple, this flag should be set to 0
   so we do not waste so much space printing out largely redundant
   type codes.  */
#define REDUNDANT_TYPE_SYSTEM 1

enum LTO_tags {
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
  LTO_field_decl0,
  LTO_field_decl1,
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
  LTO_function_decl,
  LTO_ge_expr,
  LTO_goto_expr,
  LTO_gt_expr,
  LTO_imagpart_expr,
  LTO_imported_decl,
  LTO_identifier_node,
  LTO_indirect_ref,
  LTO_integer_cst,
  LTO_label_decl,
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
  LTO_type_decl0,
  LTO_type_decl1,
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

/* Base info, e.g., for C++ */
  LTO_tree_binfo,

/* Special for global streamer. Reference to previously-streamed node.  */
  LTO_tree_pickle_reference,

  LTO_local_type_ref,
  LTO_global_type_ref,

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

/* The serialization plan is that when any of the current file, line,
   or col change (from the state last serialized), we write the
   changed entity and only that entity into the stream.  We also
   serialize the fact that the current node needs a line number.
   Otherwise we end up putting line numbers on everything.  This takes
   4 bits on every node and are added to the flags that are serialized
   for the node.  

   We waste a bit for the col even though we do not use the col except in
   USE_MAPPED_LOCATION
*/
#define LTO_SOURCE_FILE    0x1
#define LTO_SOURCE_LINE    0x2
#define LTO_SOURCE_COL     0x4
#define LTO_SOURCE_HAS_LOC 0x8
#define LTO_SOURCE_LOC_BITS 4

/* The VAR_DECL tree code has more than 32 bits in flags.  On some hosts,
   HOST_WIDE_INT is not wide enough.  */
typedef unsigned HOST_WIDEST_INT 	lto_flags_type;
#define	BITS_PER_LTO_FLAGS_TYPE		HOST_BITS_PER_WIDEST_INT

#if BITS_PER_LTO_FLAGS_TYPE <= 32
#  error "Your host should support integer types wider than 32 bits."
#endif

#ifdef LTO_STREAM_DEBUGGING
extern const char * LTO_tree_tag_names[LTO_tree_last_tag];
extern void lto_debug_tree_flags (struct lto_debug_context *, enum tree_code, lto_flags_type);
#endif

#endif /* lto-tags.h */
