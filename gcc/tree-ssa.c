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
#include "tree-opt.h"
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
	  tree_debug_bb (bb);
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
  tree m;
  varray_type added;
  varray_type in_work;
  varray_type work_list;
  size_t work_list_top;

  /* Array ADDED (indexed by basic block number) is used to determine
     whether a phi term for the current variable has already been
     inserted at block X.  */
  VARRAY_TREE_INIT (added, n_basic_blocks, "added");

  /* Array IN_WORK (indexed by basic block number) is used to determine
     whether block X has already been added to WORK_LIST for the current
     variable.  */
  VARRAY_TREE_INIT (in_work, n_basic_blocks, "in_work");

  /* Array WORK_LIST is a stack of CFG blocks.  Each block that contains
     an assignment or PHI term will be added to this work list.  */
  VARRAY_BB_INIT (work_list, n_basic_blocks, "work_list");

  work_list_top = 0;

  /* Iterate over all referenced symbols in the function. For each
     symbol, add to the work list all the blocks that have a definition
     for the symbol.  PHI terms will be added to the dominance frontier
     blocks of each definition block.  */

  for (m = ref_symbols_list; m; m = TREE_CHAIN (m))
    {
      varref_node i;
      tree sym = TREE_VALUE (m);

      /* Symbols in ref_symbols_list must have at least 1 reference.  */
      if (TREE_ANN (sym) == NULL)
	abort ();

      for (i = VARREF_LIST_FIRST (TREE_REFS (sym)); i;
	   i = VARREF_NODE_NEXT (i))
	{
	  basic_block bb;
	  varref ref = VARREF_NODE_ELEM (i);

	  if (VARREF_TYPE (ref) != VARDEF)
	    continue;

	  bb = VARREF_BB (ref);

	  /* Grow WORK_LIST by ~25%.  */
	  if (work_list_top >= VARRAY_SIZE (work_list))
	    VARRAY_GROW (work_list, work_list_top + (work_list_top + 3) / 4);
	  VARRAY_BB (work_list, work_list_top) = bb;
	  work_list_top++;
	  VARRAY_TREE (in_work, bb->index) = sym;
	}

      while (work_list_top > 0)
	{
	  int w;
	  basic_block bb;

	  work_list_top--;
	  bb = VARRAY_BB (work_list, work_list_top);

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

		  phi = create_varref (sym, VARPHI, bb, stmt_bb->head_tree, NULL);
		  VARRAY_TREE (added, w) = sym;

		  if (VARRAY_TREE (in_work, w) != sym)
		    {
		      /* Grow WORK_LIST by ~25%.  */
		      if (work_list_top >= VARRAY_SIZE (work_list))
			VARRAY_GROW (work_list, 
				     work_list_top + (work_list_top + 3) / 4);
		      VARRAY_BB (work_list, work_list_top) = bb;
		      work_list_top++;
		      VARRAY_TREE (in_work, w) = sym;
		    }
		}
	    });
	}
    }

  VARRAY_FREE (added);
  VARRAY_FREE (in_work);
  VARRAY_FREE (work_list);
}

/* }}} */

/* {{{ build_fud_chains()

   Build FUD (Factored Use-Def) chains.  */

static void
build_fud_chains (idom)
     int *idom;
{
  tree m;

  /* Initialize the current definition for all the symbols.  */

  for (m = ref_symbols_list; m; m = TREE_CHAIN (m))
    {
      tree sym = TREE_VALUE (m);
      tree_ann ann = TREE_ANN (sym);

      /* Symbols in ref_symbols_list must have at least 1 reference.  */
      if (TREE_ANN (sym) == NULL)
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
  varref_node n;
  edge e;
  int i;

  /* for each variable use or def or phi-term R in X do
         let M be the variable referenced at R
         if R is a use then
             Chain(R) = CurrDef(M)
         else if R is a def or $\phi$-term then
             SaveChain(R) = CurrDef(M)
             CurrDef(M) = R
         endif
     endfor  */

  for (n = VARREF_LIST_FIRST (BB_REFS (bb)); n; n = VARREF_NODE_NEXT (n))
    {
      varref currdef;
      varref ref = VARREF_NODE_ELEM (n);
      tree m = VARREF_SYM (ref);

      /* Retrieve the current definition for the variable.  */
      currdef = TREE_CURRDEF (m);

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
	      if (VARDEF_IMM_USES (currdef))
		push_ref (VARDEF_IMM_USES (currdef), ref);
	      else
		VARDEF_IMM_USES (currdef) = create_varref_list (ref);
	    }
	}
      else if (VARREF_TYPE (ref) == VARDEF || VARREF_TYPE (ref) == VARPHI)
	{
	  tree_ann ann;

	  VARDEF_SAVE_CHAIN (ref) = currdef;

	  /* Replace the current currdef with a new one.  */
	  ann = get_tree_ann (m);
	  ann->currdef = ref;
	}
    }



  /* for Y in SUCC(X) do
        J = WhichPred(X -> Y)
        for each phi-term R in Y do
            let M be the variable referenced at R
            phi-chain(R)[J] = CurrDef(M)
        endfor
     endfor  */

  for (e = bb->succ, i = 0; e; e = e->succ_next, i++)
    {
      basic_block y = e->dest;

      for (n = VARREF_LIST_FIRST (BB_REFS (y)); n; n = VARREF_NODE_NEXT (n))
	{
	  tree sym;
	  varref currdef;
	  varref phi = VARREF_NODE_ELEM (n);

	  if (VARREF_TYPE (phi) != VARPHI)
	    continue;

	  sym = VARREF_SYM (phi);
	  currdef = TREE_CURRDEF (sym);

	  if (currdef)
	    {
	      if (VARPHI_CHAIN (phi))
		push_ref (VARPHI_CHAIN (phi), currdef);
	      else
		VARPHI_CHAIN (phi) = create_varref_list (currdef);
	    }
	}
    }



  /* for Y in Child(X) do	<-- Child(X) is the set of dominator
    	   Search(Y)                children of X in the dominator tree.
     endfor  */

  for (i = 0; i < n_basic_blocks; i++)
    {
      if (idom[i] == bb->index)
	search_fud_chains (BASIC_BLOCK (i), idom);
    }


  /* for each variable use or def or phi-term R in X in reverse order do
         let M be the variable referenced at R
         if R is a def or a phi-term then
             CurrDef(M) = SaveChain(R)
         endif
     endfor  */

  for (n = VARREF_LIST_LAST (BB_REFS (bb)); n; n = VARREF_NODE_PREV (n))
    {
      varref ref = VARREF_NODE_ELEM (n);

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
