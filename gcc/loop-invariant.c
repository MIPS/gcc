/* Rtl-level loop invariant motion.
   Copyright (C) 2004 Free Software Foundation, Inc.
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* This implements the loop invariant motion pass.  It is very simple
   (no calls, libcalls, etc.).  This should be sufficient to cleanup things like
   address arithmetics -- other more complicated invariants should be
   eliminated on tree level either in tree-ssa-loop-im.c or in tree-ssa-pre.c.
   
   We proceed loop by loop -- it is simpler than trying to handle things
   globally and should not lose much; by avoiding looking inside already
   processed subloops (where we would not usually find anything useful anyway)
   we preserve the time complexity linear in number of insns.  First we inspect
   all sets inside loop and create a dependency graph on insns (saying "to move
   this insn, you must also move the following insns").

   We then need to determine what to move.  We estimate the number of registers
   used and move as many invariants as possible while we still have enough free
   registers.  We prefer the expensive invariants.
   
   Then we move the selected invariants out of the loop, creating a new
   temporaries for them if necessary.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "expr.h"
#include "output.h"
#include "function.h"
#include "flags.h"

/* The data stored for the loop.  */

struct loop_data
{
  bitmap modified_regs;		/* The registers modified inside the loop.  */
  struct loop *outermost_exit;	/* The outermost exit of the loop.  */
  bool has_call;		/* True if the loop contains a call.  */
};

#define LOOP_DATA(LOOP) ((struct loop_data *) (LOOP)->aux)

/* The definition of a register.  */

struct def
{
  bool simple_p;	/* Whether the definition may be simple.  */
  bool invar_rhs;	/* Whether the insn should be considered for invariant
			   motion.  */
  unsigned regno;	/* Number of the register defined.  */
  rtx insn;		/* The insn in that the definition occurs.  */
  basic_block bb;	/* Its basic block.  */
  unsigned invariant;	/* The number of the associated invariant.  */

  unsigned n_uses;	/* Number of uses.  */
  struct use *uses;	/* The list of uses.  */

  struct def *next;	/* Next definition.  */
  struct def *reg_next;	/* Next definition of the same register.  */
};

/* The description of an use.  */

struct use
{
  rtx *pos;			/* Position of the use.  */

  struct use *next;		/* Next use in the list.  */
};

/* The list of definitions.  */

static struct def *defs, *adef;
static struct def **last_def;

/* The data stored for each register.  */

struct reg
{
  bool simple_p;	/* Whether the register may be simple.  */
  bool used;		/* Whether the register is used at all.  */

  struct def *defs;	/* The definitions of the register.  */
  struct def **def_end;	/* End of the chain of the definitions.  */
  struct def *actual;	/* Actual definition of the register.  */
  basic_block last_bb;	/* The last basic block in that the register was
			   set.  */
};

static unsigned m_reg_info;
static struct reg *reg_info;

/* The data stored for each invariant.  */

struct invariant
{
  /* The number of the invariant.  */
  unsigned invno;

  /* Whether we already processed the invariant.  */
  bool processed;

  /* The definition of the invariant.  */
  struct def *def;

  /* Whether it is always executed.  */
  bool always_executed;

  /* Whether to move the invariant.  */
  bool move;

  /* Cost if the invariant.  */
  unsigned cost;

  /* The invariants it depends on.  */
  bitmap depends_on;

  /* Used for detecting already visited invariants during determining
     costs of movements.  */
  unsigned stamp;
};

/* The actual stamp for marking already visited invariants during determining
   costs of movements.  */

static unsigned actual_stamp;

/* The invariants.  */

static varray_type invariants;

/* Test for possibility of invariantness of X.  */

static bool
check_maybe_invariant (rtx x)
{
  enum rtx_code code = GET_CODE (x);
  int i, j;
  const char *fmt;

  switch (code)
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case SYMBOL_REF:
    case CONST:
    case LABEL_REF:
      return true;

    case PC:
    case CC0:
    case UNSPEC_VOLATILE:
      return false;

    case REG:
      if (HARD_REGISTER_P (x))
	{
	  /* TODO -- handling of hard regs.  */
	  return false;
	}

      return true;

    case MEM:
      /* Load/store motion is done elsewhere.  ??? Perhaps also add it here?
	 It should not be hard, and might be faster than "elsewhere".  */
      return false;

    case ASM_OPERANDS:
      /* Don't mess with insns declared volatile.  */
      if (MEM_VOLATILE_P (x))
	return false;
      break;

    default:
      break;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  if (!check_maybe_invariant (XEXP (x, i)))
	    return false;
	}
      else if (fmt[i] == 'E')
	{
	  for (j = 0; j < XVECLEN (x, i); j++)
	    if (!check_maybe_invariant (XVECEXP (x, i, j)))
	      return false;
	}
    }

  return true;
}

/* Determines the basic blocks inside LOOP that are always executed and
   stores their bitmap to ALWAYS_REACHED.  MAY_EXIT is a bitmap of
   basic blocks that may either exit the loop, or contain the call that
   does not have to return.  BODY is body of the loop obtained by
   get_loop_body_in_dom_order.  */

static void
compute_always_reached (struct loop *loop, basic_block *body,
			bitmap may_exit, bitmap always_reached)
{
  unsigned i;

  for (i = 0; i < loop->num_nodes; i++)
    {
      if (dominated_by_p (CDI_DOMINATORS, loop->latch, body[i]))
	bitmap_set_bit (always_reached, i);

      if (bitmap_bit_p (may_exit, i))
	return;
    }
}

/* Finds exits out of the LOOP with body BODY.  Marks blocks in that we may
   exit the loop by cfg edge to HAS_EXIT and MAY_EXIT.  In MAY_EXIT
   additionally mark blocks that may exit due to a call.  */

static void
find_exits (struct loop *loop, basic_block *body,
	    bitmap may_exit, bitmap has_exit)
{
  unsigned i;
  edge e;
  struct loop *outermost_exit = loop, *aexit;
  bool has_call = false;
  rtx insn;

  for (i = 0; i < loop->num_nodes; i++)
    {
      if (body[i]->loop_father == loop)
	{
	  FOR_BB_INSNS (body[i], insn)
	    {
	      if (GET_CODE (insn) == CALL_INSN
		  && !CONST_OR_PURE_CALL_P (insn))
		{
		  has_call = true;
		  bitmap_set_bit (may_exit, i);
		  break;
		}
	    }

	  for (e = body[i]->succ; e; e = e->succ_next)
	    {
	      if (flow_bb_inside_loop_p (loop, e->dest))
		continue;

	      bitmap_set_bit (may_exit, i);
	      bitmap_set_bit (has_exit, i);
	      outermost_exit = find_common_loop (outermost_exit,
						 e->dest->loop_father);
	    }
	  continue;
	}
     
      /* Use the data stored for the subloop to decide whether we may exit
	 through it.  It is sufficient to do this for header of the loop,
	 as other basic blocks inside it must be dominated by it.  */
      if (body[i]->loop_father->header != body[i])
	continue;

      if (LOOP_DATA (body[i]->loop_father)->has_call)
	{
	  has_call = true;
	  bitmap_set_bit (may_exit, i);
	}
      aexit = LOOP_DATA (body[i]->loop_father)->outermost_exit;
      if (aexit != loop)
	{
	  bitmap_set_bit (may_exit, i);
	  bitmap_set_bit (has_exit, i);

	  if (flow_loop_nested_p (aexit, outermost_exit))
	    outermost_exit = aexit;
	}
    }

  loop->aux = xcalloc (1, sizeof (struct loop_data));
  LOOP_DATA (loop)->outermost_exit = outermost_exit;
  LOOP_DATA (loop)->has_call = has_call;
}

/* Record a definition of a register REGNO in INSN in basic block BB,
   simple if SIMPLE_P.  Rhs of the definition may be considered for
   moving if INVAR_RHS is true.  */

static void
record_def (unsigned regno, rtx insn, basic_block bb, bool simple_p,
	    bool invar_rhs)
{
  struct def *def;
  basic_block last_bb;

  if (!reg_info[regno].simple_p)
    return;
  last_bb = reg_info[regno].last_bb;

  if (!dominated_by_p (CDI_DOMINATORS, bb, last_bb))
    {
      reg_info[regno].simple_p = false;
      return;
    }

  def = xmalloc (sizeof (struct def));
  def->simple_p = simple_p;
  def->invar_rhs = invar_rhs;
  def->regno = regno;
  def->insn = insn;
  def->bb = bb;
  def->next = NULL;
  def->invariant = 0;
  def->uses = NULL;
  def->n_uses = 0;
  def->reg_next = NULL;

  *last_def = def;
  last_def = &def->next;

  *reg_info[regno].def_end = def;
  reg_info[regno].def_end = &def->reg_next;
  reg_info[regno].last_bb = bb;
}

/* Check whether we may assign a value to X from a register.  */

static bool
may_assign_reg_p (rtx x)
{
#ifdef AVOID_CCMODE_COPIES
  if (GET_MODE_CLASS (GET_MODE (x)) == MODE_CC)
    return false;
#endif

  return true;
}

/* Notes the registers clobbered by the NIS_INSN.  Callback for note_stores.
   If not NIS_MAY_BE_SIMPLE, the set may not be considered simple.  */

static bool nis_may_be_simple;
static rtx nis_insn;
static void
note_insn_stores (rtx reg, rtx set, void *data ATTRIBUTE_UNUSED)
{
  bool simple = true, consider_invariantness = true, should_record_def = true;
  unsigned regno = 0;
  rtx areg = SET_DEST (set);

  if (!nis_may_be_simple)
    {
      simple = false;
      consider_invariantness = false;
    }

  if (!REG_P (areg))
    {
      simple = false;
      if (GET_CODE (reg) == SUBREG)
	reg = SUBREG_REG (reg);
    }

  if (!REG_P (reg)
      || HARD_REGISTER_P (reg))
    {
      /* It is not necessary to record the definition in this case,
	 since we are not going to use it.  */
      should_record_def = false;
    }
  else
    regno = REGNO (reg);

  if (!reg_info[regno].simple_p)
    should_record_def = false;

  if (GET_CODE (set) != SET
      || set != single_set (nis_insn)
      || !check_maybe_invariant (SET_SRC (set))
      || !may_assign_reg_p (SET_DEST (set))
      /* Until we get rid of LIBCALLS.  */
      || find_reg_note (nis_insn, REG_RETVAL, NULL_RTX)
      || find_reg_note (nis_insn, REG_LIBCALL, NULL_RTX)
      || find_reg_note (nis_insn, REG_NO_CONFLICT, NULL_RTX))
    {
      consider_invariantness = false;
      simple = false;
    }

  if (should_record_def || consider_invariantness)
    record_def (regno, nis_insn, BLOCK_FOR_INSN (nis_insn), simple,
		consider_invariantness);
}

/* Finds definitions that may correspond to invariants inside INSN.
   ALWAYS_REACHED is true if the insn is always reached.  If IS_NOT_SIMPLE,
   the set may not be simple.  */

static void
find_defs_insn (rtx insn, bool always_reached, bool is_not_simple)
{
  nis_may_be_simple = !is_not_simple;

  if (!always_reached
      && may_trap_p (insn))
    nis_may_be_simple = false;

  nis_insn = insn;
  note_stores (PATTERN (insn), note_insn_stores, NULL);
}

/* Finds definitions that may correspond to invariants inside basic block BB.
   ALWAYS_REACHED is true if the entry of the basic block is always
   reached.  */

static void
find_defs_bb (basic_block bb, bool always_reached)
{
  rtx insn;

  FOR_BB_INSNS (bb, insn)
    {
      if (!INSN_P (insn))
	continue;

      if (always_reached
	  && GET_CODE (insn) == CALL_INSN
	  && !CONST_OR_PURE_CALL_P (insn))
	always_reached = false;

      find_defs_insn (insn, always_reached,
		      (bb->flags & BB_IRREDUCIBLE_LOOP) != 0);
    }
}

/* Finds definitions that may correspond to invariants in LOOP with body BODY.
   ALWAYS_REACHED is the bitmap of basic blocks in BODY whose entry is always
   reached.  */

static void
find_defs (struct loop *loop, basic_block *body, bitmap always_reached)
{
  unsigned i, regno;
  bitmap modified_regs;

  for (i = 0; i < loop->num_nodes; i++)
    {
      if (body[i]->loop_father == loop)
	{
	  find_defs_bb (body[i], bitmap_bit_p (always_reached, i));
	  continue;
	}

      if (body[i]->loop_father->header != body[i])
	continue;

      modified_regs = LOOP_DATA (body[i]->loop_father)->modified_regs;
      EXECUTE_IF_SET_IN_BITMAP (modified_regs, 0, regno,
	{
	  record_def (regno, const0_rtx, body[i], false, false);
	});
    }
}

/* Creates a new invariant for definition DEF, depending on invariants in
   DEPENDS_ON.  ALWAYS_EXECUTED is true if the insn is always executed,
   unless the program ends due to a function call.  */

static void
create_new_invariant (struct def *def, bitmap depends_on, bool always_executed)
{
  struct invariant *inv = xmalloc (sizeof (struct invariant));
  rtx set = single_set (def->insn);

  inv->def = def;
  inv->always_executed = always_executed;
  inv->depends_on = depends_on;

  /* If the set is simple, usually by moving it we move the whole store out of
     the loop.  Otherwise we save only cost of the computation.  */
  if (def->simple_p)
    inv->cost = rtx_cost (set, SET);
  else
    inv->cost = rtx_cost (SET_SRC (set), SET);

  inv->move = false;
  inv->processed = false;
  inv->stamp = 0;

  inv->invno = def->invariant = VARRAY_ACTIVE_SIZE (invariants);
  VARRAY_PUSH_GENERIC_PTR_NOGC (invariants, inv);

  if (rtl_dump_file)
    {
      if (def->simple_p)
	fprintf (rtl_dump_file, "Set of register %d", def->regno);
      else
	fprintf (rtl_dump_file, "Set");
      fprintf (rtl_dump_file,
	       " in insn %d is invariant (%d), cost %d, depends on ",
	       INSN_UID (def->insn), def->invariant, inv->cost);
      dump_bitmap (rtl_dump_file, inv->depends_on);
    }
}

/* Record USE at DEF.  */

static void
record_use (struct def *def, rtx *use)
{
  struct use *u = xmalloc (sizeof (struct use));

  u->pos = use;
  u->next = def->uses;
  def->uses = u;
  def->n_uses++;
}

/* Gets current definition of REG in INSN and stores it to DEF.
   If it cannot be (uniquely) determined, or the def is not simple,
   false is returned.  */

static bool
get_current_def (rtx reg, rtx insn, struct def **def)
{
  unsigned regno = REGNO (reg);
  basic_block bb;

  if (!reg_info[regno].simple_p)
    return false;

  if (!reg_info[regno].actual)
    {
      /* Only succeed if there is no def of the register at all.  */
      if (reg_info[regno].defs)
	return false;

      *def = NULL;
      return true;
    }

  *def = reg_info[regno].actual;
  if (!(*def)->simple_p)
    return false;

  bb = BLOCK_FOR_INSN (insn);
  if (!dominated_by_p (CDI_DOMINATORS, bb, (*def)->bb))
    return false;

  if (!(*def)->reg_next)
    return true;

  /* Check that the use dominates the next definition.  This ensures that
     it indeed must be reached by the actual one.  */
  if (!dominated_by_p (CDI_DOMINATORS, (*def)->reg_next->bb, bb))
    return false;

  /* Still we could have a problem if the use is in the irreducible loop
     together with the next use.  */
  if ((bb->flags & BB_IRREDUCIBLE_LOOP)
      && !(*def)->reg_next->simple_p)
    return false;

  return true;
}

/* Records dependencies and uses in X.  Dependencies are marked into
   bitmap passed in DATA.  If we depend on non-invariant, RD_FAILED
   is set to true.  RD_INSN is the current insn.  Callback for
   for_each_rtx.  */

static bool rd_failed;
static rtx rd_insn;
static int
record_dependencies_fer (rtx *x, void *data)
{
  bitmap depends_on = data;
  struct def *def = NULL;

  if (!REG_P (*x))
    return 0;

  reg_info[REGNO (*x)].used = true;
  if (!get_current_def (*x, rd_insn, &def))
    {
      if (depends_on)
	rd_failed = true;
      return 0;
    }

  if (!def)
    return 0;

  if (depends_on)
    bitmap_set_bit (depends_on, def->invariant);

  record_use (def, x);
  return 0;
}

/* Records dependencies and uses in X.  Dependencies are marked into
   bitmap passed in DATA.  If we depend on non-invariant, RD_FAILED
   is set to true.  Callback for note_uses.  */

static void
record_dependencies (rtx *x, void *data)
{
  for_each_rtx (x, record_dependencies_fer, data);
}

/* Finds invariants in INSN.  ALWAYS_EXECUTED is true if the insn is always
   executed, unless the program ends due to a function call.  */

static void
find_invariants_insn (rtx insn, bool always_executed)
{
  struct def *def;
  bitmap depends_on;

  for (def = adef; def && def->insn == insn; def = def->next)
    if (def->invar_rhs)
      break;

  if (def && def->insn == insn)
    depends_on = BITMAP_XMALLOC ();
  else
    depends_on = NULL;

  rd_failed = false;
  rd_insn = insn;
  note_uses (&PATTERN (insn), record_dependencies, depends_on);
  if (rd_failed)
    {
      BITMAP_XFREE (depends_on);
      def->simple_p = false;
      return;
    }
  if (!depends_on)
    return;

  create_new_invariant (def, depends_on, always_executed);
}

/* Moves the pointers to the actual definition for defs in INSN.  */

static void
move_actual_defs (rtx insn)
{
  for (; adef && adef->insn == insn; adef = adef->next)
    reg_info[adef->regno].actual = adef;
}

/* Finds invariants in basic block BB.  ALWAYS_EXECUTED is true if the basic
   block is always executed, unless the program ends due to a function
   call.  */

static void
find_invariants_bb (basic_block bb, bool always_executed)
{
  rtx insn;

  FOR_BB_INSNS (bb, insn)
    {
      if (!INSN_P (insn))
	continue;

      find_invariants_insn (insn, always_executed);
      move_actual_defs (insn);
    }
}

/* Finds invariants in LOOP with body BODY.  ALWAYS_EXECUTED is the bitmap of
   basic blocks in BODY that are always executed, unless the program ends due
   to a function call.  */

static void
find_invariants_body (struct loop *loop, basic_block *body,
		      bitmap always_executed)
{
  unsigned i;

  adef = defs;
  for (i = 0; i < loop->num_nodes; i++)
    {
      if (body[i]->loop_father == loop)
	find_invariants_bb (body[i], bitmap_bit_p (always_executed, i));
      else
	move_actual_defs (const0_rtx);
    }
}

/* Finds invariants in LOOP.  */

static void
find_invariants (struct loop *loop)
{
  bitmap may_exit = BITMAP_XMALLOC ();
  bitmap always_reached = BITMAP_XMALLOC ();
  bitmap has_exit = BITMAP_XMALLOC ();
  bitmap always_executed = BITMAP_XMALLOC ();
  basic_block *body = get_loop_body_in_dom_order (loop);

  find_exits (loop, body, may_exit, has_exit);
  compute_always_reached (loop, body, may_exit, always_reached);
  compute_always_reached (loop, body, has_exit, always_executed);

  find_defs (loop, body, always_reached);
  find_invariants_body (loop, body, always_executed);

  BITMAP_XFREE (always_reached);
  BITMAP_XFREE (always_executed);
  BITMAP_XFREE (may_exit);
  BITMAP_XFREE (has_exit);
  free (body);
}

/* Frees a list of uses USE.  */

static void
free_use_list (struct use *use)
{
  struct use *next;

  for (; use; use = next)
    {
      next = use->next;
      free (use);
    }
}

/* Calculates cost and number of registers needed for moving invariant INV
   out of the loop and stores them to *COST and *REGS_NEEDED.  */

static void
get_inv_cost (struct invariant *inv, int *comp_cost, unsigned *regs_needed)
{
  int acomp_cost;
  unsigned aregs_needed;
  unsigned depno;
  struct invariant *dep;

  *comp_cost = 0;
  *regs_needed = 0;
  if (inv->move
      || inv->stamp == actual_stamp)
    return;
  inv->stamp = actual_stamp;

  (*regs_needed)++;
  (*comp_cost) += inv->cost;

  /* TODO -- in case there is more than one path to the dependency, we count
     its cost multiple times (and we also might have an exponential time
     complexity).  Not really likely to occur in practice.  */
  EXECUTE_IF_SET_IN_BITMAP (inv->depends_on, 0, depno,
    {
      dep = VARRAY_GENERIC_PTR_NOGC (invariants, depno);

      get_inv_cost (dep, &acomp_cost, &aregs_needed);

      if (aregs_needed
	  && dep->always_executed
	  && !dep->def->uses->next)
	{
	  /* If this is a single use, after moving the dependency we will not
	     need a new register.  */
	  aregs_needed--;
	}

      (*regs_needed) += aregs_needed;
      (*comp_cost) += acomp_cost;
    });
}

/* Calculates gain for eliminating invariant INV.  REGS_USED is the number
   of registers used in the loop, N_INV_USES is the number of uses of
   invariants, NEW_REGS is the number of new variables already added due to
   the invariant motion.  The number of registers needed for it is stored in
   *REGS_NEEDED.  */

static int
gain_for_invariant (struct invariant *inv, unsigned *regs_needed,
		    unsigned new_regs, unsigned regs_used, unsigned n_inv_uses)
{
  int comp_cost, size_cost;

  get_inv_cost (inv, &comp_cost, regs_needed);
  actual_stamp++;

  size_cost = (global_cost_for_size (new_regs + *regs_needed,
				     regs_used, n_inv_uses)
	       - global_cost_for_size (new_regs, regs_used, n_inv_uses));

  return comp_cost - size_cost;
}

/* Finds invariant with best gain for moving.  Returns the gain, stores
   the invariant in *BEST and number of registers needed for it to
   *REGS_NEEDED.  REGS_USED is the number of registers used in
   the loop, N_INV_USES is the number of uses of invariants.  NEW_REGS
   is the number of new variables already added due to invariant motion.  */

static int
best_gain_for_invariant (struct invariant **best, unsigned *regs_needed,
			 unsigned new_regs, unsigned regs_used,
			 unsigned n_inv_uses)
{
  struct invariant *inv;
  int gain = 0, again;
  unsigned aregs_needed, invno;

  for (invno = 0; invno < VARRAY_ACTIVE_SIZE (invariants); invno++)
    {
      inv = VARRAY_GENERIC_PTR_NOGC (invariants, invno);
      if (inv->move)
	continue;

      again = gain_for_invariant (inv, &aregs_needed,
				  new_regs, regs_used, n_inv_uses);
      if (again > gain)
	{
	  gain = again;
	  *best = inv;
	  *regs_needed = aregs_needed;
	}
    }

  return gain;
}

/* Marks invariant INVNO and all its dependencies for moving.  */

static void
set_move_mark (unsigned invno)
{
  struct invariant *inv = VARRAY_GENERIC_PTR_NOGC (invariants, invno);

  if (inv->move)
    return;
  inv->move = true;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Decided to move invariant %d\n", invno);

  EXECUTE_IF_SET_IN_BITMAP (inv->depends_on, 0, invno, set_move_mark (invno));
}

/* Determines which invariants to move.  */

static void
find_invariants_to_move (void)
{
  unsigned i, regs_used, n_inv_uses, regs_needed = 0, new_regs;
  struct invariant *inv = NULL;

  if (flag_move_all_movables)
    {
      /* This is easy & stupid.  */
      for (i = 0; i < VARRAY_ACTIVE_SIZE (invariants); i++)
	{
	  inv = VARRAY_GENERIC_PTR_NOGC (invariants, i);
	  inv->move = true;
	}
      return;
    }

  if (!VARRAY_ACTIVE_SIZE (invariants))
    return;

  /* Now something slightly more involved.  First estimate the number of used
     registers.  */
  n_inv_uses = 0;

  /* We do not really do a good job in this estimation; put some initial bound
     here to stand for induction variables etc. that we do not detect.  */
  regs_used = 2;

  for (i = 0; i < (unsigned) max_reg_num (); i++)
    {
      if (reg_info[i].simple_p
	  && !reg_info[i].defs
	  && reg_info[i].used)
	{
	  /* This is a value that is used but not changed inside loop.  */
	  regs_used++;
	}
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (invariants); i++)
    {
      inv = VARRAY_GENERIC_PTR_NOGC (invariants, i);
      n_inv_uses += inv->def->n_uses;
    }

  new_regs = 0;
  while (best_gain_for_invariant (&inv, &regs_needed,
				  new_regs, regs_used, n_inv_uses) > 0)
    {
      set_move_mark (inv->invno);
      new_regs += regs_needed;
    }
}

/* Move invariant INVNO out of the LOOP.  */

static void
move_invariant_reg (struct loop *loop, unsigned invno)
{
  struct invariant *inv = VARRAY_GENERIC_PTR_NOGC (invariants, invno);
  unsigned i;
  basic_block preheader = loop_preheader_edge (loop)->src;
  rtx reg, set;
  struct use *use;

  if (inv->processed)
    return;
  inv->processed = true;

  if (inv->depends_on)
    {
      EXECUTE_IF_SET_IN_BITMAP (inv->depends_on, 0, i,
	{
	  move_invariant_reg (loop, i);
	});
    }

  /* Move the set out of the loop.  If the set is always executed (we could
     omit this condition if we know that the register is unused outside of the
     loop, but it does not seem worth finding out) and it has no uses that
     would not be dominated by it, we may just move it (TODO).  Otherwise we
     need to create a temporary register.  */
  set = single_set (inv->def->insn);
  reg = gen_reg_rtx (GET_MODE (SET_DEST (set)));
  emit_insn_after (gen_move_insn (SET_DEST (set), reg), inv->def->insn);
  SET_DEST (set) = reg;
  reorder_insns (inv->def->insn, inv->def->insn, BB_END (preheader));

  /* Replace the uses we know to be dominated.  It saves work for copy
     propagation, and also it is necessary so that dependent invariants
     are computed right.  */
  for (use = inv->def->uses; use; use = use->next)
    *use->pos = reg;
}

/* Move selected invariant out of the LOOP.  Newly created regs are marked
   in TEMPORARY_REGS.  */

static void
move_invariants (struct loop *loop)
{
  struct invariant *inv;
  unsigned i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (invariants); i++)
    {
      inv = VARRAY_GENERIC_PTR_NOGC (invariants, i);
      if (inv->move)
	move_invariant_reg (loop, i);
    }
}

/* Initializes invariant motion data for LOOP.  MAX_REGNO is the maximum
   register number.  */

static void
init_inv_motion_data (struct loop *loop, unsigned max_regno)
{
  unsigned regno;

  if (m_reg_info < max_regno)
    {
      m_reg_info = 2 * max_regno;
      reg_info = xrealloc (reg_info, m_reg_info * sizeof (struct reg));
    }

  for (regno = 0; regno < max_regno; regno++)
    {
      reg_info[regno].simple_p = true;
      reg_info[regno].used = false;
      reg_info[regno].defs = NULL;
      reg_info[regno].def_end = &reg_info[regno].defs;
      reg_info[regno].actual = NULL;
      reg_info[regno].last_bb = loop_preheader_edge (loop)->src;
    }

  defs = NULL;
  last_def = &defs;

  actual_stamp = 1;

  if (!invariants)
    VARRAY_GENERIC_PTR_NOGC_INIT (invariants, 100, "invariants");
}

/* Frees the data allocated by invariant motion.  MAX_REGNO is the maximum
   register number before optimization.  Store registers that are set inside
   the loop in MODIFIED_REGS.  */

static void
free_inv_motion_data (unsigned max_regno, bitmap modified_regs)
{
  unsigned regno, i;
  struct def *def, *next;
  struct invariant *inv;

  for (regno = 0; regno < max_regno; regno++)
    {
      if (!reg_info[regno].simple_p
	  || reg_info[regno].defs)
	bitmap_set_bit (modified_regs, regno);
    }

  for (def = defs; def; def = next)
    {
      next = def->next;
      free_use_list (def->uses);
      free (def);
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (invariants); i++)
    {
      inv = VARRAY_GENERIC_PTR_NOGC (invariants, i);
      BITMAP_XFREE (inv->depends_on);
      free (inv);
    }
  VARRAY_POP_ALL (invariants);
}

/* Move the invariants out of the LOOP.  */

static void
move_single_loop_invariants (struct loop *loop)
{
  unsigned max_regno = max_reg_num ();

  init_inv_motion_data (loop, max_regno);
  find_invariants (loop);
  find_invariants_to_move ();
  move_invariants (loop);

  LOOP_DATA (loop)->modified_regs = BITMAP_XMALLOC ();
  free_inv_motion_data (max_regno, LOOP_DATA (loop)->modified_regs);
}

/* Releases the auxiliary data for LOOP.  */

static void
free_loop_data (struct loop *loop)
{
  struct loop_data *data = LOOP_DATA (loop);

  BITMAP_XFREE (data->modified_regs);
  loop->aux = NULL;
}

/* Move the invariants out of the LOOPS.  */

void
move_loop_invariants (struct loops *loops)
{
  struct loop *loop;
  unsigned i;

  if (!reg_info)
    {
      m_reg_info = 2 * max_reg_num ();
      reg_info = xmalloc (m_reg_info * sizeof (struct reg));
    }

  /* Process the loops, innermost first.  */
  loop = loops->tree_root;
  while (loop->inner)
    loop = loop->inner;

  while (loop != loops->tree_root)
    {
      move_single_loop_invariants (loop);

      if (loop->next)
	{
	  loop = loop->next;
	  while (loop->inner)
	    loop = loop->inner;
	}
      else
	loop = loop->outer;
    }

  for (i = 1; i < loops->num; i++)
    if (loops->parray[i])
      free_loop_data (loops->parray[i]);
}
