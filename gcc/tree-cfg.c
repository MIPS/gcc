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

/* Local declarations.  */

/* Initial capacity for the basic block array.  */
static const int initial_cfg_capacity = 20;

/* Dump files and flags.  */
static FILE *dump_file;		/* CFG dump file. */
static int dump_flags;		/* CFG dump flags.  */

/* Mapping of labels to their associated blocks.  This can greatly speed up
   building of the CFG in code with lots of gotos.  */
static varray_type label_to_block_map;

/* CFG statistics.  */
struct cfg_stats_d
{
  long num_merged_labels;
};

static dominance_info pdom_info = NULL;

static struct cfg_stats_d cfg_stats;

static struct obstack block_tree_ann_obstack;
static void *first_block_tree_ann_obj = 0;

/* Nonzero if we found a computed goto while building basic blocks.  */
static bool found_computed_goto;

/* If we found computed gotos, then they are all revectored to this
   location.  We try to unfactor them after we have translated out
   of SSA form.  */
static tree factored_computed_goto_label;

/* The factored computed goto.  We cache this so we can easily recover
   the destination of computed gotos when unfactoring them.  */
static tree factored_computed_goto;

/* Basic blocks and flowgraphs.  */
static void create_blocks_annotations (void);
static void create_block_annotation (basic_block);
static void free_blocks_annotations (void);
static void clear_blocks_annotations (void);
static basic_block make_blocks (tree *, basic_block);
static inline void append_stmt_to_bb (tree *, basic_block);
static inline void prepend_stmt_to_bb (tree *, basic_block);
static void factor_computed_gotos (void);

/* Edges.  */
static void make_edges (void);
static void make_ctrl_stmt_edges (basic_block);
static void make_exit_edges (basic_block);
static void make_cond_expr_edges (basic_block);
static void make_switch_expr_edges (basic_block);
static void make_goto_expr_edges (basic_block);

/* Various helpers.  */
static tree *first_exec_stmt (tree *);
static inline bool stmt_starts_bb_p (tree, tree);
static inline bool stmt_ends_bb_p (tree);
static int tree_verify_flow_info (void);
static basic_block tree_make_forwarder_block (basic_block, int, int, edge, int);
static struct loops *tree_loop_optimizer_init (FILE *);
static void tree_loop_optimizer_finalize (struct loops *, FILE *);
static bool thread_jumps (void);
static bool tree_forwarder_block_p (basic_block);

/* Flowgraph optimization and cleanup.  */

static void remove_bb (basic_block);
static void remove_stmt (tree *, bool);
static bool cleanup_control_flow (void);
static edge find_taken_edge_cond_expr (basic_block, tree);
static edge find_taken_edge_switch_expr (basic_block, tree);
static tree find_case_label_for_value (tree, tree);
static void replace_stmt (tree *, tree *);
static int phi_alternatives_equal (basic_block, edge, edge);

/* Block iterator helpers.  */
static void remove_bsi_from_block (block_stmt_iterator *, bool);
static block_stmt_iterator bsi_init (tree *, basic_block);
static inline void bsi_update_from_tsi (block_stmt_iterator *,
					tree_stmt_iterator);
static tree_stmt_iterator bsi_link_after (tree_stmt_iterator *, tree,
					  basic_block);

/* Location to track pending stmt for edge insertion.  */
#define PENDING_STMT(e)	((e)->insns.t)

/*---------------------------------------------------------------------------
			      Create basic blocks
---------------------------------------------------------------------------*/

/* Entry point to the CFG builder for trees.  FNBODY is the body of the
   function to process.  */

void
build_tree_cfg (tree *fnbody)
{
  tree *first_p;

  timevar_push (TV_TREE_CFG);

  /* Register specific tree functions.  */
  tree_register_cfg_hooks ();

  /* Initialize the basic block array.  */
  n_basic_blocks = 0;
  last_basic_block = 0;
  VARRAY_BB_INIT (basic_block_info, initial_cfg_capacity, "basic_block_info");
  memset ((void *) &cfg_stats, 0, sizeof (cfg_stats));

  /* Build a mapping of labels to their associated blocks.  */
  VARRAY_BB_INIT (label_to_block_map, initial_cfg_capacity,
		  "label to block map");

  ENTRY_BLOCK_PTR->next_bb = EXIT_BLOCK_PTR;
  EXIT_BLOCK_PTR->prev_bb = ENTRY_BLOCK_PTR;

  first_p = first_exec_stmt (fnbody);
  if (first_p)
    {
      found_computed_goto = 0;
      make_blocks (first_p, NULL);

      /* Computed gotos are hell to deal with, especially if there are
	 lots of them with a large number of destinations.  So we factor
	 them to a common computed goto location before we build the
	 edge list.  After we convert back to normal form, we will un-factor
	 the computed gotos since factoring introduces an unwanted jump.  */
      if (found_computed_goto)
	factor_computed_gotos ();

      if (n_basic_blocks > 0)
	{
	  /* Adjust the size of the array.  */
	  VARRAY_GROW (basic_block_info, n_basic_blocks);

	  /* Create block annotations.  */
	  create_blocks_annotations ();

	  /* Create the edges of the flowgraph.  */
	  make_edges ();
	}
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

/* Search the CFG for any computed gotos.  If found, factor them to a 
   common computed goto site.  Also record the location of that site so
   that we can un-factor the gotos after we have converted back to 
   normal form.  */

static void
factor_computed_gotos (void)
{
  basic_block bb;
  tree factored_label_decl = NULL;
  tree var = NULL;

  /* We know there are one or more computed gotos in this function.
     Examine the last statement in each basic block to see if the block
     ends with a computed goto.  */
	
  FOR_EACH_BB (bb)
    {
      tree *last_p = last_stmt_ptr (bb);
      tree last = *last_p;

      /* Ignore the computed goto we create when we factor the original
	 computed gotos.  */
      if (last == factored_computed_goto)
	continue;

      /* If the last statement is a compted goto, factor it.  */
      if (computed_goto_p (last))
	{
	  tree assignment;
	  block_stmt_iterator bsi = bsi_last (bb);

	  /* The first time we find a computed goto we need to create
	     the factored goto block and the variable each original
	     computed goto will use for their goto destination.  */
	  if (! factored_computed_goto)
	    {
	      tree compound;
	      tree_stmt_iterator tsi = tsi_from_bsi (bsi);

	      /* Create the destination of the factored goto.  Each original
		 computed goto will put its desired destination into this
		 variable and jump to the label we create immediately
		 below.  */
	      var = create_tmp_var (ptr_type_node, "gotovar");

	      /* Build a label for the new block which will contain the
		 factored computed goto.  */
	      factored_label_decl = create_artificial_label ();
	      factored_computed_goto_label
		= build1 (LABEL_EXPR, void_type_node, factored_label_decl);
	      modify_stmt (factored_computed_goto_label);

	      /* Build our new computed goto.  */
	      factored_computed_goto = build1 (GOTO_EXPR, void_type_node, var);
	      modify_stmt (factored_computed_goto);

	      /* Cram the new label and the computed goto into a container.  */
	      compound = build (COMPOUND_EXPR, void_type_node,
				factored_computed_goto_label,
				factored_computed_goto);

	      /* Ugh.  We want to pass the address of the container to
		 make_blocks call below.  But we certainly don't want to
		 to pass along the address of a global.  There's got to be
		 a better way to do this than to create a dummy container.  */
	      compound = build (COMPOUND_EXPR, void_type_node, compound, NULL);

	      /* Put the new statements into a new basic block.  This must
		 be done before we link them into the statement chain!  */
	      make_blocks (&TREE_OPERAND (compound, 0), NULL);

	      /* Now it is safe to link in the new statements.  */
	      tsi_link_chain_after (&tsi,
				    TREE_OPERAND (compound, 0),
				    TSI_CHAIN_START);
	    }

	  /* Copy the original computed goto's destination into VAR.  */
          assignment = build (MODIFY_EXPR, ptr_type_node,
			      var, GOTO_DESTINATION (last));
	  modify_stmt (assignment);

	  /* Insert that assignment just before the original computed
	     goto.  */
          set_bb_for_stmt (assignment, bb);
	  bsi_insert_before (&bsi, assignment, BSI_NEW_STMT);

	  /* And revector the computed goto to the new destination.  */
          GOTO_DESTINATION (last) = factored_label_decl;
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
      gcc_obstack_init (&block_tree_ann_obstack);
      initialized = 1;
    }
  /* Check whether TREE_ANNOTATIONS data are still allocated.  */
  else if (first_block_tree_ann_obj)
    abort ();
  
  first_block_tree_ann_obj = obstack_alloc (&block_tree_ann_obstack, 0);
  
  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR, NULL, next_bb)
    create_block_annotation (bb);
}

/* Create annotations for a single basic block.  */

static void create_block_annotation (basic_block bb)
{
  /* Verify that the tree_annotations field is clear.  */
  if (bb->tree_annotations || !first_block_tree_ann_obj)
    abort ();
  bb->tree_annotations = obstack_alloc (&block_tree_ann_obstack, 
					sizeof (struct bb_ann_d));
  memset (bb->tree_annotations, 0, sizeof (struct bb_ann_d));
}

/* Free the annotations for all the basic blocks.  */

static void free_blocks_annotations (void)
{
  if (!first_block_tree_ann_obj)
    abort ();
  obstack_free (&block_tree_ann_obstack, first_block_tree_ann_obj);
  first_block_tree_ann_obj = NULL;

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

/* Build a flowgraph for the statements starting at the statement pointed
   by FIRST_P.

   BB is the block where the statements should be added to.  If BB is NULL,
      a new basic block will be created for the statements.

   Return the last basic block added to the graph.  This is used to know if
   a recursive invocation built a sub-graph whose last block can accept
   more statements or not.  */

static basic_block
make_blocks (tree *first_p, basic_block bb)
{
  tree_stmt_iterator i;
  tree stmt, last;
  bool start_new_block;

  if (first_p == NULL
      || *first_p == error_mark_node)
    return NULL;

  start_new_block = (bb == NULL);
  stmt = last = NULL;
  for (i = tsi_start (first_p); !tsi_end_p (i); tsi_next (&i))
    {
      enum tree_code code;
      tree prev_stmt;
      tree *stmt_p = tsi_container (i);

      prev_stmt = stmt;
      stmt = tsi_stmt (i);

      /* If the statement starts a new basic block or if we have determined
	 in a previous pass that we need to create a new block for STMT, do
	 so now.  */
      if (start_new_block || stmt_starts_bb_p (stmt, prev_stmt))
	{
	  bb = create_bb (NULL);
	  start_new_block = false;
	}

      code = TREE_CODE (stmt);

      /* Now add STMT to BB and create the subgraphs for special statement
	 codes.  */
      append_stmt_to_bb (stmt_p, bb);

      if (computed_goto_p (*stmt_p))
	found_computed_goto = true;

      /* All BIND_EXPRs except for the outermost one are lowered already.  */
      if (code == BIND_EXPR)
	abort ();

      /* If STMT is a basic block terminator, set START_NEW_BLOCK for the
	 next iteration.  Also compute any reachable exception handlers
	 for STMT.  */
      if (stmt && stmt_ends_bb_p (stmt))
	start_new_block = true;

      last = stmt;
    }

  if (last)
    return bb_for_stmt (last);

  return NULL;
}


static inline void
append_stmt_to_bb (tree *stmt_p, basic_block bb)
{
  set_bb_for_stmt (*stmt_p, bb);

  /* Update the head and tail of the block.  */
  if (bb->head_tree_p == NULL)
    bb->head_tree_p = stmt_p;

  bb->end_tree_p = stmt_p;
}


/* Add statement pointed by STMT_P to basic block BB and update BB's
   boundaries accordingly.  */

static inline void
prepend_stmt_to_bb (tree *stmt_p, basic_block bb)
{
  set_bb_for_stmt (*stmt_p, bb);

  /* Update the head and tail of the block.  */
  bb->head_tree_p = stmt_p;

  if (bb->end_tree_p == NULL)
    bb->end_tree_p = stmt_p;
}


/* Create and return a new basic block after bb AFTER.  */

basic_block
create_bb (basic_block after)
{
  basic_block bb;

  /* Create and initialize a new basic block.  */
  bb = alloc_block ();
  memset (bb, 0, sizeof (*bb));

  bb->index = last_basic_block;
  bb->flags = BB_NEW;

  /* Add the new block to the linked list of blocks.  */
  if (!after)
    after = EXIT_BLOCK_PTR->prev_bb;
  link_block (bb, after);

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

  /* Create an edge from entry to the first block with executable
     statements in it.  */
  make_edge (ENTRY_BLOCK_PTR, BASIC_BLOCK (0), 0);

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

  /* Clean up the graph and warn for unreachable code.  */
  cleanup_tree_cfg ();
}

/* Create edges for control statement at basic block BB.  */

static void
make_ctrl_stmt_edges (basic_block bb)
{
  tree last = last_stmt (bb);
  tree first = first_stmt (bb);

#if defined ENABLE_CHECKING
  if (last == NULL_TREE)
    abort();
#endif

  if (TREE_CODE (first) == LABEL_EXPR
      && NONLOCAL_LABEL (LABEL_EXPR_LABEL (first)))
    make_edge (ENTRY_BLOCK_PTR, bb, EDGE_ABNORMAL);

  switch (TREE_CODE (last))
    {
    case GOTO_EXPR:
      make_goto_expr_edges (bb);

      /* If this is potentially a nonlocal goto, then this should also
	 create an edge to the exit block.   */
      if (nonlocal_goto_p (last))
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

/* Create the edges for a COND_EXPR starting at block BB.
   At this point, both clauses must contain only simple gotos.  */

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

  /* Entry basic blocks for each component.  */
  then_label = GOTO_DESTINATION (COND_EXPR_THEN (entry));
  else_label = GOTO_DESTINATION (COND_EXPR_ELSE (entry));
  then_bb = label_to_block (then_label);
  else_bb = label_to_block (else_label);

  make_edge (bb, then_bb, EDGE_TRUE_VALUE);
  make_edge (bb, else_bb, EDGE_FALSE_VALUE);
}

/* Create the edges for a SWITCH_EXPR starting at block BB.
   At this point, the switch body has been lowered and the
   SWITCH_LABELS filled in, so this is in effect a multi-way branch.  */

static void
make_switch_expr_edges (basic_block bb)
{
  tree entry = last_stmt (bb);
  size_t i, n;
  tree vec;

  vec = SWITCH_LABELS (entry);
  n = TREE_VEC_LENGTH (vec);

  for (i = 0; i < n; ++i)
    {
      tree lab = CASE_LABEL (TREE_VEC_ELT (vec, i));
      basic_block label_bb = label_to_block (lab);
      make_edge (bb, label_bb, 0);
    }
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
      if (simple_goto_p (goto_t))
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

	  if (TREE_CODE (dest) == LABEL_DECL)
	    continue;

	  if (
	      /* Computed GOTOs.  Make an edge to every label block that has
		 been marked as a potential target for a computed goto.  */
	      (FORCED_LABEL (LABEL_EXPR_LABEL (target)) && for_call == 0)
	      /* Nonlocal GOTO target.  Make an edge to every label block that has
		 been marked as a potential target for a nonlocal goto.  */
	      || (NONLOCAL_LABEL (LABEL_EXPR_LABEL (target)) && for_call == 1))
	    {
	      make_edge (bb, target_bb, edge_flags);
	      break;
	    }
	}
    }
}


/*---------------------------------------------------------------------------
			       Flowgraph analysis
---------------------------------------------------------------------------*/

/* Remove unreachable blocks and other miscellaneous clean up work.  */

void
cleanup_tree_cfg (void)
{
  int orig_n_basic_blocks = n_basic_blocks;
  bool something_changed = true;

  timevar_push (TV_TREE_CLEANUP_CFG);
  pdom_info = NULL;

  /* These three transformations can cascade, so we iterate on them until nothing
     changes.  */
  while (something_changed)
    {
      something_changed = cleanup_control_flow ();
      something_changed |= thread_jumps ();
      something_changed |= remove_unreachable_blocks ();
    }

  if (pdom_info != NULL)
    {
      free_dominance_info (pdom_info);
      pdom_info = NULL;
    }
  compact_blocks ();

  /* If we expunged any basic blocks, then the dominator tree is
     no longer valid.  */
  if (n_basic_blocks != orig_n_basic_blocks)
    {
      basic_block bb;
      
      FOR_EACH_BB (bb)
	clear_dom_children (bb);
    }

#ifdef ENABLE_CHECKING
  verify_flow_info ();
#endif
  timevar_pop (TV_TREE_CLEANUP_CFG);
}

/* Walk the function tree removing unnecessary statements and
   variables.

     * Empty statement nodes are removed

     * Unnecessary TRY_FINALLY and TRY_CATCH blocks are removed

     * Unnecessary COND_EXPRs are removed

     * Some unnecessary BIND_EXPRs are removed

   Clearly more work could be done.  The trick is doing the analysis
   and removal fast enough to be a net improvement in compile times.

   Note that when we remove a control structure such as a COND_EXPR
   BIND_EXPR, or TRY block, we will need to repeat this optimization pass
   to ensure we eliminate all the useless code.  */

struct rusv_data
{
  bool repeat;
  bool remove_unused_vars;
  bool may_throw;
  bool may_branch;
};

static void remove_useless_stmts_and_vars_1 (tree *, struct rusv_data *);

static void
remove_useless_stmts_and_vars_cond (tree *stmt_p, struct rusv_data *data)
{
  tree then_clause, else_clause, cond;

  remove_useless_stmts_and_vars_1 (&COND_EXPR_THEN (*stmt_p), data);
  remove_useless_stmts_and_vars_1 (&COND_EXPR_ELSE (*stmt_p), data);

  then_clause = COND_EXPR_THEN (*stmt_p);
  else_clause = COND_EXPR_ELSE (*stmt_p);
  cond = COND_EXPR_COND (*stmt_p);

  /* We may not have been able to completely optimize away the condition
     previously due to the existence of a label in one arm.  If the label
     has since become unreachable then we may be able to zap the entire
     conditional here.  If so, replace the COND_EXPR and set up to repeat
     this optimization pass.  */
  if (integer_nonzerop (cond) && IS_EMPTY_STMT (else_clause))
    {
      *stmt_p = then_clause;
       data->repeat = true;
    }
  else if (integer_zerop (cond) && IS_EMPTY_STMT (then_clause))
    {
      *stmt_p = else_clause;
      data->repeat = true;
    }

  /* Notice branches to a common destination.  */
  else if (TREE_CODE (then_clause) == GOTO_EXPR
	   && TREE_CODE (else_clause) == GOTO_EXPR
	   && (GOTO_DESTINATION (then_clause)
	       == GOTO_DESTINATION (else_clause)))
    {
      *stmt_p = then_clause;
      data->repeat = true;
    }

  /* If the THEN/ELSE clause merely assigns a value to a variable/parameter
     which is already known to contain that value, then remove the useless
     THEN/ELSE clause.  */
  else if (TREE_CODE (cond) == VAR_DECL || TREE_CODE (cond) == PARM_DECL)
    {
      if (TREE_CODE (else_clause) == MODIFY_EXPR
	  && TREE_OPERAND (else_clause, 0) == cond
	  && integer_zerop (TREE_OPERAND (else_clause, 1)))
	COND_EXPR_ELSE (*stmt_p) = build_empty_stmt ();
    }
  else if ((TREE_CODE (cond) == EQ_EXPR || TREE_CODE (cond) == NE_EXPR)
	   && (TREE_CODE (TREE_OPERAND (cond, 0)) == VAR_DECL
	       || TREE_CODE (TREE_OPERAND (cond, 0)) == PARM_DECL)
	   && TREE_CONSTANT (TREE_OPERAND (cond, 1)))
    {
      tree clause = (TREE_CODE (cond) == EQ_EXPR
		     ? then_clause : else_clause);
      tree *location = (TREE_CODE (cond) == EQ_EXPR
			? &COND_EXPR_THEN (*stmt_p)
			: &COND_EXPR_ELSE (*stmt_p));

      if (TREE_CODE (clause) == MODIFY_EXPR
	  && TREE_OPERAND (clause, 0) == TREE_OPERAND (cond, 0)
	  && TREE_OPERAND (clause, 1) == TREE_OPERAND (cond, 1))
	*location = build_empty_stmt ();
    }
}

static void
remove_useless_stmts_and_vars_tf (tree *stmt_p, struct rusv_data *data)
{
  bool save_may_branch, save_may_throw;
  bool this_may_branch, this_may_throw;

  /* Collect may_branch and may_throw information for the body only.  */
  save_may_branch = data->may_branch;
  save_may_throw = data->may_throw;
  data->may_branch = false;
  data->may_throw = false;

  remove_useless_stmts_and_vars_1 (&TREE_OPERAND (*stmt_p, 0), data);

  this_may_branch = data->may_branch;
  this_may_throw = data->may_throw;
  data->may_branch |= save_may_branch;
  data->may_throw |= save_may_throw;

  remove_useless_stmts_and_vars_1 (&TREE_OPERAND (*stmt_p, 1), data);

  /* If the body is empty, then we can emit the FINALLY block without
     the enclosing TRY_FINALLY_EXPR.  */
  if (IS_EMPTY_STMT (TREE_OPERAND (*stmt_p, 0)))
    {
      *stmt_p = TREE_OPERAND (*stmt_p, 1);
      data->repeat = true;
    }

  /* If the handler is empty, then we can emit the TRY block without
     the enclosing TRY_FINALLY_EXPR.  */
  else if (IS_EMPTY_STMT (TREE_OPERAND (*stmt_p, 1)))
    {
      *stmt_p = TREE_OPERAND (*stmt_p, 0);
      data->repeat = true;
    }

  /* If the body neither throws, nor branches, then we can safely string
     the TRY and FINALLY blocks together.  We'll reassociate this in the
     main body of remove_useless_stmts_and_vars.  */
  else if (!this_may_branch && !this_may_throw)
    TREE_SET_CODE (*stmt_p, COMPOUND_EXPR);
}

static void
remove_useless_stmts_and_vars_tc (tree *stmt_p, struct rusv_data *data)
{
  bool save_may_throw, this_may_throw;
  tree_stmt_iterator i;
  tree stmt;

  /* Collect may_throw information for the body only.  */
  save_may_throw = data->may_throw;
  data->may_throw = false;

  remove_useless_stmts_and_vars_1 (&TREE_OPERAND (*stmt_p, 0), data);

  this_may_throw = data->may_throw;
  data->may_throw = save_may_throw;

  /* If the body cannot throw, then we can drop the entire TRY_CATCH_EXPR.  */
  if (!this_may_throw)
    {
      *stmt_p = TREE_OPERAND (*stmt_p, 0);
      data->repeat = true;
      return;
    }

  /* Process the catch clause specially.  We may be able to tell that
     no exceptions propagate past this point.  */

  this_may_throw = true;
  i = tsi_start (&TREE_OPERAND (*stmt_p, 1));
  stmt = tsi_stmt (i);

  switch (TREE_CODE (stmt))
    {
    case CATCH_EXPR:
      for (; !tsi_end_p (i); tsi_next (&i))
	{
	  stmt = tsi_stmt (i);
	  /* If we catch all exceptions, then the body does not
	     propagate exceptions past this point.  */
	  if (CATCH_TYPES (stmt) == NULL)
	    this_may_throw = false;
	  remove_useless_stmts_and_vars_1 (&CATCH_BODY (stmt), data);
	}
      break;

    case EH_FILTER_EXPR:
      if (EH_FILTER_MUST_NOT_THROW (stmt))
	this_may_throw = false;
      else if (EH_FILTER_TYPES (stmt) == NULL)
	this_may_throw = false;
      remove_useless_stmts_and_vars_1 (&EH_FILTER_FAILURE (stmt), data);
      break;

    default:
      /* Otherwise this is a cleanup.  */
      remove_useless_stmts_and_vars_1 (&TREE_OPERAND (*stmt_p, 1), data);

      /* If the cleanup is empty, then we can emit the TRY block without
	 the enclosing TRY_CATCH_EXPR.  */
      if (IS_EMPTY_STMT (TREE_OPERAND (*stmt_p, 1)))
	{
	  *stmt_p = TREE_OPERAND (*stmt_p, 0);
	  data->repeat = true;
	}
      break;
    }
  data->may_throw |= this_may_throw;
}

static void
remove_useless_stmts_and_vars_bind (tree *stmt_p, struct rusv_data *data)
{
  tree block;

  /* First remove anything underneath the BIND_EXPR.  */
  remove_useless_stmts_and_vars_1 (&BIND_EXPR_BODY (*stmt_p), data);

  /* If the BIND_EXPR has no variables, then we can pull everything
     up one level and remove the BIND_EXPR, unless this is the toplevel
     BIND_EXPR for the current function or an inlined function.

     When this situation occurs we will want to apply this
     optimization again.  */
  block = BIND_EXPR_BLOCK (*stmt_p);
  if (BIND_EXPR_VARS (*stmt_p) == NULL_TREE
      && *stmt_p != DECL_SAVED_TREE (current_function_decl)
      && (! block
	  || ! BLOCK_ABSTRACT_ORIGIN (block)
	  || (TREE_CODE (BLOCK_ABSTRACT_ORIGIN (block))
	      != FUNCTION_DECL)))
    {
      *stmt_p = BIND_EXPR_BODY (*stmt_p);
      data->repeat = true;
    }
  else if (data->remove_unused_vars)
    {
      /* If we were unable to completely eliminate the BIND_EXPR,
	 go ahead and prune out any unused variables.  We do not
	 want to expand them as that is a waste of time.  If we
	 happen to remove all the variables, then we may be able
	 to eliminate the BIND_EXPR as well.  */
      tree vars, prev_var;

      /* Walk all the variables associated with the BIND_EXPR.  */
      for (prev_var = NULL, vars = BIND_EXPR_VARS (*stmt_p);
	   vars;
	   vars = TREE_CHAIN (vars))
	{
	  struct var_ann_d *ann;
	  tree  var = vars;

	  /* We could have function declarations and the like
	     on this list.  Ignore them.  Also we do not deal with
	     static variables yet.   */
	  if (TREE_CODE (var) != VAR_DECL)
	    {
	      prev_var = vars;
	      continue;
	    }

	  /* Unlike for normal expressions, the tree-inline duplicates
	     static variables for BIND_EXPR in order to get debug info right.
	     We must work out the original expression.  */
	  if (TREE_STATIC (var) && DECL_ABSTRACT_ORIGIN (var))
	    var = DECL_ABSTRACT_ORIGIN (var);

	  /* Remove all unused, unaliased temporaries.  Also remove
	     unused, unaliased local variables during highly
	     optimizing compilations.  */
	  ann = var_ann (var);
	  if (ann
	      && ! ann->may_aliases
	      && ! ann->used
	      && ! ann->has_hidden_use
	      && ! TREE_ADDRESSABLE (var)
	      && (DECL_ARTIFICIAL (var) || optimize >= 2))
	    {
	      /* Remove the variable from the BLOCK structures.  */
	      if (block)
		remove_decl (vars,
			     (block
			      ? block
			      : DECL_INITIAL (current_function_decl)));

	      /* And splice the variable out of BIND_EXPR_VARS.  */
	      if (prev_var)
		TREE_CHAIN (prev_var) = TREE_CHAIN (vars);
	      else
		BIND_EXPR_VARS (*stmt_p) = TREE_CHAIN (vars);
	    }
	  else
	    prev_var = vars;
	}

      /* If there are no variables left after removing unused
	 variables, then go ahead and remove this BIND_EXPR.  */
      if (BIND_EXPR_VARS (*stmt_p) == NULL_TREE
	  && *stmt_p != DECL_SAVED_TREE (current_function_decl)
	  && (! block
	      || ! BLOCK_ABSTRACT_ORIGIN (block)
	      || (TREE_CODE (BLOCK_ABSTRACT_ORIGIN (block))
		  != FUNCTION_DECL)))
	{
	  *stmt_p = BIND_EXPR_BODY (*stmt_p);
	  data->repeat = true;
	}
    }
}

static void
remove_useless_stmts_and_vars_goto (tree_stmt_iterator i, tree *stmt_p,
				    struct rusv_data *data)
{
  tree_stmt_iterator tsi = i;

  if (factored_computed_goto_label
      && (GOTO_DESTINATION (*stmt_p)
	  == LABEL_EXPR_LABEL (factored_computed_goto_label)))
    {
      GOTO_DESTINATION (*stmt_p) = GOTO_DESTINATION (factored_computed_goto);
      return;
    }

  /* Step past the GOTO_EXPR statement.  */
  tsi_next (&tsi);
  if (! tsi_end_p (tsi))
    {
      /* If we are not at the end of this tree, then see if
	 we are at the target label.  If so, then this jump
	 is not needed.  */
      tree label;

      label = tsi_stmt (tsi);
      if (TREE_CODE (label) == LABEL_EXPR
	  && LABEL_EXPR_LABEL (label) == GOTO_DESTINATION (*stmt_p))
	{
	  data->repeat = true;
	  *stmt_p = build_empty_stmt ();
	  return;
	}
    }

  data->may_branch = true;
}

static void
remove_useless_stmts_and_vars_1 (tree *first_p, struct rusv_data *data)
{
  tree_stmt_iterator i;

  for (i = tsi_start (first_p); !tsi_end_p (i); tsi_next (&i))
    {
      tree *container_p = tsi_container (i);
      tree *stmt_p;
      enum tree_code code;

      while (TREE_CODE (*container_p) == COMPOUND_EXPR)
	{
	  /* If either operand of a COMPOUND_EXPR is an empty statement,
	     then remove the empty statement and the COMPOUND_EXPR itself.  */
	  if (IS_EMPTY_STMT (TREE_OPERAND (*container_p, 1)))
	    *container_p = TREE_OPERAND (*container_p, 0);
	  else if (IS_EMPTY_STMT (TREE_OPERAND (*container_p, 0)))
	    *container_p = TREE_OPERAND (*container_p, 1);
	  else
	    break;
	}

      /* Dive into control structures.  */
      stmt_p = tsi_stmt_ptr (i);
      code = TREE_CODE (*stmt_p);
      switch (code)
	{
	case COND_EXPR:
	  remove_useless_stmts_and_vars_cond (stmt_p, data);
	  break;
	case TRY_FINALLY_EXPR:
	  remove_useless_stmts_and_vars_tf (stmt_p, data);
	  break;
	case TRY_CATCH_EXPR:
	  remove_useless_stmts_and_vars_tc (stmt_p, data);
	  break;
	case BIND_EXPR:
	  remove_useless_stmts_and_vars_bind (stmt_p, data);
	  break;
	case GOTO_EXPR:
	  remove_useless_stmts_and_vars_goto (i, stmt_p, data);
	  break;
	case RETURN_EXPR:
	  data->may_branch = true;
	  break;
	case MODIFY_EXPR:
	case CALL_EXPR:
	  if (tree_could_throw_p (*stmt_p))
	    data->may_throw = true;
	  break;
	default:
	  break;
	}

      /* We need to keep the tree in gimple form, so we may have to
	 re-rationalize COMPOUND_EXPRs.  */
      if (TREE_CODE (*container_p) == COMPOUND_EXPR
	  && TREE_CODE (TREE_OPERAND (*container_p, 0)) == COMPOUND_EXPR)
	*container_p = rationalize_compound_expr (*container_p);
    }
}

void
remove_useless_stmts_and_vars (tree *first_p, bool remove_unused_vars)
{
  struct rusv_data data;
  do
    {
      memset (&data, 0, sizeof (data));
      data.remove_unused_vars = remove_unused_vars;
      remove_unused_vars = false;

      remove_useless_stmts_and_vars_1 (first_p, &data);
    }
  while (data.repeat);

  factored_computed_goto = NULL;
  factored_computed_goto_label = NULL;
}

/* Delete all unreachable basic blocks.  Return true if any unreachable
   blocks were detected and removed.  */

bool
remove_unreachable_blocks (void)
{
  int i;
  bool ret = false;
  basic_block bb;

  find_unreachable_blocks ();

  for (i = last_basic_block - 1; i >= 0; i--)
    {
      bb = BASIC_BLOCK (i);

      if (bb == NULL)
	continue;

      if (!(bb->flags & BB_REACHABLE))
	{
	  remove_bb (bb);
	  ret = true;
	}
    }

  return ret;
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
  bsi_list_p stack;
  location_t loc;
  bool empty = true;

  dump_file = dump_begin (TDI_cfg, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "Removing basic block %d\n", bb->index);
      if (dump_flags & TDF_DETAILS)
	{
	  dump_bb (bb, dump_file, 0);
	  fprintf (dump_file, "\n");
	}
      dump_end (TDI_cfg, dump_file);
      dump_file = NULL;
    }

  /* Remove all the instructions in the block.  Do so in reverse order
     so that we remove all the containing COMPOUND_EXPRs as well.  */
  FOR_EACH_BSI_IN_REVERSE (stack, bb, i)
    {
      tree stmt = bsi_stmt (i);

      set_bb_for_stmt (stmt, NULL);

      if (get_lineno (stmt) != -1
	  /* Don't warn for removed gotos.  Gotos are often removed due to jump threading,
	     thus resulting into bogus warnings.  Not great, since this way we lose warnings
	     for gotos in the original program that are indeed unreachable.  */
	  && TREE_CODE (stmt) != GOTO_EXPR)
	{
	  loc.file = get_filename (stmt);
	  loc.line = get_lineno (stmt);
	  empty = false;
	}
      bsi_remove (&i);
    }

  /* If requested, give a warning that the first statement in the
     block is unreachable.  We walk statements backwards in the
     loop above, so the last statement we process is the first statement
     in the block.  */
  if (warn_notreached && !empty)
    warning ("%Hwill never be executed", &loc);

  if (bb->head_tree_p)
    set_bb_for_stmt (*bb->head_tree_p, NULL);

  if (bb->end_tree_p)
    set_bb_for_stmt (*bb->end_tree_p, NULL);

  remove_phi_nodes_and_edges_for_unreachable_block (bb);

  /* If we have pdom information, then we must also make sure to
     clean up the dominance information.  */
  if (pdom_info)
    delete_from_dominance_info (pdom_info, bb);

  /* Remove the basic block from the array.  */
  expunge_block (bb);
}

/* Remove statement pointed by iterator I.

    Note that this function will wipe out control statements that
    may span multiple basic blocks.  Make sure that you really
    want to remove the whole control structure before calling this
    function.  Remove the annotations if REMOVE_ANNOTATIONS is true.  */

static void
remove_bsi_from_block (block_stmt_iterator *i, bool remove_annotations)
{
  tree t = *(i->tp);

  if (is_exec_stmt (t))
    {
      if (TREE_CODE (t) == COMPOUND_EXPR)
	{
	  basic_block op0_bb = bb_for_stmt (TREE_OPERAND (t, 0));
	  basic_block op1_bb = bb_for_stmt (TREE_OPERAND (t, 1));

	  remove_stmt (&TREE_OPERAND (t, 0), remove_annotations);

	  /* If both operands are empty and they are not associated
	     with different basic blocks, then delete the whole
	     COMPOUND_EXPR.  */
	  if (IS_EMPTY_STMT (TREE_OPERAND (t, 1))
	      && (op0_bb == NULL
		  || op1_bb == NULL
		  || op0_bb == op1_bb))
	    remove_stmt (i->tp, remove_annotations);
	}
      else
	remove_stmt (i->tp, remove_annotations);
    }
  
  bsi_next (i);
}

void
bsi_remove (block_stmt_iterator *i)
{
  remove_bsi_from_block (i, true);
}

/* Move the statement at FROM so it comes right after the statement at
   TO.  */
void 
bsi_move_after (block_stmt_iterator from, block_stmt_iterator to)
{
  tree stmt = bsi_stmt (from);
  remove_bsi_from_block (&from, false);
  bsi_insert_after (&to, stmt, BSI_SAME_STMT);
} 

/* Move the statement at FROM so it comes right before the statement
   at TO.  */
void 
bsi_move_before (block_stmt_iterator from, block_stmt_iterator to)
{
  tree stmt = bsi_stmt (from);
  remove_bsi_from_block (&from, false);
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

  replace_stmt (bsi.tp, &stmt);
  modify_stmt (bsi_stmt (bsi));
}

/* Remove statement *STMT_P.

   Update all references associated with it.  Note that this function will
   wipe out control statements that may span multiple basic blocks.  Make
   sure that you really want to remove the whole control structure before
   calling this function.
   Reset the annotations if REMOVE_ANNOTATIONS is true.  */

static void
remove_stmt (tree *stmt_p, bool remove_annotations)
{
  varray_type vdefs;
  size_t i;
  varray_type defs;
  tree stmt = *stmt_p;
  basic_block bb = bb_for_stmt (stmt);
  int update_head = 0;
  int update_end = 0;

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
      stmt_ann_t ann = stmt_ann (stmt);
      defs = def_ops (ann);
      for (i = 0; defs && i < VARRAY_ACTIVE_SIZE (defs); i++)
	{
	  tree *def_p = VARRAY_TREE_PTR (defs, i);
	  if (TREE_CODE (*def_p) == SSA_NAME)
	    SSA_NAME_DEF_STMT (*def_p) = build_empty_stmt ();
	}
      
      vdefs = vdef_ops (ann);
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

  /* If we are removing a COMPOUND_EXPR, we may need to update block
     head/tail pointers which point into operands of the COMPOUND_EXPR.  */
  if (TREE_CODE (stmt) == COMPOUND_EXPR)
    {
      basic_block op0_bb = bb_for_stmt (TREE_OPERAND (stmt, 0));
      basic_block op1_bb = bb_for_stmt (TREE_OPERAND (stmt, 1));

#ifdef ENABLE_CHECKING
      if (op0_bb && op1_bb && op0_bb != op1_bb)
	abort ();
#endif

      if (op0_bb)
	bb = op0_bb;
      else
	bb = op1_bb;

      if (bb
	  && (&TREE_OPERAND (stmt, 0) == bb->head_tree_p
	      || &TREE_OPERAND (stmt, 1) == bb->head_tree_p))
	update_head = 1;

      if (bb
	  && (&TREE_OPERAND (stmt, 0) == bb->end_tree_p
	      || &TREE_OPERAND (stmt, 1) == bb->end_tree_p))
	update_end = 1;
    }

  /* Replace STMT with an empty statement.  */
  *stmt_p = build_empty_stmt ();
  if (bb)
    set_bb_for_stmt (*stmt_p, bb);

  if (update_head)
    bb->head_tree_p = stmt_p;

  if (update_end)
    bb->end_tree_p = stmt_p;
}

/* Examine BB to determine if it is a forwarding block (a block which only
   transfers control to a new destination).  If BB is a forwarding block,
   then return the ultimate destination.  */

basic_block
tree_block_forwards_to (basic_block bb)
{
  block_stmt_iterator bsi;
  bb_ann_t ann = bb_ann (bb);

  /* If this block is not forwardable, then avoid useless work.  */
  if (! ann->forwardable)
    return NULL;

  /* Set this block to not be forwardable.  This prevents infinite loops since
     any block currently under examination is considered non-forwardable.  */
  ann->forwardable = 0;

  /* No forwarding is possible if this block is a special block (ENTRY/EXIT),
     this block has more than one successor, this block's single successor is
     reached via an abnormal edge, this block has phi nodes, or this block's
     single successor has phi nodes.  */
  if (bb == EXIT_BLOCK_PTR
      || bb == ENTRY_BLOCK_PTR
      || !bb->succ
      || bb->succ->succ_next
      || bb->succ->dest == EXIT_BLOCK_PTR
      || (bb->succ->flags & EDGE_ABNORMAL) != 0
      || phi_nodes (bb)
      || phi_nodes (bb->succ->dest))
    return NULL;

  /* Walk past any labels or empty statements at the start of this block.  */
  bsi = bsi_start (bb);
  while (! bsi_end_p (bsi)
	 && (IS_EMPTY_STMT (bsi_stmt (bsi))
	     || TREE_CODE (bsi_stmt (bsi)) == LABEL_EXPR))
    bsi_next (&bsi);

  /* If we reached the end of this block, or hit a GOTO_EXPR to an known
     location, then we may be able to optimize this case.  */
  if (bsi_end_p (bsi)
      || (bsi_stmt (bsi)
	  && TREE_CODE (bsi_stmt (bsi)) == GOTO_EXPR
	  && TREE_CODE (GOTO_DESTINATION (bsi_stmt (bsi))) == LABEL_DECL))
    {
      basic_block dest;

      /* Recursive call to pick up chains of forwarding blocks.  */
      dest = tree_block_forwards_to (bb->succ->dest);
      if (dest)
	{
	  ann->forwardable = 1;
	  return dest;
	}

      /* If we hit the end of the block, then we may need to insert a label
	 at this block's destination.  */
      if (bsi_end_p (bsi))
	{
	  tree stmt;

	  bsi = bsi_start (bb->succ->dest);

	  /* It's not clear if we can safely insert the label in this case.  */
          if (bsi_end_p (bsi))
	    return NULL;

	  stmt = bsi_stmt (bsi);

	  /* If our new destination does not start with a label,
	     then add one.  */
	  if (TREE_CODE (stmt) != LABEL_EXPR)
	    {
	      /* DEST does not start with a label, add one.  */
	      stmt = create_artificial_label ();
	      stmt = build1 (LABEL_EXPR, void_type_node, stmt);
	      bsi_insert_before (&bsi, stmt, BSI_NEW_STMT);
	    }
	}

      /* This block forwards to bb->succ->dest.  */
      ann->forwardable = 1;
      return bb->succ->dest;
    }

  /* No forwarding possible.  */
  return NULL;
}

/* Try to remove superfluous control structures.  */

static bool
cleanup_control_flow (void)
{
  basic_block bb;
  block_stmt_iterator bsi;
  bool retval = false;

  FOR_EACH_BB (bb)
    {
      bsi = bsi_last (bb);

      if (!bsi_end_p (bsi))
	{
	  enum tree_code code = TREE_CODE (bsi_stmt (bsi));
	  if (code == COND_EXPR)
	    retval |= cleanup_cond_expr_graph (bb, bsi);
	  else if (code == SWITCH_EXPR)
	    retval |= cleanup_switch_expr_graph (bb, bsi);
	}
    }
  return retval;
}

/* Disconnect an unreachable block in the conditional expression starting
   at block BB.  */

bool
cleanup_cond_expr_graph (basic_block bb, block_stmt_iterator bsi)
{
  tree val;
  edge taken_edge;
  bool retval = false;
  tree cond_expr = bsi_stmt (bsi);

#if defined ENABLE_CHECKING
  if (cond_expr == NULL_TREE
      || TREE_CODE (cond_expr) != COND_EXPR
      || !bb->succ)
    abort ();
#endif

  if (bb->succ->succ_next)
    {
      edge e, next;

      val = COND_EXPR_COND (cond_expr);
      taken_edge = find_taken_edge (bb, val);
      if (!taken_edge)
	return false;

      /* Remove all the edges except the one that is always executed.  */
      for (e = bb->succ; e; e = next)
	{
	  next = e->succ_next;
	  if (e != taken_edge)
	    {
	      ssa_remove_edge (e);
	      retval = true;
	    }
	}
    }
  else
    taken_edge = bb->succ;

  if (taken_edge->flags & EDGE_TRUE_VALUE)
    bsi_replace (bsi, COND_EXPR_THEN (cond_expr));
  else if (taken_edge->flags & EDGE_FALSE_VALUE)
    bsi_replace (bsi, COND_EXPR_ELSE (cond_expr));
  else
    abort ();

  taken_edge->flags &= ~(EDGE_TRUE_VALUE | EDGE_FALSE_VALUE);

  return retval;
}

/* Disconnect unreachable blocks in the 'switch' expression starting at
   block SWITCH_BB.

   If the switch condition of the SWITCH_EXPR node in block SWITCH_BB is
   constant, disconnect all the subgraphs for all the case labels that will
   never be taken.  */

bool
cleanup_switch_expr_graph (basic_block bb, block_stmt_iterator bsi)
{
  tree switch_expr = bsi_stmt (bsi);
  tree switch_val, taken_case;
  basic_block dest_bb;
  bool retval;

#if defined ENABLE_CHECKING
  if (switch_expr == NULL_TREE || TREE_CODE (switch_expr) != SWITCH_EXPR)
    abort ();
#endif

  retval = false;
  if (bb->succ->succ_next)
    {
      edge e, next;

      /* Multiple destination edges.  If we've got a integer constant,
	 we can look up the value in the switch condition and replace.  */
      switch_val = SWITCH_COND (switch_expr);
      if (TREE_CODE (switch_val) != INTEGER_CST)
	return retval;

      taken_case = find_case_label_for_value (switch_expr, switch_val);
      dest_bb = label_to_block (CASE_LABEL (taken_case));

      /* Remove all the edges that will never be taken.  */
      for (e = bb->succ; e ; e = next)
	{
	  next = e->succ_next;
	  if (e->dest != dest_bb)
            {
	      ssa_remove_edge (e);
	      retval = true;
	    }
	}
    }
  else
    {
      /* There is only one destination edge, which means that all of
	 the labels go to the same place.  */
      dest_bb = bb->succ->dest;
      taken_case = TREE_VEC_ELT (SWITCH_LABELS (switch_expr), 0);
    }

  /* Simplify the SWITCH_EXPR itself.  */
  taken_case = build (GOTO_EXPR, void_type_node, CASE_LABEL (taken_case));
  bsi_replace (bsi, taken_case);

  return retval;
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
  always_false = integer_zerop (val);
  always_true = integer_nonzerop (val);

  /* If VAL is a constant but it can't be reduced to a 0 or a 1, then
     we don't really know which edge will be taken at runtime.  This
     may happen when comparing addresses (e.g., if (&var1 == 4))  */
  if (!always_false && !always_true)
    return NULL;

  for (e = bb->succ; e; e = e->succ_next)
    if (((e->flags & EDGE_TRUE_VALUE) && always_true)
	|| ((e->flags & EDGE_FALSE_VALUE) && always_false))
      return e;

  /* There always should be an edge that is taken.  */
  abort ();
}

/* Given a constant value VAL and the entry block BB to a SWITCH_EXPR
   statement, determine which edge will be taken out of the block.  Return
   NULL if any edge may be taken.  */

static edge
find_taken_edge_switch_expr (basic_block bb, tree val)
{
  tree switch_expr, taken_case;
  basic_block dest_bb;
  edge e;

  if (TREE_CODE (val) != INTEGER_CST)
    return NULL;

  switch_expr = last_stmt (bb);
  taken_case = find_case_label_for_value (switch_expr, val);
  dest_bb = label_to_block (CASE_LABEL (taken_case));

  e = find_edge (bb, dest_bb);
  if (!e)
    abort ();
  return e;
}

/* Return the CASE_LABEL_EXPR that SWITCH_EXPR will take for VAL.  */

static tree
find_case_label_for_value (tree switch_expr, tree val)
{
  tree vec = SWITCH_LABELS (switch_expr);
  size_t i, n = TREE_VEC_LENGTH (vec);
  tree default_case = NULL;

  for (i = 0; i < n; ++i)
    {
      tree t = TREE_VEC_ELT (vec, i);

      if (CASE_LOW (t) == NULL)
	default_case = t;
      else if (CASE_HIGH (t) == NULL)
	{
	  /* A `normal' case label.  */
	  if (simple_cst_equal (CASE_LOW (t), val) == 1)
	    return t;
	}
      else
	{
	  /* A case range.  We can only handle integer ranges.  */
	  if (tree_int_cst_compare (CASE_LOW (t), val) <= 0
	      && tree_int_cst_compare (CASE_HIGH (t), val) >= 0)
	    return t;
	}
    }

  if (!default_case)
    abort ();
  return default_case;
}

/* If all the phi nodes in DEST have alternatives for E1 and E2 and
   those alterantives are equal in each of the PHI nodes, then return
   nonzero, else return zero.  */

static int
phi_alternatives_equal (basic_block dest, edge e1, edge e2)
{
  tree phi, val1, val2;
  int n1, n2;

  for (phi = phi_nodes (dest); phi; phi = TREE_CHAIN (phi))
    {
      n1 = phi_arg_from_edge (phi, e1);
      n2 = phi_arg_from_edge (phi, e2);

#ifdef ENABLE_CHECKING
      if (n1 < 0 || n2 < 0)
	abort ();
#endif

      val1 = PHI_ARG_DEF (phi, n1);
      val2 = PHI_ARG_DEF (phi, n2);

      if (!operand_equal_p (val1, val2, false))
	return false;
    }

  return true;
}

/*---------------------------------------------------------------------------
			 Code insertion and replacement
---------------------------------------------------------------------------*/

/* Insert basic block NEW_BB before block BB.  */

void
insert_bb_before (basic_block new_bb, basic_block bb)
{
  edge e;

  /* Reconnect BB's predecessors to NEW_BB.  */
  for (e = bb->pred; e; e = e->pred_next)
    redirect_edge_succ (e, new_bb);

  /* Create the edge NEW_BB -> BB.  */
  make_edge (new_bb, bb, 0);
}



/*---------------------------------------------------------------------------
			      Debugging functions
---------------------------------------------------------------------------*/

/* Dump tree-specific information of BB to file OUTF.  */

void
tree_dump_bb (basic_block bb, FILE *outf, int indent)
{
  dump_generic_bb (outf, bb, indent, TDF_VOPS);
}

/* Dump a basic block on stderr.  */

void
debug_tree_bb (basic_block bb)
{
  dump_bb (bb, stderr, 0);
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
  if (flags & TDF_DETAILS)
    {
      const char *funcname
	= (*lang_hooks.decl_printable_name) (current_function_decl, 2);

      fputc ('\n', file);
      fprintf (file, ";; Function %s\n\n", funcname);
      fprintf (file, ";; \n%d basic blocks, %d edges, last basic block %d.\n\n",
	       n_basic_blocks, n_edges, last_basic_block);

      brief_dump_cfg (file);
      fprintf (file, "\n");
    }

  if (flags & TDF_STATS)
    dump_cfg_stats (file);

  dump_function_to_file (current_function_decl, file, flags | TDF_BLOCKS);
}

/* Dump CFG statistics on FILE.  */

void
dump_cfg_stats (FILE *file)
{
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
	  head_line = get_lineno (*(bb->head_tree_p));
	}
      else
        head_name = "no-statement";

      if (last)
        {
	  end_code = TREE_CODE (last);
	  end_name = tree_code_name[end_code];
	  end_line = get_lineno (*(bb->end_tree_p));
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
   rtl.h)  */

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
computed_goto_p (tree t)
{
  return (TREE_CODE (t) == GOTO_EXPR
          && TREE_CODE (GOTO_DESTINATION (t)) != LABEL_DECL);
}

/* Return true when GOTO is an non-local goto.  */
bool
nonlocal_goto_p (tree stmt)
{
 return ((TREE_CODE (GOTO_DESTINATION (stmt)) == LABEL_DECL
	   && (decl_function_context (GOTO_DESTINATION (stmt))
	       != current_function_decl))
	  || (TREE_CODE (GOTO_DESTINATION (stmt)) != LABEL_DECL
	      && DECL_CONTEXT (current_function_decl)));
}

/* Checks whether EXPR is a simple local goto.  */

bool
simple_goto_p (tree expr)
{
  return  (TREE_CODE (expr) == GOTO_EXPR
	   && TREE_CODE (GOTO_DESTINATION (expr)) == LABEL_DECL
	   && ! NONLOCAL_LABEL (GOTO_DESTINATION (expr))
	   && (decl_function_context (GOTO_DESTINATION (expr))
	       == current_function_decl));
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

  /* LABEL_EXPRs start a new basic block only if the preceding statement wasn't
     a label of the same type.  This prevents the creation of consecutive
     blocks that have nothing but a single label.  */
  code = TREE_CODE (t);
  if (code == LABEL_EXPR)
    {
      /* Nonlocal and computed GOTO targets always start a new block.  */
      if (code == LABEL_EXPR
	  && (NONLOCAL_LABEL (LABEL_EXPR_LABEL (t))
	      || FORCED_LABEL (LABEL_EXPR_LABEL (t))))
	return true;

      if (prev_t && TREE_CODE (prev_t) == code)
	{
	  cfg_stats.num_merged_labels++;

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
  return (is_ctrl_stmt (t)
	  || is_ctrl_altering_stmt (t));
}


/* Remove all the blocks and edges that make up the flowgraph.  */

void
delete_tree_cfg (void)
{
  if (n_basic_blocks > 0)
    free_blocks_annotations ();

  free_basic_block_vars (0);
}


/* Return a pointer to the first executable statement starting at ENTRY_P.  */

static tree *
first_exec_stmt (tree *entry_p)
{
  tree_stmt_iterator i;
  tree stmt;

  for (i = tsi_start (entry_p); !tsi_end_p (i); tsi_next (&i))
    {
      stmt = tsi_stmt (i);
      if (!stmt)
        continue;

      /* Note that we actually return the container for the executable
	 statement, not the statement itself.  This is to allow the caller to
	 start iterating from this point.  */
      if (is_exec_stmt (stmt))
	return tsi_container (i);
    }

  return NULL;
}

/* Return the first statement in basic block BB, stripped of any NOP
   containers.  */

tree
first_stmt (basic_block bb)
{
  block_stmt_iterator i;

  i = bsi_start (bb);
  return (!bsi_end_p (i)) ? bsi_stmt (i) : NULL_TREE;
}

/* Return the last statement in basic block BB, stripped of any NOP
   containers.

   empty statement nodes are never returned. NULL is returned if there are
   no such statements.  */

tree
last_stmt (basic_block bb)
{
  block_stmt_iterator b;

  b = bsi_last (bb);
  return (!bsi_end_p (b)) ? bsi_stmt (b) : NULL_TREE;
}


/* Return a pointer to the last statement in block BB.  */

tree *
last_stmt_ptr (basic_block bb)
{
  block_stmt_iterator last;

  last = bsi_last (bb);
  return (!bsi_end_p (last)) ? bsi_stmt_ptr (last) : NULL;
}


/* Initialize a block stmt iterator with a container that contains stmt's
   in a specified basic block. If the first real stmt is not in the
   specified basic block, then return an empty iterator.  */

static block_stmt_iterator
bsi_init (tree *tp, basic_block bb)
{
  block_stmt_iterator i;
  tree stmt;

  i.tp = tp;
  i.context = NULL_TREE;
  /* If the first statement is empty, get the next non-empty one.  */
  if (i.tp != NULL)
    {
      stmt = bsi_stmt (i);
      if (stmt == NULL_TREE)
	bsi_next_in_bb (&i, bb);
    }

  /* Now check that its the right basic block.  */
  if (i.tp != NULL)
    {
      stmt = bsi_stmt (i);
      if (bb_for_stmt (stmt) != bb)
        i.tp = NULL;
    }

  return i;
}

/* Similar to tsi_step() but stops at basic block boundaries and ignores
   empty statement nodes inside a basic block.  */

void
bsi_next_in_bb (block_stmt_iterator *i, basic_block bb)
{
  tree t, stmt = NULL_TREE;

  /* Go to the next statement skipping over empty statements we may find.  */
  do
    {
      t = *(i->tp);
      if (TREE_CODE (t) == COMPOUND_EXPR)
	i->tp = &(TREE_OPERAND (t, 1));
      else
	{
	  /* We ran out of statements.  Clear the iterator and stop
	     searching.  */
	  i->tp = NULL;
	  break;
	}

      stmt = bsi_stmt (*i);
    }
  while (IS_EMPTY_STMT (stmt));

  if (i->tp && bb_for_stmt (stmt) != bb)
    i->tp = NULL;

  if (i->tp == NULL && i->context != NULL_TREE)
    {
      /* If we haven't got a statement, and we have context, pop the state and
         traverse to the next statement.  */
      i->tp = (tree *)TREE_VALUE (i->context);
      i->context = TREE_PURPOSE (i->context);

      /* FIXME.  Hack to recover BB for cases when we are stepping out of a
	 removed statement.  If bsi_remove() has been called on the
	 last statement of a BIND_EXPR body, the next call to
	 bsi_next() will retrieve a NULL basic block from the just deleted
	 statement, so that BB will be NULL.  We restore BB using the
	 BIND_EXPR node itself.  */
      bb = bb_for_stmt (*(i->tp));

      bsi_next_in_bb (i, bb);
    }
}

/* Similar to tsi_start() but initializes the iterator at the first
   statement in basic block BB which isn't an empty statement node.

   NULL is returned if there are no such statements.  */

block_stmt_iterator
bsi_start (basic_block bb)
{
  block_stmt_iterator i;
  tree t;

  if (bb && bb->index != INVALID_BLOCK)
    {
      tree *tp = bb->head_tree_p;
      i = bsi_init (tp, bb);
      if (i.tp != NULL)
	{
	  /* If we get back a statement which is not within this basic
	     block, that is wrong!  */
	  t = bsi_stmt (i);
	  if (t != NULL_TREE && bb_for_stmt (t) != bb)
	    abort ();
	}
      }
    else
      i.tp = NULL;

  /* If there are no stmts in the block, set the context to point to the
     basic block in case we try to insert a stmt with this iterator.  */

  if (i.tp == NULL)
    i.context = (tree) bb;

  return i;
}

/* This routine will return a block iterator which points to the last stmt in
   a basic block, if there is one.  */

block_stmt_iterator
bsi_last (basic_block bb)
{
  block_stmt_iterator b, tmp;

  if (bb == NULL || bb->index == INVALID_BLOCK)
    {
      b.tp = NULL;
      return b;
    }

  b = bsi_init (bb->end_tree_p, bb);

  /* If the last stmt pointer isn't something a BSI can represent (ie, an
     empty statement node), then find the last stmt the slow way.  */
  if (b.tp == NULL)
    {
      for (tmp = b = bsi_start (bb); !bsi_end_p (tmp); bsi_next (&tmp))
        b = tmp;
    }

  return b;
}


/* Find the previous iterator value.  */

void
bsi_prev (block_stmt_iterator *i)
{
  block_stmt_iterator bi, next;

  bi = bsi_start (bb_for_stmt (bsi_stmt (*i)));
  if (bi.tp != i->tp)
    {
      for ( ; !bsi_end_p (bi); bi = next)
	{
	  next = bi;
	  bsi_next (&next);
	  if (next.tp == i->tp)
	    {
	      i->tp = bi.tp;
	      i->context = bi.context;
	      return;
	    }
	}
    }

  i->tp = NULL;
  bi.context = NULL_TREE;
  return;
}


/* Initialize a block_stmt_iterator with a statement pointed to by a tree
   iterator. If this cannot be done, a NULL iterator is returned.  */

block_stmt_iterator
bsi_from_tsi (tree_stmt_iterator ti)
{
  basic_block bb;
  tree stmt;
  block_stmt_iterator bi;

  stmt = tsi_stmt (ti);
  if (stmt)
    {
      bb = bb_for_stmt (stmt);
      if (bb)
        {
	  for (bi = bsi_start (bb); !bsi_end_p (bi); bsi_next (&bi))
	    if (bi.tp == ti.tp)
	      return bi;
	}
    }

  bi.tp = NULL;
  bi.context = NULL_TREE;
  return bi;
}


/* This is a more efficient version of bsi_from_tsi which can be used when
   we are changing a bsi in a known way. Specifically, we know that the tsi
   is located in the same 'context' area (ie, within the same BIND_EXPR),
   so that the context doesn't have to be re-evaluated. This is primarily for
   the insert routines which know what they are doing.  */

static inline void
bsi_update_from_tsi (block_stmt_iterator *bsi, tree_stmt_iterator tsi)
{
  /* Pretty simple right now, but its better to have this in an interface
     rather than exposed right in the insert routine.  */
  bsi->tp = tsi.tp;
}


/* Insert statement T into basic block BB.  */

void
set_bb_for_stmt (tree t, basic_block bb)
{
  stmt_ann_t ann;

  do
    {
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
      t = (TREE_CODE (t) == COMPOUND_EXPR) ? TREE_OPERAND (t, 0) : NULL_TREE;
    }
  while (t);
}


/* Insert routines.  */

/* Because of the way containers and CE nodes are maintained, linking a new
   stmt in can have significant consequences on the basic block information.
   The basic block structure maintains the head and tail pointers as
   containers, or pointers to the pointer to a node.

   Linking a new stmt after the last stmt in a block changes not only the
   tail pointer of this block, but the container for the head of the next block
   is now contained in a new node, so the head pointer must be updated in
   a that different block. If it is the only statement in that block, then
   the end pointer needs to be updated too.

   Linking a stmt after the penultimate (next to last) stmt in a block adds
   a node which has the container to the end block stmt, so the block end must
   be updated in this case.

   And the third case is the simple one when we are adding a new stmt to the
   end of a chain which also ends a block.  */

/* This routine returns a tree stmt iterator which points to the original
   stmt before we did an insert.  The first parameter is a tree stmt iterator
   which is updated to point to the new stmt.  */

static tree_stmt_iterator
bsi_link_after (tree_stmt_iterator *this_tsi, tree t, basic_block curr_bb)
{
  enum link_after_cases { NO_UPDATE,
			  END_OF_CHAIN,
			  PENULTIMATE_STMT,
			  AFTER_LAST_STMT,
			  JUST_UPDATE };
  enum link_after_cases update_form = NO_UPDATE;
  basic_block bb;
  tree_stmt_iterator same_tsi, next_tsi;
  tree *this_container;

  this_container = tsi_container (*this_tsi);
  same_tsi = next_tsi = *this_tsi;
  tsi_next (&next_tsi);
  if (tsi_end_p (next_tsi))
    update_form = END_OF_CHAIN;
  /* This is the penultimate case. The next stmt is actually the last stmt
     in the block, so we need to update the tail pointer to be the new
     container for that stmt after we link in the new one.  */
  else if (tsi_container (next_tsi) == curr_bb->end_tree_p)
    update_form = PENULTIMATE_STMT;
  /* The ugly case which requires updating pointers in a different
     basic block.  */
  else if (this_container == curr_bb->end_tree_p)
    {
      /* Double check to make sure the next stmt is indeed the head of
	 a different block.  */
      bb = bb_for_stmt (*tsi_container (next_tsi));
      if (bb
	  && bb != curr_bb
	  && bb->head_tree_p == tsi_container (next_tsi))
	update_form = AFTER_LAST_STMT;
      else
	/* There are nops between the end of this block and the beginning
	   of the next, so we only need to update our end pointer.  */
	update_form = JUST_UPDATE;
    }

  tsi_link_after (&same_tsi, t, TSI_SAME_STMT);
  if (update_form == END_OF_CHAIN)
    {
      /* If the stmt was added to the end of a chain, the linking routines
	 created a new CE node to be a container for what use to be the
	 last stmt in the chain.  This container needs to have the BB info
	 set for it as well.  */
      set_bb_for_stmt (*tsi_container (same_tsi), curr_bb);
    }
  *this_tsi = same_tsi;
  tsi_next (this_tsi);
  set_bb_for_stmt (*tsi_container (*this_tsi), curr_bb);

  switch (update_form)
    {
    case END_OF_CHAIN:
    case JUST_UPDATE:
      if (this_container == curr_bb->end_tree_p)
	curr_bb->end_tree_p = tsi_container (*this_tsi);
      break;

    case PENULTIMATE_STMT:
      next_tsi = *this_tsi;
      tsi_next (&next_tsi);
      curr_bb->end_tree_p = tsi_container (next_tsi);
      break;

    case AFTER_LAST_STMT:
      /* This is now the end of block.  */
      curr_bb->end_tree_p = tsi_container (*this_tsi);

      /* And the next basic block's head needs updating too.  */
      next_tsi = *this_tsi;
      tsi_next (&next_tsi);
      bb = bb_for_stmt (tsi_stmt (next_tsi));
      /* Oh, and we also need to check if this is both the head *and* the
	 end of the next block.  */
      if (bb->end_tree_p == bb->head_tree_p)
	bb->end_tree_p = tsi_container (next_tsi);
      bb->head_tree_p = tsi_container (next_tsi);
      break;

    default:
      break;
    }

  return same_tsi;
}


/* This routine inserts a stmt after the stmt iterator passed in.
   The final parameter determines whether the statement iterator
   is updated to point to the new stmt, or left pointing to the original
   statement.  (Which may have a different container, by the way.)  */

void
bsi_insert_after (block_stmt_iterator *curr_bsi, tree t,
		  enum bsi_iterator_update mode)
{
  tree_stmt_iterator inserted_tsi, same_tsi;
  basic_block curr_bb;
  tree *curr_container;
  tree curr_stmt, inserted_stmt;

  curr_container = bsi_container (*curr_bsi);
  if (curr_container)
    {
      curr_stmt = bsi_stmt (*curr_bsi);
      curr_bb = bb_for_stmt (curr_stmt);
    }
  else
    {
      curr_stmt = NULL_TREE;

      /* bsi_start () will initialize the context pointer to the basic block
         if the the block is completely devoid of instructions, except
	 for possibly an empty statement node.  */
      if (curr_bsi->tp == NULL && curr_bsi->context != NULL)
        curr_bb = (basic_block)(curr_bsi->context);
      else
        abort ();
    }

  /* Some blocks are empty. The block iterator points to an empty statement
     node in those cases only.  */
  if (curr_stmt == NULL_TREE)
    {
      inserted_tsi = tsi_start (curr_bb->head_tree_p);
      tsi_link_before (&inserted_tsi, t, TSI_NEW_STMT);
      prepend_stmt_to_bb (tsi_container (inserted_tsi), curr_bb);

      /* In this case, we will *always* return the new stmt since BSI_SAME_STMT
         doesn't really exist.  */
      *curr_bsi = bsi_from_tsi (inserted_tsi);
    }
  else
    {
      inserted_tsi = tsi_from_bsi (*curr_bsi);

      same_tsi = bsi_link_after (&inserted_tsi, t, curr_bb);
      bsi_update_from_tsi (curr_bsi, same_tsi);
      if (mode == BSI_NEW_STMT)
        bsi_next (curr_bsi);
    }

  inserted_stmt = tsi_stmt (inserted_tsi);

  /* Now update the required SSA bits.  */
  modify_stmt (inserted_stmt);

  return;
}


/* This routine inserts a stmt before the stmt iterator passed in.
   The final parameter determines whether the statement iterator
   is updated to point to the new stmt, or left pointing to the original
   statement.  (Which will have a different container.)  */
void
bsi_insert_before (block_stmt_iterator *curr_bsi, tree t,
		   enum bsi_iterator_update mode)
{
  tree_stmt_iterator inserted_tsi, same_tsi;
  basic_block curr_bb;
  tree *curr_container;
  tree curr_stmt, inserted_stmt;

  curr_container = bsi_container (*curr_bsi);

  /* If this block is empty, let bsi_insert_after() handle it.  */
  if (curr_container == NULL || bsi_stmt (*curr_bsi) == NULL_TREE)
    bsi_insert_after (curr_bsi, t, mode);

  curr_stmt = bsi_stmt (*curr_bsi);
  curr_bb = bb_for_stmt (curr_stmt);
  inserted_tsi = tsi_from_bsi (*curr_bsi);

  /* The only case that needs attention is when the insert is before
     the last stmt in a block. In this case, we have to update the
     container of the end pointer.  */
  tsi_link_before (&inserted_tsi, t, TSI_NEW_STMT);
  set_bb_for_stmt (*tsi_container (inserted_tsi), curr_bb);

  same_tsi = inserted_tsi;
  tsi_next (&same_tsi);

  /* The end block pointer can be modified when we insert before the last stmt
     in a block.  This occurs because we insert a new container for the last
     stmt.  */

  if (curr_container == curr_bb->end_tree_p)
    curr_bb->end_tree_p = tsi_container (same_tsi);

  if (mode == BSI_SAME_STMT)
    bsi_update_from_tsi (curr_bsi, same_tsi);
  else
    bsi_update_from_tsi (curr_bsi, inserted_tsi);

  inserted_stmt = tsi_stmt (inserted_tsi);

  /* Now update the required SSA bits.  */
  modify_stmt (inserted_stmt);

  return;
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
  tree_stmt_iterator tsi;
  int num_exit, num_entry;
  tree last, label, gto, old_gto;
  bb_ann_t ann;
  edge e2;

  if (old_bsi)
    old_bsi->tp = (tree *)NULL;
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
      /* If it is an empty block, simply insert after this bsi, and the
	 new stmt will become the only stmt in the block.  */
      if (bsi_end_p (bsi))
	{
	  bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);
	  return bsi;
	}

      /* If this is a fallthrough edge, then we can simply append this stmt
	 to the basic block.  */
      if (e->flags & EDGE_FALLTHRU)
	{
#ifdef ENABLE_CHECKING
	  /* Control statement edges should not be marked FALLTHRU.  */
	  if (is_ctrl_stmt (bsi_stmt (bsi)))
	    abort ();
#endif

	  if (src->head_tree_p == src->end_tree_p 
	      && IS_EMPTY_STMT (*src->head_tree_p))
	    {
	      bsi_replace (bsi, stmt);
	      if (old_bsi)
		*old_bsi = bsi;
	      return bsi;
	    }
	  else
	    {
	      bsi_insert_after (&bsi, stmt, BSI_SAME_STMT);
	      if (old_bsi)
		*old_bsi = bsi;
	      bsi_next (&bsi);
	      return bsi;
	    }
	}

      /* Otherwise, the last stmt is a control altering stmt, so we need to
	 insert before it.  */
      else
	{
#ifdef ENABLE_CHECKING
	  /* A block with a normal non-FALLTHRU edge should end with a
	     control statement.  */
	  if (!is_ctrl_stmt (bsi_stmt (bsi)))
	    abort ();
#endif

	  bsi_insert_before (&bsi, stmt, BSI_NEW_STMT);
	  if (old_bsi)
	    {
	      *old_bsi = bsi;
	      bsi_next (old_bsi);
	    }
	  return bsi;
	}
    }

  /* If dest is a single entry destination, and it isn't the exit block, the
     new stmt can be inserted at the beginning of the destination block.  */

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

  bsi = bsi_last (src);
  if (!bsi_end_p (bsi))
    {
      bool fixup;

      last = bsi_stmt (bsi);
      label = old_gto = NULL;
      tsi = tsi_start (src->end_tree_p);

      switch (TREE_CODE (last))
	{
	case COND_EXPR:
	  e = find_edge (src, new_bb);
	  if (!e)
	    abort ();

	  label = build1 (LABEL_EXPR, void_type_node, NULL_TREE);
	  gto = build_and_jump (&LABEL_EXPR_LABEL (label));
	  if (e->flags & EDGE_TRUE_VALUE)
	    {
	      old_gto = COND_EXPR_THEN (last);
	      COND_EXPR_THEN (last) = gto;
	    }
	  else
	    {
	      old_gto = COND_EXPR_ELSE (last);
	      COND_EXPR_ELSE (last) = gto;
	    }
	  break;

	case SWITCH_EXPR:
	  {
	    tree vec = SWITCH_LABELS (last);
	    size_t i, n = TREE_VEC_LENGTH (vec);
	    tree dest_label = NULL;

	    label = create_artificial_label ();
	    for (i = 0; i < n; ++i)
	      {
		tree elt = TREE_VEC_ELT (vec, i);
		if (label_to_block (CASE_LABEL (elt)) == dest)
		  {
		    dest_label = CASE_LABEL (elt);
		    CASE_LABEL (elt) = label;
		  }
	      }

	    label = build1 (LABEL_EXPR, void_type_node, label);
	    old_gto = build_and_jump (&dest_label);
	  }
	  break;

	case CALL_EXPR:
	case MODIFY_EXPR:
	  /* The block ends in a CALL which has abnormal edges.  In
	     that case, we simply create a new block right after this
	     one, and then fall through to the destination block.  */
	  e = find_edge (new_bb, dest);
	  if (!e)
	    abort ();
	  e->flags |= EDGE_FALLTHRU;
	  break;

	default:
	  /* All cases ought to have been covered by now.  */
	  abort ();
	}

      /* When insertting our first statement, we may well create a new
	 COMPOUND_EXPR container, and so we'll need to update the end
	 of the old src block.  */
      fixup = false;

      if (label)
	{
	  tsi_link_after (&tsi, label, TSI_SAME_STMT);
	  src->end_tree_p = tsi_container (tsi);
	  fixup = true;
	  tsi_next (&tsi);
          append_stmt_to_bb (tsi_container (tsi), new_bb);
	}

      tsi_link_after (&tsi, stmt, fixup ? TSI_NEW_STMT : TSI_SAME_STMT);
      if (!fixup)
	{
	  src->end_tree_p = tsi_container (tsi);
	  fixup = true;
	  tsi_next (&tsi);
	}
      append_stmt_to_bb (tsi_container (tsi), new_bb);

      if (old_gto)
	{
          tsi_link_after (&tsi, old_gto, TSI_NEW_STMT);
          append_stmt_to_bb (tsi_container (tsi), new_bb);
	}

      /* For the same reason of new containers, we have to wait until the
	 end to initialize our return bsi value.  Fortunately we don't 
	 need to search far to get it pointed to the real statement that
	 we added.  */
      bsi = bsi_start (new_bb);
      if (label)
	bsi_next (&bsi);
    }

  /* Now update the required SSA bits.  */
  modify_stmt (stmt);

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
  block_stmt_iterator bsi;
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
	    PENDING_STMT (e) = NULL_TREE;
	    next_stmt = TREE_CHAIN (stmt);
	    /* The first insert will create a new basic block if needed.  */
	    bsi = bsi_insert_on_edge_immediate (e, stmt, NULL, NULL);
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
    PENDING_STMT (e) = stmt;
  else
    {
      for ( ; TREE_CHAIN (t); t = TREE_CHAIN (t))
        continue;
      TREE_CHAIN (t) = stmt;
      TREE_CHAIN (stmt) = NULL_TREE;
    }
}

/* These 2 routines are used to process BSI's in reverse within a block.
   When there is a decent implementation of bsi_prev, we can get rid of
   these forever!  */

/* Push another block_stmt_iterator onto the stack.  */
void
push_bsi (bsi_list_p *list, block_stmt_iterator bsi)
{
  bsi_list_p tmp;
  if (*list == NULL)
    {
      *list = new_bsi_list ();
      (*list)->bsi[0] = bsi;
    }
  else
    {
      if ((*list)->curr_index == (BSI_NUM_ELEMENTS - 1))
        {
	  tmp = new_bsi_list ();
	  tmp->bsi[0] = bsi;
	  tmp->next = *list;
	  *list = tmp;
	}
      else
        {
	  (*list)->bsi[++((*list)->curr_index)] = bsi;
	}
    }
}

/* Pop a block_stmt_iterator off the stack.  */
block_stmt_iterator
pop_bsi (bsi_list_p *list)
{
  block_stmt_iterator bsi;
  bsi_list_p tmp;
  if (!list)
    abort ();

  tmp = *list;
  bsi = tmp->bsi[(tmp->curr_index)--];
  if (tmp->curr_index< 0)
    {
      tmp = *list;
      *list = (*list)->next;
      free (tmp);
    }
  return bsi;
}


/* Replace the statement pointed by TP1 with the statement pointed by TP2.
   Note that this function will not replace COMPOUND_EXPR nodes, only
   individual statements.

   If TP1 is pointing to a COMPOUND_EXPR node, only its LHS operand will be
   replaced. TP2 may not point to a COMPOUND_EXPR.  */

void
replace_stmt (tree *tp1, tree *tp2)
{
  tree t;

  if (TREE_CODE (*tp2) == COMPOUND_EXPR)
    abort ();
      
  t = *tp2;

  /* Relocate annotations for the replacement statement.  */
  SET_EXPR_LOCUS (t, EXPR_LOCUS (*tp1));
  set_bb_for_stmt (t, bb_for_stmt (*tp1));

  /* Don't replace COMPOUND_EXPRs.  Only their operands.  */
  if (TREE_CODE (*tp1) == COMPOUND_EXPR)
    TREE_OPERAND (*tp1, 0) = t;
  else
    *tp1 = t;
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
  new_bb = create_bb (edge_in->src);
  create_block_annotation (new_bb);
  redirect_edge_succ  (edge_in, new_bb);
  new_edge = make_edge (new_bb, dest, 0);

  /* Find all the PHI arguments on the original edge, and change them to
     the new edge.  */
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

  return new_bb;
}


/* Verifies that the flow information is OK.  */

static int
tree_verify_flow_info (void)
{
  int err = 0;
  basic_block bb;
  block_stmt_iterator bsi;
  tree stmt;
  tree_stmt_iterator tsi;

  FOR_EACH_BB (bb)
    {
      bsi = bsi_last (bb);
      if (bsi_end_p (bsi))
	continue;

      stmt = bsi_stmt (bsi);
      switch (TREE_CODE (stmt))
	{
	case COND_EXPR:
	  if (TREE_CODE (COND_EXPR_THEN (stmt)) != GOTO_EXPR
	      || TREE_CODE (COND_EXPR_ELSE (stmt)) != GOTO_EXPR)
	    {
	      fprintf (stderr, "Structured COND_EXPR at end of bb %d\n",
		       bb->index);
	      err = 1;
	    }
	  break;
	default: ;
	}
    }

  /* Check that order of basic blocks is the same as the order of code.  */
  bb = ENTRY_BLOCK_PTR->next_bb;
  if (bb == EXIT_BLOCK_PTR
      || !bb->head_tree_p)
    return err;

  for (tsi = tsi_start (bb->head_tree_p); !tsi_end_p (tsi); tsi_next (&tsi))
    if (*bb->head_tree_p == tsi_stmt (tsi))
      {
	tsi_next (&tsi);
	break;
      }
  for (; !tsi_end_p (tsi); tsi_next (&tsi))
    {
      if (bb->next_bb != EXIT_BLOCK_PTR
	  && *bb->next_bb->head_tree_p == tsi_stmt (tsi))
	{
	  bb = bb->next_bb;
	  continue;
	}

      if (IS_EMPTY_STMT (tsi_stmt (tsi)))
	continue;

      if (!bb_for_stmt (tsi_stmt (tsi)))
	fprintf (stderr, "Statement outside any basic block after bb %d",
		 bb->index);
      else
	{
	  fprintf (stderr, "Block missordering after bb %d\n",
		   bb->index);
	  bb = bb_for_stmt (tsi_stmt (tsi));
	}

      err = 1;
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
  dummy = create_bb (NULL);
  create_block_annotation (dummy);
  dummy->count = bb->count;
  dummy->frequency = bb->frequency;
  dummy->loop_depth = bb->loop_depth;
  dummy->head_tree_p = NULL;
  dummy->end_tree_p = NULL;

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

/* Initialization of functions specific to the tree IR.  */

void
tree_register_cfg_hooks (void)
{
  cfg_hooks = &tree_cfg_hooks;
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

/* Return true if basic block BB does nothing except pass control
   flow to another block and that we can safely insert a label at
   the start of the successor block.  */

static bool
tree_forwarder_block_p (basic_block bb)
{
  block_stmt_iterator bsi;
  edge e;

  /* If we have already determined this block is not forwardable, then
     no further checks are necessary.  */
  if (! bb_ann (bb)->forwardable)
    return false;

  /* BB must have a single outgoing normal edge.  Otherwise it can not be
     a forwarder block.  */
  if (!bb->succ
      || bb->succ->succ_next
      || bb->succ->dest == EXIT_BLOCK_PTR
      || (bb->succ->flags & EDGE_ABNORMAL)
      || bb == ENTRY_BLOCK_PTR)
    {
      bb_ann (bb)->forwardable = 0;
      return false; 
    }

  /* Successors of the entry block are not forwarders.  */
  for (e = ENTRY_BLOCK_PTR->succ; e; e = e->succ_next)
    if (e->dest == bb)
      {
	bb_ann (bb)->forwardable = 0;
	return false;
      }

  /* BB can not have any PHI nodes.  This could potentially be relaxed
     early in compilation if we re-rewrote the variables appearing in
     any PHI nodes in forwarder blocks.  */
  if (phi_nodes (bb))
    {
      bb_ann (bb)->forwardable = 0;
      return false; 
    }

  /* Now walk through the statements.  We can ignore labels, gotos and
     empty statements.  Anything else means this is not a forwarder block.  */
  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      tree stmt = bsi_stmt (bsi);
 
      /* Ignore empty statements.  */
      if (IS_EMPTY_STMT (stmt))
	continue;

      switch (TREE_CODE (stmt))
	{
	case LABEL_EXPR:
	case GOTO_EXPR:
	  break;

	default:
	  bb_ann (bb)->forwardable = 0;
	  return false;
	}
    }

  /* Now check the target for a couple special cases.  */
  bsi = bsi_start (bb->succ->dest);

  /* It's not clear if we can safely insert the label in this case.  */
  if (bsi_end_p (bsi))
    {
      bb_ann (bb)->forwardable = 0;
      return false; 
    }

  return true;
}

/* Threads jumps over empty statements.

   This code should _not_ thread over obviously equivalent conditions
   as that requires nontrivial updates to the SSA graph.  */
   
static bool
thread_jumps (void)
{
  edge e, next, last, old;
  basic_block bb, dest, tmp;
  tree phi, stmt;
  int arg;
  bool retval = false;

  FOR_EACH_BB (bb)
    bb_ann (bb)->forwardable = 1;

  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR, EXIT_BLOCK_PTR, next_bb)
    {
      /* Don't waste time on unreachable blocks.  */
      if (!bb->pred)
	continue;

      /* Nor on forwarders.  */
      if (tree_forwarder_block_p (bb))
	continue;
      
      /* Due to limitations of ir, it is difficult to redirect edge except
	 in some simple cases.  Given that ir is slowly getting more sane,
	 don't invest too much energy into monsters of bsi_insert_on_edge
	 type.  */
      stmt = last_stmt (bb);
      if (stmt
	  && stmt_ends_bb_p (stmt)
	  && TREE_CODE (stmt) != GOTO_EXPR
	  && TREE_CODE (stmt) != COND_EXPR
	  && TREE_CODE (stmt) != SWITCH_EXPR)
	continue;

      /* This block is now part of a forwarding path, mark it as not
	 forwardable so that we can detect loops.   This bit will be
	 reset below.  */
      bb_ann (bb)->forwardable = 0;

      /* Examine each of our block's successors to see if it is
	 forwardable.  */
      for (e = bb->succ; e; e = next)
	{
	  next = e->succ_next;

	  /* If the edge is abnormal or its destination is not
	     forwardable, then there's nothing to do.  */
	  if ((e->flags & EDGE_ABNORMAL)
	      || !tree_forwarder_block_p (e->dest))
	    continue;

	  /* Now walk though as many forwarder block as possible to find
	     the ultimate destination we want to thread our jump to.  */
	  last = e->dest->succ;
	  bb_ann (e->dest)->forwardable = 0;
	  for (dest = e->dest->succ->dest;
	       tree_forwarder_block_p (dest);
	       last = dest->succ,
	       dest = dest->succ->dest)
	    {
	      /* An infinite loop detected.  We redirect the edge anyway, so
		 that the loop is shrinked into single basic block.  */
	      if (!bb_ann (dest)->forwardable)
		break;

	      if (dest->succ->dest == EXIT_BLOCK_PTR)
		break;

	      bb_ann (dest)->forwardable = 0;
	    }

	  /* Reset the forwardable marks to 1.  */
	  for (tmp = e->dest;
	       tmp != dest;
	       tmp = tmp->succ->dest)
	    bb_ann (tmp)->forwardable = 1;

	  if (dest == e->dest)
	    continue;
	      
	  old = find_edge (bb, dest);
	  if (old)
	    {
	      /* If there already is an edge, check whether the values
		 in phi nodes differ.  */
	      if (!phi_alternatives_equal (dest, last, old))
		{
		  /* The previous block is forwarder.  Redirect our jump
		     to that target instead since we know it has no PHI
		     nodes that will need updating.  */
		  dest = last->src;
	  
		  /* That might mean that no forwarding at all is possible.  */
		  if (dest == e->dest)
		    continue;

		  old = find_edge (bb, dest);
		}
	    }

	  /* Perform the redirection.  */
	  retval = true;
	  e = redirect_edge_and_branch (e, dest);
	  if (!old)
	    {
	      /* Update phi nodes.   We know that the new argument should
		 have the same value as the argument associated with LAST.
		 Otherwise we would have changed our target block above.  */
	      for (phi = phi_nodes (dest); phi; phi = TREE_CHAIN (phi))
		{
		  arg = phi_arg_from_edge (phi, last);
		  if (arg < 0)
		    abort ();
		  add_phi_arg (&phi, PHI_ARG_DEF (phi, arg), e);
		}
	    }
	}

      /* Reset the forwardable bit on our block since it's no longer in
	 a forwarding chain path.  */
      bb_ann (bb)->forwardable = 1;
    }
  return retval;
}

/* Return the label in the head of basic block BLOCK.  Create one if it doesn't
   exist.  */

static tree
tree_block_label (basic_block bb)
{
  tree stmt = first_stmt (bb);
  /* We need a label at our final destination.  If it does not already exist,
     create it.  */
  if (!stmt || TREE_CODE (stmt) != LABEL_EXPR)
    {
      block_stmt_iterator iterator = bsi_start (bb);
      tree label = create_artificial_label ();
      stmt = build1 (LABEL_EXPR, void_type_node, label);
      bsi_insert_before (&iterator, stmt, BSI_SAME_STMT);
      return label;
    }
  else
    return LABEL_EXPR_LABEL (stmt);
}

/* Attempt to perform edge redirection by replacing possibly complex jump
   instruction by goto or removing jump completely.  This can apply only
   if all edges now point to the same block.  The parameters and
   return values are equivalent to redirect_edge_and_branch.  */

static edge
tree_try_redirect_by_replacing_jump (edge e, basic_block target)
{
  basic_block src = e->src;
  edge tmp;
  block_stmt_iterator b;
  tree stmt;
  int flags;

  /* Verify that all targets will be TARGET.  */
  for (tmp = src->succ; tmp; tmp = tmp->succ_next)
    if (tmp->dest != target && tmp != e)
      break;

  if (tmp)
    return NULL;

  b = bsi_last (src);
  if (bsi_end_p (b))
    return NULL;
  stmt = bsi_stmt (b);

  if (TREE_CODE (stmt) == COND_EXPR || TREE_CODE (stmt) == SWITCH_EXPR
      || (TREE_CODE (stmt) == GOTO_EXPR && target == src->next_bb))
    {
      if (target == src->next_bb)
	{
	  flags = EDGE_FALLTHRU;
          bsi_remove (&b);
	}
      else
	{
	  flags = 0;
          stmt = build1 (GOTO_EXPR, void_type_node, tree_block_label (target));
          bsi_replace (b, stmt);
	}
      e = ssa_redirect_edge (e, target);
      e->flags = flags;
      return e;
    }
  return NULL;
}

/* Redirect E to DEST.  Return NULL on failure, edge representing redirected
   branch otherwise  */

static edge
tree_redirect_edge_and_branch (edge e, basic_block dest)
{
  edge ret;
  tree label, stmt;
  basic_block bb = e->src, new_bb;
  int flags;

  if (e->flags & (EDGE_ABNORMAL_CALL | EDGE_EH))
    return NULL;

  if (e->src != ENTRY_BLOCK_PTR 
      && (ret = tree_try_redirect_by_replacing_jump (e, dest)))
    return ret;

  if (e->dest == dest)
    return NULL;

  label = tree_block_label (dest);

  /* If our block does not end with a GOTO, then create one.
     Otherwise redirect the existing GOTO_EXPR to LABEL.  */

  stmt = last_stmt (bb);
  new_bb = NULL;
  flags = 0;

  switch (stmt ? TREE_CODE (stmt) : ERROR_MARK)
    {
    case COND_EXPR:
      stmt = (e->flags & EDGE_TRUE_VALUE
	      ? COND_EXPR_THEN (stmt)
	      : COND_EXPR_ELSE (stmt));
      flags = e->flags;
      /* FALLTHRU */

    case GOTO_EXPR:
      GOTO_DESTINATION (stmt) = label;
      break;

    case SWITCH_EXPR:
      {
	tree vec = SWITCH_LABELS (stmt);
	size_t i, n = TREE_VEC_LENGTH (vec);

	for (i = 0; i < n; ++i)
	  {
	    tree elt = TREE_VEC_ELT (vec, i);
	    if (label_to_block (CASE_LABEL (elt)) == e->dest)
	      CASE_LABEL (elt) = label;
	  }
      }
      break;

    default:
      stmt = build1 (GOTO_EXPR, void_type_node, label);
      bsi_insert_on_edge_immediate (e, stmt, NULL, &new_bb);
      /* ??? In RTL equivalent we never create new basic blocks here.
	 Hopefully this will be just a temporary side case before we switch
	 to cfg_layout style mode with no explicit GOTO statements.  */
      if (new_bb)
	e = new_bb->succ;
      e->flags &= ~EDGE_FALLTHRU;
      break;
    }

  /* Update/insert PHI nodes as necessary.  */

  /* Now update the edges in the CFG.  */
  e = ssa_redirect_edge (e, dest);
  e->flags |= flags;
  return e;
}

/* Simple wrapper as we always can redirect fallthru edges.  */
static basic_block
tree_redirect_edge_and_branch_force (edge e, basic_block dest)
{
  basic_block old = e->src;
  e = tree_redirect_edge_and_branch (e, dest);
  if (!e)
    abort ();
  return e->src == old ? NULL : old;
}

/* Dump FUNCTION_DECL FN to file FILE using FLAGS (see TDF_* in tree.h)  */

void
dump_function_to_file (tree fn, FILE *file, int flags)
{
  tree arg, vars, var;
  bool ignore_topmost_bind = false, any_var = false;
  basic_block bb;
  tree chain;

  fprintf (file, "\n;; Function %s",
	    (*lang_hooks.decl_printable_name) (fn, 2));
  fprintf (file, " (%s)\n",
	    IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (fn)));
  fprintf (file, "\n");

  fprintf (file, "%s (", (*lang_hooks.decl_printable_name) (fn, 2));

  arg = DECL_ARGUMENTS (fn);
  while (arg)
    {
      print_generic_expr (file, arg, 0);
      if (TREE_CHAIN (arg))
	fprintf (file, ", ");
      arg = TREE_CHAIN (arg);
    }
  fprintf (file, ")\n");

  if (flags & TDF_RAW)
    {
      dump_node (fn, TDF_SLIM | flags, file);
      return;
    }

  /* When gimple is lowered, the variables are no longer available in the
     bind_exprs, so display them separately.  */
  if (cfun->unexpanded_var_list)
    {
      ignore_topmost_bind = true;

      fprintf (file, "{\n");
      for (vars = cfun->unexpanded_var_list; vars; vars = TREE_CHAIN (vars))
	{
	  var = TREE_VALUE (vars);

	  print_generic_decl (file, var, flags);
	  if (flags & TDF_DETAILS)
	    {
	      /* Mention if the variable will be eliminated.  Only valid in
		 the TDI_optimized dump, but still useful, and we cannot
		 recognize it here; so just mention it if details are
		 requested, and hope it won't be too missleading.  */
	      if (!expand_var_p (var))
		fprintf (file, "    # removed");
	    }
	  fprintf (file, "\n");

	  any_var = true;
	}
    }

  if (basic_block_info)
    {
      /* Make a cfg based dump.  */
      if (!ignore_topmost_bind)
	fprintf (file, "{\n");

      if (any_var && n_basic_blocks)
	fprintf (file, "\n");

      FOR_EACH_BB (bb)
	{
	  dump_generic_bb (file, bb, 2, flags);
	}
	
      fprintf (file, "}\n");
    }
  else
    {
      int indent;

      /* Make a tree based dump.  */
      chain = DECL_SAVED_TREE (fn);

      if (TREE_CODE (chain) == BIND_EXPR)
	{
	  if (ignore_topmost_bind)
	    {
	      chain = BIND_EXPR_BODY (chain);
	      indent = 2;
	    }
	  else
	    indent = 0;
	}
      else
	{
	  if (!ignore_topmost_bind)
	    fprintf (file, "{\n");
	  indent = 2;
	}

      if (any_var)
	fprintf (file, "\n");

      print_generic_stmt_indented (file, chain, flags, indent);
      if (ignore_topmost_bind)
	fprintf (file, "}\n");
    }

  fprintf (file, "\n\n");
}

/* FIXME These need to be filled in with appropriate pointers.  But this
   implies an ABI change in some functions.  */
struct cfg_hooks tree_cfg_hooks = {
  tree_verify_flow_info,
  tree_dump_bb,			/* dump_bb  */
  NULL,				/* create_basic_block  */
  tree_redirect_edge_and_branch,/* redirect_edge_and_branch  */
  tree_redirect_edge_and_branch_force,/* redirect_edge_and_branch_force  */
  NULL,				/* delete_basic_block  */
  NULL,				/* split_block  */
  NULL,				/* can_merge_blocks_p  */
  NULL,				/* merge_blocks  */
  tree_split_edge,		/* cfgh_split_edge  */
  tree_make_forwarder_block,	/* cfgh_make_forward_block  */
  tree_loop_optimizer_init,     /* cfgh_loop_optimizer_init  */
  tree_loop_optimizer_finalize  /* cfgh_loop_optimizer_finalize  */
};
