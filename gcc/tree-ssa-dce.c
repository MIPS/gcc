/* Dead code elimination pass for the GNU compiler.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Ben Elliston <bje@redhat.com> and Andrew MacLeod 
   <amacleod@redhat.com>
   
This file is part of GNU CC.
   
GNU CC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GNU CC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to the Free
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
  int removed;
} stats;

/* Forward function prototypes.  */
static bool necessary_p PARAMS ((tree));
static int mark_tree_necessary PARAMS ((tree));
static void mark_necessary PARAMS ((tree));
static void print_stats PARAMS ((void));
static void mark_control_parent_necessary PARAMS ((basic_block));
static int need_to_preserve_store PARAMS ((tree));
static void find_useful_stmts PARAMS ((void));
static void process_worklist PARAMS ((void));
static void remove_dead_stmts PARAMS ((void));


/* Is a tree necessary?  */

static inline bool
necessary_p (t)
     tree t;
{
  return (tree_flags (t) & TF_NECESSARY);
}


/* Mark a tree as necessary.  Return 1 if it was not already marked.  */

static int
mark_tree_necessary (t)
     tree t;
{
  if (t == NULL || necessary_p (t))
    return 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Marking useful stmt: ");
      print_generic_stmt (dump_file, t, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  set_tree_flag (t, TF_NECESSARY);
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
  gimple_stmt_iterator i;
  tree t;

  /* Loops through the stmts in this block, marking them as necessary. */
  while (bb != NULL && bb->index != INVALID_BLOCK)
    {
      for (i = gsi_start_bb (bb); !gsi_end_bb (i); gsi_step_bb (&i))
	{
	  /* Avoid needless calls back to this routine by directly calling 
	     mark_tree since we know we are going to cycle through all parent 
	     blocks and their statements.  */
	  t = gsi_stmt (i);
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
      dump_end (TDI_dce, dump_file);
    }
}


/* Return 1 if a store to a symbol or expression will need to be preserved.  */

static int
need_to_preserve_store (sym)
     tree sym;
{
  tree base_symbol;

  if (sym == NULL)
    return 0;

  base_symbol = get_base_symbol (sym);

  /* File scope variables must be preserved.  */
  if (decl_function_context (base_symbol) == NULL)
    return 1;
  
  /* Stores through parameter pointers must be perserved.  */
  if (TREE_CODE (sym) == INDIRECT_REF
      && TREE_CODE (base_symbol) == PARM_DECL)
    return 1;

  /* Static locals must be preserved as well.  */
  if (TREE_STATIC (base_symbol))
    return 1;

  /* If SYM may alias global memory, we also need to preserve the store.  */
  if (may_alias_global_mem_p (sym))
    return 1;

  return 0;
}


/* Find obviously useful instructions.  These are things like function
   calls and stores to file level variables.  */

static void
find_useful_stmts ()
{
  basic_block bb;
  tree t;
  ref_list blockrefs;
  gimple_stmt_iterator i;

  FOR_EACH_BB (bb)
    {
      if (bb_empty_p (bb))
	continue;
	  
      for (i = gsi_start_bb (bb); !gsi_end_bb (i); gsi_step_bb (&i))
	{
	  ref_list_iterator j;
	  tree stmt;

	  t = stmt = gsi_stmt (i);
	  STRIP_WFL (stmt);
	  STRIP_NOPS (stmt);

	  /* Asms and Returns are required. Labels are kept because 
	     they are control flow, and we have no way of knowing 
	     whether they can be removed.   DCE can eliminate all the 
	     other statements in a block, and CFG can then remove the block
	     and labels.  VA_ARG_EXPR behaves like a builtin function call
	     to __builtin_va_arg().
	     Functions calls do not need to be checked explicitly since we
	     will see a REF to the functions global name.  */

	  if ((TREE_CODE (stmt) == ASM_EXPR) 
	      || (TREE_CODE (stmt) == RETURN_EXPR)
	      || (TREE_CODE (stmt) == CASE_LABEL_EXPR)
	      || (TREE_CODE (stmt) == VA_ARG_EXPR)
	      || ((TREE_CODE (stmt) == MODIFY_EXPR)
		  && (TREE_CODE (TREE_OPERAND (stmt, 1)) == VA_ARG_EXPR))
	      || (TREE_CODE (stmt) == LABEL_EXPR))
	    {
	      mark_necessary (t);
	      t = TREE_CHAIN (t);
	      continue;
	    }

	  blockrefs = tree_refs (t);
	  if (! blockrefs)
	    continue;

	  /* iterate through all references in this statement.  */
	  for (j = rli_start (blockrefs); !rli_after_end (j); rli_step (&j))
	    {
	      tree_ref ref = rli_ref (j);
	      enum tree_ref_type type = ref_type (ref);

	      /* Look for stores to something which needs to be preserved.  */
	      if (type == V_DEF && !ref->vdef.m_relocate)
		{
		  if (TREE_THIS_VOLATILE (ref_var (ref)))
		    mark_necessary (t);
		  else 
		    {
		      tree symbol = ref_var (ref);
		      int need = 0;
		      unsigned int x;

		      if (need_to_preserve_store (symbol))
			need = 1;
		      else
			/* Or If this aliases with something that needs to 
			   be preserved, keep it.  */
			if (is_aliased (symbol))
			  {
			    for (x = 0; x < num_referenced_vars; x++)
			      {
				tree var = referenced_var (x);
				if (alias_leader (var) == alias_leader (symbol)
				    && need_to_preserve_store (var))
				  {
				    need = 1;
				    break;
				  }
			      }
			  }

		      if (need)
			mark_necessary (t);
		    }
		}
	    }
	}
    }
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
  ref_list_iterator k;

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

      /* Find any predecessor's which 'goto's this block, and mark the goto
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
      
      /* Examine all the uses in this statement, and mark all the statements 
	 which feed this statement's uses as necessary.  */
      for (k = rli_start (tree_refs (i)); !rli_after_end (k); rli_step (&k))
	{
	  ref_list_iterator l;
	  tree_ref r = rli_ref (k);

	  if (ref_type (r) != V_USE)
	    continue;

	  l = rli_start (reaching_defs (r));
	  for (; !rli_after_end (l); rli_step (&l))
	    {
	      tree_ref rdef = rli_ref (l);
	      j = ref_stmt (rdef);
	      if (j)
		mark_necessary (j);
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
  gimple_stmt_iterator i;
  dominance_info dom_info = NULL;

  FOR_EACH_BB (bb)
    {

      if (bb_empty_p (bb))
	continue;

      for (i = gsi_start_bb (bb); !gsi_end_bb (i); gsi_step_bb (&i))
	{
	  t = gsi_stmt (i);
	  stats.total++;

	  /* If `i' is not in `necessary' then remove from B.  */
	  if (!necessary_p (t))
	    {
	      /* Remove `i' from B.  */
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "Deleting : ");
		  print_generic_stmt (dump_file, t, TDF_SLIM);
		  fprintf (dump_file, "\n");
		}
	      stats.removed++;

	      STRIP_WFL (t);
	      STRIP_NOPS (t);

	      /* If we have determined that a conditional branch statement
		 contributes nothing to the program, then we not only
		 remove it, but change the flowgraph so that the block points
		 directly to the immediate post-dominator. The flow
		 graph will remove the blocks we are circumventing, and this
		 block will then simply fall-thru to the post-dominator. This 
		 prevents us from having to add any branch instuctions to 
		 replace the conditional statement.  */
	      if (TREE_CODE (t) == COND_EXPR || TREE_CODE (t) == SWITCH_EXPR)
		{
		  basic_block nb;
		  edge e;

		  /* Calculate the doiminance info, if it hasn't been yet.  */
		  if (dom_info == NULL)
		    dom_info = calculate_dominance_info (1);
		  nb = get_immediate_dominator (dom_info, bb);

 		  /* Remove all outgoing edges, and add an edge to the
		     post dominator.  */
		  for (e = bb->succ; e != NULL; )
		    {
		      edge tmp = e;
		      e = e->succ_next;
		      remove_edge (tmp);
		    }
		  make_edge (bb, nb, EDGE_FALLTHRU);
		}
	      gsi_remove (i);
	    }
	  else
	    {
	      /* Clear the 'necessary' flag for the next time DCE is run.  */
	      clear_tree_flag (t, TF_NECESSARY);
	    }
	}
    }

  /* If we needed the dominance info, free it now.  */
  if (dom_info != NULL)
    free_dominance_info (dom_info);
}


/* Main routine to eliminate dead code.  */

void
tree_ssa_eliminate_dead_code (fndecl)
     tree fndecl;
{
  tree fnbody;

  timevar_push (TV_TREE_DCE);

  stats.total = stats.removed = 0;

  fnbody = DECL_SAVED_TREE (fndecl);
  if (fnbody == NULL_TREE)
    abort ();

  VARRAY_TREE_INIT (worklist, 64, "work list");

  /* Compute reaching definitions.  */
  compute_reaching_defs ();    

  /* Initialize dump_file for debugging dumps.  */
  dump_file = dump_begin (TDI_dce, &dump_flags);

  find_useful_stmts ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nProcessing worklist:\n");

  process_worklist ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nEliminating unnecessary instructions:\n");

  remove_dead_stmts ();

  dump_end (TDI_dce, dump_file);

  timevar_pop (TV_TREE_DCE);

  /* Dump the function tree after DCE.  */
  dump_function (TDI_dce, fndecl);
  print_stats ();
}
