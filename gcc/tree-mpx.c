#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tm_p.h"
#include "basic-block.h"
#include "flags.h"
#include "function.h"
#include "tree-inline.h"
#include "gimple.h"
#include "target.h"
#include "tree-iterator.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "tree-pass.h"
#include "hashtab.h"
#include "diagnostic.h"
#include "demangle.h"
#include "langhooks.h"
#include "ggc.h"
#include "cgraph.h"
#include "gimple.h"
#include "tree-mpx.h"
#include "rtl.h"
#include "expr.h"
#include "output.h"
#include "tree-pretty-print.h"
#include "gimple-pretty-print.h"

typedef void (*assign_handler)(tree, tree, void *);

static unsigned int mpx_execute (void);
static bool mpx_gate (void);

static void mpx_output_static_bounds (tree var, tree bnd_var,
				      struct mpx_ctor_stmt_list *stmts);
static int mpx_compare_var_names (const void *i1, const void *i2);
static int mpx_add_tree_to_vec (void **slot, void *res);
static int mpx_output_size_variable (void **slot, void *res);
static int mpx_may_complete_phi_bounds (void **slot, void *res);
static bool mpx_may_finish_incomplete_bounds (void);
static int mpx_find_valid_phi_bounds (void **slot, void *res);
static int mpx_recompute_phi_bounds (void **slot, void *res);
static int mpx_mark_invalid_bounds_walker (void **slot, void *res);
static void mpx_mark_completed_bounds (tree bounds);
static bool mpx_completed_bounds (tree bounds);
static void mpx_finish_incomplete_bounds (void);
static void mpx_erase_completed_bounds (void);
static void mpx_erase_incomplete_bounds (void);
static tree mpx_get_tmp_var (void);
static tree mpx_get_size_tmp_var (void);
static void mpx_fix_function_decl (tree decl, bool make_ssa_names);
static void mpx_fix_function_decls (void) ATTRIBUTE_UNUSED;
static void mpx_init (void);
static void mpx_fini (void);
static tree mpx_build_addr_expr (tree t);
static void mpx_register_bounds (tree ptr, tree bnd);
static void mpx_register_addr_bounds (tree ptr, tree bnd);
static tree mpx_get_registered_addr_bounds (tree ptr);
static void mpx_register_incomplete_bounds (tree bounds, tree ptr);
static bool mpx_incomplete_bounds (tree bounds);
static basic_block mpx_get_entry_block (void);
static tree mpx_get_none_bounds (void);
static tree mpx_get_invalid_op_bounds (void);
static tree mpx_get_nonpointer_load_bounds (void);
static void mpx_mark_invalid_bounds (tree bounds);
static bool mpx_valid_bounds (tree bounds);
static void mpx_transform_function (void);
static tree mpx_get_bound_for_parm (tree parm);
static tree mpx_force_gimple_call_op (tree op, gimple_seq *seq);
static tree mpx_build_bndldx (tree addr, tree ptr, gimple_stmt_iterator *gsi);
static void mpx_build_bndstx (tree addr, tree ptr, tree bounds,
			     gimple_stmt_iterator *gsi);
static tree mpx_build_returned_bound (gimple call);
static tree mpx_build_component_ref (tree obj, tree field);
static tree mpx_build_array_ref (tree arr, tree etype, tree esize,
				unsigned HOST_WIDE_INT idx);
static void mpx_copy_bounds_for_assign (tree lhs, tree rhs, void *arg);
static void mpx_add_modification_to_statements_list (tree lhs, tree rhs, void *arg);
static void mpx_walk_pointer_assignments (tree lhs, tree rhs, void *arg,
					 assign_handler handler);
static tree mpx_compute_bounds_for_assignment (tree node, gimple assign);
static tree mpx_make_static_bounds (tree var);
static tree mpx_make_static_const_bounds (HOST_WIDE_INT lb,
					  HOST_WIDE_INT ub,
					  const char *name);
static tree mpx_make_bounds (tree lb, tree size, gimple_stmt_iterator *iter, bool after);
static tree mpx_make_addressed_object_bounds (tree obj,
					     gimple_stmt_iterator *iter,
					     bool always_narrow_fields);
static tree mxp_get_var_size_decl (tree var) ATTRIBUTE_UNUSED;
static tree mpx_generate_extern_var_bounds (tree var);
static tree mpx_get_bounds_for_decl (tree decl) ATTRIBUTE_UNUSED;
static tree mpx_get_bounds_for_decl_addr (tree decl);
static tree mpx_get_bounds_for_string_cst (tree cst);
static tree mpx_get_bounds_by_definition (tree node, gimple def_stmt,
					 gimple_stmt_iterator *iter);
static tree mpx_find_bounds_1 (tree ptr, tree ptr_src,
			      gimple_stmt_iterator *iter,
			      bool always_narrow_fields);
static tree mpx_find_bounds (tree ptr, gimple_stmt_iterator *iter);
static tree mpx_find_bounds_loaded (tree ptr, tree ptr_src,
				   gimple_stmt_iterator *iter);
static tree mpx_find_bounds_narrowed (tree ptr, gimple_stmt_iterator *iter) ATTRIBUTE_UNUSED;
static tree mpx_find_bounds_abnormal (tree ptr, tree phi);
static void mpx_check_mem_access (tree first, tree last, tree bounds,
				 gimple_stmt_iterator iter,
				 location_t location, tree dirflag);
static tree mpx_intersect_bounds (tree bounds1, tree bounds2,
				 gimple_stmt_iterator *iter);
static bool mpx_may_narrow_to_field (tree field);
static bool mpx_narrow_bounds_for_field (tree field, bool always_narrow);
static tree mpx_narrow_bounds_to_field (tree bounds, tree component,
				       gimple_stmt_iterator *iter);
static void mpx_parse_array_and_component_ref (tree node, tree *ptr,
					      tree *elt, bool *safe,
					      bool *bitfield,
					      tree *bounds,
					      gimple_stmt_iterator *iter,
					      bool innermost_bounds,
					      bool always_narrow);
static void mpx_replace_address_check_builtin (gimple_stmt_iterator *gsi,
					      tree dirflag);
static void mpx_add_bounds_to_call_stmt (gimple_stmt_iterator *gsi);
static void mpx_add_bounds_to_ret_stmt (gimple_stmt_iterator *gsi);
static void mpx_process_stmt (gimple_stmt_iterator *iter, tree node,
			     location_t loc, tree dirflag,
			     tree access_offs, tree access_size,
			     bool safe);
static void mpx_mark_stmt (gimple s);
static bool mpx_marked_stmt (gimple s);
static void mpx_find_bound_slots (tree type, bool *have_bound,
				 HOST_WIDE_INT offs,
				 HOST_WIDE_INT ptr_size);

static GTY (()) tree mpx_bndldx_fndecl;
static GTY (()) tree mpx_bndstx_fndecl;
static GTY (()) tree mpx_checkl_fndecl;
static GTY (()) tree mpx_checku_fndecl;
static GTY (()) tree mpx_bndmk_fndecl;
static GTY (()) tree mpx_ret_bnd_fndecl;
static GTY (()) tree mpx_intersect_fndecl;
static GTY (()) tree mpx_user_intersect_fndecl;
static GTY (()) tree mpx_bind_intersect_fndecl;
static GTY (()) tree mpx_arg_bnd_fndecl;
static GTY (()) tree mpx_sizeof_fndecl;

static GTY (()) tree mpx_uintptr_type;

static GTY (()) tree mpx_zero_bounds_var = NULL;
static GTY (()) tree mpx_none_bounds_var = NULL;
static GTY (()) vec<tree, va_gc> *mpx_static_const_bounds = NULL;
static GTY ((param_is (struct tree_map)))
     htab_t mpx_static_var_bounds = NULL;
static GTY ((param_is (struct tree_map)))
     htab_t mpx_static_var_bounds_r = NULL;

static basic_block entry_block;
static tree zero_bounds;
static tree none_bounds;
static tree tmp_var;
static tree size_tmp_var;
static tree incomplete_bounds;

static GTY ((param_is (union gimple_statement_d))) htab_t mpx_marked_stmts;
static GTY ((param_is (union tree_node))) htab_t mpx_invalid_bounds;
static GTY ((param_is (union tree_node))) htab_t mpx_completed_bounds_map;
static GTY ((if_marked ("tree_map_marked_p"), param_is (struct tree_map)))
     htab_t mpx_reg_bounds;
static GTY ((if_marked ("tree_map_marked_p"), param_is (struct tree_map)))
     htab_t mpx_reg_addr_bounds;
static GTY ((if_marked ("tree_map_marked_p"), param_is (struct tree_map)))
     htab_t mpx_incomplete_bounds_map;
static GTY ((if_marked ("tree_vec_map_marked_p"), param_is (struct tree_vec_map)))
     htab_t mpx_abnormal_phi_copies;

static const char *BOUND_TMP_NAME = "__bound_tmp";
static const char *SIZE_TMP_NAME = "__size_tmp";

static GTY (()) vec<tree, va_gc> *var_inits;// = NULL;
static GTY ((if_marked ("tree_map_marked_p"),
	     param_is (struct tree_map))) htab_t mpx_size_decls = NULL;
const char *MPXSI_IDENTIFIER = "__mpx_initialize_static_bounds";
const char *MPX_SIZE_OF_SYMBOL_PREFIX = "__mpx_size_of_";
const char *MPX_BOUNDS_OF_SYMBOL_PREFIX = "__mpx_bounds_of_";
const char *MPX_STRING_BOUNDS_PREFIX = "__mpx_string_bounds_";
const char *MPX_VAR_BOUNDS_PREFIX = "__mpx_var_bounds_";
const char *MPX_ZERO_BOUNDS_VAR_NAME = "__mpx_zero_bounds";
const char *MPX_NONE_BOUNDS_VAR_NAME = "__mpx_none_bounds";

#define MAX_STMTS_IN_STATIC_MPX_CTOR 300

struct mpx_ctor_stmt_list
{
  tree stmts;
  int avail;
};

/* Mark statement S to not be instrumented.  */
static void
mpx_mark_stmt (gimple s)
{
  void **slot;

  slot = htab_find_slot (mpx_marked_stmts, s, INSERT);
  *slot = s;
}

/* Return 1 if statement S should not be instrumented.  */
static bool
mpx_marked_stmt (gimple s)
{
  return htab_find (mpx_marked_stmts, s) != NULL;
}

/* Get var to be used for bound values.  */
static tree
mpx_get_tmp_var (void)
{
  if (!tmp_var)
    {
      tmp_var = create_tmp_reg (bound_type_node, BOUND_TMP_NAME);
      //add_referenced_var (tmp_var);
    }

  return tmp_var;
}

/* Get var to be used for bound values.  */
static tree
mpx_get_size_tmp_var (void)
{
  if (!size_tmp_var)
    {
      size_tmp_var = create_tmp_reg (mpx_uintptr_type, SIZE_TMP_NAME);
      //add_referenced_var (size_tmp_var);
    }

  return size_tmp_var;
}

/* Split rtx RETURN_REG identifying slot for function value
   into two parts RETURN_REG_VAL and RETURN_REG_BND.
   First is the slot for regular value and the other one is
   for bounds.  */
void
mpx_split_returned_reg (rtx return_reg, rtx *return_reg_val,
		       rtx *return_reg_bnd)
{
  int i;
  int val_num = 0;
  int bnd_num = 0;
  rtx *val_tmps;
  rtx *bnd_tmps;

  *return_reg_bnd = 0;

  if (!return_reg
      || GET_CODE (return_reg) != PARALLEL)
    {
      *return_reg_val = return_reg;
      return;
    }

  val_tmps = XALLOCAVEC (rtx, XVECLEN (return_reg, 0));
  bnd_tmps = XALLOCAVEC (rtx, XVECLEN (return_reg, 0));

  for (i = 0; i < XVECLEN (return_reg, 0); i++)
    {
      rtx elem = XVECEXP (return_reg, 0, i);
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
      *return_reg_val = return_reg;
      return;
    }

  if ((GET_CODE (val_tmps[0]) == EXPR_LIST) || (val_num > 1))
    *return_reg_val = gen_rtx_PARALLEL (GET_MODE (return_reg),
					gen_rtvec_v (val_num, val_tmps));
  else
    *return_reg_val = val_tmps[0];

  if ((GET_CODE (bnd_tmps[0]) == EXPR_LIST) || (bnd_num > 1))
    *return_reg_bnd = gen_rtx_PARALLEL (VOIDmode,
					gen_rtvec_v (bnd_num, bnd_tmps));
  else
    *return_reg_bnd = bnd_tmps[0];
}

/* Join previously splitted to VAL and BND rtx for function
   value and return it.  */
rtx
mpx_join_splitted_reg (rtx val, rtx bnd)
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

/* Return bndmk call which creates bounds for structure
   pointed by PTR.  */
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

/* Emit store of BOUNDS for pointer VALUE stored in MEM.  */
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

/* Traversal function for mpx_may_finish_incomplete_bounds.  */
static int
mpx_may_complete_phi_bounds (void **slot, void *res)
{
  struct tree_map *map = (struct tree_map *)*slot;
  tree bounds = map->base.from;
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
	  return 0;
	}
    }

  return 1;
}

/* Check if there is a phi node whose bounds computation
   was not completed before but may be completed now.  */
static bool
mpx_may_finish_incomplete_bounds (void)
{
  bool res = true;

  htab_traverse (mpx_incomplete_bounds_map,
		 mpx_may_complete_phi_bounds,
		 &res);

  return res;
}

/* Helper function for mpx_finish_incomplete_bounds.
   Check if bounds phi node previously did not have
   args allowing to determine value for phi but now
   has.  */
static int
mpx_find_valid_phi_bounds (void **slot, void *res)
{
  struct tree_map *map = (struct tree_map *)*slot;
  tree bounds = map->base.from;
  gimple phi;
  unsigned i;

  gcc_assert (TREE_CODE (bounds) == SSA_NAME);

  if (mpx_completed_bounds (bounds))
    return 1;

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
	  mpx_recompute_phi_bounds (slot, NULL);
	  return 1;
	}
    }

  return 1;
}

/* Helper function for mpx_finish_incomplete_bounds.
   Recompute args for bounds phi node.  */
static int
mpx_recompute_phi_bounds (void **slot, void *res ATTRIBUTE_UNUSED)
{
  struct tree_map *map = (struct tree_map *)*slot;
  tree bounds = map->base.from;
  tree ptr = map->to;
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

  return 1;
}

/* Helper function for mpx_finish_incomplete_bounds.
   Marks all found invalid bounds.  */
static int
mpx_mark_invalid_bounds_walker (void **slot, void *res ATTRIBUTE_UNUSED)
{
  struct tree_map *map = (struct tree_map *)*slot;
  tree bounds = map->base.from;

  if (!mpx_completed_bounds (bounds))
    {
      mpx_mark_invalid_bounds (bounds);
      mpx_mark_completed_bounds (bounds);
    }
  return 1;
}

/* This function is called when we have enough info
   for incomplete bounds to be finally computed.  */
static void
mpx_finish_incomplete_bounds (void)
{
  bool found_valid;

  while (found_valid)
    {
      found_valid = false;

      htab_traverse (mpx_incomplete_bounds_map,
		     mpx_find_valid_phi_bounds,
		     &found_valid);

      if (found_valid)
	htab_traverse (mpx_incomplete_bounds_map,
		       mpx_recompute_phi_bounds,
		       NULL);
    }

  htab_traverse (mpx_incomplete_bounds_map,
		 mpx_mark_invalid_bounds_walker,
		 NULL);
  htab_traverse (mpx_incomplete_bounds_map,
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

   Add new modification statement into list of static initilizer
   statementes.  If statements list becomes too big, emit MPX
   contructor and start the new one.  */
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

/* Helper function for mpx_finish_file.
   Outputs one static bounds variable from mpx_static_var_bounds table.  */
static void
mpx_output_static_bounds (tree var, tree bnd_var, struct mpx_ctor_stmt_list *stmts)
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
   Outputs one variable from mpx_size_decls table.  */
int
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
}

/* This function requests intrumentation for all statements
   working with memory.  It also removes excess statements
   from static initializers.  */
static void
mpx_transform_function (void)
{
  basic_block bb, next;
  gimple_stmt_iterator i;
  enum gimple_rhs_class grhs_class;
  bool safe = DECL_MPX_STATIC_INIT (cfun->decl);

  bb = ENTRY_BLOCK_PTR ->next_bb;
  do
    {
      next = bb->next_bb;
      for (i = gsi_start_bb (bb); !gsi_end_p (i); )
        {
          gimple s = gsi_stmt (i);

	  /* Skip statement marked to not be instrumented.  */
	  if (mpx_marked_stmt (s))
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
	  if (DECL_MPX_STATIC_INIT (cfun->decl)
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

/* Replace call to __mpx_check_address_* pointed by GSI with
   pair of bndcu and bndcl calls.  DIRFLAG determines whether
   check is for read or write.  */

void
mpx_replace_address_check_builtin (gimple_stmt_iterator *gsi,
				   tree dirflag)
{
  gimple_stmt_iterator call_iter = *gsi;
  gimple call = gsi_stmt (*gsi);
  tree addr = gimple_call_arg (call, 0);
  tree bounds = mpx_find_bounds (addr, gsi);

  mpx_check_mem_access (addr, addr, bounds, *gsi,
			gimple_location (call), dirflag);
  gsi_prev (gsi);
  gsi_remove (&call_iter, true);
}

/* Add bound arguments to call statement pointed by GSI.  */

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
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_MD
      && fndecl != mpx_user_intersect_fndecl)
    return;

  /* MPX_CHECK_ADDRESS_READ call should be replaces with two checks.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_CHECK_ADDRESS_WRITE)
    {
      mpx_replace_address_check_builtin (gsi, integer_one_node);
      return;
    }

  /* MPX_CHECK_ADDRESS_WRITE call should be replaces with two checks.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MPX_CHECK_ADDRESS_READ)
    {
      mpx_replace_address_check_builtin (gsi, integer_zero_node);
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

  /* This one is the special case because the second
     argument is not used as a pointer but used as
     lower bound instead.  */
  if (fndecl == mpx_user_intersect_fndecl)
    {
      fndecl = mpx_bind_intersect_fndecl;
      arg_cnt = 4;
      bnd_arg_cnt = 1;
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
  if (fndecl == mpx_bind_intersect_fndecl)
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

/* Generate lower and upper bound checks for memory access
   to memory slot [FIRST, LAST] againsr BOUNDS.  Checks
   are inserted before the position pointed by ITER.
   DIRFLAG indicates whether memory access is load or store.  */
static void
mpx_check_mem_access (tree first, tree last, tree bounds,
		      gimple_stmt_iterator iter,
		      location_t location ATTRIBUTE_UNUSED,
		      tree dirflag ATTRIBUTE_UNUSED)
{
  gimple_seq seq;
  gimple checkl, checku;
  tree node;

  if (bounds == mpx_get_zero_bounds ())
    return;

  if (dirflag == integer_zero_node
      && !flag_mpx_check_read)
    return;

  if (dirflag == integer_one_node
      && !flag_mpx_check_write)
    return;

  seq = NULL;//gimple_seq_alloc ();

  node = mpx_force_gimple_call_op (first, &seq);

  checkl = gimple_build_call (mpx_checkl_fndecl, 2, bounds, node);
  mpx_mark_stmt (checkl);
  gimple_seq_add_stmt (&seq, checkl);

  node = mpx_force_gimple_call_op (last, &seq);

  checku = gimple_build_call (mpx_checku_fndecl, 2, bounds, node);
  mpx_mark_stmt (checku);
  gimple_seq_add_stmt (&seq, checku);

  gsi_insert_seq_before (&iter, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      gimple before = gsi_stmt (iter);
      fprintf (dump_file, "Generated bound checks for statement ");
      print_gimple_stmt (dump_file, before, 0, TDF_VOPS|TDF_MEMSYMS);
      fprintf (dump_file, "  ");
      print_gimple_stmt (dump_file, checkl, 0, TDF_VOPS|TDF_MEMSYMS);
      fprintf (dump_file, "  ");
      print_gimple_stmt (dump_file, checku, 0, TDF_VOPS|TDF_MEMSYMS);
    }
}

/* Build and return ADDR_EXPR for specified object T.  */
static tree
mpx_build_addr_expr (tree t)
{
  return TREE_CODE (t) == TARGET_MEM_REF
    ? tree_mem_ref_addr (ptr_type_node, t)
    : build_fold_addr_expr (t);
}

/* Register bounds BND for object PTR in globa bounds table.  */
static void
mpx_register_bounds (tree ptr, tree bnd)
{
  struct tree_map **slot, *map;

  if (bnd == incomplete_bounds)
    return;

  map = ggc_alloc_tree_map ();
  map->hash = htab_hash_pointer (ptr);
  map->base.from = ptr;
  map->to = bnd;

  slot = (struct tree_map **)
    htab_find_slot_with_hash (mpx_reg_bounds, map, map->hash, INSERT);
  *slot = map;

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
  struct tree_map *res, in;
  in.base.from = ptr;
  in.hash = htab_hash_pointer (ptr);

  res = (struct tree_map *) htab_find_with_hash (mpx_reg_bounds,
						 &in, in.hash);

  return res ? res->to : NULL_TREE;
}

/* Get bounds for input argument ARG.  */
tree
mpx_get_arg_bounds (tree arg)
{
  tree bounds = mpx_find_bounds (arg, NULL);

  if (!bounds)
    {
      if (arg == integer_zero_node)
	bounds = mpx_get_none_bounds ();
      else
	bounds = mpx_get_invalid_op_bounds ();
    }

  return bounds;
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

/* Register bounds BND for address of object PTR.  */
static void
mpx_register_addr_bounds (tree ptr, tree bnd)
{
  struct tree_map **slot, *map;

  if (bnd == incomplete_bounds)
    return;

  map = ggc_alloc_tree_map ();
  map->hash = htab_hash_pointer (ptr);
  map->base.from = ptr;
  map->to = bnd;

  slot = (struct tree_map **)
    htab_find_slot_with_hash (mpx_reg_addr_bounds, map, map->hash, INSERT);
  *slot = map;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Regsitered bound ");
      print_generic_expr (dump_file, bnd, 0);
      fprintf (dump_file, " for address of ");
      print_generic_expr (dump_file, ptr, 0);
      fprintf (dump_file, "\n");
    }
}

/* Return bounds registered for address of object PTR.  */
static tree
mpx_get_registered_addr_bounds (tree ptr)
{
  struct tree_map *res, in;
  in.base.from = ptr;
  in.hash = htab_hash_pointer (ptr);

  res = (struct tree_map *) htab_find_with_hash (mpx_reg_addr_bounds,
						 &in, in.hash);

  return res ? res->to : NULL_TREE;
}

/* Mark BOUNDS associated with PTR as incomplete.  */
static void
mpx_register_incomplete_bounds (tree bounds, tree ptr)
{
  struct tree_map **slot, *map;

  map = ggc_alloc_tree_map ();
  map->hash = htab_hash_pointer (bounds);
  map->base.from = bounds;
  map->to = ptr;

  slot = (struct tree_map **)
    htab_find_slot_with_hash (mpx_incomplete_bounds_map, map, map->hash, INSERT);
  *slot = map;

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
  struct tree_map *res, in;

  if (bounds == incomplete_bounds)
    return true;

  if (mpx_completed_bounds (bounds))
    return false;

  in.base.from = bounds;
  in.hash = htab_hash_pointer (bounds);

  res = (struct tree_map *) htab_find_with_hash (mpx_incomplete_bounds_map,
						 &in, in.hash);

  return res != NULL;
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

/* Return bounds to be used as a returned by CALL.  */
static tree
mpx_build_returned_bound (gimple call)
{
  gimple_stmt_iterator gsi;
  tree bounds;
  gimple stmt;
  tree fndecl = gimple_call_fndecl (call);

  /* Currently we handle alloca separately.  May also fix
     alloca expanding to obtain bounds on b0.  */
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
  else
    {
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

/* Return bounds to be used for input argument PARM.
   Build required bounds if required.  */
static tree
mpx_get_bound_for_parm (tree parm)
{
  tree decl = SSA_NAME_VAR (parm);
  tree bounds;

  bounds = mpx_get_registered_bounds (parm);

  if (!bounds)
    bounds = mpx_get_registered_bounds (decl);

  /* NULL bounds mean parm is not a pointer and
     zero bounds should be returned.  */
  if (!bounds)
    {
      /* For static chain param we return zero bounds
	 because currently we do not check dereferences
	 of this pointer.  */
      /* !!! FIXME: there is probably a more correct way to
	 identify such parm.  */
      if (cfun->decl && DECL_STATIC_CHAIN (cfun->decl)
	  && DECL_ARTIFICIAL (decl))
	bounds = mpx_get_zero_bounds ();
      /* Currently main method does not receive correct
	 bounds and we use zero bounds instead.  Default
	 behavior may be changed by flag.  */
      else if (flag_mpx_zero_input_bounds_for_main
	       && strcmp (IDENTIFIER_POINTER (DECL_NAME (cfun->decl)), "main") == 0)
	bounds = mpx_get_zero_bounds ();
      else if (BOUNDED_P (parm))
	{
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

/* Force OP to be suitable for using as an argument for call.
   New statements (if any) go to SEQ.  */
static tree
mpx_force_gimple_call_op (tree op, gimple_seq *seq)
{
  gimple_seq stmts;
  gimple_stmt_iterator si;

  op = force_gimple_operand (unshare_expr (op), &stmts, true, NULL_TREE);

//  for (node = gimple_seq_first (stmts); node; node = node->next)
//    mpx_mark_stmt (node->stmt);

  for (si = gsi_start (stmts); !gsi_end_p (si); gsi_next (&si)) 
    mpx_mark_stmt (gsi_stmt (si));  

  gimple_seq_add_seq (seq, stmts);

  return op;
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

  seq = NULL;//gimple_seq_alloc ();

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

  /*
  if (bounds == mpx_get_zero_bounds ())
    return;
  */

  seq = NULL;//gimple_seq_alloc ();

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

/* Mark BOUNDS as invalid.  */
static void
mpx_mark_invalid_bounds (tree bounds)
{
  void **slot;

  slot = htab_find_slot (mpx_invalid_bounds, bounds, INSERT);
  *slot = bounds;

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

  if (htab_find (mpx_invalid_bounds, bounds) != NULL)
    return false;

  return true;
}

/* Mark BOUNDS as completed.  */
static void
mpx_mark_completed_bounds (tree bounds)
{
  void **slot;

  slot = htab_find_slot (mpx_completed_bounds_map, bounds, INSERT);
  *slot = bounds;

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
  return htab_find (mpx_completed_bounds_map, bounds) != NULL;
}

/* Clear comleted bound marks.  */
static void
mpx_erase_completed_bounds (void)
{
  htab_empty (mpx_completed_bounds_map);
}

/* Clear incomleted bound marks.  */
static void
mpx_erase_incomplete_bounds (void)
{
  htab_empty (mpx_incomplete_bounds_map);
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
      bounds = mpx_find_bounds_loaded (node, rhs1, &iter);
      break;

    case VAR_DECL:
    case SSA_NAME:
    case ADDR_EXPR:
    case POINTER_PLUS_EXPR:
    case NOP_EXPR:
    case CONVERT_EXPR:
    case INTEGER_CST:
      bounds = mpx_find_bounds (rhs1, &iter);
      break;

    case VIEW_CONVERT_EXPR:
      bounds = mpx_find_bounds (TREE_OPERAND (rhs1, 0), &iter);
      break;

    case PARM_DECL:
      gcc_assert (TREE_ADDRESSABLE (rhs1));
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
	   does not help then look at bound values.  */
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
	  /* Seems both operands may have valid bounds.
	     In such case use default invalid op bounds.  */
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

    case CONSTRUCTOR:
      /*
	    {
	      unsigned int idx;
	      tree purp, val;
	      FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (rhs1), idx, purp, val)
	      {
	      }
	    }
      */
      /* fall thru */

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
   Return computed bounds*/
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
	  /*bounds = mpx_find_bounds (var, iter);
	    mpx_register_bounds (node, bounds);*/
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
      mpx_register_incomplete_bounds (bounds, node);
      break;

    case GIMPLE_ASM:
      bounds = mpx_get_zero_bounds ();
      mpx_register_bounds (node, bounds);
      /*warning (0, "PL: inline assembler is a potential pointer producer; using zero bounds (%s:%d)",
	       LOCATION_FILE (gimple_location (def_stmt)),
	       LOCATION_LINE (gimple_location (def_stmt)));*/
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

/* Creates a static bounds var of specfified NAME initilized
   with bounds of passe OBJ.  */
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

/* Creates a static bounds var of specfified NAME initilized
   with specified LB and UB values.  */
static tree
mpx_make_static_const_bounds (HOST_WIDE_INT lb,
			      HOST_WIDE_INT ub,
			      const char *name)
{
  tree var = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			 get_identifier (name), bound_type_node);

  TREE_PUBLIC (var) = 0;
  TREE_USED (var) = 1;
  TREE_READONLY (var) = 1;
  TREE_STATIC (var) = 1;
  TREE_ADDRESSABLE (var) = 0;
  DECL_ARTIFICIAL (var) = 1;
  DECL_COMMON (var) = 1;
  DECL_COMDAT (var) = 1;
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

/* Return var holding size relocation for given VAR.  */
static tree
mxp_get_var_size_decl (tree var)
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

/* When var has incomplete type we cannot get size to compute its bounds.
   In such cases we generate code to compute var bounds using special
   symbols pointing its begin and end.  */
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

  //size_reloc = mxp_get_var_size_decl (var);

  stmt = gimple_build_call (mpx_sizeof_fndecl, 1, var);

  size_reloc = create_tmp_reg (mpx_uintptr_type, SIZE_TMP_NAME);
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
	    || lookup_attribute ("mpx_variable_size", DECL_ATTRIBUTES (field))
	    || mpx_variable_size_type (TREE_TYPE (field));
      }
  else
    res = !TYPE_SIZE (type)
      || TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST
      || tree_low_cst (TYPE_SIZE (type), 1) == 0;

  return res;
}

/* Compute and retrurn bounds for DECL which is
   either VAR_DECL or PARM_DECL.  */
static tree
mpx_get_bounds_for_decl (tree decl)
{
  tree bounds;

  gcc_assert (TREE_CODE (decl) == VAR_DECL
	      || TREE_CODE (decl) == PARM_DECL);

  bounds = mpx_get_registered_bounds (decl);

  if (bounds)
    return bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Building bounds for decl ");
      print_generic_expr (dump_file, decl, 0);
      fprintf (dump_file, "\n");
    }

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

  mpx_register_bounds (decl, bounds);

  return bounds;
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

	  bounds = mpx_get_bounds_by_definition(ptr_src, def_stmt, &phi_iter);

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

/* Search for narrowed bounds if applicable.  */
static tree
mpx_find_bounds_narrowed (tree ptr, gimple_stmt_iterator *iter)
{
  return mpx_find_bounds_1 (ptr, NULL_TREE, iter, true);
}

/* Search for bounds for PTR to be used in abnormal PHI node.  */
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

  if (gimple_code (SSA_NAME_DEF_STMT (bounds)) == GIMPLE_PHI)
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
      copy = create_tmp_reg (bound_type_node, BOUND_TMP_NAME);
      copy = make_ssa_name (copy, gimple_build_nop ());
      assign = gimple_build_assign (copy, bounds);

      if (gimple_code (SSA_NAME_DEF_STMT (bounds)) == GIMPLE_NOP)
	gsi = gsi_last_bb (mpx_get_entry_block ());
      else
	gsi = gsi_for_stmt (SSA_NAME_DEF_STMT (bounds));

      gsi_insert_after (&gsi, assign, GSI_SAME_STMT);

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
     we do not need this flag set anymore.  */
  SSA_NAME_OCCURS_IN_ABNORMAL_PHI (bounds) = 0;

  return copy;
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
    && !lookup_attribute ("mpx_variable_size", DECL_ATTRIBUTES (field))
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

/* Add code to copy bounds for assignment of RHS to LHS.  */
static void
mpx_copy_bounds_for_assign (tree lhs, tree rhs, void *arg)
{
  gimple_stmt_iterator *iter = (gimple_stmt_iterator *)arg;
  tree bounds = mpx_find_bounds (rhs, iter);
  tree addr = mpx_build_addr_expr(lhs);

  mpx_build_bndstx (addr, rhs, bounds, iter);
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

/* Add input bound arguments declaration to the function
   declaration DECL.  Do nothing if function already have
   bound arguments.  If MAKE_SSA_NAMES is set then ssa name
   is created and registered as a bound for each added
   argument.  */
static void
mpx_fix_function_decl (tree decl, bool make_ssa_names)
{
  tree arg = DECL_ARGUMENTS (decl);
  tree prev_arg = NULL_TREE;
  bool already_fixed = false;
  int bnd_no = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file,
	       "Building bound arguments for function declaration '");
      print_generic_expr (dump_file, decl, 0);
      fprintf (dump_file, "'\n");
    }

  /* Nothing to do if function has no input arguments.  */
  if (!arg)
    return;

  for (arg = DECL_ARGUMENTS (decl); arg; arg = TREE_CHAIN (arg))
    {
      /* Bound argument may appear only if the
	 declaration has been already fixed.  */
      if (BOUND_TYPE_P (TREE_TYPE (arg)))
	{
	  if (make_ssa_names)
	    {
	      tree bounds = make_ssa_name (arg, gimple_build_nop ());
	      SSA_NAME_IS_DEFAULT_DEF (bounds) = 1;

	      gcc_assert (prev_arg);
	      mpx_register_bounds (prev_arg, bounds);

	      already_fixed = true;
	    }
	  else
	    return;
	}

      prev_arg = arg;
    }

  if (already_fixed)
    return;

  /* Go through all input pointers and create bound
     declaration for each of them.  Bound declaration
     is placed right after pointer arg.  Also make
     register ssa name for each bound if needed.  */
  for (arg = DECL_ARGUMENTS (decl); arg; arg = TREE_CHAIN (arg))
    {
      if (BOUNDED_P (arg))
	{
	  char name_buf[20];
	  tree name;
	  tree bounds;

	  sprintf (name_buf, "__arg_bnd.%d", bnd_no++);
	  name = get_identifier (name_buf);

	  bounds = build_decl (UNKNOWN_LOCATION, PARM_DECL, name,
				 bound_type_node);
	  DECL_ARG_TYPE (bounds) = bound_type_node;

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "  Built bounds '");
	      print_generic_expr (dump_file, bounds, 0);
	      fprintf (dump_file, "' for arg '");
	      print_generic_expr (dump_file, arg, 0);
	      fprintf (dump_file, "' of type '");
	      print_generic_expr (dump_file, TREE_TYPE (arg), 0);
	      fprintf (dump_file, "'\n");
	    }

	  TREE_CHAIN (bounds) = TREE_CHAIN (arg);
	  TREE_CHAIN (arg) = bounds;

	  if (make_ssa_names)
	    {
	      tree ssa = make_ssa_name (bounds, gimple_build_nop ());
	      SSA_NAME_IS_DEFAULT_DEF (ssa) = 1;
	      mpx_register_bounds (arg, ssa);
	    }

	  /* Skip inserted bound arg.  */
	  arg = TREE_CHAIN (arg);
	}
    }
}

/* Fix all function declarations used in this function
   to have bound args in them.  */
static void
mpx_fix_function_decls (void)
{
  basic_block bb;
  gimple_stmt_iterator i;

  mpx_fix_function_decl (cfun->decl, true);

  FOR_ALL_BB (bb)
    for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
      {
	gimple stmt = gsi_stmt (i);
	if (is_gimple_call (stmt))
	  {
	    tree fndecl = gimple_call_fndecl (stmt);
	    if (fndecl)
	      mpx_fix_function_decl (fndecl, false);
	  }
      }
}

/* Some code transformation made during MPX pass may put
   code into inconsistent state.  Here we find and fix
   such flaws.  */
void
mpx_fix_cfg ()
{
  unsigned int n;
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

/* Initialize pass. */
static void
mpx_init (void)
{
  mpx_reg_bounds = htab_create_ggc (31, tree_map_hash, tree_map_eq,
				    NULL);
  mpx_reg_addr_bounds = htab_create_ggc (31, tree_map_hash, tree_map_eq,
					 NULL);
  mpx_incomplete_bounds_map = htab_create_ggc (31, tree_map_hash, tree_map_eq,
					       NULL);
  mpx_marked_stmts = htab_create_ggc (31, htab_hash_pointer, htab_eq_pointer,
				      NULL);
  mpx_invalid_bounds = htab_create_ggc (31, htab_hash_pointer,
					htab_eq_pointer, NULL);
  mpx_completed_bounds_map = htab_create_ggc (31, htab_hash_pointer,
					      htab_eq_pointer, NULL);
  mpx_abnormal_phi_copies = htab_create_ggc (31, tree_vec_map_hash,
					     tree_vec_map_eq, NULL);

  entry_block = NULL;
  zero_bounds = NULL_TREE;
  none_bounds = NULL_TREE;
  incomplete_bounds = integer_zero_node;
  tmp_var = NULL_TREE;
  size_tmp_var = NULL_TREE;

  mpx_uintptr_type = lang_hooks.types.type_for_mode (ptr_mode, true);

  /* Build declarations for builtin functions.  */
  mpx_bndldx_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_BNDLDX);
  mpx_bndstx_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_BNDSTX);
  mpx_checkl_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_BNDCL);
  mpx_checku_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_BNDCU);
  mpx_bndmk_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_BNDMK);
  mpx_ret_bnd_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_BNDRET);
  mpx_intersect_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_INTERSECT);
  mpx_user_intersect_fndecl
    = targetm.builtin_mpx_function (BUILT_IN_MPX_USER_INTERSECT);
  mpx_bind_intersect_fndecl
    = targetm.builtin_mpx_function (BUILT_IN_MPX_BIND_INTERSECT);
  mpx_arg_bnd_fndecl
    = targetm.builtin_mpx_function (BUILT_IN_MPX_ARG_BND);
  mpx_sizeof_fndecl = targetm.builtin_mpx_function (BUILT_IN_MPX_SIZEOF);
}

/* Finalize pass. */
static void
mpx_fini (void)
{

}

/* Main pass function. */
static unsigned int
mpx_execute (void)
{
  /* FIXME: check we need to instrument this function */
  mpx_init ();

  mpx_transform_function ();

  mpx_fix_cfg ();

  mpx_fini ();

  return 0;
}

/* Pass gate.  */
static bool
mpx_gate (void)
{
  return flag_mpx != 0
    && !lookup_attribute ("mpx_legacy", DECL_ATTRIBUTES (cfun->decl));
}

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
