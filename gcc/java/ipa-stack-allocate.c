/* A pass for escape analysis, for stack allocation.
   Copyright (C) 2006 Free Software Foundation, Inc.
   Contributed by Paul Biggar <paul.biggar@gmail.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.	If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA. */


/*
 * TODO remove terminal nodes incomming deferred edges according to
 * toplas03, Sect 4.2
 * TODO you must propagate before you bypass
 */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "basic-block.h"
#include "timevar.h"
#include "ggc.h"
#include "hashtab.h"
#include "langhooks.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "tree-inline.h"
#include "tree-pass.h"
#include "ipa-utils.h"
#include "except.h"
#include "congraph.h"
#include "java/java-tree.h"

void init_gcj_stack_allocate (void);
bool block_can_reach (basic_block, basic_block);
void devirt (void);
void register_dump_files (struct tree_opt_pass*, bool, int);

static bool ea_interprocedural;
static bool ea_flow_sensitive;

static VEC(basic_block, heap)* ea_worklist;

/* -------------------------------------------------------------*
 *		 general use function here			*
 * -------------------------------------------------------------*/

static tree
LHS (tree stmt)
{
  tree result; 
  if (GIMPLE_STMT_P (stmt))
    result = GIMPLE_STMT_OPERAND (stmt, 0);
  else
    result = TREE_OPERAND (stmt, 0);

  gcc_assert (result);
  return result;
}

static tree
RHS (tree stmt)
{
  tree result; 
  if (GIMPLE_STMT_P (stmt))
    result = GIMPLE_STMT_OPERAND (stmt, 1);
  else
    result = TREE_OPERAND (stmt, 1);

  gcc_assert (result);
  return result;
}

static bool
has_pointer_type (tree stmt)
{
  /* From gccint 10.4.2: In GIMPLE form, type of MODIFY_EXPR is not
   * meaningful.  Instead, use type of LHS. */
  if (GIMPLE_STMT_P (stmt))
      return has_pointer_type (GIMPLE_STMT_OPERAND (stmt, 0));

  /* TODO arrays of ints should be dealt with, no? */
  tree type = TREE_TYPE (stmt);
  if (type && TREE_CODE (type) == ARRAY_TYPE)
    return has_pointer_type (TREE_TYPE (stmt));

  return POINTER_TYPE_P (TREE_TYPE (stmt));
}

void
nl (void)
{
  fprintf (dump_file, "\n");
  fflush (dump_file);
}

void
tf (tree stmt, bool newline)
{
  fprintf (dump_file, "[");
  print_generic_expr (dump_file, stmt, 0);
  fprintf (dump_file, "]");
  if (newline) nl ();
}

void
df (con_node node)
{
  tf (node->id, false);
}

static bool
is_class_type (tree decl)
{
  tree type;
  tree type_name;
  return (SSA_VAR_P (decl)
      && (type = TREE_TYPE (decl))
      && TREE_CODE (type) == RECORD_TYPE
      && (type_name = TYPE_NAME (type))
      && get_identifier ("java.lang.Class") == DECL_NAME (type_name));
}

#define MUST_VISIT TREE_DEPRECATED
static int stmts_to_visit = 0;

static void
mark_visited (tree stmt)
{
  MUST_VISIT (stmt) = 0;
  stmts_to_visit --;
}

static void
require_visit (tree stmt)
{
  MUST_VISIT (stmt) = 1;
  stmts_to_visit ++;
}

static bool
should_visit (tree stmt)
{
  return MUST_VISIT (stmt);
}
#undef MUST_VISIT

/* Mark all uses which are in a basic block that's reachable from the
 * current block. */

static basic_block current_block;

static void
mark_uses (tree var)
{
  use_operand_p use_p;
  imm_use_iterator iter;

  /* Uses of globals are irrelevent, since we never bypass them */
  if (TREE_CODE (var) != SSA_NAME)
    return; 

  FOR_EACH_IMM_USE_FAST (use_p, iter, var)
    {
      tree stmt = USE_STMT (use_p);
      basic_block bb = bb_for_stmt (stmt);
      if (block_can_reach (current_block, bb))
	{
	  fprintf (dump_file, "Marking stmt to be visited: ");
	  tf (stmt, true);
	  if (!should_visit (stmt))
	    {
	      require_visit (stmt);
	    }
	}
      else
	{
	  fprintf (dump_file, "Not Marking stmt, as its block (%d) isnt reachable from block %d: ", bb->index, current_block->index);
	  tf (stmt, true);
	}
    }
}







/* -------------------------------------------------------------*
 *		 identifying parts of a statement		*
 * -------------------------------------------------------------*/
#if 0
static bool
is_constructor (tree stmt)
{
  tree addr_expr, func_decl, arguments;
  return TREE_CODE (stmt) == CALL_EXPR
      && VOID_TYPE_P (TREE_TYPE (stmt))
      && TREE_CODE (addr_expr = LHS (stmt)) == ADDR_EXPR
      && TREE_CODE (func_decl = LHS (addr_expr)) == FUNCTION_DECL
      && DECL_NAME (func_decl) == get_identifier ("<init>")
      && (arguments = RHS (stmt))
      && TREE_CODE (arguments) == TREE_LIST;
}
#endif

static bool
is_function_constructor (tree decl)
{
  gcc_assert (TREE_CODE (decl) == FUNCTION_DECL);
  return get_identifier ("<init>") == DECL_NAME (decl);
}

/*
static tree
get_constructor_function (tree stmt)
{
  tree result;
  gcc_assert (is_constructor (stmt));

  result = LHS (LHS (stmt));
  gcc_assert (result);
  return result;
}*/

/* this is supposed to work in theory, but doesnt
static bool
is_memory_allocation (tree stmt)
{
	return TREE_CODE (stmt) == CALL_EXPR && (call_expr_flags (stmt) & (ECF_MALLOC || ECF_MAY_BE_ALLOCA));
}
*/

static bool
is_null (tree integer_cst)
{
  return TREE_CODE (integer_cst) == INTEGER_CST
    && has_pointer_type (integer_cst)
    && TREE_INT_CST_HIGH (integer_cst) == 0
    && TREE_INT_CST_LOW (integer_cst) == 0;
}

static bool
is_assignment_to_null (tree stmt, tree* p_name)
{
  return GIMPLE_STMT_P (stmt)
    && has_pointer_type (stmt)
    && has_pointer_type (*p_name = LHS (stmt)) 
    && is_null (RHS (stmt));
}

/* -------------------------------------------------------------*
 *	 inserting statements to the connection graph		*
 * -------------------------------------------------------------*/
#if 0
static con_node
insert_object (con_graph cg, tree p_name, tree id, tree type, bool killing)
{
  con_node p, obj_node;
  gcc_assert (cg && p_name && id && type);

  p = existing_local_node (cg, p_name);

  obj_node = get_existing_node (cg, id);
  if (obj_node == NULL) obj_node = add_object_node (cg, id);
  gcc_assert (obj_node);

  if (killing)
    {
      /* do a bypass */
    }

  /* add an edge */
  if (get_edge (p, obj_node) == NULL)
    add_edge (p, obj_node);

  p->next_link = obj_node;
  return p;
}
#endif

#if 0
static con_node
insert_reference (con_graph cg, tree p_name, tree q_name, bool killing)
{
  con_node p, q;
  gcc_assert (cg && p_name && q_name);

  p = existing_local_node (cg, p_name);
  q = existing_local_node (cg, q_name);

  if (killing)
    {
      /* do a bypass */
    }

  /* add an edge */
  if (get_edge (p, q) == NULL)
    add_edge (p, q);

  p->next_link = q;
  return p;
}
#endif

#if 0
static con_node
insert_assignment_from_field (con_graph cg, tree p_name, tree q_name, tree f_name, tree stmt_id, tree type, bool killing)
{
  con_node p, q, v;
  con_node_vec u;
  int num_u;

  gcc_assert (cg && p_name && q_name && f_name && stmt_id && type);

  p = existing_local_node (cg, p_name);
  q = existing_local_node (cg, q_name);

  u = get_points_to_and_terminals (cg, q, stmt_id);
  num_u = VEC_length (con_node, u); /* get_field_nodes delinks u */

  /* get the field nodes of the pointed-to objects */
  v = get_field_nodes (u, f_name);
  gcc_assert (num_u == link_length (v));

  /* we need to do bypassing befor we add the extra edges */
  if (killing)
    {
      /* TODO: bypassing */
    }


  /* we need to return v, so keep track of the first in the list */
  q->next_link = v;

  /* add deferred edges representing the assignment p = q.f */
  while (v)
    {
      if (get_edge (p, v) == NULL)
	add_edge (p, v);

      NEXT_LINK (v); /* dont break the links, we're going to return it */
    }

  /* link together p, q, and all f */
  p->next_link = q;
  /* q is already linked to v */
  /* the 'v' are linked together */
  return p;
}
#endif


#if 0
static con_node
insert_return (con_graph cg, tree return_name, tree r_name)
{
  con_node r, return_node;
  gcc_assert (cg);
  gcc_assert (r_name);
  gcc_assert (return_name);

  r = existing_local_node (cg, r_name);
  r->escape = EA_ARG_ESCAPE;

  return_node = cg->return_node;
  if (return_node == NULL) return_node = add_return_node (cg,
							  return_name);
  gcc_assert (return_node->id == return_name);
  gcc_assert (cg->return_node);

  if (get_edge (return_node, r) == NULL)
    add_edge (return_node, r);

  return r;
}
#endif


#if 0/* returns each actual and argument node */
static con_node
insert_indirect_function_call (con_graph cg, tree func_var, tree argument_list)
{
  con_node first_link = NULL;
  con_node last_link = NULL;

  gcc_assert (cg && func_var && argument_list);

  /* We make all of these nodes GLOBAL_ESCAPE. Since we dont have the
   * function which is called on them, we can't rule out the possability
   * that each of these is assigned to a global variable, and thus escape
   * globally. If we could reduce polymorphism, or perhaps make a
   * "closed-world" assumption, we'd be happy. */

  /* TODO simplify this to creating an edge from func_var to each argument
   * node */
  func_var = 0;
  for ( ; argument_list; argument_list = TREE_CHAIN (argument_list))
    {
      tree argument = TREE_VALUE (argument_list);
      if (has_pointer_type (argument))
	{
	  con_node argument_node = existing_local_node (cg, argument);
	  set_escape_state (argument_node, EA_GLOBAL_ESCAPE);

	  if (first_link == NULL)
	    first_link = argument_node;
	  else
	    last_link->next_link = argument_node;
	  last_link = argument_node;
	}
    }

  return first_link;
}
#endif

static void
update_caller_nodes (con_graph caller, con_graph callee, tree call_id)
{
  int i, j, k;
  tree parameters = DECL_ARGUMENTS (callee->function);
  tree arguments = RHS (RHS (call_id)); /* These are in the caller */
  con_node f, node;

  /* Update by followign the parameters */
  while (parameters)
    {
      gcc_assert (arguments);
      tree argument = TREE_VALUE (arguments);
      if (has_pointer_type (parameters))
	{
	  gcc_assert (has_pointer_type (argument));
	  /* a0_ (actual) mapsTo a0 (argument) except this argument is in the
	   * callee */

	  con_node caller_node = get_existing_node (caller, argument);
	  caller_node->type = CALLER_ACTUAL_NODE;
	  con_node callee_node = get_existing_node (callee, parameters);
	  gcc_assert (caller_node && callee_node);

	  update_nodes (callee_node, caller_node, call_id);
	}
      else
	gcc_assert (!has_pointer_type (argument));

      parameters = TREE_CHAIN (parameters);
      arguments = TREE_CHAIN (arguments);
    }

  /* Update by following the return node */
  if (GIMPLE_STMT_P (call_id))
    {
      con_node caller_node = get_existing_node (caller, LHS (call_id));
      caller_node->type = CALLER_ACTUAL_NODE;
      gcc_assert (caller_node);
      con_node callee_node = callee->return_node;
      gcc_assert (callee_node);
      update_nodes (callee_node, caller_node, call_id);
    }



  /* This bit comes from section 4.4, "Updating Caller Edges": for any
   * pattern p -> f -> q, where p and q are objects, add a points to edge
   * f_bar -> q_bar, where f_bar and q_bar are in the set of mapsTo nodes
   * for f and q, respectively */
  for(f = callee->root; f; f = f->next)
    {
      if (f->type == FIELD_NODE && !VEC_empty (con_node, f->maps_to_obj))
	{
	  /*con_node p = f->in->source;*/
	  con_node_vec qs = get_points_to (f, VEC_alloc (con_node, heap,
							 10));
	  con_node q;
	  for (i = 0; VEC_iterate (con_node, qs, i, q); i++)
	    {
	      con_node f_bar;
	      for (j = 0; VEC_iterate (con_node, f->maps_to_obj, j,
				       f_bar); j++)
		{
		  con_node q_bar;
		  for (k = 0; VEC_iterate (con_node, q->maps_to_obj, k,
					   q_bar); k++)
		    {
		      if (get_edge (f_bar, q_bar) == NULL)
			add_edge (f_bar, q_bar);
		    }
		}
	    }
	}
    }
  for(node = callee->root; node; node = node->next)
    {
      if (node->maps_to_obj)
	{
	  /* TODO dealloc */
	  node->maps_to_obj = NULL;
	}
    }
}

/* returns each actual and argument node. Argument list can be null */
#if 0 /* I keep editting this by accident */
static void
insert_function_call (con_graph cg, tree function, tree argument_list, tree stmt_id)
{
  int i = 1;
  gcc_assert (cg && function && stmt_id);

  /* Check the function doesnt belong to java.lang.Object */
  if (DECL_CONTEXT (function) == object_type_node)
    {
      /* TODO not sure why - revisit */
      return;
    }

  /* Choi99 4.4 */
  /* For the call T(u0, u1, ...) each argument is modelled with the
   * assignments u0_ = u0; u1_ = u1; etc, and the function call 
   * T(u0_, u1_, ...). u0_ we call "actual nodes" and u0 "argument nodes"
   * */
  while (argument_list)
    {
      tree argument = TREE_VALUE (argument_list);

      /* model the assignments */
      if (has_pointer_type (argument))
	{
	  /* check the actual node doesnt already exist */
	  con_node actual_node, argument_node;
	  actual_node = get_caller_actual_node (cg, i, stmt_id);
	  if (actual_node == NULL)
	    {
	      actual_node = add_caller_actual_node (cg, argument, i,
						    stmt_id);
	    }
	  gcc_assert (actual_node);

	  argument_node = get_existing_node (cg, argument);

	  /* this can be an ADDR_EXPR, which means &a, so we
	   * add a, and we add &a, with an edge, if thats the
	   * case */
	  if (TREE_CODE (argument) == ADDR_EXPR)
	    {
	      tree lhs = LHS (argument);
	      con_node addressed_node = get_existing_node (cg, lhs);
	      gcc_assert (addressed_node);

	      /* in this case, the argument node probably hasnt been
	       * created */
	      if (argument_node == NULL)
		argument_node = add_local_node (cg, argument);


	      if (get_edge (argument_node, addressed_node) == NULL)
		add_edge (argument_node, addressed_node);
	    }
	  else
	    gcc_assert (argument_node);

	  if (get_edge (actual_node, argument_node) == NULL)
	    add_edge (actual_node, argument_node);

	  i++;
	}
      argument_list = TREE_CHAIN (argument_list);
    }

  /* update caller nodes for the constructors (at a minimum) */
  /* TODO do this for other function calls */
  con_graph callee_cg = get_cg_for_function (cg, function);
  if (callee_cg)
    {
      /* you must always inline the constructor */
      if (ea_interprocedural || is_function_constructor (function))
	{
	  update_caller_nodes (cg, callee_cg, stmt_id);
	}
    }
}
#endif


/* -------------------------------------------------------------*
 * extracting trees from statements and calling insertions	*
 * -------------------------------------------------------------*/
static void
do_assignment (con_node p, con_node q)
{
  gcc_assert (p && q);
  /* Is it a static field? */
  if (ea_flow_sensitive && TREE_CODE (p->id) != VAR_DECL)
    {
      if (add_killing_edge (p, q))
	{
	  fprintf (dump_file, "Killing def\n");
	  mark_uses (p->id);
	}
      else
	{
	  fprintf (dump_file, "Killing def changes nothing\n");
	}
    }
  else
    {
      if (get_edge (p, q))
	{
	  fprintf (dump_file, "Non-killing def changes nothing\n");
	}
      else
	{
	  add_edge (p, q);
	  fprintf (dump_file, "Non-killing def adds edge\n");
	  mark_uses (p->id);
	}
    }
}

#if 0
static bool
check_null_pointer_exception_stmt (con_graph cg, tree call_expr)
{
  /* _Jv_ThrowNullPointerException () */
  /* dont do anything, since nothing escapes here */
  tree addr_expr;
  if (!(TREE_CODE (call_expr) == CALL_EXPR
      && TREE_CODE (addr_expr = LHS (call_expr)) == ADDR_EXPR
      && LHS (addr_expr) == soft_nullpointer_node))
    {
      return false;
    }
  return true;
}
#endif

#if 0
static bool
check_array_allocation (con_graph cg, tree modify_stmt)
{
  /* p = _Jv_NewObjectArray (type):
   * modify_stmt
   *   var_decl - p
   *   call_expr
   *     addr_expr
   *	   func_decl - _Jv_AllocObjectNoFinalizer
   *     tree-list - int count
   *       tree-list - Class klass
   *         tree-list - initial value
   *
   */

 /* TODO 3rd parameter should be 0 */

  tree p_name, type_name;
  tree call_expr, addr_expr, func_decl;
  con_node p;
  if (!(GIMPLE_STMT_P (modify_stmt) 
	&& has_pointer_type (modify_stmt) 
	&& SSA_VAR_P (p_name = LHS (modify_stmt)) 
	&& TREE_CODE (call_expr = RHS (modify_stmt)) == CALL_EXPR
	&& TREE_CODE (addr_expr = LHS (call_expr)) == ADDR_EXPR
	&& TREE_CODE (func_decl = LHS (addr_expr)) == FUNCTION_DECL
	&& func_decl == soft_anewarray_node))
    {
      return false;
    }

  type_name = TREE_TYPE (TREE_TYPE (modify_stmt));

  p = insert_object (cg, p_name, modify_stmt, type_name, true);
  clear_links (p); /* we dont need these for anything */
  return true;
}
#endif


static bool
is_return_stmt (tree stmt, tree* result_decl, tree* q_name)
{
  return TREE_CODE (stmt) == RETURN_EXPR
    && (stmt = TREE_OPERAND (stmt, 0))
    && GIMPLE_STMT_P (stmt)
    && has_pointer_type (stmt)
    && (*result_decl = LHS (stmt))
    && (*q_name = RHS (stmt));
}

static void
do_return_stmt (con_graph cg, tree result_decl, tree q_name)
{
  con_node p = get_existing_node (cg, result_decl);
  con_node q = get_existing_node (cg, q_name);

  if (p) 
    {
      do_assignment (p, q);
    }
  else
    {
      p = add_local_node (cg, result_decl);
      add_edge (p, q);
      mark_uses (result_decl); /* this shouldnt add anything, I think */
    }

  add_edge (cg->return_node, p);
}



static bool
is_memory_allocation (tree stmt, tree* p_name)
{
  tree call_expr, addr_expr, func_decl;
  return (GIMPLE_STMT_P (stmt) 
	&& has_pointer_type (stmt) 
	&& SSA_VAR_P (*p_name = LHS (stmt)) 
	&& TREE_CODE (call_expr = RHS (stmt)) == CALL_EXPR
	&& TREE_CODE (addr_expr = LHS (call_expr)) == ADDR_EXPR
	&& TREE_CODE (func_decl = LHS (addr_expr)) == FUNCTION_DECL
	&& func_decl == alloc_no_finalizer_node);
}

static void
do_memory_allocation (con_graph cg, tree p_name, tree obj_name)
{
  con_node p = get_existing_node (cg, p_name);
  con_node obj_node = get_existing_node (cg, obj_name);

  /* They're both NULL or neither is */
  gcc_assert (!((p == NULL) ^ (obj_node == NULL)));

  if (!p)
    {
      fprintf (dump_file, "New object\n");
      p = add_local_node (cg, p_name);
      obj_node = add_object_node (cg, obj_name);
    }
  else
    gcc_unreachable (); /* i think - lets check */

  do_assignment (p, obj_node);
}

#if 0
static bool
check_indirect_goto (con_graph cg, tree modify_stmt)
{
  /* make p global */
  /* p = &*label
   * modify_stmt
   *   var_decl - p
   *   addr_expr
   *     label_decl
   */
  tree p_name, addr_expr;
  con_node p;

  if (!(GIMPLE_STMT_P (modify_stmt) 
    && has_pointer_type (modify_stmt)
    && SSA_VAR_P (p_name = LHS (modify_stmt))
    && ((TREE_CODE (addr_expr = RHS (modify_stmt)) == ADDR_EXPR) 
    && TREE_CODE (LHS (addr_expr)) == LABEL_DECL)))
    {
      return false;
    }


  p = get_existing_node (cg, p_name);
  p->escape = EA_GLOBAL_ESCAPE;

  gcc_assert (p->next_link == NULL);
  return true;
}
#endif

static bool 
is_reference_cast (tree stmt, tree* p_name, tree* q_name)
{
  tree nop_expr;
  return (GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt) 
	&& is_gimple_cast (RHS (stmt))
	&& SSA_VAR_P (*p_name = LHS (stmt)) 
	&& SSA_VAR_P (*q_name = LHS (nop_expr)));
}

static bool
is_reference_copy (tree stmt, tree* p_name, tree* q_name)
{
  return (GIMPLE_STMT_P (stmt) 
	&& has_pointer_type (stmt) 
	&& SSA_VAR_P (*p_name = LHS (stmt)) 
	&& SSA_VAR_P (*q_name = RHS (stmt)));
}

static void
do_reference_copy (con_graph cg, tree p_name, tree q_name)
{
  con_node p = get_existing_node (cg, p_name);
  con_node q = get_existing_node (cg, q_name);
  gcc_assert (q);

  if (p) return do_assignment (p, q);

  fprintf (dump_file, "New reference (copy)\n");
  p = add_local_node (cg, p_name);
  add_edge (p, q);

  mark_uses (p_name);
}

static void
do_set_to_null (tree var, con_node node)
{
  if (node->out != NULL)
    {
      mark_uses (var);
    }

  bypass_node (node);
}

static void
do_assignment_to_null (con_graph cg, tree p_name)
{
  con_node p = get_existing_node (cg, p_name);

  if (p) return do_set_to_null (p_name, p);

  fprintf (dump_file, "Assign to null");
  p = add_local_node (cg, p_name);

  mark_uses (p_name);
}

#if 0
static bool
check_return_stmt (con_graph cg, tree stmt)
{
  /* return u:
   * return_expr (void)
   *   modify_stmt
   *     result_decl - result_decl
   *     var_decl - r
   */
  tree r_name, result_decl;
  tree modify_stmt;
  con_node r;

  if (!(TREE_CODE (stmt) == RETURN_EXPR
	&& TREE_OPERAND (stmt, 0) /* dont match null cases */
	&& TREE_CODE (modify_stmt = LHS (stmt)) == MODIFY_EXPR
	&& has_pointer_type (modify_stmt)
	&& TREE_CODE (result_decl = LHS (modify_stmt)) == RESULT_DECL
	&& SSA_VAR_P (r_name = RHS (modify_stmt))))
    {
      return false;
    }

  r = insert_return (cg, result_decl, r_name);
  /* no links to clear */
  return true;
}
#endif


#if 0
static bool
check_function_call_with_return_value (con_graph cg, tree modify_stmt)
{
  /* for intraprocedural, just create node and make it ArgEsacpe */
  /* r = u.T(a,b) or
     r = T(a,b):
   *   modify_stmt
   *     var_decl - r
   *     call_expr
   *       addr_expr
   *	     func_decl - T
   *       tree-list -- arguments */

  /* any special calls have been taken care of already */
  tree r_name, func_decl, arguments = NULL;
  tree call_expr, addr_expr;
  con_node r;

  if (!(GIMPLE_STMT_P (modify_stmt) 
	&& has_pointer_type (modify_stmt) 
	&& SSA_VAR_P (r_name = LHS (modify_stmt)) 
	&& TREE_CODE (call_expr = RHS (modify_stmt)) == CALL_EXPR
	&& TREE_CODE (addr_expr = LHS (call_expr)) == ADDR_EXPR
	&& TREE_CODE (func_decl = LHS (addr_expr)) == FUNCTION_DECL))
    {
      return false;
    }

  /* check for arguments */
  if (TREE_OPERAND (call_expr, 1))
    arguments = TREE_OPERAND (call_expr, 1);

  /* we dont actually link this to anything */
  r = existing_local_node (cg, r_name);

  /* r is arg_escape since if it reaches an object, that object may now be
   * reachable by an object allocated in the function which returns r */
  r->escape = EA_ARG_ESCAPE;


/*  insert_function_call (cg, func_decl, arguments, modify_stmt);*/

  assert_all_next_link_free (cg);
  return true;
}
#endif

static bool
is_function_call (tree stmt, tree* function, tree* arguments, tree* return_value)
{
  tree a, f, r;
  tree call_expr;
  if (!GIMPLE_STMT_P (stmt))
    return false;

  if (TREE_CODE (stmt) == CALL_EXPR)
    {
      // this is a call expr with NO return value
      call_expr = stmt;
      r = NULL;
    }
  else
    {
      if (TREE_CODE (RHS (stmt)) == CALL_EXPR)
	{
	  // this is a call expr with a return value
	  call_expr = RHS (stmt);
	  r = LHS (stmt);
	}
      else if (TREE_CODE (RHS (stmt)) == WITH_SIZE_EXPR)
	{
	  /* TODO havent come across one of these */
	  gcc_unreachable ();
	}
      else
	return false;
    }

  tree addr_expr = LHS (call_expr);
  if (TREE_CODE (addr_expr) == ADDR_EXPR)
    {
      f = LHS (addr_expr);
      if (TREE_CODE (f) == FUNCTION_DECL)
	{
	  a = RHS (call_expr);
	  /* We have all we need to know. Write out the output parameters */
	  if (function) *function = f;
	  if (arguments) *arguments = a;
	  if (return_value) *return_value = r;
	  return true;
	}
    }
  return false;
}

static bool
is_special_function_call (tree stmt)
{
  tree function;
  if (is_function_call (stmt, &function, NULL, NULL))
    {
      tree identifier = DECL_NAME (function);
      if (identifier == get_identifier ("_Jv_InitClass"))
	return true;
    }
  return false;
}

#if 0
static void
model_assignment (con_graph cg, int index, tree call_id, tree arg_name)
{
  gcc_assert (call_id);
  gcc_assert (has_pointer_type (arg_name));

  /* check the actual node doesnt already exist */
  con_node actual_node, argument_node;
  actual_node = get_existing_node_with_call_id (cg, arg_name, call_id);
  if (actual_node == NULL)
    {
      actual_node = add_node_with_call_id (cg, arg_name, call_id);
      fprintf (dump_file, "Adding actual node\n");
    }
  gcc_assert (actual_node);

  argument_node = get_existing_node (cg, arg_name);

  /* this can be an ADDR_EXPR, which means &a, so we add a, and we add &a,
   * with an edge, if thats the case */
  if (TREE_CODE (arg_name) == ADDR_EXPR)
    {
      tree lhs = LHS (arg_name);
      con_node addressed_node = existing_local_node (cg, lhs);
      gcc_assert (addressed_node);

      /* in this case, the argument node probably hasnt been created */
      if (argument_node == NULL)
	argument_node = add_local_node (cg, arg_name);

      if (get_edge (argument_node, addressed_node) == NULL)
	add_edge (argument_node, addressed_node);
    }
  else
    gcc_assert (argument_node);

  if (get_edge (actual_node, argument_node) == NULL)
    add_edge (actual_node, argument_node);
}
#endif

static void
do_function_call (con_graph cg, tree function, tree arguments, tree return_value, tree call_id)
{
//  tree argument_list = arguments;
  /* TODO theres a lot of marking uses to be done here */

  /* Check the function doesnt belong to java.lang.Object */
  /* TODO move these into the special category */
  if (DECL_CONTEXT (function) == object_type_node)
    {
      /* allow <init>, see what the others do when presented with them */
      gcc_assert (DECL_NAME (function) == get_identifier ("<init>"));

      /* TODO clone and toString might represent memory allocations */

      /* java.lang.Object () does nothing */
      /* TODO is this allowed? Can a ClassLoader load a different Object?
       * */
      fprintf (dump_file, "Ignoring java.lang.Object ()\n");
      return;
    }
  if (return_value)
    {
      if (get_existing_node (cg, return_value) == NULL)
	{
	  add_local_node (cg, return_value);
	}
      mark_uses (return_value);
    }

  /* Choi99 4.4 */
  /* For the call T(u0, u1, ...) each argument is modelled with the
   * assignments u0_ = u0; u1_ = u1; etc, and the function call 
   * T(u0_, u1_, ...). u0_ we call "actual nodes" and u0 "argument nodes"
   * */
  /* There is no need to actually model any of this, since itys free in
   * the gimple */
  /*
  int i = 1;

  model_assignment (cg, 0, call_id, return_value);
  while (argument_list)
    {
      tree argument = TREE_VALUE (argument_list);

      if (has_pointer_type (argument))
	{
	  model_assignment (cg, i, call_id, argument);
	  i++;
	}
      else
	fprintf (dump_file, "Non-pointer argument: ignore\n");

      argument_list = TREE_CHAIN (argument_list);
    }
*/

  /* update caller nodes for the constructors (at a minimum) */
  /* TODO do this for other function calls */
  con_graph callee_cg = get_cg_for_function (cg, function);
  if (callee_cg)
    {
      if (ea_interprocedural || is_function_constructor (function))
	{
	  fprintf (dump_file, "Updating graph with callee graph\n");
	  update_caller_nodes (cg, callee_cg, call_id);
	}
    }
  else
    {
      fprintf (dump_file, "MISSED OPPORTUNITY: No callee graph\n");
      /* Make all the nodes global escape */
      tree argument_list = arguments;
      while (argument_list)
	{
	  tree argument = TREE_VALUE (argument_list);
	  if (has_pointer_type (argument))
	    {
	      con_node arg_node = get_existing_node (cg, argument);
	      gcc_assert (arg_node);
	      arg_node->escape = EA_GLOBAL_ESCAPE;
	    }
	  argument_list = TREE_CHAIN (argument_list);
	}
    }
}


/* TODO whatabout if functions have a return type which isnt a var_decl.
 * then they should count as no reutrn type */

#if 0
static bool
check_indirect_function_call_with_return_value (con_graph cg, tree modify_stmt)
{
   /*  r = T(a,b):
   *   modify_stmt
   *     var_decl - r
   *     call_expr
   *       var_decl - T
   *       tree-list -- arguments */
  tree r_name, T, arguments;
  tree call_expr;
  con_node arg_nodes, r;

  if (!(GIMPLE_STMT_P (modify_stmt)
	&& has_pointer_type (modify_stmt) 
	&& SSA_VAR_P (r_name = LHS (modify_stmt))
	&& TREE_CODE (call_expr = RHS (modify_stmt)) == CALL_EXPR
	&& SSA_VAR_P (T = LHS (call_expr))
	&& TREE_CODE (arguments = RHS (call_expr)) == TREE_LIST))
    {
      return false;
    }

  /* we dont actually link this to anything */
  r = existing_local_node (cg, r_name);

  /* r is arg_escape since if it reaches an object, that object may now be
   * reachable by an object allocated in the function which returns r */
  r->escape = EA_ARG_ESCAPE;


  arg_nodes = insert_indirect_function_call (cg, T, arguments);
  if (arg_nodes)
    clear_links (arg_nodes); /* we dont need these for anything */
  return true;
}
#endif

static bool
is_field_ref (tree stmt, tree* ref_name, tree* f_name)
{
  tree indirect_ref;
  return (TREE_CODE (stmt) == COMPONENT_REF
	  && (indirect_ref = LHS (stmt))
	  && TREE_CODE (indirect_ref) == INDIRECT_REF
	  && (*ref_name = LHS (indirect_ref))
	  && SSA_VAR_P (*ref_name)
	  && (*f_name = RHS (stmt))
	  && (TREE_CODE (*f_name) == FIELD_DECL));
}

#if 0
static bool
check_assignment_from_data_array_stmt (con_graph cg, tree modify_stmt)
{
  /* this is a mix of assignment from field, and assignment from array */
  /* p = q.data[x]
   * modify_stmt
   *   var_decl - p
   *   array_ref
   *     component_ref
   *	   indirect_ref
   *	     var_decl - q
   *	   field_decl - f (aka data)
   *	 integer_cst - x
   */
  tree p_name, q_name, f_name, array_ref;
  con_node p;
  if (!(GIMPLE_STMT_P (modify_stmt)
	&& has_pointer_type (modify_stmt)
	&& SSA_VAR_P (p_name = LHS (modify_stmt))
	&& TREE_CODE (array_ref = RHS (modify_stmt)) == ARRAY_REF
	&& is_field_ref (array_ref, &q_name, &f_name)
	&& SSA_VAR_P (q_name)
	&& DECL_NAME (f_name) == get_identifier ("data")))
    {
      return false;
    }

  gcc_unreachable (); /* TODO */
/*  p = insert_assignment_from_field (cg, p_name, q_name, f_name, modify_stmt,
				    TREE_TYPE (indirect_ref), true);*/

  clear_links (p);
  return true;
}
#endif

#if 0
static bool
is_assignment_to_exception (tree stmt)
{
  return (GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt)
	&& TREE_CODE (LHS (stmt)) == EXC_PTR_EXPR
	&& SSA_VAR_P (RHS (stmt)));
}

static void
do_assignment_to_exception_object (con_graph cg, tree stmt)
{
  /* TODO this isnt done yet - re-enable B for this */
  tree p_name = LHS (stmt);
  tree q_name = RHS (stmt);

  con_node q = get_existing_node (cg, q_name);
  gcc_assert (q);

  /* if it doesnt exist, create it */
  con_node p = existing_local_node (cg, p_name);
  p->escape = EA_GLOBAL_ESCAPE;
  do_assignment (p, q);
}
#endif

#if 0
static bool
check_assignment_from_exception_object (con_graph cg, tree modify_stmt)
{
  /* p = exception:
   * modify_stmt
   *   var_decl - p
   *   exc_ptr_expr - q */
  tree p_name, q_name;
  con_node p, q;
  if (!(GIMPLE_STMT_P (modify_stmt)
	&& has_pointer_type (modify_stmt)
	&& SSA_VAR_P (p_name = LHS (modify_stmt))
	&& TREE_CODE (q_name = RHS (modify_stmt)) == EXC_PTR_EXPR))
    {
      return false;
    }

  p = insert_reference (cg, p_name, q_name, false);
  q = p->next_link;
  q->escape = EA_GLOBAL_ESCAPE;
  clear_links (p);
  return true;
}
#endif

#if 0
static bool
check_assignment_from_array_stmt (con_graph cg, tree modify_stmt)
{
  /* deal with this as if it were p = q; (except it's non-killing)
   * here */

  /* p = q[x]: 
   * modify_stmt
   *   var_decl - p
   *   array_ref
   *     var_decl - q
   *     integer_cst - x*/

  tree p_name, q_name, array_ref;
  con_node p;

  if (!(GIMPLE_STMT_P (modify_stmt)
    && has_pointer_type (modify_stmt)
    && SSA_VAR_P (p_name = LHS (modify_stmt))
    && TREE_CODE (array_ref = RHS (modify_stmt)) == ARRAY_REF
    && SSA_VAR_P (q_name = LHS (array_ref))
    && TREE_CODE (RHS (array_ref)) == INTEGER_CST))
    {
      return false;
    }

  p = insert_reference (cg, p_name, q_name, false);
  clear_links (p); /* we dont need these for anything */
  return true;

}
#endif

#if 0
static bool
check_assignment_to_data_array_stmt (con_graph cg, tree modify_stmt)
{
  /* this is a mix of assignment to field, and assignment to array */
  /* p.data[x] = q:
   * modify_stmt
   *   array_ref
   *     component_ref
   *       indirect_ref
   *         var_decl - p
   *       field_decl - f (aka data)
   *     integer_cst - x
   *   var_decl - q
   */
  tree p_name, q_name, f_name;
  tree array_ref, component_ref, indirect_ref;
  con_node p;
  if (!(GIMPLE_STMT_P (modify_stmt)
	&& has_pointer_type (modify_stmt)
	&& TREE_CODE (array_ref = LHS (modify_stmt)) == ARRAY_REF
	&& TREE_CODE (component_ref = LHS (array_ref)) == COMPONENT_REF
	&& TREE_CODE (indirect_ref = LHS (component_ref)) == INDIRECT_REF
	&& SSA_VAR_P (p_name = LHS (indirect_ref))
	&& TREE_CODE (f_name = RHS (component_ref)) == FIELD_DECL
	&& DECL_NAME (f_name) == get_identifier ("data")
	&& SSA_VAR_P (q_name = RHS (modify_stmt))))
    {
      return false;
    }

  gcc_unreachable ();
  /* TODO re-enable below when we've switched to it */
/*  p = insert_assignment_to_field (cg, p_name, q_name, f_name, modify_stmt,
				  TREE_TYPE (indirect_ref), false);*/
  clear_links (p);
  return true;
}
#endif

static bool
is_assignment_to_field (tree stmt, tree* p_name, tree* f_name, tree* q_name)
{
  /* TODO allow NULL on the rhs */
  return (GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt)
	&& is_field_ref (LHS (stmt), p_name, f_name)
	&& DECL_NAME (*f_name) != get_identifier ("vtable")
	&& SSA_VAR_P (*q_name = RHS (stmt)));
}


static void
do_assignment_to_field (con_graph cg, tree p_name, tree f_name, tree q_name, tree phantom_id)
{
  /* p.f = q; */
  fprintf (dump_file, "New reference (assign to field)\n");

  con_node p = get_existing_node (cg, p_name);
  con_node q = get_existing_node (cg, q_name);
  gcc_assert (p);
  gcc_assert (q);

  con_node_vec u = get_points_to_and_terminals (cg, p, phantom_id);

  /* get the field nodes of the pointed-to objects */
  int num_u = VEC_length (con_node, u); /* get_field_nodes delinks u */
  con_node v = get_field_nodes (u, f_name);
  gcc_assert (num_u == link_length (v));
 
  /* The reason we cant bypass here is that an abstract object (one is our
   * graph) can represent more than one concrete object (one on the heap).
   * Example would be allocation in a loop. So if you over-write a field
   * in an abstract object, then you can't over-write the fields in all
   * the concrete objects, just the latest one */
  /* This means there should be scope for an object to know whether or not
   * it represents a concrete object or an abstract one. If it's concrete,
   * feel free to bypass. The problem then becomes that SSA isnt quite
   * good enough to model such uses, which is a more fundamental problem.
   * */

  /* DONT BYPASS */
  
  /* add deferred edges */
  bool edge_added = false;
  while (v)
    {
      if (get_edge (v, q) == NULL)
	{
	  edge_added = true;
	  add_edge (v, q);
	}

      NEXT_LINK_CLEAR (v);
    }

  if (edge_added)
    mark_uses (p_name);
}

/* Virtual functions which havent been devirtualized look like this:
 * This function checks for the vtable, and protects the rest of "update"
 * from it, keeping state globally. If it doesnt see what it expects next,
 * then an assertion is thrown. It handles the multi-function dispatch as
 * well */
static bool
stmt_in_virtual_function (tree stmt)
{
  tree f_name, plus_expr;
  tree obj_type_ref, q_name;
  tree indirect_ref;

  /* check for vtable: a1 = a0->vtable; */
  if ((GIMPLE_STMT_P (stmt)
       && has_pointer_type (stmt)
       && SSA_VAR_P (LHS (stmt))
       && is_field_ref (RHS (stmt), &q_name, &f_name)
       && SSA_VAR_P (q_name)
       && DECL_NAME (f_name) == get_identifier ("vtable")))
    {
      fprintf (dump_file, "vtable\n");
      mark_uses (LHS (stmt));
      return true;
    }

  /* Check for the vtable offset: a2 = a1 + xB; */
  if (GIMPLE_STMT_P (stmt) 
      && has_pointer_type (stmt)
      && SSA_VAR_P (LHS (stmt))
      && ((TREE_CODE (plus_expr = RHS (stmt)) == PLUS_EXPR) 
	  || (TREE_CODE (plus_expr) == MINUS_EXPR))
      && SSA_VAR_P (LHS (plus_expr)) 
      && TREE_CODE (RHS (plus_expr)) == INTEGER_CST)
    {
      fprintf (dump_file, "vtable offset\n");
      mark_uses (LHS (stmt));
      return true;
    }

  /* Check for the vtable dereference: a3 = *a2; */
  if (GIMPLE_STMT_P (stmt) 
      && has_pointer_type (stmt) 
      && SSA_VAR_P (LHS (stmt))
      && TREE_CODE (indirect_ref = RHS (stmt)) == INDIRECT_REF
      && SSA_VAR_P (LHS (indirect_ref)))
    {
      fprintf (dump_file, "vtable offset dereference\n");
      mark_uses (LHS (stmt));
      return true;
    }

  /* The object_type_reference: 
   * a4 = (OBJ_TYPE_REF (a3, this_8->function_name)); */
  if (GIMPLE_STMT_P (stmt) 
      && has_pointer_type (stmt)
      && SSA_VAR_P (LHS (stmt))
      && TREE_CODE (obj_type_ref = RHS (stmt)) == OBJ_TYPE_REF)
    {
      mark_uses (LHS (stmt));
      fprintf (dump_file, "virtual function call\n");
      return true;
    }

  /* A bog-standard cast - it'll be handled elsewhere:
   * a5 = (TYPE:: *) a4; */


  /* Indirect function call:
   * a5 (this_8, arg0, arg1); */ 
  if (TREE_CODE (stmt) == CALL_EXPR
      && SSA_VAR_P (LHS (stmt))
      && TREE_CODE (RHS (stmt)) == TREE_LIST)
    {
      fprintf (dump_file, "indirect virtual function call\n");
      return true;
    }
  return false;
}

static bool
is_assignment_from_field (tree stmt, tree* p_name, tree* q_name, tree* f_name)
{
  tree component_ref, indirect_ref;
  return (GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt)
	&& SSA_VAR_P (*p_name = LHS (stmt))
	&& TREE_CODE (component_ref = RHS (stmt)) == COMPONENT_REF
	&& TREE_CODE (indirect_ref = LHS (component_ref)) == INDIRECT_REF
	&& SSA_VAR_P (*q_name = LHS (indirect_ref))
	&& TREE_CODE (*f_name = RHS (component_ref)) == FIELD_DECL
	&& DECL_NAME (*f_name) != get_identifier ("vtable"));
}

/* Returns TRUE if there is an edge from P to every field node in V (which
 * is a linked list. */
static bool
all_edges (con_node p, con_node v)
{
  gcc_assert (p->type == LOCAL_NODE);
  gcc_assert (v->type == FIELD_NODE);
  while (v)
    {
      if (get_edge (p, v) == NULL)
	return false;

      NEXT_LINK (v);
    }
  return true;
}


static void
do_assignment_from_field (con_graph cg, tree p_name, tree q_name, tree f_name, tree phantom_id)
{
  fprintf (dump_file, "New reference (assign from field)\n");
  con_node p = get_existing_node (cg, p_name);
  con_node q = get_existing_node (cg, q_name);
  gcc_assert (q);

  con_node_vec u = get_points_to_and_terminals (cg, q, phantom_id);
  int num_u = VEC_length (con_node, u); /* get_field_nodes delinks u */
  gcc_assert (num_u > 0);

  /* get the field nodes of the pointed-to objects */
  con_node v = get_field_nodes (u, f_name);
  gcc_assert (num_u == link_length (v));
  gcc_assert (v);

  /* End early if no edges are missing */
  if (ea_flow_sensitive && p && !all_edges (p, v))
    {
      while (v) NEXT_LINK_CLEAR (v);
      return;
    }

  if (!p)
    p = add_local_node (cg, p_name);
  else
    bypass_node (p);

  /* add deferred edges representing the assignment p = q.f */
  while (v)
    {
      if (get_edge (p, v) == NULL)
	add_edge (p, v);

      NEXT_LINK_CLEAR (v);
    }

  mark_uses (p_name);
}

/* -------------------------------------------------------------*
 *		 code generation				*
 * -------------------------------------------------------------*/

#if 0  
static void
replace_array_with_alloca (con_node node)
{
  if (node == NULL) /* just use the node so it doesnt warn */
    {
      gcc_unreachable ();
    }
  return;
  tree type, alloca_func_call, new_alloca_stmt;
  tree new_jv_func_call_stmt;

  tree stmt, call_expr, addr_expr, func_decl, lhs, parms, klass;
  tree alloca_func, args;
  tree cast_int_stmt, cast_int_obj, nop_expr, cast_size_stmt, convert_expr, required_int;

  tree array_type, array_ptr_type, array_element_type, obj, size_obj, indirect_ref, data_field;
  tree component_ref, array_ref, int0, int1, int2, rhs;
  tree elements_stmt, minused, minus_stmt, four;
  tree mult_expr, total_size, plus_expr, total_stmt, cast_size_obj, count_size, mult_stmt, minus_expr;

  block_stmt_iterator bsi;

  stmt = node->id;
  bsi = bsi_for_stmt (stmt);

  /* generated code (we have elementClass, count)
   *   jobjectArray obj = NULL;
   *   size_t size = (size_t) elements (obj)
   *     size_t size = &obj->data[0]
   *   size += count * sizeof (jobject)
   *   _Jv_InitNewObjectArray (size, elementClass)
   */
  gcc_assert (is_memory_allocation (stmt));

  /* get the relevent parts of the trees */
  lhs = LHS (stmt);
  call_expr = RHS (stmt);
  addr_expr = TREE_OPERAND (call_expr, 0);
  parms = TREE_OPERAND (call_expr, 1);
  klass = TREE_VALUE (TREE_CHAIN (parms));
  func_decl = TREE_OPERAND (addr_expr, 0);

  /* TODO move some of these to static variables and work them out
   * in initialization */

  /* build the replacement statements */
  alloca_func = built_in_decls[BUILT_IN_ALLOCA];


  /* jobjectArray obj = NULL; */
  array_type = TREE_TYPE (lhs);/* type of the array */
  array_element_type = TREE_TYPE (array_type);/* type of array elements */
  array_type = build_array_type (array_element_type, one_elt_array_domain_type);
  array_ptr_type = build_pointer_type (array_element_type);
  obj = create_tmp_var (TREE_TYPE (lhs), "D");
  DECL_ARTIFICIAL (obj) = 1;

  /* size_t size = (size_t)obj->data) */
  size_obj = create_tmp_var (array_ptr_type, "D");
  DECL_ARTIFICIAL (size_obj) = 1;

  indirect_ref = build1 (INDIRECT_REF, array_element_type, obj);
  data_field = lookup_field (&array_element_type, get_identifier ("data"));
  component_ref = build3 (COMPONENT_REF, array_type, indirect_ref, data_field, NULL_TREE);

  /* make the array ref */
  int0 = build_int_cst (NULL_TREE, 0);
  int1 = build_int_cst (NULL_TREE, 0);
  int2 = build_int_cst (NULL_TREE, 1);
  array_ref = build4 (ARRAY_REF, array_element_type, component_ref, int0, int1, int2);

  rhs = build1 (ADDR_EXPR, array_ptr_type, array_ref);
  elements_stmt = build2 (MODIFY_EXPR, array_ptr_type, size_obj, rhs);

  /* int_size = (int) size; */
  cast_int_obj = create_tmp_var (int_type_node, "D");
  DECL_ARTIFICIAL (cast_int_obj) = 1;
  convert_expr = build1 (CONVERT_EXPR, int_type_node, size_obj);
  cast_int_stmt = build2 (MODIFY_EXPR, int_type_node, cast_int_obj,
		      convert_expr);

  /* cast_size = (size_t) int_size; */
  /* a size_type_node ~= size_t */
  cast_size_obj = create_tmp_var (size_type_node, "D");
  DECL_ARTIFICIAL (cast_size_obj) = 1;
  nop_expr = build1 (NOP_EXPR, size_type_node, cast_int_obj);
  cast_size_stmt = build2 (MODIFY_EXPR, size_type_node, cast_size_obj,
			   nop_expr);


  /* minused = 2147483646 - cast_size */ 
  minused = create_tmp_var (unsigned_int_type_node, "D");
  DECL_ARTIFICIAL (minused) = 1;
  required_int = build_int_cstu (unsigned_int_type_node, 2147483646);
  minus_expr = build2 (MINUS_EXPR, unsigned_int_type_node, required_int,
		       cast_size_obj);
  minus_stmt = build2 (MODIFY_EXPR, unsigned_int_type_node, minused,
		       minus_expr);


  /* count_size = count * sizeof (jobject) */
  count_size = create_tmp_var (unsigned_int_type_node, "D");
  DECL_ARTIFICIAL (count_size) = 1;
  four = build_int_cst (NULL_TREE, 4);
  /* TODO find real size - where does 4 come from */
  mult_expr = build2 (MULT_EXPR, unsigned_int_type_node, cast_size_obj,
		      four);
  mult_stmt = build2 (MODIFY_EXPR, unsigned_int_type_node, count_size,
		      mult_expr);

  /* size += count_size */
  total_size = create_tmp_var (size_type_node, "D");
  DECL_ARTIFICIAL (total_size) = 1;
  plus_expr = build2 (PLUS_EXPR, unsigned_int_type_node, count_size,
		      minused);
  total_stmt = build2 (MODIFY_EXPR, size_type_node, total_size,
		       plus_expr);

  /* allocate the memory */
  args = tree_cons (NULL_TREE, total_size, NULL_TREE);

  type = TREE_TYPE (TREE_TYPE (alloca_func));
  alloca_func_call = build_function_call_expr (alloca_func, args);
  new_alloca_stmt = build2 (MODIFY_EXPR, type, lhs, alloca_func_call);



  /* _Jv_InitNewObjectArray (size, elementClass, object) */
  args = tree_cons (NULL_TREE, total_size, 
		    tree_cons (NULL_TREE, klass, 
			       tree_cons (NULL_TREE, lhs, NULL_TREE)));

  new_jv_func_call_stmt = build_function_call_expr
    (init_new_array_node, args);


  /* put the statements in place */
  bsi_insert_before (&bsi, elements_stmt, BSI_SAME_STMT);
  bsi_insert_before (&bsi, cast_int_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, cast_size_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, minus_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, mult_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, total_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, new_alloca_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, new_jv_func_call_stmt, BSI_SAME_STMT); 
  bsi_remove (&bsi, true);

}

#endif

#if 0
static void
replace_with_alloca (con_node node, tree function)
{
  tree stmt, call_expr, addr_expr;
  gcc_assert (node);

  stmt = node->id;
  call_expr = RHS (stmt);
  addr_expr = LHS (call_expr);
  TREE_OPERAND (addr_expr, 0) = function;
}

static void
replace_object_with_alloca (con_node node)
{
  replace_with_alloca (node, init_no_finalizer_node);
}

static void
replace_array_with_alloca (con_node node)
{
  replace_with_alloca (node, init_anewarray_node);
}

static void
replace_multi_array_with_alloca (con_node node)
{
  replace_with_alloca (node, init_multianewarray_node);
}

static void
replace_prim_array_with_alloca (con_node node)
{
  replace_with_alloca (node, init_newarray_node);
}
#endif


#if 0
static void
replace_with_alloca (con_node node)
{
  tree size_identifier, size_field, new_size_stmt;
  tree type, alloca_func_call, new_alloca_stmt;
  tree new_jv_func_call_stmt;

  tree stmt, call_expr, addr_expr, func_decl, lhs, parms, klass;
  tree alloca_func, args, parm1, parm2;

  block_stmt_iterator bsi;

  gcc_assert (node);

  stmt = node->id;
  bsi = bsi_for_stmt (stmt);


  /* get the relevent parts of the trees */
  lhs = LHS (stmt);
  call_expr = RHS (stmt);
  addr_expr = TREE_OPERAND (call_expr, 0);
  parms = TREE_OPERAND (call_expr, 1);
  klass = TREE_VALUE (parms);
  func_decl = TREE_OPERAND (addr_expr, 0);

  /* TODO move some of these to static variables and work them out
   * in initialization */

  /* build the replacement statements */
  alloca_func = built_in_decls[BUILT_IN_ALLOCA];


  /* tmp = klass->size */
  parm1 = create_tmp_var (integer_type_node, "D");
  DECL_ARTIFICIAL (parm1) = 1;

  size_identifier = get_identifier ("size_in_bytes");
  size_field = lookup_field (&class_type_node, size_identifier);
  parm2 = build3 (COMPONENT_REF, integer_type_node, LHS (klass),
		  size_field, NULL_TREE);

  new_size_stmt = build2 (MODIFY_EXPR, integer_type_node, parm1, parm2);


  /* obj = alloca (tmp) */
  args = tree_cons (NULL_TREE, parm1, NULL_TREE);

  type = TREE_TYPE (TREE_TYPE (alloca_func));
  alloca_func_call = build_function_call_expr (alloca_func, args);
  new_alloca_stmt = build2 (MODIFY_EXPR, type, lhs, alloca_func_call);


  /* _Jv_InitObjectNoFinalizer (klass, obj); */
  args = tree_cons (NULL_TREE, klass, tree_cons (NULL_TREE, lhs,
						 NULL_TREE));

  new_jv_func_call_stmt = build_function_call_expr
    (init_no_finalizer_node, args);

  /* put the statements in place */
  bsi_insert_before (&bsi, new_size_stmt, BSI_SAME_STMT);
  bsi_insert_before (&bsi, new_alloca_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, new_jv_func_call_stmt, BSI_SAME_STMT);
  bsi_remove (&bsi, true);
}

#endif

/* Choi 99, Section 4.1 "Connection Graph at Method Entry" */
/* Adds nodes corresponding to the VAR_DECLS for the parameters and the
 * return value */
static void 
add_actual_parameters (con_graph cg, tree function)
{
  gcc_assert (cg && function);

  /* Add an edge from each VAR_DECL to its definition, which is available
   * from the gimple. */
  tree arg_list = DECL_ARGUMENTS (function);
  while (arg_list)
    {
      if (has_pointer_type (arg_list))
	{
	  tree in_ssa = gimple_default_def (cfun, arg_list);
	  if (in_ssa)
	    {
	      con_node actual_node = add_callee_parameter (cg, arg_list);
	      con_node arg_node = add_local_node (cg, in_ssa);
	      add_edge (arg_node, actual_node);
	    }
	}

      arg_list = TREE_CHAIN (arg_list);
    }

  /* Add a node for the return value */
  tree result = DECL_RESULT (cg->function);
  if (has_pointer_type (result))
    add_result_node (cg, result);



}

/* TODO when doing more than the constructor, you need to pass the reutnr
 * node aswell */


/* -------------------------------------------------------------*
 *		 processing functions				*
 * -------------------------------------------------------------*/

/* Creates an edge from the result to each argument of PHI, if PHI is a
 * reference. */
static void
update_connection_graph_from_phi (con_graph cg, tree phi)
{
  int i;
  tree result = PHI_RESULT (phi);
  if (!has_pointer_type (result)) 
    {
      fprintf (dump_file, "Ignore\n");
      return;
    }

  gcc_assert (SSA_VAR_P (result));

  bool changed = false;

  /* check it's correct. If not, bypass and recreate */
  con_node p = get_existing_node (cg, result);

  if (!p) 
    {
      fprintf (dump_file, "New result\n");
      changed = true;
    }
  else
    {
      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	{
	  tree arg = PHI_ARG_DEF (phi, i);
	  gcc_assert (SSA_VAR_P (arg));
	  con_node q = get_existing_node (cg, arg);
	  if (!q) /* a ref which hasnt been created yet */
	    {
	      fprintf (dump_file, "New argument\n");
	      changed = true;
	      break;
	    }
	  else
	    {
	      if (!get_edge (p, q)) 
		{
		  /* this mustnt have been here at the last iteration */
		  fprintf (dump_file, "New edge\n");
		  changed = true;
		  break;
		}
	    }
	}
    }
  if (!changed)
    {
      nl ();
      return;
    }

  /* Its changed, so bypass it, and add the edges */
  if (p) bypass_node (p);
  else p = add_local_node (cg, result);
  mark_uses (result);

  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      tree arg = PHI_ARG_DEF (phi, i);
      tf (arg, false);
      gcc_assert (SSA_VAR_P (arg));
      con_node q = get_existing_node (cg, arg);
      if (!q) /* a ref which hasnt been created yet */
	{
	  fprintf (dump_file, "Ignore edge\n");
	}
      else
	{
	  gcc_assert (!get_edge (p, q));
	  add_edge (p, q);
	  fprintf (dump_file, "New edge\n");
	}
    }
  nl (); nl ();
}

static void
update_connection_graph_from_statement (con_graph cg, tree stmt)
{
  gcc_assert (cg);
  gcc_assert (stmt);

  tree p_name, q_name, f_name;
  tree function, arguments, return_value;

  /* recognize all the statement types, and update the connection
   * graph accordingly */
  if (stmt_in_virtual_function (stmt))
    return;

  else if (is_special_function_call (stmt))
    return;

  else if (is_memory_allocation (stmt, &p_name))
      do_memory_allocation (cg, p_name, stmt);

  else if (is_reference_cast (stmt, &p_name, &q_name)
	   || is_reference_copy (stmt, &p_name, &q_name))
      do_reference_copy (cg, p_name, q_name);

  else if (is_assignment_from_field (stmt, &p_name, &q_name, &f_name))
      do_assignment_from_field (cg, p_name, q_name, f_name, stmt);

  else if (is_assignment_to_field (stmt, &p_name, &f_name, &q_name))
      do_assignment_to_field (cg, p_name, f_name, q_name, stmt);

  else if (is_assignment_to_null (stmt, &p_name))
      do_assignment_to_null (cg, p_name);

  else if (is_function_call (stmt, &function, &arguments, &return_value))
      do_function_call (cg, function, arguments, return_value, stmt);

  else if (is_return_stmt (stmt, &return_value, &q_name))
    do_return_stmt (cg, return_value, q_name);

  else if (TREE_CODE (stmt) == LABEL_EXPR 
	   || TREE_CODE (stmt) == COND_EXPR
	   || !has_pointer_type (stmt))
      fprintf (dump_file, "Ignore\n");

  else
    {
      fprintf (dump_file, "Still to be dealt with\n");
      con_graph_dump (cg);
      gcc_unreachable ();
      assert_all_next_link_free (cg);
    }

  return;
#if 0
  else if (check_array_allocation (cg, stmt)) { /* NOP */ }

  /* reference copies */
  else if (check_cast_stmt (cg, stmt)) { /* NOP */ }
  else if (check_reference_copy_stmt (cg, stmt)) { /* NOP */ }
  else if (check_return_stmt (cg, stmt)) { /* NOP */ }
  else if (check_pointer_dereference_stmt (cg, stmt)) { /* NOP */ }
  else if (is_assignment_to_null (stmt))
    {
      /* this comes in array, field, and reference copy varieties, so need
       * to work it into the framework */
      /* TODO this becomes relevent in flow-sensitive */
      /* TODO this can be any of the forms:
       *   this->org = oB;
       *   p = 0B;
       *   p[x] = 0B;
       *   p->data[x] = 0B; */
      
      set_statement_type (cg, stmt, IGNORED_ASSIGNMENT_TO_NULL);

      /* Done ASSIGNMENT TO NULL */
    }


  /* exceptions - put these before function calls */
  else if (check_null_pointer_exception_stmt (cg, stmt)) { /* NOP */ }
  else if (check_assignment_from_exception_object (cg, stmt)) { /* NOP */ }
  else if (check_assignment_to_exception_object (cg, stmt)) { /* NOP */ }


  /* function calls - direct and indirect */
  else if (check_indirect_function_call (cg, stmt)) { /* NOP */ }
  else if (check_indirect_function_call_with_return_value (cg, stmt)) { }

  else if (check_function_call_with_return_value (cg, stmt)) { /* NOP */ }
  else if (check_function_call (cg, stmt)) { /* NOP */ }

    /* fields */
  /* this must come before assignment_to_field, or it wont hit */
  else if (check_vtable_stmt (cg, stmt)) { /* NOP */ }
  else if (check_assignment_to_field_stmt (cg, stmt)) { /* NOP */ }
  else if (check_assignment_from_field_stmt (cg, stmt)) { /* NOP */ }

  /* arrays */
  else if (check_assignment_from_array_stmt (cg, stmt)) { /* NOP */ }
  else if (check_assignment_from_data_array_stmt (cg, stmt)) { /* NOP */ }
  else if (check_assignment_to_data_array_stmt (cg, stmt)) { /* NOP */ }

  /* awfulness */
  else if (check_pointer_arithmetic_stmt (cg, stmt)) { /* NOP */ }
  else if (check_indirect_goto (cg, stmt)) { /* NOP */ }

  /* ignored */
  else
    {
      if (TREE_CODE (stmt) == COND_EXPR)
	set_statement_type (cg, stmt, IGNORED_COND_EXPR);

      else if (TREE_CODE (stmt) == LABEL_EXPR)
	set_statement_type (cg, stmt, IGNORED_LABEL_EXPR);

      else if (! has_pointer_type (stmt))
	set_statement_type (cg, stmt, IGNORED_NOT_A_POINTER);

      else if (TREE_CODE (stmt) == RETURN_EXPR && VOID_TYPE_P (TREE_TYPE
							       (stmt)))
	set_statement_type (cg, stmt, IGNORED_RETURNING_VOID);

      else if (TREE_CODE (TREE_TYPE (TREE_TYPE (stmt))) == FUNCTION_TYPE)
	set_statement_type (cg, stmt, IGNORED_FUNCTION_POINTER);

      if (get_statement_type (cg, stmt) == 0)
	{
	  set_statement_type (cg, stmt, IGNORED_UNKNOWN);
	  gcc_unreachable ();	/* as if */
	}

      return SSA_PROP_NOT_INTERESTING;

    }
#endif

}

/* If bb_in_worklist[i] is true, basic block I is currently in the
 * worklist */
static bool* bb_in_worklist;

static void
setup_basic_blocks (void)
{
  basic_block bb;
  FOR_EACH_BB (bb)
    {
      block_stmt_iterator bsi;
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);
	  require_visit (stmt);
	}
      tree phi;
      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	{
	  require_visit (phi);
	}
    }

  bb_in_worklist = XCNEWVEC (bool, n_basic_blocks);
}

static void
add_to_work_list (basic_block bb)
{
  if (!bb_in_worklist [bb->index])
    {
      VEC_safe_push (basic_block, heap, ea_worklist, bb);
      bb_in_worklist [bb->index] = true;
      fprintf (dump_file, "Adding block %d to worklist\n", bb->index);
    }
  else
    {
      fprintf (dump_file, "Block %d already in worklist\n", bb->index);
    }
}

static basic_block
fetch_from_worklist (void)
{
  basic_block bb = VEC_ordered_remove (basic_block, ea_worklist, 0);
  bb_in_worklist [bb->index] = false;
  fprintf (dump_file, "Fetching block %d from the start of the worklist\n", bb->index);
  return bb;
}

static void
process_function (con_graph cg, tree function)
{
  struct varpool_node *vnode;
  con_node node;

  calculate_dominance_info (CDI_POST_DOMINATORS);

  setup_basic_blocks ();

  /* add the phantom actual parameters (Choi99 Sect. 4.1) */
  add_actual_parameters (cg, function);

  /* add the globals */
  FOR_EACH_STATIC_VARIABLE (vnode)
    {
      /* occasionally you see something like Jv_InitClass (x), in which x
       * is a record_type of java.lang.Class, which we should have added
       * as a global by then */
      if (has_pointer_type (vnode->decl) || is_class_type (vnode->decl))
	add_global_node (cg, vnode->decl);
    }
  ea_worklist = VEC_alloc (basic_block, heap, n_basic_blocks / 2);
  add_to_work_list (ENTRY_BLOCK_PTR);

  while (!VEC_empty(basic_block, ea_worklist))
    {
      tree phi;
      basic_block bb = fetch_from_worklist ();

      /* If theres a block in the worklist which this block
       * post-dominates, then we wont get the right answer until we
       * process that block */
      basic_block worklist_bb;
      int i;
      bool skip_bb = false;
      for (i = 0; VEC_iterate (basic_block, ea_worklist, i, worklist_bb); i++)
	{
	  if (bb != worklist_bb && dominated_by_p (CDI_POST_DOMINATORS, worklist_bb, bb))
	    {
	      fprintf (dump_file, "Skip block %d (it post-dominates block %d)\n", bb->index, worklist_bb->index);
	      add_to_work_list (bb);
	      skip_bb = true;
	      break;
	    }
	}
      if (skip_bb)
	continue;

      current_block = bb;

      /* process the phis and statements */
      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	{
	  if (should_visit (phi))
	    {
	      fprintf (dump_file, "Processing phi "); tf (phi, true);
	      update_connection_graph_from_phi (cg, phi);
	      nl (); nl ();
	      mark_visited (phi);
	    }
	}

      block_stmt_iterator bsi;
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);
	  if (should_visit (stmt))
	    {
	      fprintf (dump_file, "Processing stmt "); tf (stmt, true);
	      update_connection_graph_from_statement (cg, stmt);
	      nl (); nl ();
	      mark_visited (stmt);
	    }
	}

      if (stmts_to_visit == 0)
	break;
      else
	fprintf (dump_file, "Still %d stmts to go\n", stmts_to_visit);

      /* Add the next blocks to the worklist */
      edge e;
      edge_iterator ei;
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  add_to_work_list (e->dest);
	}
    }
    assert_all_next_link_free (cg);

  /* propagate the escape state - Choi99 Sect. 4.2 */
  for (node = cg->root; node; node = node->next)
    {
      /* if noEscape, there's no value to propagate */
      /* TODO limit when this is run */
      /* TODO move into a function which incorporates the
       * get_nodes_reachable_from */
      con_node reachable = get_nodes_reachable_from (node);
      while (reachable)
	{
	  if (node->escape > reachable->escape)
	    {
	      /* TODO test */
	      reachable->escape = node->escape;
	    }

	  NEXT_LINK_CLEAR (reachable);
	}
    }
  assert_all_next_link_free (cg);

  /* make the allocation use alloca */
  /*
  for (node = cg->root; node; node = node->next)
    {

      if ( node->type == OBJECT_NODE 
	   && node->phantom == false 
	   && node->escape == EA_NO_ESCAPE)
	{
	  enum statement_type type = get_statement_type (cg, node->id);
	  if (type == OBJECT_ALLOCATION)
	    replace_object_with_alloca (node); 
	  else if (type == OBJECT_ARRAY_ALLOCATION)
	    replace_array_with_alloca (node);
	  else if (type == PRIM_ARRAY_ALLOCATION)
	    replace_prim_array_with_alloca (node);
	  else if (type == MULTI_ARRAY_ALLOCATION)
	    replace_multi_array_with_alloca (node);
	}
    }
    */
  free_dominance_info (CDI_POST_DOMINATORS);
  free (bb_in_worklist);
  con_graph_dump (cg);

}


static unsigned int
execute_ipa_stack_allocate (void)
{
  struct cgraph_node *node;
  con_graph cg;
  con_graph last_cg = NULL;
  con_graph first_cg = NULL;
  /*tree alloca_func; */
  struct cgraph_node **order;
  int order_pos;
  int i;

  if (sorrycount || errorcount)
    {
      return 0;
    }

  /* init the markers used to traverse the graph */
  init_markers ();
  init_con_node_hashtable ();

  /* TODO should I use postorder from ipa.c ? */

  order = XCNEWVEC (struct cgraph_node *, cgraph_n_nodes);

  order_pos = ipa_utils_reduced_inorder (order, true, false);
  /* TODO what happens with cycles here. RE reduced parameter */

  for (i = 0; i < order_pos; i++)
    {
      node = order[i];
      if (node->analyzed)
	{

	  char* filename;

	  /* set up the current function */
	  push_cfun (DECL_STRUCT_FUNCTION (node->decl));
	  tree_register_cfg_hooks ();
	  current_function_decl = node->decl;

	  /* do devirtualization first */
	  FILE* saved = dump_file;
	  dump_file = NULL;
	  devirt ();
	  dump_file = saved;


	  fprintf (dump_file, "Processing function %s (%s)\n", 
		   IDENTIFIER_POINTER (DECL_NAME (current_function_decl)), 
		   IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME
				       (current_function_decl)));
	  dump_function_to_file (current_function_decl, dump_file, dump_flags);


	  /* set up this function's connection graph */
	  filename = concat ("graphs/", current_function_name(),
			     (IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME
						  (current_function_decl))),
			     ".graph", NULL);

	  if (strnlen (filename, 237) >= 237) 
	    {
	      filename[230] = '.';
	      filename[231] = 'g';
	      filename[232] = 'r';
	      filename[233] = 'a';
	      filename[234] = 'p';
	      filename[235] = 'h';
	      filename[236] = '\0';
	    }

	  cg = new_con_graph (filename, current_function_decl, last_cg);

	  /* fix up the next_cg list */
	  last_cg = cg;

	  /* make it cyclic */
	  if (first_cg == NULL)
	      first_cg = cg;
	  first_cg->next = cg;



	  process_function (cg, current_function_decl);

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

static bool
gate_ipa_stack_allocate (void)
{
  switch (flag_ipa_stack_allocate)
    {
    case 1:
      ea_interprocedural = false;
      ea_flow_sensitive = true;
      return true;
    case 3:
      ea_interprocedural = true;
      ea_flow_sensitive = true;
      return true;
    default:
      return false;
    }
}

struct tree_opt_pass pass_ipa_stack_allocate = {
  "stack-allocate",			/* name */
  gate_ipa_stack_allocate,		/* gate */
  execute_ipa_stack_allocate,		/* execute */
  &pass_rebuild_cgraph_edges,		/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_IPA_STACK_ALLOCATE,		/* tv_id */
  PROP_cfg,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  TODO_dump_func | TODO_dump_cgraph,	/* todo_flags_start */
  TODO_dump_func | TODO_dump_cgraph,	/* todo_flags_finish */
  0					/* letter */
};


static unsigned int
execute_instrument_stack_allocate (void)
{
  basic_block bb;
  block_stmt_iterator bsi;
  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);
	  tree call_expr, addr_expr, func_decl;
	  if (TREE_CODE (stmt) == MODIFY_EXPR 
	      && has_pointer_type (stmt) 
	      && SSA_VAR_P (LHS (stmt)) 
	      && TREE_CODE (call_expr = RHS (stmt)) == CALL_EXPR
	      && TREE_CODE (addr_expr = LHS (call_expr)) == ADDR_EXPR
	      && TREE_CODE (func_decl = LHS (addr_expr)) == FUNCTION_DECL)
	    {
	      /* _Jv_AllocObjectNoFinalizer */
	      if (func_decl == alloc_no_finalizer_node)
		{
		  TREE_OPERAND (addr_expr, 0) = init_no_finalizer_node;
		}
	      /* _Jv_NewObjectArray */
	      else if (func_decl == soft_anewarray_node)
		{
		  /* _Jv_InitNewObjectArray */
		  TREE_OPERAND (addr_expr, 0) = init_anewarray_node;
		}
	      /* _Jv_NewPrimArray */
	      else if (func_decl == soft_newarray_node)
		{
		  /* _Jv_InitPrimArray */
		  TREE_OPERAND (addr_expr, 0) = init_newarray_node;
		}
	      /* _Jv_AllocObject */
	      else if (func_decl == alloc_object_node)
		{
		  /* Obviously, this one we can't change with
		   * escape analysis, but its a useful metric */
		  /* _Jv_InitObject*/
		  TREE_OPERAND (addr_expr, 0) = init_object_node;
		}
	      /* _Jv_NewMultiArray*/
	      else if (func_decl == soft_multianewarray_node)
		{
		  /* _Jv_InitMultiArray */
		  TREE_OPERAND (addr_expr, 0) = init_multianewarray_node;
		}

	    }
	}
    }

  return 0;
}

static bool
gate_instrument_stack_allocate (void)
{
  return flag_ipa_stack_allocate == 2;
}

struct tree_opt_pass pass_instrument_stack_allocate = {
  "instrument-stack-allocate",		/* name */
  gate_instrument_stack_allocate,	/* gate */
  execute_instrument_stack_allocate,	/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func,			/* todo_flags_finish */
  0					/* letter */
};

void
init_gcj_stack_allocate (void)
{
  /*struct tree_opt_pass **p = &pass_all_optimizations.sub;*/
  struct tree_opt_pass **p = &all_ipa_passes;
  struct tree_opt_pass *next = &pass_ipa_type_escape;
  while (*p != next)
    p = &((*p)->next);
  *p = &pass_ipa_stack_allocate;
  pass_ipa_stack_allocate.next = &pass_instrument_stack_allocate;
  pass_instrument_stack_allocate.next = next;

  pass_ipa_stack_allocate.sub = NULL;
  pass_instrument_stack_allocate.sub = NULL;

  register_dump_files (&pass_ipa_stack_allocate, true, 0);
}
