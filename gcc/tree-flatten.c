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

static void append (tree_cell *, tree, enum tree_container_note);
static void recreate_block_constructs (struct block_tree *);
static basic_block compact_to_block (struct block_tree *);
static void link_consec_statements (basic_block, basic_block);
static int dummy_verify_flow_info (void);

/* Allocate a new tree_cell for tree T with NOTE.  */
tree_cell
tree_cell_alloc (tree t, enum tree_container_note note)
{
  tree_cell nw = ggc_alloc (sizeof (struct tree_container));

  nw->stmt = t;
  nw->note = note;
  nw->prev = nw->next = NULL;

  return nw;
}

/* Appends statement STMT with NOTE to the end of list AFTER.  */
static void
append (tree_cell *after, tree stmt, enum tree_container_note note)
{
  tree_cell nw = tree_cell_alloc (stmt, note);

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
    case BIND_EXPR:
      {
	append (after, stmt, TCN_BIND);
	tree_flatten_statement (BIND_EXPR_BODY (stmt), after, enclosing_switch);
	append (after, NULL_TREE, TCN_UNBIND);
      }
      break;

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
      append (after, stmt, TCN_STATEMENT);
      break;

    case COND_EXPR:
      {
	tree label_then = build_new_label ();
	tree label_else = build_new_label ();
	tree label_after = build_new_label ();

	append (after, stmt, TCN_STATEMENT);

	append (after, build1 (LABEL_EXPR, void_type_node, label_then),
		TCN_STATEMENT);
	tree_flatten_statement (COND_EXPR_THEN (stmt), after, enclosing_switch);
	append (after, build1 (GOTO_EXPR, void_type_node, label_after),
		TCN_STATEMENT);
	
	append (after, build1 (LABEL_EXPR, void_type_node, label_else),
		TCN_STATEMENT);
	tree_flatten_statement (COND_EXPR_ELSE (stmt), after, enclosing_switch);

	append (after, build1 (LABEL_EXPR, void_type_node, label_after),
		TCN_STATEMENT);

	COND_EXPR_THEN (stmt) =
		build1 (GOTO_EXPR, void_type_node, label_then);
	COND_EXPR_ELSE (stmt) =
		build1 (GOTO_EXPR, void_type_node, label_else);
      }
      break;

    case SWITCH_EXPR:
      {
	tree body = SWITCH_BODY (stmt);
	tree case_label, last_case_label;
	tree label_end, dflt, dflt_goto;

	SWITCH_BODY (stmt) = NULL_TREE;
	append (after, stmt, TCN_STATEMENT);

	tree_flatten_statement (body, after, stmt);

	/* Now we have a chain of CASE_LABEL_EXPR + GOTOs in
	   SWITCH_BODY.  If there is not a default alternative,
	   add one.  */
	last_case_label = SWITCH_BODY (stmt);
	if (!last_case_label
	    || CASE_LOW (TREE_CHAIN (last_case_label)) != NULL_TREE)
	  {
	    case_label = build_new_label ();
	    label_end = build_new_label ();

	    append (after, build1 (LABEL_EXPR, void_type_node, label_end),
		    TCN_STATEMENT);

	    dflt = build (CASE_LABEL_EXPR, void_type_node,
			  NULL_TREE, NULL_TREE, case_label);
	    dflt_goto = build1 (GOTO_EXPR, void_type_node, label_end);
	    TREE_CHAIN (dflt_goto) = dflt;
	    TREE_CHAIN (dflt) = last_case_label;
	    last_case_label = dflt_goto;

	    if (SWITCH_LABELS (stmt))
	      {
		tree switch_labels = SWITCH_LABELS (stmt);
		tree new_switch_labels =
			make_tree_vec (TREE_VEC_LENGTH (switch_labels) + 1);
		int i;

		for (i = 0; i < TREE_VEC_LENGTH (switch_labels); i++)
		  TREE_VEC_ELT (new_switch_labels, i) =
			  TREE_VEC_ELT (switch_labels, i);
		TREE_VEC_ELT (new_switch_labels, i) = case_label;
		SWITCH_LABELS (stmt) = new_switch_labels;
	      }
	  }

	for (case_label = TREE_CHAIN (last_case_label);
	     case_label;
	     case_label = TREE_CHAIN (case_label))
	  last_case_label = build (COMPOUND_EXPR, void_type_node,
				   case_label, last_case_label);
	SWITCH_BODY (stmt) = last_case_label;
      }
      break;

    case CASE_LABEL_EXPR:
      {
	tree case_label = build_new_label ();
	tree goto_stmt = build1 (GOTO_EXPR, void_type_node, case_label);
	tree *where, chain = SWITCH_BODY (enclosing_switch);
	
	append (after, build1 (LABEL_EXPR, void_type_node, case_label),
		TCN_STATEMENT);

	/* Keep the default alternative first.  */
	if (!chain
	    || CASE_LOW (stmt) == NULL_TREE
	    || CASE_LOW (TREE_CHAIN (chain)) != NULL_TREE)
	  where = &SWITCH_BODY (enclosing_switch);
	else
	  where = &TREE_CHAIN (TREE_CHAIN (SWITCH_BODY (enclosing_switch)));
	TREE_CHAIN (goto_stmt) = stmt;
	TREE_CHAIN (stmt) = *where;
	*where = goto_stmt;
      }
      break;

    case TRY_FINALLY_EXPR:
      {
	tree label_after = build_new_label ();

	append (after, NULL_TREE, TCN_TRY);
	tree_flatten_statement (TREE_OPERAND (stmt, 0), after, stmt);
	append (after, build1 (GOTO_EXPR, void_type_node, label_after),
		TCN_STATEMENT);
	append (after, NULL_TREE, TCN_FINALLY);
	tree_flatten_statement (TREE_OPERAND (stmt, 1), after, stmt);
	append (after, NULL_TREE, TCN_FINALLY_END);
	append (after, build1 (LABEL_EXPR, void_type_node, label_after),
		TCN_STATEMENT);
      }
      break;

    case TRY_CATCH_EXPR:
      {
	tree label_after = build_new_label ();
	tree body;

	append (after, NULL_TREE, TCN_TRY);
	tree_flatten_statement (TREE_OPERAND (stmt, 0), after, stmt);
	append (after, build1 (GOTO_EXPR, void_type_node, label_after),
		TCN_STATEMENT);

	body = TREE_OPERAND (stmt, 1);
	if (TREE_CODE (body) == EH_FILTER_EXPR)
	  {
	    append (after, body, TCN_CATCH);
	    body = EH_FILTER_FAILURE (body);
	  }
	else if (TREE_CODE (expr_first (body)) == CATCH_EXPR)
	  {
	    tree_stmt_iterator si = tsi_start (&TREE_OPERAND (stmt, 1));

	    while (1)
	      {
		body = tsi_stmt (si);
		if (TREE_CODE (body) != CATCH_EXPR)
		  abort ();

		tsi_next (&si);
		if (tsi_end_p (si))
		  break;
		
		append (after, body, TCN_CATCH);
		tree_flatten_statement (CATCH_BODY (body), after, stmt);
		append (after, build1 (GOTO_EXPR, void_type_node, label_after),
			TCN_STATEMENT);
	      }
		
	    append (after, body, TCN_CATCH);
	    body = CATCH_BODY (body);
	  }
	else
	  append (after, NULL_TREE, TCN_CATCH);

	tree_flatten_statement (body, after, stmt);
	append (after, NULL_TREE, TCN_CATCH_END);
	append (after, build1 (LABEL_EXPR, void_type_node, label_after),
		TCN_STATEMENT);
      }
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

/* Compacts the whole block corresponding to NODE to single basic block with
   single statement. The basic block is returned.
   
   TODO -- before compacting the blocks, we should sort them so that the
   number of jumps is decreased, and to remove those jumps.  Concretely
   if we have a basic ending with COND_EXPR, we should try to put the
   else branch bb immediately after the block.  */
static basic_block
compact_to_block (struct block_tree *node)
{
  basic_block entry, last, act, next;
  edge e;
  block_stmt_iterator bsi;

  if (!node->outer)
    {
      entry = ENTRY_BLOCK_PTR->next_bb;
      last = EXIT_BLOCK_PTR->prev_bb;
    }
  else
    {
      /* Make the blocks of the node to be consecutive.  */
      if (node->entry)
	entry = node->entry;
      else
	{
	  FOR_EACH_BB (entry)
	    {
	      if (bb_ann (entry)->block == node)
		break;
	    }
	  if (entry == EXIT_BLOCK_PTR)
	    return NULL;
	}
      last = entry;

      for (act = ENTRY_BLOCK_PTR->next_bb; act != EXIT_BLOCK_PTR; act = next)
	{
	  next = act->next_bb;

	  if (act == entry)
	    {
	      next = last->next_bb;
	      continue;
	    }

	  if (bb_ann (act)->block != node)
	    continue;

	  tree_move_block_after (act, last, true);
	  last = act;
	}

      /* If we have a prescribed exit node, make it the last one.  */
      if (node->exit
	  && node->exit != last)
	{
	  tree_move_block_after (node->exit, last, true);
	  last = node->exit;
	}

#ifdef ENABLE_CHECKING
      /* The entry block should only be accessed from outside.  */
      if (node->entry)
	{
	  for (e = entry->pred; e; e = e->pred_next)
	    {
	      struct block_tree *block = bb_ann (e->src)->block;

	      if (block->level >= node->level)
		{
		  while (block->level > node->level)
		    block = block->outer;
		  if (block == node)
		    abort ();
		}
	    }
	}
#endif

      /* Ensure that the block is entered through fallthru or abnormal edges.
	 Do not do this for bind blocks, as it is irrelevant here.  */
      if (node->type != BT_BIND
	  && first_stmt (entry)
	  && TREE_CODE (first_stmt (entry)) == LABEL_EXPR)
	{
	  for (bsi = bsi_start (entry); !bsi_end_p (bsi); bsi_next (&bsi))
	    if (TREE_CODE (bsi_stmt (bsi)) != LABEL_EXPR)
	      break;
	      
	  e = tree_split_block (entry, bsi);

	  bb_ann (e->src)->block = node->outer;
	  entry = e->dest;
	}
    }

  link_consec_statements (entry, last);
  bb_ann (entry)->block = node->outer;
  node->entry = NULL;
  node->exit = NULL;

  return entry;
}

/* Recreates block structures for block tree NODE.  */
static void
recreate_block_constructs (struct block_tree *node)
{
  struct block_tree *son;
  basic_block bb_try, bb_finally, bb;
  block_stmt_iterator bsi;
  tree construct, handler;
  struct block_tree **hblock;
  tree block;

  /* This is handled in the corresponding try node.  */
  if (node->type == BT_FINALLY
      || node->type == BT_CATCH)
    return;

  for (son = node->subtree; son; son = son->next)
    recreate_block_constructs (son);

  switch (node->type)
    {
    case BT_TRY:
      /* The node was removed.  */
      if (!node->entry)
	return;

      /* Remove removed handlers from the list and compact the remaining
	 ones.  */
      for (hblock = &node->of; *hblock; )
	{
	  if (!(*hblock)->entry)
	    {
	      *hblock = (*hblock)->of;
	      continue;
	    }
	      
	  for (son = (*hblock)->subtree; son; son = son->next)
	    recreate_block_constructs (son);
	  (*hblock)->entry = compact_to_block (*hblock);

	  if ((*hblock)->type == BT_CATCH
	      && (*hblock)->bind)
	    {
	      bsi = bsi_last ((*hblock)->entry);
	      TREE_OPERAND ((*hblock)->bind, 1) = bsi_stmt (bsi);
	      bsi_replace (bsi, (*hblock)->bind);
	    }
	  hblock = &(*hblock)->of;
	}
      /* If all catch/finally handlers were removed, also cancel the
	 try block.  */
      if (!node->of)
	{
	  compact_to_block (node);
	  return;
	}

      bb_try = compact_to_block (node);

      bb_finally = node->of->entry;
      handler = last_stmt (bb_finally);
      bb_finally->head_tree = bb_finally->end_tree = NULL;
      delete_basic_block (bb_finally);

      for (son = node->of->of; son; son = son->of)
	{
	  bb_finally = son->entry;
	  handler = build (COMPOUND_EXPR, void_type_node,
			   handler, last_stmt (bb_finally));
	  bb_finally->head_tree = bb_finally->end_tree = NULL;
	  delete_basic_block (bb_finally);
	}

      bsi = bsi_last (bb_try);
      construct = build (node->of->type == BT_FINALLY
			 ? TRY_FINALLY_EXPR
			 : TRY_CATCH_EXPR, void_type_node,
			 bsi_stmt (bsi), handler);
      bsi_replace (bsi, construct);
      break;

    case BT_BIND:
      bb = compact_to_block (node);

      if (!bb)
	return;

      construct = node->bind;
      /* Do not create useless bind_exprs.  */
      if (construct
	  && (BIND_EXPR_VARS (construct)
	      || !node->outer
	      || ((block = BIND_EXPR_BLOCK (construct))
		  && BLOCK_ABSTRACT_ORIGIN (block)
		  && (TREE_CODE (BLOCK_ABSTRACT_ORIGIN (block))
		      == FUNCTION_DECL))))
	{
	  BIND_EXPR_BODY (construct) = last_stmt (bb);
	  TREE_SIDE_EFFECTS (construct) = 1;
	  bsi = bsi_last (bb);
	  bsi_replace (bsi, construct);
	}
      break;

    default:
      abort ();
    }
}

/* Dummy function for verify_flow_info.  */
static int
dummy_verify_flow_info ()
{
  return 0;
}

/* Recreate a simple block structure.  */
tree
tree_unflatten_statements ()
{
  basic_block bb;
  edge e, next;
  block_stmt_iterator bsi;
  tree stmts;
  int (*old_cfgh_verify_flow_info) (void);

  /* Cfg -- code correspondence is not going to work; still we would
     like to be able to check consistency of cfg.  A bit hacky.  */
  old_cfgh_verify_flow_info = cfg_hooks->cfgh_verify_flow_info;
  cfg_hooks->cfgh_verify_flow_info = dummy_verify_flow_info;

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

  recreate_block_constructs (block_tree);
  if (n_basic_blocks != 1)
    abort ();
  stmts = last_stmt (ENTRY_BLOCK_PTR->next_bb);
  if (stmts != first_stmt (ENTRY_BLOCK_PTR->next_bb))
    abort ();

  block_tree_free ();
  cfg_hooks->cfgh_verify_flow_info = old_cfgh_verify_flow_info;
  delete_tree_cfg ();

  return stmts;
}
