/* Loop optimizer initialization routines.
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
#include "hard-reg-set.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "cfglayout.h"
#include "gcov-io.h"
#include "profile.h"

static void fixup_loop_exit_succesor (basic_block, basic_block);


/* Initialize loop optimizer.  */

struct loops *
loop_optimizer_init (dumpfile)
     FILE *dumpfile;
{
  struct loops *loops = xcalloc (1, sizeof (struct loops));
  edge e;

  /* Avoid anoying special cases of edges going to exit
     block.  */
  for (e = EXIT_BLOCK_PTR->pred; e; e = e->pred_next)
    if ((e->flags & EDGE_FALLTHRU) && e->src->succ->succ_next)
      split_edge (e);

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
  create_preheaders (loops, CP_SIMPLE_PREHEADERS | CP_INSIDE_CFGLAYOUT);

  /* Force all latches to have only single successor.  */
  force_single_succ_latches (loops);

  /* Mark irreducible loops.  */
  mark_irreducible_loops (loops);

  /* Dump loops.  */
  flow_loops_dump (loops, dumpfile, NULL, 1);

#ifdef ENABLE_CHECKING
  verify_dominators (loops->cfg.dom);
  verify_loop_structure (loops);
#endif

  return loops;
}

/* The first basic block is moved after the second in the reorder chain.  */
static void
fixup_loop_exit_succesor (basic_block exit_succ, basic_block latch)
{
  basic_block bb = exit_succ;
  basic_block bb1 = latch;

  if (!(bb && RBI (bb)->next))
    return;

  if (!bb1)
    return;
 

  if (bb && RBI (bb)->next)
    {
      basic_block c = ENTRY_BLOCK_PTR->next_bb;

      while (RBI (c)->next != bb)
	c = RBI (c)->next;

      RBI (c)->next = RBI (bb)->next;
    }

  if(  RBI (bb1)->next == NULL)
    {
      RBI (bb1)->next=bb;
      RBI (bb)->next=NULL;
    }
  else
    
    {
      basic_block tmp;

      tmp = RBI (bb1)->next;
      RBI (bb1)->next = bb;
      RBI (bb)->next = tmp;
    }
}

/* Finalize loop optimizer.  */
void
loop_optimizer_finalize (loops, dumpfile)
     struct loops *loops;
     FILE *dumpfile;
{
  basic_block bb;
  unsigned int i;

  /* Finalize layout changes.  */
  /* Make chain.  */
  FOR_EACH_BB (bb)
    if (bb->next_bb != EXIT_BLOCK_PTR)
      RBI (bb)->next = bb->next_bb;

  /* Another dump.  */
  free (loops->cfg.rc_order);
  loops->cfg.rc_order = NULL;
  free (loops->cfg.dfs_order);
  loops->cfg.dfs_order = NULL;
  flow_loops_dump (loops, dumpfile, NULL, 1);

  /* For loops ending with a branch and count instruction, move the basic 
     block found before the unrolling on the fallthru path of this branch,
     after the unrolled code.  This will allow branch simplification.  */
  for (i = 1; i < loops->num; i++)
    {
      struct loop *loop = loops->parray[i];
      struct loop_desc *desc;
      basic_block loop_real_latch, bb, bb_exit;
      edge e;

      if (loop == NULL)
	continue;
      if (!loop->simple)
        continue;
      if (!loop->has_desc)
	continue;

      if (loop->lpt_decision.decision != LPT_UNROLL_RUNTIME)
	continue;

      desc = &loop->desc;
      if (desc == NULL)
        continue;
      if (loop->latch->pred == NULL)
        continue;

      loop_real_latch = loop->latch->pred->src;
 

      if (desc->postincr)
	continue; 
      if (!is_bct_cond (BB_END (loop_real_latch)))
	 continue;

      for (e = loop_real_latch->succ; e ; e = e->succ_next)
	if (e->flags & EDGE_FALLTHRU)
          break;

      if (e == NULL)
	continue;

      bb_exit = e->dest;
     
      bb = NULL;

      /* Leave the case of the bb_exit falling through to exit to 
         fixed_fallthru_exit_predecessor */
      for (e = EXIT_BLOCK_PTR->pred; e; e = e->pred_next)
         if (e->flags & EDGE_FALLTHRU)
        bb = e->src;
  
      if (bb_exit == bb)
	continue;
      
      fixup_loop_exit_succesor (bb_exit, loop->latch);
    }

  /* Clean up.  */
  flow_loops_free (loops);
  free (loops);
 
  /* Finalize changes.  */
  cfg_layout_finalize ();

  /* Checking.  */
#ifdef ENABLE_CHECKING
  verify_flow_info ();
#endif
}

