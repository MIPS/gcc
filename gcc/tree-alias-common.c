
/* Tree based points-to analysis
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Daniel Berlin <dberlin@dberlin.org>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include "config.h"
#include "system.h"
#include "ggc.h"
#include "tree-alias-type.h"
#include "tree-alias-ecr.h"
#include "tree-alias-common.h"
#include "tree-alias-steen.h"

#include "flags.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "diagnostic.h"
#include "tree.h"  
#include "tree-optimize.h"
#include "c-common.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "ssa.h"
#include "varray.h"
#include "c-tree.h"
#include "tree-simple.h"
#include "hashtab.h"
#include "splay-tree.h"
static splay_tree alias_annot;
static GTY ((param_is (struct alias_typevar_def))) varray_type alias_vars = NULL;
static splay_tree varmap; 
static varray_type local_alias_vars;
static varray_type local_alias_varnums;
static alias_typevar get_alias_var_decl PARAMS ((tree));
static alias_typevar get_alias_var PARAMS ((tree));
static tree find_func_aliases PARAMS ((tree *, int *, void *));
static tree find_func_decls PARAMS ((tree *, int *, void *));
static alias_typevar create_fun_alias_var_ptf PARAMS ((tree, tree));
static alias_typevar create_fun_alias_var PARAMS ((tree, int));
static alias_typevar create_alias_var PARAMS ((tree));
static void intra_function_call PARAMS ((varray_type));

static alias_typevar
get_alias_var_decl (decl)
     tree decl;
{
  splay_tree_node node;
  alias_typevar newvar;

  node = splay_tree_lookup (alias_annot, (splay_tree_key) decl);
  if (node != NULL && node->value != 0)
    return (alias_typevar) node->value; 
/* For debugging, remove this, and re-enable the find_func_decls call. */
  newvar = create_alias_var (decl);
  if (!TREE_PUBLIC (decl))
    {
#if STEEN_DEBUG
      fprintf (stderr, "adding ");
      print_c_node (stderr, decl);
      fprintf (stderr, " to list of local alias vars.\n");
#endif
      VARRAY_PUSH_INT (local_alias_varnums, VARRAY_ACTIVE_SIZE (alias_vars) - 1);
      VARRAY_PUSH_GENERIC_PTR (local_alias_vars, decl);
    }
  return newvar;
										    abort ();
}

static alias_typevar
get_alias_var (expr)
     tree expr;
{
  alias_typevar tvar;
  splay_tree_node node;
  
  node = splay_tree_lookup (alias_annot, (splay_tree_key) expr);
  

  /* If it's a decl, get the alias var of the decl. We farm this off
     to get_alias_var_decl so it can abort if the alias var doesn't
     exist, and in case something else *knows* it has a decl, and
     wants the alias var. */

  if (DECL_P (expr))
    return get_alias_var_decl (expr);

  /* Non-DECL's alias vars must be temporary alias vars for an
     expression, since we never create non-temporary ones on anything
     but DECL's.  
     Since they are temporary, we want to erase it from the annotation
     since they are use once. */

  if (node != NULL && node->value != 0)
    {
      tvar = (alias_typevar) node->value;
      splay_tree_remove (alias_annot, (splay_tree_key)expr);
      return tvar;
    }

  /* True constants have no aliases (unless modifiable strings are on,
     in which case i don't think we'll end up with a STRING_CST anyway) */
  if (TREE_CODE_CLASS (TREE_CODE (expr)) == 'c')
    return NULL;


  switch (TREE_CODE (expr))
    {
    case ARRAY_REF:
      {
	/* Find the first non-array ref, and return it's alias
	   variable */
	tree p;
	for (p = expr; TREE_CODE (p) == ARRAY_REF;
	     p = TREE_OPERAND (p, 0));
	return get_alias_var (p);
      }
      break;

    case COMPONENT_REF:
      {
	/* Find the first non-component ref, and return it's alias
	   variable */
	tree p;
	for (p = expr; TREE_CODE (p) == COMPONENT_REF;
	     p = TREE_OPERAND (p, 0));
	return get_alias_var (p);
      }
      break;

    case NOP_EXPR:
    case CONVERT_EXPR:
    case FIX_TRUNC_EXPR:
    case FIX_CEIL_EXPR:
    case FIX_FLOOR_EXPR:
    case FIX_ROUND_EXPR:
    case ADDR_EXPR:
    case INDIRECT_REF:
      /* If it's a ref or cast or conversion of sometmhing, get the
         alias var of the something. */
      return get_alias_var (TREE_OPERAND (expr, 0));
      break;

    default:
      {
	/* Otherwise, we need a new temporary alias variable for this
	   expression. */
	alias_typevar tempvar 
	  = steen_alias_ops->add_var (steen_alias_ops,
				      create_tmp_alias_var
				      (void_type_node,
				       "aliastmp"));
	return tempvar;
      }
    }
}
static void
intra_function_call (args)
     varray_type args;
{
  size_t l = VARRAY_ACTIVE_SIZE (args);
  size_t i;
  tree globvar = getdecls();

  /* We assume that an actual parameter can point to any global. */
  while (globvar)
    {
      if (TREE_CODE (globvar) == VAR_DECL)
	{
	  alias_typevar av = get_alias_var (globvar);
	  for (i = 0; i < l; i++)
	    {
	      alias_typevar argav = VARRAY_GENERIC_PTR (args, i);
	      
	      /* Restricted pointers can't be aliased with other
		 restricted pointers. */	      
	      if (!TYPE_RESTRICT (TREE_TYPE (argav->decl)) 
		  || !TYPE_RESTRICT (TREE_TYPE (av->decl)))
		steen_alias_ops->addr_assign (steen_alias_ops, argav, av);
	    }
	}
      globvar = TREE_CHAIN (globvar);
    }

  /* We assume assignments among the actual parameters. */
  for (i = 0; i < l; i++) 
    {
      alias_typevar argi = VARRAY_GENERIC_PTR (args, i);
      size_t j;
      for (j = 0; j < l; j++)
	{
	  alias_typevar argj;
	  if (i == j)
	    continue;
	  argj = VARRAY_GENERIC_PTR (args, j);

	  /* Restricted pointers can't be aliased with other
	     restricted pointers. */
	  if (!TYPE_RESTRICT (TREE_TYPE (argi->decl)) 
	      || !TYPE_RESTRICT (TREE_TYPE (argj->decl)))
	    steen_alias_ops->simple_assign (steen_alias_ops, argi, argj);
	}
    }
}
static tree
find_func_aliases (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data ATTRIBUTE_UNUSED;
{
  if (TREE_CODE (*tp) == SCOPE_STMT)
    {
      *walk_subtrees = 0;
      return NULL_TREE;
    }

  if (is_simple_modify_expr (*tp))
    {
      tree op0, op1;
      alias_typevar lhsAV = NULL;
      alias_typevar rhsAV = NULL;

      *walk_subtrees = 0;
      op0 = TREE_OPERAND (*tp, 0);
      op1 = TREE_OPERAND (*tp, 1);

      /* lhsAV should always have an alias variable */
      lhsAV = get_alias_var (op0);
      /* rhsAV might not have one, c.f. c = 5 */
      rhsAV = get_alias_var (op1);
      
      /* You would think we could test rhsAV at the top, rather than
	 50 separate times, but we can't, because it can be NULL for
	 operator assignments, where we'd still collect the individual
	 alias typevars for the operator. */

      /* Note that structures are treated as a single alias
         variable, since we can disambiguate based on TBAA first,
         and fall back on points-to. */
      /* x = <something> */
      if (is_simple_varname (op0))
	{
	  /* x = y or x = foo.y */
	  if (is_simple_varname (op1))
	    {
	      if (rhsAV != NULL)
		steen_alias_ops->simple_assign (steen_alias_ops, lhsAV,
						rhsAV);
	    }
	  /* x = *y or x = foo->y */
	  else if (TREE_CODE (op1) == INDIRECT_REF
		   && is_simple_varname (TREE_OPERAND (op1, 0)))
	    {
	      if (rhsAV != NULL)
		steen_alias_ops->ptr_assign (steen_alias_ops, lhsAV, 
					     rhsAV);
	    }
	  /* x = &y = x = &foo.y */
	  else if (TREE_CODE (op1) == ADDR_EXPR
		   && is_simple_varname (TREE_OPERAND (op1, 0)))
	    {
	      if (rhsAV != NULL)
		steen_alias_ops->addr_assign (steen_alias_ops, lhsAV, 
					      rhsAV);
	    }
	  /* x = func(...) */
	  else if (is_simple_call_expr (op1))
	    {
	      /* Heap assignment. These are __attribute__ malloc or
		 something, i'll deal with it later. */
	      if (0) 
		{}
	      else
		{
		  varray_type args;
		  tree arg;
		  tree callop0, callop1;
		  VARRAY_GENERIC_PTR_INIT (args, 1, "Arguments");
		  
		  callop1 = TREE_OPERAND (op1, 1);
		  callop0 = TREE_OPERAND (op1, 0);
		  for (arg = callop1;
		       arg; 
		       arg = TREE_CHAIN (arg))
		    {
		      alias_typevar aav = get_alias_var (TREE_VALUE (arg));
		      if (aav)
			VARRAY_PUSH_GENERIC_PTR (args, aav);

		    }
		  create_fun_alias_var (TREE_OPERAND (callop0, 0), 0);
		  steen_alias_ops->function_call (steen_alias_ops, lhsAV, 
						  get_alias_var (callop0),
						  args);		  
		  if (!steen_alias_ops->ip)
		    intra_function_call (args);
		}

	    }
	  /* x = op (...) */
	  else    
	    {
	      switch (TREE_CODE_CLASS (TREE_CODE (op1)))
		{
		case 'e':  /* an expression */
		case 's':  /* an expression with side effects */
		case '<':  /* a comparison expression */
		case '1':  /* a unary arithmetic expression */
		case 'r':  /* a reference */ 
		case '2':  /* a binary arithmetic expression */
		  {
		    tree op;
		    varray_type ops;
		    int i;
		    VARRAY_GENERIC_PTR_INIT (ops, 1, "Operands");
		    for (i=0; i < TREE_CODE_LENGTH (TREE_CODE (op1)); i++)
		      {
			alias_typevar aav;
			op = TREE_OPERAND (op1, i);
			aav = get_alias_var (op);
			if (aav)
			  VARRAY_PUSH_GENERIC_PTR (ops, aav);
		      }
		    steen_alias_ops->op_assign (steen_alias_ops, lhsAV, 
						ops);
		  }
		  break;
		default:
		  break;
		}
	    }
	}
      /* *x = <something> */
      else
	{
	  /* x.f = y  or x->f = y */
	  if (TREE_CODE (op0) == COMPONENT_REF)
	    {
	      if (rhsAV != NULL)
		steen_alias_ops->simple_assign (steen_alias_ops, lhsAV,
						rhsAV);
	    }
	  /* *x.f = y or *x->f = y */
	  else if (TREE_CODE (op0) == INDIRECT_REF
		   && TREE_CODE (TREE_OPERAND (op0, 0)) == COMPONENT_REF)
	    {
	      if (rhsAV != NULL)
		steen_alias_ops->assign_ptr (steen_alias_ops, lhsAV, 
					     rhsAV);
	    }
	  /* *x = &y */
	  else if (TREE_CODE (op0) == INDIRECT_REF
		   && TREE_CODE (op1) == ADDR_EXPR)
	    {
	      /* This becomes temp = &y and *x = temp . */
	      alias_typevar tempvar = steen_alias_ops->add_var (steen_alias_ops,
								create_tmp_alias_var
								(void_type_node,
								 "aliastmp"));   
	      steen_alias_ops->addr_assign (steen_alias_ops, tempvar, rhsAV);
	      steen_alias_ops->assign_ptr (steen_alias_ops, lhsAV, tempvar);
	    }
	  
	  /* *x = *y */
	  else if (TREE_CODE (op0) == INDIRECT_REF
		   && TREE_CODE (op1) == INDIRECT_REF)
	    {
	      /* This becomes temp = *y and *x = temp . */
	      alias_typevar tempvar = steen_alias_ops->add_var (steen_alias_ops,
								create_tmp_alias_var
								(void_type_node,
								 "aliastmp"));   
	      steen_alias_ops->ptr_assign (steen_alias_ops, tempvar, rhsAV);
	      steen_alias_ops->assign_ptr (steen_alias_ops, lhsAV, tempvar);
	    }
	  
	  /* *x = (cast) y */
	  else if (TREE_CODE (op0) == INDIRECT_REF
		   && is_simple_cast (op1))
	    {
	      /* This becomes temp = (cast) y and  *x = temp. */
	      alias_typevar tempvar = steen_alias_ops->add_var (steen_alias_ops,
								create_tmp_alias_var
								(void_type_node,
								 "aliastmp"));
	      
	      steen_alias_ops->simple_assign (steen_alias_ops, tempvar, rhsAV);
	      steen_alias_ops->assign_ptr (steen_alias_ops, lhsAV, tempvar);
	    }
	  /* *x = <something else */
	  else
	    {
	      if (rhsAV != NULL)
		steen_alias_ops->assign_ptr (steen_alias_ops, lhsAV,
					     rhsAV);
	    }
	}
    }
  /* Calls without return values. */
  else if (is_simple_call_expr (*tp))
    {
      varray_type args;
      tree arg;
      VARRAY_GENERIC_PTR_INIT (args, 1, "Arguments");
      for (arg = TREE_OPERAND (*tp, 1); arg; arg = TREE_CHAIN (arg))
	{
	  alias_typevar aav = get_alias_var (TREE_VALUE (arg));
	  if (aav)
	    VARRAY_PUSH_GENERIC_PTR (args, aav);
	}
      create_fun_alias_var (TREE_OPERAND (TREE_OPERAND (*tp, 0), 0), 0);
      steen_alias_ops->function_call (steen_alias_ops, NULL, 
				      get_alias_var (TREE_OPERAND (*tp, 0)),
				      args);
      if (!steen_alias_ops->ip)
	intra_function_call (args);
    }
      
  return NULL_TREE;
}

static tree
find_func_decls (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data ATTRIBUTE_UNUSED;
{
  if (TREE_CODE (*tp) == DECL_STMT)
    {
      create_alias_var (DECL_STMT_DECL (*tp));
      VARRAY_PUSH_INT (local_alias_varnums, VARRAY_ACTIVE_SIZE (alias_vars) - 1);
#if STEEN_DEBUG
      fprintf (stderr, "adding ");
      print_c_node (stderr, DECL_STMT_DECL (*tp));
      fprintf (stderr, " to list of local alias vars.\n");
#endif
      VARRAY_PUSH_GENERIC_PTR (local_alias_vars, DECL_STMT_DECL (*tp));
    }
  return NULL_TREE;
}

/* 
   Create the alias variables for a function definition.

   This includes:
   1. The function itself.
   2. The arguments.
   3. The locals.
   4. The return value.
   
   DECL is the function declaration.
   FORCE determines whether we force creation of the alias variable,
   regardless of whether one exists or not. 
*/
static alias_typevar
create_fun_alias_var (decl, force)
     tree decl;
     int force;
{
  alias_typevar avar, retvar;
  tree rdecl;
  varray_type params = NULL;
  splay_tree_node node;
  node = splay_tree_lookup (alias_annot, (splay_tree_key) decl);
  if (!force)
  {
    if (node != NULL && node->value != 0)
      return (alias_typevar) node->value; 
  }

  VARRAY_GENERIC_PTR_INIT (params, 1, "Arguments");
  if (DECL_ARGUMENTS (decl) != NULL)
    {
      tree arg;
      for (arg = DECL_ARGUMENTS (decl); arg; arg = TREE_CHAIN (arg))
	{
	  alias_typevar tvar = create_alias_var (arg);
	  splay_tree_insert (alias_annot, (splay_tree_key) arg, (splay_tree_value) tvar); 
	  VARRAY_PUSH_GENERIC_PTR (params, tvar);
	}
    }
  else if (TYPE_ARG_TYPES (TREE_TYPE (decl)) != NULL)
    {
      tree arg;
      /* FIXME: Handle varargs */
      for (arg = TYPE_ARG_TYPES (TREE_TYPE (decl));
	   arg && TREE_VALUE (arg) != void_type_node;
	   arg = TREE_CHAIN (arg))
	{
	  tree fakedecl = create_tmp_alias_var (TREE_VALUE (arg), "normarg");
	  alias_typevar tvar = create_alias_var (fakedecl);
	  VARRAY_PUSH_GENERIC_PTR (params, tvar);
	}
    }
  /* Functions declared like void f() are *not* equivalent to void
     f(void). You can pass an argument to them. Thus, we need to
     create some fake argument that would alias any actuals that get
     passed to our function.  */
  else
    {
      tree fakedecl = create_tmp_alias_var (void_type_node, "fakearg");
      alias_typevar fakevar = create_alias_var (fakedecl);
      VARRAY_PUSH_GENERIC_PTR (params, fakevar);
    }

  rdecl = create_tmp_alias_var (TREE_TYPE (TREE_TYPE (decl)), "_rv_");
  retvar = steen_alias_ops->add_var (steen_alias_ops, rdecl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, retvar);
  avar = steen_alias_ops->add_var (steen_alias_ops, decl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, avar);

  steen_alias_ops->function_def (steen_alias_ops, avar, params, retvar);
  splay_tree_insert (alias_annot, (splay_tree_key) decl, 
		     (splay_tree_value) avar);
  /* FIXME: Also, if this is a defining declaration then add the annotation
     to all extern definitions of the function. */
  return avar;
}

/* 
   Create the alias variables for a ptf.

   This includes:
   1. The function itself.
   2. The arguments.
   3. The return value.
*/
static alias_typevar
create_fun_alias_var_ptf (decl, type)
     tree decl;
     tree type;
{
  alias_typevar avar, retvar;
  tree rdecl;
  varray_type params = NULL;
  splay_tree_node node;

  node = splay_tree_lookup (alias_annot, (splay_tree_key) decl);

  if (node != NULL && node->value != 0)
    return (alias_typevar) node->value; 

  VARRAY_GENERIC_PTR_INIT (params, 1, "Arguments");

  if (TYPE_ARG_TYPES  (type) != NULL)
    {
      tree arg;
      /* FIXME: Handle varargs */
      for (arg = TYPE_ARG_TYPES (type); 
	   arg && TREE_VALUE (arg) != void_type_node;
	   arg = TREE_CHAIN (arg))
	{
	  tree fakedecl = create_tmp_alias_var (TREE_VALUE (arg), "ptfarg");
	  alias_typevar tvar = create_alias_var (fakedecl);
	  VARRAY_PUSH_GENERIC_PTR (params, tvar);
	}
    }
  /* Functions declared like void f() are *not* equivalent to void
     f(void). You can pass an argument to them. Thus, we need to
     create some fake argument that would alias any actuals that get
     passed to our function.  */
  else
    {
      tree fakedecl = create_tmp_alias_var (void_type_node, "fakearg");
      alias_typevar fakevar = create_alias_var (fakedecl);
      VARRAY_PUSH_GENERIC_PTR (params, fakevar);
    }

  rdecl = create_tmp_alias_var (TREE_TYPE (type), "_rv_");
  retvar = steen_alias_ops->add_var (steen_alias_ops, rdecl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, retvar);

  avar = steen_alias_ops->add_var (steen_alias_ops, decl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, avar);

  steen_alias_ops->function_def (steen_alias_ops, avar, params, retvar);
  splay_tree_insert (alias_annot, (splay_tree_key) decl, 
		     (splay_tree_value) avar);

  return avar;
}

static alias_typevar
create_alias_var (decl)
     tree decl;
{
  splay_tree_node node;
  alias_typevar avar;
  
 
  node = splay_tree_lookup (alias_annot, (splay_tree_key)decl);

  if (node != NULL && node->value != 0)
    return (alias_typevar) node->value;

  /* FIXME: Need to handle creating alias variables for PTF's.  The
     cleanest way is to make create_fun_alias_var take 2 arguments:
     1. The function variable decl (IE the name in the case of normal
     function, the PTF variable's name in the case of PTF's)
     2. The function type.

   */
  if (TREE_CODE (TREE_TYPE (decl)) == POINTER_TYPE)
    if (TREE_CODE (TREE_TYPE (TREE_TYPE (decl))) == FUNCTION_TYPE)
      create_fun_alias_var_ptf (decl, TREE_TYPE (TREE_TYPE (decl)));

  avar = steen_alias_ops->add_var (steen_alias_ops, decl);
  splay_tree_insert (alias_annot, (splay_tree_key)decl, 
		     (splay_tree_value) avar);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, avar);

  /* FIXME: Add the annotation to all extern definitions of this variable. */
  return avar;
}

static unsigned int splaycount = 0;
static int splay_tree_count PARAMS ((splay_tree_node, void *));
static int display_points_to_set PARAMS ((splay_tree_node, void *));
static void display_points_to_set_helper PARAMS ((alias_typevar));
unsigned int splay_tree_size PARAMS ((splay_tree));
static void
display_points_to_set_helper (tvar)
	alias_typevar tvar;
{
  size_t i;
  varray_type tmp;
  tmp = alias_tvar_pointsto (tvar);
  if (VARRAY_ACTIVE_SIZE (tmp) <= 0)
    return;
  print_c_node (stderr, tvar->decl);
  fprintf (stderr, " => { ");
  for (i = 0; i < VARRAY_ACTIVE_SIZE (tmp); i++)
    {
      alias_typevar tmpatv = (alias_typevar) VARRAY_GENERIC_PTR (tmp, i);
      print_c_node  (stderr, tmpatv->decl);
      fprintf (stderr, ", ");
    }
  fprintf (stderr, " }\n");
}

static int
display_points_to_set (node, data)
     splay_tree_node node;
     void *data ATTRIBUTE_UNUSED;
{
  /* Display the points to set for the given alias_typevar
     (in the splay tree node.) */
  display_points_to_set_helper ((alias_typevar)node->value); 
  return 0;
}

static int 
splay_tree_count (node, data)
     splay_tree_node node ATTRIBUTE_UNUSED;
     void *data ATTRIBUTE_UNUSED;
{
  splaycount++;
  return 0;
}

unsigned int 
splay_tree_size (st)
     splay_tree st;
{
  splaycount = 0;
  splay_tree_foreach (st, splay_tree_count, NULL);
  return splaycount;
}

void
create_alias_vars ()
{
  size_t i;
  tree currdecl = getdecls ();
  init_alias_vars ();

  while (currdecl)
    {
      if (TREE_CODE (currdecl) == VAR_DECL)
	create_alias_var (currdecl);
      currdecl = TREE_CHAIN (currdecl);
    }
  create_fun_alias_var (current_function_decl, 1);
  /* For debugging, disable the on-the-fly variable creation, 
     and reenable this. */
  /*  walk_tree_without_duplicates (&DECL_SAVED_TREE (current_function_decl),
      find_func_decls, NULL);*/
  walk_tree_without_duplicates (&DECL_SAVED_TREE (current_function_decl),
				find_func_aliases, NULL);
  fprintf (stderr, "\nPoints to sets for function %s:\n",
	   IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));
  for (i = 0; i < VARRAY_ACTIVE_SIZE (alias_vars); i++)
     display_points_to_set_helper (VARRAY_GENERIC_PTR (alias_vars, i));
  for (i = 0; i < VARRAY_ACTIVE_SIZE (local_alias_vars); i++)
     splay_tree_remove (alias_annot, (splay_tree_key) VARRAY_GENERIC_PTR (local_alias_vars, i));
  for (i = 0; i < VARRAY_ACTIVE_SIZE (local_alias_varnums); i ++)
    VARRAY_GENERIC_PTR (alias_vars, VARRAY_INT (local_alias_varnums, i)) = NULL;
  if (!steen_alias_ops->ip)
    {
      splay_tree_delete (alias_annot);
      VARRAY_CLEAR (alias_vars);
      VARRAY_CLEAR (local_alias_vars);
      VARRAY_CLEAR (local_alias_varnums);
    }
  splay_tree_delete (varmap);
}
void
init_alias_vars ()
{
  init_alias_type ();
  VARRAY_GENERIC_PTR_INIT (local_alias_vars, 10, "Local alias vars");
  VARRAY_INT_INIT (local_alias_varnums, 10, "Local alias varnums");
  if (!steen_alias_ops->ip || alias_vars == NULL)
    VARRAY_GENERIC_PTR_INIT (alias_vars, 10, "Alias vars");
  if (!steen_alias_ops->ip || alias_annot == NULL)
    alias_annot = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
  if (!steen_alias_ops->ip || varmap == NULL)
    varmap = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);

}

tree
get_virtual_var (ECR ecr)
{
  splay_tree_node result = splay_tree_lookup (varmap, (splay_tree_key) ecr);
  tree var;
  var = (tree) result->value;
  if (!result->value)
    {
      char id[500];
      sprintf (id, "ecr%d", ECR_get_setid (ecr));
      var = create_tmp_alias_var (void_type_node, id);
      splay_tree_insert (varmap, (splay_tree_key) ecr, (splay_tree_value) var);
    }
  return var;
}
			    
	    
	     
#include "gt-tree-alias-common.h"
