/* SSA for trees.
   Copyright (C) 2001 Free Software Foundation, Inc.
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
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "c-common.h"
#include "tree-optimize.h"
#include "tree-flow.h"
#include "ssa.h"

/* {{{ Local declarations.  */

/* Dump file and flags.  */
static FILE *dump_file;
static int dump_flags;

/* Local functions.  */
static void insert_phi_terms PARAMS ((sbitmap *));
static void build_fud_chains PARAMS ((int *));
static void search_fud_chains PARAMS ((basic_block, int *));

/* }}} */

/* {{{ tree_build_ssa()

   Build the SSA form for the given function. This implements Factored
   Use-Def Chains as described in 

   Wolfe, M. J., High Performance Compilers for Parallel Computing,
   Addison-Wesley, 1996.  */

void
tree_build_ssa ()
{
  sbitmap *dfs;
  int *idom;

  dump_file = dump_begin (TDI_ssa, &dump_flags);

  /* Compute immediate dominators.  */
  idom = (int *) xmalloc ((size_t) n_basic_blocks * sizeof (int));
  memset ((void *) idom, -1, (size_t) n_basic_blocks * sizeof (int));
  calculate_dominance_info (idom, NULL, CDI_DOMINATORS);

  /* Compute dominance frontiers.  */
  dfs = sbitmap_vector_alloc (n_basic_blocks, n_basic_blocks);
  compute_dominance_frontiers (dfs, idom);

  /* Insert the PHI nodes and build FUD chains.  */
  insert_phi_terms (dfs);
  build_fud_chains (idom);

  sbitmap_vector_free (dfs);
  free (idom);

  /* Debugging dumps.  */
  if (dump_file)
    {
      int i;

      fputc ('\n', dump_file);
      fprintf (dump_file, ";; Function %s\n\n",
	       IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

      fputs ("SSA information\n\n", dump_file);

      for (i = 0; i < n_basic_blocks; i++)
	{
	  basic_block bb = BASIC_BLOCK (i);
	  tree_dump_bb (dump_file, "", bb, 0);
	  dump_varref_list (dump_file, "    ", BB_REFS (bb), 0, 1);
	  fputs ("\n\n", dump_file);
	}
    }
}

/* }}} */

/* {{{ insert_phi_terms()

   Insert PHI terms at the dominance frontier of nodes with variable
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
     whether a phi term for the current variable has already been
     inserted at block X.  */
  VARRAY_TREE_INIT (added, n_basic_blocks, "added");

  /* Array IN_WORK (indexed by basic block number) is used to determine
     whether block X has already been added to WORK_STACK for the current
     variable.  */
  VARRAY_TREE_INIT (in_work, n_basic_blocks, "in_work");

  /* Array WORK_STACK is a stack of CFG blocks.  Each block that contains
     an assignment or PHI term will be pushed to this stack.  */
  VARRAY_BB_INIT (work_stack, n_basic_blocks, "work_stack");

  /* Iterate over all referenced symbols in the function. For each
     symbol, add to the work list all the blocks that have a definition
     for the symbol.  PHI terms will be added to the dominance frontier
     blocks of each definition block.  */

  for (i = 0; i < VARRAY_ACTIVE_SIZE (referenced_symbols); i++)
    {
      size_t j;
      tree sym = VARRAY_TREE (referenced_symbols, i);
      varray_type refs = TREE_REFS (sym);

      /* Symbols in referenced_symbols must have at least 1 reference.  */
      if (refs == NULL)
	abort ();

      for (j = 0; j < VARRAY_ACTIVE_SIZE (refs); j++)
	{
	  basic_block bb;
	  varref ref = VARRAY_GENERIC_PTR (refs, j);

	  if (VARREF_TYPE (ref) != VARDEF)
	    continue;

	  bb = VARREF_BB (ref);
	  VARRAY_PUSH_BB (work_stack, bb);
	  VARRAY_TREE (in_work, bb->index) = sym;
	}

      while (VARRAY_ACTIVE_SIZE (work_stack) > 0)
	{
	  int w;
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

		  phi = create_varref (sym, VARPHI, bb, stmt_bb->head_tree,
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

  VARRAY_FREE (added);
  VARRAY_FREE (in_work);
  VARRAY_FREE (work_stack);
}

/* }}} */

/* {{{ build_fud_chains()

   Build FUD (Factored Use-Def) chains.  */

static void
build_fud_chains (idom)
     int *idom;
{
  size_t i;

  /* Initialize the current definition for all the symbols.  */

  for (i = 0; i < VARRAY_ACTIVE_SIZE (referenced_symbols); i++)
    {
      tree sym = VARRAY_TREE (referenced_symbols, i);
      tree_ann ann = TREE_ANN (sym);

      /* Symbols in ref_symbols_list must have at least 1 reference.  */
      if (ann == NULL)
	abort ();

      ann->currdef = NULL;
    }

  /* Search FUD chains starting with the entry block.  */
  search_fud_chains (ENTRY_BLOCK_PTR, idom);
}

/* }}} */

/* {{{ search_fud_chains()

   Perform a depth-first traversal of the dominator tree looking for FUD
   chains.  */

static void
search_fud_chains (bb, idom)
     basic_block bb;
     int *idom;
{
  varray_type bb_refs;
  edge e;
  int i;
  size_t r, nrefs;

  /* for each variable use or def or phi-term R in BB do
         let SYM be the variable referenced at R
         if R is a use then
             Chain(R) = CurrDef(SYM)
         else if R is a def or $\phi$-term then
             SaveChain(R) = CurrDef(SYM)
             CurrDef(SYM) = R
         endif
     endfor  */

  bb_refs = BB_REFS (bb);
  nrefs = (bb_refs) ? VARRAY_ACTIVE_SIZE (bb_refs) : 0;
  for (r = 0; r < nrefs; r++)
    {
      varref currdef;
      varref ref = VARRAY_GENERIC_PTR (bb_refs, r);
      tree sym = VARREF_SYM (ref);

      /* Retrieve the current definition for the variable.  */
      currdef = TREE_CURRDEF (sym);

      if (VARREF_TYPE (ref) == VARUSE)
	{
	  VARUSE_CHAIN (ref) = currdef;

	  /* Besides setting a link back to our immediate control
	     reaching definition (whether a PHI term or an actual
	     definition), we want to link that definition to its
	     immediate use.

	     If this use (ref) has a current definition (currdef), we
	     add 'ref' to the list of uses immediately reached by
	     'currdef'.  */

	  if (currdef)
	    {
	      if (VARDEF_IMM_USES (currdef) == NULL)
		VARRAY_GENERIC_PTR_INIT (VARDEF_IMM_USES (currdef),
		                         10, "imm_uses");
	      VARRAY_PUSH_GENERIC_PTR (VARDEF_IMM_USES (currdef), ref);
	    }
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
      varray_type y_refs;
      basic_block y;

      y = e->dest;
      y_refs = BB_REFS (y);
      if (y_refs == NULL)
	break;

      for (r = 0; r < VARRAY_ACTIVE_SIZE (y_refs); r++)
	{
	  tree sym;
	  varref currdef;
	  varref phi = VARRAY_GENERIC_PTR (y_refs, r);

	  if (VARREF_TYPE (phi) != VARPHI)
	    continue;

	  sym = VARREF_SYM (phi);
	  currdef = TREE_CURRDEF (sym);

	  if (currdef)
	    {
	      if (VARPHI_CHAIN (phi) == NULL)
		VARRAY_GENERIC_PTR_INIT (VARPHI_CHAIN (phi), 2, "phi_chain");

	      VARRAY_PUSH_GENERIC_PTR (VARPHI_CHAIN (phi), currdef);
	    }
	}
    }


  /* for Y in Child(BB) do	<-- Child(BB) is the set of dominator
    	   Search(Y)                children of BB in the dominator tree.
     endfor  */

  for (i = 0; i < n_basic_blocks; i++)
    {
      if (idom[i] == bb->index)
	search_fud_chains (BASIC_BLOCK (i), idom);
    }


  /* for each variable use or def or phi-term R in BB in reverse order do
         let SYM be the variable referenced at R
         if R is a def or a phi-term then
             CurrDef(SYM) = SaveChain(R)
         endif
     endfor  */

  for (i = nrefs - 1; i >= 0; i--)
    {
      varref ref = VARRAY_GENERIC_PTR (bb_refs, i);

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

/* }}} */
