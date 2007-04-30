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
#include "domwalk.h"
#include "except.h"
#include "congraph.h"
#include "java/java-tree.h"

//#define DUMP_BB_GRAPHS
/*
 * TODO remove terminal nodes incomming deferred edges according to
 * toplas03, Sect 4.2
 * TODO you must propagate before you bypass
 */

void init_gcj_stack_allocate (void);
void devirt (void);
void gcj_invariants (void);
void register_dump_files (struct tree_opt_pass*, bool, int);

static bool ea_interprocedural;
static bool ea_flow_sensitive;

static int iteration_count;
static bool current_block_is_final;

void xprintf (const char* format_string, ...);

__attribute__((__format__(__printf__, 1, 2))) void xprintf (const char* format_string, ...)
{
  if (dump_file == NULL)
    return;

  va_list argp;
  va_start(argp, format_string);

  vfprintf (dump_file, format_string, argp);
  va_end(argp);
}


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

bool
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
  xprintf ("\n");
  if (dump_file) fflush (dump_file);
}

void
tf (tree stmt, bool newline)
{
  xprintf ("[");
  if (dump_file) print_generic_expr (dump_file, stmt, 0);
  xprintf ("]");
  if (newline) nl ();
}

void
df (con_node node)
{
  if (node->id)
    tf (node->id, false);
  else 
    xprintf ("DECL.%d", node->uid);
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

static bool
is_array_type (tree decl)
{
  tree type;
  return (SSA_VAR_P (decl)
      && (type = TREE_TYPE (decl))
      && TREE_CODE (type) == ARRAY_TYPE);
}

static bool 
is_null (tree rhs)
{
  return TREE_CODE (rhs) == INTEGER_CST
    && has_pointer_type (rhs)
    && TREE_INT_CST_HIGH (rhs) == 0
    && TREE_INT_CST_LOW (rhs) == 0;
}

static bool 
is_rhs (tree rhs)
{
  return SSA_VAR_P (rhs) || is_null (rhs);
}

bool is_global (tree);

bool
is_global (tree var)
{
  /* Non globals should have SSA_NAMES */
  /* TODO parameters can be VAR_DECLS, no? */
  if (TREE_CODE (var) == SSA_NAME)
    return false;

  if (TREE_CODE (var) == ADDR_EXPR
    && TREE_CODE (LHS (var)) == FUNCTION_DECL
    && METHOD_STATIC (LHS (var)))
    return true;

  if (TREE_CODE (var) == EXC_PTR_EXPR)
    return false;

  if (TREE_CODE (var) == INTEGER_CST)
    return false;

  if (FIELD_STATIC (var))
    return true;

  if (DECL_CONTEXT (var) == NULL)
//      || DECL_CONTEXT (var) == TRANSLATION_UNIT_DECL)
    return true;


  gcc_unreachable ();
}

/* -------------------------------------------------------------*
 *		 identifying parts of a statement		*
 * -------------------------------------------------------------*/
static bool
is_function_constructor (tree decl)
{
  gcc_assert (TREE_CODE (decl) == FUNCTION_DECL);
  return get_identifier ("<init>") == DECL_NAME (decl);
}

/* this is supposed to work in theory, but doesnt
static bool
is_memory_allocation (tree stmt)
{
	return TREE_CODE (stmt) == CALL_EXPR && (call_expr_flags (stmt) & (ECF_MALLOC || ECF_MAY_BE_ALLOCA));
}
*/

/* -------------------------------------------------------------*
 *	 inserting statements to the connection graph		*
 * -------------------------------------------------------------*/

static void
update_caller_nodes (con_graph caller, con_graph callee, tree call_id)
{
  int i, j, k;
  tree parameters = DECL_ARGUMENTS (callee->function);
  tree arguments = TREE_OPERAND (call_id, 1); /* These are in the caller */
  con_node f, node;

  /* Update by following the parameters */
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
//	  caller_node->type = CALLER_ACTUAL_NODE;
	  con_node callee_node = get_existing_node (callee, parameters);

	  if (!is_null (argument))
	    {
	      gcc_assert (caller_node && callee_node);
	      update_nodes (callee_node, caller_node, call_id);
	    }
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
//      caller_node->type = CALLER_ACTUAL_NODE;
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


/* -------------------------------------------------------------*
 * extracting trees from statements and calling insertions	*
 * -------------------------------------------------------------*/
static void
do_assignment (con_node p, con_node q)
{
  gcc_assert (p && q);
  gcc_assert (p->type != RETURN_NODE && q->type != RETURN_NODE);

  /* Is it a static field? */
  if (ea_flow_sensitive && !is_global (p->id))
    {
      /* if theres no outgoing edges from p, this isnt really a killing
       * def, but it does change the graph */
      if (add_killing_edge (p, q))
	xprintf ("Killing def\n");
      else
	xprintf ("Killing def changes nothing\n");
    }
  else
    {
      if (get_edge (p, q))
	xprintf ("Non-killing def changes nothing\n");
      else
	{
	  xprintf ("Non-killing def adds edge\n");
	  add_edge (p, q);
	}
    }
}

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

static tree
get_allocation_type (tree call_expr)
{
  tree type = TREE_TYPE (call_expr);
  while (TREE_TYPE (type))
    type = TREE_TYPE (type);

  return type;
}

static bool
is_global_memory_allocation (tree stmt, tree* p_name, tree *type)
{
  tree call_expr, addr_expr, func_decl;
  if (!(GIMPLE_STMT_P (stmt) 
	&& has_pointer_type (stmt) 
	&& SSA_VAR_P (*p_name = LHS (stmt)) 
	&& TREE_CODE (call_expr = RHS (stmt)) == CALL_EXPR
	&& TREE_CODE (addr_expr = LHS (call_expr)) == ADDR_EXPR
	&& TREE_CODE (func_decl = LHS (addr_expr)) == FUNCTION_DECL
	&& func_decl == alloc_object_node))
    return false;

  *type = get_allocation_type (get_call_expr_in (stmt));
  return true;
}

static bool
is_memory_allocation (tree stmt, tree* p_name, tree* type)
{
  tree call_expr, addr_expr, func_decl, arguments;
  if (!(GIMPLE_STMT_P (stmt) 
	&& has_pointer_type (stmt) 
	&& SSA_VAR_P (*p_name = LHS (stmt)) 
	&& TREE_CODE (call_expr = RHS (stmt)) == CALL_EXPR
	&& (arguments = TREE_OPERAND (call_expr, 1))
	&& TREE_CODE (arguments) == TREE_LIST
	&& TREE_CODE (addr_expr = LHS (call_expr)) == ADDR_EXPR
	&& TREE_CODE (func_decl = LHS (addr_expr)) == FUNCTION_DECL
	&& (func_decl == alloc_no_finalizer_node
	    || func_decl == soft_anewarray_node
	    || func_decl == soft_multianewarray_node
	    || func_decl == soft_newarray_node)))
    return false;

  *type = get_allocation_type (get_call_expr_in (stmt));
  return true;
}

static bool
is_runnable (tree type)
{
  if (!POINTER_TYPE_P (type))
    return false;

  tree thread = lookup_class (get_identifier ("java.lang.Thread"));
  if (inherits_from_p (type, thread))
    return true;

  /* inherits from and interface_of use params in opposite order */
  tree runnable = lookup_class (get_identifier ("java.lang.Runnable"));
  if (interface_of_p (runnable, type))
    return true;

  return false;
}

static void
do_memory_allocation (con_graph cg, tree p_name, tree obj_name, tree type)
{
  con_node p = get_existing_node (cg, p_name);
  con_node obj_node = get_existing_node (cg, obj_name);

  /* They're both NULL or neither is */
  gcc_assert (!((p == NULL) ^ (obj_node == NULL)));
  xprintf ("New object\n");

  if (!p)
    {
      p = add_local_node (cg, p_name);
      obj_node = add_object_node (cg, obj_name);
      if (is_runnable (type))
	obj_node->escape = EA_GLOBAL_ESCAPE;
    }
  else { /* On a second iteration, this can come from a backedge */ }

  do_assignment (p, obj_node);
}


/* Has a finalizer */
static void
do_global_memory_allocation (con_graph cg, tree p_name, tree obj_name, tree type)
{
  do_memory_allocation (cg, p_name, obj_name, type);
  con_node object = get_existing_node (cg, obj_name);
  object->escape = EA_GLOBAL_ESCAPE;
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

/* This array is a global array, referring probably to a string pool. Make
 * it global so that the phantom object to which it refers is also made
 * global. */
static bool
is_assignment_from_global_array (tree stmt, tree *p_name, tree *q_name)
{
  tree array_ref;
  return (GIMPLE_STMT_P (stmt)
    && has_pointer_type (stmt)
    && SSA_VAR_P (*p_name = LHS (stmt))
    && TREE_CODE (array_ref = RHS (stmt)) == ARRAY_REF
    && TREE_CODE (*q_name = LHS (array_ref)) == VAR_DECL
    && TREE_CODE (RHS (array_ref)) == INTEGER_CST);
}

static bool 
is_function_cast (tree stmt, tree* p_name, tree* q_name)
{
  return GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt) 
	&& is_gimple_cast (RHS (stmt))
	&& SSA_VAR_P (*p_name = LHS (stmt)) 
	&& TREE_CODE (*q_name = LHS (RHS (stmt))) == ADDR_EXPR
	&& TREE_CODE (LHS (*q_name)) == FUNCTION_DECL;
}

static bool 
is_reference_cast (con_graph cg, tree stmt, tree* p_name, tree* q_name)
{
  return GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt) 
	&& is_gimple_cast (RHS (stmt))
	&& SSA_VAR_P (*p_name = LHS (stmt)) 
	&& SSA_VAR_P (*q_name = LHS (RHS (stmt)))
	&& get_existing_node (cg, (*q_name)); /* We can end up casting from
						vtable offsets, which we
						dont add nodes for */
}

static bool
is_reference_copy (tree stmt, tree* p_name, tree* q_name)
{
  return (GIMPLE_STMT_P (stmt) 
	&& has_pointer_type (stmt)
	&& SSA_VAR_P (*p_name = LHS (stmt)) 
	&& is_rhs (*q_name = RHS (stmt)));
}

static void
do_reference_copy (con_graph cg, tree p_name, tree q_name)
{
  con_node p = get_existing_node (cg, p_name);
  con_node q = get_existing_node (cg, q_name);
  if (!q)
    {
      if (is_null (q_name))
	q = add_null_node (cg, q_name);
      else
	{
	  gcc_assert (is_global (q_name));
	  q = add_global_node (cg, q_name);
	}
    }
  gcc_assert (q);

  xprintf ("New reference (copy)\n");
  if (p) 
    do_assignment (p, q);
  else
    {
      p = add_local_node (cg, p_name);
      add_edge (p, q);
    }
}

static void
do_assignment_from_global_array (con_graph cg, tree p_name, tree q_name)
{
  con_node p = get_existing_node (cg, p_name);
  con_node q = get_existing_node (cg, q_name);
  gcc_assert (q);
  if (p)
    do_assignment (p, q);
  else
    {
      p = add_local_node (cg, p_name);
      add_edge (p, q);
    }
  gcc_assert (q->escape == EA_GLOBAL_ESCAPE);
}

static void
do_return_stmt (con_graph cg, tree result_decl, tree q_name)
{
  xprintf ("Return stmt\n");
  con_node p = get_existing_node (cg, result_decl);
  if (!p) p = add_local_node (cg, result_decl);
  do_reference_copy (cg, result_decl, q_name);
  add_edge (cg->return_node, p);
}

static bool
is_function_call (tree stmt, tree* function, tree* arguments, tree* return_value)
{
  tree a, f, r;
  tree call_expr;
  if (!GIMPLE_STMT_P (stmt) && TREE_CODE (stmt) != CALL_EXPR)
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
	  a = TREE_OPERAND (call_expr, 1); /* arguments can be null */
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
is_throw (tree stmt, tree* p_name)
{
  tree function;
  tree arguments;
  return is_function_call (stmt, &function, &arguments, NULL)
    && DECL_NAME (function) == get_identifier ("_Jv_Throw")
    && arguments
    && TREE_CHAIN (arguments) == NULL
    && SSA_VAR_P (*p_name = TREE_VALUE (arguments));
}

/* Stores the connection graphs for basic blocks, indexed by basic block. */
static con_graph* bb_graphs;

/* This probably isnt right */
static void
do_throw (con_graph cg, tree p_name)
{
  con_node p = get_existing_node (cg, p_name);
  gcc_assert (p);
  set_escape_state (p, EA_ARG_ESCAPE);

  // merge now, not at the end of the block
  merge_into (bb_graphs [EXIT_BLOCK], cg); 
}

static bool 
is_check_cast (tree stmt, tree* p_name, tree* q_name)
{
  tree function;
  tree arguments;
  tree return_value;
  return is_function_call (stmt, &function, &arguments, &return_value)
    && DECL_NAME (function) == get_identifier ("_Jv_CheckCast")
    && SSA_VAR_P (*p_name = return_value)
    && arguments
    && TREE_CHAIN (arguments) /* interested in argument 1 */
    && (*q_name = TREE_VALUE (TREE_CHAIN (arguments)));
}



static bool
is_special_function_call (tree stmt)
{
  tree function;
  if (is_function_call (stmt, &function, NULL, NULL))
    {
      tree identifier = DECL_NAME (function);
      if (identifier == get_identifier ("_Jv_InitClass")
	  || identifier == get_identifier ("_Jv_ThrowBadArrayIndex")
	  || identifier == get_identifier ("_Jv_ThrowNullPointerException")
	  || identifier == get_identifier ("_Jv_CheckArrayStore")
	  || identifier == get_identifier ("_Jv_MonitorEnter")
	  || identifier == get_identifier ("_Jv_MonitorExit"))
	{
	  xprintf ("Ignoring call to %s\n", 
		   IDENTIFIER_POINTER (identifier));
	  return true;
	}
    }
  return false;
}

static void
do_clone (con_graph cg, tree p_name, tree q_name, tree phantom_id)
{
  con_node p = get_existing_node (cg, p_name);
  con_node q = get_existing_node (cg, q_name);
  con_node obj;
  int i;
  gcc_assert (p && q);

  // for each field of q, add a field to p and point it to the q field
  con_node_vec u1 = get_points_to_and_terminals (cg, q, phantom_id);
  con_node_vec u2 = get_points_to_and_terminals (cg, p, phantom_id);
  gcc_assert (VEC_length (con_node, u2) == 1);

  // get the list of field nodes
  for (i = 0; VEC_iterate (con_node, u1, i, obj); i++)
    {
      con_edge edge;
      for (edge = obj->out; edge; edge = edge->next_out)
	{
	  con_node v1 = edge->target;
	  con_node v2 = get_field_nodes (u2, v1->id);
	  gcc_assert (link_length (v2) == 1);
	  add_edge (v2, v1);
	}
    }
}

static bool
is_clonable (tree type)
{
  tree cloneable = lookup_class (get_identifier ("java.lang.Cloneable"));
  return inherits_from_p (cloneable, type) 
    || interface_of_p (type, cloneable);
}

static void
do_function_call (con_graph cg, tree function, tree arguments, tree return_value, tree call_id)
{
  /* Check the function doesnt belong to java.lang.Object */
  /* TODO move these into the special category */
  if (DECL_CONTEXT (function) == object_type_node)
    {
      /* Object () does nothing, and clone () causes an exception to be
       * thrown. */
      gcc_assert (DECL_NAME (function) == get_identifier ("<init>")
		  || DECL_NAME (function) == get_identifier ("clone")
		  || DECL_NAME (function) == get_identifier ("equals")
		  || DECL_NAME (function) == get_identifier ("finalize")
		  || DECL_NAME (function) == get_identifier ("getClass")
		  || DECL_NAME (function) == get_identifier ("hashCode")
		  || DECL_NAME (function) == get_identifier ("notify")
		  || DECL_NAME (function) == get_identifier ("notifyAll")
		  || DECL_NAME (function) == get_identifier ("toString")
		  || DECL_NAME (function) == get_identifier ("wait"));

      /* wait, equals, notify, notifyAll, hashCode and <init> don't affect
       * the graph */

      /* finalize makes the node global */
      if (DECL_NAME (function) == get_identifier ("finalize"))
	{
	  tree argument = TREE_VALUE (arguments);
	  con_node node = get_existing_node (cg, argument);
	  set_escape_state (node, EA_GLOBAL_ESCAPE);
	  xprintf ("Call to finalize\n");
	  return;
	}

      /* Clone causes an exception to be thrown, so no need to create
       * memory for it. However, the analysis will be confused if we dont
       * add a node for it */
      /* Get class returns a global field, who's memory isnt held by the
       * object */
      /* toString allocates memory, but we ignore it */
      con_node return_node;
      if (return_value 
	  && (DECL_NAME (function) == get_identifier ("clone")
	      || DECL_NAME (function) == get_identifier ("toString")
	      || DECL_NAME (function) == get_identifier ("getClass")))
	return_node = add_local_node (cg, return_value);

      /* If the object implements clonable, or is an array, then clone it.
       * Otherwise throw an exception */
      if (return_node
	  && DECL_NAME (function) == get_identifier ("clone"))
	{
	  if (is_clonable (TREE_VALUE (arguments)))
	    {
	      xprintf ("Call to Object->clone ()\n");
	      // copy all the fields of the two objects
	      do_clone (cg, return_value, TREE_VALUE (arguments), call_id);
	    }
	  else
	    {
	      xprintf ("Exception being thrown\n");
	      merge_into (bb_graphs [EXIT_BLOCK], cg); 
	    }
	}
      else
	{
	  /* TODO toString might represent memory allocations */
	  xprintf ("Ignoring call on Object ()\n");
	}

      return;
    }

  if (return_value)
    {
      if (get_existing_node (cg, return_value) == NULL)
	add_local_node (cg, return_value);
    }

  /* update caller nodes for the constructors (at a minimum) */
  /* TODO do this for other function calls */
  con_graph callee_cg = get_cg_for_function (function);
  if (callee_cg)
    {
      if (ea_interprocedural || is_function_constructor (function))
	{
	  xprintf ("Updating graph with callee graph\n");
	  update_caller_nodes (cg, callee_cg, get_call_expr_in (call_id));
	}
    }
  else
    {
      xprintf ("MISSED OPPORTUNITY: No callee graph\n");
      /* Make all the nodes global escape */
      tree argument_list = arguments;
      while (argument_list)
	{
	  tree argument = TREE_VALUE (argument_list);
	  if (TREE_CODE (argument) == ADDR_EXPR)
	    argument = LHS (argument);

	  if (has_pointer_type (argument))
	    {
	      con_node arg_node = get_existing_node (cg, argument);
	      if (arg_node)
		arg_node->escape = EA_GLOBAL_ESCAPE;
	    }
	  argument_list = TREE_CHAIN (argument_list);
	}
    }
}

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

/* Its important to remember that an array's data field represents memory
 * aswell */
static bool
is_assignment_from_array (tree stmt, tree *p_name, tree *q_name, tree *f_name)
{
  tree array_ref;
  return GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt)
	&& SSA_VAR_P (*p_name = LHS (stmt))
	&& TREE_CODE (array_ref = RHS (stmt)) == ARRAY_REF
	&& is_field_ref (LHS (array_ref), q_name, f_name)
	&& SSA_VAR_P (*q_name)
	&& DECL_NAME (*f_name) == get_identifier ("data");
}

static bool
is_assignment_to_exception (tree stmt, tree* p_name, tree* q_name)
{
  return GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt)
	&& TREE_CODE (*p_name = LHS (stmt)) == EXC_PTR_EXPR
	&& SSA_VAR_P (*q_name = RHS (stmt));
}

static bool
is_assignment_from_exception (tree stmt, tree* p_name, tree* q_name)
{
  return GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt)
	&& SSA_VAR_P (*p_name = LHS (stmt))
	&& TREE_CODE (*q_name = RHS (stmt)) == EXC_PTR_EXPR;
}

static void
do_assignment_to_exception_object (con_graph cg, tree p_name, tree q_name)
{
  con_node p = existing_local_node (cg, p_name);
  set_escape_state (p, EA_ARG_ESCAPE);

  con_node q = get_existing_node (cg, q_name);
  gcc_assert (q); /* This should be the def of p */

  do_assignment (p, q);
}

static void
do_assignment_from_exception_object (con_graph cg, tree p_name, tree q_name)
{
  con_node q = existing_local_node (cg, q_name);
  set_escape_state (q, EA_ARG_ESCAPE);

  con_node p = existing_local_node (cg, p_name);
  do_assignment (p, q);
}


static bool
is_assignment_to_array (tree stmt, tree *p_name, tree *f_name, tree *q_name)
{
  /* this is a mix of assignment to field, and assignment to array */
  /* p.data[x] = q:*/

  tree array_ref, component_ref, indirect_ref;
  return GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt)
	&& TREE_CODE (array_ref = LHS (stmt)) == ARRAY_REF
	&& TREE_CODE (component_ref = LHS (array_ref)) == COMPONENT_REF
	&& TREE_CODE (indirect_ref = LHS (component_ref)) == INDIRECT_REF
	&& SSA_VAR_P (*p_name = LHS (indirect_ref))
	&& TREE_CODE (*f_name = RHS (component_ref)) == FIELD_DECL
	&& DECL_NAME (*f_name) == get_identifier ("data")
	&& is_rhs (*q_name = RHS (stmt));
}

static bool
is_assignment_to_field (tree stmt, tree* p_name, tree* f_name, tree* q_name)
{
  /* TODO allow NULL on the rhs */
  return (GIMPLE_STMT_P (stmt)
	&& has_pointer_type (stmt)
	&& is_field_ref (LHS (stmt), p_name, f_name)
	&& DECL_NAME (*f_name) != get_identifier ("vtable")
	&& is_rhs (*q_name = RHS (stmt)));
}


static void
do_assignment_to_field (con_graph cg, tree p_name, tree f_name, tree q_name, tree phantom_id)
{
  /* p.f = q; */
  xprintf ("New reference (assign to field)\n");

  con_node p = get_existing_node (cg, p_name);
  con_node q = get_existing_node (cg, q_name);

  if (is_null (q_name) && q == NULL)
    q = add_null_node (cg, q_name);

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
  while (v)
    {
      if (get_edge (v, q) == NULL)
	add_edge (v, q);

      NEXT_LINK_CLEAR (v);
    }
}

/* Returns true if the statement is an offset of -4 from a node which
 * points directly to the exception object */
static bool
is_exception_offset (con_graph cg, tree stmt, tree *p_name, tree *q_name)
{
  tree minus_expr;
  con_node node;
  return (GIMPLE_STMT_P (stmt) 
      && has_pointer_type (stmt)
      && SSA_VAR_P (*p_name = LHS (stmt))
      && TREE_CODE (minus_expr = RHS (stmt)) == MINUS_EXPR
      && SSA_VAR_P (*q_name = LHS (minus_expr)) 
      && TREE_CODE (RHS (minus_expr)) == INTEGER_CST
      && TREE_INT_CST_LOW (RHS (minus_expr)) == 4
      && TREE_INT_CST_HIGH (RHS (minus_expr)) == 0
      && (node = get_existing_node (cg, *q_name))
      && node->out
      && TREE_CODE (node->out->target->id) == EXC_PTR_EXPR);
}

/* As above, but it has another step to look through */
static bool
is_exception_dereference (con_graph cg, tree stmt, tree *p_name, tree *q_name)
{
  tree indirect_ref;
  con_node node;
  return (GIMPLE_STMT_P (stmt) 
      && has_pointer_type (stmt) 
      && SSA_VAR_P (*p_name = LHS (stmt))
      && TREE_CODE (indirect_ref = RHS (stmt)) == INDIRECT_REF
      && SSA_VAR_P (*q_name = LHS (indirect_ref))
      && (node = get_existing_node (cg, *q_name))
      && node->out
      && node->out->target->out
      && TREE_CODE (node->out->target->out->target->id) == EXC_PTR_EXPR);
}

/* Virtual functions which havent been devirtualized look like this:
 * This function checks for the vtable, and protects the rest of "update"
 * from it. These are only here so that we can check for nodes we dont
 * expect. */
static bool
stmt_in_virtual_function (con_graph cg, tree stmt)
{
  tree f_name, plus_expr;
  tree obj_type_ref, q_name;
  tree indirect_ref;
  tree call_expr;

  /* check for vtable: a1 = a0->vtable; */
  if ((GIMPLE_STMT_P (stmt)
       && has_pointer_type (stmt)
       && SSA_VAR_P (LHS (stmt))
       && is_field_ref (RHS (stmt), &q_name, &f_name)
       && SSA_VAR_P (q_name)
       && DECL_NAME (f_name) == get_identifier ("vtable")))
    {
      xprintf ("vtable\n");
      return true;
    }

  /* check for class (interfaces) */
  if ((GIMPLE_STMT_P (stmt)
       && has_pointer_type (stmt)
       && SSA_VAR_P (LHS (stmt))
       && is_field_ref (RHS (stmt), &q_name, &f_name)
       && SSA_VAR_P (q_name)
       && DECL_NAME (f_name) == get_identifier ("class")))
    {
      xprintf ("class\n");
      return true;
    }

  if (GIMPLE_STMT_P (stmt)
       && (call_expr = get_call_expr_in (stmt))
       && TREE_CODE (LHS (call_expr)) != SSA_NAME
       && DECL_NAME (LHS (LHS (call_expr))) == 
       get_identifier ("_Jv_LookupInterfaceMethodIdx"))
    {
      xprintf ("lookup interface\n");
      return true;
    }


  /* Check for the vtable offset: a2 = a1 + xB; */
  if (GIMPLE_STMT_P (stmt) 
      && has_pointer_type (stmt)
      && SSA_VAR_P (LHS (stmt))
      && TREE_CODE (plus_expr = RHS (stmt)) == PLUS_EXPR
      && SSA_VAR_P (LHS (plus_expr)) 
      && TREE_CODE (RHS (plus_expr)) == INTEGER_CST)
    {
      xprintf ("vtable offset\n");
      return true;
    }

  /* Check for the vtable dereference: a3 = *a2; */
  if (GIMPLE_STMT_P (stmt) 
      && has_pointer_type (stmt) 
      && SSA_VAR_P (LHS (stmt))
      && TREE_CODE (indirect_ref = RHS (stmt)) == INDIRECT_REF
      && SSA_VAR_P (LHS (indirect_ref)))
    {
      xprintf ("vtable offset dereference\n");
      return true;
    }

  /* The object_type_reference: 
   * a4 = (OBJ_TYPE_REF (a3, this_8->function_name)); */
  if (GIMPLE_STMT_P (stmt) 
      && has_pointer_type (stmt)
      && SSA_VAR_P (LHS (stmt))
      && TREE_CODE (obj_type_ref = RHS (stmt)) == OBJ_TYPE_REF)
    {
      xprintf ("MISSED OPPORTUNITY: virtual function call\n");
      return true;
    }

  /* A bog-standard cast, in which the RHS isnt in the cg:
   * a5 = (TYPE:: *) a4; */
  if (GIMPLE_STMT_P (stmt)
      && has_pointer_type (stmt) 
      && is_gimple_cast (RHS (stmt))
      && SSA_VAR_P (LHS (stmt)) 
      && SSA_VAR_P (LHS (RHS (stmt))))
    {
      xprintf ("cast of virtual function call\n");
      return true;
    }


  /* Indirect function call:
   * a5 (this_8, arg0, arg1); */
  /* This can also be x = a5 (this_8, arg0, arg1) */
  call_expr = get_call_expr_in (stmt);
  if (call_expr
      && SSA_VAR_P (LHS (call_expr))
      && TREE_CODE (RHS (call_expr)) == TREE_LIST)
    {
      // escape
      xprintf ("indirect virtual function call\n");
      if (GIMPLE_STMT_P (stmt)) /* return node */
	{
	  con_node return_node;
	  if ((return_node = get_existing_node (cg, LHS (stmt))) == NULL)
	    return_node = add_global_node (cg, LHS (stmt));

	  return_node->escape = EA_GLOBAL_ESCAPE;
	}
      /* arguments */
      tree arguments = TREE_OPERAND (call_expr, 1);
      while (arguments)
	{
	  tree argument = TREE_VALUE (arguments);
	  if (has_pointer_type (argument))
	    {
	      // check for eg &isjversion1.class
	      if (TREE_CODE (argument) == ADDR_EXPR)
		argument = LHS (argument);

	      con_node node = get_existing_node (cg, argument);
	      if (node) // if it doesnt exist, dont bother creating it
		node->escape = EA_GLOBAL_ESCAPE;
	    }
	  arguments = TREE_CHAIN (arguments);
	}
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
  xprintf ("New reference (assign from field)\n");
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
add_implicit_parameters (con_graph cg, tree function)
{
  gcc_assert (cg && function);

  /* add the globals aswell */
  struct varpool_node *vnode;
  FOR_EACH_STATIC_VARIABLE (vnode)
    {
      /* occasionally you see something like Jv_InitClass (x), in which x
       * is a record_type of java.lang.Class, which we should have added
       * as a global by then */
      if (has_pointer_type (vnode->decl) || is_class_type (vnode->decl)
	  || is_array_type (vnode->decl))
	add_global_node (cg, vnode->decl);
    }

  /* Add an edge from each VAR_DECL to its definition, which is available
   * from the gimple. */
  tree arg_list = DECL_ARGUMENTS (function);
  while (arg_list)
    {
      if (has_pointer_type (arg_list))
	{
	  tree in_ssa = gimple_default_def (cfun, arg_list);
	  con_node actual_node = add_callee_parameter (cg, arg_list);

	  if (is_runnable (TREE_TYPE (TREE_TYPE (arg_list))))
	    set_escape_state (actual_node, EA_GLOBAL_ESCAPE);

	  if (in_ssa)
	    {
	      con_node arg_node = add_local_node (cg, in_ssa);
	      set_escape_state (arg_node, EA_ARG_ESCAPE);
	      add_edge (arg_node, actual_node);
	    }
	}

      arg_list = TREE_CHAIN (arg_list);
    }

  /* Add a node for the return value */
  tree result = DECL_RESULT (cg->function);
  if (has_pointer_type (result))
    {
      con_node result_node = add_result_node (cg, result);
      if (is_runnable (TREE_TYPE (TREE_TYPE (result))))
	set_escape_state (result_node, EA_GLOBAL_ESCAPE);
    }
}


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
      xprintf ("Ignore\n");
      return;
    }

  gcc_assert (SSA_VAR_P (result));

  bool changed = false;

  /* check it's correct. If not, bypass and recreate */
  con_node p = get_existing_node (cg, result);

  if (!p) 
    {
      xprintf ("New result\n");
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
	      xprintf ("New argument\n");
	      changed = true;
	      break;
	    }
	  else
	    {
	      if (!get_edge (p, q)) 
		{
		  /* this mustnt have been here at the last iteration */
		  xprintf ("New edge\n");
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

  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      tree arg = PHI_ARG_DEF (phi, i);
      tf (arg, false);
      gcc_assert (SSA_VAR_P (arg));
      con_node q = get_existing_node (cg, arg);
      if (!q) /* a ref which hasnt been created yet */
	{
	  gcc_assert (bb_graphs [PHI_ARG_EDGE (phi, i)->src->index] == NULL);
	  xprintf ("Ignore edge\n");
	}
      else
	{
	  /* I dont understand why, but i've seen a phi with 2 copies of
	   * the same node. Additionally, the result can be the same as
	   * an arg. */
//	  gcc_assert (!get_edge (p, q));
	  if (get_edge (p, q) == NULL && p != q)
	    add_edge (p, q);
	  xprintf ("New edge\n");
	}
    }
  nl (); nl ();
}

static void
check_final_block (con_graph cg, tree stmt)
{
  tree function;

  if (current_block_is_final)
    return;

  /* Any block with a throw statement can exit the basic block */
  if (is_function_call (stmt, &function, NULL, NULL))
    {
      tree identifier = DECL_NAME (function);
      if (identifier == get_identifier ("_Jv_Throw")
	  || identifier == get_identifier ("_Jv_ThrowBadArrayIndex")
	  || identifier == get_identifier ("_Jv_CheckCast") // Cant throw a ClassCastException
	  || identifier == get_identifier ("_Jv_CheckArrayStore") // Can throw an ArrayStoreException 
	  || identifier == get_identifier ("_Jv_ThrowNullPointerException"))
	{
	  // merge now, not at the end of the block
	  merge_into (bb_graphs [EXIT_BLOCK], cg); 
	  return;
	}
    }

  /* Any block with no successors can exit the basic block */
  basic_block bb = bb_for_stmt (stmt);
  if (VEC_empty (edge, bb->succs))
    {
      tree last = last_stmt (bb);

      /* A resx_expr continues an exception. It seems that the exception
       * is caught above the resx_expr for the purpose of cleanup (ie
       * destructors, and especially locks), whereafter it is relaunched.
       * I think we can simplifiy the cfg by removing this after removing
       * Jv_MonitorExits */
      if (stmt == last)
	gcc_assert (TREE_CODE (stmt) == RESX_EXPR); 

      current_block_is_final = true;
      return;
    }

  /* Any block who's successor is the exit_block can exit */
  if (VEC_length (edge, bb->succs) == 1)
    { 
      edge e;
      edge_iterator ei;
      FOR_EACH_EDGE (e, ei, bb->succs)
	if (e->dest == EXIT_BLOCK_PTR)
	  current_block_is_final = true;
    }
}

static bool
is_addr_label (tree stmt)
{
  return GIMPLE_STMT_P (stmt)
    && SSA_VAR_P (LHS (stmt))
    && TREE_CODE (RHS (stmt)) == ADDR_EXPR
    && TREE_CODE (LHS (RHS (stmt))) == LABEL_DECL;
}

static bool
is_indirect_goto (tree expr)
{
  return expr 
    && TREE_CODE (expr) == GOTO_EXPR
    && TREE_CODE (LHS (expr)) != LABEL_DECL;
}

static void
update_connection_graph_from_statement (con_graph cg, tree stmt)
{
  gcc_assert (cg);
  gcc_assert (stmt);

  tree p_name, q_name, f_name;
  tree function, arguments, return_value;
  tree type;

  /* recognize all the statement types, and update the connection
   * graph accordingly */


  if (is_special_function_call (stmt))
    ;

  else if (is_memory_allocation (stmt, &p_name, &type))
      do_memory_allocation (cg, p_name, stmt, type);

  else if (is_global_memory_allocation (stmt, &p_name, &type))
      do_global_memory_allocation (cg, p_name, stmt, type);

  else if (is_reference_cast (cg, stmt, &p_name, &q_name)
	   || is_reference_copy (stmt, &p_name, &q_name)
	   || is_function_cast (stmt, &p_name, &q_name)
	   || is_check_cast (stmt, &p_name, &q_name)
	   || is_exception_offset (cg, stmt, &p_name, &q_name)
	   || is_exception_dereference (cg, stmt, &p_name, &q_name))
      do_reference_copy (cg, p_name, q_name);

  else if (is_throw (stmt, &p_name))
    do_throw (cg, p_name);

  else if (is_assignment_to_field (stmt, &p_name, &f_name, &q_name)
	   || is_assignment_to_array (stmt, &p_name, &f_name, &q_name))
      do_assignment_to_field (cg, p_name, f_name, q_name, stmt);

  else if (is_return_stmt (stmt, &return_value, &q_name))
    do_return_stmt (cg, return_value, q_name);

  else if (is_assignment_from_exception (stmt, &p_name, &q_name))
    do_assignment_from_exception_object (cg, p_name, q_name);

  else if (is_assignment_to_exception (stmt, &p_name, &q_name))
    do_assignment_to_exception_object (cg, p_name, q_name);

  else if (is_assignment_from_global_array (stmt, &p_name, &q_name))
    do_assignment_from_global_array (cg, p_name, q_name);

  else if (stmt_in_virtual_function (cg, stmt))
    ; 

  /* these can interfere with virtual function statements */
  else if (is_assignment_from_field (stmt, &p_name, &q_name, &f_name)
	   || is_assignment_from_array (stmt, &p_name, &q_name, &f_name))
      do_assignment_from_field (cg, p_name, q_name, f_name, stmt);


  else if (is_function_call (stmt, &function, &arguments, &return_value))
      do_function_call (cg, function, arguments, return_value, stmt);

  else if (get_call_expr_in (stmt) != NULL)
    {
      xprintf ("MISSED ONE\n");
      gcc_unreachable ();
    }

  else if ((TREE_CODE (stmt) == COND_EXPR
	    && (is_indirect_goto (TREE_OPERAND (stmt, 1))
		|| is_indirect_goto (TREE_OPERAND (stmt, 2))))
	   || is_indirect_goto (stmt)
	   || TREE_CODE (stmt) == SWITCH_EXPR)
    merge_into (bb_graphs [EXIT_BLOCK], cg); 

  // dont even need to merge with final
  else if (TREE_CODE (stmt) == RETURN_EXPR)
    ; 

  else if (TREE_CODE (stmt) == LABEL_EXPR 
	   || TREE_CODE (stmt) == COND_EXPR
	   || TREE_CODE (stmt) == RESX_EXPR
	   || is_addr_label (stmt)
	   || (!has_pointer_type (stmt) && GIMPLE_STMT_P (stmt)))
      xprintf ("Ignore\n");


  else
    {
      xprintf ("Still to be dealt with\n");
      con_graph_dump (cg);
      gcc_unreachable ();
      assert_all_next_link_free (cg);
    }

  /* check whether this statement means that the block may be a final block */
  check_final_block (cg, stmt); 
  return;

}

static void
store_for_bb (basic_block bb, con_graph cg)
{
  cg->refs = VEC_length (edge, bb->succs);
  if (cg->refs == 0)
    free_con_graph (cg);
  else
    bb_graphs [bb->index] = cg;
}

/* Decrease the ref count */
static void
mark_as_used (con_graph cg)
{
  gcc_assert (cg->refs > 0);
  cg->refs--;

  if (cg->refs == 0 && cg->index != ENTRY_BLOCK)
    {
      bb_graphs [cg->index] = NULL;
      free_con_graph (cg);
    }
}

/* Merges into CG the graphs belonging to the predecessors of BB, and
 * propagates the last_changed information. If BB is the last changed
 * block, return true, else false */
static void
merge_from_predecessors (con_graph cg, basic_block bb)
{
  edge e;
  edge_iterator ei;
  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      con_graph pred = bb_graphs [e->src->index];
      if (pred == NULL)
	{
	  xprintf ("No graph for bb %d (for %d)\n", e->src->index, bb->index);
	  continue;
	}
      xprintf ("Merging graph %d into %d\n", e->src->index, bb->index);
      merge_into (cg, pred);
      mark_as_used (pred);
    }
}

static void
process_basic_block (struct dom_walk_data * walk_data __attribute__ ((unused)), basic_block bb)
{
  xprintf ("Processing bb %d\n", bb->index);
  con_graph cg;
  current_block_is_final = false;

  if (bb->index == ENTRY_BLOCK)
    cg = bb_graphs [bb->index];
  else
    cg = new_con_graph (current_function_decl, bb->index, iteration_count);

  merge_from_predecessors (cg, bb);

  /* process the phis and statements */
  tree phi;
  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      xprintf ("Processing phi "); tf (phi, true);
      update_connection_graph_from_phi (cg, phi);
      nl (); nl ();
    }

  block_stmt_iterator bsi;
  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      tree stmt = bsi_stmt (bsi);
      xprintf ("Processing stmt "); tf (stmt, true);
      update_connection_graph_from_statement (cg, stmt);
      nl (); nl ();
    }

  assert_all_next_link_free (cg);

  /* As this is potentially final, merge it into the final graph */
  if (current_block_is_final)
    merge_into (bb_graphs [EXIT_BLOCK], cg);

  store_for_bb (bb, cg);
#ifdef DUMP_BB_GRAPHS
  con_graph_dump (cg);
#endif
}

static void
create_graphs (void)
{
  bb_graphs = XCNEWVEC (con_graph, n_basic_blocks);
}

static void
delete_graphs (void)
{
  int i;
  // dont delete the EXIT_BLOCK graph
  // There are some graphs left because of their successors are along back
  // edges
  for (i = EXIT_BLOCK + 1; i < n_basic_blocks; i++)
    if (bb_graphs[i])
      free_con_graph (bb_graphs [i]);
}


static con_graph
process_function (tree function)
{
  /* only process one function */
//  if (DECL_NAME (function) != get_identifier ("expansion_choices"))
//    return NULL;

  xprintf ("Processing function: %s\n", 
	   IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (function)));
  if (dump_file) 
    dump_function_to_file (current_function_decl, dump_file, dump_flags);

    /* Set up dominance for traversal */
  struct dom_walk_data walk_data;
  calculate_dominance_info (CDI_DOMINATORS);
  init_walk_dominator_tree (&walk_data);
  walk_data.walk_stmts_backward = false;
  walk_data.dom_direction = CDI_DOMINATORS;
  walk_data.initialize_block_local_data = NULL;
  walk_data.before_dom_children_before_stmts = process_basic_block;
  walk_data.before_dom_children_walk_stmts = NULL;
  walk_data.before_dom_children_after_stmts = NULL;
  walk_data.after_dom_children_before_stmts = NULL;
  walk_data.after_dom_children_walk_stmts = NULL;
  walk_data.after_dom_children_after_stmts = NULL;
  walk_data.global_data = NULL;
  walk_data.block_local_data_size = 0;
  walk_data.interesting_blocks = NULL;

  create_graphs ();
  bb_graphs [ENTRY_BLOCK] = new_con_graph (function, ENTRY_BLOCK, 0);
  add_implicit_parameters (bb_graphs [ENTRY_BLOCK], function);
  con_graph last_exit_graph = NULL;


  for (iteration_count = 0; iteration_count < 10; iteration_count++)
    {
      xprintf ("Starting iteration %d\n", iteration_count);

      bb_graphs [EXIT_BLOCK] = 
	new_con_graph (current_function_decl, EXIT_BLOCK, iteration_count);

      walk_dominator_tree (&walk_data, ENTRY_BLOCK_PTR);

      // The final block is a merge of any block which has a Throw (of any
      // sort), or a RESX_expr, or who's succesor is the final block (i
      // think thats enough for multiple returns
      
      // check for infinite loop
      if (bb_graphs [EXIT_BLOCK]->root == NULL)
	{
	  // merge all existing graphs
	  basic_block bb;
	  FOR_EACH_BB (bb)
	    {
	      if (bb_graphs [bb->index])
		merge_into (bb_graphs [EXIT_BLOCK], bb_graphs [bb->index]);
	    }
	}

      if (last_exit_graph 
	  && graphs_equal (bb_graphs [EXIT_BLOCK], last_exit_graph))
	break;

      last_exit_graph = bb_graphs [EXIT_BLOCK];
      con_graph_dump (last_exit_graph);
    }
  if (iteration_count == 10)
    gcc_unreachable (); // TODO deal with this

  delete_graphs ();

  con_graph cg = bb_graphs [EXIT_BLOCK];

  /* Finalize and clean up.  */
  fini_walk_dominator_tree (&walk_data);
  free_dominance_info (CDI_DOMINATORS);
  gcc_assert (cg);

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

  cg->filename = concat (cg->filename, "_final_", NULL);
  con_graph_dump_prefix (cg, "before_cleanup");
  bypass_every_node (cg);
  clean_up_con_graph (cg);
  serialize_con_graph (cg);
  con_graph_dump (cg);

  return cg;
}

static unsigned int
execute_ipa_stack_allocate (void)
{
  int i;
  if (sorrycount || errorcount)
    return 0;

  /* init the markers used to traverse the graph */
  init_markers ();

  /* Init the store of connection graphs */
  init_graphs ();

  /* TODO should I use postorder from ipa.c ? */
  struct cgraph_node **order = XCNEWVEC (struct cgraph_node *, cgraph_n_nodes);

  int order_pos = ipa_utils_reduced_inorder (order, true, false);
  /* TODO what happens with cycles here. RE reduced parameter */

  for (i = 0; i < order_pos; i++)
    {
      struct cgraph_node *node = order[i];
      if (node->analyzed)
	{
	  /* set up the current function */
	  push_cfun (DECL_STRUCT_FUNCTION (node->decl));
	  tree_register_cfg_hooks ();
	  current_function_decl = node->decl;

	  con_graph cg = process_function (current_function_decl);
	  free_con_graph (cg);
//	  add_con_graph (cg);

	  /* Clean up function */
	  current_function_decl = NULL;
	  pop_cfun ();
	}
    }

  /* Cleanup. */
  struct cgraph_node *node;
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
  TODO_dump_func,			/* todo_flags_start */
  TODO_dump_func,			/* todo_flags_finish */
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
