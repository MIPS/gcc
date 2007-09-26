/* Instruction scheduling pass.
   Copyright (C) 2006, 2007 Free Software Foundation, Inc.

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


/* FIXME: check whether we need all these headers, and check the makefile.  */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "toplev.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "regs.h"
#include "function.h"
#include "flags.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "except.h"
#include "toplev.h"
#include "recog.h"
#include "cfglayout.h"
#include "params.h"
#include "target.h"
#include "output.h"
#include "timevar.h"
#include "tree-pass.h"
#include "sched-rgn.h"
#include "sched-int.h"
#include "sched-deps.h"
#include "cselib.h"
#include "ggc.h"
#include "tree.h"
#include "vec.h"
#include "langhooks.h"
#include "rtlhooks-def.h"

#ifdef INSN_SCHEDULING
#include "sel-sched-ir.h"
#include "sel-sched-dump.h"
#include "sel-sched.h"

/* FIXME: extend this comment.

   Implementation of selective scheduling approach.
   References:
   S.-M. Moon and K. Ebcioglu. Parallelizing nonnumerical code with
   selective scheduling and software pipelining. 
   ACM TOPLAS, Vol 19, No. 6, pages 853--898, Nov. 1997.  */

/* Software lookahead window size.
   According to the results in Nakatani and Ebcioglu [1993], window size of 16 
   is enough to extract most ILP in integer code.  */
#define MAX_WS (PARAM_VALUE (PARAM_SELSCHED_MAX_LOOKAHEAD))

/* True when moveup_set_path functionality should be used.  */
bool enable_moveup_set_path_p;

/* True when separable insns should be scheduled as RHSes.  */
bool enable_schedule_as_rhs_p;

/* True when pipelining is enabled.  */
bool pipelining_p;

/* True if bookkeeping is enabled.  */
bool bookkeeping_p;

/* True if we should make an aditional pass to set somewhat correct
   sched cycles.  */
bool reset_sched_cycles_p;


/* Definitions of local types and macros.  */

/* Represents possible outcomes of moving an expression through an insn.  */
enum MOVEUP_RHS_CODE { MOVEUP_RHS_SAME, MOVEUP_RHS_NULL, MOVEUP_RHS_CHANGED };

/* The container to be passed into rtx search & replace functions.  */
struct rtx_search_arg
{
  /* What we are searching for.  */
  rtx x;

  /* With what X will be replaced.  */
  rtx y;

  /* The occurence counter.  */
  int n;

  /* Stop after finding MAX_OCCUR occurences, if it is positive.  */
  int max_occur;

  /* Iterator of the av_set to remove rhs from.  */
  av_set_iterator *iter;

  /* Bitmask according to which substitutions are performed.  */
  unsigned long bitmask;
};
typedef struct rtx_search_arg *rtx_search_arg_p;

/* This descibes the data given to sel_sched_region_2.  */
struct sel_sched_region_2_data_def
{  
  int orig_max_seqno;
  int highest_seqno_in_use;
};
typedef struct sel_sched_region_2_data_def *sel_sched_region_2_data_t;

/* This struct contains precomputed hard reg sets that are needed when 
   computing registers available for renaming.  */
struct hard_regs_data 
{
  /* For every mode, this stores registers available for use with 
     that mode.  */
  HARD_REG_SET regs_for_mode[NUM_MACHINE_MODES];

  /* True when regs_for_mode[mode] is initialized.  */
  bool regs_for_mode_ok[NUM_MACHINE_MODES];

  /* For every register, it has regs that are ok to rename into it.
     The register in question is always set.  If not, this means
     that the whole set is not computed yet.  */
  HARD_REG_SET regs_for_rename[FIRST_PSEUDO_REGISTER];

  /* For every mode, this stores registers not available due to 
     call clobbering.  */
  HARD_REG_SET regs_for_call_clobbered[NUM_MACHINE_MODES];

  /* All registers that are used or call used.  */
  HARD_REG_SET regs_ever_used;

#ifdef STACK_REGS
  /* Stack registers.  */
  HARD_REG_SET stack_regs;
#endif
};

/* A global structure that contains the needed information about harg 
   regs.  */
static struct hard_regs_data sel_hrd;


/* True if/when we want to emulate Haifa scheduler in the common code.  
   This is used in sched_rgn_local_init and in various places in 
   sched-deps.c.  */
int sched_emulate_haifa_p;

/* GLOBAL_LEVEL is used to discard information stored in basic block headers
   av_sets.  Av_set of bb header is valid if its (bb header's) level is equal
   to GLOBAL_LEVEL.  And invalid if lesser.  This is primarily used to advance
   scheduling window.  */
int global_level;

/* Current fences.  */
flist_t fences;

/* !!! Remove me: not used.
   Stage of the loop pipelining process.  */
static int stage;

/* All newly emitted insns will have their uids greater than this value.  */
static int first_emitted_uid;

/* Set of basic blocks that are forced to start new ebbs.  This is a subset
   of all the ebb heads.  */
static bitmap_head _forced_ebb_heads;
bitmap_head *forced_ebb_heads = &_forced_ebb_heads;

/* True when the first lv set should be ignored when updating liveness.  */
static bool ignore_first = false;

/* Number of insns max_issue has initialized data structures for.  */
static int max_issue_size = 0;

/* Whether do we need a stall currently.  */
static int need_stall = 0;

/* Whether we can issue more instructions.  */
static int can_issue_more;

/* A vector of expressions is used to be able to sort them.  */
DEF_VEC_P(rhs_t);
DEF_VEC_ALLOC_P(rhs_t,heap);
static VEC(rhs_t, heap) *vec_av_set = NULL;

/* This shows how many times the scheduler was run.  */
static int sel_sched_region_run = 0;

/* Variables to accumulate different statistics.  */
static int stat_bookkeeping_copies;
static int stat_insns_needed_bookkeeping;
static int stat_renamed_scheduled;
static int stat_substitutions_total;


/* Forward declarations of static functions.  */
static bool rtx_search (rtx, rtx);
static int sel_rank_for_schedule (const void *, const void *);
static bool equal_after_moveup_path_p (rhs_t, ilist_t, rhs_t);
static regset compute_live (insn_t);
static void generate_bookkeeping_insn (rhs_t, insn_t, edge, edge);
static bool find_used_regs (insn_t, av_set_t, regset, HARD_REG_SET *, 
                            def_list_t *);
static bool move_op (insn_t, av_set_t, ilist_t, edge, edge, expr_t);
static void sel_sched_region_1 (void);
static void sel_sched_region_2 (sel_sched_region_2_data_t);


/* Functions that work with fences.  */

/* Advance one cycle on FENCE.  */
static void
advance_one_cycle (fence_t fence)
{
  advance_state (FENCE_STATE (fence));
  FENCE_CYCLE (fence)++;
  FENCE_ISSUED_INSNS (fence) = 0;
  FENCE_STARTS_CYCLE_P (fence) = 1;
  can_issue_more = issue_rate;
  print ("Finished a cycle.  Current cycle = %d", FENCE_CYCLE (fence));
}

/* Returns true when SUCC in a fallthru bb of INSN, possibly
   skipping empty basic blocks.  */
static bool
in_fallthru_bb_p (rtx insn, rtx succ)
{
  basic_block bb = BLOCK_FOR_INSN (insn);

  if (bb == BLOCK_FOR_INSN (succ))
    return true;

  if (find_fallthru_edge (bb))
    bb = find_fallthru_edge (bb)->dest;
  else
    return false;

  while (sel_bb_empty_p (bb))
    bb = bb->next_bb;

  return bb == BLOCK_FOR_INSN (succ);
}

/* Construct successor fences from FENCE and put them in NEW_FENCES. 
   When a successor will continue a ebb, transfer all FENCE's parameters
   to the new fence.  */
static void
extract_new_fences_from (fence_t fence, flist_tail_t new_fences,
			 sel_sched_region_2_data_t data)
{
  int orig_max_seqno = data->orig_max_seqno;
  bool was_here_p = false;
  insn_t insn = NULL_RTX;
  ilist_iterator ii;

  /* Get the only element of FENCE_BNDS (fence).  */
  FOR_EACH_INSN (insn, ii, FENCE_BNDS (fence))
    {
      gcc_assert (!was_here_p);
      was_here_p = true;
    }

  gcc_assert (was_here_p && insn != NULL_RTX);

  if (was_here_p /* true */)
    {
      insn_t succ;
      succ_iterator si;

      FOR_EACH_SUCC_1 (succ, si, insn, SUCCS_NORMAL | SUCCS_SKIP_TO_LOOP_EXITS)
	{
	  int seqno = INSN_SEQNO (succ);

	  if (0 < seqno && seqno <= orig_max_seqno
	      && (pipelining_p || INSN_SCHED_CYCLE (succ) <= 0))
	    {
	      bool b = in_same_ebb_p (insn, succ)
                       || (!flag_sel_sched_reset_tc_on_join
                           && in_fallthru_bb_p (insn, succ));


	      print ("%d[%d] (state %s); ", INSN_UID (succ),
		     BLOCK_NUM (succ), b ? "continue" : "reset");

	      if (b)
		new_fences_add_dirty (new_fences, succ, fence);
	      else
		{
		  /* Mark block of the SUCC as head of the new ebb.  */
		  bitmap_set_bit (forced_ebb_heads, BLOCK_NUM (succ));
		  new_fences_add_clean (new_fences, succ, fence);
		}
	    }
	}
    }
}


/* Functions to support substitution.  */

/* Substitute all occurences of INSN's destination in RHS' vinsn with INSN's 
   source (if INSN is eligible for substitution).  Returns TRUE if
   substitution was actually performed, FALSE otherwise.  Substitution might
   be not performed because it's either RHS' vinsn doesn't contain INSN's
   destination or the resulting insn is invalid for the target machine.  */
static bool
substitute_reg_in_rhs (rhs_t rhs, insn_t insn)
{
  rtx *where;
  bool new_insn_valid;
  vinsn_t *vi = &RHS_VINSN (rhs);

  if (VINSN_UNIQUE_P (*vi))
    /* Unique vinsns can't play this game.  */
    return false;

  /* Do not try to replace in SET_DEST.  Although we'll choose new
     register for the RHS, we don't want to change RHS' original reg.  
     If the insn is not SET, we may still be able to substitute something
     in it, and if we're here (don't have deps), it doesn't write INSN's 
     dest.  */

  where = (VINSN_SEPARABLE_P (*vi)
	   ? &VINSN_RHS (*vi)
	   : &PATTERN (VINSN_INSN (*vi)));

  /* Substitute if INSN has a form of x:=y and LHS(INSN) occurs in *VI.  */
  if (insn_eligible_for_subst_p (insn) && rtx_search (INSN_LHS (insn), *where))
    {
      rtx new_insn;
      rtx *where_replace;

      /* We should copy these rtxes before substitution.  */
      new_insn = create_copy_of_insn_rtx (VINSN_INSN_RTX (*vi));

      /* Where we'll replace.  */
      /* WHERE_REPLACE should point inside NEW_INSN, so INSN_RHS couldn't be
	 used instead of SET_SRC.  */
      where_replace = (VINSN_SEPARABLE_P (*vi)
		       ? &SET_SRC (PATTERN (new_insn))
		       : &PATTERN (new_insn));

      new_insn_valid 
        = validate_replace_rtx_part_nosimplify (INSN_LHS (insn),
                                                copy_rtx (INSN_RHS (insn)),
                                                where_replace, new_insn);

      /* ??? Actually, constrain_operands result depends upon choice of
         destination register.  E.g. if we allow single register to be an rhs,
	 and if we try to move dx=ax(as rhs) through ax=dx, we'll result 
	 in invalid insn dx=dx, so we'll loose this rhs here.
	 Just can't come up with significant testcase for this, so just
	 leaving it for now.  */
      if (new_insn_valid)
	{
	  if (!insn_rtx_valid (new_insn))
	    gcc_unreachable ();

	  change_vinsn_in_expr (rhs, create_vinsn_from_insn_rtx (new_insn));

	  return true;
	}
      else
	{
	  /* It would nice to free new_insn or return it to some pool.  */
	  return false;
	}
    }
  else
    return false;
}

/* Helper function for replace_in_vinsn_using_bitmask.  See comment to that
   function for the details.  */
static int 
replace_in_vinsn_using_bitmask_1 (rtx *cur_rtx, void *arg)
{
  struct rtx_search_arg *p = (struct rtx_search_arg *) arg;

  /* If matched...  */
  if (exp_equiv_p (*cur_rtx, p->x, 0, true))
    {
      /* Check whether we should replace it according to bitmask.  */
      if (p->bitmask & 1)
        {
	  /* Replace.  */
	  (*cur_rtx) = copy_rtx (p->y);
        }
      /* Advance to next bit.  */
      p->bitmask = p->bitmask >> 1;

      /* Finish search if no more occurences to replace, or continue search
	 (not traversing subexprs of *CUR_RTX). */
      if (!p->bitmask)
	return 1;
      else
	return -1;
    }
  return 0;
}

/* Performs the substitution according to a BITMASK: every "one" with 
   index K in bitmap mask means that the K-th occurence of WHAT will be
   replaced by BY_WHAT in WHERE, "0" means that it won't be changed.  */
static void
replace_in_rtx_using_bitmask (rtx *wherep, rtx what, rtx by_what, 
			      unsigned long bitmask)
{
  struct rtx_search_arg arg;

  gcc_assert (bitmask);

  arg.x = what;
  arg.y = by_what;
  arg.bitmask = bitmask;

  for_each_rtx (wherep,
		&replace_in_vinsn_using_bitmask_1, (void *) &arg);

  /* Make sure all occurences has been replaced.  */
  gcc_assert (!arg.bitmask);
}

/* Helper function for count_occurences_equiv.  */
static int 
count_occurrences_1 (rtx *cur_rtx, void *arg)
{
  rtx_search_arg_p p = (rtx_search_arg_p) arg;

  /* The last param FOR_GCSE is true, because otherwise it performs excessive
    substitutions like
	r8 = r33
	r16 = r33
    for the last insn it presumes r33 equivalent to r8, so it changes it to
    r33.  Actually, there's no change, but it spoils debugging.  */
  if (exp_equiv_p (*cur_rtx, p->x, 0, true))
    {
      p->n++;

      /* Stop search, if we've already found the requested number of
	 occurences.  */
      if (p->max_occur > 0 && p->n >= p->max_occur)
	return 1;

      /* Do not traverse subexprs.  */
      return -1;
    }

  if (GET_CODE (*cur_rtx) == SUBREG
      && REG_P (p->x)
      && REGNO (SUBREG_REG (*cur_rtx)) == REGNO (p->x))
    {
      /* ??? Do not support substituting regs inside subregs.  In that case,
         simplify_subreg will be called by validate_replace_rtx, and 
         unsubstitution will fail later.  */
      p->n = 0;
      p->max_occur = 0;
      return -1;
    }

  /* Continue search.  */
  return 0;
}

/* Return the number of places WHAT appears within WHERE.  Find no more
   than MAX_OCCUR occurences.  This function is analogous to
   count_occurrences but it counts not only subexprs that have 
   equal pointers, but also those for which exp_equiv_p is true.  */
static int 
count_occurrences_equiv (rtx what, rtx where, int max_occur)
{
  struct rtx_search_arg arg;

  arg.x = what;
  arg.n = 0;
  arg.max_occur = max_occur;

  for_each_rtx (&where, &count_occurrences_1, (void *) &arg);

  return arg.n;
}

/* Returns TRUE if WHAT is found in WHERE rtx tree.  */
static bool
rtx_search (rtx what, rtx where)
{
  return (count_occurrences_equiv (what, where, 1) > 0);
}

/* if INSN is a copy x:=y and if there is an rhs r in AV that uses y, 
   it adds all variants of r to AV derived by replacing one or more
   occurrences of y by x.  */
static void
un_substitute (rhs_t rhs, rtx insn, av_set_t *new_set_ptr)
{
  rtx pat, src_reg, dst_reg;
  vinsn_t vi;
  rtx where_count;
  unsigned n_occur;
  unsigned long bitmask;
  
  if (!flag_sel_sched_substitution)
    return;

  /* Catch X := Y insns, where X and Y are regs.  Otherwise return.  */
  if (!insn_eligible_for_subst_p (insn))
    return;

  pat = PATTERN (insn);
  src_reg = SET_SRC (pat);
  dst_reg = SET_DEST (pat);
  vi = RHS_VINSN (rhs);

  where_count = (VINSN_SEPARABLE_P (vi) 
                 ? VINSN_RHS (vi) : VINSN_PATTERN (vi));

  n_occur = count_occurrences_equiv (src_reg, where_count, 0);

  /* Try next rhs, if no occurences.  */
  if (!n_occur)
    return;
  
  /* Make sure we have enough bits to handle all substitutions.  */
  gcc_assert (n_occur < sizeof (unsigned long) * 8);

  /* Generate the replacement mask and perform the substitution
     according to it.  Every "one" with index K in bitmap mask means 
     we change the K-th occurence of SRC_REG with DST_REG, "0" means we
     skip it.  We already have the replacement configuration for 
     bitmask == 0 in the original set.  */	 
  for (bitmask = 1; bitmask < ((unsigned long)1 << n_occur); bitmask++)
    {
      expr_def _tmp_rhs, *tmp_rhs = &_tmp_rhs;
      rtx new_insn;
      vinsn_t new_vi;
      rtx *wherep;

      new_insn = create_copy_of_insn_rtx (VINSN_INSN (vi));

      wherep = (VINSN_SEPARABLE_P (vi)
                ? &SET_SRC (PATTERN (new_insn))
                : &PATTERN (new_insn));
      
      replace_in_rtx_using_bitmask (wherep, src_reg, dst_reg, bitmask);
      
      if (!insn_rtx_valid (new_insn))
        continue;

      new_vi = create_vinsn_from_insn_rtx (new_insn);

      gcc_assert (VINSN_SEPARABLE_P (new_vi) == EXPR_SEPARABLE_P (rhs));
      
      copy_expr (tmp_rhs, rhs);
      change_vinsn_in_expr (tmp_rhs, new_vi);
      
      av_set_add (new_set_ptr, tmp_rhs);
      
      clear_expr (tmp_rhs);
    }
}


/* Functions to support register renaming.  */

/* Substitute VI's set source with REGNO.  Returns newly created pattern
   that has REGNO as its source.  */
static rtx
create_insn_rtx_with_rhs (vinsn_t vi, rtx rhs_rtx)
{
  rtx lhs_rtx;
  rtx pattern;
  rtx insn_rtx;

  lhs_rtx = copy_rtx (VINSN_LHS (vi));

  pattern = gen_rtx_SET (VOIDmode, lhs_rtx, rhs_rtx);
  insn_rtx = create_insn_rtx_from_pattern (pattern, NULL_RTX);

  return insn_rtx;
}

/* Returns whether INSN's src can be replaced with register number 
   NEW_SRC_REG. E.g. the following insn is valid for i386:

    (insn:HI 2205 6585 2207 727 ../../gcc/libiberty/regex.c:3337 
      (set (mem/s:QI (plus:SI (plus:SI (reg/f:SI 7 sp)
			(reg:SI 0 ax [orig:770 c1 ] [770]))
		    (const_int 288 [0x120])) [0 str S1 A8])
	    (const_int 0 [0x0])) 43 {*movqi_1} (nil)
	(nil))

  But if we change (const_int 0 [0x0]) to (reg:QI 4 si), it will be invalid
  because of operand constraints: 

    (define_insn "*movqi_1"
      [(set (match_operand:QI 0 "nonimmediate_operand" "=q,q ,q ,r,r ,?r,m")
	    (match_operand:QI 1 "general_operand"      " q,qn,qm,q,rn,qm,qn")
	    )]
    
  So do constrain_operands here, before choosing NEW_SRC_REG as best 
  reg for rhs.  */

static bool
replace_src_with_reg_ok_p (insn_t insn, rtx new_src_reg)
{
  vinsn_t vi = INSN_VINSN (insn);
  enum machine_mode mode;
  rtx dst_loc;
  bool res;

  gcc_assert (VINSN_SEPARABLE_P (vi));

  get_dest_and_mode (insn, &dst_loc, &mode);
  gcc_assert (mode == GET_MODE (new_src_reg));

  if (REG_P (dst_loc) && REGNO (new_src_reg) == REGNO (dst_loc))
    return true;

  /* See whether SET_SRC can be replaced with this register.  */
  validate_change (insn, &SET_SRC (PATTERN (insn)), new_src_reg, 1);
  res = verify_changes (0);
  cancel_changes (0);

  return res;
}

/* Returns whether INSN still be valid after replacing it's DEST with
   register NEW_REG.  */
static bool
replace_dest_with_reg_ok_p (insn_t insn, rtx new_reg)
{
  vinsn_t vi = INSN_VINSN (insn);
  bool res;

  /* We should deal here only with separable insns.  */
  gcc_assert (VINSN_SEPARABLE_P (vi));
  gcc_assert (GET_MODE (VINSN_LHS (vi)) == GET_MODE (new_reg));

  /* See whether SET_DEST can be replaced with this register.  */
  validate_change (insn, &SET_DEST (PATTERN (insn)), new_reg, 1);
  res = verify_changes (0);
  cancel_changes (0);

  return res;
}

/* Create a pattern with rhs of VI and lhs of LHS_RTX.  */
static rtx
create_insn_rtx_with_lhs (vinsn_t vi, rtx lhs_rtx)
{
  rtx rhs_rtx;
  rtx pattern;
  rtx insn_rtx;

  rhs_rtx = copy_rtx (VINSN_RHS (vi));

  pattern = gen_rtx_SET (VOIDmode, lhs_rtx, rhs_rtx);
  insn_rtx = create_insn_rtx_from_pattern (pattern, NULL_RTX);

  return insn_rtx;
}

/* Substitute lhs in the given expression RHS for the register with number 
   NEW_REGNO.  SET_DEST may be arbitrary rtx, not only register.  */
static void
replace_dest_with_reg_in_rhs (rhs_t rhs, rtx new_reg)
{
  rtx insn_rtx;
  vinsn_t vinsn;

  insn_rtx = create_insn_rtx_with_lhs (RHS_VINSN (rhs), new_reg);
  vinsn = create_vinsn_from_insn_rtx (insn_rtx);

  change_vinsn_in_expr (rhs, vinsn);
}

/* Return a destination register, if any, of EXPR.  */
static rtx
expr_dest_reg (expr_t expr)
{
  rtx dest = VINSN_LHS (RHS_VINSN (expr));

  if (dest != NULL_RTX && REG_P (dest))
    return dest;

  return NULL_RTX;
}

/* Returns the REGNO of the R's destination.  */
static unsigned
rhs_dest_regno (rhs_t r)
{
  rtx dest = expr_dest_reg (r);

  gcc_assert (dest != NULL_RTX);
  return REGNO (dest);
}

/* Returns whether VI writes one of the REGS.  */
static bool
vinsn_writes_one_of_regs_p (vinsn_t vi, regset used_regs, 
                            HARD_REG_SET unavailable_hard_regs)
{
  unsigned regno;
  reg_set_iterator rsi;

  EXECUTE_IF_SET_IN_REG_SET (VINSN_REG_SETS (vi), 0, regno, rsi)
    {
      if (REGNO_REG_SET_P (used_regs, regno))
        return true;
      if (HARD_REGISTER_NUM_P (regno)
          && TEST_HARD_REG_BIT (unavailable_hard_regs, regno))
	return true;
    }

  return false;
}

#if 0
/* True when expressions of MODE are considered for renaming.  */
static inline bool
mode_ok_for_rename_p (enum machine_mode mode)
{
  enum mode_class class = GET_MODE_CLASS (mode);

  return class == MODE_INT || class == MODE_FLOAT;
}
#endif

/* Returns register class of the output register in INSN.  
   Returns NO_REGS for call insns because some targets have constraints on
   destination register of a call insn.
   
   Code adopted from regrename.c::build_def_use.  */
static enum reg_class
get_reg_class (rtx insn)
{
  int alt, i, n_ops;
  int predicated;

  extract_insn (insn);
  if (! constrain_operands (1))
    fatal_insn_not_found (insn);
  preprocess_constraints ();
  alt = which_alternative;
  n_ops = recog_data.n_operands;

  predicated = GET_CODE (PATTERN (insn)) == COND_EXEC;
  for (i = 0; i < n_ops; ++i)
    {
      int matches = recog_op_alt[i][alt].matches;
      if (matches >= 0)
	recog_op_alt[i][alt].cl = recog_op_alt[matches][alt].cl;
    }

  if (asm_noperands (PATTERN (insn)) > 0)
    {
      for (i = 0; i < n_ops; i++)
	if (recog_data.operand_type[i] == OP_OUT)
	  {
	    rtx *loc = recog_data.operand_loc[i];
	    rtx op = *loc;
	    enum reg_class cl = recog_op_alt[i][alt].cl;

	    if (REG_P (op)
		&& REGNO (op) == ORIGINAL_REGNO (op))
	      continue;

	    return cl;
	  }
    }
  else if (!CALL_P (insn))
    {
      for (i = 0; i < n_ops + recog_data.n_dups; i++)
       {
	 int opn = i < n_ops ? i : recog_data.dup_num[i - n_ops];
	 enum reg_class cl = recog_op_alt[opn][alt].cl;
  
	 if (recog_data.operand_type[opn] == OP_OUT ||
	     recog_data.operand_type[opn] == OP_INOUT)
	   return cl;
       }
    }

/*  Insns like
    (insn (set (reg:CCZ 17 flags) (compare:CCZ ...)))
    may result in returning NO_REGS, cause flags is written implicitly through
    CMP insn, which has no OP_OUT | OP_INOUT operands.  */
  return NO_REGS;
}

/* Calculate HARD_REGNO_RENAME_OK data for REGNO.  */
static void
init_hard_regno_rename (int regno)
{
  int cur_reg;

  SET_HARD_REG_BIT (sel_hrd.regs_for_rename[regno], regno);

  for (cur_reg = 0; cur_reg < FIRST_PSEUDO_REGISTER; cur_reg++)
    {
      /* We are not interested in renaming in other regs.  */
      if (!TEST_HARD_REG_BIT (sel_hrd.regs_ever_used, cur_reg))
        continue;
      
      if (HARD_REGNO_RENAME_OK (regno, cur_reg))
        SET_HARD_REG_BIT (sel_hrd.regs_for_rename[regno], cur_reg);
    }
}

/* A wrapper around HARD_REGNO_RENAME_OK that will look into the hard regs 
   data first.  */
static inline bool
sel_hard_regno_rename_ok (int from, int to)
{
#ifdef HARD_REGNO_RENAME_OK
  /* Check whether this is all calculated.  */
  if (TEST_HARD_REG_BIT (sel_hrd.regs_for_rename[from], from))
    return TEST_HARD_REG_BIT (sel_hrd.regs_for_rename[from], to);

  init_hard_regno_rename (from);

  return TEST_HARD_REG_BIT (sel_hrd.regs_for_rename[from], to);
#else
  return true;
#endif
}

/* Calculate set of registers that are capable of holding MODE.  */
static void
init_regs_for_mode (enum machine_mode mode)
{
  int cur_reg;
  
  CLEAR_HARD_REG_SET (sel_hrd.regs_for_mode[mode]);
  CLEAR_HARD_REG_SET (sel_hrd.regs_for_call_clobbered[mode]);

  for (cur_reg = 0; cur_reg < FIRST_PSEUDO_REGISTER; cur_reg++)
    {
      int nregs = hard_regno_nregs[cur_reg][mode];
      int i;
      
      for (i = nregs - 1; i >= 0; --i)
        if (fixed_regs[cur_reg + i]
                || global_regs[cur_reg + i]
            /* Can't use regs which aren't saved by 
               the prologue.  */
            || !TEST_HARD_REG_BIT (sel_hrd.regs_ever_used, cur_reg + i)
#ifdef LEAF_REGISTERS
            /* We can't use a non-leaf register if we're in a
               leaf function.  */
            || (current_function_is_leaf
                && !LEAF_REGISTERS[cur_reg + i])
#endif
            )
          break;
      
      if (i >= 0) 
        continue;
      
      /* See whether it accepts all modes that occur in
         original insns.  */
      if (! HARD_REGNO_MODE_OK (cur_reg, mode))
        continue;
      
      if (HARD_REGNO_CALL_PART_CLOBBERED (cur_reg, mode))
        SET_HARD_REG_BIT (sel_hrd.regs_for_call_clobbered[mode], 
                          cur_reg);
      
      /* If the CUR_REG passed all the checks above, 
         then it's ok.  */
      SET_HARD_REG_BIT (sel_hrd.regs_for_mode[mode], cur_reg);
    }

  sel_hrd.regs_for_mode_ok[mode] = true;
}

/* Init all register sets gathered in HRD.  */
static void
init_hard_regs_data (void)
{
  int cur_reg = 0;
  enum machine_mode cur_mode = 0;

  CLEAR_HARD_REG_SET (sel_hrd.regs_ever_used);
  for (cur_reg = 0; cur_reg < FIRST_PSEUDO_REGISTER; cur_reg++)
    if (df_regs_ever_live_p (cur_reg) || call_used_regs[cur_reg])
      SET_HARD_REG_BIT (sel_hrd.regs_ever_used, cur_reg);
  
  /* Initialize registers that are valid based on mode when this is 
     really needed.  */
  for (cur_mode = 0; cur_mode < NUM_MACHINE_MODES; cur_mode++)
    sel_hrd.regs_for_mode_ok[cur_mode] = false;
  
  /* Mark that all HARD_REGNO_RENAME_OK is not calculated.  */
  for (cur_reg = 0; cur_reg < FIRST_PSEUDO_REGISTER; cur_reg++)
    CLEAR_HARD_REG_SET (sel_hrd.regs_for_rename[cur_reg]);

#ifdef STACK_REGS
  CLEAR_HARD_REG_SET (sel_hrd.stack_regs);

  for (cur_reg = FIRST_STACK_REG; cur_reg <= LAST_STACK_REG; cur_reg++)
    SET_HARD_REG_BIT (sel_hrd.stack_regs, cur_reg);
#endif
} 

/* Mark hardware regs in UNAVAILABLE_HARD_REGS that are not suitable 
   for renaming rhs in INSN due to hardware restrictions (register class,
   modes compatibility etc).  This doesn't affect original insn's dest reg,
   if it isn't in USED_REGS.  DEF is a definition insn of rhs for which the
   destination register is sought.  LHS (DEF->ORIG_INSN) may be REG or MEM.
   Registers that are in used_regs are always marked in
   unavailable_hard_regs as well.  */

static void
mark_unavailable_hard_regs (def_t def, HARD_REG_SET *unavailable_hard_regs,
                            regset used_regs ATTRIBUTE_UNUSED)
{
  enum machine_mode mode;
  enum reg_class cl = NO_REGS;
  rtx orig_dest;
  int cur_reg, regno;
  HARD_REG_SET hard_regs_ok;

  gcc_assert (GET_CODE (PATTERN (def->orig_insn)) == SET);
  gcc_assert (unavailable_hard_regs);

  orig_dest = SET_DEST (PATTERN (def->orig_insn));
  
  /* We have decided not to rename 'mem = something;' insns, as 'something'
     is usually a register.  */
  if (!REG_P (orig_dest))
    return;

  regno = REGNO (orig_dest);

  /* If before reload, don't try to work with pseudos.  */
  if (!reload_completed && !HARD_REGISTER_NUM_P (regno))
    return;

  mode = GET_MODE (orig_dest);

  /* Stop when mode is not supported for renaming.  Also Can't proceed 
     if the original register is one of the fixed_regs, global_regs or 
     frame pointer.  */
  if (fixed_regs[regno] 
      || global_regs[regno]
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
	|| (frame_pointer_needed && regno == HARD_FRAME_POINTER_REGNUM)
#else
	|| (frame_pointer_needed && regno == FRAME_POINTER_REGNUM)
#endif
      )
    {
      SET_HARD_REG_SET (*unavailable_hard_regs);

      /* Give a chance for original register, if it isn't in used_regs.  */
      CLEAR_HARD_REG_BIT (*unavailable_hard_regs, regno);

      return;
    }

  /* If something allocated on stack in this function, mark frame pointer
     register unavailable, considering also modes.  
     FIXME: it is enough to do this once per all original defs.  */
  if (frame_pointer_needed)
    {
      int i;

      for (i = hard_regno_nregs[FRAME_POINTER_REGNUM][Pmode]; i--;)
	SET_HARD_REG_BIT (*unavailable_hard_regs, FRAME_POINTER_REGNUM + i);

#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
      for (i = hard_regno_nregs[HARD_FRAME_POINTER_REGNUM][Pmode]; i--;)
	SET_HARD_REG_BIT (*unavailable_hard_regs, 
                          HARD_FRAME_POINTER_REGNUM + i);
#endif
    }

#ifdef STACK_REGS
  /* For the stack registers the presence of FIRST_STACK_REG in USED_REGS
     is equivalent to as if all stack regs were in this set.
     I.e. no stack register can be renamed, and even if it's an original
     register here we make sure it won't be lifted over it's previous def 
     (it's previous def will appear as if it's a FIRST_STACK_REG def.  
     The HARD_REGNO_RENAME_OK covers other cases in condition below.  */
  if (IN_RANGE (REGNO (orig_dest), FIRST_STACK_REG, LAST_STACK_REG)
      && REGNO_REG_SET_P (used_regs, FIRST_STACK_REG)) 
    IOR_HARD_REG_SET (*unavailable_hard_regs, sel_hrd.stack_regs);
#endif    

  /* If there's a call on this path, make regs from call_used_reg_set 
     unavailable.  */
  if (def->crosses_call)
    IOR_HARD_REG_SET (*unavailable_hard_regs, call_used_reg_set);

  /* Stop here before reload: we need FRAME_REGS, STACK_REGS, and crosses_call, 
     but not register classes.  */
  if (!reload_completed)
    return;

  /* Leave regs as 'available' only from the current 
     register class.  */
  cl = get_reg_class (def->orig_insn);
  gcc_assert (cl != NO_REGS);
  IOR_COMPL_HARD_REG_SET (*unavailable_hard_regs, reg_class_contents[cl]);

  if (!sel_hrd.regs_for_mode_ok[mode])
    init_regs_for_mode (mode);

  /* Leave only registers available for this mode.  */
  CLEAR_HARD_REG_SET (hard_regs_ok);
  IOR_HARD_REG_SET (hard_regs_ok, sel_hrd.regs_for_mode[mode]);

  /* Exclude registers that are partially call clobbered.  */
  if (def->crosses_call
      && ! HARD_REGNO_CALL_PART_CLOBBERED (regno, mode))
    AND_COMPL_HARD_REG_SET (hard_regs_ok, 
                            sel_hrd.regs_for_call_clobbered[mode]);

  /* Leave only those that are ok to rename.  */
  for (cur_reg = 0; cur_reg < FIRST_PSEUDO_REGISTER; cur_reg++)
    {
      int nregs;
      int i;

      if (!TEST_HARD_REG_BIT (hard_regs_ok, cur_reg))
        continue;
      
      nregs = hard_regno_nregs[cur_reg][mode];
      gcc_assert (nregs > 0);

      for (i = nregs - 1; i >= 0; --i)
        if (! sel_hard_regno_rename_ok (regno + i, cur_reg + i))
          break;

      if (i >= 0) 
        CLEAR_HARD_REG_BIT (hard_regs_ok, cur_reg);
    }

  /* Regno is always ok from the renaming part of view, but it really
     could be in *unavailable_hard_regs already, so set it here instead
     of there.  */
  SET_HARD_REG_BIT (hard_regs_ok, regno);

  /* Exclude all hard regs but HARD_REGS_OK.  */
  IOR_COMPL_HARD_REG_SET (*unavailable_hard_regs, hard_regs_ok);
}

/* reg_rename_tick[REG1] > reg_rename_tick[REG2] if REG1 was chosen as the
   best register more recently than REG2.  */
static int reg_rename_tick[FIRST_PSEUDO_REGISTER];

/* Choose the register among free, that is suitable for storing 
   the rhs value.

   ORIGINAL_INSNS is the list of insns where the operation (rhs)
   originally appears.  There could be multiple original operations 
   for single rhs since we moving it up and merging along different 
   paths.

   Some code is adapted from regrename.c (regrename_optimize).
   If original register is available, function returns it.
   Otherwise it performs the checks, so the new register should
   comply with the following:
    - it should not be in the UNAVAILABLE set;
    - it should be in the class compatible with original uses;
    - it should not be clobbered through reference with different mode;
    - if we're in the leaf function, then the new register should 
      not be in the LEAF_REGISTERS;
    - etc.

   Most of this conditions are checked in find_used_regs_1, and
   unavailable registers due to this restrictions are already included
   in UNAVAILABLE set.

   If several registers meet the conditions, the register with smallest
   tick is returned to achieve more even register allocation.

   If no register satisfies the above conditions, NULL_RTX is returned.  */
static rtx
choose_best_reg_1 (HARD_REG_SET unavailable, def_list_t original_insns,
		   bool *is_orig_reg_p_ptr)
{
  int best_new_reg;
  int cur_reg;
  enum machine_mode mode = VOIDmode;
  def_list_iterator i;
  def_t def;

  /* If original register is available, return it.  */
  *is_orig_reg_p_ptr = true;

  FOR_EACH_DEF (def, i, original_insns)
    {
      rtx orig_dest = SET_DEST (PATTERN (def->orig_insn));

      gcc_assert (REG_P (orig_dest));

      /* Check that all original operations have the same mode.  
         This is done for the next loop; if we'd return from this
         loop, we'd check only part of them, but in this case 
         it doesn't matter.  */
      if (mode == VOIDmode)
        mode = GET_MODE (orig_dest);
      gcc_assert (mode == GET_MODE (orig_dest));

      if (!TEST_HARD_REG_BIT (unavailable, REGNO (orig_dest)))
        {
          gcc_assert (mode != VOIDmode);

          /* Hard registers should not be shared.  */
	  return gen_rtx_REG (mode, REGNO (orig_dest));
        }
    }

  *is_orig_reg_p_ptr = false;

  best_new_reg = -1;

  /* Among all available regs choose the register that was 
     allocated earliest.  */
  for (cur_reg = 0; cur_reg < FIRST_PSEUDO_REGISTER; cur_reg++)
    if (!TEST_HARD_REG_BIT (unavailable, cur_reg))
      {
	if (best_new_reg < 0
	    ||reg_rename_tick[cur_reg] < reg_rename_tick[best_new_reg])
	  best_new_reg = cur_reg;
      }

  if (best_new_reg >= 0)
    {
      /* Use the check from the above loop.  */
      gcc_assert (mode != VOIDmode);
      return gen_rtx_REG (mode, best_new_reg);
    }

  return NULL_RTX;
}

/* A wrapper around choose_best_reg_1 () to verify that we make correct
   assumptions about available registers in the function.  */
static rtx
choose_best_reg (HARD_REG_SET unavailable, def_list_t original_insns,
		 bool *is_orig_reg_p_ptr)
{
  rtx best_reg = choose_best_reg_1 (unavailable, original_insns,
				    is_orig_reg_p_ptr);

  gcc_assert (best_reg == NULL_RTX
	      || TEST_HARD_REG_BIT (sel_hrd.regs_ever_used, REGNO (best_reg)));

  return best_reg;
}

/* Choose the pseudo register for storing rhs value.  As this is supposed 
   to work before reload, we return either the original register or make
   the new one.  If we work with hard regs, check also UNAVAILABLE_HARD_REGS.

   TODO: take into account register pressure while doing this.  Up to this 
   moment, this function would never return NULL for pseudos, but we should 
   not rely on this.  */
static rtx
choose_best_pseudo_reg (regset used_regs, 
                        HARD_REG_SET unavailable_hard_regs, 
                        def_list_t original_insns, bool *is_orig_reg_p_ptr)
{
  def_list_iterator i;
  def_t def;
  enum machine_mode mode = VOIDmode;
  bool bad_hard_regs = false;
  
  /* We should not use this after reload.  */
  gcc_assert (!reload_completed);

  /* If original register is available, return it.  */
  *is_orig_reg_p_ptr = true;

  FOR_EACH_DEF (def, i, original_insns)
    {
      rtx dest = SET_DEST (PATTERN (def->orig_insn));
      int orig_regno;
      
      gcc_assert (REG_P (dest));
      
      /* Check that all original operations have the same mode.  */
      if (mode == VOIDmode)
        mode = GET_MODE (dest);
      else
        gcc_assert (mode == GET_MODE (dest));
      orig_regno = REGNO (dest);
      
      if (!REGNO_REG_SET_P (used_regs, orig_regno))
        {
          if (orig_regno < FIRST_PSEUDO_REGISTER)
            {
              gcc_assert (df_regs_ever_live_p (orig_regno));
              
              /* For hard registers, we have to check hardware imposed 
                 limitations (frame/stack registers, calls crossed).  */
              if (!TEST_HARD_REG_BIT (unavailable_hard_regs, orig_regno))
                return gen_rtx_REG (mode, orig_regno);
              
              bad_hard_regs = true;
            }
          else
            return dest;
        }
     }

  *is_orig_reg_p_ptr = false;
 
  /* We had some original hard registers that couldn't be used.
     Those were likely special.  Don't try to create a pseudo.  */
  if (bad_hard_regs)
    return NULL_RTX;
  
  /* We haven't found a register from original operations.  Get a new one.  
     FIXME: control register pressure somehow.  */
  gcc_assert (mode != VOIDmode);
  return gen_reg_rtx (mode);
}

/* Returns best register for given rhs, or NULL_RTX, if no register can be
   chosen.  The latter could happen when:
     - RHS_SCHEDULE_AS_RHS is true but we were unable to find suitable
       register;
     - RHS_SCHEDULE_AS_RHS is false but the insn sets/clobbers one of
       the registers that are used on the moving path.  */
static bool
find_best_reg_for_rhs (rhs_t rhs, blist_t bnds)
{
  av_set_iterator i2;
  rhs_t rhs_orig;
  regset used_regs;
  HARD_REG_SET hard_regs_used, unavailable_hard_regs;
  rtx best_reg = NULL_RTX;
  blist_t bnds1 = bnds;
  def_list_t original_insns = NULL;
  int res = 0;
  bool reg_ok = true;
  bool is_orig_reg_p = false;

  /* Don't bother to do anything if this insn doesn't set any registers.  */
  if (bitmap_empty_p (VINSN_REG_SETS (EXPR_VINSN (rhs))))
    return true;

  used_regs = get_clear_regset_from_pool ();
  CLEAR_HARD_REG_SET (unavailable_hard_regs);

  /* Collect unavailable registers from all boundaries into USED_REGS.  */
  do
    {
      av_set_t orig_ops = NULL;
      bnd_t bnd = BLIST_BND (bnds1);

      /* If the chosen best rhs doesn't belong to current boundary, 
         skip it.  */
      if (!av_set_is_in_p (BND_AV1 (bnd), RHS_VINSN (rhs)))
	continue;

      /* Put in ORIG_OPS all rhses from this boundary that became
	 RES on top.  */
      FOR_EACH_RHS (rhs_orig, i2, BND_AV (bnd))
	{
          ilist_t root = enable_moveup_set_path_p ? BND_PTR (bnd) : NULL;

	  if (equal_after_moveup_path_p (rhs_orig, root, rhs))
	    av_set_add (&orig_ops, rhs_orig);
	}

      /* Compute used regs and OR it into the USED_REGS.  */
      res = find_used_regs (BND_TO (bnd), orig_ops, used_regs, 
                            &unavailable_hard_regs, &original_insns);

      av_set_clear (&orig_ops);

      if (res)
	{
          if (false)
            /* FIXME: conditionalize dumping of regsets.  */
	    {
	      dump_hard_reg_set ("unavailable hard regs: ",
				 unavailable_hard_regs);
	      print ("unavailable regs (live issues): ");
	      dump_used_regs (used_regs);
	    }
	}
      else
	{
	  gcc_unreachable ();
	  print ("Unable to find original op with find_used_regs.");
	  break;
	}
    }
  while ((bnds1 = BLIST_NEXT (bnds1)));

  if (res)
    {
#ifdef ENABLE_CHECKING
      /* If after reload, make sure we're working with hard regs here.  */
      if (reload_completed) {
	reg_set_iterator rsi;
	unsigned i;
	EXECUTE_IF_SET_IN_REG_SET (used_regs, FIRST_PSEUDO_REGISTER, i, rsi)
	  gcc_unreachable ();
      }
#endif

      if (EXPR_SEPARABLE_P (rhs))
	{
          /* Turn everything in hard regs after reload.  */
          if (reload_completed)
            {
              REG_SET_TO_HARD_REG_SET (hard_regs_used, used_regs);
              /* Join hard registers unavailable due to register class 
                 restrictions and live range intersection.  */
              IOR_HARD_REG_SET (hard_regs_used, unavailable_hard_regs);
              best_reg = choose_best_reg (hard_regs_used, original_insns,
					  &is_orig_reg_p);
            }
          else
            best_reg = choose_best_pseudo_reg (used_regs, 
                                               unavailable_hard_regs, 
                                               original_insns,
					       &is_orig_reg_p);

	  if (!is_orig_reg_p && sel_vinsn_cost (EXPR_VINSN (rhs)) < 2)
	    best_reg = NULL_RTX;

	  if (best_reg != NULL_RTX)
	    /* Try whether we'll be able to generate the insn
	       'dest := best_reg' at the place of the original operation.  */
	    {
	      ilist_t p = original_insns;
    
	      gcc_assert (original_insns);
    
	      while (p) 
		{
		  def_t def = DEF_LIST_DEF (p);
    
		  gcc_assert (EXPR_SEPARABLE_P (INSN_EXPR (def->orig_insn)));
    
		  if (!replace_src_with_reg_ok_p (def->orig_insn, best_reg)
		      || !replace_dest_with_reg_ok_p (def->orig_insn,
						      best_reg))
		    {
		      /* Insn will be removed from rhs_vliw below.  
                         FIXME: may be it will work with other regs?  */
		      reg_ok = false;
		      break;
		    }

		  p = ILIST_NEXT (p);
		}		
		
	      if (reg_ok)
		{
		  RGN_HAS_RENAMING_P (CONTAINING_RGN (BB_TO_BLOCK (0))) = 1;

		  /* Make sure that RHS has the right destination
		     register.  */
		  if (rhs_dest_regno (rhs) != REGNO (best_reg))
		    {
		      replace_dest_with_reg_in_rhs (rhs, best_reg);

		      /* The resulting insn should be valid.  */
		      if (!insn_rtx_valid (RHS_INSN (rhs)))
			gcc_unreachable ();
		    }
		}
	    }
	  else
	    reg_ok = false;
    	}
      else
	{
	  /* If !RHS_SCHEDULE_AS_RHS (RHS), just make sure INSN doesn't set
	     any of the HARD_REGS_USED set.  */
	  if (vinsn_writes_one_of_regs_p (RHS_VINSN (rhs), used_regs,
                                          unavailable_hard_regs))
	    reg_ok = false;
	  else
	    {
	      gcc_assert (reg_ok);
	      best_reg = NULL_RTX;
	    }
	}
    }
  else
    reg_ok = false;

  ilist_clear (&original_insns);
  return_regset_to_pool (used_regs);

  return reg_ok;
}


/* Flag to enable / disable ia64 speculation.  */
static bool sel_speculation_p = true;

/* Return true if dependence described by DS can be overcomed.  */
static bool
can_overcome_dep_p (ds_t ds)
{
  if (spec_info == NULL || !sel_speculation_p)
    return false;

  /* Leave only speculative data.  */
  ds &= SPECULATIVE;

  if (ds == 0)
    return false;

  {
    /* !!! FIXME:
       1. Make sched-deps.c produce only those non-hard dependencies,
       that we can overcome.
       2. Introduce several machine flags to turn on/off different kinds of
       speculation.  */
    ds_t spec_mask = spec_info->mask;

    if ((ds & spec_mask) != ds)
      return false;
  }

  if (ds_weak (ds) < spec_info->weakness_cutoff)
    return false;

  return true;
}

static bool speculate_expr (expr_t, ds_t);

/* Get a speculation check instruction.
   C_RHS is a speculative expression,
   CHECK_DS describes speculations that should be checked,
   ORIG_INSN is the original non-speculative insn in the stream.  */
static insn_t
create_speculation_check (expr_t c_rhs, ds_t check_ds, insn_t orig_insn)
{
  rtx check_pattern;
  rtx insn_rtx;
  insn_t insn;
  basic_block recovery_block;
  rtx label;

  sel_dump_cfg ("before-gen-spec-check");

  /* Create a recovery block if target is going to emit branchy check.  */
  if (targetm.sched.needs_block_p (check_ds))
    {
      recovery_block = sel_create_recovery_block (orig_insn);
      label = BB_HEAD (recovery_block);
    }
  else
    {
      recovery_block = NULL;
      label = NULL_RTX;
    }

  /* Get pattern of the check.  */
  check_pattern = targetm.sched.gen_spec_check (EXPR_INSN_RTX (c_rhs), label,
						check_ds);

  gcc_assert (check_pattern != NULL);

  /* Emit check.  */
  insn_rtx = create_insn_rtx_from_pattern (check_pattern, label);

  insn = sel_gen_insn_from_rtx_after (insn_rtx, INSN_EXPR (orig_insn),
				      INSN_SEQNO (orig_insn), orig_insn);

  /* Make check to be non-speculative.  */
  EXPR_SPEC_DONE_DS (INSN_EXPR (insn)) &= ~check_ds;
  INSN_SPEC_CHECKED_DS (insn) = check_ds;

  if (recovery_block != NULL)
    /* Emit copy of original insn (though with replaced target register,
       if needed) to the recovery block.  */
    {
      rtx twin_rtx;
      insn_t twin;

      twin_rtx = copy_rtx (PATTERN (EXPR_INSN_RTX (c_rhs)));
      twin_rtx = create_insn_rtx_from_pattern (twin_rtx, NULL_RTX);
      twin = sel_gen_recovery_insn_from_rtx_after (twin_rtx, INSN_EXPR (insn),
						   INSN_SEQNO (insn),
						   bb_note (recovery_block));
    }

  /* If we've generated a data speculation check, make sure
     that all the bookkeeping instruction we'll create during
     this move_op () will allocate an ALAT entry so that the
     check won't fail.
     In case of control speculation we must convert C_RHS to control
     speculative mode, because failing to do so will bring us an exception
     thrown by the non-control-speculative load.  */
  {
    check_ds = ds_get_max_dep_weak (check_ds);
    speculate_expr (c_rhs, check_ds);
  }

  sel_dump_cfg ("after-gen-spec-check");

  return insn;
}

/* Try to transform EXPR to data speculative version.  Return true on
   success.  */
static bool
apply_spec_to_expr (expr_t expr, ds_t ds)
{
  int res;
  rtx orig_insn_rtx;
  rtx spec_pat;

  orig_insn_rtx = EXPR_INSN_RTX (expr);

  res = sched_speculate_insn (orig_insn_rtx, ds, &spec_pat);

  switch (res)
    {
    case 0:
      spec_pat = copy_rtx (PATTERN (orig_insn_rtx));

      /* FALLTHRU */

    case 1:
      {
	rtx spec_insn_rtx = create_insn_rtx_from_pattern (spec_pat, NULL_RTX);
	vinsn_t spec_vinsn = create_vinsn_from_insn_rtx (spec_insn_rtx);

	change_vinsn_in_expr (expr, spec_vinsn);
	EXPR_SPEC_DONE_DS (expr) = ds;

	return true;
      }

    case -1:
      return false;

    default:
      gcc_unreachable ();
      return false;
    }
}

/* Try to make EXPR speculative and on success remove from DSP those
   dependencies that were overcame.  Return true on success.  */
static bool
speculate_expr (expr_t expr, ds_t ds)
{
  ds_t target_ds = (ds & SPECULATIVE);
  ds_t current_ds = EXPR_SPEC_DONE_DS (expr);
  ds_t combined_ds = ds_full_merge (current_ds, target_ds, NULL_RTX, NULL_RTX);

  if (apply_spec_to_expr (expr, combined_ds))
    /* We already have all necessary speculations.  */
    return true;

  return false;
}

/* Return true if there is a speculative dependence between INSN and EXPR.  */
static ds_t
has_spec_dependence_p (expr_t expr, insn_t insn)
{
  ds_t *has_dep_p;
  ds_t full_ds;

  full_ds = has_dependence_p (expr, insn, &has_dep_p);

  if (full_ds == 0)
    return 0;

  if (EXPR_SEPARABLE_P (expr)
      && can_overcome_dep_p (has_dep_p [DEPS_IN_RHS]))
    return has_dep_p [DEPS_IN_RHS];

  if (can_overcome_dep_p (full_ds))
    return full_ds;

  return 0;
}

/* Record speculations that EXPR should perform in order to be moved through
   INSN.  */
static void
un_speculate (expr_t expr, insn_t insn)
{
  ds_t expr_spec_done_ds;
  ds_t full_ds;

  if (spec_info == NULL || !sel_speculation_p)
    return;
  
  expr_spec_done_ds = EXPR_SPEC_DONE_DS (expr);
  if (expr_spec_done_ds == 0)
    return;

  full_ds = has_spec_dependence_p (expr, insn);

  if (full_ds == 0)
    return;

  full_ds = ds_get_speculation_types (full_ds);
  EXPR_SPEC_TO_CHECK_DS (expr) |= full_ds;
}


/* True when INSN is a "regN = regN" copy.  */
static bool
identical_copy_p (rtx insn)
{
  rtx lhs, rhs, pat;

  pat = PATTERN (insn);

  if (GET_CODE (pat) != SET)
    return false;

  lhs = SET_DEST (pat);
  if (!REG_P (lhs))
    return false;

  rhs = SET_SRC (pat);
  if (!REG_P (rhs))
    return false;

  return REGNO (lhs) == REGNO (rhs);
}

/* Undo all transformations on *AV_PTR that were done when 
   moving through INSN.  */
static void
undo_transformations (av_set_t *av_ptr, rtx insn)
{
  av_set_iterator av_iter;
  rhs_t rhs;
  unsigned hash;
  av_set_t new_set = NULL;

  /* First, kill any RHS that uses registers set by an insn.  This is 
     required for correctness.  */
  FOR_EACH_RHS_1 (rhs, av_iter, av_ptr)
    if (!sched_insns_conditions_mutex_p (insn, RHS_INSN (rhs))
        && bitmap_intersect_p (INSN_REG_SETS (insn), 
                               VINSN_REG_USES (EXPR_VINSN (rhs)))
        /* When an insn looks like 'r1 = r1', we could substitute through
           it, but the above condition will still hold.  This happened with
           gcc.c-torture/execute/961125-1.c.  */ 
        && !identical_copy_p (insn))
      {
        dump_rhs (rhs);
        print ("- removed due to use/set conflict\n");
        av_set_iter_remove (&av_iter);
      }

  hash = VINSN_HASH (INSN_VINSN (insn));
  /* FIXME: we need to determine whether RHS was changed on this insn 
     just once.  */
  FOR_EACH_RHS (rhs, av_iter, *av_ptr)
    {
      if (find_in_hash_vect (EXPR_CHANGED_ON_INSNS (rhs), hash) >= 0)
        un_speculate (rhs, insn);
    }

  new_set = NULL;

  FOR_EACH_RHS (rhs, av_iter, *av_ptr)
    {
      if (find_in_hash_vect (EXPR_CHANGED_ON_INSNS (rhs), hash) >= 0)
        un_substitute (rhs, insn, &new_set);
    }
  
  av_set_union_and_clear (av_ptr, &new_set);
}


/* Moveup_* helpers for code motion and computing av sets.  */

/* Propagates INSN_TO_MOVE_UP inside an insn group through THROUGH_INSN.
   The difference from the below function is that only substitution is 
   performed.  */
static enum MOVEUP_RHS_CODE
moveup_rhs_inside_insn_group (rhs_t insn_to_move_up, insn_t through_insn)
{
  vinsn_t vi = RHS_VINSN (insn_to_move_up);
  insn_t insn = VINSN_INSN (vi);
 ds_t *has_dep_p;
  ds_t full_ds;

  full_ds = has_dependence_p (insn_to_move_up, through_insn, &has_dep_p);

  if (full_ds == 0)
    return MOVEUP_RHS_SAME;

  /* Substitution is the only cure in this case.  */
  if (has_dep_p[DEPS_IN_RHS])
    {
      /* Can't substitute UNIQUE VINSNs.  */
      gcc_assert (!VINSN_UNIQUE_P (vi));
      
      if (flag_sel_sched_substitution
          && insn_eligible_for_subst_p (through_insn))
	{
	  /* Substitute in vinsn.  */
	  line_start ();
	  print ("Substituting in moveup_rhs inside insn group:\nBefore: ");
	  sel_print_rtl (insn);
	  print (" Moving through: ");
	  sel_print_rtl (through_insn);
	  print (" After: ");

	  if (substitute_reg_in_rhs (insn_to_move_up, through_insn))
            {
              sel_print_rtl (EXPR_INSN_RTX (insn_to_move_up));
	      EXPR_WAS_SUBSTITUTED (insn_to_move_up) = true;
              line_finish ();
          
	      return MOVEUP_RHS_CHANGED;
            }
          else
            print (" Can't move up due to architecture constraints.\n");
          
	  line_finish ();
	}
    }

  return MOVEUP_RHS_NULL;
}

#define CANT_MOVE_TRAPPING(insn_to_move_up, through_insn)     \
  (VINSN_MAY_TRAP_P (EXPR_VINSN (insn_to_move_up))            \
   && !sel_insn_has_single_succ_p ((through_insn), SUCCS_ALL) \
   && !sel_insn_is_speculation_check (through_insn))

/* Modifies INSN_TO_MOVE_UP so it can be moved through the THROUGH_INSN,
   performing necessary transformations.  When INSIDE_INSN_GROUP, 
   permit all dependencies except true ones, and try to remove those
   too via forward substitution.  All cases when a non-eliminable 
   non-zero cost dependency exists inside an insn group will be fixed 
   in tick_check_p instead.  */
static enum MOVEUP_RHS_CODE
moveup_rhs (rhs_t insn_to_move_up, insn_t through_insn, bool inside_insn_group)
{
  vinsn_t vi = RHS_VINSN (insn_to_move_up);
  insn_t insn = VINSN_INSN (vi);
  ds_t *has_dep_p;
  ds_t full_ds;

  /* When inside_insn_group, delegate to the helper.  */
  if (inside_insn_group)
    return moveup_rhs_inside_insn_group (insn_to_move_up, through_insn);

  if (VINSN_UNIQUE_P (vi))
    {
      if (/* Don't move branches for now.  */
	  control_flow_insn_p (insn))
	return MOVEUP_RHS_NULL;

      if (CANT_MOVE (insn)
	  && BLOCK_FOR_INSN (through_insn) != BLOCK_FOR_INSN (insn))
	/* Don't move what we can't move.  */
	return MOVEUP_RHS_NULL;

      if (SCHED_GROUP_P (insn))
	/* Don't move SCHED_GROUP instruction through anything.
	   If we don't force this, then it will be possible to start
	   scheduling a sched_group before all its dependencies are
	   resolved.
	   ??? Haifa deals with this issue by delaying the SCHED_GROUP
	   as late as possible through rank_for_schedule ().  */
	return MOVEUP_RHS_NULL;
    }
  else
    gcc_assert (!control_flow_insn_p (insn));

  full_ds = has_dependence_p (insn_to_move_up, through_insn, &has_dep_p);

  if (full_ds == 0)
    {
      if (!CANT_MOVE_TRAPPING (insn_to_move_up, through_insn))
	return MOVEUP_RHS_SAME;
    }
  else
    {
      if (VINSN_UNIQUE_P (vi))
	/* We can move UNIQUE insn up only as a whole and unchanged, 
	   so it shouldn't have any dependencies.  */
	return MOVEUP_RHS_NULL;
    }

  if (full_ds != 0 && can_overcome_dep_p (full_ds))
    {
      if (speculate_expr (insn_to_move_up, full_ds))
	/* Speculation was successful.  */
	{
	  full_ds = 0;
	  sel_clear_has_dependence ();
	}
    }

  if (has_dep_p[DEPS_IN_INSN])
    /* We have some dependency that cannot be discarded.  */
    return MOVEUP_RHS_NULL;

  if (has_dep_p[DEPS_IN_LHS] && !EXPR_SEPARABLE_P (insn_to_move_up))
    return MOVEUP_RHS_NULL;

  /* At this point we have either separable insns, that will be lifted
     up only as RHSes, or non-separable insns with no dependency in lhs.
     If dependency is in RHS, then try perform substitution and move up
     substituted RHS:

      Ex. 1:				  Ex.2
	y = x;				    y = x;
	z = y*2;			    y = y*2;

    In Ex.1 y*2 can be substituted for x*2 and the whole operation can be 
    moved above y=x assignment as z=x*2.

    In Ex.2 y*2 also can be substituted for x*2, but only the right hand 
    side can be moved because of the output dependency.  The operation was
    cropped to its rhs above.  */
  if (has_dep_p[DEPS_IN_RHS])
    {
      ds_t *rhs_dsp = &has_dep_p[DEPS_IN_RHS];

      /* Can't substitute UNIQUE VINSNs.  */
      gcc_assert (!VINSN_UNIQUE_P (vi));

      if (can_overcome_dep_p (*rhs_dsp))
	{
	  if (speculate_expr (insn_to_move_up, *rhs_dsp))
            /* Speculation was successful.  */
            *rhs_dsp = 0;
	  else
	    return MOVEUP_RHS_NULL;
	}
      else if (flag_sel_sched_substitution
	       && insn_eligible_for_subst_p (through_insn))
	{
	  /* Substitute in vinsn.  */
	  line_start ();
	  print ("Substituting in moveup_rhs:\nBefore: ");
	  sel_print_rtl (insn);
	  print ("Moving through: ");
	  sel_print_rtl (through_insn);
	  print ("After: ");

	  if (substitute_reg_in_rhs (insn_to_move_up, through_insn))
	    {
	      EXPR_WAS_SUBSTITUTED (insn_to_move_up) = true;
	      sel_print_rtl (EXPR_INSN_RTX (insn_to_move_up));
	    }
	  else
	    {
	      print ("Can't move up due to architecture constraints.\n");
	      line_finish ();
	      return MOVEUP_RHS_NULL;
	    }

	  line_finish ();
	}
      else
	return MOVEUP_RHS_NULL;
    }

  /* Don't move trapping insns through jumps.
     This check should be at the end to give a chance to control speculation
     to perform its duties.  */
  if (CANT_MOVE_TRAPPING (insn_to_move_up, through_insn))
    return MOVEUP_RHS_NULL;

  return MOVEUP_RHS_CHANGED;
}


/* Moves an av set AVP up through INSN, performing necessary 
   transformations.  */
static void
moveup_set_rhs (av_set_t *avp, insn_t insn, bool inside_insn_group)
{
  av_set_iterator i;
  rhs_t rhs;

  print ("moveup_set_rhs (%d)", INSN_UID (insn));
  block_start ();

  FOR_EACH_RHS_1 (rhs, i, avp)    
    { 
      int rhs_uid = INSN_UID (EXPR_INSN_RTX (rhs));
      bool unique_p = VINSN_UNIQUE_P (RHS_VINSN (rhs));

      line_start ();
      dump_rhs (rhs);

      /* First check whether we've analyzed this situation already.  */
      if (bitmap_bit_p (INSN_ANALYZED_DEPS (insn), rhs_uid))
        {
          if (bitmap_bit_p (INSN_FOUND_DEPS (insn), rhs_uid))
            {
              av_set_iter_remove (&i);
              print (" - removed (cached)");
            }
          else
            print (" - unchanged (cached)");

          line_finish ();
          continue;
        }
      
      switch (moveup_rhs (rhs, insn, inside_insn_group))
	{
	case MOVEUP_RHS_NULL:
          /* Cache that there is a hard dependence.  */
          if (!unique_p)
            {
              bitmap_set_bit (INSN_ANALYZED_DEPS (insn), rhs_uid);
              bitmap_set_bit (INSN_FOUND_DEPS (insn), rhs_uid);
            }

	  av_set_iter_remove (&i);
	  print (" - removed");
	  break;
	case MOVEUP_RHS_CHANGED:
	  print (" - changed");
          
          /* Mark that this insn changed this expr.  */
          insert_in_hash_vect (&EXPR_CHANGED_ON_INSNS (rhs), 
                               VINSN_HASH (INSN_VINSN (insn)));

	  {
	    rhs_t rhs2 = av_set_lookup_other_equiv_rhs (*avp, RHS_VINSN (rhs));

	    /* If the resulting insn after substitution is already in av_set,
	       remove it.  */
	    if (rhs2 != NULL)
	      {
		merge_expr (rhs2, rhs);

		av_set_iter_remove (&i);
		print (" and removed.");

		rhs = rhs2;
	      }

	    print (" result: ");
	    dump_rhs (rhs);
	  }
	  break;
	case MOVEUP_RHS_SAME:
          /* Cache that there is a no dependence.  */
          if (!unique_p)
            {
              bitmap_set_bit (INSN_ANALYZED_DEPS (insn), rhs_uid);
              bitmap_clear_bit (INSN_FOUND_DEPS (insn), rhs_uid);
            }

	  print (" - unchanged");
	  break;
	default:
	  gcc_unreachable ();
	}

      line_finish ();
    }

  block_finish ();
}

/* Moves an av set AVP through PATH.  A helper for moveup_set_path.
   FIXME: kill recursion.  */
static void
moveup_set_path_1 (av_set_t *avp, ilist_t path)
{
  if (ILIST_NEXT (path))
    moveup_set_path_1 (avp, ILIST_NEXT (path));

  moveup_set_rhs (avp, ILIST_INSN (path), true);
}

/* Moves AVP set along PATH.  */
static void
moveup_set_path (av_set_t *avp, ilist_t path)
{
  print ("moveup_set_path");
  block_start ();

  if (path)
    {
      ilist_t path1 = ilist_invert (path);

      moveup_set_path_1 (avp, path1);
      ilist_clear (&path1);
    }
  
  block_finish ();
}

/* A helper to equal_after_moveup_path_p.  
   FIXME: kill recursion.  */
static bool
equal_after_moveup_path_p_1 (rhs_t rhs, ilist_t path)
{
  bool res;

  if (ILIST_NEXT (path))
    res = equal_after_moveup_path_p_1 (rhs, ILIST_NEXT (path));
  else
    res = true;

  if (res)
    res = (moveup_rhs (rhs, ILIST_INSN (path), true) != MOVEUP_RHS_NULL);

  return res;
}

/* Returns true if after moving CUR_RHS along PATH it equals to RHS_VLIW.  */
static bool
equal_after_moveup_path_p (rhs_t cur_rhs, ilist_t path, rhs_t rhs_vliw)
{
  expr_def _tmp, *tmp = &_tmp;
  bool res_p;

  copy_expr_onside (tmp, cur_rhs);

  if (path)
    {
      ilist_t path1 = ilist_invert (path);

      res_p = equal_after_moveup_path_p_1 (tmp, path1);
      ilist_clear (&path1);
    }
  else
    res_p = true;

  if (res_p)
    {
      vinsn_t tmp_vinsn = RHS_VINSN (tmp);
      vinsn_t rhs_vliw_vinsn = RHS_VINSN (rhs_vliw);

      if (tmp_vinsn != rhs_vliw_vinsn)
	res_p = vinsns_correlate_as_rhses_p (tmp_vinsn, rhs_vliw_vinsn);
    }

  clear_expr (tmp);

  return res_p;
}


/* Functions that compute av and lv sets.  */

/* Compute av set before INSN.
   INSN - the current operation (actual rtx INSN)
   P - the current path, which is list of insns visited so far
   WS - software lookahead window size.
   UNIQUE_P - TRUE, if returned av_set will be changed, hence
   if we want to save computed av_set in s_i_d, we should make a copy of it.

   In the resulting set we will have only expressions that don't have delay
   stalls and nonsubstitutable dependences.  */
static av_set_t
compute_av_set (insn_t insn, ilist_t p, int ws, bool unique_p)
{
  av_set_t av1;
  av_set_t rhs_in_all_succ_branches;
  int succs_n, real_succs_n;
  insn_t *succs;
  int succ;

  line_start ();
  print ("compute_av_set");
  dump_insn (insn);
  line_finish ();
  block_start ();

  /* Return NULL if insn is not on the legitimate downward path.  */
  if (is_ineligible_successor (insn, p))
    {
      print ("ineligible_successor (%d)", INSN_UID (insn));
      block_finish ();
      return NULL;
    }

  /* If insn already has valid av(insn) computed, just return it.  */ 
  if (AV_SET_VALID_P (insn))
    {
      av_set_t av_set;

      if (sel_bb_head_p (insn))
	av_set = BB_AV_SET (BLOCK_FOR_INSN (insn));
      else
	av_set = NULL;

      line_start ();
      print ("found valid av (%d): ", INSN_UID (insn));
      dump_av_set (av_set);
      line_finish ();
      block_finish ();

      return unique_p ? av_set_copy (av_set) : av_set;
    }

  /* If the window size exceeds at insn during the first computation of 
     av(group), leave a window boundary mark at insn, so further 
     update_data_sets calls do not compute past insn.  */
  if (ws > MAX_WS && !sel_bb_head_p (insn))
    {
      print ("Max software lookahead window size reached");
      
      /* We can reach max lookahead size at bb_header, so clean av_set 
	 first.  */
      INSN_WS_LEVEL (insn) = global_level;
      block_finish ();
      return NULL;
    }

  cfg_succs (insn, &succs, &succs_n);

  /* If there are successors that lead out of the region, then all rhses from
     the below av_sets should be speculative.  */
  real_succs_n = cfg_succs_n (insn, SUCCS_ALL);

  /* Debug output.  */
  line_start ();
  print ("successors (%d): ", INSN_UID (insn));
  dump_insn_array (succs, succs_n);
  line_finish ();

  /* Add insn to to the tail of current path.  */
  ilist_add (&p, insn);
  /* If there are any successors.  */
  /* Create empty av_set and make union of successors' av_sets or add rhs
     for current instruction to it later.  */
  /* TODO: optimize - do copy_shallow instead of union for 
     num_successors == 1.  */
  /* FIXME: avoid recursion here wherever possible, e.g.
     when num_successors == 1.  */
  av1 = NULL;
  rhs_in_all_succ_branches = NULL;

  for (succ = 0; succ < succs_n; succ++)
    {
      av_set_t succ_set;

      /* We will edit SUCC_SET and RHS_SPEC field of its elements.  */
      succ_set = compute_av_set (succs[succ], p, ws + 1, true);

      if (real_succs_n > 1)
	{
          /* Consider successors not from the current region.  */
          if (real_succs_n == succs_n)
            {
              /* Find RHS'es that came from *all* successors and save them 
                 into rhs_in_all_succ_branches.  This set will be used later
                 for calculating speculation attributes of RHS'es.  */
              if (succ == 0)
                rhs_in_all_succ_branches = av_set_copy (succ_set);
              else
                {
                  av_set_iterator i;
                  rhs_t rhs;
  
                  FOR_EACH_RHS_1 (rhs, i, &rhs_in_all_succ_branches)
                    if (!av_set_is_in_p (succ_set, RHS_VINSN (rhs)))
                      av_set_iter_remove (&i);
                }
            }
	}

      /* Union the av_sets.  */
      av_set_union_and_clear (&av1, &succ_set);
    }

  if (real_succs_n > 1)
    {
      av_set_iterator i;
      rhs_t rhs;
      
      /* Increase the spec attribute of all RHS'es that didn't come 
	 from all successors.  */
      FOR_EACH_RHS (rhs, i, av1)
	if (!av_set_is_in_p (rhs_in_all_succ_branches, RHS_VINSN (rhs)))
	  RHS_SPEC (rhs)++;

      av_set_clear (&rhs_in_all_succ_branches);
      
      /* Do not move conditional branches through other 
	 conditional branches.  So, remove all conditional 
	 branches from av_set if current operator is a conditional
	 branch.  */
      av_set_substract_cond_branches (&av1);
    }
  
  free (succs);
  ilist_remove (&p);

  line_start ();
  print ("av_succs (%d): ", INSN_UID (insn));
  dump_av_set (av1);
  line_finish ();

  /* Then, compute av1 above insn.  */
  if (!INSN_NOP_P (insn))
    {
      expr_t expr;

      moveup_set_rhs (&av1, insn, false);
      
      expr = av_set_lookup (av1, INSN_VINSN (insn));

      if (expr != NULL)
	/* ??? It is not clear if we should replace or merge exprs in this
	   case.  But it looks reasonable to me that if we unify (merge) all
	   operations beyond current point then we should find them all during
	   move_op () - which is not the case yet.  On the other hand if during
	   move_op () we find only the first operation, then we should replace
	   the expression here - which is the case.  */
	{
	  clear_expr (expr);
	  copy_expr (expr, INSN_EXPR (insn));
	}
      else
	av_set_add (&av1, INSN_EXPR (insn));
    }

  /* If insn is a bb_header, leave a copy of av1 here.  */
  if (sel_bb_head_p (insn))
    {
      basic_block bb = BLOCK_FOR_INSN (insn);

      /* Clear stale bb_av_set.  */
      av_set_clear (&BB_AV_SET (bb));

      print ("Save av(%d) in bb header", INSN_UID (insn));

      BB_AV_SET (bb) = unique_p ? av_set_copy (av1) : av1;
      BB_AV_LEVEL (bb) = global_level;
    }

  line_start ();
  print ("insn: ");
  dump_insn_1 (insn, 1);
  line_finish ();

  line_start ();
  print ("av (%d): ", INSN_UID (insn));
  dump_av_set (av1);
  line_finish ();
  
  block_finish ();
  return av1;
}

/* Propagate a liveness set LV through INSN.  */
static void
propagate_lv_set (regset lv, insn_t insn)
{
  gcc_assert (INSN_P (insn));

  if (INSN_NOP_P (insn))
    return;

  /* LV1 = LV1 \ { DEST (insn) }  */
  if (GET_CODE (PATTERN (insn)) != COND_EXEC) 
    /* May-defs should not kill other sets.  */
    AND_COMPL_REG_SET (lv, INSN_REG_SETS (insn));

  /* LV1 = LV1 U { SOURCES (insn) } */
  /* FIXME: Should we consider the whole register clobbered in the cases of
     STRICT_LOW_PART or SUBREG?  */
  /* Yes, and that's what sched-deps do: if there's subreg or 
     strict_low_part, then the whole register (as it appear in that expr)
     will be included in REG_PUSES counting all it's relevant subregs
     in the used mode.  */
  IOR_REG_SET (lv, INSN_REG_USES (insn));
}

static regset
compute_live_after_bb (basic_block bb)
{
  insn_t succ;
  succ_iterator si;
  regset lv = get_clear_regset_from_pool ();

  gcc_assert (!ignore_first);

  FOR_EACH_SUCC_1 (succ, si, BB_END (bb), SUCCS_ALL)
    IOR_REG_SET (lv, compute_live (succ));

  return lv;
}

/* Compute the set of all live registers at the point before INSN and save
   it at INSN if INSN is bb header.  */
static regset
compute_live (insn_t insn)
{
  if (sel_bb_head_p (insn) && !ignore_first)
    {
      basic_block bb = BLOCK_FOR_INSN (insn);

      if (BB_LV_SET_VALID_P (bb))
	{
	  regset lv = get_regset_from_pool ();

	  COPY_REG_SET (lv, BB_LV_SET (bb));
	  return_regset_to_pool (lv);
	  return lv;
	}
    }

  /* We've skipped the wrong lv_set.  Don't skip the right one.  */
  ignore_first = false;
  
  {
    basic_block bb;
    insn_t bb_end;
    regset lv;

    bb = BLOCK_FOR_INSN (insn);
    gcc_assert (in_current_region_p (bb));

    bb_end = BB_END (bb);
    lv = compute_live_after_bb (bb);

    while (bb_end != insn)
      {
	propagate_lv_set (lv, bb_end);

	bb_end = PREV_INSN (bb_end);
      }
    /* Now LV contains live set below INSN.  */
      
    /* Compute live set above INSN.  */
    propagate_lv_set (lv, insn);

    if (sel_bb_head_p (insn))
      {
	basic_block bb = BLOCK_FOR_INSN (insn);

	COPY_REG_SET (BB_LV_SET (bb), lv);
	BB_LV_SET_VALID_P (bb) = true;
      }

    /* We return LV to the pool, but will not clear it there.  Thus we can
       legimatelly use LV till the next use of regset_pool_get ().  */
    return_regset_to_pool (lv);

    return lv;
  }
}

/* Compute the set of live registers right below the INSN and save it to 
   REGS.  */
static void
compute_live_below_insn (insn_t insn, regset regs)
{
  insn_t succ;
  succ_iterator succ_i;

  FOR_EACH_SUCC_1 (succ, succ_i, insn, SUCCS_ALL)
    IOR_REG_SET (regs, compute_live (succ));
}

/* Update the data gathered in av and lv sets starting from INSN.  */
static void
update_data_sets (rtx insn)
{
  gcc_assert (sel_bb_head_p (insn) && AV_LEVEL (insn) != 0);

  line_start ();
  print ("update_data_sets");
  dump_insn (insn);
  line_finish ();

  /* Recompute the INSN's LV_SET as it may have got invalid.  */
  ignore_first = true;
  compute_live (insn);

  /* Recompute the INSN's AV_SET as it may have got invalid.  */
  BB_AV_LEVEL (BLOCK_FOR_INSN (insn)) = -1;
  compute_av_set (insn, NULL, 0, 0);
}


/* Helper for move_op () and find_used_regs ().
   Return speculation type for which a check should be created on the place
   of INSN.  EXPR is one of the original ops we are searching for.  */
static ds_t
get_spec_check_type_for_insn (insn_t insn, expr_t expr)
{
  ds_t to_check_ds;
  ds_t already_checked_ds = EXPR_SPEC_DONE_DS (INSN_EXPR (insn));

  to_check_ds = EXPR_SPEC_TO_CHECK_DS (expr);

  if (targetm.sched.get_insn_checked_ds)
    already_checked_ds |= targetm.sched.get_insn_checked_ds (insn);

  if (spec_info != NULL
      && (spec_info->flags & SEL_SCHED_SPEC_DONT_CHECK_CONTROL))
    already_checked_ds |= BEGIN_CONTROL;

  already_checked_ds = ds_get_speculation_types (already_checked_ds);

  to_check_ds &= ~already_checked_ds;

  return to_check_ds;
}

/* Functions to check liveness restrictions on available instructions.  */

/* Helper function for find_used_regs.
   Finds registers that are not available for storing rhses while moving
   ORIG_OPS up on the path starting from INSN. The register is used on 
   the moving path, if one of the following conditions is false 

      (1) not set or read on any path from xi to an instance of the original
          operation, 
      (2) not among the live registers of the point immediately following the 
          first original operation on a given downward path, except for the
	  original target register of the operation,
      (3) not live on the other path of any conditional branch that is passed 
	  by the operation, in case original operations are not present on
	  both paths of the conditional branch.
   
   The routine traverses the program in the way similar to move_op.
   PATH represents the edges traversed so far.  The return value is only used 
   for correct work of this function itself.  The used registers are added to
   USED_REGS set (USED_REGS should be allocated elsewhere, registers found are
   ORed into the set).

   All the original operations found during the traversal are saved in the
   ORIGINAL_INSNS list.

   UNAVAILABLE_HARD_REGS denotes the set of hardware registers that
   can not be used with renaming due to the register class restrictions,
   mode restrictions and other (the register we'll choose should be 
   compatible class with the original uses, shouldn't be in call_used_regs,
   should be HARD_REGNO_RENAME_OK etc).
   This parameter can have NULL value.  It should happen if we compute
   used registers before reload, and thus don't have information about hard
   registers.  

   CROSSES_CALL is true, if there is a call insn on the path from INSN to 
   original insn. In this case CALL_USED_REG_SET will be added to the 
   UNAVAILABLE_HARD_REGS at the point original operation is found.   

   Actually we need a complement set to the one computed by this routine,
   but it's more natural to have the inverted set.  */

static int
find_used_regs_1 (insn_t insn, av_set_t orig_ops, ilist_t path, 
		  regset used_regs, HARD_REG_SET *unavailable_hard_regs,
		  bool crosses_call, def_list_t *original_insns)
{
  rhs_t rhs;
  bool is_orig_op = false;
  regset tmp;
  int res;
  
  line_start ();
  print ("find_used_regs_1(");
  dump_insn (insn);
  print (",");
  dump_av_set (orig_ops);
  print (")");
  line_finish ();
  block_start ();

  gcc_assert (orig_ops);

  /* Check that we are still inside a dag we schedule.  */
  if (is_ineligible_successor (insn, path))
    {
      print ("ineligible_successor");
      block_finish ();
      return 0;
    }

  orig_ops = av_set_copy (orig_ops);

  /* If we've found valid av set, then filter the orig_ops set.  */
  if (AV_SET_VALID_P (insn))
    {
      line_start ();
      print ("av");
      dump_insn (insn);
      print ("=");
      dump_av_set (AV_SET (insn));
      line_finish ();

      av_set_intersect (&orig_ops, AV_SET (insn));

      /* If no more original ops, return immediately.  */
      if (!orig_ops)
	{
	  print ("no intersection");
	  block_finish ();
	  return 0;
	}

      /* !!! When using different types of speculation we must not leave
	 just one element in orig_ops.  */
      /*av_set_leave_one (&orig_ops);*/
    }

  if (CALL_P (insn))
    crosses_call = true;

  /* Look at the insn and decide if it could be an ancestor of currently 
     scheduling operation.  If it is so, then the insn "dest = op" could 
     either be replaced with "dest = reg", because REG now holds the result
     of OP, or just removed, if we've scheduled the insn as a whole.  

     If this insn doesn't contain currently scheduling OP, then proceed
     with searching and look at its successors.  Operations we're searching 
     for could have changed when moving up through this insn via 
     substituting.  In this case, firstly perform unsubstitution on them. 

     When traversing the DAG below this insn is finished, insert bookkeeping 
     code, if the insn is a joint point, and remove leftovers.  */

  rhs = av_set_lookup (orig_ops, INSN_VINSN (insn));
  if (rhs)
    {
      /* We have found the original operation. Mark the registers that do not
	 meet the following condition:
	(2) not among the live registers of the point immediately following 
	    the first original operation on a given downward path, except 
	    for the original target register of the operation.  */

      print ("found original operation!");

      {
	bool needs_spec_check_p;

	needs_spec_check_p = (get_spec_check_type_for_insn (insn, rhs) != 0);

	def_list_add (original_insns, insn, crosses_call, needs_spec_check_p);
      }

      res = 1;
      is_orig_op = true;

      tmp = get_clear_regset_from_pool ();

      compute_live_below_insn (insn, tmp);

      AND_COMPL_REG_SET (tmp, INSN_REG_SETS (insn));
      IOR_REG_SET (used_regs, tmp);

      return_regset_to_pool (tmp);

      /* (*1) We need to add to USED_REGS registers that are read by 
	 INSN's lhs. This may lead to choosing wrong src register. 
	 E.g. (scheduling const rhs enabled):  

	    429: ax=0x0	<- Can't use AX for this rhs (0x0)
	    433: dx=[bp-0x18]
	    427: [ax+dx+0x1]=ax 
	      REG_DEAD: ax
	    168: di=dx
	      REG_DEAD: dx    
	 */
      /* FIXME: see comment above and enable MEM_P in vinsn_separable_p.  */
      gcc_assert (!VINSN_SEPARABLE_P (INSN_VINSN (insn))
                  || !MEM_P (INSN_LHS (insn)));
    }
  else
    {
      succ_iterator succ_i;
      rtx succ;
      bool mutexed;
      rhs_t r;
      av_set_iterator avi;
      regset live_on_other_branches = NULL;

      res = 0;

      /* Av set ops could have changed when moving through this insn.
	 To find them below it, we have to un-speculate and un-substitute
	 them.  */

      undo_transformations (&orig_ops, insn);

      /* If all original operands have been filtered on this branch,
	 return.  */
      if (!orig_ops)
	{
	  block_finish ();
	  return 0;
	}

      /* Continue searching.  Do recursion here.  */
      ilist_add (&path, insn);

      FOR_EACH_SUCC (succ, succ_i, insn)
        {
	  int b;

	  if (succ_i.e1)
	    {
	      gcc_assert (succ_i.e2);

	      gcc_assert (succ_i.e1->src == BLOCK_FOR_INSN (insn)
			  && succ_i.e2->dest == BLOCK_FOR_INSN (succ));
	    }

	  b = find_used_regs_1 (succ, orig_ops, path,
			      used_regs, unavailable_hard_regs, crosses_call,
			      original_insns);

	  if (b == 0)
	    /* Mark all registers that do not meet the following condition:
	       (3) not live on the other path of any conditional branch 
	       that is passed by the operation, in case original
	       operations are not present on both paths of the 
	       conditional branch.

	       if b is false, then it's not present on that branch.  */
	    {
	      regset succ_live = compute_live (succ);

	      if (live_on_other_branches == NULL)
		/* We're getting succ_live out of the pool.  */
		{
		  regset tmp = get_regset_from_pool ();

		  gcc_assert (tmp == succ_live);

		  live_on_other_branches = succ_live;
		}
	      else
		/* We're leaving succ_live in the pool.  */
		IOR_REG_SET (live_on_other_branches, succ_live);
	    }
	  else if (res != -1)
	    res = b;
	}

      if (live_on_other_branches != NULL)
	{
	  IOR_REG_SET (used_regs, live_on_other_branches);
	  return_regset_to_pool (live_on_other_branches);
	  live_on_other_branches = NULL;
	}

      if (res == 0)
	res = -1;

      if (res == 1)
	{
	  /* Additionally, we need to include all live registers on 
	     the successors of all conditional branches that do not lead to 
	     the original operation.  We need to do this only for conditional
	     branches that are inside actual code motion paths for original 
	     operations.  The actual code motion path will have non-empty
	     orig_ops set.
	     This loop completes the set that is evaluated for 
	     condition (3).  */
    
	  FOR_EACH_SUCC_1 (succ, succ_i, insn, SUCCS_BACK | SUCCS_OUT)
	    IOR_REG_SET (used_regs, compute_live (succ));
    
	  /* If current insn we are looking at cannot be executed together 
	     with original insn, then we can skip it safely.
    
	     Example: ORIG_OPS = { (p6) r14 = sign_extend (r15); }
		      INSN = (!p6) r14 = r14 + 1; 
    
	     Here we can schedule ORIG_OP with lhs = r14, though only 
	     looking at the set of used and set registers of INSN we must 
	     forbid it.  So, add set/used in INSN registers to the 
	     untouchable set only if there is an insn in ORIG_OPS that can 
	     affect INSN.  
    
	     FIXME: I believe that it is enough to check only 1 insn of 
	     ORIG_OPS (because other insns must be variations of this insn, 
	     created by substitution, and so their execution conditions 
	     must be same on all insns in ORIG_OPS), 
	     though here we are more conservative.  */
	  mutexed = true;
	  gcc_assert (orig_ops);
	  FOR_EACH_RHS (r, avi, orig_ops)
	    if (!sched_insns_conditions_mutex_p (insn, RHS_INSN (r)))
	      {
		mutexed = false;
		break;
	      }
    
	  /* Mark all registers that do not meet the following condition:
	     (1) Not set or read on any path from xi to an instance of the 
		 original operation.  */
	  if (!mutexed)
	    {
	      IOR_REG_SET (used_regs, INSN_REG_SETS (insn));
	      IOR_REG_SET (used_regs, INSN_REG_USES (insn));
	    }	  
	}

      ilist_remove (&path);
    }

  av_set_clear (&orig_ops);

  gcc_assert (!sel_bb_head_p (insn) || AV_SET_VALID_P (insn)
	      || AV_LEVEL (insn) == -1);

  if (res == -1 && AV_LEVEL (insn) == -1)
    /* INSN has an incorrect av_set on which we've couldn't have filtered
       ORIG_OPS.  */
    res = 0;

  block_finish ();
  return res;
}

/* Find the set of registers that are unavailable for storing rhses 
   while moving ORIG_OPS up on the path starting from INSN due to
   liveness (USED_REGS) or hardware restrictions (UNAVAILABLE_HARD_REGS).

   All the original operations found during the traversal are saved in the
   ORIGINAL_INSNS list.

   Returns TRUE if we've found all original insns, FALSE otherwise.

   See also comment for find_used_regs_1 for the details.  */

static bool
find_used_regs (insn_t insn, av_set_t orig_ops, regset used_regs,
		HARD_REG_SET *unavailable_hard_regs, 
		def_list_t *original_insns)
{
  def_list_iterator i;
  def_t def;
  int res;

  res = find_used_regs_1 (insn, orig_ops, NULL, used_regs,
			  unavailable_hard_regs, false, original_insns);

  if (res == -1)
    return false;

  gcc_assert (res == 1);
  gcc_assert (original_insns && *original_insns);

  /* Mark hardware regs in UNAVAILABLE_HARD_REGS that are not suitable 
     for renaming rhs in INSN due to hardware restrictions (register class,
     modes compatibility etc).  This doesn't affect original insn's dest regs.
     Registers that are in used_regs are always marked also in
     unavailable_hard_regs.  */
  FOR_EACH_DEF (def, i, *original_insns)
    {
      vinsn_t vinsn = INSN_VINSN (def->orig_insn);

      if (VINSN_SEPARABLE_P (vinsn))
	mark_unavailable_hard_regs (def, unavailable_hard_regs, used_regs);

      if (def->needs_spec_check_p)
	/* Do not allow clobbering of ld.[sa] address.  */
	IOR_REG_SET (used_regs, VINSN_REG_USES (vinsn));
    }

  return true;
}


/* Functions to choose the best insn from available ones.  */

/* Rank two available rhses for schedule.  Never return 0 here.  */
static int 
sel_rank_for_schedule (const void *x, const void *y)
{
  rhs_t tmp = *(const rhs_t *) y;
  rhs_t tmp2 = *(const rhs_t *) x;
  insn_t tmp_insn, tmp2_insn;
  vinsn_t tmp_vinsn, tmp2_vinsn;
  int val;

  tmp_vinsn = RHS_VINSN (tmp);
  tmp2_vinsn = RHS_VINSN (tmp2);
  tmp_insn = RHS_INSN (tmp);
  tmp2_insn = RHS_INSN (tmp2);
  
  /* Prefer SCHED_GROUP_P insns to any others.  */
  if (SCHED_GROUP_P (tmp_insn) != SCHED_GROUP_P (tmp2_insn))
    {
      if (VINSN_UNIQUE_P (tmp_vinsn) && VINSN_UNIQUE_P (tmp2_vinsn)) 
        return SCHED_GROUP_P (tmp2_insn) ? 1 : -1;
      /* Now uniqueness means SCHED_GROUP_P is set, because schedule groups
         cannot be cloned.  */
      if (VINSN_UNIQUE_P (tmp2_vinsn))
        return 1;
      return -1;
    }

  /* Discourage scheduling of speculative checks.  */
  val = (sel_insn_is_speculation_check (tmp_insn)
	 - sel_insn_is_speculation_check (tmp2_insn));
  if (val)
    return val;

  /* Prefer not scheduled insn over scheduled one.  */
  val = EXPR_SCHED_TIMES (tmp) - EXPR_SCHED_TIMES (tmp2);
  if (val)
    return val;

  /* FIXME: Want to try first clear all insns except with minimal SPEC.  */
  /* Prefer a rhs with smaller SPEC attribute.  
     FIXME: should we use probabilities here?  */
  val = EXPR_SPEC (tmp) - EXPR_SPEC (tmp2);
  if (val)
    return val;

  /* Prefer an expr with greater priority.  */
  val = EXPR_PRIORITY (tmp2) - EXPR_PRIORITY (tmp);
  if (val)
    return val;

  if (spec_info != NULL && spec_info->mask != 0)
    /* This code was taken from haifa-sched.c: rank_for_schedule ().  */
    {
      ds_t ds1, ds2;
      dw_t dw1, dw2;
      int dw;

      ds1 = EXPR_SPEC_DONE_DS (tmp);
      if (ds1)
	dw1 = ds_weak (ds1);
      else
	dw1 = NO_DEP_WEAK;

      ds2 = EXPR_SPEC_DONE_DS (tmp2);
      if (ds2)
	dw2 = ds_weak (ds2);
      else
	dw2 = NO_DEP_WEAK;

      dw = dw2 - dw1;
      if (dw > (NO_DEP_WEAK / 8) || dw < -(NO_DEP_WEAK / 8))
	return dw;
    }

  tmp_insn = EXPR_INSN_RTX (tmp);
  tmp2_insn = EXPR_INSN_RTX (tmp2);

  /* Prefer an old insn to a bookkeeping insn.  */
  if (INSN_UID (tmp_insn) < first_emitted_uid 
      && INSN_UID (tmp2_insn) >= first_emitted_uid)
    return -1;
  if (INSN_UID (tmp_insn) >= first_emitted_uid 
      && INSN_UID (tmp2_insn) < first_emitted_uid)
    return 1;

  /* Prefer an insn with smaller UID, as a last resort.  
     We can't safely use INSN_LUID as it is defined only for those insns
     that are in the stream.  */
  return INSN_UID (tmp_insn) - INSN_UID (tmp2_insn);
}

/* Filter out expressions that are pipelined too much.  */
static void
process_pipelined_exprs (av_set_t *av_ptr)
{
  rhs_t rhs;
  av_set_iterator si;

  /* Don't pipeline already pipelined code as that would increase
     number of unnecessary register moves.  */  
  FOR_EACH_RHS_1 (rhs, si, av_ptr)
    {
      if (EXPR_SCHED_TIMES (rhs)
	  >= PARAM_VALUE (PARAM_SELSCHED_MAX_SCHED_TIMES))
	av_set_iter_remove (&si);
    }
}

/* Filter speculative insns from AV_PTR if we don't want them.  */
static void
process_spec_exprs (av_set_t *av_ptr)
{
  bool try_data_p = true;
  bool try_control_p = true;
  rhs_t rhs;
  av_set_iterator si;

  if (spec_info == NULL)
    return;

  /* Scan *AV_PTR to find out if we want to consider speculative
     instructions for scheduling.  */
  FOR_EACH_RHS_1 (rhs, si, av_ptr)
    {
      ds_t ds;

      ds = EXPR_SPEC_DONE_DS (rhs);

      /* The probability of a success is too low - don't speculate.  */
      if ((ds & SPECULATIVE)
          && ds_weak (ds) < spec_info->weakness_cutoff)
        {
          av_set_iter_remove (&si);
          continue;
        }

      if ((spec_info->flags & PREFER_NON_DATA_SPEC)
          && !(ds & BEGIN_DATA))
        try_data_p = false;

      if ((spec_info->flags & PREFER_NON_CONTROL_SPEC)
          && !(ds & BEGIN_CONTROL))
        try_control_p = false;
    }

  FOR_EACH_RHS_1 (rhs, si, av_ptr)
    {
      ds_t ds;

      ds = EXPR_SPEC_DONE_DS (rhs);

      if (ds & SPECULATIVE)
        {
          if ((ds & BEGIN_DATA) && !try_data_p)
            /* We don't want any data speculative instructions right
               now.  */
            av_set_iter_remove (&si);

          if ((ds & BEGIN_CONTROL) && !try_control_p)
            /* We don't want any control speculative instructions right
               now.  */
            av_set_iter_remove (&si);
        }
    }
}

/* Search for any use-like insns in AV_PTR and decide on scheduling 
   them.  Return one when found, and NULL otherwise.  
   Note that we check here whether a USE could be scheduled to avoid
   an infinite loop later.  */
static rhs_t
process_use_exprs (av_set_t *av_ptr, blist_t bnds)
{
  rhs_t rhs;
  av_set_iterator si;
  bool uses_present_p = false;
  bool try_uses_p = true;

  FOR_EACH_RHS_1 (rhs, si, av_ptr)
    {
      /* This will also initialize INSN_CODE for later use.  */
      if (recog_memoized (EXPR_INSN_RTX (rhs)) < 0)
        {
          /* If we have a USE in *AV_PTR that was not scheduled yet,
             do so because it will do good only.  */
          if (EXPR_SCHED_TIMES (rhs) <= 0)
            {
              if (find_best_reg_for_rhs (rhs, bnds))
                return rhs;

              av_set_iter_remove (&si);
            }
          else
            {
              gcc_assert (pipelining_p);

              uses_present_p = true;
            }
        }
      else
        try_uses_p = false;
    }

  if (uses_present_p)
    {
      /* If we don't want to schedule any USEs right now and we have some
           in *AV_PTR, remove them, else just return the first one found.  */
      if (!try_uses_p)
        {
          FOR_EACH_RHS_1 (rhs, si, av_ptr)
            if (INSN_CODE (EXPR_INSN_RTX (rhs)) < 0)
              av_set_iter_remove (&si);
        }
      else
        {
          FOR_EACH_RHS_1 (rhs, si, av_ptr)
            {
              gcc_assert (INSN_CODE (EXPR_INSN_RTX (rhs)) < 0);

              if (find_best_reg_for_rhs (rhs, bnds))
                return rhs;

              av_set_iter_remove (&si);
            }
        }
    }

  return NULL;
}

/* Turn AV into a vector, filter inappropriate insns and sort it.  Return 
   true if there is something to schedule.  */
static bool
fill_vec_av_set (av_set_t av, blist_t bnds, fence_t fence)
{
  av_set_iterator si;
  rhs_t rhs;
  int sched_next_worked = 0, stalled, n;
  deps_t dc = BND_DC (BLIST_BND (bnds));

  /* Bail out early when the ready list contained only USEs/CLOBBERs that are
     already scheduled.  */
  if (av == NULL)
    return false;

  /* Empty vector from the previous stuff.  */
  if (VEC_length (rhs_t, vec_av_set) > 0)
    VEC_block_remove (rhs_t, vec_av_set, 0, VEC_length (rhs_t, vec_av_set));

  /* Turn the set into a vector for sorting.  */
  gcc_assert (VEC_empty (rhs_t, vec_av_set));
  FOR_EACH_RHS (rhs, si, av)
    VEC_safe_push (rhs_t, heap, vec_av_set, rhs);

  /* Filter out inappropriate expressions.  */
  for (n = 0, stalled = 0; VEC_iterate (rhs_t, vec_av_set, n, rhs); )
    {
      insn_t insn = EXPR_INSN_RTX (rhs);

      /* Don't allow any insns other than from SCHED_GROUP if we have one.  */
      if (FENCE_SCHED_NEXT (fence) && insn != FENCE_SCHED_NEXT (fence))
        {
          VEC_unordered_remove (rhs_t, vec_av_set, n);
          continue;
        }

      /* Set number of sched_next insns (just in case there 
         could be several).  */
      if (FENCE_SCHED_NEXT (fence))
        sched_next_worked++;

      /* Check all liveness requirements and try renaming.  
         FIXME: try to minimize calls to this.  */
      if (! find_best_reg_for_rhs (rhs, bnds))
        {
          VEC_unordered_remove (rhs_t, vec_av_set, n);
          continue;
        }

      /* Don't allow any insns whose data is not yet ready.  */
      if (! tick_check_p (rhs, dc, fence))
	{
          /* We need to know whether we do need to stall for any insns.  */
          stalled++;
          VEC_unordered_remove (rhs_t, vec_av_set, n);
	  continue;
	}

      n++;
    }

  if (VEC_empty (rhs_t, vec_av_set))
    {
      /* It seems possible that when no insns are ready, this could be
         due to liveness restrictions.  However, we should always be able
         to schedule the next instruction.  So when the list is empty, then
         all insns must require a stall to be scheduled.  */
      if (!enable_moveup_set_path_p)
        gcc_assert (stalled);

      /* We need to set need_stall to 1 here, because later we skip this code
         when ready list is empty.  */
      need_stall = stalled;
      
      return false;
    }

  /* Sort the vector.  */
  qsort (VEC_address (rhs_t, vec_av_set), VEC_length (rhs_t, vec_av_set),
         sizeof (rhs_t), sel_rank_for_schedule);

  line_start ();
  print ("Sorted av set (%d): ", VEC_length (rhs_t, vec_av_set));

  for (n = 0; VEC_iterate (rhs_t, vec_av_set, n, rhs); n++)
    dump_rhs (rhs);
  line_finish ();

  print ("\nreally_ready: %d stalled: %d \n", 
         VEC_length (rhs_t, vec_av_set), stalled);

  /* Clear SCHED_NEXT.  */
  if (FENCE_SCHED_NEXT (fence))
    {
      gcc_assert (sched_next_worked == 1);
      FENCE_SCHED_NEXT (fence) = NULL_RTX;
    }

  return true;
}

/* Convert a vectored and sorted av set to the ready list that
   the rest of the backend wants to see.  */
static void
convert_vec_av_set_to_ready (void)
{
  int n;
  rhs_t rhs;

  /* Allocate and fill the ready list from the sorted vector.  */
  ready.n_ready = VEC_length (rhs_t, vec_av_set);
  ready.first = ready.n_ready - 1;
  
  gcc_assert (ready.n_ready > 0);

  if (ready.n_ready > max_issue_size)
    {
      max_issue_size = ready.n_ready;
      sched_extend_ready_list (ready.n_ready);
    }
  
  for (n = 0; VEC_iterate (rhs_t, vec_av_set, n, rhs); n++)
    {
      vinsn_t vi = EXPR_VINSN (rhs);
      insn_t insn = VINSN_INSN_RTX (vi);

      ready_try[n] = 0;
      ready.vec[n] = insn;
    }
}

/* Initialize ready list from the AV for the max_issue () call.
   If any unrecognizable insn found in the AV, return it (and skip
   max_issue).  BND and FENCE are current boundary and fence, 
   respectively.  */
static rhs_t
fill_ready_list (av_set_t *av_ptr, blist_t bnds, fence_t fence)
{
  rhs_t rhs;

  /* We do not support multiple boundaries per fence.  */
  gcc_assert (BLIST_NEXT (bnds) == NULL);

  /* Process expressions required special handling, i.e.  pipelined, 
     speculative and recog() < 0 expressions first.  */
  process_pipelined_exprs (av_ptr);
  process_spec_exprs (av_ptr);

  /* A USE could be scheduled immediately.  */
  rhs = process_use_exprs (av_ptr, bnds);
  if (rhs)
    return rhs;

  /* Turn the av set to a vector for sorting.  */
  if (! fill_vec_av_set (*av_ptr, bnds, fence))
    {
      ready.n_ready = 0;
      return NULL;
    }
  
  /* Build the final ready list.  */
  convert_vec_av_set_to_ready ();

  return NULL;
}

/* Wrapper for dfa_new_cycle ().  Returns TRUE if cycle was advanced.  */
static bool
sel_dfa_new_cycle (insn_t insn, fence_t fence)
{
  int last_scheduled_cycle = FENCE_LAST_SCHEDULED_INSN (fence) 
                             ? INSN_SCHED_CYCLE (FENCE_LAST_SCHEDULED_INSN (fence)) 
                             : FENCE_CYCLE (fence) - 1;
  bool res = false;
  int sort_p = 0;

  if (!targetm.sched.dfa_new_cycle)
    return false;

  memcpy (curr_state, FENCE_STATE (fence), dfa_state_size);

  while (!sort_p && targetm.sched.dfa_new_cycle (sched_dump, sched_verbose,
                                                 insn, last_scheduled_cycle,
                                                 FENCE_CYCLE (fence), &sort_p))
    {
      memcpy (FENCE_STATE (fence), curr_state, dfa_state_size);
      advance_one_cycle (fence);
      memcpy (curr_state, FENCE_STATE (fence), dfa_state_size);
      res = true;
    }

  return res;
}

/* Invoke reorder* target hooks on the ready list.  Return the number of insns
   we can issue.  */
static int
invoke_reorder_hooks (fence_t fence)
{
  int issue_more;
  bool ran_hook = false;

  /* Call the reorder hook at the beginning of the cycle, and call
     the reorder2 hook in the middle of the cycle.  */
  if (FENCE_ISSUED_INSNS (fence) == 0)
    {
      if (targetm.sched.reorder
          && !SCHED_GROUP_P (ready_element (&ready, 0))
          && ready.n_ready > 1)
        {
          /* Don't give reorder the most prioritized insn as it can break
             pipelining.  */
          if (pipelining_p)
            --ready.n_ready;

          issue_more
            = targetm.sched.reorder (sched_dump, sched_verbose,
                                     ready_lastpos (&ready),
                                     &ready.n_ready, FENCE_CYCLE (fence));

          if (pipelining_p)
            ++ready.n_ready;

          ran_hook = true;
        }
      else
        /* Initialize can_issue_more for variable_issue.  */
        issue_more = issue_rate;
    }
  else if (targetm.sched.reorder2
           && !SCHED_GROUP_P (ready_element (&ready, 0)))
    {
      if (ready.n_ready == 1)
        issue_more = 
          targetm.sched.reorder2 (sched_dump, sched_verbose,
                                  ready_lastpos (&ready),
                                  &ready.n_ready, FENCE_CYCLE (fence));
      else
        {
          if (pipelining_p)
            --ready.n_ready;

          issue_more =
            targetm.sched.reorder2 (sched_dump, sched_verbose,
                                    ready.n_ready
                                    ? ready_lastpos (&ready) : NULL,
                                    &ready.n_ready, FENCE_CYCLE (fence));

          if (pipelining_p)
            ++ready.n_ready;
        }

      ran_hook = true;
    }
  else 
    issue_more = issue_rate;

  /* Ensure that ready list and vec_av_set are in line with each other,
     i.e. vec_av_set[i] == ready_element (&ready, i).  */
  if (issue_more && ran_hook)
    {
      int i, j, n;
      rtx *arr = ready.vec;
      rhs_t *vec = VEC_address (rhs_t, vec_av_set);

      for (i = 0, n = ready.n_ready; i < n; i++)
        if (EXPR_INSN_RTX (vec[i]) != arr[i])
          {
            rhs_t tmp;

            for (j = i; j < n; j++)
              if (EXPR_INSN_RTX (vec[j]) == arr[i])
                break;
            gcc_assert (j < n);

            tmp = vec[i]; 
            vec[i] = vec[j];
            vec[j] = tmp;
          }
    }

  return issue_more;
}

/* Return an RHS correponding to INDEX element of ready list, if 
   FOLLOW_READY_ELEMENT is true (i.e., an rhs of 
   ready_element (&ready, INDEX) will be returned), and to INDEX element of 
   ready.vec otherwise.  */
static inline rhs_t
find_expr_for_ready (int index, bool follow_ready_element)
{
  rhs_t rhs;
  int real_index;

  real_index = follow_ready_element ? ready.first - index : index;

  rhs = VEC_index (rhs_t, vec_av_set, real_index);
  gcc_assert (ready.vec[real_index] == EXPR_INSN_RTX (rhs));

  return rhs;
}

/* Calculate insns worth trying via lookahead_guard hook.  Return a number
   of such insns found.  */
static int
invoke_dfa_lookahead_guard (void)
{
  int i, n;
  bool have_hook 
    = targetm.sched.first_cycle_multipass_dfa_lookahead_guard != NULL;

  line_start ();
  print ("ready after reorder: ");

  for (i = 0, n = 0; i < ready.n_ready; i++)
    {
      rhs_t rhs;
      insn_t insn;
      int r;

      /* In this loop insn is Ith element of the ready list given by 
         ready_element, not Ith element of ready.vec.  */
      insn = ready_element (&ready, i);
      
      if (! have_hook || i == 0)
        r = 0;
      else
        r = !targetm.sched.first_cycle_multipass_dfa_lookahead_guard (insn);
      
      gcc_assert (INSN_CODE (insn) >= 0);
        
      /* Only insns with ready_try = 0 can get here 
         from fill_ready_list.  */
      gcc_assert (ready_try [i] == 0);
      ready_try[i] = r;
      if (!r)
        n++;

      rhs = find_expr_for_ready (i, true);
      dump_vinsn (EXPR_VINSN (rhs));

      print (":%d; ", ready_try[i]);
    }
  
  line_finish ();

  return n;
}

/* Calculate the number of privileged insns and return it.  */
static int
calculate_privileged_insns (void)
{
  rhs_t cur_rhs, min_spec_rhs = NULL;
  insn_t cur_insn, min_spec_insn;
  int privileged_n = 0, i;

  for (i = 0; i < ready.n_ready; i++)
    {
      if (ready_try[i])
        continue;

      if (! min_spec_rhs)
        {
          min_spec_insn = ready_element (&ready, i);
          min_spec_rhs = find_expr_for_ready (i, true);
        }
      
      cur_insn = ready_element (&ready, i);
      cur_rhs = find_expr_for_ready (i, true);

      if (RHS_SPEC (cur_rhs) > RHS_SPEC (min_spec_rhs))
        break;

      ++privileged_n;
    }

  if (i == ready.n_ready)
    privileged_n = 0;

  print ("privileged_n: %d insns with SPEC %d; ",
         privileged_n, privileged_n ? RHS_SPEC (min_spec_rhs) : -1);

  return privileged_n;
}

/* Call the rest of the hooks after the choice was made.  Return 
   the number of insns that still can be issued.  */
static int
invoke_aftermath_hooks (fence_t fence, rtx best_insn, int issue_more)
{
  gcc_assert (INSN_P (best_insn));

  /* First, call dfa_new_cycle, and then variable_issue, if available.  */
  sel_dfa_new_cycle (best_insn, fence);
  
  if (targetm.sched.variable_issue)
    {
      memcpy (curr_state, FENCE_STATE (fence), dfa_state_size);
      issue_more = 
        targetm.sched.variable_issue (sched_dump, sched_verbose, best_insn,
                                      issue_more);
      memcpy (FENCE_STATE (fence), curr_state, dfa_state_size);
    }
  else if (GET_CODE (PATTERN (best_insn)) != USE
           && GET_CODE (PATTERN (best_insn)) != CLOBBER)
    issue_more--;

  return issue_more;
}

/* Find the best insn for scheduling, either via max_issue or just take 
   the most prioritized available.  */
static int
choose_best_insn (fence_t fence, int privileged_n, int *index)
{
  int can_issue = 0;

  if (dfa_lookahead > 0)
    {
      cycle_issued_insns = FENCE_ISSUED_INSNS (fence);
      can_issue = max_issue (&ready, privileged_n,
                             FENCE_STATE (fence), index);

      print ("max_issue: we can issue %d insns, already did %d insns",
             can_issue, FENCE_ISSUED_INSNS (fence));
    }
  else
    {
      /* We can't use max_issue; just return the first element.  */
      rhs_t rhs = find_expr_for_ready (0, true);

      if (vinsn_dfa_cost (EXPR_VINSN (rhs), fence) >= 1)
        {
          can_issue = 0;
          *index = -1;
        }
      else
        {
          can_issue = 1;
          *index = 0;
          print ("using first insn from the ready list");
        }
    }

  return can_issue;
}

/* Choose the best expr from *AV_VLIW_PTR and a suitable register for it.  
   BNDS and FENCE are current boundaries and scheduling fence respectively.  
   Return the expr found and NULL if nothing can be issued atm.  */ 
static expr_t
find_best_expr (av_set_t *av_vliw_ptr, blist_t bnds, fence_t fence)
{
  expr_t best;
  
  /* Choose the best insn for scheduling via:
     1) sorting the ready list based on priority;
     2) calling the reorder hook;
     3) calling max_issue.  */
  best = fill_ready_list (av_vliw_ptr, bnds, fence);

  if (best == NULL && ready.n_ready > 0)
    {
      int can_issue, privileged_n, index, avail_n;

      can_issue_more = invoke_reorder_hooks (fence);
      if (can_issue_more == 0)
        return NULL;

      /* Try choosing the best insn until we find one that is could be 
         scheduled due to liveness restrictions on its destination register.
         In the future, we'd like to choose once and then just probe insns
         in the order of their priority.  */
      avail_n = invoke_dfa_lookahead_guard ();
      privileged_n = calculate_privileged_insns ();
      can_issue = choose_best_insn (fence, privileged_n, &index);
      if (can_issue)
        best = find_expr_for_ready (index, true);
      else
        need_stall = 1;
    }

  if (best != NULL)
    can_issue_more = invoke_aftermath_hooks (fence, EXPR_INSN_RTX (best), 
                                             can_issue_more);

  return best;
}


/* Emit an instruction from EXPR with SEQNO after PLACE_TO_INSERT.  */
static insn_t
gen_insn_from_expr_after (expr_t expr, int seqno, insn_t place_to_insert)
{
  /* This assert fails when we have identical instructions
     one of which dominates the other.  In this case move_op ()
     finds the first instruction and doesn't search for second one.
     The solution would be to compute av_set after the first found
     insn and, if insn present in that set, continue searching.
     For now we workaround this issue in move_op.  */
  gcc_assert (!INSN_IN_STREAM_P (EXPR_INSN_RTX (expr)));

  {
    rtx reg = expr_dest_reg (expr);

    if (reg != NULL_RTX)
      {
	static int reg_rename_this_tick = 0;

	if (HARD_REGISTER_P (reg))
	  {
	    unsigned regno = REGNO (reg);

	    reg_rename_tick[regno] = ++reg_rename_this_tick;

            df_set_regs_ever_live (regno, true);
	  }
      }
  }

  {
    insn_t new_insn;

    new_insn = sel_gen_insn_from_expr_after (expr, seqno, place_to_insert);

    return new_insn;
  }
}

/* Functions that implement the core of the scheduler.  */

/* Generate a bookkeeping copy of "REG = CUR_RHS" insn at JOIN_POINT on the 
   ingoing path(s) to E2->dest, other than from E1->src (there could be some 
   empty blocks between E1->src and E2->dest).  If there is only one such path 
   and bookkeeping copy can be created in the last block, that is on this path,
   bookkeeping instruction is inserted at the end of this block.  Otherwise, 
   the function splits E2->dest bb on the two and emits the bookkeeping copy in
   the upper bb, redirecting all other paths to the lower bb and returns the
   newly created bb, which is the lower bb. 
   All scheduler data is initialized for the newly created insn.  */
static void
generate_bookkeeping_insn (rhs_t c_rhs, insn_t join_point, edge e1, edge e2)
{
  basic_block src, bb = e2->dest;
  basic_block new_bb = NULL;
  insn_t src_end = NULL_RTX;
  insn_t place_to_insert = NULL_RTX;
  /* Save the original destination of E1.  */
  basic_block empty_bb = e1->dest;
  int new_seqno = INSN_SEQNO (join_point);
  basic_block other_block = NULL;

  /* Find a basic block that can hold bookkeeping.  If it can be found, do not
     create new basic block, but insert bookkeeping there.  */
  if (e1 == e2)
    {
      other_block = 
        EDGE_COUNT (e1->dest->preds) > 2
          ? NULL
          : EDGE_PRED (e1->dest, 0) == e1
            ? EDGE_PRED (e1->dest, 1)->src
            : EDGE_PRED (e1->dest, 0)->src;
    }
  else
    {
      edge iter_edge = e1;
      bool search_more = true;
      do
        {
          search_more = iter_edge != e2;
          /* There must be only one edge that enters path from e1 to e2 
             from aside to be able to create bookkeeping in existing block.  */
          if (EDGE_COUNT (iter_edge->dest->preds) == 2)
            {
              if (other_block == NULL)
                other_block = 
                  EDGE_PRED (iter_edge->dest, 0) == iter_edge
                    ? EDGE_PRED (iter_edge->dest, 1)->src
                    : EDGE_PRED (iter_edge->dest, 0)->src;
              else
                {
                  /* Found additional edge leading to path from e1 to e2 
                     from aside.  */
                  other_block = NULL;
                  break;
                }
            }
          else if (EDGE_COUNT (iter_edge->dest->preds) > 2)
            {
              /* Several edges leading to path from e1 to e2 from aside.  */
              other_block = NULL;
              break;
            }
          iter_edge = EDGE_SUCC (iter_edge->dest, 0);
        }
      while (search_more);
    }
  
  print ("generate_bookkeeping_insn(%d->%d)", e1->src->index, 
	 e2->dest->index);

  /* sched_split_block () can emit an unnecessary note if the following isn't
     true.  */
  gcc_assert (bb_note (bb) != BB_END (bb));

  /* Explore, if we can insert bookkeeping into OTHER_BLOCK in case edge
     OTHER_BLOCK -> BB is fallthrough, meaning there is no jump there.  */
  if (EDGE_COUNT (bb->preds) == 2 && other_block)
    {
      /* SRC is the block, in which we possibly can insert bookkeeping insn
         without creating new basic block.  It is the other (than E2->SRC)
         predecessor block of BB.  */
      src = other_block;

      /* Instruction, after which we would try to insert bookkeeping insn.  */
      src_end = BB_END (src);
      gcc_assert (in_current_region_p (src));

      if (INSN_P (src_end))
	{
	  if (control_flow_insn_p (src_end)
              /* It might be scheduled, thus making this illegal.  */
              || INSN_SCHED_TIMES (src_end) > 0)
	    src = NULL;
	}
      else
	gcc_assert (NOTE_INSN_BASIC_BLOCK_P (src_end));
    }
  else
    src = NULL;
    
  if (!src)
    {
      /* Check that we don't spoil the loop structure.  */
      if (flag_sel_sched_pipelining_outer_loops && current_loop_nest)
        {
          basic_block latch = current_loop_nest->latch;

          /* We do not split header.  */
          gcc_assert (bb != current_loop_nest->header);

          /* We do not redirect the only edge to the latch block.  */
          gcc_assert (e1->dest != latch
                      || !single_pred_p (latch)
                      || e1 != single_pred_edge (latch));
        }

      /* Explore, if we can insert bookkeeping into OTHER_BLOCK in case edge
         OTHER_BLOCK -> BB is not fallthrough, meaning there is jump there.  */
      if (other_block && EDGE_COUNT (other_block->succs) == 1
          && (e1->flags & EDGE_FALLTHRU))
        {
          insn_t src_begin;

          get_ebb_head_tail (other_block, other_block, &src_begin, &src_end);

          gcc_assert (control_flow_insn_p (src_end));

          if (/* Jump was scheduled.  */
              INSN_SCHED_TIMES (src_end) > 0
              /* This is a floating bb header.  */
              || (src_end == src_begin
                  && EDGE_COUNT (other_block->preds) == 1
                  && INSN_P (BB_END (EDGE_I (other_block->preds, 0)->src))
                  && INSN_SCHED_TIMES (BB_END (EDGE_I (other_block->preds, 
                                                       0)->src)) > 0))
            new_bb = NULL;
          else
            {
              new_bb = other_block;
              place_to_insert = PREV_INSN (src_end);
              new_seqno = INSN_SEQNO (src_end);
              insn_init.what = INSN_INIT_WHAT_INSN;
            }
        }
      else
        new_bb = NULL;

      if (!new_bb)
        {
          /* We need to create a new basic block for bookkeeping insn.  */
          can_add_real_insns_p = false;

          /* Split the head of the BB to insert BOOK_INSN there.  */
          new_bb = sched_split_block (bb, NULL);

          /* Move note_list from the upper bb.  */
          gcc_assert (BB_NOTE_LIST (new_bb) == NULL_RTX);
          BB_NOTE_LIST (new_bb) = BB_NOTE_LIST (bb);
          BB_NOTE_LIST (bb) = NULL_RTX;

          gcc_assert (e2->dest == bb);

          can_add_real_insns_p = true;
          insn_init.what = INSN_INIT_WHAT_INSN;

          /* Make a jump skipping bookkeeping copy.  */
          if (e1->flags & EDGE_FALLTHRU)
            sel_redirect_edge_and_branch_force (e1, new_bb);
          else
            sel_redirect_edge_and_branch (e1, new_bb);

          gcc_assert (e1->dest == new_bb);

          gcc_assert (sel_bb_empty_p (bb));

          place_to_insert = BB_END (bb);
        }
    }
  else
    place_to_insert = src_end;

  /* Remove unreachable empty blocks.  */
  while (EDGE_COUNT (empty_bb->preds) == 0)
    {
      basic_block next_bb = empty_bb->next_bb;
      sel_remove_empty_bb (empty_bb, false, true);
      empty_bb = next_bb;
    }

  {
    rtx new_insn_rtx;
    vinsn_t new_vinsn;
    expr_def _new_expr, *new_expr = &_new_expr;
    insn_t new_insn;

    new_insn_rtx = create_copy_of_insn_rtx (EXPR_INSN_RTX (c_rhs));
    new_vinsn = create_vinsn_from_insn_rtx (new_insn_rtx);
    copy_expr_onside (new_expr, c_rhs);
    change_vinsn_in_expr (new_expr, new_vinsn);

    /* In preheader, make sure that the new insn is marked as not 
       scheduled.  */
    if (sel_is_loop_preheader_p (BLOCK_FOR_INSN (place_to_insert)))
      EXPR_SCHED_TIMES (new_expr) = 0;
    else
      if (EXPR_SCHED_TIMES (new_expr)
	>= PARAM_VALUE (PARAM_SELSCHED_MAX_SCHED_TIMES))
      /* To make scheduling of this bookkeeping copy possible we decrease
	 its scheduling counter.  */
      --EXPR_SCHED_TIMES (new_expr);

    new_insn = gen_insn_from_expr_after (new_expr, new_seqno, place_to_insert);

    clear_expr (new_expr);

    gcc_assert ((src == NULL && BB_END (bb) == new_insn
		 && sel_bb_head_p (new_insn))
                || (src == NULL && control_flow_insn_p (BB_END (other_block))
                    && PREV_INSN (BB_END (other_block)) == new_insn
                    && INSN_SCHED_TIMES (BB_END (other_block)) == 0)
		|| BB_END (src) == new_insn);
  }

  stat_bookkeeping_copies++;
}

/* Remove from AV_PTR all insns that may need bookkeeping when scheduling 
   on FENCE, but we are unable to copy them.  */
static void
remove_insns_that_need_bookkeeping (fence_t fence, av_set_t *av_ptr)
{
  rhs_t rhs;
  av_set_iterator i;

  /*  An expression does not need bookkeeping if it is available on all paths 
      from current block to original block and current block dominates 
      original block.  We check availability on all paths by examining 
      EXPR_SPEC; this is not equivalent, because it may be positive even 
      if expr is available on all paths (but if expr is not available on 
      any path, EXPR_SPEC will be positive).  */

  FOR_EACH_RHS_1 (rhs, i, av_ptr)
    {
      if (!control_flow_insn_p (EXPR_INSN_RTX (rhs))
	  && (!bookkeeping_p || VINSN_UNIQUE_P (EXPR_VINSN (rhs)))
	  && (EXPR_SPEC (rhs)
	      || !EXPR_ORIG_BB_INDEX (rhs)
	      || !dominated_by_p (CDI_DOMINATORS,
				  BLOCK_FOR_INSN (FENCE_INSN (fence)),
				  BASIC_BLOCK (EXPR_ORIG_BB_INDEX (rhs)))))
	{
	  line_start ();
	  print ("Removed expr that would need bookkeeping (but disabled or"
		 " expr was unique): ");
	  dump_expr (rhs);
	  line_finish ();
	  av_set_iter_remove (&i);
	}
    }
}

/* Records the number of fill_insns runs for debugging purposes.  */
static int fill_insns_run = 0;

/* Records the maximal UID before moving up an instruction.  Used for
   distinguishing between bookkeeping copies and original insns.  */
static int max_uid_before_move_op = 0;

/* Gather a parallel group of insns at FENCE and assign their seqno 
   to SEQNO.  All scheduled insns are gathered in SCHEDULED_INSNS_TAILPP 
   list for later recalculation of seqnos.  */
static void
fill_insns (fence_t fence, int seqno, ilist_t **scheduled_insns_tailpp)
{
  blist_t bnds = NULL, *bnds_tailp;
  av_set_t av_vliw = NULL;
  insn_t insn = FENCE_INSN (fence);
  state_t temp_state = alloca (dfa_state_size);

  blist_add (&bnds, insn, NULL, FENCE_DC (fence));
  bnds_tailp = &BLIST_NEXT (bnds);

  set_target_context (FENCE_TC (fence));

  print ("fill_insns (insn: %d, seqno: %d): start", INSN_UID (insn), seqno);
  block_start ();

  target_bb = INSN_BB (insn);

  /* Do while we can add any operation to the current group.  */
  do
    {
      blist_t bnds1, *bnds_tailp1, *bndsp, bnds_tail1;
      rhs_t rhs_vliw;

      line_start ();
      print ("bnds: ");
      dump_blist (bnds);
      line_finish ();

      /* Compute the set of available expressions.  */

      bnds1 = bnds;
      do
        {
	  bnd_t bnd = BLIST_BND (bnds1);
	  av_set_t av1_copy;
	  insn_t new_bnd_to = BND_TO (bnd);

	  /* Rewind BND->TO to the basic block header in case some bookkeeping
	     instructions were inserted before BND->TO and it needs to be
	     adjusted.  */
	  while (!NOTE_INSN_BASIC_BLOCK_P (PREV_INSN (new_bnd_to)))
            {
              new_bnd_to = PREV_INSN (new_bnd_to);

              /* Assert that this can only happen with unscheduled code.  */
              gcc_assert (INSN_SCHED_TIMES (new_bnd_to) == 0);
            }

	  BND_TO (bnd) = new_bnd_to;
	  gcc_assert (NOTE_INSN_BASIC_BLOCK_P (PREV_INSN (new_bnd_to)));

	  av_set_clear (&BND_AV (bnd));
	  BND_AV (bnd) = compute_av_set (BND_TO (bnd), NULL, 0, true);

	  av_set_clear (&BND_AV1 (bnd));
	  BND_AV1 (bnd) = av_set_copy (BND_AV (bnd));

          if (enable_moveup_set_path_p)
            moveup_set_path (&BND_AV1 (bnd), BND_PTR (bnd));

	  av1_copy = av_set_copy (BND_AV1 (bnd));
          av_set_union_and_clear (&av_vliw, &av1_copy);
        }
      while ((bnds1 = BLIST_NEXT (bnds1)));

      remove_insns_that_need_bookkeeping (fence, &av_vliw);

      sel_dump_cfg ("after-compute_av");

      /* If debug parameters tell us to ignore this attempt to move an insn,
	 obey.  */
      {
	int now;
	int start;
	int stop;
	bool do_p;

	now = ++fill_insns_run;
	start = PARAM_VALUE (PARAM_INSN_START);
	stop = PARAM_VALUE (PARAM_INSN_STOP);
	do_p = (PARAM_VALUE (PARAM_INSN_P) == 1);

	if (do_p)
	  do_p = (start <= now) && (now <= stop);
	else
	  do_p = (start > now) || (now > stop);

	if (!do_p)
	  /* Leave only the next insn in av_vliw.  */
	  {
	    av_set_iterator av_it;
	    rhs_t expr;
	    bnd_t bnd = BLIST_BND (bnds);
	    insn_t next = BND_TO (bnd);

	    gcc_assert (BLIST_NEXT (bnds) == NULL);

	    FOR_EACH_RHS_1 (expr, av_it, &av_vliw)
	      if (RHS_INSN (expr) != next)
		av_set_iter_remove (&av_it);
	  }
      }

      /* Now we've computed AV_VLIW - the set of expressions that can be
	 scheduled on FENCE.  */

      line_start ();
      print ("av_vliw: ");
      dump_av_set (av_vliw);
      line_finish ();

      if (av_vliw == NULL)
        break;

      /* Choose the best expression and, if needed, destination register
	 for it.  */
      rhs_vliw = find_best_expr (&av_vliw, bnds, fence);

      if (!rhs_vliw)
        {
          line_start ();
          print ("no best rhs found!");
          line_finish ();

          /* Reorder* hooks told us nothing more to schedule; indicate that
             a stall is needed.  */
          if (can_issue_more == 0)
            need_stall = 1;

	  av_set_clear (&av_vliw);
          break;
        }

      line_start ();
      print ("rhs_vliw: ");
      dump_rhs (rhs_vliw);
      print ("; cycle %d;", FENCE_CYCLE (fence));
      line_finish ();

      bndsp = &bnds;
      bnds_tailp1 = bnds_tailp;
      do
	/* !!! This code is guaranteed to execute only once.  */
        {
	  bnd_t bnd = BLIST_BND (*bndsp);
	  av_set_t rhs_seq = NULL;
	  rhs_t rhs;
	  av_set_iterator i;
	  insn_t *succs;
	  int succs_n;
	  bool asm_p;
	  bool first_p = true;

	  if (!av_set_is_in_p (BND_AV1 (bnd), RHS_VINSN (rhs_vliw)))
	    {
	      bndsp = &BLIST_NEXT (*bndsp);
	      bnds_tail1 = *bnds_tailp1;
	      continue;
	    }

	  FOR_EACH_RHS (rhs, i, BND_AV (bnd))
            {
              ilist_t root = enable_moveup_set_path_p ? BND_PTR (bnd) : NULL;

              if (equal_after_moveup_path_p (rhs, root, rhs_vliw))
		{
		  gcc_assert (first_p);
		  first_p = false;

                  /* The sequential expression has the right form to pass 
                     to move_op except when renaming happened.  Put the 
                     correct register in RHS then.  */
                  if (EXPR_SEPARABLE_P (rhs) && REG_P (EXPR_LHS (rhs))
                      && rhs_dest_regno (rhs) != rhs_dest_regno (rhs_vliw))
		    {
		      replace_dest_with_reg_in_rhs (rhs, EXPR_LHS (rhs_vliw));
		      stat_renamed_scheduled++;
		    }

		  av_set_add (&rhs_seq, rhs);
		  if (EXPR_WAS_SUBSTITUTED (rhs))
		    stat_substitutions_total++;
		}
            }

          line_start ();
          print ("rhs_seq: ");
          dump_av_set (rhs_seq);
          line_finish ();

	  /* Move chosen insn.  */
	  {
	    insn_t place_to_insert;
	    expr_def _c_rhs, *c_rhs = &_c_rhs;
	    bool b;
	    int n_bookkeeping_copies_before_moveop;

	    /* Init place_to_insert before calling move_op, as the later
	       can possibly remove BND_TO (bnd).  */
	    if (/* If this is not the first insn scheduled.  */
		BND_PTR (bnd))
	      {
		gcc_unreachable ();

		/* Add it after last scheduled.  */
		place_to_insert = ILIST_INSN (BND_PTR (bnd));
	      }
	    else
	      /* Add it before BND_TO.  The difference is in the
		 basic block, where INSN will be added.  */
	      place_to_insert = PREV_INSN (BND_TO (bnd));

	    /* Find a place for C_RHS to schedule.
	       We want to have an invariant that only insns that are
	       sel_bb_header_p () have a valid LV_SET.  But, in the same time,
	       we don't want overhead from recomputation of compute_live ()
	       for the half of a block after each movement.  Resolution of
	       this is floating bb header that will advance along with the
	       fence.

	       Please note that the invariant is an implication: e.g. there
	       can be sel_bb_header_p () insns that don't have a valid LV_SET.
	       To make an equivalence out of implication we need to invoke
	       compute_live () after scheduling of an insn that become
	       sel_bb_header_p () - the overhead will be insignificant because
	       this case is only possible when we start scheduling of a new
	       basic block.  Also I've just thought about another concerning
	       issue:
	       suppose we have a function from a single insn.  So far we've
	       stripped that insn from the stream in move_op () - and, hence,
	       deleted the only valid LV_SET - how are we supposed to get a
	       valid LV_SET for the inserted insn out of nowhere?  */

	    {
	      insn_t prev_insn = PREV_INSN (place_to_insert);
	      basic_block bb = BLOCK_FOR_INSN (place_to_insert);
	      basic_block prev_bb = bb->prev_bb;

	      if (!NOTE_INSN_BASIC_BLOCK_P (place_to_insert)
		  || prev_insn == NULL_RTX
		  /* Or it is a label, a barrier or something strange
		     alike.  */
		  || !INSN_P (prev_insn)
		  || BLOCK_FOR_INSN (prev_insn) != prev_bb
		  || !in_current_region_p (prev_bb)
		  || control_flow_insn_p (prev_insn))
		{
		  /* Generate a nop that will help us to avoid removing
		     data sets we need.  */
		  place_to_insert = NEXT_INSN (place_to_insert);
		  gcc_assert (BLOCK_FOR_INSN (place_to_insert) == bb);
		  place_to_insert = get_nop_from_pool (place_to_insert);

		  prev_bb = bb;

		  /* Split block to generate a new floating bb header.  */
		  bb = sched_split_block (bb, place_to_insert);
		}
	      else
		{
		  if (NOTE_INSN_BASIC_BLOCK_P (place_to_insert))
		    {
		      place_to_insert = NEXT_INSN (place_to_insert);
		      gcc_assert (BLOCK_FOR_INSN (place_to_insert) == bb);
		    }

		  /* Generate a nop that will help us to avoid removing
		     data sets we need.  */
		  place_to_insert = get_nop_from_pool (place_to_insert);

		  /* Move the nop to the previous block.  */
		  {
		    insn_t prev_insn = sel_bb_end (prev_bb);
		    insn_t note = bb_note (bb);
		    insn_t nop_insn = sel_bb_head (bb);
		    insn_t next_insn = NEXT_INSN (nop_insn);

		    gcc_assert (prev_insn != NULL_RTX
				&& nop_insn == place_to_insert
				&& PREV_INSN (note) == prev_insn);

		    NEXT_INSN (prev_insn) = nop_insn;
		    PREV_INSN (nop_insn) = prev_insn;

		    PREV_INSN (note) = nop_insn;
		    NEXT_INSN (note) = next_insn;

		    NEXT_INSN (nop_insn) = note;
		    PREV_INSN (next_insn) = note;

		    BB_END (prev_bb) = nop_insn;
		    BLOCK_FOR_INSN (nop_insn) = prev_bb;
		  }
		}

	      gcc_assert (single_succ (prev_bb) == bb);

	      sel_dump_cfg ("before-move_op");

	      /* Marker is useful to bind .dot dumps and the log.  */
	      print_marker_to_log ();

	      /* Make a move.  This call will remove the original operation,
		 insert all necessary bookkeeping instructions and update the
		 data sets.  After that all we have to do is add the operation
		 at before BND_TO (BND).  */
	      n_bookkeeping_copies_before_moveop = stat_bookkeeping_copies;
	      max_uid_before_move_op = get_max_uid ();

	      b = move_op (BND_TO (bnd), rhs_seq, NULL, NULL, NULL, c_rhs);

	      if (stat_bookkeeping_copies > n_bookkeeping_copies_before_moveop)
		stat_insns_needed_bookkeeping++;

	      /* We should be able to find the expression we've chosen for 
		 scheduling.  */
	      gcc_assert (b);

	      /* We want to use a pattern from rhs_vliw, because it could've 
		 been substituted, and the rest of data from rhs_seq.  */
	      if (! rtx_equal_p (EXPR_PATTERN (rhs_vliw), 
				 EXPR_PATTERN (c_rhs)))
		change_vinsn_in_expr (c_rhs, EXPR_VINSN (rhs_vliw));
	    }

	    /* Add the instruction.  */
	    insn = gen_insn_from_expr_after (c_rhs, seqno, place_to_insert);
	    clear_expr (c_rhs);

	    ++INSN_SCHED_TIMES (insn);

	    if (INSN_NOP_P (place_to_insert))
	      /* Return the nop generated for preserving of data sets back
		 into pool.  */
	      return_nop_to_pool (place_to_insert);
	  }

	  av_set_clear (&rhs_seq);

	  /* Advance the DFA.  */
	  if (recog_memoized (insn) >= 0)
	    {
	      gcc_assert (!INSN_ASM_P (insn));
	      asm_p = false;

              memcpy (temp_state, FENCE_STATE (fence), dfa_state_size);

	      succs_n = state_transition (FENCE_STATE (fence), insn);
	      gcc_assert (succs_n < 0);

              if (memcmp (temp_state, FENCE_STATE (fence), dfa_state_size))
                {
                  FENCE_ISSUED_INSNS (fence)++;
                  /* We should never issue more than issue_rate insns.  */
                  if (FENCE_ISSUED_INSNS (fence) > issue_rate)
                    gcc_unreachable ();
                }
	    } 
          else
            {
              asm_p = INSN_ASM_P (insn);
              
              /* This could be an ASM insn which we'd like to schedule 
                 on the next cycle.  */
              if (!FENCE_STARTS_CYCLE_P (fence) && asm_p)
		advance_one_cycle (fence);
            }

	  /* Set instruction scheduling info.  This will be used in bundling,
	     pipelining, tick computations etc.  */

          memcpy (temp_state, FENCE_STATE (fence), dfa_state_size);
          INSN_AFTER_STALL_P (insn) = FENCE_AFTER_STALL_P (fence);
	  INSN_SCHED_CYCLE (insn) = FENCE_CYCLE (fence);

	  if (asm_p)
	    advance_one_cycle (fence);

          FENCE_AFTER_STALL_P (fence) = 0;
          FENCE_STARTS_CYCLE_P (fence) = 0;
	  FENCE_LAST_SCHEDULED_INSN (fence) = insn;
          if (SCHED_GROUP_P (insn))
            {
              FENCE_SCHED_NEXT (fence) = INSN_SCHED_NEXT (insn);
              SCHED_GROUP_P (insn) = 0;
            }
          else
            FENCE_SCHED_NEXT (fence) = NULL_RTX;

	  advance_deps_context (BND_DC (bnd), insn);

	  /* Add insn to the list of scheduled on this cycle instructions.  */
	  ilist_add (*scheduled_insns_tailpp, insn);
	  *scheduled_insns_tailpp = &ILIST_NEXT (**scheduled_insns_tailpp);

	  line_start ();
	  print ("Scheduling insn: ");
	  dump_insn_1 (insn, 1);
	  line_finish ();

	  /* Add new boundaries.  */
	  cfg_succs_1 (insn, SUCCS_NORMAL | SUCCS_SKIP_TO_LOOP_EXITS, 
		       &succs, &succs_n);
	  while (succs_n--)
	    {
	      insn_t succ = succs[succs_n];
	      ilist_t ptr = ilist_copy (BND_PTR (bnd));

	      ilist_add (&ptr, insn);

	      blist_add (bnds_tailp, succ, ptr, FENCE_DC (fence));
	      bnds_tailp = &BLIST_NEXT (*bnds_tailp);
	    }
	  free (succs);

	  bnds_tail1 = *bnds_tailp1;
	  blist_remove (bndsp);

	  /* Check invariants.  */

	  /* Check that the recent movement didn't destroyed loop
	     structure.  */
	  gcc_assert (!flag_sel_sched_pipelining_outer_loops
		      || current_loop_nest == NULL
		      || loop_latch_edge (current_loop_nest));
        }
      while (*bndsp != bnds_tail1);

      av_set_clear (&av_vliw);

      /* Indicate that we've scheduled something on this fence.  */
      FENCE_SCHEDULED_SOMETHING (fence) = true;

      /* When can_issue_more is 0, variable_issue tells us that we should
        advance a cycle.  */
      if (can_issue_more == 0)
       {
         need_stall = 1;
         break;
       }

      /* We currently support information about candidate blocks only for
	 one 'target_bb' block.  Hence we can't schedule after jump insn,
	 as this will bring two boundaries and, hence, necessity to handle
	 information for two or more blocks concurrently.  */
      if (insn == BB_END (BLOCK_FOR_INSN (insn)))
	  break;

      /* !!! This is a possible perfomance regression as we schedule one
	 instruction at a time because of floating bb headers.  We need to
	 change the above condition to control_flow_insn_p (insn) or completely
	 remove it at all.  */
      gcc_unreachable ();
    }
  while (bnds);

  gcc_assert (!FENCE_BNDS (fence));
  
  if (need_stall)
    {
      /* Did we do all we could?  If no, we have a stall.  */ 
      need_stall = FENCE_ISSUED_INSNS (fence) < issue_rate;
      /* First advance a cycle, in case we'd clear after_stall_p flag, 
         and then set it.  */ 
      advance_one_cycle (fence);
      if (need_stall)
        FENCE_AFTER_STALL_P (fence) = 1;
      need_stall = 0;
    }

  /* Update boundaries of the FENCE.  */
  while (bnds)
    {
      ilist_t ptr = BND_PTR (BLIST_BND (bnds));

      if (ptr)
	{
	  insn = ILIST_INSN (ptr);

	  if (!ilist_is_in_p (FENCE_BNDS (fence), insn))
	    ilist_add (&FENCE_BNDS (fence), insn);
	}
      
      blist_remove (&bnds);
    }

  /* Update target context on the fence.  */
  reset_target_context (FENCE_TC (fence), false);

  block_finish ();
}

/* All exprs in ORIG_OPS must have the same destination register or memory.
   Return that destination.  */
static rtx
get_dest_from_orig_ops (av_set_t orig_ops)
{
  rtx dest = NULL_RTX;
  av_set_iterator av_it;
  expr_t expr;
  bool first_p = true;

  FOR_EACH_RHS (expr, av_it, orig_ops)
    {
      rtx x = EXPR_LHS (expr);

      if (first_p)
	{
	  first_p = false;
	  dest = x;
	}
      else
	gcc_assert (dest == x
		    || (dest != NULL_RTX && x != NULL_RTX
			&& rtx_equal_p (dest, x)));
    }

  return dest;
}

/* Move up the operations from ORIG_OPS set traversing 
   the dag started from INSN.  PATH represents the edges traversed so far.
   REG is the register chosen for scheduling the current rhs.  Insert
   bookkeeping code in the join points.  Return the current rhs.  */
static bool
move_op (insn_t insn, av_set_t orig_ops, ilist_t path, edge e1, edge e2,
	 rhs_t c_rhs)
{
  rhs_t rhs;
  bool c_rhs_inited_p;
  rtx dest;
  bool generated_nop_p = false;
  
  line_start ();
  print ("move_op(");
  dump_insn (insn);
  print (",");
  dump_av_set (orig_ops);
  print (")");
  line_finish ();
  block_start ();

  /* If no original operations exist below this insn, return immediately.  */
  if (!orig_ops || is_ineligible_successor (insn, path))
    {
      print ("ineligible_successor");
      block_finish ();
      return false;
    }

  orig_ops = av_set_copy (orig_ops);

  /* If we've found valid av set, then filter the orig_ops set.  */
  if (AV_SET_VALID_P (insn))
    {
      line_start ();
      print ("av");
      dump_insn (insn);
      print ("=");
      dump_av_set (AV_SET (insn));
      line_finish ();

      av_set_intersect (&orig_ops, AV_SET (insn));

      /* If no more original ops, return immediately.  */
      if (!orig_ops)
	{
	  print ("NULL");
	  block_finish ();
	  return false;
	}

      /* !!! When using different types of speculation we must not leave
	 just one element in orig_ops.  */
      /*av_set_leave_one (&orig_ops);*/
    }

  dest = get_dest_from_orig_ops (orig_ops);

  /* Look at the insn and decide if it could be an ancestor of currently 
     scheduling operation.  If it is so, then the insn "dest = op" could 
     either be replaced with "dest = reg", because REG now holds the result
     of OP, or just removed, if we've scheduled the insn as a whole.  

     If this insn doesn't contain currently scheduling OP, then proceed
     with searching and look at its successors.  Operations we're searching 
     for could have changed when moving up through this insn via 
     substituting.  In this case, firstly perform unsubstitution on them. 

     When traversing the DAG below this insn is finished, insert bookkeeping 
     code, if the insn is a joint point, and remove leftovers.  */

  rhs = av_set_lookup (orig_ops, INSN_VINSN (insn));

  if (rhs != NULL)
    /* We have found the original operation.  Replace it by REG, if 
       it is scheduled as RHS, or just remove it later, if it's an insn.  */
    {
      print ("found original operation!");

      copy_expr_onside (c_rhs, INSN_EXPR (insn));
      c_rhs_inited_p = true;

      /* When an insn we found is not equal to the insn from the orig_ops 
         set, this means that we've found previously created bookeeping copy.
         We cannot return original insn in C_RHS because it was already
         scheduled, and using it would break an assert saying that 
         insn should not be in stream.  We need to replace it in C_RHS with 
         the exact insn that we found.

	 Our infrastructure handles this such case with setting AV_LEVEL ()
	 to -1 in generate_bookkeeping_insn ().

	 But when there is identical instructions one of which dominates the
	 other we have the same assert failing.  */
      if (RHS_INSN (c_rhs) != insn)
	{
	  /* We now copy INSN_EXPR (insn) to C_RHS, hence the condition is
	     always false.  */
	  gcc_unreachable ();

	  change_vinsn_in_expr (c_rhs, INSN_VINSN (insn));
	}
      
      /* For instructions we must immediately remove insn from the
	 stream, so subsequent update_data_sets () won't include this
	 insn into av_set.
	 For rhs we must make insn look like "INSN_REG (insn) := c_rhs".  */
      if (INSN_UID (insn) > max_uid_before_move_op)
	stat_bookkeeping_copies--;

      {
	bool recovery_p = false;

	{
	  rtx cur_reg = expr_dest_reg (c_rhs);

	  gcc_assert (!cur_reg || (dest && REG_P (dest)));

	  /* If original operation has rhs and the register chosen for
	     that rhs is not original operation's dest reg, substitute
	     operation's right hand side with the register chosen.  */
	  if (cur_reg != NULL_RTX && REGNO (dest) != REGNO (cur_reg))
	    {
	      rtx reg_move_insn_rtx;
	      insn_t reg_move_insn;

	      reg_move_insn_rtx = create_insn_rtx_with_rhs (INSN_VINSN (insn),
							    dest);
	      reg_move_insn = sel_gen_insn_from_rtx_after (reg_move_insn_rtx,
							   INSN_EXPR (insn),
							   INSN_SEQNO (insn),
							   insn);
	      EXPR_SPEC_DONE_DS (INSN_EXPR (reg_move_insn)) = 0;

	      replace_dest_with_reg_in_rhs (c_rhs, dest);

	      recovery_p = true;
	    }
	}

	{
	  insn_t x;
	  ds_t check_ds = get_spec_check_type_for_insn (insn, rhs);

	  if (check_ds != 0)
	    {
	      /* A speculation check should be inserted.  */
	      x = create_speculation_check (c_rhs, check_ds, insn);

	      recovery_p = true;
	    }
	  else
	    {
	      EXPR_SPEC_DONE_DS (INSN_EXPR (insn)) = 0;
	      x = insn;
	    }

	  gcc_assert (EXPR_SPEC_DONE_DS (INSN_EXPR (x)) == 0
		      && EXPR_SPEC_TO_CHECK_DS (INSN_EXPR (x)) == 0);
	}

	{
	  insn_t x;

	  if (!recovery_p)
	    {
	      x = get_nop_from_pool (insn);

	      generated_nop_p = true;
	    }
	  else
	    x = NEXT_INSN (insn);

	  /* For the insns that don't have rhs just remove insn from the
	     stream.  Also remove insn if substituting it's right hand 
	     side would result in operation like reg:=reg.  This kind of
	     operation is not only excessive, but it may not be supported 
	     on certain platforms, e.g. "mov si, si" is invalid on i386.  */
	  sel_remove_insn (insn);
	  insn = x;
	}
      }
    }
  else
    {
      succ_iterator succ_i;
      rtx succ;

      c_rhs_inited_p = false;

	/* If we're scheduling separate rhs, in order to generate correct code
	   we need to stop the search at bookkeeping code generated with the 
	   same destination register or memory.  */
      if (lhs_of_insn_equals_to_dest_p (insn, dest))
	av_set_clear (&orig_ops);
      else
        /* Av set ops could have been changed when moving through this insn.
           To find them below it, we have to un-substitute them.  */
        undo_transformations (&orig_ops, insn);

      /* If all original opernads have been filtered on this branch,
	 return.  */
      if (!orig_ops)
	{
	  block_finish ();
	  return false;
	}

      /* Continue searching.  Do recursion here.  */
      ilist_add (&path, insn);

      FOR_EACH_SUCC (succ, succ_i, insn)
        {
	  bool b;
	  expr_def _x, *x = &_x;

	  if (succ_i.e1)
	    {
	      gcc_assert (succ_i.e2);

	      gcc_assert (succ_i.e1->src == BLOCK_FOR_INSN (insn)
			  && succ_i.e2->dest == BLOCK_FOR_INSN (succ));
	    }

	  b = move_op (succ, orig_ops, path, succ_i.e1, succ_i.e2, x);

          if (b)
	    {
              enum MOVEUP_RHS_CODE res;
              
	      line_start ();
	      print ("Checking for %d ", INSN_UID (insn));
	      dump_rhs (x);
	      line_finish ();

              res = moveup_rhs (x, insn, false);
              gcc_assert (res != MOVEUP_RHS_NULL);

	      if (!c_rhs_inited_p)
		{
		  copy_expr_onside (c_rhs, x);
		  c_rhs_inited_p = true;
		}
	      else
		/* We must merge all found expressions to get reasonable
		   EXPR_SPEC_DONE_DS () for the resulting insn.  If we don't
		   do so then we can first find the expr with epsilon
		   speculation success probability and only then with the
		   good probability.  As a result the insn will get epsilon
		   probability and will never be scheduled because of
		   weakness_cutoff in find_best_expr.

		   We also workaround this in can_overcome_dep_p ()
		   that consider low probability speculation success
		   dependencies as hard ones.

		   We call merge_expr_data () here instead of merge_expr ()
		   because due to speculation C_RHS and X may have the
		   same insns with different speculation types.  And as of
		   now such insns are considered non-correlable.  */
		merge_expr_data (c_rhs, x);

	      clear_expr (x);
	    }
	}

      ilist_remove (&path);
    }

  av_set_clear (&orig_ops);

  if (!c_rhs_inited_p)
    {
      block_finish ();
      return false;
    }

  /* We should generate bookkeeping code only if we are not at the
     top level of the move_op.  */
  if (e1 && sel_num_cfg_preds_gt_1 (insn))
    {
      /* INSN is a joint point, insert bookkeeping code here.  */
      generate_bookkeeping_insn (c_rhs, insn, e1, e2);
      gcc_assert (sel_bb_head_p (insn));
    }

  if (sel_bb_head_p (insn))
    update_data_sets (insn);
  else
    gcc_assert (AV_LEVEL (insn) == INSN_WS_LEVEL (insn));

  /* If INSN was previously marked for deletion, it's time to do it.  */
  if (generated_nop_p)
    {
      gcc_assert (INSN_NOP_P (insn));

      return_nop_to_pool (insn);
    }

  block_finish ();
  return c_rhs_inited_p;
}


/* Functions that work with regions.  */

/* Current number of seqno used in init_seqno and init_seqno_1.  */
static int cur_seqno;

/* A helper for init_seqno.  Traverse the region starting from BB and 
   compute seqnos for visited insns, marking visited bbs in VISITED_BBS.  */
static void
init_seqno_1 (basic_block bb, sbitmap visited_bbs)
{
  int bbi = BLOCK_TO_BB (bb->index);
  insn_t insn, note = bb_note (bb);
  insn_t succ_insn;
  succ_iterator si;


  SET_BIT (visited_bbs, bbi);

  FOR_EACH_SUCC_1 (succ_insn, si, BB_END (bb), 
		   SUCCS_NORMAL | SUCCS_SKIP_TO_LOOP_EXITS)
    {
      basic_block succ = BLOCK_FOR_INSN (succ_insn);
      int succ_bbi = BLOCK_TO_BB (succ->index);

      gcc_assert (in_current_region_p (succ));

      if (!TEST_BIT (visited_bbs, succ_bbi))
	{
	  gcc_assert (succ_bbi > bbi);

	  init_seqno_1 (succ, visited_bbs);
	}
    }

  for (insn = BB_END (bb); insn != note; insn = PREV_INSN (insn))
    INSN_SEQNO (insn) = cur_seqno--;
}

/* Initialize seqnos for the current region.  */
static int
init_seqno (bool rescheduling_p)
{
  sbitmap visited_bbs;

  visited_bbs = sbitmap_alloc (current_nr_blocks);
  sbitmap_zero (visited_bbs);

  cur_seqno = sched_max_luid - 1;

  init_seqno_1 (EBB_FIRST_BB (0), visited_bbs);

  gcc_assert (rescheduling_p || cur_seqno == 0);

  sbitmap_free (visited_bbs);

  return sched_max_luid - 1;
}

/* If region is a loop, add an empty basic block before its head, so no
   bookkeeping instructions will be generated on boundary edges.
   Return true if current region is a loop.  */
static bool
add_region_head (void)
{
  basic_block region_head = BASIC_BLOCK (BB_TO_BLOCK (0));

  /* We don't want to pipeline weird loops.  */
  if (EDGE_COUNT (region_head->preds) != 2
      || !(EDGE_PRED (region_head, 0)->flags & EDGE_FALLTHRU)
      || (EDGE_PRED (region_head, 1)->flags & EDGE_FALLTHRU))
    return false;
  else
    {
      basic_block fallthru_pred = EDGE_PRED (region_head, 0)->src;
      basic_block jump_pred = EDGE_PRED (region_head, 1)->src;
      basic_block new_region_head;

      if (in_current_region_p (fallthru_pred)
	  || !in_current_region_p (jump_pred))
	return false;

      can_add_real_insns_p = false;
      new_region_head = sel_create_basic_block_before (region_head);
      can_add_real_insns_p = true;

      RGN_WAS_PIPELINED_P (CONTAINING_RGN (BB_TO_BLOCK (0))) = 1;

      return true;
    }
}

/* Split all edges incoming to current region, but not those that 
   come to loop header, and not those that come to preheader.  */
static void
split_edges_incoming_to_rgn (void)
{
  int i;
  int cur_rgn = CONTAINING_RGN (BB_TO_BLOCK (0));
  edge e;
  VEC(edge, heap) *edges_to_split = NULL;

  if (!current_loop_nest)
    return;

  for (i = 0; i < RGN_NR_BLOCKS (cur_rgn); i++)
    {
      edge_iterator ei;
      basic_block bb;
      bool has_preds_in_rgn;

      bb = BASIC_BLOCK (BB_TO_BLOCK (i));

      /* Skip header, preheaders, and single pred blocks.  */
      if (bb == current_loop_nest->header)
        continue;
      if ((unsigned) bb->loop_depth < loop_depth (current_loop_nest))
        continue;
      if (EDGE_COUNT (bb->preds) < 2)
        continue;

      /* Skip also blocks that don't have preds in the region.  */
      has_preds_in_rgn = false;
      FOR_EACH_EDGE (e, ei, bb->preds)
        if (in_current_region_p (e->src))
          {
            has_preds_in_rgn = true;
            break;
          }
      if (!has_preds_in_rgn)
        continue;

      /* Record all edges we need to split.  */
      FOR_EACH_EDGE (e, ei, bb->preds)
        if (!in_current_region_p (e->src))
          VEC_safe_push (edge, heap, edges_to_split, e);
    }
  
  for (i = 0; VEC_iterate (edge, edges_to_split, i, e); i++)
    /* Some of edges could be already redirected by previous splits.
       So check this again before calling sel_split_edge.  */
    if (!in_current_region_p (e->src))
      sel_split_edge (e);

  VEC_free (edge, heap, edges_to_split);
}

/* Init scheduling data for RGN.  Returns true when this region should not 
   be scheduled.  */
static bool
sel_region_init (int rgn)
{
  int i;
  bb_vec_t bbs;

  rgn_setup_region (rgn);

  /* If this loop has any saved loop preheaders from nested loops,
     add these basic blocks to the current region.  */
  if (flag_sel_sched_pipelining_outer_loops)
    {
      current_loop_nest = get_loop_nest_for_rgn (rgn);
  
      if (current_loop_nest 
	  && LOOP_PREHEADER_BLOCKS (current_loop_nest))
        {
          sel_add_loop_preheader ();
          
          /* Check that we're starting with a valid information.  */
          gcc_assert (loop_latch_edge (current_loop_nest));
        }
    }

  bbs = VEC_alloc (basic_block, heap, current_nr_blocks);

  for (i = 0; i < current_nr_blocks; i++)
    VEC_quick_push (basic_block, bbs, BASIC_BLOCK (BB_TO_BLOCK (i)));

  sel_init_bbs (bbs, NULL);

  /* Initialize luids and dependence analysis which both sel-sched and haifa
     need.  */
  sched_init_luids (bbs, NULL, NULL, NULL);
  sched_deps_init (false);

  /* Initialize haifa data.  */
  {
    rgn_setup_sched_infos ();
    sel_set_sched_flags ();
    haifa_init_h_i_d (bbs, NULL, NULL, NULL);
  }

  if (sched_rgn_local_preinit (rgn))
    return true;

  /* Compute insn priorities in haifa style.  Then free haifa style 
     dependencies that we've calculated for this.  */
  compute_priorities ();

  if (sched_verbose >= 5)
    debug_rgn_dependencies (0);

  free_rgn_deps ();
  init_deps_global ();

  /* Setup flags that might affect initialization.  */
  {
    /* We don't need the semantics of moveup_set_path, because filtering of 
       dependencies inside a sched group is handled by tick_check_p and 
       the target.  */
    enable_moveup_set_path_p 
      = flag_sel_sched_substitute_inside_insn_group != 0;

    /* We need to treat insns as RHSes only when renaming is enabled.  */
    enable_schedule_as_rhs_p = (flag_sel_sched_renaming != 0);

    bookkeeping_p = (flag_sel_sched_bookkeeping != 0);

    pipelining_p = bookkeeping_p && (flag_sel_sched_pipelining != 0);

    reset_sched_cycles_p = (pipelining_p
			    && !flag_sel_sched_reschedule_pipelined);
  }

  /* Main initialization.  */
  sel_setup_sched_infos ();
  sel_init_global_and_expr (bbs);

  /* Finalize haifa-specific data.  */
  haifa_finish_h_i_d ();

  VEC_free (basic_block, heap, bbs);

  /* Set hooks so that no newly generated insn will go out unnoticed.  */
  sel_register_rtl_hooks ();
  sel_register_cfg_hooks ();

  if (pipelining_p)
    {
      /* If pipelining of outer loops is enabled, the loop header is
	 already created with loop optimizer, so if current region
	 has a corresponding loop nest, we should pipeline it.  */
      if (flag_sel_sched_pipelining_outer_loops)
	{
	  pipelining_p = (current_loop_nest != NULL);

	  if (pipelining_p)
	    split_edges_incoming_to_rgn ();
	}
      else
	pipelining_p = add_region_head ();
    }

  setup_dump_cfg_params (pipelining_p);

  /* !!! We call target.sched.md_init () for the whole region, but we invoke
     targetm.sched.md_finish () for every ebb.  */
  if (targetm.sched.md_init)
    /* None of the arguments are actually used in any target.  */
    targetm.sched.md_init (sched_dump, sched_verbose, -1);

  first_emitted_uid = get_max_uid () + 1;

  /* Reset register allocation ticks array.  */
  memset (reg_rename_tick, 0, sizeof reg_rename_tick);

  bitmap_initialize (forced_ebb_heads, 0);
  bitmap_clear (forced_ebb_heads);

  setup_nop_vinsn ();

  return false;
}

/* Free the scheduling data for the current region.  */
static void
sel_region_finish (void)
{
  int i;

  sel_finish_new_insns ();

  sched_finish_ready_list ();

  free_nop_pool ();

  /* Free the sort vector.  */
  if (vec_av_set)
    VEC_free (rhs_t, heap, vec_av_set);

  /* If LV_SET of the region head should be updated, do it now because
     there will be no other chance.  */
  {
    insn_t *succs;
    int succs_num;
    int i;

    cfg_succs_1 (bb_note (EBB_FIRST_BB (0)),
		 SUCCS_NORMAL | SUCCS_SKIP_TO_LOOP_EXITS,
		 &succs, &succs_num);

    gcc_assert (flag_sel_sched_pipelining_outer_loops
		|| succs_num == 1);

    for (i = 0; i < succs_num; i++)
      {
	insn_t insn = succs[i];
	basic_block bb = BLOCK_FOR_INSN (insn);

	if (!BB_LV_SET_VALID_P (bb))
	  compute_live (insn);
      }

    free (succs);
  }

  /* Emulate the Haifa scheduler for bundling.  */
  if (reload_completed && flag_schedule_emulate_haifa)
    {
      rtx insn, head, tail;
      int clock = 0, last_clock = 0;  
      bitmap scheduled_blocks;

      scheduled_blocks = BITMAP_ALLOC (NULL);

      for (i = 0; i < current_nr_blocks; i++)
        {
          basic_block bb = EBB_FIRST_BB (i), bb1;

          if (bitmap_bit_p (scheduled_blocks, i))
            continue;

	  /* While pipelining outer loops, skip bundling for loop 
             preheaders.  Those will be rescheduled in the outer loop.  */
	  if (sel_is_loop_preheader_p (bb))
	    continue;

          line_start ();
          print ("Finishing schedule in bbs: ");

          bb1 = bb;
          do
            {
              bitmap_set_bit (scheduled_blocks, BLOCK_TO_BB (bb1->index));
              print ("%d; ", bb1->index);
            }
          while (!bb_ends_ebb_p (bb1) && (bb1 = bb_next_bb (bb1)));

          line_finish ();

          get_ebb_head_tail (bb, bb1, &head, &tail);
 
          if (no_real_insns_p (head, tail))
            {
              continue;
            }

          current_sched_info->prev_head = PREV_INSN (head);
          current_sched_info->next_tail = NEXT_INSN (tail);
  
	  if (reset_sched_cycles_p)
	    {
	      int last_clock = 0;
	      int haifa_last_clock = -1;
	      int haifa_clock = 0;
	      insn_t next_tail = current_sched_info->next_tail;

	      if (targetm.sched.md_init)
		{
		  /* None of the arguments are actually used in any target.

		  NB: We should have md_reset () hook for cases like this.  */
		  targetm.sched.md_init (sched_dump, sched_verbose, -1);
		}

	      state_reset (curr_state);
	      advance_state (curr_state);

	      for (insn = head; insn != next_tail; insn = NEXT_INSN (insn))
		{
		  int cost, haifa_cost;
		  int sort_p;
		  bool asm_p;
		  int clock;

		  if (!INSN_P (insn))
		    continue;

		  asm_p = false;
		  clock = INSN_SCHED_CYCLE (insn);

		  cost = clock - last_clock;

		  /* Initialize HAIFA_COST.  */
		  if (recog_memoized (insn) < 0)
		    {
		      asm_p = INSN_ASM_P (insn);

		      if (asm_p)
			/* This is asm insn which *had* to be scheduled first
			   on the cycle.  */
			haifa_cost = 1;
		      else
			/* This is a use/clobber insn.  It should not change 
			   cost.  */
			haifa_cost = 0;
		    }
		  else
		    {
		      state_t tmp_state = alloca (dfa_state_size);

		      memcpy (tmp_state, curr_state, dfa_state_size);
		      haifa_cost = state_transition (tmp_state, insn);

		      print ("haifa_cost: %d", haifa_cost);
		      /* ??? We can't assert anything about cost here yet,
			 because sometimes our scheduler gets out of sync with
			 Haifa.
			 This is to be fixed.  */
		      if (haifa_cost == 0)
			haifa_cost = 1;
		      else if (haifa_cost < 0)
			haifa_cost = 0;
		    }

		  /* Stall for whatever cycles we've stalled before.  */
		  if (INSN_AFTER_STALL_P (insn) && cost > haifa_cost)
		    haifa_cost = cost;

		  if (haifa_cost > 0)
		    {
		      int i = haifa_cost;

		      while (i--)
			{
			  advance_state (curr_state);
			  print ("advance_state ()");
			}

		      haifa_clock += haifa_cost;
		    }
		  else
		    gcc_assert (haifa_cost == 0);

		  if (targetm.sched.dfa_new_cycle)
		    while (targetm.sched.dfa_new_cycle (sched_dump,
							sched_verbose,
							insn,
							haifa_last_clock,
							haifa_clock,
							&sort_p))
		      {
			advance_state (curr_state);
			haifa_clock++;
			print ("advance_state ()");
		      }

		  if (recog_memoized (insn) >= 0)
		    {
		      cost = state_transition (curr_state, insn);
		      gcc_assert (cost < 0);
		    }

		  if (targetm.sched.variable_issue)
		    targetm.sched.variable_issue (sched_dump, sched_verbose,
						  insn, 0);

		  INSN_SCHED_CYCLE (insn) = haifa_clock;

		  last_clock = clock;
		  haifa_last_clock = haifa_clock;
		}
	    }

          if (targetm.sched.md_init)
            /* None of the arguments are actually used in any target.

	       NB: We should have md_reset () hook for cases like this.  */
            targetm.sched.md_init (sched_dump, sched_verbose, -1);

          state_reset (curr_state);
          advance_state (curr_state);
          last_clock = -1;

          for (insn = head; insn != NEXT_INSN (tail); insn = NEXT_INSN (insn))
            {
              int cost;
  
              if (!INSN_P (insn))
		continue;

              clock = INSN_SCHED_CYCLE (insn);
              cost = (last_clock == -1) ? 1 : clock - last_clock;
  
	      gcc_assert (cost >= 0);

	      line_start ();
	      print ("cost: %d\t", cost);
	      dump_insn_1 (insn, 1);
	      line_finish ();

              if (issue_rate > 1
                  && GET_CODE (PATTERN (insn)) != USE
                  && GET_CODE (PATTERN (insn)) != CLOBBER)
                {
                  if (reload_completed && cost > 0)
                    PUT_MODE (insn, TImode);

                  last_clock = clock;
                }
            }

          if (targetm.sched.md_finish)
	    {
	      /* md_finish () can possibly emit new insns.  Move LV_SETs to
		 ones that happen to be emitted on bb header.  */
	      insn_init.what = INSN_INIT_WHAT_INSN;
	      targetm.sched.md_finish (sched_dump, sched_verbose);

	      /* Extend luids so that insns generated by the target will
		 get zero luid.  */
	      sched_init_luids (NULL, NULL, NULL, NULL);
	      insn_init.todo = 0;
	      sel_init_new_insns ();
	    }
        }

      /* NB: Don't forget to deallocate SCHEDULED_BLOCKS.  */
      BITMAP_FREE (scheduled_blocks);
    }

  sel_finish_global_and_expr ();

  bitmap_clear (forced_ebb_heads);

  free_nop_vinsn ();

  finish_deps_global ();
  sched_deps_local_finish ();
  sched_finish_luids ();

  sel_finish_bbs ();

  sel_unregister_cfg_hooks ();
  sel_unregister_rtl_hooks ();

  /* Reset MAX_ISSUE_SIZE.  */
  max_issue_size = 0;
}


/* Functions that implement the scheduler driver.  */

/* The main driver for scheduling a region.  This function is responsible 
   for correct propagation of fences (i.e. scheduling points) and creating 
   a group of parallel insns at each of them.  It also supports 
   pipelining.  */
static void
sel_sched_region_2 (sel_sched_region_2_data_t data)
{
  int orig_max_seqno = data->orig_max_seqno;
  int highest_seqno_in_use = orig_max_seqno;

  stat_bookkeeping_copies = 0;
  stat_insns_needed_bookkeeping = 0;
  stat_renamed_scheduled = 0;
  stat_substitutions_total = 0;

  while (fences)
    {
      flist_t fences1;
      struct flist_tail_def _new_fences, *new_fences = &_new_fences;
      int min_f, max_f, new_hs;
      ilist_t scheduled_insns = NULL;
      ilist_t *scheduled_insns_tailp = &scheduled_insns;
      ilist_iterator ii;
      insn_t insn;

      flist_tail_init (new_fences);

      line_start ();
      print ("fences: ");
      dump_flist (fences);
      line_finish ();
	  
      /* Calculate MIN_F and MAX_F.  */
      min_f = max_f = INSN_SEQNO (FENCE_INSN (FLIST_FENCE (fences)));
      fences1 = fences;
      while ((fences1 = FLIST_NEXT (fences1)))
	{
	  int seqno = INSN_SEQNO (FENCE_INSN (FLIST_FENCE (fences1)));

	  if (min_f > seqno)
	    min_f = seqno;
	  else if (max_f < seqno)
	    max_f = seqno;
	}

      fences1 = fences;
      do
	{
	  fence_t fence = NULL;
	  /* SEQNO is set to '0' to avoid 'uninitialized warning'.  */ 
	  int seqno = 0;
	  flist_t fences2 = fences;
	  bool first_p = true;
	  
	  /* Choose the next fence group to schedule.
	     NB: The fact, that insn can be scheduled only once
	     on the cycle is guaranteed by two properties:
	     1. seqnos of parallel groups decrease with each iteration.
	     2. If is_ineligible_successor () sees the larger seqno, it
	     checks if candidate insn is_in_current_fence_p ().  */
	  do
	    {
	      fence_t f = FLIST_FENCE (fences2);

	      if (!FENCE_SCHEDULED (f))
		{
		  int i = INSN_SEQNO (FENCE_INSN (f));

		  if (first_p || i > seqno)
		    {
		      seqno = i;
		      fence = f;

		      first_p = false;
		    }
		  else
		    /* ??? Seqnos of different groups should be different.  */
		    gcc_assert (1 || i != seqno);
		}
	    }
	  while ((fences2 = FLIST_NEXT (fences2)));

	  gcc_assert (fence);

	  /* As FENCE is nonnull, SEQNO is initialized.  */
	  seqno -= max_f + 1;
	  fill_insns (fence, seqno, &scheduled_insns_tailp);
	  FENCE_SCHEDULED (fence) = true;
	}
      /* This condition gives us the number of iterations equal to the number
	 of fence groups in fences.  */
      while ((fences1 = FLIST_NEXT (fences1)));

      line_start ();
      print ("existing fences: ");

      fences1 = fences;
      do
	{
	  fence_t fence = FLIST_FENCE (fences1);
	  insn_t insn;
	  state_t state = FENCE_STATE (fence);

	  if (/* This fence doesn't have any successors.  */
	      !FENCE_BNDS (fence))
	    {
	      if (!FENCE_SCHEDULED_SOMETHING (fence))
		/* Nothing was scheduled on this fence.  */
		{
		  int seqno;

		  insn = FENCE_INSN (fence);
		  seqno = INSN_SEQNO (insn);

		  gcc_assert (seqno > 0 && seqno <= orig_max_seqno);

                  print ("%d[%d] (not changed); ", 
                         INSN_UID (insn),
                         BLOCK_NUM (insn));

		  new_fences_add (new_fences,
				  insn, state, FENCE_DC (fence), 
                                  FENCE_TC (fence), 
                                  FENCE_LAST_SCHEDULED_INSN (fence), 
                                  FENCE_SCHED_NEXT (fence),
                                  FENCE_CYCLE (fence), 
				  FENCE_ISSUED_INSNS (fence),
                                  FENCE_STARTS_CYCLE_P (fence), 
                                  FENCE_AFTER_STALL_P (fence));

		  /* Null these fields so that they won't be freed twice.  */
		  FENCE_STATE (fence) = NULL;
		  FENCE_DC (fence) = NULL;
                  FENCE_TC (fence) = NULL;
		}

	      continue;
	    }

	  extract_new_fences_from (fence, new_fences, data);
	}
      while ((fences1 = FLIST_NEXT (fences1)));

      flist_clear (&fences);

      line_finish();

      fences = FLIST_TAIL_HEAD (new_fences);
      
      /* Actually, new_hs is the seqno of the instruction, that was
	 scheduled first (i.e. it is the first one in SCHEDULED_INSNS).  */
      if (scheduled_insns)
	{
	  new_hs = (INSN_SEQNO (ILIST_INSN (scheduled_insns))
		    + highest_seqno_in_use + max_f - min_f + 2);
	  gcc_assert (new_hs > highest_seqno_in_use);
	}
      else
	new_hs = highest_seqno_in_use;

      FOR_EACH_INSN (insn, ii, scheduled_insns)
	{
	  gcc_assert (INSN_SEQNO (insn) < 0);
	  INSN_SEQNO (insn) += highest_seqno_in_use + max_f - min_f + 2;
	  gcc_assert (INSN_SEQNO (insn) <= new_hs);
	}
      ilist_clear (&scheduled_insns);

      highest_seqno_in_use = new_hs;

      global_level++;
    }

  gcc_assert (data->orig_max_seqno == orig_max_seqno);
  data->highest_seqno_in_use = highest_seqno_in_use;

  print ("stats: %d,%d,%d,%d",
         stat_bookkeeping_copies,
         stat_insns_needed_bookkeeping,
         stat_renamed_scheduled,
         stat_substitutions_total);
}

/* Schedule a region.  When pipelining, search for possibly never scheduled 
   bookkeeping code and schedule it.  Reschedule pipelined code without 
   pipelining after.  */
static void
sel_sched_region_1 (void)
{
  struct sel_sched_region_2_data_def _data, *data = &_data;

  data->orig_max_seqno = init_seqno (false);

  if (data->orig_max_seqno < 1)
    {
      print ("empty region.");
      return;
    }

  fences = NULL;

  if (data->orig_max_seqno >= 1)
    {
      /* When pipelining outer loops, create fences on the loop header,
	 not preheader.  */
      if (current_loop_nest)
	init_fences (BB_END (EBB_FIRST_BB (0)));
      else
	init_fences (bb_note (EBB_FIRST_BB (0)));
    }

  global_level = 1;
  stage = 1;

  sel_sched_region_2 (data);

  gcc_assert (fences == NULL);

  if (pipelining_p)
    {
      int i;
      insn_t head;
      basic_block bb;
      struct flist_tail_def _new_fences;
      flist_tail_t new_fences = &_new_fences;

      pipelining_p = false;

      if (!flag_sel_sched_reschedule_pipelined)
        {
          /* Schedule newly created code, that has not been scheduled yet.  */
          bool do_p = true;

          while (do_p)
            {
              do_p = false;

              for (i = 0; i < current_nr_blocks; i++)
                {
                  bb = EBB_FIRST_BB (i);
                  head = sel_bb_head (bb);

                  /* While pipelining outer loops, skip bundling for loop 
                     preheaders.  Those will be rescheduled in the outer
		     loop.  */
                  if (sel_is_loop_preheader_p (bb))
                    {
                      clear_outdated_rtx_info (bb);
                      continue;
                    }
                  
                  if (head != NULL_RTX && INSN_SCHED_CYCLE (head) <= 0)
                    {
                      gcc_assert (INSN_SCHED_CYCLE (head) == 0);
                      flist_tail_init (new_fences);
  
                      /* Allow start of the scheduling at the code that was
                         generated during previous stages.  */
                      data->orig_max_seqno = data->highest_seqno_in_use;
  
                      /* Mark BB as head of the new ebb.  */
                      bitmap_set_bit (forced_ebb_heads, bb->index);
  
                      gcc_assert (fences == NULL);
  
                      init_fences (bb_note (bb));
  
                      sel_sched_region_2 (data);
  
                      do_p = true;
                      break;
                    }
                }
            }
	}
      else
        {
          basic_block loop_entry;

          /* Schedule region pre-header first, if not pipelining 
             outer loops.  */
          bb = EBB_FIRST_BB (0);
          head = sel_bb_head (bb);
          
          if (sel_is_loop_preheader_p (bb))          
            /* Don't leave old flags on insns in bb.  */
            clear_outdated_rtx_info (bb);
          else if (head != NULL_RTX)
            {
              gcc_assert (INSN_SCHED_CYCLE (head) == 0);
              flist_tail_init (new_fences);

              /* Allow start of the scheduling at the code that was
                 generated during previous stages.  */
              data->orig_max_seqno = data->highest_seqno_in_use;

              /* Mark BB as head of the new ebb.  */
              bitmap_set_bit (forced_ebb_heads, bb->index);

              gcc_assert (fences == NULL);

              init_fences (bb_note (bb));

              sel_sched_region_2 (data);
            }

          /* Reschedule pipelined code without pipelining.  */
          loop_entry = EBB_FIRST_BB (1);

	  /* Please note that loop_header (not preheader) might not be in
	     the current region.  Hence it is possible for loop_entry to have
	     arbitrary number of predecessors.  */
#if 0
	  /* ??? Why don't we assert that EBB_FIRST_BB (1) is an
	     actual loop entry?  There must be something wrong if we
	     somehow created an extra block before the loop.  */
          while (loop_entry && EDGE_COUNT (loop_entry->preds) == 1)
            loop_entry = loop_entry->next_bb;

          gcc_assert (loop_entry && EDGE_COUNT (loop_entry->preds) == 2);
#endif

          for (i = BLOCK_TO_BB (loop_entry->index); i < current_nr_blocks; i++)
            {
              insn_t insn, next_tail;

              bb = EBB_FIRST_BB (i);
              get_ebb_head_tail (bb, bb, &head, &next_tail);

              if (head == NULL_RTX || !INSN_P (head))
                continue;

              next_tail = NEXT_INSN (next_tail);

              /* Clear outdated information.  */
              for (insn = head;
                   insn != next_tail;
                   insn = NEXT_INSN (insn))
                {
                  gcc_assert (INSN_P (insn));
		  INSN_AFTER_STALL_P (insn) = 0;
                  INSN_SCHED_CYCLE (insn) = 0;

		  /* ??? Should we reset those counters which reside in
		     INSN_EXPR field (e.g. SPEC and SCHED_TIMES)?  */
		  /* For now we do need to zero SCHED_TIMES because we don't
		     want to skip dependencies from any instruction.  This
		     will be a subject to consider when we implement better
		     dependency tracking.  */
		  INSN_SCHED_TIMES (insn) = 0;
                }
            }

          data->orig_max_seqno = init_seqno (true);
          flist_tail_init (new_fences);

          /* Mark BB as head of the new ebb.  */
          bitmap_set_bit (forced_ebb_heads, loop_entry->index);

          gcc_assert (fences == NULL);

          init_fences (BB_END (EBB_FIRST_BB (0)));

          sel_sched_region_2 (data);
        }

      pipelining_p = true;
    }
}

/* Schedule the RGN region.  */
void
sel_sched_region (int rgn)
{
  if (sel_region_init (rgn))
    return;

  sel_dump_cfg ("after-region-init");

  print ("sel_sched_region: start");
  block_start ();

  {
    /* Decide if we want to schedule this region.  */
    int region;
    int region_start;
    int region_stop;
    bool region_p;
    bool schedule_p;
    
    region = ++sel_sched_region_run;
    region_start = PARAM_VALUE (PARAM_REGION_START);
    region_stop = PARAM_VALUE (PARAM_REGION_STOP);
    region_p = (PARAM_VALUE (PARAM_REGION_P) == 1);

    if (region_p)
      schedule_p = (region_start <= region) && (region <= region_stop);
    else
      schedule_p = (region_start > region) || (region > region_stop);

    if (schedule_p)
      sel_sched_region_1 ();
    else
      /* Force initialization of INSN_SCHED_CYCLEs for correct bundling.  */
      reset_sched_cycles_p = true;
  }

  sel_region_finish ();

  sel_dump_cfg_1 ("after-region-finish",
		  SEL_DUMP_CFG_CURRENT_REGION | SEL_DUMP_CFG_LV_SET
		  | SEL_DUMP_CFG_BB_INSNS);

  block_finish ();
  print ("sel_sched_region: finish");
}

/* Perform global init for the scheduler.  */
static void
sel_global_init (void)
{
  /* Pipelining outer loops is only possible when general pipelining
     capabilities are requested.  
     FIXME: move this in opts.c.  */
  if (!flag_sel_sched_pipelining)
    flag_sel_sched_pipelining_outer_loops = 0;

  calculate_dominance_info (CDI_DOMINATORS);

  if (flag_sel_sched_pipelining_outer_loops)
    pipeline_outer_loops_init ();

  setup_sched_dump_to_stderr ();

  /* Setup the infos for sched_init.  */
  sel_setup_sched_infos ();

  sched_init ();

  /* Init lv_sets.  */
  {
    bb_vec_t bbs = VEC_alloc (basic_block, heap, n_basic_blocks);
    basic_block bb;

    FOR_ALL_BB (bb)
      VEC_quick_push (basic_block, bbs, bb);

    sched_init_bbs (bbs, NULL);

    VEC_free (basic_block, heap, bbs);
  }

  sched_rgn_init (flag_sel_sched_single_block_regions != 0,
                  flag_sel_sched_ebb_regions != 0);

  sched_extend_target ();
  sched_deps_init (true);

  setup_nop_and_exit_insns ();

  sel_extend_global_bb_info ();

  init_lv_sets ();

  init_hard_regs_data ();
}

/* Free the global data of the scheduler.  */
static void
sel_global_finish (void)
{
  free_bb_note_pool ();

  free_lv_sets ();
  sel_finish_global_bb_info ();

  free_regset_pool ();

  free_nop_and_exit_insns ();

  sched_rgn_finish ();
  sched_deps_finish ();
  sched_finish_bbs ();
  sched_finish ();

  free_sel_dump_data ();

  if (flag_sel_sched_pipelining_outer_loops)
    pipeline_outer_loops_finish ();

  free_dominance_info (CDI_DOMINATORS);
}

/* The entry point.  */
void
selective_scheduling_run (void)
{
  int rgn;
  int old_sched_verbose_param = sched_verbose_param;
  int sel_sched_verbose = PARAM_VALUE (PARAM_SELSCHED_VERBOSE);

  /* Taking care of this degenerate case makes the rest of
     this code simpler.  */
  if (n_basic_blocks == NUM_FIXED_BLOCKS)
    return;

  if (flag_sel_sched_verbose)
    sel_sched_verbose = 32;

  if (sel_sched_verbose)
    sched_verbose_param = sel_sched_verbose;

  setup_dump_cfg_params (false);

  sel_dump_cfg_1 ("before-init",
		  (SEL_DUMP_CFG_BB_INSNS | SEL_DUMP_CFG_FUNCTION_NAME
		   | SEL_DUMP_CFG_BB_LIVE));

  sel_global_init ();

  block_start ();
  print ("selective_scheduling_run: start");
  block_start ();

  for (rgn = 0; rgn < nr_regions; rgn++)
    {
      char *buf;
      int buf_len = 1 + snprintf (NULL, 0, "before-region-%d", rgn);

      buf = xmalloc (buf_len * sizeof (*buf));
      snprintf (buf, buf_len, "before-region-%d", rgn);
      sel_dump_cfg_1 (buf, SEL_DUMP_CFG_LV_SET | SEL_DUMP_CFG_BB_INSNS);

      sel_sched_region (rgn);

      snprintf (buf, buf_len, "after-region-%d", rgn);
      sel_dump_cfg_1 (buf, SEL_DUMP_CFG_LV_SET | SEL_DUMP_CFG_BB_INSNS);
      free (buf);
    }

  block_finish ();
  print ("selective_scheduling_run: finish");
  block_finish ();

  sel_global_finish ();

  sel_dump_cfg_1 ("after-finish",
		  (SEL_DUMP_CFG_BB_INSNS | SEL_DUMP_CFG_FUNCTION_NAME
		   | SEL_DUMP_CFG_BB_LIVE));

  sched_verbose_param = old_sched_verbose_param;
}

#endif

/* A gate function for selective scheduling.  */
static bool
gate_handle_sel_sched (void)
{
#ifdef INSN_SCHEDULING
  return (reload_completed 
          ? flag_selective_scheduling2 && flag_schedule_insns_after_reload
          : flag_selective_scheduling && flag_schedule_insns);
#else
  return false;
#endif
}

static int sel1_run = 0;

/* Run instruction scheduler.  */
static unsigned int
handle_sel_sched (void)
{
  if (reload_completed)
    split_all_insns ();
#ifdef INSN_SCHEDULING
  {
    int now;
    int start;
    int stop;
    bool do_p;

    now = ++sel1_run;
    start = PARAM_VALUE (PARAM_SEL1_START);
    stop = PARAM_VALUE (PARAM_SEL1_STOP);
    do_p = (PARAM_VALUE (PARAM_SEL1_P) == 1);

    if (do_p)
      do_p = (start <= now) && (now <= stop);
    else
      do_p = (start > now) || (now > stop);

    if ((flag_selective_scheduling || flag_selective_scheduling2) && do_p)
      selective_scheduling_run ();
    else
      schedule_insns ();
  }
#endif
  return 0;
}

struct tree_opt_pass pass_sel_sched =
  {
    "sel_sched",                          /* name */
    gate_handle_sel_sched,                /* gate */
    handle_sel_sched,                 	/* execute */
    NULL,                                 /* sub */
    NULL,                                 /* next */
    0,                                    /* static_pass_number */
    TV_SEL_SCHED,                         /* tv_id */
    0,                                    /* properties_required */
    0,                                    /* properties_provided */
    0,                                    /* properties_destroyed */
    0,                                    /* todo_flags_start */
    TODO_dump_func |
    TODO_ggc_collect,                     /* todo_flags_finish */
    'S'                                   /* letter */
  };

