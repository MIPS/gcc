/* Perform various loop optimizations.
   Copyright (C) 1987, 1988, 1989, 1991, 1992, 1993, 1994, 1995, 1996, 1997,
   1998, 1999, 2000, 2001, 2002 Free Software Foundation, Inc.

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
#include "rtl.h"
#include "tm_p.h"
#include "obstack.h"
#include "function.h"
#include "expr.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "insn-config.h"
#include "regs.h"
#include "recog.h"
#include "flags.h"
#include "real.h"
#include "cselib.h"
#include "except.h"
#include "toplev.h"
#include "predict.h"
#include "insn-flags.h"
#include "cfglayout.h"
#include "loop.h"
#include "params.h"
#include "output.h"
/* Stupid definitions of dominator manipulation.  */

basic_block
get_immediate_dominator (bmp, bb)
     sbitmap *bmp __attribute__ ((__unused__));
     basic_block bb;
{
  return bb->dominator;
}

void
set_immediate_dominator (bmp, bb, dominated_by)
     sbitmap *bmp __attribute__ ((__unused__));
     basic_block bb;
     basic_block dominated_by;
{
  bb->dominator = dominated_by;
}

int
get_dominated_by (dom, bb, bbs)
     sbitmap *dom __attribute__ ((__unused__));
     basic_block bb;
     basic_block **bbs;
{
  int i, n;
  n = 0;
  for (i = 0; i < n_basic_blocks; i++)
    if (BASIC_BLOCK (i)->dominator == bb)
      n++;
  *bbs = xcalloc (n, sizeof (basic_block));
  n = 0;
  for (i = 0; i < n_basic_blocks; i++)
    if (BASIC_BLOCK (i)->dominator == bb)
      (*bbs)[n++] = BASIC_BLOCK (i);
  return n;
}

void
redirect_immediate_dominators (dom, bb, to)
     sbitmap *dom __attribute__ ((__unused__));
     basic_block bb;
     basic_block to;
{
  int i;

  for (i = 0; i < n_basic_blocks; i++)
    if (BASIC_BLOCK (i)->dominator == bb)
      BASIC_BLOCK (i)->dominator = to;
}

basic_block
nearest_common_dominator (bmp, bb1, bb2)
     sbitmap *bmp __attribute__ ((__unused__));
     basic_block bb1;
     basic_block bb2;
{
  int l1, l2, l;
  basic_block ab;

  if (!bb1)
    return bb2;
  if (!bb2)
    return bb1;

  for (l1 = 0, ab = bb1; ab && ab != ab->dominator; ab = ab->dominator)
    l1++;
  if (ab)
    return bb2;
  for (l2 = 0, ab = bb2; ab && ab != ab->dominator; ab = ab->dominator)
    l2++;
  if (ab)
    return bb1;

  if (l1 > l2)
    {
      ab = bb1; bb1 = bb2; bb2= ab;
      l = l1; l1 = l2; l2 = l;
    }
  while (l2 > l1)
    {
      bb2 = bb2->dominator;
      l2--;
    }
  while (bb1 != bb2)
    {
      bb1 = bb1->dominator;
      bb2 = bb2->dominator;
    }
  return bb1;
}

bool
dominated_by_p (dom, bb1, bb2)
     sbitmap *dom __attribute__ ((__unused__));
     basic_block bb1;
     basic_block bb2;
{
  while (bb1 && bb1 != bb2)
    bb1 = bb1->dominator;
  return bb1 != NULL;
}

void
verify_dominators (void)
{
  int i, err = 0;
  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb, dom_bb;
      bb = BASIC_BLOCK (i);
      dom_bb = recount_dominator (NULL, bb);
      if (dom_bb != bb->dominator)
	{
	  error ("dominator of %d should be %d, not %d",
	   bb->index, dom_bb->index, bb->dominator->index);
	  err = 1;
	}
    }
  if (err)
    abort ();
}

/* Recount dominator of BB.  */
basic_block
recount_dominator (dom, bb)
     sbitmap *dom;
     basic_block bb;
{
   basic_block dom_bb = NULL, old_dom_bb;
   edge e;

   old_dom_bb = get_immediate_dominator (dom, bb);
   set_immediate_dominator (dom, bb, bb);
   for (e = bb->pred; e; e = e->pred_next)
     dom_bb = nearest_common_dominator (dom, dom_bb, e->src);
   set_immediate_dominator (dom, bb, old_dom_bb);

   return dom_bb;
}

/* Iteratively recount dominators of BBS. If LOCAL is set, the change is
   supposed to be local and not to grow further.  Otherwise BBS is supposed
   to be able to hold all affected blocks (i.e. n_basic_blocks worst case).  */
void
iterate_fix_dominators (doms, bbs, n, local)
     sbitmap *doms;
     basic_block *bbs;
     int n;
     int local;
{
  sbitmap affected;
  int i, changed = 1;
  basic_block old_dom, new_dom;
  edge e;

  if (!local)
    affected = sbitmap_alloc (n_basic_blocks);
  while (changed)
    {
      changed = 0;
      if (!local)
	sbitmap_zero (affected);
      for (i = 0; i < n; i++)
	{
	  old_dom = get_immediate_dominator (doms, bbs[i]);
	  new_dom = recount_dominator (doms, bbs[i]);
	  if (old_dom != new_dom)
	    {
	      changed = 1;
	      set_immediate_dominator (doms, bbs[i], new_dom);
	      if (!local)
		for (e = bbs[i]->pred; e; e = e->pred_next)
		  SET_BIT (affected, e->src->index);
	    }
	}
      if (changed && !local)
	{
	  n = 0;
	  EXECUTE_IF_SET_IN_SBITMAP (affected, 0, i,
	    {
	      bbs[n++] = BASIC_BLOCK (i);
	    });
	}
    }
  if (!local)
    sbitmap_free (affected);
}

static struct loop * duplicate_loop PARAMS ((struct loops *, struct loop *, struct loop *));
static void duplicate_subloops PARAMS ((struct loops *, struct loop *, struct loop *));
static void copy_loops_to PARAMS ((struct loops *, struct loop **, int, struct loop *));
static void loop_redirect_edge PARAMS ((edge, basic_block));
static void loop_delete_branch_edge PARAMS ((edge));
static void copy_bbs PARAMS ((basic_block *, int, edge, edge, basic_block **, struct loops *, edge *, edge *));
static void remove_exit_edges PARAMS ((basic_block *, int, basic_block));
static struct loop *unswitch_loop PARAMS ((struct loops *, struct loop *, basic_block));
static void remove_bbs PARAMS ((basic_block *, int));
static bool rpe_enum_p PARAMS ((basic_block, void *));
static int find_branch PARAMS ((edge, sbitmap *, struct loop *, basic_block **));
static void remove_path PARAMS ((edge, struct loop *, sbitmap *, int));
static int try_remove_path PARAMS ((struct loops *, struct loop *, edge));
static struct loop *loopify PARAMS ((struct loops *, edge, edge, basic_block));
static bool alp_enum_p PARAMS ((basic_block, void *));
static void add_loop PARAMS ((struct loops *, struct loop *));
static void fix_loop_placement PARAMS ((struct loop *));
static void place_new_loop PARAMS ((struct loops *, struct loop *));
static void unswitch_single_loop PARAMS ((struct loops *, struct loop *, rtx, int));
static bool may_unswitch_on_p PARAMS ((struct loops *, basic_block, struct loop *, basic_block *));
static edge split_loop_bb PARAMS ((struct loops *, basic_block, rtx));
static rtx reversed_condition PARAMS ((rtx));
static void scale_loop_frequencies PARAMS ((struct loop *, int, int));
static void scale_bbs_frequencies PARAMS ((basic_block *, int, int, int));

/* Initialize loop optimizer.  */

struct loops *
loop_optimizer_init (dumpfile)
     FILE *dumpfile;
{
  struct loops *loops = xcalloc (1, sizeof (struct loops));

  /* Find the loops.  */

  if (flow_loops_find (loops, LOOP_TREE) <= 1)
    {
      /* No loops.  */
      flow_loops_free (loops);
      free (loops);
      return NULL;
    }

  /* Initialize structures for layout changes.  */
  cfg_layout_initialize (loops);

  /* Create pre-headers.  */
  create_preheaders (loops, CP_FOR_LOOP_NEW);

  /* Force all latches to have only single successor.  */
  force_single_succ_latches (loops);

  /* Dump loops.  */
  flow_loops_dump (loops, dumpfile, NULL, 1);

#ifdef ENABLE_CHECKING
  verify_dominators ();
  verify_loop_structure (loops, VLS_FOR_LOOP_NEW);
#endif

  return loops;
}

/* Finalize loop optimizer.  */
void
loop_optimizer_finalize (loops, dumpfile)
     struct loops *loops;
     FILE *dumpfile;
{
  int i;

  /* Finalize layout changes.  */
  /* Make chain.  */
  for (i = 0; i < n_basic_blocks-1; i++)
    RBI (BASIC_BLOCK (i))->next = BASIC_BLOCK (i+1);

  /* Another dump.  */
  free (loops->cfg.rc_order);
  loops->cfg.rc_order = NULL;
  free (loops->cfg.dfs_order);
  loops->cfg.dfs_order = NULL;
  flow_loops_dump (loops, dumpfile, NULL, 1);

  /* Clean up.  */
  flow_loops_free (loops);
  free (loops);
 
  /* Finalize changes.  */
  cfg_layout_finalize ();

  /* Remove dominators.  */
  for (i = 0; i < n_basic_blocks; i++)
    BASIC_BLOCK (i)->dominator = NULL;

  /* Checking.  */
#ifdef ENABLE_CHECKING
  verify_flow_info ();
#endif
}

/* Unswitch LOOPS.  */
void
unswitch_loops (loops)
     struct loops *loops;
{
  int i;
  /* Scan the loops, last ones first, since this means inner ones are done
     before outer ones.  */
  for (i = loops->num - 1; i > 0; i--)
    {
      struct loop *loop = loops->parray[i];
      unswitch_single_loop (loops, loop, NULL_RTX, 0);
#ifdef ENABLE_CHECKING
      verify_dominators ();
      verify_loop_structure (loops, VLS_FOR_LOOP_NEW);
#endif
    }
}

/* Splits basic block BB after INSN, returns created edge.  Updates loops
   and dominators.  */
static edge
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
  n_dom_bbs = get_dominated_by (loops->cfg.dom, e->src, &dom_bbs);
  for (i = 0; i < n_dom_bbs; i++)
    set_immediate_dominator (loops->cfg.dom, dom_bbs[i], e->dest);
  free (dom_bbs);
  set_immediate_dominator (loops->cfg.dom, e->dest, e->src);

  /* Take care of RBI.  */
  alloc_aux_for_block (e->dest, sizeof (struct reorder_block_def));

  return e;
}

/* Checks whether we can unswitch LOOP on basic block BB.  LOOP BODY
   is provided to save time.  */
static bool
may_unswitch_on_p (loops, bb, loop, body)
     struct loops *loops;
     basic_block bb;
     struct loop *loop;
     basic_block *body;
{
  rtx set, test;
  int i;

  /* It must be a simple conditional jump.  */
  if (!bb->succ || !bb->succ->succ_next || bb->succ->succ_next->succ_next)
    return false;
  if (!any_condjump_p (bb->end))
    return false;

  /* Are both branches inside loop?  */
  if (!flow_bb_inside_loop_p (loop, bb->succ->dest)
      || !flow_bb_inside_loop_p (loop, bb->succ->succ_next->dest))
    return false;

  /* Latch must be dominated by it (ugly technical restriction,
     we should remove this later).  */
  if (!dominated_by_p (loops->cfg.dom, loop->latch, bb))
    return false;

  /* Condition must be invariant.  */
  set = pc_set (bb->end);
  if (!set)
    abort ();
  test = XEXP (SET_SRC (set), 0);

  for (i = 0; i < loop->num_nodes; i++)
    if (modified_between_p (test, body[i]->head, NEXT_INSN (body[i]->end)))
      return false;

  return true;
}

/* Reverses CONDition; returns NULL if we cannot.  */
static rtx
reversed_condition (cond)
     rtx cond;
{
  enum rtx_code reversed;
  reversed = reversed_comparison_code (cond, NULL);
  if (reversed == UNKNOWN)
    return NULL_RTX;
  else
    return gen_rtx_fmt_ee (reversed,
	 		     GET_MODE (cond), XEXP (cond, 0),
			     XEXP (cond, 1));
}

/* Unswitch single LOOP.  COND_CHECKED holds list of conditions we already
   unswitched on and are true in our branch.  NUM is number of unswitchings
   done; do not allow it to grow too much, it is too easy to create example
   on that the code would grow exponentially.  */
static void
unswitch_single_loop (loops, loop, cond_checked, num)
     struct loops *loops;
     struct loop *loop;
     rtx cond_checked;
     int num;
{
  basic_block *bbs, bb;
  struct loop *nloop;
  int i, true_first;
  rtx cond, rcond, conds, rconds, acond, set;
  int always_true;
  int always_false;
  int repeat;
  edge e;

  /* Do not unswitch too much.  */
  if (num > PARAM_VALUE (PARAM_MAX_UNSWITCH_LEVEL))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching anymore, hit max level\n");
      return;
    }

  /* We only unswitch innermost loops (at least for now).  */
  if (loop->inner)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, not innermost loop\n");
      return;
    }
  
  /* And we must be able to duplicate loop body.  */
  if (!can_duplicate_loop_p (loop))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, can't duplicate loop\n");
      return;
    }

  /* Check the size of loop.  */
  if (num_loop_insns (loop) > PARAM_VALUE (PARAM_MAX_UNSWITCH_INSNS))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, loop too big\n");
      return;
    }
  
  /* Do not unswitch in cold areas.  */
  if (!maybe_hot_bb_p (loop->header))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, not hot area\n");
      return;
    }
  
  /* Nor if it usually do not pass.  */
  if (expected_loop_iterations (loop) < 1)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, loop iterations < 1\n");
      return;
    }

  do
    {
      repeat = 0;
    
      /* Find a bb to unswitch on.  We use just a stupid test of invariantness
	 of the condition: all used regs must not be modified inside loop body.  */
      bbs = get_loop_body (loop);
      for (i = 0; i < loop->num_nodes; i++)
	if (may_unswitch_on_p (loops, bbs[i], loop, bbs))
	  break;

      if (i == loop->num_nodes)
	{
	  free (bbs);
	  return;
	}

      if (!(cond = get_condition (bbs[i]->end, NULL)))
	abort ();
      rcond = reversed_condition (cond);
      
      /* Check whether the result can be predicted.  */
      always_true = 0;
      always_false = 0;
      for (acond = cond_checked; acond; acond = XEXP (acond, 1))
	{
	  if (rtx_equal_p (cond, XEXP (acond, 0)))
	    {
	      /* True.  */
	      always_true = 1;
	      break;
	    }
	  if (rtx_equal_p (rcond, XEXP (acond, 0)))
	    {
	      /* False.  */
	      always_false = 1;
	      break;
	    }
	}

      set = pc_set (bbs[i]->end);
      if (!set)
	abort ();
      if (always_true)
	{
	  /* Attempt to remove false path if possible.  */
 	  for (e = bbs[i]->succ; !(e->flags & EDGE_FALLTHRU); e = e->succ_next);
	  if (try_remove_path (loops, loop, e))
	    {
	      free (bbs);
	      repeat = 1;
	    }
	}
      else if (always_false)
	{
	  /* Attempt to remove true path if possible.  */
	  for (e = bbs[i]->succ; e->flags & EDGE_FALLTHRU; e = e->succ_next);
	  if (try_remove_path (loops, loop, e))
	    {
	      free (bbs);
	      repeat = 1;
	    }
	}
    } while (repeat);
 
  /* We found the condition we can unswitch on.  */
  conds = alloc_EXPR_LIST (0, cond, cond_checked);
  if (rcond)
    rconds = alloc_EXPR_LIST (0, rcond, cond_checked);
  else
    rconds = cond_checked;

  /* Separate condition.  */
  bb = split_loop_bb (loops, bbs[i], PREV_INSN (bbs[i]->end))->dest;
  free (bbs);
  true_first = !(bb->succ->flags & EDGE_FALLTHRU);
  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Unswitching loop\n");
  /* Unswitch the loop.  */
  nloop = unswitch_loop (loops, loop, bb);
  if (!nloop)
  abort ();

  /* Invoke itself on modified loops.  */
  unswitch_single_loop (loops, nloop, true_first ? conds : rconds, num + 1);
  unswitch_single_loop (loops, loop, true_first ? rconds : conds, num + 1);

  free_EXPR_LIST_node (conds);
  if (rcond)
    free_EXPR_LIST_node (rconds);
}

/* Returns expected number of LOOP iterations.  */
int
expected_loop_iterations (loop)
     struct loop *loop;
{
  int freq_in, freq_latch;
  edge e;

  freq_in = 0;

  for (e = loop->header->pred; e; e = e->pred_next)
    if (e->src == loop->latch)
      freq_latch = EDGE_FREQUENCY (e);
    else
      freq_in += EDGE_FREQUENCY (e);

  if (freq_in == 0)
    return 0;

  return freq_latch / freq_in;
}

/* Checks whether BB is inside RPE_LOOP and is dominated by RPE_DOM.  */
struct rpe_data
 {
   struct loop *loop;
   basic_block dom;
   sbitmap *doms;
 };

static bool
rpe_enum_p (bb, data)
     basic_block bb;
     void *data;
{
  struct rpe_data *rpe = data;
  return flow_bb_inside_loop_p (rpe->loop, bb)
	 && dominated_by_p (rpe->doms, bb, rpe->dom);
}

/* Remove BBS.  */
static void
remove_bbs (bbs, nbbs)
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
      flow_delete_block (bbs[i]);
    }
}

/* Find branch beginning at Edge inside LOOP and put it into BBS.  */
static int
find_branch (e, doms, loop, bbs)
     edge e;
     sbitmap *doms;
     struct loop *loop;
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
  rpe.loop = loop;
  rpe.dom = e->dest;
  rpe.doms = doms;
  *bbs = xcalloc (loop->num_nodes, sizeof (basic_block));
  return dfs_enumerate_from (e->dest, 0, rpe_enum_p, *bbs,
			     loop->num_nodes, &rpe);
}

/* Remove given edge E and all bbs inside LOOP that are dominated by target 
   of E.  Fix dominators by redirecting to copy if FIX is set.  */
static void
remove_path (e, loop, doms, fix)
     edge e;
     struct loop *loop;
     sbitmap *doms;
     int fix;
{
  int nrem, i, j;
  basic_block *rem_bbs, bb, nbb, *dom_bbs, dom_bb, ndom;
  int n_dom_bbs;
  
  nrem = find_branch (e, doms, loop, &rem_bbs);
  
  if (fix)
    {
      /* Fix dominators.  */
      for (i = 0; i < nrem; i++)
	{
	  bb = rem_bbs[i];
	  ndom = RBI (bb)->copy;
	  n_dom_bbs = get_dominated_by (doms, bb, &dom_bbs);
	  for (j = 0; j < n_dom_bbs; j++)
	    {
	      /* This may also be bbs inside loop; but it does not matter,
		 as they will be deleted anyway.  */
	      dom_bb = dom_bbs[j];
	      set_immediate_dominator (doms, dom_bb, ndom);
	    }
	  free (dom_bbs);
	}
      /* Fix frequencies.  */
      for (i = 0; i < nrem; i++)
	{
	  bb = rem_bbs[i];
	  nbb = RBI (bb)->copy;
	  nbb->frequency += bb->frequency;
	  nbb->count += bb->count;
	}
    }
  else
    {
      /* Fix frequencies.  */
      for (i = 0; i < nrem; i++)
	{
	  bb = rem_bbs[i];
	  nbb = RBI (bb)->original;
	  nbb->frequency += bb->frequency;
	  nbb->count += bb->count;
	}
     }

  /* Remove the jump and edge.  */
  loop_delete_branch_edge (e);

  /* Remove the blocks.  */
  remove_bbs (rem_bbs, nrem);
  free (rem_bbs);
}

/* Attempts to remove path beginning at E from LOOP. Returns 1 if successful.
   */
static int
try_remove_path (loops, loop, e)
     struct loops *loops;
     struct loop *loop;
     edge e;
{
  edge ae;
  basic_block *rem_bbs, *dom_bbs, from, *border_bbs;
  int i, j, nrem, n_dom_bbs, n_border_bbs;

  /* First identify the branch.  */
  nrem = find_branch (e, loops->cfg.dom, loop, &rem_bbs);

  /* Check whether we would not create unreachable blocks.  */
  for (i = 0; i < nrem; i++)
    {
      n_dom_bbs = get_dominated_by (loops->cfg.dom, rem_bbs[i], &dom_bbs);
      for (j = 0; j < n_dom_bbs; j++)
	if (!flow_bb_inside_loop_p (loop, dom_bbs[j]))
	  {
	    free (dom_bbs);
	    free (rem_bbs);
	    return 0;
	  }
      free (dom_bbs);
    }

  /* Remember border blocks.  */
  border_bbs = xcalloc (n_basic_blocks, sizeof (basic_block));
  n_border_bbs = 0;
  for (i = 0; i < nrem; i++)
    for (ae = rem_bbs[i]->succ; ae; ae = ae->succ_next)
      if (!flow_bb_inside_loop_p (loop, ae->dest))
	border_bbs[n_border_bbs++] = ae->dest;

  /* OK. Remove the path.  */
  from = e->src;
  loop_delete_branch_edge (e);
  remove_bbs (rem_bbs, nrem);
  free (rem_bbs);

  /* Recount dominators for from.  */
  n_dom_bbs = get_dominated_by (loops->cfg.dom, from, &dom_bbs);
  iterate_fix_dominators (loops->cfg.dom, dom_bbs, n_dom_bbs, 1);
  free (dom_bbs);

  /* Recount dominators for border bbs.  */
  iterate_fix_dominators (loops->cfg.dom, border_bbs, n_border_bbs, 0);
  free (border_bbs);

  /* Fix loop placement.  */
  fix_loop_placement (loop);

  return 1;
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
   latch to header.  Everything between them plus LATCH_EDGE destrination
   must be dominated by HEADER_EDGE destination.  Add SWITCH_BB to original
   entry edge.  Returns newly created loop.  Dominators outside the area
   are intentionally left wrong.  */
static struct loop *
loopify (loops, latch_edge, header_edge, switch_bb)
     struct loops *loops;
     edge latch_edge;
     edge header_edge;
     basic_block switch_bb;
{
  basic_block succ_bb = latch_edge->dest;
  basic_block pred_bb = header_edge->src;
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

  return loop;
}

/* Move LOOP up the hierarchy while it is not backward reachable from the
   latch of the outer loop.  */
static void
fix_loop_placement (loop)
     struct loop *loop;
{
  basic_block *body;
  int i;
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
    }
}

/* Unswitch a LOOP w.r. to given basic block.  We only support unswitching
   of innermost loops (this is not a principial restriction, I'm just lazy
   to handle subloops).  UNSWITCH_ON must be executed in every iteration,
   i.e. it must dominate LOOP latch.  Returns NULL if impossible, new
   loop otherwise.  */
static struct loop *
unswitch_loop (loops, loop, unswitch_on)
     struct loops *loops;
     struct loop *loop;
     basic_block unswitch_on;
{
  edge entry, e, latch_edge;
  basic_block switch_bb, unswitch_on_alt, *bbs, *dom_bbs, dom_bb;
  struct loop *nloop;
  int n_dom_bbs, i, j;
  int just_one_edge;

  /* Some sanity checking.  */
  if (!flow_bb_inside_loop_p (loop, unswitch_on))
    abort ();
  if (!unswitch_on->succ || !unswitch_on->succ->succ_next ||
      unswitch_on->succ->succ_next->succ_next)
    abort ();
  if (!dominated_by_p (loops->cfg.dom, loop->latch, unswitch_on))
    abort ();
  if (loop->inner)
    abort ();
  if (!flow_bb_inside_loop_p (loop, unswitch_on->succ->dest))
    abort ();
  if (!flow_bb_inside_loop_p (loop, unswitch_on->succ->succ_next->dest))
    abort ();
  
  /* Will we be able to perform redirection?  */
  if (!any_condjump_p (unswitch_on->end))
    return NULL;
  if (!cfg_layout_can_duplicate_bb_p (unswitch_on))
    return NULL;
  
  for (entry = loop->header->pred;
       entry->src == loop->latch;
       entry = entry->pred_next);
  
  /* Make a copy.  */
  if (!duplicate_loop_to_header_edge (loop, entry, loops, 1, 0, 0))
    return NULL;

  /* Record switch block.  */
  unswitch_on_alt = RBI (unswitch_on)->copy;

  /* Make a copy of unswitched block.  */
  switch_bb = cfg_layout_duplicate_bb (unswitch_on, NULL);
  RBI (unswitch_on)->copy = unswitch_on_alt;

  /* Loopify the copy.  */
  for (latch_edge = RBI (loop->latch)->copy->succ;
       latch_edge->dest != loop->header;
       latch_edge = latch_edge->succ_next);
  nloop = loopify (loops, latch_edge,
		   RBI (loop->header)->copy->pred, switch_bb);
  
  /* Remove paths from loop copies and update dominators.
     We rely on the fact that cfg_layout_duplicate_bb reverses
     list of edges here.  */
  for (e = unswitch_on->succ->succ_next->dest->pred; e; e = e->pred_next)
    if (e->src != unswitch_on &&
	!dominated_by_p (loops->cfg.dom, e->src, e->dest))
      break;
  just_one_edge = (e != NULL);
  remove_path (unswitch_on->succ, loop, loops->cfg.dom, 1);
  remove_path (unswitch_on_alt->succ, nloop, loops->cfg.dom, 0);

  /* One of created loops do not have to be subloop of the outer loop now.  */
  fix_loop_placement (loop);
  fix_loop_placement (nloop);

  /* Now fix dominators of outside blocks.  */
  bbs = get_loop_body (loop);
  for (i = 0; i < loop->num_nodes; i++)
    {
      if (!just_one_edge &&
	  dominated_by_p (loops->cfg.dom, bbs[i], unswitch_on->succ->dest))
	continue;
      n_dom_bbs = get_dominated_by (loops->cfg.dom, bbs[i], &dom_bbs);
      for (j = 0; j < n_dom_bbs; j++)
	{
	  dom_bb = dom_bbs[j];
	  if (flow_bb_inside_loop_p (loop, dom_bb))
	    continue;
	  set_immediate_dominator (loops->cfg.dom, dom_bb, switch_bb);
	}
      free (dom_bbs);
    }
  free (bbs);

  /* Now the hard case.  Dominators of blocks inside loop immediatelly
     dominated by unswitch_on may behave really weird.  */
  n_dom_bbs = get_dominated_by (loops->cfg.dom, unswitch_on, &dom_bbs);
  j = 0;
  for (i = 0; i < n_dom_bbs; i++)
    if (flow_bb_inside_loop_p (loop, dom_bbs[i]))
      dom_bbs[j++] = dom_bbs [i];
  iterate_fix_dominators (loops->cfg.dom, dom_bbs, j, 1);
  free (dom_bbs);

  n_dom_bbs = get_dominated_by (loops->cfg.dom, unswitch_on_alt, &dom_bbs);
  j = 0;
  for (i = 0; i < n_dom_bbs; i++)
    if (flow_bb_inside_loop_p (nloop, dom_bbs[i]))
      dom_bbs[j++] = dom_bbs [i];
  iterate_fix_dominators (loops->cfg.dom, dom_bbs, j, 1);
  free (dom_bbs);
  
  return nloop;
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

/* Copies structure of LOOP into TARGET.  */
static struct loop *
duplicate_loop (loops, loop, target)
     struct loops *loops;
     struct loop *loop;
     struct loop *target;
{
  struct loop *cloop;
  cloop = xcalloc (1, sizeof (struct loop));
  place_new_loop (loops, cloop);

  /* Initialize copied loop.  */
  cloop->level = loop->level;

  /* Set it as copy of loop.  */
  loop->copy = cloop;

  /* Add it to target.  */
  flow_loop_tree_node_add (target, cloop);

  return cloop;
}

/* Copies structure of subloops of LOOP into TARGET.  */
static void 
duplicate_subloops (loops, loop, target)
     struct loops *loops;
     struct loop *loop;
     struct loop *target;
{
  struct loop *aloop, *cloop;

  for (aloop = loop->inner; aloop; aloop = aloop->next)
    {
      cloop = duplicate_loop (loops, aloop, target);
      duplicate_subloops (loops, aloop, cloop);
    }
}

/*  Copies structure of COPIED_LOOPS into TARGET.  */
static void 
copy_loops_to (loops, copied_loops, n, target)
     struct loops *loops;
     struct loop **copied_loops;
     int n;
     struct loop *target;
{
  struct loop *aloop;
  int i;

  for (i = 0; i < n; i++)
    {
      aloop = duplicate_loop (loops, copied_loops[i], target);
      duplicate_subloops (loops, copied_loops[i], aloop);
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

  if (!e->src->succ->succ_next)
    {
      /* Code of cfglayout should be able to handle this, and redirection
	 is happier.  */
      e->flags |= EDGE_FALLTHRU;
    }

    cfg_layout_redirect_edge (e, dest);
  }

/* Deletes edge if possible.  */
static void
loop_delete_branch_edge (e)
     edge e;
{
  basic_block src = e->src;

  if (src->succ->succ_next)
    {
      int tot_pr, tot_rem;
      edge ae;

      /* Cannot handle more than two exit edges.  */
      if (src->succ->succ_next->succ_next)
	return;
      /* Neither this.  */
      if (!any_condjump_p (src->end))
	return;

      /* Try to fix probabilities.  This is probably as wrong as to leave
	 them as they are.  */
      tot_pr = 0;
      for (ae = src->succ; ae; ae = ae->succ_next)
	tot_pr += ae->probability;
      tot_rem = tot_pr - e->probability;
      if (tot_rem > 0)
	{
	  for (ae = src->succ; ae; ae = ae->succ_next)
	    ae->probability = (ae->probability * tot_pr) / tot_rem;
	}

      delete_insn (src->end);
      
      remove_edge (e);
      src->succ->flags |= EDGE_FALLTHRU;
    }
  else
    {
      /* Cannot happen -- we are duplicating loop! */
      abort ();
    }
}

/* Duplicates BBS. Newly created bbs are placed into NEW_BBS, edges to
   header (target of ENTRY) and copy of header are returned, edge ENTRY
   is redirected to header copy.  Assigns bbs into loops, updates
   dominators.  */
static void
copy_bbs (bbs, n, entry, latch_edge, new_bbs, loops, header_edge, copy_header_edge)
     basic_block *bbs;
     int n;
     edge entry;
     edge latch_edge;
     basic_block **new_bbs;
     struct loops *loops;
     edge *header_edge;
     edge *copy_header_edge;
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
      /* Possibly set header.  */
      if (bb->loop_father->header == bb && bb != header)
	new_bb->loop_father->header = new_bb;
      /* Or latch.  */
      if (bb->loop_father->latch == bb &&
	  bb->loop_father != header->loop_father)
	new_bb->loop_father->latch = new_bb;
    }

  /* Set dominators.  */
  for (i = 0; i < n; i++)
    {
      bb = bbs[i];
      new_bb = (*new_bbs)[i];
      if (bb != header)
	{
	  /* For anything than loop header, just copy it.  */
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

/* Remove edges going out from BBS, except edge to HEADER.  */
static void
remove_exit_edges (bbs, n, header)
     basic_block *bbs;
     int n;
     basic_block header;
{
  basic_block bb;
  edge e;
  int i;

  /* duplicated is not supposed to be used for this, but I need
     something to mark blocks.  */
  for (i = 0 ; i < n; i++)
    {
      bb = bbs[i];
      RBI (bb)->duplicated = 1;
    }
  RBI (header)->duplicated = 1;

  for (i = 0 ; i < n; i++)
    {
      edge succ_e;
      bb = bbs[i];
      for (e = bb->succ; e; e = succ_e)
	{
	  succ_e = e->succ_next;
	  if (!RBI (e->dest)->duplicated)
	    loop_delete_branch_edge (e);
	}
    }

  for (i = 0 ; i < n; i++)
    {
      bb = bbs[i];
      RBI (bb)->duplicated = 0;
    }
  RBI (header)->duplicated = 0;
}

/* Check whether LOOP's body can be duplicated.  */
bool
can_duplicate_loop_p (loop)
     struct loop *loop;
{
  basic_block *bbs;
  int i;

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

/* Duplicates body of LOOP to given edge E NDUPL times.  Takes care of
   updating LOOPS structure.  E's destination must be LOOP header for this to
   work.  Remove exit edges from copies corresponding to set bits in WONT_EXIT
   (bit 0 corresponds to original LOOP body).  Returns false if
   duplication is impossible.  */
int
duplicate_loop_to_header_edge (loop, e, loops, ndupl, wont_exit, flags)
     struct loop *loop;
     edge e;
     struct loops *loops;
     int ndupl;
     int wont_exit;
     int flags;
{
  struct loop *target, *aloop;
  struct loop **orig_loops;
  int n_orig_loops;
  basic_block header = loop->header, latch = loop->latch;
  basic_block *new_bbs, *bbs, *first_active;
  basic_block new_bb, bb, first_active_latch = NULL;
  edge ae, latch_edge, he;
  int i, j, n, more_active = 0;
  int is_latch = (latch == e->src);
  int k0, k, kk, freq_in, freq_e, freq_le;
  int loop_made_infinite;

  if (e->dest != loop->header)
    abort ();
  if (ndupl <= 0)
    abort ();

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

  /* Find edge from latch.  */
  for (latch_edge = header->pred;
       latch_edge->src != latch;
       latch_edge = latch_edge->pred_next);

  /* For updating frequencies.  */
  freq_e = EDGE_FREQUENCY (e);
  freq_in = header->frequency;
  freq_le = EDGE_FREQUENCY (latch_edge);

  if (is_latch)
    {
      /* Should work well unless something inside depends on parity of
	 iteration counter.  */
      if (freq_in > freq_e)
	{
	  k0 = REG_BR_PROB_BASE;
	  for (i = 0; i < ndupl + 1; i++)
	    k0 = (k0 * freq_e) / freq_in;
	  k0 = REG_BR_PROB_BASE - k0;
	  k0 = (((REG_BR_PROB_BASE * REG_BR_PROB_BASE) / freq_in) * 
		(freq_in - freq_e)) / k0;
	  k = (REG_BR_PROB_BASE * freq_e) / freq_in;
	}
      else
	{
	  k0 = REG_BR_PROB_BASE / (ndupl + 1);
	  k = REG_BR_PROB_BASE;
	}
      kk = k0;
    }
  else
    {
      /* Count probability that we will get to latch from header.
	 This is wrong; first iteration of cycle is certainly somewhat
	 special.  But I cannot do anything with it. */

      /* Strange things may happen to frequencies.  :-(  */
      if (freq_le == 0)
	freq_le = 1;
      if (freq_in <= freq_le)
	freq_in = freq_le + 1;
      if (freq_in < freq_le + freq_e)
	freq_e = freq_in - freq_le;

      k = (REG_BR_PROB_BASE * freq_le) / freq_in;
      kk = (REG_BR_PROB_BASE * freq_e) / freq_le;
      k0 = kk;
      for (i = 0; i < ndupl; i++)
	k0 = (k0 * k) / REG_BR_PROB_BASE;
      k0 = (k0 * freq_le) / REG_BR_PROB_BASE + freq_in - freq_le - freq_e;
      k0 = (REG_BR_PROB_BASE * k0) / (freq_in - freq_le);
      if (k0 < 0)
	k0 = 0;
    }
  if (k0 < 0 || k0 > REG_BR_PROB_BASE ||
      k < 0  || k > REG_BR_PROB_BASE ||
      kk < 0 || kk * k > REG_BR_PROB_BASE * REG_BR_PROB_BASE)
    {
      /* Something is wrong.  */
      abort ();
    }

  /* Check whether we will create an infinite loop.  */
  if (is_latch)
    {
      loop_made_infinite = 1;
      for (i = 0; i <= ndupl; i++)
	if (!(wont_exit & (1 << i)))
	  {
	    loop_made_infinite = 0;
	    break;
	  }
    }
  else
    loop_made_infinite = (wont_exit & 1);
  /* We cannot handle infinite loops yet.  It is relatively hard to do,
     as outer loops might become infinite too.  */
  if (loop_made_infinite)
    abort ();

  /* Loop to that new bbs will belong.  */
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
  if (is_latch && !(wont_exit & 1))
    {
      memcpy (first_active, bbs, n * sizeof (basic_block));
      first_active_latch = latch;
    }
  
  for (j = 0; j < ndupl; j++)
    {
      /* Copy loops.  */
      copy_loops_to (loops, orig_loops, n_orig_loops, target);

      /* Copy bbs.  */
      copy_bbs (bbs, n, e, latch_edge, &new_bbs, loops, &e, &he);
      if (is_latch)
	loop->latch = RBI (latch)->copy;

      /* Set counts and frequencies.  */
      kk = (kk * k) / REG_BR_PROB_BASE;
      for (i = 0; i < n; i++)
	{
	  new_bb = new_bbs[i];
	  bb = bbs[i];
	  if (flags & DLTHE_FLAG_UPDATE_FREQ)
	    {
	      new_bb->count = (kk * bb->count +
			       REG_BR_PROB_BASE / 2) / REG_BR_PROB_BASE;
	      new_bb->frequency = (bb->frequency * kk +
				   REG_BR_PROB_BASE / 2) / REG_BR_PROB_BASE;
	      for (ae = new_bb->succ; ae; ae = ae->succ_next)
		ae->count = (new_bb->count * ae->probability +
			 REG_BR_PROB_BASE / 2) / REG_BR_PROB_BASE;
	    }
	  else
	    {
	      new_bb->count = bb->count;
	      new_bb->frequency = bb->frequency;
	      for (ae = new_bb->succ; ae; ae = ae->succ_next)
		ae->count = (new_bb->count * ae->probability +
			 REG_BR_PROB_BASE / 2) / REG_BR_PROB_BASE;
	    }
	}

      /* Remove exit edges if needed.  */
      if (wont_exit & (1 << (j + 1)))
	remove_exit_edges (new_bbs, n, header);
      else if (!first_active_latch)
	{
	  memcpy (first_active, new_bbs, n * sizeof (basic_block));
	  first_active_latch = RBI (latch)->copy;
	}
      else
	more_active = 1;
      
      /* Update edge counts.  */
      for (i = 0; i < n; i++)
	{
	  bb = new_bbs[i];
	  for (ae = bb->succ; ae; ae = ae->succ_next)
	    ae->count = (bb->count * ae->probability +
			 REG_BR_PROB_BASE / 2) / REG_BR_PROB_BASE;
	}

      /* Update loops info.  */
      for (i = 0; i < n_orig_loops; i++)
	flow_loop_scan (loops, orig_loops[i]->copy, 0);

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
  free (orig_loops);

  /* Now handle original loop.  */
  
  /* Remove exit edges if needed.  */
  if (wont_exit & 1)
    remove_exit_edges (bbs, n, latch_edge->dest);
 
  /* Update edge counts.  */
  if (flags & DLTHE_FLAG_UPDATE_FREQ)
    {
      for (i = 0; i < n; i++)
	{
	  bb = bbs[i];
	  bb->count = (k0 * bb->count +
		       REG_BR_PROB_BASE / 2) / REG_BR_PROB_BASE;
	  bb->frequency = (bb->frequency * k0 +
			   REG_BR_PROB_BASE / 2) / REG_BR_PROB_BASE;
	  for (ae = bb->succ; ae; ae = ae->succ_next)
	    ae->count = (bb->count * ae->probability +
			 REG_BR_PROB_BASE / 2) / REG_BR_PROB_BASE;
	}
    }

  if (!first_active_latch)
    {
      memcpy (first_active, bbs, n * sizeof (basic_block));
      first_active_latch = latch;
    }
  else
    more_active = 1;

  /* Update dominators of other blocks if affected.  */
  if (flags & DLTHE_FLAG_UPDATE_DOMINATORS)
    {
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
	      if (more_active)
		{
		  dom_bb = nearest_common_dominator (
		    loops->cfg.dom, first_active[i], first_active_latch);
		}
	      else
		dom_bb = first_active[i];
	      set_immediate_dominator (loops->cfg.dom, dominated, dom_bb);
	    }
	  free (dom_bbs);
	}
    }
  free (first_active);

  free (bbs);

  /* Fill other info for father loops.  */
  for (aloop = target; aloop->depth > 0; aloop = aloop->outer)
    flow_loop_scan (loops, aloop, 0);

  return true;

}

