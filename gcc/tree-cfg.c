/* Control flow functions for trees.
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
#include "c-common.h"
#include "c-tree.h"
#include "ggc.h"
#include "diagnostic.h"
#include "tree-optimize.h"
#include "tree-flow.h"

/* Local declarations.  */

/* Initial capacity for the basic block array.  */
static const int initial_cfg_capacity = 20;

/* Stack of binding scopes.  */
static varray_type binding_stack;

/* Dump files and flags.  */
static FILE *dump_file;
static int dump_flags;

/* Basic blocks and flowgraphs.  */
static void make_blocks PARAMS ((tree, basic_block, tree, tree *));
static void make_for_stmt_blocks PARAMS ((tree, basic_block, tree, tree *));
static void make_if_stmt_blocks PARAMS ((tree, basic_block, tree, tree *));
static void make_while_stmt_blocks PARAMS ((tree, basic_block, tree, tree *));
static void make_switch_stmt_blocks PARAMS ((tree, basic_block, tree, tree *));
static void make_do_stmt_blocks PARAMS ((tree, basic_block, tree, tree *));
static basic_block create_bb PARAMS ((tree, tree, basic_block, tree *,
                                      basic_block));
static basic_block create_maximal_bb PARAMS ((tree, basic_block, tree, tree *));
static void map_stmt_to_bb PARAMS ((tree, basic_block));
static void delete_unreachable_blocks PARAMS ((void));
static void delete_bb PARAMS ((basic_block));

/* Edges.  */
static void make_edges PARAMS ((void));
static void make_ctrl_stmt_edges PARAMS ((basic_block));
static void make_exit_edges PARAMS ((basic_block));
static void make_for_stmt_edges PARAMS ((basic_block));
static void make_while_stmt_edges PARAMS ((basic_block));
static void make_do_stmt_edges PARAMS ((basic_block));
static void make_if_stmt_edges PARAMS ((basic_block));
static void make_goto_stmt_edges PARAMS ((basic_block));
static void make_break_stmt_edges PARAMS ((basic_block));
static void make_continue_stmt_edges PARAMS ((basic_block));

/* Various helpers.  */
static basic_block successor_block PARAMS ((basic_block));
static int block_invalidates_loop PARAMS ((basic_block, struct loop *));
static void create_bb_ann PARAMS ((basic_block));
static void remove_bb_ann PARAMS ((basic_block));

static void insert_before_ctrl_stmt PARAMS ((tree, tree, basic_block));
static void insert_before_normal_stmt PARAMS ((tree, tree, basic_block));
static void insert_after_ctrl_stmt PARAMS ((tree, basic_block));
static void insert_after_normal_stmt PARAMS ((tree, tree, basic_block));
static void insert_after_loop_body PARAMS ((tree, basic_block));


/* Create basic blocks.  */

/* {{{ tree_find_basic_blocks()
   
   Entry point to the CFG builder for trees.  */

void
tree_find_basic_blocks (t)
     tree t;
{
  /* Initialize the basic block array.  */
  n_basic_blocks = 0;
  VARRAY_BB_INIT (basic_block_info, initial_cfg_capacity, "basic_block_info");

  /* Create annotations for ENTRY_BLOCK_PTR and EXIT_BLOCK_PTR.  */
  create_bb_ann (ENTRY_BLOCK_PTR);
  create_bb_ann (EXIT_BLOCK_PTR);

  /* Initialize the stack of binding scopes.  */
  VARRAY_BB_INIT (binding_stack, 5, "binding_stack");

  /* Find the basic blocks for the flowgraph.  */
  make_blocks (t, NULL, NULL, NULL);

  if (n_basic_blocks > 0)
    {
      /* Adjust the size of the array.  */
      VARRAY_GROW (basic_block_info, n_basic_blocks);

      /* Create the edges of the flowgraph.  */
      make_edges ();

      /* Write the flowgraph to a GraphViz file.  */
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
	  tree_dump_cfg (dump_file);
	  dump_end (TDI_cfg, dump_file);
	}
    }
}

/* }}} */

/* {{{ make_blocks()

   Build a flowgraph for the tree starting with T.
   
   CONTROL_PARENT is the header block for the control structure immediately
      enclosing the new sub-graph.

   COMPOUND_STMT is the immediately enclosing compound statement to which T
      belongs.  These statements are not represented in the flowgraph, but
      are important to determine successor basic blocks in successor_block().

   PREV_CHAIN_P is the address into the tree preceeding T that contains a
      pointer to T.  This is used when we need to insert statements before
      the first tree of the block.

   When creating basic blocks one important property should be maintained:
   It must be possible to traverse all the trees inside a basic block by
   following the TREE_CHAIN from bb->head_tree.  */

static void
make_blocks (t, control_parent, compound_stmt, prev_chain_p)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
     tree *prev_chain_p;
{
  /* Traverse the statement chain building basic blocks.  */
  while (t && t != error_mark_node)
    {
      get_tree_ann (t)->compound_stmt = compound_stmt;

      switch (TREE_CODE (t))
	{
	case COMPOUND_STMT:
	  make_blocks (COMPOUND_BODY (t), control_parent, t,
	               &(COMPOUND_BODY (t)));
	  break;

	case FOR_STMT:
	  make_for_stmt_blocks (t, control_parent, compound_stmt,
	      prev_chain_p);
	  break;

	case IF_STMT:
	  make_if_stmt_blocks (t, control_parent, compound_stmt,
	      prev_chain_p);
	  break;

	case WHILE_STMT:
	  make_while_stmt_blocks (t, control_parent, compound_stmt,
	      prev_chain_p);
	  break;

	case SWITCH_STMT:
	  make_switch_stmt_blocks (t, control_parent, compound_stmt,
	      prev_chain_p);
	  break;

	case DO_STMT:
	  make_do_stmt_blocks (t, control_parent, compound_stmt,
	      prev_chain_p);
	  break;

	default:
	  if (is_statement_expression (t))
	    {
	      tree expr = TREE_OPERAND (t, 0);
	      basic_block bb = create_bb (t, t, control_parent, prev_chain_p,
		                          NULL);
	      make_blocks (STMT_EXPR_STMT (expr), bb, t,
			   &(STMT_EXPR_STMT (expr)));
	    }
	  else if (is_exec_stmt (t))
	    {
	      basic_block bb;
	      bb = create_maximal_bb (t, control_parent, compound_stmt,
		  prev_chain_p);
	      t = bb->end_tree;
	    }
	  break;
	}

      if (t)
	{
	  prev_chain_p = &(TREE_CHAIN (t));
	  t = TREE_CHAIN (t);

	  /* If the statement ends a scope, pop the top element from the
	     scope bindings stack.  */
	  if (t && TREE_CODE (t) == SCOPE_STMT && SCOPE_END_P (t))
	    VARRAY_POP (binding_stack);
	}
    }
}

/* }}} */

/* {{{ make_for_stmt_blocks()

   Create the blocks for a FOR_STMT.  */

static void
make_for_stmt_blocks (t, control_parent, compound_stmt, prev_chain_p)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
     tree *prev_chain_p;
{
  basic_block entry, bb;
  tree expr, cond;

  /* Make sure that both condition and expression blocks will be created
     for the loop.
     
     A condition block avoids a self-referencing edge into the loop header
     (which would create loop carried dependencies for the statements in
     FOR_INIT_STMT.

     An expression block avoids having multiple back edges to the condition
     block.  This, in turn, helps the natural loop recognizer identify only
     one loop instead of several shared ones.  */
  cond = (FOR_COND (t)) ? FOR_COND (t) : build_int_2 (1, 0);
  expr = (FOR_EXPR (t)) ? FOR_EXPR (t) : build_int_2 (1, 0);

  entry = create_bb (t, t, control_parent, prev_chain_p, NULL);
  entry->flags |= BB_CONTROL_ENTRY;

  bb = create_maximal_bb (FOR_INIT_STMT (t), entry, compound_stmt,
                          &(FOR_INIT_STMT (t)));
  bb->flags |= BB_CONTROL_EXPR;
  FOR_INIT_STMT_BB (entry) = bb;

  bb = create_maximal_bb (cond, entry, compound_stmt, &(FOR_COND (t)));
  bb->flags |= BB_CONTROL_EXPR;
  FOR_COND_BB (entry) = bb;

  make_blocks (FOR_BODY (t), entry, compound_stmt, &(FOR_BODY (t)));

  bb = create_maximal_bb (expr, entry, compound_stmt, &(FOR_EXPR (t)));
  bb->flags |= BB_CONTROL_EXPR;
  FOR_EXPR_BB (entry) = bb;
}

/* }}} */

/* {{{ make_while_stmt_blocks ()

   Create the blocks for a WHILE_STMT.  */

static void
make_while_stmt_blocks (t, control_parent, compound_stmt, prev_chain_p)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
     tree *prev_chain_p;
{
  basic_block bb, entry;
  
  entry = create_bb (t, t, control_parent, prev_chain_p, NULL);
  entry->flags |= BB_CONTROL_ENTRY | BB_CONTROL_EXPR;
  
  make_blocks (WHILE_BODY (t), entry, compound_stmt, &(WHILE_BODY (t)));

  /* END_WHILE block.  Needed to avoid multiple back edges that would
     result in multiple natural loops instead of just one.  */
  bb = create_maximal_bb (build_int_2 (1, 0), entry, compound_stmt, NULL);
  bb->flags |= BB_CONTROL_ENTRY;
  END_WHILE_BB (entry) = bb;
}

/* }}} */

/* {{{ make_do_stmt_blocks ()

   Create the blocks for a DO_STMT.  */

static void
make_do_stmt_blocks (t, control_parent, compound_stmt, prev_chain_p)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
     tree *prev_chain_p;
{
  basic_block bb, entry;
  
  entry = create_bb (t, t, control_parent, prev_chain_p, NULL);
  entry->flags |= BB_CONTROL_ENTRY;

  make_blocks (DO_BODY (t), entry, compound_stmt, &(DO_BODY (t)));

  bb = create_maximal_bb (DO_COND (t), entry, compound_stmt, &(DO_COND (t)));
  bb->flags |= BB_CONTROL_EXPR;
  DO_COND_BB (entry) = bb;
}

/* }}} */

/* {{{ make_if_stmt_blocks ()

   Create the blocks for an IF_STMT.  */

static void
make_if_stmt_blocks (t, control_parent, compound_stmt, prev_chain_p)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
     tree *prev_chain_p;
{
  basic_block bb = create_bb (t, t, control_parent, prev_chain_p, NULL);
  bb->flags |= BB_CONTROL_ENTRY | BB_CONTROL_EXPR;

  make_blocks (THEN_CLAUSE (t), bb, compound_stmt, &(THEN_CLAUSE (t)));
  make_blocks (ELSE_CLAUSE (t), bb, compound_stmt, &(ELSE_CLAUSE (t)));
}

/* }}} */

/* {{{ make_switch_stmt_blocks ()

   Create the blocks for a SWITCH_STMT.  */

static void
make_switch_stmt_blocks (t, control_parent, compound_stmt, prev_chain_p)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
     tree *prev_chain_p;
{
  basic_block bb = create_bb (t, t, control_parent, prev_chain_p, NULL);
  bb->flags |= BB_CONTROL_ENTRY | BB_CONTROL_EXPR;

  make_blocks (SWITCH_BODY (t), bb, compound_stmt, &(SWITCH_BODY (t)));
}

/* }}} */

/* {{{ create_maximal_bb ()

   Create a maximal basic block.  A maximal basic block is a maximal
   length sequence of consecutive statements that are always executed
   together.  In other words, if the first statement of the block is
   executed, then all the other statements will be executed in sequence
   until and including the last one in the block. 

   T is the first tree of the basic block.

   CONTROL_PARENT is the basic block of the innermost containing control
      structure.

   COMPOUND_STMT is the immediately enclosing compound statement to which
      the first tree of the block belongs.

   PREV_CHAIN_P is the address into the tree preceeding T that contains a
      pointer to T.  This is used when we need to insert statements before
      the first tree of the block.

   Returns the new basic block.  */

static basic_block
create_maximal_bb (t, control_parent, compound_stmt, prev_chain_p)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
     tree *prev_chain_p;
{
  tree first, last;
  basic_block bb;

  if (t == NULL)
    return NULL;

  first = last = t;
  bb = create_bb (first, last, control_parent, prev_chain_p, NULL);

  while (last && last != error_mark_node)
    {
      if (is_exec_stmt (last))
        {
          get_tree_ann (last)->compound_stmt = compound_stmt;
          map_stmt_to_bb (last, bb);
          bb->end_tree = last;
        }

      if (stmt_ends_bb_p (last))
        break;

      last = TREE_CHAIN (last);
    }

  return bb;
}

/* }}} */

/* {{{ create_bb()

   Creates and returns a new basic block.

   HEAD and END are the first and last statements in the block.

   CONTROL_PARENT is the entry block for the control structure containing
      the new block.

   PREV_CHAIN_P is the address into the tree preceeding T that contains a
      pointer to T.  This is used when we need to insert statements before
      the first tree of the block.

   BINDING_SCOPE is the binding scope enclosing the block.  If NULL, the
      binding scope is the top element of the array binding_stack.  */

static basic_block
create_bb (head, end, control_parent, prev_chain_p, binding_scope)
     tree head;
     tree end;
     basic_block control_parent;
     tree *prev_chain_p;
     basic_block binding_scope;
{
  basic_block bb;

  /* Create and initialize a new basic block.  */
  bb = (basic_block) ggc_alloc (sizeof (*bb));
  memset (bb, 0, sizeof (*bb));

  /* If this block starts a new scope, push it into the stack of bindings.  */
  if (binding_scope == NULL
      && TREE_CODE (head) == SCOPE_STMT && SCOPE_BEGIN_P (head))
    VARRAY_PUSH_BB (binding_stack, bb);

  bb->head_tree = head;
  bb->end_tree = end;
  bb->index = n_basic_blocks;

  /* Create annotations for the block.  */
  create_bb_ann (bb);
  BB_PARENT (bb) = control_parent;
  BB_PREV_CHAIN_P (bb) = prev_chain_p;
  BB_BINDING_SCOPE (bb) = (binding_scope)
                          ? binding_scope
			  : VARRAY_TOP_BB (binding_stack);

  if (is_loop_stmt (head))
    {
      union header_blocks *hdr;
      hdr = (union header_blocks *) ggc_alloc (sizeof (union header_blocks));
      memset (hdr, 0, sizeof (union header_blocks));
      BB_LOOP_HDR (bb) = hdr;
    }
  else
    BB_LOOP_HDR (bb) = NULL;

  /* Grow the basic block array if needed.  */
  if ((size_t) n_basic_blocks == VARRAY_SIZE (basic_block_info))
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

   Get the annotation for the given block.  Create a new one if
   necessary.  */

bb_ann
get_bb_ann (bb)
     basic_block bb;
{
  if (BB_ANN (bb) == NULL)
    create_bb_ann (bb);

  return BB_ANN (bb);
}

/* }}} */

/* {{{ create_bb_ann()

   Create a new annotation for basic block BB.  */

static void
create_bb_ann (bb)
     basic_block bb;
{
  bb_ann ann = (bb_ann) ggc_alloc (sizeof (*ann));
  memset ((void *) ann, 0, sizeof (*ann));
  VARRAY_GENERIC_PTR_INIT (ann->refs, 10, "bb_refs");
  bb->aux = (void *) ann;
}

/* }}} */

/* {{{ remove_bb_ann()

   Remove the annotation from block BB.  */

static void
remove_bb_ann (bb)
     basic_block bb;
{
  bb_ann ann = BB_ANN (bb);
  if (ann)
    {
      ann->parent = NULL;
      /* There is no need to delete the arrays in each of the reference.
	 That is done by delete_ssa().  */
      VARRAY_FREE (ann->refs);
    }
  bb->aux = NULL;
}

/* }}} */

/* {{{ tree_split_bb()

   Splits basic block BB at statement T.  A new basic block is created
   starting with the statement following T.  If T is already the last
   statement in the block, nothing is done.

   Returns the newly created basic block or NULL if no splitting is
   necessary.  */

basic_block
tree_split_bb (bb, t)
     basic_block bb;
     tree t;
{
  basic_block new_bb;

  /* If T is already BB's last statement, nothing needs to be done.  */
  if (t == bb->end_tree)
    return NULL;
  
  new_bb = create_maximal_bb (TREE_CHAIN (t), BB_PARENT (bb),
                              TREE_COMPOUND_STMT (t), &(TREE_CHAIN (t)));
  bb->end_tree = t;

  return new_bb;
}

/* }}} */


/* Create edges.  */

/* {{{ make_edges()

   Join all the blocks in the flowgraph.  */

static void
make_edges ()
{
  int i;

  make_edge (ENTRY_BLOCK_PTR, BASIC_BLOCK (0), EDGE_FALLTHRU);

  /* Traverse basic block array placing edges.  */
  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);

      /* Edges for control statements.  */
      if (is_ctrl_stmt (bb->head_tree))
	make_ctrl_stmt_edges (bb);

      /* Edges for statement expressions.  */
      if (is_statement_expression (bb->head_tree))
	{
	  basic_block dest = (i + 1 == n_basic_blocks)
	                     ? EXIT_BLOCK_PTR
			     : BASIC_BLOCK (i + 1);

	  make_edge (bb, dest, 0);
	}

      /* Edges for control flow altering statements (goto, break,
         continue, return) need an edge to the corresponding target
         block.  */
      if (is_ctrl_altering_stmt (bb->end_tree))
	make_exit_edges (bb);

      /* Incoming edges for label blocks in switch statements.  It's easier
         to deal with these bottom-up than top-down.  */
      if (TREE_CODE (bb->head_tree) == CASE_LABEL)
	{
	  basic_block switch_bb = switch_parent (bb);

	  if (switch_bb == NULL)
	    {
	      prep_stmt (bb->head_tree);
	      error ("case label not within a switch statement");
	      return;
	    }

	  make_edge (switch_bb, bb, 0);

	  /* If this label is the default label, we need to remove the
	     fallthru edge that was created when we processed the entry
	     block for the switch() statement.  */
	  if (CASE_LOW (bb->head_tree) == NULL)
	    {
	      edge e;
	      basic_block entry_bb, chain_bb;

	      entry_bb = BB_PARENT (bb);
	      chain_bb = successor_block (entry_bb);
	      for (e = entry_bb->succ; e; e = e->succ_next)
		if (e->dest == chain_bb)
		  {
		    remove_edge (e);
		    break;
		  }
	    }
	}

      /* Finally, if no edges were created above, this is a regular
         basic block that only needs a fallthru edge.  */
      if (bb->succ == NULL)
	make_edge (bb, successor_block (bb), EDGE_FALLTHRU);
    }

  /* Clean up the graph and warn for unreachable code.  */
  tree_cleanup_cfg ();
}

/* }}} */

/* {{{ make_ctrl_stmt_edges()

   Create edges for control statement at basic block BB.  */

static void
make_ctrl_stmt_edges (bb)
     basic_block bb;
{
  switch (TREE_CODE (bb->head_tree))
    {
    case FOR_STMT:
      make_for_stmt_edges (bb);
      break;

    case WHILE_STMT:
      make_while_stmt_edges (bb);
      break;

    case DO_STMT:
      make_do_stmt_edges (bb);
      break;

    case IF_STMT:
      make_if_stmt_edges (bb);
      break;

    case SWITCH_STMT:
      /* Nothing to do.  Each label inside the switch statement will create
         its own edge from the switch block.  */
      break;

    default:
      abort ();
    }
}

/* }}} */

/* {{{ make_exit_edges()

   Create exit edges for statements that alter the flow of control
   (BREAK, CONTINUE, GOTO, RETURN and calls to non-returning functions).  */

static void
make_exit_edges (bb)
     basic_block bb;
{
  switch (TREE_CODE (bb->end_tree))
    {
    case BREAK_STMT:
      make_break_stmt_edges (bb);
      break;

    case CONTINUE_STMT:
      make_continue_stmt_edges (bb);
      break;

    case GOTO_STMT:
      make_goto_stmt_edges (bb);
      break;

    case EXPR_STMT:
    case RETURN_STMT:
      make_edge (bb, EXIT_BLOCK_PTR, 0);
      break;

    default:
      abort ();
    }
}

/* }}} */

/* {{{ make_for_stmt_edges()

   Create edges for a FOR_STMT structure that starts at basic block BB.  */

static void
make_for_stmt_edges (bb)
     basic_block bb;
{
  tree body_t, entry = bb->head_tree;
  basic_block init_bb, cond_bb, expr_bb, body_bb;
  int infinite_loop, zero_iter_loop;

  if (TREE_CODE (entry) != FOR_STMT)
    abort ();

  /* Create the following edges.

     		FOR_STMT
		   |
		   v
	      FOR_INIT_STMT
		   |
                   v
            +-- FOR_COND <-+
            |      |       |
            |      |       |
            |      |       |
            |      v       |
            |   FOR_BODY   |
	    |              |
	    |              |
            |   FOR_EXPR --+
	    |
            +--> Next block
   
     - If the loop does not have an expression block, we replace it with
       the condition block.
       
     - Similarly, if the body is empty, we replace it with the expression 
       block. Hence, loops with neither component will reduce to the
       condition block with a self-referencing edge.  */

  /* make_for_stmt_blocks() guarantees that both condition and expression
     blocks exist in every for loop.  */
  init_bb = FOR_INIT_STMT_BB (bb);
  cond_bb = FOR_COND_BB (bb);
  expr_bb = FOR_EXPR_BB (bb);
  body_t = first_exec_stmt (FOR_BODY (entry));
  body_bb = (body_t) ? BB_FOR_STMT (body_t) : expr_bb;

  make_edge (bb, init_bb, 0);
  make_edge (init_bb, cond_bb, 0);

  /* Simplify the loop if the condition can be statically computed:

     - For infinite loops, do not make an edge between the condition node
       and the first block outside the loop.

     - For zero-iteration loops, do not make an edge into the first block
       of the body nor make a back edge from the latch block.  */

  infinite_loop = (FOR_COND (entry) == NULL
                   || (TREE_CODE (FOR_COND (entry)) == INTEGER_CST
		       && FOR_COND (entry) != integer_zero_node));

  zero_iter_loop = (FOR_COND (entry) == integer_zero_node);

  if (!zero_iter_loop)
    {
      make_edge (cond_bb, body_bb, 0);
      make_edge (expr_bb, cond_bb, 0);
    }

  if (!infinite_loop)
    make_edge (cond_bb, successor_block (bb), 0);
}

/* }}} */

/* {{{ make_while_stmt_edges()

   Create the edges for a WHILE_STMT structure starting with bb.  */

static void
make_while_stmt_edges (bb)
     basic_block bb;
{
  tree body_t, entry = bb->head_tree;
  basic_block end_bb, body_bb;
  int infinite_loop, zero_iter_loop;

  if (TREE_CODE (entry) != WHILE_STMT)
    abort ();

  /* Create the following edges.  The other edges will be naturally created
     by the main loop in create_edges().

         +-> WHILE_STMT ---+
	 |       |         |
         |       v         |
         |   WHILE_BODY    |
	 |                 |
	 |                 |
	 +-- END_WHILE     |
	                   |
		           |
	     Next block <--+
          
     If the body doesn't exist, we use the header instead.  */

  /* Basic blocks for each component.  */
  end_bb = latch_block (bb);
  body_t = first_exec_stmt (WHILE_BODY (entry));
  body_bb = (body_t) ? BB_FOR_STMT (body_t) : end_bb;

  /* Simplify the loop if the condition can be statically computed:

     - For infinite loops, do not make an edge between the entry node and
       the first block outside the loop.

     - For zero-iteration loops, do not make an edge into the first block
       of the body nor make a back edge from the latch block.  */

  infinite_loop = (TREE_CODE (WHILE_COND (entry)) == INTEGER_CST
                   && WHILE_COND (entry) != integer_zero_node);

  zero_iter_loop = (WHILE_COND (entry) == integer_zero_node);

  if (!zero_iter_loop)
    {
      make_edge (bb, body_bb, 0);
      make_edge (end_bb, bb, 0);
    }

  if (!infinite_loop)
    make_edge (bb, successor_block (bb), 0);
}

/* }}} */

/* {{{ make_do_stmt_edges()

   Create the edges for a DO_STMT structure starting with bb.  */

static void
make_do_stmt_edges (bb)
     basic_block bb;
{
  tree body_t, entry = bb->head_tree;
  basic_block cond_bb, body_bb;
  int infinite_loop, one_iter_loop;

  if (TREE_CODE (entry) != DO_STMT)
    abort ();

  /* Create the following edges.  The remaining edges will be added
     by the main loop in make_edges().

            DO_STMT
	       |
	       v
            DO_BODY <-+
                      |
                      |
	    DO_COND --+
	       |
	       v
	   Next block

     If the body doesn't exist, we use the condition instead.  */

  /* Basic blocks for each component.  */
  cond_bb = latch_block (bb);
  body_t = first_exec_stmt (DO_BODY (entry));
  body_bb = (body_t) ? BB_FOR_STMT (body_t) : cond_bb;

  make_edge (bb, body_bb, 0);

  /* Simplify the loop if the condition can be statically computed:

     - For infinite loops, do not make an edge between the conditional
       block and the first block outside the loop.

     - For one-iteration loops (i.e., 'do {} while (0);'), do not make a
       back edge to the beginning of the loop.  */

  infinite_loop = (TREE_CODE (DO_COND (entry)) == INTEGER_CST
                   && DO_COND (entry) != integer_zero_node);

  one_iter_loop = (DO_COND (entry) == integer_zero_node);

  if (!one_iter_loop)
    make_edge (cond_bb, body_bb, 0);

  if (!infinite_loop)
    make_edge (cond_bb, successor_block (bb), 0);
}

/* }}} */

/* {{{ make_if_stmt_edges()

   Create the edges for an IF_STMT structure starting with BB.  */

static void
make_if_stmt_edges (bb)
     basic_block bb;
{
  tree entry = bb->head_tree;
  basic_block successor_bb, then_bb, else_bb;
  tree then_t, else_t;
  int always_true, always_false;

  if (TREE_CODE (entry) != IF_STMT)
    abort ();

  /* Entry basic blocks for each component.  */
  then_t = first_exec_stmt (THEN_CLAUSE (entry));
  then_bb = (then_t) ? BB_FOR_STMT (then_t) : NULL;

  else_t = first_exec_stmt (ELSE_CLAUSE (entry));
  else_bb = (else_t) ? BB_FOR_STMT (else_t) : NULL;

  successor_bb = successor_block (bb);

  /* Create the following edges.

	      IF_STMT
		/ \
	       /   \
	    THEN   ELSE

     Either clause may be empty.  Linearize the IF statement if the
     conditional can be statically computed.  */

  always_true = (TREE_CODE (IF_COND (entry)) == INTEGER_CST
                 && IF_COND (entry) != integer_zero_node);

  always_false = (IF_COND (entry) == integer_zero_node);

  if (always_true)
    else_bb = NULL;

  if (always_false)
    then_bb = NULL;

  if (then_bb)
    make_edge (bb, then_bb, 0);

  if (else_bb)
    make_edge (bb, else_bb, 0);

  /* If the conditional cannot be statically computed and the IF is missing
     one of the clauses, make an edge between the entry block and the
     first block outside the IF.  */
  if (!always_true && !always_false && (!then_bb || !else_bb))
    make_edge (bb, successor_bb, 0);
}

/* }}} */

/* {{{ make_goto_stmt_edges()

   Create edges for a goto statement.  */

static void
make_goto_stmt_edges (bb)
     basic_block bb;
{
  tree goto_t, dest;
  int i;

  goto_t = bb->end_tree;
  if (goto_t == NULL || TREE_CODE (goto_t) != GOTO_STMT)
    abort ();

  dest = GOTO_DESTINATION (goto_t);

  /* Look for the block starting with the destination label.  In the
     case of a computed goto, make an edge to any label block we find
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
	  make_edge (bb, target_bb, 0);
	  break;
	}

      /* Computed GOTOs.  Make an edge to every label block.  */
      else if (TREE_CODE (dest) != LABEL_DECL
	       && TREE_CODE (target) == LABEL_STMT)
	make_edge (bb, target_bb, 0);
    }
}

/* }}} */

/* {{{ make_break_stmt_edges()

   A break statement creates an edge from the break block to the successor
   block for the break statement's control parent.  */

static void
make_break_stmt_edges (bb)
     basic_block bb;
{
  tree break_t;
  basic_block control_parent;

  break_t = bb->end_tree;
  if (break_t == NULL || TREE_CODE (break_t) != BREAK_STMT)
    abort ();

  /* Look for the innermost containing SWITCH, WHILE, FOR or DO.  */
  control_parent = switch_parent (bb);
  if (control_parent == NULL)
    control_parent = loop_parent (bb);

  if (control_parent == NULL)
    {
      prep_stmt (break_t);
      error ("break statement not within loop or switch");
      return;
    }

  make_edge (bb, successor_block (control_parent), 0);
}

/* }}} */

/* {{{ make_continue_stmt_edges()

   A continue statement creates an edge from the continue block to the
   control parent's expression block.  */

static void
make_continue_stmt_edges (bb)
     basic_block bb;
{
  tree continue_t;
  basic_block loop_bb;

  continue_t = bb->end_tree;
  if (continue_t == NULL || TREE_CODE (continue_t) != CONTINUE_STMT)
    abort ();

  /* A continue statement *must* have an enclosing control structure.  */
  loop_bb = loop_parent (bb);

  if (loop_bb == NULL)
    {
      prep_stmt (continue_t);
      error ("continue statement not within a loop");
      return;
    }

  make_edge (bb, latch_block (loop_bb), 0);
}

/* }}} */


/* Flowgraph analysis.  */

/* {{{ tree_cleanup_cfg()

   Remove unreachable blocks and other miscellaneous clean up work.  */

void
tree_cleanup_cfg ()
{
  delete_unreachable_blocks ();
}

/* }}} */

/* {{{ delete_unreachable_blocks()
   
   Delete all unreachable basic blocks.   */

static void
delete_unreachable_blocks ()
{
  int i;

  find_unreachable_blocks ();

  /* Delete all unreachable basic blocks.  Count down so that we
     don't interfere with the block renumbering that happens in
     delete_bb.  */
  for (i = n_basic_blocks - 1; i >= 0; --i)
    {
      basic_block bb = BASIC_BLOCK (i);

      if (!(bb->flags & BB_REACHABLE))
	delete_bb (bb);
    }
}

/* }}} */

/* {{{ delete_bb()

   Remove a block from the flowgraph.  */

static void
delete_bb (bb)
     basic_block bb;
{
  tree t;

  dump_file = dump_begin (TDI_cfg, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "Removed unreachable basic block %d\n", bb->index);
      tree_dump_bb (dump_file, "", bb, 0);
      fprintf (dump_file, "\n");
      dump_end (TDI_cfg, dump_file);
    }

  /* Unmap all the instructions in the block.  */
  t = bb->head_tree;
  while (t)
    {
      map_stmt_to_bb (t, NULL);
      if (t == bb->end_tree)
	break;
      t = TREE_CHAIN (t);
    }

  /* Remove the edges into and out of this block.  */
  while (bb->pred != NULL)
    remove_edge (bb->pred);

  while (bb->succ != NULL)
    remove_edge (bb->succ);

  bb->pred = NULL;
  bb->succ = NULL;

  /* Remove the basic block from the array, and compact behind it.  */
  expunge_block (bb);
}

/* }}} */

/* {{{ validate_loops()

   Scan all the loops in the flowgraph verifying their validity.   A valid
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

/* }}} */

/* {{{ block_invalidates_loop()

   Returns 1 if the basic block BB makes the LOOP invalid.  This occurs if
   the block contains a call to a user function, a return, a jump out of
   the loop or a non-local goto.  */

static int
block_invalidates_loop (bb, loop)
     basic_block bb;
     struct loop *loop;
{
  size_t i;
  varray_type refs;

  /* Valid loops cannot contain a return statement.  */
  if (TREE_CODE (bb->end_tree) == RETURN_STMT)
    return 1;

  /* If the destination node of a goto statement is not in the loop, mark it
     invalid.  */
  if (TREE_CODE (bb->end_tree) == GOTO_STMT
      && ! TEST_BIT (loop->nodes, bb->succ->dest->index))
    return 1;

  /* If the node contains a non-pure function call, mark it invalid.  */
  refs = BB_REFS (bb);
  for (i = 0; i < VARRAY_ACTIVE_SIZE (refs); i++)
    {
      varref ref = VARRAY_GENERIC_PTR (refs, i);
      tree fcall = VARREF_SYM (ref);

      if (TREE_CODE (fcall) == FUNCTION_DECL
	  && ! DECL_IS_PURE (fcall)
	  && ! DECL_BUILT_IN (fcall))
	return 1;
    }

  return 0;
}

/* }}} */


/* Helper functions and predicates.  */

/* {{{ successor_block()

   Return the successor block for BB.  If the block has no successors we try
   the enclosing control structure until we find one.  If we reached nesting
   level 0, return the exit block.  */

static basic_block
successor_block (bb)
     basic_block bb;
{
  basic_block parent_bb;
  tree succ_stmt;

  if (bb == NULL)
    abort ();

  /* Common case.  For control flow header blocks, return the successor of
     the block's first statement.  For regular blocks, return the successor
     of the block's last statement.  */
  succ_stmt = is_ctrl_stmt (bb->head_tree)
    ? first_exec_stmt (TREE_CHAIN (bb->head_tree))
    : first_exec_stmt (TREE_CHAIN (bb->end_tree));

  if (succ_stmt)
    return BB_FOR_STMT (succ_stmt);

  /* We couldn't find a successor for BB.  Walk up the control structure to
     see if our parent has a successor. Iterate until we find one or we
     reach nesting level 0.  */
  parent_bb = BB_PARENT (bb);
  while (parent_bb)
    {
      /* If BB is the last block inside a loop body, return the condition
         block for the loop structure.  */
      if (is_loop_stmt (parent_bb->head_tree))
	return latch_block (parent_bb);

      /* Otherwise, If BB's control parent has a successor, return its
         block.  */
      succ_stmt = first_exec_stmt (TREE_CHAIN (parent_bb->head_tree));
      if (succ_stmt)
	return BB_FOR_STMT (succ_stmt);

      /* None of the above.  Keeping going up the control parent chain.  */
      parent_bb = BB_PARENT (parent_bb);
    }

  /* We reached nesting level 0.  Return the exit block.  */
  return EXIT_BLOCK_PTR;
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
	  || TREE_CODE (t) == DO_STMT);
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

  if (TREE_CODE (t) == GOTO_STMT || TREE_CODE (t) == CONTINUE_STMT
      || TREE_CODE (t) == BREAK_STMT || TREE_CODE (t) == RETURN_STMT)
    return 1;

  /* Calls to non-returning functions also alter the flow of control.  */
  if (TREE_CODE (t) == EXPR_STMT
      && EXPR_STMT_EXPR (t)
      && TREE_CODE (EXPR_STMT_EXPR (t)) == CALL_EXPR)
    {
      tree call_expr = EXPR_STMT_EXPR (t);
      tree addr = TREE_OPERAND (call_expr, 0);
      tree decl = (TREE_CODE (addr) == ADDR_EXPR) 
	          ? TREE_OPERAND (addr, 0) 
		  : addr;
      if (TREE_THIS_VOLATILE (decl))
	return 1;
    }

  return 0;
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
	  || TREE_CODE (t) == WHILE_STMT || TREE_CODE (t) == DO_STMT);
}

/* }}} */

/* {{{ loop_body()

   Return the first statement in the body of LOOP.  */

tree
loop_body (loop)
     tree loop;
{
  if (TREE_CODE (loop) == FOR_STMT)
    return FOR_BODY (loop);
  else if (TREE_CODE (loop) == WHILE_STMT)
    return WHILE_BODY (loop);
  else if (TREE_CODE (loop) == DO_STMT)
    return DO_BODY (loop);
  else
    abort ();
}

/* }}} */

/* {{{ set_loop_body()

   Set the body of LOOP to be STMT.  */

void
set_loop_body (loop, stmt)
     tree loop;
     tree stmt;
{
  tree *prev_chain_p;

  if (TREE_CODE (loop) == FOR_STMT)
    {
      prev_chain_p = &(FOR_BODY (loop));
      FOR_BODY (loop) = stmt;
    }
  else if (TREE_CODE (loop) == WHILE_STMT)
    {
      prev_chain_p = &(WHILE_BODY (loop));
      WHILE_BODY (loop) = stmt;
    }
  else if (TREE_CODE (loop) == DO_STMT)
    {
      prev_chain_p = &(DO_BODY (loop));
      DO_BODY (loop) = stmt;
    }
  else
    abort ();

  /* Create a sub-flowgraph for the new statement and re-compute edges in
     the flowgraph.  */
  make_blocks (stmt, BB_FOR_STMT (loop), TREE_COMPOUND_STMT (loop),
               prev_chain_p);
  make_edges ();
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
	  || TREE_CODE (t) == COMPOUND_STMT
	  || is_statement_expression (t)
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
      || TREE_CHAIN (t) == NULL || stmt_starts_bb_p (TREE_CHAIN (t)))
    return 1;

  return 0;
}

/* }}} */

/* {{{ delete_cfg()

   Remove all the blocks and edges that make up the flowgraph.  */

void
delete_cfg ()
{
  int i;

  if (basic_block_info == NULL)
    return;

  for (i = 0; i < n_basic_blocks; i++)
    remove_bb_ann (BASIC_BLOCK (i));

  remove_bb_ann (ENTRY_BLOCK_PTR);
  remove_bb_ann (EXIT_BLOCK_PTR);

  clear_edges ();
  VARRAY_FREE (basic_block_info);
}

/* }}} */

/* {{{ loop_parent()

   Returns the header block for the innermost loop containing BB.  It
   returns NULL if BB is not inside a loop.  */

basic_block
loop_parent (bb)
     basic_block bb;
{
  do
    bb = BB_PARENT (bb);
  while (bb && !is_loop_stmt (bb->head_tree));

  return bb;
}

/* }}} */

/* {{{ latch_block()

   Returns the block marking the end of the loop body.  This is the block
   that contains the back edge to the start of the loop (i.e., to the block
   containing DO_COND or WHILE_COND or FOR_COND).  */

basic_block
latch_block (loop_bb)
     basic_block loop_bb;
{
  enum tree_code code = TREE_CODE (loop_bb->head_tree);

  if (code == FOR_STMT)
    return FOR_EXPR_BB (loop_bb);
  else if (code == WHILE_STMT)
    return END_WHILE_BB (loop_bb);
  else if (code == DO_STMT)
    return DO_COND_BB (loop_bb);
  else
    abort ();
}

/* }}} */

/* {{{ switch_parent()

   Returns the header block for the innermost switch statement containing
   BB.  It returns NULL if BB is not inside a switch statement.  */

basic_block
switch_parent (bb)
     basic_block bb;
{
  do
    bb = BB_PARENT (bb);
  while (bb && TREE_CODE (bb->head_tree) != SWITCH_STMT);

  return bb;
}

/* }}} */

/* {{{ first_exec_stmt()

   Return the first executable statement starting at T.  */

tree
first_exec_stmt (t)
     tree t;
{
  tree chain;

  if (t == NULL)
    return NULL;

  /* Common case.  T is already an executable statement.  */
  if (is_exec_stmt (t))
    return t;

  /* If T is a compound statement T, try the first executable statement 
     in T's body.  */
  if (TREE_CODE (t) == COMPOUND_STMT)
    {
      chain = first_exec_stmt (COMPOUND_BODY (t));
      if (chain)
	return chain;
    }

  /* If we still haven't found one and T is at the end of a tree chain, try
     the successor of the enclosing compound statement.  */
  if (TREE_CHAIN (t) == NULL)
    {
      chain = first_exec_stmt (TREE_CHAIN (TREE_COMPOUND_STMT (t)));
      if (chain)
	return chain;
    }

  /* Finally, recursively look for the first executable statement
     starting with T's successor.  */
  return first_exec_stmt (TREE_CHAIN (t));
}

/* }}} */

/* {{{ last_exec_stmt (t)

   Return the last executable statement starting at T.  */

tree
last_exec_stmt (t)
     tree t;
{
  tree prev;

  if (t == NULL)
    return NULL;

  prev = NULL;
  t = first_exec_stmt (t);
  while (t && is_exec_stmt (t))
    {
      prev = t;
      t = TREE_CHAIN (t);
    }

  return prev;
}

/* }}} */

/* {{{ is_exec_stmt()

   Return 1 if T is an executable statement.  */

int
is_exec_stmt (t)
    tree t;
{
  return (t
          && statement_code_p (TREE_CODE (t))
          && TREE_CODE (t) != COMPOUND_STMT
          && ! (TREE_CODE (t) == SCOPE_STMT && SCOPE_END_P (t)));
}

/* }}} */

/* {{{ is_statement_expression()

   Return 1 if T is a statement-expression.  */

int
is_statement_expression (t)
     tree t;
{
  return (TREE_CODE (t) == EXPR_STMT
	  && TREE_OPERAND (t, 0)
	  && TREE_CODE (TREE_OPERAND (t, 0)) == STMT_EXPR);
}

/* }}} */

/* {{{ first_non_decl_stmt()

   Returns the first statement that is not a DECL_STMT or SCOPE_STMT, starting
   with T.  */

tree
first_non_decl_stmt (t)
     tree t;
{
  while (t && (TREE_CODE (t) == SCOPE_STMT || TREE_CODE (t) == DECL_STMT))
    t = TREE_CHAIN (t);

  return t;
}

/* }}} */

/* {{{ first_decl_stmt()

   Returns the first statement that is not a DECL_STMT or SCOPE_STMT, starting
   with T.  */

tree
first_decl_stmt (t)
     tree t;
{
  while (t && TREE_CODE (t) != DECL_STMT)
    t = TREE_CHAIN (t);

  return t;
}

/* }}} */

/* {{{ first_non_label_in_bb()

   Returns the first executable statement that is not a LABEL or CASE_LABEL
   in basic block BB.  Returns NULL if the block only contains labels.  */

tree
first_non_label_in_bb (bb)
     basic_block bb;
{
  tree t;

  t = bb->head_tree;
  while (t 
         && is_exec_stmt (t)
	 && t != bb->end_tree
	 && (TREE_CODE (t) == LABEL_STMT
	     || TREE_CODE (t) == CASE_LABEL))
    t = TREE_CHAIN (t);

  return ((t && t != bb->end_tree) ? t : NULL);
}

/* }}} */


/* Code insertion and replacement.  */

/* {{{ insert_stmt_tree_before()

   Insert statement STMT before tree WHERE in basic block BB.  The
   insertion is flow-sensitive.  After insertion, statement STMT is
   guaranteed to always execute before WHERE.

   ??? Important, this code only supports the insertion of simple
   statements. Inserting control statements will require re-computing the
   flowgraph.
   
   Also, insertion of expressions is not supported.  The code is
   not prepared to handle all the side-effects and look for correct
   sequence points where to insert arbitrary expressions.  */

void
insert_stmt_tree_before (stmt, where, bb)
     tree stmt;
     tree where;
     basic_block bb;
{
  /* Make sure STMT is a statement with no existing chain.  */
  if (! statement_code_p (TREE_CODE (stmt)) || TREE_CHAIN (stmt))
    abort ();

  dump_file = dump_begin (TDI_cfg, &dump_flags);

  /* If the basic block contains a control flow expression, we may need
     to do other insertions.  */
  if (bb->flags & BB_CONTROL_EXPR)
    insert_before_ctrl_stmt (stmt, where, bb);
  else
    insert_before_normal_stmt (stmt, where, bb);

  if (dump_file)
    dump_end (TDI_cfg, dump_file);
}

/* }}} */

/* {{{ insert_before_ctrl_stmt()

   Subroutine of insert_stmt_before() to handle insertions in control
   header blocks.  */

static void
insert_before_ctrl_stmt (stmt, where, bb)
     tree stmt;
     tree where;
     basic_block bb;
{
  basic_block parent_bb;
  tree parent;

  /* If BB is already a control entry block (IF, WHILE, CASE), then we
     don't need to go to its parent.  */
  parent_bb = (bb->flags & BB_CONTROL_ENTRY) ? bb : BB_PARENT (bb);
  parent = parent_bb->head_tree;

  if (dump_file)
    {
      fprintf (dump_file, "\nAbout to insert statement: ");
      print_node_brief (dump_file, "", stmt, 0);
      fprintf (dump_file, "\nBefore statement: ");
      print_node_brief (dump_file, "", parent, 0);
      fprintf (dump_file, " (line %d)\n", STMT_LINENO (parent));
      fprintf (dump_file, "At basic block %d\n", bb->index);
    }

  /* If this is not a loop, do a normal insertion before the control
     statement's entry point.  */
  if (! is_loop_stmt (parent))
    insert_before_normal_stmt (stmt, parent, parent_bb);

  /* WHILE_STMT block.  Insert before the entry block and after the last
     block in the body.  */
  else if (TREE_CODE (parent) == WHILE_STMT)
    {
      insert_before_normal_stmt (stmt, parent, bb);

      if (first_exec_stmt (first_non_decl_stmt (WHILE_BODY (parent))) == NULL)
	WHILE_BODY (parent) = copy_node (stmt);
      else
	insert_after_loop_body (copy_node (stmt), parent_bb);
    }

  /* DO_STMT block.  Insert at the end of the loop body.  */
  else if (TREE_CODE (parent) == DO_STMT)
    {
      if (first_exec_stmt (first_non_decl_stmt (DO_BODY (parent))) == NULL)
	DO_BODY (parent) = stmt;
      else
	insert_after_loop_body (stmt, parent_bb);
    }

  /* FOR_STMT block.  Check which of FOR_INIT_EXPR, FOR_COND or FOR_EXPR we
     are dealing with.  */
  else if (TREE_CODE (parent) == FOR_STMT)
    {
      /* FOR_INIT_STMT.  Insert before its first statement.  */
      if (bb == FOR_INIT_STMT_BB (parent_bb))
	{
	  if (first_exec_stmt (FOR_INIT_STMT (parent)) == NULL)
	    FOR_INIT_STMT (parent) = stmt;
	  else
	    insert_before_normal_stmt (stmt, where, bb);
	}

      /* FOR_COND block.  Insert at the end of FOR_INIT_STMT and at the end
	 of FOR_EXPR.  */
      else if (bb == FOR_COND_BB (parent_bb))
	{
	  tree last_stmt = last_exec_stmt (FOR_INIT_STMT (parent));
	  if (last_stmt)
	    insert_after_normal_stmt (stmt, last_stmt,
				      BB_FOR_STMT (last_stmt));
	  else
	    FOR_INIT_STMT (parent) = stmt;

	  last_stmt = last_exec_stmt (FOR_EXPR (parent));
	  if (last_stmt)
	    insert_after_normal_stmt (copy_node (stmt), last_stmt,
				      BB_FOR_STMT (last_stmt));
	  else
	    FOR_EXPR (parent) = copy_node (stmt);
	}

      /* FOR_EXPR block.  Insert at the end of the loop body.  */
      else if (bb == FOR_EXPR_BB (parent_bb))
	{
	  if (first_exec_stmt (first_non_decl_stmt (FOR_BODY (parent))) == NULL)
	    FOR_BODY (parent) = stmt;
	  else
	    insert_after_loop_body (stmt, parent_bb);
	}
      else
	abort();
    }
  
  else
    abort ();
}

/* }}} */

/* {{{ insert_before_normal_stmt()

   Subroutine of insert_stmt_tree_before() to handle insertions in regular
   statements.  If STMT is inserted before a block boundary, a new basic
   block is created to hold it.  */

static void
insert_before_normal_stmt (stmt, where, bb)
     tree stmt;
     tree where;
     basic_block bb;
{
  /* If the insertion is somewhere in the middle of the block, look for
     the insertion point starting at the head.  */
  if (where != bb->head_tree)
    {
      tree last, prev;

      prev = NULL;
      last = bb->head_tree;
      while (last && last != where)
	{
	  prev = last;
	  last = TREE_CHAIN (last);
	}

      if (prev == NULL)
	abort ();

      TREE_CHAIN (prev) = stmt;
      TREE_CHAIN (stmt) = where;
      map_stmt_to_bb (stmt, bb);

      if (dump_file)
	{
	  fprintf (dump_file, "\nInserted statement: ");
	  print_node_brief (dump_file, "", stmt, 0);
	  fprintf (dump_file, "\nBefore statement  : ");
	  print_node_brief (dump_file, "", where, 0);
	  fprintf (dump_file, " (line %d)\n", STMT_LINENO (where));
	  fprintf (dump_file, "At basic block %d\n", bb->index);
	}
    }

  /* Otherwise, insert the statement in the previous tree's
     TREE_CHAIN and create a new basic block holding the statement, if
     needed.  */
  else
    {
      basic_block new_bb = NULL;
      tree *prev_chain_p = BB_PREV_CHAIN_P (bb);

      *prev_chain_p = stmt;
      TREE_CHAIN (stmt) = where;
      if (is_ctrl_stmt (where))
	{
	  new_bb = create_bb (stmt, stmt, BB_PARENT (bb), prev_chain_p,
	                      BB_BINDING_SCOPE (bb));
	  insert_bb_before (new_bb, bb);
	}
      else
	{
	  map_stmt_to_bb (stmt, bb);
	  bb->head_tree = stmt;
	}

      if (dump_file)
	{
	  fprintf (dump_file, "\nInserted statement: ");
	  print_node_brief (dump_file, "", stmt, 0);
	  fprintf (dump_file, "\nBefore statement  : ");
	  print_node_brief (dump_file, "", where, 0);
	  fprintf (dump_file, " (line %d)\n", STMT_LINENO (where));
	  if (new_bb)
	    fprintf (dump_file, "Created new basic block %d\n",
		     new_bb->index);
	  else
	    fprintf (dump_file, "At basic block %d\n", bb->index);
	}
    }
}

/* }}} */

/* {{{ insert_stmt_tree_after()

   Insert statement STMT after statement WHERE in basic block BB.  The
   insertion is flow-sensitive.  After insertion, statement STMT is
   guaranteed to always execute after WHERE.

   ??? Important, this code only supports the insertion of simple
   statements. Inserting control statements will require re-computing the
   flowgraph.
   
   Also, insertion of expressions is not supported.  The code is
   not prepared to handle all the side-effects and look for correct
   sequence points where to insert arbitrary expressions.  */

void
insert_stmt_tree_after (stmt, where, bb)
     tree stmt;
     tree where;
     basic_block bb;
{
  /* Only accept statement trees.  */
  if (! statement_code_p (TREE_CODE (stmt)))
    abort ();

  dump_file = dump_begin (TDI_cfg, &dump_flags);

  if (bb->flags & BB_CONTROL_EXPR)
    insert_after_ctrl_stmt (stmt, bb);
  else
    insert_after_normal_stmt (stmt, where, bb);

  if (dump_file)
    dump_end (TDI_cfg, dump_file);
}

/* }}} */

/* {{{ insert_after_ctrl_stmt()

   Subroutine of insert_stmt_tree_after() to handle insertions at control
   statement header blocks.  */

static void
insert_after_ctrl_stmt (stmt, bb)
     tree stmt;
     basic_block bb;
{
  basic_block parent_bb;
  tree parent;
  tree t;

  /* If BB is already a control entry block (IF, WHILE, CASE), then we
     don't need to go to its parent.  */
  parent_bb = (bb->flags & BB_CONTROL_ENTRY) ? bb : BB_PARENT (bb);
  parent = parent_bb->head_tree;

  if (dump_file)
    {
      fprintf (dump_file, "\nAbout to insert statement: ");
      print_node_brief (dump_file, "", stmt, 0);
      fprintf (dump_file, "\nAfter statement: ");
      print_node_brief (dump_file, "", parent, 0);
      fprintf (dump_file, " (line %d)\n", STMT_LINENO (parent));
      fprintf (dump_file, "At basic block %d\n", bb->index);
    }

  /* IF_STMT block.  Insert before THEN_CLAUSE and ELSE_CLAUSE.  */
  if (TREE_CODE (parent) == IF_STMT)
    {
      t = first_exec_stmt (first_non_decl_stmt (THEN_CLAUSE (parent)));
      if (t == NULL)
	THEN_CLAUSE (parent) = stmt;
      else
	insert_before_normal_stmt (stmt, t, BB_FOR_STMT (t));

      t = first_exec_stmt (first_non_decl_stmt (ELSE_CLAUSE (parent)));
      if (t == NULL)
	ELSE_CLAUSE (parent) = copy_node (stmt);
      else
	insert_before_normal_stmt (copy_node (stmt), t, BB_FOR_STMT (t));
    }

  /* SWITCH_STMT block.  Insert before each case block (after the label).  */
  else if (TREE_CODE (parent) == SWITCH_STMT)
    {
      edge e;

      for (e = parent_bb->succ; e; e = e->succ_next)
	{
	  basic_block succ_bb = e->dest;
	  t = first_non_label_in_bb (succ_bb);
	  if (t)
	    insert_before_normal_stmt (copy_node (stmt), t, succ_bb);
	}
    }

  /* WHILE_STMT block.  Insert before the first statement in the body.  */
  else if (TREE_CODE (parent) == WHILE_STMT)
    {
      t = first_exec_stmt (first_non_decl_stmt (WHILE_BODY (parent)));
      if (t == NULL)
	WHILE_BODY (parent) = stmt;
      else
	insert_before_normal_stmt (stmt, t, BB_FOR_STMT (t));
    }

  /* DO_STMT block.  Insert before the first statement in the body.
     FIXME: This is wrong, we should be replacing the conditional
            with an expression-statement.  */
  else if (TREE_CODE (parent) == DO_STMT)
    {
      t = first_exec_stmt (first_non_decl_stmt (DO_BODY (parent)));
      if (t == NULL)
	DO_BODY (parent) = stmt;
      else
	insert_before_normal_stmt (stmt, t, BB_FOR_STMT (t));
    }

  /* FOR_STMT block.  Check which of FOR_INIT_STMT, FOR_COND or FOR_EXPR
     we are dealing with.  */
  else if (TREE_CODE (parent) == FOR_STMT)
    {
      /* FOR_INIT_STMT block.  Insert after the last init statement.  */
      if (bb == FOR_INIT_STMT_BB (parent_bb))
	{
	  t = last_exec_stmt (FOR_INIT_STMT (parent));
	  if (t == NULL)
	    FOR_INIT_STMT (parent) = stmt;
	  else
	    insert_after_normal_stmt (stmt, t, bb);
	}

      /* FOR_COND block.  Insert before the first statement in the body.  */
      else if (bb == FOR_COND_BB (parent_bb))
	{
	  t = first_exec_stmt (first_non_decl_stmt (FOR_BODY (parent)));
	  if (t == NULL)
	    FOR_BODY (parent) = stmt;
	  else
	    insert_before_normal_stmt (stmt, t, BB_FOR_STMT (t));
	}

      /* FOR_EXPR block.  Insert after the last expr statement.  */
      else if (bb == FOR_EXPR_BB (parent_bb))
	{
	  t = last_exec_stmt (FOR_EXPR (parent));
	  if (t == NULL)
	    FOR_EXPR (parent) = stmt;
	  else
	    insert_after_normal_stmt (stmt, t, bb);
	}
      
      else
	abort ();
    }

  else
    abort ();
}

/* }}} */

/* {{{ insert_after_normal_stmt()

   Subroutine of insert_stmt_tree_after() to insert after normal
   statements.  */

static void
insert_after_normal_stmt (stmt, where, bb)
     tree stmt;
     tree where;
     basic_block bb;
{
  /* If the statement goes at the end of the block, we need to adjust the
     PREV_CHAIN_P pointer of each successor block that was pointing back to
     TREE_CHAIN (where).  */
  if (where == bb->end_tree)
    {
      edge e;

      for (e = bb->succ; e; e = e->succ_next)
	{
	  basic_block succ_bb = e->dest;

	  if (BB_PREV_CHAIN_P (succ_bb) == &(TREE_CHAIN (where)))
	    get_bb_ann (succ_bb)->prev_chain_p = &(TREE_CHAIN (stmt));
	}
    }

  /* Chain STMT after WHERE.  */
  TREE_CHAIN (stmt) = TREE_CHAIN (where);
  TREE_CHAIN (where) = stmt;

  /* Extend the basic block to contain STMT.  */
  map_stmt_to_bb (stmt, bb);
  if (where == bb->end_tree)
    bb->end_tree = stmt;

  if (dump_file)
    {
      fprintf (dump_file, "\nInserted statement: ");
      print_node_brief (dump_file, "", stmt, 0);
      fprintf (dump_file, "\nAfter statement  : ");
      print_node_brief (dump_file, "", where, 0);
      fprintf (dump_file, " (line %d)\n", STMT_LINENO (where));
      fprintf (dump_file, "At basic block %d\n", bb->index);
    }
}

/* }}} */

/* {{{ insert_after_loop_body()

   Inserts STMT so that it will be executed after the body of the loop
   starting at basic block LOOP.  */

static void
insert_after_loop_body (stmt, loop)
     tree stmt;
     basic_block loop;
{
  edge e;
  tree last_stmt;
  basic_block latch_bb;

  /* For every predecessor PRED_BB of the latch block, we need to insert a
     copy of STMT if PRED_BB ends in a CONTINUE_STMT, BREAK_STMT or if its
     parent block is the loop header.

     The last condition is to avoid inserting unnecessary copies in cases
     like this one:

     			for ()
			  {
			    ....
			    if ()
			      a;
			    else
			      b;
			  }

     In this case, both 'a' and 'b' are predecessors of the latch block,
     but instead of inserting STMT twice, it's better to insert it after
     the if() statement.  */
  latch_bb = latch_block (loop);
  for (e = latch_bb->pred; e; e = e->pred_next)
    {
      basic_block pred_bb = e->src;
      last_stmt = pred_bb->end_tree;

      if (TREE_CODE (last_stmt) == CONTINUE_STMT
	  || TREE_CODE (last_stmt) == BREAK_STMT)
	insert_before_normal_stmt (copy_node (stmt), pred_bb->end_tree,
	                           pred_bb);
    }

  /* Insert STMT after the last executable statement in the loop body.  */
  last_stmt = last_exec_stmt (loop_body (loop->head_tree));
  insert_after_normal_stmt (copy_node (stmt), last_stmt, 
                            BB_FOR_STMT (last_stmt));
}

/* }}} */

/* {{{ replace_expr_in_tree()

   Replace expression EXPR in statement STMT with NEW_EXPR.  */

void
replace_expr_in_tree (stmt, old_expr, new_expr)
     tree stmt;
     tree old_expr;
     tree new_expr;
{
  tree *old_expr_p = find_expr_in_tree (stmt, old_expr);

  dump_file = dump_begin (TDI_cfg, &dump_flags);
  if (dump_file)
    {
      if (old_expr_p)
	{
	  fprintf (dump_file, "\nRequested expression: ");
	  print_node_brief (dump_file, "", old_expr, 0);

	  fprintf (dump_file, "\nReplaced expression:  ");
	  print_node_brief (dump_file, "", *old_expr_p, 0);

	  fprintf (dump_file, "\nWith expression:      ");
	  print_node_brief (dump_file, "", new_expr, 0);
	}
      else
	{
	  fprintf (dump_file, "\nCould not find expression: ");
	  print_node_brief (dump_file, "", old_expr, 0);
	}

      fprintf (dump_file, "\nIn statement:        ");
      print_node_brief (dump_file, "", stmt, 0);

      fprintf (dump_file, "\nBasic block:         ");
      if (statement_code_p (TREE_CODE (stmt)))
	fprintf (dump_file, "%d", BB_FOR_STMT (stmt)->index);
      else
	fprintf (dump_file, "-1");

      fprintf (dump_file, "\nLine:                ");
      if (statement_code_p (TREE_CODE (stmt)))
	fprintf (dump_file, "%d", STMT_LINENO (stmt));
      else
	fprintf (dump_file, "-1");

      fprintf (dump_file, "\n");

      dump_end (TDI_cfg, dump_file);
    }

  if (old_expr_p)
    *old_expr_p = new_expr;
}

/* }}} */

/* {{{ find_expr_in_tree()

   Returns the location of expression EXPR in T.  The search is guaranteed
   to not go outside statement nodes, only their sub-expressions are
   searched.  */

tree *
find_expr_in_tree (t, expr)
     tree t;
     tree expr;
{
  int i;
  tree *loc;

  if (t == NULL || t == error_mark_node
      || expr == NULL || expr == error_mark_node)
    return NULL;

  /* Deal with special trees first.  */
  switch (TREE_CODE (t))
    {
      case COMPLEX_CST:
      case INTEGER_CST:
      case LABEL_DECL:
      case REAL_CST:
      case RESULT_DECL:
      case STRING_CST:
	return NULL;

      case TREE_LIST:
	  {
	    tree op;

	    /* Try the list elements.  */
	    for (op = t; op; op = TREE_CHAIN (op))
	      if (TREE_VALUE (op) == expr)
		return &(TREE_VALUE (op));

	    /* Not there?  Recurse into each of the list elements.  */
	    for (op = t; op; op = TREE_CHAIN (op))
	      {
		loc = find_expr_in_tree (TREE_VALUE (op), expr);
		if (loc)
		  return loc;
	      }

	    return NULL;
	  }

      default:
	break;
    }

  /* Try the immediate operands.  */
  for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (t)); i++)
    if (TREE_OPERAND (t, i) == expr)
      return &(TREE_OPERAND (t, i));

  /* If we still haven't found it, recurse into each sub-expression of T.  */
  for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (t)); i++)
    {
      loc = find_expr_in_tree (TREE_OPERAND (t, i), expr);
      if (loc)
	return loc;
    }

  /* Finally, if T is not a statement, recurse into its chain (this limits
     the search to a single statement).  */
  if (! statement_code_p (TREE_CODE (t)))
    {
      loc = find_expr_in_tree (TREE_CHAIN (t), expr);
      if (loc)
	return loc;
    }

  return NULL;
}

/* }}} */

/* {{{ insert_bb_before()

   Insert basic block NEW_BB before BB.  */

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

  s_indent = (char *) alloca ((size_t) indent + 1);
  memset ((void *) s_indent, ' ', (size_t) indent);
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

  fprintf (outf, "%s%sPrevious stmt: ", s_indent, prefix);
  if (BB_PREV_CHAIN_P (bb))
    {
      tree *t = BB_PREV_CHAIN_P (bb);
      lineno = (*t && statement_code_p (TREE_CODE (*t)))
	       ? STMT_LINENO (*t)
	       : -1;
      print_node_brief (outf, "", *t, 0);
      fprintf (outf, " (line: %d)\n", lineno);
    }
  else
    fputs ("nil\n", outf);
    
  fprintf (outf, "%s%sBinding scope block: ", s_indent, prefix);
  if (BB_BINDING_SCOPE (bb))
    fprintf (outf, "%d\n", BB_BINDING_SCOPE (bb)->index);
  else
    fputs ("nil\n", outf);

  fprintf (outf, "%s%sLoop depth: %d\n", s_indent, prefix, bb->loop_depth);
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
  tree_dump_cfg (stderr);
}

/* }}} */

/* {{{ tree_dump_cfg ()

   Dump the CFG on the given FILE.  */

void
tree_dump_cfg (file)
     FILE *file;
{
  int i;

  fputc ('\n', file);
  fprintf (file, ";; Function %s\n\n",
	   IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

  fprintf (file, "\n%d basic blocks, %d edges.\n", n_basic_blocks, n_edges);
  for (i = 0; i < n_basic_blocks; i++)
    {
      tree_dump_bb (file, "", BASIC_BLOCK (i), 0);
      fputc ('\n', file);
    }
}

/* }}} */

/* {{{ tree_cfg2dot()

   Dump the flowgraph to a .dot FILE to be visualized with GraphViz.
   See http://www.graphviz.org/  */

void
tree_cfg2dot (file)
     FILE *file;
{
  int i;
  edge e;

  /* Write the file header.  */
  fprintf (file, "digraph %s\n{\n",
	   IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

  /* Write blocks and edges.  */
  for (e = ENTRY_BLOCK_PTR->succ; e; e = e->succ_next)
    {
      fprintf (file, "\tENTRY -> %d", e->dest->index);

      if (e->flags & EDGE_FAKE)
	fprintf (file, " [weight=0, style=dotted]");

      fprintf (file, ";\n");
    }
  fputc ('\n', file);

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
	? STMT_LINENO (bb->head_tree) : -1;
      end_line = (statement_code_p (end_code))
	? STMT_LINENO (bb->end_tree) : -1;

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

      if (i < n_basic_blocks - 1)
	fputc ('\n', file);
    }

  fputs ("}\n\n", file);
}

/* }}} */
