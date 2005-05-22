/* Interprocedural analyses.
   Copyright (C) 2005 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "langhooks.h"
#include "ggc.h"
#include "target.h"
#include "cgraph.h"
#include "output.h"
#include "tree-iterator.h"
#include "tree-inline.h"
#include "ipa-prop.h"
#include "function.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "flags.h"
#include "timevar.h"

/* This file contains interfaces shared by different
   interprocedural analyses (ipcp and ipaa):

   - ipa_methodlist interface - worklist of methods used in
   ipcp_propagate_stage () and ipaa_propagate_stage () functions.

   - ipa_callsite interface - for each callsite this interface calculates
   ipa_edge structure associeted with it.

   - ipa_method interface - for each method this interface calculates ipa_node
   structure associeted with it.

   - jump function interface - this interface returns type and info_type of
   ipa_jump_function that was previously calculated.

   - ipa_nodes and ipa_edges - this interface is responsible for allocation 
   and deletion of ipa_node and ipa_edge structures.

   - debugging interface - for each method this interface prints info 
   collected in ipa_node of this method.  */

/* Auxiliary functions of ipa_methodlist interface.  */
static inline ipa_methodlist_p ipa_create_methodlist_node (void);
static inline ipa_method ipa_methodlist_method (ipa_methodlist_p);
static inline void ipa_methodlist_method_set (ipa_methodlist_p, ipa_method);
static inline ipa_methodlist_p ipa_methodlist_next_method (ipa_methodlist_p);
static inline void ipa_methodlist_next_method_set (ipa_methodlist_p,
						   ipa_methodlist_p);

/* Auxiliary functions for ipa_callsite interface.  */
static inline void ipa_callsite_param_set_type (ipa_callsite, int,
						enum Jfunc_type);
static inline void ipa_callsite_param_set_info_type_formal (ipa_callsite, int,
							    unsigned);
static inline void ipa_callsite_param_set_info_type_int (ipa_callsite, int,
							 ipcp_int *);
static inline void ipa_callsite_param_set_info_type_float (ipa_callsite, int,
							   REAL_VALUE_TYPE *);
static inline void ipa_callsite_param_map_create (ipa_callsite);
static inline tree ipa_callsite_tree (ipa_callsite);
static inline ipa_method ipa_callsite_caller (ipa_callsite);

/* Auxiliary functions of ipa_method interface.  */
static inline bool ipa_method_is_modified (ipa_method, int);
static inline void ipa_method_tree_map_create (ipa_method);
static inline void ipa_method_modify_create (ipa_method);
static inline void ipa_method_modify_set (ipa_method, int, bool);
static int ipa_method_tree_map (ipa_method, tree);
static tree ipa_modify_walk_tree (tree *, int *, void *);
static void ipa_modify_walk_tree_1 (tree *, void *);
static void ipa_method_modify_init (ipa_method);

/* ipa_methodlist interface.  */

/* Creates the worklist node.  */
static inline ipa_methodlist_p
ipa_create_methodlist_node (void)
{
  return ggc_alloc (sizeof (struct ipa_methodlist));
}

/* Checks that worklist is not empty.  */
inline bool
ipa_methodlist_not_empty (ipa_methodlist_p wl)
{
  return (wl != NULL);
}

/* Gets method from worklist's node.  */
static inline ipa_method
ipa_methodlist_method (ipa_methodlist_p wl)
{
  return wl->method_p;
}

/* Sets worklist's node to point to mt.  */
static inline void
ipa_methodlist_method_set (ipa_methodlist_p wl, ipa_method mt)
{
  wl->method_p = mt;
}

/* Gets next worklist's node.  */
static inline ipa_methodlist_p
ipa_methodlist_next_method (ipa_methodlist_p wl)
{
  return wl->next_method;
}

/* Sets worklist node wl1 to point to  worklist node wl2.  */
static inline void
ipa_methodlist_next_method_set (ipa_methodlist_p wl1, ipa_methodlist_p wl2)
{
  wl1->next_method = wl2;
}

/* Initializing a worklist to contain all methods.  */
ipa_methodlist_p
ipa_methodlist_init (void)
{
  ipa_method node;
  ipa_methodlist_p wl;

  wl = NULL;
  for (node = cgraph_nodes; node; node = node->next)
    ipa_add_method (&wl, node);

  return wl;
}

/* Adding a method to the worklist.  */
void
ipa_add_method (ipa_methodlist_p * wl, ipa_method mt)
{
  ipa_methodlist_p temp;

  temp = ipa_create_methodlist_node ();
  ipa_methodlist_method_set (temp, mt);
  ipa_methodlist_next_method_set (temp, *wl);
  *wl = temp;
}

/* Removing a method from the worklist.  */
ipa_method
ipa_remove_method (ipa_methodlist_p * wl)
{
  ipa_methodlist_p first;

  first = *wl;
  *wl = ipa_methodlist_next_method (*wl);
  return ipa_methodlist_method (first);
}

/* ipa_callsite interface.  */

/* Gets how many arguments the callsite has.  */
inline int
ipa_callsite_param_count (ipa_callsite cs)
{
  return ((struct ipa_edge *) cs->aux)->ipa_param_num;
}

/* Sets how many arguments the callsite has.  */
inline void
ipa_callsite_param_count_set (ipa_callsite cs, int i)
{
  ((struct ipa_edge *) cs->aux)->ipa_param_num = i;
}

/* Gets the jump function for argument i of callsite cs.  */
inline struct ipa_jump_func *
ipa_callsite_param (ipa_callsite cs, int i)
{
  return &(((struct ipa_edge *) cs->aux)->ipa_param_map[i]);
}

/* Gets the callee of callsite cs.  */
inline ipa_method
ipa_callsite_callee (ipa_callsite cs)
{
  return cs->callee;
}

/* Sets the jump function's type for argument i of callsite cs.  */
static inline void
ipa_callsite_param_set_type (ipa_callsite cs, int i, enum Jfunc_type type1)
{
  ((struct ipa_edge *) cs->aux)->ipa_param_map[i].type = type1;
}

/* Sets formal to the callsite's info type (for argument i).  */
static inline void
ipa_callsite_param_set_info_type_formal (ipa_callsite cs, int i,
					 unsigned int formal)
{
  ipa_callsite_param (cs, i)->info_type.formal_id = formal;
}

/* Sets int value to the callsite's info type (for argument i).  */
static inline void
ipa_callsite_param_set_info_type_int (ipa_callsite cs, int i,
				      ipcp_int * info_type1)
{
  ipa_callsite_param (cs, i)->info_type.int_value.high = info_type1->high;
  ipa_callsite_param (cs, i)->info_type.int_value.low = info_type1->low;
}

/* Sets float value to the callsite's info type (for argument i).  */
static inline void
ipa_callsite_param_set_info_type_float (ipa_callsite cs, int i,
					REAL_VALUE_TYPE * info_type1)
{
  ipa_callsite_param (cs, i)->info_type.float_value = *info_type1;
}

/* Allocates space for the jump functions.  */
static inline void
ipa_callsite_param_map_create (ipa_callsite cs)
{
  ((struct ipa_edge *) cs->aux)->ipa_param_map =
    xcalloc (ipa_callsite_param_count (cs), sizeof (struct ipa_jump_func));
}

/* Returns the call expr tree realted to callsite cs.  */
static inline tree
ipa_callsite_tree (ipa_callsite cs)
{
  return cs->call_expr;
}

/* Gets the caller of callsite cs.  */
static inline ipa_method
ipa_callsite_caller (ipa_callsite cs)
{
  return cs->caller;
}

/* Counts how many arguments this callsite has.  */
void
ipa_callsite_compute_count (ipa_callsite cs)
{
  tree callTree;
  tree arg;
  int arg_num;

  callTree = ipa_callsite_tree (cs);
  if (TREE_CODE (callTree) != CALL_EXPR)
    abort ();
  arg = TREE_OPERAND (callTree, 1);
  arg_num = 0;
  for (; arg != NULL_TREE; arg = TREE_CHAIN (arg))
    arg_num++;
  ipa_callsite_param_count_set (cs, arg_num);
}

/* Computes jump functions and inserts the information 
   in the ipa_param_map array.  */
void
ipa_callsite_compute_param (ipa_callsite cs)
{
  tree callTree;
  tree arg, cst_decl, arg_type, formal_type;
  int arg_num;
  int i;
  ipa_method mt;

  if (ipa_callsite_param_count (cs) == 0)
    return;
  ipa_callsite_param_map_create (cs);
  callTree = ipa_callsite_tree (cs);
  if (TREE_CODE (callTree) != CALL_EXPR)
    abort ();
  arg = TREE_OPERAND (callTree, 1);
  arg_num = 0;

  for (; arg != NULL_TREE; arg = TREE_CHAIN (arg))
    {
      if (TREE_CODE (TREE_VALUE (arg)) == PARM_DECL)
	{
	  mt = ipa_callsite_caller (cs);
	  i = ipa_method_tree_map (mt, TREE_VALUE (arg));
	  if (ipa_method_is_modified (mt, i))
	    ipa_callsite_param_set_type (cs, arg_num, UNKNOWN_IPATYPE);
	  else
	    {
	      ipa_callsite_param_set_type (cs, arg_num, FORMAL_IPATYPE);
	      ipa_callsite_param_set_info_type_formal (cs, arg_num, i);
	    }
	}
      else if (TREE_CODE (TREE_VALUE (arg)) == INTEGER_CST)
	{
	  arg_type = TREE_TYPE (TREE_VALUE (arg));
	  formal_type = TREE_TYPE (ipa_method_get_tree (cs->callee, arg_num));
	  if (TYPE_NAME (arg_type) == TYPE_NAME (formal_type)
	      && TYPE_CONTEXT (arg_type) == TYPE_CONTEXT (formal_type)
	      && attribute_list_equal (TYPE_ATTRIBUTES (arg_type),
				       TYPE_ATTRIBUTES (formal_type)))
	    {
	      ipa_callsite_param_set_type (cs, arg_num, CONST_IPATYPE_INT);
	      ipa_callsite_param_set_info_type_int (cs, arg_num,
						    &TREE_INT_CST (TREE_VALUE
								   (arg)));
	    }
	  else
	    ipa_callsite_param_set_type (cs, arg_num, UNKNOWN_IPATYPE);
	}
      else if (TREE_CODE (TREE_VALUE (arg)) == REAL_CST)
	{
	  arg_type = TREE_TYPE (TREE_VALUE (arg));
	  formal_type = TREE_TYPE (ipa_method_get_tree (cs->callee, arg_num));
	  if (TYPE_NAME (arg_type) == TYPE_NAME (formal_type)
	      && TYPE_CONTEXT (arg_type) == TYPE_CONTEXT (formal_type)
	      && attribute_list_equal (TYPE_ATTRIBUTES (arg_type),
				       TYPE_ATTRIBUTES (formal_type)))
	    {
	      ipa_callsite_param_set_type (cs, arg_num, CONST_IPATYPE_FLOAT);
	      ipa_callsite_param_set_info_type_float (cs, arg_num,
						      TREE_REAL_CST_PTR
						      (TREE_VALUE (arg)));
	    }
	  else
	    ipa_callsite_param_set_type (cs, arg_num, UNKNOWN_IPATYPE);

	}
      else if (TREE_CODE (TREE_VALUE (arg)) == ADDR_EXPR
	       && TREE_CODE (TREE_OPERAND (TREE_VALUE (arg), 0)) ==
	       CONST_DECL)
	{
	  cst_decl = TREE_OPERAND (TREE_VALUE (arg), 0);
	  arg_type = TREE_TYPE (DECL_INITIAL (cst_decl));
	  formal_type =
	    TREE_TYPE (TREE_TYPE (ipa_method_get_tree (cs->callee, arg_num)));
	  if (TREE_CODE (DECL_INITIAL (cst_decl)) == INTEGER_CST)
	    {
	      if (TYPE_NAME (arg_type) == TYPE_NAME (formal_type)
		  && TYPE_CONTEXT (arg_type) == TYPE_CONTEXT (formal_type)
		  && attribute_list_equal (TYPE_ATTRIBUTES (arg_type),
					   TYPE_ATTRIBUTES (formal_type)))

		{
		  ipa_callsite_param_set_type (cs, arg_num,
					       CONST_IPATYPE_INT_REF);
		  ipa_callsite_param_set_info_type_int (cs, arg_num,
							&TREE_INT_CST
							(DECL_INITIAL
							 (cst_decl)));

		}
	      else
		ipa_callsite_param_set_type (cs, arg_num, UNKNOWN_IPATYPE);
	    }
	  else if (TREE_CODE (DECL_INITIAL (cst_decl)) == REAL_CST)
	    {
	      if (TYPE_NAME (arg_type) == TYPE_NAME (formal_type)
		  && TYPE_CONTEXT (arg_type) == TYPE_CONTEXT (formal_type)
		  && attribute_list_equal (TYPE_ATTRIBUTES (arg_type),
					   TYPE_ATTRIBUTES (formal_type)))

		{
		  ipa_callsite_param_set_type (cs, arg_num,
					       CONST_IPATYPE_FLOAT_REF);
		  ipa_callsite_param_set_info_type_float (cs, arg_num,
							  TREE_REAL_CST_PTR
							  (DECL_INITIAL
							   (cst_decl)));
		}
	      else
		ipa_callsite_param_set_type (cs, arg_num, UNKNOWN_IPATYPE);
	    }
	}
      else
	ipa_callsite_param_set_type (cs, arg_num, UNKNOWN_IPATYPE);
      arg_num++;
    }
}

/* ipa_method interface.  */

/* Get number of formals of method mt.  */
inline int
ipa_method_formal_count (ipa_method mt)
{
  return ((struct ipa_node *) mt->aux)->ipa_arg_num;
}

/* Set number of formals of method mt.  */
inline void
ipa_method_formal_count_set (ipa_method mt, int i)
{
  ((struct ipa_node *) mt->aux)->ipa_arg_num = i;
}

/* Returns whether i-th formal of mt is modified in mt.  */
static inline bool
ipa_method_is_modified (ipa_method mt, int i)
{
  return ((struct ipa_node *) mt->aux)->ipa_mod[i].mod;
}

/* Get param tree of i-th formal of mt.  */
inline tree
ipa_method_get_tree (ipa_method mt, int i)
{
  return ((struct ipa_node *) mt->aux)->ipa_param_tree[i].param_tree;
}

/* Create tree map structure of mt.  */
static inline void
ipa_method_tree_map_create (ipa_method mt)
{
  ((struct ipa_node *) mt->aux)->ipa_param_tree =
    xcalloc (ipa_method_formal_count (mt), sizeof (struct ipa_tree_map));
}

/* Create modify structure of mt.  */
static inline void
ipa_method_modify_create (ipa_method mt)
{
  ((struct ipa_node *) mt->aux)->ipa_mod =
    xcalloc (ipa_method_formal_count (mt), sizeof (struct ipa_modify));
}

/* Set modify of i-th formal of mt.  */
static inline void
ipa_method_modify_set (ipa_method mt, int i, bool val)
{
  ((struct ipa_node *) mt->aux)->ipa_mod[i].mod = val;
}

/* Returning the parameter index of the ptree.  */
static int
ipa_method_tree_map (ipa_method mt, tree ptree)
{
  int i, count;

  count = ipa_method_formal_count (mt);
  for (i = 0; i < count; i++)
    if (((struct ipa_node *) mt->aux)->ipa_param_tree[i].param_tree == ptree)
      return i;

  return -1;
}

/* Inserting the method's PARM DECL trees to the ipa_param_tree array.  */
void
ipa_method_compute_tree_map (ipa_method mt)
{
  tree fndecl;
  tree fnargs;
  tree parm;
  int param_num;

  ipa_method_tree_map_create (mt);
  fndecl = mt->decl;
  fnargs = DECL_ARGUMENTS (fndecl);
  param_num = 0;
  for (parm = fnargs; parm; parm = TREE_CHAIN (parm))
    {
      ((struct ipa_node *) mt->aux)->ipa_param_tree[param_num].param_tree =
	parm;
      param_num++;
    }
}

/* Counts how many parameters a method has.  */
void
ipa_method_formal_compute_count (ipa_method mt)
{
  tree fndecl;
  tree fnargs;
  tree parm;
  int param_num;

  fndecl = mt->decl;
  fnargs = DECL_ARGUMENTS (fndecl);
  param_num = 0;
  for (parm = fnargs; parm; parm = TREE_CHAIN (parm))
    param_num++;
  ipa_method_formal_count_set (mt, param_num);
}

/* Called by walk_tree. In the case a paramer 
   is modified within the method,the appropriate entry is 
   updated in the ipa_mod array.  */
static tree
ipa_modify_walk_tree (tree * tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		      void *data)
{
  tree t = *tp;
  int i, j;

  ipa_method mt = (ipa_method) data;
  switch (TREE_CODE (t))
    {
    case MODIFY_EXPR:
      if (TREE_CODE (TREE_OPERAND (t, 0)) == PARM_DECL)
	{
	  i = ipa_method_tree_map (mt, TREE_OPERAND (t, 0));
	  ipa_method_modify_set (mt, i, true);
	}
      break;
    case ADDR_EXPR:
      /* If the parameter's address is taken, 
         it could be modified.  */

      if (TREE_CODE (TREE_OPERAND (t, 0)) == PARM_DECL)
	{
	  i = ipa_method_tree_map (mt, TREE_OPERAND (t, 0));
	  ipa_method_modify_set (mt, i, true);
	}
      break;
    case ASM_EXPR:
      /* Asm code could modify any of the parameters.  */
      for (j = 0; j < ipa_method_formal_count (mt); j++)
	ipa_method_modify_set (mt, j, true);
      break;
    default:
      break;
    }
  return NULL;
}

/* Initializes the ipa_mod array.  */
static void
ipa_method_modify_init (ipa_method mt)
{
  int i, count;

  ipa_method_modify_create (mt);
  count = ipa_method_formal_count (mt);
  for (i = 0; i < count; i++)
    ipa_method_modify_set (mt, i, false);
}

/* The modify computation driver.  */
void
ipa_method_compute_modify (ipa_method mt)
{
  tree decl;
  tree body;
  int j, count;
  basic_block bb;
  struct function *func;
  block_stmt_iterator bsi;
  tree stmt;

  ipa_method_modify_init (mt);
  decl = mt->decl;
  /* ??? Handle pending sizes case. Set all parameters 
     of the method to be modified.  */
  if (DECL_UNINLINABLE (decl))
    {
      count = ipa_method_formal_count (mt);
      for (j = 0; j < count; j++)
	ipa_method_modify_set (mt, j, true);
      return;
    }
  body = DECL_SAVED_TREE (decl);
  if (body != NULL)
    {
      func = DECL_STRUCT_FUNCTION (decl);
      FOR_EACH_BB_FN (bb, func)
      {
	for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	  {
	    stmt = bsi_stmt (bsi);
	    ipa_modify_walk_tree_1 (&stmt, (void *) mt);
	  }
      }
    }
}

/* Walks tp to find if there's modification to any of the parameters.  */
static void
ipa_modify_walk_tree_1 (tree * tp, void *data)
{
  walk_tree (tp, ipa_modify_walk_tree, data, NULL);
}

/* ipa_jump_func interface.  */

/* Get type of jump function jf.  */
inline enum Jfunc_type
get_type (struct ipa_jump_func *jf)
{
  return jf->type;
}

/* Get info type of jump function jf.  */
inline union info *
ipa_jf_get_info_type (struct ipa_jump_func *jf)
{
  return &(jf->info_type);
}

/* ipa_node and ipa_edge interfaces.  */

/* Allocate and initialize ipa_node structure.  */
inline void
ipa_node_create (ipa_method node)
{
  node->aux = xcalloc (1, sizeof (struct ipa_node));
}

/* Allocate and initialize ipa_node structure for all
   nodes in callgraph.  */
void
ipa_nodes_create (void)
{
  ipa_method node;

  for (node = cgraph_nodes; node; node = node->next)
    ipa_node_create (node);
}

/* Allocate and initialize ipa_edge structure.  */
void
ipa_edges_create (void)
{
  ipa_method node;
  ipa_callsite cs;

  for (node = cgraph_nodes; node; node = node->next)
    for (cs = node->callees; cs; cs = cs->next_callee)
      cs->aux = xcalloc (1, sizeof (struct ipa_edge));
}

/* Frees ipa_node structure.  */
void
ipa_nodes_free (void)
{
  ipa_method node;

  for (node = cgraph_nodes; node; node = node->next)
    {
      free (node->aux);
      node->aux = NULL;
    }
}

/* Frees ipa_edge structure.  */
void
ipa_edges_free (void)
{
  ipa_method node;
  ipa_callsite cs;

  for (node = cgraph_nodes; node; node = node->next)
    for (cs = node->callees; cs; cs = cs->next_callee)
      {
	free (cs->aux);
	cs->aux = NULL;
      }
}

/* Frees the ipa_method's ipa data structures.  */
void
ipa_free (void)
{
  ipa_method node;
  ipa_callsite cs;

  for (node = cgraph_nodes; node; node = node->next)
    {
      if (node->aux == NULL)
	continue;
      if (((struct ipa_node *) node->aux)->ipcp_cval)
	free (((struct ipa_node *) node->aux)->ipcp_cval);
      if (((struct ipa_node *) node->aux)->ipa_param_tree)
	free (((struct ipa_node *) node->aux)->ipa_param_tree);
      if (((struct ipa_node *) node->aux)->ipa_mod)
	free (((struct ipa_node *) node->aux)->ipa_mod);
      for (cs = node->callees; cs; cs = cs->next_callee)
	{
	  if (cs->aux)
	    if (((struct ipa_edge *) cs->aux)->ipa_param_map)
	      free (((struct ipa_edge *) cs->aux)->ipa_param_map);
	}
    }
}

/* Debugging interface.  */

/* Prints ipa_tree_map data structures.  */
void
ipa_method_tree_print (FILE * f)
{
  int i, count;
  tree temp;
  ipa_method node;

  fprintf (f, "\nPARAM TREE MAP PRINT\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      fprintf (f, "method  %s Trees :: \n", cgraph_node_name (node));
      count = ipa_method_formal_count (node);
      for (i = 0; i < count; i++)
	{
	  temp = ipa_method_get_tree (node, i);
	  if (TREE_CODE (temp) == PARM_DECL)
	    fprintf (f, "  param [%d] : %s\n", i,
		     (*lang_hooks.decl_printable_name) (temp, 2));
	  else
	    {
	      fprintf (stderr, "not a declaration\n");
	      abort ();
	    }
	}

    }
}

/* Printf ipa_modify data structures.  */
void
ipa_method_modify_print (FILE * f)
{
  int i, count;
  bool temp;
  ipa_method node;

  fprintf (f, "\nMODIFY PRINT\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      fprintf (f, "method  %s :: \n", cgraph_node_name (node));
      count = ipa_method_formal_count (node);
      for (i = 0; i < count; i++)
	{
	  temp = ipa_method_is_modified (node, i);
	  if (temp)
	    fprintf (f, " param [%d] true \n", i);
	  else
	    fprintf (f, " param [%d] false \n", i);
	}
    }
}
