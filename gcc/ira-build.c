/* Building allocnos for IRA.
   Copyright (C) 2006, 2007, 2008
   Free Software Foundation, Inc.
   Contributed by Vladimir Makarov <vmakarov@redhat.com>.

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
02110-1301, USA.  */

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
#include "ira-int.h"

static void create_loop_tree_nodes (int);
static void finish_loop_tree_nodes (void);
static void add_loop_to_tree (struct loop *);
static int setup_loop_tree_level (loop_tree_node_t, int);
static void form_loop_tree (void);

static void rebuild_regno_allocno_maps (void);

static void initiate_calls (void);
static void expand_calls (void);
static void compress_calls (void);
static void finish_calls (void);

static void initiate_allocnos (void);
static void check_allocno_conflict_vec (allocno_t, int);
static void add_to_allocno_conflict_vec (allocno_t, allocno_t);
static void compress_allocno_conflict_vec (allocno_t);
static void compress_conflict_vecs (void);
static allocno_t create_cap_allocno (allocno_t);
static void propagate_info_to_cap (allocno_t);
static allocno_live_range_t copy_allocno_live_range (allocno_live_range_t);
static allocno_live_range_t
  copy_allocno_live_range_list (allocno_live_range_t);

static void finish_allocno (allocno_t);
static void finish_allocnos (void);

static void initiate_copies (void);
static copy_t find_allocno_copy (allocno_t, allocno_t, rtx, loop_tree_node_t);
static void print_allocno_copies (FILE *, allocno_t);
static void finish_copy (copy_t);
static void finish_copies (void);

static void initiate_cost_vectors (void);
static void finish_cost_vectors (void);

static void create_insn_allocnos (rtx, int);
static void create_bb_allocnos (loop_tree_node_t);
static void create_loop_allocnos (edge);
static void create_loop_tree_node_allocnos (loop_tree_node_t);
static void create_allocnos (void);

static void create_loop_tree_node_caps (loop_tree_node_t);
static void propagate_info_to_loop_tree_node_caps (loop_tree_node_t);
static allocno_live_range_t merge_ranges (allocno_live_range_t,
					  allocno_live_range_t);
static loop_tree_node_t common_loop_tree_node_dominator (loop_tree_node_t,
							 loop_tree_node_t);
static void check_and_add_conflicts (allocno_t, allocno_t *);
static void add_conflict_with_underlying_allocnos (allocno_t,
						   loop_tree_node_t, int);

/* The root of the loop tree corresponding to the all function.  */
loop_tree_node_t ira_loop_tree_root;

/* Height of the loop tree.  */
int ira_loop_tree_height;

/* All basic block data are referred through the following array.  We
   can not use member `aux' for this because it is used for insertion
   of insns on edges.  */
loop_tree_node_t ira_bb_nodes;

/* All loop data are referred through the following array.  */
loop_tree_node_t ira_loop_nodes;

/* Map regno -> allocnos.  */
allocno_t *regno_allocno_map;

/* Array of references to all allocnos and their size.  The order
   number of the allocno corresponds to the index in the array.  */
allocno_t *allocnos;
int allocnos_num;

/* Array of references to copies and its size.  The order number of
   the copy corresponds to the index in the array.  */
copy_t *copies;
int copies_num;



/* LAST_BASIC_BLOCK before generating additional insns because of live
   range splitting.  Emitting insns on a critical edge creates a new
   basic block.  */
static int last_basic_block_before_change;

/* The following function creates the loop tree nodes.  If LOOPS_P is
   zero, the nodes corresponding to the loops (except the root which
   corresponds the all function) will be not created (it will be done
   only for basic blocks).  */
static void
create_loop_tree_nodes (int loops_p)
{
  unsigned int i, j;
  int max_regno, skip_p;
  edge_iterator ei;
  edge e;
  VEC (edge, heap) *edges;
  loop_p loop;

  ira_bb_nodes
    = ira_allocate (sizeof (struct loop_tree_node) * last_basic_block);
  last_basic_block_before_change = last_basic_block;
  for (i = 0; i < (unsigned int) last_basic_block; i++)
    {
      ira_bb_nodes [i].regno_allocno_map = NULL;
      memset (ira_bb_nodes [i].reg_pressure, 0,
	      sizeof (ira_bb_nodes [i].reg_pressure));
      ira_bb_nodes [i].mentioned_allocnos = NULL;
      ira_bb_nodes [i].modified_regnos = NULL;
      ira_bb_nodes [i].border_allocnos = NULL;
      ira_bb_nodes [i].local_copies = NULL;
    }
  ira_loop_nodes = ira_allocate (sizeof (struct loop_tree_node)
				 * VEC_length (loop_p, ira_loops.larray));
  max_regno = max_reg_num ();
  for (i = 0; VEC_iterate (loop_p, ira_loops.larray, i, loop); i++)
    {
      if (loop != ira_loops.tree_root)
	{
	  ira_loop_nodes [i].regno_allocno_map = NULL;
	  if (! loops_p)
	    continue;
	  skip_p = FALSE;
	  FOR_EACH_EDGE (e, ei, loop->header->preds)
	    if (e->src != loop->latch
		&& (e->flags & EDGE_ABNORMAL) && EDGE_CRITICAL_P (e))
	      {
		skip_p = TRUE;
		break;
	      }
	  if (skip_p)
	    continue;
	  edges = get_loop_exit_edges (loop);
	  for (j = 0; VEC_iterate (edge, edges, j, e); j++)
	    if ((e->flags & EDGE_ABNORMAL) && EDGE_CRITICAL_P (e))
	      {
		skip_p = TRUE;
		break;
	      }
	  VEC_free (edge, heap, edges);
	  if (skip_p)
	    continue;
	}
      ira_loop_nodes [i].regno_allocno_map
	= ira_allocate (sizeof (allocno_t) * max_regno);
      memset (ira_loop_nodes [i].regno_allocno_map, 0,
	      sizeof (allocno_t) * max_regno);
      memset (ira_loop_nodes [i].reg_pressure, 0,
	      sizeof (ira_loop_nodes [i].reg_pressure));
      ira_loop_nodes [i].mentioned_allocnos = ira_allocate_bitmap ();
      ira_loop_nodes [i].modified_regnos = ira_allocate_bitmap ();
      ira_loop_nodes [i].border_allocnos = ira_allocate_bitmap ();
      ira_loop_nodes [i].local_copies = ira_allocate_bitmap ();
    }
}

/* The function frees the loop tree nodes.  */
static void
finish_loop_tree_nodes (void)
{
  unsigned int i;
  loop_p loop;

  for (i = 0; VEC_iterate (loop_p, ira_loops.larray, i, loop); i++)
    if (ira_loop_nodes [i].regno_allocno_map != NULL)
      {
	ira_free_bitmap (ira_loop_nodes [i].local_copies);
	ira_free_bitmap (ira_loop_nodes [i].border_allocnos);
	ira_free_bitmap (ira_loop_nodes [i].modified_regnos);
	ira_free_bitmap (ira_loop_nodes [i].mentioned_allocnos);
	ira_free (ira_loop_nodes [i].regno_allocno_map);
      }
  ira_free (ira_loop_nodes);
  for (i = 0; i < (unsigned int) last_basic_block_before_change; i++)
    {
      if (ira_bb_nodes [i].local_copies != NULL)
	ira_free_bitmap (ira_bb_nodes [i].local_copies);
      if (ira_bb_nodes [i].border_allocnos != NULL)
	ira_free_bitmap (ira_bb_nodes [i].border_allocnos);
      if (ira_bb_nodes [i].modified_regnos != NULL)
	ira_free_bitmap (ira_bb_nodes [i].modified_regnos);
      if (ira_bb_nodes [i].mentioned_allocnos != NULL)
	ira_free_bitmap (ira_bb_nodes [i].mentioned_allocnos);
      if (ira_bb_nodes [i].regno_allocno_map != NULL)
	ira_free (ira_bb_nodes [i].regno_allocno_map);
    }
  ira_free (ira_bb_nodes);
}



/* The following recursive functions adds loop to the loop tree
   hierarchy.  The loop is added only once.  */
static void
add_loop_to_tree (struct loop *loop)
{
  struct loop *father;
  loop_tree_node_t loop_node, father_node;

  /* Can not use loop node access macros because of potential checking
     and because the nodes are not initialized yet.  */
  if (loop_outer (loop) != NULL)
    add_loop_to_tree (loop_outer (loop));
  if (ira_loop_nodes [loop->num].regno_allocno_map != NULL
      && ira_loop_nodes [loop->num].inner == NULL)
    {
      /* We have not added loop node to the tree yet.  */
      loop_node = &ira_loop_nodes [loop->num];
      loop_node->loop = loop;
      loop_node->bb = NULL;
      for (father = loop_outer (loop);
	   father != NULL;
	   father = loop_outer (father))
	if (ira_loop_nodes [father->num].regno_allocno_map != NULL)
	  break;
      if (father == NULL)
	{
	  loop_node->next = NULL;
	  loop_node->father = NULL;
	}
      else
	{
	  father_node = &ira_loop_nodes [father->num];
	  loop_node->next = father_node->inner;
	  father_node->inner = loop_node;
	  loop_node->father = father_node;
	}
    }
}

/* Enumerate loops in loop with root LOOP_NODE starting with LEVEL and
   return maximal value of level in the tree + 1.  */
static int
setup_loop_tree_level (loop_tree_node_t loop_node, int level)
{
  int height, max_height;
  loop_tree_node_t subloop_node;

  ira_assert (loop_node->bb == NULL);
  loop_node->level = level;
  max_height = level + 1;
  for (subloop_node = loop_node->inner;
       subloop_node != NULL;
       subloop_node = subloop_node->next)
    if (subloop_node->bb == NULL)
      {
	height = setup_loop_tree_level (subloop_node, level + 1);
	if (height > max_height)
	  max_height = height;
      }
  return max_height;
}

/* The following function creates the loop tree.  The algorithm is
   designed to provide correct order of loops (by the last loop BB)
   and basic blocks in chain formed by next.  */
static void
form_loop_tree (void)
{
  unsigned int i;
  basic_block bb;
  struct loop *father;
  loop_tree_node_t bb_node, loop_node;
  loop_p loop;

  /* Can not use loop/bb node access macros because of potential
     checking and the nodes are not initialized yet.  */
  for (i = 0; VEC_iterate (loop_p, ira_loops.larray, i, loop); i++)
     if (ira_loop_nodes [i].regno_allocno_map != NULL)
       ira_loop_nodes [i].inner = NULL;
  FOR_EACH_BB_REVERSE (bb)
    {
      bb_node = &ira_bb_nodes [bb->index];
      bb_node->bb = bb;
      bb_node->loop = NULL;
      bb_node->inner = NULL;
      bb_node->next = NULL;
      for (father = bb->loop_father;
	   father != NULL;
	   father = loop_outer (father))
	if (ira_loop_nodes [father->num].regno_allocno_map != NULL)
	  break;
      add_loop_to_tree (father);
      loop_node = &ira_loop_nodes [father->num];
      bb_node->next = loop_node->inner;
      bb_node->father = loop_node;
      loop_node->inner = bb_node;
    }
  ira_loop_tree_root = IRA_LOOP_NODE_BY_INDEX (ira_loops.tree_root->num);
  ira_loop_tree_height = setup_loop_tree_level (ira_loop_tree_root, 0);
  ira_assert (ira_loop_tree_root->regno_allocno_map != NULL);
}



/* The function rebuilds REGNO_ALLOCNO_MAP and REGNO_ALLOCNO_MAPs of
   the loops using only allocno info.  */
static void
rebuild_regno_allocno_maps (void)
{
  unsigned int l;
  int i, max_regno, regno;
  allocno_t a;
  loop_tree_node_t loop_tree_node;
  loop_p loop;

  max_regno = max_reg_num ();
  for (l = 0; VEC_iterate (loop_p, ira_loops.larray, l, loop); l++)
    if (ira_loop_nodes [l].regno_allocno_map != NULL)
      {
	ira_free (ira_loop_nodes [l].regno_allocno_map);
	ira_loop_nodes [l].regno_allocno_map
	  = ira_allocate (sizeof (allocno_t) * max_regno);
	memset (ira_loop_nodes [l].regno_allocno_map, 0,
		sizeof (allocno_t) * max_regno);
      }
  ira_free (regno_allocno_map);
  regno_allocno_map = ira_allocate (max_regno * sizeof (allocno_t));
  memset (regno_allocno_map, 0, max_regno * sizeof (allocno_t));
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (ALLOCNO_CAP_MEMBER (a) != NULL)
	continue;
      regno = ALLOCNO_REGNO (a);
      loop_tree_node = ALLOCNO_LOOP_TREE_NODE (a);
      ALLOCNO_NEXT_REGNO_ALLOCNO (a) = regno_allocno_map [regno];
      regno_allocno_map [regno] = a;
      if (loop_tree_node->regno_allocno_map [regno] == NULL)
	/* Remember that we can create temporary allocnos to break
	   cycles in register shuffle.  */
	loop_tree_node->regno_allocno_map [regno] = a;
    }
}



/* Array of vectors containing calls intersected by pseudo-registers.  */
VEC(rtx, heap) **regno_calls;

/* The length of the previous array.  */
static int regno_calls_num;

/* The function initializes array of vectors containing calls
   intersected by pseudo-registers.  */
static void
initiate_calls (void)
{
  regno_calls_num = max_reg_num () * 3 / 2;
  regno_calls = ira_allocate (regno_calls_num * sizeof (VEC(rtx, heap) *));
  memset (regno_calls, 0, regno_calls_num * sizeof (VEC(rtx, heap) *));
}

/* The function expands array of vectors containing calls for all
   pseudo-registers.  */
static void
expand_calls (void)
{
  int max_regno = max_reg_num ();

  if (max_regno > regno_calls_num)
    {
      regno_calls = ira_reallocate (regno_calls,
				    max_regno * sizeof (VEC(rtx, heap) *));
      memset (regno_calls + regno_calls_num, 0,
	      (max_regno - regno_calls_num) * sizeof (VEC(rtx, heap) *));
      regno_calls_num = max_regno;
    }
}

/* The function removes NULL elements from vectors containing calls
   intersected by pseudo-registers.  */
static void
compress_calls (void)
{
  int regno, curr, length, last;
  rtx *allocno_calls;

  for (regno = 0; regno < regno_calls_num; regno++)
    {
      allocno_calls = VEC_address (rtx, regno_calls [regno]);
      length = VEC_length (rtx, regno_calls [regno]);
      for (last = curr = 0; curr < length; curr++)
	if (allocno_calls [curr] != NULL_RTX)
	  allocno_calls [last++] = allocno_calls [curr];
      VEC_truncate (rtx, regno_calls [regno], last);
    }
}

/* The function adds CALL to REGNO's vector of intersected calls.  */
int
add_regno_call (int regno, rtx call)
{
  int result;

  gcc_assert (regno < regno_calls_num);
  if (regno_calls [regno] == NULL)
    regno_calls [regno] = VEC_alloc (rtx, heap, 10);
  result = VEC_length (rtx, regno_calls [regno]);
  VEC_safe_push (rtx, heap, regno_calls [regno], call);
  return result;
}

/* The function frees array of vectors containing calls
   intersected by pseudo-registers.  */
static void
finish_calls (void)
{
  int i;

  for (i = 0; i < regno_calls_num; i++)
    VEC_free (rtx, heap, regno_calls [i]);
  ira_free (regno_calls);
}



/* Pools for allocnos and allocno live ranges.  */
static alloc_pool allocno_pool, allocno_live_range_pool;

/* Varray containing references to all created allocnos.  It is a
   container of array allocnos.  */
static varray_type allocno_varray;

/* The function initializes data concerning allocnos.  */
static void
initiate_allocnos (void)
{
  allocno_live_range_pool
    = create_alloc_pool ("allocno live ranges",
			 sizeof (struct allocno_live_range), 100);
  allocno_pool = create_alloc_pool ("allocnos", sizeof (struct allocno), 100);
  VARRAY_GENERIC_PTR_NOGC_INIT
    (allocno_varray, max_reg_num () * 2, "allocnos");
  allocnos = NULL;
  allocnos_num = 0;
  regno_allocno_map = ira_allocate (max_reg_num () * sizeof (allocno_t));
  memset (regno_allocno_map, 0, max_reg_num () * sizeof (allocno_t));
}

/* The function creates and returns allocno corresponding to REGNO in
   LOOP_TREE_NODE.  Add the allocno to the list of allocnos with the
   same regno if ! CAP_P.  */
allocno_t
create_allocno (int regno, int cap_p, loop_tree_node_t loop_tree_node)
{
  allocno_t a;

  a = pool_alloc (allocno_pool);
  ALLOCNO_REGNO (a) = regno;
  ALLOCNO_LOOP_TREE_NODE (a) = loop_tree_node;
  if (! cap_p)
    {
      ALLOCNO_NEXT_REGNO_ALLOCNO (a) = regno_allocno_map [regno];
      regno_allocno_map [regno] = a;
      if (loop_tree_node->regno_allocno_map [regno] == NULL)
	/* Remember that we can create temporary allocnos to break
	   cycles in register shuffle.  */
	loop_tree_node->regno_allocno_map [regno] = a;
    }
  ALLOCNO_CAP (a) = NULL;
  ALLOCNO_CAP_MEMBER (a) = NULL;
  ALLOCNO_NUM (a) = allocnos_num;
  ALLOCNO_CONFLICT_ALLOCNO_VEC (a) = NULL;
  ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) = 0;
  ALLOCNO_TOTAL_CONFLICT_ALLOCNOS_NUM (a) = 0;
  CLEAR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a));
  CLEAR_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
  ALLOCNO_NREFS (a) = 0;
  ALLOCNO_FREQ (a) = 1;
  ALLOCNO_HARD_REGNO (a) = -1;
  ALLOCNO_CALL_FREQ (a) = 0;
  ALLOCNO_CALLS_CROSSED_NUM (a) = 0;
  ALLOCNO_CALLS_CROSSED_START (a) = -1;
#ifdef STACK_REGS
  ALLOCNO_NO_STACK_REG_P (a) = FALSE;
  ALLOCNO_TOTAL_NO_STACK_REG_P (a) = FALSE;
#endif
  ALLOCNO_MEM_OPTIMIZED_DEST (a) = NULL;
  ALLOCNO_MEM_OPTIMIZED_DEST_P (a) = FALSE;
  ALLOCNO_SOMEWHERE_RENAMED_P (a) = FALSE;
  ALLOCNO_DONT_REASSIGN_P (a) = FALSE;
  ALLOCNO_IN_GRAPH_P (a) = FALSE;
  ALLOCNO_ASSIGNED_P (a) = FALSE;
  ALLOCNO_MAY_BE_SPILLED_P (a) = FALSE;
  ALLOCNO_MODE (a) = (regno < 0 ? VOIDmode : PSEUDO_REGNO_MODE (regno));
  ALLOCNO_COPIES (a) = NULL;
  ALLOCNO_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_CONFLICT_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_UPDATED_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_LEFT_CONFLICTS_NUM (a) = -1;
  ALLOCNO_COVER_CLASS (a) = NO_REGS;
  ALLOCNO_BEST_CLASS (a) = NO_REGS;
  ALLOCNO_COVER_CLASS_COST (a) = 0;
  ALLOCNO_MEMORY_COST (a) = 0;
  ALLOCNO_UPDATED_MEMORY_COST (a) = 0;
  ALLOCNO_EXCESS_PRESSURE_POINTS_NUM (a) = 0;
  ALLOCNO_NEXT_BUCKET_ALLOCNO (a) = NULL;
  ALLOCNO_PREV_BUCKET_ALLOCNO (a) = NULL;
  ALLOCNO_FIRST_COALESCED_ALLOCNO (a) = a;
  ALLOCNO_NEXT_COALESCED_ALLOCNO (a) = a;
  ALLOCNO_LIVE_RANGES (a) = NULL;
  VARRAY_PUSH_GENERIC_PTR (allocno_varray, a);
  allocnos = (allocno_t *) &VARRAY_GENERIC_PTR (allocno_varray, 0);
  allocnos_num = VARRAY_ACTIVE_SIZE (allocno_varray);
  return a;
}

/* The function allocates conflict vector of A for NUM allocnos.  */
void
allocate_allocno_conflicts (allocno_t a, int num)
{
  ira_assert (ALLOCNO_CONFLICT_ALLOCNO_VEC (a) == NULL);
  ALLOCNO_CONFLICT_ALLOCNO_VEC (a)
    = ira_allocate (sizeof (allocno_t) * (num + 1));
  ALLOCNO_CONFLICT_ALLOCNO_VEC (a) [0] = NULL;
  ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) = 0;
  ALLOCNO_TOTAL_CONFLICT_ALLOCNOS_NUM (a) = 0;
  ALLOCNO_CONFLICT_ALLOCNO_VEC_SIZE (a) = num;
}

/* The function checks that conflict vector of A has enough space to
   contain NUM allocno references.  If the space is not enough, the
   function expands the conflict vector.  */
static void
check_allocno_conflict_vec (allocno_t a, int num)
{
  int size;
  allocno_t *vec;

  ira_assert (ALLOCNO_CONFLICT_ALLOCNO_VEC (a) != NULL);
  if (ALLOCNO_CONFLICT_ALLOCNO_VEC_SIZE (a) >= num)
    return;
  size = 3 * num / 2 + 1;
  vec = ira_allocate (sizeof (allocno_t) * (size + 1));
  memcpy (vec, ALLOCNO_CONFLICT_ALLOCNO_VEC (a),
	  sizeof (allocno_t)
	  * (ALLOCNO_TOTAL_CONFLICT_ALLOCNOS_NUM (a) + 1));
  ira_free (ALLOCNO_CONFLICT_ALLOCNO_VEC (a));
  ALLOCNO_CONFLICT_ALLOCNO_VEC (a) = vec;
  ALLOCNO_CONFLICT_ALLOCNO_VEC_SIZE (a) = size;
}

/* The function adds A2 to conflict vector of A1.  */
static void
add_to_allocno_conflict_vec (allocno_t a1, allocno_t a2)
{
  int size;
  allocno_t *vec;

  size = ALLOCNO_TOTAL_CONFLICT_ALLOCNOS_NUM (a1);
  check_allocno_conflict_vec (a1, size + 1);
  vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (a1);
  vec [size] = a2;
  vec [size + 1] = NULL;
  ALLOCNO_TOTAL_CONFLICT_ALLOCNOS_NUM (a1)++;
}

/* The function adds A1 to conflict vector of A2 and vise versa.  */
void
add_allocno_conflict (allocno_t a1, allocno_t a2)
{
  add_to_allocno_conflict_vec (a1, a2);
  add_to_allocno_conflict_vec (a2, a1);
}

/* The array used to find duplications in conflict vecs of
   allocnos.  */
static int *allocno_conflict_check;
/* The value used to mark allocation presence in conflict vec of the
   current allocno.  */
static int curr_allocno_conflict_check_tick;

/* The function removes duplications in conflict vector of A.  */
static void
compress_allocno_conflict_vec (allocno_t a)
{
  allocno_t *vec, conflict_a;
  int i, j;

  vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (a);
  curr_allocno_conflict_check_tick++;
  for (i = j = 0; (conflict_a = vec [i]) != NULL; i++)
    {
      if (ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) == i)
	ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) = j;
      if (allocno_conflict_check [ALLOCNO_NUM (conflict_a)]
	  != curr_allocno_conflict_check_tick)
	{
	  allocno_conflict_check [ALLOCNO_NUM (conflict_a)]
	    = curr_allocno_conflict_check_tick;
	  vec [j++] = conflict_a;
	}
    }
  if (ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) == i)
    ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) = j;
  ALLOCNO_TOTAL_CONFLICT_ALLOCNOS_NUM (a) = j;
  vec [j] = NULL;
}

/* The function removes duplications in conflict vectors of all
   allocnos.  */
static void
compress_conflict_vecs (void)
{
  int i;

  allocno_conflict_check = ira_allocate (sizeof (int) * allocnos_num);
  memset (allocno_conflict_check, 0, sizeof (int) * allocnos_num);
  curr_allocno_conflict_check_tick = 0;
  for (i = 0; i < allocnos_num; i++)
    compress_allocno_conflict_vec (allocnos [i]);
  ira_free (allocno_conflict_check);
}

/* This recursive function outputs allocno A and if it is cap the
   function outputs members.  */
void
print_expanded_allocno (allocno_t a)
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
      print_expanded_allocno (ALLOCNO_CAP_MEMBER (a));
    }
  fprintf (ira_dump_file, ")");
}

/* The function creates and returns cap representing allocno A in the
   father loop.  */
static allocno_t
create_cap_allocno (allocno_t a)
{
  allocno_t cap;
  loop_tree_node_t father;

  ira_assert (ALLOCNO_FIRST_COALESCED_ALLOCNO (a) == a
	      && ALLOCNO_NEXT_COALESCED_ALLOCNO (a) == a);
  father = ALLOCNO_LOOP_TREE_NODE (a)->father;
  cap = create_allocno (ALLOCNO_REGNO (a), TRUE, father);
  /* We just need to set a mode giving the same size.  */
  ALLOCNO_MODE (cap) = ALLOCNO_MODE (a);
  ALLOCNO_COVER_CLASS (cap) = ALLOCNO_COVER_CLASS (a);
  ALLOCNO_BEST_CLASS (cap) = ALLOCNO_BEST_CLASS (a);
  ALLOCNO_AVAILABLE_REGS_NUM (cap) = ALLOCNO_AVAILABLE_REGS_NUM (a);
  ALLOCNO_CAP_MEMBER (cap) = a;
  bitmap_set_bit (father->mentioned_allocnos, ALLOCNO_NUM (cap));
  ALLOCNO_CAP (a) = cap;
  ALLOCNO_COVER_CLASS_COST (cap) = ALLOCNO_COVER_CLASS_COST (a);
  /* ??? memory_cost */
  ALLOCNO_MEMORY_COST (cap) = ALLOCNO_MEMORY_COST (a);
  ALLOCNO_UPDATED_MEMORY_COST (cap) = ALLOCNO_UPDATED_MEMORY_COST (a);
  if (internal_flag_ira_verbose > 2 && ira_dump_file != NULL)
    {
      fprintf (ira_dump_file, "    Creating cap ");
      print_expanded_allocno (cap);
      fprintf (ira_dump_file, "\n");
    }
  return cap;
}

/* The function propagate info obtained during conflicts building to
   CAP.  */
static void
propagate_info_to_cap (allocno_t cap)
{
  int i, regno, conflicts_num;
  enum reg_class cover_class;
  allocno_t a, conflict_allocno, conflict_father_allocno;
  allocno_t another_a, father_a;
  allocno_t *allocno_vec;
  loop_tree_node_t father;
  copy_t cp, next_cp;

  ira_assert (ALLOCNO_FIRST_COALESCED_ALLOCNO (cap) == cap
	      && ALLOCNO_NEXT_COALESCED_ALLOCNO (cap) == cap
	      && ALLOCNO_CONFLICT_ALLOCNO_VEC (cap) == NULL
	      && ALLOCNO_CALLS_CROSSED_NUM (cap) == 0);
  a = ALLOCNO_CAP_MEMBER (cap);
  father = ALLOCNO_LOOP_TREE_NODE (cap);
  cover_class = ALLOCNO_COVER_CLASS (cap);
  allocate_and_copy_costs
    (&ALLOCNO_HARD_REG_COSTS (cap), cover_class, ALLOCNO_HARD_REG_COSTS (a));
  allocate_and_copy_costs
    (&ALLOCNO_CONFLICT_HARD_REG_COSTS (cap), cover_class,
     ALLOCNO_CONFLICT_HARD_REG_COSTS (a));
  ALLOCNO_NREFS (cap) = ALLOCNO_NREFS (a);
  ALLOCNO_FREQ (cap) = ALLOCNO_FREQ (a);
  ALLOCNO_CALL_FREQ (cap) = ALLOCNO_CALL_FREQ (a);
  IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (cap),
		    ALLOCNO_CONFLICT_HARD_REGS (a));
  IOR_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (cap),
		    ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
  ALLOCNO_CALLS_CROSSED_NUM (cap) = ALLOCNO_CALLS_CROSSED_NUM (a);
  ALLOCNO_CALLS_CROSSED_START (cap) = ALLOCNO_CALLS_CROSSED_START (a);
#ifdef STACK_REGS
  ALLOCNO_NO_STACK_REG_P (cap) = ALLOCNO_NO_STACK_REG_P (a);
  ALLOCNO_TOTAL_NO_STACK_REG_P (cap) = ALLOCNO_TOTAL_NO_STACK_REG_P (a);
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
      father_a = father->regno_allocno_map [ALLOCNO_REGNO (another_a)];
      if (father_a == NULL)
	father_a = ALLOCNO_CAP (another_a);
      if (father_a != NULL
	  && find_allocno_copy (cap, father_a,
				cp->insn, cp->loop_tree_node) == NULL)
	/* Upper level allocno might be not existing because it is
	   not mentioned or lived on the border.  It is just
	   living on bb start of the loop.  */
	add_allocno_copy (cap, father_a, cp->freq, cp->insn,
			  cp->loop_tree_node);
    }
  allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (a);
  if (allocno_vec != NULL)
    {
      conflicts_num = 0;
      for (i = 0;
	   (conflict_allocno = allocno_vec [i]) != NULL;
	   i++)
	conflicts_num++;
      allocate_allocno_conflicts (cap, conflicts_num);
      for (conflicts_num = i = 0;
	   (conflict_allocno = allocno_vec [i]) != NULL;
	   i++)
	{
	  regno = ALLOCNO_REGNO (conflict_allocno);
	  conflict_father_allocno = father->regno_allocno_map [regno];
	  if (conflict_father_allocno == NULL)
	    conflict_father_allocno = ALLOCNO_CAP (conflict_allocno);
	  if (conflict_father_allocno != NULL
	      && (ALLOCNO_CONFLICT_ALLOCNO_VEC (conflict_father_allocno)
		  != NULL))
	    add_allocno_conflict (cap, conflict_father_allocno);
	}
    }
  if (internal_flag_ira_verbose > 2 && ira_dump_file != NULL)
    {
      fprintf (ira_dump_file, "    Propagate info to cap ");
      print_expanded_allocno (cap);
      allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (cap);
      if (allocno_vec != NULL)
	{
	  fprintf (ira_dump_file, "\n      Conflicts:");
	  for (i = 0; (conflict_allocno = allocno_vec [i]) != NULL; i++)
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


/* Create and return allocno live range with give attributes.  */
allocno_live_range_t
create_allocno_live_range (allocno_t a, int start, int finish,
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

/* Create and return allocno live range with give attributes.  */
allocno_live_range_t
copy_allocno_live_range (allocno_live_range_t r)
{
  allocno_live_range_t p;

  p = pool_alloc (allocno_live_range_pool);
  *p = *r;
  return p;
}

/* Create and return allocno live range with give attributes.  */
allocno_live_range_t
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
finish_allocno_live_range (allocno_live_range_t r)
{
  pool_free (allocno_live_range_pool, r);
}

void
free_allocno_updated_costs (allocno_t a)
{
  enum reg_class cover_class;

  cover_class = ALLOCNO_COVER_CLASS (a);
  if (ALLOCNO_UPDATED_HARD_REG_COSTS (a) != NULL)
    free_cost_vector (ALLOCNO_UPDATED_HARD_REG_COSTS (a), cover_class);
  ALLOCNO_UPDATED_HARD_REG_COSTS (a) = NULL;
  if (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) != NULL)
    free_cost_vector (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a),
		      cover_class);
  ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) = NULL;
}

/* The function frees memory allocated for allocno A.  */
static void
finish_allocno (allocno_t a)
{
  allocno_live_range_t r, next_r;
  enum reg_class cover_class = ALLOCNO_COVER_CLASS (a);

  if (ALLOCNO_CONFLICT_ALLOCNO_VEC (a) != NULL)
    ira_free (ALLOCNO_CONFLICT_ALLOCNO_VEC (a));
  if (ALLOCNO_HARD_REG_COSTS (a) != NULL)
    free_cost_vector (ALLOCNO_HARD_REG_COSTS (a), cover_class);
  if (ALLOCNO_CONFLICT_HARD_REG_COSTS (a) != NULL)
    free_cost_vector (ALLOCNO_CONFLICT_HARD_REG_COSTS (a), cover_class);
  if (ALLOCNO_UPDATED_HARD_REG_COSTS (a) != NULL)
    free_cost_vector (ALLOCNO_UPDATED_HARD_REG_COSTS (a), cover_class);
  if (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) != NULL)
    free_cost_vector (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a),
		      cover_class);
  for (r = ALLOCNO_LIVE_RANGES (a); r != NULL; r = next_r)
    {
      next_r = r->next;
      finish_allocno_live_range (r);
    }
  pool_free (allocno_pool, a);
}

/* The function frees memory allocated for all allocnos.  */
static void
finish_allocnos (void)
{
  int i;

  for (i = 0; i < allocnos_num; i++)
    finish_allocno (allocnos [i]);
  ira_free (regno_allocno_map);
  VARRAY_FREE (allocno_varray);
  free_alloc_pool (allocno_pool);
  free_alloc_pool (allocno_live_range_pool);
}



/* Pools for copies.  */
static alloc_pool copy_pool;

/* Varray containing references to all created copies.  It is a
   container of array copies.  */
static varray_type copy_varray;

/* The function initializes data concerning allocno copies.  */
static void
initiate_copies (void)
{
  copy_pool = create_alloc_pool ("copies", sizeof (struct allocno_copy), 100);
  VARRAY_GENERIC_PTR_NOGC_INIT (copy_varray, get_max_uid (), "copies");
  copies = NULL;
  copies_num = 0;
}

/* Return copy connecting A1 and A2 and originated from INSN of
   LOOP_TREE_NODE if any.  */
static copy_t
find_allocno_copy (allocno_t a1, allocno_t a2, rtx insn,
		   loop_tree_node_t loop_tree_node)
{
  copy_t cp, next_cp;
  allocno_t another_a;

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

/* The function creates and returns created in LOOP_TREE_NODE copy of
   allocnos FIRST and SECOND with frequency FREQ, insn INSN.  */
copy_t
create_copy (allocno_t first, allocno_t second, int freq, rtx insn,
	     loop_tree_node_t loop_tree_node)
{
  copy_t cp;

  cp = pool_alloc (copy_pool);
  cp->num = copies_num;
  cp->first = first;
  cp->second = second;
  cp->freq = freq;
  cp->insn = insn;
  cp->loop_tree_node = loop_tree_node;
  VARRAY_PUSH_GENERIC_PTR (copy_varray, cp);
  copies = (copy_t *) &VARRAY_GENERIC_PTR (copy_varray, 0);
  copies_num = VARRAY_ACTIVE_SIZE (copy_varray);
  return cp;
}

/* The function attaches copy CP to allocnos involved into the copy.  */
void
add_allocno_copy_to_list (copy_t cp)
{
  allocno_t first = cp->first, second = cp->second;

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

/* The function detaches copy CP from allocnos involved into the copy.  */
void
remove_allocno_copy_from_list (copy_t cp)
{
  allocno_t first = cp->first, second = cp->second;
  copy_t prev, next;

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

/* The function makes copy CP a canonical copy where number of the
   first allocno is less than the second one.  */
void
swap_allocno_copy_ends_if_necessary (copy_t cp)
{
  allocno_t temp;
  copy_t temp_cp;

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

/* The function creates (or updates frequency) and returns new copy of
   allocnos FIRST and SECOND with frequency FREQ corresponding to move
   insn INSN (if any) and originated from LOOP_TREE_NODE.  */
copy_t
add_allocno_copy (allocno_t first, allocno_t second, int freq, rtx insn,
		  loop_tree_node_t loop_tree_node)
{
  copy_t cp;

  if ((cp = find_allocno_copy (first, second, insn, loop_tree_node)) != NULL)
    {
      cp->freq += freq;
      return cp;
    }
  cp = create_copy (first, second, freq, insn, loop_tree_node);
  ira_assert (first != NULL && second != NULL);
  add_allocno_copy_to_list (cp);
  swap_allocno_copy_ends_if_necessary (cp);
  return cp;
}

/* Print info about copies connecting allocno A into file F.  */
static void
print_allocno_copies (FILE *f, allocno_t a)
{
  allocno_t another_a;
  copy_t cp, next_cp;

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

void
debug_allocno_copies (allocno_t a)
{
  print_allocno_copies (stderr, a);
}

/* The function frees memory allocated for copy CP.  */
static void
finish_copy (copy_t cp)
{
  pool_free (copy_pool, cp);
}


/* The function frees memory allocated for all copies.  */
static void
finish_copies (void)
{
  int i;

  for (i = 0; i < copies_num; i++)
    finish_copy (copies [i]);
  VARRAY_FREE (copy_varray);
  free_alloc_pool (copy_pool);
}



/* Pools for cost vectors.  */
static alloc_pool cost_vector_pool [N_REG_CLASSES];

/* The function initiates work with hard register cost vectors.  It
   creates allocation pool for each cover class.  */
static void
initiate_cost_vectors (void)
{
  int i;
  enum reg_class cover_class;

  for (i = 0; i < reg_class_cover_size; i++)
    {
      cover_class = reg_class_cover [i];
      cost_vector_pool [cover_class]
	= create_alloc_pool ("cost vectors",
			     sizeof (int) * class_hard_regs_num [cover_class] + 32,
			     100);
    }
}

/* The function allocates and returns cost vector VEC for
   COVER_CLASS.  */
int *
allocate_cost_vector (enum reg_class cover_class)
{
  return pool_alloc (cost_vector_pool [cover_class]);
}

/* The function frees cost vector VEC for COVER_CLASS.  */
void
free_cost_vector (int *vec, enum reg_class cover_class)
{
  ira_assert (vec != NULL);
  pool_free (cost_vector_pool [cover_class], vec);
}

/* The function finishes work with hard register cost vectors.  It
   releases allocation pool for each cover class.  */
static void
finish_cost_vectors (void)
{
  int i;
  enum reg_class cover_class;

  for (i = 0; i < reg_class_cover_size; i++)
    {
      cover_class = reg_class_cover [i];
      free_alloc_pool (cost_vector_pool [cover_class]);
    }
}



/* The current loop tree node and its map.  */
loop_tree_node_t ira_curr_loop_tree_node;
allocno_t *ira_curr_regno_allocno_map;

/* The recursive function traverses loop tree node with root LOOP_NODE
   calling non-null functions PREORDER_FUNC and POSTORDER_FUNC
   correspondingly in preorder and postorder.  The function sets up
   IRA_CURR_LOOP_TREE_NODE.  If BB_FIRST_P, BB of LOOP_NODE is
   processed before its subloops.  */
void
traverse_loop_tree (int bb_first_p, loop_tree_node_t loop_node,
		    void (*preorder_func) (loop_tree_node_t),
		    void (*postorder_func) (loop_tree_node_t))
{
  loop_tree_node_t subloop_node;

  if (loop_node->bb == NULL)
    {
      ira_curr_loop_tree_node = loop_node;
      ira_curr_regno_allocno_map = ira_curr_loop_tree_node->regno_allocno_map;
    }
  if (preorder_func != NULL)
    (*preorder_func) (loop_node);
  
  for (subloop_node = loop_node->inner;
       subloop_node != NULL;
       subloop_node = subloop_node->next)
    if (! bb_first_p || subloop_node->bb != NULL)
      {
	traverse_loop_tree (bb_first_p, subloop_node,
			    preorder_func, postorder_func);
	ira_curr_loop_tree_node = loop_node;
	ira_curr_regno_allocno_map
	  = ira_curr_loop_tree_node->regno_allocno_map;
      }

  if (bb_first_p)
    for (subloop_node = loop_node->inner;
	 subloop_node != NULL;
	 subloop_node = subloop_node->next)
      if (subloop_node->bb == NULL)
	{
	  traverse_loop_tree (bb_first_p, subloop_node,
			      preorder_func, postorder_func);
	  ira_curr_loop_tree_node = loop_node;
	  ira_curr_regno_allocno_map
	    = ira_curr_loop_tree_node->regno_allocno_map;
	}

  if (postorder_func != NULL)
    (*postorder_func) (loop_node);
}



/* The current basic block.  */
static basic_block curr_bb;

/* This recursive function creates allocnos corresponding to
   pseudo-registers containing in X.  Nonzero OUTPUT_P means that X is
   lvalue.  */
static void
create_insn_allocnos (rtx x, int output_p)
{
  int i, j;
  const char *fmt;
  enum rtx_code code = GET_CODE (x);

  if (code == REG)
    {
      int regno;

      if ((regno = REGNO (x)) >= FIRST_PSEUDO_REGISTER)
	{
	  allocno_t a;

	  if ((a = ira_curr_loop_tree_node->regno_allocno_map [regno]) == NULL)
	    a = create_allocno (regno, FALSE, ira_curr_loop_tree_node);
	  
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
      create_insn_allocnos (SET_DEST (x), TRUE);
      create_insn_allocnos (SET_SRC (x), FALSE);
      return;
    }
  else if (code == CLOBBER)
    {
      create_insn_allocnos (XEXP (x, 0), TRUE);
      return;
    }
  else if (code == MEM)
    {
      create_insn_allocnos (XEXP (x, 0), FALSE);
      return;
    }
  else if (code == PRE_DEC || code == POST_DEC || code == PRE_INC || 
	   code == POST_INC || code == POST_MODIFY || code == PRE_MODIFY)
    {
      create_insn_allocnos (XEXP (x, 0), TRUE);
      create_insn_allocnos (XEXP (x, 0), FALSE);
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

/* The function creates allocnos corresponding to pseudo-registers
   living in basic blocks represented by the corresponding loop tree
   node BB_NODE.  */
static void
create_bb_allocnos (loop_tree_node_t bb_node)
{
  basic_block bb;
  rtx insn;
  unsigned int i;
  bitmap_iterator bi;
  allocno_t *map;

  curr_bb = bb = bb_node->bb;
  ira_assert (bb != NULL);
  FOR_BB_INSNS (bb, insn)
    if (INSN_P (insn))
      create_insn_allocnos (PATTERN (insn), FALSE);
  /* It might be a allocno living through from one subloop to
     another.  */
  map = ira_curr_loop_tree_node->regno_allocno_map;
  EXECUTE_IF_SET_IN_REG_SET (DF_LR_IN (bb), FIRST_PSEUDO_REGISTER, i, bi)
    if (map [i] == NULL)
      create_allocno (i, FALSE, ira_curr_loop_tree_node);
}

/* The function creates allocnos corresponding to pseudo-registers
   living on edge E (a loop enter or exit).  It also finds allocnos
   living on the loop border. */
static void
create_loop_allocnos (edge e)
{
  unsigned int i;
  bitmap live_in_regs, border_allocnos;
  bitmap_iterator bi;
  allocno_t *map;

  live_in_regs = DF_LR_IN (e->dest);
  map = ira_curr_loop_tree_node->regno_allocno_map;
  border_allocnos = ira_curr_loop_tree_node->border_allocnos;
  EXECUTE_IF_SET_IN_REG_SET (DF_LR_OUT (e->src),
			     FIRST_PSEUDO_REGISTER, i, bi)
    if (bitmap_bit_p (live_in_regs, i))
      {
	if (map [i] == NULL)
	  create_allocno (i, FALSE, ira_curr_loop_tree_node);
	bitmap_set_bit (border_allocnos, ALLOCNO_NUM (map [i]));
      }
}

/* The function creates allocnos corresponding to pseudo-registers
   living in loop represented by the corresponding loop tree node
   LOOP_NODE.  */
static void
create_loop_tree_node_allocnos (loop_tree_node_t loop_node)
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

/* The function creates allocnos corresponding to pseudo-registers in
   the current function.  The function traverses the loop tree for
   this.  */
static void
create_allocnos (void)
{
  int i;
  allocno_t a, father_a;
  loop_tree_node_t father;

  /* We need to process BB first to correctly link allocnos by
     next_regno_allocno field.  */
  traverse_loop_tree (TRUE, ira_loop_tree_root,
		      create_loop_tree_node_allocnos, NULL);
  if (flag_ira_algorithm != IRA_ALGORITHM_REGIONAL
      && flag_ira_algorithm != IRA_ALGORITHM_MIXED)
    return;
  /* Propagate number of references and frequencies for regional
     register allocator.  */
  for (i = max_reg_num () - 1; i >= FIRST_PSEUDO_REGISTER; i--)
    for (a = regno_allocno_map [i];
	 a != NULL;
	 a = ALLOCNO_NEXT_REGNO_ALLOCNO (a))
      if ((father = ALLOCNO_LOOP_TREE_NODE (a)->father) != NULL
	  && (father_a = father->regno_allocno_map [i]) != NULL)
	{
	  ALLOCNO_NREFS (father_a) += ALLOCNO_NREFS (a);
	  ALLOCNO_FREQ (father_a) += ALLOCNO_FREQ (a);
	}
}



/* Bitmap of allocnos living only inside the current loop.  */
static bitmap local_allocnos_bitmap;

/* The function creates caps representing allocnos living only inside
   the loop given by LOOP_NODE on higher loop level.  */
static void
create_loop_tree_node_caps (loop_tree_node_t loop_node)
{
  unsigned int i;
  bitmap_iterator bi;
  loop_tree_node_t father;

  if (loop_node->bb != NULL || loop_node == ira_loop_tree_root)
    return;
  bitmap_and_compl (local_allocnos_bitmap, loop_node->mentioned_allocnos,
		    loop_node->border_allocnos);
  father = loop_node->father;
  EXECUTE_IF_SET_IN_BITMAP (local_allocnos_bitmap, 0, i, bi)
    if (father->regno_allocno_map [ALLOCNO_REGNO (allocnos [i])] == NULL)
      create_cap_allocno (allocnos [i]);
}

/* The function propagate info to caps mentioned in LOOP_NODE.  */
static void
propagate_info_to_loop_tree_node_caps (loop_tree_node_t loop_node)
{
  unsigned int i;
  bitmap_iterator bi;
  allocno_t a;

  if (loop_node->bb != NULL)
    return;
  EXECUTE_IF_SET_IN_BITMAP (loop_node->mentioned_allocnos, 0, i, bi)
    {
      a = allocnos [i];
      if (ALLOCNO_CAP_MEMBER (a) != NULL)
	propagate_info_to_cap (a);
    }
}



/* The function merges ranges R1 and R2 and returns the result.  */
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
	  finish_allocno_live_range (temp);
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

/* The function returns immediate common dominator of two loop tree
   nodes N1 and N2.  */
static loop_tree_node_t
common_loop_tree_node_dominator (loop_tree_node_t n1, loop_tree_node_t n2)
{
  ira_assert (n1 != NULL && n2 != NULL);
  if (n1 == n2)
    return n1;
  if (n1->level < n2->level)
    return common_loop_tree_node_dominator (n1, n2->father);
  else if (n1->level > n2->level)
    return common_loop_tree_node_dominator (n1->father, n2);
  else
    return common_loop_tree_node_dominator (n1->father, n2->father);
}

/* Map: regno -> allocnos which will finally represent the regno for
   IR with one region.  */
static allocno_t *regno_top_level_allocno_map;


/* The function check conflicts A with allocnos from CONFLICT_VECT and
   add them (more accurately corresponding final IR allocnos) if it is
   necessary.  */
static void
check_and_add_conflicts (allocno_t a, allocno_t *conflict_vec)
{
  allocno_t conflict_a;
  int i;

  for (i = 0; (conflict_a = conflict_vec [i]) != NULL; i++)
    {
      conflict_a
	= regno_top_level_allocno_map [REGNO (ALLOCNO_REG (conflict_a))];
      if (allocno_live_ranges_intersect_p (conflict_a, a))
	{
	  add_to_allocno_conflict_vec (conflict_a, a);
	  add_to_allocno_conflict_vec (a, conflict_a);
	  if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
	    fprintf (ira_dump_file,
		     "      Add underlying conflict a%dr%d-a%dr%d\n",
		     ALLOCNO_NUM (a), REGNO (ALLOCNO_REG (a)),
		     ALLOCNO_NUM (conflict_a),\
		     REGNO (ALLOCNO_REG (conflict_a)));
	}
    }
}

/* This recursive function checks and adds (if necessary) conflicts
   with A processing conflicts of allocnos corresponding to A in
   subloops.  If GO_DEEPER_P is false, the function stops to go deeper
   in loop tree when allocno which will represent allocno in final IR
   is achieved. */
static void
add_conflict_with_underlying_allocnos (allocno_t a,
				       loop_tree_node_t loop_node,
				       int go_deeper_p)
{
  loop_tree_node_t subloop_node;
  allocno_t subloop_a;

  for (subloop_node = loop_node->inner;
       subloop_node != NULL;
       subloop_node = subloop_node->next)
    {
      if (subloop_node->bb != NULL)
	continue;
      subloop_a = subloop_node->regno_allocno_map [ALLOCNO_REGNO (a)];
      if (subloop_a == NULL)
	continue;
      if (! go_deeper_p
	  && subloop_a == regno_top_level_allocno_map [REGNO (ALLOCNO_REG
							      (subloop_a))])
	continue;
      check_and_add_conflicts (a, ALLOCNO_CONFLICT_ALLOCNO_VEC (subloop_a));
      add_conflict_with_underlying_allocnos (a, subloop_node, go_deeper_p);
    }
}

/* The function flattens IR.  In other words, the function transforms
   IR as it were build with one region (without loops).  We could make
   it much simpler by rebuilding IR with one region, but unfortunately
   it takes a lot of time.  */
void
ira_flattening (int max_regno_before_emit, int max_point_before_emit)
{
  int i, j, k, free, propagate_p, stop_p, keep_p;
  int hard_regs_num, new_allocnos_p, renamed_p, start;
  unsigned int n;
  enum reg_class cover_class;
  rtx call, *allocno_calls;
  allocno_t a, father_a, conflict_a, first, second, node_first, node_second;
  allocno_t dominator_a, *allocno_vec;
  copy_t cp;
  loop_tree_node_t father, node, dominator;
  allocno_live_range_t r;
  bitmap live_allocnos;
  bitmap_iterator bi;

  regno_top_level_allocno_map
    = ira_allocate (max_reg_num () * sizeof (allocno_t));
  memset (regno_top_level_allocno_map, 0, max_reg_num () * sizeof (allocno_t));
  expand_calls ();
  new_allocnos_p = renamed_p = FALSE;
  /* Updating accumulated attributes.  */
  for (i = max_regno_before_emit - 1; i >= FIRST_PSEUDO_REGISTER; i--)
    {
      propagate_p = FALSE;
      for (a = regno_allocno_map [i];
	   a != NULL;
	   a = ALLOCNO_NEXT_REGNO_ALLOCNO (a))
	{
	  if (ALLOCNO_CAP_MEMBER (a) != NULL)
	    continue;
	  if (ALLOCNO_SOMEWHERE_RENAMED_P (a))
	    renamed_p = TRUE;
	  if ((unsigned int) ALLOCNO_REGNO (a) != REGNO (ALLOCNO_REG (a))
	      && ALLOCNO_CALLS_CROSSED_NUM (a) != 0)
	    {
	      allocno_calls = (VEC_address (rtx,
					    regno_calls [ALLOCNO_REGNO (a)])
			       + ALLOCNO_CALLS_CROSSED_START (a));
	      for (j = ALLOCNO_CALLS_CROSSED_NUM (a) - 1; j >= 0; j--)
		{
		  call = allocno_calls [j];
		  if (call == NULL_RTX)
		    continue;
		  add_regno_call (REGNO (ALLOCNO_REG (a)), call);
		  allocno_calls [j] = NULL_RTX;
		}
	    }
	  if ((father = ALLOCNO_LOOP_TREE_NODE (a)->father) == NULL
	      || ((father_a = father->regno_allocno_map [ALLOCNO_REGNO (a)])
		  == NULL))
	    {
	      ALLOCNO_COPIES (a) = NULL;
	      regno_top_level_allocno_map [REGNO (ALLOCNO_REG (a))] = a;
	      continue;
	    }
	  ira_assert (ALLOCNO_CAP_MEMBER (father_a) == NULL);
	  if (propagate_p)
	    {
	      COPY_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (father_a),
				 ALLOCNO_CONFLICT_HARD_REGS (father_a));
	      IOR_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (father_a),
				ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
#ifdef STACK_REGS
	      ALLOCNO_TOTAL_NO_STACK_REG_P (father_a)
		= (ALLOCNO_NO_STACK_REG_P (father_a)
		   || ALLOCNO_TOTAL_NO_STACK_REG_P (a));
#endif
	    }
	  if (REGNO (ALLOCNO_REG (a)) == REGNO (ALLOCNO_REG (father_a)))
	    {
	      if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
		{
		  fprintf (ira_dump_file,
			   "      Moving ranges of a%dr%d to a%dr%d: ",
			   ALLOCNO_NUM (a), REGNO (ALLOCNO_REG (a)),
			   ALLOCNO_NUM (father_a),
			   REGNO (ALLOCNO_REG (father_a)));
		  print_live_range_list (ira_dump_file,
					 ALLOCNO_LIVE_RANGES (a));
		}
	      ALLOCNO_LIVE_RANGES (father_a)
		= merge_ranges (ALLOCNO_LIVE_RANGES (a),
				ALLOCNO_LIVE_RANGES (father_a));
	      ALLOCNO_LIVE_RANGES (a) = NULL;
	      ALLOCNO_MEM_OPTIMIZED_DEST_P (father_a)
		= (ALLOCNO_MEM_OPTIMIZED_DEST_P (father_a)
		   || ALLOCNO_MEM_OPTIMIZED_DEST_P (a));
	      continue;
	    }
	  new_allocnos_p = TRUE;
	  propagate_p = TRUE;
	  first = ALLOCNO_MEM_OPTIMIZED_DEST (a) == NULL ? NULL : a;
	  for (;;)
	    {
	      if (first == NULL
		  && ALLOCNO_MEM_OPTIMIZED_DEST (father_a) != NULL)
		first = father_a;
	      ALLOCNO_NREFS (father_a) -= ALLOCNO_NREFS (a);
	      ALLOCNO_FREQ (father_a) -= ALLOCNO_FREQ (a);
	      ALLOCNO_CALL_FREQ (father_a) -= ALLOCNO_CALL_FREQ (a);
	      cover_class = ALLOCNO_COVER_CLASS (father_a);
	      hard_regs_num = class_hard_regs_num [cover_class];
	      if (ALLOCNO_HARD_REG_COSTS (a) != NULL
		  && ALLOCNO_HARD_REG_COSTS (father_a) != NULL)
		for (j = 0; j < hard_regs_num; j++)
		  ALLOCNO_HARD_REG_COSTS (father_a) [j]
		    -= ALLOCNO_HARD_REG_COSTS (a) [j];
	      if (ALLOCNO_CONFLICT_HARD_REG_COSTS (a) != NULL
		  && ALLOCNO_CONFLICT_HARD_REG_COSTS (father_a) != NULL)
		for (j = 0; j < hard_regs_num; j++)
		  ALLOCNO_CONFLICT_HARD_REG_COSTS (father_a) [j]
		    -= ALLOCNO_CONFLICT_HARD_REG_COSTS (a) [j];
	      ALLOCNO_COVER_CLASS_COST (father_a)
		-= ALLOCNO_COVER_CLASS_COST (a);
	      ALLOCNO_MEMORY_COST (father_a) -= ALLOCNO_MEMORY_COST (a);
	      ALLOCNO_EXCESS_PRESSURE_POINTS_NUM (father_a)
		-= ALLOCNO_EXCESS_PRESSURE_POINTS_NUM (a);
	      if ((father = ALLOCNO_LOOP_TREE_NODE (father_a)->father) == NULL
		  || (father_a = (father->regno_allocno_map
				  [ALLOCNO_REGNO (father_a)])) == NULL)
		break;
	    }
	  if (first != NULL)
	    {
	      father_a = ALLOCNO_MEM_OPTIMIZED_DEST (first);
	      dominator = common_loop_tree_node_dominator
		          (ALLOCNO_LOOP_TREE_NODE (father_a),
			   ALLOCNO_LOOP_TREE_NODE (first));
	      dominator_a = dominator->regno_allocno_map [ALLOCNO_REGNO (a)];
	      ira_assert (father_a != NULL);
	      stop_p = first != a;
	      /* Remember that exit can be to a grandparent (not only
		 a parent) or a child of grandparent.  */
	      for (first = a;;)
		{
		  if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
		    {
		      fprintf
			(ira_dump_file,
			 "      Coping ranges of a%dr%d to a%dr%d: ",
			 ALLOCNO_NUM (first), REGNO (ALLOCNO_REG (first)),
			 ALLOCNO_NUM (father_a),
			 REGNO (ALLOCNO_REG (father_a)));
		      print_live_range_list (ira_dump_file,
					     ALLOCNO_LIVE_RANGES (first));
		    }
		  ALLOCNO_LIVE_RANGES (father_a)
		    = merge_ranges (copy_allocno_live_range_list
				    (ALLOCNO_LIVE_RANGES (first)),
				    ALLOCNO_LIVE_RANGES (father_a));
		  if (stop_p)
		    break;
		  father = ALLOCNO_LOOP_TREE_NODE (first)->father;
		  ira_assert (father != NULL);
		  first = father->regno_allocno_map [ALLOCNO_REGNO (a)];
		  ira_assert (first != NULL);
		  if (first == dominator_a)
		    break;
		}
	    }
	  ALLOCNO_COPIES (a) = NULL;
	  regno_top_level_allocno_map [REGNO (ALLOCNO_REG (a))] = a;
	}
    }
  ira_assert (new_allocnos_p || renamed_p
	      || max_point_before_emit == max_point);
  if (new_allocnos_p)
    {
      /* Fix final allocnos attributes concerning calls.  */
      compress_calls ();
      for (i = 0; i < allocnos_num; i++)
	{
	  a = allocnos [i];
	  if (a != regno_top_level_allocno_map [REGNO (ALLOCNO_REG (a))]
	      || ALLOCNO_CAP_MEMBER (a) != NULL)
	    continue;
	  ALLOCNO_CALLS_CROSSED_START (a) = 0;
	  ALLOCNO_CALLS_CROSSED_NUM (a)
	    = VEC_length (rtx, regno_calls [REGNO (ALLOCNO_REG (a))]);
	}
    }
  /* Mark copies for removing and change allocnos in copies.  */
  for (i = 0; i < copies_num; i++)
    {
      cp = copies [i];
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
      first = regno_top_level_allocno_map [REGNO (ALLOCNO_REG (cp->first))];
      second = regno_top_level_allocno_map [REGNO (ALLOCNO_REG (cp->second))];
      node = cp->loop_tree_node;
      if (node == NULL)
	keep_p = TRUE; /* It copy generated in ira-emit.c.  */
      else
	{
	  /* Check that the copy was not propagated from level on
	     which we will have different pseudos.  */
	  node_first = node->regno_allocno_map [ALLOCNO_REGNO (cp->first)];
	  node_second = node->regno_allocno_map [ALLOCNO_REGNO (cp->second)];
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
  if (new_allocnos_p)
    {
      /* Add conflicting allocnos from lower levels.  If we have a situation
	 A1----conflict---B1
	 A2----conflict---B2
	 
	 and A1 and A2 will be presented by themselves in final IR and
	 B1 and B2 will be presented by B1, then we need to check
	 conflict A2 and B1.
   
	 There is another situation when we should check and add
	 conflicts too.  It should be done when we removed restoring
	 allocno value at the loop exits because the allocno value is
	 stored in memory and the value is not changed in the loop.
	 In this case the allocno lives in the loop and can conflict
	 with allocnos inside the loop.  */
      for (i = 0; i < allocnos_num; i++)
	{
	  a = allocnos [i];
	  if (a != regno_top_level_allocno_map [REGNO (ALLOCNO_REG (a))]
	      || ALLOCNO_CAP_MEMBER (a) != NULL)
	    continue;
	  add_conflict_with_underlying_allocnos (a, ALLOCNO_LOOP_TREE_NODE (a),
						 FALSE);
	  if ((first = ALLOCNO_MEM_OPTIMIZED_DEST (a)) != NULL)
	    {
	      first = regno_top_level_allocno_map [REGNO (ALLOCNO_REG (first))];
	      check_and_add_conflicts
		(first, ALLOCNO_CONFLICT_ALLOCNO_VEC (a));
	      add_conflict_with_underlying_allocnos
		(first, ALLOCNO_LOOP_TREE_NODE (a), TRUE);
	    }
	}
    }
  /* Change allocnos regno, conflicting allocnos, and range allocnos.  */
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (a != regno_top_level_allocno_map [REGNO (ALLOCNO_REG (a))]
	  || ALLOCNO_CAP_MEMBER (a) != NULL)
	continue;
      ALLOCNO_LOOP_TREE_NODE (a) = ira_loop_tree_root;
      ALLOCNO_REGNO (a) = REGNO (ALLOCNO_REG (a));
      allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (a);
      for (j = 0; (conflict_a = allocno_vec [j]) != NULL; j++)
	allocno_vec [j]
	  = regno_top_level_allocno_map [REGNO (ALLOCNO_REG (conflict_a))];
      for (r = ALLOCNO_LIVE_RANGES (a); r != NULL; r = r->next)
	r->allocno = a;
    }
  /* Remove allocnos on lower levels of the loop tree and
     enumerate allocnos.  */
  for (free = 0, i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (ALLOCNO_LOOP_TREE_NODE (a) != ira_loop_tree_root
	  || ALLOCNO_CAP_MEMBER (a) != NULL)
	{
	  if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
	    fprintf (ira_dump_file, "      Remove a%dr%d\n",
		     ALLOCNO_NUM (a), REGNO (ALLOCNO_REG (a)));
	  finish_allocno (a);
	  continue;
	}
      ALLOCNO_CAP (a) = NULL;
      if (new_allocnos_p || ALLOCNO_SOMEWHERE_RENAMED_P (a))
	{
	  /* Remove conflicts.  */
	  allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (a);
	  start = (ALLOCNO_SOMEWHERE_RENAMED_P (a)
		   ? 0 : ALLOCNO_CONFLICT_ALLOCNOS_NUM (a));
	  for (k = j = start;
	       (conflict_a = allocno_vec [j]) != NULL;
	       j++)
	    {
	      if (allocno_live_ranges_intersect_p (a, conflict_a))
		allocno_vec [k++] = conflict_a;
	      else
		{
		  if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
		    fprintf (ira_dump_file,
			     "      Remove conflict a%dr%d - a%dr%d\n",
			     ALLOCNO_NUM (a), REGNO (ALLOCNO_REG (a)),
			     ALLOCNO_NUM (conflict_a),
			     REGNO (ALLOCNO_REG (conflict_a)));
		  
		}
	    }
	  allocno_vec [k] = NULL;
	  ALLOCNO_TOTAL_CONFLICT_ALLOCNOS_NUM (a) = k;
	}
      if (i == free)
	{
	  free++;
	  continue;
	}
      if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
	fprintf (ira_dump_file, "      Enumerate a%dr%d to a%d\n",
		 ALLOCNO_NUM (a), REGNO (ALLOCNO_REG (a)), free);
      ALLOCNO_UPDATED_MEMORY_COST (a) = ALLOCNO_MEMORY_COST (a);
      if (! ALLOCNO_ASSIGNED_P (a))
	free_allocno_updated_costs (a);
      ira_assert (ALLOCNO_UPDATED_HARD_REG_COSTS (a) == NULL);
      ira_assert (ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS (a) == NULL);
      ALLOCNO_NUM (a) = free;
      allocnos [free++] = a;
    }
  for (i = free; i < allocnos_num; i++)
    VARRAY_POP (allocno_varray);
  allocnos = (allocno_t *) &VARRAY_GENERIC_PTR (allocno_varray, 0);
  allocnos_num = VARRAY_ACTIVE_SIZE (allocno_varray);
  /* Remove unnecessary copies, and enumerate copies.  */
  for (free = i = 0; i < copies_num; i++)
    {
      cp = copies [i];
      if (cp->loop_tree_node == NULL)
	{
	  finish_copy (cp);
	  continue;
	}
      ira_assert
	(ALLOCNO_LOOP_TREE_NODE (cp->first) == ira_loop_tree_root
	 && ALLOCNO_LOOP_TREE_NODE (cp->second) == ira_loop_tree_root);
      add_allocno_copy_to_list (cp);
      swap_allocno_copy_ends_if_necessary (cp);
      if (i == free)
	{
	  free++;
	  continue;
	}
      if (internal_flag_ira_verbose > 4 && ira_dump_file != NULL)
	fprintf (ira_dump_file,
		 "      Enumerate cp%d to cp%d\n", cp->num, free);
      cp->num = free;
      copies [free++] = cp;
    }
  for (i = free; i < copies_num; i++)
    VARRAY_POP (copy_varray);
  copies = (copy_t *) &VARRAY_GENERIC_PTR (copy_varray, 0);
  copies_num = VARRAY_ACTIVE_SIZE (copy_varray);
  rebuild_regno_allocno_maps ();
  rebuild_start_finish_chains ();
  ira_free (regno_top_level_allocno_map);
  if (new_allocnos_p)
    {
      live_allocnos = ira_allocate_bitmap ();
      for (i = max_point_before_emit; i < max_point; i++)
	{
	  for (r = start_point_ranges [i]; r != NULL; r = r->start_next)
	    {
	      a = r->allocno;
	      j = ALLOCNO_NUM (a);
	      EXECUTE_IF_SET_IN_BITMAP (live_allocnos, 0, n, bi)
		{
		  if (n == (unsigned int) j)
		    continue;
		  conflict_a = allocnos [n];
		  if (ALLOCNO_COVER_CLASS (a)
		      == ALLOCNO_COVER_CLASS (conflict_a))
		    {
		      if (internal_flag_ira_verbose > 4
			  && ira_dump_file != NULL)
			fprintf (ira_dump_file,
				 "      Add a%dr%d to conflict vec of a%dr%d\n",
				 ALLOCNO_NUM (conflict_a),
				 REGNO (ALLOCNO_REG (conflict_a)),
				 ALLOCNO_NUM (a), REGNO (ALLOCNO_REG (a)));
		      add_to_allocno_conflict_vec (a, conflict_a);
		      if (internal_flag_ira_verbose > 4
			  && ira_dump_file != NULL)
			fprintf (ira_dump_file,
				 "      Add a%dr%d to conflict vec of a%dr%d\n",
				 ALLOCNO_NUM (a), REGNO (ALLOCNO_REG (a)),
				 ALLOCNO_NUM (conflict_a),
				 REGNO (ALLOCNO_REG (conflict_a)));
		      add_to_allocno_conflict_vec (conflict_a, a);
		    }
		}
	      bitmap_set_bit (live_allocnos, j);
	    }
	  
	  for (r = finish_point_ranges [i]; r != NULL; r = r->finish_next)
	    bitmap_clear_bit (live_allocnos, ALLOCNO_NUM (r->allocno));
	}
      compress_conflict_vecs ();
      ira_free_bitmap (live_allocnos);
    }
}



/* This entry function creates internal representation for IRA
   (allocnos, copies, loop tree nodes).  If LOOPS_P is zero the nodes
   corresponding to the loops (except the root which corresponds the
   all function) and correspondingly allocnos for the loops will be
   not created (it will be done only for basic blocks).  Such value is
   used for Chaitin-Briggs and Chow's priority coloring.  The function
   returns nonzero if we generates loop structure (besides node
   representing all function) for regional allocation.  */
int
ira_build (int loops_p)
{
  unsigned int i;
  loop_p loop;

  df_analyze ();

  CLEAR_HARD_REG_SET (cfun->emit->call_used_regs);
  initiate_calls ();
  initiate_cost_vectors ();
  initiate_allocnos ();
  initiate_copies ();
  create_loop_tree_nodes (loops_p);
  form_loop_tree ();
  create_allocnos ();
  ira_costs ();
  if (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL
      || flag_ira_algorithm == IRA_ALGORITHM_MIXED)
    {
      local_allocnos_bitmap = ira_allocate_bitmap ();
      traverse_loop_tree (FALSE, ira_loop_tree_root, NULL,
			  create_loop_tree_node_caps);
      ira_free_bitmap (local_allocnos_bitmap);
    }
  create_allocno_live_ranges ();
  ira_build_conflicts ();
  if (internal_flag_ira_verbose > 0 && ira_dump_file != NULL)
    {
      int i, n, nr;
      allocno_live_range_t r;

      for (nr = n = i = 0; i < allocnos_num; i++)
	  n += ALLOCNO_TOTAL_CONFLICT_ALLOCNOS_NUM (allocnos [i]);
      for (nr = i = 0; i < allocnos_num; i++)
	for (r = ALLOCNO_LIVE_RANGES (allocnos [i]); r != NULL; r = r->next)
	    nr++;
      fprintf (ira_dump_file, "  regions=%d, blocks=%d, points=%d\n",
	       VEC_length (loop_p, ira_loops.larray), n_basic_blocks,
	       max_point);
      fprintf (ira_dump_file,
	       "    allocnos=%d, copies=%d, conflicts=%d, ranges=%d\n",
	       allocnos_num, copies_num, n, nr);
    }
  if (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL
      || flag_ira_algorithm == IRA_ALGORITHM_MIXED)
    traverse_loop_tree (FALSE, ira_loop_tree_root, NULL,
			propagate_info_to_loop_tree_node_caps);
  tune_allocno_costs_and_cover_classes ();
  if (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL
      || flag_ira_algorithm == IRA_ALGORITHM_MIXED)
    {
      for (i = 0; VEC_iterate (loop_p, ira_loops.larray, i, loop); i++)
	if (ira_loop_nodes [i].regno_allocno_map != NULL
	    && ira_loop_tree_root != &ira_loop_nodes [i])
	  return TRUE;
    }
  return FALSE;
}

/* The function releases data created by function ira_build.  */
void
ira_destroy (void)
{
  finish_loop_tree_nodes ();
  finish_copies ();
  finish_allocnos ();
  finish_calls ();
  finish_cost_vectors ();
  finish_allocno_live_ranges ();
}
