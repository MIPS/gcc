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

  /* Create pre-headers.  */
  create_preheaders (loops);

  /* Dump loops.  */
  flow_loops_dump (loops, dumpfile, NULL, 1);

  /* Initialize structures for layout changes.  */
  cfg_layout_initialize (loops);

  /* Force all latches to have only single successor.  */
  force_single_succ_latches (loops);

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

