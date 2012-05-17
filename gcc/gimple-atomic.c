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


/* Return the type of atomic operation STMT.  */

static tree
get_atomic_type (gimple stmt)
{
  tree t;

  t = gimple_atomic_type (stmt);
  gcc_assert (INTEGRAL_TYPE_P (t));

  return t;
}


/* Extract memory model from tree EXP, and verify it is valid for KIND.  */

static enum memmodel
get_memmodel (enum gimple_atomic_kind kind, tree exp)
{
  rtx op;
  enum memmodel model;

  /* If the parameter is not a constant, it's a run time value so we'll just
     convert it to MEMMODEL_SEQ_CST to avoid annoying runtime checking.  */
  if (TREE_CODE (exp) != INTEGER_CST)
    return MEMMODEL_SEQ_CST;

  op = expand_normal (exp);
  if (INTVAL (op) < 0 || INTVAL (op) >= MEMMODEL_LAST)
    {
      warning (OPT_Winvalid_memory_model,
	       "invalid memory model argument for atomic operation");
      return MEMMODEL_SEQ_CST;
    }
  model = (enum memmodel) INTVAL (op);

  switch (kind)
    {
    case GIMPLE_ATOMIC_LOAD:
      if (model != MEMMODEL_RELEASE && model != MEMMODEL_ACQ_REL)
        return model;
      break;

    case GIMPLE_ATOMIC_STORE:
      if (model == MEMMODEL_RELAXED || model == MEMMODEL_SEQ_CST ||
	  model == MEMMODEL_RELEASE)
	 return model;
      break;

    case GIMPLE_ATOMIC_EXCHANGE:
      if (model != MEMMODEL_CONSUME)
        return model;
      break;

    case GIMPLE_ATOMIC_CLEAR:
      if (model != MEMMODEL_ACQUIRE && model != MEMMODEL_ACQ_REL)
        return model;
      break;

    default:
      return model;
    }

  error ("invalid memory model for atomic operation");
  return MEMMODEL_SEQ_CST;
}

/* Verify that all the memory model's are valid for STMT.  */

void
gimple_verify_memmodel (gimple stmt)
{
  enum memmodel a,b;

  a = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));

  if (gimple_atomic_kind (stmt) != GIMPLE_ATOMIC_COMPARE_EXCHANGE)
    return;

  b = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_fail_order (stmt));
  if (b == MEMMODEL_RELEASE || b == MEMMODEL_ACQ_REL)
    error ("invalid failure memory model for %<__atomic_compare_exchange%>");
  if (b > a)
    error ("failure memory model cannot be stronger than success "
	   "memory model for %<__atomic_compare_exchange%>");
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
get_atomic_lhs_rtx (gimple stmt, unsigned index)
{
  tree tree_lhs;
  rtx rtl_lhs;
  
  tree_lhs = gimple_atomic_lhs (stmt, index);
  if (!tree_lhs)
    return NULL_RTX;

  gcc_assert (TREE_CODE (tree_lhs) == SSA_NAME);

  rtl_lhs = expand_expr (tree_lhs, NULL_RTX, VOIDmode, EXPAND_WRITE);
  return rtl_lhs;
}

/* Expand STMT into a library call.  */

static VEC(tree,gc) *
create_atomic_library_args (gimple stmt, VEC(tree,gc) *params)
{
  bool generic = gimple_atomic_generic (stmt);

  /* Generic's require the size be the first argument.  */
  if (generic)
    {
      tree t = TYPE_SIZE_UNIT (gimple_atomic_type (stmt));
      VEC_quick_push (tree, params, t);
    }

  switch (gimple_atomic_kind (stmt))
    {
    case GIMPLE_ATOMIC_LOAD:
      VEC_quick_push (tree, params, gimple_atomic_target (stmt));
      if (generic)
	VEC_quick_push (tree, params, gimple_atomic_return (stmt));
      VEC_quick_push (tree, params, gimple_atomic_order (stmt));
      break;
      
    case GIMPLE_ATOMIC_STORE:
      VEC_quick_push (tree, params, gimple_atomic_target (stmt));
      VEC_quick_push (tree, params, gimple_atomic_expr (stmt));
      VEC_quick_push (tree, params, gimple_atomic_order (stmt));
      break;
 
    case GIMPLE_ATOMIC_EXCHANGE:
      VEC_quick_push (tree, params, gimple_atomic_target (stmt));
      VEC_quick_push (tree, params, gimple_atomic_expr (stmt));
      if (generic)
	VEC_quick_push (tree, params, gimple_atomic_return (stmt));
      VEC_quick_push (tree, params, gimple_atomic_order (stmt));
      break;

    case GIMPLE_ATOMIC_COMPARE_EXCHANGE:
      VEC_quick_push (tree, params, gimple_atomic_target (stmt));
      VEC_quick_push (tree, params, gimple_atomic_expected (stmt));
      VEC_quick_push (tree, params, gimple_atomic_expr (stmt));
      VEC_quick_push (tree, params, gimple_atomic_order (stmt));
      VEC_quick_push (tree, params, gimple_atomic_fail_order (stmt));
      break;

    case GIMPLE_ATOMIC_FETCH_OP:
    case GIMPLE_ATOMIC_OP_FETCH:
      VEC_quick_push (tree, params, gimple_atomic_target (stmt));
      VEC_quick_push (tree, params, gimple_atomic_expr (stmt));
      VEC_quick_push (tree, params, gimple_atomic_order (stmt));
      break;

    default:
      /* The other atomic kinds cannot generate external library calls.  */
      gcc_unreachable ();
    }

  return params;
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

static tree
atomic_library_name (gimple stmt, tree *t) 
{
  char name[30];
  tree fn, fntype;

  if (gimple_atomic_generic (stmt))
    fntype = void_type_node;
  else
    fntype = gimple_atomic_type (stmt);

  strcpy (name,"__atomic_");
  switch (gimple_atomic_kind (stmt))
    {
    case GIMPLE_ATOMIC_LOAD:
      strcat (name, "load");
      break;
    case GIMPLE_ATOMIC_STORE:
      strcat (name, "store");
      fntype = void_type_node;
      break;
    case GIMPLE_ATOMIC_EXCHANGE:
      strcat (name, "exchange");
      break;
    case GIMPLE_ATOMIC_COMPARE_EXCHANGE:
      strcat (name, "compare_exchange");
      fntype = boolean_type_node;
      break;
    case GIMPLE_ATOMIC_FETCH_OP:
      strcat (name, "fetch_");
      strcat (name, gimple_atomic_op_name (stmt));
      break;
    case GIMPLE_ATOMIC_OP_FETCH:
      strcat (name, gimple_atomic_op_name (stmt));
      strcat (name, "_fetch");
      break;
    default:
     gcc_unreachable ();
    }

  if (!gimple_atomic_generic (stmt))
    strcat (name, gimple_atomic_type_size_string (stmt));

  fn = build_fn_decl (name, fntype);
  *t = fntype;
  return fn;
}

void
expand_gimple_atomic_library_call (gimple stmt) 
{
  bool generic = gimple_atomic_generic (stmt);
  unsigned nargs;
  VEC(tree,gc) *params;
  tree call_exp, t;
  unsigned x;

  /* Verify the models if inlining hasn't been attempted.  */
  if (generic || !flag_inline_atomics)
    get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));

  params = VEC_alloc (tree, gc, gimple_num_ops (stmt) + 1);
  create_atomic_library_args (stmt, params);
  nargs = VEC_length (tree, params);

  call_exp = build_vl_exp (CALL_EXPR, nargs + 3);
  CALL_EXPR_FN (call_exp) = atomic_library_name (stmt, &t);
  TREE_TYPE (call_exp) = t;
  TREE_SIDE_EFFECTS (call_exp) = 1;
  TREE_NOTHROW (call_exp) = 1;
  SET_EXPR_LOCATION (call_exp, gimple_location (stmt));
  TREE_BLOCK (call_exp) = gimple_block (stmt);

  for (x = 0; x < nargs; x++)
   CALL_EXPR_ARG (call_exp, x) = VEC_index (tree, params, x);

  /* TODO need to worry about debug args?  */

  if (gimple_atomic_num_lhs (stmt) > 0)
    expand_assignment (gimple_atomic_lhs (stmt, 0), call_exp, false);
  else
    expand_expr_real_1 (call_exp, const0_rtx, VOIDmode, EXPAND_NORMAL, NULL);

  /* mark_transaction_restart_calls (stmt);  TODO  need this?  */
}

/* Expand atomic load STMT into RTL.  Return true if successful.  */

bool
expand_gimple_atomic_load (gimple stmt)
{
  enum machine_mode mode;
  enum memmodel model;
  tree type;
  rtx mem, rtl_rhs, rtl_lhs;

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_LOAD);

  type = get_atomic_type (stmt);
  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));

  mem = expand_atomic_target (gimple_atomic_target (stmt), mode);

  rtl_lhs = get_atomic_lhs_rtx (stmt, 0);
  rtl_rhs = expand_atomic_load (rtl_lhs, mem, model);

  /* If no rtl is generated, indicate the code was not inlined.  */
  if (!rtl_rhs)
    return false;

  if (rtl_lhs)
    expand_gimple_assign_move (TREE_TYPE (type), rtl_lhs, rtl_rhs, false);
  return true;
}


/* Expand atomic store STMT into RTL.  Return true if successful.  */

bool
expand_gimple_atomic_store (gimple stmt)
{
  rtx mem, val, rtl_rhs;
  enum memmodel model;
  enum machine_mode mode;
  tree type;

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_STORE);

  type = get_atomic_type (stmt);
  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));

  /* Expand the operands.  */
  mem = expand_atomic_target (gimple_atomic_target (stmt), mode);
  val = expand_expr_force_mode (gimple_atomic_expr (stmt), mode);

  rtl_rhs = expand_atomic_store (mem, val, model, false);

  /* If no rtl is generated, indicate the code was not inlined.  */
  if (!rtl_rhs)
    return false;

  return true;
}

/* Expand atomic exchange STMT into RTL.  Return true if successful.  */

bool
expand_gimple_atomic_exchange (gimple stmt)
{
  rtx mem, val, rtl_rhs, rtl_lhs;
  enum memmodel model;
  enum machine_mode mode;
  tree type;

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_EXCHANGE);

  type = get_atomic_type (stmt);
  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));

  /* Expand the operands.  */
  mem = expand_atomic_target (gimple_atomic_target (stmt), mode);
  val = expand_expr_force_mode (gimple_atomic_expr (stmt), mode);

  rtl_lhs = get_atomic_lhs_rtx (stmt, 0);
  rtl_rhs = expand_atomic_exchange (rtl_lhs, mem, val, model);

  /* If no rtl is generated, indicate the code was not inlined.  */
  if (!rtl_rhs)
    return false;

  if (rtl_lhs)
    expand_gimple_assign_move (TREE_TYPE (type), rtl_lhs, rtl_rhs, false);
  return true;
}

/* Expand atomic compare_exchange STMT into RTL.  Return true if successful.  */

bool
expand_gimple_atomic_compare_exchange (gimple stmt)
{
  rtx mem, val, rtl_lhs1, rtl_lhs2, expect;
  rtx real_lhs1, real_lhs2;
  enum memmodel success, failure;
  enum machine_mode mode;
  tree type;
  bool is_weak, emitted;

  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_COMPARE_EXCHANGE);

  type = get_atomic_type (stmt);
  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  success = get_memmodel (gimple_atomic_kind (stmt),  gimple_atomic_order (stmt));
  failure = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_fail_order (stmt));

  /* compare_exchange has additional restrictions on the failure order.  */
  if (failure == MEMMODEL_RELEASE || failure == MEMMODEL_ACQ_REL)
    error ("invalid failure memory model for %<__atomic_compare_exchange%>");

  if (failure > success)
    {
      error ("failure memory model cannot be stronger than success "
	     "memory model for %<__atomic_compare_exchange%>");
    }
  
  /* Expand the operands.  */
  mem = expand_atomic_target (gimple_atomic_target (stmt), mode);
  val = expand_expr_force_mode (gimple_atomic_expr (stmt), mode);
  expect = expand_expr_force_mode (gimple_atomic_expected (stmt), mode);
  is_weak = gimple_atomic_weak (stmt);

  rtl_lhs1 = get_atomic_lhs_rtx (stmt, 0);
  rtl_lhs2 = get_atomic_lhs_rtx (stmt, 1);
  real_lhs1 = rtl_lhs1;
  real_lhs2 = rtl_lhs2;
  emitted = expand_atomic_compare_and_swap (&real_lhs1, &real_lhs2, mem, expect,
					    val, is_weak, success, failure);

  /* If no rtl is generated, indicate the code was not inlined.  */
  if (!emitted)
    return false;

  if (rtl_lhs1)
    expand_gimple_assign_move (TREE_TYPE (type), rtl_lhs1, real_lhs1, false);
  /* The second result is not optional.  */
  expand_gimple_assign_move (TREE_TYPE (type), rtl_lhs2, real_lhs2, false);
  return true;
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


/* Expand atomic fetch operation STMT into RTL.  FETCH_AFTER is true if the 
   value returned is the post operation value.  Return true if successful.  */

static bool
expand_atomic_fetch (gimple stmt, bool fetch_after)
{
  rtx mem, val, rtl_rhs, rtl_lhs;
  enum memmodel model;
  enum machine_mode mode;
  tree type;
  enum rtx_code rcode;


  type = get_atomic_type (stmt);
  mode = mode_for_size (tree_low_cst (TYPE_SIZE (type), 1), MODE_INT, 0);
  gcc_assert (mode != BLKmode);

  model = get_memmodel (gimple_atomic_kind (stmt), gimple_atomic_order (stmt));

  /* Expand the operands.  */
  mem = expand_atomic_target (gimple_atomic_target (stmt), mode);
  val = expand_expr_force_mode (gimple_atomic_expr (stmt), mode);
  rcode = rtx_code_from_atomic_op (gimple_atomic_op_code (stmt));

  rtl_lhs = get_atomic_lhs_rtx (stmt, 0);
  rtl_rhs = expand_atomic_fetch_op (rtl_lhs, mem, val, rcode, model,
				    fetch_after);

  /* If no rtl is generated, indicate the code was not inlined.  */
  if (!rtl_rhs)
    return false;

  /* If the result is used, make sure its in correct LHS.  */
  if (rtl_lhs)
    expand_gimple_assign_move (TREE_TYPE (type), rtl_lhs, rtl_rhs, false);
  return true;
}


/* Expand atomic fetch_op operation STMT into RTL.  Return true if successful.  */

bool
expand_gimple_atomic_fetch_op (gimple stmt)
{
  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_FETCH_OP);
  return expand_atomic_fetch (stmt, false);
}

/* Expand atomic fetch_op operation STMT into RTL.  Return true if successful.  */

bool
expand_gimple_atomic_op_fetch (gimple stmt)
{
  gcc_assert (gimple_atomic_kind (stmt) == GIMPLE_ATOMIC_OP_FETCH);
  return expand_atomic_fetch (stmt, true);
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

  rtl_lhs = get_atomic_lhs_rtx (stmt, 0);
  rtl_rhs = expand_atomic_test_and_set (rtl_lhs, mem, model);

  /* Test and set is not allowed to fail.  */
  gcc_assert (rtl_rhs);

  if (rtl_lhs)
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


/* Return true if FNDECL is an atomic builtin function that can be mapped to a
   GIMPLE_ATOMIC statement.  */

static bool
is_built_in_atomic (tree fndecl)
{
  enum built_in_function fcode;


  if (!fndecl || !DECL_BUILT_IN (fndecl))
    return false;

  if (DECL_BUILT_IN_CLASS (fndecl) != BUILT_IN_NORMAL)
    return false;

  fcode = DECL_FUNCTION_CODE (fndecl);
  if (fcode >= BUILT_IN_ATOMIC_TEST_AND_SET &&
      fcode <= BUILT_IN_ATOMIC_SIGNAL_FENCE)
    return true;

  return false;
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

/* Convert an atomic builtin call at GSI_P into a GIMPLE_ATOMIC statement.  */

static void
lower_atomic_call (gimple_stmt_iterator *gsi_p)
{
  tree fndecl;
  enum built_in_function fcode;
  gimple s = NULL;
  tree order;
  tree target;
  tree expr;
  tree type;
  tree return_arg;
  enum tree_code op;
  bool fetch_op;
  gimple stmt = gsi_stmt (*gsi_p);

  fndecl = gimple_call_fndecl (stmt);
  gcc_assert (is_built_in_atomic (fndecl));

  fcode = DECL_FUNCTION_CODE (fndecl);

  switch (fcode) {
    case BUILT_IN_ATOMIC_LOAD:
      gcc_checking_assert (gimple_call_num_args (stmt) == 3);
      target = gimple_call_arg (stmt, 0);
      return_arg = gimple_call_arg (stmt, 1);
      order = gimple_call_arg (stmt, 2);

      gcc_checking_assert (POINTER_TYPE_P (TREE_TYPE (target)));
      type = TREE_TYPE (TREE_TYPE (target));
      s = gimple_build_atomic_load (type, target, order, return_arg);
      break;

    case BUILT_IN_ATOMIC_LOAD_N:
    case BUILT_IN_ATOMIC_LOAD_1:
    case BUILT_IN_ATOMIC_LOAD_2:
    case BUILT_IN_ATOMIC_LOAD_4:
    case BUILT_IN_ATOMIC_LOAD_8:
    case BUILT_IN_ATOMIC_LOAD_16:
      gcc_checking_assert (gimple_call_num_args (stmt) == 2);
      target = gimple_call_arg (stmt, 0);
      order = gimple_call_arg (stmt, 1);
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_LOAD_N);
      s = gimple_build_atomic_load (type, target, order, NULL_TREE);
      if (gimple_call_lhs (stmt))
        gimple_atomic_set_lhs (s, 0, gimple_call_lhs (stmt));
      break;

   case BUILT_IN_ATOMIC_STORE:
      gcc_assert (gimple_call_num_args (stmt) == 3);
      target = gimple_call_arg (stmt, 0);
      expr = gimple_call_arg (stmt, 1);
      order = gimple_call_arg (stmt, 2);

      gcc_checking_assert (POINTER_TYPE_P (TREE_TYPE (target)));
      type = TREE_TYPE (TREE_TYPE (target));
      s = gimple_build_atomic_store (type, target, expr, order);
      gimple_atomic_set_generic (s, true);
      break;

    case BUILT_IN_ATOMIC_STORE_N:
    case BUILT_IN_ATOMIC_STORE_1:
    case BUILT_IN_ATOMIC_STORE_2:
    case BUILT_IN_ATOMIC_STORE_4:
    case BUILT_IN_ATOMIC_STORE_8:
    case BUILT_IN_ATOMIC_STORE_16:
      gcc_assert (gimple_call_num_args (stmt) == 3);
      target = gimple_call_arg (stmt, 0);
      expr = gimple_call_arg (stmt, 1);
      order = gimple_call_arg (stmt, 2);
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_STORE_N);
      s = gimple_build_atomic_store (type, target, expr, order);
      break;


    case BUILT_IN_ATOMIC_EXCHANGE:
      gcc_assert (gimple_call_num_args (stmt) == 4);
      target = gimple_call_arg (stmt, 0);
      expr = gimple_call_arg (stmt, 1);
      return_arg = gimple_call_arg (stmt, 2);
      order = gimple_call_arg (stmt, 3);

      gcc_checking_assert (POINTER_TYPE_P (TREE_TYPE (target)));
      type = TREE_TYPE (TREE_TYPE (target));
      s = gimple_build_atomic_exchange (type, target, expr, order, return_arg);
      break;


    case BUILT_IN_ATOMIC_EXCHANGE_N:
    case BUILT_IN_ATOMIC_EXCHANGE_1:
    case BUILT_IN_ATOMIC_EXCHANGE_2:
    case BUILT_IN_ATOMIC_EXCHANGE_4:
    case BUILT_IN_ATOMIC_EXCHANGE_8:
    case BUILT_IN_ATOMIC_EXCHANGE_16:
      gcc_assert (gimple_call_num_args (stmt) == 3);
      target = gimple_call_arg (stmt, 0);
      expr = gimple_call_arg (stmt, 1);
      order = gimple_call_arg (stmt, 2);
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_EXCHANGE_N);
      s = gimple_build_atomic_exchange (type, target, expr, order, NULL_TREE);
      if (gimple_call_lhs (stmt))
        gimple_atomic_set_lhs (s, 0, gimple_call_lhs (stmt));
      break;

    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE:
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_N:
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_1:
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_2:
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_4:
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_8:
    case BUILT_IN_ATOMIC_COMPARE_EXCHANGE_16:
      {
        tree tmp1, tmp2, tmp1_type, tmp2_type, deref, deref_tmp;
        tree expected, fail, weak;
	bool is_weak = false;

        gcc_assert (gimple_call_num_args (stmt) == 6);
	target = gimple_call_arg (stmt, 0);
	expected = gimple_call_arg (stmt, 1);
	expr = gimple_call_arg (stmt, 2);
	weak = gimple_call_arg (stmt, 3);
	if (host_integerp (weak, 0) && tree_low_cst (weak, 0) != 0)
	    is_weak = true;
	order = gimple_call_arg (stmt, 4);
	fail = gimple_call_arg (stmt, 5);

	/* The generic form of compare exchange is trivial.  */
	if (fcode == BUILT_IN_ATOMIC_COMPARE_EXCHANGE)
	  {
	    gcc_checking_assert (POINTER_TYPE_P (TREE_TYPE (target)));
	    type = TREE_TYPE (TREE_TYPE (target));
	    s = gimple_build_atomic_compare_exchange (type, target, expected,
						      expr, order, fail,
						      is_weak);
	    gimple_atomic_set_generic (s , true);
	    gimple_atomic_set_lhs (s, 0, gimple_call_lhs (stmt));
	    break;
	  }
	type = atomic_func_type (fcode - BUILT_IN_ATOMIC_COMPARE_EXCHANGE_N);

	/* TODO : Translate the original
	   bool = cmp_xch (t,expect,...)
	      into
	   tmp1 = expect;
	   bool, tmp2 = cmp_xch (t,*tmp1,e)
	   *tmp1 = tmp2;  */
	/* tmp1 = expect */
	tmp1_type = TREE_TYPE (expected);
	gcc_assert (POINTER_TYPE_P (tmp1_type));
	tmp2_type = TREE_TYPE (tmp1_type);
#if 0
	if (TREE_CODE (expected) == ADDR_EXPR)
	  tmp2_type = TREE_TYPE (TREE_OPERAND (expected, 0));
	else if (tmp1_type == REFERENCE_TYPE)
	else
	  /* Handle other tree codes as this reaches. */
	  gcc_unreachable ();
#endif

	tmp1 = create_tmp_var (tmp1_type, "cmpxchg_p");
	s = gimple_build_assign (tmp1, expected);
	gimple_set_location (s, gimple_location (stmt));
	gsi_insert_before (gsi_p, s, GSI_SAME_STMT);

	/* deref_tmp = *tmp1 */
	deref = build2 (MEM_REF, tmp2_type, tmp1, 
			build_int_cst_wide (tmp1_type, 0, 0));
	/* If *expected is not integral, convert it.  */
	if (!INTEGRAL_TYPE_P (tmp2_type))
	  {
	    tmp2_type = type;
	    deref = build1 (VIEW_CONVERT_EXPR, tmp2_type, deref);
	  }
	deref_tmp = create_tmp_var (tmp2_type, "cmpxchg_d");
	s = gimple_build_assign (deref_tmp, deref);
	gimple_set_location (s, gimple_location (stmt));
	gsi_insert_before (gsi_p, s, GSI_SAME_STMT);

        /* bool, tmp2 = cmp_exchange (t, deref_tmp, ...) */
	s = gimple_build_atomic_compare_exchange (type, target, deref_tmp, expr,
						  order, fail, is_weak);
	gimple_atomic_set_lhs (s, 0, gimple_call_lhs (stmt));

	tmp2 = create_tmp_var (tmp2_type, "cmpxchg");
	gimple_atomic_set_lhs (s, 1, tmp2);
	gimple_set_location (s, gimple_location (stmt));

	gsi_insert_before (gsi_p, s, GSI_SAME_STMT);

	/* *tmp1 = tmp2  */
	if (tmp2_type != TREE_TYPE (tmp1_type))
	  {
	    tree tmp3;
	    tmp2_type = TREE_TYPE (tmp1_type);
	    tmp2 = build1 (VIEW_CONVERT_EXPR, tmp2_type, tmp2);
	    tmp3 = create_tmp_var (tmp2_type, "cmpxchg_t");
	    s = gimple_build_assign (tmp3, tmp2);
	    gimple_set_location (s, gimple_location (stmt));
	    gsi_insert_before (gsi_p, s, GSI_SAME_STMT);
	    tmp2 = tmp3;
	  }
	deref = build2 (MEM_REF, tmp2_type, tmp1, 
			build_int_cst_wide (tmp1_type, 0, 0));
	s = gimple_build_assign (deref, tmp2);
	break;
      }

    case BUILT_IN_ATOMIC_ADD_FETCH_N:
    case BUILT_IN_ATOMIC_ADD_FETCH_1:
    case BUILT_IN_ATOMIC_ADD_FETCH_2:
    case BUILT_IN_ATOMIC_ADD_FETCH_4:
    case BUILT_IN_ATOMIC_ADD_FETCH_8:
    case BUILT_IN_ATOMIC_ADD_FETCH_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_ADD_FETCH_N);
      op = PLUS_EXPR;
      fetch_op = false;
fetch_body:
      gcc_assert (gimple_call_num_args (stmt) == 3);
      target = gimple_call_arg (stmt, 0);
      expr = gimple_call_arg (stmt, 1);
      order = gimple_call_arg (stmt, 2);
      if (fetch_op)
	s = gimple_build_atomic_fetch_op (type, target, expr, op, order);
      else
	s = gimple_build_atomic_op_fetch (type, target, expr, op, order);
      if (gimple_call_lhs (stmt))
        gimple_atomic_set_lhs (s, 0, gimple_call_lhs (stmt));
      break;

    case BUILT_IN_ATOMIC_FETCH_ADD_N:
    case BUILT_IN_ATOMIC_FETCH_ADD_1:
    case BUILT_IN_ATOMIC_FETCH_ADD_2:
    case BUILT_IN_ATOMIC_FETCH_ADD_4:
    case BUILT_IN_ATOMIC_FETCH_ADD_8:
    case BUILT_IN_ATOMIC_FETCH_ADD_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_FETCH_ADD_N);
      op = PLUS_EXPR;
      fetch_op = true;
      goto fetch_body;

    case BUILT_IN_ATOMIC_SUB_FETCH_N:
    case BUILT_IN_ATOMIC_SUB_FETCH_1:
    case BUILT_IN_ATOMIC_SUB_FETCH_2:
    case BUILT_IN_ATOMIC_SUB_FETCH_4:
    case BUILT_IN_ATOMIC_SUB_FETCH_8:
    case BUILT_IN_ATOMIC_SUB_FETCH_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_SUB_FETCH_N);
      op = MINUS_EXPR;
      fetch_op = false;
      goto fetch_body;

    case BUILT_IN_ATOMIC_FETCH_SUB_N:
    case BUILT_IN_ATOMIC_FETCH_SUB_1:
    case BUILT_IN_ATOMIC_FETCH_SUB_2:
    case BUILT_IN_ATOMIC_FETCH_SUB_4:
    case BUILT_IN_ATOMIC_FETCH_SUB_8:
    case BUILT_IN_ATOMIC_FETCH_SUB_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_FETCH_SUB_N);
      op = MINUS_EXPR;
      fetch_op = true;
      goto fetch_body;

    case BUILT_IN_ATOMIC_AND_FETCH_N:
    case BUILT_IN_ATOMIC_AND_FETCH_1:
    case BUILT_IN_ATOMIC_AND_FETCH_2:
    case BUILT_IN_ATOMIC_AND_FETCH_4:
    case BUILT_IN_ATOMIC_AND_FETCH_8:
    case BUILT_IN_ATOMIC_AND_FETCH_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_AND_FETCH_N);
      op = BIT_AND_EXPR;
      fetch_op = false;
      goto fetch_body;

    case BUILT_IN_ATOMIC_FETCH_AND_N:
    case BUILT_IN_ATOMIC_FETCH_AND_1:
    case BUILT_IN_ATOMIC_FETCH_AND_2:
    case BUILT_IN_ATOMIC_FETCH_AND_4:
    case BUILT_IN_ATOMIC_FETCH_AND_8:
    case BUILT_IN_ATOMIC_FETCH_AND_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_FETCH_AND_N);
      op = BIT_AND_EXPR;
      fetch_op = true;
      goto fetch_body;

    case BUILT_IN_ATOMIC_XOR_FETCH_N:
    case BUILT_IN_ATOMIC_XOR_FETCH_1:
    case BUILT_IN_ATOMIC_XOR_FETCH_2:
    case BUILT_IN_ATOMIC_XOR_FETCH_4:
    case BUILT_IN_ATOMIC_XOR_FETCH_8:
    case BUILT_IN_ATOMIC_XOR_FETCH_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_XOR_FETCH_N);
      op = BIT_XOR_EXPR;
      fetch_op = false;
      goto fetch_body;

    case BUILT_IN_ATOMIC_FETCH_XOR_N:
    case BUILT_IN_ATOMIC_FETCH_XOR_1:
    case BUILT_IN_ATOMIC_FETCH_XOR_2:
    case BUILT_IN_ATOMIC_FETCH_XOR_4:
    case BUILT_IN_ATOMIC_FETCH_XOR_8:
    case BUILT_IN_ATOMIC_FETCH_XOR_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_FETCH_XOR_N);
      op = BIT_XOR_EXPR;
      fetch_op = true;
      goto fetch_body;

    case BUILT_IN_ATOMIC_OR_FETCH_N:
    case BUILT_IN_ATOMIC_OR_FETCH_1:
    case BUILT_IN_ATOMIC_OR_FETCH_2:
    case BUILT_IN_ATOMIC_OR_FETCH_4:
    case BUILT_IN_ATOMIC_OR_FETCH_8:
    case BUILT_IN_ATOMIC_OR_FETCH_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_OR_FETCH_N);
      op = BIT_IOR_EXPR;
      fetch_op = false;
      goto fetch_body;

    case BUILT_IN_ATOMIC_FETCH_OR_N:
    case BUILT_IN_ATOMIC_FETCH_OR_1:
    case BUILT_IN_ATOMIC_FETCH_OR_2:
    case BUILT_IN_ATOMIC_FETCH_OR_4:
    case BUILT_IN_ATOMIC_FETCH_OR_8:
    case BUILT_IN_ATOMIC_FETCH_OR_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_FETCH_OR_N);
      op = BIT_IOR_EXPR;
      fetch_op = true;
      goto fetch_body;

    case BUILT_IN_ATOMIC_NAND_FETCH_N:
    case BUILT_IN_ATOMIC_NAND_FETCH_1:
    case BUILT_IN_ATOMIC_NAND_FETCH_2:
    case BUILT_IN_ATOMIC_NAND_FETCH_4:
    case BUILT_IN_ATOMIC_NAND_FETCH_8:
    case BUILT_IN_ATOMIC_NAND_FETCH_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_NAND_FETCH_N);
      op = BIT_NOT_EXPR;
      fetch_op = false;
      goto fetch_body;

    case BUILT_IN_ATOMIC_FETCH_NAND_N:
    case BUILT_IN_ATOMIC_FETCH_NAND_1:
    case BUILT_IN_ATOMIC_FETCH_NAND_2:
    case BUILT_IN_ATOMIC_FETCH_NAND_4:
    case BUILT_IN_ATOMIC_FETCH_NAND_8:
    case BUILT_IN_ATOMIC_FETCH_NAND_16:
      type = atomic_func_type (fcode - BUILT_IN_ATOMIC_FETCH_NAND_N);
      op = BIT_NOT_EXPR;
      fetch_op = true;
      goto fetch_body;

    case BUILT_IN_ATOMIC_TEST_AND_SET:
      gcc_assert (gimple_call_num_args (stmt) == 2);
      target = gimple_call_arg (stmt, 0);
      order = gimple_call_arg (stmt, 1);
      s = gimple_build_atomic_test_and_set (target, order);
      if (gimple_call_lhs (stmt))
        gimple_atomic_set_lhs (s, 0, gimple_call_lhs (stmt));
      break;

    case BUILT_IN_ATOMIC_CLEAR:
      gcc_assert (gimple_call_num_args (stmt) == 2);
      target = gimple_call_arg (stmt, 0);
      order = gimple_call_arg (stmt, 1);
      s = gimple_build_atomic_clear (target, order);
      break;

    case BUILT_IN_ATOMIC_THREAD_FENCE:
      gcc_assert (gimple_call_num_args (stmt) == 1);
      order = gimple_call_arg (stmt, 0);
      s = gimple_build_atomic_fence (order, true);
      break;

    case BUILT_IN_ATOMIC_SIGNAL_FENCE:
      gcc_assert (gimple_call_num_args (stmt) == 1);
      order = gimple_call_arg (stmt, 0);
      s = gimple_build_atomic_fence (order, false);
      break;

    default:
      gcc_unreachable ();
  }
 
 gcc_assert (s != NULL);

 gimple_set_location (s, gimple_location (stmt));
 gsi_insert_after (gsi_p, s, GSI_SAME_STMT);
 gsi_remove (gsi_p, true);

}

/* Conversion of atomic builtin functions to tree codes.  Scan the
   function looking for BUILT_IN_ATOMIC_* functions and replace them with
   the eqivilent atomic tree codes.  */

static unsigned int
lower_atomics (void)
{
  basic_block bb;
  gimple_stmt_iterator gsi;

  FOR_EACH_BB (bb)
    {
      for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
      	{
	  if (gimple_code (gsi_stmt (gsi)) == GIMPLE_CALL)
	    {
	      if (is_built_in_atomic (gimple_call_fndecl (gsi_stmt (gsi))))
		lower_atomic_call (&gsi);
	    }
	}
    }
  return 0;
}


/* Gate to enable lowering of atomic operations.  As this will replace the 
   built-in support, always do it.  */

static bool
gate_lower_atomics (void)
{
  return 1;
}

struct gimple_opt_pass pass_lower_atomics =
{
  {
    GIMPLE_PASS,
    "lower_atomics",			/* name */
    gate_lower_atomics,			/* gate */
    lower_atomics,			/* execute */
    NULL,				/* sub */
    NULL,				/* next */
    0,					/* static_pass_number */
    TV_NONE,				/* tv_id */
    PROP_cfg,				/* properties_required */
    0,					/* properties_provided */
    0,					/* properties_destroyed */
    0,					/* todo_flags_start */
    0,					/* todo_flags_finish */
  }
};

