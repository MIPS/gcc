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
static void optimize_block (basic_block, tree);
static void optimize_stmt (block_stmt_iterator, varray_type *);
static tree get_value_for (tree, htab_t);
static void set_value_for (tree, tree, htab_t);
static hashval_t var_value_hash (const void *);
static int var_value_eq (const void *, const void *);
static tree lookup_avail_expr (tree, varray_type *, htab_t);
static tree get_eq_expr_value (tree);
static hashval_t avail_expr_hash (const void *);
static int avail_expr_eq (const void *, const void *);
static void htab_statistics (FILE *, htab_t);

/* Optimize the current function based on the dominator tree.  This does
   simple const/copy propagation and redundant expression elimination using
   value numbering.

   The const/copy propagation may propagate ADDR_EXPRs to pointer use
   sites.  If that occurrs, then we have new variables to expose to the
   SSA renamer and optimizer.  So return nonzero if we propagate any
   ADDR_EXPRs to their pointer use sites.  */

bool
tree_ssa_dominator_optimize (FILE *file, int flags)
{
  /* Set up debugging dump files.  */
  dump_file = file;
  dump_flags = flags;

  /* Create our hash tables.  */
  const_and_copies = htab_create (1024, var_value_hash, var_value_eq, free);
  avail_exprs = htab_create (1024, avail_expr_hash, avail_expr_eq, NULL);

  /* Indicate that we have not propagated any ADDR_EXPRs.  */
  addr_expr_propagated_p = false;

  /* Now optimize the dominator tree.  */
  optimize_block (ENTRY_BLOCK_PTR, NULL);

  htab_delete (const_and_copies);
  htab_delete (avail_exprs);
  const_and_copies = NULL;
  avail_exprs = NULL;

  /* Debugging dumps.  */
  if (dump_file)
    {
      if (dump_flags & TDF_STATS)
	dump_dominator_optimization_stats (dump_file);
    }
                                                                                

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
optimize_block (basic_block bb, tree eq_expr_value)
{
  varray_type block_avail_exprs;
  bitmap children;
  unsigned long i;
  block_stmt_iterator si;
  tree prev_value = NULL_TREE;

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

  /* Optimize each statement within the basic block.  */
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    optimize_stmt (si, &block_avail_exprs);

  /* Recursively optimize the dominator children of BB.  */
  children = dom_children (bb);
  if (children)
    {
      if (bb->flags & BB_CONTROL_EXPR)
	{
	  tree last = last_stmt (bb);
	  EXECUTE_IF_SET_IN_BITMAP (children, 0, i,
	    {
	      if (BASIC_BLOCK (i)->pred->flags & EDGE_TRUE_VALUE)
		optimize_block (BASIC_BLOCK (i), get_eq_expr_value (last));
	      else
		optimize_block (BASIC_BLOCK (i), NULL_TREE);
	    });
	}
      else
	{
	  EXECUTE_IF_SET_IN_BITMAP (children, 0, i,
	    optimize_block (BASIC_BLOCK (i), NULL_TREE));
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
  tree stmt, *def_p;
  varray_type uses, vuses, vdefs;
  bool may_optimize_p;

  stmt = bsi_stmt (si);
  if (IS_EMPTY_STMT (stmt))
    return;

  ann = stmt_ann (stmt);
  opt_stats.num_stmts++;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Optimizing statement ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  def_p = def_op (stmt);
  uses = use_ops (stmt);
  vuses = vuse_ops (stmt);
  vdefs = vdef_ops (stmt);

#if defined ENABLE_CHECKING
  /* Only assignments may make a new definition.  */
  if (def_p && TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();
#endif

  /* Const/copy propagate into USES.  */
  for (i = 0; uses && i < VARRAY_ACTIVE_SIZE (uses); i++)
    {
      tree val;
      tree *op_p = (tree *) VARRAY_GENERIC_PTR (uses, i);

      /* If the operand has a known constant value or it is known to be a
	 copy of some other variable, use the value or copy stored in
	 CONST_AND_COPIES.  */
      opt_stats.num_exprs_considered++;
      val = get_value_for (*op_p, const_and_copies);
      if (val)
	{
	  /* Gather statistics.  */
	  if (is_unchanging_value (val)
	      || is_optimizable_addr_expr (val))
	    opt_stats.num_const_prop++;
	  else
	    opt_stats.num_copy_prop++;

	  /* Replace the operand with its known constant value or copy.  */
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "  Replaced '");
	      print_generic_expr (dump_file, *op_p, 0);
	      fprintf (dump_file, "' with %s '",
		       TREE_CODE (val) == SSA_NAME ? "constant" : "variable");
	      print_generic_expr (dump_file, val, 0);
	      fprintf (dump_file, "'\n");
	    }

	  /* If VAL is an ADDR_EXPR, notify that we may need to have a
	     second SSA pass to rename variables exposed by the folding of
	     *&VAR expressions.  */
	  if (TREE_CODE (val) == ADDR_EXPR)
	    addr_expr_propagated_p = true;

	  if (TREE_CODE (val) == SSA_NAME)
	    propagate_copy (op_p, val);
	  else
	    *op_p = val;

	  ann->modified = 1;
	}
    }

  /* If the statement has been modified with constant replacements,
     fold its RHS before checking for redundant computations.  */
  if (ann->modified)
    fold_stmt (stmt);

  /* Check for redundant computations.  Do this optimization only
     for assignments that make no calls and have no aliased stores
     nor volatile references and no side effects (i.e., no VDEFs).  */
  may_optimize_p = !ann->makes_aliased_stores
		   && !ann->has_volatile_ops
		   && vdefs == NULL
		   && def_p
		   && ! TREE_SIDE_EFFECTS (TREE_OPERAND (stmt, 1));

  if (may_optimize_p)
    {
      /* Check if the RHS of the assignment has been computed before.  If
	 so, use the LHS of the previously computed statement as the
	 reaching definition for the variable defined by this statement.  */
      tree cached_lhs = lookup_avail_expr (stmt,
					   block_avail_exprs_p,
					   const_and_copies);
      opt_stats.num_exprs_considered++;
      if (cached_lhs
	  && (TREE_TYPE (cached_lhs) == TREE_TYPE (*def_p)
	      || (TYPE_MAIN_VARIANT (TREE_TYPE (cached_lhs))
		  == TYPE_MAIN_VARIANT (TREE_TYPE (*def_p)))))
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "  Replaced redundant expr '");
	      print_generic_expr (dump_file, TREE_OPERAND (stmt, 1), 0);
	      fprintf (dump_file, "' with '");
	      print_generic_expr (dump_file, cached_lhs, 0);
	      fprintf (dump_file, "'\n");
	    }

	  opt_stats.num_re++;
	  TREE_OPERAND (stmt, 1) = cached_lhs;
	  ann->modified = 1;
	}
    }

  /* If the RHS of an assignment is a constant or another variable that
     may be propagated, register it in the CONST_AND_COPIES table.  */
  if (def_p)
    {
      tree rhs;

      rhs = TREE_OPERAND (stmt, 1);
      if (may_optimize_p)
	{
	  if (TREE_CODE (rhs) == SSA_NAME
	      || is_unchanging_value (rhs)
	      || is_optimizable_addr_expr (rhs))
	    set_value_for (*def_p, rhs, const_and_copies);
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

  /* Don't bother remembering constant assignments and copy operations.
     Constants and copy operations are handled by the constant/copy propagator
     in optimize_stmt.  */
  rhs = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (rhs) == SSA_NAME
      || is_unchanging_value (rhs)
      || is_optimizable_addr_expr (rhs))
    return NULL_TREE;

  slot = htab_find_slot (avail_exprs, stmt, INSERT);
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
  temp = get_value_for (lhs, const_and_copies);
  if (temp)
    lhs = temp;

  return lhs;
}


/* Given a conditional statement IF_STMT, return the assignment 'X = Y', if
   the conditional is of the form 'X == Y'.  If the conditional is of the
   form 'X'.  The assignment 'X = 1' is returned.  */

static tree
get_eq_expr_value (tree if_stmt)
{
  tree cond, value;

  cond = COND_EXPR_COND (if_stmt);

  /* If the conditional is a single variable 'X', return 'X = 1'.  */
  if (SSA_VAR_P (cond))
    return build (MODIFY_EXPR, TREE_TYPE (cond), cond, integer_one_node);

  /* If the conditional is of the form 'X == Y', return 'X = Y'.  */
  else if (TREE_CODE (cond) == EQ_EXPR
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

  /* iterative_hash_expr knows how to deal with any expression and
     deals with commutative operators as well, so just use it instead
     of duplicating such complexities here.  */
  rhs = TREE_OPERAND (stmt, 1);
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
  rhs1 = TREE_OPERAND (s1, 1);

  s2 = (tree) p2;
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
