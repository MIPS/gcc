/* Callgraph based intraprocedural optimizations.
   Copyright (C) 2003, 2004 Free Software Foundation, Inc.
   Contributed by Razya Ladelsky.

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
The aim of IPA constant propagation is to find which function's argument
has the same constant value in each invocation throughout the whole program.
For example, for an application consisting of two files, foo1.c, foo2.c:

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

The IPCP algorithm should find that g's formal argument y
is always called with the value 3. 

The optimization is divided into three stages:

First stage - intraprocedural analysis  
=======================================
This phase computes jump_function and modify information. 

Jump functions represent the value that is passes to each actual argument 
by this callsite.
there are three values :
  Formal - the caller's formal parameter is passed as a parameter.
  Constant - a constant is passed as a parameter.
  Unknown - neither of the above.

Modify is true for a formal parameter if it is modified in its function.

In the case that the formal parameter passed in the callsite is 
modified, the jump fumction value for this parameter turns to UNKOWN.

The jump function structure, ipcp_jump_func, is defined in ipa_edge
structure (defined in ipa_prop.h and pointed to by cgraph_node->aux)
The modify info, ipcp_modify, is defined in ipa_node structure
(defined in ipa_prop.h and pointed to by cgraph_edge->aux).

The ipcp_tree_map (a mapping from the PARAM_DECL tree to the inner
representation of formals) and ipcp_formal (explained below) structures
are also pointed to by cgraph_node->aux and also initialized 
in this phase.

-ipcp_stage1() is the first stage driver.

Second stage - interprocedural analysis
========================================
This phase solves the interprocedural constant propagation problem.
The algorithm is based on a paper by Challahan David, 
Keith D Cooper, Ken Kennedy, Linda Torczon, 
Interprocedural Constant Propagation, Comp86.

Each formal has cval info (denoted as ipcp_formal) which represents its 
value computed so far. 
possible values : 
  TOP - not known yet. 
  BOTTOM - considered as not constant. 
  CONSTANT_TYPE - has constant value.

The algoritm computes for all formal parameters in the program 
their cval value.
Cval of formal f will have a constant value if all callsites to this 
function have the same constant value passed to f 
and will have BOTTOM value otherwise.

This stage propagates the constant information from formal paranmeters
to the actual parameters.

-ipcp_stage2() is the second stage driver.

Third phase 
============
Propagates the constant-valued formals into the function.

-ipcp_stage3() is the third phase driver.
   
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "langhooks.h"
#include "hashtab.h"
#include "toplev.h"
#include "flags.h"
#include "ggc.h"
#include "debug.h"
#include "target.h"  
#include "cgraph.h"
#include "varray.h"
#include "output.h"  
#include "tree-iterator.h"
#include "tree-gimple.h"
#include "tree-inline.h"
#include "ipa_prop.h"

typedef struct cgraph_node *ipcp_method ;
typedef  struct cgraph_edge *ipcp_callsite;

struct ipcp_methodlist
{
  ipcp_method method_p;
  struct ipcp_methodlist *next_method;
};

typedef struct ipcp_methodlist *ipcp_methodlist_p;

/* WorlList Interface.  */
static inline ipcp_methodlist_p ipcp_create_methodlist_node (void); 
static inline bool ipcp_methodlist_not_empty (ipcp_methodlist_p);
static inline ipcp_method ipcp_methodlist_method (ipcp_methodlist_p); 
static inline void ipcp_methodlist_method_set (ipcp_methodlist_p, ipcp_method);
static inline ipcp_methodlist_p ipcp_methodlist_next_method (ipcp_methodlist_p); 
static inline void ipcp_methodlist_next_method_set (ipcp_methodlist_p, 
						    ipcp_methodlist_p);
static void ipcp_add_method (ipcp_methodlist_p *, ipcp_method);
static ipcp_method ipcp_remove_method (ipcp_methodlist_p *);
static ipcp_methodlist_p ipcp_methodlist_init (void);

/* ipcp_callsite interface.  */
static inline int ipcp_callsite_param_count (ipcp_callsite); 
static inline void ipcp_callsite_param_count_set (ipcp_callsite, int); 
static inline struct ipcp_jump_func *ipcp_callsite_param (ipcp_callsite, int);
static inline ipcp_method ipcp_callsite_callee (ipcp_callsite);
static inline void ipcp_callsite_param_set_type (ipcp_callsite, int, 
						 enum Jfunc_type);
static inline void ipcp_callsite_param_set_info_type (ipcp_callsite, int, 
						      ipcp_int *); 
static inline void ipcp_callsite_param_map_create (ipcp_callsite);
static inline tree ipcp_callsite_tree (ipcp_callsite);
static inline ipcp_method ipcp_callsite_caller (ipcp_callsite);
static void ipcp_callsite_compute_param (ipcp_callsite);
static void ipcp_callsite_compute_count (ipcp_callsite);

/* ipcp_method interface.  */
static inline void ipcp_formal_create (ipcp_method); 
static inline int ipcp_method_formal_count (ipcp_method);
static inline void ipcp_method_formal_count_set (ipcp_method, int); 
static inline void ipcp_method_cval_set (ipcp_method, int, struct ipcp_formal*);             
static inline struct ipcp_formal *ipcp_method_cval (ipcp_method, int); 
static inline void ipcp_method_cval_set_cvalue_type (ipcp_method, int, 
						     enum Cvalue_type); 
static inline bool ipcp_method_is_modified (ipcp_method, int);
static inline tree ipcp_method_get_tree (ipcp_method, int);
static inline void ipcp_method_tree_map_create (ipcp_method);
static inline void ipcp_method_modify_create (ipcp_method); 
static inline void ipcp_method_modify_set (ipcp_method, int, bool);
static inline ipcp_callsite ipcp_cs_get_first (ipcp_method);
static inline ipcp_callsite ipcp_cs_get_next (ipcp_callsite);
static inline bool ipcp_cs_not_last (ipcp_callsite); 
static int  ipcp_method_tree_map (ipcp_method, tree);
static void ipcp_method_compute_tree_map (ipcp_method);
static void ipcp_cval_meet (struct ipcp_formal *, struct ipcp_formal *,
			    struct ipcp_formal *);
static void ipcp_cval_compute (struct ipcp_formal *, ipcp_method,
			       enum Jfunc_type, ipcp_int *);
static bool ipcp_cval_changed (struct ipcp_formal *, struct ipcp_formal *);
static void ipcp_method_formal_compute_count (ipcp_method);
static void ipcp_method_cval_init (ipcp_method);
static tree ipcp_modify_walk_tree (tree *, void *);
static void ipcp_method_modify_init (ipcp_method);
static void ipcp_method_compute_modify (ipcp_method);

/* jump function interface.  */
static inline enum Jfunc_type get_type (struct ipcp_jump_func *);
static inline ipcp_int *ipcp_jf_get_info_type (struct ipcp_jump_func *);
static inline void ipcp_jf_set_info_type (struct ipcp_jump_func *, ipcp_int *);

static inline void ipcp_int2ipcp_int (ipcp_int *, int);
static inline int ipcp_get_int (ipcp_int *);

/* cval interface.  */
static inline void ipcp_cval_set_cvalue_type (struct ipcp_formal *, 
					      enum Cvalue_type);
static inline void ipcp_cval_set_cvalue (struct ipcp_formal *, ipcp_int *); 
static inline ipcp_int *ipcp_cval_get_cvalue (struct ipcp_formal *);
static inline enum Cvalue_type ipcp_cval_get_cvalue_type (struct ipcp_formal *);
static inline bool ipcp_cval_equal_cvalues (ipcp_int *, ipcp_int *);

static void ipcp_stage1 (void);
static void ipcp_stage2 (void);
static void ipcp_stage3 (void);
static void ipcp_free (void);
static void ipa_nodes_create (void);

static bool ipcp_method_dont_propagate (ipcp_method);
static tree ipcp_asm_walk_tree (tree *, int *, void *);
static bool ipcp_method_contains_asm (ipcp_method);
static void ipcp_walk_statements (ipcp_method, tree *);
static void ipcp_propagate_const (ipcp_method, int, ipcp_int *);
static void constant_val_insert (tree, tree, tree);

/* Debugging interface.  */
static void ipcp_structures_print (FILE *);
static void ipcp_method_cval_print (FILE *);
static void ipcp_method_tree_print (FILE *); 
static void ipcp_method_modify_print (FILE *);
static void ipcp_callsite_param_print (FILE *f);

/* WorlList Interface.  */

static inline ipcp_methodlist_p
ipcp_create_methodlist_node (void) 
{
  return ggc_alloc (sizeof (struct ipcp_methodlist));
}

static inline bool
ipcp_methodlist_not_empty (ipcp_methodlist_p wl)
{
  return (wl != NULL);
}

static inline ipcp_method
ipcp_methodlist_method (ipcp_methodlist_p wl) 
{
  return wl->method_p;   
}

static inline void
ipcp_methodlist_method_set (ipcp_methodlist_p wl, ipcp_method mt)
{
  wl->method_p = mt;
}

static inline ipcp_methodlist_p
ipcp_methodlist_next_method (ipcp_methodlist_p wl) 
{ 
  return wl->next_method;    
}

static inline void
ipcp_methodlist_next_method_set (ipcp_methodlist_p wl1, ipcp_methodlist_p wl2) 
{ 
  wl1->next_method = wl2;    
}

/* Initializing a worklist to contain all methods.  */
static ipcp_methodlist_p 
ipcp_methodlist_init(void)
{
  ipcp_method node;
  ipcp_methodlist_p wl;

  wl = NULL;
  for (node = cgraph_nodes; node; node = node->next)
    {
      ipcp_add_method (&wl, node);
    }
  return wl;
}

/* Adding a method to the worklist.  */
static void 
ipcp_add_method (ipcp_methodlist_p *wl, ipcp_method mt)
{
  ipcp_methodlist_p temp;

  temp = ipcp_create_methodlist_node ();
  ipcp_methodlist_method_set (temp, mt);
  ipcp_methodlist_next_method_set (temp, *wl);
  *wl = temp;
}

/* Removing a method from the worklist.  */
static ipcp_method 
ipcp_remove_method (ipcp_methodlist_p *wl)
{
  ipcp_methodlist_p first;

  first = *wl;
  *wl = ipcp_methodlist_next_method (*wl);
  return ipcp_methodlist_method (first);
}

/* callsite interface.  */
static inline int 
ipcp_callsite_param_count (ipcp_callsite cs) 
{ 
  return ((struct ipa_edge *)cs->aux)->ipcp_param_num;
}

static inline void 
ipcp_callsite_param_count_set (ipcp_callsite cs, int i) 
{
  ((struct ipa_edge *)cs->aux)->ipcp_param_num = i;
}

static inline struct ipcp_jump_func *
ipcp_callsite_param (ipcp_callsite cs, int i) 
{
  return &(((struct ipa_edge *)cs->aux)->ipcp_param_map[i]);
}

static inline ipcp_method 
ipcp_callsite_callee (ipcp_callsite cs)
{
  return cs->callee;
}


static inline void
ipcp_callsite_param_set_type (ipcp_callsite cs, int i,  enum Jfunc_type type1) 
{
  ((struct ipa_edge *)cs->aux)->ipcp_param_map[i].type = type1;
}

static inline void
ipcp_callsite_param_set_info_type (ipcp_callsite cs, int i, 
				   ipcp_int *info_type1) 
{
  ipcp_jf_set_info_type (ipcp_callsite_param (cs, i), info_type1);
}

static inline void
ipcp_callsite_param_map_create (ipcp_callsite cs) 
{
  ((struct ipa_edge *)cs->aux)->ipcp_param_map = 
    xcalloc (ipcp_callsite_param_count (cs), sizeof (struct ipcp_jump_func));   
}

static inline tree
ipcp_callsite_tree (ipcp_callsite cs) 
{
  return cs->call_expr;
}

static inline ipcp_method 
ipcp_callsite_caller (ipcp_callsite cs) 
{
  return cs->caller;
}
 
/* Counts how many arguments this callsite has.  */
static void 
ipcp_callsite_compute_count (ipcp_callsite cs)
{
  tree callTree;
  tree arg;
  int arg_num;

  callTree = ipcp_callsite_tree (cs);
  if (TREE_CODE(callTree) != CALL_EXPR)
    abort();
  arg = TREE_OPERAND (callTree, 1);
  arg_num=0;
  for (; arg != NULL_TREE; arg = TREE_CHAIN (arg))
    {
      arg_num++;
    }
  ipcp_callsite_param_count_set (cs,arg_num);
}

/* Computes jump functions and inserts the information 
   in the ipcp_param_map array.  */
static void  
ipcp_callsite_compute_param (ipcp_callsite cs)
{
  tree callTree;
  tree arg;
  int arg_num;
  int i;
  ipcp_method mt;
  ipcp_int info_type;

  if (ipcp_callsite_param_count(cs) == 0)
    return;
  ipcp_callsite_param_map_create (cs);
  callTree = ipcp_callsite_tree (cs);
  if (TREE_CODE(callTree) != CALL_EXPR)
    abort();
  arg = TREE_OPERAND (callTree, 1);
  arg_num=0;

  for (; arg != NULL_TREE; arg = TREE_CHAIN (arg))
    {
      if (TREE_CODE (TREE_VALUE (arg)) == PARM_DECL)
	{  
	  mt = ipcp_callsite_caller (cs);
	  i =  ipcp_method_tree_map (mt, TREE_VALUE (arg));
	  if (ipcp_method_is_modified (mt, i))
	    {
	      ipcp_callsite_param_set_type (cs, arg_num, UNKNOWN_IPATYPE);
	    }
	  else
	    {
	      ipcp_callsite_param_set_type (cs, arg_num, FORMAL_IPATYPE);
	      ipcp_int2ipcp_int (&info_type, i);
	      ipcp_callsite_param_set_info_type (cs, arg_num, &info_type);
	    }
	}
      else if (TREE_CODE (TREE_VALUE (arg)) == INTEGER_CST)
	{
	  ipcp_callsite_param_set_type (cs, arg_num, CONST_IPATYPE);
	  ipcp_callsite_param_set_info_type (cs, arg_num,
					     &TREE_INT_CST (TREE_VALUE (arg)));
  	}
      else
	{
	  ipcp_callsite_param_set_type (cs, arg_num, UNKNOWN_IPATYPE);
	}
	arg_num++;
    }
}


/* ipcp_method interface.  */

static inline void
ipcp_formal_create (ipcp_method mt) 
{
    ((struct ipa_node *)mt->aux)->ipcp_cval = 
    xcalloc (ipcp_method_formal_count (mt), sizeof (struct ipcp_formal));
}

static inline int
ipcp_method_formal_count (ipcp_method mt)
{
  return ((struct ipa_node *)mt->aux)->ipcp_arg_num;
}

static inline void 
ipcp_method_formal_count_set (ipcp_method mt, int i) 
{
  ((struct ipa_node *)mt->aux)->ipcp_arg_num = i;
}

static inline void 
ipcp_method_cval_set (ipcp_method mt, int i, struct ipcp_formal *cval)             
{
  ((struct ipa_node *)mt->aux)->ipcp_cval[i].cvalue_type = cval->cvalue_type;   
  ipcp_cval_set_cvalue ( ipcp_method_cval (mt, i), ipcp_cval_get_cvalue (cval));     
}

static inline struct ipcp_formal *
ipcp_method_cval (ipcp_method mt, int info_type) 
{
  return &(((struct ipa_node *)mt->aux)->ipcp_cval[info_type]);
}

static inline void 
ipcp_method_cval_set_cvalue_type (ipcp_method mt, int i, 
				  enum Cvalue_type cval_type)  
{
  ((struct ipa_node *)mt->aux)->ipcp_cval[i].cvalue_type = cval_type;
}

static inline bool 
ipcp_method_is_modified (ipcp_method mt, int i)
{
  return ((struct ipa_node *)mt->aux)->ipcp_mod[i].mod;
}

static inline tree 
ipcp_method_get_tree (ipcp_method mt, int i)
{
  return ((struct ipa_node *)mt->aux)->ipcp_param_tree[i].param_tree;
}

static inline void 
ipcp_method_tree_map_create (ipcp_method mt)
{ 
  ((struct ipa_node *)mt->aux)->ipcp_param_tree = 
    xcalloc (ipcp_method_formal_count (mt), sizeof (struct ipcp_tree_map));
}

static inline void  
ipcp_method_modify_create (ipcp_method mt) 
{ 
  ((struct ipa_node *)mt->aux)->ipcp_mod = 
    xcalloc (ipcp_method_formal_count (mt), sizeof (struct ipcp_modify));
}

static inline void  
ipcp_method_modify_set (ipcp_method mt, int i, bool val)
{
  ((struct ipa_node *)mt->aux)->ipcp_mod[i].mod = val;
}

static inline ipcp_callsite
ipcp_cs_get_first (ipcp_method mt)
{
  return mt->callees;
}

static inline ipcp_callsite
ipcp_cs_get_next (ipcp_callsite cs)
{
  return cs->next_callee;
}

static inline bool 
ipcp_cs_not_last (ipcp_callsite cs) 
{
  return (cs != NULL);
}

/* Returning the parameter index of the ptree.  */
static int  
ipcp_method_tree_map (ipcp_method mt, tree ptree)
{
  int i;

  for (i =0; i < ipcp_method_formal_count (mt); i++)
    {
      if (((struct ipa_node *)mt->aux)->ipcp_param_tree[i].param_tree == ptree)
	return i;
    }
  return -1;
}

/* Inserting the method's PARM DECL trees to the ipcp_param_tree array.  */
static void  
ipcp_method_compute_tree_map (ipcp_method mt)
{
  tree fndecl;
  tree fnargs;
  tree parm;
  int param_num;
 
  ipcp_method_tree_map_create (mt);
  fndecl = mt->decl;
  fnargs = DECL_ARGUMENTS (fndecl);
  param_num = 0;
  for (parm = fnargs; parm; parm = TREE_CHAIN (parm))
    {
      ((struct ipa_node *)mt->aux)->ipcp_param_tree[param_num].param_tree = parm;
      param_num++;
    }
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
      || ipcp_cval_get_cvalue_type (cval2)== BOTTOM)
    {
      ipcp_cval_set_cvalue_type (cval, BOTTOM);
      return;
    }
  if (ipcp_cval_get_cvalue_type (cval1) == TOP)
    {
      ipcp_cval_set_cvalue_type (cval, ipcp_cval_get_cvalue_type (cval2));
      ipcp_cval_set_cvalue (cval, ipcp_cval_get_cvalue (cval2));
      return;
    }
  if (ipcp_cval_get_cvalue_type (cval2) == TOP)
    {
      ipcp_cval_set_cvalue_type (cval, ipcp_cval_get_cvalue_type (cval1));
      ipcp_cval_set_cvalue (cval, ipcp_cval_get_cvalue (cval1));
      return;
    }
  if (!ipcp_cval_equal_cvalues (ipcp_cval_get_cvalue (cval1), 
				ipcp_cval_get_cvalue (cval2)))
    {
      ipcp_cval_set_cvalue_type(cval, BOTTOM);
      return;
    }
  ipcp_cval_set_cvalue_type (cval, ipcp_cval_get_cvalue_type (cval1));
  ipcp_cval_set_cvalue (cval, ipcp_cval_get_cvalue (cval1));
}

/* Given the jump function, computes the  value of cval.  */
static void 
ipcp_cval_compute (struct ipcp_formal *cval,ipcp_method mt,
                     enum Jfunc_type type, ipcp_int *info_type)
{
  if (type == UNKNOWN_IPATYPE)
    ipcp_cval_set_cvalue_type (cval, BOTTOM);
  else if (type == CONST_IPATYPE)
    {
      ipcp_cval_set_cvalue_type (cval, CONST_VALUE);
      ipcp_cval_set_cvalue (cval, info_type);
    }
  else if (type == FORMAL_IPATYPE)
    {
      ipcp_cval_set_cvalue_type (cval, 
				 ipcp_cval_get_cvalue_type (ipcp_method_cval (mt, 
									      ipcp_get_int (info_type))));
      ipcp_cval_set_cvalue (cval, 
			    ipcp_cval_get_cvalue (ipcp_method_cval (mt, 
								    ipcp_get_int (info_type))));
    }
}

/* True when cval1's and cval2's values are not the same.  */
static bool 
ipcp_cval_changed (struct ipcp_formal *cval1, struct ipcp_formal *cval2)
{
  
 
    if (ipcp_cval_get_cvalue_type (cval1) == ipcp_cval_get_cvalue_type (cval2))
      {
        if (ipcp_cval_get_cvalue_type (cval1) != CONST_VALUE)
          return false;
        if (ipcp_cval_equal_cvalues (ipcp_cval_get_cvalue (cval1), 
				     ipcp_cval_get_cvalue (cval2)))
           return false;
      }
  return true;
}

/* Counts how many parameters a method has.  */
static void  
ipcp_method_formal_compute_count (ipcp_method mt)
{
  tree fndecl;
  tree fnargs;
  tree parm;
  int param_num;

  fndecl = mt->decl;
  fnargs = DECL_ARGUMENTS (fndecl);
  param_num = 0;
  for (parm = fnargs; parm; parm = TREE_CHAIN (parm))
    {
      param_num++;
    }
  ipcp_method_formal_count_set (mt, param_num);
}

/* Initializes the ipcp_cval array with TOP values.  */
static void 
ipcp_method_cval_init (ipcp_method mt)
{
  int i;
  tree parm_tree;

  ipcp_formal_create (mt);
  for(i = 0; i < ipcp_method_formal_count (mt); i++)
    {
      parm_tree = ipcp_method_get_tree (mt, i);
      if (TREE_CODE (TREE_TYPE (parm_tree)) == INTEGER_TYPE
          && !get_qualified_type (TREE_TYPE (parm_tree),TYPE_QUAL_CONST))
	ipcp_method_cval_set_cvalue_type (mt, i, TOP);
      else 
	ipcp_method_cval_set_cvalue_type (mt, i, BOTTOM);     
    }
}

/* Called by ipcp_walk_statements(). In the case a paramer 
   is modified within the method,
   the appropriate entry is updated in the ipcp_mod array.  */
static tree 
ipcp_modify_walk_tree (tree *tp, void *data)
{
  tree t = *tp;
  int i, j;

  ipcp_method mt = (ipcp_method)data;
  switch (TREE_CODE (t))
    {
    case MODIFY_EXPR:
      if (TREE_CODE(TREE_OPERAND (t, 0)) == PARM_DECL)
	{
	  i = ipcp_method_tree_map (mt, TREE_OPERAND (t, 0));
          ipcp_method_modify_set (mt, i, true);
	}
      break;
    case ADDR_EXPR:
      if( TREE_CODE (TREE_OPERAND (t, 0)) == PARM_DECL )
        {
	  i = ipcp_method_tree_map (mt, TREE_OPERAND (t, 0));
          ipcp_method_modify_set (mt, i, true);
        }
      break;
    case ASM_EXPR:   
      for (j = 0; j < ipcp_method_formal_count (mt); j++)
	{
	  ipcp_method_modify_set (mt, j, true);
	}
	break;
    default:
      break;
    }
  return NULL;
}

/* Initializes the ipcp_mod array.  */
static void 
ipcp_method_modify_init (ipcp_method mt)
{
  int i;
 
  ipcp_method_modify_create (mt);
  for(i = 0; i < ipcp_method_formal_count (mt); i++)
    {
      ipcp_method_modify_set (mt, i, false);
    }
}

/* The modify computation driver.  */
static void 
ipcp_method_compute_modify (ipcp_method mt)
{
  tree decl;
  tree body;
  int j;

  ipcp_method_modify_init (mt);
  decl = mt->decl;
  if (!tree_inlinable_function_p (decl))
    {
      for (j = 0; j < ipcp_method_formal_count (mt); j++)
	{
	  ipcp_method_modify_set (mt, j, true);
	}
      return;
    }
  body = DECL_SAVED_TREE (decl);
  if (body !=NULL)
    if (TREE_CODE (body) == BIND_EXPR)
      ipcp_walk_statements (mt, &body);
}

/* Walk over all statements of the method 
   and compute modify.  */ 
static void
ipcp_walk_statements (ipcp_method mt, tree *stmt_p)
{
  tree stmt = *stmt_p;
  enum tree_code code = TREE_CODE (stmt); 
  tree_stmt_iterator i;

  switch (code)
    {
    case STATEMENT_LIST:
      for (i = tsi_start (stmt); !tsi_end_p (i); )
	  {
            ipcp_walk_statements (mt, tsi_stmt_ptr (i));
	      tsi_next (&i);
	  }     
      break;
    case BIND_EXPR:
      ipcp_walk_statements (mt, &BIND_EXPR_BODY (stmt));
      break;
    default:
      ipcp_modify_walk_tree (&stmt, (void *)mt);
      break;
    }
}

/* jump function interface.  */

static inline enum Jfunc_type
get_type (struct ipcp_jump_func *jf)
{
  return jf->type;
}

static inline ipcp_int *
ipcp_jf_get_info_type (struct ipcp_jump_func *jf) 
{
  return &(jf->info_type);
}

static inline void 
ipcp_jf_set_info_type (struct ipcp_jump_func *jf, ipcp_int *info_type)
{
  jf->info_type.low = info_type->low;
  jf->info_type.high = info_type->high;
}

static inline void 
ipcp_int2ipcp_int (ipcp_int *info_type, int const_val)
{
  info_type->low = (HOST_WIDE_INT)const_val;
  info_type->high = 0;
}

static inline int
ipcp_get_int (ipcp_int *info_type)
{
  return (int)info_type->low;
}

/* cval interface.  */

static inline void 
ipcp_cval_set_cvalue_type (struct ipcp_formal *cval, enum Cvalue_type type)
{
  cval->cvalue_type = type;
}

static inline void
ipcp_cval_set_cvalue (struct ipcp_formal *cval, ipcp_int *value) 
{
  cval->cvalue.low = value->low;
  cval->cvalue.high = value->high;
}

static inline ipcp_int *
ipcp_cval_get_cvalue (struct ipcp_formal *cval)
{
  return &(cval->cvalue);
}

static inline enum Cvalue_type
ipcp_cval_get_cvalue_type (struct ipcp_formal *cval)
{
  return cval->cvalue_type;
}

static inline bool 
ipcp_cval_equal_cvalues (ipcp_int *const_val1, ipcp_int *const_val2)
{
  if ((const_val1->low == const_val2->low) 
      && (const_val1->high == const_val2->high)) 
    return true; 
  return false;
}

/* Create a new assignment statment and make
   it the first statemant in the function FN
   parse tree.
   PARM1 is the lhs of the assignment and
   is the rhs. */
static void 
constant_val_insert (tree fn, tree parm1, tree val)
{
  tree_stmt_iterator i; 
  tree init_stmts = NULL_TREE;
  tree init_stmt;
  tree stmt_list_body;
  tree body;
  
  tree rhs = convert (TREE_TYPE (parm1), val);
  if (rhs == error_mark_node)
    return;
  init_stmt = build (MODIFY_EXPR, TREE_TYPE (parm1), parm1, rhs);
  append_to_statement_list (init_stmt, &init_stmts);
  /* Find the first statment in the tree. */
  body = DECL_SAVED_TREE (fn);
  if (body == NULL)
    return;  
  if (TREE_CODE (body) != BIND_EXPR)
    return;
  stmt_list_body =  BIND_EXPR_BODY (body);
  if (stmt_list_body == NULL)
    return;
  i = tsi_start (stmt_list_body);
  tsi_link_before (&i, init_stmts, TSI_NEW_STMT);  
}

/* Build the constant tree and call constant_val_insert().  */
static void 
ipcp_propagate_const (ipcp_method mt, int param, ipcp_int *cvalue)
{
  tree fndecl;
  tree const_val;
  tree parm_tree;

  fndecl = mt->decl;
  parm_tree = ipcp_method_get_tree (mt, param);
  const_val = build_int_2 (cvalue->low, cvalue->high);   
  if (parm_tree != NULL)
    if (const_val != NULL)
      if(fndecl != NULL)
	constant_val_insert (fndecl, parm_tree, const_val);   
}

/* Propagates the constant parameters found by ipcp_stage2()
   to the function's code.  */
static void 
ipcp_stage3 (void)
{
  ipcp_method node;
  int i;
  ipcp_int *cvalue;
 
  for (node = cgraph_nodes; node; node = node->next)
    {  
      /* Propagation of the constant is forbidden in 
         certain conditions.  */
      if (ipcp_method_dont_propagate (node))
        continue;
      for(i=0; i < ipcp_method_formal_count (node); i++)
	{
	  if (ipcp_cval_get_cvalue_type (ipcp_method_cval (node, i)) 
	      == CONST_VALUE) 
	    {
	      cvalue = ipcp_cval_get_cvalue (ipcp_method_cval (node, i));
              ipcp_propagate_const (node, i, cvalue);                       
	    }
	}
    }
}

/* Initialization and computation of IPA Constant
   Propagation data structures. It is an IntraProcedural
   analysis of methods,which gathers information to be propagated
   later on.  */
static void 
ipcp_stage1 (void)
{
   ipcp_method node;
   ipcp_callsite cs;
     
   for (node = cgraph_nodes; node; node = node->next)
     {
       ipcp_method_formal_compute_count (node);
       ipcp_method_compute_tree_map (node);
       ipcp_method_cval_init (node);
       ipcp_method_compute_modify (node);      
     }
   for (node = cgraph_nodes; node; node = node->next)
     {
       /* building jump functions  */
       for (cs = ipcp_cs_get_first (node); ipcp_cs_not_last (cs);
            cs = ipcp_cs_get_next (cs))
         {
           ipcp_callsite_compute_count (cs);
           if (ipcp_callsite_param_count (cs) 
	       != ipcp_method_formal_count (cs->callee))
             {
               ipcp_callsite_param_count_set (cs, 0);
               ipcp_method_formal_count_set (cs->callee, 0);
             }
           else
             ipcp_callsite_compute_param (cs);
         }       
     }
}

/* Interprocedural analysis. The algorithm propagates constants from
   the caller's parameters to the callee's arguments.  */
static void 
ipcp_stage2 (void)
{
  int i;
  struct ipcp_formal cval1, cval;
  struct ipcp_formal *cval2;
  ipcp_method mt,callee;
  ipcp_callsite cs;
  struct ipcp_jump_func *jump_func;
  enum Jfunc_type type;
  ipcp_int *info_type;
  ipcp_methodlist_p wl;

  wl = ipcp_methodlist_init ();
  while (ipcp_methodlist_not_empty (wl))
    {
      mt = ipcp_remove_method (&wl);
      for (cs = ipcp_cs_get_first (mt); ipcp_cs_not_last (cs); 
	   cs = ipcp_cs_get_next (cs))
	{
	  callee = ipcp_callsite_callee (cs);
	  for (i = 0; i < ipcp_callsite_param_count (cs); i++)
	    {
	      jump_func = ipcp_callsite_param (cs, i);
	      type = get_type (jump_func);
	      info_type = ipcp_jf_get_info_type (jump_func);
	      ipcp_cval_compute (&cval1, mt, type, info_type);
	      callee = ipcp_callsite_callee (cs);
	      cval2 = ipcp_method_cval (callee, i);
	      ipcp_cval_meet (&cval, &cval1, cval2);
	      if (ipcp_cval_changed (&cval, cval2))
		{
		  ipcp_method_cval_set (callee, i, &cval);
                  ipcp_add_method (&wl, callee);
		}
	    }
	}
    }
}

/* Allocate and initialize ipa_node structure.  */
static void
ipa_nodes_create (void)
{
  ipcp_method node;
  
  for (node = cgraph_nodes; node; node = node->next)
     {
       node->aux = xcalloc (1, sizeof (struct ipa_node));     
       ((struct ipa_node *)node->aux)->ipcp_cval = NULL;
       ((struct ipa_node *)node->aux)->ipcp_arg_num =0;
       ((struct ipa_node *)node->aux)-> ipcp_param_tree = NULL;
       ((struct ipa_node *)node->aux)->ipcp_mod = NULL;
     }
}

/* Allocate and initialize ipa_edge structure.  */
static void
ipa_edges_create (void)
{
  ipcp_method node;
  ipcp_callsite cs;
  
  for (node = cgraph_nodes; node; node = node->next)
     {
        for (cs = ipcp_cs_get_first (node); ipcp_cs_not_last (cs);
	     cs = ipcp_cs_get_next (cs))
	  {
	    cs->aux = xcalloc (1, sizeof (struct ipa_edge));
	    ((struct ipa_edge *)cs->aux)->ipcp_param_map = NULL;
	    ((struct ipa_edge *)cs->aux)->ipcp_param_num = 0;
	  }
     
     }
}

/* Frees ipa_node structure.  */
static void
ipa_nodes_free (void)
{
  ipcp_method node;
 
  for (node = cgraph_nodes; node; node = node->next)
    {
      free (node->aux);     
      node->aux = NULL;
    }
}

/* Frees ipa_edge structure.  */
static void
ipa_edges_free (void)
{
  ipcp_method node;
  ipcp_callsite cs;
  
  for (node = cgraph_nodes; node; node = node->next)
     {
        for (cs = ipcp_cs_get_first (node); ipcp_cs_not_last (cs);
	     cs = ipcp_cs_get_next (cs))
	  {
	   
	    free (cs->aux);
	    cs->aux = NULL;
	  }
     
     }
}

/* The IPA constant propagation driver.  */
void 
ipcp_driver (void)
{
  if (cgraph_dump_file)
     fprintf (cgraph_dump_file, "\nIPA constant propagation start:\n");
  ipa_nodes_create ();
  ipa_edges_create ();
  ipcp_stage1 ();
  if (cgraph_dump_file)
    {
      fprintf (cgraph_dump_file, "\nIPA structures before propagation:\n");
      ipcp_structures_print (cgraph_dump_file); 
    }
  ipcp_stage2 ();
  if (cgraph_dump_file)
    {
      fprintf (cgraph_dump_file, "\nIPA structures after propagation:\n");
      ipcp_structures_print (cgraph_dump_file);      
    }
  ipcp_stage3 ();  
  ipcp_free (); 
  ipa_nodes_free ();
  ipa_edges_free ();
  if (cgraph_dump_file)
    fprintf (cgraph_dump_file, "\nIPA constant propagation end\n");
}

/* Frees the ipcp_method's IPA data structures.  */
static void 
ipcp_free (void)
{
  ipcp_method node;
  ipcp_callsite cs;

  for (node = cgraph_nodes; node; node = node->next)
     {
       free (((struct ipa_node *)node->aux)->ipcp_cval);
       free (((struct ipa_node *)node->aux)->ipcp_param_tree);
       free (((struct ipa_node *)node->aux)->ipcp_mod); 
       for (cs = ipcp_cs_get_first (node); ipcp_cs_not_last (cs);
            cs = ipcp_cs_get_next (cs))
         free (((struct ipa_edge *)cs->aux)->ipcp_param_map);
     }
}

/* Check conditions to forbid constant propagation.  */
static bool
ipcp_method_dont_propagate (ipcp_method mt)
{
  if (!tree_inlinable_function_p (mt->decl))
    return true;
  if (ipcp_method_contains_asm (mt))
    return true;
  return false;
}

/* Called by walk_tree to find. Returns true if an asm expr was found  */
static tree 
ipcp_asm_walk_tree (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED, 
		    void *data ATTRIBUTE_UNUSED)
{
  tree t = *tp;
  
  switch (TREE_CODE (t))
    {
    case ASM_EXPR:
      return t;
      break;
    default:
      break;
    }
  return NULL;
}

/*  Finds if there are any asm expr in the function.  */
static bool
ipcp_method_contains_asm (ipcp_method mt)
{
  tree decl;
  tree body;

  decl = mt->decl;
  body = DECL_SAVED_TREE (decl);
  if (body != NULL)
    if (walk_tree (&body, ipcp_asm_walk_tree, NULL, NULL))
      return true;
  return false;
}

/* Debuffing interface.  */

/* Prints all IPA constant propagation data structures.  */
static void 
ipcp_structures_print (FILE *f)
{
  ipcp_method_cval_print (f);
  ipcp_method_tree_print (f);
  ipcp_method_modify_print (f);
  ipcp_callsite_param_print (f);
}

static void 
ipcp_method_cval_print (FILE *f)
{
  ipcp_method node;
  int i;
  ipcp_int* cvalue;

  fprintf(f, "\nCVAL PRINT\n");
   for (node = cgraph_nodes; node; node = node->next)
     {
       fprintf (f, "printing cvals %s:\n", cgraph_node_name (node));
       for(i=0; i < ipcp_method_formal_count (node); i++)
	 {
	   if (ipcp_cval_get_cvalue_type (ipcp_method_cval (node, i)) 
               == CONST_VALUE) 
	     {
               fprintf (f, " param [%d]: ", i);
	       fprintf (f, "type is CONST ");
               cvalue = ipcp_cval_get_cvalue (ipcp_method_cval (node, i));
	       fprintf (f, "value is  %d %d\n",(int)cvalue->high, 
                        (int)cvalue->low);
	     }
	   else if (ipcp_method_cval (node, i)->cvalue_type == TOP)
	     fprintf(f, "param [%d]: type is TOP  \n", i);           
	   else	     
	     fprintf(f, "param [%d]: type is BOTTOM  \n", i);
	 }
     }
}

static void  
ipcp_method_tree_print (FILE *f) 
{
  int i;
  tree temp;
  ipcp_method node;

  fprintf(f, "\nPARAM TREE MAP PRINT\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      fprintf (f, "method  %s Trees :: \n", cgraph_node_name (node));
      for (i =0; i < ipcp_method_formal_count (node); i++)
	{
	  temp = ipcp_method_get_tree (node, i);
	  if (TREE_CODE (temp) == PARM_DECL)
	    fprintf(f, "  param [%d] : %s\n", i, 
                    (*lang_hooks.decl_printable_name) (temp, 2));
	  else
	    {
	      fprintf(stderr, "not a declaration\n");
	      abort();
	    }
	}
      
    }
}

static void  
ipcp_method_modify_print (FILE *f) 
{
  int i;
  bool temp;
  ipcp_method node;

  fprintf(f, "\nMODIFY PRINT\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      fprintf (f, "method  %s :: \n", cgraph_node_name (node));
      for (i =0; i < ipcp_method_formal_count (node); i++)
	{
	  temp = ipcp_method_is_modified (node, i);
          if (temp)
            fprintf (f, " param [%d] true \n", i);
          else
            fprintf (f, " param [%d] false \n", i);
	}
      
    }
}
 
static void  
ipcp_callsite_param_print (FILE *f)
{
  ipcp_method node;
  int i;
  ipcp_callsite cs;
  struct ipcp_jump_func *jump_func;
  enum Jfunc_type type;
  ipcp_int* info_type;
  
  fprintf(f, "\nCALLSITE PARAM PRINT\n");
  for (node = cgraph_nodes; node; node = node->next)
    {    
      for (cs = ipcp_cs_get_first (node); ipcp_cs_not_last (cs);
            cs = ipcp_cs_get_next (cs))
         {
           fprintf (f, "callsite  %s ", cgraph_node_name (node));
           fprintf (f, "-> %s :: \n", cgraph_node_name (cs->callee));
            for (i = 0; i < ipcp_callsite_param_count (cs); i++)
	    {
              jump_func = ipcp_callsite_param (cs, i);
	      type = get_type(jump_func);
	      info_type = ipcp_jf_get_info_type (jump_func);
              fprintf (f, " param %d: ", i);
              if (type == UNKNOWN_IPATYPE)
                fprintf (f, "UNKNOWN\n");
              else if (type == CONST_IPATYPE)
                {
                  fprintf (f, "CONST : ");
                  fprintf (f, "high: %d low: %d\n",(int)info_type->high, 
                           (int)info_type->low);
                }
              else if (type == FORMAL_IPATYPE)
                {
                  fprintf (f, "FORMAL : ");
                  fprintf (f, "%d\n", ipcp_get_int (info_type));
                }
            }
         }
    }
}
