/* Simplify GENERIC trees before CIL emission.

   Copyright (C) 2006 Free Software Foundation, Inc.

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
   Andrea Bona
   Andrea Ornstein
   Erven Rohou
   Roberto Costa

Contact information at STMicroelectronics:
Roberto Costa <roberto.costa@st.com>   */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "diagnostic.h"
#include "real.h"
#include "hashtab.h"
#include "tree-flow.h"
#include "langhooks.h"
#include "tree-iterator.h"
#include "tree-chrec.h"
#include "tree-pass.h"
#include "timevar.h"
#include "assert.h"
#include "toplev.h"
#include "output.h"
#include "tree-simp-cil.h"

/* The purpose of this pass is to simplify GIMPLE trees in order
   to make CIL emission easier.
   As a matter of fact, there are some transformations that are
   difficult at emission time (pass gen_cil), i.e. those that
   involve generating new local temporary variables, modifications
   in the control-flow graph or in types...
   On the other hand, these transformations can be well performed
   in GIMPLE representation.
   The choice that is taken is to add restrictions to the GIMPLE trees
   gen_cil pass can handle and to make simp_cil pass enforce them.

   Currently, these are the transformations performed by cil_simp pass:

   *) Removal of RESULT_DECL nodes. CIL doesn't treat the value
      returned by a function in any special way: if it has to be
      temporarily stored, this must be in a local.
      A new local variable is generated and each RESULT_DECL node is
      transformed into a VAR_DECL of that variable.

   *) Expansion of ABS_EXPR nodes (in case of -mexpand-abs option).
      The expansion requires changes to the control-flow graph.

   *) Expansion of MAX_EXPR and MIN_EXPR nodes (in case of
      -mexpand-minmax option).
      The expansion requires changes to the control-flow graph.

   *) Expansion of SWITCH_EXPR, when it is not profitable to have
      a switch table (heuristic decision is based on case density).
      CIL emission pass (gen_cil) always emits a SWITCH_EXPR to a
      CIL switch opcode. When a low case density makes compare trees
      preferable, the SWITCH_EXPR is expanded; otherwise the
      SWITCH_EXPR is not modified.
      The expansion requires changes to the control-flow graph.

   *) Expansion of COMPONENT_REF nodes operating on bit-fields.
      CIL has no direct support for bit-field access; hence,
      equivalent code that extracts the bit pattern and applies the
      appropriate bit mask is generated.
      Memory access is performed by using INDIRECT_REF nodes.
      Beware that such a COMPONENT_REF on the left-hand side of an
      assignment also requires a load from memory; from the memory
      access point of view, the operation cannot be made atomic.

   *) Expansion of TARGET_MEM_REF nodes.
      Emission of such nodes is not difficult in gen_cil pass;
      however, a previous expansion may trigger further optimizations
      (since there is no similar construct in CIL bytecodes).

   *) Expansion of ARRAY_REF nodes.
      Emission of such nodes is not difficult in gen_cil pass;
      however, a previous expansion may generate better code (i.e.:
      it may fold constants) or trigger further optimizations
      (CIL arrays cannot be used for C-style arrays).

   *) Expansion of CONSTRUCTOR nodes used as right-hand sides of
      INIT_EXPR and MODIFY_EXPR nodes.
      Such CONSTRUCTOR nodes must be implemented in CIL bytecode through
      a sequence of finer grain initializations.
      Hence, initializer statements containing CONSTRUCTOR nodes
      are expanded into an equivalent list of initializer statements,
      with no more CONSTRUCTOR nodes.
      Since the same expansion must occur for global variables (which
      is performed by other passes), function
      expand_init_to_stmt_list (...) is exported.

   *) Expansion of LROTATE_EXPR and RROTATE_EXPR nodes.
      In CIL there no are opcodes for rotation and they have
      to be emulated through shifts and bit operations.
      A previous expansion may generate better code (i.e.:
      it may fold constants) or trigger further optimizations.

   *) The second operand of LSHIFT_EXPR and RSHIFT_EXPR
      is converted to a 32-bit size in the very rare
      cases it isn't already. This is always safe, because shifts with
      shift amounts bigger than the size of the operand to be shifted
      produce undefined results.
      The reason is that CIL shift operations require a shift operand
      of type int32.

   *) Rename of inlined variables to unique names.
      Emitted variables by gen_cil pass keep the original name.
      In case of variables declared within inlined functions,
      renaming them is needed clashes.

   *) Globalization of function static variables.
      CIL locals can be used for function non-static variables;
      there is no CIL feature to do the same with function static
      variables. Therefore, those variables have their scope changed
      (they become global), and their name as well, to avoid clashes.

   *) Expansion of initializers of local variables.
      In order to simplify gen_cil, the initialization of local
      variables (for those that have it) is expanded into the body
      of the entry basic block of the function.
*/

/* Local functions, macros and variables.  */
static tree get_unsigned_integer_type (int);
static bool is_copy_required (tree);
static bool mostly_zeros_p (tree);
static bool all_zeros_p (tree);
static void simp_switch (block_stmt_iterator *, tree *);
static void simp_trivial_switch (block_stmt_iterator *, tree *);
static void simp_abs (block_stmt_iterator *, tree *);
static void simp_min_max (block_stmt_iterator *, tree *);
static void simp_rotate (block_stmt_iterator *, tree *);
static void simp_shift (block_stmt_iterator *, tree);
static void simp_target_mem_ref (block_stmt_iterator *, tree *);
static void simp_array_ref (block_stmt_iterator *, tree *);
static void simp_rhs_bitfield_component_ref (block_stmt_iterator *, tree *);
static void simp_lhs_bitfield_component_ref (block_stmt_iterator *, tree *);
static void pre_simp_init (block_stmt_iterator *, tree);
static void simp_cil_node (block_stmt_iterator *, tree *);
static void split_use (block_stmt_iterator, tree *);
static void rename_var (tree, const char*);
static void simp_vars (void);
static unsigned int simp_cil (void);
static bool simp_cil_gate (void);

static tree res_var;
static tree uint32_type;

/* Return the unsigned integer type with size BITS bits */

static tree
get_unsigned_integer_type (int bits)
{
    if (GET_MODE_BITSIZE (TYPE_MODE (unsigned_type_node)) == bits)
      return unsigned_type_node;
    else if (GET_MODE_BITSIZE (TYPE_MODE (long_unsigned_type_node)) == bits)
      return long_unsigned_type_node;
    else if (GET_MODE_BITSIZE (TYPE_MODE (short_unsigned_type_node)) == bits)
      return short_unsigned_type_node;
    else if (GET_MODE_BITSIZE (TYPE_MODE (long_long_unsigned_type_node))
             == bits)
      return long_long_unsigned_type_node;
    else if (GET_MODE_BITSIZE (TYPE_MODE (unsigned_char_type_node)) == bits)
      return unsigned_char_type_node;
    else
      {
        gcc_assert (0);
        return NULL_TREE;
      }
}

/* In the case of multiple uses of tree NODE, return whether
   it is required to compute NODE only once or not.
   If NODE has side effects, TRUE is obviously always returned.
   If NODE has no side effects, TRUE is still returned if
   it looks more profitable to compute NODE only once,
   FALSE otherwise (this is a heuristic decision).   */

static bool
is_copy_required (tree node)
{
  if (TREE_SIDE_EFFECTS (node))
    return TRUE;

  switch (TREE_CODE (node))
    {
    case INTEGER_CST:
    case REAL_CST:
    case VAR_DECL:
    case PARM_DECL:
      return FALSE;
    default:
      return TRUE;
    }
}

/* Simplify the node pointed by NODE_PTR in order to make CIL emission easier.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions may be inserted,
   new basic blocks created...
   NODE is passed by reference because simplification may require
   replacing the node.   */

static void
simp_cil_node (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;

  if (node == NULL_TREE)
    return;  /* ER: was spc */

  switch (TREE_CODE (node))
    {
    case COND_EXPR:
      simp_cil_node (bsi, &COND_EXPR_COND (node));
      break;

    case SWITCH_EXPR:
      simp_cil_node (bsi, &SWITCH_COND (node));
      simp_switch (bsi, node_ptr);
      break;

    case CALL_EXPR:
      {
        tree args = TREE_OPERAND (node, 1);

        simp_cil_node (bsi, &TREE_OPERAND (node, 0));

        while (args)
          {
            simp_cil_node (bsi, &TREE_VALUE (args));
            args = TREE_CHAIN (args);
          }
      }
      break;

    case MULT_EXPR:
    case PLUS_EXPR:
    case MINUS_EXPR:
    case RDIV_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
    case BIT_AND_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
    case LT_EXPR:
    case GT_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case LE_EXPR:
    case GE_EXPR:
    case EXACT_DIV_EXPR:
    case TRUNC_DIV_EXPR:
    case TRUNC_MOD_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &TREE_OPERAND (node, 1));
      break;

    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &TREE_OPERAND (node, 1));
      if (TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (TREE_OPERAND (node, 1))))
          > 32)
        simp_shift (bsi, node);
      break;

    case LROTATE_EXPR:
    case RROTATE_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &TREE_OPERAND (node, 1));
      simp_rotate (bsi, node_ptr);
      break;

    case INIT_EXPR:
    case MODIFY_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &TREE_OPERAND (node, 1));
      gcc_assert (TREE_CODE (TREE_OPERAND (node, 1)) != CONSTRUCTOR
                  && TREE_CODE (TREE_OPERAND (node, 1)) != STRING_CST);
      if (AGGREGATE_TYPE_P (TREE_TYPE (TREE_OPERAND (node, 1)))
          && TREE_CODE (TREE_OPERAND (node, 0)) == INDIRECT_REF
          && TREE_CODE (TREE_OPERAND (node, 1)) == CALL_EXPR)
        split_use (*bsi, &TREE_OPERAND (node, 1));
      break;

    case NEGATE_EXPR:
    case BIT_NOT_EXPR:
    case TRUTH_NOT_EXPR:
    case CONVERT_EXPR:
    case NOP_EXPR:
    case FLOAT_EXPR:
    case FIX_TRUNC_EXPR:
    case BIT_FIELD_REF:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      break;

    case ADDR_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      if (AGGREGATE_TYPE_P (TREE_TYPE (TREE_OPERAND (node, 0)))
          && TREE_CODE (TREE_OPERAND (node, 0)) == CALL_EXPR)
        split_use (*bsi, &TREE_OPERAND (node, 0));
      break;

    case INDIRECT_REF:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      if (AGGREGATE_TYPE_P (TREE_TYPE (node))
          && TREE_CODE (TREE_OPERAND (node, 0)) == CALL_EXPR)
        split_use (*bsi, &TREE_OPERAND (node, 0));
      break;

    case COMPONENT_REF:
      gcc_assert (TREE_CODE (TREE_OPERAND (node, 1)) == FIELD_DECL);
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      if (AGGREGATE_TYPE_P (TREE_TYPE (TREE_OPERAND (node, 0)))
          && TREE_CODE (TREE_OPERAND (node, 0)) == CALL_EXPR)
        split_use (*bsi, &TREE_OPERAND (node, 0));
      if (DECL_BIT_FIELD (TREE_OPERAND (node, 1)))
        {
          tree stmt = bsi_stmt (*bsi);

          if (TREE_CODE (stmt) == MODIFY_EXPR
              && TREE_OPERAND (stmt, 0) == node)
            simp_lhs_bitfield_component_ref (bsi, node_ptr);
          else
            simp_rhs_bitfield_component_ref (bsi, node_ptr);
        }
      break;

    case TARGET_MEM_REF:
      simp_cil_node (bsi, &TMR_SYMBOL (node));
      simp_cil_node (bsi, &TMR_BASE (node));
      simp_cil_node (bsi, &TMR_INDEX (node));
      simp_target_mem_ref (bsi, node_ptr);
      node = *node_ptr;
      gcc_assert (TREE_CODE (node) == INDIRECT_REF);
      if (AGGREGATE_TYPE_P (TREE_TYPE (node))
          && TREE_CODE (TREE_OPERAND (node, 0)) == CALL_EXPR)
        split_use (*bsi, &TREE_OPERAND (node, 0));
      break;

    case ARRAY_REF:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &TREE_OPERAND (node, 1));
      simp_array_ref (bsi, node_ptr);
      node = *node_ptr;
      gcc_assert (TREE_CODE (node) == INDIRECT_REF);
      if (AGGREGATE_TYPE_P (TREE_TYPE (node))
          && TREE_CODE (TREE_OPERAND (node, 0)) == CALL_EXPR)
        split_use (*bsi, &TREE_OPERAND (node, 0));
      break;

    case RETURN_EXPR:
      if (!TREE_OPERAND (node, 0)
          && TREE_CODE (TREE_TYPE (DECL_RESULT (current_function_decl)))
          != VOID_TYPE)
        {
          /* Pre-C99 code may contain void-returns for non-void functions.
             In this case, return the result variable.   */

          if (res_var == NULL_TREE)
            res_var = create_tmp_var (TREE_TYPE (DECL_RESULT (current_function_decl)),
                                      "cilsimp");

          TREE_OPERAND (node, 0) = res_var;
        }
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      break;

    case RESULT_DECL:
      if (res_var == NULL_TREE)
        res_var = create_tmp_var (TREE_TYPE (node), "cilsimp");

      *node_ptr = res_var;
      break;

    case ABS_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      if (TARGET_EXPAND_ABS)
        simp_abs (bsi, node_ptr);
      break;

    case MAX_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &TREE_OPERAND (node, 1));
      if (TARGET_EXPAND_MINMAX)
        simp_min_max (bsi, node_ptr);
      break;

    case MIN_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &TREE_OPERAND (node, 1));
      if (TARGET_EXPAND_MINMAX)
        simp_min_max (bsi, node_ptr);
      break;

    default:
      ;
    }
}

/* Expand the SWITCH_EXPR pointed by NODE_PTR by inserting
   compare trees. The expansion occurs only if heuristics say
   it is profitable; the current heuristic is based on case label
   density.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted,
   new basic blocks created...
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_switch (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree vec = SWITCH_LABELS (node), vec1, vec2;
  unsigned int vec_len, i;
  bool first_case = true;
  tree min_val = 0, max_val = 0;
  double_int range, n_elems = double_int_zero, density1000, tmp_elems;
  unsigned int sw1_last;
  tree sw1_last_int_cst;
  tree sw1_stmt, sw2_stmt;
  block_stmt_iterator tmp_bsi;
  basic_block bb_orig_sw, bb1, bb2;
  tree cmp_stmt;
  tree label1_decl, label2_decl, label1, label2;
  edge_iterator ei;
  edge e1, e2, tmp_edge;

  /* The switch body is lowered in gimplify.c, we should never have
     switches with a non-NULL SWITCH_BODY here.  */
  gcc_assert (TREE_CODE (node) == SWITCH_EXPR && vec && !SWITCH_BODY (node));

  vec_len = TREE_VEC_LENGTH (vec);

  /* Switches made of one case are always separately (they are always
     transformed into if ... then ... else ...    */
  if (vec_len == 2)
    simp_trivial_switch (bsi, node_ptr);

  /* Compute range of cases */
  for (i = 0; i < vec_len - 1; ++i)
    {
      tree elt  = TREE_VEC_ELT (vec, i);
      tree low  = CASE_LOW (elt);
      tree high = CASE_HIGH (elt);

      if (!high)
        high = low;

      gcc_assert (low && high);

      /* Discard empty ranges.  */
      if (INT_CST_LT (high, low))
        continue;

      /* Increment number of elements seen so far */
      n_elems = double_int_add (n_elems, TREE_INT_CST (high));
      n_elems = double_int_add (n_elems, double_int_one);
      n_elems = double_int_add (n_elems, double_int_neg (TREE_INT_CST (low)));

      if (first_case)
        {
          min_val = low;
          max_val = high;
          first_case = false;
        }
      else
        {
          if (INT_CST_LT (low, min_val))
            min_val = low;

          if (INT_CST_LT (max_val, high))
            max_val = high;
        }
    }
  gcc_assert (!INT_CST_LT (max_val, min_val));
  range = double_int_add (TREE_INT_CST (max_val), double_int_one);
  range = double_int_add (range, double_int_neg (TREE_INT_CST (min_val)));

  /* If the range density is not high, keep the switch statement */
  density1000 = double_int_udiv (double_int_mul (n_elems,
                                                 shwi_to_double_int (1000)),
                                 range,
                                 TRUNC_DIV_EXPR);
  if (! double_int_negative_p (double_int_add (density1000,
                                               shwi_to_double_int (-333))))
    return;

  /* Insert a copy of the switch condition, if required */
  if (is_copy_required (SWITCH_COND (node)))
    {
      tree cond = SWITCH_COND (node);
      tree var = create_tmp_var (TREE_TYPE (cond), "cilsimp");
      tree stmt = build2 (MODIFY_EXPR, TREE_TYPE (cond), var, cond);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      SWITCH_COND (node) = var;
    }

  /* Compute the last CASE_LABEL_EXPR that will go to the 1st switch.
     To do that, count number of elements until tmp_elems >= 0   */
  tmp_elems = double_int_neg (double_int_udiv (n_elems,
                                               double_int_two,
                                               TRUNC_DIV_EXPR));
  for (i = 0; i < vec_len - 1; ++i)
    {
      tree elt  = TREE_VEC_ELT (vec, i);
      tree low  = CASE_LOW (elt);
      tree high = CASE_HIGH (elt);

      if (!high)
        high = low;

      gcc_assert (low && high);

      /* Discard empty ranges.  */
      if (INT_CST_LT (high, low))
        continue;

      /* Increment number of elements seen so far */
      tmp_elems = double_int_add (tmp_elems, TREE_INT_CST (high));
      tmp_elems = double_int_add (tmp_elems, double_int_one);
      tmp_elems = double_int_add (tmp_elems,
                                  double_int_neg (TREE_INT_CST (low)));

      if (! double_int_negative_p (tmp_elems))
        break;
    }
  gcc_assert (! double_int_negative_p (tmp_elems));
  gcc_assert (i <= vec_len - 2);
  if (i == vec_len - 2)
    /* It's the last case! Then, take the previous case */
    i = vec_len - 3;
  sw1_last = i;
  sw1_last_int_cst = CASE_HIGH (TREE_VEC_ELT (vec, sw1_last));
  if (! sw1_last_int_cst)
    sw1_last_int_cst = CASE_LOW (TREE_VEC_ELT (vec, sw1_last));

  /* Build a COND_EXPR, replace the switch with the COND_EXPR */
  bb_orig_sw = bb_for_stmt (bsi_stmt (*bsi));
  label1_decl = create_artificial_label ();
  label2_decl = create_artificial_label ();
  cmp_stmt = build3 (COND_EXPR, void_type_node,
                     build2 (GT_EXPR, boolean_type_node,
                             SWITCH_COND (node), sw1_last_int_cst),
                     build1 (GOTO_EXPR, void_type_node, label2_decl),
                     build1 (GOTO_EXPR, void_type_node, label1_decl));
  SET_EXPR_LOCATION (cmp_stmt, locus);
  gcc_assert (stmt_ends_bb_p (node) && bb_for_stmt (node) == bb_orig_sw);

  /* Replace the original switch with the COND_EXPR */
  *node_ptr = cmp_stmt;
  set_bb_for_stmt (cmp_stmt, bb_orig_sw);

  /* Update the basic block statement iterator */
  *bsi = bsi_last (bb_orig_sw);

  /* Generate a basic block with the first switch */
  bb1 = create_empty_bb (bb_orig_sw);
  bb1->count = bb_orig_sw->count / 2;
  label1 = build1 (LABEL_EXPR, void_type_node, label1_decl);
  vec1 = make_tree_vec (sw1_last + 2);
  sw1_stmt = build3 (SWITCH_EXPR, TREE_TYPE (node),
                     SWITCH_COND (node), NULL, vec1);
  SET_EXPR_LOCATION (sw1_stmt, locus);
  tmp_bsi = bsi_start (bb1);
  bsi_insert_after (&tmp_bsi, label1, BSI_NEW_STMT);
  bsi_insert_after (&tmp_bsi, sw1_stmt, BSI_SAME_STMT);

  /* Generate a basic block with the second switch */
  bb2 = create_empty_bb (bb1);
  bb2->count = bb_orig_sw->count - bb1->count;
  label2 = build1 (LABEL_EXPR, void_type_node, label2_decl);
  vec2 = make_tree_vec (vec_len - sw1_last - 1);
  sw2_stmt = build3 (SWITCH_EXPR, TREE_TYPE (node),
                     SWITCH_COND (node), NULL, vec2);
  SET_EXPR_LOCATION (sw2_stmt, locus);
  tmp_bsi = bsi_start (bb2);
  bsi_insert_after (&tmp_bsi, label2, BSI_NEW_STMT);
  bsi_insert_after (&tmp_bsi, sw2_stmt, BSI_SAME_STMT);

  /* Build the case labels for the 1st new switch and the out-edges
     of its basic block.    */
  for (i = 0; i < sw1_last + 1; ++i)
    {
      tree elt = TREE_VEC_ELT (vec, i);
      basic_block target_bb = label_to_block (CASE_LABEL (elt));
      edge e = make_edge (bb1, target_bb, 0);

      if (!e)
        e = find_edge (bb1, target_bb);

      e->count = 0;
      e->probability = 0;

      TREE_VEC_ELT (vec1, i) = elt;
    }
    {
      tree elt = TREE_VEC_ELT (vec, vec_len - 1);
      basic_block target_bb = label_to_block (CASE_LABEL (elt));
      edge e = make_edge (bb1, target_bb, 0);

      if (!e)
        e = find_edge (bb1, target_bb);

      e->count = bb1->count;
      e->probability = REG_BR_PROB_BASE;

      TREE_VEC_ELT (vec1, sw1_last + 1) = elt;
    }

  /* Build the case labels for the 2nd new switch and the out-edges
     of its basic block.    */
  for (; i < vec_len - 1; ++i)
    {
      tree elt = TREE_VEC_ELT (vec, i);
      basic_block target_bb = label_to_block (CASE_LABEL (elt));
      edge e = make_edge (bb2, target_bb, 0);

      if (!e)
        e = find_edge (bb2, target_bb);

      e->count = 0;
      e->probability = 0;

      TREE_VEC_ELT (vec2, i - sw1_last - 1) = elt;
    }
    {
      tree elt = TREE_VEC_ELT (vec, vec_len - 1);
      basic_block target_bb = label_to_block (CASE_LABEL (elt));
      edge e = make_edge (bb2, target_bb, 0);

      if (!e)
        e = find_edge (bb2, target_bb);

      e->count = bb2->count;
      e->probability = REG_BR_PROB_BASE;

      TREE_VEC_ELT (vec2, vec_len - sw1_last - 2) = elt;
    }

  /* Update out-edges of original switch basic block */
  for (ei = ei_start (bb_orig_sw->succs); (tmp_edge = ei_safe_edge (ei)); )
    {
      remove_edge (tmp_edge);
    }
  e1 = unchecked_make_edge (bb_orig_sw, bb1, EDGE_FALSE_VALUE);
  e2 = unchecked_make_edge (bb_orig_sw, bb2, EDGE_TRUE_VALUE);
  e1->probability = REG_BR_PROB_BASE / 2;
  e1->count = bb1->count;
  e2->probability = REG_BR_PROB_BASE - e1->probability;
  e2->count = bb2->count;

  /* TODO: probabilities of the out-edges of the new basic blocks
           currently do not reflect those of the out-edges of the
           original switch basic block.
           In order to "preserve" them, the new edges should be given
           probabilities based on the original edges, but normalized.   */

  /* TODO: basic block frequencies are not updated, this makes
           the profile information sanity check to fail.   */
}

/* Expand the SWITCH_EXPR pointed by NODE_PTR, composed of just
   one case, into a COND_EXPR (or GOTO_EXPR).
   The expansion always occurs, since generally profitable.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted,
   new basic blocks may be created...
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_trivial_switch (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree one_case = TREE_VEC_ELT (SWITCH_LABELS (node), 0);
  basic_block bb_sw = bb_for_stmt (bsi_stmt (*bsi));
  basic_block bb_case = label_to_block (CASE_LABEL (one_case));

  /* The switch body is lowered in gimplify.c, we should never have
     switches with a non-NULL SWITCH_BODY here.  */
  gcc_assert (TREE_CODE (node) == SWITCH_EXPR
              && !SWITCH_BODY (node)
              && SWITCH_LABELS (node)
              && TREE_VEC_LENGTH (SWITCH_LABELS (node)) == 2);

  gcc_assert (stmt_ends_bb_p (node) && bb_for_stmt (node) == bb_sw);

  /* Check for the easiest situation: the one case and default go
     to the same basic block.
     If so, SWITCH_EXPR is replaced by a GOTO_EXPR.   */
  if (single_succ_p (bb_sw))
    {
      tree goto_stmt;
      edge e;

      gcc_assert (bb_case
                  == label_to_block (CASE_LABEL (TREE_VEC_ELT (SWITCH_LABELS (node), 1))));

      /* Build the GOTO_EXPR */
      goto_stmt = build1 (GOTO_EXPR, void_type_node, CASE_LABEL (one_case));
      SET_EXPR_LOCATION (goto_stmt, locus);

      /* Update CFG */
      e = find_edge (bb_sw, bb_case);
      e->flags |= EDGE_FALLTHRU;

      /* Replace the original switch with the GOTO_EXPR */
      *node_ptr = goto_stmt;
      set_bb_for_stmt (goto_stmt, bb_sw);

      /* Update the basic block statement iterator */
      *bsi = bsi_last (bb_sw);
    }

  /* Check whether the one case is not a range.
     If it is not, SWITCH_EXPR is replaced by a single COND_EXPR.   */
  else if (! CASE_HIGH (one_case))
    {
      tree deft = TREE_VEC_ELT (SWITCH_LABELS (node), 1);
      tree cmp_stmt;
      basic_block bb_deft = label_to_block (CASE_LABEL (deft));
      edge e1, e2;

      /* Build the COND_EXPR */
      cmp_stmt = build3 (COND_EXPR, void_type_node,
                         build2 (EQ_EXPR, boolean_type_node,
                                 SWITCH_COND (node),
                                 CASE_LOW (one_case)),
                         build1 (GOTO_EXPR, void_type_node,
                                 CASE_LABEL (one_case)),
                         build1 (GOTO_EXPR, void_type_node,
                                 CASE_LABEL (deft)));
      SET_EXPR_LOCATION (cmp_stmt, locus);

      /* Update CFG */
      e1 = find_edge (bb_sw, bb_case);
      e2 = find_edge (bb_sw, bb_deft);
      gcc_assert (e1 && e2 && e1 != e2);
      gcc_assert ((e1->flags & EDGE_FALSE_VALUE) == 0);
      gcc_assert ((e2->flags & EDGE_TRUE_VALUE) == 0);
      e1->flags |= EDGE_TRUE_VALUE;
      e2->flags |= EDGE_FALSE_VALUE;

      /* Replace the original switch with the COND_EXPR */
      *node_ptr = cmp_stmt;
      set_bb_for_stmt (cmp_stmt, bb_sw);

      /* Update the basic block statement iterator */
      *bsi = bsi_last (bb_sw);
    }

  /* The one case is a range.
     Therefore, SWITCH_EXPR is replaced by a couple of COND_EXPRs.   */
  else
    {
      tree deft = TREE_VEC_ELT (SWITCH_LABELS (node), 1);
      tree label_decl = create_artificial_label ();
      tree cmp1_stmt, cmp2_stmt, label;
      basic_block bb_deft = label_to_block (CASE_LABEL (deft));
      basic_block bb_new;
      block_stmt_iterator tmp_bsi;
      edge e1, e2, e3, e4;
      int e1_orig_prob, e2_orig_prob;

      /* Build the 1st COND_EXPR */
      cmp1_stmt = build3 (COND_EXPR, void_type_node,
                          build2 (GE_EXPR, boolean_type_node,
                                  SWITCH_COND (node),
                                  CASE_LOW (one_case)),
                          build1 (GOTO_EXPR, void_type_node, label_decl),
                          build1 (GOTO_EXPR, void_type_node,
                                  CASE_LABEL (deft)));
      SET_EXPR_LOCATION (cmp1_stmt, locus);

      /* Replace the original switch with the 1st COND_EXPR */
      *node_ptr = cmp1_stmt;
      set_bb_for_stmt (cmp1_stmt, bb_sw);

      /* Build the 2nd COND_EXPR */
      cmp2_stmt = build3 (COND_EXPR, void_type_node,
                          build2 (LE_EXPR, boolean_type_node,
                                  SWITCH_COND (node),
                                  CASE_HIGH (one_case)),
                          build1 (GOTO_EXPR, void_type_node,
                                  CASE_LABEL (one_case)),
                          build1 (GOTO_EXPR, void_type_node,
                                  CASE_LABEL (deft)));
      SET_EXPR_LOCATION (cmp2_stmt, locus);

      /* Create new basic block and insert the 2nd COND_EXPR */
      bb_new = create_empty_bb (bb_sw);
      label = build1 (LABEL_EXPR, void_type_node, label_decl);
      tmp_bsi = bsi_start (bb_new);
      bsi_insert_after (&tmp_bsi, label, BSI_NEW_STMT);
      bsi_insert_after (&tmp_bsi, cmp2_stmt, BSI_SAME_STMT);

      /* Update CFG */
      e1 = find_edge (bb_sw, bb_case);
      e2 = find_edge (bb_sw, bb_deft);
      gcc_assert (e1 && e2 && e1 != e2);
      gcc_assert ((e1->flags & EDGE_FALSE_VALUE) == 0);
      gcc_assert ((e2->flags & EDGE_TRUE_VALUE) == 0);
      redirect_edge_succ (e1, bb_new);
      e2->flags |= EDGE_FALSE_VALUE;
      e3 = unchecked_make_edge (bb_new, bb_case, EDGE_TRUE_VALUE);
      e3->flags = e1->flags | EDGE_TRUE_VALUE;;
      e4 = unchecked_make_edge (bb_new, bb_deft, EDGE_FALSE_VALUE);
      e4->flags = e2->flags;
      e1->flags = EDGE_TRUE_VALUE;

      /* Update edge and basic block counts */
      e3->count = e1->count;
      e4->count = e2->count / 2;
      e2->count = e2->count - e4->count;
      e1->count = e3->count + e4->count;
      bb_new->count = e1->count;

      /* Update edge probabilities */
      e1_orig_prob = e1->probability;
      e2_orig_prob = e2->probability;
      e2->probability = e2_orig_prob / 2;
      e1->probability = REG_BR_PROB_BASE - e2->probability;
      e3->probability = (e1_orig_prob * REG_BR_PROB_BASE) / e1->probability;
      e4->probability = REG_BR_PROB_BASE - e3->probability;

      /* Update the basic block statement iterator */
      *bsi = tmp_bsi;
    }
}

/* Remove the ABS_EXPR pointed by NODE_PTR by inserting
   explicit control flow.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted,
   new basic blocks created...
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_abs (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree op;
  tree label_decl_neg = create_artificial_label ();
  tree label_decl_sel = create_artificial_label ();
  tree label_neg, label_sel;
  tree sel_var;
  tree orig_stmt, cmp_stmt, asn_op_stmt, asn_neg_stmt;
  basic_block bb_comp, bb_neg, bb_sel;
  edge tmp_edge;
  block_stmt_iterator tmp_bsi;
  gcov_type count;

  gcc_assert (TREE_CODE (node) == ABS_EXPR);

  /* Insert a statement that copies the operand.
     This is always done: it is always useful because it avoids
     generating an extra basic block.   */
  op = TREE_OPERAND (node, 0);
  sel_var = create_tmp_var (TREE_TYPE (op), "cilsimp");
  asn_op_stmt = build2 (MODIFY_EXPR, TREE_TYPE (op), sel_var, op);
  SET_EXPR_LOCATION (asn_op_stmt, locus);
  bsi_insert_before (bsi, asn_op_stmt, BSI_SAME_STMT);

  /* Insert the comparison statement */
  cmp_stmt = build3 (COND_EXPR, void_type_node,
                     build2 (GE_EXPR, boolean_type_node, sel_var,
                             build_int_cst (TREE_TYPE (op), 0)),
                     build1 (GOTO_EXPR, void_type_node, label_decl_sel),
                     build1 (GOTO_EXPR, void_type_node, label_decl_neg));
  SET_EXPR_LOCATION (cmp_stmt, locus);
  bsi_insert_before (bsi, cmp_stmt, BSI_SAME_STMT);

  /* Update cfg */
  orig_stmt = bsi_stmt (*bsi);
  bb_comp = bb_for_stmt (orig_stmt);
  count = bb_comp->count;
  tmp_edge = split_block (bb_comp, cmp_stmt);
  bb_sel = tmp_edge->dest;
  bb_sel->count = count;
  remove_edge (tmp_edge);
  bb_neg = create_empty_bb (bb_comp);
  bb_neg->count = count / 2;
  unchecked_make_edge (bb_comp, bb_neg, EDGE_FALSE_VALUE);
  make_single_succ_edge (bb_neg, bb_sel, EDGE_FALLTHRU);

  /* Insert labels and statements into neg bb */
  label_neg = build1 (LABEL_EXPR, void_type_node, label_decl_neg);
  asn_neg_stmt = build2 (MODIFY_EXPR, TREE_TYPE (op),
                         sel_var,
                         build1 (NEGATE_EXPR, TREE_TYPE (op), sel_var));
  SET_EXPR_LOCATION (asn_neg_stmt, locus);
  tmp_bsi = bsi_start (bb_neg);
  bsi_insert_after (&tmp_bsi, label_neg, BSI_NEW_STMT);
  bsi_insert_after (&tmp_bsi, asn_neg_stmt, BSI_SAME_STMT);

  /* Insert a label into sel bb */
  label_sel = build1 (LABEL_EXPR, void_type_node, label_decl_sel);
  tmp_bsi = bsi_start (bb_sel);
  bsi_insert_before (&tmp_bsi, label_sel, BSI_SAME_STMT);

  /* Update current node in order to use the select variable */
  *node_ptr = sel_var;

  /* Update the basic block statement iterator */
  gcc_assert (bsi_stmt (tmp_bsi) == orig_stmt);
  *bsi = tmp_bsi;
}

/* Remove the MAX_EXPR or MIN_EXPR pointed by NODE_PTR by inserting
   explicit control flow.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted,
   new basic blocks created...
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_min_max (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  bool is_max;
  tree op0, op1;
  tree label_decl_op0 = create_artificial_label ();
  tree label_decl_op1 = create_artificial_label ();
  tree label_op0, label_op1;
  tree sel_var;
  tree orig_stmt, cmp_stmt, asn_op0_stmt, asn_op1_stmt;
  basic_block bb_comp, bb_op0, bb_op1, bb_sel;
  edge tmp_edge;
  block_stmt_iterator tmp_bsi;
  gcov_type count;

  gcc_assert (TREE_CODE (node) == MAX_EXPR || TREE_CODE (node) == MIN_EXPR);
  is_max = (TREE_CODE (node) == MAX_EXPR);

  /* Make sure that the two operands have no side effects */
  op0 = TREE_OPERAND (node, 0);
  if (is_copy_required (op0))
    {
      tree var = create_tmp_var (TREE_TYPE (op0), "cilsimp");
      tree stmt = build2 (MODIFY_EXPR, TREE_TYPE (op0), var, op0);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      TREE_OPERAND (node, 0) = var;
      op0 = var;
    }
  op1 = TREE_OPERAND (node, 1);
  if (is_copy_required (op1))
    {
      tree var = create_tmp_var (TREE_TYPE (op1), "cilsimp");
      tree stmt = build2 (MODIFY_EXPR, TREE_TYPE (op1), var, op1);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      TREE_OPERAND (node, 1) = var;
      op1 = var;
    }

  /* Insert the comparison statement */
  cmp_stmt = build3 (COND_EXPR, void_type_node,
                     build2 (is_max ? GT_EXPR : LT_EXPR,
                             boolean_type_node, op0, op1),
                     build1 (GOTO_EXPR, void_type_node, label_decl_op0),
                     build1 (GOTO_EXPR, void_type_node, label_decl_op1));
  SET_EXPR_LOCATION (cmp_stmt, locus);
  bsi_insert_before (bsi, cmp_stmt, BSI_SAME_STMT);

  /* Update cfg */
  orig_stmt = bsi_stmt (*bsi);
  bb_comp = bb_for_stmt (orig_stmt);
  count = bb_comp->count;
  tmp_edge = split_block (bb_comp, cmp_stmt);
  bb_sel = tmp_edge->dest;
  bb_sel->count = count;
  remove_edge (tmp_edge);
  bb_op0 = create_empty_bb (bb_comp);
  bb_op1 = create_empty_bb (bb_op0);
  bb_op0->count = count / 2;
  bb_op1->count = count - bb_op0->count;
  unchecked_make_edge (bb_comp, bb_op0, EDGE_TRUE_VALUE);
  make_single_succ_edge (bb_op0, bb_sel, EDGE_FALLTHRU);
  unchecked_make_edge (bb_comp, bb_op1, EDGE_FALSE_VALUE);
  make_single_succ_edge (bb_op1, bb_sel, EDGE_FALLTHRU);

  /* Insert labels and statements into op0 bb */
  sel_var = create_tmp_var (TREE_TYPE (node), "cilsimp");
  label_op0 = build1 (LABEL_EXPR, void_type_node, label_decl_op0);
  asn_op0_stmt = build2 (MODIFY_EXPR, TREE_TYPE (op0), sel_var, op0);
  SET_EXPR_LOCATION (asn_op0_stmt, locus);
  tmp_bsi = bsi_start (bb_op0);
  bsi_insert_after (&tmp_bsi, label_op0, BSI_NEW_STMT);
  bsi_insert_after (&tmp_bsi, asn_op0_stmt, BSI_SAME_STMT);

  /* Insert labels and statements into op1 bb */
  label_op1 = build1 (LABEL_EXPR, void_type_node, label_decl_op1);
  asn_op1_stmt = build2 (MODIFY_EXPR, TREE_TYPE (op1), sel_var, op1);
  SET_EXPR_LOCATION (asn_op1_stmt, locus);
  tmp_bsi = bsi_start (bb_op1);
  bsi_insert_after (&tmp_bsi, label_op1, BSI_NEW_STMT);
  bsi_insert_after (&tmp_bsi, asn_op1_stmt, BSI_SAME_STMT);

  /* Update current node in order to use the select variable */
  *node_ptr = sel_var;

  /* Update the basic block statement iterator */
  gcc_assert (bsi_stmt (bsi_start (bb_sel)) == orig_stmt);
  *bsi = bsi_start (bb_sel);
}

/* Remove the LROTATE_EXPR or RROTATE_EXPR pointed by NODE_PTR
   by inserting shifts and bit operations.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted,
   new basic blocks created...
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_rotate (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  bool left = (TREE_CODE (node) == LROTATE_EXPR);
  tree op0_uns_type;
  tree op0, op1;
  tree t1, t2;

  gcc_assert (TREE_CODE (node) == LROTATE_EXPR
              || TREE_CODE (node) == RROTATE_EXPR);

  /* Rotation is replaced by shifts on unsigned values:
     generate the unsigned version of first operand type. */
  op0 = TREE_OPERAND (node, 0);
  op0_uns_type = build_distinct_type_copy (TREE_TYPE (op0));
  TYPE_UNSIGNED (op0_uns_type) = 1;
  op0 = fold_convert (op0_uns_type, op0);

  /* Convert the second operand to 32-bit */
  op1 = fold_convert (uint32_type, TREE_OPERAND (node, 1));

  /* Make sure that the two operands have no side effects */
  if (is_copy_required (op0))
    {
      tree var = create_tmp_var (TREE_TYPE (op0), "cilsimp");
      tree stmt = build2 (MODIFY_EXPR, TREE_TYPE (op0), var, op0);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      TREE_OPERAND (node, 0) = var;
      op0 = var;
    }
  if (is_copy_required (op1))
    {
      tree var = create_tmp_var (TREE_TYPE (op1), "cilsimp");
      tree stmt = build2 (MODIFY_EXPR, TREE_TYPE (op1), var, op1);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      TREE_OPERAND (node, 1) = var;
      op1 = var;
    }

  /* Build first shift */
  t1 = build2 (left ? LSHIFT_EXPR : RSHIFT_EXPR, op0_uns_type,
               op0,
               op1);

  /* Build second shift */
  t2 = fold_build2 (left ? RSHIFT_EXPR : LSHIFT_EXPR, op0_uns_type,
                    op0,
                    build2 (MINUS_EXPR, uint32_type,
                            fold_convert (uint32_type,
                                          TYPE_SIZE (TREE_TYPE (op0))),
                            op1));

  /* Gimplify the two shifts */
  t1 = force_gimple_operand_bsi (bsi, t1, TRUE, NULL);
  t2 = force_gimple_operand_bsi (bsi, t2, TRUE, NULL);

  /* Build the rotate result and gimplify it */
  t1 = build2 (BIT_IOR_EXPR, op0_uns_type, t1, t2);
  t1 = fold_convert (TREE_TYPE (TREE_OPERAND (node, 0)), t1);
  t1 = force_gimple_operand_bsi (bsi, t1, TRUE, NULL);

  /* Update the current node */
  *node_ptr = t1;
}

/* Given the LSHIFT_EXPR or RSHIFT_EXPR
   in NODE with the second operand of an integer type bigger than
   32 bits, convert such operand to a 32-bit type.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.   */

static void
simp_shift (block_stmt_iterator *bsi, tree node)
{
  tree t;

  gcc_assert (TREE_CODE (node) == LSHIFT_EXPR
              || TREE_CODE (node) == RSHIFT_EXPR);

  /* Generate the type conversion */
  t = fold_convert (uint32_type, TREE_OPERAND (node, 1));

  /* Gimplify the equivalent expression and update the current node */
  TREE_OPERAND (node, 1) = force_gimple_operand_bsi (bsi, t, FALSE, NULL);
}

/* Expand the TARGET_MEM_REF pointed by NODE_PTR by inserting
   the equivalent sums and multiplication.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_target_mem_ref (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  tree t1, t2;

  gcc_assert (TREE_CODE (node) == TARGET_MEM_REF);

  /* Generate the equivalent expression */

  if (TMR_BASE (node))
    {
      if (TMR_SYMBOL (node))
        t1 = build2 (PLUS_EXPR, TREE_TYPE (TMR_BASE (node)),
                     TMR_BASE (node),
                     TMR_SYMBOL (node));
      else
        t1 = TMR_BASE (node);
    }
  else
    t1 = TMR_SYMBOL (node);

  if (TMR_INDEX (node))
    {
      if (TMR_STEP (node))
        t2 = build2 (MULT_EXPR, TREE_TYPE (TMR_INDEX (node)),
                     TMR_INDEX (node),
                     TMR_STEP (node));
      else
        t2 = TMR_INDEX (node);

      gcc_assert (t2);
      if (TMR_OFFSET (node))
        t2 = build2 (PLUS_EXPR, TREE_TYPE (t2),
                     TMR_OFFSET (node),
                     t2);
    }
  else
    {
      gcc_assert (! TMR_STEP (node));
      t2 = TMR_OFFSET (node);
    }

  if (t1)
    {
      if (t2)
        t1 = build2 (PLUS_EXPR, TREE_TYPE (t1), t1, t2);
    }
  else
    t1 = t2;

  gcc_assert (t1 && t1 != error_mark_node);
  t1 = build1 (INDIRECT_REF, TREE_TYPE (node), t1);

  /* Gimplify the equivalent expression and update the current node */
  *node_ptr = force_gimple_operand_bsi (bsi, t1, FALSE, NULL);
}

/* Expand the ARRAY_REF pointed by NODE_PTR by inserting
   the equivalent sums and multiplication.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_array_ref (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree base, index;
  tree base_type;
  tree t1, t2;
  tree tmp_var, tmp_stmt;

  gcc_assert (TREE_CODE (node) == ARRAY_REF);

  base = TREE_OPERAND (node, 0);
  index = TREE_OPERAND (node, 1);

  /* The type of base is a pointer, no longer an array! */
  base_type = build0 (POINTER_TYPE, TREE_TYPE (TREE_TYPE (base)));
  layout_type (base_type);

  /* Generate the equivalent gimplified expression */

  t1 = fold_convert (long_integer_type_node, index);
  t1 = force_gimple_operand_bsi (bsi, t1, TRUE, NULL);
  gcc_assert (t1 && t1 != error_mark_node);

  t2 = fold_convert (long_integer_type_node, array_ref_element_size (node));
  t2 = force_gimple_operand_bsi (bsi, t2, TRUE, NULL);
  gcc_assert (t2 && t2 != error_mark_node);

  if (TREE_CODE (t1) == INTEGER_CST
      && (double_int_zero_p (TREE_INT_CST (t1))
          || double_int_one_p (TREE_INT_CST (t1))))
    {
      if (double_int_one_p (TREE_INT_CST (t1)))
        t1 = t2;
    }
  else if (TREE_CODE (t2) == INTEGER_CST
           && (double_int_zero_p (TREE_INT_CST (t2))
               || double_int_one_p (TREE_INT_CST (t2))))
    {
      if (double_int_zero_p (TREE_INT_CST (t2)))
        t1 = t2;
    }
  else
    {
      tmp_var = create_tmp_var (long_integer_type_node, "cilsimp");
      tmp_stmt = build2 (MODIFY_EXPR, long_integer_type_node,
                         tmp_var,
                         fold_build2 (MULT_EXPR, long_integer_type_node,
                                      t1,
                                      t2));
      SET_EXPR_LOCATION (tmp_stmt, locus);
      t1 = tmp_var;
      bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);
    }
  gcc_assert (t1 && t1 != error_mark_node);

  tmp_var = create_tmp_var (base_type, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, base_type,
                     tmp_var,
                     build1 (ADDR_EXPR, base_type, base));
  gcc_assert (TREE_CODE (base) != CALL_EXPR);
  SET_EXPR_LOCATION (tmp_stmt, locus);
  t2 = tmp_var;
  bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);

  if (TREE_CODE (t1) == INTEGER_CST
      && double_int_zero_p (TREE_INT_CST (t1)))
    {
      t1 = t2;
    }
  else
    {
      tmp_var = create_tmp_var (base_type, "cilsimp");
      tmp_stmt = build2 (MODIFY_EXPR, base_type,
                         tmp_var,
                         build2 (PLUS_EXPR, base_type,
                                 t2,
                                 t1));
      SET_EXPR_LOCATION (tmp_stmt, locus);
      t1 = tmp_var;
      bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);
    }
  gcc_assert (t1 && t1 != error_mark_node);

  /* Update the current node */
  *node_ptr = build1 (INDIRECT_REF, TREE_TYPE (node), t1);
}

/* Expand the COMPONENT_REF (pointed by NODE_PTR) accessing
   a BIT_FIELD_DECL and being on a right-hand side by transforming it
   into an INDIRECT_REF and applying the necessary bit mask operations.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_rhs_bitfield_component_ref (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree obj = TREE_OPERAND (node, 0);
  tree fld = TREE_OPERAND (node, 1);
  tree fld_type, fld_off ;
  unsigned int cont_size, bfld_size, bfld_off;
  tree new_type, obj_ptr_type;
  tree tmp_var, tmp_stmt;
  tree t;
  HOST_WIDEST_INT off;

  gcc_assert (TREE_CODE (node) == COMPONENT_REF);
  gcc_assert (DECL_BIT_FIELD (fld));

  /* Extract bit field layout */
  fld_type = DECL_BIT_FIELD_TYPE (fld);
  fld_off = DECL_FIELD_OFFSET (fld);
  cont_size = TREE_INT_CST_LOW (TYPE_SIZE (fld_type));
  bfld_size = TYPE_PRECISION (TREE_TYPE (fld));
  bfld_off = TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (fld)) & (cont_size - 1);
  gcc_assert (cont_size >= TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (node))));

  /* Build the type corresponding of a pointer to the object */
  obj_ptr_type = build0 (POINTER_TYPE, TREE_TYPE (obj));
  layout_type (obj_ptr_type);

  /* Build the new type for the equivalent access */
  new_type = get_unsigned_integer_type (cont_size);

  /* Build the (gimplified) equivalent expression */

  tmp_var = create_tmp_var (obj_ptr_type, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, obj_ptr_type,
                     tmp_var,
                     build1 (ADDR_EXPR, obj_ptr_type, obj));
  gcc_assert (TREE_CODE (obj) != CALL_EXPR);
  SET_EXPR_LOCATION (tmp_stmt, locus);
  t = tmp_var;
  bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);

  off = TREE_INT_CST_LOW (fld_off)
        + ((TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (fld))
            & ~(cont_size - 1))
           / 8);

  gcc_assert (TREE_INT_CST_HIGH (fld_off) == 0);

  if (off > 0)
    {
      tmp_var = create_tmp_var (obj_ptr_type, "cilsimp");
      tmp_stmt = build2 (MODIFY_EXPR, obj_ptr_type,
                         tmp_var,
                         build2 (PLUS_EXPR, obj_ptr_type,
                                 t,
                                 build_int_cst (long_integer_type_node, off)));
      SET_EXPR_LOCATION (tmp_stmt, locus);
      t = tmp_var;
      bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);
    }

  tmp_var = create_tmp_var (new_type, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, new_type,
                     tmp_var,
                     build1 (INDIRECT_REF, new_type, t));
  SET_EXPR_LOCATION (tmp_stmt, locus);
  t = tmp_var;
  bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);

  if (cont_size > bfld_size + bfld_off)
    {
      tmp_var = create_tmp_var (new_type, "cilsimp");
      tmp_stmt = build2 (MODIFY_EXPR, new_type,
                         tmp_var,
                         build2 (LSHIFT_EXPR, new_type,
                                 t,
                                 build_int_cstu (uint32_type,
                                                 cont_size
                                                 - bfld_size - bfld_off)));
      SET_EXPR_LOCATION (tmp_stmt, locus);
      t = tmp_var;
      bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);
    }

  if (cont_size > bfld_size)
    {
      tmp_var = create_tmp_var (new_type, "cilsimp");
      tmp_stmt = build2 (MODIFY_EXPR, new_type,
                         tmp_var,
                         build2 (RSHIFT_EXPR, new_type,
                                 t,
                                 build_int_cstu (uint32_type,
                                                 cont_size - bfld_size)));
      SET_EXPR_LOCATION (tmp_stmt, locus);
      t = tmp_var;
      bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);
    }

  gcc_assert (t && t != error_mark_node);

  /* Update the current node */
  *node_ptr = fold_convert (TREE_TYPE (node), t);
}

/* Expand the COMPONENT_REF (pointed by NODE_PTR) accessing
   a BIT_FIELD_DECL and being on a left-hand side by transforming it
   into an INDIRECT_REF and applying the necessary bit mask operations.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_lhs_bitfield_component_ref (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree obj = TREE_OPERAND (node, 0);
  tree fld = TREE_OPERAND (node, 1);
  tree fld_type, fld_off ;
  unsigned int cont_size, bfld_size, bfld_off;
  tree stmt = bsi_stmt (*bsi), rhs, addr;
  tree new_type, obj_ptr_type;
  tree tmp_var, tmp_stmt;
  tree t;
  HOST_WIDEST_INT off;
  unsigned HOST_WIDEST_INT mask = 0;

  gcc_assert (TREE_CODE (node) == COMPONENT_REF);
  gcc_assert (DECL_BIT_FIELD (fld));
  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR
              && TREE_OPERAND (stmt, 0) == node);

  /* Extract bit field layout */
  fld_type = DECL_BIT_FIELD_TYPE (fld);
  fld_off = DECL_FIELD_OFFSET (fld);
  cont_size = TREE_INT_CST_LOW (TYPE_SIZE (fld_type));
  bfld_size = TYPE_PRECISION (TREE_TYPE (fld));
  bfld_off = TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (fld)) & (cont_size - 1);
  gcc_assert (cont_size >= TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (node))));

  /* Build the new type for the equivalent access */
  new_type = get_unsigned_integer_type (cont_size);

  /* Build the type corresponding of a pointer to the object */
  obj_ptr_type = build0 (POINTER_TYPE, TREE_TYPE (obj));
  layout_type (obj_ptr_type);

  /* Convert the original rhs into the new type */
  gcc_assert (TREE_CODE (TREE_OPERAND (stmt, 1)) == VAR_DECL);
  tmp_var = create_tmp_var (new_type, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, new_type,
                     tmp_var,
                     build1 (NOP_EXPR, new_type, TREE_OPERAND (stmt, 1)));
  SET_EXPR_LOCATION (tmp_stmt, locus);
  rhs = tmp_var;
  bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);

  /* Shift the rhs in order that it is in the right bit position */
  if (cont_size > bfld_size)
    {
      tmp_var = create_tmp_var (new_type, "cilsimp");
      tmp_stmt = build2 (MODIFY_EXPR, new_type,
                         tmp_var,
                         build2 (LSHIFT_EXPR, new_type,
                                 rhs,
                                 build_int_cstu (uint32_type,
                                                 cont_size - bfld_size)));
      SET_EXPR_LOCATION (tmp_stmt, locus);
      rhs = tmp_var;
      bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);
    }
  if (cont_size > bfld_size + bfld_off)
    {
      /* Remark: new_type is unsigned by construction:
                 therefore, the right shift doesn't introduce any 1 */

      tmp_var = create_tmp_var (new_type, "cilsimp");
      tmp_stmt = build2 (MODIFY_EXPR, new_type,
                         tmp_var,
                         build2 (RSHIFT_EXPR, new_type,
                                 rhs,
                                 build_int_cstu (uint32_type,
                                                 cont_size
                                                 - bfld_size - bfld_off)));
      SET_EXPR_LOCATION (tmp_stmt, locus);
      rhs = tmp_var;
      bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);
    }

  /* Build expression to compute the address to be accessed */
  tmp_var = create_tmp_var (obj_ptr_type, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, obj_ptr_type,
                     tmp_var,
                     build1 (ADDR_EXPR, obj_ptr_type, obj));
  gcc_assert (TREE_CODE (obj) != CALL_EXPR);
  SET_EXPR_LOCATION (tmp_stmt, locus);
  t = tmp_var;
  bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);

  off = TREE_INT_CST_LOW (fld_off)
        + ((TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (fld))
            & ~(cont_size - 1))
           / 8);

  gcc_assert (TREE_INT_CST_HIGH (fld_off) == 0);

  if (off > 0)
    {
      tmp_var = create_tmp_var (obj_ptr_type, "cilsimp");
      tmp_stmt = build2 (MODIFY_EXPR, obj_ptr_type,
                         tmp_var,
                         build2 (PLUS_EXPR, obj_ptr_type,
                                 t,
                                 build_int_cst (long_integer_type_node, off)));
      SET_EXPR_LOCATION (tmp_stmt, locus);
      t = tmp_var;
      bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);
    }
  addr = t;

  /* Read existing value at the address just computed */
  tmp_var = create_tmp_var (new_type, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, new_type,
                     tmp_var,
                     build1 (INDIRECT_REF, new_type, t));
  SET_EXPR_LOCATION (tmp_stmt, locus);
  t = tmp_var;
  bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);

  /* Compute the mask to be applied to the existing value */
  gcc_assert (HOST_BITS_PER_WIDEST_INT >= 64);
  mask |= (1 << (cont_size - bfld_size - bfld_off)) - 1;
  mask <<= bfld_off + bfld_size;
  mask |= (1 << bfld_off) - 1;

  /* Apply the mask to the existing value */
  tmp_var = create_tmp_var (new_type, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, new_type,
                     tmp_var,
                     build2 (BIT_AND_EXPR, new_type,
                             t,
                             build_int_cstu (new_type, mask)));
  SET_EXPR_LOCATION (tmp_stmt, locus);
  t = tmp_var;
  bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);

  /* Compute the new value for the rhs of the current statement */
  tmp_var = create_tmp_var (new_type, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, new_type,
                     tmp_var,
                     build2 (BIT_IOR_EXPR, new_type,
                             rhs,
                             t));
  SET_EXPR_LOCATION (tmp_stmt, locus);
  rhs = tmp_var;
  bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);

  /* Update the current statement (and the current node) */
  *node_ptr = build1 (INDIRECT_REF, new_type, addr);
  TREE_OPERAND (stmt, 1) = rhs;
}

/* Expand the INIT_EXPR (or MODIFY_EXPR) in NODE having
   a CONSTRUCTOR or STRING_CST on the right side into a sequence
   of simpler (here, it means "not involving CONSTRUCTOR or
   STRING_CST nodes) initializer statements.
   Beware that, differently from most simplification functions,
   this expansion leaves GIMPLE nodes that need further simplifications;
   hence, this function should only be called during
   the pre-simplification.
   BSI points to the iterator of the statement that contains NODE
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.   */

static void
pre_simp_init (block_stmt_iterator *bsi, tree node)
{
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree lhs = TREE_OPERAND (node, 0);
  tree rhs = TREE_OPERAND (node, 1);
  tree stmt_list = 0;
  block_stmt_iterator tmp_bsi;
  tree_stmt_iterator i;

  gcc_assert (bsi_stmt (*bsi) == node);
  gcc_assert (TREE_CODE (node) == INIT_EXPR
              || TREE_CODE (node) == MODIFY_EXPR);
  gcc_assert (TREE_CODE (rhs) == CONSTRUCTOR || TREE_CODE (rhs) == STRING_CST);

  /* Expand the constructor into a separate statement list */
  expand_init_to_stmt_list (lhs, rhs, &stmt_list, FALSE);
  gcc_assert (TREE_CODE (stmt_list) == STATEMENT_LIST);

  /* Gimplify the new statements and insert them */
  tmp_bsi = *bsi;
  for (i = tsi_start (stmt_list); !tsi_end_p (i); tsi_next (&i))
    {
      tree stmt;

      /* Gimplify the new statement */
      gimplify_stmt (tsi_stmt_ptr (i));

      /* Insert the new statements before the old */
      stmt = tsi_stmt (i);
      if (TREE_CODE (stmt) == STATEMENT_LIST)
	{
          tree_stmt_iterator j;

          for (j = tsi_start (stmt); !tsi_end_p (j); tsi_next (&j))
            {
              tree t = tsi_stmt (j);

              SET_EXPR_LOCATION (t, locus);
              bsi_insert_after (&tmp_bsi, t, BSI_NEW_STMT);
            }
	}
      else if (stmt != NULL)
        {
          SET_EXPR_LOCATION (stmt, locus);
          bsi_insert_after (&tmp_bsi, stmt, BSI_NEW_STMT);
        }
    }

  /* Remove the old statement */
  bsi_remove (bsi, false);

  /* Update the basic block statement iterator */
  *bsi = tmp_bsi;
}

/* Make sure that the tree pointed by NODE_PTR is a VAR_DECL.
   In case, split the statement containing NODE_PTR into two
   by creating a new local variable.
   BSI points to the iterator of the statement that contains NODE_PTR
   (in order to allow insertion of new statements).   */

static void
split_use (block_stmt_iterator bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (bsi));
  tree type = TREE_TYPE (node);
  tree var, stmt;

  if (TREE_CODE (node) == VAR_DECL)
    return;

  /* Split the current statement by creating a new local variable */
  var = create_tmp_var (type, "cilsimp");
  stmt = build2 (MODIFY_EXPR, type, var, node);
  SET_EXPR_LOCATION (stmt, locus);
  bsi_insert_before (&bsi, stmt, BSI_SAME_STMT);
  *node_ptr = var;
}

/* Return TRUE if EXP contains mostly (3/4)  zeros.  */
static bool
mostly_zeros_p (tree exp)
{
  HOST_WIDE_INT nz_elts, count, elts;
  bool must_clear;

  gcc_assert (TREE_CODE (exp) == CONSTRUCTOR);

  categorize_ctor_elements (exp, &nz_elts, &count, &must_clear);

  if (must_clear)
    return TRUE;

  elts = count_type_elements (TREE_TYPE (exp), false);

  return (nz_elts < elts / 4);
}

/* Return TRUE if EXP contains all zeros. */
static bool
all_zeros_p (tree exp)
{
  HOST_WIDE_INT nz_elts, count;
  bool must_clear;

  gcc_assert (TREE_CODE (exp) == CONSTRUCTOR);

  categorize_ctor_elements (exp, &nz_elts, &count, &must_clear);

  return (nz_elts == 0);
}

/* Expand the initialization of tree DECL to tree INIT
   into the statement list pointed by STMT_LIST.
   Beware that statements inserted into the list cannot be assumed
   to be in GIMPLE form and/or simplified for CIL.
   If GIMPLE CIL-simplified statements are required, explicit
   gimplification and CIL simplification have to be performed on them.
   CLEARED tells whether unmentioned fields in the initializer
   statement may be considered already initialized to zero or not.
   The expansion is especially meant to expand a CONSTRUCTOR into
   an equivalent statement sequence; anyway, any initialization
   is properly handled: in case of no expansion, a simple MODIFY_EXPR
   is appended to STMT_LIST.
   STMT_LIST may be NULL; in this case a statement list is allocated.
*/

void
expand_init_to_stmt_list (tree decl, tree init, tree *stmt_list, bool cleared)
{
  tree decl_size = TYPE_SIZE_UNIT (TREE_TYPE (decl));
  unsigned int size = TREE_INT_CST_LOW (decl_size);
  bool need_to_clear = FALSE;

  if (! *stmt_list)
    {
      *stmt_list = alloc_stmt_list ();
    }

  gcc_assert (TREE_CODE (*stmt_list) == STATEMENT_LIST);

  if (TREE_CODE (init) == CONST_DECL)
    {
      init = DECL_INITIAL (init);
      gcc_assert (init && init != error_mark_node);
    }

  if (! cleared && TREE_CODE (init) == CONSTRUCTOR && all_zeros_p (init))
    {
      tree args, t, decl_ptr;

      args = tree_cons (NULL, decl_size, NULL);
      args = tree_cons (NULL, integer_zero_node, args);
      decl_ptr = build_fold_addr_expr (decl);
      args = tree_cons (NULL, decl_ptr, args);
      t = implicit_built_in_decls[BUILT_IN_MEMSET];
      t = build_function_call_expr (t, args);

      append_to_statement_list (t, stmt_list);

      return;
    }

  switch (TREE_CODE (init))
    {
    case STRING_CST:
      {
        tree args, t, to_ptr, from_ptr;

        gcc_assert (TREE_CODE (TREE_TYPE (init)) == ARRAY_TYPE);

        args = tree_cons (NULL, decl_size, NULL);

        from_ptr = build_fold_addr_expr (init);
        args = tree_cons (NULL, from_ptr, args);

        to_ptr = build_fold_addr_expr (decl);
        args = tree_cons (NULL, to_ptr, args);

        /* We know they do not overlap */
        t = implicit_built_in_decls[BUILT_IN_MEMCPY];
        t = build_function_call_expr (t, args);

        append_to_statement_list (t, stmt_list);
      }
    break;

    case CONSTRUCTOR:
      switch (TREE_CODE (TREE_TYPE (init)))
        {
        case RECORD_TYPE:
        case UNION_TYPE:
        case QUAL_UNION_TYPE:
          {
            unsigned HOST_WIDE_INT idx;
            tree init_type = TREE_TYPE (init);
            tree field, value;

            /* If size is zero or the target is already cleared, do nothing */
            if (size == 0 || cleared)
              {
                need_to_clear = FALSE;
                cleared = TRUE;
              }

            /* We either clear the aggregate or indicate the value is dead.  */
            else if ((TREE_CODE (init_type) == UNION_TYPE
                      || TREE_CODE (init_type) == QUAL_UNION_TYPE)
                     && ! CONSTRUCTOR_ELTS (init))
              /* If the constructor is empty, clear the union.  */
              need_to_clear = TRUE;

            /* If the constructor has fewer fields than the structure or
               if we are initializing the structure to mostly zeros, clear
               the whole structure first. */
            else if (size > 0
                     && (((int)VEC_length (constructor_elt,
                                           CONSTRUCTOR_ELTS (init))
                          != fields_length (init_type))
                         || mostly_zeros_p (init)))
              need_to_clear = TRUE;

            if (need_to_clear && size > 0)
              {
                tree args, t, decl_ptr;

                args = tree_cons (NULL, decl_size, NULL);
                args = tree_cons (NULL, integer_zero_node, args);
                decl_ptr = build_fold_addr_expr (decl);
                args = tree_cons (NULL, decl_ptr, args);
                t = implicit_built_in_decls[BUILT_IN_MEMSET];
                t = build_function_call_expr (t, args);

                append_to_statement_list (t, stmt_list);

                cleared = TRUE;
              }

            /* Store each element of the constructor into the
               corresponding field of TARGET.  */
            FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (init), idx,
                                      field, value)
              {
                tree ltarget;

                /* Just ignore missing fields.  We cleared the whole
                   structure, above, if any fields are missing.  */
                if (field == 0)
                  continue;

                if (cleared && initializer_zerop (value))
                  continue;

                ltarget = build3 (COMPONENT_REF, TREE_TYPE (field), decl,
                                  field, NULL);

                expand_init_to_stmt_list (ltarget, value, stmt_list, cleared);
              }
          }
          break;

        case ARRAY_TYPE:
          {
            tree value, index;
            unsigned HOST_WIDE_INT i;
            tree domain;
            tree elttype = TREE_TYPE (TREE_TYPE (init));
            int const_bounds_p;
            HOST_WIDE_INT minelt = 0;
            HOST_WIDE_INT maxelt = 0;

            domain = TYPE_DOMAIN (TREE_TYPE (init));
            const_bounds_p = (TYPE_MIN_VALUE (domain)
                              && TYPE_MAX_VALUE (domain)
                              && host_integerp (TYPE_MIN_VALUE (domain), 0)
                              && host_integerp (TYPE_MAX_VALUE (domain), 0));

            /* If we have constant bounds for the range
               of the type, get them.  */
            if (const_bounds_p)
              {
                minelt = tree_low_cst (TYPE_MIN_VALUE (domain), 0);
                maxelt = tree_low_cst (TYPE_MAX_VALUE (domain), 0);
              }

            /* If the constructor has fewer elements than the array, clear
               the whole array first. */
            if (cleared)
              need_to_clear = FALSE;
            else
              {
                unsigned HOST_WIDE_INT idx;
                tree index, value;
                HOST_WIDE_INT count = 0;
                HOST_WIDE_INT zero_count = 0;
                need_to_clear = ! const_bounds_p;

                /* This loop is a more accurate version of the loop in
                   mostly_zeros_p (it handles RANGE_EXPR in an index).  It
                   is also needed to check for missing elements.  */
                FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (init), idx,
                                          index, value)
                  {
                    HOST_WIDE_INT this_node_count;

                    if (need_to_clear)
                      break;

                    if (index != NULL_TREE && TREE_CODE (index) == RANGE_EXPR)
                      {
                        tree lo_index = TREE_OPERAND (index, 0);
                        tree hi_index = TREE_OPERAND (index, 1);

                        if (! host_integerp (lo_index, 1)
                            || ! host_integerp (hi_index, 1))
                          {
                            need_to_clear = TRUE;
                            break;
                          }

                        this_node_count = tree_low_cst (hi_index, 1)
                            - tree_low_cst (lo_index, 1) + 1;
                      }
                    else
                      this_node_count = 1;

                    count += this_node_count;
                    if (TREE_CODE (value) == CONSTRUCTOR
                        && mostly_zeros_p (value))
                      zero_count += this_node_count;
                  }

                /* Clear the entire array first if there are any missing
                   elements, or if the incidence of zero elements is >=
                   75%.  */
                if (! need_to_clear
                    && (count < maxelt - minelt + 1
                        || 4 * zero_count >= 3 * count))
                  need_to_clear = TRUE;
              }

            if (need_to_clear && size > 0)
              {
                tree args, t, decl_ptr;

                args = tree_cons (NULL, decl_size, NULL);
                args = tree_cons (NULL, integer_zero_node, args);
                decl_ptr = build_fold_addr_expr (decl);
                args = tree_cons (NULL, decl_ptr, args);
                t = implicit_built_in_decls[BUILT_IN_MEMSET];
                t = build_function_call_expr (t, args);

                append_to_statement_list (t, stmt_list);

                cleared = TRUE;
              }

            /* Store each element of the constructor into the
               corresponding element of TARGET, determined by counting the
               elements.  */
            FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (init), i, index, value)
              {
                enum machine_mode mode;
                HOST_WIDE_INT bitsize;
                int unsignedp;
                tree t;

                if (initializer_zerop (value))
                  continue;

                unsignedp = TYPE_UNSIGNED (elttype);
                mode = TYPE_MODE (elttype);

                if (mode == BLKmode)
                  bitsize = (host_integerp (TYPE_SIZE (elttype), 1)
                             ? tree_low_cst (TYPE_SIZE (elttype), 1)
                             : -1);
                else
                  bitsize = GET_MODE_BITSIZE (mode);

                gcc_assert (index == NULL_TREE
                            || TREE_CODE (index) != RANGE_EXPR);

                if (minelt)
                  index = fold_convert (ssizetype,
                                        fold_build2 (MINUS_EXPR,
                                                     TREE_TYPE (index),
                                                     index,
                                                     TYPE_MIN_VALUE (domain)));

                t = build4 (ARRAY_REF, elttype, decl, index, NULL, NULL);
                expand_init_to_stmt_list (t, value, stmt_list, cleared);
              }
          }
          break;

        case VECTOR_TYPE:
          fprintf (stderr, "CIL: Cannot handle rhs: ");
          debug_generic_expr (init);
          gcc_assert (0);
          break;

        default:
          gcc_unreachable ();
          break;
        }
      break;

    default:
      append_to_statement_list (build2 (MODIFY_EXPR, TREE_TYPE (decl),
                                        decl, init),
                                stmt_list);
      break;
    }
}

/* Rename a single variable using the specified suffix */

static void
rename_var (tree var, const char* suffix)
{
  const char *orig_name = IDENTIFIER_POINTER (DECL_NAME (var));
  char *newsym = alloca (strlen (orig_name) + strlen (suffix) + 10 + 1);
  DECL_NAME (var) = get_identifier_with_length (newsym,
                                                sprintf (newsym,
                                                         "%s%s%lu",
                                                         orig_name,
                                                         suffix,
                                                         (unsigned long)var));
}

/* Simplify variables: rename inlined variables
   rename and globalize function static variables
   inline init for local variables.   */

static void
simp_vars (void)
{
  block_stmt_iterator bsi = bsi_start (ENTRY_BLOCK_PTR);
  tree *p = &cfun->unexpanded_var_list;

  for (; *p; )
    {
      tree var  = TREE_VALUE (*p);
      tree init = DECL_INITIAL (var);

      if (TREE_STATIC (var) && DECL_CONTEXT (var) != 0)
        {
          rename_var (var, "?fs");
          DECL_CONTEXT (var) = 0;
        }

      if (DECL_FROM_INLINE (var) && DECL_NAME (var) != NULL)
        rename_var (var, "?in");

      if (!TREE_STATIC (var) && init && init != error_mark_node)
        {
          /* DECL_INITIAL (var) = NULL_TREE; */
          bsi_insert_after (&bsi,
                            build2 (INIT_EXPR, TREE_TYPE (var), var, init),
                            BSI_NEW_STMT);
        }

      p = &TREE_CHAIN (*p);
    }
}

/* Simplify pass that makes CIL emission easier. */

static unsigned int
simp_cil (void)
{
  basic_block bb;
  block_stmt_iterator bsi;

  res_var = NULL_TREE;
  uint32_type = get_unsigned_integer_type (32);

  simp_vars ();

  /* Some pre-simplification is needed for INIT_EXPR and MODIFY_EXPR:
     *) in their expansion, it's sometimes convenient to generate ARRAY_REF
        nodes, which require further simplification.
     *) if the lhs is a bitfield COMPONENT_REF, then the rhs must be
        a VAR_DECL because the following simplification pass expects
        that.   */
  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
          tree stmt = bsi_stmt (bsi);

          if (TREE_CODE (stmt) == INIT_EXPR || TREE_CODE (stmt) == MODIFY_EXPR)
            {
              tree lhs = TREE_OPERAND (stmt, 0);
              tree rhs = TREE_OPERAND (stmt, 1);

              if (TREE_CODE (rhs) == CONSTRUCTOR
                  || TREE_CODE (rhs) == STRING_CST)
                pre_simp_init (&bsi, stmt);
              else if (TREE_CODE (lhs) == COMPONENT_REF
                       && DECL_BIT_FIELD (TREE_OPERAND (lhs, 1)))
                split_use (bsi, &TREE_OPERAND (stmt, 1));
            }
        }
    }

  /* Statement simplification loop.
     At this point, the code is still in proper GIMPLE form,
     but with no constructors nor string initializers.   */
  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
          tree *stmt_ptr = bsi_stmt_ptr (bsi);
          simp_cil_node (&bsi, stmt_ptr);
          bb = bb_for_stmt (*stmt_ptr);
        }
    }

#if 0
  FOR_EACH_BB (bb)
    {
      dump_generic_bb (stdout, bb, 4, 0);
    }
#endif

  return 0;
}

/* Gate function of CIL simplify pass. */

static bool
simp_cil_gate (void)
{
  return current_function_decl != NULL;
}

/* Define the parameters of the tree-simp-CIL pass.  */

struct tree_opt_pass pass_simp_cil =
{
  "simpcil",                            /* name */
  simp_cil_gate,			/* gate */
  simp_cil,			        /* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_REST_OF_COMPILATION,		/* tv_id */
  PROP_cfg,                     	/* properties_required */
  0,					/* properties_provided */
  /* ??? If TER is enabled, we also kill gimple.  */
  0,    				/* properties_destroyed */
  0,
  0,
  0					/* letter */
};

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
