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
 * TODO when I switch to flow sensitive, its even harder without SSA
 * TODO i think the phantom-ness is unnecessary
 * TODO throw
 * TODO remove terminal nodes incomming deferred edges according to
 * toplas03, Sect 4.2
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

#define UNHIDE_IT
#ifdef UNHIDE_IT

static FILE* debug;
static FILE* dump;
void dump_escape_function (con_graph cg, FILE*, FILE*);

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
  if (TREE_CODE (stmt) == CALL_EXPR
      && (VOID_TYPE_P (TREE_TYPE (stmt))))
    {
      tree addr_expr = TREE_OPERAND (stmt, 0);
      if (TREE_CODE (addr_expr) == ADDR_EXPR)
	{
	  tree func_decl = TREE_OPERAND (addr_expr, 0);
	  const char *name =
	    IDENTIFIER_POINTER (DECL_NAME (func_decl));
	  if (strcmp (name, "<init>") == 0)
	    return true;
	}
    }
  return false;
}


static tree
get_constructor_function (tree stmt)
{
  gcc_assert (is_constructor (stmt));

  if (TREE_CODE (stmt) == CALL_EXPR
      && (VOID_TYPE_P (TREE_TYPE (stmt))))
    {
      tree addr_expr = TREE_OPERAND (stmt, 0);
      if (TREE_CODE (addr_expr) == ADDR_EXPR)
	{
	  tree result = TREE_OPERAND (addr_expr, 0);
	  gcc_assert (result);
	  return result;
	}
    }
  gcc_unreachable ();
}

static bool
is_suitable_decl (tree decl)
{
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

static bool
is_pointer_arithmetic (tree stmt)
{
  return TREE_CODE (stmt) == MODIFY_EXPR 
    && is_pointer_type (stmt) 
    && TREE_CODE (RHS (stmt)) == PLUS_EXPR;
}

static bool
is_pointer_dereference (tree stmt)
{
  return TREE_CODE (stmt) == MODIFY_EXPR 
    && is_pointer_type (stmt) 
    && TREE_CODE (RHS (stmt)) == INDIRECT_REF
    && is_suitable_decl (LHS (RHS (stmt)));
}

/* this is supposed to work in theory, but doesnt
static bool
is_memory_allocation (tree stmt)
{
	return TREE_CODE (stmt) == CALL_EXPR && (call_expr_flags(stmt) & (ECF_MALLOC || ECF_MAY_BE_ALLOCA));
}
*/

static int allocator_count = 2;
static tree allocators[2];


static bool
is_memory_allocation (tree stmt)
{
  /* java doesnt support allocating to non-pointer types, but this is
   * here in case I dont notice for the migration to c++ */
  if (TREE_CODE (stmt) == MODIFY_EXPR
      && (POINTER_TYPE_P (TREE_TYPE (stmt))))
    {
      tree rhs = RHS (stmt);

      if (TREE_CODE (rhs) == CALL_EXPR)
	{
	  tree addr_expr = TREE_OPERAND (rhs, 0);
	  if (TREE_CODE (addr_expr) == ADDR_EXPR)
	    {
	      tree func_decl = TREE_OPERAND (addr_expr, 0);
	      int i;
	      for (i = 0; i < allocator_count; i++)
		{
		  if (func_decl == allocators[i])
		    {
		      return true;
		    }
		}
	    }
	}
    }
  return false;
}

static bool
is_assignment_from_array (tree stmt)
{
  return TREE_CODE (stmt) == MODIFY_EXPR
    && TREE_CODE (RHS (stmt)) == ARRAY_REF;
}

static bool
is_assignment_from_vtable (tree stmt)
{
  return TREE_CODE (stmt) == MODIFY_EXPR
    && TREE_CODE (RHS (stmt)) == COMPONENT_REF
    && TREE_CODE (RHS (RHS (stmt))) == FIELD_DECL
    && strcmp ("vtable", IDENTIFIER_POINTER (DECL_NAME (
							RHS (RHS (stmt))))) == 0;
}

static bool
is_null_pointer_exception (tree stmt)
{
  if (TREE_CODE (stmt) == CALL_EXPR
    && LHS (LHS (stmt)) == soft_nullpointer_node)
    {
      gcc_assert (TREE_OPERAND (stmt, 1) == NULL);
      return true;
    }
  return false;
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
  return (TREE_CODE (stmt) == MODIFY_EXPR
      && is_pointer_type (stmt)
      && TREE_CODE (RHS (stmt)) == CALL_EXPR
      && LHS (LHS (RHS (stmt))) == soft_anewarray_node);
}

static bool
is_cast_stmt (tree stmt)
{
  return TREE_CODE (stmt) == MODIFY_EXPR 
	  && is_pointer_type (stmt) 
	  && is_suitable_decl (LHS (stmt)) 
	  && TREE_CODE (RHS (stmt)) == NOP_EXPR
	  && is_suitable_decl (LHS (RHS (stmt)));
}
static bool
is_reference_copy_stmt (tree stmt)
{
  return TREE_CODE (stmt) == MODIFY_EXPR 
	  && is_pointer_type (stmt) 
	  && is_suitable_decl (LHS (stmt)) 
	  && is_suitable_decl (RHS (stmt));
}

/* refering to pointer and reference as the same */
static bool
is_pointer_return (tree stmt)
{
  /* return stmts may have a MODIFY_EXPR, which has the type in it's
   * first operand*/
  return TREE_CODE (stmt) == RETURN_EXPR 
    && TREE_OPERAND (stmt, 0)
    && is_pointer_type (LHS (stmt));
}
static bool
is_indirect_function_call (tree stmt)
{
  return TREE_CODE (stmt) == CALL_EXPR
    && is_suitable_decl (LHS (stmt));
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
is_assignment_to_field (tree stmt)
{
  return is_pointer_type (stmt)
    && TREE_CODE (stmt) == MODIFY_EXPR
    && TREE_CODE (LHS (stmt)) == COMPONENT_REF
    && is_suitable_decl (RHS (stmt));

}

static bool
is_assignment_to_indirect_array_ref (tree stmt)
{
  return is_pointer_type (stmt)
    && TREE_CODE (stmt) == MODIFY_EXPR
    && TREE_CODE (LHS (stmt)) == ARRAY_REF
    && is_suitable_decl (RHS (stmt))
    && TREE_CODE (LHS (LHS (stmt))) == COMPONENT_REF
    && TREE_CODE (RHS (LHS (stmt))) == INTEGER_CST
    && TREE_CODE (LHS (LHS (LHS (stmt)))) == INDIRECT_REF
    && TREE_CODE (RHS (LHS (LHS (stmt)))) == FIELD_DECL
    && is_suitable_decl (LHS (LHS (LHS (LHS (stmt)))));
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

static bool
is_assignment_from_field (tree stmt)
{
  return is_pointer_type (stmt)
    && TREE_CODE (stmt) == MODIFY_EXPR
    && is_suitable_decl (LHS (stmt))
    && TREE_CODE (RHS (stmt)) == COMPONENT_REF;

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
update_connection_graph_from_pointer_arithmetic (con_graph cg, tree stmt)
{
  /* make p global, link it to q */
  /* p = q + x:
   * modify_expr
   *   var_decl - p
   *   plus_expr
   *     var_decl - q
   *     integer_cst - x
   */
  tree p_name;
  tree q_name;
  con_node p;
  con_node q;

  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);
  gcc_assert (is_suitable_decl (LHS (stmt)));
  gcc_assert (TREE_CODE (RHS (stmt)) == PLUS_EXPR);
  gcc_assert (is_suitable_decl (LHS (RHS (stmt))));
  gcc_assert (TREE_CODE (RHS (RHS (stmt))) == INTEGER_CST);

  set_stmt_type (cg, stmt, POINTER_ARITHMETIC);

  p_name = LHS (stmt);
  gcc_assert (p_name);

  p = get_existing_node (cg, p_name);
  if (p == NULL)
    p = add_local_node (cg, p_name);
  gcc_assert (p);
  p->escape = EA_GLOBAL_ESCAPE;


  q_name = LHS (RHS (stmt));
  gcc_assert (q_name);

  q = get_existing_node (cg, q_name);
  if (q == NULL)
    q = add_local_node (cg, q_name);
  gcc_assert (q);
  q->escape = EA_GLOBAL_ESCAPE;


  if (get_edge (p, q) == NULL)
    add_edge (p, q);

}

static void
update_connection_graph_from_pointer_dereference (con_graph cg, tree stmt)
{
  /* a link from q to p */
  /* p = *q:
   * modify_expr
   *   var_decl - p
   *   indirect_ref
   *     var_decl - q
   */
  tree p_name;
  tree q_name;
  con_node p;
  con_node q;

  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);
  gcc_assert (is_suitable_decl (LHS (stmt)));
  gcc_assert (TREE_CODE (RHS (stmt)) == INDIRECT_REF);
  gcc_assert (is_suitable_decl (LHS (RHS (stmt))));

  set_stmt_type (cg, stmt, POINTER_DEREFERENCE);

  p_name = LHS (stmt);
  gcc_assert (p_name);

  p = get_existing_node (cg, p_name);
  if (p == NULL)
    p = add_local_node (cg, p_name);
  gcc_assert (p);


  q_name = LHS (RHS (stmt));
  gcc_assert (q_name);

  q = get_existing_node (cg, q_name);
  if (q == NULL)
    q = add_local_node (cg, q_name);
  gcc_assert (q);


  /* from q to p this time */
  if (get_edge (q, p) == NULL)
    add_edge (q, p);

}

static void
update_connection_graph_from_cast (con_graph cg, tree stmt)
{
  /* TODO lots of killing references (due to iteration) */
  /* p = (x*) q:
   * modify_expr
   *   var_decl - p
   *   nop_expr
   *     var_decl - q
   */
  tree p_name;
  tree q_name;
  con_node p;
  con_node q;

  set_stmt_type (cg, stmt, CAST);
  /* ----------------------------------------
   *      get detail from the tree
   * ----------------------------------------*/

  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);
  gcc_assert (is_suitable_decl (LHS (stmt)));
  gcc_assert (TREE_CODE (RHS (stmt)) == NOP_EXPR);
  gcc_assert (is_suitable_decl (LHS (RHS (stmt))));

  /* get the name of the lvalue */
  p_name = LHS (stmt);
  gcc_assert (p_name);

  /* get the name of the rvalue name */
  q_name = LHS (RHS (stmt));
  gcc_assert (q_name);

  /* ----------------------------------------
   *      get the nodes from the graph
   * ----------------------------------------*/

  /* bypass is necessary because of multiple iterations */
  p = get_existing_node (cg, p_name);
  if (p)
    {
      /*bypass_node (r); */
      /* no longer SSA, so these can exist */
    }
  else
    {
      /* add the lvalue node */
      p = add_local_node (cg, p_name);
    }

  /* get the rvalue node from the graph */
  q = get_existing_node (cg, q_name);

  /* it may be that the rvalue is a parameter to the function, in
   * which case the node doesnt exist yet (there are also cases in
   * which the rhs hasnt been seen before */
  if (q == NULL)
    {
      /* add it to the graph */
      q = add_local_node (cg, q_name);
    }

  gcc_assert (q);

  if (get_edge (p, q) == NULL)
    add_edge (p, q);

  /* done REFERENCE COPY */

}

static void
update_connection_graph_from_reference_copy (con_graph cg, tree stmt)
{
  /* TODO lots of killing references (due to iteration) */
  /* p = q:
   * modify_expr
   *   parm_decl - p
   *   var_decl - q
   */
  tree p_name;
  tree q_name;
  con_node p;
  con_node q;

  set_stmt_type (cg, stmt, REFERENCE_COPY);
  /* ----------------------------------------
   *      get detail from the tree
   * ----------------------------------------*/

  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);
  gcc_assert (is_suitable_decl (LHS (stmt)));
  gcc_assert (is_suitable_decl (RHS (stmt)));

  /* get the name of the lvalue */
  p_name = LHS (stmt);
  gcc_assert (p_name);

  /* get the name of the rvalue name */
  q_name = RHS (stmt);
  gcc_assert (q_name);

  /* ----------------------------------------
   *      get the nodes from the graph
   * ----------------------------------------*/

  /* bypass is necessary because of multiple iterations */
  p = get_existing_node (cg, p_name);
  if (p)
    {
      /*bypass_node (r); */
      /* no longer SSA, so these can exist */
    }
  else
    {
      /* add the lvalue node */
      p = add_local_node (cg, p_name);
    }

  /* get the rvalue node from the graph */
  q = get_existing_node (cg, q_name);

  /* it may be that the rvalue is a parameter to the function, in
   * which case the node doesnt exist yet (there are also cases in
   * which the rhs hasnt been seen before */
  if (q == NULL)
    {
      /* add it to the graph */
      q = add_local_node (cg, q_name);
    }

  gcc_assert (q);

  if (get_edge (p, q) == NULL)
    add_edge (p, q);

  /* done REFERENCE COPY */

}

static void
update_connection_graph_from_return (con_graph cg, tree stmt)
{
  /* return u; */
  tree r_name;
  con_node r;

  set_stmt_type (cg, stmt, RETURN);

  /* not using LHS cause it isnt really a lhs */
  r_name = TREE_OPERAND (TREE_OPERAND (stmt, 0), 0);
  gcc_assert (r_name);

  r = get_existing_node (cg, r_name);
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

  if (get_edge (cg->return_node, r) == NULL)
    add_edge (cg->return_node, r);

  /* RETURN_STMT */
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

  gcc_assert (TREE_CODE (stmt) == CALL_EXPR);
  gcc_assert (is_suitable_decl (LHS (stmt)));
  gcc_assert (TREE_CODE (RHS (stmt)) == TREE_LIST);

  set_stmt_type (cg, stmt, INDIRECT_FUNCTION_CALL);

  T = LHS (stmt);
  gcc_assert (T);
  argument_list = RHS (stmt);
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

static void
update_connection_graph_from_assignment_from_array (con_graph cg, tree stmt)
{
  /* deal with this as if it were p = q; (except for killing, can't bypass
   * here */

  /* p = q[x]: 
   * modify_expr
   *   var_decl - p
   *   array_ref
   *     var_decl - q
   *     integer_cst */
  tree p_name;
  tree q_name;
  con_node p;
  con_node q;

  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);
  gcc_assert (is_suitable_decl (LHS (stmt)));
  gcc_assert (TREE_CODE (RHS (stmt)) == ARRAY_REF);
  gcc_assert (is_suitable_decl (LHS (RHS (stmt))));
  gcc_assert (TREE_CODE (RHS (RHS (stmt))) == INTEGER_CST);

  set_stmt_type (cg, stmt, ASSIGNMENT_FROM_ARRAY);

  p_name = LHS (stmt);
  gcc_assert (p_name);

  p = get_existing_node (cg, p_name);
  if (p == NULL)
    p = add_local_node (cg, p_name);
  gcc_assert (p);


  q_name = LHS (RHS (stmt));
  gcc_assert (q_name);

  q = get_existing_node (cg, q_name);
  if (q == NULL)
    q = add_local_node (cg, q_name);
  gcc_assert (q);

  if (get_edge (p, q) == NULL)
    add_edge (p, q);

}

static void
update_connection_graph_from_assignment_to_indirect_array_ref (con_graph cg, tree stmt)
{
  /* this is a mix of assignment to field, and assignment to array */
  /* p.f[x] = q:
   * modify_expr
   *   array_ref
   *     component_ref
   *       indirect_ref
   *         var_decl - p
   *       field_decl - f
   *     integer_cst - x
   *   var_decl
   */
  tree p_name;
  tree f_name;
  tree q_name;
  tree lhs;
  tree indirect_ref;
  con_node p;
  con_node q;
  con_node u;
  con_node v;
  con_node terminal;
  con_node phantom;

  set_stmt_type (cg, stmt, ASSIGNMENT_TO_INDIRECT_ARRAY_REF);
  /* ----------------------------------------
   *      get detail from the tree
   * ----------------------------------------*/


  /* get the lvalue reference and its field */
  /* get to the field ref and ignore the array */
  lhs = LHS (TREE_OPERAND (stmt, 0));
  indirect_ref = TREE_OPERAND (lhs, 0);
  p_name = TREE_OPERAND (indirect_ref, 0);
  f_name = TREE_OPERAND (lhs, 1);

  gcc_assert (p_name);
  gcc_assert (f_name);

  /* get the rvalue */
  q_name = TREE_OPERAND (stmt, 1);
  gcc_assert (q_name);


  /* ----------------------------------------
   *      get the nodes from the graph
   * ----------------------------------------*/

  p = get_existing_node (cg, p_name);
  gcc_assert (p);

  /* note that we dont bypass here, as Choi99 says we can't
   * kill p.f.  Not sure why though... */

  q = get_existing_node (cg, q_name);
  gcc_assert (q);

  /* if u doesnt point to anything, (say it's passed in as a
   * parameter), create phantom object nodes for it */
  u = get_points_to (p);
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
      phantom = get_existing_node (cg, stmt); /* 1-limited */
      if (phantom == NULL)
	{
	  con_node field;
	  phantom = add_object_node (cg, stmt, TREE_TYPE (indirect_ref));
	  phantom->phantom = true;

	  /* add a field node for it */
	  field = add_field_node (cg, f_name);
	  field->phantom = true;
	  add_edge (phantom, field);
	}
      gcc_assert (phantom->phantom);

      /* add the phantom the the list of u */
      phantom->next_link = u;
      u = phantom;
    }

  /* make the terminal nodes point to the node */
  while (terminal)
    {
      if (terminal != phantom) /* this would be pointless otherwise */
	  add_edge (terminal, phantom);

      NEXT_LINK_CLEAR (terminal);
    }

  /* get the field nodes of the pointed-to objects */
  v = get_field_nodes (u, f_name);
  gcc_assert (v != NULL); /* even phantom nodes have this field */

  /* do not bypass, this isnt a killing assignment */


  /* add deferred edges */
  while (v)
    {
      if (get_edge (v, q) == NULL)
	add_edge (v, q);

      NEXT_LINK_CLEAR (v);
    }

  assert_all_next_link_free (cg);


  /* done ASSIGNMENT TO FIELD */
}

static void
update_connection_graph_from_assignment_to_field (con_graph cg, tree stmt)
{
  /* TODO look at 'may definitions' to expand this */
  /* p.f = q */
  tree p_name;
  tree f_name;
  tree q_name;
  tree lhs;
  tree indirect_ref;
  con_node p;
  con_node q;
  con_node u;
  con_node v;
  con_node terminal;
  con_node phantom;

  set_stmt_type (cg, stmt, ASSIGNMENT_TO_FIELD);
  /* ----------------------------------------
   *      get detail from the tree
   * ----------------------------------------*/


  /* get the lvalue reference and its field */
  lhs = TREE_OPERAND (stmt, 0);
  indirect_ref = TREE_OPERAND (lhs, 0);
  p_name = TREE_OPERAND (indirect_ref, 0);
  f_name = TREE_OPERAND (lhs, 1);

  gcc_assert (p_name);
  gcc_assert (f_name);

  /* get the rvalue */
  q_name = TREE_OPERAND (stmt, 1);
  gcc_assert (q_name);


  /* ----------------------------------------
   *      get the nodes from the graph
   * ----------------------------------------*/

  p = get_existing_node (cg, p_name);
  gcc_assert (p);

  /* note that we dont bypass here, as Choi99 says we can't
   * kill p.f.  Not sure why though... */

  q = get_existing_node (cg, q_name);
  gcc_assert (q);

  /* we dont exactly follow the recommendations from Choi99. In
   * particular, the paper alleges that you only use phantom nodes if
   * there are no points to nodes, but in reality, there can be both at
   * the same time.  */
  u = get_points_to (p);
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
      phantom = get_existing_node (cg, stmt); /* 1-limited */
      if (phantom == NULL)
	{
	  con_node field;
	  phantom = add_object_node (cg, stmt, TREE_TYPE (indirect_ref));
	  phantom->phantom = true;

	  /* add a field node for it */
	  gcc_assert (get_existing_field_node (cg, f_name, phantom) == NULL);
	  field = add_field_node (cg, f_name);
	  field->phantom = true;
	  add_edge (phantom, field);
	}
      gcc_assert (phantom->phantom);

      /* add the phantom the the list of u */
      gcc_assert (phantom->next_link == NULL);
      phantom->next_link = u;
      u = phantom;
    }

  /* make the terminal nodes point to the node */
  while (terminal)
    {
      if (terminal != phantom) /* this would be pointless otherwise */
	  add_edge (terminal, phantom);

      NEXT_LINK_CLEAR (terminal);
    }

  /* get the field nodes of the pointed-to objects */
  v = get_field_nodes (u, f_name);
  gcc_assert (v != NULL); /* even phantom nodes have this field */

  
  /* do not bypass, this isnt a killing assignment */


  /* add deferred edges */
  while (v)
    {
      if (get_edge (v, q) == NULL)
	add_edge (v, q);

      NEXT_LINK_CLEAR (v);
    }

  assert_all_next_link_free (cg);


  /* done ASSIGNMENT TO FIELD */
}

static void
update_connection_graph_from_assignment_from_field (con_graph cg, tree stmt)
{
  /* p = q.f */
  tree p_name;
  tree rhs;
  tree indirect_ref;
  tree q_name;
  tree f_name;
  con_node p;
  con_node q;
  con_node u;
  con_node v;
  con_node terminal;
  con_node phantom;

  set_stmt_type (cg, stmt, ASSIGNMENT_FROM_FIELD);

  /* ----------------------------------------
   *      get detail from the tree
   * ----------------------------------------*/

  /* get the id of the lvalue */
  p_name = TREE_OPERAND (stmt, 0);
  gcc_assert (p_name);

  /* get the id of the rvalue reference */
  rhs = TREE_OPERAND (stmt, 1);
  indirect_ref = TREE_OPERAND (rhs, 0);
  q_name = TREE_OPERAND (indirect_ref, 0);
  gcc_assert (q_name);

  /* get the field name - Note that the field name is shared
   * by all the objects containing this field */
  f_name = TREE_OPERAND (rhs, 1);
  gcc_assert (f_name);


  /* ----------------------------------------
   *      get the nodes from the graph
   * ----------------------------------------*/


  /* if the source exists, bypass it */
  p = get_existing_node (cg, p_name);
  if (p)
    {
      /* no longer SSA, so these can exist */
      /*bypass_node (p); */
    }
  else
    {
      /* add the lhs node */
      p = add_local_node (cg, p_name);
    }
  gcc_assert (p);


  q = get_existing_node (cg, q_name);

  /* it may be that the rvalue is a parameter to this function, in
   * which case the node doesnt exist yet */
  if (q == NULL)
    {
      /* add it to the graph */
      q = add_local_node (cg, q_name);
      q->phantom = true;
    }


  /* get the objects pointed to be q */
  u = get_points_to (q);
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
      phantom = get_existing_node (cg, stmt); /* 1-limited */
      if (phantom == NULL)
	{
	  con_node field;
	  phantom = add_object_node (cg, stmt, TREE_TYPE (indirect_ref));
	  phantom->phantom = true;

	  /* add a field node for it */
	  field = add_field_node (cg, f_name);
	  field->phantom = true;
	  add_edge (phantom, field);
	}
      gcc_assert (phantom->phantom);

      /* add the phantom the the list of u */
      phantom->next_link = u;
      u = phantom;
    }

  /* make the terminal nodes point to the node */
  while (terminal)
    {
      if (terminal != phantom) /* this would be pointless otherwise */
	  add_edge (terminal, phantom);

      NEXT_LINK_CLEAR (terminal);
    }

  /* get the field nodes of the pointed-to objects */
  v = get_field_nodes (u, f_name);
  gcc_assert (v != NULL); /* even phantom nodes have this field */

  /* TODO check for flow-sensitive */

  /* its important to bypass late, otherwise a = a.b will
   * fail */

  /* bypass_node (p); */

  /* add deferred edges */
  while (v)
    {
      if (get_edge (p, v) == NULL)
	add_edge (p, v);

      NEXT_LINK_CLEAR (v);
    }

  assert_all_next_link_free (cg);

  /* done ASSIGNMENT FROM FIELD */

}

static void
update_connection_graph_from_assignment_from_vtable (con_graph cg, tree stmt)
{
  /* take the lhs, make it global, add an edge to the rhs */

  /* p = q.vtable:
   * modify_expr
   *   var_decl - p
   *   component_ref
   *     indirect_ref
   *       var_decl - q
   *     field_decl - vtable
   */

  /* note that we dont need access to the field nodes, so theres no need
   * to go searching for objects corresponding to them, here */
  tree p_name;
  tree q_name;
  con_node p;
  con_node q;

  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);
  gcc_assert (is_suitable_decl (LHS (stmt)));
  gcc_assert (TREE_CODE (RHS (stmt)) == COMPONENT_REF);
  gcc_assert (TREE_CODE (LHS (RHS (stmt))) == INDIRECT_REF);
  gcc_assert (TREE_CODE (RHS (RHS (stmt))) == FIELD_DECL);
  gcc_assert (is_suitable_decl (LHS (LHS (RHS (stmt)))));

  set_stmt_type (cg, stmt, ASSIGNMENT_FROM_VTABLE);

  /* lhs */
  p_name = LHS (stmt);
  gcc_assert (p_name);

  p = get_existing_node (cg, p_name);
  if (p == NULL)
    {
      p = add_local_node (cg, p_name);
    }
  p->escape = EA_GLOBAL_ESCAPE;

  /* get the object */
  q_name = LHS (LHS (RHS (stmt)));
  gcc_assert (q_name);
  q = get_existing_node (cg, q_name);
  if (q == NULL)
    {
      q = add_local_node (cg, q_name);
    }

  if (get_edge (p, q) == NULL)
    add_edge (p, q);

}

static void
update_connection_graph_from_statement (con_graph cg, tree stmt)
{
  gcc_assert (cg);
  gcc_assert (stmt);

  /* recognize all the statement types, and update the connection
   * graph accordingly */
  if (is_memory_allocation (stmt))
    update_connection_graph_from_memory_allocation (cg, stmt);

  else if (is_cast_stmt (stmt))
    update_connection_graph_from_cast (cg, stmt);

  else if (is_reference_copy_stmt (stmt))
    update_connection_graph_from_reference_copy (cg, stmt);

  else if (is_pointer_return (stmt))
    update_connection_graph_from_return (cg, stmt);

  else if (is_indirect_function_call (stmt))
    update_connection_graph_from_indirect_function_call (cg, stmt);

  /* this goes before function call */
  else if (is_null_pointer_exception (stmt))
    {
      set_stmt_type (cg, stmt, IGNORED_NULL_POINTER_EXCEPTION);
      /* dont need to do anything, as it doesnt reference any var */
    }

  else if (is_array_allocation (stmt))
    {
      update_connection_graph_from_array_allocation (cg, stmt);
    }

  else if (is_function_call (stmt)
	   || is_function_call_with_return_value (stmt)
	   || is_constructor (stmt))
    update_connection_graph_from_function_call (cg, stmt);

  /* this must come before assignment_to_field, or it wont hit */
  else if (is_assignment_from_vtable (stmt))
    update_connection_graph_from_assignment_from_vtable (cg, stmt);

  else if (is_assignment_to_field (stmt))
    update_connection_graph_from_assignment_to_field (cg, stmt);
    
  else if (is_assignment_from_field (stmt))
    update_connection_graph_from_assignment_from_field (cg, stmt);
    
  else if (is_assignment_to_null (stmt))
    {
      /* TODO this becomes relevent in flow-sensitive */
      set_stmt_type (cg, stmt, IGNORED_ASSIGNMENT_TO_NULL);

      /* Done ASSIGNMENT TO NULL */
    }
  else if (is_assignment_from_array (stmt))
    update_connection_graph_from_assignment_from_array (cg, stmt);

  else if (is_assignment_to_indirect_array_ref (stmt))
    update_connection_graph_from_assignment_to_indirect_array_ref (cg, stmt);

  else if (is_assignment_to_array (stmt))
      /* TODO */ gcc_unreachable ();
    
  else if (is_pointer_dereference (stmt))
    update_connection_graph_from_pointer_dereference (cg, stmt);

  else if (is_pointer_arithmetic (stmt))
    update_connection_graph_from_pointer_arithmetic (cg, stmt);

  else
    {
      if (TREE_CODE (stmt) == RETURN_EXPR
	  && VOID_TYPE_P (TREE_TYPE (stmt)))
	{
	  set_stmt_type (cg, stmt, IGNORED_RETURNING_VOID);
	}

      if (! is_pointer_type (stmt))
	{
	  set_stmt_type (cg, stmt, IGNORED_NOT_A_POINTER);
	}

      if (TREE_CODE (stmt) == LABEL_EXPR)
	{
	  set_stmt_type (cg, stmt, IGNORED_LABEL_EXPR);
	}

      if (TREE_CODE (stmt) == COND_EXPR)
	{
	  set_stmt_type (cg, stmt, IGNORED_COND_EXPR);
	}

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
  tree type, alloca_func_call, new_alloca_stmt;
  tree new_jv_func_call_stmt;

  tree stmt, call_expr, addr_expr, func_decl, lhs, parms, klass;
  tree alloca_func, args, parm1;

  tree object_pointer_pointer_type_node, obj, size_obj, indirect_ref, array_object_pointer_type_node, data_field;
  tree array_type, component_ref, array_ref, int0, int1, int2, rhs, object_pointer_type_node;
  tree elements_stmt, nop_expr, cast_stmt, minused, minus_stmt, four;
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
  object_pointer_pointer_type_node = build_pointer_type (object_pointer_type_node);
  obj = create_tmp_var (object_pointer_pointer_type_node, "D");
  DECL_ARTIFICIAL (parm1) = 1;

  /* size_t size = (size_t) elements (obj) */
  /*   size_t size = &obj->data[0] */
  size_obj = create_tmp_var (object_pointer_pointer_type_node, "D");
  DECL_ARTIFICIAL (parm1) = 1;

  /* indirect ref */
  indirect_ref = build1 (INDIRECT_REF, build_pointer_type (array_object_pointer_type_node), obj);

  /* field decl */
  data_field = lookup_field (&array_type, get_identifier ("data"));

  /* make the component ref */
  array_object_pointer_type_node = build_array_type (object_ptr_type_node, one_elt_array_domain_type);
  component_ref = build2 (COMPONENT_REF, array_object_pointer_type_node, indirect_ref, data_field);

  /* make the array ref */
  array_ref = build1 (ARRAY_REF, object_pointer_type_node, component_ref);

  /* make the rhs */
  int0 = build_int_cst (NULL_TREE, 0);
  int1 = build_int_cst (NULL_TREE, 0);
  int2 = build_int_cst (NULL_TREE, 1);
  rhs = build4 (ADDR_EXPR, object_pointer_pointer_type_node, array_ref,
		int0, int1, int2);

  /* combine */
  elements_stmt = build2 (MODIFY_EXPR, object_pointer_pointer_type_node,
			  size_obj, rhs);

  /* cast_size = (size_t) size; */
  nop_expr = build1 (NOP_EXPR, size_type_node, size_obj);
  cast_size_obj = create_tmp_var (size_type_node, "D");
  cast_stmt = build2 (MODIFY_EXPR, size_type_node, cast_size_obj,
		      nop_expr);


  /* minused = 2147483646 - cast_size */ 
  minused = create_tmp_var (size_type_node, "D");
  minus_expr = build2 (MINUS_EXPR, size_type_node, decimal_int_max, cast_size_obj);
  minus_stmt = build2 (MODIFY_EXPR, size_type_node, minused, minus_expr);
  

  /* count_size = count * sizeof (jobject) */
  count_size = create_tmp_var (size_type_node, "D");
  four = build_int_cst (NULL_TREE, 4);
  /* TODO find real size - where does 4 come from */
  mult_expr = build2 (MULT_EXPR, size_type_node, cast_size_obj, four);
  mult_stmt = build2 (MODIFY_EXPR, size_type_node, count_size, mult_expr);

  /* size += count_size */
  total_size = create_tmp_var (size_type_node, "D");
  plus_expr = build2 (PLUS_EXPR, size_type_node, count_size, minused);
  total_stmt = build2 (MODIFY_EXPR, size_type_node, total_size, plus_expr);

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
  bsi_insert_before (&bsi, cast_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, minus_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, mult_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, total_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, new_alloca_stmt, BSI_SAME_STMT); 
  bsi_insert_before (&bsi, new_jv_func_call_stmt, BSI_SAME_STMT); 
  bsi_remove(&bsi, true);
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
  args = tree_cons (NULL_TREE, build_int_cst (NULL_TREE, 128), NULL_TREE);

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

  allocators[0] = alloc_no_finalizer_node;
  allocators[1] = soft_anewarray_node;

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

#endif /* UNHIDE_IT */

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

  debug = fopen ("testing/debug_log", "w");
  dump = fopen ("testing/dump_log", "w");

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
	  cg = new_con_graph (concat ("testing/graphs/", current_function_name (),
				      (IDENTIFIER_POINTER
				       (DECL_ASSEMBLER_NAME
					(current_function_decl))), ".graph",
				      NULL), current_function_decl,
			      last_cg);

	  /* fix up the next_cg list */
	  last_cg = cg;

	  /* make it cyclic */
	  if (first_cg == NULL)
	    {
	      first_cg = cg;
	      first_cg->next = cg;
	    }
	  else
	    {
	      first_cg->next = cg;
	    }

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

