/* Control flow functions for trees.
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

/* This file contains functions for building the Control Flow Graph (CFG)
   for a function tree.  */

/* Statements in basic blocks are kept in double-linked list of tree_cell
   nodes.  On the beginning of the block there may be some labels, then
   statements follow.  The last statement is either control one (COND_EXPR
   or SWITCH_EXPR), the one that ends the block from some other reason
   or other.
   
   If it is COND_EXPR, its then and else branches contain GOTO_EXPR to the
   target location.  None of the outgoing edges is marked fallthru.
   
   If it is SWITCH_EXPR, its body contains a list (linked by COMPOUND_EXPRs)
   containing pairs of CASE_LABEL_EXPRs and GOTO_EXPRs (CASE_* macros below
   should be used to access them).  There always must be a default branch.
   None of the outgoing edges is marked fallthru.  There is a link case_edge
   from statement annotation of the CASE_LABEL_EXPRs to the appropriate edges.
   
   If it is other block-ending statement (exception handling), the normal
   flow edge is marked fallthru.  It does not have to really lead to the
   following block.
   
   If it is any other statement, there is exactly one fallthru edge to the
   following block.  (This is ugly.  The explicit gotos should not be
   here at all.  Still we cannot get rid of them completely -- computed gotos;
   so some special handling would be needed anyway, so let it be for now
   this way).
   
   Some of these invariants may be violated by optimization passes.
   New blocks may be created on fallthru edges, and edges may be removed
   when they become unreachable.  The function tree_cleanup_edges fixes
   these issues (but it is ugly too.  The optimizations should be fixed
   to keep the state consistent.)
   
   To handle bind_exprs and eh handling, there is kept a tree of blocks
   (block_tree).  Some basic blocks are entry blocks of such constructs.
   They must be kept unless the whole construct is to be removed.  One of
   the entry edges may be marked with EDGE_CONSTRUCT_ENTRY flag; it then
   is assumed to be the entry and it will be turned into fallthru when
   the constructs are recreated.  All other edges should come from
   inside of the construct.  */

/* Local declarations.  */

/* Initial capacity for the basic block array.  */
static const int initial_cfg_capacity = 20;

/* Dump files and flags.  */
static FILE *dump_file;		/* CFG dump file. */
static int dump_flags;		/* CFG dump flags.  */

/* Mapping of labels to their associated blocks.  This can greatly speed up
   building of the CFG in code with lots of gotos.  */
static varray_type label_to_block_map;

/* Set if we no longer want bb_for_stmt to be kept.  */
int no_bb_for_stmt;

/* CFG statistics.  */
struct cfg_stats_d
{
  long num_merged_cases;
  long num_merged_labels;
  long num_failed_bind_expr_merges;
};

static dominance_info pdom_info = NULL;

static struct cfg_stats_d cfg_stats;

struct block_tree *block_tree;
static struct block_tree *block_tree_curr;

static struct obstack block_tree_obstack;

static struct obstack block_ann_obstack;
static void *first_block_ann_obj = 0;

/* Basic blocks and flowgraphs.  */
static void make_blocks (tree_cell);
static inline void append_stmt_to_bb (tree_cell, basic_block);
extern void debug_tree_chain (tree_cell);
static void create_blocks_annotations (void);
static void create_block_annotation (basic_block);
static void free_blocks_annotations (void);
static void clear_blocks_annotations (void);

/* Edges.  */
static void make_edges (void);
static void make_ctrl_stmt_edges (basic_block);
static void make_exit_edges (basic_block);
static void make_cond_expr_edges (basic_block);
static void make_goto_expr_edges (basic_block);
static void make_switch_expr_edges (basic_block);
static bool tree_redirect_edge_and_branch (edge, basic_block);

/* Various helpers.  */
static struct block_tree *block_tree_alloc (struct block_tree *);
static inline bool stmt_starts_bb_p (tree, tree);
static inline bool stmt_ends_bb_p (tree);
static int tree_verify_flow_info (void);
static basic_block tree_make_forwarder_block (basic_block, int, int, edge, int);
static void replace_stmt (tree_cell, tree);
static struct loops *tree_loop_optimizer_init (FILE *);
static void tree_loop_optimizer_finalize (struct loops *, FILE *);

/* Flowgraph optimization and cleanup.  */
static void remove_bb (basic_block);
static void tree_merge_blocks (basic_block, basic_block);
static void remove_stmt (tree_cell, basic_block, bool);
static edge find_taken_edge_cond_expr (basic_block, tree);
static edge find_taken_edge_switch_expr (basic_block, tree);
static tree find_edge_goto (tree, edge);
static bool value_matches_label (edge, tree, tree, edge *);
static void tree_cleanup_edges (void);
static void dump_block_tree_id (FILE *, struct block_tree *);
static void assign_vars_to_scope (struct block_tree *);
static void remove_superfluous_labels (void);
static void thread_jumps (void);
static bool tree_forwarder_block_p (basic_block);
static void merge_seq_blocks (void);

/* Location to track pending stmt for edge insertion.  */
#define PENDING_STMT(e)	((tree)(e->insns))

/* Set the pending stmt field.  */
#define SET_PENDING_STMT(e, t)	((e->insns) = (rtx)t)

/* Accessors for switch statement case list.  */
#define CASE_END(STMT) (TREE_CODE (TREE_OPERAND (STMT, 1)) == GOTO_EXPR)
#define CASE_NEXT_RAW(STMT) (TREE_OPERAND (TREE_OPERAND (STMT, 1), 1))
#define CASE_NEXT(STMT) (CASE_END (STMT) ? NULL_TREE : CASE_NEXT_RAW (STMT))
#define CASE_GOTO(STMT)							\
  (CASE_END (STMT)							\
   ? TREE_OPERAND (STMT, 1)						\
   : TREE_OPERAND (TREE_OPERAND (STMT, 1), 0))
#define CASE_DESTINATION(STMT) (GOTO_DESTINATION (CASE_GOTO (STMT)))
#define CASE_CASE(STMT) (TREE_OPERAND (STMT, 0))
#define CASE_EDGE(STMT) (get_stmt_ann (CASE_CASE (STMT))->case_edge)
     
/* FIXME These need to be filled in with appropriate pointers.  But this
   implies an ABI change in some functions.  */
struct cfg_hooks tree_cfg_hooks = {
  tree_verify_flow_info,
  NULL,				/* dump_bb  */
  NULL,				/* create_basic_block  */
  tree_redirect_edge_and_branch, /* redirect_edge_and_branch  */
  NULL,				/* redirect_edge_and_branch_force  */
  remove_bb,			/* delete_basic_block  */
  NULL,				/* split_block  */
  NULL,				/* can_merge_blocks_p  */
  tree_merge_blocks,		/* merge_blocks  */
  tree_split_edge,		/* cfgh_split_edge  */
  tree_make_forwarder_block,	/* cfgh_make_forward_block  */
  tree_cleanup_edges,		/* cfgh_tidy_fallthru_edges  */
  tree_loop_optimizer_init,     /* cfgh_loop_optimizer_init  */
  tree_loop_optimizer_finalize  /* cfgh_loop_optimizer_finalize  */
};

/*---------------------------------------------------------------------------
			      Create basic blocks
---------------------------------------------------------------------------*/

/* Entry point to the CFG builder for trees.  FNBODY is the body of the
   function to process.  */

void
build_tree_cfg (tree_cell fnbody)
{
  timevar_push (TV_TREE_CFG);

  /* Register specific tree functions.  */
  tree_register_cfg_hooks ();

  /* Initialize the basic block array.  */
  n_basic_blocks = 0;
  last_basic_block = 0;
  VARRAY_BB_INIT (basic_block_info, initial_cfg_capacity, "basic_block_info");
  memset ((void *) &cfg_stats, 0, sizeof (cfg_stats));
  no_bb_for_stmt = false;

  /* Build a mapping of labels to their associated blocks.  */
  VARRAY_BB_INIT (label_to_block_map, initial_cfg_capacity,
		  "label to block map");

  ENTRY_BLOCK_PTR->next_bb = EXIT_BLOCK_PTR;
  EXIT_BLOCK_PTR->prev_bb = ENTRY_BLOCK_PTR;

  /* Find the basic blocks for the flowgraph.  Ignore empty functions.  */
  if (!fnbody)
    {
      timevar_pop (TV_TREE_CFG);
      return;
    }

  gcc_obstack_init (&block_tree_obstack);
  block_tree = NULL;
  block_tree_curr = NULL;

  /* Create block annotations.  */
  create_blocks_annotations ();

  make_blocks (fnbody);
  if (block_tree_curr)
    abort ();

  if (n_basic_blocks > 0)
    {
      /* Adjust the size of the array.  */
      VARRAY_GROW (basic_block_info, n_basic_blocks);

      /* Create the edges of the flowgraph.  */
      make_edges ();
    }

#if 0
  {
    /* The loop analyzer should be initialized right after the CFG
       construction because some loops will need latch blocks, and these
       need to be added before we do anything else.  If you use this
       structure you'll have to ensure that optimizers don't invalidate the
       information gathered in the loops structure via modifications to the
       underlying structure: the CFG.  */
    struct loops *loops = loop_optimizer_init (NULL);

    /* Once initialized, it's not really necessary to keep the loop data
       structures around.  They may be rescanned using flow_loops_find.  */
    loop_optimizer_finalize (loops, NULL);
  }
#endif

  timevar_pop (TV_TREE_CFG);

  cleanup_tree_cfg (true);
#if defined ENABLE_CHECKING
  verify_flow_info ();
#endif

  /* Debugging dumps.  */
  if (n_basic_blocks > 0)
    {
      /* Write the flowgraph to a dot file.  */
      dump_file = dump_begin (TDI_dot, &dump_flags);
      if (dump_file)
	{
	  tree_cfg2dot (dump_file);
	  dump_end (TDI_dot, dump_file);
	}

      /* Dump a textual representation of the flowgraph.  */
      dump_file = dump_begin (TDI_cfg, &dump_flags);
      if (dump_file)
	{
	  dump_tree_cfg (dump_file, dump_flags);
	  dump_end (TDI_cfg, dump_file);
	}
    }
}

/* Create annotations for all the basic blocks.  */

static void create_blocks_annotations (void)
{
  basic_block bb;
  static int initialized;

  if (!initialized)
    {
      gcc_obstack_init (&block_ann_obstack);
      initialized = 1;
    }
  /* Check whether TREE_ANNOTATIONS data are still allocated.  */
  else if (first_block_ann_obj)
    abort ();
  
  first_block_ann_obj = obstack_alloc (&block_ann_obstack, 0);
  
  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR, NULL, next_bb)
    create_block_annotation (bb);
}

/* Create annotations for a single basic block.  */

static void create_block_annotation (basic_block bb)
{
  /* Verify that the tree_annotations field is clear.  */
  if (bb->tree_annotations || !first_block_ann_obj)
    abort ();
  bb->tree_annotations = obstack_alloc (&block_ann_obstack, 
					sizeof (struct bb_ann_d));
  memset (bb->tree_annotations, 0, sizeof (struct bb_ann_d));
}

/* Free the annotations for all the basic blocks.  */

static void free_blocks_annotations (void)
{
  if (!first_block_ann_obj)
    abort ();
  obstack_free (&block_ann_obstack, first_block_ann_obj);
  first_block_ann_obj = NULL;

  clear_blocks_annotations ();  
}

/* Clear the annotations for all the basic blocks.  */

static void
clear_blocks_annotations (void)
{
  basic_block bb;

  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR, NULL, next_bb)
    bb->tree_annotations = NULL;
}

/* Build a flowgraph for the statements starting at the statement STMT.  */

static void
make_blocks (tree_cell stmt)
{
  tree_cell act, next;
  basic_block bb = NULL;
  tree prev_stmt;
  struct block_tree *eht;

  if (stmt == NULL
      || stmt->stmt == error_mark_node)
    return;

  for (act = stmt; act; act = next)
    {
      next = act->next;

      if (act->note != TCN_STATEMENT)
	{
	  if (bb && bb->end_tree)
	    bb->end_tree->next = NULL;
	  if (act->next)
	    act->next->prev = NULL;
	  bb = create_bb ();

	  switch (act->note)
	    {
	    case TCN_BIND:
	      eht = block_tree_alloc (block_tree_curr);
	      eht->entry = bb;
	      eht->bind = act->stmt;
	      block_tree_curr = eht;
	      bb_ann (bb)->block = block_tree_curr;
	      break;

	    case TCN_UNBIND:
	      bb_ann (bb)->block = block_tree_curr;
	      block_tree_curr = block_tree_curr->outer;
	      break;

	    default:
	      abort ();
	    }
	  bb = NULL;
	  continue;
	}

      prev_stmt = act->prev ? act->prev->stmt : NULL_TREE;
      if (!bb || stmt_starts_bb_p (act->stmt, prev_stmt))
	{
	  if (act->prev)
	    act->prev->next = NULL;
	  act->prev = NULL;
	  bb = create_bb ();
	  bb_ann (bb)->block = block_tree_curr;
	}

      append_stmt_to_bb (act, bb);

      if (stmt_ends_bb_p (act->stmt))
	{
	  bb = NULL;
	  act->next = NULL;
	}
    }
}

/* Add statement STMT to basic block BB and update BB's
   boundaries accordingly.  */

static inline void
append_stmt_to_bb (tree_cell stmt, basic_block bb)
{
  set_bb_for_stmt (stmt->stmt, bb);

  /* Update the head and tail of the block.  */
  if (bb->head_tree == NULL)
    bb->head_tree = stmt;

  bb->end_tree = stmt;
}

/* Create and return a new basic block.  */

basic_block
create_bb (void)
{
  basic_block bb;

  /* Create and initialize a new basic block.  */
  bb = alloc_block ();
  memset (bb, 0, sizeof (*bb));
  create_block_annotation (bb);

  bb->index = last_basic_block;
  bb->flags = BB_NEW;

  /* Add the new block to the linked list of blocks.  */
  link_block (bb, EXIT_BLOCK_PTR->prev_bb);

  /* Grow the basic block array if needed.  */
  if ((size_t) n_basic_blocks == VARRAY_SIZE (basic_block_info))
    VARRAY_GROW (basic_block_info, n_basic_blocks + (n_basic_blocks + 3) / 4);

  /* Add the newly created block to the array.  */
  BASIC_BLOCK (n_basic_blocks) = bb;
  n_basic_blocks++;
  last_basic_block++;

  return bb;
}

/*---------------------------------------------------------------------------
				 Edge creation
---------------------------------------------------------------------------*/

/* Join all the blocks in the flowgraph.  */

static void
make_edges (void)
{
  basic_block bb;
  struct block_tree *bti;
  edge e;

  /* Create an edge from entry to the first block with executable
     statements in it.  */
  make_edge (ENTRY_BLOCK_PTR, BASIC_BLOCK (0), EDGE_FALLTHRU);

  /* Traverse basic block array placing edges.  */
  FOR_EACH_BB (bb)
    {
      tree first = first_stmt (bb);
      tree last = last_stmt (bb);

      if (first)
        {
	  /* Edges for statements that always alter flow control.  */
	  if (is_ctrl_stmt (last))
	    make_ctrl_stmt_edges (bb);

	  /* Edges for statements that sometimes alter flow control.  */
	  if (is_ctrl_altering_stmt (last))
	    make_exit_edges (bb);
	}

      /* Finally, if no edges were created above, this is a regular
	 basic block that only needs a fallthru edge.  */
      if (bb->succ == NULL)
	make_edge (bb, bb->next_bb, EDGE_FALLTHRU);
    }

  /* We do not care about fake edges, so remove any that the CFG
     builder inserted for completeness.  */
  remove_fake_edges ();

  /* Mark special edges of constructs.  */
  for (bti = bti_start (); !bti_end_p (bti); bti_next (&bti))
    if (bti->entry)
      {
	for (e = bti->entry->pred; e; e = e->pred_next)
	  if (e->flags & EDGE_FALLTHRU)
	    break;

	if (e)
	  e->flags |= EDGE_CONSTRUCT_ENTRY;
      }
}

/* Create edges for control statement at basic block BB.  */

static void
make_ctrl_stmt_edges (basic_block bb)
{
  tree last = last_stmt (bb);

#if defined ENABLE_CHECKING
  if (last == NULL_TREE)
    abort();
#endif

  switch (TREE_CODE (last))
    {
    case GOTO_EXPR:
      make_goto_expr_edges (bb);

      /* If this is potentially a nonlocal goto, then this should also
	 create an edge to the exit block.   */
      if ((TREE_CODE (GOTO_DESTINATION (last)) == LABEL_DECL
	   && (decl_function_context (GOTO_DESTINATION (last))
	       != current_function_decl))
	  || (TREE_CODE (GOTO_DESTINATION (last)) != LABEL_DECL
	      && DECL_CONTEXT (current_function_decl)))
	make_edge (bb, EXIT_BLOCK_PTR, EDGE_ABNORMAL);
      break;

    case RETURN_EXPR:
      make_edge (bb, EXIT_BLOCK_PTR, 0);
      break;

    case COND_EXPR:
      make_cond_expr_edges (bb);
      break;

    case SWITCH_EXPR:
      make_switch_expr_edges (bb);
      break;

    case RESX_EXPR:
      make_eh_edges (last);
      /* Yet another NORETURN hack.  */
      if (bb->succ == NULL)
	make_edge (bb, EXIT_BLOCK_PTR, EDGE_FAKE);
      break;

    default:
      abort ();
    }
}

/* Checks whether STMT is potentially a nonlocal goto.  */
static bool
nonlocal_goto_p (tree stmt)
{
  return (TREE_CODE (GOTO_DESTINATION (stmt)) == LABEL_DECL
	  && (decl_function_context (GOTO_DESTINATION (stmt))
	       != current_function_decl))
	  || (TREE_CODE (GOTO_DESTINATION (stmt)) != LABEL_DECL
	      && DECL_CONTEXT (current_function_decl));
}

/* Create exit edges for statements in block BB that alter the flow of
   control.  Statements that alter the control flow are 'goto', 'return'
   and calls to non-returning functions.  */

static void
make_exit_edges (basic_block bb)
{
  tree last = last_stmt (bb);

  if (last == NULL_TREE)
    abort ();

  switch (TREE_CODE (last))
    {
    case CALL_EXPR:
      /* If this function receives a nonlocal goto, then we need to
	 make edges from this call site to all the nonlocal goto
	 handlers.  */
      if (FUNCTION_RECEIVES_NONLOCAL_GOTO (current_function_decl))
	make_goto_expr_edges (bb);

      /* If this statement has reachable exception handlers, then
	 create abnormal edges to them.  */
      make_eh_edges (last);

      /* Some calls are known not to return.  For such calls we create
	 a fake edge.

	 We really need to revamp how we build edges so that it's not
	 such a bloody pain to avoid creating edges for this case since
	 all we do is remove these edges when we're done building the
	 CFG.  */
      if (call_expr_flags (last) & (ECF_NORETURN | ECF_LONGJMP))
	{
	  make_edge (bb, EXIT_BLOCK_PTR, EDGE_FAKE);
	  return;
	}

      /* Don't forget the fall-thru edge.  */
      make_edge (bb, bb->next_bb, EDGE_FALLTHRU);
      break;

    case MODIFY_EXPR:
      /* A MODIFY_EXPR may have a CALL_EXPR on its RHS and the CALL_EXPR
	 may have an abnormal edge.  Search the RHS for this case and
	 create any required edges.  */
      if (TREE_CODE (TREE_OPERAND (last, 1)) == CALL_EXPR
	  && FUNCTION_RECEIVES_NONLOCAL_GOTO (current_function_decl))
	make_goto_expr_edges (bb);

      make_eh_edges (last);
      make_edge (bb, bb->next_bb, EDGE_FALLTHRU);
      break;

    default:
      abort ();
    }
}

/* Create the edges for a COND_EXPR starting at block BB.  */

static void
make_cond_expr_edges (basic_block bb)
{
  tree entry = last_stmt (bb);
  basic_block then_bb, else_bb;
  tree then_label, else_label;

#if defined ENABLE_CHECKING
  if (entry == NULL_TREE || TREE_CODE (entry) != COND_EXPR)
    abort ();
#endif

  then_label = GOTO_DESTINATION (COND_EXPR_THEN (entry));
  else_label = GOTO_DESTINATION (COND_EXPR_ELSE (entry));
  then_bb = VARRAY_BB (label_to_block_map, LABEL_DECL_INDEX (then_label));
  else_bb = VARRAY_BB (label_to_block_map, LABEL_DECL_INDEX (else_label));
  make_edge (bb, then_bb, EDGE_TRUE_VALUE);
  make_edge (bb, else_bb, EDGE_FALSE_VALUE);
}

/* Create the edges for a SWITCH_EXPR starting at block BB.  */

static void
make_switch_expr_edges (basic_block bb)
{
  tree entry = last_stmt (bb);
  basic_block label_bb;
  tree label, stmt;
  edge e;

#if defined ENABLE_CHECKING
  if (entry == NULL_TREE || TREE_CODE (entry) != SWITCH_EXPR)
    abort ();
#endif

  for (stmt = SWITCH_BODY (entry); stmt; stmt = CASE_NEXT (stmt))
    {
      label = CASE_DESTINATION (stmt);
      label_bb = VARRAY_BB (label_to_block_map, LABEL_DECL_INDEX (label));
      e = make_edge (bb, label_bb, 0);
      if (!e)
	e = find_edge (bb, label_bb);
      if (!e)
	abort ();
      CASE_EDGE (stmt) = e;
    }
}

/* Redirects edge E to basic block DEST.  */
static bool
tree_redirect_edge_and_branch (edge e, basic_block dest)
{
  block_stmt_iterator bsi;
  tree stmt, gto;
  basic_block bb = e->src;
  edge old;

  if (e->dest == dest)
    return true;

  if (e->flags & EDGE_ABNORMAL)
    return false;

  stmt = last_stmt (bb);
  bsi = bsi_last (bb);

  /* If some edge is already there, use it if possible.  */
  old = find_edge (bb, dest);
  if (old && (old->flags & EDGE_ABNORMAL))
    return false;

  if (!stmt
      || !stmt_ends_bb_p (stmt))
    {
      if (!(e->flags & EDGE_FALLTHRU))
	abort ();

      if (old)
	abort ();

      goto redirect;
    }

  if (TREE_CODE (stmt) == COND_EXPR
      || TREE_CODE (stmt) == GOTO_EXPR)
    {
      gto = find_edge_goto (stmt, e);
      GOTO_DESTINATION (gto) = tree_block_label (dest);
      goto redirect;
    }
 
  if (TREE_CODE (stmt) == SWITCH_EXPR)
    {
      /* This is more complicated, since there may be more gotos corresponding
	 to this edge.  */
      for (gto = SWITCH_BODY (stmt); gto; gto = CASE_NEXT (gto))
	if (CASE_EDGE (gto) == e)
	  {
	    CASE_DESTINATION (gto) = tree_block_label (dest);
	    if (old)
	      CASE_EDGE (gto) = old;
	  }

      goto redirect;
    }

  if (e->flags & EDGE_FALLTHRU)
    goto redirect;

  return false;

redirect:
  if (old)
    ssa_remove_edge (e);
  else
    {
      tree phi;
  
      for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
	remove_phi_arg (phi, e->src);
      redirect_edge_succ (e, dest);
    }
  tree_cleanup_block_edges (bb, false);
  return true;
}

basic_block
label_to_block (tree dest)
{
  return VARRAY_BB (label_to_block_map, LABEL_DECL_INDEX (dest));
}

/* Create edges for a goto statement at block BB.  */

static void
make_goto_expr_edges (basic_block bb)
{
  tree goto_t, dest;
  basic_block target_bb;
  int edge_flags;
  int for_call;

  goto_t = last_stmt (bb);

  /* If the last statement is not a GOTO (i.e., it is a RETURN_EXPR,
     CALL_EXPR or MODIFY_EXPR), then the edge is an abnormal edge resulting
     from a nonlocal goto.  */
  if (TREE_CODE (goto_t) != GOTO_EXPR)
    {
      dest = error_mark_node;
      for_call = 1;
      edge_flags = EDGE_ABNORMAL;
    }
  else
    {
      dest = GOTO_DESTINATION (goto_t);
      for_call = 0;

      /* A GOTO to a local label creates normal edges.  */
      if (TREE_CODE (dest) == LABEL_DECL && ! NONLOCAL_LABEL (dest))
	{
	  make_edge (bb, label_to_block (dest), 0);
	  return;
	}

      /* If we reach here, then we either have a computed goto or
	 a nonlocal goto.  */
      edge_flags = EDGE_ABNORMAL;
    }

  /* Look for the block starting with the destination label.  In the
     case of a computed goto, make an edge to any label block we find
     in the CFG.  */
  FOR_EACH_BB (target_bb)
    {
      block_stmt_iterator bsi;

      for (bsi = bsi_start (target_bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree target = bsi_stmt (bsi);

	  if (TREE_CODE (target) != LABEL_EXPR)
	    break;

	  /* Computed GOTOs.  Make an edge to every label block that has
	     been marked as a potential target for a computed goto.  */
	  if (TREE_CODE (dest) != LABEL_DECL
	      && FORCED_LABEL (LABEL_EXPR_LABEL (target))
	      && for_call == 0)
	    make_edge (bb, target_bb, edge_flags);

	  /* Nonlocal GOTO target.  Make an edge to every label block that has
	     been marked as a potential target for a nonlocal goto.  */
	  else if (TREE_CODE (dest) != LABEL_DECL
		   && NONLOCAL_LABEL (LABEL_EXPR_LABEL (target))
		   && for_call == 1)
	    make_edge (bb, target_bb, edge_flags);
	}
    }
}

/*---------------------------------------------------------------------------
			       Flowgraph analysis
---------------------------------------------------------------------------*/

/* Remove unreachable blocks and other miscellaneous clean up work.  */

void
cleanup_tree_cfg (int expensive)
{
  int orig_n_basic_blocks = n_basic_blocks;

  timevar_push (TV_TREE_CLEANUP_CFG);
  if (pdom_info != NULL)
    {
      free_dominance_info (pdom_info);
      pdom_info = NULL;
    }
  tree_cleanup_edges ();
  if (expensive)
    {
      remove_superfluous_labels ();
      thread_jumps ();
    }
  delete_unreachable_blocks ();
  merge_seq_blocks ();
  compact_blocks ();
#ifdef ENABLE_CHECKING
  verify_flow_info ();
#endif

  /* If we expunged any basic blocks, then the dominator tree is
     no longer valid.  */
  if (n_basic_blocks != orig_n_basic_blocks)
    {
      basic_block bb;
      
      FOR_EACH_BB (bb)
	clear_dom_children (bb);
    }

  timevar_pop (TV_TREE_CLEANUP_CFG);
}

/* Remove PHI nodes associated with basic block BB and all edges into
   and out of BB.  */
void
remove_phi_nodes_and_edges_for_unreachable_block (basic_block bb)
{
  /* Remove the edges into and out of this block.  */
  while (bb->pred != NULL)
    {
      tree phi;

      /* Since this block is no longer reachable, we can just delete all
         of its PHI nodes.  */
      phi = phi_nodes (bb);
      while (phi)
        {
	  tree next = TREE_CHAIN (phi);
	  remove_phi_node (phi, NULL_TREE, bb);
	  phi = next;
        }

      remove_edge (bb->pred);
    }

  /* Remove edges to BB's successors.  */
  while (bb->succ != NULL)
    ssa_remove_edge (bb->succ);
}

/* Remove block BB and its statements from the flowgraph.  */

static void
remove_bb (basic_block bb)
{
  block_stmt_iterator i;
  location_t loc;

  dump_file = dump_begin (TDI_cfg, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "Removing basic block %d\n", bb->index);
      dump_tree_bb (dump_file, "", bb, 0);
      fprintf (dump_file, "\n");
      dump_end (TDI_cfg, dump_file);
      dump_file = NULL;
    }

  loc.line = -1;

  /* Remove all the instructions in the block.  */
  for (i = bsi_last (bb); !bsi_end_p (i); i = bsi_last (bb))
    {
      tree stmt = bsi_stmt (i);

      if (get_lineno (stmt) >= 0)
	{
	  loc.file = get_filename (stmt);
	  loc.line = get_lineno (stmt);
	}
      bsi_remove (&i);
      set_bb_for_stmt (stmt, NULL);
    }

  /* If requested, give a warning that the first statement in the
     block is unreachable.  We walk statements backwards in the
     loop above, so the last statement we process is the first statement
     in the block.  */
  if (warn_notreached && loc.line >= 0)
    warning ("%Hwill never be executed", &loc);

  remove_phi_nodes_and_edges_for_unreachable_block (bb);

  /* If we have pdom information, then we must also make sure to
     clean up the dominance information.  */
  if (pdom_info)
    delete_from_dominance_info (pdom_info, bb);

  /* If it is a special block of a block structure, remove the
     reference.  */
  if (bb_ann (bb)->block->entry == bb)
    bb_ann (bb)->block->entry = NULL;

  /* Remove the basic block from the array.  */
  expunge_block (bb);
}

/* Merges block B into block A.  */
static void
tree_merge_blocks (basic_block a, basic_block b)
{
  block_stmt_iterator bsi;
  edge e;
  tree phi, stmt;

  /* Ensure that b follows a.  */
  if (a->next_bb != b)
    tree_move_block_after (b, a, false);

  if (!(a->succ->flags & EDGE_FALLTHRU))
    abort ();

  if (last_stmt (a)
      && stmt_ends_bb_p (last_stmt (a)))
    abort ();

  /* Turn phi nodes into assignments.  */
  bsi = bsi_last (a);
  for (phi = phi_nodes (b); phi; phi = TREE_CHAIN (phi))
    {
      tree src = PHI_ARG_DEF (phi, 0);
      tree dest = PHI_RESULT (phi);

      if (virtual_op_p (SSA_NAME_VAR (dest)))
	{
	  tree vdef;

	  stmt = build_empty_stmt ();
	  get_stmt_ann (stmt);
	  add_vdef (SSA_NAME_VAR (dest), stmt, NULL);
	  vdef = VARRAY_TREE (vdef_ops (stmt), 0);
	  VDEF_RESULT (vdef) = dest;
	  VDEF_OP (vdef) = src;
	}
      else
	stmt = build (MODIFY_EXPR, TREE_TYPE (dest), dest, src);

      SSA_NAME_DEF_STMT (dest) = stmt;
      bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);
    }

  /* Remove labels from B and set bb_for_stmt to A for other statements.  */
  for (bsi = bsi_start (b); !bsi_end_p (bsi);)
    {
      if (TREE_CODE (bsi_stmt (bsi)) == LABEL_EXPR)
	bsi_remove (&bsi);
      else
	{
	  set_bb_for_stmt (bsi_stmt (bsi), a);
	  bsi_next (&bsi);
	}
    }

  remove_edge (a->succ);

  /* Merge the chains.  */
  if (a->end_tree)
    a->end_tree->next = b->head_tree;
  else
    a->head_tree = b->head_tree;
  if (b->head_tree)
    {
      b->head_tree->prev = a->end_tree;
      a->end_tree = b->end_tree;
    }
  b->head_tree = b->end_tree = NULL;

  /* Redirect the edges.  */
  a->succ = b->succ;
  b->succ = NULL;
  for (e = a->succ; e; e = e->succ_next)
    e->src = a;

  if (bb_ann (b)->block->entry == b)
    abort ();

  /* Remove B.  */
  delete_basic_block (b);
}

/* Remove statement pointed by iterator I.  */

void
bsi_remove (block_stmt_iterator *i)
{
  tree_cell cell = bsi_cell (*i);

  bsi_next (i);

  remove_stmt (cell, i->bb, true);
}

/* Remove statement pointed by iterator I, but leave the annotations.  */

void
bsi_remove_leave_annot (block_stmt_iterator *i)
{
  tree_cell cell = bsi_cell (*i);

  bsi_next (i);

  remove_stmt (cell, i->bb, false);
}

/* Move the statement at FROM so it comes right after the statement at
   TO.  */
void 
bsi_move_after (block_stmt_iterator from, block_stmt_iterator to)
{
  tree stmt = bsi_stmt (from);
  bsi_remove_leave_annot (&from);
  bsi_insert_after (&to, stmt, BSI_SAME_STMT);
} 

/* Move the statement at FROM so it comes right before the statement
   at TO.  */
void 
bsi_move_before (block_stmt_iterator from, block_stmt_iterator to)
{
  tree stmt = bsi_stmt (from);
  bsi_remove_leave_annot (&from);
  bsi_insert_before (&to, stmt, BSI_SAME_STMT);
}

/* Move the statement at FROM to the end of basic block BB, */
void
bsi_move_to_bb_end (block_stmt_iterator from, basic_block bb)
{
  block_stmt_iterator last = bsi_last (bb);
  
  /* Have to check bsi_end_p because it could be an empty block.  */
  if (!bsi_end_p (last)
      && is_ctrl_stmt (bsi_stmt (last)))
    {
      bsi_move_before (from, last);
    }
  else
    {
      bsi_move_after (from, last);
    }
}

/* Replace the contents of a stmt with another. The replacement cannot be
   a COMPOUND_EXPR node, only a gimple stmt.  */

void
bsi_replace (block_stmt_iterator bsi, tree stmt)
{
  if (TREE_CODE (stmt) == COMPOUND_EXPR)
    abort ();

  replace_stmt (bsi_cell (bsi), stmt);
  modify_stmt (bsi_stmt (bsi));
}

/* Remove statement CELL in basic block BB.  Reset the annotations if
   REMOVE_ANNOTATIONS is true.  */

static void
remove_stmt (tree_cell cell, basic_block bb, bool remove_annotations)
{
  varray_type vdefs;
  size_t i;
  varray_type defs;
  tree stmt = cell->stmt;

  /* If the statement is a LABEL_EXPR, remove the LABEL_DECL from
     the symbol table.  */
  if (TREE_CODE (stmt) == LABEL_EXPR)
    remove_decl (LABEL_EXPR_LABEL (stmt), DECL_INITIAL (current_function_decl));

  if (remove_annotations)
    {
      /* If the statement is already in SSA form, mark all the
	 definitions made in the statement invalid.
	 
	 FIXME: We should probably traverse all the def-use edges
	 originating at this statement to update each use of the
	 definitions made here, but that is expensive and can easily
	 be checked by every pass by checking if SSA_NAME_DEF_STMT is
	 a nop.  */ 
      defs = def_ops (stmt);
      for (i = 0; defs && i < VARRAY_ACTIVE_SIZE (defs); i++)
	{
	  tree *def_p = VARRAY_GENERIC_PTR (defs, i);
	  if (TREE_CODE (*def_p) == SSA_NAME)
	    SSA_NAME_DEF_STMT (*def_p) = build_empty_stmt ();
	}

      vdefs = vdef_ops (stmt);
      for (i = 0; vdefs && i < VARRAY_ACTIVE_SIZE (vdefs); i++)
	{
	  tree vdef = VDEF_RESULT (VARRAY_TREE (vdefs, i));
	  if (TREE_CODE (vdef) == SSA_NAME)
	    SSA_NAME_DEF_STMT (vdef) = build_empty_stmt ();
	}
  
      stmt->common.ann = NULL;
    }

  /* The RHS of a MODIFY_EXPR has an annotation for the benefit of
     SSA-PRE.  Make sure to remove that annotation as well.

     We're somewhat conservative here in that we do not remove all
     annotations on the RHS of the MODIFY_EXPR, just those of type
     TREE_ANN_COMMON.  If the annotation had another type such
     as VAR_ANN other code may still need it and it'll get removed
     when we remove all the VAR_ANNs as we tear down the SSA form.  */
  if (TREE_CODE (stmt) == MODIFY_EXPR
      && TREE_OPERAND (stmt, 1)->common.ann
      && TREE_OPERAND (stmt, 1)->common.ann->common.type == TREE_ANN_COMMON)
    TREE_OPERAND (stmt, 1)->common.ann = NULL;

  if (cell->prev)
    cell->prev->next = cell->next;
  else
    bb->head_tree = cell->next;

  if (cell->next)
    cell->next->prev = cell->prev;
  else
    bb->end_tree = cell->prev;

  cell->prev = cell->next = NULL;
}

/* Given a control block BB and a constant value VAL, return the edge that
   will be taken out of the block.  If VAL does not match a unique edge,
   NULL is returned.  */

edge
find_taken_edge (basic_block bb, tree val)
{
  tree stmt;

  stmt = last_stmt (bb);

#if defined ENABLE_CHECKING
  if (stmt == NULL_TREE || !is_ctrl_stmt (stmt))
    abort ();
#endif

  /* If VAL is not a constant, we can't determine which edge might
     be taken.  */
  if (val == NULL || !really_constant_p (val))
    return NULL;

  if (TREE_CODE (stmt) == COND_EXPR)
    return find_taken_edge_cond_expr (bb, val);

  if (TREE_CODE (stmt) == SWITCH_EXPR)
    return find_taken_edge_switch_expr (bb, val);

  return bb->succ;
}


/* Given a constant value VAL and the entry block BB to a COND_EXPR
   statement, determine which of the two edges will be taken out of the
   block.  Return NULL if either edge may be taken.  */

static edge
find_taken_edge_cond_expr (basic_block bb, tree val)
{
  bool always_false;
  bool always_true;
  edge e;

  /* Determine which branch of the if() will be taken.  */
  always_false = (simple_cst_equal (val, integer_zero_node) == 1);
  always_true = (simple_cst_equal (val, integer_one_node) == 1);

  /* If VAL is a constant but it can't be reduced to a 0 or a 1, then
     we don't really know which edge will be taken at runtime.  This
     may happen when comparing addresses (e.g., if (&var1 == 4))  */
  if (!always_false && !always_true)
    return NULL;

  for (e = bb->succ; e; e = e->succ_next)
    if (((e->flags & EDGE_TRUE_VALUE) && always_true)
	|| ((e->flags & EDGE_FALSE_VALUE) && always_false))
      return e;

  return NULL;
}


/* Given a constant value VAL and the entry block BB to a SWITCH_EXPR
   statement, determine which edge will be taken out of the block.  Return
   NULL if any edge may be taken.  */

static edge
find_taken_edge_switch_expr (basic_block bb, tree val)
{
  edge e, default_edge;
  tree case_label, stmt;

  /* See if the switch() value matches one of the case labels.  */
  default_edge = NULL;
  for (stmt = SWITCH_BODY (last_stmt (bb)); stmt; stmt = CASE_NEXT (stmt))
    {
      e = CASE_EDGE (stmt);
      case_label = CASE_CASE (stmt);

      if (value_matches_label (e, val, case_label, &default_edge))
	return e;
    }

  /* If no case exists for the value used in the switch(), we use the
     default label.  */
  if (!default_edge)
    abort ();

  return default_edge;
}

/* Return true if VAL matches the LABEL.  If one of the LABEL is the DEFAULT
   label, DEST_EDGE is stored into *DEFAULT_EDGE_P to indicate that this edge
   goes to the DEFAULT label.  This is used by the caller when no other case
   label is found to match VAL.  */

static bool
value_matches_label (edge dest_edge, tree val, tree label, edge *default_edge_p)
{
  if (TREE_CODE (label) != CASE_LABEL_EXPR)
    abort ();

  /* Remember that we found a default label, just in case no other
     label matches the switch() value.  */
  if (CASE_LOW (label) == NULL_TREE)
    *default_edge_p = dest_edge;
  else
    {
      /* If we found a match, we are done.  */
      tree label_val = CASE_LOW (label);
      if (simple_cst_equal (label_val, val) == 1)
	return true;
    }

  return false;
}

/*---------------------------------------------------------------------------
			      Debugging functions
---------------------------------------------------------------------------*/

/* Dump a basic block to a file.  */

void
dump_tree_bb (FILE *outf, const char *prefix, basic_block bb, int indent)
{
  edge e;
  char *s_indent;
  block_stmt_iterator si;
  tree phi;

  s_indent = (char *) alloca ((size_t) indent + 1);
  memset ((void *) s_indent, ' ', (size_t) indent);
  s_indent[indent] = '\0';

  fprintf (outf, "%s%sBLOCK       %d\n", s_indent, prefix, bb->index);

  fprintf (outf, "%s%sPRED:      ", s_indent, prefix);
  for (e = bb->pred; e; e = e->pred_next)
    dump_edge_info (outf, e, 0);
  putc ('\n', outf);

  fprintf (outf, "%s%sSUCC:      ", s_indent, prefix);
  for (e = bb->succ; e; e = e->succ_next)
    dump_edge_info (outf, e, 1);
  putc ('\n', outf);

  fprintf (outf, "%s%sLOOP DEPTH: %d\n", s_indent, prefix, bb->loop_depth);

  fprintf (outf, "%s%sNEXT BLOCK: ", s_indent, prefix);
  if (bb->next_bb)
    fprintf (outf, "%d\n", bb->next_bb->index);
  else
    fprintf (outf, "nil\n");

  fprintf (outf, "%s%sPREV BLOCK: ", s_indent, prefix);
  if (bb->prev_bb)
    fprintf (outf, "%d\n", bb->prev_bb->index);
  else
    fprintf (outf, "nil\n");

  if (bb->tree_annotations)
    for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
      {
	fprintf (outf, "%s%s# ", s_indent, prefix);
	print_generic_stmt (outf, phi, 0);
      }

  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      fprintf (outf, "%s%s%d  ", s_indent, prefix, get_lineno (bsi_stmt (si)));
      print_generic_stmt (outf, bsi_stmt (si), 0);
      fprintf (outf, "\n");
    }
}


/* Dump a basic block on stderr.  */

void
debug_tree_bb (basic_block bb)
{
  dump_tree_bb (stderr, "", bb, 0);
}

/* Dump a basic block N on stderr.  */

basic_block
debug_tree_bb_n (int n)
{
  debug_tree_bb (BASIC_BLOCK (n));
  return BASIC_BLOCK (n);
}	 

/* Dump the CFG on stderr.

   FLAGS are the same used by the tree dumping functions
   (see TDF_* in tree.h).  */

void
debug_tree_cfg (int flags)
{
  dump_tree_cfg (stderr, flags);
}


/* Dump the program showing basic block boundaries on the given FILE.

   FLAGS are the same used by the tree dumping functions (see TDF_* in
   tree.h).  */

void
dump_tree_cfg (FILE *file, int flags)
{
  basic_block bb;

  if (flags & TDF_DETAILS)
    {
      const char *funcname
	= (*lang_hooks.decl_printable_name) (current_function_decl, 2);

      fputc ('\n', file);
      fprintf (file, ";; Function %s\n\n", funcname);
      fprintf (file, ";; \n%d basic blocks, %d edges, last basic block %d.\n",
	       n_basic_blocks, n_edges, last_basic_block);

      FOR_EACH_BB (bb)
	{
	  dump_tree_bb (file, "", bb, 0);
	  fputc ('\n', file);
	}
    }

  if (flags & TDF_STATS)
    dump_cfg_stats (file);

  if (n_basic_blocks > 0)
    dump_cfg_function_to_file (current_function_decl, file, flags|TDF_BLOCKS);
}

/* Dumps function FN to FILE, with details given by FLAGS.  Function body is
   taken from cfg.  */
void
dump_cfg_function_to_file (tree fn, FILE *file, int flags)
{
  basic_block bb;
  tree arg, phi;
  block_stmt_iterator si;
  edge e;
  int show_bb_headers = flags & TDF_BLOCKS;

  flags &= ~TDF_BLOCKS;

  fprintf (file, "\n;; Function %s",
	    (*lang_hooks.decl_printable_name) (fn, 2));
  fprintf (file, " (%s)\n",
	    IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (fn)));
  fprintf (file, "\n");

  if (show_bb_headers)
    {
      fprintf (file, "\n# Block tree:\n");
      dump_block_tree (file, 3, block_tree);
      fprintf (file, "\n");
    }

  fprintf (file, "%s (", (*lang_hooks.decl_printable_name) (fn, 2));

  arg = DECL_ARGUMENTS (fn);
  while (arg)
    {
      print_generic_expr (file, arg, 0);
      if (TREE_CHAIN (arg))
	fprintf (file, ", ");
      arg = TREE_CHAIN (arg);
    }
  fprintf (file, ")\n{\n");

  FOR_EACH_BB (bb)
    {
      if (show_bb_headers)
	{
	  fprintf (file, "# BLOCK %d\n# CONSTRUCT ", bb->index);
	  dump_block_tree_id (file, bb_ann (bb)->block);
	  if (bb_ann (bb)->block->entry == bb)
	    fprintf (file, "(entry)");
	  fprintf (file, "\n# PRED");
	  for (e = bb->pred; e; e = e->pred_next)
	    dump_edge_info (file, e, 0);
	  putc ('\n', file);
	}
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  fprintf (file, "\t# ");
	  print_generic_stmt (file, phi, flags);
	  fprintf (file, "\n");
	}

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  fprintf (file, "%d\t", get_lineno (bsi_stmt (si)));
	  print_generic_stmt (file, bsi_stmt (si), flags & ~TDF_VOPS);
	  fprintf (file, "\n");
	}

      if (show_bb_headers)
	{
	  fprintf (file, "# SUCC");
	  for (e = bb->succ; e; e = e->succ_next)
	    dump_edge_info (file, e, 1);
	  fprintf (file, "\n\n");
	}
    }
  fprintf (file, "}\n\n");
}

/* Dump CFG statistics on FILE.  */

void
dump_cfg_stats (FILE *file)
{
  static long max_num_merged_cases = 0;
  static long max_num_merged_labels = 0;
  unsigned long size, total = 0;
  long n_edges;
  basic_block bb;
  const char * const fmt_str   = "%-30s%-13s%12s\n";
  const char * const fmt_str_1 = "%-30s%13lu%11lu%c\n";
  const char * const fmt_str_3 = "%-43s%11lu%c\n";
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);


  fprintf (file, "\nCFG Statistics for %s\n\n", funcname);

  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, fmt_str, "", "  Number of  ", "Memory");
  fprintf (file, fmt_str, "", "  instances  ", "used ");
  fprintf (file, "---------------------------------------------------------\n");

  size = n_basic_blocks * sizeof (struct basic_block_def);
  total += size;
  fprintf (file, fmt_str_1, "Basic blocks", n_basic_blocks, SCALE (size),
	   LABEL (size));

  n_edges = 0;
  FOR_EACH_BB (bb)
    {
      edge e;
      for (e = bb->succ; e; e = e->succ_next)
	n_edges++;
    }
  size = n_edges * sizeof (struct edge_def);
  total += size;
  fprintf (file, fmt_str_1, "Edges", n_edges, SCALE (size), LABEL (size));

  size = n_basic_blocks * sizeof (struct bb_ann_d);
  total += size;
  fprintf (file, fmt_str_1, "Basic block annotations", n_basic_blocks,
	   SCALE (size), LABEL (size));

  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, fmt_str_3, "Total memory used by CFG data", SCALE (total),
	   LABEL (total));
  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, "\n");

  if (cfg_stats.num_merged_labels > max_num_merged_labels)
    max_num_merged_labels = cfg_stats.num_merged_labels;

  fprintf (file, "Coalesced label blocks: %ld (Max so far: %ld)\n",
	   cfg_stats.num_merged_labels, max_num_merged_labels);


  if (cfg_stats.num_merged_cases > max_num_merged_cases)
    max_num_merged_cases = cfg_stats.num_merged_cases;

  fprintf (file, "Coalesced case label blocks: %ld (Max so far: %ld)\n",
	   cfg_stats.num_merged_cases, max_num_merged_cases);

  fprintf (file, "Number of unnecessary blocks created due to lexical scopes: %ld (%.0f%%)\n",
	   cfg_stats.num_failed_bind_expr_merges,
	   PERCENT (cfg_stats.num_failed_bind_expr_merges, n_basic_blocks));

  fprintf (file, "\n");
}


/* Dump CFG statistics on stderr.  */

void
debug_cfg_stats (void)
{
  dump_cfg_stats (stderr);
}


/* Dump the flowgraph to a .dot FILE.  */

void
tree_cfg2dot (FILE *file)
{
  edge e;
  basic_block bb;
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);

  /* Write the file header.  */
  fprintf (file, "digraph %s\n{\n", funcname);

  /* Write blocks and edges.  */
  for (e = ENTRY_BLOCK_PTR->succ; e; e = e->succ_next)
    {
      fprintf (file, "\tENTRY -> %d", e->dest->index);

      if (e->flags & EDGE_FAKE)
	fprintf (file, " [weight=0, style=dotted]");

      fprintf (file, ";\n");
    }
  fputc ('\n', file);

  FOR_EACH_BB (bb)
    {
      enum tree_code head_code, end_code;
      const char *head_name, *end_name;
      int head_line = 0;
      int end_line = 0;
      tree first = first_stmt (bb);
      tree last = last_stmt (bb);

      if (first)
        {
	  head_code = TREE_CODE (first);
	  head_name = tree_code_name[head_code];
	  head_line = get_lineno (bb->head_tree->stmt);
	}
      else
        head_name = "no-statement";

      if (last)
        {
	  end_code = TREE_CODE (last);
	  end_name = tree_code_name[end_code];
	  end_line = get_lineno (bb->end_tree->stmt);
	}
      else
        end_name = "no-statement";


      fprintf (file, "\t%d [label=\"#%d\\n%s (%d)\\n%s (%d)\"];\n",
	       bb->index, bb->index, head_name, head_line, end_name,
	       end_line);

      for (e = bb->succ; e; e = e->succ_next)
	{
	  if (e->dest == EXIT_BLOCK_PTR)
	    fprintf (file, "\t%d -> EXIT", bb->index);
	  else
	    fprintf (file, "\t%d -> %d", bb->index, e->dest->index);

	  if (e->flags & EDGE_FAKE)
	    fprintf (file, " [weight=0, style=dotted]");

	  fprintf (file, ";\n");
	}

      if (bb->next_bb != EXIT_BLOCK_PTR)
	fputc ('\n', file);
    }

  fputs ("}\n\n", file);
}



/*---------------------------------------------------------------------------
			     Miscellaneous helpers
---------------------------------------------------------------------------*/


/* Return true if T represents a stmt that always transfers control.  */

bool
is_ctrl_stmt (tree t)
{
  return (TREE_CODE (t) == COND_EXPR
	  || TREE_CODE (t) == SWITCH_EXPR
	  || TREE_CODE (t) == GOTO_EXPR
	  || TREE_CODE (t) == RETURN_EXPR
	  || TREE_CODE (t) == RESX_EXPR);
}

/* Return true if T is a stmt that may or may not alter the flow of control
   (i.e., a call to a non-returning function).  */

bool
is_ctrl_altering_stmt (tree t)
{
  tree call = t;

#if defined ENABLE_CHECKING
  if (t == NULL)
    abort ();
#endif

  switch (TREE_CODE (t))
    {
    case MODIFY_EXPR:
      /* A MODIFY_EXPR with a rhs of a call has the characteristics
	 of the call.  */
      call = TREE_OPERAND (t, 1);
      if (TREE_CODE (call) != CALL_EXPR)
	break;
      /* FALLTHRU */

    case CALL_EXPR:
      /* A CALL_EXPR alters flow control if the current function has
	 nonlocal labels.  */
      if (FUNCTION_RECEIVES_NONLOCAL_GOTO (current_function_decl))
	return true;

      /* A CALL_EXPR also alters flow control if it does not return.  */
      if (call_expr_flags (call) & (ECF_NORETURN | ECF_LONGJMP))
	return true;
      break;

    default:
      return false;
    }

  /* If a statement can throw, it alters control flow.  */
  return tree_can_throw_internal (t);
}


/* Return flags associated with the function called by T (see ECF_* in
   tree.h)  */

int
call_expr_flags (tree t)
{
  int flags;
  tree decl = get_callee_fndecl (t);

  if (decl)
    flags = flags_from_decl_or_type (decl);
  else
    {
      t = TREE_OPERAND (t, 0);
      flags = flags_from_decl_or_type (TREE_TYPE (TREE_TYPE (t)));
    }

  return flags;
}


/* Return true if T is a computed goto.  */

bool
is_computed_goto (tree t)
{
  return (TREE_CODE (t) == GOTO_EXPR
          && TREE_CODE (GOTO_DESTINATION (t)) != LABEL_DECL);
}

/* Allocates a block_tree node with outer node OUTER.  */
static struct block_tree *
block_tree_alloc (struct block_tree *outer)
{
  struct block_tree *nw = obstack_alloc (&block_tree_obstack,
					 sizeof (struct block_tree));

  nw->outer = outer;
  nw->subtree = NULL;
  nw->entry = NULL;
  nw->bind = NULL_TREE;

  if (outer)
    {
      nw->next = outer->subtree;
      outer->subtree = nw;
      nw->level = outer->level + 1;
    }
  else
    {
      block_tree = nw;
      nw->next = NULL;
      nw->level = 0;
    }

  return nw;
}

/* Releases the block_tree obstack.  */
void
block_tree_free ()
{
  obstack_free (&block_tree_obstack, NULL);
}

/* Return true if T should start a new basic block.  PREV_T is the
   statement preceding T.  It is used when T is a label or a case label.
   Labels should only start a new basic block if their previous statement
   wasn't a label.  Otherwise, sequence of labels would generate
   unnecessary basic blocks that only contain a single label.  */

static inline bool
stmt_starts_bb_p (tree t, tree prev_t)
{
  enum tree_code code;

  if (t == NULL_TREE)
    return false;

  /* LABEL_EXPRs and CASE_LABEL_EXPRs start a new basic block only if the
     preceding statement wasn't a label of the same type.  This prevents
     the creation of consecutive blocks that have nothing but a single
     label.  */
  code = TREE_CODE (t);
  if (code == LABEL_EXPR || code == CASE_LABEL_EXPR)
    {
      /* Nonlocal and computed GOTO targets always start a new block.  */
      if (code == LABEL_EXPR
	  && (NONLOCAL_LABEL (LABEL_EXPR_LABEL (t))
	      || FORCED_LABEL (LABEL_EXPR_LABEL (t))))
	return true;

      if (prev_t && TREE_CODE (prev_t) == code)
	{
	  if (code == LABEL_EXPR)
	    cfg_stats.num_merged_labels++;
	  else
	    cfg_stats.num_merged_cases++;

	  return false;
	}
      else
	return true;
    }

  return false;
}


/* Return true if T should end a basic block.  */

static inline bool
stmt_ends_bb_p (tree t)
{
  return (is_ctrl_stmt (t));
}


/* Remove all the blocks and edges that make up the flowgraph.  */

void
delete_tree_cfg (void)
{
  if (n_basic_blocks > 0)
    free_blocks_annotations ();

  free_basic_block_vars (0);
}


/* Return the first statement in basic block BB, stripped of any NOP
   containers.  */

tree
first_stmt (basic_block bb)
{
  block_stmt_iterator i;

  if (bb == ENTRY_BLOCK_PTR
      || bb == EXIT_BLOCK_PTR)
    return NULL_TREE;

  i = bsi_start (bb);
  return (!bsi_end_p (i)) ? bsi_stmt (i) : NULL_TREE;
}

/* Finds a label in basic block BB or creates one if it does not exit.  */
tree
tree_block_label (basic_block bb)
{
  tree label = first_stmt (bb);

  if (!label || TREE_CODE (label) != LABEL_EXPR)
    {
      block_stmt_iterator first = bsi_start (bb);
      tree label_decl = build_new_label ();

      label = build1 (LABEL_EXPR, void_type_node, label_decl);
      bsi_insert_before (&first, label, BSI_NEW_STMT);
    }

  return LABEL_EXPR_LABEL (label);
}

/* Return the last statement in basic block BB.  */

tree
last_stmt (basic_block bb)
{
  block_stmt_iterator b;

  if (bb == ENTRY_BLOCK_PTR
      || bb == EXIT_BLOCK_PTR)
    return NULL_TREE;

  b = bsi_last (bb);
  return (!bsi_end_p (b)) ? bsi_stmt (b) : NULL_TREE;
}

/* Return the pointer to last statement in basic block BB.  */

tree *
last_stmt_ptr (basic_block bb)
{
  block_stmt_iterator b;

  b = bsi_last (bb);
  return (!bsi_end_p (b)) ? bsi_stmt_ptr (b) : NULL;
}

/* Similar to tsi_start() but initializes the iterator at the first
   statement in basic block BB which isn't an empty statement node.

   NULL is returned if there are no such statements.  */

block_stmt_iterator
bsi_start (basic_block bb)
{
  block_stmt_iterator i;

  i.bb = bb;
  if (bb && bb->index != INVALID_BLOCK)
    i.curr_stmt = bb->head_tree;
  else
    i.curr_stmt = NULL;

  return i;
}

/* This routine will return a block iterator which points to the last stmt in
   a basic block, if there is one.  */

block_stmt_iterator
bsi_last (basic_block bb)
{
  block_stmt_iterator i;

  i.bb = bb;
  if (bb && bb->index != INVALID_BLOCK)
    i.curr_stmt = bb->end_tree;
  else
    i.curr_stmt = NULL;

  return i;
}


/* Find the previous iterator value.  */

void
bsi_prev (block_stmt_iterator *i)
{
  i->curr_stmt = i->curr_stmt->prev;
}

/* Insert statement T into basic block BB.  */

void
set_bb_for_stmt (tree t, basic_block bb)
{
  stmt_ann_t ann;

  if (no_bb_for_stmt)
    return;

  /* If the statement is a label, add the label to block-to-labels map
     so that we can speed up edge creation for GOTO_EXPRs.  */
  if (TREE_CODE (t) == LABEL_EXPR)
    {
      LABEL_DECL_INDEX (LABEL_EXPR_LABEL (t))
	      = VARRAY_ACTIVE_SIZE (label_to_block_map);
      VARRAY_PUSH_BB (label_to_block_map, bb);
    }

  ann = get_stmt_ann (t);
  ann->bb = bb;
}


/* Insert routines.  */

/* This routine inserts a stmt after the stmt iterator passed in.
   The final parameter determines whether the statement iterator
   is updated to point to the new stmt, or left pointing to the original
   statement.  */

void
bsi_insert_after (block_stmt_iterator *curr_bsi, tree t,
		  enum bsi_iterator_update mode)
{
  basic_block bb = curr_bsi->bb;
  tree_cell curr, nw = ggc_alloc (sizeof (struct tree_container));

  set_bb_for_stmt (t, bb);
  curr = bsi_cell (*curr_bsi);
  nw->stmt = t;

  if (!curr)
    {
      /* Inserting after end of bb.  Only valid if the bb is empty.  */
      if (bb->head_tree)
	abort ();

      nw->prev = nw->next = NULL;
      bb->head_tree = bb->end_tree = nw;
      *curr_bsi = bsi_start (bb);
      return;
    }

  nw->next = curr->next;
  if (!curr->next)
    bb->end_tree = nw;
  else
    nw->next->prev = nw;
  curr->next = nw;
  nw->prev = curr;

  if (mode == BSI_NEW_STMT)
    bsi_next (curr_bsi);

  /* Now update the required SSA bits.  */
  modify_stmt (t);

  return;
}


/* This routine inserts a stmt before the stmt iterator passed in.
   The final parameter determines whether the statement iterator
   is updated to point to the new stmt, or left pointing to the original
   statement.  */
void
bsi_insert_before (block_stmt_iterator *curr_bsi, tree t,
		   enum bsi_iterator_update mode)
{
  basic_block bb = curr_bsi->bb;
  tree_cell curr, nw = xmalloc (sizeof (struct tree_container));

  set_bb_for_stmt (t, bb);
  curr = bsi_cell (*curr_bsi);
  nw->stmt = t;

  if (!curr)
    {
      /* Inserting before start of bb.  Only valid if the bb is empty.  */
      if (bb->head_tree)
	abort ();

      nw->prev = nw->next = NULL;
      bb->head_tree = bb->end_tree = nw;
      *curr_bsi = bsi_start (bb);
      return;
    }

  nw->prev = curr->prev;
  if (!nw->prev)
    bb->head_tree = nw;
  else
    nw->prev->next = nw;
  curr->prev = nw;
  nw->next = curr;

  if (mode == BSI_NEW_STMT)
    bsi_prev (curr_bsi);

  /* Now update the required SSA bits.  */
  modify_stmt (t);
}

/* This routine inserts a stmt on an edge. Every attempt is made to place the
   stmt in an existing basic block, but sometimes that isn't possible.  When
   it isn't possible, a new basic block is created, edges updated, and the
   stmt is added to the new block.  An iterator to the new stmt is returned.
   If a pointer to a BSI is passed in, and the stmt is inserted before or after
   an existing stmt in a block, old_bsi will be returned with an iterator for
   that stmt (The equivalent of BSI_SAME_STMT on an insert_before or after.
   If a created_block is passed in, and the edge is split, the new block is
   returned through this parameter.  */

block_stmt_iterator
bsi_insert_on_edge_immediate (edge e, tree stmt, block_stmt_iterator *old_bsi,
			      basic_block *created_block)
{
  basic_block src, dest, new_bb;
  block_stmt_iterator bsi, tmp;
  int num_exit, num_entry;
  tree last;
  bb_ann_t ann;
  edge e2;

  if (old_bsi)
    old_bsi->curr_stmt = NULL;
  if (created_block)
    *created_block = (basic_block)NULL;

  src = e->src;
  dest = e->dest;

  /* Cannot insert on an abnormal edge.  */
  if (e->flags & EDGE_ABNORMAL)
    abort ();

  /* No immediate edge insertion if there are already pending inserts.  */
  if (PENDING_STMT (e))
    abort ();

  num_exit = num_entry = 0;

  for (e2 = src->succ; e2; e2 = e2->succ_next)
    num_exit++;

  for (e2 = dest->pred; e2; e2 = e2->pred_next)
    num_entry++;

  /* If src is a single-exit block, and it isn't the entry block, then
     insert at the end of the block, if we can.  */

  if (num_exit == 1 && src != ENTRY_BLOCK_PTR)
    {
      bsi = bsi_last (src);
      /* If it is an empty block, simply insert after this bsi, and the new stmt
	 will become the only stmt in the block.  */
      if (bsi_end_p (bsi))
	{
	  bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);
	  return bsi;
	}
      last = bsi_stmt (bsi);

      /* If this is a fallthrough edge, then we can simply append this stmt
	 to the basic block.  */
      if (e->flags & EDGE_FALLTHRU)
	{
#ifdef ENABLE_CHECKING
	  /* Control statement edges should not be marked FALLTHRU.  */
	  if (is_ctrl_stmt (bsi_stmt (bsi)))
	    abort ();
#endif

    	  /* If the last stmt isn't a control altering stmt, then we can simply
    	     append this stmt to the basic block. This should mean the edge is
    	     a fallthrough edge.  */

	  if (!is_ctrl_stmt (last) && !is_ctrl_altering_stmt (last))
	    {
	      bsi_insert_after (&bsi, stmt, BSI_SAME_STMT);
	      if (old_bsi)
		*old_bsi = bsi;
	      bsi_next (&bsi);
	      return bsi;
	    }

	  /* If the last stmt is a GOTO, the we can simply insert before it.  */
	  if (TREE_CODE (last) == GOTO_EXPR)
	    {
	      bsi_insert_before (&bsi, stmt, BSI_NEW_STMT);
	      if (old_bsi)
		{
		  *old_bsi = bsi;
		  bsi_next (old_bsi);
		}
	      return bsi;
	    }
	}
    }

  /* If dest is a single entry destination, and it isn't the exit block, the new
     stmt can be inserted at the beginning of the destination block.  */

  if (num_entry == 1 && dest != EXIT_BLOCK_PTR)
    {
      bsi = bsi_start (dest);
      /* If it is an empty block, simply insert after this bsi, and the new stmt
	 will become the only stmt in the block.  */
      if (bsi_end_p (bsi))
	{
	  bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);
	  return bsi;
	}

      /* Skip any labels, and insert before the first non-label.  */
      for (tmp = bsi, bsi_next (&bsi); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  if (!is_label_stmt (bsi_stmt (bsi)))
	    {
	      bsi_insert_before (&bsi, stmt, BSI_NEW_STMT);
	      if (old_bsi)
		{
		  *old_bsi = bsi;
		  bsi_next (old_bsi);
		}
	      return bsi;
	    }
	  tmp = bsi;
	}

      /* If this point is reached, then the block consists of nothing but
	 labels, and tmp points to the last one. Insert after it.  */
      bsi_insert_after (&tmp, stmt, BSI_SAME_STMT);
      if (old_bsi)
	*old_bsi = tmp;
      bsi_next (&tmp);
      return tmp;
    }

  /* Otherwise, create a new basic block, and split this edge.  */
  new_bb = split_edge (e);
  ann = bb_ann (new_bb);

  if (created_block)
    *created_block = new_bb;

  bsi = bsi_start (new_bb);
  if (bsi_stmt (bsi)
      && TREE_CODE (bsi_stmt (bsi)) == LABEL_EXPR)
    bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);
  else
    bsi_insert_before (&bsi, stmt, BSI_NEW_STMT);

  return bsi;
}

/* This routine will commit all pending edge insertions, creating any new
   basic blocks which are necessary. The number of edges which were inserted
   is returned.  If the flag update_annotations is true, then new bitmaps are
   created for the dominator children, and they are updated.  If specified,
   new_blocks returned a count of the number of new basic blocks which were
   created.  */

int
bsi_commit_edge_inserts (int update_annotations, int *new_blocks)
{
  basic_block bb;
  block_stmt_iterator bsi, ret;
  edge e;
  tree stmt, next_stmt;
  int blocks, count = 0;

  blocks = n_basic_blocks;

  FOR_EACH_BB (bb)
    {
      for (e = bb->succ; e; e = e->succ_next)
        if (PENDING_STMT (e))
	  {
	    stmt = PENDING_STMT (e);
	    SET_PENDING_STMT (e, NULL_TREE);
	    next_stmt = TREE_CHAIN (stmt);
	    /* The first insert will create a new basic block if needed.  */
	    ret = bsi = bsi_insert_on_edge_immediate (e, stmt, NULL, NULL);
	    count++;
	    stmt = next_stmt;
	    for ( ; stmt; stmt = next_stmt)
	      {
	        /* All further inserts can simply follow the first one.  */
		next_stmt = TREE_CHAIN (stmt);
		bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);
		count++;
	      }

	  }
    }

  if (new_blocks)
    *new_blocks = n_basic_blocks - blocks;

  /* Expand arrays if we created new blocks and need to update them.  */
  if (update_annotations && blocks != n_basic_blocks)
    {
      abort ();
      /* TODO. Unimplemented at the moment.  */
    }

  return count;
}

/* This routine adds a stmt to the pending list on an edge. No actual
   insertion is made until a call to bsi_commit_edge_inserts () is made.  */

void
bsi_insert_on_edge (edge e, tree stmt)
{
  tree t;

  t = PENDING_STMT (e);
  if (!t)
    SET_PENDING_STMT (e, stmt);
  else
    {
      for ( ; TREE_CHAIN (t); t = TREE_CHAIN (t))
        continue;
      TREE_CHAIN (t) = stmt;
      TREE_CHAIN (stmt) = NULL_TREE;
    }

}

/* Replace the statement TP1 with the statement TP2.  */

static void
replace_stmt (tree_cell tp1, tree t)
{
  basic_block bb;

  /* Relocate annotations for the replacement statement.  */
  SET_EXPR_LOCUS (t, EXPR_LOCUS (tp1->stmt));
  bb = bb_for_stmt (tp1->stmt);
  tp1->stmt = t;
  set_bb_for_stmt (t, bb);
}

/*---------------------------------------------------------------------------
	    Tree specific functions for the cfg loop optimizer
---------------------------------------------------------------------------*/

/* Split a (typically critical) edge.  Return the new block.
   Abort on abnormal edges.  */

basic_block
tree_split_edge (edge edge_in)
{
  basic_block new_bb, dest;
  edge new_edge;
  tree phi;
  int i, num_elem;

  /* Abnormal edges cannot be split.  */
  if (edge_in->flags & EDGE_ABNORMAL)
    abort ();

  dest = edge_in->dest;
  new_bb = create_bb ();
  bb_ann (new_bb)->block = bb_ann (edge_in->src)->block;

  new_edge = make_edge (new_bb, dest, EDGE_FALLTHRU);
  /* Find all the PHI arguments on the original edge, and change them to
     the new edge.  Do it now, since redirect_edge_and_branch would cancel
     the args otherwise.  */
  for (phi = phi_nodes (dest); phi; phi = TREE_CHAIN (phi))
    {
      num_elem = PHI_NUM_ARGS (phi);
      for (i = 0; i < num_elem; i++)
	if (PHI_ARG_EDGE (phi, i) == edge_in)
	  {
	    PHI_ARG_EDGE (phi, i) = new_edge;
	    break;
	  }
    }

  if (!redirect_edge_and_branch (edge_in, new_bb))
    abort ();
  tree_cleanup_block_edges (new_bb, false);


  return new_bb;
}

/* Splits the basic block BB into two after BSI and returns the created
   fallthru edge.  */
edge
tree_split_block (basic_block bb, block_stmt_iterator bsi)
{
  basic_block new_bb = create_bb ();
  edge e;
  int end;

  /* Place the new block in front of the old one.  */
  unlink_block (new_bb);
  link_block (new_bb, bb->prev_bb);

  new_bb->head_tree = bb->head_tree;
  new_bb->end_tree = bsi_cell (bsi);

  end = bsi_end_p (bsi);
  if (!end)
    {
      bsi_next (&bsi);
      end = bsi_end_p (bsi);
    }

  if (end)
    {
      /* Unless the block has only a fallthru edge, splitting the block
	 would be wrong, as the edges coming from the control flow altering
	 statement would be on the wrong place.  */
      if (!bb->succ
	  || bb->succ->succ_next
	  || !(bb->flags & EDGE_FALLTHRU))
	abort ();

      new_bb->end_tree = bb->end_tree;
      bb->head_tree = NULL;
      bb->end_tree = NULL;
    }
  else
    {
      bb->head_tree = bsi_cell (bsi);
      new_bb->end_tree->next = NULL;
      bb->head_tree->prev = NULL;
    }

  new_bb->pred = bb->pred;
  bb->pred = NULL;
  for (e = new_bb->pred; e; e = e->pred_next)
    e->dest = new_bb;

  bb_ann (new_bb)->block = bb_ann (bb)->block;

  return make_edge (new_bb, bb, EDGE_FALLTHRU);
}

/* Moves basic block BB after block AFTER.  */
void
tree_move_block_after (basic_block bb, basic_block after, int no_false_fallthru)
{
  basic_block prev = bb->prev_bb;

  if (bb->prev_bb == after)
    return;

  unlink_block (bb);
  link_block (bb, after);

  /* Fix up fallthru edges.  */
  if (prev && prev != ENTRY_BLOCK_PTR)
    tree_cleanup_block_edges (prev, no_false_fallthru);
  tree_cleanup_block_edges (bb, no_false_fallthru);
  tree_cleanup_block_edges (after, no_false_fallthru);
}

/* Verifies that the flow information is OK.  */

static int
tree_verify_flow_info (void)
{
  basic_block bb;
  tree_cell stmt, last;
  tree t, l, label, phi;
  block_stmt_iterator si;
  edge e, le, te, fe;
  struct block_tree *bti;
  int err = 0, degree;

  FOR_EACH_BB (bb)
    {
      if (!bb->head_tree)
	continue;

      last = NULL;
      for (stmt = bb->head_tree; stmt; last = stmt, stmt = stmt->next)
	{
	  if (stmt->prev != last)
	    {
	      fprintf (stderr, "chain inconsistent in bb %d\n", bb->index);
	      err = 1;
	    }

	  if (last
	      && stmt_ends_bb_p (last->stmt))
	    {
	      fprintf (stderr, "control statement in the middle of bb %d\n",
		       bb->index);
	      err = 1;
	    }
	  
	  if (TREE_CODE (stmt->stmt) == COMPOUND_EXPR
	      || TREE_CODE (stmt->stmt) == CASE_LABEL_EXPR
	      || TREE_CODE (stmt->stmt) == BIND_EXPR)
	    {
	      fprintf (stderr, "forbidden statement in the middle of bb %d\n",
		       bb->index);
	      err = 1;
	    }

	  if (bb_for_stmt (stmt->stmt) != bb)
	    {
	      fprintf (stderr, "bb_for_stmt inconsistent in bb %d\n",
		       bb->index);
	      err = 1;
	    }
	}
      if (last != bb->end_tree)
	{
	  fprintf (stderr, "end_tree inconsistent in bb %d\n",
		   bb->index);
	  err = 1;
	}

      last = NULL;
      for (stmt = bb->end_tree; stmt; last = stmt, stmt = stmt->prev)
	if (stmt->next != last)
	  {
	    fprintf (stderr, "chain inconsistent in bb %d\n", bb->index);
	    err = 1;
	  }
      if (last != bb->head_tree)
	{
	  fprintf (stderr, "head_tree inconsistent in bb %d\n",
		   bb->index);
	  err = 1;
	}

      t = last_stmt (bb);
      if (t)
	{
	  switch (TREE_CODE (t))
	    {
	    case GOTO_EXPR:
	      if (!bb->succ)
		{
		  fprintf (stderr, "goto without successors %d\n", bb->index);
		  err = 1;
		}
	      if (is_computed_goto (t)
		  || nonlocal_goto_p (t))
		break;

	      le = NULL;
	      for (e = bb->succ; e; e = e->succ_next)
		{
		  if (!(e->flags & EDGE_ABNORMAL))
		    {
		      if (le)
			{
			  fprintf (stderr, "goto with more than one successor %d\n",
				   bb->index);
			  err = 1;
			}
		      else
			le = e;
		    }

		  if (e->flags & EDGE_FALLTHRU)
		    {
		      fprintf (stderr, "goto with fallthru %d\n", bb->index);
		      err = 1;
		    }
		}
	      if (!le)
		{
		  fprintf (stderr, "goto without successors %d\n", bb->index);
		  err = 1;
		}

	      label = GOTO_DESTINATION (t);
	      for (si = bsi_start (le->dest);
		   !bsi_end_p (si);
		   bsi_next (&si))
		{
		  l = bsi_stmt (si);
		  if (TREE_CODE (l) == LABEL_EXPR
		      && LABEL_EXPR_LABEL (l) == label)
		    break;
		}
	      if (bsi_end_p (si))
		{
		  fprintf (stderr, "%d: goto label not in dest block %d\n",
			   bb->index, le->dest->index);
		  err = 1;
		}
	      break;

	    case COND_EXPR:
	      if (!bb->succ || !bb->succ->succ_next)
		{
		  fprintf (stderr, "condition with less than 2 successors %d\n",
			   bb->index);
		  err = 1;
		}

	      te = fe = NULL;
	      for (e = bb->succ; e; e = e->succ_next)
		{
		  if (e->flags & EDGE_FALLTHRU)
		    {
		      fprintf (stderr, "condition with fallthru %d\n", bb->index);
		      err = 1;
		    }
		  if (e->flags & EDGE_ABNORMAL)
		    continue;

		  if (e->flags & EDGE_TRUE_VALUE)
		    {
		      if (te)
			{
			  fprintf (stderr, "more than one true edge %d\n",
				   bb->index);
			  err = 1;
			}
		      else
			te = e;
		    }

		  if (e->flags & EDGE_FALSE_VALUE)
		    {
		      if (fe)
			{
			  fprintf (stderr, "more than one false edge %d\n",
				   bb->index);
			  err = 1;
			}
		      else
			fe = e;
		    }
		}
	      if (!te)
		{
		  fprintf (stderr, "condition with no true edge %d\n",
			   bb->index);
		  err = 1;
		}
	      if (!fe)
		{
		  fprintf (stderr, "condition with no false edge %d\n",
			   bb->index);
		  err = 1;
		}

	      label = GOTO_DESTINATION (COND_EXPR_THEN (t));
	      for (si = bsi_start (te->dest); !bsi_end_p (si); bsi_next (&si))
		{
		  l = bsi_stmt (si);
		  if (TREE_CODE (l) == LABEL_EXPR
		      && LABEL_EXPR_LABEL (l) == label)
		    break;
		}
	      if (bsi_end_p (si))
		{
		  fprintf (stderr, "%d: goto label not in dest block %d\n",
			   bb->index, te->dest->index);
		  err = 1;
		}

	      label = GOTO_DESTINATION (COND_EXPR_ELSE (t));
	      for (si = bsi_start (fe->dest); !bsi_end_p (si); bsi_next (&si))
		{
		  l = bsi_stmt (si);
		  if (TREE_CODE (l) == LABEL_EXPR
		      && LABEL_EXPR_LABEL (l) == label)
		    break;
		}
	      if (bsi_end_p (si))
		{
		  fprintf (stderr, "%d: goto label not in dest block %d\n",
			   bb->index, fe->dest->index);
		  err = 1;
		}
	      break;

	    default: ;
	    }
	}

      degree = 0;
      for (e = bb->pred; e; e = e->pred_next)
	{
	  if (bb != bb_ann (bb)->block->entry
	      && (e->flags & EDGE_CONSTRUCT_ENTRY))
	    {
	      fprintf (stderr,
		       "Construct entry edge not entering construct entry: %d\n",
		       bb->index);
	      err = 1;
	    }

	  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	    if (phi_arg_from_edge (phi, e) < 0)
	      {
		fprintf (stderr,
			 "No entry for edge in phi node: %d\n", bb->index);
		err = 1;
	      }
	  degree++;
	}
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	if (PHI_NUM_ARGS (phi) != degree)
	  {
	    fprintf (stderr,
		     "Superfluous entries in phi node: %d\n", bb->index);
	    err = 1;
	  }
    }

  /* Entry block should have just one successor.  Exit block should have
     at most one fallthru predecessor.  */
  if (!ENTRY_BLOCK_PTR->succ
      || ENTRY_BLOCK_PTR->succ->succ_next)
    {
      fprintf (stderr, "Wrong amount of edges from entry\n");
      err = 1;
    }

  if (!(ENTRY_BLOCK_PTR->succ->flags & EDGE_FALLTHRU))
    {
      fprintf (stderr, "Entry is not fallthru\n");
      err = 1;
    }

  le = NULL;
  for (e = EXIT_BLOCK_PTR->pred; e; e = e->pred_next)
    if (e->flags & EDGE_FALLTHRU)
      {
	if (le)
	  {
	    fprintf (stderr, "More than one fallthru to exit\n");
	    err = 1;
	    break;
	  }
	le = e;
      }

  /* Check special edges of constructs.  */
  for (bti = bti_start (); !bti_end_p (bti); bti_next (&bti))
    if (bti->entry)
      {
	le = NULL;
	for (e = bti->entry->pred; e; e = e->pred_next)
	  if (e->flags & EDGE_CONSTRUCT_ENTRY)
	    {
	      if (le)
		{
		  fprintf (stderr, "More than one construct %p entry\n",
			   (void *) bti);
		  err = 1;
		  break;
		}
	      le = e;
	    }
      }

  return err;
}


/* Split BB into entry part and rest; if REDIRECT_LATCH, redirect edges
   marked as latch into entry part, analogically for REDIRECT_NONLATCH.
   In both of these cases, ignore edge EXCEPT.  If CONN_LATCH, set edge
   between created entry part and BB as latch one.  Return created entry
   part.  */

static basic_block
tree_make_forwarder_block (basic_block bb, int redirect_latch,
                           int redirect_nonlatch, edge except, int conn_latch)
{
  edge e, next_e, fallthru;
  basic_block dummy;

  /* Create the new basic block.  */
  dummy = create_bb ();
  dummy->count = bb->count;
  dummy->frequency = bb->frequency;
  dummy->loop_depth = bb->loop_depth;
  dummy->head_tree = NULL;
  dummy->end_tree = NULL;

  /* Redirect the incoming edges.  */
  dummy->pred = bb->pred;
  bb->pred = NULL;
  for (e = dummy->pred; e; e = e->pred_next)
    e->dest = dummy;

  fallthru = make_edge (dummy, bb, 0);

  HEADER_BLOCK (dummy) = 0;
  HEADER_BLOCK (bb) = 1;

  /* Redirect back edges we want to keep.  */
  for (e = dummy->pred; e; e = next_e)
    {
      next_e = e->pred_next;
      if (e == except
	  || !((redirect_latch && LATCH_EDGE (e))
	       || (redirect_nonlatch && !LATCH_EDGE (e))))
	{
	  dummy->frequency -= EDGE_FREQUENCY (e);
	  dummy->count -= e->count;
	  if (dummy->frequency < 0)
	    dummy->frequency = 0;
	  if (dummy->count < 0)
	    dummy->count = 0;
	  redirect_edge_succ (e, bb);
	}
    }

  alloc_aux_for_edge (fallthru, sizeof (int));
  LATCH_EDGE (fallthru) = conn_latch;

  return dummy;
}

/* Cleanup the cfg -- edges correspondence for a single block BB.  If
   NO_FALSE_FALLTHRU, just remove false fallthru edges.  */
void
tree_cleanup_block_edges (basic_block bb, int no_false_fallthru)
{
  edge e, e_true, e_false, next;
  block_stmt_iterator bsi;
  tree stmt, alt;

  if (bb == ENTRY_BLOCK_PTR)
    return;

redo:
  stmt = last_stmt (bb);
  bsi = bsi_last (bb);

  if (!stmt
      || !stmt_ends_bb_p (stmt)
      || no_false_fallthru)
    {
      for (e = bb->succ; e; e = e->succ_next)
	if (e->flags & EDGE_FALLTHRU)
	  break;

      if (e && e->dest != bb->next_bb && e->dest != EXIT_BLOCK_PTR)
	{
	  tree label = tree_block_label (e->dest);

	  bsi_insert_after (&bsi,
			    build1 (GOTO_EXPR, void_type_node, label),
			    BSI_NEW_STMT);
	  e->flags &= ~EDGE_FALLTHRU;
	  goto redo;
	}
    }

  if (!stmt
      || no_false_fallthru)
    return;

  switch (TREE_CODE (stmt))
    {
    case GOTO_EXPR:
      if (is_computed_goto (stmt)
	  || nonlocal_goto_p (stmt))
	break;

      e_true = NULL;
      for (e = bb->succ; e; e = e->succ_next)
	{
	  if (e->flags & EDGE_ABNORMAL)
	    continue;

	  if (e_true)
	    abort ();

	  e_true = e;
	}
	    
      if (!e_true)
	break;

      if (e_true->dest == bb->next_bb)
	{
     	  bsi_remove (&bsi);
	  e_true->flags |= EDGE_FALLTHRU;
	}
      break;

    case COND_EXPR:
      e_true = e_false = NULL;

      for (e = bb->succ; e; e = e->succ_next)
	{
	  if (e->flags & EDGE_TRUE_VALUE)
	    {
	      if (e_true)
		abort ();
	      e_true = e;
	    }
	  if (e->flags & EDGE_FALSE_VALUE)
	    {
	      if (e_false)
		abort ();
	      e_false = e;
	    }
	}
      if ((!e_true && !e_false)
	  || (e_true && e_false))
	break;

      if (!e_true)
	bsi_replace (bsi, COND_EXPR_ELSE (stmt));
      else if (!e_false)
      	bsi_replace (bsi, COND_EXPR_THEN (stmt));

      /* Try optimizing the resulting goto.  */
      goto redo;

    case SWITCH_EXPR:
      if (!bb->succ)
	break;

      alt = SWITCH_BODY (stmt);
      if (bb->succ->succ_next)
	{
	  /* If there are more edges, but only one matches, we may proceed
	     anyway.  */
	  e_true = find_taken_edge (bb, SWITCH_COND (stmt));
	  if (!e_true)
	    break;
	  for (alt = SWITCH_BODY (stmt); alt; alt = CASE_NEXT (alt))
	    if (CASE_EDGE (alt) == e_true)
	      break;
	  if (!alt)
	    abort ();

	  for (e = bb->succ; e; e = next)
	    {
	      next = e->succ_next;
	      if (e == e_true
		  || (e->flags & EDGE_ABNORMAL))
		continue;

	      ssa_remove_edge (e);
	    }
	}

      bsi_replace (bsi, CASE_GOTO (alt));
      goto redo;

    default: ;
    }
}

/* Cleanup the cfg -- edges correspondence damaged by previous passes.
   Edges could be removed while unreachable jumps from their control
   statemens were not.  */
static void
tree_cleanup_edges ()
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      tree_cleanup_block_edges (bb, false);
    }
}

/* Initialization of functions specific to the tree IR.  */

void
tree_register_cfg_hooks ()
{
  cfg_hooks = &tree_cfg_hooks;
}

/* Dumps information about tree_cell chain START to stderr.  */
void
debug_tree_chain (tree_cell start)
{
  tree op;

  for (; start; start = start->next)
    {
      switch (start->note)
	{
	case TCN_STATEMENT:
	  debug_generic_stmt (start->stmt);
	  break;

	case TCN_BIND:
	  fprintf (stderr, "{\n");
	  for (op = BIND_EXPR_VARS (start->stmt); op; op = TREE_CHAIN (op))
	    debug_generic_stmt (op);
	  break;

	case TCN_UNBIND:
	  fprintf (stderr, "}\n");
	  break;

	default:
	  abort ();
	}
    }
}

/* Dumps identification of BLOCK to FILE.  */
static void
dump_block_tree_id (FILE *file, struct block_tree *block)
{
  fprintf (file, "BIND %p", (void *) block);
}

/* Dumps information about block tree BLOCK to FILE, indenting INDENT spaces.  */
void
dump_block_tree (FILE *file, int indent, struct block_tree *block)
{
  int i;
  tree var;

  for (i = 0; i < indent; i++)
    fprintf (file, " ");
  dump_block_tree_id (file, block);

  fprintf (file, " vars");
  for (var = BIND_EXPR_VARS (block->bind); var; var = TREE_CHAIN (var))
    {
      fprintf (file, " ");
      print_generic_stmt (file, var, 0);
    }

  if (block->entry)
    fprintf (file, " entry %d", block->entry->index);

  fprintf (file, "\n");
  for (block = block->subtree; block; block = block->next)
    dump_block_tree (file, indent + 2, block);
}

/* Initialize loop optimizer.  */

static struct loops *
tree_loop_optimizer_init (FILE *dumpfile)
{
  struct loops *loops = xcalloc (1, sizeof (struct loops));

  /* Find the loops.  */
  if (flow_loops_find (loops, LOOP_TREE) <= 1)
    {
      /* No loops.  */
      flow_loops_free (loops);
      free (loops);
      return NULL;
    }

  /* Not going to update these.  */
  free (loops->cfg.rc_order);
  loops->cfg.rc_order = NULL;
  free (loops->cfg.dfs_order);
  loops->cfg.dfs_order = NULL;

  /* Force all latches to have only single successor.  */
  force_single_succ_latches (loops);

  /* Mark irreducible loops.  */
  mark_irreducible_loops (loops);

  /* Dump loops.  */
  flow_loops_dump (loops, dumpfile, NULL, 1);

#ifdef ENABLE_CHECKING
  verify_dominators (loops->cfg.dom);
  verify_loop_structure (loops);
#endif

  return loops;
}

/* Finalize loop optimizer.  */
static void
tree_loop_optimizer_finalize (struct loops *loops, FILE *dumpfile)
{
  if (loops == NULL)
    return;

  /* Another dump.  */
  flow_loops_dump (loops, dumpfile, NULL, 1);

  /* Clean up.  */
  flow_loops_free (loops);
  free (loops);

  /* Checking.  */
#ifdef ENABLE_CHECKING
  verify_flow_info ();
#endif
}

/* Assigns a scope to variables defined in block SCOPE.  */
static void
assign_vars_to_scope (struct block_tree *scope)
{
  tree var;

  for (var = BIND_EXPR_VARS (scope->bind); var; var = TREE_CHAIN (var))
    get_var_ann (var)->scope = scope;
}

/* Assigns a scope in that it is defined to each variable.  */
void
assign_vars_to_scopes ()
{
  struct block_tree *act;

  for (act = bti_start (); !bti_end_p (act); bti_next (&act))
    assign_vars_to_scope (act);
}

/* Ensures that there are not useless labels.  It makes all jumps to
   the block to target one label (non-artificial ones are preferred),
   and removes all other artificial labels (the named ones are left
   for debugging purposes.  */
static void
remove_superfluous_labels ()
{
  basic_block bb;
  edge e;
  block_stmt_iterator bsi;
  tree stmt, label, alt;
  int preserve;

  FOR_EACH_BB (bb)
    {
      bsi = bsi_start (bb);
      stmt = first_stmt (bb);

      if (!stmt || TREE_CODE (stmt) != LABEL_EXPR)
	continue;

      /* Try to find a label that must stay anyway.  */
      label = LABEL_EXPR_LABEL (stmt);
      for (;
	   DECL_ARTIFICIAL (label) && !FORCED_LABEL (label) && !bsi_end_p (bsi);
	   bsi_next (&bsi))
	{
	  stmt = bsi_stmt (bsi);
	  if (TREE_CODE (stmt) != LABEL_EXPR)
	    break;

	  label = LABEL_EXPR_LABEL (stmt);
	}

      /* Replace to it within all jumps to the block.  */
      preserve = false;
      for (e = bb->pred; e; e = e->pred_next)
	{
	  if ((e->flags & EDGE_ABNORMAL)
	      || (e->flags & EDGE_FALLTHRU))
	    continue;

	  stmt = last_stmt (e->src);
	  if (TREE_CODE (stmt) == SWITCH_EXPR)
	    {
	      for (alt = SWITCH_BODY (stmt); alt; alt = CASE_NEXT (alt))
		if (CASE_EDGE (alt) == e)
		  CASE_DESTINATION (alt) = label;
	    }
	  else
	    {
	      stmt = find_edge_goto (last_stmt (e->src), e);
	      if (!stmt)
		abort ();
	      GOTO_DESTINATION (stmt) = label;
	    }

	  preserve = true;
	}

      if (!preserve)
	label = NULL_TREE;

      /* Remove unnecesary labels.  Due to way we have chosen the label to
	 keep, the kept label will then be the first one of the block,
	 therefore the one returned by tree_block_label, which is fine.  */
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); )
	{
	  stmt = bsi_stmt (bsi);
	  if (TREE_CODE (stmt) != LABEL_EXPR)
	    break;

	  if (LABEL_EXPR_LABEL (stmt) == label
	      || !DECL_ARTIFICIAL (LABEL_EXPR_LABEL (stmt))
	      || FORCED_LABEL (LABEL_EXPR_LABEL (stmt)))
	    {
	      bsi_next (&bsi);
	      continue;
	    }

	  bsi_remove (&bsi);
	}
    }
}

/* Checks whether the basic block BB does nothing except for jump.  */
static bool
tree_forwarder_block_p (basic_block bb)
{
  block_stmt_iterator bsi;

  if (!bb->succ
      || bb->succ->succ_next
      || (bb->succ->flags & EDGE_ABNORMAL)
      || (bb->succ->flags & EDGE_CONSTRUCT_ENTRY)
      || bb == ENTRY_BLOCK_PTR)
    return false; 

  if (phi_nodes (bb))
    return false;

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    switch (TREE_CODE (bsi_stmt (bsi)))
      {
      case LABEL_EXPR:
      case GOTO_EXPR:
	break;

      default:
	return false;
      }

  return true;
}

/* Threads jumps over empty statements.  Later we may add threading over
   equivalent conditions.  */
static void
thread_jumps ()
{
  edge e, next, last, old;
  basic_block bb, dest, slow;
  int set_slow;
  tree phi, val1, val2;
  int arg;

  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR, EXIT_BLOCK_PTR, next_bb)
    {
      /* Don't waste time on unreachable blocks.  */
      if (!bb->pred)
	continue;

      /* Nor on forwarders.  */
      if (tree_forwarder_block_p (bb))
	continue;

      for (e = bb->succ; e; e = next)
	{
	  next = e->succ_next;

	  if ((e->flags & EDGE_ABNORMAL)
	      || (e->flags & EDGE_CONSTRUCT_ENTRY)
	      || e->dest == EXIT_BLOCK_PTR
	      || !tree_forwarder_block_p (e->dest)
	      || e->dest->succ->dest == EXIT_BLOCK_PTR)
	    continue;

	  slow = e->dest;
	  set_slow = 0;

	  last = e->dest->succ;
	  for (dest = e->dest->succ->dest;
	       tree_forwarder_block_p (dest);
	       last = dest->succ,
	       dest = dest->succ->dest,
	       set_slow ^= 1)
	    {
	      /* Infinite loop detected.  */
	      if (slow == dest)
		break;
	      if (set_slow)
		slow = slow->succ->dest;

	      if (dest->succ->dest == EXIT_BLOCK_PTR)
		break;
	    }

	  if (dest == e->dest)
	    continue;
	      
	  old = find_edge (bb, dest);
	  if (old)
	    {
	      /* If there already is an edge, check whether the values of
		 in phi nodes differ.  */
	      for (phi = phi_nodes (dest); phi; phi = TREE_CHAIN (phi))
		{
		  val1 = PHI_ARG_DEF (phi, phi_arg_from_edge (phi, last));
		  val2 = PHI_ARG_DEF (phi, phi_arg_from_edge (phi, old));

		  if (!operand_equal_p (val1, val2, false))
		    break;
		}
	      if (phi)
		{
		  /* The previous block is forwarder, so there are no
		     phi nodes to update.  */
		  dest = last->src;
		}
	    }

	  if (dest == e->dest)
    	    continue;

	  if (redirect_edge_and_branch (e, dest)
	      && !old)
	    {
	      /* Update phi nodes.  */
	      for (phi = phi_nodes (dest); phi; phi = TREE_CHAIN (phi))
		{
		  arg = phi_arg_from_edge (phi, last);
		  if (arg < 0)
		    abort ();
		  add_phi_arg (&phi, PHI_ARG_DEF (phi, arg), e);
		}
	    }
	}
    }
}

/* Creates a new anonymous label.  */
tree
build_new_label ()
{
  tree label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
  DECL_CONTEXT (label) = current_function_decl;
  DECL_ARTIFICIAL (label) = 1;

  return label;
}

/* Returns a GOTO_EXPR for edge E found in STMT.  */
static tree
find_edge_goto (tree stmt, edge e)
{
  if (!stmt)
    return NULL_TREE;

  switch (TREE_CODE (stmt))
    {
    case SWITCH_EXPR:
      /* Using this function for switch_exprs is dangerous, since there
	 may be more than one goto to the one edge.  */
      abort ();

    case COND_EXPR:
      if (e->flags & EDGE_TRUE_VALUE)
	return COND_EXPR_THEN (stmt);
      else if (e->flags & EDGE_FALSE_VALUE)
	return COND_EXPR_ELSE (stmt);
      else
	abort ();

    case GOTO_EXPR:
      return stmt;

    default:
      return NULL_TREE;
    }
}

/* Removes unneccesary variables.  */
void 
remove_useless_stmts_and_vars ()
{
  struct block_tree *bti;
  tree vars, prev;
  struct var_ann_d *ann;

  for (bti = bti_start (); !bti_end_p (bti); bti_next (&bti))
    {
      prev = NULL_TREE;
      for (vars = BIND_EXPR_VARS (bti->bind);
	   vars;
	   vars = TREE_CHAIN (vars))
	{
	  /* We could have function declarations and the like
	     on this list.  Ignore them.  */
	  if (TREE_CODE (vars) != VAR_DECL)
	    {
	      prev = vars;
	      continue;
	    }

	  /* Remove all unused, unaliased temporaries.  Also remove
	     unused, unaliased local variables during highly
	     optimizing compilations.  */
	  ann = var_ann (vars);
	  if (ann
	      && ! ann->may_aliases
	      && ! ann->used
	      && ! ann->has_hidden_use
	      && ! TREE_ADDRESSABLE (vars)
	      && (DECL_ARTIFICIAL (vars) || optimize >= 2))
	    {
	      tree block = BIND_EXPR_BLOCK (bti->bind);

	      if (block)
		remove_decl (vars, block);
	      else
		remove_decl (vars, DECL_INITIAL (current_function_decl));

	      if (prev)
		TREE_CHAIN (prev) = TREE_CHAIN (vars);
	      else
		BIND_EXPR_VARS (bti->bind) = TREE_CHAIN (vars);
	    }
	  else
	    prev = vars;
	}
    }
}

/* Merges blocks if possible.  */
static void
merge_seq_blocks ()
{
  basic_block bb;

  for (bb = ENTRY_BLOCK_PTR->next_bb; bb != EXIT_BLOCK_PTR; )
    {
      if (
	  /* It must have a single succesor.  */
	  bb->succ
	  && !bb->succ->succ_next
	  && !(bb->succ->flags & EDGE_ABNORMAL)
	  && !(bb->succ->flags & EDGE_CONSTRUCT_ENTRY)
	  && bb->succ->dest != EXIT_BLOCK_PTR
	  && bb->succ->dest != bb
	  /* That has a single predecessor.  */
	  && !bb->succ->dest->pred->pred_next
	  /* Don't merge blocks from different contexts.  */
	  && bb_ann (bb)->block == bb_ann (bb->succ->dest)->block)
	{
	  /* Merge the blocks and retry.  */
	  merge_blocks (bb, bb->succ->dest);
	  continue;
	}

      bb = bb->next_bb;
    }
}
