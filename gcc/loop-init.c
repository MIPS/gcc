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
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "cfglayout.h"
#include "gcov-io.h"
#include "profile.h"
#include "flags.h"
#include "timevar.h"

/* Estimate on number of available registers in loops.  */
int *loop_avail_regs;

/* Initialize loop optimizer.  */

struct loops *
loop_optimizer_init (dumpfile)
     FILE *dumpfile;
{
  struct loops *loops = xcalloc (1, sizeof (struct loops));
  edge e;

  /* Avoid annoying special cases of edges going to exit
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

  /* Not going to update these.  */
  free (loops->cfg.rc_order);
  loops->cfg.rc_order = NULL;
  free (loops->cfg.dfs_order);
  loops->cfg.dfs_order = NULL;

  /* Initialize structures for layout changes.  */
  cfg_layout_initialize (loops);

  /* Create pre-headers.  */
  create_preheaders (loops, CP_SIMPLE_PREHEADERS | CP_INSIDE_CFGLAYOUT);

  /* Force all latches to have only single successor.  */
  force_single_succ_latches (loops);

  /* Mark irreducible loops.  */
  mark_irreducible_loops (loops);

  /* Do we have histograms?  */
  if (find_counters_section (GCOV_TAG_LOOP_HISTOGRAMS)->present)
    move_histograms_to_loops (loops);

  /* Dump loops.  */
  flow_loops_dump (loops, dumpfile, NULL, 1);

#ifdef ENABLE_CHECKING
  verify_dominators (loops->cfg.dom);
  verify_loop_structure (loops);
#endif

  return loops;
}

/* Finalize loop optimizer.  */
void
loop_optimizer_finalize (loops, dumpfile)
     struct loops *loops;
     FILE *dumpfile;
{
  basic_block bb;

  /* Finalize layout changes.  */
  /* Make chain.  */
  FOR_EACH_BB (bb)
    if (bb->next_bb != EXIT_BLOCK_PTR)
      RBI (bb)->next = bb->next_bb;

  /* Another dump.  */
  flow_loops_dump (loops, dumpfile, NULL, 1);

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

/* The driver that runs all loop optimizations.  */
void
loop_optimizer_optimize (struct loops *loops)
{
  struct movable_list *movables;
  struct ivopt_actions ivopt_actions;

  if (flag_unswitch_loops)
    unswitch_loops (loops);
  
  /* Now comes the part where we don't change cfg structure,
     just play with the code.  It will be nice to have landing
     pads in that part; we cannot create them in loop_optimizer_init,
     as unswitching might destroy them.  */
  create_landing_pads (loops);

  timevar_push (TV_IV_ANAL);
  initialize_iv_analysis (loops);
  analyse_induction_variables ();
  timevar_pop (TV_IV_ANAL);

  /* Do this decision now, so that we know the number of unrollings
     in prefetching.  */
  decide_unrolling_and_peeling (loops, 
				(flag_peel_loops ? UAP_PEEL : 0) |
				(flag_unroll_loops ? UAP_UNROLL : 0) |
				(flag_unroll_all_loops ? UAP_UNROLL_ALL : 0));

#ifdef HAVE_prefetch
  if (HAVE_prefetch && flag_prefetch_loop_arrays)
    prefetch_loop_arrays (loops);
#endif

  /* Detect the movables and induction variables to strength reduce.  */
  loop_avail_regs = xcalloc (loops->num, sizeof (int));
  movables = find_movables (loops);
  ivopt_actions.ivs = NULL;
  ivopt_actions.replacements = NULL;
  ivopt_actions.repl_final_value = NULL;
  if (flag_strength_reduce)
    detect_strength_reductions (loops, &ivopt_actions);
  free (loop_avail_regs);

  timevar_push (TV_IV_ANAL);
  finalize_iv_analysis ();
  timevar_pop (TV_IV_ANAL);

  /* Now move the movables and reduce the variables; it is easier to have it
     split into two parts, so that we do not have to update iv information.  */
  loops_invariant_motion (loops, movables);
  execute_strength_reductions (loops, &ivopt_actions);

  if (flag_peel_loops || flag_unroll_loops)
    unroll_and_peel_loops (loops);

  /* Rerun the optimization again, to improve the induction variable usage in
     the unrolled loops.  */
  if (optimize >= 3 && flag_unroll_loops)
    {
      timevar_push (TV_IV_ANAL);
      initialize_iv_analysis (loops);
      analyse_induction_variables ();
      timevar_pop (TV_IV_ANAL);

      loop_avail_regs = xcalloc (loops->num, sizeof (int));
      ivopt_actions.ivs = NULL;
      ivopt_actions.replacements = NULL;
      ivopt_actions.repl_final_value = NULL;
      if (flag_strength_reduce)
	detect_strength_reductions (loops, &ivopt_actions);
      free (loop_avail_regs);

      timevar_push (TV_IV_ANAL);
      finalize_iv_analysis ();
      timevar_pop (TV_IV_ANAL);
  
      execute_strength_reductions (loops, &ivopt_actions);
    }

#ifdef HAVE_doloop_end
  if (HAVE_doloop_end && flag_branch_on_count_reg)
    doloop_optimize_loops (loops);
#endif
}

