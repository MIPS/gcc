/* Construct SSA for ICG.
   Copyright (C) 2008  Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "flags.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "df.h"
#include "vecprim.h"
#include "vec.h"
#include "tree-flow.h"
#include "domwalk.h"
#include "icg.h"


/* Adds a phi node for register REG to the list of phi nodes for basic
   block BB. */

static void
add_phi_node (basic_block bb, int reg)
{
  icg_phi_node *phi;
  int npreds;

  npreds = EDGE_COUNT (bb->preds);
  gcc_assert (npreds >= 2);
  phi = (icg_phi_node *)icg_calloc (sizeof (icg_phi_node) + npreds * sizeof (int));
  phi->reg = reg;
  phi->next = icg_bb2phi[bb->index];
  icg_bb2phi[bb->index] = phi;
}

/* Returns true iff REG is live into basic block BB. */

static bool
register_live_in_p (int reg, basic_block bb)
{
  return REGNO_REG_SET_P (DF_LIVE_IN (bb), reg);
}

/* State for walking the dominator tree when renaming. */

struct walk_state
{
  /* Next available SSA name. */
  int next_name;
  /* Mapping from register to current SSA name. */
  int *curr_name;
  /* Stack of renames. */
  VEC (int, heap) *st;
};

/* Called each time a new def is encountered.
     1) reserves a unique SSA name for the def
     2) Save (register, old name) pair on to stack
     3) Set current name of register to new value
   Returns the SSA name for the def. */

static int
register_new_def (int reg, struct walk_state *gdata)
{
  int new_name = gdata->next_name++;

  VEC_reserve (int, heap, gdata->st, 2);
  VEC_quick_push (int, gdata->st, gdata->curr_name[reg]);
  VEC_quick_push (int, gdata->st, reg);

  gdata->curr_name[reg] = new_name;

  return new_name;
}

/* Callback function for renaming uses, called for each icg_node
   register use by for_each_icg_tree_use. */

static int
rename_uses (icg_node *pn, void *data)
{
  int reg = REGNO (pn->rtl);
  if (reg >= FIRST_PSEUDO_REGISTER)
    {
      struct walk_state *gdata = (struct walk_state *)data;
      int new_name = gdata->curr_name[reg];
      /* Assert will fail if use has no definition. */
      if (dump_file) fflush(dump_file);
      gcc_assert (new_name != 0);
      if (dump_file)
	fprintf (dump_file, "    use of register %3d in insn %3d renamed to %3d\n",
		 reg, INSN_UID (pn->insn), new_name);
      pn->ssa_name = new_name;
    }
  else
    pn->ssa_name = reg;
  return 0;
}

/* Callback function for registering and renaming defs, called for
   each icg_node register use by for_each_icg_tree_def. */

static int
register_defs (icg_node *pn, void *data)
{
  int reg = REGNO (pn->rtl);
  if (reg >= FIRST_PSEUDO_REGISTER)
    {
      struct walk_state *gdata = (struct walk_state *)data;
      int new_name = register_new_def (reg, gdata);
      if (dump_file)
	fprintf (dump_file, "    def of register %3d in insn %3d renamed to %3d\n",
		 reg, INSN_UID (pn->insn), new_name);
      pn->ssa_name = new_name;
    }
  else
    pn->ssa_name = reg;
  return 0;
}

/* Called during the dominator walk when a new block is entered:
     1) mark location on name stack for restore when leaving block
     2) register the defs of each phi node.
     3) iterate through trees amd rename uses and defs */

static void
rename_enter_block (struct dom_walk_data *walk_data, basic_block bb)
{
  struct walk_state *gdata = (struct walk_state *)walk_data->global_data;
  icg_phi_node *pn;
  icg_node *inode;

  if (dump_file)
    fprintf (dump_file, "  Entering BB %2d\n", bb->index);

  /* Save marker indicating new block. Stack will be rolled back to
     this point when leaving block. */
  VEC_safe_push (int, heap, gdata->st, -1);

  FOR_BB_ICG_PHI_NODES (bb, pn)
    {
      pn->def = register_new_def (pn->reg, gdata);
      if (dump_file)
	fprintf (dump_file, "    def in phi node for register %3d named %3d\n",
		 pn->reg, pn->def);
    }

  FOR_BB_ICG_TREES(bb, inode)
    {
      for_each_icg_tree_use (inode, rename_uses, gdata);
      for_each_icg_tree_def (inode, register_defs, gdata);
    }
}

/* Returns the index in the predecessor vector of BB for the
   predecessor PRED. */

static int
get_pred_index (basic_block bb, basic_block pred)
{
  edge e;
  edge_iterator ei;
  int n = 0;
  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      if (e->src == pred)
	return n;
      n++;
    }
  gcc_assert (0);
  return 0;
}

/* Called immediate before traversing children during dominator walk.
   Iterate successors of BB and fill in respective phi node arguments
   with current name of register. */

static void
rename_phi_arguments (struct dom_walk_data *walk_data, basic_block bb)
{
  struct walk_state *gdata = (struct walk_state *)walk_data->global_data;
  edge e;
  edge_iterator ei;
  icg_phi_node *pn;

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      int idx = get_pred_index (e->dest, bb);
      FOR_BB_ICG_PHI_NODES (e->dest, pn)
	{
	  pn->uses[idx] = gdata->curr_name[pn->reg];
	  if (dump_file)
	    fprintf (dump_file, "    arg %3d of phi for reg %3d in BB %3d set to %3d\n",
		     idx, pn->reg, e->dest->index, gdata->curr_name[pn->reg]);
	}
    }
}

/* Called when backing out of a basic block during the dominator tree walk.
     1) builds SSA variable live-out vector for the basic block
     2) Restore names to state prior to entering block by popping changes
        from stack. */

static void
rename_leave_block (struct dom_walk_data *walk_data,
		    basic_block bb ATTRIBUTE_UNUSED)
{
  unsigned int i;
  int n;
  bitmap_iterator bi;
  icg_names *p;
  bitmap liveout = DF_LIVE_OUT(bb);
  struct walk_state *gdata = (struct walk_state *)walk_data->global_data;

  /* Build SSA name live out vector. */
  p = (icg_names *)icg_calloc (sizeof (icg_names)
		  + sizeof (int) * (bitmap_count_bits (liveout) - 1));
  p->size = bitmap_count_bits (liveout);
  n=0;
  EXECUTE_IF_SET_IN_BITMAP (liveout, 0, i, bi)
    if (i < FIRST_PSEUDO_REGISTER)
      p->name[n++] = i;
    else
      p->name[n++] = gdata->curr_name[i];
  icg_ssa_liveout[bb->index] = p;

  /* Restore stack to state upon entering BB by popping until -1
     marker is found. */
  while (1)
    {
      int reg, name;

      gcc_assert (VEC_length (int, gdata->st) > 0);
      reg = VEC_pop (int, gdata->st);
      if (reg == -1)
	break;
      gcc_assert (VEC_length (int, gdata->st) > 0);
      name = VEC_pop (int, gdata->st);
      gdata->curr_name[reg] = name;
    }
}

/* Walks the dominator tree and renames uses and defs. Also constructs
   SSA live out vectors for each basic block. */

static int
rename_registers (void)
{
  struct walk_state gdata;
  struct dom_walk_data walk_data;

  gdata.next_name = FIRST_PSEUDO_REGISTER;
  gdata.curr_name = XCNEWVEC (int, max_reg_num());
  gdata.st = VEC_alloc (int, heap, 10);

  if (dump_file)
    fprintf (dump_file, "Walking dominance tree and renaming registers:\n");
  memset (&walk_data, 0, sizeof (walk_data));
  walk_data.dom_direction = CDI_DOMINATORS;
  walk_data.global_data = &gdata;

  walk_data.before_dom_children_before_stmts = rename_enter_block;
  walk_data.before_dom_children_after_stmts = rename_phi_arguments;
  walk_data.after_dom_children_after_stmts = rename_leave_block;

  init_walk_dominator_tree (&walk_data);
  walk_dominator_tree (&walk_data, ENTRY_BLOCK_PTR);
  fini_walk_dominator_tree (&walk_data);

  VEC_free (int, heap, gdata.st);
  free (gdata.curr_name);

  return gdata.next_name;
}

/* Helper struct and callback function for computing the set of basic
   blocks in which a register is defined. */

struct reg_defs_struct
{
  basic_block bb;
  bitmap *reg_defs;
};

static int
set_def_bit (icg_node *pn, void *data)
{
  struct reg_defs_struct *p = (struct reg_defs_struct *)data;
  int reg = REGNO (pn->rtl);
  if (reg >= FIRST_PSEUDO_REGISTER)
    bitmap_set_bit (p->reg_defs[reg], p->bb->index);
  return 0;
}

/* Fills in the allocated array of bitmaps REG_DEFS.  Array is indexed
   by rtl register name and indicates in which basic blocks the
   register is defined. */

static void
compute_def_bitmaps (bitmap *reg_defs)
{
  basic_block bb;
  icg_node *pn;
  struct reg_defs_struct st;

  st.reg_defs = reg_defs;
  FOR_EACH_BB (bb)
    {
      st.bb = bb;
      FOR_BB_ICG_TREES (bb, pn)
	for_each_icg_tree_def (pn, set_def_bit, &st);
    }
}

/* Computes the iterated dominance frontier in IDF of the set of basic blocks
   indicated by the bitmap DEF_BLOCKs using the dominance frontiers
   in DFRONT. (Taken from tree-into-ssa.c) */

static void
set_idf_bits (bitmap idf, bitmap def_blocks, bitmap *dfront)
{
  bitmap_iterator bi;
  unsigned bb_index, i;
  VEC(int,heap) *work_stack;

  work_stack = VEC_alloc (int, heap, n_basic_blocks);

  /* Seed the work list with all the blocks in DEF_BLOCKS.  We use
     VEC_quick_push here for speed.  This is safe because we know that
     the number of definition blocks is no greater than the number of
     basic blocks, which is the initial capacity of WORK_STACK.  */
  EXECUTE_IF_SET_IN_BITMAP (def_blocks, 0, bb_index, bi)
    VEC_quick_push (int, work_stack, bb_index);

  /* Pop a block off the worklist, add every block that appears in
     the original block's DF that we have not already processed to
     the worklist.  Iterate until the worklist is empty.   Blocks
     which are added to the worklist are potential sites for
     PHI nodes.  */
  while (VEC_length (int, work_stack) > 0)
    {
      bb_index = VEC_pop (int, work_stack);
      EXECUTE_IF_AND_COMPL_IN_BITMAP (dfront[bb_index], idf, 0, i, bi)
	{
	  /* Use a safe push because if there is a definition of REG
	     in every basic block, then WORK_STACK may eventually have
	     more than N_BASIC_BLOCK entries.  */
	  VEC_safe_push (int, heap, work_stack, i);
	  bitmap_set_bit (idf, i);
	}
    }

  VEC_free (int, heap, work_stack);
}

/* Constructs lists of phi nodes for every basic block using iterated
   dominance frontiers. */

static void
place_phi_nodes (void)
{
  int reg;
  bitmap_iterator bi;
  basic_block bb;
  unsigned i;
  bitmap *reg_defs;
  bitmap *dfront;
  bitmap idf;

  if (dump_file)
    fprintf (dump_file, "Placing phi nodes:\n");

  /* Initialize array of reg def bitmaps. */
  reg_defs = XNEWVEC (bitmap, max_reg_num());
  for (reg = 0; reg < max_reg_num(); reg++)
    reg_defs [reg] = BITMAP_ALLOC (NULL);
  compute_def_bitmaps (reg_defs);

  /* Compute dominance frontier bitmap.  */
  dfront = XNEWVEC (bitmap, last_basic_block);
  FOR_EACH_BB (bb)
    dfront[bb->index] = BITMAP_ALLOC (NULL);
  calculate_dominance_info (CDI_DOMINATORS);
  compute_dominance_frontiers (dfront);

  idf = BITMAP_ALLOC (NULL);
  for (reg = FIRST_PSEUDO_REGISTER; reg < max_reg_num(); reg++)
    {
      bitmap_clear (idf);
      set_idf_bits (idf, reg_defs[reg], dfront);

      EXECUTE_IF_SET_IN_BITMAP(idf, 0, i, bi)
	if (register_live_in_p (reg, BASIC_BLOCK (i)))
	  {
	    add_phi_node (BASIC_BLOCK (i), reg);
	    if (dump_file)
	      fprintf (dump_file, "phi node for register %3d placed in BB %3d\n",
		       reg, i);
	  }
    }
  BITMAP_FREE (idf);

  FOR_EACH_BB (bb)
    BITMAP_FREE (dfront[bb->index]);
  free (dfront);

  for (reg = 0; reg < max_reg_num(); reg++)
    BITMAP_FREE (reg_defs [reg]);
  free (reg_defs);
}

/* Constructs SSA form:
     1) Renames registers in icg tree.  New names placed in field
        ssa_name.
     2) Constructs phi node data structure icg_bb2phi, an array of lists
        of phi_node's.

   Sets icg_ssa_regs to the highest SSA name number + 1.  */

void
icg_build_ssa (void)
{
  df_live_add_problem ();
  df_scan_alloc (NULL);
  df_scan_blocks ();

  df_live_add_problem ();
  df_live_set_all_dirty ();
  df_analyze ();

  if (dump_file)
    {
      fprintf (dump_file, "\n*** icg_build_ssa ***\n\n");
      fprintf (dump_file, "RTL defs and uses:\n");
      dump_icg_rtl (dump_file);
    }

  icg_ssa_liveout = (icg_names**) icg_calloc (last_basic_block * sizeof (icg_names *));
  icg_bb2phi = (icg_phi_node **)icg_calloc (last_basic_block * sizeof (icg_phi_node *));

  place_phi_nodes ();

  icg_ssa_regs = rename_registers ();

  if (dump_file)
    {
      fprintf (dump_file, "SSA defs and uses:\n");
      dump_icg_ssa (dump_file);
    }
}


/* Data structure and functions for union-find used for merging SSA
   variables which share PHI nodes. */

struct live_range {
  struct live_range *root;
  int rank;
  unsigned int ssa_name;
  unsigned int lr_name;
};

static struct live_range *
range_find (struct live_range *lr)
{
  if (lr->root == lr)
    return lr;
  else
    {
      lr->root = range_find (lr->root);
      return lr->root;
    }
}

static void
range_union (struct live_range *x, struct live_range *y)
{
  x = range_find(x);
  y = range_find(y);
  if (x != y)
    {
      if (x->rank > y->rank)
	y->root = x;
      else if (x->rank < y->rank)
	x->root = y;
      else
	{
	  y->root = x;
	  x->rank++;
	}
    }
}

static int
name_live_range (icg_node *pn, void *data)
{
  struct live_range *ranges = (struct live_range *)data;
  struct live_range *plr = range_find (&ranges[pn->ssa_name]);
  pn->live_range = plr->lr_name;
  pn->r = pn->rx = pn->live_range;
  return 0;
}

/* Finds and names live ranges.  Unique name is placed in live_range
   field within the icg tree.  Names of live ranges of real registers
   are the same as the REGNO of the register.  Live ranges of symbolic
   registers are numbered consecutively from FIRST_PSEUDO_REGISTER.

   Sets icg_live_ranges to the maximum live range name + 1. */

void
icg_build_live_ranges (void)
{
  basic_block bb;
  struct live_range *ranges;
  unsigned i;
  icg_phi_node *pn;

  if (dump_file)
    fprintf (dump_file, "\n*** icg_build_live_ranges ***\n\n");

  icg_live_ranges = FIRST_PSEUDO_REGISTER;
  icg_ssa_to_lr = (unsigned int *)icg_calloc (icg_ssa_regs * sizeof (int));
  icg_lr_liveout = (icg_names **)icg_calloc (last_basic_block * sizeof (icg_names *));

  ranges = XNEWVEC (struct live_range, icg_ssa_regs);
  for (i = 0; i < icg_ssa_regs; i++)
    {
      ranges[i].root = &ranges[i];
      ranges[i].rank = 0;
      ranges[i].ssa_name = i;
      ranges[i].lr_name = 0;
    }

  /* Union together the live range of each arg of a phi node
     into the live range of the def. */
  FOR_EACH_BB (bb)
    FOR_BB_ICG_PHI_NODES (bb, pn)
      for (i = 0; i < EDGE_COUNT (bb->preds); i++)
	range_union (&ranges[pn->def], &ranges[pn->uses[i]]);

  /* Name each live range a unique name.  Also create ssa name to live
     range map. */
  for (i = 0; i < icg_ssa_regs; i++)
    {
      struct live_range *lr = range_find (&ranges[i]);
      if  (i < FIRST_PSEUDO_REGISTER)
	{
	  gcc_assert (lr->ssa_name == i);
	  lr->lr_name = i;
	}
      else
	{
	  gcc_assert (lr->ssa_name >= FIRST_PSEUDO_REGISTER);
	  if (lr->lr_name == 0)
	    lr->lr_name = icg_live_ranges++;
	}
      icg_ssa_to_lr[i] = lr->lr_name;
    }

  /* Fill in live_range field for each instance of live range
     variable. */
  FOR_EACH_BB (bb)
    {
      icg_node *inode;
      icg_names *p, *pssa;
      bool *chk;

      FOR_BB_ICG_TREES(bb, inode)
	{
	  for_each_icg_tree_def (inode, name_live_range, ranges);
	  for_each_icg_tree_use (inode, name_live_range, ranges);
	}

      pssa = icg_ssa_liveout[bb->index];
      p = (icg_names *)icg_calloc (sizeof (icg_names) + sizeof (int) * (pssa->size - 1));
      p->size = pssa->size;
      for (i = 0; i < pssa->size; i++)
	{
	  unsigned lr = icg_ssa_to_lr[pssa->name[i]];
	  p->name[i] = lr;
	}
      icg_lr_liveout[bb->index] = p;

      /* For verification, check that no live ranges appear twice.
	 There should be a one-to-one mapping from SSA names to
	 _unique_ live ranges for the SSA names which are live out of
	 each basic block.  I know, the check is not terribly
	 efficient. */
      chk = XCNEWVEC (bool, icg_ssa_regs);
      for (i = 0; i < p->size; i++)
	{
	  gcc_assert (!chk[p->name[i]]);
	  chk[p->name[i]] = true;
	}
      free (chk);
    }

  free (ranges);

  if (dump_file)
    {
      fprintf (dump_file, "Live range defs and uses:\n");
      dump_icg_live_ranges (dump_file);
    }
}


/* Helper functions for dumping register names.  To avoid confusion,
   RTL names are prefixed with 'R', SSA names with 'S', and live
   ranges with 'L'. */

typedef enum {
  ICG_RTL_NAME,
  ICG_SSA_NAME,
  ICG_LR_NAME
} icg_name_type;

static void
dump_reg (FILE *fp, bool space, icg_name_type t, int name)
{
  if (fp == 0) fp = stdout;
  if (space)
    fprintf (fp, " ");
  if (name < FIRST_PSEUDO_REGISTER)
    fprintf (fp, "%s", reg_names[name]);
  else
    {
      if (t == ICG_RTL_NAME)
	fprintf (fp, "R%d", name);
      else if (t == ICG_SSA_NAME)
	fprintf (fp, "S%d", name);
      else if (t== ICG_LR_NAME)
	fprintf (fp, "L%d", name);
    }	
}

static int
dump_rtl_name (icg_node *pn, void *data)
{
  FILE *fp = (FILE *)data;
  dump_reg (fp, true, ICG_RTL_NAME, REGNO (pn->rtl));
  return 0;
}

static int
dump_ssa_name (icg_node *pn, void *data)
{
  FILE *fp = (FILE *)data;
  dump_reg (fp, true, ICG_SSA_NAME, pn->ssa_name);
  return 0;
}

static int
dump_live_range_name (icg_node *pn, void *data)
{
  FILE *fp = (FILE *)data;
  dump_reg (fp, true, ICG_LR_NAME, pn->live_range);
  return 0;
}

/* Dumps the trees in each basic block in the following form:

     ( defs ...) <= ( uses ... )

   Also dumps live out for each BB.  Parameterized for dumping
   RTL/SSA/live ranges. */

static void
dump_def_uses (FILE *fp, bool ssa, bool lr)
{
  basic_block bb;
  icg_phi_node *phi;
  unsigned int i;
  edge e;
  edge_iterator ei;
  icg_node *inode;

  if (fp == 0) fp = stdout;
  df_live_add_problem ();
  df_live_set_all_dirty ();
  df_analyze ();

  FOR_EACH_BB (bb)
    {
      fprintf (fp, "  BB %d ( preds =", bb->index);
      FOR_EACH_EDGE (e, ei, bb->preds)
	fprintf (fp, " %d", e->src->index);
      fprintf (fp, " ; succs =");
      FOR_EACH_EDGE (e, ei, bb->succs)
	fprintf (fp, " %d", e->dest->index);
      fprintf (fp, " )\n");

      if (ssa)
	FOR_BB_ICG_PHI_NODES (bb, phi)
	  {
	    fprintf (fp, "    PHI ");
	    dump_reg (fp, false, ICG_RTL_NAME, phi->reg);
	    fprintf (fp, "  ( ");
	    dump_reg (fp, false, ICG_SSA_NAME, phi->def);
	    fprintf (fp, " ) <= (");
	    for (i = 0; i < EDGE_COUNT (bb->preds); i++)
	      dump_reg (fp, true, ICG_SSA_NAME, phi->uses[i]);
	    fprintf (fp, " )\n");
	  }

      FOR_BB_ICG_TREES (bb, inode)
	{
	  int i, pos;
	  int (*dump_f)(icg_node *, void *);

	  if (ssa)
	    dump_f = dump_ssa_name;
	  else if (lr)
	    dump_f = dump_live_range_name;
	  else
	    dump_f = dump_rtl_name;

	  fprintf (fp, "    insn %3d (", INSN_UID (inode->insn));
	  pos = ftell (fp);
	  for_each_icg_tree_def (inode, dump_f, fp);
	  /* Make sure line position is advanced by at least 8 so each
	     '<=' lines up for clarity.  This should give room for two
	     defs without causing misalignment problems. */
	  for (i = ftell (fp); i < pos + 4; i = ftell (fp))
	    fprintf (fp, " ");
	  fprintf (fp, " ) <= (");
	  for_each_icg_tree_use (inode, dump_f, fp);
	  fprintf (fp, " )\n");
	}

      fprintf (fp, "    live out (");
      if (ssa)
	for (i = 0; i < icg_ssa_liveout[bb->index]->size; i++)
	  dump_reg (fp, true, ICG_SSA_NAME, icg_ssa_liveout[bb->index]->name[i]);
      else if (lr)
	for (i = 0; i < icg_lr_liveout[bb->index]->size; i++)
	  dump_reg (fp, true, ICG_LR_NAME, icg_lr_liveout[bb->index]->name[i]);
      else
	{
	  bitmap_iterator bi;
	  EXECUTE_IF_SET_IN_BITMAP (DF_LIVE_OUT(bb), 0, i, bi)
	    dump_reg (fp, true, ICG_RTL_NAME, i);
	}
      fprintf (fp, " )\n");
    }
}


/* Dumps the defs and uses of the original RTL registers. */

void dump_icg_rtl (FILE *fp)
{
  dump_def_uses (fp, false, false);
}

/* Dumps the defs and uses of every operation and phi nodes to FP. */

void dump_icg_ssa (FILE *fp)
{
  dump_def_uses (fp, true, false);
}

/* Dumps the defs and uses of live ranges. */

void dump_icg_live_ranges (FILE *fp)
{
  unsigned lr, ssa;

  if (fp == 0) fp = stdout;
  /* Dump list of SSA names composing each (non-empty) live range.
     Inefficient way of doing it. */
  fprintf (fp, "  SSA names in live ranges:\n");
  for (lr = FIRST_PSEUDO_REGISTER; lr < icg_live_ranges; lr++)
    {
      bool first = true;
      for (ssa = FIRST_PSEUDO_REGISTER; ssa < icg_ssa_regs; ssa++)
	if (icg_ssa_to_lr[ssa] == lr)
	  {
	    if (first)
	      {
		fprintf (fp, "    ");
		dump_reg (fp, false, ICG_LR_NAME, lr);
		fprintf (fp, " = (");
		first = false;
	      }
	    dump_reg (fp, true, ICG_SSA_NAME, ssa);
	  }
      if (!first)
	fprintf (fp, " )\n");
    }
  dump_def_uses (fp, false, true);
}
