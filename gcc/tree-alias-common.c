/* Tree based points-to analysis
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
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
#include "coretypes.h"
#include "tm.h"
#include "ggc.h"
#include "tree-alias-type.h"
#include "tree-alias-common.h"
/* If we have andersen's points-to analysis, include it. */
#ifdef HAVE_BANSHEE
#include "tree-alias-ander.h"
#endif
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
#include "c-common.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "ssa.h"
#include "varray.h"
#include "c-tree.h"
#include "tree-simple.h"
#include "hashtab.h"
#include "splay-tree.h"

/**
   @file tree-alias-common.c
   This file contains the implementation of the common parts of the
   tree points-to analysis infrastructure. 
*/
#define FIELD_BASED 0

/**
   @brief Array of all created alias_typevars.
   
   @note Should contain all the alias_typevars we wanted marked
   during GC. 
*/
static GTY((param_is (union alias_typevar_def))) varray_type alias_vars = NULL;
struct tree_alias_ops *current_alias_ops;
/**
   @brief  Array of local alias_typevars.

   @note Should contain all the alias_typevars that are local to
   this function.  We delete these from alias_vars before
   collection. 
*/
static varray_type local_alias_vars;
static varray_type local_alias_varnums;

static alias_typevar get_alias_var_decl PARAMS ((tree));
static alias_typevar get_alias_var PARAMS ((tree));
static tree find_func_aliases PARAMS ((tree *, int *, void *));
#if 0
static tree find_func_decls PARAMS ((tree *, int *, void *));
#endif
static alias_typevar create_fun_alias_var_ptf PARAMS ((tree, tree));
static alias_typevar create_fun_alias_var PARAMS ((tree, int));
static alias_typevar create_alias_var PARAMS ((tree));
static void intra_function_call PARAMS ((varray_type));
static hashval_t annot_hash PARAMS ((const PTR));
static int annot_eq PARAMS ((const PTR, const PTR));
static void get_values_from_constructor PARAMS ((tree, varray_type *));

/**
   @brief Alias annotation hash table entry.
   
   Used in the annotation hash table to map trees to their
   alias_typevar's.
*/
struct alias_annot_entry GTY(())
{
  tree key;
  alias_typevar value;
};

/**
   @brief Alias annotation equality.
   @param pentry Entry in the hash table.
   @param pdata  Entry we gave the function calling this.
   @return 1 if \a pentry is equal to \a pdata, 0 if not.
*/
static int 
annot_eq (pentry, pdata)
     const PTR pentry;
     const PTR pdata;
{
  struct alias_annot_entry *entry = (struct alias_annot_entry *)pentry;
  struct alias_annot_entry *data = (struct alias_annot_entry *)pdata;
  
  return entry->key == data->key;
}

/**
   @brief Alias annotation hash function.
   @param pentry Entry to hash.
   @return Hash value of \a pentry.
*/
static hashval_t
annot_hash (pentry)
     const PTR pentry;
{
  struct alias_annot_entry *entry = (struct alias_annot_entry *) pentry;
  return htab_hash_pointer (entry->key);
}

/**
   @brief Alias annotation hash table.
   
   Maps vars to alias_typevars.
*/
static GTY ((param_is (struct alias_annot_entry))) htab_t alias_annot;

/**
   @brief Get the alias_type for a *_DECL.
   @param decl *_DECL to get the alias_typevar for.
   @return The alias_typevar for \a decl.

   @note Creates the alias_typevar if it does not exist already. Also
   handles FUNCTION_DECL properly.
*/
static alias_typevar
get_alias_var_decl (decl)
     tree decl;
{
  struct alias_annot_entry *entry;
  struct alias_annot_entry lookup;
  
  alias_typevar newvar;
  
  lookup.key = decl;
  entry = htab_find (alias_annot, &lookup);
  if (entry != NULL && entry->value != 0)
    return entry->value;
  
  /* For debugging, remove this whole if block, and re-enable the 
     find_func_decls call. */
  if (TREE_CODE (decl) == FUNCTION_DECL)
    newvar = create_fun_alias_var  (decl, 0);
  else
    {
      if (decl_function_context (decl) == NULL && decl != global_var)
	return get_alias_var (global_var);
      else
	newvar = create_alias_var (decl);
    }
  
  if (!TREE_PUBLIC (decl))
    {
      VARRAY_PUSH_INT (local_alias_varnums, 
		       VARRAY_ACTIVE_SIZE (alias_vars) - 1);
      VARRAY_PUSH_GENERIC_PTR (local_alias_vars, decl);
    }
  return newvar;
}

/**
   @brief Get the alias_typevar for an expression.
   @param expr Expression we want the alias_typevar of. 
   @return The alias_typevar for \a expr.

   @note Expects to only be handed a RHS or LHS, not a MODIFY_EXPR.
*/
static alias_typevar
get_alias_var (expr)
     tree expr;
{
  alias_typevar tvar;
  struct alias_annot_entry entry;
  struct alias_annot_entry *result;
  
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
  entry.key = expr;
  result = htab_find (alias_annot, &entry);
  if (result != NULL && result->value != 0)
    {
      tvar = result->value;
      htab_remove_elt (alias_annot, &entry);
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
#if FIELD_BASED
	return get_alias_var (TREE_OPERAND (expr, 1));
#else
        /* Find the first non-component ref, and return its alias variable. */
	tree p;
	for (p = expr; TREE_CODE (p) == COMPONENT_REF;
	     p = TREE_OPERAND (p, 0));
	return get_alias_var (p);
#endif
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
	  = current_alias_ops->add_var (current_alias_ops,
					create_tmp_alias_var
					(void_type_node,
					 "aliastmp"));
	return tempvar;
      }
    }
}

/**
   @param args Arguments that were passed to the function call.
   
   Handles function calls in intra-procedural mode, by making
   conservative assumptions about what happens to arguments to the
   call.
*/
static void
intra_function_call (args)
     varray_type args;
{
  size_t l = VARRAY_ACTIVE_SIZE (args);
  size_t i;
  alias_typevar av = get_alias_var (global_var);
  
  /* We assume that an actual parameter can point to any global. */
  for (i = 0; i < l; i++)
    {
      alias_typevar argav = VARRAY_GENERIC_PTR (args, i);
       if (!POINTER_TYPE_P (TREE_TYPE (ALIAS_TVAR_DECL (argav))))
         continue;
      /* Restricted pointers can't be aliased with other
	 restricted pointers. */	      
      if (!TYPE_RESTRICT (TREE_TYPE (ALIAS_TVAR_DECL (argav))) 
	  || !TYPE_RESTRICT (TREE_TYPE (ALIAS_TVAR_DECL (av))))
	{
	  alias_typevar tempvar;
	  tempvar = current_alias_ops->add_var (current_alias_ops,
						create_tmp_alias_var
						(void_type_node,
						 "aliastmp"));
	  /* Arguments can alias globals, and whatever they point to
	     can point to a global as well. */
	  current_alias_ops->addr_assign (current_alias_ops, argav, av);
	  current_alias_ops->addr_assign (current_alias_ops, tempvar, av);
	  current_alias_ops->assign_ptr (current_alias_ops, argav, tempvar);

	}
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
	  if (!TYPE_RESTRICT (TREE_TYPE (ALIAS_TVAR_DECL (argi))) 
	      || !TYPE_RESTRICT (TREE_TYPE (ALIAS_TVAR_DECL (argj))))
	    current_alias_ops->simple_assign (current_alias_ops, argi, argj);
	}
    }
}

/** @brief Put all pointers in a constructor in an array.
 */
static void
get_values_from_constructor (constructor, vals)
  tree constructor;
  varray_type *vals;
{
#if FIELD_BASED
 #error "Don't know how to do this at the moment"
#else
  tree elt_list;
  switch (TREE_CODE (constructor))
    {
    case CONSTRUCTOR:
      {
	for (elt_list = CONSTRUCTOR_ELTS (constructor); 
	     elt_list; 
	     elt_list = TREE_CHAIN (elt_list))
	  {
	    tree value = TREE_VALUE (elt_list);
	    if (TREE_CODE (value) == TREE_LIST 
		|| TREE_CODE (value) == CONSTRUCTOR)
	      {
		get_values_from_constructor (value, vals);
	      }
	    else
	      {
		alias_typevar aav;
		aav = get_alias_var (value);
		if (aav)
		  VARRAY_PUSH_GENERIC_PTR (*vals, aav);
	      }
	  }
      }
      break;
    case TREE_LIST:
      for (elt_list = constructor; 
	   elt_list; 
	   elt_list = TREE_CHAIN (elt_list))
	{
	  get_values_from_constructor (TREE_VALUE (elt_list), vals);
	}
      break;
    default:
      abort();      
    }
#endif
}
/**
   @brief Tree walker that is the heart of the aliasing
   infrastructure.
   @param tp Pointer to the current tree.
   @param walk_subtrees Whether to continue traversing subtrees or
   not.
   @param data Not used.
   @return NULL_TREE to keep going, anything else to stop.
   
   This function is the main part of the aliasing infrastructure. It
   walks the trees, calling the approriate alias analyzer functions to process
   various statements.
*/
static tree
find_func_aliases (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees;
     void *data ATTRIBUTE_UNUSED;
{
  tree stp = *tp;
  
  if (TREE_CODE (stp) == SCOPE_STMT)
    {
      *walk_subtrees = 0;
      return NULL_TREE;
    }

  if (is_simple_modify_expr (stp) 
      || (TREE_CODE (stp) == VAR_DECL 
	  && DECL_INITIAL (stp) != NULL_TREE ))
    {
      tree op0, op1;
      alias_typevar lhsAV = NULL;
      alias_typevar rhsAV = NULL;
      
      if (TREE_CODE (stp) == VAR_DECL)
	{
	  op0 = stp;
	  op1 = DECL_INITIAL (stp);
	}
      else
	{
	  op0 = TREE_OPERAND (stp, 0);
	  op1 = TREE_OPERAND (stp, 1);
	}
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
		current_alias_ops->simple_assign (current_alias_ops, lhsAV,
						  rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* x = (cast) y */
	  else if (is_simple_cast (op1))
	    {
	      if (rhsAV != NULL)
		current_alias_ops->simple_assign (current_alias_ops, lhsAV,
						  rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* x = *y or x = foo->y */
	  else if (TREE_CODE (op1) == INDIRECT_REF
		   && is_simple_varname (TREE_OPERAND (op1, 0)))
	    {
	      if (rhsAV != NULL)
		current_alias_ops->ptr_assign (current_alias_ops, lhsAV, 
					       rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* x = &y = x = &foo.y */
	  else if (TREE_CODE (op1) == ADDR_EXPR
		   && is_simple_varname (TREE_OPERAND (op1, 0)))
	    {
	      if (rhsAV != NULL)
		current_alias_ops->addr_assign (current_alias_ops, lhsAV, 
						rhsAV);
	      *walk_subtrees = 0;
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
		  if (TREE_CODE (callop0) == ADDR_EXPR)
		    create_fun_alias_var (TREE_OPERAND (callop0, 0), 0);
		  
		  /* NORETURN functions have no effect on aliasing. */
		  if (!(call_expr_flags (op1) & ECF_NORETURN))
		    if (current_alias_ops->function_call (current_alias_ops, lhsAV, 
							  get_alias_var (callop0),
							  args))
		      if (!current_alias_ops->ip)
			intra_function_call (args);
		  *walk_subtrees = 0;
		}

	    }
	  /* x = op (...) */
	  else	  
	    {
	      if (TREE_CODE (op1) == CONSTRUCTOR)
	        {
		  varray_type ops;

		  VARRAY_GENERIC_PTR_INIT (ops, 1, "Operands");
		  get_values_from_constructor (op1, &ops);
		  current_alias_ops->op_assign (current_alias_ops, lhsAV, 
						ops, op1);
		  *walk_subtrees = 0;
		}
	      else
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
		      current_alias_ops->op_assign (current_alias_ops, lhsAV, 
						    ops, op1);
		      *walk_subtrees = 0;
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
		current_alias_ops->simple_assign (current_alias_ops, lhsAV,
						  rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* *x.f = y or *x->f = y */
	  else if (TREE_CODE (op0) == INDIRECT_REF
		   && TREE_CODE (TREE_OPERAND (op0, 0)) == COMPONENT_REF)
	    {
	      if (rhsAV != NULL)
		current_alias_ops->assign_ptr (current_alias_ops, lhsAV, 
					       rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* *x = &y */
	  else if (TREE_CODE (op0) == INDIRECT_REF
		   && TREE_CODE (op1) == ADDR_EXPR)
	    {
	      /* This becomes temp = &y and *x = temp . */
	      alias_typevar tempvar;
	      tempvar = current_alias_ops->add_var (current_alias_ops,
						    create_tmp_alias_var
						    (void_type_node,
						     "aliastmp"));   
	      current_alias_ops->addr_assign (current_alias_ops, tempvar, 
					      rhsAV);
	      current_alias_ops->assign_ptr (current_alias_ops, lhsAV, 
					     tempvar);
	      *walk_subtrees = 0;
	    }
	  
	  /* *x = *y */
	  else if (TREE_CODE (op0) == INDIRECT_REF
		   && TREE_CODE (op1) == INDIRECT_REF)
	    {
	      /* This becomes temp = *y and *x = temp . */
	      alias_typevar tempvar;
	      tempvar = current_alias_ops->add_var (current_alias_ops,
						    create_tmp_alias_var
						    (void_type_node,
						     "aliastmp"));   
	      current_alias_ops->ptr_assign (current_alias_ops, tempvar, 
					     rhsAV);
	      current_alias_ops->assign_ptr (current_alias_ops, lhsAV,
					     tempvar);
	      *walk_subtrees = 0;
	    }
	  
	  /* *x = (cast) y */
	  else if (TREE_CODE (op0) == INDIRECT_REF
		   && is_simple_cast (op1))
	    {
	      /* This becomes temp = (cast) y and  *x = temp. */
	      alias_typevar tempvar;
	      tempvar = current_alias_ops->add_var (current_alias_ops,
						    create_tmp_alias_var
						    (void_type_node,
						     "aliastmp"));
	      
	      current_alias_ops->simple_assign (current_alias_ops, tempvar, 
						rhsAV);
	      current_alias_ops->assign_ptr (current_alias_ops, lhsAV, 
					     tempvar);
	      *walk_subtrees = 0;
	    }
	  /* *x = <something else> */
	  else
	    {
	      if (rhsAV != NULL)
		current_alias_ops->assign_ptr (current_alias_ops, lhsAV,
					       rhsAV);
	      *walk_subtrees = 0;
	    }
	}
    }
  /* Calls without return values. */
  else if (is_simple_call_expr (stp))
    {
      varray_type args;
      tree arg;
      VARRAY_GENERIC_PTR_INIT (args, 1, "Arguments");
      for (arg = TREE_OPERAND (stp, 1); arg; arg = TREE_CHAIN (arg))
	{
	  alias_typevar aav = get_alias_var (TREE_VALUE (arg));
	  if (aav)
	    VARRAY_PUSH_GENERIC_PTR (args, aav);
	}
      if (TREE_CODE (TREE_OPERAND (stp, 0)) == ADDR_EXPR)
	create_fun_alias_var (TREE_OPERAND (TREE_OPERAND (stp, 0), 0), 0);
      /* NORETURN functions have no effect on aliasing.  */
      if (!(call_expr_flags (stp) & ECF_NORETURN))
	if (current_alias_ops->function_call (current_alias_ops, NULL, 
					      get_alias_var (TREE_OPERAND (stp, 0)),
					      args))
	  if (!current_alias_ops->ip)
	    intra_function_call (args);
      *walk_subtrees = 0;
  } 
  return NULL_TREE;
}

#if 0
static tree
find_func_decls (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data ATTRIBUTE_UNUSED;
{
  if (TREE_CODE (*tp) == DECL_STMT)
    {
      create_alias_var (DECL_STMT_DECL (*tp));
      VARRAY_PUSH_INT (local_alias_varnums,
		       VARRAY_ACTIVE_SIZE (alias_vars) - 1);
      VARRAY_PUSH_GENERIC_PTR (local_alias_vars, DECL_STMT_DECL (*tp));
    }
  return NULL_TREE;
}
#endif

/**
   @brief Create the alias_typevar for a function definition.
   
   Create the alias_typevar for a function definition, it's
   arguments, and it's return value.
   @param decl FUNCTION_DECL for the function.
   @param force If true, we force creation of the alias_typevar,
   regardless of whether one exists already.
   
   @note This includes creation of alias_typevar's for
   -# The function itself.
   -# The arguments.
   -# The locals.
   -# The return value.
*/
static alias_typevar
create_fun_alias_var (decl, force)
     tree decl;
     int force;
{
  alias_typevar avar, retvar;
  tree rdecl;
  varray_type params = NULL;
  struct alias_annot_entry entry, *newentry, *result, **slot;
  
  entry.key = decl;
  result = htab_find (alias_annot, &entry);
  if (!force)
    {
      if (result != NULL && result->value != NULL)
	return result->value;
    }

  VARRAY_GENERIC_PTR_INIT (params, 1, "Arguments");
  if (DECL_ARGUMENTS (decl) != NULL)
    {
      tree arg;
      for (arg = DECL_ARGUMENTS (decl); arg; arg = TREE_CHAIN (arg))
	{
	  alias_typevar tvar = create_alias_var (arg);
	  newentry = ggc_alloc (sizeof (struct alias_annot_entry));
	  newentry->key = arg;
	  newentry->value = tvar;
	  slot = (struct alias_annot_entry **) htab_find_slot (alias_annot, 
							       newentry, 
							       INSERT);
	  *slot = newentry;
	  
	  VARRAY_PUSH_GENERIC_PTR (params, tvar);
	  /* Incoming pointers can point to global_var, unless 
	     either we are interprocedural, or we can do ip on all
	     statics + this function has been defined + it's not an
	     external function. */
	  if (POINTER_TYPE_P (TREE_TYPE (arg)) 
	      && !current_alias_ops->ip
	      /* FIXME: Need to let analyzer decide in partial case. */
	      && (!current_alias_ops->ip_partial
		  || !TREE_STATIC (decl) 
		  || TREE_PUBLIC (decl)))
	    current_alias_ops->addr_assign (current_alias_ops, tvar,
				  	    get_alias_var (global_var));
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
	  
	  /* Incoming pointers can point to global_var, unless 
	     either we are interprocedural, or we can do ip on all
	     statics + this function has been defined + it's not an
	     external function. */
	  if (POINTER_TYPE_P (TREE_TYPE (fakedecl)) 
	      && !current_alias_ops->ip
	      /* FIXME: need to let analyzer decide in partial case. */
	      && (!current_alias_ops->ip_partial
		  || !TREE_STATIC (decl)
		  || TREE_PUBLIC (decl)))
	    current_alias_ops->addr_assign (current_alias_ops, tvar, 
					    get_alias_var (global_var));
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
  retvar = current_alias_ops->add_var (current_alias_ops, rdecl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, retvar);
  avar = current_alias_ops->add_var (current_alias_ops, decl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, avar);

  current_alias_ops->function_def (current_alias_ops, avar, params, retvar);
  newentry = ggc_alloc (sizeof (struct alias_annot_entry));
  newentry->key = decl;
  newentry->value = avar;
  slot = (struct alias_annot_entry **) htab_find_slot (alias_annot, 
						       newentry, 
						       INSERT);
  *slot = newentry;
	  
  /* FIXME: Also, if this is a defining declaration then add the annotation
     to all extern definitions of the function. */
  return avar;
}

/**
   @brief Create the alias_typevar for a ptf.
   
   Create a typevar for a pointer-to-member function, it's arguments,
   and it's return value.
   @param decl The PTF FUNCTION_DECL.
   @param type the PTF type
   @return The alias_typevar for the PTF.

   @note This includes creating alias_typevar's for
   -# The function itself.
   -# The arguments.
   -# The return value.
*/
static alias_typevar
create_fun_alias_var_ptf (decl, type)
     tree decl;
     tree type;
{
  alias_typevar avar, retvar;
  tree rdecl;
  varray_type params = NULL;
  struct alias_annot_entry entry, *result, *newentry, **slot;
  
  entry.key = decl;
  result = htab_find (alias_annot, &entry);
  
  if (result != NULL && result->value != NULL)
    return result->value;

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
  retvar = current_alias_ops->add_var (current_alias_ops, rdecl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, retvar);
  
  avar = current_alias_ops->add_var (current_alias_ops, decl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, avar);
  
  current_alias_ops->function_def (current_alias_ops, avar, params, retvar);
  newentry = ggc_alloc (sizeof (struct alias_annot_entry));
  newentry->key = decl;
  newentry->value = avar;
  slot = (struct alias_annot_entry **) htab_find_slot (alias_annot,
						       newentry, INSERT);
  *slot = newentry;
  
  return avar;
}

/**
   @brief Create the alias_typevar for a *_DECL node.
   @param decl Declaration to create alias_typevar for.
   @return The alias_typevar for \a decl.

   Create the alias_typevar for all types of declaration node.   

   @note Handles creation of alias_typevar's for PTF variables.
*/
static alias_typevar
create_alias_var (decl)
     tree decl;
{
  struct alias_annot_entry entry, *result, *newentry, **slot;
  
  alias_typevar avar;
  
  entry.key = decl;
  result = htab_find (alias_annot, &entry);
  if (result != NULL && result->value != NULL)
    return result->value;
  

  if (TREE_CODE (TREE_TYPE (decl)) == POINTER_TYPE 
      && TREE_CODE (TREE_TYPE (TREE_TYPE (decl))) == FUNCTION_TYPE)
    {
      avar = create_fun_alias_var_ptf (decl, TREE_TYPE (TREE_TYPE (decl)));
    }
  else
    avar = current_alias_ops->add_var (current_alias_ops, decl);

  newentry = ggc_alloc (sizeof (struct alias_annot_entry));
  newentry->key = decl;
  newentry->value = avar;
  slot = (struct alias_annot_entry **)htab_find_slot (alias_annot, newentry, 
						      INSERT);
  *slot = newentry;
  
  VARRAY_PUSH_GENERIC_PTR (alias_vars, avar);

  return avar;
}


/**
   @brief Create points-to sets for a function.
   @param fndecl Function we are creating alias variables for.
   
   @note fndecl might not be current_function_decl, if we are in ip
   mode or ip'ing all statics.
*/
void
create_alias_vars (fndecl)
	tree fndecl;
{
#if 0
  tree currdecl = getdecls ();
  tree fnbody;
#endif
  size_t i;
  
  /* If the #if block printing out the points-to sets is #if 0'd out, the
     compiler will complain i is unused. So use it. */
  i = 0;

#if HAVE_BANSHEE
  if (flag_tree_points_to == PTA_ANDERSEN)
    current_alias_ops = andersen_alias_ops;
  else
   {
     current_alias_ops = NULL;
     flag_tree_points_to = PTA_NONE;
     return;
   }
#else
     current_alias_ops = NULL;
     flag_tree_points_to = PTA_NONE;
     return;
#endif
  
  /* If fndecl is current_function_decl, we are at the top level. */
  if (fndecl  == current_function_decl)
    init_alias_vars ();

  /* Don't force creation unless we are processing the top level
     function decl. */
  create_fun_alias_var (fndecl, fndecl == current_function_decl);
  /* For debugging, disable the on-the-fly variable creation, 
     and reenable this. */
  /*  walk_tree_without_duplicates (&DECL_SAVED_TREE (fndecl),
      find_func_decls, NULL);*/
  walk_tree_without_duplicates (&DECL_SAVED_TREE (fndecl),
				find_func_aliases, NULL);
}

/**
   @brief Delete created points-to sets.
*/
void
delete_alias_vars ()
{
  size_t i;
  struct alias_annot_entry entry;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (local_alias_vars); i++)
    {
      entry.key = VARRAY_GENERIC_PTR (local_alias_vars, i);
      htab_remove_elt (alias_annot, &entry);
    }
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (local_alias_varnums); i ++)
    VARRAY_GENERIC_PTR (alias_vars, VARRAY_INT (local_alias_varnums, i)) = NULL;
  if (!current_alias_ops->ip)
    {
      /*      htab_delete (alias_annot); */
      VARRAY_CLEAR (alias_vars);
      VARRAY_CLEAR (local_alias_vars);
      VARRAY_CLEAR (local_alias_varnums);
    }
  current_alias_ops->cleanup (current_alias_ops);
}

/**
   @brief Initialize points-to analysis machinery.
*/
void
init_alias_vars ()
{
  current_alias_ops->init (current_alias_ops);
  VARRAY_GENERIC_PTR_INIT (local_alias_vars, 10, "Local alias vars");
  VARRAY_INT_INIT (local_alias_varnums, 10, "Local alias varnums");
  if ((!current_alias_ops->ip && !current_alias_ops->ip_partial) || alias_vars == NULL)
    VARRAY_GENERIC_PTR_INIT (alias_vars, 10, "Alias vars");
  if ((!current_alias_ops->ip && !current_alias_ops->ip_partial) || alias_annot == NULL)
    alias_annot = htab_create_ggc (7, annot_hash, annot_eq, NULL);
  
}

/**
   @brief Determine whether two variables may-alias.

   @param ptr Pointer.
   @param var Variable.
   @return true if \a ptr may-alias \a var, false otherwise.
*/
bool
ptr_may_alias_var (ptr, var)
     tree ptr;
     tree var;
{
  struct alias_annot_entry entry, *result;
  alias_typevar ptrtv, vartv;

#if !FIELD_BASED
  ptr = get_base_symbol (ptr);
  var = get_base_symbol (var);
#else
  if (TREE_CODE (ptr) == COMPONENT_REF)
    ptr = TREE_OPERAND (ptr, 1);
  if (TREE_CODE (var) == COMPONENT_REF)
    var = TREE_OPERAND (var, 1);
#endif
  if (ptr == var)
    return true;

  entry.key = ptr;
  result = htab_find (alias_annot, &entry);

  if (!result 
      && decl_function_context (ptr) == NULL)
    {
      entry.key = global_var;
      result = htab_find (alias_annot, &entry);
    }
  
  if (!result && !current_alias_ops->ip && ptr != global_var)
    abort ();

  else if (!result)
    return false;

  ptrtv = result->value;  
  entry.key = var;
  result = htab_find (alias_annot, &entry);

  if (!result 
      && decl_function_context (var) == NULL)
    {
      entry.key = global_var;
      result = htab_find (alias_annot, &entry);
    }
  
  if (!result && !current_alias_ops->ip && var != global_var)
    abort ();
  else if (!result)
    return false;
  
  vartv = result->value;

  if (ptrtv == vartv)
    return true;
  
  return current_alias_ops->may_alias (current_alias_ops, ptrtv, vartv);
  
}

const char *
alias_get_name (t)
     tree t;
{
  const char *name;
  
#if FIELD_BASED
  if (TREE_CODE (t) == FIELD_DECL)
    {
      /* First get the name of the field, then the prefix, then smash them
	 together.  */
      const char *fieldname = IDENTIFIER_POINTER (DECL_NAME (t));
      const char *prefix = alias_get_name (DECL_CONTEXT (t));
      char *smashed;
      size_t neededlen = strlen (fieldname) + strlen (prefix) + 2;
      smashed = ggc_alloc (neededlen);
      sprintf (smashed, "%s.%s", prefix, fieldname);
      name = smashed;
      
    }
  else if (TYPE_P (t))
    {
      if (TYPE_NAME (t) && IDENTIFIER_POINTER (TYPE_NAME (t)))
	name = IDENTIFIER_POINTER (TYPE_NAME (t));
      else
	name = "<unnamed type>";
    }
  else
#endif
    {
      if (TREE_CODE (t) == FUNCTION_DECL)
	name = IDENTIFIER_POINTER (DECL_NAME (t));
      else
    name = get_name (t);
    }

  if (!name)
    return name;

  return name;
}

#include "gt-tree-alias-common.h"
