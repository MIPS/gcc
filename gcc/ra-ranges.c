/* Graph coloring register allocator
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   Contributed by Michael Matz <matzmich@cs.tu-berlin.de>
   and Daniel Berlin <dan@cgsoftware.com>

   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify it under the
   terms of the GNU General Public License as published by the Free Software
   Foundation; either version 2, or (at your option) any later version.

   GNU CC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
   details.

   You should have received a copy of the GNU General Public License along
   with GNU CC; see the file COPYING.  If not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tm_p.h"
#include "insn-config.h"
#include "recog.h"
#include "function.h"
#include "regs.h"
#include "obstack.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "df.h"
#include "sbitmap.h"
#include "expr.h"
#include "output.h"
#include "toplev.h"
#include "flags.h"
#include "ggc.h"
#include "reload.h"
#include "real.h"
#include "pre-reload.h"
#include "ra.h"


static bool find_splits PARAMS ((unsigned int, int *));
static void splits_init PARAMS ((void));
static basic_block dom_parent PARAMS ((basic_block));
static void find_nesting_depths PARAMS ((void));
static void calculate_pre_post PARAMS ((void));
static unsigned int walk_dom_tree PARAMS ((basic_block, unsigned int *,
					   unsigned int));
static bool sp_back_edge PARAMS ((basic_block, basic_block));
static void reach_under PARAMS ((basic_block, basic_block, sbitmap));
static void dfs PARAMS ((unsigned int));
static bool dominates PARAMS ((basic_block, basic_block));
static unsigned int DFS_DJ_graph PARAMS ((basic_block, unsigned int *));
static void visit_successors PARAMS ((unsigned int, unsigned int));
static struct dj_graph_info *dj_graph_info;
static unsigned int max_level = 0;
static struct linked_list **levels;
static struct scc_info *scc_info;
static unsigned int dfs_stack;
static unsigned int next_dfs_num;
static unsigned int max_depth;
static unsigned int *depths;
static int *pre;
static int *pre_inverse;
static int *post;
dominator_tree domtree;

static
void calculate_pre_post ()
{
  edge *stack;
  int i;
  int sp;
  int prenum = 0;
  int postnum = 0;
  sbitmap visited;

  /* Allocate the preorder and postorder number arrays.  */
  pre = (int *) xcalloc (last_basic_block+1, sizeof (int));
  post = (int *) xcalloc (last_basic_block+1, sizeof (int));
  pre_inverse = (int *) xcalloc (last_basic_block+1, sizeof (int));
  
  /* Allocate stack for back-tracking up CFG.  */
  stack = (edge *) xmalloc ((last_basic_block + 1) * sizeof (edge));
  sp = 0;

  /* Allocate bitmap to track nodes that have been visited.  */
  visited = sbitmap_alloc (last_basic_block);

  /* None of the nodes in the CFG have been visited yet.  */
  sbitmap_zero (visited);

  /* Push the first edge on to the stack.  */
  stack[sp++] = ENTRY_BLOCK_PTR->succ;

  while (sp)
    {
      edge e;
      basic_block src;
      basic_block dest;

      /* Look at the edge on the top of the stack.  */
      e = stack[sp - 1];
      src = e->src;
      dest = e->dest;

      /* Check if the edge destination has been visited yet.  */
      if (dest != EXIT_BLOCK_PTR && ! TEST_BIT (visited, dest->index))
	{
	  /* Mark that we have visited the destination.  */
	  SET_BIT (visited, dest->index);

	  pre[dest->index] = prenum++;

	  if (dest->succ)
	    {
	      /* Since the DEST node has been visited for the first
		 time, check its successors.  */
	      stack[sp++] = dest->succ;
	    }
	  else
	    post[dest->index] = postnum++;
	}
      else
	{
	  if (! e->succ_next && src != ENTRY_BLOCK_PTR)
	    post[src->index] = postnum++;

	  if (e->succ_next)
	    stack[sp - 1] = e->succ_next;
	  else
	    sp--;
	}
    }


  free (stack);
  sbitmap_free (visited);
  for (i = 0; i < last_basic_block; i++)
    pre_inverse[pre[i]] = i;
}
static basic_block
dom_parent (block)
     basic_block block;
{
  int fpred,dedge, index;
  fpred = DTREE_NODE (domtree, dom_node_for_block (domtree, block->index))->fpred;
  dedge = DTREE_EDGE (domtree, fpred)->from;
  index = DTREE_NODE (domtree, dedge)->index;
  if (index >= 0)
    return BASIC_BLOCK (index);
  if (index == ENTRY_BLOCK)
    return ENTRY_BLOCK_PTR;
  if (index == EXIT_BLOCK)
    return EXIT_BLOCK_PTR;
  abort();
}

  

static unsigned int
walk_dom_tree (block, index, level)
     basic_block block;
     unsigned int *index;
     unsigned int level;
{
  unsigned int size = 1;
  int preorder_index = pre [block->index];
  unsigned int dom_index = (*index)++;
  int child;
  dj_graph_info[preorder_index].dom_index = dom_index;
  dj_graph_info[preorder_index].dom_level = level;
  if (level > max_level) max_level = level;
  if (dom_node_for_block (domtree, block->index) < 0)
    child = -1;
  else
    child = DTREE_NODE (domtree,
			dom_node_for_block (domtree, block->index))->fsucc;
  while (child >= 0)
    {
      struct dom_edge *edge;
      basic_block block;
      edge = DTREE_EDGE (domtree, child);
      block = BASIC_BLOCK (DTREE_NODE (domtree, edge->to)->index);
      size += walk_dom_tree (block, index, level + 1);
      child =  edge->succ;
    }
  return dj_graph_info[preorder_index].dom_size = size;
}
static bool
sp_back_edge (parent, child)
     basic_block parent;
     basic_block child;
{
  unsigned int p_index = dj_graph_info[pre[parent->index]].dfs_index;
  unsigned int p_size = dj_graph_info[pre[parent->index]].dfs_size;
  unsigned int c_index = dj_graph_info[pre[child->index]].dfs_index;
  return p_index <= c_index && c_index < p_index + p_size;
}
static void
reach_under (block, head, loop)
     basic_block block;
     basic_block head;
     sbitmap loop;
{
  sbitmap worklist;
  edge edge;
  
  worklist = sbitmap_alloc (last_basic_block + 1);
  sbitmap_zero (worklist);
  SET_BIT (worklist, pre[block->index]);
  SET_BIT (worklist, pre[head->index]);
  for (edge = block->pred; edge; edge = edge->pred_next)
    {
      basic_block predblock = edge->src;
      int pred_index = pre[predblock->index];
      if (dominates (head, predblock))
	SET_BIT (worklist, pred_index);
    }
  while (sbitmap_first_set_bit (worklist) >= 0)
    {
      int index = sbitmap_first_set_bit (worklist);
      RESET_BIT (worklist, index);
      if (!TEST_BIT (loop, index))
	{
	  SET_BIT (loop, index);
	  for (edge = BASIC_BLOCK (pre_inverse[index])->pred; edge; edge = edge->pred_next)
	    {
	      basic_block predblock = edge->src;
	      int pred_index = pre[predblock->index];
	      if (dominates (head, predblock))
		SET_BIT (worklist, pred_index);
	    }
	}
    }
  sbitmap_free (worklist);
}

	      
      
static bool
dominates (parent, child)
     basic_block parent;
     basic_block child;
{
  unsigned int p_index = dj_graph_info[pre[parent->index]].dom_index;
  unsigned int p_size = dj_graph_info[pre[parent->index]].dom_size;
  unsigned int c_index = dj_graph_info[pre[child->index]].dom_index;
  return p_index <= c_index && c_index < p_index + p_size;
}
static void
dfs (i)
     unsigned int i;
{
  scc_info[i].visited = TRUE;
  scc_info[i].dfs_num = scc_info[i].low = next_dfs_num++;
  scc_info[i].in_stack = TRUE;
  scc_info[i].next = dfs_stack;
  dfs_stack = i;
  
  visit_successors (i, i);
  if (scc_info[i].low == scc_info[i].dfs_num)
    {
      unsigned int name = dfs_stack;
      if (i == name)
	{
	  unsigned int name = dfs_stack;
	  scc_info[name].in_stack = FALSE;
	  dfs_stack = scc_info[name].next;
	}
      else
	{
	  unsigned int scc = 0;
	  do
	    {
	      name = dfs_stack;
	      scc_info[name].in_stack = FALSE;
	      dfs_stack = scc_info[name].next;
	      scc_info[name].next = scc;
	      scc = name;
	    } while (name != i);
	  for (name = scc; name; name = scc_info[name].next)
	    {
	      unsigned int depth = ++(depths[i]);
	      if (depth > max_depth) max_depth = depth;
	      if (!dj_graph_info[name].loop_head || dj_graph_info[i].loop_head == name)
		{
		  dj_graph_info[name].loop_head = i;
		  if (name != i)
		    {
		      struct linked_list * node = (struct linked_list *) ggc_alloc (sizeof (struct linked_list));
		      node->name = name;
		      node->next = dj_graph_info[i].children;
		      dj_graph_info[i].children = node;
		    }		 
		}
	    }
	}
    }
}
static void
visit_successors PARAMS ((pred, i))
     unsigned int pred;
     unsigned int i;
{
  basic_block block;
  edge edge;
  struct linked_list *node;

  block = BASIC_BLOCK (pre_inverse[i]);
  for (edge = block->succ; edge; edge = edge->succ_next)
    {
      int succ_index = pre[edge->dest->index];
      if (!dj_graph_info[succ_index].loop_head)
	{
	  if (!scc_info[succ_index].visited)
	    {
	      dfs (succ_index);
	      scc_info[pred].low = MIN(scc_info[pred].low, scc_info[succ_index].low);
	    }
	  if (scc_info[succ_index].dfs_num < scc_info[pred].dfs_num
	      && scc_info[succ_index].in_stack)
	    scc_info[pred].low = MIN(scc_info[succ_index].dfs_num, scc_info[pred].low);
	}
    }
  for (node = dj_graph_info[i].children; node; node = node->next)
    visit_successors (pred, node->name);
}

static sbitmap visited;

static unsigned int
DFS_DJ_graph (block, index)
     basic_block block;
     unsigned int *index;
{
  unsigned int size = 1;
  int preorder_index = pre[block->index];
  edge succ;
  int child;
  SET_BIT (visited, block->index);
  dj_graph_info[preorder_index].dfs_index = (*index)++; 
  if (dom_node_for_block (domtree, block->index) < 0)
    child = -1;
  else
    child = DTREE_NODE (domtree,
			dom_node_for_block (domtree, block->index))->fsucc;
  while (child >= 0)
    {
      struct dom_edge *edge;
      basic_block childblock;
      edge = DTREE_EDGE (domtree, child);
      childblock = BASIC_BLOCK (DTREE_NODE (domtree, edge->to)->index);
      size += DFS_DJ_graph (childblock, index);
      child =  edge->succ;
    }
  for (succ = block->succ; succ; succ = succ->succ_next)
    {
      basic_block next_block = succ->dest;
      if (next_block->index >= 0)
	{
	  if (!TEST_BIT (visited, next_block->index))
	    size += DFS_DJ_graph (next_block, index);
	}
    }
  return dj_graph_info[preorder_index].dfs_size = size;
}
static void 
find_nesting_depths()
{
  unsigned int *depthtemp;
  unsigned int index = 1;
  unsigned int level = 0;
  int i;
  int *idom = (int *)alloca (last_basic_block * sizeof (int));
  sbitmap loop;
  
  memset (idom, -1, (size_t) last_basic_block * sizeof (int));
  calculate_pre_post ();
  depths = (unsigned int *)xcalloc (last_basic_block + 1, sizeof(unsigned int));
  depthtemp = (unsigned int *)xcalloc (last_basic_block + 1, sizeof (unsigned int));
  calculate_dominance_info (idom, NULL, CDI_DOMINATORS);
  domtree = dom_tree_from_idoms (idom);
  dj_graph_info = (struct dj_graph_info *) ggc_alloc_cleared
    ((last_basic_block + 1) * sizeof (struct dj_graph_info));
  walk_dom_tree (BASIC_BLOCK (0), &index,  level);    
  levels = (struct linked_list **) ggc_alloc_cleared ((max_level + 1) * sizeof (struct linked_list *));
  for (i = 0; i < last_basic_block; i++)
    {
      basic_block block = BASIC_BLOCK (i);
      int block_num = pre[block->index];
      struct linked_list *node;

      level = dj_graph_info[block_num].dom_level;
      node = (struct linked_list *) ggc_alloc (sizeof (struct linked_list));
      node->name = block_num;
      node->next = levels[level];
      levels[level] = node;
    }
  index = 1;
  visited = sbitmap_alloc (last_basic_block);
  sbitmap_zero (visited);
  DFS_DJ_graph (BASIC_BLOCK (0), &index);
  loop = sbitmap_alloc (last_basic_block + 1);
  sbitmap_zero (loop);
  
  for (i = max_level; i >= 0; i--)
    {
      bool irreducible_loop = FALSE;
      struct linked_list *node;
      for (node = levels[i]; node; node = node->next)
	{
	  basic_block block = BASIC_BLOCK (pre_inverse[node->name]);
	  edge edge;
	  sbitmap_zero (loop);
	  for (edge = block->pred; edge; edge = edge->pred_next)
	    {
	      basic_block pred = edge->src;
	      if (pred != dom_parent (block))
		{
		  if (!dominates (block, pred) && sp_back_edge (block, pred))
		    irreducible_loop = TRUE;
		  if (dominates (block, pred))
		    reach_under (pred, block, loop);
		}
	    }
	  if (sbitmap_first_set_bit (loop) >= 0)
	    {
	      int block_index = pre [block->index];
	      unsigned int i;
	      EXECUTE_IF_SET_IN_SBITMAP (loop, 0, i, 
	      {
		unsigned int depth = ++(depths[i]);
		if (depth > max_depth) max_depth = depth;
		if (!dj_graph_info[i].loop_head || dj_graph_info[i].loop_head == i)
		  {
		    dj_graph_info[i].loop_head = block_index;
		    if ((int )i != block_index)
		      {
			struct linked_list *node = (struct linked_list *) ggc_alloc (sizeof (struct linked_list));
			node->name = i;
			node->next = dj_graph_info[block_index].children;
			dj_graph_info[block_index].children = node;
		      }
		  }
	      });
	    }
	  if (irreducible_loop)
	    {
	      unsigned int j;
	      scc_info = (struct scc_info *) ggc_alloc_cleared (sizeof (struct
									scc_info)
								* (last_basic_block + 1));
	      next_dfs_num = 0;
	      for (j = i; j <= max_depth; j++)
		{
		  struct linked_list *node;
		  for (node = levels[j]; node; node = node->next)
		    {
		      int index = node->name;
		      if (!dj_graph_info[index].loop_head && !scc_info[index].visited)
			dfs(i);
		    }
		}
	    }
	}
    }
  memcpy (depthtemp, depths, sizeof (unsigned int) * (last_basic_block));
  for (i = 0; i < last_basic_block; i ++)
	depths[i] = depthtemp[pre[i]];
  free (depthtemp);
  sbitmap_free (loop);
  sbitmap_free (visited);
  free (pre);
  free (post);
  free (pre_inverse);
  destroy_dominator_tree (domtree);
}
bool split_live_ranges = FALSE;
static bool any_splits_found;
struct split_range
{
  double loads;
  double stores;
};
static struct split_range *split_ranges;

static struct linked_list **split_around;
static struct linked_list **neighbors_with_color;
static inline double powraise PARAMS ((unsigned int));
static inline double 
powraise (power)
     unsigned int power;
{
  double i = 1.0;
  while (power--)
      i *= 10.0;
  return i;
}
static bool 
find_splits (name, colors)
     unsigned int name ATTRIBUTE_UNUSED;
     int *colors ATTRIBUTE_UNUSED;
{
  return FALSE;
}
static void 
splits_init ()
{
  if (split_live_ranges)
    {
      unsigned int i;
      split_ranges = (struct split_range *)ggc_alloc_cleared (num_webs * sizeof (struct split_range));
      split_around = (struct linked_list **)ggc_alloc_cleared (num_webs * sizeof (struct linked_list *));
      for (i = 0; i < num_webs; i++)
	{
	  unsigned int j;
	  struct web *web = ID2WEB (i);
	  for (j = 0; j < web->num_defs; j++)
	    split_ranges[i].stores += powraise (depths[DF_REF_BB (web->defs[j])->index]);
	  for (j = 0; j < web->num_uses; j++)
	    split_ranges[i].loads += powraise (depths[DF_REF_BB (web->uses[j])->index]);
	}
      any_splits_found = FALSE;
    }
}

