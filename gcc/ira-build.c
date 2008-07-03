/* Building internal representation for IRA.
   Copyright (C) 2006, 2007, 2008
   Free Software Foundation, Inc.
   Contributed by Vladimir Makarov <vmakarov@redhat.com>.

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
#include "tm_p.h"
#include "target.h"
#include "regs.h"
#include "flags.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "insn-config.h"
#include "recog.h"
#include "toplev.h"
#include "params.h"
#include "df.h"
#include "output.h"
#include "reload.h"
#include "sparseset.h"
#include "ira-int.h"

static ira_copy_t find_allocno_copy (ira_allocno_t, ira_allocno_t, rtx,
				     ira_loop_tree_node_t);

/* The root of the loop tree corresponding to the all function.  */
ira_loop_tree_node_t ira_loop_tree_root;

/* Height of the loop tree.  */
int ira_loop_tree_height;

/* All nodes representing basic blocks are referred through the
   following array.  We can not use basic block member `aux' for this
   because it is used for insertion of insns on edges.  */
ira_loop_tree_node_t ira_bb_nodes;

/* All nodes representing loops are referred through the following
   array.  */
ira_loop_tree_node_t ira_loop_nodes;

/* Map regno -> allocnos with given regno (see comments for 
   allocno member `next_regno_allocno').  */
ira_allocno_t *ira_regno_allocno_map;

/* Array of references to all allocnos.  The order number of the
   allocno corresponds to the index in the array.  Removed allocnos
   have NULL element value.  */
ira_allocno_t *ira_allocnos;

/* Sizes of the previous array.  */
int ira_allocnos_num;

/* Map conflict id -> allocno with given conflict id (see comments for
   allocno member `conflict_id').  */
ira_allocno_t *ira_conflict_id_allocno_map;

/* Array of references to all copies.  The order number of the copy
   corresponds to the index in the array.  Removed copies have NULL
   element value.  */
ira_copy_t *ira_copies;

/* Size of the previous array.  */
int ira_copies_num;

/* Bitmap of allocnos used for different purposes.  */
static bitmap allocnos_bitmap;



/* LAST_BASIC_BLOCK before generating additional insns because of live
   range splitting.  Emitting insns on a critical edge creates a new
   basic block.  */
static int last_basic_block_before_change;

/* The following function allocates the loop tree nodes.  If LOOPS_P
   is FALSE, the nodes corresponding to the loops (except the root
   which corresponds the all function) will be not allocated but nodes
   will still be allocated for basic blocks.  */
static void
create_loop_tree_nodes (bool loops_p)
{
  unsigned int i, j;
  int max_regno;
  bool skip_p;
  edge_iterator ei;
  edge e;
  VEC (edge, heap) *edges;
  loop_p loop;

  ira_bb_nodes
    = ira_allocate (sizeof (struct ira_loop_tree_node) * last_basic_block);
  last_basic_block_before_change = last_basic_block;
  for (i = 0; i < (unsigned int) last_basic_block; i++)
    {
      ira_bb_nodes[i].regno_allocno_map = NULL;
      memset (ira_bb_nodes[i].reg_pressure, 0,
	      sizeof (ira_bb_nodes[i].reg_pressure));
      ira_bb_nodes[i].mentioned_allocnos = NULL;
      ira_bb_nodes[i].modified_regnos = NULL;
      ira_bb_nodes[i].border_allocnos = NULL;
      ira_bb_nodes[i].local_copies = NULL;
    }
  ira_loop_nodes = ira_allocate (sizeof (struct ira_loop_tree_node)
				 * VEC_length (loop_p, ira_loops.larray));
  max_regno = max_reg_num ();
  for (i = 0; VEC_iterate (loop_p, ira_loops.larray, i, loop); i++)
    {
      if (loop != ira_loops.tree_root)
	{
	  ira_loop_nodes[i].regno_allocno_map = NULL;
	  if (! loops_p)
	    continue;
	  skip_p = false;
	  FOR_EACH_EDGE (e, ei, loop->header->preds)
	    if (e->src != loop->latch
		&& (e->flags & EDGE_ABNORMAL) && EDGE_CRITICAL_P (e))
	      {
		skip_p = true;
		break;
	      }
	  if (skip_p)
	    continue;
	  edges = get_loop_exit_edges (loop);
	  for (j = 0; VEC_iterate (edge, edges, j, e); j++)
	    if ((e->flags & EDGE_ABNORMAL) && EDGE_CRITICAL_P (e))
	      {
		skip_p = true;
		break;
	      }
	  VEC_free (edge, heap, edges);
	  if (skip_p)
	    continue;
	}
      ira_loop_nodes[i].regno_allocno_map
	= ira_allocate (sizeof (ira_allocno_t) * max_regno);
      memset (ira_loop_nodes[i].regno_allocno_map, 0,
	      sizeof (ira_allocno_t) * max_regno);
      memset (ira_loop_nodes[i].reg_pressure, 0,
	      sizeof (ira_loop_nodes[i].reg_pressure));
      ira_loop_nodes[i].mentioned_allocnos = ira_allocate_bitmap ();
      ira_loop_nodes[i].modified_regnos = ira_allocate_bitmap ();
      ira_loop_nodes[i].border_allocnos = ira_allocate_bitmap ();
      ira_loop_nodes[i].local_copies = ira_allocate_bitmap ();
    }
}

/* Free the loop tree node of a loop.  */
static void
finish_loop_tree_node (ira_loop_tree_node_t loop)
{
  if (loop->regno_allocno_map != NULL)
    {
      ira_assert (loop->bb == NULL);
      ira_free_bitmap (loop->local_copies);
      ira_free_bitmap (loop->border_allocnos);
      ira_free_bitmap (loop->modified_regnos);
      ira_free_bitmap (loop->mentioned_allocnos);
      ira_free (loop->regno_allocno_map);
      loop->regno_allocno_map = NULL;
    }
}

/* Free the loop tree nodes.  */
static void
finish_loop_tree_nodes (void)
{
  unsigned int i;
  loop_p loop;

  for (i = 0; VEC_iterate (loop_p, ira_loops.larray, i, loop); i++)
    finish_loop_tree_node (&ira_loop_nodes[i]);
  ira_free (ira_loop_nodes);
  for (i = 0; i < (unsigned int) last_basic_block_before_change; i++)
    {
      if (ira_bb_nodes[i].local_copies != NULL)
	ira_free_bitmap (ira_bb_nodes[i].local_copies);
      if (ira_bb_nodes[i].border_allocnos != NULL)
	ira_free_bitmap (ira_bb_nodes[i].border_allocnos);
      if (ira_bb_nodes[i].modified_regnos != NULL)
	ira_free_bitmap (ira_bb_nodes[i].modified_regnos);
      if (ira_bb_nodes[i].mentioned_allocnos != NULL)
	ira_free_bitmap (ira_bb_nodes[i].mentioned_allocnos);
      if (ira_bb_nodes[i].regno_allocno_map != NULL)
	ira_free (ira_bb_nodes[i].regno_allocno_map);
    }
  ira_free (ira_bb_nodes);
}



/* The following recursive function adds LOOP to the loop tree
   hierarchy.  LOOP is added only once.  */
static void
add_loop_to_tree (struct loop *loop)
{
  struct loop *parent;
  ira_loop_tree_node_t loop_node, parent_node;

  /* We can not use loop node access macros here because of potential
     checking and because the nodes are not initialized enough
     yet.  */
  if (loop_outer (loop) != NULL)
    add_loop_to_tree (loop_outer (loop));
  if (ira_loop_nodes[loop->num].regno_allocno_map != NULL
      && ira_loop_nodes[loop->num].children == NULL)
    {
      /* We have not added loop node to the tree yet.  */
      loop_node = &ira_loop_nodes[loop->num];
      loop_node->loop = loop;
      loop_node->bb = NULL;
      for (parent = loop_outer (loop);
	   parent != NULL;
	   parent = loop_outer (parent))
	if (ira_loop_nodes[parent->num].regno_allocno_map != NULL)
	  break;
      if (parent == NULL)
	{
	  loop_node->next = NULL;
	  loop_node->subloop_next = NULL;
	  loop_node->parent = NULL;
	}
      else
	{
	  parent_node = &ira_loop_nodes[parent->num];
	  loop_node->next = parent_node->children;
	  parent_node->children = loop_node;
	  loop_node->subloop_next = parent_node->subloops;
	  parent_node->subloops = loop_node;
	  loop_node->parent = parent_node;
	}
    }
}

/* The following recursive function sets up levels of nodes of the
   tree given its root LOOP_NODE.  The enumeration starts with LEVEL.
   The function returns maximal value of level in the tree + 1.  */
static int
setup_loop_tree_level (ira_loop_tree_node_t loop_node, int level)
{
  int height, max_height;
  ira_loop_tree_node_t subloop_node;

  ira_assert (loop_node->bb == NULL);
  loop_node->level = level;
  max_height = level + 1;
  for (subloop_node = loop_node->subloops;
       subloop_node != NULL;
       subloop_node = subloop_node->subloop_next)
    {
      ira_assert (subloop_node->bb == NULL);
      height = setup_loop_tree_level (subloop_node, level + 1);
      if (height > max_height)
	max_height = height;
    }
  return max_height;
}

/* Create the loop tree.  The algorithm is designed to provide correct
   order of loops (they are ordered by their last loop BB) and basic
   blocks in the chain formed by member next.  */
static void
form_loop_tree (void)
{
  unsigned int i;
  basic_block bb;
  struct loop *parent;
  ira_loop_tree_node_t bb_node, loop_node;
  loop_p loop;

  /* We can not use loop/bb node access macros because of potential
     checking and because the nodes are not initialized enough
     yet.  */
  for (i = 0; VEC_iterate (loop_p, ira_loops.larray, i, loop); i++)
     if (ira_loop_nodes[i].regno_allocno_map != NULL)
       {
	 ira_loop_nodes[i].children = NULL;
	 ira_loop_nodes[i].subloops = NULL;
       }
  FOR_EACH_BB_REVERSE (bb)
    {
      bb_node = &ira_bb_nodes[bb->index];
      bb_node->bb = bb;
      bb_node->loop = NULL;
      bb_node->subloops = NULL;
      bb_node->children = NULL;
      bb_node->subloop_next = NULL;
      bb_node->next = NULL;
      for (parent = bb->loop_father;
	   parent != NULL;
	   parent = loop_outer (parent))
	if (ira_loop_nodes[parent->num].regno_allocno_map != NULL)
	  break;
      add_loop_to_tree (parent);
      loop_node = &ira_loop_nodes[parent->num];
      bb_node->next = loop_node->children;
      bb_node->parent = loop_node;
      loop_node->children = bb_node;
    }
  ira_loop_tree_root = IRA_LOOP_NODE_BY_INDEX (ira_loops.tree_root->num);
  ira_loop_tree_height = setup_loop_tree_level (ira_loop_tree_root, 0);
  ira_assert (ira_loop_tree_root->regno_allocno_map != NULL);
}



/* Rebuild IRA_REGNO_ALLOCNO_MAP and REGNO_ALLOCNO_MAPs of the loop
   tree nodes.  */
static void
rebuild_regno_allocno_maps (void)
{
  unsigned int l;
  int max_regno, regno;
  ira_allocno_t a;
  ira_loop_tree_node_t loop_tree_node;
  loop_p loop;
  ira_allocno_iterator ai;

  max_regno = max_reg_num ();
  for (l = 0; VEC_iterate (loop_p, ira_loops.larray, l, loop); l++)
    if (ira_loop_nodes[l].regno_allocno_map != NULL)
      {
	ira_free (ira_loop_nodes[l].regno_allocno_map);
	ira_loop_nodes[l].regno_allocno_map
	  = ira_allocate (sizeof (ira_allocno_t) * max_regno);
	memset (ira_loop_nodes[l].regno_allocno_map, 0,
		sizeof (ira_allocno_t) * max_regno);
      }
  ira_free (ira_regno_allocno_map);
  ira_regno_allocno_map = ira_allocate (max_regno * sizeof (ira_allocno_t));
  memset (ira_regno_allocno_map, 0, max_regno * sizeof (ira_allocno_t));
  FOR_EACH_ALLOCNO (a, ai)
    {
      if (ALLOCNO_CAP_MEMBER (a) != NULL)
	/* Caps are not in the regno allocno maps.  */
	continue;
      regno = ALLOCNO_REGNO (a);
      loop_tree_node = ALLOCNO_LOOP_TREE_NODE (a);
      ALLOCNO_NEXT_REGNO_ALLOCNO (a) = ira_regno_allocno_map[regno];
      ira_regno_allocno_map[regno] = a;
      if (loop_tree_node->regno_allocno_map[regno] == NULL)
	/* Remember that we can create temporary allocnos to break
	   cycles in register shuffle.  */
	loop_tree_node->regno_allocno_map[regno] = a;
    }
}



/* Array of vectors containing calls given pseudo-register lives
   through.  */
VEC(rtx, heap) **ira_regno_calls;

/* The length of the previous array.  */
static int regno_calls_num;

/* The function initializes array of vectors containing calls
   intersected by live ranges of pseudo-registers.  */
static void
initiate_calls (void)
{
  regno_calls_num = max_reg_num () * 3 / 2;
  ira_regno_calls = ira_allocate (regno_calls_num * sizeof (VEC(rtx, heap) *));
  memset (ira_regno_calls, 0, regno_calls_num * sizeof (VEC(rtx, heap) *));
}

/* Expand the array of vectors containing calls for
   pseudo-registers.  */
static void
expand_calls (void)
{
  int max_regno = max_reg_num ();

  if (max_regno > regno_calls_num)
    {
      ira_regno_calls = ira_reallocate (ira_regno_calls,
				    max_regno * sizeof (VEC(rtx, heap) *));
      memset (ira_regno_calls + regno_calls_num, 0,
	      (max_regno - regno_calls_num) * sizeof (VEC(rtx, heap) *));
      regno_calls_num = max_regno;
    }
}

/* Remove NULL elements from vectors containing calls intersected by
   live ranges of pseudo-registers.  */
static void
compress_calls (void)
{
  int regno, curr, length, last;
  rtx *allocno_calls;

  for (regno = 0; regno < regno_calls_num; regno++)
    {
      allocno_calls = VEC_address (rtx, ira_regno_calls[regno]);
      length = VEC_length (rtx, ira_regno_calls[regno]);
      for (last = curr = 0; curr < length; curr++)
	if (allocno_calls[curr] != NULL_RTX)
	  allocno_calls[last++] = allocno_calls[curr];
      VEC_truncate (rtx, ira_regno_calls[regno], last);
    }
}

/* Add CALLs to REGNO's vector of intersected calls and returns the
   element index in the vector.  */
int
ira_add_regno_call (int regno, rtx call)
{
  int result;

  gcc_assert (regno < regno_calls_num);
  if (ira_regno_calls[regno] == NULL)
    ira_regno_calls[regno] = VEC_alloc (rtx, heap, 10);
  result = VEC_length (rtx, ira_regno_calls[regno]);
  VEC_safe_push (rtx, heap, ira_regno_calls[regno], call);
  return result;
}

/* Free the array of vectors containing calls intersected by live
   ranges of pseudo-registers.  */
static void
finish_calls (void)
{
  int i;

  for (i = 0; i < regno_calls_num; i++)
    VEC_free (rtx, heap, ira_regno_calls[i]);
  ira_free (ira_regno_calls);
}



/* Pools for allocnos and allocno live ranges.  */
static alloc_pool allocno_pool, allocno_live_range_pool;

/* Vec containing references to all created allocnos.  It is a
   container of array allocnos.  */
static VEC(ira_allocno_t,heap) *allocno_vec;

/* Vec containing references to all created allocnos.  It is a
   container of ira_conflict_id_allocno_map.  */
static VEC(ira_allocno_t,heap) *ira_conflict_id_allocno_map_vec;

/* Initialize data concerning allocnos.  */
static void
initiate_allocnos (void)
{
  allocno_live_range_pool
    = create_alloc_pool ("allocno live ranges",
			 sizeof (struct ira_allocno_live_range), 100);
  allocno_pool
    = create_alloc_pool ("allocnos", sizeof (struct ira_allocno), 100);
  allocno_vec = VEC_alloc (ira_allocno_t, heap, max_reg_num () * 2);
  ira_allocnos = NULL;
  ira_allocnos_num = 0;
  ira_conflict_id_allocno_map_vec
    = VEC_alloc (ira_allocno_t, heap, max_reg_num () * 2);
  ira_conflict_id_allocno_map = NULL;
  ira_regno_allocno_map
    = ira_allocate (max_reg_num () * sizeof (ira_allocno_t));
  memset (ira_regno_allocno_map, 0, max_reg_num () * sizeof (ira_allocno_t));
}

/* Create and return the allocno corresponding to REGNO in
   LOOP_TREE_NODE.  Add the allocno to the list of allocnos with the
   same regno if CAP_P is FALSE.  */
ira_allocno_t
ira_create_allocno (int regno, bool cap_p, ira_loop_tree_node_t loop_tree_node)
{
  ira_allocno_t a;

  a = pool_alloc (allocno_pool);
  ALLOCNO_REGNO (a) = regno;
  ALLOCNO_LOOP_TREE_NODE (a) = loop_tree_node;
  if (! cap_p)
    {
      ALLOCNO_NEXT_REGNO_ALLOCNO (a) = ira_regno_allocno_map[regno];
      ira_regno_allocno_map[regno] = a;
      if (loop_tree_node->regno_allocno_map[regno] == NULL)
	/* Remember that we can create temporary allocnos to break
	   cycles in register shuffle on region borders (see
	   ira-emit.c).  */
	loop_tree_node->regno_allocno_map[regno] = a;
    }
  ALLOCNO_CAP (a) = NULL;
  ALLOCNO_CAP_MEMBER (a) = NULL;
  ALLOCNO_NUM (a) = ira_allocnos_num;
  ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a) = NULL;
  ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) = 0;
  COPY_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a), ira_no_alloc_regs);
  COPY_HARD_REG_SET (IRA_ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a),
		     ira_no_alloc_regs);
  ALLOCNO_NREFS (a) = 0;
  ALLOCNO_FREQ (a) = 1;
  ALLOCNO_HARD_REGNO (a) = -1;
  ALLOCNO_CALL_FREQ (a) = 0;
  ALLOCNO_CALLS_CROSSED_NUM (a) = 0;
  ALLOCNO_CALLS_CROSSED_START (a) = -1;
#ifdef STACK_REGS
  ALLOCNO_NO_STACK_REG_P (a) = false;
  IRA_ALLOCNO_TOTAL_NO_STACK_REG_P (a) = false;
#endif
  ALLOCNO_MEM_OPTIMIZED_DEST (a) = NULL;
  ALLOCNO_MEM_OPTIMIZED_DEST_P (a) = false;
  ALLOCNO_SOMEWHERE_RENAMED_P (a) = false;
  ALLOCNO_CHILD_RENAMED_P (a) = false;
  ALLOCNO_DONT_REASSIGN_P (a) = false;
  ALLOCNO_IN_GRAPH_P (a) = false;
  ALLOCNO_ASSIGNED_P (a) = false;
  ALLOCNO_MAY_BE_SPILLED_P (a) = false;
  ALLOCNO_SPLAY_REMOVED_P (a) = false;
  ALLOCNO_CONFLICT_VEC_P (a) = false;
  ALLOCNO_MODE (a) = (regno < 0 ? VOIDmode : PSEUDO_REGNO_MODE (regno));
  ALLOCNO_COPIES (a) = NULL;
  ALLOCNO_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_CONFLICT_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_UPDATED_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_LEFT_CONFLICTS_NUM (a) = -1;
  ALLOCNO_COVER_CLASS (a) = NO_REGS;
  ALLOCNO_COVER_CLASS_COST (a) = 0;
  ALLOCNO_MEMORY_COST (a) = 0;
  ALLOCNO_UPDATED_MEMORY_COST (a) = 0;
  ALLOCNO_EXCESS_PRESSURE_POINTS_NUM (a) = 0;
  ALLOCNO_NEXT_BUCKET_ALLOCNO (a) = NULL;
  ALLOCNO_PREV_BUCKET_ALLOCNO (a) = NULL;
  ALLOCNO_FIRST_COALESCED_ALLOCNO (a) = a;
  ALLOCNO_NEXT_COALESCED_ALLOCNO (a) = a;
  ALLOCNO_LIVE_RANGES (a) = NULL;
  ALLOCNO_MIN (a) = INT_MAX;
  ALLOCNO_MAX (a) = -1;
  ALLOCNO_CONFLICT_ID (a) = ira_allocnos_num;
  VEC_safe_push (ira_allocno_t, heap, allocno_vec, a);
  ira_allocnos = VEC_address (ira_allocno_t, allocno_vec);
  ira_allocnos_num = VEC_length (ira_allocno_t, allocno_vec);
  VEC_safe_push (ira_allocno_t, heap, ira_conflict_id_allocno_map_vec, a);
  ira_conflict_id_allocno_map
    = VEC_address (ira_allocno_t, ira_conflict_id_allocno_map_vec);
  return a;
}

/* Set up cover class for A and update its conflict hard registers.  */
void
ira_set_allocno_cover_class (ira_allocno_t a, enum reg_class cover_class)
{
  ALLOCNO_COVER_CLASS (a) = cover_class;
  IOR_COMPL_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a),
			  reg_class_contents[cover_class]);
  IOR_COMPL_HARD_REG_SET (IRA_ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a),
			  reg_class_contents[cover_class]);
}

/* Return TRUE if the conflict vector with NUM elements is more
   profitable than conflict bit vector for A.  */
bool
ira_conflict_vector_profitable_p (ira_allocno_t a, int num)
{
  int nw;

  if (ALLOCNO_MAX (a) < ALLOCNO_MIN (a))
    /* We prefer bit vector in such case because it does not result in
       allocation.  */
    return false;

  nw = (ALLOCNO_MAX (a) - ALLOCNO_MIN (a) + IRA_INT_BITS) / IRA_INT_BITS;
  return (2 * sizeof (ira_allocno_t) * (num + 1)
	  < 3 * nw * sizeof (IRA_INT_TYPE));
}

/* Allocates and initialize the conflict vector of A for NUM
   conflicting allocnos.  */
void
ira_allocate_allocno_conflict_vec (ira_allocno_t a, int num)
{
  int size;
  ira_allocno_t *vec;

  ira_assert (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a) == NULL);
  num++; /* for NULL end marker  */
  size = sizeof (ira_allocno_t) * num;
  vec = ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a) = ira_allocate (size);
  vec[0] = NULL;
  ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) = 0;
  ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a) = size;
  ALLOCNO_CONFLICT_VEC_P (a) = true;
}

/* Allocate and initialize the conflict bit vector of A.  */
static void
allocate_allocno_conflict_bit_vec (ira_allocno_t a)
{
  unsigned int size;

  ira_assert (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a) == NULL);
  size = ((ALLOCNO_MAX (a) - ALLOCNO_MIN (a) + IRA_INT_BITS)
	  / IRA_INT_BITS * sizeof (IRA_INT_TYPE));
  ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a) = ira_allocate (size);
  memset (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a), 0, size);
  ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a) = size;
  ALLOCNO_CONFLICT_VEC_P (a) = false;
}

/* Allocate and initialize the conflict vector or conflict bit vector
   of A for NUM conflicting allocnos whatever is more profitable.  */
void
ira_allocate_allocno_conflicts (ira_allocno_t a, int num)
{
  if (ira_conflict_vector_profitable_p (a, num))
    ira_allocate_allocno_conflict_vec (a, num);
  else
    allocate_allocno_conflict_bit_vec (a);
}

/* Add A2 to the conflicts of A1.  */
static void
add_to_allocno_conflicts (ira_allocno_t a1, ira_allocno_t a2)
{
  int num;
  unsigned int size;

  if (ALLOCNO_CONFLICT_VEC_P (a1))
    {
      ira_allocno_t *vec;

      num = ALLOCNO_CONFLICT_ALLOCNOS_NUM (a1) + 2;
      if (ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a1)
	  >=  num * sizeof (ira_allocno_t))
	vec = ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1);
      else
	{
	  size = (3 * num / 2 + 1) * sizeof (ira_allocno_t);
	  vec = ira_allocate (size);
	  memcpy (vec, ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1),
		  sizeof (ira_allocno_t) * ALLOCNO_CONFLICT_ALLOCNOS_NUM (a1));
	  ira_free (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1));
	  ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1) = vec;
	  ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a1) = size;
	}
      vec[num - 2] = a2;
      vec[num - 1] = NULL;
      ALLOCNO_CONFLICT_ALLOCNOS_NUM (a1)++;
    }
  else
    {
      int nw, added_head_nw, id;
      IRA_INT_TYPE *vec;

      id = ALLOCNO_CONFLICT_ID (a2);
      vec = ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1);
      if (ALLOCNO_MIN (a1) > id)
	{
	  /* Expand head of the bit vector.  */
	  added_head_nw = (ALLOCNO_MIN (a1) - id - 1) / IRA_INT_BITS + 1;
	  nw = (ALLOCNO_MAX (a1) - ALLOCNO_MIN (a1)) / IRA_INT_BITS + 1;
	  size = (nw + added_head_nw) * sizeof (IRA_INT_TYPE);
	  if (ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a1) >= size)
	    {
	      memmove ((char *) vec + added_head_nw * sizeof (IRA_INT_TYPE),
		       vec, nw * sizeof (IRA_INT_TYPE));
	      memset (vec, 0, added_head_nw * sizeof (IRA_INT_TYPE));
	    }
	  else
	    {
	      size = (3 * (nw + added_head_nw) / 2 + 1) * sizeof (IRA_INT_TYPE);
	      vec = ira_allocate (size);
	      memcpy
		((char *) vec + added_head_nw * sizeof (IRA_INT_TYPE),
		 ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1), nw * sizeof (IRA_INT_TYPE));
	      memset (vec, 0, added_head_nw * sizeof (IRA_INT_TYPE));
	      memset ((char *) vec + (nw + added_head_nw) * sizeof (IRA_INT_TYPE),
		      0, size - (nw + added_head_nw) * sizeof (IRA_INT_TYPE));
	      ira_free (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1));
	      ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1) = vec;
	      ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a1) = size;
	    }
	  ALLOCNO_MIN (a1) -= added_head_nw * IRA_INT_BITS;
	}
      else if (ALLOCNO_MAX (a1) < id)
	{
	  nw = (id - ALLOCNO_MIN (a1)) / IRA_INT_BITS + 1;
	  size = nw * sizeof (IRA_INT_TYPE);
	  if (ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a1) < size)
	    {
	      /* Expand tail of the bit vector.  */
	      size = (3 * nw / 2 + 1) * sizeof (IRA_INT_TYPE);
	      vec = ira_allocate (size);
	      memcpy (vec, ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1),
		      ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a1));
	      memset ((char *) vec + ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a1),
		      0, size - ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a1));
	      ira_free (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1));
	      ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a1) = vec;
	      ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a1) = size;
	    }
	  ALLOCNO_MAX (a1) = id;
	}
      SET_ALLOCNO_SET_BIT (vec, id, ALLOCNO_MIN (a1), ALLOCNO_MAX (a1));
    }
}

/* Add A1 to the conflicts of A2 and vise versa.  */
void
ira_add_allocno_conflict (ira_allocno_t a1, ira_allocno_t a2)
{
  add_to_allocno_conflicts (a1, a2);
  add_to_allocno_conflicts (a2, a1);
}

/* Clear all conflicts of allocno A.  */
static void
clear_allocno_conflicts (ira_allocno_t a)
{
  if (ALLOCNO_CONFLICT_VEC_P (a))
    {
      ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) = 0;
      ((ira_allocno_t *) ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a))[0] = NULL;
    }
  else if (ALLOCNO_CONFLICT_ALLOCNO_ARRAY_SIZE (a) != 0)
    {
      int nw;

      nw = (ALLOCNO_MAX (a) - ALLOCNO_MIN (a)) / IRA_INT_BITS + 1;
      memset (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a), 0,
	      nw * sizeof (IRA_INT_TYPE));
    }
}

/* The array used to find duplications in conflict vectors of
   allocnos.  */
static int *allocno_conflict_check;

/* The value used to mark allocation presence in conflict vector of
   the current allocno.  */
static int curr_allocno_conflict_check_tick;

/* Remove duplications in conflict vector of A.  */
static void
compress_allocno_conflict_vec (ira_allocno_t a)
{
  ira_allocno_t *vec, conflict_a;
  int i, j;

  ira_assert (ALLOCNO_CONFLICT_VEC_P (a));
  vec = ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a);
  curr_allocno_conflict_check_tick++;
  for (i = j = 0; (conflict_a = vec[i]) != NULL; i++)
    {
      if (allocno_conflict_check[ALLOCNO_NUM (conflict_a)]
	  != curr_allocno_conflict_check_tick)
	{
	  allocno_conflict_check[ALLOCNO_NUM (conflict_a)]
	    = curr_allocno_conflict_check_tick;
	  vec[j++] = conflict_a;
	}
    }
  ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) = j;
  vec[j] = NULL;
}

/* Remove duplications in conflict vectors of all allocnos.  */
static void
compress_conflict_vecs (void)
{
  ira_allocno_t a;
  ira_allocno_iterator ai;

  allocno_conflict_check = ira_allocate (sizeof (int) * ira_allocnos_num);
  memset (allocno_conflict_check, 0, sizeof (int) * ira_allocnos_num);
  curr_allocno_conflict_check_tick = 0;
  FOR_EACH_ALLOCNO (a, ai)
    if (ALLOCNO_CONFLICT_VEC_P (a))
      compress_allocno_conflict_vec (a);
  ira_free (allocno_conflict_check);
}

/* This recursive function outputs allocno A and if it is a cap the
   function outputs its members.  */
void
ira_print_expanded_allocno (ira_allocno_t a)
{
  basic_block bb;

  fprintf (ira_dump_file, " a%d(r%d", ALLOCNO_NUM (a), ALLOCNO_REGNO (a));
  if ((bb = ALLOCNO_LOOP_TREE_NODE (a)->bb) != NULL)
    fprintf (ira_dump_file, ",b%d", bb->index);
  else
    fprintf (ira_dump_file, ",l%d", ALLOCNO_LOOP_TREE_NODE (a)->loop->num);
  if (ALLOCNO_CAP_MEMBER (a) != NULL)
    {
      fprintf (ira_dump_file, ":");
      ira_print_expanded_allocno (ALLOCNO_CAP_MEMBER (a));
    }
  fprintf (ira_dump_file, ")");
}

/* Create and return the cap representing allocno A in the
   parent loop.  */
static ira_allocno_t
create_cap_allocno (ira_allocno_t a)
{
  ira_allocno_t cap;
  ira_loop_tree_node_t parent;

  ira_assert (ALLOCNO_FIRST_COALESCED_ALLOCNO (a) == a
	      && ALLOCNO_NEXT_COALESCED_ALLOCNO (a) == a);
  parent = ALLOCNO_LOOP_TREE_NODE (a)->parent;
  cap = ira_create_allocno (ALLOCNO_REGNO (a), true, parent);
  ALLOCNO_MODE (cap) = ALLOCNO_MODE (a);
  ira_set_allocno_cover_class (cap, ALLOCNO_COVER_CLASS (a));
  ALLOCNO_AVAILABLE_REGS_NUM (cap) = ALLOCNO_AVAILABLE_REGS_NUM (a);
  ALLOCNO_CAP_MEMBER (cap) = a;
  bitmap_set_bit (parent->mentioned_allocnos, ALLOCNO_NUM (cap));
  ALLOCNO_CAP (a) = cap;
  ALLOCNO_COVER_CLASS_COST (cap) = ALLOCNO_COVER_CLASS_COST (a);
  ALLOCNO_MEMORY_COST (cap) = ALLOCNO_MEMORY_COST (a);
  ALLOCNO_UPDATED_MEMORY_COST (cap) = ALLOCNO_UPDATED_MEMORY_COST (a);
  if (internal_flag_ira_verbose > 2 && ira_dump_file != NULL)
    {
      fprintf (ira_dump_file, "    Creating cap ");
      ira_print_expanded_allocno (cap);
      fprintf (ira_dump_file, "\n");
    }
  return cap;
}

/* Propagates info to the CAP from its member.  We must propagate info
   which is not available at the cap creation time.  */
static void
propagate_info_to_cap (ira_allocno_t cap)
{
  int regno, conflicts_num;
  enum reg_class cover_class;
  ira_allocno_t a, conflict_allocno, conflict_parent_allocno;
  ira_allocno_t another_a, parent_a;
  ira_loop_tree_node_t parent;
  ira_copy_t cp, next_cp;
  ira_allocno_conflict_iterator aci;

  ira_assert (ALLOCNO_FIRST_COALESCED_ALLOCNO (cap) == cap
	      && ALLOCNO_NEXT_COALESCED_ALLOCNO (cap) == cap
	      && ALLOCNO_CONFLICT_ALLOCNO_ARRAY (cap) == NULL
	      && ALLOCNO_CALLS_CROSSED_NUM (cap) == 0);
  a = ALLOCNO_CAP_MEMBER (cap);
  parent = ALLOCNO_LOOP_TREE_NODE (cap);
  cover_class = ALLOCNO_COVER_CLASS (cap);
  ira_allocate_and_copy_costs
    (&ALLOCNO_HARD_REG_COSTS (cap), cover_class, ALLOCNO_HARD_REG_COSTS (a));
  ira_allocate_and_copy_costs
    (&ALLOCNO_CONFLICT_HARD_REG_COSTS (cap), cover_class,
     ALLOCNO_CONFLICT_HARD_REG_COSTS (a));
  ALLOCNO_NREFS (cap) = ALLOCNO_NREFS (a);
  ALLOCNO_FREQ (cap) = ALLOCNO_FREQ (a);
  ALLOCNO_CALL_FREQ (cap) = ALLOCNO_CALL_FREQ (a);
  IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (cap),
		    ALLOCNO_CONFLICT_HARD_REGS (a));
  IOR_HARD_REG_SET (IRA_ALLOCNO_TOTAL_CONFLICT_HARD_REGS (cap),
		    IRA_ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
  ALLOCNO_CALLS_CROSSED_NUM (cap) = ALLOCNO_CALLS_CROSSED_NUM (a);
  ALLOCNO_CALLS_CROSSED_START (cap) = ALLOCNO_CALLS_CROSSED_START (a);
#ifdef STACK_REGS
  ALLOCNO_NO_STACK_REG_P (cap) = ALLOCNO_NO_STACK_REG_P (a);
  IRA_ALLOCNO_TOTAL_NO_STACK_REG_P (cap)
    = IRA_ALLOCNO_TOTAL_NO_STACK_REG_P (a);
#endif
  /* Add copies to the cap.  */
  for (cp = ALLOCNO_COPIES (a); cp != NULL; cp = next_cp)
    {
      if (cp->first == a)
	{
	  next_cp = cp->next_first_allocno_copy;
	  another_a = cp->second;
	}
      else if (cp->second == a)
	{
	  next_cp = cp->next_second_allocno_copy;
	  another_a = cp->first;
	}
      else
	gcc_unreachable ();
      parent_a = parent->regno_allocno_map[ALLOCNO_REGNO (another_a)];
      if (parent_a == NULL)
	parent_a = ALLOCNO_CAP (another_a);
      if (parent_a != NULL
	  && find_allocno_copy (cap, parent_a,
				cp->insn, cp->loop_tree_node) == NULL)
	/* Upper level allocno might be not existing because it is not
	   mentioned or lived on the region border.  It is just living
	   on BB start of the loop.  */
	ira_add_allocno_copy (cap, parent_a, cp->freq, cp->insn,
			      cp->loop_tree_node);
    }
  if (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a) != NULL)
    {
      conflicts_num = 0;
      FOR_EACH_ALLOCNO_CONFLICT (a, conflict_allocno, aci)
	conflicts_num++;
      ira_allocate_allocno_conflicts (cap, conflicts_num);
      FOR_EACH_ALLOCNO_CONFLICT (a, conflict_allocno, aci)
	{
	  regno = ALLOCNO_REGNO (conflict_allocno);
	  conflict_parent_allocno = parent->regno_allocno_map[regno];
	  if (conflict_parent_allocno == NULL)
	    conflict_parent_allocno = ALLOCNO_CAP (conflict_allocno);
	  if (conflict_parent_allocno != NULL
	      && (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (conflict_parent_allocno)
		  != NULL))
	    ira_add_allocno_conflict (cap, conflict_parent_allocno);
	}
    }
  if (internal_flag_ira_verbose > 2 && ira_dump_file != NULL)
    {
      fprintf (ira_dump_file, "    Propagate info to cap ");
      ira_print_expanded_allocno (cap);
      if (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (cap) != NULL)
	{
	  fprintf (ira_dump_file, "\n      Conflicts:");
	  FOR_EACH_ALLOCNO_CONFLICT (cap, conflict_allocno, aci)
	    {
	      fprintf (ira_dump_file, " a%d(r%d,",
		       ALLOCNO_NUM (conflict_allocno),
		       ALLOCNO_REGNO (conflict_allocno));
	      ira_assert
		(ALLOCNO_LOOP_TREE_NODE (conflict_allocno)->bb == NULL);
	      fprintf (ira_dump_file, "l%d)",
		       ALLOCNO_LOOP_TREE_NODE (conflict_allocno)->loop->num);
	    }
	}
      fprintf (ira_dump_file, "\n");
    }
}


/* Create and return allocno live range with given attributes.  */
allocno_live_range_t
ira_create_allocno_live_range (ira_allocno_t a, int start, int finish,
			       allocno_live_range_t next)
{
  allocno_live_range_t p;

  p = pool_alloc (allocno_live_range_pool);
  p->allocno = a;
  p->start = start;
  p->finish = finish;
  p->next = next;
  return p;
}

/* Copy allocno live range R and return the result.  */
static allocno_live_range_t
copy_allocno_live_range (allocno_live_range_t r)
{
  allocno_live_range_t p;

  p = pool_alloc (allocno_live_range_pool);
  *p = *r;
  return p;
}

/* Copy allocno live range list given by its head R and return the
   result.  */
static allocno_live_range_t
copy_allocno_live_range_list (allocno_live_range_t r)
{
  allocno_live_range_t p, first, last;

  if (r == NULL)
    return NULL;
  for (first = last = NULL; r != NULL; r = r->next)
    {
      p = copy_allocno_live_range (r);
      if (first == NULL)
	first = p;
      else
	last->next = p;
      last = p;
    }
  return first;
}

/* Free allocno live range R.  */
void
ira_finish_allocno_live_range (allocno_live_range_t r)
{
  pool_free (allocno_live_range_pool, r);
}

/* Free updated register costs of allocno A.  */
void
ira_free_allocno_updated_costs (ira_allocno_t a)
{
  enum reg_class cover_class;

  cover_class = ALLOCNO_COVER_CLASS (a);
  if (ALLOCNO_UPDATED_HARD_REG_COSTS (a) != NULL)
    ira_free_cost_vector (ALLOCNO_UPDATED_HARD_REG_COSTS (a), cover_class);
  ALLOCNO_UPDATED_HARD_REG_COSTS (a) = NULL;
  if (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) != NULL)
    ira_free_cost_vector (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a),
			  cover_class);
  ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) = NULL;
}

/* Free the memory allocated for allocno A.  */
static void
finish_allocno (ira_allocno_t a)
{
  allocno_live_range_t r, next_r;
  enum reg_class cover_class = ALLOCNO_COVER_CLASS (a);

  ira_allocnos[ALLOCNO_NUM (a)] = NULL;
  ira_conflict_id_allocno_map[ALLOCNO_CONFLICT_ID (a)] = NULL;
  if (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a) != NULL)
    ira_free (ALLOCNO_CONFLICT_ALLOCNO_ARRAY (a));
  if (ALLOCNO_HARD_REG_COSTS (a) != NULL)
    ira_free_cost_vector (ALLOCNO_HARD_REG_COSTS (a), cover_class);
  if (ALLOCNO_CONFLICT_HARD_REG_COSTS (a) != NULL)
    ira_free_cost_vector (ALLOCNO_CONFLICT_HARD_REG_COSTS (a), cover_class);
  if (ALLOCNO_UPDATED_HARD_REG_COSTS (a) != NULL)
    ira_free_cost_vector (ALLOCNO_UPDATED_HARD_REG_COSTS (a), cover_class);
  if (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) != NULL)
    ira_free_cost_vector (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a),
			  cover_class);
  for (r = ALLOCNO_LIVE_RANGES (a); r != NULL; r = next_r)
    {
      next_r = r->next;
      ira_finish_allocno_live_range (r);
    }
  pool_free (allocno_pool, a);
}

/* Free the memory allocated for all allocnos.  */
static void
finish_allocnos (void)
{
  ira_allocno_t a;
  ira_allocno_iterator ai;

  FOR_EACH_ALLOCNO (a, ai)
    finish_allocno (a);
  ira_free (ira_regno_allocno_map);
  VEC_free (ira_allocno_t, heap, ira_conflict_id_allocno_map_vec);
  VEC_free (ira_allocno_t, heap, allocno_vec);
  free_alloc_pool (allocno_pool);
  free_alloc_pool (allocno_live_range_pool);
}



/* Pools for copies.  */
static alloc_pool copy_pool;

/* Vec containing references to all created copies.  It is a
   container of array ira_copies.  */
static VEC(ira_copy_t,heap) *copy_vec;

/* The function initializes data concerning allocno copies.  */
static void
initiate_copies (void)
{
  copy_pool
    = create_alloc_pool ("copies", sizeof (struct ira_allocno_copy), 100);
  copy_vec = VEC_alloc (ira_copy_t, heap, get_max_uid ());
  ira_copies = NULL;
  ira_copies_num = 0;
}

/* Return copy connecting A1 and A2 and originated from INSN of
   LOOP_TREE_NODE if any.  */
static ira_copy_t
find_allocno_copy (ira_allocno_t a1, ira_allocno_t a2, rtx insn,
		   ira_loop_tree_node_t loop_tree_node)
{
  ira_copy_t cp, next_cp;
  ira_allocno_t another_a;

  for (cp = ALLOCNO_COPIES (a1); cp != NULL; cp = next_cp)
    {
      if (cp->first == a1)
	{
	  next_cp = cp->next_first_allocno_copy;
	  another_a = cp->second;
	}
      else if (cp->second == a1)
	{
	  next_cp = cp->next_second_allocno_copy;
	  another_a = cp->first;
	}
      else
	gcc_unreachable ();
      if (another_a == a2 && cp->insn == insn
	  && cp->loop_tree_node == loop_tree_node)
	return cp;
    }
  return NULL;
}

/* Create and return copy with given attributes LOOP_TREE_NODE, FIRST,
   SECOND, FREQ, and INSN.  */
ira_copy_t
ira_create_copy (ira_allocno_t first, ira_allocno_t second, int freq, rtx insn,
		 ira_loop_tree_node_t loop_tree_node)
{
  ira_copy_t cp;

  cp = pool_alloc (copy_pool);
  cp->num = ira_copies_num;
  cp->first = first;
  cp->second = second;
  cp->freq = freq;
  cp->insn = insn;
  cp->loop_tree_node = loop_tree_node;
  VEC_safe_push (ira_copy_t, heap, copy_vec, cp);
  ira_copies = VEC_address (ira_copy_t, copy_vec);
  ira_copies_num = VEC_length (ira_copy_t, copy_vec);
  return cp;
}

/* Attach a copy CP to allocnos involved into the copy.  */
void
ira_add_allocno_copy_to_list (ira_copy_t cp)
{
  ira_allocno_t first = cp->first, second = cp->second;

  cp->prev_first_allocno_copy = NULL;
  cp->prev_second_allocno_copy = NULL;
  cp->next_first_allocno_copy = ALLOCNO_COPIES (first);
  if (cp->next_first_allocno_copy != NULL)
    {
      if (cp->next_first_allocno_copy->first == first)
	cp->next_first_allocno_copy->prev_first_allocno_copy = cp;
      else
	cp->next_first_allocno_copy->prev_second_allocno_copy = cp;
    }
  cp->next_second_allocno_copy = ALLOCNO_COPIES (second);
  if (cp->next_second_allocno_copy != NULL)
    {
      if (cp->next_second_allocno_copy->second == second)
	cp->next_second_allocno_copy->prev_second_allocno_copy = cp;
      else
	cp->next_second_allocno_copy->prev_first_allocno_copy = cp;
    }
  ALLOCNO_COPIES (first) = cp;
  ALLOCNO_COPIES (second) = cp;
}

/* Detach a copy CP from allocnos involved into the copy.  */
void
ira_remove_allocno_copy_from_list (ira_copy_t cp)
{
  ira_allocno_t first = cp->first, second = cp->second;
  ira_copy_t prev, next;

  next = cp->next_first_allocno_copy;
  prev = cp->prev_first_allocno_copy;
  if (prev == NULL)
    ALLOCNO_COPIES (first) = next;
  else if (prev->first == first)
    prev->next_first_allocno_copy = next;
  else
    prev->next_second_allocno_copy = next;
  if (next != NULL)
    {
      if (next->first == first)
	next->prev_first_allocno_copy = prev;
      else
	next->prev_second_allocno_copy = prev;
    }
  cp->prev_first_allocno_copy = cp->next_first_allocno_copy = NULL;

  next = cp->next_second_allocno_copy;
  prev = cp->prev_second_allocno_copy;
  if (prev == NULL)
    ALLOCNO_COPIES (second) = next;
  else if (prev->second == second)
    prev->next_second_allocno_copy = next;
  else
    prev->next_first_allocno_copy = next;
  if (next != NULL)
    {
      if (next->second == second)
	next->prev_second_allocno_copy = prev;
      else
	next->prev_first_allocno_copy = prev;
    }
  cp->prev_second_allocno_copy = cp->next_second_allocno_copy = NULL;
}

/* Make a copy CP a canonical copy where number of the
   first allocno is less than the second one.  */
void
ira_swap_allocno_copy_ends_if_necessary (ira_copy_t cp)
{
  ira_allocno_t temp;
  ira_copy_t temp_cp;

  if (ALLOCNO_NUM (cp->first) <= ALLOCNO_NUM (cp->second))
    return;

  temp = cp->first;
  cp->first = cp->second;
  cp->second = temp;

  temp_cp = cp->prev_first_allocno_copy;
  cp->prev_first_allocno_copy = cp->prev_second_allocno_copy;
  cp->prev_second_allocno_copy = temp_cp;

  temp_cp = cp->next_first_allocno_copy;
  cp->next_first_allocno_copy = cp->next_second_allocno_copy;
  cp->next_second_allocno_copy = temp_cp;
}

/* Create (or update frequency if the copy already exists) and return
   the copy of allocnos FIRST and SECOND with frequency FREQ
   corresponding to move insn INSN (if any) and originated from
   LOOP_TREE_NODE.  */
ira_copy_t
ira_add_allocno_copy (ira_allocno_t first, ira_allocno_t second, int freq,
		      rtx insn, ira_loop_tree_node_t loop_tree_node)
{
  ira_copy_t cp;

  if ((cp = find_allocno_copy (first, second, insn, loop_tree_node)) != NULL)
    {
      cp->freq += freq;
      return cp;
    }
  cp = ira_create_copy (first, second, freq, insn, loop_tree_node);
  ira_assert (first != NULL && second != NULL);
  ira_add_allocno_copy_to_list (cp);
  ira_swap_allocno_copy_ends_if_necessary (cp);
  return cp;
}

/* Print info about copies involving allocno A into file F.  */
static void
print_allocno_copies (FILE *f, ira_allocno_t a)
{
  ira_allocno_t another_a;
  ira_copy_t cp, next_cp;

  fprintf (f, " a%d(r%d):", ALLOCNO_NUM (a), ALLOCNO_REGNO (a));
  for (cp = ALLOCNO_COPIES (a); cp != NULL; cp = next_cp)
    {
      if (cp->first == a)
	{
	  next_cp = cp->next_first_allocno_copy;
	  another_a = cp->second;
	}
      else if (cp->second == a)
	{
	  next_cp = cp->next_second_allocno_copy;
	  another_a = cp->first;
	}
      else
	gcc_unreachable ();
      fprintf (f, " cp%d:a%d(r%d)@%d", cp->num,
	       ALLOCNO_NUM (another_a), ALLOCNO_REGNO (another_a), cp->freq);
    }
  fprintf (f, "\n");
}

/* Print info about copies involving allocno A into stderr.  */
void
ira_debug_allocno_copies (ira_allocno_t a)
{
  print_allocno_copies (stderr, a);
}

/* The function frees memory allocated for copy CP.  */
static void
finish_copy (ira_copy_t cp)
{
  pool_free (copy_pool, cp);
}


/* Free memory allocated for all copies.  */
static void
finish_copies (void)
{
  ira_copy_t cp;
  ira_copy_iterator ci;

  FOR_EACH_COPY (cp, ci)
    finish_copy (cp);
  VEC_free (ira_copy_t, heap, copy_vec);
  free_alloc_pool (copy_pool);
}



/* Pools for cost vectors.  It is defined only for cover classes.  */
static alloc_pool cost_vector_pool[N_REG_CLASSES];

/* The function initiates work with hard register cost vectors.  It
   creates allocation pool for each cover class.  */
static void
initiate_cost_vectors (void)
{
  int i;
  enum reg_class cover_class;

  for (i = 0; i < ira_reg_class_cover_size; i++)
    {
      cover_class = ira_reg_class_cover[i];
      cost_vector_pool[cover_class]
	= create_alloc_pool ("cost vectors",
			     sizeof (int)
			     * ira_class_hard_regs_num[cover_class],
			     100);
    }
}

/* Allocate and return a cost vector VEC for COVER_CLASS.  */
int *
ira_allocate_cost_vector (enum reg_class cover_class)
{
  return pool_alloc (cost_vector_pool[cover_class]);
}

/* Free a cost vector VEC for COVER_CLASS.  */
void
ira_free_cost_vector (int *vec, enum reg_class cover_class)
{
  ira_assert (vec != NULL);
  pool_free (cost_vector_pool[cover_class], vec);
}

/* Finish work with hard register cost vectors.  Release allocation
   pool for each cover class.  */
static void
finish_cost_vectors (void)
{
  int i;
  enum reg_class cover_class;

  for (i = 0; i < ira_reg_class_cover_size; i++)
    {
      cover_class = ira_reg_class_cover[i];
      free_alloc_pool (cost_vector_pool[cover_class]);
    }
}



/* The current loop tree node and its regno allocno map.  */
ira_loop_tree_node_t ira_curr_loop_tree_node;
ira_allocno_t *ira_curr_regno_allocno_map;

/* This recursive function traverses loop tree with root LOOP_NODE
   calling non-null functions PREORDER_FUNC and POSTORDER_FUNC
   correspondingly in preorder and postorder.  The function sets up
   IRA_CURR_LOOP_TREE_NODE and IRA_CURR_REGNO_ALLOCNO_MAP.  If BB_P,
   basic block nodes of LOOP_NODE is also processed (before its
   subloop nodes).  */
void
ira_traverse_loop_tree (bool bb_p, ira_loop_tree_node_t loop_node,
			void (*preorder_func) (ira_loop_tree_node_t),
			void (*postorder_func) (ira_loop_tree_node_t))
{
  ira_loop_tree_node_t subloop_node;

  ira_assert (loop_node->bb == NULL);
  ira_curr_loop_tree_node = loop_node;
  ira_curr_regno_allocno_map = ira_curr_loop_tree_node->regno_allocno_map;

  if (preorder_func != NULL)
    (*preorder_func) (loop_node);
  
  if (bb_p)
    for (subloop_node = loop_node->children;
	 subloop_node != NULL;
	 subloop_node = subloop_node->next)
      if (subloop_node->bb != NULL)
	{
	  if (preorder_func != NULL)
	    (*preorder_func) (subloop_node);
  
	  if (postorder_func != NULL)
	    (*postorder_func) (subloop_node);
	}
  
  for (subloop_node = loop_node->subloops;
       subloop_node != NULL;
       subloop_node = subloop_node->subloop_next)
    {
      ira_assert (subloop_node->bb == NULL);
      ira_traverse_loop_tree (bb_p, subloop_node,
			      preorder_func, postorder_func);
    }

  ira_curr_loop_tree_node = loop_node;
  ira_curr_regno_allocno_map = ira_curr_loop_tree_node->regno_allocno_map;

  if (postorder_func != NULL)
    (*postorder_func) (loop_node);
}



/* The basic block currently being processed.  */
static basic_block curr_bb;

/* This recursive function creates allocnos corresponding to
   pseudo-registers containing in X.  True OUTPUT_P means that X is
   a lvalue.  */
static void
create_insn_allocnos (rtx x, bool output_p)
{
  int i, j;
  const char *fmt;
  enum rtx_code code = GET_CODE (x);

  if (code == REG)
    {
      int regno;

      if ((regno = REGNO (x)) >= FIRST_PSEUDO_REGISTER)
	{
	  ira_allocno_t a;

	  if ((a = ira_curr_regno_allocno_map[regno]) == NULL)
	    a = ira_create_allocno (regno, false, ira_curr_loop_tree_node);
	  
	  ALLOCNO_NREFS (a)++;
	  ALLOCNO_FREQ (a) += REG_FREQ_FROM_BB (curr_bb);
	  bitmap_set_bit (ira_curr_loop_tree_node->mentioned_allocnos,
			  ALLOCNO_NUM (a));
	  if (output_p)
	    bitmap_set_bit (ira_curr_loop_tree_node->modified_regnos, regno);
	}
      return;
    }
  else if (code == SET)
    {
      create_insn_allocnos (SET_DEST (x), true);
      create_insn_allocnos (SET_SRC (x), false);
      return;
    }
  else if (code == CLOBBER)
    {
      create_insn_allocnos (XEXP (x, 0), true);
      return;
    }
  else if (code == MEM)
    {
      create_insn_allocnos (XEXP (x, 0), false);
      return;
    }
  else if (code == PRE_DEC || code == POST_DEC || code == PRE_INC || 
	   code == POST_INC || code == POST_MODIFY || code == PRE_MODIFY)
    {
      create_insn_allocnos (XEXP (x, 0), true);
      create_insn_allocnos (XEXP (x, 0), false);
      return;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	create_insn_allocnos (XEXP (x, i), output_p);
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  create_insn_allocnos (XVECEXP (x, i, j), output_p);
    }
}

/* Create allocnos corresponding to pseudo-registers living in the
   basic block represented by the corresponding loop tree node
   BB_NODE.  */
static void
create_bb_allocnos (ira_loop_tree_node_t bb_node)
{
  basic_block bb;
  rtx insn;
  unsigned int i;
  bitmap_iterator bi;

  curr_bb = bb = bb_node->bb;
  ira_assert (bb != NULL);
  FOR_BB_INSNS (bb, insn)
    if (INSN_P (insn))
      create_insn_allocnos (PATTERN (insn), false);
  /* It might be a allocno living through from one subloop to
     another.  */
  EXECUTE_IF_SET_IN_REG_SET (DF_LR_IN (bb), FIRST_PSEUDO_REGISTER, i, bi)
    if (ira_curr_regno_allocno_map[i] == NULL)
      ira_create_allocno (i, false, ira_curr_loop_tree_node);
}

/* Create allocnos corresponding to pseudo-registers living on edge E
   (a loop entry or exit).  Also mark the allocnos as living on the
   loop border. */
static void
create_loop_allocnos (edge e)
{
  unsigned int i;
  bitmap live_in_regs, border_allocnos;
  bitmap_iterator bi;

  live_in_regs = DF_LR_IN (e->dest);
  border_allocnos = ira_curr_loop_tree_node->border_allocnos;
  EXECUTE_IF_SET_IN_REG_SET (DF_LR_OUT (e->src),
			     FIRST_PSEUDO_REGISTER, i, bi)
    if (bitmap_bit_p (live_in_regs, i))
      {
	if (ira_curr_regno_allocno_map[i] == NULL)
	  ira_create_allocno (i, false, ira_curr_loop_tree_node);
	bitmap_set_bit (border_allocnos,
			ALLOCNO_NUM (ira_curr_regno_allocno_map[i]));
      }
}

/* Create allocnos corresponding to pseudo-registers living in loop
   represented by the corresponding loop tree node LOOP_NODE.  This
   function is called by ira_traverse_loop_tree.  */
static void
create_loop_tree_node_allocnos (ira_loop_tree_node_t loop_node)
{
  if (loop_node->bb != NULL)
    create_bb_allocnos (loop_node);
  else if (loop_node != ira_loop_tree_root)
    {
      int i;
      edge_iterator ei;
      edge e;
      VEC (edge, heap) *edges;

      FOR_EACH_EDGE (e, ei, loop_node->loop->header->preds)
	if (e->src != loop_node->loop->latch)
	  create_loop_allocnos (e);
      
      edges = get_loop_exit_edges (loop_node->loop);
      for (i = 0; VEC_iterate (edge, edges, i, e); i++)
	create_loop_allocnos (e);
      VEC_free (edge, heap, edges);
    }
}

/* Create allocnos corresponding to pseudo-registers in the current
   function.  Traverse the loop tree for this.  */
static void
create_allocnos (void)
{
  int i;
  ira_allocno_t a, parent_a;
  ira_loop_tree_node_t parent;

  /* We need to process BB first to correctly link allocnos by member
     next_regno_allocno.  */
  ira_traverse_loop_tree (true, ira_loop_tree_root,
			  create_loop_tree_node_allocnos, NULL);
  if (flag_ira_algorithm != IRA_ALGORITHM_REGIONAL
      && flag_ira_algorithm != IRA_ALGORITHM_MIXED)
    return;
  /* Propagate number of references and frequencies for regional
     register allocation.  */
  for (i = max_reg_num () - 1; i >= FIRST_PSEUDO_REGISTER; i--)
    for (a = ira_regno_allocno_map[i];
	 a != NULL;
	 a = ALLOCNO_NEXT_REGNO_ALLOCNO (a))
      if ((parent = ALLOCNO_LOOP_TREE_NODE (a)->parent) != NULL
	  && (parent_a = parent->regno_allocno_map[i]) != NULL)
	{
	  ALLOCNO_NREFS (parent_a) += ALLOCNO_NREFS (a);
	  ALLOCNO_FREQ (parent_a) += ALLOCNO_FREQ (a);
	}
}



/* Set up minimal and maximal live range points for allocnos.  */
static void
setup_min_max_allocno_live_range_point (void)
{
  int i;
  ira_allocno_t a, parent_a, cap;
  ira_allocno_iterator ai;
  allocno_live_range_t r;
  ira_loop_tree_node_t parent;

  FOR_EACH_ALLOCNO (a, ai)
    {
      r = ALLOCNO_LIVE_RANGES (a);
      if (r == NULL)
	continue;
      ALLOCNO_MAX (a) = r->finish;
      for (; r->next != NULL; r = r->next)
	;
      ALLOCNO_MIN (a) = r->start;
    }
  for (i = max_reg_num () - 1; i >= FIRST_PSEUDO_REGISTER; i--)
    for (a = ira_regno_allocno_map[i];
	 a != NULL;
	 a = ALLOCNO_NEXT_REGNO_ALLOCNO (a))
      {
	if (ALLOCNO_MAX (a) < 0)
	  continue;
	ira_assert (ALLOCNO_CAP_MEMBER (a) == NULL);
	/* Accumulation of range info.  */
	if ((parent = ALLOCNO_LOOP_TREE_NODE (a)->parent) == NULL
	    || (parent_a = parent->regno_allocno_map[i]) == NULL)
	  {
	    for (cap = ALLOCNO_CAP (a); cap != NULL; cap = ALLOCNO_CAP (cap))
	      {
		if (ALLOCNO_MAX (cap) < ALLOCNO_MAX (a))
		  ALLOCNO_MAX (cap) = ALLOCNO_MAX (a);
		if (ALLOCNO_MIN (cap) > ALLOCNO_MIN (a))
		  ALLOCNO_MIN (cap) = ALLOCNO_MIN (a);
	      }
	    continue;
	  }
	if (ALLOCNO_MAX (parent_a) < ALLOCNO_MAX (a))
	  ALLOCNO_MAX (parent_a) = ALLOCNO_MAX (a);
	if (ALLOCNO_MIN (parent_a) > ALLOCNO_MIN (a))
	  ALLOCNO_MIN (parent_a) = ALLOCNO_MIN (a);
      }
#ifdef ENABLE_IRA_CHECKING
  FOR_EACH_ALLOCNO (a, ai)
    {
      if ((0 <= ALLOCNO_MIN (a) && ALLOCNO_MIN (a) <= ira_max_point)
	  && (0 <= ALLOCNO_MAX (a) && ALLOCNO_MAX (a) <= ira_max_point))
	continue;
      gcc_unreachable ();
    }
#endif
}

/* Sort allocnos according to their live ranges.  Allocnos with
   smaller cover class are put first.  Allocnos with the same cove
   class are ordered according their start (min).  Allocnos with the
   same start are ordered according their finish (max).  */
static int
allocno_range_compare_func (const void *v1p, const void *v2p)
{
  int diff;
  ira_allocno_t a1 = *(const ira_allocno_t *) v1p;
  ira_allocno_t a2 = *(const ira_allocno_t *) v2p;

  if ((diff = ALLOCNO_COVER_CLASS (a1) - ALLOCNO_COVER_CLASS (a2)) != 0)
    return diff;
  if ((diff = ALLOCNO_MIN (a1) - ALLOCNO_MIN (a2)) != 0)
    return diff;
  if ((diff = ALLOCNO_MAX (a1) - ALLOCNO_MAX (a2)) != 0)
     return diff;
  return ALLOCNO_NUM (a1) - ALLOCNO_NUM (a2);
}

/* Sort ira_conflict_id_allocno_map and set up conflict id of
   allocnos.  */
static void
sort_conflict_id_allocno_map (void)
{
  int i, num;
  ira_allocno_t a;
  ira_allocno_iterator ai;

  num = 0;
  FOR_EACH_ALLOCNO (a, ai)
    ira_conflict_id_allocno_map[num++] = a;
  qsort (ira_conflict_id_allocno_map, num, sizeof (ira_allocno_t),
	 allocno_range_compare_func);
  for (i = 0; i < num; i++)
    if ((a = ira_conflict_id_allocno_map[i]) != NULL)
      ALLOCNO_CONFLICT_ID (a) = i;
  for (i = num; i < ira_allocnos_num; i++)
    ira_conflict_id_allocno_map[i] = NULL;
}

/* Set up minimal and maximal conflict ids of allocnos with which
   given allocno can conflict.  */
static void
setup_min_max_conflict_allocno_ids (void)
{
  enum reg_class cover_class;
  int i, j, min, max, start, finish, first_not_finished, filled_area_start;
  int *live_range_min, *last_lived;
  ira_allocno_t a;

  live_range_min = ira_allocate (sizeof (int) * ira_allocnos_num);
  cover_class = -1;
  first_not_finished = -1;
  for (i = 0; i < ira_allocnos_num; i++)
    {
      a = ira_conflict_id_allocno_map[i];
      if (a == NULL)
	continue;
      if (cover_class != ALLOCNO_COVER_CLASS (a))
	{
	  cover_class = ALLOCNO_COVER_CLASS (a);
	  min = i;
	  first_not_finished = i;
	}
      else
	{
	  start = ALLOCNO_MIN (a);
	  /* If we skip an allocno, the allocno with smaller ids will
	     be also skipped because of the secondary sorting the
	     range finishes (see function
	     allocno_range_compare_func).  */
	  while (first_not_finished < i
		 && start > ALLOCNO_MAX (ira_conflict_id_allocno_map
					 [first_not_finished]))
	    first_not_finished++;
	  min = first_not_finished;
	}	  
      if (min == i)
	/* We could increase min further in this case but it is good
	   enough.  */
	min++;
      live_range_min[i] = ALLOCNO_MIN (a);
      ALLOCNO_MIN (a) = min;
    }
  last_lived = ira_allocate (sizeof (int) * ira_max_point);
  cover_class = -1;
  filled_area_start = -1;
  for (i = ira_allocnos_num - 1; i >= 0; i--)
    {
      a = ira_conflict_id_allocno_map[i];
      if (a == NULL)
	continue;
      if (cover_class != ALLOCNO_COVER_CLASS (a))
	{
	  cover_class = ALLOCNO_COVER_CLASS (a);
	  for (j = 0; j < ira_max_point; j++)
	    last_lived[j] = -1;
	  filled_area_start = ira_max_point;
	}
      min = live_range_min[i];
      finish = ALLOCNO_MAX (a);
      max = last_lived[finish];
      if (max < 0)
	/* We could decrease max further in this case but it is good
	   enough.  */
	max = ALLOCNO_CONFLICT_ID (a) - 1;
      ALLOCNO_MAX (a) = max;
      /* In filling, we can go further A range finish to recognize
	 intersection quickly because if the finish of subsequently
	 processed allocno (it has smaller conflict id) range is
	 further A range finish than they are definitely intersected
	 (the reason for this is the allocnos with bigger conflict id
	 have their range starts not smaller than allocnos with
	 smaller ids.  */
      for (j = min; j < filled_area_start; j++)
	last_lived[j] = i;
      filled_area_start = min;
    }
  ira_free (last_lived);
  ira_free (live_range_min);
}



/* Create caps representing allocnos living only inside the loop given
   by LOOP_NODE on higher loop level.  */
static void
create_loop_tree_node_caps (ira_loop_tree_node_t loop_node)
{
  unsigned int i;
  bitmap_iterator bi;
  ira_loop_tree_node_t parent;

  if (loop_node == ira_loop_tree_root)
    return;
  ira_assert (loop_node->bb == NULL);
  bitmap_and_compl (allocnos_bitmap, loop_node->mentioned_allocnos,
		    loop_node->border_allocnos);
  parent = loop_node->parent;
  EXECUTE_IF_SET_IN_BITMAP (allocnos_bitmap, 0, i, bi)
    if (parent->regno_allocno_map[ALLOCNO_REGNO (ira_allocnos[i])] == NULL)
      create_cap_allocno (ira_allocnos[i]);
}

/* Propagate info (not available at the cap creation time) to caps
   mentioned in LOOP_NODE.  */
static void
propagate_info_to_loop_tree_node_caps (ira_loop_tree_node_t loop_node)
{
  unsigned int i;
  bitmap_iterator bi;
  ira_allocno_t a;

  ira_assert (loop_node->bb == NULL);
  EXECUTE_IF_SET_IN_BITMAP (loop_node->mentioned_allocnos, 0, i, bi)
    {
      a = ira_allocnos[i];
      if (ALLOCNO_CAP_MEMBER (a) != NULL)
	propagate_info_to_cap (a);
    }
}



/* The page contains code transforming more one region internal
   representation (IR) to one region IR which is necessary for reload.
   This transformation is called IR flattening.  We might just rebuild
   the IR for one region but we don't do it because it takes a lot of
   time.  */

/* Merge ranges R1 and R2 and returns the result.  The function
   maintains the order of ranges and tries to minimize number of the
   result ranges.  */
static allocno_live_range_t 
merge_ranges (allocno_live_range_t r1, allocno_live_range_t r2)
{
  allocno_live_range_t first, last, temp;

  if (r1 == NULL)
    return r2;
  if (r2 == NULL)
    return r1;
  for (first = last = NULL; r1 != NULL && r2 != NULL;)
    {
      if (r1->start < r2->start)
	{
	  temp = r1;
	  r1 = r2;
	  r2 = temp;
	}
      if (r1->start <= r2->finish + 1)
	{
	  /* Intersected ranges: merge r1 and r2 into r1.  */
	  r1->start = r2->start;
	  if (r1->finish < r2->finish)
	    r1->finish = r2->finish;
	  temp = r2;
	  r2 = r2->next;
	  ira_finish_allocno_live_range (temp);
	  if (r2 == NULL)
	    {
	      /* To try to merge with subsequent ranges in r1.  */
	      r2 = r1->next;
	      r1->next = NULL;
	    }
	}
      else
	{
	  /* Add r1 to the result.  */
	  if (first == NULL)
	    first = last = r1;
	  else
	    {
	      last->next = r1;
	      last = r1;
	    }
	  r1 = r1->next;
	  if (r1 == NULL)
	    {
	      /* To try to merge with subsequent ranges in r2.  */
	      r1 = r2->next;
	      r2->next = NULL;
	    }
	}
    }
  if (r1 != NULL)
    {
      if (first == NULL)
	first = r1;
      else
	last->next = r1;
      ira_assert (r1->next == NULL);
    }
  else if (r2 != NULL)
    {
      if (first == NULL)
	first = r2;
      else
	last->next = r2;
      ira_assert (r2->next == NULL);
    }
  else
    {
      ira_assert (last->next == NULL);
    }
  return first;
}

/* This recursive function returns immediate common dominator of two
   loop tree nodes N1 and N2.  */
static ira_loop_tree_node_t
common_loop_tree_node_dominator (ira_loop_tree_node_t n1,
				 ira_loop_tree_node_t n2)
{
  ira_assert (n1 != NULL && n2 != NULL);
  if (n1 == n2)
    return n1;
  if (n1->level < n2->level)
    return common_loop_tree_node_dominator (n1, n2->parent);
  else if (n1->level > n2->level)
    return common_loop_tree_node_dominator (n1->parent, n2);
  else
    return common_loop_tree_node_dominator (n1->parent, n2->parent);
}

/* The function changes allocno in range list given by R onto A.  */
static void
change_allocno_in_range_list (allocno_live_range_t r, ira_allocno_t a)
{
  for (; r != NULL; r = r->next)
    r->allocno = a;
}

/* Flatten the IR.  In other words, this function transforms IR as if
   it were built with one region (without loops).  We could make it
   much simpler by rebuilding IR with one region, but unfortunately it
   takes a lot of time.  MAX_REGNO_BEFORE_EMIT and
   IRA_MAX_POINT_BEFORE_EMIT are correspondingly MAX_REG_NUM () and
   IRA_MAX_POINT before emitting insns on the loop borders.  */
void
ira_flattening (int max_regno_before_emit, int ira_max_point_before_emit)
{
  int i, j, num;
  bool propagate_p, stop_p, keep_p;
  int hard_regs_num;
  bool new_allocnos_p, renamed_p, merged_p;
  unsigned int n;
  enum reg_class cover_class;
  rtx call, *allocno_calls;
  ira_allocno_t a, parent_a, first, second, node_first, node_second;
  ira_allocno_t dominator_a;
  ira_copy_t cp;
  ira_loop_tree_node_t parent, node, dominator;
  allocno_live_range_t r;
  ira_allocno_iterator ai;
  ira_copy_iterator ci;
  sparseset allocnos_live;
  /* Map: regno -> allocnos which will finally represent the regno for
     IR with one region.  */
  ira_allocno_t *regno_top_level_allocno_map;
  bool *allocno_propagated_p;

  regno_top_level_allocno_map
    = ira_allocate (max_reg_num () * sizeof (ira_allocno_t));
  memset (regno_top_level_allocno_map, 0,
	  max_reg_num () * sizeof (ira_allocno_t));
  allocno_propagated_p = ira_allocate (ira_allocnos_num * sizeof (bool));
  memset (allocno_propagated_p, 0, ira_allocnos_num * sizeof (bool));
  expand_calls ();
  new_allocnos_p = renamed_p = merged_p = false;
  /* Fix final allocno attributes.  */
  for (i = max_regno_before_emit - 1; i >= FIRST_PSEUDO_REGISTER; i--)
    {
      propagate_p = false;
      for (a = ira_regno_allocno_map[i];
	   a != NULL;
	   a = ALLOCNO_NEXT_REGNO_ALLOCNO (a))
	{
	  if (ALLOCNO_CAP_MEMBER (a) != NULL)
	    continue;
	  if (ALLOCNO_SOMEWHERE_RENAMED_P (a))
	    renamed_p = true;
	  if ((unsigned int) ALLOCNO_REGNO (a) != REGNO (ALLOCNO_REG (a))
	      && ALLOCNO_CALLS_CROSSED_NUM (a) != 0)
	    {
	      allocno_calls = (VEC_address (rtx,
					    ira_regno_calls[ALLOCNO_REGNO (a)])
			       + ALLOCNO_CALLS_CROSSED_START (a));
	      for (j = ALLOCNO_CALLS_CROSSED_NUM (a) - 1; j >= 0; j--)
		{
		  call = allocno_calls[j];
		  if (call == NULL_RTX)
		    continue;
		  ira_add_regno_call (REGNO (ALLOCNO_REG (a)), call);
		  allocno_calls[j] = NULL_RTX;
		}
	    }
	  if ((parent = ALLOCNO_LOOP_TREE_NODE (a)->parent) == NULL
	      || ((parent_a = parent->regno_allocno_map[ALLOCNO_REGNO (a)])
		  == NULL))
	    {
	      ALLOCNO_COPIES (a) = NULL;
	      regno_top_level_allocno_map[REGNO (ALLOCNO_REG (a))] = a;
	      continue;
	    }
	  ira_assert (ALLOCNO_CAP_MEMBER (parent_a) == NULL);
	  if (propagate_p)
	    {
	      if (!allocno_propagated_p [ALLOCNO_NUM (parent_a)])
		COPY_HARD_REG_SET (IRA_ALLOCNO_TOTAL_CONFLICT_HARD_REGS (parent_a),
				   ALLOCNO_CONFLICT_HARD_REGS (parent_a));
	      IOR_HARD_REG_SET (IRA_ALLOCNO_TOTAL_CONFLICT_HARD_REGS (parent_a),
				IRA_ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
#ifdef STACK_REGS
	      if (!allocno_propagated_p [ALLOCNO_NUM (parent_a)])
		IRA_ALLOCNO_TOTAL_NO_STACK_REG_P (parent_a)
		  = ALLOCNO_NO_STACK_REG_P (parent_a);
	      IRA_ALLOCNO_TOTAL_NO_STACK_REG_P (parent_a)
		= (IRA_ALLOCNO_TOTAL_NO_STACK_REG_P (parent_a)
		   || IRA_ALLOCNO_TOTAL_NO_STACK_REG_P (a));
#endif
	      allocno_propagated_p [ALLOCNO_NUM (parent_a)] = true;
	    }
	  if (REGNO (ALLOCNO_REG (a)) == REGNO (ALLOCNO_REG (parent_a)))
	    {
	      if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
		{
		  fprintf (ira_dump_file,
			   "      Moving ranges of a%dr%d to a%dr%d: ",
			   ALLOCNO_NUM (a), REGNO (ALLOCNO_REG (a)),
			   ALLOCNO_NUM (parent_a),
			   REGNO (ALLOCNO_REG (parent_a)));
		  ira_print_live_range_list (ira_dump_file,
					     ALLOCNO_LIVE_RANGES (a));
		}
	      change_allocno_in_range_list (ALLOCNO_LIVE_RANGES (a), parent_a);
	      ALLOCNO_LIVE_RANGES (parent_a)
		= merge_ranges (ALLOCNO_LIVE_RANGES (a),
				ALLOCNO_LIVE_RANGES (parent_a));
	      merged_p = true;
	      ALLOCNO_LIVE_RANGES (a) = NULL;
	      ALLOCNO_MEM_OPTIMIZED_DEST_P (parent_a)
		= (ALLOCNO_MEM_OPTIMIZED_DEST_P (parent_a)
		   || ALLOCNO_MEM_OPTIMIZED_DEST_P (a));
	      continue;
	    }
	  new_allocnos_p = true;
	  propagate_p = true;
	  first = ALLOCNO_MEM_OPTIMIZED_DEST (a) == NULL ? NULL : a;
	  for (;;)
	    {
	      if (first == NULL
		  && ALLOCNO_MEM_OPTIMIZED_DEST (parent_a) != NULL)
		first = parent_a;
	      ALLOCNO_NREFS (parent_a) -= ALLOCNO_NREFS (a);
	      ALLOCNO_FREQ (parent_a) -= ALLOCNO_FREQ (a);
	      ALLOCNO_CALL_FREQ (parent_a) -= ALLOCNO_CALL_FREQ (a);
	      cover_class = ALLOCNO_COVER_CLASS (parent_a);
	      hard_regs_num = ira_class_hard_regs_num[cover_class];
	      if (ALLOCNO_HARD_REG_COSTS (a) != NULL
		  && ALLOCNO_HARD_REG_COSTS (parent_a) != NULL)
		for (j = 0; j < hard_regs_num; j++)
		  ALLOCNO_HARD_REG_COSTS (parent_a)[j]
		    -= ALLOCNO_HARD_REG_COSTS (a)[j];
	      if (ALLOCNO_CONFLICT_HARD_REG_COSTS (a) != NULL
		  && ALLOCNO_CONFLICT_HARD_REG_COSTS (parent_a) != NULL)
		for (j = 0; j < hard_regs_num; j++)
		  ALLOCNO_CONFLICT_HARD_REG_COSTS (parent_a)[j]
		    -= ALLOCNO_CONFLICT_HARD_REG_COSTS (a)[j];
	      ALLOCNO_COVER_CLASS_COST (parent_a)
		-= ALLOCNO_COVER_CLASS_COST (a);
	      ALLOCNO_MEMORY_COST (parent_a) -= ALLOCNO_MEMORY_COST (a);
	      ALLOCNO_EXCESS_PRESSURE_POINTS_NUM (parent_a)
		-= ALLOCNO_EXCESS_PRESSURE_POINTS_NUM (a);
	      if ((parent = ALLOCNO_LOOP_TREE_NODE (parent_a)->parent) == NULL
		  || (parent_a = (parent->regno_allocno_map
				  [ALLOCNO_REGNO (parent_a)])) == NULL)
		break;
	    }
	  if (first != NULL)
	    {
	      parent_a = ALLOCNO_MEM_OPTIMIZED_DEST (first);
	      dominator = common_loop_tree_node_dominator
		          (ALLOCNO_LOOP_TREE_NODE (parent_a),
			   ALLOCNO_LOOP_TREE_NODE (first));
	      dominator_a = dominator->regno_allocno_map[ALLOCNO_REGNO (a)];
	      ira_assert (parent_a != NULL);
	      stop_p = first != a;
	      /* Remember that exit can be to a grandparent (not only
		 to a parent) or a child of the grandparent.  */
	      for (first = a;;)
		{
		  if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
		    {
		      fprintf
			(ira_dump_file,
			 "      Coping ranges of a%dr%d to a%dr%d: ",
			 ALLOCNO_NUM (first), REGNO (ALLOCNO_REG (first)),
			 ALLOCNO_NUM (parent_a),
			 REGNO (ALLOCNO_REG (parent_a)));
		      ira_print_live_range_list (ira_dump_file,
						 ALLOCNO_LIVE_RANGES (first));
		    }
		  r = copy_allocno_live_range_list (ALLOCNO_LIVE_RANGES
						    (first));
		  change_allocno_in_range_list (r, parent_a);
		  ALLOCNO_LIVE_RANGES (parent_a)
		    = merge_ranges (r, ALLOCNO_LIVE_RANGES (parent_a));
		  merged_p = true;
		  if (stop_p)
		    break;
		  parent = ALLOCNO_LOOP_TREE_NODE (first)->parent;
		  ira_assert (parent != NULL);
		  first = parent->regno_allocno_map[ALLOCNO_REGNO (a)];
		  ira_assert (first != NULL);
		  if (first == dominator_a)
		    break;
		}
	    }
	  ALLOCNO_COPIES (a) = NULL;
	  regno_top_level_allocno_map[REGNO (ALLOCNO_REG (a))] = a;
	}
    }
  ira_free (allocno_propagated_p);
  ira_assert (new_allocnos_p || renamed_p
	      || ira_max_point_before_emit == ira_max_point);
  if (new_allocnos_p)
    {
      /* Fix final allocnos attributes concerning calls.  */
      compress_calls ();
      FOR_EACH_ALLOCNO (a, ai)
	{
	  if (a != regno_top_level_allocno_map[REGNO (ALLOCNO_REG (a))]
	      || ALLOCNO_CAP_MEMBER (a) != NULL)
	    continue;
	  ALLOCNO_CALLS_CROSSED_START (a) = 0;
	  ALLOCNO_CALLS_CROSSED_NUM (a)
	    = VEC_length (rtx, ira_regno_calls[REGNO (ALLOCNO_REG (a))]);
	}
    }
  if (merged_p || ira_max_point_before_emit != ira_max_point)
    ira_rebuild_start_finish_chains ();
  /* We should rebuild conflicts even if there are no new allocnos in
     situation when a pseudo used locally in loops and locally in the
     subloop because some allocnos are in conflict with the subloop
     allocno and they finally should be in conflict with the loop
     allocno.  */
  if (new_allocnos_p || renamed_p)
    {
      /* Rebuild conflicts.  */
      FOR_EACH_ALLOCNO (a, ai)
	{
	  if (a != regno_top_level_allocno_map[REGNO (ALLOCNO_REG (a))]
	      || ALLOCNO_CAP_MEMBER (a) != NULL)
	    continue;
	  for (r = ALLOCNO_LIVE_RANGES (a); r != NULL; r = r->next)
	    ira_assert (r->allocno == a);
	  clear_allocno_conflicts (a);
	}
      allocnos_live = sparseset_alloc (ira_allocnos_num);
      for (i = 0; i < ira_max_point; i++)
	{
	  for (r = ira_start_point_ranges[i]; r != NULL; r = r->start_next)
	    {
	      a = r->allocno;
	      if (a != regno_top_level_allocno_map[REGNO (ALLOCNO_REG (a))]
		  || ALLOCNO_CAP_MEMBER (a) != NULL)
		continue;
	      num = ALLOCNO_NUM (a);
	      cover_class = ALLOCNO_COVER_CLASS (a);
	      sparseset_set_bit (allocnos_live, num);
	      EXECUTE_IF_SET_IN_SPARSESET (allocnos_live, n)
		{
		  ira_allocno_t live_a = ira_allocnos[n];

		  if (cover_class == ALLOCNO_COVER_CLASS (live_a)
		      /* Don't set up conflict for the allocno with itself.  */
		      && num != (int) n)
		    ira_add_allocno_conflict (a, live_a);
		}
	    }
	  
	  for (r = ira_finish_point_ranges[i]; r != NULL; r = r->finish_next)
	    sparseset_clear_bit (allocnos_live, ALLOCNO_NUM (r->allocno));
	}
      sparseset_free (allocnos_live);
      compress_conflict_vecs ();
    }
  /* Mark some copies for removing and change allocnos in the rest
     copies.  */
  FOR_EACH_COPY (cp, ci)
    {
      if (ALLOCNO_CAP_MEMBER (cp->first) != NULL
	  || ALLOCNO_CAP_MEMBER (cp->second) != NULL)
	{
	  if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
	    fprintf
	      (ira_dump_file, "      Remove cp%d:%c%dr%d-%c%dr%d\n",
	       cp->num, ALLOCNO_CAP_MEMBER (cp->first) != NULL ? 'c' : 'a',
	       ALLOCNO_NUM (cp->first), REGNO (ALLOCNO_REG (cp->first)),
	       ALLOCNO_CAP_MEMBER (cp->second) != NULL ? 'c' : 'a',
	       ALLOCNO_NUM (cp->second), REGNO (ALLOCNO_REG (cp->second)));
	  cp->loop_tree_node = NULL;
	  continue;
	}
      first = regno_top_level_allocno_map[REGNO (ALLOCNO_REG (cp->first))];
      second = regno_top_level_allocno_map[REGNO (ALLOCNO_REG (cp->second))];
      node = cp->loop_tree_node;
      if (node == NULL)
	keep_p = true; /* It copy generated in ira-emit.c.  */
      else
	{
	  /* Check that the copy was not propagated from level on
	     which we will have different pseudos.  */
	  node_first = node->regno_allocno_map[ALLOCNO_REGNO (cp->first)];
	  node_second = node->regno_allocno_map[ALLOCNO_REGNO (cp->second)];
	  keep_p = ((REGNO (ALLOCNO_REG (first))
		     == REGNO (ALLOCNO_REG (node_first)))
		     && (REGNO (ALLOCNO_REG (second))
			 == REGNO (ALLOCNO_REG (node_second))));
	}
      if (keep_p)
	{
	  cp->loop_tree_node = ira_loop_tree_root;
	  cp->first = first;
	  cp->second = second;
	}
      else
	{
	  cp->loop_tree_node = NULL;
	  if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
	    fprintf (ira_dump_file, "      Remove cp%d:a%dr%d-a%dr%d\n",
		     cp->num, ALLOCNO_NUM (cp->first),
		     REGNO (ALLOCNO_REG (cp->first)), ALLOCNO_NUM (cp->second),
		     REGNO (ALLOCNO_REG (cp->second)));
	}
    }
  /* Remove unnecessary allocnos on lower levels of the loop tree.  */
  FOR_EACH_ALLOCNO (a, ai)
    {
      if (a != regno_top_level_allocno_map[REGNO (ALLOCNO_REG (a))]
	  || ALLOCNO_CAP_MEMBER (a) != NULL)
	{
	  if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
	    fprintf (ira_dump_file, "      Remove a%dr%d\n",
		     ALLOCNO_NUM (a), REGNO (ALLOCNO_REG (a)));
	  finish_allocno (a);
	  continue;
	}
      ALLOCNO_LOOP_TREE_NODE (a) = ira_loop_tree_root;
      ALLOCNO_REGNO (a) = REGNO (ALLOCNO_REG (a));
      ALLOCNO_CAP (a) = NULL;
      ALLOCNO_UPDATED_MEMORY_COST (a) = ALLOCNO_MEMORY_COST (a);
      if (! ALLOCNO_ASSIGNED_P (a))
	ira_free_allocno_updated_costs (a);
      ira_assert (ALLOCNO_UPDATED_HARD_REG_COSTS (a) == NULL);
      ira_assert (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) == NULL);
    }
  /* Remove unnecessary copies.  */
  FOR_EACH_COPY (cp, ci)
    {
      if (cp->loop_tree_node == NULL)
	{
	  ira_copies[cp->num] = NULL;
	  finish_copy (cp);
	  continue;
	}
      ira_assert
	(ALLOCNO_LOOP_TREE_NODE (cp->first) == ira_loop_tree_root
	 && ALLOCNO_LOOP_TREE_NODE (cp->second) == ira_loop_tree_root);
      ira_add_allocno_copy_to_list (cp);
      ira_swap_allocno_copy_ends_if_necessary (cp);
    }
  rebuild_regno_allocno_maps ();
  ira_free (regno_top_level_allocno_map);
}



#if 0

static int all_loops = 0, high_pressure_loops = 0;
    
static void
calculate_high_pressure_loops (ira_loop_tree_node_t loop_node,
			       int *all_loops, int *high_pressure_loops)
{
  ira_loop_tree_node_t subloop_node;
  int i;
  enum reg_class class;

  (*all_loops)++;
  for (i = 0; i < ira_reg_class_cover_size; i++)
    {
      class = ira_reg_class_cover[i];
      if (loop_node->reg_pressure[class] > ira_available_class_regs[class]
	  || (loop_node->parent != NULL
	      && (loop_node->parent->reg_pressure[class]
		  > ira_available_class_regs[class])))
	break;
    }
  if (i < ira_reg_class_cover_size)
    (*high_pressure_loops)++;
  for (subloop_node = loop_node->subloops;
       subloop_node != NULL;
       subloop_node = subloop_node->subloop_next)
    {
      ira_assert (subloop_node->bb == NULL);
      calculate_high_pressure_loops (subloop_node,
				     all_loops, high_pressure_loops);
    }
}
#endif

/* Create a internal representation (IR) for IRA (allocnos, copies,
   loop tree nodes).  If LOOPS_P is FALSE the nodes corresponding to
   the loops (except the root which corresponds the all function) and
   correspondingly allocnos for the loops will be not created.  Such
   parameter value is used for Chaitin-Briggs coloring.  The function
   returns TRUE if we generate loop structure (besides nodes
   representing all function and the basic blocks) for regional
   allocation.  A true return means that we really need to flatten IR
   before the reload.  */
bool
ira_build (bool loops_p)
{
  unsigned int i;
  loop_p loop;

  df_analyze ();

  allocnos_bitmap = ira_allocate_bitmap ();
  initiate_calls ();
  initiate_cost_vectors ();
  initiate_allocnos ();
  initiate_copies ();
  create_loop_tree_nodes (loops_p);
  form_loop_tree ();
  create_allocnos ();
  ira_costs ();
  ira_create_allocno_live_ranges ();
  
  if (optimize && (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL
		   || flag_ira_algorithm == IRA_ALGORITHM_MIXED))
    {
      bitmap_clear (allocnos_bitmap);
      ira_traverse_loop_tree (false, ira_loop_tree_root, NULL,
			      create_loop_tree_node_caps);
    }
  setup_min_max_allocno_live_range_point ();
  sort_conflict_id_allocno_map ();
  setup_min_max_conflict_allocno_ids ();
  ira_build_conflicts ();
  if (internal_flag_ira_verbose > 0 && ira_dump_file != NULL)
    {
      int n, nr;
      ira_allocno_t a;
      allocno_live_range_t r;
      ira_allocno_iterator ai;

      n = 0;
      FOR_EACH_ALLOCNO (a, ai)
	n += ALLOCNO_CONFLICT_ALLOCNOS_NUM (a);
      nr = 0;
      FOR_EACH_ALLOCNO (a, ai)
	for (r = ALLOCNO_LIVE_RANGES (a); r != NULL; r = r->next)
	  nr++;
      fprintf (ira_dump_file, "  regions=%d, blocks=%d, points=%d\n",
	       VEC_length (loop_p, ira_loops.larray), n_basic_blocks,
	       ira_max_point);
      fprintf (ira_dump_file,
	       "    allocnos=%d, copies=%d, conflicts=%d, ranges=%d\n",
	       ira_allocnos_num, ira_copies_num, n, nr);
    }
  if (optimize)
    {
      if (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL
	  || flag_ira_algorithm == IRA_ALGORITHM_MIXED)
	ira_traverse_loop_tree (false, ira_loop_tree_root, NULL,
				propagate_info_to_loop_tree_node_caps);
      ira_tune_allocno_costs_and_cover_classes ();
      if (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL
	  || flag_ira_algorithm == IRA_ALGORITHM_MIXED)
	{
	  for (i = 0; VEC_iterate (loop_p, ira_loops.larray, i, loop); i++)
	    if (ira_loop_nodes[i].regno_allocno_map != NULL
		&& ira_loop_tree_root != &ira_loop_nodes[i])
	      return true;
	}
    }
  return false;
}

/* Release the data created by function ira_build.  */
void
ira_destroy (void)
{
  finish_loop_tree_nodes ();
  finish_copies ();
  finish_allocnos ();
  finish_calls ();
  finish_cost_vectors ();
  ira_finish_allocno_live_ranges ();
  ira_free_bitmap (allocnos_bitmap);
}
