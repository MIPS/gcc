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
#include "tree-flow.h"
#include "tree-dchain.h"
#include "timevar.h"

/* Main entry point to the tree SSA transformation routines.  FNDECL is
   the FUNCTION_DECL node for the function to optimize.  */

void
optimize_function_tree (fndecl)
     tree fndecl;
{
  tree fnbody;
  FILE *dump_file;
  int dump_flags;

  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return;
  
  fnbody = DECL_SAVED_TREE (fndecl);

#if 0
  /* Build the doubly-linked lists so that we can delete nodes
     efficiently.  */
  double_chain_stmts (fnbody);
#endif

#if 0
  /* Transform BREAK_STMTs, CONTINUE_STMTs, SWITCH_STMTs and GOTO_STMTs.  */
  break_continue_elimination (fndecl);
  goto_elimination (fndecl);
#endif

  /* Initialize flow data.  */
  init_flow ();
  
  timevar_push (TV_TREE_CFG);
  build_tree_cfg (fnbody);
  timevar_pop (TV_TREE_CFG);

  /* Begin analysis optimization passes.  */
  if (n_basic_blocks > 0 && ! (errorcount || sorrycount))
    {
      timevar_push (TV_TREE_SSA);
      build_tree_ssa (fndecl);
      timevar_pop (TV_TREE_SSA);
      
      timevar_push (TV_TREE_PRE);
      if (flag_tree_pre)
	tree_perform_ssapre ();
      timevar_pop (TV_TREE_PRE);

      timevar_push (TV_TREE_CCP);
      if (flag_tree_ccp)
	tree_ssa_ccp (fndecl);
      timevar_pop (TV_TREE_CCP);
      
      timevar_push (TV_TREE_DCE);
      if (flag_tree_dce)
	tree_ssa_eliminate_dead_code (fndecl);
      timevar_pop(TV_TREE_DCE);
    }

#if 0
  /* Wipe out the back-pointes in the statement chain.  */
  double_chain_free (fnbody);
#endif

  /* Debugging dump after optimization.  */
  dump_file = dump_begin (TDI_optimized, &dump_flags);
  if (dump_file)
    {
      /* We never get here if the function body is empty,
	 see simplify_function_tree().  */ 
      fprintf (dump_file, "%s()\n", get_name (fndecl));

      if (dump_flags & TDF_RAW)
	dump_node (fnbody, TDF_SLIM | dump_flags, dump_file);
      else
	print_generic_stmt (dump_file, fnbody, dump_flags);
      fprintf (dump_file, "\n");

      dump_end (TDI_optimized, dump_file);
      dump_file = NULL;
    }

  /* Flush out flow graph and SSA data.  */
  delete_tree_ssa (fnbody);
  delete_tree_cfg ();
}
