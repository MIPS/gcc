/* Select alternatives for insns and register classes for each pseudo.
   Copyright (C) 2006  Free Software Foundation, Inc.

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tm_p.h"
#include "insn-config.h"
#include "expr.h"
#include "optabs.h"
#include "recog.h"
#include "reload.h"
#include "regs.h"
#include "addresses.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "real.h"
#include "df.h"
#include "obstack.h"
#include "output.h"
#include "function.h"
#include "toplev.h"
#include "params.h"
#include "target.h"
#include "timevar.h"
#include "tree-pass.h"

/* This pass runs directly before register allocation and is responsible
   for selecting a register class for each pseudo register and an alternative
   for each instruction.  In cases where it's not possible to define
   one register class for a pseudo (e.g. one used in integer and float
   context) compensation code will be added so that it will be possible.
   That will mostly be simple move instructions into and out of new
   pseudo registers.  */

struct reg_ref
{
  /* Assigned register class for this reference.  -1 if not yet assigned.  */
  int class;
  /* The operand in which this regref occured.  */
  rtx op;
  /* The number of the operand.  */
  int op_num;
  /* The number of match_dup refering to this operand.  -1 if a normal
     match_operand.  */
  int dup;
};

/* XXX It's possible the whole pass will be more natural if we would have
   a mapping of (insn -> insns operands) where each operand would
   have a list of all reg_ref.  Might make handling of constraints per operand
   easier.  Perhaps also dealing with match_dups would be easier.  */

#define UNASSIGNED -1

/* Mapping from insn UID to alternative for that insn.  -1 if not
   yet selected.  */
static int *uid2alt;
static struct df *df;
static struct obstack si_obstack;

/* Sets the goal alternative of INSN to ALTERNATIVE.  This updates
   the selected regclasses for all register references in this INSN.
   This functions requires having preprocess_constraints called.  */
static void
assign_alt_to_insn (rtx insn, int alternative)
{
  int pass;
  struct df_ref *ref;
  uid2alt[INSN_UID (insn)] = alternative;
  for (pass = 0; pass < 2; pass++)
    for (ref = (pass == 0)
	       ? DF_INSN_DEFS (df, insn)
	       : DF_INSN_USES (df, insn);
	 ref;
	 ref = ref->next_ref)
      {
	struct reg_ref *r = DF_REF_DATA (ref);
	if (r->dup >= 0)
	  {
	    if (dump_file)
	      {
		print_inline_rtx (dump_file, DF_REF_REG (ref), 4);
	        fprintf (dump_file, "     is a match_dup\n");
	      }
	    continue;
	  }
	if (!r->op)
	  abort ();
	/* If the operand to which this ref belongs is itself a register
	   then this ref's class is the class of this operand.  */
	if (REG_P (r->op)
	    || (GET_CODE (r->op) == SUBREG && REG_P (SUBREG_REG (r->op))))
	  {
	    struct operand_alternative *op_alt;
	    op_alt = &recog_op_alt[r->op_num][alternative];
	    while (op_alt->matches >= 0)
	      op_alt = &recog_op_alt[op_alt->matches][alternative];
	    r->class = op_alt->cl;
	  }
	/* Otherwise the op is no register but contains this register,
	   hence it's something more complicated, like a MEM or a mem
	   address.  We need backends help in determining which class
	   this regref needs, at least in more complicated cases.  XXX */
	else
	  {
	    /* XXX For now let's use GENERAL_REGS.  It's wrong, and we could
	       do better even without backend help, like analysing the
	       structure, and if it's a MEM or an address assign BASE_REGS
	       or INDEX_REGS, and similar tricks.  */
	    r->class = GENERAL_REGS;
	  }
	if (dump_file)
	  {
	    print_inline_rtx (dump_file, DF_REF_REG (ref), 4);
	    fprintf (dump_file, "     -> %s\n", reg_class_names[r->class]);
	  }
      }
}

/* Set's up some basic information for the given INSN.  Like the initial
   insn alternative, the local info per register ref and such.  */
static void
init_insn (rtx insn)
{
  int alt, valid, n_ops, i;
  struct df_ref *ref;

  /* Allocate the local info for all refs in this insn.  */
  for (ref = DF_INSN_DEFS (df, insn); ref; ref = ref->next_ref)
    {
      struct reg_ref *r = obstack_alloc (&si_obstack, sizeof (struct reg_ref));
      r->class = -1;
      r->op = NULL_RTX;
      r->op_num = -1;
      r->dup = -1;
      DF_REF_DATA (ref) = r;
    }
  for (ref = DF_INSN_USES (df, insn); ref; ref = ref->next_ref)
    {
      struct reg_ref *r = obstack_alloc (&si_obstack, sizeof (struct reg_ref));
      r->class = -1;
      r->op = NULL_RTX;
      r->op_num = -1;
      r->dup = -1;
      DF_REF_DATA (ref) = r;
    }
  extract_insn (insn);
  n_ops = recog_data.n_operands;

  /* Determine to which operand each register ref belongs.  */
  for (i = 0; i < n_ops; i++)
    if (*recog_data.constraints[i])
      {
	int pass;
	for (pass = 0; pass < 2; pass++)
	  for (ref = (pass == 0)
	             ? DF_INSN_DEFS (df, insn)
	             : DF_INSN_USES (df, insn);
	       ref;
	       ref = ref->next_ref)
	    if (recog_data.operand_loc[i] == DF_REF_LOC (ref)
		|| loc_mentioned_in_p (DF_REF_LOC (ref),
				       *recog_data.operand_loc[i]))
	      {
		struct reg_ref * r = DF_REF_DATA (ref);
		r->op = recog_data.operand[i];
		r->op_num = i;
	      }
      }
  for (i = 0; i < recog_data.n_dups; i++)
    {
      int pass;
      for (pass = 0; pass < 2; pass++)
	for (ref = (pass == 0)
	           ? DF_INSN_DEFS (df, insn)
		   : DF_INSN_USES (df, insn);
	     ref;
	     ref = ref->next_ref)
	  if (recog_data.dup_loc[i] == DF_REF_LOC (ref)
	      || loc_mentioned_in_p (DF_REF_LOC (ref),
				     *recog_data.dup_loc[i]))
	    {
	      struct reg_ref * r = DF_REF_DATA (ref);
	      r->op = recog_data.operand[(int)recog_data.dup_num[i]];
	      r->op_num = recog_data.dup_num[i];
	      r->dup = i;
	    }
    }

  /* Now determine the initial alternative.  */
  /* XXX We probably need our own constrain_operands version, so that we
     can callback to our code whenever we find a certain reg requiring a
     certain class.  This also enables usage of regclasses in already
     looked at references in determining which alternative to chose.  */
  valid = constrain_operands (0);
  alt = which_alternative;
  preprocess_constraints ();
  if (dump_file)
    fprintf (dump_file, " %d ", INSN_UID (insn));
  if (recog_data.n_alternatives == 0 || n_ops == 0)
    {
      /* has no constrained operands, i.e. must be valid */
      if (!valid)
	abort ();
    }
  if (valid)
    {
      if (dump_file)
	fprintf (dump_file, "   --> matched alternative %d\n", alt);
      assign_alt_to_insn (insn, alt);
    }
  else
    {
      fprintf (dump_file, "  --> invalid insn");
      fprintf (dump_file, "\n");
    }
}

static void
init_bb (basic_block bb)
{
  rtx insn;
  if (dump_file)
    fprintf (dump_file, "init_bb (%d) [freq:%d]\n", bb->index, bb->frequency);
  FOR_BB_INSNS (bb, insn)
    {
      /* XXX Defer the handling of move insns until after all other
	 instructions are dealt with.  Also first look at those instructions
         which only have one alternative.  Those can determine the regclass
         for some regs quite specific.  */
      if (INSN_P (insn))
	{
	  enum rtx_code pat_code;
	  pat_code = GET_CODE (PATTERN (insn));
	  switch (pat_code)
	    {
	      /* Ignore non-insns.  */
	      case USE: case CLOBBER: case ASM_INPUT: case ADDR_VEC:
	      case ADDR_DIFF_VEC:
		break;
	      default:
		init_insn (insn);
		break;
	    }
	}
    }
  if (dump_file)
    fprintf (dump_file, "\n");
}

static int
bb_cost_cmp (const void *a, const void *b)
{
  basic_block ab = *(basic_block *)a;
  basic_block bb = *(basic_block *)b;
  if (ab->frequency > bb->frequency)
    return -1;
  else if (ab->frequency < bb->frequency)
    return 1;
  return ab->index - bb->index;
}

static unsigned int
do_select (void)
{
  basic_block *bbs, bb;
  int i, num_bb;
  df = df_init (DF_SUBREGS);
  df_analyze (df);
  gcc_obstack_init (&si_obstack);
  uid2alt = xmalloc (get_max_uid () * sizeof (uid2alt[0]));
  memset (uid2alt, -1, get_max_uid () * sizeof (uid2alt[0]));
  bbs = xmalloc (n_basic_blocks * sizeof (bbs[0]));
  num_bb = 0;
  FOR_EACH_BB (bb)
    bbs[num_bb++] = bb;
  qsort (bbs, num_bb, sizeof (bbs[0]), bb_cost_cmp);
  if (dump_file)
    df_dump (df, dump_file);
  for (i = 0; i < num_bb; i++)
    init_bb (bbs[i]);

  free (bbs);
  free (uid2alt);
  obstack_free (&si_obstack, NULL);
  df_finish (df);
  return 0;
}

struct tree_opt_pass pass_insn_select =
{
  "isel",                               /* name */
  NULL,                                 /* gate */
  do_select,                            /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_LOCAL_ALLOC,                       /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func,                       /* todo_flags_finish */
  's'                                   /* letter */
};

/*
vim:cinoptions={.5s,g0,p5,t0,(0,^-0.5s,n-0.5s:tw=78:cindent:sw=4:
*/
