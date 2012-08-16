/* Dealing with pseudo equivalences.
   Copyright (C) 2010, 2011, 2012
   Free Software Foundation, Inc.
   Contributed by Vladimir Makarov <vmakarov@redhat.com>.

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
along with GCC; see the file COPYING3.	If not see
<http://www.gnu.org/licenses/>.	 */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "hard-reg-set.h"
#include "rtl.h"
#include "expr.h"
#include "tm_p.h"
#include "flags.h"
#include "basic-block.h"
#include "regs.h"
#include "addresses.h"
#include "insn-config.h"
#include "recog.h"
#include "tree-pass.h"
#include "target.h"
#include "params.h"
#include "df.h"
#include "ira.h"
#include "lra-int.h"

/* The size of the following arrays.  */
int lra_regno_equiv_size;

/* Element N is the list of insns that initialized reg N from its
   equivalent constant or memory slot.	*/
rtx *lra_regno_equiv_init_insns;

/* Element N is the constant value to which pseudo reg N is
   equivalent, or zero if pseudo reg N is not equivalent to a
   constant.  Fucntion set_alternative looks at this in order to
   replace pseudo reg N with the constant it stands for.  */
rtx *lra_regno_equiv_const;

/* Element N is a memory location to which pseudo reg N is equivalent,
   prior to any register elimination (such as frame pointer to stack
   pointer).  */
rtx *lra_regno_equiv_mem_loc;

/* Element N is an invariant value to which pseudo reg N is equivalent.
   eliminate_regs_in_insn uses this to replace pseudos in particular
   contexts.  */
rtx *lra_regno_equiv_invariant;

/* Initialize or resize, if necessary, the above arrays to have at
   least MAX_IND elements.  Return false, if we did nothing.  */
static bool
resize_regno_equiv (int max_ind)
{
  int i, old;
  int expanded_size;

  old = lra_regno_equiv_size;
  expanded_size = 4 * max_ind / 3 + 1;
  if (lra_regno_equiv_init_insns == NULL)
    {
      lra_regno_equiv_size = expanded_size;
      lra_assert (lra_regno_equiv_mem_loc == NULL
		  && lra_regno_equiv_const == NULL
		  && lra_regno_equiv_invariant == NULL);
      lra_regno_equiv_init_insns
	= (rtx *) xmalloc (sizeof (rtx) * lra_regno_equiv_size);
      lra_regno_equiv_mem_loc
	= (rtx *) xmalloc (sizeof (rtx) * lra_regno_equiv_size);
      lra_regno_equiv_const
	= (rtx *) xmalloc (sizeof (rtx) * lra_regno_equiv_size);
      lra_regno_equiv_invariant
	= (rtx *) xmalloc (sizeof (rtx) * lra_regno_equiv_size);
   }
  else if (old <= max_ind)
    {
      lra_regno_equiv_size = expanded_size;
      lra_regno_equiv_init_insns
	= (rtx *) xrealloc (lra_regno_equiv_init_insns,
			    sizeof (rtx) * lra_regno_equiv_size);
      lra_regno_equiv_mem_loc
	= (rtx *) xrealloc (lra_regno_equiv_mem_loc,
			    sizeof (rtx) * lra_regno_equiv_size);
      lra_regno_equiv_const
	= (rtx *) xrealloc (lra_regno_equiv_const,
			    sizeof (rtx) * lra_regno_equiv_size);
      lra_regno_equiv_invariant
	= (rtx *) xrealloc (lra_regno_equiv_invariant,
			    sizeof (rtx) * lra_regno_equiv_size);
    }

  for (i = old; i < lra_regno_equiv_size; i++)
    {
      lra_regno_equiv_init_insns[i] = NULL_RTX;
      lra_regno_equiv_mem_loc[i] = NULL_RTX;
      lra_regno_equiv_const[i] = NULL_RTX;
      lra_regno_equiv_invariant[i] = NULL_RTX;
    }
  return old < lra_regno_equiv_size;
}

/* Description of equivalences of a pseudo.  */
struct equivalence
{
  /* Set when a REG_EQUIV note is found or created.  Used to keep
     track of what memory accesses might be created later, e.g. by
     reload.  */
  rtx replacement;
  /* Location of the source of the equivalence.	 */
  rtx *src_p;
  /* The list of each insn which initializes this register.  */
  rtx init_insns;
  /* Loop depth is used to recognize equivalences which appear to be
     present within the same loop (or in an inner loop).  */
  int loop_depth;
  /* Nonzero if this had a pre-existing REG_EQUIV note.	 */
  int is_arg_equivalence;
  /* Set when an attempt should be made to replace a register with the
     associated src_p entry.  */
  char replace;
};

/* regno_equiv[N] (where N is a pseudo reg number) is the equivalence
   structure for that register.	 */
static struct equivalence *regno_equiv;

/* Used for communication between the following two functions:
   contains a MEM that we wish to ensure remains unchanged.  */
static rtx equiv_mem;

/* Set nonzero if EQUIV_MEM is modified.  */
static int equiv_mem_modified;

/* If EQUIV_MEM is modified by modifying DEST, indicate that it is
   modified.  Called via note_stores.  */
static void
validate_equiv_mem_from_store (rtx dest, const_rtx set ATTRIBUTE_UNUSED,
			       void *data ATTRIBUTE_UNUSED)
{
  if ((REG_P (dest)
       && reg_overlap_mentioned_p (dest, equiv_mem))
      || (MEM_P (dest)
	  && true_dependence (dest, VOIDmode, equiv_mem, rtx_varies_p)))
    equiv_mem_modified = 1;
}

/* Verify that no store between START and the death of REG invalidates
   MEMREF.  MEMREF is invalidated by modifying a register used in
   MEMREF, by storing into an overlapping memory location, or with a
   non-const CALL_INSN.

   Return 1 if MEMREF remains valid.  */
static int
validate_equiv_mem (rtx start, rtx reg, rtx memref)
{
  rtx insn;
  rtx note;

  equiv_mem = memref;
  equiv_mem_modified = 0;

  /* If the memory reference has side effects or is volatile, it isn't
     a valid equivalence.  */
  if (side_effects_p (memref))
    return 0;

  for (insn = start; insn && ! equiv_mem_modified; insn = NEXT_INSN (insn))
    {
      if (! INSN_P (insn))
	continue;

      if (find_reg_note (insn, REG_DEAD, reg))
	return 1;

      if (CALL_P (insn) && ! MEM_READONLY_P (memref)
	  && ! RTL_CONST_OR_PURE_CALL_P (insn))
	return 0;

      note_stores (PATTERN (insn), validate_equiv_mem_from_store, NULL);

      /* If a register mentioned in MEMREF is modified via an
	 auto-increment, we lose the equivalence.  Do the same if one
	 dies; although we could extend the life, it doesn't seem
	 worth the trouble.  */
      for (note = REG_NOTES (insn); note; note = XEXP (note, 1))
	if ((REG_NOTE_KIND (note) == REG_INC
	     || REG_NOTE_KIND (note) == REG_DEAD)
	    && REG_P (XEXP (note, 0))
	    && reg_overlap_mentioned_p (XEXP (note, 0), memref))
	  return 0;
    }

  return 0;
}

/* Returns zero if X is known to be invariant.	*/
static int
equiv_init_varies_p (rtx x)
{
  RTX_CODE code = GET_CODE (x);
  int i;
  const char *fmt;

  switch (code)
    {
    case MEM:
      return !MEM_READONLY_P (x) || equiv_init_varies_p (XEXP (x, 0));

    case CONST:
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_FIXED:
    case CONST_VECTOR:
    case SYMBOL_REF:
    case LABEL_REF:
      return 0;

    case REG:
      return regno_equiv[REGNO (x)].replace == 0 && rtx_varies_p (x, 0);

    case ASM_OPERANDS:
      if (MEM_VOLATILE_P (x))
	return 1;

      /* Fall through.	*/
    default:
      break;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    if (fmt[i] == 'e')
      {
	if (equiv_init_varies_p (XEXP (x, i)))
	  return 1;
      }
    else if (fmt[i] == 'E')
      {
	int j;
	for (j = 0; j < XVECLEN (x, i); j++)
	  if (equiv_init_varies_p (XVECEXP (x, i, j)))
	    return 1;
      }

  return 0;
}

/* Returns nonzero if X (used to initialize register REGNO) is
   movable.  X is only movable if the registers it uses have
   equivalent initializations which appear to be within the same loop
   (or in an inner loop) and movable or if they don't vary.  */
static int
equiv_init_movable_p (rtx x, int regno)
{
  int i, j;
  const char *fmt;
  enum rtx_code code = GET_CODE (x);

  switch (code)
    {
    case SET:
      return equiv_init_movable_p (SET_SRC (x), regno);

    case CC0:
    case CLOBBER:
      return 0;

    case PRE_INC:
    case PRE_DEC:
    case POST_INC:
    case POST_DEC:
    case PRE_MODIFY:
    case POST_MODIFY:
      return 0;

    case REG:
      return ((regno_equiv[REGNO (x)].loop_depth
	       >= regno_equiv[regno].loop_depth
	       && regno_equiv[REGNO (x)].replace)
	      || (REG_BASIC_BLOCK (REGNO (x)) < NUM_FIXED_BLOCKS
		  && ! rtx_varies_p (x, 0)));

    case UNSPEC_VOLATILE:
      return 0;

    case ASM_OPERANDS:
      if (MEM_VOLATILE_P (x))
	return 0;

      /* Fall through.	*/

    default:
      break;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    switch (fmt[i])
      {
      case 'e':
	if (! equiv_init_movable_p (XEXP (x, i), regno))
	  return 0;
	break;
      case 'E':
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  if (! equiv_init_movable_p (XVECEXP (x, i, j), regno))
	    return 0;
	break;
      }

  return 1;
}

/* TRUE if X uses any registers for which regno_equiv[REGNO].replace
   is true.  */
static int
contains_replace_regs (rtx x)
{
  int i, j;
  const char *fmt;
  enum rtx_code code = GET_CODE (x);

  switch (code)
    {
    case CONST_INT:
    case CONST:
    case LABEL_REF:
    case SYMBOL_REF:
    case CONST_DOUBLE:
    case CONST_FIXED:
    case CONST_VECTOR:
    case PC:
    case CC0:
    case HIGH:
      return 0;

    case REG:
      return regno_equiv[REGNO (x)].replace;

    default:
      break;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    switch (fmt[i])
      {
      case 'e':
	if (contains_replace_regs (XEXP (x, i)))
	  return 1;
	break;
      case 'E':
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  if (contains_replace_regs (XVECEXP (x, i, j)))
	    return 1;
	break;
      }

  return 0;
}

/* TRUE if X references a memory location that would be affected by a
   store to MEMREF.  */
static int
memref_referenced_p (rtx memref, rtx x)
{
  int i, j;
  const char *fmt;
  enum rtx_code code = GET_CODE (x);

  switch (code)
    {
    case CONST_INT:
    case CONST:
    case LABEL_REF:
    case SYMBOL_REF:
    case CONST_DOUBLE:
    case CONST_FIXED:
    case CONST_VECTOR:
    case PC:
    case CC0:
    case HIGH:
    case LO_SUM:
      return 0;

    case REG:
      return (regno_equiv[REGNO (x)].replacement
	      && memref_referenced_p (memref,
				      regno_equiv[REGNO (x)].replacement));

    case MEM:
      if (true_dependence (memref, VOIDmode, x, rtx_varies_p))
	return 1;
      break;

    case SET:
      /* If we are setting a MEM, it doesn't count (its address does),
	 but any other SET_DEST that has a MEM in it is referencing
	 the MEM.  */
      if (MEM_P (SET_DEST (x)))
	{
	  if (memref_referenced_p (memref, XEXP (SET_DEST (x), 0)))
	    return 1;
	}
      else if (memref_referenced_p (memref, SET_DEST (x)))
	return 1;

      return memref_referenced_p (memref, SET_SRC (x));

    default:
      break;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    switch (fmt[i])
      {
      case 'e':
	if (memref_referenced_p (memref, XEXP (x, i)))
	  return 1;
	break;
      case 'E':
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  if (memref_referenced_p (memref, XVECEXP (x, i, j)))
	    return 1;
	break;
      }

  return 0;
}

/* TRUE if some insn in the range (START, END] references a memory
   location that would be affected by a store to MEMREF.  */
static int
memref_used_between_p (rtx memref, rtx start, rtx end)
{
  rtx insn;

  for (insn = NEXT_INSN (start); insn != NEXT_INSN (end);
       insn = NEXT_INSN (insn))
    {
      if (!INSN_P (insn))
	continue;
      
      if (memref_referenced_p (memref, PATTERN (insn)))
	return 1;

      /* Nonconst functions may access memory.	*/
      if (CALL_P (insn) && (! RTL_CONST_CALL_P (insn)))
	return 1;
    }

  return 0;
}

/* Mark REG as having no known equivalence.  Some instructions might
   have been processed before and furnished with REG_EQUIV notes for
   this register; these notes will have to be removed.	STORE is the
   piece of RTL that does the non-constant / conflicting assignment -
   a SET, CLOBBER or REG_INC note.  It is currently not used, but
   needs to be there because this function is called from
   note_stores.	 */
static void
no_equiv (rtx reg, const_rtx store ATTRIBUTE_UNUSED,
	  void *data ATTRIBUTE_UNUSED)
{
  int regno;
  rtx list;

  if (!REG_P (reg))
    return;
  regno = REGNO (reg);
  list = regno_equiv[regno].init_insns;
  if (list == const0_rtx)
    return;
  regno_equiv[regno].init_insns = const0_rtx;
  regno_equiv[regno].replacement = NULL_RTX;
  /* This doesn't matter for equivalences made for argument registers,
     we should keep their initialization insns.	 */
  if (regno_equiv[regno].is_arg_equivalence)
    return;
  lra_regno_equiv_init_insns[regno] = NULL_RTX;
  for (; list; list =  XEXP (list, 1))
    {
      rtx insn = XEXP (list, 0);
      remove_note (insn, find_reg_note (insn, REG_EQUIV, NULL_RTX));
    }
}

/* Nonzero if we recorded an equivalence for a LABEL_REF.  */
static int recorded_label_ref;

/* Find registers that are equivalent to a single value throughout the
   compilation (either because they can be referenced in memory or are
   set once from a single constant).  Lower their priority for a
   register.

   If such a register is only referenced once, try substituting its
   value into the using insn.  If it succeeds, we can eliminate the
   register completely.

   Initialize the LRA_REGNO_EQUIV_INIT_INSNS array of initializing insns.

   Return non-zero if jump label rebuilding should be done.  */
static int
update_equiv_regs (void)
{
  rtx insn;
  basic_block bb;
  int loop_depth;
  bitmap cleared_regs;
  
  /* We need to keep track of whether or not we recorded a LABEL_REF
     so that we know if the jump optimizer needs to be rerun.  */
  recorded_label_ref = 0;

  regno_equiv = XCNEWVEC (struct equivalence, max_regno);

  init_alias_analysis ();

  /* Scan the insns and find which registers have equivalences.	 Do
     this in a separate scan of the insns because (due to
     -fcse-follow-jumps) a register can be set below its use.  */
  FOR_EACH_BB (bb)
    {
      loop_depth = bb->loop_depth;

      for (insn = BB_HEAD (bb);
	   insn != NEXT_INSN (BB_END (bb));
	   insn = NEXT_INSN (insn))
	{
	  rtx note;
	  rtx set;
	  rtx dest, src;
	  int regno;

	  if (! INSN_P (insn))
	    continue;

	  for (note = REG_NOTES (insn); note; note = XEXP (note, 1))
	    if (REG_NOTE_KIND (note) == REG_INC)
	      no_equiv (XEXP (note, 0), note, NULL);

	  set = single_set (insn);

	  /* If this insn contains more (or less) than a single SET,
	     only mark all destinations as having no known
	     equivalence.  */
	  if (set == 0)
	    {
	      note_stores (PATTERN (insn), no_equiv, NULL);
	      continue;
	    }
	  else if (GET_CODE (PATTERN (insn)) == PARALLEL)
	    {
	      int i;

	      for (i = XVECLEN (PATTERN (insn), 0) - 1; i >= 0; i--)
		{
		  rtx part = XVECEXP (PATTERN (insn), 0, i);
		  if (part != set)
		    note_stores (part, no_equiv, NULL);
		}
	    }

	  dest = SET_DEST (set);
	  src = SET_SRC (set);

	  /* See if this is setting up the equivalence between an
	     argument register and its stack slot.  */
	  note = find_reg_note (insn, REG_EQUIV, NULL_RTX);
	  if (note)
	    {
	      lra_assert (REG_P (dest));
	      regno = REGNO (dest);

	      /* Note that we don't want to clear
		 lra_regno_equiv_init_insns even if there are multiple
		 sets of this register.	 */
	      regno_equiv[regno].is_arg_equivalence = 1;

	      /* Record that this is an equivalencing insn.  */
	      if (rtx_equal_p (src, XEXP (note, 0)))
		lra_regno_equiv_init_insns[regno]
		  = gen_rtx_INSN_LIST (VOIDmode, insn,
				       lra_regno_equiv_init_insns[regno]);

	      /* Continue normally in case this is a candidate for
		 replacements.	*/
	    }

	  if (!optimize)
	    continue;

	  /* We only handle the case of a pseudo register being set
	     once, or always to the same value.	 */
	  if (! REG_P (dest)
	      || (regno = REGNO (dest)) < FIRST_PSEUDO_REGISTER
	      || regno_equiv[regno].init_insns == const0_rtx)
	    {
	      /* This might be setting a SUBREG of a pseudo, a pseudo
		 that is also set somewhere else to a constant.	 */
	      note_stores (set, no_equiv, NULL);
	      continue;
	    }

	  note = find_reg_note (insn, REG_EQUAL, NULL_RTX);

	  /* cse sometimes generates function invariants, but doesn't
	     put a REG_EQUAL note on the insn.	Since this note would
	     be redundant, there's no point creating it earlier than
	     here.  */
	  if (! note && ! rtx_varies_p (src, 0))
	    note = set_unique_reg_note (insn, REG_EQUAL, copy_rtx (src));

	  /* Don't bother considering a REG_EQUAL note containing an
	     EXPR_LIST since it represents a function call */
	  if (note && GET_CODE (XEXP (note, 0)) == EXPR_LIST)
	    note = NULL_RTX;

	  if (DF_REG_DEF_COUNT (regno) != 1
	      && (! note
		  || rtx_varies_p (XEXP (note, 0), 0)
		  || (regno_equiv[regno].replacement
		      && ! rtx_equal_p (XEXP (note, 0),
					regno_equiv[regno].replacement))))
	    {
	      no_equiv (dest, set, NULL);
	      continue;
	    }
	  /* Record this insn as initializing this register.  */
	  regno_equiv[regno].init_insns
	    = gen_rtx_INSN_LIST (VOIDmode, insn,
				 regno_equiv[regno].init_insns);

	  /* If this register is known to be equal to a constant,
	     record that it is always equivalent to the constant.  */
	  if (DF_REG_DEF_COUNT (regno) == 1
	      && note && ! rtx_varies_p (XEXP (note, 0), 0))
	    {
	      rtx note_value = XEXP (note, 0);
	      remove_note (insn, note);
	      set_unique_reg_note (insn, REG_EQUIV, note_value);
	    }

	  /* If this insn introduces a "constant" register, decrease
	     the priority of that register.  Record this insn if the
	     register is only used once more and the equivalence value
	     is the same as our source.

	     The latter condition is checked for two reasons: First,
	     it is an indication that it may be more efficient to
	     actually emit the insn as written (if no registers are
	     available, we will substitute the equivalence).
	     Secondly, it avoids problems with any registers dying in
	     this insn whose death notes would be missed.

	     If we don't have a REG_EQUIV note, see if this insn is
	     loading a register used only in one basic block from a
	     MEM.  If so, and the MEM remains unchanged for the life
	     of the register, add a REG_EQUIV note.  */

	  note = find_reg_note (insn, REG_EQUIV, NULL_RTX);

	  if (note == 0 && REG_BASIC_BLOCK (regno) >= NUM_FIXED_BLOCKS
	      && MEM_P (SET_SRC (set))
	      && validate_equiv_mem (insn, dest, SET_SRC (set)))
	    note = set_unique_reg_note (insn, REG_EQUIV,
					copy_rtx (SET_SRC (set)));

	  if (note)
	    {
	      int regno = REGNO (dest);
	      rtx x = XEXP (note, 0);

	      /* If we haven't done so, record that this is an
		 equivalencing insn.  */
	      if (!regno_equiv[regno].is_arg_equivalence)
		lra_regno_equiv_init_insns[regno]
		  = gen_rtx_INSN_LIST (VOIDmode, insn,
				       lra_regno_equiv_init_insns[regno]);

	      /* Record whether or not we created a REG_EQUIV note for
		 a LABEL_REF.  We might end up substituting the
		 LABEL_REF for uses of the pseudo here or later.  That
		 kind of transformation may turn an indirect jump into
		 a direct jump, in which case we must rerun the jump
		 optimizer to ensure that the JUMP_LABEL fields are
		 valid.	 */
	      if (GET_CODE (x) == LABEL_REF
		  || (GET_CODE (x) == CONST
		      && GET_CODE (XEXP (x, 0)) == PLUS
		      && (GET_CODE (XEXP (XEXP (x, 0), 0)) == LABEL_REF)))
		recorded_label_ref = 1;

	      regno_equiv[regno].replacement = x;
	      regno_equiv[regno].src_p = &SET_SRC (set);
	      regno_equiv[regno].loop_depth = loop_depth;

	      /* Don't mess with things live during setjmp.  */
	      if (REG_LIVE_LENGTH (regno) >= 0 && optimize)
		{
		  REG_LIVE_LENGTH (regno) *= 2;

		  /* If the register is referenced exactly twice,
		     meaning it is set once and used once, indicate
		     that the reference may be replaced by the
		     equivalence we computed above.  Do this even if
		     the register is only used in one block so that
		     dependencies can be handled where the last
		     register is used in a different block (i.e. HIGH
		     / LO_SUM sequences) and to reduce the number of
		     registers alive across calls.  */
		  if (REG_N_REFS (regno) == 2
		      && (rtx_equal_p (x, src)
			  || ! equiv_init_varies_p (src))
		      && NONJUMP_INSN_P (insn)
		      && equiv_init_movable_p (PATTERN (insn), regno))
		    regno_equiv[regno].replace = 1;
		}
	    }
	}
    }

  if (!optimize)
    goto out;

  /* A second pass, to gather additional equivalences with memory.
     This needs to be done after we know which registers we are going
     to replace.  */

  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      rtx set, src, dest;
      unsigned regno;

      if (! INSN_P (insn))
	continue;

      set = single_set (insn);
      if (! set)
	continue;

      dest = SET_DEST (set);
      src = SET_SRC (set);

      /* If this sets a MEM to the contents of a REG that is only used
	 in a single basic block, see if the register is always
	 equivalent to that memory location and if moving the store
	 from INSN to the insn that set REG is safe.  If so, put a
	 REG_EQUIV note on the initializing insn.

	 Don't add a REG_EQUIV note if the insn already has one.  The
	 existing REG_EQUIV is likely more useful than the one we are
	 adding.

	 If one of the regs in the address has
	 regno_equiv[REGNO].replace set, then we can't add this
	 REG_EQUIV note.  The regno_equiv[REGNO].replace optimization
	 may move the set of this register immediately before insn,
	 which puts it after regno_equiv[REGNO].init_insns, and hence
	 the mention in the REG_EQUIV note would be to an
	 uninitialized pseudo.	*/

      if (MEM_P (dest) && REG_P (src)
	  && (regno = REGNO (src)) >= FIRST_PSEUDO_REGISTER
	  && REG_BASIC_BLOCK (regno) >= NUM_FIXED_BLOCKS
	  && DF_REG_DEF_COUNT (regno) == 1
	  && regno_equiv[regno].init_insns != 0
	  && regno_equiv[regno].init_insns != const0_rtx
	  && ! find_reg_note (XEXP (regno_equiv[regno].init_insns, 0),
			      REG_EQUIV, NULL_RTX)
	  && ! contains_replace_regs (XEXP (dest, 0)))
	{
	  rtx init_insn = XEXP (regno_equiv[regno].init_insns, 0);
	  if (validate_equiv_mem (init_insn, src, dest)
	      && ! memref_used_between_p (dest, init_insn, insn)
	      /* Attaching a REG_EQUIV note will fail if INIT_INSN has
		 multiple sets.	 */
	      && set_unique_reg_note (init_insn, REG_EQUIV, copy_rtx (dest)))
	    {
	      /* This insn makes the equivalence, not the one
		 initializing the register.  */
	      lra_regno_equiv_init_insns[regno]
		= gen_rtx_INSN_LIST (VOIDmode, insn, NULL_RTX);
	      df_notes_rescan (init_insn);
	    }
	}
    }

  cleared_regs = BITMAP_ALLOC (NULL);
  /* Now scan all regs killed in an insn to see if any of them are
     registers only used that once.  If so, see if we can replace the
     reference with the equivalent form.  If we can, delete the
     initializing reference and this register will go away.  If we
     can't replace the reference, and the initializing reference is
     within the same loop (or in an inner loop), then move the
     register initialization just before the use, so that they are in
     the same basic block.  */
  FOR_EACH_BB_REVERSE (bb)
  {
    loop_depth = bb->loop_depth;
    for (insn = BB_END (bb);
	 insn != PREV_INSN (BB_HEAD (bb));
	 insn = PREV_INSN (insn))
      {
	rtx link;

	if (! INSN_P (insn))
	  continue;

	/* Don't substitute into a non-local goto, this confuses
	   CFG.	 */
	if (JUMP_P (insn)
	    && find_reg_note (insn, REG_NON_LOCAL_GOTO, NULL_RTX))
	  continue;

	for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	  {
	    if (REG_NOTE_KIND (link) == REG_DEAD
		/* Make sure this insn still refers to the
		   register.  */
		&& reg_mentioned_p (XEXP (link, 0), PATTERN (insn)))
	      {
		int regno = REGNO (XEXP (link, 0));
		rtx equiv_insn;

		if (! regno_equiv[regno].replace
		    || regno_equiv[regno].loop_depth < loop_depth)
		  continue;

		/* regno_equiv[REGNO].replace gets set only when
		   REG_N_REFS[REGNO] is 2, i.e. the register is set
		   once and used once.	(If it were only set, but
		   not used, flow would have deleted the setting
		   insns.)  Hence there can only be one insn in
		   regno_equiv[REGNO].init_insns.  */
		lra_assert (regno_equiv[regno].init_insns
			    && !XEXP (regno_equiv[regno].init_insns, 1));
		equiv_insn = XEXP (regno_equiv[regno].init_insns, 0);

		/* We may not move instructions that can throw,
		   since that changes basic block boundaries and we
		   are not prepared to adjust the CFG to match.	 */
		if (can_throw_internal (equiv_insn))
		  continue;

		if (asm_noperands (PATTERN (equiv_insn)) < 0
		    && validate_replace_rtx (regno_reg_rtx[regno],
					     *(regno_equiv[regno].src_p),
					     insn))
		  {
		    rtx equiv_link;
		    rtx last_link;
		    rtx note;

		    /* Find the last note.  */
		    for (last_link = link; XEXP (last_link, 1);
			 last_link = XEXP (last_link, 1))
		      ;

		    /* Append the REG_DEAD notes from equiv_insn.  */
		    equiv_link = REG_NOTES (equiv_insn);
		    while (equiv_link)
		      {
			note = equiv_link;
			equiv_link = XEXP (equiv_link, 1);
			if (REG_NOTE_KIND (note) == REG_DEAD)
			  {
			    remove_note (equiv_insn, note);
			    XEXP (last_link, 1) = note;
			    XEXP (note, 1) = NULL_RTX;
			    last_link = note;
			  }
		      }
		    
		    remove_death (regno, insn);
		    SET_REG_N_REFS (regno, 0);
		    REG_FREQ (regno) = 0;
		    delete_insn (equiv_insn);
		    
		    regno_equiv[regno].init_insns
		      = XEXP (regno_equiv[regno].init_insns, 1);
		    
		    lra_regno_equiv_init_insns[regno] = NULL_RTX;
		    bitmap_set_bit (cleared_regs, regno);
		  }
		/* Move the initialization of the register to just
		   before INSN.	 Update the flow information.  */
		else if (prev_nondebug_insn (insn) != equiv_insn)
		  {
		    rtx new_insn;
		    
		    new_insn = emit_insn_before (PATTERN (equiv_insn), insn);
		    REG_NOTES (new_insn) = REG_NOTES (equiv_insn);
		    REG_NOTES (equiv_insn) = 0;
		    /* Rescan it to process the notes.	*/
		    df_insn_rescan (new_insn);
		    
		    /* Make sure this insn is recognized before the
		       rest of LRA begins, otherwise
		       eliminate_regs_in_insn will die.	 */
		    INSN_CODE (new_insn) = INSN_CODE (equiv_insn);
		    
		    delete_insn (equiv_insn);

		    XEXP (regno_equiv[regno].init_insns, 0) = new_insn;
		    
		    REG_BASIC_BLOCK (regno) = bb->index;
		    REG_N_CALLS_CROSSED (regno) = 0;
		    REG_FREQ_CALLS_CROSSED (regno) = 0;
		    REG_N_THROWING_CALLS_CROSSED (regno) = 0;
		    REG_LIVE_LENGTH (regno) = 2;
		    
		    if (insn == BB_HEAD (bb))
		      BB_HEAD (bb) = PREV_INSN (insn);
		    
		    lra_regno_equiv_init_insns[regno]
		      = gen_rtx_INSN_LIST (VOIDmode, new_insn, NULL_RTX);
		    bitmap_set_bit (cleared_regs, regno);
		  }
	      }
	  }
      }
  }

  if (!bitmap_empty_p (cleared_regs))
    FOR_EACH_BB (bb)
      {
	bitmap_and_compl_into (DF_LIVE_IN (bb), cleared_regs);
	bitmap_and_compl_into (DF_LIVE_OUT (bb), cleared_regs);
	bitmap_and_compl_into (DF_LR_IN (bb), cleared_regs);
	bitmap_and_compl_into (DF_LR_OUT (bb), cleared_regs);
      }

  BITMAP_FREE (cleared_regs);

  out:
  /* Clean up.	*/

  end_alias_analysis ();
  free (regno_equiv);
  return recorded_label_ref;
}



/* Set up all reg equivalences.	 */
static void
setup_reg_equivs (void)
{
  int i;
  basic_block bb;
  rtx insn;

  FOR_EACH_BB (bb)
    FOR_BB_INSNS (bb, insn)
      {
	rtx set = single_set (insn);
	
	if (set != 0 && REG_P (SET_DEST (set)))
	  {
	    rtx note = find_reg_note (insn, REG_EQUIV, NULL_RTX);
	    rtx x;
	    
	    if (! note)
	      continue;
	    
	    i = REGNO (SET_DEST (set));
	    x = XEXP (note, 0);
	    
	    if (i <= LAST_VIRTUAL_REGISTER)
	      continue;
	    
	    if (! function_invariant_p (x)
		|| ! flag_pic
		/* A function invariant is often CONSTANT_P but may
		   include a register.	We promise to only pass
		   CONSTANT_P objects to LEGITIMATE_PIC_OPERAND_P.  */
	      || (CONSTANT_P (x) && LEGITIMATE_PIC_OPERAND_P (x)))
	      {
		/* It can happen that a REG_EQUIV note contains a MEM
		   that is not a legitimate memory operand.  As later
		   stages of reload assume that all addresses found in
		   the lra_regno_equiv_* arrays were originally
		   legitimate, we ignore such REG_EQUIV notes.	*/
		if (memory_operand (x, VOIDmode))
		  lra_regno_equiv_mem_loc[i] = x;
		else if (function_invariant_p (x))
		  {
		    if (GET_CODE (x) == PLUS
			|| x == frame_pointer_rtx || x == arg_pointer_rtx)
		      /* This is PLUS of frame pointer and a constant,
			 or fp, or argp.  */
		      lra_regno_equiv_invariant[i] = x;
		    else if (targetm.legitimate_constant_p
			     (GET_MODE (SET_DEST (set)), x))
		      lra_regno_equiv_const[i] = x;
		    else
		      lra_regno_equiv_mem_loc[i]
			= force_const_mem (GET_MODE (SET_DEST (set)), x);
		  }
	      }
	  }
      }
}

/* Entry function to set up all reg equivalences.  */
void
lra_init_reg_equivs (void)
{
  int i;

  max_regno = max_reg_num ();
  lra_regno_equiv_const = lra_regno_equiv_mem_loc
    = lra_regno_equiv_invariant = lra_regno_equiv_init_insns = NULL;
  lra_regno_equiv_size = 0;
  resize_regno_equiv (max_regno);
  update_equiv_regs ();
  setup_reg_equivs ();
  if (dump_file)
    for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
      if (lra_regno_equiv_init_insns[i])
	{
	  fprintf (dump_file, "init_insns for %u: ", i);
	  print_inline_rtx (dump_file, lra_regno_equiv_init_insns[i], 20);
	  fprintf (dump_file, "\n");
	}
}

/* Set up that REGNO has equivalence with MEM_LOC, CONSTANT, or
   INVARIANT and its initialization insns are INIT_INSNS.  */
void
lra_set_regno_equiv (int regno, rtx init_insns, rtx mem_loc,
		     rtx constant, rtx invariant)
{
  resize_regno_equiv (regno);
  lra_regno_equiv_init_insns[regno] = init_insns;
  lra_regno_equiv_mem_loc[regno] = mem_loc;
  lra_regno_equiv_const[regno] = constant;
  lra_regno_equiv_invariant[regno] = invariant;
}

/* Finish all data about pseudo equivalences.  */
void
lra_finish_reg_equivs (void)
{
  /* Indicate that we no longer have known memory locations,
     constants, invariants, or initializing insns.  */
  free (lra_regno_equiv_invariant);
  free (lra_regno_equiv_const);
  free (lra_regno_equiv_mem_loc);
  free (lra_regno_equiv_init_insns);
}
