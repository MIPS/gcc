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
#include "coretypes.h"
#include "tm.h"
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
#include "timevar.h"
#include "tree-alias-common.h"

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
   possible reaching defs can be found later by compute_reaching_defs.  For
   instance,

	      1  A[i] = 5;
	      2  A[j] = 10;
	      3  y = A[k] + 10;

   Since the assignments to A[i] at line 1 and A[j] at line 2 are partial
   definitions of A, without def-def chains the use of A[k] at line 3 will
   not be reached by the defintion at line 1.


   Modeling may-alias information
   ------------------------------

   May-aliases are discovered by compute_may_aliases.  By default, the set
   computed is too large because it is exclusively type-based.  However,
   points-to analysis (PTA) is also available using the flag
   -ftree-points-to={steen|andersen}.

   If a variable V may alias a set of variables, V becomes the "alias
   leader" for that set.  When setting up use-def and def-use links in
   set_ssa_links, we use the alias leader to represent all the variables in
   the same set.  So, when we find a definition for one of the variables in
   the set represented by V, we consider that definition to be a definition
   to V.  Similarly, a use reference to any variable in the same alias set
   will be considered a use of V.  To allow the discovery of reaching
   definitions for all the members in the set, we keep def-def links.

   For instance, suppose we have a pointer *p that aliases variables a and
   b.  compute_may_aliases will set *p to be the alias leader for a and b.

	    1	 a = 4		=> sets CURRDEF(*p)
	    2	 b = 3		=> sets CURRDEF(*p).  sets def-def link to #1
	    3	*p = 5		=> sets CURRDEF(*p).  sets def-def link to #2
	    4	 x = a

   Definitions at lines 1, 2 and 3, will be considered definitions to *p
   and set_ssa_links will also set a def-def link between the different
   assignments.

   The immediate reaching definition for the use of a at line 4 will be the
   assignment at line 5.  However, when computing reaching definitions, the
   assignment to b at line 2 will be skipped because a and b are not
   aliased to each other (see the call to ref_defines in follow_chain). 

   
   Modeling call-clobbered variables
   ---------------------------------

   Variables that might be call clobbered are handled by inserting a use
   and a clobbering definition of an artificial global variable call
   GLOBAL_VAR.  Every alias that may be call clobbered is added to the
   may-alias set of GLOBAL_VAR.  The aliasing mechanism built into SSA will
   naturally create def-use and use-def links at call sites for the
   affected variables.  */


/* Nonzero to warn about variables used before they are initialized.  */
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
static void set_ssa_links		PARAMS ((tree_ref, tree));
static void init_tree_ssa		PARAMS ((void));
static tree remove_annotations_r	PARAMS ((tree *, int *, void *));
static inline tree_ref currdef_for	PARAMS ((tree));
static inline void set_currdef_for	PARAMS ((tree, tree_ref));


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

  /* Compute immediate dominators and dominance frontiers.  */
  idom = calculate_dominance_info (CDI_DOMINATORS);
  dfs = sbitmap_vector_alloc (last_basic_block, last_basic_block);
  compute_dominance_frontiers (dfs, idom);

  /* Insert the PHI nodes and build FUD chains.  */
  insert_phi_nodes (dfs);
  build_fud_chains (idom);

  /* Free allocated memory.  */
  sbitmap_vector_free (dfs);
  free_dominance_info (idom);

  if (tree_ssa_dump_file)
    {
      /* FIXME  Default dump should be the pretty-printed function with SSA
	 indices.  */
      if (tree_ssa_dump_flags & (TDF_DETAILS))
	{
	  dump_referenced_vars (tree_ssa_dump_file, true);
	  dump_tree_ssa (tree_ssa_dump_file);
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

  timevar_push (TV_TREE_INSERT_PHI_NODES);

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

  timevar_pop (TV_TREE_INSERT_PHI_NODES);
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

  timevar_push (TV_TREE_BUILD_FUD_CHAINS);

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

  timevar_pop (TV_TREE_BUILD_FUD_CHAINS);
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

      /* Set up def-use, use-def and def-def links between REF and the
	 current definition of VAR.  */
      set_ssa_links (ref, var);
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

	      /* Besides storing the incoming definition CURRDEF, we also
		 store E, which is the edge that we are receiving CURRDEF
		 from.  */
	      add_phi_arg (phi, currdef, e);

	      /* If CURRDEF is not empty, set a def-use edge between CURRDEF
		 and this PHI node.  */
	      if (currdef)
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
	  unsigned long id = ref_id (ref);
	  tree var = ref_var (ref);
	  set_currdef_for (var, save_chain[id]);
	}
    }
}


/* Computes reaching definitions and reached uses for all the variables
   referenced in the current function.  */

void
compute_reaching_defs ()
{
  size_t i;

  timevar_push (TV_TREE_RDEFS);

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

  timevar_pop (TV_TREE_RDEFS);

  /* Debugging dumps.  */
  tree_ssa_dump_file = dump_begin (TDI_ssa, &tree_ssa_dump_flags);
  if (tree_ssa_dump_flags & (TDF_DETAILS))
    {
      fprintf (tree_ssa_dump_file, "\nFunction %s\n\n",
	       get_name (current_function_decl));
      dump_reaching_defs (tree_ssa_dump_file);
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
  else if (!is_killing_def (d, u_var))
    follow_chain (imm_reaching_def (d), u);
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
      basic_block src_bb;
      phi_node_arg arg;

      arg = phi_arg (phi_node, i);
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
	      dump_ref (file, "", u, 2, 0);
	      dump_ref_list (file, "", reaching_defs (u), 4, 0);
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
   current reaching definition for VAR.  */

static void
set_ssa_links (ref, var)
     tree_ref ref;
     tree var;
{
  tree_ref currdef = NULL;

  /* Retrieve the current definition for the variable.  Note that if the
     variable is aliased, this will retrieve the current definition for its
     alias leader (i.e., the variable representing all the variables in the
     same alias set).  */
  currdef = currdef_for (var);

  if (ref_type (ref) == V_USE)
    {
      bool found;
      tree_ref new_currdef;

      /* Make sure that CURRDEF is an actual definition for the variable.
	 In the presence of aliasing, all definitions to the variables in
	 the same alias set make a definition to their alias leader,
	 not to their variable.  This allows us to set def-def links
	 between definitions to members of the same alias set.

	 However, this does not mean that every definition to the alias
	 leader will necessarily reach this use.  For instance, assume that
	 '*p' is the alias leader for variables 'a' and 'b':

	 	*p = 5;
		 a = 3;
		   = b;

	 When processing the use of 'b' in the last line, CURRDEF is the
	 definition 'a = 3' (because 'a' and 'b' are in the same alias
	 set).   Since 'a' cannot alias 'b', we continue to the next
	 definition in the def-def chain until we find a definition for 'b'
	 or for a variable that can alias 'b'.  In this case, we find the
	 definition '*p = 5', which is what we use as CURRDEF.  */
      found = false;
      new_currdef = currdef;
      while (!found && new_currdef)
	{
	  if (ref_var (new_currdef) == var
	      || ref_var (new_currdef) == alias_leader (var)
	      || may_alias_p (ref_var (new_currdef), var))
	    found = true;
	  else
	    new_currdef = imm_reaching_def (new_currdef);
	}

      /* If we didn't find a matching definition, it means that no
	 definition in the def-def chain affects this use of VAR, so we can
	 just return.  */
      if (!found)
	return;

      currdef = new_currdef;

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

      /* Set a def-def link to the current definition.  This allows
	 compute_reaching_defs to find all the reaching definitions in the
	 presence of non-killing definitions.  It also makes it possible
	 for remove_ref to patch the SSA web when removing V_DEF
	 references.  Note that this is unnecessary when processing PHI
	 nodes.  The argument list of a PHI node represents all the def-def
	 links for the node.  */
      if (ref != currdef && ref_type (ref) != V_PHI)
	set_imm_reaching_def (ref, currdef);

      /* Finally, set REF to be the new CURRDEF for VAR.  If VAR is
	 aliased, then this becomes CURRDEF for the whole alias set of VAR.
	 In which case this will set CURRDEF for the alias leader.  */
      set_currdef_for (var, ref);
    }
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

  if (flag_tree_points_to != PTA_NONE && num_referenced_vars)
    {
      timevar_push (TV_TREE_PTA);
      delete_alias_vars ();
      timevar_pop (TV_TREE_PTA);
    }

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


/* Return the current definition for variable V.  If V is aliased, return
   the current definition for V's alias leader (i.e., the variable that
   represents the alias set to which V belongs).  */

static inline tree_ref
currdef_for (v)
     tree v;
{
  if (alias_leader (v))
    v = alias_leader (v);

  return tree_annotation (v) ? tree_annotation (v)->currdef : NULL;
}


/* Set DEF to be the current definition for variable V.  If V is aliased,
   set the current definition for V's alias leader (i.e., the variable that
   represents the alias set to which V belongs).  */

static inline void
set_currdef_for (v, def)
     tree v;
     tree_ref def;
{
  tree_ann ann;
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (v)) != 'd'
      && TREE_CODE (v) != INDIRECT_REF)
    abort ();

  if (def && ref_type (def) != V_DEF && ref_type (def) != V_PHI)
    abort ();
#endif

  if (alias_leader (v))
    v = alias_leader (v);

  ann = tree_annotation (v) ? tree_annotation (v) : create_tree_ann (v);
  ann->currdef = def;
}
