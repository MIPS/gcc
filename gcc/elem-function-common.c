/* This file is part of the Intel(R) Cilk(TM) Plus support
   This file contains the language independent functions for
   Elemental functions.
   
   Copyright (C) 2013  Free Software Foundation, Inc.
   Written by Balaji V. Iyer <balaji.v.iyer@intel.com>,
              Intel Corporation

   Many Thanks to Karthik Kumar for advice on the basic technique
   about cloning functions.
   
   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "langhooks.h"
#include "basic-block.h"
#include "output.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "tree-pass.h"
#include "timevar.h"
#include "tree-inline.h"
#include "cgraph.h"
#include "ipa-prop.h"
#include "opts.h"
#include "tree-iterator.h"
#include "vec.h"
#include "cilk.h"
#include "target.h"

enum vect_def_type {
  vect_uninitialized_def = 0,
  vect_constant_def = 1,
  vect_external_def,
  vect_internal_def,
  vect_induction_def,
  vect_reduction_def,
  vect_double_reduction_def,
  vect_nested_cycle,
  vect_unknown_def_type
};

elem_fn_info *extract_elem_fn_values (tree decl, size_t *);

/* Returns the vector length in string format based on the value in the field
   called vectorlength of ELEM_FN_VALUES. If vectorlength is not given then
   an appropriate value is computed based on the architecture information given
   in proc_type field of ELEM_FN_VALUES.  */

char *
find_vlength_code (elem_fn_info *elem_fn_values)
{
  int v_length = 0;
  char *vlength_code = XNEWVEC (char, 10);
  if (!elem_fn_values)
    { 
      sprintf (vlength_code, "4");
      return vlength_code;
    }

  memset (vlength_code, 10, 0);
  
  if (elem_fn_values->no_vlengths != 0)
    sprintf (vlength_code,"%d", elem_fn_values->vectorlength);
  else
    {
      v_length = targetm.cilkplus.builtin_find_vlength_for_proc
	(elem_fn_values->proc_type);
      if (v_length > 0)
	sprintf (vlength_code, "%d", v_length);
    }
  return vlength_code;
}


/* This function will concatinate the PREFIX to the function name in DECL.  */

tree
rename_elem_fn (tree decl, const char *prefix)
{
  int length = 0;
  const char *fn_name = IDENTIFIER_POINTER (DECL_NAME (decl));
  char *new_fn_name;
  tree new_decl = NULL_TREE;
  
  if (!prefix || !fn_name)
    return decl;
  else
    new_decl = decl;

  length = strlen (fn_name) + strlen (prefix) + 1;
  new_fn_name = XNEWVEC (char, length);
  strcpy (new_fn_name, prefix);
  strcat (new_fn_name, fn_name);
  
  DECL_NAME (new_decl) = get_identifier (new_fn_name);
  return new_decl;
}

/* Returns the appropriate mangling prefix for the vector function based on the
   information in ELEM_FN_VALUES field.  The user can specify whether they want
   a masked or unmasked function by setting the MASKED field to true or false,
   respectively.  */

char *
find_prefix (elem_fn_info *elem_fn_values, bool masked)
{
  char *prefix = XNEWVEC (char, 100);
  char tmp_st[10];
  int arg_number, ii_pvar, ii_uvar, ii_lvar;

  if (!elem_fn_values)
    return NULL;
  
  strcpy (prefix, "_ZGV");
  strcat (prefix,
	  targetm.cilkplus.builtin_find_isa_code (elem_fn_values->isa_type));
  if (masked)
    strcat (prefix, "M");
  else
    strcat (prefix, "N");
  strcat (prefix, find_vlength_code (elem_fn_values));

  for (arg_number = 0; arg_number <= elem_fn_values->total_no_args;
       arg_number++)
    {
      for (ii_lvar = 0; ii_lvar < elem_fn_values->no_lvars; ii_lvar++)
	{
	  if (elem_fn_values->linear_location[ii_lvar] == arg_number)
	    {
	      strcat (prefix, "l");
	      if (elem_fn_values->linear_steps[ii_lvar] > 1)
		{
		  sprintf (tmp_st, "%d", elem_fn_values->linear_steps[ii_lvar]);
		  strcat (prefix, tmp_st);
		}
	    }
	}
      for (ii_uvar = 0; ii_uvar < elem_fn_values->no_uvars; ii_uvar++) 
	if (elem_fn_values->uniform_location[ii_uvar] == arg_number) 
	  strcat (prefix, "u");
      for (ii_pvar = 0; ii_pvar < elem_fn_values->no_pvars; ii_pvar++) 
	if (elem_fn_values->private_location[ii_pvar] == arg_number) 
	  strcat (prefix, "v");
    }
  strcat (prefix, "_");
  return prefix;
}


/* This is an helper function for find_elem_fn_param_type.  It returns the
   parm_type (whether TYPE_LINEAR or TYPE_UNIFORM) for the parameter number
   (set by PARM_NO).  If the return value is TYPE_LINEAR, then the *STEP_SIZE
   is set with the appropriate step-size.  */
   
static enum elem_fn_parm_type
find_elem_fn_parm_type_1 (tree fndecl, int parm_no, tree *step_size)
{
  size_t no_entries = 0;
  int ii = 0;
  elem_fn_info *elem_fn_values;

  elem_fn_values = extract_elem_fn_values (fndecl, &no_entries);
  if (!elem_fn_values)
    return TYPE_NONE;
  else if (no_entries > 1)
    {
      error_at (EXPR_LOCATION (fndecl), "multiple vector attributes found");
      return TYPE_NONE;
    }
  for (ii = 0; ii < elem_fn_values->no_lvars; ii++)
    if (elem_fn_values->linear_location[ii] == parm_no)
      {
	if (step_size != NULL)
	  *step_size = build_int_cst (integer_type_node,
				      elem_fn_values->linear_steps[ii]);
	return TYPE_LINEAR;
      }
    
  for (ii = 0; ii < elem_fn_values->no_uvars; ii++)
    if (elem_fn_values->uniform_location[ii] == parm_no)
      return TYPE_UNIFORM;
    
  return TYPE_NONE;
}
  
  
/* Returns the parm_type (whether TYPE_LINEAR or TYPE_UNIFORM) for the
   parameter (indicated by OP).  If the return value is TYPE_LINEAR, then
   the *STEP_SIZE is set with the appropriate step-size.  */

enum elem_fn_parm_type
find_elem_fn_parm_type (gimple stmt, tree op, tree *step_size)
{
  tree fndecl, parm = NULL_TREE;
  int ii, nargs;
  enum elem_fn_parm_type return_type = TYPE_NONE;
  
  if (gimple_code (stmt) != GIMPLE_CALL)
    return TYPE_NONE;

  fndecl = gimple_call_fndecl (stmt);
  gcc_assert (fndecl);

  nargs = gimple_call_num_args (stmt);

  for (ii = 0; ii < nargs; ii++)
    {
      parm = gimple_call_arg (stmt, ii);
      if (op == parm)
	{
	  return_type = find_elem_fn_parm_type_1 (fndecl, ii, step_size);
	  return return_type;
	}
    }
  return return_type;
}

/* This function will return the appropriate cloned for the vectorlength
   (set by VECTYPE_OUT) named for the function whose scalar name is indicated
   by OLD_FNDECL.  */ 

tree
find_elem_fn_name (location_t loc, tree old_fndecl, tree vectype_out, 
		   tree vectype_in ATTRIBUTE_UNUSED, enum vect_def_type *dt,
		   size_t n_args, enum elem_fn_parm_type *parm_type_array)
{
  elem_fn_info *elem_fn_values = NULL;
  tree new_fndecl = NULL_TREE, arg_type = NULL_TREE;
  size_t no_entries = 0, ii = 0, jj = 0, kk = 0;
  char *prefix = NULL;
  char warning_string[90];
  bool *parms_ok = NULL, all_ok = false;
  size_t vlength_vec = (size_t) TYPE_VECTOR_SUBPARTS (vectype_out);

  parms_ok = XNEWVEC (bool, n_args);
  gcc_assert (parms_ok);
 
  elem_fn_values = extract_elem_fn_values (old_fndecl, &no_entries);
  for (ii = 0; ii < no_entries; ii++)
    {
      for (jj = 0; jj < n_args; jj++)
	parms_ok[jj] = false; /* Init. all values to not-found/false.  */
      
      if (elem_fn_values[ii].vectorlength == (int) vlength_vec)
	{
	  for (jj = 0; jj < n_args; jj++)
	    {
	      /* If the definition type is a constant definition or an
		 external definition,  then we have a uniform scenario.
		 
		 NOTE: We also check for a match for private because if the
		 user gives us a private as an option, then we might as well
		 use it, albeit it will be slightly less optimized, but better
		 than not using a vector version.
	      */
	      if (dt[jj] == vect_constant_def || dt[jj] == vect_external_def)
		{
		  for (kk = 0; kk < (size_t) elem_fn_values[ii].no_uvars; kk++)
		    if (jj == (size_t) elem_fn_values[ii].uniform_location[kk])
		      {
			parms_ok[jj] = true;
			parm_type_array[jj] = TYPE_UNIFORM;
		      }
		  for (kk = 0; kk < (size_t) elem_fn_values[ii].no_pvars; kk++)
		    if (jj == (size_t) elem_fn_values[ii].private_location[kk])
		      {
			parms_ok[jj] = true;
			parm_type_array[jj] = TYPE_NONE;
		      }
		}
	      if (dt[jj] == vect_internal_def)
		for (kk = 0; kk < (size_t) elem_fn_values[ii].no_pvars; kk++)
		  if (jj == (size_t) elem_fn_values[ii].private_location[kk])
		    {
		      parms_ok[jj] = true;
		      parm_type_array[jj] = TYPE_NONE;
		    }
	      /* If the definition type is an induction definition, then we
		 have a linear scenario.  */
	      if (dt[jj] == vect_induction_def)
		for (kk = 0; kk < (size_t) elem_fn_values[ii].no_lvars; kk++)
		  if (jj == (size_t) elem_fn_values[ii].linear_location[kk]
		      && elem_fn_values[ii].linear_steps[kk] == 1)
		    {
		      parms_ok[jj] = true;
		      parm_type_array[jj] = TYPE_LINEAR;
		    }
		  
	    }
	  all_ok = true;
	  for (jj = 0; jj < n_args; jj++)
	    if (!parms_ok[jj])
	      all_ok = false;
	  if (all_ok)
	    break;
	}
    }
  if (!all_ok)
    return NULL;
  prefix = find_prefix (&elem_fn_values[ii], false);
  if (!prefix)
    {
      memset (warning_string, 90, 0);
      sprintf (warning_string, "elemental function's vectorlength choices "
	       "does not match the loop's vectorlength (%d)",
	       (int) TYPE_VECTOR_SUBPARTS (vectype_out));
      warning_at (loc, 0, (const char *) warning_string);
      return NULL_TREE;
    }
    
  new_fndecl = copy_node (rename_elem_fn (old_fndecl, prefix));
  TREE_TYPE (new_fndecl) = copy_node (TREE_TYPE (old_fndecl));

  TYPE_ARG_TYPES (TREE_TYPE (new_fndecl)) =
    copy_list (TYPE_ARG_TYPES (TREE_TYPE (new_fndecl)));
  
  for (arg_type = TYPE_ARG_TYPES (TREE_TYPE (new_fndecl));
       arg_type && arg_type != void_type_node;
       arg_type = TREE_CHAIN (arg_type))
    TREE_VALUE (arg_type) = vectype_out;
  
  if (TREE_TYPE (TREE_TYPE (new_fndecl)) != void_type_node)
    {
      TREE_TYPE (TREE_TYPE (new_fndecl)) =
	copy_node (TREE_TYPE (TREE_TYPE (new_fndecl)));
      TREE_TYPE (TREE_TYPE (new_fndecl)) = vectype_out;
      DECL_MODE (new_fndecl) = TYPE_MODE (vectype_out);
    }
  return new_fndecl;
}

/* Extracts all the elemental function's relevant information from the attribute
   of DECL.  The extracted information are returned in a structure of type
   ELEM_FN_INFO.  */

elem_fn_info *
extract_elem_fn_values (tree decl, size_t *no_entries)
{
  elem_fn_info *elem_fn_list = NULL;
  size_t arg_number = 0, ii = 0, jj = 0;
  tree ii_tree, jj_tree, kk_tree;
  tree decl_attr = DECL_ATTRIBUTES (decl);
  tree decl_ret_type;
  size_t entries = 0;

  if (!decl_attr)
    return NULL;

  for (ii_tree = decl_attr; ii_tree; ii_tree = TREE_CHAIN (ii_tree))
    {
      tree ii_purpose = TREE_PURPOSE (ii_tree);
      if (ii_purpose && TREE_CODE (ii_purpose) == IDENTIFIER_NODE
	  && !strcmp (IDENTIFIER_POINTER (ii_purpose), "vector"))
	entries++;
    }
  *no_entries = entries;
  if (entries == 0)
    return NULL;
  
  elem_fn_list = XNEWVEC (elem_fn_info, entries);
  gcc_assert (elem_fn_list);

  decl_ret_type = TREE_TYPE (decl);
  if (decl_ret_type)
    decl_ret_type = TREE_TYPE (decl_ret_type);

  for (ii_tree = decl_attr, ii = 0; ii_tree != NULL_TREE && ii < *no_entries;
       ii_tree = TREE_CHAIN (ii_tree), ii++)
    {
      tree ii_purpose = TREE_PURPOSE (ii_tree);
      tree ii_value = TREE_VALUE (ii_tree);
      elem_fn_list[ii].proc_type = NULL;
      elem_fn_list[ii].isa_type = NULL;
      elem_fn_list[ii].mask = USE_BOTH;
      elem_fn_list[ii].no_vlengths = 0;
      elem_fn_list[ii].no_uvars = 0;
      elem_fn_list[ii].no_lvars = 0;
      elem_fn_list[ii].no_pvars = 0;
      if (decl_ret_type && COMPLETE_TYPE_P (decl_ret_type)
	  && !VOID_TYPE_P (decl_ret_type))
	switch (compare_tree_int (TYPE_SIZE (decl_ret_type), 64))
	  {
	  case 0: /* This means they are equal.  */
	    elem_fn_list[ii].vectorlength = 2;
	    break;
	  case -1: /* This means it is less than 64.  */
	    elem_fn_list[ii].vectorlength = 4;
	    break;
	  default:
	    elem_fn_list[ii].vectorlength = 1;
	  }
  
      if (TREE_CODE (ii_purpose) == IDENTIFIER_NODE
	  && !strcmp (IDENTIFIER_POINTER (ii_purpose), "vector"))
	{
	  for (jj_tree = ii_value; jj_tree;
	       jj_tree = TREE_CHAIN (jj_tree))
	    {
	      tree jj_purpose = NULL_TREE, jj_value = TREE_VALUE (jj_tree);

	      /* This means we have a mask/nomask.  */
	      if (TREE_CODE (jj_value) == IDENTIFIER_NODE)
		{ 
		  if (!strcmp (IDENTIFIER_POINTER (jj_value), "mask"))
		    elem_fn_list[ii].mask = USE_MASK;		    
		  else if (!strcmp (IDENTIFIER_POINTER (jj_value),
				    "nomask"))
		    elem_fn_list[ii].mask = USE_NOMASK;
		  continue;
		}
	      else
		jj_purpose = TREE_PURPOSE (jj_value);
	      
	      if (TREE_CODE (jj_purpose) == IDENTIFIER_NODE
		  && !strcmp (IDENTIFIER_POINTER (jj_purpose), "processor"))
		{
		  for (kk_tree = TREE_VALUE (jj_value); kk_tree;
		       kk_tree = TREE_CHAIN (kk_tree))
		    {
		      tree kk_value = TREE_VALUE (kk_tree);
		      if (TREE_CODE (kk_value) == STRING_CST)
			elem_fn_list[ii].proc_type =
			  xstrdup (TREE_STRING_POINTER (kk_value));
		    }
		}
	      else if (TREE_CODE (jj_purpose) == IDENTIFIER_NODE
		       && !strcmp (IDENTIFIER_POINTER (jj_purpose),
				   "vectorlength"))
		{
		  for (kk_tree = TREE_VALUE (jj_value); kk_tree;
		       kk_tree = TREE_CHAIN (kk_tree))
		    {
		      tree kk_value = TREE_VALUE (kk_tree);
		      if (TREE_CODE (kk_value) == INTEGER_CST)
			{
			  elem_fn_list[ii].vectorlength =
			    (int) TREE_INT_CST_LOW (kk_value);
			  elem_fn_list[ii].no_vlengths++;
			}
		    }
		}
	      else if (TREE_CODE (jj_purpose) == IDENTIFIER_NODE
		       && !strcmp (IDENTIFIER_POINTER (jj_purpose),
				   "uniform"))
		{
		  for (kk_tree = TREE_VALUE (jj_value); kk_tree;
		       kk_tree = TREE_CHAIN (kk_tree))
		    {
		      int tmp = elem_fn_list[ii].no_uvars;
		      tree kk_value = TREE_VALUE (kk_tree);
		      elem_fn_list[ii].uniform_vars[tmp] =
			xstrdup (TREE_STRING_POINTER (kk_value));
		      elem_fn_list[ii].no_uvars++;
		    }
		}
	      else if (TREE_CODE (jj_purpose) == IDENTIFIER_NODE
		       && !strcmp (IDENTIFIER_POINTER (jj_purpose),
				   "linear"))
		{
		  for (kk_tree = TREE_VALUE (jj_value); kk_tree;
		       kk_tree = TREE_CHAIN (kk_tree))
		    {
		      int tmp =  elem_fn_list[ii].no_lvars;
		      tree kk_value = TREE_VALUE (kk_tree);
		      elem_fn_list[ii].linear_vars[tmp] =
			xstrdup (TREE_STRING_POINTER (kk_value));
		      kk_tree = TREE_CHAIN (kk_tree);
		      kk_value = TREE_VALUE (kk_tree);
		      elem_fn_list[ii].linear_steps[tmp] =
			TREE_INT_CST_LOW (kk_value);
		      elem_fn_list[ii].no_lvars++;
		    }
		}
	    }	    
	}
    }
  for (ii = 0; ii < *no_entries; ii++)
    {
      for (ii_tree = DECL_ARGUMENTS (decl); ii_tree;
	   ii_tree = DECL_CHAIN (ii_tree))
	{
	  bool already_found = false;
	  for (jj = 0; jj < (size_t) elem_fn_list[ii].no_uvars; jj++)
	    {
	      if (DECL_NAME (ii_tree)
		  && !strcmp (IDENTIFIER_POINTER (DECL_NAME (ii_tree)),
			      elem_fn_list[ii].uniform_vars[jj]))
		{
		  already_found = true;
		  elem_fn_list[ii].uniform_location[jj] = arg_number;
		}
	    }
	  for (jj = 0; jj < (size_t) elem_fn_list[ii].no_lvars; jj++)
	    {
	      if (DECL_NAME (ii_tree)
		  && !strcmp (IDENTIFIER_POINTER (DECL_NAME (ii_tree)),
			      elem_fn_list[ii].linear_vars[jj]))
		{
		  if (already_found)
		    {
		      error_at (EXPR_LOCATION (ii_tree),
				"variable %s defined in both uniform and"
				" linear clause",
				elem_fn_list[ii].linear_vars[jj]);
		      return NULL;
		    }
		  else
		    {
		      already_found = true;
		      elem_fn_list[ii].linear_location[jj] = arg_number;
		    }
		}
	    }
	  if (!already_found) /* This means this variable is a private.  */
	    elem_fn_list[ii].private_location[elem_fn_list[ii].no_pvars++] =
	      arg_number;
	  arg_number++;
	}

      elem_fn_list[ii].total_no_args = arg_number;
      if (elem_fn_list[ii].no_vlengths == 0)
	/* We have a default value if none is given.  */
	elem_fn_list[ii].no_vlengths = 1;
    }
  return elem_fn_list;
}

/* Returns true of function FNDECL has an attribute with "vector" in it, thus
   indicating it as an elemental function.  */

bool
is_elem_fn (tree fndecl)
{
  tree ii_tree;

  for (ii_tree = DECL_ATTRIBUTES (fndecl); ii_tree;
       ii_tree = TREE_CHAIN (ii_tree))
    {
      tree ii_value = TREE_PURPOSE (ii_tree);
      if (TREE_CODE (ii_value) == IDENTIFIER_NODE
	  && !strcmp (IDENTIFIER_POINTER (ii_value), "vector"))
	return true;
    }
  /* If we are here then we didn't find a vector keyword, so it is false.  */
  return false;
}
