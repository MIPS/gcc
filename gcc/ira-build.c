/* Building allocnos for IRA.
   Copyright (C) 2006, 2007
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
static void form_loop_tree (void);

static void initiate_calls (void);
static void finish_calls (void);

static void initiate_allocnos (void);
static void check_allocno_conflict_vec (allocno_t, int);
static void add_allocno_conflict (allocno_t, allocno_t);
static allocno_t create_cap_allocno (allocno_t);
static void finish_allocnos (void);

static void initiate_copies (void);
static void finish_copies (void);

static void create_insn_allocnos (rtx, int);
static void create_bb_allocnos (struct ira_loop_tree_node *);
static void create_loop_allocnos (edge);
static void create_loop_tree_node_allocnos (struct ira_loop_tree_node *);
static void create_allocnos (void);
static void create_loop_tree_node_caps (struct ira_loop_tree_node *);
#ifdef ENABLE_IRA_CHECKING
static void check_coalesced_allocnos (void);
#endif

/* All natural loops.  */
struct loops ira_loops;

/* The root of the loop tree corresponding to the all function.  */
struct ira_loop_tree_node *ira_loop_tree_root;

/* All basic block data are referred through the following array.  We
   can not use member `aux' for this because it is used for insertion
   of insns on edges.  */
struct ira_loop_tree_node *ira_bb_nodes;

/* All loop data are referred through the following array.  */
struct ira_loop_tree_node *ira_loop_nodes;

/* Map regno -> allocno for the current loop tree node.  */
allocno_t *regno_allocno_map;

/* Array of references to all allocnos and its size.  The order number
   of the allocno corresponds to the index in the array.  */
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
    = ira_allocate (sizeof (struct ira_loop_tree_node) * last_basic_block);
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
  ira_loop_nodes = ira_allocate (sizeof (struct ira_loop_tree_node)
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
  struct ira_loop_tree_node *loop_node, *father_node;

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

/* The following function creates the loop tree.  The algorithm is
   designed to provide correct order of loops (by the last loop BB)
   and basic blocks in chain formed by next.  */
static void
form_loop_tree (void)
{
  unsigned int i;
  basic_block bb;
  struct loop *father;
  struct ira_loop_tree_node *bb_node, *loop_node;
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
  ira_assert (ira_loop_tree_root->regno_allocno_map != NULL);
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
  regno_calls_num = max_reg_num ();
  regno_calls = ira_allocate (regno_calls_num * sizeof (VEC(rtx, heap) *));
  memset (regno_calls, 0, regno_calls_num * sizeof (VEC(rtx, heap) *));
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



/* Varray containing references to all created allocnos.  It is a
   container of array allocnos.  */
static varray_type allocno_varray;

/* The function initializes data concerning allocnos.  */
static void
initiate_allocnos (void)
{
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
create_allocno (int regno, int cap_p, struct ira_loop_tree_node *loop_tree_node)
{
  allocno_t a;

  a = ira_allocate (sizeof (struct allocno));
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
  ALLOCNO_CONFLICT_ALLOCNO_VEC_ACTIVE_SIZE (a) = 0;
  CLEAR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a));
  ALLOCNO_FREQ (a) = 1;
  ALLOCNO_HARD_REGNO (a) = -1;
  ALLOCNO_CALL_FREQ (a) = 0;
  ALLOCNO_CALLS_CROSSED_NUM (a) = 0;
  ALLOCNO_CALLS_CROSSED_START (a) = -1;
#ifdef STACK_REGS
  ALLOCNO_NO_STACK_REG_P (a) = FALSE;
#endif
  ALLOCNO_IN_GRAPH_P (a) = FALSE;
  ALLOCNO_ASSIGNED_P (a) = FALSE;
  ALLOCNO_MAY_BE_SPILLED_P (a) = FALSE;
  ALLOCNO_MODE (a) = (regno < 0 ? VOIDmode : PSEUDO_REGNO_MODE (regno));
  ALLOCNO_COPIES (a) = NULL;
  ALLOCNO_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_CONFLICT_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_CURR_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (a) = NULL;
  ALLOCNO_LEFT_CONFLICTS_NUM (a) = -1;
  ALLOCNO_COVER_CLASS (a) = NO_REGS;
  ALLOCNO_BEST_CLASS (a) = NO_REGS;
  ALLOCNO_COVER_CLASS_COST (a) = 0;
  ALLOCNO_MEMORY_COST (a) = 0;
  ALLOCNO_NEXT_BUCKET_ALLOCNO (a) = NULL;
  ALLOCNO_PREV_BUCKET_ALLOCNO (a) = NULL;
  ALLOCNO_FIRST_COALESCED_ALLOCNO (a) = a;
  ALLOCNO_NEXT_COALESCED_ALLOCNO (a) = a;
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
  ALLOCNO_CONFLICT_ALLOCNO_VEC_ACTIVE_SIZE (a) = 0;
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
	  * (ALLOCNO_CONFLICT_ALLOCNO_VEC_ACTIVE_SIZE (a) + 1));
  ira_free (ALLOCNO_CONFLICT_ALLOCNO_VEC (a));
  ALLOCNO_CONFLICT_ALLOCNO_VEC (a) = vec;
  ALLOCNO_CONFLICT_ALLOCNO_VEC_SIZE (a) = size;
}

/* The function adds A1 to conflict vector of A2 and vise versa.  */
static void
add_allocno_conflict (allocno_t a1, allocno_t a2)
{
  int size1, size2;
  allocno_t *vec1, *vec2;

  size1 = ALLOCNO_CONFLICT_ALLOCNO_VEC_ACTIVE_SIZE (a1);
  size2 = ALLOCNO_CONFLICT_ALLOCNO_VEC_ACTIVE_SIZE (a2);
  check_allocno_conflict_vec (a1, size1 + 1);
  check_allocno_conflict_vec (a2, size2 + 1);
  vec1 = ALLOCNO_CONFLICT_ALLOCNO_VEC (a1);
  vec2 = ALLOCNO_CONFLICT_ALLOCNO_VEC (a2);
  vec1 [size1] = a2;
  vec2 [size2] = a1;
  vec1 [size1 + 1] = NULL;
  vec2 [size2 + 1] = NULL;
  ALLOCNO_CONFLICT_ALLOCNO_VEC_ACTIVE_SIZE (a1)++;
  ALLOCNO_CONFLICT_ALLOCNO_VEC_ACTIVE_SIZE (a2)++;
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
  int i, regno, hard_regs_num, conflicts_num;
  int *reg_costs, *conflict_reg_costs;
  basic_block bb;
  allocno_t cap, conflict_allocno, conflict_father_allocno;
  allocno_t *allocno_vec;
  struct ira_loop_tree_node *father;

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
  hard_regs_num = class_hard_regs_num [ALLOCNO_COVER_CLASS (a)];
  ALLOCNO_HARD_REG_COSTS (cap) = reg_costs
    = ira_allocate (hard_regs_num * sizeof (int));
  memcpy (reg_costs, ALLOCNO_HARD_REG_COSTS (a), hard_regs_num * sizeof (int));
  ALLOCNO_CONFLICT_HARD_REG_COSTS (cap) = conflict_reg_costs
    = ira_allocate (hard_regs_num * sizeof (int));
  memcpy (conflict_reg_costs, ALLOCNO_CONFLICT_HARD_REG_COSTS (a),
	  hard_regs_num * sizeof (int));
  ALLOCNO_CURR_HARD_REG_COSTS (cap)
    = ira_allocate (hard_regs_num * sizeof (int));
  ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (cap)
    = ira_allocate (hard_regs_num * sizeof (int));
  /* ??? costs, call_p etc.  */
  bitmap_set_bit (father->mentioned_allocnos, ALLOCNO_NUM (cap));
  ALLOCNO_CAP (a) = cap;
  ALLOCNO_AVAILABLE_REGS_NUM (cap) = ALLOCNO_AVAILABLE_REGS_NUM (a);
  ALLOCNO_COVER_CLASS_COST (cap) = ALLOCNO_COVER_CLASS_COST (a);
  ALLOCNO_MEMORY_COST (cap) = ALLOCNO_MEMORY_COST (a);
  ALLOCNO_FREQ (cap) = ALLOCNO_FREQ (a);
  ALLOCNO_CALL_FREQ (cap) = ALLOCNO_CALL_FREQ (a);
  IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (cap),
		    ALLOCNO_CONFLICT_HARD_REGS (a));
  ALLOCNO_CALLS_CROSSED_NUM (cap) = ALLOCNO_CALLS_CROSSED_NUM (a);
  ALLOCNO_CALLS_CROSSED_START (cap) = ALLOCNO_CALLS_CROSSED_START (a);
#ifdef STACK_REGS
  ALLOCNO_NO_STACK_REG_P (cap) = ALLOCNO_NO_STACK_REG_P (a);
#endif
  allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (a);
  for (conflicts_num = i = 0;
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
      if (conflict_father_allocno != NULL)
	add_allocno_conflict (cap, conflict_father_allocno);
    }
  if (ira_dump_file != NULL)
    {
      fprintf (ira_dump_file, "  Creating cap ");
      print_expanded_allocno (cap);
      fprintf (ira_dump_file, "\n    Conflicts:");
      allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (cap);
      for (i = 0; (conflict_allocno = allocno_vec [i]) != NULL; i++)
	{
	  fprintf (ira_dump_file, " a%d(r%d,", ALLOCNO_NUM (conflict_allocno),
		   ALLOCNO_REGNO (conflict_allocno));
	  if ((bb = ALLOCNO_LOOP_TREE_NODE (conflict_allocno)->bb) != NULL)
	    fprintf (ira_dump_file, "b%d)", bb->index);
	  else
	    fprintf (ira_dump_file, "l%d)",
		     ALLOCNO_LOOP_TREE_NODE (conflict_allocno)->loop->num);
	}
      fprintf (ira_dump_file, "\n");
    }
  return cap;
}

/* The function frees memory allocated for all allocnos.  */
static void
finish_allocnos (void)
{
  int i;
  allocno_t a;

  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (ALLOCNO_CONFLICT_ALLOCNO_VEC (a) != NULL)
	ira_free (ALLOCNO_CONFLICT_ALLOCNO_VEC (a));
      if (ALLOCNO_HARD_REG_COSTS (a) != NULL)
	ira_free (ALLOCNO_HARD_REG_COSTS (a));
      if (ALLOCNO_CONFLICT_HARD_REG_COSTS (a) != NULL)
	ira_free (ALLOCNO_CONFLICT_HARD_REG_COSTS (a));
      if (ALLOCNO_CURR_HARD_REG_COSTS (a) != NULL)
	ira_free (ALLOCNO_CURR_HARD_REG_COSTS (a));
      if (ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (a) != NULL)
	ira_free (ALLOCNO_CURR_CONFLICT_HARD_REG_COSTS (a));
      ira_free (a);
    }
  ira_free (regno_allocno_map);
  VARRAY_FREE (allocno_varray);
}



/* Varray containing references to all created copies.  It is a
   container of array copies.  */
static varray_type copy_varray;

/* The function initializes data concerning allocno copies.  */
static void
initiate_copies (void)
{
  VARRAY_GENERIC_PTR_NOGC_INIT (copy_varray, get_max_uid (), "copies");
  copies = NULL;
  copies_num = 0;
}

/* The function creates and returns copy of allocnos FIRST and SECOND
   with frequency FREQ and move insn MOVE_INSN.  */
copy_t
create_copy (allocno_t first, allocno_t second, int freq, rtx move_insn)
{
  copy_t cp;

  cp = ira_allocate (sizeof (struct allocno_copy));
  cp->num = copies_num;
  cp->first = first;
  cp->second = second;
  cp->freq = freq;
  cp->move_insn = move_insn;
  VARRAY_PUSH_GENERIC_PTR (copy_varray, cp);
  copies = (copy_t *) &VARRAY_GENERIC_PTR (copy_varray, 0);
  copies_num = VARRAY_ACTIVE_SIZE (copy_varray);
  return cp;
}

/* The function frees memory allocated for all copies.  */
static void
finish_copies (void)
{
  int i;

  for (i = 0; i < copies_num; i++)
    ira_free (copies [i]);
  VARRAY_FREE (copy_varray);
}



/* The current loop tree node.  */
struct ira_loop_tree_node *ira_curr_loop_tree_node;

/* The recursive function traverses loop tree node with root LOOP_NODE
   calling non-null functions PREORDER_FUNC and POSTORDER_FUNC
   correspondingly in preorder and postorder.  The function sets up
   IRA_CURR_LOOP_TREE_NODE.  */
void
traverse_loop_tree (struct ira_loop_tree_node *loop_node,
		    void (*preorder_func) (struct ira_loop_tree_node *),
		    void (*postorder_func) (struct ira_loop_tree_node *))
{
  struct ira_loop_tree_node *subloop_node;

  if (loop_node->bb == NULL)
    ira_curr_loop_tree_node = loop_node;
  if (preorder_func != NULL)
    (*preorder_func) (loop_node);
  
  for (subloop_node = loop_node->inner;
       subloop_node != NULL;
       subloop_node = subloop_node->next)
    {
      traverse_loop_tree (subloop_node, preorder_func, postorder_func);
      ira_curr_loop_tree_node = loop_node;
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
create_bb_allocnos (struct ira_loop_tree_node *bb_node)
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
create_loop_tree_node_allocnos (struct ira_loop_tree_node *loop_node)
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
  struct ira_loop_tree_node *father;

  traverse_loop_tree (ira_loop_tree_root, create_loop_tree_node_allocnos, NULL);
  if (flag_ira_algorithm != IRA_ALGORITHM_REGIONAL
      && flag_ira_algorithm != IRA_ALGORITHM_MIXED)
    return;
  /* Propagate frequencies for regional register allocator.  */
  for (i = max_reg_num () - 1; i >= FIRST_PSEUDO_REGISTER; i--)
    for (a = regno_allocno_map [i];
	 a != NULL;
	 a = ALLOCNO_NEXT_REGNO_ALLOCNO (a))
      if ((father = ALLOCNO_LOOP_TREE_NODE (a)->father) != NULL
	  && (father_a = father->regno_allocno_map [i]) != NULL)
	ALLOCNO_FREQ (father_a) += ALLOCNO_FREQ (a);
}



/* Bitmap of allocnos living only inside the current loop.  */
static bitmap local_allocnos_bitmap;

/* The function creates caps representing allocnos living only inside
   the loop given by LOOP_NODE on higher loop level.  */
static void
create_loop_tree_node_caps (struct ira_loop_tree_node *loop_node)
{
  unsigned int i;
  bitmap_iterator bi;
  allocno_t a, cap, another_a, father_a;
  copy_t cp, next_cp;
  struct ira_loop_tree_node *father;

  if (loop_node->bb != NULL || loop_node == ira_loop_tree_root)
    return;
  bitmap_and_compl (local_allocnos_bitmap, loop_node->mentioned_allocnos,
		    loop_node->border_allocnos);
  father = loop_node->father;
  EXECUTE_IF_SET_IN_BITMAP (local_allocnos_bitmap, 0, i, bi)
    if (father->regno_allocno_map [ALLOCNO_REGNO (allocnos [i])] == NULL)
      create_cap_allocno (allocnos [i]);
  EXECUTE_IF_SET_IN_BITMAP (local_allocnos_bitmap, 0, i, bi)
    {
      a = allocnos [i];
      cap = ALLOCNO_CAP (a);
      if (cap == NULL)
	continue;
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
	  if (father_a != NULL)
	    /* Upper level allocno might be not existing because it is
	       not mentioned or lived on the border.  It is just
	       living on bb start of the loop.  */
	    add_allocno_copy (cap, father_a, cp->freq, cp->move_insn);
	}
    }
}



#ifdef ENABLE_IRA_CHECKING
/* The function checks that there are no coalesced allocnos.  */
static void
check_coalesced_allocnos (void)
{
  int i;
  allocno_t a;

  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      ira_assert (ALLOCNO_FIRST_COALESCED_ALLOCNO (a) == a
		  && ALLOCNO_NEXT_COALESCED_ALLOCNO (a) == a);
    }
}
#endif

/* This entry function creates internal representation for IRA
   (allocnos, copies, loop tree nodes).  If LOOPS_P is zero the nodes
   corresponding to the loops (except the root which corresponds the
   all function) and correspondingly allocnos for the loops will be not
   created (it will be done only for basic blocks).  Such value is
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
  initiate_allocnos ();
  initiate_copies ();
  ira_assert (current_loops == NULL);
  flow_loops_find (&ira_loops);
  current_loops = &ira_loops;
  create_loop_tree_nodes (loops_p);
  form_loop_tree ();
  create_allocnos ();
  ira_costs ();
  ira_build_conflicts ();
  tune_allocno_costs_and_cover_classes ();
  if (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL
      || flag_ira_algorithm == IRA_ALGORITHM_MIXED)
    {
      local_allocnos_bitmap = ira_allocate_bitmap ();
      traverse_loop_tree (ira_loop_tree_root, NULL,
			  create_loop_tree_node_caps);
      ira_free_bitmap (local_allocnos_bitmap);
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
  basic_block bb;

  finish_loop_tree_nodes ();
  ira_assert  (current_loops == &ira_loops);
  flow_loops_free (&ira_loops);
  free_dominance_info (CDI_DOMINATORS);
  FOR_ALL_BB (bb)
    bb->loop_father = NULL;
  current_loops = NULL;
  finish_copies ();
  finish_allocnos ();
  finish_calls ();
}
