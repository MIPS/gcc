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

/* Structure to map variables to values.  It's used to keep track of the
   current reaching definition, constant values and variable copies while
   renaming.  */
struct var_value_d
{
  tree var;
  tree value;
};

/* Hash table with expressions made available during the renaming process.
   When an assignment of the form X_i = EXPR is found, the statement is
   stored in this table.  If the same expression EXPR is later found on the
   RHS of another statement, it is replaced with X_i (thus performing
   global redundancy elimination). */
static htab_t avail_exprs;

/* Hash table of constant values and copies indexed by SSA name.  When the
   renaming pass finds an assignment of a constant (X_i = C) or a copy
   assignment from another SSA variable (X_i = Y_j), it creates a mapping
   between X_i and the RHS in this table.  This mapping is used later on,
   when renaming uses of X_i.  If an assignment to X_i is found in this
   table, instead of using X_i, we use the RHS of the statement stored in
   this table (thus performing very simplistic copy and constant
   propagation).  */
static htab_t const_and_copies;


/* Statistics for dominator optimizations.  */
struct ssa_stats_d
{
  long num_stmts;
  long num_exprs_considered;
  long num_const_prop;
  long num_copy_prop;
  long num_re;
  /* FIXME.  [UNSSA] Not needed after SSA->normal pass is working.  */
#if 1
  long blocked_optimizations;
  long blocked_by_life_crossing;
#endif
};

static struct ssa_stats_d ssa_stats;


/* Local functions.  */
static void init_tree_ssa		PARAMS ((void));
static void delete_tree_ssa		PARAMS ((tree));
static void mark_def_sites		PARAMS ((dominance_info, sbitmap));
static void compute_global_livein	PARAMS ((bitmap, bitmap));
static void set_def_block		PARAMS ((tree, basic_block));
static void set_livein_block		PARAMS ((tree, basic_block));
static void insert_phi_nodes		PARAMS ((bitmap *, sbitmap));
static void rewrite_block		PARAMS ((basic_block));
static void rewrite_stmt		PARAMS ((block_stmt_iterator,
						 varray_type *,
						 varray_type *));
static inline void rewrite_operand	PARAMS ((tree *));
static void register_new_def		PARAMS ((tree, tree, varray_type *));
static void update_indirect_ref_vuses	PARAMS ((tree, tree, varray_type));
static void update_pointer_vuses	PARAMS ((tree, tree, varray_type));
static void insert_phi_nodes_for	PARAMS ((tree, bitmap *));
static tree remove_annotations_r	PARAMS ((tree *, int *, void *));
static tree currdef_for			PARAMS ((tree, int));
static tree get_value_for		PARAMS ((tree, htab_t));
static void set_value_for		PARAMS ((tree, tree, htab_t));
static hashval_t def_blocks_hash	PARAMS ((const void *));
static int def_blocks_eq		PARAMS ((const void *, const void *));
static hashval_t var_value_hash		PARAMS ((const void *));
static int var_value_eq			PARAMS ((const void *, const void *));
static void def_blocks_free		PARAMS ((void *));
static int debug_def_blocks_r		PARAMS ((void **, void *));
static tree lookup_avail_expr		PARAMS ((tree, varray_type *));
static hashval_t avail_expr_hash	PARAMS ((const void *));
static int avail_expr_eq		PARAMS ((const void *, const void *));
static struct def_blocks_d *get_def_blocks_for PARAMS ((tree));

/* FIXME: [UNSSA] Remove once the real unSSA pass is implemented.  */
#if 1
static bool var_is_live			PARAMS ((tree, basic_block));
#endif


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
  htab_delete (avail_exprs);
  htab_delete (const_and_copies);

  /* Debugging dumps.  */
  if (tree_ssa_dump_file)
    {
      if (tree_ssa_dump_flags & TDF_STATS)
	{
	  dump_dfa_stats (tree_ssa_dump_file);
	  dump_tree_ssa_stats (tree_ssa_dump_file);
	}

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
	      tree use = VARRAY_TREE (ops, i);
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
   variables to rename.  BB is the block where to start searching.
   Renaming is a five step process:

   1- Every definition made by PHI nodes at the start of the blocks is
      registered as the current definition for the corresponding variable.

   2- Every statement in BB is rewritten.  USE and VUSE operands are
      rewritten with their corresponding reaching definition.  DEF and
      VDEF targets are registered as new definitions.
      
      This step performs some quick and simple value number optimizations.
      Expressions computed by each statement are looked up in the
      AVAIL_EXPRS table.  If a statement is found to make a redundant
      computation, it is marked for removal.  Otherwise, the expression
      computed by the statement is assigned a value number and entered into
      the AVAIL_EXPRS table.  See try_optimize_stmt for details on the
      types of redundancies handled during renaming.

   3- All the PHI nodes in successor blocks of BB are visited.  The
      argument corresponding to BB is replaced with its current reaching
      definition.

   4- Recursively rewrite every dominator child block of BB.

   5- Restore (in reverse order) the current reaching definition for every
      new definition introduced in this block.  This is done so that when
      we return from the recursive call, all the current reaching
      definitions are restored to the names that were valid in the
      dominator parent of BB.

      This step also removes all the expressions added to the AVAIL_EXPRS
      table during renaming.  This is because the expressions made
      available to block BB and its dominator children are not valid for
      blocks above BB in the dominator tree.  */

static void
rewrite_block (bb)
     basic_block bb;
{
  edge e;
  varray_type block_defs, block_avail_exprs;
  bitmap children;
  unsigned long i;
  block_stmt_iterator si;
  tree phi;

  /* Initialize the local stacks.
     
     BLOCK_DEFS is used to save all the existing reaching definitions for
	the new SSA names introduced in this block.  Before registering a
	new definition for a variable, the existing reaching definition is
	pushed into this stack so that we can restore it in Step 5.

     BLOCK_AVAIL_EXPRS is used to store all the expressions made available
	in this block.  Since expressions made available in this block are
	only valid in blocks dominated by BB, when we finish rewriting BB
	and its dominator children, we have to remove these expressions
	from the AVAIL_EXPRS table.  This stack is used to know which
	expressions to remove from the table.  */
  VARRAY_TREE_INIT (block_defs, 20, "block_defs");
  VARRAY_TREE_INIT (block_avail_exprs, 20, "block_avail_exprs");

  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
    fprintf (tree_ssa_dump_file, "\n\nRenaming block #%d\n\n", bb->index);

  /* Step 1.  Register new definitions for every PHI node in the block.
     Conceptually, all the PHI nodes are executed in parallel and each PHI
     node introduces a new version for the associated variable.  */
  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    register_new_def (SSA_NAME_VAR (PHI_RESULT (phi)), PHI_RESULT (phi),
		      &block_defs);

  /* Step 2.  Rewrite every variable used in each statement the block with
     its immediate reaching definitions.  Update the current definition of
     a variable when a new real or virtual definition is found.  */
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    rewrite_stmt (si, &block_defs, &block_avail_exprs);

  /* Step 3.  Visit all the successor blocks of BB looking for PHI nodes.
     For every PHI node found, add a new argument containing the current
     reaching definition for the variable and the edge through which that
     definition is reaching the PHI node.  */
  for (e = bb->succ; e; e = e->succ_next)
    {
      tree phi;

      for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
	{
	  /* FIXME.  [UNSSA] After fixing the SSA->normal pass, allow
	     constants and copies to be propagated into PHI arguments.  */
	  tree currdef = currdef_for (SSA_NAME_VAR (PHI_RESULT (phi)), true);
	  add_phi_arg (phi, currdef, e);
	}
    }

  /* Step 4.  Recursively search the dominator children of BB.  */
  children = dom_children (bb);
  if (children)
    EXECUTE_IF_SET_IN_BITMAP (children, 0, i, rewrite_block (BASIC_BLOCK (i)));

  /* Step 5.  Restore the current reaching definition for each variable
     referenced in the block (in reverse order).  */
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

      set_value_for (var, saved_def, currdefs);
    }

  /* Remove all the expressions made available in this block.  */
  while (VARRAY_ACTIVE_SIZE (block_avail_exprs) > 0)
    {
      tree stmt = VARRAY_TOP_TREE (block_avail_exprs);
      VARRAY_POP (block_avail_exprs);
      htab_remove_elt (avail_exprs, stmt);
    }
}


/* Take function FNDECL out of SSA form.

   FIXME: Need to support overlapping live ranges for different versions of
	  the same variable.  At the moment, we will silently generate
	  wrong code if an optimizer pass moves code so that two versions
	  of the same variable have overlapping live ranges.

	  NOTE: Look for the string '[UNSSA]' to re-enable code that
	  depends on a properly working unSSA pass.  */

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
	      PHI_ARG_DEF (phi, i) = PHI_ARG_DEF(phi, num_elem - 1);
	      PHI_ARG_EDGE (phi, i) = PHI_ARG_EDGE(phi, num_elem - 1);
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


/* Dump SSA statistics on FILE.  */

void
dump_tree_ssa_stats (file)
     FILE *file;
{
  long tmp, n_exprs;

  fprintf (file, "Total number of statements:                   %6ld\n\n",
	   ssa_stats.num_stmts);
  fprintf (file, "Exprs considered for dominator optimizations: %6ld\n",
           ssa_stats.num_exprs_considered);

  n_exprs = ssa_stats.num_exprs_considered;
  if (n_exprs == 0)
    n_exprs = 1;

  fprintf (file, "    Constants propagated:                     %6ld (%.0f%%)\n",
           ssa_stats.num_const_prop, PERCENT (ssa_stats.num_const_prop,
	                                      n_exprs));
  fprintf (file, "    Copies propagated:                        %6ld (%.0f%%)\n",
	   ssa_stats.num_copy_prop, PERCENT (ssa_stats.num_copy_prop,
					     n_exprs));
  fprintf (file, "    Redundant expressions eliminated:         %6ld (%.0f%%)\n",
	   ssa_stats.num_re, PERCENT (ssa_stats.num_re,
				      n_exprs));

  /* FIXME.  [UNSSA] Not needed after SSA->normal pass is working.  */
#if 1
  fprintf (file, "    Optimizations blocked by lack of unSSA:   %6ld (%.0f%%)\n",
	   ssa_stats.blocked_optimizations,
	   PERCENT (ssa_stats.blocked_optimizations, n_exprs));

  tmp = ssa_stats.blocked_optimizations - ssa_stats.blocked_by_life_crossing;
  fprintf (file, "    Optimizations blocked due to pruned SSA:  %6ld (%.0f%%)\n",
	   tmp, PERCENT (tmp, n_exprs));
#endif

  fprintf (file, "\n");
}


/* Dump SSA statistics on stderr.  */

void
debug_tree_ssa_stats ()
{
  dump_tree_ssa_stats (stderr);
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
  struct def_blocks_d *def_map;
  bitmap phi_insertion_points;
  unsigned phi_vector_lengths = 0;
  int use_fully_pruned_ssa = 0;
  int bb_index;

  def_map = get_def_blocks_for (var);
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


/* Rewrite the statement pointed by iterator SI into SSA form.
   
   BLOCK_DEFS_P points to a stack with all the definitions found in the
      block.  This is used by rewrite_block to restore the current reaching
      definition for every variable defined in BB after visiting the
      immediate dominators of BB.

   BLOCK_AVAIL_EXPRS_P points to a stack with all the expressions that have
      been computed in this block and are available in children blocks to
      be reused.

   While renaming a statement, we try to perform some simplistic global
   redundancy elimination and constant propagation:

   1- To detect global redundancy, we keep track of expressions that have
      been computed in this block and its dominators.  If we find that the
      same expression is computed more than once, we eliminate repeated
      computations by using the target of the first one.  For instance,
      consider this partially renamed fragment of code:

	    a_1 = b_2 + c_3;
	    x = b_2 + c_3;
	    if (x > 0)
	      ...

      After renaming the first instance of 'b_2 + c_3', it will be added to
      the AVAIL_EXPRS table with value 'a_1'.  The next time the renaming
      process finds 'b_2 + c_3', instead of creating a new definition for
      'x', it will set the current reaching definition of 'x' to be 'a_1'.
      This way, the renaming process will proceed to generate:

	    a_1 = b_2 + c_3;
	    <deleted>
	    if (a_1 > 0)
	      ...


   2- Constant values and copy assignments.  This is used to do very
      simplistic constant and copy propagation.  When a constant or copy
      assignment is found, we map the value on the RHS of the assignment to
      the variable in the LHS in the CONST_AND_COPIES table.  The next time
      we need to rewrite an operand, we check whether the variable has a
      known value.  If so, we use that value.  Notice that this does not
      replace the constant and copy propagation passes.  It only does very
      simplistic propagation while renaming.  */

static void
rewrite_stmt (si, block_defs_p, block_avail_exprs_p)
     block_stmt_iterator si;
     varray_type *block_defs_p;
     varray_type *block_avail_exprs_p;
{
  size_t i;
  stmt_ann_t ann;
  tree stmt, *def_p;
  varray_type uses, vuses, vdefs;
  bool may_optimize_p;

  stmt = bsi_stmt (si);
  STRIP_NOPS (stmt);
  ann = stmt_ann (stmt);
  ssa_stats.num_stmts++;

  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_ssa_dump_file, "Renaming statement ");
      print_generic_stmt (tree_ssa_dump_file, stmt, TDF_SLIM);
      fprintf (tree_ssa_dump_file, "\n");
    }

#if defined ENABLE_CHECKING
  /* We have just scanned the code for operands.  No statement should
     be modified.  */
  if (ann->modified)
    abort ();
#endif

  /* FIXME: Must change the interface to statement annotations.  Helpers
	    should receive the annotation, not the statement.  Otherwise,
	    we call stmt_ann() more than necessary.  */
  def_p = def_op (stmt);
  uses = use_ops (stmt);
  vuses = vuse_ops (stmt);
  vdefs = vdef_ops (stmt);

#if defined ENABLE_CHECKING
  /* Only assignments may make a new definition.  */
  if (def_p && TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();
#endif

  /* Step 1.  Rewrite USES and VUSES in the statement.  */
  for (i = 0; uses && i < VARRAY_ACTIVE_SIZE (uses); i++)
    {
      tree val;
      tree *op_p = (tree *) VARRAY_GENERIC_PTR (uses, i);
      rewrite_operand (op_p);

      /* If the operand has a known constant value or it is known to be a
	 copy of some other variable, use the value or copy stored in
	 CONST_AND_COPIES.  */
      ssa_stats.num_exprs_considered++;
      val = get_value_for (*op_p, const_and_copies);
      if (val)
	{
#if 1
	  /* FIXME: [UNSSA] Remove the following check after implementing
	     SSA->normal.  For the time being, avoid doing copy propagation
	     if that would make two versions of VAL to be live at the same
	     time.  */
	  if (TREE_CODE (val) == SSA_NAME && !var_is_live (val, ann->bb))
	    {
	      ssa_stats.blocked_optimizations++;
	      continue;
	    }
#endif

	  /* If the replacement is a constant, mark the statement modified
	     because it just lost an operand.  */
	  if (TREE_CONSTANT (val))
	    {
	      ssa_stats.num_const_prop++;
	      ann->modified = 1;
	    }
	  else
	    ssa_stats.num_copy_prop++;

	  if (TREE_CODE (val) == SSA_NAME && vuses)
	    {
	      /* If we are replacing a pointer, the statement may have a VUSE
		 for the pointer's associated INDIRECT_REF (this happens if
		 this operand is an argument to a const function call).  We
		 need to update that VUSE appropriately.  */
	      if (POINTER_TYPE_P (TREE_TYPE (SSA_NAME_VAR (val))))
		update_indirect_ref_vuses (*op_p, val, vuses);

	      /* Similarly, if we are replacing an INDIRECT_REF, the
		 statement will have a VUSE for the old base pointer.
		 Replace it with the new one.  */
	      if (TREE_CODE (SSA_NAME_VAR (val)) == INDIRECT_REF)
		update_pointer_vuses (*op_p, val, vuses);
	    }

	  /* Replace the operand with its known constant value or copy.  */
	  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_ssa_dump_file, "  Replaced '");
	      print_generic_expr (tree_ssa_dump_file, *op_p, 0);
	      fprintf (tree_ssa_dump_file, "' with %s '",
		       TREE_CONSTANT (val) ? "constant" : "variable");
	      print_generic_expr (tree_ssa_dump_file, val, 0);
	      fprintf (tree_ssa_dump_file, "'\n");
	    }

	  *op_p = val;
	}
    }

  /* Rewrite virtual uses in the statement.  */
  for (i = 0; vuses && i < VARRAY_ACTIVE_SIZE (vuses); i++)
    rewrite_operand (&(VARRAY_TREE (vuses, i)));

  /* If the statement has been modified with constant replacements,
      fold its RHS before checking for redundant computations.  */
  if (def_p && ann->modified)
    TREE_OPERAND (stmt, 1) = fold (TREE_OPERAND (stmt, 1));


  /* Step 2.  Check for redundant computations.  Do this optimization only
     for assignments that make no calls and have no aliased nor volatile
     references and no side effects (i.e., no VDEFs).  */
  may_optimize_p = !ann->makes_aliased_loads
		   && !ann->makes_aliased_stores
		   && !ann->has_volatile_ops
		   && vdefs == NULL;

  if (may_optimize_p
      && def_p
      && TREE_CODE (TREE_OPERAND (stmt, 1)) != CALL_EXPR)
    {
      /* Check if the RHS of the assignment has been computed before.  If
	 so, use the LHS of the previously computed statement as the
	 reaching definition for the variable defined by this statement.  */
      tree cached_lhs = lookup_avail_expr (stmt, block_avail_exprs_p);
      ssa_stats.num_exprs_considered++;
      if (cached_lhs && TREE_TYPE (cached_lhs) == TREE_TYPE (*def_p))
	{
	  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_ssa_dump_file, "  Replaced redundant expr '");
	      print_generic_expr (tree_ssa_dump_file, TREE_OPERAND (stmt, 1), 0);
	      fprintf (tree_ssa_dump_file, "' with '");
	      print_generic_expr (tree_ssa_dump_file, cached_lhs, 0);
	      fprintf (tree_ssa_dump_file, "'\n");
	    }

	  /* FIXME: [UNSSA] Re-enable this once the SSA->normal pass is
	     implemented.  Otherwise, this leads to cases where a PHI node
	     contains arguments from different variables, which is
	     something we can't handle with the current unSSA pass.  It may
	     also lead to cases where we re-use the LHS of a computation at
	     a point where more than one version of the LHS is live at the
	     same time.  */
#if 0
	  register_new_def (*def_p, cached_lhs, block_defs_p);
	  ssa_stats.num_re++;
	  bsi_remove (si);
	  return;
#else
	  if (var_is_live (cached_lhs, ann->bb))
	    {
	      register_new_def (*def_p, cached_lhs, block_defs_p);
	      TREE_OPERAND (stmt, 1) = cached_lhs;
	      ann->modified = 1;
	      ssa_stats.num_re++;
	    }
	  else
	    ssa_stats.blocked_optimizations++;
#endif
	}
    }

  /* Step 3.  If the computation wasn't redundant, register its DEF and
     VDEF operands.  */
  if (def_p)
    {
      tree rhs;

      *def_p = make_ssa_name (*def_p, stmt);
      register_new_def (SSA_NAME_VAR (*def_p), *def_p, block_defs_p);

      /* If the RHS of the assignment is a constant or another variable
	 that may be propagated, register it in the CONST_AND_COPIES table.  */
      rhs = TREE_OPERAND (stmt, 1);
      if (may_optimize_p)
	{
	  bool may_copyprop_p =
	    (TREE_CODE (rhs) == SSA_NAME
	     /* Don't copy propagate assignments of the form T = *P.  It
		increases the amount of indirect memory references.  */
	     && ! (TREE_CODE (SSA_NAME_VAR (*def_p)) != INDIRECT_REF
	           && TREE_CODE (SSA_NAME_VAR (rhs)) == INDIRECT_REF)
	     /* FIXME.  For now, don't propagate pointers if they haven't been
	        dereferenced (see update_indirect_ref_vuses).  */
	     && (!POINTER_TYPE_P (TREE_TYPE (rhs)) || indirect_ref (rhs)));

	  if (may_copyprop_p
	      || (TREE_CONSTANT (rhs) && is_simple_val (rhs)))
	    set_value_for (*def_p, rhs, const_and_copies);
	}
    }

  /* Register new virtual definitions made by the statement.  */
  for (i = 0; vdefs && i < VARRAY_ACTIVE_SIZE (vdefs); i++)
    {
      tree vdef = VARRAY_TREE (vdefs, i);
      rewrite_operand (&(VDEF_OP (vdef)));
      VDEF_RESULT (vdef) = make_ssa_name (VDEF_RESULT (vdef), stmt);
      register_new_def (SSA_NAME_VAR (VDEF_RESULT (vdef)), 
			VDEF_RESULT (vdef), block_defs_p);
    }
}


/* Replace the operand pointed by OP_P with its immediate reaching
   definition.  */

static inline void
rewrite_operand (op_p)
     tree *op_p;
{
  tree op, currdef;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*op_p) == SSA_NAME)
    abort ();
#endif

  /* If the operand is an INDIRECT_REF variable, we may need to change its
     base pointer.  Consider the following situation:

	    1	p = q + 1;
	    2	q = q + 1;
	    3	... = (*q);

     The rewriting process will determine that 'q + 1' is redundant at line
     2.  Therefore, the assignment will be eliminated and every reference
     to 'q' needs to be replaced with 'p'.  This includes changing the base
     pointer for every INDIRECT_REF node that uses 'q' as its base pointer.
     So, we need to change *q to be the INDIRECT_REF variable
     associated with the current reaching definition of 'q' (i.e., instead
     of getting the current definition of *q, we actually want to get the
     current definition of *p).  */
  op = *op_p;
  if (TREE_CODE (op) == INDIRECT_REF)
    {
      tree base = currdef_for (TREE_OPERAND (op, 0), false);
      if (base && SSA_NAME_VAR (base) != TREE_OPERAND (op, 0))
	op = indirect_ref (base);
    }

  currdef = currdef_for (op, true);
  *op_p = currdef;
}


/* Register DEF to be a new definition for variable VAR and push VAR's
   current reaching definition into the stack pointed by BLOCK_DEFS_P.  */

static void
register_new_def (var, def, block_defs_p)
     tree var;
     tree def;
     varray_type *block_defs_p;
{
  tree currdef = currdef_for (var, false);

  /* If the current reaching definition is NULL or a constant, push the
     variable itself so that rewrite_blocks knows what variable is
     associated with this NULL reaching def when unwinding the
     *BLOCK_DEFS_P stack.  */
  if (currdef == NULL_TREE)
    VARRAY_PUSH_TREE (*block_defs_p, var);

  /* Push the current reaching definition into *BLOCK_DEFS_P.  This stack is
     later used by rewrite_block to restore the reaching definitions for
     all the variables defined in the block after a recursive visit to all
     its immediately dominated blocks.  */
  VARRAY_PUSH_TREE (*block_defs_p, currdef);

  /* Set the current reaching definition for VAR to be DEF.  */
  set_value_for (var, def, currdefs);
}


/* Update all virtual uses of the indirect reference associated with
   ORIG_PTR to become virtual uses of the indirect reference for pointer
   COPY_PTR.  VUSES is the array with all the virtual uses to be examined.
   Note that this assumes that both ORIG_PTR and COPY_PTR have been renamed
   into SSA form, but entries in VUSES are still in normal form.  */

static void
update_indirect_ref_vuses (orig_ptr, copy_ptr, vuses)
     tree orig_ptr;
     tree copy_ptr;
     varray_type vuses;
{
  size_t i;
  tree orig_indirect = indirect_ref (orig_ptr);
  tree copy_indirect = indirect_ref (copy_ptr);

#if defined ENABLE_CHECKING
  /* FIXME.  We should remove this limitation.  The problem is that if we
     still haven't created a dereference variable for this pointer, we may
     propagate it into a statement that does dereference the pointer.
     That causes all sorts of problems in add_stmt_operand later on,
     because the variable has not been properly renamed.  */
  if (copy_indirect == NULL_TREE)
    abort ();
#endif

  for (i = 0; i < VARRAY_ACTIVE_SIZE (vuses); i++)
    if (orig_indirect == VARRAY_TREE (vuses, i))
      {
	VARRAY_TREE (vuses, i) = copy_indirect;
	return;
      }
}


/* Update all virtual uses of the base pointer for ORIG_INDIRECT with the
   base pointer for COPY_INDIRECT.  VUSES is the array with all the virtual
   uses to be examined.  Note that this assumes that both ORIG_INDIRECT and
   COPY_INDIRECT have been renamed into SSA form, but entries in VUSES are
   still in normal form.  */

static void
update_pointer_vuses (orig_indirect, copy_indirect, vuses)
     tree orig_indirect;
     tree copy_indirect;
     varray_type vuses;
{
  size_t i;
  tree orig_ptr = TREE_OPERAND (SSA_NAME_VAR (orig_indirect), 0);
  tree copy_ptr = TREE_OPERAND (SSA_NAME_VAR (copy_indirect), 0);

  for (i = 0; i < VARRAY_ACTIVE_SIZE (vuses); i++)
    if (orig_ptr == VARRAY_TREE (vuses, i))
      {
	VARRAY_TREE (vuses, i) = copy_ptr;
	return;
      }
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
  memset ((void *) &ssa_stats, 0, sizeof (ssa_stats));

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
  currdefs = htab_create (num_referenced_vars, var_value_hash, var_value_eq,
			  free);

  /* Allocate memory for the AVAIL_EXPRS hash table.  */
  avail_exprs = htab_create (100, avail_expr_hash, avail_expr_eq, NULL);

  /* Allocate memory for the CONST_AND_COPIES hash table.  */
  const_and_copies = htab_create (50, var_value_hash, var_value_eq, free);
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

static tree
currdef_for (v, create_default)
     tree v;
     int create_default;
{
  tree def = get_value_for (v, currdefs);
  if (def == NULL_TREE && create_default)
    {
      def = make_ssa_name (v, empty_stmt_node);
      set_value_for (v, def, currdefs);
    }

  return def;
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


/* Hashing and equality functions for VAR_VALUE_D.  */

static hashval_t
var_value_hash (p)
     const void *p;
{
  return htab_hash_pointer ((const void *)((const struct var_value_d *)p)->var);
}

static int
var_value_eq (p1, p2)
     const void *p1;
     const void *p2;
{
  return ((const struct var_value_d *)p1)->var
	 == ((const struct var_value_d *)p2)->var;
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
  fprintf (stderr, "}");
  fprintf (stderr, ", LIVEIN_BLOCKS: { ");
  EXECUTE_IF_SET_IN_BITMAP (db_p->livein_blocks, 0, i,
			    fprintf (stderr, "%ld ", i));
  fprintf (stderr, "}\n");

  return 1;
}


/* Return the value associated with variable VAR in TABLE.  */

static tree
get_value_for (var, table)
     tree var;
     htab_t table;
{
  struct var_value_d *vm_p, vm;

#if defined ENABLE_CHECKING
  if (!SSA_VAR_P (var))
    abort ();
#endif

  vm.var = var;
  vm_p = (struct var_value_d *) htab_find (table, (void *) &vm);
  return (vm_p) ? vm_p->value : NULL_TREE;
}


/* Associate VALUE to variable VAR in TABLE.  */

static void
set_value_for (var, value, table)
     tree var;
     tree value;
     htab_t table;
{
  struct var_value_d *vm_p, vm;
  void **slot;

#if defined ENABLE_CHECKING
  if (!SSA_VAR_P (var))
    abort ();
#endif

  vm.var = var;
  slot = htab_find_slot (table, (void *) &vm, INSERT);
  if (*slot == NULL)
    {
      vm_p = xmalloc (sizeof *vm_p);
      vm_p->var = var;
      *slot = (void *) vm_p;
    }
  else
    vm_p = (struct var_value_d *) *slot;

  vm_p->value = value;
}


/* Search for an existing instance of STMT in the AVAIL_EXPRS table.  If
   found, return its LHS. Otherwise insert STMT in the table and return
   NULL_TREE.

   Also, when an expression is first inserted in the AVAIL_EXPRS table, it
   is also added to the stack pointed by BLOCK_AVAIL_EXPRS_P, so that they
   can be removed when we finish processing this block and its children.

   NOTE: This function assumes that STMT is a MODIFY_EXPR node that
	 contains no CALL_EXPR on its RHS and makes no volatile nor
	 aliased references.  */

static tree
lookup_avail_expr (stmt, block_avail_exprs_p)
     tree stmt;
     varray_type *block_avail_exprs_p;
{
  void **slot;
  tree rhs;

  /* Don't bother remembering constant assignments, type cast expressions
     and copy operations.  Constants and copy operations are handled by the
     constant/copy propagator in rewrite_stmt.  */
  rhs = TREE_OPERAND (stmt, 1);
  if (TREE_CONSTANT (rhs)
      || TREE_CODE (rhs) == SSA_NAME
      || is_simple_cast (rhs))
    return NULL_TREE;

  slot = htab_find_slot (avail_exprs, stmt, INSERT);
  if (*slot == NULL)
    {
      *slot = (void *) stmt;
      VARRAY_PUSH_TREE (*block_avail_exprs_p, stmt);
      return NULL_TREE;
    }

  /* Return the LHS of the assignment so that it can be used as the current
     definition of another variable.  */
  return TREE_OPERAND ((tree) *slot, 0);
}


/* Hashing and equality functions for AVAIL_EXPRS.  The table stores
   MODIFY_EXPR statements.  We compute a value number for expressions using
   the code of the expression and the SSA numbers of its operands.  */

static hashval_t
avail_expr_hash (p)
     const void *p;
{
  hashval_t val;
  tree rhs;
  size_t i;
  varray_type ops;
  tree stmt = (tree) p;

  rhs = TREE_OPERAND (stmt, 1);
  val = (hashval_t) TREE_CODE (rhs);

  /* Add the SSA version numbers of every use operand.  */
  ops = use_ops (stmt);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    {
      tree op = *((tree *) VARRAY_GENERIC_PTR (ops, i));
      if (TREE_CODE (op) == SSA_NAME)
	val += (hashval_t) SSA_NAME_VERSION (op);
      else
	val += htab_hash_pointer (op);
    }

  /* Add the SSA version numbers of every vuse operand.  This is important
     because compound variables like arrays are not renamed in the
     operands.  Rather, the rename is done on the virtual variable
     representing all the elements of the array.  */
  ops = vuse_ops (stmt);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    val += (hashval_t) SSA_NAME_VERSION (VARRAY_TREE (ops, i));

  return val;
}


static int
avail_expr_eq (p1, p2)
     const void *p1;
     const void *p2;
{
  tree s1, s2, rhs1, rhs2;

  s1 = (tree) p1;
  rhs1 = TREE_OPERAND (s1, 1);

  s2 = (tree) p2;
  rhs2 = TREE_OPERAND (s2, 1);

  return (TREE_CODE (rhs1) == TREE_CODE (rhs2)
	  && simple_cst_equal (rhs1, rhs2) == 1);
}


/* Return the set of blocks where variable VAR is defined and the blocks
   where VAR is live on entry (livein).  */

static struct def_blocks_d *
get_def_blocks_for (var)
     tree var;
{
  struct def_blocks_d dm;

  dm.var = var;
  return (struct def_blocks_d *) htab_find (def_blocks, (void *) &dm);
}

#if 1
/* Return true if the variable VAR is live at this point of the
   dominator tree walk.  This means that the current reaching definition
   for VAR is itself and that VAR is livein at basic block BB.

   FIXME: [UNSSA] This will not be necessary when the unSSA pass is
   implemented.  */

static bool
var_is_live (var, bb)
     tree var;
     basic_block bb;
{
  basic_block def_bb;
  struct def_blocks_d *def_map;
  tree real_var = SSA_NAME_VAR (var);

  if (currdef_for (real_var, false) != var)
    {
      ssa_stats.blocked_by_life_crossing++;
      return false;
    }

  /* This is gross, but since it's temporary, close your eyes.  It's needed
     to avoid miscompiling java/jcf-write.c:generate_classfile, where the
     fully pruned SSA form is not inserting PHI nodes in the main loop of
     the function for variable 'ptr'.  This makes two versions of 'ptr'
     live at the same time.

     If there are any blocks between VAR's definition block and BB where
     VAR is defined again, then two versions of VAR are live at the same
     time.  */
  def_map = get_def_blocks_for (real_var);
  def_bb = bb_for_stmt (SSA_NAME_DEF_STMT (var));
  if (def_bb && bitmap_first_set_bit (def_map->def_blocks) >= 0)
    {
      int i;
      EXECUTE_IF_SET_IN_BITMAP (def_map->def_blocks, def_bb->index + 1, i,
	return ((i < bb->index) ? false : true));
    }

  return true;
}
#endif
