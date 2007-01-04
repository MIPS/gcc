/* Simplify GENERIC trees before CIL emission.

   Copyright (C) 2006, 2007 Free Software Foundation, Inc.

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

   *) Expansion of COND_EXPR nodes used as expressions (not statements).
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

   *) Expansion of BIT_FIELD_REF nodes.
      CIL has no direct support for bit-field access; hence,
      equivalent code that extracts the bit pattern and applies the
      appropriate bit mask is generated.
      Memory access is performed by using INDIRECT_REF nodes.

   *) Expansion of TARGET_MEM_REF nodes.
      Emission of such nodes is not difficult in gen_cil pass;
      however, a previous expansion may trigger further optimizations
      (since there is no similar construct in CIL bytecodes).

   *) Expansion of ARRAY_REF nodes with non-zero indexes into
      ARRAY_REF with zero indexes.
      Emission of such nodes is not difficult in gen_cil pass;
      however, a previous expansion may generate better code (i.e.:
      it may fold constants) or trigger further optimizations
      (CIL arrays cannot be used for C-style arrays).
      Remark that such a simplification must keep ARRAY_REFs,
      they cannot be replaced by INDIRECT_REF nodes in order
      not to break strict aliasing.

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

   *) Forcing arguments of CALL_EXPRs to be local variables, only for
      specific built-in functions.
      A few built-in functions require special simplifications
      in order to make their emission easier; in particular:
      *) the 1st argument of BUILT_IN_VA_START, BUILT_IN_VA_END and
         CIL32_BUILTIN_VA_ARG has to be a variable or an ADDR_EXPR node.
         More in general, this is true any time va_list is passed
         as a parameter.
      *) the 1st argument of BUILT_IN_VA_COPY has to be a local variable
         (the emitted CIL uses a 'stloc' to store its value).
      To force arguments of calls to be local variables, new local
      variables are generated.

   *) Expansion of UNEQ_EXPR, UNLE_EXPR and UNGE_EXPR nodes.
      CIL instruction set has some support for unordered comparisons,
      but it is not orthogonal. Whenever an unordered comparison
      is difficult to be translated in CIL, it is expanded by this pass.
      While this always happens for UNEQ_EXPRs, there is a case in which
      UNLE_EXPRs and UNGE_EXPRs are kept.

   *) Expansion of LTGT_EXPR nodes.
      There is no equivalent in CIL instruction set and it is more
      convenient not to require the CIL emission pass to handle it.

   *) Inversion of targets for statements with COND_EXPR nodes
      in which the goto target is fallthru.
      This isn't strictly necessary, but it helps the CIL emission pass
      to avoid the generation of a branch opcode.

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

   *) Ensure that there is always a return statement
      Even in case the block ends with a call to a noreturn function
*/

/* Local functions, macros and variables.  */
static bool is_copy_required (tree);
static bool mostly_zeros_p (tree);
static bool all_zeros_p (tree);
static void simp_cond_stmt (block_stmt_iterator, tree);
static void rescale_out_edge_probabilities (basic_block, int);
static void simp_switch (block_stmt_iterator *, tree *);
static void simp_trivial_switch (block_stmt_iterator *, tree *);
static void simp_builtin_call (block_stmt_iterator, tree);
static void simp_abs (block_stmt_iterator *, tree *);
static void simp_min_max (block_stmt_iterator *, tree *);
static void simp_cond_expr (block_stmt_iterator *, tree *);
static void simp_unordered_comp_expr (block_stmt_iterator *, tree *);
static void simp_ltgt_expr (block_stmt_iterator *, tree *);
static void simp_rotate (block_stmt_iterator *, tree *);
static void simp_shift (block_stmt_iterator *, tree);
static void simp_target_mem_ref (block_stmt_iterator *, tree *);
static void compute_array_ref_base_disp (tree, tree *, tree *);
static void simp_array_ref (block_stmt_iterator *, tree *);
static void simp_bitfield (block_stmt_iterator *, tree *, tree, unsigned int,
                           unsigned int, unsigned int, HOST_WIDEST_INT, bool);
static void simp_rhs_bitfield_component_ref (block_stmt_iterator *, tree *);
static void simp_lhs_bitfield_component_ref (block_stmt_iterator *, tree *);
static void simp_bitfield_ref (block_stmt_iterator *, tree *);
static void pre_simp_init (block_stmt_iterator *, tree);
static void simp_cil_node (block_stmt_iterator *, tree *);
static void split_use (block_stmt_iterator, tree *, bool);
static void rename_var (tree, const char*, unsigned long);
static void simp_vars (void);
static unsigned int simp_cil (void);
static bool simp_cil_gate (void);

#define UPDATE_ADDRESSABLE(NODE)  \
do { tree _node = (NODE); \
     for (; handled_component_p (_node); _node = TREE_OPERAND (_node, 0)) \
       ; \
     if (TREE_CODE (_node) == VAR_DECL || TREE_CODE (_node) == PARM_DECL) \
       TREE_ADDRESSABLE (_node) = 1; } while (0)

static tree res_var;

/* Return the integer type with size BITS bits.
   The type is unsigned or signed depending on UNS.   */

tree
get_integer_type (int bits, bool uns)
{
  if (uns)
    {
      switch (bits)
        {
        case 8:   return unsigned_intQI_type_node;
        case 16:  return unsigned_intHI_type_node;
        case 32:  return unsigned_intSI_type_node;
        case 64:  return unsigned_intDI_type_node;
        case 128: return unsigned_intTI_type_node;
        default:
          gcc_assert (0);
          return NULL_TREE;
        }
    }
  else
    {
      switch (bits)
        {
        case 8:   return intQI_type_node;
        case 16:  return intHI_type_node;
        case 32:  return intSI_type_node;
        case 64:  return intDI_type_node;
        case 128: return intTI_type_node;
        default:
          gcc_assert (0);
          return NULL_TREE;
        }
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
    return;

  switch (TREE_CODE (node))
    {
    case COND_EXPR:
      if (bsi_stmt (*bsi) == node)
        {
          tree cond = COND_EXPR_COND (node);

          /* UNLE_EXPR and UNGE_EXPR nodes are usually simplified.
             This is the exception, in this case it is better
             to keep them, since there is a convenient CIL translation.   */
          if (TREE_CODE (cond) == UNLE_EXPR || TREE_CODE (cond) == UNGE_EXPR)
            {
              simp_cil_node (bsi, &TREE_OPERAND (cond, 0));
              simp_cil_node (bsi, &TREE_OPERAND (cond, 1));
            }
          else
            simp_cil_node (bsi, &COND_EXPR_COND (node));

          simp_cond_stmt (*bsi, node);
        }
      else
        {
          simp_cil_node (bsi, &COND_EXPR_COND (node));
          simp_cil_node (bsi, &COND_EXPR_THEN (node));
          simp_cil_node (bsi, &COND_EXPR_ELSE (node));
          simp_cond_expr (bsi, node_ptr);
        }
      break;

    case SWITCH_EXPR:
      simp_cil_node (bsi, &SWITCH_COND (node));
      simp_switch (bsi, node_ptr);
      break;

    case CALL_EXPR:
      {
        tree args = TREE_OPERAND (node, 1);
        tree fun_expr;
        tree dfun = NULL_TREE;

        simp_cil_node (bsi, &TREE_OPERAND (node, 0));

        while (args)
          {
            simp_cil_node (bsi, &TREE_VALUE (args));
            args = TREE_CHAIN (args);
          }

        fun_expr = TREE_OPERAND (node, 0);
        if (TREE_CODE (fun_expr) == ADDR_EXPR
            && TREE_CODE (TREE_OPERAND (fun_expr, 0)) == FUNCTION_DECL)
          dfun = TREE_OPERAND (fun_expr, 0);

        /* Calls to some built-in functions require ad-hoc simplifications */
        if (dfun && DECL_BUILT_IN (dfun))
          simp_builtin_call (*bsi, node);
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
    case UNLT_EXPR:
    case UNGT_EXPR:
    case UNORDERED_EXPR:
    case ORDERED_EXPR:
    case EXACT_DIV_EXPR:
    case TRUNC_DIV_EXPR:
    case TRUNC_MOD_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &TREE_OPERAND (node, 1));
      break;

    case LTGT_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &TREE_OPERAND (node, 1));
      simp_ltgt_expr (bsi, node_ptr);
      break;

    case UNLE_EXPR:
    case UNGE_EXPR:
    case UNEQ_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &TREE_OPERAND (node, 1));
      simp_unordered_comp_expr (bsi, node_ptr);
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
        split_use (*bsi, &TREE_OPERAND (node, 1), false);
      break;

    case NEGATE_EXPR:
    case BIT_NOT_EXPR:
    case TRUTH_NOT_EXPR:
    case CONVERT_EXPR:
    case NOP_EXPR:
    case FLOAT_EXPR:
    case FIX_TRUNC_EXPR:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      break;

    case ADDR_EXPR:
      if (TREE_CODE (TREE_OPERAND (node, 0)) == ARRAY_REF)
        {
          tree *t = &TREE_OPERAND (node, 0);
          bool non_zero_indexes = false;

          /* Simplify operands (of nested ARRAY_REFs as well) */
          do {
            simp_cil_node (bsi, &TREE_OPERAND (*t, 1));
            if (! integer_zerop (TREE_OPERAND (*t, 1)))
              non_zero_indexes = true;
            t = &TREE_OPERAND (*t, 0);
          } while (TREE_CODE (*t) == ARRAY_REF);
          simp_cil_node (bsi, t);

          /* Reduce the ARRAY_REF to a zero-index array access */
          if (non_zero_indexes)
            {
              simp_array_ref (bsi, &TREE_OPERAND (node, 0));
              *node_ptr = TREE_OPERAND (node, 0);
              /* The current node may require further simplification */
              simp_cil_node (bsi, node_ptr);
            }
          else
            recompute_tree_invariant_for_addr_expr (node);
        }
      else
        {
          simp_cil_node (bsi, &TREE_OPERAND (node, 0));
          if (AGGREGATE_TYPE_P (TREE_TYPE (TREE_OPERAND (node, 0)))
              && TREE_CODE (TREE_OPERAND (node, 0)) == CALL_EXPR)
            split_use (*bsi, &TREE_OPERAND (node, 0), false);
          recompute_tree_invariant_for_addr_expr (node);
        }
      break;

    case INDIRECT_REF:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      if (AGGREGATE_TYPE_P (TREE_TYPE (node))
          && TREE_CODE (TREE_OPERAND (node, 0)) == CALL_EXPR)
        split_use (*bsi, &TREE_OPERAND (node, 0), false);
      break;

    case COMPONENT_REF:
      gcc_assert (TREE_CODE (TREE_OPERAND (node, 1)) == FIELD_DECL);
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      if (AGGREGATE_TYPE_P (TREE_TYPE (TREE_OPERAND (node, 0)))
          && TREE_CODE (TREE_OPERAND (node, 0)) == CALL_EXPR)
        split_use (*bsi, &TREE_OPERAND (node, 0), false);
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

    case BIT_FIELD_REF:
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      if (AGGREGATE_TYPE_P (TREE_TYPE (TREE_OPERAND (node, 0)))
          && TREE_CODE (TREE_OPERAND (node, 0)) == CALL_EXPR)
        split_use (*bsi, &TREE_OPERAND (node, 0), false);
      gcc_assert (TREE_CODE (bsi_stmt (*bsi)) != MODIFY_EXPR
                  || TREE_OPERAND (bsi_stmt (*bsi), 0) != node);
      simp_bitfield_ref (bsi, node_ptr);
      break;

    case TARGET_MEM_REF:
      simp_cil_node (bsi, &TMR_SYMBOL (node));
      simp_cil_node (bsi, &TMR_BASE (node));
      simp_cil_node (bsi, &TMR_INDEX (node));
      simp_target_mem_ref (bsi, node_ptr);
      /* The current node may require further simplification */
      simp_cil_node (bsi, node_ptr);
      break;

    case ARRAY_REF:
      {
        tree *t = node_ptr;
        bool non_zero_indexes = false;

        /* Simplify operands (of nested ARRAY_REFs as well) */
        do {
          simp_cil_node (bsi, &TREE_OPERAND (*t, 1));
          if (! integer_zerop (TREE_OPERAND (*t, 1)))
            non_zero_indexes = true;
          t = &TREE_OPERAND (*t, 0);
        } while (TREE_CODE (*t) == ARRAY_REF);
        simp_cil_node (bsi, t);

        /* Reduce the ARRAY_REF to a zero-index array access */
        if (non_zero_indexes)
          {
            simp_array_ref (bsi, node_ptr);
            *node_ptr = build1 (INDIRECT_REF, TREE_TYPE (node), *node_ptr);
            /* The current node may require further simplification */
            simp_cil_node (bsi, node_ptr);
          }
      }
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

/* Invert the targets of the COND_EXPR pointed by NODE, when
   it is legal and there is a benefit in doing so.
   The only case in which this is currently done is when
   there is a comparison between integral or pointer types and
   the goto target is fallthough.
   BSI is an iterator of the statement that contains NODE.   */

static void
simp_cond_stmt (block_stmt_iterator bsi, tree node)
{
  basic_block bb = bb_for_stmt (bsi_stmt (bsi));
  tree cond_expr, then_expr;
  enum tree_code cond_code, rev_code;
  tree cond_type;

  /* The condition targets are lowered in gimplify.c, we should never have
     COND_EXPR with 'then' or 'else' operands that aren't GOTO_EXPRs.   */
  gcc_assert (TREE_CODE (node) == COND_EXPR
              && TREE_CODE (COND_EXPR_THEN (node)) == GOTO_EXPR
              && TREE_CODE (COND_EXPR_ELSE (node)) == GOTO_EXPR);
  gcc_assert (bsi_stmt (bsi) == node);

  cond_expr = COND_EXPR_COND (node);
  then_expr = COND_EXPR_THEN (node);

  /* Nothing to do if the condition is not a comparison */
  if (! COMPARISON_CLASS_P (cond_expr))
    return;

  /* Do something only when the condition can be inverted */
  cond_code = TREE_CODE (cond_expr);
  cond_type = TREE_TYPE (TREE_OPERAND (cond_expr, 0));
  rev_code = invert_tree_comparison (cond_code, FLOAT_TYPE_P (cond_type));
  if (rev_code != ERROR_MARK
      && label_to_block (GOTO_DESTINATION (then_expr)) == bb->next_bb)
    {
      edge e;

      /* Invert the targets of the COND_EXPR */
      TREE_SET_CODE (COND_EXPR_COND (node), rev_code);
      COND_EXPR_THEN (node) = COND_EXPR_ELSE (node);
      COND_EXPR_ELSE (node) = then_expr;

      /* Invert the out-going edges */
      e = EDGE_SUCC (bb, 0);
      e->flags ^= (EDGE_TRUE_VALUE | EDGE_FALSE_VALUE);
      e = EDGE_SUCC (bb, 1);
      e->flags ^= (EDGE_TRUE_VALUE | EDGE_FALSE_VALUE);
    }
}

/* Rescale the probabilities of the out-edge of basic block BB
   by assuming TOT_PROB as the sum of the current probabilities.   */

static void
rescale_out_edge_probabilities (basic_block bb, int tot_prob)
{
  edge e;
  edge_iterator ei;

  if (tot_prob <= 0)
    {
      EDGE_SUCC (bb, 0)->probability = REG_BR_PROB_BASE;
      ei = ei_start (bb->succs);
      ei_next (&ei);
      for (; (e = ei_safe_edge (ei)); ei_next (&ei))
        e->probability = 0;
    }
  else if (tot_prob != REG_BR_PROB_BASE)
    {
      int scale = (65536 * REG_BR_PROB_BASE) / tot_prob;

      FOR_EACH_EDGE (e, ei, bb->succs)
        {
          e->probability = (e->probability * scale) / 65536;
          gcc_assert (e->probability <= REG_BR_PROB_BASE);
        }
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
  int bb1_probs, bb2_probs;

  /* The switch body is lowered in gimplify.c, we should never have
     switches with a non-NULL SWITCH_BODY here.  */
  gcc_assert (TREE_CODE (node) == SWITCH_EXPR && vec && !SWITCH_BODY (node));

  vec_len = TREE_VEC_LENGTH (vec);

  /* Switches made of one case are always separately handled
     (they are transformed into if ... then ... else ...).   */
  if (vec_len == 2)
    {
      simp_trivial_switch (bsi, node_ptr);
      return;
    }

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

  /* For each out-going edge of the original switch basic block,
     mark whether it is going to be replicated in the first newly
     created switch block, in the second, or in both.
     This is needed in order to update counts, frequencies and
     probabilities.
     In order to do that, use the following edge flags:
     EDGE_FALSE_VALUE: 1st block
     EDGE_TRUE_VALUE:  2nd block
     This is fine because these edges are removed later on.   */
  bb_orig_sw = bb_for_stmt (bsi_stmt (*bsi));
  for (i = 0; i < vec_len; ++i)
    {
      tree elt = TREE_VEC_ELT (vec, i);
      basic_block target_bb = label_to_block (CASE_LABEL (elt));
      edge e = find_edge (bb_orig_sw, target_bb);

      gcc_assert (e);
      e->flags &= ~(EDGE_TRUE_VALUE | EDGE_FALSE_VALUE);
    }
  for (i = 0; i < vec_len; ++i)
    {
      tree elt = TREE_VEC_ELT (vec, i);
      basic_block target_bb = label_to_block (CASE_LABEL (elt));
      edge e = find_edge (bb_orig_sw, target_bb);

      gcc_assert (e);
      if (i <= sw1_last)
        e->flags |= EDGE_FALSE_VALUE;
      else if (i != vec_len - 1)
        e->flags |= EDGE_TRUE_VALUE;
      else
        e->flags |= (EDGE_TRUE_VALUE | EDGE_FALSE_VALUE);
    }

  /* Build a COND_EXPR, replace the switch with the COND_EXPR */
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
      edge e = find_edge (bb1, target_bb);

      if (!e)
        {
          edge old_e = find_edge (bb_orig_sw, target_bb);

          e = make_edge (bb1, target_bb, 0);
          gcc_assert (e);
          gcc_assert (old_e && (old_e->flags & EDGE_FALSE_VALUE) != 0);
          if (old_e->flags & EDGE_TRUE_VALUE)
            {
              e->count = old_e->count / 2;
              e->probability = old_e->probability / 2;
            }
          else
            {
              e->count = old_e->count;
              e->probability = old_e->probability;
            }
        }

      TREE_VEC_ELT (vec1, i) = elt;
    }

  /* Build the case labels for the 2nd new switch and the out-edges
     of its basic block.    */
  for (; i < vec_len - 1; ++i)
    {
      tree elt = TREE_VEC_ELT (vec, i);
      basic_block target_bb = label_to_block (CASE_LABEL (elt));
      edge e = find_edge (bb2, target_bb);

      if (!e)
        {
          edge old_e = find_edge (bb_orig_sw, target_bb);

          e = make_edge (bb2, target_bb, 0);
          gcc_assert (e);
          gcc_assert (old_e && (old_e->flags & EDGE_TRUE_VALUE) != 0);
          if (old_e->flags & EDGE_FALSE_VALUE)
            {
              e->count = old_e->count - old_e->count / 2;
              e->probability = old_e->probability - old_e->probability / 2;
            }
          else
            {
              e->count = old_e->count;
              e->probability = old_e->probability;
            }
        }

      TREE_VEC_ELT (vec2, i - sw1_last - 1) = elt;
    }

  /* Build the default labels for both new switches and the out-edges
     of their basic blocks.   */
    {
      tree elt = TREE_VEC_ELT (vec, vec_len - 1);
      tree new_elt;
      basic_block target_bb = label_to_block (CASE_LABEL (elt));
      edge e1 = find_edge (bb1, target_bb);
      edge e2 = find_edge (bb2, target_bb);
      edge old_e = NULL;

      if (!e1 || !e2)
        {
          old_e = find_edge (bb_orig_sw, target_bb);
          gcc_assert (old_e
                      && (old_e->flags & EDGE_TRUE_VALUE) != 0
                      && (old_e->flags & EDGE_FALSE_VALUE) != 0);
        }

      if (!e1)
        {
          e1 = make_edge (bb1, target_bb, 0);
          gcc_assert (e1);
          e1->count = old_e->count / 2;
          e1->probability = old_e->probability / 2;
        }

      if (!e2)
        {
          e2 = make_edge (bb2, target_bb, 0);
          gcc_assert (e2);
          e2->count = old_e->count - old_e->count / 2;
          e2->probability = old_e->probability - old_e->probability / 2;
        }

      TREE_VEC_ELT (vec1, sw1_last + 1) = elt;

      /* Duplicate this case label expression, since it is used
         in the first switch.   */
      new_elt = build3 (CASE_LABEL_EXPR, TREE_TYPE (elt),
                        CASE_LOW (elt), CASE_HIGH (elt), CASE_LABEL (elt));
      TREE_VEC_ELT (vec2, vec_len - sw1_last - 2) = new_elt;
    }

  /* Re-scale probabilities of the out-going edges of the new blocks
     and compute counts of such blocks.   */
  bb1_probs = 0;
  bb1->count = 0;
  FOR_EACH_EDGE (tmp_edge, ei, bb1->succs)
    {
      bb1->count += tmp_edge->count;
      bb1_probs += tmp_edge->probability;
    }
  rescale_out_edge_probabilities (bb1, bb1_probs);

  bb2_probs = 0;
  bb2->count = 0;
  FOR_EACH_EDGE (tmp_edge, ei, bb2->succs)
    {
      bb2->count += tmp_edge->count;
      bb2_probs += tmp_edge->probability;
    }
  rescale_out_edge_probabilities (bb2, bb2_probs);
  gcc_assert (bb1->count + bb2->count == bb_orig_sw->count);

  /* Update out-edges of original switch basic block */
  for (ei = ei_start (bb_orig_sw->succs); (tmp_edge = ei_safe_edge (ei)); )
    {
      remove_edge (tmp_edge);
    }
  e1 = unchecked_make_edge (bb_orig_sw, bb1, EDGE_FALSE_VALUE);
  e2 = unchecked_make_edge (bb_orig_sw, bb2, EDGE_TRUE_VALUE);
  if (bb1_probs + bb2_probs > 0)
    e1->probability = ((bb1_probs * 256 * REG_BR_PROB_BASE) /
                       (bb1_probs + bb2_probs)) / 256;
  else
    e1->probability = REG_BR_PROB_BASE / 2;
  e1->count = bb1->count;
  e2->probability = REG_BR_PROB_BASE - e1->probability;
  e2->count = bb2->count;
  bb1->frequency = EDGE_FREQUENCY (e1);
  bb2->frequency = EDGE_FREQUENCY (e2);
}

/* Expand the SWITCH_EXPR pointed by NODE_PTR, composed of just
   one case, into a COND_EXPR (or fallthru).
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
     If so, SWITCH_EXPR is replaced by a fallthru.   */
  if (single_succ_p (bb_sw))
    {
      edge e;

      gcc_assert (bb_case
                  == label_to_block (CASE_LABEL (TREE_VEC_ELT (SWITCH_LABELS (node), 1))));

      /* Update CFG */
      e = find_edge (bb_sw, bb_case);
      e->flags |= EDGE_FALLTHRU;

      /* Remove the original switch */
      bsi_remove (bsi, true);

      /* Update the basic block statement iterator */
      *bsi = bsi_last (bb_sw);
      *node_ptr = bsi_stmt (*bsi);
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

/* Force specific arguments of the CALL_EXPR to a built-in function
   pointed by NODE to be local variables.
   Which arguments are forced depend on the built-in function.
   BSI is the iterator of the statement that contains NODE
   (in order to allow insertion of new statements).   */

static void
simp_builtin_call (block_stmt_iterator bsi, tree node)
{
  tree fun_expr = TREE_OPERAND (node, 0);
  tree dfun = TREE_OPERAND (fun_expr, 0);

  gcc_assert (TREE_CODE (node) == CALL_EXPR);
  gcc_assert (TREE_CODE (fun_expr) == ADDR_EXPR);
  gcc_assert (TREE_CODE (dfun) == FUNCTION_DECL);
  gcc_assert (DECL_BUILT_IN (dfun));

  if (DECL_BUILT_IN_CLASS (dfun) == BUILT_IN_MD)
    {
      switch (DECL_FUNCTION_CODE (dfun))
        {
        case CIL32_BUILTIN_VA_ARG:
          {
            tree va = TREE_VALUE (TREE_OPERAND (node, 1));

            gcc_assert (POINTER_TYPE_P (TREE_TYPE (va))
                        && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va)))
                           == va_list_type_node);

            if (TREE_CODE (va) != ADDR_EXPR && TREE_CODE (va) != VAR_DECL)
              split_use (bsi, &TREE_VALUE (TREE_OPERAND (node, 1)), true);
          }
          break;

        default:
            ;
        }
    }
  else
    {
      switch (DECL_FUNCTION_CODE (dfun))
        {
        case BUILT_IN_VA_START:
        case BUILT_IN_VA_END:
          {
            tree va = TREE_VALUE (TREE_OPERAND (node, 1));

            gcc_assert (POINTER_TYPE_P (TREE_TYPE (va))
                        && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va)))
                           == va_list_type_node);

            if (TREE_CODE (va) != ADDR_EXPR && TREE_CODE (va) != VAR_DECL)
              split_use (bsi, &TREE_VALUE (TREE_OPERAND (node, 1)), true);
          }
          break;

        case BUILT_IN_VA_COPY:
          {
            tree va_dest = TREE_VALUE (TREE_OPERAND (node, 1));

            gcc_assert (TREE_CODE (va_dest) == ADDR_EXPR);
            if (TREE_CODE (TREE_OPERAND (va_dest, 0)) != VAR_DECL
                || DECL_FILE_SCOPE_P (TREE_OPERAND (va_dest, 0)))
              split_use (bsi, &TREE_OPERAND (va_dest, 0), true);
            recompute_tree_invariant_for_addr_expr (va_dest);
          }
          break;

        default:
            ;
        }
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
  edge tmp_edge, edge_comp_neg, edge_comp_sel;
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
  bb_sel->frequency = bb_comp->frequency;
  remove_edge (tmp_edge);
  bb_neg = create_empty_bb (bb_comp);
  bb_neg->count = count / 2;
  edge_comp_neg = unchecked_make_edge (bb_comp, bb_neg, EDGE_FALSE_VALUE);
  edge_comp_neg->probability = REG_BR_PROB_BASE / 2;
  edge_comp_sel = unchecked_make_edge (bb_comp, bb_sel, EDGE_TRUE_VALUE);
  edge_comp_sel->probability = REG_BR_PROB_BASE - edge_comp_neg->probability;
  make_single_succ_edge (bb_neg, bb_sel, EDGE_FALLTHRU);
  bb_neg->frequency = EDGE_FREQUENCY (edge_comp_neg);

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
  edge tmp_edge, edge_comp_op0, edge_comp_op1;
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
  bb_sel->frequency = bb_comp->frequency;
  remove_edge (tmp_edge);
  bb_op0 = create_empty_bb (bb_comp);
  bb_op1 = create_empty_bb (bb_op0);
  bb_op0->count = count / 2;
  bb_op1->count = count - bb_op0->count;
  edge_comp_op0 = unchecked_make_edge (bb_comp, bb_op0, EDGE_TRUE_VALUE);
  edge_comp_op0->probability = REG_BR_PROB_BASE / 2;
  make_single_succ_edge (bb_op0, bb_sel, EDGE_FALLTHRU);
  edge_comp_op1 = unchecked_make_edge (bb_comp, bb_op1, EDGE_FALSE_VALUE);
  edge_comp_op1->probability = REG_BR_PROB_BASE - edge_comp_op0->probability;
  make_single_succ_edge (bb_op1, bb_sel, EDGE_FALLTHRU);
  bb_op0->frequency = EDGE_FREQUENCY (edge_comp_op0);
  bb_op1->frequency = EDGE_FREQUENCY (edge_comp_op1);

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

/* Remove the COND_EXPR pointed by NODE_PTR by inserting
   explicit control flow.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted,
   new basic blocks created...
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_cond_expr (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree then_op, else_op;
  tree label_decl_then = create_artificial_label ();
  tree label_decl_else = create_artificial_label ();
  tree label_then, label_else;
  tree sel_var;
  tree orig_stmt, cmp_stmt, asn_then_stmt, asn_else_stmt;
  basic_block bb_comp, bb_then, bb_else, bb_sel;
  edge tmp_edge, edge_comp_then, edge_comp_else;
  block_stmt_iterator tmp_bsi;
  gcov_type count;

  gcc_assert (TREE_CODE (node) == COND_EXPR);
  gcc_assert (node != bsi_stmt (*bsi));

  /* Make sure that the two operands have no side effects */
  then_op = COND_EXPR_THEN (node);
  if (is_copy_required (then_op))
    {
      tree var = create_tmp_var (TREE_TYPE (then_op), "cilsimp");
      tree stmt = build2 (MODIFY_EXPR, TREE_TYPE (then_op), var, then_op);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      COND_EXPR_THEN (node) = var;
      then_op = var;
    }
  else_op = COND_EXPR_ELSE (node);
  if (is_copy_required (else_op))
    {
      tree var = create_tmp_var (TREE_TYPE (else_op), "cilsimp");
      tree stmt = build2 (MODIFY_EXPR, TREE_TYPE (else_op), var, else_op);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      COND_EXPR_ELSE (node) = var;
      else_op = var;
    }

  /* Insert the comparison statement */
  cmp_stmt = build3 (COND_EXPR, void_type_node,
                     COND_EXPR_COND (node),
                     build1 (GOTO_EXPR, void_type_node, label_decl_then),
                     build1 (GOTO_EXPR, void_type_node, label_decl_else));
  SET_EXPR_LOCATION (cmp_stmt, locus);
  bsi_insert_before (bsi, cmp_stmt, BSI_SAME_STMT);

  /* Update cfg */
  orig_stmt = bsi_stmt (*bsi);
  bb_comp = bb_for_stmt (orig_stmt);
  count = bb_comp->count;
  tmp_edge = split_block (bb_comp, cmp_stmt);
  bb_sel = tmp_edge->dest;
  bb_sel->count = count;
  bb_sel->frequency = bb_comp->frequency;
  remove_edge (tmp_edge);
  bb_then = create_empty_bb (bb_comp);
  bb_else = create_empty_bb (bb_then);
  bb_then->count = count / 2;
  bb_else->count = count - bb_then->count;
  edge_comp_then = unchecked_make_edge (bb_comp, bb_then, EDGE_TRUE_VALUE);
  edge_comp_then->probability = REG_BR_PROB_BASE / 2;
  make_single_succ_edge (bb_then, bb_sel, EDGE_FALLTHRU);
  edge_comp_else = unchecked_make_edge (bb_comp, bb_else, EDGE_FALSE_VALUE);
  edge_comp_else->probability = REG_BR_PROB_BASE - edge_comp_then->probability;
  make_single_succ_edge (bb_else, bb_sel, EDGE_FALLTHRU);
  bb_then->frequency = EDGE_FREQUENCY (edge_comp_then);
  bb_else->frequency = EDGE_FREQUENCY (edge_comp_else);

  /* Insert labels and statements into then bb */
  sel_var = create_tmp_var (TREE_TYPE (node), "cilsimp");
  label_then = build1 (LABEL_EXPR, void_type_node, label_decl_then);
  asn_then_stmt = build2 (MODIFY_EXPR, TREE_TYPE (then_op), sel_var, then_op);
  SET_EXPR_LOCATION (asn_then_stmt, locus);
  tmp_bsi = bsi_start (bb_then);
  bsi_insert_after (&tmp_bsi, label_then, BSI_NEW_STMT);
  bsi_insert_after (&tmp_bsi, asn_then_stmt, BSI_SAME_STMT);

  /* Insert labels and statements into else bb */
  label_else = build1 (LABEL_EXPR, void_type_node, label_decl_else);
  asn_else_stmt = build2 (MODIFY_EXPR, TREE_TYPE (else_op), sel_var, else_op);
  SET_EXPR_LOCATION (asn_else_stmt, locus);
  tmp_bsi = bsi_start (bb_else);
  bsi_insert_after (&tmp_bsi, label_else, BSI_NEW_STMT);
  bsi_insert_after (&tmp_bsi, asn_else_stmt, BSI_SAME_STMT);

  /* Update current node in order to use the select variable */
  *node_ptr = sel_var;

  /* Update the basic block statement iterator */
  gcc_assert (bsi_stmt (bsi_start (bb_sel)) == orig_stmt);
  *bsi = bsi_start (bb_sel);
}

/* Simplify the unordered comparison expression pointed by NODE_PTR
   by expanding it with an equivalent expression based on UNORDERED_EXPR
   and TRUTH_OR_EXPR nodes.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_unordered_comp_expr (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree op0, op1;
  enum tree_code comp_code;
  tree t;

  gcc_assert (TREE_CODE (node) == UNEQ_EXPR
              || TREE_CODE (node) == UNLE_EXPR
              || TREE_CODE (node) == UNGE_EXPR);

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

  switch (TREE_CODE (node))
    {
    case UNEQ_EXPR: comp_code = EQ_EXPR; break;
    case UNLE_EXPR: comp_code = LE_EXPR; break;
    case UNGE_EXPR: comp_code = GE_EXPR; break;
    default:
      gcc_unreachable ();
    }

  /* Build and gimplify the equivalent expression */
  t = build2 (TRUTH_OR_EXPR, TREE_TYPE (node),
              fold_build2 (comp_code, TREE_TYPE (node),
                           op0,
                           op1),
              fold_build2 (UNORDERED_EXPR, TREE_TYPE (node),
                           op0,
                           op1));
  t = force_gimple_operand_bsi (bsi, t, TRUE, NULL);

  /* Update the current node */
  *node_ptr = t;
}

/* Simplify the LTGT_EXPR pointed by NODE_PTR by expanding it with
   the equivalent expression based on LT_EXPR, GT_EXPR and
   TRUTH_OR_EXPR nodes.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_ltgt_expr (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree op0, op1;
  tree t;

  gcc_assert (TREE_CODE (node) == LTGT_EXPR);

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

  /* Build and gimplify the equivalent expression */
  t = build2 (TRUTH_OR_EXPR, TREE_TYPE (node),
              fold_build2 (LT_EXPR, TREE_TYPE (node),
                           op0,
                           op1),
              fold_build2 (GT_EXPR, TREE_TYPE (node),
                           op0,
                           op1));
  t = force_gimple_operand_bsi (bsi, t, TRUE, NULL);

  /* Update the current node */
  *node_ptr = t;
}

/* Remove the LROTATE_EXPR or RROTATE_EXPR pointed by NODE_PTR
   by inserting shifts and bit operations.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.
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
  op1 = fold_convert (unsigned_intSI_type_node, TREE_OPERAND (node, 1));

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
                    fold_build2 (MINUS_EXPR, unsigned_intSI_type_node,
                                 fold_convert (unsigned_intSI_type_node,
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
   BSI points to the iterator of the statement that contains NODE
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.   */

static void
simp_shift (block_stmt_iterator *bsi, tree node)
{
  tree t;

  gcc_assert (TREE_CODE (node) == LSHIFT_EXPR
              || TREE_CODE (node) == RSHIFT_EXPR);

  /* Generate the type conversion */
  t = fold_convert (unsigned_intSI_type_node, TREE_OPERAND (node, 1));

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

/* Given NODE of code ARRAY_REF:
   -) in BASE, return a tree of the ARRAY_REF that accesses
      the element of the array with all zero indexes;
   -) in DISP, return a tree with the computation of the
      displacement from the element with all zero indexes
      to that accessed by NODE.   */

static void
compute_array_ref_base_disp (tree node, tree *base, tree *disp)
{
  tree op0 = TREE_OPERAND (node, 0);
  tree op1 = TREE_OPERAND (node, 1);
  tree t1, t2;
  tree inner_base, inner_disp = NULL;

  gcc_assert (TREE_CODE (node) == ARRAY_REF);

  if (TREE_CODE (op0) == ARRAY_REF)
    compute_array_ref_base_disp (op0, &inner_base, &inner_disp);
  else
    inner_base = op0;

  *base = build4 (ARRAY_REF, TREE_TYPE (node),
                  inner_base, integer_zero_node, NULL, NULL);

  t1 = fold_convert (long_integer_type_node, op1);
  t2 = fold_convert (long_integer_type_node, array_ref_element_size (node));
  /* Folding a multiplication having a comparison as first operand
     may result into a COND_EXPR node, which must not be reintroduced.   */
  if (COMPARISON_CLASS_P (op1))
    *disp = build2 (MULT_EXPR, long_integer_type_node, t1, t2);
  else
    *disp = fold_build2 (MULT_EXPR, long_integer_type_node, t1, t2);

  if (inner_disp)
    *disp = fold_build2 (PLUS_EXPR, long_integer_type_node,
                         inner_disp,
                         *disp);
}

/* Simplify the ARRAY_REF pointed by NODE_PTR with the adress accesed
   by an equivalent ARRAY_REF with zero-indexes and the necessary
   sums and multiplications.
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
  tree array_start, array_disp;
  tree ptr_type;
  tree t1, t2, stmt;

  gcc_assert (TREE_CODE (node) == ARRAY_REF);

  /* Compute the first element of the array being accessed and
     the displacement of the element being accessed from it.   */
  compute_array_ref_base_disp (node, &array_start, &array_disp);

  /* Update addressable information */
  UPDATE_ADDRESSABLE (array_start);

  /* Build type pointer to the array element */
  ptr_type = build_pointer_type (TREE_TYPE (node));

  /* Build the expression for the adress of the first array element */
  t1 = build1 (ADDR_EXPR, ptr_type, array_start);
  recompute_tree_invariant_for_addr_expr (t1);
  t1 = force_gimple_operand_bsi (bsi, t1, FALSE, NULL);
  t2 = create_tmp_var (ptr_type, "cilsimp");
  stmt = build2 (MODIFY_EXPR, ptr_type, t2, t1);
  SET_EXPR_LOCATION (stmt, locus);
  bsi_insert_before (bsi, stmt, BSI_SAME_STMT);

  /* Build the expression for the access to the array element */
  t1 = fold_build2 (PLUS_EXPR, ptr_type,
                    t2,
                    fold_convert (ptr_type, array_disp));
  t1 = force_gimple_operand_bsi (bsi, t1, TRUE, NULL);

  /* Update the current node */
  *node_ptr = t1;
}

/* Expand a bit-field reference by transforming it
   into an INDIRECT_REF and applying the necessary bit mask operations.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.
   NODE is passed by reference because simplification requires
   replacing the node.
   OBJ is the object containing the bit-field.
   CONT_SIZE is the number of bits of the bit-field container.
   BFLD_SIZE is the number of bits being referenced.
   BFLD_OFF is the position of the first referenced bit.
   OFF is an additional offset in bytes of the bit-field from the
   beginning of the OBJ.
   UNS tells whether the bit-field is unsigned or not.   */

static void
simp_bitfield (block_stmt_iterator *bsi, tree *node_ptr,
               tree obj, unsigned int cont_size, unsigned int bfld_size,
               unsigned int bfld_off, HOST_WIDEST_INT off, bool uns)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree new_type, new_type_ptr, obj_ptr_type;
  tree tmp_var, tmp_stmt;
  tree t;

  gcc_assert (cont_size >= bfld_size + bfld_off);

  /* Set that the object being accessed is addressable */
  UPDATE_ADDRESSABLE (obj);

  /* Build the type corresponding of a pointer to the object */
  obj_ptr_type = build_pointer_type (TREE_TYPE (obj));

  /* Build the new type for the equivalent access (and a pointer type to it) */
  new_type = get_integer_type (cont_size, uns);
  new_type_ptr = build_pointer_type (new_type);

  /* Build the (gimplified) equivalent expression */

  tmp_var = create_tmp_var (obj_ptr_type, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, obj_ptr_type,
                     tmp_var,
                     build1 (ADDR_EXPR, obj_ptr_type, obj));
  gcc_assert (TREE_CODE (obj) != CALL_EXPR);
  SET_EXPR_LOCATION (tmp_stmt, locus);
  t = tmp_var;
  bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);
  tmp_var = create_tmp_var (new_type_ptr, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, new_type_ptr,
                     tmp_var,
                     build1 (NOP_EXPR, new_type_ptr, t));
  SET_EXPR_LOCATION (tmp_stmt, locus);
  t = tmp_var;
  bsi_insert_before (bsi, tmp_stmt, BSI_SAME_STMT);

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
                                 build_int_cstu (unsigned_intSI_type_node,
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
                                 build_int_cstu (unsigned_intSI_type_node,
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
  tree obj = TREE_OPERAND (node, 0);
  tree fld = TREE_OPERAND (node, 1);
  tree fld_type, fld_off;
  unsigned int cont_size, bfld_size, bfld_off;
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
  gcc_assert (TREE_INT_CST_HIGH (fld_off) == 0);
  off = TREE_INT_CST_LOW (fld_off)
        + ((TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (fld))
            & ~(cont_size - 1))
           / 8);

  /* Simplify the bit-field */
  simp_bitfield (bsi, node_ptr, obj, cont_size, bfld_size, bfld_off, off,
                 DECL_UNSIGNED (fld));
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
  tree new_type, new_type_ptr, obj_ptr_type;
  tree tmp_var, tmp_stmt;
  tree t;
  HOST_WIDEST_INT off;
  unsigned HOST_WIDEST_INT mask = 0;

  gcc_assert (TREE_CODE (node) == COMPONENT_REF);
  gcc_assert (DECL_BIT_FIELD (fld));
  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR
              && TREE_OPERAND (stmt, 0) == node);

  /* Set that the object being accessed is addressable */
  UPDATE_ADDRESSABLE (obj);

  /* Extract bit field layout */
  fld_type = DECL_BIT_FIELD_TYPE (fld);
  fld_off = DECL_FIELD_OFFSET (fld);
  cont_size = TREE_INT_CST_LOW (TYPE_SIZE (fld_type));
  bfld_size = TYPE_PRECISION (TREE_TYPE (fld));
  bfld_off = TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (fld)) & (cont_size - 1);
  gcc_assert (cont_size >= TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (node))));

  /* Build the new type for the equivalent access (and a pointer type to it) */
  new_type = get_integer_type (cont_size, true);
  new_type_ptr = build_pointer_type (new_type);

  /* Build the type corresponding of a pointer to the object */
  obj_ptr_type = build_pointer_type (TREE_TYPE (obj));

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
                                 build_int_cstu (unsigned_intSI_type_node,
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
                                 build_int_cstu (unsigned_intSI_type_node,
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
  tmp_var = create_tmp_var (new_type_ptr, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, new_type_ptr,
                     tmp_var,
                     build1 (NOP_EXPR, new_type_ptr, t));
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
  mask |= (1LL << (cont_size - bfld_size - bfld_off)) - 1LL;
  mask <<= bfld_off + bfld_size;
  mask |= (1LL << bfld_off) - 1;

  /* Apply the mask to the existing value */
  tmp_var = create_tmp_var (new_type, "cilsimp");
  tmp_stmt = build2 (MODIFY_EXPR, new_type,
                     tmp_var,
                     build2 (BIT_AND_EXPR, new_type,
                             t,
                             build_int_cst_wide (new_type,
                                                 mask,
                                                 mask >> HOST_BITS_PER_WIDE_INT)));
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

/* Expand the BIT_FIELD_REF (pointed by NODE_PTR) by transforming it
   into an INDIRECT_REF and applying the necessary bit mask operations.
   BSI points to the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).
   BSI is passed by reference because instructions are inserted.
   NODE is passed by reference because simplification requires
   replacing the node.   */

static void
simp_bitfield_ref (block_stmt_iterator *bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  tree obj = TREE_OPERAND (node, 0);
  unsigned int ref_bfld_off, cont_size, bfld_size, bfld_off;
  HOST_WIDE_INT off;

  gcc_assert (TREE_CODE (node) == BIT_FIELD_REF);

  /* Extract bit field layout */
  bfld_size = TREE_INT_CST_LOW (TREE_OPERAND (node, 1));
  ref_bfld_off = TREE_INT_CST_LOW (TREE_OPERAND (node, 2));
  gcc_assert (bfld_size <= 64);

  /* At least, cont_size is the next power of two of the bit-field size */
  cont_size = bfld_size - 1;
  cont_size |= (cont_size >> 1);
  cont_size |= (cont_size >> 2);
  cont_size |= (cont_size >> 4);
  ++cont_size;
  gcc_assert (cont_size == 8 || cont_size == 16
              || cont_size == 32 || cont_size == 64);

  /* Widen the container until an aligned access is enough */
  while ((ref_bfld_off & ~(cont_size - 1))
         != ((ref_bfld_off + bfld_size - 1) & ~(cont_size - 1)))
    cont_size <<= 1;

  bfld_off = ref_bfld_off & (cont_size - 1);
  off = (ref_bfld_off - bfld_off) >> 3;

  /* Simplify the bit-field */
  simp_bitfield (bsi, node_ptr, obj, cont_size, bfld_size, bfld_off, off,
                 BIT_FIELD_REF_UNSIGNED (node));
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
  expand_init_to_stmt_list (lhs, rhs, &stmt_list);
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
  bsi_remove (bsi, true);

  /* Update the basic block statement iterator */
  *bsi = tmp_bsi;
}

/* Make sure that the tree pointed by NODE_PTR is a VAR_DECL;
   if LOCAL is true, then the VAR_DECL must be a local variable.
   In case, split the statement containing NODE_PTR into two
   by creating a new local variable.
   BSI points to the iterator of the statement that contains NODE_PTR
   (in order to allow insertion of new statements).   */

static void
split_use (block_stmt_iterator bsi, tree *node_ptr, bool local)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (bsi));
  tree type = TREE_TYPE (node);
  tree var, stmt;

  if (TREE_CODE (node) == VAR_DECL
      && (!local || !DECL_FILE_SCOPE_P (node)))
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

static void
expand_init_to_stmt_list1 (tree decl, tree init,
                           tree *stmt_list1, bool cleared,
                           tree *stmt_list2, void *le_image, void *be_image)
{
  tree decl_size = TYPE_SIZE_UNIT (TREE_TYPE (decl));
  unsigned int size = TREE_INT_CST_LOW (decl_size);
  bool need_to_clear = FALSE;

  gcc_assert (TREE_CODE (*stmt_list1) == STATEMENT_LIST);
  gcc_assert (TREE_CODE (*stmt_list2) == STATEMENT_LIST);

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

      append_to_statement_list (t, stmt_list1);

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

        append_to_statement_list (t, stmt_list1);

        memcpy(le_image, TREE_STRING_POINTER (init), TREE_INT_CST_LOW (decl_size));
        memcpy(be_image, TREE_STRING_POINTER (init), TREE_INT_CST_LOW (decl_size));
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

                append_to_statement_list (t, stmt_list1);

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

                ltarget = build3 (COMPONENT_REF, TREE_TYPE (field), decl, field, NULL);

                if (le_image != NULL && !DECL_BIT_FIELD(field))
                  {
                    unsigned HOST_WIDE_INT offset = TREE_INT_CST_LOW (DECL_FIELD_OFFSET(field));
                    unsigned HOST_WIDE_INT bit_offset = TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET(field));
                    gcc_assert(bit_offset%BITS_PER_UNIT==0);
                    offset += bit_offset/BITS_PER_UNIT;

                    expand_init_to_stmt_list1 (ltarget, value,
                                               stmt_list1, cleared,
                                               stmt_list2,
                                               (void*)((intptr_t)le_image+offset),
                                               (void*)((intptr_t)be_image+offset));
                  }
                else
                  {
                    expand_init_to_stmt_list1 (ltarget, value,
                                               stmt_list1, cleared,
                                               stmt_list2, NULL, NULL);
                  }
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

                append_to_statement_list (t, stmt_list1);

                cleared = TRUE;
              }

            /* Store each element of the constructor into the
               corresponding element of TARGET, determined by counting the
               elements.  */
            FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (init), i, index, value)
              {
                tree t;
                tree elsize;

                if (initializer_zerop (value))
                  continue;

                gcc_assert (index == NULL_TREE
                            || TREE_CODE (index) != RANGE_EXPR);

                if (minelt)
                  index = fold_convert (ssizetype,
                                        fold_build2 (MINUS_EXPR,
                                                     TREE_TYPE (index),
                                                     index,
                                                     TYPE_MIN_VALUE (domain)));

                t = build4 (ARRAY_REF, elttype, decl, index, NULL, NULL);

                elsize = array_ref_element_size (t);

                if (le_image != NULL
                    && TREE_CODE (index)  == INTEGER_CST
                    && TREE_CODE (elsize) == INTEGER_CST)
                  {
                    unsigned HOST_WIDE_INT offset = TREE_INT_CST_LOW(index);
                    offset *= TREE_INT_CST_LOW(elsize);

                    expand_init_to_stmt_list1 (t, value,
                                               stmt_list1, cleared,
                                               stmt_list2,
                                               (void*)((intptr_t)le_image+offset),
                                               (void*)((intptr_t)be_image+offset));
                  }
                else
                  {
                    expand_init_to_stmt_list1 (t, value,
                                               stmt_list1, cleared,
                                               stmt_list2, NULL, NULL);
                  }
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

    case INTEGER_CST:
      {
        int type_size = TREE_INT_CST_LOW (decl_size);
        tree t = build2 (MODIFY_EXPR, TREE_TYPE (decl), decl, init);
        append_to_statement_list (t, stmt_list1);

        if (le_image != NULL)
          {
            unsigned int temp  =  TREE_INT_CST_LOW (init);
            unsigned char b[4] = {((temp>> 0)&0xff),
                                  ((temp>> 8)&0xff),
                                  ((temp>>16)&0xff),
                                  ((temp>>24)&0xff)};

            gcc_assert (be_image != NULL);
            switch (type_size)
              {
              case 1:
                *(unsigned char*)le_image = b[0];
                *(unsigned char*)be_image = b[0];
                break;

              case 2:
                *((unsigned char*)le_image+0) = b[0];
                *((unsigned char*)le_image+1) = b[1];
                *((unsigned char*)be_image+0) = b[1];
                *((unsigned char*)be_image+1) = b[0];
                break;

              case 4:
                *((unsigned char*)le_image+0) = b[0];
                *((unsigned char*)le_image+1) = b[1];
                *((unsigned char*)le_image+2) = b[2];
                *((unsigned char*)le_image+3) = b[3];
                *((unsigned char*)be_image+0) = b[3];
                *((unsigned char*)be_image+1) = b[2];
                *((unsigned char*)be_image+2) = b[1];
                *((unsigned char*)be_image+3) = b[0];
                break;

#if 0
              case 8:
                /*...*/
                break;
#endif

              default:
                append_to_statement_list (t, stmt_list2);
                break;
              }
          }
        else
          {
            append_to_statement_list (t, stmt_list2);
          }
      }
      break;

    case REAL_CST:
      /* Missing optimization, fall through for now */
    default:
      {
        tree t = build2 (MODIFY_EXPR, TREE_TYPE (decl), decl, init);
        append_to_statement_list (t, stmt_list1);
        append_to_statement_list (t, stmt_list2);
      }
      break;
    }
}

static int
statement_list_num_instr (tree stmt_list)
{
  int i = 0;
  tree_stmt_iterator it = tsi_start (stmt_list);
  while (!tsi_end_p (it))
    {
      ++i;
      tsi_next (&it);
    }
  return i;
}

void
expand_init_to_stmt_list (tree decl, tree init, tree *stmt_list)
{
  unsigned int size = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (TREE_TYPE (decl)));
  void* le_image = alloca (size);
  void* be_image = alloca (size);
  tree stmt_list1 = alloc_stmt_list ();
  int  num_list1;
  tree stmt_list2 = alloc_stmt_list ();
  int  num_list2;
  bool le_eq_be;

  memset (le_image, 0, size);
  memset (be_image, 0, size);

  expand_init_to_stmt_list1 (decl, init,
                             &stmt_list1, FALSE,
                             &stmt_list2, le_image, be_image);

  le_eq_be = (memcmp (le_image, be_image, size) == 0);
  num_list1 = statement_list_num_instr (stmt_list1);
  num_list2 = statement_list_num_instr (stmt_list2);


  /* Decide what to do */
  if ((num_list2 + 2) < num_list1)
    {
      tree mem_cpy;
      tree args;
      tree from_ptr;
      tree to_ptr = build_fold_addr_expr (decl);

      tree sconst = build_string_literal (size, le_image);

      if (le_eq_be)
        {
          from_ptr = sconst;
        }
      else
        {
          tree sconst2 = build_string_literal (size, be_image);

          gcc_assert (TREE_TYPE (sconst) == TREE_TYPE (sconst2));

          from_ptr = fold_build3 (COND_EXPR,
                                  TREE_TYPE (sconst),
                                  build_function_call_expr (cil32_is_LE_decl, NULL_TREE),
                                  sconst,
                                  sconst2);
        }

      args = tree_cons (NULL, size_int (size), NULL);
      args = tree_cons (NULL, from_ptr, args);
      args = tree_cons (NULL, to_ptr,   args);

      mem_cpy = build_function_call_expr (implicit_built_in_decls[BUILT_IN_MEMCPY],
                                          args);

      append_to_statement_list (mem_cpy,    stmt_list);
      append_to_statement_list (stmt_list2, stmt_list);
    }
  else
    append_to_statement_list (stmt_list1, stmt_list);
}

/* Rename a single variable using the specified suffix */

static void
rename_var (tree var, const char* suffix, unsigned long index)
{
  const char *orig_name = IDENTIFIER_POINTER (DECL_NAME (var));
  char *newsym = alloca (strlen (orig_name) + strlen (suffix) + 10 + 1);
  DECL_NAME (var) = get_identifier_with_length (newsym,
                                                sprintf (newsym,
                                                         "%s%s%lu",
                                                         orig_name,
                                                         suffix,
                                                         index));
}

/* Simplify variables: rename inlined variables
   rename and globalize function static variables
   inline init for local variables.   */

static void
simp_vars (void)
{
  basic_block new_bb = 0;
  block_stmt_iterator bsi;
  tree *p = &cfun->unexpanded_var_list;
  unsigned long num_loc = 0;

  for (; *p; p = &TREE_CHAIN (*p))
    {
      tree var  = TREE_VALUE (*p);
      tree init = DECL_INITIAL (var);

      if (TREE_STATIC (var) && DECL_CONTEXT (var) != 0)
        {
          rename_var (var, "?fs", DECL_UID (var));
          DECL_CONTEXT (var) = 0;
        }

      if (DECL_NAME (var) != NULL && ! TREE_STATIC (var))
        {
          if (DECL_FROM_INLINE (var))
            rename_var (var, "?in", num_loc);
          else
            rename_var (var, "?", num_loc);

          ++num_loc;
        }

      if (!TREE_STATIC (var) && init && init != error_mark_node)
        {
          /* Generate empty basic block after the entry bb, if not done yet */
          if (!new_bb)
            {
              basic_block entry_succ = single_succ (ENTRY_BLOCK_PTR);
              edge e = find_edge (ENTRY_BLOCK_PTR, entry_succ);
              edge new_e;

              new_bb = create_empty_bb (ENTRY_BLOCK_PTR);
              new_bb->count = ENTRY_BLOCK_PTR->count;
              new_bb->frequency = ENTRY_BLOCK_PTR->frequency;
              redirect_edge_pred (e, new_bb);
              new_e = make_single_succ_edge (ENTRY_BLOCK_PTR, new_bb, e->flags);
              new_e->count = e->count;
              new_e->probability = e->probability;
              bsi = bsi_start (new_bb);
            }

          DECL_INITIAL (var) = NULL_TREE;
          bsi_insert_before (&bsi,
                             build2 (MODIFY_EXPR, TREE_TYPE (var), var, init),
                             BSI_SAME_STMT);
        }
    }
}

/* Simplify pass that makes CIL emission easier. */

static unsigned int
simp_cil (void)
{
  basic_block bb;
  block_stmt_iterator bsi;

  res_var = NULL_TREE;

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
                split_use (bsi, &TREE_OPERAND (stmt, 1), false);
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
      if (EDGE_COUNT (bb->succs) == 0)
        {
          tree stmt;
          bsi = bsi_last (bb);
          stmt = bsi_stmt (bsi);
          if (TREE_CODE (stmt) != RETURN_EXPR)
            {
              tree ret_type = TREE_TYPE (DECL_RESULT (current_function_decl));
              tree ret_stmt;

              if (TREE_CODE (ret_type) != VOID_TYPE && res_var == NULL_TREE)
                res_var = create_tmp_var (ret_type, "cilsimp");

              ret_stmt = build1 (RETURN_EXPR, ret_type, res_var);
              bsi_insert_after (&bsi, ret_stmt, BSI_NEW_STMT);
              make_single_succ_edge (bb, EXIT_BLOCK_PTR, EDGE_FALLTHRU);
            }
        }
    }

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
  TODO_dump_func,			/* todo_flags_finish */
  0					/* letter */
};

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
