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
#include "timevar.h"

/* Dump file and flags.  */
static FILE *tree_dump_file;
static int tree_dump_flags;

/* The main entry into loop optimization pass.  PHASE indicates which dump file
   from the DUMP_FILES array to use when dumping debugging information.
   FNDECL is the current function decl.  */

void
tree_ssa_loop_opt (tree fndecl ATTRIBUTE_UNUSED,
		   enum tree_dump_index phase ATTRIBUTE_UNUSED)
{
  struct loops *loops;

  /* Does nothing for now except for checking that we are able to build the
     loops.  */

  tree_dump_file = dump_begin (phase, &tree_dump_flags);

  timevar_push (TV_TREE_LOOP);
  loops = loop_optimizer_init (tree_dump_file);
  loop_optimizer_finalize (loops,
			   (tree_dump_flags & TDF_DETAILS) ? tree_dump_file : NULL);
  timevar_pop (TV_TREE_LOOP);

  if (tree_dump_file)
    {
      dump_function_to_file (fndecl, tree_dump_file, tree_dump_flags);
      dump_end (phase, tree_dump_file);
    }
}
