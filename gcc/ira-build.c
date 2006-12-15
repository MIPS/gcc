/* Building pseudos for IRA.
   Contributed by Vladimir Makarov.
   Copyright (C) 2006 Free Software Foundation, Inc.

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

static void initiate_pseudos (void);
static void check_pseudo_conflict_vec (pseudo_t, int);
static void add_pseudo_conflict (pseudo_t, pseudo_t);
static pseudo_t create_cap_pseudo (pseudo_t);
static void finish_pseudos (void);

static void initiate_copies (void);
static void finish_copies (void);

static void create_insn_pseudos (rtx, int);
static void create_bb_pseudos (struct ira_loop_tree_node *);
static void create_loop_pseudos (edge);
static void create_loop_tree_node_pseudos (struct ira_loop_tree_node *);
static void create_pseudos (void);
static void create_loop_tree_node_caps (struct ira_loop_tree_node *);

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

/* Map regno -> pseudo for the current loop tree node.  */
pseudo_t *regno_pseudo_map;

/* Array of references to all pseudos and its size.  The order number
   of the pseudo corresponds to the index in the array.  */
pseudo_t *pseudos;
int pseudos_num;

/* Array of references to copies and its size.  The order number of
   the copy corresponds to the index in the array.  */
copy_t *copies;
int copies_num;

/* Data flow data used for IRA data flow analysis.  */
struct df *build_df;



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
      ira_bb_nodes [i].regno_pseudo_map = NULL;
      ira_bb_nodes [i].mentioned_pseudos = NULL;
      ira_bb_nodes [i].modified_regnos = NULL;
      ira_bb_nodes [i].border_pseudos = NULL;
      ira_bb_nodes [i].local_copies = NULL;
    }
  ira_loop_nodes = ira_allocate (sizeof (struct ira_loop_tree_node)
				 * VEC_length (loop_p, ira_loops.larray));
  max_regno = max_reg_num ();
  for (i = 0; VEC_iterate (loop_p, ira_loops.larray, i, loop); i++)
    {
      if (loop != ira_loops.tree_root)
	{
	  ira_loop_nodes [i].regno_pseudo_map = NULL;
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
      ira_loop_nodes [i].regno_pseudo_map
	= ira_allocate (sizeof (pseudo_t) * max_regno);
      memset (ira_loop_nodes [i].regno_pseudo_map, 0,
	      sizeof (pseudo_t) * max_regno);
      ira_loop_nodes [i].mentioned_pseudos = ira_allocate_bitmap ();
      ira_loop_nodes [i].modified_regnos = ira_allocate_bitmap ();
      ira_loop_nodes [i].border_pseudos = ira_allocate_bitmap ();
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
    if (ira_loop_nodes [i].regno_pseudo_map != NULL)
      {
	ira_free_bitmap (ira_loop_nodes [i].local_copies);
	ira_free_bitmap (ira_loop_nodes [i].border_pseudos);
	ira_free_bitmap (ira_loop_nodes [i].modified_regnos);
	ira_free_bitmap (ira_loop_nodes [i].mentioned_pseudos);
	ira_free (ira_loop_nodes [i].regno_pseudo_map);
      }
  ira_free (ira_loop_nodes);
  for (i = 0; i < (unsigned int) last_basic_block_before_change; i++)
    {
      if (ira_bb_nodes [i].local_copies != NULL)
	ira_free_bitmap (ira_bb_nodes [i].local_copies);
      if (ira_bb_nodes [i].border_pseudos != NULL)
	ira_free_bitmap (ira_bb_nodes [i].border_pseudos);
      if (ira_bb_nodes [i].modified_regnos != NULL)
	ira_free_bitmap (ira_bb_nodes [i].modified_regnos);
      if (ira_bb_nodes [i].mentioned_pseudos != NULL)
	ira_free_bitmap (ira_bb_nodes [i].mentioned_pseudos);
      if (ira_bb_nodes [i].regno_pseudo_map != NULL)
	ira_free (ira_bb_nodes [i].regno_pseudo_map);
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
  if (loop->outer != NULL)
    add_loop_to_tree (loop->outer);
  if (ira_loop_nodes [loop->num].regno_pseudo_map != NULL
      && ira_loop_nodes [loop->num].inner == NULL)
    {
      /* We have not added loop node to the tree yet.  */
      loop_node = &ira_loop_nodes [loop->num];
      loop_node->loop = loop;
      loop_node->bb = NULL;
      for (father = loop->outer; father != NULL; father = father->outer)
	if (ira_loop_nodes [father->num].regno_pseudo_map != NULL)
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
     if (ira_loop_nodes [i].regno_pseudo_map != NULL)
       ira_loop_nodes [i].inner = NULL;
  FOR_EACH_BB_REVERSE (bb)
    {
      bb_node = &ira_bb_nodes [bb->index];
      bb_node->bb = bb;
      bb_node->loop = NULL;
      bb_node->inner = NULL;
      bb_node->next = NULL;
      for (father = bb->loop_father; father != NULL; father = father->outer)
	if (ira_loop_nodes [father->num].regno_pseudo_map != NULL)
	  break;
      add_loop_to_tree (father);
      loop_node = &ira_loop_nodes [father->num];
      bb_node->next = loop_node->inner;
      bb_node->father = loop_node;
      loop_node->inner = bb_node;
    }
  ira_loop_tree_root = IRA_LOOP_NODE_BY_INDEX (ira_loops.tree_root->num);
  ira_assert (ira_loop_tree_root->regno_pseudo_map != NULL);
}



/* Varray containing references to all created pseudos.  It is a
   container of array pseudos.  */
static varray_type pseudo_varray;

/* The function initializes data concerning pseudos.  */
static void
initiate_pseudos (void)
{
  VARRAY_GENERIC_PTR_NOGC_INIT (pseudo_varray, max_reg_num () * 2, "pseudos");
  pseudos = NULL;
  pseudos_num = 0;
  regno_pseudo_map = ira_allocate (max_reg_num () * sizeof (pseudo_t));
  memset (regno_pseudo_map, 0, max_reg_num () * sizeof (pseudo_t));
}

/* The function creates and returns pseudo corresponding to REGNO in
   LOOP_TREE_NODE.  */
pseudo_t
create_pseudo (int regno, struct ira_loop_tree_node *loop_tree_node)
{
  pseudo_t p;

  p = ira_allocate (sizeof (struct pseudo));
  PSEUDO_REGNO (p) = regno;
  PSEUDO_LOOP_TREE_NODE (p) = loop_tree_node;
  if (regno >= 0)
    {
      PSEUDO_NEXT_REGNO_PSEUDO (p) = regno_pseudo_map [regno];
      regno_pseudo_map [regno] = p;
      if (loop_tree_node->regno_pseudo_map [regno] == NULL)
	/* Remember that we can create temporary pseudos to break
	   cycles in register shuffle.  */
	loop_tree_node->regno_pseudo_map [regno] = p;
    }
  PSEUDO_CAP (p) = NULL;
  PSEUDO_CAP_MEMBER (p) = NULL;
  PSEUDO_NUM (p) = pseudos_num;
  PSEUDO_CONFLICT_PSEUDO_VEC (p) = NULL;
  PSEUDO_CONFLICT_PSEUDO_VEC_ACTIVE_SIZE (p) = 0;
  CLEAR_HARD_REG_SET (PSEUDO_CONFLICT_HARD_REGS (p));
  PSEUDO_FREQ (p) = 1;
  PSEUDO_HARD_REGNO (p) = -1;
  PSEUDO_CALL_FREQ (p) = 0;
  PSEUDO_CALLS_CROSSED_NUM (p) = 0;
  PSEUDO_CALLS_CROSSED (p) = NULL;
  /* ??? Too conservative.  */
  if (regno >= 0 && REG_N_CALLS_CROSSED (regno) != 0)
    PSEUDO_CALLS_CROSSED (p)
      = ira_allocate (sizeof (rtx) * REG_N_CALLS_CROSSED (regno));
#ifdef STACK_REGS
  PSEUDO_NO_STACK_REG_P (p) = FALSE;
#endif
  PSEUDO_IN_GRAPH_P (p) = FALSE;
  PSEUDO_ASSIGNED_P (p) = FALSE;
  PSEUDO_MAY_BE_SPILLED_P (p) = FALSE;
  PSEUDO_MODE (p) = (regno < 0 ? VOIDmode : PSEUDO_REGNO_MODE (regno));
  PSEUDO_COPIES (p) = NULL;
  PSEUDO_HARD_REG_COSTS (p) = NULL;
  PSEUDO_CONFLICT_HARD_REG_COSTS (p) = NULL;
  PSEUDO_CURR_HARD_REG_COSTS (p) = NULL;
  PSEUDO_CURR_CONFLICT_HARD_REG_COSTS (p) = NULL;
  PSEUDO_LEFT_CONFLICTS_NUM (p) = -1;
  PSEUDO_COVER_CLASS (p) = NO_REGS;
  PSEUDO_COVER_CLASS_COST (p) = 0;
  PSEUDO_MEMORY_COST (p) = 0;
  PSEUDO_NEXT_BUCKET_PSEUDO (p) = NULL;
  PSEUDO_PREV_BUCKET_PSEUDO (p) = NULL;
  VARRAY_PUSH_GENERIC_PTR (pseudo_varray, p);
  pseudos = (pseudo_t *) &VARRAY_GENERIC_PTR (pseudo_varray, 0);
  pseudos_num = VARRAY_ACTIVE_SIZE (pseudo_varray);
  return p;
}

/* The function allocates conflict vector of P for NUM pseudos.  */
void
allocate_pseudo_conflicts (pseudo_t p, int num)
{
  ira_assert (PSEUDO_CONFLICT_PSEUDO_VEC (p) == NULL);
  PSEUDO_CONFLICT_PSEUDO_VEC (p)
    = ira_allocate (sizeof (pseudo_t) * (num + 1));
  PSEUDO_CONFLICT_PSEUDO_VEC (p) [0] = NULL;
  PSEUDO_CONFLICT_PSEUDO_VEC_ACTIVE_SIZE (p) = 0;
  PSEUDO_CONFLICT_PSEUDO_VEC_SIZE (p) = num;
}

/* The function checks that conflict vector of P has enough space to
   contain NUM pseudo references.  If the space is not enough, the
   function expands the conflict vector.  */
static void
check_pseudo_conflict_vec (pseudo_t p, int num)
{
  int size;
  pseudo_t *vec;

  ira_assert (PSEUDO_CONFLICT_PSEUDO_VEC (p) != NULL);
  if (PSEUDO_CONFLICT_PSEUDO_VEC_SIZE (p) >= num)
    return;
  size = 3 * num / 2 + 1;
  vec = ira_allocate (sizeof (pseudo_t) * (size + 1));
  memcpy (vec, PSEUDO_CONFLICT_PSEUDO_VEC (p),
	  sizeof (pseudo_t)
	  * (PSEUDO_CONFLICT_PSEUDO_VEC_ACTIVE_SIZE (p) + 1));
  ira_free (PSEUDO_CONFLICT_PSEUDO_VEC (p));
  PSEUDO_CONFLICT_PSEUDO_VEC (p) = vec;
  PSEUDO_CONFLICT_PSEUDO_VEC_SIZE (p) = size;
}

/* The function adds P1 to conflict vector of P2 and vise versa.  */
static void
add_pseudo_conflict (pseudo_t p1, pseudo_t p2)
{
  int size1, size2;
  pseudo_t *vec1, *vec2;

  size1 = PSEUDO_CONFLICT_PSEUDO_VEC_ACTIVE_SIZE (p1);
  size2 = PSEUDO_CONFLICT_PSEUDO_VEC_ACTIVE_SIZE (p2);
  check_pseudo_conflict_vec (p1, size1 + 1);
  check_pseudo_conflict_vec (p2, size2 + 1);
  vec1 = PSEUDO_CONFLICT_PSEUDO_VEC (p1);
  vec2 = PSEUDO_CONFLICT_PSEUDO_VEC (p2);
  vec1 [size1] = p2;
  vec2 [size2] = p1;
  vec1 [size1 + 1] = NULL;
  vec2 [size2 + 1] = NULL;
  PSEUDO_CONFLICT_PSEUDO_VEC_ACTIVE_SIZE (p1)++;
  PSEUDO_CONFLICT_PSEUDO_VEC_ACTIVE_SIZE (p2)++;
}

/* This recursive function outputs pseudo P and if it is cap the
   function outputs members.  */
void
print_expanded_pseudo (pseudo_t p)
{
  basic_block bb;

  fprintf (ira_dump_file, " p%d(r%d", PSEUDO_NUM (p), PSEUDO_REGNO (p));
  if ((bb = PSEUDO_LOOP_TREE_NODE (p)->bb) != NULL)
    fprintf (ira_dump_file, ",b%d", bb->index);
  else
    fprintf (ira_dump_file, ",l%d", PSEUDO_LOOP_TREE_NODE (p)->loop->num);
  if (PSEUDO_REGNO (p) < 0)
    {
      fprintf (ira_dump_file, ":");
      print_expanded_pseudo (PSEUDO_CAP_MEMBER (p));
    }
  fprintf (ira_dump_file, ")");
}

/* The function creates and returns cap representing pseudo P in the
   father loop.  */
static pseudo_t
create_cap_pseudo (pseudo_t p)
{
  int i, regno, hard_regs_num, conflicts_num;
  int *reg_costs, *conflict_reg_costs;
  basic_block bb;
  pseudo_t cap, conflict_pseudo, conflict_father_pseudo;
  pseudo_t *pseudo_vec;
  struct ira_loop_tree_node *father;

  father = PSEUDO_LOOP_TREE_NODE (p)->father;
  cap = create_pseudo (-1, father);
  /* We just need to set a mode giving the same size.  */
  PSEUDO_MODE (cap) = PSEUDO_MODE (p);
  PSEUDO_COVER_CLASS (cap) = PSEUDO_COVER_CLASS (p);
  PSEUDO_AVAILABLE_REGS_NUM (cap) = PSEUDO_AVAILABLE_REGS_NUM (p);
  PSEUDO_CAP_MEMBER (cap) = p;
  hard_regs_num = class_hard_regs_num [PSEUDO_COVER_CLASS (p)];
  PSEUDO_HARD_REG_COSTS (cap) = reg_costs
    = ira_allocate (hard_regs_num * sizeof (int));
  memcpy (reg_costs, PSEUDO_HARD_REG_COSTS (p), hard_regs_num * sizeof (int));
  PSEUDO_CONFLICT_HARD_REG_COSTS (cap) = conflict_reg_costs
    = ira_allocate (hard_regs_num * sizeof (int));
  memcpy (conflict_reg_costs, PSEUDO_CONFLICT_HARD_REG_COSTS (p),
	  hard_regs_num * sizeof (int));
  PSEUDO_CURR_HARD_REG_COSTS (cap)
    = ira_allocate (hard_regs_num * sizeof (int));
  PSEUDO_CURR_CONFLICT_HARD_REG_COSTS (cap)
    = ira_allocate (hard_regs_num * sizeof (int));
  /* ??? costs, call_p etc.  */
  bitmap_set_bit (father->mentioned_pseudos, PSEUDO_NUM (cap));
  PSEUDO_CAP (p) = cap;
  PSEUDO_AVAILABLE_REGS_NUM (cap) = PSEUDO_AVAILABLE_REGS_NUM (p);
  PSEUDO_COVER_CLASS_COST (cap) = PSEUDO_COVER_CLASS_COST (p);
  PSEUDO_MEMORY_COST (cap) = PSEUDO_MEMORY_COST (p);
  PSEUDO_FREQ (cap) = PSEUDO_FREQ (p);
  PSEUDO_CALL_FREQ (cap) = PSEUDO_CALL_FREQ (p);
  IOR_HARD_REG_SET (PSEUDO_CONFLICT_HARD_REGS (cap),
		    PSEUDO_CONFLICT_HARD_REGS (p));
  PSEUDO_CALLS_CROSSED_NUM (cap) = PSEUDO_CALLS_CROSSED_NUM (p);
  PSEUDO_CALLS_CROSSED (cap)
    = ira_allocate (sizeof (rtx) * PSEUDO_CALLS_CROSSED_NUM (p));
  memcpy (PSEUDO_CALLS_CROSSED (cap), PSEUDO_CALLS_CROSSED (p),
	  sizeof (rtx) * PSEUDO_CALLS_CROSSED_NUM (p));
#ifdef STACK_REGS
  PSEUDO_NO_STACK_REG_P (cap) = PSEUDO_NO_STACK_REG_P (p);
#endif
  pseudo_vec = PSEUDO_CONFLICT_PSEUDO_VEC (p);
  for (conflicts_num = i = 0; (conflict_pseudo = pseudo_vec [i]) != NULL; i++)
    conflicts_num++;
  allocate_pseudo_conflicts (cap, conflicts_num);
  for (conflicts_num = i = 0; (conflict_pseudo = pseudo_vec [i]) != NULL; i++)
    {
      regno = PSEUDO_REGNO (conflict_pseudo);
      if (regno < 0)
	conflict_father_pseudo = PSEUDO_CAP (conflict_pseudo);
      else
	{
	  conflict_father_pseudo = father->regno_pseudo_map [regno];
	  if (conflict_father_pseudo == NULL)
	    conflict_father_pseudo = PSEUDO_CAP (conflict_pseudo);
	}
      if (conflict_father_pseudo != NULL)
	add_pseudo_conflict (cap, conflict_father_pseudo);
    }
  if (ira_dump_file != NULL)
    {
      fprintf (ira_dump_file, "  Creating cap ");
      print_expanded_pseudo (cap);
      fprintf (ira_dump_file, "\n    Conflicts:");
      pseudo_vec = PSEUDO_CONFLICT_PSEUDO_VEC (cap);
      for (i = 0; (conflict_pseudo = pseudo_vec [i]) != NULL; i++)
	{
	  fprintf (ira_dump_file, " p%d(r%d,", PSEUDO_NUM (conflict_pseudo),
		   PSEUDO_REGNO (conflict_pseudo));
	  if ((bb = PSEUDO_LOOP_TREE_NODE (conflict_pseudo)->bb) != NULL)
	    fprintf (ira_dump_file, "b%d)", bb->index);
	  else
	    fprintf (ira_dump_file, "l%d)",
		     PSEUDO_LOOP_TREE_NODE (conflict_pseudo)->loop->num);
	}
      fprintf (ira_dump_file, "\n");
    }
  return cap;
}

/* The function frees memory allocated for all pseudos.  */
static void
finish_pseudos (void)
{
  int i;
  pseudo_t p;

  for (i = 0; i < pseudos_num; i++)
    {
      p = pseudos [i];
      if (PSEUDO_CONFLICT_PSEUDO_VEC (p) != NULL)
	ira_free (PSEUDO_CONFLICT_PSEUDO_VEC (p));
      if (PSEUDO_HARD_REG_COSTS (p) != NULL)
	ira_free (PSEUDO_HARD_REG_COSTS (p));
      if (PSEUDO_CONFLICT_HARD_REG_COSTS (p) != NULL)
	ira_free (PSEUDO_CONFLICT_HARD_REG_COSTS (p));
      if (PSEUDO_CURR_HARD_REG_COSTS (p) != NULL)
	ira_free (PSEUDO_CURR_HARD_REG_COSTS (p));
      if (PSEUDO_CURR_CONFLICT_HARD_REG_COSTS (p) != NULL)
	ira_free (PSEUDO_CURR_CONFLICT_HARD_REG_COSTS (p));
      if (PSEUDO_CALLS_CROSSED (p) != NULL)
	ira_free (PSEUDO_CALLS_CROSSED (p));
      ira_free (p);
    }
  ira_free (regno_pseudo_map);
  VARRAY_FREE (pseudo_varray);
}



/* Varray containing references to all created copies.  It is a
   container of array copies.  */
static varray_type copy_varray;

/* The function initializes data concerning pseudo copies.  */
static void
initiate_copies (void)
{
  VARRAY_GENERIC_PTR_NOGC_INIT (copy_varray, get_max_uid (), "copies");
  copies = NULL;
  copies_num = 0;
}

/* The function creates and returns copy of pseudos FIRST and SECOND
   with frequency FREQ and move insn MOVE_INSN.  */
copy_t
create_copy (pseudo_t first, pseudo_t second, int freq, rtx move_insn)
{
  copy_t cp;

  cp = ira_allocate (sizeof (struct pseudo_copy));
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

/* This recursive function creates pseudos corresponding to
   pseudo-registers containing in X.  Nonzero OUTPUT_P means that X is
   lvalue.  */
static void
create_insn_pseudos (rtx x, int output_p)
{
  int i, j;
  const char *fmt;
  enum rtx_code code = GET_CODE (x);

  if (code == REG)
    {
      int regno;

      if ((regno = REGNO (x)) >= FIRST_PSEUDO_REGISTER)
	{
	  pseudo_t p;

	  if ((p = ira_curr_loop_tree_node->regno_pseudo_map [regno]) == NULL)
	    p = create_pseudo (regno, ira_curr_loop_tree_node);
	  
	  PSEUDO_FREQ (p) += REG_FREQ_FROM_BB (curr_bb);
	  bitmap_set_bit (ira_curr_loop_tree_node->mentioned_pseudos,
			  PSEUDO_NUM (p));
	  if (output_p)
	    bitmap_set_bit (ira_curr_loop_tree_node->modified_regnos, regno);
	}
      return;
    }
  else if (code == SET)
    {
      create_insn_pseudos (SET_DEST (x), TRUE);
      create_insn_pseudos (SET_SRC (x), FALSE);
      return;
    }
  else if (code == CLOBBER)
    {
      create_insn_pseudos (XEXP (x, 0), TRUE);
      return;
    }
  else if (code == MEM)
    {
      create_insn_pseudos (XEXP (x, 0), FALSE);
      return;
    }
  else if (code == PRE_DEC || code == POST_DEC || code == PRE_INC || 
	   code == POST_INC || code == POST_MODIFY || code == PRE_MODIFY)
    {
      create_insn_pseudos (XEXP (x, 0), TRUE);
      create_insn_pseudos (XEXP (x, 0), FALSE);
      return;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	create_insn_pseudos (XEXP (x, i), output_p);
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  create_insn_pseudos (XVECEXP (x, i, j), output_p);
    }
}

/* The function creates pseudos corresponding to pseudo-registers
   living in basic blocks represented by the corresponding loop tree
   node BB_NODE.  */
static void
create_bb_pseudos (struct ira_loop_tree_node *bb_node)
{
  basic_block bb;
  rtx insn;
  unsigned int i;
  bitmap_iterator bi;
  pseudo_t *map;

  curr_bb = bb = bb_node->bb;
  ira_assert (bb != NULL);
  FOR_BB_INSNS (bb, insn)
    if (INSN_P (insn))
      create_insn_pseudos (PATTERN (insn), FALSE);
  /* It might be a pseudo living through from one subloop to
     another.  */
  map = ira_curr_loop_tree_node->regno_pseudo_map;
  EXECUTE_IF_SET_IN_REG_SET (DF_UPWARD_LIVE_IN (build_df, bb),
			     FIRST_PSEUDO_REGISTER, i, bi)
    if (map [i] == NULL)
      create_pseudo (i, ira_curr_loop_tree_node);
}

/* The function creates pseudos corresponding to pseudo-registers
   living on edge E (a loop enter or exit).  It also finds pseudos
   living on the loop border. */
static void
create_loop_pseudos (edge e)
{
  unsigned int i;
  bitmap live_in_regs, border_pseudos;
  bitmap_iterator bi;
  pseudo_t *map;

  live_in_regs = DF_UPWARD_LIVE_IN (build_df, e->dest);
  map = ira_curr_loop_tree_node->regno_pseudo_map;
  border_pseudos = ira_curr_loop_tree_node->border_pseudos;
  EXECUTE_IF_SET_IN_REG_SET (DF_UPWARD_LIVE_OUT (build_df, e->src),
			     FIRST_PSEUDO_REGISTER, i, bi)
    if (bitmap_bit_p (live_in_regs, i))
      {
	if (map [i] == NULL)
	  create_pseudo (i, ira_curr_loop_tree_node);
	bitmap_set_bit (border_pseudos, PSEUDO_NUM (map [i]));
      }
}

/* The function creates pseudos corresponding to pseudo-registers
   living in loop represented by the corresponding loop tree node
   LOOP_NODE.  */
static void
create_loop_tree_node_pseudos (struct ira_loop_tree_node *loop_node)
{
  if (loop_node->bb != NULL)
    create_bb_pseudos (loop_node);
  else if (loop_node != ira_loop_tree_root)
    {
      int i;
      edge_iterator ei;
      edge e;
      VEC (edge, heap) *edges;

      FOR_EACH_EDGE (e, ei, loop_node->loop->header->preds)
	if (e->src != loop_node->loop->latch)
	  create_loop_pseudos (e);
      
      edges = get_loop_exit_edges (loop_node->loop);
      for (i = 0; VEC_iterate (edge, edges, i, e); i++)
	create_loop_pseudos (e);
      VEC_free (edge, heap, edges);
    }
}

/* The function creates pseudos corresponding to pseudo-registers in
   the current function.  The function traverses the loop tree for
   this.  */
static void
create_pseudos (void)
{
  int i;
  pseudo_t p, father_p;
  struct ira_loop_tree_node *father;

  traverse_loop_tree (ira_loop_tree_root, create_loop_tree_node_pseudos, NULL);
  if (flag_ira_algorithm != IRA_ALGORITHM_REGIONAL)
    return;
  /* Propagate frequencies for regional register allocator.  */
  for (i = max_reg_num () - 1; i >= FIRST_PSEUDO_REGISTER; i--)
    for (p = regno_pseudo_map [i]; p != NULL; p = PSEUDO_NEXT_REGNO_PSEUDO (p))
      if ((father = PSEUDO_LOOP_TREE_NODE (p)->father) != NULL
	  && (father_p = father->regno_pseudo_map [i]) != NULL)
	PSEUDO_FREQ (father_p) += PSEUDO_FREQ (p);
}



/* Bitmap of pseudos living only inside the current loop.  */
static bitmap local_pseudos_bitmap;

/* The function creates caps representing pseudos living only inside
   the loop given by LOOP_NODE on higher loop level.  */
static void
create_loop_tree_node_caps (struct ira_loop_tree_node *loop_node)
{
  unsigned int j;
  bitmap_iterator bi;
  pseudo_t p, cap, another_p, father_p;
  struct pseudo_copy *cp, *next_cp;
  struct ira_loop_tree_node *father;

  if (loop_node->bb != NULL || loop_node == ira_loop_tree_root)
    return;
  bitmap_and_compl (local_pseudos_bitmap, loop_node->mentioned_pseudos,
		    loop_node->border_pseudos);
  EXECUTE_IF_SET_IN_BITMAP (local_pseudos_bitmap, 0, j, bi)
    create_cap_pseudo (pseudos [j]);
  father = loop_node->father;
  EXECUTE_IF_SET_IN_BITMAP (local_pseudos_bitmap, 0, j, bi)
    {
      p = pseudos [j];
      cap = PSEUDO_CAP (p);
      for (cp = PSEUDO_COPIES (p); cp != NULL; cp = next_cp)
	{
	  if (cp->first == p)
	    {
	      next_cp = cp->next_first_pseudo_copy;
	      another_p = cp->second;
	    }
	  else if (cp->second == p)
	    {
	      next_cp = cp->next_second_pseudo_copy;
	      another_p = cp->first;
	    }
	  else
	    gcc_unreachable ();
	  if ((father_p = PSEUDO_CAP (another_p)) == NULL)
	    father_p = father->regno_pseudo_map [PSEUDO_REGNO (another_p)];
	  if (father_p != NULL)
	    /* Upper level pseudo might be not existing because it is
	       not mentioned or lived on the border.  It is just
	       living on bb start of the loop.  */
	    add_pseudo_copy (cap, father_p, cp->freq, cp->move_insn);
	}
    }
}



/* This entry function creates internal representation for IRA
   (pseudos, copies, loop tree nodes).  If LOOP_P is zero the nodes
   corresponding to the loops (except the root which corresponds the
   all function) and correspondingly pseudos for the loops will be not
   created (it will be done only for basic blocks).  Such value is
   used for Chaitin-Briggs and Chow's priority coloring.  */
void
ira_build (int loop_p)
{
  build_df = df_init (DF_HARD_REGS);
  df_lr_add_problem (build_df, 0);
  df_ri_add_problem (build_df, DF_RI_LIFE);
  df_analyze (build_df);

  initiate_pseudos ();
  initiate_copies ();
  ira_assert (current_loops == NULL);
  flow_loops_find (&ira_loops);
  current_loops = &ira_loops;
  create_loop_tree_nodes (loop_p);
  form_loop_tree ();
  free_dominance_info (CDI_DOMINATORS);
  create_pseudos ();
  ira_costs ();
  ira_build_conflicts ();
  tune_pseudo_costs_and_cover_classes ();
  if (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL)
    {
      local_pseudos_bitmap = ira_allocate_bitmap ();
      traverse_loop_tree (ira_loop_tree_root, NULL,
			  create_loop_tree_node_caps);
      ira_free_bitmap (local_pseudos_bitmap);
    }
}

/* The function releases data created by function ira_build.  */
void
ira_destroy (void)
{
  basic_block bb;

  finish_loop_tree_nodes ();
  ira_assert  (current_loops == &ira_loops);
  flow_loops_free (&ira_loops);
  FOR_ALL_BB (bb)
    bb->loop_father = NULL;
  current_loops = NULL;
  finish_copies ();
  finish_pseudos ();
  df_finish (build_df);
}
