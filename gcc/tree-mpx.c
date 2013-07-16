/* MPX insrumentation pass.
   Copyright (C) 2013 Free Software Foundation, Inc.
   Contributed by Ilya Enkovich (ilya.enkovich@intel.com)

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
#include "target.h"
#include "tree-iterator.h"
#include "tree-flow.h"
#include "langhooks.h"
#include "tree-pass.h"
#include "hashtab.h"
#include "diagnostic.h"
#include "ggc.h"
#include "rtl.h"
#include "expr.h"
#include "output.h"
#include "gimple-pretty-print.h"
#include "cfgloop.h"

/*  MPX pass instruments code with memory checks to find out-of-bounds
    memory accesses.  Checks are performed by computing bounds for each
    pointer and then comparing address of accessed memory before pointer
    dereferencing.

    1. Instrumentation.

    There are few things to instrument:

    a) Memory accesses - add MPX calls to check address of accessed memory
    against bounds of dereferenced pointer.  Obviously safe memory
    accesses like static variable access does not have to be instrumented
    with checks.

    Example:

      val_2 = *p_1;

      with 4 bytes access is transformed into:

      __builtin___mpx_bndcl (__bound_tmp.1_3, p_1);
      D.1_4 = p_1 + 3;
      __builtin___mpx_bndcu (__bound_tmp.1_3, D.1_4);
      val_2 = *p_1;

      where __bound_tmp.1_3 are bounds computed for pointer p_1,
      __builtin___mpx_bndcl is a lower bound check and
      __builtin___mpx_bndcu is an upper bound check.

    b) Pointer stores.

    When pointer is stored in memory we need to store its bounds.  To
    achieve compatibility of MPX instrumented code with regular codes
    we have to keep data layout and store bounds in special bound tables
    via special mpx call.  Implementation of bounds table may vary for
    different platforms.  It has to associate pointer value and its
    location (it is required because we may have two equal pointers
    with different bounds stored in different places) with bounds.
    Another MPX builtin allows to get bounds for specified pointer
    loaded from specified location.

    Example:

      buf1[i_1] = &buf2;

      is transformed into:

      buf1[i_1] = &buf2;
      D.1_2 = &buf1[i_1];
      __builtin___mpx_bndstx (D.1_2, &buf2, __bound_tmp.1_2);

      where __bound_tmp.1_2 are bounds of &buf2.

    c) Static initialization.

    The special case of pointer store is static pointer initialization.
    Bounds initialization is performed in a few steps:
      - register all static initializations in front-end using
      mpx_register_var_initializer
      - when file compilation finishes we create functions with special
      attribute 'mpx ctor' and put explicit initialization code
      (assignments) for all statically initialized pointers.
      - when MPX constructor is compiled MPX pass adds required bounds
      initialization for all statically initialized pointers
      - since we do not actually need excess pointers initialization
      in MPX constructor we remove such assignments from them

    d) Calls.

    For each call in the code we should add additional arguments to pass
    bounds for pointer arguments.  In call expression each bound argument
    goes right after pointer argument it is added for. We determine type
    of call arguments using arguments list from function declaration; if
    function declaration is not available we use function type; otherwise
    (e.g. for unnamed arguments) we use type of passed value.

    MPX instrumentation assumes binary compatibility with non-instrumented
    codes.  Expand pass is responsible for handling added bounds arguments
    correctly identifying them by type.

    Example:

      val_1 = foo (&buf1, &buf2, &buf1, 0);

      is translated into:

      val_1 = foo (&buf1, __bound_tmp.1_2, &buf2, __bound_tmp.1_3, &buf1, __bound_tmp.1_2, 0);

    e) Returns.

    If function returns a pointer value we have to return bounds also.
    Bounds for returned value are computed and associated with retval in
    MPX structures.  MPX pass exports method mpx_get_registered_bounds to
    obtain bounds associated with decls and ssa names which is used by
    expand pass to obtain returned bounds.

    To have explicit usage of SSA_NAME corresponding to bounds of return
    value we also add new operand to return statement.  We do not actually
    use this operands for expand, but need it to prevent removal of bounds
    creation as dead code by DCE.

    Example:

      return &_buf1;

      is transformed into

      return &_buf1, __bound_tmp.1_1;

    2. Bounds computation.

    Compiler is fully responsible for computing bounds to be used for each
    memory access.  The first step for bounds computation is to find the
    origin of pointer dereferenced for memory access.  Basing on pointer
    origin we define a way to compute its bounds.  There are just few
    possible cases:

    a) Pointer is returned by call.

    In this case we use corresponding MPX builtin method to obtain returned
    bounds.

    Example:

      buf_1 = malloc (size_2);
      foo (buf_1);

      is translated into:

      buf_1 = malloc (size_2);
      __bound_tmp.1_3 = __builtin___mpx_bndret ();
      foo (buf_1, __bound_tmp.1_3);

    b) Pointer is an input argument.

    In this case we use corresponding MPX builtin method to obtatin bounds
    passed for the argument.

    Example:

      foo (int * p)
      {
        <unnamed type> __bound_tmp.3;

	<bb 3>:
	__bound_tmp.3_3 = __builtin___mpx_arg_bnd (p_1(D));

	<bb 2>:
	return p_1(D), __bound_tmp.3_3;
      }

    c) Pointer is an address of an object.

    In this case compiler tries to compute objects size and create corresponding
    bounds.  If object has incomplete type then special MPX builtin is used to
    obtain its size at runtime.

    Example:

      foo ()
      {
        <unnamed type> __bound_tmp.3;
	static int buf[100];

	<bb 3>:
	__bound_tmp.3_2 = __builtin___mpx_bndmk (&buf, 400);

	<bb 2>:
	return &buf, __bound_tmp.3_2;
      }

    Example:

      Address of an object 'extern int buf[]' with incomplete type is
      returned.

      foo ()
      {
        <unnamed type> __bound_tmp.4;
	long unsigned int __size_tmp.3;

	<bb 3>:
	__size_tmp.3_4 = __builtin_ia32_sizeof (buf);
	__bound_tmp.4_3 = __builtin_ia32_bndmk (&buf, __size_tmp.3_4);

	<bb 2>:
	return &buf, __bound_tmp.4_3;
      }

    d) Pointer is the result of object narrowing.

    It happens when we use pointer to an object to compute pointer to a part
    of an object.  E.g. we take pointer to a field of a structure. In this
    case we perform bounds intersection using bounds of original object and
    bounds of object's part (which are computed basing on its type).

    There may be some debatable questions about when narrowing should occur
    and when it should not.  To avoid false bound violations in correct
    programs we do not perform narrowing when address of an array element is
    obtained (it has address of the whole array) and when address of the first
    structure field is obtained (because it is guaranteed to be equal to
    address of the whole structure and it is legal to cast it back to structure).

    Default narrowing behavior may be changed using compiler flags.

    Example:

      In this example address of the second structure field is returned.

      foo (struct A * p)
      {
        <unnamed type> __bound_tmp.3;
	int * _2;
	int * _5;

	<bb 3>:
	__bound_tmp.3_4 = __builtin___mpx_arg_bnd (p_1(D));

	<bb 2>:
	_5 = &p_1(D)->second_field;
	__bound_tmp.3_6 = __builtin___mpx_bndmk (_5, 4);
	__bound_tmp.3_8 = __builtin___mpx_intersect (__bound_tmp.3_6, __bound_tmp.3_4);
	_2 = &p_1(D)->second_field;
	return _2, __bound_tmp.3_8;
      }

    Example:

      In this example address of the first field of array element is returned.

      foo (struct A * p, int i)
      {
        <unnamed type> __bound_tmp.3;
	long unsigned int _2;
	long unsigned int _3;
	struct A * _5;
	int * _6;

	<bb 3>:
	__bound_tmp.3_8 = __builtin___mpx_arg_bnd (p_4(D));

	<bb 2>:
	_2 = (long unsigned int) i_1(D);
	_3 = _2 * 8;
	_5 = p_4(D) + _3;
	_6 = &_5->first_field;
	return _6, __bound_tmp.3_8;
      }


    e) Pointer is the result of pointer arithmetic or type cast.

    In this case bounds of the base pointer are used.

    f) Pointer is loaded from the memory.

    In this case we just need to load bounds from the bounds table.

    Example:

      foo ()
      {
        <unnamed type> __bound_tmp.3;
	static int * buf;
	int * _2;

	<bb 2>:
	_2 = buf;
	__bound_tmp.3_4 = __builtin___mpx_bndldx (&buf, _2);
	return _2, __bound_tmp.3_4;
      }
*/

typedef void (*assign_handler)(tree, tree, void *);

enum check_type
{
  CHECK_LOWER_BOUND,
  CHECK_UPPER_BOUND
};

struct pol_item
{
  tree cst;
  tree var;
};

struct address_t
{
  vec<struct pol_item> pol;
};

/* Structure to hold check informtation.  */
struct check_info
{
  /* Type of the check.  */
  check_type type;
  /* Address used for the check.  */
  address_t addr;
  /* Bounds used for the check.  */
  tree bounds;
  /* Check statement.  Can be NULL for removed checks.*/
  gimple stmt;
};

/* Structure to hold checks information for BB.  */
struct bb_checks
{
  vec<struct check_info, va_heap, vl_ptr> checks;
};

static tree mpx_find_bounds (tree ptr, gimple_stmt_iterator *iter);
static tree mpx_find_bounds_loaded (tree ptr, tree ptr_src,
				   gimple_stmt_iterator *iter);
static tree mpx_find_bounds_abnormal (tree ptr, tree phi);
static void mpx_collect_value (tree ssa_name, address_t &res);
static void mpx_build_bndstx (tree addr, tree ptr, tree bounds,
			      gimple_stmt_iterator *gsi);

#define mpx_bndldx_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_BNDLDX))
#define mpx_bndstx_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_BNDSTX))
#define mpx_checkl_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_BNDCL))
#define mpx_checku_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_BNDCU))
#define mpx_bndmk_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_BNDMK))
#define mpx_ret_bnd_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_BNDRET))
#define mpx_intersect_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_INTERSECT))
#define mpx_narrow_bounds_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_NARROW))
#define mpx_set_bounds_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_SET_PTR_BOUNDS))
#define mpx_arg_bnd_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_ARG_BND))
#define mpx_sizeof_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_SIZEOF))
#define mpx_extract_lower_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_EXTRACT_LOWER))
#define mpx_extract_upper_fndecl (targetm.builtin_mpx_function (BUILT_IN_MPX_EXTRACT_UPPER))

static vec<struct bb_checks, va_heap, vl_ptr> check_infos;

static GTY (()) tree mpx_uintptr_type;

static GTY (()) tree mpx_zero_bounds_var;
static GTY (()) tree mpx_none_bounds_var;
static GTY (()) vec<tree, va_gc> *mpx_static_const_bounds;
static GTY ((param_is (struct tree_map)))
     htab_t mpx_static_var_bounds;
static GTY ((param_is (struct tree_map)))
     htab_t mpx_static_var_bounds_r;

static GTY (()) basic_block entry_block;
static GTY (()) tree zero_bounds;
static GTY (()) tree none_bounds;
static GTY (()) tree incomplete_bounds;
static GTY (()) tree tmp_var;
static GTY (()) tree size_tmp_var;

struct pointer_set_t *mpx_invalid_bounds;
struct pointer_set_t *mpx_completed_bounds_set;
struct pointer_map_t *mpx_reg_bounds;
struct pointer_map_t *mpx_reg_addr_bounds;
struct pointer_map_t *mpx_incomplete_bounds_map;
struct pointer_map_t *mpx_rtx_bounds;

static GTY ((if_marked ("tree_vec_map_marked_p"), param_is (struct tree_vec_map)))
     htab_t mpx_abnormal_phi_copies;
static GTY (()) vec<tree, va_gc> *var_inits;
static GTY ((if_marked ("tree_map_marked_p"),
	     param_is (struct tree_map))) htab_t mpx_size_decls;

#define MPX_BOUND_TMP_NAME "__bound_tmp"
#define MPX_SIZE_TMP_NAME "__size_tmp"
#define MPX_SIZE_OF_SYMBOL_PREFIX "__mpx_size_of_"
#define MPX_BOUNDS_OF_SYMBOL_PREFIX "__mpx_bounds_of_"
#define MPX_STRING_BOUNDS_PREFIX "__mpx_string_bounds_"
#define MPX_VAR_BOUNDS_PREFIX "__mpx_var_bounds_"
#define MPX_ZERO_BOUNDS_VAR_NAME "__mpx_zero_bounds"
#define MPX_NONE_BOUNDS_VAR_NAME "__mpx_none_bounds"

/* Static MPX constructors may become very large and their
   compilation with optimization may take too much time.
   Therefore we put a limit to number of statements in one
   MPX construcor.  Tests with 100 000 statically initialized
   pointers showed following compilation times on Sandy Bridge
   server (used -O2):
   limit    100 => ~18 sec.
   limit    300 => ~22 sec.
   limit   1000 => ~30 sec.
   limit   3000 => ~49 sec.
   limit   5000 => ~55 sec.
   limit  10000 => ~76 sec.
   limit 100000 => ~532 sec.  */
#define MAX_STMTS_IN_STATIC_MPX_CTOR (optimize > 0 ? 5000 : 100000)

struct mpx_ctor_stmt_list
{
  tree stmts;
  int avail;
};

/* Mark statement S to not be instrumented.  */
static void
mpx_mark_stmt (gimple s)
{
  gimple_set_plf (s, GF_PLF_1, true);
}

/* Mark statement S to be instrumented.  */
static void
mpx_unmark_stmt (gimple s)
{
  gimple_set_plf (s, GF_PLF_1, false);
}

/* Return 1 if statement S should not be instrumented.  */
static bool
mpx_marked_stmt_p (gimple s)
{
  return gimple_plf (s, GF_PLF_1);
}

/* Get var to be used for bound temps.  */
static tree
mpx_get_tmp_var (void)
{
  if (!tmp_var)
    tmp_var = create_tmp_reg (bound_type_node, MPX_BOUND_TMP_NAME);

  return tmp_var;
}

/* Get var to be used for size temps.  */
static tree
mpx_get_size_tmp_var (void)
{
  if (!size_tmp_var)
    size_tmp_var = create_tmp_reg (mpx_uintptr_type, MPX_SIZE_TMP_NAME);

  return size_tmp_var;
}

/* Register bounds BND for address of OBJ.  */
static void
mpx_register_addr_bounds (tree obj, tree bnd)
{
  tree *slot;

  if (bnd == incomplete_bounds)
    return;

  slot = (tree *)pointer_map_insert (mpx_reg_addr_bounds, obj);
  *slot = bnd;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Regsitered bound ");
      print_generic_expr (dump_file, bnd, 0);
      fprintf (dump_file, " for address of ");
      print_generic_expr (dump_file, obj, 0);
      fprintf (dump_file, "\n");
    }
}

/* Return bounds registered for address of OBJ.  */
static tree
mpx_get_registered_addr_bounds (tree obj)
{
  tree *slot = (tree *)pointer_map_contains (mpx_reg_addr_bounds, obj);
  return slot ? *slot : NULL_TREE;
}

/* Mark BOUNDS as completed.  */
static void
mpx_mark_completed_bounds (tree bounds)
{
  pointer_set_insert (mpx_completed_bounds_set, bounds);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Marked bounds ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, " as completed\n");
    }
}

/* Return 1 if BOUNDS were marked as completed and 0 otherwise.  */
static bool
mpx_completed_bounds (tree bounds)
{
  return pointer_set_contains (mpx_completed_bounds_set, bounds);
}

/* Clear comleted bound marks.  */
static void
mpx_erase_completed_bounds (void)
{
  pointer_set_destroy (mpx_completed_bounds_set);
  mpx_completed_bounds_set = pointer_set_create ();
}

/* Mark BOUNDS associated with PTR as incomplete.  */
static void
mpx_register_incomplete_bounds (tree bounds, tree ptr)
{
  tree *slot = (tree *)pointer_map_insert (mpx_incomplete_bounds_map, bounds);
  *slot = ptr;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Regsitered incomplete bounds ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, " for ");
      print_generic_expr (dump_file, ptr, 0);
      fprintf (dump_file, "\n");
    }
}

/* Return 1 if BOUNDS are incomplete and 0 otherwise.  */
static bool
mpx_incomplete_bounds (tree bounds)
{
  void **slot;

  if (bounds == incomplete_bounds)
    return true;

  if (mpx_completed_bounds (bounds))
    return false;

  slot = pointer_map_contains (mpx_incomplete_bounds_map, bounds);

  return slot != NULL;
}

/* Clear incomleted bound marks.  */
static void
mpx_erase_incomplete_bounds (void)
{
  pointer_map_destroy (mpx_incomplete_bounds_map);
  mpx_incomplete_bounds_map = pointer_map_create ();
}

/* Split SLOT identifying slot for function value or
   argument into two parts SLOT_VAL and SLOT_BND.
   First is the slot for regular value and the other one is
   for bounds.  */
void
mpx_split_slot (rtx slot, rtx *slot_val, rtx *slot_bnd)
{
  int i;
  int val_num = 0;
  int bnd_num = 0;
  rtx *val_tmps;
  rtx *bnd_tmps;

  *slot_bnd = 0;

  if (!slot
      || GET_CODE (slot) != PARALLEL)
    {
      *slot_val = slot;
      return;
    }

  val_tmps = XALLOCAVEC (rtx, XVECLEN (slot, 0));
  bnd_tmps = XALLOCAVEC (rtx, XVECLEN (slot, 0));

  for (i = 0; i < XVECLEN (slot, 0); i++)
    {
      rtx elem = XVECEXP (slot, 0, i);
      rtx reg = GET_CODE (elem) == EXPR_LIST ? XEXP (elem, 0) : elem;

      if (!reg)
	continue;

      if (BOUND_MODE_P (GET_MODE (reg)) || CONST_INT_P (reg))
	bnd_tmps[bnd_num++] = elem;
      else
	val_tmps[val_num++] = elem;
    }

  gcc_assert (val_num);

  if (!bnd_num)
    {
      *slot_val = slot;
      return;
    }

  if ((GET_CODE (val_tmps[0]) == EXPR_LIST) || (val_num > 1))
    *slot_val = gen_rtx_PARALLEL (GET_MODE (slot),
				  gen_rtvec_v (val_num, val_tmps));
  else
    *slot_val = val_tmps[0];

  if ((GET_CODE (bnd_tmps[0]) == EXPR_LIST) || (bnd_num > 1))
    *slot_bnd = gen_rtx_PARALLEL (VOIDmode,
				  gen_rtvec_v (bnd_num, bnd_tmps));
  else
    *slot_bnd = bnd_tmps[0];
}

/* Join previously splitted to VAL and BND rtx for function
   value or argument and return it.  */
rtx
mpx_join_splitted_slot (rtx val, rtx bnd)
{
  rtx res;
  int i, n = 0;

  if (!bnd)
    return val;

  if (GET_CODE (val) == PARALLEL)
    n += XVECLEN (val, 0);
  else
    n++;

  if (GET_CODE (bnd) == PARALLEL)
    n += XVECLEN (bnd, 0);
  else
    n++;

  res = gen_rtx_PARALLEL (GET_MODE (val), rtvec_alloc (n));

  n = 0;

  if (GET_CODE (val) == PARALLEL)
    for (i = 0; i < XVECLEN (val, 0); i++)
      XVECEXP (res, 0, n++) = XVECEXP (val, 0, i);
  else
    XVECEXP (res, 0, n++) = val;

  if (GET_CODE (bnd) == PARALLEL)
    for (i = 0; i < XVECLEN (bnd, 0); i++)
      XVECEXP (res, 0, n++) = XVECEXP (bnd, 0, i);
  else
    XVECEXP (res, 0, n++) = bnd;

  return res;
}

/* If PAR is PARALLEL holding registers then transform
   it into PARALLEL holding EXPR_LISTs of those regs
   and zero constant (similar to how function value
   on multiple registers looks like).  */
void
mpx_put_regs_to_expr_list (rtx par)
{
  int n;

  if (GET_CODE (par) != PARALLEL
      || GET_CODE (XVECEXP (par, 0, 0)) == EXPR_LIST)
    return;

  for (n = 0; n < XVECLEN (par, 0); n++)
    XVECEXP (par, 0, n) = gen_rtx_EXPR_LIST (VOIDmode,
					     XVECEXP (par, 0, n),
					     const0_rtx);
}

/* Build and return bndmk call which creates bounds for structure
   pointed by PTR.  Structure should have complete type.  */
tree
mpx_make_bounds_for_struct_addr (tree ptr)
{
  tree type = TREE_TYPE (ptr);
  tree size;

  gcc_assert (POINTER_TYPE_P (type));

  size = TYPE_SIZE (TREE_TYPE (type));

  gcc_assert (size);

  return build_call_nary (bound_type_node,
			  build_fold_addr_expr (mpx_bndmk_fndecl),
			  2, ptr, size);
}

/* Emit instructions to store BOUNDS for pointer VALUE
   stored in MEM.
   Function is used by expand to pass bounds for args
   passed on stack.  */
void
mpx_emit_bounds_store (rtx bounds, rtx value, rtx mem)
{
  gcc_assert (MEM_P (mem));

  if (REG_P (bounds) || CONST_INT_P (bounds))
    {
      rtx ptr;

      if (REG_P (value))
	ptr = value;
      else
	{
	  rtx slot = adjust_address (value, Pmode, 0);
	  ptr = gen_reg_rtx (Pmode);
	  emit_move_insn (ptr, slot);
	}

      if (CONST_INT_P (bounds))
	bounds = targetm.calls.load_bounds_for_arg (value, ptr, bounds);

      targetm.calls.store_bounds_for_arg (ptr, mem,
					  bounds, NULL);
    }
  else
    {
      int i;

      gcc_assert (GET_CODE (bounds) == PARALLEL);
      gcc_assert (GET_CODE (value) == PARALLEL || MEM_P (value));

      for (i = 0; i < XVECLEN (bounds, 0); i++)
	{
	  rtx reg = XEXP (XVECEXP (bounds, 0, i), 0);
	  rtx offs = XEXP (XVECEXP (bounds, 0, i), 1);
	  rtx slot = adjust_address (mem, Pmode, INTVAL (offs));
	  rtx ptr;

	  if (GET_CODE (value) == PARALLEL)
	    ptr = mpx_get_value_with_offs (value, offs);
	  else
	    {
	      rtx tmp = adjust_address (value, Pmode, INTVAL (offs));
	      ptr = gen_reg_rtx (Pmode);
	      emit_move_insn (ptr, tmp);
	    }

	  targetm.calls.store_bounds_for_arg (ptr, slot, reg, NULL);
	}
    }
}

/* Traversal function for mpx_may_finish_incomplete_bounds.
   Set RES to 0 if at least one argument of phi statement
   defining bounds (passed in KEY arg) is unknown.
   Traversal stops when first unknown phi argument is found.  */
static bool
mpx_may_complete_phi_bounds (const void *key, void **slot ATTRIBUTE_UNUSED,
			     void *res)
{
  const_tree bounds = (const_tree)key;
  gimple phi;
  unsigned i;

  gcc_assert (TREE_CODE (bounds) == SSA_NAME);

  phi = SSA_NAME_DEF_STMT (bounds);

  gcc_assert (phi && gimple_code (phi) == GIMPLE_PHI);

  for (i = 0; i < gimple_phi_num_args (phi); i++)
    {
      tree phi_arg = gimple_phi_arg_def (phi, i);
      if (!phi_arg)
	{
	  *((bool *)res) = false;
	  /* Do not need to traverse further.  */
	  return false;
	}
    }

  return true;
}

/* Return 1 if all phi nodes created for bounds have their
   arguments computed.  */
static bool
mpx_may_finish_incomplete_bounds (void)
{
  bool res = true;

  pointer_map_traverse (mpx_incomplete_bounds_map,
			mpx_may_complete_phi_bounds,
			&res);

  return res;
}

/* Helper function for mpx_finish_incomplete_bounds.
   Recompute args for bounds phi node.  */
static bool
mpx_recompute_phi_bounds (const void *key, void **slot, void *res ATTRIBUTE_UNUSED)
{
  tree bounds = const_cast<tree> ((const_tree)key);
  tree ptr = *(tree *)slot;
  gimple bounds_phi;
  gimple ptr_phi;
  unsigned i;

  gcc_assert (TREE_CODE (bounds) == SSA_NAME);
  gcc_assert (TREE_CODE (ptr) == SSA_NAME);

  bounds_phi = SSA_NAME_DEF_STMT (bounds);
  ptr_phi = SSA_NAME_DEF_STMT (ptr);

  gcc_assert (bounds_phi && gimple_code (bounds_phi) == GIMPLE_PHI);
  gcc_assert (ptr_phi && gimple_code (ptr_phi) == GIMPLE_PHI);

  for (i = 0; i < gimple_phi_num_args (bounds_phi); i++)
    {
      tree ptr_arg = gimple_phi_arg_def (ptr_phi, i);
      edge e = gimple_phi_arg_edge (ptr_phi, i);
      tree bound_arg;

      /* This bounds computation is final for the PHI node.
	 We have to create bound copies for abnormal edges
	 to avoid problem in SSA names coalescing.  */
      if (e->flags & EDGE_ABNORMAL)
	bound_arg = mpx_find_bounds_abnormal (ptr_arg, bounds);
      else
	bound_arg = mpx_find_bounds (ptr_arg, NULL);

      add_phi_arg (bounds_phi, bound_arg,
		   gimple_phi_arg_edge (ptr_phi, i),
		   UNKNOWN_LOCATION);
    }

  return true;
}

/* Mark BOUNDS as invalid.  */
static void
mpx_mark_invalid_bounds (tree bounds)
{
  pointer_set_insert (mpx_invalid_bounds, bounds);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Marked bounds ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, " as invalid\n");
    }
}

/* Return 1 if BOUNDS were marked as invalid and 0 otherwise.  */
static bool
mpx_valid_bounds (tree bounds)
{
  if (bounds == zero_bounds || bounds == none_bounds)
    return false;

  return !pointer_set_contains (mpx_invalid_bounds, bounds);
}

/* Helper function for mpx_finish_incomplete_bounds.
   Check all arguments of phi nodes trying to find
   valid completed bounds.  If there is at least one
   such arg then bounds produced by phi node are marked
   as valid completed bounds and all phi args are
   recomputed.  */
static bool
mpx_find_valid_phi_bounds (const void *key, void **slot, void *res)
{
  tree bounds = const_cast<tree> ((const_tree)key);
  gimple phi;
  unsigned i;

  gcc_assert (TREE_CODE (bounds) == SSA_NAME);

  if (mpx_completed_bounds (bounds))
    return true;

  phi = SSA_NAME_DEF_STMT (bounds);

  gcc_assert (phi && gimple_code (phi) == GIMPLE_PHI);

  for (i = 0; i < gimple_phi_num_args (phi); i++)
    {
      tree phi_arg = gimple_phi_arg_def (phi, i);

      gcc_assert (phi_arg);

      if (mpx_valid_bounds (phi_arg) && !mpx_incomplete_bounds (phi_arg))
	{
	  *((bool *)res) = true;
	  mpx_mark_completed_bounds (bounds);
	  mpx_recompute_phi_bounds (key, slot, NULL);
	  return true;
	}
    }

  return true;
}

/* Helper function for mpx_finish_incomplete_bounds.
   Marks all incompleted bounds as invalid.  */
static bool
mpx_mark_invalid_bounds_walker (const void *key, void **slot ATTRIBUTE_UNUSED,
				void *res ATTRIBUTE_UNUSED)
{
  tree bounds = const_cast<tree> ((const_tree)key);

  if (!mpx_completed_bounds (bounds))
    {
      mpx_mark_invalid_bounds (bounds);
      mpx_mark_completed_bounds (bounds);
    }
  return true;
}

/* When all bound phi nodes have all their args computed
   we have enough info to find valid bounds.  We iterate
   through all incompleted bounds searching for valid
   bounds.  Found valid bounds are marked as completed
   and all remaining incompleted bounds are recomputed.
   Process continues until no new valid bounds may be
   found.  All remained incompleted bounds are marked as
   invalid (i.e. have no valid source of bounds).  */
static void
mpx_finish_incomplete_bounds (void)
{
  bool found_valid;

  while (found_valid)
    {
      found_valid = false;

      pointer_map_traverse (mpx_incomplete_bounds_map,
			    mpx_find_valid_phi_bounds,
			    &found_valid);

      if (found_valid)
	pointer_map_traverse (mpx_incomplete_bounds_map,
			      mpx_recompute_phi_bounds,
			      NULL);
    }

  pointer_map_traverse (mpx_incomplete_bounds_map,
			mpx_mark_invalid_bounds_walker,
			NULL);
  pointer_map_traverse (mpx_incomplete_bounds_map,
			mpx_recompute_phi_bounds,
			&found_valid);

  mpx_erase_completed_bounds ();
  mpx_erase_incomplete_bounds ();
}

/* Return 1 if type TYPE is a pointer type or a
   structure having a pointer type as one of its fields.
   Otherwise return 0.  */
bool
mpx_type_has_pointer (tree type)
{
  bool res = false;

  if (BOUNDED_TYPE_P (type))
    res = true;
  else if (RECORD_OR_UNION_TYPE_P (type))
    {
      tree field;

      for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
	if (TREE_CODE (field) == FIELD_DECL)
	  res = res || mpx_type_has_pointer (TREE_TYPE (field));
    }
  else if (TREE_CODE (type) == ARRAY_TYPE)
    res = mpx_type_has_pointer (TREE_TYPE (type));

  return res;
}

/* Get bounds rtx associated with NODE via
   mpx_set_rtl_bounds call.  */
rtx
mpx_get_rtl_bounds (tree node)
{
  rtx *slot;

  if (!mpx_rtx_bounds)
    return NULL_RTX;

  slot = (rtx *)pointer_map_contains (mpx_rtx_bounds, node);
  return slot ? *slot : NULL_RTX;
}

/* Associate bounds rtx VAL with NODE.  */
void
mpx_set_rtl_bounds (tree node, rtx val)
{
  rtx *slot;

  if (!mpx_rtx_bounds)
    return;

  slot = (rtx *)pointer_map_insert (mpx_rtx_bounds, node);
  *slot = val;
}

/* Check if statically initialized variable VAR require
   static bounds initilization.  If VAR is added into
   bounds initlization list then 1 is returned. Otherwise
   return 0.  */
extern bool
mpx_register_var_initializer (tree var)
{
  if (!flag_mpx)
    return false;

  gcc_assert (TREE_CODE (var) == VAR_DECL);
  gcc_assert (DECL_INITIAL (var)
	      && DECL_INITIAL (var) != error_mark_node);

  if (TREE_STATIC (var)
      && mpx_type_has_pointer (TREE_TYPE (var)))
    {
      vec_safe_push (var_inits, var);
      return true;
    }

  return false;
}

/* Helper function for mpx_finish_file.

   Add new modification statement (RHS is assigned to LHS)
   into list of static initilizer statementes (passed in ARG).
   If statements list becomes too big, emit MPX contructor
   and start the new one.  */
static void
mpx_add_modification_to_statements_list (tree lhs,
					 tree rhs,
					 void *arg)
{
  struct mpx_ctor_stmt_list *stmts = (struct mpx_ctor_stmt_list *)arg;
  tree modify;

  if (!useless_type_conversion_p (TREE_TYPE (lhs), TREE_TYPE (rhs)))
    rhs = build1 (CONVERT_EXPR, TREE_TYPE (lhs), rhs);

  modify = build2 (MODIFY_EXPR, TREE_TYPE (lhs), lhs, rhs);
  append_to_statement_list (modify, &stmts->stmts);

  stmts->avail--;

  if (!stmts->avail)
    {
      cgraph_build_static_cdtor ('P', stmts->stmts,
				 MAX_RESERVED_INIT_PRIORITY + 2);
      stmts->avail = MAX_STMTS_IN_STATIC_MPX_CTOR;
      stmts->stmts = NULL;
    }
}

/* Build and return ADDR_EXPR for specified object OBJ.  */
static tree
mpx_build_addr_expr (tree obj)
{
  return TREE_CODE (obj) == TARGET_MEM_REF
    ? tree_mem_ref_addr (ptr_type_node, obj)
    : build_fold_addr_expr (obj);
}

/* Helper function for mpx_finish_file.
   Output bound variable VAR and also add its initialization code
   with bounds of variable BND_VAR to statements list STMTS.
   If statements list becomes too big, emit MPX contructor
   and start the new one.  */
static void
mpx_output_static_bounds (tree var, tree bnd_var,
			  struct mpx_ctor_stmt_list *stmts)
{
  tree size_ptr = build_pointer_type (size_type_node);
  tree bnd_p = build1 (CONVERT_EXPR, size_ptr,
		       mpx_build_addr_expr (bnd_var));
  tree lb, ub, lhs, modify, size;

  assemble_variable (bnd_var, 1, 0, 0);

  if (TREE_CODE (var) == STRING_CST)
    {
      lb = build1 (CONVERT_EXPR, size_type_node, mpx_build_addr_expr (var));
      size = build_int_cst (size_type_node, TREE_STRING_LENGTH (var) - 1);
    }
  else if (DECL_SIZE (var)
	   && !mpx_variable_size_type (TREE_TYPE (var)))
    {
      /* Compute bounds using statically known size.  */
      lb = build1 (CONVERT_EXPR, size_type_node, mpx_build_addr_expr (var));
      size = size_binop (MINUS_EXPR, DECL_SIZE_UNIT (var), size_one_node);
    }
  else
    {
      /* Compute bounds using size relocation.  */
      tree call;

      lb = build1 (CONVERT_EXPR, size_type_node, mpx_build_addr_expr (var));
      call = build1 (ADDR_EXPR,
		     build_pointer_type (TREE_TYPE (mpx_sizeof_fndecl)),
		     mpx_sizeof_fndecl);
      size = build_call_nary (TREE_TYPE (TREE_TYPE (mpx_sizeof_fndecl)),
			      call, 1, var);

      if (flag_mpx_zero_size_reloc_as_infinite)
	{
	  tree max_size, cond;

	  max_size = build2 (MINUS_EXPR, size_type_node, size_zero_node, lb);
	  cond = build2 (NE_EXPR, boolean_type_node, size, size_zero_node);
	  size = build3 (COND_EXPR, size_type_node, cond, size, max_size);
	}

      size = size_binop (MINUS_EXPR, size, size_one_node);
    }

  ub = size_binop (PLUS_EXPR, lb, size);
  ub = build1 (BIT_NOT_EXPR, size_type_node, ub);

  lhs = build1 (INDIRECT_REF, size_type_node,
		build2 (POINTER_PLUS_EXPR, size_ptr, bnd_p,
			TYPE_SIZE_UNIT (size_type_node)));
  modify = build2 (MODIFY_EXPR, TREE_TYPE (lhs), lhs, ub);
  append_to_statement_list (modify, &stmts->stmts);
  stmts->avail--;

  if (stmts->avail <= 0)
    {
      cgraph_build_static_cdtor ('B', stmts->stmts,
				 MAX_RESERVED_INIT_PRIORITY + 1);
      stmts->avail = MAX_STMTS_IN_STATIC_MPX_CTOR;
      stmts->stmts = NULL;
    }
}

/* Helper function for mpx_finish_file.
   Output one size variable.  */
static int
mpx_output_size_variable (void **slot, void *res ATTRIBUTE_UNUSED)
{
  struct tree_map *map = (struct tree_map *)*slot;
  tree size_decl = map->to;
  assemble_variable (size_decl, 1, 0, 0);

  return 1;
}

/* Helper function for mpx_finish_file to sort vars.  */
static int
mpx_compare_var_names (const void *i1, const void *i2)
{
  const tree t1 = *(const tree *)i1;
  const tree t2 = *(const tree *)i2;
  const char *name1;
  const char *name2;

  if (TREE_CODE (t1) == STRING_CST)
    {
      if (TREE_CODE (t2) != STRING_CST)
	return 1;

      name1 = TREE_STRING_POINTER (t1);
      name2 = TREE_STRING_POINTER (t2);
    }
  else
    {
      if (TREE_CODE (t2) == STRING_CST)
	return -1;

      name1 = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (t1));
      name2 = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (t2));
    }

  return strcmp (name1, name2);
}

/* Helper function for mpx_finish_file to put all
   vars into vectors.  */
static int
mpx_add_tree_to_vec (void **slot, void *res)
{
  struct tree_map *map = (struct tree_map *)*slot;
  vec<tree> *vars = (vec<tree> *)res;
  tree var = map->base.from;
  vars->safe_push (var);

  return 1;
}

/* Register bounds BND for object PTR in global bounds table.  */
static void
mpx_register_bounds (tree ptr, tree bnd)
{
  tree *slot;

  /* Do nothing if bounds are incomplete_bounds
     because it means bounds will be recomputed.  */
  if (bnd == incomplete_bounds)
    return;

  slot = (tree *)pointer_map_insert (mpx_reg_bounds, ptr);
  *slot = bnd;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Regsitered bound ");
      print_generic_expr (dump_file, bnd, 0);
      fprintf (dump_file, " for pointer ");
      print_generic_expr (dump_file, ptr, 0);
      fprintf (dump_file, "\n");
    }
}

/* Get bounds registered for object PTR in global bounds table.  */
tree
mpx_get_registered_bounds (tree ptr)
{
  tree *slot = (tree *)pointer_map_contains (mpx_reg_bounds, ptr);
  return slot ? *slot : NULL_TREE;
}

/* Add bound retvals to return statement pointed by GSI.  */

static void
mpx_add_bounds_to_ret_stmt (gimple_stmt_iterator *gsi)
{
  gimple ret = gsi_stmt (*gsi);
  tree retval = gimple_return_retval (ret);
  tree ret_decl = DECL_RESULT (cfun->decl);
  tree bounds;

  if (!retval)
    return;

  if (BOUNDED_P (ret_decl))
    {
      bounds = mpx_find_bounds (retval, gsi);
      mpx_register_bounds (ret_decl, bounds);
      gimple_return_set_retbnd (ret, bounds);
    }

  update_stmt (ret);
}

/* Force OP to be suitable for using as an argument for call.
   New statements (if any) go to SEQ.  */
static tree
mpx_force_gimple_call_op (tree op, gimple_seq *seq)
{
  gimple_seq stmts;
  gimple_stmt_iterator si;

  op = force_gimple_operand (unshare_expr (op), &stmts, true, NULL_TREE);

  for (si = gsi_start (stmts); !gsi_end_p (si); gsi_next (&si))
    mpx_mark_stmt (gsi_stmt (si));

  gimple_seq_add_seq (seq, stmts);

  return op;
}

/* Generate lower bound check for memory access by ADDR. 
   Check is inserted before the position pointed by ITER.
   DIRFLAG indicates whether memory access is load or store.  */
static void
mpx_check_lower (tree addr, tree bounds,
		 gimple_stmt_iterator iter,
		 location_t location ATTRIBUTE_UNUSED,
		 tree dirflag)
{
  gimple_seq seq;
  gimple check;
  tree node;

  if (bounds == mpx_get_zero_bounds ())
    return;

  if (dirflag == integer_zero_node
      && !flag_mpx_check_read)
    return;

  if (dirflag == integer_one_node
      && !flag_mpx_check_write)
    return;

  seq = NULL;

  node = mpx_force_gimple_call_op (addr, &seq);

  check = gimple_build_call (mpx_checkl_fndecl, 2, bounds, node);
  mpx_mark_stmt (check);
  gimple_seq_add_stmt (&seq, check);

  gsi_insert_seq_before (&iter, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      gimple before = gsi_stmt (iter);
      fprintf (dump_file, "Generated lower bound check for statement ");
      print_gimple_stmt (dump_file, before, 0, TDF_VOPS|TDF_MEMSYMS);
      fprintf (dump_file, "  ");
      print_gimple_stmt (dump_file, check, 0, TDF_VOPS|TDF_MEMSYMS);
    }
}

/* Generate upper bound check for memory access by ADDR.
   Check is inserted before the position pointed by ITER.
   DIRFLAG indicates whether memory access is load or store.  */
static void
mpx_check_upper (tree addr, tree bounds,
		 gimple_stmt_iterator iter,
		 location_t location ATTRIBUTE_UNUSED,
		 tree dirflag)
{
  gimple_seq seq;
  gimple check;
  tree node;

  if (bounds == mpx_get_zero_bounds ())
    return;

  if (dirflag == integer_zero_node
      && !flag_mpx_check_read)
    return;

  if (dirflag == integer_one_node
      && !flag_mpx_check_write)
    return;

  seq = NULL;

  node = mpx_force_gimple_call_op (addr, &seq);

  check = gimple_build_call (mpx_checku_fndecl, 2, bounds, node);
  mpx_mark_stmt (check);
  gimple_seq_add_stmt (&seq, check);

  gsi_insert_seq_before (&iter, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      gimple before = gsi_stmt (iter);
      fprintf (dump_file, "Generated upper bound check for statement ");
      print_gimple_stmt (dump_file, before, 0, TDF_VOPS|TDF_MEMSYMS);
      fprintf (dump_file, "  ");
      print_gimple_stmt (dump_file, check, 0, TDF_VOPS|TDF_MEMSYMS);
    }
}

/* Generate lower and upper bound checks for memory access
   to memory slot [FIRST, LAST] againsr BOUNDS.  Checks
   are inserted before the position pointed by ITER.
   DIRFLAG indicates whether memory access is load or store.  */
static void
mpx_check_mem_access (tree first, tree last, tree bounds,
		      gimple_stmt_iterator iter,
		      location_t location,
		      tree dirflag)
{
  mpx_check_lower (first, bounds, iter, location, dirflag);
  mpx_check_upper (last, bounds, iter, location, dirflag);
}

/* Replace call to _bnd_chk_* pointed by GSI with
   bndcu and bndcl calls.  DIRFLAG determines whether
   check is for read or write.  */

void
mpx_replace_address_check_builtin (gimple_stmt_iterator *gsi,
				   tree dirflag)
{
  gimple_stmt_iterator call_iter = *gsi;
  gimple call = gsi_stmt (*gsi);
  tree fndecl = gimple_call_fndecl (call);
  tree addr = gimple_call_arg (call, 0);
  tree bounds = mpx_find_bounds (addr, gsi);

  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_CHECK_PTR_LBOUNDS
      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_CHECK_PTR_BOUNDS)
    mpx_check_lower (addr, bounds, *gsi, gimple_location (call), dirflag);

  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_CHECK_PTR_UBOUNDS)
    mpx_check_upper (addr, bounds, *gsi, gimple_location (call), dirflag);

  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_CHECK_PTR_BOUNDS)
    {
      tree size = gimple_call_arg (call, 1);
      addr = fold_build_pointer_plus (addr, size);
      addr = fold_build_pointer_plus_hwi (addr, -1);
      mpx_check_upper (addr, bounds, *gsi, gimple_location (call), dirflag);
    }

  gsi_remove (&call_iter, true);
}

/* Replace call to _bnd_get_ptr_* pointed by GSI with
   corresponding bounds extract call.  */

void
mpx_replace_extract_builtin (gimple_stmt_iterator *gsi)
{
  gimple call = gsi_stmt (*gsi);
  tree fndecl = gimple_call_fndecl (call);
  tree addr = gimple_call_arg (call, 0);
  tree bounds = mpx_find_bounds (addr, gsi);
  gimple extract;

  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_GET_PTR_LBOUND)
    fndecl = mpx_extract_lower_fndecl;
  else if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_GET_PTR_UBOUND)
    fndecl = mpx_extract_upper_fndecl;
  else
    gcc_unreachable ();

  extract = gimple_build_call (fndecl, 1, bounds);
  gimple_call_set_lhs (extract, gimple_call_lhs (call));
  mpx_mark_stmt (extract);

  gsi_replace (gsi, extract, false);
}

/* Add bound arguments to call statement pointed by GSI.
   Also performs a replacement of user MPX builtins calls
   with internal ones.  */

static void
mpx_add_bounds_to_call_stmt (gimple_stmt_iterator *gsi)
{
  gimple call = gsi_stmt (*gsi);
  unsigned arg_no = 0;
  unsigned new_arg_no = 0;
  unsigned bnd_arg_cnt = 0;
  unsigned arg_cnt = 0;
  tree fndecl = gimple_call_fndecl (call);
  tree fntype = TREE_TYPE (TREE_TYPE (gimple_call_fn (call)));
  tree first_formal_arg;
  tree arg;
  gimple new_call;
  ssa_op_iter iter;
  tree op;
  bool use_fntype = false;

  /* Do nothing if back-end builtin is called.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_MD)
    return;

  /* Ignore MPX_INIT_PTR_BOUNDS and MPX_COPY_PTR_BOUNDS.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_INIT_PTR_BOUNDS
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_COPY_PTR_BOUNDS))
    return;

  /* Check user builtins are replaced with checks.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_CHECK_PTR_LBOUNDS
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_CHECK_PTR_UBOUNDS
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_CHECK_PTR_BOUNDS))
    {
      mpx_replace_address_check_builtin (gsi, integer_minus_one_node);
      return;
    }

  /* Check user builtins are replaced with checks.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_GET_PTR_LBOUND
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_GET_PTR_UBOUND))
    {
      mpx_replace_extract_builtin (gsi);
      return;
    }

  /* BUILT_IN_MPX_SET_PTR_BOUNDS call does not require bound args.
     Just replace fndecl with target specific one.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_SET_PTR_BOUNDS)
    {
      gimple_call_set_fndecl (call, mpx_set_bounds_fndecl);
      return;
    }

  /* BUILT_IN_MPX_NARROW_PTR_BOUNDS call is replaced with
     target narrow bounds call.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_NARROW_PTR_BOUNDS)
    {
      tree arg = gimple_call_arg (call, 1);
      tree bounds = mpx_find_bounds (arg, gsi);

      gimple_call_set_fndecl (call, mpx_narrow_bounds_fndecl);
      gimple_call_set_arg (call, 1, bounds);
      update_stmt (call);

      return;
    }

  /* BUILT_IN_MPX_STORE_PTR_BOUNDS call is replaced with
     bndstx call.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_STORE_PTR_BOUNDS)
    {
      tree addr = gimple_call_arg (call, 0);
      tree ptr = gimple_call_arg (call, 1);
      tree bounds = mpx_find_bounds (ptr, gsi);
      gimple_stmt_iterator iter = gsi_for_stmt (call);

      mpx_build_bndstx (addr, ptr, bounds, gsi);
      gsi_remove (&iter, true);

      return;
    }

  /* If function decl is available then use it for
     formal arguments list.  Otherwise use function type.  */
  if (fndecl && DECL_ARGUMENTS (fndecl))
    first_formal_arg = DECL_ARGUMENTS (fndecl);
  else
    {
      first_formal_arg = TYPE_ARG_TYPES (fntype);
      use_fntype = true;
    }

  /* Get number of arguments and bound arguments.  */
  for (arg = first_formal_arg;
       arg && (!use_fntype || TREE_VALUE (arg) != void_type_node)
	 && (arg_cnt - bnd_arg_cnt)< gimple_call_num_args (call);
       arg = TREE_CHAIN (arg))
    {
      tree type = use_fntype ? TREE_VALUE (arg) : TREE_TYPE (arg);

      if (BOUNDED_TYPE_P (type)
	  || (use_fntype
	      && pass_by_reference (NULL, TYPE_MODE (type), type, false)))
	{
	  bnd_arg_cnt++;
	  arg_cnt++;
	}

      arg_cnt++;
    }

  /* Now add number of additional bound arguments for
     stdarg functions.  */
  for (arg_no = arg_cnt - bnd_arg_cnt;
       arg_no < gimple_call_num_args (call);
       arg_no++)
    {
      if (BOUNDED_P (gimple_call_arg (call, arg_no)))
	{
	  arg_cnt++;
	  bnd_arg_cnt++;
	}
      arg_cnt++;
    }

  /* Create new call statement with additional arguments.  */
  new_call = gimple_alloc (GIMPLE_CALL, arg_cnt + 3);
  memcpy (new_call, call, sizeof (struct gimple_statement_call));
  gimple_set_num_ops (new_call, arg_cnt + 3);
  gimple_set_op (new_call, 0, gimple_op (call, 0));
  if (fndecl)
    {
      gimple_set_op (new_call, 1, mpx_build_addr_expr (fndecl));
      gimple_call_set_fntype (new_call, TREE_TYPE (fndecl));
    }
  else
    gimple_set_op (new_call, 1, gimple_op (call, 1));
  gimple_set_op (new_call, 2, gimple_op (call, 2));

  /* Add bounds for all arguments listed in formal arguments list.  */
  arg_no = 0;
  for (arg = first_formal_arg;
       arg && (!use_fntype || TREE_VALUE (arg) != void_type_node)
	 && arg_no < gimple_call_num_args (call);
       arg = TREE_CHAIN (arg))
    {
      tree type = use_fntype ? TREE_VALUE (arg) : TREE_TYPE (arg);
      tree call_arg = gimple_call_arg (call, arg_no++);

      gimple_call_set_arg (new_call, new_arg_no++, call_arg);

      if ((BOUNDED_TYPE_P (type)
	   || (use_fntype
	       && pass_by_reference (NULL, TYPE_MODE (type), type, false)))
	  && bnd_arg_cnt)
	{
	  tree bounds = mpx_find_bounds (call_arg, gsi);
	  gimple_call_set_arg (new_call, new_arg_no++, bounds);
	  bnd_arg_cnt--;
	}
    }

  /* Add bounds for all other arguments.  */
  for ( ; arg_no < gimple_call_num_args (call); arg_no++)
    {
      tree call_arg = gimple_call_arg (call, arg_no);
      gimple_call_set_arg (new_call, new_arg_no++, call_arg);
      if (BOUNDED_P (call_arg) && bnd_arg_cnt)
	{
	  tree bounds = mpx_find_bounds (call_arg, gsi);
	  gimple_call_set_arg (new_call, new_arg_no++, bounds);
	  bnd_arg_cnt--;
	}
    }

  /* replace old call statement with the new one.  */
  FOR_EACH_SSA_TREE_OPERAND (op, call, iter, SSA_OP_ALL_DEFS)
    {
      SSA_NAME_DEF_STMT (op) = new_call;
    }
  gsi_replace (gsi, new_call, true);
}

/*  Search rtx PAR describing function return value for an
    item related to value at offset OFFS and return it.
    Return NULL if item was not found.  */
rtx
mpx_get_value_with_offs (rtx par, rtx offs)
{
  int n;

  gcc_assert (GET_CODE (par) == PARALLEL);

  for (n = 0; n < XVECLEN (par, 0); n++)
    {
      rtx par_offs = XEXP (XVECEXP (par, 0, n), 1);
      if (INTVAL (offs) == INTVAL (par_offs))
	return XEXP (XVECEXP (par, 0, n), 0);
    }

  return NULL;
}

/* Helper function for mpx_copy_bounds_for_stack_parm.
   Fill HAVE_BOUND output array with information about
   bounds requred for object of type TYPE.

   OFFS is used for recursive calls and holds basic
   offset of TYPE in outer structure in bits.

   PTR_SIZE holds size of the pointer in bits.

   HAVE_BOUND[i] is set to 1 if there is a field
   in TYPE which has pointer type and offset
   equal to i * PTR_SIZE - OFFS in bits.  */
static void
mpx_find_bound_slots (tree type, bool *have_bound,
		      HOST_WIDE_INT offs,
		      HOST_WIDE_INT ptr_size)
{
  if (BOUNDED_TYPE_P (type))
    have_bound[offs / ptr_size] = true;
  else if (RECORD_OR_UNION_TYPE_P (type))
    {
      tree field;

      for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
	if (TREE_CODE (field) == FIELD_DECL)
	  {
	    HOST_WIDE_INT field_offs
	      = TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (field));
	    if (DECL_FIELD_OFFSET (field))
	      field_offs += TREE_INT_CST_LOW (DECL_FIELD_OFFSET (field)) * 8;
	    mpx_find_bound_slots (TREE_TYPE (field), have_bound,
				 offs + field_offs, ptr_size);
	  }
    }
  else if (TREE_CODE (type) == ARRAY_TYPE)
    {
      tree maxval = TYPE_MAX_VALUE (TYPE_DOMAIN (type));
      tree etype = TREE_TYPE (type);
      HOST_WIDE_INT esize = TREE_INT_CST_LOW (TYPE_SIZE (etype));
      unsigned HOST_WIDE_INT cur;

      for (cur = 0; cur <= TREE_INT_CST_LOW (maxval); cur++)
	mpx_find_bound_slots (etype, have_bound, offs + cur * esize, ptr_size);
    }
}

/* Emit code to copy bounds for structure VALUE of type TYPE
   copied to SLOT.  */
void
mpx_copy_bounds_for_stack_parm (rtx slot, rtx value, tree type)
{
  HOST_WIDE_INT ptr_size = TREE_INT_CST_LOW (TYPE_SIZE (mpx_uintptr_type));
  HOST_WIDE_INT max_bounds = TREE_INT_CST_LOW (TYPE_SIZE (type)) / ptr_size;
  bool *have_bound = (bool *)xmalloc (sizeof (bool) * max_bounds);
  HOST_WIDE_INT i;
  rtx tmp = NULL, bnd;

  memset (have_bound, 0, sizeof (bool) * max_bounds);

  gcc_assert (TYPE_SIZE (type));
  gcc_assert (MEM_P (value));
  gcc_assert (MEM_P (slot));
  gcc_assert (RECORD_OR_UNION_TYPE_P (type));

  mpx_find_bound_slots (type, have_bound, 0, ptr_size);

  for (i = 0; i < max_bounds; i++)
    if (have_bound[i])
      {
	rtx ptr = adjust_address (value, Pmode, i * ptr_size / 8);
	rtx to = adjust_address (slot, Pmode, i * ptr_size / 8);

	if (!tmp)
	  tmp = gen_reg_rtx (Pmode);

	emit_move_insn (tmp, ptr);
	bnd = targetm.calls.load_bounds_for_arg (ptr, tmp, NULL);
	targetm.calls.store_bounds_for_arg (tmp, to, bnd, NULL);
      }

  free (have_bound);
}

/* Return entry block to be used for MPX initilization code.
   Create new block if required.  */
static basic_block
mpx_get_entry_block (void)
{
  if (!entry_block)
    entry_block = split_block (ENTRY_BLOCK_PTR, NULL)->dest;

  return entry_block;
}

/* Creates a static bounds var of specfified NAME initilized
   with specified LB and UB values.  */
static tree
mpx_make_static_const_bounds (HOST_WIDE_INT lb,
			      HOST_WIDE_INT ub,
			      const char *name)
{
  tree var = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			 get_identifier (name), bound_type_node);

  TREE_PUBLIC (var) = 1;
  TREE_USED (var) = 1;
  TREE_READONLY (var) = 1;
  TREE_STATIC (var) = 1;
  TREE_ADDRESSABLE (var) = 0;
  DECL_ARTIFICIAL (var) = 1;
  DECL_COMMON (var) = 1;
  DECL_COMDAT (var) = 1;
  DECL_COMDAT_GROUP (var) = DECL_ASSEMBLER_NAME (var);
  DECL_READ_P (var) = 1;
  DECL_INITIAL (var) = build_int_cst_wide (bound_type_node, lb, ~ub);

  vec_safe_push (mpx_static_const_bounds, var);

  return var;
}

/* Generate code to make bounds with specified lower bound LB and SIZE.
   if AFTER is 1 then code is inserted after position pointed by ITER
   otherwise code is inserted before position pointed by ITER.
   If ITER is NULL then code is added to entry block.  */
static tree
mpx_make_bounds (tree lb, tree size, gimple_stmt_iterator *iter, bool after)
{
  gimple_seq seq;
  gimple_stmt_iterator gsi;
  gimple stmt;
  tree bounds;

  if (iter)
    gsi = *iter;
  else
    gsi = gsi_start_bb (mpx_get_entry_block ());

  seq = NULL;//gimple_seq_alloc ();

  lb = mpx_force_gimple_call_op (lb, &seq);
  size = mpx_force_gimple_call_op (size, &seq);

  stmt = gimple_build_call (mpx_bndmk_fndecl, 2, lb, size);
  mpx_mark_stmt (stmt);

  bounds = make_ssa_name (mpx_get_tmp_var (), stmt);
  gimple_call_set_lhs (stmt, bounds);

  gimple_seq_add_stmt (&seq, stmt);

  if (iter && after)
    gsi_insert_seq_after (&gsi, seq, GSI_SAME_STMT);
  else
    gsi_insert_seq_before (&gsi, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Made bounds: ");
      print_gimple_stmt (dump_file, stmt, 0, TDF_VOPS|TDF_MEMSYMS);
      if (iter)
	{
	  fprintf (dump_file, "  inserted before statement: ");
	  print_gimple_stmt (dump_file, gsi_stmt (*iter), 0, TDF_VOPS|TDF_MEMSYMS);
	}
      else
	fprintf (dump_file, "  at function entry\n");
    }

  /* update_stmt (stmt); */

  return bounds;
}

/* Return SSA_NAME used to represent zero bounds.  */
tree
mpx_get_zero_bounds (void)
{
  if (zero_bounds)
    return zero_bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Creating zero bounds...");

  if ((flag_mpx_use_static_bounds && flag_mpx_use_static_const_bounds)
      || flag_mpx_use_static_const_bounds > 0)
    {
      gimple_stmt_iterator gsi = gsi_start_bb (mpx_get_entry_block ());
      gimple stmt;

      if (!mpx_zero_bounds_var)
	mpx_zero_bounds_var
	  = mpx_make_static_const_bounds (0, -1,
					  MPX_ZERO_BOUNDS_VAR_NAME);

      zero_bounds = make_ssa_name (mpx_get_tmp_var (), gimple_build_nop ());
      stmt = gimple_build_assign (zero_bounds, mpx_zero_bounds_var);
      gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);
    }
  else
    zero_bounds = mpx_make_bounds (integer_zero_node,
				   integer_zero_node,
				   NULL,
				   false);

  return zero_bounds;
}

/* Return SSA_NAME used to represent none bounds.  */
static tree
mpx_get_none_bounds (void)
{
  if (none_bounds)
    return none_bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Creating none bounds...");


  if ((flag_mpx_use_static_bounds && flag_mpx_use_static_const_bounds)
      || flag_mpx_use_static_const_bounds > 0)
    {
      gimple_stmt_iterator gsi = gsi_start_bb (mpx_get_entry_block ());
      gimple stmt;

      if (!mpx_none_bounds_var)
	mpx_none_bounds_var
	  = mpx_make_static_const_bounds (-1, 0,
					  MPX_NONE_BOUNDS_VAR_NAME);

      none_bounds = make_ssa_name (mpx_get_tmp_var (), gimple_build_nop ());
      stmt = gimple_build_assign (none_bounds, mpx_none_bounds_var);
      gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);
    }
  else
    none_bounds = mpx_make_bounds (integer_minus_one_node,
				   build_int_cst (size_type_node, 2),
				   NULL,
				   false);

  return none_bounds;
}

/* Return bounds to be used as a result of operation which
   should not create poiunter (e.g. MULT_EXPR).  */
static tree
mpx_get_invalid_op_bounds (void)
{
  return mpx_get_zero_bounds ();
}

/* Return bounds to be used for loads of non-pointer values.  */
static tree
mpx_get_nonpointer_load_bounds (void)
{
  return mpx_get_zero_bounds ();
}

/* Build bounds returned by CALL.  */
static tree
mpx_build_returned_bound (gimple call)
{
  gimple_stmt_iterator gsi;
  tree bounds;
  gimple stmt;
  tree fndecl = gimple_call_fndecl (call);

  /* To avoid fixing alloca expands in targets we handle
     it separately.  */
  if (fndecl
      && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_ALLOCA
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_ALLOCA_WITH_ALIGN))
    {
      tree size = gimple_call_arg (call, 0);
      tree lb = gimple_call_lhs (call);
      gimple_stmt_iterator iter = gsi_for_stmt (call);
      bounds = mpx_make_bounds (lb, size, &iter, true);
    }
  /* Similarly handle next_arg builtin.  */
  else if (fndecl
	   && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
	   && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_NEXT_ARG)
    {
      tree size = targetm.fn_abi_va_list_bounds_size (cfun->decl);
      if (size == integer_zero_node)
	bounds = mpx_get_zero_bounds ();
      else
	{
	  tree lb = gimple_call_lhs (call);
	  gimple_stmt_iterator iter = gsi_for_stmt (call);
	  bounds = mpx_make_bounds (lb, size, &iter, true);
	}
    }
  /* Detect bounds initialization calls.  */
  else if (fndecl
      && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_INIT_PTR_BOUNDS)
    bounds = mpx_get_zero_bounds ();
  /* Detect bounds copy calls.  */
  else if (fndecl
      && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_COPY_PTR_BOUNDS)
    {
      gimple_stmt_iterator iter = gsi_for_stmt (call);
      bounds = mpx_find_bounds (gimple_call_arg (call, 1), &iter);
    }
  else
    {
      /* In general case build MPX builtin call to
	 obtain returned bounds.  */
      stmt = gimple_build_call (mpx_ret_bnd_fndecl, 0);
      mpx_mark_stmt (stmt);

      gsi = gsi_for_stmt (call);
      gsi_insert_after (&gsi, stmt, GSI_SAME_STMT);

      bounds = make_ssa_name (mpx_get_tmp_var (), stmt);
      gimple_call_set_lhs (stmt, bounds);

      update_stmt (stmt);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Built returned bounds (");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, ") for call: ");
      print_gimple_stmt (dump_file, call, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  mpx_register_bounds (gimple_call_lhs (call), bounds);

  return bounds;
}

/* Return bounds to be used for input argument PARM.  */
static tree
mpx_get_bound_for_parm (tree parm)
{
  tree decl = SSA_NAME_VAR (parm);
  tree bounds;

  bounds = mpx_get_registered_bounds (parm);

  if (!bounds)
    bounds = mpx_get_registered_bounds (decl);

  if (!bounds)
    {
      /* For static chain param we return zero bounds
	 because currently we do not check dereferences
	 of this pointer.  */
      /* ?? Is it a correct way to identify such parm?  */
      if (cfun->decl && DECL_STATIC_CHAIN (cfun->decl)
	  && DECL_ARTIFICIAL (decl))
	bounds = mpx_get_zero_bounds ();
      /* If non-MPX runtime is used then it may be useful
	 to use zero bounds for input arguments of main
	 function.  */
      else if (flag_mpx_zero_input_bounds_for_main
	       && strcmp (IDENTIFIER_POINTER (DECL_NAME (cfun->decl)), "main") == 0)
	bounds = mpx_get_zero_bounds ();
      else if (BOUNDED_P (parm))
	{
	  /* In general case we use MPX builtin to
	     obtain bounds of input arg.  */
	  gimple_stmt_iterator gsi;
	  gimple stmt;

	  stmt = gimple_build_call (mpx_arg_bnd_fndecl, 1, parm);
	  mpx_mark_stmt (stmt);

	  gsi = gsi_start_bb (mpx_get_entry_block ());
	  gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);

	  bounds = make_ssa_name (mpx_get_tmp_var (), stmt);
	  gimple_call_set_lhs (stmt, bounds);

	  update_stmt (stmt);
	  mpx_register_bounds (decl, bounds);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Built arg bounds (");
	      print_generic_expr (dump_file, bounds, 0);
	      fprintf (dump_file, ") for arg: ");
	      print_node (dump_file, "", decl, 0);
	    }
	}
      else
	bounds = mpx_get_zero_bounds ();
    }

  if (!mpx_get_registered_bounds (parm))
    mpx_register_bounds (parm, bounds);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Using bounds ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, " for parm ");
      print_generic_expr (dump_file, parm, 0);
      fprintf (dump_file, " of type ");
      print_generic_expr (dump_file, TREE_TYPE (parm), 0);
      fprintf (dump_file, ".\n");
    }

  return bounds;
}

/* Insert code to load bounds for PTR located by ADDR.
   Code is inserted after position pointed by GSI.
   Loaded bounds are returned.  */
static tree
mpx_build_bndldx (tree addr, tree ptr, gimple_stmt_iterator *gsi)
{
  gimple_seq seq;
  gimple stmt;
  tree bounds;

  seq = NULL;

  addr = mpx_force_gimple_call_op (addr, &seq);
  ptr = mpx_force_gimple_call_op (ptr, &seq);

  stmt = gimple_build_call (mpx_bndldx_fndecl, 2, addr, ptr);
  mpx_mark_stmt (stmt);
  bounds = make_ssa_name (mpx_get_tmp_var (), stmt);
  gimple_call_set_lhs (stmt, bounds);

  gimple_seq_add_stmt (&seq, stmt);

  gsi_insert_seq_after (gsi, seq, GSI_CONTINUE_LINKING);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generated bndldx for pointer ");
      print_generic_expr (dump_file, ptr, 0);
      fprintf (dump_file, ": ");
      print_gimple_stmt (dump_file, stmt, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  return bounds;
}

/* Build and return CALL_EXPR for bndstx builtin wich specified
   arguments.  */
tree
mpx_build_bndstx_call (tree addr, tree ptr, tree bounds)
{
  tree call = build1 (ADDR_EXPR,
		      build_pointer_type (TREE_TYPE (mpx_bndstx_fndecl)),
		      mpx_bndstx_fndecl);
  return build_call_nary (TREE_TYPE (TREE_TYPE (mpx_bndstx_fndecl)),
			  call, 3, addr, ptr, bounds);
}

/* Emit code to store zero bounds for PTR located at MEM.  */
void
mpx_expand_bounds_reset_for_mem (tree mem, tree ptr)
{
  tree zero_bnd, bnd, addr, bndstx;

  if (flag_mpx_use_static_const_bounds)
    {
      if (!mpx_zero_bounds_var)
	mpx_zero_bounds_var
	  = mpx_make_static_const_bounds (0, -1,
					  MPX_ZERO_BOUNDS_VAR_NAME);
      zero_bnd = mpx_zero_bounds_var;
    }
  else
    zero_bnd = mpx_build_make_bounds_call (integer_zero_node,
					   integer_zero_node);
  bnd = make_tree (bound_type_node,
			assign_temp (bound_type_node, 0, 1));
  addr = build1 (ADDR_EXPR,
		      build_pointer_type (TREE_TYPE (mem)), mem);
  bndstx = mpx_build_bndstx_call (addr, ptr, bnd);

  expand_assignment (bnd, zero_bnd, false);
  expand_normal (bndstx);
}

/* Insert code to store BOUNDS for PTR stored by ADDR.
   New statements are inserted after position pointed
   by GSI.  */
static void
mpx_build_bndstx (tree addr, tree ptr, tree bounds,
		 gimple_stmt_iterator *gsi)
{
  gimple_seq seq;
  gimple stmt;

  seq = NULL;

  addr = mpx_force_gimple_call_op (addr, &seq);
  ptr = mpx_force_gimple_call_op (ptr, &seq);

  stmt = gimple_build_call (mpx_bndstx_fndecl, 3, addr, ptr, bounds);
  mpx_mark_stmt (stmt);

  gimple_seq_add_stmt (&seq, stmt);

  gsi_insert_seq_after (gsi, seq, GSI_CONTINUE_LINKING);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generated bndstx for pointer store ");
      print_gimple_stmt (dump_file, gsi_stmt (*gsi), 0, TDF_VOPS|TDF_MEMSYMS);
      print_gimple_stmt (dump_file, stmt, 2, TDF_VOPS|TDF_MEMSYMS);
    }
}

/* Compute bounds for pointer NODE which was assigned in
   assignment statement ASSIGN.  Return computed bounds.  */
static tree
mpx_compute_bounds_for_assignment (tree node, gimple assign)
{
  enum tree_code rhs_code = gimple_assign_rhs_code (assign);
  tree rhs1 = gimple_assign_rhs1 (assign);
  tree bounds = NULL_TREE;
  gimple_stmt_iterator iter = gsi_for_stmt (assign);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Computing bounds for assignment: ");
      print_gimple_stmt (dump_file, assign, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  switch (rhs_code)
    {
    case MEM_REF:
    case ARRAY_REF:
    case COMPONENT_REF:
    case TARGET_MEM_REF:
      /* We need to load bounds from the bounds table.  */
      bounds = mpx_find_bounds_loaded (node, rhs1, &iter);
      break;

    case VAR_DECL:
    case SSA_NAME:
    case ADDR_EXPR:
    case POINTER_PLUS_EXPR:
    case NOP_EXPR:
    case CONVERT_EXPR:
    case INTEGER_CST:
      /* Bounds are just propagated from RHS.  */
      bounds = mpx_find_bounds (rhs1, &iter);
      break;

    case VIEW_CONVERT_EXPR:
      /* Bounds are just propagated from RHS.  */
      bounds = mpx_find_bounds (TREE_OPERAND (rhs1, 0), &iter);
      break;

    case PARM_DECL:
      gcc_assert (TREE_ADDRESSABLE (rhs1));
      /* We need to load bounds from the bounds table.  */
      bounds = mpx_build_bndldx (mpx_build_addr_expr (rhs1),
				node, &iter);
      break;

    case MINUS_EXPR:
    case PLUS_EXPR:
    case BIT_AND_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
      {
	tree rhs2 = gimple_assign_rhs2 (assign);
	tree bnd1 = mpx_find_bounds (rhs1, &iter);
	tree bnd2 = mpx_find_bounds (rhs2, &iter);

	/* First we try to check types of operands.  If it
	   does not help then look at bound values.

	   If some bounds are incomplete and other are
	   not proven to be valid (i.e. also incomplete
	   or invalid because value is not pointer) then
	   resulting value is incomplete and will be
	   recomputed later in mpx_finish_incomplete_bounds.  */
	if (BOUNDED_P (rhs1)
	    && !BOUNDED_P (rhs2))
	  bounds = bnd1;
	else if (BOUNDED_P (rhs2)
		 && !BOUNDED_P (rhs1)
		 && rhs_code != MINUS_EXPR)
	  bounds = bnd2;
	else if (mpx_incomplete_bounds (bnd1))
	  if (mpx_valid_bounds (bnd2) && rhs_code != MINUS_EXPR
	      && !mpx_incomplete_bounds (bnd2))
	    bounds = bnd2;
	  else
	    bounds = incomplete_bounds;
	else if (mpx_incomplete_bounds (bnd2))
	  if (mpx_valid_bounds (bnd1)
	      && !mpx_incomplete_bounds (bnd1))
	    bounds = bnd1;
	  else
	    bounds = incomplete_bounds;
	else if (!mpx_valid_bounds (bnd1))
	  if (mpx_valid_bounds (bnd2) && rhs_code != MINUS_EXPR)
	    bounds = bnd2;
	  else if (bnd2 == mpx_get_zero_bounds ())
	    bounds = bnd2;
	  else
	    bounds = bnd1;
	else if (!mpx_valid_bounds (bnd2))
	  bounds = bnd1;
	else
	  /* Seems both operands may have valid bounds
	     (e.g. pointer minus pointer).  In such case
	     use default invalid op bounds.  */
	  bounds = mpx_get_invalid_op_bounds ();
      }
      break;

    case BIT_NOT_EXPR:
    case NEGATE_EXPR:
    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case MULT_EXPR:
    case RDIV_EXPR:
    case TRUNC_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case CEIL_MOD_EXPR:
    case ROUND_MOD_EXPR:
    case EXACT_DIV_EXPR:
    case FIX_TRUNC_EXPR:
    case FLOAT_EXPR:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
      /* No valid bounds may be produced by these exprs.  */
      bounds = mpx_get_invalid_op_bounds ();
      break;

    case COND_EXPR:
      {
	tree val1 = gimple_assign_rhs2 (assign);
	tree val2 = gimple_assign_rhs3 (assign);
	tree bnd1 = mpx_find_bounds (val1, &iter);
	tree bnd2 = mpx_find_bounds (val2, &iter);
	gimple stmt;

	if (mpx_incomplete_bounds (bnd1) || mpx_incomplete_bounds (bnd2))
	  bounds = incomplete_bounds;
	else if (bnd1 == bnd2)
	  bounds = bnd1;
	else
	  {
	    if (!tree_node_can_be_shared (rhs1))
	      rhs1 = unshare_expr (rhs1);

	    bounds = make_ssa_name (mpx_get_tmp_var (), assign);
	    stmt = gimple_build_assign_with_ops (COND_EXPR, bounds,
						  rhs1, bnd1, bnd2);
	    gsi_insert_after (&iter, stmt, GSI_SAME_STMT);

	    if (!mpx_valid_bounds (bnd1) && !mpx_valid_bounds (bnd2))
	      mpx_mark_invalid_bounds (bounds);
	  }
      }
      break;

    case MAX_EXPR:
    case MIN_EXPR:
      {
	tree rhs2 = gimple_assign_rhs2 (assign);
	tree bnd1 = mpx_find_bounds (rhs1, &iter);
	tree bnd2 = mpx_find_bounds (rhs2, &iter);

	if (mpx_incomplete_bounds (bnd1) || mpx_incomplete_bounds (bnd2))
	  bounds = incomplete_bounds;
	else if (bnd1 == bnd2)
	  bounds = bnd1;
	else
	  {
	    gimple stmt;
	    tree cond = build2 (rhs_code == MAX_EXPR ? GT_EXPR : LT_EXPR,
				boolean_type_node, rhs1, rhs2);
	    bounds = make_ssa_name (mpx_get_tmp_var (), assign);
	    stmt = gimple_build_assign_with_ops (COND_EXPR, bounds,
						  cond, bnd1, bnd2);

	    gsi_insert_after (&iter, stmt, GSI_SAME_STMT);

	    if (!mpx_valid_bounds (bnd1) && !mpx_valid_bounds (bnd2))
	      mpx_mark_invalid_bounds (bounds);
	  }
      }
      break;

    default:
      internal_error ("mpx_compute_bounds_for_assignment: Unexpected RHS code %s",
		      tree_code_name[rhs_code]);
    }

  gcc_assert (bounds);

  if (node)
    mpx_register_bounds (node, bounds);

  return bounds;
}

/* Compute bounds for ssa name NODE defined by DEF_STMT pointed by ITER.

   There are just few statement codes allowed: NOP (for default ssa names),
   ASSIGN, CALL, PHI, ASM.

   Return computed bounds.  */
static tree
mpx_get_bounds_by_definition (tree node, gimple def_stmt, gimple_stmt_iterator *iter)
{
  tree var, bounds;
  enum gimple_code code = gimple_code (def_stmt);
  gimple stmt;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Searching for bounds for node: ");
      print_generic_expr (dump_file, node, 0);

      fprintf (dump_file, " using its definition: ");
      print_gimple_stmt (dump_file, def_stmt, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  switch (code)
    {
    case GIMPLE_NOP:
      var = SSA_NAME_VAR (node);
      switch (TREE_CODE (var))
	{
	case PARM_DECL:
	  bounds = mpx_get_bound_for_parm (node);
	  break;

	case VAR_DECL:
	  /* For uninitialized pointers use none bounds.  */
	  bounds = mpx_get_none_bounds ();
	  mpx_register_bounds (node, bounds);
	  break;

	case RESULT_DECL:
	  {
	    tree base_type;

	    gcc_assert (TREE_CODE (TREE_TYPE (node)) == REFERENCE_TYPE);

	    base_type = TREE_TYPE (TREE_TYPE (node));

	    gcc_assert (TYPE_SIZE (base_type)
			&& TREE_CODE (TYPE_SIZE (base_type)) == INTEGER_CST
			&& tree_low_cst (TYPE_SIZE (base_type), 1) != 0);

	    bounds = mpx_make_bounds (node, TYPE_SIZE_UNIT (base_type), NULL, false);
	    mpx_register_bounds (node, bounds);
	  }
	  break;

	default:
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Unexpected var with no definition\n");
	      print_generic_expr (dump_file, var, 0);
	    }
	  internal_error ("mpx_get_bounds_by_definition: Unexpected var of type %s",
			  tree_code_name[(int) TREE_CODE (var)]);
	}
      break;

    case GIMPLE_ASSIGN:
      bounds = mpx_compute_bounds_for_assignment (node, def_stmt);
      break;

    case GIMPLE_CALL:
      bounds = mpx_build_returned_bound (def_stmt);
      break;

    case GIMPLE_PHI:
      stmt = create_phi_node (mpx_get_tmp_var (), gimple_bb (def_stmt));
      bounds = gimple_phi_result (stmt);
      *iter = gsi_for_stmt (stmt);

      mpx_register_bounds (node, bounds);

      /* Created bounds do not have all phi args computed and
	 therefore we do not know if there is a valid source
	 of bounds for that node.  Therefore we mark bounds
	 as incomplete and then recompute them when all phi
	 args are computed.  */
      mpx_register_incomplete_bounds (bounds, node);
      break;

    case GIMPLE_ASM:
      bounds = mpx_get_zero_bounds ();
      mpx_register_bounds (node, bounds);
      break;

    default:
      internal_error ("mpx_get_bounds_by_definition: Unexpected GIMPLE code %s",
		      gimple_code_name[code]);
    }

  return bounds;
}

/* Return CALL_EXPR for bndmk with specified LOWER_BOUND and SIZE.  */
tree
mpx_build_make_bounds_call (tree lower_bound, tree size)
{
  tree call = build1 (ADDR_EXPR,
		      build_pointer_type (TREE_TYPE (mpx_bndmk_fndecl)),
		      mpx_bndmk_fndecl);
  return build_call_nary (TREE_TYPE (TREE_TYPE (mpx_bndmk_fndecl)),
			  call, 2, lower_bound, size);
}

/* Create static bounds var of specfified NAME initilized
   with bounds of OBJ which is either VAR_DECL or string
   constant.  */
static tree
mpx_make_static_bounds (tree obj)
{
  static int string_id = 1;
  static int var_id = 1;
  struct tree_map **slot, *map;
  const char *var_name;
  char *bnd_var_name;
  tree bnd_var;

  /* First check if we already have required var.  */
  if (mpx_static_var_bounds)
    {
      struct tree_map *res, in;
      in.base.from = obj;
      in.hash = htab_hash_pointer (obj);

      res = (struct tree_map *) htab_find_with_hash (mpx_static_var_bounds,
						     &in, in.hash);

      if (res)
	return res->to;
    }

  if (TREE_CODE (obj) == VAR_DECL)
    {
      if (DECL_IGNORED_P (obj))
	{
	  bnd_var_name = (char *) xmalloc (strlen (MPX_VAR_BOUNDS_PREFIX) + 10);
	  sprintf (bnd_var_name, "%s%d", MPX_VAR_BOUNDS_PREFIX, var_id++);
	}
      else
	{
	  var_name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (obj));

	  /* For hidden symbols we want to skip first '*' char.  */
	  if (*var_name == '*')
	    var_name++;

	  bnd_var_name = (char *) xmalloc (strlen (var_name)
					   + strlen (MPX_BOUNDS_OF_SYMBOL_PREFIX) + 1);
	  strcpy (bnd_var_name, MPX_BOUNDS_OF_SYMBOL_PREFIX);
	  strcat (bnd_var_name, var_name);
	}

      bnd_var = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			    get_identifier (bnd_var_name), bound_type_node);

      /* Address of the var will be used as lower bound.  */
      TREE_ADDRESSABLE (obj) = 1;

      /* There are cases when symbol is removed ignoring that
	 we have bounds for it.  Avoid it by forcing symbol
	 output.  */
      symtab_get_node (obj)->symbol.force_output = 1;
    }
  else
    {
      bnd_var_name = (char *) xmalloc (strlen (MPX_STRING_BOUNDS_PREFIX) + 10);
      sprintf (bnd_var_name, "%s%d", MPX_STRING_BOUNDS_PREFIX, string_id++);

      bnd_var = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			    get_identifier (bnd_var_name), bound_type_node);
    }

  TREE_PUBLIC (bnd_var) = 0;
  TREE_USED (bnd_var) = 1;
  TREE_READONLY (bnd_var) = 0;
  TREE_STATIC (bnd_var) = 1;
  TREE_ADDRESSABLE (bnd_var) = 0;
  DECL_ARTIFICIAL (bnd_var) = 1;
  DECL_COMMON (bnd_var) = 1;
  DECL_COMDAT (bnd_var) = 1;
  DECL_READ_P (bnd_var) = 1;
  DECL_INITIAL (bnd_var) = mpx_build_addr_expr (obj);

  /* Add created var to the global hash map.  */
  if (!mpx_static_var_bounds)
    {
      mpx_static_var_bounds = htab_create_ggc (31, tree_map_hash, tree_map_eq, NULL);
      mpx_static_var_bounds_r = htab_create_ggc (31, tree_map_hash, tree_map_eq, NULL);
    }

  map = ggc_alloc_tree_map ();
  map->hash = htab_hash_pointer (obj);
  map->base.from = obj;
  map->to = bnd_var;

  slot = (struct tree_map **)
    htab_find_slot_with_hash (mpx_static_var_bounds, map, map->hash, INSERT);
  *slot = map;

  /* We use reversed hash map to provide determined
     order of var emitting.  With undetermined order
     we cannot pass bootstrap.  */
  map = ggc_alloc_tree_map ();
  map->hash = htab_hash_pointer (bnd_var);
  map->base.from = bnd_var;
  map->to = obj;

  slot = (struct tree_map **)
    htab_find_slot_with_hash (mpx_static_var_bounds_r, map, map->hash, INSERT);
  *slot = map;

  return bnd_var;
}

static tree mpx_get_var_size_decl (tree var) ATTRIBUTE_UNUSED;

/* Return var holding size relocation for given VAR.  */
static tree
mpx_get_var_size_decl (tree var)
{
  struct tree_map **slot, *map;
  const char *var_name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (var));
  const char *size_suffix = "@SIZE";
  char *decl_name;
  char *size_name;
  tree size_decl, size_reloc;

  /* For hidden symbols we want to skip first '*' char.  */
  if (*var_name == '*')
    var_name++;

  /* Check if we have decl already.  */
  if (mpx_size_decls)
    {
      struct tree_map *res, in;
      in.base.from = var;
      in.hash = htab_hash_pointer (var);

      res = (struct tree_map *) htab_find_with_hash (mpx_size_decls,
						     &in, in.hash);

      if (res)
	return res->to;
    }

  /* No prepared decl was found.  Create new decl for var size.  */
  size_name = (char *) xmalloc (strlen (var_name) + strlen (size_suffix) + 1);
  strcpy (size_name, var_name);
  strcat (size_name, size_suffix);

  size_reloc = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			   get_identifier (size_name), mpx_uintptr_type);

  TREE_PUBLIC (size_reloc) = 1;
  TREE_USED (size_reloc) = 1;
  DECL_ARTIFICIAL (size_reloc) = 1;
  DECL_EXTERNAL (size_reloc) = 1;
  DECL_COMMON (size_reloc) = 1;

  decl_name = (char *) xmalloc (strlen (var_name)
			       + strlen (MPX_SIZE_OF_SYMBOL_PREFIX) + 1);
  strcpy (decl_name, MPX_SIZE_OF_SYMBOL_PREFIX);
  strcat (decl_name, var_name);

  size_decl = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			 get_identifier (decl_name), mpx_uintptr_type);

  TREE_PUBLIC (size_decl) = 0;
  TREE_USED (size_decl) = 1;
  TREE_READONLY (size_decl) = 1;
  TREE_STATIC (size_decl) = 1;
  TREE_ADDRESSABLE (size_decl) = 1;
  DECL_ARTIFICIAL (size_decl) = 1;
  DECL_COMMON (size_decl) = 1;
  DECL_COMDAT (size_decl) = 1;
  DECL_READ_P (size_decl) = 1;
  DECL_INITIAL (size_decl) = mpx_build_addr_expr (size_reloc);

  free (size_name);
  free (decl_name);

  /* Add created decl to the global hash map.  */
  if (!mpx_size_decls)
    mpx_size_decls = htab_create_ggc (31, tree_map_hash, tree_map_eq, NULL);

  map = ggc_alloc_tree_map ();
  map->hash = htab_hash_pointer (var);
  map->base.from = var;
  map->to = size_decl;

  slot = (struct tree_map **)
    htab_find_slot_with_hash (mpx_size_decls, map, map->hash, INSERT);
  *slot = map;

  return size_decl;
}

/* When var has incomplete type we cannot get size to
   compute its bounds.  In such cases we use MPX builtin
   call which determines object size at runtime.  */
static tree
mpx_generate_extern_var_bounds (tree var)
{
  tree bounds, size_reloc, lb, size, max_size, cond;
  gimple_stmt_iterator gsi;
  gimple_seq seq = NULL;
  gimple stmt;

  /* If MPX instrumentation is not enabled for vars having
     incomplete type then just return zero bounds to avoid
     checks for this var.  */
  if (!flag_mpx_incomplete_type)
    return mpx_get_zero_bounds ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generating bounds for extern symbol '");
      print_generic_expr (dump_file, var, 0);
      fprintf (dump_file, "'\n");
    }

  //size_reloc = mpx_get_var_size_decl (var);

  stmt = gimple_build_call (mpx_sizeof_fndecl, 1, var);

  size_reloc = create_tmp_reg (mpx_uintptr_type, MPX_SIZE_TMP_NAME);
  gimple_call_set_lhs (stmt, size_reloc);

  gimple_seq_add_stmt (&seq, stmt);

  lb = mpx_build_addr_expr (var);
  size = make_ssa_name (mpx_get_size_tmp_var (), gimple_build_nop ());

  if (flag_mpx_zero_size_reloc_as_infinite)
    {
      /* We should check that size relocation was resolved.
	 If it was not then use maximum possible size for the var.  */
      max_size = build2 (MINUS_EXPR, mpx_uintptr_type, integer_zero_node,
			 fold_convert (mpx_uintptr_type, lb));
      max_size = mpx_force_gimple_call_op (max_size, &seq);

      cond = build2 (NE_EXPR, boolean_type_node, size_reloc, integer_zero_node);
      stmt = gimple_build_assign_with_ops (COND_EXPR, size,
					   cond, size_reloc, max_size);
      gimple_seq_add_stmt (&seq, stmt);
    }
  else
    {
      stmt = gimple_build_assign (size, size_reloc);
      gimple_seq_add_stmt (&seq, stmt);
    }

  gsi = gsi_start_bb (mpx_get_entry_block ());
  gsi_insert_seq_after (&gsi, seq, GSI_CONTINUE_LINKING);

  bounds = mpx_make_bounds (lb, size, &gsi, true);

  return bounds;
}

/* Return 1 if TYPE has fields with zero size or fields
   marked with mpx_variable_size attribute.  */
bool
mpx_variable_size_type (tree type)
{
  bool res = false;
  tree field;

  if (RECORD_OR_UNION_TYPE_P (type))
    for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
      {
	if (TREE_CODE (field) == FIELD_DECL)
	  res = res
	    || lookup_attribute ("bnd_variable_size", DECL_ATTRIBUTES (field))
	    || mpx_variable_size_type (TREE_TYPE (field));
      }
  else
    res = !TYPE_SIZE (type)
      || TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST
      || tree_low_cst (TYPE_SIZE (type), 1) == 0;

  return res;
}

/* Compute and return bounds for address of DECL which is
   one of VAR_DECL, PARM_DECL, RESULT_DECL.  */
static tree
mpx_get_bounds_for_decl_addr (tree decl)
{
  tree bounds;

  gcc_assert (TREE_CODE (decl) == VAR_DECL
	      || TREE_CODE (decl) == PARM_DECL
	      || TREE_CODE (decl) == RESULT_DECL);

  bounds = mpx_get_registered_addr_bounds (decl);

  if (bounds)
    return bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Building bounds for address of decl ");
      print_generic_expr (dump_file, decl, 0);
      fprintf (dump_file, "\n");
    }

  /* Use zero bounds if size is unknown and checks for
     unknown sizes are restricted.  */
  if ((!DECL_SIZE (decl)
       || (mpx_variable_size_type (TREE_TYPE (decl))
	   && (TREE_STATIC (decl)
	       || DECL_EXTERNAL (decl)
	       || TREE_PUBLIC (decl))))
      && !flag_mpx_incomplete_type)
      return mpx_get_zero_bounds ();

  if (flag_mpx_use_static_bounds
      && TREE_CODE (decl) == VAR_DECL
      && (TREE_STATIC (decl)
	      || DECL_EXTERNAL (decl)
	      || TREE_PUBLIC (decl)))
    {
      tree bnd_var = mpx_make_static_bounds (decl);
      gimple_stmt_iterator gsi = gsi_start_bb (mpx_get_entry_block ());
      gimple stmt;

      bounds = make_ssa_name (mpx_get_tmp_var (),  gimple_build_nop ());
      stmt = gimple_build_assign (bounds, bnd_var);
      gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);
    }
  else if (!DECL_SIZE (decl)
      || (mpx_variable_size_type (TREE_TYPE (decl))
	  && (TREE_STATIC (decl)
	      || DECL_EXTERNAL (decl)
	      || TREE_PUBLIC (decl))))
    {
      gcc_assert (TREE_CODE (decl) == VAR_DECL);
      bounds = mpx_generate_extern_var_bounds (decl);
    }
  else
    {
      tree lb = mpx_build_addr_expr (decl);
      bounds = mpx_make_bounds (lb, DECL_SIZE_UNIT (decl), NULL, false);
    }

  return bounds;
}

/* Compute and return bounds for constant string.  */
static tree
mpx_get_bounds_for_string_cst (tree cst)
{
  tree bounds;
  tree lb;
  tree size;

  gcc_assert (TREE_CODE (cst) == STRING_CST);

  bounds = mpx_get_registered_bounds (cst);

  if (bounds)
    return bounds;

  if ((flag_mpx_use_static_bounds && flag_mpx_use_static_const_bounds)
      || flag_mpx_use_static_const_bounds > 0)
    {
      tree bnd_var = mpx_make_static_bounds (cst);
      gimple_stmt_iterator gsi = gsi_start_bb (mpx_get_entry_block ());
      gimple stmt;

      bounds = make_ssa_name (mpx_get_tmp_var (),  gimple_build_nop ());
      stmt = gimple_build_assign (bounds, bnd_var);
      gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);
    }
  else
    {
      lb = mpx_build_addr_expr (cst);
      size = build_int_cst (mpx_uintptr_type, TREE_STRING_LENGTH (cst));
      bounds = mpx_make_bounds (lb, size, NULL, false);
    }

  mpx_register_bounds (cst, bounds);

  return bounds;
}

/* Generate code to instersect bounds BOUNDS1 and BOUNDS2 and
   return the result.  if ITER is not NULL then Code is inserted
   before position pointed by ITER.  Otherwise code is added to
   entry block.  */
static tree
mpx_intersect_bounds (tree bounds1, tree bounds2, gimple_stmt_iterator *iter)
{
  if (!bounds1 || bounds1 == mpx_get_zero_bounds ())
    return bounds2 ? bounds2 : bounds1;
  else if (!bounds2 || bounds2 == mpx_get_zero_bounds ())
    return bounds1;
  else
    {
      gimple_seq seq;
      gimple stmt;
      tree bounds;

      seq = NULL;//gimple_seq_alloc ();

      stmt = gimple_build_call (mpx_intersect_fndecl, 2, bounds1, bounds2);
      mpx_mark_stmt (stmt);

      bounds = make_ssa_name (mpx_get_tmp_var (), stmt);
      gimple_call_set_lhs (stmt, bounds);

      gimple_seq_add_stmt (&seq, stmt);

      /* We are probably doing narrowing for constant expression.
	 In such case iter may be undefined.  */
      if (!iter)
	{
	  gimple_stmt_iterator gsi = gsi_last_bb (mpx_get_entry_block ());
	  iter = &gsi;
	  gsi_insert_seq_after (iter, seq, GSI_SAME_STMT);
	}
      else
	gsi_insert_seq_before (iter, seq, GSI_SAME_STMT);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Bounds intersection: ");
	  print_gimple_stmt (dump_file, stmt, 0, TDF_VOPS|TDF_MEMSYMS);
	  fprintf (dump_file, "  inserted before statement: ");
	  print_gimple_stmt (dump_file, gsi_stmt (*iter), 0,
			     TDF_VOPS|TDF_MEMSYMS);
	}

      return bounds;
    }
}

/* Return 1 if we are allowed to narrow bounds for addressed FIELD
   and 0 othersize.  */
static bool
mpx_may_narrow_to_field (tree field)
{
  return DECL_SIZE (field) && TREE_CODE (DECL_SIZE (field)) == INTEGER_CST
    && tree_low_cst (DECL_SIZE (field), 1) != 0
    && (!DECL_FIELD_OFFSET (field)
	|| TREE_CODE (DECL_FIELD_OFFSET (field)) == INTEGER_CST)
    && (!DECL_FIELD_BIT_OFFSET (field)
	|| TREE_CODE (DECL_FIELD_BIT_OFFSET (field)) == INTEGER_CST)
    && !lookup_attribute ("bnd_variable_size", DECL_ATTRIBUTES (field))
    && !mpx_variable_size_type (TREE_TYPE (field));
}

/* Return 1 if bounds for FIELD should be narrowed to
   field's own size.

   If ALWAYS_NARROW is non zero and narrowing is possible
   then true is returned.  */
static bool
mpx_narrow_bounds_for_field (tree field, bool always_narrow)
{
  HOST_WIDE_INT offs;
  HOST_WIDE_INT bit_offs;

  if (!mpx_may_narrow_to_field (field))
    return false;

  /* Accesse to compiler generated fields should not cause
     bounds narrowing.  */
  if (DECL_ARTIFICIAL (field))
    return false;

  offs = tree_low_cst (DECL_FIELD_OFFSET (field), 1);
  bit_offs = tree_low_cst (DECL_FIELD_BIT_OFFSET (field), 1);

  return (always_narrow || flag_mpx_first_field_has_own_bounds || offs || bit_offs);
}

/* Perform narrowing for BOUNDS using bounds computed for field
   access COMPONENT.  ITER meaning is the same as for
   mpx_intersect_bounds.  */
static tree
mpx_narrow_bounds_to_field (tree bounds, tree component,
			    gimple_stmt_iterator *iter)
{
  tree field = TREE_OPERAND (component, 1);
  tree size = DECL_SIZE_UNIT (field);
  tree field_ptr = mpx_build_addr_expr (component);
  tree field_bounds;

  field_bounds = mpx_make_bounds (field_ptr, size, iter, false);

  return mpx_intersect_bounds (field_bounds, bounds, iter);
}

/* Parse field or array access NODE.

   PTR ouput parameter holds a pointer to the outermost
   object.

   BITFIELD output parameter is set to 1 if bitfield is
   accessed and to 0 otherwise.  If it is 1 then ELT holds
   outer component for accessed bit field.

   SAFE outer parameter is set to 1 if access is safe and
   checks are not required.

   BOUNDS outer parameter holds bounds to be used to check
   access.

   If INNERMOST_BOUNDS is 1 then try to narrow bounds to the
   innermost ccessed component.

   If ALWAYS_NARROW then do narrowing ignoring field offset.  */
static void
mpx_parse_array_and_component_ref (tree node, tree *ptr,
				  tree *elt, bool *safe,
				  bool *bitfield,
				  tree *bounds,
				  gimple_stmt_iterator *iter,
				  bool innermost_bounds,
				  bool always_narrow)
{
  tree comp_to_narrow = NULL_TREE;
  tree last_comp = NULL_TREE;
  bool array_ref_found = false;
  tree *nodes;
  tree var;
  int len;
  int i;

  /* Compute tree height for expression.  */
  var = node;
  len = 1;
  while (TREE_CODE (var) == COMPONENT_REF
	 || TREE_CODE (var) == ARRAY_REF)
    {
      var = TREE_OPERAND (var, 0);
      len++;
    }

  gcc_assert (len > 1);

  /* It is more convenient for us to scan left-to-right,
     so walk tree again and put all node to nodes vector
     in reversed order.  */
  nodes = XALLOCAVEC (tree, len);
  nodes[len - 1] = node;
  for (i = len - 2; i >= 0; i--)
    nodes[i] = TREE_OPERAND (nodes[i + 1], 0);

  *bounds = NULL;
  *safe = true;
  *bitfield = (TREE_CODE (node) == COMPONENT_REF
	       && DECL_BIT_FIELD_TYPE (TREE_OPERAND (node, 1)));
  /* To get bitfield address we will need outer elemnt.  */
  if (*bitfield)
    *elt = nodes[len - 2];
  else
    *elt = NULL_TREE;

  /* If we have indirection in expression then compute
     outermost structure bounds.  Computed bounds may be
     narrowed later.  */
  if (TREE_CODE (nodes[0]) == MEM_REF || INDIRECT_REF_P (nodes[0]))
    {
      *safe = false;
      *ptr = TREE_OPERAND (nodes[0], 0);
      *bounds = mpx_find_bounds (*ptr, iter);
    }
  else
    {
      gcc_assert (TREE_CODE (var) == VAR_DECL
		  || TREE_CODE (var) == PARM_DECL
		  || TREE_CODE (var) == RESULT_DECL
		  || TREE_CODE (var) == STRING_CST);

      *ptr = mpx_build_addr_expr (var);
    }

  /* In this loop we are trying to find a field access
     requiring narrowing.  There are two simple rules
     for search:
     1. Leftmost array_ref is chosen if any.
     2. Rightmost suitable component_ref is chosen if innermost
     bounds are required and no array_ref exists.  */
  for (i = 1; i < len; i++)
    {
      var = nodes[i];

      if (TREE_CODE (var) == ARRAY_REF)
	{
	  *safe = false;
	  array_ref_found = true;
	  if (!flag_mpx_narrow_to_innermost_arrray
	      && (!last_comp
		  || mpx_may_narrow_to_field (TREE_OPERAND (last_comp, 1))))
	    {
	      comp_to_narrow = last_comp;
	      break;
	    }
	}
      else if (TREE_CODE (var) == COMPONENT_REF)
	{
	  tree field = TREE_OPERAND (var, 1);

	  if (innermost_bounds
	      && !array_ref_found
	      && mpx_narrow_bounds_for_field (field, always_narrow))
	    comp_to_narrow = var;
	  last_comp = var;

	  if (flag_mpx_narrow_to_innermost_arrray
	      && TREE_CODE (TREE_TYPE (field)) == ARRAY_TYPE)
	    {
	      *bounds = mpx_narrow_bounds_to_field (*bounds, var, iter);
	      comp_to_narrow = NULL;
	    }
	}
      else
	gcc_unreachable ();
    }

  if (comp_to_narrow && DECL_SIZE (TREE_OPERAND (comp_to_narrow, 1)))
    *bounds = mpx_narrow_bounds_to_field (*bounds, comp_to_narrow, iter);

  if (innermost_bounds && !*bounds)
    *bounds = mpx_find_bounds (*ptr, iter);
}

/* Compute and returne bounds for address of OBJ.
   If ALWAYS_NARROW_FIELDS is 1 then we need to narrow
   bounds to the smallest addressed field.  */
static tree
mpx_make_addressed_object_bounds (tree obj, gimple_stmt_iterator *iter,
				  bool always_narrow_fields)
{
  tree bounds = mpx_get_registered_addr_bounds (obj);

  if (bounds)
    return bounds;

  switch (TREE_CODE (obj))
    {
    case VAR_DECL:
    case PARM_DECL:
    case RESULT_DECL:
      bounds = mpx_get_bounds_for_decl_addr (obj);
      break;

    case STRING_CST:
      bounds = mpx_get_bounds_for_string_cst (obj);
      break;

    case ARRAY_REF:
    case COMPONENT_REF:
      {
	tree elt;
	tree ptr;
	bool safe;
	bool bitfield;

	mpx_parse_array_and_component_ref (obj, &ptr, &elt, &safe,
					  &bitfield, &bounds, iter, true,
					  always_narrow_fields);

	gcc_assert (bounds);
      }
      break;

    case FUNCTION_DECL:
    case LABEL_DECL:
      bounds = mpx_get_zero_bounds ();
      break;

    case MEM_REF:
      bounds = mpx_find_bounds (TREE_OPERAND (obj, 0), iter);
      break;

    case REALPART_EXPR:
    case IMAGPART_EXPR:
      bounds = mpx_make_addressed_object_bounds (TREE_OPERAND (obj, 0),
						iter,
						always_narrow_fields);
      break;

    default:
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "mpx_make_addressed_object_bounds: "
		   "unexpected object of type %s\n",
		   tree_code_name[TREE_CODE (obj)]);
	  print_node (dump_file, "", obj, 0);
	}
      internal_error ("mpx_make_addressed_object_bounds: Unexpected tree code %s",
		      tree_code_name[TREE_CODE (obj)]);
    }

  mpx_register_addr_bounds (obj, bounds);

  return bounds;
}

/* Compute bounds for pointer PTR loaded from PTR_SRC.  Generate statements
   to compute bounds if required.  Computed bounds should be available at
   position pointed by ITER.

   If PTR_SRC is NULL_TREE then pointer definition is identified.

   If PTR_SRC is not NULL_TREE then ITER points to statements which loads
   PTR.  If PTR is a any memory reference then ITER points to a statement
   after which bndldx will be inserterd.  In both cases ITER will be updated
   to point to the inserted bndldx statement.

   If ALWAYS_NARROW_FIELD is non zero and PTR is an address of structure
   field then we have to ignore flag_mpx_first_field_has_own_bounds flag
   value and perform bounds narrowing for this field.  */

static tree
mpx_find_bounds_1 (tree ptr, tree ptr_src, gimple_stmt_iterator *iter,
		   bool always_narrow_fields)
{
  tree addr = NULL_TREE;
  tree bounds = NULL_TREE;

  if (!ptr_src)
    ptr_src = ptr;

  bounds = mpx_get_registered_bounds (ptr_src);

  if (bounds)
    return bounds;

  switch (TREE_CODE (ptr_src))
    {
    case MEM_REF:
    case ARRAY_REF:
    case COMPONENT_REF:
    case VAR_DECL:
      if (BOUNDED_P (ptr_src))
	if (TREE_CODE (ptr) == VAR_DECL && DECL_REGISTER (ptr))
	  bounds = mpx_get_zero_bounds ();
	else
	  {
	    addr = mpx_build_addr_expr (ptr_src);
	    bounds = mpx_build_bndldx (addr, ptr, iter);
	  }
      else
	bounds = mpx_get_nonpointer_load_bounds ();
      break;

    case PARM_DECL:
      gcc_unreachable ();
      bounds = mpx_get_bound_for_parm (ptr_src);
      break;

    case TARGET_MEM_REF:
      addr = mpx_build_addr_expr (ptr_src);
      bounds = mpx_build_bndldx (addr, ptr, iter);
      break;

    case SSA_NAME:
      bounds = mpx_get_registered_bounds (ptr_src);
      if (!bounds)
	{
	  gimple def_stmt = SSA_NAME_DEF_STMT (ptr_src);
	  gimple_stmt_iterator phi_iter;

	  bounds = mpx_get_bounds_by_definition (ptr_src, def_stmt, &phi_iter);

	  gcc_assert (bounds);

	  if (gimple_code (def_stmt) == GIMPLE_PHI)
	    {
	      unsigned i;

	      for (i = 0; i < gimple_phi_num_args (def_stmt); i++)
		{
		  tree arg = gimple_phi_arg_def (def_stmt, i);
		  tree arg_bnd;
		  gimple phi_bnd;

		  arg_bnd = mpx_find_bounds (arg, NULL);

		  /* mpx_get_bounds_by_definition created new phi
		     statement and phi_iter points to it.

		     Previous call to mpx_find_bounds could create
		     new basic block and therefore change phi statement
		     phi_iter points to.  */
		  phi_bnd = gsi_stmt (phi_iter);

		  add_phi_arg (phi_bnd, arg_bnd,
			       gimple_phi_arg_edge (def_stmt, i),
			       UNKNOWN_LOCATION);
		}

	      /* If all bound phi nodes have their arg computed
		 then we may finish its computation.  See
		 mpx_finish_incomplete_bounds for more details.  */
	      if (mpx_may_finish_incomplete_bounds ())
		mpx_finish_incomplete_bounds ();
	    }

	  gcc_assert (bounds == mpx_get_registered_bounds (ptr_src)
		      || mpx_incomplete_bounds (bounds));
	}
      break;

    case ADDR_EXPR:
      bounds = mpx_make_addressed_object_bounds (TREE_OPERAND (ptr_src, 0), iter,
						always_narrow_fields);
      break;

    case INTEGER_CST:
      if (integer_zerop (ptr_src))
	bounds = mpx_get_none_bounds ();
      else
	bounds = mpx_get_invalid_op_bounds ();
      break;

    default:
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "mpx_find_bounds: unexpected ptr of type %s\n",
		   tree_code_name[TREE_CODE (ptr_src)]);
	  print_node (dump_file, "", ptr_src, 0);
	}
      internal_error ("mpx_find_bounds: Unexpected tree code %s",
		      tree_code_name[TREE_CODE (ptr_src)]);
    }

  if (!bounds)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (stderr, "mpx_find_bounds: cannot find bounds for pointer\n");
	  print_node (dump_file, "", ptr_src, 0);
	}
      internal_error ("mpx_find_bounds: Cannot find bounds for pointer");
    }

  return bounds;
}

/* Normal case for bounds search without forced narrowing.  */
static tree
mpx_find_bounds (tree ptr, gimple_stmt_iterator *iter)
{
  return mpx_find_bounds_1 (ptr, NULL_TREE, iter, false);
}

/* Search bounds for pointer PTR loaded from PTR_SRC
   by statement *ITER points to.  */
static tree
mpx_find_bounds_loaded (tree ptr, tree ptr_src, gimple_stmt_iterator *iter)
{
  return mpx_find_bounds_1 (ptr, ptr_src, iter, false);
}

/* Search for bounds for PTR to be used in abnormal PHI node.
   Similar to regular bounds search but create bound copy to
   be used over abnormal edge.  */
static tree
mpx_find_bounds_abnormal (tree ptr, tree phi)
{
  tree bounds = mpx_find_bounds_1 (ptr, NULL_TREE, NULL, false);
  tree copy = NULL;
  gimple assign;
  gimple_stmt_iterator gsi;
  struct tree_vec_map *found, in;
  vec<tree, va_gc> **copies = NULL;
  unsigned int i;

  if (gimple_code (SSA_NAME_DEF_STMT (bounds)) == GIMPLE_PHI
      && bounds == phi)
    return bounds;

  /* Check for existing bound copies created for specified
     PHI bounds.  */
  in.base.from = phi;
  found = (struct tree_vec_map *)
    htab_find_with_hash (mpx_abnormal_phi_copies, &in,
			 htab_hash_pointer (phi));

  if (found)
    {
      copies = &found->to;
      for (i = 0; i < (*copies)->length (); i++)
	{
	  tree ssa = (**copies)[i];
	  gimple def = SSA_NAME_DEF_STMT (ssa);
	  if (gimple_assign_rhs1 (def) == bounds)
	    {
	      copy = ssa;
	      break;
	    }
	}
    }

  /* If copy was not found then create it and store into
     vector of copies for PHI.  */
  if (!copy)
    {
      copy = create_tmp_reg (bound_type_node, MPX_BOUND_TMP_NAME);
      copy = make_ssa_name (copy, gimple_build_nop ());
      assign = gimple_build_assign (copy, bounds);

      if (gimple_code (SSA_NAME_DEF_STMT (bounds)) == GIMPLE_PHI)
	{
	  gsi = gsi_after_labels (gimple_bb (SSA_NAME_DEF_STMT (bounds)));
	  gsi_insert_before (&gsi, assign, GSI_SAME_STMT);
	}
      else
	{
	  if (gimple_code (SSA_NAME_DEF_STMT (bounds)) == GIMPLE_NOP)
	    gsi = gsi_last_bb (mpx_get_entry_block ());
	  else
	    gsi = gsi_for_stmt (SSA_NAME_DEF_STMT (bounds));

	  gsi_insert_after (&gsi, assign, GSI_SAME_STMT);
	}

      if (!copies)
	{
	  void **loc;

	  found = ggc_alloc_tree_vec_map ();
	  found->base.from = phi;
	  found->to = NULL;
	  loc = htab_find_slot_with_hash (mpx_abnormal_phi_copies, found,
					  htab_hash_pointer (phi),
					  INSERT);
	  *(struct tree_vec_map **) loc = found;

	  copies = &found->to;
	}

      vec_safe_push (*copies, copy);
    }

  /* After bounds are replaced with their copy in abnormal PHI,
     we need to recompute usage in abnormal phi flag.
     Current copy creation algorithm allows original bounds usage
     in abnormal phi only if it is a result of this phi.  */
  SSA_NAME_OCCURS_IN_ABNORMAL_PHI (bounds) = 0;
  if (gimple_code (SSA_NAME_DEF_STMT (bounds)) == GIMPLE_PHI)
    {
      gimple def = SSA_NAME_DEF_STMT (bounds);
      for (i = 0; i < gimple_phi_num_args (def); i++)
	{
	  tree arg = gimple_phi_arg_def (def, i);
	  edge e = gimple_phi_arg_edge (def, i);
	  if ((e->flags & EDGE_ABNORMAL)
	      && arg == bounds)
	    {
	      SSA_NAME_OCCURS_IN_ABNORMAL_PHI (bounds) = 1;
	      break;
	    }
	}
    }

  return copy;
}

/* Return COMPONENT_REF accessing FIELD in OBJ.  */
static tree
mpx_build_component_ref (tree obj, tree field)
{
  tree res;

  /* If object is TMR then we do not use component_ref but
     add offset instead.  We need it to be able to get addr
     of the reasult later.  */
  if (TREE_CODE (obj) == TARGET_MEM_REF) {
    tree offs = TMR_OFFSET (obj);
    offs = fold_binary_to_constant (PLUS_EXPR, TREE_TYPE (offs),
				    offs, DECL_FIELD_OFFSET (field));

    gcc_assert (offs);

    res = copy_node (obj);
    TREE_TYPE (res) = TREE_TYPE (field);
    TMR_OFFSET (res) = offs;
  } else
    res = build3 (COMPONENT_REF, TREE_TYPE (field), obj, field, NULL_TREE);

  return res;
}

/* Return ARRAY_REF for array ARR and index IDX with
   specified element type ETYPE and element size ESIZE.  */
static tree
mpx_build_array_ref (tree arr, tree etype, tree esize,
		    unsigned HOST_WIDE_INT idx)
{
  tree index = build_int_cst (size_type_node, idx);
  tree res;

  /* If object is TMR then we do not use array_ref but
     add offset instead.  We need it to be able to get addr
     of the reasult later.  */
  if (TREE_CODE (arr) == TARGET_MEM_REF)
    {
      tree offs = TMR_OFFSET (arr);

      esize = fold_binary_to_constant (MULT_EXPR, TREE_TYPE (esize),
				     esize, index);
      gcc_assert(esize);

      offs = fold_binary_to_constant (PLUS_EXPR, TREE_TYPE (offs),
				    offs, esize);
      gcc_assert (offs);

      res = copy_node (arr);
      TREE_TYPE (res) = etype;
      TMR_OFFSET (res) = offs;
    }
  else
    res = build4 (ARRAY_REF, etype, arr, index, NULL_TREE, NULL_TREE);

  return res;
}

/* Helper function which checks type of RHS and finds all pointers in
   it.  For each found pointer we build it's accesses in LHS and RHS
   objects and then call HANDLER for them.  Function is used to copy
   or initilize bounds for copied object.  */
static void
mpx_walk_pointer_assignments (tree lhs, tree rhs, void *arg, assign_handler handler)
{
  tree type = TREE_TYPE (rhs);

  /* We have nothing to do with clobbers.  */
  if (TREE_CLOBBER_P (rhs))
    return;

  if (BOUNDED_TYPE_P (type))
    handler (lhs, rhs, arg);
  else if (RECORD_OR_UNION_TYPE_P (type))
    {
      tree field;

      if (TREE_CODE (rhs) == CONSTRUCTOR)
	{
	  unsigned HOST_WIDE_INT cnt;
	  tree val;

	  FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (rhs), cnt, field, val)
	    {
	      if (mpx_type_has_pointer (TREE_TYPE (field)))
		{
		  tree lhs_field = mpx_build_component_ref (lhs, field);
		  mpx_walk_pointer_assignments (lhs_field, val, arg, handler);
		}
	    }
	}
      else
	for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
	  if (TREE_CODE (field) == FIELD_DECL
	      && mpx_type_has_pointer (TREE_TYPE (field)))
	    {
	      tree rhs_field = mpx_build_component_ref (rhs, field);
	      tree lhs_field = mpx_build_component_ref (lhs, field);
	      mpx_walk_pointer_assignments (lhs_field, rhs_field, arg, handler);
	    }
    }
  else if (TREE_CODE (type) == ARRAY_TYPE)
    {
      unsigned HOST_WIDE_INT cur = -1;
      tree maxval = TYPE_MAX_VALUE (TYPE_DOMAIN (type));
      tree etype = TREE_TYPE (type);
      tree esize = TYPE_SIZE (etype);

      if (TREE_CODE (rhs) == CONSTRUCTOR)
	{
	  unsigned HOST_WIDE_INT cnt;
	  tree purp, val, lhs_elem;

	  FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (rhs), cnt, purp, val)
	    {
	      if (TREE_CODE (purp) == RANGE_EXPR)
		{
		  tree lo_index = TREE_OPERAND (purp, 0);
		  tree hi_index = TREE_OPERAND (purp, 1);

		  for (cur = (unsigned)tree_low_cst (lo_index, 1);
		       cur <= (unsigned)tree_low_cst (hi_index, 1);
		       cur++)
		    {
		      lhs_elem = mpx_build_array_ref (lhs, etype, esize, cur);
		      mpx_walk_pointer_assignments (lhs_elem, val, arg, handler);
		    }
		}
	      else
		{
		  if (TREE_CODE (purp) == INTEGER_CST)
		    cur = tree_low_cst (purp, 1);
		  else
		    {
		      gcc_assert (!purp);
		      cur++;
		    }

		  lhs_elem = mpx_build_array_ref (lhs, etype, esize, cur);

		  mpx_walk_pointer_assignments (lhs_elem, val, arg, handler);
		}
	    }
	}
      /* Copy array only whe size is known.  */
      else if (maxval)
	for (cur = 0; cur <= TREE_INT_CST_LOW (maxval); cur++)
	  {
	    tree lhs_elem = mpx_build_array_ref (lhs, etype, esize, cur);
	    tree rhs_elem = mpx_build_array_ref (rhs, etype, esize, cur);
	    mpx_walk_pointer_assignments (lhs_elem, rhs_elem, arg, handler);
	  }
    }
  else
    internal_error("mpx_walk_pointer_assignments: unexpected RHS type: %s",
		   tree_code_name[TREE_CODE (type)]);
}

/* Add code to copy bounds for assignment of RHS to LHS.  */
static void
mpx_copy_bounds_for_assign (tree lhs, tree rhs, void *arg)
{
  gimple_stmt_iterator *iter = (gimple_stmt_iterator *)arg;
  tree bounds = mpx_find_bounds (rhs, iter);
  tree addr = mpx_build_addr_expr(lhs);

  mpx_build_bndstx (addr, rhs, bounds, iter);
}

/* Emit static bound initilizers and size vars.  */
void
mpx_finish_file (void)
{
  struct mpx_ctor_stmt_list stmts;
  bool tmp;
  int i;
  tree var;

  if (seen_error ())
    return;

  if (var_inits)
    {
      stmts.avail = MAX_STMTS_IN_STATIC_MPX_CTOR;
      stmts.stmts = NULL;

      FOR_EACH_VEC_ELT (*var_inits, i, var)
	/* !!! We must check that var is actually emitted and we need
	   and may initialize its bounds.  Currently asm_written flag and
	   rtl are checked.  Probably some other fields should be checked.  */
	if (DECL_RTL (var) && MEM_P (DECL_RTL (var)) && TREE_ASM_WRITTEN (var))
	  mpx_walk_pointer_assignments (var, DECL_INITIAL (var), &stmts,
					mpx_add_modification_to_statements_list);

      if (stmts.stmts)
	cgraph_build_static_cdtor ('P', stmts.stmts,
				   MAX_RESERVED_INIT_PRIORITY + 2);
    }

  if (mpx_static_const_bounds)
    FOR_EACH_VEC_ELT (*mpx_static_const_bounds, i, var)
      assemble_variable (var, 1, 0, 0);

  if (mpx_static_var_bounds)
    {
      unsigned int i;
      vec<tree> vars;

      stmts.avail = MAX_STMTS_IN_STATIC_MPX_CTOR;
      stmts.stmts = NULL;

      vars.create (htab_size (mpx_static_var_bounds));

      /* It seems that htab_traverse gives random vars order and thus
	 causes bootstrap to fails due to differences.  To fix it we
	 sort all vars by name first.  */
      htab_traverse (mpx_static_var_bounds_r,
		     mpx_add_tree_to_vec, &vars);
      vars.qsort (&mpx_compare_var_names);

      for (i = 0; i < vars.length (); i++)
	{
	  struct tree_map *res, in;
	  in.base.from = vars[i];
	  in.hash = htab_hash_pointer (vars[i]);

	  res = (struct tree_map *) htab_find_with_hash (mpx_static_var_bounds_r,
							 &in, in.hash);

	  mpx_output_static_bounds (res->to, vars[i], &stmts);
	}

      if (stmts.stmts)
	cgraph_build_static_cdtor ('B', stmts.stmts,
				   MAX_RESERVED_INIT_PRIORITY + 1);

      htab_delete (mpx_static_var_bounds);
      htab_delete (mpx_static_var_bounds_r);
      mpx_static_var_bounds = NULL;
      mpx_static_var_bounds_r = NULL;
    }

  if (mpx_size_decls)
    {
      htab_traverse (mpx_size_decls, mpx_output_size_variable, &tmp);

      htab_delete (mpx_size_decls);
      mpx_size_decls = NULL;
    }

  if (mpx_rtx_bounds)
    pointer_map_destroy (mpx_rtx_bounds);
}

/* An instrumentation function which is called for each statement
   having memory access we want to instrument.  It inserts check
   code and bounds copy code.

   ITER points to statement to instrument.

   NODE holds memory access in statement to check.

   LOC holds the location information for statement.

   DIRFLAGS determines whether access is read or write.

   ACCESS_OFFS should be added to address used in NODE
   before check.

   ACCESS_SIZE holds size of checked access.

   SAFE indicates if NODE access is safe and should not be
   checked.  */
static void
mpx_process_stmt (gimple_stmt_iterator *iter, tree node,
		 location_t loc, tree dirflag,
		 tree access_offs, tree access_size,
		 bool safe)
{
  tree node_type = TREE_TYPE (node);
  tree size = access_size ? access_size : TYPE_SIZE_UNIT (node_type);
  tree addr_first = NULL_TREE; /* address of the first accessed byte */
  tree addr_last = NULL_TREE; /* address of the last accessed byte */
  tree ptr = NULL_TREE; /* a pointer used for dereference */
  tree bounds = NULL_TREE;

  switch (TREE_CODE (node))
    {
    case ARRAY_REF:
    case COMPONENT_REF:
      {
	bool bitfield;
	tree elt;

	if (safe)
	  {
	    /* We are not going to generate any checks, so do not
	       generate bounds as well.  */
	    addr_first = mpx_build_addr_expr (node);
	    break;
	  }

	mpx_parse_array_and_component_ref (node, &ptr, &elt, &safe,
					  &bitfield, &bounds, iter, false,
					  false);

	/* Break if there is no dereference and operation is safe.  */

	if (bitfield)
          {
            tree field = TREE_OPERAND (node, 1);

            if (TREE_CODE (DECL_SIZE_UNIT (field)) == INTEGER_CST)
              size = DECL_SIZE_UNIT (field);

	    if (elt)
	      elt = mpx_build_addr_expr (elt);
            addr_first = fold_convert_loc (loc, ptr_type_node, elt ? elt : ptr);
            addr_first = fold_build_pointer_plus_loc (loc,
						      addr_first,
						      byte_position (field));
          }
        else
          addr_first = mpx_build_addr_expr (node);
      }
      break;

    case INDIRECT_REF:
      ptr = TREE_OPERAND (node, 0);
      addr_first = ptr;
      break;

    case MEM_REF:
      ptr = TREE_OPERAND (node, 0);
      addr_first = mpx_build_addr_expr (node);
      break;

    case TARGET_MEM_REF:
      ptr = TMR_BASE (node);
      addr_first = mpx_build_addr_expr (node);
      break;

    case ARRAY_RANGE_REF:
      printf("ARRAY_RANGE_REF\n");
      debug_gimple_stmt(gsi_stmt(*iter));
      debug_tree(node);
      gcc_unreachable ();
      break;

    case BIT_FIELD_REF:
      {
	tree offs, rem, bpu;

	gcc_assert (!access_offs);
	gcc_assert (!access_size);

	bpu = fold_convert (size_type_node, bitsize_int (BITS_PER_UNIT));
	offs = fold_convert (size_type_node, TREE_OPERAND (node, 2));
	rem = size_binop_loc (loc, TRUNC_MOD_EXPR, offs, bpu);
	offs = size_binop_loc (loc, TRUNC_DIV_EXPR, offs, bpu);

	size = fold_convert (size_type_node, TREE_OPERAND (node, 1));
        size = size_binop_loc (loc, PLUS_EXPR, size, rem);
        size = size_binop_loc (loc, CEIL_DIV_EXPR, size, bpu);
        size = fold_convert (size_type_node, size);

	mpx_process_stmt (iter, TREE_OPERAND (node, 0), loc,
			 dirflag, offs, size, safe);
	return;
      }
      break;

    case VAR_DECL:
    case RESULT_DECL:
    case PARM_DECL:
      if (dirflag != integer_one_node
	  || DECL_REGISTER (node))
	return;

      safe = true;
      addr_first = mpx_build_addr_expr (node);
      break;

    default:
      return;
    }

  /* If addr_last was not computed then use (addr_first + size - 1)
     expression to compute it.  */
  if (!addr_last)
    {
      addr_last = fold_build_pointer_plus_loc (loc, addr_first, size);
      addr_last = fold_build_pointer_plus_hwi_loc (loc, addr_last, -1);
    }

  /* Shift both first_addr and last_addr by access_offs if specified.  */
  if (access_offs)
    {
      addr_first = fold_build_pointer_plus_loc (loc, addr_first, access_offs);
      addr_last = fold_build_pointer_plus_loc (loc, addr_last, access_offs);
    }

  /* Generate bndcl/bndcu checks if memory access is not safe.  */
  if (!safe)
    {
      gimple_stmt_iterator stmt_iter = *iter;

      if (!bounds)
	bounds = mpx_find_bounds (ptr, iter);

      mpx_check_mem_access (addr_first, addr_last, bounds,
			   stmt_iter, loc, dirflag);
    }

  /* We need to generate bndstx in case pointer is stored.  */
  if (dirflag == integer_one_node && mpx_type_has_pointer (node_type))
    {
      gimple stmt = gsi_stmt (*iter);
      tree rhs1 = gimple_assign_rhs1 (stmt);
      enum tree_code rhs_code = gimple_assign_rhs_code (stmt);

      if (get_gimple_rhs_class (rhs_code) == GIMPLE_SINGLE_RHS)
	mpx_walk_pointer_assignments (node, rhs1, iter, mpx_copy_bounds_for_assign);
      else
	{
	  bounds = mpx_compute_bounds_for_assignment (NULL_TREE, stmt);
	  mpx_build_bndstx (addr_first, rhs1, bounds, iter);
	}
    }
}

/* Some code transformation made during MPX pass may put
   code into inconsistent state.  Here we find and fix
   such flaws.  */
void
mpx_fix_cfg ()
{
  basic_block bb;
  gimple_stmt_iterator i;

  /* We could insert some code right after stmt which ends bb.
     We wanted to put this code on fallthru edge but did not
     add new edges from the beginning because it may cause new
     phi node creation which may be incorrect due to incomplete
     bound phi nodes.  */
  FOR_ALL_BB (bb)
    for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
      {
	gimple stmt = gsi_stmt (i);
	gimple_stmt_iterator next = i;

	gsi_next (&next);

	if (stmt_ends_bb_p (stmt)
	    && !gsi_end_p (next))
	  {
	    edge fall = find_fallthru_edge (bb->succs);
	    basic_block dest = NULL;
	    int flags = 0;

	    gcc_assert (fall);

	    /* We cannot split abnormal edge.  Therefore we
	       store its params, make it regular and then
	       rebuild abnormal edge after split.  */
	    if (fall->flags & EDGE_ABNORMAL)
	      {
		flags = fall->flags & ~EDGE_FALLTHRU;
		dest = fall->dest;

		fall->flags &= ~EDGE_COMPLEX;
	      }

	    while (!gsi_end_p (next))
	      {
		gimple next_stmt = gsi_stmt (next);
		gsi_remove (&next, false);
		gsi_insert_on_edge (fall, next_stmt);
	      }

	    gsi_commit_edge_inserts ();

	    /* Re-create abnormal edge.  */
	    if (dest)
	      make_edge (bb, dest, flags);
	  }
      }
}

/* This function requests intrumentation for all statements
   working with memory, calls and rets.  It also removes
   excess statements from static initializers.  */
static void
mpx_instrument_function (void)
{
  basic_block bb, next;
  gimple_stmt_iterator i;
  enum gimple_rhs_class grhs_class;
  bool safe = lookup_attribute ("mpx ctor", DECL_ATTRIBUTES (cfun->decl));

  bb = ENTRY_BLOCK_PTR ->next_bb;
  do
    {
      next = bb->next_bb;
      for (i = gsi_start_bb (bb); !gsi_end_p (i); )
        {
          gimple s = gsi_stmt (i);

	  /* Skip statement marked to not be instrumented.  */
	  if (mpx_marked_stmt_p (s))
	    {
	      gsi_next (&i);
	      continue;
	    }

          switch (gimple_code (s))
            {
            case GIMPLE_ASSIGN:
	      mpx_process_stmt (&i, gimple_assign_lhs (s),
			       gimple_location (s), integer_one_node,
			       NULL_TREE, NULL_TREE, safe);
	      mpx_process_stmt (&i, gimple_assign_rhs1 (s),
			       gimple_location (s), integer_zero_node,
			       NULL_TREE, NULL_TREE, safe);
	      grhs_class = get_gimple_rhs_class (gimple_assign_rhs_code (s));
	      if (grhs_class == GIMPLE_BINARY_RHS)
		mpx_process_stmt (&i, gimple_assign_rhs2 (s),
				 gimple_location (s), integer_zero_node,
				 NULL_TREE, NULL_TREE, safe);
              break;

            case GIMPLE_RETURN:
              if (gimple_return_retval (s) != NULL_TREE)
                {
                  mpx_process_stmt (&i, gimple_return_retval (s),
				   gimple_location (s),
				   integer_zero_node,
				   NULL_TREE, NULL_TREE, safe);

		  /* Additionall we need to add bounds
		     to return statement.  */
		  mpx_add_bounds_to_ret_stmt (&i);
                }
              break;

	    case GIMPLE_CALL:
	      mpx_add_bounds_to_call_stmt (&i);
	      break;

            default:
              ;
            }

	  gsi_next (&i);

	  /* We do not need any statements in MPX static initializer except
	     created in MPX pass.  */
	  if (lookup_attribute ("mpx ctor", DECL_ATTRIBUTES (cfun->decl))
	      && gimple_code (s) == GIMPLE_ASSIGN)
	    {
	      gimple_stmt_iterator del_iter = gsi_for_stmt (s);
	      gsi_remove (&del_iter, true);
	      unlink_stmt_vdef (s);
	      release_defs(s);
	    }
        }
      bb = next;
    }
  while (bb);
}

/* Initialize pass. */
static void
mpx_init (void)
{
  basic_block bb;
  gimple_stmt_iterator i;

  for (bb = ENTRY_BLOCK_PTR ->next_bb; bb; bb = bb->next_bb)
    for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
      mpx_unmark_stmt (gsi_stmt (i));

  mpx_invalid_bounds = pointer_set_create ();
  mpx_completed_bounds_set = pointer_set_create ();
  mpx_reg_bounds = pointer_map_create ();
  mpx_reg_addr_bounds = pointer_map_create ();
  mpx_incomplete_bounds_map = pointer_map_create ();
  if (mpx_rtx_bounds)
    pointer_map_destroy (mpx_rtx_bounds);
  mpx_rtx_bounds = pointer_map_create ();
  mpx_abnormal_phi_copies = htab_create_ggc (31, tree_map_base_hash,
					     tree_vec_map_eq, NULL);

  entry_block = NULL;
  zero_bounds = NULL_TREE;
  none_bounds = NULL_TREE;
  incomplete_bounds = integer_zero_node;
  tmp_var = NULL_TREE;
  size_tmp_var = NULL_TREE;

  mpx_uintptr_type = lang_hooks.types.type_for_mode (ptr_mode, true);
}

/* Finalize MPX instrumentation pass. */
static void
mpx_fini (void)
{
  pointer_set_destroy (mpx_invalid_bounds);
  pointer_set_destroy (mpx_completed_bounds_set);
  pointer_map_destroy (mpx_reg_bounds);
  pointer_map_destroy (mpx_reg_addr_bounds);
  pointer_map_destroy (mpx_incomplete_bounds_map);
}

/* Main MPX instrumentation pass function. */
static unsigned int
mpx_execute (void)
{
  mpx_init ();

  mpx_instrument_function ();

  mpx_fix_cfg ();

  mpx_fini ();

  return 0;
}

/* MPX instrumentation pass gate.  */
static bool
mpx_gate (void)
{
  return flag_mpx != 0
    && !lookup_attribute ("bnd_legacy", DECL_ATTRIBUTES (cfun->decl));
}

/* Comparator for pol_item structures I1 and I2 to be used
   to find items with equal var.  Also used for polynomial
   sorting.  */
int
mpx_pol_item_compare (const void *i1, const void *i2)
{
  const struct pol_item *p1 = (const struct pol_item *)i1;
  const struct pol_item *p2 = (const struct pol_item *)i2;

  if (p1->var == p2->var)
    return 0;
  else if (p1->var > p2->var)
    return 1;
  else
    return -1;
}

/* Find plynomial item in ADDR with var equal to VAR
   and return its index.  Return -1 if item was not
   found.  */
int
mpx_pol_find (address_t &addr, tree var)
{
  int left = 0;
  int right = addr.pol.length () - 1;
  int n;

  while (right >= left)
    {
      n = (left + right) / 2;

      if (addr.pol[n].var == var
	  || (var && addr.pol[n].var
	      && TREE_CODE (var) == ADDR_EXPR
	      && TREE_CODE (addr.pol[n].var) == ADDR_EXPR
	      && TREE_OPERAND (var, 0) == TREE_OPERAND (addr.pol[n].var, 0)))
	return n;
      else if (addr.pol[n].var > var)
	right = n - 1;
      else
	left = n + 1;
    }

  return -1;
}

/* Return constant CST extended to size type.  */
tree
mpx_extend_const (tree cst)
{
  if (TYPE_PRECISION (TREE_TYPE (cst)) < TYPE_PRECISION (size_type_node))
    return build_int_cst_type (size_type_node, tree_low_cst (cst, 0));

  return cst;
}

/* Add polynomial item CST * VAR to ADDR.  */
void
mpx_add_addr_item (address_t &addr, tree cst, tree var)
{
  int n = mpx_pol_find (addr, var);

  cst = mpx_extend_const (cst);

  if (n < 0)
    {
      struct pol_item item;
      item.cst = cst;
      item.var = var;

      addr.pol.safe_push (item);
      addr.pol.qsort (&mpx_pol_item_compare);
    }
  else
    {
      addr.pol[n].cst = fold_build2 (PLUS_EXPR, TREE_TYPE (addr.pol[n].cst),
				     addr.pol[n].cst, cst);
      if (TREE_CODE (addr.pol[n].cst) == INTEGER_CST
	  && integer_zerop (addr.pol[n].cst))
	addr.pol.ordered_remove (n);
    }
}

/* Subtract polynomial item CST * VAR from ADDR.  */
void
mpx_sub_addr_item (address_t &addr, tree cst, tree var)
{
  int n = mpx_pol_find (addr, var);

  cst = mpx_extend_const (cst);

  if (n < 0)
    {
      struct pol_item item;
      item.cst = fold_build2 (MINUS_EXPR, TREE_TYPE (cst),
			      integer_zero_node, cst);
      item.var = var;

      addr.pol.safe_push (item);
      addr.pol.qsort (&mpx_pol_item_compare);
    }
  else
    {
      addr.pol[n].cst = fold_build2 (MINUS_EXPR, TREE_TYPE (addr.pol[n].cst),
				     addr.pol[n].cst, cst);
      if (TREE_CODE (addr.pol[n].cst) == INTEGER_CST
	  && integer_zerop (addr.pol[n].cst))
	addr.pol.ordered_remove (n);
    }
}

/* Add address DELTA to ADDR.  */
void
mpx_add_addr_addr (address_t &addr, address_t &delta)
{
  unsigned int i;
  for (i = 0; i < delta.pol.length (); i++)
    mpx_add_addr_item (addr, delta.pol[i].cst, delta.pol[i].var);
}

/* Subtract address DELTA from ADDR.  */
void
mpx_sub_addr_addr (address_t &addr, address_t &delta)
{
  unsigned int i;
  for (i = 0; i < delta.pol.length (); i++)
    mpx_sub_addr_item (addr, delta.pol[i].cst, delta.pol[i].var);
}

/* Mutiply address ADDR by integer constant MULT.  */
void
mpx_mult_addr (address_t &addr, tree mult)
{
  unsigned int i;
  for (i = 0; i < addr.pol.length (); i++)
    addr.pol[i].cst = fold_build2 (MULT_EXPR, TREE_TYPE (addr.pol[i].cst),
				   addr.pol[i].cst, mult);
}

/* Return 1 if we may prove ADDR has a constant value with
   determined sign, which is put into *SIGN.  Otherwise
   return 0.  */
bool
mpx_is_constant_addr (const address_t &addr, int *sign)
{
  *sign = 0;

  if (addr.pol.length () == 0)
    return true;
  else if (addr.pol.length () > 1)
    return false;
  else if (addr.pol[0].var)
    return false;
  else if (integer_zerop (addr.pol[0].cst))
    *sign = 0;
  else if  (tree_int_cst_sign_bit (addr.pol[0].cst))
    *sign = -1;
  else
    *sign = 1;

  return true;
}

/* Dump ADDR into dump_file.  */
void
mpx_print_addr (const address_t &addr)
{
  unsigned int n = 0;
  for (n = 0; n < addr.pol.length (); n++)
    {
      if (n > 0)
	fprintf (dump_file, " + ");

      if (addr.pol[n].var == NULL_TREE)
	print_generic_expr (dump_file, addr.pol[n].cst, 0);
      else
	{
	  if (TREE_CODE (addr.pol[n].cst) != INTEGER_CST
	      || !integer_onep (addr.pol[n].cst))
	    {
	      print_generic_expr (dump_file, addr.pol[n].cst, 0);
	      fprintf (dump_file, " * ");
	    }
	  print_generic_expr (dump_file, addr.pol[n].var, 0);
	}
    }
}

/* Compute value of PTR and put it into address RES.
   PTR has to be ADDR_EXPR.  */
void
mpx_collect_addr_value (tree ptr, address_t &res)
{
  tree obj = TREE_OPERAND (ptr, 0);
  address_t addr;

  switch (TREE_CODE (obj))
    {
    case INDIRECT_REF:
      mpx_collect_value (TREE_OPERAND (obj, 0), res);
      break;

    case MEM_REF:
      mpx_collect_value (TREE_OPERAND (obj, 0), res);
      addr.pol.create (0);
      mpx_collect_value (TREE_OPERAND (obj, 1), addr);
      mpx_add_addr_addr (res, addr);
      addr.pol.release ();
      break;

    case ARRAY_REF:
      mpx_collect_value (build_fold_addr_expr (TREE_OPERAND (obj, 0)), res);
      addr.pol.create (0);
      mpx_collect_value (TREE_OPERAND (obj, 1), addr);
      mpx_mult_addr (addr, array_ref_element_size (obj));
      mpx_add_addr_addr (res, addr);
      addr.pol.release ();
      break;

    case COMPONENT_REF:
      {
	tree str = TREE_OPERAND (obj, 0);
	tree field = TREE_OPERAND (obj, 1);
	mpx_collect_value (build_fold_addr_expr (str), res);
	addr.pol.create (0);
	mpx_collect_value (component_ref_field_offset (obj), addr);
	mpx_add_addr_addr (res, addr);
	addr.pol.release ();
	if (DECL_FIELD_BIT_OFFSET (field))
	  {
	    addr.pol.create (0);
	    mpx_collect_value (fold_build2 (TRUNC_DIV_EXPR, size_type_node,
					    DECL_FIELD_BIT_OFFSET (field),
					    size_int (BITS_PER_UNIT)),
			   addr);
	    mpx_add_addr_addr (res, addr);
	    addr.pol.release ();
	  }
      }
      break;

    default:
      mpx_add_addr_item (res, integer_one_node, ptr);
      break;
    }
}

/* Compute value of PTR and put it into address RES.  */
void
mpx_collect_value (tree ptr, address_t &res)
{
  gimple def_stmt;
  enum gimple_code code;
  enum tree_code rhs_code;
  address_t addr;
  tree rhs1;

  if (TREE_CODE (ptr) == INTEGER_CST)
    {
      mpx_add_addr_item (res, ptr, NULL);
      return;
    }
  else if (TREE_CODE (ptr) == ADDR_EXPR)
    {
      mpx_collect_addr_value (ptr, res);
      return;
    }
  else if (TREE_CODE (ptr) != SSA_NAME)
    {
      mpx_add_addr_item (res, integer_one_node, ptr);
      return;
    }

  /* Now we handle the case when polynomial is computed
     for SSA NAME.  */
  def_stmt = SSA_NAME_DEF_STMT (ptr);
  code = gimple_code (def_stmt);

  /* Currently we do not walk through statements other
     than assignment.  */
  if (code != GIMPLE_ASSIGN)
    {
      mpx_add_addr_item (res, integer_one_node, ptr);
      return;
    }

  rhs_code = gimple_assign_rhs_code (def_stmt);
  rhs1 = gimple_assign_rhs1 (def_stmt);

  switch (rhs_code)
    {
    case SSA_NAME:
    case INTEGER_CST:
    case ADDR_EXPR:
      mpx_collect_value (rhs1, res);
      break;

    case PLUS_EXPR:
    case POINTER_PLUS_EXPR:
      mpx_collect_value (rhs1, res);
      addr.pol.create (0);
      mpx_collect_value (gimple_assign_rhs2 (def_stmt), addr);
      mpx_add_addr_addr (res, addr);
      addr.pol.release ();
      break;

    case MINUS_EXPR:
      mpx_collect_value (rhs1, res);
      addr.pol.create (0);
      mpx_collect_value (gimple_assign_rhs2 (def_stmt), addr);
      mpx_sub_addr_addr (res, addr);
      addr.pol.release ();
      break;

    case MULT_EXPR:
      if (TREE_CODE (rhs1) == SSA_NAME
	  && TREE_CODE (gimple_assign_rhs2 (def_stmt)) == INTEGER_CST)
	{
	  mpx_collect_value (rhs1, res);
	  mpx_mult_addr (res, gimple_assign_rhs2 (def_stmt));
	}
      else if (TREE_CODE (gimple_assign_rhs2 (def_stmt)) == SSA_NAME
	       && TREE_CODE (rhs1) == INTEGER_CST)
	{
	  mpx_collect_value (gimple_assign_rhs2 (def_stmt), res);
	  mpx_mult_addr (res, rhs1);
	}
      else
	mpx_add_addr_item (res, integer_one_node, ptr);
      break;

    default:
      mpx_add_addr_item (res, integer_one_node, ptr);
      break;
    }
}

/* Fill check_info structure *CI with information about
   check STMT.  */
void
mpx_fill_check_info (gimple stmt, struct check_info *ci)
{
  ci->addr.pol.create (0);
  ci->bounds = gimple_call_arg (stmt, 0);
  mpx_collect_value (gimple_call_arg (stmt, 1), ci->addr);
  ci->type = (gimple_call_fndecl (stmt) == mpx_checkl_fndecl
	     ? CHECK_LOWER_BOUND
	     : CHECK_UPPER_BOUND);
  ci->stmt = stmt;
}

/* Release structures holding check information
   for current function.  */
void
mpx_release_check_info (void)
{
  unsigned int n, m;

  if (check_infos.exists ())
    {
      for (n = 0; n < check_infos.length (); n++)
	{
	  for (m = 0; m < check_infos[n].checks.length (); m++)
	    if (check_infos[n].checks[m].addr.pol.exists ())
	      check_infos[n].checks[m].addr.pol.release ();
	  check_infos[n].checks.release ();
	}
      check_infos.release ();
    }
}

/* Create structures to hold check information
   for current function.  */
void
mpx_init_check_info (void)
{
  struct bb_checks empty_bbc;
  int n;

  empty_bbc.checks.create (0);
  empty_bbc.checks.release ();

  mpx_release_check_info ();

  check_infos.create (n_basic_blocks);
  for (n = 0; n < n_basic_blocks; n++)
    {
      check_infos.safe_push (empty_bbc);
      check_infos.last ().checks.create (0);
    }
}

/* Find all checks in current function and store info about them
   in check_infos.  */
void
mpx_gather_checks_info (void)
{
  basic_block bb;
  gimple_stmt_iterator i;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Gathering information about checks...\n");

  mpx_init_check_info ();

  bb = ENTRY_BLOCK_PTR ->next_bb;
  FOR_EACH_BB (bb)
    {
      struct bb_checks *bbc = &check_infos[bb->index];

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Searching checks in BB%d...\n", bb->index);

      for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
        {
	  gimple stmt = gsi_stmt (i);

	  if (gimple_code (stmt) != GIMPLE_CALL)
	    continue;

	  if (gimple_call_fndecl (stmt) == mpx_checkl_fndecl
	      || gimple_call_fndecl (stmt) == mpx_checku_fndecl)
	    {
	      struct check_info ci;

	      mpx_fill_check_info (stmt, &ci);
	      bbc->checks.safe_push (ci);

	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "Adding check information:\n");
		  fprintf (dump_file, "  bounds: ");
		  print_generic_expr (dump_file, ci.bounds, 0);
		  fprintf (dump_file, "\n  address: ");
		  mpx_print_addr (ci.addr);
		  fprintf (dump_file, "\n  check: ");
		  print_gimple_stmt (dump_file, stmt, 0, 0);
		}
	    }
	}
    }
}

/* Return 1 if check CI against BOUNDS always pass,
   -1 if check CI against BOUNDS always fails and
   0 if we cannot compute check result.  */
int
mpx_get_check_result (struct check_info *ci, tree bounds)
{
  gimple bnd_def;
  address_t bound_val;
  int sign, res = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Trying to compute result of the check\n");
      fprintf (dump_file, "  check: ");
      print_gimple_stmt (dump_file, ci->stmt, 0, 0);
      fprintf (dump_file, "  address: ");
      mpx_print_addr (ci->addr);
      fprintf (dump_file, "\n  bounds: ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, "\n");
    }

  if (TREE_CODE (bounds) != SSA_NAME)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: bounds tree code is not ssa_name\n");
      return 0;
    }

  if (bounds == zero_bounds)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always pass with zero bounds\n");
      return 1;
    }

  if (bounds == none_bounds)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always fails with none bounds\n");
      return -1;
    }

  bnd_def = SSA_NAME_DEF_STMT (bounds);
  /* Currently we handle cases when bounds are result of bndmk
     or loaded static bounds var.  */
  if (gimple_code (bnd_def) == GIMPLE_CALL
      && gimple_call_fndecl (bnd_def) == mpx_bndmk_fndecl)
    {
      bound_val.pol.create (0);
      mpx_collect_value (gimple_call_arg (bnd_def, 0), bound_val);
      if (ci->type == CHECK_UPPER_BOUND)
	{
	  address_t size_val;
	  size_val.pol.create (0);
	  mpx_collect_value (gimple_call_arg (bnd_def, 1), size_val);
	  mpx_add_addr_addr (bound_val, size_val);
	  size_val.pol.release ();
	  mpx_add_addr_item (bound_val, integer_minus_one_node, NULL);
	}
    }
  else if (gimple_code (bnd_def) == GIMPLE_ASSIGN
	   && TREE_CODE (gimple_assign_rhs1 (bnd_def)) == VAR_DECL)
    {
      struct tree_map *res, in;
      tree var = gimple_assign_rhs1 (bnd_def);
      tree size;
      in.base.from = var;
      in.hash = htab_hash_pointer (var);

      res = (struct tree_map *) htab_find_with_hash (mpx_static_var_bounds_r,
						     &in, in.hash);
      gcc_assert (res);

      bound_val.pol.create (0);
      mpx_collect_value (mpx_build_addr_expr (res->to), bound_val);
      if (ci->type == CHECK_UPPER_BOUND)
	{
	  if (TREE_CODE (res->to) == VAR_DECL)
	    {
	      if (DECL_SIZE (res->to)
		  && !mpx_variable_size_type (TREE_TYPE (res->to)))
		size = DECL_SIZE_UNIT (res->to);
	      else
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    fprintf (dump_file, "  result: cannot compute bounds\n");
		  return 0;
		}
	    }
	  else
	    {
	      gcc_assert (TREE_CODE (res->to) == STRING_CST);
	      size = build_int_cst (size_type_node,
				    TREE_STRING_LENGTH (res->to));
	    }

	  address_t size_val;
	  size_val.pol.create (0);
	  mpx_collect_value (size, size_val);
	  mpx_add_addr_addr (bound_val, size_val);
	  size_val.pol.release ();
	  mpx_add_addr_item (bound_val, integer_minus_one_node, NULL);
	}
    }
  else
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: cannot compute bounds\n");
      return 0;
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "  bound value: ");
      mpx_print_addr (bound_val);
      fprintf (dump_file, "\n");
    }

  mpx_sub_addr_addr (bound_val, ci->addr);

  if (!mpx_is_constant_addr (bound_val, &sign))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: cannot compute result\n");

      res = 0;
    }
  else if (sign == 0
	   || (ci->type == CHECK_UPPER_BOUND && sign > 0)
	   || (ci->type == CHECK_LOWER_BOUND && sign < 0))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always pass\n");

      res = 1;
    }
  else
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always fail\n");

      res = -1;
    }

  bound_val.pol.release ();

  return res;
}

/* Try to compare bounds value and address value
   used in the check CI.  If we can prove that check
   always pass then remove it.  */
void
mpx_remove_check_if_pass (struct check_info *ci)
{
  int result = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Trying to remove check: ");
      print_gimple_stmt (dump_file, ci->stmt, 0, 0);
    }

  result = mpx_get_check_result (ci, ci->bounds);

  if (result == 1)
    {
      gimple_stmt_iterator i = gsi_for_stmt (ci->stmt);

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  action: delete check (always pass)\n");

      gsi_remove (&i, true);
      unlink_stmt_vdef (ci->stmt);
      release_defs (ci->stmt);
      ci->stmt = NULL;
    }
  else if (result == -1)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  action: keep check (always fail)\n");
    }
  else if (result == 0)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  action: keep check (cannot compute result)\n");
    }
}

/* For bounds used in CI check if bounds are produced by
   intersection and we may use outer bounds instead.  If
   transformation is possible then fix check statement and
   recompute its info.  */
void
mpx_use_outer_bounds_if_possible (struct check_info *ci)
{
  gimple bnd_def;
  tree bnd1, bnd2, bnd_res = NULL;
  int check_res1, check_res2;

  if (TREE_CODE (ci->bounds) != SSA_NAME)
    return;

  bnd_def = SSA_NAME_DEF_STMT (ci->bounds);
  if (gimple_code (bnd_def) != GIMPLE_CALL
      || gimple_call_fndecl (bnd_def) != mpx_intersect_fndecl)
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Check if bounds intersection is redundant: \n");
      fprintf (dump_file, "  check: ");
      print_gimple_stmt (dump_file, ci->stmt, 0, 0);
      fprintf (dump_file, "  intersection: ");
      print_gimple_stmt (dump_file, bnd_def, 0, 0);
      fprintf (dump_file, "\n");
    }

  bnd1 = gimple_call_arg (bnd_def, 0);
  bnd2 = gimple_call_arg (bnd_def, 1);

  check_res1 = mpx_get_check_result (ci, bnd1);
  check_res2 = mpx_get_check_result (ci, bnd2);
  if (check_res1 == 1)
    bnd_res = bnd2;
  else if (check_res1 == -1)
    bnd_res = bnd1;
  else if (check_res2 == 1)
    bnd_res = bnd1;
  else if (check_res2 == -1)
    bnd_res = bnd2;

  if (bnd_res)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "  action: use ");
	  print_generic_expr (dump_file, bnd2, 0);
	  fprintf (dump_file, " instead of ");
	  print_generic_expr (dump_file, ci->bounds, 0);
	}

      ci->bounds = bnd_res;
      gimple_call_set_arg (ci->stmt, 0, bnd_res);
      update_stmt (ci->stmt);
    }
}

/*  Try to find checks whose bounds were produced by intersection
    which does not affect check result.  In such check outer bounds
    are used instead.  It allows to remove excess intersections
    and helps to compare checks.  */
void
mpx_remove_excess_intersections (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for redundant bounds intersections...\n");

  bb = ENTRY_BLOCK_PTR ->next_bb;
  FOR_EACH_BB (bb)
    {
      struct bb_checks *bbc = &check_infos[bb->index];
      unsigned int no;

      /* Iterate throw all found checks in BB.  */
      for (no = 0; no < bbc->checks.length (); no++)
	if (bbc->checks[no].stmt)
	  mpx_use_outer_bounds_if_possible (&bbc->checks[no]);
    }
}

/*  Try to remove all checks which are known to alwyas pass.  */
void
mpx_remove_constant_checks (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for redundant checks...\n");

  bb = ENTRY_BLOCK_PTR ->next_bb;
  FOR_EACH_BB (bb)
    {
      struct bb_checks *bbc = &check_infos[bb->index];
      unsigned int no;

      /* Iterate throw all found checks in BB.  */
      for (no = 0; no < bbc->checks.length (); no++)
	if (bbc->checks[no].stmt)
	  mpx_remove_check_if_pass (&bbc->checks[no]);
    }
}

/* Compare two checks CI1 and CI2 to find redundant one.
   CI1 is known to dominate CI2.  POSTDOM indicated if
   CI2 postdominateds CI1.

   Few conditions are checked to find redundant check:
     1. Checks has the same type
     2. Checks use the same bounds
     3. One check fail means other check fail
     4. Stronger check is always executed if weaker one is executed

   If redundant check is found it is removed. If removed check is CI1
   then CI2 is moved to CI1's position to avoid bound violation happened
   before check is executed.  */
void
mpx_compare_checks (struct check_info *ci1, struct check_info *ci2, bool postdom)
{
  address_t delta;
  int sign;

  if (ci1->type != ci2->type)
    return;

  if (ci1->bounds != ci2->bounds)
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "  Comparing checks...\n");
      fprintf (dump_file, "    First check: ");
      print_gimple_stmt (dump_file, ci1->stmt, 0, 0);
      fprintf (dump_file, "    Second check: ");
      print_gimple_stmt (dump_file, ci2->stmt, 0, 0);
    }

  delta.pol = ci1->addr.pol.copy ();
  mpx_sub_addr_addr (delta, ci2->addr);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "    Delta: ");
      mpx_print_addr (delta);
      fprintf (dump_file, "\n");
    }

  if (!mpx_is_constant_addr (delta, &sign))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "    Action: skip (delta is not constant)\n");
    }
  else
    {
      if (sign)
	{
	  if ((sign > 0 && ci1->type == CHECK_UPPER_BOUND)
	      || (sign < 0 && ci1->type == CHECK_LOWER_BOUND))
	    {
	      gimple_stmt_iterator i = gsi_for_stmt (ci2->stmt);

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "    Action: delete the second check\n");

	      gsi_remove (&i, true);
	      unlink_stmt_vdef (ci2->stmt);
	      release_defs (ci2->stmt);
	      ci2->stmt = NULL;
	    }
	  else if (postdom)
	    {
	      gimple_stmt_iterator i = gsi_for_stmt (ci2->stmt);
	      gimple_seq seq = NULL;
	      tree addr = gimple_call_arg (ci1->stmt, 1);
	      unsigned int n;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "    Action: replace the first check with the second one\n");

	      gsi_remove (&i, true);
	      unlink_stmt_vdef (ci2->stmt);
	      release_defs (ci2->stmt);
	      ci2->stmt = NULL;

	      for (n = 0; n < delta.pol.length (); n++)
		if (delta.pol[n].var == NULL)
		  {
		    tree tmp = fold_build2 (MINUS_EXPR,
					    TREE_TYPE (delta.pol[n].cst),
					    integer_zero_node,
					    delta.pol[n].cst);
		    addr = fold_build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
					addr, convert_to_ptrofftype (tmp));
		  }
		else if (integer_onep (delta.pol[n].cst))
		  {
		    tree tmp = fold_build2 (MINUS_EXPR,
					    TREE_TYPE (delta.pol[n].var),
					    integer_zero_node,
					    delta.pol[n].var);
		    addr = build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
				   addr, convert_to_ptrofftype (tmp));
		  }
		else if (tree_int_cst_compare (delta.pol[n].cst,
					       integer_minus_one_node) == 0)
		  addr = build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
				 addr, convert_to_ptrofftype (delta.pol[n].var));
		else
		  {
		    tree tmp = fold_build2 (MULT_EXPR,
					    TREE_TYPE (delta.pol[n].var),
					    delta.pol[n].var,
					    delta.pol[n].cst);
		    tmp = fold_build2 (MINUS_EXPR, TREE_TYPE (tmp),
				       integer_zero_node, tmp);
		    addr = fold_build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
					addr, convert_to_ptrofftype (tmp));
		  }

	      addr = force_gimple_operand (unshare_expr (addr), &seq,
					   true, NULL_TREE);

	      i = gsi_for_stmt (ci1->stmt);
	      gsi_insert_seq_before (&i, seq, GSI_CONTINUE_LINKING);
	      gimple_call_set_arg (ci1->stmt, 1, addr);
	      update_stmt (ci1->stmt);

	      ci1->addr.pol.release ();
	      mpx_fill_check_info (ci1->stmt, ci1);
	    }
	  else
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "    Action: skip (the first check is not post-dominanted by the second check)\n");
	    }
	}
      else
	{
	  gimple_stmt_iterator i = gsi_for_stmt (ci2->stmt);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "    Action: delete the second check (same addresses)\n");

	  gsi_remove (&i, true);
	  unlink_stmt_vdef (ci2->stmt);
	  release_defs (ci2->stmt);
	  ci2->stmt = NULL;
	}
    }

  delta.pol.release ();
}

/* Here we try to find checks which are covered by other checks
   and thus can be removed.  To do it we simply find all pairs of
   checks where the first check dominates the second one and
   call mpx_compare_checks to find and remove redundant ones.  */
void
mpx_remove_redundant_checks (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for redundant checks...\n");

  bb = ENTRY_BLOCK_PTR ->next_bb;
  FOR_EACH_BB (bb)
    {
      struct bb_checks *bbc = &check_infos[bb->index];
      unsigned int no;

      /* Iterate throw all found checks in BB.  */
      for (no = 0; no < bbc->checks.length (); no++)
	if (bbc->checks[no].stmt)
	  {
	    vec<basic_block> dom_bbs;
	    unsigned bb_no, other;

	    /* Compare check with all other following checks in this BB.  */
	    for (other = no + 1; other < bbc->checks.length (); other++)
	      if (bbc->checks[other].stmt)
		mpx_compare_checks (&bbc->checks[no], &bbc->checks[other],
				    true);

	    /* Now compare with checks in BBs dominated by current one.  */
	    dom_bbs = get_all_dominated_blocks (CDI_DOMINATORS, bb);
	    for (bb_no = 0; bb_no < dom_bbs.length (); bb_no++)
	      {
		struct bb_checks *dom_bbc = &check_infos[dom_bbs[bb_no]->index];

		if (dom_bbs[bb_no] == bb)
		  continue;

		for (other = 0; other < dom_bbc->checks.length (); other++)
		  if (dom_bbc->checks[other].stmt)
		    mpx_compare_checks (&bbc->checks[no],
					&dom_bbc->checks[other],
					dominated_by_p (CDI_POST_DOMINATORS, bb,
							dom_bbs[bb_no]));
	      }
	  }
    }
}

/* Return fast version of string function FNCODE.  */
tree
mpx_get_nobnd_fndecl (enum built_in_function fncode)
{
  /* Check if we are allowed to use fast string functions.  */
  if (!flag_mpx_use_fast_string_functions)
    return NULL_TREE;

  switch (fncode)
    {
    case BUILT_IN_MEMCPY:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMCPY_NOBND);

    case BUILT_IN_MEMPCPY:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMPCPY_NOBND);

    case BUILT_IN_MEMMOVE:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMMOVE_NOBND);

    case BUILT_IN_MEMSET:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMSET_NOBND);

    case BUILT_IN_MPX_MEMCPY_NOCHK:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMCPY_NOBND_NOCHK);

    case BUILT_IN_MPX_MEMPCPY_NOCHK:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMPCPY_NOBND_NOCHK);

    case BUILT_IN_MPX_MEMMOVE_NOCHK:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMMOVE_NOBND_NOCHK);

    case BUILT_IN_MPX_MEMSET_NOCHK:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMSET_NOBND_NOCHK);

    default:
      return NULL_TREE;
    }
}


/* Return no-check version of string function FNCODE.  */
tree
mpx_get_nochk_fndecl (enum built_in_function fncode)
{
  /* Check if we are allowed to use fast string functions.  */
  if (!flag_mpx_use_nochk_string_functions)
    return NULL_TREE;

  switch (fncode)
    {
    case BUILT_IN_MEMCPY:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMCPY_NOCHK);

    case BUILT_IN_MEMPCPY:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMPCPY_NOCHK);

    case BUILT_IN_MEMMOVE:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMMOVE_NOCHK);

    case BUILT_IN_MEMSET:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMSET_NOCHK);

    case BUILT_IN_MPX_MEMCPY_NOBND:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMCPY_NOBND_NOCHK);

    case BUILT_IN_MPX_MEMPCPY_NOBND:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMPCPY_NOBND_NOCHK);

    case BUILT_IN_MPX_MEMMOVE_NOBND:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMMOVE_NOBND_NOCHK);

    case BUILT_IN_MPX_MEMSET_NOBND:
      return builtin_decl_implicit (BUILT_IN_MPX_MEMSET_NOBND_NOCHK);

    default:
      return NULL_TREE;
    }
}

/* Find memcpy, mempcpy, memmove and memset calls, perform
   checks before call and then call no_chk version of
   functions.  We do it on O2 to enable inlining of these
   functions during expand.

   Also try to find memcpy, mempcpy, memmove and memset calls
   which are known to not write pointers to memory and use
   faster function versions for them.  */
void
mpx_optimize_string_function_calls (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for replacable string function calls...\n");

  bb = ENTRY_BLOCK_PTR ->next_bb;
  FOR_EACH_BB (bb)
    {
      gimple_stmt_iterator i;

      for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
        {
	  gimple stmt = gsi_stmt (i);
	  tree fndecl;

	  if (gimple_code (stmt) != GIMPLE_CALL)
	    continue;

	  fndecl = gimple_call_fndecl (stmt);

	  if (!fndecl || DECL_BUILT_IN_CLASS (fndecl) != BUILT_IN_NORMAL)
	    continue;

	  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMCPY
	      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMPCPY
	      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMMOVE
	      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMSET)
	    {
	      tree dst = gimple_call_arg (stmt, 0);
	      tree dst_bnd = gimple_call_arg (stmt, 1);
	      bool is_memset = DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMSET;
	      tree size = gimple_call_arg (stmt, is_memset ? 3 : 4);
	      tree fndecl_nochk;
	      gimple_stmt_iterator j;
	      basic_block check_bb;
	      edge fall;
	      address_t size_val;
	      int sign;
	      bool known;

	      /* We may replace call with corresponding __mpx_*_nobnd
		 call in case destination pointer base type is not
		 void or pointer.  */
	      if (POINTER_TYPE_P (TREE_TYPE (dst))
		  && !VOID_TYPE_P (TREE_TYPE (TREE_TYPE (dst)))
		  && !mpx_type_has_pointer (TREE_TYPE (TREE_TYPE (dst))))
		{
		  tree fndecl_nobnd
		    = mpx_get_nobnd_fndecl (DECL_FUNCTION_CODE (fndecl));

		  if (fndecl_nobnd)
		    fndecl = fndecl_nobnd;
		}

	      fndecl_nochk = mpx_get_nochk_fndecl (DECL_FUNCTION_CODE (fndecl));

	      if (fndecl_nochk)
		fndecl = fndecl_nochk;

	      gimple_call_set_fndecl (stmt, fndecl);

	      /* If there is no nochk version of function then
		 do nothing.  Otherwise insert checks before
		 the call.  */
	      if (!fndecl_nochk)
		continue;

	      /* If size passed to call is known and > 0
		 then we may insert checks unconditionally.  */
	      size_val.pol.create (0);
	      mpx_collect_value (size, size_val);
	      known = mpx_is_constant_addr (size_val, &sign);
	      size_val.pol.release ();

	      /* If we are not sure size is not zero then we have
		 to perform runtiome check for size and perform
		 MPX checks only when size is not zero.  */
	      if (!known)
		{
		  gimple check = gimple_build_cond (NE_EXPR,
						    size,
						    size_zero_node,
						    NULL_TREE,
						    NULL_TREE);

		  /* Split block before string function call.  */
		  j = i;
		  gsi_prev (&j);
		  fall = split_block (bb, gsi_stmt (j));
		  bb = fall->src;

		  /* Add size check.  */
		  j = gsi_last_bb (bb);
		  if (gsi_end_p (j))
		    gsi_insert_before (&j, check, GSI_SAME_STMT);
		  else
		    gsi_insert_after (&j, check, GSI_SAME_STMT);

		  /* Create basic block for checks.  */
		  check_bb = create_empty_bb (bb);
		  make_edge (bb, check_bb, EDGE_TRUE_VALUE);
		  make_single_succ_edge (check_bb, fall->dest, EDGE_FALLTHRU);

		  /* Fix edge for splitted bb.  */
		  fall->flags = EDGE_FALSE_VALUE;
		  fall->count = bb->count;
		  fall->probability = REG_BR_PROB_BASE;

		  /* Update dominance info.  */
		  if (dom_info_available_p (CDI_DOMINATORS))
		    {
		      set_immediate_dominator (CDI_DOMINATORS, check_bb, bb);
		      set_immediate_dominator (CDI_DOMINATORS, fall->dest, bb);
		    }

		  /* Update loop info.  */
		  if (current_loops)
		    add_bb_to_loop (check_bb, bb->loop_father);

		  /* Set position for checks.  */
		  j = gsi_last_bb (check_bb);

		  /* The block was splitted and therefore we
		     need to set iterator to its end.   */
		  i = gsi_last_bb (bb);
		}
	      /* If size is known to be zero then no MPX checks
		 should be performed.  */
	      else if (!sign)
		continue;
	      else
		j = i;

	      size = size_binop (MINUS_EXPR, size, size_one_node);
	      if (!is_memset)
		{
		  tree src = gimple_call_arg (stmt, 2);
		  tree src_bnd = gimple_call_arg (stmt, 3);

		  mpx_check_mem_access (src, fold_build_pointer_plus (src, size),
					src_bnd, j, gimple_location (stmt),
					integer_zero_node);
		}

	      mpx_check_mem_access (dst, fold_build_pointer_plus (dst, size),
				    dst_bnd, j, gimple_location (stmt),
				    integer_one_node);

	    }
	}
    }
}

/* MPX pass inserts most of bounds creation code in
   the header of the function.  We want to move bounds
   creation closer to bounds usage to reduce bounds
   lifetime.  We also try to avoid bounds creation
   code on paths where bounds are not used.  */
void
mpx_reduce_bounds_lifetime (void)
{
  basic_block bb = FALLTHRU_EDGE (ENTRY_BLOCK_PTR)->dest;
  gimple_stmt_iterator i;

  for (i = gsi_start_bb (bb); !gsi_end_p (i); )
    {
      gimple dom_use, use_stmt, stmt = gsi_stmt (i);
      basic_block dom_bb;
      ssa_op_iter iter;
      imm_use_iterator use_iter;
      use_operand_p use_p;
      tree op;
      bool want_move = false;
      bool deps = false;

      if (gimple_code (stmt) == GIMPLE_CALL
	  && (gimple_call_fndecl (stmt) == mpx_bndmk_fndecl
	      || (gimple_call_fndecl (stmt) == mpx_arg_bnd_fndecl)))
	want_move = true;

      if (gimple_code (stmt) == GIMPLE_ASSIGN
	  && BOUND_TYPE_P (TREE_TYPE (gimple_assign_lhs (stmt)))
	  && gimple_assign_rhs_code (stmt) == VAR_DECL)
	want_move = true;

      if (!want_move)
	{
	  gsi_next (&i);
	  continue;
	}

      /* Check we do not increase other values lifetime.  */
      FOR_EACH_PHI_OR_STMT_USE (use_p, stmt, iter, SSA_OP_USE)
	{
	  op = USE_FROM_PTR (use_p);

	  if (TREE_CODE (op) == SSA_NAME
	      && gimple_code (SSA_NAME_DEF_STMT (op)) != GIMPLE_NOP)
	    deps = true;
	}

      if (deps)
	{
	  gsi_next (&i);
	  continue;
	}

      /* Check all usages of bounds.  */
      if (gimple_code (stmt) == GIMPLE_CALL)
	op = gimple_call_lhs (stmt);
      else
	{
	  gcc_assert (gimple_code (stmt) == GIMPLE_ASSIGN);
	  op = gimple_assign_lhs (stmt);
	}

      dom_use = NULL;
      dom_bb = NULL;

      FOR_EACH_IMM_USE_STMT (use_stmt, use_iter, op)
	{
	  if (dom_bb &&
	      dominated_by_p (CDI_DOMINATORS,
			      dom_bb, gimple_bb (use_stmt)))
	    {
	      dom_use = use_stmt;
	      dom_bb = NULL;
	    }
	  else if (dom_bb)
	    dom_bb = nearest_common_dominator (CDI_DOMINATORS, dom_bb,
					       gimple_bb (use_stmt));
	  else if (!dom_use)
	    dom_use = use_stmt;
	  else if (stmt_dominates_stmt_p (use_stmt, dom_use))
	    dom_use = use_stmt;
	  else if (!stmt_dominates_stmt_p (dom_use, use_stmt)
		   /* If dom_use and use_stmt are PHI nodes in one BB
		      then it is OK to keep any of them as dom_use.
		      stmt_dominates_stmt_p returns 0 for such
		      combination, so check it here manually.  */
		   && (gimple_code (dom_use) != GIMPLE_PHI
		       || gimple_code (use_stmt) != GIMPLE_PHI
		       || gimple_bb (use_stmt) != gimple_bb (dom_use))
		   )
	    {
	      dom_bb = nearest_common_dominator (CDI_DOMINATORS,
						 gimple_bb (use_stmt),
						 gimple_bb (dom_use));
	      dom_use = NULL;
	    }
	}

      /* In case there is a single use, just move bounds
	 creation to the use.  */
      if (dom_use || dom_bb)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Moving creation of ");
	      print_generic_expr (dump_file, op, 0);
	      fprintf (dump_file, " down to its use.\n");
	    }

	  if (dom_use && gimple_code (dom_use) == GIMPLE_PHI)
	    {
	      dom_bb = get_immediate_dominator (CDI_DOMINATORS,
						gimple_bb (dom_use));
	      dom_use = NULL;
	    }

	  if (dom_bb == bb
	      || (dom_use && gimple_bb (dom_use) == bb))
	    {
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    fprintf (dump_file, "Cannot move statement bacause there is no "
			     "suitable dominator block other than entry block.\n");

		  gsi_next (&i);
	    }
	  else
	    {
	      if (dom_bb)
		{
		  gimple_stmt_iterator last = gsi_last_bb (dom_bb);
		  if (!gsi_end_p (last) && stmt_ends_bb_p (gsi_stmt (last)))
		    gsi_move_before (&i, &last);
		  else
		    gsi_move_after (&i, &last);
		}
	      else
		{
		  gimple_stmt_iterator gsi = gsi_for_stmt (dom_use);
		  gsi_move_before (&i, &gsi);
		}

	      update_stmt (stmt);
	    }
	}
      else
	gsi_next (&i);
    }
}

/* Initilize MPX initialization pass.  */
void
mpxopt_init (void)
{
  check_infos.create (0);

  calculate_dominance_info (CDI_DOMINATORS);
  calculate_dominance_info (CDI_POST_DOMINATORS);
}

/* Finalise MPX optimization  pass.  */
void
mpxopt_fini (void)
{
  mpx_fix_cfg ();

  free_dominance_info (CDI_DOMINATORS);
  free_dominance_info (CDI_POST_DOMINATORS);
}

/* MPX optimization pass function.  */
unsigned int
mpxopt_execute (void)
{
  mpxopt_init();

  /* This optimization may introduce new checks
     and thus we put it before checks search.  */
  mpx_optimize_string_function_calls ();

  mpx_gather_checks_info ();

  mpx_remove_excess_intersections ();

  mpx_remove_constant_checks ();

  mpx_remove_redundant_checks ();

  mpx_reduce_bounds_lifetime ();

  mpx_release_check_info ();

  mpxopt_fini ();

  return 0;
}

/* Pass gate.  */
bool
mpxopt_gate (void)
{
  return flag_mpx != 0 && (flag_mpxopt > 0
			   || (flag_mpxopt == -1 && optimize > 0));
}

struct gimple_opt_pass pass_mpxopt =
{
 {
  GIMPLE_PASS,
  "mpxopt",                             /* name */
  OPTGROUP_NONE,                        /* optinfo_flags */
  mpxopt_gate,                          /* gate */
  mpxopt_execute,                       /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_NONE,                              /* tv_id */
  PROP_ssa | PROP_cfg,                  /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_verify_flow | TODO_verify_stmts
  | TODO_update_ssa                     /* todo_flags_finish */
 }
};

struct gimple_opt_pass pass_mpx =
{
 {
  GIMPLE_PASS,
  "mpx",                                 /* name */
  OPTGROUP_NONE,                        /* optinfo_flags */
  mpx_gate,                              /* gate */
  mpx_execute,                           /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_NONE,                              /* tv_id */
  PROP_ssa | PROP_cfg,                  /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_verify_flow | TODO_verify_stmts
  | TODO_update_ssa                     /* todo_flags_finish */
 }
};

#include "gt-tree-mpx.h"
