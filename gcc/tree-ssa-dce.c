/* Dead code elimination pass for the GNU compiler.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Ben Elliston <bje@redhat.com> and Andrew MacLeod 
   <amacleod@redhat.com>
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* Dead code elimination.

   References:

     Building an Optimizing Compiler,
     Robert Morgan, Butterworth-Heinemann, 1998, Section 8.9.

     Advanced Compiler Design and Implementation,
     Steven Muchnick, Morgan Kaufmann, 1997, Section 18.10.

   Dead-code elimination is the removal of instructions which have no
   impact on the program's output.  "Dead instructions" have no impact
   on the program's output, while "necessary instructions" may have
   impact on the output.

   The algorithm consists of three phases:
   1. Marking as necessary all instructions known to be necessary,
      e.g., function calls, writing a value to memory, etc;
   2. Propagating necessary instructions, e.g., the instructions
      giving values to operands in necessary instructions; and
   3. Removing dead instructions (except replacing dead conditionals
      with unconditional jumps).  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"

/* These RTL headers are needed for basic-block.h.  */
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"

#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-simple.h"
#include "tree-dump.h"
#include "timevar.h"


/* Debugging dumps.  */
static FILE *dump_file;
static int dump_flags;

static varray_type worklist;

static struct stmt_stats
{
  int total;
  int total_phis;
  int removed;
  int removed_phis;
} stats;

/* Forward function prototypes.  */
static inline bool necessary_p (tree);
static inline void clear_necessary (tree);
static inline void mark_necessary (tree, tree);
static void print_stats (void);
static bool need_to_preserve_store (tree);
static void find_useful_stmts (void);
static bool stmt_useful_p (tree);
static void process_worklist (void);
static void remove_dead_stmts (void);
static void remove_dead_stmt (block_stmt_iterator *);
static void remove_dead_phis (basic_block);

#define NECESSARY(stmt)	   stmt->common.asm_written_flag

/* vector indicating an SSA name has already been processed and marked 
   as necessary.  */
static sbitmap processed;

/* Is a tree necessary?  */

static inline bool
necessary_p (tree t)
{
  return NECESSARY (t);
}

static inline void
clear_necessary (tree t)
{
  NECESSARY (t) = 0;
}

/* Mark a tree as necessary.  */

static inline void
mark_necessary (tree def, tree stmt)
{
  int ver;
#ifdef ENABLE_CHECKING
  if ((def == NULL && stmt == NULL) || stmt == error_mark_node 
      || (stmt && DECL_P (stmt)))
    abort ();
#endif 

  if (def)
    {
      ver = SSA_NAME_VERSION (def);
      if (TEST_BIT (processed, ver))
	return;
      SET_BIT (processed, ver);
      if (!stmt)
	stmt = SSA_NAME_DEF_STMT (def);
    }
  
  if (necessary_p (stmt))
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Marking useful stmt: ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  NECESSARY (stmt) = 1;
  VARRAY_PUSH_TREE (worklist, stmt);
}



/* Print out removed statement statistics.  */

static void
print_stats (void)
{
  if (dump_file && (dump_flags & (TDF_STATS|TDF_DETAILS)))
    {
      float percg;

      percg = ((float) stats.removed / (float) stats.total) * 100;
      fprintf (dump_file, "Removed %d of %d statements (%d%%)\n",
			  stats.removed, stats.total, (int) percg);

      if (stats.total_phis == 0)
	percg = 0;
      else
	percg = ((float) stats.removed_phis / (float) stats.total_phis) * 100;

      fprintf (dump_file, "Removed %d of %d PHI nodes (%d%%)\n",
			  stats.removed_phis, stats.total_phis, (int) percg);
    }
}


/* Return true if a store to a variable needs to be preserved.  */

static bool
need_to_preserve_store (tree var)
{
  tree base_symbol;
  tree sym;

  if (var == NULL)
    return false;

  sym = SSA_NAME_VAR (var);
  base_symbol = get_base_symbol (var);

  /* Store to global variables must be preserved.  */
  if (decl_function_context (base_symbol) != current_function_decl)
    return true;
  
  /* Static locals must be preserved as well.  */
  if (TREE_STATIC (base_symbol))
    return true;

  /* If SYM may alias global memory, we also need to preserve the store.  */
  if (may_alias_global_mem_p (sym))
    return true;

  return false;
}


/* Find obviously useful instructions.  These are things like function
   calls and stores to file level variables.  */

static void
find_useful_stmts (void)
{
  basic_block bb;
  block_stmt_iterator i;

  FOR_EACH_BB (bb)
    {
      tree phi;

      /* Check any PHI nodes in the block.  */
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
        {
	  clear_necessary (phi);
	  if (need_to_preserve_store (PHI_RESULT (phi)))
	    mark_necessary (PHI_RESULT (phi), phi);
	}

      /* Check all statements in the block.  */
      for (i = bsi_start (bb); !bsi_end_p (i); bsi_next (&i))
	{
	  tree stmt = bsi_stmt (i);

	  clear_necessary (stmt);
	  if (stmt_useful_p (stmt))
	    mark_necessary (NULL_TREE, stmt);
	}
    }
}


/* Return true if STMT is necessary.  */

static bool
stmt_useful_p (tree stmt)
{
  varray_type ops;
  stmt_ann_t ann;
  size_t i;

  /* Instructions that are implicitly live.  Function calls, asm and return
     statements are required.  Labels and BIND_EXPR nodes are kept because
     they are control flow, and we have no way of knowing whether they can
     be removed.   DCE can eliminate all the other statements in a block,
     and CFG can then remove the block and labels.  */
  switch (TREE_CODE (stmt))
    {
    case ASM_EXPR:
    case RETURN_EXPR:
    case CASE_LABEL_EXPR:
    case LABEL_EXPR:
    case BIND_EXPR:
    case CALL_EXPR:
    case RESX_EXPR:
      return true;

    case MODIFY_EXPR:
      if (TREE_CODE (TREE_OPERAND (stmt, 1)) == CALL_EXPR)
	return true;

      /* These values are mildly magic bits of the EH runtime.  We can't
	 see the entire lifetime of these values until landing pads are
	 generated.  */
      if (TREE_CODE (TREE_OPERAND (stmt, 0)) == EXC_PTR_EXPR)
	return true;
      if (TREE_CODE (TREE_OPERAND (stmt, 0)) == FILTER_EXPR)
	return true;
      break;

    case COND_EXPR:
      if (TREE_CODE (COND_EXPR_THEN (stmt)) == GOTO_EXPR
	  && TREE_CODE (COND_EXPR_ELSE (stmt)) == GOTO_EXPR)
        {
	  /* Check if the dest labels are the same. If they are, the condition
	     is useless.  */
	  if (GOTO_DESTINATION (COND_EXPR_THEN (stmt))
	      == GOTO_DESTINATION (COND_EXPR_ELSE (stmt)))
	    return false;
	}
    default:
      break;
    }

  if (is_ctrl_stmt (stmt) || is_ctrl_altering_stmt (stmt))
    return true;

  /* If the statement has volatile operands, it needs to be preserved.  */
  ann = stmt_ann (stmt);
  if (ann->has_volatile_ops)
    return true;

  get_stmt_operands (stmt);

  ops = def_ops (ann);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    if (need_to_preserve_store (*(VARRAY_TREE_PTR (ops, i))))
      return true;

  ops = vdef_ops (ann);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    if (need_to_preserve_store (VDEF_RESULT (VARRAY_TREE (ops, i))))
      return true;

  return false;
}


/* Process worklist.  Process the uses on each statement in the worklist,
   and add all feeding statements which contribute to the calculation of 
   this value to the worklist.  */

static void
process_worklist (void)
{
  basic_block bb;
  tree i;

  while (VARRAY_ACTIVE_SIZE (worklist) > 0)
    {
      /* Take `i' from worklist.  */
      i = VARRAY_TOP_TREE (worklist);
      VARRAY_POP (worklist);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "processing: ");
	  print_generic_stmt (dump_file, i, TDF_SLIM);
	  fprintf (dump_file, "\n");
	}

      /* Find any predecessor which 'goto's this block, and mark the goto
	 as necessary since it is control flow.  A block's predecessors only
	 need to be checked once.  */
      bb = bb_for_stmt (i);
      
      if (TREE_CODE (i) == PHI_NODE)
	{
	  int k;

	  /* All the statements feeding this PHI node's arguments are
	     necessary.  */
	  for (k = 0; k < PHI_NUM_ARGS (i); k++)
	    {
	      tree arg = PHI_ARG_DEF (i, k);
	      if (TREE_CODE (arg) == SSA_NAME)
		mark_necessary (arg, NULL);
	    }
	}
      else
	{
	  /* Examine all the USE, VUSE and VDEF operands in this statement.
	     Mark all the statements which feed this statement's uses as
	     necessary.  */
	  varray_type ops;
	  stmt_ann_t ann;
	  size_t k;

	  get_stmt_operands (i);
	  ann = stmt_ann (i);

	  ops = use_ops (ann);
	  for (k = 0; ops && k < VARRAY_ACTIVE_SIZE (ops); k++)
	    {
	      tree *use_p = VARRAY_TREE_PTR (ops, k);
	      mark_necessary (*use_p, NULL_TREE);
	    }

	  ops = vuse_ops (ann);
	  for (k = 0; ops && k < VARRAY_ACTIVE_SIZE (ops); k++)
	    {
	      tree vuse = VARRAY_TREE (ops, k);
	      mark_necessary (vuse, NULL_TREE);
	    }

	  /* The operands of VDEF expressions are also needed as they
	     represent potential definitions that may reach this
	     statement (VDEF operands allow us to follow def-def links).  */
	  ops = vdef_ops (ann);
	  for (k = 0; ops && k < VARRAY_ACTIVE_SIZE (ops); k++)
	    {
	      tree vdef = VARRAY_TREE (ops, k);
	      mark_necessary (VDEF_OP (vdef), NULL_TREE);
	    }
	}
    }
}


/* Eliminate unnecessary instructions. Any instruction not marked as necessary
   contributes nothing to the program, and can be deleted.  */

static void
remove_dead_stmts (void)
{
  basic_block bb;
  tree t;
  block_stmt_iterator i;

  FOR_EACH_BB_REVERSE (bb)
    {
      bsi_list_p stack;
      /* Remove dead PHI nodes.  */
      remove_dead_phis (bb);

      /* Remove dead statements.  */
      FOR_EACH_BSI_IN_REVERSE (stack, bb, i)
	{
	  t = bsi_stmt (i);
	  stats.total++;

	  /* If `i' is not in `necessary' then remove from B.  */
	  if (!necessary_p (t))
	    remove_dead_stmt (&i);
	}
    }
}


/* Remove dead PHI nodes from block BB.  */

static void
remove_dead_phis (basic_block bb)
{
  tree prev, phi;

  prev = NULL_TREE;
  phi = phi_nodes (bb);
  while (phi)
    {
      stats.total_phis++;

      if (!necessary_p (phi))
	{
	  tree next = TREE_CHAIN (phi);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Deleting : ");
	      print_generic_stmt (dump_file, phi, TDF_SLIM);
	      fprintf (dump_file, "\n");
	    }

	  remove_phi_node (phi, prev, bb);
	  stats.removed_phis++;
	  phi = next;
	}
      else
	{
	  prev = phi;
	  phi = TREE_CHAIN (phi);
	}
    }
}


/* Remove dead statement pointed by iterator I from block BB.  */

static void
remove_dead_stmt (block_stmt_iterator *i)
{
  tree t;

  t = bsi_stmt (*i);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Deleting : ");
      print_generic_stmt (dump_file, t, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  stats.removed++;

  if (TREE_CODE (t) == COND_EXPR)
    {
      /* A dead COND_EXPR means the condition is dead. We dont change any
         flow, just replace the expression with a constant.  */
      COND_EXPR_COND (t) = integer_zero_node;
      modify_stmt (t);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "   by replacing the condition with 0:\n");
	  print_generic_stmt (dump_file, t, TDF_SLIM);
	  fprintf (dump_file, "\n");
	}

      return;
    }

#ifdef ENABLE_CHECKING
  if (is_ctrl_stmt (t) || is_ctrl_altering_stmt (t))
    abort ();
#endif

  bsi_remove (i);
}

/* Main routine to eliminate dead code.

   PHASE indicates which dump file from the DUMP_FILES array to use when
   dumping debugging information.  */

void
tree_ssa_dce (tree fndecl, enum tree_dump_index phase)
{
  tree fnbody;

  timevar_push (TV_TREE_DCE);

  memset ((void *) &stats, 0, sizeof (stats));

  fnbody = DECL_SAVED_TREE (fndecl);
  if (fnbody == NULL_TREE)
    abort ();

  VARRAY_TREE_INIT (worklist, 64, "work list");

  processed = sbitmap_alloc (next_ssa_version + 1);
  sbitmap_zero (processed);

  /* Initialize dump_file for debugging dumps.  */
  dump_file = dump_begin (phase, &dump_flags);

  find_useful_stmts ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nProcessing worklist:\n");

  process_worklist ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nEliminating unnecessary instructions:\n");

  sbitmap_free (processed);

  remove_dead_stmts ();
  cleanup_tree_cfg ();

  /* Debugging dumps.  */
  if (dump_file)
    {
      dump_function_to_file (fndecl, dump_file, dump_flags);
      print_stats ();
      dump_end (phase, dump_file);
    }

  timevar_pop (TV_TREE_DCE);
}
