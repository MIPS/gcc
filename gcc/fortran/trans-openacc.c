/*
 */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "gimple.h"     /* For create_tmp_var_raw.  */
#include "diagnostic-core.h"    /* For internal_error.  */
#include "gfortran.h"
#include "trans.h"
#include "trans-stmt.h"
#include "trans-types.h"
#include "trans-array.h"
#include "trans-const.h"
#include "arith.h"

static tree
trans_acc_variable (gfc_symbol *sym)
{
  tree t = gfc_get_symbol_decl (sym);
  tree parent_decl;
  int parent_flag;
  bool return_value;
  bool alternate_entry;
  bool entry_master;

  return_value = sym->attr.function && sym->result == sym;
  alternate_entry = sym->attr.function && sym->attr.entry
                    && sym->result == sym;
  entry_master = sym->attr.result
                 && sym->ns->proc_name->attr.entry_master
                 && !gfc_return_by_reference (sym->ns->proc_name);
  parent_decl = DECL_CONTEXT (current_function_decl);

  if ((t == parent_decl && return_value)
       || (sym->ns && sym->ns->proc_name
           && sym->ns->proc_name->backend_decl == parent_decl
           && (alternate_entry || entry_master)))
    parent_flag = 1;
  else
    parent_flag = 0;

  /* Special case for assigning the return value of a function.
     Self recursive functions must have an explicit return value.  */
  if (return_value && (t == current_function_decl || parent_flag))
    t = gfc_get_fake_result_decl (sym, parent_flag);

  /* Similarly for alternate entry points.  */
  else if (alternate_entry
           && (sym->ns->proc_name->backend_decl == current_function_decl
               || parent_flag))
    {
      gfc_entry_list *el = NULL;

      for (el = sym->ns->entries; el; el = el->next)
        if (sym == el->sym)
          {
            t = gfc_get_fake_result_decl (sym, parent_flag);
            break;
          }
    }

  else if (entry_master
           && (sym->ns->proc_name->backend_decl == current_function_decl
               || parent_flag))
    t = gfc_get_fake_result_decl (sym, parent_flag);

  return t;
}

static inline tree
trans_add_clause (tree node, tree tail)
{
  ACC_CLAUSE_CHAIN (node) = tail;
  return node;
}

static void
trans_acc_array_reduction (tree c, gfc_symbol *sym, locus where)
{
  gfc_symtree *root1 = NULL, *root2 = NULL, *root3 = NULL, *root4 = NULL;
  gfc_symtree *symtree1, *symtree2, *symtree3, *symtree4 = NULL;
  gfc_symbol init_val_sym, outer_sym, intrinsic_sym;
  gfc_expr *e1, *e2, *e3, *e4;
  gfc_ref *ref;
  tree decl, backend_decl, stmt, type, outer_decl;
  locus old_loc = gfc_current_locus;
  const char *iname;
  bool t;

  decl = ACC_CLAUSE_DECL (c);
  gfc_current_locus = where;
  type = TREE_TYPE (decl);
  outer_decl = create_tmp_var_raw (type, NULL);
  if (TREE_CODE (decl) == PARM_DECL
      && TREE_CODE (type) == REFERENCE_TYPE
      && GFC_DESCRIPTOR_TYPE_P (TREE_TYPE (type))
      && GFC_TYPE_ARRAY_AKIND (TREE_TYPE (type)) == GFC_ARRAY_ALLOCATABLE)
    {
      decl = build_fold_indirect_ref (decl);
      type = TREE_TYPE (type);
    }

  /* Create a fake symbol for init value.  */
  memset (&init_val_sym, 0, sizeof (init_val_sym));
  init_val_sym.ns = sym->ns;
  init_val_sym.name = sym->name;
  init_val_sym.ts = sym->ts;
  init_val_sym.attr.referenced = 1;
  init_val_sym.declared_at = where;
  init_val_sym.attr.flavor = FL_VARIABLE;
  backend_decl = acc_reduction_init (c, gfc_sym_type (&init_val_sym));
  init_val_sym.backend_decl = backend_decl;

  /* Create a fake symbol for the outer array reference.  */
  outer_sym = *sym;
  outer_sym.as = gfc_copy_array_spec (sym->as);
  outer_sym.attr.dummy = 0;
  outer_sym.attr.result = 0;
  outer_sym.attr.flavor = FL_VARIABLE;
  outer_sym.backend_decl = outer_decl;
  if (decl != ACC_CLAUSE_DECL (c))
    outer_sym.backend_decl = build_fold_indirect_ref (outer_decl);

  /* Create fake symtrees for it.  */
  symtree1 = gfc_new_symtree (&root1, sym->name);
  symtree1->n.sym = sym;
  gcc_assert (symtree1 == root1);

  symtree2 = gfc_new_symtree (&root2, sym->name);
  symtree2->n.sym = &init_val_sym;
  gcc_assert (symtree2 == root2);

  symtree3 = gfc_new_symtree (&root3, sym->name);
  symtree3->n.sym = &outer_sym;
  gcc_assert (symtree3 == root3);

  /* Create expressions.  */
  e1 = gfc_get_expr ();
  e1->expr_type = EXPR_VARIABLE;
  e1->where = where;
  e1->symtree = symtree1;
  e1->ts = sym->ts;
  e1->ref = ref = gfc_get_ref ();
  ref->type = REF_ARRAY;
  ref->u.ar.where = where;
  ref->u.ar.as = sym->as;
  ref->u.ar.type = AR_FULL;
  ref->u.ar.dimen = 0;
  t = gfc_resolve_expr (e1);
  gcc_assert (t);

  e2 = gfc_get_expr ();
  e2->expr_type = EXPR_VARIABLE;
  e2->where = where;
  e2->symtree = symtree2;
  e2->ts = sym->ts;
  t = gfc_resolve_expr (e2);
  gcc_assert (t);

  e3 = gfc_copy_expr (e1);
  e3->symtree = symtree3;
  t = gfc_resolve_expr (e3);
  gcc_assert (t);

  iname = NULL;
  switch (ACC_CLAUSE_REDUCTION_CODE (c))
    {
    case PLUS_EXPR:
    case MINUS_EXPR:
      e4 = gfc_add (e3, e1);
      break;
    case MULT_EXPR:
      e4 = gfc_multiply (e3, e1);
      break;
    case TRUTH_ANDIF_EXPR:
      e4 = gfc_and (e3, e1);
      break;
    case TRUTH_ORIF_EXPR:
      e4 = gfc_or (e3, e1);
      break;
    case EQ_EXPR:
      e4 = gfc_eqv (e3, e1);
      break;
    case NE_EXPR:
      e4 = gfc_neqv (e3, e1);
      break;
    case MIN_EXPR:
      iname = "min";
      break;
    case MAX_EXPR:
      iname = "max";
      break;
    case BIT_AND_EXPR:
      iname = "iand";
      break;
    case BIT_IOR_EXPR:
      iname = "ior";
      break;
    case BIT_XOR_EXPR:
      iname = "ieor";
      break;
    default:
      gcc_unreachable ();
    }
  if (iname != NULL)
    {
      memset (&intrinsic_sym, 0, sizeof (intrinsic_sym));
      intrinsic_sym.ns = sym->ns;
      intrinsic_sym.name = iname;
      intrinsic_sym.ts = sym->ts;
      intrinsic_sym.attr.referenced = 1;
      intrinsic_sym.attr.intrinsic = 1;
      intrinsic_sym.attr.function = 1;
      intrinsic_sym.result = &intrinsic_sym;
      intrinsic_sym.declared_at = where;

      symtree4 = gfc_new_symtree (&root4, iname);
      symtree4->n.sym = &intrinsic_sym;
      gcc_assert (symtree4 == root4);

      e4 = gfc_get_expr ();
      e4->expr_type = EXPR_FUNCTION;
      e4->where = where;
      e4->symtree = symtree4;
      e4->value.function.isym = gfc_find_function (iname);
      e4->value.function.actual = gfc_get_actual_arglist ();
      e4->value.function.actual->expr = e3;
      e4->value.function.actual->next = gfc_get_actual_arglist ();
      e4->value.function.actual->next->expr = e1;
    }
  /* e1 and e3 have been stored as arguments of e4, avoid sharing.  */
  e1 = gfc_copy_expr (e1);
  e3 = gfc_copy_expr (e3);
  t = gfc_resolve_expr (e4);
  gcc_assert (t);

  /* Create the init statement list.  */
  pushlevel ();
  if (GFC_DESCRIPTOR_TYPE_P (type)
      && GFC_TYPE_ARRAY_AKIND (type) == GFC_ARRAY_ALLOCATABLE)
    {
      /* If decl is an allocatable array, it needs to be allocated
         with the same bounds as the outer var.  */
      tree rank, size, esize, ptr;
      stmtblock_t block;

      gfc_start_block (&block);

      gfc_add_modify (&block, decl, outer_sym.backend_decl);
      rank = gfc_rank_cst[GFC_TYPE_ARRAY_RANK (type) - 1];
      size = gfc_conv_descriptor_ubound_get (decl, rank);
      size = fold_build2_loc (input_location, MINUS_EXPR,
                              gfc_array_index_type, size,
                              gfc_conv_descriptor_lbound_get (decl, rank));
      size = fold_build2_loc (input_location, PLUS_EXPR, gfc_array_index_type,
                              size, gfc_index_one_node);
      if (GFC_TYPE_ARRAY_RANK (type) > 1)
        size = fold_build2_loc (input_location, MULT_EXPR,
                                gfc_array_index_type, size,
                                gfc_conv_descriptor_stride_get (decl, rank));
      esize = fold_convert (gfc_array_index_type,
                            TYPE_SIZE_UNIT (gfc_get_element_type (type)));
      size = fold_build2_loc (input_location, MULT_EXPR, gfc_array_index_type,
                              size, esize);
      size = gfc_evaluate_now (fold_convert (size_type_node, size), &block);

      ptr = gfc_create_var (pvoid_type_node, NULL);
      gfc_allocate_using_malloc (&block, ptr, size, NULL_TREE);
      gfc_conv_descriptor_data_set (&block, decl, ptr);

      gfc_add_expr_to_block (&block, gfc_trans_assignment (e1, e2, false,
                             false));
      stmt = gfc_finish_block (&block);
    }
  else
    stmt = gfc_trans_assignment (e1, e2, false, false);
  if (TREE_CODE (stmt) != BIND_EXPR)
    stmt = build3_v (BIND_EXPR, NULL, stmt, poplevel (1, 0));
  else
    poplevel (0, 0);
  ACC_CLAUSE_REDUCTION_INIT (c) = stmt;

  /* Create the merge statement list.  */
  pushlevel ();
  if (GFC_DESCRIPTOR_TYPE_P (type)
      && GFC_TYPE_ARRAY_AKIND (type) == GFC_ARRAY_ALLOCATABLE)
    {
      /* If decl is an allocatable array, it needs to be deallocated
         afterwards.  */
      stmtblock_t block;

      gfc_start_block (&block);
      gfc_add_expr_to_block (&block, gfc_trans_assignment (e3, e4, false,
                             true));
      gfc_add_expr_to_block (&block, gfc_trans_dealloc_allocated (decl, false, NULL));
      stmt = gfc_finish_block (&block);
    }
  else
    stmt = gfc_trans_assignment (e3, e4, false, true);
  if (TREE_CODE (stmt) != BIND_EXPR)
    stmt = build3_v (BIND_EXPR, NULL, stmt, poplevel (1, 0));
  else
    poplevel (0, 0);
  ACC_CLAUSE_REDUCTION_MERGE (c) = stmt;

  /* And stick the placeholder VAR_DECL into the clause as well.  */
  ACC_CLAUSE_REDUCTION_PLACEHOLDER (c) = outer_decl;

  gfc_current_locus = old_loc;

  gfc_free_expr (e1);
  gfc_free_expr (e2);
  gfc_free_expr (e3);
  gfc_free_expr (e4);
  free (symtree1);
  free (symtree2);
  free (symtree3);
  free (symtree4);
  gfc_free_array_spec (outer_sym.as);
}

static tree
convert_expr_to_tree(stmtblock_t *block, gfc_expr *expr)
{
  gfc_se se;
  tree result;

  gfc_init_se (&se, NULL );
  gfc_conv_expr (&se, expr);
  gfc_add_block_to_block (block, &se.pre);
  result = gfc_evaluate_now (se.expr, block);
  gfc_add_block_to_block (block, &se.post);

  return result;
}

static tree
trans_acc_variable_list (stmtblock_t *block, enum acc_clause_code code, gfc_namelist *namelist,
                             tree list)
{
  for (; namelist != NULL; namelist = namelist->next)
    if (namelist->sym->attr.referenced)
      {
        if (!namelist->acc_subarray)
          {
            tree t = trans_acc_variable (namelist->sym);
            if (t != error_mark_node)
              {
                tree node = build_acc_clause (input_location, code);
                ACC_CLAUSE_DECL (node) = t;
                ACC_IS_SUBARRAY (node) = false;
                list = trans_add_clause (node, list);
              }
          }
        else
          {
            tree arr_tree = trans_acc_variable (namelist->sym);
            if (arr_tree != error_mark_node)
              {
                int i;
                tree node;

                node = build_acc_clause (input_location, code);
                ACC_CLAUSE_DECL (node) = arr_tree;
                ACC_IS_SUBARRAY (node) = 1;
                ACC_SUBARRAY_DIMENSIONS(node) = namelist->acc_subarray->dimensions;

                for (i = 0; i < namelist->acc_subarray->dimensions; i++)
                  {
                    gfc_expr *left_expr, *right_expr;

                    left_expr = gfc_subtract (namelist->acc_subarray->left[i],
                                             gfc_copy_expr (namelist->sym->as->lower[i]));
                    ACC_SUBARRAY_LEFT_BOUND (node, i) = convert_expr_to_tree (block, left_expr);

                    right_expr = gfc_subtract (namelist->acc_subarray->right[i],
                                              gfc_copy_expr (namelist->sym->as->lower[i]));
                    ACC_SUBARRAY_RIGHT_BOUND (node, i) = convert_expr_to_tree (block, right_expr);;

                    gfc_free_expr (left_expr);
                    gfc_free_expr (right_expr);
                  }

                list = trans_add_clause (node, list);
              }
          }
      }
  return list;
}

static tree
trans_acc_reduction_list (gfc_namelist *namelist, tree list,
                              enum tree_code reduction_code, locus where)
{
  for (; namelist != NULL; namelist = namelist->next)
    if (namelist->sym->attr.referenced)
      {
        tree t = trans_acc_variable (namelist->sym);
        if (t != error_mark_node)
          {
            tree node = build_acc_clause (where.lb->location,
                                          ACC_CLAUSE_REDUCTION);
            ACC_CLAUSE_DECL (node) = t;
            ACC_CLAUSE_REDUCTION_CODE (node) = reduction_code;
            if (namelist->sym->attr.dimension)
              trans_acc_array_reduction (node, namelist->sym, where);
            list = trans_add_clause (node, list);
          }
      }
  return list;
}

static tree
trans_acc_clauses (stmtblock_t *block, gfc_acc_clauses *clauses,
                       locus where)
{
  tree acc_clauses = NULL_TREE, c;
  int list;
  enum acc_clause_code clause_code;
  gfc_se se;

  if (clauses == NULL)
    return NULL_TREE;

  for (list = 0; list < ACC_LIST_NUM; list++)
    {
      gfc_namelist *n = clauses->lists[list];

      if (n == NULL)
        continue;
      if (list >= ACC_LIST_REDUCTION_FIRST
          && list <= ACC_LIST_REDUCTION_LAST)
        {
          enum tree_code reduction_code;
          switch (list)
            {
            case OMP_LIST_PLUS:
              reduction_code = PLUS_EXPR;
              break;
            case OMP_LIST_MULT:
              reduction_code = MULT_EXPR;
              break;
            case OMP_LIST_SUB:
              reduction_code = MINUS_EXPR;
              break;
            case OMP_LIST_AND:
              reduction_code = TRUTH_ANDIF_EXPR;
              break;
            case OMP_LIST_OR:
              reduction_code = TRUTH_ORIF_EXPR;
              break;
            case OMP_LIST_EQV:
              reduction_code = EQ_EXPR;
              break;
            case OMP_LIST_NEQV:
              reduction_code = NE_EXPR;
              break;
            case OMP_LIST_MAX:
              reduction_code = MAX_EXPR;
              break;
            case OMP_LIST_MIN:
              reduction_code = MIN_EXPR;
              break;
            case OMP_LIST_IAND:
              reduction_code = BIT_AND_EXPR;
              break;
            case OMP_LIST_IOR:
              reduction_code = BIT_IOR_EXPR;
              break;
            case OMP_LIST_IEOR:
              reduction_code = BIT_XOR_EXPR;
              break;
            default:
              gcc_unreachable ();
            }
          acc_clauses
            = trans_acc_reduction_list (n, acc_clauses, reduction_code,
                                            where);
          continue;
        }
      switch (list)
        {
        case ACC_LIST_COPY:
          clause_code = ACC_CLAUSE_COPY;
          goto add_clause;
        case ACC_LIST_COPYIN:
          clause_code = ACC_CLAUSE_COPYIN;
          goto add_clause;
        case ACC_LIST_COPYOUT:
          clause_code = ACC_CLAUSE_COPYOUT;
          goto add_clause;
        case ACC_LIST_CREATE:
          clause_code = ACC_CLAUSE_CREATE;
          goto add_clause;
        case ACC_LIST_PRESENT:
          clause_code = ACC_CLAUSE_PRESENT;
          goto add_clause;
        case ACC_LIST_PRESENT_OR_COPY:
          clause_code = ACC_CLAUSE_PRESENT_OR_COPY;
          goto add_clause;
        case ACC_LIST_PRESENT_OR_COPYIN:
          clause_code = ACC_CLAUSE_PRESENT_OR_COPYIN;
          goto add_clause;
        case ACC_LIST_PRESENT_OR_COPYOUT:
          clause_code = ACC_CLAUSE_PRESENT_OR_COPYOUT;
          goto add_clause;
        case ACC_LIST_PRESENT_OR_CREATE:
          clause_code = ACC_CLAUSE_PRESENT_OR_CREATE;
          goto add_clause;
        case ACC_LIST_USE_DEVICE:
          clause_code = ACC_CLAUSE_USE_DEVICE;
          goto add_clause;
        case ACC_LIST_DEVICEPTR:
          clause_code = ACC_CLAUSE_DEVICEPTR;
          goto add_clause;
        case ACC_LIST_PRIVATE:
          clause_code = ACC_CLAUSE_PRIVATE;
          goto add_clause;
        case ACC_LIST_FIRSTPRIVATE:
          clause_code = ACC_CLAUSE_FIRSTPRIVATE;
          goto add_clause;
        case ACC_LIST_DEVICE_RESIDENT:
          clause_code = ACC_CLAUSE_DEVICE_RESIDENT;
          goto add_clause;
        case ACC_LIST_HOST:
          clause_code = ACC_CLAUSE_HOST;
          goto add_clause;
        case ACC_LIST_DEVICE:
          clause_code = ACC_CLAUSE_DEVICE;
          goto add_clause;
        case ACC_LIST_CACHE:
          clause_code = ACC_NO_CLAUSE_CACHE;
          /* FALLTHROUGH */
        add_clause:
          acc_clauses
              = trans_acc_variable_list (block, clause_code, n, acc_clauses);
          break;
        default:
          break;
        }
    }

  if (clauses->if_expr)
    {
      tree if_var = convert_expr_to_tree(block, clauses->if_expr);
      c = build_acc_clause (where.lb->location, ACC_CLAUSE_IF);
      ACC_CLAUSE_IF_EXPR (c) = if_var;
      acc_clauses = trans_add_clause (c, acc_clauses);
    }
  if (clauses->async)
    {
      c = build_acc_clause (where.lb->location, ACC_CLAUSE_ASYNC);
      ACC_CLAUSE_ASYNC_EXPR (c) =
          convert_expr_to_tree (block, clauses->async_expr);
      acc_clauses = trans_add_clause (c, acc_clauses);
    }
  if (clauses->seq)
    {
      c = build_acc_clause (where.lb->location, ACC_CLAUSE_SEQ);
      acc_clauses = trans_add_clause (c, acc_clauses);
    }
  if (clauses->independent)
    {
      c = build_acc_clause (where.lb->location, ACC_CLAUSE_INDEPENDENT);
      acc_clauses = trans_add_clause (c, acc_clauses);
    }
  if (clauses->num_gangs_expr)
    {
      tree num_gangs_var = convert_expr_to_tree(block, clauses->num_gangs_expr);
      c = build_acc_clause (where.lb->location, ACC_CLAUSE_NUM_GANGS);
      ACC_CLAUSE_NUM_GANGS_EXPR (c) = num_gangs_var;
      acc_clauses = trans_add_clause (c, acc_clauses);
    }
  if (clauses->num_workers_expr)
    {
      tree num_workers_var = convert_expr_to_tree(block, clauses->num_workers_expr);
      c = build_acc_clause (where.lb->location, ACC_CLAUSE_NUM_WORKERS);
      ACC_CLAUSE_NUM_WORKERS_EXPR (c)= num_workers_var;
      acc_clauses = trans_add_clause (c, acc_clauses);
    }
  if (clauses->vector_length_expr)
    {
      tree vector_length_var = convert_expr_to_tree(block, clauses->vector_length_expr);
      c = build_acc_clause (where.lb->location, ACC_CLAUSE_VECTOR_LENGTH);
      ACC_CLAUSE_VECTOR_LENGTH_EXPR (c)= vector_length_var;
      acc_clauses = trans_add_clause (c, acc_clauses);
    }
  if (clauses->vector)
    {
      if (clauses->vector_expr)
        {
          tree vector_var = convert_expr_to_tree(block, clauses->vector_expr);
          c = build_acc_clause (where.lb->location, ACC_CLAUSE_VECTOR);
          ACC_CLAUSE_VECTOR_EXPR (c)= vector_var;
          acc_clauses = trans_add_clause (c, acc_clauses);
        }
      else
        {
          c = build_acc_clause (where.lb->location, ACC_CLAUSE_VECTOR);
          acc_clauses = trans_add_clause (c, acc_clauses);
        }
    }
  if (clauses->worker)
    {
      if (clauses->worker_expr)
        {
          tree worker_var = convert_expr_to_tree(block, clauses->worker_expr);
          c = build_acc_clause (where.lb->location, ACC_CLAUSE_WORKER);
          ACC_CLAUSE_WORKER_EXPR (c)= worker_var;
          acc_clauses = trans_add_clause (c, acc_clauses);
        }
      else
        {
          c = build_acc_clause (where.lb->location, ACC_CLAUSE_WORKER);
          acc_clauses = trans_add_clause (c, acc_clauses);
        }
    }
  if (clauses->gang)
    {
      if (clauses->gang_expr)
        {
          tree gang_var = convert_expr_to_tree(block, clauses->gang_expr);
          c = build_acc_clause (where.lb->location, ACC_CLAUSE_GANG);
          ACC_CLAUSE_GANG_EXPR (c)= gang_var;
          acc_clauses = trans_add_clause (c, acc_clauses);
        }
      else
        {
          c = build_acc_clause (where.lb->location, ACC_CLAUSE_GANG);
          acc_clauses = trans_add_clause (c, acc_clauses);
        }
    }
  if (clauses->non_clause_wait_expr)
    {
      tree wait_var = convert_expr_to_tree(block, clauses->non_clause_wait_expr);
      c = build_acc_clause (where.lb->location, ACC_NO_CLAUSE_WAIT);
      ACC_WAIT_EXPR (c)= wait_var;
      acc_clauses = trans_add_clause (c, acc_clauses);
    }
  if (clauses->collapse)
    {
      c = build_acc_clause (where.lb->location, ACC_CLAUSE_COLLAPSE);
      ACC_CLAUSE_COLLAPSE_EXPR (c)
        = build_int_cst (integer_type_node, clauses->collapse);
      acc_clauses = trans_add_clause (c, acc_clauses);
    }

  return acc_clauses;
}

static tree
trans_acc_code (gfc_code *code, bool force_empty)
{
  tree stmt;

  pushlevel ();
  stmt = gfc_trans_code (code);
  if (TREE_CODE (stmt) != BIND_EXPR)
    {
      if (!IS_EMPTY_STMT (stmt) || force_empty)
        {
          tree block = poplevel (1, 0);
          stmt = build3_v (BIND_EXPR, NULL, stmt, block);
        }
      else
        poplevel (0, 0);
    }
  else
    poplevel (0, 0);
  return stmt;
}

//TODO: split into single function
static tree
trans_acc_parallel (gfc_code *code)
{
  stmtblock_t block;
  tree stmt, acc_clauses;

  gfc_start_block (&block);
  acc_clauses = trans_acc_clauses (&block, code->ext.omp_clauses,
                                       code->loc);
  stmt = trans_acc_code (code->block->next, true);
  stmt = build2_loc (input_location, ACC_PARALLEL, void_type_node, stmt,
                     acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
trans_acc_kernels (gfc_code *code)
{
  stmtblock_t block;
  tree stmt, acc_clauses;

  gfc_start_block (&block);
  acc_clauses = trans_acc_clauses (&block, code->ext.omp_clauses,
                                       code->loc);
  stmt = trans_acc_code (code->block->next, true);
  stmt = build2_loc (input_location, ACC_KERNELS, void_type_node, stmt,
                     acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
trans_acc_data (gfc_code *code)
{
  stmtblock_t block;
  tree stmt, acc_clauses;

  gfc_start_block (&block);
  acc_clauses = trans_acc_clauses (&block, code->ext.omp_clauses,
                                       code->loc);
  stmt = trans_acc_code (code->block->next, true);
  stmt = build2_loc (input_location, ACC_DATA, void_type_node, stmt,
                     acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
trans_acc_host_data (gfc_code *code)
{
  stmtblock_t block;
  tree stmt, acc_clauses;

  gfc_start_block (&block);
  acc_clauses = trans_acc_clauses (&block, code->ext.omp_clauses,
                                       code->loc);
  stmt = trans_acc_code (code->block->next, true);
  stmt = build2_loc (input_location, ACC_HOST_DATA, void_type_node, stmt,
                     acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

typedef struct dovar_init_d {
  tree var;
  tree init;
} dovar_init;

static tree
trans_acc_loop (gfc_code *code, stmtblock_t *pblock,
                    gfc_acc_clauses *do_clauses)
{
  gfc_se se;
  tree dovar, stmt, from, to, step, type, init, cond, incr;
  tree count = NULL_TREE, cycle_label, tmp, acc_clauses;
  stmtblock_t block;
  stmtblock_t body;
  gfc_acc_clauses *clauses = code->ext.omp_clauses;
  int i, collapse = clauses->collapse;
  vec<dovar_init> inits = vNULL;
  dovar_init *di;
  unsigned ix;

  if (collapse <= 0)
    collapse = 1;

  code = code->block->next;
  gcc_assert (code->op == EXEC_DO);

  init = make_tree_vec (collapse);
  cond = make_tree_vec (collapse);
  incr = make_tree_vec (collapse);

  if (pblock == NULL)
    {
      gfc_start_block (&block);
      pblock = &block;
    }

  acc_clauses = trans_acc_clauses (pblock, do_clauses, code->loc);

  for (i = 0; i < collapse; i++)
    {
      int simple = 0;
      int dovar_found = 0;
      tree dovar_decl;

      /* Evaluate all the expressions in the iterator.  */
      gfc_init_se (&se, NULL);
      gfc_conv_expr_lhs (&se, code->ext.iterator->var);
      gfc_add_block_to_block (pblock, &se.pre);
      dovar = se.expr;
      type = TREE_TYPE (dovar);
      gcc_assert (TREE_CODE (type) == INTEGER_TYPE);

      gfc_init_se (&se, NULL);
      gfc_conv_expr_val (&se, code->ext.iterator->start);
      gfc_add_block_to_block (pblock, &se.pre);
      from = gfc_evaluate_now (se.expr, pblock);

      gfc_init_se (&se, NULL);
      gfc_conv_expr_val (&se, code->ext.iterator->end);
      gfc_add_block_to_block (pblock, &se.pre);
      to = gfc_evaluate_now (se.expr, pblock);

      gfc_init_se (&se, NULL);
      gfc_conv_expr_val (&se, code->ext.iterator->step);
      gfc_add_block_to_block (pblock, &se.pre);
      step = gfc_evaluate_now (se.expr, pblock);
      dovar_decl = dovar;

      /* Special case simple loops.  */
      if (TREE_CODE (dovar) == VAR_DECL)
        {
          if (integer_onep (step))
            simple = 1;
          else if (tree_int_cst_equal (step, integer_minus_one_node))
            simple = -1;
        }
      else
        dovar_decl
          = trans_acc_variable (code->ext.iterator->var->symtree->n.sym);

      /* Loop body.  */
      if (simple)
        {
          TREE_VEC_ELT (init, i) = build2_v (MODIFY_EXPR, dovar, from);
          /* The condition should not be folded.  */
          TREE_VEC_ELT (cond, i) = build2_loc (input_location, simple > 0
                                               ? LE_EXPR : GE_EXPR,
                                               boolean_type_node, dovar, to);
          TREE_VEC_ELT (incr, i) = fold_build2_loc (input_location, PLUS_EXPR,
                                                    type, dovar, step);
          TREE_VEC_ELT (incr, i) = fold_build2_loc (input_location,
                                                    MODIFY_EXPR,
                                                    type, dovar,
                                                    TREE_VEC_ELT (incr, i));
        }
      else
        {
          /* STEP is not 1 or -1.  Use:
             for (count = 0; count < (to + step - from) / step; count++)
               {
                 dovar = from + count * step;
                 body;
               cycle_label:;
               }  */
          tmp = fold_build2_loc (input_location, MINUS_EXPR, type, step, from);
          tmp = fold_build2_loc (input_location, PLUS_EXPR, type, to, tmp);
          tmp = fold_build2_loc (input_location, TRUNC_DIV_EXPR, type, tmp,
                                 step);
          tmp = gfc_evaluate_now (tmp, pblock);
          count = gfc_create_var (type, "count");
          TREE_VEC_ELT (init, i) = build2_v (MODIFY_EXPR, count,
                                             build_int_cst (type, 0));
          /* The condition should not be folded.  */
          TREE_VEC_ELT (cond, i) = build2_loc (input_location, LT_EXPR,
                                               boolean_type_node,
                                               count, tmp);
          TREE_VEC_ELT (incr, i) = fold_build2_loc (input_location, PLUS_EXPR,
                                                    type, count,
                                                    build_int_cst (type, 1));
          TREE_VEC_ELT (incr, i) = fold_build2_loc (input_location,
                                                    MODIFY_EXPR, type, count,
                                                    TREE_VEC_ELT (incr, i));

          /* Initialize DOVAR.  */
          tmp = fold_build2_loc (input_location, MULT_EXPR, type, count, step);
          tmp = fold_build2_loc (input_location, PLUS_EXPR, type, from, tmp);
          dovar_init e = {dovar, tmp};
          inits.safe_push (e);
        }
      gcc_assert (dovar_found != 2);

      if (i + 1 < collapse)
        code = code->block->next;
    }

  if (pblock != &block)
    {
      pushlevel ();
      gfc_start_block (&block);
    }

  gfc_start_block (&body);

  FOR_EACH_VEC_ELT (inits, ix, di)
    gfc_add_modify (&body, di->var, di->init);
  inits.release ();

  /* Cycle statement is implemented with a goto.  Exit statement must not be
     present for this loop.  */
  cycle_label = gfc_build_label_decl (NULL_TREE);

  /* Put these labels where they can be found later.  */

  code->cycle_label = cycle_label;
  code->exit_label = NULL_TREE;

  /* Main loop body.  */
  tmp = trans_acc_code (code->block->next, true);
  gfc_add_expr_to_block (&body, tmp);

  /* Label for cycle statements (if needed).  */
  if (TREE_USED (cycle_label))
    {
      tmp = build1_v (LABEL_EXPR, cycle_label);
      gfc_add_expr_to_block (&body, tmp);
    }

  /* End of loop body.  */
  stmt = make_node (ACC_LOOP);

  TREE_TYPE (stmt) = void_type_node;
  ACC_LOOP_BODY (stmt) = gfc_finish_block (&body);
  ACC_LOOP_CLAUSES (stmt) = acc_clauses;
  ACC_LOOP_INIT (stmt) = init;
  ACC_LOOP_COND (stmt) = cond;
  ACC_LOOP_INCR (stmt) = incr;
  gfc_add_expr_to_block (&block, stmt);

  return gfc_finish_block (&block);
}

static tree
trans_acc_update (gfc_code *code)
{
  stmtblock_t block;
  tree stmt, acc_clauses;

  gfc_start_block (&block);
  acc_clauses = trans_acc_clauses (&block, code->ext.omp_clauses,
                                       code->loc);
  stmt = build1_loc (input_location, ACC_UPDATE, void_type_node, acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
trans_acc_enter_data (gfc_code *code)
{
  stmtblock_t block;
  tree stmt, acc_clauses;

  gfc_start_block (&block);
  acc_clauses = trans_acc_clauses (&block, code->ext.omp_clauses,
                                       code->loc);
  stmt = build1_loc (input_location, ACC_ENTER_DATA, void_type_node, acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
trans_acc_exit_data (gfc_code *code)
{
  stmtblock_t block;
  tree stmt, acc_clauses;

  gfc_start_block (&block);
  acc_clauses = trans_acc_clauses (&block, code->ext.omp_clauses,
                                       code->loc);
  stmt = build1_loc (input_location, ACC_EXIT_DATA, void_type_node, acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
trans_acc_wait (gfc_code *code)
{
  stmtblock_t block;
  tree stmt, acc_clauses;

  gfc_start_block (&block);
  acc_clauses = trans_acc_clauses (&block, code->ext.omp_clauses,
                                       code->loc);
  stmt = build1_loc (input_location, ACC_WAIT, void_type_node, acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
trans_acc_cache (gfc_code *code)
{
  stmtblock_t block;
  tree stmt, acc_clauses;

  gfc_start_block (&block);
  acc_clauses = trans_acc_clauses (&block, code->ext.omp_clauses,
                                       code->loc);
  stmt = build1_loc (input_location, ACC_CACHE, void_type_node, acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
trans_acc_parallel_loop (gfc_code *code)
{
  stmtblock_t block, *pblock = NULL;
  gfc_acc_clauses parallel_clauses, loop_clauses;
  tree stmt, acc_clauses = NULL_TREE;

  gfc_start_block (&block);

  memset (&loop_clauses, 0, sizeof (loop_clauses));
  if (code->ext.omp_clauses != NULL)
    {
      memcpy (&parallel_clauses, code->ext.omp_clauses,
              sizeof (parallel_clauses));
      loop_clauses.collapse = parallel_clauses.collapse;
      loop_clauses.gang = parallel_clauses.gang;
      loop_clauses.vector = parallel_clauses.vector;
      loop_clauses.worker = parallel_clauses.worker;
      loop_clauses.seq = parallel_clauses.seq;
      loop_clauses.independent = parallel_clauses.independent;
      parallel_clauses.collapse = 0;
      parallel_clauses.gang = false;
      parallel_clauses.vector = false;
      parallel_clauses.worker = false;
      parallel_clauses.seq = false;
      parallel_clauses.independent = false;
      acc_clauses = trans_acc_clauses (&block, &parallel_clauses,
                                           code->loc);
    }
  if (!loop_clauses.seq)
    pblock = &block;
  else
    pushlevel ();
  stmt = trans_acc_loop (code, pblock, &loop_clauses);
  if (TREE_CODE (stmt) != BIND_EXPR)
    stmt = build3_v (BIND_EXPR, NULL, stmt, poplevel (1, 0));
  else
    poplevel (0, 0);
  stmt = build2_loc (input_location, ACC_PARALLEL, void_type_node, stmt,
                     acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
trans_acc_kernels_loop (gfc_code *code)
{
  stmtblock_t block, *pblock = NULL;
  gfc_acc_clauses kernels_clauses, loop_clauses;
  tree stmt, acc_clauses = NULL_TREE;

  gfc_start_block (&block);

  memset (&loop_clauses, 0, sizeof (loop_clauses));
  if (code->ext.omp_clauses != NULL)
    {
      memcpy (&kernels_clauses, code->ext.omp_clauses,
              sizeof (kernels_clauses));
      loop_clauses.collapse = kernels_clauses.collapse;
      loop_clauses.gang = kernels_clauses.gang;
      loop_clauses.vector = kernels_clauses.vector;
      loop_clauses.worker = kernels_clauses.worker;
      loop_clauses.seq = kernels_clauses.seq;
      loop_clauses.independent = kernels_clauses.independent;
      kernels_clauses.collapse = 0;
      kernels_clauses.gang = false;
      kernels_clauses.vector = false;
      kernels_clauses.worker = false;
      kernels_clauses.seq = false;
      kernels_clauses.independent = false;
      acc_clauses = trans_acc_clauses (&block, &kernels_clauses,
                                           code->loc);
    }
  if (!loop_clauses.seq)
    pblock = &block;
  else
    pushlevel ();
  stmt = trans_acc_loop (code, pblock, &loop_clauses);
  if (TREE_CODE (stmt) != BIND_EXPR)
    stmt = build3_v (BIND_EXPR, NULL, stmt, poplevel (1, 0));
  else
    poplevel (0, 0);
  stmt = build2_loc (input_location, ACC_KERNELS, void_type_node, stmt,
                     acc_clauses);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

tree
gfc_trans_acc_declare (stmtblock_t *block, gfc_namespace *ns)
{
  tree acc_clauses;
  acc_clauses = trans_acc_clauses (block, ns->declare_clauses,
                                       ns->code->loc);
  return build1_loc (input_location, ACC_DECLARE, void_type_node,
                                       acc_clauses);
}

tree
gfc_trans_acc_directive (gfc_code *code)
{
  switch (code->op)
    {
    case EXEC_ACC_PARALLEL_LOOP:
      return trans_acc_parallel_loop (code);
    case EXEC_ACC_PARALLEL:
      return trans_acc_parallel (code);
    case EXEC_ACC_KERNELS_LOOP:
      return trans_acc_kernels_loop (code);
    case EXEC_ACC_KERNELS:
      return trans_acc_kernels (code);
    case EXEC_ACC_DATA:
      return trans_acc_data (code);
    case EXEC_ACC_HOST_DATA:
      return trans_acc_host_data (code);
    case EXEC_ACC_LOOP:
      return trans_acc_loop (code, NULL, code->ext.omp_clauses);
    case EXEC_ACC_UPDATE:
      return trans_acc_update (code);
    case EXEC_ACC_WAIT:
      return trans_acc_wait (code);
    case EXEC_ACC_CACHE:
      return trans_acc_cache (code);
    case EXEC_ACC_ENTER_DATA:
      return trans_acc_enter_data (code);
    case EXEC_ACC_EXIT_DATA:
      return trans_acc_exit_data (code);
    default:
      gcc_unreachable ();
    }
}
