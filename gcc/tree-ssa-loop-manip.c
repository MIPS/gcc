/* High-level loop manipulation functions.
   Copyright (C) 2004 Free Software Foundation, Inc.
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "timevar.h"
#include "cfgloop.h"
#include "tree-pass.h"

/* Checks whether E is an exit edge of the MFB_REE_LOOP.  Callback for
   make_forwarder_block.  */

static struct loop *mfb_ree_loop;
static bool
mfb_redirect_exit_edges (edge e)
{
  return flow_bb_inside_loop_p (mfb_ree_loop, e->src);
}

/* Copies phi nodes in newly created copies of the LOOP.  The new blocks start
   since FIRST_NEW_BLOCK index.  PEELING is true if we were peeling
   the loop.  */

static void
copy_phi_nodes (struct loop *loop, unsigned first_new_block, bool peeling)
{
  unsigned i;
  basic_block bb, orig;
  tree phi, new_phi, def;
  edge e, new_e;
  edge latch = loop_latch_edge (loop), entry = loop_preheader_edge (loop);

  for (i = first_new_block; i < (unsigned) last_basic_block; i++)
    {
      bb = BASIC_BLOCK (i);
      orig = bb->rbi->original;

      for (phi = phi_nodes (orig); phi; phi = TREE_CHAIN (phi))
	{
	  new_phi = create_phi_node (PHI_RESULT (phi), bb);

	  if (orig == loop->header)
	    {
	      if (!bb->pred || bb->pred->pred_next)
		abort ();

	      new_e = bb->pred;
	      e = (peeling && bb->rbi->copy_number == 1
		   ? entry
		   : latch);
	      def = phi_element_for_edge (phi, e)->def;
	      add_phi_arg (&new_phi, def, new_e);
	      continue;
	    }

	  for (new_e = bb->pred; new_e; new_e = new_e->pred_next)
	    {
	      e = find_edge (new_e->src->rbi->original, orig);
	      if (!e)
		abort ();

	      def = phi_element_for_edge (phi, e)->def;
	      add_phi_arg (&new_phi, def, new_e);
	    }
	}
    }

  if (peeling)
    {
      /* Update the phi nodes in the header so that the latch value comes from
	 both edges.  */
      for (phi = phi_nodes (loop->header); phi; phi = TREE_CHAIN (phi))
	{
	  def = phi_element_for_edge (phi, latch)->def;
	  phi_element_for_edge (phi, entry)->def = def;
	}
    }
}

/* Constructs list of all ssa names defined inside LOOP.  */

static tree
collect_defs (struct loop *loop)
{
  basic_block *body = get_loop_body (loop);
  unsigned i, j;
  tree phi, stmt;
  block_stmt_iterator bsi;
  def_optype defs;
  vdef_optype vdefs;
  tree ret = NULL_TREE;

  for (i = 0; i < loop->num_nodes; i++)
    {
      for (bsi = bsi_start (body[i]); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  stmt = bsi_stmt (bsi);

	  get_stmt_operands (stmt);

	  defs = STMT_DEF_OPS (stmt);
	  for (j = 0; j < NUM_DEFS (defs); j++)
	    ret = tree_cons (NULL, DEF_OP (defs, j), ret);

	  vdefs = STMT_VDEF_OPS (stmt);
	  for (j = 0; j < NUM_VDEFS (vdefs); j++)
	    ret = tree_cons (NULL, VDEF_RESULT (vdefs, j), ret);
	}

      for (phi = phi_nodes (body[i]); phi; phi = TREE_CHAIN (phi))
	ret = tree_cons (NULL, PHI_RESULT (phi), ret);
    }

  return ret;
}

/* For each definition in DEFINITIONS allocates NDUPL + 1 copies
   (one for each duplicate of the loop body).  */

static void
allocate_new_names (tree definitions, unsigned ndupl)
{
  tree def;
  unsigned i;
  struct misc_ann_d *ann;
  tree *new_names;

  for (; definitions; definitions = TREE_CHAIN (definitions))
    {
      def = TREE_VALUE (definitions);
      ann = xmalloc (sizeof (struct misc_ann_d));
      new_names = xmalloc (sizeof (tree) * (ndupl + 1));
      ann->data = new_names;

      for (i = 0; i <= ndupl; i++)
	new_names[i] = make_ssa_name (SSA_NAME_VAR (def),
				      SSA_NAME_DEF_STMT (def));
      def->common.ann = (union tree_ann_d *) ann;
    }
}

/* Renames the variable *OP_P in statement STMT.  If DEF is true,
   *OP_P is defined by the statement.  N_COPY is the number of the
   copy of the loop body we are renaming.  */

static void
rename_op (tree *op_p, bool def, tree stmt, unsigned n_copy)
{
  struct misc_ann_d *ann;
  tree *new_names;

  if (TREE_CODE (*op_p) != SSA_NAME)
    return;

  ann = &(*op_p)->common.ann->misc;
  new_names = ann ? ann->data : NULL;

  /* Something defined outside of the loop.  */
  if (!new_names)
    return;

  /* An ordinary ssa name defined in the loop.  */

  *op_p = new_names[n_copy];
  if (def)
    SSA_NAME_DEF_STMT (*op_p) = stmt;
}

/* Renames the variables in basic block BB.  */

static void
rename_variables_in_bb (basic_block bb)
{
  tree phi;
  block_stmt_iterator bsi;
  tree stmt;
  stmt_ann_t ann;
  use_optype uses;
  vuse_optype vuses;
  def_optype defs;
  vdef_optype vdefs;
  unsigned i, nbb = bb->rbi->copy_number;
  edge e;

  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    rename_op (&PHI_RESULT (phi), true, phi, nbb);

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      stmt = bsi_stmt (bsi);
      get_stmt_operands (stmt);
      ann = stmt_ann (stmt);

      uses = USE_OPS (ann);
      for (i = 0; i < NUM_USES (uses); i++)
	rename_op (USE_OP_PTR (uses, i), false, stmt, nbb);

      defs = DEF_OPS (ann);
      for (i = 0; i < NUM_DEFS (defs); i++)
	rename_op (DEF_OP_PTR (defs, i), true, stmt, nbb);

      vuses = VUSE_OPS (ann);
      for (i = 0; i < NUM_VUSES (vuses); i++)
	rename_op (VUSE_OP_PTR (vuses, i), false, stmt, nbb);

      vdefs = VDEF_OPS (ann);
      for (i = 0; i < NUM_VDEFS (vdefs); i++)
	{
	  rename_op (VDEF_OP_PTR (vdefs, i), false, stmt, nbb);
	  rename_op (VDEF_RESULT_PTR (vdefs, i), true, stmt, nbb);
	}
    }

  for (e = bb->succ; e; e = e->succ_next)
    for (phi = phi_nodes (e->dest); phi; phi = TREE_CHAIN (phi))
      rename_op (&phi_element_for_edge (phi, e)->def, false, phi, nbb);
}

/* Renames variables in the area copied by tree_duplicate_loop_to_header_edge.
   FIRST_NEW_BLOCK is the first block in the copied area.   */

static void
rename_variables (unsigned first_new_block)
{
  unsigned i;
  basic_block bb;

  for (i = first_new_block; i < (unsigned) last_basic_block; i++)
    {
      bb = BASIC_BLOCK (i);

      rename_variables_in_bb (bb);

      if (bb->rbi->copy_number == 1)
	rename_variables_in_bb (bb->rbi->original);
    }
}

/* Releases the structures holding the new ssa names.  If FREE_VARS is true,
   also the original ssa names are released.  */

static void
free_new_names (tree definitions, bool free_vars)
{
  tree def;
  struct misc_ann_d *ann;

  for (; definitions; definitions = TREE_CHAIN (definitions))
    {
      def = TREE_VALUE (definitions);
      ann = &def->common.ann->misc;

      free (ann->data);
      free (ann);
      def->common.ann = NULL;

      if (free_vars)
	release_ssa_name (def);
    }
}

/* Sets SSA_NAME_DEF_STMT for results of all phi nodes in BB.  */

static void
set_phi_def_stmts (basic_block bb)
{
  tree phi;

  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    SSA_NAME_DEF_STMT (PHI_RESULT (phi)) = phi;
}

/* The same ad cfgloopmanip.c:duplicate_loop_to_header_edge, but also updates
   ssa.  In order to achieve this, only loops whose exits all lead to the same
   location are handled.
   
   FIXME: we create some degenerate phi nodes that could be avoided by copy
   propagating them instead.  Unfortunately this is not completely
   straightforward due to problems with constant folding.  */

bool
tree_duplicate_loop_to_header_edge (struct loop *loop, edge e,
				    struct loops *loops,
				    unsigned int ndupl, sbitmap wont_exit,
				    edge orig, edge *to_remove,
				    unsigned int *n_to_remove, int flags)
{
  unsigned first_new_block;
  basic_block exit_block = NULL, bb;
  unsigned n_exits, i;
  edge *exits = get_loop_exit_edges (loop, &n_exits);
  bool peeling = (e != loop_latch_edge (loop));
  edge latch, latch_copy, ae, oe;
  tree phi, arg, map, def;
  tree definitions, usable_outside;

  if (!(loops->state & LOOPS_HAVE_SIMPLE_LATCHES))
    return false;
  if (!(loops->state & LOOPS_HAVE_PREHEADERS))
    return false;

  for (i = 0; i < n_exits; i++)
    {
      /* Edges to exit can be safely ignored, since no uses may be reached
	 through them.  */
      if(exits[i]->dest == EXIT_BLOCK_PTR)
	continue;

      if (!exit_block)
	exit_block = exits[i]->dest;
      else if (exits[i]->dest != exit_block)
	{
	  free (exits);
	  return false;
	}
    }
  free (exits);

  /* Ensure that only exits of the loop enter the exit_block.  */
  if (exit_block)
    {
      for (ae = exit_block->pred; ae; ae = ae->pred_next)
	if (!flow_bb_inside_loop_p (loop, ae->src))
	  break;

      if (ae)
	{
	  mfb_ree_loop = loop;
	  make_forwarder_block (exit_block, mfb_redirect_exit_edges, NULL);
	  bb = exit_block->succ->dest;
	  add_bb_to_loop (bb, exit_block->loop_father);

	  if (exit_block->loop_father->latch == exit_block)
	    exit_block->loop_father->latch = bb;
	}
    }

  definitions = collect_defs (loop);
  usable_outside = NULL_TREE;
  if (exit_block)
    {
      for (arg = definitions; arg; arg = TREE_CHAIN (arg))
	{
	  def = TREE_VALUE (arg);
	  if (!dominated_by_p (CDI_DOMINATORS, exit_block,
		    	       bb_for_stmt (SSA_NAME_DEF_STMT (def))))
	    continue;

	  usable_outside = tree_cons (NULL, def, usable_outside);
	}
    }

  first_new_block = last_basic_block;
  if (!duplicate_loop_to_header_edge (loop, e, loops, ndupl, wont_exit,
				      orig, to_remove, n_to_remove, flags))
    return false;

  allocate_new_names (definitions, ndupl);

  /* Readd the removed phi args for e.  */
  latch = loop_latch_edge (loop);
  latch_copy = peeling ? loop_preheader_edge (loop) : latch;
  map = PENDING_STMT (e);
  PENDING_STMT (e) = NULL;

  for (phi = phi_nodes (loop->header), arg = map;
       phi;
       phi = TREE_CHAIN (phi), arg = TREE_CHAIN (arg))
    {
      def = TREE_VALUE (arg);
      add_phi_arg (&phi, def, latch_copy);
    }
  if (arg)
    abort ();

  if (exit_block)
    {
      /* Extend the phi nodes in exit_block.  */
      for (phi = phi_nodes (exit_block); phi; phi = TREE_CHAIN (phi))
	{
	  for (ae = exit_block->pred; ae; ae = ae->pred_next)
	    {
	      if (ae->src->rbi->copy_number == 0)
		continue;

	      oe = find_edge (ae->src->rbi->original, exit_block);
	      if (!oe)
		abort ();

	      def = phi_element_for_edge (phi, oe)->def;
	      add_phi_arg (&phi, def, ae);
	    }
	}

      /* Add phi nodes for definitions to exit_block (we could find out
	 which of them are really used outside of the loop and don't emit the
	 phi nodes for the remaining ones; for this we would however need
	 to know immediate uses).  */
      for (arg = usable_outside; arg; arg = TREE_CHAIN (arg))
	{
	  def = TREE_VALUE (arg);
	  phi = create_phi_node (def, exit_block);
	  for (ae = exit_block->pred; ae; ae = ae->pred_next)
	    add_phi_arg (&phi, def, ae);
	}
    }
  
  /* Copy the phi nodes.  */
  copy_phi_nodes (loop, first_new_block, peeling);

  /* Rename the variables.  */
  rename_variables (first_new_block);
  free_new_names (definitions, exit_block == NULL);

  /* For some time we have the identical ssa names as results in multiple phi
     nodes.  When phi node is resized, it sets SSA_NAME_DEF_STMT of its result
     to the new copy.  This means that we cannot easily ensure that the ssa
     names defined in those phis are pointing to the right one -- so just
     recompute SSA_NAME_DEF_STMT for them.  */ 

  for (i = first_new_block; i < (unsigned) last_basic_block; i++)
    {
      bb = BASIC_BLOCK (i);
      set_phi_def_stmts (bb);
      if (bb->rbi->copy_number == 1)
  	set_phi_def_stmts (bb->rbi->original);
    }
  if (exit_block)
    set_phi_def_stmts (exit_block);

  return true;
}

/* Unrolls and peels each loop twice for testing.  */

void
test_unrolling_and_peeling (struct loops *loops)
{
  struct loop *loop;
  unsigned i;

  for (i = 1; i < loops->num; i++)
    {
      loop = loops->parray[i];

      if (!loop
	  || loop->inner)
	continue;

      tree_duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
					  loops, 2, NULL, NULL, NULL, NULL, 0);
      verify_loop_structure (loops);
      verify_ssa ();

      tree_duplicate_loop_to_header_edge (loop, loop_latch_edge (loop),
					  loops, 2, NULL, NULL, NULL, NULL, 0);
      verify_loop_structure (loops);
      verify_ssa ();
    }
}
