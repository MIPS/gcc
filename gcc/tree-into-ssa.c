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
static sbitmap old_ssa_names;

/* Set of new SSA names being added by update_ssa.  */
static sbitmap new_ssa_names;

/* Growth factor for NEW_SSA_NAMES and OLD_SSA_NAMES.  These sets need
   to grow as the callers to register_new_name_mapping will typically
   create new names on the fly.  FIXME.  Currently set to 1/3 to avoid
   frequent reallocations but still need to find a reasonable growth
   strategy.  */
#define NAME_SETS_GROWTH_FACTOR	(MAX (3, num_ssa_names / 3))

/* Tuple used to represent replacement mappings.  */
struct repl_map_d
{
  tree name;
  bitmap set;
};

/* NEW -> OLD_SET replacement table.  If we are replacing several
   existing SSA names O_1, O_2, ..., O_j with a new name N_i,
   then REPL_TBL[N_i] = { O_1, O_2, ..., O_j }.  */
static htab_t repl_tbl;

/* true if register_new_name_mapping needs to initialize the data
   structures needed by update_ssa.  */
static bool need_to_initialize_update_ssa_p = true;

/* true if update_ssa needs to update virtual operands.  */
static bool need_to_update_vops_p = false;

/* Global data to attach to the main dominator walk structure.  */
struct mark_def_sites_global_data
{
  /* This bitmap contains the variables which are set before they
     are used in a basic block.  */
  bitmap kills;

  /* Bitmap of names to rename.  */
  sbitmap names_to_rename;

  /* Set of blocks that mark_def_sites deems interesting for the
     renamer to process.  */
  sbitmap interesting_blocks;
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

/* Use the unsigned flag to keep track of which statements we want to
   visit when marking new definition sites.  This is slightly
   different than REWRITE_THIS_STMT: it's used by update_ssa to
   distinguish statements that need to have both uses and defs
   processed from those that only need to have their defs processed.
   Statements that define new SSA names only need to have their defs
   registered, but they don't need to have their uses renamed.  */
#define REGISTER_DEFS_IN_THIS_STMT(T)	(T)->common.unsigned_flag


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
    = (basic_block *) xmalloc (sizeof (basic_block) * (last_basic_block + 1));

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
      db_p->def_blocks = BITMAP_ALLOC (NULL);
      db_p->phi_blocks = BITMAP_ALLOC (NULL);
      db_p->livein_blocks = BITMAP_ALLOC (NULL);
      *slot = (void *) db_p;
    }
  else
    db_p = (struct def_blocks_d *) *slot;

  return db_p;
}


/* Mark block BB as the definition site for variable VAR.  PHI_P is true if
   VAR is defined by a PHI node.  */

static void
set_def_block (tree var, basic_block bb, bool phi_p)
{
  struct def_blocks_d *db_p;
  enum need_phi_state state;

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


/* Return true if the underlying symbol for NAME is marked for
   renaming.  Note that in this context we may assume that a naked
   symbol is marked for renaming (this avoids the more expensive
   bitmap presence test).  */

static inline bool
symbol_marked_for_renaming (tree name)
{
  return DECL_P (name)
	 || bitmap_bit_p (vars_to_rename, var_ann (SSA_NAME_VAR (name))->uid);
}


/* Hashing and equality functions for REPL_TBL.  */

static hashval_t
repl_map_hash (const void *p)
{
  return htab_hash_pointer ((const void *)((const struct repl_map_d *)p)->name);
}

static int
repl_map_eq (const void *p1, const void *p2)
{
  return ((const struct repl_map_d *)p1)->name
	 == ((const struct repl_map_d *)p2)->name;
}

static void
repl_map_free (void *p)
{
  BITMAP_FREE (((struct repl_map_d *)p)->set);
  free (p);
}


/* Return the names replaced by NEW (i.e., REPL_TBL[NEW].SET).  */

static inline bitmap
names_replaced_by (tree new)
{
  struct repl_map_d m;
  void **slot;

  m.name = new;
  slot = htab_find_slot (repl_tbl, (void *) &m, NO_INSERT);

  /* If N was not registered in the replacement table, return NULL.  */
  if (slot == NULL || *slot == NULL)
    return NULL;

  return ((struct repl_map_d *) *slot)->set;
}


/* Register symbol SYM to be renamed by update_ssa.  */

static void
mark_sym_for_renaming (tree sym)
{
  need_to_update_vops_p = true;
  bitmap_set_bit (vars_to_rename, var_ann (sym)->uid);
}


/* Add OLD to REPL_TBL[NEW].SET.  */

static inline void
add_to_repl_tbl (tree new, tree old)
{
  struct repl_map_d m, *mp;
  void **slot;

  m.name = new;
  slot = htab_find_slot (repl_tbl, (void *) &m, INSERT);
  if (*slot == NULL)
    {
      mp = xmalloc (sizeof (*mp));
      mp->name = new;
      mp->set = BITMAP_ALLOC (NULL);
      *slot = (void *) mp;
    }
  else
    mp = (struct repl_map_d *) *slot;

  bitmap_set_bit (mp->set, SSA_NAME_VERSION (old));
}


/* Add a new mapping NEW -> OLD REPL_TBL.  Every entry N_i in REPL_TBL
   represents the set of names O_1 ... O_j replaced by N_i.  This is
   used by update_ssa and its helpers to introduce new SSA names in an
   already formed SSA web.  */

static void
add_new_name_mapping (tree new, tree old)
{
  timevar_push (TV_TREE_SSA_INCREMENTAL);

  /* We may need to grow NEW_SSA_NAMES and OLD_SSA_NAMES because our
     caller may have created new names since the set was created.  */
  if (new_ssa_names->n_bits <= num_ssa_names - 1)
    {
      unsigned int new_sz = num_ssa_names + NAME_SETS_GROWTH_FACTOR;
      new_ssa_names = sbitmap_resize (new_ssa_names, new_sz, 0);
      old_ssa_names = sbitmap_resize (old_ssa_names, new_sz, 0);
    }

  /* We don't need to keep replacement mappings for virtual names.
     Since these names are kept in FUD-chain form, just add NEW to
     NEW_SSA_NAMES and mark its symbol for renaming.  */
  if (!is_gimple_reg (new))
    {
      tree sym;

      gcc_assert (!is_gimple_reg (old));

      if (TREE_CODE (new) == SSA_NAME)
	{
	  SET_BIT (new_ssa_names, SSA_NAME_VERSION (new));
	  sym = SSA_NAME_VAR (new);
	}
      else
	sym = new;

      mark_sym_for_renaming (sym);

      timevar_pop (TV_TREE_SSA_INCREMENTAL);

      return;
    }

  /* Assume that OLD and NEW are different GIMPLE register names.  */
  gcc_assert (new != old && is_gimple_reg (old));

  /* Update the REPL_TBL table.  */
  add_to_repl_tbl (new, old);

  /* If OLD had already been registered as a new name, then all the
     names that OLD replaces should also be replaced by NEW.  */
  if (TEST_BIT (new_ssa_names, SSA_NAME_VERSION (old)))
    bitmap_ior_into (names_replaced_by (new), names_replaced_by (old));

  /* Register NEW and OLD in NEW_SSA_NAMES and OLD_SSA_NAMES,
     respectively.  */
  SET_BIT (new_ssa_names, SSA_NAME_VERSION (new));
  SET_BIT (old_ssa_names, SSA_NAME_VERSION (old));

  timevar_pop (TV_TREE_SSA_INCREMENTAL);
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


/* If symbol SYM needs to be renamed, set *UID_P to the underlying
   variable's uid and return true.  Otherwise return false.  */

static bool
prepare_def_operand_for_rename (tree sym, size_t *uid_p)
{
  *uid_p = var_ann (sym)->uid;

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
	  tree sym = USE_FROM_PTR (use_p);
	  if (!bitmap_bit_p (kills, uid))
	    set_livein_block (sym, bb);
	  REWRITE_THIS_STMT (stmt) = 1;
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
	  tree sym = USE_FROM_PTR (use_p);
          set_livein_block (sym, bb);
	  set_def_block (sym, bb, false);
	  REWRITE_THIS_STMT (stmt) = 1;
	}
    }

  /* Now process the defs and must-defs made by this statement.  */
  FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_DEF | SSA_OP_VMUSTDEF)
    {
      tree sym = DECL_P (def) ? def : SSA_NAME_VAR (def);
      if (prepare_def_operand_for_rename (sym, &uid))
	{
	  set_def_block (sym, bb, false);
	  bitmap_set_bit (kills, uid);
	  REWRITE_THIS_STMT (stmt) = 1;
	}
    }

  /* If we found the statement interesting then also mark the block BB
     as interesting.  */
  if (REWRITE_THIS_STMT (stmt))
    SET_BIT (gd->interesting_blocks, bb->index);
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

  work_stack = VEC_alloc (basic_block, last_basic_block);
  phi_insertion_points = BITMAP_ALLOC (NULL);

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

      /* Since the registration of NEW -> OLD name mappings is done
	 separately from the call to update_ssa, when updating the SSA
	 form, the basic blocks where new and/or old names are defined
	 may have disappeared by CFG cleanup calls.  In this case,
	 we may pull a NULL block from the work stack.  */
      if (bb == NULL)
	continue;

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


/* Retrieve or create a default definition for symbol SYM.  */

static inline tree
get_default_def_for (tree sym)
{
  tree ddef = default_def (sym);

  if (ddef == NULL_TREE)
    {
      ddef = make_ssa_name (sym, build_empty_stmt ());
      set_default_def (sym, ddef);
    }

  return ddef;
}


/* Insert PHI nodes for variable VAR using the iterated dominance
   frontier given in PHI_INSERTION_POINTS.  If UPDATE_P is true, this
   function assumes that the caller is incrementally updating the SSA
   form, in which case (1) VAR is assumed to be an SSA name, (2) a new
   SSA name is created for VAR's symbol, and, (3) all the arguments
   for the newly created PHI node are set to VAR.

   PHI_INSERTION_POINTS is updated to reflect nodes that already had a
   PHI node for VAR.  On exit, only the nodes that received a PHI node
   for VAR will be present in PHI_INSERTION_POINTS.  */

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
  gcc_assert (def_map);

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
  sym = DECL_P (var) ? var : SSA_NAME_VAR (var);

  /* And insert the PHI nodes.  */
  EXECUTE_IF_AND_IN_BITMAP (phi_insertion_points, def_map->livein_blocks,
			    0, bb_index, bi)
    {
      bb = BASIC_BLOCK (bb_index);
      phi = create_phi_node (sym, bb);

      /* If we are rewriting SSA names, also add PHI arguments.  */
      if (update_p)
	{
	  edge_iterator ei;
	  FOR_EACH_EDGE (e, ei, bb->preds)
	    add_phi_arg (phi, var, e);

	  /* If updating the SSA form, mark the LHS of the new PHI
	     node to be a replacement for VAR.  Uses of VAR downstream
	     from this PHI node will be replaced with the new name
	     created by it.  */
	  if (update_p)
	    add_new_name_mapping (PHI_RESULT (phi), var);
	}

      /* Mark this PHI node as interesting for the rename process.  */
      REGISTER_DEFS_IN_THIS_STMT (phi) = 1;
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
  tree currdef_var, avar;
  
  /* Lookup the current reaching definition for VAR.  */
  currdef_var = get_current_def (var);

  /* If there is no reaching definition for VAR, create and register a
     default definition for it (if needed).  */
  if (currdef_var == NULL_TREE)
    {
      avar = DECL_P (var) ? var : SSA_NAME_VAR (var);
      currdef_var = get_default_def_for (avar);
      set_current_def (var, currdef_var);
    }

  /* Return the current reaching definition for VAR, or the default
     definition, if we had to create one.  */
  return currdef_var;
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
  BITMAP_FREE (entry->def_blocks);
  BITMAP_FREE (entry->phi_blocks);
  BITMAP_FREE (entry->livein_blocks);
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


/* Register NEW_NAME to be the new reaching definition for OLD_NAME.  */

static inline void
register_new_update_single (tree new_name, tree old_name)
{
  tree currdef = get_current_def (old_name);

  /* Push the current reaching definition into *BLOCK_DEFS_P.
     This stack is later used by the dominator tree callbacks to
     restore the reaching definitions for all the variables
     defined in the block after a recursive visit to all its
     immediately dominated blocks.  */
  VEC_safe_push (tree_on_heap, block_defs_stack, currdef);
  VEC_safe_push (tree_on_heap, block_defs_stack, old_name);

  /* Set the current reaching definition for OLD_NAME to be
     NEW_NAME.  */
  set_current_def (old_name, new_name);
}


/* Register NEW_NAME to be the new reaching definition for all the
   names in OLD_NAMES.  Used by the incremental SSA update routines to
   replace old SSA names with new ones.  */

static inline void
register_new_update_set (tree new_name, bitmap old_names)
{
  bitmap_iterator bi;
  unsigned i;

  EXECUTE_IF_SET_IN_BITMAP (old_names, 0, i, bi)
    register_new_update_single (new_name, ssa_name (i));
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
    fprintf (dump_file, "\n\nRegistering new PHI nodes in block #%d\n\n",
	     bb->index);

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
     register it as a new definition for its corresponding name.  Also
     register definitions for names whose underlying symbols are
     marked for renaming.  */
  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      tree lhs;

      if (!REGISTER_DEFS_IN_THIS_STMT (phi))
	continue;
      
      lhs = PHI_RESULT (phi);
      if (need_to_update_vops_p && symbol_marked_for_renaming (lhs))
	register_new_update_single (lhs, SSA_NAME_VAR (lhs));
      else
	{
	  /* If LHS is a new name, register a new definition for all
	     the names replaced by LHS.  */
	  if (TEST_BIT (new_ssa_names, SSA_NAME_VERSION (lhs)))
	    register_new_update_set (lhs, names_replaced_by (lhs));
	  
	  /* If LHS is an OLD name, register it as a new definition
	     for itself.  */
	  if (TEST_BIT (old_ssa_names, SSA_NAME_VERSION (lhs)))
	    register_new_update_single (lhs, lhs);
	}

      if (is_abnormal_phi)
	SSA_NAME_OCCURS_IN_ABNORMAL_PHI (lhs) = 1;
    }
}


/* Called after visiting block BB.  Unwind BLOCK_DEFS_STACK to restore
   the current reaching definition of every name re-written in BB to
   the original reaching definition before visiting BB.  This
   unwinding must be done in the opposite order to what is done in
   register_new_update_set.  */

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

  /* Only update marked statements.  */
  if (!REWRITE_THIS_STMT (stmt) && !REGISTER_DEFS_IN_THIS_STMT (stmt))
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Updating SSA information for statement ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  get_stmt_operands (stmt);

  /* Rewrite USES included in OLD_SSA_NAMES and USES whose underlying
     symbol is marked for renaming.  */
  if (REWRITE_THIS_STMT (stmt))
    {
      FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter, SSA_OP_USE)
	{
	  tree use = USE_FROM_PTR (use_p);
	  if (TEST_BIT (old_ssa_names, SSA_NAME_VERSION (use)))
	    SET_USE (use_p, get_reaching_def (use));
	}

      if (need_to_update_vops_p)
	FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter,
				  SSA_OP_VIRTUAL_USES | SSA_OP_VIRTUAL_KILLS)
	  {
	    tree use = USE_FROM_PTR (use_p);
	    if (symbol_marked_for_renaming (use))
	      {
		tree sym = DECL_P (use) ? use : SSA_NAME_VAR (use);
		SET_USE (use_p, get_reaching_def (sym));
	      }
	  }
    }

  /* Register definitions of names in NEW_SSA_NAMES and OLD_SSA_NAMES.
     Also register definitions for names whose underlying symbol is
     marked for renaming.  */
  if (REGISTER_DEFS_IN_THIS_STMT (stmt))
    {
      FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_DEF)
	{
	  tree def = DEF_FROM_PTR (def_p);

	  /* If DEF is a new name, register it as a new definition for
	     all the names replaced by DEF.  */
	  if (TEST_BIT (new_ssa_names, SSA_NAME_VERSION (def)))
	    register_new_update_set (def, names_replaced_by (def));

	  /* If DEF is an old name, register DEF as a new definition
	     for itself.  */
	  if (TEST_BIT (old_ssa_names, SSA_NAME_VERSION (def)))
	    register_new_update_single (def, def);
	}

      if (need_to_update_vops_p)
	FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_VIRTUAL_DEFS)
	  {
	    tree def = DEF_FROM_PTR (def_p);
	    if (symbol_marked_for_renaming (def))
	      {
		tree sym;

		if (DECL_P (def))
		  {
		    sym = def;
		    def = make_ssa_name (def, stmt);
		    SET_DEF (def_p, def);
		  }
		else
		  sym = SSA_NAME_VAR (def);

		register_new_update_single (def, sym);
	      }
	  }
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

	  if (arg && !DECL_P (arg) && TREE_CODE (arg) != SSA_NAME)
	    continue;

	  if (arg == NULL_TREE || symbol_marked_for_renaming (arg))
	    {
	      /* When updating a PHI node for a virtual variable, we
		 may find NULL arguments that's why we take the symbol
		 from the LHS of the PHI node.  */
	      tree sym = SSA_NAME_VAR (PHI_RESULT (phi));
	      gcc_assert (!is_gimple_reg (PHI_RESULT (phi)));
	      arg = get_reaching_def (sym);
	      SET_USE (arg_p, arg);
	    }
	  else if (is_gimple_reg (arg)
	           && TEST_BIT (old_ssa_names, SSA_NAME_VERSION (arg)))
	    SET_USE (arg_p, get_reaching_def (arg));

	  if (e->flags & EDGE_ABNORMAL)
	    SSA_NAME_OCCURS_IN_ABNORMAL_PHI (USE_FROM_PTR (arg_p)) = 1;
	}
    }
}


/* Rewrite the actual blocks, statements, and PHI arguments, to be in SSA
   form.  

   ENTRY indicates the block where to start.  Every block dominated by
      ENTRY will be rewritten.

   WHAT indicates what actions will be taken by the renamer (see enum
      rewrite_mode).

   BLOCKS are the set of interesting blocks for the dominator walker
      to process.  If this set is NULL, then all the nodes dominated
      by ENTRY are walked.  Otherwise, blocks dominated by ENTRY that
      are not present in BLOCKS are ignored.  */

static void
rewrite_blocks (basic_block entry, enum rewrite_mode what, sbitmap blocks)
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
  walk_data.interesting_blocks = blocks;

  block_defs_stack = VEC_alloc (tree_on_heap, 10);

  /* Initialize the dominator walker.  */
  init_walk_dominator_tree (&walk_data);

  /* Recursively walk the dominator tree rewriting each statement in
     each basic block.  */
  walk_dominator_tree (&walk_data, entry);

  /* Finalize the dominator walker.  */
  fini_walk_dominator_tree (&walk_data);

  /* Debugging dumps.  */
  if (dump_file && (dump_flags & TDF_STATS))
    {
      dump_dfa_stats (dump_file);
      if (def_blocks)
	dump_tree_ssa_stats (dump_file);
    }

  if (def_blocks)
    {
      htab_delete (def_blocks);
      def_blocks = NULL;
    }
  
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


/* Mark the definition site blocks for each variable, so that we know
   where the variable is actually live.

   INTERESTING_BLOCKS will be filled in with all the blocks that
      should be processed by the renamer.  It is assumed to be
      initialized and zeroed by the caller.  */

static void
mark_def_site_blocks (sbitmap interesting_blocks)
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
  walk_data.interesting_blocks = NULL;

  /* Notice that this bitmap is indexed using variable UIDs, so it must be
     large enough to accommodate all the variables referenced in the
     function, not just the ones we are renaming.  */
  mark_def_sites_global_data.kills = BITMAP_ALLOC (NULL);

  /* Create the set of interesting blocks that will be filled by
     mark_def_sites.  */
  mark_def_sites_global_data.interesting_blocks = interesting_blocks;
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
  BITMAP_FREE (mark_def_sites_global_data.kills);
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
  sbitmap interesting_blocks;
  
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

  /* Initialize the set of interesting blocks.  The callback
     mark_def_sites will add to this set those blocks that the renamer
     should process.  */
  interesting_blocks = sbitmap_alloc (last_basic_block);
  sbitmap_zero (interesting_blocks);

  mark_def_site_blocks (interesting_blocks);

  /* Initialize dominance frontier and immediate dominator bitmaps. 
     Also count the number of predecessors for each block.  Doing so
     can save significant time during PHI insertion for large graphs.  */
  dfs = (bitmap *) xmalloc (last_basic_block * sizeof (bitmap *));
  FOR_EACH_BB (bb)
    dfs[bb->index] = BITMAP_ALLOC (NULL);

  /* Compute dominance frontiers.  */
  compute_dominance_frontiers (dfs);

  /* Insert PHI nodes at dominance frontiers of definition blocks.  */
  insert_phi_nodes (dfs, NULL);

  rewrite_blocks (ENTRY_BLOCK_PTR, REWRITE_ALL, interesting_blocks);

  /* Free allocated memory.  */
  FOR_EACH_BB (bb)
    BITMAP_FREE (dfs[bb->index]);
  free (dfs);
  sbitmap_free (interesting_blocks);

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
  sbitmap interesting_blocks;

  /* Initialize the set of interesting blocks.  The callback
     mark_def_sites will add to this set those blocks that the renamer
     should process.  */
  interesting_blocks = sbitmap_alloc (last_basic_block);
  sbitmap_zero (interesting_blocks);

  mark_def_site_blocks (interesting_blocks);
  rewrite_blocks (ENTRY_BLOCK_PTR, REWRITE_FIX_DEF_DEF_CHAINS,
		  interesting_blocks);

  sbitmap_free (interesting_blocks);
}


/* Mark the definition of NAME at STMT and BB as interesting for the
   renamer.  BLOCKS is the set of blocks that need updating.  */

static void
mark_def_interesting (tree name, tree stmt, basic_block bb, bitmap blocks,
		      bool insert_phi_p)
{
  REGISTER_DEFS_IN_THIS_STMT (stmt) = 1;
  bitmap_set_bit (blocks, bb->index);

  if (insert_phi_p)
    {
      bool is_phi_p = TREE_CODE (stmt) == PHI_NODE;

      set_def_block (name, bb, is_phi_p);

      /* If NAME is an SSA name in NEW_SSA_NAMES, this is a definition
	 site for both itself and all the old names replaced by it.  */
      if (TREE_CODE (name) == SSA_NAME
	  && TEST_BIT (new_ssa_names, SSA_NAME_VERSION (name)))
	{
	  bitmap_iterator bi;
	  unsigned i;
	  bitmap set = names_replaced_by (name);
	  if (set)
	    EXECUTE_IF_SET_IN_BITMAP (set, 0, i, bi)
	      set_def_block (ssa_name (i), bb, is_phi_p);
	}
    }
}


/* Mark the use of NAME at STMT and BB as interesting for the
   renamer.  INSERT_PHI_P is true if we are going to insert new PHI
   nodes.  BLOCKS is the set of blocks that need updating.  */

static inline void
mark_use_interesting (tree name, tree stmt, basic_block bb, bitmap blocks,
		      bool insert_phi_p)
{
  REWRITE_THIS_STMT (stmt) = 1;
  bitmap_set_bit (blocks, bb->index);

  /* If NAME has not been defined in BB, then it is live-on-entry
     to BB.  Note that we cannot just use the block holding NAME's
     definition because if NAME is one of the names in OLD_SSA_NAMES,
     it will have several definitions (itself and all the names that
     replace it).  */
  if (insert_phi_p)
    {
      struct def_blocks_d *db_p = get_def_blocks_for (name);
      if (!bitmap_bit_p (db_p->def_blocks, bb->index))
	set_livein_block (name, bb);
    }
}


/* If any of the arguments is in OLD_SSA_NAMES, mark PHI to
   be rewritten.  FIXME, explain carefully.  */

static void
prepare_phi_args_for_update (tree phi, basic_block bb, bitmap blocks,
                             bool insert_phi_p)
{
  int i;
  tree arg0, lhs = PHI_RESULT (phi);
  bool all_args_equal_p = true;

  arg0 = PHI_ARG_DEF (phi, 0);
  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      tree arg = PHI_ARG_DEF (phi, i);

      if (arg == NULL)
	{
	  gcc_unreachable ();

	  /* We can only add new arguments to a PHI node if LHS has
	     already been registered in the replacement table.  If LHS
	     is an old name, we use it as the argument.  Otherwise, we
	     pick any old name replaced by LHS and use it (the
	     rewriting process will fill in the correct one later on).  */
	  if (TEST_BIT (new_ssa_names, SSA_NAME_VERSION (lhs)))
	    arg = ssa_name (bitmap_first_set_bit (names_replaced_by (lhs)));
	  else if (TEST_BIT (old_ssa_names, SSA_NAME_VERSION (lhs)))
	    arg = lhs;
	  else
	    gcc_unreachable ();

	  SET_PHI_ARG_DEF (phi, i, arg);
	  if (arg0 == NULL)
	    arg0 = arg;
	}

      if (arg != arg0 || TREE_CODE (arg) != SSA_NAME)
	all_args_equal_p = false;

      if (TREE_CODE (arg) == SSA_NAME
	  && TEST_BIT (old_ssa_names, SSA_NAME_VERSION (arg)))
	{
	  /* We cannot just call mark_use_interesting here.
	     FIXME, explain why.  */
	  REWRITE_THIS_STMT (phi) = 1;
	  bitmap_set_bit (blocks, bb->index);

	  /* FIXME, this is not intuitive, add explanation.  */
	  if (insert_phi_p)
	    {
	      edge e;
	      struct def_blocks_d *db;

	      e = PHI_ARG_EDGE (phi, i);
	      db = get_def_blocks_for (arg);
	      if (!bitmap_bit_p (db->def_blocks, e->src->index))
		set_livein_block (arg, e->src);
	    }
	}
    }
}


/* Do a dominator walk starting at BB processing statements that
   reference variables in OLD_SSA_NAMES and NEW_SSA_NAMES.

   1- Mark in BLOCKS the defining block of every name N in
      NEW_SSA_NAMES.

   2- Mark in BLOCKS the defining block of every name O in
      OLD_SSA_NAMES.

   3- For every statement or PHI node that uses a name O in
      OLD_SSA_NAMES.  If INSERT_PHI_P is true, mark those uses as live
      in the corresponding block.  This is later used by the PHI
      placement algorithm to make PHI pruning decisions.

   If VISIT_DOM_P is true, all the dominator children of BB are also
   visited.

   FIXME.  This process is slower than necessary.  Once we have
   immediate uses merged in, we should be able to just visit the
   immediate uses of all the names that we are about to replace,
   instead of visiting the whole block.  */

static void
prepare_block_for_update (basic_block bb, bool insert_phi_p,
                          bitmap blocks, bool visit_dom_p)
{
  basic_block son;
  block_stmt_iterator si;
  tree phi;
  bool scan_vops_p = need_to_update_vops_p;

  /* Process PHI nodes marking interesting those that define or use
     the names that we are interested in.  */
  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      tree lhs = PHI_RESULT (phi);

      REWRITE_THIS_STMT (phi) = 0;
      REGISTER_DEFS_IN_THIS_STMT (phi) = 0;

      /* Ignore virtual PHIs if we are not updating virtual operands.  */
      if (!scan_vops_p && !is_gimple_reg (lhs))
	continue;

      if (scan_vops_p && symbol_marked_for_renaming (lhs))
	{
	  /* If the LHS is a virtual symbol marked for renaming, then
	     we don't need to scan the argument list.  Since virtual
	     operands are in FUD-chain form, all the arguments of this
	     PHI must be the same symbol as the LHS.  So, we just need
	     to mark this site as both an interesting use and an
	     interesting def for the symbol.  */
	  tree sym = SSA_NAME_VAR (lhs);
	  mark_use_interesting (sym, phi, bb, blocks, true);
	  mark_def_interesting (sym, phi, bb, blocks, true);
	}
      else
	{
	  /* If the LHS is in OLD_SSA_NAMES or NEW_SSA_NAMES, this is
	     a definition site for it.  */
	  if (TEST_BIT (old_ssa_names, SSA_NAME_VERSION (lhs))
	      || TEST_BIT (new_ssa_names, SSA_NAME_VERSION (lhs)))
	    mark_def_interesting (lhs, phi, bb, blocks, insert_phi_p);

	  prepare_phi_args_for_update (phi, bb, blocks, insert_phi_p);
	}
    }

  /* Process the statements.  */
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      tree stmt;
      ssa_op_iter i;
      use_operand_p use_p;
      def_operand_p def_p;
      
      stmt = bsi_stmt (si);
      get_stmt_operands (stmt);

      REWRITE_THIS_STMT (stmt) = 0;
      REGISTER_DEFS_IN_THIS_STMT (stmt) = 0;

      FOR_EACH_SSA_USE_OPERAND (use_p, stmt, i, SSA_OP_USE)
	{
	  tree use = USE_FROM_PTR (use_p);
	  if (TEST_BIT (old_ssa_names, SSA_NAME_VERSION (use)))
	    mark_use_interesting (use, stmt, bb, blocks, insert_phi_p);
	}

      FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, i, SSA_OP_DEF)
	{
	  tree def = DEF_FROM_PTR (def_p);
	  if (TEST_BIT (new_ssa_names, SSA_NAME_VERSION (def))
	      || TEST_BIT (old_ssa_names, SSA_NAME_VERSION (def)))
	    mark_def_interesting (def, stmt, bb, blocks, insert_phi_p);
	}

      /* If we don't need to update virtual operands, continue to the
	 next statement.  */
      if (!scan_vops_p)
	continue;

      /* For every interesting N_i = V_MAY_DEF <N_j> and
	 N_i = V_MUST_DEF <N_j>, mark the statement as interesting.
	 Notice that N_j may in fact be a naked symbol (if this
	 statement is the result of basic block duplication). The
	 rename process will later fill in the appropriate reaching
	 definition for the symbol.  */
      FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, i, SSA_OP_VIRTUAL_DEFS)
	{
	  tree def = DEF_FROM_PTR (def_p);

	  if (symbol_marked_for_renaming (def))
	    {
	      tree sym = DECL_P (def) ? def : SSA_NAME_VAR (def);
	      mark_use_interesting (sym, stmt, bb, blocks, true);
	      mark_def_interesting (sym, stmt, bb, blocks, true);
	    }
	}

      /* Similarly, for V_USE <N_i>.  */
      FOR_EACH_SSA_USE_OPERAND (use_p, stmt, i, SSA_OP_VUSE)
	{
	  tree use = USE_FROM_PTR (use_p);
	  if (symbol_marked_for_renaming (use))
	    {
	      tree sym = DECL_P (use) ? use : SSA_NAME_VAR (use);
	      mark_use_interesting (sym, stmt, bb, blocks, true);
	    }
	}
    }

  /* Now visit all the blocks reached by BB.  Skip ENTRY_BLOCK_PTR and
     EXIT_BLOCK_PTR because they have nothing of interest.  */
  if (visit_dom_p)
    for (son = first_dom_son (CDI_DOMINATORS, bb);
	 son;
	 son = next_dom_son (CDI_DOMINATORS, son))
      prepare_block_for_update (son, insert_phi_p, blocks, true);
}


/* Remove SSA name with version number VER from the internal tables used by
   update_ssa.  */

static void
remove_name_from_mappings (unsigned ver)
{
  unsigned i;

  if (TEST_BIT (old_ssa_names, ver))
    {
      /* If VER was an old name, remove it from every replacement set
	 of names in NEW_SSA_NAMEs.  If a replacement set becomes
	 empty, then also remove the new name from NEW_SSA_NAMES.  */
      EXECUTE_IF_SET_IN_SBITMAP (new_ssa_names, 0, i,
	{
	  tree new = ssa_name (i);
	  if (new)
	    {
	      bitmap repl_set = names_replaced_by (new);
	      bitmap_clear_bit (repl_set, ver);
	      if (bitmap_empty_p (repl_set))
		RESET_BIT (new_ssa_names, i);
	    }
	  else
	    RESET_BIT (new_ssa_names, i);
	});

      RESET_BIT (old_ssa_names, ver);
    }

  if (TEST_BIT (new_ssa_names, ver))
    {
      /* Note, we could also remove NAME from REPL_TBL, but it's not
	 really necessary.  All the replacements are driven by names
	 in OLD_SSA_NAMES exclusively.  */
      RESET_BIT (new_ssa_names, ver);
    }
}


/* Helper for prepare_def_sites.  VER is the version number for the
   SSA name to process.  BLOCKS and INSERT_PHI_P are as in
   prepare_def_sites.  */

static void
prepare_def_site_for (unsigned ver, bitmap blocks, bool insert_phi_p)
{
  tree name, stmt;
  basic_block bb;

  name = ssa_name (ver);

  /* If NAME has been removed after the caller registered the
     replacement, remove it from NEW_SSA_NAMES and OLD_SSA_NAMES to
     avoid processing it any further.  */
  if (name == NULL_TREE || SSA_NAME_IN_FREE_LIST (name))
    {
      remove_name_from_mappings (ver);
      return;
    }

  stmt = SSA_NAME_DEF_STMT (name);
  bb = bb_for_stmt (stmt);
  if (bb)
    mark_def_interesting (name, stmt, bb, blocks, insert_phi_p);
}


/* Mark definition sites of names in NEW_SSA_NAMES and OLD_SSA_NAMES.
   Add each definition block to BLOCKS.  INSERT_PHI_P is true if the
   caller wants to insert PHI nodes for newly created names.  */

static void
prepare_def_sites (bitmap blocks, bool insert_phi_p)
{
  unsigned i;

  EXECUTE_IF_SET_IN_SBITMAP (old_ssa_names, 0, i,
    prepare_def_site_for (i, blocks, insert_phi_p));

  EXECUTE_IF_SET_IN_SBITMAP (new_ssa_names, 0, i,
    prepare_def_site_for (i, blocks, insert_phi_p));
}


/* Dump all names replaced by SSA name version VER to FILE.  */

void
dump_names_replaced_by (FILE *file, unsigned ver)
{
  tree name = ssa_name (ver);

  if (name)
    print_generic_expr (file, name, 0);
  else
    {
      fprintf (file, "<REMOVED>_%u\n", ver);
      return;
    }

  fprintf (file, " -> { ");

  if (!is_gimple_reg (name))
    {
      print_generic_expr (file, SSA_NAME_VAR (name), 0);
      fprintf (file, " ");
    }
  else
    {
      unsigned i;
      bitmap old_set;
      bitmap_iterator bi;

      old_set = names_replaced_by (name);
      EXECUTE_IF_SET_IN_BITMAP (old_set, 0, i, bi)
	{
	  tree old = ssa_name (i);
	  if (old)
	    print_generic_expr (file, ssa_name (i), 0);
	  else
	    fprintf (file, "<REMOVED>_%u", i);
	  fprintf (file, " ");
	}
    }

  fprintf (file, "}\n");
}


/* Dump all the new replacements for SSA name version VER to stderr.  */

void
debug_names_replaced_by (unsigned ver)
{
  dump_names_replaced_by (stderr, ver);
}


/* Dump the SSA name replacement table to FILE.  */

void
dump_repl_tbl (FILE *file)
{
  unsigned i;

  if (!need_ssa_update_p ())
    return;

  fprintf (file, "\nSSA replacement table\n");
  fprintf (file, "N_i -> { O_1 ... O_j } means that N_i replaces O_1, ..., O_j\n");
  fprintf (file, "N_i -> { SYM } means that N_i is a new name for virtual variable SYM\n\n");

  EXECUTE_IF_SET_IN_SBITMAP (new_ssa_names, 0, i,
    dump_names_replaced_by (file, i));

  fprintf (file, "\n");
}


/* Dump the SSA name replacement table to stderr.  */

void
debug_repl_tbl (void)
{
  dump_repl_tbl (stderr);
}


/* Initialize data structures used for incremental SSA updates.  */

static void
init_update_ssa (void)
{
  /* Reserve 1/3 more than the current number of names.  The calls to
     add_new_name_mapping are typically done after creating new SSA
     names, so we'll need to reallocate these arrays.  */
  old_ssa_names = sbitmap_alloc (num_ssa_names + NAME_SETS_GROWTH_FACTOR);
  sbitmap_zero (old_ssa_names);

  new_ssa_names = sbitmap_alloc (num_ssa_names + NAME_SETS_GROWTH_FACTOR);
  sbitmap_zero (new_ssa_names);

  repl_tbl = htab_create (20, repl_map_hash, repl_map_eq, repl_map_free);
  need_to_initialize_update_ssa_p = false;
  need_to_update_vops_p = false;
}


/* Deallocate data structures used for incremental SSA updates.  */

static void
delete_update_ssa (void)
{
  sbitmap_free (old_ssa_names);
  old_ssa_names = NULL;

  sbitmap_free (new_ssa_names);
  new_ssa_names = NULL;

  htab_delete (repl_tbl);
  repl_tbl = NULL;

  need_to_initialize_update_ssa_p = true;
  need_to_update_vops_p = false;
  bitmap_clear (vars_to_rename);
}


/* Create a new name for OLD_NAME in statement STMT and replace the
   operand pointed to by DEF_P with the newly created name.  Return
   the new name and register the replacement mapping <NEW, OLD> in
   update_ssa's tables.  */

tree
create_new_def_for (tree old_name, tree stmt, def_operand_p def)
{
  tree new_name = duplicate_ssa_name (old_name, stmt);

  SET_DEF (def, new_name);

  if (TREE_CODE (stmt) == PHI_NODE)
    {
      edge e;
      edge_iterator ei;
      basic_block bb = bb_for_stmt (stmt);

      /* If needed, mark NEW_NAME as occurring in an abnormal PHI node. */
      FOR_EACH_EDGE (e, ei, bb->preds)
	if (e->flags & EDGE_ABNORMAL)
	  {
	    SSA_NAME_OCCURS_IN_ABNORMAL_PHI (new_name) = 1;
	    break;
	  }
    }

  register_new_name_mapping (new_name, old_name);

  return new_name;
}


/* Register name NEW to be a replacement for name OLD.  This function
   must be called for every replacement that should be performed by
   update_ssa.  */

void
register_new_name_mapping (tree new, tree old)
{
  if (need_to_initialize_update_ssa_p)
    init_update_ssa ();

  add_new_name_mapping (new, old);
}


/* Return true if there are any SSA name tuples registered in the
   replacement table.  */

bool
need_ssa_update_p (void)
{
  return new_ssa_names && sbitmap_first_set_bit (new_ssa_names) >= 0;
}


/* Return true if name N has been registered in the replacement table.  */

bool
name_registered_for_update_p (tree n)
{
  unsigned ver = SSA_NAME_VERSION (n);

  if (new_ssa_names == NULL || ver >= new_ssa_names->n_bits)
    return false;

  return TEST_BIT (new_ssa_names, ver)
         || TEST_BIT (old_ssa_names, ver)
	 || symbol_marked_for_renaming (n);
}


/* Insert new PHI nodes to replace VAR.  DFS contains dominance
   frontier information.  BLOCKS is the set of blocks to be updated.

   This is slightly different than the regular PHI insertion
   algorithm.  In this case, we are only interested in PHI nodes
   inside the region affected by the block that defines VAR and the
   blocks that define all its replacements.  All these definition
   blocks have been gathered by prepare_block_for_update and they are
   stored in DEF_BLOCKS[VAR]->DEF_BLOCKS.

   First, we compute the entry point to the region (ENTRY).  This is
   given by the nearest common dominator to all the definition blocks.
   When computing the iterated dominance frontier (IDF), any block not
   strictly dominated by ENTRY is ignored.

   We then call the standard PHI insertion algorithm
   (insert_phi_nodes_for) with the pruned IDF.  */

static void
insert_updated_phi_nodes_for (tree var, bitmap *dfs, bitmap blocks)
{
  basic_block entry;
  struct def_blocks_d *db;
  bitmap idf, pruned_idf;
  bitmap_iterator bi;
  unsigned i;

#if defined ENABLE_CHECKING
  if (TREE_CODE (var) == SSA_NAME)
    gcc_assert (TEST_BIT (old_ssa_names, SSA_NAME_VERSION (var)));
  else
    gcc_assert (bitmap_bit_p (vars_to_rename, var_ann (var)->uid));
#endif

  /* Get all the definition sites for VAR.  */
  db = find_def_blocks_for (var);

  /* No need to do anything if there were no definitions to VAR.  */
  if (db == NULL || bitmap_empty_p (db->def_blocks))
    return;

  /* Compute the entry block to the region.  */
  entry = nearest_common_dominator_for_set (CDI_DOMINATORS, db->def_blocks);
  
  /* Compute the initial iterated dominance frontier.  */
  idf = find_idf (db->def_blocks, dfs);

  /* We are only interested in IDF blocks dominated by ENTRY.  */
  pruned_idf = BITMAP_ALLOC (NULL);
  EXECUTE_IF_SET_IN_BITMAP (idf, 0, i, bi)
    if (BASIC_BLOCK (i) != entry
	&& dominated_by_p (CDI_DOMINATORS, BASIC_BLOCK (i), entry))
      bitmap_set_bit (pruned_idf, i);

  if (!bitmap_empty_p (pruned_idf))
    {
      /* Make sure that PRUNED_IDF blocks and all their feeding blocks
	 are included in the region to be updated.  The feeding blocks
	 are important to guarantee that the PHI arguments are renamed
	 properly.  */
      bitmap_ior_into (blocks, pruned_idf);
      EXECUTE_IF_SET_IN_BITMAP (pruned_idf, 0, i, bi)
	{
	  edge e;
	  edge_iterator ei;
	  basic_block bb = BASIC_BLOCK (i);

	  FOR_EACH_EDGE (e, ei, bb->preds)
	    bitmap_set_bit (blocks, e->src->index);
	}

      insert_phi_nodes_for (var, pruned_idf, true);
    }

  BITMAP_FREE (pruned_idf);
  BITMAP_FREE (idf);
}


/* Given a set of newly created SSA names (NEW_NAMES) and a set of
   existing SSA names (OLD_NAMES), update the SSA form so that:

   1- The names in OLD_NAMES dominated by the definitions of
      NEW_NAMES are all re-written to be reached by the
      appropriate definition from NEW_NAMES.

   2- If needed, new PHI nodes are added to the iterated dominance
      frontier of the blocks where each of NEW_NAMES are defined.

   The mapping between OLD_NAMES and NEW_NAMES is setup by calling
   register_new_name_mapping for every pair of names that the caller
   wants to replace.

   The caller identifies the new names that have been inserted and the
   names that need to be replaced by calling register_new_name_mapping
   for every pair <NEW, OLD>.  Note that the function assumes that the
   new names have already been inserted in the IL.

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

   Additionally, we may need to insert a PHI node at line 11 because
   that is a merge point for x_10 and x_11.  So the use of x_1 at line
   11 will be replaced with the new PHI node.  The insertion of PHI
   nodes is optional.  They are not strictly necessary to preserve the
   SSA form, and depending on what the caller inserted, they may not
   even be useful for the optimizers.  If INSERT_PHI_P is true, then
   PHI nodes are inserted as necessary, otherwise they are not.  */

void
update_ssa (bool insert_phi_p)
{
  bitmap *dfs, blocks;
  basic_block bb, start_bb;
  bitmap_iterator bi;
  unsigned i;
  sbitmap tmp;

  if (!need_ssa_update_p ())
    return;

  timevar_push (TV_TREE_SSA_INCREMENTAL);

  /* Ensure that the dominance information is up-to-date.  */
  calculate_dominance_info (CDI_DOMINATORS);

  if (insert_phi_p)
    {
      /* If the caller requested PHI nodes to be added, compute
	 dominance frontiers and initialize live-in information data
	 structures (DEF_BLOCKS).  */
      dfs = (bitmap *) xmalloc (last_basic_block * sizeof (bitmap *));
      FOR_EACH_BB (bb)
	dfs[bb->index] = BITMAP_ALLOC (NULL);
      compute_dominance_frontiers (dfs);

      /* For each SSA name N, the DEF_BLOCKS table describes where the
	 name is defined, which blocks have PHI nodes for N, and which
	 blocks have uses of N (i.e., N is live-on-entry in those
	 blocks).  */
      def_blocks = htab_create (num_ssa_names, def_blocks_hash,
				def_blocks_eq, def_blocks_free);
    }
  else
    {
      dfs = NULL;
      def_blocks = NULL;
    }

  blocks = BITMAP_ALLOC (NULL);

  /* Determine the CFG region that we are going to update.  First add
     all the blocks that define each of the names in NEW_SSA_NAMES
     and OLD_SSA_NAMES.  */
  prepare_def_sites (blocks, insert_phi_p);

  /* Next, determine the nearest common dominator START_BB for all the
     blocks in the region.  */
  if (need_to_update_vops_p || bitmap_empty_p (blocks))
    {
      /* If the region to update is seemingly empty, it means that all
	 we found were virtual uses for which we now need to
	 reconstruct their FUD chains.  For now just start at the top
	 of the CFG.  

	 A similar problem occurs when we need to update virtual
	 operands.
	 
	 FIXME, it would be more efficient to determine the nearest
	 dominator that had a definition for the symbols we are
	 updating (for that, mark_sym_for_renaming should also
	 register where the VUSE was found).  */
      start_bb = ENTRY_BLOCK_PTR;
    }
  else
    start_bb = nearest_common_dominator_for_set (CDI_DOMINATORS, blocks);

  /* Traverse all the blocks dominated by START_BB.  Mark interesting
     blocks and statements and set local live-in information for the
     PHI placement heuristics.  */
  prepare_block_for_update (start_bb, insert_phi_p, blocks, true);

  /* If are going to insert PHI nodes, blocks in the dominance
     frontier of START_BB may be affected.  Note that we don't need to
     visit the dominator children of blocks in the dominance frontier
     of START_BB.  None of the changes inside this region can affect
     blocks on the outside.  */
  if (insert_phi_p && start_bb->index >= 0)
    EXECUTE_IF_SET_IN_BITMAP (dfs[start_bb->index], 0, i, bi)
      prepare_block_for_update (BASIC_BLOCK (i), insert_phi_p,
				blocks, false);

  /* If requested, insert PHI nodes at the iterated dominance frontier
     of every block making new definitions for names in OLD_SSA_NAMES
     and for symbols in VARS_TO_RENAME.  */
  if (insert_phi_p)
    {
      /* insert_update_phi_nodes_for will call add_new_name_mapping
	 when inserting new PHI nodes, so the set OLD_SSA_NAMES will
	 grow while we are traversing it (but it will not gain any new
	 members).  Copy OLD_SSA_NAMES to a temporary for traversal.  */
      sbitmap tmp = sbitmap_alloc (old_ssa_names->n_bits);
      sbitmap_copy (tmp, old_ssa_names);
      EXECUTE_IF_SET_IN_SBITMAP (tmp, 0, i,
	insert_updated_phi_nodes_for (ssa_name (i), dfs, blocks));
      sbitmap_free (tmp);

      if (need_to_update_vops_p)
	EXECUTE_IF_SET_IN_BITMAP (vars_to_rename, 0, i, bi)
	  insert_updated_phi_nodes_for (referenced_var (i), dfs,
					blocks);

      /* Insertion of PHI nodes may have added blocks to the region.
	 We need to re-compute START_BB to include the newly added
	 blocks.  */
      if (start_bb != ENTRY_BLOCK_PTR)
	start_bb = nearest_common_dominator_for_set (CDI_DOMINATORS, blocks);
    }

  /* Reset the current definition for name and symbol before renaming
     the sub-graph.  */
  EXECUTE_IF_SET_IN_SBITMAP (old_ssa_names, 0, i,
    set_current_def (ssa_name (i), ssa_name (i)));

  if (need_to_update_vops_p)
    EXECUTE_IF_SET_IN_BITMAP (vars_to_rename, 0, i, bi)
      set_current_def (referenced_var (i), NULL_TREE);

  /* Now start the renaming process at START_BB.  */
  tmp = sbitmap_alloc (last_basic_block);
  sbitmap_zero (tmp);
  EXECUTE_IF_SET_IN_BITMAP (blocks, 0, i, bi)
    SET_BIT (tmp, i);

  rewrite_blocks (start_bb, REWRITE_UPDATE, tmp);

  sbitmap_free (tmp);

  /* Debugging dumps.  */
  if (dump_file)
    {
      int c;
      unsigned i;

      dump_repl_tbl (dump_file);

      fprintf (dump_file, "Incremental SSA update started at block: %d\n\n",
	       start_bb->index);

      c = 0;
      EXECUTE_IF_SET_IN_BITMAP (blocks, 0, i, bi)
	c++;
      fprintf (dump_file, "Number of blocks in CFG: %d\n", last_basic_block);
      fprintf (dump_file, "Number of blocks to update: %d (%3.0f%%)\n\n",
	       c, PERCENT (c, last_basic_block));

      if (dump_flags & TDF_DETAILS)
	{
	  fprintf (dump_file, "Affected blocks: ");
	  EXECUTE_IF_SET_IN_BITMAP (blocks, 0, i, bi)
	    fprintf (dump_file, "%u ", i);
	  fprintf (dump_file, "\n");
	}

      fprintf (dump_file, "\n\n");
    }

  /* Free allocated memory.  */
  if (insert_phi_p)
    {
      FOR_EACH_BB (bb)
	BITMAP_FREE (dfs[bb->index]);
      free (dfs);
    }

  BITMAP_FREE (blocks);
  delete_update_ssa ();

  timevar_pop (TV_TREE_SSA_INCREMENTAL);
}
