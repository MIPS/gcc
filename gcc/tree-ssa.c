/* SSA for trees.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
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
#include "flags.h"
#include "rtl.h"
#include "tm_p.h"
#include "ggc.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "diagnostic.h"

/* This should be eventually be generalized to other languages, but
   this would require a shared function-as-trees infrastructure.  */
#include "c-common.h"
#include "c-tree.h"
#include "bitmap.h"

#include "tree-optimize.h"
#include "tree-flow.h"
#include "tree-simple.h"
#include "tree-inline.h"
#include "ssa.h"
#include "varray.h"


/* Nonzero to warn about variables used before they are initialized.  Used
   by analyze_rdefs().  */
int tree_warn_uninitialized = 0;


/* Array of saved definition chains.  Used by search_fud_chains to restore
   the previous definition chain when returning from a recursive call.  */
static tree_ref *save_chain;


/* Visited marks.  Used when computing reaching definitions (follow_chain).  */
static tree_ref *marked;


/* Dump file and flags.  */
static FILE *dump_file;
static int dump_flags;


/* Local functions.  */
static void insert_phi_terms PARAMS ((sbitmap *));
static void build_fud_chains PARAMS ((dominance_info));
static void search_fud_chains PARAMS ((basic_block, dominance_info));
static void follow_chain PARAMS ((tree_ref, tree_ref));


/* Build the SSA form for the given function. This implements Factored
   Use-Def Chains as described in 

   Wolfe, M. J., High Performance Compilers for Parallel Computing,
   Addison-Wesley, 1996.  */

void
tree_build_ssa ()
{
  sbitmap *dfs;
  dominance_info idom;
  
  /* Compute immediate dominators.  */
  idom = calculate_dominance_info (CDI_DOMINATORS);

  /* Compute dominance frontiers.  */
  dfs = sbitmap_vector_alloc (last_basic_block, last_basic_block);
  compute_dominance_frontiers (dfs, idom);

  /* Insert the PHI terms and build FUD chains.  */
  insert_phi_terms (dfs);
  build_fud_chains (idom);

  sbitmap_vector_free (dfs);
  free_dominance_info (idom);

  /* Debugging dumps.  */
  dump_file = dump_begin (TDI_ssa, &dump_flags);

  if (dump_file)
    {
      dump_tree_ssa (dump_file);
      dump_end (TDI_ssa, dump_file);
    }
}


/* Insert PHI terms at the dominance frontier of nodes with variable
   definitions.  */

static void
insert_phi_terms (dfs)
     sbitmap *dfs;
{
  size_t i;
  varray_type added;
  varray_type in_work;
  varray_type work_stack;

  /* Array ADDED (indexed by basic block number) is used to determine
     whether a PHI term for the current variable has already been
     inserted at block X.  */
  VARRAY_TREE_INIT (added, last_basic_block, "added");

  /* Array IN_WORK (indexed by basic block number) is used to determine
     whether block X has already been added to WORK_STACK for the current
     variable.  */
  VARRAY_TREE_INIT (in_work, last_basic_block, "in_work");

  /* Array WORK_STACK is a stack of CFG blocks.  Each block that contains
     an assignment or PHI term will be pushed to this stack.  */
  VARRAY_BB_INIT (work_stack, last_basic_block, "work_stack");

  /* Iterate over all referenced variables in the function.  For each
     variable, add to the work list all the blocks that have a definition
     for the variable.  PHI nodes will be added to the dominance frontier
     blocks of each definition block.  */
  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      struct ref_list_node *tmp;
      tree_ref ref;

#if defined ENABLE_CHECKING
      /* Variables in referenced_vars must have at least 1 reference.  */
      if (tree_refs (var)->first == NULL)
	abort ();
#endif

      FOR_EACH_REF (ref, tmp, tree_refs (var))
	{
	  basic_block bb = ref_bb (ref);

	  if (ref_type (ref) & V_DEF)
	    {
	      VARRAY_PUSH_BB (work_stack, bb);
	      VARRAY_TREE (in_work, bb->index) = var;
	    }
	}

      while (VARRAY_ACTIVE_SIZE (work_stack) > 0)
	{
	  size_t w;
	  basic_block bb;

	  bb = VARRAY_TOP_BB (work_stack);
	  VARRAY_POP (work_stack);

	  EXECUTE_IF_SET_IN_SBITMAP (dfs[bb->index], 0, w,
	    {
	      if (VARRAY_TREE (added, w) != var)
		{
		  tree_ref phi;
		  basic_block stmt_bb;
		  basic_block bb = BASIC_BLOCK (w);

		  /* Determine the parent statement for the new PHI
		     term.  If BB is a block for an expression, look in
		     its control parent.  This is needed because
		     variable references must be associated with
		     statement trees.  */
		  stmt_bb = bb;
		  while (stmt_bb
		         && !statement_code_p (TREE_CODE (stmt_bb->head_tree)))
		    stmt_bb = bb_parent (stmt_bb);

		  phi = create_ref (var, V_PHI, bb, stmt_bb->head_tree, NULL,
		                    NULL, false);
		  add_ref_to_list_begin (bb_refs (bb), phi);

		  VARRAY_TREE (added, w) = var;

		  if (VARRAY_TREE (in_work, w) != var)
		    {
		      VARRAY_PUSH_BB (work_stack, bb);
		      VARRAY_TREE (in_work, w) = var;
		    }
		}
	    });
	}
    }
}


/* Build FUD (Factored Use-Def) chains.  */

static void
build_fud_chains (idom)
     dominance_info idom;
{
  size_t i;

  /* Initialize the current definition for all the variables.  */
  for (i = 0; i < num_referenced_vars; i++)
    set_currdef_for (referenced_var (i), NULL);

  /* Initialize the array of saved definition chains.  Used by
     search_fud_chains to restore the previous definition chain when
     returning from a recursive call.  */
  save_chain = (tree_ref *) xmalloc (next_tree_ref_id * sizeof (tree_ref));
  if (save_chain == NULL)
    abort ();

  /* Search FUD chains starting with the entry block.  */
  search_fud_chains (ENTRY_BLOCK_PTR, idom);

  free (save_chain);
}


/* Perform a depth-first traversal of the dominator tree looking for FUD
   chains.  */

static void
search_fud_chains (bb, idom)
     basic_block bb;
     dominance_info idom;
{
  edge e;
  basic_block child_bb;
  struct ref_list_node *tmp;
  tree_ref ref;

  /* for each variable use or def or phi-term R in BB do
         let VAR be the variable referenced at R
         if R is a use then
             Chain(R) = CurrDef(VAR)
         else if R is a def or $\phi$-term then
             SaveChain(R) = CurrDef(VAR)
             CurrDef(VAR) = R
         endif
     endfor  */
  FOR_EACH_REF (ref, tmp, bb_refs (bb))
    {
      tree_ref currdef;
      tree var = ref_var (ref);

      if (!(ref_type (ref) & (V_DEF | V_USE | V_PHI)))
	continue;

      /* Retrieve the current definition for the variable.  */
      currdef = currdef_for (var);

      if (ref_type (ref) & V_USE)
	{
	  /* Set up a def-use chain between CURRDEF (the immediately
	     reaching definition for REF) and REF.  Each definition may
	     have more than one immediate use, so make sure that we don't
	     add REF to CURRDEF's immediate uses more than once.  */
	  if (currdef && imm_reaching_def (ref) != currdef)
	    add_ref_to_list_end (imm_uses (currdef), ref);

	  /* Set up a use-def chain between REF and CURRDEF.  */
	  set_imm_reaching_def (ref, currdef);
	}
      else if (ref_type (ref) & (V_DEF | V_PHI))
	{
	  /* Save the current definition chain and replace the current reaching
	     definition with a new one.  */
	  save_chain[ref_id (ref)] = currdef;

	  /* If the current reference (REF) is a non-killing definition
	     (may-def or partial-def), then set a def-def chain between REF
	     and CURRDEF.  This is used to chain definitions to arrays and
	     pointers so that all possible reaching defs can be found later
	     by tree_compute_rdefs.  For instance,

			  1	x = 5;
			  2	*ptr = 10;
			  3	y = x + 10;

	     Since the assignment to '*ptr' at line 2 introduces a may-def
	     for 'x', without def-def chains the use of 'x' at line 3 will
	     not be reached by the defintion at line 1.  */
	  if (ref_type (ref) & (M_PARTIAL | M_MAY))
	    set_imm_reaching_def (ref, currdef);

	  set_currdef_for (var, ref);
	}
    }


  /* for Y in SUCC(BB) do
        J = WhichPred(BB -> Y)
        for each phi-term R in Y do
            let VAR be the variable referenced at R
            phi-chain(R)[J] = CurrDef(VAR)
        endfor
     endfor  */
  for (e = bb->succ; e; e = e->succ_next)
    {
      
      basic_block y;
      tree_ref phi;

      y = e->dest;
      if (bb_refs (y) == NULL)
	continue;

      FOR_EACH_REF (phi, tmp, bb_refs (y))
	{
	  tree var;
	  tree_ref currdef;
	
	  if (!(ref_type (phi) & V_PHI))
	    continue;

	  var = ref_var (phi);
	  currdef = currdef_for (var);

	  /* Besides storing the incoming definition CURRDEF, we also store
	     E, which is the edge that we are receiving CURRDEF from.   */
	  if (currdef)
	    {
	      add_phi_arg (phi, currdef, e);

	      /* Set a def-use edge between CURRDEF and this PHI node.  */
	      add_ref_to_list_end (imm_uses (currdef), phi);
	    }
	}
    }


  /* for Y in Child(BB) do	<-- Child(BB) is the set of dominator
    	   Search(Y)                children of BB in the dominator tree.
     endfor  */
  FOR_EACH_BB (child_bb)
    {
      if (get_immediate_dominator (idom, child_bb)->index == bb->index)
	search_fud_chains (child_bb, idom);
    }


  /* for each variable use or def or phi-term R in BB in reverse order do
         let VAR be the variable referenced at R
         if R is a def or a phi-term then
             CurrDef(VAR) = SaveChain(R)
         endif
     endfor  */
  FOR_EACH_REF_REV (ref, tmp, bb_refs (bb))
    {
      if (ref_type (ref) & (V_DEF | V_PHI))
	set_currdef_for (ref_var (ref), save_chain[ref_id (ref)]);
    }
}


/* Deallocate memory associated with SSA data structures.  */

void
delete_ssa ()
{
  size_t i;

  /* Remove annotations from every variable.  We should only need to remove
     annotations from global variables, because those are the only ones that
     might be re-used in other functions.  But better be safe.  */
  for (i = 0; i < num_referenced_vars; i++)
    remove_tree_ann (referenced_var (i));

  delete_ref_list (call_sites);
  call_sites = NULL;
  num_referenced_vars = 0;
  referenced_vars = NULL;
}


/* Reaching definitions.  */

/* Computes reaching definitions and reached uses for all the variables
   referenced in the current function.  */

void
tree_compute_rdefs ()
{
  size_t i;

  /* Initialize reaching definition and reached uses information for every
     reference in the function.  */
  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      tree_ref r;
      struct ref_list_node *tmp;

      FOR_EACH_REF (r, tmp, tree_refs (var))
	{
	  if (ref_type (r) & V_USE)
	    empty_ref_list (reaching_defs (r));
	  else if (ref_type (r) & (V_DEF | V_PHI))
	    empty_ref_list (reached_uses (r));
	}
    }

  /* Initialize the mark array to keep track of which definitions have been
     visited already.  */
  marked = (tree_ref *) xmalloc (next_tree_ref_id * sizeof (tree_ref));
  if (marked == NULL)
    abort ();

  /* Traverse all the uses following their use-def chains looking for
     reaching definitions and reached uses.  */
  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      tree_ref u;
      struct ref_list_node *tmp;

      FOR_EACH_REF (u, tmp, tree_refs (var))
	{
	  if (ref_type (u) & V_USE)
	    follow_chain (imm_reaching_def (u), u);
	}
    }

  free (marked);

  analyze_rdefs ();

  /* Debugging dumps.  */
  dump_file = dump_begin (TDI_ssa, &dump_flags);
  if (dump_file)
    {
      if (dump_flags & TDF_RDEFS)
	dump_reaching_defs (dump_file);

      dump_end (TDI_ssa, dump_file);
    }
}


/* Analyze reaching definition information and warn about uses of
   potentially uninitialized variables if -Wuninitialized was given.  */

void
analyze_rdefs ()
{
  size_t i;

#if 0
  if (tree_warn_uninitialized == 0)
    return;
#else
  /* FIXME Disable these warnings for now.  Too many false positives.  */
  return;
#endif

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      struct ref_list_node *tmp;
      tree_ref use;

      /* Uninitialized warning messages are only given for local variables
	 with auto declarations.  */
      if (TREE_CODE (var) != VAR_DECL
	  || decl_function_context (var) == NULL
	  || TREE_STATIC (var)
	  || TREE_ADDRESSABLE (var))
	continue;

      /* For each use of VAR, if the use is reached by VAR's default
	 definition, then the variable may have been used uninitialized in
	 the function.  */
      FOR_EACH_REF (use, tmp, tree_refs (var))
	{
	  int found_default;
	  tree_ref def;
	  struct ref_list_node *tmp2;

	  if (!(ref_type (use) & V_USE))
	    continue;

	  /* Check all the reaching definitions looking for the default
	     definition.  */
	  found_default = 0;
	  FOR_EACH_REF (def, tmp2, reaching_defs (use))
	    {
	      if (ref_type (def) & (V_DEF | M_DEFAULT))
		found_default = 1;
	    }

	  /* If we found a default definition for VAR, then the reference may
	     be accessing an uninitialized variable.  If the default def is the
	     only reaching definition, then the variable _is_ used
	     uninitialized.  Otherwise it _may_ be used uninitialized.  */
	  if (found_default)
	    {
	      prep_stmt (ref_stmt (use));
	      if (reaching_defs (use)->last == reaching_defs (use)->first)
		warning ("`%s' is used uninitialized at this point",
		         IDENTIFIER_POINTER (DECL_NAME (var)));
	      else
		warning ("`%s' may be used uninitialized at this point",
		         IDENTIFIER_POINTER (DECL_NAME (var)));
	    }
	}
    }
}


/* Follow factored use-def chains to find all possible reaching definitions
   for U, starting with D.  This also updates reached uses for each
   reaching definition found.  */

static void
follow_chain (d, u)
     tree_ref d;
     tree_ref u;
{
  /* Do nothing if the definition doesn't exist.  */
  if (d == NULL)
    return;

#if defined ENABLE_CHECKING
  /* Consistency check.  D should be a definition or a PHI term.  */
  if (!(ref_type (d) & (V_DEF | V_PHI)))
    abort ();
#endif

  /* Do nothing if we've already visited this definition.  */
  if (marked[ref_id (d)] == u)
    return;

  marked[ref_id (d)] = u;

  /* If D is a definition for U, add it to the list of definitions reaching
     U.  Similarly, add U to the list of reached uses of D.  */
  if ((ref_type (d) & V_DEF) && ref_var (d) == ref_var (u))
    {
      add_ref_to_list_end (reaching_defs (u), d);
      add_ref_to_list_end (reached_uses (d), u);
    }

  /* If D is a PHI term, recursively follow each of its arguments.  */
  if (ref_type (d) & V_PHI)
    {
      size_t i;
      for (i = 0; i < num_phi_args (d); i++)
	follow_chain (phi_arg_def (phi_arg (d, i)), u);
    }

  /* If D is a non-killing definition of U (i.e., D is a may-def or a
     partial-def), follow D's def-def link (see explanation in
     search_fud_chains).  */
  else if (ref_type (d) & (V_DEF | M_PARTIAL | M_MAY))
    follow_chain (imm_reaching_def (d), u);
}


/* Return true if one or more uses of VAR in BB_SET have reaching definitions
   coming from blocks outside BB_SET.  If EXCLUDE_INIT_DECL is nonzero,
   the initializer expression used in the declaration of VAR will always
   be considered external to BB_SET.  */

bool
is_upward_exposed (var, bb_set, exclude_init_decl)
     tree var;
     sbitmap bb_set;
     int exclude_init_decl;
{
  struct ref_list_node *tmp;
  tree_ref r;

  FOR_EACH_REF (r, tmp, tree_refs (var))
    {
      /* If this is a use of the variable in one of the basic blocks that
	 we are interested in, check its reaching definitions.  */
      if ((ref_type (r) & V_USE) && TEST_BIT (bb_set, ref_bb (r)->index))
	{
	  tree_ref def;
	  struct ref_list_node *tmp2;

	  FOR_EACH_REF (def, tmp2, reaching_defs (r))
	    {
	      basic_block def_bb = ref_bb (def);

	      if ((ref_type (def) & (V_DEF | M_DEFAULT))
		  || (exclude_init_decl
		      && TREE_CODE (ref_stmt (def)) == DECL_STMT)
		  || ! TEST_BIT (bb_set, def_bb->index))
		return 1;
	    }
	}
    }

  return 0;
}


/* Remove the PHI alternative for the predecessor block BLOCK from
   PHI_NODE. 

   This routine assumes ordering of alternatives in the vector is
   not important and implements removal by swapping the last alternative
   with the alternative we want to delete, then shrinking the vector.  */

void
tree_ssa_remove_phi_alternative (phi_node, block)
     tree_ref phi_node;
     basic_block block;
{
  varray_type phi_vec = phi_args (phi_node);
  unsigned int num_elem = VARRAY_ACTIVE_SIZE (phi_vec);
  unsigned int i;

  for (i = 0; i < num_elem; i++)
    {
      tree_ref ref;
      basic_block src_bb;
      phi_node_arg arg;

      arg = phi_arg (phi_node, i);
      ref = phi_arg_def (arg);
      src_bb = phi_arg_edge (arg)->src;

      if (src_bb == block)
	{
	  /* If we are not at the last element, switch the last element
	     with the element we want to delete.  */
	  if (i != num_elem - 1)
	    set_phi_arg (phi_node, i, phi_arg (phi_node, num_elem - 1));

	  /* Shrink the vector.  */
	  VARRAY_ACTIVE_SIZE (phi_vec) -= 1;
	}
    }
}


/* Dump reaching definitions for all the definitions in the function.  */

void
dump_reaching_defs (dump_file)
     FILE *dump_file;
{
  size_t i;

  fprintf (dump_file, ";; Function %s\n\n",
      IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

  fprintf (dump_file, "Reaching definitions:\n");

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      struct ref_list_node *tmp;
      tree_ref u;

      fprintf (dump_file, "Variable: ");
      print_c_node (dump_file, var);
      fprintf (dump_file, "\n");

      FOR_EACH_REF (u, tmp, tree_refs (var))
	{
	  if (ref_type (u) & V_USE)
	    {
	      dump_ref (dump_file, "", u, 4, 0);
	      dump_ref_list (dump_file, "", reaching_defs (u), 6, 0);
	      fprintf (dump_file, "\n");
	    }
	}
      fprintf (dump_file, "\n");
    }
}


/* Dump reaching definitions on stderr.  */

void
debug_reaching_defs ()
{
  dump_reaching_defs (stderr);
}


/* Dump SSA information to DUMP_FILE.  */

void
dump_tree_ssa (dump_file)
     FILE *dump_file;
{
  basic_block bb;

  fputc ('\n', dump_file);
  fprintf (dump_file, ";; Function %s\n\n",
      IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

  fputs ("SSA information\n\n", dump_file);

  FOR_EACH_BB (bb)
    {
      tree_dump_bb (dump_file, "", bb, 0);
      dump_ref_list (dump_file, "    ", bb_refs (bb), 0, 1);
      fputs ("\n\n", dump_file);
    }
}


/* Dump SSA information to stderr.  */

void
debug_tree_ssa ()
{
  dump_tree_ssa (stderr);
}
