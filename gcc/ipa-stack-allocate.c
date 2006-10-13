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

void dump_escape_function (con_graph cg, FILE*, FILE*);

#define UNHIDE_IT
#ifdef UNHIDE_IT

static FILE* debug;
static FILE* dump;

static tree
LHS (tree stmt)
{
  tree result = TREE_OPERAND (stmt, 0);
  gcc_assert (result);
  return result;

}

static tree
RHS (tree stmt)
{
  tree result = TREE_OPERAND (stmt, 1);
  gcc_assert (result);
  return result;
}

static bool
is_constructor (tree stmt)
{
  tree addr_expr, func_decl;
  return TREE_CODE (stmt) == CALL_EXPR
      && VOID_TYPE_P (TREE_TYPE (stmt))
      && TREE_CODE (addr_expr = LHS (stmt)) == ADDR_EXPR
      && TREE_CODE (func_decl = LHS (addr_expr)) == FUNCTION_DECL
      && DECL_NAME (func_decl) == get_identifier ("<init>");
}


static tree
get_constructor_function (tree stmt)
{
  tree result;
  gcc_assert (is_constructor (stmt));

  result = LHS (LHS (stmt));
  gcc_assert (result);
  return result;
}

static bool
is_suitable_decl (tree decl)
{
  gcc_assert (decl);
  return TREE_CODE (decl) == VAR_DECL || TREE_CODE (decl) == PARM_DECL;
}

static bool
is_pointer_type (tree stmt)
{
  /* From gccint 10.4.2: In GIMPLE form, type of MODIFY_EXPR is not
   * meaningful.  Instead, use type of LHS. */
  if (TREE_CODE (stmt) == MODIFY_EXPR)
    {
      return is_pointer_type (LHS (stmt));
    }
  /* TODO arrays of ints should be dealt with, no? */
  if (TREE_CODE (TREE_TYPE (stmt)) == ARRAY_TYPE)
    {
      return is_pointer_type (TREE_TYPE (stmt));
    }

  return POINTER_TYPE_P (TREE_TYPE (stmt));
}

/* this is supposed to work in theory, but doesnt
static bool
is_memory_allocation (tree stmt)
{
	return TREE_CODE (stmt) == CALL_EXPR && (call_expr_flags(stmt) & (ECF_MALLOC || ECF_MAY_BE_ALLOCA));
}
*/

static bool
is_java_allocator (tree func_decl)
{
  return func_decl == alloc_no_finalizer_node
    || func_decl == soft_anewarray_node;
}


static bool
is_memory_allocation (tree stmt)
{
  tree call_expr, addr_expr, func_decl;
  return TREE_CODE (stmt) == MODIFY_EXPR
    && is_pointer_type (stmt)
    && TREE_CODE (call_expr = RHS (stmt)) == CALL_EXPR
    && TREE_CODE (addr_expr = LHS (call_expr)) == ADDR_EXPR
    && TREE_CODE (func_decl = LHS (addr_expr)) == FUNCTION_DECL
    && is_java_allocator (func_decl);
}

static bool
check_null_pointer_exception_stmt (con_graph cg, tree stmt)
{
  /* _Jv_ThrowNullPointerException () */
  /* dont do anything, since nothing escapes here */
  tree addr_expr;
  if (!(TREE_CODE (stmt) == CALL_EXPR
      && TREE_CODE (addr_expr = LHS (stmt)) == ADDR_EXPR
      && LHS (addr_expr) == soft_nullpointer_node))
    {
      return false;
    }
  set_stmt_type (cg, stmt, IGNORED_NULL_POINTER_EXCEPTION);
  return true;
}

static bool
is_assignment_to_array (tree stmt)
{
  /* TODO */
  return false && stmt == stmt;
}

static bool
is_array_allocation (tree stmt)
{
  tree call_expr, addr_expr, func_decl;
  return TREE_CODE (stmt) == MODIFY_EXPR
      && is_pointer_type (stmt)
      && TREE_CODE (call_expr = RHS (stmt)) == CALL_EXPR
      && TREE_CODE (addr_expr = LHS (call_expr)) == ADDR_EXPR
      && TREE_CODE (func_decl = LHS (addr_expr)) == FUNCTION_DECL
      && func_decl == soft_anewarray_node;
}

static bool
is_indirect_function_call (tree stmt)
{
  return TREE_CODE (stmt) == CALL_EXPR
    && is_suitable_decl (LHS (stmt));
}

static bool
is_indirect_function_call_with_return_value (tree stmt)
{
  return TREE_CODE (stmt) == MODIFY_EXPR 
    && is_pointer_type (stmt) 
    && TREE_CODE (RHS (stmt)) == CALL_EXPR
    && is_suitable_decl (LHS (RHS (stmt)));
}

static bool
is_function_call (tree stmt)
{
  return TREE_CODE (stmt) == CALL_EXPR;
}

static bool
is_function_call_with_return_value (tree stmt)
{
  return TREE_CODE (stmt) == MODIFY_EXPR 
    && is_pointer_type (stmt) &&
    TREE_CODE (RHS (stmt)) == CALL_EXPR;
}

static bool
is_null (tree stmt)
{
  return TREE_CODE (stmt) == INTEGER_CST
    && is_pointer_type (stmt)
    && TREE_INT_CST_HIGH (stmt) == 0
    && TREE_INT_CST_LOW (stmt) == 0;
}

static bool
is_assignment_to_null (tree stmt)
{
  return TREE_CODE (stmt) == MODIFY_EXPR
    && is_pointer_type (stmt)
    && is_pointer_type (LHS (stmt)) 
    && is_null (RHS (stmt));
}

static void
update_connection_graph_from_array_allocation (con_graph cg, tree stmt)
{
  /* p = _Jv_NewObjectArray (type) */
  tree type_name;
  tree p_name;
  tree f_name;
  con_node object;
  con_node p;

  set_stmt_type (cg, stmt, MEMORY_ALLOCATION);

  /* get the class id */
  type_name = TREE_TYPE (TREE_TYPE (stmt));
  gcc_assert (type_name);

  /* get the source id */
  p_name = LHS (stmt);
  gcc_assert (p_name);

  /* if the source exists, bypass it */
  p = get_existing_node (cg, p_name);
  if (p)
    {
      /* bypass_node (p); */
      /* gcc_unreachable(); */
      /* still need an iterative solution when you're not doing
       * flow-sensisitve */
    }
  else
    {
      /* add the lhs node */
      p = add_local_node (cg, p_name);
    }
  gcc_assert (p);

  /* make a new object node, and add it to the graph */
  object = get_existing_node (cg, stmt);
  if (!object)
    {
      object = add_object_node (cg, stmt, type_name);
    }
  gcc_assert (object);


  /* add the points-to edge */
  if (get_edge (p, object) == NULL)
    add_edge (p, object);


  /* add fields */
  f_name = TREE_CHAIN (TYPE_FIELDS (TREE_TYPE (TREE_TYPE (stmt))));

  while (f_name)
    {
      /* dont add static fields */
      if (TREE_CODE (f_name) == FIELD_DECL)
	{
	  /* add the field */
	  con_node field = get_existing_field_node (cg, f_name, object);

	  /* only add edges which arent already there */
	  if (!field)
	    {
	      field = add_field_node (cg, f_name);
	      add_edge (object, field);
	    }

	  gcc_assert (field);

	}

      /* get the next field */
      f_name = TREE_CHAIN (f_name);
    }

  /* done MEMORY ALLOCATION */
}

static void
update_connection_graph_from_memory_allocation (con_graph cg, tree stmt)
{
  /* p = jv_AllocNoFinalizer(type) */
  tree type_name;
  tree p_name;
/*  tree f_name; */
  con_node object;
  con_node p;

  set_stmt_type (cg, stmt, MEMORY_ALLOCATION);

  /* get the class id */
  type_name = TREE_TYPE (TREE_TYPE (stmt));
  gcc_assert (type_name);

  /* get the source id */
  p_name = LHS (stmt);
  gcc_assert (p_name);

  /* if the source exists, bypass it */
  p = get_existing_node (cg, p_name);
  if (p)
    {
      /* bypass_node (p); */
      /* gcc_unreachable(); */
      /* still need an iterative solution when you're not doing
       * flow-sensisitve */
    }
  else
    {
      /* add the lhs node */
      p = add_local_node (cg, p_name);
    }
  gcc_assert (p);

  /* make a new object node, and add it to the graph */
  object = get_existing_node (cg, stmt);
  if (!object)
    {
      object = add_object_node (cg, stmt, type_name);
    }
  gcc_assert (object);


  /* add the points-to edge */
  if (get_edge (p, object) == NULL)
    add_edge (p, object);

}

static con_node
insert_reference (con_graph cg, tree p_name, tree q_name, bool killing)
{
  con_node p, q;
  gcc_assert (cg);
  gcc_assert (p_name);
  gcc_assert (q_name);

  p = get_existing_node (cg, p_name);
  if (p == NULL) p = add_local_node (cg, p_name);
  gcc_assert (p);

  q = get_existing_node (cg, q_name);
  if (q == NULL) q = add_local_node (cg, q_name);
  gcc_assert (q);

  if (killing)
    {
      /* do a bypass */
    }

  /* add an edge */
  if (get_edge (p, q) == NULL)
    add_edge (p, q);

  /* link the nodes and return them */
  assert_all_next_link_free (cg);
  p->next_link = q;
  return p;
}

static con_node
insert_assignment_from_field (con_graph cg, tree p_name, tree q_name, tree f_name, tree stmt_id, tree type, bool killing)
{
  con_node p, q, u, v;
  con_node terminal, phantom;
  int num_u;

  gcc_assert (cg);
  gcc_assert (p_name && q_name && f_name && stmt_id && type);

  p = get_existing_node (cg, p_name);
  if (p == NULL) p = add_local_node (cg, p_name);
  gcc_assert (p);

  q = get_existing_node (cg, q_name);
  if (q == NULL) q = add_local_node (cg, q_name);
  gcc_assert (q);

  assert_all_next_link_free (cg);

  /* we dont exactly follow the recommendations from Choi99. In
   * particular, the paper alleges that you only use phantom nodes if
   * there are no points to nodes, but in reality, there can be both at
   * the same time.  */

  /* get the objects pointed to be q */
  u = get_points_to (q);

  /* there may also be nodes pointed to by q which don't end up as an
   * object. These need to point to phantom objects */
  terminal = get_terminal_nodes (q);

  /* this is a lonely node */
  if (terminal == NULL && u == NULL)
    {
      gcc_assert (q->out == NULL);
      terminal = q;
    }

  /* get the phantom node ready, if needed */
  if (terminal)
    {
      phantom = get_existing_node (cg, stmt_id); /* 1-limited */
      if (phantom == NULL)
	{
	  phantom = add_object_node (cg, stmt_id, type);
	  phantom->phantom = true;
	}
      gcc_assert (phantom->phantom);

      /* add the phantom the the list of u */
      if (!in_link_list(u, phantom))
	{
	  phantom->next_link = u;
	  u = phantom;
	}
    }

  /* make the terminal nodes point to the node */
  while (terminal)
    {
      if (terminal != phantom) /* this would be pointless otherwise */
	  add_edge (terminal, phantom);

      NEXT_LINK_CLEAR (terminal);
    }

  /* get the field nodes of the pointed-to objects */
  num_u = link_length (u);
  v = get_field_nodes (u, f_name);
  gcc_assert (num_u == link_length (v));
  gcc_assert (v != NULL);


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


static bool
check_pointer_arithmetic_stmt (con_graph cg, tree stmt)
{
  /* make p global, link it to q */
  /* p = q + x     or
   * p = q - x:
   * modify_expr
   *   var_decl - p
   *   plus_expr (or minus_expr)
   *     var_decl - q
   *     integer_cst - x
   */
  tree p_name, q_name, plus_expr;
  con_node p, q;

  if (!(TREE_CODE (stmt) == MODIFY_EXPR 
    && is_pointer_type (stmt)
    && is_suitable_decl (p_name = LHS (stmt))
    && ((TREE_CODE (plus_expr = RHS (stmt)) == PLUS_EXPR) || (TREE_CODE (plus_expr) == MINUS_EXPR))
    /* should be able to use BINARY_CLASS_P here */
    && is_suitable_decl (q_name = LHS (plus_expr))
    && TREE_CODE (RHS (plus_expr)) == INTEGER_CST))
    {
      return false;
    }

  set_stmt_type (cg, stmt, POINTER_ARITHMETIC);

  p = insert_reference (cg, p_name, q_name, false);
  p->escape = EA_GLOBAL_ESCAPE;

  q = p->next_link;
  q->escape = EA_GLOBAL_ESCAPE;

  clear_links (p);
  return true;
}


static bool
check_pointer_dereference_stmt (con_graph cg, tree stmt)
{
  /* a link from q to p */
  /* p = *q:
   * modify_expr
   *   var_decl - p
   *   indirect_ref
   *     var_decl - q
   */
  tree p_name, q_name, indirect_ref;
  con_node q;

  if (!(TREE_CODE (stmt) == MODIFY_EXPR 
    && is_pointer_type (stmt) 
    && is_suitable_decl (p_name = LHS (stmt))
    && TREE_CODE (indirect_ref = RHS (stmt)) == INDIRECT_REF
    && is_suitable_decl (q_name = LHS (indirect_ref))))
    {
      return false;
    }

  set_stmt_type (cg, stmt, POINTER_DEREFERENCE);

  q = insert_reference (cg, q_name, p_name, true);
  clear_links (q); /* we dont need these for anything */

  return true;
}

static bool
check_cast_stmt (con_graph cg, tree stmt)
{
  /* TODO lots of killing references (due to iteration) */
  /* p = (x*) q:
   * modify_expr
   *   var_decl - p
   *   nop_expr
   *     var_decl - q
   */

  tree p_name, q_name, nop_expr;
  con_node p;
  if (!(TREE_CODE (stmt) == MODIFY_EXPR 
	&& is_pointer_type (stmt) 
	&& is_suitable_decl (p_name = LHS (stmt)) 
	&& TREE_CODE (nop_expr = RHS (stmt)) == NOP_EXPR
	&& is_suitable_decl (q_name = LHS (nop_expr))))
    {
      return false;
    }

  set_stmt_type (cg, stmt, CAST);
  p = insert_reference (cg, p_name, q_name, true);
  clear_links (p); /* we dont need these for anything */
  return true;
}

static bool
check_reference_copy_stmt (con_graph cg, tree stmt)
{
  /* TODO lots of killing references (due to iteration) */
  /* p = q:
   * modify_expr
   *   var_decl - p
   *   var_decl - q
   */
  tree p_name, q_name;
  con_node p;

  if (!(TREE_CODE (stmt) == MODIFY_EXPR 
	&& is_pointer_type (stmt) 
	&& is_suitable_decl (p_name = LHS (stmt)) 
	&& is_suitable_decl (q_name = RHS (stmt))))
    {
      return false;
    }

  set_stmt_type (cg, stmt, REFERENCE_COPY);
  p = insert_reference (cg, p_name, q_name, true);
  clear_links (p); /* we dont need these for anything */
  return true;
}

static con_node
insert_return (con_graph cg, tree return_name, tree r_name)
{
  con_node r, return_node;
  gcc_assert (cg);
  gcc_assert (r_name);
  gcc_assert (return_name);

  r = get_existing_node (cg, r_name);
  if (r == NULL) r = add_local_node (cg, r_name);
  gcc_assert (r);
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

static bool
check_return_stmt (con_graph cg, tree stmt)
{
  /* return u:
   * return_expr (void)
   *   modify_expr
   *     result_decl - result_decl
   *     var_decl - r
   */
  tree r_name, result_decl;
  tree modify_expr;
  con_node r;

  if (!(TREE_CODE (stmt) == RETURN_EXPR
	&& TREE_OPERAND (stmt, 0) /* dont match null cases */
	&& TREE_CODE (modify_expr = LHS (stmt)) == MODIFY_EXPR
	&& is_pointer_type (modify_expr)
	&& TREE_CODE (result_decl = LHS (modify_expr)) == RESULT_DECL
	&& is_suitable_decl (r_name = RHS (modify_expr))))
    {
      return false;
    }

  set_stmt_type (cg, stmt, RETURN);
  r = insert_return (cg, result_decl, r_name);
  assert_all_next_link_free (cg);
  return true;
}

static void
update_connection_graph_from_indirect_function_call (con_graph cg, tree stmt)
{
  /* make all nodes involved argEscape */
  /* T (a, b):
   * call_expr
   *   var_decl - T
   *   tree_list  - a | (b, c, d...)
   */
  tree T;
  tree argument_list;
  tree r_name;
  con_node r;
  tree call_expr;


  if (is_indirect_function_call_with_return_value (stmt))
    {
      /* get the name of the lvalue */
      r_name = LHS (stmt);
      gcc_assert (r_name);

      /* ----------------------------------------
       *      get the nodes from the graph
       * ----------------------------------------*/

      r = get_existing_node (cg, r_name);
      /* TODO should use DECL_RESULT here, and use that as R
       * instead of lhs. arrow then from lhs to r? */
      if (r)
	{
	  /* bypass_node (r); */
	  /* gcc_unreachable(); */
	}
      else
	{
	  r = add_local_node (cg, r_name);
	  r->escape = EA_ARG_ESCAPE;
	}
      call_expr = RHS (stmt);
      set_stmt_type (cg, stmt, INDIRECT_FUNCTION_CALL_WITH_RETURN);
    }
  else
    {
      set_stmt_type (cg, stmt, INDIRECT_FUNCTION_CALL);

      call_expr = stmt;
    }

  gcc_assert (TREE_CODE (call_expr) == CALL_EXPR);
  gcc_assert (is_suitable_decl (LHS (call_expr)));
  gcc_assert (TREE_CODE (RHS (call_expr)) == TREE_LIST);

  T = LHS (call_expr);
  gcc_assert (T);
  argument_list = RHS (call_expr);
  gcc_assert (argument_list);

  /* We make all of these nodes GLOBAL_ESCAPE. Since we dont have the
   * function which is called on them, we can't out the possability that
   * each of these is assigned to a global variable, and thus escape
   * globally. The correct thing to do is to have an edge from all global
   * nodes, and from all field nodes of the polymorphic object to whom the
   * function T belongs, to each of the arguments, but setting them as
   * GLOBAL is just as correct, and far easier */
  while (argument_list)
    {
      tree argument = TREE_VALUE (argument_list);
      if (is_pointer_type (argument))
	{
	  con_node argument_node = get_existing_node (cg, argument);

	  if (argument_node == NULL)
	    {
	      argument_node = add_local_node (cg, argument);
	    }
	  gcc_assert (argument_node);
	  set_escape_state (argument_node, EA_GLOBAL_ESCAPE);

	}
      argument_list = TREE_CHAIN (argument_list);
    }

  /* done INDIRECT_FUNCTION_CALL */
}

static void
update_connection_graph_from_function_call (con_graph cg, tree stmt)
{
  /* u.T(a,b); or */
  /* r = u.T(a,b); */
  /* for intraprocedural, just create node and make it ArgEsacpe */
  tree r_name;
  con_node r = NULL;

  tree call_expr;
  tree addr_expr;
  tree function_decl;
  tree argument_list;

  bool function_in_java_lang_object;

  int i;


  if (is_function_call_with_return_value (stmt))
    {
      /* get the name of the lvalue */
      r_name = LHS (stmt);
      gcc_assert (r_name);

      /* ----------------------------------------
       *      get the nodes from the graph
       * ----------------------------------------*/

      r = get_existing_node (cg, r_name);
      /* TODO should use DECL_RESULT here, and use that as R
       * instead of lhs. arrow then from lhs to r? */
      if (r)
	{
	  /* bypass_node (r); */
	  /* gcc_unreachable(); */
	}
      else
	{
	  r = add_local_node (cg, r_name);
	  r->escape = EA_ARG_ESCAPE;
	}
      call_expr = RHS (stmt);
      set_stmt_type (cg, stmt, FUNCTION_CALL_WITH_RETURN);
    }
  else
    {
      if (is_constructor (stmt))
	set_stmt_type (cg, stmt, CONSTRUCTOR_STMT);
      else
	set_stmt_type (cg, stmt, FUNCTION_CALL);

      call_expr = stmt;
    }

  gcc_assert (TREE_CODE (call_expr) == CALL_EXPR);

  /* ----------------------------------------
   *      get detail from the tree
   * ----------------------------------------*/

  /* this isnt necessarily an addr_expr, but it is in the context we use
   * it here */
  addr_expr = TREE_OPERAND (call_expr, 0);

  gcc_assert (TREE_CODE (addr_expr) == ADDR_EXPR);

  function_decl = TREE_OPERAND (addr_expr, 0);
  gcc_assert (TREE_CODE (addr_expr) == ADDR_EXPR);
  argument_list = TREE_OPERAND (call_expr, 1);


  function_in_java_lang_object = (object_type_node == DECL_CONTEXT (function_decl));


  i = 1;

  /* Add special nodes for the function arguments */
  /* Choi99 4.4 */
  while (!function_in_java_lang_object && argument_list)
    {
      tree argument = TREE_VALUE (argument_list);
      if (is_pointer_type (argument))
	{
	  con_node actual_node =
	    get_caller_actual_node (cg, i, stmt);
	  if (actual_node == NULL)
	    {
	      con_node argument_node;
	      actual_node = add_caller_actual_node (cg, argument, i,
						    stmt);
	      argument_node = get_existing_node (cg, argument);

	      if (argument_node == NULL)
		{
		  /* this can be an ADDR_EXPR, which means &a, so we
		   * add a, and we add &a, with an edge, if thats the
		   * case */
		  if (TREE_CODE (argument) == ADDR_EXPR)
		    {
		      tree addressed_expr = TREE_OPERAND (argument, 0);
		      con_node addressed_node = get_existing_node (cg,
								   addressed_expr);
		      if (addressed_node == NULL)
			addressed_node = add_local_node (cg, addressed_expr);
		      argument_node = add_local_node (cg, argument);
		      add_edge (argument_node, addressed_node);
		    }
		  else
		    {
		      argument_node = add_local_node (cg, argument);
		    }
		}
	      gcc_assert (argument_node);
	      gcc_assert (actual_node);
	      add_edge (actual_node, argument_node);

	      i++;
	    }
	}
      argument_list = TREE_CHAIN (argument_list);
    }

  /* done FUNCTION_CALL, FUNCTION_CALL_WITH_RETURN and
   * CONSTRUCTOR */
}

static bool
check_assignment_from_data_array_stmt (con_graph cg, tree stmt)
{
  /* this is a mix of assignment from field, and assignment from array */
  /* p = q.data[x]
   * modify_expr
   *   var_decl - p
   *   array_ref
   *     component_ref
   *	   indirect_ref
   *	     var_decl - q
   *	   field_decl - f (aka data)
   *	 integer_cst - x
   */
  tree p_name, q_name, f_name;
  tree array_ref, component_ref, indirect_ref;
  con_node p;
  if (!(TREE_CODE (stmt) == MODIFY_EXPR
	&& is_pointer_type (stmt)
	&& is_suitable_decl (p_name = LHS (stmt))
	&& TREE_CODE (array_ref = RHS (stmt)) == ARRAY_REF
	&& TREE_CODE (component_ref = LHS (array_ref)) == COMPONENT_REF
	&& TREE_CODE (indirect_ref = LHS (component_ref)) == INDIRECT_REF
	&& is_suitable_decl (q_name = LHS (indirect_ref))
	&& TREE_CODE (f_name = RHS (component_ref)) == FIELD_DECL
	&& DECL_NAME (f_name) == get_identifier ("data")))
    {
      return false;
    }

  set_stmt_type (cg, stmt, ASSIGNMENT_FROM_DATA_ARRAY);
  p = insert_assignment_from_field (cg, p_name, q_name, f_name, stmt,
				    TREE_TYPE (indirect_ref), true);

  clear_links (p);
  return true;
}

/* you might think this returns p, then the fs, then q, but it really
 * returns p, then q, then all fs. It means q is easy to access, as you
 * dont have to iterate over the f list to get it */
static con_node
insert_assignment_to_field (con_graph cg, tree p_name, tree q_name, tree f_name, tree stmt_id, tree type, bool killing)
{
  con_node p, q, u, v;
  con_node terminal, phantom;
  int num_u;

  gcc_assert (cg);
  gcc_assert (p_name && q_name && f_name && stmt_id && type);

  p = get_existing_node (cg, p_name);
  if (p == NULL) p = add_local_node (cg, p_name);
  gcc_assert (p);

  q = get_existing_node (cg, q_name);
  if (q == NULL) q = add_local_node (cg, q_name);
  gcc_assert (q);

  assert_all_next_link_free (cg);

  /* we dont exactly follow the recommendations from Choi99. In
   * particular, the paper alleges that you only use phantom nodes if
   * there are no points to nodes, but in reality, there can be both at
   * the same time.  */
  u = get_points_to (p);
  terminal = get_terminal_nodes (p);

  /* this is a lonely node */
  if (terminal == NULL && u == NULL)
    {
      gcc_assert (p->out == NULL);
      terminal = p;
    }

  /* get the phantom node ready, if needed */
  if (terminal)
    {
      phantom = get_existing_node (cg, stmt_id); /* 1-limited */
      if (phantom == NULL)
	{
	  phantom = add_object_node (cg, stmt_id, type);
	  phantom->phantom = true;
	}
      gcc_assert (phantom->phantom);

      /* add the phantom the the list of u */
      gcc_assert (phantom->next_link == NULL);
      if (!in_link_list(u, phantom))
	{
	  phantom->next_link = u;
	  u = phantom;
	}
    }

  /* make the terminal nodes point to the node */
  while (terminal)
    {
      if (terminal != phantom) /* this would be pointless otherwise */
	  add_edge (terminal, phantom);

      NEXT_LINK_CLEAR (terminal);
    }

  /* get the field nodes of the pointed-to objects */
  num_u = link_length (u);
  v = get_field_nodes (u, f_name);
  gcc_assert (num_u == link_length (v));
  gcc_assert (v != NULL); /* even phantom nodes have this field */

  
  /* note that we dont bypass here, as Choi99 says we can't
   * kill p.f.  Not sure why though... */
  /* do not bypass, this isnt a killing assignment */
  if (killing)
    {
      /* TODO: bypassing */
    }

  /* we need to return v, so keep track of the first in the list */
  q->next_link = v;

  /* add deferred edges */
  while (v)
    {
      if (get_edge (v, q) == NULL)
	add_edge (v, q);

      NEXT_LINK (v);
    }

  /* link together p, q, and all f */
  p->next_link = q;
  return p;
}

static bool
check_assignment_to_exception_object (con_graph cg, tree stmt)
{
  /* p = exception:
   * modify_expr
   *   var_decl - p
   *   exc_ptr_expr - q */
  tree p_name, q_name;
  con_node p;
  if (!(TREE_CODE (stmt) == MODIFY_EXPR
	&& is_pointer_type (stmt)
	&& TREE_CODE (p_name = LHS (stmt)) == EXC_PTR_EXPR
	&& is_suitable_decl (q_name = RHS (stmt))))
    {
      return false;
    }

  set_stmt_type (cg, stmt, ASSIGNMENT_TO_EXCEPTION);
  p = insert_reference (cg, p_name, q_name, false);
  p->escape = EA_GLOBAL_ESCAPE;
  clear_links (p);
  return true;
}

static bool
check_assignment_from_exception_object (con_graph cg, tree stmt)
{
  /* p = exception:
   * modify_expr
   *   var_decl - p
   *   exc_ptr_expr - q */
  tree p_name, q_name;
  con_node p, q;
  if (!(TREE_CODE (stmt) == MODIFY_EXPR
	&& is_pointer_type (stmt)
	&& is_suitable_decl (p_name = LHS (stmt))
	&& TREE_CODE (q_name = RHS (stmt)) == EXC_PTR_EXPR))
    {
      return false;
    }

  set_stmt_type (cg, stmt, ASSIGNMENT_FROM_EXCEPTION);
  p = insert_reference (cg, p_name, q_name, false);
  q = p->next_link;
  q->escape = EA_GLOBAL_ESCAPE;
  clear_links (p);
  return true;
}

static bool
check_assignment_from_array_stmt (con_graph cg, tree stmt)
{
  /* deal with this as if it were p = q; (except it's non-killing)
   * here */

  /* p = q[x]: 
   * modify_expr
   *   var_decl - p
   *   array_ref
   *     var_decl - q
   *     integer_cst - x*/

  tree p_name, q_name, array_ref;
  con_node p;

  if (!(TREE_CODE (stmt) == MODIFY_EXPR
    && is_pointer_type (stmt)
    && is_suitable_decl (p_name = LHS (stmt))
    && TREE_CODE (array_ref = RHS (stmt)) == ARRAY_REF
    && is_suitable_decl (q_name = LHS (array_ref))
    && TREE_CODE (RHS (array_ref)) == INTEGER_CST))
    {
      return false;
    }

  set_stmt_type (cg, stmt, ASSIGNMENT_FROM_ARRAY);
  p = insert_reference (cg, p_name, q_name, false);
  clear_links (p); /* we dont need these for anything */
  return true;

}

static bool
check_assignment_to_data_array_stmt (con_graph cg, tree stmt)
{
  /* this is a mix of assignment to field, and assignment to array */
  /* p.data[x] = q:
   * modify_expr
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
  if (!(TREE_CODE (stmt) == MODIFY_EXPR
	&& is_pointer_type (stmt)
	&& TREE_CODE (array_ref = LHS (stmt)) == ARRAY_REF
	&& TREE_CODE (component_ref = LHS (array_ref)) == COMPONENT_REF
	&& TREE_CODE (indirect_ref = LHS (component_ref)) == INDIRECT_REF
	&& is_suitable_decl (p_name = LHS (indirect_ref))
	&& TREE_CODE (f_name = RHS (component_ref)) == FIELD_DECL
	&& DECL_NAME (f_name) == get_identifier ("data")
	&& is_suitable_decl (q_name = RHS (stmt))))
    {
      return false;
    }

  set_stmt_type (cg, stmt, ASSIGNMENT_TO_INDIRECT_ARRAY_REF);
  p = insert_assignment_to_field (cg, p_name, q_name, f_name, stmt,
				  TREE_TYPE (indirect_ref), false);
  clear_links (p);
  return true;
}


static bool
check_assignment_to_field_stmt (con_graph cg, tree stmt)
{
  /* p.f = q */
  /* modify_expr
   *   component_ref
   *     indirect_ref
   *       var_decl - p
   *     field_decl - f
   *   var_decl - q
   */
  tree p_name, f_name, q_name;
  tree indirect_ref, component_ref;
  con_node p;

  if (!(TREE_CODE (stmt) == MODIFY_EXPR
	&& is_pointer_type (stmt)
	&& TREE_CODE (component_ref = LHS (stmt)) == COMPONENT_REF
	&& TREE_CODE (indirect_ref = LHS (component_ref)) == INDIRECT_REF
	&& is_suitable_decl (p_name = LHS (indirect_ref))
	&& TREE_CODE (f_name = RHS (component_ref)) == FIELD_DECL
	&& is_suitable_decl (q_name = RHS (stmt))))
    {
      return false;
    }

  set_stmt_type (cg, stmt, ASSIGNMENT_TO_FIELD);

  p = insert_assignment_to_field (cg, p_name, q_name, f_name, stmt,
				  TREE_TYPE (indirect_ref), true);
  clear_links (p);
  return true;
}



static bool
check_assignment_from_field_stmt (con_graph cg, tree stmt)
{
  /* p = q.f:
   * modify_expr
   *   var_decl - p
   *   component_ref
   *     indirect_ref
   *       var_decl - q
   *     field_decl - vtable
   */

  tree p_name, q_name, f_name;
  tree component_ref, indirect_ref;
  con_node p;

  if (!(TREE_CODE (stmt) == MODIFY_EXPR
	&& is_pointer_type (stmt)
	&& is_suitable_decl (p_name = LHS (stmt))
	&& TREE_CODE (component_ref = RHS (stmt)) == COMPONENT_REF
	&& TREE_CODE (indirect_ref = LHS (component_ref)) == INDIRECT_REF
	&& is_suitable_decl (q_name = LHS (indirect_ref))
	&& TREE_CODE (f_name = RHS (component_ref)) == FIELD_DECL))
    {
      return false;
    }

  set_stmt_type (cg, stmt, ASSIGNMENT_FROM_FIELD);
  /* TODO: I think we can get rid of type, since we'll do the fields
   * lazily soon */
  p = insert_assignment_from_field (cg, p_name, q_name, f_name, stmt, TREE_TYPE (indirect_ref), true);
  clear_links (p);

  return true;

}

static bool
check_vtable_stmt (con_graph cg, tree stmt)
{
  /* note that we dont need access to the field nodes, so theres no need
   * to go searching for objects corresponding to them, here */

  /* p = q.vtable:
   * modify_expr
   *   var_decl - p
   *   component_ref
   *     indirect_ref
   *       var_decl - q
   *     field_decl - vtable
   */
  tree p_name, q_name, component_ref, indirect_ref, field_decl;
  con_node p;
  if (!(TREE_CODE (stmt) == MODIFY_EXPR
	&& is_pointer_type (stmt)
	&& is_suitable_decl (p_name = LHS (stmt))
	&& TREE_CODE (component_ref = RHS (stmt)) == COMPONENT_REF
	&& TREE_CODE (indirect_ref = LHS (component_ref)) == INDIRECT_REF
	&& is_suitable_decl (q_name = LHS (indirect_ref))
	&& TREE_CODE (field_decl = RHS (component_ref)) == FIELD_DECL
	&& DECL_NAME (field_decl) == get_identifier ("vtable")))
    {
      return false;
    }
  set_stmt_type (cg, stmt, ASSIGNMENT_FROM_VTABLE);

  /* this is a sticky situation, so we're choosing a BOTTOM solution */
  p = insert_reference (cg, p_name, q_name, false);
  p->escape = EA_GLOBAL_ESCAPE;
  clear_links (p);
  return true;
}

static void
update_connection_graph_from_statement (con_graph cg, tree stmt)
{
  gcc_assert (cg);
  gcc_assert (stmt);

  /* recognize all the statement types, and update the connection
   * graph accordingly */

  /* memory allocation */
  if (is_memory_allocation (stmt))
    update_connection_graph_from_memory_allocation (cg, stmt);

  else if (is_array_allocation (stmt))
    {
      update_connection_graph_from_array_allocation (cg, stmt);
    }

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
      set_stmt_type (cg, stmt, IGNORED_ASSIGNMENT_TO_NULL);

      /* Done ASSIGNMENT TO NULL */
    }


  /* exceptions - put these before function calls */
  else if (check_null_pointer_exception_stmt (cg, stmt)) { /* NOP */ }
  else if (check_assignment_from_exception_object (cg, stmt)) { /* NOP */ }
  else if (check_assignment_to_exception_object (cg, stmt)) { /* NOP */ }


  /* function calls - direct and indirect */
  else if (is_indirect_function_call (stmt)
	   || is_indirect_function_call_with_return_value (stmt))
    update_connection_graph_from_indirect_function_call (cg, stmt);

  else if (is_function_call (stmt)
	   || is_function_call_with_return_value (stmt)
	   || is_constructor (stmt))
    update_connection_graph_from_function_call (cg, stmt);


    /* fields */
  /* this must come before assignment_to_field, or it wont hit */
  else if (check_vtable_stmt (cg, stmt)) { /* NOP */ }
  else if (check_assignment_to_field_stmt (cg, stmt)) { /* NOP */ }
  else if (check_assignment_from_field_stmt (cg, stmt)) { /* NOP */ }

  /* arrays */
  else if (check_assignment_from_array_stmt (cg, stmt)) { /* NOP */ }
  else if (check_assignment_from_data_array_stmt (cg, stmt)) { /* NOP */ }
  else if (check_assignment_to_data_array_stmt (cg, stmt)) { /* NOP */ }

  else if (is_assignment_to_array (stmt))
      /* TODO */ gcc_unreachable ();
   

  /* awfulness */
  else if (check_pointer_arithmetic_stmt (cg, stmt)) { /* NOP */ }

  /* ignored */
  else
    {
      if (TREE_CODE (stmt) == RETURN_EXPR && VOID_TYPE_P (TREE_TYPE
							  (stmt)))
	  set_stmt_type (cg, stmt, IGNORED_RETURNING_VOID);

      if (! is_pointer_type (stmt))
	  set_stmt_type (cg, stmt, IGNORED_NOT_A_POINTER);

      if (TREE_CODE (stmt) == LABEL_EXPR)
	  set_stmt_type (cg, stmt, IGNORED_LABEL_EXPR);

      if (TREE_CODE (stmt) == COND_EXPR)
	  set_stmt_type (cg, stmt, IGNORED_COND_EXPR);

      if (get_stmt_type (cg, stmt) == NULL)
	{
	  set_stmt_type (cg, stmt, IGNORED_UNKNOWN);
	  gcc_unreachable ();	/* as if */
	}

    }
}

static void
replace_array_with_alloca (con_node node)
{
  if (node == NULL)
    {
      gcc_unreachable ();
    }
  return;
#if 0  
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
  bsi_remove(&bsi, true);

#endif
}

static void
replace_with_alloca (con_node node)
{
  tree size_identifier, size_field, new_size_stmt;
  tree type, alloca_func_call, new_alloca_stmt;
  tree new_jv_func_call_stmt;

  tree stmt, call_expr, addr_expr, func_decl, lhs, parms, klass;
  tree alloca_func, args, parm1, parm2;
  block_stmt_iterator bsi;

  stmt = node->id;
  bsi = bsi_for_stmt (stmt);

  gcc_assert (is_memory_allocation (stmt));

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
  bsi_remove(&bsi, true);
}

static void add_actual_parameters (con_graph cg, tree function)
{
  tree arg_list;
  int i = 1;
  con_node arg_node;
  con_node actual_node;

  gcc_assert (cg);
  gcc_assert (function);

  arg_list = DECL_ARGUMENTS (function);
  while (arg_list)
    {
      if (is_pointer_type (arg_list))
	{
	  actual_node = add_callee_actual_node (cg, arg_list, i,
						function);
	  arg_node = add_local_node (cg, arg_list);

	  /* without this, we would match the parameter to the node
	   * which parameter_node points at, at the end of this
	   * function, instead of now */
	  add_edge (arg_node, actual_node);
	  i++;
	}

      /* move the list on */
      arg_list = TREE_CHAIN (arg_list);
    }


}

static void
process_function (con_graph cg, tree function)
{
  struct cgraph_varpool_node *vnode;
  basic_block bb;
  block_stmt_iterator bsi;
  con_node node;
  int j;

  gcc_assert (cg);
  gcc_assert (function);
  gcc_assert (cg->function);

  /* add the phantom actual parameters (Choi99 Sect. 4.1) */
  add_actual_parameters (cg, function);

  /* add the globals */
  for (vnode = cgraph_varpool_nodes_queue;
       vnode; vnode = vnode->next_needed)
    {
      if (is_pointer_type (vnode->decl))
	add_global_node (cg, vnode->decl);
    }

  /* process the method body */
  for (j = 0; j < 10; j++)
    {

      FOR_EACH_BB (bb)
      {
        /*
        fprintf (stderr, "\n\n----------------------------\n");
        fprintf (stderr, "Basic block %d (iteration %d) \n", 
            bb->index, get_iteration_count(cg, bb->index));
        */

	gcc_assert (bb->index < n_basic_blocks);

	/* process the statements */
	for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	  {
	    tree stmt = bsi_stmt (bsi);
	    update_connection_graph_from_statement (cg, stmt);
	  }
      }
    }

  
  /* inline the constructors */
  FOR_EACH_BB (bb)
  {
    /* process the statements */
    for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
      {
	tree stmt = bsi_stmt (bsi);

	/* inline constructors. this only needs to be
	 * done once per BB */
	if (is_constructor (stmt))
	  {
	    con_graph callee_cg =
	      get_cg_for_function (cg,
				   get_constructor_function (stmt));

	    if (callee_cg)
	      {
		gcc_assert (get_caller_actual_node (cg, 1, stmt));
		/* the caller nodes should alredy be added (we need them
		 * to inline) */
		inline_constructor_graph (cg, callee_cg, stmt);
	      }
	  }
      }
  }

  assert_all_next_link_free (cg);

  /* do the bypassing */
  for (node = cg->root; node; node = node->next)
    {
      if (is_reference_node (node))
	{
	  /* TODO currently broken, but doesnt affect the correctness, and
	   * is really a speedup step, so leave til later */
/*				bypass_node (node); */
	}
    }
  assert_all_next_link_free (cg);




  /* propagate the escape state - Choi99 Sect. 4.2 */
  for (node = cg->root; node; node = node->next)
    {
      /* if noEscape, there's no value to propagate */
      /* TODO limit when this is run */
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
  for (node = cg->root; node; node = node->next)
    {

      if ( node->type == OBJECT_NODE 
	   && node->phantom == false 
	   && node->escape == EA_NO_ESCAPE)
	{
	  if (is_array_allocation (node->id))
	    replace_array_with_alloca (node);
	  else
	    replace_with_alloca (node); 
	}
    }
  dump_escape_function (cg, debug, dump);
  con_graph_dump (cg);

}


static unsigned int
execute_ipa_stack_allocate (void)
{
  struct cgraph_varpool_node *vnode;
  struct cgraph_node *node;
  con_graph cg;
  con_graph last_cg = NULL;
  con_graph first_cg = NULL;
  /*tree alloca_func; */
  struct cgraph_node **order;
  int order_pos;
  int i;

  int flags = TDF_SLIM | TDF_DETAILS | TDF_BLOCKS | TDF_IPA | TDF_TREE |
    TDF_GRAPH;
  pretty_printer debug_buffer;
  pretty_printer dump_buffer;

  debug = fopen ("debug_log", "a");
  dump = fopen ("dump_log", "a");
  gcc_assert (debug);
  gcc_assert (dump);

  pp_construct (&debug_buffer, NULL, 0);
  pp_construct (&dump_buffer, NULL, 0);
  pp_needs_newline (&debug_buffer) = true;
  pp_needs_newline (&dump_buffer) = true;

  debug_buffer.buffer->stream = debug;
  dump_buffer.buffer->stream = dump;

  /* print the global nodes */
  for (vnode = cgraph_varpool_nodes_queue;
       vnode; vnode = vnode->next_needed)
    {
      if (is_pointer_type (vnode->decl))
	{
	  dump_generic_node (&debug_buffer, vnode->decl, 0, flags, true);
	  pp_flush (&debug_buffer);
	  dump_generic_node (&dump_buffer, vnode->decl, 0, flags, true);
	  pp_flush (&dump_buffer);
	}
    }



  /* TODO should I use postorder from ipa.c ? */

  order = XCNEWVEC (struct cgraph_node *, cgraph_n_nodes);

  order_pos = ipa_utils_reduced_inorder (order, true, false);
  /* TODO what happens with cycles here. RE reduced parameter */

  for (i = 0; i < order_pos; i++)
    {
      node = order[i];
      if (node->analyzed)
	{
	  /* set up the current function */
	  push_cfun (DECL_STRUCT_FUNCTION (node->decl));
	  tree_register_cfg_hooks ();
	  current_function_decl = node->decl;


	  /* set up this function's connection graph */
	  cg = new_con_graph (concat ("graphs/", current_function_name (),
				      (IDENTIFIER_POINTER
				       (DECL_ASSEMBLER_NAME
					(current_function_decl))), ".graph",
				      NULL), current_function_decl,
			      last_cg);

	  /* fix up the next_cg list */
	  last_cg = cg;

	  /* make it cyclic */
	  if (first_cg == NULL)
	      first_cg = cg;
	  first_cg->next = cg;

#ifdef UNHIDE_IT
	  process_function (cg, current_function_decl);
#endif
	  pop_cfun ();


	  /* dump the connection graph */
	  con_graph_dump (cg);
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

  fclose (debug);
  fclose (dump);
  return 0;
}

#else

static unsigned int
execute_ipa_stack_allocate (void)
{
  return 0;
}

#endif /* UNHIDE_IT */


static bool
gate_ipa_stack_allocate (void)
{
  if (flag_ipa_stack_allocate != 0)
    {
      fprintf (stderr, "Running\n");
    }
  else
      fprintf (stderr, "Not running\n");

  return flag_ipa_stack_allocate != 0;
}

struct tree_opt_pass pass_ipa_stack_allocate = {
  "stack-allocate",		/* name */
  gate_ipa_stack_allocate,	/* gate */
  execute_ipa_stack_allocate,	/* execute */
  &pass_rebuild_cgraph_edges,	/* sub */
  NULL,				/* next */
  0,				/* static_pass_number */
  TV_IPA_STACK_ALLOCATE,	/* tv_id */
  0,				/* properties_required */
  0,				/* properties_provided */
  0,				/* properties_destroyed */
  TODO_dump_func | TODO_dump_cgraph,	/* todo_flags_start */
  TODO_dump_func | TODO_dump_cgraph,	/* todo_flags_finish */
  0				/* letter */
};

