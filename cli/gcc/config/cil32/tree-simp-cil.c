/* Simplify GENERIC trees before CIL emission.

   Copyright (C) 2006-2007 Free Software Foundation, Inc.

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
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Erven Rohou             <erven.rohou@st.com>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "timevar.h"
#include "toplev.h"
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
         CIL32_BUILT_IN_VA_ARG has to be a variable or an ADDR_EXPR node.
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
static bool mostly_zeros_p (tree);
static bool all_zeros_p (tree);
static void simp_cond_stmt (block_stmt_iterator, tree);
static void simp_builtin_call (block_stmt_iterator, tree *);
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
static void simp_rhs_bitfield_component_ref (block_stmt_iterator *, tree *);
static void simp_lhs_bitfield_component_ref (block_stmt_iterator *, tree *);
static void simp_bitfield_ref (block_stmt_iterator *, tree *);
static void pre_simp_init (block_stmt_iterator *, tree, tree, tree);
static void simp_cil_node (block_stmt_iterator *, tree *);
static void split_use (block_stmt_iterator, tree *, bool);
static void rename_var (tree, const char*, unsigned long);
static void simp_vars (void);
static unsigned int simp_cil_final (void);
static bool simp_cil_gate (void);

#define UPDATE_ADDRESSABLE(NODE)  \
do { tree _node = (NODE); \
     for (; handled_component_p (_node); _node = TREE_OPERAND (_node, 0)) \
       ; \
     if (TREE_CODE (_node) == VAR_DECL || TREE_CODE (_node) == PARM_DECL) \
       TREE_ADDRESSABLE (_node) = 1; } while (0)

static bool simp_final;
static tree res_var;

static GTY(()) tree two = NULL_TREE;
static GTY(()) tree bits_per_unit = NULL_TREE;

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

bool
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

          if (simp_final)
            simp_cond_stmt (*bsi, node);
        }
      else
        {
          simp_cil_node (bsi, &COND_EXPR_COND (node));
          simp_cil_node (bsi, &COND_EXPR_THEN (node));
          simp_cil_node (bsi, &COND_EXPR_ELSE (node));
          if (simp_final)
            simp_cond_expr (bsi, node_ptr);
        }
      break;

    case SWITCH_EXPR:
      simp_cil_node (bsi, &SWITCH_COND (node));
      break;

    case CALL_EXPR:
      {
        unsigned int aidx  = 0;
        unsigned int nargs = call_expr_nargs(node);

        simp_cil_node (bsi, &CALL_EXPR_FN (node));

        for (;aidx<nargs;++aidx)
            simp_cil_node (bsi, &CALL_EXPR_ARG (node, aidx));

        if (simp_final)
          {
            tree fun_expr = CALL_EXPR_FN (node);
            tree dfun = NULL_TREE;

            if (TREE_CODE (fun_expr) == ADDR_EXPR
                && TREE_CODE (TREE_OPERAND (fun_expr, 0)) == FUNCTION_DECL)
              dfun = TREE_OPERAND (fun_expr, 0);

            /* Calls to some built-in funs require ad-hoc simplifications */
            if (dfun && DECL_BUILT_IN (dfun))
              simp_builtin_call (*bsi, node_ptr);
          }
      }
      break;

    case MULT_EXPR:
    case PLUS_EXPR:
    case POINTER_PLUS_EXPR:
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
    case GIMPLE_MODIFY_STMT:
      simp_cil_node (bsi, &GENERIC_TREE_OPERAND (node, 0));
      simp_cil_node (bsi, &GENERIC_TREE_OPERAND (node, 1));
      gcc_assert (TREE_CODE (GENERIC_TREE_OPERAND (node, 1)) != CONSTRUCTOR
                  && TREE_CODE (GENERIC_TREE_OPERAND (node, 1)) != STRING_CST);
      if (AGGREGATE_TYPE_P (TREE_TYPE (GENERIC_TREE_OPERAND (node, 1)))
          && TREE_CODE (GENERIC_TREE_OPERAND (node, 0)) == INDIRECT_REF
          && TREE_CODE (GENERIC_TREE_OPERAND (node, 1)) == CALL_EXPR)
        split_use (*bsi, &GENERIC_TREE_OPERAND (node, 1), false);
      break;

    case NEGATE_EXPR:
    case BIT_NOT_EXPR:
    case TRUTH_NOT_EXPR:
    case CONVERT_EXPR:
    case NOP_EXPR:
    case FLOAT_EXPR:
    case FIX_TRUNC_EXPR:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
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
      if (simp_final && DECL_BIT_FIELD (TREE_OPERAND (node, 1)))
        {
          tree stmt = bsi_stmt (*bsi);

          if ((TREE_CODE (stmt) == MODIFY_EXPR
               || TREE_CODE (stmt) == GIMPLE_MODIFY_STMT)
              && GENERIC_TREE_OPERAND (stmt, 0) == node)
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
      if (simp_final)
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
      if (simp_final
          && !TREE_OPERAND (node, 0)
          && TREE_CODE (TREE_TYPE (DECL_RESULT (current_function_decl)))
          != VOID_TYPE)
        {
          /* Pre-C99 code may contain void-returns for non-void functions.
             In this case, return the result variable.   */

          tree res_type = TREE_TYPE (DECL_RESULT (current_function_decl));
          if (TYPE_SIZE (res_type) != NULL_TREE
              && TREE_CODE (TYPE_SIZE (res_type)) != INTEGER_CST)
            internal_error ("Returned type cannot be a variable size array or struct\n");

          if (res_var == NULL_TREE)
            res_var = create_tmp_var (TREE_TYPE (DECL_RESULT (current_function_decl)),
                                      "cilsimp");

          TREE_OPERAND (node, 0) = res_var;
        }
      simp_cil_node (bsi, &TREE_OPERAND (node, 0));
      break;

    case RESULT_DECL:
      if (simp_final
          && (TREE_CODE (bsi_stmt (*bsi)) != RETURN_EXPR
              || (TREE_CODE (TREE_OPERAND (bsi_stmt (*bsi), 0)) != MODIFY_EXPR
                  && TREE_CODE (TREE_OPERAND (bsi_stmt (*bsi), 0)) != GIMPLE_MODIFY_STMT)))
        {
          tree res_type = TREE_TYPE (node);
          if (TYPE_SIZE (res_type) != NULL_TREE
              && TREE_CODE (TYPE_SIZE (res_type)) != INTEGER_CST)
            internal_error ("Returned type cannot be a variable size array or struct\n");

          if (res_var == NULL_TREE)
            res_var = create_tmp_var (TREE_TYPE (node), "cilsimp");

          *node_ptr = res_var;
        }
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

    case GOTO_EXPR:
      if (TREE_CODE (GOTO_DESTINATION (node)) != LABEL_DECL)
        simp_cil_node (bsi, &GOTO_DESTINATION (node));
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

  gcc_assert (bsi_stmt (bsi) == node);
  gcc_assert (TREE_CODE (node) == COND_EXPR);
  cond_expr = COND_EXPR_COND (node);
  then_expr = COND_EXPR_THEN (node);

  /* Skip the optimization if then and else are not GOTO_EXPR */
  if (then_expr == NULL_TREE || !simple_goto_p (then_expr))
    {
      return;
    }

  gcc_assert (COND_EXPR_ELSE (node) && simple_goto_p (COND_EXPR_ELSE (node)));

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

/* Force specific arguments of the CALL_EXPR to a built-in function
   pointed by NODE_PTR to be local variables.
   Which arguments are forced depend on the built-in function.
   BSI is the iterator of the statement that contains *NODE_PTR
   (in order to allow insertion of new statements).  */

static void
simp_builtin_call (block_stmt_iterator bsi, tree *node_ptr)
{
  tree node = *node_ptr;
  location_t locus = EXPR_LOCATION (bsi_stmt (bsi));
  tree fun_expr;
  tree dfun;

  gcc_assert (TREE_CODE (node) == CALL_EXPR);

  fun_expr = CALL_EXPR_FN (node);
  gcc_assert (TREE_CODE (fun_expr) == ADDR_EXPR);

  dfun = TREE_OPERAND (fun_expr, 0);
  gcc_assert (TREE_CODE (dfun) == FUNCTION_DECL);
  gcc_assert (DECL_BUILT_IN (dfun));

  if (DECL_BUILT_IN_CLASS (dfun) != BUILT_IN_MD)
    {
      switch (DECL_FUNCTION_CODE (dfun))
        {
        case BUILT_IN_VA_START:
          {
            tree va_ref = CALL_EXPR_ARG (node, 0);
            tree va;

            switch (TREE_CODE (va_ref)) {
            case INDIRECT_REF:
            case ADDR_EXPR:
              va = TREE_OPERAND (va_ref, 0);
              break;

            case VAR_DECL:
              va = build1 (INDIRECT_REF, cil32_va_list_type, va_ref);
              break;

            default:
              gcc_assert(0);
              break;
            }

            gcc_assert (POINTER_TYPE_P (TREE_TYPE (va))
                        && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va)))
                           == cil32_arg_iterator_type);

            {
              tree argiter = create_tmp_var (cil32_arg_iterator_type, "arg_iterator");
              location_t locus = EXPR_LOCATION (bsi_stmt (bsi));
              tree stmt = build_gimple_modify_stmt(va, build1 (ADDR_EXPR, cil32_va_list_type, argiter));
              SET_EXPR_LOCATION (stmt, locus);
              bsi_insert_before (&bsi, stmt, BSI_SAME_STMT);
            }

            {
              tree stmt;
              tree new_args;

              new_args = tree_cons (NULL, va, NULL);

              stmt = build_function_call_expr (cil32_builtins[CIL32_BUILT_IN_VA_START],
                                               new_args);
              bsi_replace (&bsi, stmt, true);
            }

          }
          break;

        case BUILT_IN_VA_END:
          {
            tree va_ref = CALL_EXPR_ARG (node, 0);
            tree va;

            switch (TREE_CODE (va_ref)) {
            case INDIRECT_REF:
            case ADDR_EXPR:
              va = TREE_OPERAND (va_ref, 0);
              break;

            case VAR_DECL:
              va = build1 (INDIRECT_REF, cil32_va_list_type, va_ref);
              break;

            default:
              gcc_assert(0);
              break;
            }

            gcc_assert (POINTER_TYPE_P (TREE_TYPE (va))
                        && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va)))
                           == cil32_arg_iterator_type);

            {
              tree stmt;
              tree new_args;

              new_args = tree_cons (NULL, va, NULL);

              stmt = build_function_call_expr (cil32_builtins[CIL32_BUILT_IN_VA_END],
                                               new_args);
              bsi_replace (&bsi, stmt, true);
            }
          }
          break;

        case BUILT_IN_VA_COPY:
          {
            tree va_dest_ref = CALL_EXPR_ARG (node, 0);
            tree va_dest;
            tree va_src = CALL_EXPR_ARG (node, 1);

            switch (TREE_CODE (va_dest_ref)) {
            case INDIRECT_REF:
            case ADDR_EXPR:
              va_dest = TREE_OPERAND (va_dest_ref, 0);
              break;

            case VAR_DECL:
              va_dest = build1 (INDIRECT_REF, cil32_va_list_type, va_dest_ref);
              break;

            default:
              gcc_assert(0);
              break;
            }

            gcc_assert (POINTER_TYPE_P (TREE_TYPE (va_dest))
                        && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va_dest)))
                           == cil32_arg_iterator_type);

            gcc_assert (POINTER_TYPE_P (TREE_TYPE (va_src))
                        && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va_src)))
                           == cil32_arg_iterator_type);

            {
              tree argiter = create_tmp_var (cil32_arg_iterator_type, "arg_iterator");
              location_t locus = EXPR_LOCATION (bsi_stmt (bsi));
              tree stmt = build_gimple_modify_stmt(va_dest, build1 (ADDR_EXPR, cil32_va_list_type, argiter));
              SET_EXPR_LOCATION (stmt, locus);
              bsi_insert_before (&bsi, stmt, BSI_SAME_STMT);
            }

            {
              tree stmt;
              tree new_args;

              new_args = tree_cons (NULL, va_src, NULL);
              new_args = tree_cons (NULL, va_dest,  new_args);

              stmt = build_function_call_expr (cil32_builtins[CIL32_BUILT_IN_VA_COPY],
                                               new_args);
              bsi_replace (&bsi, stmt, true);
            }
          }
          break;

        case BUILT_IN_OBJECT_SIZE:
          {
            /* Inspired from 'expand_builtin_object_size' in builtins.c. We
               return -1 for types 0 and 1, and 0 for types 2 and 3.  */
            tree arg2 = CALL_EXPR_ARG (node, 1);
            int  obj_type;

            STRIP_NOPS (arg2);
            gcc_assert (TREE_CODE (arg2) == INTEGER_CST);

            obj_type = tree_low_cst (arg2, 0);
            switch (obj_type)
              {
              case 0:
              case 1:
                *node_ptr = integer_zero_node;
                break;

              case 2:
              case 3:
                *node_ptr = integer_minus_one_node;
                break;

              default:
                gcc_assert (0);
              }
          }
          break;

        case BUILT_IN_PREFETCH:
          {
            tree exp = CALL_EXPR_ARG (node, 0);

            gcc_assert (bsi_stmt (bsi) == node);

            /* For a target that does not support data prefetch,
               evaluate the memory address argument in case it has
               side effects.  */

            if (! TREE_SIDE_EFFECTS (exp))
              exp = build1 (NOP_EXPR, void_type_node, integer_zero_node);

            SET_EXPR_LOCATION (exp, locus);
            *node_ptr = exp;
          }
          break;

        case BUILT_IN_FRAME_ADDRESS:
        case BUILT_IN_RETURN_ADDRESS:
          {
            /* Supported (sort of) only for non-zero parameter, when it is ok
               to return NULL.  */
            tree arg = CALL_EXPR_ARG (node, 0);
            int  int_arg;
            gcc_assert (TREE_CODE (arg) == INTEGER_CST);
            int_arg = TREE_INT_CST_LOW (arg);
            if (int_arg == 0)
              {
                internal_error ("__builtin_{return,frame}_address not implemented\n");
              }
            else
              {
                *node_ptr = integer_zero_node;
              }
          }
          break;

        case BUILT_IN_BCMP:
          {
            tree src1 = CALL_EXPR_ARG (node, 0);
            tree src2 = CALL_EXPR_ARG (node, 1);
            tree size = CALL_EXPR_ARG (node, 2);

            tree stmt;
            tree new_args;
            
            new_args = tree_cons (NULL, fold_convert(sizetype, size), NULL);
            new_args = tree_cons (NULL, src2, new_args);
            new_args = tree_cons (NULL, src1, new_args);

            stmt = build_function_call_expr (built_in_decls[BUILT_IN_MEMCMP], new_args);
            bsi_replace (&bsi, stmt, true);
          }
          break;

        case BUILT_IN_BZERO:
          {
            tree src1 = CALL_EXPR_ARG (node, 0);
            tree size = CALL_EXPR_ARG (node, 1);

            tree stmt;
            tree new_args;
            
            new_args = tree_cons (NULL, fold_convert(sizetype, size), NULL);
            new_args = tree_cons (NULL, integer_zero_node, new_args);
            new_args = tree_cons (NULL, src1, new_args);

            stmt = build_function_call_expr (built_in_decls[BUILT_IN_MEMSET], new_args);
            bsi_replace (&bsi, stmt, true);
          }
          break;

        case BUILT_IN_BCOPY:
          {
            tree src1 = CALL_EXPR_ARG (node, 0);
            tree dst1 = CALL_EXPR_ARG (node, 1);
            tree size = CALL_EXPR_ARG (node, 2);

            tree stmt;
            tree new_args;
            
            new_args = tree_cons (NULL, fold_convert(sizetype, size), NULL);
            new_args = tree_cons (NULL, src1, new_args);
            new_args = tree_cons (NULL, dst1, new_args);

            stmt = build_function_call_expr (built_in_decls[BUILT_IN_MEMMOVE], new_args);
            bsi_replace (&bsi, stmt, true);
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
  asn_op_stmt = build_gimple_modify_stmt(sel_var, op);
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
  asn_neg_stmt = build_gimple_modify_stmt(sel_var,
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
      tree stmt = build_gimple_modify_stmt(var, op0);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      TREE_OPERAND (node, 0) = var;
      op0 = var;
    }
  op1 = TREE_OPERAND (node, 1);
  if (is_copy_required (op1))
    {
      tree var = create_tmp_var (TREE_TYPE (op1), "cilsimp");
      tree stmt = build_gimple_modify_stmt(var, op1);

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
  asn_op0_stmt = build_gimple_modify_stmt(sel_var, op0);
  SET_EXPR_LOCATION (asn_op0_stmt, locus);
  tmp_bsi = bsi_start (bb_op0);
  bsi_insert_after (&tmp_bsi, label_op0, BSI_NEW_STMT);
  bsi_insert_after (&tmp_bsi, asn_op0_stmt, BSI_SAME_STMT);

  /* Insert labels and statements into op1 bb */
  label_op1 = build1 (LABEL_EXPR, void_type_node, label_decl_op1);
  asn_op1_stmt = build_gimple_modify_stmt(sel_var, op1);
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
      tree stmt = build_gimple_modify_stmt(var, then_op);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      COND_EXPR_THEN (node) = var;
      then_op = var;
    }
  else_op = COND_EXPR_ELSE (node);
  if (is_copy_required (else_op))
    {
      tree var = create_tmp_var (TREE_TYPE (else_op), "cilsimp");
      tree stmt = build_gimple_modify_stmt(var, else_op);

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
  asn_then_stmt = build_gimple_modify_stmt(sel_var, then_op);
  SET_EXPR_LOCATION (asn_then_stmt, locus);
  tmp_bsi = bsi_start (bb_then);
  bsi_insert_after (&tmp_bsi, label_then, BSI_NEW_STMT);
  bsi_insert_after (&tmp_bsi, asn_then_stmt, BSI_SAME_STMT);

  /* Insert labels and statements into else bb */
  label_else = build1 (LABEL_EXPR, void_type_node, label_decl_else);
  asn_else_stmt = build_gimple_modify_stmt(sel_var, else_op);
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
      tree stmt = build_gimple_modify_stmt(var, op0);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      TREE_OPERAND (node, 0) = var;
      op0 = var;
    }
  op1 = TREE_OPERAND (node, 1);
  if (is_copy_required (op1))
    {
      tree var = create_tmp_var (TREE_TYPE (op1), "cilsimp");
      tree stmt = build_gimple_modify_stmt(var, op1);

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
  t = force_gimple_operand_bsi (bsi, t, TRUE, NULL, TRUE, BSI_SAME_STMT);

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
      tree stmt = build_gimple_modify_stmt(var, op0);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      TREE_OPERAND (node, 0) = var;
      op0 = var;
    }
  op1 = TREE_OPERAND (node, 1);
  if (is_copy_required (op1))
    {
      tree var = create_tmp_var (TREE_TYPE (op1), "cilsimp");
      tree stmt = build_gimple_modify_stmt(var, op1);

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
  t = force_gimple_operand_bsi (bsi, t, TRUE, NULL, TRUE, BSI_SAME_STMT);

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
      tree stmt = build_gimple_modify_stmt(var, op0);

      SET_EXPR_LOCATION (stmt, locus);
      bsi_insert_before (bsi, stmt, BSI_SAME_STMT);
      TREE_OPERAND (node, 0) = var;
      op0 = var;
    }
  if (is_copy_required (op1))
    {
      tree var = create_tmp_var (TREE_TYPE (op1), "cilsimp");
      tree stmt = build_gimple_modify_stmt(var, op1);

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
  t1 = force_gimple_operand_bsi (bsi, t1, TRUE, NULL, TRUE, BSI_SAME_STMT);
  t2 = force_gimple_operand_bsi (bsi, t2, TRUE, NULL, TRUE, BSI_SAME_STMT);

  /* Build the rotate result and gimplify it */
  t1 = build2 (BIT_IOR_EXPR, op0_uns_type, t1, t2);
  t1 = fold_convert (TREE_TYPE (TREE_OPERAND (node, 0)), t1);
  t1 = force_gimple_operand_bsi (bsi, t1, TRUE, NULL, TRUE, BSI_SAME_STMT);

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
  TREE_OPERAND (node, 1) = force_gimple_operand_bsi (bsi, t, FALSE, NULL, TRUE, BSI_SAME_STMT);
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
  *node_ptr = force_gimple_operand_bsi (bsi, t1, FALSE, NULL, TRUE, BSI_SAME_STMT);
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
  t1 = force_gimple_operand_bsi (bsi, t1, FALSE, NULL, TRUE, BSI_SAME_STMT);
  t2 = create_tmp_var (ptr_type, "cilsimp");
  stmt = build_gimple_modify_stmt(t2, t1);
  SET_EXPR_LOCATION (stmt, locus);
  bsi_insert_before (bsi, stmt, BSI_SAME_STMT);

  /* Build the expression for the access to the array element */
  t1 = fold_build2 (POINTER_PLUS_EXPR, ptr_type,
                    t2,
                    fold_convert (long_unsigned_type_node, array_disp));
  t1 = force_gimple_operand_bsi (bsi, t1, TRUE, NULL, TRUE, BSI_SAME_STMT);

  /* Update the current node */
  *node_ptr = t1;
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
  tree stmt = bsi_stmt (*bsi);
  location_t locus = EXPR_LOCATION (stmt);
  tree obj = TREE_OPERAND (node, 0);
  tree fld = TREE_OPERAND (node, 1);
  tree fld_type, cont_type, cont_type_ptr, obj_ptr_type;
  tree fld_off, fld_size, cont_size, diff;
  tree lshift_cst, rshift_cst, ptr_off_cst;
  tree obj_addr, addr, cont_var;
  tree new_stmt, list = NULL_TREE;
  tree two = build_int_cst_type (size_type_node, 2);
  tree bits_per_unit = fold_convert (size_type_node, bitsize_unit_node);

  /* Extract bit field layout */
  fld_type = TREE_TYPE (fld);
  fld_off = size_binop (MULT_EXPR, DECL_FIELD_OFFSET (fld), bits_per_unit);
  fld_off = size_binop (PLUS_EXPR, fld_off,
			fold_convert (size_type_node,
				      DECL_FIELD_BIT_OFFSET (fld)));
  fld_size = build_int_cst_type (size_type_node, TYPE_PRECISION (fld_type));
  cont_size = size_binop (TRUNC_DIV_EXPR, bits_per_unit, two);

  do {
    cont_size = size_binop (MULT_EXPR, cont_size, two);
    diff = size_binop (TRUNC_MOD_EXPR, fld_off, cont_size);
    diff = size_binop (PLUS_EXPR, diff, fld_size);
  } while (tree_int_cst_lt (cont_size, diff));

  /* lshift_cst = cont_size + ((fld_off % cont_size) + fld_size) */
  lshift_cst = size_binop (TRUNC_MOD_EXPR, fld_off, cont_size);
  lshift_cst = size_binop (PLUS_EXPR, lshift_cst, fld_size);
  lshift_cst = size_binop (MINUS_EXPR, cont_size, lshift_cst);

  /* rshift_cst = cont_size - fld_size */
  rshift_cst = size_binop (MINUS_EXPR, cont_size, fld_size);

  /* Build the new type for the equivalent access (and a pointer type to it) */
  cont_type = get_integer_type (tree_low_cst (cont_size, true), true);
  cont_type_ptr = build_pointer_type (cont_type);

  /* Build the type corresponding of a pointer to the object */
  obj_ptr_type = build_pointer_type (TREE_TYPE (obj));

  /* Build expression to compute the address to be accessed */
  obj_addr = create_tmp_var (obj_ptr_type, "cilsimp");
  new_stmt = build_gimple_modify_stmt (obj_addr,
				       build_fold_addr_expr (obj));
  gcc_assert (TREE_CODE (obj) != CALL_EXPR);
  append_to_statement_list (new_stmt, &list);

  addr = create_tmp_var (cont_type_ptr, "cilsimp");
  new_stmt = build_gimple_modify_stmt (addr,
				       fold_convert (cont_type_ptr, obj_addr));
  append_to_statement_list (new_stmt, &list);

  /* ptr_off_cst = ((fld_off / cont_size) * cont_size) / 8 */
  ptr_off_cst = size_binop (TRUNC_DIV_EXPR, fld_off, cont_size);
  ptr_off_cst = size_binop (MULT_EXPR, ptr_off_cst, cont_size);
  ptr_off_cst = size_binop (TRUNC_DIV_EXPR, ptr_off_cst, bits_per_unit);

  if (!tree_int_cst_equal (ptr_off_cst, size_zero_node))
    {
      new_stmt = build_gimple_modify_stmt (addr,
					   build2 (POINTER_PLUS_EXPR,
						   cont_type_ptr, addr,
						   ptr_off_cst));
      append_to_statement_list (new_stmt, &list);
    }

  /* Read the value at the address just computed */
  cont_var = create_tmp_var (cont_type, "cilsimp");
  new_stmt = build_gimple_modify_stmt(cont_var,
				      build_fold_indirect_ref (addr));
  append_to_statement_list (new_stmt, &list);

  /* Shift the bit-field into position, clearing the upper bits */
  if (! tree_int_cst_equal (lshift_cst, size_zero_node))
    {
      new_stmt = build_gimple_modify_stmt (cont_var,
					   build2 (LSHIFT_EXPR, cont_type,
						   cont_var, lshift_cst));
      append_to_statement_list (new_stmt, &list);
    }

  if (! tree_int_cst_equal (rshift_cst, size_zero_node))
    {
      new_stmt = build_gimple_modify_stmt (cont_var,
					   build2 (RSHIFT_EXPR, cont_type,
						   cont_var, rshift_cst));
      append_to_statement_list (new_stmt, &list);
    }

  /* Update the current statement (and the current node) */
  set_statement_list_location (list, locus);
  bsi_insert_before (bsi, list, BSI_SAME_STMT);
  *node_ptr = fold_convert (TREE_TYPE (node), cont_var);
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
  tree stmt = bsi_stmt (*bsi);
  location_t locus = EXPR_LOCATION (stmt);
  tree obj = TREE_OPERAND (node, 0);
  tree fld = TREE_OPERAND (node, 1);
  tree fld_type, cont_type, cont_type_ptr, obj_ptr_type;
  tree fld_off, fld_size, cont_size, diff;
  tree shift_cst, mask_cst, ptr_off_cst;
  tree obj_addr, addr, rhs, folded_rhs, cont_var;
  tree new_stmt, list = NULL_TREE;
  tree two = build_int_cst_type (size_type_node, 2);
  tree bits_per_unit = fold_convert (size_type_node, bitsize_unit_node);

  gcc_assert (TREE_CODE (node) == COMPONENT_REF);
  gcc_assert ((TREE_CODE (stmt) == MODIFY_EXPR
	       || TREE_CODE (stmt) == GIMPLE_MODIFY_STMT)
	      && GENERIC_TREE_OPERAND (stmt, 0) == node);

  /* Extract bit field layout */
  fld_type = TREE_TYPE (fld);
  fld_off = size_binop (MULT_EXPR, DECL_FIELD_OFFSET (fld), bits_per_unit);
  fld_off = size_binop (PLUS_EXPR, fld_off,
			fold_convert (size_type_node,
				      DECL_FIELD_BIT_OFFSET (fld)));
  fld_size = build_int_cst_type (size_type_node, TYPE_PRECISION (fld_type));
  cont_size = size_binop (TRUNC_DIV_EXPR, bits_per_unit, two);

  do {
    cont_size = size_binop (MULT_EXPR, cont_size, two);
    diff = size_binop (TRUNC_MOD_EXPR, fld_off, cont_size);
    diff = size_binop (PLUS_EXPR, diff, fld_size);
  } while (tree_int_cst_lt (cont_size, diff));

  shift_cst = size_binop (TRUNC_MOD_EXPR, fld_off, cont_size);

  /* Build the new type for the equivalent access (and a pointer type to it) */
  cont_type = get_integer_type (tree_low_cst (cont_size, true), true);
  cont_type_ptr = build_pointer_type (cont_type);

  /* Build the type corresponding of a pointer to the object */
  obj_ptr_type = build_pointer_type (TREE_TYPE (obj));

  /* Create the mask */
  mask_cst = build_int_cst_type (cont_type, 1);
  mask_cst = size_binop (LSHIFT_EXPR, mask_cst, fld_size);
  mask_cst = size_binop (MINUS_EXPR, mask_cst,
			 build_int_cst_type (cont_type, 1));
  mask_cst = size_binop (LSHIFT_EXPR, mask_cst, shift_cst);

  /* If the rhs is a constant fold the shift & mask operations, if it is not
     copy it and convert it in the container type */
  rhs = fold_convert (cont_type, GENERIC_TREE_OPERAND (stmt, 1));
  folded_rhs = fold_binary_to_constant (LSHIFT_EXPR, cont_type, rhs, shift_cst);

  if (folded_rhs != NULL_TREE)
    {
      folded_rhs = fold_binary_to_constant (BIT_AND_EXPR, cont_type, folded_rhs,
					    mask_cst);
    }

  if (folded_rhs != NULL_TREE)
    rhs = folded_rhs;
  else
    {
      rhs = create_tmp_var (cont_type, "cilsimp");
      new_stmt = fold_convert (cont_type, GENERIC_TREE_OPERAND (stmt, 1));
      new_stmt = build_gimple_modify_stmt (rhs, new_stmt);
      append_to_statement_list (new_stmt, &list);

      new_stmt = build_gimple_modify_stmt (rhs,
					   build2 (LSHIFT_EXPR, cont_type,
						   rhs, shift_cst));
      append_to_statement_list (new_stmt, &list);

      new_stmt = build_gimple_modify_stmt (rhs,
					   build2 (BIT_AND_EXPR, cont_type,
						   rhs, mask_cst));
      append_to_statement_list (new_stmt, &list);
    }

  /* Build expression to compute the address to be accessed */
  obj_addr = create_tmp_var (obj_ptr_type, "cilsimp");
  new_stmt = build_gimple_modify_stmt (obj_addr,
				       build_fold_addr_expr (obj));
  gcc_assert (TREE_CODE (obj) != CALL_EXPR);
  append_to_statement_list (new_stmt, &list);

  addr = create_tmp_var (cont_type_ptr, "cilsimp");
  new_stmt = build_gimple_modify_stmt (addr,
				       fold_convert (cont_type_ptr, obj_addr));
  append_to_statement_list (new_stmt, &list);

  /* ptr_off_cst = ((fld_off / cont_size) * cont_size) / 8 */
  ptr_off_cst = size_binop (TRUNC_DIV_EXPR, fld_off, cont_size);
  ptr_off_cst = size_binop (MULT_EXPR, ptr_off_cst, cont_size);
  ptr_off_cst = size_binop (TRUNC_DIV_EXPR, ptr_off_cst, bits_per_unit);

  if (tree_int_cst_compare (ptr_off_cst, size_zero_node) != 0)
    {
      new_stmt = build_gimple_modify_stmt (addr,
					   build2 (POINTER_PLUS_EXPR,
						   cont_type_ptr, addr,
						   ptr_off_cst));
      append_to_statement_list (new_stmt, &list);
    }

  /* Read existing value at the address just computed */
  cont_var = create_tmp_var (cont_type, "cilsimp");
  new_stmt = build_gimple_modify_stmt(cont_var,
				      build_fold_indirect_ref (addr));
  append_to_statement_list (new_stmt, &list);

  /* Compute the mask to be applied to the existing value */
  mask_cst = size_binop (BIT_XOR_EXPR, mask_cst,
			 build_int_cst_type (cont_type, -1));
  mask_cst = fold_convert (cont_type, mask_cst);

  /* Apply the mask to the existing value */
  new_stmt = build_gimple_modify_stmt (cont_var,
				       build2 (BIT_AND_EXPR, cont_type,
					       cont_var, mask_cst));
  append_to_statement_list (new_stmt, &list);

  /* Compute the new value for the rhs of the current statement */
  if (!((TREE_CODE (rhs) == INTEGER_CST)
	&& tree_int_cst_equal (rhs, size_zero_node)))
    {
      new_stmt = build_gimple_modify_stmt (cont_var,
					   build2 (BIT_IOR_EXPR, cont_type,
						   cont_var, rhs));
      append_to_statement_list (new_stmt, &list);
    }

  /* Update the current statement (and the current node) */
  set_statement_list_location (list, locus);
  bsi_insert_before (bsi, list, BSI_SAME_STMT);
  *node_ptr = build_fold_indirect_ref (addr);
  GENERIC_TREE_OPERAND (stmt, 1) = cont_var;
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
  tree stmt = bsi_stmt (*bsi);
  location_t locus = EXPR_LOCATION (stmt);
  tree obj = TREE_OPERAND (node, 0);
  tree type, cont_type, cont_type_ptr, obj_ptr_type;
  tree obj_addr, addr, cont_var;
  tree off, size, cont_size, diff;
  tree lshift_cst, rshift_cst, ptr_off_cst;
  tree new_stmt, list = NULL_TREE;

  /* Extract bit field layout */
  size = fold_convert (size_type_node, TREE_OPERAND (node, 1));
  off = fold_convert (size_type_node, TREE_OPERAND (node, 2));
  type = TREE_TYPE (node);
  cont_size = size_binop (TRUNC_DIV_EXPR, bits_per_unit, two);

  do {
    cont_size = size_binop (MULT_EXPR, cont_size, two);
    diff = size_binop (TRUNC_MOD_EXPR, off, cont_size);
    diff = size_binop (PLUS_EXPR, diff, size);
  } while (tree_int_cst_lt (cont_size, diff));

  /* lshift_cst = cont_size + ((fld_off % cont_size) + fld_size) */
  lshift_cst = size_binop (TRUNC_MOD_EXPR, off, cont_size);
  lshift_cst = size_binop (PLUS_EXPR, lshift_cst, size);
  lshift_cst = size_binop (MINUS_EXPR, cont_size, lshift_cst);

  /* rshift_cst = cont_size - fld_size */
  rshift_cst = size_binop (MINUS_EXPR, cont_size, size);

  /* Build the new type for the equivalent access (and a pointer type to it) */
  cont_type = get_integer_type (tree_low_cst (cont_size, true), true);
  cont_type_ptr = build_pointer_type (cont_type);

  /* Build the type corresponding of a pointer to the object */
  obj_ptr_type = build_pointer_type (TREE_TYPE (obj));

  /* Build expression to compute the address to be accessed */
  obj_addr = create_tmp_var (obj_ptr_type, "cilsimp");
  new_stmt = build_gimple_modify_stmt (obj_addr,
				       build_fold_addr_expr (obj));
  gcc_assert (TREE_CODE (obj) != CALL_EXPR);
  append_to_statement_list (new_stmt, &list);

  addr = create_tmp_var (cont_type_ptr, "cilsimp");
  new_stmt = build_gimple_modify_stmt (addr,
				       fold_convert (cont_type_ptr, obj_addr));
  append_to_statement_list (new_stmt, &list);

  /* ptr_off_cst = ((fld_off / cont_size) * cont_size) / 8 */
  ptr_off_cst = size_binop (TRUNC_DIV_EXPR, off, cont_size);
  ptr_off_cst = size_binop (MULT_EXPR, ptr_off_cst, cont_size);
  ptr_off_cst = size_binop (TRUNC_DIV_EXPR, ptr_off_cst, bits_per_unit);

  if (!tree_int_cst_equal (ptr_off_cst, size_zero_node))
    {
      new_stmt = build_gimple_modify_stmt (addr,
					   build2 (POINTER_PLUS_EXPR,
						   cont_type_ptr, addr,
						   ptr_off_cst));
      append_to_statement_list (new_stmt, &list);
    }

  /* Read the value at the address just computed */
  cont_var = create_tmp_var (cont_type, "cilsimp");
  new_stmt = build_gimple_modify_stmt(cont_var,
				      build_fold_indirect_ref (addr));
  append_to_statement_list (new_stmt, &list);

  /* Shift the bit-field into position, clearing the upper bits */
  if (! tree_int_cst_equal (lshift_cst, size_zero_node))
    {
      new_stmt = build_gimple_modify_stmt (cont_var,
					   build2 (LSHIFT_EXPR, cont_type,
						   cont_var, lshift_cst));
      append_to_statement_list (new_stmt, &list);
    }

  if (! tree_int_cst_equal (rshift_cst, size_zero_node))
    {
      new_stmt = build_gimple_modify_stmt (cont_var,
					   build2 (RSHIFT_EXPR, cont_type,
						   cont_var, rshift_cst));
      append_to_statement_list (new_stmt, &list);
    }

  /* Update the current statement (and the current node) */
  set_statement_list_location (list, locus);
  bsi_insert_before (bsi, list, BSI_SAME_STMT);
  *node_ptr = fold_convert (TREE_TYPE (node), cont_var);
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
pre_simp_init (block_stmt_iterator *bsi, tree node, tree lhs, tree rhs)
{
  location_t locus = EXPR_LOCATION (bsi_stmt (*bsi));
  tree stmt_list = 0;
  block_stmt_iterator tmp_bsi;
  tree_stmt_iterator i;

  gcc_assert (bsi_stmt (*bsi) == node);
  gcc_assert (TREE_CODE (node) == INIT_EXPR
              || TREE_CODE (node) == MODIFY_EXPR
              || TREE_CODE (node) == GIMPLE_MODIFY_STMT);
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
  stmt = build_gimple_modify_stmt(var, node);
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
      t = cil32_builtins[CIL32_BUILT_IN_INITBLK];
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
        t = cil32_builtins[CIL32_BUILT_IN_CPBLK];
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
                t = cil32_builtins[CIL32_BUILT_IN_INITBLK];
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
                t = cil32_builtins[CIL32_BUILT_IN_INITBLK];
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
          {
            tree fun, stmt;
            tree args = NULL;
            tree value;
            tree ctor_fun = NULL;
            unsigned HOST_WIDE_INT idx;
            tree vector_type = TREE_TYPE (init);
            tree vector_elt_type = TREE_TYPE (vector_type);
            int vec_size = TREE_INT_CST_LOW (TYPE_SIZE (vector_type));
            int elt_size = TREE_INT_CST_LOW (TYPE_SIZE (vector_elt_type));
            int num_elt = vec_size / elt_size;
            int i, num_args = 0;

            /* Build the list of args. */
            FOR_EACH_CONSTRUCTOR_VALUE (CONSTRUCTOR_ELTS (init), idx, value)
              {
                args = tree_cons (NULL, value, args);
                ++num_args;
              }
            /* The constructor might not initialize all args.  */
            for(i = num_args; i < num_elt; ++i )
                args = tree_cons (NULL, integer_zero_node, args);

            /* find the right constructor */
            if (TREE_CODE (vector_elt_type) == INTEGER_TYPE)
              {
                switch (num_elt)
                  {
                  case 2:
                    if (vec_size == 32)
                      ctor_fun = cil32_builtins[CIL32_V2HI_CTOR];
                    else if (vec_size == 64)
                      ctor_fun = cil32_builtins[CIL32_V2SI_CTOR];
                    break;

                  case 4:
                    if (vec_size == 32)
                      ctor_fun = cil32_builtins[CIL32_V4QI_CTOR];
                    else if (vec_size == 64)
                      ctor_fun = cil32_builtins[CIL32_V4HI_CTOR];
                    else if (vec_size == 128)
                      ctor_fun = cil32_builtins[CIL32_V4SI_CTOR];
                    break;

                  case 8:
                    if (vec_size == 64)
                      ctor_fun = cil32_builtins[CIL32_V8QI_CTOR];
                    else if (vec_size == 128)
                      ctor_fun = cil32_builtins[CIL32_V8HI_CTOR];
                    break;

                  case 16:
                    if (vec_size == 128)
                      ctor_fun = cil32_builtins[CIL32_V16QI_CTOR];
                    break;

                  default:
                    fprintf (stderr,
                             "V%d int vectors not supported\n",
                             num_elt);
                    gcc_assert (0);
                    break;
                  }
              }
            else if (TREE_CODE (vector_elt_type) == REAL_TYPE)
              {
                if (num_elt != 2 && num_elt != 4)
                  {
                    fprintf (stderr, "V%dSF vectors not supported\n", num_elt);
                    gcc_assert (0);
                  }
                ctor_fun = cil32_builtins[CIL32_V2SF_CTOR];
              }
            gcc_assert (ctor_fun);

            /* Note that the args list must be reversed. Can do better? */
            fun = build_function_call_expr (ctor_fun, nreverse (args));
            stmt = build_gimple_modify_stmt(decl, fun);
            append_to_statement_list (stmt, stmt_list1);
            append_to_statement_list (stmt, stmt_list2);
          }
          break;

        default:
          gcc_unreachable ();
          break;
        }
      break;

    case INTEGER_CST:
      {
        int type_size = TREE_INT_CST_LOW (decl_size);
        tree t = build_gimple_modify_stmt(decl, init);
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
        tree t = build_gimple_modify_stmt(decl, init);
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

  memset (le_image, 0, size);
  memset (be_image, 0, size);

  expand_init_to_stmt_list1 (decl, init,
                             &stmt_list1, FALSE,
                             &stmt_list2, le_image, be_image);

  num_list1 = statement_list_num_instr (stmt_list1);
  num_list2 = statement_list_num_instr (stmt_list2);


  /* Decide what to do */
  if (TARGET_BIG_ENDIAN || TARGET_LITTLE_ENDIAN
      || ( (num_list2 + 2) < num_list1))
    {
      tree mem_cpy;
      tree args;
      tree from_ptr;
      tree to_ptr = build_fold_addr_expr (decl);

      if (TARGET_LITTLE_ENDIAN || (memcmp (le_image, be_image, size) == 0))
        {
          from_ptr = build_string_literal (size, le_image);
        }
      else if (TARGET_BIG_ENDIAN)
        {
          from_ptr = build_string_literal (size, be_image);
        }
      else
        {
          tree sconst = build_string_literal (size, le_image);
          tree sconst2 = build_string_literal (size, be_image);

          gcc_assert (TREE_TYPE (sconst) == TREE_TYPE (sconst2));

          args = tree_cons (NULL, sconst2, NULL);
          args = tree_cons (NULL, sconst, args);
          from_ptr = build_function_call_expr (cil32_builtins[CIL32_BUILT_IN_ENDIAN_SELECT], args);
        }

      args = tree_cons (NULL, size_int (size), NULL);
      args = tree_cons (NULL, from_ptr, args);
      args = tree_cons (NULL, to_ptr,   args);

      mem_cpy = build_function_call_expr (cil32_builtins[CIL32_BUILT_IN_CPBLK],
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
                             build_gimple_modify_stmt(var, init),
                             BSI_SAME_STMT);
        }
    }
}

/* Simplify pass that makes CIL emission easier. */

static unsigned int
simp_cil_final (void)
{
  basic_block bb;
  block_stmt_iterator bsi;

  two = build_int_cst_type (size_type_node, 2);
  bits_per_unit = fold_convert (size_type_node, bitsize_unit_node);
  simp_final = true;
  push_gimplify_context ();
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

          if (TREE_CODE (stmt) == INIT_EXPR
              || TREE_CODE (stmt) == MODIFY_EXPR
              || TREE_CODE (stmt) == GIMPLE_MODIFY_STMT)
            {
              tree lhs = GENERIC_TREE_OPERAND (stmt, 0);
              tree rhs = GENERIC_TREE_OPERAND (stmt, 1);

              if (TREE_CODE (rhs) == CONSTRUCTOR
                  || TREE_CODE (rhs) == STRING_CST)
                pre_simp_init (&bsi, stmt, lhs, rhs);
              else if (TREE_CODE (lhs) == COMPONENT_REF
                       && DECL_BIT_FIELD (TREE_OPERAND (lhs, 1))
                       && (TREE_CODE (GENERIC_TREE_OPERAND (stmt, 1))
                           != INTEGER_CST))
                {
                  split_use (bsi, &GENERIC_TREE_OPERAND (stmt, 1), false);
                }
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

          if (simp_final && TREE_CODE (stmt) != RETURN_EXPR)
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

  pop_gimplify_context (NULL);
  two = NULL_TREE;
  bits_per_unit = NULL_TREE;

  return 0;
}

/* Gate function of CIL simplify pass. */

static bool
simp_cil_gate (void)
{
  return current_function_decl != NULL;
}

/* Define the parameters of the tree-final-simp-CIL pass.  */

struct tree_opt_pass pass_simp_cil_final =
{
  "finsimpcil",                         /* name */
  simp_cil_gate,                        /* gate */
  simp_cil_final,                       /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_SIMP_CIL,                          /* tv_id */
  PROP_cfg,                             /* properties_required */
  0,                                    /* properties_provided */
  /* ??? If TER is enabled, we also kill gimple.  */
  0,                                    /* properties_destroyed */
  0,
  TODO_ggc_collect | TODO_dump_func,    /* todo_flags_finish */
  0                                     /* letter */
};

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
