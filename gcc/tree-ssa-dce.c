/* Dead code elimination pass for the GNU compiler.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Ben Elliston <bje@redhat.com>
   
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
#include "errors.h"
#include "ggc.h"
#include "tree.h"

/* These RTL headers are needed for basic-block.h.  */
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"

#include "tree-flow.h"
#include "tree-simple.h"
#include "tree-optimize.h"

/* This should be eventually be generalized to other languages, but
   this would require a shared function-as-trees infrastructure.  */
#include "c-common.h"
#include "c-tree.h"


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
static void mark_necessary PARAMS ((tree));
static void print_stats PARAMS ((void));
static void mark_control_parent_necessary PARAMS ((basic_block));

static void
mark_necessary (t)
     tree t;
{
  set_tree_flag (t, TF_NECESSARY);

  /* Mark all statements in control parent blocks as necessary.  */
  if (bb_for_stmt (t))
    mark_control_parent_necessary (bb_parent (bb_for_stmt (t)));
}

static void
mark_control_parent_necessary (bb)
     basic_block bb;
{
  tree t;

  if (bb)
    {
      for (t = bb->head_tree; t;)
	{
	  mark_necessary (t);
	  VARRAY_PUSH_TREE (worklist, t);
	  if (t == bb->end_tree)
	    break;
	  t = TREE_CHAIN (t);
	}
      mark_control_parent_necessary (bb_parent (bb));
    }
}

static void
print_stats ()
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      float percg;
      percg = ((float) stats.removed / (float) stats.total) * 100;
      fprintf (dump_file, "Removed %d of %d statements (%d%%)\n",
			  stats.removed, stats.total, (int) percg);
    }
}

/* Is a tree necessary?  */
static inline bool
necessary_p (t)
     tree t;
{
  if (tree_annotation (t))
    return (tree_flags (t) & TF_NECESSARY);
  else
    return false;
}

void
tree_ssa_eliminate_dead_code (fndecl)
     tree fndecl;
{
  basic_block bb;
  tree fnbody, t;
  struct ref_list_node *tmp;

  stats.total = stats.removed = 0;

  fnbody = COMPOUND_BODY (DECL_SAVED_TREE (fndecl));
  if (fnbody == NULL_TREE)
    abort ();

  VARRAY_TREE_INIT (worklist, 64, "work list");

  /* Debugging dumps.  */
  dump_file = dump_begin (TDI_dce, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "%s\n",
	       IDENTIFIER_POINTER (DECL_NAME (fndecl)));
      
      if (dump_flags & TDF_RAW)
	dump_node (fnbody, TDF_SLIM | dump_flags, dump_file);
      else
	print_c_tree (dump_file, fnbody);

      fclose (dump_file);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Finding obviously useful instructions:\n");

  /* Find obviously useful instructions.  */
  FOR_EACH_BB (bb)
    {
      tree_ref ref;
      ref_list blockrefs;

      if (bb_empty_p (bb))
	continue;
	  
      for (t = bb->head_tree; t;)
	{
	  if (TREE_CODE (t) == ASM_STMT)
	    {
	      mark_necessary (t);
	      VARRAY_PUSH_TREE (worklist, t);
	      t = TREE_CHAIN (t);
	      continue;
	    }

	  /* FIXME  Weird guard -- seems to be necessary.  */
	  if (!tree_annotation (t))
	    return;

	  blockrefs = tree_refs (t);
	  if (! blockrefs)
	    { 
	      t = TREE_CHAIN (t);
	      continue;
	    }

	  FOR_EACH_REF (ref, tmp, blockrefs)
	    {
	      HOST_WIDE_INT type = ref_type (ref);

	      if (type & V_DEF)
		{
		  if (TREE_THIS_VOLATILE (ref_var (ref)))
		    {
		      mark_necessary (t);
		      VARRAY_PUSH_TREE (worklist, t);
		      mark_necessary (get_base_symbol (ref_var (ref)));
		    }
		  /* FIXME Hack to deal with null replies from
		     get_base_symbol().  */
		  else if (get_base_symbol (ref_var (ref)) &&
			   (DECL_CONTEXT (get_base_symbol (ref_var (ref)))) == NULL)
		    {
		      mark_necessary (t);
		      VARRAY_PUSH_TREE (worklist, t);
		      mark_necessary (get_base_symbol (ref_var (ref)));
		    }
		}
	      else if (type & E_FCALL)
		  {
		    mark_necessary (t);
		    VARRAY_PUSH_TREE (worklist, t);
		  }
	    }
	  if (t == bb->end_tree)
	    break;
	  t = TREE_CHAIN (t);
	}
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Processing worklist:\n");

  /* Process worklist.  */
  while (VARRAY_ACTIVE_SIZE (worklist) > 0)
    {
      tree i, j;
      basic_block b;
      tree_ref r, rdef;

      /* Take `i' from worklist.  */
      i = VARRAY_TOP_TREE (worklist);
      VARRAY_POP (worklist);

      /* Let `b' be the block containing `i'.  */
      b = bb_for_stmt (i);

      /* For each use by `i' .. */
      FOR_EACH_REF (r, tmp, tree_refs (i))
	{
	  struct ref_list_node *tmp2;
	  if (! (ref_type (r) & V_USE))
	    continue;

	  FOR_EACH_REF (rdef, tmp2, reaching_defs (r))
	    {
	      /* J = definition (T).  */
	      j = ref_stmt (rdef);
	      if (j && ! necessary_p (j))
		{
		  mark_necessary (j);
		  VARRAY_PUSH_TREE (worklist, j);
		  mark_necessary (get_base_symbol (ref_var (rdef)));
		}
	    }
	}
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Eliminating unnecessary instructions:\n");

  /* Eliminate unnecessary instructions.  */
  FOR_EACH_BB (bb)
    {
      tree prev;

      if (bb_empty_p (bb))
	continue;

      for (t = bb->head_tree; t;)
	{
	  stats.total++;

	  /* If `i' is not in `necessary' then remove from B.  */
	  if (!necessary_p (t) && (TREE_CODE (t) == EXPR_STMT))
	    {
	      /* Remove `i' from B.  */
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "Warning: removing ");
		  print_c_node (dump_file, t);
		}
	      stats.removed++;

	      /* Unlink `i'. Head and tail are special cases.  */
	      if (t == bb->head_tree)
		bb->head_tree = TREE_CHAIN (t);
	      else 
		{
		  if (t == bb->end_tree)
		    bb->end_tree = prev;
		  TREE_CHAIN (prev) = TREE_CHAIN (t);
		}
	    }
	  else
	    prev = t;

	  if (t == bb->end_tree)
	    break;
	  t = TREE_CHAIN (t);
	}
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "New translation unit:\n");
      print_c_tree (dump_file, fnbody);
    }
  print_stats ();
}
