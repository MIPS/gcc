/* Promote virtual operands to real operands by converting may-alias
   relations into must-alias relations.
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"

/* These RTL headers are needed for basic-block.h.  */
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-simple.h"
#include "tree-dump.h"
#include "timevar.h"


/* Local functions.  */
static void find_addressable_vars (sbitmap);
static void promote_var (tree, sbitmap);


/* Local variables.  */
static FILE *dump_file;
static int dump_flags;


/* This pass finds must-alias relations exposed by constant propagation of
   ADDR_EXPR values into INDIRECT_REF expressions.  */

void
tree_compute_must_alias (tree fndecl)
{
  size_t i;
  sbitmap addresses_needed, promoted_vars;

  timevar_push (TV_TREE_MUST_ALIAS);

  /* Initialize debugging dumps.  */
  dump_file = dump_begin (TDI_mustalias, &dump_flags);

  /* Initialize internal data structures.  */
  promoted_vars = sbitmap_alloc (num_referenced_vars);
  sbitmap_zero (promoted_vars);

  addresses_needed = sbitmap_alloc (num_referenced_vars);
  sbitmap_zero (addresses_needed);

  /* Find variables that still need to have their address taken.  */
  find_addressable_vars (addresses_needed);

  /* Now traverse the original list of addressable variables and remove
     those whose addresses are not needed anymore.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (call_clobbered_vars); i++)
    {
      tree var = VARRAY_TREE (call_clobbered_vars, i);
      
      /* We are only interested in disambiguating addressable locals.  */
      if (TREE_ADDRESSABLE (var)
	  /* FIXME Why exactly do we need to ignore pointers and arrays?  */
	  && !POINTER_TYPE_P (TREE_TYPE (var))
	  && TREE_CODE (TREE_TYPE (var)) != ARRAY_TYPE
	  && decl_function_context (var) == current_function_decl
	  && !TEST_BIT (addresses_needed, var_ann (var)->uid))
	promote_var (var, promoted_vars);
    }

  /* Run the SSA pass again if we need to rename new variables.  */
  if (sbitmap_first_set_bit (promoted_vars) >= 0)
    {
      /* Rename all variables in PROMOTED_VARS into SSA form.  */
      remove_all_phi_nodes_for (promoted_vars);
      rewrite_into_ssa (fndecl, promoted_vars);

      /* FIXME  Also remove the disambiguated variables from
	 CALL_CLOBBERED_VARS.  */
    }

  /* Free allocated memory.  */
  sbitmap_free (addresses_needed);
  sbitmap_free (promoted_vars);

  /* Debugging dumps.  */
  if (dump_file)
    {
      dump_function_to_file (fndecl, dump_file, dump_flags);
      dump_end (TDI_mustalias, dump_file);
    }

  timevar_pop (TV_TREE_MUST_ALIAS);
}


/* Find all the variables that need to remain marked addressable and
   variables that need to be renamed in a second SSA pass because of the
   propagation of ADDR_EXPR values into INDIRECT_REF expressions.

   Return a bitmap indexed by variable UID that will be set if for all
   variables that still need to be addressable.  */

static void
find_addressable_vars (sbitmap addresses_needed)
{
  basic_block bb;

  /* Since ADDR_EXPRs have been propagated into INDIRECT_REF expressions
     and folded from '*&VAR' into 'VAR', some statements will have operands
     that are not in SSA form.  Those variables are added to a list of
     variables to be renamed in a second SSA pass.  */
  FOR_EACH_BB (bb)
    {
      block_stmt_iterator si;

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  varray_type ops;
	  size_t i;
	  tree stmt = bsi_stmt (si);

	  get_stmt_operands (stmt);

	  ops = addresses_taken (stmt);
	  if (ops)
	    {
	      for (i = 0; i < VARRAY_ACTIVE_SIZE (ops); i++)
		{
		  tree var = VARRAY_TREE (ops, i);
		  SET_BIT (addresses_needed, var_ann (var)->uid);
		}
	    }
	  else
	    {
	      /* The statement doesn't take the address of any variable,
		 mark it modified so that any promoted variables in its
		 operands are renamed again.  FIXME: We should only mark
		 statements that have promoted variables, but at this point
		 we still don't know which (if any) variables can be
		 promoted.  */
	      modify_stmt (stmt);
	    }
	}
    }
}


/* Remove all the aliasing marks from variable VAR and mark it not
   addressable.  If the variable was an alias tag for other variables,
   remove it from those alias sets.  Finally, add VAR to the PROMOTED_VARS
   bitmap.  */

static void
promote_var (tree var, sbitmap promoted_vars)
{
  var_ann_t ann = var_ann (var);

  /* FIXME: Apparently we always need TREE_ADDRESSABLE for aggregate
     types.  Is this a backend quirk or do we actually need these?  */
  if (!AGGREGATE_TYPE_P (TREE_TYPE (var)))
    TREE_ADDRESSABLE (var) = 0;

  ann->may_aliases = NULL;
  ann->is_call_clobbered = 0;
  ann->may_alias_global_mem = 0;
  ann->may_point_to_global_mem = 0;

  /* If the variable was an alias tag, remove it from every variable that
     had it in its may-alias set.  */
  if (ann->is_alias_tag)
    {
      size_t i;

      for (i = 0; i < num_call_clobbered_vars; i++)
	{
	  size_t j, len;
	  tree aliased_var = call_clobbered_var (i);
	  var_ann_t aliased_ann = var_ann (aliased_var);
	  varray_type aliases = aliased_ann->may_aliases;

	  if (aliases == NULL)
	    continue;

	  len = VARRAY_ACTIVE_SIZE (aliases);
	  for (j = 0; j < len; j++)
	    {
	      if (var == VARRAY_TREE (aliases, j))
		{
		  if (j < len - 1)
		    VARRAY_TREE (aliases, j) = VARRAY_TREE (aliases, len - 1);
		  VARRAY_POP (aliases);
		}
	    }

	  /* Completely remove the may-alias array if it's empty.  */
	  if (VARRAY_ACTIVE_SIZE (aliases) == 0)
	    aliased_ann->may_aliases = NULL;
	}

      /* Clear the alias-tag mark from VAR.  */
      ann->is_alias_tag = 0;
    }

  /* Add VAR to the list of variables to rename.  */
  SET_BIT (promoted_vars, var_ann (var)->uid);

  /* Debugging dumps.  */
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Promoting variable ");
      print_generic_expr (dump_file, var, 0);
      fprintf (dump_file, " from virtual to real operands.\n");
    }
}
