/* Induction variable analysis.
   Copyright (C) 2003 Free Software Foundation, Inc.

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
#include "params.h"
#include "output.h"
#include "expr.h"
#include "function.h"
#include "df.h"
#include "algebraic.h"

/* We perform induction variable analysis here.  We expect the loops to be
   in the normal form for the loop optimizer -- i.e. with preheaders and latches
   with exactly one successor (therefore the entry edge to a loop always leads
   from its immediate superloop and exit edges lead to superloops of the loop).
   For end of each basic block BB we express the values of registers in form
     REG = (PLUS base (MULT step ITERATION))
   where
     LOOP is the innermost loop containing BB
     base and step are invariant in LOOP. They may use INITIAL_VALUE
      operation and no registers may occur inside the expressions outside
      of INITIAL_VALUE.  The registers in step must also be unchanging.
     ITERATION is the number of executions of LOOP's latch.

   The results are stored in the aux field of df information (i.e. for each
   use and definition of induction variable, we have its value stored there).

   Additionally, following structures are provided:
   loop_entry_values ... for each loop the value of register at start of the
     iteration is stored here
   initial_values ... for each loop the value of register at entry (i.e. start
     of the 0th iteration) is stored here
   iv_occurences ... all definitions of ivs as well as memory addresses that
     act as ivs are stored here, divided by values of their step and base.
     A constant additional factor is also split out of base.

   A note on special rtl codes used in rtl expressions for induction variables:

   initial_value(reg) ... wraps a register inside it and means the value of the
     register at entry of the loop. It may occur in any iv expression, but
     not in initial_values (as its value is relative to the enclosing loop and
     also presence of any explicit register operands is unnecessary there).
     This code is in fact redundant (registers wrapped and not wrapped inside
     it may never occur in a single expression). It is present only for clarity
     of the semantics of those expressions.
   iteration ... means the actual iteration of the enclosing loop. May not
     be present in initial_values (due to being relative to the enclosing loop;
     see the value_at description to how it is replaced there).
   value_at(reg_no, insn_no) ... means the value of register with number
     reg_no immediatelly before the insn with uid insn_no was executed last
     time before the control flow reached the point where it is used. This
     code may occur in any expression.  This code may be only used on places
     where we are sure that the insn insn_no was executed (i.e. they must be
     dominated by it) and the value could not change since that time.
     In initial_values, if the reg_no is -1, insn_no must be the first insn
     in some loop's header and this value_at means the current iteration
     of this loop if we are in some of its subloops, or the number of iterations
     of the loop when we passed through it the last time otherwise (this is
     mostly useless now, but later we might exploit it for loops where we
     are able to determine this number precisely).  */

/* ??? I dislike a code with that much global variables.  It would be better to
   encalupsate them in a structure that would be passed when needed.  Perhaps
   add it as new fields to struct loops?  */

/* Maximal register number.  */
static unsigned max_regno;

/* The loops for that we compute the ivs.  */
static struct loops *current_loops;

/* The real number of loops (not including the deleted ones).  */
static unsigned real_loops_num;

/* Basic blocks in a breath-first search order in a dominance tree.  */
static basic_block *block_dominance_order;

/* The array of loops ordered by the dominance relation on their headers.  */
static struct loop **loops_dominance_order;

/* Dataflow informations.  */
static struct df *df;

/* The induction variables at loop entries.  */
rtx **loop_entry_values;

/* The induction variables at loop exits.  */
static rtx **loop_end_values;

/* The values of registers at entries to the loops.  */
rtx **initial_values;

/* Bitmaps of registers modified in the loops.  */
static sbitmap *modified_regs;

/* Bitmap of insns that we already processed.  */
static sbitmap insn_processed;

/* The dominator relation.  */
static fast_dominance_info dom;

/* Values of registers at current point.  This array is never cleared,
   so the user is responsible for initializing those fields that he's going
   to need.  */
rtx *iv_register_values;

/* Similar to reaching defs, but limiting ourselves only to inside of loops.  */
bitmap *loop_rd_in;

/* Only the defs that are marked here are already computed.  */
sbitmap loop_rd_in_ok;

/* Sbitmap of registers that are interesting for us (pseudoregisters
   in integer modes).  */
sbitmap iv_interesting_reg;

/* Shared rtxes.  */
static rtx *initial_value_rtx;

/* For each loop, a linklist of induction variable occurences.  */
struct iv_occurence_step_class **iv_occurences;

static rtx gen_initial_value		PARAMS ((unsigned));
static rtx gen_value_at			PARAMS ((unsigned, rtx, int));
static void record_def_value		PARAMS ((rtx, unsigned, rtx));
static void record_use_value		PARAMS ((rtx, unsigned, rtx))
     ATTRIBUTE_UNUSED;
static bool invariant_wrto_ivs_p	PARAMS ((rtx, rtx *));
static void simulate_set		PARAMS ((rtx, rtx, void *));
static void simplify_ivs_using_values	PARAMS ((rtx *, rtx *));
static void clear_reg_values		PARAMS ((int));
static rtx earliest_value_at_for	PARAMS ((basic_block, int));
static rtx get_reg_value_at		PARAMS ((basic_block, rtx,
						 struct ref *));
static int iv_omit_initial_values_1	PARAMS ((rtx *, void *));
static void compute_reg_values		PARAMS ((basic_block, rtx));
static void compute_register_values	PARAMS ((int));
static void simplify_reg_values		PARAMS ((basic_block, rtx));
static void simplify_register_values	PARAMS ((void));
static void compute_loop_end_values	PARAMS ((struct loop *, rtx *));
static int replace_iteration_with_value_at PARAMS ((rtx *, void *));
static rtx iv_make_initial_value	PARAMS ((struct loop *, rtx, rtx, unsigned));
static void compute_initial_values	PARAMS ((struct loop *));
static void fill_loop_rd_in_for_def	PARAMS ((struct ref *));
static void fill_rd_for_defs		PARAMS ((basic_block, bitmap));
static void fill_loops_dominance_order	PARAMS ((void));
static void enter_iv_occurence		PARAMS ((struct iv_occurence_step_class **,
						 rtx, rtx, rtx, rtx, rtx, rtx *,
						 enum machine_mode, enum machine_mode,
						 enum rtx_code));
static int record_iv_occurences_1	PARAMS ((rtx *, void *));
static void record_iv_occurences	PARAMS ((struct iv_occurence_step_class **,
						 rtx));
static void iv_new_insn_changes_commit	PARAMS ((basic_block, rtx, rtx));
extern void dump_equations		PARAMS ((FILE *, rtx *));
extern void dump_insn_ivs		PARAMS ((FILE *, rtx));
extern void dump_iv_occurences		PARAMS ((FILE *,
						 struct iv_occurence_step_class *));

/* Dump equations for induction variables in VALUES to FILE.  */
void
dump_equations (file, values)
     FILE *file;
     rtx *values;
{
  unsigned regno;

  fprintf (file, "   unchanged:");
  for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
    if (TEST_BIT (iv_interesting_reg, regno)
	&& values[regno] == gen_initial_value (regno))
      fprintf (file, " %d", regno);
  fprintf (file, "\n");

  for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
    if (TEST_BIT (iv_interesting_reg, regno)
	&& values[regno] != gen_initial_value (regno))
      {
	fprintf (file, "   reg %d:\n", regno);
	print_rtl (file, values[regno]);
	fprintf (file, "\n");
      }
  fprintf (file, "\n\n");
}

/* Dump equations for induction variables at INSN to FILE.  */
void
dump_insn_ivs (file, insn)
     FILE *file;
     rtx insn;
{
  struct df_link *def = DF_INSN_DEFS (df, insn);
  struct df_link *use = DF_INSN_USES (df, insn);

  fprintf (file, "USES:\n");
  for (; use; use = use->next)
    {
      if (!TEST_BIT (iv_interesting_reg, DF_REF_REGNO (use->ref)))
	continue;
      fprintf (file, " reg %d:\n", DF_REF_REGNO (use->ref));
      print_rtl (file, use->ref->aux);
      fprintf (file, "\n");
    }

  fprintf (file, "DEFS:\n");
  for (; def; def = def->next)
    {
      if (!TEST_BIT (iv_interesting_reg, DF_REF_REGNO (def->ref)))
	continue;
      fprintf (file, " reg %d:\n", DF_REF_REGNO (def->ref));
      print_rtl (file, def->ref->aux);
      fprintf (file, "\n");
    }
}

/* Dump equations for induction variables in list of step classes SC to FILE.  */
void
dump_iv_occurences (file, sc)
     FILE *file;
     struct iv_occurence_step_class *sc;
{
  struct iv_occurence_base_class *bc;
  struct iv_occurence *oc;

  for (; sc; sc = sc->sc_next)
    {
      fprintf (file, "  Step class ");
      print_rtl (file, sc->step);
      fprintf (file, ":\n");

      for (bc = sc->bc_first; bc; bc = bc->bc_next)
	{
	  fprintf (file, "   Base class ");
	  print_rtl (file, bc->base);
	  fprintf (file, ":\n");

	  for (oc = bc->oc_first; oc; oc = oc->oc_next)
	    {
	      fprintf (file, "    In insn %d, with delta ", INSN_UID (oc->insn));
	      fprintf (file, HOST_WIDE_INT_PRINT_DEC, INTVAL (oc->delta));
	      fprintf (file, ":\n");
	      print_rtl (file, *oc->occurence);
	      fprintf (file, "\n");
	    }
	  fprintf (file, "\n");
	}
      fprintf (file, "\n");
    }
  fprintf (file, "\n");
}

/* Generate INITIAL_VALUE for register REGNO (they are shared, so just return
   the rtx).  */
static rtx
gen_initial_value (regno)
     unsigned regno;
{
  return initial_value_rtx[regno];
}

/* Generate a VALUE_AT rtx for register REGNO at INSN (if AFTER, immediatelly
   after it).  */
static rtx
gen_value_at (regno, insn, after)
     unsigned regno;
     rtx insn;
     int after;
{
  int place_number;

  if (!after)
    place_number = INSN_UID (insn);
  else if (NEXT_INSN (insn))
    place_number = INSN_UID (NEXT_INSN (insn));
  else
    place_number = -INSN_UID (insn);
  return gen_rtx_fmt_ii (VALUE_AT, GET_MODE (regno_reg_rtx[regno]), regno,
			 place_number);
}

/* Record VALUE to def of register REGNO in INSN.  */
static void
record_def_value (insn, regno, value)
     rtx insn;
     unsigned regno;
     rtx value;
{
  struct df_link *def = DF_INSN_DEFS (df, insn);

  for (; def; def = def->next)
    if (DF_REF_REGNO (def->ref) == regno)
      break;
  if (!def)
    abort ();

  def->ref->aux = value;
}

/* Record VALUE to use of register REGNO in INSN.  */
static void
record_use_value (insn, regno, value)
     rtx insn;
     unsigned regno;
     rtx value;
{
  struct df_link *use = DF_INSN_USES (df, insn);

  for (; use; use = use->next)
    if (DF_REF_REGNO (use->ref) == regno)
      break;
  if (!use)
    abort ();

  use->ref->aux = value;
}

/* Get value from def of register REGNO in INSN.  */
rtx
get_def_value (insn, regno)
     rtx insn;
     unsigned regno;
{
  struct df_link *def = DF_INSN_DEFS (df, insn);

  for (; def; def = def->next)
    if (DF_REF_REGNO (def->ref) == regno)
      break;
  if (!def)
    abort ();

  return (rtx) def->ref->aux;
}

/* Get VALUE from use of register REGNO in INSN.  */
rtx
get_use_value (insn, regno)
     rtx insn;
     unsigned regno;
{
  struct df_link *use = DF_INSN_USES (df, insn);

  for (; use; use = use->next)
    if (DF_REF_REGNO (use->ref) == regno)
      break;
  if (!use)
    abort ();

  return (rtx) use->ref->aux;
}

/* Called through for_each_rtx from iv_omit_initial_values.  */
static int
iv_omit_initial_values_1 (expr, data)
     rtx *expr;
     void *data ATTRIBUTE_UNUSED;
{
  if (GET_CODE (*expr) == INITIAL_VALUE)
    {
      *expr = XEXP (*expr, 0);
      return -1;
    }
  return 0;
}

/* Omits initial_values from the expression EXPR.  */
rtx
iv_omit_initial_values (expr)
     rtx expr;
{
  expr = copy_rtx (expr);
  for_each_rtx (&expr, iv_omit_initial_values_1, NULL);
  return expr;
}

/* Checks whether all registers used to compute EXPR are unchanging according
   to VALUES.  */
static bool
invariant_wrto_ivs_p (expr, values)
     rtx expr;
     rtx *values;
{
  unsigned regno;
  int i, length;
  const char *format;
 
  if (!expr)
    return false;

  switch (GET_CODE (expr))
    {
    case INITIAL_VALUE:
      regno = REGNO (XEXP (expr, 0));
      return (TEST_BIT (iv_interesting_reg, regno)
	      && values[regno] == gen_initial_value (regno));

    case VALUE_AT:
      return false;

    default: ;
    }

  length = GET_RTX_LENGTH (GET_CODE (expr));
  format = GET_RTX_FORMAT (GET_CODE (expr));
  for (i = 0; i < length; i++)
    {
      switch (format[i])
	{
	case 'e':
	  if (!invariant_wrto_ivs_p (XEXP (expr, i), values))
	    return false;
	  break;

	case 'V':
	case 'E':
	  abort ();

	default:
	  /* Nothing to do.  */
	  break;
	}
    }

  return true;
}

/* Simulate effect of one set on values passed in DATA.  Called through
   note_stores.  The global variable CURRENT_INSN is used to provide the
   insn inside that the set occurs.  In ACT_MODIFIED_REGS a bitmap of registers
   whose values were altered is stored.  */
static rtx current_insn;
static sbitmap act_modified_regs;

static void
simulate_set (reg, set, data)
     rtx reg;
     rtx set;
     void *data;
{
  rtx *values = (rtx *) data;
  rtx src, value;
  unsigned regno;

  if (!REG_P (reg))
    return;
  regno = REGNO (reg);

  if (!TEST_BIT (iv_interesting_reg, regno))
    return;

  SET_BIT (act_modified_regs, regno);
  if (GET_CODE (set) == CLOBBER)
    value = NULL_RTX;
  else
    {
      src = SET_SRC (set);
      value = substitute_into_expr (src, iv_interesting_reg, values,
				    SIE_SIMPLIFY);
    }
  if (!value)
    value = gen_value_at (regno, current_insn, true);
  record_def_value (current_insn, regno, value);
}

/* Try to substitute initial values of registers (INITIAL_VALUES) into
   induction variables (VALUES) and keep those that result in constants.  */
static void
simplify_ivs_using_values (values, initial_values)
     rtx *values;
     rtx *initial_values;
{
  unsigned regno;
  rtx value, svalue;

  for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
    if (TEST_BIT (iv_interesting_reg, regno))
      {
	value = values[regno];
	svalue = simplify_alg_expr_using_values (value, iv_interesting_reg,
						 initial_values);
	if (svalue && svalue != value)
	  values[regno] = svalue;
      }
}

/* Attempt to simplify the expression EXPR using initial values at entry of
   the LOOP.  If EXPR is list, then its elements are combined together through
   operation OP.  */
rtx
iv_simplify_using_initial_values (op, expr, loop)
     enum rtx_code op;
     rtx expr;
     struct loop *loop;
{
  rtx l, r, tmp;

  if (!expr || good_constant_p (expr))
    return expr;

  if (GET_CODE (expr) == EXPR_LIST)
    {
      l = iv_simplify_using_initial_values (NIL, XEXP (expr, 0), loop);
      r = iv_simplify_using_initial_values (op, XEXP (expr, 1), loop);

      switch (op)
	{
	case AND:
	  if (l == const0_rtx)
	    expr = alloc_EXPR_LIST (0, const0_rtx, NULL_RTX);
	  else if (l == const_true_rtx)
	    expr = r;
	  else if (r && XEXP (r, 0) == const0_rtx)
	    expr = r;
	  break;

	case IOR:
	  if (l == const_true_rtx)
	    expr = alloc_EXPR_LIST (0, const_true_rtx, NULL_RTX);
	  else if (l == const0_rtx)
	    expr = r;
	  else if (r && XEXP (r, 0) == const_true_rtx)
	    expr = r;
	  break;

	default:
	  abort ();
	}
      return expr;
    }

  tmp = substitute_into_expr (expr, iv_interesting_reg,
			      initial_values[loop->num], SIE_SIMPLIFY);
  if (tmp && good_constant_p (tmp))
    return tmp;

  return expr;
}

/* Clears stored values of registers (unless INCLUDING_TOP, ignore values
   in the fake outermost loop).  */
static void
clear_reg_values (including_top)
     int including_top;
{
  unsigned i;
  struct ref *ref;

  for (i = 0; i < df->n_defs; i++)
    {
      ref = df->defs[i];
      if (!including_top && !DF_REF_BB (ref)->loop_father->outer)
	continue;
      ref->aux = NULL_RTX;
    }

  for (i = 0; i < df->n_uses; i++)
    {
      ref = df->uses[i];
      if (!including_top && !DF_REF_BB (ref)->loop_father->outer)
	continue;
      ref->aux = NULL_RTX;
    }
}

/* Generates VALUE_AT for register REGNO as near to entry as possible, startin
   at basic block BB.  */
static rtx
earliest_value_at_for (bb, regno)
     basic_block bb;
     int regno;
{
  /* We may continue backwards as long as we have an unique predecessor;
     if the register was altered in any such block, it would have just this
     single definition and we would not be called.  */
  while (bb->pred && !bb->pred->pred_next)
    bb = bb->pred->src;
  return gen_value_at (regno, bb->head, false);
}

/* Computes value of register referenced by REF immediately before INSN in
   basic block BB. */
static rtx
get_reg_value_at (bb, insn, ref)
     basic_block bb;
     rtx insn;
     struct ref *ref;
{
  struct df_link *def;
  struct loop *loop = bb->loop_father, *def_loop;
  basic_block def_bb;
  rtx def_insn;
  unsigned regno = DF_REF_REGNO (ref);
  unsigned defno;

  /* There are three cases:
     -- a single definition inside loop strictly dominates us, and is not
	part of any subloop -- then this is the value we want.
     -- a definition inside loop that reaches us, or definition from inside
	of subloop -- then the value is unknown
     -- any number of other definitions (outside, or inside of loop but
	only reaching us through latch/outside of loop) -- then the
	value is the initial one.  */

  for (def = DF_REF_CHAIN (ref); def; def = def->next)
    {
      def_insn = DF_REF_INSN (def->ref);
      def_bb = BLOCK_FOR_INSN (def_insn);
      def_loop = def_bb->loop_father;
      defno = DF_REF_ID (def->ref);

      if (loop != def_loop)
	{
	  if (flow_loop_nested_p (loop, def_loop))
	    return earliest_value_at_for (bb, regno);
	  else
	    continue;
	}

      /* The definition that dominates us.  */
      if (def_bb == bb
	  && DF_INSN_LUID (df, def_insn) < DF_INSN_LUID (df, insn))
	break;
      if (def_bb != bb && fast_dominated_by_p (dom, bb, def_bb))
	break;

      if (!TEST_BIT (loop_rd_in_ok, defno))
	fill_loop_rd_in_for_def (def->ref);

      /* The definition that does not dominate us, but reaches us.  */
      if (bitmap_bit_p (loop_rd_in[bb->index], defno))
	return earliest_value_at_for (bb, regno);
    }

  if (!def)
    return loop_entry_values[loop->num][regno];

  /* The def dominates us.  If it is not the last one,  the remaining
     def(s) must reach us from inside of the loop and cannot dominate us.  */
  if (def->next)
    return earliest_value_at_for (bb, regno);

  /* The value must be computed, because we refer here to a definition
     that dominates us and we process the blocks in the dominance tree
     breath-first search order.  */
  if (!TEST_BIT (insn_processed, INSN_UID (def_insn)))
    abort ();

  return get_def_value (def_insn, regno);
}

/* Store values of registers used in INSN to VALUES.  */
extern void
iv_load_used_values (insn, values)
     rtx insn;
     rtx *values;
{
  struct df_link *use = DF_INSN_USES (df, insn);
  unsigned regno;

  use = DF_INSN_USES (df, insn);
  for (; use; use = use->next)
    {
      regno = DF_REF_REGNO (use->ref);
      values[regno] = (rtx) use->ref->aux;
    }
}

/* Computes values of registers stored at INSN in basic block BB.  */
static void
compute_reg_values (bb, insn)
     basic_block bb;
     rtx insn;
{
  struct df_link *use = DF_INSN_USES (df, insn);
  unsigned regno;

  if (TEST_BIT (insn_processed, INSN_UID (insn)))
    return;
  SET_BIT (insn_processed, INSN_UID (insn));

  if (!INSN_P (insn))
    return;

  /* First compute the values of used registers.  */
  for (; use; use = use->next)
    {
      regno = DF_REF_REGNO (use->ref);
      if (!TEST_BIT (iv_interesting_reg, regno))
	continue;

      use->ref->aux = (void *) get_reg_value_at (bb, insn, use->ref);
    }

  /* Now simulate the computation to fill in the values of defs.  */
  iv_load_used_values (insn, iv_register_values);
  current_insn = insn;
  act_modified_regs = modified_regs[bb->loop_father->num];
  note_stores (PATTERN (insn), simulate_set, (void *) iv_register_values);
}

/* Computes value of registers used/defined in all insns wrto the innermost
   loop they belong to; values of registers at loop entries are assumed
   to be stored in loop_entry_values.  Unless INCLUDING_TOP, fake loop
   around the function is ignored.  */
static void
compute_register_values (including_top)
     int including_top;
{
  int abb;
  basic_block bb;
  rtx insn;

  clear_reg_values (including_top);

  sbitmap_zero (insn_processed);
  /* Scan basic blocks in the dominance order, so that the values are known
     when we need them.  */
  for (abb = 0; abb < n_basic_blocks; abb++)
    {
      bb = block_dominance_order[abb];

      FOR_BB_INSNS (bb, insn)
	{
	  if (!including_top && !bb->loop_father->outer)
	    SET_BIT (insn_processed, INSN_UID (insn));
	  else
	    compute_reg_values (bb, insn);
	}
    }
}

/* Simplifies values of registers stored at INSN in basic block BB.  */
static void
simplify_reg_values (bb, insn)
     basic_block bb;
     rtx insn;
{
  struct df_link *use = DF_INSN_USES (df, insn);
  struct df_link *def = DF_INSN_DEFS (df, insn);
  rtx svalue;
  rtx *values = initial_values[bb->loop_father->num];

  if (!INSN_P (insn))
    return;

  for (; use; use = use->next)
    {
      if (!use->ref->aux)
	continue;
      svalue = simplify_alg_expr_using_values (use->ref->aux,
					       iv_interesting_reg, values);
      if (svalue)
	use->ref->aux = svalue;
    }
  for (; def; def = def->next)
    {
      if (!def->ref->aux)
	continue;
      svalue = simplify_alg_expr_using_values (def->ref->aux,
					       iv_interesting_reg, values);
      if (svalue)
	def->ref->aux = svalue;
    }
}

/* Simplifies values of registers used/defined in all insns using the initial
   values of the loop they belong to.  */
static void
simplify_register_values ()
{
  basic_block bb;
  rtx insn;

  FOR_EACH_BB (bb)
    {
      if (!bb->loop_father->outer)
	continue;

      FOR_BB_INSNS (bb, insn)
	simplify_reg_values (bb, insn);
    }
}

/* Fill in loop_rd_in for a given DEF.  */
static void
fill_loop_rd_in_for_def (def)
     struct ref *def;
{
  unsigned defno = DF_REF_ID (def);
  basic_block def_bb = DF_REF_BB (def), dest;
  struct loop *def_loop = def_bb->loop_father;
  edge *stack, act;
  int stack_top = 0;
  edge latch = def_loop->outer ? loop_latch_edge (def_loop) : NULL;

  SET_BIT (loop_rd_in_ok, defno);
  if (!TEST_BIT (iv_interesting_reg, DF_REF_REGNO (def))
      || !bitmap_bit_p (DF_BB_INFO (df, def_bb)->rd_out, defno)
      || !def_bb->succ)
    return;

  stack = xmalloc (sizeof (edge) * n_basic_blocks);
  act = def_bb->succ;

  while (1)
    {
      for (; act; act = act->succ_next)
	{
	  dest = act->dest;
	  if (dest == EXIT_BLOCK_PTR
	      || act == latch
	      || !flow_bb_inside_loop_p (def_loop, dest)
	      || bitmap_bit_p (loop_rd_in[dest->index], defno))
	    continue;

	  bitmap_set_bit (loop_rd_in[dest->index], defno);
	  if (!bitmap_bit_p (DF_BB_INFO (df, dest)->rd_kill, defno))
	    break;
	}

      if (act)
	{
	  if (act->succ_next)
	    stack[stack_top++] = act->succ_next;
	  act = dest->succ;
	}
      else if (stack_top == 0)
	break;
      else act = stack[--stack_top];
    }
  free (stack);
}

/* Fill in reaching definitions for DEFS with unique set in basic block BB.  */
static void
fill_rd_for_defs (bb, defs)
     basic_block bb;
     bitmap defs;
{
  basic_block dest;
  edge *stack, act;
  int stack_top = 0;
  sbitmap processed;

  bitmap_operation (DF_BB_INFO (df, bb)->rd_gen,
		    DF_BB_INFO (df, bb)->rd_gen,
		    defs, BITMAP_IOR);
  bitmap_operation (DF_BB_INFO (df, bb)->rd_out,
		    DF_BB_INFO (df, bb)->rd_out,
		    defs, BITMAP_IOR);

  stack = xmalloc (sizeof (edge) * n_basic_blocks);
  processed = sbitmap_alloc (last_basic_block);
  sbitmap_zero (processed);
  act = bb->succ;

  while (1)
    {
      for (; act; act = act->succ_next)
	{
	  dest = act->dest;
	  if (dest == EXIT_BLOCK_PTR
	      || TEST_BIT (processed, dest->index))
	    continue;

	  SET_BIT (processed, dest->index);
	  bitmap_operation (DF_BB_INFO (df, dest)->rd_in,
			    DF_BB_INFO (df, dest)->rd_in,
			    defs, BITMAP_IOR);
	  if (dest != bb)
	    {
	      bitmap_operation (DF_BB_INFO (df, dest)->rd_out,
				DF_BB_INFO (df, dest)->rd_out,
				defs, BITMAP_IOR);
	      break;
	    }
	}

      if (act)
	{
	  if (act->succ_next)
	    stack[stack_top++] = act->succ_next;
	  act = dest->succ;
	}
      else if (stack_top == 0)
	break;
      else act = stack[--stack_top];
    }
  sbitmap_free (processed);
  free (stack);
}

/* Enumerates loops in the dominance order of their headers.  (More precisely
   in an breath-first search order in the dominators tree).  */
static void
fill_loops_dominance_order ()
{
  basic_block bb, *dom_bbs;
  unsigned n_bbs, abb, n_dom_bbs, n_loops;

  block_dominance_order = xmalloc (n_basic_blocks * sizeof (basic_block));
  n_bbs = 1;
  block_dominance_order[0] = ENTRY_BLOCK_PTR->succ->dest;

  abb = 0;
  while (abb < n_bbs)
    {
      n_dom_bbs = get_dominated_by (current_loops->cfg.dom,
				    block_dominance_order[abb], &dom_bbs);
      abb++;
      while (n_dom_bbs--)
	block_dominance_order[n_bbs++] = dom_bbs[n_dom_bbs];
      free (dom_bbs);
    }
  if (n_bbs != (unsigned) n_basic_blocks)
    abort ();

  loops_dominance_order[0] = current_loops->tree_root;
  n_loops = 1;
  for (abb = 0; abb < n_bbs; abb++)
    {
      bb = block_dominance_order[abb];
      if (bb->loop_father->header == bb)
	loops_dominance_order[n_loops++] = bb->loop_father;
    }
  if (n_loops != real_loops_num)
    abort ();
}

/* Initialize variables used by the analysis.  */
void
initialize_iv_analysis (loops)
     struct loops *loops;
{
  unsigned i;
  basic_block bb;
  enum machine_mode mode;

  df = df_init ();
  df_analyse (df, 0, DF_UD_CHAIN | DF_RD);
  loop_rd_in = xmalloc (sizeof (bitmap) * last_basic_block);
  FOR_EACH_BB (bb)
    {
      loop_rd_in[bb->index] = BITMAP_XMALLOC ();
      bitmap_zero (loop_rd_in[bb->index]);
    }
  loop_rd_in_ok = sbitmap_alloc (df->n_defs);
  sbitmap_zero (loop_rd_in_ok);

  current_loops = loops;
  dom = create_fq_dominators (current_loops->cfg.dom);

  max_regno = max_reg_num ();

  iv_interesting_reg = sbitmap_alloc (max_regno);
  sbitmap_zero (iv_interesting_reg);
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    {
      mode = GET_MODE (regno_reg_rtx[i]);
      if (GET_MODE_CLASS (mode) == MODE_INT
	  || GET_MODE_CLASS (mode) == MODE_PARTIAL_INT)
	SET_BIT (iv_interesting_reg, i);
    }

  initial_value_rtx = xmalloc (sizeof (rtx) * max_regno);
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    {
      mode = GET_MODE (regno_reg_rtx[i]);

      if (!TEST_BIT (iv_interesting_reg, i))
	initial_value_rtx[i] = NULL_RTX;
      else
	initial_value_rtx[i] = gen_rtx_fmt_e (INITIAL_VALUE,
					      mode, regno_reg_rtx[i]);
    }

  modified_regs = xmalloc (current_loops->num * sizeof (sbitmap));
  for (i = 0; i < current_loops->num; i++)
    if (current_loops->parray[i])
      modified_regs[i] = sbitmap_alloc (max_regno);
  insn_processed = sbitmap_alloc (get_max_uid () + 1);
  iv_register_values = xmalloc (max_regno * sizeof (rtx));

  initial_values = xmalloc (sizeof (rtx *) * current_loops->num);
  loop_entry_values = xmalloc (sizeof (rtx *) * current_loops->num);
  loop_end_values = xmalloc (sizeof (rtx *) * current_loops->num);
  for (i = 0; i < current_loops->num; i++)
    if (current_loops->parray[i])
      {
	initial_values[i] = xmalloc (sizeof (rtx) * max_regno);
	loop_entry_values[i] = xmalloc (sizeof (rtx) * max_regno);
	loop_end_values[i] = xmalloc (sizeof (rtx) * max_regno);
      }

  iv_occurences = xcalloc (current_loops->num,
			   sizeof (struct iv_occurence_step_class *));

  real_loops_num = 0;
  for (i = 0; i < current_loops->num; i++)
    if (current_loops->parray[i])
      real_loops_num++;
  loops_dominance_order = xmalloc (sizeof (struct loop *) * real_loops_num);
  fill_loops_dominance_order ();
}

/* Free variables used by the analysis.  */
void
finalize_iv_analysis ()
{
  unsigned i;
  basic_block bb;
  struct iv_occurence_step_class *sc_next;
  struct iv_occurence_base_class *bc_act, *bc_next;
  struct iv_occurence *oc_act, *oc_next;

  sbitmap_free (insn_processed);
  free (iv_register_values);

  sbitmap_free (iv_interesting_reg);

  for (i = 0; i < current_loops->num; i++)
    if (current_loops->parray[i])
      {
	sbitmap_free (modified_regs[i]);
	free (initial_values[i]);
	free (loop_entry_values[i]);
	free (loop_end_values[i]);
      }
  free (initial_values);
  free (loop_entry_values);
  free (loop_end_values);

  free (initial_value_rtx);

  df_finish (df);
  FOR_EACH_BB (bb)
    {
      BITMAP_XFREE (loop_rd_in[bb->index]);
    }
  free (loop_rd_in);
  sbitmap_free (loop_rd_in_ok);

  for (i = 1; i < current_loops->num; i++)
    for (; iv_occurences[i]; iv_occurences[i] = sc_next)
      {
	sc_next = iv_occurences[i]->sc_next;
	for (bc_act = iv_occurences[i]->bc_first; bc_act; bc_act = bc_next)
	  {
	    bc_next = bc_act->bc_next;
	    for (oc_act = bc_act->oc_first; oc_act; oc_act = oc_next)
	      {
		oc_next = oc_act->oc_next;
		free (oc_act);
	      }
	    free (bc_act);
	  }
	free (iv_occurences[i]);
      }
  free (iv_occurences);

  release_fq_dominators (dom);
  free (loops_dominance_order);
  free (block_dominance_order);
}

/* Computes values of modified registers at end of LOOP, putting the result
   into VALUES.  */
static void
compute_loop_end_values (loop, values)
     struct loop *loop;
     rtx *values;
{
  unsigned regno;
  int defno;
  struct ref *def;
  basic_block def_bb;
  struct ref **found_def = xcalloc (max_regno, sizeof (struct ref *));
  sbitmap invalid = sbitmap_alloc (max_regno);
  sbitmap_zero (invalid);

  /* There must be exactly one definition of reg coming from inside of the
     loop that dominates the loop latch and belongs directly to the loop.  */

  EXECUTE_IF_SET_IN_BITMAP (DF_BB_INFO (df, loop->latch)->rd_out, 0, defno,
    {
      def = df->defs[defno];
      def_bb = DF_REF_BB (def);
      regno = DF_REF_REGNO (def);
      if (TEST_BIT (modified_regs[loop->num], regno)
	  && flow_bb_inside_loop_p (loop, def_bb))
	{
	  if (def_bb->loop_father == loop
	      && fast_dominated_by_p (dom, loop->latch, def_bb))
	    found_def[regno] = def;
	  else
	    SET_BIT (invalid, regno);
	}
    });

  EXECUTE_IF_SET_IN_SBITMAP (modified_regs[loop->num], 0, regno,
    {
      if (!TEST_BIT (invalid, regno))
	{
	  if (found_def[regno])
	    values[regno] = found_def[regno]->aux;
	  else
	    values[regno] = gen_initial_value (regno);
  	}
      else
	values[regno] = NULL_RTX;
    });
  sbitmap_free (invalid);
  free (found_def);
}

/* Called through for_each_rtx from iv_make_initial_value; replaces occurence
   of iteration with value_at (-1, insn (passed in data)).  */
static int replaced;	/* Used to return whether any replacement was done.  */
static int
replace_iteration_with_value_at (expr, data)
     rtx *expr;
     void *data;
{
  rtx insn = data;

  if (GET_CODE (*expr) != ITERATION)
    return 0;

  *expr = gen_rtx_fmt_ii (VALUE_AT, GET_MODE (*expr), -1, INSN_UID (insn));
  replaced = true;
  return -1;
}

/* Make EXPR suitable for usage as initial value by replacing iteration with
   value_at (-1, loop header's start) and substituting for initial values.
   INSN is the place where this def of register REGNO with value EXPR occurs.  */
static rtx
iv_make_initial_value (loop, insn, expr, regno)
     struct loop *loop;
     rtx insn;
     rtx expr;
     unsigned regno;
{
  int original = true;

  if (fast_expr_mentions_code_p (expr, INITIAL_VALUE))
    {
      expr = substitute_into_expr (expr, iv_interesting_reg,
				   initial_values[loop->num], SIE_SIMPLIFY);
      if (!expr)
	return gen_value_at (regno, insn, true);
      original = false;
    }

  if (!loop->outer)
    return expr;

  if (original)
    {
      if (!fast_expr_mentions_code_p (expr, ITERATION))
	return expr;
      expr = copy_rtx (expr);
    }

  replaced = false;
  for_each_rtx (&expr, replace_iteration_with_value_at, loop->header->head);
  if (replaced)
    expr = simplify_alg_expr (expr);
  return expr;
}

/* Compute values of registers at entry to the LOOP, using the values
   already computed for its superloops.  */
static void
compute_initial_values (loop)
     struct loop *loop;
{
  rtx *values = initial_values[loop->num];
  unsigned defno, regno;
  struct ref **found_def = xcalloc (max_regno, sizeof (struct ref *));
  sbitmap invalid = sbitmap_alloc (max_regno);
  basic_block preheader = loop_preheader_edge (loop)->src;
  struct loop *outer = loop->outer;
  rtx outer_preheader_end;
  int outer_preheader_end_after;
  rtx *outer_values = initial_values[outer->num];
  struct ref *def;
  basic_block def_bb;

  if (outer && outer->outer)
    {
      outer_preheader_end = loop_preheader_edge (loop)->src->end;
      outer_preheader_end_after = true;
    }
  else
    {
      outer_preheader_end = ENTRY_BLOCK_PTR->succ->dest->head;
      outer_preheader_end_after = false;
    }
  sbitmap_zero (invalid);

  /* Check definitions reaching the end of the loop's preheader.  We are able
     to determine values of registers that are either only defined outside of
     the outer loop (using the initial values of the outer loop), or have
     exactly one definition in the outer loop that dominates the preheader.  */
  EXECUTE_IF_SET_IN_BITMAP (DF_BB_INFO (df, preheader)->rd_out, 0, defno,
    {
      def = df->defs[defno];
      def_bb = DF_REF_BB (def);
      regno = DF_REF_REGNO (def);
      if (flow_bb_inside_loop_p (outer, def_bb))
	{
	  if (!fast_dominated_by_p (dom, preheader, def_bb))
	    SET_BIT (invalid, regno);
	  else if (found_def[regno])
	    abort ();
	  else
	    found_def[regno] = def;
	}
    });

  for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
    {
      def = found_def[regno];
      if (!TEST_BIT (iv_interesting_reg, regno))
	values[regno] = NULL_RTX;
      else if (TEST_BIT (invalid, regno))
	values[regno] = gen_value_at (regno, preheader->end, true);
      else if (def)
	values[regno] = iv_make_initial_value (DF_REF_BB (def)->loop_father,
					       def->insn, def->aux, regno);
      else
	values[regno] = outer_values[regno];
    }
  free (found_def);
  sbitmap_free (invalid);
}

/* Enters occurence with given parameters into list TO.  For meaning of
   the parameters see definition of struct iv_occurence (and related
   structures).  */
static void
enter_iv_occurence (to, base, delta, local_base, step, insn, occurence,
		    real_mode, extended_mode, extend)
     struct iv_occurence_step_class **to;
     rtx base;
     rtx delta;
     rtx local_base;
     rtx step;
     rtx insn;
     rtx *occurence;
     enum machine_mode real_mode;
     enum machine_mode extended_mode;
     enum rtx_code extend;
{
  struct iv_occurence *nw = xmalloc (sizeof (struct iv_occurence));
  struct iv_occurence_base_class **bc;

  while (*to && !rtx_equal_p (step, (*to)->step))
    to = &(*to)->sc_next;
  if (!*to)
    {
      *to = xmalloc (sizeof (struct iv_occurence_step_class));
      (*to)->sc_next = NULL;
      (*to)->bc_first = NULL;
      (*to)->step = step;
    }

  bc = &(*to)->bc_first;
  while (*bc && !rtx_equal_p (base, (*bc)->base))
    bc = &(*bc)->bc_next;
  if (!*bc)
    {
      *bc = xmalloc (sizeof (struct iv_occurence_base_class));
      (*bc)->bc_next = NULL;
      (*bc)->oc_first = NULL;
      (*bc)->base = base;
      (*bc)->step_class = *to;
    }

  nw->insn = insn;
  nw->occurence = occurence;
  nw->delta = delta;
  nw->local_base = local_base;
  nw->real_mode = real_mode;
  nw->extended_mode = extended_mode;
  nw->extend = extend;
  nw->base_class = *bc;
  nw->oc_next = (*bc)->oc_first;
  (*bc)->oc_first = nw;
}

/* Called through for_each_rtx from record_iv_occurences.  Record the
   occurences found.  */
static int
record_iv_occurences_1 (expr, data)
     rtx *expr;
     void *data;
{
  struct iv_occurence_step_class **to = data;
  rtx val, dest, base, lbase, sbase, step, delta, *tmp, *last;
  struct loop *loop = BLOCK_FOR_INSN (current_insn)->loop_father;
  enum machine_mode real_mode, extended_mode;
  enum rtx_code extend;

  if (GET_CODE (*expr) == SET)
    {
      dest = SET_DEST (*expr);
      if (GET_CODE (dest) != REG
	  || !TEST_BIT (iv_interesting_reg, REGNO (dest)))
	return 0;
      val = get_def_value (current_insn, REGNO (dest));
    }
  else if (GET_CODE (*expr) == MEM)
    {
      val = XEXP (*expr, 0);
      val = substitute_into_expr (val, iv_interesting_reg,
				  iv_register_values, SIE_SIMPLIFY);
    }
  else
    return 0;

  if (!val)
    return 0;

  val = simplify_alg_expr_using_values (val, iv_interesting_reg,
					initial_values[loop->num]);
  if (!val)
    return 0;

  extended_mode = GET_MODE (val);
  if (GET_CODE (val) == SIGN_EXTEND
      || GET_CODE (val) == ZERO_EXTEND)
    {
      extend = GET_CODE (val);
      val = XEXP (val, 0);
    }
  else
    extend = NIL;
  real_mode = GET_MODE (val);

  iv_split (val, &base, &step);
  if (!base
      || expr_mentions_code_p (base, VALUE_AT)
      || expr_mentions_code_p (step, VALUE_AT))
    return 0;

  lbase = copy_rtx (base);
  sbase = substitute_into_expr (base, iv_interesting_reg,
				initial_values[loop->num], SIE_SIMPLIFY);
  if (sbase)
    base = sbase;

  tmp = &base;
  last = NULL;
  while (GET_CODE (*tmp) == PLUS)
    {
      last = tmp;
      tmp = &XEXP (*tmp, 0);
    }
  if (GET_CODE (*tmp) == CONST_INT)
    {
      delta = *tmp;
      if (!last)
	base = const0_rtx;
      else
	*last = XEXP (*last, 1);
    }
  else
    delta = const0_rtx;

  enter_iv_occurence (to, base, delta, lbase, step, current_insn, expr,
		      real_mode, extended_mode, extend);
  return 0;
}

/* Record iv occurences in INSN to list *TO.  */
static void
record_iv_occurences (to, insn)
     struct iv_occurence_step_class **to;
     rtx insn;
{
  struct df_link *use = DF_INSN_USES (df, insn);
  struct df_link *def = DF_INSN_DEFS (df, insn);

  /* Check that there is anything to bother with.  */
  for (; use; use = use->next)
    if (DF_REF_REG_MEM_P (use->ref)
	&& TEST_BIT (iv_interesting_reg, DF_REF_REGNO (use->ref))
	&& fast_expr_mentions_code_p ((rtx) use->ref->aux, ITERATION))
      break;
  if (!use)
    {
      for (; def; def = def->next)
	if (TEST_BIT (iv_interesting_reg, DF_REF_REGNO (def->ref))
	    && fast_expr_mentions_code_p ((rtx) def->ref->aux, ITERATION))
	  break;
      if (!def)
	return;
    }

  current_insn = insn;
  iv_load_used_values (insn, iv_register_values);
  for_each_rtx (&PATTERN (insn), record_iv_occurences_1, to);
}

/* Updates a df and iv information for new INSN.  It is expecting the situation
   in that it is called from iv_emit_insn_*, i.e. that it is used to
   sequentially process newly created insns that set only entirely new
   registers.  We also are not allowed to create new bivs this way.

   We do the following:

   -- scan the insn for defs and uses
   -- for each def -- add it to reaching defs of all reachable blocks in the
		      function; this is slow (it is quadratic on functions
		      with a lot of small loops) and almost unnecessary
		      (we don't need the information outside the current
		      loop), so we may change it in the future. For now
		      play it safe and keep the state consistent.
   -- for each use -- find the defs that reach it and form the UD chain
		      from them; calculate their value for iv analysis.
   -- for each def -- calculate their value for iv analysis

   It is split into two phases -- first we find the new defs & uses,
   then we reallocate the structures if needed and finally we fill in
   the info.

   It would be nice if we could reuse more of the df.c code, but it is
   not very well suited for this type of "I know what I am doing" changes.

   BB is the altered basic block, FIRST and LAST are the first and the last
   of the altered insns.  */
static void
iv_new_insn_changes_commit (bb, first, last)
     basic_block bb;
     rtx first;
     rtx last;
{
  unsigned i, regno;
  enum machine_mode mode;
  unsigned new_max_regno = max_reg_num ();
  rtx x;
  struct df_link *def;
  struct loop *loop = bb->loop_father;

  df_refs_queue (df);
  for (x = first; x != NEXT_INSN (last); x = NEXT_INSN (x))
    {
      df_insn_modify (df, bb, x);
      df_insn_refs_record (df, bb, x);
    }
  df_refs_process (df);
    
  if (new_max_regno > max_regno)
    {
      iv_interesting_reg = sbitmap_resize (iv_interesting_reg, new_max_regno, 0);
      for (regno = max_regno; regno < new_max_regno; regno++)
	{
	  mode = GET_MODE (regno_reg_rtx[regno]);
	  if (GET_MODE_CLASS (mode) == MODE_INT
	      || GET_MODE_CLASS (mode) == MODE_PARTIAL_INT)
	    SET_BIT (iv_interesting_reg, regno);
	}

      initial_value_rtx = xrealloc (initial_value_rtx,
				    sizeof (rtx) * new_max_regno);
      for (regno = max_regno; regno < new_max_regno; regno++)
	{
	  mode = GET_MODE (regno_reg_rtx[regno]);
	  if (!TEST_BIT (iv_interesting_reg, regno))
	    initial_value_rtx[regno] = NULL_RTX;
	  else
	    initial_value_rtx[regno] =
	      gen_rtx_fmt_e (INITIAL_VALUE,
			     mode, regno_reg_rtx[regno]);
	}

      iv_register_values = xrealloc (iv_register_values,
				     new_max_regno * sizeof (rtx));

      for (i = 0; i < current_loops->num; i++)
	if (current_loops->parray[i])
	  {
	    /* We do not reallocate loop_end_values at all, as they are only
	       used internally during biv analysis.  We also do not try to
	       determine the initial values, as saying "don't know" is
	       conservative and it is not probable that we will need to know
	       them.  */
	    modified_regs[i] = sbitmap_resize (modified_regs[i],
					       new_max_regno, 0);
	    initial_values[i] = xrealloc (initial_values[i],
					  sizeof (rtx) * new_max_regno);
	    loop_entry_values[i] = xrealloc (loop_entry_values[i],
					     sizeof (rtx) * new_max_regno);
	    for (regno = max_regno; regno < new_max_regno; regno++)
	      {
		if (!TEST_BIT (iv_interesting_reg, regno))
		  continue;

		if (i != 0
		    && flow_bb_inside_loop_p (current_loops->parray[i], bb))
		  loop_entry_values[i][regno] =
		    gen_value_at (regno,
				  current_loops->parray[i]->header->head,
				  false);
		else
		  loop_entry_values[i][regno] = gen_initial_value (regno);
		initial_values[i][regno] = NULL_RTX;
	      }
	  }
      df->reg_def_last = xrealloc (df->reg_def_last,
				   new_max_regno * sizeof (struct ref *));
      df_reg_table_realloc (df, new_max_regno);
      df->n_regs = new_max_regno;
      max_regno = new_max_regno;
    }

  if (df->def_id > df->n_defs)
    {
      bitmap defs = BITMAP_XMALLOC ();
      loop_rd_in_ok = sbitmap_resize (loop_rd_in_ok, df->def_id, 0);

      for (i = df->n_defs; i < df->def_id; i++)
	{
	  struct ref *def = df->defs[i];
	  bitmap_set_bit (defs, i);
	  df->regs[DF_REF_REGNO (def)].defs =
	    df_link_create (def, df->regs[DF_REF_REGNO (def)].defs);
	}
      fill_rd_for_defs (bb, defs);
      BITMAP_XFREE (defs);
      df->n_defs = df->def_id;
    }

  df->n_uses = df->use_id;

  insn_processed = sbitmap_resize (insn_processed, get_max_uid () + 1, 0);

  memset (df->reg_def_last, 0, max_regno * sizeof (struct ref *));
  for (x = bb->head; x != first; x = NEXT_INSN (x))
    {
      if (! INSN_P (x))
	continue;

      /* For each def in insn record the last def of each reg.  We could
	 instead pass reg-def chains for uses, which perhaps might be faster.  */
      for (def = DF_INSN_DEFS (df, x); def; def = def->next)
	{
	  struct ref *ref = def->ref;
	  int dregno = DF_REF_REGNO (ref);

	  df->reg_def_last[dregno] = ref;
	}
    }
  
  df_bb_luids_set (df, bb);
  for (x = first; x != NEXT_INSN (last); x = NEXT_INSN (x))
    {
      if (! INSN_P (x))
	continue;

      df_insn_ud_chain_create (df, bb, x);
      compute_reg_values (bb, x);
      record_iv_occurences (&iv_occurences[loop->num], x);
    }
}

/* Insert insns SEQ before INSN. The sequence must not contain jumps
   and must not set any registers that are not entirely new.  */
void
iv_emit_insn_before (seq, insn)
     rtx seq;
     rtx insn;
{
  rtx prev = PREV_INSN (insn);
  basic_block bb = BLOCK_FOR_INSN (insn);

  if (!seq)
    return;
  emit_insn_before (seq, insn);

  prev = prev ? NEXT_INSN (prev) : get_insns ();
  iv_new_insn_changes_commit (bb, prev, PREV_INSN (insn));
}

/* Insert insns SEQ before INSN. The sequence must not contain jumps
   and must not set any registers that are not entirely new.  */
void
iv_emit_insn_after (seq, insn)
     rtx seq;
     rtx insn;
{
  rtx next = NEXT_INSN (insn);
  basic_block bb = BLOCK_FOR_INSN (insn);

  if (!seq)
    return;
  emit_insn_after (seq, insn);
  next = next ? PREV_INSN (next) : get_last_insn ();
  iv_new_insn_changes_commit (bb, NEXT_INSN (insn), next);
}

/* The main entry point.  Run the analysis for all LOOPS starting from
   innermost ones.  */
void
analyse_induction_variables ()
{
  unsigned i, regno;
  rtx eq;
  struct loop *loop;
  basic_block bb;
  rtx insn, value;
  enum machine_mode mode, extended_mode;
  enum rtx_code extend;

  /* Compute register values in the first iteration.  */
  for (i = 0; i < current_loops->num; i++)
    if (current_loops->parray[i])
      {
	sbitmap_zero (modified_regs[i]);
	for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
	  loop_entry_values[i][regno] = gen_initial_value (regno);
      }
  compute_register_values (true);

  /* Now identify the induction variables.  */
  for (i = 1; i < current_loops->num; i++)
    {
      loop = current_loops->parray[i];
      if (!loop)
	continue;

      compute_loop_end_values (loop, loop_end_values[i]);
      EXECUTE_IF_SET_IN_SBITMAP (modified_regs[i], 0, regno,
	{
	  value = loop_end_values[i][regno];
	  if (!value)
	    eq = NULL_RTX;
	  else
	    {
	      mode = GET_MODE (value);
	      extend = GET_CODE (value);

	      /* We try to handle induction variables that in fact operate
		 in different mode than their register modes suggest.  I am
		 not really 100% sure that what we do here is correct (in
		 fact I am sure that it is not, just I don't know whether we
		 ever produce code that could cause us to behave incorrectly).

		 So what we do: if we for example see that from
		 INITIAL_VALUE:SI we got to SIGN_EXTEND:SI (something:HI),
		 we check whether something:HI - SUBREG:HI (INITIAL_VALUE:SI, 0)
		 is constant.  If it is, we assume this is such an induction
		 variable.  This is almost right, except for that in the
		 first iteration of the cycle, we did not have to have a value
		 in range of the narrower mode.  For now we ignore this
		 possibility; it should be easy to handle if it caused
		 problems -- we would just have to be a bit more cautious
		 about this first iteration.  */
		 
	      if (extend == SIGN_EXTEND || extend == ZERO_EXTEND)
		extended_mode = GET_MODE (XEXP (value, 0));
	      else
		extended_mode = mode;
	      eq = gen_rtx_fmt_ee (MINUS, mode,
				   copy_rtx (value),
				   gen_initial_value (regno));
	      if (extended_mode != mode)
		{
		  eq = gen_rtx_fmt_ei (SUBREG, extended_mode, eq, 0);
		  eq = gen_rtx_fmt_e (extend, mode, eq);
		}
	      eq = simplify_alg_expr (eq);
	      if (!invariant_wrto_ivs_p (eq, loop_end_values[i]))
		eq = NULL_RTX;
	      else
		{
		  eq = gen_rtx_fmt_ee (MULT, mode, eq, gen_iteration (mode));
		  eq = gen_rtx_fmt_ee (PLUS, mode,
				       gen_initial_value (regno), eq);
		  if (extended_mode != mode)
		    {
		      eq = gen_rtx_fmt_ei (SUBREG, extended_mode, eq, 0);
		      eq = gen_rtx_fmt_e (extend, mode, eq);
		    }
		  eq = simplify_alg_expr (eq);
		  if (!eq)
		    abort ();
		}
	    }
	  if (!eq)
	    eq = gen_value_at (regno, loop->header->head, false);
	  loop_entry_values[i][regno] = eq;
	});
    }

  /* Compute register values again, now including the ITERATION marks.  */
  compute_register_values (false);

  /* Compute the loop initial value information using the data computed above
     and simplify loop entry information using it.  We process the loops in
     the order determined by the dominance relation of their headers (so
     that when we refer to a value that is defined in a previous loop,
     we already know their initial values and we may use them).  */
  insn = ENTRY_BLOCK_PTR->succ->dest->head;
  for (regno = FIRST_PSEUDO_REGISTER; regno < max_regno; regno++)
    initial_values[0][regno] = gen_value_at (regno, insn, false);
  for (i = 1; i < real_loops_num; i++)
    {
      loop = loops_dominance_order[i];
      compute_initial_values (loop);
      simplify_ivs_using_values (loop_entry_values[loop->num],
				 initial_values[loop->num]);
    }

  /* Simplify values stored at insns using this knowledge.  */
  simplify_register_values ();

  /* Extract information about ivs, sorted by loop, step and base.  */
  FOR_EACH_BB (bb)
    {
      loop = bb->loop_father;
      if (!loop->outer)
	continue;

      FOR_BB_INSNS (bb, insn)
	{
	  if (INSN_P (insn))
	    record_iv_occurences (&iv_occurences[loop->num], insn);
	}
    }

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, ";; Induction variables:\n\n");

      for (i = 1; i < current_loops->num; i++)
	{
	  loop = current_loops->parray[i];
	  if(!loop)
	    continue;

	  fprintf (rtl_dump_file, ";; Loop %d:\n", i);
      	  fprintf (rtl_dump_file, ";;  initial values:\n");
	  dump_equations (rtl_dump_file, initial_values[loop->num]);
      	  fprintf (rtl_dump_file, ";;  induction variables at entry:\n");
	  dump_equations (rtl_dump_file, loop_entry_values[loop->num]);
	}
      fprintf (rtl_dump_file, "\n\n");

      FOR_EACH_BB (bb)
	{
	  fprintf (rtl_dump_file, ";; Basic block: %d (loop %d)\n\n",
		   bb->index, bb->loop_father->num);
	  FOR_BB_INSNS (bb, insn)
	    {
	      print_rtl_single (rtl_dump_file, insn);
	      if (INSN_P (insn))
		dump_insn_ivs (rtl_dump_file, insn);
	    }
	}
      fprintf (rtl_dump_file, "\n\nSorted:\n");
      for (i = 1; i < current_loops->num; i++)
	if (iv_occurences[i])
	  {
	    fprintf (rtl_dump_file, " Loop %d:\n", i);
	    dump_iv_occurences (rtl_dump_file, iv_occurences[i]);
	    fprintf (rtl_dump_file, "\n");
	  }
    }
}
