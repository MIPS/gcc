/* Type declarations and prototypes of the CIL statements.

   Copyright (C) 2006-2009 Free Software Foundation, Inc.

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

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

#ifndef CIL_STMT_H
#define CIL_STMT_H

#include "input.h"
#include "tree.h"
#include "tree-flow.h"
#include "cil-types.h"

/******************************************************************************
 * CIL statement interface                                                    *
 ******************************************************************************/

extern cil_stmt cil_build_stmt (enum cil_opcode);
extern cil_stmt cil_build_stmt_arg (enum cil_opcode, tree);
extern cil_stmt cil_build_switch (tree);
extern cil_stmt cil_build_call_generic (enum cil_opcode, tree);
extern cil_stmt cil_build_call_generic_list (enum cil_opcode, bool,
					     tree, VEC (tree, heap) *);
extern cil_stmt cil_copy_stmt (const_cil_stmt);
extern bool cil_prefix_tail (const_cil_stmt);
extern void cil_set_prefix_tail (cil_stmt, bool);
extern int cil_prefix_unaligned (const_cil_stmt);
extern void cil_set_prefix_unaligned (cil_stmt, int);
extern bool cil_prefix_volatile (const_cil_stmt);
extern void cil_set_prefix_volatile (cil_stmt, bool);
extern bool cil_short_ctor (const_cil_stmt);
extern void cil_set_short_ctor (cil_stmt, bool);
extern size_t cil_call_nargs_base (const_cil_stmt);
extern HOST_WIDE_INT cil_switch_case_low (const_cil_stmt, size_t);
extern HOST_WIDE_INT cil_switch_case_high (const_cil_stmt, size_t);

static inline enum cil_opcode cil_opcode (const_cil_stmt);
static inline basic_block cil_bb (const_cil_stmt);
static inline void cil_set_bb (cil_stmt, basic_block);
static inline location_t cil_locus (const_cil_stmt);
static inline void cil_set_locus (cil_stmt, location_t);
static inline tree cil_var (const_cil_stmt);
static inline tree cil_type (const_cil_stmt);
static inline tree cil_field (const_cil_stmt);
static inline tree cil_label (const_cil_stmt);
static inline size_t cil_switch_ncases (const_cil_stmt);
static inline tree cil_switch_case (const_cil_stmt, size_t);
static inline tree cil_switch_default (const_cil_stmt);
static inline tree cil_switch_case_label (const_cil_stmt, size_t);
static inline tree cil_func (const_cil_stmt);
static inline cil_stmt cil_build_call (tree);
static inline cil_stmt cil_build_calli (tree);
static inline cil_stmt cil_build_jmp (tree);
static inline cil_stmt cil_build_call_va (tree, VEC (tree, heap) *);
static inline cil_stmt cil_build_calli_va (tree, VEC (tree, heap) *);
static inline cil_stmt cil_build_jmp_va (tree, VEC (tree, heap) *);
static inline cil_stmt cil_build_call_mp (tree, VEC (tree, heap) *);
static inline cil_stmt cil_build_calli_mp (tree, VEC (tree, heap) *);
static inline cil_stmt cil_build_jmp_mp (tree, VEC (tree, heap) *);
static inline tree cil_call_ftype (const_cil_stmt);
static inline tree cil_call_fdecl (const_cil_stmt);
static inline size_t cil_call_nargs (const_cil_stmt);
static inline tree cil_call_arg_type (const_cil_stmt, size_t);
static inline void cil_call_set_static_chain (cil_stmt, tree);
static inline tree cil_call_static_chain (const_cil_stmt);
static inline void cil_call_set_dummy_arg (cil_stmt, tree);
static inline tree cil_call_dummy_arg (const_cil_stmt);
static inline bool cil_call_vararg_p (const_cil_stmt);
static inline bool cil_call_missing_proto_p (const_cil_stmt);
static inline tree cil_cst (const_cil_stmt);
static inline tree cil_string (const_cil_stmt);

extern bool cil_conversion_p (const_cil_stmt);
extern bool cil_cond_branch_p (const_cil_stmt);
extern bool cil_ldind_p (const_cil_stmt);
extern bool cil_stind_p (const_cil_stmt);
extern bool cil_push_p (const_cil_stmt);

/******************************************************************************
 * CIL sequence interface                                                     *
 ******************************************************************************/

static inline cil_seq_node cil_seq_first (const_cil_seq);
static inline cil_stmt cil_seq_first_stmt (const_cil_seq);
static inline cil_seq_node cil_seq_last (const_cil_seq);
static inline cil_stmt cil_seq_last_stmt (const_cil_seq);
static inline void cil_seq_set_last (cil_seq, cil_seq_node);
static inline void cil_seq_set_first (cil_seq, cil_seq_node);
static inline bool cil_seq_empty_p (const_cil_seq);
extern cil_seq cil_seq_alloc (void);
extern void cil_seq_free (cil_seq);
extern void cil_seq_add_stmt (cil_seq *, cil_stmt);
extern void cil_seq_add_seq (cil_seq *, cil_seq);
extern cil_seq cil_seq_deep_copy (cil_seq);
extern cil_seq cil_bb_seq (basic_block);
extern void cil_set_bb_seq (basic_block, cil_seq);
extern unsigned int cil_seq_stack_depth (cil_seq, unsigned int, bool);

/******************************************************************************
 * CIL statement iterator declaration & interface                             *
 ******************************************************************************/

/* Iterator object for CIL statement sequences.  */

typedef struct
{
  /* Current node.  */
  cil_seq_node ptr;

  /* Sequence and basic block associated with this iterator.  */
  cil_seq seq;
  basic_block bb;
} cil_stmt_iterator;

/* CIL statement list iterator interface */
static inline cil_stmt_iterator csi_start (cil_seq);
static inline cil_stmt_iterator csi_start_bb (basic_block);
static inline cil_stmt_iterator csi_last (cil_seq);
static inline cil_stmt_iterator csi_last_bb (basic_block);
static inline bool csi_end_p (cil_stmt_iterator);
static inline bool csi_one_before_end_p (cil_stmt_iterator);
static inline bool csi_first_p (cil_stmt_iterator);
static inline void csi_next (cil_stmt_iterator *);
static inline void csi_prev (cil_stmt_iterator *);
static inline cil_stmt csi_stmt (cil_stmt_iterator);
static inline cil_stmt *csi_stmt_ptr (cil_stmt_iterator *);
static inline basic_block csi_bb (cil_stmt_iterator);
static inline cil_seq csi_seq (cil_stmt_iterator);

enum csi_iterator_update
{
  CSI_NEW_STMT,		/* Only valid when single statement is added, move
			   iterator to it.  */
  CSI_SAME_STMT,	/* Leave the iterator at the same statement.  */
  CSI_CONTINUE_LINKING	/* Move iterator to whatever position is suitable
			   for linking other statements in the same
			   direction.  */
};

extern cil_seq csi_split_seq_after (cil_stmt_iterator);
extern cil_seq csi_split_seq_before (cil_stmt_iterator *);
extern void csi_replace (cil_stmt_iterator *, cil_stmt);
extern void csi_insert_before (cil_stmt_iterator *, cil_stmt,
			       enum csi_iterator_update);
extern void csi_insert_seq_before (cil_stmt_iterator *, cil_seq,
				   enum csi_iterator_update);
extern void csi_insert_after (cil_stmt_iterator *, cil_stmt,
			      enum csi_iterator_update);
extern void csi_insert_seq_after (cil_stmt_iterator *, cil_seq,
				  enum csi_iterator_update);
extern void csi_remove (cil_stmt_iterator *);
extern cil_stmt_iterator csi_for_stmt (cil_stmt);
extern void csi_move_after (cil_stmt_iterator *, cil_stmt_iterator *);
extern void csi_move_before (cil_stmt_iterator *, cil_stmt_iterator *);
extern void csi_move_to_bb_end (cil_stmt_iterator *, basic_block);

/* Inlined functions */
#include "cil-stmt-inline.h"

#endif /* !CIL_STMT_H */
