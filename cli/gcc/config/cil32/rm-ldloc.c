/*

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
Andrea C. Ornstein      <andrea.ornstein@st.com>
Erven Rohou             <erven.rohou@st.com>
*/

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
#include "pointer-set.h"
#include "output.h"
#include "varray.h"
#include "ggc.h"
#include "gen-cil.h"

/* Local functions, macros and variables.  */
static void mark_var_defs_uses (tree);
static void remove_stloc_ldloc_1 (block_stmt_iterator, tree *, bool *);

static struct pointer_set_t *defd_vars;
static struct pointer_set_t *defd_more_than_once_vars;
static struct pointer_set_t *used_vars;
static struct pointer_set_t *used_more_than_once_vars;
static struct pointer_set_t *useless_vars;


/* Recursively mark the definitions and the uses of the non-static
   non-volatile local variables used in tree NODE.   */

static void
mark_var_defs_uses (tree node)
{
  if (node == NULL_TREE || node == error_mark_node)
    return;

  switch (TREE_CODE (node))
    {
    case COND_EXPR:
      mark_var_defs_uses (COND_EXPR_COND (node));
      break;

    case SWITCH_EXPR:
      mark_var_defs_uses (SWITCH_COND (node));
      break;

    case CALL_EXPR:
      {
        tree arg;
        call_expr_arg_iterator iter;

        mark_var_defs_uses (CALL_EXPR_FN (node));

        FOR_EACH_CALL_EXPR_ARG (arg, iter, node)
            mark_var_defs_uses (arg);
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
    case EXACT_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case TRUNC_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
    case MAX_EXPR:
    case MIN_EXPR:
    case UNORDERED_EXPR:
    case ORDERED_EXPR:
    case UNLT_EXPR:
    case UNLE_EXPR:
    case UNGT_EXPR:
    case UNGE_EXPR:
    case COMPLEX_EXPR:
      mark_var_defs_uses (TREE_OPERAND (node, 0));
      mark_var_defs_uses (TREE_OPERAND (node, 1));
      break;

    case INIT_EXPR:
    case MODIFY_EXPR:
    case GIMPLE_MODIFY_STMT:
      {
        tree lhs = GENERIC_TREE_OPERAND (node, 0);
        tree rhs = GENERIC_TREE_OPERAND (node, 1);

        if (TREE_CODE (lhs) == VAR_DECL)
          {
            if (! TREE_ADDRESSABLE (lhs)
                && ! TREE_STATIC (lhs)
                && ! TREE_SIDE_EFFECTS (lhs)
                && ! DECL_FILE_SCOPE_P (lhs))
              {
                TREE_LANG_FLAG_0 (lhs) = false;

                if (!pointer_set_contains (defd_vars, lhs))
                  pointer_set_insert (defd_vars, lhs);
                else if (!pointer_set_contains (defd_more_than_once_vars, lhs))
                  pointer_set_insert (defd_more_than_once_vars, lhs);
              }
            else
              TREE_LANG_FLAG_0 (lhs) = true;
          }
        else
          mark_var_defs_uses (lhs);

        mark_var_defs_uses (rhs);

        if (AGGREGATE_TYPE_P (TREE_TYPE (lhs)) && TREE_CODE (rhs) == VAR_DECL)
          TREE_LANG_FLAG_0 (rhs) = true;

        gcc_assert (TREE_CODE (rhs) != CONSTRUCTOR
                    && TREE_CODE (rhs) != STRING_CST);
      }
      break;

    case NEGATE_EXPR:
    case BIT_NOT_EXPR:
    case TRUTH_NOT_EXPR:
    case CONVERT_EXPR:
    case NOP_EXPR:
    case FLOAT_EXPR:
    case FIX_TRUNC_EXPR:
    case ABS_EXPR:
    case WITH_SIZE_EXPR:
    case VIEW_CONVERT_EXPR:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
      mark_var_defs_uses (TREE_OPERAND (node, 0));
      break;

    case RETURN_EXPR:
      {
        tree op = TREE_OPERAND (node, 0);

        if (op)
          {
            if (TREE_CODE (op) == MODIFY_EXPR
                || TREE_CODE (op) == GIMPLE_MODIFY_STMT)
              op = GENERIC_TREE_OPERAND (op, 1);

            mark_var_defs_uses (op);
          }
      }
      break;

    case ARRAY_REF:
      gcc_assert (integer_zerop (TREE_OPERAND (node, 1)));
    case ADDR_EXPR:
    case COMPONENT_REF:
    case INDIRECT_REF:
      {
        tree op = TREE_OPERAND (node, 0);

        mark_var_defs_uses (op);
        if (AGGREGATE_TYPE_P (TREE_TYPE (op)) && TREE_CODE (op) == VAR_DECL)
          TREE_LANG_FLAG_0 (op) = true;
      }
      break;

    case COMPLEX_CST:
    case INTEGER_CST:
    case REAL_CST:
    case STRING_CST:
    case VECTOR_CST:
    case PARM_DECL:
    case FUNCTION_DECL:
    case LABEL_DECL:
    case LABEL_EXPR:
    case GOTO_EXPR:
    case ASM_EXPR:
      break;

    case VAR_DECL:
      if (! TREE_ADDRESSABLE (node)
          && ! TREE_STATIC (node)
          && ! TREE_THIS_VOLATILE (node)
          && ! DECL_FILE_SCOPE_P (node))
        {
          TREE_LANG_FLAG_0 (node) = false;

          if (! pointer_set_contains (used_vars, node))
            pointer_set_insert (used_vars, node);
          else if (! pointer_set_contains (used_more_than_once_vars, node))
            pointer_set_insert (used_more_than_once_vars, node);
        }
      else
        TREE_LANG_FLAG_0 (node) = true;
      break;

    default:
      fprintf (stderr, "mark_var_defs_uses: Cannot handle '%s' ",
               tree_code_name [TREE_CODE (node)]);
      debug_generic_expr (node);
      gcc_assert (0);
    }
}

/* Avoids couple of useless stloc - ldloc from being emitted by removing
   non-static non-volatile local variables used exactly once
   throughout the entire function from tree NODE_PTR.
   BSI is the statement iterator pointing to the statement containing NODE_PTR.
   The bool pointed by MOD is set if the function changed anything,
   otherwise its value is not modified.
   This function works recursively, looking for VAR_DECLs corresponding
   to local non-static non-volatile local variables used exactly once
   in the function.
   When such a variable is found, if its only definition is in
   the statement immediately preceding BSI, then the use of the variable is
   replaced by its definition (and that statement is removed).
   By doing so, the code exits GIMPLE form; however, the CIL emission
   mechanism is able to handle such a hybrid GIMPLE representation.   */

static void
remove_stloc_ldloc_1 (block_stmt_iterator bsi, tree *node_ptr, bool *mod)
{
  tree node = *node_ptr;
  block_stmt_iterator prev_bsi;

  if (node == NULL_TREE || node == error_mark_node)
    return;

  /* Get iterator for the previous statememt */
  prev_bsi = bsi;
  bsi_prev (&prev_bsi);

  /* No remotion is possible if the statement is the first in the block */
  if (bsi_end_p (prev_bsi))
    return;

  switch (TREE_CODE (node))
    {
    case COND_EXPR:
      remove_stloc_ldloc_1 (bsi, &COND_EXPR_COND (node), mod);
      break;

    case SWITCH_EXPR:
      remove_stloc_ldloc_1 (bsi, &SWITCH_COND (node), mod);
      break;

    case CALL_EXPR:
      {
        unsigned int aidx  = 0;
        unsigned int nargs = call_expr_nargs(node);

        for (;aidx<nargs;++aidx)
          remove_stloc_ldloc_1 (bsi, &CALL_EXPR_ARG (node, aidx), mod);

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
    case EXACT_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case TRUNC_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
    case MAX_EXPR:
    case MIN_EXPR:
    case UNORDERED_EXPR:
    case ORDERED_EXPR:
    case UNLT_EXPR:
    case UNLE_EXPR:
    case UNGT_EXPR:
    case UNGE_EXPR:
      remove_stloc_ldloc_1 (bsi, &TREE_OPERAND (node, 0), mod);
      remove_stloc_ldloc_1 (bsi, &TREE_OPERAND (node, 1), mod);
      break;

    case INIT_EXPR:
    case MODIFY_EXPR:
    case GIMPLE_MODIFY_STMT:
      if (TREE_CODE (GENERIC_TREE_OPERAND (node, 0)) != VAR_DECL)
        remove_stloc_ldloc_1 (bsi, &GENERIC_TREE_OPERAND (node, 0), mod);
      remove_stloc_ldloc_1 (bsi, &GENERIC_TREE_OPERAND (node, 1), mod);
      gcc_assert (TREE_CODE (GENERIC_TREE_OPERAND (node, 1)) != CONSTRUCTOR
                  && TREE_CODE (GENERIC_TREE_OPERAND (node, 1)) != STRING_CST);
      break;

    case NEGATE_EXPR:
    case BIT_NOT_EXPR:
    case TRUTH_NOT_EXPR:
    case CONVERT_EXPR:
    case NOP_EXPR:
    case FLOAT_EXPR:
    case FIX_TRUNC_EXPR:
    case ADDR_EXPR:
    case COMPONENT_REF:
    case INDIRECT_REF:
    case ARRAY_REF:
    case ABS_EXPR:
    case WITH_SIZE_EXPR:
      remove_stloc_ldloc_1 (bsi, &TREE_OPERAND (node, 0), mod);
      break;

    case RETURN_EXPR:
      {
        tree op = TREE_OPERAND (node, 0);

        if (op)
          {
            if (TREE_CODE (op) == MODIFY_EXPR || TREE_CODE (op) == GIMPLE_MODIFY_STMT)
              remove_stloc_ldloc_1 (bsi, &GENERIC_TREE_OPERAND (op, 1), mod);
            else
              remove_stloc_ldloc_1 (bsi, &TREE_OPERAND (node, 0), mod);
          }
      }
      break;

    case VAR_DECL:
      /* mark_var_defs_uses (...) function sets TREE_LANG_FLAG_0
         for some variables that shouldn't be removed.   */
      if (! TREE_LANG_FLAG_0 (node))
        {
          tree prev_stmt;

          /* Get the previous statement */
          prev_stmt = bsi_stmt (prev_bsi);

          /* If the variable is used exactly once and the definition
             is in the previous statement, then remove it */
          if ((TREE_CODE (prev_stmt) == MODIFY_EXPR
               || TREE_CODE (prev_stmt) == GIMPLE_MODIFY_STMT)
              && GENERIC_TREE_OPERAND (prev_stmt, 0) == node
              && pointer_set_contains (used_vars, node)
              && ! pointer_set_contains (used_more_than_once_vars, node))
            {
              /* If the definition is unique, set that
                 the variable has become useless.   */
              if (! pointer_set_contains (defd_more_than_once_vars, node))
                pointer_set_insert (useless_vars, node);

              /* Replace variable's use with the definition */
              *node_ptr = GENERIC_TREE_OPERAND (prev_stmt, 1);

              /* Remove the previous statement */
              bsi_remove (&prev_bsi, true);

              /* Set that the function changed something */
              *mod = true;
            }
        }
      break;

    default:
      ;
    }
}

void
remove_stloc_ldloc (void)
{
  basic_block bb;
  block_stmt_iterator bsi;

  /* Mark defs and uses of local non-static variables */
  defd_vars = pointer_set_create ();
  defd_more_than_once_vars = pointer_set_create ();
  used_vars = pointer_set_create ();
  used_more_than_once_vars = pointer_set_create ();

  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
          tree stmt = bsi_stmt (bsi);

          mark_var_defs_uses (stmt);
        }
    }
  pointer_set_destroy (defd_vars);

  /* Remove useless pairs of stloc - ldloc */
  useless_vars = pointer_set_create ();
  FOR_EACH_BB (bb)
    {
      /* Start from the second statement, if any */
      bsi = bsi_start (bb);
      if (! bsi_end_p (bsi))
        bsi_next (&bsi);

      for (; !bsi_end_p (bsi); bsi_next (&bsi))
        {
          bool changed;

          /* Remove stloc - ldloc pairs until no change is done
             to the current statement */
          do {
            changed = false;
            remove_stloc_ldloc_1 (bsi, bsi_stmt_ptr (bsi), &changed);
          } while (changed);
        }
    }
  pointer_set_destroy (defd_more_than_once_vars);
  pointer_set_destroy (used_vars);
  pointer_set_destroy (used_more_than_once_vars);

  /* Remove useless vars (only used in removed stloc - ldloc pairs) */
  {
    tree cell, prev_cell = NULL_TREE;

    for (cell = cfun->unexpanded_var_list;
         cell;
         cell = TREE_CHAIN (cell))
      {
        tree var = TREE_VALUE (cell);

        if (pointer_set_contains (useless_vars, var))
          {
            if (prev_cell == NULL_TREE)
              cfun->unexpanded_var_list = TREE_CHAIN (cell);
            else
              TREE_CHAIN (prev_cell) = TREE_CHAIN (cell);
          }
        else
          prev_cell = cell;
      }
  }
  pointer_set_destroy (useless_vars);
}

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
