/* Rewrite a program in Normal form into SSA.
   Copyright (C) 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.
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
#include "tree-gimple.h"
#include "tree-inline.h"
#include "varray.h"
#include "timevar.h"
#include "hashtab.h"
#include "tree-dump.h"
#include "tree-pass.h"
#include "cfgloop.h"
#include "domwalk.h"
#include "ggc.h"
#include "pointer-set.h"

/* This file builds the SSA form for a function as described in:
   R. Cytron, J. Ferrante, B. Rosen, M. Wegman, and K. Zadeck. Efficiently
   Computing Static Single Assignment Form and the Control Dependence
   Graph. ACM Transactions on Programming Languages and Systems,
   13(4):451-490, October 1991.  */

/* Structure to map a variable VAR to the set of blocks that contain
   definitions for VAR.  */
struct def_blocks_d
{
  /* The variable.  */
  tree var;

  /* Blocks that contain definitions of VAR.  Bit I will be set if the
     Ith block contains a definition of VAR.  */
  bitmap def_blocks;

  /* Blocks that contain a PHI node for VAR.  */
  bitmap phi_blocks;

  /* Blocks where VAR is live-on-entry.  Similar semantics as
     DEF_BLOCKS.  */
  bitmap livein_blocks;
};


/* Each entry in DEF_BLOCKS contains an element of type STRUCT
   DEF_BLOCKS_D, mapping a variable VAR to a bitmap describing all the
   basic blocks where VAR is defined (assigned a new value).  It also
   contains a bitmap of all the blocks where VAR is live-on-entry
   (i.e., there is a use of VAR in block B without a preceding
   definition in B).  The live-on-entry information is used when
   computing PHI pruning heuristics.  */
static htab_t def_blocks;

/* Stack of trees used to restore the global currdefs to its original
   state after completing rewriting of a block and its dominator children.

   This vector is used in two contexts.  The first is rewriting of _DECL
   nodes into SSA_NAMEs.  In that context its elements have the
   following properties:

     An SSA_NAME indicates that the current definition of the underlying
     variable should be set to the given SSA_NAME.
                                                                                
     A _DECL node indicates that the underlying variable has no current
     definition.
                                                                                
     A NULL node is used to mark the last node associated with the
     current block. 

   This vector is also used when rewriting an SSA_NAME which has multiple
   definition sites into multiple SSA_NAMEs.  In that context entries come
   in pairs.

     The top entry is an SSA_NAME and the top-1 entry is the
     current value for that SSA_NAME. 

     A NULL node at the top entry is used to mark the last node associated
     with the current block.  */
static VEC(tree_on_heap) *block_defs_stack;

/* Basic block vectors used in this file ought to be allocated in the heap.  */
DEF_VEC_MALLOC_P(basic_block);

/* Set of existing SSA names being replaced by update_ssa.  */
static struct pointer_set_t *old_ssa_names;

/* Set of new SSA names being added by update_ssa.  */
static struct pointer_set_t *new_ssa_names;

/* Tuple used to map between old and new names.  */
struct new_to_old_d
{
  tree new;
  tree old;
};
   
/* Replacement table.  If we are replacing an existing SSA name O_j
   with a new name N_i, then NEW_TO_OLD[N_i] = O_j.  */
static htab_t new_to_old;

/* Global data to attach to the main dominator walk structure.  */
struct mark_def_sites_global_data
{
  /* This sbitmap contains the variables which are set before they
     are used in a basic block.  We keep it as a global variable
     solely to avoid the overhead of allocating and deallocating
     the bitmap.  */
  bitmap kills;

  /* Bitmap of names to rename.  */
  sbitmap names_to_rename;
};


/* Information stored for ssa names.  */
struct ssa_name_info
{
  /* This field indicates whether or not the variable may need PHI nodes.
     See the enum's definition for more detailed information about the
     states.  */
  ENUM_BITFIELD (need_phi_state) need_phi_state : 2;

  /* The actual definition of the ssa name.  */
  tree current_def;
};


/* The main entry point to the SSA renamer (rewrite_blocks) may be
   called several times to do different, but related, tasks.
   Initially, we need it to rename the whole program into SSA form.
   At other times, we may need it to only rename into SSA newly
   exposed symbols.  Finally, we can also call it to incrementally fix
   an already built SSA web.  */
enum rewrite_mode {
    /* Convert all variables in VARS_TO_RENAME into SSA form.  */
    REWRITE_ALL,

    /* Used after DCE and CD-DCE.  Both passes may leave DEF-DEF
       chains for PHI nodes out of date.  In this mode, the renamer
       just updates every virtual PHI argument to the appropriate
       reaching definition (see rewrite_def_def_chains).  */
    REWRITE_FIX_DEF_DEF_CHAINS,
	
    /* Incrementally update the SSA web by replacing existing SSA
       names with new ones.  See update_ssa for details.  */
    REWRITE_UPDATE
};


/* Use TREE_VISITED to keep track of which statements we want to
   rename.  When renaming a subset of the variables, not all
   statements will be processed.  This is decided in mark_def_sites.  */
#define REWRITE_THIS_STMT(T)	TREE_VISITED (T)


/* Get the information associated with NAME.  */

static inline struct ssa_name_info *
get_ssa_name_ann (tree name)
{
  if (!SSA_NAME_AUX (name))
    SSA_NAME_AUX (name) = xcalloc (1, sizeof (struct ssa_name_info));

  return SSA_NAME_AUX (name);
}


/* Gets phi_state field for VAR.  */

static inline enum need_phi_state
get_phi_state (tree var)
{
  if (TREE_CODE (var) == SSA_NAME)
    return get_ssa_name_ann (var)->need_phi_state;
  else
    return var_ann (var)->need_phi_state;
}


/* Sets phi_state field for VAR to STATE.  */

static inline void
set_phi_state (tree var, enum need_phi_state state)
{
  if (TREE_CODE (var) == SSA_NAME)
    get_ssa_name_ann (var)->need_phi_state = state;
  else
    var_ann (var)->need_phi_state = state;
}


/* Return the current definition for VAR.  */

static inline tree
get_current_def (tree var)
{
  if (TREE_CODE (var) == SSA_NAME)
    return get_ssa_name_ann (var)->current_def;
  else
    return var_ann (var)->current_def;
}


/* Sets current definition of VAR to DEF.  */

static inline void
set_current_def (tree var, tree def)
{
  if (TREE_CODE (var) == SSA_NAME)
    get_ssa_name_ann (var)->current_def = def;
  else
    var_ann (var)->current_def = def;
}


/* Compute global livein information given the set of blockx where
   an object is locally live at the start of the block (LIVEIN)
   and the set of blocks where the object is defined (DEF_BLOCKS).

   Note: This routine augments the existing local livein information
   to include global livein (i.e., it modifies the underlying bitmap
   for LIVEIN).  */

void
compute_global_livein (bitmap livein, bitmap def_blocks)
{
  basic_block bb, *worklist, *tos;
  unsigned i;
  bitmap_iterator bi;

  tos = worklist
    = (basic_block *) xmalloc (sizeof (basic_block) * (n_basic_blocks + 1));

  EXECUTE_IF_SET_IN_BITMAP (livein, 0, i, bi)
    {
      *tos++ = BASIC_BLOCK (i);
    }

  /* Iterate until the worklist is empty.  */
  while (tos != worklist)
    {
      edge e;
      edge_iterator ei;

      /* Pull a block off the worklist.  */
      bb = *--tos;

      /* For each predecessor block.  */
      FOR_EACH_EDGE (e, ei, bb->preds)
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


/* Return the set of blocks where variable VAR is defined and the blocks
   where VAR is live on entry (livein).  If no entry is found in
   DEF_BLOCKS, a new one is created and returned.  */

static inline struct def_blocks_d *
get_def_blocks_for (tree var)
{
  struct def_blocks_d db, *db_p;
  void **slot;

  db.var = var;
  slot = htab_find_slot (def_blocks, (void *) &db, INSERT);
  if (*slot == NULL)
    {
      db_p = xmalloc (sizeof (*db_p));
      db_p->var = var;
      db_p->def_blocks = BITMAP_XMALLOC ();
      db_p->phi_blocks = BITMAP_XMALLOC ();
      db_p->livein_blocks = BITMAP_XMALLOC ();
      *slot = (void *) db_p;
    }
  else
    db_p = (struct def_blocks_d *) *slot;

  return db_p;
}


/* Mark block BB as the definition site for variable VAR.  PHI_P is true if
   VAR is defined by a PHI node.  IS_UPDATE is true if the caller is
   updating an existing SSA form.  */

static void
set_def_block (tree var, basic_block bb, bool phi_p, bool is_update)
{
  struct def_blocks_d *db_p;
  enum need_phi_state state;

  if (!is_update && TREE_CODE (var) == SSA_NAME)
    var = SSA_NAME_VAR (var);

  state = get_phi_state (var);
  db_p = get_def_blocks_for (var);

  /* Set the bit corresponding to the block where VAR is defined.  */
  bitmap_set_bit (db_p->def_blocks, bb->index);
  if (phi_p)
    bitmap_set_bit (db_p->phi_blocks, bb->index);

  /* Keep track of whether or not we may need to insert PHI nodes.

     If we are in the UNKNOWN state, then this is the first definition
     of VAR.  Additionally, we have not seen any uses of VAR yet, so
     we do not need a PHI node for this variable at this time (i.e.,
     transition to NEED_PHI_STATE_NO).

     If we are in any other state, then we either have multiple definitions
     of this variable occurring in different blocks or we saw a use of the
     variable which was not dominated by the block containing the
     definition(s).  In this case we may need a PHI node, so enter
     state NEED_PHI_STATE_MAYBE.  */
  if (state == NEED_PHI_STATE_UNKNOWN)
    set_phi_state (var, NEED_PHI_STATE_NO);
  else
    set_phi_state (var, NEED_PHI_STATE_MAYBE);
}


/* Mark block BB as having VAR live at the entry to BB.  */

static void
set_livein_block (tree var, basic_block bb)
{
  struct def_blocks_d *db_p;
  enum need_phi_state state = get_phi_state (var);

  db_p = get_def_blocks_for (var);

  /* Set the bit corresponding to the block where VAR is live in.  */
  bitmap_set_bit (db_p->livein_blocks, bb->index);

  /* Keep track of whether or not we may need to insert PHI nodes.

     If we reach here in NEED_PHI_STATE_NO, see if this use is dominated
     by the single block containing the definition(s) of this variable.  If
     it is, then we remain in NEED_PHI_STATE_NO, otherwise we transition to
     NEED_PHI_STATE_MAYBE.  */
  if (state == NEED_PHI_STATE_NO)
    {
      int def_block_index = bitmap_first_set_bit (db_p->def_blocks);

      if (def_block_index == -1
	  || ! dominated_by_p (CDI_DOMINATORS, bb,
	                       BASIC_BLOCK (def_block_index)))
	set_phi_state (var, NEED_PHI_STATE_MAYBE);
    }
  else
    set_phi_state (var, NEED_PHI_STATE_MAYBE);
}


/* Hashing and equality functions for NEW_TO_OLD.  */

static hashval_t
new_to_old_hash (const void *p)
{
  return htab_hash_pointer
	((const void *)((const struct new_to_old_d *)p)->new);
}

static int
new_to_old_eq (const void *p1, const void *p2)
{
  return ((const struct new_to_old_d *)p1)->new
	 == ((const struct new_to_old_d *)p2)->new;
}


/* Add a new mapping between names OLD and NEW in mapping table
   NAME_MAP.  Every entry in NAME_MAP represents the replacement
   OLD -> NEW.  This is used by update_ssa and its helpers
   to introduce new SSA names in an already formed SSA web.  */

static void
add_new_name_mapping (tree new, tree old, htab_t name_map)
{
  struct new_to_old_d m, *mp;
  void **slot;

  m.new = new;
  slot = htab_find_slot (name_map, (void *) &m, INSERT);
  if (*slot == NULL)
    {
      mp = xmalloc (sizeof (*mp));
      mp->new = new;
      mp->old = old;
      *slot = (void *) mp;
      pointer_set_insert (new_ssa_names, new);
    }
  else
    {
      /* There should always be a 1-to-1 correspondence between NEW
	 and OLD.  Given a new name N1 and two old names O1 and O2, we
	 cannot have O1->N1 and O2->N1.  */
      mp = (struct new_to_old_d *) *slot;
      gcc_assert (mp->old == old);
    }
}


/* Given a new name N, return the old name O that is being replaced
   with N.  NAME_MAP is the table where to look N up.  */

static tree
name_replaced_by (tree n, htab_t name_map)
{
  struct new_to_old_d m, *mp;
  void **slot;

  m.new = n;
  slot = htab_find_slot (name_map, (void *) &m, NO_INSERT);
  gcc_assert (slot && *slot);
  mp = (struct new_to_old_d *) *slot;
  return mp->old;
}


/* If the use operand pointed to by OP_P needs to be renamed, then strip away 
   any SSA_NAME wrapping the operand, set *UID_P to the underlying variable's 
   uid, and return true.  Otherwise return false.  If the operand was an 
   SSA_NAME, change it to the stripped name.  */

static bool
prepare_use_operand_for_rename (use_operand_p op_p, size_t *uid_p)
{
  tree use = USE_FROM_PTR (op_p);
  tree var = (TREE_CODE (use) != SSA_NAME) ? use : SSA_NAME_VAR (use);
  *uid_p = var_ann (var)->uid;

  /* Ignore variables that don't need to be renamed.  */
  if (vars_to_rename && !bitmap_bit_p (vars_to_rename, *uid_p))
    return false;

  /* The variable needs to be renamed.  If this is a use which already
     has an SSA_NAME, then strip it off.

     By not throwing away SSA_NAMEs on assignments, we avoid a lot of 
     useless churn of SSA_NAMEs without having to overly complicate the
     renamer.  */
  if (TREE_CODE (use) == SSA_NAME)
    SET_USE (op_p, var);

  return true;
}


/* If the def variable DEF needs to be renamed, then strip away any SSA_NAME 
   wrapping the operand, set *UID_P to the underlying variable's uid and return
   true.  Otherwise return false.  */

static bool
prepare_def_operand_for_rename (tree def, size_t *uid_p)
{
  tree var = (TREE_CODE (def) != SSA_NAME) ? def : SSA_NAME_VAR (def);
  *uid_p = var_ann (var)->uid;

  /* Ignore variables that don't need to be renamed.  */
  if (vars_to_rename && !bitmap_bit_p (vars_to_rename, *uid_p))
    return false;

  return true;
}


/* Call back for walk_dominator_tree used to collect definition sites
   for every variable in the function.  For every statement S in block
   BB:

   1- Variables defined by S in DEF_OPS(S) are marked in the bitmap
      WALK_DATA->GLOBAL_DATA->KILLS.

   2- If S uses a variable VAR and there is no preceding kill of VAR,
      then it is marked in marked in the LIVEIN_BLOCKS bitmap
      associated with VAR.

   This information is used to determine which variables are live
   across block boundaries to reduce the number of PHI nodes
   we create.  */

static void
mark_def_sites (struct dom_walk_data *walk_data,
		basic_block bb,
		block_stmt_iterator bsi)
{
  struct mark_def_sites_global_data *gd = walk_data->global_data;
  bitmap kills = gd->kills;
  size_t uid;
  tree stmt, def;
  use_operand_p use_p;
  def_operand_p def_p;
  ssa_op_iter iter;

  /* Mark all the blocks that have definitions for each variable in the
     VARS_TO_RENAME bitmap.  */
  stmt = bsi_stmt (bsi);
  get_stmt_operands (stmt);

  REWRITE_THIS_STMT (stmt) = 0;

  /* If a variable is used before being set, then the variable is live
     across a block boundary, so mark it live-on-entry to BB.  */

  FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter,
			    SSA_OP_USE | SSA_OP_VUSE | SSA_OP_VMUSTDEFKILL)
    {
      if (prepare_use_operand_for_rename (use_p, &uid))
	{
	  REWRITE_THIS_STMT (stmt) = 1;
	  if (!bitmap_bit_p (kills, uid))
	    set_livein_block (USE_FROM_PTR (use_p), bb);
	}
    }
  
  /* Note that virtual definitions are irrelevant for computing KILLS
     because a V_MAY_DEF does not constitute a killing definition of the
     variable.  However, the operand of a virtual definitions is a use
     of the variable, so it may cause the variable to be considered
     live-on-entry.  */
  FOR_EACH_SSA_MAYDEF_OPERAND (def_p, use_p, stmt, iter)
    {
      if (prepare_use_operand_for_rename (use_p, &uid))
	{
	  /* If we do not already have an SSA_NAME for our destination,
	     then set the destination to the source.  */
	  if (TREE_CODE (DEF_FROM_PTR (def_p)) != SSA_NAME)
	    SET_DEF (def_p, USE_FROM_PTR (use_p));
	    
          set_livein_block (USE_FROM_PTR (use_p), bb);
	  set_def_block (DEF_FROM_PTR (def_p), bb, false, false);
	  REWRITE_THIS_STMT (stmt) = 1;
	}
    }

  /* Now process the defs and must-defs made by this statement.  */
  FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_DEF | SSA_OP_VMUSTDEF)
    {
      if (prepare_def_operand_for_rename (def, &uid))
	{
	  set_def_block (def, bb, false, false);
	  bitmap_set_bit (kills, uid);
	  REWRITE_THIS_STMT (stmt) = 1;
	}
    }
}


/* Given a set of blocks with variable definitions (DEF_BLOCKS),
   return a bitmap with all the blocks in the iterated dominance
   frontier of the blocks in DEF_BLOCKS.  DFS contains dominance
   frontier information as returned by compute_dominance_frontiers.
   
   The resulting set of blocks are the potential sites where PHI nodes
   are needed.  The caller is responsible from freeing the memory
   allocated for the return value.  */

static bitmap
find_idf (bitmap def_blocks, bitmap *dfs)
{
  bitmap_iterator bi;
  unsigned bb_index;
  VEC(basic_block) *work_stack;
  bitmap phi_insertion_points;

  work_stack = VEC_alloc (basic_block, n_basic_blocks);
  phi_insertion_points = BITMAP_XMALLOC ();

  /* Seed the work list with all the blocks in DEF_BLOCKS.  */
  EXECUTE_IF_SET_IN_BITMAP (def_blocks, 0, bb_index, bi)
    VEC_safe_push (basic_block, work_stack, BASIC_BLOCK (bb_index));

  /* Pop a block off the worklist, add every block that appears in
     the original block's DF that we have not already processed to
     the worklist.  Iterate until the worklist is empty.   Blocks
     which are added to the worklist are potential sites for
     PHI nodes.  */
  while (VEC_length (basic_block, work_stack) > 0)
    {
      basic_block bb = VEC_pop (basic_block, work_stack);
      bb_index = bb->index;
      
      EXECUTE_IF_AND_COMPL_IN_BITMAP (dfs[bb_index], phi_insertion_points,
				      0, bb_index, bi)
	{
	  bb = BASIC_BLOCK (bb_index);

	  /* Use a safe push because if there is a definition of VAR
	     in every basic block, then WORK_STACK may eventually have
	     more than N_BASIC_BLOCK entries.  */
	  VEC_safe_push (basic_block, work_stack, bb);
	  bitmap_set_bit (phi_insertion_points, bb_index);
	}
    }

  VEC_free (basic_block, work_stack);

  return phi_insertion_points;
}


/* Return the set of blocks where variable VAR is defined and the blocks
   where VAR is live on entry (livein).  Return NULL, if no entry is
   found in DEF_BLOCKS.  */

static inline struct def_blocks_d *
find_def_blocks_for (tree var)
{
  struct def_blocks_d dm;
  dm.var = var;
  return (struct def_blocks_d *) htab_find (def_blocks, &dm);
}


/* Insert PHI nodes for variable VAR using the iterated dominance
   frontier given in PHI_INSERTION_POINTS.  If UPDATE_P is true, this
   function assumes that the caller is incrementally updating the SSA
   form, in which case (1) VAR is assumed to be an SSA name, (2) a new
   SSA name is created for VAR's symbol, and, (3) all the arguments
   for the newly created PHI node are set to VAR.  */

static void
insert_phi_nodes_for (tree var, bitmap phi_insertion_points, bool update_p)
{
  unsigned bb_index;
  edge e;
  tree phi;
  basic_block bb;
  bitmap_iterator bi;
  struct def_blocks_d *def_map;
  tree sym;

  def_map = find_def_blocks_for (var);

  /* Remove the blocks where we already have PHI nodes for VAR.  */
  bitmap_and_compl_into (phi_insertion_points, def_map->phi_blocks);

  /* Now compute global livein for this variable.  Note this modifies
     def_map->livein_blocks.  */
  compute_global_livein (def_map->livein_blocks, def_map->def_blocks);

  /* If we are updating the SSA form, then VAR must already be an SSA
     name which is about to be replaced.  In this case, we create a
     new name for VAR's symbol and set all the arguments for the new
     PHI to be VAR.  When the renamer runs over this PHI node, it will
     retrieve the correct new name for each of the arguments.  */
  sym = (update_p) ? SSA_NAME_VAR (var) : var;

  /* And insert the PHI nodes.  */
  EXECUTE_IF_AND_IN_BITMAP (phi_insertion_points, def_map->livein_blocks,
			    0, bb_index, bi)
    {
      bb = BASIC_BLOCK (bb_index);
      phi = create_phi_node (sym, bb);

      /* If we are rewriting SSA names, add also the PHI arguments.
	 NOTE: this should be triggered when UPDATE_P is true, but
	 rewrite_ssa_into_ssa works differently than the incremental
	 updating algorithm.  When doing incremental updates we never
	 have the same SSA_NAME defined in more than one statement.
	 Instead, rewrite_ssa_into_ssa, relies on SSA_NAMEs being
	 defined multiple times.  If rewrite_ssa_into_ssa is ever
	 replaced with the incremental algorithm, this conditional
	 should test UPDATE_P.  */
      if (TREE_CODE (var) == SSA_NAME)
	{
	  edge_iterator ei;
	  FOR_EACH_EDGE (e, ei, bb->preds)
	    add_phi_arg (phi, var, e);

	  /* If updating the SSA form, mark the LHS of the new PHI
	     node to be a replacement for VAR.  Uses of VAR downstream
	     from this PHI node will be replaced with the new name
	     created by it.  */
	  if (update_p)
	    add_new_name_mapping (PHI_RESULT (phi), var, new_to_old);
	}

      /* Mark this PHI node as interesting for the rename process.  */
      REWRITE_THIS_STMT (phi) = 1;
    }
}


/* Helper for insert_phi_nodes.  If VAR needs PHI nodes, insert them
   at the dominance frontier (DFS) of blocks defining VAR.  */

static inline void
insert_phi_nodes_1 (tree var, bitmap *dfs)
{
  struct def_blocks_d *def_map;
  bitmap idf;

  def_map = find_def_blocks_for (var);
  if (def_map == NULL)
    return;

  idf = find_idf (def_map->def_blocks, dfs);

  if (get_phi_state (var) != NEED_PHI_STATE_NO)
    insert_phi_nodes_for (var, idf, false);

  BITMAP_FREE (idf);
}


/* Insert PHI nodes at the dominance frontier of blocks with variable
   definitions.  DFS contains the dominance frontier information for
   the flowgraph.  PHI nodes will only be inserted at the dominance
   frontier of definition blocks for variables whose NEED_PHI_STATE
   annotation is marked as ``maybe'' or ``unknown'' (computed by
   mark_def_sites).  If NAMES_TO_RENAME is not NULL, do the same but
   for ssa name rewriting.  */

static void
insert_phi_nodes (bitmap *dfs, bitmap names_to_rename)
{
  unsigned i;
  bitmap_iterator bi;

  timevar_push (TV_TREE_INSERT_PHI_NODES);

  /* Iterate over all variables in VARS_TO_RENAME.  For each variable, add
     to the work list all the blocks that have a definition for the
     variable.  PHI nodes will be added to the dominance frontier blocks of
     each definition block.  */
  if (names_to_rename)
    {
      EXECUTE_IF_SET_IN_BITMAP (names_to_rename, 0, i, bi)
	if (ssa_name (i))
	  insert_phi_nodes_1 (ssa_name (i), dfs);
    }
  else if (vars_to_rename)
    {
      EXECUTE_IF_SET_IN_BITMAP (vars_to_rename, 0, i, bi)
	insert_phi_nodes_1 (referenced_var (i), dfs);
    }
  else
    {
      for (i = 0; i < num_referenced_vars; i++)
	insert_phi_nodes_1 (referenced_var (i), dfs);
    }

  timevar_pop (TV_TREE_INSERT_PHI_NODES);
}


/* Register DEF (an SSA_NAME) to be a new definition for its underlying
   variable (SSA_NAME_VAR (DEF)) and push VAR's current reaching definition
   into the stack pointed by BLOCK_DEFS_P.  */

void
register_new_def (tree def, VEC (tree_on_heap) **block_defs_p)
{
  tree var = SSA_NAME_VAR (def);
  tree currdef;
   
  /* If this variable is set in a single basic block and all uses are
     dominated by the set(s) in that single basic block, then there is
     no reason to record anything for this variable in the block local
     definition stacks.  Doing so just wastes time and memory.

     This is the same test to prune the set of variables which may
     need PHI nodes.  So we just use that information since it's already
     computed and available for us to use.  */
  if (get_phi_state (var) == NEED_PHI_STATE_NO)
    {
      set_current_def (var, def);
      return;
    }

  currdef = get_current_def (var);

  /* Push the current reaching definition into *BLOCK_DEFS_P.  This stack is
     later used by the dominator tree callbacks to restore the reaching
     definitions for all the variables defined in the block after a recursive
     visit to all its immediately dominated blocks.  If there is no current
     reaching definition, then just record the underlying _DECL node.  */
  VEC_safe_push (tree_on_heap, *block_defs_p, currdef ? currdef : var);

  /* Set the current reaching definition for VAR to be DEF.  */
  set_current_def (var, def);
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

/* SSA Rewriting Step 1.  Initialization, create a block local stack
   of reaching definitions for new SSA names produced in this block
   (BLOCK_DEFS).  Register new definitions for every PHI node in the
   block.  */

static void
rewrite_initialize_block (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
			  basic_block bb)
{
  tree phi;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\n\nRenaming block #%d\n\n", bb->index);

  /* Mark the unwind point for this block.  */
  VEC_safe_push (tree_on_heap, block_defs_stack, NULL_TREE);

  /* Step 1.  Register new definitions for every PHI node in the block.
     Conceptually, all the PHI nodes are executed in parallel and each PHI
     node introduces a new version for the associated variable.  */
  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      tree result = PHI_RESULT (phi);
      if (REWRITE_THIS_STMT (phi))
	register_new_def (result, &block_defs_stack);
    }
}


/* Return the current definition for variable VAR.  If none is found,
   create a new SSA name to act as the zeroth definition for VAR.  If VAR
   is call clobbered and there exists a more recent definition of
   GLOBAL_VAR, return the definition for GLOBAL_VAR.  This means that VAR
   has been clobbered by a function call since its last assignment.  */

static tree
get_reaching_def (tree var)
{
  tree default_d, currdef_var, avar;
  
  /* Lookup the current reaching definition for VAR.  */
  default_d = NULL_TREE;
  currdef_var = get_current_def (var);

  /* If there is no reaching definition for VAR, create and register a
     default definition for it (if needed).  */
  if (currdef_var == NULL_TREE)
    {
      if (TREE_CODE (var) == SSA_NAME)
	avar = SSA_NAME_VAR (var);
      else
	avar = var;

      default_d = default_def (avar);
      if (default_d == NULL_TREE)
	{
	  default_d = make_ssa_name (avar, build_empty_stmt ());
	  set_default_def (avar, default_d);
	}
      set_current_def (var, default_d);
    }

  /* Return the current reaching definition for VAR, or the default
     definition, if we had to create one.  */
  return (currdef_var) ? currdef_var : default_d;
}


/* Replace the operand pointed by OP_P with its immediate reaching
   definition.  */

static inline void
rewrite_operand (use_operand_p op_p)
{
  tree var = USE_FROM_PTR (op_p);
  if (TREE_CODE (var) != SSA_NAME)
    SET_USE (op_p, get_reaching_def (var));
  else
    {
#if defined ENABLE_CHECKING
      /* If we get to this point, VAR is an SSA_NAME.  If VAR's symbol
	 was marked for renaming, make sure that its reaching
	 definition is VAR itself.  Otherwise, something has gone
	 wrong.  */
      tree sym = SSA_NAME_VAR (var);
      if (bitmap_bit_p (vars_to_rename, var_ann (sym)->uid))
	gcc_assert (var == get_reaching_def (SSA_NAME_VAR (var)));
#endif
    }
}


/* SSA Rewriting Step 2.  Rewrite every variable used in each statement in
   the block with its immediate reaching definitions.  Update the current
   definition of a variable when a new real or virtual definition is found.  */

static void
rewrite_stmt (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
	      basic_block bb ATTRIBUTE_UNUSED,
	      block_stmt_iterator si)
{
  stmt_ann_t ann;
  tree stmt;
  use_operand_p use_p;
  def_operand_p def_p;
  ssa_op_iter iter;

  stmt = bsi_stmt (si);
  ann = stmt_ann (stmt);

  /* If mark_def_sites decided that we don't need to rewrite this
     statement, ignore it.  */
  if (!REWRITE_THIS_STMT (stmt))
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Renaming statement ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  get_stmt_operands (stmt);

  /* Step 1.  Rewrite USES and VUSES in the statement.  */
  FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter, SSA_OP_ALL_USES | SSA_OP_ALL_KILLS)
    rewrite_operand (use_p);

  /* Step 2.  Register the statement's DEF and VDEF operands.  */
  FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_ALL_DEFS)
    {
      if (TREE_CODE (DEF_FROM_PTR (def_p)) != SSA_NAME)
	SET_DEF (def_p, make_ssa_name (DEF_FROM_PTR (def_p), stmt));

      /* FIXME: We shouldn't be registering new defs if the variable
	 doesn't need to be renamed.  */
      register_new_def (DEF_FROM_PTR (def_p), &block_defs_stack);
    }
}


/* SSA Rewriting Step 3.  Visit all the successor blocks of BB looking for
   PHI nodes.  For every PHI node found, add a new argument containing the
   current reaching definition for the variable and the edge through which
   that definition is reaching the PHI node.  */

static void
rewrite_add_phi_arguments (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
			   basic_block bb)
{
  edge e;
  edge_iterator ei;

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      tree phi;

      for (phi = phi_nodes (e->dest); phi; phi = PHI_CHAIN (phi))
	{
	  tree currdef;

	  /* If this PHI node has already been rewritten, then there is
	     nothing to do for this PHI or any following PHIs since we
	     always add new PHI nodes at the start of the PHI chain.  */
	  if (PHI_REWRITTEN (phi))
	    break;

	  currdef = get_reaching_def (SSA_NAME_VAR (PHI_RESULT (phi)));
	  add_phi_arg (phi, currdef, e);
	}
    }
}


/*  Rewrite existing virtual PHI arguments so that they have the correct
    reaching definitions.  BB is the basic block whose successors contain the
    PHI nodes we want to add arguments for.  */

static void
rewrite_virtual_phi_arguments (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
			       basic_block bb)
{
  edge e;
  use_operand_p op;
  edge_iterator ei;

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      tree phi;

      if (e->dest == EXIT_BLOCK_PTR)
	continue;

      for (phi = phi_nodes (e->dest); phi; phi = PHI_CHAIN (phi))
	{
	  tree result = PHI_RESULT (phi);
	  op = PHI_ARG_DEF_PTR_FROM_EDGE (phi, e);
	  
	  if (is_gimple_reg (result) 
	      || !bitmap_bit_p (vars_to_rename, 
				var_ann (SSA_NAME_VAR (result))->uid))
	    continue;

	  SET_USE (op, get_reaching_def (SSA_NAME_VAR (result)));
	  if (e->flags & EDGE_ABNORMAL)
	    SSA_NAME_OCCURS_IN_ABNORMAL_PHI (USE_FROM_PTR (op)) = 1;
	}
    }
}


/* Called after visiting basic block BB.  Restore CURRDEFS to its
   original value.  */

static void
rewrite_finalize_block (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
			basic_block bb ATTRIBUTE_UNUSED)
{
  /* Restore CURRDEFS to its original state.  */
  while (VEC_length (tree_on_heap, block_defs_stack) > 0)
    {
      tree tmp = VEC_pop (tree_on_heap, block_defs_stack);
      tree saved_def, var;

      if (tmp == NULL_TREE)
	break;

      /* If we recorded an SSA_NAME, then make the SSA_NAME the current
	 definition of its underlying variable.  If we recorded anything
	 else, it must have been an _DECL node and its current reaching
	 definition must have been NULL.  */
      if (TREE_CODE (tmp) == SSA_NAME)
	{
	  saved_def = tmp;
	  var = SSA_NAME_VAR (saved_def);
	}
      else
	{
	  saved_def = NULL;
	  var = tmp;
	}
                                                                                
      set_current_def (var, saved_def);
    }
}


/* Dump SSA information to FILE.  */

void
dump_tree_ssa (FILE *file)
{
  basic_block bb;
  const char *funcname
    = lang_hooks.decl_printable_name (current_function_decl, 2);

  fprintf (file, "SSA information for %s\n\n", funcname);

  FOR_EACH_BB (bb)
    {
      dump_bb (bb, file, 0);
      fputs ("    ", file);
      print_generic_stmt (file, phi_nodes (bb), dump_flags);
      fputs ("\n\n", file);
    }
}


/* Dump SSA information to stderr.  */

void
debug_tree_ssa (void)
{
  dump_tree_ssa (stderr);
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


/* Free memory allocated by one entry in DEF_BLOCKS.  */

static void
def_blocks_free (void *p)
{
  struct def_blocks_d *entry = p;
  BITMAP_XFREE (entry->def_blocks);
  BITMAP_XFREE (entry->phi_blocks);
  BITMAP_XFREE (entry->livein_blocks);
  free (entry);
}


/* Callback for htab_traverse to dump the DEF_BLOCKS hash table.  */

static int
debug_def_blocks_r (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct def_blocks_d *db_p = (struct def_blocks_d *) *slot;
  
  fprintf (stderr, "VAR: ");
  print_generic_expr (stderr, db_p->var, dump_flags);
  bitmap_print (stderr, db_p->def_blocks, ", DEF_BLOCKS: { ", "}");
  bitmap_print (stderr, db_p->livein_blocks, ", LIVEIN_BLOCKS: { ", "}\n");

  return 1;
}


/* Dump the DEF_BLOCKS hash table on stderr.  */

void
debug_def_blocks (void)
{
  htab_traverse (def_blocks, debug_def_blocks_r, NULL);
}


/* If a variable V in VARS_TO_RENAME is a pointer, the renaming
   process will cause us to lose the name memory tags that may have
   been associated with the various SSA_NAMEs of V.  This means that
   the variables aliased to those name tags also need to be renamed
   again.

   FIXME 1- We should either have a better scheme for renaming
	    pointers that doesn't lose name tags or re-run alias
	    analysis to recover points-to information.

	 2- Currently we just invalidate *all* the name tags.  This
	    should be more selective.  */

static void
invalidate_name_tags (bitmap vars_to_rename)
{
  unsigned i;
  bool rename_name_tags_p;
  bitmap_iterator bi;

  rename_name_tags_p = false;
  EXECUTE_IF_SET_IN_BITMAP (vars_to_rename, 0, i, bi)
    {
      if (POINTER_TYPE_P (TREE_TYPE (referenced_var (i))))
	{
	  rename_name_tags_p = true;
	  break;
	}
    }

  if (rename_name_tags_p)
    for (i = 0; i < num_referenced_vars; i++)
      {
	var_ann_t ann = var_ann (referenced_var (i));

	if (ann->mem_tag_kind == NAME_TAG)
	  {
	    size_t j;
	    varray_type may_aliases = ann->may_aliases;

	    bitmap_set_bit (vars_to_rename, ann->uid);
	    if (ann->may_aliases)
	      for (j = 0; j < VARRAY_ACTIVE_SIZE (may_aliases); j++)
		{
		  tree var = VARRAY_TREE (may_aliases, j);
		  bitmap_set_bit (vars_to_rename, var_ann (var)->uid);
		}
	  }
      }
}


/* Register NEW_NAME to be the new reaching definition for OLD_NAME.
   Used by the incremental SSA update routines to replace old SSA
   names with new ones.  The initial set of replacements is given to
   update_ssa.  This set may be later augmented if new PHI nodes are
   necessary for the new SSA names being introduced.  */

static void
register_new_update (tree old_name, tree new_name)
{
  tree currdef;
   
  currdef = get_current_def (old_name);

  /* Push the current reaching definition into *BLOCK_DEFS_P.  This stack is
     later used by the dominator tree callbacks to restore the reaching
     definitions for all the variables defined in the block after a recursive
     visit to all its immediately dominated blocks.  */
  VEC_safe_push (tree_on_heap, block_defs_stack, currdef);
  VEC_safe_push (tree_on_heap, block_defs_stack, old_name);

  /* Set the current reaching definition for OLD_NAME to be NEW_NAME.  */
  set_current_def (old_name, new_name);
}


/* Initialization of block data structures for the incremental SSA
   update pass.  Create a block local stack of reaching definitions
   for new SSA names produced in this block (BLOCK_DEFS).  Register
   new definitions for every PHI node in the block.  */

static void
rewrite_update_init_block (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
		           basic_block bb)
{
  edge e;
  edge_iterator ei;
  tree phi;
  bool is_abnormal_phi;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\n\nUpdating SSA form for block #%d\n\n", bb->index);

  /* Mark the unwind point for this block.  */
  VEC_safe_push (tree_on_heap, block_defs_stack, NULL_TREE);

  /* Mark the LHS if any of the arguments flows through an abnormal
     edge.  */
  is_abnormal_phi = false;
  FOR_EACH_EDGE (e, ei, bb->preds)
    if (e->flags & EDGE_ABNORMAL)
      {
	is_abnormal_phi = true;
	break;
      }

  /* If any of the PHI nodes is a replacement for a name in
     OLD_SSA_NAMES or it's one of the names in NEW_SSA_NAMES, then
     register it as a new definition for its corresponding name.  */
  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      tree lhs;

      if (!REWRITE_THIS_STMT (phi))
	continue;
      
      lhs = PHI_RESULT (phi);
      if (pointer_set_contains (new_ssa_names, lhs))
	register_new_update (name_replaced_by (lhs, new_to_old), lhs);
      else if (pointer_set_contains (old_ssa_names, lhs))
	register_new_update (lhs, lhs);

      if (is_abnormal_phi)
	SSA_NAME_OCCURS_IN_ABNORMAL_PHI (lhs) = 1;
    }
}


/* Called after visiting block BB.  Unwind BLOCK_DEFS_STACK to restore
   the current reaching definition of every name re-written in BB to
   the original reaching definition before visiting BB.  This
   unwinding must be done in the opposite order to what is done in
   register_new_update.  */

static void
rewrite_update_fini_block (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
			   basic_block bb ATTRIBUTE_UNUSED)
{
  while (VEC_length (tree_on_heap, block_defs_stack) > 0)
    {
      tree var = VEC_pop (tree_on_heap, block_defs_stack);
      tree saved_def;
      
      /* NULL indicates the unwind stop point for this block (see
	 rewrite_update_init_block).  */
      if (var == NULL)
	break;

      saved_def = VEC_pop (tree_on_heap, block_defs_stack);
      set_current_def (var, saved_def);
    }
}


/* Update every variable used in the statement pointed-to by SI.  The
   statement is assumed to be in SSA form already.  Names in
   OLD_SSA_NAMES used by SI will be updated to their current reaching
   definition.  Names in OLD_SSA_NAMES or NEW_SSA_NAMES defined by SI
   will be registered as a new definition for their corresponding name
   in OLD_SSA_NAMES.  */

static void
rewrite_update_stmt (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
		     basic_block bb ATTRIBUTE_UNUSED,
		     block_stmt_iterator si)
{
  stmt_ann_t ann;
  tree stmt;
  use_operand_p use_p;
  def_operand_p def_p;
  ssa_op_iter iter;

  stmt = bsi_stmt (si);
  ann = stmt_ann (stmt);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Updating SSA information for statement ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  /* Only update marked statements.  */
  if (!REWRITE_THIS_STMT (stmt))
    return;

  get_stmt_operands (stmt);

  /* Rewrite USES included in OLD_SSA_NAMES.  */
  FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter, SSA_OP_ALL_USES)
    {
      tree use = USE_FROM_PTR (use_p);
      if (pointer_set_contains (old_ssa_names, use))
	SET_USE (use_p, get_reaching_def (use));
    }

  /* Register definitions of names in NEW_SSA_NAMES and OLD_SSA_NAMES.  */
  FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_ALL_DEFS)
    {
      tree def = DEF_FROM_PTR (def_p);
      if (pointer_set_contains (new_ssa_names, def))
	register_new_update (name_replaced_by (def, new_to_old), def);
      else if (pointer_set_contains (old_ssa_names, def))
	register_new_update (def, def);
    }
}


/* Visit all the successor blocks of BB looking for PHI nodes.  For
   every PHI node found, check if any of its arguments is in
   OLD_SSA_NAMES.  If so, and if the argument has a current reaching
   definition, replace it.  */

static void
rewrite_update_phi_arguments (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
			      basic_block bb)
{
  edge e;
  edge_iterator ei;

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      tree phi;

      for (phi = phi_nodes (e->dest); phi; phi = PHI_CHAIN (phi))
	{
	  tree arg;
	  use_operand_p arg_p;

	  /* Skip PHI nodes that are not marked for rewrite.  */
	  if (!REWRITE_THIS_STMT (phi))
	    continue;

	  arg_p = PHI_ARG_DEF_PTR_FROM_EDGE (phi, e);
	  arg = USE_FROM_PTR (arg_p);
	  if (TREE_CODE (arg) == SSA_NAME
	      && pointer_set_contains (old_ssa_names, arg))
	    {
	      SET_USE (arg_p, get_reaching_def (arg));
	      if (e->flags & EDGE_ABNORMAL)
		SSA_NAME_OCCURS_IN_ABNORMAL_PHI (USE_FROM_PTR (arg_p)) = 1;
	    }
	}
    }
}


/* Rewrite the actual blocks, statements, and PHI arguments, to be in SSA
   form.  

   ENTRY indicates the block where to start.  Every block dominated by
      ENTRY will be rewritten.

   WHAT indicates what actions will be taken by the renamer (see enum
      rewrite_mode).  */

static void
rewrite_blocks (basic_block entry, enum rewrite_mode what)
{
  struct dom_walk_data walk_data;
  
  /* Rewrite all the basic blocks in the program.  */
  timevar_push (TV_TREE_SSA_REWRITE_BLOCKS);

  /* Setup callbacks for the generic dominator tree walker.  */
  walk_data.walk_stmts_backward = false;
  walk_data.dom_direction = CDI_DOMINATORS;
  walk_data.initialize_block_local_data = NULL;
  if (what == REWRITE_UPDATE)
    walk_data.before_dom_children_before_stmts = rewrite_update_init_block;
  else
    walk_data.before_dom_children_before_stmts = rewrite_initialize_block;

  if (what == REWRITE_ALL || what == REWRITE_FIX_DEF_DEF_CHAINS)
    walk_data.before_dom_children_walk_stmts = rewrite_stmt;
  else if (what == REWRITE_UPDATE)
    walk_data.before_dom_children_walk_stmts = rewrite_update_stmt;

  walk_data.before_dom_children_after_stmts = NULL;

  if (what == REWRITE_ALL)
    walk_data.before_dom_children_after_stmts = rewrite_add_phi_arguments;
  else if (what == REWRITE_FIX_DEF_DEF_CHAINS)
    walk_data.before_dom_children_after_stmts = rewrite_virtual_phi_arguments;
  else if (what == REWRITE_UPDATE)
    walk_data.before_dom_children_after_stmts = rewrite_update_phi_arguments;
  else
    gcc_unreachable ();
  
  walk_data.after_dom_children_before_stmts =  NULL;
  walk_data.after_dom_children_walk_stmts =  NULL;
  if (what == REWRITE_UPDATE)
    walk_data.after_dom_children_after_stmts = rewrite_update_fini_block;
  else
    walk_data.after_dom_children_after_stmts =  rewrite_finalize_block;
  walk_data.global_data = NULL;
  walk_data.block_local_data_size = 0;

  block_defs_stack = VEC_alloc (tree_on_heap, 10);

  /* Initialize the dominator walker.  */
  init_walk_dominator_tree (&walk_data);

  /* Recursively walk the dominator tree rewriting each statement in
     each basic block.  */
  walk_dominator_tree (&walk_data, entry);

  /* Finalize the dominator walker.  */
  fini_walk_dominator_tree (&walk_data);

  htab_delete (def_blocks);
  
  VEC_free (tree_on_heap, block_defs_stack);
  block_defs_stack = NULL;

  timevar_pop (TV_TREE_SSA_REWRITE_BLOCKS);
}


/* Block initialization routine for mark_def_sites.  Clear the 
   KILLS bitmap at the start of each block.  */

static void
mark_def_sites_initialize_block (struct dom_walk_data *walk_data,
				 basic_block bb ATTRIBUTE_UNUSED)
{
  struct mark_def_sites_global_data *gd = walk_data->global_data;
  bitmap kills = gd->kills;
  bitmap_clear (kills);
}


/* Mark the definition site blocks for each variable, so that we know where
   the variable is actually live.  */

static void 
mark_def_site_blocks (void)
{
  size_t i;
  struct dom_walk_data walk_data;
  struct mark_def_sites_global_data mark_def_sites_global_data;

  /* Allocate memory for the DEF_BLOCKS hash table.  */
  def_blocks = htab_create (VARRAY_ACTIVE_SIZE (referenced_vars),
			    def_blocks_hash, def_blocks_eq, def_blocks_free);

  for (i = 0; i < num_referenced_vars; i++)
    set_current_def (referenced_var (i), NULL_TREE);

  /* Ensure that the dominance information is OK.  */
  calculate_dominance_info (CDI_DOMINATORS);


  /* Setup callbacks for the generic dominator tree walker to find and
     mark definition sites.  */
  walk_data.walk_stmts_backward = false;
  walk_data.dom_direction = CDI_DOMINATORS;
  walk_data.initialize_block_local_data = NULL;
  walk_data.before_dom_children_before_stmts = mark_def_sites_initialize_block;
  walk_data.before_dom_children_walk_stmts = mark_def_sites;
  walk_data.before_dom_children_after_stmts = NULL; 
  walk_data.after_dom_children_before_stmts =  NULL;
  walk_data.after_dom_children_walk_stmts =  NULL;
  walk_data.after_dom_children_after_stmts =  NULL;

  /* Notice that this bitmap is indexed using variable UIDs, so it must be
     large enough to accommodate all the variables referenced in the
     function, not just the ones we are renaming.  */
  mark_def_sites_global_data.kills = BITMAP_XMALLOC ();
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
  BITMAP_XFREE (mark_def_sites_global_data.kills);
}


/* Main entry point into the SSA builder.  The renaming process
   proceeds in five main phases:

   1- If VARS_TO_RENAME has any entries, any existing PHI nodes for
      those variables are removed from the flow graph so that they can
      be computed again.

   2- Compute dominance frontier and immediate dominators, needed to
      insert PHI nodes and rename the function in dominator tree
      order.

   3- Find and mark all the blocks that define variables
      (mark_def_site_blocks).

   4- Insert PHI nodes at dominance frontiers (insert_phi_nodes).

   5- Rename all the blocks (rewrite_blocks) and statements in the program.

   Steps 3 and 5 are done using the dominator tree walker
   (walk_dominator_tree).

   ALL is true if all variables should be renamed (otherwise just those
   mentioned in vars_to_rename are taken into account).  */

void
rewrite_into_ssa (bool all)
{
  bitmap *dfs;
  basic_block bb;
  bitmap old_vars_to_rename = vars_to_rename;
  
  timevar_push (TV_TREE_SSA_OTHER);

  if (all)
    vars_to_rename = NULL;
  else
    {
      /* Initialize the array of variables to rename.  */
      gcc_assert (vars_to_rename);

      if (bitmap_empty_p (vars_to_rename))
	{
	  timevar_pop (TV_TREE_SSA_OTHER);
	  return;
	}
      
      invalidate_name_tags (vars_to_rename);

      /* Now remove all the existing PHI nodes (if any) for the variables
	 that we are about to rename into SSA.  */
      remove_all_phi_nodes_for (vars_to_rename);
    }

  mark_def_site_blocks ();

  /* Initialize dominance frontier and immediate dominator bitmaps. 
     Also count the number of predecessors for each block.  Doing so
     can save significant time during PHI insertion for large graphs.  */
  dfs = (bitmap *) xmalloc (last_basic_block * sizeof (bitmap *));
  FOR_EACH_BB (bb)
    dfs[bb->index] = BITMAP_XMALLOC ();

  /* Compute dominance frontiers.  */
  compute_dominance_frontiers (dfs);

  /* Insert PHI nodes at dominance frontiers of definition blocks.  */
  insert_phi_nodes (dfs, NULL);

  rewrite_blocks (ENTRY_BLOCK_PTR, REWRITE_ALL);

  /* Debugging dumps.  */
  if (dump_file && (dump_flags & TDF_STATS))
    {
      dump_dfa_stats (dump_file);
      dump_tree_ssa_stats (dump_file);
    }

  /* Free allocated memory.  */
  FOR_EACH_BB (bb)
    BITMAP_XFREE (dfs[bb->index]);
  free (dfs);

  vars_to_rename = old_vars_to_rename;
  timevar_pop (TV_TREE_SSA_OTHER);
}


/* Rewrites all variables into SSA.  */

static void
rewrite_all_into_ssa (void)
{
  rewrite_into_ssa (true);
}

struct tree_opt_pass pass_build_ssa = 
{
  "ssa",				/* name */
  NULL,					/* gate */
  rewrite_all_into_ssa,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg | PROP_referenced_vars,	/* properties_required */
  PROP_ssa,				/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_verify_ssa,	/* todo_flags_finish */
  0					/* letter */
};


/* Rewrite the def-def chains of virtual operands so that they have
   the correct reaching definitions.  */

void
rewrite_def_def_chains (void)
{
  /* Ensure that the dominance information is OK.  */
  calculate_dominance_info (CDI_DOMINATORS);
  mark_def_site_blocks ();
  rewrite_blocks (ENTRY_BLOCK_PTR, REWRITE_FIX_DEF_DEF_CHAINS);
}


/* Helper for prepare_blocks_for_update.  Recurse into the sub-graph
   rooted at BB processing statements that reference variables in
   OLD_SSA_NAMES.  Use BB_VISITED to avoid visiting the same block
   more than once.
   
   If BB contains a definition for a name in either NEW_SSA_NAMES or
   OLD_SSA_NAMES, mark BB as a definition site for that name.

   If BB contains a use for a name in OLD_SSA_NAMES, mark it as
   locally live in BB, unless the name is defined in BB (which would
   mean that its definition comes before the first use).  */

static void
prepare_block_for_update (basic_block bb, sbitmap bb_visited)
{
  basic_block son;
  block_stmt_iterator si;
  tree phi;

  if (TEST_BIT (bb_visited, bb->index))
    return;

  SET_BIT (bb_visited, bb->index);

  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      int i;
      tree lhs = PHI_RESULT (phi);

      REWRITE_THIS_STMT (phi) = 0;

      /* If this PHI creates one of the names in OLD_SSA_NAMES, mark
	 it as interesting to the renamer so that it can properly set
	 the current reaching definition when it finds it.  */
      if (pointer_set_contains (old_ssa_names, lhs))
	{
	  set_def_block (lhs, bb, true, true);
	  REWRITE_THIS_STMT (phi) = 1;
	}

      /* If any of the arguments uses one of the names in
	 OLD_SSA_NAMES, set the argument as live-on-entry to this
	 block and mark the PHI node for renaming.  */
      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	{
	  tree arg = PHI_ARG_DEF (phi, i);
	  if (TREE_CODE (arg) == SSA_NAME
	      && pointer_set_contains (old_ssa_names, arg))
	    {
	      REWRITE_THIS_STMT (phi) = 1;
	      set_livein_block (arg, bb);
	    }
	}
    }

  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      tree stmt;
      ssa_op_iter i;
      use_operand_p use_p;
      def_operand_p def_p;
      
      stmt = bsi_stmt (si);
      get_stmt_operands (stmt);
      REWRITE_THIS_STMT (stmt) = 0;

      FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, i, SSA_OP_DEF)
	{
	  tree def = DEF_FROM_PTR (def_p);

	  if (pointer_set_contains (new_ssa_names, def))
	    {
	      /* If DEF is a new name, then this is a definition site
		 for the name replaced by DEF.  */
	      REWRITE_THIS_STMT (stmt) = 1;
	      set_def_block (name_replaced_by (def, new_to_old), bb, false,
			     true);
	    }
	  else if (pointer_set_contains (old_ssa_names, def))
	    {
	      /* If DEF is an old name, this is a definition site for
		 DEF.  */
	      REWRITE_THIS_STMT (stmt) = 1;
	      set_def_block (def, bb, false, true);
	    }
	}

      FOR_EACH_SSA_USE_OPERAND (use_p, stmt, i, SSA_OP_USE)
	{
	  tree use = USE_FROM_PTR (use_p);
	  if (pointer_set_contains (old_ssa_names, use))
	    {
	      REWRITE_THIS_STMT (stmt) = 1;
	      if (bb_for_stmt (stmt) != bb)
		set_livein_block (use, bb);
	    }
	}
    }

  for (son = first_dom_son (CDI_DOMINATORS, bb);
       son;
       son = next_dom_son (CDI_DOMINATORS, son))
    prepare_block_for_update (son, bb_visited);
}


/* Remove from SET1 those blocks that do not dominate any block in SET2.  */

static void
remove_non_dominated (bitmap set1, bitmap set2)
{
  unsigned i, j;
  bitmap_iterator bi, bj;
  bitmap tmp = BITMAP_XMALLOC ();

  EXECUTE_IF_SET_IN_BITMAP (set1, 0, i, bi)
    EXECUTE_IF_SET_IN_BITMAP (set2, 0, j, bj)
    if (dominated_by_p (CDI_DOMINATORS, BASIC_BLOCK (j), BASIC_BLOCK (i)))
      {
	bitmap_set_bit (tmp, i);
	break;
      }

  bitmap_and_into (set1, tmp);
  BITMAP_FREE (tmp);
}


/* Given a set of newly created SSA names (NEW_NAMES) and a set of
   existing SSA names (OLD_NAMES), update the SSA form so that:

   1- The names in OLD_NAMES dominated by the definitions of
      NEW_NAMES are all re-written to be reached by the
      appropriate definition from NEW_NAMES.

   2- If needed, new PHI nodes are added to the iterated dominance
      frontier of the blocks where each of NEW_NAMES are defined.

   The two input vectors should be arranged so that for every I,
   OLD_NAMES[I] is replaced with NEW_NAMES[I].

   This function is useful when inserting new statements that generate
   new SSA names for GIMPLE registers.  Since SSA names for GIMPLE
   registers may have overlapping live ranges, it is not possible to
   just mark the base symbol for rewriting.  We would have to take the
   symbol out of SSA form and rename all uses of it.  That is
   expensive.

   Instead, the caller identifies the new names that have been
   inserted and the names that need to be replaced.  Note that the
   function assumes that the new names have already been inserted in
   the IL.  All this function does is rewire the use-def chains for
   the names in OLD_NAMES so that they use the names in
   NEW_NAMES.

   For instance, given the following code:

     1	L0:
     2	x_1 = PHI (0, x_5)
     3	if (x_1 < 10)
     4	  if (x_1 > 7)
     5	    y_2 = 0
     6	  else
     7	    y_3 = x_1 + x_7
     8	  endif
     9	  x_5 = x_1 + 1
     10   goto L0;
     11	endif

   Suppose that we insert new names x_10 and x_11 (lines 4 and 8).

     1	L0:
     2	x_1 = PHI (0, x_5)
     3	if (x_1 < 10)
     4	  x_10 = ...
     5	  if (x_1 > 7)
     6	    y_2 = 0
     7	  else
     8	    x_11 = ...
     9	    y_3 = x_1 + x_7
     10	  endif
     11	  x_5 = x_1 + 1
     12	  goto L0;
     13	endif

   We want to replace all the uses of x_1 with the new definitions of
   x_10 and x_11.  Note that the only uses that should be replaced are
   those at lines 5, 9 and 11.  Also, the use of x_7 at line 9 should
   *not* be replaced (this is why we cannot just mark symbol 'x' for
   renaming).

   Additionally, we will need to insert a PHI node at line 11 because
   that is a merge point for x_10 and x_11.  So the use of x_1 at line
   11 will be replaced with the new PHI node.  */

void
update_ssa (VEC (tree_on_heap) *new_names, VEC (tree_on_heap) *old_names)
{
  bitmap *dfs, blocks;
  basic_block bb, start_bb;
  bitmap_iterator bi;
  unsigned i;
  sbitmap bb_visited;
  tree name;

  timevar_push (TV_TREE_SSA_INCREMENTAL);

  /* Ensure that the dominance information is up-to-date and compute
     dominance frontiers.  */
  calculate_dominance_info (CDI_DOMINATORS);
  dfs = (bitmap *) xmalloc (last_basic_block * sizeof (bitmap *));
  FOR_EACH_BB (bb)
    dfs[bb->index] = BITMAP_XMALLOC ();
  compute_dominance_frontiers (dfs);

  /* For each SSA name N, the DEF_BLOCKS table describes where the name
     is defined, which blocks have PHI nodes for N, and which blocks
     have uses of N (i.e., N is live-on-entry in those blocks).  */
  def_blocks = htab_create (num_ssa_names, def_blocks_hash, def_blocks_eq,
			    def_blocks_free);
  blocks = BITMAP_XMALLOC ();
  bb_visited = sbitmap_alloc (n_basic_blocks);
  sbitmap_zero (bb_visited);
  old_ssa_names = pointer_set_create ();
  new_ssa_names = pointer_set_create ();
  new_to_old = htab_create (VEC_length (tree_on_heap, new_names),
			    new_to_old_hash, new_to_old_eq, free);

  /* Determine the blocks that define each of the names in NEW_NAMES.
     Build a replacement table to map between new and old SSA
     names.  */
  for (i = 0; VEC_iterate (tree_on_heap, new_names, i, name); i++)
    {
      basic_block bb;
      tree old = VEC_index (tree_on_heap, old_names, i);

      /* Add definition sites for new and old SSA names to BLOCKS.  */
      bb = bb_for_stmt (SSA_NAME_DEF_STMT (name));
      bitmap_set_bit (blocks, bb->index);
      bb = bb_for_stmt (SSA_NAME_DEF_STMT (old));
      if (bb)
	bitmap_set_bit (blocks, bb->index);

      /* Create the mapping NEW <- OLD for every SSA name to be
	 replaced.  Also add to BLOCKS the definition sites for
	 variables in OLD_NAMES.  */
      add_new_name_mapping (name, old, new_to_old);

      /* Add old SSA names to OLD_SSA_NAMES so that they can be looked up
	 quickly during renaming.  */
      pointer_set_insert (old_ssa_names, old);
    }

  /* Traverse all the collected blocks and their dominated sub-graphs,
     marking statements for renaming and setting local live-in
     information for the PHI placement heuristics.  */
  EXECUTE_IF_SET_IN_BITMAP (blocks, 0, i, bi)
    prepare_block_for_update (BASIC_BLOCK (i), bb_visited);

  /* Insert PHI nodes at the iterated dominance frontier of every
     block making new definitions for names in OLD_NAMES.  */
  for (i = 0; VEC_iterate (tree_on_heap, old_names, i, name); i++)
    {
      struct def_blocks_d *db;
      bitmap idf;

      db = find_def_blocks_for (name);
      idf = find_idf (db->def_blocks, dfs);

      /* We are only interested in inserting PHI nodes on IDF blocks
	 that dominate blocks where NAME is live-on-entry.  Otherwise,
	 we would create unnecessary PHI nodes that could trigger
	 false uninitialized use warnings.  */
      remove_non_dominated (idf, db->livein_blocks);

      if (!bitmap_empty_p (idf))
	{
	  insert_phi_nodes_for (name, idf, true);

	  /* Add the IDF blocks to BLOCKS to find a common dominator
	     where to start renaming at.  Since blocks in the iterated
	     dominance frontier may also dominate blocks in BLOCKS, we
	     need to count them in.  */
	  bitmap_ior_into (blocks, idf);
	}

      BITMAP_FREE (idf);
    }

  /* The renaming algorithm starts at the first block that
     dominates all the blocks in BLOCKS.  If the nearest common
     dominator has more than one predecessor, the renaming process may
     need to add PHI nodes in it.  In this case, at least one of the
     incoming edges will come from outside BLOCKS, leading to a PHI
     node with a NULL argument.  To avoid this problem, we just move
     one level up to START_BB's dominator.  */
  start_bb = nearest_common_dominator_for_set (CDI_DOMINATORS, blocks);
  if (EDGE_COUNT (start_bb->preds) > 1)
    start_bb = get_immediate_dominator (CDI_DOMINATORS, start_bb);

  /* Reset the current definition for every symbol before renaming the
     sub-graph.  */
  for (i = 0; VEC_iterate (tree_on_heap, old_names, i, name); i++)
    {
      tree default_def = var_ann (SSA_NAME_VAR (name))->default_def;
      set_current_def (name, (name != default_def) ? NULL_TREE : name);
    }

  /* Now start the renaming process at START_BB.  */
  rewrite_blocks (start_bb, REWRITE_UPDATE);

  /* Debugging dumps.  */
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nSSA replacement table\n");
      for (i = 0; VEC_iterate (tree_on_heap, new_names, i, name); i++)
	{
	  tree old = VEC_index (tree_on_heap, old_names, i);
	  print_generic_expr (dump_file, old, i);
	  fprintf (dump_file, " -> ");
	  print_generic_expr (dump_file, name, i);
	  fprintf (dump_file, "\n");
	}
      fprintf (dump_file, "\n");

      fprintf (dump_file, "Affected blocks: ");
      EXECUTE_IF_SET_IN_BITMAP (blocks, 0, i, bi)
	fprintf (dump_file, "%d ", i);
      fprintf (dump_file, "\n");
      fprintf (dump_file, "\n");

      fprintf (dump_file, "Rename started at nearest common dominator: ");
      fprintf (dump_file, "%d\n\n", start_bb->index);
    }

  /* Free allocated memory.  */
  FOR_EACH_BB (bb)
    BITMAP_XFREE (dfs[bb->index]);
  free (dfs);
  BITMAP_FREE (blocks);
  sbitmap_free (bb_visited);
  sbitmap_free (old_ssa_names);
  free (new_to_old);

  for (i = 0; VEC_iterate (tree_on_heap, new_names, i, name); i++)
    {
      tree old = VEC_index (tree_on_heap, old_names, i);

      free (SSA_NAME_AUX (name));
      SSA_NAME_AUX (name) = NULL;

      free (SSA_NAME_AUX (old));
      SSA_NAME_AUX (old) = NULL;
    }

  timevar_pop (TV_TREE_SSA_INCREMENTAL);
}


/*---------------------------------------------------------------------------
    Functions to fix a program in invalid SSA form into valid SSA
    form.  The main entry point here is rewrite_ssa_into_ssa.
---------------------------------------------------------------------------*/

/* Called after visiting basic block BB.  Restore CURRDEFS to its
   original value.  */

static void
ssa_rewrite_finalize_block (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
			    basic_block bb ATTRIBUTE_UNUSED)
{

  /* Step 5.  Restore the current reaching definition for each variable
     referenced in the block (in reverse order).  */
  while (VEC_length (tree_on_heap, block_defs_stack) > 0)
    {
      tree var = VEC_pop (tree_on_heap, block_defs_stack);
      tree saved_def;
      
      if (var == NULL)
	break;

      saved_def = VEC_pop (tree_on_heap, block_defs_stack);
      set_current_def (var, saved_def);
    }
}


/* Register DEF (an SSA_NAME) to be a new definition for the original
   ssa name VAR and push VAR's current reaching definition
   into the stack pointed by BLOCK_DEFS_P.  */

static void
ssa_register_new_def (tree var, tree def)
{
  tree currdef;
   
  /* If this variable is set in a single basic block and all uses are
     dominated by the set(s) in that single basic block, then there is
     nothing to do.  TODO we should not be called at all, and just
     keep the original name.  */
  if (get_phi_state (var) == NEED_PHI_STATE_NO)
    {
      set_current_def (var, def);
      return;
    }

  currdef = get_current_def (var);

  /* Push the current reaching definition into *BLOCK_DEFS_P.  This stack is
     later used by the dominator tree callbacks to restore the reaching
     definitions for all the variables defined in the block after a recursive
     visit to all its immediately dominated blocks.  */
  VEC_safe_push (tree_on_heap, block_defs_stack, currdef);
  VEC_safe_push (tree_on_heap, block_defs_stack, var);

  /* Set the current reaching definition for VAR to be DEF.  */
  set_current_def (var, def);
}


/* Same as rewrite_stmt, for rewriting ssa names.  */

static void
ssa_rewrite_stmt (struct dom_walk_data *walk_data,
		  basic_block bb ATTRIBUTE_UNUSED,
		  block_stmt_iterator si)
{
  stmt_ann_t ann;
  tree stmt, var;
  ssa_op_iter iter;
  use_operand_p use_p;
  def_operand_p def_p;
  sbitmap names_to_rename = walk_data->global_data;

  stmt = bsi_stmt (si);
  ann = stmt_ann (stmt);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Renaming statement ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  /* We have just scanned the code for operands.  No statement should
     be modified.  */
  gcc_assert (!ann->modified);

  /* Step 1.  Rewrite USES and VUSES in the statement.  */
  FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter, SSA_OP_ALL_USES | SSA_OP_ALL_KILLS)
    {
      if (TEST_BIT (names_to_rename, SSA_NAME_VERSION (USE_FROM_PTR (use_p))))
	SET_USE (use_p, get_reaching_def (USE_FROM_PTR (use_p)));
    }

  /* Step 2.  Register the statement's DEF and VDEF operands.  */
  FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_ALL_DEFS)
    {
      var = DEF_FROM_PTR (def_p);

      if (!TEST_BIT (names_to_rename, SSA_NAME_VERSION (var)))
	continue;

      SET_DEF (def_p, duplicate_ssa_name (var, stmt));
      ssa_register_new_def (var, DEF_FROM_PTR (def_p));
    }
}


/* Ditto, for ssa name rewriting.  */

static void
ssa_rewrite_phi_arguments (struct dom_walk_data *walk_data, basic_block bb)
{
  edge e;
  sbitmap names_to_rename = walk_data->global_data;
  use_operand_p op;
  edge_iterator ei;

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      tree phi;

      if (e->dest == EXIT_BLOCK_PTR)
	continue;

      for (phi = phi_nodes (e->dest); phi; phi = PHI_CHAIN (phi))
	{
	  op = PHI_ARG_DEF_PTR_FROM_EDGE (phi, e);
	  if (TREE_CODE (USE_FROM_PTR (op)) != SSA_NAME)
	    continue;
	  
	  if (!TEST_BIT (names_to_rename, SSA_NAME_VERSION (USE_FROM_PTR (op))))
	    continue; 

	  SET_USE (op, get_reaching_def (USE_FROM_PTR (op)));
	  if (e->flags & EDGE_ABNORMAL)
	    SSA_NAME_OCCURS_IN_ABNORMAL_PHI (USE_FROM_PTR (op)) = 1;
	}
    }
}

/* Ditto, for rewriting ssa names.  */

static void
ssa_rewrite_initialize_block (struct dom_walk_data *walk_data, basic_block bb)
{
  tree phi, new_name;
  sbitmap names_to_rename = walk_data->global_data;
  edge e;
  bool abnormal_phi;
  edge_iterator ei;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\n\nRenaming block #%d\n\n", bb->index);

  /* Mark the unwind point for this block.  */
  VEC_safe_push (tree_on_heap, block_defs_stack, NULL_TREE);

  FOR_EACH_EDGE (e, ei, bb->preds)
    if (e->flags & EDGE_ABNORMAL)
      break;
  abnormal_phi = (e != NULL);

  /* Step 1.  Register new definitions for every PHI node in the block.
     Conceptually, all the PHI nodes are executed in parallel and each PHI
     node introduces a new version for the associated variable.  */
  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      tree result = PHI_RESULT (phi);

      if (TEST_BIT (names_to_rename, SSA_NAME_VERSION (result)))
	{
	  new_name = duplicate_ssa_name (result, phi);
	  SET_PHI_RESULT (phi, new_name);

	  if (abnormal_phi)
	    SSA_NAME_OCCURS_IN_ABNORMAL_PHI (new_name) = 1;
	  ssa_register_new_def (result, new_name);
	}
    }
}


/* Same as mark_def_sites, but works over SSA names.  */

static void
ssa_mark_def_sites (struct dom_walk_data *walk_data,
		    basic_block bb,
		    block_stmt_iterator bsi)
{
  struct mark_def_sites_global_data *gd = walk_data->global_data;
  bitmap kills = gd->kills;
  size_t uid, def_uid;
  tree stmt, use, def;
  ssa_op_iter iter;

  /* Mark all the blocks that have definitions for each variable in the
     names_to_rename bitmap.  */
  stmt = bsi_stmt (bsi);
  get_stmt_operands (stmt);

  /* If a variable is used before being set, then the variable is live
     across a block boundary, so mark it live-on-entry to BB.  */
  FOR_EACH_SSA_TREE_OPERAND (use, stmt, iter, SSA_OP_ALL_USES | SSA_OP_ALL_KILLS)
    {
      uid = SSA_NAME_VERSION (use);

      if (TEST_BIT (gd->names_to_rename, uid)
	  && !bitmap_bit_p (kills, uid))
	set_livein_block (use, bb);
    }
	  
  /* Now process the definition made by this statement.  Mark the
     variables in KILLS.  */
  FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_ALL_DEFS)
    {
      def_uid = SSA_NAME_VERSION (def);

      if (TEST_BIT (gd->names_to_rename, def_uid))
	{
	  set_def_block (def, bb, false, true);
	  bitmap_set_bit (kills, def_uid);
	}
    }
}


/* Block initialization routine for mark_def_sites.  Clear the 
   KILLS bitmap at the start of each block.  */

static void
ssa_mark_def_sites_initialize_block (struct dom_walk_data *walk_data,
				     basic_block bb)
{
  struct mark_def_sites_global_data *gd = walk_data->global_data;
  bitmap kills = gd->kills;
  tree phi, def;
  unsigned def_uid;

  bitmap_clear (kills);

  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      def = PHI_RESULT (phi);
      def_uid = SSA_NAME_VERSION (def);

      if (!TEST_BIT (gd->names_to_rename, def_uid))
	continue;

      set_def_block (def, bb, true, true);
      bitmap_set_bit (kills, def_uid);
    }
}

/* Marks ssa names used as arguments of phis at the end of BB.  */

static void
ssa_mark_phi_uses (struct dom_walk_data *walk_data, basic_block bb)
{
  struct mark_def_sites_global_data *gd = walk_data->global_data;
  bitmap kills = gd->kills;
  edge e;
  tree phi, use;
  unsigned uid;
  edge_iterator ei;

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      if (e->dest == EXIT_BLOCK_PTR)
	continue;

      for (phi = phi_nodes (e->dest); phi; phi = PHI_CHAIN (phi))
	{
	  use = PHI_ARG_DEF_FROM_EDGE (phi, e);
	  if (TREE_CODE (use) != SSA_NAME)
	    continue;

	  uid = SSA_NAME_VERSION (use);

	  if (TEST_BIT (gd->names_to_rename, uid)
	      && !bitmap_bit_p (kills, uid))
	    set_livein_block (use, bb);
	}
    }
}
       
   
/* The marked ssa names may have more than one definition;
   add PHI nodes and rewrite them to fix this.  */

void
rewrite_ssa_into_ssa (void)
{
  bitmap *dfs;
  basic_block bb;
  struct dom_walk_data walk_data;
  struct mark_def_sites_global_data mark_def_sites_global_data;
  unsigned i;
  sbitmap snames_to_rename;
  bitmap to_rename;
  bitmap_iterator bi;
  
  if (!any_marked_for_rewrite_p ())
    return;
  to_rename = marked_ssa_names ();

  timevar_push (TV_TREE_SSA_OTHER);

  /* Allocate memory for the DEF_BLOCKS hash table.  */
  def_blocks = htab_create (num_ssa_names,
			    def_blocks_hash, def_blocks_eq, def_blocks_free);

  /* Initialize dominance frontier and immediate dominator bitmaps. 
     Also count the number of predecessors for each block.  Doing so
     can save significant time during PHI insertion for large graphs.  */
  dfs = (bitmap *) xmalloc (last_basic_block * sizeof (bitmap *));
  FOR_EACH_BB (bb)
    dfs[bb->index] = BITMAP_XMALLOC ();

  /* Ensure that the dominance information is OK.  */
  calculate_dominance_info (CDI_DOMINATORS);

  /* Compute dominance frontiers.  */
  compute_dominance_frontiers (dfs);

  /* Setup callbacks for the generic dominator tree walker to find and
     mark definition sites.  */
  walk_data.walk_stmts_backward = false;
  walk_data.dom_direction = CDI_DOMINATORS;
  walk_data.initialize_block_local_data = NULL;
  walk_data.before_dom_children_before_stmts
	  = ssa_mark_def_sites_initialize_block;
  walk_data.before_dom_children_walk_stmts = ssa_mark_def_sites;
  walk_data.before_dom_children_after_stmts = ssa_mark_phi_uses; 
  walk_data.after_dom_children_before_stmts =  NULL;
  walk_data.after_dom_children_walk_stmts =  NULL;
  walk_data.after_dom_children_after_stmts =  NULL;

  snames_to_rename = sbitmap_alloc (num_ssa_names);
  sbitmap_zero (snames_to_rename);
  EXECUTE_IF_SET_IN_BITMAP (to_rename, 0, i, bi)
    {
      SET_BIT (snames_to_rename, i);
      set_current_def (ssa_name (i), NULL_TREE);
    }

  mark_def_sites_global_data.kills = BITMAP_XMALLOC ();
  mark_def_sites_global_data.names_to_rename = snames_to_rename;
  walk_data.global_data = &mark_def_sites_global_data;

  block_defs_stack = VEC_alloc (tree_on_heap, 10);

  /* We do not have any local data.  */
  walk_data.block_local_data_size = 0;

  /* Initialize the dominator walker.  */
  init_walk_dominator_tree (&walk_data);

  /* Recursively walk the dominator tree.  */
  walk_dominator_tree (&walk_data, ENTRY_BLOCK_PTR);

  /* Finalize the dominator walker.  */
  fini_walk_dominator_tree (&walk_data);

  /* We no longer need this bitmap, clear and free it.  */
  BITMAP_XFREE (mark_def_sites_global_data.kills);

  /* Insert PHI nodes at dominance frontiers of definition blocks.  */
  insert_phi_nodes (dfs, to_rename);

  /* Rewrite all the basic blocks in the program.  */
  timevar_push (TV_TREE_SSA_REWRITE_BLOCKS);

  /* Setup callbacks for the generic dominator tree walker.  */
  walk_data.walk_stmts_backward = false;
  walk_data.dom_direction = CDI_DOMINATORS;
  walk_data.initialize_block_local_data = NULL;
  walk_data.before_dom_children_before_stmts = ssa_rewrite_initialize_block;
  walk_data.before_dom_children_walk_stmts = ssa_rewrite_stmt;
  walk_data.before_dom_children_after_stmts = ssa_rewrite_phi_arguments;
  walk_data.after_dom_children_before_stmts = NULL;
  walk_data.after_dom_children_walk_stmts =  NULL;
  walk_data.after_dom_children_after_stmts =  ssa_rewrite_finalize_block;
  walk_data.global_data = snames_to_rename;
  walk_data.block_local_data_size = 0;

  /* Initialize the dominator walker.  */
  init_walk_dominator_tree (&walk_data);

  /* Recursively walk the dominator tree rewriting each statement in
     each basic block.  */
  walk_dominator_tree (&walk_data, ENTRY_BLOCK_PTR);

  /* Finalize the dominator walker.  */
  fini_walk_dominator_tree (&walk_data);

  unmark_all_for_rewrite ();

  EXECUTE_IF_SET_IN_BITMAP (to_rename, 0, i, bi)
    {
      /* Free SSA_NAME_AUX.  We don't have to zero it because
	 release_ssa_name will.  */
      if (SSA_NAME_AUX (ssa_name (i)))
	free (SSA_NAME_AUX (ssa_name (i)));

      release_ssa_name (ssa_name (i));
    }

  sbitmap_free (snames_to_rename);

  timevar_pop (TV_TREE_SSA_REWRITE_BLOCKS);

  /* Debugging dumps.  */
  if (dump_file && (dump_flags & TDF_STATS))
    {
      dump_dfa_stats (dump_file);
      dump_tree_ssa_stats (dump_file);
    }

  /* Free allocated memory.  */
  FOR_EACH_BB (bb)
    BITMAP_XFREE (dfs[bb->index]);
  free (dfs);

  htab_delete (def_blocks);

#ifdef ENABLE_CHECKING
  for (i = 1; i < num_ssa_names; i++)
    {
      tree name = ssa_name (i);
      if (!name)
	continue;

      gcc_assert (SSA_NAME_AUX (name) == NULL);
    }
#endif

  BITMAP_XFREE (to_rename);
  
  VEC_free (tree_on_heap, block_defs_stack);
  block_defs_stack = NULL;
  timevar_pop (TV_TREE_SSA_OTHER);
}
