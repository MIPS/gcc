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
#include "tree.h"  
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
   * Create SSA may/must def variables.
   * Reduce memory usage (mainly due to fragmentation, not leakage).
   * Don't pass alias ops as first argument, just have a global 
     "current_alias_ops".
   * Finish post-pass cleanup.
*/

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
  0, /* data */
  0 /* Currently non-interprocedural */
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

