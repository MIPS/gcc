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
#include "expr.h"
#include "c-common.h"
#include "ggc.h"
#include "diagnostic.h"
#include "tree-opt.h"
#include "tree-flow.h"

/* {{{ Local declarations.  */

/* Initial capacity for the basic block array.  */

static const int initial_cfg_capacity = 20;

/* Dump files and flags.  */
static FILE *cfg_dump_file;
static FILE *dot_dump_file;
static int cfg_dump_flags;
static int dot_dump_flags;

/* Basic blocks and flowgraphs.  */
static void make_blocks PARAMS ((tree, basic_block, tree));
static void make_for_stmt_blocks PARAMS ((tree, basic_block, tree));
static void make_if_stmt_blocks PARAMS ((tree, basic_block, tree));
static void make_while_stmt_blocks PARAMS ((tree, basic_block, tree));
static void make_switch_stmt_blocks PARAMS ((tree, basic_block, tree));
static void make_do_stmt_blocks PARAMS ((tree, basic_block, tree));
static basic_block create_bb PARAMS ((tree, tree, basic_block));
static basic_block create_maximal_bb PARAMS ((tree, basic_block, tree));
static void map_stmt_to_bb PARAMS ((tree, basic_block));
static void delete_unreachable_blocks PARAMS ((void));
static void delete_block PARAMS ((basic_block));

/* Edges.  */
static void make_edges PARAMS ((void));
static void make_ctrl_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_exit_edges PARAMS ((sbitmap *, basic_block));
static void make_for_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_while_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_do_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_if_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_goto_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_break_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_continue_stmt_edges PARAMS ((sbitmap *, basic_block));
static void make_return_stmt_edges PARAMS ((sbitmap *, basic_block));

/* Various helpers.  */
static basic_block successor_block PARAMS ((basic_block));

/* }}} */


/* Create basic blocks.  */

/* {{{ tree_find_basic_blocks()
   
   Entry point to the CFG builder for trees.  */

void
tree_find_basic_blocks (t)
     tree t;
{
  cfg_dump_file = dump_begin (TDI_cfg, &cfg_dump_flags);
  dot_dump_file = dump_begin (TDI_dot, &dot_dump_flags);

  /* Flush out existing data.  */
  delete_cfg ();

  /* Initialize the basic block array.  */
  n_basic_blocks = 0;
  VARRAY_BB_INIT (basic_block_info, initial_cfg_capacity, "basic_block_info");

  /* Find the basic blocks for the flowgraph.  */
  make_blocks (t, NULL, NULL);

  if (n_basic_blocks > 0)
    {
      /* Create the edges of the flowgraph.  */
      make_edges ();

      mark_critical_edges ();

      /* Write the flowgraph to a GraphViz file.  */
      if (dot_dump_file)
	tree_cfg2dot (dot_dump_file);

      /* Dump a textual representation of the flowgraph.  */
      if (cfg_dump_file)
	tree_dump_cfg (cfg_dump_file);
    }

  if (dot_dump_file)
    dump_end (TDI_dot, dot_dump_file);

  if (cfg_dump_file)
    dump_end (TDI_cfg, cfg_dump_file);
}

/* }}} */

/* {{{ make_blocks()

   Build a flowgraph for the tree starting with T.
   
   CONTROL_PARENT is the header block for the control structure immediately
   enclosing the new sub-graph.
 
   COMPOUND_STMT is the immediately enclosing compound statement to which T
   belongs.  These statements are not represented in the flowgraph, but are
   important to determine successor basic blocks in successor_block().  */

static void
make_blocks (t, control_parent, compound_stmt)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
{
  /* Traverse the statement chain building basic blocks.  */
  while (t && t != error_mark_node)
    {
      get_tree_ann (t)->compound_stmt = compound_stmt;

      switch (TREE_CODE (t))
	{
	case COMPOUND_STMT:
	  make_blocks (COMPOUND_BODY (t), control_parent, t);
	  break;

	case FOR_STMT:
	  make_for_stmt_blocks (t, control_parent, compound_stmt);
	  break;

	case IF_STMT:
	  make_if_stmt_blocks (t, control_parent, compound_stmt);
	  break;

	case WHILE_STMT:
	  make_while_stmt_blocks (t, control_parent, compound_stmt);
	  break;

	case SWITCH_STMT:
	  make_switch_stmt_blocks (t, control_parent, compound_stmt);
	  break;

	case DO_STMT:
	  make_do_stmt_blocks (t, control_parent, compound_stmt);
	  break;

	default:
	  if (is_exec_stmt (t))
	    {
	      basic_block bb;
	      bb = create_maximal_bb (t, control_parent, compound_stmt);
	      t = bb->end_tree;
	    }
	  break;
	}

      if (t)
	t = TREE_CHAIN (t);
    }
}

/* }}} */

/* {{{ make_for_stmt_blocks()

   Create the blocks for a FOR_STMT.  */

static void
make_for_stmt_blocks (t, control_parent, compound_stmt)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
{
  basic_block bb;
  tree cond;

  /* Make sure that the condition block will be created even if the loop
     has no condition.  This avoids a self-referencing edge into the loop
     header (which would create loop carried dependencies for the
     statements in FOR_INIT_STMT.  */
  cond = (FOR_COND (t)) ? FOR_COND (t) : build_int_2 (1, 0);

  bb = create_bb (t, FOR_INIT_STMT (t), control_parent);
  create_maximal_bb (cond, bb, compound_stmt);
  create_maximal_bb (FOR_EXPR (t), bb, compound_stmt);
  make_blocks (FOR_BODY (t), bb, compound_stmt);
}

/* }}} */

/* {{{ make_while_stmt_blocks ()

   Create the blocks for a WHILE_STMT.  */

static void
make_while_stmt_blocks (t, control_parent, compound_stmt)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
{
  basic_block bb = create_bb (t, t, control_parent);
  make_blocks (WHILE_BODY (t), bb, compound_stmt);
}

/* }}} */

/* {{{ make_do_stmt_blocks ()

   Create the blocks for a DO_STMT.  */

static void
make_do_stmt_blocks (t, control_parent, compound_stmt)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
{
  basic_block bb = create_bb (t, t, control_parent);
  create_maximal_bb (DO_COND (t), bb, compound_stmt);
  make_blocks (DO_BODY (t), bb, compound_stmt);
}

/* }}} */

/* {{{ make_if_stmt_blocks ()

   Create the blocks for an IF_STMT.  */

static void
make_if_stmt_blocks (t, control_parent, compound_stmt)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
{
  basic_block bb = create_bb (t, IF_COND (t), control_parent);
  make_blocks (THEN_CLAUSE (t), bb, compound_stmt);
  make_blocks (ELSE_CLAUSE (t), bb, compound_stmt);
}

/* }}} */

/* {{{ make_switch_stmt_blocks ()

   Create the blocks for a SWITCH_STMT.  */

static void
make_switch_stmt_blocks (t, control_parent, compound_stmt)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
{
  basic_block bb = create_bb (t, SWITCH_COND (t), control_parent);
  make_blocks (SWITCH_BODY (t), bb, compound_stmt);
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

   Returns the new basic block.  */

static basic_block
create_maximal_bb (t, control_parent, compound_stmt)
     tree t;
     basic_block control_parent;
     tree compound_stmt;
{
  tree first, last;
  basic_block bb;

  if (t == NULL)
    return NULL;

  first = last = t;
  bb = create_bb (first, last, control_parent);

  while (last && last != error_mark_node)
    {
      get_tree_ann (last)->compound_stmt = compound_stmt;

      if (is_exec_stmt (last))
	{
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
   the new block.  */

static basic_block
create_bb (head, end, control_parent)
     tree head;
     tree end;
     basic_block control_parent;
{
  basic_block bb;

  /* Create and initialize a new basic block.  */
  bb = (basic_block) ggc_alloc (sizeof (*bb));
  memset (bb, 0, sizeof (*bb));

  bb->head_tree = head;
  bb->end_tree = end;
  bb->index = n_basic_blocks;
  get_bb_ann (bb)->parent = control_parent;

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

basic_block_ann
get_bb_ann (bb)
     basic_block bb;
{
  basic_block_ann ann = BB_ANN (bb);

  if (ann == NULL)
    {
      ann = (basic_block_ann) ggc_alloc (sizeof (*ann));
      memset ((void *) ann, 0, sizeof (*ann));

      bb->aux = (void *) ann;
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

  /* Traverse basic block array placing edges.  */
  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);

      /* Edges for control statements.  */
      if (is_ctrl_stmt (bb->head_tree))
	make_ctrl_stmt_edges (edge_cache, bb);

      /* Edges for control flow altering statements (goto, break,
         continue, return) need an edge to the corresponding target
         block.  */
      if (is_ctrl_altering_stmt (bb->end_tree))
	make_exit_edges (edge_cache, bb);

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

	  make_edge (edge_cache, switch_bb, bb, 0);
	}

      /* Finally, if no edges were created above, this is a regular
         basic block that only needs a fallthru edge.  */
      if (bb->succ == NULL)
	make_edge (edge_cache, bb, successor_block (bb), EDGE_FALLTHRU);
    }

  /* Clean up the graph and warn for unreachable code.  */
  delete_unreachable_blocks ();

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
      /* Nothing to do.  Each label inside the switch statement will create
         its own edge form the switch block.  */
      break;

    default:
      abort ();
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
  switch (TREE_CODE (bb->end_tree))
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

/* {{{ make_for_stmt_edges()

   Create edges for a FOR_STMT structure that starts at basic block BB.  */

static void
make_for_stmt_edges (edge_cache, bb)
     sbitmap *edge_cache;
     basic_block bb;
{
  tree entry = bb->head_tree;
  int ix;
  basic_block cond_bb, expr_bb, body_bb;

  if (TREE_CODE (entry) != FOR_STMT)
    abort ();

  /* Create the following edges.

     		FOR_STMT
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
            +--> Next block
   
     - If the loop does not have an expression block, we replace it with
       the condition block.
       
     - Similarly, if the body is empty, we replace it with the expression 
       block. Hence, loops with neither component will reduce to the
       condition block with a self-referencing edge.  */

  /* Basic blocks for each component.  Note that the block numbers are
     determined by make_for_stmt_blocks().  */
  ix = bb->index + 1;

  /* make_for_stmt_blocks() guarantees that the condition block is created
     even for unconditional loops.  */
  cond_bb = BASIC_BLOCK (ix++);
  expr_bb = (FOR_EXPR (entry)) ? BASIC_BLOCK (ix++) : cond_bb;
  body_bb = (first_exec_stmt (FOR_BODY (entry))) ? BASIC_BLOCK (ix) : expr_bb;

  make_edge (edge_cache, bb, cond_bb, 0);
  make_edge (edge_cache, cond_bb, body_bb, 0);

  /* Special case.  Only create the edge expr_bb -> cond_bb if there really
     is a block for FOR_EXPR.  Otherwise, if the loop has a body, and since
     in this case expr_bb == cond_bb, we end up with an unnecessary
     self-referencing edge in cond_bb.  */
  if (FOR_EXPR (entry))
    make_edge (edge_cache, expr_bb, cond_bb, 0);
  make_edge (edge_cache, cond_bb, successor_block (bb), 0);
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
  basic_block body_bb;

  if (TREE_CODE (entry) != WHILE_STMT)
    abort ();

  /* Create the following edges.  The other edges will be naturally created
     by the main loop in create_edges().

          +- WHILE_STMT
	  |      |
          |      v
          |  WHILE_BODY
	  |
	  |
	  +--> EXIT
          
     If the body doesn't exist, we use the header instead.  */

  /* Basic blocks for each component.  Note that the block numbers are
     determined by make_while_stmt_blocks().  */
  body_bb = (first_exec_stmt (WHILE_BODY (entry)))
            ? BASIC_BLOCK (bb->index + 1)
	    : bb;

  make_edge (edge_cache, bb, body_bb, 0);
  make_edge (edge_cache, bb, successor_block (bb), 0);
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
	     EXIT

     If the body doesn't exist, we use the condition instead.  */

  /* Basic blocks for each component. Note that the block numbers are
     determined by make_do_stmt_blocks().  */
  cond_bb = BASIC_BLOCK (bb->index + 1);	/* This block always exists.  */

  body_bb = (first_exec_stmt (DO_BODY (entry)))
	    ? BASIC_BLOCK (bb->index + 2)
	    : cond_bb;

  make_edge (edge_cache, bb, body_bb, 0);
  make_edge (edge_cache, cond_bb, body_bb, 0);
  make_edge (edge_cache, cond_bb, successor_block (bb), 0);
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
  basic_block then_bb, else_bb;
  tree then_t, else_t;

  if (TREE_CODE (entry) != IF_STMT)
    abort ();

  /* Entry basic blocks for each component.  */
  then_t = first_exec_stmt (THEN_CLAUSE (entry));
  then_bb = (then_t) ? BB_FOR_STMT (then_t) : NULL;

  else_t = first_exec_stmt (ELSE_CLAUSE (entry));
  else_bb = (else_t) ? BB_FOR_STMT (else_t) : NULL;

  /* Create the following edges.

	      IF_STMT
		/ \
	       /   \
	    THEN   ELSE

     Either clause may be empty.  */

  if (then_bb)
    make_edge (edge_cache, bb, then_bb, 0);

  if (else_bb)
    make_edge (edge_cache, bb, else_bb, 0);

  /* We only need an edge to BB's successor block if one of the clauses is
     missing.  */
  if (then_bb == NULL || else_bb == NULL)
    make_edge (edge_cache, bb, successor_block (bb), 0);
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
	  make_edge (edge_cache, bb, target_bb, 0);
	  break;
	}

      /* Computed GOTOs.  Make an edge to every label block.  */
      else if (TREE_CODE (dest) != LABEL_DECL
	       && TREE_CODE (target) == LABEL_STMT)
	make_edge (edge_cache, bb, target_bb, 0);
    }
}

/* }}} */

/* {{{ make_break_stmt_edges()

   A break statement creates an edge from the break block to the successor
   block for the break statement's control parent.  */

static void
make_break_stmt_edges (edge_cache, bb)
     sbitmap *edge_cache;
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

  make_edge (edge_cache, bb, successor_block (control_parent), 0);
}

/* }}} */

/* {{{ make_continue_stmt_edges()

   A continue statement creates an edge from the continue block to the
   control parent's expression block.  */

static void
make_continue_stmt_edges (edge_cache, bb)
     sbitmap *edge_cache;
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

  make_edge (edge_cache, bb, condition_block (loop_bb), 0);
}

/* }}} */

/* {{{ make_return_stmt_edges()

   Create an edge from the RETURN block to EXIT_BLOCK_PTR.  */

static void
make_return_stmt_edges (edge_cache, bb)
     sbitmap *edge_cache;
     basic_block bb;
{
  tree return_t = bb->end_tree;

  if (return_t == NULL || TREE_CODE (return_t) != RETURN_STMT)
    abort ();

  make_edge (edge_cache, bb, EXIT_BLOCK_PTR, 0);
}

/* }}} */


/* Flowgraph analysis.  */

/* {{{ delete_unreachable_blocks()
   
   Delete all unreachable basic blocks.   */

static void
delete_unreachable_blocks ()
{
  int i;

  find_unreachable_blocks ();

  /* Delete all unreachable basic blocks.  Count down so that we
     don't interfere with the block renumbering that happens in
     delete_block.  */
  for (i = n_basic_blocks - 1; i >= 0; --i)
    {
      basic_block bb = BASIC_BLOCK (i);

      if (!(bb->flags & BB_REACHABLE))
	delete_block (bb);
    }
}

/* }}} */

/* {{{ delete_block()

   Remove a block from the flowgraph.  Emit a warning if -Wunreachable-code
   is set.  */

static void
delete_block (bb)
     basic_block bb;
{
  edge e, next, *q;
  tree t;

  if (warn_notreached)
    {
      tree stmt;

      /* The only nodes whose head tree is not a statement are condition
         nodes for DO_STMT and WHILE_STMT loops.  */
      if (statement_code_p (TREE_CODE (bb->head_tree)))
	stmt = bb->head_tree;
      else
	stmt = BB_PARENT (bb)->head_tree;

      prep_stmt (stmt);
      warning ("unreachable statement");
    }

  /* Remove all the instructions in the block.

     FIXME - This merely unmaps the statements.  We should remove them.  */
  t = bb->head_tree;
  while (t)
    {
      map_stmt_to_bb (t, NULL);
      if (t == bb->end_tree)
	break;
      t = TREE_CHAIN (t);
    }

  /* Remove the edges into and out of this block.  Note that there
     may indeed be edges in, if we are removing an unreachable loop.  */
  for (e = bb->pred; e; e = next)
    {
      for (q = &e->src->succ; *q != e; q = &(*q)->succ_next)
	continue;
      *q = e->succ_next;
      next = e->pred_next;
      n_edges--;
      free (e);
    }

  for (e = bb->succ; e; e = next)
    {
      for (q = &e->dest->pred; *q != e; q = &(*q)->pred_next)
	continue;
      *q = e->pred_next;
      next = e->succ_next;
      n_edges--;
      free (e);
    }

  bb->pred = NULL;
  bb->succ = NULL;

  /* Remove the basic block from the array, and compact behind it.  */
  expunge_block (bb);
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
	return condition_block (parent_bb);

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
	  || TREE_CODE (t) == SWITCH_STMT || TREE_CODE (t) == DO_STMT);
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
	  || TREE_CODE (t) == BREAK_STMT || TREE_CODE (t) == RETURN_STMT);
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
	  || TREE_CODE (t) == COMPOUND_STMT || is_ctrl_stmt (t));
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

/* {{{ condition_block()

   Returns the block containing the condition expression controlling the
   loop starting at LOOP_BB (i.e., the block containing DO_COND or
   WHILE_COND or FOR_EXPR).

   Since these blocks hold expressions, not statements, we cannot use
   BB_FOR_STMT() to determine their basic block.  Instead, we count from
   the basic block for the loop entry.
   
   ??? Note that this relies heavily on the order in which basic blocks
       get constructed, but I couldn't find another way of dealing with
       this.  */

basic_block
condition_block (loop_bb)
     basic_block loop_bb;
{
  int index;
  enum tree_code code = TREE_CODE (loop_bb->head_tree);

  if (code == FOR_STMT)
    {
      /* Usually, the loop expression in a FOR_STMT will be the second
         block after the loop entry block.  However, the FOR_EXPR block
         does not always exist.  */
      if (FOR_EXPR (loop_bb->head_tree))
	index = loop_bb->index + 2;
      else
	index = loop_bb->index + 1;
    }
  else if (code == DO_STMT)
    index = loop_bb->index + 1;
  else if (code == WHILE_STMT)
    index = loop_bb->index;
  else
    abort ();

  return BASIC_BLOCK (index);
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

/* {{{ is_exec_stmt()

   Return 1 if T is an executable statement.  */

int
is_exec_stmt (t)
     tree t;
{
  return (t
	  && statement_code_p (TREE_CODE (t))
	  && TREE_CODE (t) != COMPOUND_STMT && TREE_CODE (t) != SCOPE_STMT);
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
