/* Type promotion of SSA names to minimise redundant zero/sign extension.
   Copyright (C) 2015 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "flags.h"
#include "hash-set.h"
#include "machmode.h"
#include "vec.h"
#include "double-int.h"
#include "input.h"
#include "alias.h"
#include "symtab.h"
#include "wide-int.h"
#include "inchash.h"
#include "tree.h"
#include "fold-const.h"
#include "stor-layout.h"
#include "calls.h"
#include "predict.h"
#include "hard-reg-set.h"
#include "function.h"
#include "dominance.h"
#include "cfg.h"
#include "basic-block.h"
#include "tree-ssa-alias.h"
#include "gimple-fold.h"
#include "tree-eh.h"
#include "gimple-expr.h"
#include "is-a.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "gimple-ssa.h"
#include "tree-phinodes.h"
#include "ssa-iterators.h"
#include "stringpool.h"
#include "tree-ssanames.h"
#include "tree-pass.h"
#include "gimple-pretty-print.h"
#include "langhooks.h"
#include "sbitmap.h"
#include "domwalk.h"

/* This pass applies type promotion to SSA names in the function and
   inserts appropriate truncations.  Idea of this pass is to promote operations
   such a way that we can minimise generation of subreg in RTL,
   that intern results in removal of redundant zero/sign extensions.  This pass
   will run prior to The VRP and DOM such that they will be able to optimise
   redundant truncations and extensions.  This is based on the discussion from
   https://gcc.gnu.org/ml/gcc-patches/2014-09/msg00472.html.

   This pass execute as follows:

   1.  This pass records gimple statements that may produce results that can
   overflow (beyond the original type) and operations that has to be always
   performed in the original type.  This is done in
   process_all_stmts_for_unsafe_promotion.  Here, gimple which sets SSA_NAMES
   are processed in a work_list to set ssa_sets_higher_bits_bitmap
   (set_ssa_overflows) and ssa_not_safe_bitmap.

   2.  promote_all_stmts  traverses the basic blocks in dominator order and
   promotes all the SSA_NAMES that were  selected as safe in the  step 1 above.
   It uses promote_all_stmts to do the register promotion stmt by stmt.
   The definition of the SSA_NAME is promoted first and then all the uses are
   promoted according to the gimple stmt type.  If the SSA_NAME can overflow
   when promoted necessary fix-ups are also performed to preserve the semantics
   of the program.
*/

static unsigned n_ssa_val;
static sbitmap ssa_not_safe_bitmap;
static sbitmap ssa_to_be_promoted_bitmap;
static sbitmap ssa_sets_higher_bits_bitmap;

/* Return the promoted type for TYPE.  */
static tree
get_promoted_type (tree type)
{
  tree promoted_type;
  enum machine_mode mode;
  int uns;
  if (POINTER_TYPE_P (type)
      || TYPE_PRECISION (type) == 1
      || !INTEGRAL_TYPE_P (type))
    return type;
#ifdef PROMOTE_MODE
  mode = TYPE_MODE (type);
  uns = TYPE_SIGN (type);
  PROMOTE_MODE (mode, uns, type);
#else
  mode = smallest_mode_for_size (GET_MODE_PRECISION (TYPE_MODE (type)),
				 MODE_INT);
#endif
  uns = TYPE_SIGN (type);
  promoted_type = lang_hooks.types.type_for_mode (mode, uns);
  if (promoted_type
      && (TYPE_PRECISION (promoted_type) > TYPE_PRECISION (type)))
    type = promoted_type;
  return type;
}

/* Predicate that tells if promoting computation with ssa NAME is safe.  */
static bool
promotion_safe_p (tree name)
{
  if (TREE_CODE (name) == SSA_NAME)
    {
      gimple stmt = SSA_NAME_DEF_STMT (name);
      unsigned int index = SSA_NAME_VERSION (name);

      if (gimple_vdef (stmt) != NULL_TREE
	  || gimple_vuse (stmt) != NULL_TREE)
	return false;
      if (index < n_ssa_val)
	return !bitmap_bit_p (ssa_not_safe_bitmap, index);
    }
  return false;
}

/* Return true if ssa NAME is already considered for promotion.  */
static bool
ssa_promoted_p (tree name)
{
  if (TREE_CODE (name) == SSA_NAME)
    {
      unsigned int index = SSA_NAME_VERSION (name);
      if (index < n_ssa_val)
	return !bitmap_bit_p (ssa_to_be_promoted_bitmap, index);
    }
  return true;
}

/* Return true if ssa NAME will be considered for promotion.  */
static bool
ssa_tobe_promoted_p (tree name)
{
  if (TREE_CODE (name) == SSA_NAME)
    {
      unsigned int index = SSA_NAME_VERSION (name);
      if (index < n_ssa_val)
	return bitmap_bit_p (ssa_to_be_promoted_bitmap, index);
    }
  return false;
}

/* Set ssa NAME to be already considered for promotion.  */
static void
set_ssa_promoted (tree name)
{
  if (TREE_CODE (name) == SSA_NAME)
    {
      unsigned int index = SSA_NAME_VERSION (name);
      if (index < n_ssa_val)
	bitmap_clear_bit (ssa_to_be_promoted_bitmap, index);
    }
}

/* Set ssa NAME will have higher bits if promoted.  */
static void
set_ssa_overflows (tree name)
{
  if (TREE_CODE (name) == SSA_NAME)
    {
      unsigned int index = SSA_NAME_VERSION (name);
      if (index < n_ssa_val)
	bitmap_set_bit (ssa_sets_higher_bits_bitmap, index);
    }
}

/* Return true if ssa NAME will have higher bits if promoted.  */
static bool
ssa_overflows_p (tree name)
{
  if (TREE_CODE (name) == SSA_NAME)
    {
      unsigned int index = SSA_NAME_VERSION (name);
      if (index < n_ssa_val)
	return bitmap_bit_p (ssa_sets_higher_bits_bitmap, index);
    }
  return false;
}

/* Create an ssa with TYPE to copy ssa VAR.  */
static tree
make_promoted_copy (tree var, gimple def_stmt, tree type)
{
  tree new_lhs = make_ssa_name (type, def_stmt);
  if (SSA_NAME_OCCURS_IN_ABNORMAL_PHI (var))
    SSA_NAME_OCCURS_IN_ABNORMAL_PHI (new_lhs) = 1;
  return new_lhs;
}

/* Return single successor (excluding EH edge) for basic block BB.  If there
   are more than one successors, return NULL.  */
static basic_block
get_single_successor_bb (basic_block bb)
{
  edge e, res = NULL;
  edge_iterator ei;

  FOR_EACH_EDGE (e, ei, bb->succs)
    if (!(e->flags & EDGE_EH))
      {
	if (res)
	  return NULL;
	res = e;
      }
  return res->dest;
}

/* Insert COPY_STMT along the edge from STMT to its successor.  */
static void
insert_stmt_on_edge (gimple stmt, gimple copy_stmt)
{
  edge_iterator ei;
  edge e, edge = NULL;
  basic_block bb = gimple_bb (stmt);

  FOR_EACH_EDGE (e, ei, bb->succs)
    if (!(e->flags & EDGE_EH))
      {
	gcc_assert (edge == NULL);
	edge = e;
      }

  gcc_assert (edge);
  gsi_insert_on_edge_immediate (edge, copy_stmt);
}

/* Convert constant CST to TYPE.  */
static tree
convert_int_cst (tree type, tree cst, signop sign = SIGNED)
{
  wide_int wi_cons = fold_convert (type, cst);
  wi_cons = wi::ext (wi_cons, TYPE_PRECISION (TREE_TYPE (cst)), sign);
  return wide_int_to_tree (type, wi_cons);
}


/* Promote constants in STMT to TYPE.  If PROMOTE_COND_EXPR is true,
 promote only the constants in conditions part of the COND_EXPR.  */
static void
promote_cst_in_stmt (gimple stmt, tree type,
		     bool promote_cond_expr = false, signop sign = SIGNED)
{
  tree op;
  ssa_op_iter iter;
  use_operand_p oprnd;
  int index;
  tree op0, op1;

  if (promote_cond_expr)
    {
      /* Promote constant in COND_EXPR.  */
      gcc_assert (gimple_assign_rhs_code (stmt) == COND_EXPR);
      op = gimple_assign_rhs1 (stmt);
      op0 = TREE_OPERAND (op, 0);
      op1 = TREE_OPERAND (op, 1);

      if (TREE_CODE (op0) == INTEGER_CST)
	op0 = convert_int_cst (type, op0, sign);
      if (TREE_CODE (op1) == INTEGER_CST)
	op1 = convert_int_cst (type, op1, sign);

      tree new_op = build2 (TREE_CODE (op), type, op0, op1);
      gimple_assign_set_rhs1 (stmt, new_op);
      return;
    }

  switch (gimple_code (stmt))
    {
    case GIMPLE_ASSIGN:
      op = gimple_assign_rhs1 (stmt);

      if (op && TREE_CODE (op) == INTEGER_CST)
	gimple_assign_set_rhs1 (stmt, convert_int_cst (type, op, sign));
      op = gimple_assign_rhs2 (stmt);

      if (op && TREE_CODE (op) == INTEGER_CST)
	gimple_assign_set_rhs2 (stmt, convert_int_cst (type, op, sign));
      op = gimple_assign_rhs3 (stmt);

      if (op && TREE_CODE (op) == INTEGER_CST)
	gimple_assign_set_rhs3 (stmt, convert_int_cst (type, op, sign));
      break;

    case GIMPLE_PHI:
	{
	  gphi *phi = as_a <gphi *> (stmt);
	  FOR_EACH_PHI_ARG (oprnd, phi, iter, SSA_OP_USE)
	    {
	      op = USE_FROM_PTR (oprnd);
	      index = PHI_ARG_INDEX_FROM_USE (oprnd);
	      if (TREE_CODE (op) == INTEGER_CST)
		SET_PHI_ARG_DEF (phi, index, convert_int_cst (type, op, sign));
	    }
	}
      break;

    case GIMPLE_COND:
	{
	  gcond *cond = as_a <gcond *> (stmt);
	  op = gimple_cond_lhs (cond);

	  if (op && TREE_CODE (op) == INTEGER_CST)
	    gimple_cond_set_lhs (cond, convert_int_cst (type, op, sign));
	  op = gimple_cond_rhs (cond);

	  if (op && TREE_CODE (op) == INTEGER_CST)
	    gimple_cond_set_rhs (cond, convert_int_cst (type, op, sign));
	}

    default:
      break;
    }
}

/* Zero/sign extend (depending on type) VAR and truncate to WIDTH bits.
   Assign the zero/sign extended value in NEW_VAR.  gimple statement
   that performs the zero/sign extension is returned.  */
static gimple
zero_sign_extend_stmt (tree new_var, tree var, int width)
{
  gcc_assert (TYPE_PRECISION (TREE_TYPE (var))
	      == TYPE_PRECISION (TREE_TYPE (new_var)));
  gcc_assert (TYPE_PRECISION (TREE_TYPE (var)) > width);
  gimple stmt;

  if (TYPE_UNSIGNED (TREE_TYPE (new_var)))
    /* Zero extend.  */
    stmt = gimple_build_assign (new_var,
				BIT_AND_EXPR,
				var, build_int_cst (TREE_TYPE (var),
						    ((1ULL << width) - 1)));
  else
    /* Sign extend.  */
    stmt = gimple_build_assign (new_var,
				SEXT_EXPR,
				var, build_int_cst (TREE_TYPE (var), width));
  return stmt;
}

/* Promote use in an assignment.  Depending on the gimple_assign_rhs_code,
   values in NEW_USE might have to be truncated to the type of USE.  */
static void
promote_assign_stmt_use (gimple stmt,
			 tree use,
			 imm_use_iterator *ui,
			 tree new_use,
			 tree copy_of_use,
			 tree promoted_type)
{
  tree lhs = gimple_assign_lhs (stmt);
  tree rhs1 = gimple_assign_rhs1 (stmt);
  tree rhs2 = gimple_assign_rhs2 (stmt);
  tree rhs3 = gimple_assign_rhs3 (stmt);
  gimple_stmt_iterator gsi;
  use_operand_p op;
  enum tree_code code = gimple_assign_rhs_code (stmt);
  /* If promoted and fix up is to be performed, fix is true.  */
  bool fix = false;

  switch (code)
    {
    CASE_CONVERT:
      if (ssa_tobe_promoted_p (lhs)
	  && promotion_safe_p (lhs)
	  && TREE_TYPE (new_use) == promoted_type)
	{
	  if (TYPE_PRECISION (TREE_TYPE (lhs)) > TYPE_PRECISION (TREE_TYPE (rhs1)))
	    {
	      tree temp = make_promoted_copy (lhs, NULL, promoted_type);
	      gimple copy_stmt =
		zero_sign_extend_stmt (temp, new_use,
				       TYPE_PRECISION (TREE_TYPE (use)));
	      gsi = gsi_for_stmt (stmt);
	      gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
	      FOR_EACH_IMM_USE_ON_STMT (op, *ui)
		SET_USE (op, temp);
	      update_stmt (stmt);
	    }
	  else
	    {
	      FOR_EACH_IMM_USE_ON_STMT (op, *ui)
		SET_USE (op, new_use);
	      update_stmt (stmt);
	    }
	}
      else
	{
	  if (TYPE_PRECISION (TREE_TYPE (lhs)) < TYPE_PRECISION (TREE_TYPE (rhs1)))
	    {
	      FOR_EACH_IMM_USE_ON_STMT (op, *ui)
		SET_USE (op, new_use);
	      update_stmt (stmt);
	    }
	  else if (!copy_of_use)
	    {
	      tree temp = make_promoted_copy (use, NULL, TREE_TYPE (use));
	      gimple copy_stmt = gimple_build_assign (temp, CONVERT_EXPR,
						      new_use, NULL_TREE);
	      gsi = gsi_for_stmt (stmt);
	      gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
	      FOR_EACH_IMM_USE_ON_STMT (op, *ui)
		SET_USE (op, temp);
	      update_stmt (stmt);
	    }
	  else
	    {
	      FOR_EACH_IMM_USE_ON_STMT (op, *ui)
		SET_USE (op, copy_of_use);
	      update_stmt (stmt);
	    }
	}
      return;

    case COND_EXPR:
      /* Promote COND_EXPR coparison operands.  */
      if (use != rhs2
	  && use != rhs3)
	{
	  tree temp;
	  tree op0 = TREE_OPERAND (rhs1, 0);
	  tree op1 = TREE_OPERAND (rhs1, 1);
	  bool is_cst = false;

	  if (TREE_CODE (op0) == INTEGER_CST
	      || TREE_CODE (op1) == INTEGER_CST)
	    is_cst = true;

	  /* If this SSA is not promoted.  */
	  if (use == new_use)
	    {
	      if (is_cst)
		{
		  temp = new_use;
		}
	      else
		{
		  temp = make_promoted_copy (use, NULL, promoted_type);
		  gimple copy_stmt = gimple_build_assign (temp, CONVERT_EXPR,
							  new_use, NULL_TREE);
		  gsi = gsi_for_stmt (stmt);
		  gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
		  promote_cst_in_stmt (stmt, promoted_type, true,
				       TYPE_SIGN (TREE_TYPE (use)));
		}
	    }
	  /* If this SSA is promoted and can overflow.  */
	  else if (ssa_overflows_p (use))
	    {
	      temp = make_promoted_copy (use, NULL, promoted_type);
	      gimple copy_stmt
		= zero_sign_extend_stmt (temp, new_use,
					 TYPE_PRECISION (TREE_TYPE (use)));
	      gsi = gsi_for_stmt (stmt);
	      gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
	      promote_cst_in_stmt (stmt, promoted_type, true,
				   TYPE_SIGN (TREE_TYPE (use)));
	    }
	  else
	    {
	      /* If this SSA is promoted and does not overflow.  */
	      temp = new_use;
	      promote_cst_in_stmt (stmt, promoted_type, true,
				   TYPE_SIGN (TREE_TYPE (use)));
	    }

	  if (op0 == use)
	    op0 = temp;
	  else
	    op1 = temp;

	  tree new_op = build2 (TREE_CODE (rhs1), promoted_type, op0, op1);
	  gimple_assign_set_rhs1 (stmt, new_op);
	  update_stmt (stmt);
	  return;
	}
      else
	{
	  promote_cst_in_stmt (stmt, promoted_type);
	}
      break;

    case RSHIFT_EXPR:
    case LSHIFT_EXPR:
    case WIDEN_LSHIFT_EXPR:
    case TRUNC_MOD_EXPR:
    case CEIL_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case ROUND_MOD_EXPR:
    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case RDIV_EXPR:
    case ROUND_DIV_EXPR:
    case EXACT_DIV_EXPR:
    case MIN_EXPR:
    case MAX_EXPR:
    case RANGE_EXPR:
      if (ssa_overflows_p (use))
	fix = true;
      break;

    default:
      break;
    }

  if (fix && promotion_safe_p (lhs)
      && TREE_TYPE (new_use) == promoted_type)
    {
      /* Promoted with values truncated.  */
      tree temp = make_promoted_copy (use, NULL, promoted_type);
      gimple copy_stmt = zero_sign_extend_stmt (temp, new_use,
						TYPE_PRECISION (TREE_TYPE (use)));
      gsi = gsi_for_stmt (stmt);
      gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
      FOR_EACH_IMM_USE_ON_STMT (op, *ui)
	SET_USE (op, temp);
      update_stmt (stmt);
      return;
    }
  else if (!(TREE_CODE_CLASS (code) == tcc_comparison
	     || TREE_CODE_CLASS (code) == tcc_reference
	     || code == VIEW_CONVERT_EXPR
	     || code == COMPLEX_EXPR
	     || code == ASM_EXPR
	     || code == OBJ_TYPE_REF
	     || gimple_vdef (stmt)
	     || VECTOR_TYPE_P (TREE_TYPE (lhs)))
	   && (promotion_safe_p (lhs)
	       || (TREE_CODE_CLASS (code) == tcc_comparison)))
    {
      /* Statement promoted.  */
      if ((TYPE_PRECISION (TREE_TYPE (use))
	   < TYPE_PRECISION (promoted_type))
	  && (code != COND_EXPR))
	promote_cst_in_stmt (stmt, promoted_type);

      if (promoted_type == TREE_TYPE (new_use))
	{
	  /* Operand also promoted.  */
	  FOR_EACH_IMM_USE_ON_STMT (op, *ui)
	    SET_USE (op, new_use);
	  update_stmt (stmt);
	}
      else
	{
	  /* Operand not promoted.  */
	  tree temp = make_promoted_copy (use, NULL, promoted_type);
	  gimple copy_stmt = gimple_build_assign (temp, CONVERT_EXPR,
						  new_use, NULL_TREE);
	  gsi = gsi_for_stmt (stmt);
	  gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
	  FOR_EACH_IMM_USE_ON_STMT (op, *ui)
	    SET_USE (op, temp);
	  update_stmt (stmt);
	}
    }
  else
    {
      /* Statement not promoted.  */
      if (copy_of_use)
	{
	  /* Operand also not promoted.  */
	  FOR_EACH_IMM_USE_ON_STMT (op, *ui)
	    SET_USE (op, copy_of_use);
	  update_stmt (stmt);
	}
      else
	{
	  /* Operand promoted.  */
	  tree temp = make_promoted_copy (use, NULL, TREE_TYPE (use));
	  gimple copy_stmt = gimple_build_assign (temp, CONVERT_EXPR,
				   new_use, NULL_TREE);

	  gsi = gsi_for_stmt (stmt);
	  gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);

	  FOR_EACH_IMM_USE_ON_STMT (op, *ui)
	    SET_USE (op, temp);
	  update_stmt (stmt);
	}
    }
}

/* Promote ssa USE in phi STMT to PROMOTED_TYPE.  */
static void
promote_phi_stmt_use (gimple stmt,
		       tree use,
		       imm_use_iterator *ui,
		       tree new_use,
		       tree copy_of_use,
		       tree promoted_type)
{
  tree lhs = PHI_RESULT (stmt);
  tree type;
  tree temp;
  gimple_stmt_iterator gsi;
  use_operand_p op;

  if (ssa_tobe_promoted_p (lhs)
      && promotion_safe_p (lhs))
    type = promoted_type;
  else
    type = TREE_TYPE (lhs);

  /* Check if we need a convert stmt to get the required type.  */
  if (type == TREE_TYPE (new_use))
    temp = new_use;
  else if (copy_of_use && (type == TREE_TYPE (copy_of_use)))
    temp = copy_of_use;
  else
    {
      temp = make_promoted_copy (use, NULL, type);
      gimple copy_stmt
	= gimple_build_assign (temp, CONVERT_EXPR,
			       new_use, NULL_TREE);

      if (gimple_code (SSA_NAME_DEF_STMT (new_use)) == GIMPLE_NOP)
	{
	  basic_block bb = ENTRY_BLOCK_PTR_FOR_FN (cfun);
	  bb = get_single_successor_bb (bb);
	  gcc_assert (bb);
	  gsi = gsi_after_labels (bb);
	  gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
	}
      else if (gimple_code (SSA_NAME_DEF_STMT (new_use))
	       != GIMPLE_PHI)
	{
	  gsi = gsi_for_stmt (SSA_NAME_DEF_STMT (new_use));
	  if (lookup_stmt_eh_lp (SSA_NAME_DEF_STMT (new_use)) > 0)
	    insert_stmt_on_edge (SSA_NAME_DEF_STMT (new_use), copy_stmt);
	  else
	    gsi_insert_after (&gsi, copy_stmt, GSI_NEW_STMT);
	}
      else
	{
	  gsi = gsi_after_labels
	    (gimple_bb (SSA_NAME_DEF_STMT (new_use)));
	  gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
	}
    }

  FOR_EACH_IMM_USE_ON_STMT (op, *ui)
    SET_USE (op, temp);
  update_stmt (stmt);
}

/* Promote ssa USE in STMT to PROMOTED_TYPE.  */
static void
promote_cond_stmt_use (gimple stmt,
		       tree use,
		       imm_use_iterator *ui,
		       tree new_use,
		       tree promoted_type)
{
  gimple_stmt_iterator gsi;
  use_operand_p op;
  bool fix = false;
  bool is_cst = false;
  tree lhs = gimple_cond_lhs (stmt);
  tree rhs = gimple_cond_rhs (stmt);

  if (TREE_CODE (lhs) == INTEGER_CST
      || TREE_CODE (rhs) == INTEGER_CST)
    is_cst = true;

  if (ssa_overflows_p (use))
    fix = true;

  if (fix
      && TREE_TYPE (new_use) == promoted_type)
    {
      tree temp = make_promoted_copy (use, NULL, promoted_type);
      gimple copy_stmt = zero_sign_extend_stmt (temp, new_use,
						TYPE_PRECISION (TREE_TYPE (use)));

      gsi = gsi_for_stmt (stmt);
      gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
      FOR_EACH_IMM_USE_ON_STMT (op, *ui)
	SET_USE (op, temp);
      update_stmt (stmt);
      promote_cst_in_stmt (stmt, promoted_type, false,
			   TYPE_SIGN (TREE_TYPE (use)));
    }
  else
    {
      /* Copmparison will happen in promoted type.  */
      tree temp;
      if (TREE_TYPE (new_use) == promoted_type)
	{
	  temp = new_use;
	  promote_cst_in_stmt (stmt, promoted_type, false,
			       TYPE_SIGN (TREE_TYPE (use)));
	}
      else if (is_cst)
	{
	  temp = new_use;
	}
      else
	{
	  temp = make_promoted_copy (use, NULL, promoted_type);
	  gimple copy_stmt = gimple_build_assign (temp, CONVERT_EXPR,
						  new_use, NULL_TREE);
	  gsi = gsi_for_stmt (stmt);
	  gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
	}

      FOR_EACH_IMM_USE_ON_STMT (op, *ui)
	SET_USE (op, temp);
      update_stmt (stmt);
    }
}

/* Promote definition DEF to NEW_TYPE.  If the DEF is replaced and has to
   be released, set RELEASE_DEF.  Also return COPY_OF_DEF with the original
   type for any use statement that needs truncation.  */
static tree
promote_definition (tree def,
		    tree promoted_type,
		    tree *copy_of_def,
		    bool *release_def)
{
  gimple def_stmt = SSA_NAME_DEF_STMT (def);
  gimple copy_stmt = NULL;
  tree new_def;
  basic_block bb;
  gimple_stmt_iterator gsi;
  gphi *phi;

  gcc_assert (release_def);
  *release_def = false;
  if (SSA_NAME_VAR (def) == NULL
      && gimple_code (def_stmt) == GIMPLE_NOP)
    {
      TREE_TYPE (def) = promoted_type;
      promote_cst_in_stmt (def_stmt, promoted_type);
      new_def = def;
      *copy_of_def = NULL;
      return new_def;
    }

  switch (gimple_code (def_stmt))
    {

    case GIMPLE_PHI:
      phi = as_a <gphi *> (def_stmt);
      new_def = make_promoted_copy (def, phi, promoted_type);
      *copy_of_def = NULL;
      gimple_phi_set_result (phi, new_def);
      SET_PHI_RESULT (phi, new_def);
      *release_def = true;
      update_stmt (def_stmt);
      promote_cst_in_stmt (def_stmt, promoted_type);
      break;

    case GIMPLE_NOP:
      /* Create a promoted type copy of parameters.  */
      bb = ENTRY_BLOCK_PTR_FOR_FN (cfun);
      bb = get_single_successor_bb (bb);
      gcc_assert (bb);
      gsi = gsi_after_labels (bb);
      new_def = make_promoted_copy (def, NULL, promoted_type);
      copy_stmt = gimple_build_assign (new_def, CONVERT_EXPR,
				       def, NULL_TREE);
      gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
      *copy_of_def = def;
      break;

    case GIMPLE_ASSIGN:
	{
	  enum tree_code code = gimple_assign_rhs_code (def_stmt);
	  tree rhs1 = gimple_assign_rhs1 (def_stmt);
	  if (CONVERT_EXPR_CODE_P (code)
	      && TREE_TYPE (rhs1) == promoted_type)
	    {
	      new_def = make_promoted_copy (def, NULL, promoted_type);
	      gimple copy_stmt =
		zero_sign_extend_stmt (new_def, rhs1,
				       TYPE_PRECISION (TREE_TYPE (def)));
	      gsi = gsi_for_stmt (def_stmt);
	      gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
	      gsi = gsi_for_stmt (def_stmt);
	      gsi_remove (&gsi, true);
	    }
	  else
	    {
	      new_def = make_promoted_copy (def, def_stmt, promoted_type);
	      gimple_assign_set_lhs (def_stmt, new_def);
	      update_stmt (def_stmt);
	      if (TREE_CODE_CLASS (gimple_assign_rhs_code (def_stmt))
		  != tcc_comparison)
		promote_cst_in_stmt (def_stmt, promoted_type);
	    }
	  *release_def = true;
	  *copy_of_def = NULL;
	  break;
	}

    default:
      new_def = make_promoted_copy (def, NULL, promoted_type);
      copy_stmt = gimple_build_assign (new_def, CONVERT_EXPR,
						def, NULL_TREE);
      gsi = gsi_for_stmt (def_stmt);
      if (lookup_stmt_eh_lp (def_stmt) > 0)
	insert_stmt_on_edge (def_stmt, copy_stmt);
      else
	gsi_insert_after (&gsi, copy_stmt, GSI_NEW_STMT);
      update_stmt (copy_stmt);
      *copy_of_def = def;
      break;
    }

  return new_def;
}


/* Promote all the USE with NEW_USE.  */
static unsigned int
promote_all_uses (tree use, tree new_use, tree copy_of_use,
		  tree promoted_type)
{
  gimple stmt;
  imm_use_iterator ui;
  gimple_stmt_iterator gsi;
  use_operand_p op;

  /* Replace all the use with the promoted variable.  */
  FOR_EACH_IMM_USE_STMT (stmt, ui, use)
    {
      if (stmt == SSA_NAME_DEF_STMT (new_use))
	continue;

      switch (gimple_code (stmt))
	{

	case GIMPLE_ASSIGN:
	  promote_assign_stmt_use (stmt, use, &ui, new_use,
				   copy_of_use, promoted_type);
	  break;

	case GIMPLE_PHI:
	  promote_phi_stmt_use (stmt, use, &ui, new_use,
				copy_of_use, promoted_type);
	  break;

	case GIMPLE_COND:
	  promote_cond_stmt_use (stmt, use, &ui, new_use,
				 promoted_type);
	  break;

	case GIMPLE_DEBUG:
	  if (TREE_TYPE (use) != TREE_TYPE (new_use)
	      && gimple_debug_bind_p (stmt))
	    {
	      gsi = gsi_for_stmt (stmt);
	      gsi_remove (&gsi, true);
	    }
	  break;

	default:
	  if (TREE_TYPE (use) != TREE_TYPE (new_use))
	    {
	      tree temp;
	      if (copy_of_use)
		temp = copy_of_use;
	      else
		{
		  temp = make_promoted_copy (use, NULL, TREE_TYPE (use));
		  gimple copy_stmt = gimple_build_assign (temp, CONVERT_EXPR,
							  new_use, NULL_TREE);
		  gsi = gsi_for_stmt (stmt);
		  gsi_insert_before (&gsi, copy_stmt, GSI_NEW_STMT);
		}

	      FOR_EACH_IMM_USE_ON_STMT (op, ui)
		SET_USE (op, temp);
	      update_stmt (stmt);
	    }
	  break;
	}
    }

  return 0;
}

/* Promote definition of NAME and all its uses.  */
static unsigned int
promote_def_and_uses (tree name)
{
  tree type, new_name, copy_of_name;
  bool release_def = false;

  if (TREE_CODE (name) != SSA_NAME
      || POINTER_TYPE_P (TREE_TYPE (name))
      || !INTEGRAL_TYPE_P (TREE_TYPE (name))
      || VECTOR_TYPE_P (TREE_TYPE (name))
      || ssa_promoted_p (name)
      || (type = get_promoted_type (TREE_TYPE (name))) == TREE_TYPE (name))
    return 0;

  if (promotion_safe_p (name))
    {
      new_name = promote_definition (name, type, &copy_of_name,
				     &release_def);
      promote_all_uses (name, new_name, copy_of_name, type);
    }
  else
    promote_all_uses (name, name, name, type);
  set_ssa_promoted (name);

  if (release_def)
    release_ssa_name (name);
  return 0;
}

/* Mark the candidates for promotion.  */
static void
set_ssa_to_be_promoted_flag (gimple stmt)
{
  ssa_op_iter i;
  tree def;
  use_operand_p op;

  switch (gimple_code (stmt))
    {

    case GIMPLE_PHI:
	{
	  gphi *phi = as_a <gphi *> (stmt);
	  def = PHI_RESULT (phi);
	  bitmap_set_bit (ssa_to_be_promoted_bitmap, SSA_NAME_VERSION (def));
	  FOR_EACH_PHI_ARG (op, phi, i, SSA_OP_USE)
	    {
	      def = USE_FROM_PTR (op);
	      if (TREE_CODE (def) == SSA_NAME)
		bitmap_set_bit (ssa_to_be_promoted_bitmap, SSA_NAME_VERSION (def));
	    }
	  break;
	}

    default:
      FOR_EACH_SSA_TREE_OPERAND (def, stmt, i, SSA_OP_USE | SSA_OP_DEF)
	{
	  if (TREE_CODE (def) == SSA_NAME)
	    bitmap_set_bit (ssa_to_be_promoted_bitmap, SSA_NAME_VERSION (def));
	}
      break;
    }
}

/* Visit PHI stmt and record if variables might have higher bits set if
   promoted.  */
static bool
record_visit_phi_node (gimple stmt)
{
  tree def;
  ssa_op_iter i;
  use_operand_p op;
  bool high_bits_set = false;
  gphi *phi = as_a <gphi *> (stmt);
  tree lhs = PHI_RESULT (phi);

  if (TREE_CODE (lhs) != SSA_NAME
      || POINTER_TYPE_P (TREE_TYPE (lhs))
      || !INTEGRAL_TYPE_P (TREE_TYPE (lhs))
      || ssa_overflows_p (lhs))
    return false;

  FOR_EACH_PHI_ARG (op, phi, i, SSA_OP_USE)
    {
      def = USE_FROM_PTR (op);
      if (ssa_overflows_p (def))
	high_bits_set = true;
    }

  if (high_bits_set)
    {
      set_ssa_overflows (lhs);
      return true;
    }
  else
    return false;
}

/* Visit STMT and record if variables might have higher bits set if
   promoted.  */
static bool
record_visit_stmt (gimple stmt)
{
  tree def;
  ssa_op_iter i;
  bool changed = false;
  gcc_assert (gimple_code (stmt) == GIMPLE_ASSIGN);
  enum tree_code code = gimple_assign_rhs_code (stmt);
  tree lhs = gimple_assign_lhs (stmt);
  tree rhs1 = gimple_assign_rhs1 (stmt);

  if (TREE_CODE (lhs) != SSA_NAME
      || POINTER_TYPE_P (TREE_TYPE (lhs))
      || !INTEGRAL_TYPE_P (TREE_TYPE (lhs)))
    return false;

  switch (code)
    {
      /* Conversion expressions that may need to be preserved.  */
    CASE_CONVERT:
      /* if the precision of LHS is greater than RHS, it is not safe to
	 convert this with  ZEXT/SEXT stmt when there is also type change.  */
      if ((TYPE_PRECISION (TREE_TYPE (lhs))
	   > TYPE_PRECISION (TREE_TYPE (rhs1)))
	  && (TYPE_UNSIGNED (TREE_TYPE (lhs))
	      != TYPE_PRECISION (TREE_TYPE (rhs1))))
	bitmap_set_bit (ssa_not_safe_bitmap, SSA_NAME_VERSION (lhs));
      else if ((TYPE_PRECISION (TREE_TYPE (lhs))
		<= TYPE_PRECISION (TREE_TYPE (rhs1)))
	       && !ssa_overflows_p (lhs))
	{
	  set_ssa_overflows (lhs);
	  changed = true;
	}
      break;

    case SSA_NAME:
      if (!ssa_overflows_p (lhs)
	  && ssa_overflows_p (rhs1))
	{
	  set_ssa_overflows (lhs);
	  changed = true;
	}
      break;

    case NE_EXPR:
    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case UNLT_EXPR:
    case UNLE_EXPR:
    case UNGT_EXPR:
    case UNGE_EXPR:
    case UNEQ_EXPR:
    case LTGT_EXPR:
    case RSHIFT_EXPR:
    case LSHIFT_EXPR:
    case WIDEN_LSHIFT_EXPR:
    case MIN_EXPR:
    case MAX_EXPR:
    case RANGE_EXPR:
      break;

    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case RDIV_EXPR:
    case ROUND_DIV_EXPR:
    case EXACT_DIV_EXPR:
      if (!ssa_overflows_p (lhs))
	{
	  set_ssa_overflows (lhs);
	  changed = true;
	}
      break;

      /* Expressions which may produce results that will have higher bits if
	 computed in promoted type.  (i.e.  results may overflow) */
    case MULT_HIGHPART_EXPR:
    case PLUS_EXPR:
    case MINUS_EXPR:
    case MULT_EXPR:
    case BIT_XOR_EXPR:
    case BIT_NOT_EXPR:
    case WIDEN_MULT_EXPR:
    case WIDEN_MULT_PLUS_EXPR:
    case WIDEN_MULT_MINUS_EXPR:
    case WIDEN_SUM_EXPR:
    case BIT_IOR_EXPR:
    case BIT_AND_EXPR:
      if (!ssa_overflows_p (lhs))
	{
	  set_ssa_overflows (lhs);
	  changed = true;
	}
      break;

      /* Expressions for which operation has to be performed in original
	 types if promoted operands may have higher bits.  */
    case ABS_EXPR:
    case NEGATE_EXPR:
    case TRUNC_MOD_EXPR:
    case CEIL_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case ROUND_MOD_EXPR:
      FOR_EACH_SSA_TREE_OPERAND (def, stmt, i, SSA_OP_USE)
	{
	  if (ssa_overflows_p (def))
	    bitmap_set_bit (ssa_not_safe_bitmap, SSA_NAME_VERSION (lhs));
	}
      break;

    case COND_EXPR:
	{
	  tree rhs2 = gimple_assign_rhs2 (stmt);
	  tree rhs3 = gimple_assign_rhs3 (stmt);

	  if (ssa_overflows_p (rhs2))
	    {
	      set_ssa_overflows (lhs);
	      changed = true;
	    }
	  else if (ssa_overflows_p (rhs3))
	    {
	      set_ssa_overflows (lhs);
	      changed = true;
	    }
	}
      break;

      /* Expressions that has to be done in original types.  */
    case LROTATE_EXPR:
    case RROTATE_EXPR:
      bitmap_set_bit (ssa_not_safe_bitmap, SSA_NAME_VERSION (lhs));
      break;

      /* To be safe, all other have to be done in original types.  */
    default:
      bitmap_set_bit (ssa_not_safe_bitmap, SSA_NAME_VERSION (lhs));
      break;
    }
  return changed;
}


/* Promote all the stmts in the basic block.  */
static void
promote_all_stmts (basic_block bb)
{
  gimple_stmt_iterator gsi;
  ssa_op_iter iter;
  tree def;

  for (gphi_iterator gpi = gsi_start_phis (bb);
       !gsi_end_p (gpi); gsi_next (&gpi))
    {
      gphi *phi = gpi.phi ();
      use_operand_p op;

      def = PHI_RESULT (phi);
      promote_def_and_uses (def);
      FOR_EACH_PHI_ARG (op, phi, iter, SSA_OP_USE)
	{
	  def = USE_FROM_PTR (op);
	  promote_def_and_uses (def);
	}
    }
  for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
    {
      gimple stmt = gsi_stmt (gsi);

      FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_USE | SSA_OP_DEF)
	promote_def_and_uses (def);
    }
}

static void
process_all_stmts_for_unsafe_promotion ()
{
  basic_block bb;
  gimple_stmt_iterator gsi;
  auto_vec<gimple> work_list;

  FOR_EACH_BB_FN (bb, cfun)
    {
      for (gsi = gsi_start_phis (bb); !gsi_end_p (gsi); gsi_next (&gsi))
	{
	  gimple phi = gsi_stmt (gsi);

	  set_ssa_to_be_promoted_flag (phi);
	  work_list.safe_push (phi);
	}

      for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
	{
	  gimple stmt = gsi_stmt (gsi);

	  set_ssa_to_be_promoted_flag (stmt);
	  if (gimple_code (stmt) == GIMPLE_ASSIGN)
	    work_list.safe_push (stmt);
	}
    }

  while (work_list.length () > 0)
    {
      bool changed;
      gimple stmt = work_list.pop ();
      tree lhs;

      switch (gimple_code (stmt))
	{

	case GIMPLE_ASSIGN:
	  changed = record_visit_stmt (stmt);
	  lhs = gimple_assign_lhs (stmt);
	  break;

	case GIMPLE_PHI:
	  changed = record_visit_phi_node (stmt);
	  lhs = PHI_RESULT (stmt);
	  break;

	default:
	  gcc_assert (false);
	  break;
	}

      if (changed)
	{
	  gimple use_stmt;
	  imm_use_iterator ui;

	  FOR_EACH_IMM_USE_STMT (use_stmt, ui, lhs)
	    {
	      if (gimple_code (use_stmt) == GIMPLE_ASSIGN
		  || gimple_code (use_stmt) == GIMPLE_PHI)
		work_list.safe_push (use_stmt);
	    }
	}
    }
}

class type_promotion_dom_walker : public dom_walker
{
public:
  type_promotion_dom_walker (cdi_direction direction)
    : dom_walker (direction) {}
  virtual void before_dom_children (basic_block bb)
    {
      promote_all_stmts (bb);
    }
};

/* Main entry point to the pass.  */
static unsigned int
execute_type_promotion (void)
{
  n_ssa_val = num_ssa_names;
  ssa_not_safe_bitmap = sbitmap_alloc (n_ssa_val);
  bitmap_clear (ssa_not_safe_bitmap);
  ssa_to_be_promoted_bitmap = sbitmap_alloc (n_ssa_val);
  bitmap_clear (ssa_to_be_promoted_bitmap);
  ssa_sets_higher_bits_bitmap = sbitmap_alloc (n_ssa_val);
  bitmap_clear (ssa_sets_higher_bits_bitmap);

  calculate_dominance_info (CDI_DOMINATORS);
  process_all_stmts_for_unsafe_promotion ();
  /* Walk the CFG in dominator order.  */
  type_promotion_dom_walker (CDI_DOMINATORS)
    .walk (ENTRY_BLOCK_PTR_FOR_FN (cfun));

  sbitmap_free (ssa_not_safe_bitmap);
  sbitmap_free (ssa_to_be_promoted_bitmap);
  sbitmap_free (ssa_sets_higher_bits_bitmap);
    free_dominance_info (CDI_DOMINATORS);
  return 0;
}

namespace {
const pass_data pass_data_type_promotion =
{
  GIMPLE_PASS, /* type */
  "promotion", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_TREE_TYPE_PROMOTE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  (TODO_cleanup_cfg | TODO_update_ssa | TODO_verify_all),
};

class pass_type_promotion : public gimple_opt_pass
{
public:
  pass_type_promotion (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_type_promotion, ctxt)
  {}

  /* opt_pass methods: */
  opt_pass * clone () { return new pass_type_promotion (m_ctxt); }
  virtual bool gate (function *) { return flag_tree_type_promote != 0; }
  virtual unsigned int execute (function *)
    {
      return execute_type_promotion ();
    }

}; // class pass_type_promotion

} // anon namespace

gimple_opt_pass *
make_pass_type_promote (gcc::context *ctxt)
{
  return new pass_type_promotion (ctxt);
}

