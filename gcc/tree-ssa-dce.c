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
static dominance_info dom_info = NULL;

static struct stmt_stats
{
  int total;
  int total_phis;
  int removed;
  int removed_phis;
} stats;

static htab_t needed_stmts;

/* Forward function prototypes.  */
static bool necessary_p				PARAMS ((tree));
static int mark_tree_necessary			PARAMS ((tree));
static void mark_necessary			PARAMS ((tree));
static void print_stats 			PARAMS ((void));
static void mark_control_parent_necessary	PARAMS ((basic_block));
static bool need_to_preserve_store		PARAMS ((tree));
static void find_useful_stmts			PARAMS ((void));
static bool stmt_useful_p			PARAMS ((tree));
static void process_worklist			PARAMS ((void));
static void remove_dead_stmts			PARAMS ((void));
static void remove_dead_stmt			PARAMS ((block_stmt_iterator,
      							 basic_block));
static void remove_dead_phis			PARAMS ((basic_block));


/* Is a tree necessary?  */

static inline bool
necessary_p (t)
     tree t;
{
  return htab_find (needed_stmts, t) != NULL;
}


/* Mark a tree as necessary.  Return 1 if it was not already marked.  */

static int
mark_tree_necessary (t)
     tree t;
{
  void **slot;

  if (t == NULL
      || t == empty_stmt_node
      || t == error_mark_node
      || necessary_p (t))
    return 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Marking useful stmt: ");
      print_generic_stmt (dump_file, t, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  slot = htab_find_slot (needed_stmts, t, INSERT);
  *slot = (void *) t;
  VARRAY_PUSH_TREE (worklist, t);

  return 1;
}


/* Mark a tree as necessary, and mark it's control parents as well.  */

static void
mark_necessary (t)
     tree t;
{
  if (mark_tree_necessary (t))
    {
      /* Mark all statements in control parent blocks as necessary.  */
      if (bb_for_stmt (t))
	mark_control_parent_necessary (parent_block (bb_for_stmt (t)));
    }
}


/* Mark all statements in all nested control parent block as necessary
   statements.  */
   
static void
mark_control_parent_necessary (bb)
     basic_block bb;
{
  block_stmt_iterator i;
  tree t;

  /* Loops through the stmts in this block, marking them as necessary. */
  while (bb != NULL && bb->index != INVALID_BLOCK)
    {
      for (i = bsi_start (bb); !bsi_end_p (i); bsi_next (&i))
	{
	  /* Avoid needless calls back to this routine by directly calling 
	     mark_tree since we know we are going to cycle through all parent 
	     blocks and their statements.  */
	  t = bsi_stmt (i);
	  mark_tree_necessary (t);
	}
      bb = parent_block (bb);
    }
}


/* Print out removed statement statistics.  */

static void
print_stats ()
{
  dump_file = dump_begin (TDI_dce, &dump_flags);
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

      dump_end (TDI_dce, dump_file);
    }
}


/* Return true if a store to a variable needs to be preserved.  */

static bool
need_to_preserve_store (var)
     tree var;
{
  tree base_symbol;
  tree sym;

  if (var == NULL)
    return false;

  sym = SSA_NAME_VAR (var);
  base_symbol = get_base_symbol (var);

  /* Stores to volatiles must be preserved.  */
  if (TREE_THIS_VOLATILE (sym))
    return true;

  /* File scope variables must be preserved.  */
  if (decl_function_context (base_symbol) == NULL)
    return true;
  
  /* Stores through parameter pointers must be perserved.  */
  if (TREE_CODE (sym) == INDIRECT_REF
      && TREE_CODE (base_symbol) == PARM_DECL)
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
find_useful_stmts ()
{
  basic_block bb;
  block_stmt_iterator i;

  FOR_EACH_BB (bb)
    {
      tree phi;

      /* Check any PHI nodes in the block.  */
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	if (need_to_preserve_store (PHI_RESULT (phi)))
	  mark_necessary (phi);

      /* Check all statements in the block.  */
      for (i = bsi_start (bb); !bsi_end_p (i); bsi_next (&i))
	{
	  tree stmt = bsi_stmt (i);
	  STRIP_NOPS (stmt);

	  if (stmt_useful_p (stmt))
	    mark_necessary (stmt);
	}
    }
}


/* Return true if STMT is necessary.  */

static bool
stmt_useful_p (stmt)
     tree stmt;
{
  varray_type ops;
  size_t i;

  /* Instructions that are implicitly live.  Function calls, asm and return
     statements are required.  Labels are kept because they are control
     flow, and we have no way of knowing whether they can be removed.   DCE
     can eliminate all the other statements in a block, and CFG can then
     remove the block and labels.  */
  if ((TREE_CODE (stmt) == ASM_EXPR)
      || (TREE_CODE (stmt) == RETURN_EXPR)
      || (TREE_CODE (stmt) == CASE_LABEL_EXPR)
      || (TREE_CODE (stmt) == CALL_EXPR)
      || (TREE_CODE (stmt) == LABEL_EXPR)
      || ((TREE_CODE (stmt) == MODIFY_EXPR)
	  && (TREE_CODE (TREE_OPERAND (stmt, 1)) == CALL_EXPR)))
    return true;

  /* GOTO_EXPR nodes to nonlocal labels need to be kept (This fixes
     gcc.c-torture/execute/920501-7.c and others that have nested functions
     with nonlocal gotos).  FIXME: If we were doing IPA we could determine
     if the label is actually reachable.  */
  if (TREE_CODE (stmt) == GOTO_EXPR)
    {
      edge e;
      for (e = bb_for_stmt (stmt)->succ; e; e = e->succ_next)
	if (e->dest == EXIT_BLOCK_PTR && e->flags & EDGE_ABNORMAL)
	  return true;
    }

  /* Examine all the stores in this statement.  */
  get_stmt_operands (stmt);

  if (def_op (stmt) && need_to_preserve_store (*(def_op (stmt))))
    return true;

  ops = vdef_ops (stmt);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    if (need_to_preserve_store (VDEF_RESULT (VARRAY_TREE (ops, i))))
      return true;

  return false;
}


/* Process worklist.  Process the uses on each statement in the worklist,
   and add all feeding statements which contribute to the calculation of 
   this value to the worklist.  */

static void
process_worklist ()
{
  basic_block bb;
  tree i, j;
  edge e;

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

      bb = bb_for_stmt (i);

      /* Find any predecessor which 'goto's this block, and mark the goto
	 as necessary since it is control flow.  */
      for (e = bb->pred; e != NULL; e = e->pred_next)
	{
	  basic_block p = e->src;
	  if (p == ENTRY_BLOCK_PTR)
	    continue;
	  j = last_stmt (p);
	  if (j && TREE_CODE (j) == GOTO_EXPR)
	    mark_necessary (j);
	}
      
      if (TREE_CODE (i) == PHI_NODE)
	{
	  int k;

	  /* All the statements feeding this PHI node's arguments are
	     necessary.  */
	  for (k = 0; k < PHI_NUM_ARGS (i); k++)
	    mark_necessary (SSA_NAME_DEF_STMT (PHI_ARG_DEF (i, k)));
	}
      else
	{
	  /* Examine all the USE, VUSE and VDEF operands in this statement.
	     Mark all the statements which feed this statement's uses as
	     necessary.  */
	  varray_type ops;
	  size_t k;

	  get_stmt_operands (i);

	  ops = use_ops (i);
	  for (k = 0; ops && k < VARRAY_ACTIVE_SIZE (ops); k++)
	    {
	      tree *use_p = VARRAY_GENERIC_PTR (ops, k);
	      mark_necessary (SSA_NAME_DEF_STMT (*use_p));
	    }

	  ops = vuse_ops (i);
	  for (k = 0; ops && k < VARRAY_ACTIVE_SIZE (ops); k++)
	    {
	      tree vuse = VARRAY_TREE (ops, k);
	      mark_necessary (SSA_NAME_DEF_STMT (vuse));
	    }

	  /* The operands of VDEF expressions are also needed as they
	     represent potential definitions that may reach this
	     statement (VDEF operands allow us to follow def-def links).  */
	  ops = vdef_ops (i);
	  for (k = 0; ops && k < VARRAY_ACTIVE_SIZE (ops); k++)
	    {
	      tree vdef = VARRAY_TREE (ops, k);
	      mark_necessary (SSA_NAME_DEF_STMT (VDEF_OP (vdef)));
	    }
	}
    }
}


/* Eliminate unnecessary instructions. Any instuction not marked as necessary
   contributes nothing to the program, and can be deleted.  */

static void
remove_dead_stmts ()
{
  basic_block bb;
  tree t;
  block_stmt_iterator i;

  dom_info = NULL;

  FOR_EACH_BB (bb)
    {
      /* Remove dead PHI nodes.  */
      remove_dead_phis (bb);

      /* Remove dead statements.  */
      for (i = bsi_start (bb); !bsi_end_p (i); bsi_next (&i))
	{
	  t = bsi_stmt (i);
	  stats.total++;

	  /* If `i' is not in `necessary' then remove from B.  */
	  if (!necessary_p (t))
	    remove_dead_stmt (i, bb);
	}
    }

  /* If we needed the dominance info, free it now.  */
  if (dom_info != NULL)
    free_dominance_info (dom_info);
}


/* Remove dead PHI nodes from block BB.  */

static void
remove_dead_phis (bb)
     basic_block bb;
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
remove_dead_stmt (i, bb)
     block_stmt_iterator i;
     basic_block bb;
{
  tree t;

  t = bsi_stmt (i);
  STRIP_NOPS (t);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Deleting : ");
      print_generic_stmt (dump_file, t, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  stats.removed++;

  /* If we have determined that a conditional branch statement contributes
     nothing to the program, then we not only remove it, but change the
     flowgraph so that the block points directly to the immediate
     post-dominator.  The flow graph will remove the blocks we are
     circumventing, and this block will then simply fall-thru to the
     post-dominator.  This prevents us from having to add any branch
     instuctions to replace the conditional statement.  */
  if (TREE_CODE (t) == COND_EXPR || TREE_CODE (t) == SWITCH_EXPR)
    {
      basic_block nb;
      edge e;

      /* Calculate the dominance info, if it hasn't been computed yet.  */
      if (dom_info == NULL)
	dom_info = calculate_dominance_info (1);
      nb = get_immediate_dominator (dom_info, bb);

      /* Remove all outgoing edges, and add an edge to the
         post dominator.  */
      for (e = bb->succ; e != NULL;)
	{
	  edge tmp = e;
	  e = e->succ_next;
	  remove_edge (tmp);
	}
      make_edge (bb, nb, EDGE_FALLTHRU);
    }

  bsi_remove (i);
}

/* Main routine to eliminate dead code.  */

void
tree_ssa_dce (fndecl)
     tree fndecl;
{
  tree fnbody;

  timevar_push (TV_TREE_DCE);

  memset ((void *) &stats, 0, sizeof (stats));

  fnbody = DECL_SAVED_TREE (fndecl);
  if (fnbody == NULL_TREE)
    abort ();

  VARRAY_TREE_INIT (worklist, 64, "work list");

  needed_stmts = htab_create (64, htab_hash_pointer, htab_eq_pointer, NULL);

  /* Initialize dump_file for debugging dumps.  */
  dump_file = dump_begin (TDI_dce, &dump_flags);

  find_useful_stmts ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nProcessing worklist:\n");

  process_worklist ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nEliminating unnecessary instructions:\n");

  remove_dead_stmts ();

  if (dump_file)
    dump_end (TDI_dce, dump_file);

  htab_delete (needed_stmts);

  timevar_pop (TV_TREE_DCE);

  /* Dump the function tree after DCE.  */
  dump_function (TDI_dce, fndecl);
  print_stats ();
}
