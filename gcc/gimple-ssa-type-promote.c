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
#include "backend.h"
#include "hash-set.h"
#include "machmode.h"
#include "vec.h"
#include "double-int.h"
#include "input.h"
#include "symtab.h"
#include "wide-int.h"
#include "inchash.h"
#include "tree.h"
#include "fold-const.h"
#include "stor-layout.h"
#include "predict.h"
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
#include "tree-dfa.h"
#include "tree-ssa-loop-niter.h"

/* This pass applies type promotion to SSA names in the function and
   inserts appropriate truncations.  Idea of this pass is to promote operations
   such a way that we can minimise generation of subreg in RTL,
   that in turn results in removal of redundant zero/sign extensions.  This pass
   will run prior to The VRP and DOM such that they will be able to optimise
   redundant truncations and extensions.  This is based on the discussion from
   https://gcc.gnu.org/ml/gcc-patches/2014-09/msg00472.html.
*/

/* Structure to hold the type and promoted type for promoted ssa variables.  */
struct ssa_name_info
{
  tree ssa;		/* Name of the SSA_NAME.  */
  tree val_1;		/* Value in original type, if present.  */
  tree type;		/* Original type of ssa.  */
  tree promoted_type;	/* Promoted type of ssa.  */
};

/* Obstack for ssa_name_info.  */
static struct obstack ssa_name_info_obstack;

static unsigned n_ssa_val;
static sbitmap ssa_to_be_promoted_bitmap;
static hash_map <tree, ssa_name_info *> *ssa_name_info_map;

/* Is the type precision ok for promition.  */
static bool
type_precision_ok (tree type)
{
  return (TYPE_PRECISION (type)
	  == GET_MODE_PRECISION (TYPE_MODE (type)));
}

/* Return the promoted type for TYPE.  */
static tree
get_promoted_type (tree type)
{
  tree promoted_type;
  enum machine_mode mode;
  int uns;

  if (POINTER_TYPE_P (type)
      || !INTEGRAL_TYPE_P (type)
      || !type_precision_ok (type))
    return type;

  mode = TYPE_MODE (type);
#ifdef PROMOTE_MODE
  uns = TYPE_SIGN (type);
  PROMOTE_MODE (mode, uns, type);
#endif
  uns = TYPE_SIGN (type);
  if (TYPE_PRECISION (type) == GET_MODE_PRECISION (mode))
    return type;
  promoted_type
    = build_nonstandard_integer_type (GET_MODE_PRECISION (mode),
				      uns);
  gcc_assert (TYPE_PRECISION (promoted_type) == GET_MODE_PRECISION (mode));
  return promoted_type;
}

/* Return true if ssa NAME is already considered for promotion.  */
static bool
ssa_promoted_p (tree name)
{
  gcc_assert (TREE_CODE (name) == SSA_NAME);
  unsigned int index = SSA_NAME_VERSION (name);
  if (index < n_ssa_val)
    return bitmap_bit_p (ssa_to_be_promoted_bitmap, index);
  return true;
}

/* Set ssa NAME as considered for promotion.  */
static void
set_ssa_promoted (tree name)
{
  gcc_assert (TREE_CODE (name) == SSA_NAME);
  unsigned int index = SSA_NAME_VERSION (name);
  if (index < n_ssa_val)
    bitmap_set_bit (ssa_to_be_promoted_bitmap, index);
}

/* Return true if the tree CODE does not require the propmoted operand
   to be truncated (when stray bits are set beyond the original type in
   promoted mode) to preserve the semantics.  */
static bool
not_needed_truncated_operand_p (enum tree_code code)
{
  if (TREE_CODE_CLASS (code) == tcc_comparison
      || code == TRUNC_DIV_EXPR
      || code == CEIL_DIV_EXPR
      || code == FLOOR_DIV_EXPR
      || code == ROUND_DIV_EXPR
      || code == TRUNC_MOD_EXPR
      || code == CEIL_MOD_EXPR
      || code == FLOOR_MOD_EXPR
      || code == ROUND_MOD_EXPR
      || code == LSHIFT_EXPR
      || code == RSHIFT_EXPR
      || code == MAX_EXPR
      || code == MIN_EXPR)
    return false;
  else
    return true;
}

/* Return true if LHS will be promoted later.  */
static bool
tobe_promoted_p (tree lhs)
{
  if (TREE_CODE (lhs) == SSA_NAME
      && INTEGRAL_TYPE_P (TREE_TYPE (lhs))
      && !VECTOR_TYPE_P (TREE_TYPE (lhs))
      && !POINTER_TYPE_P (TREE_TYPE (lhs))
      && !ssa_promoted_p (lhs)
      && (get_promoted_type (TREE_TYPE (lhs))
	  != TREE_TYPE (lhs)))
    return true;
  else
    return false;
}

/* Convert and sign-extend constant CST to TYPE.  */
static tree
fold_convert_sext (tree type, tree cst)
{
  wide_int wi_cons = fold_convert (type, cst);
  wi_cons = wi::ext (wi_cons, TYPE_PRECISION (TREE_TYPE (cst)), SIGNED);
  return wide_int_to_tree (type, wi_cons);
}

/* Promote constants in STMT to TYPE.  */

static void
promote_cst_in_stmt (gimple *stmt, tree type)
{
  tree op;
  ssa_op_iter iter;
  use_operand_p oprnd;
  int index;
  tree op0, op1;

  switch (gimple_code (stmt))
    {
    case GIMPLE_ASSIGN:
      if (gimple_assign_rhs_code (stmt) == COND_EXPR
	  && TREE_OPERAND_LENGTH (gimple_assign_rhs1 (stmt)) == 2)
	{
	  /* Promote INTEGER_CST that are tcc_compare arguments.  */
	  op = gimple_assign_rhs1 (stmt);
	  op0 = TREE_OPERAND (op, 0);
	  op1 = TREE_OPERAND (op, 1);
	  if (TREE_TYPE (op0) != TREE_TYPE (op1))
	    {
	      if (TREE_CODE (op0) == INTEGER_CST)
		TREE_OPERAND (op, 0) = fold_convert (type, op0);
	      if (TREE_CODE (op1) == INTEGER_CST)
		TREE_OPERAND (op, 1) = fold_convert (type, op1);
	    }
	}
      /* Promote INTEGER_CST in GIMPLE_ASSIGN.  */
      if (not_needed_truncated_operand_p (gimple_assign_rhs_code (stmt)))
	{
	  op = gimple_assign_rhs3 (stmt);
	  if (op && TREE_CODE (op) == INTEGER_CST)
	    gimple_assign_set_rhs3 (stmt, fold_convert_sext (type, op));
	  op = gimple_assign_rhs1 (stmt);
	  if (op && TREE_CODE (op) == INTEGER_CST)
	    gimple_assign_set_rhs1 (stmt, fold_convert_sext (type, op));
	  op = gimple_assign_rhs2 (stmt);
	  if (op && TREE_CODE (op) == INTEGER_CST)
	    gimple_assign_set_rhs2 (stmt, fold_convert_sext (type, op));
	}
      else
	{
	  op = gimple_assign_rhs3 (stmt);
	  if (op && TREE_CODE (op) == INTEGER_CST)
	    gimple_assign_set_rhs3 (stmt, fold_convert (type, op));
	  op = gimple_assign_rhs1 (stmt);
	  if (op && TREE_CODE (op) == INTEGER_CST)
	    gimple_assign_set_rhs1 (stmt, fold_convert (type, op));
	  op = gimple_assign_rhs2 (stmt);
	  if (op && TREE_CODE (op) == INTEGER_CST)
	    gimple_assign_set_rhs2 (stmt, fold_convert (type, op));
	}
      break;

    case GIMPLE_PHI:
	{
	  /* Promote INTEGER_CST arguments to GIMPLE_PHI.  */
	  gphi *phi = as_a <gphi *> (stmt);
	  FOR_EACH_PHI_ARG (oprnd, phi, iter, SSA_OP_USE)
	    {
	      op = USE_FROM_PTR (oprnd);
	      index = PHI_ARG_INDEX_FROM_USE (oprnd);
	      if (TREE_CODE (op) == INTEGER_CST)
		SET_PHI_ARG_DEF (phi, index, fold_convert (type, op));
	    }
	}
      break;

    case GIMPLE_COND:
	{
	  /* Promote INTEGER_CST that are GIMPLE_COND arguments.  */
	  gcond *cond = as_a <gcond *> (stmt);
	  op = gimple_cond_lhs (cond);
	  if (op && TREE_CODE (op) == INTEGER_CST)
	    gimple_cond_set_lhs (cond, fold_convert (type, op));

	  op = gimple_cond_rhs (cond);
	  if (op && TREE_CODE (op) == INTEGER_CST)
	    gimple_cond_set_rhs (cond, fold_convert (type, op));
	}
      break;

    default:
      gcc_unreachable ();
    }
}

/* Zero/sign extend VAR and truncate to INNER_TYPE. Assign the zero/sign
   extended value in NEW_VAR.  Gimple statement that performs the zero/sign
   extension is returned.  */

static gimple *
zero_sign_extend_stmt (tree new_var, tree var, tree inner_type)
{
  gcc_assert (TYPE_PRECISION (TREE_TYPE (var))
	      == TYPE_PRECISION (TREE_TYPE (new_var)));
  gcc_assert (TYPE_PRECISION (TREE_TYPE (var)) > TYPE_PRECISION (inner_type));
  gimple *stmt;

  if (TYPE_UNSIGNED (inner_type))
    {
      /* Zero extend.  */
      tree cst
	= wide_int_to_tree (TREE_TYPE (var),
			    wi::mask (TYPE_PRECISION (inner_type), false,
				      TYPE_PRECISION (TREE_TYPE (var))));
      stmt = gimple_build_assign (new_var, BIT_AND_EXPR, var, cst);
    }
  else
    /* Sign extend.  */
    stmt = gimple_build_assign (new_var, SEXT_EXPR, var,
				build_int_cst (TREE_TYPE (var),
					       TYPE_PRECISION (inner_type)));
  return stmt;
}

/* Copy default ssa definition FORM to TO. Used as a helper before promoting
   the SSA.  */
static void
copy_default_ssa (tree to, tree from)
{
  SET_SSA_NAME_VAR_OR_IDENTIFIER (to, SSA_NAME_VAR (from));
  SSA_NAME_DEF_STMT (to) = SSA_NAME_DEF_STMT (from);
  SET_SSA_NAME_VAR_OR_IDENTIFIER (from, NULL_TREE);
  SSA_NAME_IS_DEFAULT_DEF (to) = 1;
  SSA_NAME_IS_DEFAULT_DEF (from) = 0;
}

/* Return true if SSA defintion in DEF_STMT can be promoted, return false
   otherwise.  */
static bool
do_not_promote_def_p (gimple *def_stmt)
{
  enum tree_code code = gimple_assign_rhs_code (def_stmt);
  tree def = gimple_assign_lhs (def_stmt);
  tree rhs = gimple_assign_rhs1 (def_stmt);
  if (gimple_vuse (def_stmt) != NULL_TREE
      || gimple_vdef (def_stmt) != NULL_TREE
      || (ANY_INTEGRAL_TYPE_P (TREE_TYPE (def))
	  && !operation_no_trapping_overflow (TREE_TYPE (def), code))
      || TREE_CODE_CLASS (code) == tcc_reference
      || TREE_CODE_CLASS (code) == tcc_comparison
      || code == LROTATE_EXPR
      || code == RROTATE_EXPR
      || code == VIEW_CONVERT_EXPR
      || code == REALPART_EXPR
      || code == IMAGPART_EXPR
      || code == REDUC_PLUS_EXPR
      || code == REDUC_MAX_EXPR
      || code == REDUC_MIN_EXPR
      || !INTEGRAL_TYPE_P (TREE_TYPE (rhs)))
    return true;
  return false;
}

/* Promote definition DEF to promoted type.  If the stmt that defines def
   is def_stmt, make the type of def promoted type.  If the stmt is such
   that, result of the def_stmt cannot be of promoted type, create a new_def
   of the original_type and make the def_stmt assign its value to newdef.
   Then, create a NOP_EXPR to convert new_def to def of promoted type.

   For example, for stmt with original_type char and promoted_type int:
		char _1 = mem;
	becomes:
		char _2 = mem;
		int _1 = (int)_2;

   If the def_stmt allows def to be promoted, promote def in-place
   (and its arguments when needed).

   For example:
		char _3 = _1 + _2;
	becomes:
		int _3 = _1 + _2;
   Here, _1 and _2 will also be promoted.  */

static bool
promote_ssa (tree def, gimple_stmt_iterator *gsi)
{
  gimple *def_stmt = SSA_NAME_DEF_STMT (def);
  gimple *copy_stmt = NULL;
  gimple_stmt_iterator gsi2;
  tree original_type = TREE_TYPE (def);
  tree new_def = NULL_TREE;
  ssa_name_info *info;
  bool do_not_promote = false;
  tree promoted_type = get_promoted_type (TREE_TYPE (def));

  if (!tobe_promoted_p (def))
    return true;

  info = (ssa_name_info *) obstack_alloc (&ssa_name_info_obstack,
					  sizeof (ssa_name_info));
  info->type = original_type;
  info->promoted_type = promoted_type;
  info->ssa = def;
  info->val_1 = NULL_TREE;
  ssa_name_info_map->put (def, info);

  switch (gimple_code (def_stmt))
    {
    case GIMPLE_PHI:
      {
	/* Promote def by fixing its type and make def anonymous.  */
	TREE_TYPE (def) = promoted_type;
	SET_SSA_NAME_VAR_OR_IDENTIFIER (def, NULL_TREE);
	promote_cst_in_stmt (def_stmt, promoted_type);
	break;
      }

    case GIMPLE_ASM:
      {
	gasm *asm_stmt = as_a <gasm *> (def_stmt);
	for (unsigned int i = 0; i < gimple_asm_noutputs (asm_stmt); ++i)
	  {
	    /* Promote def and copy (i.e. convert) the value defined
	       by asm to def.  */
	    tree link = gimple_asm_output_op (asm_stmt, i);
	    tree op = TREE_VALUE (link);
	    if (op == def)
	      {
		new_def = copy_ssa_name (def);
		set_ssa_promoted (new_def);
		copy_default_ssa (new_def, def);
		TREE_VALUE (link) = new_def;
		gimple_asm_set_output_op (asm_stmt, i, link);

		TREE_TYPE (def) = promoted_type;
		copy_stmt = gimple_build_assign (def, NOP_EXPR, new_def);
		SSA_NAME_IS_DEFAULT_DEF (new_def) = 0;
		gimple_set_location (copy_stmt, gimple_location (def_stmt));
		gsi2 = gsi_for_stmt (def_stmt);
		gsi_insert_after (&gsi2, copy_stmt, GSI_NEW_STMT);
		break;
	      }
	  }
	break;
      }

    case GIMPLE_NOP:
      {
	gcc_unreachable ();
      }

    case GIMPLE_ASSIGN:
      {
	enum tree_code code = gimple_assign_rhs_code (def_stmt);
	tree rhs = gimple_assign_rhs1 (def_stmt);
	if (do_not_promote_def_p (def_stmt))
	  {
	    do_not_promote = true;
	  }
	else if (CONVERT_EXPR_CODE_P (code))
	  {
	    if (!type_precision_ok (TREE_TYPE (rhs)))
	      {
		do_not_promote = true;
	      }
	    else if (types_compatible_p (TREE_TYPE (rhs), promoted_type))
	      {
		/* As we travel statements in dominated order, arguments
		   of def_stmt will be visited before visiting def.  If RHS
		   is already promoted and type is compatible, we can convert
		   them into ZERO/SIGN EXTEND stmt.  */
		ssa_name_info *info = ssa_name_info_map->get_or_insert (rhs);
		tree type;
		if (info == NULL)
		  type = TREE_TYPE (rhs);
		else
		  type = info->type;
		if ((TYPE_PRECISION (original_type)
		     > TYPE_PRECISION (type))
		    || (TYPE_UNSIGNED (original_type)
			!= TYPE_UNSIGNED (type)))
		  {
		    if (TYPE_PRECISION (original_type) < TYPE_PRECISION (type))
		      type = original_type;
		    gcc_assert (type != NULL_TREE);
		    TREE_TYPE (def) = promoted_type;
		    copy_stmt = zero_sign_extend_stmt (def, rhs, type);
		    SET_SSA_NAME_VAR_OR_IDENTIFIER (def, NULL_TREE);
		    gsi_replace (gsi, copy_stmt, false);
		  }
		else
		  {
		    TREE_TYPE (def) = promoted_type;
		    SET_SSA_NAME_VAR_OR_IDENTIFIER (def, NULL_TREE);
		  }
	      }
	    else
	      {
		/* If RHS is not promoted OR their types are not
		   compatible, create NOP_EXPR that converts
		   RHS to  promoted DEF type and perform a
		   ZERO/SIGN EXTEND to get the required value
		   from RHS.  */
		ssa_name_info *info = ssa_name_info_map->get_or_insert (rhs);
		if (info != NULL)
		  {
		    tree type = info->type;
		    new_def = copy_ssa_name (rhs);
		    SET_SSA_NAME_VAR_OR_IDENTIFIER (new_def, NULL_TREE);
		    TREE_TYPE (def) = promoted_type;
		    SET_SSA_NAME_VAR_OR_IDENTIFIER (def, NULL_TREE);
		    copy_stmt = zero_sign_extend_stmt (new_def, rhs, type);
		    gimple_set_location (copy_stmt, gimple_location (def_stmt));
		    gsi2 = gsi_for_stmt (def_stmt);
		    gsi_insert_before (&gsi2, copy_stmt, GSI_NEW_STMT);
		    gassign *new_def_stmt = gimple_build_assign (def, code, new_def);
		    gsi_replace (gsi, new_def_stmt, false);
		  }
		else
		  {
		    TREE_TYPE (def) = promoted_type;
		    SET_SSA_NAME_VAR_OR_IDENTIFIER (def, NULL_TREE);
		  }
	      }
	  }
	else
	  {
	    /* Promote def by fixing its type and make def anonymous.  */
	    promote_cst_in_stmt (def_stmt, promoted_type);
	    SET_SSA_NAME_VAR_OR_IDENTIFIER (def, NULL_TREE);
	    TREE_TYPE (def) = promoted_type;
	  }
	break;
      }

    default:
    do_not_promote = true;
    break;
    }

  if (do_not_promote)
    {
      /* Promote def and copy (i.e. convert) the value defined
	 by the stmt that cannot be promoted.  */
      if (lookup_stmt_eh_lp (def_stmt) > 0
	  || (gimple_code (def_stmt) == GIMPLE_CALL
	      && gimple_call_ctrl_altering_p (def_stmt)))
	{
	  edge_iterator ei;
	  edge e;
	  int count = 0;

	  FOR_EACH_EDGE (e, ei, gimple_bb (def_stmt)->succs)
	    {
	      if (!(e->flags & EDGE_ABNORMAL))
		count ++;
	    }
	  if (count == 1)
	    {
	      new_def = copy_ssa_name (def);
	      set_ssa_promoted (new_def);
	      SET_SSA_NAME_VAR_OR_IDENTIFIER (def, NULL_TREE);
	      TREE_TYPE (def) = promoted_type;
	      gimple_set_lhs (def_stmt, new_def);
	      copy_stmt = gimple_build_assign (def, NOP_EXPR, new_def);
	      gimple_set_location (copy_stmt, gimple_location (def_stmt));
	      gsi_insert_on_edge (FALLTHRU_EDGE (gimple_bb (def_stmt)),
				  copy_stmt);
	      info->val_1 = new_def;
	    }
	  else
	    {
	      info->val_1 = def;
	      return false;
	    }
	}
      else
	{
	  new_def = copy_ssa_name (def);
	  set_ssa_promoted (new_def);
	  SET_SSA_NAME_VAR_OR_IDENTIFIER (def, NULL_TREE);
	  TREE_TYPE (def) = promoted_type;
	  gimple_set_lhs (def_stmt, new_def);
	  copy_stmt = gimple_build_assign (def, NOP_EXPR, new_def);
	  gimple_set_location (copy_stmt, gimple_location (def_stmt));
	  gsi2 = gsi_for_stmt (def_stmt);
	  gsi_insert_after (&gsi2, copy_stmt, GSI_NEW_STMT);
	  info->val_1 = new_def;
	}
    }
  reset_flow_sensitive_info (def);
  if (new_def && TREE_NO_WARNING (def))
    TREE_NO_WARNING (new_def) = 1;
  return true;
}

static bool
do_not_promote_use_p (gimple *stmt)
{
  enum tree_code code = gimple_assign_rhs_code (stmt);
  tree lhs = gimple_assign_lhs (stmt);
  if (gimple_vuse (stmt) != NULL_TREE
      || gimple_vdef (stmt) != NULL_TREE
      || (ANY_INTEGRAL_TYPE_P (TREE_TYPE (lhs))
	  && !operation_no_trapping_overflow (TREE_TYPE (lhs), code))
      || code == VIEW_CONVERT_EXPR
      || code == LROTATE_EXPR
      || code == RROTATE_EXPR
      || code == CONSTRUCTOR
      || code == BIT_FIELD_REF
      || code == COMPLEX_EXPR
      || VECTOR_TYPE_P (TREE_TYPE (lhs)))
    return true;
  return false;
}

/* Fix the (promoted) USE in stmts where USE cannot be be promoted.  */
static unsigned int
fixup_use (gimple *stmt, gimple_stmt_iterator *gsi,
	   use_operand_p op, tree use, int ind)
{
  gimple *copy_stmt;
  ssa_name_info **info = ssa_name_info_map->get (use);
  /* If USE is not promoted, nothing to do.  */
  if (!info || *info == NULL)
    return 0;

  tree promoted_type = (*info)->promoted_type;
  tree old_type = (*info)->type;
  bool do_not_promote = false;

  switch (gimple_code (stmt))
    {
    case GIMPLE_DEBUG:
	{
	  SET_USE (op, fold_convert (old_type, use));
	  update_stmt (stmt);
	  break;
	}

    case GIMPLE_ASM:
    case GIMPLE_CALL:
    case GIMPLE_RETURN:
    case GIMPLE_SWITCH:
	{
	  /* USE cannot be promoted here.  */
	  do_not_promote = true;
	  break;
	}

    case GIMPLE_ASSIGN:
	{
	  enum tree_code code = gimple_assign_rhs_code (stmt);
	  tree lhs = gimple_assign_lhs (stmt);
	  if (do_not_promote_use_p (stmt))
	    {
	      do_not_promote = true;
	    }
	  else if (TREE_TYPE (use) == old_type)
	    {
	      if (TREE_CODE_CLASS (code) == tcc_comparison)
		promote_cst_in_stmt (stmt, promoted_type);
	      tree temp = make_ssa_name (promoted_type, NULL);
	      copy_stmt = gimple_build_assign (temp, NOP_EXPR, use);
	      gimple_set_location (copy_stmt, gimple_location (stmt));
	      gsi_insert_before (gsi, copy_stmt, GSI_NEW_STMT);

	      SET_USE (op, temp);
	      update_stmt (stmt);
	    }
	  else if (!not_needed_truncated_operand_p (code))
	    {
	      /* Promote the constant in comparison when other comparison
		 operand is promoted.  All other constants are promoted as
		 part of promoting definition in promote_ssa.  */
	      if (TREE_CODE_CLASS (code) == tcc_comparison)
		promote_cst_in_stmt (stmt, promoted_type);
	      /* In some stmts, value in USE has to be ZERO/SIGN
		 Extended based on the original type for correct
		 result.  */
	      tree temp = make_ssa_name (TREE_TYPE (use), NULL);
	      copy_stmt = zero_sign_extend_stmt (temp, use, old_type);
	      gimple_set_location (copy_stmt, gimple_location (stmt));
	      gsi_insert_before (gsi, copy_stmt, GSI_NEW_STMT);

	      SET_USE (op, temp);
	      update_stmt (stmt);
	    }
	  else if (CONVERT_EXPR_CODE_P (code)
		   || code == FLOAT_EXPR)
	    {
	      if (types_compatible_p (TREE_TYPE (lhs), promoted_type))
		{
		  /* Type of LHS and promoted RHS are compatible, we can
		     convert this into ZERO/SIGN EXTEND stmt.  */
		  copy_stmt = zero_sign_extend_stmt (lhs, use, old_type);
		  gimple_set_location (copy_stmt, gimple_location (stmt));
		  set_ssa_promoted (lhs);
		  gsi_replace (gsi, copy_stmt, false);
		}
	      else if (!tobe_promoted_p (lhs)
		       || !INTEGRAL_TYPE_P (TREE_TYPE (lhs))
		       || (TYPE_UNSIGNED (TREE_TYPE (use)) != TYPE_UNSIGNED (TREE_TYPE (lhs))))
		{
		  tree temp = make_ssa_name (TREE_TYPE (use), NULL);
		  copy_stmt = zero_sign_extend_stmt (temp, use, old_type);
		  gimple_set_location (copy_stmt, gimple_location (stmt));
		  gsi_insert_before (gsi, copy_stmt, GSI_NEW_STMT);
		  SET_USE (op, temp);
		  update_stmt (stmt);
		}
	    }
	  break;
	}

    case GIMPLE_COND:
      if (TREE_TYPE (use) == old_type)
	{
	  tree temp = make_ssa_name (promoted_type, NULL);
	  copy_stmt = gimple_build_assign (temp, NOP_EXPR, use);
	  gimple_set_location (copy_stmt, gimple_location (stmt));
	  gsi_insert_before (gsi, copy_stmt, GSI_NEW_STMT);

	  SET_USE (op, temp);
	  promote_cst_in_stmt (stmt, promoted_type);
	  update_stmt (stmt);
	}
      else
	{
	  /* In GIMPLE_COND, value in USE has to be ZERO/SIGN
	     Extended based on the original type for correct
	     result.  */
	  tree temp = make_ssa_name (TREE_TYPE (use), NULL);
	  copy_stmt = zero_sign_extend_stmt (temp, use, old_type);
	  gimple_set_location (copy_stmt, gimple_location (stmt));
	  gsi_insert_before (gsi, copy_stmt, GSI_NEW_STMT);
	  SET_USE (op, temp);
	  promote_cst_in_stmt (stmt, promoted_type);
	  update_stmt (stmt);
	}
      break;
    case GIMPLE_PHI:
      if (!types_compatible_p (TREE_TYPE (use), promoted_type))
	{
	  tree temp = make_ssa_name (promoted_type, NULL);
	  copy_stmt = gimple_build_assign (temp, NOP_EXPR, use);
	  vec<edge, va_gc> *preds = gimple_bb (stmt)->preds;
	  edge e = (*preds)[ind];
	  gsi_insert_on_edge (e, copy_stmt);

	  SET_USE (op, temp);
	  update_stmt (stmt);
	}
      break;

    default:
      break;
    }

  if (do_not_promote)
    {
      /* FOR stmts where USE cannot be promoted, create an
	 original type copy.  */
      tree temp = (*info)->val_1;
      if (temp  == NULL_TREE)
	{
	  temp = copy_ssa_name (use);
	  SET_SSA_NAME_VAR_OR_IDENTIFIER (temp, NULL_TREE);
	  set_ssa_promoted (temp);
	  TREE_TYPE (temp) = old_type;
	  copy_stmt = gimple_build_assign (temp, NOP_EXPR, use);
	  gimple_set_location (copy_stmt, gimple_location (stmt));
	  gsi_insert_before (gsi, copy_stmt, GSI_NEW_STMT);
	}
      SET_USE (op, temp);
      update_stmt (stmt);
    }
  return 0;
}

static void
promote_all_ssa_defined_with_nop ()
{
  unsigned n = num_ssa_names, i;
  gimple_stmt_iterator gsi2;
  tree new_def;
  basic_block bb;
  gimple *copy_stmt;

  for (i = 1; i < n; ++i)
    {
      tree name = ssa_name (i);
      if (name
	  && gimple_code (SSA_NAME_DEF_STMT (name)) == GIMPLE_NOP
	  && tobe_promoted_p (name)
	  && !has_zero_uses (name))
	{
	  tree promoted_type = get_promoted_type (TREE_TYPE (name));
	  ssa_name_info *info;
	  set_ssa_promoted (name);
	  info = (ssa_name_info *) obstack_alloc (&ssa_name_info_obstack,
						  sizeof (ssa_name_info));
	  info->type = TREE_TYPE (name);
	  info->promoted_type = promoted_type;
	  info->ssa = name;
	  info->val_1 = NULL_TREE;
	  ssa_name_info_map->put (name, info);

	  if (SSA_NAME_VAR (name) == NULL)
	    {
	      /* Promote def by fixing its type for anonymous def.  */
	      TREE_TYPE (name) = promoted_type;
	    }
	  else if (TREE_CODE (SSA_NAME_VAR (name)) != PARM_DECL)
	    {
	      bool no_warn = false;
	      location_t loc = DECL_SOURCE_LOCATION (SSA_NAME_VAR (name));
	      if (TREE_NO_WARNING (SSA_NAME_VAR (name)))
		no_warn = true;
	      tree var = create_tmp_reg (promoted_type);
	      DECL_NAME (var) = DECL_NAME (SSA_NAME_VAR (name));
	      set_ssa_default_def (cfun, SSA_NAME_VAR (name), NULL_TREE);
	      TREE_TYPE (name) = promoted_type;
	      SET_SSA_NAME_VAR_OR_IDENTIFIER (name, var);
	      DECL_SOURCE_LOCATION (SSA_NAME_VAR (name)) = loc;
	      set_ssa_default_def (cfun, var, name);
	      if (no_warn)
		TREE_NO_WARNING (var) = 1;
	    }
	  else
	    {
	      /* Create a promoted copy of parameters.  */
	      bb = single_succ (ENTRY_BLOCK_PTR_FOR_FN (cfun));
	      gcc_assert (bb);
	      gsi2 = gsi_after_labels (bb);
	      /* Create new_def of the original type and set that to be the
		 parameter.  */
	      new_def = copy_ssa_name (name);
	      set_ssa_promoted (new_def);
	      set_ssa_default_def (cfun, SSA_NAME_VAR (name), new_def);
	      copy_default_ssa (new_def, name);

	      /* Now promote the def and copy the value from parameter.  */
	      TREE_TYPE (name) = promoted_type;
	      copy_stmt = gimple_build_assign (name, NOP_EXPR, new_def);
	      SSA_NAME_DEF_STMT (name) = copy_stmt;
	      gsi_insert_before (&gsi2, copy_stmt, GSI_NEW_STMT);
	      info->val_1 = new_def;
	    }
	  reset_flow_sensitive_info (name);
	}
    }
}

/* Promote all the stmts in the basic block.  */
static void
promote_all_stmts (basic_block bb)
{
  gimple_stmt_iterator gsi;
  ssa_op_iter iter;
  tree def, use;
  use_operand_p op;

  for (gphi_iterator gpi = gsi_start_phis (bb);
       !gsi_end_p (gpi); gsi_next (&gpi))
    {
      gphi *phi = gpi.phi ();
      int ind = 0;
      FOR_EACH_PHI_ARG (op, phi, iter, SSA_OP_USE)
	{
	  use = USE_FROM_PTR (op);
	  fixup_use (phi, &gsi, op, use, ind++);
	}

      def = PHI_RESULT (phi);
      if (tobe_promoted_p (def))
	promote_ssa (def, &gsi);
    }

  for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
    {
      gimple *stmt = gsi_stmt (gsi);
      FOR_EACH_SSA_USE_OPERAND (op, stmt, iter, SSA_OP_USE)
	{
	  use = USE_FROM_PTR (op);
	  fixup_use (stmt, &gsi, op, use, 0);
	}

      FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_DEF)
	{
	  imm_use_iterator ui2;
	  gimple *use_stmt;
	  use_operand_p op2;
	  ssa_op_iter iter;
	  if (tobe_promoted_p (def))
	    if (!promote_ssa (def, &gsi))
	      {
		/* If def cannot be promoted, fixup statments that use def.
		 */
		FOR_EACH_IMM_USE_STMT (use_stmt, ui2, def)
		  {
		    int ind = 0;
		    if (gimple_code (use_stmt) == GIMPLE_PHI)
		      FOR_EACH_PHI_ARG (op2, as_a <gphi *> (use_stmt),
					iter, SSA_OP_USE)
			{
			  if (def == USE_FROM_PTR (op2))
			    fixup_use (use_stmt, NULL, op2, def, ind);
			  ind++;
			}
		  }
	      }
	}
    }
}

class type_promotion_dom_walker : public dom_walker
{
public:
  type_promotion_dom_walker (cdi_direction direction)
    : dom_walker (direction) {}
  virtual edge before_dom_children (basic_block bb)
    {
      promote_all_stmts (bb);
      return NULL;
    }
};

/* Main entry point to the pass.  */
static unsigned int
execute_type_promotion (void)
{
  n_ssa_val = num_ssa_names;
  ssa_name_info_map = new hash_map<tree, ssa_name_info *>;
  ssa_to_be_promoted_bitmap = sbitmap_alloc (n_ssa_val);
  bitmap_clear (ssa_to_be_promoted_bitmap);

  /* Create the obstack where ssa_name_info will reside.  */
  gcc_obstack_init (&ssa_name_info_obstack);

  calculate_dominance_info (CDI_DOMINATORS);
  promote_all_ssa_defined_with_nop ();
  /* Walk the CFG in dominator order.  */
  type_promotion_dom_walker (CDI_DOMINATORS)
    .walk (ENTRY_BLOCK_PTR_FOR_FN (cfun));
  gsi_commit_edge_inserts ();

  obstack_free (&ssa_name_info_obstack, NULL);
  sbitmap_free (ssa_to_be_promoted_bitmap);
  delete ssa_name_info_map;
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

