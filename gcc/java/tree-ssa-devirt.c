/* Java-specific devirtualization pass.
   Copyright (C) 2006 Free Software Foundation, Inc.

This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "flags.h"
#include "rtl.h"
#include "tm_p.h"
#include "ggc.h"
#include "basic-block.h"
#include "output.h"
#include "expr.h"
#include "function.h"
#include "diagnostic.h"
#include "timevar.h"
#include "tree-dump.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "tree-inline.h"
#include "ipa-utils.h"
#include "tree-ssa-propagate.h"
#include "langhooks.h"
#include "target.h"
#include "java-tree.h"

/* TODO a post-pass to check for cloning violations. Issue a warning, and
 * replace the call, and the remainder of the try block, with the contents
 * of the catch block. Alternately, remove the catch block */

void init_gcj_devirt (void);
void register_dump_files (struct tree_opt_pass*, bool, int);

/*
 * TO DO: special-case string constants
 */

/* see comment in first if statement of replace_call */
struct devirt_type_d
{
  /* This is NULL_TREE when this object has not yet been initialized.  */
  tree lower_bound;
  tree upper_bound;
};

//#define Dprintf fprintf
#define Dprintf if (dump_file) fprintf
//#define Dprint_generic_stmt print_generic_expr
#define Dprint_generic_stmt if (dump_file) print_generic_stmt

//static void nop (FILE* a __attribute__ ((unused)), ...) { }
//static void nopex (FILE* a __attribute__ ((unused)), ...) { }

static enum ssa_prop_result 
pass_through (enum ssa_prop_result result)
{
  switch (result)
    {
    case SSA_PROP_NOT_INTERESTING:
//      Dprintf (dump_file, "Result: NOT\n"); 
      break;
    case SSA_PROP_INTERESTING:
//      Dprintf (dump_file, "Result: INT\n");
      break;
    case SSA_PROP_VARYING:
//      Dprintf (dump_file, "Result: VAR\n");
      break;
    default:
      gcc_unreachable ();
    }
  return result;
}

typedef struct devirt_type_d devirt_type_t;

/* A record for every SSA_NAME.  */
static devirt_type_t *types;

static tree
unwrap (tree op)
{
  while (TREE_CODE (op) == CONVERT_EXPR
	 || TREE_CODE (op) == NOP_EXPR
	 || TREE_CODE (op) == SSA_NAME)
    {
      if (TREE_CODE (op) == SSA_NAME)
	{
	  tree orig = op;
	  op = SSA_NAME_DEF_STMT (op);
	  if (GIMPLE_STMT_P (op))
	    return orig;
	  op = TREE_OPERAND (op, 1);
	}
      else
	op = TREE_OPERAND (op, 0);
    }
  return op;
}

/* Compute the greatest common ancestor of two types.  If either type
   is an interface, this will always merge to Object.  */
static tree
gca (tree t1, tree t2)
{
  gcc_assert (TREE_CODE (t1) == TYPE_DECL);
  gcc_assert (TREE_CODE (t2) == TYPE_DECL);
  if (CLASS_INTERFACE (t1) || CLASS_INTERFACE (t2))
    return object_type_node;

  t1 = TREE_TYPE (t1);
  t2 = TREE_TYPE (t2);
  while (t1 != NULL_TREE)
    {
      tree i;
      for (i = t2; i != NULL_TREE; i = CLASSTYPE_SUPER (i))
	if (i == t1)
	  return TYPE_NAME (i);
      t1 = CLASSTYPE_SUPER (t1);
    }
  gcc_unreachable ();
}

/* Merge the type bound RIGHT into the type bound LEFT.  Return true
   if the type bound remains 'interesting', false if we merged away
   all useful information.  */
static bool
devirt_meet (devirt_type_t *left, devirt_type_t *right)
{
  /* If LEFT is undefined, result is RIGHT.  */
  if (left->lower_bound == NULL_TREE)
    {
      if (right->lower_bound == NULL_TREE)
	{
	  return false;
	}
      *left = *right;
      return true;
    }
  /* If RIGHT is undefined, LEFT is already the answer.  Otherwise,
     try to merge.  */
  if (right->lower_bound != NULL_TREE)
    {
      left->lower_bound = gca (left->lower_bound, right->lower_bound);
      if (left->lower_bound != left->upper_bound
	  || right->upper_bound != left->upper_bound)
	left->upper_bound = NULL_TREE;
    }
  /* Once we merge types down to Object, there's nothing useful we can
     do -- this will destroy the upper bound, and as Object does not
     implement any interfaces, we also won't be able to strength
     reduce interface calls.  */
  return left->lower_bound != object_type_node;
}

static devirt_type_t *
get_devirt (tree node)
{
  unsigned ver;
  gcc_assert (TREE_CODE (node) == SSA_NAME);
  ver = SSA_NAME_VERSION (node);
  Dprintf (dump_file, "%s: ", types[ver].lower_bound ? IDENTIFIER_POINTER (DECL_NAME(types[ver].lower_bound)) : "None");
  Dprint_generic_stmt (dump_file, node, 0);
  return &types[ver];
}

/* Try to determine whether a CALL_EXPR has interesting type
   properties.  */
static enum ssa_prop_result
devirt_visit_call (tree call, devirt_type_t *type)
{
  tree name, klass;
  Dprintf (dump_file, "Checking call: ");
  Dprint_generic_stmt (dump_file, call, 0);
  gcc_assert (TREE_CODE (call) == CALL_EXPR);

  name = unwrap (TREE_OPERAND (call, 0));
  if (TREE_CODE (name) != ADDR_EXPR)
    return pass_through (SSA_PROP_VARYING);
  name = TREE_OPERAND (name, 0);
  if (TREE_CODE (name) != FUNCTION_DECL
      || (name != alloc_object_node && name != alloc_no_finalizer_node))
    return pass_through (SSA_PROP_VARYING);

  /* At this point we have an allocator call.  */
  klass = TYPE_NAME (TREE_TYPE (TREE_TYPE (call)));  /* FIXME? */
  gcc_assert (TREE_CODE (klass) == TYPE_DECL);

  /* Don't bother if we've already been here.  */
  if (type->lower_bound == klass && type->upper_bound == klass)
    return pass_through (SSA_PROP_NOT_INTERESTING);

  type->lower_bound = klass;
  type->upper_bound = klass;
  return pass_through (SSA_PROP_INTERESTING);
}

static enum ssa_prop_result
copy_type (tree lhs, tree rhs)
{
  Dprintf (dump_file, "Copying type: ");
  devirt_type_t *v1 = get_devirt (lhs);
  if (TREE_CODE (rhs) == SSA_NAME)
    {
      devirt_type_t *v2 = get_devirt (rhs);
      *v1 = *v2;
    }
  else
    {
      v1->lower_bound = TREE_TYPE (rhs);
      v1->upper_bound = TREE_TYPE (rhs);
    }
  return v1->lower_bound ? pass_through (SSA_PROP_INTERESTING) : pass_through (SSA_PROP_NOT_INTERESTING);
}

static enum ssa_prop_result
devirt_visit_statement (tree node, 
			edge *taken_edge_p __attribute__((unused)), 
			tree *result_p __attribute__((unused)))
{
  Dprintf (dump_file, "Checking statement: ");
  Dprint_generic_stmt (dump_file, node, 0);
  if (GIMPLE_STMT_P (node))
    {
      devirt_type_t *virt;
      /* We only care about reference types.  */
      if (!POINTER_TYPE_P (TREE_TYPE (GIMPLE_STMT_OPERAND (node, 0))))
	return pass_through (SSA_PROP_NOT_INTERESTING);
      /* And only about SSA_NAMEs.  */
      if (TREE_CODE (GIMPLE_STMT_OPERAND (node, 0)) != SSA_NAME)
	return pass_through (SSA_PROP_NOT_INTERESTING);
      if (TREE_CODE (GIMPLE_STMT_OPERAND (node, 1)) == SSA_NAME)
	return copy_type (GIMPLE_STMT_OPERAND (node, 0),
			  GIMPLE_STMT_OPERAND (node, 1));
      if (TREE_CODE (GIMPLE_STMT_OPERAND (node, 1)) == NOP_EXPR
	  || TREE_CODE (GIMPLE_STMT_OPERAND (node, 1)) == CONVERT_EXPR)
	return copy_type (GIMPLE_STMT_OPERAND (node, 0),
			  TREE_OPERAND (GIMPLE_STMT_OPERAND (node, 1), 0));
      if (TREE_CODE (GIMPLE_STMT_OPERAND (node, 1)) != CALL_EXPR)
	return pass_through (SSA_PROP_NOT_INTERESTING);
      virt = get_devirt (GIMPLE_STMT_OPERAND (node, 0));
      return devirt_visit_call (GIMPLE_STMT_OPERAND (node, 1), virt);
    }
  else if (TREE_CODE (node) == COND_EXPR)
    {
      /* FIXME */
      return pass_through (SSA_PROP_VARYING);
    }
  else
    return pass_through (SSA_PROP_NOT_INTERESTING);
}

static enum ssa_prop_result
devirt_visit_phi (tree phi)
{
  int i;
  devirt_type_t result = { NULL_TREE, NULL_TREE };
  devirt_type_t *self = get_devirt (PHI_RESULT (phi));
  bool interesting_edge_exists = false;

  Dprintf (dump_file, "Checking phi: ");
  Dprint_generic_stmt (dump_file, phi, 0);

  if (! POINTER_TYPE_P (TREE_TYPE (phi)))
    return pass_through (SSA_PROP_NOT_INTERESTING);

  for (i = 0; i < PHI_NUM_ARGS (phi); ++i)
    {
      edge e = PHI_ARG_EDGE (phi, i);
      if (e->flags & EDGE_EXECUTABLE)
	{
	  interesting_edge_exists = true;
	  tree arg = PHI_ARG_DEF (phi, i);
	  Dprintf (dump_file, "Interesting edge: ");
	  Dprint_generic_stmt (dump_file, arg, 0);
	  if (TREE_CODE (arg) == SSA_NAME)
	    {
	      devirt_type_t *argtype = get_devirt (arg);
	      if (! devirt_meet (&result, argtype))
		return pass_through (SSA_PROP_VARYING);

	      Dprintf (dump_file, "Actual interesting edge: ");
	      Dprint_generic_stmt (dump_file, arg, 0);
	    }
	}
    }

  /* try this */
  if (self->lower_bound == result.lower_bound
      && self->upper_bound == result.upper_bound)
    return pass_through (SSA_PROP_NOT_INTERESTING);

  *self = result;
  return interesting_edge_exists ? pass_through (SSA_PROP_INTERESTING) : pass_through (SSA_PROP_NOT_INTERESTING);
}

static void
devirt_initialize (void)
{
  types = XNEWVEC (devirt_type_t, num_ssa_names);
  memset (types, 0, num_ssa_names * sizeof (devirt_type_t));

  basic_block bb;
  FOR_EACH_BB (bb)
    {
      block_stmt_iterator i;
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	DONT_SIMULATE_AGAIN (phi) = false;

      for (i = bsi_start (bb); !bsi_end_p (i); )
	{
	  tree stmt = bsi_stmt (i);
	  DONT_SIMULATE_AGAIN (stmt) = false;
	  bsi_next (&i);
	}
    }
}

static void
devirt_destroy (void)
{
  free (types);
  types = NULL;
}

static bool
replace_call (tree *stmt, tree method, devirt_type_t *vtype)
{
  if (vtype->upper_bound != NULL_TREE)
    {
      tree found_method, signature;
      if (dump_file) fprintf (dump_file, "Devirtualizing call in %s\n",
	       IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));
      /* Here we know that the object is the result of a 'new'.  So,
	 we don't have to insert a null check here, and we can turn
	 any sort of call into a direct call.  */
      gcc_assert (vtype->lower_bound == vtype->upper_bound);
      signature = build_java_argument_signature (TREE_TYPE (method));
      /* FIXME: what about method return type?  Argh.  */
      found_method = lookup_argument_method (TREE_TYPE (vtype->upper_bound),
					     DECL_NAME (method),
					     signature);
      gcc_assert (found_method);
      *stmt = build_address_of (found_method);
      return true;
    }
  else if (vtype->lower_bound && CLASS_FINAL (vtype->lower_bound))
    {
      /* FIXME: if the class is final, we could handle it
	 here...  but then we would have to introduce a null
	 pointer check.  */
    }
  else if (CLASS_INTERFACE (TYPE_NAME (DECL_CONTEXT (method))))
    {
      /* For an interface method, we can only reduce if the merge type
	 we've found actually implements the interface.  */
      if (vtype->lower_bound
	  && can_widen_reference_to (vtype->lower_bound,
				     TYPE_NAME (DECL_CONTEXT (method))))
	{
	  /* FIXME: for now we don't handle these.  */
	  /* it would be fairly simple though... */
	}
    }
    return false;
}

static void
remove_stmt (tree stmt)
{
  basic_block bb = bb_for_stmt (stmt);
  block_stmt_iterator i; 

  fprintf (dump_file, "Removing statement from block %d: ", bb->index);
  print_generic_stmt (dump_file, stmt, 0);

  for (i = bsi_start (bb); !bsi_end_p (i); bsi_next (&i))
    {
      if (bsi_stmt (i) == stmt)
	{
	  bsi_remove (&i, true); /* We're not putting this back anywhere */
	  return;
	}
    }
  gcc_unreachable ();
}

/* Find the indirect call related to the lhs of STMT, and replace it with
 * the rhs. Remove inconvenient statements along the way, in particular
 * vtable offseting and casts. */
static void
propagate_function_value_to_call (tree stmt)
{
  gcc_assert (GIMPLE_STMT_P (stmt));
  tree addr_expr = GIMPLE_STMT_OPERAND (stmt, 1);
  tree function = TREE_OPERAND (addr_expr, 0);
  gcc_assert (TREE_CODE (function) == FUNCTION_DECL);
  tree var = GIMPLE_STMT_OPERAND (stmt, 0);
  gcc_assert (TREE_CODE (var) == SSA_NAME);
  imm_use_iterator iter;
  use_operand_p use_p;
  int count;

  /* First, find the cast to the pointer type */
  count = 0;
  tree use_stmt1;
  FOR_EACH_IMM_USE_FAST (use_p, iter, var)
    {
      count++;
      use_stmt1 = USE_STMT (use_p);
    }
  gcc_assert (count == 1);
  gcc_assert (GIMPLE_STMT_P (use_stmt1));
  tree nop_expr = GIMPLE_STMT_OPERAND (use_stmt1, 1);
  gcc_assert (TREE_CODE (nop_expr) == NOP_EXPR);
  tree casted_var = GIMPLE_STMT_OPERAND (use_stmt1, 0);
  gcc_assert (TREE_CODE (casted_var) == SSA_NAME);
  /* Check the type is as expected */
  gcc_assert (TREE_TYPE (function) == TREE_TYPE (TREE_TYPE (nop_expr)));

  /* Find the indirect function call*/
  count = 0;
  tree use_stmt2;
  FOR_EACH_IMM_USE_FAST (use_p, iter, casted_var)
    {
      count++;
      use_stmt2 = USE_STMT (use_p);
    }
  gcc_assert (count == 1);
  tree current_function_call = get_call_expr_in (use_stmt2);
  gcc_assert (TREE_CODE (TREE_OPERAND (current_function_call, 0)) == SSA_NAME);

  /* replace with direct function call */
  TREE_OPERAND (current_function_call, 0) = addr_expr;

  mark_symbols_for_renaming (use_stmt2);
  /* Now remove stmt and use_stmt1 */
  remove_stmt (stmt);
  remove_stmt (use_stmt1);
}

static void
devirt_devirt (void)
{
  basic_block bb;
  FOR_EACH_BB (bb)
    {
      block_stmt_iterator i;
      bool redo_eh = false;
      for (i = bsi_start (bb); !bsi_end_p (i); bsi_next (&i))
	{
	  tree method;
	  devirt_type_t *vtype;

	  tree stmt = bsi_stmt (i);
	  /* All the calls we can handle are wrapped in OBJ_TYPE_REF
	     nodes.  So simply look for these.  */
	  if (GIMPLE_STMT_P (stmt)
	      && TREE_CODE (GIMPLE_STMT_OPERAND (stmt, 1)) == OBJ_TYPE_REF)
	    {
	      tree objr = GIMPLE_STMT_OPERAND (stmt, 1);
	      tree object = OBJ_TYPE_REF_OBJECT (objr);
	      if (TREE_CODE (object) != INTEGER_CST) /* NULL has been observed here, despite being a semantic error */
		{
		  vtype = get_devirt (object);
		  method = OBJ_TYPE_REF_TOKEN (objr);
		  if (replace_call (&GIMPLE_STMT_OPERAND (stmt, 1), method, vtype))
		    {
/*		      recompute_tree_invariant_for_addr_expr (GIMPLE_STMT_OPERAND (stmt,
										   1));*/
		      propagate_function_value_to_call (stmt);
/*		      mark_symbols_for_renaming (stmt);*/
		      redo_eh = true;
		    }
		}
	    }
	}

      if (redo_eh)
	tree_purge_dead_eh_edges (bb);
    }

}

static unsigned int
devirt (void)
{
  if (dump_file) fprintf (dump_file, "Beginning devirtualization\n");

  /* Avoid printing propagation info */
  FILE* saved = dump_file;
  dump_file = NULL;
  devirt_initialize ();

  /* In the first phase, propagate type information.  */
  ssa_propagate (devirt_visit_statement, devirt_visit_phi);

  dump_file = saved;
  /* In the second phase, devirtualize method calls.  */
  devirt_devirt ();

  devirt_destroy ();
  if (dump_file) fprintf (dump_file, "Finished devirtualization\n");
  return 0;
}


static unsigned int
ipa_devirt (void)
{
  struct cgraph_node *node;
  struct cgraph_node **order;
  int order_pos;
  int i;

  if (sorrycount || errorcount)
    return 0;

  order = XCNEWVEC (struct cgraph_node *, cgraph_n_nodes);
  order_pos = ipa_utils_reduced_inorder (order, true, false);

  for (i = 0; i < order_pos; i++)
    {
      node = order[i];
      if (node->analyzed)
	{
	  /* set up the current function */
	  push_cfun (DECL_STRUCT_FUNCTION (node->decl));
	  tree_register_cfg_hooks ();
	  current_function_decl = node->decl;

	  devirt ();

	  /* Clean up function */
	  current_function_decl = NULL;
	  pop_cfun ();

	}
    }

  /* Cleanup. */
  for (node = cgraph_nodes; node; node = node->next)
    {
      /* Get rid of the aux information.  */
      if (node->aux)
	{
	  struct ipa_dfs_info *w_info;
	  w_info = node->aux;
	  if (w_info->aux)
	    free (w_info->aux);
	  free (node->aux);
	  node->aux = NULL;
	}
    }
  free (order);

  return 0;
}

struct tree_opt_pass pass_gcj_devirtualize = 
{
  "devirt",				/* name */
  NULL,					/* gate */
  ipa_devirt,				/* execute */
  &pass_rebuild_cgraph_edges,		/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_ssa,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func
    | TODO_verify_ssa
    | TODO_update_ssa
    | TODO_cleanup_cfg,			/* todo_flags_finish */
  0					/* letter */
};



extern struct tree_opt_pass pass_ipa_necessary;

void
init_gcj_devirt (void)
{
  /* FIXME: GCC doesn't have a way to register lang-specific passes.  */
  /* I want this done before stack_allocation */
  struct tree_opt_pass **p = &all_ipa_passes;
  struct tree_opt_pass *next = &pass_ipa_stack_allocate;

  while (*p != next)
    p = &((*p)->next);
  *p = &pass_gcj_devirtualize;
  pass_gcj_devirtualize.next = next;
  register_dump_files (&pass_gcj_devirtualize, true, 0);
}
