/* Control flow functions for trees.
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
#include "errors.h"
#include "flags.h"
#include "expr.h"
#include "ggc.h"
#include "diagnostic.h"
#include "tree-flow.h"

/* Local declarations.  */

/* Initial capacity for the basic block array.  */
static const int initial_cfg_capacity = 20;

/* Dump files and flags.  */
static FILE *dump_file;
static int dump_flags;

/* Array with control flow parents.  */
varray_type parent_array;


/* Basic blocks and flowgraphs.  */
static void make_blocks			PARAMS ((tree *, basic_block));
static void make_bind_expr_blocks	PARAMS ((tree *, basic_block));
static void make_cond_expr_blocks	PARAMS ((tree *, basic_block));
static void make_loop_expr_blocks	PARAMS ((tree *, basic_block));
static void make_switch_expr_blocks	PARAMS ((tree *, basic_block));
static basic_block create_bb		PARAMS ((tree *, basic_block));
static void remove_unreachable_blocks	PARAMS ((void));

/* Edges.  */
static void make_edges PARAMS ((void));
static void make_ctrl_stmt_edges	PARAMS ((basic_block));
static void make_exit_edges		PARAMS ((basic_block));
static void make_loop_expr_edges	PARAMS ((basic_block));
static void make_cond_expr_edges	PARAMS ((basic_block));
static void make_goto_expr_edges	PARAMS ((basic_block));
static void make_case_label_edges	PARAMS ((basic_block));

/* Various helpers.  */
static basic_block successor_block	PARAMS ((basic_block));
static basic_block first_exec_block	PARAMS ((tree *));
static tree *first_exec_stmt		PARAMS ((tree *));
static bool block_invalidates_loop	PARAMS ((basic_block, struct loop *));
static basic_block switch_parent	PARAMS ((basic_block));
static void create_block_annotations	PARAMS ((void));

/* Flowgraph optimization and cleanup.  */
static void remove_tree_bb		PARAMS ((basic_block, int));
static void remove_stmt			PARAMS ((tree *));
static bool blocks_unreachable_p	PARAMS ((varray_type));
static void remove_blocks		PARAMS ((varray_type));
static varray_type find_subblocks	PARAMS ((basic_block));
static bool is_parent			PARAMS ((basic_block, basic_block));
static bool is_nonlocal_label_block	PARAMS ((basic_block));
static void cleanup_control_flow	PARAMS ((void));
static void cleanup_cond_expr_graph	PARAMS ((basic_block));
static void cleanup_switch_expr_graph	PARAMS ((basic_block));
static void disconnect_unreachable_case_labels PARAMS ((basic_block));


/* Remove any COMPOUND_EXPR and WFL container from NODE.  */
#define STRIP_CONTAINERS(NODE)					\
  do {								\
    while (TREE_CODE (NODE) == COMPOUND_EXPR			\
	   || TREE_CODE (NODE) == EXPR_WITH_FILE_LOCATION)	\
      if (TREE_CODE (NODE) == COMPOUND_EXPR)			\
	NODE = TREE_OPERAND (NODE, 0);				\
      else if (TREE_CODE (NODE) == EXPR_WITH_FILE_LOCATION)	\
	NODE = EXPR_WFL_NODE (NODE);				\
  } while (0)


/*---------------------------------------------------------------------------
			      Create basic blocks
---------------------------------------------------------------------------*/

/* Entry point to the CFG builder for trees.  FNBODY is the body of the
   function to process.  */

void
build_tree_cfg (fnbody)
     tree fnbody;
{
  tree *first_p;

  /* Initialize the basic block array.  */
  n_basic_blocks = 0;
  last_basic_block = 0;
  VARRAY_BB_INIT (basic_block_info, initial_cfg_capacity, "basic_block_info");
  VARRAY_BB_INIT (parent_array, initial_cfg_capacity, "parent_array");

  ENTRY_BLOCK_PTR->next_bb = EXIT_BLOCK_PTR;
  EXIT_BLOCK_PTR->prev_bb = ENTRY_BLOCK_PTR;

  /* Find the basic blocks for the flowgraph.  First skip any
     non-executable statements at the start of the function.  Otherwise
     we'll end up with an empty basic block 0, which is useless.  */
  STRIP_WFL (fnbody);

  if (fnbody == empty_stmt_node || TREE_CODE (fnbody) != BIND_EXPR)
    return;

  first_p = first_exec_stmt (&BIND_EXPR_BODY (fnbody));
  if (first_p)
    {
      make_blocks (first_p, NULL);

      if (n_basic_blocks > 0)
	{
	  /* Adjust the size of the array.  */
	  VARRAY_GROW (basic_block_info, n_basic_blocks);

	  /* Create block annotations.  */
	  create_block_annotations ();

	  /* Create the edges of the flowgraph.  */
	  make_edges ();

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
}


/* Build a flowgraph for the statements starting at the statement pointed
   by FIRST_P.
   
   PARENT_BLOCK is the header block for the control structure
      immediately enclosing the new sub-graph.  */

static void
make_blocks (first_p, parent_block)
     tree *first_p;
     basic_block parent_block;
{
  basic_block bb;
  bool start_new_block;
  gimple_stmt_iterator i;

  if (first_p == NULL
      || *first_p == empty_stmt_node
      || *first_p == error_mark_node)
    return;

  bb = NULL;
  start_new_block = true;
  for (i = gsi_start (first_p); !gsi_after_end (i); gsi_step (&i))
    {
      tree stmt;
      enum tree_code code;
      tree *container = gsi_container (i);

      stmt = gsi_stmt (i);
      STRIP_WFL (stmt);
      STRIP_NOPS (stmt);

      /* Ignore non-executable statements.  */
      if (stmt == empty_stmt_node)
	continue;

      code = TREE_CODE (stmt);

      /* Note that we check whether we need to start a new block before
	 processing control statements.  This is because after creating the
	 sub-graph for a control statement, we will always need to start a
	 new block, and by then it will be too late to figure it out.  */
      if (stmt_starts_bb_p (stmt))
	start_new_block = true;

      if (code == BIND_EXPR)
	make_bind_expr_blocks (container, parent_block);
      else if (code == COND_EXPR)
	make_cond_expr_blocks (container, parent_block);
      else if (code == LOOP_EXPR)
	make_loop_expr_blocks (container, parent_block);
      else if (code == SWITCH_EXPR)
	make_switch_expr_blocks (container, parent_block);
      else if (code == TRY_FINALLY_EXPR || code == TRY_CATCH_EXPR)
	/* FIXME: Some of these nodes could be lowered by gimplify.c  */
	; /* abort (); */
      else
	{
	  /* For regular statements, add them the current block and move on
	     to the next statement.  */
	  if (start_new_block)
	    {
	      /* Create a new basic block.  */
	      bb = create_bb (container, parent_block);
	      start_new_block = false;
	    }
	  else
	    {
	      /* Add the statement to the existing basic block.  */
	      set_bb_for_stmt (*container, bb);
	      bb->end_tree_p = container;
	    }

	  /* If STMT alters the flow of control, we need to start a new
	     block on the next iteration.  */
	  if (is_ctrl_altering_stmt (stmt))
	    start_new_block = true;
	}
    }
}


/* Create the blocks for the BIND_EXPR node *BIND_P.  PARENT_BLOCK is as in
   make_blocks.  */

static void
make_bind_expr_blocks (bind_p, parent_block)
     tree *bind_p;
     basic_block parent_block;
{
  basic_block entry;
  tree bind = *bind_p;

  entry = create_bb (bind_p, parent_block);
  entry->flags |= BB_COMPOUND_ENTRY;

  STRIP_CONTAINERS (bind);
  make_blocks (&BIND_EXPR_BODY (bind), entry);
}


/* Create the blocks for the LOOP_EXPR node *LOOP_P.  PARENT_BLOCK is as in
   make_blocks.  */

static void
make_loop_expr_blocks (loop_p, parent_block)
     tree *loop_p;
     basic_block parent_block;
{
  basic_block entry, latch;
  tree loop = *loop_p;
  
  entry = create_bb (loop_p, parent_block);
  entry->flags |= BB_COMPOUND_ENTRY | BB_CONTROL_EXPR | BB_LOOP_CONTROL_EXPR;

  /* Create an empty block to serve as latch to prevent flow_loops_find
     from finding multiple natural loops in the presence of multiple back
     edges.
     
     ??? Note that latch_block assumes that the latch block is the
	 successor of the entry block in the linked list of blocks.  */
  latch = create_bb (&empty_stmt_node, entry);
  latch->flags |= BB_CONTROL_EXPR | BB_LOOP_CONTROL_EXPR;
  
  STRIP_CONTAINERS (loop);
  make_blocks (&LOOP_EXPR_BODY (loop), entry);
}


/* Create the blocks for a COND_EXPR.  PARENT_BLOCK is as in make_blocks.  */

static void
make_cond_expr_blocks (cond_p, parent_block)
     tree *cond_p;
     basic_block parent_block;
{
  tree cond = *cond_p;
  basic_block entry = create_bb (cond_p, parent_block);
  entry->flags |= BB_COMPOUND_ENTRY | BB_CONTROL_EXPR;

  STRIP_CONTAINERS (cond);
  make_blocks (&COND_EXPR_THEN (cond), entry);
  make_blocks (&COND_EXPR_ELSE (cond), entry);
}


/* Create the blocks for a SWITCH_EXPR.  PARENT_BLOCK is as as in
   make_blocks.  */

static void
make_switch_expr_blocks (switch_e_p, parent_block)
     tree *switch_e_p;
     basic_block parent_block;
{
  tree switch_e = *switch_e_p;
  basic_block entry = create_bb (switch_e_p, parent_block);
  entry->flags |= BB_COMPOUND_ENTRY | BB_CONTROL_EXPR;

  STRIP_CONTAINERS (switch_e);
  make_blocks (&SWITCH_BODY (switch_e), entry);
}


/* Create and return a new basic block.

   HEAD_P is a pointer to the first statement in the block.

   PARENT_BLOCK is the entry block for the control structure containing
      the new block.  */

static basic_block
create_bb (head_p, parent_block)
     tree *head_p;
     basic_block parent_block;
{
  basic_block bb;

  /* Create and initialize a new basic block.  */
  bb = alloc_block ();
  memset (bb, 0, sizeof (*bb));

  bb->head_tree_p = bb->end_tree_p = head_p;
  bb->index = last_basic_block;
  bb->flags = BB_NEW;

  /* Add the new block to the linked list of blocks.  */
  if (n_basic_blocks > 0)
    link_block (bb, BASIC_BLOCK (n_basic_blocks - 1));
  else
    link_block (bb, ENTRY_BLOCK_PTR);

  /* Grow the basic block array if needed.  */
  if ((size_t) n_basic_blocks == VARRAY_SIZE (basic_block_info))
    {
      VARRAY_GROW (basic_block_info, n_basic_blocks + (n_basic_blocks + 3) / 4);
      VARRAY_GROW (parent_array, n_basic_blocks + (n_basic_blocks + 3) / 4);
    }

  /* Add the newly created block to the array.  */
  BASIC_BLOCK (n_basic_blocks) = bb;
  VARRAY_BB (parent_array, n_basic_blocks) = parent_block;
  n_basic_blocks++;
  last_basic_block++;

  /* Associate the newly created block to the head tree.  */
  set_bb_for_stmt (*head_p, bb);

  return bb;
}



/* Create annotations for all the blocks in the flowgraph.  */

static void
create_block_annotations ()
{
  basic_block bb;
  int i;

  alloc_aux_for_blocks (sizeof (struct bb_ann_d));

  /* Set parent block information for each block.  */
  i = 0;
  FOR_EACH_BB (bb)
    {
      bb_ann ann = (bb_ann)bb->aux;
      ann->refs = create_ref_list ();
      ann->parent_block = VARRAY_BB (parent_array, i);
      i++;
    }
}


/*---------------------------------------------------------------------------
				 Edge creation
---------------------------------------------------------------------------*/

/* Join all the blocks in the flowgraph.  */

static void
make_edges ()
{
  basic_block bb;

  /* Create an edge from entry to the first block with executable
     statements in it.  */
  make_edge (ENTRY_BLOCK_PTR, BASIC_BLOCK (0), EDGE_FALLTHRU);

  /* Traverse basic block array placing edges.  */
  FOR_EACH_BB (bb)
    {
      tree first = first_stmt (bb);
      tree last = last_stmt (bb);

      /* Edges for control statements.  */
      if (is_ctrl_stmt (first))
	make_ctrl_stmt_edges (bb);

      /* Edges for control flow altering statements (GOTO_EXPR and
	 RETURN_EXPR) need an edge to the corresponding target block.  */
      if (is_ctrl_altering_stmt (last))
	make_exit_edges (bb);

      /* Incoming edges for label blocks in switch statements.  It's easier
         to deal with these bottom-up than top-down.  */
      if (TREE_CODE (first) == CASE_LABEL_EXPR)
	make_case_label_edges (bb);

      /* BIND_EXPR blocks get an edge to the first basic block in the
	 BIND_EXPR_BODY.  */
      if (TREE_CODE (first) == BIND_EXPR)
	{
	  basic_block body_bb = first_exec_block (&BIND_EXPR_BODY (first));
	  if (body_bb)
	    make_edge (bb, body_bb, EDGE_FALLTHRU);
	}

      /* Finally, if no edges were created above, this is a regular
         basic block that only needs a fallthru edge.  */
      if (bb->succ == NULL)
	make_edge (bb, successor_block (bb), EDGE_FALLTHRU);
    }

  /* Clean up the graph and warn for unreachable code.  */
  cleanup_tree_cfg ();
}


/* Create edges for control statement at basic block BB.  */

static void
make_ctrl_stmt_edges (bb)
     basic_block bb;
{
  switch (TREE_CODE (first_stmt (bb)))
    {
    case LOOP_EXPR:
      make_loop_expr_edges (bb);
      break;

    case COND_EXPR:
      make_cond_expr_edges (bb);
      break;

    case SWITCH_EXPR:
      /* FIXME  We should not need to make an edge to the body of the switch.
		Each label block inside the switch statement will create
		its own edge from the switch block.  However, if we remove
		the code that might exist between the switch() and the
		first case statement, the RTL expander gets all confused
		and enters an infinite loop.  */
      {
	tree body = SWITCH_BODY (first_stmt (bb));
	STRIP_WFL (body);
	STRIP_NOPS (body);
	if (TREE_CODE (body) == BIND_EXPR)
	  {
	    make_edge (bb, bb_for_stmt (body), 0);
	    make_edge (bb, successor_block (bb), EDGE_FALLTHRU);
	  }
      }
      break;

    default:
      abort ();
    }
}


/* Create exit edges for statements that alter the flow of control
   (BREAK, CONTINUE, GOTO, RETURN and calls to non-returning functions).  */

static void
make_exit_edges (bb)
     basic_block bb;
{
  switch (TREE_CODE (last_stmt (bb)))
    {
    case GOTO_EXPR:
      make_goto_expr_edges (bb);
      break;

      /* A CALL_EXPR node here means that the last statement of the block
	 is a call to a non-returning function.  */
    case CALL_EXPR:
    case RETURN_EXPR:
      make_edge (bb, EXIT_BLOCK_PTR, 0);
      break;

    default:
      abort ();
    }
}


/* Create the edges for a LOOP_EXPR structure starting with bb.  */

static void
make_loop_expr_edges (bb)
     basic_block bb;
{
  tree entry = first_stmt (bb);
  basic_block end_bb, body_bb;

#if defined ENABLE_CHECKING
  if (TREE_CODE (entry) != LOOP_EXPR)
    abort ();
#endif

  /* Create the following edges.  The other edges will be naturally created
     by the main loop in create_edges().

         +---> LOOP_EXPR -----+
	 |         |          |
         |         v          |
         |   LOOP_EXPR_BODY   |
	 |                    |
	 |                    |
	 +---- END_WHILE      |
	                      |
		              |
	     Next block <-----+
          
     If the body doesn't exist, we use the header instead.  */

  /* Basic blocks for each component.  */
  end_bb = latch_block (bb);
  body_bb = first_exec_block (&LOOP_EXPR_BODY (entry));

  /* Empty loop body?  Use the latch block.  */
  if (body_bb == NULL)
    body_bb = end_bb;

  make_edge (bb, body_bb, 0);
  make_edge (end_bb, bb, 0);
}


/* Create the edges for a COND_EXPR structure starting with BB.  */

static void
make_cond_expr_edges (bb)
     basic_block bb;
{
  tree entry = first_stmt (bb);
  basic_block successor_bb, then_bb, else_bb;

#if defined ENABLE_CHECKING
  if (TREE_CODE (entry) != COND_EXPR)
    abort ();
#endif

  /* Entry basic blocks for each component.  */
  then_bb = first_exec_block (&COND_EXPR_THEN (entry));
  else_bb = first_exec_block (&COND_EXPR_ELSE (entry));
  successor_bb = successor_block (bb);

  /* Create the following edges.

	     COND_EXPR
		/ \
	       /   \
	    THEN   ELSE

     Either clause may be empty.  */
  if (then_bb)
    make_edge (bb, then_bb, EDGE_TRUE_VALUE);

  if (else_bb)
    make_edge (bb, else_bb, EDGE_FALSE_VALUE);

  /* If conditional is missing one of the clauses, make an edge between the
     entry block and the first block outside the conditional.  */
  if (!then_bb || !else_bb)
    make_edge (bb, successor_bb, 0);
}


/* Create edges for a goto statement.  */

static void
make_goto_expr_edges (bb)
     basic_block bb;
{
  tree goto_t, dest;
  basic_block target_bb;

  goto_t = last_stmt (bb);

#if defined ENABLE_CHECKING
  if (goto_t == NULL || TREE_CODE (goto_t) != GOTO_EXPR)
    abort ();
#endif

  dest = GOTO_DESTINATION (goto_t);

  /* Look for the block starting with the destination label.  In the
     case of a computed goto, make an edge to any label block we find
     in the CFG.  */
  FOR_EACH_BB (target_bb)
    {
      tree target = first_stmt (target_bb);

      /* Common case, destination is a single label.  Make the edge
         and leave.  */
      if (TREE_CODE (dest) == LABEL_DECL
	  && TREE_CODE (target) == LABEL_EXPR
	  && LABEL_EXPR_LABEL (target) == dest)
	{
	  make_edge (bb, target_bb, 0);
	  break;
	}

      /* Computed GOTOs.  Make an edge to every label block.  */
      else if (TREE_CODE (dest) != LABEL_DECL
	       && TREE_CODE (target) == LABEL_EXPR)
	make_edge (bb, target_bb, 0);
    }
}


/* Create the edge between CASE_LABEL_EXPR block BB and the block for the
   associated SWITCH_EXPR node.  */

static void
make_case_label_edges (bb)
     basic_block bb;
{
  basic_block switch_bb = switch_parent (bb);

#if defined ENABLE_CHECKING
  if (switch_bb == NULL)
    abort ();
#endif

  make_edge (switch_bb, bb, 0);
}



/*---------------------------------------------------------------------------
			       Flowgraph analysis
---------------------------------------------------------------------------*/

/* Remove unreachable blocks and other miscellaneous clean up work.  */

void
cleanup_tree_cfg ()
{
  cleanup_control_flow ();
  remove_unreachable_blocks ();
  compact_blocks ();
}


/* Delete all unreachable basic blocks.   */

static void
remove_unreachable_blocks ()
{
  int i, n;

  find_unreachable_blocks ();

  /* n_basic_blocks will change constantly as we delete blocks, so get a
     copy first.  */
  n = n_basic_blocks;
  for (i = 0; i < n; i++)
    {
      basic_block bb = BASIC_BLOCK (i);

      /* The block may have been removed in a previous iteration if it was
	 inside an unreachable control structure.  */
      if (bb == NULL || bb->index == INVALID_BLOCK)
	continue;

      if (!(bb->flags & BB_REACHABLE))
	{
	  if (bb->flags & BB_COMPOUND_ENTRY)
	    {
	      /* Before removing an entry block for a compound structure,
		 make sure that all its subblocks are unreachable as well.  */
	      varray_type subblocks = find_subblocks (bb);
	      if (blocks_unreachable_p (subblocks))
		{
		  remove_blocks (subblocks);
		  remove_tree_bb (bb, !is_nonlocal_label_block (bb));
		}
	      else
		remove_tree_bb (bb, 0);
	    }
	  else
	    remove_tree_bb (bb, !is_nonlocal_label_block (bb));
	}
    }
}


/* Remove block BB and its statements from the flowgraph.  Note that if
   REMOVE_STMTS is nonzero and BB is the entry block for a compound
   statement (control structures or blocks of code), removing BB will
   effectively remove the whole structure from the program.  The caller is
   responsible for making sure that all the blocks in the compound
   structure are also removed.  */

static void
remove_tree_bb (bb, remove_stmts)
     basic_block bb;
     int remove_stmts;
{
  gimple_stmt_iterator i;

  dump_file = dump_begin (TDI_cfg, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "Removing unreachable basic block %d\n", bb->index);
      dump_tree_bb (dump_file, "", bb, 0);
      fprintf (dump_file, "\n");
      dump_end (TDI_cfg, dump_file);
    }

  /* Remove all the instructions in the block.  */
  for (i = gsi_start_bb (bb); !gsi_after_end (i); gsi_step_bb (&i))
    {
      tree stmt = gsi_stmt (i);

      if (dump_file && is_exec_stmt (gsi_stmt (i)))
	{
	  fprintf (dump_file, "WARNING: Removing executable statement: ");
	  print_generic_stmt (dump_file, gsi_stmt (i), dump_flags|TDF_SLIM);
	}

      set_bb_for_stmt (stmt, NULL);
      if (remove_stmts)
	gsi_remove (i);
    }

  /* Remove the edges into and out of this block.  */
  while (bb->pred != NULL)
    remove_edge (bb->pred);

  while (bb->succ != NULL)
    remove_edge (bb->succ);

  bb->pred = NULL;
  bb->succ = NULL;

  /* Remove the basic block from the array.  */
  expunge_block (bb);
}


/* Remove all the blocks in BB_ARRAY.  */

static void
remove_blocks (bb_array)
     varray_type bb_array;
{
  size_t i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (bb_array); i++)
    {
      basic_block bb = VARRAY_BB (bb_array, i);
      remove_tree_bb (bb, !is_nonlocal_label_block (bb));
    }
}


/* Return true if all the blocks in BB_ARRAY are unreachable.  */

static bool
blocks_unreachable_p (bb_array)
     varray_type bb_array;
{
  size_t i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (bb_array); i++)
    {
      basic_block bb = VARRAY_BB (bb_array, i);
      if (bb->flags & BB_REACHABLE || is_nonlocal_label_block (bb))
	return false;
    }

  return true;
}


/* Return true if block BB starts with a nonlocal label.  */

static bool
is_nonlocal_label_block (bb)
     basic_block bb;
{
  tree t = first_stmt (bb);

  /* FIXME  We don't compute nonlocal labels until RTL expansion.  This
	    returns false positives.  */
  return (TREE_CODE (t) == LABEL_EXPR && DECL_NAME (LABEL_EXPR_LABEL (t)));
}


/* Find all the blocks in the graph that are included in the compound
   structure starting at block BB.  */

static varray_type
find_subblocks (bb)
     basic_block bb;
{
  varray_type subblocks;
  basic_block child_bb;

  VARRAY_BB_INIT (subblocks, 5, "subblocks");

  if (bb->flags & BB_COMPOUND_ENTRY)
    {
      /* Note: This assumes that all the blocks inside a compound a control
	 structure are consecutive in the linked list of blocks.  */
      child_bb = bb->next_bb;
      while (child_bb != EXIT_BLOCK_PTR && is_parent (bb, child_bb))
	{
	  VARRAY_PUSH_BB (subblocks, child_bb);
	  child_bb = child_bb->next_bb;
	}
    }

  return subblocks;
}


/* Return true if BB is a control parent for CHILD_BB.  Notice that this
   property is not the same as dominance.  This is a test for containment.
   Given two blocks A and B, A DOM B does not imply A is-parent-of B.  For
   instance,

	    1	{
	    2	  s1;
	    3	}
	    4	{
	    5	  s2;
	    6	}

   The block at line 1 dominates the block at line 4, but line 4 is not
   contained in 1's compound structure.  */

static bool
is_parent (bb, child_bb)
     basic_block bb;
     basic_block child_bb;
{
  basic_block parent;

  if (bb == child_bb)
    return true;

  for (parent = parent_block (child_bb);
       parent && parent->index != INVALID_BLOCK;
       parent = parent_block (parent))
    if (parent == bb)
      return true;

  return false;
}


/* Remove statement pointed by iterator I.  Note that this function will
   wipe out control statements that may span multiple basic blocks.  Make
   sure that you really want to remove the whole control structure before
   calling this function.  */

void
gsi_remove (i)
     gimple_stmt_iterator i;
{
  tree t = *(i.tp);

  STRIP_WFL (t);
  STRIP_NOPS (t);

  if (!is_exec_stmt (t))
    return;

  if (TREE_CODE (t) == COMPOUND_EXPR)
    {
      remove_stmt (&TREE_OPERAND (t, 0));

      /* If both operands are empty, delete the whole COMPOUND_EXPR.  */
      if (TREE_OPERAND (t, 1) == empty_stmt_node)
	remove_stmt (i.tp);
    }
  else
    remove_stmt (i.tp);
}


/* Remove statement *STMT_P.  Update all references associated with it.
   Note that this function will wipe out control statements that may span
   multiple basic blocks.  Make sure that you really want to remove the
   whole control structure before calling this function.  */

static void
remove_stmt (stmt_p)
     tree *stmt_p;
{
  ref_list_iterator i;
  varray_type todelete;
  tree stmt = *stmt_p;

  STRIP_WFL (stmt);
  STRIP_NOPS (stmt);

  /* Collect variables referenced by the statement.  */
  VARRAY_TREE_INIT (todelete, 5, "todelete");
  for (i = rli_start (tree_refs (stmt)); !rli_after_end (i); rli_step (&i))
    {
      tree var = ref_var (rli_ref (i));
      if (!TREE_VISITED (var))
	{
	  VARRAY_PUSH_TREE (todelete, var);
	  TREE_VISITED (var) = 1;
	}
    }

  /* For every variable, remove all the references associated with STMT.  */
  while (VARRAY_ACTIVE_SIZE (todelete) > 0)
    {
      ref_list_iterator j;
      tree var = VARRAY_TOP_TREE (todelete);
      VARRAY_POP (todelete);

      for (j = rli_start (tree_refs (var)); !rli_after_end (j); rli_step (&j))
	{
	  tree_ref ref = rli_ref (j);
	  if (ref_stmt (ref) == stmt)
	    rli_delete (j);
	}

      /* If there are no more references to VAR, mark it unused.  */
      if (ref_list_is_empty (tree_refs (var)))
	TREE_USED (var) = 0;

      TREE_VISITED (var) = 0;
    }

  /* Finally, if the statement is a LABEL_EXPR, remove the LABEL_DECL from
     the symbol table.  */
  if (TREE_CODE (stmt) == LABEL_EXPR)
    remove_decl (LABEL_EXPR_LABEL (stmt));

  /* Replace STMT with empty_stmt_node.  */
  *stmt_p = empty_stmt_node;
}


/* Scan all the loops in the flowgraph verifying their validity.   A valid
   loop L contains no calls to user functions, no returns, no jumps out of
   the loop and non-local gotos.  */

void
validate_loops (loops)
     struct loops *loops;
{
  int i;

  for (i = 0; i < loops->num; i++)
    {
      struct loop *loop = &(loops->array[i]);
      sbitmap nodes = loop->nodes;
      int n;

      EXECUTE_IF_SET_IN_SBITMAP (nodes, 0, n,
	  {
	    if (block_invalidates_loop (BASIC_BLOCK (n), loop))
	      {
		loop->invalid = 1;
		break;
	      }
	  });
    }
}


/* Return true if the basic block BB makes the LOOP invalid.  This occurs if
   the block contains a call to a user function, a return, a jump out of the
   loop or a non-local goto.  */

static bool
block_invalidates_loop (bb, loop)
     basic_block bb;
     struct loop *loop;
{
  ref_list_iterator i;

  /* Valid loops cannot contain a return statement.  */
  if (TREE_CODE (last_stmt (bb)) == RETURN_EXPR)
    return true;

  /* If the destination node of a goto statement is not in the loop, mark it
     invalid.  */
  if (TREE_CODE (last_stmt (bb)) == GOTO_EXPR
      && ! TEST_BIT (loop->nodes, bb->succ->dest->index))
    return true;

  /* If the node contains a non-pure function call, mark it invalid.  A
     non-pure function call is marked by the presence of a clobbering
     definition of GLOBAL_VAR.  */
  for (i = rli_start (bb_refs (bb)); !rli_after_end (i); rli_step (&i))
    if (ref_var (rli_ref (i)) == global_var && is_clobbering_def (rli_ref (i)))
      return true;

  return false;
}


/* Try to remove superfluous control structures.  */

static void
cleanup_control_flow ()
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      enum tree_code code;

      if (bb->flags & BB_CONTROL_EXPR
	  && bb->flags & BB_COMPOUND_ENTRY)
	{
	  code = TREE_CODE (first_stmt (bb));

	  if (code == COND_EXPR)
	    cleanup_cond_expr_graph (bb);
	  else if (code == SWITCH_EXPR)
	    cleanup_switch_expr_graph (bb);
	}
    }
}

      
/* If the predicate of the COND_EXPR node in block BB is constant,
   disconnect the subgraph that contains the clause that is never
   executed.  FIXME  Must also update DFA/SSA information.  */

static void
cleanup_cond_expr_graph (bb)
     basic_block bb;
{
  tree cond_expr = first_stmt (bb);
  tree val;
  edge taken_edge;

#if defined ENABLE_CHECKING
  if (TREE_CODE (cond_expr) != COND_EXPR)
    abort ();
#endif

  val = COND_EXPR_COND (cond_expr);
  taken_edge = find_taken_edge (bb, val);
  if (taken_edge)
    {
      edge e, next;

      /* Remove all the edges except the one that is always executed.  */
      for (e = bb->succ; e; e = next)
	{
	  next = e->succ_next;
	  if (e != taken_edge)
	    remove_edge (e);
	}
    }
}


/* If the switch condition of the SWITCH_EXPR node in block BB is constant,
   disconnect all the subgraphs for all the case labels that will never be
   taken.  FIXME  Must also update DFA/SSA information.  */

static void
cleanup_switch_expr_graph (bb)
     basic_block bb;
{
  tree switch_expr = first_stmt (bb);
  edge e;

#if defined ENABLE_CHECKING
  if (TREE_CODE (switch_expr) != SWITCH_EXPR)
    abort ();
#endif

  disconnect_unreachable_case_labels (bb);

  /* If the switch() has a default label, remove the fallthru edge that was
     created when we processed the entry block for the switch() statement.  */
  for (e = bb->succ; e; e = e->succ_next)
    {
      basic_block bb = e->dest;
      tree t = first_stmt (bb);

      if (TREE_CODE (t) == CASE_LABEL_EXPR && CASE_LOW (t) == NULL_TREE)
	{
	  basic_block entry_bb = parent_block (bb);
	  basic_block chain_bb = successor_block (entry_bb);
	  edge e = find_edge (entry_bb, chain_bb);
	  if (e)
	    remove_edge (e);
	  break;
	}
    }
}


/* If the switch() statement starting at basic block BB has a constant
   condition, disconnect all the unreachable case labels.  */

static void
disconnect_unreachable_case_labels (bb)
     basic_block bb;
{
  edge taken_edge;
  tree switch_val;

  switch_val = SWITCH_COND (first_stmt (bb));
  taken_edge = find_taken_edge (bb, switch_val);
  if (taken_edge)
    {
      edge e, next;

      /* Remove all the edges that go to case labels that will never
	 be taken.  */
      for (e = bb->succ; e; e = next)
	{
	  next = e->succ_next;
	  if (e != taken_edge)
	    remove_edge (e);
	}
    }
}


/* Given a control block BB and a constant value VAL, return the edge
   that will be taken out of BLOCK.  If VAL does not match a unique edge,
   NULL is returned.  */

edge
find_taken_edge (bb, val)
     basic_block bb;
     tree val;
{
  tree stmt;
  edge e, taken_edge;

  stmt = first_stmt (bb);

#if defined ENABLE_CHECKING
  if (!is_ctrl_stmt (stmt))
    abort ();
#endif

  /* If VAL is not a constant, we can't determine which edge might
     be taken.  */
  if (val == NULL || !really_constant_p (val))
    return NULL;

  taken_edge = NULL;
  if (TREE_CODE (stmt) == COND_EXPR)
    {
      bool always_false;
      bool always_true;

      /* Determine which branch of the if() will be taken.  */
      always_false = (simple_cst_equal (val, integer_zero_node) == 1);
      always_true = (simple_cst_equal (val, integer_one_node) == 1);

      /* If VAL is a constant but it can't be reduced to a 0 or a 1, then
	 we don't really know which edge will be taken at runtime.  This
	 may happen when comparing addresses (e.g., if (&var1 == 4))  */
      if (!always_false && !always_true)
	return NULL;

      for (e = bb->succ; e; e = e->succ_next)
	{
	  if (((e->flags & EDGE_TRUE_VALUE) && always_true)
	      || ((e->flags & EDGE_FALSE_VALUE) && always_false))
	    {
	      taken_edge = e;
	      break;
	    }
	}

      if (taken_edge == NULL)
	{
	  /* If E is not going to the THEN nor the ELSE clause, then it's
	     the fallthru edge to the successor block of the if() block.  */
	  taken_edge = find_edge (bb, successor_block (bb));
	}
    }
  else if (TREE_CODE (stmt) == SWITCH_EXPR)
    {
      edge default_edge = NULL;

      /* See if the switch() value matches one of the case labels.  */
      for (e = bb->succ; e; e = e->succ_next)
	{
	  tree label_val;
	  edge dest_edge = e;
	  tree dest_t = first_stmt (dest_edge->dest);

	  /* Remember the edge out of the switch() just in case there is no
	     matching label in the body.  */
	  if (TREE_CODE (dest_t) != CASE_LABEL_EXPR)
	    continue;

	  label_val = CASE_LOW (dest_t);
	  if (label_val == NULL_TREE)
	    {
	      /* Remember that we found a default label, just in case no other
	         label matches the switch() value.  */
	      default_edge = dest_edge;
	    }
	  else if (simple_cst_equal (label_val, val) == 1)
	    {
	      /* We found the unique label that will be taken by the switch.
	         No need to keep looking.  All the other labels are never
	         reached directly from the switch().  */
	      taken_edge = dest_edge;
	      break;
	    }
	}

      /* If no case exists for the value used in the switch(), we use the
         default label.  If the switch() has no label, we use the edge
	 going out of the switch() body.  */
      if (taken_edge == NULL)
	taken_edge = default_edge 
		     ? default_edge
		     : find_edge (bb, successor_block (bb));
    }
  else
    {
      /* LOOP_EXPR nodes are always followed by their successor block.  */
      taken_edge = bb->succ;
    }


  return taken_edge;
}


/*---------------------------------------------------------------------------
			 Code insertion and replacement
---------------------------------------------------------------------------*/

/* Insert basic block NEW_BB before BB.  */

void
insert_bb_before (new_bb, bb)
     basic_block new_bb;
     basic_block bb;
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

/* Dump a basic block to a file.  */

void
dump_tree_bb (outf, prefix, bb, indent)
     FILE *outf;
     const char *prefix;
     basic_block bb;
     int indent;
{
  edge e;
  tree head = *(bb->head_tree_p);
  tree end = *(bb->end_tree_p);
  char *s_indent;
  int lineno;

  s_indent = (char *) alloca ((size_t) indent + 1);
  memset ((void *) s_indent, ' ', (size_t) indent);
  s_indent[indent] = '\0';

  fprintf (outf, "%s%sBasic block %d", s_indent, prefix, bb->index);

  if (is_latch_block (bb))
    fprintf (outf, " (latch for #%d)\n", bb->index - 1);
  else
    fprintf (outf, "\n");

  fprintf (outf, "%s%sPredecessors: ", s_indent, prefix);
  for (e = bb->pred; e; e = e->pred_next)
    dump_edge_info (outf, e, 0);
  putc ('\n', outf);

  fprintf (outf, "%s%sSuccessors: ", s_indent, prefix);
  for (e = bb->succ; e; e = e->succ_next)
    dump_edge_info (outf, e, 1);
  putc ('\n', outf);

  fprintf (outf, "%s%sHead: ", s_indent, prefix);
  if (head)
    {
      lineno = get_lineno (head);
      print_generic_stmt (outf, head, TDF_SLIM);
      fprintf (outf, " (line: %d)\n", lineno);
    }
  else
    fputs ("nil\n", outf);

  fprintf (outf, "%s%sEnd: ", s_indent, prefix);
  if (end)
    {
      lineno = get_lineno (end);
      print_generic_stmt (outf, end, TDF_SLIM);
      fprintf (outf, " (line: %d)\n", lineno);
    }
  else
    fputs ("nil\n", outf);

  fprintf (outf, "%s%sParent block: ", s_indent, prefix);
  if (parent_block (bb))
    fprintf (outf, "%d\n", parent_block (bb)->index);
  else
    fputs ("nil\n", outf);

  fprintf (outf, "%s%sLoop depth: %d\n", s_indent, prefix, bb->loop_depth);

  fprintf (outf, "%s%sNext block: ", s_indent, prefix);
  if (bb->next_bb)
    fprintf (outf, "%d\n", bb->next_bb->index);
  else
    fprintf (outf, "nil\n");

  fprintf (outf, "%s%sPrevious block: ", s_indent, prefix);
  if (bb->prev_bb)
    fprintf (outf, "%d\n", bb->prev_bb->index);
  else
    fprintf (outf, "nil\n");
}


/* Dump a basic block on stderr.  */

void
debug_tree_bb (bb)
     basic_block bb;
{
  dump_tree_bb (stderr, "", bb, 0);
}


/* Dump the CFG on stderr.  FLAGS are the same used by the tree dumping
   functions  (see TDF_* in tree.h).  */

void
debug_tree_cfg (flags)
     int flags;
{
  dump_tree_cfg (stderr, flags);
}


/* Dump the program showing basic block boundaries on the given FILE.
   FLAGS are the same used by the tree dumping functions (see TDF_* in
   tree.h).  */

void
dump_tree_cfg (file, flags)
     FILE *file;
     int flags;
{
  basic_block bb;

  if (flags & TDF_DETAILS)
    {
      fputc ('\n', file);
      fprintf (file, "Function %s\n\n", get_name (current_function_decl));
      fprintf (file, "\n%d basic blocks, %d edges, last basic block %d.\n",
	       n_basic_blocks, n_edges, last_basic_block);

      FOR_EACH_BB (bb)
	{
	  dump_tree_bb (file, "", bb, 0);
	  fputc ('\n', file);
	}
    }

  fprintf (file, "%s()\n", get_name (current_function_decl));
  print_generic_stmt (file, DECL_SAVED_TREE (current_function_decl),
                      flags|TDF_BLOCK);
  fprintf (file, "\n");
}


/* Dump the flowgraph to a .dot FILE.  */

void
tree_cfg2dot (file)
     FILE *file;
{
  edge e;
  basic_block bb;

  /* Write the file header.  */
  fprintf (file, "digraph %s\n{\n", get_name (current_function_decl));

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
      int head_line, end_line;

      head_code = TREE_CODE (first_stmt (bb));
      end_code = TREE_CODE (last_stmt (bb));

      head_name = tree_code_name[head_code];
      end_name = tree_code_name[end_code];

      head_line = get_lineno (*(bb->head_tree_p));
      end_line = get_lineno (*(bb->end_tree_p));

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
		 Miscellaneous helper functions and predicates
---------------------------------------------------------------------------*/

/* Return the successor block for BB.  If the block has no successors we
   try the enclosing control structure until we find one.  If we reached
   nesting level 0, return the exit block.  */

static basic_block
successor_block (bb)
     basic_block bb;
{
  basic_block succ_bb, parent_bb;
  gimple_stmt_iterator i;

#if defined ENABLE_CHECKING
  if (bb == NULL)
    abort ();
#endif

  /* By default, the successor block will be the block for the statement
     following BB's last statement.  */
  i = gsi_start (bb->end_tree_p);
  gsi_step (&i);
  succ_bb = first_exec_block (gsi_container (i));
  if (succ_bb)
    return succ_bb;

  /* We couldn't find a successor for BB.  Walk up the control structure to
     see if our parent has a successor.  Iterate until we find one or we
     reach nesting level 0.  */
  parent_bb = parent_block (bb);
  while (parent_bb)
    {
      /* If BB is the last block inside a loop body, return the condition
         block for the loop structure.  */
      if (is_loop_stmt (first_stmt (parent_bb)))
	return latch_block (parent_bb);

      /* Otherwise, If BB's control parent has a successor, return its
         block.  */
      i = gsi_start (parent_bb->end_tree_p);
      gsi_step (&i);
      succ_bb = first_exec_block (gsi_container (i));
      if (succ_bb)
	return succ_bb;

      /* None of the above.  Keeping going up the control parent chain.  */
      parent_bb = parent_block (parent_bb);
    }

  /* We reached nesting level 0.  Return the next block in the linked list
     of blocks.  */
  return bb->next_bb;
}


/* Return true if T represents a control statement.  */

bool
is_ctrl_stmt (t)
     tree t;
{
#if defined ENABLE_CHECKING
  if (t == NULL)
    abort ();
#endif

  STRIP_WFL (t);
  return (TREE_CODE (t) == COND_EXPR
	  || TREE_CODE (t) == LOOP_EXPR
	  || TREE_CODE (t) == SWITCH_EXPR);
}


/* Return true if T alters the flow of control (i.e., T is GOTO,
   RETURN or a call to a non-returning function)  */

bool
is_ctrl_altering_stmt (t)
     tree t;
{
#if defined ENABLE_CHECKING
  if (t == NULL)
    abort ();
#endif

  STRIP_WFL (t);
  if (TREE_CODE (t) == GOTO_EXPR || TREE_CODE (t) == RETURN_EXPR)
    return true;

  /* Calls to non-returning functions also alter the flow of control.  */
  if (TREE_CODE (t) == CALL_EXPR)
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
	
      return flags & ECF_NORETURN;
    }

  return false;
}


/* Return true if T represent a loop statement.  */

bool
is_loop_stmt (t)
     tree t;
{
  STRIP_WFL (t);
  return (TREE_CODE (t) == LOOP_EXPR);
}


/* Return true if T is a computed goto.  */

bool
is_computed_goto (t)
     tree t;
{
  STRIP_WFL (t);
  return (TREE_CODE (t) == GOTO_EXPR
          && TREE_CODE (GOTO_DESTINATION (t)) != LABEL_DECL);
}


/* Return true if T should start a new basic block.  */

bool
stmt_starts_bb_p (t)
     tree t;
{
  STRIP_WFL (t);
  return (TREE_CODE (t) == CASE_LABEL_EXPR
	  || TREE_CODE (t) == LABEL_EXPR
	  || TREE_CODE (t) == BIND_EXPR
	  || is_ctrl_stmt (t));
}


/* Remove all the blocks and edges that make up the flowgraph.  */

void
delete_tree_cfg ()
{
  if (basic_block_info == NULL)
    return;

  if (n_basic_blocks > 0)
    {
      free_aux_for_blocks ();
      clear_edges ();
    }

  VARRAY_FREE (basic_block_info);
  VARRAY_FREE (parent_array);
}


/* Returns the block marking the end of the loop body.  This is the block
   that contains the back edge to the start of the loop.  */

basic_block
latch_block (loop_bb)
     basic_block loop_bb;
{
  int entry_ix, latch_ix;
  basic_block latch_bb;
  tree loop = first_stmt (loop_bb);

  if (loop == NULL_TREE || TREE_CODE (loop) != LOOP_EXPR)
    return NULL;

  if (loop_bb->index == INVALID_BLOCK)
    return NULL;

  entry_ix = loop_bb->index;
  latch_ix = entry_ix + 1;
  latch_bb = BASIC_BLOCK (latch_ix);

  /* The latch block for a LOOP_EXPR is guaranteed to be the next block in
     the linked list (see make_loop_expr_blocks).  */
  return latch_bb;
}


/* Return true if BB is a latch block.  */

bool
is_latch_block (bb)
     basic_block bb;
{
  if (bb->index > 0
      && first_stmt (bb) == empty_stmt_node
      && last_stmt (bb) == empty_stmt_node)
    {
      basic_block loop_bb = BASIC_BLOCK (bb->index - 1);
      tree loop_t = first_stmt (loop_bb);
      return (loop_t && TREE_CODE (loop_t) == LOOP_EXPR);
    }

  return false;
}


/* Return a pointer to the first executable statement starting at ENTRY_P.  */

static tree *
first_exec_stmt (entry_p)
     tree *entry_p;
{
  gimple_stmt_iterator i;
  tree stmt;

  for (i = gsi_start (entry_p); !gsi_after_end (i); gsi_step (&i))
    {
      stmt = gsi_stmt (i);
      STRIP_WFL (stmt);
      STRIP_NOPS (stmt);

      /* Note that we actually return the container for the executable
	 statement, not the statement itself.  This is to allow the caller to
	 start iterating from this point.  */
      if (is_exec_stmt (stmt))
	return gsi_container (i);
    }

  return NULL;
}


/* Return the first basic block with executable statements in it, starting
   at ENTRY_P.  */

static basic_block
first_exec_block (entry_p)
     tree *entry_p;
{
  tree *exec_p;

  if (entry_p == NULL || *entry_p == empty_stmt_node)
    return NULL;

  exec_p = first_exec_stmt (entry_p);
  return (exec_p) ? bb_for_stmt (*exec_p) : NULL;
}


/* Returns the header block for the innermost switch statement containing
   BB.  It returns NULL if BB is not inside a switch statement.  */

static basic_block
switch_parent (bb)
     basic_block bb;
{
  do
    bb = parent_block (bb);
  while (bb && TREE_CODE (first_stmt (bb)) != SWITCH_EXPR);

  return bb;
}


/* Return the first statement in basic block BB, stripped of any WFL or NOP
   containers.  */

tree
first_stmt (bb)
     basic_block bb;
{
  gimple_stmt_iterator i;
  tree t;

  if (bb == NULL || bb->index == INVALID_BLOCK)
    return NULL_TREE;

  i = gsi_start_bb (bb);
  t = gsi_stmt (i);
  STRIP_WFL (t);
  STRIP_NOPS (t);
  return t;
}


/* Return the last statement in basic block BB, stripped of any WFL or NOP
   containers.  */

tree
last_stmt (bb)
     basic_block bb;
{
  gimple_stmt_iterator i;
  tree t;
  
  if (bb == NULL || bb->index == INVALID_BLOCK)
    return NULL_TREE;

  i = gsi_start (bb->end_tree_p);
  t = gsi_stmt (i);
  STRIP_WFL (t);
  STRIP_NOPS (t);
  return t;
}
tree *
last_stmt_ptr (bb)
     basic_block bb;
{
  gimple_stmt_iterator i;
  
  if (bb == NULL || bb->index == INVALID_BLOCK)
    return NULL;

  i = gsi_start (bb->end_tree_p);
  return gsi_stmt_ptr (i);
}
