/* SSA for trees.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
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
#include "hashtab.h"
#include "tree-dump.h"

/* This file builds the SSA form for a function as described in:

   R. Cytron, J. Ferrante, B. Rosen, M. Wegman, and K. Zadeck. Efficiently
   Computing Static Single Sssignment Form and the Control Dependence
   Graph. ACM Transactions on Programming Languages and Systems,
   13(4):451-490, October 1991.  */


/* Next SSA version number to be assigned by make_ssa_name.  Initialized by
   init_tree_ssa.  */
unsigned int next_ssa_version;

/* Dump file and flags.  */
FILE *tree_ssa_dump_file;
int tree_ssa_dump_flags;

/* Workstack for computing PHI node insertion points.  */
static GTY (()) varray_type work_stack = NULL;

/* Each entry in DEF_BLOCKS is a bitmap B for a variable VAR.  If B(J) is
   set, then there is a definition for variable VAR in basic block J.
   This hashtable is created by the variable reference finder and used by
   the SSA builder to add PHI nodes.  It is destroyed after the program is
   converted to SSA form.  */
static htab_t def_blocks;

/* Structure to map a variable VAR to the set of blocks that contain
   definitions for VAR.  */
struct def_blocks_d
{
  tree var;
  bitmap def_blocks;
  bitmap livein_blocks;
};

/* Hash table to store the current reaching definition for every variable in
   the function.  Given a variable V, its entry will be its immediately
   reaching SSA_NAME node.  */
static htab_t currdefs;

/* Structure to map variables and their current reaching definition.  */
struct currdef_d
{
  tree var;
  tree currdef;
};


/* Local functions.  */
static void init_tree_ssa		PARAMS ((void));
static void delete_tree_ssa		PARAMS ((tree));
static void mark_def_sites		PARAMS ((dominance_info, sbitmap));
static void compute_global_livein	PARAMS ((bitmap, bitmap));
static void set_def_block		PARAMS ((tree, basic_block));
static void set_livein_block		PARAMS ((tree, basic_block));
static void insert_phi_nodes		PARAMS ((bitmap *, sbitmap));
static void rewrite_block		PARAMS ((basic_block));
static void rewrite_stmts		PARAMS ((basic_block, varray_type *));
static void rewrite_stmt		PARAMS ((tree, varray_type *));
static inline void rewrite_operand	PARAMS ((tree *));
static void register_new_def		PARAMS ((tree, varray_type *));
static void insert_phi_nodes_for	PARAMS ((tree, bitmap *));
static tree remove_annotations_r	PARAMS ((tree *, int *, void *));
static inline tree currdef_for		PARAMS ((tree, int));
static inline void set_currdef_for	PARAMS ((tree, tree));
static hashval_t def_blocks_hash	PARAMS ((const void *));
static int def_blocks_eq		PARAMS ((const void *, const void *));
static hashval_t currdef_hash		PARAMS ((const void *));
static int currdef_eq			PARAMS ((const void *, const void *));
static void def_blocks_free		PARAMS ((void *));
static int debug_def_blocks_r		PARAMS ((void **, void *));


/* Main entry point to the SSA builder.  FNDECL is the gimplified function
   to convert.

   Every statement has two different sets of operands: real and virtual.
   Real operands are those that use scalar, non-aliased variables.  Virtual
   operands are everything else (pointers, arrays, compound structures).
   In addition, virtual operands are used to model side effects like
   call-clobbered variables.

   The conversion process works in four phases:

   1- Compute aliasing information (compute_may_aliases).
   2- Mark blocks that contain variable definitions (mark_def_sites).
   3- Insert PHI nodes (insert_phi_nodes).
   4- Rewrite all the basic blocks into SSA form (rewrite_block).

   Both operands (OPS) and virtual operands (VOPS) are rewritten by the
   conversion process.  When new definitions are found, a new SSA_NAME
   wrapper is created for the variable.  SSA_NAMEs are akin to VAR_DECLs,
   but contain additional information (defining statement and SSA version
   number).

   The pretty printer renders SSA_NAME nodes using brackets and underscores
   to highlight the variable and its version number.  Given a variable VAR,
   a new SSA_NAME for VAR with version number N is rendered VAR_N.  For
   instance,

   	a = 4;			a_1 = 4;
	*p = a;			(*p)_3 = a_1;

   Notice that the SSA builder treats INDIRECT_REF expressions as if they
   were variables.  It renames both P and *P separately.  Every time P is
   assigned a new value, *P is considered clobbered.  But new versions of
   *P do not affect P.


   Dealing with aliases
   --------------------

   Assume that in the following code fragment, variable *p may alias b:

	    1	b = 4;
	    2	*p = 5;
	    3	... = b;

   When converting this code to SSA form, we need to model the fact that
   the use of 'b' at line 3 may be reached by the definition of *p at line
   2.  We cannot just rewrite the use of 'b' to be a use of '*p':

	    1	b_1 = 4;
	    2	(*p)_2 = 5;
	    3	... = (*p)_2;

   The above transformation is wrong, because *p and b MAY alias, the
   compiler doesn't know whether they will always alias at runtime.  On the
   other hand, ignoring the may alias relation also leads to an incorrect
   transformation:
   
	    1	b_1 = 4;
	    2	(*p)_2 = 5;
	    3	... = b_1;

   With this version, the optimizers will not notice that the assignment to
   *p at line 2 may in fact reach the use of b at line 3.  To deal with
   this problem, the rewriting pass will move aliased operands into the
   virtual operands and then re-write the virtual operand, while leaving
   the original operand untouched.  So, the above code fragment is converted
   into:

	     	# (*p)_2 = VDEF <(*p)_1>;
	    1	b = 4;
	    2	(*p)_3 = 5;
	     	# VUSE <(*p)_3>
	    3	... = b;

   Notice how variable 'b' is not rewritten anymore.  Instead, references
   to it are moved to the virtual operands which are all rewritten using *p
   instead of b.

   Virtual operands provide safe information about potential references to
   the operands in a statement.  But they are imprecise by nature.
   Optimizations may want to take them into account, at the expense of
   increased compilation time.  */

void
rewrite_into_ssa (fndecl)
     tree fndecl;
{
  bitmap *dfs;
  sbitmap globals;
  dominance_info idom;
  int i;
  
  timevar_push (TV_TREE_SSA_OTHER);

  /* Initialize common SSA structures.  */
  init_tree_ssa ();

  /* Debugging dumps.  */
  tree_ssa_dump_file = dump_begin (TDI_ssa, &tree_ssa_dump_flags);

  /* Compute aliasing information.  */
  compute_may_aliases ();

  globals = sbitmap_alloc (num_referenced_vars);
  sbitmap_zero (globals);

  /* Compute immediate dominators and dominance frontiers.  */
  idom = calculate_dominance_info (CDI_DOMINATORS);

  /* Find variable references and mark definition sites.  */
  mark_def_sites (idom, globals);

  dfs = (bitmap *) xmalloc (n_basic_blocks * sizeof (bitmap *));
  for (i = 0; i < n_basic_blocks; i++)
    dfs[i] = BITMAP_XMALLOC ();
  compute_dominance_frontiers (dfs, idom);

  /* Insert PHI nodes at dominance frontiers of definition blocks.  */
  insert_phi_nodes (dfs, globals);

  /* Rewrite all the basic blocks in the program.  */
  timevar_push (TV_TREE_SSA_REWRITE_BLOCKS);
  rewrite_block (ENTRY_BLOCK_PTR);
  timevar_pop (TV_TREE_SSA_REWRITE_BLOCKS);

  /* Free allocated memory.  */
  for (i = 0; i < n_basic_blocks; i++)
    BITMAP_XFREE (dfs[i]);
  free (dfs);
  free (globals);
  free_dominance_info (idom);
  htab_delete (def_blocks);
  htab_delete (currdefs);

  /* Debugging dumps.  */
  if (tree_ssa_dump_file)
    {
      if (tree_ssa_dump_flags & TDF_STATS)
	dump_dfa_stats (tree_ssa_dump_file);

      dump_end (TDI_ssa, tree_ssa_dump_file);
      tree_ssa_dump_file = NULL;
    }

  dump_function (TDI_ssa, fndecl);
  timevar_pop (TV_TREE_SSA_OTHER);
}

/* Compute global livein information given the set of blocks where
   an object is locally live at the start of the block (LIVEIN)
   and the set of blocks where the object is defined (DEF_BLOCKS).

   Note: This routine augments the existing local livein information
   to include global livein.  Ie, it modifies the underlying bitmap
   for LIVEIN.  */

static void
compute_global_livein (livein, def_blocks)
     bitmap livein;
     bitmap def_blocks;
{
  basic_block bb, *worklist, *tos;

  tos = worklist
    = (basic_block *) xmalloc (sizeof (basic_block) * (n_basic_blocks + 1));

  FOR_EACH_BB (bb)
    {
      if (bitmap_bit_p (livein, bb->index))
	*tos++ = bb;
    }

  while (tos != worklist)
    {
      edge e;

      bb = *--tos;
      for (e = bb->pred; e; e = e->pred_next)
	{
	  basic_block bb = e->src;
	  int bb_index = bb->index;

	  if (bb != ENTRY_BLOCK_PTR
	      && ! bitmap_bit_p (livein, bb_index)
	      && ! bitmap_bit_p (def_blocks, bb_index))
	    {
	      *tos++ = bb;
	      bitmap_set_bit (livein, bb_index);
	    }
	}
    }
  free (worklist);
}



/* Look for variable references in every block of the flowgraph, compute
   aliasing information and collect definition sites for every variable.

   Also, compute the set of dominator children for each block in the
   flowgraph.  This will be used by rewrite_block when traversing the
   flowgraph.

   Return a bitmap for the set of referenced variables which are
   "nonlocal", ie those which are live across block boundaries.
   This information is used to reduce the number of PHI nodes
   we create.  */

static void
mark_def_sites (idom, globals)
     dominance_info idom;
     sbitmap globals;
{
  basic_block bb;
  block_stmt_iterator si;
  sbitmap kills;

  kills = sbitmap_alloc (num_referenced_vars);

  /* Mark all the blocks that have definitions for each variable referenced
     in the function.  */
  FOR_EACH_BB (bb)
    {
      /* Add BB to the set of dominator children of BB's immediate
	 dominator.  */
      basic_block idom_bb = get_immediate_dominator (idom, bb);
      if (idom_bb)
	add_dom_child (idom_bb, bb);

      sbitmap_zero (kills);

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  varray_type ops;
	  size_t i;
	  tree stmt;
	  tree *dest;

	  stmt = bsi_stmt (si);
	  STRIP_NOPS (stmt);

	  get_stmt_operands (stmt);

	  /* If a variable is used before being set, then the variable
	     is live across a block boundary, so add it to NONLOCAL_VARS.  */
	  ops = use_ops (stmt);
	  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
	    {
	      tree *use = VARRAY_GENERIC_PTR (ops, i);
	      int uid = var_ann (*use)->uid;

	      if (! TEST_BIT (kills, uid))
		{
	          SET_BIT (globals, uid);
		  set_livein_block (*use, bb);
		}
	    }
	  
	  /* Similarly for virtual uses.  */
	  ops = vuse_ops (stmt);
	  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
	    {
	      tree use = VARRAY_GENERIC_PTR (ops, i);
	      int uid = var_ann (use)->uid;

	      if (! TEST_BIT (kills, uid))
	        {
	          SET_BIT (globals, uid);
		  set_livein_block (use, bb);
		}
	    }

	  /* Note that virtual definitions are irrelevant for computing
	     KILLED_VARS because a VDEF does not constitute a killing
	     definition of the variable.  However, the operand of a virtual
	     definitions is a use of the variable, so it may affect
	     NONLOCAL_VARS.  */
	  ops = vdef_ops (stmt);
	  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
	    {
	      tree vdef = VARRAY_TREE (ops, i);
	      tree vdef_op = VDEF_OP (vdef);
	      int uid = var_ann (vdef_op)->uid;

	      set_def_block (VDEF_RESULT (vdef), bb);
	      if (!TEST_BIT (kills, uid))
		{
		  SET_BIT (globals, uid);
	          set_livein_block (vdef_op, bb);
		}

	    }

	  /* Now process the definition made by this statement.  */
	  dest = def_op (stmt);
	  if (dest)
	    {
	      set_def_block (*dest, bb);
	      SET_BIT (kills, var_ann (*dest)->uid);
	    }

	}
    }

  free (kills);
}


/* Mark block BB as the definition site for variable VAR.  */

static void
set_def_block (var, bb)
     tree var;
     basic_block bb;
{
  struct def_blocks_d db, *db_p;
  void **slot;

  /* Find the DEFS bitmap associated with variable VAR.  */
  db.var = var;
  slot = htab_find_slot (def_blocks, (void *) &db, INSERT);
  if (*slot == NULL)
    {
      db_p = xmalloc (sizeof (*db_p));
      db_p->var = var;
      db_p->def_blocks = BITMAP_XMALLOC ();
      db_p->livein_blocks = BITMAP_XMALLOC ();
      *slot = (void *) db_p;
    }
  else
    db_p = (struct def_blocks_d *) *slot;

  /* Set the bit corresponding to the block where VAR is defined.  */
  bitmap_set_bit (db_p->def_blocks, bb->index);
}

/* Mark block BB as having VAR live at the entry to BB.  */

static void
set_livein_block (var, bb)
     tree var;
     basic_block bb;
{
  struct def_blocks_d db, *db_p;
  void **slot;

  /* Find the DEFS bitmap associated with variable VAR.  */
  db.var = var;
  slot = htab_find_slot (def_blocks, (void *) &db, INSERT);
  if (*slot == NULL)
    {
      db_p = xmalloc (sizeof (*db_p));
      db_p->var = var;
      db_p->def_blocks = BITMAP_XMALLOC ();
      db_p->livein_blocks = BITMAP_XMALLOC ();
      *slot = (void *) db_p;
    }
  else
    db_p = (struct def_blocks_d *) *slot;

  /* Set the bit corresponding to the block where VAR is defined.  */
  bitmap_set_bit (db_p->livein_blocks, bb->index);
}


/* Insert PHI nodes at the dominance frontier of blocks with variable
   definitions.  DFS contains the dominance frontier information for the
   flowgraph.
   
   NONLOCAL_VARs is a bitmap representing the set of variables which are
   live across basic block boundaries.  Only variables in NONLOCAL_VARs
   need PHI nodes.  */

static void
insert_phi_nodes (dfs, globals)
     bitmap *dfs;
     sbitmap globals;
{
  size_t i;

  timevar_push (TV_TREE_INSERT_PHI_NODES);

  /* Array WORK_STACK is a stack of CFG blocks.  Each block that contains
     an assignment or PHI node will be pushed to this stack.  */
  VARRAY_BB_INIT (work_stack, last_basic_block, "work_stack");

  /* Iterate over all referenced variables in the function.  For each
     variable, add to the work list all the blocks that have a definition
     for the variable.  PHI nodes will be added to the dominance frontier
     blocks of each definition block.  */
  for (i = 0; i < num_referenced_vars; i++)
    if (TEST_BIT (globals, i))
      insert_phi_nodes_for (referenced_var (i), dfs);

  work_stack = NULL;
  timevar_pop (TV_TREE_INSERT_PHI_NODES);
}


/* Perform a depth-first traversal of the dominator tree looking for
   variables to rename.  BB is the block where to start searching.  */

static void
rewrite_block (bb)
     basic_block bb;
{
  edge e;
  varray_type block_defs;
  bitmap children;
  unsigned long i;

  VARRAY_TREE_INIT (block_defs, 20, "block_defs");

  rewrite_stmts (bb, &block_defs);

  /* Visit all the successor blocks of BB looking for PHI nodes.  For every
     PHI node found, add a new argument containing the current reaching
     definition for the variable and the edge through which that definition
     is reaching the PHI node.  */
  for (e = bb->succ; e; e = e->succ_next)
    {
      tree phi;

      for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
	{
	  tree currdef = currdef_for (SSA_NAME_VAR (PHI_RESULT (phi)), true);
	  add_phi_arg (phi, currdef, e);
	}
    }

  /* Recursively search the dominator children of BB.  */
  children = dom_children (bb);
  if (children)
    EXECUTE_IF_SET_IN_BITMAP (children, 0, i, rewrite_block (BASIC_BLOCK (i)));

  /* Restore the current reaching definition for each variable referenced
     in the block (in reverse order).  */
  while (VARRAY_ACTIVE_SIZE (block_defs) > 0)
    {
      tree var;
      tree saved_def = VARRAY_TOP_TREE (block_defs);
      VARRAY_POP (block_defs);
      
      /* If SAVED_DEF is NULL, then the next slot in the stack contains the
	 variable associated with SAVED_DEF.  */
      if (saved_def == NULL_TREE)
	{
	  var = VARRAY_TOP_TREE (block_defs);
	  VARRAY_POP (block_defs);
	}
      else
	var = SSA_NAME_VAR (saved_def);

      set_currdef_for (var, saved_def);
    }
}


/* Take function FNDECL out of SSA form.

   FIXME: Need to support overlapping live ranges for different versions of
	  the same variable.  At the moment, we will silently generate
	  wrong code if an optimizer pass moves code so that two versions
	  of the same variable have overlapping live ranges.  */

void
rewrite_out_of_ssa (fndecl)
     tree fndecl;
{
  basic_block bb;
  block_stmt_iterator si;

  FOR_EACH_BB (bb)
    for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
      {
	size_t i;
	varray_type ops;
	tree stmt = bsi_stmt (si);
	STRIP_NOPS (stmt);

	get_stmt_operands (stmt);

	if (def_op (stmt))
	  {
	    tree *def_p = def_op (stmt);
	    *def_p = SSA_NAME_VAR (*def_p);
	  }

	ops = use_ops (stmt);
	for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
	  {
	    tree *use_p = VARRAY_GENERIC_PTR (ops, i);
	    *use_p = SSA_NAME_VAR (*use_p);
	  }
      }

  /* Flush out flow graph and SSA data.  */
  delete_tree_ssa (fndecl);
  delete_tree_cfg ();
}


/* Remove a PHI argument from PHI.  BLOCK is the predecessor block where
   the PHI argument is coming from.

   This routine assumes ordering of alternatives in the vector is not
   important and implements removal by swapping the last alternative with
   the alternative we want to delete, then shrinking the vector.  */

void
remove_phi_arg (phi, block)
     tree phi;
     basic_block block;
{
  size_t i, num_elem = PHI_NUM_ARGS (phi);

  for (i = 0; i < num_elem; i++)
    {
      basic_block src_bb;

      src_bb = PHI_ARG_EDGE (phi, i)->src;

      if (src_bb == block)
	{
	  /* If we are not at the last element, switch the last element
	     with the element we want to delete.  */
	  if (i != num_elem - 1)
	    {
	      PHI_ARG_DEF(phi, i) = PHI_ARG_DEF(phi, num_elem - 1);
	      PHI_ARG_EDGE(phi, i) = PHI_ARG_EDGE(phi, num_elem - 1);
	    }

	  /* Shrink the vector.  */
	  PHI_NUM_ARGS (phi)--;
	}
    }
}


/* Remove PHI node PHI from basic block BB.  If PREV is non-NULL, it is
   used as the node immediately before PHI in the linked list.  */

void
remove_phi_node (phi, prev, bb)
    tree phi;
    tree prev;
    basic_block bb;
{
  if (prev)
    {
      /* Rewire the list if we are given a PREV pointer.  */
      TREE_CHAIN (prev) = TREE_CHAIN (phi);
    }
  else if (phi == phi_nodes (bb))
    {
      /* Update the list head if removing the first element.  */
      bb_ann_t ann = bb_ann (bb);
      ann->phi_nodes = TREE_CHAIN (phi);
    }
  else
    {
      /* Traverse the list looking for the node to remove.  */
      tree prev, t;
      prev = NULL_TREE;
      for (t = phi_nodes (bb); t && t != phi; t = TREE_CHAIN (t))
	prev = t;
      if (t)
	remove_phi_node (t, prev, bb);
    }
}


/*---------------------------------------------------------------------------
			       Debugging support
---------------------------------------------------------------------------*/
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
      fputs ("    ", file);
      print_generic_stmt (file, phi_nodes (bb), 0);
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
     bitmap *dfs;
{
  struct def_blocks_d dm, *def_map;
  bitmap phi_insertion_points;
  unsigned phi_vector_lengths = 0;
  int use_fully_pruned_ssa = 0;
  int bb_index;

  dm.var = var;
  def_map = (struct def_blocks_d *) htab_find (def_blocks, (void *) &dm);
  if (def_map == NULL)
    return;

  phi_insertion_points = BITMAP_XMALLOC ();

  EXECUTE_IF_SET_IN_BITMAP (def_map->def_blocks, 0, bb_index,
    {
      VARRAY_PUSH_BB (work_stack, BASIC_BLOCK (bb_index));
    });

  /* Pop a block off the worklist, add every block that appears in
     the original block's dfs that we have not already processed to
     the worklist.  Iterate until the worklist is empty.   Blocks
     which are added to the worklist are potential sites for
     PHI nodes. 

     The iteration step could be done during PHI insertion.  But
     finding all the PHI insertion blocks first allows us to use
     that list of blocks in our heuristics to determine if we should
     use semi-pruned for fully-pruned SSA forms. 

     While we're iterating we also compute the total length of all the
     PHI node vectors for this variable.  We use this in our 
     heuristic.  */
  while (VARRAY_ACTIVE_SIZE (work_stack) > 0)
    {
      edge e;
      basic_block bb = VARRAY_TOP_BB (work_stack);
      int bb_index = bb->index;

      VARRAY_POP (work_stack);
      

      EXECUTE_IF_SET_IN_BITMAP (dfs[bb_index], 0, bb_index,
	{
          basic_block bb = BASIC_BLOCK (bb_index);

	  if (! bitmap_bit_p (phi_insertion_points, bb_index))
	    {
	      for (e = bb->pred; e; e = e->pred_next)
		phi_vector_lengths++;
	      VARRAY_PUSH_BB (work_stack, bb);
	      bitmap_set_bit (phi_insertion_points, bb_index);
	    }
	});
    }

  /* Now that we know the number of elements in all the potential
     PHI nodes for this variable, we can determine if it is 
     worth computing the fully pruned SSA form.  The larger the
     total number of elements, the more important it is to use
     the fully pruned form.
 
     Experimentation showed that once we get more than 8 phi vector
     entries that moving to a fully-pruned implementation is comparable
     to semi-pruned.  32 showed up as the threshhold which maximized
     overall compile-time performance. 

     Note that as this number gets larger, the potential for the
     compiler to run wild and eat all available memory increases. 
     So if you decide to change it, do so with care.  Consider
     compile/20001226-1 with all memory references disambiguated
     as the testcase for the compiler running wild eating memory.  */
  if (phi_vector_lengths > 32)
    {
      use_fully_pruned_ssa = 1;
      compute_global_livein (def_map->livein_blocks, def_map->def_blocks);
    }

  EXECUTE_IF_SET_IN_BITMAP (phi_insertion_points, 0, bb_index,
    {
      if (! use_fully_pruned_ssa
	  || bitmap_bit_p (def_map->livein_blocks, bb_index))
        create_phi_node (var, BASIC_BLOCK (bb_index));
    });

  BITMAP_XFREE (phi_insertion_points);
}

/* Rewrite all the statements in basic block BB.
   
   BLOCK_DEFS_P points to a stack with all the definitions found in the
      block.  This is used by rewrite_block to restore the current reaching
      definition for every variable defined in BB after visiting the
      immediate dominators of BB.  */

static void
rewrite_stmts (bb, block_defs_p)
     basic_block bb;
     varray_type *block_defs_p;
{
  block_stmt_iterator si;
  tree phi;

  /* Process PHI nodes in the block.  Conceptually, all the PHI nodes are
     executed in parallel and each PHI node introduces a new version for
     the associated variable.  */
  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    register_new_def (PHI_RESULT (phi), block_defs_p);

  /* Rewrite every variable used in each statement the block with its
     immediate reaching definitions.  Update the current definition of a
     variable when a new real or virtual definition is found.  */
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    rewrite_stmt (bsi_stmt (si), block_defs_p);
}


/* Rewrite statement STMT into SSA form.  BLOCK_DEFS_P is as in
   rewrite_stmts.  */

static void
rewrite_stmt (stmt, block_defs_p)
     tree stmt;
     varray_type *block_defs_p;
{
  size_t i;
  varray_type ops;

  STRIP_NOPS (stmt);

#if defined ENABLE_CHECKING
  /* We have just scanned the code for operands.  No statement should
     be modified.  */
  if (stmt_modified_p (stmt))
    abort ();
#endif

  /* Rewrite uses and virtual uses in the statement (note that we have
     already collected operands for every statement in mark_def_sites).  If
     there are aliased loads in the statement, move the operands to VUSES.
     This way, we will be able to re-write aliased loads to their
     immediately reaching aliased definition.  */
  ops = use_ops (stmt);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    rewrite_operand ((tree *)VARRAY_GENERIC_PTR (ops, i));

  ops = vuse_ops (stmt);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    rewrite_operand (&(VARRAY_TREE (ops, i)));

  /* Register new definitions and virtual definitions made by the
     statement.  */
  if (def_op (stmt))
    {
      tree *def = def_op (stmt);
      *def = make_ssa_name (*def, stmt);
      register_new_def (*def, block_defs_p);
    }

  ops = vdef_ops (stmt);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    {
      tree vdef = VARRAY_TREE (ops, i);
      rewrite_operand (&(VDEF_OP (vdef)));
      VDEF_RESULT (vdef) = make_ssa_name (VDEF_RESULT (vdef), stmt);
      register_new_def (VDEF_RESULT (vdef), block_defs_p);
    }
}


/* Replace the operand pointed by OP_P with its immediate reaching
   definition.  */

static inline void
rewrite_operand (op_p)
     tree *op_p;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE (*op_p) == SSA_NAME)
    abort ();
#endif
  *op_p = currdef_for (*op_p, true);
}


/* Register a new definition for the variable defined by DEF and push its
   current reaching definition into the stack pointed by BLOCK_DEFS_P.  */

static void
register_new_def (def, block_defs_p)
     tree def;
     varray_type *block_defs_p;
{
  tree var = SSA_NAME_VAR (def);
  tree currdef = currdef_for (var, false);

  /* If the current reaching definition is NULL, push the variable
     itself so that rewrite_blocks knows what variable is associated with
     this NULL reaching def when unwinding the *BLOCK_DEFS_P stack.  */
  if (currdef == NULL_TREE)
    VARRAY_PUSH_TREE (*block_defs_p, var);

  /* Push the current reaching definition into *BLOCK_DEFS_P.  This stack is
     later used by rewrite_block to restore the reaching definitions for
     all the variables defined in the block after a recursive visit to all
     its immediately dominated blocks.  */
  VARRAY_PUSH_TREE (*block_defs_p, currdef);

  /* Set the current reaching definition for VAR to be DEF.  */
  set_currdef_for (var, def);
}


/*---------------------------------------------------------------------------
			     Various helpers.
---------------------------------------------------------------------------*/
/* Initialize DFA/SSA structures.  */

static void
init_tree_ssa ()
{
  next_ssa_version = 1;
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

  /* Allocate memory for the DEF_BLOCKS hash table.  */
  def_blocks = htab_create (num_referenced_vars, def_blocks_hash,
			    def_blocks_eq, def_blocks_free);

  /* Allocate memory for the CURRDEFS hash table.  */
  currdefs = htab_create (num_referenced_vars, currdef_hash, currdef_eq, free);
}


/* Deallocate memory associated with SSA data structures for FNDECL.  */

static void
delete_tree_ssa (fndecl)
     tree fndecl;
{
  unsigned long int i;

  /* Remove annotations from every tree in the function.  */
  walk_tree (&DECL_SAVED_TREE (fndecl), remove_annotations_r, NULL, NULL);

  /* Remove annotations from every referenced variable.  */
  for (i = 0; i < num_referenced_vars; i++)
    referenced_var (i)->common.ann = NULL;

  num_referenced_vars = 0;
  referenced_vars = NULL;
  global_var = NULL_TREE;
}


/* Callback function for walk_tree to clear DFA/SSA annotations from node
   *TP.  */

static tree
remove_annotations_r (tp, walk_subtrees, data)
    tree *tp;
    int *walk_subtrees ATTRIBUTE_UNUSED;
    void *data ATTRIBUTE_UNUSED;
{
  (*tp)->common.ann = NULL;
  return NULL_TREE;
}


/* Return the current definition for variable V.  If none is found and
   CREATE_DEFAULT is nonzero, create a new SSA name to act as the zeroth
   definition for V.  */

static inline tree
currdef_for (v, create_default)
     tree v;
     int create_default;
{
  struct currdef_d *def_map, dm;

#if defined ENABLE_CHECKING
  if (!SSA_VAR_P (v))
    abort ();
#endif

  dm.var = v;
  def_map = (struct currdef_d *) htab_find (currdefs, (void *) &dm);
  if (def_map && def_map->currdef)
    return def_map->currdef;
  else
    {
      if (create_default)
	{
	  tree def = make_ssa_name (v, empty_stmt_node);
	  set_currdef_for (v, def);
	  return def;
	}
      else
	return NULL_TREE;
    }
}


/* Set DEF to be the current definition for variable V.  */

static inline void
set_currdef_for (v, def)
     tree v;
     tree def;
{
  struct currdef_d *currdef_p, cd;
  void **slot;

#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (v)) != 'd'
      && TREE_CODE (v) != INDIRECT_REF)
    abort ();

  if (def && TREE_CODE (def) != SSA_NAME)
    abort ();
#endif

  cd.var = v;
  slot = htab_find_slot (currdefs, (void *) &cd, INSERT);
  if (*slot == NULL)
    {
      currdef_p = xmalloc (sizeof *currdef_p);
      currdef_p->var = v;
      *slot = (void *) currdef_p;
    }
  else
    currdef_p = (struct currdef_d *) *slot;

  currdef_p->currdef = def;
}


/* Free memory allocated for a <def, def_blocks> tuple.  */

static void
def_blocks_free (p)
     void *p;
{
  struct def_blocks_d *db_p = (struct def_blocks_d *) p;
  BITMAP_XFREE (db_p->def_blocks);
  BITMAP_XFREE (db_p->livein_blocks);
  free (p);
}


/* Hashing and equality functions for DEF_BLOCKS.  */

static hashval_t
def_blocks_hash (p)
     const void *p;
{
  return htab_hash_pointer ((const void *)((const struct def_blocks_d *)p)->var);
}

static int
def_blocks_eq (p1, p2)
     const void *p1;
     const void *p2;
{
  return ((const struct def_blocks_d *)p1)->var
	 == ((const struct def_blocks_d *)p2)->var;
}


/* Hashing and equality functions for CURRDEF.  */

static hashval_t
currdef_hash (p)
     const void *p;
{
  return htab_hash_pointer ((const void *)((const struct currdef_d *)p)->var);
}

static int
currdef_eq (p1, p2)
     const void *p1;
     const void *p2;
{
  return ((const struct currdef_d *)p1)->var
	 == ((const struct currdef_d *)p2)->var;
}


/* Dump the DEF_BLOCKS hash table on stderr.  */

void
debug_def_blocks ()
{
  htab_traverse (def_blocks, debug_def_blocks_r, NULL);
}

/* Callback for htab_traverse to dump the DEF_BLOCKS hash table.  */

static int
debug_def_blocks_r (slot, data)
     void **slot;
     void *data ATTRIBUTE_UNUSED;
{
  unsigned long i;
  struct def_blocks_d *db_p = (struct def_blocks_d *) *slot;
  
  fprintf (stderr, "VAR: ");
  print_generic_expr (stderr, db_p->var, 0);
  fprintf (stderr, ", DEF_BLOCKS: { ");
  EXECUTE_IF_SET_IN_BITMAP (db_p->def_blocks, 0, i,
			    fprintf (stderr, "%ld ", i));
  fprintf (stderr, "}\n");
  fprintf (stderr, ", LIVEIN_BLOCKS: { ");
  EXECUTE_IF_SET_IN_BITMAP (db_p->livein_blocks, 0, i,
			    fprintf (stderr, "%ld ", i));
  fprintf (stderr, "}\n");

  return 1;
}
