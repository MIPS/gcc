/* Removal of superfluous tree structures.
   Copyright (C) 2003 Free Software Foundation, Inc.

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

/* Emit the list of simple statements corresponding to STMT to chain whose
   end is AFTER.  */

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
#include "errors.h"
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "ggc.h"
#include "langhooks.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "timevar.h"
#include "tree-dump.h"
#include "toplev.h"
#include "except.h"
#include "cfgloop.h"

static void append (tree_cell *, tree);
static basic_block compact_to_block (void);
static void link_consec_statements (basic_block, basic_block);

/* Allocate a new tree_cell for tree T.  */
tree_cell
tree_cell_alloc (tree t)
{
  tree_cell nw = ggc_alloc (sizeof (struct tree_container));

  nw->stmt = t;
  nw->prev = nw->next = NULL;

  return nw;
}

/* Appends statement STMT to the end of list AFTER.  */
static void
append (tree_cell *after, tree stmt)
{
  tree_cell nw = tree_cell_alloc (stmt);

  nw->next = NULL;
  nw->prev = *after;
  (*after)->next = nw;
  *after = nw;
}

/* Removes superfluous tree nodes from STMT, places elemental statements
   to list AFTER.  ENCLOSING_SWITCH is set to the innermost SWITCH_EXPR
   statement we are processing.  */
void
tree_flatten_statement (tree stmt, tree_cell *after, tree enclosing_switch)
{
  switch (TREE_CODE (stmt))
    {
    case COMPOUND_EXPR:
      tree_flatten_statement (TREE_OPERAND (stmt, 0), after, enclosing_switch);
      tree_flatten_statement (TREE_OPERAND (stmt, 1), after, enclosing_switch);
      break;

    case NOP_EXPR:
      break;

    case ASM_EXPR:
    case RETURN_EXPR:
    case MODIFY_EXPR:
    case CALL_EXPR:
    case GOTO_EXPR:
    case LABEL_EXPR:
    case VA_ARG_EXPR:
    case RESX_EXPR:
    case COND_EXPR:
    case SWITCH_EXPR:
      append (after, stmt);
      break;

    default:
      print_node_brief (stderr, "", stmt, 0);
      abort ();
    }
}

/* Links all statements between FROM and TO into a single chain,
   adds the single created statement as whole contents of FROM and
   removes all other basic blocks between FROM and TO.  */
static void
link_consec_statements (basic_block from, basic_block to)
{
  tree top, *astmt;
  basic_block bb, tto;
  tree_cell first, last, act;
  block_stmt_iterator bsi;
  edge e, next;

  top = NULL_TREE;
  astmt = &top;

  /* Remove the unnecessary edges (all except the fallthru edges to the
     first block and from the last one).  */
  FOR_BB_BETWEEN (bb, from, to->next_bb, next_bb)
    {
      for (e = bb->succ; e; e = next)
	{
	  next = e->succ_next;
	  if (bb != to 
	      || !(e->flags & EDGE_FALLTHRU))
	    remove_edge (e);
	}

      for (e = bb->pred; e; e = next)
	{
	  next = e->pred_next;
	  if (bb != from
	      || !(e->flags & EDGE_FALLTHRU))
	    remove_edge (e);
	}
    }
  /* Redirect the last fallthru edge if there is one.  */
  if (to->succ)
    redirect_edge_pred (to->succ, from);
  
  FOR_BB_BETWEEN (bb, from, to->next_bb, next_bb)
    {
      if (bb->head_tree)
	break;
    }
  if (bb != to->next_bb)
    {
      first = bb->head_tree;
      last = bb->end_tree;
      bb->head_tree = bb->end_tree = NULL;

      FOR_BB_BETWEEN (bb, bb->next_bb, to->next_bb, next_bb)
	{
	  if (bb->head_tree)
	    {
	      last->next = bb->head_tree;
	      last = bb->end_tree;
	      bb->head_tree = bb->end_tree = NULL;
	    }
	}

      for (act = first; act->next; act = act->next)
	{
	  *astmt = build (COMPOUND_EXPR, void_type_node,
			  act->stmt, NULL_TREE);
	  TREE_SIDE_EFFECTS (*astmt) = 1;
	  astmt = &TREE_OPERAND (*astmt, 1);
	}
      *astmt = act->stmt;
    }
  else
    *astmt = build_empty_stmt ();

  tto = to->next_bb;
  while (from->next_bb != tto)
    delete_basic_block (from->next_bb);

  bsi = bsi_start (from);
  bsi_insert_after (&bsi, top, BSI_NEW_STMT);
}

/* Compacts the whole chain to a single basic block with single statement. The
   basic block is returned.
   
   TODO -- before compacting the blocks, we should sort them so that the
   number of jumps is decreased, and to remove those jumps.  Concretely
   if we have a basic ending with COND_EXPR, we should try to put the
   else branch bb immediately after the block.  */
static basic_block
compact_to_block ()
{
  basic_block entry, last;

  entry = ENTRY_BLOCK_PTR->next_bb;
  last = EXIT_BLOCK_PTR->prev_bb;

  link_consec_statements (entry, last);

  return entry;
}

/* Plug the statements back into toplevel BIND_EXPR.  */
tree
tree_unflatten_statements ()
{
  basic_block bb;
  edge e, next;
  block_stmt_iterator bsi;
  tree stmts;

  /* Remove abnormal edges; they are no longer needed nor used, and
     it is faster to remove them now than to handle them later.  */
  FOR_EACH_BB (bb)
    {
      for (e = bb->succ; e; e = next)
	{
	  next = e->succ_next;
	  if (e->flags & EDGE_ABNORMAL)
	    remove_edge (e);
	}
    }
 
  /* Ensure that fallthru edges from start and to exit are correct.  */
  for (e = ENTRY_BLOCK_PTR->succ; e; e = e->succ_next)
    if (e->flags & EDGE_FALLTHRU)
      break;
  if (e)
    tree_move_block_after (e->dest, ENTRY_BLOCK_PTR, true);
  for (e = EXIT_BLOCK_PTR->pred; e; e = e->pred_next)
    if (e->flags & EDGE_FALLTHRU)
      break;
  if (e && e->src != EXIT_BLOCK_PTR->prev_bb)
    {
      if (e->src == ENTRY_BLOCK_PTR->next_bb)
	{
	  /* The block cannot be on both places simultaneously, so split
	     it.  */
	  bsi = bsi_last (e->src);
	  if (!bsi_end_p (bsi))
	    bsi_next (&bsi);

	  tree_split_block (e->src, bsi);
	}
      tree_move_block_after (e->src, EXIT_BLOCK_PTR->prev_bb, true);
    }
  
  /* Eliminate fake fallthru edges.  */
  FOR_EACH_BB (bb)
    {
      tree_cleanup_block_edges (bb, true);
    }

  compact_to_block ();
  if (n_basic_blocks != 1)
    abort ();
  stmts = last_stmt (ENTRY_BLOCK_PTR->next_bb);
  if (stmts != first_stmt (ENTRY_BLOCK_PTR->next_bb))
    abort ();

  delete_tree_cfg ();

  return stmts;
}
