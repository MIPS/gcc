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
  long num_merged_cases;
  long num_merged_labels;
  long num_failed_bind_expr_merges;
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
   the destation of computed gotos when unfactoring them.  */
static tree factored_computed_goto;

/* Basic blocks and flowgraphs.  */
static void create_blocks_annotations (void);
static void create_block_annotation (basic_block);
static void free_blocks_annotations (void);
static void clear_blocks_annotations (void);
static basic_block make_blocks (tree *, tree, tree, basic_block, tree);
static void make_cond_expr_blocks (tree *, tree, basic_block, tree);
static void make_switch_expr_blocks (tree *, tree, basic_block, tree);
static basic_block make_bind_expr_blocks (tree *, tree, basic_block, tree,
					  tree);
static inline void add_stmt_to_bb (tree *, basic_block, tree);
static inline void append_stmt_to_bb (tree *, basic_block, tree);
static inline void set_parent_stmt (tree *, tree);
static void factor_computed_gotos (void);

/* Edges.  */
static void make_edges (void);
static void make_ctrl_stmt_edges (basic_block);
static void make_exit_edges (basic_block);
static void make_cond_expr_edges (basic_block);
static void make_goto_expr_edges (basic_block);
static void make_case_label_edges (basic_block);

/* Various helpers.  */
static void assign_vars_to_scope (tree);
static basic_block successor_block (basic_block);
static tree *first_exec_stmt (tree *);
static basic_block switch_parent (basic_block);
static inline bool stmt_starts_bb_p (tree, tree);
static inline bool stmt_ends_bb_p (tree);
static void find_contained_blocks (tree *, bitmap, tree **);
static int tree_verify_flow_info (void);
static basic_block tree_make_forwarder_block (basic_block, int, int, edge, int);
static struct loops *tree_loop_optimizer_init (FILE *);
static void tree_loop_optimizer_finalize (struct loops *, FILE *);

/* Flowgraph optimization and cleanup.  */

/* Flags to pass to remove_bb to indicate which (if any) statements
   should be removed.  */
#define REMOVE_ALL_STMTS -1
#define REMOVE_NO_STMTS 0
#define REMOVE_NON_CONTROL_STRUCTS 0x1
#define REMOVE_CONTROL_STRUCTS 0x2

static void remove_unreachable_block (basic_block);
static void remove_bb (basic_block, int);
static void remove_stmt (tree *, bool);
static bool blocks_unreachable_p (bitmap);
static void remove_blocks (bitmap);
static void cleanup_control_flow (void);
static bool disconnect_unreachable_case_labels (basic_block, tree);
static edge find_taken_edge_cond_expr (basic_block, tree);
static edge find_taken_edge_switch_expr (basic_block, tree);
static bool value_matches_some_label (edge, tree, edge *);
static void linearize_control_structures (void);
static bool linearize_cond_expr (tree *, basic_block);
static void replace_stmt (tree *, tree *);
static void move_outgoing_edges (basic_block, basic_block);
static void merge_tree_blocks (basic_block, basic_block);
static bool remap_stmts (basic_block, basic_block, tree *);
static tree *handle_switch_split (basic_block, basic_block);
static tree *handle_switch_fallthru (tree, basic_block, basic_block);

/* Block iterator helpers.  */
static void remove_bsi_from_block (block_stmt_iterator *, bool);
static block_stmt_iterator bsi_init (tree *, basic_block);
static inline void bsi_update_from_tsi
		(block_stmt_iterator *, tree_stmt_iterator);
static tree_stmt_iterator bsi_link_after
		(tree_stmt_iterator *, tree, basic_block, tree);

/* Values returned by location parameter of find_insert_location.  */

enum find_location_action {
  EDGE_INSERT_LOCATION_BEFORE,
  EDGE_INSERT_LOCATION_AFTER,
  EDGE_INSERT_LOCATION_THEN,
  EDGE_INSERT_LOCATION_ELSE,
  EDGE_INSERT_LOCATION_NEW_ELSE,
  EDGE_INSERT_LOCATION_BSI_AFTER };

static tree_stmt_iterator find_insert_location
	(basic_block, basic_block, basic_block, enum find_location_action *);

/* Location to track pending stmt for edge insertion.  */
#define PENDING_STMT(e)	((tree)(e->insns))

/* Set the pending stmt field.  */
#define SET_PENDING_STMT(e, t)	((e->insns) = (rtx)t)


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

/* FIXME These need to be filled in with appropriate pointers.  But this
   implies an ABI change in some functions.  */
struct cfg_hooks tree_cfg_hooks = {
  tree_verify_flow_info,
  tree_dump_bb,			/* dump_bb  */
  NULL,				/* create_basic_block  */
  NULL,				/* redirect_edge_and_branch  */
  NULL,				/* redirect_edge_and_branch_force  */
  NULL,				/* delete_basic_block  */
  NULL,				/* split_block  */
  NULL,				/* can_merge_blocks_p  */
  NULL,				/* merge_blocks  */
  tree_split_edge,		/* cfgh_split_edge  */
  tree_make_forwarder_block,	/* cfgh_make_forward_block  */
  tree_loop_optimizer_init,     /* cfgh_loop_optimizer_init  */
  tree_loop_optimizer_finalize  /* cfgh_loop_optimizer_finalize  */
};

/*---------------------------------------------------------------------------
			      Create basic blocks
---------------------------------------------------------------------------*/

/* Entry point to the CFG builder for trees.  FNBODY is the body of the
   function to process.  */

void
build_tree_cfg (tree fnbody)
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

  /* Find the basic blocks for the flowgraph.  Ignore empty functions.  */
  if (IS_EMPTY_STMT (fnbody) || TREE_CODE (fnbody) != BIND_EXPR)
    {
      timevar_pop (TV_TREE_CFG);
      return;
    }

  first_p = first_exec_stmt (&BIND_EXPR_BODY (fnbody));
  if (first_p)
    {
      found_computed_goto = 0;
      make_blocks (first_p, NULL_TREE, NULL, NULL, fnbody);

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
factor_computed_gotos ()
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
      if (is_computed_goto (last))
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
	      factored_label_decl
		= build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
	      DECL_CONTEXT (factored_label_decl) = current_function_decl;
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
	      make_blocks (&TREE_OPERAND (compound, 0), NULL, NULL, NULL, NULL);

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

   PARENT_STMT is the entry statement for the control structure immediately
      enclosing the new sub-graph.

   BB is the block where the statements should be added to.  If BB is NULL,
      a new basic block will be created for the statements.

   SCOPE is the BIND_EXPR block containing *FIRST_P.

   Return the last basic block added to the graph.  This is used to know if
   a recursive invocation built a sub-graph whose last block can accept
   more statements or not.  */

static basic_block
make_blocks (tree *first_p, tree next_block_link, tree parent_stmt,
	     basic_block bb, tree scope)
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
	  bb = create_bb ();
	  start_new_block = false;
	}

      NEXT_BLOCK_LINK (stmt) = NULL_TREE;
      code = TREE_CODE (stmt);

      /* Now add STMT to BB and create the subgraphs for special statement
	 codes.  */
      append_stmt_to_bb (stmt_p, bb, parent_stmt);
      get_stmt_ann (stmt)->scope = scope;

      if (is_computed_goto (*stmt_p))
	found_computed_goto = true;

      if (code == COND_EXPR)
	make_cond_expr_blocks (stmt_p, next_block_link, bb, scope);
      else if (code == SWITCH_EXPR)
	make_switch_expr_blocks (stmt_p, next_block_link, bb, scope);
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
	  assign_vars_to_scope (stmt);
	  get_stmt_ann (stmt)->scope_level =
		  get_stmt_ann (scope)->scope_level + 1;

	  last_bb = make_bind_expr_blocks (stmt_p, next_block_link, bb,
	                                   parent_stmt, stmt);
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
	      if (!stmt || !stmt_ends_bb_p (stmt))
		cfg_stats.num_failed_bind_expr_merges++;
	    }
	}

      /* If STMT is a basic block terminator, set START_NEW_BLOCK for the
	 next iteration.  Also compute any reachable exception handlers
	 for STMT.  */
      if (stmt && stmt_ends_bb_p (stmt))
	start_new_block = true;

      last = stmt;
    }

  /* If LAST is set, link it to NEXT_BLOCK_LINK.  This allows making edges
     from the last block inside a lexical scope (see successor_block).  */
  if (last)
    {
      NEXT_BLOCK_LINK (last) = next_block_link;
      return bb_for_stmt (last);
    }

  return NULL;
}

/* Create the blocks for the COND_EXPR node pointed by COND_P.

   NEXT_BLOCK_LINK is the first statement of the successor basic block for
      the block holding *COND_P.  If *COND_P is the last statement inside a
      lexical scope, this will be the statement that comes after COND_P's
      container (see the documentation for NEXT_BLOCK_LINK).

   ENTRY is the block whose last statement is *COND_P.
   
   SCOPE is the BIND_EXPR node holding *COND_P.  */

static void
make_cond_expr_blocks (tree *cond_p, tree next_block_link,
		       basic_block entry, tree scope)
{
  tree_stmt_iterator si;
  tree cond = *cond_p;
  entry->flags |= BB_CONTROL_STRUCTURE;

  /* Determine NEXT_BLOCK_LINK for statements inside the COND_EXPR body.  */
  si = tsi_start (cond_p);
  tsi_next (&si);

  /* Ignore any empty statements at the tail of this tree.  */
  while (!tsi_end_p (si) && tsi_stmt (si) == NULL)
    tsi_next (&si);

  if (!tsi_end_p (si) && tsi_stmt (si) != NULL_TREE)
    next_block_link = *(tsi_container (si));

  STRIP_CONTAINERS (cond);
  make_blocks (&COND_EXPR_THEN (cond), next_block_link, cond, NULL, scope);
  make_blocks (&COND_EXPR_ELSE (cond), next_block_link, cond, NULL, scope);
}

/* Create the blocks for the SWITCH_EXPR node pointed by SWITCH_E_P.

   NEXT_BLOCK_LINK is the first statement of the successor basic block for
      the block holding *SWITCH_E_P.  If *SWITCH_E_P is the last statement
      inside a lexical scope, this will be the statement that comes after
      *SWITCH_E_P's container (see the documentation for NEXT_BLOCK_LINK).

   ENTRY is the block whose last statement is *SWITCH_E_P.
   
   SCOPE is the BIND_EXPR node holding *SWITCH_E_P.  */

static void
make_switch_expr_blocks (tree *switch_e_p, tree next_block_link,
			 basic_block entry, tree scope)
{
  tree_stmt_iterator si;
  tree switch_e = *switch_e_p;
  entry->flags |= BB_CONTROL_STRUCTURE;

  /* Determine NEXT_BLOCK_LINK for statements inside the COND_EXPR body.  */
  si = tsi_start (switch_e_p);
  tsi_next (&si);

  /* Ignore any empty statements at the tail of this tree.  */
  while (!tsi_end_p (si) && tsi_stmt (si) == NULL)
    tsi_next (&si);

  if (!tsi_end_p (si) && tsi_stmt (si) != NULL_TREE)
    next_block_link = *(tsi_container (si));

  STRIP_CONTAINERS (switch_e);
  make_blocks (&SWITCH_BODY (switch_e), next_block_link, switch_e, NULL, scope);
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
   not.
   
   SCOPE is the BIND_EXPR node holding *BIND_P (in fact it is equal to
   *BIND_P).  */

static basic_block
make_bind_expr_blocks (tree *bind_p, tree next_block_link,
		       basic_block entry, tree parent_stmt, tree scope)
{
  tree_stmt_iterator si;
  tree bind = *bind_p;

  /* Determine NEXT_BLOCK_LINK for statements inside the BIND_EXPR
     body.  */
  si = tsi_start (bind_p);
  tsi_next (&si);

  /* Ignore any empty statements at the tail of this tree.  */
  while (!tsi_end_p (si) && tsi_stmt (si) == NULL)
    tsi_next (&si);

  if (!tsi_end_p (si) && tsi_stmt (si) != NULL_TREE)
    next_block_link = *(tsi_container (si));

  /* By passing the current block ENTRY to make_blocks, we will keep adding
     statements to ENTRY until we find a block terminating statement inside
     the body of the BIND_EXPR.  On return from make_blocks, our caller
     will start a new basic block only if the body of the BIND_EXPR node
     ends with a block terminating statement.  */
  STRIP_CONTAINERS (bind);
  return make_blocks (&BIND_EXPR_BODY (bind), next_block_link, parent_stmt,
		      entry, scope);
}


/* Set PARENT_STMT to be the control structure that contains the statement
   pointed by STMT_P.  */

static inline void
set_parent_stmt (tree *stmt_p, tree parent_stmt)
{
  tree t;

  /* Associate *STMT_P (and the trees it contains) to its control parent.  */
  t = *stmt_p;
  do
    {
      stmt_ann_t ann = stmt_ann (t);
      ann->parent_stmt = parent_stmt;
      t = (TREE_CODE (t) == COMPOUND_EXPR) ? TREE_OPERAND (t, 0) : NULL_TREE;
    }
  while (t);
}


/* Add statement pointed by STMT_P to basic block BB.  PARENT_STMT is the
   entry statement to the control structure holding *STMT_P.  If parent
   is passed a NULL, this routine will try to pick up the parent from the
   first statement in the block.  */

static inline void
add_stmt_to_bb (tree *stmt_p, basic_block bb, tree parent)
{
  set_bb_for_stmt (*stmt_p, bb);

  /* Try to determine the parent if there isn't one.  */
  if (parent == NULL_TREE && bb->head_tree_p != NULL)
    parent = parent_stmt (*bb->head_tree_p);

  set_parent_stmt (stmt_p, parent);
}


/* Add statement pointed by STMT_P to basic block BB and update BB's
   boundaries accordingly.  PARENT_STMT is the entry statement to the
   control structure holding *STMT_P.  */

static inline void
append_stmt_to_bb (tree *stmt_p, basic_block bb, tree parent)
{
  add_stmt_to_bb (stmt_p, bb, parent);

  /* Update the head and tail of the block.  */
  if (bb->head_tree_p == NULL)
    bb->head_tree_p = stmt_p;

  bb->end_tree_p = stmt_p;
}


/* Add statement pointed by STMT_P to basic block BB and update BB's
   boundaries accordingly.  PARENT_STMT is the entry statement to the
   control structure holding *STMT_P.  */

static inline void
prepend_stmt_to_bb (tree *stmt_p, basic_block bb, tree parent)
{
  add_stmt_to_bb (stmt_p, bb, parent);

  /* Update the head and tail of the block.  */
  bb->head_tree_p = stmt_p;

  if (bb->end_tree_p == NULL)
    bb->end_tree_p = stmt_p;
}


/* Create and return a new basic block.  */

basic_block
create_bb (void)
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

  /* We do not care about fake edges, so remove any that the CFG
     builder inserted for completeness.  */
  remove_fake_edges ();

  /* Clean up the graph and warn for unreachable code.  */
  cleanup_tree_cfg ();
}

/* Find all the basic blocks contained within *STMT_P and its children
   and mark them in MY_BLOCKS.  For each outgoing edge in MY_BLOCKS,
   mark the destination of the edge in MY_TARGETS.  Also record the
   last statement processed in *last_p.  */

static void
find_contained_blocks (tree *stmt_p, bitmap my_blocks, tree **last_p)
{
  tree_stmt_iterator tsi;

  for (tsi = tsi_start (stmt_p); !tsi_end_p (tsi); tsi_next (&tsi))
    {
      tree stmt;
      enum tree_code code;
      basic_block bb;

      stmt = tsi_stmt (tsi);
      if (!stmt || ! stmt_ann (stmt))
	break;

      /* Keep track of the last statement we've processed.  */
      *last_p = tsi_stmt_ptr (tsi);

      /* Mark this statement's block as being contained.  */
      bb = bb_for_stmt (stmt);
      if (bb)
	bitmap_set_bit (my_blocks, bb->index);

      /* And recurse down into control structures.  */
      code = TREE_CODE (stmt);
      if (code == COND_EXPR)
	{
	  find_contained_blocks (&COND_EXPR_THEN (stmt), my_blocks, last_p);
	  find_contained_blocks (&COND_EXPR_ELSE (stmt), my_blocks, last_p);
	}
      else if (code == CATCH_EXPR)
	{
	  find_contained_blocks (&CATCH_BODY (stmt), my_blocks, last_p);
	}
      else if (code == EH_FILTER_EXPR)
	{
	  find_contained_blocks (&EH_FILTER_FAILURE (stmt), my_blocks, last_p);
	}
      else if (code == TRY_CATCH_EXPR)
	{
	  tree *save_last_p;
	  find_contained_blocks (&TREE_OPERAND (stmt, 0), my_blocks, last_p);

	  /* We do not want to include statements in the CATCH block
	     when determining the last executed statement.  FIXME,
	     what would probably work better would be a to include
	     an empty block at the end of each FINALLY block and
	     use it as the last statement.

	     I worry that we do the wrong thing with ELSE clauses,
	     and other control structures.  */
	  save_last_p = *last_p;
	  find_contained_blocks (&TREE_OPERAND (stmt, 1), my_blocks, last_p);
	  *last_p = save_last_p;
	}
      else if (code == TRY_FINALLY_EXPR
	       || code == COMPOUND_EXPR)
	{
	  find_contained_blocks (&TREE_OPERAND (stmt, 0), my_blocks, last_p);
	  find_contained_blocks (&TREE_OPERAND (stmt, 1), my_blocks, last_p);
	}
      else if (code == SWITCH_EXPR)
	{
	  find_contained_blocks (&SWITCH_BODY (stmt), my_blocks, last_p);
	}
      else if (code == BIND_EXPR)
	{
	  find_contained_blocks (&BIND_EXPR_BODY (stmt), my_blocks, last_p);
	}
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
      make_edge (bb, successor_block (bb), EDGE_FALLTHRU);
      break;

    case MODIFY_EXPR:
      /* A MODIFY_EXPR may have a CALL_EXPR on its RHS and the CALL_EXPR
	 may have an abnormal edge.  Search the RHS for this case and
	 create any required edges.  */
      if (TREE_CODE (TREE_OPERAND (last, 1)) == CALL_EXPR
	  && FUNCTION_RECEIVES_NONLOCAL_GOTO (current_function_decl))
	make_goto_expr_edges (bb);

      make_eh_edges (last);
      make_edge (bb, successor_block (bb), EDGE_FALLTHRU);
      break;

    default:
      abort ();
    }
}



/* Create the edges for a COND_EXPR starting at block BB.

   Create the following edges.

	     COND_EXPR
		/ \
	       /   \
	    THEN   ELSE

   Either clause may be empty.  */

static void
make_cond_expr_edges (basic_block bb)
{
  tree entry = last_stmt (bb);
  basic_block successor_bb, then_bb, else_bb;

#if defined ENABLE_CHECKING
  if (entry == NULL_TREE || TREE_CODE (entry) != COND_EXPR)
    abort ();
#endif

  /* Entry basic blocks for each component.  */
  then_bb = bb_for_stmt (COND_EXPR_THEN (entry));
  else_bb = bb_for_stmt (COND_EXPR_ELSE (entry));
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
make_case_label_edges (basic_block bb)
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
cleanup_tree_cfg (void)
{
  int orig_n_basic_blocks = n_basic_blocks;

  timevar_push (TV_TREE_CLEANUP_CFG);
  pdom_info = NULL;

  cleanup_control_flow ();
  remove_unreachable_blocks ();
  linearize_control_structures ();
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
  else
    {
      /* We are at the end of this tree, we may still have
	 an unnecessary GOTO_EXPR if NEXT_BLOCK_LINK
	 points to the target label.  */
      tree next_block_link = NEXT_BLOCK_LINK (*stmt_p);

      if (next_block_link)
	{
	  tree next_stmt;

	  /* Get the statement at NEXT_BLOCK_LINK and see if it
	     is our target label.  */
	  next_stmt = tsi_stmt (tsi_start (&next_block_link));
	  if (next_stmt
	      && TREE_CODE (next_stmt) == LABEL_EXPR
	      && (LABEL_EXPR_LABEL (next_stmt)
		  == GOTO_DESTINATION (*stmt_p)))
	    {
	      data->repeat = true;
	      *stmt_p = build_empty_stmt ();
	      return;
	    }
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
	case SWITCH_EXPR:
	  remove_useless_stmts_and_vars_1 (&SWITCH_BODY (*stmt_p), data);
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

  /* Remove unreachable blocks in reverse.  That will expose more unnecessary
     COMPOUND_EXPRs that we can remove.  */
  for (i = last_basic_block - 1; i >= 0; i--)
    {
      bb = BASIC_BLOCK (i);

      /* The block may have been removed in a previous iteration if it was
	 inside an unreachable control structure.  */
      if (bb == NULL)
	continue;

      if (!(bb->flags & BB_REACHABLE))
	{
	  remove_unreachable_block (bb);
	  ret = true;
	}
    }

  return ret;
}


/* Helper for remove_unreachable_blocks.  */

static void
remove_unreachable_block (basic_block bb)
{
  if (bb->flags & BB_CONTROL_STRUCTURE)
    {
      tree *last_p = last_stmt_ptr (bb);
      tree *dummy_p;
      bitmap subblocks = BITMAP_XMALLOC ();

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
      find_contained_blocks (last_p, subblocks, &dummy_p);
      if (blocks_unreachable_p (subblocks))
	{
	  remove_blocks (subblocks);
	}
      else
	{

	  /* If we have reachable subblocks, then we can not remove
	     control statements.  But we also can't simply leave them
	     as-is since they may refer to SSA variables which will
	     not get renamed since this block has become unreachable.

	     So cleanup any variable references in toplevel control
	     structures.  This may or may not be sufficient.  */
	  if (TREE_CODE (*last_p) == COND_EXPR)
	    COND_EXPR_COND (*last_p) = integer_zero_node;
	  else if (TREE_CODE (*last_p) == SWITCH_EXPR)
	    SWITCH_COND (*last_p) = integer_zero_node;
	  remove_bb (bb, REMOVE_NON_CONTROL_STRUCTS);
	}

      BITMAP_XFREE (subblocks);
    }
  else
    remove_bb (bb, REMOVE_ALL_STMTS);
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

/* Remove block BB and its statements from the flowgraph.  REMOVE_STMTS is
   nonzero if the statements in BB should also be removed.

   Note that if REMOVE_STMTS is nonzero and BB is the entry block for a
   compound statement (control structures or blocks of code), removing BB
   will effectively remove the whole structure from the program.  The
   caller is responsible for making sure that all the blocks in the
   compound structure are also removed.  */

static void
remove_bb (basic_block bb, int remove_stmt_flags)
{
  block_stmt_iterator i;
  bsi_list_p stack;
  location_t loc;

  dump_file = dump_begin (TDI_cfg, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "Removing basic block %d\n", bb->index);
      dump_bb (bb, dump_file, 0);
      fprintf (dump_file, "\n");
      dump_end (TDI_cfg, dump_file);
      dump_file = NULL;
    }

  /* Remove all the instructions in the block.  Do so in reverse order
     so that we remove all the containing COMPOUND_EXPRs as well.  */
  FOR_EACH_BSI_IN_REVERSE (stack, bb, i)
    {
      tree stmt = bsi_stmt (i);

      set_bb_for_stmt (stmt, NULL);
      if (remove_stmt_flags)
        {
	  int ctrl_struct = is_ctrl_structure (stmt);

	  loc.file = get_filename (stmt);
	  loc.line = get_lineno (stmt);
	  if ((ctrl_struct && (remove_stmt_flags & REMOVE_CONTROL_STRUCTS))
	      || (!ctrl_struct && (remove_stmt_flags & REMOVE_NON_CONTROL_STRUCTS)))
	    bsi_remove (&i);
        }
    }

  /* If requested, give a warning that the first statement in the
     block is unreachable.  We walk statements backwards in the
     loop above, so the last statement we process is the first statement
     in the block.  */
  if (remove_stmt_flags && warn_notreached)
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


/* Remove all the blocks in bitmap BLOCKS.  */

static void
remove_blocks (bitmap blocks)
{
  size_t i;

  EXECUTE_IF_SET_IN_BITMAP (blocks, 0, i,
    {
      basic_block bb = BASIC_BLOCK (i);

      if (bb && bb->index != INVALID_BLOCK)     
	remove_bb (bb, REMOVE_ALL_STMTS);
    });
}


/* Return true if all the blocks in bitmap BLOCKS are unreachable.  */

static bool
blocks_unreachable_p (bitmap blocks)
{
  size_t i;

  EXECUTE_IF_SET_IN_BITMAP (blocks, 0, i,
    {
      basic_block bb = BASIC_BLOCK (i);

      if (bb && bb->index != INVALID_BLOCK && bb->flags & BB_REACHABLE)
	return false;
    });

  return true;
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
  tree parent = parent_stmt (stmt);
  int update_head = 0;
  int update_end = 0;

  /* If the statement is a control structure, clear the appropriate BB_*
     flags from the basic block.  */
  if (bb && is_ctrl_structure (stmt))
    bb->flags &= ~BB_CONTROL_STRUCTURE;

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
    add_stmt_to_bb (stmt_p, bb, parent);

  if (update_head)
    bb->head_tree_p = stmt_p;

  if (update_end)
    bb->end_tree_p = stmt_p;
}


/* Try to remove superfluous control structures.  */

static void
cleanup_control_flow (void)
{
  basic_block bb;

  FOR_EACH_BB (bb)
    if (bb->flags & BB_CONTROL_STRUCTURE)
      {
	tree last = last_stmt (bb);
	if (last)
	  {
	    enum tree_code code = TREE_CODE (last);
	    if (code == COND_EXPR)
	      cleanup_cond_expr_graph (bb, last);
	    else if (code == SWITCH_EXPR)
	      cleanup_switch_expr_graph (bb, last);
	  }
      }
}


/* Disconnect an unreachable block in the conditional expression starting
   at block BB.

   If the predicate of the COND_EXPR node in block BB is constant,
   disconnect the subgraph that contains the clause that is never executed.  */

bool
cleanup_cond_expr_graph (basic_block bb, tree cond_expr)
{
  tree val;
  edge taken_edge;
  bool retval = false;

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
	      ssa_remove_edge (e);
	      retval = true;
	    }
	}
    }
  return retval;
}


/* Disconnect unreachable blocks in the 'switch' expression starting at
   block SWITCH_BB.

   If the switch condition of the SWITCH_EXPR node in block SWITCH_BB is
   constant, disconnect all the subgraphs for all the case labels that will
   never be taken.  */

bool
cleanup_switch_expr_graph (basic_block switch_bb, tree switch_expr)
{
  int found = 0;
  edge e;
  bool retval;

#if defined ENABLE_CHECKING
  if (switch_expr == NULL_TREE || TREE_CODE (switch_expr) != SWITCH_EXPR)
    abort ();
#endif

  retval = disconnect_unreachable_case_labels (switch_bb, switch_expr);

  /* If the switch() has a default label, remove the fallthru edge that was
     created when we processed the entry block for the switch() statement.  */
  for (e = switch_bb->succ; e && !found; e = e->succ_next)
    {
      block_stmt_iterator bsi;
      for (bsi = bsi_start (e->dest); !bsi_end_p (bsi); bsi_next (&bsi))
        {
	  tree t = bsi_stmt (bsi);
          if (TREE_CODE (t) != CASE_LABEL_EXPR)
	    break;
	  if (CASE_LOW (t) == NULL_TREE)
	    {
	      basic_block chain_bb = successor_block (switch_bb);
	      edge e = find_edge (switch_bb, chain_bb);
	      if (e)
		{
		  ssa_remove_edge (e);
		  retval = true;
		}
	      found = 1;
	      break;
	    }
	}
    }
  return retval;
}


/* Clean up the 'switch' expression at block BB.

   If the switch() statement starting at basic block BB has a constant
   condition, disconnect all the unreachable case labels.  */

static bool
disconnect_unreachable_case_labels (basic_block bb, tree t)
{
  edge taken_edge;
  tree switch_val;
  bool retval = false;

  if (t == NULL_TREE)
    return retval;

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
	      ssa_remove_edge (e);
	      retval = true;
	    }
	}
    }
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
  if (stmt == NULL_TREE || !is_ctrl_structure (stmt))
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

  /* If E is not going to the THEN nor the ELSE clause, then it's
     the fallthru edge to the successor block of the if() block.  */
  return find_edge (bb, successor_block (bb));
}


/* Given a constant value VAL and the entry block BB to a SWITCH_EXPR
   statement, determine which edge will be taken out of the block.  Return
   NULL if any edge may be taken.  */

static edge
find_taken_edge_switch_expr (basic_block bb, tree val)
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
value_matches_some_label (edge dest_edge, tree val, edge *default_edge_p)
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
	 label matches the switch() value.  If we do find a match, we
	 are done.  */
      if (CASE_LOW (stmt) == NULL_TREE)
	*default_edge_p = dest_edge;
      else if (CASE_HIGH (stmt) == NULL_TREE)
	{
	  /* A `normal' case label.  */
	  tree label_val = CASE_LOW (stmt);
	  if (simple_cst_equal (label_val, val) == 1)
	    return true;
	}
      else
	{
	  /* A case range.  We can only handle integer ranges.  */
	  if (tree_int_cst_compare (CASE_LOW (stmt), val) <= 0
	      && tree_int_cst_compare (CASE_HIGH (stmt), val) >= 0)
	    return true;
	}
    }

  return false;
}


/* Convert control structures into linear code whenever possible.  */

static void
linearize_control_structures (void)
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      tree *entry_p;

      if (!(bb->flags & BB_CONTROL_STRUCTURE))
	continue;

      /* After converting the current COND_EXPR into straight line code it
	 may happen that the block that was merged into BB also ends in a
	 COND_EXPR (nested conditionals).  Therefore, we need to iterate
	 until we either fail to linearize the conditional or BB ends in
	 something other than a conditional.  */
      entry_p = last_stmt_ptr (bb);
      while (entry_p
	     && TREE_CODE (*entry_p) == COND_EXPR
	     && linearize_cond_expr (entry_p, bb))
	entry_p = last_stmt_ptr (bb);
    }
}

/* If all the phi nodes in PHI have alternatives for BB1 and BB2 and
   those alterantives are equal in each of the PHI nodes, then return
   nonzero, else return zero.  */

static int
phi_alternatives_equal (tree phi, basic_block bb1, basic_block bb2)
{
  /* Walk through each PHI nodes on the chain.  */
  for ( ; phi ; phi = TREE_CHAIN (phi))
    {
      int i;
      tree val1 = NULL;
      tree val2 = NULL;

      /* Find the alterantive associated with BB1 and BB2.  Quit when we
	 have found both or we exhaust the alternatives.  */
      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	{
	  if (PHI_ARG_EDGE (phi, i)->src == bb1)
	    val1 = PHI_ARG_DEF (phi, i);
	  if (PHI_ARG_EDGE (phi, i)->src == bb2)
	    val2 = PHI_ARG_DEF (phi, i);

	  if (val1 && val2)
	    break;
	}

      /* If we exhaused the alternatives or the alternatives found are
	 not equal, then return false.  */
      if (i == PHI_NUM_ARGS (phi)
	  || ! operand_equal_p (val1, val2, 0))
	return false;
    }

  return true;
}

/* Convert conditional expressions of the form 'if (1)' and 'if (0)' into
   straight line code.  ENTRY_P is a pointer to the COND_EXPR statement to
   check.  Return true if the conditional was modified.  */

static bool
linearize_cond_expr (tree *entry_p, basic_block bb)
{
  basic_block pdom_bb;
  tree entry = *entry_p;
  tree pred = COND_EXPR_COND (entry);
  tree then_clause = COND_EXPR_THEN (entry);
  tree else_clause = COND_EXPR_ELSE (entry);
  basic_block then_block = bb_for_stmt (then_clause);
  basic_block else_block = bb_for_stmt (else_clause);
  int always_true = (simple_cst_equal (pred, integer_one_node) == 1);
  int always_false = (simple_cst_equal (pred, integer_zero_node) == 1);

  /* Remove the conditional if both branches have been removed.  */
  if (body_is_empty (then_clause) && body_is_empty (else_clause))
    {
      /* Calculate dominance info, if it hasn't been computed yet.  */
      if (pdom_info == NULL)
	pdom_info = calculate_dominance_info (CDI_POST_DOMINATORS);
      pdom_bb = get_immediate_dominator (pdom_info, bb);

      /* If there is no post dominator, or the post dominator has no
	 PHI nodes, or the PHI node alternatives are equal, then we
	 can eliminate this conditional.  */
      if (!pdom_bb
	  || !phi_nodes (pdom_bb)
	  || phi_alternatives_equal (phi_nodes (pdom_bb),
				     then_block, else_block))
        {
	  /* While neither arm of the conditional has any code, there
	     may still be important edges attached to those arms such
	     as the backedge in a loop, or exception handling related
	     edges (the common characteristic is they are edges implied
	     by control structures which are not explicitly represented
	     in the IL).  */
	  if ((always_true || ! always_false) && then_block)
	    move_outgoing_edges (bb, then_block);

	  if ((always_false || ! always_true) && else_block)
	    move_outgoing_edges (bb, else_block);

	  /* Now that we've moved all the edges, go ahead and remove
	     the disconnected blocks.  Note this will remove any edges
	     from BB to the disconnected blocks.  */
	  if (then_block)
	    remove_bb (then_block, REMOVE_NO_STMTS);
	  if (else_block)
	    remove_bb (else_block, REMOVE_NO_STMTS);

	  /* And finally remove the useless statement.  */
	  remove_stmt (entry_p, true);
	  return true;
	}
    }

  /* There should be no other entry edges into the branches, otherwise
     merging the blocks would be an error.  */
  if ((then_block && then_block->pred->pred_next)
      || (else_block && else_block->pred->pred_next))
    return false;
  
  /* Linearize 'if (1)'.  */
  if (always_true && body_is_empty (else_clause))
    {
      /* If there is no THEN_CLAUSE, remove the conditional.  */
      if (body_is_empty (then_clause))
	{
	  if (then_block)
	    {
	      move_outgoing_edges (bb, then_block);
	      remove_bb (then_block, REMOVE_NO_STMTS);
	    }
	  remove_stmt (entry_p, true);
	}
      else
	merge_tree_blocks (bb, bb_for_stmt (then_clause));

      return true;
    }

  /* Linearize 'if (0)'.  */
  if (always_false && body_is_empty (then_clause))
    {
      /* If there is no ELSE_CLAUSE, remove the conditional.  */
      if (body_is_empty (else_clause))
	{
	  if (else_block)
	    {
	      move_outgoing_edges (bb, else_block);
	      remove_bb (else_block, REMOVE_NO_STMTS);
	    }
	  remove_stmt (entry_p, true);
	}
      else
	merge_tree_blocks (bb, bb_for_stmt (else_clause));

      return true;
    }

  return false;
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
  char *s_indent;
  block_stmt_iterator si;
  tree phi;

  s_indent = (char *) alloca ((size_t) indent + 1);
  memset ((void *) s_indent, ' ', (size_t) indent);
  s_indent[indent] = '\0';

  fprintf (outf, ";;%s parent:     ", s_indent);
  if (bb->tree_annotations && parent_block (bb))
    fprintf (outf, "%d\n", parent_block (bb)->index);
  else
    fputs ("nil\n", outf);

  if (bb->tree_annotations)
    for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
      {
	fprintf (outf, "%s# ", s_indent);
	print_generic_stmt (outf, phi, 0);
      }

  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      fprintf (outf, "%s%d  ", s_indent, get_lineno (bsi_stmt (si)));
      print_generic_stmt (outf, bsi_stmt (si), TDF_SLIM);
    }
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
	  dump_bb (bb, file, 0);
	  fputc ('\n', file);
	}
    }

  if (flags & TDF_STATS)
    dump_cfg_stats (file);

  if (n_basic_blocks > 0)
    dump_function_to_file (current_function_decl, file, flags|TDF_BLOCKS);
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
successor_block (basic_block bb)
{
  basic_block succ_bb;
  tree_stmt_iterator i;
  tree last_stmt;
  tree *container_p;

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
    i = tsi_start (&BIND_EXPR_BODY (last_stmt));
  else
    tsi_next (&i);

  container_p = tsi_container (i);
  if (container_p)
    {
      succ_bb = bb_for_stmt (*container_p);
      if (succ_bb)
	return succ_bb;
    }

  /* We couldn't find a successor for BB.  This means that BB is the last
     block inside a control structure or lexical scope.  Use the
     NEXT_BLOCK_LINK for BB's last statement.  If NEXT_BLOCK_LINK is still
     NULL, then BB is the last basic block in the function.  In which case
     we have reached the end of the flowgraph and return EXIT_BLOCK_PTR.  */
  if (last_stmt == NULL_TREE)
    last_stmt = *bb->end_tree_p;

  if (NEXT_BLOCK_LINK (last_stmt))
    return bb_for_stmt (NEXT_BLOCK_LINK (last_stmt));
  else
    return EXIT_BLOCK_PTR;
}


/* Return true if T represents a control structure.  */

bool
is_ctrl_structure (tree t)
{
#if defined ENABLE_CHECKING
  if (t == NULL)
    abort ();
#endif

  return (TREE_CODE (t) == COND_EXPR
	  || TREE_CODE (t) == CATCH_EXPR
	  || TREE_CODE (t) == EH_FILTER_EXPR
	  || TREE_CODE (t) == TRY_CATCH_EXPR
	  || TREE_CODE (t) == TRY_FINALLY_EXPR
	  || TREE_CODE (t) == SWITCH_EXPR);
}

/* Return true if T represents a stmt that always transfers control.  */

bool
is_ctrl_stmt (tree t)
{
  return (is_ctrl_structure (t)
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
is_computed_goto (tree t)
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

/* Return the header block for the innermost switch statement containing
   BB.  Return NULL if BB is not inside a switch statement.  */

static basic_block
switch_parent (basic_block bb)
{
  tree parent = parent_stmt (last_stmt (bb));

  while (parent && TREE_CODE (parent) != SWITCH_EXPR)
    parent = parent_stmt (parent);

  return (parent) ? bb_for_stmt (parent) : NULL;
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
   specified basic block, then return an empty iterator.  If the first
   real stmt is contained in a BIND_EXPR, descend into the BIND_EXPR and
   set up the context chains properly. */

static block_stmt_iterator
bsi_init (tree *tp, basic_block bb)
{
  block_stmt_iterator i, bind;
  tree stmt;

  i.tp = tp;
  i.context = NULL_TREE;
  /* If the first statement is empty, get the next non-empty one.  */
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
   empty statement nodes inside a basic block.  */

void
bsi_next_in_bb (block_stmt_iterator *i, basic_block bb)
{
  tree t, stmt = NULL_TREE;
  block_stmt_iterator bind;

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
	}
      *i = bind;
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

/* Note this routine is a bit ugly. Since BIND_EXPRs dont cause new block,
   the block iterator keeps a stack of BIND_EXPRs which have been decended
   into.  In order to create this stack properly, this routine traverses
   through the block until it finds the specified tsi stmt.  */

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
bsi_link_after (tree_stmt_iterator *this_tsi, tree t,
		basic_block curr_bb, tree parent)
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
      add_stmt_to_bb (tsi_container (same_tsi), curr_bb, parent);
    }
  *this_tsi = same_tsi;
  tsi_next (this_tsi);
  add_stmt_to_bb (tsi_container (*this_tsi), curr_bb, parent);

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
  tree parent;

  curr_container = bsi_container (*curr_bsi);
  if (curr_container)
    {
      curr_stmt = bsi_stmt (*curr_bsi);
      curr_bb = bb_for_stmt (curr_stmt);
      parent = parent_stmt (curr_stmt);
    }
  else
    {
      curr_stmt = NULL_TREE;
      parent = NULL_TREE;

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
      /* An empty block should have only one successor, so try to find the
         parent block from it.  */
      edge succ;

      succ = curr_bb->succ;
      if (succ->succ_next != NULL)
        abort ();

      if (curr_bb->head_tree_p == NULL)
        abort ();
      if (succ->dest != EXIT_BLOCK_PTR)
	parent = parent_stmt (*(succ->dest->head_tree_p));

      inserted_tsi = tsi_start (curr_bb->head_tree_p);
      tsi_link_before (&inserted_tsi, t, TSI_NEW_STMT);
      prepend_stmt_to_bb (tsi_container (inserted_tsi), curr_bb, parent);

      /* In this case, we will *always* return the new stmt since BSI_SAME_STMT
         doesn't really exist.  */
      *curr_bsi = bsi_from_tsi (inserted_tsi);
    }
  else
    {
      inserted_tsi = tsi_from_bsi (*curr_bsi);

      same_tsi = bsi_link_after (&inserted_tsi, t, curr_bb, parent);
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
  tree parent;

  curr_container = bsi_container (*curr_bsi);

  /* If this block is empty, let bsi_insert_after() handle it.  */
  if (curr_container == NULL || bsi_stmt (*curr_bsi) == NULL_TREE)
    bsi_insert_after (curr_bsi, t, mode);

  curr_stmt = bsi_stmt (*curr_bsi);
  curr_bb = bb_for_stmt (curr_stmt);
  parent = parent_stmt (curr_stmt);
  inserted_tsi = tsi_from_bsi (*curr_bsi);

  /* The only case that needs attention is when the insert is before
     the last stmt in a block. In this case, we have to update the
     container of the end pointer.  */
  tsi_link_before (&inserted_tsi, t, TSI_NEW_STMT);
  add_stmt_to_bb (tsi_container (inserted_tsi), curr_bb, parent);

  if (curr_container == curr_bb->head_tree_p)
    {
      curr_bb->head_tree_p = tsi_container (inserted_tsi);
      /* If the parent block is a COND_EXPR, check if this
	 is the block which they point to and update if necessary.  */
      if (parent)
        {
	  tree insert_container = *tsi_container (inserted_tsi);
	  switch (TREE_CODE (parent))
	    {
	      case COND_EXPR:
		if (bb_for_stmt (COND_EXPR_THEN (parent)) == curr_bb)
		  COND_EXPR_THEN (parent) = insert_container;
		else
		  if (bb_for_stmt (COND_EXPR_ELSE (parent)) == curr_bb)
		    COND_EXPR_ELSE (parent) = insert_container;
		break;

	      default:
		break;
	    }
	}
    }

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

/* When inserting on a FALLTHRU edge from a switch, create a new default case
   for the code. If there is a fallthru edge, there should be no default case.
   Inputs are the SWITCH source block, the original DEST block, and the new
   block which will contain the new default case.  The edge from src->dest
   has already been split at this point.  */

static tree *
handle_switch_fallthru (tree sw_stmt, basic_block dest, basic_block new_bb)
{
  tree_stmt_iterator tsi;
  block_stmt_iterator bsi;
  tree goto_stmt, stmt, label, tmp;
  basic_block src, tmp_bb;
  edge e;


  /* First, make all predecessors which don't explicitly goto the DEST block
     do so, except for SRC->DEST.  */

  bsi = bsi_start (dest);
  stmt = bsi_stmt (bsi);
  if (TREE_CODE (stmt) != LABEL_EXPR)
    {
      /* DEST does not start with a label, add one.  */
      label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
      DECL_CONTEXT (label) = current_function_decl;
      stmt = build1 (LABEL_EXPR, void_type_node, label);
      bsi_insert_before (&bsi, stmt, BSI_NEW_STMT);
    }
  else
    label = LABEL_EXPR_LABEL (stmt);

  src = bb_for_stmt (sw_stmt);
  for (e = dest->pred; e; e = e->pred_next)
    if (e->src != new_bb)
      {
        /* The only way stmt can be NULL is if we are in the process of
	   handling a nested switch stmt when we get here, and haven't 
	   fully constructed the default case for the other one yet.  This 
	   ought to be safe to ignore at this point.  */
        stmt = last_stmt (e->src);
	if (stmt && TREE_CODE (stmt) != GOTO_EXPR)
	  {
	    goto_stmt = build1 (GOTO_EXPR, void_type_node, label);
	    tmp = PENDING_STMT (e);
	    SET_PENDING_STMT (e, NULL_TREE);
	    bsi_insert_on_edge_immediate (e, goto_stmt, NULL, &tmp_bb);
	    SET_PENDING_STMT (e, tmp);
	    e->flags = e->flags & ~EDGE_FALLTHRU;
	    /* Insertion should never cause a new block, unless STMT needs
	       to be the last statement in E->SRC (e.g., STMT is a
	       CALL_EXPR that may throw).  */
	    if (tmp_bb && !stmt_ends_bb_p (stmt))
	      abort ();
	  }
      }
    else
      /* This will no longer be a fallthru edge.  */
      e->flags = e->flags & ~EDGE_FALLTHRU;

  /* Now there are no fall throughs to the DEST block, so simple create
     the default case, and insert there.  */

  stmt = SWITCH_BODY (sw_stmt);
  /* If the switch isn't inside a BIND_EXPR, make one.  */
  if (TREE_CODE (stmt) != BIND_EXPR)
    {
      tree bind = build (BIND_EXPR, void_type_node, NULL_TREE, stmt, NULL_TREE);
      set_bb_for_stmt (bind, bb_for_stmt (stmt));
      SWITCH_BODY (sw_stmt) = bind;
    }

  tsi = tsi_last (&BIND_EXPR_BODY (SWITCH_BODY (sw_stmt)));
  label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
  DECL_CONTEXT (label) = current_function_decl;
  stmt = build (CASE_LABEL_EXPR, void_type_node, NULL_TREE, NULL_TREE, label);

  /* Update block in the new CE node.  */
  tmp_bb = bb_for_stmt (tsi_stmt (tsi));
  if (tmp_bb)
    tsi = bsi_link_after (&tsi, stmt, tmp_bb, parent_stmt (tsi_stmt (tsi)));
  else
    {
      tsi_link_after (&tsi, stmt, TSI_SAME_STMT);
      append_stmt_to_bb (tsi_container (tsi), new_bb, sw_stmt);
    }
  tsi_next (&tsi);
  append_stmt_to_bb (tsi_container (tsi), new_bb, sw_stmt);

  new_bb->succ->flags = new_bb->succ->flags | EDGE_FALLTHRU;

  return tsi_container (tsi);
}

/* Arrange for a place to insert a stmt when we are splitting a block which is
   targeted by a switch stmt.  Return the container which is used to build
   a TSI where the edge stmt should be inserted after.

   Fallthrough code must be directed around the target label, and a target
   label must be inserted on the other side of the code we are inserting.
   ie:
     case X:
	// fallthrough
  BB_a
     case Y:
       code;

   will be turned into:

     case X:
       goto newlab;
  BB_b
     case Y:
       inserted_code;
  BB_a
     newlab:
       code;

   Note that upon entry to this function, src is *not* the switch stmt's block
   any more. bsi_insert_on_edge_immediate() has already split the edge from
   src->dest, so we have   original_src -> src -> dest. This new src block
   is currently empty.  */


static tree *
handle_switch_split (basic_block src, basic_block dest)
{
  block_stmt_iterator bsi, tmp;
  tree_stmt_iterator tsi, end_tsi;
  tree stmt, label, goto_stmt;
  tree tmp_tree;
  basic_block new_bb;
  edge e;

  /* The edge between SRC and DEST will always be a fallthru edge at this
     point.  */
  src->succ->flags |= EDGE_FALLTHRU;

  label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
  DECL_CONTEXT (label) = current_function_decl;
  TREE_USED (label) = 1;

  /* Insert a goto on all edges except the one from src to this label. */

restart_loop:
  for (e = dest->pred; e ; e = e->pred_next)
    {
      if (e->src != src)
        {
	  tmp = bsi_last (e->src);
	  goto_stmt = bsi_stmt (tmp);
	  /* Dont issue a goto if it already goto's this label.  See below 
	     for how this can happen to a new label.  */
	  if (goto_stmt && TREE_CODE (goto_stmt) == GOTO_EXPR
	      && GOTO_DESTINATION (goto_stmt) == label)
	    continue;

	  goto_stmt = build1 (GOTO_EXPR, void_type_node, label);
	  tmp_tree = PENDING_STMT (e);
	  SET_PENDING_STMT (e, NULL_TREE);
	  bsi_insert_on_edge_immediate (e, goto_stmt, NULL, &new_bb);

	  /* So splitting this edge *can* result in another basic block
	     if there is a case label nested inside an if construct, for
	     instance. Yes, this is allowed. blah. 
	     So this is ugly. The edge may no longer be in the edge list we
	     have been traversing, so we have to start over.  First attach any
	     pending insertions to the new edge.  This is why we need to check 
	     for exisiting GOTO's to our label above.  */
	  if (new_bb)
	    {
#ifdef ENABLE_CHECKING
	      /* There ought to be exactly one successor to the new block.  */
	      if (new_bb->succ == NULL || new_bb->succ->succ_next != NULL)
	        abort();
#endif
	      SET_PENDING_STMT (new_bb->succ, tmp_tree);
	      new_bb->succ->flags = new_bb->succ->flags & ~EDGE_FALLTHRU;
	      goto restart_loop;
	    }
	  SET_PENDING_STMT (e, tmp_tree);
	  e->flags = e->flags & ~EDGE_FALLTHRU;

	}
    }

  /* Find the last case label.  That will be where the code seperation
     between bb_c and bb_a will be formed.  Upon exit of the loop, bsi will
     point to the first stmt in BB_a.  */

  bsi = bsi_start (dest);
  for (tmp = bsi; !bsi_end_p (bsi); bsi_next (&bsi))
    {
      stmt = bsi_stmt (bsi);
      if (is_label_stmt (bsi_stmt (bsi)))
	{
	  if (TREE_CODE (stmt) != CASE_LABEL_EXPR)
	    break;
	}
      else
        break;
      tmp = bsi;
    }

  /* Now the stmts delineating the new block are known. Change the basic
     block for those stmts. It cannot be done in the above loop, for
     changing the basic block of a stmt pointed to by an iterator will cause
     the iterator to think its reached the end of a block. (It is now
     pointing to BB_b, the next stmt is in BB_a, so it terminates.

     We know at least one statement will need it's block changed, so a
     "do" loop is appropriate here.

     After the above loop, 'tmp' will be the last BSI stmt that should be in
     the new block. We end our loop with the next tsi_stmt after that. Note
     that 'bsi' is not the correct place to end the loop because block
     iterators ignore certain stmts, like BIND_EXPR. These can have local
     automatics, and we dont want to copy these stmts into the new block.  */

  tsi = tsi_start (dest->head_tree_p);
  end_tsi = tsi_from_bsi (tmp);
  tsi_next (&end_tsi);
  do
    {
      append_stmt_to_bb (tsi_container (tsi),
			 src,
			 parent_stmt (tsi_stmt (tsi)));
      tsi_next (&tsi);
    }
  while (!tsi_end_p (tsi) && (tsi_container (tsi) != tsi_container (end_tsi)));

  /* Issue the label at the beginning of DEST, and update DEST's head
     and end pointers.  */

  stmt = build1 (LABEL_EXPR, void_type_node, label);
  if (bsi_end_p (bsi))
    {
      /* There are no stmts left, so we need to link an empty_stmt node
	 after the last stmt in BB_c (which is pointed to by 'tmp'), and make
	 it the only element of BB_a.  */
      tsi = tsi_from_bsi (tmp);
      tsi_link_after (&tsi, stmt, TSI_SAME_STMT);
      /* If we're linking after the last stmt in the chain, we have to update
         the container for the current stmt as well since we may have a new
	 container.  */
      set_bb_for_stmt (*tsi_container (tsi), bb_for_stmt (tsi_stmt (tsi)));
      tsi_next (&tsi);
      dest->head_tree_p = (tree *) NULL;
      dest->end_tree_p = (tree *) NULL;
      append_stmt_to_bb (tsi_container (tsi),
			 dest,
			 parent_stmt (bsi_stmt (tmp)));
    }
  else
    {
      dest->head_tree_p = bsi_container (bsi);
      bsi_insert_before (&bsi, stmt, BSI_NEW_STMT);
    }

  bsi = bsi_last (src);
  return bsi_container (bsi);
}

/* Given an edge between src and dest, return a TSI representing the location
   that any instructions on this edge should be inserted.
   The location parameter returns a value indicating how this iterator is
   to be used.  */

static tree_stmt_iterator
find_insert_location (basic_block src, basic_block dest, basic_block new_block,
		      enum find_location_action *location)
{
  block_stmt_iterator bsi;
  tree *ret, stmt;

  *location = EDGE_INSERT_LOCATION_BEFORE;
  bsi = bsi_last (src);
  if (!bsi_end_p (bsi))
    {
      stmt = bsi_stmt (bsi);
      switch (TREE_CODE (stmt))
	{
	  case COND_EXPR:
	    /* If the ELSE block is non-existant, and this is an edge from the
	       COND_EXPR to a block other than the THEN block, then we create
	       a new ELSE clause.  */
	    if (bb_for_stmt (COND_EXPR_ELSE (stmt)) == NULL)
	      if (bb_for_stmt (COND_EXPR_THEN (stmt)) != dest)
		{
		  ret = &COND_EXPR_ELSE (stmt);
		  *location = EDGE_INSERT_LOCATION_NEW_ELSE;
		  break;
		}

	    /* It must be an edge from the COND_EXPR to either the THEN or
	       ELSE block. We will need to insert a new stmt in front of the
	       first stmt in the block, *and* update the pointer to the
	       THEN or ELSE clause.  */
	    if (bb_for_stmt (COND_EXPR_THEN (stmt)) == dest)
	      {
		ret = &COND_EXPR_THEN (stmt);
		*location = EDGE_INSERT_LOCATION_THEN;
	      }
	    else
	      {
		ret = &COND_EXPR_ELSE (stmt);
		*location = EDGE_INSERT_LOCATION_ELSE;
	      }
	    break;

	  case TRY_CATCH_EXPR:
	  case TRY_FINALLY_EXPR:
	    if (bb_for_stmt (TREE_OPERAND (stmt, 0)) == dest)	
	      ret = &TREE_OPERAND (stmt, 0);
	    else
	      ret = &TREE_OPERAND (stmt, 1);
	    *location = EDGE_INSERT_LOCATION_BEFORE;
	    break;

	  case SWITCH_EXPR:
	    bsi = bsi_start (dest);
	    if (TREE_CODE (bsi_stmt (bsi)) != CASE_LABEL_EXPR)
	      {
		ret = handle_switch_fallthru (stmt, dest, new_block);
		*location = EDGE_INSERT_LOCATION_BSI_AFTER;
	      }
	    else
	      {
		ret = handle_switch_split (new_block, dest);
		*location = EDGE_INSERT_LOCATION_AFTER;
	      }
	    break;

	case CALL_EXPR:
	case MODIFY_EXPR:
	    /* The block ends in a CALL which has abnormal edges.  In
	       that case, we simply create a new block right after this
	       one, and then fall through to the destination block.  */
	    ret = src->end_tree_p;
	    *location = EDGE_INSERT_LOCATION_AFTER;
	    break;

	default:
	    /* All cases ought to have been covered by now.  */
	    abort ();
	}
    }
  else
    ret = src->end_tree_p;

 return tsi_start (ret);
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
  enum find_location_action location;
  tree first, last, inserted_stmt, parent;
  bb_ann_t ann;
  edge e2;

  if (old_bsi)
    old_bsi->tp = (tree *)NULL;
  if (created_block)
    *created_block = (basic_block)NULL;

  first = last = NULL_TREE;
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

  tsi = find_insert_location (src, dest, new_bb, &location);
  parent = parent_stmt (tsi_stmt (tsi));

  switch (location)
    {
      case EDGE_INSERT_LOCATION_BEFORE:
      case EDGE_INSERT_LOCATION_THEN:
      case EDGE_INSERT_LOCATION_ELSE:
      case EDGE_INSERT_LOCATION_NEW_ELSE:
	tsi_link_before (&tsi, stmt, TSI_NEW_STMT);
	break;

      case EDGE_INSERT_LOCATION_AFTER:
	tsi_link_after (&tsi, stmt, TSI_NEW_STMT);
	break;

      case EDGE_INSERT_LOCATION_BSI_AFTER:
        bsi = bsi_from_tsi (tsi);
	bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);
    }

  if (location != EDGE_INSERT_LOCATION_BSI_AFTER)
    {
      append_stmt_to_bb (tsi_container (tsi), new_bb, parent);
      inserted_stmt = tsi_stmt (tsi);
      bsi = bsi_from_tsi (tsi);
    }
  else
    inserted_stmt = bsi_stmt (bsi);

  switch (location)
    {
      case EDGE_INSERT_LOCATION_THEN:
      case EDGE_INSERT_LOCATION_ELSE:
	stmt = last_stmt (src);
	if (location == EDGE_INSERT_LOCATION_THEN)
	  COND_EXPR_THEN (stmt) = *tsi_container (tsi);
	else
	  COND_EXPR_ELSE (stmt) = *tsi_container (tsi);
	/* Fallthru.  */

      case EDGE_INSERT_LOCATION_BEFORE:
      case EDGE_INSERT_LOCATION_AFTER:
	tsi_next (&tsi);
	if (!tsi_end_p (tsi))
	  {
	    /* The container for the head of the dest block has been
	       changed. (we've linked a new stmt in front of it.)  */
	    if (dest->end_tree_p == dest->head_tree_p)
	      dest->end_tree_p = tsi_container (tsi);
	    dest->head_tree_p = tsi_container (tsi);
	  }
	break;

      case EDGE_INSERT_LOCATION_NEW_ELSE:
	/* This causes a new stmt chain to be formed, and the ELSE clause needs
	   to be set.  Set the block number for the empty stmt which might
	   follow this stmt as well.  */
	stmt = last_stmt (src);
	COND_EXPR_ELSE (stmt) = inserted_stmt;
	tsi_next (&tsi);
	if (tsi_container (tsi))
	  append_stmt_to_bb (tsi_container (tsi), new_bb, parent);
	break;

      case EDGE_INSERT_LOCATION_BSI_AFTER:
        break;
    }

  /* Now update the required SSA bits.  */
  modify_stmt (inserted_stmt);

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
   replaced. If TP2 points to a COMPOUND_EXPR, a new BIND_EXPR will be
   created to wrap the whole chain of statements into a single block.  */

void
replace_stmt (tree *tp1, tree *tp2)
{
  tree t;

  if (TREE_CODE (*tp2) == COMPOUND_EXPR)
    {
      /* If TP2 points to a COMPOUND_EXPR, create a BIND_EXPR to hold the
	 chain of statements.  */
      t = build (BIND_EXPR, void_type_node, NULL_TREE, *tp2, NULL_TREE);
    }
  else
    {
      /* Otherwise use TP2 statement directly.  */
      t = *tp2;
    }

  /* Relocate annotations for the replacement statement.  */
  SET_EXPR_LOCUS (t, EXPR_LOCUS (*tp1));
  add_stmt_to_bb (&t, bb_for_stmt (*tp1), NULL_TREE);

  /* Don't replace COMPOUND_EXPRs.  Only their operands.  */
  if (TREE_CODE (*tp1) == COMPOUND_EXPR)
    TREE_OPERAND (*tp1, 0) = t;
  else
    *tp1 = t;
}

/* Move all outgoing edges from BB2 to BB1 and keep PHI nodes and
   dominator information up to date.  */

static void
move_outgoing_edges (basic_block bb1, basic_block bb2)
{

  while (bb2->succ)
    {
      tree phi;
      edge new_edge, old_edge;

      old_edge = bb2->succ;
      new_edge = make_edge (bb1, old_edge->dest, old_edge->flags);

      /* If make_edge created a new edge, then we need to update the PHI
	 node at BB2's successor.  The arguments that used to come from
	 BB2 now come from BB1.

	 If make_edge did not create a new edge, then we already had an
	 edge from BB1 to BB2's successor.  In this case we want to
	 remove the edge and remove its alternative from BB2's successor's
	 PHI nodes, hence we use ssa_remove_edge.  */
      if (new_edge)
	{
	  for (phi = phi_nodes (old_edge->dest); phi; phi = TREE_CHAIN (phi))
	    {
	      int i;
	      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
		if (PHI_ARG_EDGE (phi, i) == old_edge)
		  PHI_ARG_EDGE (phi, i) = new_edge;
	    }

	  /* Note that we shouldn't call ssa_remove_edge here because we've
	     already dealt with PHI nodes.  */
	  remove_edge (old_edge);
	}
      else
	{
	  ssa_remove_edge (old_edge);
	}

    }

  /* BB2's dominator children are now BB1's.  Also, remove BB2 as a
     dominator child of BB1.  */
  if (dom_children (bb1))
    {
      bitmap dom1 = dom_children (bb1);
      bitmap dom2 = dom_children (bb2);
      bitmap_clear_bit (dom1, bb2->index);
      if (dom2)
	bitmap_a_or_b (dom1, dom1, dom2);
    }
}

/* Given two blocks BB1 and BB2, merge the two blocks by moving all the
   statements in BB2 after the last statement of BB1.
   Note that no error checking is done, if there is more than one edges
   coming into BB2 this function will happily munge the CFG.  */

static void
merge_tree_blocks (basic_block bb1, basic_block bb2)
{
  tree t1;

  /* Step 1.  Chain all the statements in BB2 at the end of BB1.  */
  t1 = last_stmt (bb1);
  if (is_ctrl_structure (t1))
    {
      /* If BB1 ends in a control statement C, BB2 is the first block of
	 C's body.  In this case we don't need to insert statements from
	 BB2 into BB1, we can simply replace C with the first statement of
	 BB2.  */
      if (TREE_CODE (t1) == COND_EXPR)
	replace_stmt (bb1->end_tree_p, bb2->head_tree_p);
      else if (TREE_CODE (t1) == SWITCH_EXPR)
	{
	  /* Skip over all the CASE labels.  */
	  block_stmt_iterator bi2 = bsi_start (bb2);
	  while (!bsi_end_p (bi2)
	         && TREE_CODE (bsi_stmt (bi2)) == CASE_LABEL_EXPR)
	    bsi_next (&bi2);

	  if (!bsi_end_p (bi2))
	    replace_stmt (bb1->end_tree_p, bsi_container (bi2));
	}
      else
	abort ();
    }
  else
    {
      /* Insert the first statement of BB2 after the last statement of BB1.  */
      block_stmt_iterator bi1 = bsi_from_tsi (tsi_start (bb1->end_tree_p));
      bsi_insert_after (&bi1, *(bb2->head_tree_p), BSI_SAME_STMT);
    }

  /* Step 2.  After chaining the statements from BB2 at the end of BB1, we
     need to map all the statements between BB1->END_TREE_P and
     BB2->END_TREE_P to BB1.  */
  remap_stmts (bb1, bb2, bb1->end_tree_p);

  /* Step 3.  Update edges and dominator children for BB1, and remove BB2.  */

  /* BB2's successors are now BB1's.  */
  while (bb1->succ)
    ssa_remove_edge (bb1->succ);

  /* Now relocate all the outgoing edges from BB2 to BB1.  */
  move_outgoing_edges (bb1, bb2);

  /* BB1 may no longer be a control expression after merging with BB2.
     Copy BB2's flags into BB1.  */
  bb1->flags &= ~BB_CONTROL_STRUCTURE;
  bb1->flags |= bb2->flags;

  /* Before removing BB2, clear out its predecessors, successors and
     head/tail statements, otherwise remove_bb will try to remove
     statements and edges that now belong to BB1.  */
  bb2->head_tree_p = NULL;
  bb2->end_tree_p = NULL;
  bb2->pred = NULL;
  bb2->succ = NULL;
  remove_bb (bb2, REMOVE_NO_STMTS);
}


/* Map all the statements from block BB2 to block BB1 starting at the
   statement pointed by FIRST_P.  Note that we cannot use block iterators
   here.  This would confuse bsi_end_p() because, when moving from one
   statement s1 to its successor s2, both s1 and s2 will be in different
   blocks and bsi_end_p will stop the iteration.

   Return true after mapping the last statement.  */

static bool
remap_stmts (basic_block bb1, basic_block bb2, tree *first_p)
{
  tree_stmt_iterator ti;
  tree parent = parent_stmt (*(bb1->head_tree_p));

  for (ti = tsi_start (first_p); !tsi_end_p (ti); tsi_next (&ti))
    {
      tree stmt;
      tree *container = tsi_container (ti);
      basic_block bb = bb_for_stmt (*container);

      /* If we have gone past the end of BB2, we're done.  */
      if (bb != bb1 && bb != bb2)
	return true;

      append_stmt_to_bb (container, bb1, parent);

      /* Recurse into BIND_EXPR bodies.  */
      stmt = tsi_stmt (ti);
      if (stmt && TREE_CODE (stmt) == BIND_EXPR)
	if (remap_stmts (bb1, bb2, &BIND_EXPR_BODY (stmt)))
	  return true;
    }

  return false;
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
  create_block_annotation (new_bb);
  redirect_edge_succ  (edge_in, new_bb);
  new_edge = make_edge (new_bb, dest, EDGE_FALLTHRU);

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
  return 0;
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
tree_register_cfg_hooks ()
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

/* Assigns a scope to variables defined in bind_expr SCOPE.  */
static void
assign_vars_to_scope (tree scope)
{
  tree var;

  for (var = BIND_EXPR_VARS (scope); var; var = TREE_CHAIN (var))
    get_var_ann (var)->scope = scope;
}
