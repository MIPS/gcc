/* Pass for lowering and manipulating atomic gimple statements.
   Various __builtin_atomic function calls are turned into atomic gimple
   expressions. 
   Any memory references of type atomic are also translated into
   the approriate atomic expression.
   Contributed by Andrew MacLeod <amacleod@redhat.com>

   Copyright (C) 2012
   Free Software Foundation, Inc.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "gimple.h"
#include "tree-iterator.h"
#include "tree-inline.h"
#include "langhooks.h"
#include "diagnostic-core.h"
#include "tree-flow.h"
#include "timevar.h"
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "tree-pass.h"
#include "ggc.h"
#include "except.h"
#include "splay-tree.h"
#include "optabs.h"
#include "cfgloop.h"
#include "tree-pretty-print.h"
#include "gimple-pretty-print.h"
#include "target.h"


/* Return the type of atomic operation STMT.  */

static tree
get_atomic_type (gimple stmt)
{
  tree t;

  t = gimple_atomic_type (stmt);
  return t;
}


/* Mask off any target dependent bits from memodel A and return the value.  */

static inline  enum memmodel
pure_memmodel (enum memmodel a)
{
   enum memmodel ma = (enum memmodel) (a & MEMMODEL_MASK);
   return ma;
}

/* Extract memory model from tree EXP, and verify it is valid for KIND.  */

static enum memmodel
get_memmodel (enum gimple_atomic_kind kind, tree exp, bool report_error = true)
{
  rtx op;
  unsigned HOST_WIDE_INT val;
  enum memmodel model;
  bool model_error = false;

  /* If the parameter is not a constant, it's a run time value so we'll just
     convert it to MEMMODEL_SEQ_CST to avoid annoying runtime checking.  */
  if (TREE_CODE (exp) != INTEGER_CST)
    return MEMMODEL_SEQ_CST;

  op = expand_normal (exp);
  val = INTVAL (op);
  if (targetm.memmodel_check)
    val = targetm.memmodel_check (val);
  else if (val & ~MEMMODEL_MASK)
    {
      if (report_error)
	warning (OPT_Winvalid_memory_model,
		 "Unknown architecture specifier in memory model");
      val = val & MEMMODEL_MASK;
    }

  model = pure_memmodel ((enum memmodel)val);
  if (model >= MEMMODEL_LAST)
    {
      if (report_error)
	error ("invalid memory model argument");
      return MEMMODEL_SEQ_CST;
    }

  switch (kind)
    {
    case GIMPLE_ATOMIC_LOAD:
    case GIMPLE_ATOMIC_LOAD_GENERIC:
      if (model == MEMMODEL_RELEASE || model == MEMMODEL_ACQ_REL)
	model_error = true;
      break;

    case GIMPLE_ATOMIC_STORE:
    case GIMPLE_ATOMIC_STORE_GENERIC:
      if (model == MEMMODEL_CONSUME || model == MEMMODEL_ACQUIRE ||
	  model == MEMMODEL_ACQ_REL)
	 model_error = true;
      break;

    case GIMPLE_ATOMIC_EXCHANGE:
    case GIMPLE_ATOMIC_EXCHANGE_GENERIC:
      if (model == MEMMODEL_CONSUME)
	model_error = true;
      break;

    case GIMPLE_ATOMIC_CLEAR:
      if (model == MEMMODEL_ACQUIRE || model == MEMMODEL_ACQ_REL)
	model_error = true;
      break;

    default:
      break;
    }

  if (model_error)
    { 
      if (report_error)
	error ("invalid memory model argument to builtin.");
      return MEMMODEL_SEQ_CST;
    }

  return (enum memmodel) val;
}

/* If type is compatible with a lock free size, indicate the offset from
   ATOMIC_FUNC_N required to set the correct function.  */
static unsigned
atomic_lockfree_compatible (tree type)
{
  tree type_size;
  unsigned bits;

  if (!type)
    return 0;
  type_size = TYPE_SIZE (type);
  if (type_size)
    bits = TREE_INT_CST_LOW (TYPE_SIZE (type));
  else 
    return 0;

  switch (bits)
    {
    case 8:
      return 1;
    case 16:
      return 2;
    case 32:
      return 3;
    case 64:
      return 4;
    case 128:
      return 5;
    default:
      break;
    }
  return 0; 
}


/* Generate RTL for accessing the atomic location LOC in MODE.  */

static rtx
expand_atomic_target (tree loc, enum machine_mode mode)
{
  rtx addr, mem;

  addr = expand_expr (loc, NULL_RTX, ptr_mode, EXPAND_SUM);
  addr = convert_memory_address (Pmode, addr);

  /* Note that we explicitly do not want any alias information for this
     memory, so that we kill all other live memories.  Otherwise we don't
     satisfy the full barrier semantics of the intrinsic.  */
  mem = validize_mem (gen_rtx_MEM (mode, addr));

  /* The alignment needs to be at least according to that of the mode.  */
  set_mem_align (mem, MAX (GET_MODE_ALIGNMENT (mode),
			   get_pointer_alignment (loc)));
  set_mem_alias_set (mem, ALIAS_SET_MEMORY_BARRIER);
  MEM_VOLATILE_P (mem) = 1;

  return mem;
}


/* Make sure an argument is in the right mode.
   EXP is the tree argument. 
   MODE is the mode it should be in.  */

static rtx
expand_expr_force_mode (tree exp, enum machine_mode mode)
{
  rtx val;
  enum machine_mode old_mode;

  val = expand_expr (exp, NULL_RTX, mode, EXPAND_NORMAL);
  /* If VAL is promoted to a wider mode, convert it back to MODE.  Take care
     of CONST_INTs, where we know the old_mode only from the call argument.  */

  old_mode = GET_MODE (val);
  if (old_mode == VOIDmode)
    old_mode = TYPE_MODE (TREE_TYPE (exp));
  val = convert_modes (mode, old_mode, val, 1);
  return val;
}

/* Get the RTL for lhs #INDEX of STMT.  */

static rtx
get_atomic_lhs_rtx (tree tree_lhs)
{
  rtx rtl_lhs;
  
  if (!tree_lhs)
    return const0_rtx;

  gcc_assert (TREE_CODE (tree_lhs) == SSA_NAME);

  if (has_zero_uses (tree_lhs))
    return const0_rtx;

  rtl_lhs = expand_expr (tree_lhs, NULL_RTX, VOIDmode, EXPAND_WRITE);
  return rtl_lhs;
}


const char *
gimple_atomic_op_name (const_gimple gs)
{
  switch (gimple_atomic_op_code (gs))
    {
    case GIMPLE_ATOMIC_ARITH_OP_ADD:
      return "ADD";

    case GIMPLE_ATOMIC_ARITH_OP_SUB:
      return "SUB";

    case GIMPLE_ATOMIC_ARITH_OP_AND:
      return "AND";

    case GIMPLE_ATOMIC_ARITH_OP_OR:
      return "OR";

    case GIMPLE_ATOMIC_ARITH_OP_XOR:
      return "XOR";

    case GIMPLE_ATOMIC_ARITH_OP_NAND:
      return "NAND";

    default:
      break;
    }
  gcc_unreachable ();
}


const char *
gimple_atomic_type_size_string (const_gimple gs)
{
  tree t = gimple_atomic_type (gs);
  unsigned n = TREE_INT_CST_LOW (TYPE_SIZE (t));
  switch (n)
    {
    case 8:
      return "_1";
    case 16:
      return "_2";
    case 32:
      return "_4";
    case 64:
      return "_8";
    case 128:
      return "_16";
    default:
      break;
    }
  return "";
}


static rtx
get_libcall_rtx (enum built_in_function fcode, tree type)
{
  rtx libfunc;
  unsigned off = 0;

  if (type)
    off = atomic_lockfree_compatible (type);

  fcode = (enum built_in_function)((unsigned)fcode + off);
  libfunc = XEXP (DECL_RTL (builtin_decl_explicit (fcode)), 0);

  return libfunc;
}

static rtx
get_libcall_size (tree type)
{
  int size = tree_low_cst (TYPE_SIZE_UNIT (type), 1);
  return GEN_INT (size);
}

/* Expand atomic load STMT into RTL.  Return true if successful.  */
void
expand_gimple_atomic_load (gimple stmt)
{
  enum machine_mode mode;
  enum memmodel model;
  tree type;
  rtx mem, rtl_lhs;
  rtx rtl_rhs = NULL_RTX;

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_LOAD);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));
  type = get_atomic_type (stmt);

  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  rtl_lhs = get_atomic_lhs_rtx (gimple_atomic_lhs (stmt));

  /* Attempt to expand into a lock free sequence.  */
  if (flag_inline_atomics)
    {
      gcc_assert (mode != BLKmode);
      mem = expand_atomic_target (gimple_atomic_target (stmt), mode);
      rtl_rhs = expand_atomic_load (rtl_lhs, mem, model);
    }

  /* If no rtl is generated, issue a library call.  */
  if (!rtl_rhs)
    {
      rtx libfunc = get_libcall_rtx (BUILT_IN_ATOMIC_LOAD_N, type);
      mem = expand_expr (gimple_atomic_target (stmt), NULL_RTX, ptr_mode,
			 EXPAND_SUM);
      emit_library_call_value (libfunc, rtl_lhs, LCT_NORMAL, mode, 2, mem,
			       Pmode, GEN_INT (model), SImode);
    }
  else
    if (rtl_lhs != const0_rtx)
      expand_gimple_assign_move (TREE_TYPE (type), rtl_lhs, rtl_rhs, false);

}

/* Expand atomic generic load STMT into RTL.  Return true if successful.  */
void
expand_gimple_atomic_load_generic (gimple stmt)
{
  enum memmodel model;
  tree type = get_atomic_type (stmt);
  rtx mem, rtl_lhs;
  rtx libfunc = get_libcall_rtx (BUILT_IN_ATOMIC_LOAD, NULL_TREE);
  rtx size = get_libcall_size (type);

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_LOAD_GENERIC);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));
  mem = expand_expr (gimple_atomic_target (stmt), NULL_RTX, ptr_mode,
		     EXPAND_SUM);
  rtl_lhs = expand_expr (gimple_atomic_return (stmt), NULL_RTX, ptr_mode,
			 EXPAND_SUM);
  emit_library_call (libfunc, LCT_NORMAL, VOIDmode, 4, size, SImode, mem,
		     Pmode, rtl_lhs, Pmode, GEN_INT (model), SImode);
  return;
}


/* Expand atomic store STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_store (gimple stmt)
{
  rtx mem, val, rtl_rhs = NULL_RTX;
  enum memmodel model;
  enum machine_mode mode;
  tree type;

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_STORE);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));
  type = get_atomic_type (stmt);

  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);

  val = expand_expr_force_mode (gimple_atomic_expr (stmt), mode);
  if (flag_inline_atomics)
    {
      gcc_assert (mode != BLKmode);
      /* Expand the operands.  */
      mem = expand_atomic_target (gimple_atomic_target (stmt), mode);
      rtl_rhs = expand_atomic_store (mem, val, model, false);
    }

  if (!rtl_rhs)
    {
      rtx libfunc = get_libcall_rtx (BUILT_IN_ATOMIC_STORE_N, type);
      mem = expand_expr (gimple_atomic_target (stmt), NULL_RTX, ptr_mode,
			 EXPAND_SUM);
      emit_library_call (libfunc, LCT_NORMAL, VOIDmode, 3, mem, Pmode,
			 val, mode, GEN_INT (model), SImode);
    }
}

/* Expand atomic generic store STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_store_generic (gimple stmt)
{
  rtx mem, rtl_rhs;
  enum memmodel model;
  tree type = get_atomic_type (stmt);
  rtx size = get_libcall_size (type);
  rtx libfunc = get_libcall_rtx (BUILT_IN_ATOMIC_STORE, NULL_TREE);

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_STORE_GENERIC);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));
  mem = expand_expr (gimple_atomic_target (stmt), NULL_RTX, ptr_mode,
		     EXPAND_SUM);
  rtl_rhs = expand_expr (gimple_atomic_expr (stmt), NULL_RTX, ptr_mode,
			 EXPAND_SUM);
  emit_library_call (libfunc, LCT_NORMAL, VOIDmode, 4, size, SImode, mem,
		     Pmode, rtl_rhs, Pmode, GEN_INT (model), SImode);
  return;
}


/* Expand atomic exchange STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_exchange (gimple stmt)
{
  rtx mem, val, rtl_rhs = NULL_RTX, rtl_lhs;
  enum memmodel model;
  enum machine_mode mode;
  tree type;

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_EXCHANGE);
  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));
  type = get_atomic_type (stmt);

  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  /* Expand the operands.  */
  val = expand_expr_force_mode (gimple_atomic_expr (stmt), mode);
  rtl_lhs = get_atomic_lhs_rtx (gimple_atomic_lhs (stmt));

  if (flag_inline_atomics)
    {
      mem = expand_atomic_target (gimple_atomic_target (stmt), mode);

      /* __sync_test_and_set is mapped to an atomic exchange operation, but 
	 RTL expansion may be different if there is no exchange pattern.  */
      if (gimple_atomic_from_sync (stmt))
	{
	  /* If the __sync_lock_test_and_set fails to generate code, then the
	     normal libatomic exchange call will suffice.  */
	  gcc_assert (model == MEMMODEL_ACQUIRE);
	  rtl_rhs = expand_sync_lock_test_and_set (rtl_lhs, mem, val);
	}
      else
	rtl_rhs = expand_atomic_exchange (rtl_lhs, mem, val, model);
     }

  /* If no rtl is generated, call the libatomic function.  */
  if (!rtl_rhs)
    {
      rtx libfunc = get_libcall_rtx (BUILT_IN_ATOMIC_EXCHANGE_N, type);
      mem = expand_expr (gimple_atomic_target (stmt), NULL_RTX, ptr_mode,
			 EXPAND_SUM);
      emit_library_call_value (libfunc, rtl_lhs, LCT_NORMAL, mode, 3, mem,
			       Pmode, val, mode, GEN_INT (model), SImode);
    }
  else
    if (rtl_lhs != const0_rtx)
      expand_gimple_assign_move (TREE_TYPE (type), rtl_lhs, rtl_rhs, false);
}

/* Expand atomic generic exchange STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_exchange_generic (gimple stmt)
{
  rtx mem, rtl_rhs = NULL_RTX, rtl_lhs;
  enum memmodel model;
  tree type = get_atomic_type (stmt);
  rtx libfunc = get_libcall_rtx (BUILT_IN_ATOMIC_EXCHANGE, NULL_TREE);
  rtx size = get_libcall_size (type);

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_EXCHANGE_GENERIC);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));
  mem = expand_expr (gimple_atomic_target (stmt), NULL_RTX, ptr_mode,
		     EXPAND_SUM);
  rtl_lhs = expand_expr (gimple_atomic_return (stmt), NULL_RTX, ptr_mode,
			 EXPAND_SUM);
  rtl_rhs = expand_expr (gimple_atomic_expr (stmt), NULL_RTX, ptr_mode,
			 EXPAND_SUM);
  emit_library_call (libfunc, LCT_NORMAL, VOIDmode, 5, size, SImode, mem, Pmode,
		     rtl_rhs, Pmode, rtl_lhs, Pmode, GEN_INT (model), SImode);
}

static enum memmodel
process_compare_exchange_models (gimple stmt, enum memmodel *fail)
{
  enum memmodel s, f;
  enum memmodel success = get_memmodel (gimple_atomic_kind (stmt),
					gimple_atomic_order (stmt));
  *fail = get_memmodel (gimple_atomic_kind (stmt),
			gimple_atomic_fail_order (stmt));
  s = pure_memmodel (success);
  f = pure_memmodel (*fail);

  /* compare_exchange has additional restrictions on the failure order.  */
  if (f == MEMMODEL_RELEASE || f == MEMMODEL_ACQ_REL)
    error ("invalid failure memory model for %<__atomic_compare_exchange%>");

  if (f > s)
    error ("failure memory model cannot be stronger than success "
	   "memory model for %<__atomic_compare_exchange%>");
  return success;
}

/* Expand atomic compare_exchange STMT into RTL.  Return true if successful.  */
static void
expand_into_compare_exchange_libcall (gimple stmt, enum memmodel success,
				      enum memmodel failure)
{
  rtx mem, val, rtl_lhs1, expect;
  enum machine_mode mode;
  tree type;
  rtx rtl_lhs2 = const0_rtx;

  type = get_atomic_type (stmt);
  rtl_lhs1 = get_atomic_lhs_rtx (gimple_atomic_lhs (stmt));

  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  val = expand_expr_force_mode (gimple_atomic_expr (stmt), mode);

  rtx libfunc = get_libcall_rtx (BUILT_IN_ATOMIC_COMPARE_EXCHANGE_N, type);
  enum machine_mode bool_mode = TYPE_MODE (boolean_type_node);
  tree e, expect_mem;
  tree tmp_var = NULL_TREE;

  if (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_COMPARE_EXCHANGE_LIBRARY)
    expect_mem = gimple_atomic_expected (stmt);
  else
    {
      /* This is a 2 result form being converted to a library call. 
	 load expected into a temp variable and pass the address of that.  */
      tree ptr_type;
      rtl_lhs2 = get_atomic_lhs_rtx (gimple_atomic_2nd_lhs (stmt));
      e = gimple_atomic_expected (stmt);
      tmp_var = add_new_static_var (TREE_TYPE (e));
      TREE_ADDRESSABLE (tmp_var) = true;
      expand_assignment (tmp_var, e, false);
      ptr_type = build_pointer_type (TREE_TYPE (e));
      expect_mem = build1 (ADDR_EXPR, ptr_type, tmp_var);
    }
  expect = expand_expr (expect_mem, NULL_RTX, ptr_mode, EXPAND_SUM);
  mem = expand_expr (gimple_atomic_target (stmt), NULL_RTX, ptr_mode,
		     EXPAND_SUM);
  emit_library_call_value (libfunc, rtl_lhs1, LCT_NORMAL, bool_mode, 5,
			   mem, Pmode, expect, Pmode, val,
			   mode, GEN_INT (success),
			   SImode, GEN_INT (failure), SImode);
  /* If the expected return value is used, and we had to use a temporary
     variable for expected, copy the value back.  */
  if (rtl_lhs2 != const0_rtx && tmp_var)
    expand_assignment (gimple_atomic_2nd_lhs (stmt), tmp_var, false);
}

/* Expand atomic compare_exchange STMT into RTL.  Return true if successful.  */
void
expand_gimple_atomic_compare_exchange_library (gimple stmt)
{
  enum memmodel success, failure;

  gcc_assert (gimple_atomic_kind (stmt)
	      == GIMPLE_ATOMIC_COMPARE_EXCHANGE_LIBRARY);
  success = process_compare_exchange_models (stmt, &failure);
  expand_into_compare_exchange_libcall (stmt, success, failure);
}

/* Expand atomic compare_exchange STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_compare_exchange (gimple stmt)
{
  rtx mem, val, rtl_lhs1, expect;
  rtx real_lhs1, real_lhs2;
  enum memmodel success, failure;
  enum machine_mode mode;
  tree type;
  bool is_weak, emitted = false;
  rtx rtl_lhs2 = const0_rtx;

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_COMPARE_EXCHANGE);

  type = get_atomic_type (stmt);
  rtl_lhs1 = get_atomic_lhs_rtx (gimple_atomic_lhs (stmt));

  success = process_compare_exchange_models (stmt, &failure);

  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  expect = expand_expr_force_mode (gimple_atomic_expected (stmt), mode);
  val = expand_expr_force_mode (gimple_atomic_expr (stmt), mode);
  if (flag_inline_atomics)
    {
      /* Expand the operands.  */
      is_weak = gimple_atomic_weak (stmt);
      rtl_lhs2 = get_atomic_lhs_rtx (gimple_atomic_2nd_lhs (stmt));
      real_lhs1 = rtl_lhs1;
      real_lhs2 = rtl_lhs2;
      mem = expand_atomic_target (gimple_atomic_target (stmt), mode);
      emitted = expand_atomic_compare_and_swap (&real_lhs1, &real_lhs2, mem,
						expect, val, is_weak, success,
						failure);
    }
  /* If no rtl is generated, then call libatomic.  */
  if (!emitted)
    expand_into_compare_exchange_libcall (stmt, success, failure);
  else
    {
      if (rtl_lhs1 != const0_rtx)
	expand_gimple_assign_move (type, rtl_lhs1, real_lhs1, false);
      if (rtl_lhs2 != const0_rtx)
	expand_gimple_assign_move (type, rtl_lhs2, real_lhs2, false);
    }
}

/* Expand atomic compare_exchange STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_compare_exchange_generic (gimple stmt)
{
  rtx mem, rtl_lhs, expect, expr;
  enum memmodel success, failure;
  tree type = get_atomic_type (stmt);
  rtx libfunc = get_libcall_rtx (BUILT_IN_ATOMIC_COMPARE_EXCHANGE, NULL_TREE);
  rtx size = get_libcall_size (type);
  enum machine_mode bool_mode = TYPE_MODE (boolean_type_node);

  gcc_assert (gimple_atomic_kind (stmt)
	      == GIMPLE_ATOMIC_COMPARE_EXCHANGE_GENERIC);
  rtl_lhs = get_atomic_lhs_rtx (gimple_atomic_lhs (stmt));

  success = process_compare_exchange_models (stmt, &failure);
  mem = expand_expr (gimple_atomic_target (stmt), NULL_RTX, ptr_mode,
		     EXPAND_SUM);
  expr = expand_expr (gimple_atomic_expr (stmt), NULL_RTX, ptr_mode,
		      EXPAND_SUM);
  expect = expand_expr (gimple_atomic_expected (stmt), NULL_RTX, ptr_mode,
			EXPAND_SUM);
  emit_library_call_value (libfunc, rtl_lhs, LCT_NORMAL, bool_mode, 6, size,
			   SImode, mem, Pmode, expect, Pmode, expr, Pmode,
			   GEN_INT (success), SImode, GEN_INT (failure),
			   SImode);
}



/* Return the RTL code for the tree operation TCODE.  */

static enum rtx_code
rtx_code_from_atomic_op (enum gimple_atomic_arith_op op)
{
  switch (op)
    {
    case GIMPLE_ATOMIC_ARITH_OP_ADD:
      return PLUS;
    case GIMPLE_ATOMIC_ARITH_OP_SUB:
      return MINUS;
    case GIMPLE_ATOMIC_ARITH_OP_AND:
      return AND;
    case GIMPLE_ATOMIC_ARITH_OP_OR:
      return IOR;
    case GIMPLE_ATOMIC_ARITH_OP_XOR:
      return XOR;
    case GIMPLE_ATOMIC_ARITH_OP_NAND:
      return NOT;
    default :
      error ("invalid operation type in atomic fetch operation");
    }
  return PLUS;
}

static enum built_in_function
rtx_function_code_from_atomic_op (enum gimple_atomic_arith_op op,
				  bool fetch_after)
{
  switch (op)
    {
    case GIMPLE_ATOMIC_ARITH_OP_ADD:
      return (fetch_after ? BUILT_IN_ATOMIC_ADD_FETCH_N
			  : BUILT_IN_ATOMIC_FETCH_ADD_N);

    case GIMPLE_ATOMIC_ARITH_OP_SUB:
      return (fetch_after ? BUILT_IN_ATOMIC_SUB_FETCH_N
			  : BUILT_IN_ATOMIC_FETCH_SUB_N);

    case GIMPLE_ATOMIC_ARITH_OP_AND:
      return (fetch_after ? BUILT_IN_ATOMIC_AND_FETCH_N
			  : BUILT_IN_ATOMIC_FETCH_AND_N);

    case GIMPLE_ATOMIC_ARITH_OP_OR:
      return (fetch_after ? BUILT_IN_ATOMIC_OR_FETCH_N
			  : BUILT_IN_ATOMIC_FETCH_OR_N);

    case GIMPLE_ATOMIC_ARITH_OP_XOR:
      return (fetch_after ? BUILT_IN_ATOMIC_XOR_FETCH_N
			  : BUILT_IN_ATOMIC_FETCH_XOR_N);

    case GIMPLE_ATOMIC_ARITH_OP_NAND:
      return (fetch_after ? BUILT_IN_ATOMIC_NAND_FETCH_N
			  : BUILT_IN_ATOMIC_FETCH_NAND_N);

    default :
      error ("invalid operation type in atomic fetch operation");
    }
  return BUILT_IN_ATOMIC_FETCH_ADD_N;
}



/* Expand atomic fetch operation STMT into RTL.  FETCH_AFTER is true if the 
   value returned is the post operation value.  Return true if successful.  */

static void
expand_atomic_fetch (gimple stmt, bool fetch_after)
{
  rtx mem, val, rtl_rhs = NULL_RTX, rtl_lhs;
  enum memmodel model;
  enum machine_mode mode;
  tree type;
  static bool warned_f_a_n = false;
  static bool warned_n_a_f = false;
  location_t loc = gimple_location (stmt);

  type = get_atomic_type (stmt);
  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));

  /* Expand the operands.  */
  val = expand_expr_force_mode (gimple_atomic_expr (stmt), mode);
  rtl_lhs = get_atomic_lhs_rtx (gimple_atomic_lhs (stmt));

  if (warn_sync_nand && gimple_atomic_from_sync (stmt)
      && gimple_atomic_op_code (stmt) == GIMPLE_ATOMIC_ARITH_OP_NAND)
    {
      if (fetch_after && !warned_n_a_f)
        {
	  tree fndecl = builtin_decl_implicit (BUILT_IN_SYNC_NAND_AND_FETCH_N);
	  inform (loc, "%qD changed semantics in GCC 4.4", fndecl);
	  warned_n_a_f = true;
	}
      if (!fetch_after && !warned_f_a_n)
        {
	  tree fndecl = builtin_decl_implicit (BUILT_IN_SYNC_FETCH_AND_NAND_N);
	  inform (loc, "%qD changed semantics in GCC 4.4", fndecl);
	  warned_f_a_n = true;
	}
    }

  if (flag_inline_atomics)
    {
      enum rtx_code rcode;

      rcode = rtx_code_from_atomic_op (gimple_atomic_op_code (stmt));
      mem = expand_atomic_target (gimple_atomic_target (stmt), mode);
      rtl_rhs = expand_atomic_fetch_op (rtl_lhs, mem, val, rcode, model,
				    fetch_after);
    }

  /* If no rtl is generated, call libatomic.  */
  if (!rtl_rhs)
    {
      enum built_in_function fcode;
      rtx libfunc, res;
      enum rtx_code code;

      if (fetch_after)
        res = NULL_RTX;
      else
        res = rtl_lhs;
      /* The library only provides fetch-before variants, so any fetch-after
	 will have to be converted.  */
      fcode = rtx_function_code_from_atomic_op (gimple_atomic_op_code (stmt),
						false);
      libfunc = get_libcall_rtx (fcode, type);
      mem = expand_expr (gimple_atomic_target (stmt), NULL_RTX, ptr_mode,
			 EXPAND_SUM);
      res = emit_library_call_value (libfunc, res, LCT_NORMAL, mode, 3,
					mem, Pmode, val, mode, GEN_INT (model),
					SImode);
      if (fetch_after && rtl_lhs != const0_rtx)
        {
	  switch (gimple_atomic_op_code (stmt))
	    {
	    case  GIMPLE_ATOMIC_ARITH_OP_ADD:
	      code = PLUS;
	      break;
	    case GIMPLE_ATOMIC_ARITH_OP_SUB:
	      code = MINUS;
	      break;
	    case GIMPLE_ATOMIC_ARITH_OP_AND:
	      code = AND;
	      break;
	    case GIMPLE_ATOMIC_ARITH_OP_XOR:
	      code = XOR;
	      break;
	    case GIMPLE_ATOMIC_ARITH_OP_OR:
	      code = IOR;
	      break;
	    case GIMPLE_ATOMIC_ARITH_OP_NAND:
	      code = UNKNOWN;
	      res = expand_simple_binop (mode, AND, res, val, NULL_RTX,
					 true, OPTAB_LIB_WIDEN);
	      res = expand_simple_unop (mode, NOT, res, rtl_lhs, true);
	      break;
	    default:
	      gcc_unreachable();

	    }
	  if (code != UNKNOWN)
	    res = expand_simple_binop (mode, code, res, val, rtl_lhs, true,
				       OPTAB_LIB_WIDEN);

	}
    }
  else
    if (rtl_lhs != const0_rtx)
      expand_gimple_assign_move (TREE_TYPE (type), rtl_lhs, rtl_rhs, false);
}


/* Expand atomic fetch_op operation STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_fetch_op (gimple stmt)
{
  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_FETCH_OP);
  expand_atomic_fetch (stmt, false);
}

/* Expand atomic fetch_op operation STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_op_fetch (gimple stmt)
{
  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_OP_FETCH);
  expand_atomic_fetch (stmt, true);
}

/* Expand atomic test_and_set STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_test_and_set (gimple stmt)
{
  rtx mem, rtl_rhs, rtl_lhs;
  enum memmodel model;
  enum machine_mode mode;
  tree type;

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_TEST_AND_SET);

  type = get_atomic_type (stmt);
  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  mode = mode_for_size (BOOL_TYPE_SIZE, MODE_INT, 0);
  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));

  /* Expand the operands.  */
  mem = expand_atomic_target (gimple_atomic_target (stmt), mode);

  rtl_lhs = get_atomic_lhs_rtx (gimple_atomic_lhs (stmt));
  rtl_rhs = expand_atomic_test_and_set (rtl_lhs, mem, model);

  /* Test and set is not allowed to fail.  */
  gcc_assert (rtl_rhs);

  if (rtl_lhs != const0_rtx)
    expand_gimple_assign_move (TREE_TYPE (type), rtl_lhs, rtl_rhs, false);
}

#ifndef HAVE_atomic_clear
# define HAVE_atomic_clear 0
# define gen_atomic_clear(x,y) (gcc_unreachable (), NULL_RTX)
#endif

/* Expand atomic clear STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_clear (gimple stmt)
{
  rtx mem, ret;
  enum memmodel model;
  enum machine_mode mode;
  tree type;

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_CLEAR);

  type = get_atomic_type (stmt);
  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  mode = mode_for_size (BOOL_TYPE_SIZE, MODE_INT, 0);
  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));
  mem = expand_atomic_target (gimple_atomic_target (stmt), mode);

  if (HAVE_atomic_clear)
    {
      emit_insn (gen_atomic_clear (mem, model));
      return;
    }

  /* Try issuing an __atomic_store, and allow fallback to __sync_lock_release.
     Failing that, a store is issued by __atomic_store.  The only way this can
     fail is if the bool type is larger than a word size.  Unlikely, but
     handle it anyway for completeness.  Assume a single threaded model since
     there is no atomic support in this case, and no barriers are required.  */
  ret = expand_atomic_store (mem, const0_rtx, model, true);
  if (!ret)
    emit_move_insn (mem, const0_rtx);
}

/* Expand atomic fence STMT into RTL.  Return true if successful.  */

void
expand_gimple_atomic_fence (gimple stmt)
{
  enum memmodel model;
  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_FENCE);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));

  if (gimple_atomic_thread_fence (stmt))
    expand_mem_thread_fence (model);
  else
    expand_mem_signal_fence (model);
}


/* Return base type for an atomic builtin function.  */

static tree
atomic_func_type (unsigned i)
{
  gcc_checking_assert (i <= 5);

  switch (i)
    {
    case 0:
      gcc_unreachable ();
    case 1:
      return unsigned_intQI_type_node;
    case 2:
      return unsigned_intHI_type_node;
    case 3:
      return unsigned_intSI_type_node;
    case 4:
      return unsigned_intDI_type_node;
    case 5:
      return unsigned_intTI_type_node;
    default:
      gcc_unreachable ();
    }
}
static bool
arg_count_ok (unsigned nargs, unsigned expected)
{
  if (nargs == expected)
    return true;

  if (nargs < expected)
    error ("too few arguments to built-in function");
  else
    error ("too many arguments to built-in function");

  return false;
}

static void
incompatible_type (unsigned n)
{
  error ("argument %d must be a pointer type", n);
}

typedef struct atomic_types_d {
  tree BI_return_type;		/* Return type of built in function.  */
  tree BI_atomic_type;		/* Atomic type expected by builtin.  */
  tree target_type;		/* Actual type of atomic target.  */
} atomic_types, *atomic_types_p;


static void
types_match (tree BI_type, tree target_type)
{
  if (BI_type == error_mark_node || BI_type == NULL_TREE
      || target_type == error_mark_node || target_type == NULL_TREE)
    return;

  if (TYPE_SIZE (BI_type) == TYPE_SIZE (target_type))
    return;

  if (!TYPE_SIZE (target_type))
    return;

  warning (0, "atomic object size does not match explicit built-in size");
}

static tree
atomic_type (tree atomic_expr)
{
  tree target_type;

  switch (DECL_FUNCTION_CODE (ATOMIC_EXPR_DECL (atomic_expr)))
    {
    case BUILT_IN_SYNC_SYNCHRONIZE:
    case BUILT_IN_ATOMIC_THREAD_FENCE:
    case BUILT_IN_ATOMIC_SIGNAL_FENCE:
      return NULL_TREE;

    default:
      target_type = TREE_TYPE (ATOMIC_EXPR_ARG (atomic_expr, 0));
      if (TREE_CODE (target_type) == POINTER_TYPE)
	return TREE_TYPE (target_type);
      break;
    }
  return error_mark_node;
}

static tree
handle_any_casts (tree dest_type, tree value, gimple_seq *pre_p)
{
  tree src_type = TREE_TYPE (value);

  if (!useless_type_conversion_p (dest_type, src_type))
    {
      gimple_seq casts;
      value = fold_convert (dest_type, value);
      value = force_gimple_operand (value, &casts, true, NULL_TREE);
      if (casts != NULL)
        gimple_seq_add_seq (pre_p, casts);
    }

  return value;
}


static tree
handle_atomic_target (tree atomic_expr, gimple_seq *pre_p, tree target_type, tree BI_type)
{
  tree *target_p;
  enum gimplify_status status;
  tree BI_ptr_type;

  if (target_type != error_mark_node)
    {
      if (target_type == NULL_TREE)
        return NULL_TREE;

      if (BI_type != NULL_TREE)
	BI_ptr_type = build_pointer_type (BI_type);
      else
        BI_ptr_type = target_type;

      target_p = &ATOMIC_EXPR_ARG (atomic_expr, 0);

      gcc_assert (TREE_CODE (TREE_TYPE (*target_p)) == POINTER_TYPE);

      if (!useless_type_conversion_p (BI_ptr_type, target_type))
	*target_p = fold_convert (BI_ptr_type, *target_p);

      status = gimplify_expr (target_p, pre_p, NULL,
			      is_gimple_val, fb_either);

      if (status != GS_ERROR)
	return *target_p;
    }
  incompatible_type (0);
  return error_mark_node;
}


static tree
atomic_BI_type (tree targ_type)
{
  tree type = NULL_TREE;
  unsigned bt;
  
  if (targ_type != NULL_TREE && targ_type != error_mark_node)
    {
      bt = atomic_lockfree_compatible (targ_type);
      if (bt)
	type = atomic_func_type (bt);
    }
  else
    return NULL_TREE;

  if (TYPE_SIZE (targ_type) != 0)
    return type;
  else
    return NULL_TREE;
}


static tree
get_atomic_expression (tree BI_type, tree *t, gimple_seq *pre_p)
{
  enum gimplify_status status;
  
  status = gimplify_expr (t, pre_p, NULL, is_gimple_val, fb_rvalue);

  if (status == GS_ERROR)
    return NULL_TREE;

  *t = handle_any_casts (BI_type, *t, pre_p);
  return *t;
}

static tree
get_atomic_addr (tree BI_type, tree *t, gimple_seq *pre_p, unsigned parm_num)
{
  enum gimplify_status status;
  tree ptr_type;
  if (BI_type)
    ptr_type = build_pointer_type (BI_type);
  else
    ptr_type = build_pointer_type (void_type_node);
  
  if (TREE_CODE (TREE_TYPE (*t)) == POINTER_TYPE)
    {
      if (!useless_type_conversion_p (ptr_type, TREE_TYPE (*t)))
	*t = fold_convert (ptr_type, TREE_TYPE (*t));
      status = gimplify_expr (t, pre_p, NULL, is_gimple_val, fb_either);

      if (status != GS_ERROR)
	return *t;
    }

  incompatible_type (parm_num);
  return NULL_TREE;
}


static tree
get_atomic_order (tree *t, gimple_seq *pre_p)
{
  enum gimplify_status status;
  
  if (!INTEGRAL_TYPE_P (TREE_TYPE (*t)))
    {
      error ("non-integer memory model argument");
      status = GS_ERROR;
    }
  else
    if (TREE_CODE (*t) == INTEGER_CST
	&& (tree_low_cst (*t, 1) & MEMMODEL_MASK) >= MEMMODEL_LAST)
      {
	warning (OPT_Winvalid_memory_model,
		 "invalid memory model argument");
	status = GS_ERROR;
      }
    else
      status = gimplify_expr (t, pre_p, NULL, is_gimple_val, fb_rvalue);

  if (status == GS_ERROR)
    return build_int_cst (integer_type_node, MEMMODEL_SEQ_CST);
  return *t;
}


/* Finish processing atomic statement STMT by linking it in after PRE_P
   and setting EXPR_P to the return value. if there is one.
   If present, BI_TYPE indicates the built-in return type of the atomic
   operation
   If present, RET_TYPE indicates the type of the user declared type the
   return value is copied into.  */
static inline enum gimplify_status
finish_atomic_stmt (gimple stmt, gimple_seq *pre_p, tree *expr_p,
		    tree BI_type = NULL_TREE, tree ret_type = NULL_TREE)
{
  tree tmp = NULL_TREE;

  /* Set return variable if required and issue atomic stmt.  */
  if (BI_type)
    {
      tmp = create_tmp_var (BI_type, NULL);
      gimple_atomic_set_lhs (stmt, tmp);
    }
  gimple_seq_add_stmt_without_update (pre_p, stmt);

  /* Handle any conversion to the user's type from the atomic built in type.  */
  if (tmp)
    tmp = handle_any_casts (ret_type, tmp, pre_p);

  *expr_p = tmp;
  return GS_ALL_DONE;
}


/* load memory pointed to by ARG into a temporary sequenced after PRE_P.
   Return the tmp variable.  */
static inline tree
load_from_atomic_param_pointer (tree arg, gimple_seq *pre_p)
{
  gimple stmt;
  tree deref_tmp, tmp;
  tree ptr_type;
  tree type;

  /* Error condition that has been reported.  */
  if (arg == NULL_TREE)
    return NULL_TREE;

  ptr_type = TREE_TYPE (arg);
  gcc_assert (TREE_CODE (ptr_type) == POINTER_TYPE);
  type = TREE_TYPE (ptr_type);

  tmp = create_tmp_var (type, NULL);
  deref_tmp = build2 (MEM_REF, type, arg,
		    build_int_cst_wide (ptr_type, 0, 0));
  stmt = gimple_build_assign (tmp, deref_tmp);
  gimple_seq_add_stmt_without_update (pre_p, stmt);
  return tmp;
}

/* Store VAL into the memory pointer to by ARG, and sequence it after PRE_P.  */
static void
store_into_atomic_param_pointer (tree arg, tree val, gimple_seq *pre_p)
{
  gimple stmt;
  tree deref_tmp;
  tree arg_ptr_type;
  tree arg_type;

  /* Error condition that has been reported.  */
  if (arg == NULL_TREE)
    return;

  arg_ptr_type = TREE_TYPE (arg);
  gcc_assert (TREE_CODE (arg_ptr_type) == POINTER_TYPE);
  arg_type = TREE_TYPE (arg_ptr_type);
  if (!INTEGRAL_TYPE_P (arg_type) && !POINTER_TYPE_P (arg_type))
    {
      arg_ptr_type = build_pointer_type (TREE_TYPE (val));
      arg = handle_any_casts (arg_ptr_type, arg, pre_p);
      arg_type = TREE_TYPE (val);
    }
  else
    val = handle_any_casts (arg_type, val, pre_p);
  deref_tmp = build2 (MEM_REF, arg_type, arg,
		    build_int_cst_wide (arg_ptr_type, 0, 0));

  stmt = gimple_build_assign (deref_tmp, val);
  gimple_seq_add_stmt_without_update (pre_p, stmt);
}


static tree
issue_cmpxchg (tree BI_type, tree target, tree expected, tree val, bool is_weak, tree order, tree fail_order, gimple_seq *pre_p) 
{
  tree tmp1, tmp2, expected_ptr_type, expected_type;
  tree bool_ret;
  gimple ret;
  tree BI_ptr_type, deref_tmp;
  bool expand_2_results = (can_compare_and_swap_p (TYPE_MODE (BI_type), true)
			   && flag_inline_atomics);

  gcc_assert (TREE_CODE (TREE_TYPE (target)) == POINTER_TYPE);
  BI_ptr_type = build_pointer_type (BI_type);

  expected_ptr_type = TREE_TYPE (expected);

  if (!useless_type_conversion_p (BI_ptr_type, expected_ptr_type))
    expected = fold_convert (BI_ptr_type, expected);

  /* If this can nopt be expanded into a 2 result pattern on the target
     don't bother expoanding it here.  */
  if (!expand_2_results)
    {
      /* bool, tmp2 = cmp_exchange (t, deref_tmp, ...) */
      ret  = gimple_build_atomic_compare_exchange_library (BI_type,
			     target, expected, val, order, fail_order, is_weak);
      bool_ret = create_tmp_var (boolean_type_node, NULL);
      gimple_atomic_set_lhs (ret, bool_ret);
      gimple_seq_add_stmt_without_update (pre_p, ret);
      return bool_ret;
    }

  expected_type = TREE_TYPE (expected_ptr_type);
  /* Handle void * or struct * types.  */
  if (!TYPE_SIZE (expected_type)
      || (!INTEGRAL_TYPE_P (expected_type) && !POINTER_TYPE_P (expected_type)))
    expected_type = BI_type;

  tmp1 = create_tmp_var (BI_ptr_type, "cmpxchg_p");
  ret = gimple_build_assign (tmp1, expected);
  gimple_seq_add_stmt_without_update (pre_p, ret);

  /* deref_tmp = *tmp1 */
  deref_tmp = load_from_atomic_param_pointer (tmp1, pre_p);

  /* bool, tmp2 = cmp_exchange (t, deref_tmp, ...) */
  ret  = gimple_build_atomic_compare_exchange (BI_type, target, deref_tmp,
					       val, order, fail_order,
					       is_weak);
  bool_ret = create_tmp_var (boolean_type_node, NULL);
  gimple_atomic_set_lhs (ret, bool_ret);
  tmp2 = create_tmp_var (expected_type, NULL);
  gimple_atomic_set_2nd_lhs (ret , tmp2);
  gimple_seq_add_stmt_without_update (pre_p, ret);

  /* *tmp1 = tmp2;  */
  store_into_atomic_param_pointer (tmp1, tmp2, pre_p);

  return bool_ret;
}


static tree
issue_sync_cmpxchg (tree BI_type, tree target, tree expected, tree val, gimple_seq *pre_p, bool bool_result)
{
  tree tmp;
  tree bool_ret;
  gimple ret;
  tree order = build_int_cst (integer_type_node, MEMMODEL_SEQ_CST);

  gcc_assert (TREE_CODE (TREE_TYPE (target)) == POINTER_TYPE);

  /* bool, tmp2 = cmp_exchange (t, deref_tmp, ...) */
  ret  = gimple_build_atomic_compare_exchange (BI_type, target, expected, val,
					       order, order, false);
  bool_ret = create_tmp_var (boolean_type_node, NULL);
  gimple_atomic_set_lhs (ret, bool_ret);
  tmp = create_tmp_var (BI_type, NULL);
  gimple_atomic_set_2nd_lhs (ret , tmp);
  gimple_seq_add_stmt_without_update (pre_p, ret);

  if (bool_result)
    return bool_ret;
  else
    return tmp;
}


static inline bool
verify_arg (tree t1, tree expr, unsigned arg)
{
  tree t2 = TREE_TYPE (ATOMIC_EXPR_ARG (expr, arg));
  int s1 = tree_low_cst (TYPE_SIZE_UNIT (t1), 1);
  int s2 = tree_low_cst (TYPE_SIZE_UNIT (t2), 1);

  if (s1 != s2)
    {
       error ("size mismatch in argument %d", arg + 1);
       return false;
    }
  return true;
}

static inline bool
verify_ptr_arg (tree t1, tree expr, unsigned arg)
{
  tree t2 = TREE_TYPE (ATOMIC_EXPR_ARG (expr, arg));
  int s1, s2;

  if (!POINTER_TYPE_P (t2))
    {
      error ("argument %d must be a pointer type", arg + 1);
      return false;
    }

  t2 = TREE_TYPE (t2);
  s1 = tree_low_cst (TYPE_SIZE_UNIT (t1), 1);
  s2 = tree_low_cst (TYPE_SIZE_UNIT (t2), 1);

  if (s1 != s2)
    {
       error ("size mismatch in argument %d", arg + 1);
       return false;
    }
  return true;
} 


enum gimplify_status
gimplify_atomic_expr (tree *expr_p, gimple_seq *pre_p,
		      gimple_seq *post_p ATTRIBUTE_UNUSED)
{
  tree expr = *expr_p;
  tree decl = ATOMIC_EXPR_DECL (expr);
  tree target, val, order, return_arg;
  tree BI_type, BI_return_type, target_type;
  unsigned nargs = atomic_expr_nargs (expr);
  gimple ret;
  enum built_in_function fcode;
  enum tree_code arith_op;
  bool fetch_op = true;
  tree deref_tmp;

  /* The ATOMIC_EXPR node's type indicates the function return type.  */
  BI_return_type = TREE_TYPE (expr);
  target_type = atomic_type (expr);
  BI_type = atomic_BI_type (target_type);

  /* Target will be set to NULL if there is no target.  */
  target = handle_atomic_target (expr, pre_p, target_type, BI_type);

  fcode = DECL_FUNCTION_CODE (decl);
  switch (fcode)
    {
    case BUILT_IN_ATOMIC_LOAD:
      if (!arg_count_ok (nargs, 3) || !verify_ptr_arg (target_type, expr, 1))
        {
	  expr_p = 0;
	  return GS_ALL_DONE;
	}

      return_arg = get_atomic_addr (BI_type, &ATOMIC_EXPR_ARG (expr, 1), pre_p,
				    1);
      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 2), pre_p);

      if (atomic_lockfree_compatible (target_type))
        {
	  ret = gimple_build_atomic_load (BI_type, target, order);
	  finish_atomic_stmt (ret, pre_p, expr_p, BI_type, BI_type);
	  store_into_atomic_param_pointer (return_arg, *expr_p, pre_p);
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}
      
      ret = gimple_build_atomic_load_generic (target_type, target, order,
					      return_arg);
      return finish_atomic_stmt (ret, pre_p, expr_p);

    case BUILT_IN_ATOMIC_LOAD_1:
    case BUILT_IN_ATOMIC_LOAD_2:
    case BUILT_IN_ATOMIC_LOAD_4:
    case BUILT_IN_ATOMIC_LOAD_8:
    case BUILT_IN_ATOMIC_LOAD_16:
      BI_type = atomic_func_type (fcode - BUILT_IN_ATOMIC_LOAD_N);
      /* Fallthru.  */
    case BUILT_IN_ATOMIC_LOAD_N:
      if (!arg_count_ok (nargs, 2))
        {
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}

      types_match (BI_type, target_type);

      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 1), pre_p);
      ret = gimple_build_atomic_load (BI_type, target, order);
      return finish_atomic_stmt (ret, pre_p, expr_p, BI_type, target_type);

    case BUILT_IN_ATOMIC_STORE:
      if (!arg_count_ok (nargs, 3) || !verify_ptr_arg (target_type, expr, 1))
        {
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}
      val = get_atomic_addr (BI_type, &ATOMIC_EXPR_ARG (expr, 1), pre_p, 1);
      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 2), pre_p);

      if (atomic_lockfree_compatible (target_type))
        {
	  deref_tmp = load_from_atomic_param_pointer (val, pre_p);
	  ret = gimple_build_atomic_store (BI_type, target, deref_tmp, order);
	  return finish_atomic_stmt (ret, pre_p, expr_p);
	}
      
      ret = gimple_build_atomic_store_generic (target_type, target, val, order);
      return finish_atomic_stmt (ret, pre_p, expr_p, NULL_TREE);

    case BUILT_IN_ATOMIC_STORE_1:
    case BUILT_IN_ATOMIC_STORE_2:
    case BUILT_IN_ATOMIC_STORE_4:
    case BUILT_IN_ATOMIC_STORE_8:
    case BUILT_IN_ATOMIC_STORE_16:
      BI_type = atomic_func_type (fcode - BUILT_IN_ATOMIC_STORE_N);
      /* Fallthru.  */
    case BUILT_IN_ATOMIC_STORE_N:
      if (!arg_count_ok (nargs, 3))
        {
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}
      types_match (BI_type, target_type);

      val = get_atomic_expression (BI_type, &ATOMIC_EXPR_ARG (expr, 1), pre_p);
      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 2), pre_p);
      ret = gimple_build_atomic_store (BI_type, target, val, order);
      return finish_atomic_stmt (ret, pre_p, expr_p, NULL_TREE);


    case BUILT_IN_ATOMIC_EXCHANGE:
      if (!arg_count_ok (nargs, 4) || !verify_ptr_arg (target_type, expr, 1))
        {
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}

      val = get_atomic_addr (BI_type, &ATOMIC_EXPR_ARG (expr, 1), pre_p, 1);
      return_arg = get_atomic_addr (BI_type, &ATOMIC_EXPR_ARG (expr, 2), pre_p,
				    2);
      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 3), pre_p);

      if (atomic_lockfree_compatible (BI_type))
        {
	  deref_tmp = load_from_atomic_param_pointer (val, pre_p);
	  ret = gimple_build_atomic_exchange (BI_type, target, deref_tmp,
					      order);			
	  finish_atomic_stmt (ret, pre_p, expr_p, BI_type, BI_type);
	  store_into_atomic_param_pointer (return_arg, *expr_p, pre_p);
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}
      

      ret = gimple_build_atomic_exchange_generic (target_type, target, val,
						  order, return_arg);
      return finish_atomic_stmt (ret, pre_p, expr_p);

    case BUILT_IN_ATOMIC_EXCHANGE_1:
    case BUILT_IN_ATOMIC_EXCHANGE_2:
    case BUILT_IN_ATOMIC_EXCHANGE_4:
    case BUILT_IN_ATOMIC_EXCHANGE_8:
    case BUILT_IN_ATOMIC_EXCHANGE_16:
      BI_type = atomic_func_type (fcode - BUILT_IN_ATOMIC_EXCHANGE_N);
      /* Fallthru.  */
    case BUILT_IN_ATOMIC_EXCHANGE_N:
      if (!arg_count_ok (nargs, 3))
        {
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}
      types_match (BI_type, target_type);

      val = get_atomic_expression (BI_type, &ATOMIC_EXPR_ARG (expr, 1), pre_p);
      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 2), pre_p);
      ret = gimple_build_atomic_exchange (BI_type, target, val, order);
      return finish_atomic_stmt (ret, pre_p, expr_p, BI_type, target_type);

    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE:
      {
	tree fail_order;
	tree expected;
	tree weak;
	bool is_weak = false;

	if (!arg_count_ok (nargs, 6) || !verify_ptr_arg (target_type, expr, 1)
	    || !verify_ptr_arg (target_type, expr, 2))
	  {
	    *expr_p = 0;
	    return GS_ALL_DONE;
	  }

	expected = get_atomic_addr (BI_type, &ATOMIC_EXPR_ARG (expr, 1), pre_p,
				    1);
	val = get_atomic_addr (BI_type, &ATOMIC_EXPR_ARG (expr, 2), pre_p, 2);
	weak = ATOMIC_EXPR_ARG (expr, 3);
	order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 4), pre_p);
	fail_order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 5), pre_p);

	if (host_integerp (weak, 0) && tree_low_cst (weak, 0) != 0)
	  is_weak = true;

	if (atomic_lockfree_compatible (BI_type))
	  {
	    deref_tmp = load_from_atomic_param_pointer (val, pre_p);
	    *expr_p = issue_cmpxchg (BI_type, target, expected, deref_tmp,
				     is_weak, order, fail_order, pre_p);
	    return GS_ALL_DONE;
	  }
      
	ret = gimple_build_atomic_compare_exchange_generic (target_type, 
			    target, expected, val, order, fail_order, is_weak);
	return finish_atomic_stmt (ret, pre_p, expr_p, BI_return_type,
				   boolean_type_node);
      }

    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_1:
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_2:
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_4:
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_8:
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_16:
      BI_type = atomic_func_type (fcode - BUILT_IN_ATOMIC_COMPARE_EXCHANGE_N);
      /* Fallthru.  */
   
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_N:
      {
	tree fail_order;
	tree expected;
	tree weak;
	bool is_weak = false;

	if (!arg_count_ok (nargs, 6) || !verify_ptr_arg (target_type, expr, 1))
	  {
	    *expr_p = 0;
	    return GS_ALL_DONE;
	  }
	types_match (BI_type, target_type);

	/* Translate the original
	     bool = cmp_xch (t,expect,...)
	       into
	     tmp1 = expect;
	     deref_tmp = *tmp1;
	     bool, tmp2 = cmp_xch (t,deref_tmp,e)
	     *tmp1 = tmp2;  */

	expected = get_atomic_addr (BI_type, &ATOMIC_EXPR_ARG (expr, 1), pre_p,
				    1);
	val = get_atomic_expression (BI_type, &ATOMIC_EXPR_ARG (expr, 2),
				     pre_p);
	weak = ATOMIC_EXPR_ARG (expr, 3);
	order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 4), pre_p);
	fail_order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 5), pre_p);

	if (host_integerp (weak, 0) && tree_low_cst (weak, 0) != 0)
	  is_weak = true;

	*expr_p = issue_cmpxchg (BI_type, target, expected, val, is_weak, order,
				 fail_order, pre_p);
	return GS_ALL_DONE;
      }
    case BUILT_IN_ATOMIC_TEST_AND_SET:
      if (!arg_count_ok (nargs, 2))
	{
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}
      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 1), pre_p);
      ret = gimple_build_atomic_test_and_set (target, order);
      return finish_atomic_stmt (ret, pre_p, expr_p, BI_return_type,
				 boolean_type_node);

    case BUILT_IN_ATOMIC_CLEAR:
      if (!arg_count_ok (nargs, 2))
	{
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}
      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 1), pre_p);
      ret = gimple_build_atomic_clear (target, order);
      return finish_atomic_stmt (ret, pre_p, expr_p);

    case BUILT_IN_ATOMIC_THREAD_FENCE:
      if (!arg_count_ok (nargs, 1))
	{
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}
      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 0), pre_p);
      ret = gimple_build_atomic_fence (order, true);
      return finish_atomic_stmt (ret, pre_p, expr_p);

    case BUILT_IN_ATOMIC_SIGNAL_FENCE:
      if (!arg_count_ok (nargs, 1))
	{
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}
      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 0), pre_p);
      ret = gimple_build_atomic_fence (order, false);
      return finish_atomic_stmt (ret, pre_p, expr_p);


/* CASE's for ATOMIC FETCH.  opcode OP with arithmetic operator OP, and
   FETCH_OP is true if the fetch preceeds the operation.  */
#define CASE_ATOMIC_OP(OP, CODE, FETCH_OP)	\
  case OP ## _1:					\
  case OP ## _2:					\
  case OP ## _4:					\
  case OP ## _8:					\
  case OP ## _16:					\
    BI_type = atomic_func_type (fcode - OP ## _N);	\
    /* Fallthru.  */					\
  case OP ## _N:					\
  arith_op = CODE;					\
  fetch_op = FETCH_OP;					\
  goto gimplify_atomic_fetch;

    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_FETCH_ADD, PLUS_EXPR, true);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_FETCH_SUB, MINUS_EXPR, true);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_FETCH_AND, BIT_AND_EXPR, true);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_FETCH_XOR, BIT_XOR_EXPR, true);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_FETCH_OR, BIT_IOR_EXPR, true);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_FETCH_NAND, BIT_NOT_EXPR, true);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_ADD_FETCH, PLUS_EXPR, false);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_SUB_FETCH, MINUS_EXPR, false);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_AND_FETCH, BIT_AND_EXPR, false);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_XOR_FETCH, BIT_XOR_EXPR, false);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_OR_FETCH, BIT_IOR_EXPR, false);
    CASE_ATOMIC_OP (BUILT_IN_ATOMIC_NAND_FETCH, BIT_NOT_EXPR, false);

gimplify_atomic_fetch:
      if (!arg_count_ok (nargs, 3))
	{
	  *expr_p = 0;
	  return GS_ALL_DONE;
	}
      types_match (BI_type, target_type);

      val = get_atomic_expression (BI_type, &ATOMIC_EXPR_ARG (expr, 1), pre_p);
      order = get_atomic_order (&ATOMIC_EXPR_ARG (expr, 2), pre_p);
      if (fetch_op)
	ret = gimple_build_atomic_fetch_op (BI_type, target, val, arith_op, order);
      else
	ret = gimple_build_atomic_op_fetch (BI_type, target, val, arith_op, order);
      return finish_atomic_stmt (ret, pre_p, expr_p, BI_type, target_type);


/* CASE's for SYNC FETCH.  opcode OP with arithmetic operator OP, and
   FETCH_OP is true if the fetch preceeds the operation.  */
#define CASE_SYNC_OP(OP, CODE, FETCH_OP)	\
  case OP ## _1:					\
  case OP ## _2:					\
  case OP ## _4:					\
  case OP ## _8:					\
  case OP ## _16:					\
    BI_type = atomic_func_type (fcode - OP ## _N);	\
    /* Fallthru.  */					\
  case OP ## _N:					\
  arith_op = CODE;					\
  fetch_op = FETCH_OP;					\
  goto gimplify_sync_fetch;

    CASE_SYNC_OP (BUILT_IN_SYNC_FETCH_AND_ADD, PLUS_EXPR, true);
    CASE_SYNC_OP (BUILT_IN_SYNC_FETCH_AND_SUB, MINUS_EXPR, true);
    CASE_SYNC_OP (BUILT_IN_SYNC_FETCH_AND_AND, BIT_AND_EXPR, true);
    CASE_SYNC_OP (BUILT_IN_SYNC_FETCH_AND_XOR, BIT_XOR_EXPR, true);
    CASE_SYNC_OP (BUILT_IN_SYNC_FETCH_AND_OR, BIT_IOR_EXPR, true);
    CASE_SYNC_OP (BUILT_IN_SYNC_FETCH_AND_NAND, BIT_NOT_EXPR, true);
    CASE_SYNC_OP (BUILT_IN_SYNC_ADD_AND_FETCH, PLUS_EXPR, false);
    CASE_SYNC_OP (BUILT_IN_SYNC_SUB_AND_FETCH, MINUS_EXPR, false);
    CASE_SYNC_OP (BUILT_IN_SYNC_AND_AND_FETCH, BIT_AND_EXPR, false);
    CASE_SYNC_OP (BUILT_IN_SYNC_XOR_AND_FETCH, BIT_XOR_EXPR, false);
    CASE_SYNC_OP (BUILT_IN_SYNC_OR_AND_FETCH, BIT_IOR_EXPR, false);
    CASE_SYNC_OP (BUILT_IN_SYNC_NAND_AND_FETCH, BIT_NOT_EXPR, false);

  gimplify_sync_fetch:
	if (!arg_count_ok (nargs, 2))
	  {
	    *expr_p = 0;
	    return GS_ALL_DONE;
	  }
	types_match (BI_type, target_type);

	val = get_atomic_expression (BI_type, &ATOMIC_EXPR_ARG (expr, 1), pre_p);
	order = build_int_cst (integer_type_node, MEMMODEL_SEQ_CST);
	if (fetch_op)
	  ret = gimple_build_atomic_fetch_op (BI_type, target, val, arith_op,
					      order);
	else
	  ret = gimple_build_atomic_op_fetch (BI_type, target, val, arith_op,
					      order);
	gimple_atomic_set_from_sync (ret, true);
	return finish_atomic_stmt (ret, pre_p, expr_p, BI_type, target_type);


      case BUILT_IN_SYNC_BOOL_COMPARE_AND_SWAP_1:
      case BUILT_IN_SYNC_BOOL_COMPARE_AND_SWAP_2:
      case BUILT_IN_SYNC_BOOL_COMPARE_AND_SWAP_4:
      case BUILT_IN_SYNC_BOOL_COMPARE_AND_SWAP_8:
      case BUILT_IN_SYNC_BOOL_COMPARE_AND_SWAP_16:
	BI_type = atomic_func_type (fcode - BUILT_IN_SYNC_LOCK_TEST_AND_SET_N);
	/* Fallthru.  */
      case BUILT_IN_SYNC_BOOL_COMPARE_AND_SWAP_N:
	{
	  tree expected;
	  if (!arg_count_ok (nargs, 3))
	    {
	      *expr_p = 0;
	      return GS_ALL_DONE;
	    }
	  types_match (BI_type, target_type);

	  expected = get_atomic_expression (BI_type, &ATOMIC_EXPR_ARG (expr, 1),
					    pre_p);
	  val = get_atomic_expression (BI_type, &ATOMIC_EXPR_ARG (expr, 2),
				       pre_p);
	  expected = issue_sync_cmpxchg (BI_type, target, expected, val, pre_p,
					 true);
	  *expr_p = handle_any_casts (BI_return_type, expected, pre_p);
	  return GS_ALL_DONE;
	}

      case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_1:
      case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_2:
      case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_4:
      case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_8:
      case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_16:
	BI_type = atomic_func_type (fcode - BUILT_IN_SYNC_LOCK_TEST_AND_SET_N);
	/* Fallthru.  */
      case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_N:
	{
	  tree expected;
	  if (!arg_count_ok (nargs, 3))
	    {
	      *expr_p = 0;
	      return GS_ALL_DONE;
	    }
	  types_match (BI_type, target_type);

	  expected = get_atomic_expression (BI_type, &ATOMIC_EXPR_ARG (expr, 1),
					    pre_p);
	  val = get_atomic_expression (BI_type, &ATOMIC_EXPR_ARG (expr, 2),
				       pre_p);
	  expected = issue_sync_cmpxchg (BI_type, target, expected, val, pre_p,
					 false);
	  *expr_p = handle_any_casts (BI_return_type, expected, pre_p);
	  return GS_ALL_DONE;
	}

      case BUILT_IN_SYNC_LOCK_TEST_AND_SET_1:
      case BUILT_IN_SYNC_LOCK_TEST_AND_SET_2:
      case BUILT_IN_SYNC_LOCK_TEST_AND_SET_4:
      case BUILT_IN_SYNC_LOCK_TEST_AND_SET_8:
      case BUILT_IN_SYNC_LOCK_TEST_AND_SET_16:
	BI_type = atomic_func_type (fcode - BUILT_IN_SYNC_LOCK_TEST_AND_SET_N);
	/* Fallthru.  */
      case BUILT_IN_SYNC_LOCK_TEST_AND_SET_N:
	if (!arg_count_ok (nargs, 2))
	  {
	    *expr_p = 0;
	    return GS_ALL_DONE;
	  }
	types_match (BI_type, target_type);

	order = build_int_cst (integer_type_node, MEMMODEL_ACQUIRE);
	val = get_atomic_expression (BI_type, &ATOMIC_EXPR_ARG (expr, 1),
				     pre_p);

        if (!INTEGRAL_TYPE_P (TREE_TYPE (val)))
	  {
	    error ("non-integer argument 1 to __sync_lock_test_and_set");
	    val = integer_zero_node;
	  }
	/* __sync_lock_test_and_set is an atomic exchange, but may be
	   implemented with restrictions on some targets.  */
	ret = gimple_build_atomic_exchange (BI_type, target, val, order);
	gimple_atomic_set_from_sync (ret, true);
	return finish_atomic_stmt (ret, pre_p, expr_p, BI_type, target_type);

      case BUILT_IN_SYNC_LOCK_RELEASE_1:
      case BUILT_IN_SYNC_LOCK_RELEASE_2:
      case BUILT_IN_SYNC_LOCK_RELEASE_4:
      case BUILT_IN_SYNC_LOCK_RELEASE_8:
      case BUILT_IN_SYNC_LOCK_RELEASE_16:
	BI_type = atomic_func_type (fcode - BUILT_IN_SYNC_LOCK_RELEASE_N);
	/* Fallthru.  */
      case BUILT_IN_SYNC_LOCK_RELEASE_N:
	if (!arg_count_ok (nargs, 1))
	  {
	    *expr_p = 0;
	    return GS_ALL_DONE;
	  }
	types_match (BI_type, target_type);

	val = build_int_cst (BI_type, 0);
	order = build_int_cst (integer_type_node, MEMMODEL_RELEASE);
	ret = gimple_build_atomic_store (BI_type, target, val, order);
	gimple_atomic_set_from_sync (ret, true);
	return finish_atomic_stmt (ret, pre_p, expr_p);

      case BUILT_IN_SYNC_SYNCHRONIZE:
	if (!arg_count_ok (nargs, 0))
	  {
	    *expr_p = 0;
	    return GS_ALL_DONE;
	  }
	order = build_int_cst (integer_type_node, MEMMODEL_SEQ_CST);
	ret = gimple_build_atomic_fence (order, true);
	gimple_atomic_set_from_sync (ret, true);
	return finish_atomic_stmt (ret, pre_p, expr_p);

      default:
	gcc_unreachable ();
      }

    *expr_p = error_mark_node;
    return GS_ERROR;
  }

