/* Loop manipulation code for GNU compiler.
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "cfglayout.h"
#include "output.h"

static struct loop * duplicate_loop	PARAMS ((struct loops *,
						struct loop *, struct loop *, int));
static void scale_loop_histograms	PARAMS ((struct loop *, int));
static void duplicate_subloops		PARAMS ((struct loops *, struct loop *,
						struct loop *, int));
static void copy_loops_to		PARAMS ((struct loops *, struct loop **,
						int, struct loop *, int));
static void loop_redirect_edge		PARAMS ((edge, basic_block));
static bool loop_delete_branch_edge	PARAMS ((edge));
static void copy_bbs			PARAMS ((basic_block *, int, edge,
						edge, basic_block **,
						struct loops *, edge *,
						edge *, int));
static void remove_bbs			PARAMS ((dominance_info, basic_block *,
						int));
static bool rpe_enum_p			PARAMS ((basic_block, void *));
static int find_branch			PARAMS ((edge, dominance_info,
						basic_block **));
static bool alp_enum_p			PARAMS ((basic_block, void *));
static void add_loop			PARAMS ((struct loops *, struct loop *));
static void fix_loop_placements		PARAMS ((struct loop *));
static int fix_bb_placement		PARAMS ((struct loops *, basic_block));
static void fix_bb_placements		PARAMS ((struct loops *, basic_block));
static void place_new_loop		PARAMS ((struct loops *, struct loop *));
static void scale_loop_frequencies	PARAMS ((struct loop *, int, int));
static void scale_bbs_frequencies	PARAMS ((basic_block *, int, int, int));
static void record_exit_edges		PARAMS ((edge, basic_block *, int,
						edge *, unsigned *, int));
static basic_block create_preheader	PARAMS ((struct loop *, dominance_info,
						int));

/* Splits basic block BB after INSN, returns created edge.  Updates loops
   and dominators.  */
edge
split_loop_bb (loops, bb, insn)
     struct loops *loops;
     basic_block bb;
     rtx insn;
{
  edge e;
  basic_block *dom_bbs;
  int n_dom_bbs, i;

  /* Split the block.  */
  e = split_block (bb, insn);

  /* Add dest to loop.  */
  add_bb_to_loop (e->dest, e->src->loop_father);

  /* Fix dominators.  */
  add_to_dominance_info (loops->cfg.dom, e->dest);
  n_dom_bbs = get_dominated_by (loops->cfg.dom, e->src, &dom_bbs);
  for (i = 0; i < n_dom_bbs; i++)
    set_immediate_dominator (loops->cfg.dom, dom_bbs[i], e->dest);
  free (dom_bbs);
  set_immediate_dominator (loops->cfg.dom, e->dest, e->src);

  /* Take care of RBI.  */
  alloc_aux_for_block (e->dest, sizeof (struct reorder_block_def));

  return e;
}

/* Checks whether BB is inside RPE_LOOP and is dominated by RPE_DOM.  */
struct rpe_data
 {
   basic_block dom;
   dominance_info doms;
 };

static bool
rpe_enum_p (bb, data)
     basic_block bb;
     void *data;
{
  struct rpe_data *rpe = data;
  return dominated_by_p (rpe->doms, bb, rpe->dom);
}

/* Remove BBS.  */
static void
remove_bbs (dom, bbs, nbbs)
     dominance_info dom;
     basic_block *bbs;
     int nbbs;
{
  int i;
  edge ae;

  for (i = 0; i < nbbs; i++)
    {
      edge next_ae;
      for (ae = bbs[i]->succ; ae; ae = next_ae)
	{
	  next_ae = ae->succ_next;
	  remove_edge (ae);
	}
      remove_bb_from_loops (bbs[i]);
      delete_from_dominance_info (dom, bbs[i]);
      flow_delete_block (bbs[i]);
    }
}

/* Find branch beginning at Edge and put it into BBS.  */
static int
find_branch (e, doms, bbs)
     edge e;
     dominance_info doms;
     basic_block **bbs;
{
  edge ae = NULL;
  struct rpe_data rpe;

  if (e->dest->pred->pred_next)
    {
      for (ae = e->dest->pred; ae; ae = ae->pred_next)
	if (ae != e && !dominated_by_p (doms, ae->src, e->dest))
	  break;
    }
  if (ae)
    {
      /* Just the edge.  */
      *bbs = NULL;
      return 0;
    }

  /* Find bbs we are interested in.  */
  rpe.dom = e->dest;
  rpe.doms = doms;
  *bbs = xcalloc (n_basic_blocks, sizeof (basic_block));
  return dfs_enumerate_from (e->dest, 0, rpe_enum_p, *bbs,
			     n_basic_blocks, &rpe);
}

/* Fix BB placement inside loop hierarchy.  */
static int
fix_bb_placement (loops, bb)
     struct loops *loops;
     basic_block bb;
{
  edge e;
  struct loop *loop = loops->tree_root, *act;

  for (e = bb->succ; e; e = e->succ_next)
    {
      if (e->dest == EXIT_BLOCK_PTR)
	continue;

      act = e->dest->loop_father;
      if (act->header == e->dest)
	act = act->outer;

      if (flow_loop_nested_p (loop, act))
	loop = act;
    }

  if (loop == bb->loop_father)
    return 0;

  remove_bb_from_loops (bb);
  add_bb_to_loop (bb, loop);

  return 1;
}

/* Fix bb placements, starting from FROM.  Also fix placement of subloops
   of FROM->loop_father.  */
static void
fix_bb_placements (loops, from)
     struct loops *loops;
     basic_block from;
{
  sbitmap in_queue;
  basic_block *queue, *qtop, *qbeg, *qend;
  struct loop *base_loop;
  edge e;

  /* We pass through blocks back-reachable from FROM, testing whether some
     of their successors moved to outer loop.  It may be necessary to
     iterate several times, but it is finite, as we stop unless we move
     the basic block up the loop structure.  The whole story is a bit
     more complicated due to presence of subloops, those are moved using
     fix_loop_placement.  */

  base_loop = from->loop_father;
  if (base_loop == loops->tree_root)
    return;

  in_queue = sbitmap_alloc (last_basic_block);
  sbitmap_zero (in_queue);
  SET_BIT (in_queue, from->index);
  /* Prevent us from going out of the base_loop.  */
  SET_BIT (in_queue, base_loop->header->index);

  queue = xcalloc (base_loop->num_nodes + 1, sizeof (basic_block));
  qtop = queue + base_loop->num_nodes + 1;
  qbeg = queue;
  qend = queue + 1;
  *qbeg = from;

  while (qbeg != qend)
    {
      /* Get element from queue.  */
      from = *qbeg;
      qbeg++;
      if (qbeg == qtop)
	qbeg = queue;
      RESET_BIT (in_queue, from->index);

      if (from->loop_father->header == from)
	{
	  /* Subloop header, maybe move the loop upward.  */
	  if (!fix_loop_placement (from->loop_father))
	    continue;
	}
      else
	{
	  if (!fix_bb_placement (loops, from))
	    continue;
	}

      /* Something has changed, insert predecessors into queue.  */
      for (e = from->pred; e; e = e->pred_next)
	{
	  basic_block pred = e->src;
	  struct loop *nca;

	  if (TEST_BIT (in_queue, pred->index))
	    continue;

	  /* If it is subloop, then it either was not moved, or 
	     the path up the loop tree from base_loop do not contain
	     it.  */
	  nca = find_common_loop (pred->loop_father, base_loop);
	  if (pred->loop_father != base_loop
	      && (nca == base_loop
		  || nca != pred->loop_father))
	    pred = pred->loop_father->header;
	  else if (!flow_loop_nested_p (from->loop_father, pred->loop_father))
	    {
	      /* No point in processing it.  */
	      continue;
	    }

	  if (TEST_BIT (in_queue, pred->index))
	    continue;

	  /* Schedule the basic block.  */
	  *qend = pred;
	  qend++;
	  if (qend == qtop)
	    qend = queue;
	  SET_BIT (in_queue, pred->index);
	}
    }
}

/* Removes path beginning at E.  */
bool
remove_path (loops, e)
     struct loops *loops;
     edge e;
{
  edge ae;
  basic_block *rem_bbs, *bord_bbs, *dom_bbs, from, bb;
  int i, nrem, n_bord_bbs, n_dom_bbs;
  sbitmap seen;

  /* First identify the branch.  */
  nrem = find_branch (e, loops->cfg.dom, &rem_bbs);

  /* Find blocks whose immediate dominators may be affected.  */
  n_dom_bbs = 0;
  n_bord_bbs = 0;
  bord_bbs = xcalloc (n_basic_blocks, sizeof (basic_block));
  seen = sbitmap_alloc (last_basic_block);
  sbitmap_zero (seen);

  /* Find border hexes.  */
  for (i = 0; i < nrem; i++)
    SET_BIT (seen, rem_bbs[i]->index);
  if (nrem)
    {
      for (i = 0; i < nrem; i++)
	{
	  bb = rem_bbs[i];
	  for (ae = rem_bbs[i]->succ; ae; ae = ae->succ_next)
	    if (ae->dest != EXIT_BLOCK_PTR && !TEST_BIT (seen, ae->dest->index))
	      {
		SET_BIT (seen, ae->dest->index);
		bord_bbs[n_bord_bbs++] = ae->dest;
	      }
	}
    }
  else if (e->dest != EXIT_BLOCK_PTR)
    bord_bbs[n_bord_bbs++] = e->dest;

  /* OK. Remove the path.  */
  from = e->src;
  if (!loop_delete_branch_edge (e))
    {
      free (rem_bbs);
      free (bord_bbs);
      free (seen);
      return false;
    }
  dom_bbs = xcalloc (n_basic_blocks, sizeof (basic_block));

  /* Now cancel contained loops.  */
  for (i = 0; i < nrem; i++)
    if (rem_bbs[i]->loop_father->header == rem_bbs[i])
      cancel_loop_tree (loops, rem_bbs[i]->loop_father);

  remove_bbs (loops->cfg.dom, rem_bbs, nrem);
  free (rem_bbs);

  /* Find blocks with affected dominators.  */
  sbitmap_zero (seen);
  for (i = 0; i < n_bord_bbs; i++)
    {
      int j, nldom;
      basic_block *ldom;

      bb = get_immediate_dominator (loops->cfg.dom, bord_bbs[i]);
      if (TEST_BIT (seen, bb->index))
	continue;
      SET_BIT (seen, bb->index);

      nldom = get_dominated_by (loops->cfg.dom, bb, &ldom);
      for (j = 0; j < nldom; j++)
	if (!dominated_by_p (loops->cfg.dom, from, ldom[j]))
	  dom_bbs[n_dom_bbs++] = ldom[j];
      free(ldom);
    }

  free (bord_bbs);
  free (seen);

  /* Recount dominators.  */
  iterate_fix_dominators (loops->cfg.dom, dom_bbs, n_dom_bbs);
  free (dom_bbs);

  /* Fix placements of basic blocks inside loops.  */
  fix_bb_placements (loops, from);

  /* Fix loop placements.  */
  fix_loop_placements (from->loop_father);

  return true;
}

/* Predicate for enumeration in add_loop.  */
static bool
alp_enum_p (bb, alp_header)
     basic_block bb;
     void *alp_header;
{
  return bb != (basic_block) alp_header;
}

/* Compute loop from header and latch info filled in LOOP and add it to
   LOOPS.  */
static void
add_loop (loops, loop)
     struct loops *loops;
     struct loop *loop;
{
  basic_block *bbs;
  int i, n;
  
  /* Add it to loop structure.  */
  place_new_loop (loops, loop);
  loop->level = 1;

  /* Find its nodes.  */
  bbs = xcalloc (n_basic_blocks, sizeof (basic_block));
  n = dfs_enumerate_from (loop->latch, 1, alp_enum_p, bbs, n_basic_blocks, loop->header);

  /* Add those nodes.  */
  for (i = 0; i < n; i++)
    add_bb_to_loop (bbs[i], loop);
  add_bb_to_loop (loop->header, loop);

  free (bbs);
}

/* Multiplies all frequencies of BBS by NUM/DEN.  */
static void
scale_bbs_frequencies (bbs, nbbs, num, den)
     basic_block *bbs;
     int nbbs;
     int num;
     int den;
{
  int i;
  edge e;

  for (i = 0; i < nbbs; i++)
    {
      bbs[i]->frequency = (bbs[i]->frequency * num) / den;
      bbs[i]->count = (bbs[i]->count * num) / den;
      for (e = bbs[i]->succ; e; e = e->succ_next)
	e->count = (e->count * num) /den;
    }
}

/* Multiplies all frequencies in LOOP by NUM/DEN.  */
static void
scale_loop_frequencies (loop, num, den)
     struct loop *loop;
     int num;
     int den;
{
  basic_block *bbs;

  bbs = get_loop_body (loop);
  scale_bbs_frequencies (bbs, loop->num_nodes, num, den);
  free (bbs);
}

/* Make area between HEADER_EDGE and LATCH_EDGE a loop by connecting
   latch to header.  Everything between them plus LATCH_EDGE destination
   must be dominated by HEADER_EDGE destination, and back-reachable from
   LATCH_EDGE source.  Add SWITCH_BB to original entry edge.  Returns newly
   created loop.  */
struct loop *
loopify (loops, latch_edge, header_edge, switch_bb)
     struct loops *loops;
     edge latch_edge;
     edge header_edge;
     basic_block switch_bb;
{
  basic_block succ_bb = latch_edge->dest;
  basic_block pred_bb = header_edge->src;
  basic_block *dom_bbs, *body;
  unsigned n_dom_bbs, i, j;
  sbitmap seen;
  struct loop *loop = xcalloc (1, sizeof (struct loop));
  struct loop *outer = succ_bb->loop_father->outer;
  int freq, prob, tot_prob;
  gcov_type cnt;

  loop->header = header_edge->dest;
  loop->latch = latch_edge->src;

  freq = EDGE_FREQUENCY (header_edge);
  cnt = header_edge->count;
  prob = switch_bb->succ->probability;
  tot_prob = prob + switch_bb->succ->succ_next->probability;
  if (tot_prob == 0)
    tot_prob = 1;

  /* Redirect edges.  */
  loop_redirect_edge (latch_edge, loop->header);
  loop_redirect_edge (header_edge, switch_bb);
  loop_redirect_edge (switch_bb->succ->succ_next, loop->header);
  loop_redirect_edge (switch_bb->succ, succ_bb);

  /* And update dominators.  */
  set_immediate_dominator (loops->cfg.dom, switch_bb, pred_bb);
  set_immediate_dominator (loops->cfg.dom, loop->header, switch_bb);
  set_immediate_dominator (loops->cfg.dom, succ_bb, switch_bb);

  /* Compute new loop.  */
  add_loop (loops, loop);
  flow_loop_tree_node_add (outer, loop);

  /* And add switch_bb to appropriate loop.  */
  add_bb_to_loop (switch_bb, outer);

  /* Now fix frequencies.  */
  switch_bb->frequency = freq;
  switch_bb->count = cnt;
  switch_bb->succ->count =
   (switch_bb->count * switch_bb->succ->probability) / REG_BR_PROB_BASE;
  switch_bb->succ->succ_next->count =
   (switch_bb->count * switch_bb->succ->succ_next->probability) / REG_BR_PROB_BASE;
  scale_loop_frequencies (loop, prob, tot_prob);
  scale_loop_frequencies (succ_bb->loop_father, tot_prob - prob, tot_prob);

  /* Update dominators of outer blocks.  */
  dom_bbs = xcalloc (n_basic_blocks, sizeof (basic_block));
  n_dom_bbs = 0;
  seen = sbitmap_alloc (last_basic_block);
  sbitmap_zero (seen);
  body = get_loop_body (loop);

  for (i = 0; i < loop->num_nodes; i++)
    SET_BIT (seen, body[i]->index);

  for (i = 0; i < loop->num_nodes; i++)
    {
      unsigned nldom;
      basic_block *ldom;

      nldom = get_dominated_by (loops->cfg.dom, body[i], &ldom);
      for (j = 0; j < nldom; j++)
	if (!TEST_BIT (seen, ldom[j]->index))
	  {
	    SET_BIT (seen, ldom[j]->index);
	    dom_bbs[n_dom_bbs++] = ldom[j];
	  }
      free (ldom);
    }

  iterate_fix_dominators (loops->cfg.dom, dom_bbs, n_dom_bbs);

  free (body);
  free (seen);
  free (dom_bbs);

  return loop;
}

/* Move LOOP up the hierarchy while it is not backward reachable from the
   latch of the outer loop.  */
int
fix_loop_placement (loop)
     struct loop *loop;
{
  basic_block *body;
  unsigned i;
  edge e;
  struct loop *father = loop->pred[0], *act;

  body = get_loop_body (loop);
  for (i = 0; i < loop->num_nodes; i++)
    for (e = body[i]->succ; e; e = e->succ_next)
      if (!flow_bb_inside_loop_p (loop, e->dest))
	{
	  act = find_common_loop (loop, e->dest->loop_father);
	  if (flow_loop_nested_p (father, act))
	    father = act;
	}
  free (body);

  if (father != loop->outer)
    {
      for (act = loop->outer; act != father; act = act->outer)
	act->num_nodes -= loop->num_nodes;
      flow_loop_tree_node_remove (loop);
      flow_loop_tree_node_add (father, loop);
      return 1;
    }
  return 0;
}

/* Fix placement of superloops of LOOP.  */
static void
fix_loop_placements (loop)
     struct loop *loop;
{
  struct loop *outer;

  while (loop->outer)
    {
      outer = loop->outer;
      if (!fix_loop_placement (loop))
        break;
      loop = outer;
    }
}

/* Scales histograms of LOOP and subloops by PROB.  */
static void
scale_loop_histograms (loop, prob)
     struct loop *loop;
     int prob;
{
  struct loop *aloop;

  for (aloop = loop->inner; aloop; aloop = aloop->next)
    scale_loop_histograms (aloop, prob);
  if (loop->histogram)
    add_histogram (loop->histogram, loop->histogram, prob - REG_BR_PROB_BASE);
}

/* Creates place for a new LOOP.  */
static void
place_new_loop (loops, loop)
     struct loops *loops;
     struct loop *loop;
{
  loops->parray =
    xrealloc (loops->parray, (loops->num + 1) * sizeof (struct loop *));
  loops->parray[loops->num] = loop;

  loop->num = loops->num++;
}

/* Copies structure of LOOP into TARGET, scaling histogram by PROB.  */
static struct loop *
duplicate_loop (loops, loop, target, prob)
     struct loops *loops;
     struct loop *loop;
     struct loop *target;
     int prob;
{
  struct loop *cloop;
  cloop = xcalloc (1, sizeof (struct loop));
  place_new_loop (loops, cloop);

  /* Initialize copied loop.  */
  cloop->level = loop->level;

  /* Set it as copy of loop.  */
  loop->copy = cloop;

  /* Scale histogram.  */
  if (loop->histogram)
    cloop->histogram = copy_histogram (loop->histogram, prob);

  /* Add it to target.  */
  flow_loop_tree_node_add (target, cloop);

  return cloop;
}

/* Copies structure of subloops of LOOP into TARGET; histograms are scaled by PROB.  */
static void 
duplicate_subloops (loops, loop, target, prob)
     struct loops *loops;
     struct loop *loop;
     struct loop *target;
     int prob;
{
  struct loop *aloop, *cloop;

  for (aloop = loop->inner; aloop; aloop = aloop->next)
    {
      cloop = duplicate_loop (loops, aloop, target, prob);
      duplicate_subloops (loops, aloop, cloop, prob);
    }
}

/*  Copies structure of COPIED_LOOPS into TARGET.  */
static void 
copy_loops_to (loops, copied_loops, n, target, prob)
     struct loops *loops;
     struct loop **copied_loops;
     int n;
     struct loop *target;
     int prob;
{
  struct loop *aloop;
  int i;

  for (i = 0; i < n; i++)
    {
      aloop = duplicate_loop (loops, copied_loops[i], target, prob);
      duplicate_subloops (loops, copied_loops[i], aloop, prob);
    }
}

/* Redirects edge E to DEST.  */
static void
loop_redirect_edge (e, dest)
     edge e;
     basic_block dest;
{
  if (e->dest == dest)
    return;

  cfg_layout_redirect_edge (e, dest);
}

/* Deletes edge if possible.  */
static bool
loop_delete_branch_edge (e)
     edge e;
{
  basic_block src = e->src;

  if (src->succ->succ_next)
    {
      basic_block newdest;
      /* Cannot handle more than two exit edges.  */
      if (src->succ->succ_next->succ_next)
	return false;
      /* Neither this.  */
      if (!any_condjump_p (src->end))
	return false;

      newdest = (e == src->succ
		 ? src->succ->succ_next->dest : src->succ->dest);
      if (newdest == EXIT_BLOCK_PTR)
	return false;

      return cfg_layout_redirect_edge (e, newdest);
    }
  else
    {
      /* Cannot happen -- we are duplicating loop! */
      abort ();
    }

  return false;  /* To avoid warning, cannot get here.  */
}

/* Duplicates BBS. Newly created bbs are placed into NEW_BBS, edges to
   header (target of ENTRY) and copy of header are returned, edge ENTRY
   is redirected to header copy.  Assigns bbs into loops, updates
   dominators.  If ADD_IRREDUCIBLE_FLAG, basic blocks that are not
   member of any inner loop are marked irreducible.  */
static void
copy_bbs (bbs, n, entry, latch_edge, new_bbs, loops, header_edge, copy_header_edge, add_irreducible_flag)
     basic_block *bbs;
     int n;
     edge entry;
     edge latch_edge;
     basic_block **new_bbs;
     struct loops *loops;
     edge *header_edge;
     edge *copy_header_edge;
     int add_irreducible_flag;
{
  int i;
  basic_block bb, new_bb, header = entry->dest, dom_bb;
  edge e;

  /* Duplicate bbs, update dominators, assign bbs to loops.  */
  (*new_bbs) = xcalloc (n, sizeof (basic_block));
  for (i = 0; i < n; i++)
    {
      /* Duplicate.  */
      bb = bbs[i];
      new_bb = (*new_bbs)[i] = cfg_layout_duplicate_bb (bb, NULL);
      RBI (new_bb)->duplicated = 1;
      /* Add to loop.  */
      add_bb_to_loop (new_bb, bb->loop_father->copy);
      add_to_dominance_info (loops->cfg.dom, new_bb);
      /* Possibly set header.  */
      if (bb->loop_father->header == bb && bb != header)
	new_bb->loop_father->header = new_bb;
      /* Or latch.  */
      if (bb->loop_father->latch == bb &&
	  bb->loop_father != header->loop_father)
	new_bb->loop_father->latch = new_bb;
      /* Take care of irreducible loops.  */
      if (add_irreducible_flag
	  && bb->loop_father == header->loop_father)
	new_bb->flags |= BB_IRREDUCIBLE_LOOP;
    }

  /* Set dominators.  */
  for (i = 0; i < n; i++)
    {
      bb = bbs[i];
      new_bb = (*new_bbs)[i];
      if (bb != header)
	{
	  /* For anything else than loop header, just copy it.  */
	  dom_bb = get_immediate_dominator (loops->cfg.dom, bb);
	  dom_bb = RBI (dom_bb)->copy;
	}
      else
	{
	  /* Copy of header is dominated by entry source.  */
	  dom_bb = entry->src;
	}
      if (!dom_bb)
	abort ();
      set_immediate_dominator (loops->cfg.dom, new_bb, dom_bb);
    }

  /* Redirect edges.  */
  for (i = 0; i < n; i++)
    {
      edge e_pred;
      new_bb = (*new_bbs)[i];
      bb = bbs[i];
      for (e = bb->pred; e; e = e_pred)
	{
	  basic_block src = e->src;

	  e_pred = e->pred_next;
	  
	  /* Does this edge interest us? */
	  if (!RBI (src)->duplicated)
	    continue;

	  /* So it interests us; redirect it.  */
	  if (bb != header)
	    loop_redirect_edge (e, new_bb);
	}
    }

  /* Redirect header edge.  */
  bb = RBI (latch_edge->src)->copy;
  for (e = bb->succ; e->dest != latch_edge->dest; e = e->succ_next);
  *header_edge = e;
  loop_redirect_edge (*header_edge, header);

  /* Redirect entry to copy of header.  */
  loop_redirect_edge (entry, RBI (header)->copy);
  *copy_header_edge = entry;

  /* Cancel duplicated flags.  */
  for (i = 0; i < n; i++)
   RBI ((*new_bbs)[i])->duplicated = 0;
}

/* Check whether LOOP's body can be duplicated.  */
bool
can_duplicate_loop_p (loop)
     struct loop *loop;
{
  basic_block *bbs;
  unsigned i;

  bbs = get_loop_body (loop);

  for (i = 0; i < loop->num_nodes; i++)
    {
      edge e;

      /* In case loop contains abnormal edge we can not redirect,
         we can't perform duplication.  */

      for (e = bbs[i]->succ; e; e = e->succ_next)
	if ((e->flags & EDGE_ABNORMAL)
	    && flow_bb_inside_loop_p (loop, e->dest))
	  {
	    free (bbs);
	    return false;
	  }

      if (!cfg_layout_can_duplicate_bb_p (bbs[i]))
	{
	  free (bbs);
	  return false;
	}
    }
  free (bbs);

  return true;
}

/* Store edges created by copying ORIG edge (simply this ORIG edge if IS_ORIG)
   (if ORIG is NULL, then all exit edges) into TO_REMOVE array.  */
static void
record_exit_edges (orig, bbs, nbbs, to_remove, n_to_remove, is_orig)
     edge orig;
     basic_block *bbs;
     int nbbs;
     edge *to_remove;
     unsigned *n_to_remove;
     int is_orig;
{
  sbitmap my_blocks;
  int i;
  edge e;

  if (orig)
    {
      if (is_orig)
	{
	  to_remove[(*n_to_remove)++] = orig;
	  return;
	}

      for (e = RBI (orig->src)->copy->succ; e; e = e->succ_next)
	if (e->dest == orig->dest)
	  break;
      if (!e)
	abort ();

      to_remove[(*n_to_remove)++] = e;
    }
  else
    {
      my_blocks = sbitmap_alloc (last_basic_block);
      sbitmap_zero (my_blocks);
      for (i = 0; i < nbbs; i++)
        SET_BIT (my_blocks, bbs[i]->index);

      for (i = 0; i < nbbs; i++)
	for (e = bbs[i]->succ; e; e = e->succ_next)
	  if (e->dest == EXIT_BLOCK_PTR ||
	      !TEST_BIT (my_blocks, e->dest->index))
	    to_remove[(*n_to_remove)++] = e;

      free (my_blocks);
    }
}


#define RDIV(X,Y) (((X) + (Y) / 2) / (Y))

/* Duplicates body of LOOP to given edge E NDUPL times.  Takes care of
   updating LOOPS structure.  E's destination must be LOOP header for this to
   work.  Store edges created by copying ORIG edge (if NULL, then all exit
   edges) from copies corresponding to set bits in WONT_EXIT (bit 0 corresponds
   to original LOOP body) into TO_REMOVE array.  Returns false if duplication
   is impossible.  */
int
duplicate_loop_to_header_edge (loop, e, loops, ndupl, wont_exit, orig,
			       to_remove, n_to_remove, flags)
     struct loop *loop;
     edge e;
     struct loops *loops;
     unsigned ndupl;
     sbitmap wont_exit;
     edge orig;
     edge *to_remove;
     unsigned *n_to_remove;
     int flags;
{
  struct loop *target, *aloop;
  struct loop **orig_loops;
  unsigned n_orig_loops;
  basic_block header = loop->header, latch = loop->latch;
  basic_block *new_bbs, *bbs, *first_active;
  basic_block new_bb, bb, first_active_latch = NULL;
  edge ae, latch_edge, he;
  unsigned i, j, n;
  int is_latch = (latch == e->src);
  int scale_act = 0, *scale_step = NULL, scale_main, p, freq_in, freq_le, freq_out_orig;
  unsigned hsteps;
  int prob_pass_thru, prob_pass_wont_exit, prob_pass_main;
  int add_irreducible_flag;
  gcov_type all_counters, iterations;

  if (e->dest != loop->header)
    abort ();
  if (ndupl <= 0)
    abort ();

  if (orig)
    {
      /* Orig must be edge out of the loop.  */
      if (!flow_bb_inside_loop_p (loop, orig->src))
	abort ();
      if (flow_bb_inside_loop_p (loop, orig->dest))
	abort ();
    }

  bbs = get_loop_body (loop);

  /* Check whether duplication is possible.  */

  for (i = 0; i < loop->num_nodes; i++)
    {
      if (!cfg_layout_can_duplicate_bb_p (bbs[i]))
	{
	  free (bbs);
	  return false;
	}
    }

  add_irreducible_flag = !is_latch && (e->src->flags & BB_IRREDUCIBLE_LOOP);

  /* Find edge from latch.  */
  latch_edge = loop_latch_edge (loop);

  if (flags & DLTHE_FLAG_UPDATE_FREQ)
    {
      /* For updating frequencies.  */
      freq_in = header->frequency;
      freq_le = EDGE_FREQUENCY (latch_edge);
      if (freq_in == 0)
	freq_in = 1;
      if (freq_in < freq_le)
	freq_in = freq_le;
      freq_out_orig = orig ? EDGE_FREQUENCY (orig) : freq_in - freq_le;
      if (freq_out_orig > freq_in - freq_le)
	freq_out_orig = freq_in - freq_le;
      prob_pass_thru = RDIV (REG_BR_PROB_BASE * freq_le, freq_in);
      prob_pass_wont_exit = RDIV (REG_BR_PROB_BASE * (freq_le + freq_out_orig), freq_in);

      scale_step = xmalloc (ndupl * sizeof (int));

      switch (DLTHE_PROB_UPDATING (flags))
	{
	  case DLTHE_USE_HISTOGRAM_PROB:
	    if (is_latch || !loop->histogram)
	      abort ();
	    iterations = latch_edge->count;
	    all_counters = loop->histogram->more;
	    for (i = 0; i < loop->histogram->steps; i++)
	      all_counters += loop->histogram->counts[i];
	    hsteps = ndupl;
	    if (hsteps > loop->histogram->steps)
	      hsteps = loop->histogram->steps;
	    for (i = 0; i < hsteps; i++)
	      {
		if (all_counters)
		  scale_step[i] = (all_counters - loop->histogram->counts[i]) * REG_BR_PROB_BASE / all_counters;
		else
		  scale_step[i] = 0;
		all_counters -= loop->histogram->counts[i];
		iterations -= i * loop->histogram->counts[i];
	      }
	    if (iterations < 0)
	      iterations = 0;
	    if (iterations)
	      p = iterations * REG_BR_PROB_BASE / (iterations + all_counters);
	    else
	      p = 0;
	    for (; i < ndupl; i++)
	      scale_step[i] = p;

	    /* Update histogram.  */
	    if (ndupl >= loop->histogram->steps)
	      {
		free (loop->histogram->counts);
		free (loop->histogram);
		loop->histogram = NULL;
	      }
	    else
	      {
		for (i = ndupl; i < loop->histogram->steps; i++)
		  loop->histogram->counts[i - ndupl] = loop->histogram->counts[i];
		loop->histogram->steps -= ndupl;
	      }
	    break;

	  case DLTHE_USE_WONT_EXIT:
	    for (i = 1; i <= ndupl; i++)
	      scale_step[i - 1] = TEST_BIT (wont_exit, i) ? prob_pass_wont_exit : prob_pass_thru;
	    break;

	  default:
	    abort ();
	}

      if (is_latch)
	{
	  prob_pass_main = TEST_BIT (wont_exit, 0) ? prob_pass_wont_exit : prob_pass_thru;
	  p = prob_pass_main;
	  scale_main = REG_BR_PROB_BASE;
	  for (i = 0; i < ndupl; i++)
	    {
	      scale_main += p;
	      p = RDIV (p * scale_step[i], REG_BR_PROB_BASE);
	    }
	  scale_main = RDIV (REG_BR_PROB_BASE * REG_BR_PROB_BASE, scale_main);
	  scale_act = RDIV (scale_main * prob_pass_main, REG_BR_PROB_BASE);
	}
      else
	{
	  scale_main = REG_BR_PROB_BASE;
	  for (i = 0; i < ndupl; i++)
	    scale_main = RDIV (scale_main * scale_step[i], REG_BR_PROB_BASE);
	  scale_act = REG_BR_PROB_BASE - prob_pass_thru;
	}
      for (i = 0; i < ndupl; i++)
	if (scale_step[i] < 0 || scale_step[i] > REG_BR_PROB_BASE)
	  abort ();
      if (scale_main < 0 || scale_main > REG_BR_PROB_BASE
	  || scale_act < 0  || scale_act > REG_BR_PROB_BASE)
	abort ();
    }

  /* Loop the new bbs will belong to.  */
  target = find_common_loop (e->src->loop_father, e->dest->loop_father);

  /* Original loops.  */
  n_orig_loops = 0;
  for (aloop = loop->inner; aloop; aloop = aloop->next)
    n_orig_loops++;
  orig_loops = xcalloc (n_orig_loops, sizeof (struct loop *));
  for (aloop = loop->inner, i = 0; aloop; aloop = aloop->next, i++)
    orig_loops[i] = aloop;

  loop->copy = target;
  
  /* Original basic blocks.  */
  n = loop->num_nodes;

  first_active = xcalloc(n, sizeof (basic_block));
  if (is_latch)
    {
      memcpy (first_active, bbs, n * sizeof (basic_block));
      first_active_latch = latch;
    }

  /* Record exit edges in original loop body.  */
  if (TEST_BIT (wont_exit, 0))
    record_exit_edges (orig, bbs, n, to_remove, n_to_remove, true);
  
  for (j = 0; j < ndupl; j++)
    {
      /* Copy loops.  */
      copy_loops_to (loops, orig_loops, n_orig_loops, target, scale_act);

      /* Copy bbs.  */
      copy_bbs (bbs, n, e, latch_edge, &new_bbs, loops, &e, &he, add_irreducible_flag);
      if (is_latch)
	loop->latch = RBI (latch)->copy;

      /* Record exit edges in this copy.  */
      if (TEST_BIT (wont_exit, j + 1))
	record_exit_edges (orig, new_bbs, n, to_remove, n_to_remove, false);
  
      /* Set counts and frequencies.  */
      for (i = 0; i < n; i++)
	{
	  new_bb = new_bbs[i];
	  bb = bbs[i];

	  if (flags & DLTHE_FLAG_UPDATE_FREQ)
	    {
	      new_bb->count = RDIV (scale_act * bb->count, REG_BR_PROB_BASE);
	      new_bb->frequency = RDIV (scale_act * bb->frequency,
     					REG_BR_PROB_BASE);
	    }
	  else
	    {
	      new_bb->count = bb->count;
	      new_bb->frequency = bb->frequency;
	    }

	  for (ae = new_bb->succ; ae; ae = ae->succ_next)
    	    ae->count = RDIV (new_bb->count * ae->probability,
			      REG_BR_PROB_BASE);
	}
      if (flags & DLTHE_FLAG_UPDATE_FREQ)
	scale_act = RDIV (scale_act * scale_step[j], REG_BR_PROB_BASE);

      if (!first_active_latch)
	{
	  memcpy (first_active, new_bbs, n * sizeof (basic_block));
	  first_active_latch = RBI (latch)->copy;
	}
      
      free (new_bbs);

      /* Original loop header is dominated by latch copy
	 if we duplicated on its only entry edge.  */
      if (!is_latch && !header->pred->pred_next->pred_next)
	set_immediate_dominator (loops->cfg.dom, header, RBI (latch)->copy);
      if (is_latch && j == 0)
	{
	  /* Update edge from latch.  */
	  for (latch_edge = RBI (header)->copy->pred;
	       latch_edge->src != latch;
	       latch_edge = latch_edge->pred_next);
	}
    }
  /* Now handle original loop.  */
  
  /* Update edge counts.  */
  if (flags & DLTHE_FLAG_UPDATE_FREQ)
    {
      for (i = 0; i < n; i++)
	{
	  bb = bbs[i];
	  bb->count = RDIV (scale_main * bb->count, REG_BR_PROB_BASE);
	  bb->frequency = RDIV (scale_main * bb->frequency, REG_BR_PROB_BASE);
	  for (ae = bb->succ; ae; ae = ae->succ_next)
	    ae->count = RDIV (bb->count * ae->probability, REG_BR_PROB_BASE);
	}
      for (i = 0; i < n_orig_loops; i++)
	scale_loop_histograms (orig_loops[i], scale_main);
      free (scale_step);
    }
  free (orig_loops);

  /* Update dominators of other blocks if affected.  */
  for (i = 0; i < n; i++)
    {
      basic_block dominated, dom_bb, *dom_bbs;
      int n_dom_bbs,j;

      bb = bbs[i];
      n_dom_bbs = get_dominated_by (loops->cfg.dom, bb, &dom_bbs);
      for (j = 0; j < n_dom_bbs; j++)
	{
	  dominated = dom_bbs[j];
	  if (flow_bb_inside_loop_p (loop, dominated))
	    continue;
	  dom_bb = nearest_common_dominator (
			loops->cfg.dom, first_active[i], first_active_latch);
          set_immediate_dominator (loops->cfg.dom, dominated, dom_bb);
	}
      free (dom_bbs);
    }
  free (first_active);

  free (bbs);

  return true;
}

/* Creates a pre-header for a LOOP.  Returns newly created block.  Unless
   CP_SIMPLE_PREHEADERS is set in FLAGS, we only force LOOP to have single
   entry; otherwise we also force preheader block to have only one successor.
   */
static basic_block
create_preheader (loop, dom, flags)
     struct loop *loop;
     dominance_info dom;
     int flags;
{
  edge e, fallthru;
  basic_block dummy;
  basic_block jump, src = 0;
  struct loop *cloop, *ploop;
  int nentry = 0;
  rtx insn;

  cloop = loop->outer;

  for (e = loop->header->pred; e; e = e->pred_next)
    {
      if (e->src == loop->latch)
	continue;
      nentry++;
    }
  if (!nentry)
    abort ();
  if (nentry == 1)
    {
      for (e = loop->header->pred; e->src == loop->latch; e = e->pred_next);
      if (!(flags & CP_SIMPLE_PREHEADERS)
	  || !e->src->succ->succ_next)
	return NULL;
    }

  insn = first_insn_after_basic_block_note (loop->header);
  if (insn)
    insn = PREV_INSN (insn);
  else
    insn = get_last_insn ();
  if (insn == loop->header->end)
    {
      /* Split_block would not split block after its end.  */
      emit_note_after (NOTE_INSN_DELETED, insn);
    }
  if (flags & CP_INSIDE_CFGLAYOUT)
    fallthru = cfg_layout_split_block (loop->header, insn);
  else
    fallthru = split_block (loop->header, insn);
  dummy = fallthru->src;
  loop->header = fallthru->dest;

  /* The header could be a latch of some superloop(s); due to design of
     split_block, it would now move to fallthru->dest.  */
  for (ploop = loop; ploop; ploop = ploop->outer)
    if (ploop->latch == dummy)
      ploop->latch = fallthru->dest;

  add_to_dominance_info (dom, fallthru->dest);
  
  /* Redirect edges. */
  for (e = dummy->pred; e; e = e->pred_next)
    {
      src = e->src;
      if (src == loop->latch)
	break;
    }
  if (!e)
    abort ();

  dummy->frequency -= EDGE_FREQUENCY (e);
  dummy->count -= e->count;
  fallthru->count -= e->count;
  if (flags & CP_INSIDE_CFGLAYOUT)
    cfg_layout_redirect_edge (e, loop->header);
  else
    {
      jump = redirect_edge_and_branch_force (e, loop->header);
      if (jump)
	{
	  add_to_dominance_info (dom, jump);
	  set_immediate_dominator (dom, jump, src);
	  add_bb_to_loop (jump, loop);
	  loop->latch = jump;
	}
    }

  /* Update structures.  */
  redirect_immediate_dominators (dom, dummy, loop->header);
  set_immediate_dominator (dom, loop->header, dummy);
  loop->header->loop_father = loop;
  add_bb_to_loop (dummy, cloop);
  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Created preheader block for loop %i\n",
	     loop->num);

  return dummy;
}

/* Create preheaders for each loop; for meaning of flags see
   create_preheader.  */
void
create_preheaders (loops, flags)
     struct loops *loops;
     int flags;
{
  unsigned i;
  for (i = 1; i < loops->num; i++)
    create_preheader (loops->parray[i], loops->cfg.dom, flags);
  loops->state |= LOOPS_HAVE_PREHEADERS;
}

/* Forces all loop latches to have only single successor.  */
void
force_single_succ_latches (loops)
     struct loops *loops;
{
  unsigned i;
  struct loop *loop;
  edge e;

  for (i = 1; i < loops->num; i++)
    {
      loop = loops->parray[i];
      if (!loop->latch->succ->succ_next)
	continue;
 
      for (e = loop->header->pred; e->src != loop->latch; e = e->pred_next)
	continue;

      loop_split_edge_with (e, NULL_RTX, loops);
    }
  loops->state |= LOOPS_HAVE_SIMPLE_LATCHES;
}

/* A quite stupid function to put INSNS on E. They are supposed to form
   just one basic block. Jumps out are not handled, so cfg do not have to
   be ok after this function.  */
basic_block
loop_split_edge_with (e, insns, loops)
     edge e;
     rtx insns;
     struct loops *loops;
{
  basic_block src, dest, new_bb;
  struct loop *loop_c;
  edge new_e;
  
  src = e->src;
  dest = e->dest;

  loop_c = find_common_loop (src->loop_father, dest->loop_father);

  /* Create basic block for it.  */

  new_bb = create_basic_block (NULL_RTX, NULL_RTX, EXIT_BLOCK_PTR->prev_bb);
  add_to_dominance_info (loops->cfg.dom, new_bb);
  add_bb_to_loop (new_bb, loop_c);
  new_bb->flags = insns ? BB_SUPERBLOCK : 0;
  if (src->flags & BB_IRREDUCIBLE_LOOP)
    {
      /* We expect simple preheaders here.  */
      if ((dest->flags & BB_IRREDUCIBLE_LOOP)
          || dest->loop_father->header == dest)
        new_bb->flags |= BB_IRREDUCIBLE_LOOP;
    }

  new_e = make_edge (new_bb, dest, EDGE_FALLTHRU);
  new_e->probability = REG_BR_PROB_BASE;
  new_e->count = e->count;

  new_e->loop_histogram = e->loop_histogram;
  e->loop_histogram = NULL;

  new_bb->count = e->count;

  new_bb->frequency = EDGE_FREQUENCY (e);
  cfg_layout_redirect_edge (e, new_bb);

  alloc_aux_for_block (new_bb, sizeof (struct reorder_block_def));
  if (insns)
    {
      start_sequence ();
      emit_insn (insns);
      insns = get_insns ();
      end_sequence ();
      emit_insn_after (insns, new_bb->end);
    }

  set_immediate_dominator (loops->cfg.dom, new_bb, src);
  set_immediate_dominator (loops->cfg.dom, dest,
    recount_dominator (loops->cfg.dom, dest));

  if (dest->loop_father->latch == src)
    dest->loop_father->latch = new_bb;
  
  return new_bb;
}

