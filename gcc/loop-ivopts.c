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
   
   The optimization itself is easy, all the neccessary information is provided
   by induction variable analysis.
   
   We proceed this way: for each iv class (starting from those with smaller
   steps) we pass all locations.  For each iv we then try to find best biv
   on that we may base the replacement (plus we try to introduce new biv).
   
   The heuristics to decide what to do tries to take register pressure,
   addressing modes and various other issues into account.  */

/* A description of new iv from strength reduction.  */
struct str_red
{
  rtx reg;		/* Register to hold the new induction variable.  */
  rtx equal_to;		/* The equivalent old iv, if any.  */
  struct iv_occurence *eq_occ;
			/* The occurence where the old iv is incremented.  */
  int chosen;		/* Whether to use it or not.  */
  enum machine_mode mode; /* Mode of the induction variable.  */
  struct loop *loop;	/* Loop in that it should occur.  */
  int elimination;	/* If this in fact is a description of elimination
			   of a biv.  */
  struct str_red *forces; /* Elimination may force existence of some biv.  */
  rtx remove_notes;	/* A list of insns in that the note mentioning REG
			   should be removed.  */
  rtx initial_value;	/* Value to that it should be initialized.  */
  rtx step;		/* Step of this iv.  */
  rtx value;		/* The value of iv.  */
  rtx increment_after;	/* The insn after that it should be incremented.  */
  int cost;		/* For incrementing this iv.  */
  struct str_red *next;	/* In list of scheduled reductions.  */
};

/* A description of replacement alternative.  */
struct repl_alt
{
  struct str_red *iv;	/* On that it is based.  */
  rtx seq;		/* The sequence to compute.  */
  rtx replacement;	/* Expression to replace it with (valid only for
			   memory address ivs; sets of ivs emit their
			   result directly into the target register).  */
  rtx pre;		/* The sequence to be emitted in front of loop.  */
  int cost;		/* Of the alternative.  */
  struct repl_alt *next; /* In list of alternatives.  */
};

/* A description of replacement.  */
struct repl
{
  rtx insn;		/* Insn in that to replace.  */
  enum rtx_code type;	/* Type of expression replaced (SET, MEM or
			   comparison).  */
  rtx *loc;		/* The location of replaced value.  */
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
  int initialize_before; /* If the value has to be computed before the loop.  */
  struct fval_repl *next; /* The next replacement in list.  */
};

/* List of iv_occurences.  */
struct iv_occ_list
{
  struct iv_occ_list *next;
  struct iv_occurence *elt;
  void *aux;
};

/* Array containing list of occurences for each insn.  */
struct iv_occ_list **insn_occurences;

#define COST_INFTY 100000
#define MANY_IVS 50
#define ALTER_BY 3

/* Checks whether insn I1 is dominated by I2.  */
#define INSN_DOMINATED_BY_P(I1, I2)					\
  (fast_dominated_by_p (BLOCK_FOR_INSN (I1), BLOCK_FOR_INSN (I2))	\
   && (BLOCK_FOR_INSN (I1) != BLOCK_FOR_INSN (I2)			\
       || DF_INSN_LUID (loop_df, (I2)) < DF_INSN_LUID (loop_df, (I1))))

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
static struct repl *decide_strength_reduction (struct loop *,
					       struct iv_occurence *,
					       struct str_red **,
					       struct repl **);
static struct fval_repl *kill_old_global_iv (struct loop *,
					     struct iv_occurence *,
					     struct fval_repl **);
static void remove_unnecesary_fvrepls (struct str_red *, struct fval_repl **);
static void fixup_preserved_bivs (struct str_red **);
static void add_iv_elimination (struct loop *, struct iv_occurence *,
				struct repl *, struct fval_repl *,
				struct str_red **);
static void remove_incorrect_eliminations (struct loop *, struct str_red **,
					   struct repl *);
static bool elimination_invalid_p (struct loop *, rtx, rtx, rtx *);
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
static void create_biv (struct loops *loops, struct str_red *);
static void replace_final_value (struct fval_repl *);
static void execute_strength_reduction (struct repl *);
static struct iv_occurence *partial_computation_p (struct loop *,
						   struct iv_occurence *);
static struct repl_alt *derive_iv_from (struct repl *, struct str_red *);
static int invariant_in_loop (rtx *, void *);
static bool invariant_displacement_p (struct loop *, rtx);
static struct str_red *make_default_ivs (struct loop *loop);
static void create_bivs_for (struct loop *, struct iv_occurence *,
			     struct str_red **);
static void create_biv_for (struct loop *, struct iv_occurence *,
			    struct str_red *);
static void record_old_biv (struct loops *, struct loop *, int, rtx, rtx,
			    struct str_red **);
static HOST_WIDE_INT greatest_common_divisor (HOST_WIDE_INT, HOST_WIDE_INT);
static bool schedule_repl_for (struct repl *, struct repl_alt *);
static rtx gen_repl_rtx (int, rtx, struct str_red *, enum machine_mode);
static rtx gen_cond_repl_rtx (HOST_WIDE_INT, HOST_WIDE_INT,
			      struct iv_occurence *, struct str_red *, rtx,
			      enum machine_mode, enum rtx_code);
static int repl_mem_addr_cost (struct iv_occurence *, rtx, rtx *, rtx *,
			       struct str_red *);
static int repl_move_cost (struct iv_occurence *, rtx, rtx *);
static int repl_comparison_cost (struct iv_occurence *, HOST_WIDE_INT,
				 rtx, rtx *, rtx *, rtx *, struct str_red *,
				 int);
static bool may_wrap_p (struct loop *, rtx, rtx, enum machine_mode, int);
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
  struct fval_repl **lfvrepl = &actions->repl_final_value;
  struct fval_repl *loop_fvrepls, *afvrepl;
  struct iv_occ_list *nw, *nxt;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "\nStrength reduction:\n\n");

  /* Prepare array of lists of occurences in insns.  */
  insn_occurences = xcalloc (max_uid, sizeof (struct iv_occ_list *));
  for (i = 0; i < loops->num; i++)
    for (step = iv_occurences[i]; step; step = step->sc_next)
      if (step->step != const0_rtx)
	for (base = step->bc_first; base; base = base->bc_next)
	  for (occ = base->oc_first; occ; occ = occ->oc_next)
	    {
	      nw = xmalloc (sizeof (struct iv_occ_list));

	      nw->elt = occ;
	      nw->next = insn_occurences[INSN_UID (occ->insn)];
	      insn_occurences[INSN_UID (occ->insn)] = nw;
	    }

  /* Pass the loop tree in dfs, processing outer loops first (this is important
     in remove_incorrect_eliminations, where we assume that the inner loops
     weren't processed yet.  */
  loop = loops->tree_root->inner;

  while (1)
    {
      sort_iv_lists (&iv_occurences[loop->num]);

      loop_reds = make_default_ivs (loop);
      loop_repls = NULL;
      loop_fvrepls = NULL;

      for (step = iv_occurences[loop->num]; step; step = step->sc_next)
	if (step->step != const0_rtx)
	  for (base = step->bc_first; base; base = base->bc_next)
	    for (occ = base->oc_first; occ; occ = occ->oc_next)
	      create_bivs_for (loop, occ, &loop_reds);

      for (i = 0; i < loop_max_regno; i++)
	if (TEST_BIT (iv_interesting_reg, i))
	  {
	    rtx base, step;
	    iv_split (loop_entry_values[loop->num][i], &base, &step);
	    if (base && GET_CODE (step) == CONST_INT && step != const0_rtx)
	      record_old_biv (loops, loop, i, base, step, &loop_reds);
	  }

      for (step = iv_occurences[loop->num]; step; step = step->sc_next)
	if (step->step != const0_rtx)
	  for (base = step->bc_first; base; base = base->bc_next)
	    for (occ = base->oc_first; occ; occ = occ->oc_next)
	      {
		struct repl *arepl;

		arepl = decide_strength_reduction (loop, occ,
						   &loop_reds, &loop_repls);
		if (!arepl)
		  continue;

		afvrepl = kill_old_global_iv (loop, occ, &loop_fvrepls);
		add_iv_elimination (loop, occ, arepl, afvrepl, &loop_reds);
	      }
      remove_incorrect_eliminations (loop, &loop_reds, loop_repls);

      determine_reductions_to_perform (loop, &loop_reds, loop_repls);
      fixup_preserved_bivs (&loop_reds);
      remove_unnecesary_fvrepls (loop_reds, &loop_fvrepls);

#ifdef AUTO_INC_DEC
      if (loop_repls)
	  schedule_autoinc (loops, loop, loop_reds, loop_repls);
#endif

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

      *lfvrepl = loop_fvrepls;
      while (*lfvrepl)
	lfvrepl = &(*lfvrepl)->next;

      if (loop->inner)
	{
	  loop = loop->inner;
	  continue;
	}

      while (!loop->next
	     && loop->outer)
	loop = loop->outer;
      if (!loop->outer)
	break;

      loop = loop->next;
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
  int size, step;
  rtx last_insn;
  basic_block bb;
  edge e;
  struct df_link *def, *use, *ddef;

  for (; reds; reds = reds->next)
    {
      if (reds->elimination)
	continue;

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
      if (reds->increment_after)
	last_insn = reds->increment_after;
      else
	last_insn = loop->latch->end;

      /* Find candidate for the last mem.  */
      for (repl = repls; repl; repl = repl->next)
	{
	  if (repl->chosen->iv != reds)
	    continue;

	  bb = BLOCK_FOR_INSN (repl->insn);
	  if (!just_once_each_iteration_p (loops, loop, bb))
	    continue;

	  if (INSN_DOMINATED_BY_P (repl->insn, last_insn))
	    continue;

	  if (last_repl
	      && INSN_DOMINATED_BY_P (last_repl->insn, repl->insn))
	    continue;

	  last_repl = repl;
	}
      if (!last_repl
	  || last_repl->type != MEM)
	continue;

      /* Check that size maches.  */
      size = GET_MODE_SIZE (GET_MODE (*last_repl->occ->occurence));
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

	  if (INSN_DOMINATED_BY_P (repl->insn, last_insn))
	    continue;

	  if (INSN_DOMINATED_BY_P (repl->insn, last_repl->insn))
	    break;
	}

      if (repl)
	continue;

      /* We cannot place the increment after the insn, as then the
	 REG_EH_REGION would occur inside a basic block.  We could
	 put it to the start of the next bb, but flow.c is too stupid
	 to find it there.  */
      if (find_reg_note (last_repl->insn, REG_EH_REGION, NULL_RTX))
	continue;

      if (reds->equal_to)
	{
	  /* If this variable already existed, we must also be sure about
	     that we do not change a position of its increment wrto other
	     uses and if it is used outside of loop, also wrto loop exits.
	     ??? The condition on loop exits could be eliminated if the
	     final value can be computed.  */
	  for (def = DF_INSN_DEFS (loop_df, reds->increment_after);
	       def;
	       def = def->next)
	    if (DF_REF_REGNO (def->ref) == REGNO (reds->equal_to))
	      break;

	  for (use = loop_df->regs[REGNO (reds->equal_to)].uses;
	       use;
	       use = use->next)
	    {
	      for (ddef = DF_REF_CHAIN (use->ref); ddef; ddef = ddef->next)
		if (ddef->ref == def->ref)
		  break;

	      if (!ddef)
		continue;

	      if (flow_bb_inside_loop_p (loop, DF_REF_BB (use->ref)))
		{
		  if (DF_REF_INSN (use->ref) != reds->increment_after
		      && INSN_DOMINATED_BY_P (DF_REF_INSN (use->ref),
					      last_repl->insn)
		      && !INSN_DOMINATED_BY_P (DF_REF_INSN (use->ref),
					       reds->increment_after))
		    break;

		  continue;
		}

	      if (BLOCK_FOR_INSN (reds->increment_after)
		  == BLOCK_FOR_INSN (last_repl->insn))
		continue;

	      /* ??? This is pure laziness; we should instead look up the
		 exits other way.  */
	      if (!loop->landing_pad)
		break;

	      for (e = loop->landing_pad->pred; e; e = e->pred_next)
		if (fast_dominated_by_p (e->src,
					 BLOCK_FOR_INSN (last_repl->insn))
		    && !fast_dominated_by_p (e->src,
					     BLOCK_FOR_INSN (reds->increment_after)))
		  break;
	      if (e)
		break;
	    }

	  if (use)
	    continue;
	}

      reds->increment_after = last_repl->insn;
    }
}
#endif

/* Attempts to kill induction variable OCC that would not become dead
   because it is used outside of LOOP.  We may sometimes achieve this
   by computing the final value and assigning it on exit; if we are
   successful, add the description of the operation to list FREPL
   and return it, otherwise return NULL.  */
static struct fval_repl *
kill_old_global_iv (struct loop *loop, struct iv_occurence *occ,
		    struct fval_repl **frepl)
{
  rtx reg;
  struct df_link *def, *use, *ddef;
  basic_block bb = BLOCK_FOR_INSN (occ->insn), preheader;
  int pre_exit;
  rtx val, it;
  struct fval_repl *nw;
  int may_enter;

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
   return NULL;

  if (GET_CODE (*occ->occurence) != SET)
    return NULL;

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
    return NULL;

  preheader = loop_preheader_edge (loop)->src;
  may_enter = bitmap_bit_p (DF_BB_INFO (loop_df, preheader)->rd_out,
			    DF_REF_ID (def->ref));

  /* Unless there is some use of the value outside of loop, it is dead
     anyway, so we have nothing to do.  */
  for (use = loop_df->regs[REGNO (reg)].uses; use; use = use->next)
    {
      for (ddef = DF_REF_CHAIN (use->ref); ddef; ddef = ddef->next)
	if (ddef->ref == def->ref)
	  break;

      if (!ddef)
	continue;

      if (!flow_bb_inside_loop_p (loop, DF_REF_BB (use->ref)))
	break;

      if (!may_enter)
	continue;

      /* Except that it also might be used inside loop, if it passes through
	 entry edge.  */
      if (!INSN_DOMINATED_BY_P (DF_REF_INSN (use->ref), DF_REF_INSN (def->ref)))
	break;
    }
  if (!use)
    return NULL;

  if (pre_exit
      && simplify_gen_relational (NE, SImode, loop->desc.mode, const0_rtx,
				  loop->desc.niter_expr) != const_true_rtx)
    {
      rtx ival = initial_values[loop->num][REGNO (reg)];
      rtx m1 = constm1_rtx;
      rtx fival = substitute_into_expr (occ->value, NULL, &m1,
					gen_iteration (LONG_INT_TYPE),
					SIE_IGNORE_MODE);

      fival = substitute_into_expr (fival, iv_interesting_reg,
				    initial_values[loop->num], NULL,
				    SIE_SIMPLIFY);
      if (!fival
	  || !rtx_equal_p (fival, ival))
	return NULL;
    }

  it = pre_exit
	  ? GEN_BINARY (MINUS, loop->desc.mode,
			loop->desc.niter_expr, const1_rtx)
	  : loop->desc.niter_expr;
  val = substitute_into_expr (occ->value, NULL, &it,
			      gen_iteration (loop->desc.mode),
			      SIE_SIMPLIFY | SIE_IGNORE_MODE);
  if (!val)
    return NULL;

  nw = xmalloc (sizeof (struct fval_repl));
  nw->loop = loop;
  nw->reg = reg;
  nw->value = iv_omit_initial_values (val);
  /* Prefer to compute the value after loop.  While it means we may
     lose a possibility to cse, it saves a register, which should
     pay up usually.  ??? We might be more clever and check the
     cost of computation and estimate on number of free regs.  */
  nw->initialize_before = !invariant_displacement_p (loop, nw->value);
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

  return nw;
}

/* Adds an alternative to eliminate the biv completely to list of alternatives
   to replacement REPL of iv occurence OCC in LOOP.  FVAL_REPL is description
   of final value of the iv if it is stored into it after loops end (so that
   we know if we can consider it to be dead).  A fake "eliminated biv" element
   to form basis for REPL is added to the list of REDS if it is not present
   there already.  */
static void
add_iv_elimination (struct loop *loop, struct iv_occurence *occ,
		    struct repl *repl, struct fval_repl *fval_repl,
		    struct str_red **reds)
{
  rtx reg;
  struct df_link *def, *use, *ddef;
  struct str_red *nw;
  struct repl_alt *alt;
  int may_enter;
  basic_block preheader;
  struct str_red *forces = NULL;

  if (GET_CODE (*occ->occurence) != SET)
    return;
  reg = SET_DEST (*occ->occurence);

  /* We need the definition to be dead after the loop.  */
  if (!fval_repl)
    {
      for (def = DF_INSN_DEFS (loop_df, occ->insn); def; def = def->next)
	if (DF_REF_REGNO (def->ref) == REGNO (reg))
	  break;

      preheader = loop_preheader_edge (loop)->src;
      may_enter = bitmap_bit_p (DF_BB_INFO (loop_df, preheader)->rd_out,
				DF_REF_ID (def->ref));
      for (use = loop_df->regs[REGNO (reg)].uses; use; use = use->next)
	{
	  for (ddef = DF_REF_CHAIN (use->ref); ddef; ddef = ddef->next)
	    if (ddef->ref == def->ref)
	      break;

	  if (!ddef)
	    continue;

	  if (!flow_bb_inside_loop_p (loop, DF_REF_BB (use->ref)))
	    break;
	      
	  if (!may_enter)
	    continue;

	  /* ??? We want to check whether the definition coming from entry
	     edge may reach us.  The answer could be false even if the
	     following conditions are not satisfied (as it could be killed
	     by other defs on remaining paths), but this is slow to check.  */
	  if (!INSN_DOMINATED_BY_P (DF_REF_INSN (use->ref),
				    DF_REF_INSN (def->ref)))
	    break;
	}

      if (use)
	{
	  /* Using the original iv is probably the best choice here.  */
	  for (alt = repl->alts; alt; alt = alt->next)
	    if (alt->iv->equal_to
		&& REGNO (alt->iv->equal_to) == DF_REF_REGNO (def->ref))
	      break;

	  if (!alt)
	    return;

	  forces = alt->iv;
	}
    }

  /* We need all uses inside loop to be covered by some replacement.  Since
     not all replacements are gathered now, suppose it is true for now and
     invalidate the wrong ones later.  */

  /* Check whether there is not the "invalidate biv" red already for this
     register.  */
  for (; *reds; reds = &(*reds)->next)
    if ((*reds)->elimination
	&& (*reds)->reg == reg)
      break;

  if (!*reds)
    {
      nw = xmalloc (sizeof (struct str_red));
      nw->reg = reg;
      nw->equal_to = NULL_RTX;
      nw->eq_occ = NULL;
      nw->chosen = false;
      nw->mode = VOIDmode;
      nw->loop = loop;
      nw->elimination = true;
      nw->forces = forces;
      nw->remove_notes = NULL_RTX;
      nw->initial_value = NULL_RTX;
      nw->increment_after = NULL_RTX;
      nw->step = NULL_RTX;
      nw->value = NULL_RTX;
      /* Ensure that it is always chosen if possible.  */
      nw->cost = -COST_INFTY;
      nw->next = NULL;
      *reds = nw;
    }
  else
    nw = *reds;

  alt = xmalloc (sizeof (struct repl_alt));
  alt->iv = nw;
  alt->seq = NULL_RTX;
  alt->replacement = NULL_RTX;
  alt->cost = -COST_INFTY;
  alt->next = NULL;
  enter_alt (&repl->alts, alt);
}

/* Checks the eliminations for correctness (i.e. verify that all uses
   of the appropriate def inside LOOP are covered by one of REPLS),
   and remove the incorrect ones from list of REDS.  */
static void
remove_incorrect_eliminations (struct loop *loop, struct str_red **reds,
			       struct repl *repls)
{
  struct str_red *red;
  struct repl_alt *alt;
  struct repl *repl;

  for (repl = repls; repl; repl = repl->next)
    {
      /* If there is an elimination, it always must be the first one in the
	 list.  */
      if (!repl->alts->iv->elimination
	  || repl->alts->iv->chosen)
	continue;

      repl->alts->iv->chosen |=
	      elimination_invalid_p (loop, repl->insn, *repl->loc,
				     &repl->alts->iv->remove_notes);
    }

  for (repl = repls; repl; repl = repl->next)
    {
      if (!repl->alts->iv->elimination
	  || !repl->alts->iv->chosen)
	continue;

      /* The elimination is invalid, remove it.  */
      alt = repl->alts;
      repl->alts = alt->next;
      free (alt);
    }

  while (*reds)
    {
      if (!(*reds)->chosen)
	{
	  reds = &(*reds)->next;
	  continue;
	}

      /* The elimination is invalidated, remove it.  */
      red = *reds;
      if (!red->elimination)
	abort ();
      *reds = red->next;
      free_INSN_LIST_list (&red->remove_notes);
      free (red);
    }
}

/* Checks whether elimination of definition of REG in INSN inside LOOP is
   invalid.  */
static bool
elimination_invalid_p (struct loop *loop, rtx insn, rtx reg,
		       rtx *notes_to_remove)
{
  struct df_link *def, *use, *ddef;
  struct iv_occ_list *occ_l, *prev, *next;
  rtx note, use_insn;
  int uid;
  int ret = false;

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Verifying elimination of\n");
      print_rtl_single (rtl_dump_file, insn);
    }

  for (def = DF_INSN_DEFS (loop_df, insn); def; def = def->next)
    if (DF_REF_REGNO (def->ref) == REGNO (reg))
      break;

  for (use = loop_df->regs[REGNO (reg)].uses; use; use = use->next)
    {
      if (!flow_bb_inside_loop_p (loop, DF_REF_BB (use->ref)))
	continue;

      for (ddef = DF_REF_CHAIN (use->ref); ddef; ddef = ddef->next)
	if (ddef->ref == def->ref)
	  break;

      if (!ddef)
	continue;

      /* Check that all uses in the body are covered by some replacement.  */
      use_insn = DF_REF_INSN (use->ref);
      uid = INSN_UID (use_insn);
      prev = NULL;
      for (occ_l = insn_occurences[uid]; occ_l; occ_l = next)
	{
	  if (reg_mentioned_p (reg, *occ_l->elt->occurence)
	      && !occ_l->elt->aux)
	    ret = true;

	  occ_l->aux = *occ_l->elt->occurence;
	  *occ_l->elt->occurence = NULL_RTX;

	  /* We reverse the list in progress, so that the changes are undone
	     in the correct order below.  */
	  next = occ_l->next;
	  occ_l->next = prev;
	  prev = occ_l;
	}
      insn_occurences[uid] = prev;

      if (reg_mentioned_p (reg, PATTERN (use_insn)))
	ret = true;
      for (occ_l = insn_occurences[uid]; occ_l; occ_l = occ_l->next)
	*occ_l->elt->occurence = occ_l->aux;
      if (ret)
	{
	  if (rtl_dump_file)
	    {
	      fprintf (rtl_dump_file, "Failed in\n");
	      print_rtl_single (rtl_dump_file, DF_REF_INSN (use->ref));
	      fprintf (rtl_dump_file, "\n");
	    }
	  return true;
	}

      /* If the insn pattern itself is ok but there is a reference from
	 equiv/equal note, schedule it for removal.  */
      for (; *notes_to_remove; notes_to_remove = &XEXP (*notes_to_remove, 1))
	if (XEXP (*notes_to_remove, 0) == use_insn)
	  break;
      if (!*notes_to_remove)
	{
	  note = find_reg_equal_equiv_note (DF_REF_INSN (use->ref));
	  if (note && reg_mentioned_p (reg, note))
	    *notes_to_remove = alloc_INSN_LIST (DF_REF_INSN (use->ref),
						NULL_RTX);
	}
    }

  /* Additionally, the register might be used in description of simple loop
     properties of inner loops or in movables; then we also should not eliminate
     it, but I believe both of these cases are already handled by the above
     checks.  Note that the order in that we pass the loops (outer to inner
     is important here, otherwise the occurences in inner loops could also
     have defined aux fields).  */
  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Is ok\n\n");
  return false;
}

/* Actually perform the reductions scheduled in ACTIONS.  */
void
execute_strength_reductions (struct loops *loops, struct ivopt_actions *actions)
{
  struct str_red *next, *reds;
  struct repl *repl, *rnext;
  struct fval_repl *frepl, *fnext;

  /* Ensure that the position to that we want to be placed is not removed.  */
  for (reds = actions->ivs; reds; reds = reds->next)
    if (reds->increment_after)
      reds->increment_after =
	      emit_note_after (NOTE_INSN_DELETED, reds->increment_after);

  for (repl = actions->replacements; repl; repl = rnext)
    {
      rnext = repl->next;
      execute_strength_reduction (repl);
      free (repl->chosen);
      free (repl);
    }

  for (reds = actions->ivs; reds; reds = next)
    {
      create_biv (loops, reds);
      next = reds->next;
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
   NULL if the occurence is not suitable, description of replacement added
   to REPLS otherwise.  */
static struct repl *
decide_strength_reduction (struct loop *loop, struct iv_occurence *occ,
			   struct str_red **reds, struct repl **repls)
{
  rtx step = occ->base_class->step_class->step;
  struct repl_alt *alt;
  struct repl *nw;

  if (GET_CODE (step) != CONST_INT
      || step == const0_rtx)
    return NULL;

  /* ??? Add handling of these.  */
  if (occ->real_mode != occ->extended_mode)
    return NULL;

#if 0
  /* While this would be correct and it may decrease the number of found
     ivs signigicantly, the problem is that the ignored occurences prevent
     induction variable elimination and that they are hard to handle there.  */
  /* If this is a set occurence and there is exactly one use in other set,
     ignore it (we will optimize only the final result).  */
  if (partial_computation_p (loop, occ))
    return NULL;
#endif

  nw = xmalloc (sizeof (struct repl));
  nw->insn = occ->insn;
  nw->occ = occ;
  nw->chosen = NULL;
  nw->type = GET_CODE (*occ->occurence);
  switch (nw->type)
    {
    case SET:
      nw->loc = &SET_DEST (*occ->occurence);
      break;
    case MEM:
      nw->loc = &XEXP (*occ->occurence, 0);
      break;
    default:
      nw->loc = occ->occurence;
    }
  nw->cost = computation_cost (loop, occ);
  nw->alts = NULL;

  /* Try to derive occurence from all possibilities.  */
  for (; *reds; reds = &(*reds)->next)
    {
      if ((*reds)->elimination)
	continue;

      alt = derive_iv_from (nw, *reds);

      if (!alt)
	continue;

      enter_alt (&nw->alts, alt);
    }

  if (!nw->alts)
    {
      free (nw);
      return NULL;
    }

  nw->occ->aux = nw;
  nw->next = *repls;
  *repls = nw;
  return nw;
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
   of a chain of computations.  If so, return the single occurence in that
   it is used.  */
static struct iv_occurence *
partial_computation_p (struct loop *loop, struct iv_occurence *occ)
{
  rtx reg, tgt, occ_rtx;
  struct ref *tg;
  struct iv_occ_list *occl, *soccl;
  struct iv_occurence *ret;

  if (GET_CODE (*occ->occurence) != SET)
    return NULL;

  reg = SET_DEST (*occ->occurence);
  tg = find_single_def_use (reg, loop, occ->insn);
  if (!tg)
    return NULL;
  tgt = DF_REF_INSN (tg);

  soccl = NULL;
  for (occl = insn_occurences[INSN_UID (tgt)]; occl; occl = occl->next)
    if (reg_mentioned_p (reg, *occl->elt->occurence))
      {
	if (soccl)
	  return NULL;
	soccl = occl;
      }

  if (!soccl)
    return NULL;

  /* We have the single occurence.  We need to check that there is no use
     of the reg outside of it.  */
  ret = soccl->elt;
  occ_rtx = *soccl->elt->occurence;
  *soccl->elt->occurence = NULL_RTX;
  if (reg_mentioned_p (reg, PATTERN (soccl->elt->insn)))
    ret = NULL;
  *soccl->elt->occurence = occ_rtx;

  return ret;
}

/* Returns a cost of computing OCC, including partial computations before it.
   LOOP is the loop where OCC occurs.
   
   It might seem that the code is slightly incorrect, as it may count one
   computation multiple times if used on more than one place.  This is not true,
   because we only continue back if partial_computation_p.  Using aux as marker
   is probably useless and is here just for sure.  */
static int
computation_cost (struct loop *loop, struct iv_occurence *occ)
{
  int cost, uid;
  struct df_link *use, *def;
  struct iv_occ_list *occl;
  void *old_aux;

  if (occ->aux == &occ->aux)
    return 0;

  old_aux = occ->aux;
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

  occ->aux = old_aux;
  return cost;
}

/* Tries to create replacement alternative for replacement REPL derived from
   biv RED.  */
static struct repl_alt *
derive_iv_from (struct repl *repl, struct str_red *red)
{
  struct repl_alt *alt = xmalloc (sizeof (struct repl_alt));

  alt->iv = red;
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

/* Returns greatest common divisor of A and B.  */
static HOST_WIDE_INT
greatest_common_divisor (HOST_WIDE_INT a, HOST_WIDE_INT b)
{
  HOST_WIDE_INT tmp;

  if (a < 0)
    a = -a;
  if (b < 0)
    b = -b;
  if (a < b)
    {
      tmp = a;
      a = b;
      b = tmp;
    }

  while (b)
    {
      tmp = b;
      b = a % b;
      a = tmp;
    }

  return a;
}

/* Fills in data for replacement alternative ALT of replacement REPL.  Returns
   false if not succesful.  */
static bool
schedule_repl_for (struct repl *repl, struct repl_alt *alt)
{
  int really = alt->iv->reg != NULL_RTX;
  /* ??? This is slightly wrong, in case the variable is unsigned and step
     is too great to fit into HOST_WIDE_INT.  */
  HOST_WIDE_INT occ_step = INTVAL (repl->occ->base_class->step_class->step);
  HOST_WIDE_INT red_step = INTVAL (alt->iv->step);
  HOST_WIDE_INT gcd, occ_step1 = 0, red_step1;
  rtx r, v, val, repl_rtx, delta, seq, addr = NULL, pre = NULL;
  int repl_cost = COST_INFTY;
  int n_regs = reg_rtx_no;
  rtx expr = *repl->occ->occurence, set, set1;
  enum rtx_code code = GET_CODE (expr), ccode;
  struct ref *use;
  int scale, swap = false;

  /* ??? We could support extending ivs, if we know that there is no
     overflow.  */
  if (GET_MODE_BITSIZE (alt->iv->mode)
      < GET_MODE_BITSIZE (repl->occ->extended_mode))
    return false;

  if (!really)
    alt->iv->reg = gen_reg_rtx (alt->iv->mode);

  if (code == MEM || code == SET)
    {
      /* If this is the original iv, we may "replace" it at no extra cost
	 (cost of addition is paid in cost of biv).  */
      if (code == SET
	  && SET_DEST (expr) == alt->iv->equal_to)
	{
	  seq = NULL_RTX;
	  pre = NULL_RTX;
	  addr = NULL_RTX;
	  repl_cost = 0;
	  goto succeed;
	}

      /* This combination makes % and / to abort, so ignore it.  */
      if (occ_step == ((HOST_WIDE_INT) 1 << (HOST_BITS_PER_WIDE_INT - 1))
	  && red_step == -1)
	goto fail;

      if (occ_step % red_step != 0)
	goto fail;

      scale = occ_step / red_step;

      if (!alt->iv->increment_after
	  || !INSN_DOMINATED_BY_P (repl->insn, alt->iv->increment_after))
	val = alt->iv->value;
      else
	val = simplify_alg_expr (GEN_BINARY (OP_PLUS, alt->iv->mode,
					     COPY_EXPR (alt->iv->value),
					     alt->iv->step));

      val = scale == 1
	      ? val
	      : simplify_alg_expr (GEN_BINARY (OP_MULT, alt->iv->mode,
					       COPY_EXPR (val),
					       CONST_INT_EXPR (scale)));
      if (alt->iv->mode != repl->occ->extended_mode)
	val = GEN_SHORTEN (repl->occ->extended_mode, val);
      delta = iv_omit_initial_values (
		simplify_alg_expr (GEN_BINARY (OP_MINUS,
					       repl->occ->extended_mode,
					       COPY_EXPR (repl->occ->value),
					       COPY_EXPR (val))));

      if (!invariant_displacement_p (alt->iv->loop, delta))
	goto fail;

      repl_rtx = gen_repl_rtx (scale, delta, alt->iv, repl->occ->extended_mode);
    }
  else
    {
      /* We try to express the condition in the other variable.  */

      v = XEXP (expr, repl->occ->arg);

      /* We may always use the original iv.  */
      if (v == alt->iv->equal_to)
	{
	  seq = NULL_RTX;
	  pre = NULL_RTX;
	  addr = expr;
	  repl_cost = 0;
	  goto succeed;
	}

      /* Determine what is the result used for.  */
      if (code == COMPARE)
	{
	  set = single_set (repl->insn);
	  if (!set)
	    goto fail;
	  /* ??? Support cc0.  */
	  if (!REG_P (SET_DEST (set)))
	    goto fail;

	  use = find_single_def_use (SET_DEST (set), alt->iv->loop, repl->insn);
	  if (!use)
	    goto fail;

	  set1 = single_set (DF_REF_INSN (use));
	  if (!set1)
	    goto fail;

	  if (GET_CODE (SET_SRC (set1)) == IF_THEN_ELSE)
	    set1 = XEXP (SET_SRC (set1), 0);
	  else
	    set1 = SET_SRC (set1);

	  if (!reg_mentioned_p (SET_DEST (set), set1))
	    goto fail;

	  ccode = GET_CODE (set1);
	}
      else
	ccode = code;

      r = XEXP (expr, 1 - repl->occ->arg);
      if (!invariant_displacement_p (alt->iv->loop, r)
	  || GET_CODE (v) != REG)
	goto fail;

      gcd = greatest_common_divisor (occ_step, red_step);
      occ_step1 = occ_step / gcd;
      red_step1 = red_step / gcd;
      if (red_step1 < 0)
	{
	  occ_step1 = -occ_step1;
	  red_step1 = -red_step1;
	}
      /* Prefer occ_step1 to be positive, as we really multiply by
	 it and multiplication by negative value may be a bit slower.
	 We however must be able to reverse the comparison if it makes
	 red_step1 to be negative.  ??? Even if it is compare, it should
	 be possible to do.  */
      if (occ_step1 < 0
	  && (ccode == EQ || ccode == NE || code != COMPARE))
	{
	  occ_step1 = -occ_step1;
	  red_step1 = -red_step1;
	}
      swap = red_step1 < 0;
      repl_rtx = gen_cond_repl_rtx (occ_step1, red_step1, repl->occ, alt->iv, r,
				    repl->occ->extended_mode, ccode);
      if (!repl_rtx)
	goto fail;
    }

  switch (code)
    {
      case MEM:
	repl_cost = repl_mem_addr_cost (repl->occ, repl_rtx, &seq, &addr,
					alt->iv);
	break;

      case SET:
	repl_cost = repl_move_cost (repl->occ, repl_rtx, &seq);
	break;

      default:
	if (!COMPARISON_OP_P (code))
	  abort ();
	/* Fallthru.  */
      case COMPARE:
	repl_cost = repl_comparison_cost (repl->occ, occ_step1, repl_rtx,
					  &seq, &addr, &pre, alt->iv, swap);
	break;
    }

  /* If we are only computing costs, cancel the new pseudos created.  */
succeed: ;
fail: ;
  if (!really)
    {
      reg_rtx_no = n_regs;
      alt->iv->reg = NULL_RTX;
      seq = NULL_RTX;
      addr = NULL_RTX;
      pre = NULL_RTX;
    }

  if (repl_cost == COST_INFTY)
    return false;

  alt->seq = seq;
  alt->replacement = addr;
  alt->pre = pre;
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
}

/* Returns estimate on cost of computing SEQ.  */
static int
seq_cost (rtx seq)
{
  int cost = 0;
  rtx set;

  for (; seq; seq = NEXT_INSN (seq))
    {
      set = single_set (seq);
      if (set)
	cost += rtx_cost (set, SET);
      else
	cost++;
    }

  return cost;
}

/* A cost to replace addr of occurence OCC by REPL (derived from IV).
   Sequence SEQ is emitted to do so and addr is replaced by ADDR.
   Cost of the replacement is returned.  */
static int
repl_mem_addr_cost (struct iv_occurence *occ, rtx repl, rtx *seq, rtx *addr,
		    struct str_red *iv ATTRIBUTE_UNUSED)
{
  int cost = 0;
  rtx old_occ, reg, tmp;
  int fail;
#ifdef AUTO_INC_DEC
  int size = GET_MODE_SIZE (GET_MODE (*occ->occurence));
  int auto_ben = (iv->cost + 1) / 2;
#endif

  start_sequence ();
  *addr = memory_address (GET_MODE (*occ->occurence), repl);
  *seq = get_insns ();
  end_sequence ();

  old_occ = XEXP (*occ->occurence, 0);
  XEXP (*occ->occurence, 0) = *addr;
  fail = recog (PATTERN (occ->insn), occ->insn, NULL) < 0;
  XEXP (*occ->occurence, 0) = old_occ;
  if (fail)
    {
      reg = gen_reg_rtx (Pmode);
      start_sequence ();
      emit_insn (*seq);
      tmp = force_operand (*addr, reg);
      if (tmp != reg)
	emit_move_insn (reg, tmp);
      *seq = get_insns ();
      *addr = reg;
      *seq = get_insns ();
      end_sequence ();
  
      old_occ = XEXP (*occ->occurence, 0);
      XEXP (*occ->occurence, 0) = *addr;
      fail = recog (PATTERN (occ->insn), occ->insn, NULL) < 0;
      XEXP (*occ->occurence, 0) = old_occ;

      if (fail)
	return COST_INFTY;
    }

  cost += seq_cost (*seq);
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
  rtx tmp, dest = SET_DEST (*occ->occurence);

  /* If the insn is a complicated one, ignore it.  */
  if (!single_set (occ->insn))
    return COST_INFTY;

  start_sequence ();
  tmp = force_operand (repl, dest);
  if (tmp != dest)
    emit_move_insn (dest, tmp);
  *seq = get_insns ();
  end_sequence ();

  cost += seq_cost (*seq);

  return cost;
}

/* A cost to replace comparison in occurence OCC by one based on variable RED
   multiplied OCC_STEP times and compared with REPL; the condition is swapped
   if SWAP.  Sequence SEQ is emitted to do so, condition is replaced with COMP,
   INIT is computed before loop.  Cost of the replacement is returned.  */
static int
repl_comparison_cost (struct iv_occurence *occ, HOST_WIDE_INT occ_step,
		      rtx repl, rtx *seq, rtx *comp, rtx *init,
		      struct str_red *red, int swap)
{
  int cost = 0, cst, niter;
  rtx l, ll, tmp, r = NULL, *rplace, old_rplace;
  int unchanging, must_subtract = 0;
  int right = occ->arg == 0;

  if (occ_step == 1 && occ->extended_mode == red->mode)
    {
      l = red->reg;
      *seq = NULL_RTX;
    }
  else
    {
      start_sequence ();
      l = gen_reg_rtx (occ->extended_mode);
      if (occ->extended_mode == red->mode)
	tmp = force_operand (simplify_gen_binary (MULT, red->mode,
						  red->reg, GEN_INT (occ_step)),
			     l);
      else
	{
	  ll = gen_reg_rtx (red->mode);
	  tmp = force_operand (simplify_gen_binary (MULT, red->mode,
						    red->reg, GEN_INT (occ_step)),
			       ll);
	  if (tmp != ll)
	    emit_move_insn (ll, tmp);
	  tmp = simplify_gen_subreg (occ->extended_mode, ll, red->mode, 0);
	}
      if (tmp != l)
	emit_move_insn (l, tmp);
      *seq = get_insns ();
      end_sequence ();

      cost += seq_cost (*seq);
    }

  r = NULL;
  unchanging = invariant_displacement_p (red->loop, repl);
  if (unchanging)
    {
      /* Check whether we may just put it into the condition.  */
      rplace = &XEXP (*occ->occurence, right ? 1 : 0);
      old_rplace = *rplace;
      *rplace = repl;

      if (recog (PATTERN (occ->insn), occ->insn, NULL) >= 0)
	{
	  *init = NULL_RTX;
	  r = repl;
	}
      *rplace = old_rplace;
    }

  if (!r)
    {
      r = gen_reg_rtx (occ->extended_mode);

      /* Check whether we may replace the right side by a register, ...  */
      rplace = &XEXP (*occ->occurence, right ? 1 : 0);
      old_rplace = *rplace;
      *rplace = r;
      must_subtract = recog (PATTERN (occ->insn), occ->insn, NULL) < 0;

      if (must_subtract)
	{
	  /* ... or at least subtract the rhs from lhs.  */
	  *rplace = const0_rtx;
	  if (recog (PATTERN (occ->insn), occ->insn, NULL) < 0)
	    return COST_INFTY;
	}
      *rplace = old_rplace;

      cst = COST_INFTY;

      start_sequence ();
      tmp = force_operand (repl, r);
      if (tmp != r)
	emit_move_insn (r, tmp);
      tmp = get_insns ();
      end_sequence ();
      cst = seq_cost (tmp);

      if (unchanging && cst <= PARAM_VALUE (STR_SPILL_COST))
	{
	  cost += cst;
	  start_sequence ();
	  emit_insn (*seq);
	  emit_insn (tmp);
	  *seq = get_insns ();
	  end_sequence ();
	  *init = NULL_RTX;
	}
      else
	{
	  *init = tmp;
	  niter = expected_loop_iterations (red->loop);
	  if (!niter)
	    niter = 1;
	  cost += cst / niter + PARAM_VALUE (STR_REG_WEIGHT);
	}
    }

  if (must_subtract)
    {
      start_sequence ();
      ll = gen_reg_rtx (occ->extended_mode);
      tmp = force_operand (GEN_BINARY (OP_MINUS, occ->extended_mode,
				       l, r), ll);
      if (tmp != ll)
	emit_move_insn (ll, tmp);
      tmp = get_insns ();
      end_sequence ();
      cost += seq_cost (tmp);

      start_sequence ();
      emit_insn (*seq);
      emit_insn (tmp);
      *seq = get_insns ();
      end_sequence ();

      l = ll;
      r = const0_rtx;
    }
  *comp = copy_rtx (*occ->occurence);
  if (right)
    {
      XEXP (*comp, 0) = l;
      XEXP (*comp, 1) = r;
    }
  else
    {
      XEXP (*comp, 0) = r;
      XEXP (*comp, 1) = l;
    }
  if (swap && GET_CODE (*comp) != COMPARE)
    PUT_CODE (*comp, swap_condition (GET_CODE (*comp)));

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
    tmp = GEN_BINARY (OP_MULT, red->mode, tmp, GEN_INT (scale));
  if (mode != red->mode)
    tmp = simplify_alg_expr_shorten (tmp, red->mode, mode);
  if (delta != const0_rtx)
    tmp = simplify_gen_binary (OP_PLUS, mode, tmp, delta);

  return tmp;
}

/* Generates rtx for constant argument of comparison of eliminated induction
   variable.  OCC_STEP is step of the eliminated variable, RED_STEP of the
   variable used to eliminate it (divided by their gcd, so that they are
   coprime).  OCC_BASE is base of the eliminated iv.  RED is the replacing biv.
   R is the original constant argument.  MODE is the mode of the eliminated iv.
   COND is the condition's code.
   
   ??? For now this only gaves sane results if bases of ivs and R are
   const_ints.  We should enhance this by allowing it to notice things
   like that the iv is used to address an array, and therefore it cannot
   wrap (we of course need to know that this usage for addressing is
   done in every iteration).  */
static rtx
gen_cond_repl_rtx (HOST_WIDE_INT occ_step, HOST_WIDE_INT red_step,
		   struct iv_occurence *occ, struct str_red *red, rtx r,
		   enum machine_mode mode, enum rtx_code cond)
{
  rtx tmp;
  rtx tmp1;
  rtx occ_base = iv_omit_initial_values (COPY_EXPR (occ->local_base));
  int sign;
  rtx mmin, mmax;
  rtx a, b, c, l, bound;
  int grows;
  rtx ival;

  /* For now; we may try to handle this later.  */
  if (mode != red->mode)
    return NULL_RTX;

  
  if (!red->increment_after
      || !INSN_DOMINATED_BY_P (occ->insn, red->increment_after))
    ival = COPY_EXPR (red->initial_value);
  else
    ival = simplify_alg_expr (GEN_BINARY (OP_PLUS, red->mode,
					  COPY_EXPR (red->initial_value),
					  red->step));

  /* If the condition is equality comparison and it is only multiplied by
     an odd number, the final equation is equivalent with the original one
     as everything is just an arithmetics mod power of 2.  Otherwise we must
     be more careful.  */
  if (red_step % 2 != 1 || (cond != NE && cond != EQ))
    {
      switch (cond)
	{
	case LEU:
	case LTU:
	case GEU:
	case GTU:
	case EQ:
	case NE:
	  sign = false;
	  break;

	case LE:
	case LT:
	case GE:
	case GT:
	  sign = true;
	  break;

	default:
	  return NULL_RTX;
	}
      get_mode_bounds (mode, sign, &mmin, &mmax);

      /* If the original condition uses overflows, things would be complicated.  */
      if (may_wrap_p (red->loop, occ->local_base,
		      occ->base_class->step_class->step,
		      mode, sign))
	return NULL_RTX;

      /* And if the replacement iv wraps, things would not be easy, either.  */
      if (may_wrap_p (red->loop, ival, red->step, mode, sign))
	return NULL_RTX;

      if (sign)
	grows = simplify_gen_relational (GT, VOIDmode, mode,
					 occ->base_class->step_class->step,
					 const0_rtx) == const_true_rtx;
      else
	{
	  tmp = simplify_gen_binary (PLUS, mode,
				     occ->base_class->step_class->step,
				     occ->base_class->step_class->step);
	  grows = simplify_gen_relational (GTU, VOIDmode, mode,
					   tmp,
					   occ->base_class->step_class->step)
		  == const_true_rtx;
	}
	
      if (grows)
	{    
	  /* We have an equation a * i + b cmp c.  Verify that it is equivalent
	     to ai cmp c - b.  */

	  a = occ->base_class->step_class->step;
	  b = occ->local_base;
	  c = r;

	  /* We check that b <= c.  This is not exactly required, but it should
	     be true in any interesting case.  */
	  if (simplify_gen_relational (sign ? LE : LEU, VOIDmode, mode,
				       b, c) == const_true_rtx)
	    ;
	  else if (iv_simplify_using_branches (AND,
			alloc_EXPR_LIST (0,
					 gen_rtx_fmt_ee (sign ? LE : LEU,
							 VOIDmode,
							 b, c),
					 NULL_RTX),
			red->loop) == NULL_RTX)
	    ;
	  else
	    return NULL_RTX;

	  /* If b is negative, we must check that c - b does not overflow.
	     It is only easy to do if b is const_int.  */
	  bound = simplify_gen_binary (PLUS, mode, mmax, b);
	  if (sign
	      && simplify_gen_relational (LT, VOIDmode, mode,
					  b, const0_rtx) != const0_rtx
	      && (GET_CODE (b) != CONST_INT
		  || simplify_gen_relational (LE, VOIDmode, mode,
					      c, bound) == const0_rtx))
	    return NULL_RTX;
	  c = simplify_gen_binary (MINUS, mode, c, b);

	  if (red_step != 1)
	    {
	      /* Verify that it is equivalent to
		 red_step * a * i cmp red_step * (c - b).  */

	      if (GET_CODE (c) != CONST_INT)
		return NULL_RTX;

	      bound = simplify_gen_binary (sign ? DIV : UDIV, mode,
					   mmax, GEN_INT (red_step));
	      if (simplify_gen_relational (sign ? LE : LEU, VOIDmode, mode,
					   c, bound) != const_true_rtx)
		return NULL_RTX;
	      bound = simplify_gen_binary (DIV, mode,
					   mmin, GEN_INT (red_step));
	      if (sign
		  && simplify_gen_relational (GE, VOIDmode, mode,
					      c, bound) != const_true_rtx)
		return NULL_RTX;

	      a = simplify_gen_binary (MULT, mode, a, GEN_INT (red_step));
	      c = simplify_gen_binary (MULT, mode, c, GEN_INT (red_step));
	      if (may_wrap_p (red->loop, const0_rtx, a, mode, sign))
		return NULL_RTX;
	    }

	  l = ival;
	  if (occ_step != 1)
	    {
	      bound = simplify_gen_binary (sign ? DIV : UDIV, mode,
					   mmax, GEN_INT (occ_step));
	      if (simplify_gen_relational (sign ? LE : LEU, VOIDmode, mode,
					   l, bound) != const_true_rtx)
		return NULL_RTX;
	      bound = simplify_gen_binary (DIV, mode,
					   mmin, GEN_INT (occ_step));
	      if (sign
		  && simplify_gen_relational (GE, VOIDmode, mode,
					      l, bound) != const_true_rtx)
		return NULL_RTX;
	      l = simplify_gen_binary (MULT, mode, l, GEN_INT (occ_step));
	    }

	  /* Check that adding l to both sides of the equation does not spoil
	     anything.  ??? Here the code to verify the conditions is
	     particularily strict and eliminates too many interesting cases.  */
	  if (GET_CODE (l) != CONST_INT)
	    return NULL_RTX;
	  if (l != const0_rtx)
	    {
	      if (!sign
		  || simplify_gen_relational (GT, VOIDmode, mode,
					      l, const0_rtx) == const_true_rtx)
		{
		  bound = simplify_gen_binary (MINUS, mode, mmax, l);
		  if (simplify_gen_relational (sign ? LE : LEU, VOIDmode, mode,
					       c, bound) != const_true_rtx)
		    return NULL_RTX;
		}

	      if (sign
		  && simplify_gen_relational (LT, VOIDmode, mode,
					      l, const0_rtx) == const_true_rtx)
		{
		  bound = simplify_gen_binary (MINUS, mode,
					       mmin, GEN_INT (occ_step));
		  if (sign
		      && simplify_gen_relational (GE, VOIDmode, mode,
						  c, bound) != const_true_rtx)
		    return NULL_RTX;
		}
	    }
	}
      else
	{
	  /* ??? Add handling of this case.  */
	  return NULL_RTX;
	}
    }

  tmp = GEN_BINARY (OP_MINUS, mode,
		    COPY_EXPR (r), COPY_EXPR (occ_base));
  tmp = GEN_BINARY (OP_MULT, mode, tmp, GEN_INT (red_step));
  tmp1 = GEN_BINARY (OP_MULT, mode, ival, GEN_INT (occ_step)); 
  tmp = GEN_BINARY (OP_PLUS, mode, tmp, tmp1);

  return simplify_alg_expr (tmp);
}

/* Checks whether an induction variable with STEP and BASE iterating in MODE
   (signed if SIGN) in LOOP may overflow.  ??? We should use knowledge
   about what ivs are used to address memory arrays here.  */
static bool
may_wrap_p (struct loop *loop, rtx base, rtx step, enum machine_mode mode,
	    int sign)
{
  rtx mmin, mmax, imax, tmp, bound;
  int grows;

  if (GET_CODE (base) != CONST_INT)
    return true;

  get_mode_bounds (mode, sign, &mmin, &mmax);  

  if (!loop->simple
      || loop->desc.assumptions
      || loop->desc.infinite != const0_rtx)
    return true;
  imax = GEN_INT (loop_iterations_max (loop));

  
  if (sign)
    grows = simplify_gen_relational (GT, VOIDmode, mode,
				     step, const0_rtx) == const_true_rtx;
  else
    {
      tmp = simplify_gen_binary (PLUS, mode, step, step);
      grows = simplify_gen_relational (GTU, VOIDmode, mode,
				       tmp, step) == const_true_rtx;
    }

  if (grows)
    tmp = simplify_gen_binary (MINUS, mode, mmax, base);
  else
    tmp = simplify_gen_binary (MINUS, mode, base, mmin);
  bound = simplify_gen_binary (UDIV, mode, tmp, step);
  return simplify_gen_relational (LEU, VOIDmode, mode,
				  imax, bound) != const_true_rtx;
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
  red->equal_to = NULL_RTX;
  red->eq_occ = NULL;
  red->elimination = false;
  red->forces = NULL;
  red->remove_notes = NULL_RTX;
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
  red->increment_after = NULL_RTX;
  red->value = simplify_alg_expr (tmp);
  if (!red->value)
    abort ();
}

/* Creates bivs that are suitable as base for occurence OCC in LOOP and
   adds them to the list REDS.  */
static void
create_bivs_for (struct loop *loop, struct iv_occurence *occ,
		 struct str_red **reds)
{
  struct str_red *selfs[3];
  int i;

  /* ??? Add handling of these.  */
  if (occ->real_mode != occ->extended_mode)
    return;

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

  for (; *reds; reds = &(*reds)->next)
    {
      for (i = 0; i < 3; i++)
	if (selfs[i] && iv_equal_p (*reds, selfs[i]))
	  {
	    free (selfs[i]);
	    selfs[i] = NULL;
	  }
    }

  for (i = 0; i < 3; i++)
    if (selfs[i])
      { 
	*reds = selfs[i];
	reds = &selfs[i]->next;
      }
}

/* Adds a biv equivalent to register REGNO with BASE and STEP in LOOP to list
   of REDS, if it is simple enough.  */
static void
record_old_biv (struct loops *loops, struct loop *loop, int regno,
		rtx base, rtx step, struct str_red **reds)
{
  struct df_link *def, *adef = NULL;
  int n_regs = reg_rtx_no;
  struct str_red *red;
  rtx tmp;
  struct iv_occ_list *occ_l;

  /* ??? We only manage bivs set on exactly one place for now.  */

  for (def = loop_df->regs[regno].defs; def; def = def->next)
    {
      if (!flow_bb_inside_loop_p (loop, DF_REF_BB (def->ref)))
	continue;
      if (adef)
	return;

      adef = def;
    }

  if (!adef
      || !just_once_each_iteration_p (loops, loop, DF_REF_BB (adef->ref)))
    abort ();

  red = xmalloc (sizeof (struct str_red));
  red->initial_value = regno_reg_rtx[regno];
  red->increment_after = DF_REF_INSN (adef->ref);
  red->mode = GET_MODE (regno_reg_rtx[regno]);
  red->reg = NULL_RTX;
  red->equal_to = regno_reg_rtx[regno];
  for (occ_l = insn_occurences[INSN_UID (DF_REF_INSN (adef->ref))];
       occ_l;
       occ_l = occ_l->next)
    if (GET_CODE (*occ_l->elt->occurence) == SET
	&& SET_DEST (*occ_l->elt->occurence) == red->equal_to)
      break;
  if (!occ_l)
    abort ();
  red->eq_occ = occ_l->elt;
  red->elimination = false;
  red->forces = NULL;
  red->remove_notes = NULL_RTX;
  red->chosen = false;
  red->loop = loop;
  red->step = step;

  tmp = gen_reg_rtx (red->mode);
  tmp = GEN_BINARY (OP_PLUS, red->mode, tmp, red->step);
  red->cost = rtx_cost (tmp, SET);
  reg_rtx_no = n_regs;

  /* Slightly prefer the original iv.  */
  red->cost--;

  tmp = GEN_BINARY (OP_MULT, red->mode, gen_iteration (red->mode), red->step);
  tmp = GEN_BINARY (OP_PLUS, red->mode, COPY_EXPR (base), tmp);
  red->value = simplify_alg_expr (tmp);
  if (!red->value)
    abort ();

  red->next = *reds;
  *reds = red;
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
create_biv (struct loops *loops, struct str_red *red)
{
  rtx seq, tmp, *note;
  rtx insn, after = NULL_RTX, cond;
  basic_block latch;

  if (red->elimination)
    {
      for (tmp = red->remove_notes; tmp; tmp = XEXP (tmp, 1))
	{
	  for (note = &REG_NOTES (XEXP (tmp, 0));
	       *note;
	       note = &XEXP (*note, 1))
	    if (REG_NOTE_KIND (*note) == REG_EQUAL
		|| REG_NOTE_KIND (*note) == REG_EQUIV)
	      break;
	  if (*note)
	    *note = XEXP (*note, 1);
	}

      free_INSN_LIST_list (&red->remove_notes);
      return;
    }

  start_sequence ();
  tmp = simplify_gen_binary (PLUS, red->mode, red->reg, red->step);
  tmp = force_operand (tmp, red->reg);
  if (tmp != red->reg)
    emit_move_insn (red->reg, tmp);
  seq = get_insns ();
  end_sequence ();

  if (red->increment_after)
    after = red->increment_after;
  else
    {
      /* Find place where to put the increment.  If the latch is not empty,
	 we won't spoil much by putting it there.  If it is empty and it has
	 exactly one predecessor, we may try putting it into this predecessor,
	 but we must be careful not to clobber flags and not to put it into
	 an subloop.  The last possibility is to place it to the start of
	 header; then we must adjust the initial value.  */
      latch = loop_latch_edge (red->loop)->src;
      FOR_BB_INSNS_REVERSE (latch, insn)
	{
	  if (INSN_P (insn))
	    break;
	}
      if (insn != PREV_INSN (latch->head))
	after = insn;
      else
	{
	  int done = false;

	  if (GET_CODE (red->initial_value) != CONST_INT
	      && !latch->pred->pred_next
	      && just_once_each_iteration_p (loops, red->loop,
					     latch->pred->src)
	      && any_condjump_p (latch->pred->src->end)
	      && get_condition (latch->pred->src->end, &cond))
	    {
	      done = true;
	      after = PREV_INSN (cond);
	      for (insn = cond;
		   insn != NEXT_INSN (latch->pred->src->end);
		   insn = NEXT_INSN (insn))
		if (reg_mentioned_p (red->reg, insn))
		  {
		    done = false;
		    break;
		  }
	      
	      /* If the increment does not clobber anything, we may
		 safely put it after insn.  ??? This could be made more
		 general.  */
	      if (!done
		  && insn != latch->pred->src->end
		  && !NEXT_INSN (seq)
		  && GET_CODE (PATTERN (seq)) == SET)
		{
		  after = insn;
		  done = true;
		}
	    }

	  if (!done)
	    {
	      red->initial_value =
		      simplify_alg_expr (GEN_BINARY (MINUS, red->mode,
						     red->initial_value,
						     red->step));
	      after = block_label (red->loop->header);
	      if (NEXT_INSN (after)
		  && GET_CODE (NEXT_INSN (after)) == NOTE)
		after = NEXT_INSN (after);
	    }
	}
    }
  if (!after)
    abort ();
  emit_insn_after (seq, after);

  if (red->increment_after)
    {
      if (GET_CODE (red->increment_after) != NOTE
	  || NOTE_LINE_NUMBER (red->increment_after) != NOTE_INSN_DELETED)
	abort ();
      delete_insn (red->increment_after);
    }

  /* Prepare the initial value.  */
  start_sequence ();
  tmp = force_operand (red->initial_value, red->reg);
  if (tmp != red->reg)
    emit_move_insn (red->reg, tmp);
  seq = get_insns ();
  end_sequence ();
  hoist_insn_to_depth (red->loop, red->loop->depth - 1, seq, true);
}

/* Executes replacement for strength reduction REPL.  */
static void
execute_strength_reduction (struct repl *repl)
{
  if (repl->chosen->iv->elimination)
    {
      delete_insn (repl->insn);
      return;
    }

  emit_insn_before (repl->chosen->seq, repl->insn);
  hoist_insn_to_depth (repl->chosen->iv->loop,
		       repl->chosen->iv->loop->depth - 1,
		       repl->chosen->pre, true);

  if (repl->type == SET)
    {
      delete_insn (repl->insn);
      return;
    }

  if (!validate_change (repl->insn, repl->loc, repl->chosen->replacement, 0))
      abort ();
}

/* Replaces the final value according to FREPL.  */
static void
replace_final_value (struct fval_repl *frepl)
{
  rtx rg, seq, tmp;
  
  if (frepl->initialize_before)
    rg = gen_reg_rtx (GET_MODE (frepl->reg));
  else
    rg = frepl->reg;

  start_sequence ();
  tmp = force_operand (frepl->value, rg);
  if (tmp != rg)
    emit_move_insn (rg, tmp);
  seq = get_insns ();
  end_sequence ();
  hoist_insn_to_depth (frepl->loop, frepl->loop->depth - 1, seq,
		       frepl->initialize_before);

  if (frepl->initialize_before)
    {
      start_sequence ();
      emit_move_insn (frepl->reg, rg);
      seq = get_insns ();
      end_sequence ();
      hoist_insn_to_depth (frepl->loop, frepl->loop->depth - 1, seq, false);
    }
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

  /* Remove unused reds.  */
  for (act = repls; act; act = act->next)
    for (alt = act->alts; alt; alt = alt->next)
      alt->iv->chosen = true;

  for (rarray = reds; *rarray; )
    {
      if ((*rarray)->chosen)
	{
	  (*rarray)->chosen = false;
	  rarray = &(*rarray)->next;
	}
      else
	{
	  red = *rarray;
	  *rarray = red->next;
	  free (red);
	}
    }
  if (!*reds)
    return;

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Loop %d:\n", loop->num);
      fprintf (rtl_dump_file, "Before decision:\n");
      dump_strength_reductions (rtl_dump_file, *reds, repls);
    }

  /* Choose the cheapest possibility for every replacement.  */
  for (act = repls; act; act = act->next)
    {
      act->chosen = act->alts;
      act->chosen->iv->chosen = true;
      if (act->chosen->iv->forces)
	act->chosen->iv->forces->chosen = true;
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

      /* First optimize in a greedy fashion.  */
      while (decrease_by_one_iv (*reds, repls, loop_avail_regs[loop->num]))
	continue;

      if (nivs <= MANY_IVS)
	{
	  /* Try optimizing the rest of ivs now.  */
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
      if (act->chosen->iv->forces
	  && !act->chosen->iv->forces->reg)
	finish_biv (act->chosen->iv->forces);
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
 
/* Removes final value replacements FVREPLS that are unneccesary because
   the original iv is preserved.  */
static void
remove_unnecesary_fvrepls (struct str_red *reds, struct fval_repl **fvrepls)
{
  struct str_red *red;
  struct fval_repl *ths;

  while (*fvrepls)
    {
      /* We want to keep the replacement if it gives more information and
	 the assignment is cheap.  */
      if (good_constant_p ((*fvrepls)->value))
	{
	  fvrepls = &(*fvrepls)->next;
	  continue;
	}

      for (red = reds; red; red = red->next)
	if (red->equal_to
	    && red->equal_to == (*fvrepls)->reg)
	  break;
      if (!red)
	{
	  fvrepls = &(*fvrepls)->next;
	  continue;
	}
      ths = *fvrepls;
      *fvrepls = ths->next;
      free (ths);
    }
}

/* If we decide to preserve a biv, we need to ensure that its original
   increment will be eliminated.  Scan REDS and modify them to achieve
   this.  */
static void
fixup_preserved_bivs (struct str_red **reds)
{
  struct str_red *red, *el;
  struct repl *repl;

  for (red = *reds; red; red = red->next)
    {
      if (!red->equal_to)
	continue;

      repl = red->eq_occ->aux;
      if (!repl)
	abort ();

      if (repl->chosen->iv->elimination
	  || repl->chosen->iv == red)
	continue;

      free (repl->chosen);

      for (el = *reds; el; el = el->next)
	if (el->elimination && el->reg == red->reg)
	  break;
      if (!el)
	{
	  el = xmalloc (sizeof (struct str_red));
	  el->reg = red->reg;
	  el->equal_to = NULL_RTX;
	  el->eq_occ = NULL;
	  el->chosen = false;
	  el->mode = VOIDmode;
	  el->loop = red->loop;
	  el->elimination = true;
	  el->forces = NULL;
	  el->remove_notes = NULL;
	  el->initial_value = NULL_RTX;
	  el->increment_after = NULL_RTX;
	  el->step = NULL_RTX;
	  el->value = NULL_RTX;
	  el->cost = -COST_INFTY;
	  el->next = *reds;
	  *reds = el;
	}

      repl->chosen = xmalloc (sizeof (struct repl_alt));
      repl->chosen->iv = el;
      repl->chosen->seq = NULL_RTX;
      repl->chosen->replacement = NULL_RTX;
      repl->chosen->pre = NULL_RTX;
      repl->chosen->cost = -COST_INFTY;
      repl->chosen->next = NULL;
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

  if (remove)
    remove->chosen = false;

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

      gain += act->cost - alt->cost;
      if (!alt->iv->elimination)
	n_repls++;

      if (alt->iv->forces
	  && !alt->iv->forces->chosen)
	return -COST_INFTY;
    }

  for (red = reds; red; red = red->next)
    if (red->chosen)
      {
	gain -= red->cost;
	if (!red->elimination)
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
  red->reg = red->equal_to ? red->equal_to : gen_reg_rtx (red->mode);
}

/* Fill in remaining fields in replacement repl.  */
static void
finish_repl (struct repl *repl)
{
  if (!repl->chosen->iv->elimination)
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
  if (red->elimination)
    fprintf (file, " (elimination)");
  if (!red->reg && red->equal_to)
    {
      fprintf (file, " -- equal to ");
      print_rtl (file, red->equal_to);
    }
  fprintf (file, "\n");

  if (red->elimination)
    {
      fprintf (file, "  notes should be removed from ");
      print_rtl (file, red->remove_notes);
      fprintf (file, "\n\n");
      return;
    }

  fprintf (file, "  initial value ");
  print_rtl (file, red->initial_value);
  fprintf (file, "\n");

  fprintf (file, "  step ");
  print_rtl (file, red->step);
  fprintf (file, "\n");

  fprintf (file, "  value ");
  print_rtl (file, red->value);
  fprintf (file, "\n"); 

  if (red->increment_after)
    {
      fprintf (file, "  increment after ");
      print_rtl_single (file, red->increment_after);
      fprintf (file, "\n");
    }

  if (red->forces)
    fprintf (file, "  forces %p\n", (void *) red->forces);

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
	  fprintf (file, "    replacement ");
	  print_rtl (file, alt->replacement);
	  fprintf (file, "\n"); 
	}
    }
  fprintf (file, "\n"); 
}
