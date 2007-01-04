/* Layout of basic blocks.

   Copyright (C) 2006, 2007 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Bona
   Andrea Ornstein
   Erven Rohou
   Roberto Costa

Contact information at STMicroelectronics:
Roberto Costa <roberto.costa@st.com>   */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "diagnostic.h"
#include "hashtab.h"
#include "tree-flow.h"
#include "langhooks.h"
#include "tree-pass.h"
#include "timevar.h"
#include "assert.h"


/* Hijacking the BB_RTL flag. Should not be used since we do not use RTL
   passes. */
#define VISITED BB_RTL

static inline void
visit_block (basic_block bb)
{
  bb->flags |= VISITED;
}

static inline bool
visited (basic_block bb)
{
  return ((bb->flags & VISITED) != 0);
}


/* used to walk the blocks in original order, so that we can detect if the
   layout changes */
static basic_block current_bb_at_pos = NULL;


/* Compute a simple layout for the CFG, placing a node after all its
   predecessors have been placed. When a block has two successors, we place
   the one with the highest probability first.

   'bb_order' is an array of basic_blocks, allocated by the caller and large
   enough to hold all the blocks. 'num_so_far' is the number of placed blocks.
*/
static void
compute_layout (basic_block bb, basic_block* bb_order, int* num_so_far,
                bool* changed)
{
  bool   all_visited;
  int    num_edges, i;
  int    edge_pos = 0, edge_dir = 1;

  visit_block (bb);
  bb_order[*num_so_far] = bb;

  /* check if we change the order */
  if (current_bb_at_pos != bb)
      *changed = true;
  current_bb_at_pos = current_bb_at_pos->next_bb;

  ++*num_so_far;

  num_edges = EDGE_COUNT (bb->succs);

  /* Consider only the case of two edges: one edge is straightforward; more
     than two is a switch, we leave it alone. */
  if (num_edges == 2)
    {
      edge e1 = EDGE_SUCC (bb, 0);
      edge e2 = EDGE_SUCC (bb, 1);
      /* In case the second edge is heavier than the first one, we invert the
         scanning order. */
      if (e1->probability < e2->probability)
        {
          edge_pos = 1;  /* start from second edge */
          edge_dir = -1; /* and go backwards */
        }
    }

  for(i = 0;  i < num_edges; ++i)
    {
      edge          pred_edge;
      edge_iterator pred_it;
      edge          e = EDGE_SUCC (bb, edge_pos);
      basic_block   succ = e->dest;

      edge_pos += edge_dir;
      if ((succ == EXIT_BLOCK_PTR) ||   /* Skip EXIT block */
          (e->flags & EDGE_DFS_BACK) || /* Skip back edges */
          (visited (succ)))             /* Already visited */
        continue;

      /* Check if successor has all its predecessors visited, not considering
         backedges. */
      all_visited = true;
      for (pred_it = ei_start (succ->preds);
           ei_cond (pred_it, &pred_edge);
           ei_next (&pred_it))
        {
          if (pred_edge->flags & EDGE_DFS_BACK)
            continue;

          if ((visited (pred_edge->src)) == 0)
            {
              all_visited = false;
              break;
            }
        }
      if (all_visited)
        compute_layout (succ, bb_order, num_so_far, changed);
    }
}


static unsigned int
bblayout (void)
{
  bool         layout_changed = false;
  int          bb_num = 0;
  int          i;
  basic_block* bb_order = NULL;

  mark_dfs_back_edges ();
  bb_order = (basic_block*) alloca (n_basic_blocks * sizeof (basic_block));
  current_bb_at_pos = ENTRY_BLOCK_PTR;
  compute_layout (ENTRY_BLOCK_PTR, bb_order, &bb_num, &layout_changed);
  bb_order[bb_num++] = EXIT_BLOCK_PTR;
  gcc_assert (bb_num == n_basic_blocks);

  if (layout_changed)
    {
      bb_order[0]->prev_bb = NULL;
      bb_order[0]->next_bb = bb_order[1];
      for(i=1; i < bb_num-1; ++i)
        {
          bb_order[i]->prev_bb = bb_order[i-1];
          bb_order[i]->next_bb = bb_order[i+1];
        }
      bb_order[bb_num-1]->prev_bb = bb_order[bb_num-2];
      bb_order[bb_num-1]->next_bb = NULL;
    }
  return 0;
}


static bool
bblayout_gate (void)
{
  return current_function_decl != NULL;
}


/* Define the parameters of the bb layout pass.  */

struct tree_opt_pass pass_bb_layout =
{
  "bblayout",                           /* name */
  bblayout_gate,                        /* gate */
  bblayout,                             /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_BB_LAYOUT,                         /* tv_id */
  PROP_cfg,                             /* properties_required */
  0,                                    /* properties_provided */
  /* ??? If TER is enabled, we also kill gimple.  */
  0,                                    /* properties_destroyed */
  0,
  0,
  0                                     /* letter */
};

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
