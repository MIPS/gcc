/* Tree based linear points-to analysis
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
#include "tree-optimize.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "ssa.h"
#include "varray.h"
#include "c-tree.h"
#include "tree-simple.h"
#include "hashtab.h"
#include "splay-tree.h"
/* Steensgaard's almost-linear interprocedural points-to analysis.
   This is a flow-insensitive, context insensitive algorithm.  It
   also does not distinguish structure fields, or direction of
   assignments.
   It works through non-standard type inferencing.
   By non-standard types, we do not mean "integer, float", etc. The
   types here represent sets of abstract locations, including
   relations between abstract locations (thus modeling the store).

   We then perform type inferencing, which attempts to infer the
   non-standard types involved in each expression, and get the
   points-to sets as a result (Since the types represent the store
   locations). 
   B. Steengaard. Points-to analysis in almost linear time.  In
   Conference Record of the 23rd Annual ACM Symposium on Principles
   of Programming Languages (POPL '96), pages 32--41, ACM Press,
   January 1996.

  http://citeseer.nj.nec.com/steengaard96pointsto.html.

*/

/* Todo list:
   * Intraprocedural call handling.
   * Create SSA may/must def variables.
   * Reduce memory usage (mainly due to fragmentation, not leakage).
   * Don't pass alias ops as first argument, just have a global 
     "current_alias_ops".
   * Finish post-pass cleanup.
*/
 
static splay_tree alias_annot;
static GTY ((param_is (struct alias_typevar_def))) varray_type alias_vars = NULL;
static varray_type local_alias_vars;
static varray_type local_alias_varnums;
#include "gt-tree-alias-steen.h"
#define STEEN_DEBUG 1
static void steen_simple_assign PARAMS ((struct tree_alias_ops *,
					 alias_typevar, alias_typevar));
static void steen_addr_assign PARAMS ((struct tree_alias_ops *,
				       alias_typevar, alias_typevar));
static void steen_ptr_assign PARAMS ((struct tree_alias_ops *,
				      alias_typevar, alias_typevar));
static void steen_op_assign PARAMS ((struct tree_alias_ops *,
				     alias_typevar, varray_type));
static void steen_heap_assign PARAMS ((struct tree_alias_ops *,
				       alias_typevar));
static void steen_assign_ptr PARAMS ((struct tree_alias_ops *,
				      alias_typevar, alias_typevar));
static void steen_function_def PARAMS ((struct tree_alias_ops *,
					alias_typevar, varray_type,
					alias_typevar));
static void steen_function_call PARAMS ((struct tree_alias_ops *,
					 alias_typevar, alias_typevar,
					 varray_type));
static void steen_init PARAMS ((struct tree_alias_ops *));
static void steen_cleanup PARAMS ((struct tree_alias_ops *));
static alias_typevar steen_add_var PARAMS ((struct tree_alias_ops *, tree));
static alias_typevar steen_add_var_same PARAMS ((struct tree_alias_ops *,
						 tree, alias_typevar));
static alias_typevar get_alias_var_decl PARAMS ((tree));
static alias_typevar get_alias_var PARAMS ((tree));
static tree find_func_aliases PARAMS ((tree *, int *, void *));
static tree find_func_decls PARAMS ((tree *, int *, void *));
static alias_typevar create_fun_alias_var PARAMS ((tree, int));
static alias_typevar create_alias_var PARAMS ((tree));

static struct tree_alias_ops steen_ops = {
  steen_init,
  steen_cleanup,
  steen_add_var,
  steen_add_var_same,
  steen_simple_assign,
  steen_addr_assign,
  steen_ptr_assign,
  steen_op_assign,
  steen_heap_assign,
  steen_assign_ptr,
  steen_function_def,
  steen_function_call,
  0
};
struct tree_alias_ops *steen_alias_ops = &steen_ops;

/* Initialize Steensgaard alias analysis.  
   Currently does nothing.  */
static void
steen_init (ops)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
{
}

/* Cleanup after Steensgaard alias analysis. 
   Currently does nothing.  */
static void
steen_cleanup (ops)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
{
}

/* Add decl to the analyzer, and return a typevar for it.  For
   Steensgaard, we create a new alias typevar for the declaration, and
   return that.  */

static alias_typevar
steen_add_var (ops, decl)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     tree decl;
{
#if STEEN_DEBUG
  fprintf (stderr, "Steen: Adding variable " );
  print_c_node (stderr, decl);
  fprintf (stderr, "\n");
#endif
  return alias_tvar_new (decl);

}

/* Add a variable to the analyzer that is equivalent (as far as
   aliases go) to some existing typevar.  
   For Steensgaard, we just call a function that does this for us.  */
static alias_typevar
steen_add_var_same (ops, decl, tv)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     tree decl;
     alias_typevar tv;
{
#if STEEN_DEBUG
  fprintf (stderr, "Steen: Adding variable " );
  print_c_node (stderr, decl);
  fprintf (stderr, " same as ");
  print_c_node (stderr, tv->decl);
  fprintf (stderr, "\n");
#endif
  return alias_tvar_new_equiv_to (decl, tv);
}

/* Inference for simple assignment (lhs = rhs) */
static void
steen_simple_assign (ops, lhs, rhs)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
     alias_typevar rhs;
{
  alias_type type1, type2;
  ECR tau1, tau2, lambda1, lambda2;

#if STEEN_DEBUG
  fprintf (stderr, "Steen: simple assignment ");
  print_c_node (stderr, lhs->decl);
  fprintf (stderr, " = ");
  print_c_node (stderr, rhs->decl);
  fprintf (stderr, "\n");
#endif

  /* Get the non-standard types involved.  */
  type1 = ECR_get_type (alias_tvar_get_ECR (lhs));
  type2 = ECR_get_type (alias_tvar_get_ECR (rhs));

  /* Conditionally join the locations those types represent, if they
     aren't equivalent.  */
  tau1 = alias_ltype_loc (type1);
  tau2 = alias_ltype_loc (type2);
  if (!ECR_equiv (tau1, tau2))
    ECR_cjoin (tau1, tau2);

  /* Ditto on the functions.  */
  lambda1 = alias_ltype_func (type1);
  lambda2 = alias_ltype_func (type2);
  if (!ECR_equiv (lambda1, lambda2))
    ECR_cjoin (lambda1, lambda2);
}

/* Inference for address assignment (lhs = &addr) */
static void
steen_addr_assign (ops, lhs, addr)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
     alias_typevar addr;
{
  alias_type type1;
  ECR tau1, tau2;

  if (addr == NULL)
    return;

#if STEEN_DEBUG
  fprintf (stderr, "Steen: address assignment ");
  print_c_node (stderr, lhs->decl);
  fprintf (stderr, " = &");
  print_c_node (stderr, addr->decl);
  fprintf (stderr, "\n");
#endif

  /* Get the non-standard type for the lhs. */
  type1 = ECR_get_type (alias_tvar_get_ECR (lhs));

  /* Get the location set for the lhs's non-std type, and if it's not
     equivalent to the ADDR's ECR (rather than that ECR's set of
     locations), join them.  */
  tau1 = alias_ltype_loc (type1);
  tau2 = alias_tvar_get_ECR (addr);
  if (!ECR_equiv (tau1, tau2))
    ECR_join (tau1, tau2);
}


/* Inference for pointer assignment (lhs = *ptr) */
static void
steen_ptr_assign (ops, lhs, ptr)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
     alias_typevar ptr;
{
  alias_type type1, type2;
  ECR tau1, tau2, lambda1;

  if (ptr == NULL)
    return;
#if STEEN_DEBUG
  fprintf (stderr, "Steen: pointer assignment ");
  print_c_node (stderr, lhs->decl);
  fprintf (stderr, " = *");
  print_c_node (stderr, ptr->decl);
  fprintf (stderr, "\n");
#endif

  /* Get the non-standard types for the lhs, and the ptr. */
  type1 = ECR_get_type (alias_tvar_get_ECR (lhs));
  type2 = ECR_get_type (alias_tvar_get_ECR (ptr));

  /* Get the location sets for those types, and the function for the
     lhs.  */
  tau1 = alias_ltype_loc (type1);
  tau2 = alias_ltype_loc (type2);
  lambda1 = alias_ltype_func (type1);

  /* If ptr is BOTTOM, then set ptr's non-std type to type1 (since
     we consider assignments transitive, and we have nothing to
     join).  */
  if (ECR_get_type (tau2) == alias_bottom)
    {
      ECR_set_type (tau2, type1);
    }
  /* Otherwise, get the location set for tau2's type (tau2 is the non-std type
     representing the things type2 could point to), which is the
     location set that could be pointed to by the locations of type2
     (IE location set for things *ptr could point to), and cjoin with
     tau1 it if it's not equivalent.  */
  else
    {
      alias_type type3;
      ECR tau3, lambda3;

      type3 = ECR_get_type (tau2);
      tau3 = alias_ltype_loc (type3);
      if (!ECR_equiv (tau1, tau3))
	ECR_cjoin (tau1, tau3);

      lambda3 = alias_ltype_func (type3);
      if (!ECR_equiv (lambda1, lambda3))
	ECR_cjoin (lambda1, lambda3);
    }
}

/* Inference rule for operations (lhs = operation(operands)) */
static void
steen_op_assign (ops, lhs, operands)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
     varray_type operands;
{
  size_t i;
  ECR ecr, tau1, lambda1;
  alias_type type1;

#if STEEN_DEBUG
  fprintf (stderr, "Steen: op assignment ");
  print_c_node (stderr, lhs->decl);
  fprintf (stderr, " = op(...)");
  fprintf (stderr, "\n");
#endif

  ecr = alias_tvar_get_ECR (lhs);
  type1 = ECR_get_type (ecr);

  tau1 = alias_ltype_loc (type1);
  lambda1 = alias_ltype_func (type1);

  for (i = 0; i < VARRAY_ACTIVE_SIZE (operands); i++)
    {
      alias_typevar tv = VARRAY_GENERIC_PTR (operands, i);
      ECR operand, taui, lambdai;
      alias_type typei;

      if (tv == NULL)
	continue;

      operand = alias_tvar_get_ECR (tv);
      typei = ECR_get_type (operand);
      taui = alias_ltype_loc (typei);
      lambdai = alias_ltype_func (typei);

      if (!ECR_equiv (tau1, taui))
	ECR_cjoin (tau1, taui);
      if (!ECR_equiv (lambda1, lambdai))
	ECR_cjoin (lambda1, lambdai);
    }
}

/* Inference for heap assignment (lhs = alloc) */
static void
steen_heap_assign (ops, lhs)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
{
  alias_type type1;
  ECR tau;
  type1 = ECR_get_type (alias_tvar_get_ECR (lhs));
  tau = alias_ltype_loc (type1);

  if (ECR_get_type (tau) == alias_bottom)
    ECR_set_type (tau, alias_ltype_new ());
}

/* Inference for assignment to a pointer (*ptr = rhs) */
static void
steen_assign_ptr (ops, ptr, rhs)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar ptr;
     alias_typevar rhs;
{
  alias_type type1, type2;
  ECR tau1, tau2, lambda2;

  if (rhs == NULL)
    return;

#if STEEN_DEBUG
  fprintf (stderr, "Steen: assignment to pointer  *");
  print_c_node (stderr, ptr->decl);
  fprintf (stderr, " = ");
  print_c_node (stderr, rhs->decl);
  fprintf (stderr, "\n");
#endif
  type1 = ECR_get_type (alias_tvar_get_ECR (ptr));
  type2 = ECR_get_type (alias_tvar_get_ECR (rhs));

  tau1 = alias_ltype_loc (type1);
  tau2 = alias_ltype_loc (type2);
  lambda2 = alias_ltype_func (type2);

  if (ECR_get_type (tau1) == alias_bottom)
    {
      ECR_set_type (tau1, type2);
    }
  else
    {
      alias_type type3;
      ECR tau3, lambda3;

      type3 = ECR_get_type (tau1);
      tau3 = alias_ltype_loc (type3);
      lambda3 = alias_ltype_func (type3);
      if (!ECR_equiv (tau2, tau3))
	ECR_cjoin (tau3, tau2);
      if (!ECR_equiv (lambda2, lambda3))
	ECR_cjoin (lambda3, lambda2);
    }
}

/* Inference for a function definition. */

static void
steen_function_def (ops, func, params, retval)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar func;
     varray_type params;
     alias_typevar retval;
{
  alias_type type;
  ECR lambda;

  type = ECR_get_type (alias_tvar_get_ECR (func));
  lambda = alias_ltype_func (type);
  /* If we've never seen the function before, it'll be alias_bottom. */
  if (ECR_get_type (lambda) == alias_bottom)
    {
      alias_type ftype = alias_ftype_new ();
      alias_type typea;
      varray_type *args = &ALIAS_FTYPE_ARGUMENTS (ftype);
      size_t l = VARRAY_ACTIVE_SIZE (params);
      size_t i;

      /* Set up the arguments for the new function type. */
      for (i = 0; i < l; i++)
	{
	  alias_type newarg;
	  alias_typevar tv = VARRAY_GENERIC_PTR (params, i);
	  typea = ECR_get_type (alias_tvar_get_ECR (tv));
	  newarg = alias_vtype_new_with_lf (alias_ltype_loc (typea),
					    alias_ltype_func (typea));
	  VARRAY_PUSH_GENERIC_PTR (*args, newarg);
	}
      /* Set up the return value location. */
      typea = ECR_get_type (alias_tvar_get_ECR (retval));
      ALIAS_FTYPE_RETVAL (ftype) =
	alias_vtype_new_with_lf (alias_ltype_loc (typea),
				 alias_ltype_func (typea));
      /* Lastly, set the ECR type to the new function type. */
      ECR_set_type (lambda, ftype);
    }
  else
    {
      varray_type args = ALIAS_FTYPE_ARGUMENTS (ECR_get_type (lambda));
      size_t la = VARRAY_ACTIVE_SIZE (args);
      size_t lp = VARRAY_ACTIVE_SIZE (params);
      size_t i;
      ECR tau1, tau2, lambda1, lambda2, alphar;
      alias_type type1, type2, alpha;
#if STEEN_DEBUG
        fprintf (stderr, "Merging function definitions!\n");
#endif
      /* Number of actual and formal parameters should match */
      if (lp != la)
	abort ();

      for (i = 0; i < lp; i++)
	{
	  type1 = ECR_get_type (alias_tvar_get_ECR ((alias_typevar) VARRAY_GENERIC_PTR (params, i)));
	  type2 = VARRAY_GENERIC_PTR (args, i);

	  tau1 = alias_vtype_loc (type1);
	  tau2 = alias_vtype_loc (type2);
	  if (!ECR_equiv (tau1, tau2))
	    ECR_join (tau1, tau2);

	  lambda1 = alias_vtype_func (type1);
	  lambda2 = alias_vtype_func (type2);
	  if (!ECR_equiv (lambda1, lambda2))
	    ECR_join (lambda1, lambda2);
	}

      alpha = ALIAS_FTYPE_RETVAL (ECR_get_type (lambda));
      alphar = alias_tvar_get_ECR (retval);

      tau1 = alias_vtype_loc (alpha);
      tau2 = alias_vtype_loc (ECR_get_type (alphar));

      if (!ECR_equiv (tau1, tau2))
	ECR_join (tau1, tau2);

      lambda1 = alias_vtype_func (alpha);
      lambda2 = alias_vtype_func (ECR_get_type (alphar));
      if (!ECR_equiv (lambda1, lambda2))
	ECR_join (lambda1, lambda2);
    }
}

/* Inference for a function call assignment */
static void
steen_function_call (ops, lhs, func, args)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
     alias_typevar func;
     varray_type args;
{
  alias_type type, type1;
  ECR lambda;
  varray_type largs;
  size_t l1, la;
  size_t i;

  type = ECR_get_type (alias_tvar_get_ECR (func));
  lambda = alias_ltype_func (type);

  if (ECR_get_type (lambda) == alias_bottom)
    {
      alias_type lam = alias_ftype_new ();

      alias_ftype_add_new_arguments (lam, VARRAY_ACTIVE_SIZE (args));
      ALIAS_FTYPE_RETVAL (lam) = alias_vtype_new ();
      ECR_set_type (lambda, lam);
    }
  largs = ALIAS_FTYPE_ARGUMENTS (ECR_get_type (lambda));
  l1 = VARRAY_ACTIVE_SIZE (largs);
  la = VARRAY_ACTIVE_SIZE (args);
  type1 = NULL;

  if ((l1 <= 0) && (la > 0))
    abort ();

  for (i = 0; i < la; i++)
    {
      alias_type type2;
      ECR tau1, tau2;
      ECR lambda1, lambda2;
      alias_typevar tvar;

      if (i < l1)
	type1 = VARRAY_GENERIC_PTR (largs, i);
      
      tvar = VARRAY_GENERIC_PTR (args, i);

      type2 = ECR_get_type (alias_tvar_get_ECR (tvar));
      

      tau1 = alias_vtype_loc (type1);
      tau2 = alias_ltype_loc (type2);
      if (!ECR_equiv (tau1, tau2))
	ECR_cjoin (tau1, tau2);

      lambda1 = alias_vtype_func (type1);
      lambda2 = alias_ltype_func (type2);
      if (!ECR_equiv (lambda1, lambda2))
	ECR_cjoin (lambda1, lambda2);
    }

  if (lhs != NULL)
    {
      alias_type alpha;
      ECR alphar;
      ECR tau1, tau2;
      ECR lambda1, lambda2;

      alpha = ALIAS_FTYPE_RETVAL (ECR_get_type (lambda));
      alphar = alias_tvar_get_ECR (lhs);

      tau1 = alias_vtype_loc (alpha);
      tau2 = alias_ltype_loc (ECR_get_type (alphar));
      if (!ECR_equiv (tau1, tau2))
	ECR_cjoin (tau1, tau2);

      lambda1 = alias_vtype_func (alpha);
      lambda2 = alias_ltype_func (ECR_get_type (alphar));
      if (!ECR_equiv (lambda1, lambda2))
	ECR_cjoin (lambda1, lambda2);
    }
}

extern int next_color;
void test_assign PARAMS((void));

/* Test Steensgaard points-to alias through a series of simple
   assignments.  */
void
test_assign ()
{
  alias_typevar a, b, c, d;
  varray_type temp;
  init_ggc ();
  /* Simulate variable creation for variables a,b,c,d */
  a = alias_tvar_new (NULL);
  b = alias_tvar_new (NULL);
  c = alias_tvar_new (NULL);
  d = alias_tvar_new (NULL);

  /* a = &b */
  steen_addr_assign (0, a, b);
  temp = alias_tvar_pointsto (a);
  VARRAY_CLEAR (temp);

  /* c = &b */
  steen_addr_assign (0, c, b);
  temp = alias_tvar_pointsto (c);
  VARRAY_CLEAR (temp);

  /* d = a */
  steen_simple_assign (0, d, a);
  temp = alias_tvar_pointsto (d);
  VARRAY_CLEAR (temp);

  /* Now get all the points to sets */
  VARRAY_GENERIC_PTR_INIT (temp, 1, "all points to");
  alias_tvar_allpointsto (d, &temp);
  VARRAY_CLEAR (temp);
  next_color++;
  VARRAY_GENERIC_PTR_INIT (temp, 1, "all points to");
  alias_tvar_allpointsto (c, &temp);
  VARRAY_CLEAR (temp);
  next_color++;
  VARRAY_GENERIC_PTR_INIT (temp, 1, "all points to");
  alias_tvar_allpointsto (b, &temp);
  VARRAY_CLEAR (temp);
  next_color++;
  VARRAY_GENERIC_PTR_INIT (temp, 1, "all points to");
  alias_tvar_allpointsto (a, &temp);
}

static alias_typevar
get_alias_var_decl (decl)
     tree decl;
{
  splay_tree_node node;
  node = splay_tree_lookup (alias_annot, (splay_tree_key) decl);
  if (node != NULL && node->value != 0)
    return (alias_typevar) node->value; 
/* For debugging, remove this, and re-enable the find_func_decls call. */
  return create_alias_var (decl);
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
      STRIP_NOPS (op0);
      op1 = TREE_OPERAND (*tp, 1);
      STRIP_NOPS (op1);

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
		  /* FIXME: If this is the non-interprocedural
		     version, or we don't have the alias info for the
		     called function, we need to handle it.
		     In particular, the actual arguments can alias
		     each other, and could take the address of any
		     global. */

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
      /* FIXME: Same caveats as above for calls to functions we have no
	alias variables for. */

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
#if 0
  if (node)
    {
	    ECR tau1, tau2;
	    tau1 = alias_tvar_get_ECR ((alias_typevar)node->value);
	    tau2 = alias_tvar_get_ECR (avar);
	    if (!ECR_equiv (tau1, tau2))
		    ECR_join (tau1, tau2);
    } 
#endif
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
unsigned int splay_tree_size PARAMS ((splay_tree));

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
  /* For debugging, disable the on-the-fly variable creation, and reenable this. */
/*  walk_tree_without_duplicates (&DECL_SAVED_TREE (current_function_decl),
			        find_func_decls, NULL);*/
  walk_tree_without_duplicates (&DECL_SAVED_TREE (current_function_decl),
				find_func_aliases, NULL);
  for (i = 0; i < VARRAY_ACTIVE_SIZE (local_alias_vars); i++)
     splay_tree_remove (alias_annot, (splay_tree_key) VARRAY_GENERIC_PTR (local_alias_vars, i));
  for (i = 0; i < VARRAY_ACTIVE_SIZE (local_alias_varnums); i ++)
	  VARRAY_GENERIC_PTR (alias_vars, VARRAY_INT (local_alias_varnums, i)) = NULL;
}
void
init_alias_vars ()
{
  init_alias_type ();
  VARRAY_GENERIC_PTR_INIT (local_alias_vars, 10, "Local alias vars");
  VARRAY_INT_INIT (local_alias_varnums, 10, "Local alias varnums");
  if (alias_vars == NULL)
    VARRAY_GENERIC_PTR_INIT (alias_vars, 10, "Alias vars");
  if (alias_annot == NULL)
    alias_annot = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
}
