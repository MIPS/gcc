/* {{{ Control flow functions for trees.
   Copyright (C) 2001 Free Software Foundation, Inc.
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

/* Make sure that structures shared with the RTL optimizer use trees
   instead of rtx.  */
#define USE_TREE_IL 1

#include "config.h"
#include "system.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "c-common.h"
#include "ggc.h"
#include "diagnostic.h"
#include "tree-opt.h"
#include "tree-flow.h"

/* }}} */

/* {{{ Debug macros.  */

#define DEBUG_TREE_FLOW	1	/* Force debugging on.  */

#ifdef DEBUG_TREE_FLOW
#undef DEBUG_TREE_FLOW
#define DEBUG_TREE_FLOW(level) debug_tree_flow (level, __FILE__, __LINE__)

static int debug_tree_flow PARAMS ((int, const char *, int));

static int
debug_tree_flow (level, filename, line)
    int level;
    const char *filename;
    int line;
{
  char *trigger = getenv ("DEBUG_TREE_FLOW");
  if (trigger && atoi (trigger) >= level)
    {
      if (atoi (trigger) > level)
	{
	  fputs ("\n----------------------------------------------------------------------------\n\n", stderr);
	  fprintf (stderr, "%s:%d\n", filename, line);
	}

      return 1;
    }

  return 0;

}
#endif	/* DEBUG_TREE_FLOW  */

/* }}} */

/* {{{ Local variables and data structures.  */

/* Initial capacity for the basic block array.  */

static const int initial_cfg_capacity = 20;

/* }}} */

/* {{{ Local function declarations.  */

/* Basic blocks and flowgraphs.  */
static void make_nodes PARAMS ((tree, basic_block));
static void make_compound_stmt_nodes PARAMS ((tree, basic_block));
static void make_for_stmt_nodes PARAMS ((tree, basic_block));
static void make_if_stmt_nodes PARAMS ((tree, basic_block));
static void make_while_stmt_nodes PARAMS ((tree, basic_block));
static void make_switch_stmt_nodes PARAMS ((tree, basic_block));
static void make_do_stmt_nodes PARAMS ((tree, basic_block));
static basic_block create_bb PARAMS ((tree, tree, basic_block, int));
static tree create_maximal_bb PARAMS ((tree, basic_block, int));
static void map_stmt_to_bb PARAMS ((tree, basic_block));

/* Edges.  */
static void make_edges PARAMS ((void));
static void make_ctrl_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_back_edges PARAMS ((sbitmap *, basic_block));
static void make_exit_edges PARAMS ((sbitmap *, basic_block));
static void make_fallthru_edge PARAMS ((sbitmap *, basic_block bb));
static void make_compound_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_for_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_while_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_do_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_if_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_switch_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_goto_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_break_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_continue_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_return_stmt_edges PARAMS ((sbitmap *, basic_block));

/* Various helpers.  */
static basic_block get_successor_block PARAMS ((basic_block));
static basic_block get_outermost_scope_block PARAMS ((basic_block));

/* }}} */


/* Create basic blocks.  */

/* {{{ tree_find_basic_blocks()
   
   Entry point to the CFG builder for trees.  */

void
tree_find_basic_blocks (t, nregs, file)
    tree t;
    int nregs ATTRIBUTE_UNUSED;
    FILE *file ATTRIBUTE_UNUSED;
{
  /* Flush out existing data.  */
  delete_cfg ();
      
  /* Initialize the basic block array.  */
  n_basic_blocks = 0;
  VARRAY_BB_INIT (basic_block_info, initial_cfg_capacity, "basic_block_info");

  /* Find the basic blocks for the flowgraph.  */
  make_nodes (t, NULL);

#ifdef DEBUG_TREE_FLOW
  if (DEBUG_TREE_FLOW (2))
    tree_debug_cfg ();
#endif	/* DEBUG_TREE_FLOW  */

  /* Create the edges of the flowgraph.  */
  make_edges ();

  /* Don't bother doing anything else if we found errors.  */
  if (errorcount)
    return;

  mark_critical_edges ();

#ifdef DEBUG_TREE_FLOW
  if (DEBUG_TREE_FLOW (1))
    {
      char name[1024];

      tree_debug_cfg ();
      sprintf (name, "%s.dot", 
	       IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));
      tree_cfg2dot (name);
    }
#endif	/* DEBUG_TREE_FLOW  */
}
/* }}} */

/* {{{ make_nodes()

   Build a flowgraph for the tree starting with T. Make CONTROL_PARENT
   the block dominating all the nodes in the new flowgraph.  */

static void
make_nodes (t, control_parent)
    tree t;
    basic_block control_parent;
{
  /* Traverse the statement chain building basic blocks.  */
  while (t && t != error_mark_node)
    {
      switch (TREE_CODE (t))
	{
	case COMPOUND_STMT:
	  make_compound_stmt_nodes (t, control_parent);
	  break;

	case FOR_STMT:
	  make_for_stmt_nodes (t, control_parent);
	  break;

	case IF_STMT:
	  make_if_stmt_nodes (t, control_parent);
	  break;

	case WHILE_STMT:
	  make_while_stmt_nodes (t, control_parent);
	  break;

	case SWITCH_STMT:
	  make_switch_stmt_nodes (t, control_parent);
	  break;

	case DO_STMT:
	  make_do_stmt_nodes (t, control_parent);
	  break;

	default:
	  t = create_maximal_bb (t, control_parent, 0);
	  break;
	}

      if (t)
	t = TREE_CHAIN (t);
    }
}
/* }}} */

/* {{{ make_compound_stmt_nodes()

   Create the nodes for a COMPOUND_STMT.  */

static void
make_compound_stmt_nodes (t, control_parent)
    tree t;
    basic_block control_parent;
{
  basic_block bb = create_bb (t, t, control_parent, 0);
  make_nodes (COMPOUND_BODY (t), bb);
}
/* }}} */

/* {{{ make_for_stmt_nodes()

   Create the nodes for a FOR_STMT.  */

static void
make_for_stmt_nodes (t, control_parent)
    tree t;
    basic_block control_parent;
{
  basic_block bb;

  bb = create_bb (t, t, control_parent, 1);
  create_maximal_bb (FOR_INIT_STMT (t), bb, 1);
  create_maximal_bb (FOR_COND (t), bb, 1);
  create_maximal_bb (FOR_EXPR (t), bb, 1);
  make_nodes (FOR_BODY (t), bb);
}
/* }}} */

/* {{{ make_while_stmt_nodes ()

   Create the nodes for a WHILE_STMT.  */

static void
make_while_stmt_nodes (t, control_parent)
    tree t;
    basic_block control_parent;
{
  basic_block bb = create_bb (t, t, control_parent, 1);
  create_maximal_bb (WHILE_COND (t), bb, 1);
  make_nodes (WHILE_BODY (t), bb);
}
/* }}} */

/* {{{ make_do_stmt_nodes ()

   Create the nodes for a DO_STMT.  */

static void
make_do_stmt_nodes (t, control_parent)
    tree t;
    basic_block control_parent;
{
  basic_block bb = create_bb (t, t, control_parent, 0);
  create_maximal_bb (DO_COND (t), bb, 1);
  make_nodes (DO_BODY (t), bb);
}
/* }}} */

/* {{{ make_if_stmt_nodes ()

   Create the nodes for an IF_STMT.  */

static void
make_if_stmt_nodes (t, control_parent)
    tree t;
    basic_block control_parent;
{
  basic_block bb = create_bb (t, t, control_parent, 0);
  create_maximal_bb (IF_COND (t), bb, 0);
  make_nodes (THEN_CLAUSE (t), bb);
  make_nodes (ELSE_CLAUSE (t), bb);
}
/* }}} */

/* {{{ make_switch_stmt_nodes ()

   Create the nodes for a SWITCH_STMT.  */

static void
make_switch_stmt_nodes (t, control_parent)
    tree t;
    basic_block control_parent;
{
  basic_block bb = create_bb (t, t, control_parent, 0);
  create_maximal_bb (SWITCH_COND (t), bb, 0);
  make_nodes (SWITCH_BODY (t), bb);
}
/* }}} */

/* {{{ create_maximal_bb ()

   Create a maximal basic block. A maximal basic block is a maximal
   length sequence of consecutive statements that are always executed
   together. That is, if the first statement of the block is executed,
   then all the other statements will be executed in sequence until and
   including the last one in the block. 

   Returns the last statement in the basic block.  */

static tree
create_maximal_bb (t, control_parent, is_loop_header)
    tree t;
    basic_block control_parent;
    int is_loop_header;
{
  tree first, last;
  basic_block bb;

  if (t == NULL)
    return NULL;

  first = last = t;
  bb = create_bb (first, last, control_parent, is_loop_header);

  while (last && last != error_mark_node)
    {
      map_stmt_to_bb (last, bb);

      /* If this is a control flow altering statement, store it in
	 bb->exit_stmt.  */
      if (is_ctrl_altering_stmt (last))
	{
	  /* Sanity check. Allow only one control flow altering
	     statement per basic block.  */
	  if (BB_EXIT_STMT (bb))
	    abort ();

	  get_bb_ann (bb)->exit_stmt = last;
	}

      /* If this statement is the end of the basic block, save it and
	 return.  */
      if (stmt_ends_bb_p (last))
	{
	  bb->end_tree = last;
	  break;
	}

      last = TREE_CHAIN (last);
    }

  return last;
}
/* }}} */

/* {{{ create_bb()

   Creates and returns a new basic block. HEAD and END are the first and
   last statements in the block. CONTROL_PARENT is the entry block for
   the control structure containing the new block.  Flag IS_LOOP_HEADER
   is non-zero if this block is part of a loop header.  */

static basic_block
create_bb (head, end, control_parent, is_loop_header)
    tree head;
    tree end;
    basic_block control_parent;
    int is_loop_header;
{
  basic_block bb;

  /* Create and initialize a new basic block.  */
  bb = (basic_block) ggc_alloc (sizeof (*bb));
  memset (bb, 0, sizeof (*bb));

  bb->head_tree = head;
  bb->end_tree = end;
  bb->index = n_basic_blocks;
  get_bb_ann (bb)->parent = control_parent;
  get_bb_ann (bb)->is_loop_header = is_loop_header;

  /* Grow the basic block array if needed.  */
  if ((size_t)n_basic_blocks == VARRAY_SIZE (basic_block_info))
    VARRAY_GROW (basic_block_info, n_basic_blocks + (n_basic_blocks + 3) / 4);

  /* Add the newly created block to the array.  */
  BASIC_BLOCK (n_basic_blocks) = bb;
  n_basic_blocks++;

  /* Associate the newly created block to the head and end tree.  */
  map_stmt_to_bb (head, bb);
  map_stmt_to_bb (end, bb);

  return bb;
}

/* }}} */

/* {{{ map_stmt_to_bb()

   Map statement T to basic block BB.  Note that we only do this on
   statement trees.  Other types of trees may be shared, making this
   annotation useless.  Code requesting the basic block for a tree,
   should be ready to deal with NULL responses.  */

static void
map_stmt_to_bb (t, bb)
    tree t;
    basic_block bb;
{
  tree_ann ann;

  if (t == NULL || !statement_code_p (TREE_CODE (t)))
    return;
  
  ann = get_tree_ann (t);
  ann->bb = bb;
}
/* }}} */

/* {{{ get_bb_ann()

   Get the annotation for the given block. Create a new one if
   necessary.  */

basic_block_ann
get_bb_ann (bb)
    basic_block bb;
{
  basic_block_ann ann = BB_ANN (bb);

  if (ann == NULL)
    {
      ann = (basic_block_ann) ggc_alloc (sizeof (*ann));
      memset ((void *)ann, 0, sizeof (*ann));

      bb->aux = (void *)ann;
    }

  return ann;
}
/* }}} */


/* Create edges.  */

/* {{{ make_edges()

   Join all the blocks in the flowgraph.  */

static void
make_edges ()
{
  int i;
  sbitmap *edge_cache;

  edge_cache = sbitmap_vector_alloc (n_basic_blocks, n_basic_blocks);
  sbitmap_vector_zero (edge_cache, n_basic_blocks);

  make_edge (edge_cache, ENTRY_BLOCK_PTR, BASIC_BLOCK (0), EDGE_FALLTHRU);

  /* Traverse basic block array placing edges. Note that order is
     important, here.  In particular, exit edges take precedence over
     back edges (think of a return statement at the end of a loop).  */
  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);

      /* Edges for control statements.  */
      if (is_ctrl_stmt (bb->head_tree))
	make_ctrl_stmt_edges (edge_cache, bb);

      /* Edges for control flow altering statements (goto, break,
	 continue, return) need an edge to the corresponding target
	 block.  */
      if (BB_EXIT_STMT (bb))
	make_exit_edges (edge_cache, bb);

      /* Back edges for the last block inside a loop construct.  */
      if (is_last_block_of_loop (bb))
	make_back_edges (edge_cache, bb);

      /* Finally, if no edges were created above, this is a regular
	 basic block that only needs a fallthru edge.  */
      if (bb->succ == NULL)
	make_fallthru_edge (edge_cache, bb);
    }


  /* Reachability analysis.  Add fake edges to blocks with no
     predecessors and give a warning if -Wunreachable-code is on.

     FIXME: This is gross. We should not be adding fake edges.  But we
     need them to avoid problems during DFA and SSA.  */

  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);
      tree head = bb->head_tree;
      if (bb->pred == NULL)
	{
	  basic_block parent;

	  parent = (BB_PARENT (bb)) ? BB_PARENT (bb) : ENTRY_BLOCK_PTR;
	  make_edge (edge_cache, parent, bb, EDGE_FAKE);

	  if (warn_notreached)
	    {
	      while (!statement_code_p (TREE_CODE (head)))
		head = (BB_PARENT (bb))->head_tree;

	      prep_stmt (head);
	      warning ("unreachable statement");
	    }
	}
    }

  if (edge_cache)
    sbitmap_vector_free (edge_cache);
}
/* }}} */

/* {{{ make_ctrl_stmt_edges()

   Create edges for control statement at basic block BB.  */

static void
make_ctrl_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  switch (TREE_CODE (bb->head_tree))
    {
    case COMPOUND_STMT:
      make_compound_stmt_edges (edge_cache, bb);
      break;

    case FOR_STMT:
      make_for_stmt_edges (edge_cache, bb);
      break;

    case WHILE_STMT:
      make_while_stmt_edges (edge_cache, bb);
      break;

    case DO_STMT:
      make_do_stmt_edges (edge_cache, bb);
      break;

    case IF_STMT:
      make_if_stmt_edges (edge_cache, bb);
      break;

    case SWITCH_STMT:
      make_switch_stmt_edges (edge_cache, bb);
      break;

    default:
      abort ();
    }
}

/* }}} */

/* {{{ make_back_edges()

   Create back edges from BB to its loop controlling node(s).  If BB is
   the last block in a series of nested loops, this function will create
   back edges to all the enclosing loop headers.
   
   Note: This function assumes that BB is in fact the last block of a
   loop (see is_last_block_of_loop()).  */

static void
make_back_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  basic_block cond_bb, loop_bb;

  /* Find the innermost loop block containing BB.  */
  loop_bb = find_loop_parent (bb);

  /* Sanity check.  If BB is not inside a loop, something is wrong.  */
  if (loop_bb == NULL || !is_loop_stmt (loop_bb->head_tree))
    abort ();

  /* Since BB may be contained in more than one loop, we keep going up
     in the loop nesting structure until we find no more.  */
  while (loop_bb)
    {
      cond_bb = get_condition_block (loop_bb);
      make_edge (edge_cache, bb, cond_bb, 0);

      /* See if our loop parent is itself inside a loop.  */ 
      loop_bb = find_loop_parent (loop_bb);
    }
}

/* }}} */

/* {{{ make_exit_edges()

   Create exit edges for statements that alter the flow of control
   (BREAK, CONTINUE, GOTO, RETURN).  */

static void
make_exit_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  switch (TREE_CODE (BB_EXIT_STMT (bb)))
    {
    case BREAK_STMT:
      make_break_stmt_edges (edge_cache, bb);
      break;

    case CONTINUE_STMT:
      make_continue_stmt_edges (edge_cache, bb);
      break;

    case GOTO_STMT:
      make_goto_stmt_edges (edge_cache, bb);
      break;

    case RETURN_STMT:
      make_return_stmt_edges (edge_cache, bb);
      break;

    default:
      abort ();
    }
}

/* }}} */

/* {{{ make_fallthru_edge()

   Create a fallthru edge to the block holding the successor of the last
   tree in the block, if needed.  Ignore blocks that already have
   successors set up (e.g., nodes in control statements).  */

static void
make_fallthru_edge (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree chain = TREE_CHAIN (bb->end_tree);
  basic_block control_parent = BB_PARENT (bb);

  /* Do nothing if successors have been set already.  */

  if (bb->succ)
    return;


  /* If chain is NULL, maybe this is the last block inside a compound
     statement. Try going up in the chain of control parents until we
     find the topmost scope or a tree with a non-null chain.  */

  while (chain == NULL && control_parent)
    {
      chain = TREE_CHAIN (control_parent->head_tree);
      control_parent = BB_PARENT (control_parent);
    }
    

  /* If chain is still NULL, this must be the last block in the
     function. Add an edge to the exit block.  */

  if (chain == NULL)
    make_edge (edge_cache, bb, EXIT_BLOCK_PTR, EDGE_FALLTHRU);
  else
    make_edge (edge_cache, bb, BB_FOR_STMT (chain), EDGE_FALLTHRU);
}

/* }}} */

/* {{{ make_compound_stmt_edges()

   Create edges for a COMPOUND_STMT structure that starts at basic block bb.  */

static void
make_compound_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree entry = bb->head_tree;
  basic_block body_bb;

  if (TREE_CODE (entry) != COMPOUND_STMT)
    abort ();

  body_bb = (COMPOUND_BODY (entry)) ? BASIC_BLOCK (bb->index + 1) : NULL;

  /* Create the following edges.

	     COMPOUND_STMT
	          |
		  v
	     COMPOUND_BODY  */

  if (body_bb)
    make_edge (edge_cache, bb, body_bb, 0);
}
/* }}} */

/* {{{ make_for_stmt_edges()

   Create edges for a FOR_STMT structure that starts at basic block BB.  */

static void
make_for_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree entry = bb->head_tree;
  int index;
  basic_block init_bb, cond_bb, expr_bb, body_bb;

  if (TREE_CODE (entry) != FOR_STMT)
    abort ();

  /* Basic blocks for each component. Note that the block numbers are
     determined by make_for_stmt_nodes().  */
  index = bb->index + 1;
  init_bb = (FOR_INIT_STMT (entry)) ? BASIC_BLOCK (index++) : NULL;
  cond_bb = (FOR_COND (entry))      ? BASIC_BLOCK (index++) : NULL;
  expr_bb = (FOR_EXPR (entry))      ? BASIC_BLOCK (index++) : NULL;
  body_bb = (FOR_BODY (entry))      ? BASIC_BLOCK (index++) : NULL;

  /* Create the following edges.

     		FOR_STMT
		   |
		   v
		FOR_INIT
		   |
                   v
            +-- FOR_COND <----------+
            |      |                |
            |      |             FOR_EXPR
            |      |
            |      v
            |   FOR_BODY
            |
	    |
            +--> EXIT
   
     - If the loop does not have a condition expression, edges involving
       the condition block, go to the loop header ("infinite" loops).

     - If the loop does not have an expression block, we replace it with
       the condition block.
       
     - Similarly, if the body is empty, we replace it with the
       expression block. Hence, loops with neither component will reduce
       to the header block with a self-referencing edge.  */

  if (init_bb == NULL)
    init_bb = bb;

  if (cond_bb == NULL) 
    cond_bb = init_bb;

  if (expr_bb == NULL) 
    expr_bb = cond_bb;

  if (body_bb == NULL) 
    body_bb = expr_bb;

  make_edge (edge_cache, bb, init_bb, 0);
  make_edge (edge_cache, init_bb, cond_bb, 0);
  make_edge (edge_cache, cond_bb, body_bb, 0);
  make_edge (edge_cache, expr_bb, cond_bb, 0);
  make_edge (edge_cache, cond_bb, get_successor_block (bb), 0);
}
/* }}} */

/* {{{ make_while_stmt_edges()

   Create the edges for a WHILE_STMT structure starting with bb.  */

static void
make_while_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree entry = bb->head_tree;
  basic_block cond_bb, body_bb;
  int index;

  if (TREE_CODE (entry) != WHILE_STMT)
    abort ();

  /* Basic blocks for each component. Note that the block numbers are
     determined by make_for_stmt_nodes().  */
  index = bb->index + 1;
  cond_bb = (WHILE_COND (entry)) ? BASIC_BLOCK (index++) : NULL;
  body_bb = (WHILE_BODY (entry)) ? BASIC_BLOCK (index++) : NULL;

  /* Create the following edges. The other edges will be naturally created
     by the main loop in create_edges().

             WHILE_STMT
	         |
		 v
          +- WHILE_COND
	  |      |
          |      v
          |  WHILE_BODY
	  |
	  |
	  +--> EXIT
          
     If the body doesn't exist, we use the header instead.  */

  if (cond_bb == NULL)
    cond_bb = bb;

  if (body_bb == NULL)
    body_bb = cond_bb;

  make_edge (edge_cache, bb, cond_bb, 0);
  make_edge (edge_cache, cond_bb, body_bb, 0);
  make_edge (edge_cache, cond_bb, get_successor_block (bb), 0);
}
/* }}} */

/* {{{ make_do_stmt_edges()

   Create the edges for a DO_STMT structure starting with bb.  */

static void
make_do_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree entry = bb->head_tree;
  basic_block cond_bb, body_bb;
  int index;

  if (TREE_CODE (entry) != DO_STMT)
    abort ();

  /* Basic blocks for each component. Note that the block numbers are
     determined by make_for_stmt_nodes().  */
  index = bb->index + 1;
  cond_bb = (DO_COND (entry)) ? BASIC_BLOCK (index++) : NULL;
  body_bb = (DO_BODY (entry)) ? BASIC_BLOCK (index++) : NULL;

  /* Create the following edges.

            DO_STMT
	       |
	       v
            DO_BODY <-+
                      |
                      |
	    DO_COND --+
	       |
	       v
	     EXIT

     If the body doesn't exist, we use the condition instead.  */

  if (cond_bb == NULL)
    cond_bb = bb;

  if (body_bb == NULL)
    body_bb = cond_bb;

  make_edge (edge_cache, bb, body_bb, 0);
  make_edge (edge_cache, cond_bb, body_bb, 0);
  make_edge (edge_cache, cond_bb, get_successor_block (bb), 0);
}
/* }}} */

/* {{{ make_if_stmt_edges()

   Create the edges for an IF_STMT structure starting with BB.  */

static void
make_if_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree entry = bb->head_tree;
  basic_block cond_bb, then_bb, else_bb;
  int index;

  if (TREE_CODE (entry) != IF_STMT)
    abort ();
 
  /* Basic blocks for each component.  */
  index = bb->index + 1;
  cond_bb = BASIC_BLOCK (bb->index + 1);
  then_bb = (THEN_CLAUSE (entry)) ? BB_FOR_STMT (THEN_CLAUSE (entry)): NULL;
  else_bb = (ELSE_CLAUSE (entry)) ? BB_FOR_STMT (ELSE_CLAUSE (entry)): NULL;

  /* Create the following edges.

	      IF_STMT
	         |
		 v
	      IF_COND
		/ \
	       /   \
	    THEN   ELSE

     Either clause may be empty.  */

  make_edge (edge_cache, bb, cond_bb, 0);

  if (then_bb)
    make_edge (edge_cache, cond_bb, then_bb, 0);

  if (else_bb)
    make_edge (edge_cache, cond_bb, else_bb, 0);

  /* We only need an edge to the exit block if one of the clauses is
     missing.  */
  if (then_bb == NULL || else_bb == NULL)
    make_edge (edge_cache, cond_bb, get_successor_block (bb), 0);
}
/* }}} */

/* {{{ make_switch_stmt_edges()

   Create edges for a SWITCH_STMT structure starting at block bb.  */

static void
make_switch_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree t, body;
  tree entry = bb->head_tree;
  basic_block cond_bb;

  if (TREE_CODE (entry) != SWITCH_STMT)
    abort ();

  /* Create the following edges.

		      SWITCH_STMT
		           |
			   v
		      SWITCH_COND
		         / | \
		        /  |  \
                       L1 ... Ln  */

  cond_bb = BASIC_BLOCK (bb->index + 1);
  make_edge (edge_cache, bb, cond_bb, 0);

  /* Make an edge to each case label in the body.  The first label is
     treated differently.  Instead of creating an edge to the first
     label, create an edge to the COMPOUND_STMT containing all the
     labels.  This prevents the blocks for the COMPOUND_STMT and
     SCOPE_STMT to be left disconnected from the graph.  */

  body = SWITCH_BODY (entry);
  if (TREE_CODE (body) != COMPOUND_STMT)
    abort ();

  make_edge (edge_cache, cond_bb, BB_FOR_STMT (body), 0);

  body = COMPOUND_BODY (body);
  while (body && TREE_CODE (body) != CASE_LABEL)
    body = TREE_CHAIN (body);

  if (body == NULL)
    abort ();

  for (t = TREE_CHAIN (body); t; t = TREE_CHAIN (t))
    if (TREE_CODE (t) == CASE_LABEL)
      make_edge (edge_cache, cond_bb, BB_FOR_STMT (t), 0);
}

/* }}} */

/* {{{ make_goto_stmt_edges()

   Create edges for a goto statement.  */

static void
make_goto_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree goto_t, dest;
  int i;
  
  goto_t = BB_EXIT_STMT (bb);
  if (goto_t == NULL || TREE_CODE (goto_t) != GOTO_STMT)
    abort ();

  /* If the GOTO statement is followed by a scope statement, make the
     edge out of the scope statement.  Otherwise, the block for the
     scope statement will be disconnected from the graph.  */
  bb = get_outermost_scope_block (bb);
  dest = GOTO_DESTINATION (goto_t);

  /* Look for the block starting with the destination label.  In the
     case of a computed goto, make an edge to any label node we find
     in the CFG.  */
  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block target_bb = BASIC_BLOCK (i);
      tree target = target_bb->head_tree;

      /* Common case, destination is a single label.  Make the edge
	 and leave.  */
      if (TREE_CODE (dest) == LABEL_DECL
	  && TREE_CODE (target) == LABEL_STMT
	  && LABEL_STMT_LABEL (target) == dest)
	{
	  make_edge (edge_cache, bb, target_bb, 0);
	  break;
	}

      /* Computed GOTOs.  Make an edge to every label node.  */
      else if (TREE_CODE (dest) != LABEL_DECL
	       && TREE_CODE (target) == LABEL_STMT)
	make_edge (edge_cache, bb, target_bb, 0);
    }
}

/* }}} */

/* {{{ make_break_stmt_edges()

   A break statement creates an edge from the break node to the successor
   node for the break statement's control parent.  */

static void
make_break_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree break_t;
  basic_block control_parent;

  break_t = BB_EXIT_STMT (bb);
  if (break_t == NULL || TREE_CODE (break_t) != BREAK_STMT)
    abort ();

  /* A break statement *must* have an enclosing control structure.  */
  control_parent = BB_PARENT (bb);
  if (control_parent == NULL)
    abort ();

  /* Look for the innermost containing WHILE, FOR, DO or SWITCH statement.  */
  while (control_parent
         && !is_loop_stmt (control_parent->head_tree)
	 && TREE_CODE (control_parent->head_tree) != SWITCH_STMT)
    control_parent = BB_PARENT (control_parent);

  if (control_parent == NULL)
    {
      prep_stmt (BB_EXIT_STMT (bb));
      error ("break statement not within loop or switch.");
      return;
    }

  /* If the BREAK statement is followed by a scope statement, make the
     edge out of the scope statement.  Otherwise, the block for the
     scope statement will be disconnected from the graph.  */
  bb = get_outermost_scope_block (bb);

  /* We might need to make a back-edge (or edges) if the control
     parent's block is the last block of a loop.  Otherwise create an
     edge to the successor block of BB's control parent.  */
  if (is_last_block_of_loop (control_parent))
    make_back_edges (edge_cache, bb);
  else
    make_edge (edge_cache, bb, get_successor_block (control_parent), 0);
}
/* }}} */

/* {{{ make_continue_stmt_edges()

   A continue statement creates an edge from the continue node to the
   control parent's expression node.  */

static void
make_continue_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree continue_t;
  int index;
  basic_block loop_bb;

  continue_t = BB_EXIT_STMT (bb);
  if (continue_t == NULL || TREE_CODE (continue_t) != CONTINUE_STMT)
    abort ();
  
  /* A continue statement *must* have an enclosing control structure.  */
  loop_bb = BB_PARENT (bb);
  if (loop_bb == NULL)
    abort ();

  /* If the CONTINUE statement is followed by a scope statement, make the
     edge out of the scope statement.  Otherwise, the block for the
     scope statement will be disconnected from the graph.  */
  bb = get_outermost_scope_block (bb);

  /* Look for the innermost containing WHILE, FOR or DO structure.  */
  while (!is_loop_stmt (loop_bb->head_tree))
    loop_bb = BB_PARENT (loop_bb);

  index = loop_bb->index + 1;
  if (TREE_CODE (loop_bb->head_tree) == FOR_STMT)
    {
      /* Usually, the loop expression in a FOR_STMT will be the third
	 node after the loop entry block. However, a FOR_STMT may be
	 missing both the INIT and COND expressions.  */
      if (FOR_INIT_STMT (loop_bb->head_tree))
	index++;

      if (FOR_COND (loop_bb->head_tree))
	index++;
    }

  make_edge (edge_cache, bb, BASIC_BLOCK (index), 0);
}

/* }}} */

/* {{{ make_return_stmt_edges()

   Create an edge from the RETURN block to EXIT_BLOCK_PTR.  */

static void
make_return_stmt_edges (edge_cache, bb)
    sbitmap *edge_cache;
    basic_block bb;
{
  tree return_t = BB_EXIT_STMT (bb);

  if (return_t == NULL || TREE_CODE (return_t) != RETURN_STMT)
    abort ();

  /* If the RETURN statement is followed by a scope statement, make the
     edge out of the scope statement.  Otherwise, the block for the
     scope statement will be disconnected from the graph.  */
  bb = get_outermost_scope_block (bb);

  make_edge (edge_cache, bb, EXIT_BLOCK_PTR, 0);
}

/* }}} */


/* Helper functions and predicates.  */

/* {{{ get_successor_block()

   Return the successor block for BB. If the block has no successors we try
   the enclosing control structure until we find one. If we reached nesting
   level 0, return the exit block.  */

static basic_block
get_successor_block (bb)
    basic_block bb;
{
  basic_block parent_bb;

  if (bb == NULL)
    abort ();

  /* Common case.  Try to return the basic block of the last statement's
     successor.  */
  if (TREE_CHAIN (bb->end_tree))
    return BB_FOR_STMT (TREE_CHAIN (bb->end_tree));
  
  /* Walk up the control structure to see if our parent has a successor.
     Iterate until we find one or we reach nesting level 0.  */
  parent_bb = BB_PARENT (bb);

  while (parent_bb && TREE_CHAIN (parent_bb->head_tree) == NULL)
    parent_bb = BB_PARENT (parent_bb);

  /* If we found an enclosing control statement with a successor, return
     the successor's basic block. Otherwise, we must be at the top of
     the control hierarchy. In that case, return the exit block.  */
  if (parent_bb)
    return (BB_FOR_STMT (TREE_CHAIN (parent_bb->head_tree)));
  else
    return EXIT_BLOCK_PTR;
}
/* }}} */

/* {{{ get_outermost_scope_block()

   Given a basic block BB, find the outermost enclosing scope. If no scope
   statements are found, return BB.  */

static basic_block
get_outermost_scope_block (bb)
    basic_block bb;
{
  basic_block parent_bb, outer_bb;

  if (bb == NULL)
    abort ();

  /* Traverse the BB_PARENT chain looking for the outermost COMPOUND_STMT.
     This finds the outermost scope in a set of nested scopes.
     (e.g., '{{ a = 4; }}').  */
  outer_bb = bb;
  bb = get_successor_block (bb);
  while (bb && TREE_CODE (bb->head_tree) == SCOPE_STMT)
    {
      outer_bb = bb;

      /* We stop when BB's parent's parent is not a COMPOUND_STMT.  */
      parent_bb = BB_PARENT (bb);
      if (parent_bb) parent_bb = BB_PARENT (parent_bb);

      if (parent_bb == NULL
	  || TREE_CODE (parent_bb->head_tree) != COMPOUND_STMT)
	break;

      bb = get_successor_block (bb);
    }

  return outer_bb;
}

/* }}} */

/* {{{ is_last_block_of_loop()

   Returns 1 if BB is the last block of a loop body.  */

int
is_last_block_of_loop (bb)
    basic_block bb;
{
  basic_block control_parent;

  if (bb == NULL)
    abort ();

  /* A block BB is the last block in a loop construct if it complies with
     all of the following:

     1- TREE_CHAIN (BB->END_TREE) is NULL.

     2- BB is not one of the loop header nodes.

     3- BB->HEAD_TREE is not a compound statement.

     4- BB is not a top-level block.

     5- BB's control parent node is the entry block for a loop
	construct. Or, if BB's control parent is a COMPOUND_STMT, the
	compound statement's parent must be a loop construct.  */


  /* Condition 1.  */
  if (TREE_CHAIN (bb->end_tree))
    return 0;

  /* Condition 2.  */
  if (BB_IS_LOOP_HEADER (bb))
    return 0;

  /* Condition 3.  */
  if (TREE_CODE (bb->head_tree) == COMPOUND_STMT)
    return 0;

  /* Condition 4.  */
  control_parent = BB_PARENT (bb);
  if (control_parent == NULL)
    return 0;

  /* Condition 5.  */
  if (TREE_CODE (control_parent->head_tree) == COMPOUND_STMT)
    control_parent = BB_PARENT (control_parent);

  if (control_parent == NULL
      || !is_loop_stmt (control_parent->head_tree))
    return 0;

  return 1;
}

/* }}} */

/* {{{ is_ctrl_stmt()

   Return 1 if T represents a control statement.  */

int
is_ctrl_stmt (t)
    tree t;
{
  if (t == NULL)
    abort ();

  return (TREE_CODE (t) == FOR_STMT
	  || TREE_CODE (t) == IF_STMT
	  || TREE_CODE (t) == WHILE_STMT
	  || TREE_CODE (t) == SWITCH_STMT
	  || TREE_CODE (t) == DO_STMT
	  || TREE_CODE (t) == COMPOUND_STMT);
}
/* }}} */

/* {{{ is_ctrl_altering_stmt()

   Returns 1 if T alters the flow of control (i.e., T is BREAK, GOTO,
   CONTINUE or RETURN)  */

int
is_ctrl_altering_stmt (t)
    tree t;
{
  if (t == NULL)
    abort ();

  return (TREE_CODE (t) == GOTO_STMT
	  || TREE_CODE (t) == CONTINUE_STMT
	  || TREE_CODE (t) == BREAK_STMT
	  || TREE_CODE (t) == RETURN_STMT);
}

/* }}} */

/* {{{ is_loop_stmt()

   Return 1 if T represent a loop statement.  */

int
is_loop_stmt (t)
    tree t;
{
  if (t == NULL)
    abort ();

  return (TREE_CODE (t) == FOR_STMT
	  || TREE_CODE (t) == WHILE_STMT
	  || TREE_CODE (t) == DO_STMT);
}
/* }}} */

/* {{{ stmt_starts_bb_p()

   Return 1 if the given tree should start a new basic block.  */

int
stmt_starts_bb_p (t)
    tree t;
{
  if (t == NULL)
    abort ();

  return (TREE_CODE (t) == CASE_LABEL
	  || TREE_CODE (t) == LABEL_STMT
	  || TREE_CODE (t) == RETURN_STMT
	  || is_ctrl_stmt (t));
}
/* }}} */

/* {{{ stmt_ends_bb_p()

   Return 1 if the given tree T should be the last in a basic block.
   T ends a basic block if T's successor starts a new one.  */

int
stmt_ends_bb_p (t)
    tree t;
{
  /* Sanity check.  */

  if (t == NULL)
    abort ();

  if (is_ctrl_altering_stmt (t)
      || TREE_CHAIN (t) == NULL
      || stmt_starts_bb_p (TREE_CHAIN (t)))
    return 1;

  return 0;
}
/* }}} */

/* {{{ delete_cfg()

   Remove all the nodes and edges that make up the flowgraph.  */

void
delete_cfg ()
{
  int i;

  if (basic_block_info == NULL)
    return;

  clear_edges ();

  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block_ann ann = BB_ANN (BASIC_BLOCK (i));
      ann->parent = NULL;
      ann->refs = NULL;
      BASIC_BLOCK (i)->aux = NULL;
    }

  VARRAY_FREE (basic_block_info);
}
/* }}} */

/* {{{ find_loop_parent()

   Returns the header block for the innermost loop containing BB.  It
   returns NULL if BB is not inside a loop.  */

basic_block
find_loop_parent (bb)
    basic_block bb;
{
  do
    bb = BB_PARENT (bb);
  while (bb && !is_loop_stmt (bb->head_tree));

  return bb;
}

/* }}} */

/* {{{ get_condition_block()

   Returns the block containing the condition expression controlling the
   loop starting at LOOP_BB (i.e., the block containing DO_COND or
   WHILE_COND or FOR_COND).

   Since these nodes hold expressions, not statements, we cannot use
   BB_FOR_STMT() to determine their basic block.  Instead, we count from
   the basic block for the loop entry.
   
   ??? Note that this relies heavily on the order in which basic blocks
       get constructed, but I couldn't find another way of dealing with
       this.  */

basic_block
get_condition_block (loop_bb)
    basic_block loop_bb;
{
  basic_block cond_bb;
  int index;

  /* Sanity check.  This is only valid on loop header nodes.  */
  if (TREE_CODE (loop_bb->head_tree) != FOR_STMT
      && TREE_CODE (loop_bb->head_tree) != WHILE_STMT
      && TREE_CODE (loop_bb->head_tree) != DO_STMT)
    abort ();

  index = loop_bb->index + 1;
  if (TREE_CODE (loop_bb->head_tree) == FOR_STMT)
    {
      /* Usually, the loop expression in a FOR_STMT will be the third
	 node after the loop entry block. However, a FOR_STMT may be
	 missing both the INIT and COND expressions.  */
      if (FOR_INIT_STMT (loop_bb->head_tree))
	index++;

      if (FOR_COND (loop_bb->head_tree))
	index++;
    }

  cond_bb = BASIC_BLOCK (index);

  return cond_bb;
}

/* }}} */


/* Debugging functions.  */

/* {{{ tree_dump_bb

   Dump a basic block to a file.  */

void
tree_dump_bb (outf, prefix, bb, indent)
    FILE *outf;
    const char *prefix;
    basic_block bb;
    int indent;
{
  edge e;
  tree head = bb->head_tree;
  tree end = bb->end_tree;
  char *s_indent;
  int lineno;
  
  s_indent = (char *) alloca ((size_t)indent + 1);
  memset ((void *)s_indent, ' ', (size_t)indent);
  s_indent[indent] = '\0';

  fprintf (outf, "%s%sBasic block %d\n", s_indent, prefix, bb->index);

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
      lineno = (statement_code_p (TREE_CODE (head))) ? STMT_LINENO (head) : -1;
      print_node_brief (outf, "", head, 0);
      fprintf (outf, " (line: %d)\n", lineno);
    }
  else
    fputs ("nil\n", outf);

  fprintf (outf, "%s%sEnd: ", s_indent, prefix);
  if (end)
    {
      lineno = (statement_code_p (TREE_CODE (end))) ? STMT_LINENO (end) : -1;
      print_node_brief (outf, "", end, 0);
      fprintf (outf, " (line: %d)\n", lineno);
    }
  else
    fputs ("nil\n", outf);

  fprintf (outf, "%s%sParent block: ", s_indent, prefix);
  if (BB_PARENT (bb))
    fprintf (outf, "%d\n", BB_PARENT (bb)->index);
  else
    fputs ("nil\n", outf);
}
/* }}} */

/* {{{ tree_debug_bb()

   Dump a basic block on stderr.  */

void
tree_debug_bb (bb)
    basic_block bb;
{
  tree_dump_bb (stderr, "", bb, 0);
}
/* }}} */

/* {{{ tree_debug_cfg()

   Dump the CFG on stderr.  */

void
tree_debug_cfg ()
{
  int i;

  fputc ('\n', stderr);
  fprintf (stderr, ";; Function %s\n\n",
      IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

  fprintf (stderr, "\n%d basic blocks, %d edges.\n", n_basic_blocks, n_edges);
  for (i = 0; i < n_basic_blocks; i++)
    {
      tree_debug_bb (BASIC_BLOCK (i));
      fputc ('\n', stderr);
    }
}
/* }}} */

/* {{{ tree_cfg2dot()

   Write a .dot file for the flowgraph.  */

void
tree_cfg2dot (fname)
    char *fname;
{
  int i;
  FILE *f;
  edge e;
  
  f = fopen (fname, "w");
  if (f == NULL)
    abort ();

  /* Write the file header.  */
  fputs ("digraph cfg {\n", f);

  /* Write blocks and edges.  */
  for (e = ENTRY_BLOCK_PTR->succ; e; e = e->succ_next)
    {
      if (e->flags & EDGE_FAKE)
	continue;

      fprintf (f, "\tENTRY -> %d;\n", e->dest->index);
    }
  fputc ('\n', f);

  for (i = 0; i < n_basic_blocks; i++)
    {
      enum tree_code head_code, end_code;
      const char *head_name, *end_name;
      int head_line, end_line;
      basic_block bb;
      
      bb = BASIC_BLOCK (i);

      head_code = TREE_CODE (bb->head_tree);
      end_code = TREE_CODE (bb->end_tree);

      head_name = tree_code_name[head_code];
      end_name = tree_code_name[end_code];

      head_line = (statement_code_p (head_code)) 
		  ? STMT_LINENO (bb->head_tree)
		  : -1;
      end_line = (statement_code_p (end_code))
		 ? STMT_LINENO (bb->end_tree)
		 : -1;

      fprintf (f, "\t%d [label=\"#%d\\n%s (%d)\\n%s (%d)\"];\n", 
	       bb->index, bb->index, head_name, head_line, end_name, end_line);
      
      for (e = bb->succ; e; e = e->succ_next)
	{
	  if (e->flags & EDGE_FAKE)
	    continue;

	  if (e->dest == EXIT_BLOCK_PTR)
	    fprintf (f, "\t%d -> EXIT;\n", bb->index);
	  else
	    fprintf (f, "\t%d -> %d;\n", bb->index, e->dest->index);
	}

      if (i < n_basic_blocks - 1)
	fputc ('\n', f);
    }

  fputc ('}', f);
  fclose (f);
}

/* }}} */
