/* Basic block reordering routines for the GNU compiler.
   Copyright (C) 2000 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

/* Structure to hold information about the blocks during reordering.  */
typedef struct reorder_block_def
{
  rtx header;
  rtx footer;
  basic_block next;
  basic_block original;
  /* Used by loop copying.  */
  basic_block copy;
  int duplicated;

  /* These fields are used by bb-reorder pass.  */
  int visited;
} *reorder_block_def;

#define RBI(BB)	((reorder_block_def) (BB)->aux)

/* A type for vertices in tsp solver.  */
struct vertex
{
  int n_spec;			/* Number of special edges.  */
  int spec_tgt[2];		/* Special edge targets.  */
  int spec_cost[2];		/* Special edge costs.  */
  int def_cost;			/* Cost of remaining edges.  */
};

extern void cfg_layout_initialize	PARAMS ((struct loops *));
extern void cfg_layout_finalize		PARAMS ((void));
extern bool cfg_layout_can_duplicate_bb_p PARAMS ((basic_block));
extern basic_block cfg_layout_duplicate_bb PARAMS ((basic_block, edge));
extern void scope_to_insns_initialize	PARAMS ((void));
extern void scope_to_insns_finalize	PARAMS ((void));
extern bool cfg_layout_redirect_edge	PARAMS ((edge, basic_block));
extern edge cfg_layout_split_block	PARAMS ((basic_block, rtx));
extern bool can_copy_bbs_p		PARAMS ((basic_block *, unsigned));
extern void copy_bbs	PARAMS ((basic_block *, unsigned, basic_block *,
				 edge *, unsigned, edge *, struct loop *,
				 struct loops *));

/* Maximum number of vertices of tsp instance solved (in fact 2 less, as source
   and target are not included).  */
#define MAX_TSP_SIZE 200

extern void solve_tsp (int, int *, struct vertex *, int);
