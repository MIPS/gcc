/* Interprocedural Alias Analysis.
   Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by Olga Golovanevsky <olga@il.ibm.com>

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

/* The interprocedural aliasing analysis is analysis of formals
   based on the following article:

   Keith D. Cooper and Ken Kennedy, "Fast interprocedural alias analysis",
   16th ACM Symposium on Principles of Programming Languages. pages 49-57.,
   Jan 11-13 1989, Austin, Texas.

   The analysis comprises three stages: (1) initialization, (2) propagation
   and (3) stage of preserving results of ipaa analysis for subsequent
   compilation stages (this stage is not implemented yet).

   Stage (1) is done by ipaa_init_stage () function. Intraprocedural 
   analysis are used to define aliasing of actual parameters at callsites. 
   For now, only combinations of local arrays and formals are considered. 
   Formals and local arrays which were modified inside the function, or 
   which have their addresses taken, are removed from consideration.

   Stage (2) is done in ipaa_propagate_stage (); starting from callers
   the aliasing info which was calculated at the initialization stage is
   propagated to callees and further on to callers of callees.

   The flag "-fipa-alias" activates this optimization.

*/

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
#include "alloc-pool.h"

/* Static variable to indicate that ipaa was executed already.  */
static int ipaa_was_executed = 0; 

/* ipaa_method interface  */
static inline void ipaa_method_create_alias_info (struct cgraph_node *);
static inline void ipaa_method_alias_init (struct cgraph_node *);
static inline void ipaa_method_set_num_pairs_args (struct cgraph_node *, int);
static inline int ipaa_method_get_num_pairs_args (struct cgraph_node *);
static inline void ipaa_method_compute_num_pairs (struct cgraph_node *);
static inline void ipaa_method_set_pair_alias_info (struct cgraph_node *,
						    ipaa_alias_info, int,
						    int);
static inline ipaa_alias_info ipaa_method_get_pair_alias_info (struct
							       cgraph_node *,
							       int, int);
static inline void ipaa_method_set_alias_info (struct cgraph_node *,
					       ipaa_alias_info, int);
static inline ipaa_alias_info ipaa_method_get_alias_info (struct 
							  cgraph_node *, 
							  int);

/* ipaa_callsite interface.  */
static inline void ipaa_callsite_create_alias_info (struct cgraph_edge *);
static inline void ipaa_callsite_alias_init (struct cgraph_edge *);
static inline void ipaa_callsite_set_num_pairs_args (struct cgraph_edge *,
						     int);
static inline int ipaa_callsite_get_num_pairs_args (struct cgraph_edge *);
static inline void ipaa_callsite_compute_num_pairs (struct cgraph_edge *);
static inline void ipaa_callsite_set_pair_alias_info (struct cgraph_edge *,
						      ipaa_alias_info, int,
						      int);
static inline ipaa_alias_info ipaa_callsite_get_pair_alias_info (struct
								 cgraph_edge
								 *, int, int);
static inline void ipaa_callsite_set_alias_info (struct cgraph_edge *,
						 ipaa_alias_info, int);
static inline ipaa_alias_info ipaa_callsite_get_alias_info (struct 
							    cgraph_edge *, 
							    int);
static inline void ipaa_callsite_compute_param (struct cgraph_edge *);

/* ipaa_varslist interface.  */

/* This structure is used to keep list of variables 
   than need to be checked on modification or address 
   taken inside method.  */
struct ipaa_varslist
{
  bool modified;
  tree var_decl;
  tree call_expr;
  struct ipaa_varslist *next_var;
};

typedef struct ipaa_varslist *ipaa_varslist_p;
static alloc_pool ipaa_varslist_pool;

static inline ipaa_varslist_p ipaa_create_varslist_node (void);
static inline bool ipaa_varslist_not_empty (ipaa_varslist_p);
static inline tree ipaa_varslist_var (ipaa_varslist_p);
static inline void ipaa_varslist_var_set (ipaa_varslist_p, tree);
static inline tree ipaa_varslist_call_expr (ipaa_varslist_p);
static inline void ipaa_varslist_call_expr_set (ipaa_varslist_p, tree);
static inline bool ipaa_varslist_modified (ipaa_varslist_p);
static inline void ipaa_varslist_modified_set (ipaa_varslist_p, bool);
static inline ipaa_varslist_p ipaa_varslist_next_var (ipaa_varslist_p);
static inline void ipaa_varslist_next_var_set (ipaa_varslist_p,
					       ipaa_varslist_p);
static ipaa_varslist_p ipaa_varslist_init (void);
static ipaa_varslist_p ipaa_add_var (ipaa_varslist_p, tree, tree);
static void ipaa_destroy_varslist (void);
static void ipaa_print_varslist (FILE *, ipaa_varslist_p);
static bool ipaa_varslist_is_modified (tree, ipaa_varslist_p);
static ipaa_varslist_p ipaa_varslist_find_var (tree, ipaa_varslist_p);

/* This structure is used to pass data to walk_tree function.  */
struct walk_tree_data
{
  ipaa_varslist_p list;
  tree stmt;
};

typedef struct walk_tree_data *walk_tree_data_p;

/* ipaa analysis functions.  */
static bool ipaa_is_address_of_local_array (tree, tree *, tree);
static bool ipaa_is_non_modified_formal (struct cgraph_edge *, int,
					 struct cgraph_node *);
static void ipaa_callsites_init_aliasing (struct cgraph_node *);
static void ipaa_method_compute_modify (struct cgraph_node *,
					ipaa_varslist_p);
static void ipaa_modify_walk_tree_1 (tree *, void *);
static tree ipaa_modify_walk_tree (tree * t, int *, void *);
static void ipaa_meet_alias_info (ipaa_alias_info, ipaa_alias_info,
				  ipaa_alias_info *);
static void ipaa_init_stage (void);
static void ipaa_propagate_stage (void);
static void ipaa_insert_stage (void);

/* ipaa debug interface.  */
static void ipaa_structures_print (FILE *);
static void ipaa_callsite_alias_info_print (FILE *);
static void ipaa_method_alias_info_print (FILE *);
static void ipaa_callsite_addr_local_array_print (FILE *, tree);
static void ipaa_callsite_param_trees_print (FILE *);
static void ipaa_tree_alias_info_print (FILE *);

/* auxilary functions to get alias info connected to func_decl.  */
static int ipaa_get_func_parms_count (tree);
static enum alias_info_d ipaa_get_aliasing (tree, int, int);

/* ipaa_method interface.  */

/* Create ipaa_alias_info for method MT.  */
static inline void
ipaa_method_create_alias_info (struct cgraph_node *mt)
{
  IPA_NODE_REF (mt)->ipaa_aliasing =
    xcalloc (ipaa_method_get_num_pairs_args (mt), sizeof (ipaa_alias_info));
}

/* Create and initialize the ipaa_alias_info of the method MT
   with DO_NOT_KNOW values.  */
static void
ipaa_method_alias_init (struct cgraph_node *mt)
{
  int i;

  ipaa_method_create_alias_info (mt);
  for (i = 0; i < ipaa_method_get_num_pairs_args (mt); i++)
    ipaa_method_set_alias_info (mt, DO_NOT_KNOW, i);
}

/* Set ipaa_num_pairs_args to I.   */
static inline void
ipaa_method_set_num_pairs_args (struct cgraph_node *mt, int i)
{
  IPA_NODE_REF (mt)->ipaa_num_pairs_args = i;
}

/* Return ipaa_num_pairs_args.  */
static inline int
ipaa_method_get_num_pairs_args (struct cgraph_node *mt)
{
  return IPA_NODE_REF (mt)->ipaa_num_pairs_args;
}

/* Count how many pairs of parameters the method MT has.  */
static inline void
ipaa_method_compute_num_pairs (struct cgraph_node *mt)
{
  int n = ipa_method_formal_count (mt);
  ipaa_method_set_num_pairs_args (mt, n * (n - 1) / 2);
}

/* Set ipaa_alias_info for the pair of formals (I,J).  */
static inline void
ipaa_method_set_pair_alias_info (struct cgraph_node *mt,
				 ipaa_alias_info alias, int i, int j)
{
  int n = ipa_method_formal_count (mt);
  int k = (i * n) - ((i + 1) * i) / 2 + (j - i - 1);
  gcc_assert (j > i && i >= 0 && j >= 1 && i < (n - 1) && j < n);
  IPA_NODE_REF (mt)->ipaa_aliasing[k] = alias;
}

/* Get ipaa_alias_info for the pair of formals (I,J).  */
static inline ipaa_alias_info
ipaa_method_get_pair_alias_info (struct cgraph_node *mt, int i, int j)
{
  int n = ipa_method_formal_count (mt);
  int k = (i * n) - ((i + 1) * i) / 2 + (j - i - 1);
  gcc_assert (j > i && i >= 0 && j >= 1 && i < (n - 1) && j < n); 
  return IPA_NODE_REF (mt)->ipaa_aliasing[k];
}

/* Set I's value of the array ipaa_aliasing to be equal to ALIAS.  */
static inline void
ipaa_method_set_alias_info (struct cgraph_node *mt, ipaa_alias_info alias,
			    int i)
{
  int n = ipaa_method_get_num_pairs_args (mt);
  gcc_assert (i >= 0 && i < n);
  IPA_NODE_REF (mt)->ipaa_aliasing[i] = alias;
}

/* Return I's value of the array ipaa_aliasing.  */
static inline ipaa_alias_info
ipaa_method_get_alias_info (struct cgraph_node *mt, int i)
{
  int n = ipaa_method_get_num_pairs_args (mt);
  gcc_assert (i >= 0 && i < n);
  return IPA_NODE_REF (mt)->ipaa_aliasing[i];
}

/* ipaa_callsite interface.  */

/* Create ipaa_alias_info for callsite CS.  */
static inline void
ipaa_callsite_create_alias_info (struct cgraph_edge *cs)
{
  IPA_EDGE_REF (cs)->ipaa_aliasing =
    xcalloc (ipaa_callsite_get_num_pairs_args (cs), sizeof (ipaa_alias_info));
}

/* Create and initialize the ipaa_alias_info of the callsite CS
   with DO_NOT_KNOW values.  */
static void
ipaa_callsite_alias_init (struct cgraph_edge *cs)
{
  int i;

  ipaa_callsite_create_alias_info (cs);
  for (i = 0; i < ipaa_callsite_get_num_pairs_args (cs); i++)
    ipaa_callsite_set_alias_info (cs, DO_NOT_KNOW, i);
}

/* Set ipaa_num_pairs_args to I.   */
static inline void
ipaa_callsite_set_num_pairs_args (struct cgraph_edge *cs, int i)
{
  IPA_EDGE_REF (cs)->ipaa_num_pairs_args = i;
}

/* Return ipaa_num_pairs_args.  */
static inline int
ipaa_callsite_get_num_pairs_args (struct cgraph_edge *cs)
{
  return IPA_EDGE_REF (cs)->ipaa_num_pairs_args;
}

/* Counts how many pairs of parameters the callsite CS has.  */
static inline void
ipaa_callsite_compute_num_pairs (struct cgraph_edge *cs)
{
  int n = ipa_callsite_param_count (cs);
  ipaa_callsite_set_num_pairs_args (cs, n * (n - 1) / 2);
}

/* Set ipaa_alias_info for the pair of actual parameters (I,J).  */
static inline void
ipaa_callsite_set_pair_alias_info (struct cgraph_edge *cs,
				   ipaa_alias_info alias, int i, int j)
{
  int n = ipa_callsite_param_count (cs);
  int k = (i * n) - ((i + 1) * i) / 2 + (j - i - 1);
  gcc_assert (j > i && i >= 0 && j >= 1 && i < (n - 1) && j < n);
  IPA_EDGE_REF (cs)->ipaa_aliasing[k] = alias;
}

/* Get ipaa_alias_info for the pair of formals (I,J).  */
static inline ipaa_alias_info
ipaa_callsite_get_pair_alias_info (struct cgraph_edge *cs, int i, int j)
{
  int n = ipa_callsite_param_count (cs);
  int k = (i * n) - ((i + 1) * i) / 2 + (j - i - 1);
  gcc_assert (j > i && i >= 0 && j >= 1 && i < (n - 1) && j < n);
  return IPA_EDGE_REF (cs)->ipaa_aliasing[k];
}

/* Set I's value of the array ipaa_aliasing to be equal to ALIAS.  */
static inline void
ipaa_callsite_set_alias_info (struct cgraph_edge *cs, ipaa_alias_info alias,
			      int i)
{
  int n = ipaa_callsite_get_num_pairs_args (cs);
  gcc_assert (i >= 0 && i < n);
  IPA_EDGE_REF (cs)->ipaa_aliasing[i] = alias;
}

/* Return I's value of the array ipaa_aliasing.  */
static inline ipaa_alias_info
ipaa_callsite_get_alias_info (struct cgraph_edge *cs, int i)
{
  int n = ipaa_callsite_get_num_pairs_args (cs);
  gcc_assert (i >= 0 && i < n);
  return IPA_EDGE_REF (cs)->ipaa_aliasing[i];
}

/* This function compute alias_info for each pair of actual 
   parametrers of callsite CS.  */
static void
ipaa_callsite_compute_param (struct cgraph_edge *cs)
{
  if (ipaa_callsite_get_num_pairs_args (cs) == 0)
    return;
  ipaa_callsite_alias_init (cs);
}

/* ipaa_varslist interface.  */

/* Create the varslist node.  */
static inline ipaa_varslist_p
ipaa_create_varslist_node (void)
{
  return pool_alloc (ipaa_varslist_pool);
}

/* Check that varslist is not empty.  */
static inline bool
ipaa_varslist_not_empty (ipaa_varslist_p vl)
{
  return (vl != NULL);
}

/* Get var tree from varslist's node.  */
static inline tree
ipaa_varslist_var (ipaa_varslist_p vl)
{
  return vl->var_decl;
}

/* Set var_decl of varslist's node to be equal to VAR.  */
static inline void
ipaa_varslist_var_set (ipaa_varslist_p vl, tree var)
{
  vl->var_decl = var;
}

/* Get call_expr tree from varslist's node.  */
static inline tree
ipaa_varslist_call_expr (ipaa_varslist_p vl)
{
  return vl->call_expr;
}

/* Set call_expr of varslist's node to be equal to CALL_EXPR.  */
static inline void
ipaa_varslist_call_expr_set (ipaa_varslist_p vl, tree call_expr)
{
  vl->call_expr = call_expr;
}

/* Get modified from varslist's node.  */
static inline bool
ipaa_varslist_modified (ipaa_varslist_p vl)
{
  return vl->modified;
}

/* Set modified of varslist's node to be equal to MOD.  */
static inline void
ipaa_varslist_modified_set (ipaa_varslist_p vl, bool mod)
{
  vl->modified = mod;
}

/* Get next varslist's node.  */
static inline ipaa_varslist_p
ipaa_varslist_next_var (ipaa_varslist_p vl)
{
  return vl->next_var;
}

/* Set varslist node VL1 to point to  varslist node VL2.  */
static inline void
ipaa_varslist_next_var_set (ipaa_varslist_p vl1, ipaa_varslist_p vl2)
{
  vl1->next_var = vl2;
}

/* Initialize varslist.  */
static ipaa_varslist_p
ipaa_varslist_init (void)
{
  ipaa_varslist_pool =
    create_alloc_pool ("List of variables", sizeof (struct ipaa_varslist),
		       100);

  return NULL;
}

/* Add a var VAR to the varslist.  */
static ipaa_varslist_p
ipaa_add_var (ipaa_varslist_p vl, tree var, tree call_expr)
{
  ipaa_varslist_p temp;

  temp = ipaa_create_varslist_node ();
  ipaa_varslist_var_set (temp, var);
  ipaa_varslist_call_expr_set (temp, call_expr);
  ipaa_varslist_modified_set (temp, false);
  ipaa_varslist_next_var_set (temp, vl);
  return temp;
}

/* Destroy varslist.  */
static void
ipaa_destroy_varslist (void)
{
  free_alloc_pool (ipaa_varslist_pool);
}

/* Print varslist.  */
static void
ipaa_print_varslist (FILE * f, ipaa_varslist_p vl)
{
  ipaa_varslist_p p = vl;

  while (p)
    {
      fprintf (f, "\nvar %s",
	       IDENTIFIER_POINTER (DECL_NAME (ipaa_varslist_var (p))));
      if (ipaa_varslist_modified (p))
	fprintf (f, " is modified");
      else
	fprintf (f, " is not modified");
      p = ipaa_varslist_next_var (p);
    }
}

/* Return false if var is in worklist and was not modified, and true otherwise.  */
static bool
ipaa_varslist_is_modified (tree var, ipaa_varslist_p list)
{
  ipaa_varslist_p p = list;
  gcc_assert (var != NULL);
  while (p)
    {
      if (ipaa_varslist_var (p) == var)
	return ipaa_varslist_modified (p);
      p = ipaa_varslist_next_var (p);
    }
  return true;
}

/* Starting from node pointed by the LIST, this function returns pointer 
   to the fisrt node that contain var and NULL otherwise.  */
static ipaa_varslist_p
ipaa_varslist_find_var (tree var, ipaa_varslist_p list)
{
  ipaa_varslist_p p = list;
  gcc_assert (var != NULL);
  while (p)
    {
      if (ipaa_varslist_var (p) == var)
	return p;
      p = ipaa_varslist_next_var (p);
    }
  return NULL;
}

/* ipaa analysis functions.  */

/* This function return true if ARG is an address of array defined 
   in function with DECL_NAME CURR_FUNC. In this case ARRAY_NAME contains 
   the ARRAY_DECL of array. Otherwise this function returns false and
   ARRAY_DECL points to NULL.  */
static bool
ipaa_is_address_of_local_array (tree arg, tree * array_decl, tree curr_func)
{
  tree arg_0, func;

  *array_decl = NULL;

  if (arg)
    {
      if (TREE_CODE (arg) == TREE_LIST &&
	  TREE_CODE (TREE_VALUE (arg)) == ADDR_EXPR &&
	  TREE_CODE (TREE_TYPE (TREE_VALUE (arg))) == POINTER_TYPE)
	{
	  if (TREE_INVARIANT (TREE_VALUE (arg)))
	    {
	      arg_0 = TREE_OPERAND (TREE_VALUE (arg), 0);
	      if (TREE_CODE (arg_0) == VAR_DECL)
		{
		  if (TREE_CODE (TREE_TYPE (arg_0)) == ARRAY_TYPE)
		    {
		      if (DECL_CONTEXT (arg_0))
			{
			  if (TREE_CODE (DECL_CONTEXT (arg_0)) ==
			      FUNCTION_DECL)
			    {
			      func = DECL_NAME (DECL_CONTEXT (arg_0));
			      if (func == curr_func)
				{
				  *array_decl = arg_0;
				  return true;
				}
			    }
			}
		    }
		}
	    }
	}
    }
  return false;
}

/* This function returns true if I's arg of callsite CS is non modified 
   formal of the method MT, and false otherwise.  */
static bool
ipaa_is_non_modified_formal (struct cgraph_edge *cs, int i,
			     struct cgraph_node *mt)
{
  struct ipa_jump_func *jump_func;
  enum jump_func_type type;
  union parameter_info *info_type;

  jump_func = ipa_callsite_param (cs, i);
  type = get_type (jump_func);
  info_type = ipa_jf_get_info_type (jump_func);

  if (type == FORMAL_IPATYPE)
    {
      if (!ipa_method_is_modified (mt, info_type->formal_id))
	return true;
    }

  return false;
}

/* For each callsite of method MT this function calculates 
   ipa_edge.ipaa_aliasing data.  */
static void
ipaa_callsites_init_aliasing (struct cgraph_node *mt)
{
  ipaa_varslist_p list = ipaa_varslist_init ();
  tree call_tree;
  struct cgraph_edge *cs;

  for (cs = mt->callees; cs; cs = cs->next_callee)
    {
      tree arg;

      call_tree = ipa_callsite_tree (cs);
      if (TREE_CODE (call_tree) != CALL_EXPR)
	abort ();
      arg = TREE_OPERAND (call_tree, 1);
      for (; arg != NULL_TREE; arg = TREE_CHAIN (arg))
	{
	  tree array_decl = NULL;
	  if (ipaa_is_address_of_local_array (arg, &array_decl,
					      DECL_NAME (mt->decl)))
	    list = ipaa_add_var (list, array_decl, call_tree);
	}
    }

  if (dump_file)
    {
      fprintf (dump_file,
	       "\nVarslist of the function %s before computing modify:\n",
	       cgraph_node_name (mt));
      ipaa_print_varslist (dump_file, list);
    }
  ipaa_method_compute_modify (mt, list);
  if (dump_file)
    {
      fprintf (dump_file,
	       "\nVarslist of the function %s after computing modify:\n",
	       cgraph_node_name (mt));
      ipaa_print_varslist (dump_file, list);
    }

  for (cs = mt->callees; cs; cs = cs->next_callee)
    {
      tree arg1, arg2;
      int i, j;

      /* This "if" handles the case with variable 
         number of parameters. The count of such callsites is zeroed 
         by ipa_init_stage function.  */
      if (ipa_callsite_param_count (cs) == 0)
	continue;

      call_tree = ipa_callsite_tree (cs);
      if (TREE_CODE (call_tree) != CALL_EXPR)
	abort ();
      arg1 = TREE_OPERAND (call_tree, 1);
      for (i = 0; arg1 != NULL_TREE; arg1 = TREE_CHAIN (arg1), i++)
	{
	  for (j = i + 1, arg2 = TREE_CHAIN (arg1); arg2 != NULL_TREE;
	       arg2 = TREE_CHAIN (arg2), j++)
	    {
	      tree array_decl1 = NULL;
	      tree array_decl2 = NULL;

	      bool arg1_is_non_mod_address_of_local_array =
		ipaa_is_address_of_local_array (arg1, &array_decl1,
						DECL_NAME (mt->decl)) &&
		!ipaa_varslist_is_modified (array_decl1, list);
	      bool arg2_is_non_mod_address_of_local_array =
		ipaa_is_address_of_local_array (arg2, &array_decl2,
						DECL_NAME (mt->decl)) &&
		!ipaa_varslist_is_modified (array_decl2, list);
	      bool arg1_is_non_mod_formal =
		ipaa_is_non_modified_formal (cs, i, mt);
	      bool arg2_is_non_mod_formal =
		ipaa_is_non_modified_formal (cs, j, mt);

	      /* arg1 and arg2 are local arrays.  */
	      if (arg1_is_non_mod_address_of_local_array &&
		  arg2_is_non_mod_address_of_local_array)
		{
		  if (array_decl1 == array_decl2)
		    /* They are aliased.  */
		    ipaa_callsite_set_pair_alias_info (cs, MUST_ALIAS, i, j);
		  else
		    /* They are not aliased.  */
		    ipaa_callsite_set_pair_alias_info (cs, NON_ALIAS, i, j);
		}
	      else if ((arg1_is_non_mod_formal &&
			arg2_is_non_mod_address_of_local_array) ||
		       (arg2_is_non_mod_formal &&
			arg1_is_non_mod_address_of_local_array))
		{
		  ipaa_callsite_set_pair_alias_info (cs, NON_ALIAS, i, j);
		}
	    }
	}
    }

  ipaa_destroy_varslist ();
}

/* This fucntion checks whether variables from the LIST were 
   modified in the method MT.  */
static void
ipaa_method_compute_modify (struct cgraph_node *mt, ipaa_varslist_p list)
{
  tree decl;
  tree body;
  basic_block bb;
  struct function *func;
  block_stmt_iterator bsi;
  tree stmt;
  ipaa_varslist_p p = list;
  struct walk_tree_data wt_data;

  wt_data.list = list;
  wt_data.stmt = NULL;

  decl = mt->decl;
  /* this "if" is legacy from ipcp.  */
  /* ??? Handle pending sizes case. Set all vars 
     in list to be modified.  */
  if (DECL_UNINLINABLE (decl))
    {
      while (ipaa_varslist_not_empty (p))
	{
	  ipaa_varslist_modified_set (p, true);
	  p = ipaa_varslist_next_var (p);
	}
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
	    wt_data.stmt = stmt;
	    ipaa_modify_walk_tree_1 (&stmt, (void *) &wt_data);
	  }
      }
    }
}

/* Walks over TP tree to find whether vars collected in LIST are modified in
   method MT (MT and LIST are parts of data).  */
static void
ipaa_modify_walk_tree_1 (tree * tp, void *data)
{
  walk_tree (tp, ipaa_modify_walk_tree, data, NULL);
}

/* This function is called by walk_tree. If a varible in the LIST
   is modified within the method MD or its address is taken, 
   the "modified" field of this variable in the LIST is set to be true.  */
static tree
ipaa_modify_walk_tree (tree * tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		       void *data)
{
  tree t = *tp;

  struct walk_tree_data wt_data = *((walk_tree_data_p) data);
  ipaa_varslist_p list = wt_data.list;
  tree stmt = wt_data.stmt;

  switch (TREE_CODE (t))
    {
    case MODIFY_EXPR:
      if (TREE_CODE (TREE_OPERAND (t, 0)) == VAR_DECL)
	{
	  tree var_decl = TREE_OPERAND (t, 0);
	  ipaa_varslist_p p = list;
	  while (p)
	    {
	      p = ipaa_varslist_find_var (var_decl, p);
	      if (p)
		{
		  if (ipaa_varslist_call_expr (p) != stmt)
		    ipaa_varslist_modified_set (p, true);
		  p = ipaa_varslist_next_var (p);
		}
	    }
	}
      break;
    case ADDR_EXPR:
      /* If the parameter's address is taken, 
         it could be modified.  */

      if (TREE_CODE (TREE_OPERAND (t, 0)) == VAR_DECL)
	{
	  tree var_decl = TREE_OPERAND (t, 0);
	  ipaa_varslist_p p = list;
	  while (p)
	    {
	      p = ipaa_varslist_find_var (var_decl, p);
	      if (p)
		{
		  if (ipaa_varslist_call_expr (p) != stmt)
		    ipaa_varslist_modified_set (p, true);
		  p = ipaa_varslist_next_var (p);
		}
	    }
	}
      break;
    case ASM_EXPR:
      /* Asm code could modify any of the vars.  */
      {
	ipaa_varslist_p p = list;
	while (p)
	  {
	    ipaa_varslist_modified_set (p, true);
	    p = ipaa_varslist_next_var (p);
	  }
      }
      break;
    default:
      break;
    }
  return NULL;
}

/* Meet function for aliasing info:
   meet (DO_NOT_KNOW, x) = x
   meet (MAY_ALIAS, x) = MAY_ALIAS
   meet (MUST_ALIAS, NON_ALIAS) = MAY_ALIAS  */
static void
ipaa_meet_alias_info (ipaa_alias_info alias1, ipaa_alias_info alias2,
		      ipaa_alias_info * res_alias_p)
{
  gcc_assert (res_alias_p != NULL);

  if (alias1 == alias2)
    *res_alias_p = alias1;
  else if (alias1 == MAY_ALIAS || alias2 == MAY_ALIAS)
    *res_alias_p = MAY_ALIAS;
  else if ((alias1 == DO_NOT_KNOW && alias2 == NON_ALIAS) ||
	   (alias2 == DO_NOT_KNOW && alias1 == NON_ALIAS))
    *res_alias_p = NON_ALIAS;
  else if ((alias1 == DO_NOT_KNOW && alias2 == MUST_ALIAS) ||
	   (alias2 == DO_NOT_KNOW && alias1 == MUST_ALIAS))
    *res_alias_p = MUST_ALIAS;
  else if ((alias1 == MUST_ALIAS && alias2 == NON_ALIAS) ||
	   (alias2 == MUST_ALIAS && alias1 == NON_ALIAS))
    *res_alias_p = MAY_ALIAS;

  return;
}

/* This funciton calculates alias info and initializes
   ipa_node and ipa_edge structures.  */
static void
ipaa_init_stage (void)
{
  struct cgraph_node *node;
  struct cgraph_edge *cs;

  ipaa_was_executed = 0;

  ipa_calc_formals_counts ();

  for (node = cgraph_nodes; node; node = node->next)
    {
      ipa_method_compute_tree_map (node);
      ipa_method_compute_modify (node);
      ipaa_method_compute_num_pairs (node);
      ipaa_method_alias_init (node);
    }

  for (node = cgraph_nodes; node; node = node->next)
    {
      for (cs = node->callees; cs; cs = cs->next_callee)
	{
	  /* Building jump functions.  */	  
	  ipaa_callsite_compute_num_pairs (cs);
	  ipa_callsite_compute_param (cs);
	  ipaa_callsite_alias_init (cs);
	}
      /* Initialize aliasing info at callsites by doing 
         intraprocedural alias analysis.  */
      ipaa_callsites_init_aliasing (node);
    }
}

/* The algorithm propagates aliasing info from the pairs of 
   actual function parameters to the pairs of formals.  */
static void
ipaa_propagate_stage (void)
{
  int i, j, i1, j1;
  ipa_methodlist_p wl;
  struct cgraph_node *mt, *callee;
  struct cgraph_edge *cs;
  int count;
  struct ipa_jump_func *jump_func1, *jump_func2;
  ipaa_alias_info meet_res_alias_info = DO_NOT_KNOW;
  ipaa_alias_info callsite_alias_info;
  ipaa_alias_info method_alias_info;
  struct cgraph_node *node;
  enum jump_func_type type1, type2;
  union parameter_info *info_type1, *info_type2;

  wl = ipa_methodlist_init_empty ();
  for (node = cgraph_nodes; node; node = node->next)
    {
      for (cs = node->callees; cs; cs = cs->next_callee)
	{
	  callee = ipa_callsite_callee (cs);
	  count = ipa_callsite_param_count (cs);
	  for (i = 0; i < count - 1; i++)
	    {
	      for (j = i + 1; j < count; j++)
		{
		  callsite_alias_info =
		    ipaa_callsite_get_pair_alias_info (cs, i, j);

		  method_alias_info =
		    ipaa_method_get_pair_alias_info (callee, i, j);
		  
		  ipaa_meet_alias_info (callsite_alias_info,
					method_alias_info,
					&meet_res_alias_info);
		  if (meet_res_alias_info != method_alias_info)
		    {
		      if (!ipa_method_is_modified (callee, i) &&
			  !ipa_method_is_modified (callee, j))
			{
			  ipaa_method_set_pair_alias_info (callee,
							   meet_res_alias_info, i,
							   j);
			  ipa_add_method (&wl, callee);
			}
		    }
		}
	    }
	}
    }

  while (ipa_methodlist_not_empty (wl))
    {
      mt = ipa_remove_method (&wl);
      /* This "if" support functions with variable number of parameters.  */
      if (!ipa_method_formal_count (mt))
	continue;
      for (cs = mt->callees; cs; cs = cs->next_callee)
	{
	  callee = ipa_callsite_callee (cs);
	  count = ipa_callsite_param_count (cs);
	  for (i = 0; i < count - 1; i++)
	    {
	      for (j = i + 1; j < count; j++)
		{
		  jump_func1 = ipa_callsite_param (cs, i);
		  type1 = get_type (jump_func1);
		  info_type1 = ipa_jf_get_info_type (jump_func1);

		  jump_func2 = ipa_callsite_param (cs, j);
		  type2 = get_type (jump_func2);
		  info_type2 = ipa_jf_get_info_type (jump_func2);

		  if (type1 == FORMAL_IPATYPE && type2 == FORMAL_IPATYPE)
		    {
		      i1 = info_type1->formal_id;
		      j1 = info_type2->formal_id;

		      if (i1 < j1)
			callsite_alias_info =
			  ipaa_method_get_pair_alias_info (mt, i1, j1);
		      else if (i1 == j1)
			callsite_alias_info = MUST_ALIAS;
		      else
			callsite_alias_info =
			  ipaa_method_get_pair_alias_info (mt, j1, i1);

		      method_alias_info =
			ipaa_method_get_pair_alias_info (callee, i, j);

		      ipaa_meet_alias_info (callsite_alias_info,
					    method_alias_info,
					    &meet_res_alias_info);
		      if (meet_res_alias_info != method_alias_info)
			{
			  if (!ipa_method_is_modified (callee, i) &&
			      !ipa_method_is_modified (callee, j))
			    {
			      ipaa_method_set_pair_alias_info (callee,
							       meet_res_alias_info,
							       i, j);
			      ipa_add_method (&wl, callee);
			    }
			}
		    }
		}
	    }
	}
    }
}

/* This function saves aliasing info of formals calculated 
   at stage 2 for further compiler passes.  */
static void
ipaa_insert_stage (void)
{
  struct cgraph_node *node;

    for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;
      var_ann_t var_ann = get_var_ann (decl);
      int i;

      gcc_assert (var_ann);
      
      if (var_ann->ipa_alias)
	  free (var_ann->ipa_alias);

      if (ipaa_method_get_num_pairs_args (node) == 0)
	{
	  var_ann->ipa_alias = NULL;
	  continue;
	}

      var_ann->ipa_alias = xcalloc (ipaa_method_get_num_pairs_args (node), 
					  sizeof (enum alias_info_d));
     
      /* Copy alis_info from cgraph_node to function declaration tree.
         We are not using memcpy here to keep freedom of choice of data 
	 structure for alias info inside ipaa and in function declaration.  */

      gcc_assert (var_ann->ipa_alias);

      for (i = 0; i < ipaa_method_get_num_pairs_args (node); i++)
	var_ann->ipa_alias [i] = ipaa_method_get_alias_info (node, i);
    }

    ipaa_was_executed = 1;
}

/* ipaa debug interface.  */

/* Prints all ipaa data structures.  */
static void
ipaa_structures_print (FILE * f)
{
  ipa_method_tree_print (f);
  ipa_method_modify_print (f);
  ipaa_method_alias_info_print (f);
  ipaa_callsite_alias_info_print (f);
  ipaa_callsite_param_trees_print (f);
}

/* Prints ipa_edge.ipaa_aliasing data.  */
static void
ipaa_callsite_alias_info_print (FILE * f)
{
  struct cgraph_node *node;
  struct cgraph_edge *cs;
  int i, j, count;

  fprintf (f, "\nALIAS PRINT CALLSITES\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      for (cs = node->callees; cs; cs = cs->next_callee)
	{
	  fprintf (f, "callsite  %s ", cgraph_node_name (node));
	  fprintf (f, "-> %s :: \n", cgraph_node_name (cs->callee));
	  count = ipa_callsite_param_count (cs);
	  for (i = 0; i < count - 1; i++)
	    {
	      for (j = i + 1; j < count; j++)
		{
		  fprintf (f, "alias_info [%d, %d] = ", i, j);
		  if (ipaa_callsite_get_pair_alias_info (cs, i, j) ==
		      MAY_ALIAS)
		    fprintf (f, "MAY_ALIAS\n");
		  else if (ipaa_callsite_get_pair_alias_info (cs, i, j) ==
			   MUST_ALIAS)
		    fprintf (f, "MUST_ALIAS\n");
		  else if (ipaa_callsite_get_pair_alias_info (cs, i, j) ==
			   NON_ALIAS)
		    fprintf (f, "NON_ALIAS\n");
		  else
		    fprintf (f, "DO_NOT_KNOW\n");
		}
	    }
	}
    }
}

void
ipaa_func_tree_alias_info_print (tree func_decl, FILE * f)
{
  int i, j, count;

  gcc_assert (f);

  fprintf (f, "\nalias info of function %s:\n", IDENTIFIER_POINTER (DECL_NAME (func_decl)));
  if (ipaa_was_executed == 0)
    {
      fprintf (f, "ipaa was not executed\n");
      return;
    }

  count = ipaa_get_func_parms_count (func_decl);
  for (i = 0; i < count - 1; i++)
    {
      for (j = i + 1; j < count; j++)
	{
	  fprintf (f, "alias_info [%d, %d] = ", i, j);
	  if (ipaa_get_aliasing (func_decl, i, j) == MAY_ALIAS)
	    fprintf (f, "MAY_ALIAS\n");
	  else if (ipaa_get_aliasing (func_decl, i, j) == MUST_ALIAS)
	    fprintf (f, "MUST_ALIAS\n");
	  else if (ipaa_get_aliasing (func_decl, i, j) == NON_ALIAS)
	    fprintf (f, "NON_ALIAS\n");
	  else if (ipaa_get_aliasing (func_decl, i, j) == DO_NOT_KNOW)
	    fprintf (f, "DO_NOT_KNOW\n");
	  else
	    fprintf (f, "the value is incorrect\n");
	}
    }
}

static void
ipaa_tree_alias_info_print (FILE * f)
{
  struct cgraph_node *node;

  fprintf (f, "\nTREE ALIAS INFO OF FUNC_DECL's \n");
  for (node = cgraph_nodes; node; node = node->next)
    {      
      tree decl = node->decl;
      ipaa_func_tree_alias_info_print (decl, f);
    }
}

/* Prints ipa_node.ipaa_aliasing data.  */
static void
ipaa_method_alias_info_print (FILE * f)
{
  struct cgraph_node *node;
  int i, j, count;

  fprintf (f, "\nALIAS PRINT METHODS\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      fprintf (f, "printing alias info for %s\n", cgraph_node_name (node));
      count = ipa_method_formal_count (node);
      for (i = 0; i < count - 1; i++)
	{
	  for (j = i + 1; j < count; j++)
	    {
	      fprintf (f, "alias_info [%d, %d] = ", i, j);
	      if (ipaa_method_get_pair_alias_info (node, i, j) == MAY_ALIAS)
		fprintf (f, "MAY_ALIAS\n");
	      else if (ipaa_method_get_pair_alias_info (node, i, j) ==
		       MUST_ALIAS)
		fprintf (f, "MUST_ALIAS\n");
	      else if (ipaa_method_get_pair_alias_info (node, i, j) ==
		       NON_ALIAS)
		fprintf (f, "NON_ALIAS\n");
	      else		/* DO_NOT_KNOW  */
		fprintf (f, "DO_NOT_KNOW\n");
	    }
	}
    }
}

/* This function prints address of local array when it appears as argument ARG 
   of callsite.  */
static void
ipaa_callsite_addr_local_array_print (FILE * f, tree arg)
{
  tree arg_0;

  fprintf (f, "\n");
  fprintf (f, "arg code is: ");
  fprintf (f, "%s\n", tree_code_name[(int) TREE_CODE (arg)]);
  fprintf (f, "arg value code is: ");
  fprintf (f, "%s\n", tree_code_name[(int) TREE_CODE (TREE_VALUE (arg))]);
  fprintf (f, "arg value type code is: ");
  fprintf (f, "%s\n",
	   tree_code_name[(int) TREE_CODE (TREE_TYPE (TREE_VALUE (arg)))]);
  fprintf (f, "arg value type type code is:");
  fprintf (f, "%s\n",
	   tree_code_name[(int)
			  TREE_CODE (TREE_TYPE
				     (TREE_TYPE (TREE_VALUE (arg))))]);
  fprintf (f, "arg value is invariant: ");
  if (TREE_INVARIANT (TREE_VALUE (arg)))
    fprintf (f, "yes\n");
  else
    fprintf (f, "no\n");
  if (TREE_CODE (TREE_VALUE (arg)) == ADDR_EXPR)
    {
      arg_0 = TREE_OPERAND (TREE_VALUE (arg), 0);
      fprintf (f, "code of arg 0 of arg is: ");
      fprintf (f, "%s\n", tree_code_name[(int) TREE_CODE (arg_0)]);
      if (TREE_CODE (arg_0) == VAR_DECL)
	{
	  if (DECL_NAME (arg_0))
	    fprintf (f,
		     "arg 0 of arg is var_decl and its name is: %s\n",
		     IDENTIFIER_POINTER (DECL_NAME (arg_0)));
	  if (DECL_CONTEXT (arg_0))
	    {
	      fprintf (f, "context of arg 0 of arg is: %s\n",
		       tree_code_name[(int)
				      TREE_CODE (DECL_CONTEXT (arg_0))]);
	      fprintf (f, "context function name of arg 0 of arg is: %s\n",
		       IDENTIFIER_POINTER (DECL_NAME (DECL_CONTEXT (arg_0))));
	    }
	}
      fprintf (f, "the type of arg 0 of arg is: %s\n",
	       tree_code_name[(int) TREE_CODE (TREE_TYPE (arg_0))]);
    }
}

/* Prints parameter trees of callsites.  */
static void
ipaa_callsite_param_trees_print (FILE * f)
{
  struct cgraph_node *node;
  struct cgraph_edge *cs;
  tree call_tree, arg;

  fprintf (f, "\nCALLSITE PARAM TREES PRINT\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      for (cs = node->callees; cs; cs = cs->next_callee)
	{
	  fprintf (f, "callsite  %s ", cgraph_node_name (node));
	  fprintf (f, "-> %s :: \n", cgraph_node_name (cs->callee));
	  call_tree = ipa_callsite_tree (cs);
	  if (TREE_CODE (call_tree) != CALL_EXPR)
	    abort ();
	  arg = TREE_OPERAND (call_tree, 1);
	  for (; arg != NULL_TREE; arg = TREE_CHAIN (arg))
	    {
	      tree array_decl = NULL;
	      fprintf (f, "\n");
	      if (ipaa_is_address_of_local_array (arg, &array_decl,
						  DECL_NAME (node->decl)))
		ipaa_callsite_addr_local_array_print (f, arg);
	    }
	}
    }
}

/* The ipaa driver.  */
void
ipaa_driver (void)
{
  ipa_nodes_create ();
  ipa_edges_create ();
  if (dump_file)
      fprintf (dump_file, "\nIPA alias analysis start:\n");

  /* 1. Call the init stage to initialize 
     the ipa_node and ipa_edge structures.  */
  ipaa_init_stage ();

  if (dump_file)
    {
      fprintf (dump_file, "\nIPA structures before propagation:\n");
      ipaa_structures_print (dump_file);
    }
  /* 2. Do the interprocedural propagation.  */
  ipaa_propagate_stage (); 
  if (dump_file)
      ipaa_structures_print (dump_file);

  /* 3. Insert the constants found to the functions.  */
  ipaa_insert_stage (); 
  if (dump_file)
    {
      fprintf (dump_file, "\nIPA tree alias info after insertion:\n");
      ipaa_tree_alias_info_print (dump_file);
    }
  /* Free all IPA structures.  */
  ipa_free ();
  ipa_nodes_free ();
  ipa_edges_free ();
  if (dump_file)
    {
      fprintf (dump_file, "\nIPA alias analysis end:\n");
    }
}

/* Given a tree of FORMAL, this function returns its index.
   Calculation of indices starts from 0.  */
static int
ipaa_get_index_of_formal (tree formal, tree func_decl)
{
  tree func_parm, parm;
  int index = 0;

  func_parm = DECL_ARGUMENTS (func_decl);
  for (parm = func_parm; parm; parm = TREE_CHAIN (parm))
    {
      if (parm == formal)
	return index; 
      index++;  
    }

  return -1;
}

static int 
ipaa_get_func_parms_count (tree func_decl)
{
  int count = 0;
  tree func_parm, parm;

  func_parm = DECL_ARGUMENTS (func_decl);
  for (parm = func_parm; parm; parm = TREE_CHAIN (parm))
      count++;  

  return count;
}

/* Given function declaration tree FUNC_DECL and two indices,
 this function returns there aliasing.  */
static enum alias_info_d
ipaa_get_aliasing (tree func_decl, int index1, int index2)
{
  var_ann_t v_ann;
  int n, i, j, k;

  n = ipaa_get_func_parms_count (func_decl);
  gcc_assert (n >= 0);

  if (index1 == index2)
    return DO_NOT_KNOW;

  if (index1 > index2)
    {
      i = index2;
      j = index1;
    }
  else
    {
      i = index1;
      j = index2;
    }

  if (i < 0 || j < 1 || i >= (n-1) || j >= n)
    return DO_NOT_KNOW;

  v_ann = get_var_ann (func_decl);
  gcc_assert (v_ann);

  if (!v_ann->ipa_alias)
    return DO_NOT_KNOW;

  k = (i * n) - ((i + 1) * i) / 2 + (j - i - 1);
  
  return v_ann->ipa_alias [k];

}

/* Given two formals, this function returns their mutual aliasing 
   as calculated by ipaa.  */
enum alias_info_d
ipaa_get_aliasing_of_formals (tree formal1, tree formal2)
{
  tree func_decl;
  int index1, index2;
  enum alias_info_d alias;

  if (!ipaa_was_executed)
      return DO_NOT_KNOW;
    
  if (TREE_CODE (formal1) != PARM_DECL || TREE_CODE (formal2) != PARM_DECL)
      return DO_NOT_KNOW;

  /* Check that FORMAL1 and FORMAL2 belong to the same function.  */
    if (TREE_CODE (DECL_CONTEXT (formal1)) != FUNCTION_DECL 
      || TREE_CODE (DECL_CONTEXT (formal2)) != FUNCTION_DECL
      || DECL_NAME (DECL_CONTEXT (formal1)) != DECL_NAME (DECL_CONTEXT (formal2)))
      {
	return DO_NOT_KNOW;
      }

  func_decl = DECL_CONTEXT (formal1);
  
  
  /* For VAR and PTR we have to find their indices as formals.  */
  index1 = ipaa_get_index_of_formal (formal1, func_decl);
  index2 = ipaa_get_index_of_formal (formal2, func_decl);

  if (index1 == -1 || index2 == -1)
    return DO_NOT_KNOW;

  alias = ipaa_get_aliasing (func_decl, index1, index2);
  
  if (dump_file)
    {
      fprintf (dump_file, "\nipaa is questioned for: %s [%d,%d] = ", 
	       IDENTIFIER_POINTER (DECL_NAME (func_decl)),
	       index1,index2);
      if (alias == MAY_ALIAS)
	fprintf (dump_file, "MAY_ALIAS\n");	
      if (alias == MUST_ALIAS)
	fprintf (dump_file, "MUST_ALIAS\n");	
      if (alias == NON_ALIAS)
	fprintf (dump_file, "NON_ALIAS\n");	
      if (alias == DO_NOT_KNOW)
	fprintf (dump_file, "DO_NOT_KNOW\n");	
      else
	fprintf (dump_file, "incorrect value\n");	
    }

  return alias; 

}

static bool
gate_ipa_alias (void)
{
  return flag_ipa_alias;
}

struct tree_opt_pass pass_ipa_alias = {
  "alias",			/* name */
  gate_ipa_alias,		/* gate */
  ipaa_driver,			/* execute */
  NULL,				/* sub */
  NULL,				/* next */
  0,				/* static_pass_number */
  TV_IPA_ALIAS_ANALYSIS,	/* tv_id */
  0,				/* properties_required */
  PROP_trees,			/* properties_provided */
  0,				/* properties_destroyed */
  0,				/* todo_flags_start */
  TODO_dump_cgraph | TODO_dump_func,	/* todo_flags_finish */
  0				/* letter */
};
