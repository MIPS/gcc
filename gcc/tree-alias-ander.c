/* Tree based Andersen points-to analysis
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
#include "c-common.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "ssa.h"
#include "varray.h"
#include "c-tree.h"
#include "tree-simple.h"
#include "hashtab.h"
#include "splay-tree.h"
#include "engine/util.h"
#include "libcompat/regions.h"
#include "andersen_terms.h"

/**
   @file tree-alias-ander.c
   Andersen's interprocedural points-to analysis.
   This is a flow-insensitive, context insensitive algorithm. 
   
   @todo Don't pass alias ops as first argument, just have a global 
   "current_alias_ops".
*/

static unsigned int id_num = 1;
static FILE *dump_file;
static int dump_flags;
static region andersen_rgn;
static void andersen_simple_assign PARAMS ((struct tree_alias_ops *,
					    alias_typevar, alias_typevar));
static void andersen_addr_assign PARAMS ((struct tree_alias_ops *,
					  alias_typevar, alias_typevar));
static void andersen_ptr_assign PARAMS ((struct tree_alias_ops *,
					 alias_typevar, alias_typevar));
static void andersen_op_assign PARAMS ((struct tree_alias_ops *,
					alias_typevar, varray_type));
static void andersen_heap_assign PARAMS ((struct tree_alias_ops *,
					  alias_typevar));
static void andersen_assign_ptr PARAMS ((struct tree_alias_ops *,
					 alias_typevar, alias_typevar));
static void andersen_function_def PARAMS ((struct tree_alias_ops *,
					   alias_typevar, varray_type,
					   alias_typevar));
static int andersen_function_call PARAMS ((struct tree_alias_ops *,
					    alias_typevar, alias_typevar,
					    varray_type));
static void andersen_init PARAMS ((struct tree_alias_ops *));
static int print_out_result PARAMS ((splay_tree_node, void *));
static void andersen_cleanup PARAMS ((struct tree_alias_ops *));
static bool andersen_may_alias PARAMS ((struct tree_alias_ops *,
					alias_typevar, alias_typevar));
static alias_typevar andersen_add_var PARAMS ((struct tree_alias_ops *, tree));
static alias_typevar andersen_add_var_same PARAMS ((struct tree_alias_ops *, tree, alias_typevar));
static hashval_t ptset_map_hash PARAMS ((const PTR));
static int ptset_map_eq PARAMS ((const PTR, const PTR));

static splay_tree ptamap;
static htab_t ptset_map;

#define POINTER_HASH(x) (hashval_t)((long)x >> 3)
struct ptset_map_data
{
  tree decl;
  aterm_list ptset;
};
static hashval_t
ptset_map_hash (p)
     const PTR p;
{
  return POINTER_HASH (((struct ptset_map_data *)p)->decl);
}
static int
ptset_map_eq (p1, p2)
     const PTR p1;
     const PTR p2;
{
  return ((struct ptset_map_data *)p1)->decl == p2;
}

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
  0, /* data */
  0, /* Currently non-interprocedural */
  1  /* Can do IP on all statics without help. */
};
struct tree_alias_ops *andersen_alias_ops = &andersen_ops;

static void term_inclusion PARAMS ((aterm, aterm));
static void pta_init PARAMS ((void));
static void pta_reset PARAMS ((void));
static aterm get_ref PARAMS ((aterm));
static argterm fun_rec_aterm PARAMS ((aterm_list));
static aterm pta_make_lam PARAMS ((const char *, aterm, aterm_list));
static aterm pta_make_ref PARAMS ((const char *));
#if 0
static aterm pta_bottom PARAMS ((void));
#endif
static aterm pta_join PARAMS ((aterm, aterm));
static aterm pta_deref PARAMS ((aterm));
static aterm pta_rvalue PARAMS ((aterm));
static aterm pta_address PARAMS ((aterm));
static void pta_assignment PARAMS ((aterm, aterm));
static aterm pta_make_fun PARAMS ((const char *, aterm, aterm_list));
static aterm pta_application PARAMS ((aterm, aterm_list));

typedef aterm contents_type;
static contents_type pta_get_contents PARAMS ((aterm));
static void pr_ptset_aterm_elem PARAMS ((aterm));
static void pta_pr_ptset PARAMS ((contents_type));
#if 0
static int pta_get_ptsize PARAMS ((contents_type));
#endif

/* Hook for debugging */
static void
term_inclusion (t1, t2)
     aterm t1;
     aterm t2;
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

static void
pta_init ()
{
  andersen_terms_init ();
}

static void
pta_reset ()
{
  andersen_terms_reset ();
}


static aterm
get_ref (t)
     aterm t;
{
  struct ref_decon r_decon;
  r_decon = ref_decon (t);

  assert (r_decon.f1);

  return r_decon.f1;
}

static argterm
fun_rec_aterm (args)
     aterm_list args;
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
pta_make_lam (id, ret, args)
     const char *id;
     aterm ret;
     aterm_list args;
{
  return lam (label_term_constant (id), fun_rec_aterm (args), ret);
}

static aterm
pta_make_ref (id)
     const char *id;
{

  aterm var = aterm_fresh (id);

  label_term tag = label_term_constant (id);

  return ref (tag, var, var);
}

#if 0
static aterm
pta_bottom ()
{
  return aterm_zero ();
}
#endif

static aterm
pta_join (t1, t2)
     aterm t1;
     aterm t2;
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

static aterm
pta_deref (t1)
     aterm t1;
{
  return ref_proj2 (t1);
}

static aterm
pta_rvalue (t1)
     aterm t1;
{
  return pta_deref (t1);
}

static aterm
pta_address (t1)
     aterm t1;
{
  return ref (label_term_one (), aterm_one (), t1);
}

static void
pta_assignment (t1, t2)
     aterm t1;
     aterm t2;
{
  term_inclusion (t1, ref_pat1 (t2));
}

static aterm
pta_make_fun (name, ret, args)
     const char *name;
     aterm ret;
     aterm_list args;
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

static aterm
pta_application (t, actuals)
     aterm t;
     aterm_list actuals;
{
  argterm args = fun_rec_aterm (actuals);

  term_inclusion (t, lam_pat1 (args));
  return pta_address (lam_proj2 (t));
}

static contents_type
pta_get_contents (t)
     aterm t;
{
  struct ref_decon t_decon;
  t_decon = ref_decon (t);

  return t_decon.f1;
}

static void
pr_ptset_aterm_elem (t)
     aterm t;
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


static void
pta_pr_ptset (t)
     contents_type t;
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
pta_get_ptsize (t)
     contents_type t;
{
  aterm_list ptset = aterm_tlb (t);
  return aterm_list_length (ptset);
}
#endif

/* Initialize Andersen alias analysis. */
static int initted = 0;

static void
andersen_init (ops)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
{
  if (!initted || !flag_ip)
    {
      pta_init ();
      andersen_rgn = newregion ();
      initted = 1;
    }
  
  dump_file = dump_begin (TDI_pta, &dump_flags);
  ptamap = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
  ptset_map = htab_create (7, ptset_map_hash, ptset_map_eq, free);
  /* Don't claim we can do ip partial unless the user requests it. */
  if (!flag_ip)
    andersen_ops.ip_partial = 0;
  
}

static int
print_out_result (node, data)
     splay_tree_node node;
     void *data ATTRIBUTE_UNUSED;
{
  fprintf (dump_file, "%s :=",
	   alias_get_name (ALIAS_TVAR_DECL (((alias_typevar) node->value))));
  pta_pr_ptset (pta_get_contents ((aterm) node->key));
  return 0;
}

/* Cleanup after Andersen alias analysis. 
   Currently does nothing.  */
static void
andersen_cleanup (ops)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
{
  if (dump_file)
    {
      if (dump_flags & TDF_STATS)    
	{
	  fprintf (dump_file, "\nPoints-to stats:\n");
	  andersen_terms_stats (dump_file);
	}
      /*      if (dump_flags & TDF_DOT)
	      {
	      
	      snprintf (name, 512, "%s.dot", get_name (current_function_decl));
	      dot = fopen (name, "w");
	      andersen_terms_print_graph (dot);
	      fclose (dot);
	      }*/
      
      fprintf (dump_file, "\nPoints-to sets:\n");
      splay_tree_foreach (ptamap, print_out_result, NULL);
      dump_end (TDI_pta, dump_file);
    }
  
  
  if (!flag_ip)
    {
      pta_reset ();
      splay_tree_delete (ptamap);
      htab_delete (ptset_map);
      deleteregion (andersen_rgn);
    }
  

}

/* Add decl to the analyzer, and return a typevar for it.  For
   Andersen, we create a new alias typevar for the declaration, and
   return that.  */

static alias_typevar
andersen_add_var (ops, decl)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     tree decl;
{
  alias_typevar ret;
  PTR *slot;
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
  slot = htab_find_slot_with_hash (ptset_map, 
				   decl,
				   POINTER_HASH (decl),
				   NO_INSERT);
  if (slot)
    htab_clear_slot (ptset_map, slot);

  return ret;
}

/* Add a variable to the analyzer that is equivalent (as far as
   aliases go) to some existing typevar.  
   For Andersen, we just call a function that does this for us.  */
static alias_typevar
andersen_add_var_same (ops, decl, tv)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     tree decl;
     alias_typevar tv;
{
  alias_typevar ret;
  PTR *slot;
  
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
  slot = htab_find_slot_with_hash (ptset_map, 
				   decl,
				   POINTER_HASH (decl),
				   NO_INSERT);
  if (slot)
    htab_clear_slot (ptset_map, slot);
  
  slot = htab_find_slot_with_hash (ptset_map,
				   ALIAS_TVAR_DECL (tv),
				   POINTER_HASH (ALIAS_TVAR_DECL (tv)),
				   NO_INSERT);
  if (slot)
    htab_clear_slot (ptset_map, slot);
  
  return ret;
}

/* Inference for simple assignment (lhs = rhs) */
static void
andersen_simple_assign (ops, lhs, rhs)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
     alias_typevar rhs;
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Simple assignment %s = %s\n",
	     alias_get_name (ALIAS_TVAR_DECL (lhs)),
	     alias_get_name (ALIAS_TVAR_DECL (rhs)));
  
  pta_assignment (ALIAS_TVAR_ATERM (lhs),
		  pta_rvalue (ALIAS_TVAR_ATERM (rhs)));
}

/* Inference for address assignment (lhs = &addr) */
static void
andersen_addr_assign (ops, lhs, addr)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
     alias_typevar addr;
{
  if (addr == NULL)
    return;
 if (dump_file && (dump_flags & TDF_DETAILS))
   fprintf (dump_file, "Address assignment %s = &%s\n",
	    alias_get_name (ALIAS_TVAR_DECL (lhs)),
	    alias_get_name (ALIAS_TVAR_DECL (addr)));
  pta_assignment (ALIAS_TVAR_ATERM (lhs),
		  pta_rvalue (pta_address (ALIAS_TVAR_ATERM (addr))));
}


/* Inference for pointer assignment (lhs = *ptr) */
static void
andersen_ptr_assign (ops, lhs, ptr)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
     alias_typevar ptr;
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

/* Inference rule for operations (lhs = operation(operands)) */
static void
andersen_op_assign (ops, lhs, operands)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
     varray_type operands;
{
  size_t i;
  
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Op assignment %s = op(...)\n",
	     alias_get_name (ALIAS_TVAR_DECL (lhs)));
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (operands); i++)
    {
      alias_typevar tv = VARRAY_GENERIC_PTR (operands, i);

      if (tv == NULL)
	continue;

      pta_assignment (ALIAS_TVAR_ATERM (lhs),
		      pta_rvalue (ALIAS_TVAR_ATERM (tv)));
/*      pta_join  (ALIAS_TVAR_ATERM (lhs), ALIAS_TVAR_ATERM (tv));*/
    }
}

/* Inference for heap assignment (lhs = alloc) */
static void
andersen_heap_assign (ops, lhs)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs ATTRIBUTE_UNUSED;
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

/* Inference for assignment to a pointer (*ptr = rhs) */
static void
andersen_assign_ptr (ops, ptr, rhs)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar ptr;
     alias_typevar rhs;
{

  if (rhs == NULL)
    return;
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Assignment to pointer  *%s = %s\n",
	     alias_get_name (ALIAS_TVAR_DECL (ptr)),
	     alias_get_name (ALIAS_TVAR_DECL (rhs)));

  pta_assignment (pta_deref (ALIAS_TVAR_ATERM (ptr)),
		  pta_rvalue (ALIAS_TVAR_ATERM (rhs)));
}

/* Inference for a function definition. */

static void
andersen_function_def (ops, func, params, retval)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar func;
     varray_type params;
     alias_typevar retval;
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
  fun_type = pta_make_fun (alias_get_name (ALIAS_TVAR_DECL (func)),
			   ALIAS_TVAR_ATERM (retval), args);
  pta_assignment (ALIAS_TVAR_ATERM (func), fun_type);
}

/* Inference for a function call assignment */
static int
andersen_function_call (ops, lhs, func, args)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar lhs;
     alias_typevar func;
     varray_type args;
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
  
  res = pta_application (pta_rvalue (ftype), actuals);
  if (ret)
    pta_assignment (ret, pta_rvalue (res));
  
  /* We can handle functions we've got trees for. non-statics will
     just have incoming parameters assigned to global_var if
     necessary. */
  if (TREE_CODE (decl) == FUNCTION_DECL 
      && DECL_SAVED_TREE (decl) != NULL_TREE 
      && flag_ip 
      && (!TREE_PUBLIC (decl) && TREE_STATIC (decl)))
    {
      return 0;
    } 
  return 1;
}


static aterm stupid_hack;
static bool eq_to_var PARAMS ((const aterm));
static bool
eq_to_var (term)
     const aterm term;
{
  return stupid_hack == term;
}

static bool
andersen_may_alias (ops, ptrtv, vartv)
     struct tree_alias_ops *ops ATTRIBUTE_UNUSED;
     alias_typevar ptrtv;
     alias_typevar vartv;
{
  aterm_list ptset;
  struct ptset_map_data *data;
  
  data = htab_find_with_hash (ptset_map, ALIAS_TVAR_DECL (ptrtv),
			      POINTER_HASH (ALIAS_TVAR_DECL (ptrtv)));
  
  if (!data)
    {
      ptset = aterm_tlb (pta_get_contents (ALIAS_TVAR_ATERM (ptrtv)));
      data = xmalloc (sizeof (struct ptset_map_data));
      data->decl = ALIAS_TVAR_DECL (ptrtv);
      data->ptset = ptset;
      *(htab_find_slot_with_hash (ptset_map,
				  ALIAS_TVAR_DECL (ptrtv),
				  POINTER_HASH (ALIAS_TVAR_DECL (ptrtv)),
				  INSERT)) = data;
    }
  else
    {
      ptset = data->ptset;
    }

  if (aterm_list_empty (ptset))
    return false;

  stupid_hack = ALIAS_TVAR_ATERM (vartv);

  return aterm_list_find (ptset, eq_to_var) != NULL;
}
