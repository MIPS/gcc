/* Control and data flow functions for trees.
   Copyright (C) 2001, 2002, 2003 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "expr.h"
#include "diagnostic.h"
#include "basic-block.h"
#include "flags.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "timevar.h"

/* Rewrite a function tree to the SSA form and perform the SSA-based
   optimizations on it.  */

/* Main entry point to the tree SSA transformation routines.  FNDECL is the
   FUNCTION_DECL node for the function to optimize.  */
void
optimize_function_tree (tree fndecl)
{
  tree fnbody;
  struct tree_container head;
  tree_cell first, last;

  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return;

  fnbody = DECL_SAVED_TREE (fndecl);

  /* Flatten the trees.  */
  head.prev = head.next = NULL;
  last = &head;
  tree_flatten_statement (fnbody, &last, NULL_TREE);
  first = head.next;

  if (!first)
    {
      /* The function is empty, we are done.  */
      DECL_SAVED_TREE (fndecl) = build_empty_stmt ();
      return;
    }

  first->prev = NULL;

  /* Build the flowgraph.  */
  init_flow ();
  build_tree_cfg (first);

  /* Begin analysis and optimization passes.  */
  if (n_basic_blocks > 0 && ! (errorcount || sorrycount))
    {
      /* Initialize common SSA structures.  */
      init_tree_ssa ();

      /* Find all the variables referenced in the function.  */
      find_referenced_vars (fndecl);
      assign_vars_to_scopes ();

      /* Compute aliasing information for all the variables referenced in
	 the function.  */
      compute_may_aliases (fndecl);

      /* Rewrite the function into SSA form.  */
      rewrite_into_ssa (fndecl, NULL);

      if (flag_tree_dce)
	{
	  /* If the dominator optimizations propagated ADDR_EXPRs into
	     INDIRECT_REF expressions, we may be able to promote may-alias
	     into must-alias relations.  If DCE eliminated all the pointer
	     assignments that were taking the address of a local variable X,
	     we can now rename X as a non-aliased local.  */
	  if (0)
	    tree_ssa_dce (fndecl);
	  if (flag_tree_dom && flag_tree_must_alias)
	    tree_compute_must_alias (fndecl);
	}

      if (flag_tree_pre)
	tree_perform_ssapre (fndecl);

      if (flag_tree_ccp)
	tree_ssa_ccp (fndecl);

      if (flag_tree_copyprop)
	tree_ssa_copyprop (fndecl);

      if (0 && flag_tree_dce)
	tree_ssa_dce (fndecl);

#if defined ENABLE_CHECKING
      verify_flow_info ();
#endif

      /* Rewrite the function out of SSA form.  */
      rewrite_out_of_ssa (fndecl);
    }

  fnbody = tree_unflatten_statements ();
  DECL_SAVED_TREE (fndecl) = fnbody;

  /* Debugging dump after optimization.  */
  dump_function (TDI_optimized, fndecl);
}
