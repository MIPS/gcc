/* Decompose multiword subregs.
   Contributed by Richard Henderson.
   Copyright (C) 2005, 2006 Free Software Foundation, Inc.

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
#include "machmode.h"
#include "tm.h"
#include "rtl.h"
#include "function.h"
#include "expr.h"
#include "obstack.h"
#include "bitmap.h"
#include "tree-pass.h"
#include "timevar.h"
#include "tm_p.h"


#ifdef STACK_GROWS_DOWNWARD
# undef STACK_GROWS_DOWNWARD
# define STACK_GROWS_DOWNWARD 1
#else
# define STACK_GROWS_DOWNWARD 0
#endif


DEF_VEC_P(bitmap);
DEF_VEC_ALLOC_P(bitmap,heap);

/* Bit N set if regno N is used in a context in which we can decompose it.  */
static bitmap decomposable_context;

/* Bit N set if regno N is used in a context in which it cannot
   be decomposed.  */
static bitmap non_decomposable_context;

/* Bit N in element M set if there exists a copy from reg M to reg N.  */
static VEC(bitmap,heap) *reg_copy_graph;


/* Return true if INSN is a single set between two objects.  Such insns
   can always be decomposed.  */

static rtx
simple_move (rtx insn)
{
  rtx x, set = single_set (insn);

  if (!set)
    return NULL;

  x = SET_DEST (set);
  if (!OBJECT_P (x) && GET_CODE (x) != SUBREG)
    return NULL;
  if (MEM_P (x) && MEM_VOLATILE_P (x))
    return NULL;

  x = SET_SRC (set);
  if (!OBJECT_P (x) && GET_CODE (x) != SUBREG)
    return NULL;
  if (MEM_P (x) && MEM_VOLATILE_P (x))
    return NULL;

  return set;
}

/*  */

static void
find_pseudo_copy (rtx set)
{
  rtx dst = SET_DEST (set);
  rtx src = SET_SRC (set);
  unsigned int rd, rs;
  bitmap b;

  if (!REG_P (dst) || !REG_P (src))
    return;

  rd = REGNO (dst);
  rs = REGNO (src);
  if (HARD_REGISTER_NUM_P (rd) || HARD_REGISTER_NUM_P (rs))
    return;

  if (GET_MODE_SIZE (GET_MODE (dst)) < UNITS_PER_WORD)
    return;

  b = VEC_index (bitmap, reg_copy_graph, rs);
  if (b == NULL)
    {
      b = BITMAP_ALLOC (NULL);
      VEC_replace (bitmap, reg_copy_graph, rs, b);
    }
  bitmap_set_bit (b, rd);
}

/*  */

static void
propagate_pseudo_copies (void)
{
  bitmap queue, propagate;

  queue = BITMAP_ALLOC (NULL);
  propagate = BITMAP_ALLOC (NULL);

  bitmap_copy (queue, decomposable_context);
  do
    {
      bitmap_iterator iter;
      unsigned int i;

      bitmap_clear (propagate);
      EXECUTE_IF_SET_IN_BITMAP (queue, 0, i, iter)
	{
	  bitmap b = VEC_index (bitmap, reg_copy_graph, i);
	  if (b)
	    bitmap_ior_and_compl_into (propagate, b, non_decomposable_context);
	}

      bitmap_and_compl (queue, propagate, decomposable_context);
      bitmap_ior_into (decomposable_context, propagate);
    }
  while (!bitmap_empty_p (queue));

  BITMAP_FREE (queue);
  BITMAP_FREE (propagate);
}

/* Called via for_each_rtx.  Examine the given expression and set bits as
   appropriate in decomposable_context and non_decomposable_context.  SM
   is the result of simple_move for the complete insn.  */

static int
find_decomposable_subregs (rtx *px, void *sm)
{
  rtx x = *px, inner;
  unsigned int inner_size, outer_size;
  unsigned int inner_words, outer_words;
  unsigned int regno;

  switch (GET_CODE (x))
    {
    case SUBREG:
      /* Ensure we're not looking at something other than a subreg of a
	 pseudo register.  One might hope these tests never fail, since
	 that would indicate someone not using simplify_gen_subreg or some
	 related interface, but that no doubt happens all too often.  */
      inner = SUBREG_REG (x);
      if (!REG_P (inner))
	break;

      regno = REGNO (inner);
      if (HARD_REGISTER_NUM_P (regno))
	return -1;

      /* Compute the number of words covered by the subreg and the reg.  */
      outer_size = GET_MODE_SIZE (GET_MODE (x));
      inner_size = GET_MODE_SIZE (GET_MODE (inner));
      outer_words = (outer_size + UNITS_PER_WORD - 1) / UNITS_PER_WORD;
      inner_words = (inner_size + UNITS_PER_WORD - 1) / UNITS_PER_WORD;

      /* If we've got a single-word subreg of a multi-word reg, then this
	 should be a candidate for decomposition.  Return -1 so that we
	 don't iterate over the inner register and decide it is used in a
	 context we can't decompose.  */
      /* ??? This doesn't allow e.g. DImode subregs of TImode values on
	 32-bit targets.  We'd need to record the way in which the pseudo
	 is used, and only decompose if all uses were with the same number
	 of pieces.  Hopefully this doesn't happen with any frequency.  */
      /* ??? This is a bald-faced assumption that the subreg is actually
	 inside an operand, and is thus replacable.  This might be false
	 if the target plays games with subregs in the patterns.  Perhaps
	 a better approach is to mirror what regrename does wrt recognizing
	 the insn, iterating over the operands, smashing the operands out
	 and iterating over the resulting pattern.  */
      if (outer_words == 1 && inner_words > 1)
	{
	  bitmap_set_bit (decomposable_context, regno);
	  return -1;
	}
      break;

    case REG:
      /* Since we see outer subregs and avoid iterating over inner registers
	 when we can handle the decomposition, that means that anywhere else
	 we come across the register must be a place we can't decompose it.
	 Avoid setting the bit for single-word pseudos to keep down the size
	 of the bitmap.  */
      regno = REGNO (x);
      if (!HARD_REGISTER_NUM_P (regno) && !sm
	  && GET_MODE_SIZE (GET_MODE (x)) > UNITS_PER_WORD)
	bitmap_set_bit (non_decomposable_context, regno);
      break;

    default:
      break;
    }

  return 0;
}

/* Decompose psuedo REGNO into word-sized components.  We smash the REG
   node in place.  This ensures that (1) something goes wrong quickly if
   we fail to find a place in which we ought to be performing some 
   replacement, and (2) the debug information inside the symbol table is
   automatically kept up to date.  */

static void
decompose_register (unsigned int regno)
{
  unsigned int words;
  rtx reg;

  reg = regno_reg_rtx[regno];
  regno_reg_rtx[regno] = NULL;

  words = GET_MODE_SIZE (GET_MODE (reg));
  words = (words + UNITS_PER_WORD - 1) / UNITS_PER_WORD;

  if (0 && words == 2)
    {
      PUT_CODE (reg, CONCAT);
      XEXP (reg, 0) = gen_reg_rtx_offset (reg, word_mode, 0);
      XEXP (reg, 1) = gen_reg_rtx_offset (reg, word_mode, UNITS_PER_WORD);
    }
  else
    {
      unsigned int i;
      rtvec v;

      if (dump_file)
	fprintf (dump_file, "; Splitting reg %u ->", REGNO (reg));

      PUT_CODE (reg, CONCATN);
      XVEC (reg, 0) = v = rtvec_alloc (words);

      for (i = 0; i < words; ++i)
	RTVEC_ELT (v, i)
	  = gen_reg_rtx_offset (reg, word_mode, i * UNITS_PER_WORD);

      if (dump_file)
	{
	  for (i = 0; i < words; ++i)
	    fprintf (dump_file, " %u", REGNO (XVECEXP (reg, 0, i)));
	  fputc ('\n', dump_file);
	}
    }
}

static inline bool
resolve_reg_p (rtx x)
{
  return GET_CODE (x) == CONCAT || GET_CODE (x) == CONCATN;
}

static bool
resolve_subreg_p (rtx x)
{
  if (GET_CODE (x) == SUBREG)
    return resolve_reg_p (SUBREG_REG (x));
  return false;
}

/* */

static int
resolve_subreg_use (rtx *px, void *data ATTRIBUTE_UNUSED)
{
  rtx x = *px;

  if (x == NULL)
    return 0;

  /* If this is a (subreg (concat)) pattern, then it must be something that
     we created via decompose_register.  */
  if (resolve_subreg_p (x))
    {
      /* This must be resolvable.  */
      *px = simplify_subreg (GET_MODE (x), SUBREG_REG (x),
			     GET_MODE (SUBREG_REG (x)), SUBREG_BYTE (x));
      gcc_assert (*px != NULL);
      return -1;
    }

  if (resolve_reg_p (x))
    return 1;

  return 0;
}

/* */

static void
move_libcall_note (rtx old_start, rtx new_start)
{
  rtx note0, note1, end;

  note0 = find_reg_note (old_start, REG_LIBCALL, NULL);
  if (note0 == NULL)
    return;

  remove_note (old_start, note0);
  end = XEXP (note0, 0);
  note1 = find_reg_note (end, REG_RETVAL, NULL);

  XEXP (note0, 1) = REG_NOTES (new_start);
  REG_NOTES (new_start) = note0;
  XEXP (note1, 0) = new_start;
}

/* */

static void
remove_retval_note (rtx insn1)
{
  rtx note, note0, insn0, note1, insn;

  note1 = find_reg_note (insn1, REG_RETVAL, NULL);
  if (note1 == NULL)
    return;

  insn0 = XEXP (note1, 0);
  note0 = find_reg_note (insn0, REG_LIBCALL, NULL);

  remove_note (insn0, note0);
  remove_note (insn1, note1);

  for (insn = insn0; insn != insn1; insn = NEXT_INSN (insn))
    while ((note = find_reg_note (insn, REG_NO_CONFLICT, NULL)))
      remove_note (insn, note);
}

/* */

static void
resolve_reg_notes (rtx insn)
{
  rtx *pnote, note;

  note = find_reg_equal_equiv_note (insn);
  if (note && for_each_rtx (&XEXP (note, 0), resolve_subreg_use, NULL))
    {
      remove_note (insn, note);
      remove_retval_note (insn);
    }

  pnote = &REG_NOTES (insn);
  while ((note = *pnote))
    {
      bool delete = false;

      switch (REG_NOTE_KIND (note))
	{
	case REG_NO_CONFLICT:
	  if (resolve_reg_p (XEXP (note, 0)))
	    delete = true;
	  break;

	default:
	  break;
	}

      if (delete)
	*pnote = XEXP (note, 1);
      else
	pnote = &XEXP (note, 1);
    }
}

/* */

static bool
cannot_decompose_p (rtx x)
{
  if (REG_P (x))
    {
      unsigned int regno = REGNO (x);
      if (HARD_REGISTER_NUM_P (regno))
	return !validate_subreg (word_mode, GET_MODE (x), x, UNITS_PER_WORD);
      else
	return bitmap_bit_p (non_decomposable_context, regno);
    }
  return false;
}

/* */

static rtx
resolve_simple_move (rtx set, rtx insn)
{
  rtx dst, src, tmp;
  bool rdp, rsp, sdp, ssp, delete;
  unsigned int i, words;
  enum machine_mode orig_mode;

  dst = SET_DEST (set);
  src = SET_SRC (set);
  orig_mode = GET_MODE (dst);

  sdp = ssp = false;
  if (GET_CODE (dst) == SUBREG && GET_MODE_SIZE (orig_mode) > UNITS_PER_WORD)
    sdp = rdp = resolve_reg_p (SUBREG_REG (dst));
  else
    rdp = resolve_reg_p (dst);

  if (GET_CODE (src) == SUBREG && GET_MODE_SIZE (orig_mode) > UNITS_PER_WORD)
    ssp = rsp = resolve_reg_p (SUBREG_REG (src));
  else
    rsp = resolve_reg_p (src);

  if (!rdp && !rsp)
    return insn;

  start_sequence ();

  delete = true;

  if (ssp)
    {
      tmp = SUBREG_REG (src);
      orig_mode = GET_MODE (tmp);
      dst = gen_reg_rtx (orig_mode);
      SUBREG_REG (src) = dst;
      src = tmp;
      delete = false;
    }
  if (!rsp && cannot_decompose_p (src))
    {
      tmp = gen_reg_rtx (orig_mode);
      emit_move_insn (tmp, src);
      src = tmp;
    }

  if (sdp)
    {
      tmp = SUBREG_REG (dst);
      orig_mode = GET_MODE (tmp);
      SUBREG_REG (dst) = gen_reg_rtx (orig_mode);
      emit_move_insn (dst, src);
      src = SUBREG_REG (dst);
      dst = tmp;
    }
  if (!rdp && cannot_decompose_p (dst))
    {
      dst = gen_reg_rtx (orig_mode);
      SET_SRC (set) = dst;
      delete = false;
    }

  words = (GET_MODE_SIZE (orig_mode) + UNITS_PER_WORD - 1) / UNITS_PER_WORD;

  if (push_operand (dst, orig_mode))
    {
      unsigned int j, jinc;

      gcc_assert (GET_MODE_SIZE (orig_mode) % UNITS_PER_WORD == 0);
      gcc_assert (GET_CODE (XEXP (dst, 0)) != PRE_MODIFY);
      gcc_assert (GET_CODE (XEXP (dst, 0)) != POST_MODIFY);

      if (WORDS_BIG_ENDIAN == STACK_GROWS_DOWNWARD)
	j = 0, jinc = 1;
      else
	j = words - 1, jinc = -1;

      for (i = 0; i < words; i++, j += jinc)
	{
	  tmp = copy_rtx (XEXP (dst, 0));
	  tmp = adjust_automodify_address_nv (dst, word_mode, tmp,
					      j * UNITS_PER_WORD);
	  emit_move_insn (tmp, simplify_subreg (word_mode, src, orig_mode,
						j * UNITS_PER_WORD));
	}
    }
  else
    {
      gcc_assert (!MEM_P (dst)
		  || GET_RTX_CLASS (GET_CODE (XEXP (dst, 0))) != RTX_AUTOINC);
      gcc_assert (!MEM_P (src)
		  || GET_RTX_CLASS (GET_CODE (XEXP (src, 0))) != RTX_AUTOINC);

      if (REG_P (dst) && !HARD_REGISTER_NUM_P (REGNO (dst)))
	emit_insn (gen_rtx_CLOBBER (VOIDmode, dst));

      for (i = 0; i < words; ++i)
	emit_move_insn (simplify_gen_subreg (word_mode, dst, orig_mode,
					     UNITS_PER_WORD * i),
			simplify_gen_subreg (word_mode, src, orig_mode,
					     UNITS_PER_WORD * i));
    }

  tmp = get_insns ();
  end_sequence ();

  emit_insn_before (tmp, insn);
  if (delete)
    {
      move_libcall_note (insn, tmp);
      remove_retval_note (insn);
      delete_insn (insn);
    }

  return tmp;
}

static void
resolve_clobber (rtx pat, rtx insn)
{
  rtx reg = XEXP (pat, 0);
  unsigned int words, i;
  enum machine_mode orig_mode;

  if (!resolve_reg_p (reg))
    return;

  orig_mode = GET_MODE (reg);
  words = GET_MODE_SIZE (orig_mode);
  words = (words + UNITS_PER_WORD - 1) / UNITS_PER_WORD;

  XEXP (pat, 0) = simplify_subreg (word_mode, reg, orig_mode, 0);
  for (i = words - 1; i > 0; --i)
    {
      pat = simplify_subreg (word_mode, reg, orig_mode, i * UNITS_PER_WORD);
      pat = gen_rtx_CLOBBER (VOIDmode, pat);
      emit_insn_after (pat, insn);
    }
}

static void
resolve_use (rtx pat, rtx insn)
{
  if (resolve_subreg_p (XEXP (pat, 0)))
    delete_insn (insn);
}

/* */

static void
decompose_multiword_subregs (void)
{
  rtx insn, set;

  decomposable_context = BITMAP_ALLOC (NULL);
  non_decomposable_context = BITMAP_ALLOC (NULL);

  {
    unsigned int max = max_reg_num ();
    reg_copy_graph = VEC_alloc (bitmap, heap, max);
    VEC_safe_grow (bitmap, heap, reg_copy_graph, max);
    memset (VEC_address (bitmap, reg_copy_graph), 0, sizeof (bitmap) * max);
  }

  for (insn = get_insns (); insn ; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      {
        if (GET_CODE (PATTERN (insn)) == CLOBBER
	    || GET_CODE (PATTERN (insn)) == USE)
	  continue;

	set = simple_move (insn);
	if (set)
         {
           /* (set cc0 reg) is a comparison instruction and cannot be
              decomposed.  Clear SET so that we recognize this fact when
              we see it in find_decomposable_subregs.  */
           if (CC0_P (SET_DEST (set)))
             set = NULL;
           else
             find_pseudo_copy (set);
         }
        for_each_rtx (&PATTERN (insn), find_decomposable_subregs, set);
      }

  bitmap_and_compl_into (decomposable_context, non_decomposable_context);
  if (!bitmap_empty_p (decomposable_context))
    {
      bitmap_iterator iter;
      unsigned int regno;

      propagate_pseudo_copies ();

      EXECUTE_IF_SET_IN_BITMAP (decomposable_context, 0, regno, iter)
	decompose_register (regno);

      for (insn = get_insns (); insn ; insn = NEXT_INSN (insn))
	{
	  rtx pat;

	  if (!INSN_P (insn))
	    continue;

	  pat = PATTERN (insn);
	  if (GET_CODE (pat) == CLOBBER)
	    resolve_clobber (pat, insn);
	  else if (GET_CODE (pat) == USE)
	    resolve_use (pat, insn);
	  else
	    {
	      set = simple_move (insn);
	      if (set)
		insn = resolve_simple_move (set, insn);
	      for_each_rtx (&PATTERN (insn), resolve_subreg_use, NULL);
	      resolve_reg_notes (insn);
	    }
	}
    }

  BITMAP_FREE (decomposable_context);
  BITMAP_FREE (non_decomposable_context);

  {
    unsigned int i;
    bitmap b;
    for (i = 0; VEC_iterate (bitmap, reg_copy_graph, i, b); ++i)
      if (b)
	BITMAP_FREE (b);
  }
  VEC_free (bitmap, heap, reg_copy_graph);
}



static bool
gate_lower_subreg (void)
{
  return flag_lower_subreg;
}

/* Run yet another register allocator.  */
static unsigned int
rest_of_handle_lower_subreg (void)
{
  int max_reg_num_before = max_reg_num ();

  decompose_multiword_subregs ();
  if (max_reg_num_before != max_reg_num ())
    reg_scan (get_insns (), max_reg_num ());
  return 0;
}

struct tree_opt_pass pass_lower_subreg =
{
  "lower_subreg",                               /* name */
  gate_lower_subreg,                            /* gate */
  rest_of_handle_lower_subreg,		        /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_LOWER_SUBREG,	                        /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'Y'                                   /* letter */
};
