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

/* This file implements optimizations on the dominator tree.  */

/* Dump file and flags.  */
static FILE *dump_file;
static int dump_flags;

/* Structure to map variables to values.  It's used to keep track of the
   available constants and copies.  */
struct var_value_d
{
  tree var;
  tree value;
};

/* Hash table with expressions made available during the renaming process.
   When an assignment of the form X_i = EXPR is found, the statement is
   stored in this table.  If the same expression EXPR is later found on the
   RHS of another statement, it is replaced with X_i (thus performing
   global redundancy elimination). */
static htab_t avail_exprs;

/* Hash table of constant values and copies indexed by SSA name.  When the
   renaming pass finds an assignment of a constant (X_i = C) or a copy
   assignment from another SSA variable (X_i = Y_j), it creates a mapping
   between X_i and the RHS in this table.  This mapping is used later on,
   when renaming uses of X_i.  If an assignment to X_i is found in this
   table, instead of using X_i, we use the RHS of the statement stored in
   this table (thus performing very simplistic copy and constant
   propagation).  */
static htab_t const_and_copies;

/* Statistics for dominator optimizations.  */
struct opt_stats_d
{
  long num_stmts;
  long num_exprs_considered;
  long num_const_prop;
  long num_copy_prop;
  long num_re;
};

static struct opt_stats_d opt_stats;
static bool addr_expr_propagated_p;

/* Local functions.  */
static void optimize_block (basic_block, tree, int);
static void optimize_stmt (block_stmt_iterator, varray_type *);
static tree get_value_for (tree, htab_t);
static void set_value_for (tree, tree, htab_t);
static hashval_t var_value_hash (const void *);
static int var_value_eq (const void *, const void *);
static tree lookup_avail_expr (tree, varray_type *, htab_t);
static tree get_eq_expr_value (tree, int, varray_type *, htab_t);
static hashval_t avail_expr_hash (const void *);
static int avail_expr_eq (const void *, const void *);
static void htab_statistics (FILE *, htab_t);
static void record_cond_is_false (tree cond, varray_type *, htab_t);
static void record_cond_is_true (tree cond, varray_type *, htab_t);

/* Optimize function FNDECL based on the dominator tree.  This does
   simple const/copy propagation and redundant expression elimination using
   value numbering.

   The const/copy propagation may propagate ADDR_EXPRs to pointer use
   sites.  If that occurrs, then we have new variables to expose to the
   SSA renamer and optimizer.  So return nonzero if we propagate any
   ADDR_EXPRs to their pointer use sites.  */

bool
tree_ssa_dominator_optimize (tree fndecl)
{
  bool found_unreachable;
  bitmap unreachable_bitmap = BITMAP_XMALLOC ();

  timevar_push (TV_TREE_SSA_DOMINATOR_OPTS);

  /* Set up debugging dump files.  */
  dump_file = dump_begin (TDI_dom, &dump_flags);

  /* Create our hash tables.  */
  const_and_copies = htab_create (1024, var_value_hash, var_value_eq, free);
  avail_exprs = htab_create (1024, avail_expr_hash, avail_expr_eq, NULL);

  /* Indicate that we have not propagated any ADDR_EXPRs.  */
  addr_expr_propagated_p = false;

  /* If we prove certain blocks are unreachable, then we want to
     repeat the dominator optimization process as PHI nodes may
     have turned into copies which allows better propagation of
     values.  So we repeat until we do not identify any new unreachable
     blocks.  */
  do
    {
      int i;

      /* Optimize the dominator tree.  */
      optimize_block (ENTRY_BLOCK_PTR, NULL, 0);

      /* Wipe the hash tables.  */
      htab_empty (const_and_copies);
      htab_empty (avail_exprs);

      /* We may have made some basic blocks unreachable.  We do not
	 want to call tree_cleanup_cfg here as it renumbers the blocks
	 which makes dom_children invalid.

	 Instead we simplify identify the unreachable blocks and
	 remove their edges and PHI nodes.  After rewriting we will
	 complete removal of the unreachable blocks.  */
      find_unreachable_blocks ();

      found_unreachable = false;

      for (i = 0; i < n_basic_blocks; i++)
	{
	  basic_block bb = BASIC_BLOCK (i);

	  if (! (bb->flags & BB_REACHABLE))
	    {
	      /* If a previous iteration determined this block was
		 unreachable, then just ignore the block.  */
	      if (bitmap_bit_p (unreachable_bitmap, bb->index))
		continue;

	      found_unreachable = true;
	      bitmap_set_bit (unreachable_bitmap, bb->index);
	      remove_phi_nodes_and_edges_for_unreachable_block (bb);
	    }
	}
    }
  while (found_unreachable);

  htab_delete (const_and_copies);
  htab_delete (avail_exprs);
  BITMAP_XFREE (unreachable_bitmap);

  /* Debugging dumps.  */
  if (dump_file)
    {
      if (dump_flags & TDF_STATS)
	dump_dominator_optimization_stats (dump_file);
      dump_function_to_file (fndecl, dump_file, dump_flags);
      dump_end (TDI_dom, dump_file);
    }

  timevar_pop (TV_TREE_SSA_DOMINATOR_OPTS);

  return addr_expr_propagated_p;
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

   EQ_EXPR_VALUE is an assignment expression created when BB's immediate
   dominator ends in a COND_EXPR statement whose predicate is of the form
   'VAR == VALUE', where VALUE may be another variable or a constant. 
   This is used to propagate VALUE on the THEN_CLAUSE of that conditional.
   This assignment is inserted in CONST_AND_COPIES so that the copy and
   constant propagator can find more propagation opportunities.  */

static void
optimize_block (basic_block bb, tree parent_block_last_stmt, int edge_flags)
{
  varray_type block_avail_exprs;
  bitmap children;
  unsigned long i;
  block_stmt_iterator si;
  tree prev_value = NULL_TREE;
  tree eq_expr_value = NULL_TREE;
  edge e;
  tree phi;

  /* Initialize the local stacks.
     
     BLOCK_AVAIL_EXPRS stores all the expressions made available in this
     block.  Since expressions made available in this block are only valid
     in blocks dominated by BB, when we finish rewriting BB and its
     dominator children, we have to remove these expressions from the
     AVAIL_EXPRS table.  This stack is used to know which expressions
     to remove from the table.  */
  VARRAY_TREE_INIT (block_avail_exprs, 20, "block_avail_exprs");

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\n\nOptimizing block #%d\n\n", bb->index);

  if (parent_block_last_stmt
      && TREE_CODE (parent_block_last_stmt) == COND_EXPR
      && (edge_flags & (EDGE_TRUE_VALUE | EDGE_FALSE_VALUE)))
    eq_expr_value = get_eq_expr_value (parent_block_last_stmt,
				       (edge_flags & EDGE_TRUE_VALUE) != 0,
				       &block_avail_exprs,
				       const_and_copies);


  /* If EQ_EXPR_VALUE (VAR == VALUE) is given, register the VALUE as a
     new value for VAR, so that occurrences of VAR can be replaced with
     VALUE while re-writing the THEN arm of a COND_EXPR.  */
  if (eq_expr_value)
    {
      prev_value = get_value_for (TREE_OPERAND (eq_expr_value, 0),
				  const_and_copies);
      set_value_for (TREE_OPERAND (eq_expr_value, 0),
		     TREE_OPERAND (eq_expr_value, 1),
		     const_and_copies);
    }

  /* PHI nodes can create equivalences too.

     Ignoring any alternatives which are the same as the result, if
     all the alternatives are equal, then the PHI node creates an
     equivalence.  */
  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    {
      tree lhs = PHI_RESULT (phi);
      tree rhs = NULL;
      int i;

      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	{
	  tree t = PHI_ARG_DEF (phi, i);

	  if (TREE_CODE (t) == SSA_NAME || TREE_CONSTANT (t))
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
	set_value_for (lhs, rhs, const_and_copies);
    }

  /* Optimize each statement within the basic block.  */
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    optimize_stmt (si, &block_avail_exprs);

  /* Propagate known constants/copies into PHI nodes.  */
  for (e = bb->succ; e; e = e->succ_next)
    {
      for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
	{
	  int i;

	  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	    {
	      tree new;
	      if (PHI_ARG_EDGE (phi, i) == e)
		{
		  tree *orig_p = &PHI_ARG_DEF (phi, i);

		  if (! SSA_VAR_P (*orig_p))
		    break;

		  new = get_value_for (*orig_p, const_and_copies);
		  /* We want to allow copy propagation as well as constant
		     propagation.  */
		  if (new
		      && (TREE_CODE (new) == SSA_NAME || TREE_CONSTANT (new))
		      && may_propagate_copy (*orig_p, new))
		    *orig_p = new;
		  break;
		}
	    }
	}
    }

  /* Recursively optimize the dominator children of BB.  */
  children = dom_children (bb);
  if (children)
    {
      if (bb->flags & BB_CONTROL_EXPR)
	{
	  tree last = last_stmt (bb);
	  EXECUTE_IF_SET_IN_BITMAP (children, 0, i,
	    {
	      basic_block dest = BASIC_BLOCK (i);

	      /* The destination block may have become unreachable, in
		 which case there's no point in optimizing it.  */
	      if (dest->pred)
		optimize_block (dest, last, dest->pred->flags);
	    });
	}
      else
	{
	  EXECUTE_IF_SET_IN_BITMAP (children, 0, i,
	    {
	      basic_block dest = BASIC_BLOCK (i);

	      /* The destination block may have become unreachable, in
		 which case there's no point in optimizing it. */
	      if (dest->pred)
		optimize_block (dest, NULL_TREE, 0);
	    });
	}
    }

  /* Remove all the expressions made available in this block.  */
  while (VARRAY_ACTIVE_SIZE (block_avail_exprs) > 0)
    {
      tree stmt = VARRAY_TOP_TREE (block_avail_exprs);
      VARRAY_POP (block_avail_exprs);
      htab_remove_elt (avail_exprs, stmt);
    }

  /* Also remove equivalences created by EQ_EXPR_VALUE.  */
  if (eq_expr_value)
    {
      struct var_value_d vm;
      vm.var = TREE_OPERAND (eq_expr_value, 0);
      if (prev_value)
        set_value_for (vm.var, prev_value, const_and_copies);
      else
        htab_remove_elt (const_and_copies, &vm);
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

  fprintf (file, "    const_and_copies: ");
  htab_statistics (file, const_and_copies);

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
record_cond_is_true (tree cond,
		     varray_type *block_avail_exprs_p,
		     htab_t const_and_copies)
{
  tree stmt;

  stmt = build (MODIFY_EXPR, boolean_type_node, integer_one_node, cond);
  lookup_avail_expr (stmt, block_avail_exprs_p, const_and_copies);
}

/* Enter a statement into the available expression hash table indicating
   that the condition COND is false.  */

static void
record_cond_is_false (tree cond,
		      varray_type *block_avail_exprs_p,
		      htab_t const_and_copies)
{
  tree stmt;

  stmt = build (MODIFY_EXPR, boolean_type_node, integer_zero_node, cond);
  lookup_avail_expr (stmt, block_avail_exprs_p, const_and_copies);
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

static void
optimize_stmt (block_stmt_iterator si, varray_type *block_avail_exprs_p)
{
  size_t i;
  stmt_ann_t ann;
  tree stmt;
  varray_type defs, uses, vuses, vdefs, operand_tables[4], *table;
  bool may_optimize_p;

  stmt = bsi_stmt (si);
  if (IS_EMPTY_STMT (stmt))
    return;

  get_stmt_operands (stmt);
  ann = stmt_ann (stmt);
  opt_stats.num_stmts++;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Optimizing statement ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  defs = def_ops (stmt);
  uses = use_ops (stmt);
  vuses = vuse_ops (stmt);
  vdefs = vdef_ops (stmt);

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
	  op_p = (tree *) VARRAY_GENERIC_PTR (uses, i);
	else if (*table == vuses)
	  op_p = (tree *) &VARRAY_TREE (vuses, i);
	else
	  op_p = (tree *) &VDEF_OP (VARRAY_TREE (vdefs, i));

	/* If the operand is not an ssa variable, then there is nothing
	   to do.  */
	if (! SSA_VAR_P (*op_p))
	  continue;

	/* If the operand has a known constant value or it is known to be a
	   copy of some other variable, use the value or copy stored in
	   CONST_AND_COPIES.  */
	opt_stats.num_exprs_considered++;
	val = get_value_for (*op_p, const_and_copies);
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
	    if (is_unchanging_value (val) || is_optimizable_addr_expr (val))
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
		    && is_unchanging_value (val)))
	      addr_expr_propagated_p = true;

	    if (TREE_CODE (val) == SSA_NAME)
	      propagate_copy (op_p, val);
	    else
	      *op_p = val;

	    /* If we only update virtual operands, then we should not
	       consider this statement as modified.  */
	    if (*table != vuses && *table != vdefs)
	      ann->modified = 1;
	}
    }

  /* If the statement has been modified with constant replacements,
     fold its RHS before checking for redundant computations.  */
  if (ann->modified)
    fold_stmt (bsi_stmt_ptr (si));

  /* Check for redundant computations.  Do this optimization only
     for assignments that make no calls and have no aliased stores
     nor volatile references and no side effects (i.e., no VDEFs).  */
  may_optimize_p = (!ann->makes_aliased_stores
		    && !ann->has_volatile_ops
		    && vdefs == NULL
		    && ((TREE_CODE (stmt) == MODIFY_EXPR
			 && ! TREE_SIDE_EFFECTS (TREE_OPERAND (stmt, 1)))
			|| TREE_CODE (stmt) == COND_EXPR));

  if (may_optimize_p)
    {
      tree *expr_p, def = NULL_TREE;

      /* Check if the RHS of the assignment has been computed before.  If
	 so, use the LHS of the previously computed statement as the
	 reaching definition for the variable defined by this statement.  */
      tree cached_lhs = lookup_avail_expr (stmt,
					   block_avail_exprs_p,
					   const_and_copies);

      if (TREE_CODE (stmt) == MODIFY_EXPR)
	def = TREE_OPERAND (stmt, 0);

      opt_stats.num_exprs_considered++;
      /* It is safe to ignore types here since we have already done
	 type checking in the hashing and equality routines.  In fact
	 type checking here merely gets in the way of constant
	 propagation.  */
      if (cached_lhs)
	{
	  if (TREE_CODE (stmt) == COND_EXPR)
	    expr_p = &TREE_OPERAND (stmt, 0);
	  else
	    expr_p = &TREE_OPERAND (stmt, 1);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "  Replaced redundant expr '");
	      print_generic_expr (dump_file, *expr_p, 0);
	      fprintf (dump_file, "' with '");
	      print_generic_expr (dump_file, cached_lhs, 0);
	      fprintf (dump_file, "'\n");
	    }

	  opt_stats.num_re++;
	  *expr_p = cached_lhs;
	  ann->modified = 1;
	}
    }

  /* If the RHS of an assignment is a constant or another variable that
     may be propagated, register it in the CONST_AND_COPIES table.  */
  if (TREE_CODE (stmt) == MODIFY_EXPR
      && TREE_CODE (TREE_OPERAND (stmt, 0)) == SSA_NAME)
    {
      tree rhs;

      rhs = TREE_OPERAND (stmt, 1);

      /* Some NOP_EXPRs are totally uninteresting and get in the way
	 to optimizing as they prevent equivalences from being recognized.

	 If we have an assignment that merely uses a NOP_EXPR to change
	 the top of the RHS to the type of the LHS and the type conversion
	 is "safe", then strip away the type conversion so that we can
	 enter LHS = RHS into the const_and_copies table.  */
      if (TREE_CODE (rhs) == NOP_EXPR)
	{
	  tree outer_type = TREE_TYPE (rhs);
	  tree inner_type = TREE_TYPE (TREE_OPERAND (rhs, 0));

	  /* If the inner and outer types are effectively the same, then
	     strip the type conversion and enter the equivalence into
	     the table.  */
	  if (TYPE_MAIN_VARIANT (inner_type) == TYPE_MAIN_VARIANT (outer_type)
	      || outer_type == inner_type)
	    STRIP_MAIN_TYPE_NOPS (rhs);
	  /* If the outer type is a (void *), then we can enter the
	     equivalence into the table.  The opposite is not true since
	     that conversion would result in a loss of information if
	     the equivalence was used.  Consider an indirect function call
	     where we need to know the exact type of the function to
	     correctly implement the ABI.  */
	  else if (POINTER_TYPE_P (outer_type) && POINTER_TYPE_P (inner_type))
	    {
	      if (TREE_CODE (TREE_TYPE (outer_type)) == VOID_TYPE)
		rhs = TREE_OPERAND (rhs, 0);
	    }
	  /* If both the inner and outer types are integral types, then
	     we can enter the equivalence if they have the same mode
	     and signedness.  */
	  else if (INTEGRAL_TYPE_P (outer_type) && INTEGRAL_TYPE_P (inner_type))
	    STRIP_SIGN_NOPS (rhs);
	}

      if (may_optimize_p)
	{
	  if (TREE_CODE (rhs) == SSA_NAME
	      || is_unchanging_value (rhs)
	      || is_optimizable_addr_expr (rhs))
	    set_value_for (TREE_OPERAND (stmt, 0), rhs, const_and_copies);
	}
    }

  /* Now a few special cases.  Odds are this code will be factored out
     into several subroutines in the near future.  I'm waiting to see
     what other cases arise before factoring the code out.  */
  if (TREE_CODE (stmt) == MODIFY_EXPR)
    {
      int i;

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
		  cond = build (EQ_EXPR, boolean_type_node,
				op, null_pointer_node);
		  record_cond_is_false (cond,
					block_avail_exprs_p,
					const_and_copies);

		  cond = build (NE_EXPR, boolean_type_node,
				op, null_pointer_node);
		  record_cond_is_true (cond,
				       block_avail_exprs_p,
				       const_and_copies);

		  /* A memory store, even an aliased store, creates a
		     useful equivalence.  By exchanging the LHS and RHS,
		     creating suitable vops and recording the result in
		     the available expression table, we may be able to
		     expose more redundant loads.  */
		  if (!ann->has_volatile_ops
		      && i == 0
		      && (SSA_VAR_P (TREE_OPERAND (stmt, 1))
			  || TREE_CONSTANT (TREE_OPERAND (stmt, 1))))
		    {
		      tree new;
		      unsigned int j;

		      /* Build a new statement with the RHS and LHS
			 exchanged.  */
		      new = build (MODIFY_EXPR, TREE_TYPE (stmt),
				   TREE_OPERAND (stmt, 1),
				   TREE_OPERAND (stmt, 0));

		      /* Get an annotation and set up the real operands.  */
		      get_stmt_ann (new);
		      get_stmt_operands (new);

		      /* Clear out the virtual operands on the new statement,
			 we are going to set them explicitly below.  */
		      get_stmt_ann (new)->vops = NULL;

		      /* For each VDEF on the original statement, we want
			 to create a VUSE of the VDEF result on the new
			 statement.  */
		      for (j = 0; vdefs && j < VARRAY_ACTIVE_SIZE (vdefs); j++)
			{
			  tree op;

			  op = (tree) VDEF_RESULT (VARRAY_TREE (vdefs, i));
			  add_vuse (op, new, NULL);
			}

		      /* Finally enter the statement into the available
			 expression table.  */
		      lookup_avail_expr (new,
					 block_avail_exprs_p,
					 const_and_copies);
		    }
		}
	    }
	}

      /* IOR of any value with a nonzero value will result in a nonzero
         value.  Even if we do not know the exact result recording that
	 the result is nonzero is worth the effort.  */
      if (TREE_CODE (TREE_OPERAND (stmt, 0)) == SSA_NAME
	  && TREE_CODE (TREE_OPERAND (stmt, 1)) == BIT_IOR_EXPR
	  && integer_nonzerop (TREE_OPERAND (TREE_OPERAND (stmt, 1), 1)))
	{
	  tree cond;
	  tree op = TREE_OPERAND (stmt, 0);

	  cond = build (EQ_EXPR, boolean_type_node, op, null_pointer_node);
	  record_cond_is_false (cond, block_avail_exprs_p, const_and_copies);

	  cond = build (NE_EXPR, boolean_type_node, op, null_pointer_node);
	  record_cond_is_true (cond, block_avail_exprs_p, const_and_copies);
			  	
	}

      /* Transform TRUNC_DIV_EXPR and TRUNC_MOD_EXPR into RSHIFT_EXPR and
         BIT_AND_EXPR respectively if the first operand is greater than
	 zero and the second operand is an exact power of two.  */
      if ((TREE_CODE (TREE_OPERAND (stmt, 1)) == TRUNC_DIV_EXPR
	   || TREE_CODE (TREE_OPERAND (stmt, 1)) == TRUNC_MOD_EXPR)
	  && INTEGRAL_TYPE_P (TREE_TYPE (TREE_OPERAND (TREE_OPERAND (stmt, 1), 0)))
	  && integer_pow2p (TREE_OPERAND (TREE_OPERAND (stmt, 1), 1)))
        {
	  tree cond, val;
	  tree op = TREE_OPERAND (TREE_OPERAND (stmt, 1), 0);

	  cond = build (GT_EXPR, boolean_type_node, op, integer_zero_node);
	  cond = build (COND_EXPR, void_type_node, cond, NULL, NULL);
	  val = lookup_avail_expr (cond, block_avail_exprs_p, const_and_copies);

	  /* Also try with GE_EXPR if we did not get a hit with GT_EXPR.  */
	  if (! val || ! integer_onep (val))
	    {
	      cond = build (GE_EXPR, boolean_type_node, op, integer_zero_node);
	      cond = build (COND_EXPR, void_type_node, cond, NULL, NULL);
	      val = lookup_avail_expr (cond,
				       block_avail_exprs_p,
				       const_and_copies);
	    }
	    
	  if (val && integer_onep (val))
	    {
	      tree t;
	      tree op0 = TREE_OPERAND (TREE_OPERAND (stmt, 1), 0);
	      tree op1 = TREE_OPERAND (TREE_OPERAND (stmt, 1), 1);

	      if (TREE_CODE (TREE_OPERAND (stmt, 1)) == TRUNC_DIV_EXPR)
		t = build (RSHIFT_EXPR, TREE_TYPE (op0), op0,
			   build_int_2 (tree_log2 (op1), 0));
	      else
		t = build (BIT_AND_EXPR, TREE_TYPE (op0), op0,
			   fold (build (MINUS_EXPR, TREE_TYPE (op1),
					op1, integer_one_node)));

	      /* Remove the old entry from the hash table.  */
	      if (may_optimize_p)
		htab_remove_elt (avail_exprs, stmt);

	      /* Now update the RHS of the assignment to use the new node.  */
	      TREE_OPERAND (stmt, 1) = t;

	      if (may_optimize_p)
		{
		  /* Now force the updated statement into the hash table.  */
		  lookup_avail_expr (stmt,
				     block_avail_exprs_p,
				     const_and_copies);

		  /* Annoyingly we now have two entries for this statement in
		     BLOCK_AVAIL_EXPRs.  Luckily we can just pop off the newest
		     entry.  */
		  VARRAY_POP (*block_avail_exprs_p);
		}

	      /* And make sure we record the fact that we modified this
	         statement.  */
	      ann->modified = 1;
	    }
        }

      if (TREE_CODE (TREE_OPERAND (stmt, 1)) == ABS_EXPR
	  && INTEGRAL_TYPE_P (TREE_TYPE (TREE_OPERAND (TREE_OPERAND (stmt, 1), 0))))
        {
	  tree cond, val;
	  tree op = TREE_OPERAND (TREE_OPERAND (stmt, 1), 0);
	  tree type = TREE_TYPE (op);

	  cond = build (LT_EXPR, boolean_type_node, op,
			convert (type, integer_zero_node));
	  cond = build (COND_EXPR, void_type_node, cond, NULL, NULL);
	  val = lookup_avail_expr (cond, block_avail_exprs_p, const_and_copies);

	  if (! val
	      || (! integer_onep (val) && ! integer_zerop (val)))
	    {
	      cond = build (LE_EXPR, boolean_type_node, op,
			    convert (type, integer_zero_node));
	      cond = build (COND_EXPR, void_type_node, cond, NULL, NULL);
	      val = lookup_avail_expr (cond,
				       block_avail_exprs_p,
				       const_and_copies);
	    }
	    
	  if (val
	      && (integer_onep (val) || integer_zerop (val)))
	    {
	      tree t;

	      if (integer_onep (val))
		t = build1 (NEGATE_EXPR, TREE_TYPE (op), op);
	      else
		t = op;

	      /* Remove the old entry from the hash table.  */
	      if (may_optimize_p)
		htab_remove_elt (avail_exprs, stmt);

	      /* Now update the RHS of the assignment to use the new node.  */
	      TREE_OPERAND (stmt, 1) = t;

	      if (may_optimize_p)
		{
		  /* Now force the updated statement into the hash table.  */
		  lookup_avail_expr (stmt,
				     block_avail_exprs_p,
				     const_and_copies);

		  /* Annoyingly we now have two entries for this statement in
		     BLOCK_AVAIL_EXPRs.  Luckily we can just pop off the newest
		     entry.  */
		  VARRAY_POP (*block_avail_exprs_p);
		}

	      /* And make sure we record the fact that we modified this
	         statement.  */
	      ann->modified = 1;
	    }
        }
    }

  /* If STMT is a COND_EXPR and it was modified, then we may know
     where it goes.  In which case we can remove some edges, simplify
     some PHI nodes, maybe even avoid optimizing some blocks completely,
     etc.  */
  if (TREE_CODE (stmt) == COND_EXPR && ann->modified)
    {
      basic_block bb = bb_for_stmt (stmt);
      edge taken_edge = find_taken_edge (bb, TREE_OPERAND (stmt, 0));

      if (taken_edge)
	{
	  edge e, next;
	  /* The other edges leaving this block are not executable
	     and can be removed.  */
	  for (e = bb_for_stmt (stmt)->succ; e; e = next)
	    {
	      next = e->succ_next;
	      if (e != taken_edge)
		ssa_remove_edge (e);
	   }
	}
    }

}

/* Hashing and equality functions for VAR_VALUE_D.  */

static hashval_t
var_value_hash (const void *p)
{
  return htab_hash_pointer
	((const void *)((const struct var_value_d *)p)->var);
}

static int
var_value_eq (const void *p1, const void *p2)
{
  return ((const struct var_value_d *)p1)->var
	 == ((const struct var_value_d *)p2)->var;
}

/* Return the value associated with variable VAR in TABLE.  */

static tree
get_value_for (tree var, htab_t table)
{
  struct var_value_d *vm_p, vm;

#if defined ENABLE_CHECKING
  if (!SSA_VAR_P (var))
    abort ();
#endif

  vm.var = var;
  vm_p = (struct var_value_d *) htab_find (table, (void *) &vm);
  return (vm_p) ? vm_p->value : NULL_TREE;
}


/* Associate VALUE to variable VAR in TABLE.  */

static void
set_value_for (tree var, tree value, htab_t table)
{
  struct var_value_d *vm_p, vm;
  void **slot;

#if defined ENABLE_CHECKING
  if (!SSA_VAR_P (var))
    abort ();
#endif

  vm.var = var;
  slot = htab_find_slot (table, (void *) &vm, INSERT);
  if (*slot == NULL)
    {
      vm_p = xmalloc (sizeof *vm_p);
      vm_p->var = var;
      *slot = (void *) vm_p;
    }
  else
    vm_p = (struct var_value_d *) *slot;

  vm_p->value = value;
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
lookup_avail_expr (tree stmt,
		   varray_type *block_avail_exprs_p,
		   htab_t const_and_copies)
{
  void **slot;
  tree rhs;
  tree lhs;
  tree temp;
  int insert;

  /* For a COND_EXPR, the expression we care about is in operand 0, not
     operand 1.  Also note that for a COND_EXPR, we merely want to see if
     we have the expression in the hash table, we do not want to create
     a new entry in the hash table.  */
  if (TREE_CODE (stmt) == COND_EXPR)
    {
      rhs = TREE_OPERAND (stmt, 0);
      insert = 0;
    }
  else
    {
      rhs = TREE_OPERAND (stmt, 1);
      insert = 1;
    }

  /* Don't bother remembering constant assignments and copy operations.
     Constants and copy operations are handled by the constant/copy propagator
     in optimize_stmt.  */
  if (TREE_CODE (rhs) == SSA_NAME
      || is_unchanging_value (rhs)
      || is_optimizable_addr_expr (rhs))
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

  /* See if the LHS appears in the const_and_copies table.  If it does, then
     use the value from the const_and_copies table.  */
  if (SSA_VAR_P (lhs))
    {
      temp = get_value_for (lhs, const_and_copies);
      if (temp)
	lhs = temp;
    }

  return lhs;
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
get_eq_expr_value (tree if_stmt, int true_arm,
		   varray_type *block_avail_exprs_p, htab_t const_and_copies)
{
  tree cond, value;

  cond = COND_EXPR_COND (if_stmt);

  /* If we have a comparison expression, then record its result into
     the available expression table.  */
  if (TREE_CODE_CLASS (TREE_CODE (cond)) == '<')
    {
      /* When we find an available expression in the hash table, we replace
	 the expression with the LHS of the statement in the hash table.

	 So, we want to build statements such as "1 = <condition>" on the
	 true arm and "0 = <condition>" on the false arm.  That way if we
	 find the expression in the table, we will replace it with its
	 known constant value.  Also insert inversions of the result and
	 condition into the hash table.  */
      if (true_arm)
	{
	  record_cond_is_true (cond, block_avail_exprs_p, const_and_copies);
	  record_cond_is_false (invert_truthvalue (cond),
				block_avail_exprs_p,
				const_and_copies);
	}
      else
	{
	  record_cond_is_true (invert_truthvalue (cond),
			       block_avail_exprs_p,
			       const_and_copies);
	  record_cond_is_false (cond, block_avail_exprs_p, const_and_copies);
	}
    }

  /* If the conditional is a single variable 'X', return 'X = 1' for
     the true arm and 'X = 0' on the false arm.   */
  if (SSA_VAR_P (cond))
    return build (MODIFY_EXPR, TREE_TYPE (cond), cond,
		  (true_arm ? integer_one_node : integer_zero_node));

  /* If the conditional is of the form 'X == Y', return 'X = Y' for the
     true arm.  */
  else if (true_arm
	   && TREE_CODE (cond) == EQ_EXPR
	   && TREE_CODE (TREE_OPERAND (cond, 0)) == SSA_NAME
	   && (is_unchanging_value (TREE_OPERAND (cond, 1))
	       || is_optimizable_addr_expr (TREE_OPERAND (cond, 1))
	       || TREE_CODE (TREE_OPERAND (cond, 1)) == SSA_NAME))
    value = build (MODIFY_EXPR, TREE_TYPE (cond),
		   TREE_OPERAND (cond, 0),
		   TREE_OPERAND (cond, 1));

  /* If the conditional is of the form 'X != Y', return 'X = Y' for the
     false arm.  */
  else if (! true_arm
	   && TREE_CODE (cond) == NE_EXPR
	   && TREE_CODE (TREE_OPERAND (cond, 0)) == SSA_NAME
	   && (is_unchanging_value (TREE_OPERAND (cond, 1))
	       || is_optimizable_addr_expr (TREE_OPERAND (cond, 1))
	       || TREE_CODE (TREE_OPERAND (cond, 1)) == SSA_NAME))
    value = build (MODIFY_EXPR, TREE_TYPE (cond),
		   TREE_OPERAND (cond, 0),
		   TREE_OPERAND (cond, 1));

  /* Return nothing for any other conditional.  */
  else
    value = NULL_TREE;

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

  /* If we're hashing a COND_EXPR, the expression we care about is
     in operand position 0, not position 1.  */
  if (TREE_CODE (stmt) == COND_EXPR)
    rhs = TREE_OPERAND (stmt, 0);
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
  ops = vuse_ops (stmt);
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
    rhs1 = TREE_OPERAND (s1, 0);
  else
    rhs1 = TREE_OPERAND (s1, 1);

  s2 = (tree) p2;
  if (TREE_CODE (s2) == COND_EXPR)
    rhs2 = TREE_OPERAND (s2, 0);
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
      varray_type ops1 = vuse_ops (s1);
      varray_type ops2 = vuse_ops (s2);

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
	    if (VARRAY_GENERIC_PTR (ops1, i) != VARRAY_GENERIC_PTR (ops2, i))
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
