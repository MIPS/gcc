/* Perform optimization of induction variables.
   Copyright (C) 1987, 1988, 1989, 1991, 1992, 1993, 1994, 1995, 1996, 1997,
   1998, 1999, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

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
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "loop.h"
#include "cfglayout.h"
#include "params.h"
#include "output.h"
#include "expr.h"
#include "function.h"
#include "df.h"
#include "flags.h"
#include "insn-config.h"
#include "recog.h"
#include "regs.h"

#define EL_RTX
#include "algebraic.h"

/* For now, we do only strength reduction.  The optimization is done
   in two phases -- in the first one places where to perform it are
   detected, in the second one (in that we no longer have information
   about induction variables, so we don't need to update it) the optimization
   is performed.  Note that the same approach is taken in invariant motion;
   we do not collide, as from obvious reasons we operate on disjoint sets
   of insns.
   
   The heuristics to decide whether to perform the optimization is missing
   for now, it will be added later.
   
   The optimization itself is easy, all the neccessary information is provided
   by induction variable analysis.
   
   We proceed this way: for each iv class (starting from those with smaller
   steps) we pass all locations.  For each iv we then try to find best biv
   on that we may base the replacement (plus we try to introduce new biv).  */

/* A description of new iv from strength reduction.  */
struct str_red
{
  rtx reg;		/* Register to hold the new induction variable.  */
  int chosen;		/* Whether to use it or not.  */
  enum machine_mode mode; /* Mode of the induction variable.  */
  struct loop *loop;	/* Loop in that it should occur.  */
  rtx initial_value;	/* Value to that it should be initialized.  */
  rtx step;		/* Step of this iv.  */
  rtx value;		/* The value of iv.  */
  int cost;		/* For incrementing this iv.  */
  struct str_red *next;	/* In list of scheduled reductions.  */
};

/* A description of replacement alternative.  */
struct repl_alt
{
  struct str_red *iv;	/* On that it is based.  */
  rtx inc_by;		/* Increase the iv by this amount after preforming
			   the replacement.  */
  rtx seq;		/* The sequence to compute.  */
  rtx replacement;	/* Expression to replace it with (valid only for
			   memory address ivs; sets of ivs emit their
			   result directly into the target register).  */
  int cost;		/* Of the alternative.  */
  struct repl_alt *next; /* In list of alternatives.  */
};

/* A description of replacement.  */
struct repl
{
  rtx insn;		/* Insn in that to replace.  */
  rtx loc;		/* The location of replaced value.  */
  int cost;		/* Of the original computation.  */
  struct iv_occurence *occ; /* The occurence we care about.  */
  struct repl_alt *alts; /* Possible replacements.  */
  struct repl_alt *chosen; /* The alternative to use.  */
  struct repl *next;	/* The next replacement in list.  */
};

/* Description of replacement of the final value.  */
struct fval_repl
{
  rtx reg;		/* Register whose value is replaced.  */
  rtx value;		/* With this value.  */
  struct loop *loop;	/* In that replacement is done.  */
  struct fval_repl *next; /* The next replacement in list.  */
};

/* List of iv_occurences.  */
struct iv_occ_list
{
  struct iv_occ_list *next;
  struct iv_occurence *elt;
};

/* Array containing list of occurences for each insn.  */
struct iv_occ_list **insn_occurences;

#define COST_INFTY 1000000
#define MANY_IVS 30
#define ALTER_BY 3

/* Generic mergesort function on lists.  */
#define MERGESORT(SUFFIX, TYPE, NEXT, KEY)		\
							\
static TYPE *split_##SUFFIX (TYPE **);			\
static void merge_##SUFFIX (TYPE **, TYPE **);		\
static void mergesort_##SUFFIX (TYPE **);		\
							\
/* Splits the LIST into two equally sized parts.  */	\
static TYPE *						\
split_##SUFFIX (TYPE **list)				\
{							\
  TYPE *ret = (*list)->NEXT;				\
  TYPE *act = *list, *next;				\
							\
  for (; act->NEXT; act = next)				\
    {							\
      next = act->NEXT;					\
      act->NEXT = next->NEXT;				\
    }							\
							\
  act->NEXT = NULL;					\
  return ret;						\
}							\
							\
/* Merges the list L2 into list L1.  */			\
static void						\
merge_##SUFFIX (TYPE **l1, TYPE **l2)			\
{							\
  while (*l1 && *l2)					\
    {							\
      rtx key1 = (*l1)->KEY;				\
      rtx key2 = (*l2)->KEY;				\
      TYPE *nxt;					\
							\
      if (GET_CODE (key1) == CONST_INT			\
	  && (GET_CODE (key2) != CONST_INT		\
	      || INTVAL (key1) < INTVAL (key2)))	\
	l1 = &(*l1)->NEXT;				\
      else						\
	{						\
	  nxt = (*l2)->NEXT;				\
	  (*l2)->NEXT = *l1;				\
	  *l1 = *l2;					\
	  *l2 = nxt;					\
	}						\
    }							\
  if (!*l1)						\
    *l1 = *l2;						\
}							\
							\
/* Sort the LIST by BASE.  */				\
static void						\
mergesort_##SUFFIX (TYPE **list)			\
{							\
  TYPE *l1;						\
							\
  if (!*list || !(*list)->NEXT)				\
    return;						\
 							\
  l1 = split_##SUFFIX (list);				\
  mergesort_##SUFFIX (&l1);				\
  mergesort_##SUFFIX (list);				\
  merge_##SUFFIX (list, &l1);				\
}

/* Create the mergesort functions we need.  */
MERGESORT (step, struct iv_occurence_step_class, sc_next, step)
MERGESORT (base, struct iv_occurence_base_class, bc_next, base)
MERGESORT (delta, struct iv_occurence, oc_next, delta)

static void sort_iv_lists (struct iv_occurence_step_class **);
static bool decide_strength_reduction (struct loop *, struct iv_occurence *,
				       struct str_red **, struct repl **);
static void kill_old_global_iv (struct loop *, struct iv_occurence *,
				struct fval_repl **);
static bool decrease_by_one_iv (struct str_red *, struct repl *, int);
static bool alter_by_few_ivs (int, struct str_red **, struct repl *, int);
static int estimate_gain (struct str_red *, struct repl *, int);
static int computation_cost (struct loop *, struct iv_occurence *);
static bool iv_equal_p (struct str_red *, struct str_red *);
static void enter_alt (struct repl_alt **, struct repl_alt *);
#ifdef AUTO_INC_DEC
static void schedule_autoinc (struct loops *, struct loop *, struct str_red *,
			      struct repl *);
#endif
static void determine_reductions_to_perform (struct loop *, struct str_red **,
					     struct repl *);
static void finish_biv (struct str_red *);
static void finish_repl (struct repl *);
static void create_biv (struct str_red *);
static void replace_final_value (struct fval_repl *);
static void execute_strength_reduction (struct repl *);
static bool partial_computation_p (struct loop *, struct iv_occurence *);
static struct repl_alt *derive_iv_from (struct repl *, struct str_red *);
static int invariant_in_loop (rtx *, void *);
static bool invariant_displacement_p (struct loop *, rtx);
static struct str_red *make_default_ivs (struct loop *loop);
static void create_biv_for (struct loop *, struct iv_occurence *,
			    struct str_red *);
static bool schedule_repl_for (struct repl *, struct repl_alt *);
static rtx gen_repl_rtx (int, rtx, struct str_red *, enum machine_mode);
static int repl_mem_addr_cost (struct iv_occurence *, rtx, rtx *, rtx *,
			       struct str_red *);
static int repl_move_cost (struct iv_occurence *, rtx, rtx *);
extern void dump_strength_reductions (FILE *, struct str_red *, struct repl *);
extern void dump_new_biv (FILE *, struct str_red *);
extern void dump_sr_replacement (FILE *file, struct repl *repl);

/* Detects the induction variables to strength reduce in LOOPS; list of them
   is returned.  The list of actions to perform is returned in ACTIONS.  */
void
detect_strength_reductions (struct loops *loops, struct ivopt_actions *actions)
{
  struct iv_occurence_step_class *step;
  struct iv_occurence_base_class *base;
  struct iv_occurence *occ;
  struct loop *loop;
  unsigned i, max_uid = get_max_uid ();
  struct str_red **lred = &actions->ivs, *loop_reds;
  struct repl **lrepl = &actions->replacements, *loop_repls;
  struct iv_occ_list *nw, *nxt;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "\nStrength reduction:\n\n");

  /* Prepare array of lists of occurences in insns.  */
  insn_occurences = xcalloc (max_uid, sizeof (struct iv_occ_list *));
  for (i = 0; i < loops->num; i++)
    for (step = iv_occurences[i]; step; step = step->sc_next)
      for (base = step->bc_first; base; base = base->bc_next)
	for (occ = base->oc_first; occ; occ = occ->oc_next)
	  {
	    nw = xmalloc (sizeof (struct iv_occ_list));

	    nw->elt = occ;
	    nw->next = insn_occurences[INSN_UID (occ->insn)];
	    insn_occurences[INSN_UID (occ->insn)] = nw;
	  }

  loop = loops->tree_root;
  while (loop->inner)
    loop = loop->inner;

  while (1)
    {
      sort_iv_lists (&iv_occurences[loop->num]);

      loop_reds = make_default_ivs (loop);
      loop_repls = NULL;
      for (step = iv_occurences[loop->num]; step; step = step->sc_next)
	for (base = step->bc_first; base; base = base->bc_next)
	  for (occ = base->oc_first; occ; occ = occ->oc_next)
	    {
	      if (decide_strength_reduction (loop, occ, &loop_reds, &loop_repls))
  		kill_old_global_iv (loop, occ, &actions->repl_final_value);
	    }

      if (rtl_dump_file)
	{
	  fprintf (rtl_dump_file, "Loop %d:\n", loop->num);
	  fprintf (rtl_dump_file, "Before decision:\n");
	  dump_strength_reductions (rtl_dump_file, loop_reds, loop_repls);
	}

      if (loop_reds)
	{
	  determine_reductions_to_perform (loop, &loop_reds, loop_repls);
#ifdef AUTO_INC_DEC
	  schedule_autoinc (loops, loop, loop_reds, loop_repls);
#endif
	}
      
      if (rtl_dump_file)
	{
	  fprintf (rtl_dump_file, "\nAfter decision:\n");
	  dump_strength_reductions (rtl_dump_file, loop_reds, loop_repls);
	}

      *lred = loop_reds;
      while (*lred)
	lred = &(*lred)->next;

      *lrepl = loop_repls;
      while (*lrepl)
	lrepl = &(*lrepl)->next;

      if (!loop->next)
	{
	  loop = loop->outer;
	  if (loop == loops->tree_root)
	    break;
	  continue;
	}
      for (loop = loop->next; loop->inner; loop = loop->inner)
	continue;
    }

  for (i = 0; i < max_uid; i++)
    for (nw = insn_occurences[i]; nw; nw = nxt)
      {
	nxt = nw->next;
	free (nw);
      }
}

#ifdef AUTO_INC_DEC
/* Tries to put increment(s) immediatelly after memory reference so
   that it may be combined into autoinc addressing mode.  LOOP is the loop
   in that ivs are replaced by REPLS based on bivs REDS.  */
static void
schedule_autoinc (struct loops *loops, struct loop *loop, struct str_red *reds,
		  struct repl *repls)
{
  struct repl *last_repl, *repl;
  basic_block bb, last_bb;
  int size, step;

  for (; reds; reds = reds->next)
    {
      /* We check whether there is a mem whose address is based on the biv
	 that is always executed exactly once per iteration and postdominates
	 all other replacements.  If so and it also matches the size of mem
	 access, place the increment after it.
	 
	 ??? We could be more clever about this and use autoinc modes even for
	 mems that are between other replacements, or even using several
	 autoincrements for single iv (this is risky, as we don't know for sure
	 that the autoincrement will be created.  Of course we could verify
	 this).  It would however require to update the following replacements,
	 and we would have to take this into account already during decision
	 about replacements, as this could affect their cost.  Alternatively we
	 could do it provided that we check that the replacement is cheap.  */

      step = INTVAL (reds->step);
      last_repl = NULL;
      last_bb = NULL;
      /* Find candidate for the last mem.  */
      for (repl = repls; repl; repl = repl->next)
	{
	  if (repl->chosen->iv != reds)
	    continue;

	  bb = BLOCK_FOR_INSN (repl->insn);
	  if (!just_once_each_iteration_p (loops, loop, bb))
	    continue;

	  if (last_repl
	      && fast_dominated_by_p (last_bb, bb)
	      && (bb != last_bb
		  || (DF_INSN_LUID (loop_df, repl->insn)
		      < DF_INSN_LUID (loop_df, last_repl->insn))))
	    continue;

	  last_repl = repl;
	  last_bb = bb;
	}
      if (!last_repl
	  || GET_CODE (last_repl->loc) == SET)
	continue;

      /* Check that size maches.  */
      size = GET_MODE_SIZE (GET_MODE (last_repl->loc));
      if ((HAVE_POST_INCREMENT || HAVE_PRE_INCREMENT)
	  && step == size)
	;
      else if ((HAVE_POST_DECREMENT || HAVE_PRE_DECREMENT)
	  && step == -size)
	;
      else
	continue;

      /* Verify that the candidate indeed postdominates all other
	 replacements.  */
      for (repl = repls; repl; repl = repl->next)
	{
	  if (repl->chosen->iv != reds)
	    continue;

	  bb = BLOCK_FOR_INSN (repl->insn);

	  if (fast_dominated_by_p (bb, last_bb)
	      && (bb != last_bb
		  || (DF_INSN_LUID (loop_df, last_repl->insn)
		      < DF_INSN_LUID (loop_df, repl->insn))))
	    break;
	}

      if (repl)
	continue;

      last_repl->chosen->inc_by = last_repl->chosen->iv->step;
      last_repl->chosen->iv->step = const0_rtx;
    }
}
#endif

/* Attempts to kill induction variable OCC that would not become dead
   because it is used outside of LOOP.  We may sometimes achieve this
   by computing the final value and assigning it on exit; if we are
   successful, add the description of the operation to list FREPL.  */
static void
kill_old_global_iv (struct loop *loop, struct iv_occurence *occ,
		    struct fval_repl **frepl)
{
  rtx reg;
  struct df_link *def, *use, *ddef;
  basic_block bb = BLOCK_FOR_INSN (occ->insn);
  int pre_exit;
  rtx val, it;
  struct fval_repl *nw;

  /* We need to gave a place where to move the computation, and to know that
     the definition is evaluated in every iteration.  Also we must know
     number of iterations and it must be reliable (which also means that
     there must be exactly one exit out of the loop).
     
     ??? We could precondition the loop and thus get rid of some of these
     conditions.  */
  if (!loop->landing_pad
      || !fast_dominated_by_p (loop->latch, bb)
      || loop->landing_pad->pred->pred_next
      || !loop->simple
      || loop->desc.assumptions
      || loop->desc.noloop_assumptions)
   return;

  if (GET_CODE (*occ->occurence) != SET)
    return;

  /* If the definition dominates exit, the situation is easy.  More complicated
     is the case when it postdominates the exit; in this case we must know that
     either the loop is executed at least once, or that the initial value
     matches what we compute.  */
  pre_exit = !fast_dominated_by_p (loop->landing_pad, bb);
  reg = SET_DEST (*occ->occurence);

  for (def = DF_INSN_DEFS (loop_df, occ->insn); def; def = def->next)
    if (DF_REF_REGNO (def->ref) == REGNO (reg))
      break;

  /* It should be a single def of the register inside loop.  ??? Not really
     neccessary, it would be enough to dominate all defs (but we would have
     to be careful if pre_exit).  */
  for (ddef = loop_df->regs[REGNO (reg)].defs; ddef; ddef = ddef->next)
    if (ddef->ref != def->ref
	&& flow_bb_inside_loop_p (loop, DF_REF_BB (ddef->ref)))
      break;
  if (ddef)
    return;

  /* Unless there is some use of the value outside of loop, it is dead
     anyway, so we have nothing to do.  */
  for (use = loop_df->regs[REGNO (reg)].uses; use; use = use->next)
    {
      if (flow_bb_inside_loop_p (loop, DF_REF_BB (use->ref)))
	continue;

      for (ddef = DF_REF_CHAIN (use->ref); ddef; ddef = ddef->next)
	if (ddef->ref == def->ref)
	  break;

      if (ddef)
	break;
    }
  if (!use)
    return;

  if (pre_exit
      && simplify_gen_relational (NE, SImode, loop->desc.mode, const0_rtx,
				  loop->desc.niter_expr) != const_true_rtx)
    {
      rtx ival = initial_values[loop->num][REGNO (reg)];
      rtx m1 = constm1_rtx;
      rtx fival = substitute_into_expr (occ->value, NULL, &m1, 0);

      fival = substitute_into_expr (fival, iv_interesting_reg,
				    initial_values[loop->num], SIE_SIMPLIFY);
      if (!fival
	  || !rtx_equal_p (fival, ival))
	return;
    }

  it = pre_exit
	  ? GEN_BINARY (MINUS, loop->desc.mode,
			loop->desc.niter_expr, const1_rtx)
	  : loop->desc.niter_expr;
  val = substitute_into_expr (occ->value, NULL, &it, SIE_SIMPLIFY);
  if (!val)
    return;

  nw = xmalloc (sizeof (struct fval_repl));
  nw->loop = loop;
  nw->reg = reg;
  nw->value = iv_omit_initial_values (val);
  nw->next = *frepl;
  *frepl = nw;

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "\nReplacing final value of occurence ");
      print_rtl (rtl_dump_file, *occ->occurence);
      fprintf (rtl_dump_file, "\n");

      fprintf (rtl_dump_file, "  loop %d\n", loop->num);
      fprintf (rtl_dump_file, "  value");
      print_rtl (rtl_dump_file, nw->value);
      fprintf (rtl_dump_file, "\n");
    }
}

/* Actually perform the reductions scheduled in ACTIONS.  */
void
execute_strength_reductions (struct ivopt_actions *actions)
{
  struct str_red *next, *reds;
  struct repl *repl, *rnext;
  struct fval_repl *frepl, *fnext;

  for (repl = actions->replacements; repl; repl = rnext)
    {
      rnext = repl->next;
      execute_strength_reduction (repl);
      free (repl->chosen);
      free (repl);
    }

  for (reds = actions->ivs; reds; reds = next)
    {
      next = reds->next;
      create_biv (reds);
      free (reds);
    }

  for (frepl = actions->repl_final_value; frepl; frepl = fnext)
    {
      fnext = frepl->next;
      replace_final_value (frepl);
      free (frepl);
    }
}

/* Sort the LIST so that first ivs with lower step and base are passed.  */
static void
sort_iv_lists (struct iv_occurence_step_class **list)
{
  struct iv_occurence_base_class *base;
  struct iv_occurence_step_class *step;

  for (step = *list; step; step = step->sc_next)
    {
      for (base = step->bc_first; base; base = base->bc_next)
	mergesort_delta (&base->oc_first);
      mergesort_base (&step->bc_first);
    }
  mergesort_step (list);
}

/* Decides how it is possible to strength reduce an occurence of induction
   variable OCC in LOOP and records it in lists REDS and REPLS.  Returns
   false if the occurence is not suitable.  */
static bool
decide_strength_reduction (struct loop *loop, struct iv_occurence *occ,
			   struct str_red **reds, struct repl **repls)
{
  rtx step = occ->base_class->step_class->step;
  struct str_red *selfs[3];
  int i;
  struct repl_alt *alt;
  struct repl *nw;

  if (GET_CODE (step) != CONST_INT
      || step == const0_rtx)
    return false;

  /* ??? Add handling of these.  */
  if (occ->real_mode != occ->extended_mode)
    return false;

  /* If this is a set occurence and there is exactly one use in other set,
     ignore it (we will optimize only the final result).  */
  if (partial_computation_p (loop, occ))
    return false;

  nw = xmalloc (sizeof (struct repl));
  nw->insn = occ->insn;
  nw->occ = occ;
  nw->chosen = NULL;
  nw->loc = *occ->occurence;
  nw->cost = computation_cost (loop, occ);
  nw->alts = NULL;
  nw->next = *repls;
  *repls = nw;

  /* Create new possibilities on that to base the iv -- self, self but
     without constant displacement and self without base.  */
  selfs[0] = xmalloc (sizeof (struct str_red));
  create_biv_for (loop, occ, selfs[0]);

  if (occ->delta != const0_rtx)
    {
      rtx local_base = occ->local_base;

      selfs[1] = xmalloc (sizeof (struct str_red));
      occ->local_base =
	      simplify_alg_expr (GEN_BINARY (OP_MINUS, occ->extended_mode,
					     COPY_EXPR (occ->local_base),
					     occ->delta));
      create_biv_for (loop, occ, selfs[1]);
      occ->local_base = local_base;
    }
  else
    selfs[1] = NULL;

  if (occ->base_class->base != const0_rtx)
    {
      rtx local_base = occ->local_base;

      selfs[2] = xmalloc (sizeof (struct str_red));
      occ->local_base = const0_rtx;
      create_biv_for (loop, occ, selfs[2]);
      occ->local_base = local_base;
    }
  else
    selfs[2] = NULL;

  /* Try to derive occurence from all possibilities.  */
  for (; *reds; reds = &(*reds)->next)
    {
      alt = derive_iv_from (nw, *reds);
      for (i = 0; i < 3; i++)
	if (selfs[i] && iv_equal_p (*reds, selfs[i]))
	  {
	    free (selfs[i]);
	    selfs[i] = NULL;
	  }

      if (!alt)
	continue;

      enter_alt (&nw->alts, alt);
    }
  for (i = 0; i < 3; i++)
    {
      if (!selfs[i])
	continue;

      alt = derive_iv_from (nw, selfs[i]);
      if (!alt)
	{
	  free (selfs[i]);
	  continue;
	}
	  
      enter_alt (&nw->alts, alt);
      *reds = selfs[i];
      reds = &selfs[i]->next;
    }

  if (!nw->alts)
    abort ();

  return true;
}

/* Checks whether two newly created ivs RED1 and RED2 are equal.  */
static bool
iv_equal_p (struct str_red *red1, struct str_red *red2)
{
  if (red1->mode != red2->mode)
    return false;

  return rtx_equal_p (red1->value, red2->value);
}

/* Enters ALT into sorted list of alternatives ALTS.  */
static void
enter_alt (struct repl_alt **alts, struct repl_alt *alt)
{
  for (; *alts; alts = &(*alts)->next)
    if ((*alts)->cost > alt->cost)
      break;
  alt->next = *alts;
  *alts = alt;
}

/* Checks whether the computation performed by OCC is only used as part
   of a chain of computations.  */
static bool
partial_computation_p (struct loop *loop, struct iv_occurence *occ)
{
  rtx reg, tgt;
  struct ref *tg;
  struct iv_occ_list *occl;

  if (GET_CODE (*occ->occurence) != SET)
    return false;

  reg = SET_DEST (*occ->occurence);
  tg = find_single_def_use (reg, loop, occ->insn);
  if (!tg)
    return false;
  tgt = DF_REF_INSN (tg);

  for (occl = insn_occurences[INSN_UID (tgt)]; occl; occl = occl->next)
    if (reg_mentioned_p (reg, *occl->elt->occurence))
      break;

  return occl != NULL;
}

/* Returns a cost of computing OCC, including partial computations before it.
   LOOP is the loop where OCC occurs.  */
static int
computation_cost (struct loop *loop, struct iv_occurence *occ)
{
  int cost, uid;
  struct df_link *use, *def;
  struct iv_occ_list *occl;

  if (occ->aux)
    return 0;
  occ->aux = &occ->aux;
  if (GET_CODE (*occ->occurence) == SET)
    cost = rtx_cost (*occ->occurence, SET);
  else
    cost = address_cost (XEXP (*occ->occurence, 0), GET_MODE (*occ->occurence));

  for (use = DF_INSN_USES (loop_df, occ->insn); use; use = use->next)
    {
      def = DF_REF_CHAIN (use->ref);
      if (!def
	  || def->next
	  || DF_REF_BB (def->ref)->loop_father != loop)
	continue;

      uid = INSN_UID (DF_REF_INSN (def->ref));
  
      for (occl = insn_occurences[uid]; occl; occl = occl->next)
	if (GET_CODE (*occl->elt->occurence) == SET
	    && DF_REF_REG (def->ref) == SET_DEST (*occl->elt->occurence))
	  break;

      if (!occl
	  || !partial_computation_p (loop, occl->elt))
	continue;

      cost += computation_cost (loop, occl->elt);
    }

  occ->aux = NULL;
  return cost;
}

/* Tries to create replacement alternative for replacement REPL derived from
   biv RED.  */
static struct repl_alt *
derive_iv_from (struct repl *repl, struct str_red *red)
{
  struct repl_alt *alt = xmalloc (sizeof (struct repl_alt));

  alt->iv = red;
  alt->inc_by = const0_rtx;
  alt->seq = NULL_RTX;
  alt->replacement = NULL_RTX;
  alt->cost = 0;
  alt->next = NULL;
  
  if (!schedule_repl_for (repl, alt))
    {
      free (alt);
      return NULL;
    }

  return alt;
}

/* Fills in data for replacement alternative ALT of replacement REPL.  Returns
   false if not succesful.  */
static bool
schedule_repl_for (struct repl *repl, struct repl_alt *alt)
{
  int really = alt->iv->reg != NULL_RTX;
  int occ_step = INTVAL (repl->occ->base_class->step_class->step);
  int red_step = INTVAL (alt->iv->step);
  int scale = occ_step / red_step;
  rtx val, repl_rtx, delta, seq, addr = NULL;
  int repl_cost;
  int n_regs = reg_rtx_no;

  if (occ_step % red_step != 0)
    return false;

  /* ??? We could support extending ivs, if we know that there is no
     overflow.  */
  if (GET_MODE_BITSIZE (alt->iv->mode)
      < GET_MODE_BITSIZE (repl->occ->extended_mode))
    return false;

  val = scale == 1
	  ? alt->iv->value
	  : simplify_alg_expr (GEN_BINARY (OP_MULT, alt->iv->mode,
					   COPY_EXPR (alt->iv->value),
					   CONST_INT_EXPR (scale)));
  if (alt->iv->mode != repl->occ->extended_mode)
    val = GEN_SHORTEN (repl->occ->extended_mode, val);
  delta = iv_omit_initial_values (
		simplify_alg_expr (GEN_BINARY (OP_MINUS,
					       repl->occ->extended_mode,
					       COPY_EXPR (repl->occ->value),
					       COPY_EXPR (val))));

  if (!invariant_displacement_p (alt->iv->loop, delta))
    return false;

  if (!really)
    alt->iv->reg = gen_reg_rtx (alt->iv->mode);

  repl_rtx = gen_repl_rtx (scale, delta, alt->iv, repl->occ->extended_mode);

  if (GET_CODE (*repl->occ->occurence) == MEM)
    repl_cost = repl_mem_addr_cost (repl->occ, repl_rtx, &seq, &addr,
				    alt->iv);
  else
    repl_cost = repl_move_cost (repl->occ, repl_rtx, &seq);

  /* If we are only computing costs, cancel the new pseudos created.  */
  if (!really)
    {
      reg_rtx_no = n_regs;
      alt->iv->reg = NULL_RTX;
      seq = NULL_RTX;
      addr = NULL_RTX;
    }

  if (repl_cost == COST_INFTY)
    return false;

  alt->seq = seq;
  alt->replacement = addr;
  alt->cost = repl_cost;
  alt->next = NULL;

  return true;
}

/* Called from invariant_displacement_p through for_each_rtx, checks
   whether the expression is invariant in loop.  */
static int
invariant_in_loop (rtx *x, void *data)
{
  struct loop *loop = data;
  struct df_link *def;

  if (GET_CODE (*x) != REG)
    return 0;
      
  for (def = loop_df->regs[REGNO (*x)].defs; def; def = def->next)
    if (flow_bb_inside_loop_p (loop, DF_REF_BB (def->ref)))
      return 1;

  return 0;
}

/* Check whether DELTA is an invariant (in LOOP) displacement that might be
   used in address.  */
static bool
invariant_displacement_p (struct loop *loop, rtx delta)
{
  return !for_each_rtx (&delta, invariant_in_loop, loop);
  if (good_constant_p (delta))
    return true;

  if (delta == frame_pointer_rtx)
    return true;

  if (GET_OPERATOR (delta) != OP_PLUS
      || GET_OPERATOR (ARG (delta, 0)) != OP_CONST_INT)
    return false;

  if (ARG (delta, 1) == frame_pointer_rtx
      || GET_OPERATOR (ARG (delta, 1)) == OP_CONST_ADDRESS)
    return true;

  return false;
}

/* A cost to replace addr of occurence OCC by REPL (derived from IV).
   Sequence SEQ is emitted to do so and addr is replaced by ADDR.
   Cost of the replacement is returned.  */
static int
repl_mem_addr_cost (struct iv_occurence *occ, rtx repl, rtx *seq, rtx *addr,
		    struct str_red *iv ATTRIBUTE_UNUSED)
{
  int cost = 0;
  rtx insn, set;
#ifdef AUTO_INC_DEC
  int size = GET_MODE_SIZE (GET_MODE (*occ->occurence));
  int auto_ben = (iv->cost + 1) / 2;
#endif

  start_sequence ();
  *addr = memory_address (GET_MODE (*occ->occurence), repl);
  *seq = get_insns ();
  end_sequence ();

  for (insn = *seq; insn; insn = NEXT_INSN (insn))
    {
      set = single_set (insn);
      if (set)
	cost += rtx_cost (set, SET);
      else
	cost++;
    }
  cost += address_cost (*addr, GET_MODE (*occ->occurence));

#ifdef AUTO_INC_DEC
  /* Slightly prefer the choice that gives us an opportunity to use
     auto inc/dec.  */
  if ((HAVE_POST_INCREMENT || HAVE_PRE_INCREMENT)
      && INTVAL (iv->step) == size)
    cost -= auto_ben;
  else if ((HAVE_POST_DECREMENT || HAVE_PRE_DECREMENT)
	   && INTVAL (iv->step) == -size)
    cost -= auto_ben;
#endif

  return cost;
}

/* A cost to replace set in occurence OCC by REPL.  Sequence SEQ is emitted to
   do so.  Cost of the replacement is returned.  */
static int
repl_move_cost (struct iv_occurence *occ, rtx repl, rtx *seq)
{
  int cost = 0;
  rtx insn, set, tmp, dest = SET_DEST (*occ->occurence);

  start_sequence ();
  tmp = force_operand (repl, dest);
  if (tmp != dest)
    emit_move_insn (dest, tmp);
  *seq = get_insns ();
  end_sequence ();

  for (insn = *seq; insn; insn = NEXT_INSN (insn))
    {
      set = single_set (insn);
      if (set)
	cost += rtx_cost (set, SET);
      else
	cost++;
    }

  return cost;
}

/* Generates a replacement rtx to replace giv based on biv RED scaled SCALE
   times and incremented by DELTA.  MODE is the desired mode of target
   rtx.  */
static rtx
gen_repl_rtx (int scale, rtx delta, struct str_red *red, enum machine_mode mode)
{
  rtx tmp = red->reg;

  if (scale != 1)
    tmp = gen_rtx_fmt_ee (MULT, red->mode, tmp, GEN_INT (scale));
  if (mode != red->mode)
    tmp = gen_rtx_fmt_ei (SUBREG, mode, tmp, 0);
  if (delta != const0_rtx)
    tmp = simplify_gen_binary (PLUS, mode, tmp, delta);

  return tmp;
}

/* Creates basic induction variable for occurence OCC in LOOP and stores
   it in RED.  Returns false if unable to.  */
static void
create_biv_for (struct loop *loop, struct iv_occurence *occ,
		struct str_red *red)
{
  rtx tmp;
  int n_regs = reg_rtx_no;

  red->initial_value = iv_omit_initial_values (copy_rtx (occ->local_base));
  red->mode = occ->extended_mode;
  red->reg = NULL_RTX;
  red->chosen = false;
  red->loop = loop;
  /* A hack for calling this from make_default_ivs.  */
  if (occ->base_class)
    red->step = occ->base_class->step_class->step;
  else
    red->step = const1_rtx;
  red->next = NULL;

  tmp = gen_reg_rtx (red->mode);
  tmp = GEN_BINARY (OP_PLUS, red->mode, tmp, red->step);
  red->cost = rtx_cost (tmp, SET);
  reg_rtx_no = n_regs;
  
  tmp = GEN_BINARY (OP_MULT, red->mode, gen_iteration (red->mode), red->step);
  tmp = GEN_BINARY (OP_PLUS, red->mode, COPY_EXPR (occ->local_base), tmp);
  red->value = simplify_alg_expr (tmp);
  if (!red->value)
    abort ();
}

/* Creates default bivs (SI and DI mode with step 1) for LOOP.  */
static struct str_red *
make_default_ivs (struct loop *loop)
{
  struct str_red *ivSI, *ivDI;
  struct iv_occurence occ;

  occ.local_base = const0_rtx;
  occ.base_class = NULL;
  ivSI = xmalloc (sizeof (struct str_red));
  ivDI = xmalloc (sizeof (struct str_red));

  occ.extended_mode = SImode;
  create_biv_for (loop, &occ, ivSI);
  occ.extended_mode = DImode;
  create_biv_for (loop, &occ, ivDI);

  ivSI->next = ivDI;
  return ivSI;
}

/* Creates new biv according to RED.  */
static void
create_biv (struct str_red *red)
{
  rtx seq, tmp;

  /* Prepare the initial value.  */
  start_sequence ();
  tmp = force_operand (red->initial_value, red->reg);
  if (tmp != red->reg)
    emit_move_insn (red->reg, tmp);
  seq = get_insns ();
  end_sequence ();
  hoist_insn_to_depth (red->loop, red->loop->depth - 1, seq, true);

  /* The step could be reduced to 0 by autoinc transformation.  */
  if (red->step != const0_rtx)
    {
      start_sequence ();
      tmp = simplify_gen_binary (PLUS, red->mode, red->reg, red->step);
      tmp = force_operand (tmp, red->reg);
      if (tmp != red->reg)
	emit_move_insn (red->reg, tmp);
      seq = get_insns ();
      emit_insn_after (seq, loop_latch_edge (red->loop)->src->end);
    }
}

/* Executes replacement for strength reduction REPL.  */
static void
execute_strength_reduction (struct repl *repl)
{
  rtx tmp, seq;

  emit_insn_before (repl->chosen->seq, repl->insn);

  if (repl->chosen->inc_by != const0_rtx)
    {
      start_sequence ();
      tmp = simplify_gen_binary (PLUS, repl->chosen->iv->mode,
				 repl->chosen->iv->reg, repl->chosen->inc_by);
      tmp = force_operand (tmp, repl->chosen->iv->reg);
      if (tmp != repl->chosen->iv->reg)
	emit_move_insn (repl->chosen->iv->reg, tmp);
      seq = get_insns ();
      emit_insn_after (seq, repl->insn);
    }

  if (GET_CODE (repl->loc) == SET)
    {
      delete_insn (repl->insn);
      return;
    }

  XEXP (repl->loc, 0) = repl->chosen->replacement;
}

/* Replaces the final value according to FREPL.  */
static void
replace_final_value (struct fval_repl *frepl)
{
  rtx rg = gen_reg_rtx (GET_MODE (frepl->reg)), seq, tmp;

  start_sequence ();
  tmp = force_operand (frepl->value, rg);
  if (tmp != rg)
    emit_move_insn (rg, tmp);
  seq = get_insns ();
  end_sequence ();
  hoist_insn_to_depth (frepl->loop, frepl->loop->depth - 1, seq, true);

  start_sequence ();
  emit_move_insn (frepl->reg, rg);
  seq = get_insns ();
  end_sequence ();
  hoist_insn_to_depth (frepl->loop, frepl->loop->depth - 1, seq, false);
}

/* Determines which bivs REDS to create and which replacements REPLS to
   perform and throws away the rest.  LOOP is the loop to that the ivs
   belong.  */
static void
determine_reductions_to_perform (struct loop *loop, struct str_red **reds,
				 struct repl *repls)
{
  struct repl *act;
  struct repl_alt *alt, *next;
  struct str_red *red, **rarray;
  int nivs, i, old_ivs = 0;

  /* Choose the cheapest possibility for every replacement.  */
  for (act = repls; act; act = act->next)
    {
      act->chosen = act->alts;
      act->chosen->iv->chosen = true;
    }

  if (!flag_reduce_all_givs)
    {
      /* We want to choose as good combination of ivs as possible.  We proceed
	 the following way:  if there are many ivs, we just try to decrease
	 the number of used ivs one by one as long as there is a choice that
	 is a win.  Otherwise we are a bit more clever and try taking a best
	 combination that differs from the current one in at most 3 ivs.
	 
	 This requieres us to be able to judge how much a given choice costs.
	 For every replacement we take gain
	 (cost of original computation - cost of the replacement) and sum
	 them.  Additionally we estimate the effect register availability.
	 For this we inherit from loop invariant motion an estimate on number
	 of free registers, we increase the number by number of old induction
	 variables.  For every iv above this number we add a penalty
	 proportional to number of places where it is used.  Then we also add
	 a penalty proportional to decrease of number of available regs.  */

      for (i = 0; i < (int) loop_max_regno; i++)
	if (TEST_BIT (iv_interesting_reg, i))
	  {
	    rtx base, step;
	    iv_split (loop_entry_values[loop->num][i], &base, &step);
	    if (base && GET_CODE (step) == CONST_INT && step != const0_rtx)
	      {
		loop_avail_regs[loop->num]++;
		old_ivs++;
	      }
	  }
      if (loop_avail_regs[loop->num] > PARAM_VALUE (STR_N_USABLE_REGS))
	loop_avail_regs[loop->num] = PARAM_VALUE (STR_N_USABLE_REGS);
      if (loop_avail_regs[loop->num] < 0)
	loop_avail_regs[loop->num] = 0;

      if (rtl_dump_file)
	fprintf (rtl_dump_file, "\nAvailable registers %d, old ivs %d\n",
		 loop_avail_regs[loop->num], old_ivs);

      for (red = *reds, nivs = 0; red; red = red->next)
	nivs++;

      if (nivs > MANY_IVS)
	{
	  while (decrease_by_one_iv (*reds, repls, loop_avail_regs[loop->num]))
	    continue;
	}
      else
	{
	  rarray = xmalloc (nivs * sizeof (struct str_red *));
	  for (red = *reds, nivs = 0; red; red = red->next)
	    {
	      rarray[nivs] = red;
	      nivs++;
	    }

	  while (alter_by_few_ivs (nivs, rarray, repls,
				   loop_avail_regs[loop->num]))
	    continue;

	  free (rarray);
	}

      for (act = repls; act; act = act->next)
	{
	  for (alt = act->alts; !alt->iv->chosen; alt = alt->next)
	    continue;

	  act->chosen = alt;
	}
    }
  
  /* Fill in the remaining info for every chosen replacement and iv.  */
  for (act = repls; act; act = act->next)
    {
      for (alt = act->alts; alt; alt = next)
	{
	  next = alt->next;
	  if (alt != act->chosen)
	    free (alt);
	}
      act->alts = NULL;
      act->chosen->next = NULL;
      
      if (!act->chosen->iv->reg)
	finish_biv (act->chosen->iv);
      finish_repl (act);
    }

  while (*reds)
    {
      red = *reds;
      if (red->chosen)
	reds = &red->next;
      else
	{
	  *reds = red->next;
	  free (red);
	}
    }
}

/* Tries to improve assignments of replacements REPLS to induction variables
   REDS by removing one of chosen ivs.  FREE_REGS is number of free registers.
   Returns true if the removal was done.  */
static bool
decrease_by_one_iv (struct str_red *reds, struct repl *repls, int free_regs)
{
  struct str_red *remove = NULL, *red;
  int gain = estimate_gain (reds, repls, free_regs);
  int again;

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Choice ");
      for (red = reds; red; red = red->next)
	fprintf (rtl_dump_file, "%d", red->chosen ? 1 : 0);
      fprintf (rtl_dump_file, " gain %d\n", gain);
    }

  for (red = reds; red; red = red->next)
    if (red->chosen)
      {
	red->chosen = false;
	again = estimate_gain (reds, repls, free_regs);
	if (again > gain)
	  {
	    remove = red;
	    gain = again;
	  }
	red->chosen = true;
      }

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Choice ");
      for (red = reds; red; red = red->next)
	fprintf (rtl_dump_file, "%d", red->chosen ? 1 : 0);
      fprintf (rtl_dump_file, " gain %d\n", gain);
    }

  return remove != NULL;
}

/* Tries to improve assignments of replacements REPLS to induction variables
   RARRAY by changing chosenness of at most ALTER_BY ivs.  FREE_REGS is
   the number of free registers.  Returns true if the change was done.  */
static bool
alter_by_few_ivs (int nreds, struct str_red **rarray, struct repl *repls,
		  int free_regs)
{
  int gain = estimate_gain (rarray[0], repls, free_regs);
  int again;
  int bchoices[ALTER_BY], blen = 0;
  int choices[ALTER_BY];
  int i, j;

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Choice ");
      for (i = 0; i < nreds; i++)
	fprintf (rtl_dump_file, "%d", rarray[i]->chosen ? 1 : 0);
      fprintf (rtl_dump_file, " gain %d\n", gain);
    }

  for (i = 1; i <= (nreds < ALTER_BY ? nreds : ALTER_BY); i++)
    {
      for (j = 0; j < i; j++)
	choices[j] = i - j - 1;

      while (1)
	{
	  for (j = 0; j < i; j++)
	    rarray[choices[j]]->chosen = !rarray[choices[j]]->chosen;

	  again = estimate_gain (rarray[0], repls, free_regs);
	  if (again > gain)
	    {
	      blen = i;
	      for (j = 0; j < i; j++)
		bchoices[j] = choices[j];
	      gain = again;
	    }

	  for (j = 0; j < i; j++)
	    rarray[choices[j]]->chosen = !rarray[choices[j]]->chosen;

	  for (j = 0; j < i; j++)
	    if (choices[j] != nreds - j - 1)
	      break;
	  if (i == j)
	    break;
	  choices[j]++;
	  for (; j > 0; j--)
	    choices[j - 1] = choices[j] + 1;
	}
    }

  for (j = 0; j < blen; j++)
    rarray[bchoices[j]]->chosen = !rarray[bchoices[j]]->chosen;

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Choice ");
      for (i = 0; i < nreds; i++)
	fprintf (rtl_dump_file, "%d", rarray[i]->chosen ? 1 : 0);
      fprintf (rtl_dump_file, " gain %d\n", gain);
    }

  return blen != 0;
}

/* Estimates gain when chosen ivs REDS are used for replacements REPLS.
   FREE_REGS is number of free registers.*/
static int
estimate_gain (struct str_red *reds, struct repl *repls, int free_regs)
{
  int gain = 0, n_repls = 0, n_chosen = 0;
  struct str_red *red;
  struct repl *act;
  struct repl_alt *alt;

  /* Choose the best choice for replacements.  */
  for (act = repls; act; act = act->next)
    {
      for (alt = act->alts; alt && !alt->iv->chosen; alt = alt->next)
	continue;

      if (!alt)
	return -COST_INFTY;
      act->chosen = alt;

      gain += act->cost - act->chosen->cost;
      n_repls++;
    }
  for (red = reds; red; red = red->next)
    if (red->chosen)
      {
	gain -= red->cost;
	n_chosen++;
      }

  /* Account for registers that are not available.  */
  if (n_chosen > free_regs)
    gain -= PARAM_VALUE (STR_SPILL_COST) * (n_chosen - free_regs) * n_repls
	    / n_chosen;

  /* Account for change in number of ivs.  */
  gain -= n_chosen * PARAM_VALUE (STR_REG_WEIGHT);

  return gain;
}

/* Fill in the remaining fields in biv RED.  */
static void
finish_biv (struct str_red *red)
{
  red->reg = gen_reg_rtx (red->mode);
}

/* Fill in remaining fields in replacement repl.  */
static void
finish_repl (struct repl *repl)
{
  schedule_repl_for (repl, repl->chosen);
}

/* Dumps information about scheduled strength reductions REDS and REPLS to
   FILE.  */
void
dump_strength_reductions (FILE *file, struct str_red *reds, struct repl *repls)
{
  fprintf (file, "\nNew ivs for strength reduction:\n\n");
  for (; reds; reds = reds->next)
    dump_new_biv (file, reds);

  fprintf (file, "\nReplacements:\n\n");
  for (; repls; repls = repls->next)
    dump_sr_replacement (file, repls);
}

/* Dumps information about new iv RED to FILE.  */
void
dump_new_biv (FILE *file, struct str_red *red)
{
  fprintf (file, "New variable %p", (void *) red);
  if (red->reg)
    {
      fprintf (file, " -- ");
      print_rtl (file, red->reg);
    }
  fprintf (file, "\n");

  fprintf (file, "  initial value ");
  print_rtl (file, red->initial_value);
  fprintf (file, "\n");

  fprintf (file, "  step ");
  print_rtl (file, red->step);
  fprintf (file, "\n");

  fprintf (file, "  value ");
  print_rtl (file, red->value);
  fprintf (file, "\n"); 

  fprintf (file, "  cost %d\n\n", red->cost);
}

/* Dumps information about replacement REPL to FILE.  */
void
dump_sr_replacement (FILE *file, struct repl *repl)
{
  struct repl_alt *alt;

  fprintf (file, "Replacement %p, insn %d\n", (void *) repl,
	   INSN_UID (repl->insn));

  fprintf (file, "  occurence ");
  print_rtl (file, *repl->occ->occurence);
  fprintf (file, "\n"); 

  fprintf (file, "  cost %d\n", repl->cost);

  for (alt = repl->alts ? repl->alts : repl->chosen; alt; alt = alt->next)
    {
      fprintf (file, "  alt %p\n", (void *) alt);

      fprintf (file, "    based on %p", (void *) alt->iv);
      if (alt->iv->reg)
	{
	  fprintf (file, " -- ");
	  print_rtl (file, alt->iv->reg);
	}
      fprintf (file, "\n");
     
      fprintf (file, "    cost %d\n", alt->cost);

      if (alt->seq)
	{
	  fprintf (file, "    seq\n");
	  print_rtl (file, alt->seq);
	}

      if (alt->replacement)
	{
	  fprintf (file, "    replacement");
	  print_rtl (file, alt->replacement);
	  fprintf (file, "\n"); 
	}
    }
  fprintf (file, "\n"); 
}
