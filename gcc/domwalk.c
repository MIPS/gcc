/* Generic dominator tree walker
   Copyright (C) 2003 Free Software Foundation, Inc.
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
#include "basic-block.h"
#include "tree-flow.h"
#include "domwalk.h"
#include "ggc.h"

/* This file implements a generic walker for dominator trees.  */

/* Recursively walk the dominator tree.  This is meant to provide a generic
   engine for performing transformations on the tree structure.  For
   example, it is currently used to:

     1. Perform SSA rewriting to convert the function into SSA form.

     2. Redundancy elimination and copy propagation.

   We expect other optimizations to be able to plug into this engine.

   WALK_DATA contains a set of callbacks to perform pass-specific
   actions during the dominator walk as well as a stack of block local
   data maintained during the dominator walk.

   BB is the basic block we are currently visiting.

   LAST is the last statement in our parent's block if our parent's block
   ended with a control structure.  Long term this argument should probably
   go away.  Once that happens this engine could also be used in the 
   RTL optimizers.

   Right now we have no pass-global data attached to the dominator walker
   datastructure.  That will probably change in the future.  */

void
walk_dominator_tree (struct dom_walk_data *walk_data,
		     basic_block bb,
		     tree last)
{
  bitmap children;
  void *bd = NULL;

  /* Callback to initialize the local data structure.  */
  if (walk_data->initialize_block_local_data)
    {
      /* First get some local data, reusing any local data pointer we may
	 have saved.  */
      if (VARRAY_ACTIVE_SIZE (walk_data->free_block_data) > 0)
	{
	  bd = VARRAY_TOP_GENERIC_PTR (walk_data->free_block_data);
	  VARRAY_POP (walk_data->free_block_data);
	}
      else
	{
	  bd = xcalloc (1, walk_data->block_local_data_size);
	}

      /* Push the local data into the local data stack.  */
      VARRAY_PUSH_GENERIC_PTR (walk_data->block_data_stack, bd);

      /* Call the initializer.  */
      walk_data->initialize_block_local_data (walk_data, bb, last);

    }

  /* Callback for operations to execute before we have walked the
     dominator children, but before we walk statements.  */
  if (walk_data->before_dom_children_before_stmts)
    (*walk_data->before_dom_children_before_stmts) (walk_data, bb, last);

  /* Statement walk before walking dominator children.  */
  if (walk_data->before_dom_children_walk_stmts)
    (*walk_data->before_dom_children_walk_stmts) (walk_data, bb, last);

  /* Callback for operations to execute before we have walked the
     dominator children, and after we walk statements.  */
  if (walk_data->before_dom_children_after_stmts)
    (*walk_data->before_dom_children_after_stmts) (walk_data, bb, last);

  /* Recursively call ourselves on the dominator children of BB.  */
  children = dom_children (bb);
  if (children)
    {
      tree clast;
      unsigned long i;

      /* If this block ends with a control statement, pass it down
	 so that we might reason with it.  */
      clast = last_stmt (bb);
      if (clast && !is_ctrl_stmt (clast))
	clast = NULL;

      EXECUTE_IF_SET_IN_BITMAP (children, 0, i,
	{
	  basic_block dest = BASIC_BLOCK (i);

	  /* The destination block may have become unreachable, in
	     which case there's no point in optimizing it.  */
	  if (dest->pred)
	    walk_dominator_tree (walk_data, dest, clast);
	});
    }

  /* Callback for operations to execute after we have walked the
     dominator children, but before we walk statements.  */
  if (walk_data->after_dom_children_before_stmts)
    (*walk_data->after_dom_children_before_stmts) (walk_data, bb, last);

  /* Statement walk after walking dominator children.  */
  if (walk_data->after_dom_children_walk_stmts)
    (*walk_data->after_dom_children_walk_stmts) (walk_data, bb, last);

  /* Callback for operations to execute after we have walked the
     dominator children and after we have walked statements.  */
  if (walk_data->after_dom_children_after_stmts)
    (*walk_data->after_dom_children_after_stmts) (walk_data, bb, last);

  if (walk_data->initialize_block_local_data)
    {
      /* And save the block data so that we can re-use it.  */
      VARRAY_PUSH_GENERIC_PTR (walk_data->free_block_data, bd);

      /* And finally pop the record off the block local data stack.  */
      VARRAY_POP (walk_data->block_data_stack);
    }
}

void
init_walk_dominator_tree (struct dom_walk_data *walk_data)
{
  if (walk_data->initialize_block_local_data)
    {
      VARRAY_GENERIC_PTR_INIT (walk_data->free_block_data, 2, "freelist ");
      VARRAY_GENERIC_PTR_INIT (walk_data->block_data_stack, 2, "block_data");
    }
  else
    {
      walk_data->free_block_data = NULL;
      walk_data->block_data_stack = NULL;
    }
}

void
fini_walk_dominator_tree (struct dom_walk_data *walk_data)
{
  if (walk_data->initialize_block_local_data)
    {
      while (VARRAY_ACTIVE_SIZE (walk_data->free_block_data) > 0)
	{
	  free (VARRAY_TOP_GENERIC_PTR (walk_data->free_block_data));
	  VARRAY_POP (walk_data->free_block_data);
	}
    }
}
