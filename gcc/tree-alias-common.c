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

/*  This file contains the implementation of the common parts of the
    tree points-to analysis infrastructure.
    
    Overview:
    
    This file contains the points-to analysis driver.  It does two main things:
    1. Keeps track of the PTA data for each variable (IE the data each
    specific PTA implementation wants to keep associated with a
    variable).
    2. Walks the function trees, calling the appropriate functions that
    each PTA implementation has implemented.
    
    In order to speed up PTA queries, the PTA specific data is stored
    in the tree for *_DECL's, in DECL_PTA_TYPEVAR.  This way, we only
    need to use the hash table for non-DECL's.  */
#define FIELD_BASED 0

/*  Array of all created alias_typevars.
    Note that this should contain all the alias_typevars we wanted
    marked during GC.  */
static GTY((param_is (union alias_typevar_def))) varray_type alias_vars = NULL;
struct tree_alias_ops *current_alias_ops;

/*  Array of local (to a function) alias_typevars.
    Note that this should contain all the alias_typevars that are
    local to this function.  We delete these from alias_vars before
    collection.  */
static varray_type local_alias_vars;
static varray_type local_alias_varnums;

static tree currptadecl;
static alias_typevar get_alias_var_decl (tree);
static alias_typevar get_alias_var (tree);
static tree find_func_aliases (tree *, int *, void *);
#if 0
static tree find_func_decls (tree *, int *, void *);
#endif
static alias_typevar create_fun_alias_var_ptf (tree, tree);
static alias_typevar create_fun_alias_var (tree, int);
static alias_typevar create_alias_var (tree);
static void intra_function_call (varray_type);
static hashval_t annot_hash (const PTR);
static int annot_eq (const PTR, const PTR);
static void get_values_from_constructor (tree, varray_type *);
static bool call_may_clobber (tree);

/* Return true if a EXPR, which is a CALL_EXPR, may clobber variables.  */

static bool
call_may_clobber (tree expr)
{
  tree callee;
  int flags;

  if (TREE_CODE (expr) != CALL_EXPR)
    return false;

  callee = get_callee_fndecl (expr);
  flags = (callee) ? flags_from_decl_or_type (callee) : 0;
  return (! (flags & (ECF_CONST | ECF_PURE | ECF_NORETURN)));
}


/*  Alias annotation hash table entry.
    Used in the annotation hash table to map trees to their
    alias_typevar's.  */

struct alias_annot_entry GTY(())
{
  tree key;
  alias_typevar value;
};

/*  Alias annotation equality function. PENTRY is the hash table
    entry, PDATA is the entry we gave the function calling this.
    Return 1 if PENTRY is equal to PDATA.  */

static int
annot_eq (const PTR pentry, const PTR pdata)
{
  struct alias_annot_entry *entry = (struct alias_annot_entry *)pentry;
  struct alias_annot_entry *data = (struct alias_annot_entry *)pdata;

  return entry->key == data->key;
}

/*  Alias annotation hash function.  Hash PENTRY and return a value.  */

static hashval_t
annot_hash (const PTR pentry)
{
  struct alias_annot_entry *entry = (struct alias_annot_entry *) pentry;
  return htab_hash_pointer (entry->key);
}

/*  Alias annotation hash table.  Maps vars to alias_typevars.  */

static GTY ((param_is (struct alias_annot_entry))) htab_t alias_annot;

/*  Get the alias_typevar for DECL.  
    Creates the alias_typevar if it does not exist already. Also
    handles FUNCTION_DECL properly.  */

static alias_typevar
get_alias_var_decl (tree decl)
{
  alias_typevar newvar;

  if (DECL_P (decl))
    {
      if (DECL_PTA_TYPEVAR (decl))
	return DECL_PTA_TYPEVAR (decl);
    }
  /* For debugging, remove this whole if block, and re-enable the
     find_func_decls call. */
  if (TREE_CODE (decl) == FUNCTION_DECL)
    newvar = create_fun_alias_var  (decl, 0);
  else
    {
      newvar = create_alias_var (decl);
      /* Assign globals to global var for purposes of intraprocedural
	 analysis. */
      if ((DECL_CONTEXT (decl) == NULL || TREE_PUBLIC (decl)
	   || decl_function_context (decl) == NULL) && decl != global_var)
	current_alias_ops->addr_assign (current_alias_ops, 
					get_alias_var (global_var), newvar);
    }

  if (!current_alias_ops->ip)
    {
      if (!current_alias_ops->ip_partial
	  || (TREE_CODE (decl) != FUNCTION_DECL
	      && TREE_CODE (decl)!= PARM_DECL))
	{
	  VARRAY_PUSH_INT (local_alias_varnums,
			   VARRAY_ACTIVE_SIZE (alias_vars) - 1);
	  VARRAY_PUSH_TREE (local_alias_vars, decl);
	}
    }
  return newvar;
}

/* Get the alias_typevar for an expression EXPR.
   Note that this function expects to only be handed a RHS or LHS, not
   a MODIFY_EXPR.  */

static alias_typevar
get_alias_var (tree expr)
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
      /* If it's a ref or cast or conversion of something, get the
         alias var of the something. */
      return get_alias_var (TREE_OPERAND (expr, 0));
      break;

    default:
      {
	/* Otherwise, we need a new temporary alias variable for this
	   expression. */
	tree temp = create_tmp_alias_var (void_type_node, "aliastmp");
	alias_typevar tempvar;
	tempvar = current_alias_ops->add_var (current_alias_ops,
					      temp);
	return tempvar;
      }
    }
}

/*  Perform conservative aliasing for an intraprocedural mode function
    call.  ARGS are the arguments that were passed to that function
    call.  */

static void
intra_function_call (varray_type args)
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
	  /* Arguments can alias globals, and whatever they point to
	     can point to a global as well. */
	  if (!TREE_READONLY (ALIAS_TVAR_DECL (argav)))
	    current_alias_ops->simple_assign (current_alias_ops, argav, av);
	}
    }
  /* We assume assignments among the actual parameters. */
  for (i = 0; i < l; i++)
    {
      alias_typevar argi = VARRAY_GENERIC_PTR (args, i);
      size_t j;
      if (!POINTER_TYPE_P (TREE_TYPE (ALIAS_TVAR_DECL (argi))))
	continue;
      for (j = 0; j < l; j++)
	{
	  alias_typevar argj;
	  if (i == j)
	    continue;
	  argj = VARRAY_GENERIC_PTR (args, j);

	  /* Non pointers can't point to pointers.  */
	  if (!POINTER_TYPE_P (TREE_TYPE (ALIAS_TVAR_DECL (argj))))
	    continue;
	  /* Restricted pointers can't be aliased with other
	     restricted pointers. */
	  if (!TYPE_RESTRICT (TREE_TYPE (ALIAS_TVAR_DECL (argi)))
	      || !TYPE_RESTRICT (TREE_TYPE (ALIAS_TVAR_DECL (argj))))
	    /* Do a bit of TBAA to avoid pointless assignments. */
	    if (alias_sets_conflict_p (get_alias_set (ALIAS_TVAR_DECL (argi)),
				       get_alias_set (ALIAS_TVAR_DECL (argj))))	      
	      current_alias_ops->simple_assign (current_alias_ops, argi, argj);
	}
    }
}

/* Put all pointers in a constructor in an array.  */

static void
get_values_from_constructor (tree constructor, varray_type *vals)
{
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
}

/*  Tree walker that is the heart of the aliasing infrastructure.
    TP is a pointer to the current tree.
    WALK_SUBTREES specifies whether to continue traversing subtrees or
    not.
    Returns NULL_TREE when we should stop.
    
    This function is the main part of the aliasing infrastructure. It
    walks the trees, calling the appropriate alias analyzer functions to process
    various statements.  */

static tree
find_func_aliases (tree *tp, int *walk_subtrees, void *data ATTRIBUTE_UNUSED)
{
  tree stp = *tp;

  if (TREE_CODE (stp) == SCOPE_STMT)
    {
      *walk_subtrees = 0;
      return NULL_TREE;
    }

  if (TREE_CODE (stp) == MODIFY_EXPR
      || (DECL_P (stp) && DECL_INITIAL (stp) != NULL_TREE ))
    {
      tree op0, op1;
      alias_typevar lhsAV = NULL;
      alias_typevar rhsAV = NULL;

      if (DECL_P (stp))
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
      if (is_gimple_variable (op0))
	{
	  /* x = y or x = foo.y */
	  if (is_gimple_variable (op1))
	    {
	      if (rhsAV != NULL)
		current_alias_ops->simple_assign (current_alias_ops, lhsAV,
						  rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* x = (cast) y */
	  else if (is_gimple_cast (op1))
	    {
	      if (rhsAV != NULL)
		current_alias_ops->simple_assign (current_alias_ops, lhsAV,
						  rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* x = *y or x = foo->y */
	  else if (TREE_CODE (op1) == INDIRECT_REF 
		   || TREE_CODE (op1) == ARRAY_REF)
	    {
	      if (rhsAV != NULL)
		current_alias_ops->ptr_assign (current_alias_ops, lhsAV,
					       rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* x = &y = x = &foo.y */
	  else if (TREE_CODE (op1) == ADDR_EXPR)
	    {
	      if (rhsAV != NULL)
		current_alias_ops->addr_assign (current_alias_ops, lhsAV,
						rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* x = func(...) */
	  else if (TREE_CODE (op1) == CALL_EXPR)
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
		  /* NORETURN and CONST functions have no effect on aliasing. */
		  if (call_may_clobber (op1))
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
        *walk_subtrees = 0;
		    break;
		  }
	    }
	}
      /* *x = <something> */
      else
	{
	  /* x.f = y  or x->f = y */
	  if (TREE_CODE (op0) == COMPONENT_REF 
	      && is_gimple_variable (op1))
	    {
	      if (rhsAV != NULL)
		current_alias_ops->simple_assign (current_alias_ops, lhsAV,
						  rhsAV);
	      *walk_subtrees = 0;
	    }
	  else if (TREE_CODE (op0) == COMPONENT_REF 
		   && TREE_CODE (op1) == ADDR_EXPR)
	    {
	      if (rhsAV != NULL)
		current_alias_ops->addr_assign (current_alias_ops, lhsAV,
						rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* *x.f = y or *x->f = y */
	  else if ((TREE_CODE (op0) == INDIRECT_REF 
		    || TREE_CODE (op0) == ARRAY_REF)
		   && TREE_CODE (TREE_OPERAND (op0, 0)) == COMPONENT_REF
		   && is_gimple_variable (op1))
	    {
	      if (rhsAV != NULL)
		current_alias_ops->assign_ptr (current_alias_ops, lhsAV,
					       rhsAV);
	      *walk_subtrees = 0;
	    }
	  /* *x = &y */
	  else if ((TREE_CODE (op0) == INDIRECT_REF
		    || TREE_CODE (op0) == ARRAY_REF)
		   && TREE_CODE (op1) == ADDR_EXPR)
	    {
	      /* This becomes temp = &y and *x = temp . */
	      alias_typevar tempvar;
	      tree temp = create_tmp_alias_var (void_type_node, "aliastmp");
	      tempvar = current_alias_ops->add_var (current_alias_ops, temp);
	      current_alias_ops->addr_assign (current_alias_ops, tempvar,
					      rhsAV);
	      current_alias_ops->assign_ptr (current_alias_ops, lhsAV,
					     tempvar);
	      *walk_subtrees = 0;
	    }

	  /* *x = *y */
	  else if ((TREE_CODE (op0) == INDIRECT_REF 
		    || TREE_CODE (op0) == ARRAY_REF)
		   && (TREE_CODE (op1) == INDIRECT_REF
		       || TREE_CODE (op1) == ARRAY_REF))
	    {
	      /* This becomes temp = *y and *x = temp . */
	      alias_typevar tempvar;
	      tree temp;
	      temp = create_tmp_alias_var (void_type_node, "aliastmp");
	      tempvar = current_alias_ops->add_var (current_alias_ops, temp);
	      current_alias_ops->ptr_assign (current_alias_ops, tempvar,
					     rhsAV);
	      current_alias_ops->assign_ptr (current_alias_ops, lhsAV,
					     tempvar);
	      *walk_subtrees = 0;
	    }

	  /* *x = (cast) y */
	  else if ((TREE_CODE (op0) == INDIRECT_REF 
		    || TREE_CODE (op0) == ARRAY_REF)
		   && is_gimple_cast (op1))
	    {
	      if (rhsAV != NULL)
		{
		  /* This becomes temp = (cast) y and  *x = temp. */
		  alias_typevar tempvar;
		  tree temp;
		  temp = create_tmp_alias_var (void_type_node, "aliastmp");
		  tempvar = current_alias_ops->add_var (current_alias_ops,
							temp);
		  current_alias_ops->simple_assign (current_alias_ops,
						    tempvar, rhsAV);
		  current_alias_ops->assign_ptr (current_alias_ops, lhsAV,
						 tempvar);
		}
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
  else if (TREE_CODE (stp) == CALL_EXPR)
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
      /* NORETURN and CONST functions have no effect on aliasing.  */
      if (call_may_clobber (stp))
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
find_func_decls (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		 void *data ATTRIBUTE_UNUSED)
{
  if (TREE_CODE (*tp) == DECL_STMT)
    {
      create_alias_var (DECL_STMT_DECL (*tp));
      VARRAY_PUSH_INT (local_alias_varnums,
		       VARRAY_ACTIVE_SIZE (alias_vars) - 1);
      VARRAY_PUSH_TREE (local_alias_vars, DECL_STMT_DECL (*tp));
    }
  return NULL_TREE;
}
#endif

/*  Create the alias_typevar for a function definition DECL, it's
    arguments, and it's return value. If FORCE is true, we force 
    creation of the alias_typevar, regardless of whether one exists already.
    
    This includes creation of alias_typevar's for
    - The function itself.
    - The arguments.
    - The return value.  */

static alias_typevar
create_fun_alias_var (tree decl, int force)
{
  alias_typevar avar, retvar;
  tree rdecl;
  varray_type params = NULL;

  if (!force)
    {
      if (DECL_PTA_TYPEVAR (decl))
        return DECL_PTA_TYPEVAR (decl);
    }

  VARRAY_GENERIC_PTR_INIT (params, 1, "Arguments");
  if (DECL_ARGUMENTS (decl) != NULL)
    {
      tree arg;
      for (arg = DECL_ARGUMENTS (decl); arg; arg = TREE_CHAIN (arg))
	{
	  alias_typevar tvar = get_alias_var (arg);
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
	  alias_typevar tvar;
	  tvar = get_alias_var (fakedecl);
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
      alias_typevar fakevar;
      fakevar = get_alias_var (fakedecl);
      VARRAY_PUSH_GENERIC_PTR (params, fakevar);
    }

  if (!DECL_RESULT (decl))
    {
      rdecl = create_tmp_alias_var (TREE_TYPE (TREE_TYPE (decl)), "_rv_");
      retvar = current_alias_ops->add_var (current_alias_ops, rdecl);
    }
  else
    {
    retvar = current_alias_ops->add_var (current_alias_ops,
					 DECL_RESULT (decl));
    }
  VARRAY_PUSH_GENERIC_PTR (alias_vars, retvar);
  avar = current_alias_ops->add_var (current_alias_ops, decl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, avar);

  current_alias_ops->function_def (current_alias_ops, avar, params, retvar);
  DECL_PTA_TYPEVAR (decl) = avar;

  /* FIXME: Also, if this is a defining declaration then add the annotation
     to all extern definitions of the function. */
  return avar;
}

/*  Create a typevar for a pointer-to-member function DECL of type TYPE, it's arguments,
    and it's return value.
    Returns the alias_typevar for the PTF.
    
    This includes creating alias_typevar's for
    - The function itself.
    - The arguments.
    - The return value.  */

static alias_typevar
create_fun_alias_var_ptf (tree decl, tree type)
{
  alias_typevar avar, retvar;
  tree rdecl;
  varray_type params = NULL;

  if (DECL_PTA_TYPEVAR (decl))
    return DECL_PTA_TYPEVAR (decl);

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
	  alias_typevar tvar;
	  tvar = get_alias_var (fakedecl);
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
      alias_typevar fakevar;
      fakevar = get_alias_var (fakedecl);
      VARRAY_PUSH_GENERIC_PTR (params, fakevar);
    }

  rdecl = create_tmp_alias_var (TREE_TYPE (type), "_rv_");
  retvar = current_alias_ops->add_var (current_alias_ops, rdecl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, retvar);

  avar = current_alias_ops->add_var (current_alias_ops, decl);
  VARRAY_PUSH_GENERIC_PTR (alias_vars, avar);

  current_alias_ops->function_def (current_alias_ops, avar, params, retvar);
  DECL_PTA_TYPEVAR (decl) = avar;

  return avar;
}

/*  Create the alias_typevar for a *_DECL node DECL.
    Returns the alias_typevar for DECL.
    
    This function also handles creation of alias_typevar's for PTF 
    variables.  */

static alias_typevar
create_alias_var (tree decl)
{
  struct alias_annot_entry entry, *result, *newentry, **slot;

  alias_typevar avar;
  if (DECL_P (decl))
    {
      if (DECL_PTA_TYPEVAR (decl))
	return DECL_PTA_TYPEVAR (decl);
    }
  entry.key = decl;
  result = htab_find (alias_annot, &entry);
  if (result != NULL && result->value != NULL)
    return result->value;


  if (POINTER_TYPE_P (TREE_TYPE (decl))
      && TREE_CODE (TREE_TYPE (TREE_TYPE (decl))) == FUNCTION_TYPE)
    {
      avar = create_fun_alias_var_ptf (decl, TREE_TYPE (TREE_TYPE (decl)));
    }
  else
    avar = current_alias_ops->add_var (current_alias_ops, decl);

  if (DECL_P (decl))
    {
      DECL_PTA_TYPEVAR (decl) = avar;
    }
  else
    {
      newentry = ggc_alloc (sizeof (struct alias_annot_entry));
      newentry->key = decl;
      newentry->value = avar;
      slot =
	(struct alias_annot_entry **)htab_find_slot (alias_annot, newentry,
						     INSERT);
      *slot = newentry;
    }
  VARRAY_PUSH_GENERIC_PTR (alias_vars, avar);

  return avar;
}

/* Create points-to sets for function FNDECL.

   Note that fndecl might not be current_function_decl, if we are in
   ip mode or ip'ing all statics.  */

void
create_alias_vars (tree fndecl)
{
#if 0
  tree currdecl = getdecls ();
  tree fnbody;
#endif
  size_t i;
  currptadecl = fndecl;
  create_global_var ();

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
  if (fndecl == current_function_decl)
    DECL_PTA_TYPEVAR (fndecl) = NULL;
  get_alias_var (fndecl);

  /* For debugging, disable the on-the-fly variable creation,
     and reenable this. */
  /*  walk_tree_without_duplicates (&DECL_SAVED_TREE (fndecl),
      find_func_decls, NULL);*/
  walk_tree_without_duplicates (&DECL_SAVED_TREE (fndecl),
				find_func_aliases, NULL);
  global_var = NULL_TREE;
}

/* Delete created points-to sets.  */

void
delete_alias_vars (void)
{
  size_t i;
  struct alias_annot_entry entry;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (local_alias_vars); i++)
    {
      entry.key = VARRAY_TREE (local_alias_vars, i);
      if (!DECL_P (entry.key))
	{
	  htab_remove_elt (alias_annot, &entry);
	}
      else
	{
	  DECL_PTA_TYPEVAR (entry.key) = NULL;
	}
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

/*  Initialize points-to analysis machinery.  */

void
init_alias_vars (void)
{
  current_alias_ops->init (current_alias_ops);
  VARRAY_TREE_INIT (local_alias_vars, 10, "Local alias vars");
  VARRAY_INT_INIT (local_alias_varnums, 10, "Local alias varnums");
  if ((!current_alias_ops->ip && !current_alias_ops->ip_partial)
      || alias_vars == NULL)
    VARRAY_GENERIC_PTR_INIT (alias_vars, 10, "Alias vars");
  if ((!current_alias_ops->ip && !current_alias_ops->ip_partial)
      || alias_annot == NULL)
    alias_annot = htab_create_ggc (7, annot_hash, annot_eq, NULL);

}

/* Return true if PTR and VAR have the same points-to set.  */

bool
same_points_to_set (tree ptr, tree var)
{
  struct alias_annot_entry entry, *result;
  alias_typevar ptrtv, vartv;
  
#if !FIELD_BASED
#else
  if (TREE_CODE (ptr) == COMPONENT_REF)
    ptr = TREE_OPERAND (ptr, 1);
  if (TREE_CODE (var) == COMPONENT_REF)
    var = TREE_OPERAND (var, 1);
#endif

  if (ptr == var)
    return true;

  if (DECL_P (ptr))
    {
      ptrtv = DECL_PTA_TYPEVAR (ptr);
      if (!ptrtv)
	return false;
    }
  else
    {
      entry.key = ptr;
      result = htab_find (alias_annot, &entry);
      if (!result)
	return false;
      ptrtv = result->value;
    }

  if (DECL_P (var))
    {
      vartv = DECL_PTA_TYPEVAR (var);
      if (!vartv)
	return false;
    }
  else
    {
      entry.key = var;
      result = htab_find (alias_annot, &entry);
      if (!result)
	return false;

      vartv = result->value;
    }
  return current_alias_ops->same_points_to_set (current_alias_ops, vartv, ptrtv);
}

/*  Determine whether two variables (PTR and VAR) may-alias.
    Returns TRUE if PTR may-alias VAR.  */

bool
ptr_may_alias_var (tree ptr, tree var)
{
  struct alias_annot_entry entry, *result;
  alias_typevar ptrtv, vartv;

#if !FIELD_BASED
#else
  if (TREE_CODE (ptr) == COMPONENT_REF)
    ptr = TREE_OPERAND (ptr, 1);
  if (TREE_CODE (var) == COMPONENT_REF)
    var = TREE_OPERAND (var, 1);
#endif

  if (ptr == var)
    return true;

  if (DECL_P (ptr))
    {
      ptrtv = DECL_PTA_TYPEVAR (ptr);
      if (!ptrtv)
	return false;
    }
  else
    {
      entry.key = ptr;
      result = htab_find (alias_annot, &entry);
      if (!result)
	return false;
      ptrtv = result->value;
    }

  if (DECL_P (var))
    {
      vartv = DECL_PTA_TYPEVAR (var);
      if (!vartv)
	return false;
    }
  else
    {
      entry.key = var;
      result = htab_find (alias_annot, &entry);
      if (!result)
	return false;

      vartv = result->value;
    }

  return current_alias_ops->may_alias (current_alias_ops, ptrtv, vartv);
}

#define MASK_POINTER(P)	((unsigned)((unsigned long)(P) & 0xffff))

const char *
alias_get_name (tree t)
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
      else if (TREE_CODE (t) == RESULT_DECL)
	name = "<return value>";
      else
	name = get_name (t);
    }

  if (!name)
    {
      char *namep;
      /* 2 = UF
	 4 = the masked pointer
	 2 = the <> around it
	 1 = the terminator. */
      namep = ggc_alloc (2 + 4 + 2 + 1);
      sprintf (namep, "<UV%x>", MASK_POINTER (t));
      return namep;
    }

  return name;
}

#include "gt-tree-alias-common.h"
