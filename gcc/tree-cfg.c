/* Control flow functions for trees.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
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
#include "diagnostic.h"
#include "tree-flow.h"
#include "timevar.h"
#include "tree-dump.h"
#include "toplev.h"
   
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
  long num_merged_cases;
  long num_merged_labels;
  long num_failed_bind_expr_merges;
};

static struct cfg_stats_d cfg_stats;

/* Basic blocks and flowgraphs.  */
static basic_block make_blocks		PARAMS ((tree *, tree, tree,
						 basic_block));
static void make_cond_expr_blocks	PARAMS ((tree *, tree, basic_block));
static void make_loop_expr_blocks	PARAMS ((tree *, tree, basic_block));
static void make_switch_expr_blocks	PARAMS ((tree *, tree, basic_block));
static basic_block make_bind_expr_blocks PARAMS ((tree *, tree, basic_block,
						  tree));
static inline void add_stmt_to_bb	PARAMS ((tree *, basic_block, tree));
static basic_block create_bb		PARAMS ((void));

/* Edges.  */
static void make_edges			PARAMS ((void));
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
static basic_block switch_parent	PARAMS ((basic_block));
static inline bool stmt_starts_bb_p	PARAMS ((tree, tree));
static inline bool stmt_ends_bb_p	PARAMS ((tree));

/* Flowgraph optimization and cleanup.  */
static void remove_unreachable_blocks	PARAMS ((void));
static void remove_unreachable_block	PARAMS ((basic_block));
static void remove_bb			PARAMS ((basic_block, int));
static void remove_stmt			PARAMS ((tree *));
static bool blocks_unreachable_p	PARAMS ((varray_type));
static void remove_blocks		PARAMS ((varray_type));
static varray_type find_subblocks	PARAMS ((basic_block));
static bool is_parent			PARAMS ((basic_block, basic_block));
static void cleanup_control_flow	PARAMS ((void));
static void cleanup_cond_expr_graph	PARAMS ((basic_block));
static void cleanup_switch_expr_graph	PARAMS ((basic_block));
static void disconnect_unreachable_case_labels PARAMS ((basic_block));
static edge find_taken_edge_cond_expr	PARAMS ((basic_block, tree));
static edge find_taken_edge_switch_expr	PARAMS ((basic_block, tree));
static bool value_matches_some_label	PARAMS ((edge, tree, edge *));

/* Block iterator helpers.  */

static block_stmt_iterator bsi_init 	PARAMS ((tree *, basic_block));


/* Remove any COMPOUND_EXPR container from NODE.  */
#define STRIP_CONTAINERS(NODE)					\
  do {								\
    while (TREE_CODE (NODE) == COMPOUND_EXPR)			\
      NODE = TREE_OPERAND (NODE, 0);				\
  } while (0)


/* NEXT_BLOCK_LINK is used to store the successor statement of the entry
   statement to a lexical or control block.  This allows successor_block to
   find the block that should come after the last statement of the last
   block inside a lexical scope.  For instance,

	    1	if (...)
	    2	  {
	    3	    s1;
	    4	    {
	    5	      s2;
	    6	      s3;
	    7	    }
	    8	  }
	    9	s4;

  When make_blocks starts processing the if() at line 1, it sets
  NEXT_BLOCK_LINK to be 's4'.  This way, when it finishes the basic block
  at line 6, it sets NEXT_BLOCK_LINK (s3) to 's4'.  */
#define NEXT_BLOCK_LINK(STMT)	TREE_CHAIN (STMT)


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

  timevar_push (TV_TREE_CFG);

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

  /* Find the basic blocks for the flowgraph.  First skip any
     non-executable statements at the start of the function.  Otherwise
     we'll end up with an empty basic block 0, which is useless.  */

  if (fnbody == empty_stmt_node || TREE_CODE (fnbody) != BIND_EXPR)
    {
      timevar_pop (TV_TREE_CFG);
      return;
    }

  first_p = first_exec_stmt (&BIND_EXPR_BODY (fnbody));
  if (first_p)
    {
      make_blocks (first_p, NULL_TREE, NULL, NULL);

      if (n_basic_blocks > 0)
	{
	  /* Adjust the size of the array.  */
	  VARRAY_GROW (basic_block_info, n_basic_blocks);

	  /* Create block annotations.  */
	  alloc_aux_for_blocks (sizeof (struct bb_ann_d));

	  /* Create the edges of the flowgraph.  */
	  make_edges ();
	}
    }

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


/* Build a flowgraph for the statements starting at the statement pointed
   by FIRST_P.
   
   PARENT_STMT is the entry statement for the control structure immediately
      enclosing the new sub-graph.

   BB is the block where the statements should be added to.  If BB is NULL,
      a new basic block will be created for the statements.

   Return the last basic block added to the graph.  This is used to know if
   a recursive invocation built a sub-graph whose last block can accept
   more statements or not.  */

static basic_block
make_blocks (first_p, next_block_link, parent_stmt, bb)
     tree *first_p;
     tree next_block_link;
     tree parent_stmt;
     basic_block bb;
{
  tree_stmt_iterator i;
  tree stmt, last;
  bool start_new_block;

  if (first_p == NULL
      || *first_p == empty_stmt_node
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

      /* Set the block for the container of non-executable statements.  */
      if (stmt == NULL_TREE)
	{
	  if (bb && *stmt_p != empty_stmt_node)
	    add_stmt_to_bb (stmt_p, bb, parent_stmt);
	  continue;
	}

      STRIP_NOPS (stmt);
      NEXT_BLOCK_LINK (stmt) = NULL_TREE;
      code = TREE_CODE (stmt);

      /* If the statement starts a new basic block or if we have determined
	 in a previous pass that we need to create a new block for STMT, do
	 so now.  */
      if (start_new_block || stmt_starts_bb_p (stmt, prev_stmt))
	{
	  bb = create_bb ();
	  start_new_block = false;
	}

      /* Now add STMT to BB and create the subgraphs for special statement
	 codes.  */
      add_stmt_to_bb (stmt_p, bb, parent_stmt);

      if (code == LOOP_EXPR)
	make_loop_expr_blocks (stmt_p, next_block_link, bb);
      else if (code == COND_EXPR)
	make_cond_expr_blocks (stmt_p, next_block_link, bb);
      else if (code == SWITCH_EXPR)
	make_switch_expr_blocks (stmt_p, next_block_link, bb);
      else if (code == BIND_EXPR)
	{
	  int num_blocks_before;
	  basic_block last_bb;

	  /* BIND_EXPR nodes are a special case.  We neither force a new
	     block for their bodies, nor force a new block after creating
	     the subgraph.  On return from make_bind_expr_blocks, LAST_BB
	     will be the last basic block of the BIND_EXPR's subgraph.  We
	     point STMT to LAST_BB's last statement to determine if we
	     should start a new block or not.  */
	  num_blocks_before = n_basic_blocks;
	  last_bb = make_bind_expr_blocks (stmt_p, next_block_link, bb,
	                                   parent_stmt);
	  if (last_bb)
	    {
	      bb = last_bb;
	      stmt = last_stmt (bb);
	    }

	  /* FIXME.  Obscene hack to work around iterator limitations.  If
	     during processing of the BIND_EXPR body we were forced to
	     create new blocks (i.e., the BIND_EXPR body contains control
	     flow structures), then force the creation of a new basic block
	     for the next iteration.  This avoids the following problem
	     (assume that all the Si statements are regular GIMPLE
	     statements):

		    1	s1;		<-- BLOCK #0
		    2	{
		    3	  s2;
		    4	  s3;
		    5	  if ()
		    6	    s4;		<-- BLOCK #1
		    7	  s5;		<-- BLOCK #2
		    8	}
		    9	s6;

	     Since s5 and s6 are two regular statements, they could both be
	     in block #2.  However, if we started an iterator on block #2,
	     the iterator would have no way of knowing how to go from
	     statement s5 to statement s6 because the iterator was started
	     in the middle of its BIND_EXPR's body, so bsi_step_in_bb() has
	     not enough context to determine how to get to s6.  */
	  if (n_basic_blocks > num_blocks_before)
	    {
	      start_new_block = true;

	      /* If we are starting the new block just to work around
		 iterator limitations, keep track of it.  */
	      if (!stmt_ends_bb_p (stmt))
		cfg_stats.num_failed_bind_expr_merges++;
	    }
	}
      else if (code == TRY_FINALLY_EXPR || code == TRY_CATCH_EXPR)
	{
	  /* FIXME: Need to handle these nodes for C++ and mudflap.  */
	  abort ();
	}

      /* If STMT is a basic block terminator, set START_NEW_BLOCK for the
	 next iteration.  */
      if (stmt_ends_bb_p (stmt))
	start_new_block = true;

      last = stmt;
    }

  /* If LAST_STMT is set, link it to NEXT_BLOCK_LINK.  This allows making
     edges from the last block inside a lexical scope (see
     successor_block).  */
  if (last)
    {
      NEXT_BLOCK_LINK (last) = next_block_link;
      return bb_for_stmt (last);
    }

  return NULL;
}


/* Create the blocks for the LOOP_EXPR node pointed by LOOP_P.

   NEXT_BLOCK_LINK is the first statement of the successor basic block for
      the block holding *LOOP_P.  If *LOOP_P is the last statement inside a
      lexical scope, this will be the statement that comes after LOOP_P's
      container (see the documentation for NEXT_BLOCK_LINK).

   ENTRY is the block whose last statement is *LOOP_P.  */

static void
make_loop_expr_blocks (loop_p, next_block_link, entry)
     tree *loop_p;
     tree next_block_link;
     basic_block entry;
{
  tree_stmt_iterator si;
  tree loop = *loop_p;
  
  entry->flags |= BB_COMPOUND_ENTRY | BB_CONTROL_EXPR | BB_LOOP_CONTROL_EXPR;

  /* Determine NEXT_BLOCK_LINK for statements inside the LOOP_EXPR body.
     Note that in the case of a loop, NEXT_BLOCK_LINK should be the
     first statement of the LOOP_EXPR body.  This is because LOOP_EXPR
     statements are actually infinite loops, so they can only be left with a
     'goto' statement.  Any other statement that reaches the end of the
     LOOP_EXPR body, will naturally loop back.  */
  STRIP_CONTAINERS (loop);
  si = tsi_start (&LOOP_EXPR_BODY (loop));
  next_block_link = tsi_stmt (si);

  /* If the loop body is empty, point NEXT_BLOCK_LINK to the statement
     following the LOOP_EXPR node, as we do with the other control
     structures.  */
  if (next_block_link == NULL_TREE)
    {
      si = tsi_start (loop_p);
      tsi_next (&si);
      if (!tsi_end_p (si))
	next_block_link = tsi_stmt (si);
    }

  make_blocks (&LOOP_EXPR_BODY (loop), next_block_link, loop, NULL);
}


/* Create the blocks for the COND_EXPR node pointed by COND_P.

   NEXT_BLOCK_LINK is the first statement of the successor basic block for
      the block holding *COND_P.  If *COND_P is the last statement inside a
      lexical scope, this will be the statement that comes after COND_P's
      container (see the documentation for NEXT_BLOCK_LINK).

   ENTRY is the block whose last statement is *COND_P.  */

static void
make_cond_expr_blocks (cond_p, next_block_link, entry)
     tree *cond_p;
     tree next_block_link;
     basic_block entry;
{
  tree_stmt_iterator si;
  tree cond = *cond_p;
  entry->flags |= BB_COMPOUND_ENTRY | BB_CONTROL_EXPR;

  /* Determine NEXT_BLOCK_LINK for statements inside the COND_EXPR body.  */
  si = tsi_start (cond_p);
  tsi_next (&si);
  if (!tsi_end_p (si))
    next_block_link = tsi_stmt (si);

  STRIP_CONTAINERS (cond);
  make_blocks (&COND_EXPR_THEN (cond), next_block_link, cond, NULL);
  make_blocks (&COND_EXPR_ELSE (cond), next_block_link, cond, NULL);
}


/* Create the blocks for the SWITCH_EXPR node pointed by SWITCH_E_P.

   NEXT_BLOCK_LINK is the first statement of the successor basic block for
      the block holding *SWITCH_E_P.  If *SWITCH_E_P is the last statement
      inside a lexical scope, this will be the statement that comes after
      *SWITCH_E_P's container (see the documentation for NEXT_BLOCK_LINK).

   ENTRY is the block whose last statement is *SWITCH_E_P.  */

static void
make_switch_expr_blocks (switch_e_p, next_block_link, entry)
     tree *switch_e_p;
     tree next_block_link;
     basic_block entry;
{
  tree_stmt_iterator si;
  tree switch_e = *switch_e_p;
  entry->flags |= BB_COMPOUND_ENTRY | BB_CONTROL_EXPR;

  /* Determine NEXT_BLOCK_LINK for statements inside the COND_EXPR body.  */
  si = tsi_start (switch_e_p);
  tsi_next (&si);
  if (!tsi_end_p (si))
    next_block_link = tsi_stmt (si);

  STRIP_CONTAINERS (switch_e);
  make_blocks (&SWITCH_BODY (switch_e), next_block_link, switch_e, NULL);
}


/* Create the blocks for the BIND_EXPR node pointed by BIND_P.  In contrast
   with the other make_*_blocks functions, this function will not start a
   new basic block for the statements in the BIND_EXPR body.  Rather, the
   statements in the BIND_EXPR body are added to the block ENTRY and use
   the same PARENT_STMT.

   NEXT_BLOCK_LINK is the first statement of the successor basic block for
      the block holding *BIND_P.  If *BIND_P is the last statement inside a
      lexical scope, this will be the statement that comes after *BIND_P's
      container (see the documentation for NEXT_BLOCK_LINK).

   ENTRY is the block whose last statement is *SWITCH_E_P.
   
   Return the last basic block added to the BIND_EXPR's subgraph.  This
   allows the caller to determine whether a new block should be started or
   not.  */

static basic_block
make_bind_expr_blocks (bind_p, next_block_link, entry, parent_stmt)
     tree *bind_p;
     tree next_block_link;
     basic_block entry;
     tree parent_stmt;
{
  tree_stmt_iterator si;
  tree bind = *bind_p;

  /* Determine NEXT_BLOCK_LINK for statements inside the BIND_EXPR
     body.  */
  si = tsi_start (bind_p);
  tsi_next (&si);
  if (!tsi_end_p (si))
    next_block_link = tsi_stmt (si);

  /* By passing the current block ENTRY to make_blocks, we will keep adding
     statements to ENTRY until we find a block terminating statement inside
     the body of the BIND_EXPR.  On return from make_blocks, our caller
     will start a new basic block only if the body of the BIND_EXPR node
     ends with a block terminating statement.  */
  STRIP_CONTAINERS (bind);
  return make_blocks (&BIND_EXPR_BODY (bind), next_block_link, parent_stmt,
		      entry);
}


/* Add statement pointed by STMT_P to basic block BB.  PARENT_STMT is the
   entry statement to the control structure holding *STMT_P.  */

static inline void
add_stmt_to_bb (stmt_p, bb, parent_stmt)
     tree *stmt_p;
     basic_block bb;
     tree parent_stmt;
{
  tree t;

  set_bb_for_stmt (*stmt_p, bb);

  /* Link *STMT_P (and the trees it contains) to its control parent.  */
  t = *stmt_p;
  do
    {
      stmt_ann_t ann = stmt_ann (t);
      ann->parent_stmt = parent_stmt;
      t = (TREE_CODE (t) == COMPOUND_EXPR) ? TREE_OPERAND (t, 0) : NULL_TREE;
    }
  while (t && t != empty_stmt_node);

  /* Update the head and tail of the block.  */
  if (bb->head_tree_p == NULL)
    bb->head_tree_p = stmt_p;

  bb->end_tree_p = stmt_p;
}


/* Create and return a new basic block.  */

static basic_block
create_bb ()
{
  basic_block bb;

  /* Create and initialize a new basic block.  */
  bb = alloc_block ();
  memset (bb, 0, sizeof (*bb));

  bb->index = last_basic_block;
  bb->flags = BB_NEW;

  /* Add the new block to the linked list of blocks.  */
  if (n_basic_blocks > 0)
    link_block (bb, BASIC_BLOCK (n_basic_blocks - 1));
  else
    link_block (bb, ENTRY_BLOCK_PTR);

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

      if (first)
        {
	  /* Edges for control statements.  */
	  if (is_ctrl_stmt (last))
	    make_ctrl_stmt_edges (bb);

	  /* Edges for control flow altering statements (GOTO_EXPR and
	     RETURN_EXPR) need an edge to the corresponding target block.  */
	  if (is_ctrl_altering_stmt (last))
	    make_exit_edges (bb);

	  /* Incoming edges for label blocks in switch statements.  It's
	     easier to deal with these bottom-up than top-down.  */
	  if (TREE_CODE (first) == CASE_LABEL_EXPR)
	    make_case_label_edges (bb);
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
  tree last = last_stmt (bb);

#if defined ENABLE_CHECKING
  if (last == NULL_TREE)
    abort();
#endif

  switch (TREE_CODE (last))
    {
    case LOOP_EXPR:
      make_loop_expr_edges (bb);
      break;

    case COND_EXPR:
      make_cond_expr_edges (bb);
      break;

    case SWITCH_EXPR:
      break;

    default:
      abort ();
    }
}


/* Create exit edges for statements in block BB that alter the flow of
   control.  Statements that alter the control flow are 'goto', 'return'
   and calls to non-returning functions.  */

static void
make_exit_edges (bb)
     basic_block bb;
{
  tree last = last_stmt (bb);

  if (last == NULL_TREE)
    abort ();

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

      /* A CALL_EXPR node here means that the last statement of the block
	 is a call to a non-returning function.  */
    case CALL_EXPR:
      /* Some calls are known not to return, so we just need to make
	 an edge from them to the exit block.  Note that we do not
	 need to worry about nonlocal gotos for such calls.  */
      if (call_expr_flags (last) & (ECF_NORETURN | ECF_LONGJMP))
	make_edge (bb, EXIT_BLOCK_PTR, 0);
      else if (FUNCTION_RECEIVES_NONLOCAL_GOTO (current_function_decl))
	{
	  make_goto_expr_edges (bb);
          make_edge (bb, successor_block (bb), EDGE_FALLTHRU);
	}
      break;

    case RETURN_EXPR:
      make_edge (bb, EXIT_BLOCK_PTR, 0);

      /* A RETURN_EXPR may contain a CALL_EXPR and the CALL_EXPR may
	 have an abnormal edge.  Search the operand of the RETURN_EXPR
	 for this case and create any required edges.  */
      if (TREE_OPERAND (last, 0)
	  && TREE_CODE (TREE_OPERAND (last, 0)) == MODIFY_EXPR
	  && TREE_CODE (TREE_OPERAND (TREE_OPERAND (last, 0), 1)) == CALL_EXPR
	  && FUNCTION_RECEIVES_NONLOCAL_GOTO (current_function_decl))
	make_goto_expr_edges (bb);
      break;

    case MODIFY_EXPR:
      /* A MODIFY_EXPR may have a CALL_EXPR on its RHS and the CALL_EXPR
	 may have an abnormal edge.  Search the RHS for this case and
	 create any required edges.  */
      if (TREE_CODE (TREE_OPERAND (last, 1)) == CALL_EXPR
	  && FUNCTION_RECEIVES_NONLOCAL_GOTO (current_function_decl))
	{
	  make_goto_expr_edges (bb);
          make_edge (bb, successor_block (bb), EDGE_FALLTHRU);
	}
      break;

    default:
      abort ();
    }
}


/* Create the edges for the LOOP_EXPR structure starting at block BB.
   Only create the edge that join the LOOP_EXPR header block to the loop
   body.  The edge out of the loop and back into the LOOP_EXPR header will
   be naturally created by the main loop in create_edges().

               LOOP_EXPR
	           |
                   v
             LOOP_EXPR_BODY  */

static void
make_loop_expr_edges (bb)
     basic_block bb;
{
  tree entry = last_stmt (bb);
  basic_block body_bb;

#if defined ENABLE_CHECKING
  if (entry == NULL_TREE || TREE_CODE (entry) != LOOP_EXPR)
    abort ();
#endif

  body_bb = first_exec_block (&LOOP_EXPR_BODY (entry));
  if (body_bb)
    make_edge (bb, body_bb, 0);
}


/* Create the edges for a COND_EXPR starting at block BB.

   Create the following edges.

	     COND_EXPR
		/ \
	       /   \
	    THEN   ELSE

   Either clause may be empty.  */

static void
make_cond_expr_edges (bb)
     basic_block bb;
{
  tree entry = last_stmt (bb);
  basic_block successor_bb, then_bb, else_bb;

#if defined ENABLE_CHECKING
  if (entry == NULL_TREE || TREE_CODE (entry) != COND_EXPR)
    abort ();
#endif

  /* Entry basic blocks for each component.  */
  then_bb = first_exec_block (&COND_EXPR_THEN (entry));
  else_bb = first_exec_block (&COND_EXPR_ELSE (entry));
  successor_bb = successor_block (bb);

  if (then_bb)
    make_edge (bb, then_bb, EDGE_TRUE_VALUE);

  if (else_bb)
    make_edge (bb, else_bb, EDGE_FALSE_VALUE);

  /* If conditional is missing one of the clauses, make an edge between the
     entry block and the first block outside the conditional.  */
  if (!then_bb || !else_bb)
    make_edge (bb, successor_bb, 0);
}


/* Create edges for a goto statement at block BB.  */

static void
make_goto_expr_edges (bb)
     basic_block bb;
{
  tree goto_t, dest;
  basic_block target_bb;
  int edge_flags;
  int for_call;

  goto_t = last_stmt (bb);

  /* If the last statement is not a GOTO (ie, it is a RETURN_EXPR, CALL_EXPR
     or MODIFY_EXPR, then the edge is an abnormal edge resulting from
     a nonlocal goto.  */
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
      /* The RTL CFG code got this wrong.  A computed goto creates
	 normal edges, except for one case.  Namely a computed goto
	 in a nested function has an abnormal edge to the exit block
	 (which is handled elsewhere).  */
      edge_flags = 0;

      if (TREE_CODE (dest) == LABEL_DECL && ! NONLOCAL_LABEL (dest))
	{
	  make_edge (bb,
		     VARRAY_BB (label_to_block_map, LABEL_DECL_INDEX (dest)),
		     edge_flags);
	  return;
	}
    }

  /* Look for the block starting with the destination label.  In the
     case of a computed goto, make an edge to any label block we find
     in the CFG.  */
  FOR_EACH_BB (target_bb)
    {
      tree target = first_stmt (target_bb);

      if (target == NULL_TREE)
        continue;

      /* Computed GOTOs.  Make an edge to every label block that has
	 been marked as a potential target for a computed goto.  */
      if (TREE_CODE (dest) != LABEL_DECL
	       && TREE_CODE (target) == LABEL_EXPR
	       && FORCED_LABEL (LABEL_EXPR_LABEL (target))
	       && for_call == 0)
	make_edge (bb, target_bb, edge_flags);

      /* Nonlocal GOTO target.  Make an edge to every label block that has
	 been marked as a potential target for a nonlocal goto.  */
      else if (TREE_CODE (dest) != LABEL_DECL
	       && TREE_CODE (target) == LABEL_EXPR
	       && NONLOCAL_LABEL (LABEL_EXPR_LABEL (target))
	       && for_call == 1)
	make_edge (bb, target_bb, edge_flags);
    }
}


/* Create the edge between a case label at block BB and the block for the
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
	remove_unreachable_block (bb);
    }
}


/* Helper for remove_unreachable_blocks.  */

static void
remove_unreachable_block (bb)
     basic_block bb;
{
  varray_type subblocks;

  if (bb->flags & BB_COMPOUND_ENTRY)
    {
      /* Before removing an entry block for a compound structure,
         make sure that all its subblocks are unreachable as well.
	 FIXME: This is lame.  We should linearize this control
	 structure.  The problem is that we do need to remove the entry
	 block.  Otherwise, we will fail when computing dominance
	 information.  This is usually caused by unstructured control flow.
	 E.g. (from real.c),

		    1	goto start;
		    2	do
		    3	  {
		    4	    s1;
		    5	  start:
		    6	    s2;
		    7	    s3;
		    8	  } while (...);

	 The entry block (line 2) is unreachable but its body isn't.  */
      subblocks = find_subblocks (bb);
      if (blocks_unreachable_p (subblocks))
	{
	  remove_blocks (subblocks);
	  remove_bb (bb, 1);
	}
      else
	remove_bb (bb, 0);
    }
  else
    remove_bb (bb, 1);
}


/* Remove block BB and its statements from the flowgraph.  REMOVE_STMTS is
   nonzero if the statements in BB should also be removed.

   Note that if REMOVE_STMTS is nonzero and BB is the entry block for a
   compound statement (control structures or blocks of code), removing BB
   will effectively remove the whole structure from the program.  The
   caller is responsible for making sure that all the blocks in the
   compound structure are also removed.  */

static void
remove_bb (bb, remove_stmts)
     basic_block bb;
     int remove_stmts;
{
  block_stmt_iterator i;
  int already_warned;

  dump_file = dump_begin (TDI_cfg, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "Removing unreachable basic block %d\n", bb->index);
      dump_tree_bb (dump_file, "", bb, 0);
      fprintf (dump_file, "\n");
      dump_end (TDI_cfg, dump_file);
      dump_file = NULL;
    }

  /* Remove all the instructions in the block.  */
  already_warned = 0;
  for (i = bsi_start (bb); !bsi_end_p (i); )
    {
      tree stmt = bsi_stmt (i);

      set_bb_for_stmt (stmt, NULL);
      if (remove_stmts)
        {
	  if (! already_warned && warn_notreached)
	    {
	      warning_with_file_and_line (get_filename (stmt),
			      		  get_lineno (stmt), 
			  	          "will never be executed");
	      already_warned = 1;
	    }
	  bsi_remove (&i);
        }
      else
        bsi_next (&i);
    }

  set_bb_for_stmt (*bb->head_tree_p, NULL);
  set_bb_for_stmt (*bb->end_tree_p, NULL);

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

  while (bb->succ != NULL)
    {
      tree phi;

      /* PHI nodes in successors of this block now have one less
         alternative.  */
      for (phi = phi_nodes (bb->succ->dest); phi; phi = TREE_CHAIN (phi))
	remove_phi_arg (phi, bb);
      remove_edge (bb->succ);
    }


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
      remove_bb (bb, 1);
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
      if (bb->flags & BB_REACHABLE)
	return false;
    }

  return true;
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


/* Return true if BB is a control parent for CHILD_BB.

   Notice that this property is not the same as dominance.  This
   is a test for containment.  Given two blocks A and B, A DOM B
   does not imply A is-parent-of B.  For instance,

	    1	{
	    2	  s1;
	    3	}
	    4	{
	    5	  s2;
	    6	}

   The block at line 1 dominates the block at line 4, but line 4
   is not contained in 1's compound structure.  */

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


/* Remove statement pointed by iterator I.

    Note that this function will wipe out control statements that
    may span multiple basic blocks.  Make sure that you really
    want to remove the whole control structure before calling this
    function.  */

void
bsi_remove (i)
     block_stmt_iterator *i;
{
  tree t = *(i->tp);

  STRIP_NOPS (t);

  if (!is_exec_stmt (t))
    return;

  if (TREE_CODE (t) == COMPOUND_EXPR)
    {
      remove_stmt (&TREE_OPERAND (t, 0));

      /* If both operands are empty, delete the whole COMPOUND_EXPR.  */
      if (TREE_OPERAND (t, 1) == empty_stmt_node)
	remove_stmt (i->tp);
    }
  else
    remove_stmt (i->tp);

  bsi_next (i);
}


/* Remove statement *STMT_P.

   Update all references associated with it.  Note that this function will
   wipe out control statements that may span multiple basic blocks.  Make
   sure that you really want to remove the whole control structure before
   calling this function.  */

static void
remove_stmt (stmt_p)
     tree *stmt_p;
{
  varray_type vdefs;
  size_t i;
  tree *def_p;
  tree stmt = *stmt_p;

  STRIP_NOPS (stmt);

  /* If the statement is a LABEL_EXPR, remove the LABEL_DECL from
     the symbol table.  */
  if (TREE_CODE (stmt) == LABEL_EXPR)
    remove_decl (LABEL_EXPR_LABEL (stmt));

  /* If the statement is already in SSA form, mark all the definitions made in
     the statement invalid.

     FIXME: We should probably traverse all the def-use edges originating at
	    this statement to update each use of the definitions made here, but
	    that is expensive and can easily be checked by every pass by
	    checking if SSA_NAME_DEF_STMT is empty_stmt_node.  */
  def_p = def_op (stmt);
  if (def_p && TREE_CODE (*def_p) == SSA_NAME)
    SSA_NAME_DEF_STMT (*def_p) = empty_stmt_node;

  vdefs = vdef_ops (stmt);
  for (i = 0; vdefs && i < VARRAY_ACTIVE_SIZE (vdefs); i++)
    {
      tree vdef = VDEF_RESULT (VARRAY_TREE (vdefs, i));
      if (TREE_CODE (vdef) == SSA_NAME)
	SSA_NAME_DEF_STMT (vdef) = empty_stmt_node;
    }

  stmt->common.ann = NULL;

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

  /* Replace STMT with empty_stmt_node.  */
  *stmt_p = empty_stmt_node;
}


/* Try to remove superfluous control structures.  */

static void
cleanup_control_flow ()
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      enum tree_code code;
      tree t;

      t = last_stmt (bb);
      if (t == NULL_TREE)
        continue;

      if (bb->flags & BB_CONTROL_EXPR
	  && bb->flags & BB_COMPOUND_ENTRY)
	{
	  code = TREE_CODE (t);

	  if (code == COND_EXPR)
	    cleanup_cond_expr_graph (bb);
	  else if (code == SWITCH_EXPR)
	    cleanup_switch_expr_graph (bb);
	}
    }
}

      
/* Disconnect an unreachable block in the conditional expression starting
   at block BB.

   If the predicate of the COND_EXPR node in block BB is constant,
   disconnect the subgraph that contains the clause that is never executed.  */

static void
cleanup_cond_expr_graph (bb)
     basic_block bb;
{
  tree cond_expr = last_stmt (bb);
  tree val;
  edge taken_edge;

#if defined ENABLE_CHECKING
  if (cond_expr == NULL_TREE || TREE_CODE (cond_expr) != COND_EXPR)
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
	    {
	      tree phi;

	      /* Remove the appropriate PHI alternative in the
	         target block for each unexecutable edge.  */
	      for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
		remove_phi_arg (phi, e->dest);
	      remove_edge (e);
	    }
	}
    }
}


/* Disconnect unreachable blocks in the 'switch' expression starting at
   block SWITCH_BB.

   If the switch condition of the SWITCH_EXPR node in block SWITCH_BB is
   constant, disconnect all the subgraphs for all the case labels that will
   never be taken.  */

static void
cleanup_switch_expr_graph (switch_bb)
     basic_block switch_bb;
{
#if defined ENABLE_CHECKING
  tree switch_expr = last_stmt (switch_bb);
#endif
  edge e;

#if defined ENABLE_CHECKING
  if (switch_expr == NULL_TREE || TREE_CODE (switch_expr) != SWITCH_EXPR)
    abort ();
#endif

  disconnect_unreachable_case_labels (switch_bb);

  /* If the switch() has a default label, remove the fallthru edge that was
     created when we processed the entry block for the switch() statement.  */
  for (e = switch_bb->succ; e; e = e->succ_next)
    {
      tree t = first_stmt (e->dest);
      if (t && TREE_CODE (t) == CASE_LABEL_EXPR && CASE_LOW (t) == NULL_TREE)
	{
	  basic_block chain_bb = successor_block (switch_bb);
	  edge e = find_edge (switch_bb, chain_bb);
	  if (e)
	    {
	      tree phi;

	      /* Remove the appropriate PHI alternative in the
	         target block for each unexecutable edge.  */
	      for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
		remove_phi_arg (phi, e->src);
	      remove_edge (e);
	    }
	  break;
	}
    }
}


/* Clean up the 'switch' expression at block BB.

   If the switch() statement starting at basic block BB has a constant
   condition, disconnect all the unreachable case labels.  */

static void
disconnect_unreachable_case_labels (bb)
     basic_block bb;
{
  edge taken_edge;
  tree switch_val;
  tree t = last_stmt (bb);

  if (t == NULL_TREE)
    return;

  switch_val = SWITCH_COND (t);
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
	    {
	      tree phi;

	      /* Remove the appropriate PHI alternative in the
	         target block for each unexecutable edge.  */
	      for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
		remove_phi_arg (phi, e->src);
	      remove_edge (e);
	    }
	}
    }
}


/* Given a control block BB and a constant value VAL, return the edge that
   will be taken out of the block.  If VAL does not match a unique edge,
   NULL is returned.  */

edge
find_taken_edge (bb, val)
     basic_block bb;
     tree val;
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

  /* LOOP_EXPR nodes are always followed by their successor block.  */
  return bb->succ;
}


/* Given a constant value VAL and the entry block BB to a COND_EXPR
   statement, determine which of the two edges will be taken out of the
   block.  Return NULL if either edge may be taken.  */

static edge
find_taken_edge_cond_expr (bb, val)
     basic_block bb;
     tree val;
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

  /* If E is not going to the THEN nor the ELSE clause, then it's
     the fallthru edge to the successor block of the if() block.  */
  return find_edge (bb, successor_block (bb));
}


/* Given a constant value VAL and the entry block BB to a SWITCH_EXPR
   statement, determine which edge will be taken out of the block.  Return
   NULL if any edge may be taken.  */

static edge
find_taken_edge_switch_expr (bb, val)
     basic_block bb;
     tree val;
{
  edge e, default_edge;

  /* See if the switch() value matches one of the case labels.  */
  default_edge = NULL;
  for (e = bb->succ; e; e = e->succ_next)
    {
      edge dest_edge;
      tree dest_t;

      dest_edge = e;
      dest_t = first_stmt (dest_edge->dest);

      /* We are only interested in edges that go to CASE_LABEL_EXPRs.  */
      if (dest_t == NULL_TREE || TREE_CODE (dest_t) != CASE_LABEL_EXPR)
	continue;

      if (value_matches_some_label (dest_edge, val, &default_edge))
	return dest_edge;
    }

  /* If no case exists for the value used in the switch(), we use the
     default label.  If the switch() has no default label, we use the edge
     going out of the switch() body.  */
  return default_edge ? default_edge : find_edge (bb, successor_block (bb));
}


/* Return true if VAL matches one of the labels in the destination block of
   edge DEST_EDGE.  If one of the labels in the block is the DEFAULT label,
   DEST_EDGE is stored into *DEFAULT_EDGE_P to indicate that this edge goes
   to the DEFAULT label.  This is used by the caller when no other case
   label is found to match VAL.  */

static bool
value_matches_some_label (dest_edge, val, default_edge_p)
     edge dest_edge;
     tree val;
     edge *default_edge_p;
{
  basic_block dest_bb = dest_edge->dest;
  block_stmt_iterator i;

  for (i = bsi_start (dest_bb); !bsi_end_p (i); bsi_next (&i))
    {
      tree stmt = bsi_stmt (i);

      /* No more labels.  We haven't found a match.  */
      if (TREE_CODE (stmt) != CASE_LABEL_EXPR)
	return false;

      /* Remember that we found a default label, just in case no other
	  label matches the switch() value.  */
      if (CASE_LOW (stmt) == NULL_TREE)
	*default_edge_p = dest_edge;
      else
	{
	  /* If we found a match, we are done.  */
	  tree label_val = CASE_LOW (stmt);
	  if (simple_cst_equal (label_val, val) == 1)
	    return true;
	}
    }

  return false;
}


/*---------------------------------------------------------------------------
			 Code insertion and replacement
---------------------------------------------------------------------------*/

/* Insert basic block NEW_BB before block BB.  */

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
  char *s_indent;
  int lineno;
  basic_block loop_bb;
  tree head = (bb->head_tree_p) ? *(bb->head_tree_p) : NULL_TREE;
  tree end = (bb->end_tree_p) ? *(bb->end_tree_p) : NULL_TREE;

  s_indent = (char *) alloca ((size_t) indent + 1);
  memset ((void *) s_indent, ' ', (size_t) indent);
  s_indent[indent] = '\0';

  fprintf (outf, "%s%sBasic block %d", s_indent, prefix, bb->index);

  loop_bb = is_latch_block_for (bb);
  if (loop_bb)
    fprintf (outf, " (latch for #%d)\n", loop_bb->index);
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
  if (bb->aux && parent_block (bb))
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


/* Dump the CFG on stderr.

   FLAGS are the same used by the tree dumping functions
   (see TDF_* in tree.h).  */

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
      fprintf (file, ";; Function %s\n\n", current_function_name);
      fprintf (file, ";; \n%d basic blocks, %d edges, last basic block %d.\n",
	       n_basic_blocks, n_edges, last_basic_block);

      FOR_EACH_BB (bb)
	{
	  dump_tree_bb (file, "", bb, 0);
	  fputc ('\n', file);
	}
    }

  if (flags & TDF_STATS)
    dump_cfg_stats (dump_file);

  if (n_basic_blocks > 0)
    dump_function_to_file (current_function_decl, file, flags|TDF_BLOCKS);
}


/* Dump CFG statistics on FILE.  */

void
dump_cfg_stats (file)
     FILE *file;
{
  static long max_num_merged_cases = 0;
  static long max_num_merged_labels = 0;
  unsigned long size, total = 0;
  long n_edges;
  basic_block bb;
  const char * const fmt_str   = "%-30s%-13s%12s\n";
  const char * const fmt_str_1 = "%-30s%13lu%11lu%c\n";
  const char * const fmt_str_3 = "%-43s%11lu%c\n";

  fprintf (file, "\nCFG Statistics for %s\n\n", current_function_name);

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
debug_cfg_stats ()
{
  dump_cfg_stats (stderr);
}


/* Dump the flowgraph to a .dot FILE.  */

void
tree_cfg2dot (file)
     FILE *file;
{
  edge e;
  basic_block bb;

  /* Write the file header.  */
  fprintf (file, "digraph %s\n{\n", current_function_name);

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

/* Return the successor block for BB.  If the block has no successors we
   try the enclosing control structure until we find one.  If we reached
   nesting level 0, return the exit block.  */

static basic_block
successor_block (bb)
     basic_block bb;
{
  basic_block succ_bb;
  tree_stmt_iterator i;
  tree last_stmt;

#if defined ENABLE_CHECKING
  if (bb == NULL)
    abort ();
#endif

  /* By default, the successor block will be the block for the statement
     following BB's last statement.  */
  i = tsi_start (bb->end_tree_p);
  last_stmt = tsi_stmt (i);

  /* Special case.  If the block ends in a BIND_EXPR node, the successor
     block will be inside the BIND_EXPR's body.  */
  if (last_stmt && TREE_CODE (last_stmt) == BIND_EXPR)
    {
      STRIP_NOPS (last_stmt);
      i = tsi_start (&BIND_EXPR_BODY (last_stmt));
    }
  else
    tsi_next (&i);

  succ_bb = first_exec_block (tsi_container (i));
  if (succ_bb)
    return succ_bb;

  /* We couldn't find a successor for BB.  This means that BB is the last
     block inside a control structure or lexical scope.  Use the
     NEXT_BLOCK_LINK for BB's last statement.  If NEXT_BLOCK_LINK is still
     NULL, then BB is the last basic block in the function.  In which case
     we have reached the end of the flowgraph and return EXIT_BLOCK_PTR.  */
  if (last_stmt && NEXT_BLOCK_LINK (last_stmt))
    return bb_for_stmt (NEXT_BLOCK_LINK (last_stmt));
  else
    return EXIT_BLOCK_PTR;
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

  return (TREE_CODE (t) == COND_EXPR
	  || TREE_CODE (t) == LOOP_EXPR
	  || TREE_CODE (t) == SWITCH_EXPR);
}


/* Return true if T alters the flow of control (i.e., return true if T is
   GOTO, RETURN or a call to a non-returning function.  */

bool
is_ctrl_altering_stmt (t)
     tree t;
{
  enum tree_code code;

#if defined ENABLE_CHECKING
  if (t == NULL)
    abort ();
#endif

  code = TREE_CODE (t);

  /* GOTO_EXPRs and RETURN_EXPRs always alter flow control.  */
  if (TREE_CODE (t) == GOTO_EXPR || TREE_CODE (t) == RETURN_EXPR)
    return true;

  /* A CALL_EXPR alters flow control if the current function has
     nonlocal labels.  */
  if (code == CALL_EXPR
      && FUNCTION_RECEIVES_NONLOCAL_GOTO (current_function_decl))
    return true;

  /* A CALL_EXPR also alters flow control if it does not return.  */
  if (code == CALL_EXPR)
    return call_expr_flags (t) & (ECF_NORETURN | ECF_LONGJMP);

  /* A MODIFY_EXPR may contain a CALL_EXPR, which in turn may have
     an abnormal edge if the current function has nonlocal labels.  */
  if (code == MODIFY_EXPR
      && TREE_CODE (TREE_OPERAND (t, 1)) == CALL_EXPR
      && FUNCTION_RECEIVES_NONLOCAL_GOTO (current_function_decl))
    return true;

  return false;
}


/* Return flags associated with the function called by T (see ECF_* in
   rtl.h)  */

int
call_expr_flags (t)
  tree t;
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


/* Return true if T represent a loop statement.  */

bool
is_loop_stmt (t)
     tree t;
{
  return (TREE_CODE (t) == LOOP_EXPR);
}


/* Return true if T is a computed goto.  */

bool
is_computed_goto (t)
     tree t;
{
  return (TREE_CODE (t) == GOTO_EXPR
          && TREE_CODE (GOTO_DESTINATION (t)) != LABEL_DECL);
}


/* Return true if T should start a new basic block.  PREV_T is the
   statement preceding T.  It is used when T is a label or a case label.
   Labels should only start a new basic block if their previous statement
   wasn't a label.  Otherwise, sequence of labels would generate
   unnecessary basic blocks that only contain a single label.  */

static inline bool
stmt_starts_bb_p (t, prev_t)
     tree t;
     tree prev_t;
{
  enum tree_code code = TREE_CODE (t);

  /* LABEL_EXPRs and CASE_LABEL_EXPRs start a new basic block only if the
     preceding statement wasn't a label of the same type.  This prevents
     the creation of consecutive blocks that have nothing but a single
     label.  */
  if (code == LABEL_EXPR || code == CASE_LABEL_EXPR)
    {
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
stmt_ends_bb_p (t)
     tree t;
{
  enum tree_code code = TREE_CODE (t);

  return (code == COND_EXPR
	  || code == SWITCH_EXPR
	  || code == LOOP_EXPR
	  || code == TRY_FINALLY_EXPR
	  || code == TRY_CATCH_EXPR
	  || is_ctrl_altering_stmt (t));
}


/* Remove all the blocks and edges that make up the flowgraph.  */

void
delete_tree_cfg ()
{
  if (n_basic_blocks > 0)
    free_aux_for_blocks ();

  free_basic_block_vars (0);
}


/* If BB is a latch block, return the header block controlling the loop.
   FIXME: the name of this function stinks, but I can't think of a better
   one at the moment.  */

basic_block
is_latch_block_for (bb)
     basic_block bb;
{
  edge e;

  /* BB is a latch if one of its successors is a loop entry block and BB is
     a block in that loop's body.  */
  for (e = bb->succ; e; e = e->succ_next)
    if (e->dest->flags & BB_LOOP_CONTROL_EXPR
	&& is_parent (e->dest, bb))
      return e->dest;

  return NULL;
}


/* Return a pointer to the first executable statement starting at ENTRY_P.  */

static tree *
first_exec_stmt (entry_p)
     tree *entry_p;
{
  tree_stmt_iterator i;
  tree stmt;

  for (i = tsi_start (entry_p); !tsi_end_p (i); tsi_next (&i))
    {
      stmt = tsi_stmt (i);
      if (!stmt)
        continue;

      STRIP_NOPS (stmt);

      /* Note that we actually return the container for the executable
	 statement, not the statement itself.  This is to allow the caller to
	 start iterating from this point.  */
      if (is_exec_stmt (stmt))
	return tsi_container (i);
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


/* Return the header block for the innermost switch statement containing
   BB.  Return NULL if BB is not inside a switch statement.  */

static basic_block
switch_parent (bb)
     basic_block bb;
{
  tree parent = parent_stmt (last_stmt (bb));

  while (parent && TREE_CODE (parent) != SWITCH_EXPR)
    parent = parent_stmt (parent);

  return (parent) ? bb_for_stmt (parent) : NULL;
}


/* Return the first statement in basic block BB, stripped of any NOP
   containers.  */

tree
first_stmt (bb)
     basic_block bb;
{
  block_stmt_iterator i;
  tree t;

  if (bb == NULL || bb->index < 0)
    return NULL_TREE;

  i = bsi_start (bb);

  /* Check for blocks with no remaining statements.  */
  if (bsi_end_p (i))
    return NULL_TREE;
  t = bsi_stmt (i);
  STRIP_NOPS (t);
  return t;
}


/* Return the last statement in basic block BB, stripped of any NOP
   containers.

   empty_stmt_nodes are never returned. NULL is returned if there are no
   such statements.  */

tree
last_stmt (bb)
     basic_block bb;
{
  tree_stmt_iterator i;
  block_stmt_iterator b;
  tree t;
  
  if (bb == NULL || bb->index == INVALID_BLOCK)
    return NULL_TREE;

  i = tsi_start (bb->end_tree_p);
  t = tsi_stmt (i);

  /* If the last statement is an empty_stmt_node, we have to traverse through
     the basic block until we find the last non-empty statement. ick.  */
  if (!t)
    {
      for (b = bsi_start (bb); !bsi_end_p (b); bsi_next (&b))
        t = bsi_stmt(b);
    }
  if (t)
    {
      STRIP_NOPS (t);
    }

  return t;
}


/* Return a pointer to the last statement in block BB.  */

tree *
last_stmt_ptr (bb)
     basic_block bb;
{
  block_stmt_iterator i, last;

  if (bb == NULL || bb->index == INVALID_BLOCK)
    return NULL;

  /* We can be more efficient here if we check if end_tree_p points to a
     non empty_stmt_node first.  */
     
  for (last = i = bsi_start (bb); !bsi_end_p (i); bsi_next (&i))
    last = i;

  return bsi_stmt_ptr (last);
}


/* Initialize a block stmt iterator with a container that contains stmt's
   in a specified basic block. If the first real stmt is not in the
   specified basic block, then return an empty iterator.  If the first
   real stmt is contained in a BIND_EXPR, descend into the BIND_EXPR and
   set up the context chains properly. */

static block_stmt_iterator
bsi_init (tp, bb)
     tree *tp;
     basic_block bb;
{
  block_stmt_iterator i, bind;
  tree stmt;

  i.tp = tp;
  i.context = NULL_TREE;
  /* If the first stmt is empty, get the next non-empty one.  */
  if (i.tp != NULL)
    {
      stmt = bsi_stmt (i);
      if (stmt == NULL_TREE)
	bsi_next_in_bb (&i, bb);
      else
        if (TREE_CODE (stmt) == BIND_EXPR)
	  {
	    bind = bsi_init (&BIND_EXPR_BODY (stmt), bb);

	    /* If the basic block of the child is the same as this block,
	       then add this context to the end, and use that iterator.  */
	    if (bind.tp != NULL)
	      {
		tree tmp, end;

	        tmp = build_tree_list  (NULL_TREE, (tree) i.tp);
	        if (bind.context)
		  {
		    for (end = bind.context; 
			 TREE_PURPOSE (end) != NULL;
			 end = TREE_PURPOSE (end))
		      ;
		    TREE_PURPOSE (end) = tmp;
		  }
		else
		  bind.context = tmp;
		    
		return bind;

	      }
	    else
	      /* If the children of the BIND_EXPR are no good, try the next
	         statement.  */
	      bsi_next_in_bb (&i, bb);
	  }
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
   empty_stmt_nodes inside a basic block.  */

void
bsi_next_in_bb (i, bb)
     block_stmt_iterator *i;
     basic_block bb;
{
  tree t, stmt = NULL_TREE;
  block_stmt_iterator bind;

  do
    {
      t = *(i->tp);
      STRIP_NOPS (t);
      if (TREE_CODE (t) == COMPOUND_EXPR)
	i->tp = &(TREE_OPERAND (t, 1));
      else
	i->tp = NULL;
    }
  while (i->tp && (stmt = bsi_stmt (*i)) == NULL_TREE);

  if (i->tp && bb_for_stmt (*(i->tp)) != bb) 
    i->tp = NULL;

  if (i->tp && TREE_CODE (stmt) == BIND_EXPR)
    {
      bind = bsi_init (&BIND_EXPR_BODY (stmt), bb);

      /* If the basic block of the child is the same as this block, then push 
	 this context, and add it to the end of the new iterator.  */
      if (bind.tp != NULL)
	{
	  tree tmp, end;
	  tmp = build_tree_list (i->context, (tree) i->tp);
	  if (bind.context)
	    {
	      for (end = bind.context; 
		   TREE_PURPOSE (end) != NULL;
		   end = TREE_PURPOSE (end))
		;
	      TREE_PURPOSE (end) = tmp;
	    }
	  else
	    bind.context = tmp;
	  *i = bind;
	}
	
    }

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
   statement in basic block BB which isn't an empty_stmt_node.

   NULL is returned if there are no such statements.  */

block_stmt_iterator
bsi_start (bb)
     basic_block bb;
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

  return i;
}

/* Find the previous iterator value.  */

void
bsi_prev (i)
     block_stmt_iterator *i;
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
bsi_from_tsi (ti)
     tree_stmt_iterator ti;
{
  basic_block bb;
  tree stmt;
  block_stmt_iterator bi;

  stmt = tsi_stmt (ti);
  if (stmt)
    {
      bb = bb_for_stmt (stmt);
      if (bb)
	return bsi_init (ti.tp, bb);
    }

  bi.tp = NULL;
  bi.context = NULL_TREE;
  return bi;
}

/* Insert statement T into basic block BB.  */

void
set_bb_for_stmt (t, bb)
     tree t;
     basic_block bb;
{
  stmt_ann_t ann;

  if (t == empty_stmt_node)
    return;

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

      ann = stmt_ann (t) ? stmt_ann (t) : create_stmt_ann (t);
      ann->bb = bb;
      t = (TREE_CODE (t) == COMPOUND_EXPR) ? TREE_OPERAND (t, 0) : NULL_TREE;
    }
  while (t && t != empty_stmt_node);
}
