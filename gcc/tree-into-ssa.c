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
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

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
#include "params.h"
#include "vecprim.h"

/* This file builds the SSA form for a function as described in:
   R. Cytron, J. Ferrante, B. Rosen, M. Wegman, and K. Zadeck. Efficiently
   Computing Static Single Assignment Form and the Control Dependence
   Graph. ACM Transactions on Programming Languages and Systems,
   13(4):451-490, October 1991.  */

/* True if the code is in SSA form.  */
bool in_ssa_p;

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
   state after completing rewriting of a block and its dominator
   children.  Its elements have the following properties:

   - An SSA_NAME (N) indicates that the current definition of the
     underlying variable should be set to the given SSA_NAME.  If the
     symbol associated with the SSA_NAME is not a GIMPLE register, the
     next slot in the stack must be a _DECL node (SYM).  In this case,
     the name N in the previous slot is the current reaching
     definition for SYM.

   - A _DECL node indicates that the underlying variable has no
     current definition.

   - A NULL node at the top entry is used to mark the last slot
     associated with the current block.  */
static VEC(tree,heap) *block_defs_stack;

/* Set of existing SSA names being replaced by update_ssa.  */
static sbitmap old_ssa_names;

/* Set of new SSA names being added by update_ssa.  Note that both
   NEW_SSA_NAMES and OLD_SSA_NAMES are dense bitmaps because most of
   the operations done on them are presence tests.  */
static sbitmap new_ssa_names;

/* Symbols whose SSA form needs to be updated or created for the first
   time.  */
static bitmap syms_to_rename;

/* Set of SSA names that have been marked to be released after they
   were registered in the replacement table.  They will be finally
   released after we finish updating the SSA web.  */
static bitmap names_to_release;

/* Set of SSA names that have been marked stale by the SSA updater.
   This happens when the LHS of a VDEF operator needs a new SSA name
   (i.e., it used to be a .MEM factored store and got converted into a
   regular store).  When this occurs, other VDEF and VUSE operators
   using the original LHS must stop using it.
   See rewrite_update_stmt_vops.  */
static bitmap stale_ssa_names;

/* For each block, the PHI nodes that need to be rewritten are stored into
   these vectors.  */
typedef VEC(tree, heap) *tree_vec;
DEF_VEC_P (tree_vec);
DEF_VEC_ALLOC_P (tree_vec, heap);

static VEC(tree_vec, heap) *phis_to_rewrite;

/* The bitmap of non-NULL elements of PHIS_TO_REWRITE.  */
static bitmap blocks_with_phis_to_rewrite;

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

/* Statistics kept by update_ssa to use in the virtual mapping
   heuristic.  If the number of virtual mappings is beyond certain
   threshold, the updater will switch from using the mappings into
   renaming the virtual symbols from scratch.  In some cases, the
   large number of name mappings for virtual names causes significant
   slowdowns in the PHI insertion code.  */
struct update_ssa_stats_d
{
  unsigned num_virtual_mappings;
  unsigned num_total_mappings;
  bitmap virtual_symbols;
  unsigned num_virtual_symbols;
};
static struct update_ssa_stats_d update_ssa_stats;

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


/* Information stored for SSA names.  */
struct ssa_name_info
{
  /* The actual definition of the ssa name.  */
  tree current_def;

  /* This field indicates whether or not the variable may need PHI nodes.
     See the enum's definition for more detailed information about the
     states.  */
  ENUM_BITFIELD (need_phi_state) need_phi_state : 2;

  /* Age of this record (so that info_for_ssa_name table can be cleared
     quicky); if AGE < CURRENT_INFO_FOR_SSA_NAME_AGE, then the fields
     are assumed to be null.  */
  unsigned age;
};

/* The information associated with names.  */
typedef struct ssa_name_info *ssa_name_info_p;
DEF_VEC_P (ssa_name_info_p);
DEF_VEC_ALLOC_P (ssa_name_info_p, heap);

static VEC(ssa_name_info_p, heap) *info_for_ssa_name;
static unsigned current_info_for_ssa_name_age;

/* The set of blocks affected by update_ssa.  */
static bitmap blocks_to_update;

/* The main entry point to the SSA renamer (rewrite_blocks) may be
   called several times to do different, but related, tasks.
   Initially, we need it to rename the whole program into SSA form.
   At other times, we may need it to only rename into SSA newly
   exposed symbols.  Finally, we can also call it to incrementally fix
   an already built SSA web.  */
enum rewrite_mode {
    /* Convert the whole function into SSA form.  */
    REWRITE_ALL,

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

DEF_VEC_P(bitmap);
DEF_VEC_ALLOC_P(bitmap,heap);

/* Mapping between a statement and the symbols referenced by it.  */
struct mem_syms_map_d
{
  tree stmt;
  bitmap loaded;
  bitmap stored;
};

typedef struct mem_syms_map_d *mem_syms_map_t;

/* Table, indexed by statement, for symbols loaded and stored by
   statements that reference memory.  */
static htab_t mem_syms_tbl;

/* Prototypes for debugging functions.  */
extern void dump_tree_ssa (FILE *);
extern void debug_tree_ssa (void);
extern void debug_def_blocks (void);
extern void dump_tree_ssa_stats (FILE *);
extern void debug_tree_ssa_stats (void);
extern void dump_update_ssa (FILE *);
extern void debug_update_ssa (void);
extern void dump_names_replaced_by (FILE *, tree);
extern void debug_names_replaced_by (tree);
extern void dump_def_blocks (FILE *);
extern void debug_def_blocks (void);
extern void dump_defs_stack (FILE *, int);
extern void debug_defs_stack (int);
extern void dump_currdefs (FILE *);
extern void debug_currdefs (void);

/* Get the information associated with NAME.  */

static inline struct ssa_name_info *
get_ssa_name_ann (tree name)
{
  unsigned ver = SSA_NAME_VERSION (name);
  unsigned len = VEC_length (ssa_name_info_p, info_for_ssa_name);
  struct ssa_name_info *info;

  if (ver >= len)
    {
      unsigned new_len = num_ssa_names;

      VEC_reserve (ssa_name_info_p, heap, info_for_ssa_name, new_len);
      while (len++ < new_len)
	{
	  struct ssa_name_info *info = XCNEW (struct ssa_name_info);
	  info->age = current_info_for_ssa_name_age;
	  VEC_quick_push (ssa_name_info_p, info_for_ssa_name, info);
	}
    }

  info = VEC_index (ssa_name_info_p, info_for_ssa_name, ver);
  if (info->age < current_info_for_ssa_name_age)
    {
      info->need_phi_state = 0;
      info->current_def = NULL_TREE;
      info->age = current_info_for_ssa_name_age;
    }

  return info;
}

/* Clears info for ssa names.  */

static void
clear_ssa_name_info (void)
{
  current_info_for_ssa_name_age++;
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

tree
get_current_def (tree var)
{
  if (TREE_CODE (var) == SSA_NAME)
    return get_ssa_name_ann (var)->current_def;
  else
    return var_ann (var)->current_def;
}


/* Sets current definition of VAR to DEF.  */

void
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


/* Cleans up the REWRITE_THIS_STMT and REGISTER_DEFS_IN_THIS_STMT flags for
   all statements in basic block BB.  */

static void
initialize_flags_in_bb (basic_block bb)
{
  tree phi, stmt;
  block_stmt_iterator bsi;

  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      REWRITE_THIS_STMT (phi) = 0;
      REGISTER_DEFS_IN_THIS_STMT (phi) = 0;
    }

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      stmt = bsi_stmt (bsi);
      /* We are going to use the operand cache API, such as
	 SET_USE, SET_DEF, and FOR_EACH_IMM_USE_FAST.  The operand
	 cache for each statement should be up-to-date.  */
      gcc_assert (!stmt_modified_p (stmt));
      REWRITE_THIS_STMT (stmt) = 0;
      REGISTER_DEFS_IN_THIS_STMT (stmt) = 0;
    }
}

/* Mark block BB as interesting for update_ssa.  */

static void
mark_block_for_update (basic_block bb)
{
  gcc_assert (blocks_to_update != NULL);
  if (bitmap_bit_p (blocks_to_update, bb->index))
    return;
  bitmap_set_bit (blocks_to_update, bb->index);
  initialize_flags_in_bb (bb);
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
      db_p = XNEW (struct def_blocks_d);
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
  if (state == NEED_PHI_STATE_NO && !bitmap_empty_p (db_p->def_blocks))
    {
      int ix = bitmap_first_set_bit (db_p->def_blocks);
      if (!dominated_by_p (CDI_DOMINATORS, bb, BASIC_BLOCK (ix)))
	set_phi_state (var, NEED_PHI_STATE_MAYBE);
    }
  else
    set_phi_state (var, NEED_PHI_STATE_MAYBE);
}


/* Return true if symbol SYM is marked for renaming.  */

static inline bool
symbol_marked_for_renaming (tree sym)
{
  return bitmap_bit_p (syms_to_rename, DECL_UID (sym));
}


/* Return true if NAME is in OLD_SSA_NAMES.  */

static inline bool
is_old_name (tree name)
{
  unsigned ver = SSA_NAME_VERSION (name);
  return ver < new_ssa_names->n_bits && TEST_BIT (old_ssa_names, ver);
}


/* Return true if NAME is in NEW_SSA_NAMES.  */

static inline bool
is_new_name (tree name)
{
  unsigned ver = SSA_NAME_VERSION (name);
  return ver < new_ssa_names->n_bits && TEST_BIT (new_ssa_names, ver);
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
      mp = XNEW (struct repl_map_d);
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

  /* OLD and NEW must be different SSA names for the same symbol.  */
  gcc_assert (new != old && SSA_NAME_VAR (new) == SSA_NAME_VAR (old));

  /* We may need to grow NEW_SSA_NAMES and OLD_SSA_NAMES because our
     caller may have created new names since the set was created.  */
  if (new_ssa_names->n_bits <= num_ssa_names - 1)
    {
      unsigned int new_sz = num_ssa_names + NAME_SETS_GROWTH_FACTOR;
      new_ssa_names = sbitmap_resize (new_ssa_names, new_sz, 0);
      old_ssa_names = sbitmap_resize (old_ssa_names, new_sz, 0);
    }

  /* If this mapping is for virtual names, we will need to update
     virtual operands.  Ignore .MEM as it is not a symbol that
     can be put into SSA form independently.  If the heuristic for
     renaming the virtual symbols from scratch is enabled, it will
     want to put .MEM into SSA form from scratch, and that cannot be
     done.  */
  if (!is_gimple_reg (new) && SSA_NAME_VAR (new) != mem_var)
    {
      tree sym;
      size_t uid;

      need_to_update_vops_p = true;

      /* Keep counts of virtual mappings and symbols to use in the
	 virtual mapping heuristic.  If we have large numbers of
	 virtual mappings for a relatively low number of symbols, it
	 will make more sense to rename the symbols from scratch.
	 Otherwise, the insertion of PHI nodes for each of the old
	 names in these mappings will be very slow.  */
      sym = SSA_NAME_VAR (new);
      uid = DECL_UID (sym);
      update_ssa_stats.num_virtual_mappings++;
      if (!bitmap_bit_p (update_ssa_stats.virtual_symbols, uid))
	{
	  bitmap_set_bit (update_ssa_stats.virtual_symbols, uid);
	  update_ssa_stats.num_virtual_symbols++;
	}
    }

  /* Update the REPL_TBL table.  */
  add_to_repl_tbl (new, old);

  /* If OLD had already been registered as a new name, then all the
     names that OLD replaces should also be replaced by NEW.  */
  if (is_new_name (old))
    bitmap_ior_into (names_replaced_by (new), names_replaced_by (old));

  /* Register NEW and OLD in NEW_SSA_NAMES and OLD_SSA_NAMES,
     respectively.  */
  SET_BIT (new_ssa_names, SSA_NAME_VERSION (new));
  SET_BIT (old_ssa_names, SSA_NAME_VERSION (old));

  /* Update mapping counter to use in the virtual mapping heuristic.  */
  update_ssa_stats.num_total_mappings++;

  timevar_pop (TV_TREE_SSA_INCREMENTAL);
}


/* Hashing and equality functions for MEM_SYMS_TBL.  */

static hashval_t
mem_syms_hash (const void *p)
{
  return htab_hash_pointer ((const void *)((const mem_syms_map_t)p)->stmt);
}

static int
mem_syms_eq (const void *p1, const void *p2)
{
  return ((const mem_syms_map_t)p1)->stmt == ((const mem_syms_map_t)p2)->stmt;
}

static void
mem_syms_free (void *p)
{
  BITMAP_FREE (((mem_syms_map_t)p)->loaded);
  BITMAP_FREE (((mem_syms_map_t)p)->stored);
  free (p);
}


/* Return the memory symbols referenced by STMT.  */

static inline mem_syms_map_t
syms_referenced_by (tree stmt)
{
  struct mem_syms_map_d m, *mp;
  void **slot;

  m.stmt = stmt;
  slot = htab_find_slot (mem_syms_tbl, (void *) &m, INSERT);
  if (*slot == NULL)
    {
      mp = XNEW (struct mem_syms_map_d);
      mp->stmt = stmt;
      mp->loaded = BITMAP_ALLOC (NULL);
      mp->stored = BITMAP_ALLOC (NULL);

      get_loads_and_stores (stmt, mp->loaded, mp->stored);

      if (bitmap_empty_p (mp->loaded))
	BITMAP_FREE (mp->loaded);

      if (bitmap_empty_p (mp->stored))
	BITMAP_FREE (mp->stored);

      *slot = (void *) mp;
    }
  else
    mp = (mem_syms_map_t) *slot;

  return mp;
}


/* Call back for walk_dominator_tree used to collect definition sites
   for every variable in the function.  For every statement S in block
   BB:

   1- Variables defined by S in the DEFS of S are marked in the bitmap
      WALK_DATA->GLOBAL_DATA->KILLS.

   2- If S uses a variable VAR and there is no preceding kill of VAR,
      then it is marked in the LIVEIN_BLOCKS bitmap associated with VAR.

   This information is used to determine which variables are live
   across block boundaries to reduce the number of PHI nodes
   we create.  */

static void
mark_def_sites (struct dom_walk_data *walk_data, basic_block bb,
		block_stmt_iterator bsi)
{
  struct mark_def_sites_global_data *gd;
  bitmap kills;
  tree stmt, def;
  use_operand_p use_p;
  ssa_op_iter iter;

  stmt = bsi_stmt (bsi);
  update_stmt_if_modified (stmt);

  gd = (struct mark_def_sites_global_data *) walk_data->global_data;
  kills = gd->kills;

  gcc_assert (blocks_to_update == NULL);
  REGISTER_DEFS_IN_THIS_STMT (stmt) = 0;
  REWRITE_THIS_STMT (stmt) = 0;

  /* If a variable is used before being set, then the variable is live
     across a block boundary, so mark it live-on-entry to BB.  */
  FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter, SSA_OP_USE)
    {
      tree sym = USE_FROM_PTR (use_p);
      gcc_assert (DECL_P (sym));
      if (!bitmap_bit_p (kills, DECL_UID (sym)))
	set_livein_block (sym, bb);
      REWRITE_THIS_STMT (stmt) = 1;
    }
  
  /* Now process the defs.  Mark BB as the definition block and add
     each def to the set of killed symbols.  */
  FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_DEF)
    {
      gcc_assert (DECL_P (def));
      set_def_block (def, bb, false);
      bitmap_set_bit (kills, DECL_UID (def));
      REGISTER_DEFS_IN_THIS_STMT (stmt) = 1;
    }

  /* If the statement has memory references, process the associated
     symbols.  */
  if (stmt_references_memory_p (stmt))
    {
      mem_syms_map_t syms;
      unsigned i;
      bitmap_iterator bi;

      syms = syms_referenced_by (stmt);

      if (syms->loaded)
	{
	  REWRITE_THIS_STMT (stmt) = 1;
	  EXECUTE_IF_SET_IN_BITMAP (syms->loaded, 0, i, bi)
	    {
	      tree sym = referenced_var (i);

	      /* Note that VDEF operators are never considered killing
		 definitions, so memory symbols are always
		 live-on-entry.  */
	      set_livein_block (sym, bb);
	    }
	}

      if (syms->stored)
	{
	  REGISTER_DEFS_IN_THIS_STMT (stmt) = 1;
	  REWRITE_THIS_STMT (stmt) = 1;

	  EXECUTE_IF_SET_IN_BITMAP (syms->stored, 0, i, bi)
	    {
	      tree sym = referenced_var (i);
	      set_def_block (sym, bb, false);

	      /* Note that virtual definitions are irrelevant for
		 computing KILLS because a VDEF does not constitute a
		 killing definition of the variable.  However, the
		 operand of a virtual definitions is a use of the
		 variable, so it may cause the variable to be
		 considered live-on-entry.  */
	      set_livein_block (sym, bb);
	    }
	}
    }

  /* If we found the statement interesting then also mark the block BB
     as interesting.  */
  if (REWRITE_THIS_STMT (stmt) || REGISTER_DEFS_IN_THIS_STMT (stmt))
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
  VEC(int,heap) *work_stack;
  bitmap phi_insertion_points;

  work_stack = VEC_alloc (int, heap, n_basic_blocks);
  phi_insertion_points = BITMAP_ALLOC (NULL);

  /* Seed the work list with all the blocks in DEF_BLOCKS.  */
  EXECUTE_IF_SET_IN_BITMAP (def_blocks, 0, bb_index, bi)
    /* We use VEC_quick_push here for speed.  This is safe because we
       know that the number of definition blocks is no greater than
       the number of basic blocks, which is the initial capacity of
       WORK_STACK.  */
    VEC_quick_push (int, work_stack, bb_index);

  /* Pop a block off the worklist, add every block that appears in
     the original block's DF that we have not already processed to
     the worklist.  Iterate until the worklist is empty.   Blocks
     which are added to the worklist are potential sites for
     PHI nodes.  */
  while (VEC_length (int, work_stack) > 0)
    {
      bb_index = VEC_pop (int, work_stack);

      /* Since the registration of NEW -> OLD name mappings is done
	 separately from the call to update_ssa, when updating the SSA
	 form, the basic blocks where new and/or old names are defined
	 may have disappeared by CFG cleanup calls.  In this case,
	 we may pull a non-existing block from the work stack.  */
      gcc_assert (bb_index < (unsigned) last_basic_block);

      EXECUTE_IF_AND_COMPL_IN_BITMAP (dfs[bb_index], phi_insertion_points,
	                              0, bb_index, bi)
	{
	  /* Use a safe push because if there is a definition of VAR
	     in every basic block, then WORK_STACK may eventually have
	     more than N_BASIC_BLOCK entries.  */
	  VEC_safe_push (int, heap, work_stack, bb_index);
	  bitmap_set_bit (phi_insertion_points, bb_index);
	}
    }

  VEC_free (int, heap, work_stack);

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


/* Marks phi node PHI in basic block BB for rewrite.  */

static void
mark_phi_for_rewrite (basic_block bb, tree phi)
{
  tree_vec phis;
  unsigned i, idx = bb->index;

  if (REWRITE_THIS_STMT (phi))
    return;
  REWRITE_THIS_STMT (phi) = 1;

  if (!blocks_with_phis_to_rewrite)
    return;

  bitmap_set_bit (blocks_with_phis_to_rewrite, idx);
  VEC_reserve (tree_vec, heap, phis_to_rewrite, last_basic_block + 1);
  for (i = VEC_length (tree_vec, phis_to_rewrite); i <= idx; i++)
    VEC_quick_push (tree_vec, phis_to_rewrite, NULL);

  phis = VEC_index (tree_vec, phis_to_rewrite, idx);
  if (!phis)
    phis = VEC_alloc (tree, heap, 10);

  VEC_safe_push (tree, heap, phis, phi);
  VEC_replace (tree_vec, phis_to_rewrite, idx, phis);
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

  def_map = find_def_blocks_for (var);
  gcc_assert (def_map);

  /* Remove the blocks where we already have PHI nodes for VAR.  */
  bitmap_and_compl_into (phi_insertion_points, def_map->phi_blocks);

  /* Now compute global livein for this variable.  Note this modifies
     def_map->livein_blocks.  */
  compute_global_livein (def_map->livein_blocks, def_map->def_blocks);

  /* And insert the PHI nodes.  */
  EXECUTE_IF_AND_IN_BITMAP (phi_insertion_points, def_map->livein_blocks,
			    0, bb_index, bi)
    {
      bb = BASIC_BLOCK (bb_index);
      if (update_p)
	mark_block_for_update (bb);

      if (update_p && TREE_CODE (var) == SSA_NAME)
	{
	  /* If we are rewriting SSA names, create the LHS of the PHI
	     node by duplicating VAR.  This is useful in the case of
	     pointers, to also duplicate pointer attributes (alias
	     information, in particular).  */
	  edge_iterator ei;
	  tree new_lhs;

	  phi = create_phi_node (var, bb);
	  new_lhs = duplicate_ssa_name (var, phi);
	  SET_PHI_RESULT (phi, new_lhs);
	  add_new_name_mapping (new_lhs, var);

	  /* Add VAR to every argument slot of PHI.  We need VAR in
	     every argument so that rewrite_update_phi_arguments knows
	     which name is this PHI node replacing.  If VAR is a
	     symbol marked for renaming, this is not necessary, the
	     renamer will use the symbol on the LHS to get its
	     reaching definition.  */
	  FOR_EACH_EDGE (e, ei, bb->preds)
	    add_phi_arg (phi, var, e);
	}
      else
	{
	  tree sym = DECL_P (var) ? var : SSA_NAME_VAR (var);
	  phi = create_phi_node (sym, bb);
	}

      /* Mark this PHI node as interesting for update_ssa.  */
      REGISTER_DEFS_IN_THIS_STMT (phi) = 1;
      mark_phi_for_rewrite (bb, phi);
    }
}


/* Insert PHI nodes at the dominance frontier of blocks with variable
   definitions.  DFS contains the dominance frontier information for
   the flowgraph.  PHI nodes will only be inserted at the dominance
   frontier of definition blocks for variables whose NEED_PHI_STATE
   annotation is marked as ``maybe'' or ``unknown'' (computed by
   mark_def_sites).  */

static void
insert_phi_nodes (bitmap *dfs)
{
  referenced_var_iterator rvi;
  tree var;

  timevar_push (TV_TREE_INSERT_PHI_NODES);
  
  FOR_EACH_REFERENCED_VAR (var, rvi)
    {
      struct def_blocks_d *def_map;
      bitmap idf;

      def_map = find_def_blocks_for (var);
      if (def_map == NULL)
	continue;

      if (get_phi_state (var) != NEED_PHI_STATE_NO)
	{
	  idf = find_idf (def_map->def_blocks, dfs);
	  insert_phi_nodes_for (var, idf, false);
	  BITMAP_FREE (idf);
	}
    }

  timevar_pop (TV_TREE_INSERT_PHI_NODES);
}


/* Push SYM's current reaching definition into BLOCK_DEFS_STACK and
   register DEF (an SSA_NAME) to be a new definition for SYM.  */

static void
register_new_def (tree def, tree sym)
{
  tree currdef;
   
  /* If this variable is set in a single basic block and all uses are
     dominated by the set(s) in that single basic block, then there is
     no reason to record anything for this variable in the block local
     definition stacks.  Doing so just wastes time and memory.

     This is the same test to prune the set of variables which may
     need PHI nodes.  So we just use that information since it's already
     computed and available for us to use.  */
  if (get_phi_state (sym) == NEED_PHI_STATE_NO)
    {
      set_current_def (sym, def);
      return;
    }

  currdef = get_current_def (sym);

  /* If SYM is not a GIMPLE register, then CURRDEF may be a name whose
     SSA_NAME_VAR is not necessarily SYM.  In this case, also push SYM
     in the stack so that we know which symbol is being defined by
     this SSA name when we unwind the stack.  */
  if (currdef && !is_gimple_reg (sym))
    VEC_safe_push (tree, heap, block_defs_stack, sym);

  /* Push the current reaching definition into BLOCK_DEFS_STACK.  This
     stack is later used by the dominator tree callbacks to restore
     the reaching definitions for all the variables defined in the
     block after a recursive visit to all its immediately dominated
     blocks.  If there is no current reaching definition, then just
     record the underlying _DECL node.  */
  VEC_safe_push (tree, heap, block_defs_stack, currdef ? currdef : sym);

  /* Set the current reaching definition for SYM to be DEF.  */
  set_current_def (sym, def);
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
  VEC_safe_push (tree, heap, block_defs_stack, NULL_TREE);

  /* Step 1.  Register new definitions for every PHI node in the block.
     Conceptually, all the PHI nodes are executed in parallel and each PHI
     node introduces a new version for the associated variable.  */
  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      tree result = PHI_RESULT (phi);
      /* FIXME.  For memory symbols, this will be .MEM, but we should
	 determine what symbol is associated with this particular PHI
	 function.  */
      gcc_assert (SSA_NAME_VAR (result) != mem_var);
      register_new_def (result, SSA_NAME_VAR (result));
    }
}


/* Return the current definition for variable VAR.  If none is found,
   create a new SSA name to act as the zeroth definition for VAR.  */

static inline tree
get_reaching_def (tree var)
{
  tree currdef;
  
  /* Lookup the current reaching definition for VAR.  */
  currdef = get_current_def (var);

  /* If there is no reaching definition for VAR, create and register a
     default definition for it (if needed).  */
  if (currdef == NULL_TREE)
    {
      /* If VAR is not a GIMPLE register, use the default definition
	 for .MEM.  */
      tree sym = DECL_P (var) ? var : SSA_NAME_VAR (var);
      sym = (is_gimple_reg (sym)) ? sym : mem_var;
      currdef = get_default_def_for (sym);
      set_current_def (var, currdef);
    }

  /* Return the current reaching definition for VAR, or the default
     definition, if we had to create one.  */
  return currdef;
}


/* Rewrite memory references in STMT.  */

static void
rewrite_memory_stmt (tree stmt)
{
  unsigned i;
  bitmap rdefs;
  bitmap_iterator bi;
  mem_syms_map_t syms;

  syms = syms_referenced_by (stmt);

  /* If the statement makes no loads or stores, it must have volatile
     operands.  */
  if (syms->loaded == NULL && syms->stored == NULL)
    {
      gcc_assert (stmt_ann (stmt)->has_volatile_ops);
      return;
    }

  rdefs = BITMAP_ALLOC (NULL);

  /* Rewrite loaded symbols.  */
  if (syms->loaded)
    {
      size_t j;
      struct vuse_optype_d *vuses;

      /* If STMT is a load, it should have exactly one VUSE operator.  */
      vuses = VUSE_OPS (stmt);
      gcc_assert (vuses && vuses->next == NULL);

      /* Collect all the reaching definitions for symbols loaded by STMT.  */
      bitmap_clear (rdefs);
      EXECUTE_IF_SET_IN_BITMAP (syms->loaded, 0, i, bi)
	{
	  tree sym = referenced_var (i);
	  tree rdef = get_reaching_def (sym);
	  bitmap_set_bit (rdefs, SSA_NAME_VERSION (rdef));
	}

      /* Rewrite the VUSE as VUSE <{RDEFS}>.  */
      vuses = realloc_vuse (vuses, bitmap_count_bits (rdefs));
      j = 0;
      EXECUTE_IF_SET_IN_BITMAP (rdefs, 0, i, bi)
	SET_USE (VUSE_OP_PTR (vuses, j++), ssa_name (i));

      vuses = vuses->next;
    }

  /* Rewrite stored symbols.  */
  if (syms->stored)
    {
      tree lhs, new_name;
      size_t j;
      struct vdef_optype_d *vdefs;

      /* If STMT is a store, it should have exactly one VDEF operator.  */
      vdefs = VDEF_OPS (stmt);
      gcc_assert (vdefs && vdefs->next == NULL);

      /* Collect all the current reaching definitions for symbols in STORES.  */
      bitmap_clear (rdefs);
      EXECUTE_IF_SET_IN_BITMAP (syms->stored, 0, i, bi)
	{
	  tree sym = referenced_var (i);
	  tree rdef = get_reaching_def (sym);
	  bitmap_set_bit (rdefs, SSA_NAME_VERSION (rdef));
	}

      /* Create a new name for the LHS.  If there is a single symbol
	 in STORES, use it as the target of the VDEF.  Otherwise
	 factor all the stored symbols into .MEM.  */
      if (bitmap_count_bits (syms->stored) == 1)
	lhs = referenced_var (bitmap_first_set_bit (syms->stored));
      else
	{
	  lhs = VDEF_RESULT (vdefs);
	  gcc_assert (lhs == mem_var);
	}

      new_name = make_ssa_name (lhs, stmt);

      /* Set NEW_NAME to be the current reaching definition for every
	 symbol loaded by STMT.  */
      EXECUTE_IF_SET_IN_BITMAP (syms->stored, 0, i, bi)
	{
	  tree sym = referenced_var (i);
	  register_new_def (new_name, sym);
	}

      /* Rewrite the VDEF as NEW_NAME = VDEF <{RDEFS}>.  */
      vdefs = realloc_vdef (vdefs, bitmap_count_bits (rdefs));
      SET_DEF (VDEF_RESULT_PTR (vdefs), new_name);
      j = 0;
      EXECUTE_IF_SET_IN_BITMAP (rdefs, 0, i, bi)
	SET_USE (VDEF_OP_PTR (vdefs, j++), ssa_name (i));
    }

  BITMAP_FREE (rdefs);
}


/* SSA Rewriting Step 2.  Rewrite every variable used in each statement in
   the block with its immediate reaching definitions.  Update the current
   definition of a variable when a new real or virtual definition is found.  */

static void
rewrite_stmt (struct dom_walk_data *walk_data ATTRIBUTE_UNUSED,
	      basic_block bb ATTRIBUTE_UNUSED, block_stmt_iterator si)
{
  tree stmt;
  use_operand_p use_p;
  def_operand_p def_p;
  ssa_op_iter iter;

  stmt = bsi_stmt (si);

  /* If mark_def_sites decided that we don't need to rewrite this
     statement, ignore it.  */
  gcc_assert (blocks_to_update == NULL);
  if (!REWRITE_THIS_STMT (stmt) && !REGISTER_DEFS_IN_THIS_STMT (stmt))
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Renaming statement ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  /* Step 1.  Rewrite USES in the statement.  */
  if (REWRITE_THIS_STMT (stmt))
    FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter, SSA_OP_USE)
      {
	tree var = USE_FROM_PTR (use_p);
	gcc_assert (DECL_P (var));
	SET_USE (use_p, get_reaching_def (var));
      }

  /* Step 2.  Register the statement's DEF operands.  */
  if (REGISTER_DEFS_IN_THIS_STMT (stmt))
    FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_DEF)
      {
	tree var = DEF_FROM_PTR (def_p);
	gcc_assert (DECL_P (var));
	SET_DEF (def_p, make_ssa_name (var, stmt));
	register_new_def (DEF_FROM_PTR (def_p), var);
      }

  /* Rewrite virtual operands for statements that make memory
     references.  */
  if (stmt_references_memory_p (stmt))
    rewrite_memory_stmt (stmt);
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
	  currdef = get_reaching_def (SSA_NAME_VAR (PHI_RESULT (phi)));
	  add_phi_arg (phi, currdef, e);
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
  while (VEC_length (tree, block_defs_stack) > 0)
    {
      tree tmp = VEC_pop (tree, block_defs_stack);
      tree saved_def, var;

      if (tmp == NULL_TREE)
	break;

      if (TREE_CODE (tmp) == SSA_NAME)
	{
	  /* If we recorded an SSA_NAME, then make the SSA_NAME the
	     current definition of its underlying variable.  Note that
	     if the SSA_NAME is not for a GIMPLE register, the symbol
	     being defined is stored in the next slot in the stack.
	     This mechanism is needed because an SSA name for a
	     non-register symbol may be the definition for more than
	     one symbol (e.g., SFTs, aliased variables, etc).  */
	  saved_def = tmp;
	  var = SSA_NAME_VAR (saved_def);
	  if (!is_gimple_reg (var))
	    var = VEC_pop (tree, block_defs_stack);
	}
      else
	{
	  /* If we recorded anything else, it must have been a _DECL
	     node and its current reaching definition must have been
	     NULL.  */
	  saved_def = NULL;
	  var = tmp;
	}
                                                                                
      set_current_def (var, saved_def);
    }
}


/* Dump the renaming stack (block_defs_stack) to FILE.  Traverse the
   stack up to a maximum of N levels.  If N is -1, the whole stack is
   dumped.  New levels are created when the dominator tree traversal
   used for renaming enters a new sub-tree.  */

void
dump_defs_stack (FILE *file, int n)
{
  int i, j;

  fprintf (file, "\n\nRenaming stack");
  if (n > 0)
    fprintf (file, " (up to %d levels)", n);
  fprintf (file, "\n\n");

  i = 1;
  fprintf (file, "Level %d (current level)\n", i);
  for (j = (int) VEC_length (tree, block_defs_stack) - 1; j >= 0; j--)
    {
      tree name, var;
      
      name = VEC_index (tree, block_defs_stack, j);
      if (name == NULL_TREE)
	{
	  i++;
	  if (n > 0 && i > n)
	    break;
	  fprintf (file, "\nLevel %d\n", i);
	  continue;
	}

      if (DECL_P (name))
	{
	  var = name;
	  name = NULL_TREE;
	}
      else
	{
	  var = SSA_NAME_VAR (name);
	  if (!is_gimple_reg (var))
	    {
	      j--;
	      var = VEC_index (tree, block_defs_stack, j);
	    }
	}

      fprintf (file, "    Previous CURRDEF (");
      print_generic_expr (file, var, 0);
      fprintf (file, ") = ");
      if (name)
	print_generic_expr (file, name, 0);
      else
	fprintf (file, "<NIL>");
      fprintf (file, "\n");
    }
}


/* Dump the renaming stack (block_defs_stack) to stderr.  Traverse the
   stack up to a maximum of N levels.  If N is -1, the whole stack is
   dumped.  New levels are created when the dominator tree traversal
   used for renaming enters a new sub-tree.  */

void
debug_defs_stack (int n)
{
  dump_defs_stack (stderr, n);
}


/* Dump the current reaching definition of every symbol to FILE.  */

void
dump_currdefs (FILE *file)
{
  referenced_var_iterator i;
  tree var;

  fprintf (file, "\n\nCurrent reaching definitions\n\n");
  FOR_EACH_REFERENCED_VAR (var, i)
    {
      fprintf (file, "CURRDEF (");
      print_generic_expr (file, var, 0);
      fprintf (file, ") = ");
      if (get_current_def (var))
	print_generic_expr (file, get_current_def (var), 0);
      else
	fprintf (file, "<NIL>");
      fprintf (file, "\n");
    }
}

/* Dump the current reaching definition of every symbol to stderr.  */

void
debug_currdefs (void)
{
  dump_currdefs (stderr);
}


/* Dump SSA information to FILE.  */

void
dump_tree_ssa (FILE *file)
{
  const char *funcname
    = lang_hooks.decl_printable_name (current_function_decl, 2);

  fprintf (file, "SSA renaming information for %s\n\n", funcname);

  dump_def_blocks (file);
  dump_defs_stack (file, -1);
  dump_currdefs (file);
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
  struct def_blocks_d *entry = (struct def_blocks_d *) p;
  BITMAP_FREE (entry->def_blocks);
  BITMAP_FREE (entry->phi_blocks);
  BITMAP_FREE (entry->livein_blocks);
  free (entry);
}


/* Callback for htab_traverse to dump the DEF_BLOCKS hash table.  */

static int
debug_def_blocks_r (void **slot, void *data)
{
  FILE *file = (FILE *) data;
  struct def_blocks_d *db_p = (struct def_blocks_d *) *slot;
  
  fprintf (file, "VAR: ");
  print_generic_expr (file, db_p->var, dump_flags);
  bitmap_print (file, db_p->def_blocks, ", DEF_BLOCKS: { ", "}");
  bitmap_print (file, db_p->livein_blocks, ", LIVEIN_BLOCKS: { ", "}");
  bitmap_print (file, db_p->phi_blocks, ", PHI_BLOCKS: { ", "}\n");

  return 1;
}


/* Dump the DEF_BLOCKS hash table on FILE.  */

void
dump_def_blocks (FILE *file)
{
  fprintf (file, "\n\nDefinition and live-in blocks:\n\n");
  if (def_blocks)
    htab_traverse (def_blocks, debug_def_blocks_r, file);
}


/* Dump the DEF_BLOCKS hash table on stderr.  */

void
debug_def_blocks (void)
{
  dump_def_blocks (stderr);
}


/* Register NEW_NAME to be the new reaching definition for OLD_NAME.  */

static inline void
register_new_update_single (tree new_name, tree old_name)
{
  tree currdef = get_current_def (old_name);

  /* Push the current reaching definition into BLOCK_DEFS_STACK.
     This stack is later used by the dominator tree callbacks to
     restore the reaching definitions for all the variables
     defined in the block after a recursive visit to all its
     immediately dominated blocks.  */
  VEC_reserve (tree, heap, block_defs_stack, 2);
  VEC_quick_push (tree, block_defs_stack, currdef);
  VEC_quick_push (tree, block_defs_stack, old_name);

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
  VEC_safe_push (tree, heap, block_defs_stack, NULL_TREE);

  if (!bitmap_bit_p (blocks_to_update, bb->index))
    return;

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
      tree lhs, lhs_sym;

      if (!REGISTER_DEFS_IN_THIS_STMT (phi))
	continue;
      
      lhs = PHI_RESULT (phi);
      lhs_sym = SSA_NAME_VAR (lhs);

      if (symbol_marked_for_renaming (lhs_sym))
	/* If LHS is .MEM here, we should know which symbol is this
	   .MEM a new name for.  */
	register_new_update_single (lhs, lhs_sym);
      else
	{
	  /* If LHS is a new name, register a new definition for all
	     the names replaced by LHS.  */
	  if (is_new_name (lhs))
	    register_new_update_set (lhs, names_replaced_by (lhs));
	  
	  /* If LHS is an OLD name, register it as a new definition
	     for itself.  */
	  if (is_old_name (lhs))
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
  while (VEC_length (tree, block_defs_stack) > 0)
    {
      tree var = VEC_pop (tree, block_defs_stack);
      tree saved_def;
      
      /* NULL indicates the unwind stop point for this block (see
	 rewrite_update_init_block).  */
      if (var == NULL)
	return;

      saved_def = VEC_pop (tree, block_defs_stack);
      set_current_def (var, saved_def);
    }
}


/* If the operand pointed to by USE_P is a name in OLD_SSA_NAMES or
   it is a symbol marked for renaming, replace it with USE_P's current
   reaching definition.  */

static inline void
maybe_replace_use (use_operand_p use_p)
{
  tree rdef = NULL_TREE;
  tree use = USE_FROM_PTR (use_p);
  tree sym = DECL_P (use) ? use : SSA_NAME_VAR (use);

  if (TREE_CODE (use) == SSA_NAME && is_old_name (use))
    {
      gcc_assert (!symbol_marked_for_renaming (sym));
      rdef = get_reaching_def (use);
    }
  else if (is_gimple_reg (sym) && symbol_marked_for_renaming (sym))
    {
      /* Note that when renaming naked symbols, we are only interested
	 in handling GIMPLE registers.  Memory operands are updated in
	 rewrite_update_memory_stmt.  */
      rdef = get_reaching_def (sym);
    }

  if (rdef && rdef != use)
    SET_USE (use_p, rdef);
}


/* If the operand pointed to by DEF_P is an SSA name in NEW_SSA_NAMES
   or OLD_SSA_NAMES, or if it is a symbol marked for renaming,
   register it as the current definition for the names replaced by
   DEF_P.  */

static inline void
maybe_register_def (def_operand_p def_p, tree stmt)
{
  tree def = DEF_FROM_PTR (def_p);
  tree sym = DECL_P (def) ? def : SSA_NAME_VAR (def);

  if (TREE_CODE (def) == SSA_NAME && is_new_name (def))
    {
      /* If DEF is a new name, register it as a new definition
	 for all the names replaced by DEF.  */
      gcc_assert (!symbol_marked_for_renaming (sym));
      register_new_update_set (def, names_replaced_by (def));
    }

  if (TREE_CODE (def) == SSA_NAME && is_old_name (def))
    {
      /* If DEF is an old name, register DEF as a new
	 definition for itself.  */
      gcc_assert (!symbol_marked_for_renaming (sym));
      register_new_update_single (def, def);
    }

  /* Note that when renaming naked symbols, we are only interested
     in handling GIMPLE registers.  Memory operands are updated in
     rewrite_update_memory_stmt.  */
  if (is_gimple_reg (sym) && symbol_marked_for_renaming (sym))
    {
      /* If DEF is a naked symbol that needs renaming, create a new
	 name for it.  */
      if (DECL_P (def))
	{
	  def = make_ssa_name (def, stmt);
	  SET_DEF (def_p, def);
	}

      register_new_update_single (def, sym);
    }
}


/* Return true if name N has been marked to be released after the SSA
   form has been updated.  */

static inline bool
name_marked_for_release_p (tree n)
{
  return names_to_release
         && bitmap_bit_p (names_to_release, SSA_NAME_VERSION (n));
}


/* Stale names are those that have been replaced by register_new_vdef_name.
   Since it will sometimes decide to create a new name for the LHS,
   uses of the original LHS on the virtual operands of statements
   downstream should not keep using the original LHS.
   
   This happens when the LHS used to be a .MEM name, which we
   typically try to preserve when updating the RHS of VDEF and VUSE
   operators (see rewrite_update_stmt_vops).  */

static inline void
mark_ssa_name_stale (tree n)
{
  gcc_assert (!need_to_initialize_update_ssa_p);

  if (stale_ssa_names == NULL)
    stale_ssa_names = BITMAP_ALLOC (NULL);

  bitmap_set_bit (stale_ssa_names, SSA_NAME_VERSION (n));
}


/* Return true if name N has been marked stale by the SSA updater.  */

static inline bool
stale_ssa_name_p (tree n)
{
  return stale_ssa_names
         && bitmap_bit_p (stale_ssa_names, SSA_NAME_VERSION (n));
}


/* Preserve in RDEFS all the names from the virtual operands in STMT
   that represent the symbols in UNMARKED_SYMS.  WHICH_VOPS indicates
   what virtual operator to process.
   
   At this point, RDEFS contains the reaching definitions for all the
   symbols marked for renaming in SYMS.  However, there may be some
   symbols in SYMS that had not been marked for renaming (i.e., those
   collected in UNMARKED_SYMS).

   We need to match those symbols in UNMARKED_SYMS to existing SSA
   names in the virtual operands for STMT.  Otherwise, we will remove
   use-def chains for symbols that had not been marked for renaming.

   Notice that we need to do all this maneuvering because the SSA
   names for .MEM in the operand list may need to be removed when they
   don't represent symbols in this statement anymore.

   After this, names in WHICH_VOPS that have not been added to RDEFS
   will be discarded.  */

static void
preserve_needed_names_in_vops (tree stmt, bitmap unmarked_syms,
                               bitmap rdefs, int which_vops)
{
  tree name, sym;
  use_operand_p use_p;
  ssa_op_iter iter;
  VEC(tree,heap) *mem_names_stack = NULL;

  /* We first need to match SSA names for individual symbols to
     avoid the following problem:

	    1	# .MEM_3 = VDEF <a_5, b_1, c_9>
	    2	*p_8 = ...
	    3	 ...
	    4	# a_7 = VDEF <.MEM_3>
	    5	a = ...
	    6	...
	    7	# VUSE <.MEM_3, a_7>
	    8	... = a

   Suppose that we are only renaming 'a'.  Since .MEM_3 was used
   to factor a store to 'a' and 'b', if we process .MEM_3 first,
   we will decide to keep the .MEM_3 operand because 'a' is one of its
   factored symbols.  However, the definition of 'a_7' now supercedes
   the definition for .MEM_3, and so, .MEM_3 needs to be discarded.

   Doing this two-stage matching is correct because the existing
   SSA web guarantees that the individual name 'a_7'
   post-dominates the factored name '.MEM_3'.  If there was
   another factored store in the path, then we would have
   another factored .MEM name reaching the load from 'a' at line 8.  */
  FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter, which_vops)
    {
      name = USE_FROM_PTR (use_p);
      sym = DECL_P (name) ? name : SSA_NAME_VAR (name);

      if (TREE_CODE (name) == SSA_NAME && !name_marked_for_release_p (name))
	{
	  if (sym == mem_var)
	    {
	      /* Save .MEM names to be processed after individual
		 names.  */
	      VEC_safe_push (tree, heap, mem_names_stack, name);
	    }
	  else if (bitmap_bit_p (unmarked_syms, DECL_UID (sym)))
	    {
	      /* Otherwise, if SYM is in UNMARKED_SYMS and NAME
		 has not been marked to be released, add NAME to
		 RDEFS.  */
	      bitmap_set_bit (rdefs, SSA_NAME_VERSION (name));
	      bitmap_clear_bit (unmarked_syms, DECL_UID (sym));
	    }
	}
    }

  /* If we still have unmarked symbols, match them to the .MEM name(s)  */
  if (!bitmap_empty_p (unmarked_syms))
    {
      mem_syms_map_t syms;
      bool found_default_mem_name_p = false;
      tree mem_default_def = default_def (mem_var);
      bitmap tmp = BITMAP_ALLOC (NULL);
      bitmap matched_syms = BITMAP_ALLOC (NULL);

      while (VEC_length (tree, mem_names_stack) > 0)
	{
	  name = VEC_pop (tree, mem_names_stack);

	  if (name == mem_default_def)
	    {
	      /* The default definition for .MEM is special because it
		 matches every memory symbol.  However, we only want
		 to use it as a last resort (i.e., if no other
		 existing SSA name matches).  */
	      found_default_mem_name_p = true;
	      continue;
	    }

	  /* If this .MEM name matches some of the symbols in
	     UNMARKED_SYMS, add it to RDEFS.  */
	  syms = syms_referenced_by (SSA_NAME_DEF_STMT (name));
	  if (bitmap_intersect_p (unmarked_syms, syms->stored))
	    {
	      /* Remove from UNMARKED_SYMS the symbols that are common
		 between UNMARKED_SYMS and SYMS->STORED.  If there were
		 symbols in common, add NAME to RDEFS.  */
	      bitmap_set_bit (rdefs, SSA_NAME_VERSION (name));

	      /* Add the matched symbols to MATCHED_SYMS.  */
	      bitmap_and (tmp, unmarked_syms, syms->stored);
	      bitmap_ior_into (matched_syms, tmp);
	    }
	}

      /* Remove all the matched symbols from UNMARKED_SYMS, if all the
	 unmarked symbols were matched, then we can discard the
	 default definition for .MEM (if we found one).  */
      bitmap_and_compl_into (unmarked_syms, matched_syms);

      /* The default definition for .MEM matches all the symbols that
	 could not be matched to other names in the list of operands.  */
      if (found_default_mem_name_p && !bitmap_empty_p (unmarked_syms))
	{
	  bitmap_set_bit (rdefs, SSA_NAME_VERSION (mem_default_def));
	  bitmap_clear (unmarked_syms);
	}

      BITMAP_FREE (tmp);
      BITMAP_FREE (matched_syms);
    }

  /* We should have found matches for every symbol in UNMARKED_SYMS.  */
  gcc_assert (unmarked_syms == NULL || bitmap_empty_p (unmarked_syms));
  VEC_free (tree, heap, mem_names_stack);
}


/* Helper for rewrite_update_memory_stmt.  WHICH_VOPS is either
   SSA_OP_VUSE to update the RHS of a VUSE operator or SSA_OP_VMAYUSE
   to update the RHS of a VDEF operator.  This is done by collecting
   reaching definitions for all the symbols in SYMS and writing a new
   RHS for the virtual operator.

   RDEFS is a scratch bitmap used to store reaching definitions for
   all the symbols in SYMS.  The caller is responsible for allocating
   and freeing it.

   FIXME, change bitmaps to pointer-sets when possible.  */

static void
rewrite_update_stmt_vops (tree stmt, bitmap syms, bitmap rdefs, int which_vops)
{
  unsigned i, j, num_rdefs;
  bitmap_iterator bi;
  bool all_marked_p;
  bitmap unmarked_syms = NULL;
  
  gcc_assert (which_vops == SSA_OP_VUSE || which_vops == SSA_OP_VMAYUSE);

  /* Collect all the reaching definitions for symbols marked for
     renaming in SYMS.  */
  all_marked_p = true;
  EXECUTE_IF_SET_IN_BITMAP (syms, 0, i, bi)
    {
      tree sym = referenced_var (i);
      if (symbol_marked_for_renaming (sym))
	{
	  tree rdef = get_reaching_def (sym);
	  bitmap_set_bit (rdefs, SSA_NAME_VERSION (rdef));
	}
      else
	{
	  all_marked_p = false;

	  /* Add SYM to UNMARKED_SYMS so that they can be matched to
	     existing SSA names in WHICH_VOPS.  */
	  if (unmarked_syms == NULL)
	    unmarked_syms = BITMAP_ALLOC (NULL);
	  bitmap_set_bit (unmarked_syms, DECL_UID (sym));
	}
    }

  /* Preserve names from VOPS that are needed for the symbols that
     have not been marked for renaming.  */
  if (!all_marked_p)
    {
      preserve_needed_names_in_vops (stmt, unmarked_syms, rdefs, which_vops);
      BITMAP_FREE (unmarked_syms);
    }

  /* Rewrite the appropriate virtual operand setting its RHS to RDEFS.  */
  num_rdefs = bitmap_count_bits (rdefs);
  if (which_vops == SSA_OP_VUSE)
    {
      /* STMT should have exactly one VUSE operator.  */
      struct vuse_optype_d *vuses = VUSE_OPS (stmt);
      gcc_assert (vuses && vuses->next == NULL);

      vuses = realloc_vuse (vuses, num_rdefs);
      j = 0;
      EXECUTE_IF_SET_IN_BITMAP (rdefs, 0, i, bi)
	SET_USE (VUSE_OP_PTR (vuses, j++), ssa_name (i));
    }
  else
    {
      tree lhs;
      struct vdef_optype_d *vdefs;

      /* STMT should have exactly one VDEF operator.  */
      vdefs = VDEF_OPS (stmt);
      gcc_assert (vdefs && vdefs->next == NULL);

      /* Preserve the existing LHS to avoid creating SSA names
	 unnecessarily.  */
      lhs = VDEF_RESULT (vdefs);

      vdefs = realloc_vdef (vdefs, num_rdefs);
      j = 0;
      EXECUTE_IF_SET_IN_BITMAP (rdefs, 0, i, bi)
	SET_USE (VDEF_OP_PTR (vdefs, j++), ssa_name (i));

      SET_DEF (VDEF_RESULT_PTR (vdefs), lhs);
    }
}


/* Helper for rewrite_update_memory_stmt.  Register the LHS of the
   VDEF operator in STMT to be the current reaching definition of
   every symbol in the bitmap STORES.  */

static void
register_new_vdef_name (tree stmt, bitmap stores)
{
  tree lhs, new_name;
  struct vdef_optype_d *vdefs;
  bitmap_iterator bi;
  unsigned i;

  /* If needed, create a new name for the LHS.  */
  vdefs = VDEF_OPS (stmt);
  lhs = VDEF_RESULT (vdefs);
  if (DECL_P (lhs))
    {
      /* If there is a single symbol in STORES, use it as the target
	 of the VDEF.  Otherwise factor all the stored symbols into
	 .MEM.  */
      if (bitmap_count_bits (stores) == 1)
	lhs = referenced_var (bitmap_first_set_bit (stores));
      else
	lhs = mem_var;

      new_name = make_ssa_name (lhs, stmt);
    }
  else
    {
      /* If the LHS is already an SSA name, then we may not need to
	 create a new name.  If the underlying symbol for LHS is the
	 same as the symbol we want to use, then re-use it.
	 Otherwise, create a new SSA name for it.  */
      tree new_lhs_sym;

      if (bitmap_count_bits (stores) == 1)
	new_lhs_sym = referenced_var (bitmap_first_set_bit (stores));
      else
	new_lhs_sym = mem_var;

      if (new_lhs_sym == SSA_NAME_VAR (lhs))
	new_name = lhs;
      else
	{
	  /* Create a new SSA name for the LHS and mark the original
	     LHS stale.  This will prevent rewrite_update_stmt_vops
	     from keeping LHS in statements that still use it.  FIXME,
	     this does not help statements that are never visited by
	     update_ssa.  */
	  new_name = make_ssa_name (new_lhs_sym, stmt);
	  mark_ssa_name_stale (lhs);
	}
    }

  /* Set NEW_NAME to be the current reaching definition for every
     symbol on the RHS of the VDEF.  */
  SET_DEF (VDEF_RESULT_PTR (vdefs), new_name);
  EXECUTE_IF_SET_IN_BITMAP (stores, 0, i, bi)
    {
      tree sym = referenced_var (i);
      if (symbol_marked_for_renaming (sym))
	register_new_update_single (new_name, sym);
    }
}


/* Update every SSA memory reference in STMT.  */

static void
rewrite_update_memory_stmt (tree stmt)
{
  bitmap rdefs;
  mem_syms_map_t syms;

  syms = syms_referenced_by (stmt);

  /* If the statement makes no loads or stores, it must have volatile
     operands.  */
  if (syms->loaded == NULL && syms->stored == NULL)
    {
      gcc_assert (stmt_ann (stmt)->has_volatile_ops);
      return;
    }

  rdefs = BITMAP_ALLOC (NULL);

  /* Rewrite loaded symbols marked for renaming.  */
  if (syms->loaded)
    {
      rewrite_update_stmt_vops (stmt, syms->loaded, rdefs, SSA_OP_VUSE);
      bitmap_clear (rdefs);
    }

  if (syms->stored)
    {
      /* Rewrite stored symbols marked for renaming.  */
      rewrite_update_stmt_vops (stmt, syms->stored, rdefs, SSA_OP_VMAYUSE);

      /* Register the LHS of the VDEF to be the new reaching
	 definition of all the symbols in STORES.  */
      register_new_vdef_name (stmt, syms->stored);
    }

  BITMAP_FREE (rdefs);
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

  gcc_assert (bitmap_bit_p (blocks_to_update, bb->index));

  /* Only update marked statements.  */
  if (!REWRITE_THIS_STMT (stmt) && !REGISTER_DEFS_IN_THIS_STMT (stmt))
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Updating SSA information for statement ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  /* If there are memory symbols to put in SSA form, process them.  */
  if (need_to_update_vops_p
      && stmt_references_memory_p (stmt)
      && !bitmap_empty_p (syms_to_rename))
    rewrite_update_memory_stmt (stmt);

  /* Rewrite USES included in OLD_SSA_NAMES and USES whose underlying
     symbol is marked for renaming.  */
  if (REWRITE_THIS_STMT (stmt))
    FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter, SSA_OP_ALL_USES)
      maybe_replace_use (use_p);

  /* Register definitions of names in NEW_SSA_NAMES and OLD_SSA_NAMES.
     Also register definitions for names whose underlying symbol is
     marked for renaming.  */
  if (REGISTER_DEFS_IN_THIS_STMT (stmt))
    FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_ALL_DEFS)
      maybe_register_def (def_p, stmt);
}


/* Replace the operand pointed to by USE_P with USE's current reaching
   definition.  */

static inline void
replace_use (use_operand_p use_p, tree use)
{
  tree rdef = get_reaching_def (use);
  if (rdef != use)
    SET_USE (use_p, rdef);
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
  unsigned i;

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      tree phi;
      tree_vec phis;

      if (!bitmap_bit_p (blocks_with_phis_to_rewrite, e->dest->index))
	continue;
     
      phis = VEC_index (tree_vec, phis_to_rewrite, e->dest->index);
      for (i = 0; VEC_iterate (tree, phis, i, phi); i++)
	{
	  tree arg;
	  use_operand_p arg_p;

  	  gcc_assert (REWRITE_THIS_STMT (phi));

	  arg_p = PHI_ARG_DEF_PTR_FROM_EDGE (phi, e);
	  arg = USE_FROM_PTR (arg_p);

	  if (arg && !DECL_P (arg) && TREE_CODE (arg) != SSA_NAME)
	    continue;

	  if (arg == NULL_TREE)
	    {
	      /* When updating a PHI node for a recently introduced
		 symbol we may find NULL arguments.  That's why we
		 take the symbol from the LHS of the PHI node.  */
	      replace_use (arg_p, SSA_NAME_VAR (PHI_RESULT (phi)));
	    }
	  else
	    {
	      tree sym, lhs, arg_sym, lhs_sym;

	      lhs = PHI_RESULT (phi);
	      lhs_sym = SSA_NAME_VAR (lhs);
	      arg_sym = DECL_P (arg) ? arg : SSA_NAME_VAR (arg);

	      /* Make sure we use the right symbol when updating PHIs
		 for memory symbols.  When either the LHS of a PHI or
		 the argument is a memory symbol, then use the LHS of
		 the PHI as that always contains the right symbol
		 being defined by this PHI.  */
	      if (!is_gimple_reg (lhs_sym) || !is_gimple_reg (arg_sym))
		sym = lhs_sym;
	      else
		sym = arg_sym;

	      if (symbol_marked_for_renaming (sym))
		replace_use (arg_p, sym);
	      else if (is_old_name (arg))
		replace_use (arg_p, arg);
	    }

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
  memset (&walk_data, 0, sizeof (walk_data));

  walk_data.dom_direction = CDI_DOMINATORS;
  walk_data.interesting_blocks = blocks;

  if (what == REWRITE_UPDATE)
    walk_data.before_dom_children_before_stmts = rewrite_update_init_block;
  else
    walk_data.before_dom_children_before_stmts = rewrite_initialize_block;

  if (what == REWRITE_ALL)
    walk_data.before_dom_children_walk_stmts = rewrite_stmt;
  else if (what == REWRITE_UPDATE)
    walk_data.before_dom_children_walk_stmts = rewrite_update_stmt;
  else
    gcc_unreachable ();

  if (what == REWRITE_ALL)
    walk_data.before_dom_children_after_stmts = rewrite_add_phi_arguments;
  else if (what == REWRITE_UPDATE)
    walk_data.before_dom_children_after_stmts = rewrite_update_phi_arguments;
  else
    gcc_unreachable ();
  
  if (what == REWRITE_ALL)
    walk_data.after_dom_children_after_stmts =  rewrite_finalize_block;
  else if (what == REWRITE_UPDATE)
    walk_data.after_dom_children_after_stmts = rewrite_update_fini_block;
  else
    gcc_unreachable ();

  block_defs_stack = VEC_alloc (tree, heap, 10);

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
  
  VEC_free (tree, heap, block_defs_stack);

  timevar_pop (TV_TREE_SSA_REWRITE_BLOCKS);
}


/* Block initialization routine for mark_def_sites.  Clear the 
   KILLS bitmap at the start of each block.  */

static void
mark_def_sites_initialize_block (struct dom_walk_data *walk_data,
				 basic_block bb ATTRIBUTE_UNUSED)
{
  struct mark_def_sites_global_data *gd =
     (struct mark_def_sites_global_data *) walk_data->global_data;
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
  struct dom_walk_data walk_data;
  struct mark_def_sites_global_data mark_def_sites_global_data;

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


/* Initialize internal data needed during renaming.  */

static void
init_ssa_renamer (void)
{
  tree var;
  referenced_var_iterator rvi;

  in_ssa_p = false;

  /* Allocate memory for the DEF_BLOCKS hash table.  */
  gcc_assert (def_blocks == NULL);
  def_blocks = htab_create (num_referenced_vars, def_blocks_hash,
                            def_blocks_eq, def_blocks_free);

  FOR_EACH_REFERENCED_VAR(var, rvi)
    set_current_def (var, NULL_TREE);

  /* Allocate the table to map statements to the symbols they load/store.  */
  gcc_assert (mem_syms_tbl == NULL);
  mem_syms_tbl = htab_create (200, mem_syms_hash, mem_syms_eq, mem_syms_free);
}


/* Deallocate internal data structures used by the renamer.  */

static void
fini_ssa_renamer (void)
{
  if (mem_syms_tbl)
    {
      htab_delete (mem_syms_tbl);
      mem_syms_tbl = NULL;
    }

  if (def_blocks)
    {
      htab_delete (def_blocks);
      def_blocks = NULL;
    }

  in_ssa_p = true;
}


/* Main entry point into the SSA builder.  The renaming process
   proceeds in four main phases:

   1- Compute dominance frontier and immediate dominators, needed to
      insert PHI nodes and rename the function in dominator tree
      order.

   2- Find and mark all the blocks that define variables
      (mark_def_site_blocks).

   3- Insert PHI nodes at dominance frontiers (insert_phi_nodes).

   4- Rename all the blocks (rewrite_blocks) and statements in the program.

   Steps 3 and 4 are done using the dominator tree walker
   (walk_dominator_tree).  */

static unsigned int
rewrite_into_ssa (void)
{
  bitmap *dfs;
  basic_block bb;
  sbitmap interesting_blocks;
  
  timevar_push (TV_TREE_SSA_OTHER);

  /* Initialize operand data structures.  */
  init_ssa_operands ();

  /* Initialize internal data needed by the renamer.  */
  init_ssa_renamer ();

  /* Initialize the set of interesting blocks.  The callback
     mark_def_sites will add to this set those blocks that the renamer
     should process.  */
  interesting_blocks = sbitmap_alloc (last_basic_block);
  sbitmap_zero (interesting_blocks);

  /* Initialize dominance frontier.  */
  dfs = (bitmap *) xmalloc (last_basic_block * sizeof (bitmap));
  FOR_EACH_BB (bb)
    dfs[bb->index] = BITMAP_ALLOC (NULL);

  /* 1- Compute dominance frontiers.  */
  calculate_dominance_info (CDI_DOMINATORS);
  compute_dominance_frontiers (dfs);

  /* 2- Find and mark definition sites.  */
  mark_def_site_blocks (interesting_blocks);

  /* 3- Insert PHI nodes at dominance frontiers of definition blocks.  */
  insert_phi_nodes (dfs);

  /* 4- Rename all the blocks.  */
  rewrite_blocks (ENTRY_BLOCK_PTR, REWRITE_ALL, interesting_blocks);

  /* Free allocated memory.  */
  FOR_EACH_BB (bb)
    BITMAP_FREE (dfs[bb->index]);
  free (dfs);
  sbitmap_free (interesting_blocks);

  fini_ssa_renamer ();

  timevar_pop (TV_TREE_SSA_OTHER);
  return 0;
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
  TODO_dump_func
    | TODO_verify_ssa
    | TODO_remove_unused_locals,	/* todo_flags_finish */
  0					/* letter */
};


/* Mark the definition of VAR at STMT and BB as interesting for the
   renamer.  BLOCKS is the set of blocks that need updating.  */

static void
mark_def_interesting (tree var, tree stmt, basic_block bb, bool insert_phi_p)
{
  gcc_assert (bitmap_bit_p (blocks_to_update, bb->index));
  REGISTER_DEFS_IN_THIS_STMT (stmt) = 1;

  if (insert_phi_p)
    {
      bool is_phi_p = TREE_CODE (stmt) == PHI_NODE;

      set_def_block (var, bb, is_phi_p);

      /* If VAR is an SSA name in NEW_SSA_NAMES, this is a definition
	 site for both itself and all the old names replaced by it.  */
      if (TREE_CODE (var) == SSA_NAME && is_new_name (var))
	{
	  bitmap_iterator bi;
	  unsigned i;
	  bitmap set = names_replaced_by (var);
	  if (set)
	    EXECUTE_IF_SET_IN_BITMAP (set, 0, i, bi)
	      set_def_block (ssa_name (i), bb, is_phi_p);
	}
    }
}


/* Mark the use of VAR at STMT and BB as interesting for the
   renamer.  INSERT_PHI_P is true if we are going to insert new PHI
   nodes.  */

static inline void
mark_use_interesting (tree var, tree stmt, basic_block bb, bool insert_phi_p)
{
  basic_block def_bb = bb_for_stmt (stmt);

  mark_block_for_update (def_bb);
  mark_block_for_update (bb);

  if (TREE_CODE (stmt) == PHI_NODE)
    mark_phi_for_rewrite (def_bb, stmt);
  else
    REWRITE_THIS_STMT (stmt) = 1;

  /* If VAR has not been defined in BB, then it is live-on-entry
     to BB.  Note that we cannot just use the block holding VAR's
     definition because if VAR is one of the names in OLD_SSA_NAMES,
     it will have several definitions (itself and all the names that
     replace it).  */
  if (insert_phi_p)
    {
      struct def_blocks_d *db_p = get_def_blocks_for (var);
      if (!bitmap_bit_p (db_p->def_blocks, bb->index))
	set_livein_block (var, bb);
    }
}


/* Do a dominator walk starting at BB processing statements that
   reference symbols in SYMS_TO_RENAME.  This is very similar to
   mark_def_sites, but the scan handles statements whose operands may
   already be SSA names.

   If INSERT_PHI_P is true, mark those uses as live in the
   corresponding block.  This is later used by the PHI placement
   algorithm to make PHI pruning decisions.  */

static void
prepare_block_for_update (basic_block bb, bool insert_phi_p)
{
  basic_block son;
  block_stmt_iterator si;
  tree phi;

  mark_block_for_update (bb);

  /* Process PHI nodes marking interesting those that define or use
     the symbols that we are interested in.  */
  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      tree lhs_sym, lhs = PHI_RESULT (phi);

      lhs_sym = DECL_P (lhs) ? lhs : SSA_NAME_VAR (lhs);

      /* Have to check all the symbols in the arg list of the PHI
	 node.  */
      if (symbol_marked_for_renaming (lhs_sym))
	{
	  mark_use_interesting (lhs_sym, phi, bb, insert_phi_p);
	  mark_def_interesting (lhs_sym, phi, bb, insert_phi_p);
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

      FOR_EACH_SSA_USE_OPERAND (use_p, stmt, i, SSA_OP_USE)
	{
	  tree use = USE_FROM_PTR (use_p);
	  tree sym = DECL_P (use) ? use : SSA_NAME_VAR (use);
	  if (symbol_marked_for_renaming (sym))
	    mark_use_interesting (use, stmt, bb, insert_phi_p);
	}

      FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, i, SSA_OP_DEF)
	{
	  tree def = DEF_FROM_PTR (def_p);
	  tree sym = DECL_P (def) ? def : SSA_NAME_VAR (def);
	  if (symbol_marked_for_renaming (sym))
	    mark_def_interesting (def, stmt, bb, insert_phi_p);
	}

      if (need_to_update_vops_p && stmt_references_memory_p (stmt))
	{
	  unsigned i;
	  bitmap_iterator bi;
	  mem_syms_map_t syms;

	  syms = syms_referenced_by (stmt);

	  if (syms->stored)
	    EXECUTE_IF_SET_IN_BITMAP (syms->stored, 0, i, bi)
	      {
		tree sym = referenced_var (i);
		if (symbol_marked_for_renaming (sym))
		  {
		    mark_use_interesting (sym, stmt, bb, insert_phi_p);
		    mark_def_interesting (sym, stmt, bb, insert_phi_p);
		  }
	      }

	  if (syms->loaded)
	    EXECUTE_IF_SET_IN_BITMAP (syms->loaded, 0, i, bi)
	      {
		tree sym = referenced_var (i);
		if (symbol_marked_for_renaming (sym))
		  mark_use_interesting (sym, stmt, bb, insert_phi_p);
	      }
	}
    }

  /* Now visit all the blocks dominated by BB.  */
  for (son = first_dom_son (CDI_DOMINATORS, bb);
      son;
      son = next_dom_son (CDI_DOMINATORS, son))
    prepare_block_for_update (son, insert_phi_p);
}


/* Helper for prepare_names_to_update.  Mark all the use sites for
   NAME as interesting.  BLOCKS and INSERT_PHI_P are as in
   prepare_names_to_update.  */

static void
prepare_use_sites_for (tree name, bool insert_phi_p)
{
  use_operand_p use_p;
  imm_use_iterator iter;

  FOR_EACH_IMM_USE_FAST (use_p, iter, name)
    {
      tree stmt = USE_STMT (use_p);
      basic_block bb = bb_for_stmt (stmt);

      if (TREE_CODE (stmt) == PHI_NODE)
	{
	  /* Mark this use of NAME interesting for the renamer.
	     Notice that we explicitly call mark_use_interesting with
	     INSERT_PHI_P == false.

	     This is to avoid marking NAME as live-in in this block
	     BB. If we were to mark NAME live-in to BB, then NAME
	     would be considered live-in through ALL incoming edges to
	     BB which is not what we want.  Since we are updating the
	     SSA form for NAME, we don't really know what other names
	     of NAME are coming in through other edges into BB.

	     If we considered NAME live-in at BB, then the PHI
	     placement algorithm may try to insert PHI nodes in blocks
	     that are not only unnecessary but also the renamer would
	     not know how to fill in.  */
	  mark_use_interesting (name, stmt, bb, false);

	  /* As discussed above, we only want to mark NAME live-in
	     through the edge corresponding to its slot inside the PHI
	     argument list.  So, we look for the block BB1 where NAME
	     is flowing through.  If BB1 does not contain a definition
	     of NAME, then consider NAME live-in at BB1.  */
	  if (insert_phi_p)
	    {
	      int ix = PHI_ARG_INDEX_FROM_USE (use_p);
	      edge e = PHI_ARG_EDGE (stmt, ix);
	      basic_block bb1 = e->src;
	      struct def_blocks_d *db = get_def_blocks_for (name);

	      if (!bitmap_bit_p (db->def_blocks, bb1->index))
		set_livein_block (name, bb1);
	    }
	}
      else
	{
	  /* For regular statements, mark this as an interesting use
	     for NAME.  */
	  mark_use_interesting (name, stmt, bb, insert_phi_p);
	}
    }
}


/* Helper for prepare_names_to_update.  Mark the definition site for
   NAME as interesting.  BLOCKS and INSERT_PHI_P are as in
   prepare_names_to_update.  */

static void
prepare_def_site_for (tree name, bool insert_phi_p)
{
  tree stmt;
  basic_block bb;

  gcc_assert (names_to_release == NULL
	      || !bitmap_bit_p (names_to_release, SSA_NAME_VERSION (name)));

  stmt = SSA_NAME_DEF_STMT (name);
  bb = bb_for_stmt (stmt);
  if (bb)
    {
      gcc_assert (bb->index < last_basic_block);
      mark_block_for_update (bb);
      mark_def_interesting (name, stmt, bb, insert_phi_p);
    }
}


/* Mark definition and use sites of names in NEW_SSA_NAMES and
   OLD_SSA_NAMES.  INSERT_PHI_P is true if the caller wants to insert
   PHI nodes for newly created names.  */

static void
prepare_names_to_update (bool insert_phi_p)
{
  unsigned i = 0;
  bitmap_iterator bi;
  sbitmap_iterator sbi;

  /* If a name N from NEW_SSA_NAMES is also marked to be released,
     remove it from NEW_SSA_NAMES so that we don't try to visit its
     defining basic block (which most likely doesn't exist).  Notice
     that we cannot do the same with names in OLD_SSA_NAMES because we
     want to replace existing instances.  */
  if (names_to_release)
    EXECUTE_IF_SET_IN_BITMAP (names_to_release, 0, i, bi)
      RESET_BIT (new_ssa_names, i);

  /* First process names in NEW_SSA_NAMES.  Otherwise, uses of old
     names may be considered to be live-in on blocks that contain
     definitions for their replacements.  */
  EXECUTE_IF_SET_IN_SBITMAP (new_ssa_names, 0, i, sbi)
    prepare_def_site_for (ssa_name (i), insert_phi_p);

  /* If an old name is in NAMES_TO_RELEASE, we cannot remove it from
     OLD_SSA_NAMES, but we have to ignore its definition site.  */
  EXECUTE_IF_SET_IN_SBITMAP (old_ssa_names, 0, i, sbi)
    {
      if (names_to_release == NULL || !bitmap_bit_p (names_to_release, i))
	prepare_def_site_for (ssa_name (i), insert_phi_p);
      prepare_use_sites_for (ssa_name (i), insert_phi_p);
    }
}


/* Dump all the names replaced by NAME to FILE.  */

void
dump_names_replaced_by (FILE *file, tree name)
{
  unsigned i;
  bitmap old_set;
  bitmap_iterator bi;

  print_generic_expr (file, name, 0);
  fprintf (file, " -> { ");

  old_set = names_replaced_by (name);
  EXECUTE_IF_SET_IN_BITMAP (old_set, 0, i, bi)
    {
      print_generic_expr (file, ssa_name (i), 0);
      fprintf (file, " ");
    }

  fprintf (file, "}\n");
}


/* Dump all the names replaced by NAME to stderr.  */

void
debug_names_replaced_by (tree name)
{
  dump_names_replaced_by (stderr, name);
}


/* Dump SSA update information to FILE.  */

void
dump_update_ssa (FILE *file)
{
  unsigned i = 0;
  bitmap_iterator bi;

  if (!need_ssa_update_p ())
    return;

  if (new_ssa_names && sbitmap_first_set_bit (new_ssa_names) >= 0)
    {
      sbitmap_iterator sbi;

      fprintf (file, "\nSSA replacement table\n");
      fprintf (file, "N_i -> { O_1 ... O_j } means that N_i replaces "
	             "O_1, ..., O_j\n\n");

      EXECUTE_IF_SET_IN_SBITMAP (new_ssa_names, 0, i, sbi)
	dump_names_replaced_by (file, ssa_name (i));

      fprintf (file, "\n");
      fprintf (file, "Number of virtual NEW -> OLD mappings: %7u\n",
	       update_ssa_stats.num_virtual_mappings);
      fprintf (file, "Number of real NEW -> OLD mappings:    %7u\n",
	       update_ssa_stats.num_total_mappings
	       - update_ssa_stats.num_virtual_mappings);
      fprintf (file, "Number of total NEW -> OLD mappings:   %7u\n",
	       update_ssa_stats.num_total_mappings);

      fprintf (file, "\nNumber of virtual symbols: %u\n",
	       update_ssa_stats.num_virtual_symbols);
    }

  if (syms_to_rename && !bitmap_empty_p (syms_to_rename))
    {
      fprintf (file, "\n\nSymbols to be put in SSA form\n\n");
      EXECUTE_IF_SET_IN_BITMAP (syms_to_rename, 0, i, bi)
	{
	  print_generic_expr (file, referenced_var (i), 0);
	  fprintf (file, " ");
	}
    }

  if (names_to_release && !bitmap_empty_p (names_to_release))
    {
      fprintf (file, "\n\nSSA names to release after updating the SSA web\n\n");
      EXECUTE_IF_SET_IN_BITMAP (names_to_release, 0, i, bi)
	{
	  print_generic_expr (file, ssa_name (i), 0);
	  fprintf (file, " ");
	}
    }

  if (stale_ssa_names && !bitmap_empty_p (stale_ssa_names))
    {
      fprintf (file, "\n\nSSA names marked stale\n\n");
      EXECUTE_IF_SET_IN_BITMAP (stale_ssa_names, 0, i, bi)
	{
	  print_generic_expr (file, ssa_name (i), 0);
	  fprintf (file, " ");
	}
    }

  fprintf (file, "\n\n");
}


/* Dump SSA update information to stderr.  */

void
debug_update_ssa (void)
{
  dump_update_ssa (stderr);
}


/* Initialize data structures used for incremental SSA updates.  */

static void
init_update_ssa (void)
{
  /* Reserve more space than the current number of names.  The calls to
     add_new_name_mapping are typically done after creating new SSA
     names, so we'll need to reallocate these arrays.  */
  old_ssa_names = sbitmap_alloc (num_ssa_names + NAME_SETS_GROWTH_FACTOR);
  sbitmap_zero (old_ssa_names);

  new_ssa_names = sbitmap_alloc (num_ssa_names + NAME_SETS_GROWTH_FACTOR);
  sbitmap_zero (new_ssa_names);

  repl_tbl = htab_create (20, repl_map_hash, repl_map_eq, repl_map_free);
  need_to_initialize_update_ssa_p = false;
  need_to_update_vops_p = false;
  syms_to_rename = BITMAP_ALLOC (NULL);
  names_to_release = NULL;
  stale_ssa_names = NULL;
  memset (&update_ssa_stats, 0, sizeof (update_ssa_stats));
  update_ssa_stats.virtual_symbols = BITMAP_ALLOC (NULL);
}


/* Deallocate data structures used for incremental SSA updates.  */

void
delete_update_ssa (void)
{
  unsigned i;
  bitmap_iterator bi;

  sbitmap_free (old_ssa_names);
  old_ssa_names = NULL;

  sbitmap_free (new_ssa_names);
  new_ssa_names = NULL;

  htab_delete (repl_tbl);
  repl_tbl = NULL;

  need_to_initialize_update_ssa_p = true;
  need_to_update_vops_p = false;
  BITMAP_FREE (syms_to_rename);
  BITMAP_FREE (update_ssa_stats.virtual_symbols);
  BITMAP_FREE (stale_ssa_names);

  if (names_to_release)
    {
      EXECUTE_IF_SET_IN_BITMAP (names_to_release, 0, i, bi)
	release_ssa_name (ssa_name (i));
      BITMAP_FREE (names_to_release);
    }

  clear_ssa_name_info ();

  fini_ssa_renamer ();
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

  /* For the benefit of passes that will be updating the SSA form on
     their own, set the current reaching definition of OLD_NAME to be
     NEW_NAME.  */
  set_current_def (old_name, new_name);

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


/* Register symbol SYM to be renamed by update_ssa.  */

void
mark_sym_for_renaming (tree sym)
{
  /* .MEM is not a regular symbol, it is a device for factoring
     multiple stores, much like a PHI function factors multiple
     control flow paths.  */
  gcc_assert (sym != mem_var);

#if 0
  /* Variables with sub-variables should have their sub-variables
     marked separately.  */
  gcc_assert (get_subvars_for_var (sym) == NULL);
#endif

  if (need_to_initialize_update_ssa_p)
    init_update_ssa ();

#if 1
  /* HACK.  Caller should be responsible for this.  */
  {
    subvar_t svars;
    if (var_can_have_subvars (sym) && (svars = get_subvars_for_var (sym)))
      {
	subvar_t sv;
	for (sv = svars; sv; sv = sv->next)
	  bitmap_set_bit (syms_to_rename, DECL_UID (sv->var));
      }
  }
#endif

  bitmap_set_bit (syms_to_rename, DECL_UID (sym));

  if (!is_gimple_reg (sym))
    need_to_update_vops_p = true;
}


/* Register all the symbols in SET to be renamed by update_ssa.  */

void
mark_set_for_renaming (bitmap set)
{
  bitmap_iterator bi;
  unsigned i;

#if 0
  /* Variables with sub-variables should have their sub-variables
     marked separately.  */
  EXECUTE_IF_SET_IN_BITMAP (set, 0, i, bi)
    gcc_assert (get_subvars_for_var (referenced_var (i)) == NULL);
#endif

  if (bitmap_empty_p (set))
    return;

  if (need_to_initialize_update_ssa_p)
    init_update_ssa ();

#if 1
  /* HACK.  Caller should be responsible for this.  */
  EXECUTE_IF_SET_IN_BITMAP (set, 0, i, bi)
    {
      subvar_t svars;
      tree var = referenced_var (i);
      if (var_can_have_subvars (var) && (svars = get_subvars_for_var (var)))
	{
	  subvar_t sv;
	  for (sv = svars; sv; sv = sv->next)
	    bitmap_set_bit (syms_to_rename, DECL_UID (sv->var));
	}
    }
#endif

  bitmap_ior_into (syms_to_rename, set);

  if (!need_to_update_vops_p)
    EXECUTE_IF_SET_IN_BITMAP (set, 0, i, bi)
      if (!is_gimple_reg (referenced_var (i)))
	{
	  need_to_update_vops_p = true;
	  break;
	}
}


/* Return true if there is any work to be done by update_ssa.  */

bool
need_ssa_update_p (void)
{
  return syms_to_rename || old_ssa_names || new_ssa_names;
}


/* Return true if name N has been registered in the replacement table.  */

bool
name_registered_for_update_p (tree n)
{
  if (!need_ssa_update_p ())
    return false;

  return is_new_name (n)
         || is_old_name (n)
	 || symbol_marked_for_renaming (SSA_NAME_VAR (n));
}


/* Return the set of all the SSA names marked to be replaced.  */

bitmap
ssa_names_to_replace (void)
{
  unsigned i = 0;
  bitmap ret;
  sbitmap_iterator sbi;
  
  ret = BITMAP_ALLOC (NULL);
  EXECUTE_IF_SET_IN_SBITMAP (old_ssa_names, 0, i, sbi)
    bitmap_set_bit (ret, i);

  return ret;
}


/* Mark NAME to be released after update_ssa has finished.  */

void
release_ssa_name_after_update_ssa (tree name)
{
  if (need_to_initialize_update_ssa_p)
    init_update_ssa ();

  if (names_to_release == NULL)
    names_to_release = BITMAP_ALLOC (NULL);

  bitmap_set_bit (names_to_release, SSA_NAME_VERSION (name));
}


/* Insert new PHI nodes to replace VAR.  DFS contains dominance
   frontier information.  BLOCKS is the set of blocks to be updated.

   This is slightly different than the regular PHI insertion
   algorithm.  The value of UPDATE_FLAGS controls how PHI nodes for
   real names (i.e., GIMPLE registers) are inserted:
 
   - If UPDATE_FLAGS == TODO_update_ssa, we are only interested in PHI
     nodes inside the region affected by the block that defines VAR
     and the blocks that define all its replacements.  All these
     definition blocks are stored in DEF_BLOCKS[VAR]->DEF_BLOCKS.

     First, we compute the entry point to the region (ENTRY).  This is
     given by the nearest common dominator to all the definition
     blocks. When computing the iterated dominance frontier (IDF), any
     block not strictly dominated by ENTRY is ignored.

     We then call the standard PHI insertion algorithm with the pruned
     IDF.

   - If UPDATE_FLAGS == TODO_update_ssa_full_phi, the IDF for real
     names is not pruned.  PHI nodes are inserted at every IDF block.  */

static void
insert_updated_phi_nodes_for (tree var, bitmap *dfs, bitmap blocks,
                              unsigned update_flags)
{
  basic_block entry;
  struct def_blocks_d *db;
  bitmap idf, pruned_idf;
  bitmap_iterator bi;
  unsigned i;

#if defined ENABLE_CHECKING
  if (TREE_CODE (var) == SSA_NAME)
    gcc_assert (is_old_name (var));
  else
    gcc_assert (symbol_marked_for_renaming (var));
#endif

  /* Get all the definition sites for VAR.  */
  db = find_def_blocks_for (var);

  /* No need to do anything if there were no definitions to VAR.  */
  if (db == NULL || bitmap_empty_p (db->def_blocks))
    return;

  /* Compute the initial iterated dominance frontier.  */
  idf = find_idf (db->def_blocks, dfs);
  pruned_idf = BITMAP_ALLOC (NULL);

  if (TREE_CODE (var) == SSA_NAME)
    {
      if (update_flags == TODO_update_ssa)
	{
	  /* If doing regular SSA updates for GIMPLE registers, we are
	     only interested in IDF blocks dominated by the nearest
	     common dominator of all the definition blocks.  */
	  entry = nearest_common_dominator_for_set (CDI_DOMINATORS,
						    db->def_blocks);

	  if (entry != ENTRY_BLOCK_PTR)
	    EXECUTE_IF_SET_IN_BITMAP (idf, 0, i, bi)
	      if (BASIC_BLOCK (i) != entry
		  && dominated_by_p (CDI_DOMINATORS, BASIC_BLOCK (i), entry))
		bitmap_set_bit (pruned_idf, i);
	}
      else
	{
	  /* Otherwise, do not prune the IDF for VAR.  */
	  gcc_assert (update_flags == TODO_update_ssa_full_phi);
	  bitmap_copy (pruned_idf, idf);
	}
    }
  else
    {
      /* Otherwise, VAR is a symbol that needs to be put into SSA form
	 for the first time, so we need to compute the full IDF for
	 it.  */
      bitmap_copy (pruned_idf, idf);
    }

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
	    if (e->src->index >= 0)
	      bitmap_set_bit (blocks, e->src->index);
	}

      insert_phi_nodes_for (var, pruned_idf, true);
    }

  BITMAP_FREE (pruned_idf);
  BITMAP_FREE (idf);
}


/* Heuristic to determine whether SSA name mappings for virtual names
   should be discarded and their symbols rewritten from scratch.  When
   there is a large number of mappings for virtual names, the
   insertion of PHI nodes for the old names in the mappings takes
   considerable more time than if we inserted PHI nodes for the
   symbols instead.

   Currently the heuristic takes these stats into account:

   	- Number of mappings for virtual SSA names.
	- Number of distinct virtual symbols involved in those mappings.

   If the number of virtual mappings is much larger than the number of
   virtual symbols, then it will be faster to compute PHI insertion
   spots for the symbols.  Even if this involves traversing the whole
   CFG, which is what happens when symbols are renamed from scratch.  */

static bool
switch_virtuals_to_full_rewrite_p (void)
{
  if (update_ssa_stats.num_virtual_mappings < (unsigned) MIN_VIRTUAL_MAPPINGS)
    return false;

  if (update_ssa_stats.num_virtual_mappings
      > (unsigned) VIRTUAL_MAPPINGS_TO_SYMS_RATIO
        * update_ssa_stats.num_virtual_symbols)
    return true;

  return false;
}


/* Remove every virtual mapping and mark all the affected virtual
   symbols for renaming.  */

static void
switch_virtuals_to_full_rewrite (void)
{
  unsigned i = 0;
  sbitmap_iterator sbi;

  if (dump_file)
    {
      fprintf (dump_file, "\nEnabled virtual name mapping heuristic.\n");
      fprintf (dump_file, "\tNumber of virtual mappings:       %7u\n",
	       update_ssa_stats.num_virtual_mappings);
      fprintf (dump_file, "\tNumber of unique virtual symbols: %7u\n",
	       update_ssa_stats.num_virtual_symbols);
      fprintf (dump_file, "Updating FUD-chains from top of CFG will be "
	                  "faster than processing\nthe name mappings.\n\n");
    }

  /* Remove all virtual names from NEW_SSA_NAMES and OLD_SSA_NAMES.
     Note that it is not really necessary to remove the mappings from
     REPL_TBL, that would only waste time.  */
  EXECUTE_IF_SET_IN_SBITMAP (new_ssa_names, 0, i, sbi)
    if (!is_gimple_reg (ssa_name (i)))
      RESET_BIT (new_ssa_names, i);

  EXECUTE_IF_SET_IN_SBITMAP (old_ssa_names, 0, i, sbi)
    if (!is_gimple_reg (ssa_name (i)))
      RESET_BIT (old_ssa_names, i);

  bitmap_ior_into (syms_to_rename, update_ssa_stats.virtual_symbols);
}


/* While the SSA web is updated, statements that make memory stores
   may need to switch from a factored store into .MEM to a single
   store into the corresponding symbol and vice-versa.  For instance,
   suppose that we have this structure assignment before gathering
   call-clobbered variables in the aliaser:

   	# D.1528_9 = VDEF <.MEM_1(D)>
	D.1528 = g ();		--> stores { D.1528 }

   Since D.1528 is a structure, we create a VDEF for it.  And since we
   have not yet computed call-clobbered variables, we assume that the
   only symbol stored by the assignment is D.1528 itself.

   After computing call-clobbered variables, however, the store may
   get converted into a factored store:

   	# .MEM_10 = VDEF <.MEM_1(D)>
	D.1528 = g ();		--> stores { s D.1528 }

   But since we had never marked D.1528 for renaming (it was not
   necessary), the immediate uses of D.1528_9 do not get updated.  So,
   we must update them at the end.  */

static void
replace_stale_ssa_names (void)
{
  unsigned i;
  bitmap_iterator bi;
  tree new_name;

  /* If there are any .MEM names that are marked to be released, we
     need to replace their immediate uses with the default definition
     for .MEM.  Consider this 

		    struct { ... } x;
		    if (i_12 > 10)
			# .MEM_39 = VDEF <.MEM_4(D)>
			x = y;
		    else
			# .MEM_15 = VDEF <.MEM_4(D)>
			x = z;
		    endif
		    # .MEM_59 = PHI <.MEM_15, .MEM_39>

     After scalarization

		    struct { ... } x;
		    if (i_12 > 10)
			x$a_40 = y$a_39;
			x$b_41 = y$b_38;
		    else
			x$a_45 = y$a_35;
			x$b_46 = y$b_34;
		    endif
                    # .MEM_59 = PHI <.MEM_15, .MEM_39>
                    # x$a_60 = PHI <x$a_40, x$a_45>
		    # x$b_61 = PHI <x$b_41, x$b_46>

     Both .MEM_15 and .MEM_39 have disappeared and have been marked
     for removal.  But since .MEM is not a symbol that can be marked
     for renaming, the PHI node for it remains in place.  Moreover,
     because 'x' has been scalarized, there will be no uses of .MEM_59
     downstream.  However, the SSA verifier will see uses of .MEM_15
     and .MEM_39 and trigger an ICE.  By replacing both of them with
     .MEM's default definition, we placate the verifier and maintain
     the removability of this PHI node.  */
  if (names_to_release)
    {
      new_name = get_default_def_for (mem_var);
      EXECUTE_IF_SET_IN_BITMAP (names_to_release, 0, i, bi)
	{
	  /* The replacement name for every stale SSA name is the new
	     LHS of the VDEF operator in the original defining
	     statement.  */
	  tree use_stmt, old_name;
	  imm_use_iterator iter;

	  old_name = ssa_name (i);

	  /* We only care about .MEM.  All other symbols should've
	     been marked for renaming.  */
	  if (SSA_NAME_VAR (old_name) != mem_var)
	    continue;

	  FOR_EACH_IMM_USE_STMT (use_stmt, iter, old_name)
	    {
	      use_operand_p use_p;
	      FOR_EACH_IMM_USE_ON_STMT (use_p, iter)
		SET_USE (use_p, new_name);
	    }
	}
    }


  /* Replace every stale name with the new name created for the VDEF
     of its original defining statement.  */
  if (stale_ssa_names)
    EXECUTE_IF_SET_IN_BITMAP (stale_ssa_names, 0, i, bi)
      {
	/* The replacement name for every stale SSA name is the new
	   LHS of the VDEF operator in the original defining
	   statement.  */
	tree use_stmt, old_name, new_name;
	imm_use_iterator iter;

	old_name = ssa_name (i);
	new_name = VDEF_RESULT (VDEF_OPS (SSA_NAME_DEF_STMT (old_name)));

	FOR_EACH_IMM_USE_STMT (use_stmt, iter, old_name)
	  {
	    use_operand_p use_p;

	    FOR_EACH_IMM_USE_ON_STMT (use_p, iter)
	      SET_USE (use_p, new_name);
	  }

	release_ssa_name_after_update_ssa (old_name);
      }
}


/* Given a set of newly created SSA names (NEW_SSA_NAMES) and a set of
   existing SSA names (OLD_SSA_NAMES), update the SSA form so that:

   1- The names in OLD_SSA_NAMES dominated by the definitions of
      NEW_SSA_NAMES are all re-written to be reached by the
      appropriate definition from NEW_SSA_NAMES.

   2- If needed, new PHI nodes are added to the iterated dominance
      frontier of the blocks where each of NEW_SSA_NAMES are defined.

   The mapping between OLD_SSA_NAMES and NEW_SSA_NAMES is setup by
   calling register_new_name_mapping for every pair of names that the
   caller wants to replace.

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
   even be useful for the optimizers.  UPDATE_FLAGS controls various
   aspects of how update_ssa operates, see the documentation for
   TODO_update_ssa*.  */

void
update_ssa (unsigned update_flags)
{
  basic_block bb, start_bb;
  bitmap_iterator bi;
  unsigned i = 0;
  sbitmap tmp;
  bool insert_phi_p;
  sbitmap_iterator sbi;

  if (!need_ssa_update_p ())
    return;

  timevar_push (TV_TREE_SSA_INCREMENTAL);

  /* Initialize internal data needed by the renamer.  */
  init_ssa_renamer ();

  blocks_with_phis_to_rewrite = BITMAP_ALLOC (NULL);
  if (!phis_to_rewrite)
    phis_to_rewrite = VEC_alloc (tree_vec, heap, last_basic_block);
  blocks_to_update = BITMAP_ALLOC (NULL);

  /* Ensure that the dominance information is up-to-date.  */
  calculate_dominance_info (CDI_DOMINATORS);

  /* Only one update flag should be set.  */
  gcc_assert (update_flags == TODO_update_ssa
              || update_flags == TODO_update_ssa_no_phi
	      || update_flags == TODO_update_ssa_full_phi
	      || update_flags == TODO_update_ssa_only_virtuals);

  /* If we only need to update virtuals, remove all the mappings for
     real names before proceeding.  The caller is responsible for
     having dealt with the name mappings before calling update_ssa.  */
  if (update_flags == TODO_update_ssa_only_virtuals)
    {
      sbitmap_zero (old_ssa_names);
      sbitmap_zero (new_ssa_names);
      htab_empty (repl_tbl);
    }

  insert_phi_p = (update_flags != TODO_update_ssa_no_phi);

  if (insert_phi_p)
    {
      /* If the caller requested PHI nodes to be added, initialize
	 live-in information data structures (DEF_BLOCKS).  */

      /* For each SSA name N, the DEF_BLOCKS table describes where the
	 name is defined, which blocks have PHI nodes for N, and which
	 blocks have uses of N (i.e., N is live-on-entry in those
	 blocks).  */
      def_blocks = htab_create (num_ssa_names, def_blocks_hash,
				def_blocks_eq, def_blocks_free);
    }
  else
    {
      def_blocks = NULL;
    }

  /* Heuristic to avoid massive slow downs when the replacement
     mappings include lots of virtual names.  */
  if (insert_phi_p && switch_virtuals_to_full_rewrite_p ())
    switch_virtuals_to_full_rewrite ();

  /* If there are names defined in the replacement table, prepare
     definition and use sites for all the names in NEW_SSA_NAMES and
     OLD_SSA_NAMES.  */
  if (sbitmap_first_set_bit (new_ssa_names) >= 0)
    {
      prepare_names_to_update (insert_phi_p);

      /* If all the names in NEW_SSA_NAMES had been marked for
	 removal, and there are no symbols to rename, then there's
	 nothing else to do.  */
      if (sbitmap_first_set_bit (new_ssa_names) < 0
	  && bitmap_empty_p (syms_to_rename))
	goto done;
    }

  /* Next, determine the block at which to start the renaming process.  */
  if (!bitmap_empty_p (syms_to_rename))
    {
      /* If we have to rename some symbols from scratch, we need to
	 start the process at the root of the CFG.  FIXME, it should
	 be possible to determine the nearest block that had a
	 definition for each of the symbols that are marked for
	 updating.  For now this seems more work than it's worth.  */
      start_bb = ENTRY_BLOCK_PTR;

      /* Traverse the CFG looking for definitions and uses of symbols
	 in SYMS_TO_RENAME.  Mark interesting blocks and statements
	 and set local live-in information for the PHI placement
	 heuristics.  */
      prepare_block_for_update (start_bb, insert_phi_p);
    }
  else
    {
      /* Otherwise, the entry block to the region is the nearest
	 common dominator for the blocks in BLOCKS.  */
      start_bb = nearest_common_dominator_for_set (CDI_DOMINATORS,
						   blocks_to_update);
    }

  /* If requested, insert PHI nodes at the iterated dominance frontier
     of every block, creating new definitions for names in OLD_SSA_NAMES
     and for symbols in SYMS_TO_RENAME.  */
  if (insert_phi_p)
    {
      bitmap *dfs;

      /* If the caller requested PHI nodes to be added, compute
	 dominance frontiers.  */
      dfs = XNEWVEC (bitmap, last_basic_block);
      FOR_EACH_BB (bb)
	dfs[bb->index] = BITMAP_ALLOC (NULL);
      compute_dominance_frontiers (dfs);

      if (sbitmap_first_set_bit (old_ssa_names) >= 0)
	{
	  sbitmap_iterator sbi;

	  /* insert_update_phi_nodes_for will call add_new_name_mapping
	     when inserting new PHI nodes, so the set OLD_SSA_NAMES
	     will grow while we are traversing it (but it will not
	     gain any new members).  Copy OLD_SSA_NAMES to a temporary
	     for traversal.  */
	  sbitmap tmp = sbitmap_alloc (old_ssa_names->n_bits);
	  sbitmap_copy (tmp, old_ssa_names);
	  EXECUTE_IF_SET_IN_SBITMAP (tmp, 0, i, sbi)
	    insert_updated_phi_nodes_for (ssa_name (i), dfs, blocks_to_update,
	                                  update_flags);
	  sbitmap_free (tmp);
	}

      EXECUTE_IF_SET_IN_BITMAP (syms_to_rename, 0, i, bi)
	insert_updated_phi_nodes_for (referenced_var (i), dfs,
				      blocks_to_update, update_flags);

      FOR_EACH_BB (bb)
	BITMAP_FREE (dfs[bb->index]);
      free (dfs);

      /* Insertion of PHI nodes may have added blocks to the region.
	 We need to re-compute START_BB to include the newly added
	 blocks.  */
      if (start_bb != ENTRY_BLOCK_PTR)
	start_bb = nearest_common_dominator_for_set (CDI_DOMINATORS,
						     blocks_to_update);
    }

  /* Reset the current definition for name and symbol before renaming
     the sub-graph.  */
  EXECUTE_IF_SET_IN_SBITMAP (old_ssa_names, 0, i, sbi)
    set_current_def (ssa_name (i), NULL_TREE);

  EXECUTE_IF_SET_IN_BITMAP (syms_to_rename, 0, i, bi)
    set_current_def (referenced_var (i), NULL_TREE);

  /* Now start the renaming process at START_BB.  */
  tmp = sbitmap_alloc (last_basic_block);
  sbitmap_zero (tmp);
  EXECUTE_IF_SET_IN_BITMAP (blocks_to_update, 0, i, bi)
    SET_BIT (tmp, i);

  rewrite_blocks (start_bb, REWRITE_UPDATE, tmp);

  /* If the update process generated stale SSA names, their immediate
     uses need to be replaced with the new name that was created in
     their stead.  */
  if (stale_ssa_names || names_to_release)
    replace_stale_ssa_names ();

  sbitmap_free (tmp);

  /* Debugging dumps.  */
  if (dump_file)
    {
      int c;
      unsigned i;

      dump_update_ssa (dump_file);

      fprintf (dump_file, "Incremental SSA update started at block: %d\n\n",
	       start_bb->index);

      c = 0;
      EXECUTE_IF_SET_IN_BITMAP (blocks_to_update, 0, i, bi)
	c++;
      fprintf (dump_file, "Number of blocks in CFG: %d\n", last_basic_block);
      fprintf (dump_file, "Number of blocks to update: %d (%3.0f%%)\n\n",
	       c, PERCENT (c, last_basic_block));

      if (dump_flags & TDF_DETAILS)
	{
	  fprintf (dump_file, "Affected blocks: ");
	  EXECUTE_IF_SET_IN_BITMAP (blocks_to_update, 0, i, bi)
	    fprintf (dump_file, "%u ", i);
	  fprintf (dump_file, "\n");
	}

      fprintf (dump_file, "\n\n");
    }

  /* Free allocated memory.  */
done:
  in_ssa_p = true;
  EXECUTE_IF_SET_IN_BITMAP (blocks_with_phis_to_rewrite, 0, i, bi)
    {
      tree_vec phis = VEC_index (tree_vec, phis_to_rewrite, i);

      VEC_free (tree, heap, phis);
      VEC_replace (tree_vec, phis_to_rewrite, i, NULL);
    }
  BITMAP_FREE (blocks_with_phis_to_rewrite);
  BITMAP_FREE (blocks_to_update);
  delete_update_ssa ();

  timevar_pop (TV_TREE_SSA_INCREMENTAL);
}
