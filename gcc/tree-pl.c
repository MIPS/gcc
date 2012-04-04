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

static unsigned int pl_execute (void);
static bool pl_gate (void);

static tree pl_make_builtin (enum tree_code category, const char *name,
			     tree type);
static void pl_init (void);
static void pl_fini (void);
static void pl_register_bounds (tree ptr, tree bnd);
static tree pl_get_registered_bounds (tree ptr);
static basic_block pl_get_entry_block (void);
static tree pl_get_zero_bounds (void);
static void pl_transform_function (void);
static tree pl_build_bound_for_arg_ptr (tree arg, int arg_no);
static tree pl_get_bound_for_arg_ptr (tree arg);
static tree pl_build_bndldx (tree addr, tree ptr, gimple_stmt_iterator gsi);
static void pl_build_bndstx (tree addr, tree ptr, tree bounds,
			     gimple_stmt_iterator gsi);
static tree pl_compute_bounds_for_assignment (tree node, gimple assign);
static tree pl_make_bounds (tree lb, tree size, gimple_stmt_iterator *iter);
static tree pl_make_addressed_object_bounds (tree obj,
					     gimple_stmt_iterator *iter);
static tree pl_get_bounds_for_var_decl (tree var);
static tree pl_get_bounds_for_string_cst (tree cst);
static tree pl_get_bounds_by_definition (tree node, gimple def_stmt,
					 gimple_stmt_iterator *iter);
static tree pl_find_bounds (tree ptr, gimple_stmt_iterator *iter);
static void pl_check_mem_access (tree first, tree last, tree bounds,
				 gimple_stmt_iterator *instr_gsi,
				 location_t location, tree dirflag);
static void pl_parse_array_and_component_ref (tree node, tree *ptr,
					      tree *elt, bool *component,
					      bool *bitfield);
static void pl_process_stmt(gimple_stmt_iterator *iter, tree *tp,
			    location_t loc, tree dirflag);

static GTY (()) tree pl_arg_bnd_register_fntype;
static GTY (()) tree pl_bndldx_register_fntype;
static GTY (()) tree pl_bndstx_register_fntype;
static GTY (()) tree pl_checkl_register_fntype;
static GTY (()) tree pl_checku_register_fntype;
static GTY (()) tree pl_mkbnd_register_fntype;
static GTY (()) tree pl_ret_bnd_register_fntype;
static GTY (()) tree pl_arg_bnd_fndecl;
static GTY (()) tree pl_bndldx_fndecl;
static GTY (()) tree pl_bndstx_fndecl;
static GTY (()) tree pl_checkl_fndecl;
static GTY (()) tree pl_checku_fndecl;
static GTY (()) tree pl_mkbnd_fndecl;
static GTY (()) tree pl_ret_bnd_fndecl;

static GTY (()) tree pl_bnd_record;
static GTY (()) tree pl_uintptr_type;

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

          switch (gimple_code (s))
            {
            case GIMPLE_ASSIGN:
	      pl_process_stmt (&i, gimple_assign_lhs_ptr (s),
			       gimple_location (s), integer_one_node);
	      pl_process_stmt (&i, gimple_assign_rhs1_ptr (s),
			       gimple_location (s), integer_zero_node);
	      grhs_class = get_gimple_rhs_class (gimple_assign_rhs_code (s));
	      if (grhs_class == GIMPLE_BINARY_RHS)
		pl_process_stmt (&i, gimple_assign_rhs2_ptr (s),
				 gimple_location (s), integer_zero_node);
              break;

            case GIMPLE_RETURN:
              if (gimple_return_retval (s) != NULL_TREE)
                {
                  pl_process_stmt (&i, gimple_return_retval_ptr (s),
				   gimple_location (s),
				   integer_zero_node);
                }
              break;

            default:
              ;
            }
        }
      bb = next;
    }
  while (bb && bb->index <= saved_last_basic_block);
}

static void
pl_check_mem_access (tree first, tree last, tree bounds,
		     gimple_stmt_iterator *instr_gsi,
		     location_t location, tree dirflag)
{
  gimple_seq seq, stmts;
  gimple stmt;
  tree node;

  seq = gimple_seq_alloc ();

  node = force_gimple_operand (first, &stmts, true, NULL_TREE);
  gimple_seq_add_seq (&seq, stmts);

  stmt = gimple_build_call (pl_checkl_fndecl, 2, node, bounds);
  gimple_seq_add_stmt (&seq, stmt);

  node = force_gimple_operand (last, &stmts, true, NULL_TREE);
  gimple_seq_add_seq (&seq, stmts);

  stmt = gimple_build_call (pl_checku_fndecl, 2, node, bounds);
  gimple_seq_add_stmt (&seq, stmt);

  gsi_insert_seq_before (instr_gsi, seq, GSI_SAME_STMT);
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
  static basic_block entry_block = NULL;

  if (!entry_block)
    {
      basic_block prev_entry = ENTRY_BLOCK_PTR->next_bb;
      edge e = find_edge (ENTRY_BLOCK_PTR, prev_entry);
      entry_block = split_edge (e);
      //entry_block = create_empty_bb (ENTRY_BLOCK_PTR);
      //make_edge (ENTRY_BLOCK_PTR, entry_block, EDGE_FALLTHRU);
      //redirect_edge_pred (e, entry_block);
    }

  return entry_block;
}

static tree
pl_get_zero_bounds (void)
{
  static tree zero_bounds = NULL_TREE;

  if (zero_bounds)
    return zero_bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Creating zero bounds...");

  zero_bounds = pl_make_bounds (integer_zero_node,
				integer_minus_one_node,
				NULL);

  return zero_bounds;
}

static tree
pl_build_bound_for_arg_ptr (tree arg, int arg_no)
{
  tree bounds;
  gimple_stmt_iterator gsi;
  gimple stmt;
  basic_block bb;

  /* Check if we've already built required bounds.  */
  bounds = pl_get_registered_bounds (arg);
  if (bounds)
    return bounds;

  stmt = gimple_build_call (pl_arg_bnd_fndecl, 1,
			    build_int_cst (integer_type_node, arg_no));
  bb = pl_get_entry_block ();
  gsi = gsi_start_bb (bb);
  gsi_insert_before (&gsi, stmt, GSI_CONTINUE_LINKING);

  bounds = create_tmp_reg (pl_bnd_record, NULL);
  add_referenced_var (bounds);
  bounds = make_ssa_name (bounds, stmt);
  gimple_call_set_lhs (stmt, bounds);

  update_stmt (stmt);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Built bounds for arg_%d ( ", arg_no);
      print_generic_expr (dump_file, arg, 0);

      fprintf (dump_file, " [");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, " of type ");
      print_generic_expr (dump_file, TREE_TYPE (bounds), 0);
      fprintf (dump_file, " ]");

      fprintf (dump_file, "): ");
      print_gimple_stmt (dump_file, stmt, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  pl_register_bounds (arg, bounds);

  return bounds;
}

static tree
pl_build_returned_bound (gimple call)
{
  gimple_stmt_iterator gsi = gsi_for_stmt (call);
  tree bounds;
  gimple stmt;
  basic_block bb;

  stmt = gimple_build_call (pl_ret_bnd_fndecl, 0);
  gsi_insert_after (&gsi, stmt, GSI_SAME_STMT);

  bounds = create_tmp_reg (pl_bnd_record, NULL);
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
pl_get_bound_for_arg_ptr (tree arg)
{
  tree type = TREE_TYPE (arg);
  tree decl = cfun->decl;
  tree args = DECL_ARGUMENTS (decl);
  int ptr_no = 0;

  while (args != arg)
    {
      enum tree_code arg_type = TREE_CODE (TREE_TYPE (args));
      if (arg_type == POINTER_TYPE
	  || arg_type == ARRAY_TYPE
	  || arg_type == REFERENCE_TYPE)
	ptr_no++;
      args = TREE_CHAIN (args);
    }

  switch (TREE_CODE (type))
    {
    case POINTER_TYPE:
    case ARRAY_TYPE:
    case REFERENCE_TYPE:
      return pl_build_bound_for_arg_ptr (arg, ptr_no);
      break;

    case OFFSET_TYPE:
      fprintf (stderr, "pl_get_bound_for_arg_ptr: OFFSET_TYPE is NYI\n");
      gcc_unreachable ();
      break;

    default:
      /* For non pointer types use zero bounds.  */
      return pl_get_zero_bounds ();
    }
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
  bounds = create_tmp_reg (pl_bnd_record, NULL);
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

  stmt = gimple_build_call (pl_bndstx_fndecl, 2, addr, ptr, bounds);

  gimple_seq_add_stmt (&seq, stmt);

  gsi_insert_seq_after (&gsi, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generated bndstx for pointer store ");
      print_gimple_stmt (dump_file, gsi_stmt (gsi), 0, TDF_VOPS|TDF_MEMSYMS);
      print_gimple_stmt (dump_file, stmt, 2, TDF_VOPS|TDF_MEMSYMS);
    }
}

static tree
pl_compute_bounds_for_assignment (tree node, gimple assign)
{
  enum tree_code rhs_code = gimple_assign_rhs_code (assign);
  location_t loc = gimple_location (assign);
  tree rhs1 = gimple_assign_rhs1 (assign);
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

      if (POINTER_TYPE_P (TREE_TYPE (TREE_TYPE (offs))))
	{
	  /* In this case we must use bndldx to load bounds.  */
	  addr = fold_build_pointer_plus_loc (loc, ptr, offs);
	  bounds = pl_build_bndldx (addr, node, gsi_for_stmt (assign));
	}
      else
	/* If we load a non pointer value then use zero bounds.  */
	bounds = pl_get_zero_bounds ();
      break;

    case ARRAY_REF:
    case COMPONENT_REF:
      {
	tree elt;
	bool component;
	bool bitfield;

	pl_parse_array_and_component_ref (rhs1, &ptr, &elt, &component,
					  &bitfield);
	iter = gsi_for_stmt (assign);
	bounds = pl_find_bounds (ptr, &iter);
      }
      break;

    case SSA_NAME:
    case ADDR_EXPR:
    case POINTER_PLUS_EXPR:
      iter = gsi_for_stmt (assign);
      bounds = pl_find_bounds (rhs1, &iter);
      break;

    case INTEGER_CST:
      bounds = pl_get_zero_bounds ();
      break;

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
	  bounds = pl_get_bound_for_arg_ptr (var);
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
      bounds = create_tmp_reg (pl_bnd_record, NULL);
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

  stmt = gimple_build_call (pl_mkbnd_fndecl, 2, lb, size);

  bounds = create_tmp_reg (pl_bnd_record, NULL);
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
	  fprintf (dump_file, "  Inserted before statement: ");
	  print_gimple_stmt (dump_file, gsi_stmt (gsi), 0, TDF_VOPS|TDF_MEMSYMS);
	}
      else
	fprintf (dump_file, "At function entry");
    }

  /* update_stmt (stmt); */

  return bounds;
}

static tree
pl_get_bounds_for_var_decl (tree var)
{
  tree bounds;
  tree lb;
  tree size;

  gcc_assert (TREE_CODE (var) == VAR_DECL);

  bounds = pl_get_registered_bounds (var);

  if (bounds)
    return bounds;

  lb = fold_build1 (ADDR_EXPR, build_pointer_type (TREE_TYPE (var)), var);
  size = DECL_SIZE (var);
  bounds = pl_make_bounds (lb, size, NULL);

  pl_register_bounds (var, bounds);

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
pl_make_addressed_object_bounds (tree obj, gimple_stmt_iterator *iter)
{
  tree bounds;

  switch (TREE_CODE (obj))
    {
    case VAR_DECL:
      bounds = pl_get_bounds_for_var_decl (obj);
      break;

    case STRING_CST:
      bounds = pl_get_bounds_for_string_cst (obj);
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
pl_find_bounds (tree ptr, gimple_stmt_iterator *iter)
{
  tree bounds = NULL_TREE;

  switch (TREE_CODE (ptr))
    {
    case SSA_NAME:
      bounds = pl_get_registered_bounds (ptr);
      if (!bounds)
	{
	  gimple def_stmt = SSA_NAME_DEF_STMT (ptr);

	  bounds = pl_get_bounds_by_definition(ptr, def_stmt, iter);

	  gcc_assert (bounds);

	  if (gimple_code (def_stmt) == GIMPLE_PHI)
	    {
	      gimple phi_bnd = gsi_stmt (*iter);
	      int i;

	      for (i = 0; i < gimple_phi_num_args (def_stmt); i++)
		{
		  tree arg = gimple_phi_arg_def (def_stmt, i);
		  tree arg_bnd;

		  gcc_assert (arg && (TREE_CODE (arg) == SSA_NAME));

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

static void
pl_parse_array_and_component_ref (tree node, tree *ptr,
				  tree *elt, bool *component,
				  bool *bitfield)
{
  tree var = TREE_OPERAND (node, 0);

  *component = (TREE_CODE (node) == COMPONENT_REF);
  *bitfield = (TREE_CODE (node) == COMPONENT_REF
	       && DECL_BIT_FIELD_TYPE (TREE_OPERAND (node, 1)));
  *elt = NULL_TREE;

  while (true)
    {
      if (*bitfield && *elt == NULL_TREE
	  && (TREE_CODE (var) == ARRAY_REF
	      || TREE_CODE (var) == COMPONENT_REF))
	*elt = var;

      if (TREE_CODE (var) == ARRAY_REF)
	{
	  *component = false;
	  var = TREE_OPERAND (var, 0);
	}
      else if (TREE_CODE (var) == COMPONENT_REF)
	var = TREE_OPERAND (var, 0);
      else if (INDIRECT_REF_P (var)
	       || TREE_CODE (var) == MEM_REF)
	{
	  *ptr = TREE_OPERAND (var, 0);
	  break;
	}
      else if (TREE_CODE (var) == VIEW_CONVERT_EXPR)
	{
	  var = TREE_OPERAND (var, 0);
	  if (CONSTANT_CLASS_P (var)
	      && TREE_CODE (var) != STRING_CST)
	    return;
	}
      else
	{
	  gcc_assert (TREE_CODE (var) == VAR_DECL
		      || TREE_CODE (var) == PARM_DECL
		      || TREE_CODE (var) == RESULT_DECL
		      || TREE_CODE (var) == STRING_CST);

	  if (*component)
	    return;
	  else
	    {
	      *ptr = build1 (ADDR_EXPR,
			     build_pointer_type (TREE_TYPE (var)), var);
	      break;
	    }
	}
    }
}

static void
pl_process_stmt (gimple_stmt_iterator *iter, tree *tp,
		 location_t loc, tree dirflag)
{
  tree node = *tp;
  tree node_type = TREE_TYPE (node);
  tree size = TYPE_SIZE_UNIT (node_type);
  tree addr_first = NULL_TREE; /* address of the first accessed byte */
  tree addr_end = NULL_TREE; /* address of the byte past the last accessed byte */
  tree addr_last = NULL_TREE; /* address of the last accessed byte */
  tree ptr = NULL_TREE; /* a pointer used for dereference */
  tree bounds;
  bool safe = false;

  // TODO: check we need to instrument this node

  switch (TREE_CODE (node))
    {
    case ARRAY_REF:
    case COMPONENT_REF:
      {
	bool bitfield;
	bool component;
	tree elt;

	pl_parse_array_and_component_ref (node, &ptr, &elt, &safe,
					  &bitfield);

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

        addr_last = fold_build2_loc (loc, MINUS_EXPR, pl_uintptr_type,
                             fold_build2_loc (loc, PLUS_EXPR, pl_uintptr_type,
					  fold_convert (pl_uintptr_type, addr_first),
					  size),
                             size_one_node);
      }
      break;

    case INDIRECT_REF:
      ptr = TREE_OPERAND (node, 0);
      addr_first = ptr;
      addr_end = fold_build_pointer_plus_loc (loc, addr_first, size);
      addr_last = fold_build_pointer_plus_hwi_loc (loc, addr_end, -1);
      break;

    case MEM_REF:
      ptr = TREE_OPERAND (node, 0);
      addr_first = fold_build_pointer_plus_loc (loc, ptr,
						TREE_OPERAND (node, 1));
      addr_end = fold_build_pointer_plus_loc (loc, addr_first, size);
      addr_last = fold_build_pointer_plus_hwi_loc (loc, addr_end, -1);
      break;

    case TARGET_MEM_REF:
      printf("TARGET_MEM_REF\n");
      debug_gimple_stmt(gsi_stmt(*iter));
      debug_tree(node);
      gcc_unreachable ();
      break;

    case ARRAY_RANGE_REF:
      printf("ARRAY_RANGE_REF\n");
      debug_gimple_stmt(gsi_stmt(*iter));
      debug_tree(node);
      gcc_unreachable ();
      break;

    case BIT_FIELD_REF:
      printf("BIT_FIELD_REF\n");
      debug_gimple_stmt(gsi_stmt(*iter));
      debug_tree(node);
      gcc_unreachable ();
      break;

    default:
      return;
    }

  if (!safe)
    {
      bounds = pl_find_bounds (ptr, iter);
      pl_check_mem_access (addr_first, addr_last, bounds, iter, loc, dirflag);
    }

  /* We need to generate bndstx in case pointer is stored.  */
  if (dirflag == integer_one_node && POINTER_TYPE_P (node_type))
    {
      gimple stmt = gsi_stmt (*iter);

      gcc_assert ( gimple_code(stmt) == GIMPLE_ASSIGN);

      bounds = pl_compute_bounds_for_assignment (NULL_TREE, stmt);
      pl_build_bndstx (addr_first, gimple_assign_rhs1 (stmt), bounds, *iter);
    }
}

static tree
pl_make_builtin (enum tree_code category, const char *name, tree type)
{
  tree decl = build_decl (UNKNOWN_LOCATION,
			  category, get_identifier (name), type);
  TREE_PUBLIC (decl) = 1;

  /*  It is a builtin.  */
  DECL_SOURCE_LOCATION (decl) = BUILTINS_LOCATION;
  /* for now do not mark as built in because it is not expanded and will fail */
  /* DECL_BUILT_IN_CLASS (decl) = BUILT_IN_NORMAL; */

  /* lang_hooks.decls.pushdecl (decl);  */

  /* Declared by the compiler.  */
  DECL_ARTIFICIAL (decl) = 1;
  /* No debug info for it.  */
  DECL_IGNORED_P (decl) = 1;
  return decl;
}

static void
pl_init (void)
{
  /* Allocate hash table for bounds.  */
  pl_reg_bounds = htab_create_ggc (31, tree_map_hash, tree_map_eq,
				   NULL);

  /* Build bound structure type.  */
  /*
  tree field_lb = build_decl (UNKNOWN_LOCATION, FIELD_DECL,
			      get_identifier ("lb"), ptr_type_node);
  tree field_ub = build_decl (UNKNOWN_LOCATION, FIELD_DECL,
			      get_identifier ("ub"), ptr_type_node);
  pl_bnd_record = make_node (RECORD_TYPE);
  DECL_CONTEXT (field_lb) = pl_bnd_record;
  DECL_CONTEXT (field_ub) = pl_bnd_record;
  DECL_CHAIN (field_lb) = field_ub;
  TYPE_FIELDS (pl_bnd_record) = field_lb;
  TYPE_NAME (pl_bnd_record) = get_identifier ("__bnd");
  layout_type (pl_bnd_record);
  */
  /* Structure causes fails in SSA verifier for some reason.
     There will be special builtin type in future and for now
     I just use complex type of appropriate size.  */
  pl_bnd_record = TARGET_64BIT ? int128_unsigned_type_node : long_long_unsigned_type_node;
  pl_bnd_record = build_complex_type (TARGET_64BIT
				      ? long_long_unsigned_type_node
				      : unsigned_type_node);
  pl_uintptr_type = lang_hooks.types.type_for_mode (ptr_mode, true);

  /* Build types for intrinsic functions.  */
  pl_arg_bnd_register_fntype =
    build_function_type_list (pl_bnd_record, integer_type_node, NULL_TREE);
  pl_bndldx_register_fntype =
    build_function_type_list (pl_bnd_record, ptr_type_node, ptr_type_node,
			      NULL_TREE);
  pl_bndstx_register_fntype =
    build_function_type_list (void_type_node, ptr_type_node, ptr_type_node,
			      pl_bnd_record, NULL_TREE);
  pl_checku_register_fntype =
    build_function_type_list (void_type_node, ptr_type_node, pl_bnd_record,
			      NULL_TREE);
  pl_checkl_register_fntype = pl_checku_register_fntype;
  pl_mkbnd_register_fntype =
    build_function_type_list (pl_bnd_record, ptr_type_node,
			      ptr_type_node, NULL_TREE);
  pl_ret_bnd_register_fntype = build_function_type_list (pl_bnd_record,
							 NULL_TREE);

  /* Build declarations for intrinsic functions.  */
  pl_arg_bnd_fndecl = pl_make_builtin (FUNCTION_DECL, "__pl_arg_bnd",
				      pl_arg_bnd_register_fntype);
  pl_bndldx_fndecl = pl_make_builtin (FUNCTION_DECL, "__pl_bndldx",
				      pl_bndldx_register_fntype);
  pl_bndstx_fndecl = pl_make_builtin (FUNCTION_DECL, "__pl_bndstx",
				      pl_bndstx_register_fntype);
  pl_checku_fndecl = pl_make_builtin (FUNCTION_DECL, "__pl_checku",
				      pl_checku_register_fntype);
  pl_checkl_fndecl = pl_make_builtin (FUNCTION_DECL, "__pl_checkl",
				      pl_checkl_register_fntype);
  pl_mkbnd_fndecl = pl_make_builtin (FUNCTION_DECL, "__pl_bndmk",
				     pl_mkbnd_register_fntype);
  pl_ret_bnd_fndecl = pl_make_builtin (FUNCTION_DECL, "__pl_ret_bnd",
				       pl_ret_bnd_register_fntype);
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
