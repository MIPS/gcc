/* Loop optimizations over tree-ssa.
   Copyright (C) 2003 Free Software Foundation, Inc.
   
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
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "diagnostic.h"
#include "basic-block.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "tree-pass.h"
#include "timevar.h"
#include "cfgloop.h"
#include "flags.h"


/* The main entry into loop optimization pass.  PHASE indicates which dump file
   from the DUMP_FILES array to use when dumping debugging information.
   FNDECL is the current function decl.  */

static void
tree_ssa_loop_opt (void)
{
  struct loops *loops;

  /* Does nothing for now except for checking that we are able to build the
     loops.  */

  loops = loop_optimizer_init (tree_dump_file);
  loop_optimizer_finalize (loops,
			   (tree_dump_flags & TDF_DETAILS
			    ? tree_dump_file : NULL));
}

static bool
gate_loop (void)
{
  return flag_tree_loop != 0;
}

struct tree_opt_pass pass_loop = 
{
  "loop",				/* name */
  gate_loop,				/* gate */
  tree_ssa_loop_opt,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_LOOP,				/* tv_id */
  PROP_cfg,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_verify_ssa	/* todo_flags_finish */
};
