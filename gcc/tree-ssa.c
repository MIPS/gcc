/* SSA for trees.
   Copyright (C) 2001, 2002, 2003, 2004 Free Software Foundation, Inc.
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
#include "varray.h"
#include "timevar.h"
#include "tree-alias-common.h"
#include "hashtab.h"
#include "tree-dump.h"
#include "tree-ssa-live.h"
#include "tree-pass.h"
#include "cfgloop.h"
#include "domwalk.h"

/* This file builds the SSA form for a function as described in:

   R. Cytron, J. Ferrante, B. Rosen, M. Wegman, and K. Zadeck. Efficiently
   Computing Static Single Assignment Form and the Control Dependence
   Graph. ACM Transactions on Programming Languages and Systems,
   13(4):451-490, October 1991.  */


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
struct GTY(()) def_blocks_d
{
  tree var;
  bitmap def_blocks;
  bitmap livein_blocks;
};

/* Global data to attach to the main dominator walk structure.  */
struct mark_def_sites_global_data
{
  /* This sbitmap contains the variables which are set before they
     are used in a basic block.  We keep it as a global variable
     solely to avoid the overhead of allocating and deallocating
     the bitmap.  */
  sbitmap kills;
};

/* Table to store the current reaching definition for every variable in
   the function.  Given a variable V, its entry will be its immediately
   reaching SSA_NAME node.  */
static varray_type currdefs;

/* Structure to map variables to values.  It's used to keep track of the
   current reaching definition, constant values and variable copies while
   renaming.  */
struct GTY(()) var_value_d
{
  tree var;
  tree value;
};

/* Used to hold all the components requires to do SSA PHI elimination.
   The implementation of the node list and pred/succ list has been changed
   to a simple linear list of nodes and edges represented as pairs of nodes.

   The predecessor and successor list.  Nodes are entered in pairs, where
   [0] ->PRED, [1]->SUCC.  All the even indexes in the array represent 
   predecessors, all the odd elements are successors. 
   
   Rationale:
   When implemented as bitmaps, very large programs SSA->Normal times were 
   being dominated by clearing the interference graph.

   Typically this list of edges is extremely small since it only includes 
   PHI results and uses from a single edge which have not coalesced with 
   each other. This means no virtual PHI nodes are included, and empirical
   evidence suggests that the number of edges rarely exceed 3, and in a 
   bootstrap of GCC, the maximum size encountered was 7. THis also limits 
   the number of possible nodes that are involved to rarely more than 6, 
   and in the bootstrap of gcc, the maximum number of nodes encountered
   was 12.  */
 
typedef struct _elim_graph
{
  /* Size of the elimination vectors.  */
  int size;

  /* List of nodes in the elimination graph.  */
  varray_type nodes;
  /*  The predecessor and successor edge list. */
  varray_type edge_list;

  /* Visited vector.  */
  sbitmap visited;

  /* Stack for visited nodes.  */
  varray_type stack;
  
  /* The variable partition map.  */
  var_map map;
  /* edge being eliminated by this graph.  */
  edge e;
  /* List of constant copies to emit.  These are pushed on in pairs.  */
  varray_type  const_copies;
} *elim_graph;

/* Statistics for dominator optimizations.  */
struct ssa_stats_d
{
  long num_stmts;
};

struct rewrite_block_data
{
  varray_type block_defs;
};

static struct ssa_stats_d ssa_stats;

/* Local functions.  */
static void rewrite_finalize_block (struct dom_walk_data *, basic_block);
static void rewrite_initialize_block_local_data (struct dom_walk_data *,
						 basic_block, bool);
static void rewrite_initialize_block (struct dom_walk_data *, basic_block);
static void rewrite_walk_stmts (struct dom_walk_data *, basic_block);
static void rewrite_add_phi_arguments (struct dom_walk_data *, basic_block);
static void mark_def_sites (struct dom_walk_data *walk_data, basic_block bb);
static void compute_global_livein (bitmap, bitmap);
static void set_def_block (tree, basic_block);
static void set_livein_block (tree, basic_block);
static bool prepare_operand_for_rename (tree *op_p, size_t *uid_p);
static void insert_phi_nodes (bitmap *);
static void rewrite_stmt (block_stmt_iterator, varray_type *);
static inline void rewrite_operand (tree *);
static void insert_phi_nodes_for (tree, bitmap *);
static tree get_reaching_def (tree);
static tree get_value_for (tree, varray_type);
static void set_value_for (tree, tree, varray_type);
static hashval_t def_blocks_hash (const void *);
static int def_blocks_eq (const void *, const void *);
static int debug_def_blocks_r (void **, void *);
static struct def_blocks_d *get_def_blocks_for (tree);
static void htab_statistics (FILE *, htab_t);

static tree create_temp (tree);
static void insert_copy_on_edge (edge, tree, tree);
static elim_graph new_elim_graph (int);
static inline void delete_elim_graph (elim_graph);
static inline void clear_elim_graph (elim_graph);
static inline int elim_graph_size (elim_graph);
static inline void elim_graph_add_node (elim_graph, tree);
static inline void elim_graph_add_edge (elim_graph, int, int);
static inline int elim_graph_remove_succ_edge (elim_graph, int);

static inline void eliminate_name (elim_graph, tree);
static void eliminate_build (elim_graph, basic_block, int);
static void elim_forward (elim_graph, int);
static int elim_unvisited_predecessor (elim_graph, int);
static void elim_backward (elim_graph, int);
static void elim_create (elim_graph, int);
static void eliminate_phi (edge, int, elim_graph);
static tree_live_info_p coalesce_ssa_name (var_map, int);
static void assign_vars (var_map);
static bool replace_variable (var_map, tree *, tree *);
static void eliminate_virtual_phis (void);
static void coalesce_abnormal_edges (var_map, conflict_graph, root_var_p);
static void print_exprs (FILE *, const char *, tree, const char *, tree,
			 const char *);
static void print_exprs_edge (FILE *, edge, const char *, tree, const char *,
			      tree);
static bool verify_def (basic_block, basic_block *, tree, tree);
static bool verify_use (basic_block, basic_block, tree, tree, bool);
static bool verify_phi_args (tree, basic_block, basic_block *);

/* Return the value associated with variable VAR in TABLE.  */

static inline tree
get_value_for (tree var, varray_type table)
{
  return VARRAY_TREE (table, var_ann (var)->uid);
}


/* Associate VALUE to variable VAR in TABLE.  */

static inline void
set_value_for (tree var, tree value, varray_type table)
{
  VARRAY_TREE (table, var_ann (var)->uid) = value;
}

/* Main entry point to the SSA builder.  FNDECL is the gimplified function
   to convert.  VARS is an sbitmap representing variables that need to be
   renamed into SSA form.  A variable in REFERENCED_VARS will be renamed
   into SSA if the element corresponding to the variable's UID is set in
   VARS.  If VARS is NULL, all variables in REFERENCED_VARS will be
   processed.

   PHASE indicates which dump file from the DUMP_FILES array to use when
   dumping debugging information.

   Every statement has two different sets of operands: real and virtual.
   Real operands are those that use scalar, non-aliased variables.  Virtual
   operands are everything else (pointers, arrays, compound structures).
   In addition, virtual operands are used to model side effects like
   call-clobbered variables.

   The conversion process works in four phases:

   1- Compute aliasing information (compute_may_aliases).
   2- Mark blocks that contain variable definitions (mark_def_sites).
   3- Insert PHI nodes (insert_phi_nodes).
   4- Rewrite all the basic blocks into SSA form (walk_dominator_tree).

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

	     	# MT.1_2 = VDEF <MT.1_1>;
	    1	b = 4;
	        # MT.1_3 = VDEF <MT.1_2>;
	    2	*p_3 = 5;
	     	# VUSE <MT.1_3>
	    3	... = b;

   Notice how variable 'b' is not rewritten anymore.  Instead, references
   to it are moved to the virtual operands which are all rewritten using
   MT.1, which is an artificial variable representing all the variables
   aliased by 'p'.

   Virtual operands provide safe information about potential references to
   the operands in a statement.  But they are imprecise by nature.
   Optimizations may want to take them into account, at the expense of
   increased compilation time.  */

void
rewrite_into_ssa (void)
{
  bitmap *dfs;
  basic_block bb;
  struct dom_walk_data walk_data;
  struct mark_def_sites_global_data mark_def_sites_global_data;
  
  timevar_push (TV_TREE_SSA_OTHER);

  /* Initialize the array of variables to rename.  */
  if (vars_to_rename != NULL)
    {
      size_t i;
      bool rename_name_tags_p;

      /* If any of the variables in VARS_TO_RENAME is a pointer, we need to
	 invalidate all the name memory tags associated with the variables
	 that we are about to rename.  FIXME: Currently we just invalidate
	 *all* the NMTs.  Make this more selective.  */
      rename_name_tags_p = false;
      EXECUTE_IF_SET_IN_BITMAP (vars_to_rename, 0, i,
	  if (POINTER_TYPE_P (TREE_TYPE (referenced_var (i))))
	    {
	      rename_name_tags_p = true;
	      break;
	    });

      if (rename_name_tags_p)
	for (i = 0; i < num_referenced_vars; i++)
	  {
	    var_ann_t ann = var_ann (referenced_var (i));

	    if (ann->mem_tag_kind == NAME_TAG)
	      bitmap_set_bit (vars_to_rename, ann->uid);
	  }

      /* Now remove all the existing PHI nodes (if any) for the variables
	 that we are about to rename into SSA.  */
      remove_all_phi_nodes_for (vars_to_rename);
    }

  /* Allocate memory for the DEF_BLOCKS hash table.  */
  def_blocks = htab_create (VARRAY_ACTIVE_SIZE (referenced_vars),
			    def_blocks_hash, def_blocks_eq, NULL);

  VARRAY_TREE_INIT (currdefs, num_referenced_vars, "currdefs");

  /* Initialize dominance frontier and immediate dominator bitmaps. 

     Also count the number of predecessors for each block.  Doing so
     can save significant time during PHI insertion for large graphs.  */
  dfs = (bitmap *) xmalloc (n_basic_blocks * sizeof (bitmap *));
  FOR_EACH_BB (bb)
    {
      edge e;
      int count = 0;

      for (e = bb->pred; e; e = e->pred_next)
	count++;

      bb_ann (bb)->num_preds = count;
      dfs[bb->index] = BITMAP_XMALLOC ();
    }

  /* Ensure that the dominance information is OK.  */
  calculate_dominance_info (CDI_DOMINATORS);

  /* Compute dominance frontiers.  */
  compute_dominance_frontiers (dfs);

  /* Setup callbacks for the generic dominator tree walker to find and
     mark definition sites.  */
  walk_data.initialize_block_local_data = NULL;
  walk_data.before_dom_children_before_stmts = NULL;
  walk_data.before_dom_children_walk_stmts = mark_def_sites;
  walk_data.before_dom_children_after_stmts = NULL; 
  walk_data.after_dom_children_before_stmts =  NULL;
  walk_data.after_dom_children_walk_stmts =  NULL;
  walk_data.after_dom_children_after_stmts =  NULL;

  /* Notice that this bitmap is indexed using variable UIDs, so it must be
     large enough to accommodate all the variables referenced in the
     function, not just the ones we are renaming.  */
  mark_def_sites_global_data.kills = sbitmap_alloc (num_referenced_vars);
  walk_data.global_data = &mark_def_sites_global_data;

  /* We do not have any local data.  */
  walk_data.block_local_data_size = 0;

  /* Initialize the dominator walker.  */
  init_walk_dominator_tree (&walk_data);

  /* Recursively walk the dominator tree.  */
  walk_dominator_tree (&walk_data, ENTRY_BLOCK_PTR);

  /* Finalize the dominator walker.  */
  fini_walk_dominator_tree (&walk_data);

  /* We no longer need this bitmap, clear and free it.  */
  sbitmap_free (mark_def_sites_global_data.kills);

  /* Insert PHI nodes at dominance frontiers of definition blocks.  */
  insert_phi_nodes (dfs);

  /* Rewrite all the basic blocks in the program.  */
  timevar_push (TV_TREE_SSA_REWRITE_BLOCKS);

  /* Setup callbacks for the generic dominator tree walker.  */
  walk_data.initialize_block_local_data = rewrite_initialize_block_local_data;
  walk_data.before_dom_children_before_stmts = rewrite_initialize_block;
  walk_data.before_dom_children_walk_stmts = rewrite_walk_stmts;
  walk_data.before_dom_children_after_stmts = rewrite_add_phi_arguments; 
  walk_data.after_dom_children_before_stmts =  NULL;
  walk_data.after_dom_children_walk_stmts =  NULL;
  walk_data.after_dom_children_after_stmts =  rewrite_finalize_block;
  walk_data.global_data = NULL;
  walk_data.block_local_data_size = sizeof (struct rewrite_block_data);

  /* Initialize the dominator walker.  */
  init_walk_dominator_tree (&walk_data);

  /* Recursively walk the dominator tree rewriting each statement in
     each basic block.  */
  walk_dominator_tree (&walk_data, ENTRY_BLOCK_PTR);

  /* Finalize the dominator walker.  */
  fini_walk_dominator_tree (&walk_data);

  timevar_pop (TV_TREE_SSA_REWRITE_BLOCKS);

  /* Debugging dumps.  */
  if (tree_dump_file && (tree_dump_flags & TDF_STATS))
    {
      dump_dfa_stats (tree_dump_file);
      dump_tree_ssa_stats (tree_dump_file);
    }

  /* Free allocated memory.  */
  FOR_EACH_BB (bb)
    BITMAP_XFREE (dfs[bb->index]);
  free (dfs);

  htab_delete (def_blocks);
  VARRAY_CLEAR (currdefs);

  timevar_pop (TV_TREE_SSA_OTHER);
}

struct tree_opt_pass pass_build_ssa = 
{
  "ssa",				/* name */
  NULL,					/* gate */
  rewrite_into_ssa,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg | PROP_referenced_vars,	/* properties_required */
  PROP_ssa,				/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_verify_ssa	/* todo_flags_finish */
};


/* Compute global livein information given the set of blockx where
   an object is locally live at the start of the block (LIVEIN)
   and the set of blocks where the object is defined (DEF_BLOCKS).

   Note: This routine augments the existing local livein information
   to include global livein.  Ie, it modifies the underlying bitmap
   for LIVEIN.  */

static void
compute_global_livein (bitmap livein, bitmap def_blocks)
{
  basic_block bb, *worklist, *tos;

  tos = worklist
    = (basic_block *) xmalloc (sizeof (basic_block) * (n_basic_blocks + 1));

  /* Initialize the worklist.  */
  FOR_EACH_BB (bb)
    {
      if (bitmap_bit_p (livein, bb->index))
	*tos++ = bb;
    }

  /* Iterate until the worklist is empty.  */
  while (tos != worklist)
    {
      edge e;

      /* Pull a block off the worklist.  */
      bb = *--tos;

      /* For each predecessor block.  */
      for (e = bb->pred; e; e = e->pred_next)
	{
	  basic_block pred = e->src;
	  int pred_index = pred->index;

	  /* None of this is necessary for the entry block.  */
	  if (pred != ENTRY_BLOCK_PTR
	      && ! bitmap_bit_p (livein, pred_index)
	      && ! bitmap_bit_p (def_blocks, pred_index))
	    {
	      *tos++ = pred;
	      bitmap_set_bit (livein, pred_index);
	    }
	}
    }

  free (worklist);
}

/* Collect definition sites for every variable in the function.
   Return a bitmap for the set of referenced variables which are
   "nonlocal", ie those which are live across block boundaries.
   This information is used to reduce the number of PHI nodes
   we create.  */

static void
mark_def_sites (struct dom_walk_data *walk_data, basic_block bb)
{
  struct mark_def_sites_global_data *gd = walk_data->global_data;
  sbitmap kills = gd->kills;
  block_stmt_iterator si;

  /* Mark all the blocks that have definitions for each variable in the
     VARS_TO_RENAME bitmap.  */
  sbitmap_zero (kills);

  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      vdef_optype vdefs;
      vuse_optype vuses;
      def_optype defs;
      use_optype uses;
      size_t i, uid;
      tree stmt;
      stmt_ann_t ann;

      stmt = bsi_stmt (si);
      get_stmt_operands (stmt);
      ann = stmt_ann (stmt);

      /* If a variable is used before being set, then the variable
         is live across a block boundary, so add it to NONLOCAL_VARS.  */
      uses = USE_OPS (ann);
      for (i = 0; i < NUM_USES (uses); i++)
        {
          tree *use_p = USE_OP_PTR (uses, i);

          if (prepare_operand_for_rename (use_p, &uid)
	      && !TEST_BIT (kills, uid))
	    set_livein_block (*use_p, bb);
	}
	  
      /* Similarly for virtual uses.  */
      vuses = VUSE_OPS (ann);
      for (i = 0; i < NUM_VUSES (vuses); i++)
        {
          tree *use_p = VUSE_OP_PTR (vuses, i);

          if (prepare_operand_for_rename (use_p, &uid)
	      && !TEST_BIT (kills, uid))
	    set_livein_block (*use_p, bb);
	}

      /* Note that virtual definitions are irrelevant for computing
	 KILLED_VARS because a VDEF does not constitute a killing
	 definition of the variable.  However, the operand of a virtual
	 definitions is a use of the variable, so it may affect
	 GLOBALS.  */
      vdefs = VDEF_OPS (ann);
      for (i = 0; i < NUM_VDEFS (vdefs); i++)
        {
	  size_t dummy;

          if (prepare_operand_for_rename (VDEF_OP_PTR (vdefs, i), &uid)
	      && prepare_operand_for_rename (VDEF_RESULT_PTR (vdefs, i), 
					     &dummy))
	    {
	      VDEF_RESULT (vdefs, i) = VDEF_OP (vdefs, i);

	      set_def_block (VDEF_RESULT (vdefs, i), bb);
	      if (!TEST_BIT (kills, uid))
		set_livein_block (VDEF_OP (vdefs, i), bb);
	    }
	}

      /* Now process the definition made by this statement.  */
      defs = DEF_OPS (ann);
      for (i = 0; i < NUM_DEFS (defs); i++)
        {
          tree *def_p = DEF_OP_PTR (defs, i);

          if (prepare_operand_for_rename (def_p, &uid))
	    {
	      set_def_block (*def_p, bb);
	      SET_BIT (kills, uid);
	    }
	}
    }
}

/* Mark block BB as the definition site for variable VAR.  */

static void
set_def_block (tree var, basic_block bb)
{
  struct def_blocks_d db, *db_p;
  void **slot;
  enum need_phi_state state = var_ann (var)->need_phi_state;

  /* Find the DEFS bitmap associated with variable VAR.  */
  db.var = var;
  slot = htab_find_slot (def_blocks, (void *) &db, INSERT);
  if (*slot == NULL)
    {
      db_p = ggc_alloc (sizeof (*db_p));
      db_p->var = var;
      db_p->def_blocks = BITMAP_GGC_ALLOC ();
      db_p->livein_blocks = BITMAP_GGC_ALLOC ();
      *slot = (void *) db_p;
    }
  else
    db_p = (struct def_blocks_d *) *slot;

  /* Set the bit corresponding to the block where VAR is defined.  */
  bitmap_set_bit (db_p->def_blocks, bb->index);

  /* Keep track of whether or not we may need to insert phi nodes.

     If we are in the UNKNOWN state, then this is the first definition
     of VAR.  Additionally, we have not seen any uses of VAR yet, so we
     do not need a phi node for this variable at this time (ie, transition
     to NEED_PHI_STATE_NO).

     If we are in any other state, then we either have multiple definitions
     of this variable occurring in different blocks or we saw a use of the
     variable which was not dominated by the block containing the
     definition(s).  In this case we may need a PHI node, so enter
     state NEED_PHI_STATE_MAYBE.  */
  if (state == NEED_PHI_STATE_UNKNOWN)
    var_ann (var)->need_phi_state = NEED_PHI_STATE_NO;
  else
    var_ann (var)->need_phi_state = NEED_PHI_STATE_MAYBE;
}

/* Mark block BB as having VAR live at the entry to BB.  */

static void
set_livein_block (tree var, basic_block bb)
{
  struct def_blocks_d db, *db_p;
  void **slot;
  enum need_phi_state state = var_ann (var)->need_phi_state;

  /* Find the DEFS bitmap associated with variable VAR.  */
  db.var = var;
  slot = htab_find_slot (def_blocks, (void *) &db, INSERT);
  if (*slot == NULL)
    {
      db_p = ggc_alloc (sizeof (*db_p));
      db_p->var = var;
      db_p->def_blocks = BITMAP_GGC_ALLOC ();
      db_p->livein_blocks = BITMAP_GGC_ALLOC ();
      *slot = (void *) db_p;
    }
  else
    db_p = (struct def_blocks_d *) *slot;

  /* Set the bit corresponding to the block where VAR is live in.  */
  bitmap_set_bit (db_p->livein_blocks, bb->index);

  /* Keep track of whether or not we may need to insert phi nodes.

     If we reach here in NEED_PHI_STATE_NO, see if this use is dominated
     by the single block containing the definition(s) of this variable.  If
     it is, then we remain in NEED_PHI_STATE_NO, otherwise we transition to
     NEED_PHI_STATE_MAYBE.  */
  if (state == NEED_PHI_STATE_NO)
    {
      int def_block_index = bitmap_first_set_bit (db_p->def_blocks);

      if (def_block_index == -1
	  || ! dominated_by_p (CDI_DOMINATORS, bb, BASIC_BLOCK (def_block_index)))
	var_ann (var)->need_phi_state = NEED_PHI_STATE_MAYBE;
    }
  else
    var_ann (var)->need_phi_state = NEED_PHI_STATE_MAYBE;
}


/* If the operand pointed by OP_P needs to be renamed, strip away SSA_NAME
   wrappers (if needed) and return true.  The unique ID for the operand's
   variable will be stored in *UID_P.  */

static bool
prepare_operand_for_rename (tree *op_p, size_t *uid_p)
{
  tree var = (TREE_CODE (*op_p) != SSA_NAME) ? *op_p : SSA_NAME_VAR (*op_p);
  *uid_p = var_ann (var)->uid;

  /* Ignore variables that don't need to be renamed.  */
  if (vars_to_rename && !bitmap_bit_p (vars_to_rename, *uid_p))
    return false;

  /* The variable needs to be renamed.  If it already had an
     SSA_NAME, strip it off.  This way, the SSA rename pass
     doesn't need to deal with existing SSA names.  */
  if (TREE_CODE (*op_p) == SSA_NAME)
    {
      if (default_def (SSA_NAME_VAR (*op_p)) != *op_p)
	release_ssa_name (*op_p);
      *op_p = var;
    }

  return true;
}

/* Insert PHI nodes at the dominance frontier of blocks with variable
   definitions.  DFS contains the dominance frontier information for the
   flowgraph.
   
   GLOBALS is a bitmap representing the set of variables which are live
   across basic block boundaries.  Only variables in GLOBALS need PHI
   nodes.  */

static void
insert_phi_nodes (bitmap *dfs)
{
  size_t i;

  timevar_push (TV_TREE_INSERT_PHI_NODES);

  /* Array WORK_STACK is a stack of CFG blocks.  Each block that contains
     an assignment or PHI node will be pushed to this stack.  */
  VARRAY_BB_INIT (work_stack, last_basic_block, "work_stack");

  /* Iterate over all variables in VARS_TO_RENAME.  For each variable, add
     to the work list all the blocks that have a definition for the
     variable.  PHI nodes will be added to the dominance frontier blocks of
     each definition block.  */
  if (vars_to_rename)
    EXECUTE_IF_SET_IN_BITMAP (vars_to_rename, 0, i,
      {
        tree var = referenced_var (i);
        var_ann_t ann = var_ann (var);
  
        if (ann->need_phi_state != NEED_PHI_STATE_NO)
  	  insert_phi_nodes_for (var, dfs);
      });
  else
    for (i = 0; i < num_referenced_vars; i++)
      {
	tree var = referenced_var (i);
	var_ann_t ann = var_ann (var);

	if (ann->need_phi_state != NEED_PHI_STATE_NO)
	  insert_phi_nodes_for (var, dfs);
      }

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
      
   3- All the PHI nodes in successor blocks of BB are visited.  The
      argument corresponding to BB is replaced with its current reaching
      definition.

   4- Recursively rewrite every dominator child block of BB.

   5- Restore (in reverse order) the current reaching definition for every
      new definition introduced in this block.  This is done so that when
      we return from the recursive call, all the current reaching
      definitions are restored to the names that were valid in the
      dominator parent of BB.  */

/* Initialize the local stacks.
     
   BLOCK_DEFS is used to save all the existing reaching definitions for
   the new SSA names introduced in this block.  Before registering a
   new definition for a variable, the existing reaching definition is
   pushed into this stack so that we can restore it in Step 5.  */

static void
rewrite_initialize_block_local_data (struct dom_walk_data *walk_data,
				     basic_block bb ATTRIBUTE_UNUSED,
				     bool recycled)
{
  struct rewrite_block_data *bd
    = (struct rewrite_block_data *)VARRAY_TOP_GENERIC_PTR (walk_data->block_data_stack);
                                                                                
  /* We get cleared memory from the allocator, so if the memory is
     not cleared, then we are re-using a previously allocated entry.  In
     that case, we can also re-use the underlying virtal arrays.  Just
     make sure we clear them before using them!  */
  if (recycled && bd->block_defs)
    VARRAY_CLEAR (bd->block_defs);
}

/* SSA Rewriting Step 1.  Initialization, create a block local stack
   of reaching definitions for new SSA names produced in this block
   (BLOCK_DEFS).  Register new definitions for every PHI node in the
   block.  */

static void
rewrite_initialize_block (struct dom_walk_data *walk_data, basic_block bb)
{
  tree phi;
  struct rewrite_block_data *bd
    = (struct rewrite_block_data *)VARRAY_TOP_GENERIC_PTR (walk_data->block_data_stack);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n\nRenaming block #%d\n\n", bb->index);

  /* Step 1.  Register new definitions for every PHI node in the block.
     Conceptually, all the PHI nodes are executed in parallel and each PHI
     node introduces a new version for the associated variable.  */
  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    {
      tree result = PHI_RESULT (phi);

      register_new_def (SSA_NAME_VAR (result), result,
			&bd->block_defs, currdefs);
    }
}

/* SSA Rewriting Step 2.  Rewrite every variable used in each statement in
   the block with its immediate reaching definitions.  Update the current
   definition of a variable when a new real or virtual definition is found.  */

static void
rewrite_walk_stmts (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
		    basic_block bb)
{
  block_stmt_iterator si;
  struct rewrite_block_data *bd
    = (struct rewrite_block_data *)VARRAY_TOP_GENERIC_PTR (walk_data->block_data_stack);

  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    rewrite_stmt (si, &bd->block_defs);
}


/* SSA Rewriting Step 3.  Visit all the successor blocks of BB looking for
   PHI nodes.  For every PHI node found, add a new argument containing the
   current reaching definition for the variable and the edge through which
   that definition is reaching the PHI node.   */

static void
rewrite_add_phi_arguments (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
			   basic_block bb)
{
  edge e;

  for (e = bb->succ; e; e = e->succ_next)
    {
      tree phi;

      for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
	{
	  tree currdef;

	  /* If this PHI node has already been rewritten, then there is
	     nothing to do for this PHI or any following PHIs since we
	     always add new PHI nodes at the start of the PHI chain.  */
	  if (PHI_REWRITTEN (phi))
	    break;

	  currdef = get_reaching_def (SSA_NAME_VAR (PHI_RESULT (phi)));
	  add_phi_arg (&phi, currdef, e);
	}
    }
}

/* SSA Rewriting Step 5.  Restore the current reaching definition for each
   variable referenced in the block (in reverse order).  */

static void
rewrite_finalize_block (struct dom_walk_data *walk_data,
			basic_block bb ATTRIBUTE_UNUSED)
{
  struct rewrite_block_data *bd
    = (struct rewrite_block_data *)VARRAY_TOP_GENERIC_PTR (walk_data->block_data_stack);

  /* Step 5.  Restore the current reaching definition for each variable
     referenced in the block (in reverse order).  */
  while (bd->block_defs && VARRAY_ACTIVE_SIZE (bd->block_defs) > 0)
    {
      tree var;
      tree saved_def = VARRAY_TOP_TREE (bd->block_defs);
      VARRAY_POP (bd->block_defs);
      
      /* If SAVED_DEF is NULL, then the next slot in the stack contains the
	 variable associated with SAVED_DEF.  */
      if (saved_def == NULL_TREE)
	{
	  var = VARRAY_TOP_TREE (bd->block_defs);
	  VARRAY_POP (bd->block_defs);
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
  add_referenced_tmp_var (tmp);

  /* add_referenced_tmp_var will create the annotation and set up some
     of the flags in the annotation.  However, some flags we need to
     inherit from our original variable.  */
  var_ann (tmp)->type_mem_tag = var_ann (t)->type_mem_tag;
  var_ann (tmp)->is_dereferenced_load = var_ann (t)->is_dereferenced_load;
  var_ann (tmp)->is_dereferenced_store = var_ann (t)->is_dereferenced_store;
  if (is_call_clobbered (t))
    mark_call_clobbered (tmp);
  var_ann (tmp)->is_stored = var_ann (t)->is_stored;

  return tmp;
}

/* This helper function fill insert a copy from a constant or a variable to 
   a variable on the specified edge.  */

static void
insert_copy_on_edge (edge e, tree dest, tree src)
{
  tree copy;

  copy = build (MODIFY_EXPR, TREE_TYPE (dest), dest, src);
  set_is_used (dest);

  if (TREE_CODE (src) == ADDR_EXPR)
    src = TREE_OPERAND (src, 0);
  if (TREE_CODE (src) == VAR_DECL || TREE_CODE (src) == PARM_DECL)
    set_is_used (src);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file,
	       "Inserting a copy on edge BB%d->BB%d :",
	       e->src->index,
	       e->dest->index);
      print_generic_expr (tree_dump_file, copy, tree_dump_flags);
      fprintf (tree_dump_file, "\n");
    }

  bsi_insert_on_edge (e, copy);
}

/* --------------------------------------------------------------------- */
/* Create an elimination graph and associated data structures.  */

static elim_graph
new_elim_graph (int size)
{
  elim_graph g = (elim_graph) xmalloc (sizeof (struct _elim_graph));

  VARRAY_TREE_INIT (g->nodes, 30, "Elimination Node List");
  VARRAY_TREE_INIT (g->const_copies, 20, "Elimination Constant Copies");
  VARRAY_INT_INIT (g->edge_list, 20, "Elimination Edge List");
  VARRAY_INT_INIT (g->stack, 30, " Elimination Stack");
  
  g->visited = sbitmap_alloc (size);

  return g;
}


/* Empty the elimination graph.  */
static inline void
clear_elim_graph (elim_graph g)
{
  VARRAY_POP_ALL (g->nodes);
  VARRAY_POP_ALL (g->edge_list);
}


/* Delete an elimination graph.  */
static inline void
delete_elim_graph (elim_graph g)
{
  sbitmap_free (g->visited);
  free (g);
}


/* Return the number of nodes in the graph.  */
static inline int
elim_graph_size (elim_graph g)
{
  return VARRAY_ACTIVE_SIZE (g->nodes);
}


/* Add a node to the graph, if it doesn't exist already.  */
static inline void 
elim_graph_add_node (elim_graph g, tree node)
{
  int x;
  for (x = 0; x < elim_graph_size (g); x++)
    if (VARRAY_TREE (g->nodes, x) == node)
      return;
  VARRAY_PUSH_TREE (g->nodes, node);
}


/* Add an edge to the graph.  */
static inline void
elim_graph_add_edge (elim_graph g, int pred, int succ)
{
  VARRAY_PUSH_INT (g->edge_list, pred);
  VARRAY_PUSH_INT (g->edge_list, succ);
}


/* Remove an edge from the graph for which node is the predecessor, and
   return the successor node.  -1 is returned if there is no such edge.  */
static inline int
elim_graph_remove_succ_edge (elim_graph g, int node)
{
  int y;
  unsigned x;
  for (x = 0; x < VARRAY_ACTIVE_SIZE (g->edge_list); x += 2)
    if (VARRAY_INT (g->edge_list, x) == node)
      {
        VARRAY_INT (g->edge_list, x) = -1;
	y = VARRAY_INT (g->edge_list, x + 1);
	VARRAY_INT (g->edge_list, x + 1) = -1;
	return y;
      }
  return -1;
}

/* Find all the nodes which are successors to NODE in the edge list.  */
#define FOR_EACH_ELIM_GRAPH_SUCC(GRAPH, NODE, VAR, CODE)		\
do {									\
  unsigned x_;								\
  int y_;								\
  for (x_ = 0; x_ < VARRAY_ACTIVE_SIZE ((GRAPH)->edge_list); x_ += 2)	\
    {									\
      y_ = VARRAY_INT ((GRAPH)->edge_list, x_);				\
      if (y_ != (NODE))							\
        continue;							\
      (VAR) = VARRAY_INT ((GRAPH)->edge_list, x_ + 1);			\
      CODE;								\
    }									\
} while (0)

/* Find all the nodes which are predecessors of NODE in the edge list.  */
#define FOR_EACH_ELIM_GRAPH_PRED(GRAPH, NODE, VAR, CODE)		\
do {									\
  unsigned x_;								\
  int y_;								\
  for (x_ = 0; x_ < VARRAY_ACTIVE_SIZE ((GRAPH)->edge_list); x_ += 2)	\
    {									\
      y_ = VARRAY_INT ((GRAPH)->edge_list, x_ + 1);			\
      if (y_ != (NODE))							\
        continue;							\
      (VAR) = VARRAY_INT ((GRAPH)->edge_list, x_);			\
      CODE;								\
    }									\
} while (0)

/* The following procedures implement the out of SSA algorithm detailed in 
   the Morgan Book pages 176-186.  */


/* Add T to the elimination graph.  */

static inline void
eliminate_name (elim_graph g, tree T)
{
  elim_graph_add_node (g, T);
}

/* Build the auxiliary graph.  */

static void
eliminate_build (elim_graph g, basic_block B, int i)
{
  tree phi;
  tree T0, Ti;
  int p0, pi;

  clear_elim_graph (g);
  
  for (phi = phi_nodes (B); phi; phi = TREE_CHAIN (phi))
    {
      T0 = var_to_partition_to_var (g->map, PHI_RESULT (phi));
      /* Ignore results which are not in partitions.  */
      if (T0 == NULL_TREE)
        {
#ifdef ENABLE_CHECKING
	  /* There should be no arguments of this PHI which are in
	     the partition list, or we get incorrect results.  */
	  for (pi = 0; pi < PHI_NUM_ARGS (phi); pi++)
	    {
	      tree arg = PHI_ARG_DEF (phi, pi);
	      if (TREE_CODE (arg) == SSA_NAME
		  && var_to_partition (g->map, arg) != NO_PARTITION)
		{
		  fprintf (stderr, "Argument of PHI is in a partition :(");
		  print_generic_expr (stderr, arg, TDF_SLIM);
		  fprintf (stderr, "), but the result is not :");
		  print_generic_stmt (stderr, phi, TDF_SLIM);
		  abort();
		}
	    }
#endif
	  continue;
	}
      if (PHI_ARG_EDGE (phi, i) == g->e)
	Ti = PHI_ARG_DEF (phi, i);
      else
        {
	  /* On rare occasions, a PHI node may not have the arguments
	     in the same order as all of the other PHI nodes. If they don't 
	     match, find the appropriate index here.  */
	  pi = phi_arg_from_edge (phi, g->e);
	  if (pi == -1)
	    abort();
	  Ti = PHI_ARG_DEF (phi, pi);
	}

      /* If this argument is a constant, or a SSA_NAME which is being
	 left in SSA form, just queue a copy to be emitted on this
	 edge.  */
      if (!phi_ssa_name_p (Ti)
	  || (TREE_CODE (Ti) == SSA_NAME
	      && var_to_partition (g->map, Ti) == NO_PARTITION))
        {
	  /* Save constant copies until all other copies have been emitted
	     on this edge.  */
	  VARRAY_PUSH_TREE (g->const_copies, T0);
	  VARRAY_PUSH_TREE (g->const_copies, Ti);
	}
      else
        {
	  Ti = var_to_partition_to_var (g->map, Ti);
	  if (T0 != Ti)
	    {
	      eliminate_name (g, T0);
	      eliminate_name (g, Ti);
	      p0 = var_to_partition (g->map, T0);
	      pi = var_to_partition (g->map, Ti);
	      elim_graph_add_edge (g, p0, pi);
	    }
	}
    }
}

/* Push successors onto the stack depth first.  */

static void 
elim_forward (elim_graph g, int T)
{
  int S;
  SET_BIT (g->visited, T);
  FOR_EACH_ELIM_GRAPH_SUCC (g, T, S,
    {
      if (!TEST_BIT (g->visited, S))
        elim_forward (g, S);
    });
  VARRAY_PUSH_INT (g->stack, T);
}


/* Are there unvisited predecessors?  */

static int
elim_unvisited_predecessor (elim_graph g, int T)
{
  int P;
  FOR_EACH_ELIM_GRAPH_PRED (g, T, P, 
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
  FOR_EACH_ELIM_GRAPH_PRED (g, T, P, 
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
      FOR_EACH_ELIM_GRAPH_PRED (g, T, P, 
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
      S = elim_graph_remove_succ_edge (g, T);
      if (S != -1)
	{
	  SET_BIT (g->visited, T);
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
  int num_nodes = 0;
  int x;
  basic_block B = e->dest;

#if defined ENABLE_CHECKING
  if (i == -1)
    abort ();
  if (VARRAY_ACTIVE_SIZE (g->const_copies) != 0)
    abort ();
#endif

  /* Abnormal edges already have everything coalesced, or the coalescer
     would have aborted.  */
  if (e->flags & EDGE_ABNORMAL)
    return;

  num_nodes = num_var_partitions (g->map);
  g->e = e;

  eliminate_build (g, B, i);

  if (elim_graph_size (g) != 0)
    {
      sbitmap_zero (g->visited);
      VARRAY_POP_ALL (g->stack);

      for (x = 0; x < elim_graph_size (g); x++)
        {
	  tree var = VARRAY_TREE (g->nodes, x);
	  int p = var_to_partition (g->map, var);
	  if (!TEST_BIT (g->visited, p))
	    elim_forward (g, p);
	}
       
      sbitmap_zero (g->visited);
      while (VARRAY_ACTIVE_SIZE (g->stack) > 0)
	{
	  x = VARRAY_TOP_INT (g->stack);
	  VARRAY_POP (g->stack);
	  if (!TEST_BIT (g->visited, x))
	    elim_create (g, x);
	}
    }

  /* If there are any pending constant copies, issue them now.  */
  while (VARRAY_ACTIVE_SIZE (g->const_copies) > 0)
    {
      tree src, dest;
      src = VARRAY_TOP_TREE (g->const_copies);
      VARRAY_POP (g->const_copies);
      dest = VARRAY_TOP_TREE (g->const_copies);
      VARRAY_POP (g->const_copies);
      insert_copy_on_edge (e, dest, src);
    }
}


/* Shortcut routine to print messages of the form: "str expr str expr str."  */

static void
print_exprs (FILE *f, const char *str1, tree expr1, const char *str2,
	     tree expr2, const char *str3)
{
  fprintf (f, "%s", str1);
  print_generic_expr (f, expr1, TDF_SLIM);
  fprintf (f, "%s", str2);
  print_generic_expr (f, expr2, TDF_SLIM);
  fprintf (f, "%s", str3);
}

static void
print_exprs_edge (FILE *f, edge e, const char *str1, tree expr1, 
		  const char *str2, tree expr2)
{
  print_exprs (f, str1, expr1, str2, expr2, " across an abnormal edge");
  fprintf (f, " from BB%d->BB%d\n", e->src->index,
	       e->dest->index);
}

/* Coalesce partitions which are live across abnormal edges. Since code 
   cannot be inserted on these edges, failure to coalesce something across
   an abnormal edge is a non-compilable situation.  */

static void
coalesce_abnormal_edges (var_map map, conflict_graph graph, root_var_p rv)
{
  basic_block bb;
  edge e;
  tree phi, var, tmp;
  int x, y;

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

	    /* Ignore results which are not relevant.  */
	    if (x == NO_PARTITION)
	      continue;

	    y = phi_arg_from_edge (phi, e);
	    if (y == -1)
	      abort ();

	    tmp = PHI_ARG_DEF (phi, y);
	    if (!phi_ssa_name_p (tmp))
	      {
	        print_exprs_edge (stderr, e,
				  "\nConstant argument in PHI. Can't insert :",
				  var, " = ", tmp);
		abort ();
	      }
	    y = var_to_partition (map, tmp);
	    if (x == NO_PARTITION || y == NO_PARTITION)
	      abort ();
	    if (root_var_find (rv, x) != root_var_find (rv, y))
	      {
		print_exprs_edge (stderr, e, "\nDifferent root vars: ",
				  root_var (rv, root_var_find (rv, x)), 
				  " and ", root_var (rv, root_var_find (rv, y)));
		abort ();
	      }

	    if (x != y)
	      {
		if (!conflict_graph_conflict_p (graph, x, y))
		  {
		    /* Now map the partitions back to their real variables.  */
		    var = partition_to_var (map, x);
		    tmp = partition_to_var (map, y);
		    if (tree_dump_file 
			&& (tree_dump_flags & TDF_DETAILS))
		      {
			print_exprs_edge (tree_dump_file, e, "ABNORMAL: Coalescing ",
					  var, " and ", tmp);
		      }
		    if (var_union (map, var, tmp) == NO_PARTITION)
		      {
			print_exprs_edge (stderr, e, "\nUnable to coalesce", 
					  partition_to_var (map, x), " and ", 
					  partition_to_var (map, y));
			abort ();
		      }
		    conflict_graph_merge_regs (graph, x, y);
		  }
		else
		  {
		    print_exprs_edge (stderr, e, "\n Conflict ", 
				      partition_to_var (map, x),
				      " and ", partition_to_var (map, y));
		    abort ();
		  }
	      }
	  }
}


/* Reduce the number of live ranges in the var_map. The only partitions
   which are associated with actual variables at this point are those which 
   are forced to be coalesced for various reason. (live on entry, live 
   across abnormal edges, etc.). 
   Live range information is returned if FLAGS indicates that we are
   combining temporaries, otherwise NULL is returned.  */

static tree_live_info_p
coalesce_ssa_name (var_map map, int flags)
{
  int num, x, i;
  sbitmap live;
  tree var, phi;
  root_var_p rv;
  tree_live_info_p liveinfo;
  var_ann_t ann;
  conflict_graph graph;
  basic_block bb;
  coalesce_list_p cl = NULL;

  if (num_var_partitions (map) <= 1)
    return NULL;

  /* If no preference given, use cheap coalescing of all partitions.  */
  if ((flags & (SSANORM_COALESCE_PARTITIONS | SSANORM_USE_COALESCE_LIST)) == 0)
    flags |= SSANORM_COALESCE_PARTITIONS;
  
  liveinfo = calculate_live_on_entry (map);
  calculate_live_on_exit (liveinfo);
  rv = root_var_init (map);

  /* Remove single element variable from the list.  */
  root_var_compact (rv);

  if (flags & SSANORM_USE_COALESCE_LIST)
    {
      cl = create_coalesce_list (map);
      
      /* Add all potential copies via PHI arguments to the list.  */
      FOR_EACH_BB (bb)
	{
	  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	    {
	      tree res = PHI_RESULT (phi);
	      int p = var_to_partition (map, res);
	      if (p == NO_PARTITION)
		continue;
	      for (x = 0; x < PHI_NUM_ARGS (phi); x++)
		{
		  tree arg = PHI_ARG_DEF (phi, x);
		  int p2;

		  if (TREE_CODE (arg) != SSA_NAME)
		    continue;
		  if (SSA_NAME_VAR (res) != SSA_NAME_VAR (arg))
		    continue;
		  p2 = var_to_partition (map, PHI_ARG_DEF (phi, x));
		  if (p2 != NO_PARTITION)
		    add_coalesce (cl, p, p2, 1);
		}
	    }
	}

      /* Coalesce all the result decls together.  */
      var = NULL_TREE;
      i = 0;
      for (x = 0; x < num_var_partitions (map); x++)
	{
	  tree p = partition_to_var (map, x);
	  if (TREE_CODE (SSA_NAME_VAR(p)) == RESULT_DECL)
	    {
	      if (var == NULL_TREE)
		{
		  var = p;
		  i = x;
		}
	      else
		add_coalesce (cl, i, x, 1);
	    }
	}
    }

  /* Build a conflict graph.  */
  graph = build_tree_conflict_graph (liveinfo, rv, cl);

  if (cl)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "Before sorting:\n");
	  dump_coalesce_list (tree_dump_file, cl);
	}

      sort_coalesce_list (cl);

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "\nAfter sorting:\n");
	  dump_coalesce_list (tree_dump_file, cl);
	}
    }

  /* Put the single element variables back in.  */
  root_var_decompact (rv);

  /* First, coalesce all live on entry variables to their root variable. 
     This will ensure the first use is coming from the correct location. */

  live = sbitmap_alloc (num_var_partitions (map));
  sbitmap_zero (live);

  /* Set 'live' vector to indicate live on entry partitions.  */
  num = num_var_partitions (map);
  for (x = 0 ; x < num; x++)
    {
      var = partition_to_var (map, x);
      if (default_def (SSA_NAME_VAR (var)) == var)
	SET_BIT (live, x);
    }

  if ((flags & SSANORM_COMBINE_TEMPS) == 0)
    {
      delete_tree_live_info (liveinfo);
      liveinfo = NULL;
    }

  /* Assign root variable as partition representative for each live on entry
     partition.  */
  EXECUTE_IF_SET_IN_SBITMAP (live, 0, x, 
    {
      var = root_var (rv, root_var_find (rv, x));
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

	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    {
	      print_exprs (tree_dump_file, "Must coalesce ", 
			   partition_to_var (map, x),
			   " with the root variable ", var, ".\n");
	    }

	  change_partition_var (map, var, x);
	}
    });

  sbitmap_free (live);

  /* Coalesce partitions live across abnormal edges.  */
  coalesce_abnormal_edges (map, graph, rv);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      dump_var_map (tree_dump_file, map);
    }

  /* Coalesce partitions.  */
  if (flags & SSANORM_USE_COALESCE_LIST)
    coalesce_tpa_members (rv, graph, map, cl, 
			  ((tree_dump_flags & TDF_DETAILS) ? tree_dump_file 
							   : NULL));

  
  if (flags & SSANORM_COALESCE_PARTITIONS)
    coalesce_tpa_members (rv, graph, map, NULL, 
			  ((tree_dump_flags & TDF_DETAILS) ? tree_dump_file 
							   : NULL));
  if (cl)
    delete_coalesce_list (cl);
  root_var_delete (rv);
  conflict_graph_delete (graph);

  return liveinfo;
}

/* Take the ssa-name var_map, and assign real variables to each partition.  */

static void
assign_vars (var_map map)
{
  int x, i, num, rep;
  tree t, var;
  var_ann_t ann;
  root_var_p rv;

  rv = root_var_init (map);
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
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_dump_file, "partition %d has variable ", x);
	      print_generic_expr (tree_dump_file, var, TDF_SLIM);
	      fprintf (tree_dump_file, " assigned to it.\n");
	    }

	}
    }

  num = root_var_num (rv);
  for (x = 0; x < num; x++)
    {
      var = root_var (rv, x);
      ann = var_ann (var);
      for (i = root_var_first_partition (rv, x);
	   i != ROOT_VAR_NONE;
	   i = root_var_next_partition (rv, i))
	{
	  t = partition_to_var (map, i);

	  if (t == var || TREE_CODE (t) != SSA_NAME)
	    continue;

	  rep = var_to_partition (map, t);
	  
	  if (!ann->out_of_ssa_tag)
	    {
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		print_exprs (tree_dump_file, "", t, "  --> ", var, "\n");
	      change_partition_var (map, var, rep);
	      continue;
	    }

	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    print_exprs (tree_dump_file, "", t, " not coalesced with ", var, 
			 "");

	  var = create_temp (t);
	  change_partition_var (map, var, rep);
	  ann = var_ann (var);

	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_dump_file, " -->  New temp:  '");
	      print_generic_expr (tree_dump_file, var, TDF_SLIM);
	      fprintf (tree_dump_file, "'\n");
	    }
	}
    }

  root_var_delete (rv);
}

/* Replace *p with whatever variable it has been rewritten to.  If it changes
   the stmt, return true.  */

static inline bool
replace_variable (var_map map, tree *p, tree *expr)
{
  tree new_var;
  tree var = *p;

  /* Check if we are replacing this variable with an expression.  */
  if (expr)
    {
      int version = SSA_NAME_VERSION (*p);
      if (expr[version])
        {
	  tree new_expr = TREE_OPERAND (expr[version], 1);
	  *p = new_expr;
	  /* Clear the stmt's RHS, or GC might bite us.  */
	  TREE_OPERAND (expr[version], 1) = NULL_TREE;
	  return true;
	}
    }

  new_var = var_to_partition_to_var (map, var);
  if (new_var)
    {
      *p = new_var;
      set_is_used (new_var);
      return true;
    }
  return false;
}


/* Remove any PHI node which is virtual PHI.  */
static void
eliminate_virtual_phis (void)
{
  basic_block bb;
  tree phi, next;

  FOR_EACH_BB (bb)
    {
      for (phi = phi_nodes (bb); phi; phi = next)
        {
	  next = TREE_CHAIN (phi);
	  if (!is_gimple_reg (SSA_NAME_VAR (PHI_RESULT (phi))))
	    {
#ifdef ENABLE_CHECKING
	      int i;
	      /* There should be no arguments of this PHI which are in
		 the partition list, or we get incorrect results.  */
	      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	        {
		  tree arg = PHI_ARG_DEF (phi, i);
		  if (TREE_CODE (arg) == SSA_NAME 
		      && is_gimple_reg (SSA_NAME_VAR (arg)))
		    {
		      fprintf (stderr, "Argument of PHI is not virtual (");
		      print_generic_expr (stderr, arg, TDF_SLIM);
		      fprintf (stderr, "), but the result is :");
		      print_generic_stmt (stderr, phi, TDF_SLIM);
		      abort();
		    }
		}
#endif
	      remove_phi_node (phi, NULL_TREE, bb);
	    }
	}
    }
}


/* This routine will coalesce variables of the same type which do not 
   interfere with each other. This will both reduce the memory footprint of
   the stack, and allow us to coalesce together local copies of globals and
   scalarized component refs.  */

static void
coalesce_vars (var_map map, tree_live_info_p liveinfo)
{
  basic_block bb;
  type_var_p tv;
  tree var;
  int x, p, p2;
  coalesce_list_p cl;
  conflict_graph graph;

  cl = create_coalesce_list (map);

  /* Merge all the live on entry vectors for coalesced partitions.  */
  for (x = 0; x < num_var_partitions (map); x++)
    {
      var = partition_to_var (map, x);
      p = var_to_partition (map, var);
      if (p != x)
        live_merge_and_clear (liveinfo, p, x);
    }

  /* When PHI nodes are turned into copies, the result of each PHI node
     becomes live on entry to the block. Mark these now.  */
  FOR_EACH_BB (bb)
    {
      tree phi, arg;
      int p;
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  p = var_to_partition (map, PHI_RESULT (phi));

	  /* Skip virtual PHI nodes.  */
	  if (p == NO_PARTITION)
	    continue;

	  make_live_on_entry (liveinfo, bb, p);

	  /* Each argument is a potential copy operation. Add any arguments 
	     which are not coalesced to the result to the coalesce list.  */
	  for (x = 0; x < PHI_NUM_ARGS (phi); x++)
	    {
	      arg = PHI_ARG_DEF (phi, x);
	      if (!phi_ssa_name_p (arg))
	        continue;
	      p2 = var_to_partition (map, arg);
	      if (p2 == NO_PARTITION)
		continue;
	      if (p != p2)
	        add_coalesce (cl, p, p2, 1);
	    }
	}
   }

  
  /* Re-calculate live on exit info.  */
  calculate_live_on_exit (liveinfo);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "Live range info for variable memory coalescing.\n");
      dump_live_info (tree_dump_file, liveinfo, LIVEDUMP_ALL);

      fprintf (tree_dump_file, "Coalesce list from phi nodes:\n");
      dump_coalesce_list (tree_dump_file, cl);
    }


  tv = type_var_init (map);
  if (tree_dump_file)
    type_var_dump (tree_dump_file, tv);
  type_var_compact (tv);
  if (tree_dump_file)
    type_var_dump (tree_dump_file, tv);

  graph = build_tree_conflict_graph (liveinfo, tv, cl);

  type_var_decompact (tv);
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "type var list now looks like:n");
      type_var_dump (tree_dump_file, tv);

      fprintf (tree_dump_file, "Coalesce list after conflict graph build:\n");
      dump_coalesce_list (tree_dump_file, cl);
    }

  sort_coalesce_list (cl);
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "Coalesce list after sorting:\n");
      dump_coalesce_list (tree_dump_file, cl);
    }

  coalesce_tpa_members (tv, graph, map, cl, 
			((tree_dump_flags & TDF_DETAILS) ? tree_dump_file : NULL));

  type_var_delete (tv);
  delete_coalesce_list (cl);
}


/* Temporary Expression Replacement (TER)

   Replace SSA version variables during out-of-ssa with their defining
   expression if there is only one use of the variable.

   A pass is made through the function, one block at a time. No cross block
   information is tracked.

   Variables which only have one use, and whose defining stmt is considered
   a replaceable expression (see check_replaceable) are entered into 
   consideration by adding a list of dependent partitions to the version_info
   vector for that ssa_name_version. This information comes from the partition
   mapping for each USE. At the same time, the partition_dep_list vector for 
   these partitions have this version number entered into their lists.

   When the use of a replaceable ssa_variable is encountered, the dependence
   list in version_info[] is moved to the "pending_dependence" list in case
   the current expression is also replaceable. (To be determined later in 
   processing this stmt.) version_info[] for the version is then updated to 
   point to the defining stmt and the 'replaceable' bit is set.

   Any partition which is defined by a statement 'kills' any expression which
   is dependent on this partition. Every ssa version in the partitions' 
   dependence list is removed from future consideration.

   All virtual references are lumped together. Any expression which is
   dependent on any virtual variable (via a VUSE) has a dependence added
   to the special partition defined by VIRTUAL_PARTITION.

   Whenever a VDEF is seen, all expressions dependent this VIRTUAL_PARTITION
   are removed from consideration.

   At the end of a basic block, all expression are removed from consideration
   in preparation for the next block.  
   
   The end result is a vector over SSA_NAME_VERSION which is passed back to
   rewrite_out_of_ssa.  As the SSA variables are being rewritten, instead of
   replacing the SSA_NAME tree element with the partition it was assigned, 
   it is replaced with the RHS of the defining expression.  */

/* Dependancy list element.  This can contain either a partition index or a
   version number, depending on which list it is in.  */
typedef struct value_expr_d 
{
  int value;
  struct value_expr_d *next;
} *value_expr_p;

/* Temporary Expression Replacement (TER) table information.  */
typedef struct temp_expr_table_d 
{
  var_map map;
  void **version_info;		
  value_expr_p *partition_dep_list;
  bitmap replaceable;
  bool saw_replaceable;
  int virtual_partition;
  bitmap partition_in_use;
  value_expr_p free_list;
  value_expr_p pending_dependence;
} *temp_expr_table_p;

/* Used to indicate a dependancy on VDEFs.  */
#define VIRTUAL_PARTITION(table)	(table->virtual_partition)

static temp_expr_table_p new_temp_expr_table (var_map);
static tree *free_temp_expr_table (temp_expr_table_p);
static inline value_expr_p new_value_expr (temp_expr_table_p);
static inline void free_value_expr (temp_expr_table_p, value_expr_p);
static inline value_expr_p find_value_in_list (value_expr_p, int, 
					       value_expr_p *);
static inline void add_value_to_list (temp_expr_table_p, value_expr_p *, int);
static inline void add_info_to_list (temp_expr_table_p, value_expr_p *, 
				     value_expr_p);
static value_expr_p remove_value_from_list (value_expr_p *, int);
static void add_dependance (temp_expr_table_p, int, tree);
static bool check_replaceable (temp_expr_table_p, tree);
static void finish_expr (temp_expr_table_p, int, bool);
static void mark_replaceable (temp_expr_table_p, tree);
static inline void kill_expr (temp_expr_table_p, int, bool);
static inline void kill_virtual_exprs (temp_expr_table_p, bool);
static void find_replaceable_in_bb (temp_expr_table_p, basic_block);
static tree *find_replaceable_exprs (var_map);
static void dump_replaceable_exprs (FILE *, tree *);

/* Create a new TER table.  */
static temp_expr_table_p
new_temp_expr_table (var_map map)
{
  temp_expr_table_p t;

  t = (temp_expr_table_p) xmalloc (sizeof (struct temp_expr_table_d));
  t->map = map;

  t->version_info = xcalloc (highest_ssa_version + 1, sizeof (void *));
  t->partition_dep_list = xcalloc (num_var_partitions (map) + 1, 
				   sizeof (value_expr_p));

  t->replaceable = BITMAP_XMALLOC ();
  t->partition_in_use = BITMAP_XMALLOC ();

  t->saw_replaceable = false;
  t->virtual_partition = num_var_partitions (map);
  t->free_list = NULL;
  t->pending_dependence = NULL;

  return t;
}


/* Free a TER table.  If there are valid replacements, return the expression 
   vector.  */
static tree *
free_temp_expr_table (temp_expr_table_p t)
{
  value_expr_p p;
  tree *ret = NULL;

#ifdef ENABLE_CHECKING
  int x;
  for (x = 0; x <= num_var_partitions (t->map); x++)
    if (t->partition_dep_list[x] != NULL)
      abort();
#endif

  while ((p = t->free_list))
    {
      t->free_list = p->next;
      free (p);
    }

  BITMAP_XFREE (t->partition_in_use);
  BITMAP_XFREE (t->replaceable);

  free (t->partition_dep_list);
  if (t->saw_replaceable)
    ret = (tree *)t->version_info;
  else
    free (t->version_info);
  
  free (t);
  return ret;
}


/* Allocate a new value list node. Take it from the free list if possible.  */
static inline value_expr_p
new_value_expr (temp_expr_table_p table)
{
  value_expr_p p;
  if (table->free_list)
    {
      p = table->free_list;
      table->free_list = p->next;
    }
  else
    p = (value_expr_p) xmalloc (sizeof (struct value_expr_d));

  return p;
}


/* Add a value list node to the free list.  */
static inline void
free_value_expr (temp_expr_table_p table, value_expr_p p)
{
  p->next = table->free_list;
  table->free_list = p;
}


/* Find a specific value if its in a list.  Return a pointer to the list 
   object if found.  Return NULL if it isn't.  If last_ptr is provided,
   it will point to the previous item upon return, or NULL if this is the 
   first item in the list.  */
static inline value_expr_p
find_value_in_list (value_expr_p list, int value, value_expr_p *last_ptr)
{
  value_expr_p curr;
  value_expr_p last = NULL;

  for (curr = list; curr; last = curr, curr = curr->next)
    {
      if (curr->value == value)
        break;
    }
  if (last_ptr)
    *last_ptr = last;
  return curr;
}


/* Add a value to a list, if it isn't already present.  */
static inline void
add_value_to_list (temp_expr_table_p tab, value_expr_p *list, int value)
{
  value_expr_p info;

  if (!find_value_in_list (*list, value, NULL))
    {
      info = new_value_expr (tab);
      info->value = value;
      info->next = *list;
      *list = info;
    }
}


/* Add a value node if it's value isn't already in the list.  Free this node if
   it is already in the list.  */
static inline void
add_info_to_list (temp_expr_table_p tab, value_expr_p *list, value_expr_p info)
{
  if (find_value_in_list (*list, info->value, NULL))
    free_value_expr (tab, info);
  else
    {
      info->next = *list;
      *list = info;
    }
}


/* Look for a value in a list.  If found, remove it from the list and return 
   it's pointer.  */
static value_expr_p
remove_value_from_list (value_expr_p *list, int value)
{
  value_expr_p info, last;

  info = find_value_in_list (*list, value, &last);
  if (!info)
    return NULL;
  if (!last)
    *list = info->next;
  else
    last->next = info->next;
 
  return info;
}


/* Add a dependancy between the def of an SSA version and the partitions each
   use in the expression represent.  */
static void
add_dependance (temp_expr_table_p tab, int version, tree var)
{
  int i, x;
  value_expr_p info;

  i = SSA_NAME_VERSION (var);
  if (bitmap_bit_p (tab->replaceable, i))
    {
      /* This variable is being substituted, so use whatever dependences
         were queued up when we marked this as replaceable earlier.  */
      while ((info = tab->pending_dependence))
        {
	  tab->pending_dependence = info->next;
	  /* Get the partition this variable was dependent on. Reuse this
	     object to represent the current  expression instead.  */
	  x = info->value;
	  info->value = version;
	  add_info_to_list (tab, &(tab->partition_dep_list[x]), info);
          add_value_to_list (tab, 
			     (value_expr_p *)&(tab->version_info[version]), x);
	  bitmap_set_bit (tab->partition_in_use, x);
	}
    }
  else
    {
      i = var_to_partition (tab->map, var);
#ifdef ENABLE_CHECKING
      if (i== NO_PARTITION)
	abort ();
#endif
      add_value_to_list (tab, &(tab->partition_dep_list[i]), version);
      add_value_to_list (tab, (value_expr_p *)&(tab->version_info[version]), i);
      bitmap_set_bit (tab->partition_in_use, i);
    }
}


/* Check if an expression is suitable for replacement.  If so, create an 
   expression entry.  Return true if this stmt is replaceable.  */

static bool
check_replaceable (temp_expr_table_p tab, tree stmt)
{
  stmt_ann_t ann;
  vuse_optype vuseops;
  def_optype defs;
  use_optype uses;
  tree var, def;
  int num_use_ops, version, i;
  var_map map = tab->map;

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return false;
  
  ann = stmt_ann (stmt);
  defs = DEF_OPS (ann);

  /* Punt if there is more than 1 def, or more than 1 use.  */
  if (NUM_DEFS (defs) != 1)
    return false;
  def = DEF_OP (defs, 0);
  if (version_ref_count (map, def) != 1)
    return false;

  /* Assignments to variables assigned to hard registers are not
     replaceable.  */
  if (DECL_HARD_REGISTER (SSA_NAME_VAR (def)))
    return false;

  /* There must be no VDEFS.  */
  if (NUM_VDEFS (VDEF_OPS (ann)) != 0)
    return false;

  /* Float expressions must go through memory if float-store is on.  */
  if (flag_float_store && FLOAT_TYPE_P (TREE_TYPE (TREE_OPERAND (stmt, 1))))
    return false;

  uses = USE_OPS (ann);
  num_use_ops = NUM_USES (uses);
  vuseops = VUSE_OPS (ann);

  /* Any expression which has no virtual operands and no real operands
     should have been propagated if it's possible to do anything with them. 
     If this happens here, it probably exists that way for a reason, so we 
     won't touch it.   An example is:
         b_4 = &tab
     There are no virtual uses nor any real uses, so we just leave this 
     alone to be safe.  */

  if (num_use_ops == 0 && NUM_VUSES (vuseops) == 0)
    return false;

  version = SSA_NAME_VERSION (def);

  /* Add this expression to the dependancy list for each use partition.  */
  for (i = 0; i < num_use_ops; i++)
    {
      var = USE_OP (uses, i);
      add_dependance (tab, version, var);
    }

  /* If there are VUSES, add a dependence on virtual defs.  */
  if (NUM_VUSES (vuseops) != 0)
    {
      add_value_to_list (tab, (value_expr_p *)&(tab->version_info[version]), 
			 VIRTUAL_PARTITION (tab));
      add_value_to_list (tab, 
			 &(tab->partition_dep_list[VIRTUAL_PARTITION (tab)]), 
			 version);
      bitmap_set_bit (tab->partition_in_use, VIRTUAL_PARTITION (tab));
    }

  return true;
}


/* This function will remove an expression from replacement consideration.  If
   'replace' is true, it is marked as replaceable, otherwise not.  */
static void
finish_expr (temp_expr_table_p tab, int version, bool replace)
{
  value_expr_p info, tmp;
  int partition;

  /* Remove this expression from its dependent lists.  The partition dependance
     list is retained and transfered later to whomever uses this version.  */
  for (info = (value_expr_p) tab->version_info[version]; info; info = tmp)
    {
      partition = info->value;
#ifdef ENABLE_CHECKING
      if (tab->partition_dep_list[partition] == NULL)
        abort ();
#endif
      tmp = remove_value_from_list (&(tab->partition_dep_list[partition]), 
				    version);
#ifdef ENABLE_CHECKING
      if (!tmp)
        abort ();
#endif
      free_value_expr (tab, tmp);
      /* Only clear the bit when the dependancy list is emptied via 
         a replacement. Otherwise kill_expr will take care of it.  */
      if (!(tab->partition_dep_list[partition]) && replace)
        bitmap_clear_bit (tab->partition_in_use, partition);
      tmp = info->next;
      if (!replace)
        free_value_expr (tab, info);
    }

  if (replace)
    {
      tab->saw_replaceable = true;
      bitmap_set_bit (tab->replaceable, version);
    }
  else
    {
#ifdef ENABLE_CHECKING
      if (bitmap_bit_p (tab->replaceable, version))
	abort ();
#endif
      tab->version_info[version] = NULL;
    }
}


/* Mark the expression associated with a variable as replaceable, and enter
   the defining stmt into the version_info table.  */
static void
mark_replaceable (temp_expr_table_p tab, tree var)
{
  value_expr_p info;
  int version = SSA_NAME_VERSION (var);
  finish_expr (tab, version, true);

  /* Move the dependence list to the pending list.  */
  if (tab->version_info[version])
    {
      info = (value_expr_p) tab->version_info[version]; 
      for ( ; info->next; info = info->next)
	continue;
      info->next = tab->pending_dependence;
      tab->pending_dependence = (value_expr_p)tab->version_info[version];
    }

  tab->version_info[version] = SSA_NAME_DEF_STMT (var);
}


/* This function finishes any expression which is dependent on this partition
   as NOT replaceable.  clear_bit is used to determine whether partition_in_use
   should have iuts bit cleared. Since this can be called within an
   EXECUTE_IF_SET_IN_BITMAP, the bit can't always be cleared.  */
static inline void
kill_expr (temp_expr_table_p tab, int partition, bool clear_bit)
{
  value_expr_p ptr;

  /* Mark every active expr dependant on this var as not replaceable.  */
  while ((ptr = tab->partition_dep_list[partition]) != NULL)
    finish_expr (tab, ptr->value, false);

  if (clear_bit)
    bitmap_clear_bit (tab->partition_in_use, partition);
}


/* This function kills all expressions which are dependant on virtual DEFs.  */
static inline void
kill_virtual_exprs (temp_expr_table_p tab, bool clear_bit)
{
  kill_expr (tab, VIRTUAL_PARTITION (tab), clear_bit);
}


/* This function processes a basic block, and looks for variables which can
   be replaced by their expressions.  */
static void
find_replaceable_in_bb (temp_expr_table_p tab, basic_block bb)
{
  block_stmt_iterator bsi;
  tree stmt, def;
  stmt_ann_t ann;
  int partition, num, i;
  use_optype uses;
  def_optype defs;
  var_map map = tab->map;
  value_expr_p p;

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      stmt = bsi_stmt (bsi);
      ann = stmt_ann (stmt);

      /* Determine if this stmt finishes an existing expression.  */
      uses = USE_OPS (ann);
      num = NUM_USES (uses);
      for (i = 0; i < num; i++)
	{
	  def = USE_OP (uses, i);
	  if (tab->version_info[SSA_NAME_VERSION (def)])
	    mark_replaceable (tab, def);
	}
      
      /* Next, see if this stmt kills off an active expression.  */
      defs = DEF_OPS (ann);
      num = NUM_DEFS (defs);
      for (i = 0; i < num; i++)
	{
	  def = DEF_OP (defs, i);
	  partition = var_to_partition (map, def);
	  if (partition != NO_PARTITION && tab->partition_dep_list[partition])
	    kill_expr (tab, partition, true);
	}

      /* Now see if we are creating a new expression or not.  */
      check_replaceable (tab, stmt);

      /* Free any unused dependancy lists.  */
      while ((p = tab->pending_dependence))
	{
	  tab->pending_dependence = p->next;
	  free_value_expr (tab, p);
	}

      /* A VDEF kills any expression using a virtual operand.  */
      if (NUM_VDEFS (VDEF_OPS (ann)) > 0)
        kill_virtual_exprs (tab, true);
    }
}


/* This function is the driver routine for replacement of temporary expressions
   in the SSA->normal phase.  If there are replaceable expressions, a table is 
   returned which maps SSA versions to the expressions they should be replaced 
   with.  A NULL_TREE indicates no replacement should take place.  
   If there are no replacements at all, NULL is returned by the function.  */
static tree *
find_replaceable_exprs (var_map map)
{
  basic_block bb;
  int i;
  temp_expr_table_p table;
  tree *ret;

  table = new_temp_expr_table (map);
  FOR_EACH_BB (bb)
    {
      find_replaceable_in_bb (table, bb);
      EXECUTE_IF_SET_IN_BITMAP ((table->partition_in_use), 0, i,
        {
	  kill_expr (table, i, false);
	});
    }

  ret = free_temp_expr_table (table);
  return ret;
}


/* Dump the TER expression table.  */
static void
dump_replaceable_exprs (FILE *f, tree *expr)
{
  tree stmt, var;
  int x;
  fprintf (f, "\nReplacing Expressions\n");
  for (x = 0; x < (int)highest_ssa_version + 1; x++)
    if (expr[x])
      {
        stmt = expr[x];
	var = DEF_OP (STMT_DEF_OPS (stmt), 0);
	print_generic_expr (f, var, TDF_SLIM);
	fprintf (f, " replace with --> ");
	print_generic_expr (f, TREE_OPERAND (stmt, 1), TDF_SLIM);
	fprintf (f, "\n");
      }
  fprintf (f, "\n");
}


/* This function will rewrite the current program using the variable mapping
   found in 'map'. If the replacement vector 'values' is provided, any 
   occurrences of partitions with non-null entries in the vector will be 
   replaced with the expression in the vector instead of its mapped 
   variable.  */

static void
rewrite_trees (var_map map, tree *values)
{
  elim_graph g;
  basic_block bb;
  block_stmt_iterator si;
  edge e;
  tree phi;
  bool changed;

  /* Replace PHI nodes with any required copies.  */
  g = new_elim_graph (map->num_partitions);
  g->map = map;
  FOR_EACH_BB (bb)
    {
      for (si = bsi_start (bb); !bsi_end_p (si); )
	{
	  size_t i, num_uses, num_defs;
	  use_optype uses;
	  def_optype defs;
	  tree stmt = bsi_stmt (si);
	  tree *use_p = NULL;
	  int remove = 0, is_copy = 0;
	  stmt_ann_t ann;

	  get_stmt_operands (stmt);
	  ann = stmt_ann (stmt);
	  changed = false;

	  if (TREE_CODE (stmt) == MODIFY_EXPR 
	      && (TREE_CODE (TREE_OPERAND (stmt, 1)) == SSA_NAME))
	    is_copy = 1;

	  uses = USE_OPS (ann);
	  num_uses = NUM_USES (uses);

	  for (i = 0; i < num_uses; i++)
	    {
	      use_p = USE_OP_PTR (uses, i);
	      if (replace_variable (map, use_p, values))
	        changed = true;
	    }

	  defs = DEF_OPS (ann);
	  num_defs = NUM_DEFS (defs);

	  if (values && num_defs == 1)
	    {
	      tree def = DEF_OP (defs, 0);
	      tree val;
	      val = values[SSA_NAME_VERSION (def)];
	      if (val)
		remove = 1;
	    }
	  if (!remove)
	    {
	      for (i = 0; i < num_defs; i++)
		{
		  tree *def_p = DEF_OP_PTR (defs, i);

		  if (replace_variable (map, def_p, NULL))
		    changed = true;

		  if (is_copy
		      && num_uses == 1
		      && use_p
		      && def_p
		      && (*def_p == *use_p))
		    remove = 1;
		}
	      if (changed)
		modify_stmt (stmt);
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
	}
    }

  delete_elim_graph (g);

  /* If any copies were inserted on edges, actually insert them now.  */
  bsi_commit_edge_inserts (NULL);
}

/* Remove the variables specified in a var map from SSA form.  */
void
remove_ssa_form (FILE *dump, var_map map, int flags)
{
  tree_live_info_p liveinfo;
  basic_block bb;
  tree phi, next;
  FILE *save;
  tree *values = NULL;

  save = tree_dump_file;
  tree_dump_file = dump;

  /* If we are not combining temps, dont calculate live ranges fo variables
     with only one SSA version.  */
  if ((flags & SSANORM_COMBINE_TEMPS) == 0)
    compact_var_map (map, VARMAP_NO_SINGLE_DEFS);
  else
    compact_var_map (map, VARMAP_NORMAL);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    dump_var_map (tree_dump_file, map);

  liveinfo = coalesce_ssa_name (map, flags);

  /* Make sure even single occurrence variables are in the list now.  */
  if ((flags & SSANORM_COMBINE_TEMPS) == 0)
    compact_var_map (map, VARMAP_NORMAL);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "After Coalescing:\n");
      dump_var_map (tree_dump_file, map);
    }

  if (flags & SSANORM_PERFORM_TER)
    {
      values = find_replaceable_exprs (map);
      if (values && tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	dump_replaceable_exprs (tree_dump_file, values);
    }

  /* Assign real variables to the partitions now.  */
  assign_vars (map);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "After Root variable replacement:\n");
      dump_var_map (tree_dump_file, map);
    }

  if ((flags & SSANORM_COMBINE_TEMPS) && liveinfo)
    {
      coalesce_vars (map, liveinfo);
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "After variable memory coalescing:\n");
	  dump_var_map (tree_dump_file, map);
	}
    }
  
  if (liveinfo)
    delete_tree_live_info (liveinfo);

  rewrite_trees (map, values);

  if (values)
    free (values);

  /* Remove phi nodes which have been translated back to real variables.  */
  FOR_EACH_BB (bb)
    {
      for (phi = phi_nodes (bb); phi; phi = next)
	{
	  next = TREE_CHAIN (phi);
	  if ((flags & SSANORM_REMOVE_ALL_PHIS) 
	      || var_to_partition (map, PHI_RESULT (phi)) != NO_PARTITION)
	    remove_phi_node (phi, NULL_TREE, bb);
	}
    }

  tree_dump_file = save;
}

/* Take a subset of the variables (VARS) in the current function out of SSA
   form.  */

void
rewrite_vars_out_of_ssa (bitmap vars)
{
  if (bitmap_first_set_bit (vars) >= 0)
    {
      var_map map;
      basic_block bb;
      tree phi;
      int i;
      int ssa_flags;

      /* Search for PHIs in which one of the PHI arguments is marked for
	 translation out of SSA form, but for which the PHI result is not
	 marked for translation out of SSA form.

	 Our per-variable out of SSA translation can not handle that case;
	 however we can easily handle it here by creating a new instance
	 of the PHI result's underlying variable and initializing it to
	 the offending PHI argument on the edge associated with the
	 PHI argument.  We then change the PHI argument to use our new
	 instead of the PHI's underlying variable.

	 You might think we could register partitions for the out-of-ssa
	 translation here and avoid a second walk of the PHI nodes.  No
	 such luck since the size of the var map will change if we have
	 to manually take variables out of SSA form here.  */
      FOR_EACH_BB (bb)
	{
	  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	    {
	      tree result = SSA_NAME_VAR (PHI_RESULT (phi));

	      /* If the definition is marked for renaming, then we need
		 to do nothing more for this PHI node.  */
	      if (bitmap_bit_p (vars, var_ann (result)->uid))
		continue;

	      /* Look at all the arguments and see if any of them are
		 marked for renaming.  If so, we need to handle them
		 specially.  */
	      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
		{
		  tree arg = PHI_ARG_DEF (phi, i);

		  /* If the argument is not an SSA_NAME, then we can ignore
		     this argument.  */
		  if (TREE_CODE (arg) != SSA_NAME)
		    continue;

		  /* If this argument is marked for renaming, then we need
		     to undo the copy propagation so that we can take
		     the argument out of SSA form without taking the
		     result out of SSA form.  */
		  arg = SSA_NAME_VAR (arg);
		  if (bitmap_bit_p (vars, var_ann (arg)->uid))
		    {
		      tree new_name, copy;

		      /* Get a new SSA_NAME for the copy, it is based on
			 the result, not the argument!   We use the PHI
			 as the definition since we haven't created the
			 definition statement yet.  */
		      new_name = make_ssa_name (result, phi);

		      /* Now create the copy statement.  */
		      copy = build (MODIFY_EXPR, TREE_TYPE (arg),
				    new_name, PHI_ARG_DEF (phi, i));

		      /* Now update SSA_NAME_DEF_STMT to point to the
			 newly created statement.  */
		      SSA_NAME_DEF_STMT (new_name) = copy;

		      /* Now make the argument reference our new SSA_NAME.  */
		      PHI_ARG_DEF (phi, i) = new_name;

		      /* Queue the statement for insertion.  */
		      bsi_insert_on_edge (PHI_ARG_EDGE (phi, i), copy);
		      modify_stmt (copy);
		    }
		}
	    }
	}

      /* If any copies were inserted on edges, actually insert them now.  */
      bsi_commit_edge_inserts (NULL);
                                                                                
      /* Now register partitions for all instances of the variables we
	 are taking out of SSA form.  */
      map = init_var_map (highest_ssa_version + 1);
      register_ssa_partitions_for_vars (vars, map);

      /* Now that we have all the partitions registered, translate the
	 appropriate variables out of SSA form.  */
      ssa_flags = SSANORM_COALESCE_PARTITIONS;
      if (flag_tree_combine_temps)
	ssa_flags |= SSANORM_COMBINE_TEMPS;
      remove_ssa_form (tree_dump_file, map, ssa_flags);

      /* And finally, reset the out_of_ssa flag for each of the vars
	 we just took out of SSA form.  */
      EXECUTE_IF_SET_IN_BITMAP (vars, 0, i,
	{
	  var_ann (referenced_var (i))->out_of_ssa_tag = 0;
	});

    }
}

/* Take function FNDECL out of SSA form.

   PHASE indicates which dump file from the TREE_DUMP_FILES array to use when
   dumping debugging information.  */

static void
rewrite_out_of_ssa (void)
{
  var_map map;
  int var_flags = 0;
  int ssa_flags = (SSANORM_REMOVE_ALL_PHIS | SSANORM_COALESCE_PARTITIONS);

  eliminate_virtual_phis ();

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    dump_tree_cfg (tree_dump_file, tree_dump_flags & ~TDF_DETAILS);

  /* We cannot allow unssa to un-gimplify trees before we instrument them.  */
  if (flag_tree_ter && !flag_mudflap)
    var_flags = SSA_VAR_MAP_REF_COUNT;

  map = create_ssa_var_map (var_flags);

  if (flag_tree_combine_temps)
    ssa_flags |= SSANORM_COMBINE_TEMPS;
  if (flag_tree_ter && !flag_mudflap)
    ssa_flags |= SSANORM_PERFORM_TER;

  remove_ssa_form (tree_dump_file, map, ssa_flags);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    dump_tree_cfg (tree_dump_file, tree_dump_flags & ~TDF_DETAILS);

  /* Do some cleanups which reduce the amount of data the
     tree->rtl expanders deal with.  */
  cfg_remove_useless_stmts ();

  /* Remove unnecessary variables.  */
  remove_useless_vars ();

  /* Flush out flow graph and SSA data.  */
  delete_var_map (map);

  /* Mark arrays indexed with non-constant indices with TREE_ADDRESSABLE.
     FIXME: Is this really needed long-term?  This is done for the benefit
     of the RTL expanders.  */
  discover_nonconstant_array_refs ();
}

struct tree_opt_pass pass_del_ssa = 
{
  "optimized",				/* name */
  NULL,					/* gate */
  rewrite_out_of_ssa,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_SSA_TO_NORMAL,		/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  /* ??? If TER is enabled, we also kill gimple.  */
  PROP_ssa,				/* properties_destroyed */
  TODO_verify_ssa | TODO_verify_flow
    | TODO_verify_stmts,		/* todo_flags_start */
  TODO_dump_func | TODO_ggc_collect	/* todo_flags_finish */
};

/* Remove edge E and remove the corresponding arguments from the PHI nodes
   in E's destination block.  */

void
ssa_remove_edge (edge e)
{
  tree phi;

  /* Remove the appropriate PHI arguments in E's destination block.  */
  for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
    remove_phi_arg (phi, e->src);

  remove_edge (e);
}

/* Remove remove the corresponding arguments from the PHI nodes
   in E's destination block and redirect it to DEST.  Return redirected edge.
   The list of removed arguments is stored in PENDING_STMT (e).  */

edge
ssa_redirect_edge (edge e, basic_block dest)
{
  tree phi;
  tree list = NULL, *last = &list;
  tree src, dst, node;
  int i;

  /* Remove the appropriate PHI arguments in E's destination block.  */
  for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
    {
      i = phi_arg_from_edge (phi, e);
      if (i < 0)
	continue;

      src = PHI_ARG_DEF (phi, i);
      dst = PHI_RESULT (phi);
      node = build_tree_list (dst, src);
      *last = node;
      last = &TREE_CHAIN (node);

      remove_phi_arg_num (phi, i);
    }

  e = redirect_edge_succ_nodup (e, dest);
  PENDING_STMT (e) = list;

  return e;
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
      dump_bb (bb, file, 0);
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



/* Return true if the definition of SSA_NAME at block BB is malformed.

   STMT is the statement where SSA_NAME is created.

   DEFINITION_BLOCK is an array of basic blocks indexed by SSA_NAME version
      numbers.  If DEFINITION_BLOCK[SSA_NAME_VERSION] is set, it means that the
      block in that array slot contains the definition of SSA_NAME.  */

static bool
verify_def (basic_block bb, basic_block *definition_block, tree ssa_name,
	    tree stmt)
{
  bool err = false;

  if (TREE_CODE (ssa_name) != SSA_NAME)
    {
      error ("Expected an SSA_NAME object");
      debug_generic_stmt (ssa_name);
      debug_generic_stmt (stmt);
    }

  if (definition_block[SSA_NAME_VERSION (ssa_name)])
    {
      error ("SSA_NAME created in two different blocks %i and %i",
	     definition_block[SSA_NAME_VERSION (ssa_name)]->index, bb->index);
      fprintf (stderr, "SSA_NAME: ");
      debug_generic_stmt (ssa_name);
      debug_generic_stmt (stmt);
      err = true;
    }

  definition_block[SSA_NAME_VERSION (ssa_name)] = bb;

  if (SSA_NAME_DEF_STMT (ssa_name) != stmt)
    {
      error ("SSA_NAME_DEF_STMT is wrong");
      fprintf (stderr, "SSA_NAME: ");
      debug_generic_stmt (ssa_name);
      fprintf (stderr, "Expected definition statement:\n");
      debug_generic_stmt (SSA_NAME_DEF_STMT (ssa_name));
      fprintf (stderr, "\nActual definition statement:\n");
      debug_generic_stmt (stmt);
      err = true;
    }

  return err;
}


/* Return true if the use of SSA_NAME at statement STMT in block BB is
   malformed.

   DEF_BB is the block where SSA_NAME was found to be created.

   IDOM contains immediate dominator information for the flowgraph.

   CHECK_ABNORMAL is true if the caller wants to check whether this use
      is flowing through an abnormal edge (only used when checking PHI
      arguments).  */

static bool
verify_use (basic_block bb, basic_block def_bb, tree ssa_name,
	    tree stmt, bool check_abnormal)
{
  bool err = false;

  if (IS_EMPTY_STMT (SSA_NAME_DEF_STMT (ssa_name)))
    ; /* Nothing to do.  */
  else if (!def_bb)
    {
      error ("Missing definition");
      err = true;
    }
  else if (bb != def_bb
	   && !dominated_by_p (CDI_DOMINATORS, bb, def_bb))
    {
      error ("Definition in block %i does not dominate use in block %i",
	     def_bb->index, bb->index);
      err = true;
    }

  if (check_abnormal
      && !SSA_NAME_OCCURS_IN_ABNORMAL_PHI (ssa_name))
    {
      error ("SSA_NAME_OCCURS_IN_ABNORMAL_PHI should be set");
      err = true;
    }

  if (err)
    {
      fprintf (stderr, "for SSA_NAME: ");
      debug_generic_stmt (ssa_name);
      fprintf (stderr, "in statement:\n");
      debug_generic_stmt (stmt);
    }

  return err;
}


/* Return true if any of the arguments for PHI node PHI at block BB is
   malformed.

   IDOM contains immediate dominator information for the flowgraph.

   DEFINITION_BLOCK is an array of basic blocks indexed by SSA_NAME version
      numbers.  If DEFINITION_BLOCK[SSA_NAME_VERSION] is set, it means that the
      block in that array slot contains the definition of SSA_NAME.  */

static bool
verify_phi_args (tree phi, basic_block bb, basic_block *definition_block)
{
  edge e;
  bool err = false;
  int i, phi_num_args = PHI_NUM_ARGS (phi);

  /* Mark all the incoming edges.  */
  for (e = bb->pred; e; e = e->pred_next)
    e->aux = (void *) 1;

  for (i = 0; i < phi_num_args; i++)
    {
      tree op = PHI_ARG_DEF (phi, i);

      e = PHI_ARG_EDGE (phi, i);

      if (TREE_CODE (op) == SSA_NAME)
	err |= verify_use (e->src, definition_block[SSA_NAME_VERSION (op)], op,
			   phi, e->flags & EDGE_ABNORMAL);

      if (e->dest != bb)
	{
	  error ("Wrong edge %d->%d for PHI argument\n",
	         e->src->index, e->dest->index, bb->index);
	  err = true;
	}

      if (e->aux == (void *) 0)
	{
	  error ("PHI argument flowing through dead edge %d->%d\n",
	         e->src->index, e->dest->index);
	  err = true;
	}

      if (e->aux == (void *) 2)
	{
	  error ("PHI argument duplicated for edge %d->%d\n", e->src->index,
	         e->dest->index);
	  err = true;
	}

      if (err)
	{
	  fprintf (stderr, "PHI argument\n");
	  debug_generic_stmt (op);
	}

      e->aux = (void *) 2;
    }

  for (e = bb->pred; e; e = e->pred_next)
    {
      if (e->aux != (void *) 2)
	{
	  error ("No argument flowing through edge %d->%d\n", e->src->index,
		 e->dest->index);
	  err = true;
	}
      e->aux = (void *) 0;
    }

  if (err)
    {
      fprintf (stderr, "for PHI node\n");
      debug_generic_stmt (phi);
    }


  return err;
}


/* Verify common invariants in the SSA web.
   TODO: verify the variable annotations.  */

void
verify_ssa (void)
{
  bool err = false;
  basic_block bb;
  basic_block *definition_block = xcalloc (highest_ssa_version,
		  			   sizeof (basic_block));

  timevar_push (TV_TREE_SSA_VERIFY);

  calculate_dominance_info (CDI_DOMINATORS);

  /* Verify and register all the SSA_NAME definitions found in the
     function.  */
  FOR_EACH_BB (bb)
    {
      tree phi;
      block_stmt_iterator bsi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	err |= verify_def (bb, definition_block, PHI_RESULT (phi), phi);

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt;
	  unsigned int j;
	  vdef_optype vdefs;
	  def_optype defs;

	  stmt = bsi_stmt (bsi);
	  get_stmt_operands (stmt);

	  vdefs = STMT_VDEF_OPS (stmt);
	  for (j = 0; j < NUM_VDEFS (vdefs); j++)
	    {
	      tree op = VDEF_RESULT (vdefs, j);
	      if (is_gimple_reg (op))
		{
		  error ("Found a virtual definition for a GIMPLE register");
		  debug_generic_stmt (op);
		  debug_generic_stmt (stmt);
		  err = true;
		}
	      err |= verify_def (bb, definition_block, op, stmt);
	    }

	  defs = STMT_DEF_OPS (stmt);
	  for (j = 0; j < NUM_DEFS (defs); j++)
	    {
	      tree op = DEF_OP (defs, j);
	      if (TREE_CODE (op) == SSA_NAME && !is_gimple_reg (op))
		{
		  error ("Found a real definition for a non-GIMPLE register");
		  debug_generic_stmt (op);
		  debug_generic_stmt (stmt);
		  err = true;
		}
	      err |= verify_def (bb, definition_block, op, stmt);
	    }
	}
    }


  /* Now verify all the uses and make sure they agree with the definitions
     found in the previous pass.  */
  FOR_EACH_BB (bb)
    {
      edge e;
      tree phi;
      block_stmt_iterator bsi;

      /* Make sure that all edges have a clear 'aux' field.  */
      for (e = bb->pred; e; e = e->pred_next)
	{
	  if (e->aux)
	    {
	      error ("AUX pointer initialized for edge %d->%d\n", e->src->index,
		      e->dest->index);
	      err = true;
	    }
	}

      /* Verify the arguments for every PHI node in the block.  */
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	err |= verify_phi_args (phi, bb, definition_block);

      /* Now verify all the uses and vuses in every statement of the block.  */
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);
	  unsigned int j;
	  vuse_optype vuses;
	  use_optype uses;

	  vuses = STMT_VUSE_OPS (stmt);
	  for (j = 0; j < NUM_VUSES (vuses); j++)
	    {
	      tree op = VUSE_OP (vuses, j);

	      if (is_gimple_reg (op))
		{
		  error ("Found a virtual use for a GIMPLE register");
		  debug_generic_stmt (op);
		  debug_generic_stmt (stmt);
		  err = true;
		}
	      err |= verify_use (bb, definition_block[SSA_NAME_VERSION (op)],
				 op, stmt, false);
	    }

	  uses = STMT_USE_OPS (stmt);
	  for (j = 0; j < NUM_USES (uses); j++)
	    {
	      tree op = USE_OP (uses, j);

	      if (TREE_CODE (op) == SSA_NAME && !is_gimple_reg (op))
		{
		  error ("Found a real use of a non-GIMPLE register");
		  debug_generic_stmt (op);
		  debug_generic_stmt (stmt);
		  err = true;
		}
	      err |= verify_use (bb, definition_block[SSA_NAME_VERSION (op)],
				 op, stmt, false);
	    }
	}
    }

  free (definition_block);

  timevar_pop (TV_TREE_SSA_VERIFY);

  if (err)
    internal_error ("verify_ssa failed.");
}


/*---------------------------------------------------------------------------
		  Helpers for the main SSA building functions
---------------------------------------------------------------------------*/

/* Insert PHI nodes for variable VAR.  */

static void
insert_phi_nodes_for (tree var, bitmap *dfs)
{
  struct def_blocks_d *def_map;
  bitmap phi_insertion_points;
  int bb_index;

  def_map = get_def_blocks_for (var);
  if (def_map == NULL)
    return;

  phi_insertion_points = BITMAP_GGC_ALLOC ();

  EXECUTE_IF_SET_IN_BITMAP (def_map->def_blocks, 0, bb_index,
    {
      VARRAY_PUSH_BB (work_stack, BASIC_BLOCK (bb_index));
    });

  /* Pop a block off the worklist, add every block that appears in
     the original block's dfs that we have not already processed to
     the worklist.  Iterate until the worklist is empty.   Blocks
     which are added to the worklist are potential sites for
     PHI nodes. 

     The iteration step could be done during PHI insertion just as
     easily.  We do it here for historical reasons -- we used to have
     a heuristic which used the potential PHI insertion points to
     determine if fully pruned or semi pruned SSA form was appropriate.

     We now always use fully pruned SSA form.  */
  while (VARRAY_ACTIVE_SIZE (work_stack) > 0)
    {
      basic_block bb = VARRAY_TOP_BB (work_stack);
      int bb_index = bb->index;
      int dfs_index;

      VARRAY_POP (work_stack);
      
      EXECUTE_IF_AND_COMPL_IN_BITMAP (dfs[bb_index],
				      phi_insertion_points,
				      0, dfs_index,
	{
	  basic_block bb = BASIC_BLOCK (dfs_index);

	  VARRAY_PUSH_BB (work_stack, bb);
	  bitmap_set_bit (phi_insertion_points, dfs_index);
	});
    }

  /* Now compute global livein for this variable.  Note this modifies
     def_map->livein_blocks.  */
  compute_global_livein (def_map->livein_blocks, def_map->def_blocks);

  /* And insert the PHI nodes.  */
  EXECUTE_IF_AND_IN_BITMAP (phi_insertion_points, def_map->livein_blocks,
			    0, bb_index,
    {
      create_phi_node (var, BASIC_BLOCK (bb_index));
    });

  phi_insertion_points = NULL;
}

/* Rewrite statement pointed by iterator SI into SSA form. 

   BLOCK_DEFS_P points to a stack with all the definitions found in the
   block.  This is used by the dominator tree walker callbacks to restore
   the current reaching definition for every variable defined in BB after
   visiting the immediate dominators of BB.  */

static void
rewrite_stmt (block_stmt_iterator si, varray_type *block_defs_p)
{
  size_t i;
  stmt_ann_t ann;
  tree stmt;
  vuse_optype vuses;
  vdef_optype vdefs;
  def_optype defs;
  use_optype uses;

  stmt = bsi_stmt (si);
  ann = stmt_ann (stmt);
  ssa_stats.num_stmts++;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "Renaming statement ");
      print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
      fprintf (tree_dump_file, "\n");
    }

#if defined ENABLE_CHECKING
  /* We have just scanned the code for operands.  No statement should
     be modified.  */
  if (ann->modified)
    abort ();
#endif

  defs = DEF_OPS (ann);
  uses = USE_OPS (ann);
  vuses = VUSE_OPS (ann);
  vdefs = VDEF_OPS (ann);

  /* Step 1.  Rewrite USES and VUSES in the statement.  */
  for (i = 0; i < NUM_USES (uses); i++)
    rewrite_operand (USE_OP_PTR (uses, i));

  /* Rewrite virtual uses in the statement.  */
  for (i = 0; i < NUM_VUSES (vuses); i++)
    rewrite_operand (VUSE_OP_PTR (vuses, i));

  /* Step 2.  Register the statement's DEF and VDEF operands.  */
  for (i = 0; i < NUM_DEFS (defs); i++)
    {
      tree *def_p = DEF_OP_PTR (defs, i);

      if (TREE_CODE (*def_p) != SSA_NAME)
	*def_p = make_ssa_name (*def_p, stmt);

      /* FIXME: We shouldn't be registering new defs if the variable
	 doesn't need to be renamed.  */
      register_new_def (SSA_NAME_VAR (*def_p), *def_p,
			block_defs_p, currdefs);
    }

  /* Register new virtual definitions made by the statement.  */
  for (i = 0; i < NUM_VDEFS (vdefs); i++)
    {
      rewrite_operand (VDEF_OP_PTR (vdefs, i));

      if (TREE_CODE (VDEF_RESULT (vdefs, i)) != SSA_NAME)
	*VDEF_RESULT_PTR (vdefs, i) = make_ssa_name (VDEF_RESULT (vdefs, i), stmt);

      /* FIXME: We shouldn't be registering new defs if the variable
	 doesn't need to be renamed.  */
      register_new_def (SSA_NAME_VAR (VDEF_RESULT (vdefs, i)), 
			VDEF_RESULT (vdefs, i), block_defs_p, currdefs);
    }
}


/* Set the USED bit in the annotation for T.  */

void
set_is_used (tree t)
{
  t = get_base_decl (t);
  var_ann (t)->used = 1;
}


/* Replace the operand pointed by OP_P with its immediate reaching
   definition.  */

static inline void
rewrite_operand (tree *op_p)
{
  if (TREE_CODE (*op_p) != SSA_NAME)
    *op_p = get_reaching_def (*op_p);
}


/* Register DEF to be a new definition for variable VAR and push VAR's
   current reaching definition into the stack pointed by BLOCK_DEFS_P.
   IS_REAL_OPERAND is true when DEF is a real definition.  */

void
register_new_def (tree var, tree def,
		  varray_type *block_defs_p, varray_type table)
{
  tree currdef = get_value_for (var, table);

  if (! *block_defs_p)
    VARRAY_TREE_INIT (*block_defs_p, 20, "block_defs");

  /* If the current reaching definition is NULL, push the variable itself
     so that the dominator tree callbacks know what variable is associated
     with this NULL reaching def when unwinding the *BLOCK_DEFS_P stack.  */
  if (currdef == NULL_TREE)
    VARRAY_PUSH_TREE (*block_defs_p, var);

  /* Push the current reaching definition into *BLOCK_DEFS_P.  This stack is
     later used by the dominator tree callbacks to restore the reaching
     definitions for all the variables defined in the block after a recursive
     visit to all its immediately dominated blocks.  */
  VARRAY_PUSH_TREE (*block_defs_p, currdef);

  /* Set the current reaching definition for VAR to be DEF.  */
  set_value_for (var, def, table);
}


/* Initialize global DFA and SSA structures.  */

void
init_tree_ssa (void)
{
  VARRAY_TREE_INIT (referenced_vars, 20, "referenced_vars");
  call_clobbered_vars = BITMAP_XMALLOC ();
  bitmap_clear (call_clobbered_vars);
  init_ssa_operands ();
  init_ssanames ();
  init_phinodes ();
  memset (&ssa_stats, 0, sizeof (ssa_stats));
  global_var = NULL_TREE;
  aliases_computed_p = false;
}


/* Deallocate memory associated with SSA data structures for FNDECL.  */

void
delete_tree_ssa (void)
{
  size_t i;
  basic_block bb;
  block_stmt_iterator bsi;

  /* Remove annotations from every tree in the function.  */
  FOR_EACH_BB (bb)
    for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
      bsi_stmt (bsi)->common.ann = NULL;

  /* Remove annotations from every referenced variable.  */
  if (referenced_vars)
    {
      for (i = 0; i < num_referenced_vars; i++)
	referenced_var (i)->common.ann = NULL;
      referenced_vars = NULL;
    }

  fini_ssanames ();
  fini_phinodes ();
  fini_ssa_operands ();

  global_var = NULL_TREE;
  BITMAP_FREE (call_clobbered_vars);
  call_clobbered_vars = NULL;
  aliases_computed_p = false;
}

/* Return the current definition for variable VAR.  If none is found,
   create a new SSA name to act as the zeroth definition for VAR.  If VAR
   is call clobbered and there exists a more recent definition of
   GLOBAL_VAR, return the definition for GLOBAL_VAR.  This means that VAR
   has been clobbered by a function call since its last assignment.  */

static tree
get_reaching_def (tree var)
{
  tree default_d, currdef_var;
  
  /* Lookup the current reaching definition for VAR.  */
  default_d = NULL_TREE;
  currdef_var = get_value_for (var, currdefs);

  /* If there is no reaching definition for VAR, create and register a
     default definition for it (if needed).  */
  if (currdef_var == NULL_TREE)
    {
      default_d = default_def (var);
      if (default_d == NULL_TREE)
	{
	  default_d = make_ssa_name (var, build_empty_stmt ());
	  set_default_def (var, default_d);
	}
      set_value_for (var, default_d, currdefs);
    }

  /* Return the current reaching definition for VAR, or the default
     definition, if we had to create one.  */
  return (currdef_var) ? currdef_var : default_d;
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


/* Return the set of blocks where variable VAR is defined and the blocks
   where VAR is live on entry (livein).  */

static struct def_blocks_d *
get_def_blocks_for (tree var)
{
  struct def_blocks_d dm;

  dm.var = var;
  return (struct def_blocks_d *) htab_find (def_blocks, &dm);
}
/* Return true if EXPR is a useless type conversion, otherwise return
   false.  */

bool
tree_ssa_useless_type_conversion_1 (tree outer_type, tree inner_type)
{
  /* If the inner and outer types are effectively the same, then
     strip the type conversion and enter the equivalence into
     the table.  */
  if (inner_type == outer_type
      || TYPE_MAIN_VARIANT (inner_type) == TYPE_MAIN_VARIANT (outer_type))
    return true;

  /* If the outer type is a (void *), then we can enter the
     equivalence into the table.  The opposite is not true since
     that conversion would result in a loss of information if
     the equivalence was used.  Consider an indirect function call
     where we need to know the exact type of the function to
     correctly implement the ABI.  */
  else if (POINTER_TYPE_P (inner_type) && POINTER_TYPE_P (outer_type)
	   && TREE_CODE (TREE_TYPE (outer_type)) == VOID_TYPE)
    return true;

  /* Pointers and references are equivalent once we get to GENERIC,
     so strip conversions that just switch between them.  */
  else if (POINTER_TYPE_P (inner_type) && POINTER_TYPE_P (outer_type)
	   && (TYPE_MAIN_VARIANT (TREE_TYPE (inner_type))
	       == TYPE_MAIN_VARIANT (TREE_TYPE (outer_type))))
    return true;

  /* If both the inner and outer types are integral types, then
     we can enter the equivalence if they have the same mode
     and signedness and precision (The type _Bool can have size of 4
     (only happens on powerpc-darwin right now but can happen on any 
     target that defines BOOL_TYPE_SIZE to be INT_TYPE_SIZE) and a
     precision of 1 while unsigned int is the same expect for a 
     precision of 4 so testing of precision is necessary).  */
  else if (INTEGRAL_TYPE_P (inner_type) && INTEGRAL_TYPE_P (outer_type)
	   && TYPE_MODE (inner_type) == TYPE_MODE (outer_type)
	   && TREE_UNSIGNED (inner_type) == TREE_UNSIGNED (outer_type)
	   && TYPE_PRECISION (inner_type) == TYPE_PRECISION (outer_type))
    return true;
  /* Recurse for complex types.  */
  else if (TREE_CODE (inner_type) == COMPLEX_TYPE
	   && TREE_CODE (outer_type) == COMPLEX_TYPE
	   && tree_ssa_useless_type_conversion_1 (TREE_TYPE (outer_type),
						  TREE_TYPE (inner_type)))
    return true;
  return false;
}

/* Return true if EXPR is a useless type conversion, otherwise return
   false.  */

bool
tree_ssa_useless_type_conversion (tree expr)
{
  /* If we have an assignment that merely uses a NOP_EXPR to change
     the top of the RHS to the type of the LHS and the type conversion
     is "safe", then strip away the type conversion so that we can
     enter LHS = RHS into the const_and_copies table.  */
  if (TREE_CODE (expr) == NOP_EXPR || TREE_CODE (expr) == CONVERT_EXPR)
    return tree_ssa_useless_type_conversion_1 (TREE_TYPE (expr),
					       TREE_TYPE (TREE_OPERAND (expr,
									0)));


  return false;
}


/* Internal helper for walk_use_def_chains.  VAR, FN and DATA are as
   described in walk_use_def_chains.  VISITED is a bitmap used to mark
   visited SSA_NAMEs to avoid infinite loops.  */

static void
walk_use_def_chains_1 (tree var, walk_use_def_chains_fn fn, void *data,
		       bitmap visited)
{
  tree def_stmt;

  if (bitmap_bit_p (visited, SSA_NAME_VERSION (var)))
    return;

  bitmap_set_bit (visited, SSA_NAME_VERSION (var));

  def_stmt = SSA_NAME_DEF_STMT (var);

  if (TREE_CODE (def_stmt) != PHI_NODE)
    {
      /* If we reached the end of the use-def chain, call FN.  */
      (*fn) (var, def_stmt, data);
    }
  else
    {
      int i;

      /* Otherwise, follow use-def links out of each PHI argument and call
	 FN after visiting each one.  */
      for (i = 0; i < PHI_NUM_ARGS (def_stmt); i++)
	{
	  tree arg = PHI_ARG_DEF (def_stmt, i);
	  if (TREE_CODE (arg) == SSA_NAME)
	    walk_use_def_chains_1 (arg, fn, data, visited);
	  (*fn) (arg, def_stmt, data);
	}
    }
}
  


/* Walk use-def chains starting at the SSA variable VAR.  Call function FN
   at each reaching definition found.  FN takes three arguments: VAR, its
   defining statement (DEF_STMT) and a generic pointer to whatever state
   information that FN may want to maintain (DATA).

   Note, that if DEF_STMT is a PHI node, the semantics are slightly
   different.  For each argument ARG of the PHI node, this function will:

	1- Walk the use-def chains for ARG.
	2- Call (*FN) (ARG, PHI, DATA).

   Note how the first argument to FN is no longer the original variable
   VAR, but the PHI argument currently being examined.  If FN wants to get
   at VAR, it should call PHI_RESULT (PHI).  */

void
walk_use_def_chains (tree var, walk_use_def_chains_fn fn, void *data)
{
  tree def_stmt;

#if defined ENABLE_CHECKING
  if (TREE_CODE (var) != SSA_NAME)
    abort ();
#endif

  def_stmt = SSA_NAME_DEF_STMT (var);

  /* We only need to recurse if the reaching definition comes from a PHI
     node.  */
  if (TREE_CODE (def_stmt) != PHI_NODE)
    (*fn) (var, def_stmt, data);
  else
    {
      bitmap visited = BITMAP_XMALLOC ();
      walk_use_def_chains_1 (var, fn, data, visited);
      BITMAP_XFREE (visited);
    }
}

/* Replaces immediate uses of VAR by REPL.  */

static void
replace_immediate_uses (tree var, tree repl)
{
  use_optype uses;
  vuse_optype vuses;
  vdef_optype vdefs;
  int i, j, n;
  dataflow_t df;
  tree stmt;

  df = get_immediate_uses (SSA_NAME_DEF_STMT (var));
  n = num_immediate_uses (df);

  for (i = 0; i < n; i++)
    {
      stmt = immediate_use (df, i);

      if (TREE_CODE (stmt) == PHI_NODE)
	{
	  for (j = 0; j < PHI_NUM_ARGS (stmt); j++)
	    if (PHI_ARG_DEF (stmt, j) == var)
	      {
		PHI_ARG_DEF (stmt, j) = repl;
		if (TREE_CODE (repl) == SSA_NAME
		    && PHI_ARG_EDGE (stmt, j)->flags & EDGE_ABNORMAL)
		  SSA_NAME_OCCURS_IN_ABNORMAL_PHI (repl) = 1;
	      }

	  continue;
	}

      get_stmt_operands (stmt);
      if (is_gimple_reg (SSA_NAME_VAR (var)))
	{
	  uses = STMT_USE_OPS (stmt);
	  for (j = 0; j < (int) NUM_USES (uses); j++)
	    if (USE_OP (uses, j) == var)
	      *USE_OP_PTR (uses, j) = repl;
	}
      else
	{
	  vuses = STMT_VUSE_OPS (stmt);
	  for (j = 0; j < (int) NUM_VUSES (vuses); j++)
	    if (VUSE_OP (vuses, j) == var)
	      *VUSE_OP_PTR (vuses, j) = repl;

	  vdefs = STMT_VDEF_OPS (stmt);
	  for (j = 0; j < (int) NUM_VDEFS (vdefs); j++)
	    if (VDEF_OP (vdefs, j) == var)
	      *VDEF_OP_PTR (vdefs, j) = repl;
	}
      modify_stmt (stmt);
    }
}

/* Raises value of phi node PHI by joining it with VAL.  Processes immediate
   uses of the phi recursively.  */

static void
raise_value (tree phi, tree val, tree *eq_to)
{
  int i, n;
  tree var = PHI_RESULT (phi), stmt;
  int ver = SSA_NAME_VERSION (var);
  dataflow_t df;

  if (eq_to[ver] == var)
    return;

  switch (TREE_CODE (val))
    {
    case SSA_NAME:
    case REAL_CST:
    case COMPLEX_CST:
      break;
    case INTEGER_CST:
      if (TREE_CODE (TREE_TYPE (var)) != POINTER_TYPE)
	break;

    default:
      /* Do not propagate pointer constants.  This might require folding
	 things like *&foo and rewriting the ssa, which is not worth the
	 trouble.  */
      val = var;
    }

  if (eq_to[ver])
    {
      if (operand_equal_p (eq_to[ver], val, 0))
	return;

      eq_to[ver] = var;
    }
  else
    eq_to[ver] = val;

  df = get_immediate_uses (SSA_NAME_DEF_STMT (var));
  n = num_immediate_uses (df);

  for (i = 0; i < n; i++)
    {
      stmt = immediate_use (df, i);

      if (TREE_CODE (stmt) != PHI_NODE)
	continue;

      raise_value (stmt, eq_to[ver], eq_to);
    }
}

/* Removes redundant phi nodes.

   A redundant PHI node is a PHI node where all of its PHI arguments
   are the same value, excluding any PHI arguments which are the same
   as the PHI result.

   A redundant PHI node is effectively a copy, so we forward copy propagate
   which removes all uses of the destination of the PHI node then
   finally we delete the redundant PHI node.

   Note that if we can not copy propagate the PHI node, then the PHI
   will not be removed.  Thus we do not have to worry about dependencies
   between PHIs and the problems serializing PHIs into copies creates. 
   
   The most important effect of this pass is to remove degenerate PHI
   nodes created by removing unreachable code.  */

void
kill_redundant_phi_nodes (void)
{
  tree *eq_to, *ssa_names;
  unsigned i, ver, aver;
  basic_block bb;
  tree phi, t, stmt, var;

  /* The EQ_TO array holds the current value of the ssa name in the
     lattice:

          top
         / | \
     const   variables
         \ | /
        bottom

     Bottom is represented by NULL and top by the variable itself.

     Once the dataflow stabilizes, we know that the phi nodes we need to keep
     are exactly those with top as their result. 

     The remaining phi nodes have their uses replaced with their value
     in the lattice and the phi node itself is removed.  */
  eq_to = xcalloc (highest_ssa_version, sizeof (tree));

  /* The SSA_NAMES array holds each SSA_NAME node we encounter
     in a PHI node (indexed by ssa version number).

     One could argue that the SSA_NAME manager ought to provide a
     generic interface to get at the SSA_NAME node for a given
     ssa version number.  */
  ssa_names = xcalloc (highest_ssa_version, sizeof (tree));

  /* We have had cases where computing immediate uses takes a
     significant amount of compile time.  If we run into such
     problems here, we may want to only compute immediate uses for
     a subset of all the SSA_NAMEs instead of computing it for
     all of the SSA_NAMEs.  */
  compute_immediate_uses (TDFA_USE_OPS | TDFA_USE_VOPS, NULL);

  FOR_EACH_BB (bb)
    {
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  var = PHI_RESULT (phi);
	  ver = SSA_NAME_VERSION (var);
	  ssa_names[ver] = var;

	  for (i = 0; i < (unsigned) PHI_NUM_ARGS (phi); i++)
	    {
	      t = PHI_ARG_DEF (phi, i);

	      if (TREE_CODE (t) != SSA_NAME)
		{
		  raise_value (phi, t, eq_to);
		  continue;
		}

	      stmt = SSA_NAME_DEF_STMT (t);
	      aver = SSA_NAME_VERSION (t);
	      ssa_names[aver] = t;

	      /* If the defining statement for this argument is not a
		 phi node or the argument is associated with an abnormal
		 edge, then we need to recursively start the forward
		 dataflow starting with PHI.  */
	      if (TREE_CODE (stmt) != PHI_NODE
		  || SSA_NAME_OCCURS_IN_ABNORMAL_PHI (t))
		{
		  eq_to[aver] = t;
		  raise_value (phi, t, eq_to);
		}
	    }
	}
    }

  /* Now propagate the values.  */
  for (i = 0; i < highest_ssa_version; i++)
    if (eq_to[i]
	&& eq_to[i] != ssa_names[i])
      replace_immediate_uses (ssa_names[i], eq_to[i]);

  /* And remove the dead phis.  */
  for (i = 0; i < highest_ssa_version; i++)
    if (eq_to[i]
	&& eq_to[i] != ssa_names[i])
      {
	stmt = SSA_NAME_DEF_STMT (ssa_names[i]);
	remove_phi_node (stmt, 0, bb_for_stmt (stmt));
      }

  free_df ();
  free (eq_to);
  free (ssa_names);
}

/* Emit warnings for uninitialized variables.  This is done in two passes.

   The first pass notices real uses of SSA names with default definitions.
   Such uses are unconditionally uninitialized, and we can be certain that
   such a use is a mistake.  This pass is run before most optimizations,
   so that we catch as many as we can.

   The second pass follows PHI nodes to find uses that are potentially
   uninitialized.  In this case we can't necessarily prove that the use
   is really uninitialized.  This pass is run after most optimizations,
   so that we thread as many jumps and possible, and delete as much dead
   code as possible, in order to reduce false positives.  We also look
   again for plain uninitialized variables, since optimization may have
   changed conditionally uninitialized to unconditionally uninitialized.  */

/* Emit a warning for T, an SSA_NAME, being uninitialized.  The exact
   warning text is in MSGID and LOCUS may contain a location or be null.  */

static void
warn_uninit (tree t, const char *msgid, location_t *locus)
{
  tree var = SSA_NAME_VAR (t);
  tree def = SSA_NAME_DEF_STMT (t);

  /* Default uses (indicated by an empty definition statement), are
     uninitialized.  Except for PARMs of course, which are always
     initialized.  TREE_NO_WARNING either means we already warned,
     or the front end wishes to suppress the warning.  */
  if (IS_EMPTY_STMT (def)
      && TREE_CODE (var) != PARM_DECL
      && !TREE_NO_WARNING (var))
    {
      if (!locus)
	locus = &DECL_SOURCE_LOCATION (var);
      warning (msgid, locus, var);
      TREE_NO_WARNING (var) = 1;
    }
}
   
/* Called via walk_tree, look for SSA_NAMEs that have empty definitions
   and warn about them.  */

static tree
warn_uninitialized_var (tree *tp, int *walk_subtrees, void *data)
{
  location_t *locus = data;
  tree t = *tp;

  /* We only do data flow with SSA_NAMEs, so that's all we can warn about.  */
  if (TREE_CODE (t) == SSA_NAME)
    {
      warn_uninit (t, "%H'%D' is used uninitialized in this function", locus);
      *walk_subtrees = 0;
    }
  else if (DECL_P (t) || TYPE_P (t))
    *walk_subtrees = 0;

  return NULL_TREE;
}

/* Look for inputs to PHI that are SSA_NAMEs that have empty definitions
   and warn about them.  */

static void
warn_uninitialized_phi (tree phi)
{
  int i, n = PHI_NUM_ARGS (phi);

  /* Don't look at memory tags.  */
  if (!is_gimple_reg (PHI_RESULT (phi)))
    return;

  for (i = 0; i < n; ++i)
    {
      tree op = PHI_ARG_DEF (phi, i);
      if (TREE_CODE (op) == SSA_NAME)
	warn_uninit (op, "%H'%D' may be used uninitialized in this function",
		     NULL);
    }
}

static void
execute_early_warn_uninitialized (void)
{
  block_stmt_iterator bsi;
  basic_block bb;

  FOR_EACH_BB (bb)
    for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
      walk_tree (bsi_stmt_ptr (bsi), warn_uninitialized_var,
		 EXPR_LOCUS (bsi_stmt (bsi)), NULL);
}

static void
execute_late_warn_uninitialized (void)
{
  basic_block bb;
  tree phi;

  /* Re-do the plain uninitialized variable check, as optimization may have
     straightened control flow.  Do this first so that we don't accidentally
     get a "may be" warning when we'd have seen an "is" warning later.  */
  execute_early_warn_uninitialized ();

  FOR_EACH_BB (bb)
    for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
      warn_uninitialized_phi (phi);
}

static bool
gate_warn_uninitialized (void)
{
  return warn_uninitialized != 0;
}

struct tree_opt_pass pass_early_warn_uninitialized =
{
  NULL,					/* name */
  gate_warn_uninitialized,		/* gate */
  execute_early_warn_uninitialized,	/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_ssa,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0					/* todo_flags_finish */
};

struct tree_opt_pass pass_late_warn_uninitialized =
{
  NULL,					/* name */
  gate_warn_uninitialized,		/* gate */
  execute_late_warn_uninitialized,	/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_ssa,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0					/* todo_flags_finish */
};

#include "gt-tree-ssa.h"
