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

static bool pl_type_has_pointer (tree type);
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
static tree pl_force_gimple_call_op (tree op, gimple_seq *seq);
static tree pl_build_bndldx (tree addr, tree ptr, gimple_stmt_iterator *gsi);
static void pl_build_bndstx (tree addr, tree ptr, tree bounds,
			     gimple_stmt_iterator *gsi);
static tree pl_build_returned_bound (gimple call);
static void pl_copy_bounds_for_assign (tree lhs, tree rhs,
				       gimple_stmt_iterator *iter);
static tree pl_compute_bounds_for_assignment (tree node, gimple assign);
static tree pl_make_bounds (tree lb, tree size, gimple_stmt_iterator *iter);
static tree pl_make_addressed_object_bounds (tree obj,
					     gimple_stmt_iterator iter);
static tree pl_generate_extern_var_bounds (tree var);
static tree pl_get_bounds_for_decl (tree decl);
static tree pl_get_bounds_for_string_cst (tree cst);
static tree pl_get_bounds_by_definition (tree node, gimple def_stmt,
					 gimple_stmt_iterator *iter);
static tree pl_find_bounds (tree ptr, tree ptr_src, gimple_stmt_iterator *iter);
static void pl_check_mem_access (tree first, tree last, tree bounds,
				 gimple_stmt_iterator iter,
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
			     tree access_offs, tree access_size,
			     bool safe);
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

static VEC(tree,heap) *var_inits = NULL;
const char *PLSI_IDENTIFIER = "__pl_initialize_static_bounds";

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

static bool
pl_type_has_pointer (tree type)
{
  bool res = false;

  if (POINTER_TYPE_P (type))
    res = true;
  else if (RECORD_OR_UNION_TYPE_P (type))
    {
      tree field;

      for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
	if (TREE_CODE (field) == FIELD_DECL)
	  res = res || pl_type_has_pointer (TREE_TYPE (field));
    }
  else if (TREE_CODE (type) == ARRAY_TYPE)
    res = pl_type_has_pointer (TREE_TYPE (type));

  return res;
}

extern bool
pl_register_var_initializer (tree var)
{
  if (!flag_pl)
    return false;

  gcc_assert (TREE_CODE (var) == VAR_DECL);
  gcc_assert (DECL_INITIAL (var)
	      && DECL_INITIAL (var) != error_mark_node);

  if (TREE_STATIC (var)
      && pl_type_has_pointer (TREE_TYPE (var)))
    {
      VEC_safe_push (tree, heap, var_inits, var);
      return true;
    }

  return false;
}

extern void
pl_finish_file (void)
{
  tree stmts = NULL_TREE;
  int i;
  tree var;

  if (!var_inits)
    return;

  if (seen_error ())
    return;

  FOR_EACH_VEC_ELT (tree, var_inits, i, var)
    /* !!! We must check that var is actually emitted and we need
       and may initialize its bounds.  Currently addressable_flag
       is checked.  Probably rtl, section_name or other fields
       should be checked.  */
    if (TREE_ADDRESSABLE (var))
      {
	tree val = DECL_INITIAL (var);
	tree modify = build2 ( MODIFY_EXPR, TREE_TYPE (var), var, val);
	append_to_statement_list (modify, &stmts);
      }

  if (stmts)
    cgraph_build_static_cdtor ('P', stmts, MAX_RESERVED_INIT_PRIORITY-1);

  VEC_free (tree, heap, var_inits);
  var_inits = NULL;
}

static void
pl_transform_function (void)
{
  basic_block bb, next;
  gimple_stmt_iterator i;
  int saved_last_basic_block = last_basic_block;
  enum gimple_rhs_class grhs_class;
  bool safe = DECL_PL_STATIC_INIT (cfun->decl);

  bb = ENTRY_BLOCK_PTR ->next_bb;
  do
    {
      next = bb->next_bb;
      for (i = gsi_start_bb (bb); !gsi_end_p (i); )
        {
          gimple s = gsi_stmt (i);

	  if (pl_marked_stmt (s))
	    {
	      gsi_next (&i);
	      continue;
	    }

          switch (gimple_code (s))
            {
            case GIMPLE_ASSIGN:
	      pl_process_stmt (&i, gimple_assign_lhs (s),
			       gimple_location (s), integer_one_node,
			       NULL_TREE, NULL_TREE, safe);
	      pl_process_stmt (&i, gimple_assign_rhs1 (s),
			       gimple_location (s), integer_zero_node,
			       NULL_TREE, NULL_TREE, safe);
	      grhs_class = get_gimple_rhs_class (gimple_assign_rhs_code (s));
	      if (grhs_class == GIMPLE_BINARY_RHS)
		pl_process_stmt (&i, gimple_assign_rhs2 (s),
				 gimple_location (s), integer_zero_node,
				 NULL_TREE, NULL_TREE, safe);
              break;

            case GIMPLE_RETURN:
              if (gimple_return_retval (s) != NULL_TREE)
                {
                  pl_process_stmt (&i, gimple_return_retval (s),
				   gimple_location (s),
				   integer_zero_node,
				   NULL_TREE, NULL_TREE, safe);

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

	  gsi_next (&i);

	  /* We do not need any statements in static initializer except
	     created in PL pass.  */
	  if (DECL_PL_STATIC_INIT (cfun->decl)
	      && gimple_code (s) == GIMPLE_ASSIGN)
	    {
	      gimple_stmt_iterator del_iter = gsi_for_stmt (s);
	      gsi_remove (&del_iter, true);
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
      gimple_return_set_retval2 (ret, pl_find_bounds (retval, NULL_TREE, gsi));
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
	  tree bounds = pl_find_bounds (prev_arg, NULL_TREE, gsi);
	  gimple_call_set_arg (new_call, new_arg_no++, bounds);
	}
      else
	{
	  tree call_arg = gimple_call_arg (call, arg_no++);
	  gimple_call_set_arg (new_call, new_arg_no++, call_arg);

	  if (!fndecl && POINTER_TYPE_P (TREE_VALUE (arg)))
	    {
	      tree bounds = pl_find_bounds (call_arg, NULL_TREE, gsi);
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
			     pl_find_bounds (call_arg, NULL_TREE, gsi));
    }

  FOR_EACH_SSA_TREE_OPERAND (op, call, iter, SSA_OP_ALL_DEFS)
    {
      SSA_NAME_DEF_STMT (op) = new_call;
    }

  gsi_replace (gsi, new_call, true);
}

static void
pl_check_mem_access (tree first, tree last, tree bounds,
		     gimple_stmt_iterator iter,
		     location_t location, tree dirflag)
{
  gimple_seq seq;
  gimple checkl, checku;
  tree node;

  seq = gimple_seq_alloc ();

  node = pl_force_gimple_call_op (first, &seq);

  checkl = gimple_build_call (pl_checkl_fndecl, 2, bounds, node);
  pl_mark_stmt (checkl);
  gimple_seq_add_stmt (&seq, checkl);

  node = pl_force_gimple_call_op (last, &seq);

  checku = gimple_build_call (pl_checku_fndecl, 2, bounds, node);
  pl_mark_stmt (checku);
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
pl_force_gimple_call_op (tree op, gimple_seq *seq)
{
  gimple_seq stmts;
  gimple_seq_node node;

  op = force_gimple_operand (unshare_expr (op), &stmts, true, NULL_TREE);

  for (node = gimple_seq_first (stmts); node; node = node->next)
    pl_mark_stmt (node->stmt);

  gimple_seq_add_seq (seq, stmts);

  /*
  if ((is_gimple_reg_type (TREE_TYPE (op)) && !is_gimple_val (op))
      || (!is_gimple_reg_type (TREE_TYPE (op)) && !is_gimple_lvalue (op)))
    {
      tree tmp;
      gimple stmt;

      stmt = gimple_build_assign_with_ops (TREE_CODE (op), NULL_TREE,
					   op, NULL_TREE);
      tmp = create_tmp_reg (TREE_TYPE (op), NULL);
      add_referenced_var (tmp);
      tmp = make_ssa_name (tmp, stmt);
      gimple_assign_set_lhs (stmt, tmp);

      gimple_seq_add_stmt (seq, stmt);

      op = tmp;
    }
  */
  return op;
}

static tree
pl_build_bndldx (tree addr, tree ptr, gimple_stmt_iterator *gsi)
{
  gimple_seq seq;
  gimple stmt;
  tree bounds;

  seq = gimple_seq_alloc ();

  addr = pl_force_gimple_call_op (addr, &seq);
  ptr = pl_force_gimple_call_op (ptr, &seq);

  stmt = gimple_build_call (pl_bndldx_fndecl, 2, addr, ptr);
  pl_mark_stmt (stmt);
  bounds = create_tmp_reg (pl_bound_type, BOUND_TMP_NAME);
  add_referenced_var (bounds);
  bounds = make_ssa_name (bounds, stmt);
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

static void
pl_build_bndstx (tree addr, tree ptr, tree bounds,
		 gimple_stmt_iterator *gsi)
{
  gimple_seq seq;
  gimple stmt;

  seq = gimple_seq_alloc ();

  addr = pl_force_gimple_call_op (addr, &seq);
  ptr = pl_force_gimple_call_op (ptr, &seq);

  stmt = gimple_build_call (pl_bndstx_fndecl, 3, addr, ptr, bounds);
  pl_mark_stmt (stmt);

  gimple_seq_add_stmt (&seq, stmt);

  gsi_insert_seq_after (gsi, seq, GSI_CONTINUE_LINKING);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generated bndstx for pointer store ");
      print_gimple_stmt (dump_file, gsi_stmt (*gsi), 0, TDF_VOPS|TDF_MEMSYMS);
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
  tree rhs1 = gimple_assign_rhs1 (assign);
  tree rhs2 = gimple_assign_rhs2 (assign);
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
    case VAR_DECL:
    case SSA_NAME:
    case ADDR_EXPR:
    case POINTER_PLUS_EXPR:
    case NOP_EXPR:
    case CONVERT_EXPR:
    case INTEGER_CST:
      bounds = pl_find_bounds (node, rhs1, &iter);
      break;

    case MINUS_EXPR:
    case PLUS_EXPR:
    case BIT_AND_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
      {
	tree bnd1 = pl_find_bounds (rhs1, NULL_TREE, &iter);
	tree bnd2 = pl_find_bounds (rhs2, NULL_TREE, &iter);

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

	case VAR_DECL:
	  bounds = pl_get_none_bounds ();
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
  gimple_seq seq;
  gimple_stmt_iterator gsi;
  gimple stmt;
  tree bounds;

  if (iter)
    gsi = *iter;
  else
    gsi = gsi_start_bb (pl_get_entry_block ());

  seq = gimple_seq_alloc ();

  lb = pl_force_gimple_call_op (lb, &seq);
  size = pl_force_gimple_call_op (size, &seq);

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

  lb = build_fold_addr_expr (var);
  var_end = build_fold_addr_expr (end_decl);
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

  lb = build_fold_addr_expr (decl);

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

  lb = build_fold_addr_expr (cst);
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
      bounds = pl_find_bounds (TREE_OPERAND (obj, 0), NULL_TREE, &iter);
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

/* Compute bounds for pointer PTR loaded from PTR_SRC.  Generate statements
   to compute bounds if required.  Computed bounds should be available at
   position pointed by ITER.

   If PTR_SRC is NULL_TREE then pointer definition is identified.

   If PTR_SRC is not NULL_TREE then ITER points to statements which loads
   PTR.  If PTR is a any memory reference then ITER points to a statement
   after which bndldx will be inserterd.  In both cases ITER will be updated
   to point to the inserted bndldx statement.  */

static tree
pl_find_bounds (tree ptr, tree ptr_src, gimple_stmt_iterator *iter)
{
  tree addr = NULL_TREE;
  tree bounds = NULL_TREE;

  if (!ptr_src)
    ptr_src = ptr;

  switch (TREE_CODE (ptr_src))
    {
    case MEM_REF:
    case ARRAY_REF:
    case COMPONENT_REF:
    case TARGET_MEM_REF:
    case VAR_DECL:
      addr = build_fold_addr_expr (ptr_src);
      bounds = pl_build_bndldx (addr, ptr, iter);
      break;

    case SSA_NAME:
      bounds = pl_get_registered_bounds (ptr_src);
      if (!bounds)
	{
	  gimple def_stmt = SSA_NAME_DEF_STMT (ptr_src);
	  gimple_stmt_iterator phi_iter;

	  bounds = pl_get_bounds_by_definition(ptr_src, def_stmt, &phi_iter);

	  gcc_assert (bounds);

	  if (gimple_code (def_stmt) == GIMPLE_PHI)
	    {
	      /* pl_get_bounds_by_definition created new phi
		 statement and phi_iter points to it.  */
	      gimple phi_bnd = gsi_stmt (phi_iter);
	      unsigned i;

	      for (i = 0; i < gimple_phi_num_args (def_stmt); i++)
		{
		  tree arg = gimple_phi_arg_def (def_stmt, i);
		  tree arg_bnd;

		  arg_bnd = pl_find_bounds (arg, NULL_TREE, &phi_iter);

		  add_phi_arg (phi_bnd, arg_bnd,
			       gimple_phi_arg_edge (def_stmt, i),
			       UNKNOWN_LOCATION);
		}
	    }

	  gcc_assert (bounds == pl_get_registered_bounds (ptr_src));
	}
      break;

    case ADDR_EXPR:
      bounds = pl_make_addressed_object_bounds (TREE_OPERAND (ptr_src, 0), *iter);
      break;

    case INTEGER_CST:
      /* For all constants return zero bounds.  */
      bounds = pl_get_none_bounds ();
      break;

    default:
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "pl_find_bounds: unexpected ptr of type %s\n",
		   tree_code_name[TREE_CODE (ptr_src)]);
	  print_node (dump_file, "", ptr_src, 0);
	}
      internal_error ("pl_find_bounds: Unexpected tree code %s",
		      tree_code_name[TREE_CODE (ptr_src)]);
    }

  if (!bounds)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (stderr, "pl_find_bounds: cannot find bounds for pointer\n");
	  print_node (dump_file, "", ptr_src, 0);
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
      tree field_ptr = build_fold_addr_expr (node);

      *bounds = pl_make_bounds (field_ptr,
				build_int_cst (size_type_node, size),
				&iter);
    }
  else if (TREE_CODE (node) == ARRAY_REF)
    {
      tree array_addr;

      array_addr = build_fold_addr_expr (var);
      *bounds = pl_find_bounds (array_addr, NULL_TREE, &iter);

      precise_bounds = true;
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

	  array_addr = build_fold_addr_expr (var);

	  if (!*bounds || !precise_bounds)
	    {
	      *bounds = pl_intersect_bounds (pl_find_bounds (array_addr, NULL_TREE, &iter),
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
	      *bounds = pl_intersect_bounds (pl_find_bounds (*ptr, NULL_TREE, &iter),
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

	  *ptr = build_fold_addr_expr (var);
	  break;
	}
    }
}

static void
pl_copy_bounds_for_assign (tree lhs, tree rhs, gimple_stmt_iterator *iter)
{
  tree type = TREE_TYPE (rhs);

  /* We have nothing to do with clobbers.  */
  if (TREE_CLOBBER_P (rhs))
    return;

  if (POINTER_TYPE_P (type))
    {
      tree bounds = pl_find_bounds (rhs, NULL_TREE, iter);
      tree addr = fold_build1 (ADDR_EXPR, build_pointer_type (type), lhs);
      pl_build_bndstx (addr, rhs, bounds, iter);
    }
  else if (RECORD_OR_UNION_TYPE_P (type))
    {
      tree field;

      if (TREE_CODE (rhs) == CONSTRUCTOR)
	{
	  unsigned HOST_WIDE_INT cnt;
	  tree val;

	  FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (rhs), cnt, field, val)
	    {
	      if (pl_type_has_pointer (TREE_TYPE (field)))
		{
		  tree lhs_field = build3 (COMPONENT_REF, TREE_TYPE (field),
					   lhs, field, NULL_TREE);
		  pl_copy_bounds_for_assign (lhs_field, val, iter);
		}
	    }
	}
      else
	for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
	  if (TREE_CODE (field) == FIELD_DECL
	      && pl_type_has_pointer (TREE_TYPE (field)))
	    {
	      tree lhs_field = build3 (COMPONENT_REF, TREE_TYPE (field),
				       lhs, field, NULL_TREE);
	      tree rhs_field = build3 (COMPONENT_REF, TREE_TYPE (field),
				       rhs, field, NULL_TREE);
	      pl_copy_bounds_for_assign (lhs_field, rhs_field, iter);
	    }
    }
  else if (TREE_CODE (type) == ARRAY_TYPE)
    {
      unsigned HOST_WIDE_INT cur = -1;
      tree maxval = TYPE_MAX_VALUE (TYPE_DOMAIN (type));

      gcc_assert (maxval && TREE_CODE (maxval) == INTEGER_CST);

      if (TREE_CODE (rhs) == CONSTRUCTOR)
	{
	  unsigned HOST_WIDE_INT cnt;
	  tree purp, val, idx, lhs_elem;

	  FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (rhs), cnt, purp, val)
	    {
	      if (TREE_CODE (purp) == RANGE_EXPR)
		{
		  tree lo_index = TREE_OPERAND (purp, 0);
		  tree hi_index = TREE_OPERAND (purp, 1);

		  for (cur = tree_low_cst (lo_index, 1);
		       cur <= tree_low_cst (hi_index, 1);
		       cur++)
		    {
		      idx = build_int_cst (size_type_node, cur);
		      lhs_elem = build4 (ARRAY_REF, TREE_TYPE (type),
					      lhs, idx, NULL_TREE, NULL_TREE);
		      pl_copy_bounds_for_assign (lhs_elem, val, iter);
		    }
		}
	      else
		{
		  if (purp == NULL_TREE)
		    cur++;

		  idx = build_int_cst (size_type_node, cur);
		  lhs_elem = build4 (ARRAY_REF, TREE_TYPE (type),
				     lhs, idx, NULL_TREE, NULL_TREE);
		  pl_copy_bounds_for_assign (lhs_elem, val, iter);
		}
	    }
	}
      else
	for (cur = 0; cur <= TREE_INT_CST_LOW (maxval); cur++)
	  {
	    tree index = build_int_cst (size_type_node, cur);
	    tree lhs_elem = build4 (ARRAY_REF, TREE_TYPE (type),
				    lhs, index, NULL_TREE, NULL_TREE);
	    tree rhs_elem = build4 (ARRAY_REF, TREE_TYPE (type),
				    rhs, index, NULL_TREE, NULL_TREE);
	    pl_copy_bounds_for_assign (lhs_elem, rhs_elem, iter);
	  }
    }
  else
    internal_error("pl_copy_bounds_for_assign: unexpected RHS type: %s",
		   tree_code_name[TREE_CODE (type)]);
}

static void
pl_process_stmt (gimple_stmt_iterator *iter, tree node,
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

  // TODO: check we need to instrument this node

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
	    addr_first = build_fold_addr_expr (node);
	    break;
	  }

	pl_parse_array_and_component_ref (node, &ptr, &elt, &safe,
					  &bitfield, &bounds, *iter, false);

	/* Break if there is no dereference and operation is safe.  */
	if (safe)
	  {
	    /* We may still need addr_first for bndstx in case
	       write a pointer into memory.  */
	    addr_first = build_fold_addr_expr (node);
	    break;
	  }

	if (bitfield)
          {
            tree field = TREE_OPERAND (node, 1);

            if (TREE_CODE (DECL_SIZE_UNIT (field)) == INTEGER_CST)
              size = DECL_SIZE_UNIT (field);

	    if (elt)
	      elt = build_fold_addr_expr (elt);
            addr_first = fold_convert_loc (loc, ptr_type_node, elt ? elt : ptr);
            addr_first = fold_build_pointer_plus_loc (loc,
						      addr_first,
						      byte_position (field));
          }
        else
          addr_first = build_fold_addr_expr (node);
      }
      break;

    case INDIRECT_REF:
      ptr = TREE_OPERAND (node, 0);
      addr_first = ptr;
      break;

    case MEM_REF:
      ptr = TREE_OPERAND (node, 0);
      addr_first = build_fold_addr_expr (node);
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
			 dirflag, offs, size, safe);
	return;
      }
      break;

    case VAR_DECL:
      if (dirflag != integer_one_node)
	return;

      safe = true;
      addr_first = build_fold_addr_expr (node);
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
	bounds = pl_find_bounds (ptr, NULL_TREE, iter);

      pl_check_mem_access (addr_first, addr_last, bounds,
			   stmt_iter, loc, dirflag);
    }

  /* We need to generate bndstx in case pointer is stored.  */
  if (dirflag == integer_one_node && pl_type_has_pointer (node_type))
    {
      gimple stmt = gsi_stmt (*iter);
      tree rhs1 = gimple_assign_rhs1 (stmt);
      enum tree_code rhs_code = gimple_assign_rhs_code (stmt);

      if (get_gimple_rhs_class (rhs_code) == GIMPLE_SINGLE_RHS)
	pl_copy_bounds_for_assign (node, rhs1, iter);
      else
	{
	  bounds = pl_compute_bounds_for_assignment (NULL_TREE, stmt);
	  pl_build_bndstx (addr_first, rhs1, bounds, iter);
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
