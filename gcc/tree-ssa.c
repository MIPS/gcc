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
#include "function.h"
#include "diagnostic.h"
#include "bitmap.h"
#include "tree-flow.h"
#include "tree-simple.h"
#include "tree-inline.h"
#include "ssa.h"
#include "varray.h"

/* This file builds the SSA form for a function using Factored
   Use-Def chains (FUD chains) as described in 

   Wolfe, M. J., High Performance Compilers for Parallel Computing,
   Addison-Wesley, 1996.

   This is the same algorithm described in 

   R. Cytron, J. Ferrante, B. Rosen, M. Wegman, and K. Zadeck.
   Efficiently computing static single assignment form and the control
   dependence graph. ACM Transactions on Programming Languages and Systems,
   13(4):451-490, October 1991.

   Except that instead of re-writing the program into SSA form, we build a
   web of pointers between variable uses and their immediately reaching
   definitions.

   Partial references
   ------------------

   To deal with arrays and structures, we use the concept of non-killing
   definitions.  When an array location or structure element is defined,
   the SSA builder will consider that a partial definition of the array
   itself.  Partial definitions are modeled by keeping an SSA link
   (def-def link) between the current definition and the previous one.

   This is used to chain definitions to arrays and structures, so that all
   possible reaching defs can be found later by compute_tree_rdefs.  For
   instance,

	      1  A[i] = 5;
	      2  A[j] = 10;
	      3  y = A[k] + 10;

   Since the assignments to A[i] at line 1 and A[j] at line 2 are partial
   definitions of A, without def-def chains the use of A[k] at line 3 will
   not be reached by the defintion at line 1.


   Modeling may-alias information
   ------------------------------

   May-aliases are discovered by compute_may_aliases.  Currently, the set
   computed is conservatively large because it uses a type-based,
   flow-insensitive method.  For each variable V, we compute all the
   may-aliases for V and store them in tree_annotation(V)->may_aliases.

   When we build the SSA form for the program, a definition for V will be
   considered a regular killing definition for V, but will be considered a
   non-killing definition for V's aliases.  However, contrary to the case
   with arrays, we don't keep a single def-def link between this definition
   and the previous one.  We keep N def-def links at each definition site,
   one for each alias of V.  For instance, assume that in the following
   fragment *p may-alias a and b:

	    1	a = 2;  <-------------------------+
	    2	                                  |
	    3	b = 5;  <----------------------+  |
	    4	                               |  |
	    5	*p = 93;    ALIAS_IMM_RDEF = { +, + }
	    6	 ^------+
	    7	 |      |
	    8	 +--+   |
	    9	    |   |
	    10	c = a * b;

   At the definition site for *p, we keep an array of reaching definitions
   for each alias of *p (ALIAS_IMM_RDEF).  We also make *p the current
   reaching definition for a and b.  This way, we maintain the program in
   SSA form and also keep track of all the definitions that may reach a and
   b.

   When computing reaching definitions, since *p is not a killing
   definition for either a nor b, follow_chain will examine the
   ALIAS_IMM_RDEF array at the definition for *p and find the other
   definitions.  Giving the expected result that at line 10, a can be
   reached by '*p = 93' and by 'a = 2'.

   Notice that this array of def-def links for aliases is kept at *every*
   definition site.  If the may-alias information is too conservative,
   these arrays may be very large, causing severe memory problems.

   To address this problem, we have a heuristic aggregation mechanism that
   after a certain number of may-aliases, will give up and create a single
   virtual variable that represents all the variables in the may-alias set.
   After this, every reference to any variable in the may-alias set will be
   considered a reference to this one virtual variable.  This is,
   effectively, the same mechanism used to deal with array references.

   
   Modeling call-clobbered variables
   ---------------------------------

   Variables that might be call clobbered are handled by inserting a use
   and a clobbering definition of an artificial global variable call
   GLOBAL_VAR.  Every alias that may be call clobbered is added to the
   may-alias set of GLOBAL_VAR.  The aliasing mechanism built into SSA will
   naturally create def-use and use-def links at call sites for the
   affected variables.  */


/* Nonzero to warn about variables used before they are initialized.  Used
   by analyze_rdefs().  */
int tree_warn_uninitialized = 0;

/* Array of saved definition chains.  Used by search_fud_chains to restore
   the previous definition chain when returning from a recursive call.  */
static tree_ref *save_chain;

/* Visited marks.  Used when computing reaching definitions (follow_chain).  */
static tree_ref *marked;

/* Dump file and flags.  */
FILE *tree_ssa_dump_file;
int tree_ssa_dump_flags;

/* Arrays used to keep track of where to insert PHI nodes for variables
   definitions (see insert_phi_nodes).  */
static GTY (()) varray_type added = NULL;
static GTY (()) varray_type in_work = NULL;
static GTY (()) varray_type work_stack = NULL;

/* Counters updated every time we allocate a new object.  Used to compare
   against the counts collected by collect_dfa_stats.  */
extern struct dfa_counts_d dfa_counts;

/* Local functions.  */
static void insert_phi_nodes		PARAMS ((sbitmap *));
static void build_fud_chains		PARAMS ((dominance_info));
static void search_fud_chains		PARAMS ((basic_block, dominance_info));
static void follow_chain		PARAMS ((tree_ref, tree_ref));
static void insert_phi_nodes_for	PARAMS ((tree, sbitmap *));
static void add_phi_node 		PARAMS ((basic_block, tree));
static void set_ssa_links		PARAMS ((tree_ref, tree, int));
static void set_alias_imm_reaching_def	PARAMS ((tree_ref, size_t, tree_ref));
static tree_ref create_default_def	PARAMS ((tree));
static void init_tree_ssa		PARAMS ((void));
static tree remove_annotations_r	PARAMS ((tree *, int *, void *));


/* Main entry point to the SSA builder.  */

void
build_tree_ssa (fndecl)
     tree fndecl;
{
  sbitmap *dfs;
  dominance_info idom;
  
  /* Initialize DFA structures.  */
  init_tree_ssa ();

  /* Debugging dumps.  */
  tree_ssa_dump_file = dump_begin (TDI_ssa, &tree_ssa_dump_flags);
  if (tree_ssa_dump_file)
    fprintf (tree_ssa_dump_file, "\nFunction %s\n\n", get_name (fndecl));

  /* Find variable references.  */
  find_tree_refs ();

  /* Compute immediate dominators.  */
  idom = calculate_dominance_info (CDI_DOMINATORS);

  /* Compute dominance frontiers.  */
  dfs = sbitmap_vector_alloc (last_basic_block, last_basic_block);
  compute_dominance_frontiers (dfs, idom);

  /* Insert the PHI nodes and build FUD chains.  */
  insert_phi_nodes (dfs);
  build_fud_chains (idom);

  sbitmap_vector_free (dfs);
  free_dominance_info (idom);

  /* Compute reaching definitions.  */
  compute_tree_rdefs ();    

  if (tree_ssa_dump_file)
    {
      /* FIXME  Default dump should be the pretty-printed function with SSA
	 indices.  */
      if (tree_ssa_dump_flags & (TDF_DETAILS))
	{
	  dump_referenced_vars (tree_ssa_dump_file);
	  dump_tree_ssa (tree_ssa_dump_file);
	  dump_reaching_defs (tree_ssa_dump_file);
	}

      if (tree_ssa_dump_flags & TDF_STATS)
	dump_dfa_stats (tree_ssa_dump_file);

      dump_end (TDI_ssa, tree_ssa_dump_file);
      tree_ssa_dump_file = NULL;
    }
}


/* Insert PHI nodes at the dominance frontier of nodes with variable
   definitions.  */

static void
insert_phi_nodes (dfs)
     sbitmap *dfs;
{
  size_t i;

  /* Array ADDED (indexed by basic block number) is used to determine
     whether a PHI node for the current variable has already been
     inserted at block X.  */
  VARRAY_TREE_INIT (added, last_basic_block, "added");

  /* Array IN_WORK (indexed by basic block number) is used to determine
     whether block X has already been added to WORK_STACK for the current
     variable.  */
  VARRAY_TREE_INIT (in_work, last_basic_block, "in_work");

  /* Array WORK_STACK is a stack of CFG blocks.  Each block that contains
     an assignment or PHI node will be pushed to this stack.  */
  VARRAY_BB_INIT (work_stack, last_basic_block, "work_stack");

  /* Iterate over all referenced variables in the function.  For each
     variable, add to the work list all the blocks that have a definition
     for the variable.  PHI nodes will be added to the dominance frontier
     blocks of each definition block.  */
  for (i = 0; i < num_referenced_vars; i++)
    insert_phi_nodes_for (referenced_var (i), dfs);

  added = NULL;
  in_work = NULL;
  work_stack = NULL;
}


/* Build FUD (Factored Use-Def) chains.  This links every V_USE reference
   of every variable to its immediate reaching V_DEF or V_PHI and fills in
   the arguments for all the PHI nodes placed by insert_phi_nodes.

   The algorithm works by doing a depth-first walk of the dominator tree.
   Every time a V_DEF or V_PHI reference is found, it is marked as the
   current definition of the associated variable VAR.  It also marks it as
   the current definition of every variable that VAR might be aliasing.

   When a V_USE reference for VAR is found, the current definition of VAR
   is retrieved and a use-def link is created between the use and that
   definition.

   The algorithm also creates def-use links and def-def links similarly.
   The def-def links are used to model "partial" and "may" definitions:
   
   - A partial definition is used in the case of arrays and structures.
     Whenever the an element of the array is defined, the DFA pass creates
     a V_DEF/partial reference for the array.  So, when computing reaching
     definitions, every use of the array will be reached by all the
     definitions made to individual elements.  For instance,

     		A[i] = foo();
		A[j] = bar();
		...
		x = A[k];

     Lacking dependency information, we have to assume that A[k] might be
     reached by both A[i] and A[j].  The FUD chaining algorithm will place
     a def-def link between A[i] and A[j].

   - A "may" definition is introduced in the presence of aliasing.  When a
     variable VAR is defined, that definition is also considered a
     potential definition of all the variables that might be aliased by
     VAR.  In this case the compiler keeps a def-def link for every
     variable that VAR might be aliasing.  */

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
  memset ((void *) save_chain, 0, next_tree_ref_id * sizeof (tree_ref));

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
  ref_list_iterator i;

  /* Chain every V_USE reference in the block to its immediate reaching
     definition.  Update the current definition of a variable when V_DEF
     references are found.  */
  for (i = rli_start (bb_refs (bb)); !rli_after_end (i); rli_step (&i))
    {
      tree_ref ref = rli_ref (i);
      tree var = ref_var (ref);

      if (ref_type (ref) != V_DEF
	  && ref_type (ref) != V_USE
	  && ref_type (ref) != V_PHI)
	continue;

      /* For V_USE references and non-killing definitions, create a default
	 definition if CURRDEF doesn't exist.  */
      if (ref_type (ref) == V_USE || is_partial_def (ref))
	{
	  tree_ref currdef = currdef_for (var);
	  if (currdef == NULL)
	    currdef = create_default_def (var);
	}

      /* Set up def-use, use-def and def-def links between REF and the
	 current definition of VAR.  */
      set_ssa_links (ref, var, -1);

      /* Ditto for any variables that might be aliased by VAR.  Notice that
	 we don't process aliases when looking at V_USE references.  Each
	 alias sets its own def-use and use-def links.  We only need to
	 process aliases when setting def-def links.  */
      if (ref_type (ref) != V_USE)
	{
	  size_t i;
	  for (i = 0; i < num_may_alias (var); i++)
	    set_ssa_links (ref, may_alias (var, i), i);
	}
    }


  /* Visit all the successor blocks of BB looking for PHI nodes.  For every
     PHI node found, add a new argument containing the current reaching
     definition for the variable and the edge through which that definition
     is reaching the PHI node.  */
  for (e = bb->succ; e; e = e->succ_next)
    {
      ref_list_iterator i;

      for (i = rli_start (bb_refs (e->dest)); !rli_after_end (i); rli_step (&i))
	{
	  tree_ref phi = rli_ref (i);
	  if (ref_type (phi) == V_PHI)
	    {
	      tree_ref currdef = currdef_for (ref_var (phi));
	      if (currdef == NULL)
		currdef = create_default_def (ref_var (phi));

	      /* Besides storing the incoming definition CURRDEF, we also
		store E, which is the edge that we are receiving CURRDEF
		from.  */
	      add_phi_arg (phi, currdef, e);

	      /* Set a def-use edge between CURRDEF and this PHI node.  */
	      add_ref_to_list_end (imm_uses (currdef), phi);
	    }
	}
    }


  /* Recursively search the dominator children of BB.  */
  FOR_EACH_BB (child_bb)
    if (get_immediate_dominator (idom, child_bb)->index == bb->index)
      search_fud_chains (child_bb, idom);


  /* Restore the current reaching definition for each variable referenced
     in the block (in reverse order).  */
  for (i = rli_start_last (bb_refs (bb)); !rli_after_end (i); rli_step_rev (&i))
    {
      tree_ref ref = rli_ref (i);

      if (ref_type (ref) == V_DEF || ref_type (ref) == V_PHI)
	{
	  size_t i;
	  unsigned long id = ref_id (ref);
	  tree var = ref_var (ref);

	  set_currdef_for (var, save_chain[id]);

	  /* Also restore CURRDEF for every alias of REF's variable.  */
	  for (i = 0; i < num_may_alias (var); i++)
	    set_currdef_for (may_alias (var, i), save_chain[id]);
	}
    }
}


/* Computes reaching definitions and reached uses for all the variables
   referenced in the current function.  */

void
compute_tree_rdefs ()
{
  size_t i;

  /* Initialize reaching definition and reached uses information for every
     reference in the function.  */
  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      ref_list_iterator j;

      for (j = rli_start (tree_refs (var)); !rli_after_end (j); rli_step (&j))
	{
	  tree_ref r = rli_ref (j);
	  if (ref_type (r) == V_USE)
	    empty_ref_list (reaching_defs (r));
	  else if (ref_type (r) == V_DEF || ref_type (r) == V_PHI)
	    empty_ref_list (reached_uses (r));
	}
    }

  /* Initialize the mark array to keep track of which definitions have been
     visited already.  */
  marked = (tree_ref *) xmalloc (next_tree_ref_id * sizeof (tree_ref));
  if (marked == NULL)
    abort ();
  memset ((void *) marked, 0, next_tree_ref_id * sizeof (tree_ref));

  /* Traverse all the uses following their use-def chains looking for
     reaching definitions and reached uses.  */
  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      ref_list_iterator j;

      for (j = rli_start (tree_refs (var)); !rli_after_end (j); rli_step (&j))
	{
	  tree_ref u = rli_ref (j);
	  if (ref_type (u) == V_USE)
	    follow_chain (imm_reaching_def (u), u);
	}
    }

  free (marked);

  analyze_rdefs ();
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
      ref_list_iterator j;

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
      for (j = rli_start (tree_refs (var)); !rli_after_end (j); rli_step (&j))
	{
	  tree_ref use = rli_ref (j);
	  int found_default;
	  ref_list_iterator k;

	  if (ref_type (use) != V_USE)
	    continue;

	  /* Check all the reaching definitions looking for the default
	     definition.  */
	  found_default = 0;
	  k = rli_start (reaching_defs (use));
	  for (; !rli_after_end (k); rli_step (&k))
	    if (is_default_def (rli_ref (k)))
	      found_default = 1;

	  /* If we found a default definition for VAR, then the reference may
	     be accessing an uninitialized variable.  If the default def is the
	     only reaching definition, then the variable _is_ used
	     uninitialized.  Otherwise it _may_ be used uninitialized.  */
	  if (found_default)
	    {
	      if (reaching_defs (use)->last == reaching_defs (use)->first)
		warning ("`%s' is used uninitialized at this point",
		         get_name (var));
	      else
		warning ("`%s' may be used uninitialized at this point",
		         get_name (var));
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
  tree d_var, u_var;

  /* Do nothing if the definition doesn't exist.  */
  if (d == NULL)
    return;

#if defined ENABLE_CHECKING
  /* Consistency check.  D should be a definition or a PHI node.  */
  if (ref_type (d) != V_DEF && ref_type (d) != V_PHI)
    abort ();

  /* U should be a V_USE.  */
  if (ref_type (u) != V_USE)
    abort ();
#endif

  /* Do nothing if we've already visited this definition.  */
  if (marked[ref_id (d)] == u)
    return;

  marked[ref_id (d)] = u;

  d_var = ref_var (d);
  u_var = ref_var (u);

  /* If D is a definition for U, add it to the list of definitions reaching
     U.  Similarly, add U to the list of reached uses of D.  We consider
     killing and non-killing definitions.  */
  if (ref_type (d) == V_DEF && ref_defines (d, u_var))
    {
      add_ref_to_list_end (reaching_defs (u), d);
      add_ref_to_list_end (reached_uses (d), u);
    }

  /* If D is a PHI node, recursively follow each of its arguments.  */
  if (ref_type (d) == V_PHI)
    {
      size_t i;
      for (i = 0; i < num_phi_args (d); i++)
	follow_chain (phi_arg_def (phi_arg (d, i)), u);
    }

  /* If D is a non-killing definition for U, follow D's def-def link.  */
  else if (is_partial_def (d))
    follow_chain (imm_reaching_def (d), u);

  /* If D is a may-def for U (i.e., D does not define U_VAR, but U_VAR is
     an alias of D_VAR), follow the def-def link in D's may_imm_rdefs that
     corresponds to U's variable.  */
  else
    {
      int i = get_alias_index (d_var, u_var);
      if (i >= 0)
	follow_chain (alias_imm_reaching_def (d, i), u);
    }
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
dump_reaching_defs (file)
     FILE *file;
{
  size_t i;

  fprintf (file, "Reaching definitions for %s\n", current_function_name);

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      ref_list_iterator j;

      dump_variable (file, var);

      for (j = rli_start (tree_refs (var)); !rli_after_end (j); rli_step (&j))
	{
	  tree_ref u = rli_ref (j);
	  if (ref_type (u) == V_USE)
	    {
	      dump_ref (file, "", u, 4, 0);
	      dump_ref_list (file, "", reaching_defs (u), 6, 0);
	      fprintf (file, "\n");
	    }
	}
      fprintf (file, "\n");
    }
}


/* Dump reaching definitions on stderr.  */

void
debug_reaching_defs ()
{
  dump_reaching_defs (stderr);
}


/* Dump SSA information to FILE.  */

void
dump_tree_ssa (file)
     FILE *file;
{
  basic_block bb;

  fprintf (file, "SSA information for %s\n\n", current_function_name);

  FOR_EACH_BB (bb)
    {
      dump_tree_bb (file, "", bb, 0);
      dump_ref_list (file, "    ", bb_refs (bb), 0, 1);
      fputs ("\n\n", file);
    }
}


/* Dump SSA information to stderr.  */

void
debug_tree_ssa ()
{
  dump_tree_ssa (stderr);
}


/*---------------------------------------------------------------------------
		  Helpers for the main SSA building functions
---------------------------------------------------------------------------*/

/* Insert PHI nodes for variable VAR.  */

static void
insert_phi_nodes_for (var, dfs)
     tree var;
     sbitmap *dfs;
{
  ref_list_iterator i;

#if defined ENABLE_CHECKING
  /* Variables in referenced_vars must have at least 1 reference.  */
  if (tree_refs (var)->first == NULL)
    abort ();
#endif

  /* Add to the worklist all the basic blocks that have definitions of
     VAR.  */
  for (i = rli_start (tree_refs (var)); !rli_after_end (i); rli_step (&i))
    {
      tree_ref ref = rli_ref (i);
      basic_block bb = ref_bb (ref);

      if (ref_type (ref) == V_DEF)
	{
	  VARRAY_PUSH_BB (work_stack, bb);
	  VARRAY_TREE (in_work, bb->index) = var;
	}
    }

  /* Insert PHI nodes at the dominance frontier of all the basic blocks
     in the worklist.  */
  while (VARRAY_ACTIVE_SIZE (work_stack) > 0)
    {
      size_t w;
      basic_block bb;

      bb = VARRAY_TOP_BB (work_stack);
      VARRAY_POP (work_stack);

      EXECUTE_IF_SET_IN_SBITMAP (dfs[bb->index], 0, w,
				 add_phi_node (BASIC_BLOCK (w), var));
    }
}


/* Add a new PHI node for variable VAR at the start of basic block BB.
   If BB didn't have a definition of BB, we add BB itself to the worklist
   because the PHI node introduces a new definition of VAR.  */

static void
add_phi_node (bb, var)
     basic_block bb;
     tree var;
{
  if (VARRAY_TREE (added, bb->index) != var)
    {
      tree_ref phi;
      tree *stmt_p;

      /* If the basic block is not empty, associate the PHI node to its
	 first executable statement.  This is for debugging convenience.
	 This way, the PHI node will have line number information.  */
      stmt_p = !bb_empty_p (bb) ? bb->head_tree_p : NULL;
      phi = create_ref (var, V_PHI, 0, bb, stmt_p, 0);
      add_ref_to_list_begin (bb_refs (bb), phi);

      VARRAY_TREE (added, bb->index) = var;

      /* Basic block BB now has a new definition of VAR.  If BB wasn't in
	 the worklist already, add it.  */
      if (VARRAY_TREE (in_work, bb->index) != var)
	{
	  VARRAY_PUSH_BB (work_stack, bb);
	  VARRAY_TREE (in_work, bb->index) = var;
	}
    }
}


/* Set up use-def, def-use and def-def links between reference REF and the
   current reaching definition for VAR.
   
   If ALIAS_IX is nonnegative, then VAR is an alias of REF.  In which case,
   ALIAS_IX is the index into the array of immediate reaching definitions
   for aliases (i.e. alias_imm_reaching_def(REF, ALIAS_IX) is the immediate
   reaching definition for VAR).  */

static void
set_ssa_links (ref, var, alias_ix)
     tree_ref ref;
     tree var;
     int alias_ix;
{
  tree_ref currdef;

  /* Retrieve the current definition for the variable.  */
  currdef = currdef_for (var);

  if (ref_type (ref) == V_USE)
    {
#if defined ENABLE_CHECKING
      /* We should not set def-use/use-def edges for aliases, only def-def
	 edges.  Each alias will naturally get a def-use/use-def edge to a
	 most recent definition of either itself or an alias.  */
      if (currdef == NULL || alias_ix != -1)
	abort ();
#endif

      /* Set up a def-use chain between CURRDEF (the immediately
	reaching definition for REF) and REF.  Each definition may
	have more than one immediate use, so make sure that we don't
	add REF to CURRDEF's immediate uses more than once.  */
      if (imm_reaching_def (ref) != currdef)
	add_ref_to_list_end (imm_uses (currdef), ref);

      /* Set up a use-def chain between REF and CURRDEF.  */
      set_imm_reaching_def (ref, currdef);
    }
  else if (ref_type (ref) == V_DEF || ref_type (ref) == V_PHI)
    {
      /* Save the current definition chain for the variable.  This is used
	 when visiting the dominator children of a block in
	 search_fud_chains.  Once all the dominator children of block BB
	 have been visited, we need to restore CURRDEF for all the
	 variables referenced in BB.  */
      save_chain[ref_id (ref)] = currdef;

      /* Set a def-def link for partial definitions.  */
      if (is_partial_ref (ref))
	set_imm_reaching_def (ref, currdef);

      /* Similarly, if VAR is one of the aliases for REF's variable then
	 this definition is a non-killing definition for VAR.  Set up in
	 position ALIAS_IX of the ALIAS_IMM_RDEFS array a def-def link
         between this definition and VAR's current reaching definition.  */
      if (alias_ix >= 0)
	set_alias_imm_reaching_def (ref, alias_ix, currdef);

      /* Finally, set REF to be the new CURRDEF for VAR.  */
      set_currdef_for (var, ref);
    }
}


/* Set MAY_RDEF to be the immediate reaching definition for the Ith alias
   of REF.  */

static void
set_alias_imm_reaching_def (ref, i, may_rdef)
     tree_ref ref;
     size_t i;
     tree_ref may_rdef;
{
#if defined ENABLE_CHECKING
  if (i > num_may_alias (ref_var (ref)))
    abort ();
#endif

  /* Create the array the first time.  */
  if (ref->vref.alias_imm_rdefs == NULL)
    {
      tree var = ref_var (ref);
      size_t num = num_may_alias (var);
      size_t size = sizeof (tree_ref *) * num;

      ref->vref.alias_imm_rdefs = (tree_ref *) ggc_alloc (size);
      memset ((void *) ref->vref.alias_imm_rdefs, 0, size);

      dfa_counts.num_alias_imm_rdefs += num;

      /* Adjust the may-alias array for the variable.  */
      VARRAY_GROW (tree_annotation (var)->may_aliases, num);
    }

  ref->vref.alias_imm_rdefs[i] = may_rdef;
}


/* Create a default definition for VAR.  This is called when the SSA
   builder needs to get the current reaching definition for a PHI node or a
   V_USE reference and finds it to be NULL.  In the case of a V_USE
   reference, this means that the variable may be used uninitialized.

   If the variable is static and DECL_INITIAL is set, then an initializing
   definition is created.  This makes a difference when doing constant
   propagation.  If we are initializing a read-only static variable, then
   we can assume that DECL_INITIAL will be the constant value for the
   variable.  */

static tree_ref
create_default_def (var)
     tree var;
{
  basic_block decl_bb;
  tree_ref def;
  size_t i;
  unsigned mod;

  decl_bb = ENTRY_BLOCK_PTR->succ->dest;

  if (TREE_STATIC (var) && DECL_INITIAL (var))
    mod = TRM_INITIAL;
  else
    mod = TRM_DEFAULT;

  /* Create a default definition and set it to be CURRDEF(var).  */
  def = create_ref (var, V_DEF, mod, decl_bb, NULL, 0);
  add_ref_to_list_begin (bb_refs (decl_bb), def);
  set_currdef_for (var, def);

  /* Reallocate the SAVE_CHAIN array to accomodate the new reference.  */
  save_chain = (tree_ref *) xrealloc (save_chain, 
                                      next_tree_ref_id * sizeof (tree_ref));
  save_chain[ref_id (def)] = NULL;

  /* Set SSA links for the new definition.  */
  set_ssa_links (def, var, -1);
  for (i = 0; i < num_may_alias (var); i++)
    set_ssa_links (def, may_alias (var, i), i);

  return def;
}


/* Initialize DFA/SSA structures.  */

static void
init_tree_ssa ()
{
  next_tree_ref_id = 0;
  memset ((void *) &dfa_counts, 0, sizeof (struct dfa_counts_d));
  
  num_referenced_vars = 0;
  VARRAY_TREE_INIT (referenced_vars, 20, "referenced_vars");

  /* Declare an artificial global variable to act as a representative of
     all the variables that may be clobbered by function calls.  */
  global_var = build_decl (VAR_DECL, get_identifier (".GLOBAL_VAR"),
                           ptr_type_node);
  DECL_ARTIFICIAL (global_var) = 1;
  TREE_READONLY (global_var) = 0;
  DECL_EXTERNAL (global_var) = 0;
  TREE_STATIC (global_var) = 0;
  TREE_USED (global_var) = 1;
  DECL_CONTEXT (global_var) = NULL_TREE;
  TREE_THIS_VOLATILE (global_var) = 1;
  TREE_ADDRESSABLE (global_var) = 1;
  set_indirect_var (global_var, create_indirect_ref (global_var));

  /* If -Wuninitialized was used, set tree_warn_uninitialized and clear
     warn_uninitialized to avoid duplicate warnings.  */
  if (warn_uninitialized == 1)
    {
      tree_warn_uninitialized = 1;
      warn_uninitialized = 0;
    }
}


/* Deallocate memory associated with SSA data structures.  */

void
delete_tree_ssa (fnbody)
     tree fnbody;
{
  unsigned long int i;

  /* Remove annotations from every tree in the function.  */
  walk_tree (&fnbody, remove_annotations_r, NULL, NULL);

  /* Remove annotations from every referenced variable.  */
  for (i = 0; i < num_referenced_vars; i++)
    referenced_var (i)->common.ann = NULL;

  num_referenced_vars = 0;
  referenced_vars = NULL;
  global_var = NULL_TREE;
}


/* Callback function for walk_tree to clear DFA/SSA annotations from
   node *TP.  */

static tree
remove_annotations_r (tp, walk_subtrees, data)
    tree *tp;
    int *walk_subtrees ATTRIBUTE_UNUSED;
    void *data ATTRIBUTE_UNUSED;
{
  (*tp)->common.ann = NULL;
  return NULL_TREE;
}
