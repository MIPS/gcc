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
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "diagnostic.h"
#include "tree-optimize.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "ssa.h"
#include "varray.h"
/* This should be eventually be generalized to other languages, but
   this would require a shared function-as-trees infrastructure.  */
#include "c-common.h"
#include "bitmap.h"

/* Nonzero to warn about variables used before they are initialized.  Used
   by analyze_rdefs().  */

int tree_warn_uninitialized = 0;


/* Dump file and flags.  */

static FILE *dump_file;
static int dump_flags;


/* Local functions.  */

static void insert_phi_terms PARAMS ((sbitmap *));
static void build_fud_chains PARAMS ((dominance_info));
static void search_fud_chains PARAMS ((basic_block, dominance_info));
static void follow_chain PARAMS ((varref, varref));


/*  Build the SSA form for the given function. This implements Factored
    Use-Def Chains as described in 

    Wolfe, M. J., High Performance Compilers for Parallel Computing,
    Addison-Wesley, 1996.  */

void
tree_build_ssa ()
{
  sbitmap *dfs;
  dominance_info idom;
  size_t i;
  
  /* Compute immediate dominators.  */
  idom = calculate_dominance_info (CDI_DOMINATORS);

  /* Compute dominance frontiers.  */
  dfs = sbitmap_vector_alloc (last_basic_block, last_basic_block);
  compute_dominance_frontiers (dfs, idom);

  /* Insert default definitions (a.k.a. ghost definitions) for all the
     symbols referenced in the function.  This allows the identification of
     variables that have been used without a preceding definition.

     These definitions do not affect code generation, they are associated
     with no statement or expression (to distinguish them from actual
     definitions).  */
  for (i = 0; i < NREF_SYMBOLS; i++)
    create_ref (REF_SYMBOL (i), VARDEF, ENTRY_BLOCK_PTR, NULL, NULL);

  /* Insert the PHI terms and build FUD chains.  */
  insert_phi_terms (dfs);
  build_fud_chains (idom);

  sbitmap_vector_free (dfs);
  free_dominance_info (idom);

  /* Debugging dumps.  */
  dump_file = dump_begin (TDI_ssa, &dump_flags);

  if (dump_file)
    {
      basic_block bb;

      fputc ('\n', dump_file);
      fprintf (dump_file, ";; Function %s\n\n",
	       IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

      fputs ("SSA information\n\n", dump_file);

      FOR_EACH_BB (bb)
	{
	  tree_dump_bb (dump_file, "", bb, 0);
	  dump_varref_list (dump_file, "    ", BB_REFS (bb), 0, 1);
	  fputs ("\n\n", dump_file);
	}

      dump_end (TDI_ssa, dump_file);
    }
}


/*  Insert PHI terms at the dominance frontier of nodes with variable
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

  /* Iterate over all referenced symbols in the function.  For each
     symbol, add to the work list all the blocks that have a definition
     for the symbol.  PHI terms will be added to the dominance frontier
     blocks of each definition block.  */
  for (i = 0; i < NREF_SYMBOLS; i++)
    {

      tree sym = REF_SYMBOL (i);
      struct ref_list_node *tmp;
      varref ref;

      /* Symbols in referenced_symbols must have at least 1 reference.  */
      if (TREE_REFS (sym)->first == NULL)
	abort ();

      FOR_EACH_REF (ref, tmp, TREE_REFS (sym))
	{
	  basic_block bb = VARREF_BB (ref);

	  /* Ignore ghost definitions in ENTRY_BLOCK_PTR.  */
	  if (VARREF_TYPE (ref) != VARDEF || IS_GHOST_DEF (ref))
	    continue;

	  VARRAY_PUSH_BB (work_stack, bb);
	  VARRAY_TREE (in_work, bb->index) = sym;
	}

      while (VARRAY_ACTIVE_SIZE (work_stack) > 0)
	{
	  size_t w;
	  basic_block bb;

	  bb = VARRAY_TOP_BB (work_stack);
	  VARRAY_POP (work_stack);

	  EXECUTE_IF_SET_IN_SBITMAP (dfs[bb->index], 0, w,
	    {
	      if (VARRAY_TREE (added, w) != sym)
		{
		  varref phi;
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
		    stmt_bb = BB_PARENT (stmt_bb);

		  if (stmt_bb == NULL)
		    abort ();

		  phi = create_ref (sym, VARPHI, bb, stmt_bb->head_tree,
		                       NULL);
		  VARRAY_TREE (added, w) = sym;

		  if (VARRAY_TREE (in_work, w) != sym)
		    {
		      VARRAY_PUSH_BB (work_stack, bb);
		      VARRAY_TREE (in_work, w) = sym;
		    }
		}
	    });
	}
    }
}


/*  Build FUD (Factored Use-Def) chains.  */

static void
build_fud_chains (idom)
     dominance_info idom;
{
  size_t i;

  /* Initialize the current definition for all the symbols.  */
  for (i = 0; i < NREF_SYMBOLS; i++)
    get_tree_ann (REF_SYMBOL (i))->currdef = NULL;

  /* Search FUD chains starting with the entry block.  */
  search_fud_chains (ENTRY_BLOCK_PTR, idom);
}


/*  Perform a depth-first traversal of the dominator tree looking for FUD
    chains.  */

static void
search_fud_chains (bb, idom)
     basic_block bb;
     dominance_info idom;
{

  edge e;
  basic_block child_bb;
  struct ref_list_node *tmp;
  varref ref;

  /* for each variable use or def or phi-term R in BB do
         let SYM be the variable referenced at R
         if R is a use then
             Chain(R) = CurrDef(SYM)
         else if R is a def or $\phi$-term then
             SaveChain(R) = CurrDef(SYM)
             CurrDef(SYM) = R
         endif
     endfor  */
  if (BB_REFS (bb))
    FOR_EACH_REF (ref, tmp, BB_REFS (bb))
    {
      varref currdef;
      tree sym = VARREF_SYM (ref);

      /* Retrieve the current definition for the variable.  */
      currdef = TREE_CURRDEF (sym);

      if (VARREF_TYPE (ref) == VARUSE)
	{
	  VARUSE_CHAIN (ref) = currdef;

	  /* Besides setting a link back to our immediate reaching
	     definition, we want to link that definition to its immediate
	     use.

	     If this use (ref) has a current definition (currdef), add
	     'ref' to the list of uses immediately reached by 'currdef'.  */
	  if (currdef)
	    add_ref_to_list_end (VARDEF_IMM_USES (currdef), ref);
	}
      else if (VARREF_TYPE (ref) == VARDEF || VARREF_TYPE (ref) == VARPHI)
	{
	  VARDEF_SAVE_CHAIN (ref) = currdef;

	  /* Replace the current reaching definition with a new one.  */
	  get_tree_ann (sym)->currdef = ref;
	}
    }


  /* for Y in SUCC(BB) do
        J = WhichPred(BB -> Y)
        for each phi-term R in Y do
            let SYM be the variable referenced at R
            phi-chain(R)[J] = CurrDef(SYM)
        endfor
     endfor  */
  for (e = bb->succ; e; e = e->succ_next)
    {
      
      basic_block y;
      varref phi;

      y = e->dest;
      if (BB_REFS (y) == NULL)
	continue;

      FOR_EACH_REF (phi, tmp, BB_REFS (y))
	{
	  tree sym;
	  varref currdef;
	

	  if (VARREF_TYPE (phi) != VARPHI)
	    continue;

	  sym = VARREF_SYM (phi);
	  currdef = TREE_CURRDEF (sym);

	  if (currdef)
	    {
	      VARRAY_PUSH_GENERIC_PTR (VARDEF_PHI_CHAIN (phi), currdef);
	      VARRAY_PUSH_BB (VARDEF_PHI_CHAIN_BB (phi), bb);
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
         let SYM be the variable referenced at R
         if R is a def or a phi-term then
             CurrDef(SYM) = SaveChain(R)
         endif
     endfor  */
  FOR_EACH_REF_REV (ref, tmp, BB_REFS (bb))
    {

      if (VARREF_TYPE (ref) == VARDEF || VARREF_TYPE (ref) == VARPHI)
	{
	  tree sym = VARREF_SYM (ref);
	  tree_ann ann;

	  /* Restore the current definition for the variable.  */
	  ann = TREE_ANN (sym);
	  if (ann == NULL)
	    abort ();

	  ann->currdef = VARDEF_SAVE_CHAIN (ref);
	}
    }
}


/*  Deallocate memory associated with SSA data structures.  */

void
delete_ssa ()
{
  size_t i;

  for (i = 0; i < NREF_SYMBOLS; i++)
    delete_ref_list (TREE_REFS (REF_SYMBOL (i)));
}


/* Reaching definitions.  */

/*  Computes reaching definitions and reached uses for all the variables
    referenced in the current function.  */

void
tree_compute_rdefs ()
{
  size_t i;

  /* Initialize reaching definition and reached uses information for every
     reference in the function.  */
  for (i = 0; i < NREF_SYMBOLS; i++)
    {
      tree sym = REF_SYMBOL (i);
      varref r;
      struct ref_list_node *tmp;

      FOR_EACH_REF (r, tmp, TREE_REFS (sym))
	{
	  if (VARREF_TYPE (r) == VARUSE)
	    empty_ref_list (VARUSE_RDEFS (r));
	  else if (VARREF_TYPE (r) == VARDEF)
	    empty_ref_list (VARDEF_RUSES (r));
	}
    }

  /* Traverse all the uses following their use-def chains looking for
     reaching definitions and reached uses.  */
  for (i = 0; i < NREF_SYMBOLS; i++)
    {
      tree sym = REF_SYMBOL (i);
      varref u;
      struct ref_list_node *tmp;

      FOR_EACH_REF (u, tmp, TREE_REFS (sym))
	{
	  if (VARREF_TYPE (u) == VARUSE)
	    follow_chain (VARUSE_CHAIN (u), u);
	}
    }


  analyze_rdefs ();

  /* Debugging dumps.  */
  dump_file = dump_begin (TDI_ssa, &dump_flags);

  if (dump_file && (dump_flags & TDF_RDEFS))
    {
      fprintf (dump_file, ";; Function %s\n\n",
	       IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

      fprintf (dump_file, "Reaching definitions:\n");

      for (i = 0; i < NREF_SYMBOLS; i++)
	{
	  tree sym = REF_SYMBOL (i);
	  struct ref_list_node *tmp;
	  varref u;
	  
	  fprintf (dump_file, "Symbol: ");
	  print_node_brief (dump_file, "", sym, 0);
	  fprintf (dump_file, "\n");

	  FOR_EACH_REF (u, tmp, TREE_REFS (sym))
	    {
	      if (VARREF_TYPE (u) == VARUSE)
		{
		  dump_varref (dump_file, "", u, 4, 0);
		  dump_varref_list (dump_file, "", VARUSE_RDEFS (u), 6, 0);
		  fprintf (dump_file, "\n");
		}
	    }
	  fprintf (dump_file, "\n");
	}

      dump_end (TDI_ssa, dump_file);
    }
}


/*  Analyze reaching definition information and warn about uses of
    potentially uninitialized variables if -Wuninitialized was given.  */

void
analyze_rdefs ()
{
  size_t i;

  if (tree_warn_uninitialized == 0)
    return;

  for (i = 0; i < NREF_SYMBOLS; i++)
    {
      tree sym = REF_SYMBOL (i);
      struct ref_list_node *tmp;
      varref use;

      /* Uninitialized warning messages are only given for local variables
	 with auto declarations.  */
      if (TREE_CODE (sym) != VAR_DECL
	  || DECL_CONTEXT (sym) == NULL
	  || TREE_STATIC (sym)
	  || TREE_ADDRESSABLE (sym))
	continue;

      /* For each use of SYM, if the use is reached by SYM's ghost
	 definition, then the symbol may have been used uninitialized in
	 the function.  */
      FOR_EACH_REF (use, tmp, TREE_REFS (sym))
	{
	  int found_ghost;
	  varref def;
	  struct ref_list_node *tmp2;

	  if (VARREF_TYPE (use) != VARUSE)
	    continue;

	  /* Check all the reaching definitions looking for the ghost
	     definition.  */
	  found_ghost = 0;
	  FOR_EACH_REF (def, tmp2, VARUSE_RDEFS (use))
	    {
	      if (IS_GHOST_DEF (def))
		found_ghost = 1;
	    }

	  /* If we found a ghost definition for SYM, then the reference may
	     be accessing an uninitialized symbol.  If the ghost def is the
	     only reaching definition, then the symbol _is_ used
	     uninitialized.  Otherwise it _may_ be used uninitialized.  */
	  if (found_ghost)
	    {
	      prep_stmt (VARREF_STMT (use));
	      if (VARUSE_RDEFS (use)->last == VARUSE_RDEFS (use)->first)
		warning ("`%s' is used uninitialized at this point",
		         IDENTIFIER_POINTER (DECL_NAME (sym)));
	      else
		warning ("`%s' may be used uninitialized at this point",
		         IDENTIFIER_POINTER (DECL_NAME (sym)));
	    }
	}
    }
}


/*  Follows the factored use-def links to find all possible reaching
    definitions for U, starting with D.  This also updates reached uses for
    each reaching definition found.  */

static void
follow_chain (d, u)
     varref d;
     varref u;
{
  /* Do nothing if the definition doesn't exist.  */
  if (d == NULL)
    return;

  /* Consistency check.  D should be a definition or a PHI term.  */
  if (VARREF_TYPE (d) != VARDEF && VARREF_TYPE (d) != VARPHI)
    abort ();

  /* Do nothing if we've already visited this definition.  */
  if (VARDEF_MARKED (d) == u)
    return;

  VARDEF_MARKED (d) = u;

  /* If D is a definition for U, add it to the list of definitions reaching
     U.  Similarly, add U to the list of reached uses of D.  */
  if (VARREF_TYPE (d) == VARDEF && VARREF_SYM (d) == VARREF_SYM (u))
    {
      add_ref_to_list_end (VARUSE_RDEFS (u), d);
      add_ref_to_list_end (VARDEF_RUSES (d), u);

    }

    /* If D is a PHI term, recursively follow each of its arguments.  */
    if (VARREF_TYPE (d) == VARPHI)
      {
	size_t i;
	varray_type phi_chain = VARDEF_PHI_CHAIN (d);

	for (i = 0; i < VARRAY_ACTIVE_SIZE (phi_chain); i++)
	  follow_chain (VARRAY_GENERIC_PTR (phi_chain, i), u);
      }
}


/*  Return 1 if one or more uses of SYM in BB_SET have reaching definitions
    coming from blocks outside BB_SET.  If EXCLUDE_INIT_DECL is nonzero,
    the initializer expression used in the declaration of SYM will always
    be considered external to BB_SET.  */

int
is_upward_exposed (sym, bb_set, exclude_init_decl)
     tree sym;
     sbitmap bb_set;
     int exclude_init_decl;
{
  struct ref_list_node *tmp;
  varref r;

  FOR_EACH_REF (r, tmp, TREE_REFS (sym))
    {
      /* If this is a use of symbol in one of the basic blocks we are
	 interested in, check its reaching definitions.  */
      if (VARREF_TYPE (r) == VARUSE
	  && TEST_BIT (bb_set, VARREF_BB (r)->index))
	{
	  varref def;
	  struct ref_list_node *tmp2;

	  FOR_EACH_REF (def, tmp2, VARUSE_RDEFS (r))
	    {
	      basic_block def_bb = VARREF_BB (def);

	      if (IS_GHOST_DEF (def)
		  || (exclude_init_decl
		      && TREE_CODE (VARREF_STMT (def)) == DECL_STMT)
		  || ! TEST_BIT (bb_set, def_bb->index))
		return 1;
	    }
	}
    }

  return 0;
}
