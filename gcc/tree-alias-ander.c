/* Tree based Andersen points-to analysis
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
#include "tree-alias-ander.h"

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
#include "tree-flow.h"
#include "tree-inline.h"
#include "ssa.h"
#include "varray.h"
#include "tree-simple.h"
#include "hashtab.h"
#include "splay-tree.h"
#include "engine/util.h"
#include "libcompat/regions.h"
#include "andersen_terms.h"

/*  Andersen's interprocedural points-to analysis.
    This is a flow-insensitive, context insensitive algorithm.

    This file is an implementation of the alias_ops structure used by
    tree-alias-common.c to drive PTA analysis. 
    
    All these functions do is generate constraints for and through
    libbanshee. When we query for a points-to set, we ask libbanshee
    to solve the constraints and give us the answer.  The terms of the
    constraints are the aterms, which are an opaque data structure
    that stores libbanshee specific data for the constraints.  

    The constraints to be generated come from andersen's paper. By
    constraint, we mean something like "the points-to set of A must be
    a subset or equal to the points-to set of B" or "the points-to set
    of A must include Q".  In order to avoid having to write all the
    constraints directly in the code, we use helper functions such as
    pta_assignment, pta_rvalue, etc, that generate the necessary
    constraint terms  for us, making for much more readable code.

    One could replace libbanshee with some other constraint solving
    engine, and you'd simply have to replace the implementation of the
    pta_* functions, and provide replacements for the aterm specific
    functions (like making a list of aterms, printing the label of an
    aterm).  However, libbanshee is extremely fast, and extremely low
    memory usage, so one would be hard pressed to do better than it
    anyway.    

    Understanding how constraint solving and what each constraint means is
    beyond the scope of this documentation.  See the libbanshee
    documentation, and references therein for more enlightenment.
    
    That said, our constraints inclusion constraints of  set
    expressions.  Given the helper functions, the various inference
    functions we implement should *look* relatively straightforward.  

    In order to save time during queries, we cache the resulting
    points-to sets of each variable, rather than recalculate them
    again and again. (libbanshee actually has it's own internal
    caching, but the function call overhead for calling the solver is
    non-trivial, given the number of queries).

    Todo: Don't pass alias ops as first argument, just have a global
    "current_alias_ops".  */
				
static unsigned int id_num = 1;
static FILE *dump_file;
static int dump_flags;
static region andersen_rgn;
static void andersen_simple_assign (struct tree_alias_ops *,
				    alias_typevar, alias_typevar);
static void andersen_addr_assign (struct tree_alias_ops *,
				  alias_typevar, alias_typevar);
static void andersen_ptr_assign (struct tree_alias_ops *,
				 alias_typevar, alias_typevar);
static void andersen_op_assign (struct tree_alias_ops *,
				alias_typevar, varray_type, tree);
static void andersen_heap_assign (struct tree_alias_ops *, alias_typevar);
static void andersen_assign_ptr (struct tree_alias_ops *,
				 alias_typevar, alias_typevar);
static void andersen_function_def (struct tree_alias_ops *, alias_typevar,
				   varray_type, alias_typevar);
static int andersen_function_call (struct tree_alias_ops *, alias_typevar,
				   alias_typevar, varray_type);
static void andersen_init (struct tree_alias_ops *);
static int print_out_result (splay_tree_node, void *);
static void andersen_cleanup (struct tree_alias_ops *);
static bool andersen_may_alias (struct tree_alias_ops *, alias_typevar,
				alias_typevar);
static bool andersen_same_points_to_set (struct tree_alias_ops *, alias_typevar,
				alias_typevar);
static alias_typevar andersen_add_var (struct tree_alias_ops *, tree);
static alias_typevar andersen_add_var_same (struct tree_alias_ops *,
					    tree, alias_typevar);
static bool pointer_destroying_op (tree);

static splay_tree ptamap;


static struct tree_alias_ops andersen_ops = {
  andersen_init,
  andersen_cleanup,
  andersen_add_var,
  andersen_add_var_same,
  andersen_simple_assign,
  andersen_addr_assign,
  andersen_ptr_assign,
  andersen_op_assign,
  andersen_heap_assign,
  andersen_assign_ptr,
  andersen_function_def,
  andersen_function_call,
  andersen_may_alias,
  andersen_same_points_to_set,
  0, /* data */
  0, /* Currently non-interprocedural */
  1  /* Can do IP on all statics without help. */
};
struct tree_alias_ops *andersen_alias_ops = &andersen_ops;

static void term_inclusion (aterm, aterm);
static void pta_init (void);
static void pta_reset (void);
static aterm get_ref (aterm);
static argterm fun_rec_aterm (aterm_list);
static aterm pta_make_lam (const char *, aterm, aterm_list);
static aterm pta_make_ref (const char *);
static aterm pta_bottom (void);
static aterm pta_join (aterm, aterm);
static aterm pta_deref (aterm);
static aterm pta_rvalue (aterm);
static aterm pta_address (aterm);
static void pta_assignment (aterm, aterm);
static aterm pta_make_fun (const char *, aterm, aterm_list);
static aterm pta_application (aterm, aterm_list);

typedef aterm contents_type;
static contents_type pta_get_contents (aterm);
static void pr_ptset_aterm_elem (aterm);
static void pta_pr_ptset (contents_type);
#if 0
static int pta_get_ptsize (contents_type);
#endif

/* Hook for debugging.  This function is called instead of
   aterm_inclusion, and lets us print the actual constraints as they
   are generated.  */

static void
term_inclusion (aterm t1, aterm t2)
{
  if (dump_file)
    {
      fprintf (dump_file, "Constraint: ");
      aterm_print (dump_file, t1);
      fprintf (dump_file, " <= ");
      aterm_print (dump_file, t2);
      fprintf (dump_file,  "\n");
    }

  aterm_inclusion (t1, t2);
}

/* Initialize libbanshee's constraint engine.  */

static void
pta_init (void)
{
  andersen_terms_init ();
}

/* Reset libbanshee's constraint engine.  We do this when we are done
   using it, as it releases the memory libbanshee is using.  */

static void
pta_reset (void)
{
  andersen_terms_reset ();
}

static aterm
get_ref (aterm t)
{
  struct ref_decon r_decon;
  r_decon = ref_decon (t);

  assert (r_decon.f1);

  return r_decon.f1;
}

/* Make a function record out of the arguments.  */

static argterm
fun_rec_aterm (aterm_list args)
{
  region scratch;
  int counter = 0;
  argterm rest, result;
  aterm_list_scanner scan;
  aterm temp;
  char field_name[512];
  argterm_map map;

  scratch = newregion ();
  map = new_argterm_map (scratch);
  aterm_list_scan (args, &scan);
  while (aterm_list_next (&scan, &temp))
    {
      snprintf (field_name, 512, "%d", counter++);
      argterm_map_cons (argterm_make_field (field_name, temp), map);
    }

  rest = argterm_wild ();
  /* rest = argterm_fresh(); */

  /*  safe since field_add makes a copy of the string*/
  result = argterm_row (map, rest);

  deleteregion (scratch);

  return result;
}


static aterm
pta_make_lam (const char *id, aterm ret, aterm_list args)
{
  return lam (label_term_constant (id), fun_rec_aterm (args), ret);
}

/* Make a label reference to the given id.  */

static aterm
pta_make_ref (const char *id)
{

  aterm var = aterm_fresh (id);

  label_term tag = label_term_constant (id);

  return ref (tag, var, var);
}

/* Return the empty set.  */

static aterm
pta_bottom (void)
{
  return aterm_zero ();
}

/* Join two terms, such that anything in set t1 will also be in set
   t2, and vice versa.  */

static aterm
pta_join (aterm t1, aterm t2)
{
  aterm result;
  region scratch_rgn = newregion ();
  aterm_list list = new_aterm_list (scratch_rgn);

  aterm_list_cons (t1, list);
  aterm_list_cons (t2, list);


  result = aterm_union (list);
  deleteregion (scratch_rgn);

  return result;
}

/* Generate the constraint for a dereference of term t1.  */

static aterm
pta_deref (aterm t1)
{
  return ref_proj2 (t1);
}

/* Generate the constraint for t1 being an rvalue.  */

static aterm
pta_rvalue (aterm t1)
{
  return pta_deref (t1);
}

/* Generate the constraint for taking the address of t1.  */

static aterm
pta_address (aterm t1)
{
  return ref (label_term_one (), aterm_one (), t1);
}

/* Generate the constraint for assigning t2 to t1.  */

static void
pta_assignment (aterm t1, aterm t2)
{
  term_inclusion (t1, ref_pat1 (t2));
}

/* Make a function from the given name, return value, and arguments.  */

static aterm
pta_make_fun (const char *name, aterm ret, aterm_list args)
{
  aterm temp;
  aterm_list_scanner scan;
  region scratch_rgn = newregion ();
  aterm_list arg_list = new_aterm_list (scratch_rgn);

  aterm_list_scan (args, &scan);

  while (aterm_list_next (&scan, &temp))
    {
      aterm_list_cons (get_ref (temp), arg_list);
    }

  return pta_make_lam (name, get_ref (ret), arg_list);
}

/* Return the constraint for calling function T with arguments
   ACTUALS.  */

static aterm
pta_application (aterm t, aterm_list actuals)
{
  argterm args = fun_rec_aterm (actuals);

  term_inclusion (t, lam_pat1 (args));
  return pta_address (lam_proj2 (t));
}

/* Return the contents of set expression T.  */

static contents_type
pta_get_contents (aterm t)
{
  struct ref_decon t_decon;
  t_decon = ref_decon (t);

  return t_decon.f1;
}

/* Print out a points-to set element.  */

static void
pr_ptset_aterm_elem (aterm t)
{
  struct ref_decon ref;
  struct lam_decon lam;

  ref = ref_decon (t);
  lam = lam_decon (t);

  fprintf (dump_file, ",");
  if (ref.f0)
    label_term_print (dump_file, ref.f0);
  else if (lam.f0)
    label_term_print (dump_file, lam.f0);
  /*
     fprintf(stderr, ",");
     aterm_pr(stdout,(aterm)t);
   */
}


/* Print out a points-to set.  */

static void
pta_pr_ptset (contents_type t)
{
  int size;
  region scratch_rgn;
  aterm_list ptset;
  scratch_rgn = newregion ();
  ptset = aterm_list_copy (scratch_rgn, aterm_tlb (t));

  size = aterm_list_length (ptset);

  fprintf (dump_file, "{");
  if (!aterm_list_empty (ptset))
    {
      struct ref_decon ref;
      struct lam_decon lam;
      ref = ref_decon (aterm_list_head (ptset));
      lam = lam_decon (aterm_list_head (ptset));
      if (ref.f0)
	label_term_print (dump_file, ref.f0);
      else if (lam.f0)
	label_term_print (dump_file, lam.f0);

      /*      aterm_pr(stdout,aterm_hd(ptset)); */
      ptset = aterm_list_tail (ptset);
    }
  aterm_list_app (ptset, pr_ptset_aterm_elem);
  fprintf (dump_file, "}(%d)\n", size);
  deleteregion (scratch_rgn);
}

#if 0
static int
pta_get_ptsize (contents_type t)
{
  aterm_list ptset = aterm_tlb (t);
  return aterm_list_length (ptset);
}
#endif

/* Initialize Andersen alias analysis. */
static int initted = 0;

static void
andersen_init (struct tree_alias_ops *ops ATTRIBUTE_UNUSED)
{
  if (!initted || !flag_unit_at_a_time)
    {
      pta_init ();
      andersen_rgn = newregion ();
      initted = 1;
    }

  dump_file = dump_begin (TDI_pta, &dump_flags);
  ptamap = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
  /* Don't claim we can do ip partial unless the user requests it. */
  if (!flag_unit_at_a_time)
    andersen_ops.ip_partial = 0;

}

static int
print_out_result (splay_tree_node node, void *data ATTRIBUTE_UNUSED)
{
  fprintf (dump_file, "%s :=",
	   alias_get_name (ALIAS_TVAR_DECL (((alias_typevar) node->value))));
  pta_pr_ptset (pta_get_contents ((aterm) node->key));
  return 0;
}

/* Cleanup after Andersen alias analysis.  */

static void
andersen_cleanup (struct tree_alias_ops *ops ATTRIBUTE_UNUSED)
{
  if (dump_file)
    {
      if (dump_flags & TDF_STATS)
	{
	  fprintf (dump_file, "\nPoints-to stats:\n");
	  andersen_terms_stats (dump_file);
	}

      fprintf (dump_file, "\nPoints-to sets:\n");
      splay_tree_foreach (ptamap, print_out_result, NULL);
      dump_end (TDI_pta, dump_file);
    }

  if (!flag_unit_at_a_time)
    {
      pta_reset ();
      splay_tree_delete (ptamap);
      deleteregion (andersen_rgn);
      andersen_rgn = NULL;
    }


}

/* Add decl to the analyzer, and return a typevar for it.  For
   Andersen, we create a new alias typevar for the declaration, and
   return that.  */

static alias_typevar
andersen_add_var (struct tree_alias_ops *ops ATTRIBUTE_UNUSED, tree decl)
{
  alias_typevar ret;
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Adding variable %s\n",
	     alias_get_name (decl));

  if (alias_get_name (decl) != NULL)
    {
      ret = alias_tvar_new_with_aterm (decl,
				       pta_make_ref (alias_get_name (decl)));
    }
  else
    {
      char *tmp_name;
      ASM_FORMAT_PRIVATE_NAME (tmp_name, "unnamed var", id_num++);
      ret = alias_tvar_new_with_aterm (decl, pta_make_ref (tmp_name));
    }
  splay_tree_insert (ptamap, (splay_tree_key) ALIAS_TVAR_ATERM (ret),
		     (splay_tree_value) ret);
  ALIAS_TVAR_PTSET (ret) = NULL;

  return ret;
}

/* Add a variable to the analyzer that is equivalent (as far as
   aliases go) to some existing typevar.
   For Andersen, we just call a function that does this for us.  */

static alias_typevar
andersen_add_var_same (struct tree_alias_ops *ops ATTRIBUTE_UNUSED, tree decl,
		       alias_typevar tv)
{
  alias_typevar ret;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Adding variable %s same as %s\n",
	     alias_get_name (decl), alias_get_name (ALIAS_TVAR_DECL (tv)));

  if (alias_get_name (decl) != NULL)
    ret = alias_tvar_new_with_aterm (decl,
				     pta_make_ref (alias_get_name (decl)));
  else
    {
      char *tmp_name;
      ASM_FORMAT_PRIVATE_NAME (tmp_name, "unnamed var", id_num++);
      ret = alias_tvar_new_with_aterm (decl, pta_make_ref (tmp_name));
    }

  pta_join (ALIAS_TVAR_ATERM (tv), ALIAS_TVAR_ATERM (ret));
  splay_tree_insert (ptamap, (splay_tree_key) ALIAS_TVAR_ATERM (ret),
		     (splay_tree_value) ret);
  ALIAS_TVAR_PTSET (tv) = NULL;
  ALIAS_TVAR_PTSET (ret) = NULL;

  return ret;
}

/* Inference for simple assignment (lhs = rhs) */

static void
andersen_simple_assign (struct tree_alias_ops *ops ATTRIBUTE_UNUSED,
			alias_typevar lhs, alias_typevar rhs)
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Simple assignment %s = %s\n",
	     alias_get_name (ALIAS_TVAR_DECL (lhs)),
	     alias_get_name (ALIAS_TVAR_DECL (rhs)));
  if (lhs == rhs)
    return;
  
  /* The rvalue is just the term itself, and we generate a constraint
     for assigning it to the lhs.  */
  pta_assignment (ALIAS_TVAR_ATERM (lhs),
		  pta_rvalue (ALIAS_TVAR_ATERM (rhs)));
}

/* Inference for address assignment (lhs = &addr) */

static void
andersen_addr_assign (struct tree_alias_ops *ops ATTRIBUTE_UNUSED,
		      alias_typevar lhs, alias_typevar addr)
{
  if (addr == NULL)
    return;
 if (dump_file && (dump_flags & TDF_DETAILS))
   fprintf (dump_file, "Address assignment %s = &%s\n",
	    alias_get_name (ALIAS_TVAR_DECL (lhs)),
	    alias_get_name (ALIAS_TVAR_DECL (addr)));

 /* The rvalue here is the address of a term, and we generate a
    constraint to assign this address to the lhs.  */
  pta_assignment (ALIAS_TVAR_ATERM (lhs),
		  pta_rvalue (pta_address (ALIAS_TVAR_ATERM (addr))));
}


/* Inference for pointer assignment (lhs = *ptr) */

static void
andersen_ptr_assign (struct tree_alias_ops *ops ATTRIBUTE_UNUSED,
		     alias_typevar lhs, alias_typevar ptr)
{

  if (ptr == NULL)
    return;
 if (dump_file && (dump_flags & TDF_DETAILS))
   fprintf (dump_file, "Pointer assignment %s = *%s\n",
	    alias_get_name (ALIAS_TVAR_DECL (lhs)),
	    alias_get_name (ALIAS_TVAR_DECL (ptr)));

  pta_assignment (ALIAS_TVAR_ATERM (lhs),
		  pta_rvalue (pta_deref (ALIAS_TVAR_ATERM (ptr))));

}

/* Determine if OP destroys the current assumed to be valid pointer
   (whether it generates a new valid pointer is not relevant).  */

static bool
pointer_destroying_op (tree op)
{
  switch (TREE_CODE (op))
    {
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_NOT_EXPR:
    case LT_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case LE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case MULT_EXPR:
    case TRUNC_DIV_EXPR:
    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
    case LROTATE_EXPR:
    case RROTATE_EXPR:
      return true;
    default:
      return false;
    }
  return false;
}

/* Inference rule for operations (lhs = operation(operands)).  */

static void
andersen_op_assign (struct tree_alias_ops *ops ATTRIBUTE_UNUSED,
		    alias_typevar lhs, varray_type operands, tree operation)
{
  aterm newvar = NULL;
  
  if (VARRAY_ACTIVE_SIZE (operands) == 0)
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Op assignment %s = ",
	       alias_get_name (ALIAS_TVAR_DECL (lhs)));
      print_generic_stmt (dump_file, operation, 0);
      fprintf (dump_file, "\n");
    }
  
      
  /* Pointer destroying operations do not give us the same valid pointer
     back, and thus, are assignment to pta_bottom. */
  if (pointer_destroying_op (operation))
    {
      pta_assignment (ALIAS_TVAR_ATERM (lhs), pta_rvalue (pta_bottom ()));
      return;
    }
  
  /* Operations in general we can't track the exact effect of.  Thus,
     we conservatively assume that it could make the LHS point to
     *anything* the RHS points to.  To signify this, we join the RHS
     variables together and assign it to the LHS.  */
  /* The >2 case occurs when we are dealing with constructors.  */
  if (VARRAY_ACTIVE_SIZE (operands) > 2)
    {
      size_t i;
      alias_typevar tv1 = VARRAY_GENERIC_PTR (operands, 0);
      newvar = ALIAS_TVAR_ATERM (tv1);
      for (i = 1; i < VARRAY_ACTIVE_SIZE (operands); i++)
	{
	  alias_typevar tempvar = VARRAY_GENERIC_PTR (operands, i);
	  aterm t2 = ALIAS_TVAR_ATERM (tempvar);
	  newvar = pta_join (newvar, t2);
	}
    }
  else if (VARRAY_ACTIVE_SIZE (operands) == 2)
    {
      alias_typevar tv1 = VARRAY_GENERIC_PTR (operands, 0);
      alias_typevar tv2 = VARRAY_GENERIC_PTR (operands, 1);
      aterm t1 = ALIAS_TVAR_ATERM (tv1);
      aterm t2 = ALIAS_TVAR_ATERM (tv2);
      newvar = pta_join (t1, t2);
    }
  else if (VARRAY_ACTIVE_SIZE (operands) == 1)
    {
      alias_typevar tv1 = VARRAY_GENERIC_PTR (operands, 0);
      aterm t1 = ALIAS_TVAR_ATERM (tv1);
      newvar = t1;
    }
  pta_assignment (ALIAS_TVAR_ATERM (lhs), pta_rvalue (newvar));
}

/* Inference for heap assignment (lhs = alloc).  */

static void
andersen_heap_assign (struct tree_alias_ops *ops ATTRIBUTE_UNUSED,
		      alias_typevar lhs ATTRIBUTE_UNUSED)
{
#if 0
  alias_type type1;
  ECR tau;
  type1 = ECR_get_type (alias_tvar_get_ECR (lhs));
  tau = alias_ltype_loc (type1);

  if (ECR_get_type (tau) == alias_bottom)
    ECR_set_type (tau, alias_ltype_new ());
#endif
}

/* Inference for assignment to a pointer (*ptr = rhs).  */

static void
andersen_assign_ptr (struct tree_alias_ops *ops ATTRIBUTE_UNUSED,
		     alias_typevar ptr, alias_typevar rhs)
{

  if (rhs == NULL)
    return;
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Assignment to pointer  *%s = %s\n",
	     alias_get_name (ALIAS_TVAR_DECL (ptr)),
	     alias_get_name (ALIAS_TVAR_DECL (rhs)));
  /* The RHS is a standard rvalue, and the LHS is a pointer
     dereference.  */
  pta_assignment (pta_deref (ALIAS_TVAR_ATERM (ptr)),
		  pta_rvalue (ALIAS_TVAR_ATERM (rhs)));
}

/* Inference for a function definition.  */

static void
andersen_function_def (struct tree_alias_ops *ops ATTRIBUTE_UNUSED,
		       alias_typevar func, varray_type params,
		       alias_typevar retval)
{
  aterm_list args = new_aterm_list (andersen_rgn);
  aterm fun_type;

  size_t l = VARRAY_ACTIVE_SIZE (params);
  size_t i;

  /* Set up the arguments for the new function type. */
  for (i = 0; i < l; i++)
    {
      alias_typevar tv = VARRAY_GENERIC_PTR (params, i);
      aterm_list_cons (ALIAS_TVAR_ATERM (tv), args);
    }
  /* Create the function type.  */
  fun_type = pta_make_fun (alias_get_name (ALIAS_TVAR_DECL (func)),
			   ALIAS_TVAR_ATERM (retval), args);

  /* Assign the function type itself to the function.  */
  pta_assignment (ALIAS_TVAR_ATERM (func), fun_type);
}

/* Inference for a function call assignment.  */

static int
andersen_function_call (struct tree_alias_ops *ops ATTRIBUTE_UNUSED,
			alias_typevar lhs, alias_typevar func,
			varray_type args)
{
  aterm_list actuals = new_aterm_list (andersen_rgn);
  aterm ftype = ALIAS_TVAR_ATERM (func);
  aterm ret = NULL;
  aterm res;
  tree decl = ALIAS_TVAR_DECL (func);

  size_t i;

  if (lhs)
    ret = ALIAS_TVAR_ATERM (lhs);
  for (i = 0; i < VARRAY_ACTIVE_SIZE (args); i++)
    {
      alias_typevar argtv = VARRAY_GENERIC_PTR (args, i);
      aterm arg = ALIAS_TVAR_ATERM (argtv);
      aterm_list_cons (pta_rvalue (arg), actuals);
    }
  aterm_list_reverse (actuals);
  
  /* Generate the constraint that calls the function with it's
     arguments, and gives us the result.  This in turn applies
     whatever constraints are in that function.  */
  res = pta_application (pta_rvalue (ftype), actuals);
  /* We only need care about the result if we have an LHS.  If we do,
     assign the result of function application back to the LHS.  */
  if (ret)
    pta_assignment (ret, pta_rvalue (res));

  /* We can handle functions we've got trees for. non-statics will
     just have incoming parameters assigned to global_var if
     necessary. */
  if (TREE_CODE (decl) == FUNCTION_DECL
      && DECL_PTA_TYPEVAR (decl)
      && flag_unit_at_a_time
      && (!TREE_PUBLIC (decl) && TREE_STATIC (decl)))
    {
      return 0;
    }
  return 1;
}


/* Simple pointer comparison function for list sorting.  */

static int 
simple_cmp (const aterm a, const aterm b)
{
  return (int *)a - (int *)b;
}


/* Determine if two aterm's have the same points-to set.
   When we didn't create global_var, we can just get the two points-to
   sets and compare the lengths, then the names.
   When we did create global_var, we have to filter it out.  */

static bool
andersen_same_points_to_set (struct tree_alias_ops *ops ATTRIBUTE_UNUSED,
			     alias_typevar ptrtv, alias_typevar vartv)
{
  aterm_list ptset1, ptset2;
  aterm_list_scanner scan1, scan2;
  aterm data1, data2;
  region scratch_rgn = newregion ();

  ptset1 = ALIAS_TVAR_PTSET (ptrtv);
  ptset2 = ALIAS_TVAR_PTSET (vartv);
  /* Solve the points-to constraints and get the resulting sets if
     they weren't cached previously.  */
  if (!ptset1)
    {
      ptset1 = aterm_tlb (pta_get_contents (ALIAS_TVAR_ATERM (ptrtv)));
      ALIAS_TVAR_PTSET (ptrtv) = ptset1;
    }
  if (!ptset2)
    {
      ptset2 = aterm_tlb (pta_get_contents (ALIAS_TVAR_ATERM (vartv)));
      ALIAS_TVAR_PTSET (vartv) = ptset2;
    }
  
  if (aterm_list_length (ptset1) != aterm_list_length (ptset2))
    return false;

  if (ptset1 == ptset2)
    return true;

  ptset1 = aterm_list_copy (scratch_rgn, ptset1);
  ptset2 = aterm_list_copy (scratch_rgn, ptset2);
  
  if (aterm_list_length (ptset1) != aterm_list_length (ptset2))
    return false;

  ptset1 = aterm_list_sort (ptset1, simple_cmp);
  ptset2 = aterm_list_sort (ptset2, simple_cmp);
  
  aterm_list_scan (ptset1, &scan1);
  aterm_list_scan (ptset2, &scan2);
  while (aterm_list_next (&scan1, &data1))
    {
      aterm_list_next (&scan2, &data2);
      if (data1 != data2)
	{
	  deleteregion(scratch_rgn);
	  return false;
	}
    }
  deleteregion(scratch_rgn);
  return true;  
}


/* Determine if two variables may alias.  In our case, this means
   whether the decl represented by PTRTV can point to VARTV.  */

static bool
andersen_may_alias (struct tree_alias_ops *ops ATTRIBUTE_UNUSED,
		    alias_typevar ptrtv, alias_typevar vartv)
{
  aterm_list ptset;
  ptset = ALIAS_TVAR_PTSET (ptrtv);

  /* Solve the points-to constraints and get the resulting set if we
     haven't cached it.  */
  if (!ptset)
    {
      ptset = aterm_tlb (pta_get_contents (ALIAS_TVAR_ATERM (ptrtv)));
      ALIAS_TVAR_PTSET (ptrtv) = ptset;
    }

  if (aterm_list_empty (ptset))
    return false;

  return aterm_list_member (ptset, ALIAS_TVAR_ATERM (vartv));
}
