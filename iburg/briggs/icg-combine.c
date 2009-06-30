/* Combines insns via forward propagation to create maximally sized
   RTL expressions for ICG.  Copyright (C) 2008 Free Software
   Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3, or (at your option) any later
   version.

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "flags.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "df.h"


/* Integrated code generator combine.

   Combines RTL insns in to maximally sized expression trees.  These
   expression trees are input to the combined instruction selector and
   graph-coloring register allocator. */


#ifdef DEBUG
/* Dump data for scraping via a script. */
#define COMBINE_LOG_DATA(f, d) (dump_file ? fprintf (dump_file, "COMBINE_DATA %s: %d\n", (f), (d)) : 0)
#define COMBINE_LOG(s)  (dump_file ? fprintf (dump_file, "COMBINE_LOG %s\n", (s)) : 0)
#else
#define COMBINE_LOG_DATA(f, d) (dump_file ? fprintf (dump_file, "    %s: %d\n", (f), (d)) : 0)
#define COMBINE_LOG(s)  (dump_file ? fprintf (dump_file, "%s\n", (s)) : 0)
#endif


/* Data structure for accumulating the number of leaves and types in
   an insn.  A leaf is a register, constant, or memory reference.
   Does not count the register destination of a SET.  This data
   structure and related functions are only used for data collection,
   not for an transformations.  */

struct leaf_counts
{
  int leaves;
  int mem;
  int con;
  int reg;
  int reg_single_live;
  int reg_single_dead;
  int reg_mult_live;
  int reg_mult_dead;
};

static void dump_def_use (FILE *, basic_block);
static void log_bb_data (basic_block, const char *);
static void insn_count_leaves (rtx, struct leaf_counts *);
static void log_leaves (const char *, struct leaf_counts *);


static bool
is_reg_or_subreg_p (rtx expr)
{
  return (REG_P (expr) || GET_CODE (expr) == SUBREG);
}

static rtx
get_reg_rtx (rtx expr)
{
  gcc_assert (is_reg_or_subreg_p (expr));
  if (REG_P (expr))
    return expr;
  else
    return SUBREG_REG (expr);
}

/* Returns true iff EXPR is a real register or subreg of real
   register.  If EXCEPT_CC is true, then doesn't count CC as a real
   register.  */

static bool
real_reg_or_subreg_p (rtx expr, bool except_cc)
{
  if (is_reg_or_subreg_p (expr))
    {
      rtx reg = get_reg_rtx (expr);
      return (REGNO (reg) < FIRST_PSEUDO_REGISTER
	      && !(except_cc && REGNO (reg) == FLAGS_REG));
    }
  return false;
}

/* Returns true iff INSN is a copy involving a machine register */

static bool
copy_mach_reg_p (rtx insn)
{
  /* The opcode of the insn is a register-to-register copy (reg or
     subreg) and one of the operands is a machine reg (or subreg).  */
  rtx insn_pat = PATTERN (insn);
  if (GET_CODE (insn_pat) == SET)
    {
      rtx src = SET_SRC (insn_pat);
      rtx dest = SET_DEST (insn_pat);
      if (is_reg_or_subreg_p (src) && is_reg_or_subreg_p (dest)
	  && (real_reg_or_subreg_p (src, false)
	      || real_reg_or_subreg_p (dest, false)))
	return true;
    }
  return false;
}

/* Returns true iff REG is dead after INSN. */

static bool
reg_dead_p (rtx insn, rtx reg)
{
  /* Register is dead if either data flow says so (REG_DEAD) or if
     INSN sets the register. */
  gcc_assert (REG_P (reg));
  return (find_regno_note (insn, REG_DEAD, REGNO (reg)) != NULL_RTX
	  || reg_set_p (reg, insn));
}

/* Returns true iff REG defined by INSN is never used. */

static bool
reg_unused_p (rtx insn, rtx reg)
{
  rtx note;
  gcc_assert (REG_P (reg));
  for (note = REG_NOTES (insn); note; note = XEXP (note, 1))
    {
      if (REG_NOTE_KIND (note) == REG_UNUSED)
	{
	  rtx x = XEXP (note, 0);
	  gcc_assert (REG_P (x));
	  if (REGNO (reg) == REGNO (x))
	    return true;
	}
    }
  return false;
}

/* Returns true iff the INSN is a conditional jump. Does it in a
   rather hamfisted manner.  Checks to see if INSN is a control flow
   insn and if its basic block has more than one successor. */

static bool
conditional_jump_p (rtx insn)
{
  if (control_flow_insn_p (insn) && !CALL_P (insn)
      && EDGE_COUNT (BLOCK_FOR_INSN (insn)->succs) > 1)
    return true;
  return false;
}

/* Helper function which calls for_each_rtx on part of *X which
   satisfies the conditions below in the comments for
   for_each_ref_exp. */

static int
for_each_ref_exp_1 (rtx insn, rtx *x, rtx_function f,
		    bool only_used, void *data)
{
  if (GET_CODE (*x) == SET && is_reg_or_subreg_p (SET_DEST (*x)))
    {
      rtx reg = get_reg_rtx (SET_DEST (*x));
      if (only_used && reg_unused_p (insn, reg))
	return 0;
      else
	return for_each_rtx (&SET_SRC (*x), f, data);
    }
  else if (GET_CODE (*x) != CLOBBER)
    return for_each_rtx (x, f, data);
  return 0;
}

/* Calls F on each non-destination (sort of) rtx in INSN.  If
   ONLY_USED is true, then skip SET expressions whose destinations are
   not used.

   More precisely, calls F on each rtx of ISNS's pattern which
   satisfies the following:

     - not the register destination of a SET or CLOBBER

     - if ONLY_USED is true, then not part of a SET (source or
       destination) which is not used

   Uses for_each_rtx to traverse sub-expressions, so the return value
   of F is treated as in for_each_rtx. */

static int
for_each_ref_exp (rtx insn, rtx_function f, bool only_used, void *data)
{
  rtx pat = PATTERN (insn);
  if (GET_CODE (pat) == PARALLEL)
    {
      int i;
      gcc_assert (GET_CODE (pat) == PARALLEL);
      for (i = 0; i < XVECLEN (pat, 0); i++)
	{
	  int rv = for_each_ref_exp_1 (insn, &XVECEXP (pat, 0, i),
				       f, only_used, data);
	  if (rv != 0)
	    return rv;
	}
      return 0;
    }
  else
    return for_each_ref_exp_1 (insn, &PATTERN (insn), f, only_used, data);
}

/* Returns number of references of EXPR in INSN.  EXPR can be a REG or
   SUBREG.  If USED is true, then only count those which are actually
   used.  Better than directly using count_occurrences because
   count_occurrences counts within sources of every SET, even if not
   every SET is used.  -1 is returned if register in EXPR is used but
   in a different mode or in a way which cannot be forward propagated
   into. This includes if EXPR is a subreg and the corresponding
   register is referenced.  */

struct occur_data
{
  rtx expr;
  int n;
  bool invalid;
};

static int
count_references_1 (rtx *x, void *data)
{
  struct occur_data *p = (struct occur_data *)data;
  if (GET_CODE (p->expr) == SUBREG)
    {
      if (GET_CODE (*x) == SUBREG)
	{
	  if (rtx_equal_p (*x, p->expr))
	    {
	      p->n++;
	      return -1; /* Don't traverse subexpressions. */
	    }
	  else if (REGNO (SUBREG_REG (*x)) == REGNO (SUBREG_REG (p->expr)))
	    {
	      /* SUBREGs share the same regno, but different modes. */
	      p->invalid = true;
	      return 1; /* stop traversal */
	    }
	}
      else if (REG_P (*x) && REGNO (*x) == REGNO (SUBREG_REG (p->expr)))
	{
	  /* X shares the same register as SUBREG.  */
	  p->invalid = true;
	  return 1; /* stop traversal */
	}
    }
  else if (REG_P (p->expr))
    {
      if (rtx_equal_p (*x, p->expr))
	p->n++;
      else if (REGNO (p->expr) == REGNO (*x))
	{
	  /* Same regno but different mode.  */
	  p->invalid = true;
	  return 1; /* stop traversal */
	}
    }
  else
    gcc_assert (0);

  return 0;
}

/* Checks for a use rtx of the register defined by DATA.  Sets invalid
   field of DATA if one is found. */

static int
check_for_use_expr (rtx *x, void *data)
{
  struct occur_data *p = (struct occur_data *)data;
  if (GET_CODE (*x) == USE) {
    rtx use_reg = XEXP (*x, 0);
    rtx data_reg;
    gcc_assert (REG_P (use_reg));
    if (GET_CODE (p->expr) == SUBREG)
      data_reg = SUBREG_REG (p->expr);
    else
      data_reg = p->expr;
    if (REGNO (data_reg) == REGNO (use_reg))
      {
	p->invalid = true;
	return 1;
      }
  }

  return 0;
}

static int
count_references (rtx insn, rtx expr, bool used)
{
  struct occur_data data;
  data.expr = expr;
  data.n = 0;
  data.invalid = false;
  for_each_ref_exp (insn, count_references_1, used, &data);
  if (!data.invalid)
    for_each_rtx (&PATTERN (insn), check_for_use_expr, &data);
  return (data.invalid ? -1 : data.n);
}

/* Helper function for store_aliases_p. */

static int
store_aliases_p_1 (rtx *x, void *p)
{
  rtx store = (rtx)p;
  if (MEM_P (*x) && memory_modified_in_insn_p (*x, store))
    return 1;
  return 0;
}

/* Returns true iff STORE may write a memory location read or written
   to by INSN. */

static bool
store_aliases_p (rtx store, rtx insn)
{
  return (for_each_rtx (&PATTERN (insn), store_aliases_p_1, store) != 0);
}

static void
modifies_memory_p_1 (rtx x, const_rtx pat ATTRIBUTE_UNUSED, void *data)
{
  bool *p = (bool *)data;
  if (MEM_P (x))
    *p = true;
}

/* Returns true iff INSN modifies memory. */

static bool
modifies_memory_p (rtx insn)
{
  bool mod = false;
  note_stores (PATTERN (insn), modifies_memory_p_1, &mod);
  return mod;
}


static int
references_memory_p_1 (rtx *x, void *data)
{
  bool *p = (bool *)data;
  if (MEM_P (*x))
    *p = true;
  return 0;
}
/* Returns true iff INSN modifies memory. */

static bool
references_memory_p (rtx insn)
{
  bool ref = false;
  for_each_rtx (&PATTERN (insn), references_memory_p_1, &ref);
  return ref;
}

static int
uses_real_reg_except_cc_1 (rtx *x, void *data)
{
  bool *p = (bool *)data;
  if (real_reg_or_subreg_p (*x, true))
    {
      *p = true;
      return -1;
    }
  return 0;
}

/* Returns true iff INSN uses a real (hard) register except CC. */

static bool
uses_real_reg_except_cc_p (rtx insn)
{
  bool uses_reg = false;
  for_each_ref_exp (insn, uses_real_reg_except_cc_1, false, &uses_reg);
  return uses_reg;
}

/* Helper struct and function for defines_real_reg_p. */

struct define_data
{
  rtx insn;
  bool defines_reg;
  bool except_cc;
};

static void
defines_used_real_reg_p_1 (rtx x, const_rtx pat ATTRIBUTE_UNUSED, void *data)
{
  struct define_data *p = (struct define_data *)data;
  if (REG_P (x) && real_reg_or_subreg_p (x, p->except_cc)
      && !reg_dead_p (p->insn, x))
    p->defines_reg = true;
}

/* Returns true iff INSN defines a real (hard) register except CC, and
   definition is not dead. */

static bool
defines_used_real_reg_p (rtx insn, bool except_cc)
{
  struct define_data data;
  data.insn = insn;
  data.defines_reg = false;
  data.except_cc = except_cc;
  note_stores (PATTERN (insn), defines_used_real_reg_p_1, &data);
  return data.defines_reg;
}

struct merge_data
{
  rtx from;
  rtx to;
};

/* Helper function for merge_def. */

static int
merge_def_1 (rtx *x, void *data)
{
  struct merge_data *p = (struct merge_data *)data;
  if (rtx_equal_p (*x, p->from))
    {
      *x = p->to;
      return -1; /* Don't traverse below this point.  */
    }
  else if (REG_P (*x))
    /* All instances of the target register mustbe substituted.  */
    gcc_assert (REGNO (*x) != REGNO (get_reg_rtx (p->from)));
  return 0;
}

/* Merge DEF_INSN into USE_INSN.  Necessarily, DEF_INSN defines a
   register or subreg used by USE_INSN. */

static void
merge_def (rtx def_insn, rtx use_insn, rtx expr)
{
  struct merge_data data;
  const_rtx set = set_of (expr, def_insn);

  data.from = expr;
  gcc_assert (set != NULL_RTX);
  data.to = SET_SRC (set);
  for_each_ref_exp (use_insn, merge_def_1, false, &data);

/*   /\* Fix up REG_DEAD reg notes in new combined insn. Add dead notes of */
/*      src insn to new insn and remove dead note of merged register.  *\/ */
/*   for (rn = REG_NOTES (def_insn); rn; rn = XEXP (rn, 1)) */
/*     if (REG_NOTE_KIND (rn) == REG_DEAD) */
/*       REG_NOTES (use_insn) = gen_rtx_EXPR_LIST (REG_DEAD, */
/* 						XEXP (rn, 0), */
/* 						REG_NOTES (use_insn)); */
/*   rn = find_regno_note (use_insn, REG_DEAD, REGNO (reg)); */
/*   if (rn != NULL_RTX) */
/*     remove_note (use_insn, rn); */

  delete_insn (def_insn);
}

/* Simple-minded O(n^2) algorithm to combine insns together.
   insn A is combined into insn B iff:
     1. A and B are in same BB
     2. A defines only one used reg (call it rA)
     3. B is only use of rA (rA is dead at B), and B only uses rA once
     4. A's dependences are available at B
   This probably should be made O(n). */

static void
combine_insns (void)
{
  basic_block bb;

  df_note_add_problem ();
  df_analyze ();

  COMBINE_LOG ("function");
  FOR_EACH_BB (bb)
    {
      rtx def_insn, use_insn, next;

      if (dump_file)
	{
	  COMBINE_LOG ("bb");
	  dump_def_use (dump_file, bb);
	  log_bb_data (bb, "bb-before");
	}

      FOR_BB_INSNS_SAFE (bb, def_insn, next)
	{
	  df_ref *use_vec;
	  rtx def_expr;
	  bool merged = false;
	
	  if (!INSN_P (def_insn))
	    continue;

	  if (dump_file)
	    {
	      struct leaf_counts cnt;
	      insn_count_leaves (def_insn, &cnt);
	      COMBINE_LOG ("insn");
	      if (conditional_jump_p (def_insn))
		COMBINE_LOG ("insn-type-cond-cf");
	      else if (control_flow_insn_p (def_insn))
		COMBINE_LOG ("insn-type-uncond-cf");
	      else
		COMBINE_LOG ("insn-type-noncf");
	      fprintf (dump_file, "def insn %d: ", INSN_UID (def_insn));
	      print_inline_rtx (dump_file, def_insn, 4);
	      fprintf (dump_file, "\n");
	      log_leaves ("insn", &cnt);
	    }

	  /* Check that def_insn defines only one used register
	     (condition 2) */
	  def_expr = single_set (def_insn);
	  if (def_expr == NULL_RTX)
	    {
	      COMBINE_LOG ("insn defines more than one or no value");
	      continue;
	    }
	  def_expr = SET_DEST (def_expr);

	  /* TODO: SUBREG or REG.
	     DONE.  */
	  if (!is_reg_or_subreg_p (def_expr))
	    {
	      COMBINE_LOG ("insn defines a non-register value");
	      continue;
	    }

	  /* TODO: defines real reg or subreg.
	     DONE.  */
	  if (real_reg_or_subreg_p (def_expr, true))
	    {
	      COMBINE_LOG ("insn defines real register which is not CC");
	      continue;
	    }

	  /* TODO: no change req'd.  Use of SUBREG implies use of REG.
	     DONE.  */
	  if (uses_real_reg_except_cc_p (def_insn))
	    {
	      COMBINE_LOG ("insn uses real register");
	      continue;
	    }

	  if (modifies_memory_p (def_insn))
	    {
	      COMBINE_LOG ("insn modifies memory and register");
	      continue;
	    }

	  /* Iterate from def to first use of defined register (or end
	     of BB). */
	  for (use_insn = NEXT_INSN (def_insn);
	       use_insn && use_insn != NEXT_INSN (BB_END (bb));
	       use_insn = NEXT_INSN (use_insn))
	    {
	      int uses;
	      if (!INSN_P (use_insn))
		continue;

	      if (conditional_jump_p (use_insn))
		continue;

	      /* USES is number of non-def appearances of def in USE_INSN. */

	      /* TODO:
		
		 If SUBREG, then count number of proper uses of
		 subreg.  If overlapping reg found, then -1 should be
		 returned.
		
		 If reg, then no change.
	
		 DONE. */
	      uses = count_references (use_insn, def_expr, false);
	      if (uses)
		{
		  /* USED_USES is number of non-def appearances of def
		     in USE_INSN which are part of an expression which
		     actually used (not dead). */

		  /* TODO: same as above.  DONE.  */
		  int used_uses = count_references (use_insn, def_expr, true);

		  /* insn uses reg.  Check to see if it is only used once and
		     if it is dead. */
		  if (dump_file)
		    {
		      fprintf (dump_file, "use insn %d: ", INSN_UID (use_insn));
		      print_inline_rtx (dump_file, use_insn, 4);
		      fprintf (dump_file, "\n");
		    }
		  if (uses == -1)
		    {
		      COMBINE_LOG ("unpropagatable use (different mode, in use insn, etc)");
		      break;
		    }
		  if (used_uses > 1)
		    {
		      COMBINE_LOG ("register used more than once at first use");
		      break;
		    }

		  /* TODO: make sure that dead_p properly handles SUBREG case.  DONE.*/
		  if (!reg_dead_p (use_insn, get_reg_rtx (def_expr)))
		    {
		      COMBINE_LOG ("register not dead at first use");
		      break;
		    }

		  /* Check for instances of SUBREG copies.  DONE. */
		  if (copy_mach_reg_p (use_insn))
		    {
		      COMBINE_LOG ("copy instruction involving a machine register");
		      break;
		    }

		  /* Combine insns together. */
		  merge_def (def_insn, use_insn, def_expr);
		  if (dump_file)
		    {
		      COMBINE_LOG ("insn merged");
		      fprintf (dump_file, "new insn: ");
		      print_inline_rtx (dump_file, use_insn, 4);
		      fprintf (dump_file, "\n");
		    }
		  merged = true;
		  break;
		}
	      else
		{
		  /* Check to see if use_insn sets/clobbers any of the
		     registers used by def_insn.  If one of these
		     registers is set/clobbers then we can't combine
		     def_insn into anything later because it's
		     dependences are changed. (condition 4).
		
		     Note: Can't use DF_INSN_USES because the insn
		     combining makes these invalid.  However,
		     DF_INSN_DEF will always be valid. */
		  for (use_vec = DF_INSN_DEFS (use_insn); *use_vec; use_vec++)
		    if (reg_referenced_p (DF_REF_REG (*use_vec), PATTERN (def_insn)))
		      {
			if (dump_file)
			  {
			    COMBINE_LOG ("dependency of def clobbered before use");
			    fprintf (dump_file, "clobbering insn: ");
			    print_inline_rtx (dump_file, use_insn, 4);
			    fprintf (dump_file, "\n");
			  }
			break;
		      }
		  /* Loop exited prematurely only if dependencies clobbered. */
		  if (*use_vec)
		    break;

		  if (store_aliases_p (use_insn, def_insn))
		    {
		      if (dump_file)
			{
			  COMBINE_LOG ("store aliases memory loads/stores of def");
			  fprintf (dump_file, "aliasing insn: ");
			  print_inline_rtx (dump_file, use_insn, 4);
			  fprintf (dump_file, "\n");
			}
		      break;
		    }
		
		  /* Don't propagate across definition of real register. */
		  if (defines_used_real_reg_p (use_insn, true))
		    {
		      if (dump_file)
			{
			  COMBINE_LOG ("real register defined before use");
			  fprintf (dump_file, "defined insn: ");
			  print_inline_rtx (dump_file, use_insn, 4);
			  fprintf (dump_file, "\n");
			}
		      break;
		    }

		  /* If def references memory, then don't propagate across a call. */
		  if (CALL_P (use_insn)
		      && references_memory_p (def_insn))
		    {
		      if (dump_file)
			{
			  COMBINE_LOG ("call encountered and def references memory");
			  fprintf (dump_file, "call insn: ");
			  print_inline_rtx (dump_file, use_insn, 4);
			  fprintf (dump_file, "\n");
			}
		      break;
		    }
		}
	    }
	  if (!use_insn || use_insn == NEXT_INSN (BB_END (bb)))
	    COMBINE_LOG ("no use in basic block");
	}
      if (dump_file)
	log_bb_data (bb, "bb-after");
    }
}

static unsigned
rest_of_handle_icg_combine (void)
{
  combine_insns ();

  return 0;
}

static bool
gate_icg_combine (void)
{
  return flag_icg;
}


struct rtl_opt_pass pass_icg_combine =
{
  {
    RTL_PASS,
    "icg_combine",                        /* name */
    gate_icg_combine,                     /* gate */
    rest_of_handle_icg_combine,           /* execute */
    NULL,                                 /* sub */
    NULL,                                 /* next */
    0,                                    /* static_pass_number */
    0,                                    /* tv_id */
    0,                                    /* properties_required */
    0,                                    /* properties_provided */
    0,                                    /* properties_destroyed */
    0,                                    /* todo_flags_start */
    TODO_dump_func,                       /* todo_flags_finish */
  }
};


/* Logging stuff */

/* Returns number of insns in basic block BB.  Better, existing way to
   do this? */

static int
bb_num_insns (basic_block bb)
{
  rtx insn;
  int n = 0;
  FOR_BB_INSNS (bb, insn)
    if (INSN_P (insn))
      n++;
  return n;
}

struct leaf_data
{
  rtx insn;

  /* Arrays indexed by regno recording the number of live/dead uses in
     the insn.  (Shamefully terrible, should use more space-efficient data
     structure). */
  int *dead_uses;
  int *live_uses;
  struct leaf_counts *pcnt;
};

static int
accum_leaf_1 (rtx *x, void *p)
{
  struct leaf_data *data = (struct leaf_data *)p;
  if ((*x != NULL_RTX)
      && (REG_P (*x) || MEM_P (*x) || CONST_INT_P (*x)))
    {
      data->pcnt->leaves++;
      if (CONST_INT_P (*x))
	data->pcnt->con++;
      else if (MEM_P (*x))
	data->pcnt->mem++;
      else if (REG_P (*x))
	{
	  data->pcnt->reg++;
	  if (reg_dead_p (data->insn, *x))
	    data->dead_uses[REGNO (*x)]++;
	  else
	    data->live_uses[REGNO (*x)]++;
	}
    }
  return 0;
}

/* Accumulate the types and number of leaves in the insn defined in
   DATA. DATA->dead_uses and DATA->live uses must be arrays allocated
   with max_reg_num() elements. DATA->pcnt must be valid pointer to
   leaf count data structure.  */

static void
accum_leaf (struct leaf_data *data)
{
  int r;

  for_each_ref_exp (data->insn, accum_leaf_1, true, data);

  for (r = 0; r < max_reg_num (); r++)
    {
      if (data->dead_uses[r] == 1)
	data->pcnt->reg_single_dead++;
      else if (data->dead_uses[r] > 1)
	data->pcnt->reg_mult_dead += data->dead_uses[r];

      if (data->live_uses[r] == 1)
	data->pcnt->reg_single_live++;
      else if (data->live_uses[r] > 1)
	data->pcnt->reg_mult_live += data->live_uses[r];
    }
}

/* Count the types and number of leaves in INSN.  Values are placed in
   PCNT. */

static void
insn_count_leaves (rtx insn, struct leaf_counts *pcnt)
{
  struct leaf_data data;
  data.insn = insn;
  data.dead_uses = (int *)xcalloc (sizeof (int), max_reg_num ());
  data.live_uses = (int *)xcalloc (sizeof (int), max_reg_num ());
  data.pcnt = pcnt;
  memset (data.pcnt, 0, sizeof (struct leaf_counts));
  accum_leaf (&data);

  free (data.dead_uses);
  free (data.live_uses);
}

/* Count the types and number of leaves in BB.  Values are placed in
   PCNT. */

static void
bb_count_leaves (basic_block bb, struct leaf_counts *pcnt)
{
  struct leaf_data data;
  rtx insn;

  data.dead_uses = (int *)xmalloc (sizeof (int) * max_reg_num ());
  data.live_uses = (int *)xmalloc (sizeof (int) * max_reg_num ());
  data.pcnt = pcnt;
  memset (data.pcnt, 0, sizeof (struct leaf_counts));

  FOR_BB_INSNS (bb, insn)
    if (INSN_P (insn))
      {
	data.insn = insn;
	memset (data.dead_uses, 0, max_reg_num () * sizeof (int));
	memset (data.live_uses, 0, max_reg_num () * sizeof (int));
	accum_leaf (&data);
      }

  free (data.dead_uses);
  free (data.live_uses);
}

static void
log_leaves (const char *str, struct leaf_counts *pcnt)
{
  if (dump_file)
    {
      char s[256];
#define LOG(p, f, d) { snprintf (s, 256, "%s-%s", p, f); COMBINE_LOG_DATA (s, d); }
      LOG (str, "leaves", pcnt->leaves);
      LOG (str, "memory", pcnt->mem);
      LOG (str, "const", pcnt->con);
      LOG (str, "register", pcnt->reg);
      LOG (str, "register-single-live", pcnt->reg_single_live);
      LOG (str, "register-multiple-live", pcnt->reg_mult_live);
      LOG (str, "register-single-dead", pcnt->reg_single_dead);
      LOG (str, "register-multiple-dead", pcnt->reg_mult_dead);
#undef LOG
    }
}

/* Dump dataflow information to a file. */

static void
dump_def_use (FILE *fp, basic_block bb)
{
  rtx insn;
  fprintf (dump_file, "\n" "Dataflow for BB %d:\n", bb->index);
  FOR_BB_INSNS (bb, insn)
    {
      df_ref *df_vec;

      if (!INSN_P (insn))
	continue;

      fprintf (fp, "\n" "insn %d: ", INSN_UID (insn));
      print_inline_rtx (fp, insn, 4);
      fprintf (fp, "\n  Defs: ");
      for (df_vec = DF_INSN_DEFS (insn); *df_vec; df_vec++)
	{
	  rtx reg = DF_REF_REG (*df_vec);
	  print_inline_rtx (fp, reg, 2);
	  if (reg_unused_p (insn, get_reg_rtx (reg)))
	    fprintf (fp, " UNUSED");
	  fprintf (fp, ";  ");
	}

      /* use DF_REF_REG to get register reference. */

      fprintf (fp, " \n  Uses: ");
      for (df_vec = DF_INSN_USES (insn); *df_vec; df_vec++)
	{
	  rtx reg = DF_REF_REG (*df_vec);
	  print_inline_rtx (fp, reg, 2);
	  if (reg_dead_p (insn, get_reg_rtx (reg)))
	    fprintf (fp, " DEAD");
	  fprintf (fp, ";  ");
	}
      fprintf (fp, "\n");
    }
}

/* Log leaf information about the basic block BB to dump_file.  Prefix
   any data fields with PREFIX. */

static void
log_bb_data (basic_block bb, const char *prefix)
{
  rtx insn;
  struct leaf_counts cnt;
  int ucf = 0, ccf = 0, ncf = 0;
  int ucf_leaves = 0, ccf_leaves = 0, ncf_leaves = 0;
  char s[256];

  /* Log data for all insns. */
  bb_count_leaves (bb, &cnt);
  snprintf (s, 256, "%s-insns", prefix);
  COMBINE_LOG_DATA (s, bb_num_insns(bb));
  log_leaves (prefix, &cnt);

  /* Count number of leaves and insns for three different insn types:
       - unconditional control_flow
       - conditional control_flow
       - non control flow */
  FOR_BB_INSNS (bb, insn)
    if (INSN_P (insn))
      {
	insn_count_leaves (insn, &cnt);
	if (conditional_jump_p (insn))
	  {
	    ccf++;
	    ccf_leaves += cnt.leaves;
	  }
	else if (control_flow_insn_p (insn))
	  {
	    ucf++;
	    ucf_leaves += cnt.leaves;
	  }
	else
	  {
	    ncf++;
	    ncf_leaves += cnt.leaves;
	  }
      }
  snprintf (s, 256, "%s-uncond-cf-insns", prefix);
  COMBINE_LOG_DATA (s, ucf);
  snprintf (s, 256, "%s-cond-cf-insns", prefix);
  COMBINE_LOG_DATA (s, ccf);
  snprintf (s, 256, "%s-non-cf-insns", prefix);
  COMBINE_LOG_DATA (s, ncf);

  snprintf (s, 256, "%s-uncond-cf-leaves", prefix);
  COMBINE_LOG_DATA (s, ucf_leaves);
  snprintf (s, 256, "%s-cond-cf-leaves", prefix);
  COMBINE_LOG_DATA (s, ccf_leaves);
  snprintf (s, 256, "%s-non-cf-leaves", prefix);
  COMBINE_LOG_DATA (s, ncf_leaves);
}
