/* Search an insn for pseudo regs that must be in reg_class and are not.
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   Contributed by Denis Chertykov <denisc@overta.ru>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


#undef REG_OK_STRICT

#include "config.h"
#include "system.h"

#include "machmode.h"
#include "hard-reg-set.h"
#include "rtl.h"
#include "tm_p.h"
#include "obstack.h"
#include "insn-config.h"
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "optabs.h"
#include "regs.h"
#include "basic-block.h"
#include "reload.h"
#include "recog.h"
#include "output.h"
#include "cselib.h"
#include "real.h"
#include "toplev.h"
#include "except.h"
#include "df.h"
#include "pre-reload.h"

/* Disable this because i386 port has a strange abort inside it.  */
#undef SECONDARY_MEMORY_NEEDED
#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free


#define NEW_REG_P(X) (REGNO (X) >= max_regno)

static rtx gen_pre_reload        PARAMS ((rtx, rtx, int, enum reload_type,
					  enum reg_class));
static rtx find_pre_reloads_toplev PARAMS ((rtx, int, enum reload_type,
					    int, int, rtx, int *));
static int pre_reload_immune_p   PARAMS ((rtx, rtx, struct decomposition));
static struct decomposition pre_reload_decompose PARAMS ((rtx));
static int push_pre_reload       PARAMS ((rtx, rtx, rtx *,rtx *,
					  enum reg_class, enum machine_mode,
					  enum machine_mode,
					  int, int, int, enum reload_type));
static rtx find_dummy_pre_reload PARAMS ((rtx, rtx, rtx *,rtx *, 
					  enum machine_mode, enum machine_mode,
					  enum reg_class, int, int));
static void do_input_pre_reload  PARAMS ((rtx, struct reload *,int));
static void emit_input_pre_reload_insns PARAMS ((rtx, struct reload *,
						 rtx, int));
static rtx inc_for_pre_reload    PARAMS ((rtx, rtx, rtx, int));
static void do_output_pre_reload PARAMS ((rtx, struct reload *, int));
static void emit_output_pre_reload_insns PARAMS ((rtx, struct reload *, int));
static int pseudo_clobbered_p    PARAMS ((unsigned int, rtx, enum machine_mode,
					  int));
static void pre_reload_collect   PARAMS ((struct ra_info *, bitmap));
static int pre_operands_match_p  PARAMS ((rtx, rtx));
static void collect_insn_info    PARAMS ((struct ra_info *, rtx,
					  ra_ref **, ra_ref **,
					  int *, int *));
static void debug_ra_insn_refs   PARAMS ((struct ra_info *, rtx));
static void debug_ra_reg_refs    PARAMS ((struct ra_info *, int));
static struct ra_refs * build_ra_refs_for_insn PARAMS ((struct ra_info *,
							ra_ref **, ra_ref **,
							int, int));
static inline struct ra_link * ra_link_create PARAMS ((struct ra_info *,
						       ra_ref *,
						       struct ra_link *));

static void ra_info_add_insn_refs PARAMS ((struct ra_info *, rtx,
					   struct ra_refs *));
static void ra_info_add_reg_refs  PARAMS ((struct ra_info *, rtx,
					   struct ra_refs *));
static void ra_insn_table_realloc PARAMS ((struct ra_info *ra_info, int));
static void ra_reg_table_realloc  PARAMS ((struct ra_info *ra_info, int));
static int pseudo_fits_class_p    PARAMS ((rtx, enum reg_class,
					   enum machine_mode));

struct df2ra build_df2ra          PARAMS ((struct df *, struct ra_info *));
static void ra_info_remove_refs   PARAMS ((struct ra_info *,
					   struct ra_refs *));
static int df_link2ra_link        PARAMS ((struct df2ra, rtx, struct df_link *,
					   struct ra_link *));

static const char *const reg_class_names[] = REG_CLASS_NAMES;
static int indirect_levels = -1;

/* Create a link in a def-use or use-def chain.  */
static inline struct ra_link *
ra_link_create (ra_info, ref, next)
     struct ra_info *ra_info;
     ra_ref *ref;
     struct ra_link *next;
{
  struct ra_link *link;

  link = (struct ra_link *) obstack_alloc (&ra_info->obstack,
					   sizeof (*link));
  link->next = next;
  link->ref = ref;
  return link;
}


/* Emit code to perform a reload from IN (which may be a reload register) to
   OUT (which may also be a reload register).  IN or OUT is from operand
   OPNUM with reload type TYPE.

   Returns first insn emitted.  */

static rtx
gen_pre_reload (out, in, opnum, type, class)
     rtx out;
     rtx in;
     int opnum;
     enum reload_type type;
     enum reg_class class;
{
  rtx last = get_last_insn ();

  /* If IN is a paradoxical SUBREG, remove it and try to put the
     opposite SUBREG on OUT.  Likewise for a paradoxical SUBREG on OUT.  */
#if 0
  if (GET_CODE (in) == SUBREG
      && (GET_MODE_SIZE (GET_MODE (in))
	  > GET_MODE_SIZE (GET_MODE (SUBREG_REG (in))))
      && (tem = gen_lowpart_common (GET_MODE (SUBREG_REG (in)), out)) != 0)
    in = SUBREG_REG (in), out = tem;
  else
    if (GET_CODE (out) == SUBREG
	&& (GET_MODE_SIZE (GET_MODE (out))
	    > GET_MODE_SIZE (GET_MODE (SUBREG_REG (out))))
	&& (tem = gen_lowpart_common (GET_MODE (SUBREG_REG (out)), in)) != 0)
      out = SUBREG_REG (out), in = tem;
#endif

  out = copy_rtx (out);
  in = copy_rtx (in);

  /* How to do this reload can get quite tricky.  Normally, we are being
     asked to reload a simple operand, such as a MEM, a constant, or a pseudo
     register that didn't get a hard register.  In that case we can just
     call emit_move_insn.

     We can also be asked to reload a PLUS that adds a register or a MEM to
     another register, constant or MEM.  This can occur during frame pointer
     elimination and while reloading addresses.  This case is handled by
     trying to emit a single insn to perform the add.  If it is not valid,
     we use a two insn sequence.

     Finally, we could be called to handle an 'o' constraint by putting
     an address into a register.  In that case, we first try to do this
     with a named pattern of "reload_load_address".  If no such pattern
     exists, we just emit a SET insn and hope for the best (it will normally
     be valid on machines that use 'o').

     This entire process is made complex because reload will never
     process the insns we generate here and so we must ensure that
     they will fit their constraints and also by the fact that parts of
     IN might be being reloaded separately and replaced with spill registers.
     Because of this, we are, in some sense, just guessing the right approach
     here.  The one listed above seems to work.

     ??? At some point, this whole thing needs to be rethought.  */

  if (GET_CODE (in) == PLUS
      && (REG_P (XEXP (in, 0))
	  || GET_CODE (XEXP (in, 0)) == SUBREG
	  || GET_CODE (XEXP (in, 0)) == MEM)
      && (REG_P (XEXP (in, 1))
	  || GET_CODE (XEXP (in, 1)) == SUBREG
	  || CONSTANT_P (XEXP (in, 1))
	  || GET_CODE (XEXP (in, 1)) == MEM))
    {
      /* We need to compute the sum of a register or a MEM and another
	 register, constant, or MEM, and put it into the reload
	 register.  The best possible way of doing this is if the machine
	 has a three-operand ADD insn that accepts the required operands.

	 The simplest approach is to try to generate such an insn and see if it
	 is recognized and matches its constraints.  If so, it can be used.

	 It might be better not to actually emit the insn unless it is valid,
	 but we need to pass the insn as an operand to `recog' and
	 `extract_insn' and it is simpler to emit and then delete the insn if
	 not valid than to dummy things up.  */

      rtx op0, op1, tem, insn;
      int code;

      op0 = find_replacement (&XEXP (in, 0));
      op1 = find_replacement (&XEXP (in, 1));

      /* Since constraint checking is strict, commutativity won't be
	 checked, so we need to do that here to avoid spurious failure
	 if the add instruction is two-address and the second operand
	 of the add is the same as the reload reg, which is frequently
	 the case.  If the insn would be A = B + A, rearrange it so
	 it will be A = A + B as constrain_operands expects.  */

      if (GET_CODE (XEXP (in, 1)) == REG
	  && REGNO (out) == REGNO (XEXP (in, 1)))
	tem = op0, op0 = op1, op1 = tem;

      if (op0 != XEXP (in, 0) || op1 != XEXP (in, 1))
	in = gen_rtx_PLUS (GET_MODE (in), op0, op1);

      insn = emit_insn (gen_rtx_SET (VOIDmode, out, in));
      code = recog_memoized (insn);

      if (code >= 0)
	return insn;

      delete_insns_since (last);

      /* If that failed, we must use a conservative two-insn sequence.

	 Use a move to copy one operand into the reload register.  Prefer
	 to reload a constant, MEM or pseudo since the move patterns can
	 handle an arbitrary operand.  If OP1 is not a constant, MEM or
	 pseudo and OP1 is not a valid operand for an add instruction, then
	 reload OP1.

	 After reloading one of the operands into the reload register, add
	 the reload register to the output register.

	 If there is another way to do this for a specific machine, a
	 DEFINE_PEEPHOLE should be specified that recognizes the sequence
	 we emit below.  */

      code = (int) add_optab->handlers[(int) GET_MODE (out)].insn_code;

      if (CONSTANT_P (op1) || GET_CODE (op1) == MEM || GET_CODE (op1) == SUBREG
	  || (REG_P (op1)
	      && REGNO (op1) >= FIRST_PSEUDO_REGISTER)
	  || (code != CODE_FOR_nothing
	      && ! ((*insn_data[code].operand[2].predicate)
		    (op1, insn_data[code].operand[2].mode))))
	tem = op0, op0 = op1, op1 = tem;

      gen_pre_reload (out, op0, opnum, type, class);

      /* If OP0 and OP1 are the same, we can use OUT for OP1.
	 This fixes a problem on the 32K where the stack pointer cannot
	 be used as an operand of an add insn.  */

      if (rtx_equal_p (op0, op1))
	op1 = out;

      insn = emit_insn (gen_add2_insn (out, op1));

      /* If that failed, copy the address register to the reload register.
	 Then add the constant to the reload register.  */

      code = recog_memoized (insn);

      if (code >= 0)
	return insn;

      delete_insns_since (last);

      gen_pre_reload (out, op1, opnum, type, class);
      insn = emit_insn (gen_add2_insn (out, op0));
    }

#ifdef SECONDARY_MEMORY_NEEDED
  /* If we need a memory location to do the move, do it that way.  */
  else if (REG_P (in) && REG_P (out)
	   && SECONDARY_MEMORY_NEEDED ((NEW_REG_P (in) ? class
					: pre_reg_pref (REGNO (in))),
				       (NEW_REG_P (out) ? class
					: pre_reg_pref (REGNO (out))),
				       GET_MODE (out)))
    {
      /* Get the memory to use and rewrite both registers to its mode.  */
      rtx loc = get_pre_secondary_mem (in, GET_MODE (out), opnum, type);

      if (GET_MODE (loc) != GET_MODE (out))
	out = gen_rtx_REG (GET_MODE (loc), REGNO (out));

      if (GET_MODE (loc) != GET_MODE (in))
	in = gen_rtx_REG (GET_MODE (loc), REGNO (in));

      gen_pre_reload (loc, in, opnum, type, class);
      gen_pre_reload (out, loc, opnum, type, class);
    }
#endif

  /* If IN is a simple operand, use gen_move_insn.  */
  else if (GET_RTX_CLASS (GET_CODE (in)) == 'o' || GET_CODE (in) == SUBREG)
    emit_insn (gen_move_insn (out, in));

#ifdef HAVE_reload_load_address
  else if (HAVE_reload_load_address)
    emit_insn (gen_reload_load_address (out, in));
#endif

  /* Otherwise, just write (set OUT IN) and hope for the best.  */
  else
    emit_insn (gen_rtx_SET (VOIDmode, out, in));

  /* Return the first insn emitted.
     We can not just return get_last_insn, because there may have
     been multiple instructions emitted.  Also note that gen_move_insn may
     emit more than one insn itself, so we can not assume that there is one
     insn emitted per emit_insn_before call.  */

  return last ? NEXT_INSN (last) : get_insns ();
}

/* Output reload-insns to reload VALUE into RELOADREG.
   VALUE is an autoincrement or autodecrement RTX whose operand
   is a register or memory location;
   so reloading involves incrementing that location.
   IN is either identical to VALUE, or some cheaper place to reload from.

   INC_AMOUNT is the number to increment or decrement by (always positive).
   This cannot be deduced from VALUE.

   Return the instruction that stores into RELOADREG.  */

static rtx
inc_for_pre_reload (reloadreg, in, value, inc_amount)
     rtx reloadreg;
     rtx in, value;
     int inc_amount;
{
  rtx last;
  rtx inc;
  rtx add_insn;
  int code;
  rtx store;
  /* REG or MEM to be copied and incremented.  */
  rtx incloc = XEXP (value, 0);
  /* Nonzero if increment after copying.  */
  int post = (GET_CODE (value) == POST_DEC || GET_CODE (value) == POST_INC);
  rtx real_in = in == value ? XEXP (in, 0) : in;

  if (GET_CODE (value) == PRE_DEC || GET_CODE (value) == POST_DEC)
    inc_amount = -inc_amount;

  inc = GEN_INT (inc_amount);

  reloadreg = copy_rtx (reloadreg);
  incloc = copy_rtx (reloadreg);
  real_in = copy_rtx (real_in);

  /* If this is post-increment, first copy the location to the reload reg.  */
  if (post && real_in != reloadreg)
    emit_insn (gen_move_insn (reloadreg, real_in));

  if (in == value)
    {
      /* See if we can directly increment INCLOC.  Use a method similar to
	 that in gen_reload.  */

      last = get_last_insn ();
      add_insn = emit_insn (gen_rtx_SET (VOIDmode, incloc,
					 gen_rtx_PLUS (GET_MODE (incloc),
						       incloc, inc)));

      code = recog_memoized (add_insn);
      if (code >= 0)
	{
	  /* If this is a pre-increment and we have incremented the value
	     where it lives, copy the incremented value to RELOADREG to
	     be used as an address.  */
	  
	  if (! post)
	    emit_insn (gen_move_insn (reloadreg, incloc));
	  
	  return add_insn;
	}
      delete_insns_since (last);
    }

  /* If couldn't do the increment directly, must increment in RELOADREG.
     The way we do this depends on whether this is pre- or post-increment.
     For pre-increment, copy INCLOC to the reload register, increment it
     there, then save back.  */

  if (! post)
    {
      if (in != reloadreg)
	emit_insn (gen_move_insn (reloadreg, real_in));
      emit_insn (gen_add2_insn (reloadreg, inc));
      store = emit_insn (gen_move_insn (incloc, reloadreg));
    }
  else
    {
      /* Postincrement.
	 Because this might be a jump insn or a compare, and because RELOADREG
	 may not be available after the insn in an input reload, we must do
	 the incrementation before the insn being reloaded for.

	 We have already copied IN to RELOADREG.  Increment the copy in
	 RELOADREG, save that back, then decrement RELOADREG so it has
	 the original value.  */

      emit_insn (gen_add2_insn (reloadreg, inc));
      store = emit_insn (gen_move_insn (incloc, reloadreg));
      emit_insn (gen_add2_insn (reloadreg, GEN_INT (-inc_amount)));
    }

  return store;
}

/* Generate insns to perform reload RL, which is for the insn in CHAIN and
   has the number J.  OLD contains the value to be used as input.  */

static void
emit_input_pre_reload_insns (insn, rl, old, j)
     rtx insn;
     struct reload *rl;
     rtx old;
     int j ATTRIBUTE_UNUSED;
{
  rtx reloadreg = rl->reg_rtx;
  rtx oldequiv = 0;
  int special = 0;
  enum machine_mode mode;
  rtx *where;

  /* Determine the mode to reload in.
     This is very tricky because we have three to choose from.
     There is the mode the insn operand wants (rl->inmode).
     There is the mode of the reload register RELOADREG.
     There is the intrinsic mode of the operand, which we could find
     by stripping some SUBREGs.
     It turns out that RELOADREG's mode is irrelevant:
     we can change that arbitrarily.

     Consider (SUBREG:SI foo:QI) as an operand that must be SImode;
     then the reload reg may not support QImode moves, so use SImode.
     If foo is in memory due to spilling a pseudo reg, this is safe,
     because the QImode value is in the least significant part of a
     slot big enough for a SImode.  If foo is some other sort of
     memory reference, then it is impossible to reload this case,
     so previous passes had better make sure this never happens.

     Then consider a one-word union which has SImode and one of its
     members is a float, being fetched as (SUBREG:SF union:SI).
     We must fetch that as SFmode because we could be loading into
     a float-only register.  In this case OLD's mode is correct.

     Consider an immediate integer: it has VOIDmode.  Here we need
     to get a mode from something else.

     In some cases, there is a fourth mode, the operand's
     containing mode.  If the insn specifies a containing mode for
     this operand, it overrides all others.

     I am not sure whether the algorithm here is always right,
     but it does the right things in those cases.  */

  mode = GET_MODE (old);
  if (mode == VOIDmode)
    mode = rl->inmode;

  oldequiv = old;
  
  /* Encapsulate both RELOADREG and OLDEQUIV into that mode,
     then load RELOADREG from OLDEQUIV.  Note that we cannot use
     gen_lowpart_common since it can do the wrong thing when
     RELOADREG has a multi-word mode.  Note that RELOADREG
     must always be a REG here.  */

  if (GET_MODE (reloadreg) != mode)
    {
      if (INTEGRAL_MODE_P (GET_MODE (reloadreg)) && INTEGRAL_MODE_P (mode))
	{
	  int offset;
	  if (GET_MODE_SIZE (GET_MODE (reloadreg)) > GET_MODE_SIZE (mode))
	    {
	      offset = subreg_lowpart_offset (mode, GET_MODE (reloadreg));
	      reloadreg = simplify_gen_subreg (mode, reloadreg,
					       GET_MODE (reloadreg), offset);
	      if (!reloadreg)
		abort ();
	    }
	  else if (GET_MODE_SIZE (GET_MODE (reloadreg))
		   == GET_MODE_SIZE (mode))
	    reloadreg = simplify_gen_subreg (mode, reloadreg,
					     GET_MODE (reloadreg), 0);
	    /* reloadreg = gen_rtx_REG (mode, REGNO (reloadreg)); */
	  else
	    abort ();
	}
      else
	reloadreg = simplify_gen_subreg (mode, reloadreg,
					 GET_MODE (reloadreg), 0);
	/* reloadreg = gen_rtx_REG (mode, REGNO (reloadreg)); */
    }
  
  while (GET_CODE (oldequiv) == SUBREG && GET_MODE (oldequiv) != mode)
    oldequiv = SUBREG_REG (oldequiv);
  
  if (GET_MODE (oldequiv) != VOIDmode
      && mode != GET_MODE (oldequiv))
    oldequiv = gen_lowpart_SUBREG (mode, oldequiv);

  /* Switch to the right place to emit the reload insns.  */
  switch (rl->when_needed)
    {
    case RELOAD_OTHER:
      where = &other_input_reload_insns;
      break;
    case RELOAD_FOR_INPUT:
      where = &input_reload_insns[rl->opnum];
      break;
    case RELOAD_FOR_INPUT_ADDRESS:
      where = &input_address_reload_insns[rl->opnum];
      break;
    case RELOAD_FOR_INPADDR_ADDRESS:
      where = &inpaddr_address_reload_insns[rl->opnum];
      break;
    case RELOAD_FOR_OUTPUT_ADDRESS:
      where = &output_address_reload_insns[rl->opnum];
      break;
    case RELOAD_FOR_OUTADDR_ADDRESS:
      where = &outaddr_address_reload_insns[rl->opnum];
      break;
    case RELOAD_FOR_OPERAND_ADDRESS:
      where = &operand_reload_insns;
      break;
    case RELOAD_FOR_OPADDR_ADDR:
      where = &other_operand_reload_insns;
      break;
    case RELOAD_FOR_OTHER_ADDRESS:
      where = &other_input_address_reload_insns;
      break;
    default:
      abort ();
    }

  push_to_sequence (*where);

  /* Auto-increment addresses must be reloaded in a special way.  */
  if (rl->out && ! rl->out_reg)
    {
      /* We are not going to bother supporting the case where a
	 incremented register can't be copied directly from
	 OLDEQUIV since this seems highly unlikely.  */
      if (rl->secondary_in_reload >= 0)
	abort ();

      old = XEXP (rl->in_reg, 0);

      /* Prevent normal processing of this reload.  */
      special = 1;
      /* Output a special code sequence for this case.  */
      inc_for_pre_reload (reloadreg, oldequiv, rl->out,
			  rl->inc);
    }

  /* If we are reloading a pseudo-register that was set by the previous
     insn, see if we can get rid of that pseudo-register entirely
     by redirecting the previous insn into our reload register.  */

  if (! special && ! rtx_equal_p (reloadreg, oldequiv))
    {
      gen_pre_reload (reloadreg, oldequiv, rl->opnum, rl->when_needed,
		      rl->class);
    }

  if (flag_non_call_exceptions)
    copy_eh_notes (insn, get_insns ());

  /* End this sequence.  */
  *where = get_insns ();
  end_sequence ();
}

/* Do output reloading for reload RL, which is for the insn described by
   CHAIN and has the number J.
   ??? At some point we need to support handling output reloads of
   JUMP_INSNs or insns that set cc0.  */
static void
do_output_pre_reload (insn, rl, j)
     rtx insn;
     struct reload *rl;
     int j;
{
  rtx old;
  /* If this is an output reload that stores something that is
     not loaded in this same reload, see if we can eliminate a previous
     store.  */

  old = rl->out_reg;
  if (old == 0
      || rl->reg_rtx == old
      || rl->reg_rtx == 0)
    return;

  if (GET_CODE (old) == SCRATCH)
    /* If we aren't optimizing, there won't be a REG_UNUSED note,
       but we don't want to make an output reload.  */
    return;
  
  /* If is a JUMP_INSN, we can't support output reloads yet.  */
  if (GET_CODE (insn) == JUMP_INSN)
    abort ();

  emit_output_pre_reload_insns (insn, rld + j, j);
}

/* Return 1 if register REGNO is the subject of a clobber in insn INSN.
   If SETS is nonzero, also consider SETs.  */

static int
pseudo_clobbered_p (regno, insn, mode, sets)
     unsigned int regno;
     rtx insn;
     enum machine_mode mode ATTRIBUTE_UNUSED;
     int sets;
{
  if ((GET_CODE (PATTERN (insn)) == CLOBBER
       || (sets && GET_CODE (PATTERN (insn)) == SET))
      && REG_P (XEXP (PATTERN (insn), 0)))
    {
      unsigned int test = REGNO (XEXP (PATTERN (insn), 0));

      return test == regno;
    }

  if (GET_CODE (PATTERN (insn)) == PARALLEL)
    {
      int i = XVECLEN (PATTERN (insn), 0) - 1;

      for (; i >= 0; i--)
	{
	  rtx elt = XVECEXP (PATTERN (insn), 0, i);
	  if ((GET_CODE (elt) == CLOBBER
	       || (sets && GET_CODE (PATTERN (insn)) == SET))
	      && REG_P (XEXP (elt, 0)))
	    {
	      unsigned int test = REGNO (XEXP (elt, 0));
	      
	      if (test == regno)
		return 1;
	    }
	}
    }

  return 0;
}

/* Generate insns to for the output reload RL, which is for the insn described
   by CHAIN and has the number J.  */
static void
emit_output_pre_reload_insns (insn, rl, j)
     rtx insn;
     struct reload *rl;
     int j ATTRIBUTE_UNUSED;
{
  rtx reloadreg = rl->reg_rtx;
  int special = 0;
  rtx old = rl->out;
  enum machine_mode mode = GET_MODE (old);

  if (rl->when_needed == RELOAD_OTHER)
    start_sequence ();
  else
    push_to_sequence (output_reload_insns[rl->opnum]);

  /* Determine the mode to reload in.
     See comments above (for input reloading).  */

  if (mode == VOIDmode)
    {
      /* VOIDmode should never happen for an output.  */
      if (asm_noperands (PATTERN (insn)) < 0)
	/* It's the compiler's fault.  */
	fatal_insn ("VOIDmode on an output", insn);
      error_for_asm (insn, "output operand is constant in `asm'");
      /* Prevent crash--use something we know is valid.  */
      mode = word_mode;
      old = gen_rtx_REG (mode, REGNO (reloadreg));
    }

  if (GET_MODE (reloadreg) != mode)
    {
      int offset;
      if (INTEGRAL_MODE_P (GET_MODE (reloadreg))
	  && INTEGRAL_MODE_P (mode))
	{
	  if (GET_MODE_SIZE (GET_MODE (reloadreg)) > GET_MODE_SIZE (mode))
	    {
	      offset = subreg_lowpart_offset (mode, GET_MODE (reloadreg));
	      reloadreg = simplify_gen_subreg (mode, reloadreg,
					       GET_MODE (reloadreg), offset);
	      if (!reloadreg)
		abort ();
	    }
	  else if (GET_MODE_SIZE (GET_MODE (reloadreg))
		   == GET_MODE_SIZE (mode))
	    reloadreg = simplify_gen_subreg (mode, reloadreg,
					     GET_MODE (reloadreg), 0);
	    /* reloadreg = gen_rtx_REG (mode, REGNO (reloadreg)); */
	  else
	    abort ();
	}
      else
	reloadreg = simplify_gen_subreg (mode, reloadreg,
					 GET_MODE (reloadreg), 0);
	/* reloadreg = gen_rtx_REG (mode, REGNO (reloadreg)); */
    }

  /* Output the last reload insn.  */
  if (! special)
    {
      rtx set;

      /* Don't output the last reload if OLD is not the dest of
	 INSN and is in the src and is clobbered by INSN.  */
      if (! flag_expensive_optimizations
	  || ! REG_P (old)
	  || ! (set = single_set (insn))
	  || rtx_equal_p (old, SET_DEST (set))
	  || ! reg_mentioned_p (old, SET_SRC (set))
	  || ! pseudo_clobbered_p (REGNO (old), insn, rl->mode, 0))
	gen_pre_reload (old, reloadreg, rl->opnum, rl->when_needed, rl->class);
    }

  if (rl->when_needed == RELOAD_OTHER)
    {
      emit_insn (other_output_reload_insns[rl->opnum]);
      other_output_reload_insns[rl->opnum] = get_insns ();
    }
  else
    output_reload_insns[rl->opnum] = get_insns ();

  if (flag_non_call_exceptions)
    copy_eh_notes (insn, get_insns ());

  end_sequence ();
}

/* Do input reloading for reload RL, which is for the insn described by CHAIN
   and has the number J.  */
static void
do_input_pre_reload (insn, rl, j)
     rtx insn;
     struct reload *rl;
     int j;
{
  rtx old = (rl->in && GET_CODE (rl->in) == MEM
	     ? rl->in_reg : rl->in);

  if (old != 0
      /* AUTO_INC reloads need to be handled even if inherited.  We got an
	 AUTO_INC reload if reload_out is set but reload_out_reg isn't.  */
      && (/*  ! reload_inherited[j] || (rl->out && ! rl->out_reg)*/ 1)
      && ! rtx_equal_p (rl->reg_rtx, old)
      && rl->reg_rtx != 0)
    emit_input_pre_reload_insns (insn, rld + j, old, j);
}

/* Output insns to reload values in and out of the chosen reload regs.  */

void
emit_pre_reload_insns (insn)
     rtx insn;
{
  int j;
  rtx following_insn = NEXT_INSN (insn);

  for (j = 0; j < n_reloads; ++j)
    {
      rtx in;
      rtx out;
      int r = j;
      
      /* Ignore reloads that got marked inoperative.  */
      if (rld[r].optional
	  || (rld[r].out == 0 && rld[r].in == 0 && ! rld[r].secondary_p))
	continue;
  
      /* If find_reloads chose to use reload_in or reload_out as a reload
	 register, we don't need to chose one.  Otherwise, try even if it
	 found one since we might save an insn if we find the value lying
	 around.
	 Try also when reload_in is a pseudo without a hard reg.  */
      if (rld[r].in != 0 && rld[r].reg_rtx != 0
	  && (rtx_equal_p (rld[r].in, rld[r].reg_rtx)
	      || (rtx_equal_p (rld[r].out, rld[r].reg_rtx)
		  && GET_CODE (rld[r].in) != MEM)))
	continue;

      in = rld[r].in;
      out = rld[r].out;
      rld[r].reg_rtx = gen_reg_rtx (rld[r].mode);
    }

  for (j = 0; j < reload_n_operands; j++)
    input_reload_insns[j] = input_address_reload_insns[j]
      = inpaddr_address_reload_insns[j]
      = output_reload_insns[j] = output_address_reload_insns[j]
      = outaddr_address_reload_insns[j]
      = other_output_reload_insns[j] = 0;
  other_input_address_reload_insns = 0;
  other_input_reload_insns = 0;
  operand_reload_insns = 0;
  other_operand_reload_insns = 0;

  /* Dump reloads into the dump file.  */
  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "\nReloads for insn # %d\n", INSN_UID (insn));
      debug_reload_to_stream (rtl_dump_file);
      fprintf (rtl_dump_file, "\n");
    }

  /* Now output the instructions to copy the data into and out of the
     reload registers.  Do these in the order that the reloads were reported,
     since reloads of base and index registers precede reloads of operands
     and the operands may need the base and index registers reloaded.  */

  for (j = 0; j < n_reloads; j++)
    {
      if (!rld[j].optional)
	{
	  do_input_pre_reload (insn, rld + j, j);
	  do_output_pre_reload (insn, rld + j, j);
	}
    }

  /* Now write all the insns we made for reloads in the order expected by
     the allocation functions.  Prior to the insn being reloaded, we write
     the following reloads:

     RELOAD_FOR_OTHER_ADDRESS reloads for input addresses.

     RELOAD_OTHER reloads.

     For each operand, any RELOAD_FOR_INPADDR_ADDRESS reloads followed
     by any RELOAD_FOR_INPUT_ADDRESS reloads followed by the
     RELOAD_FOR_INPUT reload for the operand.

     RELOAD_FOR_OPADDR_ADDRS reloads.

     RELOAD_FOR_OPERAND_ADDRESS reloads.

     After the insn being reloaded, we write the following:

     For each operand, any RELOAD_FOR_OUTADDR_ADDRESS reloads followed
     by any RELOAD_FOR_OUTPUT_ADDRESS reload followed by the
     RELOAD_FOR_OUTPUT reload, followed by any RELOAD_OTHER output
     reloads for the operand.  The RELOAD_OTHER output reloads are
     output in descending order by reload number.  */

  emit_insn_before (other_input_address_reload_insns, insn);
  emit_insn_before (other_input_reload_insns, insn);

  for (j = 0; j < reload_n_operands; j++)
    {
      emit_insn_before (inpaddr_address_reload_insns[j], insn);
      emit_insn_before (input_address_reload_insns[j], insn);
      emit_insn_before (input_reload_insns[j], insn);
    }

  emit_insn_before (other_operand_reload_insns, insn);
  emit_insn_before (operand_reload_insns, insn);

  for (j = 0; j < reload_n_operands; j++)
    {
      emit_insn_before (outaddr_address_reload_insns[j], following_insn);
      emit_insn_before (output_address_reload_insns[j], following_insn);
      emit_insn_before (output_reload_insns[j], following_insn);
      emit_insn_before (other_output_reload_insns[j], following_insn);
    }
}


/* Substitute into the current INSN the registers into which we have reloaded
   the things that need reloading.  The array `replacements'
   contains the locations of all pointers that must be changed
   and says what to replace them with.

   Return the rtx that X translates into; usually X, but modified.  */

void
subst_pre_reloads (insn)
     rtx insn ATTRIBUTE_UNUSED;
{
  int i;

  for (i = 0; i < n_replacements; i++)
    {
      struct replacement *r = &replacements[i];
      rtx reloadreg = rld[r->what].reg_rtx;
      if (GET_CODE (reloadreg) != REG)
	reloadreg = copy_rtx (reloadreg);

      if (reloadreg && ! rld[r->what].optional)
	{
	  /* Encapsulate RELOADREG so its machine mode matches what
	     used to be there.  Note that gen_lowpart_common will
	     do the wrong thing if RELOADREG is multi-word.  RELOADREG
	     will always be a REG here.  */
	  if (GET_MODE (reloadreg) != r->mode && r->mode != VOIDmode)
	    {
	      if (INTEGRAL_MODE_P (GET_MODE (reloadreg))
		  && INTEGRAL_MODE_P (r->mode))
		{
		  int offset;

		  if (GET_MODE_SIZE (GET_MODE (reloadreg))
		      > GET_MODE_SIZE (r->mode))
		    {
		      offset = subreg_lowpart_offset (r->mode,
						      GET_MODE (reloadreg));
		      reloadreg = simplify_gen_subreg (r->mode, reloadreg,
						       GET_MODE (reloadreg),
						       offset);
		      if (!reloadreg)
			abort ();
		    }
		  else if (GET_MODE_SIZE (GET_MODE (reloadreg))
			   == GET_MODE_SIZE (r->mode))
		    reloadreg = simplify_gen_subreg (r->mode, reloadreg,
						     GET_MODE (reloadreg), 0);
		    /*reloadreg = gen_rtx_REG (r->mode, REGNO (reloadreg));*/
		  else
		    abort ();
		}
	      else
		reloadreg = simplify_gen_subreg (r->mode, reloadreg,
						 GET_MODE (reloadreg), 0);
		/* reloadreg = gen_rtx_REG (r->mode, REGNO (reloadreg)); */
	    }

	  /* If we are putting this into a SUBREG and RELOADREG is a
	     SUBREG, we would be making nested SUBREGs, so we have to fix
	     this up.  Note that r->where == &SUBREG_REG (*r->subreg_loc).  */

	  if (r->subreg_loc != 0 && GET_CODE (reloadreg) == SUBREG)
	    {
	      if (GET_MODE (*r->subreg_loc)
		  == GET_MODE (SUBREG_REG (reloadreg)))
		*r->subreg_loc = SUBREG_REG (reloadreg);
	      else
		{
		  int final_offset =
		    SUBREG_BYTE (*r->subreg_loc) + SUBREG_BYTE (reloadreg);

		  /* When working with SUBREGs the rule is that the byte
		     offset must be a multiple of the SUBREG's mode.  */
		  final_offset = (final_offset /
				  GET_MODE_SIZE (GET_MODE (*r->subreg_loc)));
		  final_offset = (final_offset *
				  GET_MODE_SIZE (GET_MODE (*r->subreg_loc)));

		  *r->where = SUBREG_REG (reloadreg);
		  SUBREG_BYTE (*r->subreg_loc) = final_offset;
		}
	    }
	  else
	    *r->where = reloadreg;
	}
      /* If reload got no reg and isn't optional, something's wrong.  */
      else if (! rld[r->what].optional)
	abort ();
    }
}

#ifdef SECONDARY_MEMORY_NEEDED

/* Return a memory location that will be used to copy X in mode MODE.
   If we haven't already made a location for this mode in this insn,
   call find_reloads_address on the location being returned.  */

rtx
get_pre_secondary_mem (x, mode, opnum, type)
     rtx x ATTRIBUTE_UNUSED;
     enum machine_mode mode;
     int opnum ATTRIBUTE_UNUSED;
     enum reload_type type ATTRIBUTE_UNUSED;
{
  rtx loc;

  /* By default, if MODE is narrower than a word, widen it to a word.
     This is required because most machines that require these memory
     locations do not support short load and stores from all registers
     (e.g., FP registers).  */

#ifdef SECONDARY_MEMORY_NEEDED_MODE
  mode = SECONDARY_MEMORY_NEEDED_MODE (mode);
#else
  if (GET_MODE_BITSIZE (mode) < BITS_PER_WORD && INTEGRAL_MODE_P (mode))
    mode = mode_for_size (BITS_PER_WORD, GET_MODE_CLASS (mode), 0);
#endif

  /* If this is the first time we've tried to get a MEM for this mode,
     allocate a new one.  `something_changed' in reload will get set
     by noticing that the frame size has changed.  */

  if (secondary_memlocs[(int) mode] == 0)
    {
#ifdef SECONDARY_MEMORY_NEEDED_RTX
      loc = secondary_memlocs[(int) mode] = SECONDARY_MEMORY_NEEDED_RTX (mode);
#else
      loc = secondary_memlocs[(int) mode]
	= assign_stack_local (mode, GET_MODE_SIZE (mode), 0);
#endif
    }

  return loc;
}
#endif

/* Try to find a reload register for an in-out reload (expressions IN and OUT).
   See if one of IN and OUT is a register that may be used;
   this is desirable since a spill-register won't be needed.
   If so, return the register rtx that proves acceptable.

   INLOC and OUTLOC are locations where IN and OUT appear in the insn.
   CLASS is the register class required for the reload.

   If FOR_REAL is >= 0, it is the number of the reload,
   and in some cases when it can be discovered that OUT doesn't need
   to be computed, clear out rld[FOR_REAL].out.

   If FOR_REAL is -1, this should not be done, because this call
   is just to see if a register can be found, not to find and install it.

   EARLYCLOBBER is non-zero if OUT is an earlyclobber operand.  This
   puts an additional constraint on being able to use IN for OUT since
   IN must not appear elsewhere in the insn (it is assumed that IN itself
   is safe from the earlyclobber).  */

static rtx
find_dummy_pre_reload (real_in, real_out, inloc, outloc,
		       inmode, outmode, class, for_real, earlyclobber)
     rtx real_in, real_out;
     rtx *inloc ATTRIBUTE_UNUSED;
     rtx *outloc;
     enum machine_mode inmode, outmode;
     enum reg_class class;
     int for_real ATTRIBUTE_UNUSED;
     int earlyclobber ATTRIBUTE_UNUSED;
{
  rtx in = real_in;
  rtx out = real_out;
  int in_offset = 0;
  int out_offset = 0;
  rtx value = 0;

  /* If operands exceed a word, we can't use either of them
     unless they have the same size.  */
  if (GET_MODE_SIZE (outmode) != GET_MODE_SIZE (inmode)
      && (GET_MODE_SIZE (outmode) > UNITS_PER_WORD
	  || GET_MODE_SIZE (inmode) > UNITS_PER_WORD))
    return 0;

  /* We must use widest mode for reload reg rtx.  */
  if (INTEGRAL_MODE_P (outmode) && INTEGRAL_MODE_P (inmode)
      && GET_MODE_SIZE (outmode) < GET_MODE_SIZE (inmode))
    return 0;

  /* Note that {in,out}_offset are needed only when 'in' or 'out'
     respectively refers to a hard register.  */

  /* Find the inside of any subregs.  */
  while (GET_CODE (out) == SUBREG)
    {
      if (GET_CODE (SUBREG_REG (out)) == REG
	  && REGNO (SUBREG_REG (out)) < FIRST_PSEUDO_REGISTER)
	out_offset += subreg_regno_offset (REGNO (SUBREG_REG (out)),
					   GET_MODE (SUBREG_REG (out)),
					   SUBREG_BYTE (out),
					   GET_MODE (out));
      out = SUBREG_REG (out);
    }
  while (GET_CODE (in) == SUBREG)
    {
      if (GET_CODE (SUBREG_REG (in)) == REG
	  && REGNO (SUBREG_REG (in)) < FIRST_PSEUDO_REGISTER)
	in_offset += subreg_regno_offset (REGNO (SUBREG_REG (in)),
					  GET_MODE (SUBREG_REG (in)),
					  SUBREG_BYTE (in),
					  GET_MODE (in));
      in = SUBREG_REG (in);
    }

  /* Narrow down the reg class, the same way push_reload will;
     otherwise we might find a dummy now, but push_reload won't.  */
  class = PREFERRED_RELOAD_CLASS (in, class);

  /* See if OUT will do.  */
  if (GET_CODE (out) == REG)
    {
      unsigned int regno = REGNO (out) + out_offset;

      /* When we consider whether the insn uses OUT,
	 ignore references within IN.  They don't prevent us
	 from copying IN into OUT, because those refs would
	 move into the insn that reloads IN.

	 However, we only ignore IN in its role as this reload.
	 If the insn uses IN elsewhere and it contains OUT,
	 that counts.  We can't be sure it's the "same" operand
	 so it might not go through this reload.  */

      if (! refers_to_regno_p (regno, regno + 1,
			       PATTERN (this_insn), outloc))
	value = real_out;
    }

  return value;
}

/* Record one reload that needs to be performed.
   IN is an rtx saying where the data are to be found before this instruction.
   OUT says where they must be stored after the instruction.
   (IN is zero for data not read, and OUT is zero for data not written.)
   INLOC and OUTLOC point to the places in the instructions where
   IN and OUT were found.
   If IN and OUT are both non-zero, it means the same register must be used
   to reload both IN and OUT.

   CLASS is a register class required for the reloaded data.
   INMODE is the machine mode that the instruction requires
   for the reg that replaces IN and OUTMODE is likewise for OUT.

   If IN is zero, then OUT's location and mode should be passed as
   INLOC and INMODE.

   STRICT_LOW is the 1 if there is a containing STRICT_LOW_PART rtx.

   OPTIONAL nonzero means this reload does not need to be performed:
   it can be discarded if that is more convenient.

   OPNUM and TYPE say what the purpose of this reload is.

   The return value is the reload-number for this reload.

   If both IN and OUT are nonzero, in some rare cases we might
   want to make two separate reloads.  (Actually we never do this now.)
   Therefore, the reload-number for OUT is stored in
   output_reloadnum when we return; the return value applies to IN.
   Usually (presently always), when IN and OUT are nonzero,
   the two reload-numbers are equal, but the caller should be careful to
   distinguish them.  */

static int
push_pre_reload (in, out, inloc, outloc, class,
		 inmode, outmode, strict_low, optional, opnum, type)
     rtx in, out;
     rtx *inloc, *outloc;
     enum reg_class class;
     enum machine_mode inmode, outmode;
     int strict_low;
     int optional;
     int opnum;
     enum reload_type type;
{
  int i;
  int dont_share = 0;
  int dont_remove_subreg = 0;
  rtx *in_subreg_loc = 0, *out_subreg_loc = 0;
  int secondary_in_reload = -1, secondary_out_reload = -1;
  enum insn_code secondary_in_icode = CODE_FOR_nothing;
  enum insn_code secondary_out_icode = CODE_FOR_nothing;
  enum machine_mode general_mode = VOIDmode;

  /* INMODE and/or OUTMODE could be VOIDmode if no mode
     has been specified for the operand.  In that case,
     use the operand's mode as the mode to reload.  */
  if (inmode == VOIDmode && in != 0)
    inmode = GET_MODE (in);
  if (outmode == VOIDmode && out != 0)
    outmode = GET_MODE (out);

  /* If we have a read-write operand with an address side-effect,
     change either IN or OUT so the side-effect happens only once.  */
  if (in != 0 && out != 0 && GET_CODE (in) == MEM && rtx_equal_p (in, out))
    switch (GET_CODE (XEXP (in, 0)))
      {
      case POST_INC: case POST_DEC:   case POST_MODIFY:
	in = replace_equiv_address_nv (in, XEXP (XEXP (in, 0), 0));
	break;

      case PRE_INC: case PRE_DEC: case PRE_MODIFY:
	out = replace_equiv_address_nv (out, XEXP (XEXP (out, 0), 0));
	break;

      default:
	break;
    }

  /* If we are reloading a (SUBREG constant ...), really reload just the
     inside expression in its own mode.  Similarly for (SUBREG (PLUS ...)).
     If we have (SUBREG:M1 (MEM:M2 ...) ...) (or an inner REG that is still
     a pseudo and hence will become a MEM) with M1 wider than M2 and the
     register is a pseudo, also reload the inside expression.
     For machines that extend byte loads, do this for any SUBREG of a pseudo
     where both M1 and M2 are a word or smaller, M1 is wider than M2, and
     M2 is an integral mode that gets extended when loaded.
     Similar issue for (SUBREG:M1 (REG:M2 ...) ...) for a hard register R where
     either M1 is not valid for R or M2 is wider than a word but we only
     need one word to store an M2-sized quantity in R.
     (However, if OUT is nonzero, we need to reload the reg *and*
     the subreg, so do nothing here, and let following statement handle it.)

     Note that the case of (SUBREG (CONST_INT...)...) is handled elsewhere;
     we can't handle it here because CONST_INT does not indicate a mode.

     Similarly, we must reload the inside expression if we have a
     STRICT_LOW_PART (presumably, in == out in the cas).

     Also reload the inner expression if it does not require a secondary
     reload but the SUBREG does.

     Finally, reload the inner expression if it is a register that is in
     the class whose registers cannot be referenced in a different size
     and M1 is not the same size as M2.  If SUBREG_BYTE is nonzero, we
     cannot reload just the inside since we might end up with the wrong
     register class.  But if it is inside a STRICT_LOW_PART, we have
     no choice, so we hope we do get the right register class there.  */

  if (in != 0 && GET_CODE (in) == SUBREG
      && (SUBREG_BYTE (in) == 0 || strict_low)
#ifdef CLASS_CANNOT_CHANGE_MODE
      && (class != CLASS_CANNOT_CHANGE_MODE
	  || ! CLASS_CANNOT_CHANGE_MODE_P (GET_MODE (SUBREG_REG (in)), inmode))
#endif
      && (CONSTANT_P (SUBREG_REG (in))
	  || GET_CODE (SUBREG_REG (in)) == PLUS
	  || strict_low
	  || (((0
		&& GET_CODE (SUBREG_REG (in)) == REG
		&& REGNO (SUBREG_REG (in)) >= FIRST_PSEUDO_REGISTER)
	       || GET_CODE (SUBREG_REG (in)) == MEM)
	      && ((GET_MODE_SIZE (inmode)
		   > GET_MODE_SIZE (GET_MODE (SUBREG_REG (in))))
#ifdef LOAD_EXTEND_OP
		  || (GET_MODE_SIZE (inmode) <= UNITS_PER_WORD
		      && (GET_MODE_SIZE (GET_MODE (SUBREG_REG (in)))
			  <= UNITS_PER_WORD)
		      && (GET_MODE_SIZE (inmode)
			  > GET_MODE_SIZE (GET_MODE (SUBREG_REG (in))))
		      && INTEGRAL_MODE_P (GET_MODE (SUBREG_REG (in)))
		      && LOAD_EXTEND_OP (GET_MODE (SUBREG_REG (in))) != NIL)
#endif
#ifdef WORD_REGISTER_OPERATIONS
		  || ((GET_MODE_SIZE (inmode)
		       < GET_MODE_SIZE (GET_MODE (SUBREG_REG (in))))
		      && ((GET_MODE_SIZE (inmode) - 1) / UNITS_PER_WORD ==
			  ((GET_MODE_SIZE (GET_MODE (SUBREG_REG (in))) - 1)
			   / UNITS_PER_WORD)))
#endif
		  ))
	  || (GET_CODE (SUBREG_REG (in)) == REG
	      && REGNO (SUBREG_REG (in)) < FIRST_PSEUDO_REGISTER
	      /* The case where out is nonzero
		 is handled differently in the following statement.  */
	      && (out == 0 || SUBREG_BYTE (in) == 0)
	      && ((GET_MODE_SIZE (inmode) <= UNITS_PER_WORD
		   && (GET_MODE_SIZE (GET_MODE (SUBREG_REG (in)))
		       > UNITS_PER_WORD)
		   && ((GET_MODE_SIZE (GET_MODE (SUBREG_REG (in)))
			/ UNITS_PER_WORD)
		       != HARD_REGNO_NREGS (REGNO (SUBREG_REG (in)),
					    GET_MODE (SUBREG_REG (in)))))
		  || ! HARD_REGNO_MODE_OK (subreg_regno (in), inmode)))))
    {
      in_subreg_loc = inloc;
      inloc = &SUBREG_REG (in);
      in = *inloc;
#if ! defined (LOAD_EXTEND_OP) && ! defined (WORD_REGISTER_OPERATIONS)
      if (GET_CODE (in) == MEM)
	/* This is supposed to happen only for paradoxical subregs made by
	   combine.c.  (SUBREG (MEM)) isn't supposed to occur other ways.  */
	if (GET_MODE_SIZE (GET_MODE (in)) > GET_MODE_SIZE (inmode))
	  abort ();
#endif
      general_mode = inmode;
      inmode = GET_MODE (in);
    }

  /* Similar issue for (SUBREG:M1 (REG:M2 ...) ...) for a hard register R where
     either M1 is not valid for R or M2 is wider than a word but we only
     need one word to store an M2-sized quantity in R.

     However, we must reload the inner reg *as well as* the subreg in
     that case.  */

  /* Similar issue for (SUBREG constant ...) if it was not handled by the
     code above.  This can happen if SUBREG_BYTE != 0.  */

  if (in != 0 && reload_inner_reg_of_subreg (in, inmode, 0))
    {
      enum reg_class in_class = class;

      if (GET_CODE (SUBREG_REG (in)) == REG)
	in_class
	  = find_valid_class (inmode,
			      subreg_regno_offset (REGNO (SUBREG_REG (in)),
						   GET_MODE (SUBREG_REG (in)),
						   SUBREG_BYTE (in),
						   GET_MODE (in)),
			      REGNO (SUBREG_REG (in)));

      /* This relies on the fact that emit_reload_insns outputs the
	 instructions for input reloads of type RELOAD_OTHER in the same
	 order as the reloads.  Thus if the outer reload is also of type
	 RELOAD_OTHER, we are guaranteed that this inner reload will be
	 output before the outer reload.  */
      push_pre_reload (SUBREG_REG (in), NULL_RTX, &SUBREG_REG (in), (rtx *)0,
		       in_class, VOIDmode, VOIDmode, 0, 0, opnum, type);
      dont_remove_subreg = 1;
    }

  /* Similarly for paradoxical and problematical SUBREGs on the output.
     Note that there is no reason we need worry about the previous value
     of SUBREG_REG (out); even if wider than out,
     storing in a subreg is entitled to clobber it all
     (except in the case of STRICT_LOW_PART,
     and in that case the constraint should label it input-output.)  */
  if (out != 0 && GET_CODE (out) == SUBREG
      && (SUBREG_BYTE (out) == 0 || strict_low)
#ifdef CLASS_CANNOT_CHANGE_MODE
      && (class != CLASS_CANNOT_CHANGE_MODE
	  || ! CLASS_CANNOT_CHANGE_MODE_P (GET_MODE (SUBREG_REG (out)),
					   outmode))
#endif
      && (CONSTANT_P (SUBREG_REG (out))
	  || strict_low
	  || (((0
		&& GET_CODE (SUBREG_REG (out)) == REG
		&& REGNO (SUBREG_REG (out)) >= FIRST_PSEUDO_REGISTER)
	       || GET_CODE (SUBREG_REG (out)) == MEM)
	      && ((GET_MODE_SIZE (outmode)
		   > GET_MODE_SIZE (GET_MODE (SUBREG_REG (out))))
#ifdef WORD_REGISTER_OPERATIONS
		  || ((GET_MODE_SIZE (outmode)
		       < GET_MODE_SIZE (GET_MODE (SUBREG_REG (out))))
		      && ((GET_MODE_SIZE (outmode) - 1) / UNITS_PER_WORD ==
			  ((GET_MODE_SIZE (GET_MODE (SUBREG_REG (out))) - 1)
			   / UNITS_PER_WORD)))
#endif
		  ))
	  || (GET_CODE (SUBREG_REG (out)) == REG
	      && REGNO (SUBREG_REG (out)) < FIRST_PSEUDO_REGISTER
	      && ((GET_MODE_SIZE (outmode) <= UNITS_PER_WORD
		   && (GET_MODE_SIZE (GET_MODE (SUBREG_REG (out)))
		       > UNITS_PER_WORD)
		   && ((GET_MODE_SIZE (GET_MODE (SUBREG_REG (out)))
			/ UNITS_PER_WORD)
		       != HARD_REGNO_NREGS (REGNO (SUBREG_REG (out)),
					    GET_MODE (SUBREG_REG (out)))))
		  || ! HARD_REGNO_MODE_OK (subreg_regno (out), outmode)))
#ifdef SECONDARY_OUTPUT_RELOAD_CLASS
	  || (SECONDARY_OUTPUT_RELOAD_CLASS (class, outmode, out) != NO_REGS
	      && (SECONDARY_OUTPUT_RELOAD_CLASS (class,
						 GET_MODE (SUBREG_REG (out)),
						 SUBREG_REG (out))
		  == NO_REGS))
#endif
#ifdef CLASS_CANNOT_CHANGE_MODE
	  || (GET_CODE (SUBREG_REG (out)) == REG
	      && REGNO (SUBREG_REG (out)) < FIRST_PSEUDO_REGISTER
	      && (TEST_HARD_REG_BIT
		  (reg_class_contents[(int) CLASS_CANNOT_CHANGE_MODE],
		   REGNO (SUBREG_REG (out))))
	      && CLASS_CANNOT_CHANGE_MODE_P (GET_MODE (SUBREG_REG (out)),
					     outmode))
#endif
	  ))
    {
      out_subreg_loc = outloc;
      outloc = &SUBREG_REG (out);
      out = *outloc;
#if ! defined (LOAD_EXTEND_OP) && ! defined (WORD_REGISTER_OPERATIONS)
      if (GET_CODE (out) == MEM
	  && GET_MODE_SIZE (GET_MODE (out)) > GET_MODE_SIZE (outmode))
	abort ();
#endif
      general_mode = outmode;
      outmode = GET_MODE (out);
    }

  /* Similar issue for (SUBREG:M1 (REG:M2 ...) ...) for a hard register R where
     either M1 is not valid for R or M2 is wider than a word but we only
     need one word to store an M2-sized quantity in R.

     However, we must reload the inner reg *as well as* the subreg in
     that case.  In this case, the inner reg is an in-out reload.  */

  if (out != 0 && reload_inner_reg_of_subreg (out, outmode, 1))
    {
      /* This relies on the fact that emit_reload_insns outputs the
	 instructions for output reloads of type RELOAD_OTHER in reverse
	 order of the reloads.  Thus if the outer reload is also of type
	 RELOAD_OTHER, we are guaranteed that this inner reload will be
	 output after the outer reload.  */
      dont_remove_subreg = 1;
      push_pre_reload (SUBREG_REG (out), SUBREG_REG (out), &SUBREG_REG (out),
		       &SUBREG_REG (out),
		       find_valid_class (outmode,
					 subreg_regno_offset (REGNO (SUBREG_REG (out)),
							      GET_MODE (SUBREG_REG (out)),
							      SUBREG_BYTE (out),
							      GET_MODE (out)),
					 REGNO (SUBREG_REG (out))),
		       VOIDmode, VOIDmode, 0, 0,
		       opnum, RELOAD_OTHER);
    }

  /* If IN appears in OUT, we can't share any input-only reload for IN.  */
  if (in != 0 && out != 0 && GET_CODE (out) == MEM
      && (GET_CODE (in) == REG || GET_CODE (in) == MEM))
    dont_share = 1;

  /* If IN is a SUBREG of a hard register, make a new REG.  This
     simplifies some of the cases below.  */

  if (in != 0 && GET_CODE (in) == SUBREG && GET_CODE (SUBREG_REG (in)) == REG
      && REGNO (SUBREG_REG (in)) < FIRST_PSEUDO_REGISTER
      && ! dont_remove_subreg)
    in = gen_rtx_REG (GET_MODE (in), subreg_regno (in));

  /* Similarly for OUT.  */
  if (out != 0 && GET_CODE (out) == SUBREG
      && GET_CODE (SUBREG_REG (out)) == REG
      && REGNO (SUBREG_REG (out)) < FIRST_PSEUDO_REGISTER
      && ! dont_remove_subreg)
    out = gen_rtx_REG (GET_MODE (out), subreg_regno (out));

  /* Narrow down the class of register wanted if that is
     desirable on this machine for efficiency.  */
  if (in != 0)
    class = PREFERRED_RELOAD_CLASS (in, class);

  /* Output reloads may need analogous treatment, different in detail.  */
#ifdef PREFERRED_OUTPUT_RELOAD_CLASS
  if (out != 0)
    class = PREFERRED_OUTPUT_RELOAD_CLASS (out, class);
#endif

  /* Make sure we use a class that can handle the actual pseudo
     inside any subreg.  For example, on the 386, QImode regs
     can appear within SImode subregs.  Although GENERAL_REGS
     can handle SImode, QImode needs a smaller class.  */
#ifdef LIMIT_RELOAD_CLASS
  if (in_subreg_loc)
    class = LIMIT_RELOAD_CLASS (inmode, class);
  else if (in != 0 && GET_CODE (in) == SUBREG)
    class = LIMIT_RELOAD_CLASS (GET_MODE (SUBREG_REG (in)), class);

  if (out_subreg_loc)
    class = LIMIT_RELOAD_CLASS (outmode, class);
  if (out != 0 && GET_CODE (out) == SUBREG)
    class = LIMIT_RELOAD_CLASS (GET_MODE (SUBREG_REG (out)), class);
#endif

  /* Verify that this class is at least possible for the mode that
     is specified.  */
  if (this_insn_is_asm)
    {
      enum machine_mode mode;
      if (GET_MODE_SIZE (inmode) > GET_MODE_SIZE (outmode))
	mode = inmode;
      else
	mode = outmode;
      if (mode == VOIDmode)
	{
	  error_for_asm (this_insn,
			 "cannot reload integer constant operand in `asm'");
	  mode = word_mode;
	  if (in != 0)
	    inmode = word_mode;
	  if (out != 0)
	    outmode = word_mode;
	}
    }

  /* Optional output reloads are always OK even if we have no register class,
     since the function of these reloads is only to have spill_reg_store etc.
     set, so that the storing insn can be deleted later.  */
  if (class == NO_REGS
      && (optional == 0 || type != RELOAD_FOR_OUTPUT))
    abort ();


#ifdef SECONDARY_MEMORY_NEEDED
  /* If a memory location is needed for the copy, make one.  */
  if (in != 0 && REG_P (in)
      && SECONDARY_MEMORY_NEEDED ((REGNO (in) >= FIRST_PSEUDO_REGISTER
				   ? reg_preferred_class (REGNO (in))
				   : REGNO_REG_CLASS (REGNO (in))),
				  class, inmode))
    get_pre_secondary_mem (in, inmode, opnum, type);
#endif

  i = n_reloads;
  rld[i].in = in;
  rld[i].out = out;
  rld[i].class = class;
  rld[i].inmode = inmode;
  rld[i].outmode = outmode;
  rld[i].mode = general_mode;
  rld[i].reg_rtx = 0;
  rld[i].optional = optional;
  rld[i].inc = 0;
  rld[i].nocombine = 0;
  rld[i].in_reg = inloc ? *inloc : 0;
  rld[i].out_reg = outloc ? *outloc : 0;
  rld[i].opnum = opnum;
  rld[i].when_needed = type;
  rld[i].secondary_in_reload = secondary_in_reload;
  rld[i].secondary_out_reload = secondary_out_reload;
  rld[i].secondary_in_icode = secondary_in_icode;
  rld[i].secondary_out_icode = secondary_out_icode;
  rld[i].secondary_p = 0;

  n_reloads++;

#ifdef SECONDARY_MEMORY_NEEDED
  if (out != 0 && REG_P (out)
      && SECONDARY_MEMORY_NEEDED (class,
				  (REGNO (out) >= FIRST_PSEUDO_REGISTER
				   ? reg_preferred_class (REGNO (out))
				   : REGNO_REG_CLASS (REGNO (out))),
				  outmode))
    get_pre_secondary_mem (out, outmode, opnum, type);
#endif

  /* If the ostensible rtx being reloaded differs from the rtx found
     in the location to substitute, this reload is not safe to combine
     because we cannot reliably tell whether it appears in the insn.  */

  if (in != 0 && in != *inloc)
    rld[i].nocombine = 1;

  /* If we will replace IN and OUT with the reload-reg,
     record where they are located so that substitution need
     not do a tree walk.  */

  if (inloc != 0)
    {
      struct replacement *r = &replacements[n_replacements++];
      r->what = i;
      r->subreg_loc = in_subreg_loc;
      r->where = inloc;
      r->mode = inmode;
    }
  if (outloc != 0 && outloc != inloc)
    {
      struct replacement *r = &replacements[n_replacements++];
      r->what = i;
      r->where = outloc;
      r->subreg_loc = out_subreg_loc;
      r->mode = outmode;
    }

  /* If this reload is just being introduced and it has both
     an incoming quantity and an outgoing quantity that are
     supposed to be made to match, see if either one of the two
     can serve as the place to reload into.

     If one of them is acceptable, set rld[i].reg_rtx
     to that one.  */

  if (in != 0 && out != 0 && in != out && rld[i].reg_rtx == 0)
    rld[i].reg_rtx = find_dummy_pre_reload (in, out, inloc, outloc,
					    inmode, outmode,
					    rld[i].class, i,
					    earlyclobber_operand_p (out));

  if (out)
    output_reloadnum = i;

  return i;
}


/* Describe the range of registers or memory referenced by X.
   If X is a register, set REG_FLAG and put the first register
   number into START and the last plus one into END.
   If X is a memory reference, put a base address into BASE
   and a range of integer offsets into START and END.
   If X is pushing on the stack, we can assume it causes no trouble,
   so we set the SAFE field.  */

static struct decomposition
pre_reload_decompose (x)
     rtx x;
{
  struct decomposition val;
  int all_const = 0;

  val.reg_flag = 0;
  val.safe = 0;
  val.base = 0;
  if (GET_CODE (x) == MEM)
    {
      rtx base = NULL_RTX, offset = 0;
      rtx addr = XEXP (x, 0);

      if (GET_CODE (addr) == PRE_DEC || GET_CODE (addr) == PRE_INC
	  || GET_CODE (addr) == POST_DEC || GET_CODE (addr) == POST_INC)
	{
	  val.base = XEXP (addr, 0);
	  val.start = -GET_MODE_SIZE (GET_MODE (x));
	  val.end = GET_MODE_SIZE (GET_MODE (x));
	  val.safe = REGNO (val.base) == STACK_POINTER_REGNUM;
	  return val;
	}

      if (GET_CODE (addr) == PRE_MODIFY || GET_CODE (addr) == POST_MODIFY)
	{
	  if (GET_CODE (XEXP (addr, 1)) == PLUS
	      && XEXP (addr, 0) == XEXP (XEXP (addr, 1), 0)
	      && CONSTANT_P (XEXP (XEXP (addr, 1), 1)))
	    {
	      val.base  = XEXP (addr, 0);
	      val.start = -INTVAL (XEXP (XEXP (addr, 1), 1));
	      val.end   = INTVAL (XEXP (XEXP (addr, 1), 1));
	      val.safe  = REGNO (val.base) == STACK_POINTER_REGNUM;
	      return val;
	    }
	}

      if (GET_CODE (addr) == CONST)
	{
	  addr = XEXP (addr, 0);
	  all_const = 1;
	}
      if (GET_CODE (addr) == PLUS)
	{
	  if (CONSTANT_P (XEXP (addr, 0)))
	    {
	      base = XEXP (addr, 1);
	      offset = XEXP (addr, 0);
	    }
	  else if (CONSTANT_P (XEXP (addr, 1)))
	    {
	      base = XEXP (addr, 0);
	      offset = XEXP (addr, 1);
	    }
	}

      if (offset == 0)
	{
	  base = addr;
	  offset = const0_rtx;
	}
      if (GET_CODE (offset) == CONST)
	offset = XEXP (offset, 0);
      if (GET_CODE (offset) == PLUS)
	{
	  if (GET_CODE (XEXP (offset, 0)) == CONST_INT)
	    {
	      base = gen_rtx_PLUS (GET_MODE (base), base, XEXP (offset, 1));
	      offset = XEXP (offset, 0);
	    }
	  else if (GET_CODE (XEXP (offset, 1)) == CONST_INT)
	    {
	      base = gen_rtx_PLUS (GET_MODE (base), base, XEXP (offset, 0));
	      offset = XEXP (offset, 1);
	    }
	  else
	    {
	      base = gen_rtx_PLUS (GET_MODE (base), base, offset);
	      offset = const0_rtx;
	    }
	}
      else if (GET_CODE (offset) != CONST_INT)
	{
	  base = gen_rtx_PLUS (GET_MODE (base), base, offset);
	  offset = const0_rtx;
	}

      if (all_const && GET_CODE (base) == PLUS)
	base = gen_rtx_CONST (GET_MODE (base), base);

      if (GET_CODE (offset) != CONST_INT)
	abort ();

      val.start = INTVAL (offset);
      val.end = val.start + GET_MODE_SIZE (GET_MODE (x));
      val.base = base;
      return val;
    }
  else if (GET_CODE (x) == REG)
    {
      val.reg_flag = 1;
      val.start = REGNO (x);
      if (val.start >= FIRST_PSEUDO_REGISTER)
	{
	  /* A pseudo with no hard reg.  */
	  val.start = REGNO (x);
	  val.end = val.start + 1;
	}
      else
	/* A hard reg.  */
	val.end = val.start + HARD_REGNO_NREGS (val.start, GET_MODE (x));
    }
  else if (GET_CODE (x) == SUBREG)
    {
      if (GET_CODE (SUBREG_REG (x)) != REG)
	/* This could be more precise, but it's good enough.  */
	return pre_reload_decompose (SUBREG_REG (x));
      val.reg_flag = 1;
      val.start = REGNO (SUBREG_REG (x));
      if (val.start >= FIRST_PSEUDO_REGISTER)
	return pre_reload_decompose (SUBREG_REG (x));
      else
	/* A hard reg.  */
	val.end = val.start + HARD_REGNO_NREGS (val.start, GET_MODE (x));
    }
  else if (CONSTANT_P (x)
	   /* This hasn't been assigned yet, so it can't conflict yet.  */
	   || GET_CODE (x) == SCRATCH)
    val.safe = 1;
  else
    abort ();
  return val;
}

/* Return 1 if altering Y will not modify the value of X.
   Y is also described by YDATA, which should be decompose (Y).  */

static int
pre_reload_immune_p (x, y, ydata)
     rtx x, y;
     struct decomposition ydata;
{
  struct decomposition xdata;

  if (ydata.reg_flag)
    return !refers_to_regno_p (ydata.start, ydata.end, x, (rtx*)0);
  if (ydata.safe)
    return 1;

  if (GET_CODE (y) != MEM)
    abort ();
  /* If Y is memory and X is not, Y can't affect X.  */
  if (GET_CODE (x) != MEM)
    return 1;

  xdata = pre_reload_decompose (x);

  if (! rtx_equal_p (xdata.base, ydata.base))
    {
      /* If bases are distinct symbolic constants, there is no overlap.  */
      if (CONSTANT_P (xdata.base) && CONSTANT_P (ydata.base))
	return 1;
      /* Constants and stack slots never overlap.  */
      if (CONSTANT_P (xdata.base)
	  && (ydata.base == frame_pointer_rtx
	      || ydata.base == hard_frame_pointer_rtx
	      || ydata.base == stack_pointer_rtx))
	return 1;
      if (CONSTANT_P (ydata.base)
	  && (xdata.base == frame_pointer_rtx
	      || xdata.base == hard_frame_pointer_rtx
	      || xdata.base == stack_pointer_rtx))
	return 1;
      /* If either base is variable, we don't know anything.  */
      return 0;
    }

  return (xdata.start >= ydata.end || ydata.start >= xdata.end);
}

/* Scan X for memory references and scan the addresses for reloading.
   Also checks for references to "constant" regs that we want to eliminate
   and replaces them with the values they stand for.
   We may alter X destructively if it contains a reference to such.
   If X is just a constant reg, we return the equivalent value
   instead of X.

   IND_LEVELS says how many levels of indirect addressing this machine
   supports.

   OPNUM and TYPE identify the purpose of the reload.

   IS_SET_DEST is true if X is the destination of a SET, which is not
   appropriate to be replaced by a constant.

   INSN, if nonzero, is the insn in which we do the reload.  It is used
   to determine if we may generate output reloads, and where to put USEs
   for pseudos that we have to replace with stack slots.

   ADDRESS_RELOADED.  If nonzero, is a pointer to where we put the
   result of find_reloads_address.  */

static rtx
find_pre_reloads_toplev (x, opnum, type, ind_levels, is_set_dest, insn,
			 address_reloaded)
     rtx x;
     int opnum;
     enum reload_type type;
     int ind_levels;
     int is_set_dest;
     rtx insn;
     int *address_reloaded;
{
  RTX_CODE code = GET_CODE (x);

  const char *fmt = GET_RTX_FORMAT (code);
  int i;
  int copied;

  if (code == REG)
    {
      return x;
    }
  if (code == MEM)
    {
      rtx tem = x;
      /* FIXME: denisc@overta.ru We don't use find_reloads_address now.  */
      /*
      i = find_reloads_address (GET_MODE (x), &tem, XEXP (x, 0), &XEXP (x, 0),
				opnum, type, ind_levels, insn);
      if (address_reloaded)
        *address_reloaded = i;
      */
      
      return tem;
    }

  if (code == SUBREG && GET_CODE (SUBREG_REG (x)) == MEM
      && (GET_MODE_SIZE (GET_MODE (x))
	  > GET_MODE_SIZE (GET_MODE (SUBREG_REG (x))))
      && mode_dependent_address_p (XEXP (SUBREG_REG (x), 0)))
    {
      /* A paradoxical subreg will simply have the mode of the access
	 changed, so we need to reload such a memory operand to stabilize
	 the meaning of the memory access.  */
      enum machine_mode subreg_mode = GET_MODE (SUBREG_REG (x));

      /* SUBREG_REG (x) is a MEM, so we cant take the offset, instead we 
         calculate the register number as : 
	 SUBREG_BYTE (x) / GET_MODE_SIZE (subreg_mode) */
      if (is_set_dest)
	push_pre_reload (NULL_RTX, SUBREG_REG (x), (rtx*)0, &SUBREG_REG (x),
			 find_valid_class (subreg_mode, 
					   SUBREG_BYTE (x)
					   / GET_MODE_SIZE (subreg_mode),
					   REGNO (SUBREG_REG (x))),
			 VOIDmode, subreg_mode, 0, 0, opnum, type);
      else
	push_pre_reload (SUBREG_REG (x), NULL_RTX, &SUBREG_REG (x), (rtx*)0,
			 find_valid_class (subreg_mode,
					   SUBREG_BYTE (x)
					   / GET_MODE_SIZE (subreg_mode),
					   REGNO (SUBREG_REG (x))),
			 subreg_mode, VOIDmode, 0, 0, opnum, type);
    }

  for (copied = 0, i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  rtx new_part = find_pre_reloads_toplev (XEXP (x, i), opnum, type,
						  ind_levels, is_set_dest,
						  insn, address_reloaded);
	  /* If we have replaced a reg with it's equivalent memory loc -
	     that can still be handled here e.g. if it's in a paradoxical
	     subreg - we must make the change in a copy, rather than using
	     a destructive change.  This way, find_reloads can still elect
	     not to do the change.  */
	  if (new_part != XEXP (x, i) && ! CONSTANT_P (new_part) && ! copied)
	    {
	      x = shallow_copy_rtx (x);
	      copied = 1;
	    }
	  XEXP (x, i) = new_part;
	}
    }
  return x;
}

/* Return 1 iff OPERAND (assumed to be a REG rtx)
   is a pseudo reg in class CLASS and have a mode MODE
   If REG will occupies multiple hard regs, all of them must be in CLASS.  */

static int
pseudo_fits_class_p (operand, class, mode)
     rtx operand ATTRIBUTE_UNUSED;
     enum reg_class class;
     enum machine_mode mode;
{
  static int max_consecutive[FIRST_PSEUDO_REGISTER];
  int i;
  int num_consecutive = 0;
  int clr = -1;

  if (!max_consecutive[class])
    {
      for (i = 0; i < FIRST_PSEUDO_REGISTER; ++i)
	if (TEST_HARD_REG_BIT (reg_class_contents[(int) class], i)
	    && i - clr > num_consecutive)
	  num_consecutive = i - clr;
	else
	  clr = i;
      max_consecutive[class] = num_consecutive;
    }
  
  return CLASS_MAX_NREGS (class, mode) <= max_consecutive[class];
}

struct scan_addr_state
{
  rtx insn;			/* insn which contain register */
  int opno;			/* operand number in insn */
  int alt;			/* selected constraints alternative  */
  ra_ref **defs;		/* defs ra_ref's for insn */
  ra_ref **uses;		/* uses ra_ref's for insn */
  char *constraints;
  int modified;			/* Flag for auto modified addresses */
  enum reg_class class;		/* Register class */
  struct ra_info *ra_info;
};

static int scan_addr_func            PARAMS ((rtx *,
					      struct scan_addr_state *));
static ra_ref * scan_addr_create_ref PARAMS ((struct ra_info *, rtx *,
					      struct scan_addr_state *,
					      enum ra_ref_type));

/* Quick hack for scan address registers preferences.
   FIXME: Must be substituted by define_address.  */

static int
scan_addr_func (loc, scan_state)
     rtx *loc;
     struct scan_addr_state *scan_state;
{
  switch (GET_CODE (*loc))
    {
    case REG:
      {
	ra_ref *ref;
	enum ra_ref_type t;
	t = RA_REF_ADDRESS | RA_REF_READ;
	ref = scan_addr_create_ref (scan_state->ra_info, loc, scan_state, t);
	scan_state->class = INDEX_REG_CLASS;
      }
      break;
      
    case PLUS:
    case MINUS:
      {
	rtx x1 = XEXP (*loc, 1);
	rtx x0 = XEXP (*loc, 0);
      
	if (GET_CODE (x1) == SUBREG)
	  x1 = SUBREG_REG (x1);
	if (GET_CODE (x0) == SUBREG)
	  x0 = SUBREG_REG (x0);

	if (REG_P (x1) && ! REG_P (x0))
	  {
	    scan_addr_func (&XEXP (*loc, 1), scan_state);
	    for_each_rtx (&XEXP (*loc, 0), (rtx_function) scan_addr_func,
			  scan_state);
	    return -1;
	  }
      }
      break;
      
    case PRE_DEC:
    case PRE_INC:
    case POST_DEC:
    case POST_INC:
      {
	rtx *loc0 = &XEXP (*loc, 0);

	if (GET_CODE (*loc0) == SUBREG)
	  loc0 = &SUBREG_REG (*loc0);
	
	if (REG_P (*loc0))
	  {
	    ra_ref *ref;
	    ref = scan_addr_create_ref (scan_state->ra_info, loc0, scan_state,
					RA_REF_ADDRESS | RA_REF_RDWR);
	    ref->class = BASE_REG_CLASS;
	    return -1;
	  }
      }
      break;
      
    case PRE_MODIFY:
    case POST_MODIFY:
      {
	rtx *loc0 = &XEXP (*loc, 0);

	if (GET_CODE (*loc0) == SUBREG)
	  loc0 = &SUBREG_REG (*loc0);

	if (REG_P (*loc0))
	  {
	    ra_ref *ref;
	    ref = scan_addr_create_ref (scan_state->ra_info,
					loc0, scan_state,
					RA_REF_ADDRESS | RA_REF_WRITE);
	    scan_state->class = BASE_REG_CLASS;
	    for_each_rtx (&XEXP (*loc, 1), (rtx_function) scan_addr_func,
			  scan_state);
	    return 1;
	  }
      }
      break;

    default:
      break;
    }

  return 0;
}

/* Helper for scan_addr_func. Create, fill and return re_ref structure.  */

static ra_ref *
scan_addr_create_ref (ra_info, loc, scan_state, ref_type)
     struct ra_info *ra_info;
     rtx *loc;
     struct scan_addr_state *scan_state;
     enum ra_ref_type ref_type;
{
  ra_ref *ref = (ra_ref *)obstack_alloc (&ra_info->obstack, sizeof (ra_ref));
  ref->type = ref_type;
  ref->reg = *loc;
  ref->loc = loc;
  ref->insn = scan_state->insn;
  ref->opno = scan_state->opno;
  ref->operand = recog_data.operand[scan_state->opno];
  ref->alt = scan_state->alt;
  ref->matches = NULL;
  ref->matched = NULL;
  ref->class = scan_state->class;
  if (RA_REF_REGNO (ref) < FIRST_PSEUDO_REGISTER)
    {
      CLEAR_HARD_REG_SET (ref->hardregs);
      SET_HARD_REG_BIT (ref->hardregs, RA_REF_REGNO (ref));
    }
  else
    COPY_HARD_REG_SET (ref->hardregs, reg_class_contents[ref->class]);
  ref->constraints = scan_state->constraints;

  if (RA_REF_WRITE_P (ref))
    *scan_state->defs++ = ref;
  if (RA_REF_READ_P (ref))
    *scan_state->uses++ = ref;
  return ref;
}

static int
pre_operands_match_p (x, y)
     rtx x, y;
{
  rtx s = (GET_CODE (x) == SUBREG) ? x : y;
  rtx t = (s == x) ? y : x;
  /* As we are working on pseudos, which only have one mode,
     we can't easily prereload operands which must match, but have different
     modes.  This should happen only for scratches, so simply take them
     as matching.  */ 
/*  if (GET_MODE (x) != GET_MODE (y)
      && (GET_CODE (x) == SCRATCH || GET_CODE (y) == SCRATCH))
    return 1;*/
  /* Both must be regs.  */
  if (REG_P (s))
    return s == t;
  /* Both are subregs of regs.  */
  if (GET_CODE (t) == SUBREG && REG_P (SUBREG_REG (s))
      && REG_P (SUBREG_REG (t)))
    return SUBREG_REG (s) == SUBREG_REG (t);
  if (GET_CODE (s) == SUBREG && REG_P (SUBREG_REG (s)) && REG_P (t))
    return SUBREG_REG (s) == t;
/*  if (GET_CODE (x) == SUBREG && GET_CODE (y) == SUBREG
      && GET_CODE (SUBREG_REG (x)) == REG && GET_CODE (SUBREG_REG (y)) == REG
      && REGNO (SUBREG_REG (x)) == REGNO (SUBREG_REG (y)))
    return 1;*/
  return operands_match_p (x, y);
}


struct alternative_info
{
  int class;			/* this_alternative */
  char match_win;		/* this_alternative_match_win */
  char win;			/* this_alternative_win */
  char offmemok;		/* this_alternative_offmemok */
  char earlyclobber;		/* this_alternative_earlyclobber */
  int matches;			/* this_alternative_matches */
  int matched;			/* goal_alternative_matched */
  rtx reg;			/* this_alternative_reg */
  rtx *reg_loc;			/* this_alternative_reg_loc */
  char *constraints;		/* this_alternative_constraints */
  int address_operand;		/* this_alternative_address_operand */
};

enum reload_usage { RELOAD_READ, RELOAD_READ_WRITE, RELOAD_WRITE };
static int scan_alternative PARAMS ((struct alternative_info [], char *[],
				     enum reload_usage [], int [],
				     char [MAX_RECOG_OPERANDS]
				     [MAX_RECOG_OPERANDS],
				     int, int *));

/* Scan one alternative and fill alternative info.  */

static int
scan_alternative (this_alt, constraints, modified, address_reloaded,
		  operands_match, swapped, commut)
     struct alternative_info this_alt[];
     char *constraints[];
     enum reload_usage modified[];
     int address_reloaded[];
     char operands_match[MAX_RECOG_OPERANDS][MAX_RECOG_OPERANDS];
     int swapped;
     int *commut;
{
  int i;
  int j;
  int noperands;
  int losers = 0;
  /* BAD is set to 1 if it some operand can't fit this alternative
     even after reloading.  */
  int bad = 0;
  /* REJECT is a count of how undesirable this alternative says it is
     if any reloading is required.  If the alternative matches exactly
     then REJECT is ignored, but otherwise it gets this much
     counted against it in addition to the reloading needed.  Each
     ? counts three times here since we want the disparaging caused by
     a bad register class to only count 1/3 as much.  */
  int reject = 0;
  enum machine_mode *operand_mode = recog_data.operand_mode;
  int commutative = *commut;
    
  noperands = recog_data.n_operands;
  
  for (i = 0; i < noperands; i++)
    {
      struct alternative_info *op_alt = &this_alt[i];
      char *p = constraints[i];
      int win = 0;
      int did_match = 0;
      /* 0 => this operand can be reloaded somehow for this
	 alternative.  */
      int badop = 1;
      /* 0 => this operand can be reloaded if the alternative
	 allows regs.  */
      int winreg = 0;
      int c;
      rtx operand = recog_data.operand[i];
      rtx *operand_loc = recog_data.operand_loc[i];
      int offset = 0;
      /* Nonzero means this is a MEM that must be reloaded into a reg
	 regardless of what the constraint says.  */
      int force_reload = 0;
      int offmemok = 0;
      /* Nonzero if a constant forced into memory would be OK for this
	 operand.  */
      int constmemok = 0;
      int earlyclobber = 0;
      int match_seen = 0;

      /* If the predicate accepts a unary operator, it means that
	 we need to reload the operand, but do not do this for
	 match_operator and friends.  */
      if (GET_RTX_CLASS (GET_CODE (operand)) == '1' && *p != 0)
	{
	  operand = XEXP (operand, 0);
	  operand_loc = &XEXP (operand, 0);
	}

      /* If the operand is a SUBREG, extract
	 the REG or MEM (or maybe even a constant) within.
	 (Constants can occur as a result of reg_equiv_constant.)  */

      while (GET_CODE (operand) == SUBREG)
	{
	  /* Offset only matters when operand is a REG and
	     it is a hard reg.  This is because it is passed
	     to reg_fits_class_p if it is a REG and all pseudos
	     return 0 from that function.  */
	  if (REG_P (SUBREG_REG (operand))
	      && REGNO (SUBREG_REG (operand)) < FIRST_PSEUDO_REGISTER)
	    {
	      offset +=
		subreg_regno_offset (REGNO (SUBREG_REG (operand)),
				     GET_MODE (SUBREG_REG (operand)),
				     SUBREG_BYTE (operand),
				     GET_MODE (operand));
	    }
	  operand = SUBREG_REG (operand);
	  operand_loc = &SUBREG_REG (operand);
	  /* Force reload if this is a constant or PLUS or if there may
	     be a problem accessing OPERAND in the outer mode.  */
	  if (CONSTANT_P (operand)
	      || GET_CODE (operand) == PLUS
	      /* We must force a reload of paradoxical SUBREGs
		 of a MEM because the alignment of the inner value
		 may not be enough to do the outer reference.  On
		 big-endian machines, it may also reference outside
		 the object.

		 On machines that extend byte operations and we have a
		 SUBREG where both the inner and outer modes are no wider
		 than a word and the inner mode is narrower, is integral,
		 and gets extended when loaded from memory, combine.c has
		 made assumptions about the behavior of the machine in such
		 register access.  If the data is, in fact, in memory we
		 must always load using the size assumed to be in the
		 register and let the insn do the different-sized
		 accesses.

		 This is doubly true if WORD_REGISTER_OPERATIONS.  In
		 this case eliminate_regs has left non-paradoxical
		 subregs for push_reloads to see.  Make sure it does
		 by forcing the reload.

		 ??? When is it right at this stage to have a subreg
		 of a mem that is _not_ to be handled specialy?  IMO
		 those should have been reduced to just a mem.  */
	      || ((GET_CODE (operand) == MEM
		   || (0
		       && REG_P (operand)
		       && REGNO (operand) >= FIRST_PSEUDO_REGISTER))
#ifndef WORD_REGISTER_OPERATIONS
		  && (((GET_MODE_BITSIZE (GET_MODE (operand))
			< BIGGEST_ALIGNMENT)
		       && (GET_MODE_SIZE (operand_mode[i])
			   > GET_MODE_SIZE (GET_MODE (operand))))
		      || (GET_CODE (operand) == MEM && BYTES_BIG_ENDIAN)
#ifdef LOAD_EXTEND_OP
		      || (GET_MODE_SIZE (operand_mode[i]) <= UNITS_PER_WORD
			  && (GET_MODE_SIZE (GET_MODE (operand))
			      <= UNITS_PER_WORD)
			  && (GET_MODE_SIZE (operand_mode[i])
			      > GET_MODE_SIZE (GET_MODE (operand)))
			  && INTEGRAL_MODE_P (GET_MODE (operand))
			  && LOAD_EXTEND_OP (GET_MODE (operand)) != NIL)
#endif
		      )
#endif
		  )
	      )
	    force_reload = 1;
	}

      op_alt->class = (int) NO_REGS;
      op_alt->win = 0;
      op_alt->match_win = 0;
      op_alt->offmemok = 0;
      op_alt->earlyclobber = 0;
      op_alt->matches = -1;
      op_alt->reg = NULL_RTX;
      op_alt->reg_loc = NULL;
      op_alt->constraints = p;
      op_alt->address_operand = 0;
	  
      /* An empty constraint or empty alternative
	 allows anything which matched the pattern.  */
      if (*p == 0 || *p == ',')
	{
	  win = 1, badop = 0;
	  if (REG_P (operand))
	    {
	      op_alt->reg = operand;
	      op_alt->reg_loc = operand_loc;
	      op_alt->class = GENERAL_REGS;
	    }
	}

      /* Scan this alternative's specs for this operand;
	 set WIN if the operand fits any letter in this alternative.
	 Otherwise, clear BADOP if this operand could
	 fit some letter after reloads,
	 or set WINREG if this operand could fit after reloads
	 provided the constraint allows some registers.  */

      match_seen = 0;
      while (*p && (c = *p++) != ',')
	switch (c)
	  {
	  case '=':  case '+':
	    break;
		
	  case '*':
	    reject += 4;
	    break;

	  case '%':
	    /* The last operand should not be marked commutative.  */
	    if (i != noperands - 1)
	      *commut = commutative = i;
	    break;

	  case '?':
	    reject += 6;
	    break;

	  case '!':
	    reject = 600;
	    break;

	  case '#':
	    /* Ignore rest of this alternative as far as
	       reloading is concerned.  */
	    if (match_seen)
	      while (*p && *p != ',')
		p++;
	    break;

	  case '0':  case '1':  case '2':  case '3':  case '4':
	  case '5':  case '6':  case '7':  case '8':  case '9':
	    {
	      struct alternative_info *mop_alt;
	      match_seen = 1;
	      c -= '0';
	      op_alt->matches = c;
	      mop_alt = &this_alt[c];
	      /* We are supposed to match a previous operand.
		 If we do, we win if that one did.
		 If we do not, count both of the operands as losers.
		 (This is too conservative, since most of the time
		 only a single reload insn will be needed to make
		 the two operands win.  As a result, this alternative
		 may be rejected when it is actually desirable.)  */
	      if ((swapped && (c != commutative || i != commutative + 1))
		  /* If we are matching as if two operands were swapped,
		     also pretend that operands_match had been computed
		     with swapped.
		     But if I is the second of those and C is the first,
		     don't exchange them, because operands_match is valid
		     only on one side of its diagonal.  */
		  ? (operands_match
		     [(c == commutative || c == commutative + 1)
		     ? 2 * commutative + 1 - c : c]
		     [(i == commutative || i == commutative + 1)
		     ? 2 * commutative + 1 - i : i])
		  : operands_match[c][i])
		{
		  /* If we are matching a non-offsettable address where an
		     offsettable address was expected, then we must reject
		     this combination, because we can't reload it.  */
		  if (mop_alt->offmemok
		      && GET_CODE (recog_data.operand[c]) == MEM
		      && mop_alt->class == (int) NO_REGS
		      && ! mop_alt->win)
		    bad = 1;

		  did_match = mop_alt->win;
		}
	      else
		{
		  /* Retroactively mark the operand we had to match
		     as a loser, if it wasn't already.  */
		  if (mop_alt->win)
		    losers++;
		  mop_alt->win = 0;
		  if (mop_alt->class == (int) NO_REGS)
		    bad = 1;
		}
	      /* This can be fixed with reloads if the operand
		 we are supposed to match can be fixed with reloads.  */
	      badop = 0;
	      op_alt->class = mop_alt->class;

	      /* If we have to reload this operand and some previous
		 operand also had to match the same thing as this
		 operand, we don't know how to do that.  So reject this
		 alternative.  */
	      if (! did_match || force_reload)
		for (j = 0; j < i; j++)
		  if (this_alt[j].matches
		      == op_alt->matches)
		    badop = 1;

	      if (REG_P (operand))
		{
		  op_alt->reg = operand;
		  op_alt->reg_loc = operand_loc;
		}
	    }
	    break;

	  case 'p':
	    /* All necessary reloads for an address_operand
	       were handled in find_reloads_address.  */
	    op_alt->class = (int) BASE_REG_CLASS;
	    op_alt->address_operand = 1;
	    win = 1;
	    break;

	  case 'm':
	    if (force_reload)
	      break;
	    if (GET_CODE (operand) == MEM)
	      win = 1;
	    /* force_const_mem does not accept HIGH.  */
	    if (CONSTANT_P (operand)
		&& GET_CODE (operand) != HIGH)
	      badop = 0;
	    constmemok = 1;
	    break;

	  case '<':
	    if (GET_CODE (operand) == MEM
		&& ! address_reloaded[i]
		&& (GET_CODE (XEXP (operand, 0)) == PRE_DEC
		    || GET_CODE (XEXP (operand, 0)) == POST_DEC))
	      win = 1;
	    break;

	  case '>':
	    if (GET_CODE (operand) == MEM
		&& ! address_reloaded[i]
		&& (GET_CODE (XEXP (operand, 0)) == PRE_INC
		    || GET_CODE (XEXP (operand, 0)) == POST_INC))
	      win = 1;
	    break;

	    /* Memory operand whose address is not offsettable.  */
	  case 'V':
	    if (force_reload)
	      break;
	    if (GET_CODE (operand) == MEM
		&& ! offsettable_nonstrict_memref_p (operand))
	      win = 1;
	    break;

	    /* Memory operand whose address is offsettable.  */
	  case 'o':
	    if (force_reload)
	      break;
	    if (GET_CODE (operand) == MEM
		/* If IND_LEVELS, find_reloads_address won't reload a
		   pseudo that didn't get a hard reg, so we have to
		   reject that case.  */
		&& (offsettable_nonstrict_memref_p (operand)
		    /* A reloaded address is offsettable because it is now
		       just a simple register indirect.  */
		    || address_reloaded[i]))
	      win = 1;
	    /* force_const_mem does not accept HIGH.  */
	    if ((CONSTANT_P (operand) && GET_CODE (operand) != HIGH)
		|| GET_CODE (operand) == MEM)
	      badop = 0;
	    constmemok = 1;
	    offmemok = 1;
	    break;

	  case '&':
	    /* Output operand that is stored before the need for the
	       input operands (and their index registers) is over.  */
	    earlyclobber = 1;
	    break;

	  case 'E':
#ifndef REAL_ARITHMETIC
	    /* Match any floating double constant, but only if
	       we can examine the bits of it reliably.  */
	    if ((HOST_FLOAT_FORMAT != TARGET_FLOAT_FORMAT
		 || HOST_BITS_PER_WIDE_INT != BITS_PER_WORD)
		&& GET_MODE (operand) != VOIDmode && ! flag_pretend_float)
	      break;
#endif
	    if (GET_CODE (operand) == CONST_DOUBLE)
	      win = 1;
	    break;

	  case 'F':
	    if (GET_CODE (operand) == CONST_DOUBLE)
	      win = 1;
	    break;

	  case 'G':
	  case 'H':
	    if (GET_CODE (operand) == CONST_DOUBLE
		&& CONST_DOUBLE_OK_FOR_LETTER_P (operand, c))
	      win = 1;
	    break;

	  case 's':
	    if (GET_CODE (operand) == CONST_INT
		|| (GET_CODE (operand) == CONST_DOUBLE
		    && GET_MODE (operand) == VOIDmode))
	      break;
	  case 'i':
	    if (CONSTANT_P (operand)
#ifdef LEGITIMATE_PIC_OPERAND_P
		&& (! flag_pic || LEGITIMATE_PIC_OPERAND_P (operand))
#endif
		)
	      win = 1;
	    break;

	  case 'n':
	    if (GET_CODE (operand) == CONST_INT
		|| (GET_CODE (operand) == CONST_DOUBLE
		    && GET_MODE (operand) == VOIDmode))
	      win = 1;
	    break;

	  case 'I':
	  case 'J':
	  case 'K':
	  case 'L':
	  case 'M':
	  case 'N':
	  case 'O':
	  case 'P':
	    if (GET_CODE (operand) == CONST_INT
		&& CONST_OK_FOR_LETTER_P (INTVAL (operand), c))
	      win = 1;
	    break;

	  case 'X':
	    win = 1;
	    break;

	  case 'g':
	    if (! force_reload
		/* A PLUS is never a valid operand, but reload can make
		   it from a register when eliminating registers.  */
		&& GET_CODE (operand) != PLUS
		/* A SCRATCH is not a valid operand.  */
		&& GET_CODE (operand) != SCRATCH
#ifdef LEGITIMATE_PIC_OPERAND_P
		&& (! CONSTANT_P (operand)
		    || ! flag_pic
		    || LEGITIMATE_PIC_OPERAND_P (operand))
#endif
		&& (GENERAL_REGS == ALL_REGS
		    || GET_CODE (operand) != REG))
	      win = 1;
	    /* Drop through into 'r' case.  */

	  case 'r':
	    op_alt->class = (int) (reg_class_subunion
				   [op_alt->class]
				   [(int) GENERAL_REGS]);
	    goto reg;

	  default:
	    if (REG_CLASS_FROM_LETTER (c) == NO_REGS)
	      {
#ifdef EXTRA_CONSTRAINT
		if (EXTRA_CONSTRAINT (operand, c))
		  win = 1;
#endif
		break;
	      }
	    /* FIXME: denisc@overta.ru Better to use union of contents
	       of classes.  */
	    op_alt->class
	      = (int) (reg_class_subunion
		       [op_alt->class]
		       [(int) REG_CLASS_FROM_LETTER (c)]);
	  reg:
	    if (GET_MODE (operand) == BLKmode)
	      break;
	    winreg = 1;
	    if (REG_P (operand))
	      {
		op_alt->reg_loc = operand_loc;
		op_alt->reg = operand;
		if (REGNO (operand) >= FIRST_PSEUDO_REGISTER)
		  win = pseudo_fits_class_p (operand, op_alt->class,
					     GET_MODE (operand)) ;
		else if (reg_fits_class_p (operand, op_alt->class,
					   offset, GET_MODE (operand)))
		  win = 1;
	      }
	    break;
	  }

      constraints[i] = p;

      /* If this operand could be handled with a reg,
	 and some reg is allowed, then this operand can be handled.  */
      if (winreg && op_alt->class != (int) NO_REGS)
	badop = 0;

      /* Record which operands fit this alternative.  */
      op_alt->earlyclobber = earlyclobber;
      if (win && ! force_reload)
	op_alt->win = 1;
      else if (did_match && ! force_reload)
	op_alt->match_win = 1;
      else
	{
	  int const_to_mem = 0;

	  op_alt->offmemok = offmemok;
	  losers++;
	  if (badop)
	    bad = 1;
	  /* Alternative loses if it has no regs for a reg operand.  */
	  if (REG_P (operand)
	      && op_alt->class == (int) NO_REGS
	      && op_alt->matches < 0)
	    bad = 1;

	  /* If this is a constant that is reloaded into the desired
	     class by copying it to memory first, count that as another
	     reload.  This is consistent with other code and is
	     required to avoid choosing another alternative when
	     the constant is moved into memory by this function on
	     an early reload pass.  Note that the test here is
	     precisely the same as in the code below that calls
	     force_const_mem.  */
	  if (CONSTANT_P (operand)
	      /* force_const_mem does not accept HIGH.  */
	      && GET_CODE (operand) != HIGH
	      && (PREFERRED_RELOAD_CLASS (operand,
					  (enum reg_class) op_alt->class)
		  == NO_REGS)
	      && operand_mode[i] != VOIDmode)
	    {
	      const_to_mem = 1;
	      if (op_alt->class != (int) NO_REGS)
		losers++;
	    }

	  /* If we can't reload this value at all, reject this
	     alternative.  Note that we could also lose due to
	     LIMIT_RELOAD_RELOAD_CLASS, but we don't check that
	     here.  */

	  if (! CONSTANT_P (operand)
	      && (enum reg_class) op_alt->class != NO_REGS
	      && (PREFERRED_RELOAD_CLASS (operand,
					  (enum reg_class) op_alt->class)
		  == NO_REGS))
	    bad = 1;

	  /* We prefer to reload pseudos over reloading other things,
	     since such reloads may be able to be eliminated later.
	     If we are reloading a SCRATCH, we won't be generating any
	     insns, just using a register, so it is also preferred.
	     So bump REJECT in other cases.  Don't do this in the
	     case where we are forcing a constant into memory and
	     it will then win since we don't want to have a different
	     alternative match then.  */
	  if (! (REG_P (operand)
		 && (0
		     && REGNO (operand) >= FIRST_PSEUDO_REGISTER))
	      && GET_CODE (operand) != SCRATCH
	      && ! (const_to_mem && constmemok))
	    reject += 2;

	  /* Input reloads can be inherited more often than output
	     reloads can be removed, so penalize output reloads.  */
	  if (modified[i] != RELOAD_READ
	      && GET_CODE (operand) != SCRATCH)
	    reject++;
	}
    }

  /* Now see if any output operands that are marked "earlyclobber"
     in this alternative conflict with any input operands
     or any memory addresses.  */

  for (i = 0; i < noperands; i++)
    {
      struct alternative_info *op_alt = &this_alt[i];
      if (op_alt->earlyclobber
	  && (op_alt->win || op_alt->match_win))
	{
	  struct decomposition early_data;

	  early_data = pre_reload_decompose (recog_data.operand[i]);

	  if (modified[i] == RELOAD_READ)
	    abort ();

	  if (op_alt->class == NO_REGS)
	    {
	      op_alt->earlyclobber = 0;
	      if (this_insn_is_asm)
		error_for_asm (this_insn,
			       "`&' constraint used with no register class");
	      else
		abort ();
	    }

	  for (j = 0; j < noperands; j++)
	    /* Is this an input operand or a memory ref?  */
	    if ((GET_CODE (recog_data.operand[j]) == MEM
		 || modified[j] != RELOAD_WRITE)
		&& j != i
		/* Ignore things like match_operator operands.  */
		&& *recog_data.constraints[j] != 0
		/* Don't coutn an input operand that is constrained to match
		   the early clobber operand.  */
		&& ! (this_alt[j].matches == i
		      && rtx_equal_p (recog_data.operand[i],
				      recog_data.operand[j]))
		/* Is it altered by storing the earlyclobber operand?  */
		&& !pre_reload_immune_p (recog_data.operand[j],
					 recog_data.operand[i],
					 early_data))
	      {
		/* If the output is in a single-reg class,
		   it's costly to reload it, so reload the input instead.  */
		if (reg_class_size[op_alt->class] == 1
		    && (REG_P (recog_data.operand[j])
			|| GET_CODE (recog_data.operand[j]) == SUBREG))
		  {
		    losers++;
		    this_alt[j].win = 0;
		    this_alt[j].match_win = 0;
		  }
		else
		  break;
	      }
	  /* If an earlyclobber operand conflicts with something,
	     it must be reloaded, so request this and count the cost.  */
	  if (j != noperands)
	    {
	      losers++;
	      op_alt->win = 0;
	      this_alt[j].match_win = 0;
	      for (j = 0; j < noperands; j++)
		{
		  struct alternative_info * op = &this_alt[j];
		  if (op->matches == i && op->match_win)
		    {
		      op->win = 0;
		      op->match_win = 0;
		      losers++;
		    }
		}
	    }
	}
    }
      
  /* REJECT, set by the '!', '*' and '?' constraint characters and when
     a register would be reloaded into a non-preferred class, discourages
     the use of this alternative for a reload goal.  REJECT is incremented
     by six for each ? and two for each non-preferred class.  */
  losers = losers * 6 + reject;

  return bad ? -1 : losers;
}

/* Pre-reload INSN or collect all defs/uses for INSN.
   If n_reloads nonzero then INSN was reloaded and no defs/uses was
   callected.
   All collected defs/uses will be putted to DEF_REFS/USE_REFS plain arrays.
   N_DEFS will be set to count of defs and N_USES to count of uses.  */

static void
collect_insn_info (ra_info, insn, def_refs, use_refs, n_defs, n_uses)
     struct ra_info *ra_info;
     rtx insn;
     ra_ref **def_refs;
     ra_ref **use_refs;
     int *n_defs;
     int *n_uses;
{
  int insn_code_number;
  int i, j;
  int noperands;
  /* These start out as the constraints for the insn
     and they are chewed up as we consider alternatives.  */
  char *constraints[MAX_RECOG_OPERANDS];
  /* These are the preferred classes for an operand, or NO_REGS if it isn't
     a register.  */
  /* Nonzero for a MEM operand whose entire address needs a reload.  */
  int address_reloaded[MAX_RECOG_OPERANDS];
  /* Value of enum reload_type to use for operand.  */
  enum reload_type operand_type[MAX_RECOG_OPERANDS];
  /* Value of enum reload_type to use within address of operand.  */
  enum reload_type address_type[MAX_RECOG_OPERANDS];
  /* Save the usage of each operand.  */
  enum reload_usage modified[MAX_RECOG_OPERANDS];
  int n_alternatives;
  struct alternative_info this_alt[MAX_RECOG_OPERANDS];
  
  int this_alternative_number;
  int swapped;

  struct alternative_info goal_alt[MAX_RECOG_OPERANDS];
  
  int goal_alternative_number = 0;
  int operand_reloadnum[MAX_RECOG_OPERANDS];
  int goal_alternative_swapped;
  int best;
  int commutative;
  char operands_match[MAX_RECOG_OPERANDS][MAX_RECOG_OPERANDS];
  rtx substed_operand[MAX_RECOG_OPERANDS];
  rtx set = single_set (insn);
  enum machine_mode operand_mode[MAX_RECOG_OPERANDS];
  int ind_levels = indirect_levels;
  ra_ref **orig_def_refs = def_refs;
  ra_ref **orig_use_refs = use_refs;

  *n_defs = 0;
  *n_uses = 0;
  
  extract_insn (insn);

  noperands = reload_n_operands = recog_data.n_operands;
  n_alternatives = recog_data.n_alternatives;

  n_reloads = 0;
  n_replacements = 0;
  n_earlyclobbers = 0;

  /* Just return "no reloads" if insn has no operands with constraints.  */
  if (noperands == 0 || n_alternatives == 0)
    return;

  this_insn = insn;
  insn_code_number = INSN_CODE (insn);
  this_insn_is_asm = insn_code_number < 0;

  memcpy (operand_mode, recog_data.operand_mode,
	  noperands * sizeof (enum machine_mode));
  memcpy (constraints, recog_data.constraints, noperands * sizeof (char *));

  commutative = -1;

  /* If we will need to know, later, whether some pair of operands
     are the same, we must compare them now and save the result.
     Reloading the base and index registers will clobber them
     and afterward they will fail to match.  */

  for (i = 0; i < noperands; i++)
    {
      char *p;
      int c;

      substed_operand[i] = recog_data.operand[i];
      p = constraints[i];

      modified[i] = RELOAD_READ;

      /* Scan this operand's constraint to see if it is an output operand,
	 an in-out operand, is commutative, or should match another.  */

      while ((c = *p++))
	{
	  if (c == '=')
	    modified[i] = RELOAD_WRITE;
	  else if (c == '+')
	    modified[i] = RELOAD_READ_WRITE;
	  else if (c == '%')
	    {
	      /* The last operand should not be marked commutative.  */
	      if (i == noperands - 1)
		abort ();

	      commutative = i;
	    }
	  else if (c >= '0' && c <= '9')
	    {
	      rtx op1 = recog_data.operand[c - '0'];
	      rtx op2 = recog_data.operand[i];
	      c -= '0';
	      if (GET_MODE (op1) != GET_MODE (op2)
		  && INTEGRAL_MODE_P (GET_MODE (op1))
		  && INTEGRAL_MODE_P (GET_MODE (op2)))
		{
		  if (GET_MODE_SIZE (GET_MODE (op1))
		      < GET_MODE_SIZE (GET_MODE (op2)))
		    {
		      rtx t = op1;
		      op1 = op2;
		      op2 = t;
		    }
		  if (GET_CODE (op2) == SUBREG)
		    {
		      unsigned o = subreg_lowpart_offset (GET_MODE (op2),
							  GET_MODE (op1));
		      if (o == SUBREG_BYTE (op2))
			op2 = SUBREG_REG (op2);
		    }
		}
	      operands_match[c][i] = pre_operands_match_p (op1, op2);;

	      /* An operand may not match itself.  */
	      if (c == i)
		abort ();

	      /* If C can be commuted with C+1, and C might need to match I,
		 then C+1 might also need to match I.  */
	      if (commutative >= 0)
		{
		  if (c == commutative || c == commutative + 1)
		    {
		      int other = c + (c == commutative ? 1 : -1);
		      operands_match[other][i]
			= pre_operands_match_p (recog_data.operand[other],
					    recog_data.operand[i]);
		    }
		  if (i == commutative || i == commutative + 1)
		    {
		      int other = i + (i == commutative ? 1 : -1);
		      operands_match[c][other]
			= pre_operands_match_p (recog_data.operand[c],
					    recog_data.operand[other]);
		    }
		  /* Note that C is supposed to be less than I.
		     No need to consider altering both C and I because in
		     that case we would alter one into the other.  */
		}
	    }
	}
    }

  /* Examine each operand that is a memory reference or memory address
     and reload parts of the addresses into registers.

     Finally, set up the preferred classes of each operand.  */

  for (i = 0; i < noperands; i++)
    {
      RTX_CODE code = GET_CODE (recog_data.operand[i]);

      address_reloaded[i] = 0;
      operand_type[i] = (modified[i] == RELOAD_READ ? RELOAD_FOR_INPUT
			 : modified[i] == RELOAD_WRITE ? RELOAD_FOR_OUTPUT
			 : RELOAD_OTHER);
      address_type[i]
	= (modified[i] == RELOAD_READ ? RELOAD_FOR_INPUT_ADDRESS
	   : modified[i] == RELOAD_WRITE ? RELOAD_FOR_OUTPUT_ADDRESS
	   : RELOAD_OTHER);

      if (*constraints[i] == 0)
	/* Ignore things like match_operator operands.  */
	;
      else if (constraints[i][0] == 'p')
	{
	  /* FIXME: denisc@overta.ru We don't use reload_address now.
	     define_address must be used here.  */
#if 0
	  find_reloads_address (VOIDmode, (rtx*)0,
				recog_data.operand[i],
				recog_data.operand_loc[i],
				i, operand_type[i], ind_levels, insn);

	  /* If we now have a simple operand where we used to have a
	     PLUS or MULT, re-recognize and try again.  */
	  if ((GET_RTX_CLASS (GET_CODE (*recog_data.operand_loc[i])) == 'o'
	       || GET_CODE (*recog_data.operand_loc[i]) == SUBREG)
	      && (GET_CODE (recog_data.operand[i]) == MULT
		  || GET_CODE (recog_data.operand[i]) == PLUS))
	    {
	      INSN_CODE (insn) = -1;
	      retval = find_pre_reloads (insn, ind_levels);
	      return retval;
	    }

	  recog_data.operand[i] = *recog_data.operand_loc[i];
#endif
	  substed_operand[i] = recog_data.operand[i];
	}
      else if (code == MEM)
	{
	  /* FIXME: denisc@overta.ru We don't use relad_address now.
	     I must use define_address here. */
	  /*
	    address_reloaded[i]
	    = find_reloads_address (GET_MODE (recog_data.operand[i]),
	    recog_data.operand_loc[i],
	    XEXP (recog_data.operand[i], 0),
	    &XEXP (recog_data.operand[i], 0),
	    i, address_type[i], ind_levels, insn);
	    recog_data.operand[i] = *recog_data.operand_loc[i];
	  */
	  substed_operand[i] = recog_data.operand[i];
	}
      else if (code == SUBREG)
	{
	  rtx op
	    = find_pre_reloads_toplev (recog_data.operand[i], i,
				       address_type[i], ind_levels,
				       set != 0
				       && (&SET_DEST (set)
					   == recog_data.operand_loc[i]),
				       insn,
				       &address_reloaded[i]);

	  /* If we made a MEM to load (a part of) the stackslot of a pseudo
	     that didn't get a hard register, emit a USE with a REG_EQUAL
	     note in front so that we might inherit a previous, possibly
	     wider reload.  */

	  substed_operand[i] = recog_data.operand[i] = op;
	}
    }

  /* Now see what we need for pseudo-regs. For this, we must consider
     all the operands together against the register constraints.  */

  best = MAX_RECOG_OPERANDS * 2 + 600;

  swapped = 0;
  goal_alternative_swapped = 0;
 try_swapped:

  /* The constraints are made of several alternatives.
     Each operand's constraint looks like foo,bar,... with commas
     separating the alternatives.  The first alternatives for all
     operands go together, the second alternatives go together, etc.

     First loop over alternatives.  */

  for (this_alternative_number = 0;
       this_alternative_number < n_alternatives;
       this_alternative_number++)
    {
      /* Loop over operands for one constraint alternative.  */
      /* LOSERS counts those that don't fit this alternative
	 and would require loading.  */

      int losers = scan_alternative (this_alt, constraints, modified,
				     address_reloaded, operands_match,
				     swapped, &commutative);
      
      /* If this alternative can be made to work by reloading,
	 and it needs less reloading than the others checked so far,
	 record it as the chosen goal for reloading.  */
      if (losers >= 0 && best > losers)
	{
	  for (i = 0; i < noperands; i++)
	    memcpy (&goal_alt[i], &this_alt[i], sizeof (this_alt[0]));
	  goal_alternative_swapped = swapped;
	  goal_alternative_number = this_alternative_number;
	  best = losers;
	}
    }

  /* If insn is commutative (it's safe to exchange a certain pair of operands)
     then we need to try each alternative twice,
     the second time matching those two operands
     as if we had exchanged them.
     To do this, really exchange them in operands.

     If we have just tried the alternatives the second time,
     return operands to normal and drop through.  */

  if (commutative >= 0)
    {
      swapped = !swapped;
      if (swapped)
	{
	  recog_data.operand[commutative] = substed_operand[commutative + 1];
	  recog_data.operand[commutative + 1] = substed_operand[commutative];

	  memcpy (constraints, recog_data.constraints,
		  noperands * sizeof (char *));
	  goto try_swapped;
	}
      else
	{
	  recog_data.operand[commutative] = substed_operand[commutative];
	  recog_data.operand[commutative + 1]
	    = substed_operand[commutative + 1];
	}
    }

  /* The operands don't meet the constraints.
     goal_alternative describes the alternative
     that we could reach by reloading the fewest operands.
     Reload so as to fit it.  */

  if (best == MAX_RECOG_OPERANDS * 2 + 600)
    {
      /* No alternative works with reloads??  */
      if (insn_code_number >= 0)
	fatal_insn ("Unable to generate reloads for:", insn);
      error_for_asm (insn, "inconsistent operand constraints in an `asm'");
      /* Avoid further trouble with this insn.  */
      PATTERN (insn) = gen_rtx_USE (VOIDmode, const0_rtx);
      n_reloads = 0;
      return;
    }

  /* Right now, for any pair of operands I and J that are required to match,
     with I < J,
     goal_alternative_matches[J] is I.
     Set up goal_alternative_matched as the inverse function:
     goal_alternative_matched[I] = J.  */

  for (i = 0; i < noperands; i++)
    goal_alt[i].matched = -1;
 
  for (i = 0; i < noperands; i++)
    if (! goal_alt[i].win
	&& goal_alt[i].matches >= 0)
      goal_alt[goal_alt[i].matches].matched = i;

  for (i = 0; i < noperands; i++)
    goal_alt[i].win |= goal_alt[i].match_win;

  /* If the best alternative is with operands 1 and 2 swapped,
     consider them swapped before reporting the reloads.  Update the
     operand numbers of any reloads already pushed.  */

  if (goal_alternative_swapped)
    {
      rtx tem;

      tem = substed_operand[commutative];
      substed_operand[commutative] = substed_operand[commutative + 1];
      substed_operand[commutative + 1] = tem;
      tem = recog_data.operand[commutative];
      recog_data.operand[commutative] = recog_data.operand[commutative + 1];
      recog_data.operand[commutative + 1] = tem;
      tem = *recog_data.operand_loc[commutative];
      *recog_data.operand_loc[commutative]
	= *recog_data.operand_loc[commutative + 1];
      *recog_data.operand_loc[commutative + 1] = tem;

      for (i = 0; i < n_reloads; i++)
	{
	  if (rld[i].opnum == commutative)
	    rld[i].opnum = commutative + 1;
	  else if (rld[i].opnum == commutative + 1)
	    rld[i].opnum = commutative;
	}
    }

  for (i = 0; i < noperands; i++)
    {
      operand_reloadnum[i] = -1;

      /* If this is an earlyclobber operand, we need to widen the scope.
	 The reload must remain valid from the start of the insn being
	 reloaded until after the operand is stored into its destination.
	 We approximate this with RELOAD_OTHER even though we know that we
	 do not conflict with RELOAD_FOR_INPUT_ADDRESS reloads.

	 One special case that is worth checking is when we have an
	 output that is earlyclobber but isn't used past the insn (typically
	 a SCRATCH).  In this case, we only need have the reload live
	 through the insn itself, but not for any of our input or output
	 reloads.
	 But we must not accidentally narrow the scope of an existing
	 RELOAD_OTHER reload - leave these alone.

	 In any case, anything needed to address this operand can remain
	 however they were previously categorized.  */

      if (goal_alt[i].earlyclobber && operand_type[i] != RELOAD_OTHER)
	operand_type[i] = RELOAD_OTHER;
    }

  /* Any constants that aren't allowed and can't be reloaded
     into registers are here changed into memory references.  */
  for (i = 0; i < noperands; i++)
    if (! goal_alt[i].win
	&& CONSTANT_P (recog_data.operand[i])
	/* force_const_mem does not accept HIGH.  */
	&& GET_CODE (recog_data.operand[i]) != HIGH
	&& (PREFERRED_RELOAD_CLASS (recog_data.operand[i],
				    (enum reg_class) goal_alt[i].class)
	    == NO_REGS)
	&& operand_mode[i] != VOIDmode)
      {
	substed_operand[i] = recog_data.operand[i]
	  = find_pre_reloads_toplev (force_const_mem (operand_mode[i],
						      recog_data.operand[i]),
				     i, address_type[i], ind_levels, 0, insn,
				     NULL);
	if (alternative_allows_memconst (recog_data.constraints[i],
					 goal_alternative_number))
	  goal_alt[i].win = 1;
      }

  /* Record the values of the earlyclobber operands for the caller.  */
  for (i = 0; i < noperands; i++)
    if (goal_alt[i].earlyclobber)
      reload_earlyclobbers[n_earlyclobbers++] = recog_data.operand[i];

  /* Now record reloads for all the operands that need them.  */
  for (i = 0; i < noperands; i++)
    if (! goal_alt[i].win)
      {
	/* Operands that match previous ones have already been handled.  */
	if (goal_alt[i].matches >= 0)
	  ;
	/* Handle an operand with a nonoffsettable address
	   appearing where an offsettable address will do
	   by reloading the address into a base register.

	   ??? We can also do this when the operand is a register and
	   reg_equiv_mem is not offsettable, but this is a bit tricky,
	   so we don't bother with it.  It may not be worth doing.  */
	else if (goal_alt[i].matched == -1
		 && goal_alt[i].offmemok
		 && GET_CODE (recog_data.operand[i]) == MEM)
	  {
	    operand_reloadnum[i]
	      = push_pre_reload (XEXP (recog_data.operand[i], 0), NULL_RTX,
				 &XEXP (recog_data.operand[i], 0), (rtx*)0,
				 BASE_REG_CLASS, VOIDmode, VOIDmode,
				 0, 0, i, RELOAD_FOR_INPUT);
	    rld[operand_reloadnum[i]].inc
	      = GET_MODE_SIZE (GET_MODE (recog_data.operand[i]));

	    /* If this operand is an output, we will have made any
	       reloads for its address as RELOAD_FOR_OUTPUT_ADDRESS, but
	       now we are treating part of the operand as an input, so
	       we must change these to RELOAD_FOR_INPUT_ADDRESS.  */

	    if (modified[i] == RELOAD_WRITE)
	      {
		for (j = 0; j < n_reloads; j++)
		  {
		    if (rld[j].opnum == i)
		      {
			if (rld[j].when_needed == RELOAD_FOR_OUTPUT_ADDRESS)
			  rld[j].when_needed = RELOAD_FOR_INPUT_ADDRESS;
			else if (rld[j].when_needed
				 == RELOAD_FOR_OUTADDR_ADDRESS)
			  rld[j].when_needed = RELOAD_FOR_INPADDR_ADDRESS;
		      }
		  }
	      }
	  }
	else if (goal_alt[i].matched == -1)
	  {
	    operand_reloadnum[i]
	      = push_pre_reload ((modified[i] != RELOAD_WRITE
				  ? recog_data.operand[i] : 0),
				 (modified[i] != RELOAD_READ
				  ? recog_data.operand[i] : 0),
				 (modified[i] != RELOAD_WRITE
				  ? recog_data.operand_loc[i] : 0),
				 (modified[i] != RELOAD_READ
				  ? recog_data.operand_loc[i] : 0),
				 (enum reg_class) goal_alt[i].class,
				 (modified[i] == RELOAD_WRITE
				  ? VOIDmode : operand_mode[i]),
				 (modified[i] == RELOAD_READ
				  ? VOIDmode : operand_mode[i]),
				 (insn_code_number < 0 ? 0
				  : insn_data[insn_code_number].operand[i].strict_low),
				 0, i, operand_type[i]);
	  }
	/* In a matching pair of operands, one must be input only
	   and the other must be output only.
	   Pass the input operand as IN and the other as OUT.  */
	else if (modified[i] == RELOAD_READ
		 && modified[goal_alt[i].matched] == RELOAD_WRITE)
	  {
	    operand_reloadnum[i]
	      = push_pre_reload (recog_data.operand[i],
				 recog_data.operand [goal_alt[i].matched],
				 recog_data.operand_loc[i],
				 recog_data.operand_loc[goal_alt[i].matched],
				 (enum reg_class) goal_alt[i].class,
				 operand_mode[i],
				 operand_mode[goal_alt[i].matched],
				 0, 0, i, RELOAD_OTHER);
	    operand_reloadnum[goal_alt[i].matched] = output_reloadnum;
	  }
	else if (modified[i] == RELOAD_WRITE
		 && modified[goal_alt[i].matched] == RELOAD_READ)
	  {
	    operand_reloadnum[goal_alt[i].matched]
	      = push_pre_reload (recog_data.operand[goal_alt[i].matched],
				 recog_data.operand[i],
				 recog_data.operand_loc[goal_alt[i].matched],
				 recog_data.operand_loc[i],
				 (enum reg_class) goal_alt[i].class,
				 operand_mode[goal_alt[i].matched],
				 operand_mode[i],
				 0, 0, i, RELOAD_OTHER);
	    operand_reloadnum[i] = output_reloadnum;
	  }
	else if (insn_code_number >= 0)
	  abort ();
	else
	  {
	    error_for_asm (insn,
			   "inconsistent operand constraints in an `asm'");
	    /* Avoid further trouble with this insn.  */
	    PATTERN (insn) = gen_rtx_USE (VOIDmode, const0_rtx);
	    n_reloads = 0;
	    return;
	  }
      }

  /* Perform whatever substitutions on the operands we are supposed
     to make due to commutativity or replacement of registers
     with equivalent constants or memory slots.  */

  for (i = 0; i < noperands; i++)
    {
      /* We only do this on the last pass through reload, because it is
	 possible for some data (like reg_equiv_address) to be changed during
	 later passes.  Moreover, we loose the opportunity to get a useful
	 reload_{in,out}_reg when we do these replacements.  */

      rtx substitution = substed_operand[i];

      *recog_data.operand_loc[i] = substitution;

      /* If we're replacing an operand with a LABEL_REF, we need
	 to make sure that there's a REG_LABEL note attached to
	 this instruction.  */
      /*
      if (GET_CODE (insn) != JUMP_INSN
	  && GET_CODE (substitution) == LABEL_REF
	  && !find_reg_note (insn, REG_LABEL, XEXP (substitution, 0)))
	REG_NOTES (insn) = gen_rtx_EXPR_LIST (REG_LABEL,
					      XEXP (substitution, 0),
					      REG_NOTES (insn));
      */
    }

  /* If this insn pattern contains any MATCH_DUP's, make sure that
     they will be substituted if the operands they match are substituted.
     Also do now any substitutions we already did on the operands.

     Don't do this if we aren't making replacements because we might be
     propagating things allocated by frame pointer elimination into places
     it doesn't expect.  */

  if (insn_code_number >= 0)
    for (i = insn_data[insn_code_number].n_dups - 1; i >= 0; i--)
      {
	int opno = recog_data.dup_num[i];
	*recog_data.dup_loc[i] = *recog_data.operand_loc[opno];
	if (operand_reloadnum[opno] >= 0)
	  push_replacement (recog_data.dup_loc[i], operand_reloadnum[opno],
			    insn_data[insn_code_number].operand[opno].mode);
      }

  if (n_reloads)
    {
      for (i = 0; i < n_reloads; i++)
	if (rld[i].mode == VOIDmode)
	  {
	    if (!rld[i].out)
	      rld[i].mode = rld[i].inmode;
	    else if (!rld[i].in)
	      rld[i].mode = rld[i].outmode;
	    else
	      rld[i].mode = (GET_MODE_SIZE (rld[i].outmode)
			     > GET_MODE_SIZE (rld[i].inmode)
			     ? rld[i].outmode : rld[i].inmode);
	  }
      return;
    }
  
  /* This is a valid insn !!!
     Fill ra_ref structures for all regs and link them to ra_list.  */
  {
    ra_ref *opno2ref[MAX_RECOG_OPERANDS];
    
    for (i = 0; i < noperands; ++i)
      {
	opno2ref[i] = 0;
      
	if (GET_CODE (recog_data.operand[i]) == MEM
	    || goal_alt[i].address_operand)
	  {
	    /* FIXME: Now we can only record registers inside address. */
	    struct scan_addr_state scan_state;
	    scan_state.opno = i;
	    scan_state.insn = insn;
	    scan_state.defs = def_refs;
	    scan_state.uses = use_refs;
	    scan_state.alt = goal_alternative_number;
	    scan_state.constraints = goal_alt[i].constraints;
	    scan_state.modified = 0;
	    scan_state.class = BASE_REG_CLASS;
	    scan_state.ra_info = ra_info;
	    for_each_rtx (recog_data.operand_loc[i],
			  (rtx_function) scan_addr_func, &scan_state);
	    def_refs = scan_state.defs;
	    use_refs = scan_state.uses;
	  }
	else if (goal_alt[i].reg)
	  {
	    int matches = goal_alt[i].matches;
	  
	    ra_ref *ref = (ra_ref *) obstack_alloc (&ra_info->obstack,
						    sizeof (ra_ref));
	    opno2ref[i] = ref;
	    ref->reg = goal_alt[i].reg;
	    ref->loc = goal_alt[i].reg_loc;
	    ref->insn = insn;
	    ref->opno = i;
	    ref->operand = recog_data.operand[i];
	    ref->type = RA_REF_NONE;

	    if (*goal_alt[i].constraints != '\0')
	      {
		if (goal_alt[i].earlyclobber)
		  RA_REF_SET_TYPE (ref, RA_REF_CLOBBER);
		if (operand_type[i] == RELOAD_OTHER)
		  RA_REF_SET_TYPE (ref, RA_REF_RDWR);
		else if (operand_type[i] == RELOAD_FOR_INPUT)
		  RA_REF_SET_TYPE (ref, RA_REF_READ);
		else if (operand_type[i] == RELOAD_FOR_OUTPUT)
		  RA_REF_SET_TYPE (ref, RA_REF_WRITE);
	      }

	    if (matches >= 0)
	      {
		if (!opno2ref[matches])
		  abort ();
		ref->matches = opno2ref[matches];
	      }
	    else
	      ref->matches = NULL;

	    if (RA_REF_REGNO (ref) < FIRST_PSEUDO_REGISTER)
	      {
		CLEAR_HARD_REG_SET (ref->hardregs);
		SET_HARD_REG_BIT (ref->hardregs, RA_REF_REGNO (ref));
	      }
	    else
	      COPY_HARD_REG_SET (ref->hardregs,
				 reg_class_contents[goal_alt[i].class]);

	    /* Fields for debugging.  */
	    ref->alt = goal_alternative_number;
	    ref->constraints = goal_alt[i].constraints;
	    ref->class = goal_alt[i].class;
	    
	    if (RA_REF_WRITE_P (ref) || RA_REF_CLOBBER_P (ref))
	      *def_refs++ = ref;
	    if (RA_REF_READ_P (ref))
	      *use_refs++ = ref;
	  }
      }
  }

  *n_defs = def_refs - orig_def_refs;
  *n_uses = use_refs - orig_use_refs;
}

/* Recognize the insn INSN and check constraints validity very
   strictly.  */
int
ra_check_constraints (insn)
     rtx insn;
{
  int insn_code_number;
  int i;
  int noperands;
  /* These start out as the constraints for the insn
     and they are chewed up as we consider alternatives.  */
  char *constraints[MAX_RECOG_OPERANDS];
  /* These are the preferred classes for an operand, or NO_REGS if it isn't
     a register.  */
  /* Nonzero for a MEM operand whose entire address needs a reload.  */
  int address_reloaded[MAX_RECOG_OPERANDS];
  /* Value of enum reload_type to use for operand.  */
  enum reload_type operand_type[MAX_RECOG_OPERANDS];
  /* Value of enum reload_type to use within address of operand.  */
  enum reload_type address_type[MAX_RECOG_OPERANDS];
  /* Save the usage of each operand.  */
  enum reload_usage modified[MAX_RECOG_OPERANDS];
  int n_alternatives;
  struct alternative_info this_alt[MAX_RECOG_OPERANDS];
  
  int this_alternative_number;
  int swapped;

  int best;
  int commutative;
  char operands_match[MAX_RECOG_OPERANDS][MAX_RECOG_OPERANDS];
  rtx substed_operand[MAX_RECOG_OPERANDS];
  enum machine_mode operand_mode[MAX_RECOG_OPERANDS];
  rtx pat = PATTERN (insn);

  /* If we are before reload and the pattern is a SET, see if we can add
     clobbers.  */
  int icode = recog (pat, insn, 0);
  int is_asm = icode < 0 && asm_noperands (PATTERN (insn)) >= 0;


  /* If this is an asm and the operand aren't legal, then fail.  Likewise if
     this is not an asm and the insn wasn't recognized.  */
  if ((is_asm && ! check_asm_operands (PATTERN (insn)))
      || (!is_asm && icode < 0))
    return 0;

  INSN_CODE (insn) = icode;
  extract_insn (insn);

  noperands = reload_n_operands = recog_data.n_operands;
  n_alternatives = recog_data.n_alternatives;

  n_reloads = 0;
  n_replacements = 0;
  n_earlyclobbers = 0;

  /* Just return "no reloads" if insn has no operands with constraints.  */
  if (noperands == 0 || n_alternatives == 0)
    return 1;

  this_insn = insn;
  insn_code_number = INSN_CODE (insn);
  this_insn_is_asm = insn_code_number < 0;

  memcpy (operand_mode, recog_data.operand_mode,
	  noperands * sizeof (enum machine_mode));
  memcpy (constraints, recog_data.constraints, noperands * sizeof (char *));

  commutative = -1;

  /* If we will need to know, later, whether some pair of operands
     are the same, we must compare them now and save the result.
     Reloading the base and index registers will clobber them
     and afterward they will fail to match.  */

  for (i = 0; i < noperands; i++)
    {
      char *p;
      int c;

      substed_operand[i] = recog_data.operand[i];
      p = constraints[i];

      modified[i] = RELOAD_READ;

      /* Scan this operand's constraint to see if it is an output operand,
	 an in-out operand, is commutative, or should match another.  */

      while ((c = *p++))
	{
	  if (c == '=')
	    modified[i] = RELOAD_WRITE;
	  else if (c == '+')
	    modified[i] = RELOAD_READ_WRITE;
	  else if (c == '%')
	    {
	      /* The last operand should not be marked commutative.  */
	      if (i == noperands - 1)
		abort ();

	      commutative = i;
	    }
	  else if (c >= '0' && c <= '9')
	    {
	      rtx op1 = recog_data.operand[c - '0'];
	      rtx op2 = recog_data.operand[i];
	      c -= '0';
	      if (GET_MODE (op1) != GET_MODE (op2)
		  && INTEGRAL_MODE_P (GET_MODE (op1))
		  && INTEGRAL_MODE_P (GET_MODE (op2)))
		{
		  if (GET_MODE_SIZE (GET_MODE (op1))
		      < GET_MODE_SIZE (GET_MODE (op2)))
		    {
		      rtx t = op1;
		      op1 = op2;
		      op2 = t;
		    }
		  if (GET_CODE (op2) == SUBREG)
		    {
		      unsigned o = subreg_lowpart_offset (GET_MODE (op2),
							  GET_MODE (op1));
		      if (o == SUBREG_BYTE (op2))
			op2 = SUBREG_REG (op2);
		    }
		}
	      operands_match[c][i] = pre_operands_match_p (op1, op2);;

	      /* An operand may not match itself.  */
	      if (c == i)
		abort ();

	      /* If C can be commuted with C+1, and C might need to match I,
		 then C+1 might also need to match I.  */
	      if (commutative >= 0)
		{
		  if (c == commutative || c == commutative + 1)
		    {
		      int other = c + (c == commutative ? 1 : -1);
		      operands_match[other][i]
			= pre_operands_match_p (recog_data.operand[other],
					    recog_data.operand[i]);
		    }
		  if (i == commutative || i == commutative + 1)
		    {
		      int other = i + (i == commutative ? 1 : -1);
		      operands_match[c][other]
			= pre_operands_match_p (recog_data.operand[c],
					    recog_data.operand[other]);
		    }
		  /* Note that C is supposed to be less than I.
		     No need to consider altering both C and I because in
		     that case we would alter one into the other.  */
		}
	    }
	}
    }

  /* Examine each operand that is a memory reference or memory address
     and check addresses.  */
  for (i = 0; i < noperands; i++)
    {
      RTX_CODE code = GET_CODE (recog_data.operand[i]);

      address_reloaded[i] = 0;
      operand_type[i] = (modified[i] == RELOAD_READ ? RELOAD_FOR_INPUT
			 : modified[i] == RELOAD_WRITE ? RELOAD_FOR_OUTPUT
			 : RELOAD_OTHER);
      address_type[i]
	= (modified[i] == RELOAD_READ ? RELOAD_FOR_INPUT_ADDRESS
	   : modified[i] == RELOAD_WRITE ? RELOAD_FOR_OUTPUT_ADDRESS
	   : RELOAD_OTHER);

      if (*constraints[i] == 0)
	/* Ignore things like match_operator operands.  */
	;
      else if (constraints[i][0] == 'p')
	{
	  /* FIXME: denisc@overta.ru We don't use reload_address now.
	     define_address must be used here.  */
	  if (!memory_address_p (recog_data.operand_mode[i],
				 recog_data.operand[i]))
	    return 0;
	}
      else if (code == MEM)
	{
	  /* FIXME: denisc@overta.ru We don't use reload_address now.
	     define_address must be used here.  */
	  if (!memory_address_p (recog_data.operand_mode[i],
				 XEXP (recog_data.operand[i], 0)))
	    return 0;
	}
    }

  /* Now see what we need for pseudo-regs. For this, we must consider
     all the operands together against the register constraints.  */

  best = MAX_RECOG_OPERANDS * 2 + 600;

  swapped = 0;
 try_swapped:

  /* The constraints are made of several alternatives.
     Each operand's constraint looks like foo,bar,... with commas
     separating the alternatives.  The first alternatives for all
     operands go together, the second alternatives go together, etc.

     First loop over alternatives.  */

  for (this_alternative_number = 0;
       this_alternative_number < n_alternatives;
       this_alternative_number++)
    {
      /* Loop over operands for one constraint alternative.  */
      /* LOSERS counts those that don't fit this alternative
	 and would require loading.  */

      int losers = scan_alternative (this_alt, constraints, modified,
				     address_reloaded, operands_match,
				     swapped, &commutative);

      if (losers == 0)
	return 1;
      /* If this alternative can be made to work by reloading,
	 and it needs less reloading than the others checked so far,
	 record it as the chosen goal for reloading.  */
      if (losers >= 0 && best > losers)
	{
	  for (i = 0; i < noperands; i++)
	    if (! this_alt[i].win)
	      break;

	  if (i == noperands)
	    return 1;
	  best = losers;
	}
    }

  /* If insn is commutative (it's safe to exchange a certain pair of operands)
     then we need to try each alternative twice,
     the second time matching those two operands
     as if we had exchanged them.
     To do this, really exchange them in operands.

     If we have just tried the alternatives the second time,
     return operands to normal and drop through.  */

  if (commutative >= 0)
    {
      swapped = !swapped;
      if (swapped)
	{
	  recog_data.operand[commutative] = substed_operand[commutative + 1];
	  recog_data.operand[commutative + 1] = substed_operand[commutative];

	  memcpy (constraints, recog_data.constraints,
		  noperands * sizeof (char *));
	  goto try_swapped;
	}
    }

  return 0;
}


/* Increase the insn info table for handling SIZE elements.  */
static void
ra_insn_table_realloc (ra_info, size)
     struct ra_info *ra_info;
     int size;
{
  /* Make table 25 percent larger by default.  */
  if (! size)
    size = ra_info->insn_size;
  
  size += ra_info->insn_size / 4 + 1;

  
  ra_info->insns = (struct ra_refs **)
    xrealloc (ra_info->insns, size * sizeof (struct ra_refs*));
  
  memset (ra_info->insns + ra_info->insn_size, 0, 
	  (size - ra_info->insn_size) * sizeof (struct ra_refs*));

  ra_info->insn_size = size;
}


/* Increase the reg info table for handling SIZE elements.  */
static void
ra_reg_table_realloc (ra_info, size)
     struct ra_info *ra_info;
     int size;
{
  /* Make table 25 percent larger by default.  */
  if (! size)
    size = ra_info->reg_size;
  
  size += ra_info->reg_size / 4 + 1;

  ra_info->regs = (struct ra_refs **)
    xrealloc (ra_info->regs, size * sizeof (struct ra_refs *));

  /* Zero the new entries.  */
  memset (ra_info->regs + ra_info->reg_size, 0, 
	  (size - ra_info->reg_size) * sizeof (struct ra_refs *));

  ra_info->reg_size = size;
}

/* Allocate and initialise dataflow memory.  */
struct ra_info *
ra_info_init (n_regs)
     int n_regs;
{
  int n_insns;
  struct ra_info *ra_info;
  
  ra_info = (struct ra_info *)xcalloc (1, sizeof (struct ra_info));

  gcc_obstack_init (&ra_info->obstack);

  /* Perhaps we should use LUIDs to save memory for the insn_refs
     table.  This is only a small saving; a few pointers.  */
  n_insns = get_max_uid () + 1;

  ra_info->def_id = 0;
  ra_info->n_defs = 0;
  /* Approximate number of defs by number of insns.  */
  ra_info->def_size = n_insns;
  ra_info->defs = xmalloc (ra_info->def_size * sizeof (*ra_info->defs));

  ra_info->use_id = 0;
  ra_info->n_uses = 0;
  /* Approximate number of uses by twice number of insns.  */
  ra_info->use_size = n_insns * 2;
  ra_info->uses = xmalloc (ra_info->use_size * sizeof (*ra_info->uses));

  ra_info->n_regs = n_regs;
  ra_info->insns = NULL;
  ra_insn_table_realloc (ra_info, n_insns);
  ra_info->regs = NULL;
  ra_reg_table_realloc (ra_info, ra_info->n_regs);
  return ra_info;
}

/* Free all the dataflow info.  */
void
ra_info_free (ra_info)
     struct ra_info *ra_info;
{
  if (ra_info->insns)
    free (ra_info->insns);

  if (ra_info->defs)
    free (ra_info->defs);

  if (ra_info->uses)
    free (ra_info->uses);

  if (ra_info->regs)
    free (ra_info->regs);

  obstack_free (&ra_info->obstack, NULL);
  free (ra_info);
}

/* Print all defs/uses for INSN from RA_INFO.  */

static void
debug_ra_insn_refs (ra_info, insn)
     struct ra_info *ra_info;
     rtx insn;
{
  int i;
  struct ra_link *link;

  if (RA_INSN_REFS (ra_info, insn))
    {
      link = RA_INSN_DEFS (ra_info, insn);
      
      for (i=0; i < 2; ++i)
	{
	  for (;link; link = link->next)
	    {
	      char *s;
	      ra_ref *ref = link->ref;
 
	      fprintf (stderr,"op: %d  %-20s", ref->opno,
		       reg_class_names[ref->class]);
	      print_inline_rtx (stderr, ref->reg, 27/2);
	      if (ref->matches)
		fprintf (stderr, " -> %d", ref->matches->opno);
	      fprintf (stderr, "\n");
	      if (ref->reg != ref->operand)
		{
		  fprintf (stderr, "%27s", "");
		  print_inline_rtx (stderr, ref->operand, 27/2);
		  fprintf (stderr, "\n");
		}

	      fprintf (stderr, "%27s", "");
	      if (RA_REF_ADDRESS_P (ref))
		fprintf (stderr, "%s ", "address");
	      if (RA_REF_CLOBBER_P (ref))
		fprintf (stderr, "%s ", "clobber");
	      if (RA_REF_READ_P (ref))
		fprintf (stderr, "%s ", "read");
	      if (RA_REF_WRITE_P (ref))
		fprintf (stderr, "%s ", "write");
	      fprintf (stderr, "\n");
		
	      fprintf (stderr, "%27salt: %d '", "",ref->alt);
	      for (s = ref->constraints; *s && *s != ','; ++s)
		fprintf (stderr,"%c", *s);
	      fprintf (stderr, "'\n");
	      debug_hard_reg_set (ref->hardregs);
	    }
	  link = RA_INSN_USES (ra_info, insn);
	}
    }
  else
    fprintf (stderr, "No ra_ref info\n");
}


/* Print all defs/uses for REGNO from RA_INFO.  */

static void ATTRIBUTE_UNUSED
debug_ra_reg_refs (ra_info, regno)
     struct ra_info *ra_info;
     int regno;
{
  int i;
  struct ra_link *link;

  if (RA_REG_REFS (ra_info, regno))
    {
      link = RA_REG_DEFS (ra_info, regno);
      
      for (i=0; i < 2; ++i)
	{
	  for (;link; link = link->next)
	    {
	      char *s;
	      ra_ref *ref = link->ref;
 
	      fprintf (stderr,"op: %d  %-20s", ref->opno,
		       reg_class_names[ref->class]);
	      print_inline_rtx (stderr, ref->reg, 27/2);
	      if (ref->matches)
		fprintf (stderr, " -> %d", ref->matches->opno);
	      fprintf (stderr, "\n");
	      if (ref->reg != ref->operand)
		{
		  fprintf (stderr, "%27s", "");
		  print_inline_rtx (stderr, ref->operand, 27/2);
		  fprintf (stderr, "\n");
		}

	      fprintf (stderr, "%27s", "");
	      if (RA_REF_ADDRESS_P (ref))
		fprintf (stderr, "%s ", "address");
	      if (RA_REF_CLOBBER_P (ref))
		fprintf (stderr, "%s ", "clobber");
	      if (RA_REF_READ_P (ref))
		fprintf (stderr, "%s ", "read");
	      if (RA_REF_WRITE_P (ref))
		fprintf (stderr, "%s ", "write");
	      fprintf (stderr, "\n");
		
	      fprintf (stderr, "%27salt: %d '", "",ref->alt);
	      for (s = ref->constraints; *s && *s != ','; ++s)
		fprintf (stderr,"%c", *s);
	      fprintf (stderr, "'\n");
	      debug_hard_reg_set (ref->hardregs);
	    }
	  link = RA_REG_USES (ra_info, regno);
	}
    }
  else
    fprintf (stderr, "No ra_ref info\n");
}


/* Put pointer REFS (pointer to defs/uses for INSN) to ra_info->insns array.
   ra_info->insns array can be reallocated.  */

static void
ra_info_add_insn_refs (ra_info, insn, refs)
     struct ra_info *ra_info;
     rtx insn;
     struct ra_refs *refs;
{
  int uid = INSN_UID (insn);
  
  if (uid >= ra_info->insn_size)
    ra_insn_table_realloc (ra_info, uid);
  else if (RA_INSN_REFS (ra_info, insn))
    abort ();

  ra_info->insns[uid] = refs;
}

/* Add reference to ra_info->regs array for each register of INSN.
   ra_info->regs array can be reallocated.  */
static void
ra_info_add_reg_refs (ra_info, insn, refs)
     struct ra_info *ra_info;
     rtx insn ATTRIBUTE_UNUSED;
     struct ra_refs *refs;
{
  int regno;
  struct ra_link *link;
  struct ra_link **l;

  for (link = refs->defs; link; link = link->next)
    {
      regno = RA_REF_REGNO (link->ref);
      if (regno >= ra_info->reg_size)
	ra_reg_table_realloc (ra_info, regno);

      if (!RA_REG_REFS (ra_info, regno))
	{
	  RA_REG_REFS (ra_info, regno)
	    = (struct ra_refs *)obstack_alloc (&ra_info->obstack,
					       sizeof (struct ra_refs));
	  memset (ra_info->regs[regno], 0, sizeof (struct ra_refs));
	}
      RA_REG_REFS (ra_info, regno)->n_defs++;
      l = &RA_REG_DEFS (ra_info, regno);
      *l = ra_link_create (ra_info, link->ref, *l);
    }

  for (link = refs->uses; link; link = link->next)
    {
      regno = RA_REF_REGNO (link->ref);
      if (regno >= ra_info->reg_size)
	ra_reg_table_realloc (ra_info, regno);

      if (!RA_REG_REFS (ra_info, regno))
	{
	  RA_REG_REFS (ra_info, regno)
	    = (struct ra_refs *)obstack_alloc (&ra_info->obstack,
					       sizeof (struct ra_refs));
	  memset (ra_info->regs[regno], 0, sizeof (struct ra_refs));
	}
	
      RA_REG_REFS (ra_info, regno)->n_uses++;
      l = &RA_REG_USES (ra_info, regno);
      *l = ra_link_create (ra_info, link->ref, *l);
    }
}

/* Build ra_refs. This is a list of defs and uses for one insn.
   List builded from plain arrays DEF_REFS and USE_REFS.  */

static struct ra_refs *
build_ra_refs_for_insn (ra_info, def_refs, use_refs, n_defs, n_uses)
     struct ra_info *ra_info;
     ra_ref **def_refs;
     ra_ref **use_refs;
     int n_defs;
     int n_uses;
{
  int n;
  struct ra_refs *ra_refs
    = (struct ra_refs *) obstack_alloc (&ra_info->obstack,
					sizeof (struct ra_refs));
  memset (ra_refs, 0, sizeof (struct ra_refs));
  
  for (n = 0; n < n_defs; ++n)
    {
      ra_ref *ref = def_refs[n];
      if (ra_info->def_id >= ra_info->def_size)
	{
	  /* Make table 25 percent larger.  */
	  ra_info->def_size += (ra_info->def_size / 4);
	  ra_info->defs
	    = xrealloc (ra_info->defs,
			ra_info->def_size * sizeof (*ra_info->defs));
	}
      RA_REF_ID (ref) = ra_info->def_id;
      ra_info->defs[ra_info->def_id++] = ref;
      ra_refs->defs = ra_link_create (ra_info, ref, ra_refs->defs);
      ra_refs->n_defs++;
    }

  for (n= 0; n < n_uses; ++n)
    {
      ra_ref *ref = use_refs[n];
      if (ra_info->use_id >= ra_info->use_size)
	{
	  /* Make table 25 percent larger.  */
	  ra_info->use_size += (ra_info->use_size / 4);
	  ra_info->uses
	    = xrealloc (ra_info->uses, 
			ra_info->use_size * sizeof (*ra_info->uses));
	}
      RA_REF_ID (ref) = ra_info->use_id;
      ra_info->uses[ra_info->use_id++] = ref;
      ra_refs->uses = ra_link_create (ra_info, ref, ra_refs->uses);
      ra_refs->n_uses++;
    }
  return ra_refs;
}


/* Build array for translation all df refs from dlink to ra_refs from
   ra_link.
   Return zero if error.  */

static int
df_link2ra_link (df2ra, insn, dlink, rlink)
     struct df2ra df2ra;
     rtx insn;
     struct df_link *dlink;
     struct ra_link *rlink;
{
  unsigned int regno;
  int bad = 0;
  
  for (; dlink; dlink = dlink->next)
    /* This condition shows which df ref's can't be reached by
       ra_ref's.  */
    if (dlink->ref/*
	&& GET_MODE_CLASS (GET_MODE (DF_REF_REG (dlink->ref))) != MODE_CC
	&& GET_CODE (PATTERN (insn)) != ASM_INPUT
	&& GET_CODE (insn) != CALL_INSN*/
	&& (DF_REF_REGNO (dlink->ref) < FIRST_PSEUDO_REGISTER
	    ? !fixed_regs[DF_REF_REGNO (dlink->ref)]
	    : 1))
      {
	struct ra_link *link;
	struct ref *dref = dlink->ref;
	int founded = 0;
	regno = DF_REF_REGNO (dlink->ref);
	
	if (regno < FIRST_PSEUDO_REGISTER)
	  for (link = rlink; link; link = link->next)
	    {
	      ra_ref *ref = link->ref;
	      unsigned int ref_regno = RA_REF_REGNO (ref);
		      
	      if (ref_regno < FIRST_PSEUDO_REGISTER)
		{
		  int i;
		  int nregs = HARD_REGNO_NREGS (ref_regno,
						GET_MODE (ref->reg));
			  
		  for (i = 0; i < nregs; ++i)
		    if (regno == ref_regno + i)
		      {
			founded = 1;
			DF2RA (df2ra, dref) = ref;
			break;
		      }
		}
	    }
	else
	  for (link = rlink;link; link = link->next)
	    {
	      ra_ref *ref = link->ref;
	      if (regno == RA_REF_REGNO (ref))
		{
		  founded = 1;
		  DF2RA (df2ra, dref) = ref;
		  break;
		}
	    }
	if (!founded)
	  bad = 1;
      }
  return bad;
}

struct df2ra
build_df2ra (df, ra_info)
     struct df *df;
     struct ra_info *ra_info;
{
  struct df2ra df2ra;
  rtx insn;
    
  df2ra.def2def = xcalloc (df->def_id + 1, sizeof (ra_ref *));
  df2ra.use2use = xcalloc (df->use_id + 1, sizeof (ra_ref *));
  
  /* Check ra_info by comparing it with the df info and build array
     for translation df ref to ra_ref.  */
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      int bad = 0;

      if (RA_INSN_REFS (ra_info, insn))
	{
	  bad = df_link2ra_link (df2ra, insn, DF_INSN_DEFS (df, insn),
				 RA_INSN_DEFS (ra_info, insn));
	  bad |= df_link2ra_link (df2ra, insn, DF_INSN_USES (df, insn),
				  RA_INSN_USES (ra_info, insn));
	}
      /* FIXME denisc@overta.ru */
      if (bad & 0)
	{
	  fprintf (stderr, "NONEQUAL: ");
	  debug_df_insn (insn);
	  debug_ra_insn_refs (ra_info, insn);
	}
    }
  return df2ra;
}

void
pre_reload (ra_info, modified)
     struct ra_info *ra_info;
     bitmap modified;
{
  int j;
  int max;
  
  /* Often (MEM (REG n)) is still valid even if (REG n) is put
     on the stack. Set spill_indirect_levels to the number of
     levels such addressing is permitted, zero if it is not
     permitted at all.  */
  if (indirect_levels < 0)
    {
      rtx tem = gen_rtx_MEM (Pmode,
			     gen_rtx_PLUS (Pmode,
					   gen_rtx_REG (Pmode,
							LAST_VIRTUAL_REGISTER
							+ 1),
					   GEN_INT (4)));
      indirect_levels = 0;
      while (memory_address_p (QImode, tem))
	{
	  indirect_levels++;
	  tem = gen_rtx_MEM (Pmode, tem);
	}
    }

  /* FIXME denisc@overta.ru Only for compatibility with reload.  */
  replace_reloads = 1;
  no_new_pseudos = 0;

  if (modified)
    {
      max = 0;
      EXECUTE_IF_SET_IN_BITMAP
	(modified, 0, j,
	 {
	   if (j > max)
	     max = j;
	   if (j < ra_info->insn_size && ra_info->insns[j])
	     {
	       ra_info_remove_refs (ra_info, ra_info->insns[j]);
	       ra_info->insns[j] = NULL;
	     }
	 });
      if (max >= ra_info->insn_size)
	ra_insn_table_realloc (ra_info, max);
    }
  pre_reload_collect (ra_info, modified);
}

/* Collect all defs/uses for all insns.  */
static void
pre_reload_collect (ra_info, modified)
     struct ra_info *ra_info;
     bitmap modified;
{
  rtx insn;
  int cnt;
  basic_block bb;

  ra_ref *def_refs[(sizeof (ra_ref *)
		    * MAX_RECOG_OPERANDS * MAX_REGS_PER_ADDRESS + 1)];
  ra_ref *use_refs[(sizeof (ra_ref *)
		    * MAX_RECOG_OPERANDS * MAX_REGS_PER_ADDRESS + 1)];

  FOR_EACH_BB (bb)
    {
      for (insn = bb->head;
	   insn && PREV_INSN (insn) != bb->end;
	   insn = NEXT_INSN (insn))
	{
	  enum rtx_code pat_code;
	  enum rtx_code code = GET_CODE (insn);
	  rtx prev;
	  rtx next;
	  rtx orig_insn;
	  rtx deb_insn;

	  if (modified && !bitmap_bit_p (modified, INSN_UID (insn)))
	    continue;
	  
	  ra_info_add_insn_refs (ra_info, insn, NULL);
      
	  if (GET_RTX_CLASS (code) != 'i')
	    continue;

	  pat_code = GET_CODE (PATTERN (insn));
	  if (pat_code == USE
	      || pat_code == CLOBBER
	      || pat_code == ASM_INPUT
	      || pat_code == ADDR_VEC
	      || pat_code == ADDR_DIFF_VEC)
	    continue;

	  cnt = 0;
      
	  next = NEXT_INSN (insn);
	  prev = PREV_INSN (insn);
	  orig_insn = insn;
	  while (1)
	    {
	      int n_defs;
	      int n_uses;

	      ra_info_add_insn_refs (ra_info, insn, NULL);
	      collect_insn_info (ra_info, insn,
				 def_refs, use_refs, &n_defs, &n_uses);
	      if (++cnt > 30)
		internal_error ("Didn't find a pre-reload.\n");
	  
	      if (n_reloads)
		{
		  rtx before = PREV_INSN (insn);
		  rtx after = NEXT_INSN (insn);

		  if (rtl_dump_file)
		    {
		      fprintf (rtl_dump_file, "Reload for insn:\n");
		      print_rtl_single (rtl_dump_file, insn);
		      fprintf (rtl_dump_file, "\n");
		    }
		  
		  emit_pre_reload_insns (insn);
		  subst_pre_reloads (insn);
		  
		  if (rtl_dump_file)
		    {
		      fprintf (rtl_dump_file, "Reload results:\n");
		      for (deb_insn = NEXT_INSN (before); deb_insn != after;
			   deb_insn = NEXT_INSN (deb_insn))
			{
			  print_rtl_single (rtl_dump_file, deb_insn);
			  fprintf (rtl_dump_file, "\n");
			}
		    }
		  
		  insn = NEXT_INSN (before);
		  continue;
		}

	      if (n_defs || n_uses)
		{
		  struct ra_refs *insn_refs;
		  insn_refs = build_ra_refs_for_insn (ra_info,
						      def_refs, use_refs,
						      n_defs, n_uses);
		  ra_info_add_insn_refs (ra_info, insn, insn_refs);
		  ra_info_add_reg_refs (ra_info, insn, insn_refs);
		}
	      else
		ra_info_add_insn_refs (ra_info, insn, NULL);

	      if (NEXT_INSN (insn) == next)
		break;
	      insn = NEXT_INSN (insn);
	    }
	  /* Keep basic block info up to date.  */
	  if (bb->head == orig_insn)
	    {
	      if (prev)
		bb->head = NEXT_INSN (prev);
	      else
		bb->head = get_insns ();
	    }
	  if (bb->end == orig_insn)
	    {
	      if (next)
		bb->end = PREV_INSN (next);
	      else
		bb->end = get_last_insn ();
	    }
	}
    }

  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      /* FIXME: denisc@overta.ru */
      if (0)
	{
	  if (RA_INSN_REFS (ra_info, insn))
	    {
	      debug_rtx (insn);
	      debug_ra_insn_refs (ra_info, insn);
	    }
	  else
	    {
	      enum rtx_code pat_code;
	      enum rtx_code code = GET_CODE (insn);

	      if (GET_RTX_CLASS (code) != 'i')
		continue;

	      pat_code = GET_CODE (PATTERN (insn));
	      if (pat_code == USE
		  || pat_code == CLOBBER
		  || pat_code == ASM_INPUT
		  || pat_code == ADDR_VEC
		  || pat_code == ADDR_DIFF_VEC)
		continue;

	      debug_rtx (insn);
	      fprintf (stderr, "---- NOREFS ----\n");
	    }
	}
    }
}

/* Remove all references which referenced in REFS.  */
static void
ra_info_remove_refs (ra_info, refs)
     struct ra_info *ra_info;
     struct ra_refs *refs;
{
  int regno;
  struct ra_link *link;
  struct ra_link **l;

  for (link = refs->defs; link; link = link->next)
    {
      regno = RA_REF_REGNO (link->ref);
      ra_info->defs[RA_REF_ID (link->ref)] = NULL;
      for (l = &RA_REG_DEFS (ra_info, regno); *l; l = &(*l)->next)
	if (link->ref == (*l)->ref)
	  {
	    RA_REG_REFS (ra_info, regno)->n_defs--;
	    *l = (*l)->next;
	    if (!*l)
	      break;
	  }
      if (!RA_REG_USES (ra_info, regno) && !RA_REG_DEFS (ra_info, regno))
	RA_REG_REFS (ra_info, regno) = NULL;
    }

  for (link = refs->uses; link; link = link->next)
    {
      regno = RA_REF_REGNO (link->ref);
      ra_info->uses[RA_REF_ID (link->ref)] = NULL;
      for (l = &RA_REG_USES (ra_info, regno); *l; l = &(*l)->next)
	if (link->ref == (*l)->ref)
	  {
	    RA_REG_REFS (ra_info, regno)->n_uses--;
	    *l = (*l)->next;
	    if (!*l)
	      break;
	  }
      if (!RA_REG_USES (ra_info, regno) && !RA_REG_DEFS (ra_info, regno))
	RA_REG_REFS (ra_info, regno) = NULL;
    }
}

/* Compare constraints based incrementally updated ra_info with ra_info
   collected from scratch.  */
void
compare_ra_info (ra1)
     struct ra_info *ra1;
{
  struct ra_info *ra2;
  rtx insn;
  int regno;

  ra2 = ra_info_init (max_reg_num ());
  pre_reload (ra2, NULL);

  /* Check insn based info.  */
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      struct ra_refs *ref1 = RA_INSN_REFS (ra1, insn);
      struct ra_refs *ref2 = RA_INSN_REFS (ra2, insn);
      struct ra_link *l1, *l2;
      
      if ((!ref1 || !ref2) && ref1 != ref2)
	abort ();
      if (!ref1)
	continue;
      if (ref1->n_uses != ref2->n_uses)
	abort ();
      if (ref1->n_defs != ref2->n_defs)
	abort ();

      /* Check all defs.  */
      for (l1 = ref1->defs, l2 = ref2->defs;
	   l1 && l2;
	   l1 = l1->next, l2 = l2->next)
	{
	  if (RA_REF_REGNO (l1->ref) != RA_REF_REGNO (l2->ref))
	    abort ();
	}
      /* Check all uses.  */
      for (l1 = ref1->uses, l2 = ref2->uses;
	   l1 && l2;
	   l1 = l1->next, l2 = l2->next)
	{
	  if (RA_REF_REGNO (l1->ref) != RA_REF_REGNO (l2->ref))
	    abort ();
	}
      
    }

  /* Check registers based info.  */
  for (regno = 0; regno < max_reg_num (); ++regno)
    {
      struct ra_refs *ref1 = RA_REG_REFS (ra1, regno);
      struct ra_refs *ref2 = RA_REG_REFS (ra2, regno);
      struct ra_link *l1, *l2;
      
      if ((!ref1 || !ref2) && ref1 != ref2)
	abort ();
      if (!ref1)
	continue;
      if (ref1->n_uses != ref2->n_uses)
	abort ();
      if (ref1->n_defs != ref2->n_defs)
	abort ();

      /* Check all defs.  */
      for (l1 = ref1->defs; l1; l1 = l1->next)
	{
	  for (l2 = ref2->defs; l2; l2 = l2->next)
	    if (l1->ref->insn == l2->ref->insn
		&& l1->ref->loc == l2->ref->loc
		&& l1->ref->opno == l2->ref->opno)
	      break;
	  
	  if (!l2)
	    abort ();
	}
	  
      /* Check all uses.  */
      for (l1 = ref1->uses; l1; l1 = l1->next)
	{
	  for (l2 = ref2->uses; l2; l2 = l2->next)
	    if (l1->ref->insn == l2->ref->insn
		&& l1->ref->loc == l2->ref->loc
		&& l1->ref->opno == l2->ref->opno)
	      break;
	  
	  if (!l2)
	    abort ();
	}
      
    }

  ra_info_free (ra2);
}

/* Static data for the next two routines.  */

typedef struct change_t
{
  rtx object;
  int old_code;
  rtx *loc;
  rtx old;
} change_t;

static change_t *changes;
static int changes_allocated;

static int num_changes = 0;

/* Validate a proposed change to OBJECT.  LOC is the location in the rtl
   at which NEW will be placed.  If OBJECT is zero, no validation is done,
   the change is simply made.

   If OBJECT must be an INSN, CALL_INSN, or JUMP_INSN, the insn will be
   re-recognized with the change in place.

   IN_GROUP is nonzero if this is part of a group of changes that must be
   performed as a group.  In that case, the changes will be stored.  The
   function `ra_apply_change_group' will validate and apply the changes.

   If IN_GROUP is zero, this is a single change.  Try to recognize the insn
   or validate the memory reference with the change applied.  If the result
   is not valid for the machine, suppress the change and return zero.
   Otherwise, perform the change and return 1.  */

int
ra_validate_change (object, loc, new, in_group)
    rtx object;
    rtx *loc;
    rtx new;
    int in_group;
{
  rtx old = *loc;

  if (old == new || rtx_equal_p (old, new))
    return 1;

  if (in_group == 0 && num_changes != 0)
    abort ();

  *loc = new;

  /* Save the information describing this change.  */
  if (num_changes >= changes_allocated)
    {
      if (changes_allocated == 0)
	/* This value allows for repeated substitutions inside complex
	   indexed addresses, or changes in up to 5 insns.  */
	changes_allocated = MAX_RECOG_OPERANDS * 5;
      else
	changes_allocated *= 2;

      changes =
	(change_t*) xrealloc (changes,
			      sizeof (change_t) * changes_allocated);
    }

  changes[num_changes].object = object;
  changes[num_changes].loc = loc;
  changes[num_changes].old = old;

  if (object)
    {
      /* Set INSN_CODE to force rerecognition of insn.  Save old code in
	 case invalid.  */
      changes[num_changes].old_code = INSN_CODE (object);
      INSN_CODE (object) = -1;
    }

  num_changes++;

  /* If we are making a group of changes, return 1.  Otherwise, validate the
     change group we made.  */

  if (in_group)
    return 1;
  else
    return ra_apply_change_group ();
}

/* Apply a group of changes previously issued with `ra_validate_change'.
   Return 1 if all changes are valid, zero otherwise.  */

int
ra_apply_change_group ()
{
  int i;
  rtx last_validated = NULL_RTX;

  /* The changes have been applied and all INSN_CODEs have been reset to
     force rerecognition. */

  for (i = 0; i < num_changes; i++)
    {
      rtx object = changes[i].object;

      /* if there is no object to test or if it is the same as the one we
         already tested, ignore it.  */
      if (object == 0 || object == last_validated)
	continue;

      if (!ra_check_constraints (object))
	{
	  rtx pat = PATTERN (object);

	  if (GET_CODE (pat) == USE || GET_CODE (pat) == CLOBBER)
	    /* If this insn is a CLOBBER or USE, it is always valid, but is
	       never recognized.  */
	    continue;
	  else
	    break;
	}
      last_validated = object;
    }

  if (i == num_changes)
    {
      basic_block bb;

      for (i = 0; i < num_changes; i++)
	if (changes[i].object
	    && INSN_P (changes[i].object)
	    && (bb = BLOCK_FOR_INSN (changes[i].object)))
	  bb->flags |= BB_DIRTY;

      num_changes = 0;
      return 1;
    }
  else
    {
      ra_cancel_changes (0);
      return 0;
    }
}

/* Retract the changes numbered NUM and up.  */

void
ra_cancel_changes (num)
     int num;
{
  int i;

  /* Back out all the changes.  Do this in the opposite order in which
     they were made.  */
  for (i = num_changes - 1; i >= num; i--)
    {
      *changes[i].loc = changes[i].old;
      if (changes[i].object && GET_CODE (changes[i].object) != MEM)
	INSN_CODE (changes[i].object) = changes[i].old_code;
    }
  num_changes = num;
}

