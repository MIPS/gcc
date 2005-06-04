/* Interprocedural constant propagation
   Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by Razya Ladelsky <RAZYA@il.ibm.com>
   
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

/* Interprocedural constant propagation.
   The aim of interprocedural constant propagation (IPCP) is to find which 
   function's argument has the same constant value in each invocation throughout 
   the whole program. For example, for an application consisting of two files, 
   foo1.c, foo2.c:

   foo1.c contains :
   
   int f (int x)
   {
     g (x);
   }
   void main (void)
   {
     f (3);
     h (3);
   }
   
   foo2.c contains :
   
   int h (int y)
   {
     g (y);
   }
   int g (int y)
   {
     printf ("value is %d",y);
   }
   
   The IPCP algorithm will find that g's formal argument y
   is always called with the value 3.
   
   The algorithm used is based on "Interprocedural Constant Propagation",
   by Challahan David, Keith D Cooper, Ken Kennedy, Linda Torczon, Comp86, 
   pg 152-161
   
   The optimization is divided into three stages:

   First stage - intraprocedural analysis
   =======================================
   This phase computes jump_function and modify information.
   
   A jump function for a callsite represents the values passed as actual 
   arguments
   of the callsite. There are three types of values :
   Formal - the caller's formal parameter is passed as an actual argument.
   Constant - a constant is passed as a an actual argument.
   Unknown - neither of the above.
   
   In order to compute the jump functions, we need the modify information for 
   the formal parameters of methods.
   
   The jump function info, ipa_jump_func, is defined in ipa_edge
   structure (defined in ipa_prop.h and pointed to by cgraph_node->aux)
   The modify info, ipa_modify, is defined in ipa_node structure
   (defined in ipa_prop.h and pointed to by cgraph_edge->aux).
   
   -ipcp_init_stage() is the first stage driver.

   Second stage - interprocedural analysis
   ========================================
   This phase does the interprocedural constant propagation.
   It computes for all formal parameters in the program
   their cval value that may be:
   TOP - unknown.
   BOTTOM - non constant.
   CONSTANT_TYPE - constant value.
   
   Cval of formal f will have a constant value if all callsites to this
   function have the same constant value passed to f.
   
   The cval info, ipcp_formal, is defined in ipa_node structure
   (defined in ipa_prop.h and pointed to by cgraph_edge->aux).
   
   -ipcp_iterate_stage() is the second stage driver.
   
   Third phase - transformation of methods code
   ============================================
   Propagates the constant-valued formals into the function.
   For each method mt, whose parameters are consts, we create a clone.
   
   We insert an assignment statement 'parameter = const' at the beginning
   of the cloned method.
   
   We also need to modify some callsites to call to the cloned methods instead
   of the original ones. For a callsite passing an argument found to be a 
   constant by IPCP, there are two different cases to handle:
   1. A constant is passed as an argument.
   2. A parameter (of the caller) passed as an argument (pass through argument).

   In the first case, the callsite in the original caller should be redirected
   to call the cloned callee.
   In the second case, both the caller and the callee have clones
   and the callsite of the cloned caller would be redirected to call to
   the cloned callee.
   
   The callgraph is updated accordingly.
   
   This update is done in two stages:
   First all cloned methods are created during a traversal of the callgraph,
   during which all callsites are redirected to call the cloned method.
   Then the callsites are traversed and updated as described above.
   
   -ipcp_insert_stage() is the third phase driver.
   
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

/* ipcp_method interface.  */
static inline gcov_type ipcp_method_get_scale (struct cgraph_node *);
static inline void ipcp_method_set_scale (struct cgraph_node *, gcov_type);
static inline bool ipcp_method_is_cloned (struct cgraph_node *);
static inline void ipcp_method_set_orig_node (struct cgraph_node *,
					      struct cgraph_node *);
static inline struct cgraph_node *ipcp_method_orig_node (struct cgraph_node *);
static void ipcp_cloned_create (struct cgraph_node *orig_node,
				struct cgraph_node *new_node);
static inline void ipcp_formal_create (struct cgraph_node *);
static inline void ipcp_method_cval_set (struct cgraph_node *, int,
					 struct ipcp_formal *);
static inline struct ipcp_formal *ipcp_method_cval (struct cgraph_node *,
						    int);
static inline void ipcp_method_cval_set_cvalue_type (struct cgraph_node *,
						     int, enum Cvalue_type);
static void ipcp_cval_meet (struct ipcp_formal *, struct ipcp_formal *,
			    struct ipcp_formal *);
static void ipcp_cval_compute (struct ipcp_formal *, struct cgraph_node *,
			       enum Jfunc_type, union info *);
static bool ipcp_cval_changed (struct ipcp_formal *, struct ipcp_formal *);
static bool ipcp_type_is_const (enum Cvalue_type);
static void ipcp_method_cval_init (struct cgraph_node *);
static void ipcp_method_compute_scale (struct cgraph_node *);

/* cval interface.  */
static inline void ipcp_cval_set_cvalue_type (struct ipcp_formal *,
					      enum Cvalue_type);
static inline void ipcp_cval_set_cvalue (struct ipcp_formal *, union info *,
					 enum Cvalue_type);
static inline union info *ipcp_cval_get_cvalue (struct ipcp_formal *);
static inline enum Cvalue_type ipcp_cval_get_cvalue_type (struct ipcp_formal *);
static inline bool ipcp_cval_equal_cvalues (union info *const_val1,
					    union info *const_val2,
					    enum Cvalue_type type1,
					    enum Cvalue_type type2);

static void ipcp_init_stage (void);
static void ipcp_iterate_stage (void);
static bool ipcp_after_propagate (void);
static void ipcp_propagate_stage (void);
static void ipcp_insert_stage (void);
static void ipcp_update_callgraph (void);
static void ipcp_update_profiling (void);
static void ipcp_update_bb_counts (struct cgraph_node *, gcov_type);
static void ipcp_update_edges_counts (struct cgraph_node *, gcov_type);
static bool ipcp_redirect (struct cgraph_edge *);

static bool ipcp_method_dont_insert_const (struct cgraph_node *);
static bool ipcp_method_contains_asm (struct cgraph_node *);
static void ipcp_propagate_const (struct cgraph_node *, int, union info *,
				  enum Cvalue_type);
static void constant_val_insert (tree, tree, tree);

/* Debugging interface.  */
static void ipcp_structures_print (FILE *);
static void ipcp_method_cval_print (FILE *);
static void ipcp_callsite_param_print (FILE * f);
/* Profile prints.  */
static void ipcp_method_scale_print (FILE *);
static void ipcp_profile_print (FILE *);
static void ipcp_profile_mt_count_print (FILE *);
static void ipcp_profile_cs_count_print (FILE *);
static void ipcp_profile_bb_print (FILE *);
static void ipcp_profile_edge_print (FILE *);

/* ipcp_method interface.  */

/* Get scale for MT.  */
static inline gcov_type
ipcp_method_get_scale (struct cgraph_node *mt)
{
  return IPA_NODE_REF (mt)->count_scale;
}

/* Set Count as scale for MT.  */
static inline void
ipcp_method_set_scale (struct cgraph_node *node, gcov_type count)
{
  IPA_NODE_REF (node)->count_scale = count;
}

/* Returns true if node is a cloned/versioned node.  */
static inline bool
ipcp_method_is_cloned (struct cgraph_node *node)
{
  return (ipcp_method_orig_node (node) != NULL);
}

void
ipcp_cloned_create (struct cgraph_node *orig_node,
		    struct cgraph_node *new_node)
{
  ipa_node_create (new_node);
  ipcp_method_set_orig_node (new_node, orig_node);
  ipa_method_formal_compute_count (new_node);
  ipa_method_compute_tree_map (new_node);
}

/* Get orig node of method mt.  */
static inline struct cgraph_node *
ipcp_method_orig_node (struct cgraph_node *mt)
{
  return IPA_NODE_REF (mt)->ipcp_orig_node;
}

/* Sets orig node of method node.  */
static inline void
ipcp_method_set_orig_node (struct cgraph_node *node,
			   struct cgraph_node *orig_node)
{
  IPA_NODE_REF (node)->ipcp_orig_node = orig_node;
}

/* Create cval structure for method mt.  */
static inline void
ipcp_formal_create (struct cgraph_node *mt)
{
  IPA_NODE_REF (mt)->ipcp_cval =
    xcalloc (ipa_method_formal_count (mt), sizeof (struct ipcp_formal));
}

/* Set cval structure of i-th formal of mt to cval.  */
static inline void
ipcp_method_cval_set (struct cgraph_node *mt, int i, struct ipcp_formal *cval)
{
  IPA_NODE_REF (mt)->ipcp_cval[i].cvalue_type = cval->cvalue_type;
  ipcp_cval_set_cvalue (ipcp_method_cval (mt, i),
			ipcp_cval_get_cvalue (cval), cval->cvalue_type);
}

/* Get cval structure of i-th formal of mt.  */
static inline struct ipcp_formal *
ipcp_method_cval (struct cgraph_node *mt, int info_type)
{
  return &(IPA_NODE_REF (mt)->ipcp_cval[info_type]);
}

/* Set type of cval structure of formal i of mt to cval_type.  */
static inline void
ipcp_method_cval_set_cvalue_type (struct cgraph_node *mt, int i,
				  enum Cvalue_type cval_type)
{
  IPA_NODE_REF (mt)->ipcp_cval[i].cvalue_type = cval_type;
}

/* Computes Meet arithmetics:
   Meet(BOTTOM, x) = BOTTOM
   Meet(TOP,x) = x
   Meet(const_a,const_b) = BOTTOM, if const_a != const_b.  */
static void
ipcp_cval_meet (struct ipcp_formal *cval, struct ipcp_formal *cval1,
		struct ipcp_formal *cval2)
{
  if (ipcp_cval_get_cvalue_type (cval1) == BOTTOM
      || ipcp_cval_get_cvalue_type (cval2) == BOTTOM)
    {
      ipcp_cval_set_cvalue_type (cval, BOTTOM);
      return;
    }
  if (ipcp_cval_get_cvalue_type (cval1) == TOP)
    {
      ipcp_cval_set_cvalue_type (cval, ipcp_cval_get_cvalue_type (cval2));
      ipcp_cval_set_cvalue (cval, ipcp_cval_get_cvalue (cval2),
			    ipcp_cval_get_cvalue_type (cval2));
      return;
    }
  if (ipcp_cval_get_cvalue_type (cval2) == TOP)
    {
      ipcp_cval_set_cvalue_type (cval, ipcp_cval_get_cvalue_type (cval1));
      ipcp_cval_set_cvalue (cval, ipcp_cval_get_cvalue (cval1),
			    ipcp_cval_get_cvalue_type (cval1));
      return;
    }
  if (!ipcp_cval_equal_cvalues (ipcp_cval_get_cvalue (cval1),
				ipcp_cval_get_cvalue (cval2),
				ipcp_cval_get_cvalue_type (cval1),
				ipcp_cval_get_cvalue_type (cval2)))
    {
      ipcp_cval_set_cvalue_type (cval, BOTTOM);
      return;
    }
  ipcp_cval_set_cvalue_type (cval, ipcp_cval_get_cvalue_type (cval1));
  ipcp_cval_set_cvalue (cval, ipcp_cval_get_cvalue (cval1),
			ipcp_cval_get_cvalue_type (cval1));
}

/* Given the jump function, computes the  value of cval.  */
static void
ipcp_cval_compute (struct ipcp_formal *cval, struct cgraph_node *mt,
		   enum Jfunc_type type, union info *info_type)
{
  if (type == UNKNOWN_IPATYPE)
    ipcp_cval_set_cvalue_type (cval, BOTTOM);
  else if (type == CONST_IPATYPE_INT)
    {
      ipcp_cval_set_cvalue_type (cval, CONST_VALUE_INT);
      ipcp_cval_set_cvalue (cval, info_type, CONST_VALUE_INT);
    }
  else if (type == CONST_IPATYPE_FLOAT)
    {
      ipcp_cval_set_cvalue_type (cval, CONST_VALUE_FLOAT);
      ipcp_cval_set_cvalue (cval, info_type, CONST_VALUE_FLOAT);
    }
  else if (type == CONST_IPATYPE_INT_REF)
    {
      ipcp_cval_set_cvalue_type (cval, CONST_VALUE_INT_REF);
      ipcp_cval_set_cvalue (cval, info_type, CONST_VALUE_INT_REF);
    }
  else if (type == CONST_IPATYPE_FLOAT_REF)
    {
      ipcp_cval_set_cvalue_type (cval, CONST_VALUE_FLOAT_REF);
      ipcp_cval_set_cvalue (cval, info_type, CONST_VALUE_FLOAT_REF);
    }
  else if (type == FORMAL_IPATYPE)
    {
      enum Cvalue_type type =
	ipcp_cval_get_cvalue_type (ipcp_method_cval
				   (mt, info_type->formal_id));
      ipcp_cval_set_cvalue_type (cval, type);
      ipcp_cval_set_cvalue (cval,
			    ipcp_cval_get_cvalue (ipcp_method_cval
						  (mt, info_type->formal_id)),
			    type);
    }
}

/* True when cval1's and cval2's values are not the same.  */
static bool
ipcp_cval_changed (struct ipcp_formal *cval1, struct ipcp_formal *cval2)
{
  if (ipcp_cval_get_cvalue_type (cval1) == ipcp_cval_get_cvalue_type (cval2))
    {
      if (ipcp_cval_get_cvalue_type (cval1) != CONST_VALUE_INT &&
	  ipcp_cval_get_cvalue_type (cval1) != CONST_VALUE_FLOAT &&
	  ipcp_cval_get_cvalue_type (cval1) != CONST_VALUE_INT_REF &&
	  ipcp_cval_get_cvalue_type (cval1) != CONST_VALUE_FLOAT_REF)
	return false;
      if (ipcp_cval_equal_cvalues (ipcp_cval_get_cvalue (cval1),
				   ipcp_cval_get_cvalue (cval2),
				   ipcp_cval_get_cvalue_type (cval1),
				   ipcp_cval_get_cvalue_type (cval2)))
	return false;
    }
  return true;
}

/* Returns whether the type is a constant type.  */
static bool
ipcp_type_is_const (enum Cvalue_type type)
{
  if (type == CONST_VALUE_INT || type == CONST_VALUE_FLOAT
      || type == CONST_VALUE_INT_REF || type == CONST_VALUE_FLOAT_REF)
    return true;
  else
    return false;
}

/* Initializes the ipcp_cval array with TOP values.  */
static void
ipcp_method_cval_init (struct cgraph_node *mt)
{
  int i;
  tree parm_tree;

  ipcp_formal_create (mt);
  for (i = 0; i < ipa_method_formal_count (mt); i++)
    {
      parm_tree = ipa_method_get_tree (mt, i);
      if (INTEGRAL_TYPE_P (TREE_TYPE (parm_tree)) ||
	  SCALAR_FLOAT_TYPE_P (TREE_TYPE (parm_tree)) ||
	  POINTER_TYPE_P (TREE_TYPE (parm_tree)))
	ipcp_method_cval_set_cvalue_type (mt, i, TOP);
      else
	ipcp_method_cval_set_cvalue_type (mt, i, BOTTOM);
    }
}

/* Compute the proper scale for Node.  */
static void
ipcp_method_compute_scale (struct cgraph_node *node)
{
  gcov_type sum;
  struct cgraph_edge *cs;

  sum = 0;
  /* Compute sum of all counts of callers. */
  for (cs = node->callers; cs != NULL; cs = cs->next_caller)
    sum += cs->count;
  if (node->count == 0)
    ipcp_method_set_scale (node, 0);
  else
    ipcp_method_set_scale (node, sum * REG_BR_PROB_BASE / node->count);
}

/* cval interface.  */

/* Sets type to cval.  */
static inline void
ipcp_cval_set_cvalue_type (struct ipcp_formal *cval, enum Cvalue_type type)
{
  cval->cvalue_type = type;
}

/* Sets value to cval.  */
static inline void
ipcp_cval_set_cvalue (struct ipcp_formal *cval, union info *value,
		      enum Cvalue_type type)
{
  if (type == CONST_VALUE_INT || type == CONST_VALUE_INT_REF)
    {
      cval->cvalue.int_value.low = value->int_value.low;
      cval->cvalue.int_value.high = value->int_value.high;
    }
  else if (type == CONST_VALUE_FLOAT || type == CONST_VALUE_FLOAT_REF)
    cval->cvalue.float_value = value->float_value;
}

/* Returns value part of cval.  */
static inline union info *
ipcp_cval_get_cvalue (struct ipcp_formal *cval)
{
  return &(cval->cvalue);
}

/* Returns type part of cval.  */
static inline enum Cvalue_type
ipcp_cval_get_cvalue_type (struct ipcp_formal *cval)
{
  return cval->cvalue_type;
}

/* Returns true if const_val1 and const_val2 are equal.  */
static inline bool
ipcp_cval_equal_cvalues (union info *const_val1, union info *const_val2,
			 enum Cvalue_type type1, enum Cvalue_type type2)
{
  gcc_assert (ipcp_type_is_const (type1) && ipcp_type_is_const (type2));
  if (type1 != type2)
    return false;

  if (type1 == CONST_VALUE_INT || type1 == CONST_VALUE_INT_REF)
    {
      if ((const_val1->int_value.low == const_val2->int_value.low)
	  && (const_val1->int_value.high == const_val2->int_value.high))
	return true;
    }
  else
    if (REAL_VALUES_IDENTICAL
	(const_val1->float_value, const_val2->float_value))
    return true;

  return false;
}

/* Create a new assignment statment and make
   it the first statemant in the function FN
   parse tree.
   PARM1 is the lhs of the assignment and
   val is the rhs. */
static void
constant_val_insert (tree fn, tree parm1, tree val)
{
  struct function *func;
  tree init_stmt;
  edge e_step;
  edge_iterator ei;
  init_stmt = build2 (MODIFY_EXPR, void_type_node, parm1, val);
  func = DECL_STRUCT_FUNCTION (fn);
  cfun = func;
  if (ENTRY_BLOCK_PTR_FOR_FUNCTION (func)->succs)
    FOR_EACH_EDGE (e_step, ei, ENTRY_BLOCK_PTR_FOR_FUNCTION (func)->succs)
      bsi_insert_on_edge_immediate (e_step, init_stmt);
}

static tree
build_const_val (union info *cvalue, enum Cvalue_type type, tree tree_type)
{
  tree const_val;

  if (type == CONST_VALUE_INT || type == CONST_VALUE_INT_REF)
    const_val = build_int_cst_wide (tree_type, cvalue->int_value.low,
				    cvalue->int_value.high);
  else if (type == CONST_VALUE_FLOAT || type == CONST_VALUE_FLOAT_REF)
    const_val = build_real (tree_type, cvalue->float_value);
  else
    abort ();
  return const_val;
}


/* Build the constant tree and call constant_val_insert().  */
static void
ipcp_propagate_const (struct cgraph_node *mt, int param, union info *cvalue,
		      enum Cvalue_type type)
{
  tree fndecl;
  tree const_val;
  tree parm_tree;

  if (dump_file)
    fprintf (dump_file, "propagating const to %s\n", cgraph_node_name (mt));
  fndecl = mt->decl;
  parm_tree = ipa_method_get_tree (mt, param);
  const_val = build_const_val (cvalue, type, TREE_TYPE (parm_tree));
  if (parm_tree != NULL)
    if (fndecl != NULL)
      constant_val_insert (fndecl, parm_tree, const_val);
}

/* Initialization and computation of IPCP data structures. 
   It is an intraprocedural
   analysis of methods,which gathers information to be propagated
   later on.  */
static void
ipcp_init_stage (void)
{
  struct cgraph_node *node;
  struct cgraph_edge *cs;

  for (node = cgraph_nodes; node; node = node->next)
    {
      ipa_method_formal_compute_count (node);
      ipa_method_compute_tree_map (node);
      ipcp_method_cval_init (node);
      ipa_method_compute_modify (node);
      ipcp_method_compute_scale (node);
    }
  for (node = cgraph_nodes; node; node = node->next)
    {
      /* building jump functions  */
      for (cs = node->callees; cs; cs = cs->next_callee)
	{
	  ipa_callsite_compute_count (cs);
	  if (ipa_callsite_param_count (cs)
	      != ipa_method_formal_count (cs->callee))
	    {
	      /* Handles the cases of functions with 
	         a variable number of parameters.  */
	      ipa_callsite_param_count_set (cs, 0);
	      ipa_method_formal_count_set (cs->callee, 0);
	    }
	  else
	    ipa_callsite_compute_param (cs);
	}
    }
}

/* Calls the constant propagation algorithm and recalls it if necessary
   (if there are undetermined values left).  */
static void
ipcp_iterate_stage (void)
{
  ipcp_propagate_stage ();
  if (ipcp_after_propagate ())
    ipcp_propagate_stage ();
}

/* Returns true if there are some parameters whose value is TOP.
   Changes their values to BOTTOM, since they weren't determined.  */
static bool
ipcp_after_propagate (void)
{
  int i, count;
  struct cgraph_node *node;
  bool prop_again;

  prop_again = false;
  for (node = cgraph_nodes; node; node = node->next)
    {
      count = ipa_method_formal_count (node);
      for (i = 0; i < count; i++)
	if (ipcp_cval_get_cvalue_type (ipcp_method_cval (node, i)) == TOP)
	  {
	    prop_again = true;
	    ipcp_method_cval_set_cvalue_type (node, i, BOTTOM);
	  }
    }
  return prop_again;
}

/* Interprocedural analysis. The algorithm propagates constants from
   the caller's parameters to the callee's arguments.  */
static void
ipcp_propagate_stage (void)
{
  int i;
  struct ipcp_formal cval1 = { 0, {{0, 0}} }, cval ={ 0, {{0, 0}} };
  struct ipcp_formal *cval2;
  struct cgraph_node *mt, *callee;
  struct cgraph_edge *cs;
  struct ipa_jump_func *jump_func;
  enum Jfunc_type type;
  union info *info_type;
  ipa_methodlist_p wl;
  int count;

  wl = ipa_methodlist_init ();
  while (ipa_methodlist_not_empty (wl))
    {
      mt = ipa_remove_method (&wl);
      for (cs = mt->callees; cs; cs = cs->next_callee)
	{
	  callee = ipa_callsite_callee (cs);
	  count = ipa_callsite_param_count (cs);
	  for (i = 0; i < count; i++)
	    {
	      jump_func = ipa_callsite_param (cs, i);
	      type = get_type (jump_func);
	      info_type = ipa_jf_get_info_type (jump_func);
	      ipcp_cval_compute (&cval1, mt, type, info_type);
	      cval2 = ipcp_method_cval (callee, i);
	      ipcp_cval_meet (&cval, &cval1, cval2);
	      if (ipcp_cval_changed (&cval, cval2))
		{
		  ipcp_method_cval_set (callee, i, &cval);
		  ipa_add_method (&wl, callee);
		}
	    }
	}
    }
}

/* Builds and initializes ipa_replace_map struct 
   according to type. This struct is read by versioning, which
   operates according to the flags sent.  */
static struct ipa_replace_map *
ipcp_replace_map_create (enum Cvalue_type type, tree parm_tree,
			 union info *cvalue)
{
  struct ipa_replace_map *replace_map;
  tree const_val;

  replace_map = xcalloc (1, sizeof (struct ipa_replace_map));
  gcc_assert (ipcp_type_is_const (type));
  if (type == CONST_VALUE_INT_REF || type == CONST_VALUE_FLOAT_REF)
    {
      const_val =
	build_const_val (cvalue, type, TREE_TYPE (TREE_TYPE (parm_tree)));
      replace_map->old_tree = parm_tree;
      replace_map->new_tree = const_val;
      replace_map->replace_p = true;
      replace_map->ref_p = true;
    }
  else if (TREE_READONLY (parm_tree) && !TREE_ADDRESSABLE (parm_tree))
    {
      const_val = build_const_val (cvalue, type, TREE_TYPE (parm_tree));
      replace_map->old_tree = parm_tree;
      replace_map->new_tree = const_val;
      replace_map->replace_p = true;
      replace_map->ref_p = false;
    }
  else
    {
      replace_map->old_tree = NULL;
      replace_map->new_tree = NULL;
      replace_map->replace_p = false;
      replace_map->ref_p = false;
    }

  return replace_map;
}

/* Propagates the constant parameters found by ipcp_iterate_stage()
   to the function's code.  */
static void
ipcp_insert_stage (void)
{
  struct cgraph_node *node, *node1 = NULL;
  int i, const_param;
  union info *cvalue;
  varray_type redirect_callers, replace_trees;
  struct cgraph_edge *cs;
  int node_callers, count;
  tree parm_tree;
  enum Cvalue_type type;
  struct ipa_replace_map *replace_param;

  for (node = cgraph_nodes; node; node = node->next)
    {
      /* Propagation of the constant is forbidden in 
         certain conditions.  */
      if (ipcp_method_dont_insert_const (node))
	continue;
      const_param = 0;
      count = ipa_method_formal_count (node);
      for (i = 0; i < count; i++)
	{
	  type = ipcp_cval_get_cvalue_type (ipcp_method_cval (node, i));
	  if (ipcp_type_is_const (type))
	    const_param++;
	}
      if (const_param == 0)
	continue;
      VARRAY_GENERIC_PTR_INIT (replace_trees, const_param, "replace_trees");
      for (i = 0; i < count; i++)
	{
	  type = ipcp_cval_get_cvalue_type (ipcp_method_cval (node, i));
	  if (ipcp_type_is_const (type))
	    {
	      cvalue = ipcp_cval_get_cvalue (ipcp_method_cval (node, i));
	      parm_tree = ipa_method_get_tree (node, i);
	      replace_param =
		ipcp_replace_map_create (type, parm_tree, cvalue);
	      VARRAY_PUSH_GENERIC_PTR (replace_trees, replace_param);
	    }
	}
      /* Compute how many callers node has.  */
      node_callers = 0;
      for (cs = node->callers; cs != NULL; cs = cs->next_caller)
	node_callers++;
      VARRAY_GENERIC_PTR_INIT (redirect_callers, node_callers,
			       "redirect_callers");
      for (cs = node->callers; cs != NULL; cs = cs->next_caller)
	VARRAY_PUSH_GENERIC_PTR (redirect_callers, cs);
      /* Redirecting all the callers of the node to the 
         new versioned node.  */
      node1 =
	cgraph_function_versioning (node, redirect_callers, replace_trees);
      VARRAY_CLEAR (redirect_callers);
      VARRAY_CLEAR (replace_trees);
      if (node1 == NULL)
	continue;
      if (dump_file)
	fprintf (dump_file, "versioned function %s\n",
		 cgraph_node_name (node));
      ipcp_cloned_create (node, node1);
      for (i = 0; i < count; i++)
	{
	  type = ipcp_cval_get_cvalue_type (ipcp_method_cval (node, i));
	  if (ipcp_type_is_const (type))
	    {
	      cvalue = ipcp_cval_get_cvalue (ipcp_method_cval (node, i));
	      parm_tree = ipa_method_get_tree (node, i);
	      if (type != CONST_VALUE_INT_REF && type != CONST_VALUE_FLOAT_REF
		  && !TREE_READONLY (parm_tree))
		ipcp_propagate_const (node1, i, cvalue, type);
	    }
	}
    }
  ipcp_update_callgraph ();
  ipcp_update_profiling ();
}

/* The IPCP driver.  */
void
ipcp_driver (void)
{
  if (dump_file)
    fprintf (dump_file, "\nIPA constant propagation start:\n");
  ipa_nodes_create ();
  ipa_edges_create ();
  /* 1. Call the init stage to initialize 
     the ipa_node and ipa_edge structures.  */
  ipcp_init_stage ();
  if (dump_file)
    {
      fprintf (dump_file, "\nIPA structures before propagation:\n");
      ipcp_structures_print (dump_file);
    }
  /* 2. Do the interprocedural propagation.  */
  ipcp_iterate_stage ();
  if (dump_file)
    {
      fprintf (dump_file, "\nIPA structures after propagation:\n");
      ipcp_structures_print (dump_file);
      fprintf (dump_file, "\nProfiling info before insert stage:\n");
      ipcp_profile_print (dump_file);
    }
  /* 3. Insert the constants found to the functions.  */
  ipcp_insert_stage ();
  if (dump_file)
    {
      fprintf (dump_file, "\nProfiling info after insert stage:\n");
      ipcp_profile_print (dump_file);
    }
  /* Free all IPCP structures.  */
  ipa_free ();
  ipa_nodes_free ();
  ipa_edges_free ();
  if (dump_file)
    fprintf (dump_file, "\nIPA constant propagation end\n");
  cgraph_remove_unreachable_nodes (true, NULL);
}

/* Check conditions to forbid constant insertion to the function.  */
static bool
ipcp_method_dont_insert_const (struct cgraph_node *mt)
{
  /* ??? Handle pending sizes case.  */
  if (DECL_UNINLINABLE (mt->decl))
    return true;
  /* Prevent propagation in the case of ASM calls.  */
  if (ipcp_method_contains_asm (mt))
    return true;
  return false;
}

/*  Finds if there are any asm expr in the function.  */
static bool
ipcp_method_contains_asm (struct cgraph_node *mt)
{
  tree decl;
  tree body;
  block_stmt_iterator bsi;
  tree stmt;
  struct function *func;
  basic_block bb;

  decl = mt->decl;
  body = DECL_SAVED_TREE (decl);
  if (body != NULL)
    {
      func = DECL_STRUCT_FUNCTION (decl);
      FOR_EACH_BB_FN (bb, func)
      {
	for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	  {
	    stmt = bsi_stmt (bsi);
	    if (TREE_CODE (stmt) == ASM_EXPR)
	      return true;
	  }
      }
    }
  return false;
}

/* Debugging interface.  */

/* Prints all IPCP data structures.  */
static void
ipcp_structures_print (FILE * f)
{
  ipcp_method_cval_print (f);
  ipcp_method_scale_print (f);
  ipa_method_tree_print (f);
  ipa_method_modify_print (f);
  ipcp_callsite_param_print (f);
}

/* Print profile info for all methods.  */
static void
ipcp_profile_print (FILE * f)
{
  fprintf (f, "\nNODE COUNTS :\n");
  ipcp_profile_mt_count_print (f);
  fprintf (f, "\nCS COUNTS stage:\n");
  ipcp_profile_cs_count_print (f);
  fprintf (f, "\nBB COUNTS and FREQUENCIES :\n");
  ipcp_profile_bb_print (f);
  fprintf (f, "\nCFG EDGES COUNTS and PROBABILITIES :\n");
  ipcp_profile_edge_print (f);
}

/* Prints count scale data structures.  */
static void
ipcp_method_scale_print (FILE * f)
{
  struct cgraph_node *node;

  for (node = cgraph_nodes; node; node = node->next)
    {
      fprintf (f, "printing scale for %s: ", cgraph_node_name (node));
      fprintf (f, "value is  " HOST_WIDE_INT_PRINT_DEC
	       "  \n", (HOST_WIDE_INT) ipcp_method_get_scale (node));
    }
}

/* Print counts of all cgraph nodes.  */
static void
ipcp_profile_mt_count_print (FILE * f)
{
  struct cgraph_node *node;

  for (node = cgraph_nodes; node; node = node->next)
    {
      fprintf (f, "method %s: ", cgraph_node_name (node));
      fprintf (f, "count is  " HOST_WIDE_INT_PRINT_DEC
	       "  \n", (HOST_WIDE_INT) node->count);
    }
}

/* Print counts of all cgraph edgess.  */
static void
ipcp_profile_cs_count_print (FILE * f)
{
  struct cgraph_node *node;
  struct cgraph_edge *cs;

  for (node = cgraph_nodes; node; node = node->next)
    {
      for (cs = node->callees; cs; cs = cs->next_callee)
	{
	  fprintf (f, "%s -> %s ", cgraph_node_name (cs->caller),
		   cgraph_node_name (cs->callee));
	  fprintf (f, "count is  " HOST_WIDE_INT_PRINT_DEC "  \n",
		   (HOST_WIDE_INT) cs->count);
	}
    }
}

/* Print all counts and probabilities of cfg edges of all methods.  */
static void
ipcp_profile_edge_print (FILE * f)
{
  struct cgraph_node *node;
  basic_block bb;
  edge_iterator ei;
  edge e;

  for (node = cgraph_nodes; node; node = node->next)
    {
      fprintf (f, "method %s: \n", cgraph_node_name (node));
      if (DECL_SAVED_TREE (node->decl))
	{
	  bb =
	    ENTRY_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (node->decl));
	  fprintf (f, "ENTRY: ");
	  fprintf (f, " " HOST_WIDE_INT_PRINT_DEC
		   " %d\n", (HOST_WIDE_INT) bb->count, bb->frequency);

	  if (bb->succs)
	    FOR_EACH_EDGE (e, ei, bb->succs)
	    {
	      if (e->dest ==
		  EXIT_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION
					       (node->decl)))
		fprintf (f, "edge ENTRY -> EXIT,  Count");
	      else
		fprintf (f, "edge ENTRY -> %d,  Count", e->dest->index);
	      fprintf (f, " " HOST_WIDE_INT_PRINT_DEC
		       " Prob %d\n", (HOST_WIDE_INT) e->count,
		       e->probability);
	    }
	  FOR_EACH_BB_FN (bb, DECL_STRUCT_FUNCTION (node->decl))
	  {
	    fprintf (f, "bb[%d]: ", bb->index);
	    fprintf (f, " " HOST_WIDE_INT_PRINT_DEC
		     " %d\n", (HOST_WIDE_INT) bb->count, bb->frequency);
	    FOR_EACH_EDGE (e, ei, bb->succs)
	    {
	      if (e->dest ==
		  EXIT_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION
					       (node->decl)))
		fprintf (f, "edge %d -> EXIT,  Count", e->src->index);
	      else
		fprintf (f, "edge %d -> %d,  Count", e->src->index,
			 e->dest->index);
	      fprintf (f, " " HOST_WIDE_INT_PRINT_DEC " Prob %d\n",
		       (HOST_WIDE_INT) e->count, e->probability);
	    }
	  }
	}
    }
}

/* Print counts and frequencies for all basic blocks of all methods.  */
static void
ipcp_profile_bb_print (FILE * f)
{
  basic_block bb;
  struct cgraph_node *node;

  for (node = cgraph_nodes; node; node = node->next)
    {
      fprintf (f, "method %s: \n", cgraph_node_name (node));
      if (DECL_SAVED_TREE (node->decl))
	{
	  bb =
	    ENTRY_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (node->decl));
	  fprintf (f, "ENTRY: Count");
	  fprintf (f, " " HOST_WIDE_INT_PRINT_DEC
		   " Frquency  %d\n", (HOST_WIDE_INT) bb->count,
		   bb->frequency);

	  FOR_EACH_BB_FN (bb, DECL_STRUCT_FUNCTION (node->decl))
	  {
	    fprintf (f, "bb[%d]: Count", bb->index);
	    fprintf (f, " " HOST_WIDE_INT_PRINT_DEC
		     " Frequency %d\n", (HOST_WIDE_INT) bb->count,
		     bb->frequency);
	  }
	  bb =
	    EXIT_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (node->decl));
	  fprintf (f, "EXIT: Count");
	  fprintf (f, " " HOST_WIDE_INT_PRINT_DEC
		   " Frequency %d\n", (HOST_WIDE_INT) bb->count,
		   bb->frequency);

	}
    }
}

/* Prints ipcp_cval data structures.  */
static void
ipcp_method_cval_print (FILE * f)
{
  struct cgraph_node *node;
  int i, count;
  ipcp_int *cvalue;

  fprintf (f, "\nCVAL PRINT\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      fprintf (f, "printing cvals %s:\n", cgraph_node_name (node));
      count = ipa_method_formal_count (node);
      for (i = 0; i < count; i++)
	{
	  if (ipcp_cval_get_cvalue_type (ipcp_method_cval (node, i))
	      == CONST_VALUE_INT
	      || ipcp_cval_get_cvalue_type (ipcp_method_cval (node, i)) ==
	      CONST_VALUE_INT_REF)
	    {
	      fprintf (f, " param [%d]: ", i);
	      fprintf (f, "type is CONST_INT ");
	      cvalue =
		&(ipcp_cval_get_cvalue (ipcp_method_cval (node, i))->
		  int_value);
	      fprintf (f,
		       "value is  " HOST_WIDE_INT_PRINT_DEC " "
		       HOST_WIDE_INT_PRINT_DEC " \n",
		       (HOST_WIDE_INT) cvalue->high,
		       (HOST_WIDE_INT) cvalue->low);
	    }
	  else if (ipcp_cval_get_cvalue_type (ipcp_method_cval (node, i))
		   == CONST_VALUE_FLOAT
		   || ipcp_cval_get_cvalue_type (ipcp_method_cval (node, i))
		   == CONST_VALUE_FLOAT_REF)
	    {
	      char string[100];
	      REAL_VALUE_TYPE d;
	      fprintf (f, " param [%d]: ", i);
	      fprintf (f, "type is CONST_FLOAT ");
	      d =
		(ipcp_cval_get_cvalue (ipcp_method_cval (node, i)))->
		float_value;
	      real_to_decimal (string, &d, sizeof (string), 0, 1);
	      fprintf (f, "value is %s\n", string);
	    }
	  else if (ipcp_method_cval (node, i)->cvalue_type == TOP)
	    fprintf (f, "param [%d]: type is TOP  \n", i);
	  else
	    fprintf (f, "param [%d]: type is BOTTOM  \n", i);
	}
    }
}

/* Prints ipa_jump_func data structures.  */
static void
ipcp_callsite_param_print (FILE * f)
{
  struct cgraph_node *node;
  int i, count;
  struct cgraph_edge *cs;
  struct ipa_jump_func *jump_func;
  enum Jfunc_type type;
  ipcp_int *info_type_int;
  REAL_VALUE_TYPE info_type_float;

  fprintf (f, "\nCALLSITE PARAM PRINT\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      for (cs = node->callees; cs; cs = cs->next_callee)
	{
	  fprintf (f, "callsite  %s ", cgraph_node_name (node));
	  fprintf (f, "-> %s :: \n", cgraph_node_name (cs->callee));
	  count = ipa_callsite_param_count (cs);
	  for (i = 0; i < count; i++)
	    {
	      jump_func = ipa_callsite_param (cs, i);
	      type = get_type (jump_func);

	      fprintf (f, " param %d: ", i);
	      if (type == UNKNOWN_IPATYPE)
		fprintf (f, "UNKNOWN\n");
	      else if (type == CONST_IPATYPE_INT)
		{
		  info_type_int =
		    &((ipa_jf_get_info_type (jump_func))->int_value);
		  fprintf (f, "CONST INT : ");
		  fprintf (f, " " HOST_WIDE_INT_PRINT_DEC
			   " " HOST_WIDE_INT_PRINT_DEC " \n",
			   (HOST_WIDE_INT) info_type_int->high,
			   (HOST_WIDE_INT) info_type_int->low);
		}
	      else if (type == CONST_IPATYPE_FLOAT)
		{
		  char string[100];
		  info_type_float =
		    (ipa_jf_get_info_type (jump_func))->float_value;
		  fprintf (f, "CONST FLOAT ");
		  real_to_decimal (string, &info_type_float, sizeof (string),
				   0, 1);
		  fprintf (f, " %s\n", string);
		}
	      else if (type == FORMAL_IPATYPE)
		{
		  fprintf (f, "FORMAL : ");
		  fprintf (f, "%d\n",
			   ipa_jf_get_info_type (jump_func)->formal_id);
		}
	    }
	}
    }
}

/* Fix the callsites and the callgraph after function cloning was done.  */
static void
ipcp_update_callgraph (void)
{
  struct cgraph_node *node, *orig_callee;
  struct cgraph_edge *cs;

  for (node = cgraph_nodes; node; node = node->next)
    {
      /* want to fix only original nodes  */
      if (ipcp_method_is_cloned (node))
	continue;
      for (cs = node->callees; cs; cs = cs->next_callee)
	if (ipcp_method_is_cloned (cs->callee))
	  {
	    /* Callee is a cloned node  */
	    orig_callee = ipcp_method_orig_node (cs->callee);
	    if (ipcp_redirect (cs))
	      {
		cgraph_redirect_edge_callee (cs, orig_callee);
		TREE_OPERAND (TREE_OPERAND (get_call_expr_in (cs->call_stmt), 0), 0) =
		  orig_callee->decl;
	      }
	  }
    }
}

/* Retruns true if this callsite should be redirected to
   the orig callee (instead of the cloned one).  */
static bool
ipcp_redirect (struct cgraph_edge *cs)
{
  struct cgraph_node *caller, *callee, *orig_callee;
  int i, count;
  struct ipa_jump_func *jump_func;
  enum Jfunc_type type;
  enum Cvalue_type cval_type;

  caller = cs->caller;
  callee = cs->callee;
  orig_callee = ipcp_method_orig_node (callee);
  count = ipa_method_formal_count (orig_callee);
  for (i = 0; i < count; i++)
    {
      cval_type =
	ipcp_cval_get_cvalue_type (ipcp_method_cval (orig_callee, i));
      if (ipcp_type_is_const (cval_type))
	{
	  jump_func = ipa_callsite_param (cs, i);
	  type = get_type (jump_func);
	  if (type != CONST_IPATYPE_INT && type != CONST_IPATYPE_FLOAT
	      && type != CONST_IPATYPE_INT_REF
	      && type != CONST_IPATYPE_FLOAT_REF)
	    return true;
	}
    }

  return false;
}

/* Update profiling info for versioned methods and the
   methods they were versioned from.  */
static void
ipcp_update_profiling (void)
{
  struct cgraph_node *node, *orig_node;
  gcov_type scale, scale_complement;
  struct cgraph_edge *cs;

  for (node = cgraph_nodes; node; node = node->next)
    {
      if (ipcp_method_is_cloned (node))
	{
	  orig_node = ipcp_method_orig_node (node);
	  scale = ipcp_method_get_scale (orig_node);
	  node->count = orig_node->count * scale / REG_BR_PROB_BASE;
	  scale_complement = REG_BR_PROB_BASE - scale;
	  orig_node->count =
	    orig_node->count * scale_complement / REG_BR_PROB_BASE;
	  for (cs = node->callees; cs; cs = cs->next_callee)
	    cs->count = cs->count * scale / REG_BR_PROB_BASE;
	  for (cs = orig_node->callees; cs; cs = cs->next_callee)
	    cs->count = cs->count * scale_complement / REG_BR_PROB_BASE;
	  ipcp_update_bb_counts (node, scale);
	  ipcp_update_bb_counts (orig_node, scale_complement);
	  ipcp_update_edges_counts (node, scale);
	  ipcp_update_edges_counts (orig_node, scale_complement);
	}
    }
}

/* Update all cfg basic blocks in NODE according to SCALE.  */
static void
ipcp_update_bb_counts (struct cgraph_node *node, gcov_type scale)
{
  basic_block bb;

  FOR_ALL_BB_FN (bb, DECL_STRUCT_FUNCTION (node->decl))
    bb->count = bb->count * scale / REG_BR_PROB_BASE;
}

/* Update all cfg edges in NODE according to SCALE.  */
static void
ipcp_update_edges_counts (struct cgraph_node *node, gcov_type scale)
{
  basic_block bb;
  edge_iterator ei;
  edge e;

  FOR_ALL_BB_FN (bb, DECL_STRUCT_FUNCTION (node->decl))
    FOR_EACH_EDGE (e, ei, bb->succs)
    e->count = e->count * scale / REG_BR_PROB_BASE;
}

static bool
cgraph_gate_cp (void)
{
  return flag_ipa_cp;
}

struct tree_opt_pass pass_ipa_cp = {
  "cp",				/* name */
  cgraph_gate_cp,		/* gate */
  NULL,				/* function analysis */
  NULL,				/* variable analysis */
  ipcp_driver,			/* execute */
  NULL,				/* function modification */
  NULL,				/* variable modification */
  NULL,				/* sub */
  NULL,				/* next */
  0,				/* static_pass_number */
  TV_IPA_CONSTANT_PROP,		/* tv_id */
  0,				/* properties_required */
  PROP_trees,			/* properties_provided */
  0,				/* properties_destroyed */
  0,				/* todo_flags_start */
  TODO_dump_cgraph | TODO_dump_func,	/* todo_flags_finish */
  0				/* letter */
};
