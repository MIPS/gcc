/* Control and data flow functions for trees.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
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
#include "tree-optimize.h"
#include "tree-flow.h"
#include "tree-alias-common.h"
#include "tree-dchain.h"

/* Local functions.  */
static void init_tree_flow PARAMS ((void));

/*  Main entry point to the tree SSA transformation routines.  FNDECL is
    the FUNCTION_DECL node for the function to optimize.  */

void
optimize_function_tree (fndecl)
     tree fndecl;
{
  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return;
  
  /* Build the doubly-linked lists so that we can delete nodes
     efficiently.  */
  double_chain_stmts (DECL_SAVED_TREE (fndecl));

#if 0
  /* Transform BREAK_STMTs, CONTINUE_STMTs, SWITCH_STMTs and GOTO_STMTs.  */
  break_continue_elimination (fndecl);
  goto_elimination (fndecl);
#endif

  /* Build the SSA representation for the function.  */
  build_tree_ssa (fndecl); 

  /* Begin optimization passes.  */
  if (n_basic_blocks > 0 && ! (errorcount || sorrycount))
    {
      if (flag_tree_ssa_pre)
	tree_perform_ssapre ();

      if (flag_tree_ssa_ccp)
	tree_ssa_ccp (fndecl);
    }

  /* Wipe out the back-pointes in the statement chain.  */
  double_chain_free (DECL_SAVED_TREE (fndecl));

  /* Flush out flow graph and SSA data.  */
  delete_cfg ();
  delete_ssa ();
  referenced_symbols = NULL;
}


/*  Main entry point to the tree SSA analysis routines.  */

void
build_tree_ssa (fndecl)
     tree fndecl;
{
  /* Initialize flow data.  */
  init_flow ();
  init_tree_flow ();

  tree_find_basic_blocks (DECL_SAVED_TREE (fndecl));

  if (n_basic_blocks > 0 && ! (errorcount || sorrycount))
    {
      tree_find_varrefs ();
      tree_build_ssa ();
      tree_compute_rdefs ();    
    }
  if (flag_tree_points_to)
    create_alias_vars ();
}


/*  Initialize internal data structures and flags for the tree SSA pass.  */

static void
init_tree_flow ()
{
  VARRAY_TREE_INIT (referenced_symbols, 20, "Referenced symbols");

  /* If -Wuninitialized was used, set tree_warn_uninitialized and clear
     warn_uninitialized to avoid duplicate warnings.  */
  if (warn_uninitialized == 1)
    {
      tree_warn_uninitialized = 1;
      warn_uninitialized = 0;
    }
}
