/* SSA for trees.
   Copyright (C) 2001, 2002, 2003 Free Software Foundation, Inc.
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
#include "cfgloop.h"

/* This file builds the SSA form for a function as described in:

   R. Cytron, J. Ferrante, B. Rosen, M. Wegman, and K. Zadeck. Efficiently
   Computing Static Single Assignment Form and the Control Dependence
   Graph. ACM Transactions on Programming Languages and Systems,
   13(4):451-490, October 1991.  */


/* Next SSA version number to be assigned by make_ssa_name.  Initialized by
   init_tree_ssa.  */
unsigned int next_ssa_version;

/* Dump file and flags.  */
static FILE *dump_file;
static int dump_flags;

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
};

static struct ssa_stats_d ssa_stats;

/* Bitmap representing variables that need to be renamed into SSA form.  */
static sbitmap vars_to_rename;


/* Local functions.  */
static void delete_tree_ssa (tree);
static void mark_def_sites (dominance_info, sbitmap);
static void compute_global_livein (varray_type);
static void set_def_block (tree, basic_block);
static void set_livein_block (tree, basic_block);
static void insert_phi_nodes (bitmap *, sbitmap);
static void insert_phis_for_deferred_variables (varray_type);
static void rewrite_block (basic_block);
static void check_for_new_variables (void);
static void rewrite_stmt (block_stmt_iterator, varray_type *);
static inline void rewrite_operand (tree *, bool);
static void register_new_def (tree, tree, varray_type *, bool);
static void insert_phi_nodes_for (tree, bitmap *, varray_type);
static tree remove_annotations_r (tree *, int *, void *);
static tree get_reaching_def (tree);
static tree get_value_for (tree, htab_t);
static void set_value_for (tree, tree, htab_t);
static hashval_t def_blocks_hash (const void *);
static int def_blocks_eq (const void *, const void *);
static hashval_t var_value_hash (const void *);
static int var_value_eq (const void *, const void *);
static void def_blocks_free (void *);
static int debug_def_blocks_r (void **, void *);
static struct def_blocks_d *get_def_blocks_for (tree);
static void htab_statistics (FILE *, htab_t);

static tree create_temp (tree);
static void insert_copy_on_edge (edge, tree, tree);
static elim_graph new_elim_graph (int);
static void delete_elim_graph (elim_graph);
static void clear_elim_graph (elim_graph);
static void eliminate_name (elim_graph, tree);
static int eliminate_build (elim_graph, basic_block, int);
static void elim_forward (elim_graph, int);
static int elim_unvisited_predecessor (elim_graph, int);
static void elim_backward (elim_graph, int);
static void elim_create (elim_graph, int);
static void eliminate_phi (edge, int, elim_graph);
static void coalesce_ssa_name (var_map);
static void assign_vars (var_map);
static inline void set_if_valid (var_map, sbitmap, tree);
static inline void add_conflicts_if_valid (root_var_p, conflict_graph,
					   var_map, sbitmap, tree);
static void replace_variable (var_map, tree *);

/* Main entry point to the SSA builder.  FNDECL is the gimplified function
   to convert.  VARS is an sbitmap representing variables that need to be
   renamed into SSA form.  A variable in REFERENCED_VARS will be renamed
   into SSA if the element corresponding to the variable's UID is set in
   VARS.  If VARS is NULL, all variables in REFERENCED_VARS will be
   processed.

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
	*p = a;			*p_3 = a_1;


   Dealing with aliases
   --------------------

   Assume that in the following code fragment, pointer p may point to b:

	    1	b = 4;
	    2	*p = 5;
	    3	... = b;

   When converting this code to SSA form, we need to model the fact that
   the use of 'b' at line 3 may be reached by the definition of *p at line
   2.  We cannot just rewrite the use of 'b' to be a use of '*p':

	    1	b_1 = 4;
	    2	*p_2 = 5;
	    3	... = *p_2;

   The above transformation is wrong, because p and b MAY alias, the
   compiler doesn't know whether they will always alias at runtime.  On the
   other hand, ignoring the may alias relation also leads to an incorrect
   transformation:
   
	    1	b_1 = 4;
	    2	*p_2 = 5;
	    3	... = b_1;

   With this version, the optimizers will not notice that the assignment to
   *p at line 2 may in fact reach the use of b at line 3.  To deal with
   this problem, the rewriting pass will move aliased operands into the
   virtual operands and then re-write the virtual operand, while leaving
   the original operand untouched.  So, the above code fragment is converted
   into:

	     	# AT.1_2 = VDEF <AT.1_1>;
	    1	b = 4;
	        # AT.1_3 = VDEF <AT.1_2>;
	    2	*p_3 = 5;
	     	# VUSE <AT.1_3>
	    3	... = b;

   Notice how variable 'b' is not rewritten anymore.  Instead, references
   to it are moved to the virtual operands which are all rewritten using
   AT.1, which is an artificial variable representing all the variables
   aliased by 'p'.

   Virtual operands provide safe information about potential references to
   the operands in a statement.  But they are imprecise by nature.
   Optimizations may want to take them into account, at the expense of
   increased compilation time.  */

void
rewrite_into_ssa (tree fndecl, sbitmap vars)
{
  bitmap *dfs;
  sbitmap globals;
  dominance_info idom;
  int i, rename_count;
  bool compute_df;
  bool addr_expr_propagated_p;
  
  timevar_push (TV_TREE_SSA_OTHER);

  /* Debugging dumps.  */
  dump_file = dump_begin (TDI_ssa, &dump_flags);

  /* Initialize the array of variables to rename.  */
  if (vars == NULL)
    {
      vars_to_rename = sbitmap_alloc (num_referenced_vars);
      sbitmap_ones (vars_to_rename);
    }
  else
    vars_to_rename = vars;

  /* Allocate memory for the DEF_BLOCKS hash table.  */
  def_blocks = htab_create (VARRAY_ACTIVE_SIZE (referenced_vars),
			    def_blocks_hash, def_blocks_eq, def_blocks_free);

  /* Allocate memory for the CURRDEFS hash table.  */
  currdefs = htab_create (VARRAY_ACTIVE_SIZE (referenced_vars),
			  var_value_hash, var_value_eq, free);

  /* Allocate memory for the GLOBALS bitmap which will indicate which
     variables are live across basic block boundaries.  Note that this
     bitmap is indexed by variable UID, so it must always be large enough
     to accomodate all the variables referenced in the program, not just
     the ones we are renaming.  */
  globals = sbitmap_alloc (num_referenced_vars);
  sbitmap_zero (globals);

  /* Initialize dominance frontier and immediate dominator bitmaps.  */
  dfs = (bitmap *) xmalloc (n_basic_blocks * sizeof (bitmap *));
  for (i = 0; i < n_basic_blocks; i++)
    {
      dfs[i] = BITMAP_XMALLOC ();
      bb_ann (BASIC_BLOCK (i))->dom_children = NULL;
    }

  /* Compute immediate dominators.  */
  idom = calculate_dominance_info (CDI_DOMINATORS);

  /* Start the SSA rename process.  This may need to be repeated if the
     dominator optimizations exposed more symbols to rename by propagating
     ADDR_EXPR values into INDIRECT_REF expressions.  */
  rename_count = 0;
  compute_df = true;
  addr_expr_propagated_p = false;
  do
    {
      /* Find variable references and mark definition sites.  */
      mark_def_sites (idom, globals);

      /* Compute dominance frontiers (only once) and insert PHI nodes at
	 dominance frontiers of definition blocks.  */
      if (compute_df)
	{
	  compute_dominance_frontiers (dfs, idom);
	  compute_df = false;
	}

      insert_phi_nodes (dfs, globals);

      /* Rewrite all the basic blocks in the program.  */
      timevar_push (TV_TREE_SSA_REWRITE_BLOCKS);
      sbitmap_zero (vars_to_rename);
      rewrite_block (ENTRY_BLOCK_PTR);
      timevar_pop (TV_TREE_SSA_REWRITE_BLOCKS);

      if (flag_tree_dom)
	{
	  /* Now optimize all the basic blocks in the program.  */
	  timevar_push (TV_TREE_SSA_DOMINATOR_OPTS);
	  addr_expr_propagated_p = tree_ssa_dominator_optimize (dump_file,
								dump_flags);
	  timevar_pop (TV_TREE_SSA_DOMINATOR_OPTS);
	}

      /* If the dominator optimizations propagated ADDR_EXPRs, we may need
	 to repeat the SSA renaming process for the new symbols that may
	 have been exposed.  Re-scan the program for operands not in SSA
	 form and if new symbols are added to VARS_TO_RENAME, repeat the
	 process.  */
      if (addr_expr_propagated_p)
	{
	  check_for_new_variables ();
	  if (sbitmap_first_set_bit (vars_to_rename) >= 0)
	    {
	      /* Remove PHI nodes for the new symbols, clear the hash
		 tables and bitmaps and run SSA again on the new exposed
		 variables.  */
	      remove_all_phi_nodes_for (vars_to_rename);
	      htab_empty (def_blocks);
	      htab_empty (currdefs);
	      sbitmap_zero (globals);
	    }
	}
    }
  while (sbitmap_first_set_bit (vars_to_rename) >= 0);

  /* Free allocated memory.  */
  for (i = 0; i < n_basic_blocks; i++)
    BITMAP_XFREE (dfs[i]);
  free (dfs);
  free (globals);
  free_dominance_info (idom);
  htab_delete (def_blocks);
  htab_delete (currdefs);
  if (vars == NULL)
    sbitmap_free (vars_to_rename);

  /* Debugging dumps.  */
  if (dump_file)
    {
      if (dump_flags & TDF_STATS)
	{
	  dump_dfa_stats (dump_file);
	  dump_tree_ssa_stats (dump_file);
	}

      dump_end (TDI_ssa, dump_file);
      dump_file = NULL;
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
compute_global_livein (varray_type def_maps)
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
mark_def_sites (dominance_info idom, sbitmap globals)
{
  basic_block bb;
  block_stmt_iterator si;
  sbitmap kills;

  /* Notice that this bitmap is indexed using variable UIDs, so it must be
     large enough to accomodate all the variables referenced in the
     function, not just the ones we are renaming.  */
  kills = sbitmap_alloc (num_referenced_vars);

  /* Mark all the blocks that have definitions for each variable in the
     VARS_TO_RENAME bitmap.  */
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
	      size_t uid;

	      /* Ignore variables that have been renamed already.  */
	      if (TREE_CODE (*use) == SSA_NAME)
		continue;
	      
	      uid = var_ann (*use)->uid;

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
	      size_t uid;

	      /* Ignore variables that have been renamed already.  */
	      if (TREE_CODE (use) == SSA_NAME)
		continue;
	      
	      uid = var_ann (use)->uid;

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
	     GLOBALS.  */
	  ops = vdef_ops (stmt);
	  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
	    {
	      tree vdef = VARRAY_TREE (ops, i);
	      tree vdef_op = VDEF_OP (vdef);
	      size_t uid;

	      /* Ignore variables that have been renamed already.  */
	      if (TREE_CODE (vdef_op) == SSA_NAME)
		continue;

	      uid = var_ann (vdef_op)->uid;

	      set_def_block (VDEF_RESULT (vdef), bb);
	      if (!TEST_BIT (kills, uid))
		{
		  SET_BIT (globals, uid);
	          set_livein_block (vdef_op, bb);
		}
	    }

	  /* Now process the definition made by this statement.  If the
	     definition has been renamed already, do nothing.  */
	  dest = def_op (stmt);
	  if (dest && TREE_CODE (*dest) != SSA_NAME)
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
set_def_block (tree var, basic_block bb)
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
set_livein_block (tree var, basic_block bb)
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
   
   GLOBALS is a bitmap representing the set of variables which are live
   across basic block boundaries.  Only variables in GLOBALS need PHI
   nodes.  */

static void
insert_phi_nodes (bitmap *dfs, sbitmap globals)
{
  size_t i;
  varray_type def_maps;

  timevar_push (TV_TREE_INSERT_PHI_NODES);

  VARRAY_GENERIC_PTR_INIT (def_maps, HOST_BITS_PER_WIDE_INT, "deferred_vars");

  /* Array WORK_STACK is a stack of CFG blocks.  Each block that contains
     an assignment or PHI node will be pushed to this stack.  */
  VARRAY_BB_INIT (work_stack, last_basic_block, "work_stack");

  /* Iterate over all variables in VARS_TO_RENAME.  For each variable, add
     to the work list all the blocks that have a definition for the
     variable.  PHI nodes will be added to the dominance frontier blocks of
     each definition block.  */
  EXECUTE_IF_SET_IN_SBITMAP (vars_to_rename, 0, i,
    {
      tree var = referenced_var (i);
      var_ann_t ann = var_ann (var);

      /* If we have queued enough variables, then drain the queue.  */
      if (VARRAY_ACTIVE_SIZE (def_maps) == HOST_BITS_PER_WIDE_INT)
	insert_phis_for_deferred_variables (def_maps);

      if (TEST_BIT (globals, ann->uid))
	insert_phi_nodes_for (var, dfs, def_maps);
    });

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
      
   3- All the PHI nodes in successor blocks of BB are visited.  The
      argument corresponding to BB is replaced with its current reaching
      definition.

   4- Recursively rewrite every dominator child block of BB.

   5- Restore (in reverse order) the current reaching definition for every
      new definition introduced in this block.  This is done so that when
      we return from the recursive call, all the current reaching
      definitions are restored to the names that were valid in the
      dominator parent of BB.  */

static void
rewrite_block (basic_block bb)
{
  edge e;
  varray_type block_defs;
  bitmap children;
  unsigned long i;
  block_stmt_iterator si;
  tree phi;

  /* Initialize the local stacks.
     
     BLOCK_DEFS is used to save all the existing reaching definitions for
	the new SSA names introduced in this block.  Before registering a
	new definition for a variable, the existing reaching definition is
	pushed into this stack so that we can restore it in Step 5.  */

  VARRAY_TREE_INIT (block_defs, 20, "block_defs");

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\n\nRenaming block #%d\n\n", bb->index);

  /* Step 1.  Register new definitions for every PHI node in the block.
     Conceptually, all the PHI nodes are executed in parallel and each PHI
     node introduces a new version for the associated variable.  */
  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    register_new_def (SSA_NAME_VAR (PHI_RESULT (phi)), PHI_RESULT (phi),
		      &block_defs, false);

  /* Step 2.  Rewrite every variable used in each statement the block with
     its immediate reaching definitions.  Update the current definition of
     a variable when a new real or virtual definition is found.  */
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    rewrite_stmt (si, &block_defs);


  /* Step 3.  Visit all the successor blocks of BB looking for PHI nodes.
     For every PHI node found, add a new argument containing the current
     reaching definition for the variable and the edge through which that
     definition is reaching the PHI node.  */
  for (e = bb->succ; e; e = e->succ_next)
    {
      tree phi;

      for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
	{
	  tree currdef;

	  /* Ignore PHI nodes that have already been renamed.  */
	  if (PHI_NUM_ARGS (phi) == PHI_ARG_CAPACITY (phi))
	    continue;

	  currdef = get_reaching_def (SSA_NAME_VAR (PHI_RESULT (phi)));
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
}

/* This function will create a temporary for a partition based on the
   type of the variable which already represents a partition.  */

static tree
create_temp (tree t)
{
  tree tmp;
  const char *name = NULL;
  tree type;

  if (TREE_CODE (t) == SSA_NAME)
    t = SSA_NAME_VAR (t);
 
  if (TREE_CODE (t) != VAR_DECL 
      && TREE_CODE (t) != PARM_DECL)
    abort ();

  type = TREE_TYPE (t);
  tmp = DECL_NAME (t);
  if (tmp)
    name = IDENTIFIER_POINTER (tmp);

  if (name == NULL)
    name = "temp";
  tmp = create_tmp_var (type, name);
  create_var_ann (tmp);
  set_is_used (tmp);
  return tmp;
}


/* This helper function fill insert a copy from one variable to another
   on the specified edge.  */

static void
insert_copy_on_edge (edge e, tree dest, tree src)
{
  tree copy;

  copy = build (MODIFY_EXPR, TREE_TYPE (dest), dest, src);
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file,
	       "Inserting a copy on edge BB%d->BB%d :",
	       e->src->index,
	       e->dest->index);
      print_generic_expr (dump_file, copy, dump_flags);
      fprintf (dump_file, "\n");
    }
  bsi_insert_on_edge (e, copy);
}

/* --------------------------------------------------------------------- */
/* Create an elimination graph and associated data structures.  */

static elim_graph
new_elim_graph (int size)
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
clear_elim_graph (elim_graph g)
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
delete_elim_graph (elim_graph g)
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
eliminate_name (elim_graph g, tree T)
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
eliminate_build (elim_graph g, basic_block B, int i)
{
  tree phi;
  tree T0, Ti;
  int p0, pi;
  int edges = 0;

  g->num_nodes = 0;
  
  for (phi = phi_nodes (B); phi; phi = TREE_CHAIN (phi))
    {
      if (!SSA_NAME_HAS_REAL_REFS (PHI_RESULT (phi))
	  || !SSA_NAME_HAS_REAL_REFS (PHI_ARG_DEF (phi, i)))
	continue;

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
elim_forward (elim_graph g, int T)
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
elim_unvisited_predecessor (elim_graph g, int T)
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
elim_backward (elim_graph g, int T)
{
  int P;
  SET_BIT (g->visited, T);
  EXECUTE_IF_SET_IN_BITMAP (g->pred[T], 0, P,
    {
      if (!TEST_BIT (g->visited, P))
        {
	  elim_backward (g, P);
	  insert_copy_on_edge (g->e, 
			       partition_to_var (g->map, P), 
			       partition_to_var (g->map, T));
	}
    });
}

/* Check for a SCR, and create a temporary if there is one, and break
   the cycle. Then issue the copies. Otherwise, simply insert the
   required copies.  */

static void 
elim_create (elim_graph g, int T)
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
      S = bitmap_first_set_bit (g->succ[T]);
      if (S != -1)
	{
	  SET_BIT (g->visited, T);
	  bitmap_clear_bit (g->succ[T], S);
	  insert_copy_on_edge (g->e, 
			       partition_to_var (g->map, T), 
			       partition_to_var (g->map, S));
	}
    }
  
}

/* Eliminate all the phi nodes on this edge.  */

static void
eliminate_phi (edge e, int i, elim_graph g)
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
set_if_valid (var_map map, sbitmap vec, tree var)
{ 
  int p = var_to_partition (map, var);
  if (p != NO_PARTITION)
    SET_BIT (vec, p);
}

/* If a variable is in a partition, and it's not already live, add a 
   conflict between it and any other live partition.  Reset the live bit.  */

static inline void 
add_conflicts_if_valid (root_var_p rv, conflict_graph graph,
			var_map map, sbitmap vec, tree var)
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
coalesce_ssa_name (var_map map)
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

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Must coalesce ");
	      print_generic_expr (dump_file, 
				  partition_to_var (map, x), 
				  TDF_SLIM);
	      fprintf (dump_file, " with the root variable ");
	      print_generic_expr (dump_file, var, TDF_SLIM);
	      fprintf (dump_file, ".\n");
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

	    /* Ignore SSA names that have no real references, as those
	       don't generate code.  */
	    if (!SSA_NAME_HAS_REAL_REFS (var))
	      continue;

	    x = var_to_partition (map, var);
	    y = phi_arg_from_edge (phi, e);
	    if (y == -1)
	      abort ();

	    tmp = PHI_ARG_DEF (phi, y);
	    if (!SSA_NAME_HAS_REAL_REFS (tmp))
	      continue;

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
		fprintf (stderr, "\nDifferent root vars '\n");
		print_generic_expr (stderr, 
				    root_var (rv, find_root_var (rv, x)),
				    TDF_SLIM);
		fprintf (stderr, "' and '");
		print_generic_expr (stderr, 
				    root_var (rv, find_root_var (rv, y)),
				    TDF_SLIM);
		fprintf (stderr, "' across an abnormal edge\n");
		abort ();
	      }

	    if (x != y)
	      {
		if (!conflict_graph_conflict_p (graph, x, y))
		  {
		    /* Now map the partitions back to their real variables.  */
		    var = partition_to_var (map, x);
		    tmp = partition_to_var (map, y);
		    if (dump_file 
			&& (dump_flags & TDF_DETAILS))
		      {
			fprintf (dump_file , "ABNORMAL: Coalescing ");
			print_generic_expr (dump_file, 
					    var, 
					    TDF_SLIM);
			fprintf (dump_file , " and ");
			print_generic_expr (dump_file, 
					    tmp, 
					    TDF_SLIM);
			fprintf (dump_file , " over abnormal edge.\n");
		      }
		    var_union (map, var, tmp);
		    conflict_graph_merge_regs (graph, x, y);
		  }
		else
		  {
		    fprintf (stderr, "\n");
		    print_generic_expr (stderr, 
					partition_to_var (map, x), 
					TDF_SLIM);
		    fprintf (stderr, " and ");
		    print_generic_expr (stderr, 
					partition_to_var (map, y), 
					TDF_SLIM);
		    fprintf (stderr, " conflict across an abnormal edge\n");
		    abort ();
		  }
	      }
	  }

    if (dump_file && (dump_flags & TDF_DETAILS))
      {
        dump_var_map (dump_file, map);
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
	      else if (!conflict_graph_conflict_p (graph, y, z))
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
assign_vars (var_map map)
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
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "partition %d has variable ", x);
	      print_generic_expr (dump_file, var, TDF_SLIM);
	      fprintf (dump_file, " assigned to it.\n");
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

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Overlap :  '");
	      print_generic_expr (dump_file, t, TDF_SLIM);
	      fprintf (dump_file, "'  conflicts with  '");
	      print_generic_expr (dump_file, var, TDF_SLIM);
	    }

	  var = create_temp (t);
	  change_partition_var (map, var, i);
	  ann = var_ann (var);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "'     New temp:  '");
	      print_generic_expr (dump_file, var, TDF_SLIM);
	      fprintf (dump_file, "'\n");
	    }
	}
    }

  delete_root_var (rv);
}

/* Replace *p with whatever variable it has been rewritten to.  */

static void
replace_variable (var_map map, tree *p)
{
  tree new_var;
  tree var = *p;
  tree copy;

  new_var = var_to_partition_to_var (map, var);
  if (new_var)
    *p = new_var;
  else
    {
      /* Replace (*var)_version with just (*var).  */
      if (TREE_CODE (SSA_NAME_VAR (var)) == INDIRECT_REF)
	{
	  tree var2 = TREE_OPERAND (SSA_NAME_VAR (var), 0);
	  new_var = var_to_partition_to_var (map, var2);
	  copy = copy_node (SSA_NAME_VAR (var));
	  if (new_var)
	    TREE_OPERAND (copy, 0) = new_var;
	  else
	    TREE_OPERAND (copy, 0) = var2;
	  *p = copy;
	}
    }
}

/* Take function FNDECL out of SSA form.  */

void
rewrite_out_of_ssa (tree fndecl)
{
  basic_block bb;
  block_stmt_iterator si;
  edge e;
  var_map map;
  tree phi, next;
  elim_graph g;
  int repeat, first_iteration;

  timevar_push (TV_TREE_SSA_TO_NORMAL);

  dump_file = dump_begin (TDI_optimized, &dump_flags);

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_tree_cfg (dump_file, dump_flags & ~TDF_DETAILS);

  map = create_ssa_var_map ();

  /* Shrink the map to include only referenced variables.  Exclude variables
     which have only one SSA version since there is nothing to coalesce.  */
  compact_var_map (map, VARMAP_NO_SINGLE_DEFS);

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_var_map (dump_file, map);

  coalesce_ssa_name (map);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "After Coalescing:\n");
      dump_var_map (dump_file, map);
    }

  /* This is the final var list, so assign real variables to the different
     partitions.  Include single reference vars this time. */

  compact_var_map (map, VARMAP_NORMAL);
  assign_vars (map);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "After Root variable replacement:\n");
      dump_var_map (dump_file, map);
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
	      replace_variable (map, use_p);
	    }

	  if (def_op (stmt))
	    {
	      tree *def_p = def_op (stmt);
	      *def_p = var_to_partition_to_var (map, *def_p);
	      replace_variable (map, def_p);

	      if (is_copy
		  && num_ops == 1
		  && use_p
		  && def_p
		  && (*def_p == *use_p))
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

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_tree_cfg (dump_file, dump_flags & ~TDF_DETAILS);

  /* Do some cleanups which reduce the amount of data the
     tree->rtl expanders deal with.  */
  first_iteration = 1;
  do
    {
      repeat = remove_useless_stmts_and_vars (&DECL_SAVED_TREE (fndecl),
					      first_iteration);
      first_iteration = 0;
    }
  while (repeat);
  
  /* Flush out flow graph and SSA data.  */
  delete_tree_ssa (fndecl);
  delete_tree_cfg ();
  delete_var_map (map);
  timevar_pop (TV_TREE_SSA_TO_NORMAL);

  if (dump_file)
    dump_end (TDI_optimized, dump_file);
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

/*---------------------------------------------------------------------------
			       Debugging support
---------------------------------------------------------------------------*/
/* Dump SSA information to FILE.  */

void
dump_tree_ssa (FILE *file)
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
debug_tree_ssa (void)
{
  dump_tree_ssa (stderr);
}


/* Dump SSA statistics on FILE.  */

void
dump_tree_ssa_stats (FILE *file)
{
  fprintf (file, "\nHash table statistics:\n");

  fprintf (file, "    def_blocks: ");
  htab_statistics (file, def_blocks);

  fprintf (file, "    currdefs: ");
  htab_statistics (file, currdefs);

  fprintf (file, "\n");
}


/* Dump SSA statistics on stderr.  */

void
debug_tree_ssa_stats (void)
{
  dump_tree_ssa_stats (stderr);
}


/* Dump statistics for the hash table HTAB.  */

static void
htab_statistics (FILE *file, htab_t htab)
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
insert_phis_for_deferred_variables (varray_type def_maps)
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
insert_phi_nodes_for (tree var, bitmap *dfs, varray_type def_maps)
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

/* Scan all the statements looking for symbols not in SSA form.  If any are
   found, add them to VARS_TO_RENAME to trigger a second SSA pass.  */

static void
check_for_new_variables (void)
{
  basic_block bb;
  
  FOR_EACH_BB (bb)
    {
      block_stmt_iterator si;

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  varray_type ops;
	  size_t i;
	  tree *def_p;

	  tree stmt = bsi_stmt (si);

	  get_stmt_operands (stmt);

	  def_p = def_op (stmt);
	  if (def_p && DECL_P (*def_p))
	    SET_BIT (vars_to_rename, var_ann (*def_p)->uid);

	  ops = use_ops (stmt);
	  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
	    {
	      tree *use_p = VARRAY_GENERIC_PTR (ops, i);
	      if (DECL_P (*use_p))
		SET_BIT (vars_to_rename, var_ann (*use_p)->uid);
	    }

	  ops = vdef_ops (stmt);
	  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
	    {
	      tree var = VDEF_RESULT (VARRAY_TREE (ops, i));
	      if (DECL_P (var))
		SET_BIT (vars_to_rename, var_ann (var)->uid);
	    }

	  ops = vuse_ops (stmt);
	  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
	    {
	      tree var = VARRAY_TREE (ops, i);
	      if (DECL_P (var))
		SET_BIT (vars_to_rename, var_ann (var)->uid);
	    }
	}
    }
}


/* Rewrite statement pointed by iterator SI into SSA form. 

   BLOCK_DEFS_P points to a stack with all the definitions found in the
   block.  This is used by rewrite_block to restore the current reaching
   definition for every variable defined in BB after visiting the
   immediate dominators of BB.  */

static void
rewrite_stmt (block_stmt_iterator si, varray_type *block_defs_p)
{
  size_t i;
  stmt_ann_t ann;
  tree stmt, *def_p;
  varray_type uses, vuses, vdefs;

  stmt = bsi_stmt (si);
  if (IS_EMPTY_STMT (stmt))
    return;

  ann = stmt_ann (stmt);
  ssa_stats.num_stmts++;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Renaming statement ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
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
      tree *op_p = (tree *) VARRAY_GENERIC_PTR (uses, i);
      if (TREE_CODE (*op_p) != SSA_NAME)
	rewrite_operand (op_p, true);
    }

  /* Rewrite virtual uses in the statement.  */
  for (i = 0; vuses && i < VARRAY_ACTIVE_SIZE (vuses); i++)
    if (TREE_CODE (VARRAY_TREE (vuses, i)) != SSA_NAME)
      rewrite_operand (&(VARRAY_TREE (vuses, i)), false);

  /* Step 2.  Register the statement's DEF and VDEF operands.  */
  if (def_p && TREE_CODE (*def_p) != SSA_NAME)
    {
      *def_p = make_ssa_name (*def_p, stmt);
      register_new_def (SSA_NAME_VAR (*def_p), *def_p, block_defs_p, true);
    }

  /* Register new virtual definitions made by the statement.  */
  for (i = 0; vdefs && i < VARRAY_ACTIVE_SIZE (vdefs); i++)
    {
      tree vdef = VARRAY_TREE (vdefs, i);

      if (TREE_CODE (VDEF_RESULT (vdef)) == SSA_NAME)
	continue;

      rewrite_operand (&(VDEF_OP (vdef)), false);

      VDEF_RESULT (vdef) = make_ssa_name (VDEF_RESULT (vdef), stmt);
      register_new_def (SSA_NAME_VAR (VDEF_RESULT (vdef)), 
			VDEF_RESULT (vdef), block_defs_p, false);
    }
}


/* Set the USED bit in the annotation for T.  */

void
set_is_used (tree t)
{
  t = get_base_symbol (t);
  var_ann (t)->used = 1;
}


/* Replace the operand pointed by OP_P with its immediate reaching
   definition.  IS_REAL_OPERAND is true when this is a USE operand.  */

static inline void
rewrite_operand (tree *op_p, bool is_real_operand)
{
#if defined ENABLE_CHECKING
  if (TREE_CODE (*op_p) == SSA_NAME)
    abort ();
#endif

  *op_p = get_reaching_def (*op_p);
  if (is_real_operand)
    SSA_NAME_HAS_REAL_REFS (*op_p) = true;
}


/* Register DEF to be a new definition for variable VAR and push VAR's
   current reaching definition into the stack pointed by BLOCK_DEFS_P.
   IS_REAL_OPERAND is true when DEF is a real definition.  */

static void
register_new_def (tree var, tree def, varray_type *block_defs_p,
		  bool is_real_operand)
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

  if (is_real_operand)
    SSA_NAME_HAS_REAL_REFS (def) = true;
}


/*---------------------------------------------------------------------------
			     Various helpers.
---------------------------------------------------------------------------*/
/* Initialize DFA/SSA structures.  */

void
init_tree_ssa (void)
{
  next_ssa_version = 1;
  VARRAY_TREE_INIT (referenced_vars, 20, "referenced_vars");
  VARRAY_TREE_INIT (call_clobbered_vars, 20, "call_clobbered_vars");
  memset ((void *) &ssa_stats, 0, sizeof (ssa_stats));
  global_var = NULL_TREE;
}


/* Deallocate memory associated with SSA data structures for FNDECL.  */

static void
delete_tree_ssa (tree fndecl)
{
  size_t i;

  /* Remove annotations from every tree in the function.  */
  walk_tree (&DECL_SAVED_TREE (fndecl), remove_annotations_r, NULL, NULL);

  /* Remove annotations from every referenced variable.  */
  for (i = 0; i < num_referenced_vars; i++)
    referenced_var (i)->common.ann = NULL;

  referenced_vars = NULL;
  global_var = NULL_TREE;
  call_clobbered_vars = NULL;
}


/* Callback function for walk_tree to clear DFA/SSA annotations from node
   *TP.  */

static tree
remove_annotations_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		      void *data ATTRIBUTE_UNUSED)
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
get_reaching_def (tree var)
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
def_blocks_free (void *p)
{
  struct def_blocks_d *db_p = (struct def_blocks_d *) p;
  BITMAP_XFREE (db_p->def_blocks);
  BITMAP_XFREE (db_p->livein_blocks);
  free (p);
}


/* Hashing and equality functions for DEF_BLOCKS.  */

static hashval_t
def_blocks_hash (const void *p)
{
  return htab_hash_pointer
	((const void *)((const struct def_blocks_d *)p)->var);
}

static int
def_blocks_eq (const void *p1, const void *p2)
{
  return ((const struct def_blocks_d *)p1)->var
	 == ((const struct def_blocks_d *)p2)->var;
}


/* Hashing and equality functions for VAR_VALUE_D.  */

static hashval_t
var_value_hash (const void *p)
{
  return htab_hash_pointer
	((const void *)((const struct var_value_d *)p)->var);
}

static int
var_value_eq (const void *p1, const void *p2)
{
  return ((const struct var_value_d *)p1)->var
	 == ((const struct var_value_d *)p2)->var;
}


/* Dump the DEF_BLOCKS hash table on stderr.  */

void
debug_def_blocks (void)
{
  htab_traverse (def_blocks, debug_def_blocks_r, NULL);
}

/* Callback for htab_traverse to dump the DEF_BLOCKS hash table.  */

static int
debug_def_blocks_r (void **slot, void *data ATTRIBUTE_UNUSED)
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
get_value_for (tree var, htab_t table)
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
set_value_for (tree var, tree value, htab_t table)
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

/* Return the set of blocks where variable VAR is defined and the blocks
   where VAR is live on entry (livein).  */

static struct def_blocks_d *
get_def_blocks_for (tree var)
{
  struct def_blocks_d dm;

  dm.var = var;
  return (struct def_blocks_d *) htab_find (def_blocks, (void *) &dm);
}
