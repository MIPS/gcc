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
#include "langhooks.h"
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
#include "tree-ssa-live.h"

/* This file builds the SSA form for a function as described in:

   R. Cytron, J. Ferrante, B. Rosen, M. Wegman, and K. Zadeck. Efficiently
   Computing Static Single Assignment Form and the Control Dependence
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
  bitmap phi_insertion_points;
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

/* Used to hold all the components requires to do SSA PHI elimination.  */

typedef struct _elim_graph
{
  /* Size of the elimination vectors.  */
  int size;
  /* Number of nodes entered into the elimination graph.  */
  int num_nodes;
  /* The actual nodes in the elimination graph.  */
  tree *nodes;
  /* The predecessor and successor list.  */
  bitmap *pred;
  bitmap *succ;

  /* Visited vector.  */
  sbitmap visited;
  /* Stack for visited nodes.  */
  int *stack;
  int top_of_stack;
  
  /* The variable partition map.  */
  var_map map;
  /* edge being eliminated by this graph.  */
  edge e;
} *elim_graph;

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
static void compute_global_livein	PARAMS ((varray_type));
static void set_def_block		PARAMS ((tree, basic_block));
static void set_livein_block		PARAMS ((tree, basic_block));
static void insert_phi_nodes		PARAMS ((bitmap *, sbitmap));
static void insert_phis_for_deferred_variables PARAMS ((varray_type));
static void rewrite_block		PARAMS ((basic_block, tree));
static void rewrite_stmt		PARAMS ((block_stmt_iterator,
						 varray_type *,
						 varray_type *));
static inline void rewrite_operand	PARAMS ((tree *));
static void register_new_def		PARAMS ((tree, tree, varray_type *));
static void update_indirect_ref_vuses	PARAMS ((tree, tree, varray_type));
static void update_pointer_vuses	PARAMS ((tree, tree, varray_type));
static void insert_phi_nodes_for	PARAMS ((tree, bitmap *, varray_type));
static tree remove_annotations_r	PARAMS ((tree *, int *, void *));
static tree get_reaching_def		PARAMS ((tree));
static tree get_value_for		PARAMS ((tree, htab_t));
static void set_value_for		PARAMS ((tree, tree, htab_t));
static hashval_t def_blocks_hash	PARAMS ((const void *));
static int def_blocks_eq		PARAMS ((const void *, const void *));
static hashval_t var_value_hash		PARAMS ((const void *));
static int var_value_eq			PARAMS ((const void *, const void *));
static void def_blocks_free		PARAMS ((void *));
static int debug_def_blocks_r		PARAMS ((void **, void *));
static tree lookup_avail_expr		PARAMS ((tree, varray_type *));
static tree get_eq_expr_value		PARAMS ((tree));
static hashval_t avail_expr_hash	PARAMS ((const void *));
static int avail_expr_eq		PARAMS ((const void *, const void *));
static struct def_blocks_d *get_def_blocks_for PARAMS ((tree));
static void htab_statistics		PARAMS ((FILE *, htab_t));

static tree create_temp			PARAMS ((tree));
static void insert_copy_on_edge		PARAMS ((edge, tree, tree));
static elim_graph new_elim_graph	PARAMS ((int));
static void delete_elim_graph		PARAMS ((elim_graph));
static void clear_elim_graph		PARAMS ((elim_graph));
static void eliminate_name		PARAMS ((elim_graph, tree));
static int eliminate_build		PARAMS ((elim_graph, basic_block, int));
static void elim_forward		PARAMS ((elim_graph, int));
static int elim_unvisited_predecessor	PARAMS ((elim_graph, int));
static void elim_backward		PARAMS ((elim_graph, int));
static void elim_create			PARAMS ((elim_graph, int));
static void eliminate_phi		PARAMS ((edge, int, elim_graph));
static void coalesce_ssa_name		PARAMS ((var_map));
static void assign_vars			PARAMS ((var_map));
static inline void set_if_valid		PARAMS ((var_map, sbitmap, tree));
static inline void add_conflicts_if_valid	PARAMS ((root_var_p, conflict_graph, var_map, sbitmap, tree));

/* FIXME: [UNSSA] Remove once the real unSSA pass is implemented.  */
#if 1
static bool var_is_live			PARAMS ((tree, basic_block));
#endif

/* Return nonzero if RHS may be copy propagated into subsequent
   uses of LHS.  */
#define MAY_COPYPROP_P(LHS, RHS)					\
    (TREE_CODE (RHS) == SSA_NAME					\
     /* Don't copy propagate assignments of the form T = *P.  It	\
        increases the amount of indirect memory references.  */		\
     && ! (TREE_CODE (SSA_NAME_VAR (LHS)) != INDIRECT_REF		\
	   && TREE_CODE (SSA_NAME_VAR (RHS)) == INDIRECT_REF)		\
     /* FIXME.  For now, don't propagate pointers if they haven't been	\
        dereferenced (see update_indirect_ref_vuses).  */		\
     && (!POINTER_TYPE_P (TREE_TYPE (RHS)) || indirect_ref (RHS))	\
     && ! var_ann (SSA_NAME_VAR (LHS))->occurs_in_abnormal_phi		\
     && ! var_ann (SSA_NAME_VAR (RHS))->occurs_in_abnormal_phi)


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
  compute_may_aliases (fndecl);

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
  rewrite_block (ENTRY_BLOCK_PTR, NULL_TREE);
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

/* Compute global livein information for one or more objects.

   DEF_BLOCKS is a varray of struct def_blocks_d pointers, one for
   each object of interest.

   Each of the struct def_blocks_d entries points to a set of
   blocks where the variable is locally live at the start of
   a block and the set of blocks where the variable is defined.

   Note: This routine augments the existing local livein information
   to include global livein.  Ie, it modifies the underlying LIVEIN
   bitmap for each object. 

   We can get great speedups by performing life analysis for several
   objects in parallel, but this design also allows for per-variable
   life computation by having a varray with a single element.  */

static void
compute_global_livein (def_maps)
     varray_type def_maps;
{
  basic_block bb, *worklist, *tos;
  bitmap in_worklist = BITMAP_XMALLOC ();
  struct def_blocks_d *def_map;
  unsigned int n_elements = VARRAY_ACTIVE_SIZE (def_maps);
  unsigned int i;

  tos = worklist
    = (basic_block *) xmalloc (sizeof (basic_block) * (n_basic_blocks + 1));

  /* Build a bitmap of all the blocks which belong in the worklist.  */
  for (i = 0; i < n_elements; i++)
    {
      def_map = VARRAY_GENERIC_PTR (def_maps, i);
      bitmap_a_or_b (in_worklist, in_worklist, def_map->livein_blocks);
    }
    
  /* Initialize the worklist itself.  */
  FOR_EACH_BB (bb)
    {
      if (bitmap_bit_p (in_worklist, bb->index))
	*tos++ = bb;
    }

  /* Iterate until the worklist is empty.  */
  while (tos != worklist)
    {
      edge e;

      /* Pull a block off the worklist.  */
      bb = *--tos;
      bitmap_clear_bit (in_worklist, bb->index);

      /* For each predecessor block.  */
      for (e = bb->pred; e; e = e->pred_next)
	{
	  basic_block pred = e->src;
	  int pred_index = pred->index;

	  /* None of this is necessary for the entry block.  */
	  if (pred != ENTRY_BLOCK_PTR)
	    {
	      /* Update livein_blocks for each element in the 
		 def_maps vector.  */
	      for (i = 0; i < n_elements; i++)
		{
		  def_map = VARRAY_GENERIC_PTR (def_maps, i);

		  if (bitmap_bit_p (def_map->livein_blocks, bb->index)
		      && ! bitmap_bit_p (def_map->livein_blocks, pred_index)
		      && ! bitmap_bit_p (def_map->def_blocks, pred_index))
		    {
		      bitmap_set_bit (def_map->livein_blocks, pred_index);

		      /* If this block is not in the worklist, then add
			 it to the worklist.  */
		      if (! bitmap_bit_p (in_worklist, pred_index))
			{
			  *tos++ = pred;
			  bitmap_set_bit (in_worklist, pred_index);
			}
		    }
		}
	    }
	}
    }
  free (worklist);
  BITMAP_XFREE (in_worklist);
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
	  get_stmt_operands (stmt);

	  /* If a variable is used before being set, then the variable
	     is live across a block boundary, so add it to NONLOCAL_VARS.  */
	  ops = use_ops (stmt);
	  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
	    {
	      tree *use = VARRAY_GENERIC_PTR (ops, i);
	      size_t uid = var_ann (*use)->uid;

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
	      size_t uid = var_ann (use)->uid;

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
	      size_t uid = var_ann (vdef_op)->uid;

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
  varray_type def_maps;

  timevar_push (TV_TREE_INSERT_PHI_NODES);

  VARRAY_GENERIC_PTR_INIT (def_maps, HOST_BITS_PER_WIDE_INT, "deferred_vars");

  /* Array WORK_STACK is a stack of CFG blocks.  Each block that contains
     an assignment or PHI node will be pushed to this stack.  */
  VARRAY_BB_INIT (work_stack, last_basic_block, "work_stack");

  /* Iterate over all referenced variables in the function.  For each
     variable, add to the work list all the blocks that have a definition
     for the variable.  PHI nodes will be added to the dominance frontier
     blocks of each definition block.  */
  for (i = 0; i < num_referenced_vars; i++)
    {
      /* If we have queued enough variables, then drain the queue.  */
      if (VARRAY_ACTIVE_SIZE (def_maps) == HOST_BITS_PER_WIDE_INT)
	insert_phis_for_deferred_variables (def_maps);

      if (TEST_BIT (globals, i))
	insert_phi_nodes_for (referenced_var (i), dfs, def_maps);
    }

  if (VARRAY_ACTIVE_SIZE (def_maps) != 0)
    insert_phis_for_deferred_variables (def_maps);

  work_stack = NULL;
  def_maps = NULL;
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
      blocks above BB in the dominator tree.

   EQ_EXPR_VALUE is an assignment expression created when BB's immediate
   dominator ends in a COND_EXPR statement whose predicate is of the form
   'VAR == VALUE', where VALUE may be another variable or a constant. 
   This is used to propagate VALUE on the THEN_CLAUSE of that conditional.
   This assignment is inserted in CONST_AND_COPIES so that the copy and
   constant propagator can find more propagation opportunities.  */

static void
rewrite_block (bb, eq_expr_value)
     basic_block bb;
     tree eq_expr_value;
{
  edge e;
  varray_type block_defs, block_avail_exprs;
  bitmap children;
  unsigned long i;
  block_stmt_iterator si;
  tree phi;
  tree prev_value = NULL_TREE;

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

  if (eq_expr_value)
    {
      prev_value = get_value_for (TREE_OPERAND (eq_expr_value, 0),
				  const_and_copies);
      set_value_for (TREE_OPERAND (eq_expr_value, 0),
		     TREE_OPERAND (eq_expr_value, 1),
		     const_and_copies);
    }

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
	  tree currdef = get_reaching_def (SSA_NAME_VAR (PHI_RESULT (phi)));
	  add_phi_arg (phi, currdef, e);
	}
    }

  /* Step 4.  Recursively search the dominator children of BB.  */
  children = dom_children (bb);
  if (children)
    {
      if (bb->flags & BB_CONTROL_EXPR)
	{
	  tree last = last_stmt (bb);
	  EXECUTE_IF_SET_IN_BITMAP (children, 0, i,
	    {
	      if (BASIC_BLOCK (i)->pred->flags & EDGE_TRUE_VALUE)
		rewrite_block (BASIC_BLOCK (i), get_eq_expr_value (last));
	      else
		rewrite_block (BASIC_BLOCK (i), NULL_TREE);
	    });
	}
      else
	{
	  EXECUTE_IF_SET_IN_BITMAP (children, 0, i,
	    rewrite_block (BASIC_BLOCK (i), NULL_TREE));
	}
    }

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

  if (eq_expr_value)
    {
      struct var_value_d vm;
      vm.var = TREE_OPERAND (eq_expr_value, 0);
      if (prev_value)
	set_value_for (vm.var, prev_value, const_and_copies);
      else
	htab_remove_elt (const_and_copies, &vm);
    }
}


/* This function will create a temporary for a partition based on the
   type of the variable which already represents a partition.  */

static tree
create_temp (t)
     tree t;
{
  tree tmp;
  const char *name = NULL;
  tree type;

  if (TREE_CODE (t) == SSA_NAME)
    t = SSA_NAME_VAR (t);
 
  if (TREE_CODE (t) != VAR_DECL 
      && TREE_CODE (t) != PARM_DECL
      && TREE_CODE (t) != INDIRECT_REF)
    abort ();

  if (TREE_CODE (t) == INDIRECT_REF)
    {
      if (TREE_CODE (TREE_OPERAND (t, 0)) != VAR_DECL 
          && TREE_CODE (TREE_OPERAND (t, 0)) != PARM_DECL)
        abort ();
      type = TREE_TYPE (TREE_OPERAND (t,0));
      tmp = DECL_NAME (TREE_OPERAND(t, 0));
      if (tmp)
	name = IDENTIFIER_POINTER (tmp);
    }
  else
    {
      type = TREE_TYPE (t);
      tmp = DECL_NAME (t);
      if (tmp)
	name = IDENTIFIER_POINTER (tmp);
    }

  if (name == NULL)
    name = "temp";
  tmp = create_tmp_var (type, name);
  create_var_ann (tmp);
  return tmp;
}


/* This helper function fill insert a copy from one variable to another
   on the specified edge.  */

static void
insert_copy_on_edge (e, dest, src)
     edge e;
     tree dest, src;
{
  tree copy;

  copy = build (MODIFY_EXPR, TREE_TYPE (dest), dest, src);
  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_ssa_dump_file,
	       "Inserting a copy on edge BB%d->BB%d :",
	       e->src->index,
	       e->dest->index);
      print_generic_expr (tree_ssa_dump_file, copy, tree_ssa_dump_flags);
      fprintf (tree_ssa_dump_file, "\n");
    }
  bsi_insert_on_edge (e, copy);
}

/* --------------------------------------------------------------------- */
/* Create an elimination graph and associated data structures.  */

static elim_graph
new_elim_graph (size)
     int size;
{
  int x;
  elim_graph g = (elim_graph) xmalloc (sizeof (struct _elim_graph));

  g->size = size;
  g->nodes = (tree *) xmalloc (sizeof (tree) * size);

  g->pred = (bitmap *) xmalloc (sizeof (bitmap) * size);
  g->succ= (bitmap *) xmalloc (sizeof (bitmap) * size);
  for (x = 0; x < size; x++)
    {
      g->pred[x] = BITMAP_XMALLOC ();
      g->succ[x] = BITMAP_XMALLOC ();
    }
  g->visited = sbitmap_alloc (size);
  g->stack = (int *) xmalloc (sizeof (int) * size);
  
  return g;
}

static void
clear_elim_graph (g)
     elim_graph g;
{
  int x;
  int size = g->size;

  g->num_nodes = 0;
  memset (g->nodes, 0, sizeof (tree) * size);
  for (x = 0; x < size; x++)
    {
      bitmap_clear (g->pred[x]);
      bitmap_clear (g->succ[x]);
    }
}

/* Delete an elimination graph.  */
static void
delete_elim_graph (g)
     elim_graph g;
{
  int x;
  free (g->stack);
  sbitmap_free (g->visited);

  for (x = g->size - 1; x >= 0 ; x--)
    {
      BITMAP_XFREE (g->succ[x]);
      BITMAP_XFREE (g->pred[x]);
    }
  free (g->succ);
  free (g->pred);

  free (g->nodes);
  free (g);
}

/* The following procedures implement the out of SSA algorithm detailed in 
   the Morgan Book pages 176-186.  */


/* Add T to the elimination graph.  */

static void
eliminate_name (g, T)
     elim_graph g;
     tree T;
{
  int version = var_to_partition (g->map, T);

  /* If this is the first time a node is added, clear the graph.
     Delaying it until here prevents clearing all the vectors
     until it is known for sure that they are going to be used.  */

  if (g->num_nodes == 0)
    clear_elim_graph (g);

  if (g->nodes[version] == NULL)
    {
      g->nodes[version] = T;
      g->num_nodes++;
    }
}

/* Build the auxillary graph.  */

static int
eliminate_build (g, B, i)
     elim_graph g;
     basic_block B;
     int i;
{
  tree phi;
  tree T0, Ti;
  int p0, pi;
  int edges = 0;

  g->num_nodes = 0;
  
  for (phi = phi_nodes (B); phi; phi = TREE_CHAIN (phi))
    {
      T0 = var_to_partition_to_var (g->map, PHI_RESULT (phi));
      Ti = var_to_partition_to_var (g->map, PHI_ARG_DEF (phi, i));
      if (T0 != Ti)
        {
	  eliminate_name (g, T0);
	  eliminate_name (g, Ti);
	  p0 = var_to_partition (g->map, T0);
	  pi = var_to_partition (g->map, Ti);
	  bitmap_set_bit (g->pred[pi], p0);
	  bitmap_set_bit (g->succ[p0], pi);
	  edges++;
	}
    }
  return edges;
}

/* Push successors onto the stack depth first.  */

static void 
elim_forward (g, T)
     elim_graph g;
     int T;
{
  int S;
  SET_BIT (g->visited, T);
  EXECUTE_IF_SET_IN_BITMAP (g->succ[T], 0, S,
    {
      if (!TEST_BIT (g->visited, S))
        elim_forward (g, S);
    });
  g->stack[g->top_of_stack] = T;
  g->top_of_stack++;
}


/* Are there unvisited predecessors?  */

static int
elim_unvisited_predecessor (g, T)
     elim_graph g;
     int T;
{
  int P;
  EXECUTE_IF_SET_IN_BITMAP (g->pred[T], 0, P,
    {
      if (!TEST_BIT (g->visited, P))
        return 1;
    });
  return 0;
}

/* Process predecessors first, and insert a copy.  */

static void
elim_backward (g, T)
     elim_graph g;
     int T;
{
  int P;
  SET_BIT (g->visited, T);
  EXECUTE_IF_SET_IN_BITMAP (g->pred[T], 0, P,
    {
      if (!TEST_BIT (g->visited, P))
        elim_backward (g, P);
      insert_copy_on_edge (g->e, 
			   partition_to_var (g->map, P), 
			   partition_to_var (g->map, T));
    });
}

/* Check for a SCR, and create a temporary if there is one, and break
   the cycle. Then issue the copies. Otherwise, simply insert the
   required copies.  */

static void 
elim_create (g, T)
     elim_graph g;
     int T;
{
  tree U;
  int P, S;

  if (elim_unvisited_predecessor (g, T))
    {
      U = create_temp (partition_to_var (g->map, T));
      insert_copy_on_edge (g->e, U, partition_to_var (g->map, T));
      EXECUTE_IF_SET_IN_BITMAP (g->pred[T], 0, P,
	{
	  if (!TEST_BIT (g->visited, P))
	    {
	      elim_backward (g, P);
	      insert_copy_on_edge (g->e, partition_to_var (g->map, P), U);
	    }
	});
    }
  else
    {
      EXECUTE_IF_SET_IN_BITMAP (g->succ[T], 0, S,
	{
	  SET_BIT (g->visited, T);
	  insert_copy_on_edge (g->e, 
			       partition_to_var (g->map, T), 
			       partition_to_var (g->map, S));
	});
      bitmap_clear (g->succ[T]);
    }
  
}

/* Eliminate all the phi nodes on this edge.  */

static void
eliminate_phi (e, i, g)
     edge e;
     int i;
     elim_graph g;
{
  tree phi;
  int num_phi = 0;
  int num_nodes = 0;
  int x, limit;
  basic_block B = e->dest;

#if defined ENABLE_CHECKING
  if (i == -1)
    abort ();
#endif

  for (phi = phi_nodes (B); phi; phi = TREE_CHAIN (phi))
    {
      num_phi++;
    }

  num_nodes = num_var_partitions (g->map);
  g->e = e;

  x = eliminate_build (g, B, i);

  if (g->num_nodes == 0)
    return;

  sbitmap_zero (g->visited);
  g->top_of_stack = 0;

  limit = g->num_nodes;
  for (x = 0; limit; x++)
     if (g->nodes[x])
       {
         limit--;
	 if (!TEST_BIT (g->visited, x))
	   elim_forward (g, x);
       }
   
  sbitmap_zero (g->visited);
  while (g->top_of_stack > 0)
    {
      x = g->stack[--(g->top_of_stack)];
      if (!TEST_BIT (g->visited, x))
        elim_create (g, x);
    }
}

/* Set the bit for a partition index if the variable is in a partition.  */

static inline void 
set_if_valid (map, vec, var)
     var_map map;
     sbitmap vec;
     tree var;
{ 
  int p = var_to_partition (map, var);
  if (p != NO_PARTITION)
    SET_BIT (vec, p);
}

/* If a variable is in a partition, and its not already live, add a 
   conflict between it and any other live partition.  Reset the live bit.  */

static inline void 
add_conflicts_if_valid (rv, graph, map, vec, var)
     root_var_p rv;
     conflict_graph graph;
     var_map map;
     sbitmap vec;
     tree var;
{ 
  int p, y;
  p = var_to_partition (map, var);
  if (p != NO_PARTITION)
    { 
      RESET_BIT (vec, p);
      for (y = first_root_var_partition (rv, find_root_var (rv, p));
	   y != ROOT_VAR_NONE;
	   y = next_root_var_partition (rv, y))
	{
	  if (TEST_BIT (vec, y))
	    conflict_graph_add (graph, p, y);
	}
    }
}

/* Reduce the number of live ranges in the var_map. The only partitions
   which are associated with actual variables are those which are forced
   to be coalesced for various reason. (live on entry, live across abnormal 
   edges, etc.)  */

static void
coalesce_ssa_name (map)
     var_map map;
{
  int num, x, y, z;
  conflict_graph graph;
  basic_block bb;
  varray_type stmt_stack, ops;
  tree stmt;
  sbitmap live;
  tree *var_p, var, tmp, phi;
  root_var_p rv;
  tree_live_info_p liveinfo;
  edge e;
  var_ann_t ann;

  if (num_var_partitions (map) <= 1)
    return;
  
  liveinfo = calculate_live_on_entry (map);
  calculate_live_on_exit (liveinfo);
  graph = conflict_graph_new (num_var_partitions (map));
  live = sbitmap_alloc (num_var_partitions (map));
  rv = init_root_var (map);

  /* Build a conflict graph.  */

  FOR_EACH_BB (bb)
    {
      /* Start with live on exit temporaries.  */
      sbitmap_copy (live, live_on_exit (liveinfo, bb));

      FOR_EACH_STMT_IN_REVERSE (stmt_stack, bb, stmt)
        {
	  get_stmt_operands (stmt);

	  var_p = def_op (stmt);
	  if (var_p)
	    {
	      add_conflicts_if_valid (rv, graph, map, live, *var_p);
	    }

	  ops = vdef_ops (stmt);
	  num = ((ops) ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (x = 0; x < num; x++)
	    {
	      var = VDEF_RESULT (VARRAY_TREE (ops, x));
	      add_conflicts_if_valid (rv, graph, map, live, var);
	    }

	  
	  ops = use_ops (stmt);
	  num = ((ops) ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (x = 0; x < num; x++)
	    {
	      var_p = VARRAY_GENERIC_PTR (ops, x);
	      set_if_valid (map, live, *var_p);
	    }
  
	  ops = vuse_ops (stmt);
	  num = ((ops) ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (x = 0; x < num; x++)
	    {
	      var = VARRAY_TREE (ops, x);
	      set_if_valid (map, live, var);
	    }
 
	  ops = vdef_ops (stmt);
	  num = ((ops) ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (x = 0; x < num; x++)
	    {
	      var = VDEF_OP (VARRAY_TREE (ops, x));
	      set_if_valid (map, live, var);
	    }
	}

      /* Anything which is still live at this point interferes.  */

      EXECUTE_IF_SET_IN_SBITMAP (live, 0, x,
        {
	  for (y = first_root_var_partition (rv, find_root_var (rv, x));
	       y != ROOT_VAR_NONE;
	       y = next_root_var_partition (rv, y))
	    {
	      if (x != y && TEST_BIT (live, y))
		conflict_graph_add (graph, x, y);
	    }

	});
    }

  /* First, coalesce all live on entry variables to their root variable. 
     This will ensure the first use is coming from the right memory location. */

  sbitmap_zero (live);

  /* Set 'live' vector to indicate live on entry partitions.  */

  num = num_var_partitions (map);
  for (x = 0 ; x < num; x++)
    for (e = ENTRY_BLOCK_PTR->succ; e; e = e->succ_next)
      if (e->dest != EXIT_BLOCK_PTR)
	{
	  if (TEST_BIT (live_entry_blocks (liveinfo, x), e->dest->index))
	    SET_BIT (live, x);
	}
  delete_tree_live_info (liveinfo);

  /* Assign root variable as partition representative for each live on entry
     partition.  */
  EXECUTE_IF_SET_IN_SBITMAP (live, 0, x, 
    {
      var = root_var (rv, find_root_var (rv, x));
      ann = var_ann (var);
      /* If these aren't already coalesced...  */
      if (partition_to_var (map, x) != var)
	{
	  if (ann->out_of_ssa_tag)
	    {
	      /* This root variable has already been assigned to another
		 partition which is not coalesced with this one.  */
	      abort ();
	    }

	  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_ssa_dump_file, "Must coalesce ");
	      print_generic_expr (tree_ssa_dump_file, 
				  partition_to_var (map, x), 
				  TDF_SLIM);
	      fprintf (tree_ssa_dump_file, " with the root variable ");
	      print_generic_expr (tree_ssa_dump_file, var, TDF_SLIM);
	      fprintf (tree_ssa_dump_file, ".\n");
	    }

	  change_partition_var (map, var, x);
	}
    });

  sbitmap_free (live);

  /* Code cannot be inserted on abnormal edges. Look for all abnormal 
     edges, and coalesce any PHI results with their arguments across 
     that edge.  */

  FOR_EACH_BB (bb)
    for (e = bb->succ; e; e = e->succ_next)
      if (e->dest != EXIT_BLOCK_PTR && e->flags & EDGE_ABNORMAL)
	for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
	  {
	    /* Visit each PHI on the destination side of this abnormal
	       edge, and attempt to coalesce the argument with the result.  */
	    var = PHI_RESULT (phi);
	    x = var_to_partition (map, var);
	    y = phi_arg_from_edge (phi, e);
	    if (y == -1)
	      abort ();
	    tmp = PHI_ARG_DEF (phi, y);
	    y = var_to_partition (map, tmp);
	    if (x == NO_PARTITION || y == NO_PARTITION)
	      abort ();
	    if (find_root_var (rv, x) != find_root_var (rv, y))
	      {
		/* FIXME. If the root variables are not the same, we 
		   can't coalesce the partitions, but maybe we can create
		   a new version of the PHI_RESULT variable, issue 
		   a copy in the DEST block instead, and use it in the 
		   PHI node. Adding a new partition/version at this point
		   is really a bad idea, so for now, PUNT!.  */
		error ("Different root vars across an abnormal edge\n");
	      }

	    if (x != y)
	      {
		if (!conflict_graph_conflict_p (graph, x, y))
		  {
		    /* NOw map the partitions back to their real variables.  */
		    var = partition_to_var (map, x);
		    tmp = partition_to_var (map, y);
		    if (tree_ssa_dump_file 
			&& (tree_ssa_dump_flags & TDF_DETAILS))
		      {
			fprintf (tree_ssa_dump_file , "ABNORMAL: Coalescing ");
			print_generic_expr (tree_ssa_dump_file, 
					    var, 
					    TDF_SLIM);
			fprintf (tree_ssa_dump_file , " and ");
			print_generic_expr (tree_ssa_dump_file, 
					    tmp, 
					    TDF_SLIM);
			fprintf (tree_ssa_dump_file , " over abnormal edge.\n");
		      }
		    var_union (map, var, tmp);
		    conflict_graph_merge_regs (graph, x, y);
		  }
		else
		  error ("Vars Conflict across an abnormal edge\n");
	      }
	  }

    if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
      {
        dump_var_map (tree_ssa_dump_file, map);
      }


  for (x = 0; x < num_root_vars (rv); x++)
    {
      while (first_root_var_partition (rv, x) != ROOT_VAR_NONE)
        {
	  /* Coalesce first partition with everything that doesn't conflict.  */
	  y = first_root_var_partition (rv, x);
	  remove_root_var_partition (rv, x, y);
	  var = partition_to_var (map, y);
	  for (z = next_root_var_partition (rv, y); 
	       z != ROOT_VAR_NONE; 
	       z = next_root_var_partition (rv, z))
	    {
	      tmp = partition_to_var (map, z);
	      /* If partitions are already merged, don't check for conflict.  */
	      if (tmp == var)
	        remove_root_var_partition (rv, x, z);
	      else
		if (!conflict_graph_conflict_p (graph, y, z))
		  {
		    var_union (map, var, tmp);
		    remove_root_var_partition (rv, x, z);
		    conflict_graph_merge_regs (graph, y, z);
		  }
	    }
	}
    }

  delete_root_var (rv);
  conflict_graph_delete (graph);
}

/* Take the ssa-name var_map, and assign real variables to each partition.  */

static void
assign_vars (map)
     var_map map;
{
  int x, i, num;
  tree t, var;
  var_ann_t ann;
  root_var_p rv;

  rv = init_root_var (map);
  if (!rv) 
    return;

  /* Coalescing may already have forced some partitions to their root 
     variable. Find these and tag them.  */

  num = num_var_partitions (map);
  for (x = 0; x < num; x++)
    {
      var = partition_to_var (map, x);
      if (TREE_CODE (var) != SSA_NAME)
	{
	  /* Coalescing will already have verified that more than one
	     partition doesn't have the same root variable. Simply marked
	     the variable as assigned.  */
	  ann = var_ann (var);
	  ann->out_of_ssa_tag = 1;
	  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
	    {
	      fprintf(tree_ssa_dump_file, "partition %d has variable ", x);
	      print_generic_expr (tree_ssa_dump_file, var, TDF_SLIM);
	      fprintf(tree_ssa_dump_file, " assigned to it.\n");
	    }

	}
    }

  num = num_root_vars (rv);
  for (x = 0; x < num; x++)
    {
      var = root_var (rv, x);
      ann = var_ann (var);
      for (i = first_root_var_partition (rv, x);
	   i != ROOT_VAR_NONE;
	   i = next_root_var_partition (rv, i))
	{
	  t = partition_to_var (map, i);

	  if (t == var || TREE_CODE (t) != SSA_NAME)
	    continue;
	  
	  if (!ann->out_of_ssa_tag)
	    {
	      change_partition_var (map, var, i);
	      continue;
	    }

	  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_ssa_dump_file, "Overlap :  '");
	      print_generic_expr (tree_ssa_dump_file, t, TDF_SLIM);
	      fprintf (tree_ssa_dump_file, "'  conflicts with  '");
	      print_generic_expr (tree_ssa_dump_file, var, TDF_SLIM);
	    }

	  /* FIXME. Since we still don't have passes that create overlapping 
	  live ranges, the code above should've coalesced all the versions of
	  the variable together.  */
	  abort ();

	  var = create_temp (t);
	  change_partition_var (map, var, i);
	  ann = var_ann (var);

	  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_ssa_dump_file, "'     New temp:  '");
	      print_generic_expr (tree_ssa_dump_file, var, TDF_SLIM);
	      fprintf (tree_ssa_dump_file, "'\n");
	    }
	}
    }

  delete_root_var (rv);
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
  edge e;
  var_map map;
  tree phi, next;
  elim_graph g;
  int repeat;

  timevar_push (TV_TREE_SSA_TO_NORMAL);

  tree_ssa_dump_file = dump_begin (TDI_optimized, &tree_ssa_dump_flags);

  map = create_ssa_var_map ();

  /* Shrink the map to include only referenced variables.  Exclude variables
     which have only one SSA version since there is nothing to coalesce.  */
  compact_var_map (map, VARMAP_NO_SINGLE_DEFS);

  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
    dump_var_map (tree_ssa_dump_file, map);

  coalesce_ssa_name (map);

  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
    {
      fprintf(tree_ssa_dump_file, "After Coalescing:\n");
      dump_var_map (tree_ssa_dump_file, map);
    }

  /* This is the final var list, so assign real variables to the different
     partitions.  Include single reference vars this time. */

  compact_var_map (map, VARMAP_NORMAL);
  assign_vars (map);

  if (tree_ssa_dump_file && (tree_ssa_dump_flags & TDF_DETAILS))
    {
      fprintf(tree_ssa_dump_file, "After Root variable replacement:\n");
      dump_var_map (tree_ssa_dump_file, map);
    }

  g = new_elim_graph (map->num_partitions);
  g->map = map;
  FOR_EACH_BB (bb)
    {
      for (si = bsi_start (bb); !bsi_end_p (si); )
	{
	  size_t i, num_ops;
	  varray_type ops;
	  tree stmt = bsi_stmt (si);
	  tree *use_p = NULL;
	  int remove = 0, is_copy = 0;

	  get_stmt_operands (stmt);

	  if (TREE_CODE (stmt) == MODIFY_EXPR 
	      && (TREE_CODE (TREE_OPERAND (stmt, 1)) == SSA_NAME))
	    is_copy = 1;

	  ops = use_ops (stmt);
	  num_ops = ((ops) ? VARRAY_ACTIVE_SIZE (ops) : 0);

	  for (i = 0; i < num_ops; i++)
	    {
	      use_p = VARRAY_GENERIC_PTR (ops, i);
	      *use_p = var_to_partition_to_var (map, *use_p);
	    }

	  if (def_op (stmt))
	    {
	      tree *def_p = def_op (stmt);
	      *def_p = var_to_partition_to_var (map, *def_p);

	      if (is_copy && num_ops == 1 && use_p && (*def_p == *use_p))
		remove = 1;
	    }

	  /* Remove copies of the form 'var = var'.  */
	  if (remove)
	    bsi_remove (&si);
	  else
	    bsi_next (&si);
	}

      phi = phi_nodes (bb);
      if (phi)
        {
	  for (e = bb->pred; e; e = e->pred_next)
	    eliminate_phi (e, phi_arg_from_edge (phi, e), g);
	  for ( ; phi; phi = next)
	    {
	      next = TREE_CHAIN (phi);
	      remove_phi_node (phi, NULL_TREE, bb);
	    }
	}

    }

  delete_elim_graph (g);

  /* If any copies were inserted on edges, actually insert them now.  */
  bsi_commit_edge_inserts (0, NULL);

  /* Do some cleanups which reduce the amount of data the
     tree->rtl expanders deal with.  */
  do
    {
      repeat = remove_useless_stmts_and_vars (&DECL_SAVED_TREE (fndecl));
    }
  while (repeat);
  
  /* Flush out flow graph and SSA data.  */
  delete_tree_ssa (fndecl);
  delete_tree_cfg ();
  delete_var_map (map);
  timevar_pop (TV_TREE_SSA_TO_NORMAL);
}


/* Remove edge E and remove the corresponding arguments from the PHI nodes
   in E's destination block.  Return a TREE_LIST node with all the removed
   PHI arguments.  */

void
ssa_remove_edge (edge e)
{
  tree phi;

  /* Remove the appropriate PHI arguments in E's destination block.  */
  for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
    remove_phi_arg (phi, e->src);

  remove_edge (e);
}


/* Make a new edge between BB1 and BB2.  All the PHI nodes at BB2 will
   receive a new argument that should be provided in PHI_ARG_LIST.  */

edge
ssa_make_edge (basic_block bb1, basic_block bb2, int flags, tree phi_arg_list)
{
  tree phi;
  edge e;
  
  e = make_edge (bb1, bb2, flags);

  /* Add a new argument to every PHI node in BB2.  FIXME: Hmm, double
     linear scan.  This may slow things down.  */
  if (phi_arg_list)
    for (phi = phi_nodes (bb2); phi; phi = TREE_CHAIN (phi))
      {
	/* Look for the new argument to add in PHI_ARG_LIST.  */
	tree node;

	for (node = phi_arg_list; node; node = TREE_CHAIN (node))
	  {
	    tree arg = TREE_VALUE (node);
	    if (SSA_NAME_VAR (arg) == SSA_NAME_VAR (PHI_RESULT (phi)))
	      {
		add_phi_arg (phi, arg, e);
		break;
	      }
	  }

	/* If we didn't find an argument for the PHI node, then PHI_ARG_LIST
	  is wrong.  */
	if (node == NULL_TREE)
	  abort ();
      }

  return e;
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
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);

  fprintf (file, "SSA information for %s\n\n", funcname);

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

  fprintf (file, "\nHash table statistics:\n");

  fprintf (file, "    def_blocks: ");
  htab_statistics (file, def_blocks);

  fprintf (file, "    currdefs: ");
  htab_statistics (file, currdefs);

  fprintf (file, "    avail_exprs: ");
  htab_statistics (file, avail_exprs);

  fprintf (file, "    const_and_copies: ");
  htab_statistics (file, const_and_copies);

  fprintf (file, "\n");
}


/* Dump SSA statistics on stderr.  */

void
debug_tree_ssa_stats ()
{
  dump_tree_ssa_stats (stderr);
}


/* Dump statistics for the hash table HTAB.  */

static void
htab_statistics (file, htab)
     FILE *file;
     htab_t htab;
{
  fprintf (file, "size %ld, %ld elements, %f collision/search ratio\n",
	   (long) htab_size (htab),
	   (long) htab_elements (htab),
	   htab_collisions (htab));
}


/*---------------------------------------------------------------------------
		  Helpers for the main SSA building functions
---------------------------------------------------------------------------*/

/* When using fully pruned SSA form we need to compute global life
   information for each object using fully pruned SSA form.

   For large CFGs with many variable performing a per-object global
   life analysis can be extremely expensive.  So instead we queue
   objects so that we can compute life information for several in
   parallel.

   This routine drains the queue of deferred objects and inserts
   PHI nodes for those objects.   The queue of objects is DEF_MAPS,
   a virtual array of struct def_blocks_d pointers, one for each
   object.  */

static void
insert_phis_for_deferred_variables (def_maps)
     varray_type def_maps;
{
  unsigned int i;
  unsigned int num_elements;

  /* Compute global life information for the variables we have deferred.  */
  compute_global_livein (def_maps);

  /* Now insert PHIs for each of the deferred variables.  */
  num_elements = VARRAY_ACTIVE_SIZE (def_maps);
  for (i = 0; i < num_elements; i++)
    {
      /* Pop an element off the list and restore enough state
	 so that we can insert its PHI nodes.  */
      struct def_blocks_d *def_map = VARRAY_GENERIC_PTR (def_maps, i);
      tree var = def_map->var;
      bitmap phi_insertion_points = def_map->phi_insertion_points;
      unsigned bb_index;

      VARRAY_POP (def_maps);
      EXECUTE_IF_SET_IN_BITMAP (phi_insertion_points, 0, bb_index,
	{
	  if (bitmap_bit_p (def_map->livein_blocks, bb_index))
	    create_phi_node (var, BASIC_BLOCK (bb_index));
	});

      BITMAP_XFREE (phi_insertion_points);
      def_map->phi_insertion_points = NULL;
    }
}

/* Insert PHI nodes for variable VAR.  */

static void
insert_phi_nodes_for (var, dfs, def_maps)
     tree var;
     bitmap *dfs;
     varray_type def_maps;
{
  struct def_blocks_d *def_map;
  bitmap phi_insertion_points;
  unsigned phi_vector_lengths = 0;
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
  if (phi_vector_lengths > 64)
    {
      /* Save away enough information so that we can defer PHI
	 insertion for this variable.  */
      def_map->phi_insertion_points = phi_insertion_points;
      VARRAY_PUSH_GENERIC_PTR (def_maps, def_map);
      return;
    }

  EXECUTE_IF_SET_IN_BITMAP (phi_insertion_points, 0, bb_index,
    {
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
  if (IS_EMPTY_STMT (stmt))
    return;

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
  if (ann->modified)
    fold_stmt (stmt);

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
	  ssa_stats.num_re++;
	  TREE_OPERAND (stmt, 1) = cached_lhs;
	  ann->modified = 1;
#else
	  if (cached_lhs && get_value_for (*def_p, currdefs) == cached_lhs)
	    {
	      /* A redundant assignment to the same lhs, perhaps a new
                 evaluation of an expression temporary that is still live.
                 Just discard it.  */
	      ssa_stats.num_re++;
	      bsi_remove (&si);
	      return;
	    }
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
	  if (MAY_COPYPROP_P (*def_p, rhs)
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
  tree op;

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
      tree base = get_value_for (TREE_OPERAND (op, 0), currdefs);
      if (base && SSA_NAME_VAR (base) != TREE_OPERAND (op, 0))
	op = indirect_ref (base);
    }

  *op_p = get_reaching_def (op);
}


/* Register DEF to be a new definition for variable VAR and push VAR's
   current reaching definition into the stack pointed by BLOCK_DEFS_P.  */

static void
register_new_def (var, def, block_defs_p)
     tree var;
     tree def;
     varray_type *block_defs_p;
{
  tree currdef = get_value_for (var, currdefs);

  /* If the current reaching definition is NULL or a constant, push the
     variable itself so that rewrite_block knows what variable is
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
  num_call_clobbered_vars = 0;
  VARRAY_TREE_INIT (call_clobbered_vars, 20, "call_clobbered_vars");
  memset ((void *) &ssa_stats, 0, sizeof (ssa_stats));
  global_var = NULL_TREE;

  /* Allocate memory for the DEF_BLOCKS hash table.  */
  def_blocks = htab_create (num_referenced_vars, def_blocks_hash,
			    def_blocks_eq, def_blocks_free);

  /* Allocate memory for the CURRDEFS hash table.  */
  currdefs = htab_create (num_referenced_vars, var_value_hash, var_value_eq,
			  free);

  /* Allocate memory for the AVAIL_EXPRS hash table.  */
  avail_exprs = htab_create (1024, avail_expr_hash, avail_expr_eq, NULL);

  /* Allocate memory for the CONST_AND_COPIES hash table.  */
  const_and_copies = htab_create (1024, var_value_hash, var_value_eq, free);
}


/* Deallocate memory associated with SSA data structures for FNDECL.  */

static void
delete_tree_ssa (fndecl)
     tree fndecl;
{
  size_t i;

  /* Remove annotations from every tree in the function.  */
  walk_tree (&DECL_SAVED_TREE (fndecl), remove_annotations_r, NULL, NULL);

  /* Remove annotations from every referenced variable.  */
  for (i = 0; i < num_referenced_vars; i++)
    referenced_var (i)->common.ann = NULL;

  num_referenced_vars = 0;
  referenced_vars = NULL;
  global_var = NULL_TREE;
  num_call_clobbered_vars = 0;
  call_clobbered_vars = NULL;
}


/* Callback function for walk_tree to clear DFA/SSA annotations from node
   *TP.  */

static tree
remove_annotations_r (tp, walk_subtrees, data)
    tree *tp;
    int *walk_subtrees ATTRIBUTE_UNUSED;
    void *data ATTRIBUTE_UNUSED;
{
  tree t = *tp;
  enum tree_code code = TREE_CODE (t);

  t->common.ann = NULL;
  /* If the node is not a container, then it has nothing interesting
     underneath it.  */
  if (code != LOOP_EXPR
      && code != COND_EXPR
      && code != CATCH_EXPR
      && code != TRY_CATCH_EXPR
      && code != TRY_FINALLY_EXPR
      && code != SWITCH_EXPR
      && code != BIND_EXPR
      && code != COMPOUND_EXPR)
    {
      /* Ugh.  A MODIFY_EXPR has an annotation on its RHS for the
         benefit of SSA-PRE.  See tree-dfa.c::create_stmt_ann.  */
      if (code == MODIFY_EXPR)
	TREE_OPERAND (t, 1)->common.ann = NULL;

      *walk_subtrees = 0;
      return NULL_TREE;
    }

  return NULL_TREE;
}

/* Return the current definition for variable VAR.  If none is found,
   create a new SSA name to act as the zeroth definition for VAR.  If VAR
   is call clobbered and there exists a more recent definition of
   GLOBAL_VAR, return the definition for GLOBAL_VAR.  This means that VAR
   has been clobbered by a function call since its last assignment.  */

static tree
get_reaching_def (var)
     tree var;
{
  tree default_def, currdef_var;
  
  /* Lookup the current reaching definition for VAR.  */
  default_def = NULL_TREE;
  currdef_var = get_value_for (var, currdefs);

  /* If there is no reaching definition for VAR, create and register a
     default definition for it.  */
  if (currdef_var == NULL_TREE)
    {
      default_def = make_ssa_name (var, build_empty_stmt ());
      set_value_for (var, default_def, currdefs);
    }

  /* Return the current reaching definition for VAR, or the default
     definition, if we had to create one.  */
  return (currdef_var) ? currdef_var : default_def;
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


/* Given a conditional statement IF_STMT, return the assignment 'X = Y', if
   the conditional is of the form 'X == Y'.  If the conditional is of the
   form 'X'.  The assignment 'X = 1' is returned.  */

static tree
get_eq_expr_value (if_stmt)
     tree if_stmt;
{
  tree cond, value;

  cond = COND_EXPR_COND (if_stmt);

  /* If the conditional is a single variable 'X', return 'X = 1'.  */
  if (SSA_VAR_P (cond))
    return build (MODIFY_EXPR, TREE_TYPE (cond), cond, integer_one_node);

  /* If the conditional is of the form 'X == Y', return 'X = Y'.  */
  else if (TREE_CODE (cond) == EQ_EXPR
	   && TREE_CODE (TREE_OPERAND (cond, 0)) == SSA_NAME
	   && (TREE_CONSTANT (TREE_OPERAND (cond, 1))
	       || (TREE_CODE (TREE_OPERAND (cond, 1)) == SSA_NAME
		   && MAY_COPYPROP_P (TREE_OPERAND (cond, 0),
				      TREE_OPERAND (cond, 1)))))
    value = build (MODIFY_EXPR, TREE_TYPE (cond),
		   TREE_OPERAND (cond, 0),
		   TREE_OPERAND (cond, 1));

  /* Return nothing for any other conditional.  */
  else
    value = NULL_TREE;

  return value;
}


/* Hashing and equality functions for AVAIL_EXPRS.  The table stores
   MODIFY_EXPR statements.  We compute a value number for expressions using
   the code of the expression and the SSA numbers of its operands.  */

static hashval_t
avail_expr_hash (p)
     const void *p;
{
  hashval_t val = 0;
  tree rhs;
  size_t i;
  varray_type ops;
  tree stmt = (tree) p;
  enum tree_code code;

  rhs = TREE_OPERAND (stmt, 1);
  code = TREE_CODE (rhs);
  val = iterative_hash_object (code, val);

  /* Add the SSA version numbers of every use operand.  */
  ops = use_ops (stmt);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    {
      tree op = *((tree *) VARRAY_GENERIC_PTR (ops, i));
      if (TREE_CODE (op) == SSA_NAME)
	val = iterative_hash_object (SSA_NAME_VERSION (op), val);
      else
	val = iterative_hash_object (op, val);
    }

  /* Add the SSA version numbers of every vuse operand.  This is important
     because compound variables like arrays are not renamed in the
     operands.  Rather, the rename is done on the virtual variable
     representing all the elements of the array.  */
  ops = vuse_ops (stmt);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    val = iterative_hash_object (SSA_NAME_VERSION (VARRAY_TREE (ops, i)), val);

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

  /* In case of a collision, both RHS have to be identical and have the
     same VUSE operands.  */
  if (TREE_CODE (rhs1) == TREE_CODE (rhs2)
      && simple_cst_equal (rhs1, rhs2) == 1)
    {
      varray_type ops1 = vuse_ops (s1);
      varray_type ops2 = vuse_ops (s2);

      if (ops1 == NULL && ops2 == NULL)
	return true;

      if (VARRAY_ACTIVE_SIZE (ops1) == VARRAY_ACTIVE_SIZE (ops2))
	{
	  size_t i;
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (ops1); i++)
	    if (VARRAY_GENERIC_PTR (ops1, i) != VARRAY_GENERIC_PTR (ops2, i))
	      return false;

	  return true;
	}
    }

  return false;
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

  if (get_value_for (real_var, currdefs) != var)
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
