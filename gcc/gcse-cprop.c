/* Global common subexpression elimination/Partial redundancy elimination
   and global constant/copy propagation for GNU compiler.
   Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002
   Free Software Foundation, Inc.

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
#include "toplev.h"

#include "rtl.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "real.h"
#include "insn-config.h"
#include "recog.h"
#include "basic-block.h"
#include "output.h"
#include "function.h"
#include "expr.h"
#include "except.h"
#include "ggc.h"
#include "params.h"
#include "cselib.h"

#include "obstack.h"

#include "gcse-globals.h"
#include "gcse-utils.h"

static void alloc_cprop_mem		PARAMS ((int, struct cprop_global *));
static void free_cprop_mem		PARAMS ((struct cprop_global *));
static int cprop_jump			PARAMS ((basic_block, rtx, rtx, rtx, rtx, int *));
static int cprop_insn			PARAMS ((rtx, int, struct cprop_global *, int *));
static rtx fis_get_condition		PARAMS ((rtx));
static void find_implicit_sets		PARAMS ((void));
static void local_cprop_find_used_regs	PARAMS ((rtx *, void *));
static void compute_cprop_data		PARAMS ((struct cprop_global *));
static void find_used_regs		PARAMS ((rtx *, void *));
static int try_replace_reg		PARAMS ((rtx, rtx, rtx));
static struct expr *find_avail_set 	PARAMS ((int, rtx, struct cprop_global *));
static bool constprop_register		PARAMS ((rtx, rtx, rtx, int, int *));
static struct expr *find_bypass_set	PARAMS ((int, int, struct cprop_global *));
static int bypass_block			PARAMS ((basic_block, rtx, rtx, struct cprop_global *));
static int bypass_conditional_jumps	PARAMS ((struct cprop_global *));
static bool do_local_cprop		PARAMS ((rtx, rtx, int, int *, rtx *));
static bool adjust_libcall_notes	PARAMS ((rtx, rtx, rtx, rtx*));
static void local_cprop_pass		PARAMS ((int, int *, struct cprop_global *));
static int cprop			PARAMS ((int, struct cprop_global *, int *));

/* Allocate vars used for copy/const propagation.  N_BLOCKS is the number of
   basic blocks.  N_SETS is the number of sets.  */

static void
alloc_cprop_mem (n_blocks, cprop_data)
     int n_blocks;
     struct cprop_global *cprop_data;
{
  cprop_data->pavloc = sbitmap_vector_alloc (n_blocks, cprop_data->set_hash_table.n_elems);
  cprop_data->absaltered = sbitmap_vector_alloc (n_blocks, cprop_data->set_hash_table.n_elems);

  cprop_data->avin = sbitmap_vector_alloc (n_blocks, cprop_data->set_hash_table.n_elems);
  cprop_data->avout = sbitmap_vector_alloc (n_blocks, cprop_data->set_hash_table.n_elems);
}

/* Free vars used by copy/const propagation.  */

static void
free_cprop_mem (cprop_data)
     struct cprop_global *cprop_data;
{
  sbitmap_vector_free (cprop_data->pavloc);
  sbitmap_vector_free (cprop_data->absaltered);
  sbitmap_vector_free (cprop_data->avin);
  sbitmap_vector_free (cprop_data->avout);
}

/* Top level routine to do the dataflow analysis needed by copy/const
   propagation.  */

static void
compute_cprop_data (cprop_data)
     struct cprop_global *cprop_data;
{
  compute_local_properties (cprop_data->absaltered, cprop_data->pavloc, NULL, &cprop_data->set_hash_table);
  compute_available (cprop_data->pavloc, cprop_data->absaltered,
		     cprop_data->avout, cprop_data->avin);
}

/* Copy/constant propagation.  */

/* Set up a list of register numbers used in INSN.  The found uses are stored
   in `reg_use_table'.  `reg_use_count' is initialized to zero before entry,
   and contains the number of uses in the table upon exit.

   ??? If a register appears multiple times we will record it multiple times.
   This doesn't hurt anything but it will slow things down.  */

static void
find_used_regs (xptr, data)
     rtx *xptr;
     void *data;
{
  int i, j;
  enum rtx_code code;
  const char *fmt;
  rtx x = *xptr;
  struct cprop_global *cprop_data = (struct cprop_global *) data;

  /* repeat is used to turn tail-recursion into iteration since GCC
     can't do it when there's no return value.  */
 repeat:
  if (x == 0)
    return;

  code = GET_CODE (x);
  if (REG_P (x))
    {
      if (cprop_data->reg_use_count == MAX_USES)
	return;

      cprop_data->reg_use_table[cprop_data->reg_use_count].reg_rtx = x;
      cprop_data->reg_use_count++;
    }

  /* Recursively scan the operands of this expression.  */

  for (i = GET_RTX_LENGTH (code) - 1, fmt = GET_RTX_FORMAT (code); i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  /* If we are about to do the last recursive call
	     needed at this level, change it into iteration.
	     This function is called enough to be worth it.  */
	  if (i == 0)
	    {
	      x = XEXP (x, 0);
	      goto repeat;
	    }

	  find_used_regs (&XEXP (x, i), data);
	}
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  find_used_regs (&XVECEXP (x, i, j), data);
    }
}

/* Try to replace all non-SET_DEST occurrences of FROM in INSN with TO.
   Returns non-zero is successful.  */

static int
try_replace_reg (from, to, insn)
     rtx from, to, insn;
{
  rtx note = find_reg_equal_equiv_note (insn);
  rtx src = 0;
  int success = 0;
  rtx set = single_set (insn);

  validate_replace_src_group (from, to, insn);
  if (num_changes_pending () && apply_change_group ())
    success = 1;

  if (!success && set && reg_mentioned_p (from, SET_SRC (set)))
    {
      /* If above failed and this is a single set, try to simplify the source of
	 the set given our substitution.  We could perhaps try this for multiple
	 SETs, but it probably won't buy us anything.  */
      src = simplify_replace_rtx (SET_SRC (set), from, to);

      if (!rtx_equal_p (src, SET_SRC (set))
	  && validate_change (insn, &SET_SRC (set), src, 0))
	success = 1;

      /* If we've failed to do replacement, have a single SET, and don't already
	 have a note, add a REG_EQUAL note to not lose information.  */
      if (!success && note == 0 && set != 0)
	note = set_unique_reg_note (insn, REG_EQUAL, copy_rtx (src));
    }

  /* If there is already a NOTE, update the expression in it with our
     replacement.  */
  else if (note != 0)
    XEXP (note, 0) = simplify_replace_rtx (XEXP (note, 0), from, to);

  /* REG_EQUAL may get simplified into register.
     We don't allow that. Remove that note. This code ought
     not to happen, because previous code ought to syntetize
     reg-reg move, but be on the safe side.  */
  if (note && REG_P (XEXP (note, 0)))
    remove_note (insn, note);

  return success;
}

/* Find a set of REGNOs that are available on entry to INSN's block.  Returns
   NULL no such set is found.  */

static struct expr *
find_avail_set (regno, insn, cprop_data)
     int regno;
     rtx insn;
     struct cprop_global *cprop_data;
{
  /* SET1 contains the last set found that can be returned to the caller for
     use in a substitution.  */
  struct expr *set1 = 0;

  /* Loops are not possible here.  To get a loop we would need two sets
     available at the start of the block containing INSN.  ie we would
     need two sets like this available at the start of the block:

       (set (reg X) (reg Y))
       (set (reg Y) (reg X))

     This can not happen since the set of (reg Y) would have killed the
     set of (reg X) making it unavailable at the start of this block.  */
  while (1)
    {
      rtx src;
      struct expr *set = lookup_set (regno, &cprop_data->set_hash_table);

      /* Find a set that is available at the start of the block
	 which contains INSN.  */
      while (set)
	{
	  if (TEST_BIT (cprop_data->avin[BLOCK_NUM (insn)], set->bitmap_index))
	    break;
	  set = next_set (regno, set);
	}

      /* If no available set was found we've reached the end of the
	 (possibly empty) copy chain.  */
      if (set == 0)
	break;

      if (GET_CODE (set->expr) != SET)
	abort ();

      src = SET_SRC (set->expr);

      /* We know the set is available.
	 Now check that SRC is ANTLOC (i.e. none of the source operands
	 have changed since the start of the block).

         If the source operand changed, we may still use it for the next
         iteration of this loop, but we may not use it for substitutions.  */

      if (CONSTANT_P (src) || oprs_not_set_p (src, insn))
	set1 = set;

      /* If the source of the set is anything except a register, then
	 we have reached the end of the copy chain.  */
      if (GET_CODE (src) != REG)
	break;

      /* Follow the copy chain, ie start another iteration of the loop
	 and see if we have an available copy into SRC.  */
      regno = REGNO (src);
    }

  /* SET1 holds the last set that was available and anticipatable at
     INSN.  */
  return set1;
}

/* Subroutine of cprop_insn that tries to propagate constants into
   JUMP_INSNS.  JUMP must be a conditional jump.  If SETCC is non-NULL
   it is the instruction that immediately precedes JUMP, and must be a
   single SET of a register.  FROM is what we will try to replace,
   SRC is the constant we will try to substitute for it.  Returns nonzero
   if a change was made.  */

static int
cprop_jump (bb, setcc, jump, from, src, run_jump_opt_after_gcse)
     basic_block bb;
     rtx setcc;
     rtx jump;
     rtx from;
     rtx src;
     int *run_jump_opt_after_gcse;
{
  rtx new, new_set;
  rtx set = pc_set (jump);

  /* First substitute in the INSN condition as the SET_SRC of the JUMP,
     then substitute that given values in this expanded JUMP.  */
  if (setcc != NULL)
    {
      rtx setcc_set = single_set (setcc);
      new_set = simplify_replace_rtx (SET_SRC (set),
				      SET_DEST (setcc_set),
				      SET_SRC (setcc_set));
    }
  else
    new_set = set;

  new = simplify_replace_rtx (new_set, from, src);

  /* If no simplification can be made, then try the next
     register.  */
  if (rtx_equal_p (new, new_set))
    return 0;

  /* If this is now a no-op delete it, otherwise this must be a valid insn.  */
  if (new == pc_rtx)
    delete_insn (jump);
  else
    {
      if (! validate_change (jump, &SET_SRC (set), new, 0))
	return 0;

      /* If this has turned into an unconditional jump,
	 then put a barrier after it so that the unreachable
	 code will be deleted.  */
      if (GET_CODE (SET_SRC (set)) == LABEL_REF)
	emit_barrier_after (jump);
     }

#ifdef HAVE_cc0
  /* Delete the cc0 setter.  */
  if (setcc != NULL && CC0_P (SET_DEST (single_set (setcc))))
    delete_insn (setcc);
#endif

  *run_jump_opt_after_gcse = 1;

  const_prop_count++;
  if (gcse_file != NULL)
    {
      fprintf (gcse_file,
	       "CONST-PROP: Replacing reg %d in jump_insn %d with constant ",
	       REGNO (from), INSN_UID (jump));
      print_rtl (gcse_file, src);
      fprintf (gcse_file, "\n");
    }
  purge_dead_edges (bb);

  return 1;
}

static bool
constprop_register (insn, from, to, alter_jumps, run_jump_opt_after_gcse)
     rtx insn;
     rtx from;
     rtx to;
     int alter_jumps;
     int *run_jump_opt_after_gcse;
{
  rtx sset;

  /* Check for reg or cc0 setting instructions followed by
     conditional branch instructions first.  */
  if (alter_jumps
      && (sset = single_set (insn)) != NULL
      && NEXT_INSN (insn)
      && any_condjump_p (NEXT_INSN (insn)) && onlyjump_p (NEXT_INSN (insn)))
    {
      rtx dest = SET_DEST (sset);
      if ((REG_P (dest) || CC0_P (dest))
	  && cprop_jump (BLOCK_FOR_INSN (insn), insn, NEXT_INSN (insn), from, to,
			 run_jump_opt_after_gcse))
	return 1;
    }

  /* Handle normal insns next.  */
  if (GET_CODE (insn) == INSN
      && try_replace_reg (from, to, insn))
    return 1;

  /* Try to propagate a CONST_INT into a conditional jump.
     We're pretty specific about what we will handle in this
     code, we can extend this as necessary over time.

     Right now the insn in question must look like
     (set (pc) (if_then_else ...))  */
  else if (alter_jumps && any_condjump_p (insn) && onlyjump_p (insn))
    return cprop_jump (BLOCK_FOR_INSN (insn), NULL, insn, from, to,
		       run_jump_opt_after_gcse);
  return 0;
}

/* Similar to get_condition, only the resulting condition must be
   valid at JUMP, instead of at EARLIEST.

   This differs from noce_get_condition in ifcvt.c in that we prefer not to
   settle for the condition variable in the jump instruction being integral.
   We prefer to be able to record the value of a user variable, rather than
   the value of a temporary used in a condition.  This could be solved by
   recording the value of *every* register scaned by canonicalize_condition,
   but this would require some code reorganization.  */

static rtx
fis_get_condition (jump)
     rtx jump;
{
  rtx cond, set, tmp, insn, earliest;
  bool reverse;

  if (! any_condjump_p (jump))
    return NULL_RTX;

  set = pc_set (jump);
  cond = XEXP (SET_SRC (set), 0);

  /* If this branches to JUMP_LABEL when the condition is false,
     reverse the condition.  */
  reverse = (GET_CODE (XEXP (SET_SRC (set), 2)) == LABEL_REF
	     && XEXP (XEXP (SET_SRC (set), 2), 0) == JUMP_LABEL (jump));

  /* Use canonicalize_condition to do the dirty work of manipulating
     MODE_CC values and COMPARE rtx codes.  */
  tmp = canonicalize_condition (jump, cond, reverse, &earliest, NULL_RTX);
  if (!tmp)
    return NULL_RTX;

  /* Verify that the given condition is valid at JUMP by virtue of not
     having been modified since EARLIEST.  */
  for (insn = earliest; insn != jump; insn = NEXT_INSN (insn))
    if (INSN_P (insn) && modified_in_p (tmp, insn))
      break;
  if (insn == jump)
    return tmp;

  /* The condition was modified.  See if we can get a partial result
     that doesn't follow all the reversals.  Perhaps combine can fold
     them together later.  */
  tmp = XEXP (tmp, 0);
  if (!REG_P (tmp) || GET_MODE_CLASS (GET_MODE (tmp)) != MODE_INT)
    return NULL_RTX;
  tmp = canonicalize_condition (jump, cond, reverse, &earliest, tmp);
  if (!tmp)
    return NULL_RTX;

  /* For sanity's sake, re-validate the new result.  */
  for (insn = earliest; insn != jump; insn = NEXT_INSN (insn))
    if (INSN_P (insn) && modified_in_p (tmp, insn))
      return NULL_RTX;

  return tmp;
}

/* Find the implicit sets of a function.  An "implicit set" is a constraint
   on the value of a variable, implied by a conditional jump.  For example,
   following "if (x == 2)", the then branch may be optimized as though the
   conditional performed an "explicit set", in this example, "x = 2".  This
   function records the set patterns that are implicit at the start of each
   basic block.  */

static void
find_implicit_sets ()
{
  basic_block bb, dest;
  unsigned int count;
  rtx cond, new;

  count = 0;
  FOR_EACH_BB (bb)
    /* Check for more than one sucessor.  */
    if (bb->succ && bb->succ->succ_next)
      {
	cond = fis_get_condition (bb->end);

	if (cond
	    && (GET_CODE (cond) == EQ || GET_CODE (cond) == NE)
	    && GET_CODE (XEXP (cond, 0)) == REG
	    && REGNO (XEXP (cond, 0)) >= FIRST_PSEUDO_REGISTER
	    && CONSTANT_P (XEXP (cond, 1)))
	  {
	    dest = GET_CODE (cond) == EQ ? BRANCH_EDGE (bb)->dest
					 : FALLTHRU_EDGE (bb)->dest;

	    if (dest && ! dest->pred->pred_next
		&& dest != EXIT_BLOCK_PTR)
	      {
		new = gen_rtx_SET (VOIDmode, XEXP (cond, 0),
					     XEXP (cond, 1));
		implicit_sets[dest->index] = new;
		if (gcse_file)
		  {
		    fprintf(gcse_file, "Implicit set of reg %d in ",
			    REGNO (XEXP (cond, 0)));
		    fprintf(gcse_file, "basic block %d\n", dest->index);
		  }
		count++;
	      }
	  }
      }

  if (gcse_file)
    fprintf (gcse_file, "Found %d implicit sets\n", count);
}


/* Perform constant and copy propagation on INSN.
   The result is non-zero if a change was made.  */

static int
cprop_insn (insn, alter_jumps, cprop_data, run_jump_opt_after_gcse)
     rtx insn;
     int alter_jumps;
     struct cprop_global *cprop_data;
     int *run_jump_opt_after_gcse;
{
  struct reg_use *reg_used;
  int changed = 0;
  rtx note;

  if (!INSN_P (insn))
    return 0;

  cprop_data->reg_use_count = 0;
  note_uses (&PATTERN (insn), find_used_regs, cprop_data);

  note = find_reg_equal_equiv_note (insn);

  /* We may win even when propagating constants into notes.  */
  if (note)
    find_used_regs (&XEXP (note, 0), cprop_data);

  for (reg_used = &cprop_data->reg_use_table[0]; cprop_data->reg_use_count > 0;
       reg_used++, cprop_data->reg_use_count--)
    {
      unsigned int regno = REGNO (reg_used->reg_rtx);
      rtx pat, src;
      struct expr *set;

      /* Ignore registers created by GCSE.
	 We do this because ...  */
      if (regno >= max_gcse_regno)
	continue;

      /* If the register has already been set in this block, there's
	 nothing we can do.  */
      if (! oprs_not_set_p (reg_used->reg_rtx, insn))
	continue;

      /* Find an assignment that sets reg_used and is available
	 at the start of the block.  */
      set = find_avail_set (regno, insn, cprop_data);
      if (! set)
	continue;

      pat = set->expr;
      /* ??? We might be able to handle PARALLELs.  Later.  */
      if (GET_CODE (pat) != SET)
	abort ();

      src = SET_SRC (pat);

      /* Constant propagation.  */
      if (CONSTANT_P (src))
	{
          if (constprop_register (insn, reg_used->reg_rtx, src, alter_jumps, run_jump_opt_after_gcse))
	    {
	      changed = 1;
	      const_prop_count++;
	      if (gcse_file != NULL)
		{
		  fprintf (gcse_file, "GLOBAL CONST-PROP: Replacing reg %d in ", regno);
		  fprintf (gcse_file, "insn %d with constant ", INSN_UID (insn));
		  print_rtl (gcse_file, src);
		  fprintf (gcse_file, "\n");
		}
	    }
	}
      else if (GET_CODE (src) == REG
	       && REGNO (src) >= FIRST_PSEUDO_REGISTER
	       && REGNO (src) != regno)
	{
	  if (try_replace_reg (reg_used->reg_rtx, src, insn))
	    {
	      changed = 1;
	      copy_prop_count++;
	      if (gcse_file != NULL)
		{
		  fprintf (gcse_file, "GLOBAL COPY-PROP: Replacing reg %d in insn %d",
			   regno, INSN_UID (insn));
		  fprintf (gcse_file, " with reg %d\n", REGNO (src));
		}

	      /* The original insn setting reg_used may or may not now be
		 deletable.  We leave the deletion to flow.  */
	      /* FIXME: If it turns out that the insn isn't deletable,
		 then we may have unnecessarily extended register lifetimes
		 and made things worse.  */
	    }
	}
    }

  return changed;
}

/* Like find_used_regs, but avoid recording uses that appear in
   input-output contexts such as zero_extract or pre_dec.  This
   restricts the cases we consider to those for which local cprop
   can legitimately make replacements.  */

static void
local_cprop_find_used_regs (xptr, data)
     rtx *xptr;
     void *data;
{
  rtx x = *xptr;

  if (x == 0)
    return;

  switch (GET_CODE (x))
    {
    case ZERO_EXTRACT:
    case SIGN_EXTRACT:
    case STRICT_LOW_PART:
      return;

    case PRE_DEC:
    case PRE_INC:
    case POST_DEC:
    case POST_INC:
    case PRE_MODIFY:
    case POST_MODIFY:
      /* Can only legitimately appear this early in the context of
	 stack pushes for function arguments, but handle all of the
	 codes nonetheless.  */
      return;

    case SUBREG:
      /* Setting a subreg of a register larger than word_mode leaves
	 the non-written words unchanged.  */
      if (GET_MODE_BITSIZE (GET_MODE (SUBREG_REG (x))) > BITS_PER_WORD)
	return;
      break;

    default:
      break;
    }

  find_used_regs (xptr, data);
}
  

/* LIBCALL_SP is a zero-terminated array of insns at the end of a libcall;
   their REG_EQUAL notes need updating.  */
static bool
do_local_cprop (x, insn, alter_jumps, run_jump_opt_after_gcse, libcall_sp)
     rtx x;
     rtx insn;
     int alter_jumps;
     int *run_jump_opt_after_gcse;
     rtx *libcall_sp;
{
  rtx newreg = NULL, newcnst = NULL;

  /* Rule out USE instructions and ASM statements as we don't want to change the hard registers mentioned.  */
  if (GET_CODE (x) == REG
      && (REGNO (x) >= FIRST_PSEUDO_REGISTER
          || (GET_CODE (PATTERN (insn)) != USE && asm_noperands (PATTERN (insn)) < 0)))
    {
      cselib_val *val = cselib_lookup (x, GET_MODE (x), 0);
      struct elt_loc_list *l;

      if (!val)
	return false;
      for (l = val->locs; l; l = l->next)
	{
	  rtx this_rtx = l->loc;
	  rtx note;

	  if (CONSTANT_P (this_rtx)
	      && GET_CODE (this_rtx) != CONSTANT_P_RTX)
	    newcnst = this_rtx;
	  if (REG_P (this_rtx) && REGNO (this_rtx) >= FIRST_PSEUDO_REGISTER
	      /* Don't copy propagate if it has attached REG_EQUIV note.
		 At this point this only function parameters should have
		 REG_EQUIV notes and if the argument slot is used somewhere
		 explicitly, it means address of parameter has been taken,
		 so we should not extend the lifetime of the pseudo.  */
	      && (!(note = find_reg_note (l->setting_insn, REG_EQUIV, NULL_RTX))
		  || GET_CODE (XEXP (note, 0)) != MEM))
	    newreg = this_rtx;
	}
      if (newcnst && constprop_register (insn, x, newcnst, alter_jumps, run_jump_opt_after_gcse))
	{
	  /* If we find a case where we can't fix the retval REG_EQUAL notes
	     match the new register, we either have to abandon this replacement
	     or fix delete_trivially_dead_insns to preserve the setting insn,
	     or make it delete the REG_EUAQL note, and fix up all passes that
	     require the REG_EQUAL note there.  */
	  if (!adjust_libcall_notes (x, newcnst, insn, libcall_sp))
	    abort ();
	  if (gcse_file != NULL)
	    {
	      fprintf (gcse_file, "LOCAL CONST-PROP: Replacing reg %d in ",
		       REGNO (x));
	      fprintf (gcse_file, "insn %d with constant ",
		       INSN_UID (insn));
	      print_rtl (gcse_file, newcnst);
	      fprintf (gcse_file, "\n");
	    }
	  const_prop_count++;
	  return true;
	}
      else if (newreg && newreg != x && try_replace_reg (x, newreg, insn))
	{
	  adjust_libcall_notes (x, newreg, insn, libcall_sp);
	  if (gcse_file != NULL)
	    {
	      fprintf (gcse_file,
		       "LOCAL COPY-PROP: Replacing reg %d in insn %d",
		       REGNO (x), INSN_UID (insn));
	      fprintf (gcse_file, " with reg %d\n", REGNO (newreg));
	    }
	  copy_prop_count++;
	  return true;
	}
    }
  return false;
}

/* LIBCALL_SP is a zero-terminated array of insns at the end of a libcall;
   their REG_EQUAL notes need updating to reflect that OLDREG has been
   replaced with NEWVAL in INSN.  Return true if all substitutions could
   be made.  */
static bool
adjust_libcall_notes (oldreg, newval, insn, libcall_sp)
     rtx oldreg, newval, insn, *libcall_sp;
{
  rtx end;

  while ((end = *libcall_sp++))
    {
      rtx note = find_reg_equal_equiv_note (end);

      if (! note)
	continue;

      if (REG_P (newval))
	{
	  if (reg_set_between_p (newval, PREV_INSN (insn), end))
	    {
	      do
		{
		  note = find_reg_equal_equiv_note (end);
		  if (! note)
		    continue;
		  if (reg_mentioned_p (newval, XEXP (note, 0)))
		    return false;
		}
	      while ((end = *libcall_sp++));
	      return true;
	    }
	}
      XEXP (note, 0) = replace_rtx (XEXP (note, 0), oldreg, newval);
      insn = end;
    }
  return true;
}

#define MAX_NESTED_LIBCALLS 9

static void
local_cprop_pass (alter_jumps, run_jump_opt_after_gcse, cprop_data)
     int alter_jumps;
     int *run_jump_opt_after_gcse;
     struct cprop_global *cprop_data;
{
  rtx insn;
  struct reg_use *reg_used;
  rtx libcall_stack[MAX_NESTED_LIBCALLS + 1], *libcall_sp;
  bool changed = false;

  cselib_init ();
  libcall_sp = &libcall_stack[MAX_NESTED_LIBCALLS];
  *libcall_sp = 0;
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      if (INSN_P (insn))
	{
	  rtx note = find_reg_note (insn, REG_LIBCALL, NULL_RTX);

	  if (note)
	    {
	      if (libcall_sp == libcall_stack)
		abort ();
	      *--libcall_sp = XEXP (note, 0);
	    }
	  note = find_reg_note (insn, REG_RETVAL, NULL_RTX);
	  if (note)
	    libcall_sp++;
	  note = find_reg_equal_equiv_note (insn);
	  do
	    {
	      cprop_data->reg_use_count = 0;
	      note_uses (&PATTERN (insn), local_cprop_find_used_regs, cprop_data);
	      if (note)
		local_cprop_find_used_regs (&XEXP (note, 0), cprop_data);

	      for (reg_used = &cprop_data->reg_use_table[0]; cprop_data->reg_use_count > 0;
		   reg_used++, cprop_data->reg_use_count--)
		if (do_local_cprop (reg_used->reg_rtx, insn, alter_jumps,
			run_jump_opt_after_gcse, libcall_sp))
		  {
		    changed = true;
		    break;
		  }
	    }
	  while (cprop_data->reg_use_count);
	}
      cselib_process_insn (insn);
    }
  /* Global analysis may get into infinite loops for unreachable blocks.  */
  if (changed && alter_jumps)
    {
      delete_unreachable_blocks ();
      free_reg_set_mem ();
      alloc_reg_set_mem (max_reg_num ());
      compute_sets (get_insns ());
    }
  cselib_finish ();
}

/* Bypass conditional jumps.  */

/* Find a set of REGNO to a constant that is available at the end of basic
   block BB.  Returns NULL if no such set is found.  Based heavily upon
   find_avail_set.  */

static struct expr *
find_bypass_set (regno, bb, cprop_data)
     int regno;
     int bb;
     struct cprop_global *cprop_data;
{
  struct expr *result = 0;

  for (;;)
    {
      rtx src;
      struct expr *set = lookup_set (regno, &cprop_data->set_hash_table);

      while (set)
	{
	  if (TEST_BIT (cprop_data->avout[bb], set->bitmap_index))
	    break;
	  set = next_set (regno, set);
	}

      if (set == 0)
	break;

      if (GET_CODE (set->expr) != SET)
	abort ();

      src = SET_SRC (set->expr);
      if (CONSTANT_P (src))
	result = set;

      if (GET_CODE (src) != REG)
	break;

      regno = REGNO (src);
    }
  return result;
}

/* The value of last_basic_block at the beginning of the jump_bypass
   pass.  The use of redirect_edge_and_branch_force may introduce new
   basic blocks, but the data flow analysis is only valid for basic
   block indices less than bypass_last_basic_block.  */

static int bypass_last_basic_block;

/* Subroutine of bypass_conditional_jumps that attempts to bypass the given
   basic block BB which has more than one predecessor.  If not NULL, SETCC
   is the first instruction of BB, which is immediately followed by JUMP_INSN
   JUMP.  Otherwise, SETCC is NULL, and JUMP is the first insn of BB.
   Returns nonzero if a change was made.  */

static int
bypass_block (bb, setcc, jump, cprop_data)
     basic_block bb;
     rtx setcc, jump;
     struct cprop_global *cprop_data;
{
  rtx insn, note;
  edge e, enext;
  int i, change;

  insn = (setcc != NULL) ? setcc : jump;

  /* Determine set of register uses in INSN.  */
  cprop_data->reg_use_count = 0;
  note_uses (&PATTERN (insn), find_used_regs, cprop_data);
  note = find_reg_equal_equiv_note (insn);
  if (note)
    find_used_regs (&XEXP (note, 0), cprop_data);

  change = 0;
  for (e = bb->pred; e; e = enext)
    {
      enext = e->pred_next;
      if (e->flags & EDGE_COMPLEX)
	continue;

      /* We can't redirect edges from new basic blocks.  */
      if (e->src->index >= bypass_last_basic_block)
	continue;

      for (i = 0; i < cprop_data->reg_use_count; i++)
	{
	  struct reg_use *reg_used = &cprop_data->reg_use_table[i];
	  unsigned int regno = REGNO (reg_used->reg_rtx);
	  basic_block dest, old_dest;
	  struct expr *set;
	  rtx src, new;

	  if (regno >= max_gcse_regno)
	    continue;

          set = find_bypass_set (regno, e->src->index, cprop_data);

	  if (! set)
	    continue;

	  src = SET_SRC (pc_set (jump));

	  if (setcc != NULL)
	      src = simplify_replace_rtx (src,
					  SET_DEST (PATTERN (setcc)),
					  SET_SRC (PATTERN (setcc)));

	  new = simplify_replace_rtx (src, reg_used->reg_rtx,
				      SET_SRC (set->expr));

	  if (new == pc_rtx)
	    dest = FALLTHRU_EDGE (bb)->dest;
	  else if (GET_CODE (new) == LABEL_REF)
	    dest = BRANCH_EDGE (bb)->dest;
	  else
	    dest = NULL;

	  old_dest = e->dest;
	  if (dest != NULL
	      && dest != old_dest
	      && dest != EXIT_BLOCK_PTR)
            {
	      redirect_edge_and_branch_force (e, dest);

	      /* Copy the register setter to the redirected edge.
		 Don't copy CC0 setters, as CC0 is dead after jump.  */
	      if (setcc)
		{
		  rtx pat = PATTERN (setcc);
		  if (!CC0_P (SET_DEST (pat)))
		    insert_insn_on_edge (copy_insn (pat), e);
		}

	      if (gcse_file != NULL)
		{
		  fprintf (gcse_file, "JUMP-BYPASS: Proved reg %d in jump_insn %d equals constant ",
			   regno, INSN_UID (jump));
		  print_rtl (gcse_file, SET_SRC (set->expr));
		  fprintf (gcse_file, "\nBypass edge from %d->%d to %d\n",
			   e->src->index, old_dest->index, dest->index);
		}
	      change = 1;
	      break;
	    }
	}
    }
  return change;
}

/* Find basic blocks with more than one predecessor that only contain a
   single conditional jump.  If the result of the comparison is known at
   compile-time from any incoming edge, redirect that edge to the
   appropriate target.  Returns nonzero if a change was made.  */

static int
bypass_conditional_jumps (cprop_data)
     struct cprop_global *cprop_data;
{
  basic_block bb;
  int changed;
  rtx setcc;
  rtx insn;
  rtx dest;

  /* Note we start at block 1.  */
  if (ENTRY_BLOCK_PTR->next_bb == EXIT_BLOCK_PTR)
    return 0;

  bypass_last_basic_block = last_basic_block;

  changed = 0;
  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR->next_bb->next_bb,
		  EXIT_BLOCK_PTR, next_bb)
    {
      /* Check for more than one predecessor.  */
      if (bb->pred && bb->pred->pred_next)
	{
	  setcc = NULL_RTX;
	  for (insn = bb->head;
	       insn != NULL && insn != NEXT_INSN (bb->end);
	       insn = NEXT_INSN (insn))
	    if (GET_CODE (insn) == INSN)
	      {
		if (setcc)
		  break;
		if (GET_CODE (PATTERN (insn)) != SET)
		  break;

		dest = SET_DEST (PATTERN (insn));
		if (REG_P (dest) || CC0_P (dest))
		  setcc = insn;
		else
		  break;
	      }
	    else if (GET_CODE (insn) == JUMP_INSN)
	      {
		if (any_condjump_p (insn) && onlyjump_p (insn))
		  changed |= bypass_block (bb, setcc, insn, cprop_data);
		break;
	      }
	    else if (INSN_P (insn))
	      break;
	}
    }

  /* If we bypassed any register setting insns, we inserted a
     copy on the redirected edge.  These need to be committed.  */
  if (changed)
    commit_edge_insertions();

  return changed;
}

/* Forward propagate copies.  This includes copies and constants.  Return
   non-zero if a change was made.  */

static int
cprop (alter_jumps, cprop_data, run_jump_opt_after_gcse)
     int alter_jumps;
     struct cprop_global *cprop_data;
     int *run_jump_opt_after_gcse;
{
  int changed;
  basic_block bb;
  rtx insn;

  /* Note we start at block 1.  */
  if (ENTRY_BLOCK_PTR->next_bb == EXIT_BLOCK_PTR)
    {
      if (gcse_file != NULL)
	fprintf (gcse_file, "\n");
      return 0;
    }

  changed = 0;
  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR->next_bb->next_bb, EXIT_BLOCK_PTR, next_bb)
    {
      /* Reset tables used to keep track of what's still valid [since the
	 start of the block].  */
      reset_opr_set_tables ();

      for (insn = bb->head;
	   insn != NULL && insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	if (INSN_P (insn))
	  {
	    changed |= cprop_insn (insn, alter_jumps, cprop_data, run_jump_opt_after_gcse);

	    /* Keep track of everything modified by this insn.  */
	    /* ??? Need to be careful w.r.t. mods done to INSN.  Don't
	       call mark_oprs_set if we turned the insn into a NOTE.  */
	    if (GET_CODE (insn) != NOTE)
	      mark_oprs_set (insn);
	  }
    }

  if (gcse_file != NULL)
    fprintf (gcse_file, "\n");

  return changed;
}

/* Perform one copy/constant propagation pass.
   PASS is the pass count.  If CPROP_JUMPS is true, perform constant
   propagation into conditional jumps.  If BYPASS_JUMPS is true,
   perform conditional jump bypassing optimizations.  */

int
one_cprop_pass (pass, alter_jumps, run_jump_opt_after_gcse, bypass_jumps)
     int pass;
     int alter_jumps;
     int *run_jump_opt_after_gcse;
     int bypass_jumps;
{
  int changed = 0;
  struct cprop_global cprop_data;

  const_prop_count = 0;
  copy_prop_count = 0;

  local_cprop_pass (alter_jumps, run_jump_opt_after_gcse, &cprop_data);

  /* Determine implicit sets.  */
  implicit_sets = (rtx *) xcalloc (last_basic_block, sizeof (rtx));
  find_implicit_sets ();

  alloc_hash_table (max_cuid, &cprop_data.set_hash_table, 1);
  compute_hash_table (&cprop_data.set_hash_table);

  /* Free implicit_sets before peak usage.  */
  free (implicit_sets);
  implicit_sets = NULL;

  if (gcse_file)
    dump_hash_table (gcse_file, "SET", &cprop_data.set_hash_table);
  if (cprop_data.set_hash_table.n_elems > 0)
    {
      alloc_cprop_mem (last_basic_block, &cprop_data);
      compute_cprop_data (&cprop_data);
      changed = cprop (alter_jumps, &cprop_data, run_jump_opt_after_gcse);
      if (alter_jumps && bypass_jumps)
	changed |= bypass_conditional_jumps (&cprop_data);
      free_cprop_mem (&cprop_data);
    }

  free_hash_table (&cprop_data.set_hash_table);

  if (gcse_file)
    {
      fprintf (gcse_file, "CPROP of %s, pass %d: %d bytes needed, ",
	       current_function_name, pass, bytes_used);
      fprintf (gcse_file, "%d const props, %d copy props\n\n",
	       const_prop_count, copy_prop_count);
    }

  /* Global analysis may get into infinite loops for unreachable blocks.  */
  if (changed && cprop_jump)
    delete_unreachable_blocks ();
  return changed;
}
