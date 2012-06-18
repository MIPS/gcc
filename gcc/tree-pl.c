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
#include "tree-pl.h"

static unsigned int pl_execute (void);
static bool pl_gate (void);

static void pl_fix_function_decl (tree decl, bool make_ssa_names);
static void pl_fix_function_decls (void);
static void pl_init (void);
static void pl_fini (void);
static void pl_register_bounds (tree ptr, tree bnd);
static tree pl_get_registered_bounds (tree ptr);
static basic_block pl_get_entry_block (void);
static tree pl_get_zero_bounds (void);
static tree pl_get_none_bounds (void);
static bool pl_valid_bounds (tree bounds);
static void pl_transform_function (void);
static tree pl_get_bound_for_parm (tree parm);
static tree pl_build_bndldx (tree addr, tree ptr, gimple_stmt_iterator gsi);
static void pl_build_bndstx (tree addr, tree ptr, tree bounds,
			     gimple_stmt_iterator gsi);
static tree pl_build_returned_bound (gimple call);
static tree pl_compute_bounds_for_assignment (tree node, gimple assign);
static tree pl_make_bounds (tree lb, tree size, gimple_stmt_iterator *iter);
static tree pl_make_addressed_object_bounds (tree obj,
					     gimple_stmt_iterator iter);
static tree pl_generate_extern_var_bounds (tree var);
static tree pl_get_bounds_for_decl (tree decl);
static tree pl_get_bounds_for_string_cst (tree cst);
static tree pl_get_bounds_by_definition (tree node, gimple def_stmt,
					 gimple_stmt_iterator *iter);
static tree pl_find_bounds (tree ptr, gimple_stmt_iterator iter);
static void pl_check_mem_access (tree first, tree last, tree bounds,
				 gimple_stmt_iterator *instr_gsi,
				 location_t location, tree dirflag);
static tree pl_intersect_bounds (tree bounds1, tree bounds2,
				 gimple_stmt_iterator iter);
static void pl_parse_array_and_component_ref (tree node, tree *ptr,
					      tree *elt, bool *component,
					      bool *bitfield,
					      tree *bounds,
					      gimple_stmt_iterator iter,
					      bool innermost_bounds);
static void pl_add_bounds_to_call_stmt (gimple_stmt_iterator *gsi);
static void pl_add_bounds_to_ret_stmt (gimple_stmt_iterator *gsi);
static void pl_process_stmt (gimple_stmt_iterator *iter, tree node,
			     location_t loc, tree dirflag,
			     tree access_offs, tree access_size);
static void pl_mark_stmt (gimple s);
static bool pl_marked_stmt (gimple s);

static GTY (()) tree pl_bndldx_fndecl;
static GTY (()) tree pl_bndstx_fndecl;
static GTY (()) tree pl_checkl_fndecl;
static GTY (()) tree pl_checku_fndecl;
static GTY (()) tree pl_bndmk_fndecl;
static GTY (()) tree pl_ret_bnd_fndecl;
static GTY (()) tree pl_intersect_fndecl;

static GTY (()) tree pl_bound_type;
static GTY (()) tree pl_uintptr_type;

static basic_block entry_block;
static tree zero_bounds;
static tree none_bounds;

static GTY ((param_is (union tree_node))) htab_t pl_marked_stmts;

static const char *BOUND_TMP_NAME = "__bound_tmp";

static VEC(tree,gc) *var_inits = NULL;

static void
pl_mark_stmt (gimple s)
{
  void **slot;

  slot = htab_find_slot (pl_marked_stmts, s, INSERT);
  *slot = s;
}

static bool
pl_marked_stmt (gimple s)
{
  return htab_find (pl_marked_stmts, s) != NULL;
}

extern void
pl_register_var_initializer (tree var)
{
  tree init;
  tree var_addr;

  if (!flag_pl)
    return;

  gcc_assert (TREE_CODE (var) == VAR_DECL);

  init = DECL_INITIAL (var);
  gcc_assert (init && init != error_mark_node);

  if (TREE_STATIC (var)
      && POINTER_TYPE_P (TREE_TYPE (var)))
    VEC_safe_push (tree, gc, var_inits, var);
}

extern VEC(tree,gc) *
pl_get_initialized_vars ()
{
  return var_inits;
}

static void
pl_transform_function (void)
{
  basic_block bb, next;
  gimple_stmt_iterator i;
  int saved_last_basic_block = last_basic_block;
  enum gimple_rhs_class grhs_class;

  bb = ENTRY_BLOCK_PTR ->next_bb;
  do
    {
      next = bb->next_bb;
      for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
        {
          gimple s = gsi_stmt (i);

	  if (pl_marked_stmt (s))
	    continue;

          switch (gimple_code (s))
            {
            case GIMPLE_ASSIGN:
	      pl_process_stmt (&i, gimple_assign_lhs (s),
			       gimple_location (s), integer_one_node,
			       NULL_TREE, NULL_TREE);
	      pl_process_stmt (&i, gimple_assign_rhs1 (s),
			       gimple_location (s), integer_zero_node,
			       NULL_TREE, NULL_TREE);
	      grhs_class = get_gimple_rhs_class (gimple_assign_rhs_code (s));
	      if (grhs_class == GIMPLE_BINARY_RHS)
		pl_process_stmt (&i, gimple_assign_rhs2 (s),
				 gimple_location (s), integer_zero_node,
				 NULL_TREE, NULL_TREE);
              break;

            case GIMPLE_RETURN:
              if (gimple_return_retval (s) != NULL_TREE)
                {
                  pl_process_stmt (&i, gimple_return_retval (s),
				   gimple_location (s),
				   integer_zero_node,
				   NULL_TREE, NULL_TREE);

		  /* Additionall we need to add bounds
		     to return statement.  */
		  pl_add_bounds_to_ret_stmt (&i);
                }
              break;

	    case GIMPLE_CALL:
	      pl_add_bounds_to_call_stmt (&i);
	      break;

            default:
              ;
            }
        }
      bb = next;
    }
  while (bb && bb->index <= saved_last_basic_block);
}

/* Add bound retvals to return statement pointed by GSI.  */

static void
pl_add_bounds_to_ret_stmt (gimple_stmt_iterator *gsi)
{
  gimple ret = gsi_stmt (*gsi);
  tree retval = gimple_return_retval (ret);
  tree ret_decl = DECL_RESULT (cfun->decl);

  if (!retval)
    return;

  switch TREE_CODE (TREE_TYPE (ret_decl))
    {
    case POINTER_TYPE:
    case REFERENCE_TYPE:
    case ARRAY_TYPE:
      gimple_return_set_retval2 (ret, pl_find_bounds (retval, *gsi));
      break;

      /* TODO: Add support for structures which may
	 be returned on registers.  */

    default:
      break;
    }

  update_stmt (ret);
}

/* Add bound arguments to call statement pointed by GSI.  */

static void
pl_add_bounds_to_call_stmt (gimple_stmt_iterator *gsi)
{
  gimple call = gsi_stmt (*gsi);
  unsigned arg_no = 0;
  unsigned new_arg_no = 0;
  unsigned bnd_arg_cnt = 0;
  unsigned arg_cnt = 0;
  tree fndecl = gimple_call_fndecl (call);
  tree first_formal_arg;
  tree arg;
  gimple new_call;
  ssa_op_iter iter;
  tree op;

  /* Do nothing if back-end builtin is called.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_MD)
    return;

  if (fndecl)
    first_formal_arg = DECL_ARGUMENTS (fndecl);
  else
    {
      tree fntype;
      tree fnptr = gimple_call_fn (call);
      gcc_assert (TREE_CODE (fnptr) == SSA_NAME
		  || TREE_CODE (fnptr) == OBJ_TYPE_REF);

      fntype = TREE_TYPE (TREE_TYPE (fnptr));
      gcc_assert (TREE_CODE (fntype) == FUNCTION_TYPE
		  || TREE_CODE (fntype) == METHOD_TYPE);

      first_formal_arg = TYPE_ARG_TYPES (fntype);
    }

  /* Get number of arguments and bound arguments from
     functiond declaration or function pointer type.  */
  for (arg = first_formal_arg;
       arg && (fndecl || arg != void_list_node);
       arg = TREE_CHAIN (arg))
    {
      if (fndecl && BOUND_TYPE_P (TREE_TYPE (arg)))
	bnd_arg_cnt++;

      /* Currently we do not fix function types. Therefore
	 look for pointer args and but count arg_cnt like
	 if it was fixed.  */
      if (!fndecl && POINTER_TYPE_P (TREE_VALUE (arg)))
	{
	  bnd_arg_cnt++;
	  arg_cnt++;
	}

      arg_cnt++;
    }

  /* Now add number of additional bound arguments for pointers
     passed to vararg functions.  */
  for (arg_no = arg_cnt - bnd_arg_cnt;
       arg_no < gimple_call_num_args (call);
       arg_no++)
    {
      if (POINTER_TYPE_P (TREE_TYPE (gimple_call_arg (call, arg_no))))
	arg_cnt++;
      arg_cnt++;
    }

  new_call = gimple_alloc (GIMPLE_CALL, arg_cnt + 3);
  memcpy (new_call, call, sizeof (struct gimple_statement_call));
  gimple_set_num_ops (new_call, arg_cnt + 3);
  gimple_set_op (new_call, 0, gimple_op (call, 0));
  gimple_set_op (new_call, 1, gimple_op (call, 1));
  gimple_set_op (new_call, 2, gimple_op (call, 2));

  arg_no = 0;
  for (arg = first_formal_arg;
       arg && (fndecl || arg != void_list_node);
       arg = TREE_CHAIN (arg))
    {
      if (fndecl && BOUND_TYPE_P (TREE_TYPE (arg)))
	{
	  tree prev_arg = gimple_call_arg (call, arg_no - 1);
	  tree bounds = pl_find_bounds (prev_arg, *gsi);
	  gimple_call_set_arg (new_call, new_arg_no++, bounds);
	}
      else
	{
	  tree call_arg = gimple_call_arg (call, arg_no++);
	  gimple_call_set_arg (new_call, new_arg_no++, call_arg);

	  if (!fndecl && POINTER_TYPE_P (TREE_VALUE (arg)))
	    {
	      tree bounds = pl_find_bounds (call_arg, *gsi);
	      gimple_call_set_arg (new_call, new_arg_no++, bounds);
	    }
	}
    }

  for ( ; arg_no < gimple_call_num_args (call); arg_no++)
    {
      tree call_arg = gimple_call_arg (call, arg_no);
      gimple_call_set_arg (new_call, new_arg_no++, call_arg);
      if (POINTER_TYPE_P (TREE_TYPE (call_arg)))
	gimple_call_set_arg (new_call, new_arg_no++,
			     pl_find_bounds (call_arg, *gsi));
    }

  FOR_EACH_SSA_TREE_OPERAND (op, call, iter, SSA_OP_ALL_DEFS)
    {
      SSA_NAME_DEF_STMT (op) = new_call;
    }

  gsi_replace (gsi, new_call, true);
}

static void
pl_check_mem_access (tree first, tree last, tree bounds,
		     gimple_stmt_iterator *instr_gsi,
		     location_t location, tree dirflag)
{
  gimple_seq seq, stmts;
  gimple checkl, checku;
  tree node;

  seq = gimple_seq_alloc ();

  node = force_gimple_operand (first, &stmts, true, NULL_TREE);
  gimple_seq_add_seq (&seq, stmts);

  checkl = gimple_build_call (pl_checkl_fndecl, 2, bounds, node);
  pl_mark_stmt (checkl);
  gimple_seq_add_stmt (&seq, checkl);

  node = force_gimple_operand (last, &stmts, true, NULL_TREE);
  gimple_seq_add_seq (&seq, stmts);

  checku = gimple_build_call (pl_checku_fndecl, 2, bounds, node);
  pl_mark_stmt (checku);
  gimple_seq_add_stmt (&seq, checku);

  gsi_insert_seq_before (instr_gsi, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      gimple before = gsi_stmt (*instr_gsi);
      fprintf (dump_file, "Generated bound checks for statement ");
      print_gimple_stmt (dump_file, before, 0, TDF_VOPS|TDF_MEMSYMS);
      fprintf (dump_file, "  ");
      print_gimple_stmt (dump_file, checkl, 0, TDF_VOPS|TDF_MEMSYMS);
      fprintf (dump_file, "  ");
      print_gimple_stmt (dump_file, checku, 0, TDF_VOPS|TDF_MEMSYMS);
    }
}

static GTY ((if_marked ("tree_map_marked_p"), param_is (struct tree_map)))
     htab_t pl_reg_bounds;

static void
pl_register_bounds (tree ptr, tree bnd)
{
  struct tree_map **slot, *map;

  map = ggc_alloc_tree_map ();
  map->hash = htab_hash_pointer (ptr);
  map->base.from = ptr;
  map->to = bnd;

  slot = (struct tree_map **)
    htab_find_slot_with_hash (pl_reg_bounds, map, map->hash, INSERT);
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

static tree
pl_get_registered_bounds (tree ptr)
{
  struct tree_map *res, in;
  in.base.from = ptr;
  in.hash = htab_hash_pointer (ptr);

  res = (struct tree_map *) htab_find_with_hash (pl_reg_bounds,
						 &in, in.hash);

  return res ? res->to : NULL_TREE;
}

static basic_block
pl_get_entry_block (void)
{
  if (!entry_block)
    {
      basic_block prev_entry = ENTRY_BLOCK_PTR->next_bb;
      edge e = find_edge (ENTRY_BLOCK_PTR, prev_entry);
      entry_block = split_edge (e);
    }

  return entry_block;
}

static tree
pl_get_zero_bounds (void)
{
  if (zero_bounds)
    return zero_bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Creating zero bounds...");

  zero_bounds = pl_make_bounds (integer_zero_node,
				integer_zero_node,
				NULL);

  return zero_bounds;
}

static tree
pl_get_none_bounds (void)
{
  if (none_bounds)
    return none_bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Creating none bounds...");

  none_bounds = pl_make_bounds (integer_minus_one_node,
				build_int_cst (size_type_node, 2),
				NULL);

  return none_bounds;
}

static tree
pl_build_returned_bound (gimple call)
{
  gimple_stmt_iterator gsi;
  tree bounds;
  gimple stmt;

  stmt = gimple_build_call (pl_ret_bnd_fndecl, 0);
  pl_mark_stmt (stmt);

  /* If call may throw then we have to insert new
     statement to the next BB.  Otherwise insert
     it right after call.  */
  if (stmt_can_throw_internal (call))
    {
      basic_block bb = gimple_bb (call);

      gcc_assert (EDGE_COUNT (bb->succs) == 2);

      gsi = gsi_start_bb (FALLTHRU_EDGE (bb)->dest);
      gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);
    }
  else
    {
      gsi = gsi_for_stmt (call);
      gsi_insert_after (&gsi, stmt, GSI_SAME_STMT);
    }

  bounds = create_tmp_reg (pl_bound_type, BOUND_TMP_NAME);
  add_referenced_var (bounds);
  bounds = make_ssa_name (bounds, stmt);
  gimple_call_set_lhs (stmt, bounds);

  update_stmt (stmt);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Built returned bounds (");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, ") for call: ");
      print_gimple_stmt (dump_file, call, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  pl_register_bounds (gimple_call_lhs (call), bounds);

  return bounds;
}

static tree
pl_get_bound_for_parm (tree parm)
{
  tree bounds;

  bounds = pl_get_registered_bounds (parm);

  /* NULL bounds mean parm is not a pointer and
     zero bounds should be returned.  */
  if (!bounds)
    {
      gcc_assert (!POINTER_TYPE_P (TREE_TYPE (parm)));
      bounds = pl_get_zero_bounds ();
    }

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

static tree
pl_build_bndldx (tree addr, tree ptr, gimple_stmt_iterator gsi)
{
  gimple_seq seq, stmts;
  gimple stmt;
  tree bounds;

  seq = gimple_seq_alloc ();

  addr = force_gimple_operand (addr, &stmts, true, NULL_TREE);
  gimple_seq_add_seq (&seq, stmts);

  stmt = gimple_build_call (pl_bndldx_fndecl, 2, addr, ptr);
  pl_mark_stmt (stmt);
  bounds = create_tmp_reg (pl_bound_type, BOUND_TMP_NAME);
  add_referenced_var (bounds);
  bounds = make_ssa_name (bounds, stmt);
  gimple_call_set_lhs (stmt, bounds);

  gimple_seq_add_stmt (&seq, stmt);

  gsi_insert_seq_after (&gsi, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generated bndldx for pointer ");
      print_generic_expr (dump_file, ptr, 0);
      fprintf (dump_file, ": ");
      print_gimple_stmt (dump_file, stmt, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  return bounds;
}

static void
pl_build_bndstx (tree addr, tree ptr, tree bounds,
		 gimple_stmt_iterator gsi)
{
  gimple_seq seq, stmts;
  gimple stmt;

  seq = gimple_seq_alloc ();

  addr = force_gimple_operand (addr, &stmts, true, NULL_TREE);
  gimple_seq_add_seq (&seq, stmts);

  ptr = force_gimple_operand (ptr, &stmts, true, NULL_TREE);
  gimple_seq_add_seq (&seq, stmts);

  stmt = gimple_build_call (pl_bndstx_fndecl, 3, addr, ptr, bounds);
  pl_mark_stmt (stmt);

  gimple_seq_add_stmt (&seq, stmt);

  gsi_insert_seq_after (&gsi, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generated bndstx for pointer store ");
      print_gimple_stmt (dump_file, gsi_stmt (gsi), 0, TDF_VOPS|TDF_MEMSYMS);
      print_gimple_stmt (dump_file, stmt, 2, TDF_VOPS|TDF_MEMSYMS);
    }
}

static bool
pl_valid_bounds (tree bounds)
{
  if (bounds == zero_bounds || bounds == none_bounds)
    return false;

  return true;
}

static tree
pl_compute_bounds_for_assignment (tree node, gimple assign)
{
  enum tree_code rhs_code = gimple_assign_rhs_code (assign);
  location_t loc = gimple_location (assign);
  tree rhs1 = gimple_assign_rhs1 (assign);
  tree rhs2 = gimple_assign_rhs2 (assign);
  tree bounds = NULL_TREE;
  gimple_stmt_iterator iter;
  tree ptr;
  tree addr;
  tree offs;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Computing bounds for assignment: ");
      print_gimple_stmt (dump_file, assign, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  switch (rhs_code)
    {
    case MEM_REF:
      gcc_assert (node);

      ptr = TREE_OPERAND (rhs1, 0);
      offs = TREE_OPERAND (rhs1, 1);

      addr = fold_build_pointer_plus_loc (loc, ptr, offs);
      bounds = pl_build_bndldx (addr, node, gsi_for_stmt (assign));
      break;

    case ARRAY_REF:
    case COMPONENT_REF:
      {
	addr = fold_build1 (ADDR_EXPR,
			    build_pointer_type (TREE_TYPE (rhs1)), rhs1);
	bounds = pl_build_bndldx (addr, node, gsi_for_stmt (assign));
      }
      break;

    case SSA_NAME:
    case ADDR_EXPR:
    case POINTER_PLUS_EXPR:
    case NOP_EXPR:
    case CONVERT_EXPR:
      iter = gsi_for_stmt (assign);
      bounds = pl_find_bounds (rhs1, iter);
      break;

    case TARGET_MEM_REF:
      bounds = pl_find_bounds (TMR_BASE (rhs1), iter);
      break;

    case MINUS_EXPR:
    case PLUS_EXPR:
    case BIT_AND_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
      {
	tree bnd1 = pl_find_bounds (rhs1, iter);
	tree bnd2 = pl_find_bounds (rhs2, iter);

	if (!pl_valid_bounds (bnd1))
	  if (pl_valid_bounds (bnd2) && rhs_code != MINUS_EXPR)
	    bounds = bnd2;
	  else
	    bounds = pl_get_none_bounds ();
	else if (!pl_valid_bounds (bnd2))
	  bounds = bnd1;
	else
	  bounds = pl_get_none_bounds ();
      }
      break;

    case VAR_DECL:
      addr = fold_build1 (ADDR_EXPR,
			  build_pointer_type (TREE_TYPE (rhs1)), rhs1);
      bounds = pl_build_bndldx (addr, node, gsi_for_stmt (assign));
      break;

    case INTEGER_CST:
      bounds = pl_get_zero_bounds ();
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
      internal_error ("pl_compute_bounds_for_assignment: Unexpected RHS code %s",
		      tree_code_name[rhs_code]);
    }

  gcc_assert (bounds);

  if (node)
    pl_register_bounds (node, bounds);

  return bounds;
}

static tree
pl_get_bounds_by_definition (tree node, gimple def_stmt, gimple_stmt_iterator *iter)
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
	  bounds = pl_get_bound_for_parm (var);
	  pl_register_bounds (node, bounds);
	  break;

	default:
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Unexpected var with no definition\n");
	      print_generic_expr (dump_file, var, 0);
	    }
	  internal_error ("pl_get_bounds_by_definition: Unexpected var of type %s",
			  tree_code_name[(int) TREE_CODE (var)]);
	}
      break;

    case GIMPLE_ASSIGN:
      bounds = pl_compute_bounds_for_assignment (node, def_stmt);
      break;

    case GIMPLE_CALL:
      bounds = pl_build_returned_bound (def_stmt);
      break;

    case GIMPLE_PHI:
      bounds = create_tmp_reg (pl_bound_type, BOUND_TMP_NAME);
      add_referenced_var (bounds);

      stmt = create_phi_node (bounds, gimple_bb (def_stmt));
      bounds = gimple_phi_result (stmt);
      *iter = gsi_for_stmt (stmt);

      pl_register_bounds (node, bounds);
      break;

    default:
      internal_error ("pl_get_bounds_by_definition: Unexpected GIMPLE code %s",
		      gimple_code_name[code]);
    }

  return bounds;
}

static tree
pl_make_bounds (tree lb, tree size, gimple_stmt_iterator *iter)
{
  gimple_seq seq, stmts;
  gimple_stmt_iterator gsi;
  gimple stmt;
  tree bounds;

  if (iter)
    gsi = *iter;
  else
    gsi = gsi_start_bb (pl_get_entry_block ());

  seq = gimple_seq_alloc ();

  lb = force_gimple_operand (lb, &stmts, true, NULL_TREE);
  gimple_seq_add_seq (&seq, stmts);

  size = force_gimple_operand (size, &stmts, true, NULL_TREE);
  gimple_seq_add_seq (&seq, stmts);

  stmt = gimple_build_call (pl_bndmk_fndecl, 2, lb, size);
  pl_mark_stmt (stmt);

  bounds = create_tmp_reg (pl_bound_type, BOUND_TMP_NAME);
  add_referenced_var (bounds);
  bounds = make_ssa_name (bounds, stmt);
  gimple_call_set_lhs (stmt, bounds);

  gimple_seq_add_stmt (&seq, stmt);

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

/* When var has incomplete type we cannot get size to compute its bounds.
   In such cases we generate code to compute var bounds using special
   symbols pointing its begin and end.  */
static tree
pl_generate_extern_var_bounds (tree var)
{
  const char *prefix = "__pl_end_of_";
  const char *var_name = IDENTIFIER_POINTER (DECL_NAME (var));
  tree bounds;
  tree end_decl;
  tree var_end;
  tree lb;
  tree size;
  char *end_name;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generating bounds for extern symbol '");
      print_generic_expr (dump_file, var, 0);
      fprintf (dump_file, "'\n");
    }

  end_name = (char *) xmalloc (strlen (var_name) + strlen (prefix) + 1);
  strcpy (end_name, prefix);
  strcat (end_name, var_name);

  end_decl = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			 get_identifier (end_name), TREE_TYPE (var));
  TREE_PUBLIC (end_decl) = 1;
  DECL_EXTERNAL (end_decl) = 1;
  DECL_ARTIFICIAL (end_decl) = 1;

  lb = build1 (ADDR_EXPR,
	       build_pointer_type (TREE_TYPE (var)), var);
  var_end = build1 (ADDR_EXPR,
		    build_pointer_type (TREE_TYPE (end_decl)), end_decl);
  size = size_binop (MINUS_EXPR,
		     fold_convert (pl_uintptr_type, var_end),
		     fold_convert (pl_uintptr_type, lb));

  bounds = pl_make_bounds (lb, size, NULL);

  free (end_name);

  return bounds;
}

static tree
pl_get_bounds_for_decl (tree decl)
{
  tree bounds;
  tree lb;
  tree size;

  gcc_assert (TREE_CODE (decl) == VAR_DECL
	      || TREE_CODE (decl) == PARM_DECL);

  bounds = pl_get_registered_bounds (decl);

  if (bounds)
    return bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Building bounds for decl ");
      print_generic_expr (dump_file, decl, 0);
      fprintf (dump_file, "\n");
    }

  lb = fold_build1 (ADDR_EXPR, build_pointer_type (TREE_TYPE (decl)), decl);

  if (DECL_SIZE (decl))
    {
      /* We need size in bytes rounded up.  */
      size = build_int_cst (size_type_node,
			    (tree_low_cst (DECL_SIZE (decl), 1) + 7) / 8);
      bounds = pl_make_bounds (lb, size, NULL);
    }
  else
    {
      gcc_assert (TREE_CODE (decl) == VAR_DECL);
      warning (0, "using zero bounds for var with incomplete type\n");
      bounds = pl_get_zero_bounds ();
      /*bounds = pl_generate_extern_var_bounds (decl);*/
    }

  pl_register_bounds (decl, bounds);

  return bounds;
}

static tree
pl_get_bounds_for_string_cst (tree cst)
{
  tree bounds;
  tree lb;
  tree size;

  gcc_assert (TREE_CODE (cst) == STRING_CST);

  bounds = pl_get_registered_bounds (cst);

  if (bounds)
    return bounds;

  lb = fold_build1 (ADDR_EXPR, build_pointer_type (TREE_TYPE (cst)), cst);
  size = build_int_cst (pl_uintptr_type, TREE_STRING_LENGTH (cst));
  bounds = pl_make_bounds (lb, size, NULL);

  pl_register_bounds (cst, bounds);

  return bounds;
}

static tree
pl_make_addressed_object_bounds (tree obj, gimple_stmt_iterator iter)
{
  tree bounds;

  switch (TREE_CODE (obj))
    {
    case VAR_DECL:
    case PARM_DECL:
      bounds = pl_get_bounds_for_decl (obj);
      break;

    case STRING_CST:
      bounds = pl_get_bounds_for_string_cst (obj);
      break;

    case ARRAY_REF:
    case COMPONENT_REF:
      {
	tree elt;
	tree ptr;
	bool component;
	bool bitfield;

	pl_parse_array_and_component_ref (obj, &ptr, &elt, &component,
					  &bitfield, &bounds, iter, true);

	gcc_assert (bounds);
      }
      break;

    case FUNCTION_DECL:
      bounds = pl_get_zero_bounds ();
      break;

    case MEM_REF:
      bounds = pl_find_bounds (TREE_OPERAND (obj, 0), iter);
      break;

    default:
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "pl_make_addressed_object_bounds: "
		   "unexpected object of type %s\n",
		   tree_code_name[TREE_CODE (obj)]);
	  print_node (dump_file, "", obj, 0);
	}
      internal_error ("pl_make_addressed_object_bounds: Unexpected tree code %s",
		      tree_code_name[TREE_CODE (obj)]);
    }

  return bounds;
}

static tree
pl_find_bounds (tree ptr, gimple_stmt_iterator iter)
{
  tree bounds = NULL_TREE;

  switch (TREE_CODE (ptr))
    {
    case SSA_NAME:
      bounds = pl_get_registered_bounds (ptr);
      if (!bounds)
	{
	  gimple def_stmt = SSA_NAME_DEF_STMT (ptr);

	  bounds = pl_get_bounds_by_definition(ptr, def_stmt, &iter);

	  gcc_assert (bounds);

	  if (gimple_code (def_stmt) == GIMPLE_PHI)
	    {
	      /* pl_get_bounds_by_definition created new phi
		 statement and iter points to it.  */
	      gimple phi_bnd = gsi_stmt (iter);
	      unsigned i;

	      for (i = 0; i < gimple_phi_num_args (def_stmt); i++)
		{
		  tree arg = gimple_phi_arg_def (def_stmt, i);
		  tree arg_bnd;

		  arg_bnd = pl_find_bounds (arg, iter);

		  add_phi_arg (phi_bnd, arg_bnd,
			       gimple_phi_arg_edge (def_stmt, i),
			       UNKNOWN_LOCATION);
		}
	    }

	  gcc_assert (bounds == pl_get_registered_bounds (ptr));
	}
      break;

    case ADDR_EXPR:
      bounds = pl_make_addressed_object_bounds (TREE_OPERAND (ptr, 0), iter);
      break;

    case INTEGER_CST:
      /* For all constants return zero bounds.  */
      bounds = pl_get_none_bounds ();
      break;

    default:
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "pl_find_bounds: unexpected ptr of type %s\n",
		   tree_code_name[TREE_CODE (ptr)]);
	  print_node (dump_file, "", ptr, 0);
	}
      internal_error ("pl_find_bounds: Unexpected tree code %s",
		      tree_code_name[TREE_CODE (ptr)]);
    }

  if (!bounds)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (stderr, "pl_find_bounds: cannot find bounds for pointer\n");
	  print_node (dump_file, "", ptr, 0);
	}
      internal_error ("pl_find_bounds: Cannot find bounds for pointer");
    }

  return bounds;
}

static tree
pl_intersect_bounds (tree bounds1, tree bounds2, gimple_stmt_iterator iter)
{
  if (!bounds1)
    return bounds2;
  else if (!bounds2)
    return bounds1;
  else
    {
        gimple_seq seq;
	gimple stmt;
	tree bounds;

	seq = gimple_seq_alloc ();

	stmt = gimple_build_call (pl_intersect_fndecl, 2, bounds1, bounds2);
	pl_mark_stmt (stmt);

	bounds = create_tmp_reg (pl_bound_type, BOUND_TMP_NAME);
	add_referenced_var (bounds);
	bounds = make_ssa_name (bounds, stmt);
	gimple_call_set_lhs (stmt, bounds);

	gimple_seq_add_stmt (&seq, stmt);

	gsi_insert_seq_before (&iter, seq, GSI_SAME_STMT);

	if (dump_file && (dump_flags & TDF_DETAILS))
	  {
	    fprintf (dump_file, "Bounds intersection: ");
	    print_gimple_stmt (dump_file, stmt, 0, TDF_VOPS|TDF_MEMSYMS);
	    fprintf (dump_file, "  inserted before statement: ");
	    print_gimple_stmt (dump_file, gsi_stmt (iter), 0,
			       TDF_VOPS|TDF_MEMSYMS);
	  }

	return bounds;
    }
}

static void
pl_parse_array_and_component_ref (tree node, tree *ptr,
				  tree *elt, bool *component,
				  bool *bitfield,
				  tree *bounds,
				  gimple_stmt_iterator iter,
				  bool innermost_bounds)
{
  tree var = TREE_OPERAND (node, 0);
  bool precise_bounds = false;

  *component = (TREE_CODE (node) == COMPONENT_REF);
  *bitfield = (TREE_CODE (node) == COMPONENT_REF
	       && DECL_BIT_FIELD_TYPE (TREE_OPERAND (node, 1)));
  *elt = NULL_TREE;
  *bounds = NULL_TREE;

  if (TREE_CODE (node) == COMPONENT_REF && innermost_bounds)
    {
      tree bit_size = DECL_SIZE (TREE_OPERAND (node, 1));
      HOST_WIDE_INT size = (tree_low_cst (bit_size, 1) + 7) / 8;
      tree node_type = TREE_TYPE (TREE_OPERAND (node, 1));
      tree field_ptr = fold_build1 (ADDR_EXPR,
				    build_pointer_type (node_type),
				    node);

      *bounds = pl_make_bounds (field_ptr,
				build_int_cst (size_type_node, size),
				&iter);
    }
  else if (TREE_CODE (node) == ARRAY_REF)
    {
      tree array_addr;

      array_addr = build1 (ADDR_EXPR,
			   build_pointer_type (TREE_TYPE (var)), var);

      *bounds = pl_find_bounds (array_addr, iter);

      precise_bounds = true;
      /* According to C standard we may use such pointer to access whole array
      if (innermost_bounds)
	{
	  tree lb = build1 (ADDR_EXPR,
			   build_pointer_type (TREE_TYPE (node)), node);
	  tree elem_bounds = pl_make_bounds (lb, array_ref_element_size (node),
					     &iter);

	  *bounds = pl_intersect_bounds (elem_bounds, *bounds, iter);
	  }*/
    }

  while (true)
    {
      if (*bitfield && *elt == NULL_TREE
	  && (TREE_CODE (var) == ARRAY_REF
	      || TREE_CODE (var) == COMPONENT_REF))
	*elt = var;

      if (TREE_CODE (var) == ARRAY_REF)
	{
	  tree array_addr;

	  *component = false;
	  var = TREE_OPERAND (var, 0);

	  array_addr = build1 (ADDR_EXPR,
			       build_pointer_type (TREE_TYPE (var)), var);

	  if (!*bounds || !precise_bounds)
	    {
	      *bounds = pl_intersect_bounds (pl_find_bounds (array_addr, iter),
					     *bounds, iter);
	      precise_bounds = true;
	    }
	}
      else if (TREE_CODE (var) == COMPONENT_REF)
	{
	  var = TREE_OPERAND (var, 0);
	}
      else if (INDIRECT_REF_P (var)
	       || TREE_CODE (var) == MEM_REF)
	{
	  *component = false;
	  *ptr = TREE_OPERAND (var, 0);

	  if (!*bounds || !precise_bounds)
	    {
	      *bounds = pl_intersect_bounds (pl_find_bounds (*ptr, iter),
					     *bounds, iter);
	      precise_bounds = true;
	    }
	  break;
	}
      else if (TREE_CODE (var) == VIEW_CONVERT_EXPR)
	{
	  gcc_unreachable (); /* look at it later */

	  var = TREE_OPERAND (var, 0);
	  if (CONSTANT_CLASS_P (var)
	      && TREE_CODE (var) != STRING_CST)
	    break;
	}
      else
	{
	  gcc_assert (TREE_CODE (var) == VAR_DECL
		      || TREE_CODE (var) == PARM_DECL
		      || TREE_CODE (var) == RESULT_DECL
		      || TREE_CODE (var) == STRING_CST);

	  *ptr = build1 (ADDR_EXPR,
			 build_pointer_type (TREE_TYPE (var)), var);
	  break;
	}
    }
}

static void
pl_process_stmt (gimple_stmt_iterator *iter, tree node,
		 location_t loc, tree dirflag,
		 tree access_offs, tree access_size)
{
  tree node_type = TREE_TYPE (node);
  tree size = access_size ? access_size : TYPE_SIZE_UNIT (node_type);
  tree addr_first = NULL_TREE; /* address of the first accessed byte */
  tree addr_last = NULL_TREE; /* address of the last accessed byte */
  tree ptr = NULL_TREE; /* a pointer used for dereference */
  tree bounds = NULL_TREE;
  bool safe = false;

  // TODO: check we need to instrument this node

  switch (TREE_CODE (node))
    {
    case ARRAY_REF:
    case COMPONENT_REF:
      {
	bool bitfield;
	tree elt;

	pl_parse_array_and_component_ref (node, &ptr, &elt, &safe,
					  &bitfield, &bounds, *iter, false);

	/* Break if there is no dereference and operation is safe.  */
	if (safe)
	  {
	    /* We may still need addr_first for bndstx in case
	       write a pointer into memory.  */
	    addr_first = fold_build1_loc (loc, ADDR_EXPR,
					  build_pointer_type (node_type),
					  node);
	    break;
	  }

	if (bitfield)
          {
            tree field = TREE_OPERAND (node, 1);

            if (TREE_CODE (DECL_SIZE_UNIT (field)) == INTEGER_CST)
              size = DECL_SIZE_UNIT (field);

	    if (elt)
	      elt = build1 (ADDR_EXPR, build_pointer_type (TREE_TYPE (elt)),
			    elt);
            addr_first = fold_convert_loc (loc, ptr_type_node, elt ? elt : ptr);
            addr_first = fold_build_pointer_plus_loc (loc,
						      addr_first,
						      byte_position (field));
          }
        else
          addr_first = build1 (ADDR_EXPR, build_pointer_type (node_type), node);
      }
      break;

    case INDIRECT_REF:
      ptr = TREE_OPERAND (node, 0);
      addr_first = ptr;
      break;

    case MEM_REF:
      ptr = TREE_OPERAND (node, 0);
      addr_first = fold_build_pointer_plus_loc (loc, ptr,
						TREE_OPERAND (node, 1));
      break;

    case TARGET_MEM_REF:
      ptr = TMR_BASE (node);
      addr_first = tree_mem_ref_addr (ptr_type_node, node);
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

	bpu = bitsize_int (BITS_PER_UNIT);
	offs = fold_convert (size_type_node, TREE_OPERAND (node, 2));
	rem = size_binop_loc (loc, TRUNC_MOD_EXPR, offs, bpu);
	offs = size_binop_loc (loc, TRUNC_DIV_EXPR, offs, bpu);

	size = fold_convert (bitsizetype, TREE_OPERAND (node, 1));
        size = size_binop_loc (loc, PLUS_EXPR, size, rem);
        size = size_binop_loc (loc, CEIL_DIV_EXPR, size, bpu);
        size = fold_convert (size_type_node, size);

	pl_process_stmt (iter, TREE_OPERAND (node, 0), loc,
			 dirflag, offs, size);
	return;
      }
      break;

    case VAR_DECL:
      if (dirflag != integer_one_node)
	return;

      safe = 1;
      addr_first = fold_build1 (ADDR_EXPR,
				build_pointer_type (TREE_TYPE (node)), node);
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
      if (!bounds)
	bounds = pl_find_bounds (ptr, *iter);

      pl_check_mem_access (addr_first, addr_last, bounds, iter, loc, dirflag);
    }

  /* We need to generate bndstx in case pointer is stored.  */
  if (dirflag == integer_one_node && POINTER_TYPE_P (node_type))
    {
      gimple stmt = gsi_stmt (*iter);

      gcc_assert ( gimple_code(stmt) == GIMPLE_ASSIGN);

      if (TREE_CLOBBER_P (gimple_assign_rhs1 (stmt)))
	{
	  /* Probably we should perform bndstx with zero bounds for all
	     clobbered pointers.  Currently just ignore it.  */
	}
      else
	{
	  bounds = pl_compute_bounds_for_assignment (NULL_TREE, stmt);
	  pl_build_bndstx (addr_first, gimple_assign_rhs1 (stmt),
			   bounds, *iter);
	}
    }
}

/* Add input bound arguments declaration to the function
   declaration DECL.  Do nothing if function already have
   bound arguments.  If MAKE_SSA_NAMES is set then ssa name
   is created and registered as a bound for each added
   argument.  */
static void
pl_fix_function_decl (tree decl, bool make_ssa_names)
{
  tree arg = DECL_ARGUMENTS (decl);
  tree prev_arg = NULL_TREE;
  bool already_fixed = false;
  int bnd_no = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file,
	       "Building bound arguments for funtion declaration '");
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
	      pl_register_bounds (prev_arg, bounds);

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
     is placed right before pointer arg.  Also make
     register ssa name for each bound if needed.  */
  for (arg = DECL_ARGUMENTS (decl); arg; arg = TREE_CHAIN (arg))
    {
      if (POINTER_TYPE_P (TREE_TYPE (arg)))
	{
	  char name_buf[20];
	  tree name;
	  tree bounds;

	  sprintf (name_buf, "__arg_bnd.%d", bnd_no++);
	  name = get_identifier (name_buf);

	  bounds = build_decl (UNKNOWN_LOCATION, PARM_DECL, name,
				 pl_bound_type);
	  DECL_ARG_TYPE (bounds) = pl_bound_type;

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
	      pl_register_bounds (arg, ssa);
	    }

	  /* Skip inserted bound arg.  */
	  arg = TREE_CHAIN (arg);
	}
    }
}

static void
pl_fix_function_decls (void)
{
  basic_block bb;
  gimple_stmt_iterator i;

  pl_fix_function_decl (cfun->decl, true);

  FOR_ALL_BB (bb)
    for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
      {
	gimple stmt = gsi_stmt (i);
	if (is_gimple_call (stmt))
	  {
	    tree fndecl = gimple_call_fndecl (stmt);
	    if (fndecl)
	      pl_fix_function_decl (fndecl, false);
	  }
      }
}

static void
pl_init (void)
{
  pl_reg_bounds = htab_create_ggc (31, tree_map_hash, tree_map_eq,
				   NULL);
  pl_marked_stmts = htab_create_ggc (31, htab_hash_pointer, htab_eq_pointer,
				     NULL);

  entry_block = NULL;
  zero_bounds = NULL_TREE;
  none_bounds = NULL_TREE;

  pl_bound_type = TARGET_64BIT ? bound64_type_node : bound32_type_node;
  pl_uintptr_type = lang_hooks.types.type_for_mode (ptr_mode, true);

  /* Build declarations for builtin functions.  */
  pl_bndldx_fndecl = targetm.builtin_pl_function (BUILT_IN_PL_BNDLDX);
  pl_bndstx_fndecl = targetm.builtin_pl_function (BUILT_IN_PL_BNDSTX);
  pl_checkl_fndecl = targetm.builtin_pl_function (BUILT_IN_PL_BNDCL);
  pl_checku_fndecl = targetm.builtin_pl_function (BUILT_IN_PL_BNDCU);
  pl_bndmk_fndecl = targetm.builtin_pl_function (BUILT_IN_PL_BNDMK);
  pl_ret_bnd_fndecl = targetm.builtin_pl_function (BUILT_IN_PL_BNDRET);
  pl_intersect_fndecl = targetm.builtin_pl_function (BUILT_IN_PL_INTERSECT);
}

static void
pl_fini (void)
{

}

static unsigned int
pl_execute (void)
{
  //TODO: check we need to instrument this function
  pl_init ();

  pl_fix_function_decls ();
  pl_transform_function ();

  pl_fini ();

  return 0;
}

static bool
pl_gate (void)
{
  return flag_pl != 0;
}

struct gimple_opt_pass pass_pl =
{
 {
  GIMPLE_PASS,
  "pl",                                 /* name */
  pl_gate,                              /* gate */
  pl_execute,                           /* execute */
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
