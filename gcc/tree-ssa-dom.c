/* SSA Dominator optimizations for trees
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
#include "flags.h"
#include "rtl.h"
#include "tm_p.h"
#include "ggc.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "function.h"
#include "diagnostic.h"
#include "timevar.h"
#include "tree-dump.h"
#include "tree-flow.h"
#include "domwalk.h"

/* This file implements optimizations on the dominator tree.  */

/* Dump file and flags.  */
static FILE *dump_file;
static int dump_flags;

/* Hash table with expressions made available during the renaming process.
   When an assignment of the form X_i = EXPR is found, the statement is
   stored in this table.  If the same expression EXPR is later found on the
   RHS of another statement, it is replaced with X_i (thus performing
   global redundancy elimination). */
static htab_t avail_exprs;

/* Table of constant values and copies indexed by SSA name.  When the
   renaming pass finds an assignment of a constant (X_i = C) or a copy
   assignment from another SSA variable (X_i = Y_j), it creates a mapping
   between X_i and the RHS in this table.  This mapping is used later on,
   when renaming uses of X_i.  If an assignment to X_i is found in this
   table, instead of using X_i, we use the RHS of the statement stored in
   this table (thus performing very simplistic copy and constant
   propagation).  */
static varray_type const_and_copies;

/* Track whether or not we have changed the control flow graph.  */
static bool cfg_altered;

/* This pass can expose additional variables to rename.  We track them in
   this bitmap.  */
static sbitmap vars_to_rename;

/* Statistics for dominator optimizations.  */
struct opt_stats_d
{
  long num_stmts;
  long num_exprs_considered;
  long num_const_prop;
  long num_copy_prop;
  long num_re;
};

/* Value range propagation record.  Each time we encounter a conditional
   of the form SSA_NAME COND CONST we create a new vrp_element to record
   how the condition affects the possible values SSA_NAME may have.

   Each record contains the condition tested (COND), and the the range of
   values the variable may legitimately have if COND is true.  Note the
   range of values may be a smaller range than COND specifies if we have
   recorded other ranges for this variable.  Each record also contains the
   block in which the range was recorded for invalidation purposes.

   Note that the current known range is computed lazily.  This allows us
   to avoid the overhead of computing ranges which are never queried.

   When we encounter a conditional, we look for records which constrain
   the SSA_NAME used in the condition.  In some cases those records allow
   us to determine the condition's result at compile time.  In other cases
   they may allow us to simplify the condition.

   We also use value ranges to do things like transform signed div/mod
   operations into unsigned div/mod or to simplify ABS_EXPRs. 

   Simple experiments have shown these optimizations to not be all that
   useful on switch statements (much to my surprise).  So switch statement
   optimizations are not performed.

   Note carefully we do not propagate information through each statement
   in the block.  ie, if we know variable X has a value defined of
   [0, 25] and we encounter Y = X + 1, we do not track a value range
   for Y (which would be [1, 26] if we cared).  Similarly we do not
   constrain values as we encounter narrowing typecasts, etc.  */

struct vrp_element
{
  /* The highest and lowest values the variable in COND may contain when
     COND is true.  Note this may not necessarily be the same values
     tested by COND if the same variable was used in earlier conditionals. 

     Note this is computed lazily and thus can be NULL indicating that
     the values have not been computed yet.  */
  tree low;
  tree high;

  /* The actual conditional we recorded.  This is needed since we compute
     ranges lazily.  */
  tree cond;

  /* The basic block where this record was created.  We use this to determine
     when to remove records.  */
  basic_block bb;
};

static struct opt_stats_d opt_stats;

/* Redirections scheduled by jump threading.   */
static varray_type edges_to_redirect;
static varray_type redirection_targets;

/* A virtual array holding value range records for the variable identified
   by the index, SSA_VERSION.  */
static varray_type vrp_data;

/* Datastructure for block local data used during the dominator walk.  
   We maintain a stack of these as we recursively walk down the
   dominator tree.  */

struct dom_walk_block_data
{
  /* Array of all the expressions entered into the global expression
     hash table by this block.  During finalization we use this array to
     know what expressions to remove from the global expression hash
     table.  */
  varray_type avail_exprs;

  /* Array of dest, src pairs that need to be restored during finalization
     into the global const/copies table during finalization.  */
  varray_type const_and_copies;

  /* Array of statements we need to rescan during finalization for newly
     exposed variables.  */
  varray_type stmts_to_rescan;

  /* Array of variables which have their values constrained by operations
     in this basic block.  We use this during finalization to know
     which variables need their VRP data updated.  */
  varray_type vrp_variables;
};

/* Local functions.  */
static bool optimize_stmt (block_stmt_iterator, varray_type *);
static inline tree get_value_for (tree);
static inline void set_value_for (tree, tree);
static tree lookup_avail_expr (tree, varray_type *, bool);
static tree get_eq_expr_value (tree, int, varray_type *,
			       basic_block, varray_type *);
static hashval_t avail_expr_hash (const void *);
static int avail_expr_eq (const void *, const void *);
static void htab_statistics (FILE *, htab_t);
static void record_cond_is_false (tree, varray_type *);
static void record_cond_is_true (tree, varray_type *);
static void thread_edge (edge, basic_block);
static tree update_rhs_and_lookup_avail_expr (tree, tree, varray_type *,
					      stmt_ann_t, bool);
static tree simplify_rhs_and_lookup_avail_expr (tree, varray_type *,
						stmt_ann_t, int);
static tree simplify_cond_and_lookup_avail_expr (tree, varray_type *,
						 stmt_ann_t, int);
static tree find_equivalent_equality_comparison (tree);
static void record_range (tree, basic_block, varray_type *);
static bool extract_range_from_cond (tree, tree *, tree *, int *);
static bool cprop_into_stmt (tree);
static void record_equivalences_from_phis (basic_block);
static void record_equivalences_from_incoming_edge (basic_block, tree,
						    varray_type *,
						    varray_type *,
						    varray_type *);
static bool eliminate_redundant_computations (tree, varray_type *, stmt_ann_t);
static void record_equivalences_from_stmt (tree, varray_type *,
					   int, stmt_ann_t);
static void thread_through_successor (basic_block, varray_type *);
static void dom_opt_finalize_block (struct dom_walk_data *, basic_block, tree);
static void dom_opt_initialize_block (struct dom_walk_data *,
				      basic_block, tree);
static void dom_opt_walk_stmts (struct dom_walk_data *, basic_block, tree);
static void cprop_into_phis (struct dom_walk_data *, basic_block, tree);

/* Return the value associated with variable VAR in TABLE.  */

static inline tree
get_value_for (tree var)
{
  return VARRAY_TREE (const_and_copies, SSA_NAME_VERSION (var));
}


/* Associate VALUE to variable VAR in TABLE.  */

static inline void
set_value_for (tree var, tree value)
{
  VARRAY_TREE (const_and_copies, SSA_NAME_VERSION (var)) = value;
}


/* Optimize function FNDECL based on the dominator tree.  This does
   simple const/copy propagation and redundant expression elimination using
   value numbering.

   This pass may expose new symbols that need to be renamed into SSA.  For
   every new symbol exposed, its corresponding bit will be set in
   VARS_TO_RENAME.

   PHASE indicates which dump file from the DUMP_FILES array to use when
   dumping debugging information.  */

void
tree_ssa_dominator_optimize (tree fndecl, sbitmap vars,
			     enum tree_dump_index phase)
{
  edge e;
  struct dom_walk_data walk_data;

  timevar_push (TV_TREE_SSA_DOMINATOR_OPTS);

  /* Set up debugging dump files.  */
  dump_file = dump_begin (phase, &dump_flags);

  /* Create our hash tables.  */
  avail_exprs = htab_create (1024, avail_expr_hash, avail_expr_eq, NULL);
  VARRAY_TREE_INIT (const_and_copies, next_ssa_version, "const_and_copies");
  VARRAY_EDGE_INIT (edges_to_redirect, 20, "edges_to_redirect");
  VARRAY_BB_INIT (redirection_targets, 20, "redirection_targets");
  VARRAY_GENERIC_PTR_INIT (vrp_data, next_ssa_version, "vrp_data");
  VARRAY_GENERIC_PTR_INIT (walk_data.block_data_stack, 2, "block_data");


  /* Setup callbacks for the generic dominator tree walker.  */
  walk_data.before_dom_children_before_stmts = dom_opt_initialize_block;
  walk_data.before_dom_children_walk_stmts = dom_opt_walk_stmts;
  walk_data.before_dom_children_after_stmts = cprop_into_phis;
  walk_data.after_dom_children_after_stmts = dom_opt_finalize_block;
  walk_data.after_dom_children_walk_stmts = NULL;
  walk_data.after_dom_children_before_stmts = NULL;

  /* Once we have the ability to attach pass-global data to the dominator
     walker datastructure this (along with other pass-global data) should
     move into that structure.  */
  vars_to_rename = vars;

  /* Build the dominator tree if necessary. 

     We don't have a flag indicating if the dominator tree is available,
     but we can make a very accurate approximation by checking to see if
     the successors of the entry block have any dominator children.  If
     they do not, then we assume that the dominator tree is not available.  */
  for (e = ENTRY_BLOCK_PTR->succ; e; e = e->succ_next)
    if (dom_children (e->dest))
      break;

  /* If we did not find any dominator children in the successors of the
     entry block, then rebuild the dominator tree.  */
  if (e == NULL)
    {
      dominance_info idom;

      idom = calculate_dominance_info (CDI_DOMINATORS);
      build_dominator_tree (idom);
      free_dominance_info (idom);
    }

  /* If we prove certain blocks are unreachable, then we want to
     repeat the dominator optimization process as PHI nodes may
     have turned into copies which allows better propagation of
     values.  So we repeat until we do not identify any new unreachable
     blocks.  */
  do
    {
      /* Optimize the dominator tree.  */
      cfg_altered = false;

      /* Recursively walk the dominator tree optimizing statements.  */
      walk_dominator_tree (&walk_data, ENTRY_BLOCK_PTR, NULL);

      /* Wipe the hash tables.  */
      htab_empty (avail_exprs);

      VARRAY_CLEAR (const_and_copies);

      /* If some edges were threaded in this iteration, then perform
	 the required redirections and recompute the dominators.  */
      if (VARRAY_ACTIVE_SIZE (edges_to_redirect) > 0)
	{
	  basic_block tgt;

	  while (VARRAY_ACTIVE_SIZE (edges_to_redirect) > 0)
	    {
	      e = VARRAY_TOP_EDGE (edges_to_redirect);
	      tgt = VARRAY_TOP_BB (redirection_targets);
	      VARRAY_POP (edges_to_redirect);
	      VARRAY_POP (redirection_targets);

	      thread_edge (e, tgt);
	    }

	  cfg_altered = true;
	}

      /* We may have made some basic blocks unreachable, remove them.  */
      cfg_altered |= remove_unreachable_blocks ();

      /* If the CFG was altered, then recompute the dominator tree.  This
	 is not strictly needed if we only removed unreachable blocks, but
	 may produce better results.  If we threaded jumps, then rebuilding
	 the dominator tree is strictly necessary.  */
      if (cfg_altered)
	{
	  dominance_info idom;
	  cleanup_tree_cfg ();
	  idom = calculate_dominance_info (CDI_DOMINATORS);
	  build_dominator_tree (idom);
	  free_dominance_info (idom);
	}

      /* If we are going to iterate (CFG_ALTERED is true), then we must
	 perform any queued renaming before the next iteration.  */
      if (cfg_altered
	  && sbitmap_first_set_bit (vars_to_rename) >= 0)
	{
	  rewrite_into_ssa (fndecl, vars_to_rename, TDI_none);
	  sbitmap_zero (vars_to_rename);
	  VARRAY_GROW (const_and_copies, next_ssa_version);
	  VARRAY_GROW (vrp_data, next_ssa_version);
	}
    }
  while (cfg_altered);

  /* Remove any unreachable blocks left behind and linearize the CFG.  */
  cleanup_tree_cfg ();

  /* Debugging dumps.  */
  if (dump_file)
    {
      if (dump_flags & TDF_STATS)
	dump_dominator_optimization_stats (dump_file);
      dump_function_to_file (fndecl, dump_file, dump_flags);
      dump_end (phase, dump_file);
    }

  htab_delete (avail_exprs);

  VARRAY_FREE (edges_to_redirect);
  VARRAY_FREE (redirection_targets);

  timevar_pop (TV_TREE_SSA_DOMINATOR_OPTS);
}


/* Redirects edge E to basic block DEST.  */

static void
thread_edge (edge e, basic_block dest)
{
  block_stmt_iterator dest_iterator = bsi_start (dest);
  tree dest_stmt = first_stmt (dest);
  tree label, goto_stmt, stmt;
  basic_block bb = e->src, new_bb;
  int flags = e->flags;

  if (e != bb->succ
      || bb->succ->succ_next)
    abort ();

  /* Remove EDGE_FALLTHRU from the edge (by convention, control edges are
     not fallthru).  */
  flags &= ~EDGE_FALLTHRU;

  /* We need a label at our final destination.  If it does not already exist,
     create it.  */
  if (!dest_stmt
      || TREE_CODE (dest_stmt) != LABEL_EXPR)
    {
      label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
      DECL_CONTEXT (label) = current_function_decl;
      dest_stmt = build1 (LABEL_EXPR, void_type_node, label);
      bsi_insert_before (&dest_iterator, dest_stmt, BSI_NEW_STMT);
    }
  else
    label = LABEL_EXPR_LABEL (dest_stmt);

  /* If our block does not end with a GOTO, then create one.  Otherwise
     redirect the existing GOTO_EXPR to LABEL.  */
  stmt = last_stmt (bb);
  if (!stmt || TREE_CODE (stmt) != GOTO_EXPR)
    {
      goto_stmt = build1 (GOTO_EXPR, void_type_node, label);
      bsi_insert_on_edge_immediate (e, goto_stmt, NULL, &new_bb);
    }
  else
    {
      GOTO_DESTINATION (stmt) = label;
      new_bb = NULL;
    }

  /* Update/insert PHI nodes as necessary.  */

  /* Now update the edges in the CFG.  */
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "  Threaded jump %d --> %d to %d\n",
	       e->src->index, e->dest->index, dest->index);
      if (new_bb)
	fprintf (dump_file, "    basic block %d created\n", new_bb->index);
    }

  if (new_bb)
    {
      ssa_remove_edge (new_bb->succ);
      make_edge (new_bb, dest, 0);
    }
  else
    {
      ssa_remove_edge (e);
      make_edge (bb, dest, flags);
    }
}

/* We are exiting BB, see if the target block begins with a conditional
   jump which has a known value when reached via BB.  */

static void
thread_through_successor (basic_block bb, varray_type *block_avail_exprs)
{
  /* If we have a single successor, then we may be able to thread
     the edge out of our block to a destination of our successor.

     To simplify the initial implementation we require that
     our successor have no PHI nodes.  */
  if (bb->succ && bb->succ->succ_next == NULL && ! phi_nodes (bb->succ->dest))
    {
      block_stmt_iterator i = bsi_start (bb->succ->dest);
      tree stmt;

      /* Get the successor's first real statement.  */
      while (! bsi_end_p (i)
	     && (IS_EMPTY_STMT (bsi_stmt (i))
		 || TREE_CODE (bsi_stmt (i)) == LABEL_EXPR))
	bsi_next (&i);
      stmt = bsi_end_p (i) ? NULL : bsi_stmt (i);

      /* If the successor's first real statement is a COND_EXPR, then
	 see if we know which arm will be taken.  */
      if (stmt && TREE_CODE (stmt) == COND_EXPR)
	{
	  tree cached_lhs = lookup_avail_expr (stmt, block_avail_exprs, false);

	  if (cached_lhs)
	    {
	      edge taken_edge = find_taken_edge (bb->succ->dest, cached_lhs);
	      basic_block dest = (taken_edge ? taken_edge->dest : NULL);

	      /* If we have a known destination for the conditional, then
		 we can perform this optimization, which saves at least one
		 conditional jump each time it applies since we get to
		 bypass the conditional at our original destination.  */
	      if (dest && ! phi_nodes (dest))
		{
		  VARRAY_PUSH_EDGE (edges_to_redirect, bb->succ);
		  VARRAY_PUSH_BB (redirection_targets, dest);
		}
	    }
	}
    }
}

/* Initialize local stacks for this optimizer and record equivalences
   upon entry to BB.  Equivalences can come from the edge traversed to
   reach BB or they may come from PHI nodes at the start of BB.  */

static void
dom_opt_initialize_block (struct dom_walk_data *walk_data,
			  basic_block bb,
			  tree parent_block_last_stmt)
{
  struct dom_walk_block_data *bd;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\n\nOptimizing block #%d\n\n", bb->index);

  /* Initialize the local stacks.
     
     AVAIL_EXPRS stores all the expressions made available in this block.

     CONST_AND_COPIES stores var/value pairs to restore at the end of this
     block.

     STMTS_TO_RESCAN is a list of statements we will rescan for operands.

     VRP_VARIABLES is the list of variables which have had their values
     constrained by an operation in this block.

     These stacks are cleared in the finalization routine run for each
     block.  */
  bd = ggc_alloc (sizeof (struct dom_walk_block_data));
  VARRAY_TREE_INIT (bd->avail_exprs, 20, "block_avail_exprs");
  VARRAY_TREE_INIT (bd->const_and_copies, 2, "block_const_and_copies");
  VARRAY_TREE_INIT (bd->stmts_to_rescan, 20, "stmts_to_rescan");
  VARRAY_TREE_INIT (bd->vrp_variables, 2, "vrp_variables");

  /* Push this block record onto the stack of block records.  */
  VARRAY_PUSH_GENERIC_PTR (walk_data->block_data_stack, bd);

  record_equivalences_from_incoming_edge (bb,
					  parent_block_last_stmt,
					  &bd->avail_exprs,
					  &bd->const_and_copies,
					  &bd->vrp_variables);

  /* PHI nodes can create equivalences too.  */
  record_equivalences_from_phis (bb);
}
/* We have finished processing the dominator children of BB, perform
   any finalization actions in preparation for leaving this node in
   the dominator tree.  */

static void
dom_opt_finalize_block (struct dom_walk_data *walk_data,
			basic_block bb,
			tree parent_block_last_stmt ATTRIBUTE_UNUSED)
{
  struct dom_walk_block_data *bd
    = (struct dom_walk_block_data *)VARRAY_TOP_GENERIC_PTR (walk_data->block_data_stack);
  varray_type block_avail_exprs = bd->avail_exprs;
  varray_type block_const_and_copies = bd->const_and_copies;
  varray_type vrp_variables = bd->vrp_variables;
  varray_type stmts_to_rescan = bd->stmts_to_rescan;

  /* See if we can thread the edge from BB through its successor.
     Do this before we remove entries from our equivalence tables.  */
  thread_through_successor (bb, NULL);

  /* Remove all the expressions made available in this block.  */
  while (VARRAY_ACTIVE_SIZE (block_avail_exprs) > 0)
    {
      tree stmt = VARRAY_TOP_TREE (block_avail_exprs);
      VARRAY_POP (block_avail_exprs);
      htab_remove_elt (avail_exprs, stmt);
    }

  /* Also remove equivalences created by EQ_EXPR_VALUE.  */
  while (VARRAY_ACTIVE_SIZE (block_const_and_copies) > 0)
    {
      tree prev_value, dest;

      prev_value = VARRAY_TOP_TREE (block_const_and_copies);
      VARRAY_POP (block_const_and_copies);
      dest = VARRAY_TOP_TREE (block_const_and_copies);
      VARRAY_POP (block_const_and_copies);

      set_value_for (dest, prev_value);
    }

  /* Remove VRP records associated with this basic block.  They are no
     longer valid.

     To be efficient, we note which variables have had their values
     constrained in this block.  So walk over each variable in the
     VRP_VARIABLEs array.  */
  while (VARRAY_ACTIVE_SIZE (vrp_variables) > 0)
    {
      tree var = VARRAY_TOP_TREE (vrp_variables);

      /* Each variable has a stack of value range records.  We want to
	 invalidate those associated with our basic block.  So we walk
	 the array backwards popping off records associated with our
	 block.  Once we hit a record not associated with our block
	 we are done.  */
      varray_type var_vrp_records = VARRAY_GENERIC_PTR (vrp_data,
							SSA_NAME_VERSION (var));

      while (VARRAY_ACTIVE_SIZE (var_vrp_records) > 0)
	{
	  struct vrp_element *element
	    = (struct vrp_element *)VARRAY_TOP_GENERIC_PTR (var_vrp_records);

	  if (element->bb != bb)
	    break;
  
	  VARRAY_POP (var_vrp_records);
	}

      VARRAY_POP (vrp_variables);
    }

  /* Re-scan operands in all statements that may have had new symbols
     exposed.  */
  while (VARRAY_ACTIVE_SIZE (stmts_to_rescan) > 0)
    {
      tree stmt = VARRAY_TOP_TREE (stmts_to_rescan);
      VARRAY_POP (stmts_to_rescan);
      mark_new_vars_to_rename (stmt, vars_to_rename);
    }

  /* And finally pop the record off the block local data.  */
  VARRAY_POP (walk_data->block_data_stack);
}

/* PHI nodes can create equivalences too.

   Ignoring any alternatives which are the same as the result, if
   all the alternatives are equal, then the PHI node creates an
   equivalence.  */
static void
record_equivalences_from_phis (basic_block bb)
{
  tree phi;

  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    {
      tree lhs = PHI_RESULT (phi);
      tree rhs = NULL;
      int i;

      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	{
	  tree t = PHI_ARG_DEF (phi, i);

	  if (TREE_CODE (t) == SSA_NAME || is_gimple_min_invariant (t))
	    {
	      /* Ignore alternatives which are the same as our LHS.  */
	      if (operand_equal_p (lhs, t, 0))
		continue;

	      /* If we have not processed an alternative yet, then set
		 RHS to this alternative.  */
	      if (rhs == NULL)
		rhs = t;
	      /* If we have processed an alternative (stored in RHS), then
		 see if it is equal to this one.  If it isn't, then stop
		 the search.  */
	      else if (! operand_equal_p (rhs, t, 0))
		break;
	    }
	  else
	    break;
	}

      /* If we had no interesting alternatives, then all the RHS alternatives
	 must have been the same as LHS.  */
      if (!rhs)
	rhs = lhs;

      /* If we managed to iterate through each PHI alternative without
	 breaking out of the loop, then we have a PHI which may create
	 a useful equivalence.  */
      if (i == PHI_NUM_ARGS (phi)
	  && may_propagate_copy (lhs, rhs))
	set_value_for (lhs, rhs);
    }
}

/* Record any equivalences created by the incoming edge to BB.  If BB
   has more than one incoming edge, then no equivalence is created.  */

static void
record_equivalences_from_incoming_edge (basic_block bb,
					tree parent_block_last_stmt,
					varray_type *block_avail_exprs_p,
					varray_type *block_const_and_copies_p,
					varray_type *vrp_variables_p)
{
  int edge_flags;
  tree eq_expr_value = NULL_TREE;

  /* If we have a single predecessor, then extract EDGE_FLAGS from
     our single incoming edge.  Otherwise clear EDGE_FLAGS and
     PAREND_BLOCK_LAST_STMT since they're not needed.  */
  if (bb->pred && !bb->pred->pred_next)
    {
      edge_flags = bb->pred->flags;
    }
  else
    {
      edge_flags = 0;
      parent_block_last_stmt = NULL;
    }

  /* If our parent block ended in a COND_EXPR, add any equivalences
     created by the COND_EXPR to the hash table and initialize
     EQ_EXPR_VALUE appropriately.

     EQ_EXPR_VALUE is an assignment expression created when BB's immediate
     dominator ends in a COND_EXPR statement whose predicate is of the form
     'VAR == VALUE', where VALUE may be another variable or a constant.
     This is used to propagate VALUE on the THEN_CLAUSE of that
     conditional. This assignment is inserted in CONST_AND_COPIES so that
     the copy and constant propagator can find more propagation
     opportunities.  */
  if (parent_block_last_stmt
      && bb->pred->pred_next == NULL
      && TREE_CODE (parent_block_last_stmt) == COND_EXPR
      && (edge_flags & (EDGE_TRUE_VALUE | EDGE_FALSE_VALUE)))
    eq_expr_value = get_eq_expr_value (parent_block_last_stmt,
				       (edge_flags & EDGE_TRUE_VALUE) != 0,
				       block_avail_exprs_p,
				       bb,
				       vrp_variables_p);
  /* Similarly when the parent block ended in a SWITCH_EXPR.  */
  else if (parent_block_last_stmt
	   && TREE_CODE (parent_block_last_stmt) == SWITCH_EXPR
	   && bb->pred->pred_next == NULL)
    {
      int case_count = 0;
      tree case_value = NULL_TREE;
      tree switch_cond = SWITCH_COND (parent_block_last_stmt);

      /* Strip away any useless type conversions.  */
      while (tree_ssa_useless_type_conversion (switch_cond))
	switch_cond = TREE_OPERAND (switch_cond, 0);

      /* If the switch's condition is an SSA variable, then we may
	 know its value at each of the case labels.  */
      if (TREE_CODE (switch_cond) == SSA_NAME)
	{
	  block_stmt_iterator si;

	  /* Walk the statements at the start of this block.  */
	  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	    {
	      tree stmt = bsi_stmt (si);

	      /* If we hit anything other than a CASE_LABEL_EXPR, then
		 stop our search.  */
	      if (TREE_CODE (stmt) != CASE_LABEL_EXPR)
		break;

	      /* If we encountered more than one CASE_LABEL_EXPR, then
		 there are multiple values for the switch's condition
		 which reach this particular destination.  We can not
		 optimize in that case.  */
	      case_count++;
	      if (case_count > 1)
		break;

	      /* If this is the default case or any other abnormal
		 situation, then stop the loop and do not optimize.  */
	      if (! CASE_LOW (stmt) || CASE_HIGH (stmt))
		break;

	      /* Record this case's value.  */
	      case_value = CASE_LOW (stmt);
	    }

	  /* If we encountered precisely one CASE_LABEL_EXPR and it
	     was not the default case, then we know the exact value
	     of SWITCH_COND which caused us to get to this block.
	     Record that equivalence in EQ_EXPR_VALUE.  */
	  if (case_count == 1 && case_value)
	    eq_expr_value = build (MODIFY_EXPR, TREE_TYPE (switch_cond),
				   switch_cond, case_value);
	}
    }

  /* If EQ_EXPR_VALUE (VAR == VALUE) is given, register the VALUE as a
     new value for VAR, so that occurrences of VAR can be replaced with
     VALUE while re-writing the THEN arm of a COND_EXPR.  */
  if (eq_expr_value)
    {
      tree dest = TREE_OPERAND (eq_expr_value, 0);
      tree src = TREE_OPERAND (eq_expr_value, 1);
      tree prev_value = get_value_for (dest);

      set_value_for (dest, src);

      /* Record the destination and its previous value so that we can
	 reset them as we leave this block.  */
      VARRAY_PUSH_TREE (*block_const_and_copies_p, dest);
      VARRAY_PUSH_TREE (*block_const_and_copies_p, prev_value);
    }
}

/* Perform a depth-first traversal of the dominator tree looking for
   redundant expressions and copy/constant propagation opportunities. 

   Expressions computed by each statement are looked up in the
   AVAIL_EXPRS table.  If a statement is found to make a redundant
   computation, it is marked for removal.  Otherwise, the expression
   computed by the statement is assigned a value number and entered
   into the AVAIL_EXPRS table.  See optimize_stmt for details on the
   types of redundancies handled during renaming.

   Once we've optimized the statements in this block we recursively
   optimize every dominator child of this block.

   Finally, remove all the expressions added to the AVAIL_EXPRS
   table during renaming.  This is because the expressions made
   available to block BB and its dominator children are not valid for
   blocks above BB in the dominator tree.

   EDGE_FLAGS are the flags for the incoming edge from BB's dominator
   parent block.  This is used to determine whether BB is the first block
   of a THEN_CLAUSE or an ELSE_CLAUSE.

   VARS_TO_RENAME is a bitmap representing variables that will need to be
   renamed into SSA after dominator optimization.
   
   CFG_ALTERED is set to true if cfg is altered.  */

static void
dom_opt_walk_stmts (struct dom_walk_data *walk_data,
		    basic_block bb,
		    tree parent_block_last_stmt ATTRIBUTE_UNUSED)
{
  block_stmt_iterator si;

  /* Optimize each statement within the basic block.  */
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      struct dom_walk_block_data *bd
	= VARRAY_TOP_GENERIC_PTR (walk_data->block_data_stack);
      /* Optimization may have exposed new symbols that need to be renamed
	 into SSA form.  If that happens, queue the statement to re-scan its
	 operands after finishing optimizing this block and its dominator
	 children.  Notice that we cannot re-scan the statement immediately
	 because that would change the statement's value number.  If the
	 statement had been added to AVAIL_EXPRS, we would not be able to
	 find it again.  */
      if (optimize_stmt (si, &bd->avail_exprs))
	VARRAY_PUSH_TREE (bd->stmts_to_rescan, bsi_stmt (si));
    }
}

/* Dump SSA statistics on FILE.  */

void
dump_dominator_optimization_stats (FILE *file)
{
  long n_exprs;

  fprintf (file, "Total number of statements:                   %6ld\n\n",
	   opt_stats.num_stmts);
  fprintf (file, "Exprs considered for dominator optimizations: %6ld\n",
           opt_stats.num_exprs_considered);

  n_exprs = opt_stats.num_exprs_considered;
  if (n_exprs == 0)
    n_exprs = 1;

  fprintf (file, "    Constants propagated:                     %6ld (%.0f%%)\n",
           opt_stats.num_const_prop, PERCENT (opt_stats.num_const_prop,
	                                      n_exprs));
  fprintf (file, "    Copies propagated:                        %6ld (%.0f%%)\n",
	   opt_stats.num_copy_prop, PERCENT (opt_stats.num_copy_prop,
					     n_exprs));
  fprintf (file, "    Redundant expressions eliminated:         %6ld (%.0f%%)\n",
	   opt_stats.num_re, PERCENT (opt_stats.num_re,
				      n_exprs));

  fprintf (file, "\nHash table statistics:\n");

  fprintf (file, "    avail_exprs: ");
  htab_statistics (file, avail_exprs);

  fprintf (file, "\n");
}


/* Dump SSA statistics on stderr.  */

void
debug_dominator_optimization_stats (void)
{
  dump_dominator_optimization_stats (stderr);
}


/* Dump statistics for the hash table HTAB.  */

static void
htab_statistics (FILE *file, htab_t htab)
{
  fprintf (file, "size %ld, %ld elements, %f collision/search ratio\n",
	   (long) htab_size (htab),
	   (long) htab_elements (htab),
	   htab_collisions (htab));
}

/* Enter a statement into the available expression hash table indicating
   that the condition COND is true.  */

static void
record_cond_is_true (tree cond, varray_type *block_avail_exprs_p)
{
  tree stmt;

  stmt = build (MODIFY_EXPR, boolean_type_node, integer_one_node, cond);
  lookup_avail_expr (stmt, block_avail_exprs_p, true);
}

/* Enter a statement into the available expression hash table indicating
   that the condition COND is false.  */

static void
record_cond_is_false (tree cond, varray_type *block_avail_exprs_p)
{
  tree stmt;

  stmt = build (MODIFY_EXPR, boolean_type_node, integer_zero_node, cond);
  lookup_avail_expr (stmt, block_avail_exprs_p, true);
}

/* STMT is a MODIFY_EXPR for which we were unable to find RHS in the
   hash tables.  Try to simplify the RHS using whatever equivalences
   we may have recorded.

   If we are able to simplify the RHS, then lookup the simplified form in
   the hash table and return the result.  Otherwise return NULL.  */

static tree
simplify_rhs_and_lookup_avail_expr (tree stmt,
				    varray_type *block_avail_exprs_p,
				    stmt_ann_t ann,
				    int insert)
{
  tree rhs = TREE_OPERAND (stmt, 1);
  enum tree_code rhs_code = TREE_CODE (rhs);
  tree result = NULL;

  /* If we have lhs = ~x, look and see if we earlier had x = ~y.
     In which case we can change this statement to be lhs = y.
     Which can then be copy propagated. 

     Similarly for negation.  */
  if ((rhs_code == BIT_NOT_EXPR || rhs_code == NEGATE_EXPR)
      && TREE_CODE (TREE_OPERAND (rhs, 0)) == SSA_NAME)
    {
      /* Get the definition statement for our RHS.  */
      tree rhs_def_stmt = SSA_NAME_DEF_STMT (TREE_OPERAND (rhs, 0));

      /* See if the RHS_DEF_STMT has the same form as our statement.  */
      if (TREE_CODE (rhs_def_stmt) == MODIFY_EXPR
	  && TREE_CODE (TREE_OPERAND (rhs_def_stmt, 1)) == rhs_code)
	{
	  tree rhs_def_operand;

	  rhs_def_operand = TREE_OPERAND (TREE_OPERAND (rhs_def_stmt, 1), 0);

	  /* Verify that RHS_DEF_OPERAND is a suitable SSA variable.  */
	  if (TREE_CODE (rhs_def_operand) == SSA_NAME
	      && ! SSA_NAME_OCCURS_IN_ABNORMAL_PHI (rhs_def_operand))
	    result = update_rhs_and_lookup_avail_expr (stmt,
						       rhs_def_operand,
						       block_avail_exprs_p,
						       ann,
						       insert);
	}
    }

  /* If we have z = (x OP C1), see if we earlier had x = y OP C2.
     If OP is associative, create and fold (y OP C2) OP C1 which
     should result in (y OP C3), use that as the RHS for the
     assignment.  */
  if (associative_tree_code (rhs_code)
      && TREE_CODE (TREE_OPERAND (rhs, 0)) == SSA_NAME
      && TREE_CONSTANT (TREE_OPERAND (rhs, 1)))
    {
      tree rhs_def_stmt = SSA_NAME_DEF_STMT (TREE_OPERAND (rhs, 0));

      /* See if the RHS_DEF_STMT has the same form as our statement.  */
      if (TREE_CODE (rhs_def_stmt) == MODIFY_EXPR
	  && TREE_CODE (TREE_OPERAND (rhs_def_stmt, 1)) == rhs_code)
	{
	  tree rhs_def_rhs = TREE_OPERAND (rhs_def_stmt, 1);
	  tree def_stmt_op0 = TREE_OPERAND (rhs_def_rhs, 0);
	  tree def_stmt_op1 = TREE_OPERAND (rhs_def_rhs, 1);

	  if (TREE_CODE (def_stmt_op0) == SSA_NAME
	      && ! SSA_NAME_OCCURS_IN_ABNORMAL_PHI (def_stmt_op0)
	      && TREE_CONSTANT (def_stmt_op1))
	    {
	      tree outer_const = TREE_OPERAND (rhs, 1);
	      tree type = TREE_TYPE (TREE_OPERAND (stmt, 0));
	      tree t;

	      /* Build and fold (Y OP C2) OP C1.  */
	      t = fold (build (rhs_code, type, rhs_def_rhs, outer_const));

	      /* If the result is a suitable looking gimple expression,
		 then use it instead of the original expression for STMT.  */
	      if (TREE_CODE (t) == SSA_NAME
		  || (TREE_CODE (t) == rhs_code
		      && TREE_CODE (TREE_OPERAND (t, 0)) == SSA_NAME
		      && TREE_CONSTANT (TREE_OPERAND (t, 1))))
		result = update_rhs_and_lookup_avail_expr (stmt, t,
							   block_avail_exprs_p,
						           ann,
							   insert);
	    }
	}
    }

  /* Transform TRUNC_DIV_EXPR and TRUNC_MOD_EXPR into RSHIFT_EXPR
     and BIT_AND_EXPR respectively if the first operand is greater
     than zero and the second operand is an exact power of two.  */
  if ((rhs_code == TRUNC_DIV_EXPR || rhs_code == TRUNC_MOD_EXPR)
      && INTEGRAL_TYPE_P (TREE_TYPE (TREE_OPERAND (rhs, 0)))
      && integer_pow2p (TREE_OPERAND (rhs, 1)))
    {
      tree cond, val;
      tree op = TREE_OPERAND (rhs, 0);

      cond = build (GT_EXPR, boolean_type_node, op, integer_zero_node);
      cond = build (COND_EXPR, void_type_node, cond, NULL, NULL);
      val = simplify_cond_and_lookup_avail_expr (cond, block_avail_exprs_p,
						 NULL, false);

      if (val && integer_onep (val))
	{
	  tree t;
	  tree op0 = TREE_OPERAND (rhs, 0);
	  tree op1 = TREE_OPERAND (rhs, 1);

	  if (rhs_code == TRUNC_DIV_EXPR)
	    t = build (RSHIFT_EXPR, TREE_TYPE (op0), op0,
		       build_int_2 (tree_log2 (op1), 0));
	  else
	    t = build (BIT_AND_EXPR, TREE_TYPE (op0), op0,
		       fold (build (MINUS_EXPR, TREE_TYPE (op1),
				    op1, integer_one_node)));

	  result = update_rhs_and_lookup_avail_expr (stmt, t,
						     block_avail_exprs_p,
						     ann, insert);
	}
    }

  /* Transform ABS (X) into X or -X as appropriate.  */
  if (rhs_code == ABS_EXPR
      && INTEGRAL_TYPE_P (TREE_TYPE (TREE_OPERAND (rhs, 0))))
    {
      tree cond, val;
      tree op = TREE_OPERAND (rhs, 0);
      tree type = TREE_TYPE (op);

      cond = build (LT_EXPR, boolean_type_node, op,
		    convert (type, integer_zero_node));
      cond = build (COND_EXPR, void_type_node, cond, NULL, NULL);
      val = simplify_cond_and_lookup_avail_expr (cond, block_avail_exprs_p,
						 NULL, false);

      if (val && (integer_onep (val) || integer_zerop (val)))
	{
	  tree t;

	  if (integer_onep (val))
	    t = build1 (NEGATE_EXPR, TREE_TYPE (op), op);
	  else
	    t = op;

	  result = update_rhs_and_lookup_avail_expr (stmt, t,
						     block_avail_exprs_p,
						     ann, insert);
	}
    }

  /* Optimize *"foo" into 'f'.  This is done here rather than
     in fold to avoid problems with stuff like &*"foo".  */
  if (TREE_CODE (rhs) == INDIRECT_REF || TREE_CODE (rhs) == ARRAY_REF)
    {
      tree t = fold_read_from_constant_string (rhs);

      if (t)
        result = update_rhs_and_lookup_avail_expr (stmt, t,
						   block_avail_exprs_p,
						   ann, insert);
    }

  return result;
}

/* COND is a condition of the form:

     x == const or x != const

   Look back to x's defining statement and see if x is defined as

     x = (type) y;

   If const is unchanged if we convert it to type, then we can build
   the equivalent expression:


      y == const or y != const

   Which may allow further optimizations.

   Return the equivalent comparison or NULL if no such equivalent comparison
   was found.  */

static tree
find_equivalent_equality_comparison (tree cond)
{
  tree op0 = TREE_OPERAND (cond, 0);
  tree op1 = TREE_OPERAND (cond, 1);
  tree def_stmt = SSA_NAME_DEF_STMT (op0);

  /* OP0 might have been a parameter, so first make sure it
     was defined by a MODIFY_EXPR.  */
  if (def_stmt && TREE_CODE (def_stmt) == MODIFY_EXPR)
    {
      tree def_rhs = TREE_OPERAND (def_stmt, 1);

      /* Now make sure the RHS of the MODIFY_EXPR is a typecast.  */
      if (TREE_CODE (def_rhs) == NOP_EXPR
	  && TREE_CODE (TREE_OPERAND (def_rhs, 0)) == SSA_NAME)
	{
	  tree def_rhs_inner = TREE_OPERAND (def_rhs, 0);
	  tree def_rhs_inner_type = TREE_TYPE (def_rhs_inner);
	  tree new;

	  if (TYPE_PRECISION (def_rhs_inner_type)
	      > TYPE_PRECISION (TREE_TYPE (def_rhs)))
	    return NULL;

	  /* What we want to prove is that if we convert OP1 to
	     the type of the object inside the NOP_EXPR that the
	     result is still equivalent to SRC. 

	     If that is true, the build and return new equivalent
	     condition which uses the source of the typecast and the
	     new constant (which has only changed its type).  */
	  new = fold (build1 (NOP_EXPR, def_rhs_inner_type, op1));
	  if (is_gimple_val (new)
	      && integer_onep (fold (build (EQ_EXPR, boolean_type_node,
					    new, op1))))
	    return build (TREE_CODE (cond), TREE_TYPE (cond),
			  def_rhs_inner, new);
	}
    }
  return NULL;
}

/* STMT is a COND_EXPR for which we could not trivially determine its
   result.  This routine attempts to find equivalent forms of the
   condition which we may be able to optimize better.  It also 
   uses simple value range propagation to optimize conditionals.  */

static tree
simplify_cond_and_lookup_avail_expr (tree stmt,
				     varray_type *block_avail_exprs_p,
				     stmt_ann_t ann,
				     int insert)
{
  tree cond = COND_EXPR_COND (stmt);

  if (TREE_CODE_CLASS (TREE_CODE (cond)) == '<')
    {
      tree op0 = TREE_OPERAND (cond, 0);
      tree op1 = TREE_OPERAND (cond, 1);

      if (TREE_CODE (op0) == SSA_NAME && is_gimple_min_invariant (op1))
	{
	  int limit;
	  tree low, high, cond_low, cond_high;
	  int lowequal, highequal, swapped, no_overlap, subset, cond_inverted;
	  varray_type vrp_records;
	  struct vrp_element *element;

	  /* First see if we have test of an SSA_NAME against a constant
	     where the SSA_NAME is defined by an earlier typecast which
	     is irrelevant when performing tests against the given
	     constant.  */
	  if (TREE_CODE (cond) == EQ_EXPR || TREE_CODE (cond) == NE_EXPR)
	    {
	      tree new_cond = find_equivalent_equality_comparison (cond);

	      if (new_cond)
		{
		  /* Update the statement to use the new equivalent
		     condition.  */
		  COND_EXPR_COND (stmt) = new_cond;
		  ann->modified = 1;

		  /* Lookup the condition and return its known value if it
		     exists.  */
		  new_cond = lookup_avail_expr (stmt, block_avail_exprs_p,
						insert);
		  if (new_cond)
		    return new_cond;

		  /* The operands have changed, so update op0 and op1.  */
		  op0 = TREE_OPERAND (cond, 0);
		  op1 = TREE_OPERAND (cond, 1);
		}
	    }

	  /* Consult the value range records for this variable (if they exist)
	     to see if we can eliminate or simplify this conditional. 

	     Note two tests are necessary to determine no records exist.
	     First we have to see if the virtual array exists, if it 
	     exists, then we have to check its active size. 

	     Also note the vast majority of conditionals are not testing
	     a variable which has had its range constrained by an earlier
	     conditional.  So this filter avoids a lot of unnecessary work.  */
	  vrp_records = VARRAY_GENERIC_PTR (vrp_data, SSA_NAME_VERSION (op0));
	  if (vrp_records == NULL)
	    return NULL;

	  limit = VARRAY_ACTIVE_SIZE (vrp_records);

	  /* If we have no value range records for this variable, or we are
	     unable to extract a range for this condition, then there is
	     nothing to do.  */
	  if (limit == 0
	      || ! extract_range_from_cond (cond, &cond_high,
					    &cond_low, &cond_inverted))
	    return NULL;

	  /* We really want to avoid unnecessary computations of range
	     info.  So all ranges are computed lazily; this avoids a
	     lot of unnecessary work.  ie, we record the conditional,
	     but do not process how it constrains the variable's 
	     potential values until we know that processing the condition
	     could be helpful.

	     However, we do not want to have to walk a potentially long
	     list of ranges, nor do we want to compute a variable's
	     range more than once for a given path.

	     Luckily, each time we encounter a conditional that can not
	     be otherwise optimized we will end up here and we will
	     compute the necessary range information for the variable
	     used in this condition.

	     Thus you can conclude that there will never be more than one
	     conditional associated with a variable which has not been
	     processed.  So we never need to merge more than one new
	     conditional into the current range. 

	     These properties also help us avoid unnecessary work.  */
	   element
	     = (struct vrp_element *)VARRAY_GENERIC_PTR (vrp_records, limit - 1);

	  if (element->high && element->low)
	    {
	      /* The last element has been processed, so there is no range
		 merging to do, we can simply use the high/low values
		 recorded in the last element.  */
	      low = element->low;
	      high = element->high;
	    }
	  else
	    {
	      tree tmp_high, tmp_low;
	      int dummy;

	      /* The last element has not been processed.  Process it now.  */
	      extract_range_from_cond (element->cond, &tmp_high,
				       &tmp_low, &dummy);
	  
	      /* If this is the only element, then no merging is necessary, 
		 the high/low values from extract_range_from_cond are all
		 we need.  */
	      if (limit == 1)
		{
		  low = tmp_low;
		  high = tmp_high;
		}
	      else
		{
		  /* Get the high/low value from the previous element.  */
		  struct vrp_element *prev
		    = (struct vrp_element *)VARRAY_GENERIC_PTR (vrp_records,
								limit - 2);
		  low = prev->low;
		  high = prev->high;

		  /* Merge in this element's range with the range from the
		     previous element.

		     The low value for the merged range is the maximum of
		     the previous low value and the low value of this record.

		     Similarly the high value for the merged range is the
		     minimum of the previous high value and the high value of
		     this record.  */
		  low = fold (build (MAX_EXPR, TREE_TYPE (low),
				     low, tmp_low));
		  high = fold (build (MIN_EXPR, TREE_TYPE (high),
				      high, tmp_high));

		}

	      /* And record the computed range.  */
	      element->low = low;
	      element->high = high;

	    }

	  /* After we have constrained this variable's potential values,
	     we try to determine the result of the given conditional.

	     To simplify later tests, first determine if the current
	     low value is the same low value as the conditional.
	     Similarly for the current high value and the high value
	     for the conditional.  */
	  lowequal = integer_onep (fold (build (EQ_EXPR, boolean_type_node,
						low, cond_low)));
	  highequal = integer_onep (fold (build (EQ_EXPR, boolean_type_node,
						 high, cond_high)));

	  if (lowequal && highequal)
	    return (cond_inverted ? boolean_false_node : boolean_true_node);

	  /* To simplify the overlap/subset tests below we may want
	     to swap the two ranges so that the larger of the two
	     ranges occurs "first".  */
	  swapped = 0;
	  if (integer_onep (fold (build (GT_EXPR, boolean_type_node,
					 low, cond_low)))
	      || (lowequal 
		  && integer_onep (fold (build (GT_EXPR, boolean_type_node,
						cond_high, high)))))
	    {
	      tree temp;

	      swapped = 1;
	      temp = low;
	      low = cond_low;
	      cond_low = temp;
	      temp = high;
	      high = cond_high;
	      cond_high = temp;
	    }

	  /* Now determine if there is no overlap in the ranges
	     or if the second range is a subset of the first range.  */
	  no_overlap = integer_onep (fold (build (LT_EXPR,
						   boolean_type_node,
						   high, cond_low)));
	  subset = integer_onep (fold (build (LE_EXPR, boolean_type_node,
					      cond_high, high)));

	  /* If there was no overlap in the ranges, then this conditional
	     always has a false value (unless we had to invert this
	     conditional, in which case it always has a true value).  */
	  if (no_overlap)
	    return (cond_inverted ? boolean_true_node : boolean_false_node);

	  /* If the current range is a subset of the condition's range,
	     then this conditional always has a true value (unless we
	     had to invert this conditional, in which case it always
	     has a true value).  */
	  if (subset && swapped)
	    return (cond_inverted ? boolean_false_node : boolean_true_node);

	  /* We were unable to determine the result of the conditional.
	     However, we may be able to simplify the conditional.  First
	     merge the ranges in the same manner as range merging above.  */
	  low = fold (build (MAX_EXPR, TREE_TYPE (low), low, cond_low));
	  high = fold (build (MIN_EXPR, TREE_TYPE (high), high, cond_high));
	  
	  /* If the range has converged to a single point, then turn this
	     into an equality comparison.  */
	  if (TREE_CODE (cond) != EQ_EXPR
	      && TREE_CODE (cond) != NE_EXPR
	      && integer_onep (fold (build (EQ_EXPR, boolean_type_node,
					    high, low))))
	    {
	      TREE_SET_CODE (cond, EQ_EXPR);
	      TREE_OPERAND (cond, 1) = high;
	    }
	}
    }
  return 0;
}

/* Const/copy propagate into STMT's USES, VUSES, and the RHS of VDEFs. 

   Return nonzero if new symbols may have been exposed.  */

static bool
cprop_into_stmt (tree stmt)
{
  size_t i;
  varray_type defs, uses, vuses, vdefs, operand_tables[4], *table;
  bool may_have_exposed_new_symbols = false;
  stmt_ann_t ann = stmt_ann (stmt);

  defs = def_ops (ann);
  uses = use_ops (ann);
  vuses = vuse_ops (ann);
  vdefs = vdef_ops (ann);

  /* Const/copy propagate into USES, VUSES and the RHS of VDEFs.  */
  operand_tables[0] = uses;
  operand_tables[1] = vuses;
  operand_tables[2] = vdefs;
  operand_tables[3] = NULL;
  for (table = operand_tables; *table; table++)
    for (i = 0; *table && i < VARRAY_ACTIVE_SIZE (*table); i++)
      {
	tree val;
	tree *op_p;

	/* Get a pointer to the operand we want to const/copy propagate
	   into.  Each table has a slightly different structure.  */
	if (*table == uses)
	  op_p = VARRAY_TREE_PTR (uses, i);
	else if (*table == vuses)
	  op_p = (tree *) &VARRAY_TREE (vuses, i);
	else
	  op_p = (tree *) &VDEF_OP (VARRAY_TREE (vdefs, i));

	/* If the operand is not an ssa variable, then there is nothing
	   to do.  */
	if (TREE_CODE (*op_p) != SSA_NAME)
	  continue;

	/* If the operand has a known constant value or it is known to be a
	   copy of some other variable, use the value or copy stored in
	   CONST_AND_COPIES.  */
	opt_stats.num_exprs_considered++;
	val = get_value_for (*op_p);
	if (val)
	  {
	    /* Do not change the base variable in the virtual operand
	       tables.  That would make it impossible to reconstruct
	       the renamed virtual operand if we later modify this
	       statement.  Also only allow the new value to be an SSA_NAME
	       for propagation into virtual operands.  */
	    if ((*table == vuses || *table == vdefs)
		&& (get_virtual_var (val) != get_virtual_var (*op_p)
		    || TREE_CODE (val) != SSA_NAME))
	      continue;

	    /* Certain operands are not allowed to be copy propagated due
	       to their interaction with exception handling and some
	       GCC extensions.  */
	    if (TREE_CODE (val) == SSA_NAME
		&& !may_propagate_copy (*op_p, val))
	      continue;

	    /* Gather statistics.  */
	    if (is_gimple_min_invariant (val))
	      opt_stats.num_const_prop++;
	    else
	      opt_stats.num_copy_prop++;

	    /* Dump details.  */
	    if (dump_file && (dump_flags & TDF_DETAILS))
	      {
		fprintf (dump_file, "  Replaced '");
		print_generic_expr (dump_file, *op_p, 0);
		fprintf (dump_file, "' with %s '",
			 (TREE_CODE (val) != SSA_NAME
			    ? "constant" : "variable"));
		print_generic_expr (dump_file, val, 0);
		fprintf (dump_file, "'\n");
	      }

	    /* If VAL is an ADDR_EXPR or a constant of pointer type, note
	       that we may need to have a second SSA pass to rename
	       variables exposed by the folding of *&VAR expressions.  */
	    if (TREE_CODE (val) == ADDR_EXPR
		|| (POINTER_TYPE_P (TREE_TYPE (*op_p))
		    && is_gimple_min_invariant (val)))
	      may_have_exposed_new_symbols = true;

	    if (TREE_CODE (val) == SSA_NAME)
	      propagate_copy (op_p, val, stmt_ann (stmt)->scope);
	    else
	      *op_p = val;

	    /* And note that we modified this statement.  This is now
	       safe, even if we changed virtual operands since we will
	       rescan the statement and rewrite its operands again.  */
	    ann->modified = 1;
	}
    }

  return may_have_exposed_new_symbols;
}

/* Propagate known constants/copies into PHI nodes.  */

static void
cprop_into_phis (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
		 basic_block bb,
		 tree parent_block_last_stmt ATTRIBUTE_UNUSED)
{
  edge e;

  /* This can get rather expensive if the implementation is naive in
     how it finds the phi alternative associated with a particular edge.  */
  for (e = bb->succ; e; e = e->succ_next)
    {
      tree phi;
      int phi_num_args;
      int hint;

      /* If this is an abnormal edge, then we do not want to copy propagate
	 into the PHI alternative associated with this edge.  */
      if (e->flags & EDGE_ABNORMAL)
	continue;

      phi = phi_nodes (e->dest);
      if (! phi)
	continue;

      /* There is no guarantee that for any two PHI nodes in a block that
	 the phi alternative associated with a particular edge will be
	 at the same index in the phi alternative array.

	 However, it is very likely they will be the same.  So we keep
	 track of the index of the alternative where we found the edge in
	 the previous phi node and check that index first in the next
	 phi node.  If that hint fails, then we actually search all
	 the entries.  */
      phi_num_args = PHI_NUM_ARGS (phi);
      hint = phi_num_args;
      for ( ; phi; phi = TREE_CHAIN (phi))
	{
	  int i;
	  tree new;
	  tree *orig_p;

	  /* If the hint is valid (!= phi_num_args), see if it points
	     us to the desired phi alternative.  */
	  if (hint != phi_num_args && PHI_ARG_EDGE (phi, hint) == e)
	    ;
	  else
	    {
	      /* The hint was either invalid or did not point to the
		 correct phi alternative.  Search all the alternatives
		 for the correct one.  Update the hint.  */
	      for (i = 0; i < phi_num_args; i++)
		if (PHI_ARG_EDGE (phi, i) == e)
		  break;
	      hint = i;
	    }

#ifdef ENABLE_CHECKING
	  /* If we did not find the proper alternative, then something is
	     horribly wrong.  */
	  if (hint == phi_num_args)
	    abort ();
#endif

	  /* The alternative may be associated with a constant, so verify
	     it is an SSA_NAME before doing anything with it.  */
	  orig_p = &PHI_ARG_DEF (phi, hint);
	  if (TREE_CODE (*orig_p) != SSA_NAME)
	    continue;

	  /* If we have *ORIG_P in our constant/copy table, then replace
	     ORIG_P with its value in our constant/copy table.  */
	  new = get_value_for (*orig_p);
	  if (new
	      && (TREE_CODE (new) == SSA_NAME || is_gimple_min_invariant (new))
	      && may_propagate_copy (*orig_p, new))
	    *orig_p = new;
	}
    }
}

/* Search for redundant computations in STMT.  If any are found, then
   replace them with the variable holding the result of the computation.

   If safe, record this expression into the available expression hash
   table.  */

static bool
eliminate_redundant_computations (tree stmt,
				  varray_type *block_avail_exprs_p,
				  stmt_ann_t ann)
{
  varray_type vdefs = vdef_ops (ann);
  tree *expr_p, def = NULL_TREE;
  bool insert = true;
  tree cached_lhs;
  bool retval = false;

  if (TREE_CODE (stmt) == MODIFY_EXPR)
    def = TREE_OPERAND (stmt, 0);

  /* Certain expressions on the RHS can be optimized away, but can not
     themselves be entered into the hash tables.   */
  if (ann->makes_aliased_stores
      || ! def
      || TREE_CODE (def) != SSA_NAME
      || SSA_NAME_OCCURS_IN_ABNORMAL_PHI (def)
      || vdefs)
    insert = false;

  /* Check if the expression has been computed before.  */
  cached_lhs = lookup_avail_expr (stmt, block_avail_exprs_p,
				  insert);

  /* If this is an assignment and the RHS was not in the hash table,
     then try to simplify the RHS and lookup the new RHS in the
     hash table.  */
  if (! cached_lhs && TREE_CODE (stmt) == MODIFY_EXPR)
    cached_lhs = simplify_rhs_and_lookup_avail_expr (stmt,
						     block_avail_exprs_p,
						     ann,
						     insert);
  /* Similarly if this is a COND_EXPR and we did not find its
     expression in the hash table, simplify the condition and
     try again.  */
  else if (! cached_lhs && TREE_CODE (stmt) == COND_EXPR)
    cached_lhs = simplify_cond_and_lookup_avail_expr (stmt,
						      block_avail_exprs_p,
						      ann,
						      insert);
  /* We could do the same with SWITCH_EXPRs in the future.  */

  opt_stats.num_exprs_considered++;

  /* Get a pointer to the expression we are trying to optimize.  */
  if (TREE_CODE (stmt) == COND_EXPR)
    expr_p = &COND_EXPR_COND (stmt);
  else if (TREE_CODE (stmt) == SWITCH_EXPR)
    expr_p = &SWITCH_COND (stmt);
  else if (TREE_CODE (stmt) == RETURN_EXPR && TREE_OPERAND (stmt, 0))
    expr_p = &TREE_OPERAND (TREE_OPERAND (stmt, 0), 1);
  else
    expr_p = &TREE_OPERAND (stmt, 1);

  /* It is safe to ignore types here since we have already done
     type checking in the hashing and equality routines.  In fact
     type checking here merely gets in the way of constant
     propagation.  Also, make sure that it is safe to propagate
     CACHED_LHS into *EXPR_P.  */
  if (cached_lhs
      && (TREE_CODE (cached_lhs) != SSA_NAME
	  || may_propagate_copy (cached_lhs, *expr_p)))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "  Replaced redundant expr '");
	  print_generic_expr (dump_file, *expr_p, 0);
	  fprintf (dump_file, "' with '");
	  print_generic_expr (dump_file, cached_lhs, 0);
	   fprintf (dump_file, "'\n");
	}

      opt_stats.num_re++;

#if defined ENABLE_CHECKING
      if (TREE_CODE (cached_lhs) != SSA_NAME
	  && !is_gimple_min_invariant (cached_lhs))
	abort ();
#endif

      if (TREE_CODE (cached_lhs) == SSA_NAME)
	fixup_var_scope (cached_lhs, ann->scope);
      else if (TREE_CODE (cached_lhs) == ADDR_EXPR
	       || (POINTER_TYPE_P (TREE_TYPE (*expr_p))
		   && is_gimple_min_invariant (cached_lhs)))
	retval = true;

      *expr_p = cached_lhs;
      ann->modified = 1;
    }
  return retval;
}

/* STMT, a MODIFY_EXPR, may create certain equivalences, in either
   the available expressions table or the const_and_copies table.
   Detect and record those equivalences.  */

static void
record_equivalences_from_stmt (tree stmt,
			       varray_type *block_avail_exprs_p,
			       int may_optimize_p,
			       stmt_ann_t ann)
{
  tree lhs = TREE_OPERAND (stmt, 0);
  enum tree_code lhs_code = TREE_CODE (lhs);
  int i;

  if (lhs_code == SSA_NAME)
    {
      tree rhs = TREE_OPERAND (stmt, 1);

      /* Strip away any useless type conversions.  */
      while (tree_ssa_useless_type_conversion (rhs))
	rhs = TREE_OPERAND (rhs, 0);

      /* If the RHS of the assignment is a constant or another variable that
	 may be propagated, register it in the CONST_AND_COPIES table.  */
      if (may_optimize_p
	  && (TREE_CODE (rhs) == SSA_NAME
	      || is_gimple_min_invariant (rhs)))
	set_value_for (lhs, rhs);

      /* alloca never returns zero and the address of a non-weak symbol
	 is never zero.  NOP_EXPRs can be completely stripped as they
	 do not affect this equivalence.  */
      while (TREE_CODE (rhs) == NOP_EXPR)
        rhs = TREE_OPERAND (rhs, 0);

      if (alloca_call_p (rhs)
          || (TREE_CODE (rhs) == ADDR_EXPR
	      && DECL_P (TREE_OPERAND (rhs, 0))
	      && ! DECL_WEAK (TREE_OPERAND (rhs, 0))))
	{
	  tree cond;

	  cond = build (EQ_EXPR, boolean_type_node, lhs, null_pointer_node);
	  record_cond_is_false (cond, block_avail_exprs_p);

	  cond = build (NE_EXPR, boolean_type_node, lhs, null_pointer_node);
	  record_cond_is_true (cond, block_avail_exprs_p);
	}
    }

  /* Look at both sides for pointer dereferences.  If we find one, then
     the pointer must be nonnull and we can enter that equivalence into
     the hash tables.  */
  for (i = 0; i < 2; i++)
    {
      tree t = TREE_OPERAND (stmt, i);

      /* Strip away any COMPONENT_REFs.  */
      while (TREE_CODE (t) == COMPONENT_REF)
        t = TREE_OPERAND (t, 0);

      /* Now see if this is a pointer dereference.  */
      if (TREE_CODE (t) == INDIRECT_REF)
        {
	  tree op = TREE_OPERAND (t, 0);
	  tree cond;

	  /* If the pointer is a SSA variable, then enter new
	     equivalences into the hash table.  */
	  if (TREE_CODE (op) == SSA_NAME)
	    {
	      cond = build (EQ_EXPR, boolean_type_node, op, null_pointer_node);
	      record_cond_is_false (cond, block_avail_exprs_p);

	      cond = build (NE_EXPR, boolean_type_node, op, null_pointer_node);
	      record_cond_is_true (cond, block_avail_exprs_p);
	    }
	}
    }

  /* A memory store, even an aliased store, creates a useful
     equivalence.  By exchanging the LHS and RHS, creating suitable
     vops and recording the result in the available expression table,
     we may be able to expose more redundant loads.  */
  if (!ann->has_volatile_ops
      && (TREE_CODE (TREE_OPERAND (stmt, 1)) == SSA_NAME
	  || is_gimple_min_invariant (TREE_OPERAND (stmt, 1)))
      && !is_gimple_reg (lhs))
    {
      tree rhs = TREE_OPERAND (stmt, 1);
      tree new;
      size_t j;

      /* FIXME: If the LHS of the assignment is a bitfield and the RHS
         is a constant, we need to adjust the constant to fit into the
         type of the LHS.  This fixes gcc.c-torture/execute/921016-1.c
	 and should not be necessary if GCC represented bitfields
	 properly.  */
      if (TREE_CONSTANT (rhs)
	  && lhs_code == COMPONENT_REF
	  && DECL_BIT_FIELD (TREE_OPERAND (lhs, 1)))
	rhs = widen_bitfield (rhs, TREE_OPERAND (lhs, 1), lhs);

      if (rhs)
	{
	  varray_type vdefs = vdef_ops (ann);

	  /* Build a new statement with the RHS and LHS exchanged.  */
	  new = build (MODIFY_EXPR, TREE_TYPE (stmt), rhs, lhs);

	  /* Get an annotation and set up the real operands.  */
	  get_stmt_ann (new);
	  get_stmt_operands (new);

	  /* Clear out the virtual operands on the new statement, we are
	     going to set them explicitly below.  */
	  get_stmt_ann (new)->vops = NULL;

	  /* For each VDEF on the original statement, we want to create a
	     VUSE of the VDEF result on the new statement.  */
	  for (j = 0; vdefs && j < VARRAY_ACTIVE_SIZE (vdefs); j++)
	    {
	      tree op = VDEF_RESULT (VARRAY_TREE (vdefs, j));
	      add_vuse (op, new, NULL);
	    }

	  /* Finally enter the statement into the available expression
	     table.  */
	  lookup_avail_expr (new, block_avail_exprs_p, true);
	}
    }
}

/* Optimize the statement pointed by iterator SI into SSA form. 
   
   BLOCK_AVAIL_EXPRS_P points to a stack with all the expressions that have
   been computed in this block and are available in children blocks to
   be reused.

   We try to perform some simplistic global redundancy elimination and
   constant propagation:

   1- To detect global redundancy, we keep track of expressions that have
      been computed in this block and its dominators.  If we find that the
      same expression is computed more than once, we eliminate repeated
      computations by using the target of the first one.

   2- Constant values and copy assignments.  This is used to do very
      simplistic constant and copy propagation.  When a constant or copy
      assignment is found, we map the value on the RHS of the assignment to
      the variable in the LHS in the CONST_AND_COPIES table.  */

static bool
optimize_stmt (block_stmt_iterator si, varray_type *block_avail_exprs_p)
{
  stmt_ann_t ann;
  tree stmt;
  varray_type vdefs;
  bool may_optimize_p;
  bool may_have_exposed_new_symbols = false;

  stmt = bsi_stmt (si);
  if (IS_EMPTY_STMT (stmt))
    return false;

  get_stmt_operands (stmt);
  ann = stmt_ann (stmt);
  vdefs = vdef_ops (ann);
  opt_stats.num_stmts++;
  may_have_exposed_new_symbols = false;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Optimizing statement ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  /* Const/copy propagate into USES, VUSES and the RHS of VDEFs.  */
  may_have_exposed_new_symbols = cprop_into_stmt (stmt);

  /* If the statement has been modified with constant replacements,
     fold its RHS before checking for redundant computations.  */
  if (ann->modified)
    {
      /* Try to fold the statement making sure that STMT is kept
	 up to date.  */
      if (fold_stmt (bsi_stmt_ptr (si)))
	{
	  stmt = bsi_stmt (si);
	  ann = stmt_ann (stmt);
	}

      /* Constant/copy propagation above may change the set of 
	 virtual operands associated with this statement.  Folding
	 may remove the need for some virtual operands.

	 Indicate we will need to rescan and rewrite the statement.  */
      may_have_exposed_new_symbols = true;
    }

  /* Check for redundant computations.  Do this optimization only
     for assignments that have no volatile ops and conditionals.  */
  may_optimize_p = (!ann->has_volatile_ops
		    && ((TREE_CODE (stmt) == RETURN_EXPR
			 && TREE_OPERAND (stmt, 0)
			 && TREE_CODE (TREE_OPERAND (stmt, 0)) == MODIFY_EXPR
			 && ! (TREE_SIDE_EFFECTS
			       (TREE_OPERAND (TREE_OPERAND (stmt, 0), 1))))
			|| (TREE_CODE (stmt) == MODIFY_EXPR
			    && ! TREE_SIDE_EFFECTS (TREE_OPERAND (stmt, 1)))
			|| TREE_CODE (stmt) == COND_EXPR
			|| TREE_CODE (stmt) == SWITCH_EXPR));

  if (may_optimize_p)
    may_have_exposed_new_symbols
      |= eliminate_redundant_computations (stmt, block_avail_exprs_p, ann);

  /* Record any additional equivalences created by this statement.  */
  if (TREE_CODE (stmt) == MODIFY_EXPR)
    record_equivalences_from_stmt (stmt, block_avail_exprs_p,
				   may_optimize_p, ann);

  /* If STMT is a COND_EXPR and it was modified, then we may know
     where it goes.  In which case we can remove some edges, simplify
     some PHI nodes, maybe even avoid optimizing some blocks completely,
     etc.  */
  if (ann->modified)
    {
      if (TREE_CODE (stmt) == COND_EXPR)
	cfg_altered |= cleanup_cond_expr_graph (bb_for_stmt (stmt), stmt);

      if (TREE_CODE (stmt) == SWITCH_EXPR)
	cfg_altered |= cleanup_switch_expr_graph (bb_for_stmt (stmt), stmt);
    }
                                                                                
  return may_have_exposed_new_symbols;
}

/* Replace the RHS of STMT with NEW_RHS.  If RHS can be found in the
   available expression hashtable, then return the LHS from the hash
   table.

   If INSERT is true, then we also update the available expression
   hash table to account for the changes made to STMT.  */

static tree
update_rhs_and_lookup_avail_expr (tree stmt, tree new_rhs, 
				  varray_type *block_avail_exprs_p,
				  stmt_ann_t ann,
				  bool insert)
{
  tree cached_lhs = NULL;

  /* Remove the old entry from the hash table.  */
  if (insert)
    htab_remove_elt (avail_exprs, stmt);

  /* Now update the RHS of the assignment.  */
  TREE_OPERAND (stmt, 1) = new_rhs;

  /* Now lookup the updated statement in the hash table.  */
  cached_lhs = lookup_avail_expr (stmt, block_avail_exprs_p, insert);

  /* We have now called lookup_avail_expr twice with two different
     versions of this same statement, once in optimize_stmt, once here.

     We know the call in optimize_stmt did not find an existing entry
     in the hash table, so a new entry was created.  At the same time
     this statement was pushed onto the BLOCK_AVAIL_EXPRS varray. 

     If this call failed to find an existing entry on the hash table,
     then the new version of this statement was entered into the
     hash table.  And this statement was pushed onto BLOCK_AVAIL_EXPR
     for the second time.  So there are two copies on BLOCK_AVAIL_EXPRs

     If this call succeeded, we still have one copy of this statement
     on the BLOCK_AVAIL_EXPRs varray.

     For both cases, we need to pop the most recent entry off the
     BLOCK_AVAIL_EXPRs varray.  For the case where we never found this
     statement in the hash tables, that will leave precisely one
     copy of this statement on BLOCK_AVAIL_EXPRs.  For the case where
     we found a copy of this statement in the second hash table lookup
     we want _no_ copies of this statement in BLOCK_AVAIL_EXPRs.  */
  if (insert)
    VARRAY_POP (*block_avail_exprs_p);

  /* And make sure we record the fact that we modified this
     statement.  */
  ann->modified = 1;

  return cached_lhs;
}

/* Search for an existing instance of STMT in the AVAIL_EXPRS table.  If
   found, return its LHS. Otherwise insert STMT in the table and return
   NULL_TREE.

   Also, when an expression is first inserted in the AVAIL_EXPRS table, it
   is also added to the stack pointed by BLOCK_AVAIL_EXPRS_P, so that they
   can be removed when we finish processing this block and its children.

   NOTE: This function assumes that STMT is a MODIFY_EXPR node that
   contains no CALL_EXPR on its RHS and makes no volatile nor
   aliased references.  */

static tree
lookup_avail_expr (tree stmt, varray_type *block_avail_exprs_p, bool insert)
{
  void **slot;
  tree rhs;
  tree lhs;
  tree temp;

  /* Find the location of the expression we care about.  Unfortunately,
     its location differs depending on the type of statement we are
     examining.  */
  if (TREE_CODE (stmt) == COND_EXPR)
    rhs = COND_EXPR_COND (stmt);
  else if (TREE_CODE (stmt) == SWITCH_EXPR)
    rhs = SWITCH_COND (stmt);
  else if (TREE_CODE (stmt) == RETURN_EXPR
	   && TREE_OPERAND (stmt, 0))
    rhs = TREE_OPERAND (TREE_OPERAND (stmt, 0), 1);
  else
    rhs = TREE_OPERAND (stmt, 1);

  /* Don't bother remembering constant assignments and copy operations.
     Constants and copy operations are handled by the constant/copy propagator
     in optimize_stmt.  */
  if (TREE_CODE (rhs) == SSA_NAME
      || is_gimple_min_invariant (rhs))
    return NULL_TREE;

  slot = htab_find_slot (avail_exprs, stmt, (insert ? INSERT : NO_INSERT));
  if (slot == NULL)
    return NULL_TREE;

  if (*slot == NULL)
    {
      *slot = (void *) stmt;
      VARRAY_PUSH_TREE (*block_avail_exprs_p, stmt);
      return NULL_TREE;
    }

  /* Extract the LHS of the assignment so that it can be used as the current
     definition of another variable.  */
  lhs = TREE_OPERAND ((tree) *slot, 0);

  /* See if the LHS appears in the CONST_AND_COPIES table.  If it does, then
     use the value from the const_and_copies table.  */
  if (TREE_CODE (lhs) == SSA_NAME)
    {
      temp = get_value_for (lhs);
      if (temp)
	lhs = temp;
    }

  return lhs;
}

/* Given a condition COND, record into HI_P, LO_P and INVERTED_P the
   range of values that result in the conditional having a true value.

   Return true if we are successful in extracting a range from COND and
   false if we are unsuccessful.  */

static bool
extract_range_from_cond (tree cond, tree *hi_p, tree *lo_p, int *inverted_p)
{
  tree op1 = TREE_OPERAND (cond, 1);
  tree high, low, type;
  int inverted;
  
  /* Experiments have shown that it's rarely, if ever useful to
     record ranges for enumerations.  Presumably this is due to
     the fact that they're rarely used directly.  They are typically
     cast into an integer type and used that way.  */
  if (TREE_CODE (TREE_TYPE (op1)) != INTEGER_TYPE)
    return 0;

  type = TREE_TYPE (op1);

  switch (TREE_CODE (cond))
    {
    case EQ_EXPR:
      high = low = op1;
      inverted = 0;
      break;

    case NE_EXPR:
      high = low = op1;
      inverted = 1;
      break;

    case GE_EXPR:
      low = op1;
      high = TYPE_MAX_VALUE (type);
      inverted = 0;
      break;

    case GT_EXPR:
      low = fold (build (PLUS_EXPR, TREE_TYPE (op1), op1, integer_one_node));
      high = TYPE_MAX_VALUE (type);
      inverted = 0;
      break;

    case LE_EXPR:
      high = op1;
      low = TYPE_MIN_VALUE (type);
      inverted = 0;
      break;

    case LT_EXPR:
      high = fold (build (MINUS_EXPR, TREE_TYPE (op1), op1, integer_one_node));
      low = TYPE_MIN_VALUE (type);
      inverted = 0;
      break;

    default:
      return 0;
    }

  *hi_p = high;
  *lo_p = low;
  *inverted_p = inverted;
  return 1;
}

/* Record a range created by COND for basic block BB.  */

static void
record_range (tree cond, basic_block bb, varray_type *vrp_variables_p)
{
  /* We explicitly ignore NE_EXPRs.  They rarely allow for meaningful
     range optimizations and significantly complicate the implementation.  */
  if (TREE_CODE_CLASS (TREE_CODE (cond)) == '<'
      && TREE_CODE (cond) != NE_EXPR
      && TREE_CODE (TREE_TYPE (TREE_OPERAND (cond, 1))) == INTEGER_TYPE)
    {
      struct vrp_element *element = ggc_alloc (sizeof (struct vrp_element));
      int ssa_version = SSA_NAME_VERSION (TREE_OPERAND (cond, 0));

      varray_type *vrp_records_p
	= (varray_type *)&VARRAY_GENERIC_PTR (vrp_data, ssa_version);

      element->low = NULL;
      element->high = NULL;
      element->cond = cond;
      element->bb = bb;

      if (*vrp_records_p == NULL)
	{
	  VARRAY_GENERIC_PTR_INIT (*vrp_records_p, 2, "vrp records");
	  VARRAY_GENERIC_PTR (vrp_data, ssa_version) = *vrp_records_p;
	}
      
      VARRAY_PUSH_GENERIC_PTR (*vrp_records_p, element);
      VARRAY_PUSH_TREE (*vrp_variables_p, TREE_OPERAND (cond, 0));
    }
}

/* Given a conditional statement IF_STMT, return the assignment 'X = Y'
   known to be true depending on which arm of IF_STMT is taken.

   Not all conditional statements will result in a useful assignment.
   Return NULL_TREE in that case.

   Also enter into the available expression table statements of
   the form:

     TRUE ARM		FALSE ARM
     1 = cond		1 = cond'
     0 = cond'		0 = cond

   This allows us to lookup the condition in a dominated block and
   get back a constant indicating if the condition is true.  */

static tree
get_eq_expr_value (tree if_stmt, int true_arm, varray_type *block_avail_exprs_p,
		   basic_block bb, varray_type *vrp_variables_p)
{
  tree cond, value;

  cond = COND_EXPR_COND (if_stmt);
  value = NULL;

  /* If the conditional is a single variable 'X', return 'X = 1' for
     the true arm and 'X = 0' on the false arm.   */
  if (TREE_CODE (cond) == SSA_NAME)
    return build (MODIFY_EXPR, TREE_TYPE (cond), cond,
		  (true_arm ? integer_one_node : integer_zero_node));

  /* If we have a comparison expression, then record its result into
     the available expression table.  */
  if (TREE_CODE_CLASS (TREE_CODE (cond)) == '<')
    {
      tree op0 = TREE_OPERAND (cond, 0);
      tree op1 = TREE_OPERAND (cond, 1);

      if (TREE_CODE (op0) == SSA_NAME
	  && (is_gimple_min_invariant (op1) || TREE_CODE (op1) == SSA_NAME))
	{
	  tree inverted = invert_truthvalue (cond);

	  /* When we find an available expression in the hash table, we replace
	     the expression with the LHS of the statement in the hash table.

	     So, we want to build statements such as "1 = <condition>" on the
	     true arm and "0 = <condition>" on the false arm.  That way if we
	     find the expression in the table, we will replace it with its
	     known constant value.  Also insert inversions of the result and
	     condition into the hash table.  */
	  if (true_arm)
	    {
	      record_cond_is_true (cond, block_avail_exprs_p);
	      record_cond_is_false (inverted, block_avail_exprs_p);

	      if (TREE_CONSTANT (op1))
		record_range (cond, bb, vrp_variables_p);

		/* If the conditional is of the form 'X == Y', return 'X = Y'
		   for the true arm.  */
	      if (TREE_CODE (cond) == EQ_EXPR)
		value = build (MODIFY_EXPR, TREE_TYPE (cond), op0, op1);
	    }
	  else
	    {

	      record_cond_is_true (inverted, block_avail_exprs_p);
	      record_cond_is_false (cond, block_avail_exprs_p);

	      if (TREE_CONSTANT (op1))
		record_range (inverted, bb, vrp_variables_p);

		/* If the conditional is of the form 'X != Y', return 'X = Y'
		   for the false arm.  */
	      if (TREE_CODE (cond) == NE_EXPR)
		value = build (MODIFY_EXPR, TREE_TYPE (cond), op0, op1);
	    }
	}
    }

  return value;
}


/* Hashing and equality functions for AVAIL_EXPRS.  The table stores
   MODIFY_EXPR statements.  We compute a value number for expressions using
   the code of the expression and the SSA numbers of its operands.  */

static hashval_t
avail_expr_hash (const void *p)
{
  hashval_t val = 0;
  tree rhs;
  size_t i;
  varray_type ops;
  tree stmt = (tree) p;

  /* Find the location of the expression we care about.  Unfortunately,
     its location differs depending on the type of statement we are
     examining.  */
  if (TREE_CODE (stmt) == COND_EXPR)
    rhs = COND_EXPR_COND (stmt);
  else if (TREE_CODE (stmt) == SWITCH_EXPR)
    rhs = SWITCH_COND (stmt);
  else if (TREE_CODE (stmt) == RETURN_EXPR && TREE_OPERAND (stmt, 0))
    rhs = TREE_OPERAND (TREE_OPERAND (stmt, 0), 1);
  else
    rhs = TREE_OPERAND (stmt, 1);
 
  /* iterative_hash_expr knows how to deal with any expression and
     deals with commutative operators as well, so just use it instead
     of duplicating such complexities here.  */
  val = iterative_hash_expr (rhs, val);

  /* Add the SSA version numbers of every vuse operand.  This is important
     because compound variables like arrays are not renamed in the
     operands.  Rather, the rename is done on the virtual variable
     representing all the elements of the array.  */
  ops = vuse_ops (stmt_ann (stmt));
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    val = iterative_hash_expr (VARRAY_TREE (ops, i), val);

  return val;
}


static int
avail_expr_eq (const void *p1, const void *p2)
{
  tree s1, s2, rhs1, rhs2;

  s1 = (tree) p1;
  if (TREE_CODE (s1) == COND_EXPR)
    rhs1 = COND_EXPR_COND (s1);
  else if (TREE_CODE (s1) == SWITCH_EXPR)
    rhs1 = SWITCH_COND (s1);
  else if (TREE_CODE (s1) == RETURN_EXPR && TREE_OPERAND (s1, 0))
    rhs1 = TREE_OPERAND (TREE_OPERAND (s1, 0), 1);
  else
    rhs1 = TREE_OPERAND (s1, 1);

  s2 = (tree) p2;
  if (TREE_CODE (s2) == COND_EXPR)
    rhs2 = COND_EXPR_COND (s2);
  else if (TREE_CODE (s2) == SWITCH_EXPR)
    rhs2 = SWITCH_COND (s2);
  else if (TREE_CODE (s2) == RETURN_EXPR && TREE_OPERAND (s2, 0))
    rhs2 = TREE_OPERAND (TREE_OPERAND (s2, 0), 1);
  else
    rhs2 = TREE_OPERAND (s2, 1);

  /* If they are the same physical statement, return true.  */
  if (s1 == s2)
    return true;

  /* In case of a collision, both RHS have to be identical and have the
     same VUSE operands.  */
  if (TREE_CODE (rhs1) == TREE_CODE (rhs2)
      && (TREE_TYPE (rhs1) == TREE_TYPE (rhs2)
	  || (TYPE_MAIN_VARIANT (TREE_TYPE (rhs1))
	      == TYPE_MAIN_VARIANT (TREE_TYPE (rhs2))))
      && operand_equal_p (rhs1, rhs2, 0))
    {
      varray_type ops1 = vuse_ops (stmt_ann (s1));
      varray_type ops2 = vuse_ops (stmt_ann (s2));

      if (ops1 == NULL && ops2 == NULL)
	{
#ifdef ENABLE_CHECKING
	  if (avail_expr_hash (s1) != avail_expr_hash (s2))
	    abort ();
#endif
	  return true;
	}

      /* If one has virtual operands and the other does not, then we
	 consider them not equal.  */
      if ((ops1 == NULL && ops2 != NULL)
	  || (ops1 != NULL && ops2 == NULL))
	return false;

      if (VARRAY_ACTIVE_SIZE (ops1) == VARRAY_ACTIVE_SIZE (ops2))
	{
	  size_t i;
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (ops1); i++)
	    if (VARRAY_TREE_PTR (ops1, i) != VARRAY_TREE_PTR (ops2, i))
	      return false;

#ifdef ENABLE_CHECKING
	  if (avail_expr_hash (s1) != avail_expr_hash (s2))
	    abort ();
#endif
	  return true;
	}
    }

  return false;
}
