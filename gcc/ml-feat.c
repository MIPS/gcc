/* Feature extraction support
   Copyright (C) 2009 Free Software Foundation, Inc.
   Contributed by Mircea Namolaru <namolaru@il.ibm.com>, 
   Sergei Dyshel <sergeid@il.ibm.com>.
   
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
#undef FLOAT /* This is for hpux. They should change hpux.  */
#undef FFS  /* Some systems define this in param.h.  */
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include <signal.h>

#ifdef HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif

#ifdef HAVE_SYS_TIMES_H
# include <sys/times.h>
#endif

#include "line-map.h"
#include "input.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "flags.h"
#include "insn-config.h"
#include "insn-flags.h"
#include "hard-reg-set.h"
#include "recog.h"
#include "output.h"
#include "except.h"
#include "function.h"
#include "toplev.h"
#include "expr.h"
#include "basic-block.h"
#include "intl.h"
#include "ggc.h"
#include "graph.h"
#include "regs.h"
#include "timevar.h"
#include "diagnostic.h"
#include "params.h"
#include "reload.h"
#include "dwarf2asm.h"
#include "integrate.h"
#include "real.h"
#include "debug.h"
#include "target.h"
#include "langhooks.h"
#include "cfglayout.h"
#include "cfgloop.h"
#include "hosthooks.h"
#include "cgraph.h"
#include "opts.h"
#include "value-prof.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "tree-dump.h"
#include "hashtab.h"

static FILE *ml_fp_interf = NULL;

static void
ml_feat_init (void)
{
  ml_fp_interf = fopen ("features.P", "w");

  fprintf (ml_fp_interf, "fn_name(fn,'%s').\n", lang_hooks.decl_printable_name (current_function_decl, 2));
}

static void
ml_feat_close (void)
{
  fclose (ml_fp_interf);
}

static unsigned int expr_nr;
static unsigned int type_nr;
static unsigned int var_nr;
static unsigned int edge_nr;
static unsigned int stmt_nr;

/* The hash table for types.  */
struct ml_state 
{
  htab_t var_decl_hash_table;
  VEC(tree,heap) *var_decls;

  htab_t type_decl_hash_table;
  VEC(tree,heap) *type_decls;
};

struct ml_state *mls;

struct ml_decl 
{
  tree t;
  int index;
};

static hashval_t
key_ml_decl_node (const void *p)
{
  const struct ml_decl *ds = (const struct ml_decl *) p;
  return (hashval_t) DECL_UID (ds->t);
}

static int
eq_ml_decl_node (const void *p1, const void *p2)
{
  const struct ml_decl *ds1 =
    (const struct ml_decl *) p1;
  const struct ml_decl *ds2 =
    (const struct ml_decl *) p2;

  return DECL_UID (ds1->t) == DECL_UID (ds2->t);
}

static hashval_t
key_ml_type_node (const void *p)
{
  const struct ml_decl *ds = (const struct ml_decl *) p;
  return (hashval_t) TYPE_UID (ds->t);
}

static int
eq_ml_type_node (const void *p1, const void *p2)
{
  const struct ml_decl *ds1 =
    (const struct ml_decl *) p1;
  const struct ml_decl *ds2 =
    (const struct ml_decl *) p2;

  return TYPE_UID (ds1->t) == TYPE_UID (ds2->t);
}

static bool
ml_decl_index (htab_t table,
	       unsigned int *next_index, tree name, 
	       unsigned int *this_index)
{
  void **slot;
  struct ml_decl d_slot;
  d_slot.t = name;

  slot = htab_find_slot (table, &d_slot, INSERT);
  if (*slot == NULL)
    {
      struct ml_decl *new_slot = XNEW (struct ml_decl);
      int index = (*next_index);

      new_slot->t = name;
      new_slot->index = index;
      *this_index = index;
      *slot = new_slot;
      return true;
    }
  else
    {
      struct ml_decl *old_slot = (struct ml_decl *)*slot;
      *this_index = old_slot->index;
      return false;
    }
}

static int
ml_decl_ref (tree node, unsigned int *index)
{
  bool is_new;

  is_new = ml_decl_index (mls->var_decl_hash_table, &var_nr, node, index);

  if (is_new)
    {
      fprintf(ml_fp_interf,"var_p(v%d).\n",var_nr++);
      VEC_safe_push (tree, heap, mls->var_decls, node);
    }

  return is_new;
}

static int
ml_type_ref (tree node, unsigned int *index)
{
  bool is_new;

  is_new = ml_decl_index (mls->type_decl_hash_table, &expr_nr, node, index);

  if (is_new)
    {
      fprintf(ml_fp_interf,"type_p(e%d).\n",expr_nr++);
      VEC_safe_push (tree, heap, mls->type_decls, node);
    }

  return is_new;
}

static void
ml_inp_expr (tree t, int expr_index)
{
  tree x;
  int i;
  int prev_op;

  if (t == NULL)
    return;

  fprintf (ml_fp_interf,"expr_class(e%d,%s).\n",
	   expr_index,
	   TREE_CODE_CLASS_STRING (TREE_CODE_CLASS(TREE_CODE (t))));

  fprintf (ml_fp_interf,"expr_code(e%d,%s).\n",
	   expr_index,
	   tree_code_name [(int)TREE_CODE (t)]);

  /* Flags */
  if (t->common.base.addressable_flag)
    fprintf (ml_fp_interf, "expr_flags(e%d,adddresable).\n",
	     expr_index);

  if (t->common.base.static_flag)
    fprintf (ml_fp_interf, "expr_flags(e%d,static).\n",
	     expr_index);

  if (t->common.base.volatile_flag)
    fprintf (ml_fp_interf, "expr_flags(e%d,volatile).\n",
	     expr_index);

  if (t->common.base.side_effects_flag)
    fprintf (ml_fp_interf, "expr_flags(e%d,side_effects).\n",
	     expr_index);

  if (t->common.base.constant_flag)
    fprintf (ml_fp_interf, "expr_flags(e%d,constant).\n",
	     expr_index);

  if (t->common.base.nothrow_flag)
    fprintf (ml_fp_interf, "expr_flags(e%d,nothrow).\n",
	     expr_index);

  if (t->common.base.public_flag)
    fprintf (ml_fp_interf, "expr_flags(e%d,public).\n",
	     expr_index);

  if (TREE_TYPE (t))
    {
      unsigned int index;
      bool is_new;

      is_new = ml_type_ref (TREE_TYPE (t), &index);

      fprintf (ml_fp_interf,"expr_type(e%d,e%d). \n",
		 expr_index,
		 index);

      if (is_new)
	ml_inp_expr (TREE_TYPE (t), index);
    }

  fprintf (ml_fp_interf,"expr_code_len(e%d,%d).\n",
	   expr_index,
	   TREE_CODE_LENGTH (TREE_CODE (t)));

  prev_op = -1;
  for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (t)); i++)
    {
      if (!TREE_OPERAND (t, i))
	continue;

      if (prev_op == -1)
	fprintf (ml_fp_interf, "expr_op_f(e%d,e%d).\n",
		 expr_index, expr_nr);
      else
	fprintf (ml_fp_interf, "expr_op_n(e%d,e%d,e%d).\n",
		 expr_index, prev_op, expr_nr);

      prev_op = expr_nr;

      ml_inp_expr (TREE_OPERAND (t, i), expr_nr++);
    }

  switch (TREE_CODE (t))
    {
    case SSA_NAME:
      fprintf (ml_fp_interf, "expr_ssa_name(e%d,s%d).\n",
	       expr_index, SSA_NAME_VERSION (t));

      fprintf (ml_fp_interf, "expr_ssa_var(e%d,e%d).\n",
	       expr_index, expr_nr);

      ml_inp_expr (SSA_NAME_VAR (t), expr_nr++);
      break;

    case INTEGER_TYPE:
      fprintf (ml_fp_interf, "expr_int_size(e%d,%d).\n",
	       expr_index, TYPE_PRECISION(t));
      break;

    case INTEGER_CST:
      if (TREE_INT_CST_HIGH (t) != 0)
	break;

      if ((long int)TREE_INT_CST_LOW (t) < 0)
	break;

      fprintf (ml_fp_interf, "expr_intcst_val(e%d,%d).\n",
	       expr_index, (int)TREE_INT_CST_LOW (t));
      break;

    case ASSERT_EXPR:
      break;

    case LABEL_EXPR:
      break;

    case RESULT_DECL:
    case FIELD_DECL:
    case TYPE_DECL:
    case FUNCTION_DECL:
    case PARM_DECL:
    case VAR_DECL:
      {
	unsigned int index;
	bool is_new;

	is_new = ml_decl_ref (t, &index);

	fprintf (ml_fp_interf,"expr_var(e%d,v%d). \n",
     	         expr_index,
	         index);
      }
      break;


    case MODIFY_EXPR:
      break;

    case ADDR_EXPR:
      {
	/* Skip any references and ensure that any 
           variable used as a prefix is marked
	   addressable.  */
	for (x = TREE_OPERAND (t, 0);
	     handled_component_p (x);
	     x = TREE_OPERAND (x, 0))
	  ;

	if (TREE_CODE (x) != VAR_DECL && TREE_CODE (x) != PARM_DECL)
	  {
	    return;
	  }
	if (!TREE_ADDRESSABLE (x))
	  {
	    error ("address taken, but ADDRESSABLE bit not set");
	    return;
	  }
	break;
      }

    case COND_EXPR:
      x = COND_EXPR_COND (t);
      if (TREE_CODE (TREE_TYPE (x)) != BOOLEAN_TYPE)
	{
	  error ("non-boolean used in condition");
	  return;
	}
      if (!is_gimple_condexpr (x))
        {
	  error ("invalid conditional operand");
	  return;
	}
      break;

    case NOP_EXPR:
    case CONVERT_EXPR:
    case FIX_TRUNC_EXPR:
    case FLOAT_EXPR:
    case NEGATE_EXPR:
    case ABS_EXPR:
    case BIT_NOT_EXPR:
    case NON_LVALUE_EXPR:
    case TRUTH_NOT_EXPR:
      break;

    case REALPART_EXPR:
    case IMAGPART_EXPR:
    case COMPONENT_REF:
    case ARRAY_REF:
    case ARRAY_RANGE_REF:
    case BIT_FIELD_REF:
    case VIEW_CONVERT_EXPR:
      /* We have a nest of references.  Verify that each of the operands
	 that determine where to reference is either a constant or a variable,
	 verify that the base is valid */
      while (handled_component_p (t))
	{
	  t = TREE_OPERAND (t, 0);
	}

      break;

    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case UNORDERED_EXPR:
    case ORDERED_EXPR:
    case UNLT_EXPR:
    case UNLE_EXPR:
    case UNGT_EXPR:
    case UNGE_EXPR:
    case UNEQ_EXPR:
    case LTGT_EXPR:
    case PLUS_EXPR:
    case MINUS_EXPR:
    case MULT_EXPR:
    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case CEIL_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case ROUND_MOD_EXPR:
    case RDIV_EXPR:
    case EXACT_DIV_EXPR:
    case MIN_EXPR:
    case MAX_EXPR:
    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
    case LROTATE_EXPR:
    case RROTATE_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
    case BIT_AND_EXPR:
      break;

    case CONSTRUCTOR:
      break;

    case TREE_LIST:
      {
	tree t1;
	int prev;

	prev = -1;
	for (t1 = t; t1; t1 = TREE_CHAIN (t1))
	  {
	    if (!TREE_VALUE (t1))
	      continue;

	    if (prev == -1)
	      fprintf (ml_fp_interf, "expr_lst_f(e%d,e%d).\n",
		       expr_index, expr_nr);
	    else
	      fprintf (ml_fp_interf, "expr_lst_n(e%d,e%d,e%d).\n",
		       expr_index, prev, expr_nr);

	    prev = expr_nr;

	    ml_inp_expr (TREE_VALUE (t1), expr_nr++);
	  }

      }
      break;

    case CALL_EXPR:
      break;

    default:
      break;
    }

  return;
}

static void
ml_inp_edge (edge e, int edge_index)
{

  if (e->src)
    fprintf (ml_fp_interf, "edge_src(ed%d,bb%d).\n", 
	     edge_index, e->src->index);

  if (e->dest)
    fprintf (ml_fp_interf, "edge_dest(ed%d,bb%d).\n", 
	     edge_index, e->dest->index);

  if (e->probability)
    fprintf (ml_fp_interf, "edge_prob(ed%d,%.1f).\n", 
	     edge_index,
	     e->probability * 100.0 / REG_BR_PROB_BASE);

  if (e->count)
    fprintf (ml_fp_interf, "edge_count(ed%d,%d).\n", 
	     edge_index, (int)e->count);

  if (e->flags)
    {
      static const char * const bitnames[] = {
	"fallthru", "abnormal", "abnormal_call", "eh", 
	"fake", "dfs_back", "can_fallthru", "irreducible", 
	"sibcall", "loop_exit",
	"true", "false", "exec"
      };
      int i, flags = e->flags;

      for (i = 0; flags; i++)
	if (flags & (1 << i))
	  {
	    flags &= ~(1 << i);

	    if (i < (int) ARRAY_SIZE (bitnames))
	      fprintf (ml_fp_interf, "edge_flag(ed%d,%s). \n", edge_index, bitnames[i]);
	  }
    }
}

static void
ml_inp_bb (basic_block bb)
{
  edge e;
  edge_iterator ei;

  fprintf (ml_fp_interf, "%% bb start\n");

  if (bb->frequency)
    fprintf (ml_fp_interf, "bb_freq(bb%d,%d).\n", 
	     bb->index, bb->frequency);

  if (bb->count)
    fprintf (ml_fp_interf, "bb_count(bb%d,%d).\n", 
	     bb->index, (int)bb->count);

  FOR_EACH_EDGE (e, ei, bb->preds)
    ml_inp_edge (e, edge_nr++);

  fprintf (ml_fp_interf, "%% bb end\n");
}

static void
ml_inp_stmt (gimple stmt, int stmt_index)
{
  static const char *gimple_rhs_class_name[] = {"", "binary", "unary", "single"};
  unsigned int i;
  
  fprintf (ml_fp_interf, "%% stmt start\n");

  if (is_gimple_omp (stmt))
      return;

  fprintf (ml_fp_interf, "stmt_code(st%d,%s).\n", stmt_index,
           gimple_code_name [gimple_code (stmt)]);
  fprintf (ml_fp_interf, "stmt_num_ops(st%d,%d).\n", stmt_index,
           gimple_num_ops (stmt));
  if (gimple_has_mem_ops (stmt))
    fprintf (ml_fp_interf, "stmt_flag(st%d,has_mem_ops).\n", stmt_index);
  if (gimple_has_volatile_ops (stmt))
    fprintf (ml_fp_interf, "stmt_flag(st%d,has_volatile_ops).\n", stmt_index);

  switch (gimple_code (stmt))
    {
    case GIMPLE_ASSIGN:
      fprintf (ml_fp_interf, "assign_class(st%d,%s).\n", stmt_index, 
               gimple_rhs_class_name [gimple_assign_rhs_class (stmt)]);
      fprintf (ml_fp_interf, "assign_subcode(st%d,%s).\n", stmt_index, 
	       tree_code_name [gimple_assign_rhs_code (stmt)]);
      if (gimple_assign_cast_p (stmt))
        fprintf (ml_fp_interf, "assign_flag(st%d,%s).\n", stmt_index, "cast"); /* TODO: needed? */
      fprintf (ml_fp_interf, "assign_lhs(st%d,e%d).\n", stmt_index, expr_nr);
      ml_inp_expr (gimple_assign_lhs (stmt), expr_nr++);
      fprintf (ml_fp_interf, "assign_rhs1(st%d,e%d).\n", stmt_index, expr_nr);
      ml_inp_expr (gimple_assign_rhs1 (stmt), expr_nr++);
      if (gimple_assign_rhs_class (stmt) == GIMPLE_BINARY_RHS)
        {
          fprintf (ml_fp_interf, "assign_rhs2(st%d,e%d).\n", stmt_index, expr_nr);
          ml_inp_expr (gimple_assign_rhs2 (stmt), expr_nr++);
        }
      break;

    case GIMPLE_CALL:
      fprintf (ml_fp_interf, "call_num_args(st%d,%d).\n", stmt_index,
               gimple_call_num_args (stmt));
      if (gimple_call_tail_p (stmt))
        fprintf (ml_fp_interf, "call_flag(st%d,tail).\n", stmt_index);
      if (gimple_call_noreturn_p (stmt))
        fprintf (ml_fp_interf, "call_flag(st%d,noreturn).\n", stmt_index);
      if (gimple_call_cannot_inline_p (stmt))
        fprintf (ml_fp_interf, "call_flag(st%d,uninlinable).\n", stmt_index);
      fprintf (ml_fp_interf, "call_lhs(st%d,e%d).\n", stmt_index, expr_nr);
      ml_inp_expr (gimple_call_lhs (stmt), expr_nr++);
      fprintf (ml_fp_interf, "call_fn(st%d,e%d).\n", stmt_index, expr_nr);
      ml_inp_expr (gimple_call_fn (stmt), expr_nr++);
      fprintf (ml_fp_interf, "call_treturn(st%d,e%d).\n", stmt_index, expr_nr);
      ml_inp_expr (gimple_call_return_type (stmt), expr_nr++);

      for (i = 0; i < gimple_call_num_args (stmt); i++)
        {
          fprintf (ml_fp_interf, "call_arg(st%d,e%d).\n", stmt_index, expr_nr);
          ml_inp_expr (gimple_call_arg (stmt, i), expr_nr++);
        }
      break;

    case GIMPLE_COND:
      /* left operand */
      fprintf (ml_fp_interf, "cond_op(st%d,e%d).\n", stmt_index, expr_nr); 
      ml_inp_expr (gimple_cond_lhs (stmt), expr_nr++);
      /* right operand */
      fprintf (ml_fp_interf, "cond_op(st%d,e%d).\n", stmt_index, expr_nr);
      ml_inp_expr (gimple_cond_rhs (stmt), expr_nr++);
      /* labels */
      fprintf (ml_fp_interf, "cond_true(st%d,e%d).\n", stmt_index, expr_nr);
      ml_inp_expr (gimple_cond_true_label (stmt), expr_nr++);
      fprintf (ml_fp_interf, "cond_false(st%d,e%d).\n", stmt_index, expr_nr);
      ml_inp_expr (gimple_cond_false_label (stmt), expr_nr++);
      break;

    default:
      for (i = 0; i < gimple_num_ops (stmt); i++)
        {
          if (gimple_op (stmt, i) != NULL_TREE)  
            {
              fprintf (ml_fp_interf, "stmt_op(st%d,e%d).\n",
                       stmt_index, expr_nr);
              ml_inp_expr (gimple_op (stmt, i), expr_nr++);
            }
        }
    } /* switch */

  fprintf (ml_fp_interf, "%% stmt end\n");
}

static void
ml_inp_phi (basic_block bb)
{
  int phi_num;
  gimple phi;
  gimple_stmt_iterator gsi;

  fprintf (ml_fp_interf, "%% phi start\n");
  phi_num = 0;
  for (gsi = gsi_start (phi_nodes (bb)); !gsi_end_p (gsi); gsi_next (&gsi))
    {
      phi = gsi_stmt (gsi);
      int phi_len = gimple_phi_num_args (phi);
      int i;

      fprintf (ml_fp_interf,"phi_bb(phi%d,bb%d). \n",
	       phi_num,
	       bb->index);
      
      fprintf (ml_fp_interf,"phi_len(phi%d,%d). \n",
	       phi_num,
	       phi_len);

      fprintf (ml_fp_interf,"phi_result(phi%d,s%d). \n",
	       phi_num,
	       SSA_NAME_VERSION (PHI_RESULT (phi)));
      
      for (i = 0; i < phi_len; i++)
	{
	  fprintf (ml_fp_interf,"phi_arg(bb%d,e%d). \n",
		   SSA_NAME_VERSION (PHI_RESULT (phi)),
		   expr_nr);
	  
	  ml_inp_expr (PHI_ARG_DEF (phi, i), expr_nr++);
	}
    }
  fprintf (ml_fp_interf, "%% phi end\n");
}

static void 
ml_feat_extract (void)
{
  basic_block bb;
  gimple_stmt_iterator gsi;

  fprintf (ml_fp_interf, "%% fn start\n");

  mls = XCNEW (struct ml_state);
  mls->var_decl_hash_table
    = htab_create (37, key_ml_decl_node, eq_ml_decl_node, free);
  mls->type_decl_hash_table
    = htab_create (37, key_ml_type_node, eq_ml_type_node, free);

  expr_nr = 0;
  type_nr = 0;
  var_nr = 0;
  edge_nr = 0;
  stmt_nr = 0;

  fprintf (ml_fp_interf, "fn_bb_nr(fn,%d).\n", n_basic_blocks);
  fprintf (ml_fp_interf, "fn_edge_nr(fn,%d).\n", n_edges);

  FOR_EACH_BB (bb)
    {
      int prev;

      fprintf (ml_fp_interf, "bb_p(bb%d).\n", bb->index);

      ml_inp_bb (bb);

      ml_inp_phi (bb);

      prev = -1;
      for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
        {
          gimple stmt = gsi_stmt (gsi);

          if (prev == -1)
            fprintf (ml_fp_interf, "bb_stmt_f(bb%d,st%d).\n",
                     bb->index, stmt_nr);
          else
            fprintf (ml_fp_interf, "bb_stmt_n(bb%d,st%d,st%d).\n",
                     bb->index, prev, stmt_nr);

          prev = stmt_nr;
          ml_inp_stmt (stmt, stmt_nr++);
        }
    }

  htab_delete (mls->var_decl_hash_table);
  htab_delete (mls->type_decl_hash_table);

  VEC_free (tree, heap, mls->var_decls);
  VEC_free (tree, heap, mls->type_decls);

  free(mls);

  fprintf (ml_fp_interf, "%% fn end\n");
}

static bool
gate_handle_ml_feat (void)
{
  return 1;
}

static unsigned int
rest_of_handle_ml_feat (void)
{
  ml_feat_init ();
  ml_feat_extract ();
  ml_feat_close ();

  return 0;
}

struct gimple_opt_pass pass_ml_feat = 
{
  {
    GIMPLE_PASS,				      /* opt_pass_type */
    "ml-feat",			        /* name */
    gate_handle_ml_feat,			/* gate */
    rest_of_handle_ml_feat,		/* execute */
    NULL,					/* sub */
    NULL,					/* next */
    0,					/* static_pass_number */
    (timevar_id_t) 0,			/* tv_id */
    0,	                                /* properties_required */
    0,					/* properties_provided */
    0,					/* properties_destroyed */
    0,					/* todo_flags_start */
    TODO_dump_func,
  }
};
