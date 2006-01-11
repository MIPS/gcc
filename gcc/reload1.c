/* Reload pseudo regs into hard regs for insns that require hard regs.
   Copyright (C) 1987, 1988, 1989, 1992, 1993, 1994, 1995, 1996, 1997, 1998,
   1999, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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
#include "real.h"
#include "toplev.h"
#include "except.h"
#include "tree.h"

/* This file contains the reload pass of the compiler, which is
   run after register allocation has been done.  It checks that
   each insn is valid (operands required to be in registers really
   are in registers of the proper class) and fixes up invalid ones
   by copying values temporarily into registers for the insns
   that need them.

   The results of register allocation are described by the vector
   reg_renumber; the insns still contain pseudo regs, but reg_renumber
   can be used to find which hard reg, if any, a pseudo reg is in.

   The technique we always use is to free up a few hard regs that are
   called ``reload regs'', and for each place where a pseudo reg
   must be in a hard reg, copy it temporarily into one of the reload regs.

   Reload regs are allocated locally for every instruction that needs
   reloads.  When there are pseudos which are allocated to a register that
   has been chosen as a reload reg, such pseudos must be ``spilled''.
   This means that they go to other hard regs, or to stack slots if no other
   available hard regs can be found.  Spilling can invalidate more
   insns, requiring additional need for reloads, so we must keep checking
   until the process stabilizes.

   For machines with different classes of registers, we must keep track
   of the register class needed for each reload, and make sure that
   we allocate enough reload registers of each class.

   The file reload.c contains the code that checks one insn for
   validity and reports the reloads that it needs.  This file
   is in charge of scanning the entire rtl code, accumulating the
   reload needs, spilling, assigning reload registers to use for
   fixing up each insn, and generating the new insns to copy values
   into the reload registers.  */

/* Element N is the constant value to which pseudo reg N is equivalent,
   or zero if pseudo reg N is not equivalent to a constant.
   find_reloads looks at this in order to replace pseudo reg N
   with the constant it stands for.  */
rtx *reg_equiv_constant;

/* Element N is a memory location to which pseudo reg N is equivalent,
   prior to any register elimination (such as frame pointer to stack
   pointer).  Depending on whether or not it is a valid address, this value
   is transferred to either reg_equiv_address or reg_equiv_mem.  */
rtx *reg_equiv_memory_loc;

/* We allocate reg_equiv_memory_loc inside a varray so that the garbage
   collector can keep track of what is inside.  */
varray_type reg_equiv_memory_loc_varray;

/* Element N is the address of stack slot to which pseudo reg N is equivalent.
   This is used when the address is not valid as a memory address
   (because its displacement is too big for the machine.)  */
rtx *reg_equiv_address;

/* Element N is the memory slot to which pseudo reg N is equivalent,
   or zero if pseudo reg N is not equivalent to a memory slot.  */
rtx *reg_equiv_mem;

/* Widest width in which each pseudo reg is referred to (via subreg).  */
static unsigned int *reg_max_ref_width;

/* Element N is the list of insns that initialized reg N from its equivalent
   constant or memory slot.  */
static rtx *reg_equiv_init;

/* Vector to remember old contents of reg_renumber before spilling.  */
static short *reg_old_renumber;

/* Indicate whether the register's current value is one that is not
   safe to retain across a call, even for registers that are normally
   call-saved.  */
static HARD_REG_SET reg_reloaded_call_part_clobbered;

/* Holds the last rtx used for any given reg, or 0 if it has never
   been used for spilling yet.  This rtx is reused, provided it has
   the proper mode.  */
static rtx spill_reg_rtx[FIRST_PSEUDO_REGISTER];

/* This reg set indicates registers that can't be used as spill registers for
   the currently processed insn.  These are the hard registers which are live
   during the insn, but not allocated to pseudos, as well as fixed
   registers.  */
static HARD_REG_SET bad_spill_regs;

/* These are the hard registers that can't be used as spill register for any
   insn.  This includes registers used for user variables and registers that
   we can't eliminate.  A register that appears in this set also can't be used
   to retry register allocation.  */
static HARD_REG_SET bad_spill_regs_global;

/* This vector of reg sets indicates, for each pseudo, which hard registers
   may not be used for retrying global allocation because the register was
   formerly spilled from one of them.  If we allowed reallocating a pseudo to
   a register that it was already allocated to, reload might not
   terminate.  */
static HARD_REG_SET *pseudo_previous_regs;

/* This vector of reg sets indicates, for each pseudo, which hard
   registers may not be used for retrying global allocation because they
   are used as spill registers during one of the insns in which the
   pseudo is live.  */
static HARD_REG_SET *pseudo_forbidden_regs;

/* All hard regs that have been used as spill registers for any insn are
   marked in this set.  This is used to update regs_ever_live in
   finish_spills.  */
static HARD_REG_SET used_spill_regs;

/* Nonzero if indirect addressing is supported on the machine; this means
   that spilling (REG n) does not require reloading it into a register in
   order to do (MEM (REG n)) or (MEM (PLUS (REG n) (CONST_INT c))).  The
   value indicates the level of indirect addressing supported, e.g., two
   means that (MEM (MEM (REG n))) is also valid if (REG n) does not get
   a hard register.  */
static char spill_indirect_levels;

/* Nonzero if indirect addressing is supported when the innermost MEM is
   of the form (MEM (SYMBOL_REF sym)).  It is assumed that the level to
   which these are valid is the same as spill_indirect_levels, above.  */
char indirect_symref_ok;

/* Nonzero if an address (plus (reg frame_pointer) (reg ...)) is valid.  */
char double_reg_address_ok;

/* Record the stack slot for each spilled hard register.  */
static rtx spill_stack_slot[FIRST_PSEUDO_REGISTER];

/* Width allocated so far for that stack slot.  */
static unsigned int spill_stack_slot_width[FIRST_PSEUDO_REGISTER];

/* Record which pseudos needed to be spilled.  */
static regset_head spilled_pseudos;

/* Used for communication between order_regs_for_reload and count_pseudo.
   Used to avoid counting one pseudo twice.  */
static regset_head pseudos_counted;

/* First uid used by insns created by reload in this function.
   Used in find_equiv_reg.  */
int reload_first_uid;

/* Flag set by local-alloc or global-alloc if anything is live in
   a call-clobbered reg across calls.  */
int caller_save_needed;

/* Set to 1 while reload_as_needed is operating.
   Required by some machines to handle any generated moves differently.  */
int reload_in_progress = 0;

/* These arrays record the insn_code of insns that may be needed to
   perform input and output reloads of special objects.  They provide a
   place to pass a scratch register.  */
enum insn_code reload_in_optab[NUM_MACHINE_MODES];
enum insn_code reload_out_optab[NUM_MACHINE_MODES];

/* This obstack is used for allocation of rtl during register elimination.
   The allocated storage can be freed once find_reloads has processed the
   insn.  */
static struct obstack reload_obstack;

/* Points to the beginning of the reload_obstack.  All insn_chain structures
   are allocated first.  */
static char *reload_startobj;

/* The point after all insn_chain structures.  Used to quickly deallocate
   memory allocated in copy_reloads during calculate_needs_all_insns.  */
static char *reload_firstobj;

/* This points before all local rtl generated by register elimination.
   Used to quickly free all memory after processing one insn.  */
static char *reload_insn_firstobj;

/* List of insn_chain instructions, one for every insn that reload needs to
   examine.  */
struct insn_chain *reload_insn_chain;

/* List of all insns needing reloads.  */
static struct insn_chain *insns_need_reload;

/* This structure is used to record information about register eliminations.
   Each array entry describes one possible way of eliminating a register
   in favor of another.   If there is more than one way of eliminating a
   particular register, the most preferred should be specified first.  */

struct elim_table
{
  int from;			/* Register number to be eliminated.  */
  int to;			/* Register number used as replacement.  */
  HOST_WIDE_INT initial_offset;	/* Initial difference between values.  */
  int can_eliminate;		/* Nonzero if this elimination can be done.  */
  int can_eliminate_previous;	/* Value of CAN_ELIMINATE in previous scan over
				   insns made by reload.  */
  HOST_WIDE_INT offset;		/* Current offset between the two regs.  */
  HOST_WIDE_INT previous_offset;/* Offset at end of previous insn.  */
  int ref_outside_mem;		/* "to" has been referenced outside a MEM.  */
  rtx from_rtx;			/* REG rtx for the register to be eliminated.
				   We cannot simply compare the number since
				   we might then spuriously replace a hard
				   register corresponding to a pseudo
				   assigned to the reg to be eliminated.  */
  rtx to_rtx;			/* REG rtx for the replacement.  */
};

static struct elim_table *reg_eliminate = 0;

/* This is an intermediate structure to initialize the table.  It has
   exactly the members provided by ELIMINABLE_REGS.  */
static const struct elim_table_1
{
  const int from;
  const int to;
} reg_eliminate_1[] =

/* If a set of eliminable registers was specified, define the table from it.
   Otherwise, default to the normal case of the frame pointer being
   replaced by the stack pointer.  */

#ifdef ELIMINABLE_REGS
  ELIMINABLE_REGS;
#else
  {{ FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}};
#endif

#define NUM_ELIMINABLE_REGS ARRAY_SIZE (reg_eliminate_1)

/* Record the number of pending eliminations that have an offset not equal
   to their initial offset.  If nonzero, we use a new copy of each
   replacement result in any insns encountered.  */
int num_not_at_initial_offset;

/* Count the number of registers that we may be able to eliminate.  */
static int num_eliminable;
/* And the number of registers that are equivalent to a constant that
   can be eliminated to frame_pointer / arg_pointer + constant.  */
static int num_eliminable_invariants;

/* For each label, we record the offset of each elimination.  If we reach
   a label by more than one path and an offset differs, we cannot do the
   elimination.  This information is indexed by the difference of the
   number of the label and the first label number.  We can't offset the
   pointer itself as this can cause problems on machines with segmented
   memory.  The first table is an array of flags that records whether we
   have yet encountered a label and the second table is an array of arrays,
   one entry in the latter array for each elimination.  */

static int first_label_num;
static char *offsets_known_at;
static HOST_WIDE_INT (*offsets_at)[NUM_ELIMINABLE_REGS];

/* A preallocated REG rtx that is available throughout reload.  */
static rtx permanent_test_reg;

/* Number of labels in the current function.  */

static int num_labels;

static void replace_pseudos_in (rtx *, enum machine_mode, rtx);
static void maybe_fix_stack_asms (void);
static void calculate_needs_all_insns (int);
static void find_reload_regs (struct insn_chain *);
static void select_reload_regs (void);
static void delete_caller_save_insns (void);

static void spill_failure (rtx, enum reg_class);
static void count_spilled_pseudo (int, int, int);
static void delete_dead_insn (rtx);
static void alter_reg (int, int);
static void set_label_offsets (rtx, rtx, int);
static void check_eliminable_occurrences (rtx);
static void elimination_effects (rtx, enum machine_mode);
static int eliminate_regs_in_insn (rtx, int);
static void update_eliminable_offsets (void);
static void mark_not_eliminable (rtx, rtx, void *);
static void set_initial_elim_offsets (void);
static void verify_initial_elim_offsets (void);
static void set_initial_label_offsets (void);
static void set_offsets_for_label (rtx);
static void init_elim_table (void);
static void update_eliminables (HARD_REG_SET *);
static void spill_hard_reg (unsigned int, int);
static int finish_spills (int);
static void scan_paradoxical_subregs (rtx);
static void count_pseudo (int);
static void order_regs_for_reload (struct insn_chain *);
static void reload_as_needed (int);
static int reload_reg_class_lower (const void *, const void *);
static int function_invariant_p (rtx);
static void choose_reload_regs (struct insn_chain *);
static void emit_input_reload_insns (struct insn_chain *, struct reload *,
				     rtx, int);
static void emit_output_reload_insns (struct insn_chain *, struct reload *,
				      int);
static rtx do_input_reload (struct insn_chain *, struct reload *, int);
static rtx do_output_reload (struct insn_chain *, struct reload *, int);
static void emit_reload_insns (struct insn_chain *);
static rtx inc_for_reload (rtx, rtx, rtx, int);
#ifdef AUTO_INC_DEC
static void add_auto_inc_notes (rtx, rtx);
#endif
static void copy_eh_notes (rtx, rtx);
static rtx gen_reload (rtx, rtx, int);

/* Initialize the reload pass once per compilation.  */

void
init_reload (void)
{
  int i;

  /* Often (MEM (REG n)) is still valid even if (REG n) is put on the stack.
     Set spill_indirect_levels to the number of levels such addressing is
     permitted, zero if it is not permitted at all.  */

  rtx tem
    = gen_rtx_MEM (Pmode,
		   gen_rtx_PLUS (Pmode,
				 gen_rtx_REG (Pmode,
					      LAST_VIRTUAL_REGISTER + 1),
				 GEN_INT (4)));
  spill_indirect_levels = 0;

  while (memory_address_p (QImode, tem))
    {
      spill_indirect_levels++;
      tem = gen_rtx_MEM (Pmode, tem);
    }

  /* See if indirect addressing is valid for (MEM (SYMBOL_REF ...)).  */

  tem = gen_rtx_MEM (Pmode, gen_rtx_SYMBOL_REF (Pmode, "foo"));
  indirect_symref_ok = memory_address_p (QImode, tem);

  /* See if reg+reg is a valid (and offsettable) address.  */

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      tem = gen_rtx_PLUS (Pmode,
			  gen_rtx_REG (Pmode, HARD_FRAME_POINTER_REGNUM),
			  gen_rtx_REG (Pmode, i));

      /* This way, we make sure that reg+reg is an offsettable address.  */
      tem = plus_constant (tem, 4);

      if (memory_address_p (QImode, tem))
	{
	  double_reg_address_ok = 1;
	  break;
	}
    }

  /* Initialize obstack for our rtl allocation.  */
  gcc_obstack_init (&reload_obstack);
  reload_startobj = obstack_alloc (&reload_obstack, 0);

  INIT_REG_SET (&spilled_pseudos);
  INIT_REG_SET (&pseudos_counted);
  VARRAY_RTX_INIT (reg_equiv_memory_loc_varray, 0, "reg_equiv_memory_loc");
}

/* List of insn chains that are currently unused.  */
static struct insn_chain *unused_insn_chains = 0;

/* Allocate an empty insn_chain structure.  */
struct insn_chain *
new_insn_chain (void)
{
  struct insn_chain *c;

  if (unused_insn_chains == 0)
    {
      c = obstack_alloc (&reload_obstack, sizeof (struct insn_chain));
      INIT_REG_SET (&c->live_before);
      INIT_REG_SET (&c->live_after);
      INIT_REG_SET (&c->unreloaded_sets);
      INIT_REG_SET (&c->unreloaded_uses);
    }
  else
    {
      c = unused_insn_chains;
      unused_insn_chains = c->next;
    }
  c->will_be_deleted = 0;
  c->is_caller_save_insn = 0;
  c->need_operand_change = 0;
  c->need_elim = 0;
  c->n_reloads = 0;
  return c;
}

/* Small utility function to set all regs in hard reg set TO which are
   allocated to pseudos in regset FROM.  */

void
compute_use_by_pseudos (HARD_REG_SET *to, regset from)
{
  unsigned int regno;
  reg_set_iterator rsi;

  EXECUTE_IF_SET_IN_REG_SET (from, FIRST_PSEUDO_REGISTER, regno, rsi)
    {
      int r = reg_renumber[regno];
      int nregs;

      if (r < 0)
	{
	  /* reload_combine uses the information from
	     BASIC_BLOCK->global_live_at_start, which might still
	     contain registers that have not actually been allocated
	     since they have an equivalence.  */
	  gcc_assert (reload_completed);
	}
      else
	{
	  nregs = hard_regno_nregs[r][PSEUDO_REGNO_MODE (regno)];
	  while (nregs-- > 0)
	    SET_HARD_REG_BIT (*to, r + nregs);
	}
    }
}

/* Replace all pseudos found in LOC with their corresponding
   equivalences.  */

static void
replace_pseudos_in (rtx *loc, enum machine_mode mem_mode, rtx usage)
{
  rtx x = *loc;
  enum rtx_code code;
  const char *fmt;
  int i, j;

  if (! x)
    return;

  code = GET_CODE (x);
  if (code == REG)
    {
      unsigned int regno = REGNO (x);

      if (regno < FIRST_PSEUDO_REGISTER)
	return;

      x = eliminate_regs (x, mem_mode, usage);
      if (x != *loc)
	{
	  *loc = x;
	  replace_pseudos_in (loc, mem_mode, usage);
	  return;
	}

      if (reg_equiv_constant[regno])
	*loc = reg_equiv_constant[regno];
      else if (reg_equiv_mem[regno])
	*loc = reg_equiv_mem[regno];
      else if (reg_equiv_address[regno])
	*loc = gen_rtx_MEM (GET_MODE (x), reg_equiv_address[regno]);
      else
	{
	  gcc_assert (!REG_P (regno_reg_rtx[regno])
		      || REGNO (regno_reg_rtx[regno]) != regno);
	  *loc = regno_reg_rtx[regno];
	}

      return;
    }
  else if (code == MEM)
    {
      replace_pseudos_in (& XEXP (x, 0), GET_MODE (x), usage);
      return;
    }

  /* Process each of our operands recursively.  */
  fmt = GET_RTX_FORMAT (code);
  for (i = 0; i < GET_RTX_LENGTH (code); i++, fmt++)
    if (*fmt == 'e')
      replace_pseudos_in (&XEXP (x, i), mem_mode, usage);
    else if (*fmt == 'E')
      for (j = 0; j < XVECLEN (x, i); j++)
	replace_pseudos_in (& XVECEXP (x, i, j), mem_mode, usage);
}

static void mark_dead_insns (void)
{
  struct insn_chain *chain;
  int i;

  for (chain = reload_insn_chain; chain; chain = chain->next)
    {
      rtx insn = chain->insn;
      chain->will_be_deleted = 0;

      if (INSN_P (insn))
	{
	  rtx set = single_set (insn);
	  PUT_MODE (insn, VOIDmode);

	  /* Skip insns that only set an equivalence.  */
	  if (set && REG_P (SET_DEST (set))
	      && reg_renumber[REGNO (SET_DEST (set))] < 0
	      && reg_equiv_constant[REGNO (SET_DEST (set))])
	    chain->will_be_deleted = 1;
	}
    }

  /* If a pseudo has no hard reg, delete the insns that made the equivalence.
     If that insn didn't set the register (i.e., it copied the register to
     memory), just delete that insn instead of the equivalencing insn plus
     anything now dead.  If we call delete_dead_insn on that insn, we may
     delete the insn that actually sets the register if the register dies
     there and that is incorrect.  */

  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    {
      if (reg_renumber[i] < 0 && reg_equiv_init[i] != 0)
	{
	  rtx list;
	  for (list = reg_equiv_init[i]; list; list = XEXP (list, 1))
	    {
	      rtx equiv_insn = XEXP (list, 0);

	      /* If we already deleted the insn or if it may trap, we can't
		 delete it.  The latter case shouldn't happen, but can
		 if an insn has a variable address, gets a REG_EH_REGION
		 note added to it, and then gets converted into an load
		 from a constant address.  */
	      if (GET_CODE (equiv_insn) == NOTE
		  || can_throw_internal (equiv_insn))
		continue;
#if 0
	      if (reg_set_p (regno_reg_rtx[i], PATTERN (equiv_insn)))
		delete_dead_insn (equiv_insn);
	      else
#endif
		PUT_MODE (equiv_insn, SImode);
	    }
	}
    }
}

/* Global variables used by reload and its subroutines.  */

/* Set during calculate_needs if an insn needs register elimination.  */
static int something_needs_elimination;
/* Set during calculate_needs if an insn needs an operand changed.  */
static int something_needs_operands_changed;

/* Nonzero means we couldn't get enough spill regs.  */
static int failure;

/* Main entry point for the reload pass.

   FIRST is the first insn of the function being compiled.

   GLOBAL nonzero means we were called from global_alloc
   and should attempt to reallocate any pseudoregs that we
   displace from hard regs we will use for reloads.
   If GLOBAL is zero, we do not have enough information to do that,
   so any pseudo reg that is spilled must go to the stack.

   Return value is nonzero if reload failed
   and we must not do any more for this function.  */

int
reload (rtx first, int global)
{
  int i;
  rtx insn;
  struct elim_table *ep;
  basic_block bb;

  /* Make sure even insns with volatile mem refs are recognizable.  */
  init_recog ();

  failure = 0;

  reload_firstobj = obstack_alloc (&reload_obstack, 0);

  /* Make sure that the last insn in the chain
     is not something that needs reloading.  */
  emit_note (NOTE_INSN_DELETED);

  /* Enable find_equiv_reg to distinguish insns made by reload.  */
  reload_first_uid = get_max_uid ();

#ifdef SECONDARY_MEMORY_NEEDED
  /* Initialize the secondary memory table.  */
  clear_secondary_mem ();
#endif

  /* We don't have a stack slot for any spill reg yet.  */
  memset (spill_stack_slot, 0, sizeof spill_stack_slot);
  memset (spill_stack_slot_width, 0, sizeof spill_stack_slot_width);

  /* Initialize the save area information for caller-save, in case some
     are needed.  */
  init_save_areas ();

  /* Compute which hard registers are now in use
     as homes for pseudo registers.
     This is done here rather than (eg) in global_alloc
     because this point is reached even if not optimizing.  */
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    mark_home_live (i);

  /* A function that receives a nonlocal goto must save all call-saved
     registers.  */
  if (current_function_has_nonlocal_label)
    for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
      if (! call_used_regs[i] && ! fixed_regs[i] && ! LOCAL_REGNO (i))
	regs_ever_live[i] = 1;

  /* Find all the pseudo registers that didn't get hard regs
     but do have known equivalent constants or memory slots.
     These include parameters (known equivalent to parameter slots)
     and cse'd or loop-moved constant memory addresses.

     Record constant equivalents in reg_equiv_constant
     so they will be substituted by find_reloads.
     Record memory equivalents in reg_mem_equiv so they can
     be substituted eventually by altering the REG-rtx's.  */

  reg_equiv_constant = xcalloc (max_regno, sizeof (rtx));
  reg_equiv_mem = xcalloc (max_regno, sizeof (rtx));
  reg_equiv_init = xcalloc (max_regno, sizeof (rtx));
  reg_equiv_address = xcalloc (max_regno, sizeof (rtx));
  reg_max_ref_width = xcalloc (max_regno, sizeof (int));
  reg_old_renumber = xcalloc (max_regno, sizeof (short));
  memcpy (reg_old_renumber, reg_renumber, max_regno * sizeof (short));
  pseudo_forbidden_regs = xmalloc (max_regno * sizeof (HARD_REG_SET));
  pseudo_previous_regs = xcalloc (max_regno, sizeof (HARD_REG_SET));

  COPY_HARD_REG_SET (bad_spill_regs_global, fixed_reg_set);

  permanent_test_reg = gen_rtx_raw_REG (Pmode, 1);

  /* Look for REG_EQUIV notes; record what each pseudo is equivalent
     to.  Also find all paradoxical subregs and find largest such for
     each pseudo.  */

  num_eliminable_invariants = 0;
  for (insn = first; insn; insn = NEXT_INSN (insn))
    {
      rtx set = single_set (insn);

      /* We may introduce USEs that we want to remove at the end, so
	 we'll mark them with QImode.  Make sure there are no
	 previously-marked insns left by say regmove.  */
      if (INSN_P (insn) && GET_CODE (PATTERN (insn)) == USE
	  && GET_MODE (insn) != VOIDmode)
	PUT_MODE (insn, VOIDmode);

      if (set != 0 && REG_P (SET_DEST (set)))
	{
	  rtx note = find_reg_note (insn, REG_EQUIV, NULL_RTX);
	  if (note
	      && (! function_invariant_p (XEXP (note, 0))
		  || ! flag_pic
		  /* A function invariant is often CONSTANT_P but may
		     include a register.  We promise to only pass
		     CONSTANT_P objects to LEGITIMATE_PIC_OPERAND_P.  */
		  || (CONSTANT_P (XEXP (note, 0))
		      && LEGITIMATE_PIC_OPERAND_P (XEXP (note, 0)))))
	    {
	      rtx x = XEXP (note, 0);
	      i = REGNO (SET_DEST (set));
	      if (i > LAST_VIRTUAL_REGISTER)
		{
		  /* It can happen that a REG_EQUIV note contains a MEM
		     that is not a legitimate memory operand.  As later
		     stages of reload assume that all addresses found
		     in the reg_equiv_* arrays were originally legitimate,

		     It can also happen that a REG_EQUIV note contains a
		     readonly memory location.  If the destination pseudo
		     is set from some other value (typically a different
		     pseudo), and the destination pseudo does not get a
		     hard reg, then reload will replace the destination
		     pseudo with its equivalent memory location.  This
		     is horribly bad as it creates a store to a readonly
		     memory location and a runtime segfault.  To avoid
		     this problem we reject readonly memory locations
		     for equivalences.  This is overly conservative as
		     we could find all sets of the destination pseudo
		     and remove them as they should be redundant.  */
		  if (memory_operand (x, VOIDmode) && ! MEM_READONLY_P (x))
		    {
		      /* Always unshare the equivalence, so we can
			 substitute into this insn without touching the
			 equivalence.  */
		      reg_equiv_memory_loc[i] = copy_rtx (x);
		    }
		  else if (function_invariant_p (x))
		    {
		      if (GET_CODE (x) == PLUS)
			{
			  /* This is PLUS of frame pointer and a constant,
			     and might be shared.  Unshare it.  */
			  reg_equiv_constant[i] = copy_rtx (x);
			  num_eliminable_invariants++;
			}
		      else if (x == frame_pointer_rtx
			       || x == arg_pointer_rtx)
			{
			  reg_equiv_constant[i] = x;
			  num_eliminable_invariants++;
			}
		      else if (LEGITIMATE_CONSTANT_P (x))
			reg_equiv_constant[i] = x;
		      else
			{
			  reg_equiv_memory_loc[i]
			    = force_const_mem (GET_MODE (SET_DEST (set)), x);
			  if (!reg_equiv_memory_loc[i])
			    continue;
			}
		    }
		  else
		    continue;

		  /* If this register is being made equivalent to a MEM
		     and the MEM is not SET_SRC, the equivalencing insn
		     is one with the MEM as a SET_DEST and it occurs later.
		     So don't mark this insn now.  */
		  if (!MEM_P (x)
		      || rtx_equal_p (SET_SRC (set), x))
		    reg_equiv_init[i]
		      = gen_rtx_INSN_LIST (VOIDmode, insn, reg_equiv_init[i]);
		}
	    }
	}

      /* If this insn is setting a MEM from a register equivalent to it,
	 this is the equivalencing insn.  */
      else if (set && MEM_P (SET_DEST (set))
	       && REG_P (SET_SRC (set))
	       && reg_equiv_memory_loc[REGNO (SET_SRC (set))]
	       && rtx_equal_p (SET_DEST (set),
			       reg_equiv_memory_loc[REGNO (SET_SRC (set))]))
	reg_equiv_init[REGNO (SET_SRC (set))]
	  = gen_rtx_INSN_LIST (VOIDmode, insn,
			       reg_equiv_init[REGNO (SET_SRC (set))]);

      if (INSN_P (insn))
	scan_paradoxical_subregs (PATTERN (insn));
    }

  init_elim_table ();

  first_label_num = get_first_label_num ();
  num_labels = max_label_num () - first_label_num;

  /* Allocate the tables used to store offset information at labels.  */
  /* We used to use alloca here, but the size of what it would try to
     allocate would occasionally cause it to exceed the stack limit and
     cause a core dump.  */
  offsets_known_at = xmalloc (num_labels);
  offsets_at = xmalloc (num_labels * NUM_ELIMINABLE_REGS * sizeof (HOST_WIDE_INT));

  /* Alter each pseudo-reg rtx to contain its hard reg number.
     Assign stack slots to the pseudos that lack hard regs or equivalents.
     Do not touch virtual registers.  */

  for (i = LAST_VIRTUAL_REGISTER + 1; i < max_regno; i++)
    alter_reg (i, -1);

  /* If we have some registers we think can be eliminated, scan all insns to
     see if there is an insn that sets one of these registers to something
     other than itself plus a constant.  If so, the register cannot be
     eliminated.  Doing this scan here eliminates an extra pass through the
     main reload loop in the most common case where register elimination
     cannot be done.  */
  for (insn = first; insn && num_eliminable; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      note_stores (PATTERN (insn), mark_not_eliminable, NULL);

  maybe_fix_stack_asms ();

  insns_need_reload = 0;
  something_needs_elimination = 0;

  /* Spill any hard regs that we know we can't eliminate.  */
  CLEAR_HARD_REG_SET (used_spill_regs);
  /* There can be multiple ways to eliminate a register;
     they should be listed adjacently.
     Elimination for any register fails only if all possible ways fail.  */
  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; )
    {
      int from = ep->from;
      int can_eliminate = 0;
      do
	{
          can_eliminate |= ep->can_eliminate;
          ep++;
	}
      while (ep < &reg_eliminate[NUM_ELIMINABLE_REGS] && ep->from == from);
      if (! can_eliminate)
	spill_hard_reg (from, 1);
    }

#if HARD_FRAME_POINTER_REGNUM != FRAME_POINTER_REGNUM
  if (frame_pointer_needed)
    spill_hard_reg (HARD_FRAME_POINTER_REGNUM, 1);
#endif
  finish_spills (global);

  /* From now on, we may need to generate moves differently.  We may also
     allow modifications of insns which cause them to not be recognized.
     Any such modifications will be cleaned up during reload itself.  */
  reload_in_progress = 1;

  /* This loop scans the entire function each go-round
     and repeats until one repetition spills no additional hard regs.  */
  for (;;)
    {
      int something_changed;
      int did_spill;

      HOST_WIDE_INT starting_frame_size;

      /* Round size of stack frame to stack_alignment_needed.  This must be done
	 here because the stack size may be a part of the offset computation
	 for register elimination, and there might have been new stack slots
	 created in the last iteration of this loop.  */
      if (cfun->stack_alignment_needed)
        assign_stack_local (BLKmode, 0, cfun->stack_alignment_needed);

      starting_frame_size = get_frame_size ();

      set_initial_elim_offsets ();
      set_initial_label_offsets ();

      /* For each pseudo register that has an equivalent location defined,
	 try to eliminate any eliminable registers (such as the frame pointer)
	 assuming initial offsets for the replacement register, which
	 is the normal case.

	 If the resulting location is directly addressable, substitute
	 the MEM we just got directly for the old REG.

	 If it is not addressable but is a constant or the sum of a hard reg
	 and constant, it is probably not addressable because the constant is
	 out of range, in that case record the address; we will generate
	 hairy code to compute the address in a register each time it is
	 needed.  Similarly if it is a hard register, but one that is not
	 valid as an address register.

	 If the location is not addressable, but does not have one of the
	 above forms, assign a stack slot.  We have to do this to avoid the
	 potential of producing lots of reloads if, e.g., a location involves
	 a pseudo that didn't get a hard register and has an equivalent memory
	 location that also involves a pseudo that didn't get a hard register.

	 Perhaps at some point we will improve reload_when_needed handling
	 so this problem goes away.  But that's very hairy.  */

      for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
	if (reg_renumber[i] < 0 && reg_equiv_memory_loc[i])
	  {
	    rtx x = eliminate_regs (reg_equiv_memory_loc[i], 0, NULL_RTX);

	    if (strict_memory_address_p (GET_MODE (regno_reg_rtx[i]),
					 XEXP (x, 0)))
	      reg_equiv_mem[i] = x, reg_equiv_address[i] = 0;
	    else if (CONSTANT_P (XEXP (x, 0))
		     || (REG_P (XEXP (x, 0))
			 && REGNO (XEXP (x, 0)) < FIRST_PSEUDO_REGISTER)
		     || (GET_CODE (XEXP (x, 0)) == PLUS
			 && REG_P (XEXP (XEXP (x, 0), 0))
			 && (REGNO (XEXP (XEXP (x, 0), 0))
			     < FIRST_PSEUDO_REGISTER)
			 && CONSTANT_P (XEXP (XEXP (x, 0), 1))))
	      reg_equiv_address[i] = XEXP (x, 0), reg_equiv_mem[i] = 0;
	    else
	      {
		/* Make a new stack slot.  Then indicate that something
		   changed so we go back and recompute offsets for
		   eliminable registers because the allocation of memory
		   below might change some offset.  reg_equiv_{mem,address}
		   will be set up for this pseudo on the next pass around
		   the loop.  */
		reg_equiv_memory_loc[i] = 0;
		reg_equiv_init[i] = 0;
		alter_reg (i, -1);
	      }
	  }

      if (caller_save_needed)
	setup_save_areas ();

      /* If we allocated another stack slot, redo elimination bookkeeping.  */
      if (starting_frame_size != get_frame_size ())
	continue;

      if (caller_save_needed)
	{
	  save_call_clobbered_regs ();
	  /* That might have allocated new insn_chain structures.  */
	  reload_firstobj = obstack_alloc (&reload_obstack, 0);
	}

      mark_dead_insns ();

      calculate_needs_all_insns (global);

      CLEAR_REG_SET (&spilled_pseudos);
      did_spill = 0;

      something_changed = 0;

      /* If we allocated any new memory locations, make another pass
	 since it might have changed elimination offsets.  */
      if (starting_frame_size != get_frame_size ())
	something_changed = 1;

      {
	HARD_REG_SET to_spill;
	CLEAR_HARD_REG_SET (to_spill);
	update_eliminables (&to_spill);
	for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	  if (TEST_HARD_REG_BIT (to_spill, i))
	    {
	      spill_hard_reg (i, 1);
	      did_spill = 1;

	      /* Regardless of the state of spills, if we previously had
		 a register that we thought we could eliminate, but now can
		 not eliminate, we must run another pass.

		 Consider pseudos which have an entry in reg_equiv_* which
		 reference an eliminable register.  We must make another pass
		 to update reg_equiv_* so that we do not substitute in the
		 old value from when we thought the elimination could be
		 performed.  */
	      something_changed = 1;
	    }
      }

      select_reload_regs ();
      if (failure)
	goto failed;

      if (insns_need_reload != 0 || did_spill)
	something_changed |= finish_spills (global);

      if (! something_changed)
	break;

      if (caller_save_needed)
	delete_caller_save_insns ();

      obstack_free (&reload_obstack, reload_firstobj);
    }

  /* If global-alloc was run, notify it of any register eliminations we have
     done.  */
  if (global)
    for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
      if (ep->can_eliminate)
	mark_elimination (ep->from, ep->to);

  /* Use the reload registers where necessary
     by generating move instructions to move the must-be-register
     values into or out of the reload registers.  */

  if (insns_need_reload != 0 || something_needs_elimination
      || something_needs_operands_changed)
    {
      HOST_WIDE_INT old_frame_size = get_frame_size ();

      reload_as_needed (global);

      gcc_assert (old_frame_size == get_frame_size ());

      if (num_eliminable)
	verify_initial_elim_offsets ();
    }

  /* If we were able to eliminate the frame pointer, show that it is no
     longer live at the start of any basic block.  If it ls live by
     virtue of being in a pseudo, that pseudo will be marked live
     and hence the frame pointer will be known to be live via that
     pseudo.  */

  if (! frame_pointer_needed)
    FOR_EACH_BB (bb)
      CLEAR_REGNO_REG_SET (bb->global_live_at_start,
			   HARD_FRAME_POINTER_REGNUM);

  /* Come here (with failure set nonzero) if we can't get enough spill
     regs.  */
 failed:

  CLEAR_REG_SET (&spilled_pseudos);
  reload_in_progress = 0;

  /* Now eliminate all pseudo regs by modifying them into
     their equivalent memory references.
     The REG-rtx's for the pseudos are modified in place,
     so all insns that used to refer to them now refer to memory.

     For a reg that has a reg_equiv_address, all those insns
     were changed by reloading so that no insns refer to it any longer;
     but the DECL_RTL of a variable decl may refer to it,
     and if so this causes the debugging info to mention the variable.  */

  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    {
      rtx addr = 0;

      if (reg_equiv_mem[i])
	addr = XEXP (reg_equiv_mem[i], 0);

      if (reg_equiv_address[i])
	addr = reg_equiv_address[i];

      if (addr)
	{
	  if (reg_renumber[i] < 0)
	    {
	      rtx reg = regno_reg_rtx[i];

	      REG_USERVAR_P (reg) = 0;
	      PUT_CODE (reg, MEM);
	      XEXP (reg, 0) = addr;
	      if (reg_equiv_memory_loc[i])
		MEM_COPY_ATTRIBUTES (reg, reg_equiv_memory_loc[i]);
	      else
		{
		  MEM_IN_STRUCT_P (reg) = MEM_SCALAR_P (reg) = 0;
		  MEM_ATTRS (reg) = 0;
		}
	    }
	  else if (reg_equiv_mem[i])
	    XEXP (reg_equiv_mem[i], 0) = addr;
	}
    }

  /* We must set reload_completed now since the cleanup_subreg_operands call
     below will re-recognize each insn and reload may have generated insns
     which are only valid during and after reload.  */
  reload_completed = 1;

  /* Make a pass over all the insns and delete all USEs which we inserted
     only to tag a REG_EQUAL note on them.  Remove all REG_DEAD and REG_UNUSED
     notes.  Delete all CLOBBER insns, except those that refer to the return
     value and the special mem:BLK CLOBBERs added to prevent the scheduler
     from misarranging variable-array code, and simplify (subreg (reg))
     operands.  Also remove all REG_RETVAL and REG_LIBCALL notes since they
     are no longer useful or accurate.  Strip and regenerate REG_INC notes
     that may have been moved around.  */

  for (insn = first; insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      {
	rtx *pnote;

	if (CALL_P (insn))
	  replace_pseudos_in (& CALL_INSN_FUNCTION_USAGE (insn),
			      VOIDmode, CALL_INSN_FUNCTION_USAGE (insn));

	if ((GET_CODE (PATTERN (insn)) == USE
	     /* We mark with QImode USEs introduced by reload itself.  */
	     && (GET_MODE (insn) == QImode
		 || find_reg_note (insn, REG_EQUAL, NULL_RTX)))
	    || (GET_CODE (PATTERN (insn)) == CLOBBER
		&& (!MEM_P (XEXP (PATTERN (insn), 0))
		    || GET_MODE (XEXP (PATTERN (insn), 0)) != BLKmode
		    || (GET_CODE (XEXP (XEXP (PATTERN (insn), 0), 0)) != SCRATCH
			&& XEXP (XEXP (PATTERN (insn), 0), 0)
				!= stack_pointer_rtx))
		&& (!REG_P (XEXP (PATTERN (insn), 0))
		    || ! REG_FUNCTION_VALUE_P (XEXP (PATTERN (insn), 0)))))
	  {
	    delete_insn (insn);
	    continue;
	  }

	/* Some CLOBBERs may survive until here and still reference unassigned
	   pseudos with const equivalent, which may in turn cause ICE in later
	   passes if the reference remains in place.  */
	if (GET_CODE (PATTERN (insn)) == CLOBBER)
	  replace_pseudos_in (& XEXP (PATTERN (insn), 0),
			      VOIDmode, PATTERN (insn));

	/* Discard obvious no-ops, even without -O.  This optimization
	   is fast and doesn't interfere with debugging.  */
	if (NONJUMP_INSN_P (insn)
	    && GET_CODE (PATTERN (insn)) == SET
	    && REG_P (SET_SRC (PATTERN (insn)))
	    && REG_P (SET_DEST (PATTERN (insn)))
	    && (REGNO (SET_SRC (PATTERN (insn)))
		== REGNO (SET_DEST (PATTERN (insn)))))
	  {
	    delete_insn (insn);
	    continue;
	  }

	pnote = &REG_NOTES (insn);
	while (*pnote != 0)
	  {
	    if (REG_NOTE_KIND (*pnote) == REG_DEAD
		|| REG_NOTE_KIND (*pnote) == REG_UNUSED
		|| REG_NOTE_KIND (*pnote) == REG_INC
		|| REG_NOTE_KIND (*pnote) == REG_RETVAL
		|| REG_NOTE_KIND (*pnote) == REG_LIBCALL)
	      *pnote = XEXP (*pnote, 1);
	    else
	      pnote = &XEXP (*pnote, 1);
	  }

#ifdef AUTO_INC_DEC
	add_auto_inc_notes (insn, PATTERN (insn));
#endif

	/* And simplify (subreg (reg)) if it appears as an operand.  */
	cleanup_subreg_operands (insn);
      }

  /* If we are doing stack checking, give a warning if this function's
     frame size is larger than we expect.  */
  if (flag_stack_check && ! STACK_CHECK_BUILTIN)
    {
      HOST_WIDE_INT size = get_frame_size () + STACK_CHECK_FIXED_FRAME_SIZE;
      static int verbose_warned = 0;

      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if (regs_ever_live[i] && ! fixed_regs[i] && call_used_regs[i])
	  size += UNITS_PER_WORD;

      if (size > STACK_CHECK_MAX_FRAME_SIZE)
	{
	  warning (0, "frame size too large for reliable stack checking");
	  if (! verbose_warned)
	    {
	      warning (0, "try reducing the number of local variables");
	      verbose_warned = 1;
	    }
	}
    }

  /* Indicate that we no longer have known memory locations or constants.  */
  if (reg_equiv_constant)
    free (reg_equiv_constant);
  reg_equiv_constant = 0;
  VARRAY_GROW (reg_equiv_memory_loc_varray, 0);
  reg_equiv_memory_loc = 0;

  if (offsets_known_at)
    free (offsets_known_at);
  if (offsets_at)
    free (offsets_at);

  free (reg_equiv_mem);
  free (reg_equiv_init);
  free (reg_equiv_address);
  free (reg_max_ref_width);
  free (reg_old_renumber);
  free (pseudo_previous_regs);
  free (pseudo_forbidden_regs);

  /* Free all the insn_chain structures at once.  */
  obstack_free (&reload_obstack, reload_startobj);
  unused_insn_chains = 0;
  fixup_abnormal_edges ();

  /* Replacing pseudos with their memory equivalents might have
     created shared rtx.  Subsequent passes would get confused
     by this, so unshare everything here.  */
  unshare_all_rtl_again (first);

#ifdef STACK_BOUNDARY
  /* init_emit has set the alignment of the hard frame pointer
     to STACK_BOUNDARY.  It is very likely no longer valid if
     the hard frame pointer was used for register allocation.  */
  if (!frame_pointer_needed)
    REGNO_POINTER_ALIGN (HARD_FRAME_POINTER_REGNUM) = BITS_PER_UNIT;
#endif

  return failure;
}

/* Yet another special case.  Unfortunately, reg-stack forces people to
   write incorrect clobbers in asm statements.  These clobbers must not
   cause the register to appear in bad_spill_regs, otherwise we'll call
   fatal_insn later.  We clear the corresponding regnos in the live
   register sets to avoid this.
   The whole thing is rather sick, I'm afraid.  */

static void
maybe_fix_stack_asms (void)
{
#ifdef STACK_REGS
  const char *constraints[MAX_RECOG_OPERANDS];
  enum machine_mode operand_mode[MAX_RECOG_OPERANDS];
  struct insn_chain *chain;

  for (chain = reload_insn_chain; chain != 0; chain = chain->next)
    {
      int i, noperands;
      HARD_REG_SET clobbered, allowed;
      rtx pat;

      if (! INSN_P (chain->insn)
	  || (noperands = asm_noperands (PATTERN (chain->insn))) < 0)
	continue;
      pat = PATTERN (chain->insn);
      if (GET_CODE (pat) != PARALLEL)
	continue;

      CLEAR_HARD_REG_SET (clobbered);
      CLEAR_HARD_REG_SET (allowed);

      /* First, make a mask of all stack regs that are clobbered.  */
      for (i = 0; i < XVECLEN (pat, 0); i++)
	{
	  rtx t = XVECEXP (pat, 0, i);
	  if (GET_CODE (t) == CLOBBER && STACK_REG_P (XEXP (t, 0)))
	    SET_HARD_REG_BIT (clobbered, REGNO (XEXP (t, 0)));
	}

      /* Get the operand values and constraints out of the insn.  */
      decode_asm_operands (pat, recog_data.operand, recog_data.operand_loc,
			   constraints, operand_mode);

      /* For every operand, see what registers are allowed.  */
      for (i = 0; i < noperands; i++)
	{
	  const char *p = constraints[i];
	  /* For every alternative, we compute the class of registers allowed
	     for reloading in CLS, and merge its contents into the reg set
	     ALLOWED.  */
	  int cls = (int) NO_REGS;

	  for (;;)
	    {
	      char c = *p;

	      if (c == '\0' || c == ',' || c == '#')
		{
		  /* End of one alternative - mark the regs in the current
		     class, and reset the class.  */
		  IOR_HARD_REG_SET (allowed, reg_class_contents[cls]);
		  cls = NO_REGS;
		  p++;
		  if (c == '#')
		    do {
		      c = *p++;
		    } while (c != '\0' && c != ',');
		  if (c == '\0')
		    break;
		  continue;
		}

	      switch (c)
		{
		case '=': case '+': case '*': case '%': case '?': case '!':
		case '0': case '1': case '2': case '3': case '4': case 'm':
		case '<': case '>': case 'V': case 'o': case '&': case 'E':
		case 'F': case 's': case 'i': case 'n': case 'X': case 'I':
		case 'J': case 'K': case 'L': case 'M': case 'N': case 'O':
		case 'P':
		  break;

		case 'p':
		  cls = (int) reg_class_subunion[cls]
		    [(int) MODE_BASE_REG_CLASS (VOIDmode)];
		  break;

		case 'g':
		case 'r':
		  cls = (int) reg_class_subunion[cls][(int) GENERAL_REGS];
		  break;

		default:
		  if (EXTRA_ADDRESS_CONSTRAINT (c, p))
		    cls = (int) reg_class_subunion[cls]
		      [(int) MODE_BASE_REG_CLASS (VOIDmode)];
		  else
		    cls = (int) reg_class_subunion[cls]
		      [(int) REG_CLASS_FROM_CONSTRAINT (c, p)];
		}
	      p += CONSTRAINT_LEN (c, p);
	    }
	}
      /* Those of the registers which are clobbered, but allowed by the
	 constraints, must be usable as reload registers.  So clear them
	 out of the life information.  */
      AND_HARD_REG_SET (allowed, clobbered);
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if (TEST_HARD_REG_BIT (allowed, i))
	  {
	    CLEAR_REGNO_REG_SET (&chain->live_before, i);
	    CLEAR_REGNO_REG_SET (&chain->live_after, i);
	  }
    }

#endif
}

static int
conflict (struct reload_reg_use *ru1, struct reload_reg_use *ru2)
{
  return (! ru1->ignored && ! ru2->ignored
	  && ru1->birth < ru2->death && ru2->birth < ru1->death);
}

/* Called from scan_rtx if that function is called for the purpose of marking
   the occurring registers.  mark_reg will update CHAIN->SET_REGS or
   CHAIN->USED_REGS for an occurrence of register X.  If it is inside a
   subreg, WORD contains the value of SUBREG_WORD.  MODE is the mode of the
   access, IS_OUTPUT describes whether the register is being written to.  */
static void
mark_reg (struct insn_chain *chain, int regno, int real_regno, enum machine_mode mode,
	  int is_output, int is_reloaded)
{
  int nregs = 1;
  int i;

  if (real_regno < 0)
    {
      if (! is_reloaded)
	{
	  if (is_output)
	    SET_REGNO_REG_SET (&chain->unreloaded_sets, regno);
	  else
	    SET_REGNO_REG_SET (&chain->unreloaded_uses, regno);
	}
      return;
    }

  if (regno < FIRST_PSEUDO_REGISTER)
    {
      nregs = HARD_REGNO_NREGS (real_regno, mode);
    }
  else
    abort ();

  for (i = 0; i < nregs; i++)
    if (is_output)
      SET_HARD_REG_BIT (chain->set_regs, real_regno + i);
    else
      SET_HARD_REG_BIT (chain->used_regs, real_regno + i);
}

static void
add_feed (struct reload_insn *source, struct reload_insn *dest)
{
  int space = source->feeds_space;
  if (source->n_feeds == space)
    {
      space += 5;
      if (source->n_feeds == 0)
	source->feeds = (struct reload_insn **) xmalloc (space * sizeof (struct reload_insn *));
      else
	source->feeds = (struct reload_insn **) xrealloc (source->feeds,
							  space * sizeof (struct reload_insn *));
      source->feeds_space = space;
    }
  source->feeds[source->n_feeds] = dest;
  source->n_feeds++;
  source->n_feeds_remaining++;
  source->status = RLIS_NOT_SCHEDULED;
  dest->status = RLIS_NOT_SCHEDULED;
}

static void
add_reg_feed (struct insn_chain *chain, int regno, int real_regno,
	      enum machine_mode mode, struct reload_insn *rli, int is_output)
{
  int scan_start = 1 + 2 * chain->n_reloads + (is_output != 0 ? chain->n_input_regs : 0);
  int scan_count = (is_output == 0 ? chain->n_input_regs : chain->n_output_regs);
  int nregs, i;

  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (real_regno >= 0)
	abort ();
      return;
    }

  nregs = HARD_REGNO_NREGS (real_regno, mode);
  for (i = 0; i < nregs; i++)
    {
      int j;
      struct reload_insn *reg_insn = chain->rli + scan_start;

      for (j = 0; j < scan_count; j++, reg_insn++)
	{
	  if (reg_insn->nr == real_regno + i)
	    break;
	}
      if (j == scan_count)
	abort ();
      if (is_output == 0)
	add_feed (reg_insn, rli);
      else
	{
	  if (is_output == 2)
	    chain->reg_usage[j + chain->n_input_regs].earlyclobber |= is_output == 2;
	  add_feed (rli, reg_insn);
	}
    }
}

/* Walk the rtx found in *LOC, which occurs inside the insn described by
   CHAIN.

   We are looking for register references; JUST_MARK determines
   what we do with them.  If it is nonzero, we compute the two reg sets
   CHAIN->SET_REGS and CHAIN->USED_REGS, otherwise we compute
   dependence information for the reload_insn structures of CHAIN.

   If we compute dependency information, RLI contains the reload insn that
   we are "inside", this information is gathered by looking at the
   replacements set up by find_reloads.  The caller must call subst_dummy
   before calling scan_rtx so that we can detect replacements.

   IS_OUTPUT determines whether the piece of rtl we are scanning is being
   written to; it is 1 for normal writes, 2 for earlyclobbers, and 0 for
   reads.

   NO_ADDRESSES can be used to skip memory addresses.  */
static void
scan_rtx (struct insn_chain *chain, rtx *loc, int is_output,
	  struct reload_insn *rli, int no_addresses, int just_mark,
	  int is_reloaded)
{
  const char *fmt;
  rtx x = *loc;
  enum rtx_code code = GET_CODE (x);
  enum machine_mode mode = GET_MODE (x);
  int i, j, regno, real_regno;

  if (x == &dummy_replacement_rtx)
    {
      /* Look up the reload for this replacement and continue scanning inside
	 the replaced contents.  */
      int k = replacement_nr (loc);
      int reload_nr = replacements[k].what;
      rtx contents = replacements[k].contents;
      struct reload_insn *reload_in_insn, *reload_out_insn;
      struct reload *rl;

      if (reload_nr < 0)
	abort ();
      rl = chain->rld + reload_nr;

      if ((! rl->in && ! rl->out) || rl->secondary_p)
	abort ();

      if (is_output != 0 && ! rl->out)
	abort ();
      if (is_output == 0 && ! rl->in)
	abort ();

      if (just_mark)
	{
	  if (is_output == 0)
	    {
	      if (rl->optional)
		scan_rtx (chain, &contents, 0, 0, 0, 1, 1);
	      else
		scan_rtx (chain, &rl->in, 0, 0, 0, 1, 1);
	    }
	  else
	    {
	      if (rl->optional)
		scan_rtx (chain, &contents, is_output, 0, 0, 1, 1);
	      else
		scan_rtx (chain, &rl->out, is_output, 0, 0, 1, 1);
	    }
	  return;
	}

      reload_in_insn = chain->rli + 1 + reload_nr * 2;
      reload_out_insn = chain->rli + 2 + reload_nr * 2;

      if (is_output == 0)
	{
	  add_feed (reload_in_insn, rli);
	  if (rl->scanned_input)
	    return;
	  if (! just_mark)
	    rl->scanned_input = 1;
	  if (rl->optional)
	    scan_rtx (chain, &contents, 0, reload_in_insn, 0, 0, 1);
	  else
	    scan_rtx (chain, &rl->in, 0, reload_in_insn, 0, 0, 1);
	}
      else
	{
	  rl->reginfo.earlyclobber |= is_output == 2;
	  add_feed (rli, reload_out_insn);
	  if (rl->scanned_output)
	    return;
	  if (! just_mark)
	    rl->scanned_output = 1;
	  if (rl->optional)
	    scan_rtx (chain, &contents, is_output, reload_out_insn, 0, 0, 1);
	  else
	    scan_rtx (chain, &rl->out, is_output, reload_out_insn, 0, 0, 1);

	}
      return;
    }

  code = GET_CODE (x);
  fmt = GET_RTX_FORMAT (code);
  switch (code)
    {
    case SUBREG:
      if (GET_CODE (SUBREG_REG (x)) != REG)
	break;
      regno = REGNO (SUBREG_REG (x));
      real_regno = (regno >= FIRST_PSEUDO_REGISTER
		    ? reg_renumber[regno] : regno);

      if (real_regno >= 0)
	real_regno += subreg_regno_offset (real_regno,
					   GET_MODE (SUBREG_REG (x)),
					   SUBREG_BYTE (x), GET_MODE (x));

      /* @@@ This is probably going to trigger some day... */
      if (x == &dummy_replacement_rtx)
	abort ();

      goto reg_subreg_common;

    case REG:
      regno = REGNO (x);
      real_regno = (regno >= FIRST_PSEUDO_REGISTER
		    ? reg_renumber[regno] : regno);
    reg_subreg_common:
      if (just_mark)
	mark_reg (chain, regno, real_regno, mode, is_output, is_reloaded);
      else
	add_reg_feed (chain, regno, real_regno, mode, rli, is_output);
      return;

    case MEM:
      if (! no_addresses)
	scan_rtx (chain, &XEXP (x, 0), 0, rli, 0, just_mark, 0);
      return;

    case SET:
      scan_rtx (chain, &SET_SRC (x), 0, rli, 0, just_mark, 0);
      scan_rtx (chain, &SET_DEST (x), 1, rli, 0, just_mark, 0);
      return;

    case STRICT_LOW_PART:
      scan_rtx (chain, &XEXP (x, 0), 0, rli, 0, just_mark, 0);
      scan_rtx (chain, &XEXP (x, 0), is_output, rli, 0, just_mark, 0);
      return;

    case ZERO_EXTRACT:
    case SIGN_EXTRACT: 
      scan_rtx (chain, &XEXP (x, 0), 0, rli, 0, just_mark, 0);
      if (is_output)
	scan_rtx (chain, &XEXP (x, 0), is_output, rli, 0, just_mark, 0);
      scan_rtx (chain, &XEXP (x, 1), 0, rli, 0, just_mark, 0);
      scan_rtx (chain, &XEXP (x, 2), 0, rli, 0, just_mark, 0);
      return;

    case POST_INC:
    case PRE_INC:
    case POST_DEC:
    case PRE_DEC:
      scan_rtx (chain, &XEXP (x, 0), 0, rli, 0, just_mark, 0);
      scan_rtx (chain, &XEXP (x, 0), 1, rli, 0, just_mark, 0);
      return;

    case CLOBBER:
      scan_rtx (chain, &SET_DEST (x), 2, rli, 0, just_mark, 0);
      return;

    default:
      break;
    }

  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	scan_rtx (chain, &XEXP (x, i), is_output, rli, no_addresses,
		  just_mark, 0);
      else if (fmt[i] == 'E')
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  scan_rtx (chain, &XVECEXP (x, i, j), is_output, rli, no_addresses,
		    just_mark, 0);
    }
}

/* Generate dependencies for the secondary memory rtx's.  We need to do this
   because secondary memory may have address reloads.  */
static void
scan_secondary_mem (struct insn_chain *chain, int just_mark)
{
#ifdef SECONDARY_MEMORY_NEEDED
  int i;

  for (i = 0; i < chain->n_reloads; i++)
    {
      struct reload *rl = chain->rld + i;
      if (rl->in
	  && GET_CODE (rl->in) == REG
	  && REGNO (rl->in) < FIRST_PSEUDO_REGISTER
	  && SECONDARY_MEMORY_NEEDED (REGNO_REG_CLASS (REGNO (rl->in)),
				      rl->class, rl->inmode))
	{
	  rtx mem = get_secondary_mem (NULL_RTX, rl->inmode, i, RELOAD_FOR_NONE);
	  scan_rtx (chain, &XEXP (mem, 0), 0, chain->rli + 1 + i * 2, 0,
		    just_mark, 0);
	}
      if (rl->out
	  && GET_CODE (rl->out) == REG
	  && REGNO (rl->out) < FIRST_PSEUDO_REGISTER
	  && SECONDARY_MEMORY_NEEDED (REGNO_REG_CLASS (REGNO (rl->out)),
				      rl->class, rl->outmode))
	{
	  rtx mem = get_secondary_mem (NULL_RTX, rl->outmode, i, RELOAD_FOR_NONE);
	  scan_rtx (chain, &XEXP (mem, 0), 0, chain->rli + 2 + i * 2, 0,
		    just_mark, 0);
	}
    }
#endif
}

static inline struct reload_insn *
rli_from_reload_nr (struct insn_chain *chain, int nr, int output)
{
  return chain->rli + (output ? 2 : 1) + nr * 2;
}

/* Compute dependency information for the reloads and hard regs in CHAIN.  */
static void
compute_feeds (struct insn_chain *chain)
{
  int count;
  int i;

  for (i = 0; i < chain->n_reloads; i++)
    {
      chain->rld[i].scanned_input = 0;
      chain->rld[i].scanned_output = 0;
      chain->rld[i].reginfo.earlyclobber = 0;
    }

  extract_insn (chain->insn_copy);

  subst_dummy ();

  for (i = 0; i < recog_data.n_operands; i++)
    {
      if (recog_data.operand_type[i] != OP_OUT)
	scan_rtx (chain, recog_data.operand_loc[i], 0, chain->rli, 0, 0, 0);
      if (recog_data.operand_type[i] != OP_IN)
	{
	  int ec = earlyclobber_operand_p (recog_data.operand[i]);
	  scan_rtx (chain, recog_data.operand_loc[i], 1 + ec, chain->rli,
		    recog_data.operand_type[i] == OP_INOUT, 0, 0);
	}
    }
  
  scan_secondary_mem (chain, 0);

  undo_subst_dummy ();

  /* We're scanning only the operands of the insn.  Hard register references
     may be outside the operands, so see if we have any RLIS_IGNORED
     registers.  */
  count = chain->n_input_regs + chain->n_output_regs;
  for (i = 0; i < count; i++)
    {
      struct reload_insn *rli = chain->rli + 1 + 2 * chain->n_reloads + i;
      if (rli->status != RLIS_IGNORED)
	continue;
      if (rli->type == RLI_INPUTREG)
	add_feed (rli, chain->rli);
      else if (rli->type == RLI_OUTPUTREG)
	add_feed (chain->rli, rli);
      else
	abort ();
    }

}

/* Determine whether we need to actually emit an output reload for RL.  */
static int
output_reload_unnecessary (struct insn_chain *chain, struct reload *rl)
{
  rtx old = rl->out_reg;

  if (GET_CODE (old) == SCRATCH)
    return 1;
  if (GET_CODE (old) == SUBREG)
    old = SUBREG_REG (old);

  if (REG_P (old)
      && find_reg_note (chain->insn, REG_UNUSED, old) != 0)
    return 1;
  return 0;
}

/* Generate a valid order for the reload insns, taking into account the
   feeder/user relationships between them.  This is basically a "mini
   scheduler".  It gives out time slots to every reload insn and the main
   insn we're reloading.  Each reload insn gets two time slots, and the main
   insn three.  */
static void
compute_reload_order (struct insn_chain *chain)
{
  int count = 1 + 2 * chain->n_reloads + chain->n_input_regs + chain->n_output_regs;
  int i, j;
  int last = -1;
  int pos;

  pos = 0;
  for (;;)
    {
      struct reload_insn *rli;
      int any_available = 0;
      /* Find an insn that can be scheduled at this point.  */
      for (i = 0; i < count; i++)
	{
	  rli = chain->rli + i;
	  if (rli->status != RLIS_NOT_SCHEDULED)
	    continue;
	  any_available = 1;
	  if (rli->n_feeds_remaining > 0)
	    continue;
	  break;
	}
      if (i == count)
	{
	  if (any_available)
	    abort ();
	  else
	    break;
	}

      if (last == -1)
	chain->last_rlinsn = i;
      else
	chain->rli[last].prev_order = i;
      last = i;

      /* Leave three slots instead of two for the main insn.  */
      if (i == 0)
	pos--;

      rli->order = pos;
      pos -= 2;
      rli->status = RLIS_SCHEDULED;
      /* Reduce the remaining feed count for all insns that feed this one.  */
      for (j = 0; j < count; j++)
	{
	  int k;
	  struct reload_insn *rlj = chain->rli + j;
	  if (rlj->status != 1 || rlj->n_feeds_remaining == 0)
	    continue;
	  for (k = 0; k < rlj->n_feeds; k++)
	    if (rlj->feeds[k] == rli)
	      rlj->n_feeds_remaining--;
	}
    }
  if (last < 0)
    abort ();
  chain->rli[last].prev_order = -1;

  /* Make the order values start at 1.  */
  for (i = 0; i < count; i++)
    if (chain->rli[i].status == RLIS_SCHEDULED)
      chain->rli[i].order -= pos + 1;

  /* Correct the order for unnecessary output reloads:  these die within the
     main insn's final slot.  */
  for (i = 0; i < chain->n_reloads; i++)
    {
      struct reload *rl = chain->rld + i;
      if (rl->out_reg != 0
	  && output_reload_unnecessary (chain, rl))
	{
	  struct reload_insn *rli = chain->rli + 2 + 2 * i;
	  rli->order = chain->rli[0].order + 1;
	  rli->unnecessary = 1;
	}
    }
}

/* The following two functions deal with computing proper values of
   the "order" field in a reload_insn for reloads that are optional.
   An optional reload can be enabled or disabled; if it's enabled its
   order is used normally, otherwise we pretend it isn't there.  */
static int
rlinsn_order (struct insn_chain *chain, struct reload_insn *rli)
{
  if (rli->type != RLI_OUTPUTRELOAD
      || ! chain->rld[rli->nr].optional
      || chain->rld[rli->nr].enabled)
    return rli->order;
  return chain->rli[0].order;
}

static int
rlinsn_last_use (struct insn_chain *chain, struct reload_insn *rli)
{
  int j;
  int retval = 0;

  /* An input reload dies no earlier than its last use.  */
  for (j = 0; j < rli->n_feeds; j++)
    {
      struct reload_insn *rlj = rli->feeds[j];
      int order;

      if (rlj->ignored)
	continue;

      order = rlinsn_order (chain, rlj);

      /* Optional reloads are transparent for this purpose.  */
      if (rlj->type == RLI_INPUTRELOAD
	  && chain->rld[rlj->nr].optional
	  && ! chain->rld[rlj->nr].enabled)
	order = rlinsn_last_use (chain, rlj);

      if (order > retval)
	retval = order;
    }
  return retval;
}

/* This function computes the IGNORED fields of reload insns and hard register
   usage information.  Ignored reload insns are never emitted, and ignored
   hard regs do not conflict with anything.
   The following cases are handled:
     - If an output reload is unnecessary (because the output register is dead
       after the insn), then it is ignored.  There will be an output register
       for it, which will also be marked ignored.
       @@@ Still need to handle reg that dies, but has no output reload.  This
           will give better code.
     - If we decide to perform an optional input reload, there may be address
       reloads for the now reloaded rtx.  These are unnecessary and will be
       ignored.  This can also affect input regs.  */
static void
compute_ignored (struct insn_chain *chain)
{
  int count = 1 + 2 * chain->n_reloads + chain->n_input_regs + chain->n_output_regs;
  int i, changed;

  /* Initialization.  */
  for (i = 0; i < count; i++)
    chain->rli[i].n_feeds_remaining = 0;

  for (i = 0; i < count; i++)
    {
      int j;
      struct reload_insn *rli = chain->rli + i;
      rli->ignored = rli->unnecessary;

      if (! rli->ignored)
	for (j = 0; j < rli->n_feeds; j++)
	  rli->feeds[j]->n_feeds_remaining++;
    }

  /* Iterate until no more reload_insns are marked ignored.  */
  do
    {
      changed = 0;
      for (i = 0; i < count; i++)
	{
	  struct reload_insn *rli = chain->rli + i;
	  int j, needed = 0;

	  if (rli->status == RLIS_IGNORED || rli->ignored)
	    continue;

	  /* Can't ignore an input reload that is the head for inheritance.  */
	  if (rli->type == RLI_INPUTRELOAD
	      && chain->rld[rli->nr].out == 0
	      && chain->rld[rli->nr].reginfo.live_until_end)
	    needed = 1;

	  if (rli->type == RLI_OUTPUTREG)
	    {
	      if (rli->n_feeds_remaining != 0)
		needed = 1;
	    }
	  else if (rli->type == RLI_INPUTRELOAD || rli->type == RLI_INPUTREG)
	    {
	      for (j = 0; j < rli->n_feeds; j++)
		{
		  struct reload_insn *fed = rli->feeds[j];
		  if ((fed->type != RLI_INPUTRELOAD
		       || chain->rld[fed->nr].override_in == 0)
		      && ! rli->feeds[j]->ignored)
		    {
		      needed = 1;
		      break;
		    }
		}
	    }
	  else
	    continue;

	  if (! needed)
	    {
	      rli->ignored = 1;
	      changed = 1;
	      for (j = 0; j < rli->n_feeds; j++)
		rli->feeds[j]->n_feeds_remaining--;
	    }
	}
    }
  while (changed);

  /* Propagate the ignored flag from the reload_insns to the register life
     information for hard regs.  */
  for (i = 0; i < chain->n_output_regs + chain->n_input_regs; i++)
    {
      struct reload_reg_use *ru = chain->reg_usage + i;
      struct reload_insn *rli = chain->rli + 1 + 2 * chain->n_reloads + i;
      if (rli->ignored)
	ru->ignored = 1;
    }
}

/* Find out the birth and death positions for reloads and hard register
   references.  They are computed from the relative positions of the
   reload_insn structures after these have been scheduled in
   compute_reload_order. */
static void
compute_birth_death (struct insn_chain *chain)
{
  int count = 1 + 2 * chain->n_reloads + chain->n_input_regs + chain->n_output_regs;
  int i;
  int maxpos;
  HARD_REG_SET live_after, live_before, tmp;

  REG_SET_TO_HARD_REG_SET (live_before, &chain->live_before);
  compute_use_by_pseudos (&live_before, &chain->live_before);
  REG_SET_TO_HARD_REG_SET (live_after, &chain->live_after);
  compute_use_by_pseudos (&live_after, &chain->live_after);

  /* This is an additional optimization.  For pseudos allocated to a multi-word
     hard register, LIVE_BEFORE will contain each of the single hard regs, even
     if this insn stores into one of them.  We can take them out again very
     easily with the following three lines.  */
  COPY_HARD_REG_SET (tmp, chain->set_regs);
  AND_COMPL_HARD_REG_SET (tmp, chain->used_regs);
  AND_COMPL_HARD_REG_SET (live_before, tmp);

  maxpos = chain->rli[chain->last_rlinsn].order;

  compute_ignored (chain);
  
  /* Step 1: Initialize birth and death positions to default values.  */
  for (i = 0; i < chain->n_reloads; i++)
    {
      chain->rld[i].reginfo.birth = maxpos + 1;
      chain->rld[i].reginfo.death = chain->rld[i].out ? chain->rli[0].order + 1 : 0;
    }
  for (i = 0; i < chain->n_input_regs; i++)
    {
      struct reload_reg_use *ru = chain->reg_usage + i;
      int regno = ru->regno;
      ru->birth = 0;
      /* For a reg that dies in this insn, initialize death to 0;
	 it will get increased for every use found.
	 If the reg does not die in this insn, we may not record a
	 death earlier than the last time slot.  However, if we know
	 that this insn sets the register, we know we'll have a
	 corresponding entry in the output regs section which will
	 properly record the fact that this reg is live beyond the insn.  */
      ru->death = ((! TEST_HARD_REG_BIT (live_after, regno)
		    || TEST_HARD_REG_BIT (chain->set_regs, regno))
		   ? 0 : maxpos + 1);
    }
  for (i = 0; i < chain->n_output_regs; i++)
    {
      struct reload_reg_use *ru = chain->reg_usage + chain->n_input_regs + i;
      int regno = ru->regno;
      ru->death = maxpos + 1;
      /* Similar issue as for the input regs above.  */
      ru->birth = ((! TEST_HARD_REG_BIT (live_before, regno)
		    || TEST_HARD_REG_BIT (chain->used_regs, regno))
		   ? maxpos + 1 : 0);
    }

  /* Step 2: Look at all the reload insns, and adjust live ranges for the
     involved registers.  */

  /* Process the inputs.  */
  for (i = 0; i < count; i++)
    {
      struct reload_insn *rli = chain->rli + i;
      struct reload_reg_use *ru;
      if (rli->ignored || rli->status != RLIS_SCHEDULED)
	continue;
      if (rli->type == RLI_INPUTRELOAD)
	{
	  ru = &chain->rld[rli->nr].reginfo;
	  /* An input reload is born at the place where it is loaded.  */
	  ru->birth = rlinsn_order (chain, rli);
	}
      else if (rli->type == RLI_INPUTREG)
	{
	  ru = chain->reg_usage + i - 2 * chain->n_reloads - 1;
	  if (ru->regno != chain->rli[i].nr)
	    abort ();
	}
      else
	continue;

      /* An input reload dies no earlier than its last use.  */
      ru->death = MAX (ru->death, rlinsn_last_use (chain, rli));
    }

  /* Process the outputs.  */
  for (i = 0; i < count; i++)
    {
      int j;
      struct reload_insn *rli = chain->rli + i;
      int order;

      if (rli->ignored
	  || rli->status != RLIS_SCHEDULED
	  || (rli->type != RLI_OUTPUTRELOAD && rli->type != RLI_INSN))
	continue;

      order = rlinsn_order (chain, rli);
      /* An output reload dies at the place where it is stored into its
	 final destination.  */
      if (rli->type == RLI_OUTPUTRELOAD)
	chain->rld[rli->nr].reginfo.death = order;

      /* Anything fed by an output gets born no later than the time of the
	 output.  */
      for (j = 0; j < rli->n_feeds; j++)
	{
	  struct reload_insn *rlj = rli->feeds[j];
	  struct reload_reg_use *ru_fed;
	  if (rlj->type == RLI_OUTPUTRELOAD)
	    ru_fed = &chain->rld[rlj->nr].reginfo;
	  else if (rlj->type == RLI_OUTPUTREG)
	    {
	      ru_fed = chain->reg_usage + (rlj - chain->rli) - 2 * chain->n_reloads - 1;
	      if (ru_fed->regno != rlj->nr)
		abort ();
	    }
	  else
	    continue;
	  ru_fed->birth = MIN (ru_fed->birth, order);
	}
    }

  /* Perform necessary adjustments for earlyclobbered operands.  If they
     currently are marked as being born at the main insn, decrement their
     birth by one so it matches the earlyclobber slot.  */
  for (i = 0; i < chain->n_reloads; i++)
    {
      struct reload_reg_use *ru = &chain->rld[i].reginfo;
      if (ru->ignored || ! ru->earlyclobber)
	continue;
      if (ru->birth == chain->rli[0].order)
	ru->birth--;
    }
  for (i = 0; i < chain->n_output_regs; i++)
    {
      struct reload_reg_use *ru = chain->reg_usage + i + chain->n_input_regs;
      if (ru->ignored || ! ru->earlyclobber)
	continue;
      if (ru->birth == chain->rli[0].order)
	ru->birth--;
    }

  /* Time slots for secondary reloads are computed from the time slots of
     their primary reloads.
     Walk reloads in reverse order so that we first see the primary, then
     the secondary, and finally any tertiary reloads.  */
  i = chain->n_reloads;
  while (i-- > 0)
    {
      struct reload *rl = chain->rld + i;

      if (rl->secondary_in_reload >= i || rl->secondary_out_reload >= i)
	abort ();

      if (rl->secondary_in_reload >= 0)
	{
	  struct reload *rl2 = chain->rld + rl->secondary_in_reload;
	  rl2->reginfo.birth = rl->reginfo.birth;
	  rl2->reginfo.death = rl->reginfo.birth;
	}
      if (rl->secondary_out_reload >= 0)
	{
	  struct reload *rl2 = chain->rld + rl->secondary_out_reload;
	  rl2->reginfo.birth = rl->reginfo.death;
	  rl2->reginfo.death = rl->reginfo.death;
	}
    }
  /* Second pass over the secondary reloads, this time to make sure secondary
     reloads and their primaries conflict.  */
  for (i = 0; i < chain->n_reloads; i++)
    {
      struct reload *rl = chain->rld + i;
      if (rl->secondary_in_reload >= 0)
	{
	  struct reload *rl2 = chain->rld + rl->secondary_in_reload;
	  /* Make the secondary conflict with the input register.  */
	  rl2->reginfo.birth--;
	  /* Make secondary conflict with its primary.  */
	  rl->reginfo.birth--;
	}
      if (rl->secondary_out_reload >= 0)
	{
	  struct reload *rl2 = chain->rld + rl->secondary_out_reload;
	  rl2->reginfo.birth--;
	}
    }
  /* Extend lifetimes for things that need to live past the insn.  */
  for (i = 0; i < chain->n_reloads; i++)
    {
      struct reload_reg_use *ru = &chain->rld[i].reginfo;

      if (ru->live_until_end)
	ru->death = maxpos + 1;
    }
}

/* Enable an optional reload and recompute birth/death positions for
   CHAIN.  */
static void
enable_optional (struct insn_chain *chain, struct reload *rl)
{
  if (! rl->optional || rl->enabled)
    return;
  if (rl->reginfo.allocated)
    abort ();
  rl->enabled = 1;
  compute_birth_death (chain);
}

/* Disable an optional reload and recompute birth/death positions for
   CHAIN.  */
static void
disable_optional (struct insn_chain *chain, struct reload *rl)
{
  if (! rl->optional || ! rl->enabled)
    return;
  rl->enabled = 0;
  rl->reginfo.allocated = 0;
  compute_birth_death (chain);
}

/* For the insn described by CHAIN, set up all the data structures we need
   to allocate reload registers later on.  */
static void
init_rlinsns (struct insn_chain *chain)
{
  struct reload_insn *rli;
  struct reload_reg_use *ru;
  int i, count;
  HARD_REG_SET tmp;
  int n_regs;

  REG_SET_TO_HARD_REG_SET (chain->hard_live_across, &chain->live_before);
  REG_SET_TO_HARD_REG_SET (tmp, &chain->live_after);
  AND_HARD_REG_SET (chain->hard_live_across, tmp);

  CLEAR_HARD_REG_SET (chain->pseudo_hard_live_across);
  compute_use_by_pseudos (&chain->pseudo_hard_live_across, &chain->live_before);
  compute_use_by_pseudos (&tmp, &chain->live_after);
  AND_HARD_REG_SET (chain->pseudo_hard_live_across, tmp);

  AND_COMPL_HARD_REG_SET (chain->hard_live_across, chain->set_regs);
  AND_COMPL_HARD_REG_SET (chain->hard_live_across, chain->used_regs);
  AND_COMPL_HARD_REG_SET (chain->pseudo_hard_live_across, chain->set_regs);
  AND_COMPL_HARD_REG_SET (chain->pseudo_hard_live_across, chain->used_regs);

  /* Count hard register references.  */
  chain->n_input_regs = 0;
  chain->n_output_regs = 0;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (TEST_HARD_REG_BIT (chain->set_regs, i))
	chain->n_output_regs++;
      if (TEST_HARD_REG_BIT (chain->used_regs, i))
	chain->n_input_regs++;
    }

  /* Set up reload_insn and reload_reg_use structures.  */
  n_regs = chain->n_input_regs + chain->n_output_regs;
  count = n_regs + 2 * chain->n_reloads + 1;

  rli = (struct reload_insn *) xmalloc (sizeof (struct reload_insn) * count);
  chain->rli = rli;
  memset (rli, 0, sizeof (struct reload_insn) * count);
  ru = (struct reload_reg_use *) xmalloc (sizeof (struct reload_reg_use) * n_regs);
  chain->reg_usage = ru;
  memset (ru, 0, sizeof (struct reload_reg_use) * n_regs);
  rli->type = RLI_INSN;
  rli->status = RLIS_NOT_SCHEDULED;
  rli++;

  for (i = 0; i < chain->n_reloads; i++)
    {
      rli->type = RLI_INPUTRELOAD;
      rli->status = RLIS_IGNORED;
      rli->nr = i;
      rli++;
      rli->type = RLI_OUTPUTRELOAD;
      rli->status = RLIS_IGNORED;
      rli->nr = i;
      rli++;
    }
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (chain->used_regs, i))
      {
	rli->type = RLI_INPUTREG;
	rli->status = RLIS_IGNORED;
	rli->nr = i;
	rli++;
	ru->regno = i;
	ru->hardreg = REGNO_REG_SET_P (&chain->live_before, i);
	ru++;
      }
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (chain->set_regs, i))
      {
	rli->type = RLI_OUTPUTREG;
	rli->status = RLIS_IGNORED;
	rli->nr = i;
	rli++;
	ru->regno = i;
	ru->hardreg = REGNO_REG_SET_P (&chain->live_after, i);
	ru++;
      }
}

/* After find_reloads has run for CHAIN, save the information it gathered
   for later.  This will also set up lifetime information for the reloads
   and registers used by the insn.  */
static void
copy_reloads (struct insn_chain *chain)
{
  int i;

  CLEAR_HARD_REG_SET (chain->used_regs);
  CLEAR_HARD_REG_SET (chain->set_regs);

  chain->rld = (struct reload *) xmalloc (chain->n_reloads
					  * sizeof (struct reload));
  memcpy (chain->rld, rld, chain->n_reloads * sizeof (struct reload));

  /* Find out which hard regs are set or used inside this insn.  */
  subst_dummy ();
  scan_rtx (chain, &PATTERN (chain->insn_copy), 0, 0, 0, 1, 0);
  scan_secondary_mem (chain, 1);
  undo_subst_dummy ();

  init_rlinsns (chain);

  for (i = 0; i < chain->n_reloads; i++)
    {
      struct reload *rl = chain->rld + i;
      rl->reginfo.allocated = 0;
      rl->reginfo.live_until_end = 0;
      rl->override_in = rl->override_out = 0;
#if 0
      if (rl->reg_rtx != 0)
	abort ();
#endif
    }

  compute_feeds (chain);
  compute_reload_order (chain);
  compute_birth_death (chain);
}

/* Walk the chain of insns, and determine for each whether it needs reloads
   and/or eliminations.  Build the corresponding insns_need_reload list, and
   set something_needs_elimination as appropriate.  */
static void
calculate_needs_all_insns (int global)
{
  struct insn_chain **pprev_reload = &insns_need_reload;
  struct insn_chain *chain, *next = 0;

  something_needs_elimination = 0;

  reload_insn_firstobj = obstack_alloc (&reload_obstack, 0);
  for (chain = reload_insn_chain; chain != 0; chain = next)
    {
      rtx insn = chain->insn;

      next = chain->next;

      /* Clear out the shortcuts.  */
      chain->n_reloads = 0;
      chain->need_elim = 0;
      chain->need_operand_change = 0;

      CLEAR_HARD_REG_SET (chain->inherited_live);
      CLEAR_REG_SET (&chain->unreloaded_sets);
      CLEAR_REG_SET (&chain->unreloaded_uses);

      /* If this is a label, a JUMP_INSN, or has REG_NOTES (which might
	 include REG_LABEL), we need to see what effects this has on the
	 known offsets at labels.  */

      if (LABEL_P (insn) || JUMP_P (insn)
	  || (INSN_P (insn) && REG_NOTES (insn) != 0))
	set_label_offsets (insn, insn, 0);

      if (INSN_P (insn))
	{
	  rtx copy;
	  int did_elimination = 0;
	  int operands_changed = 0;

	  copy = copy_insn (insn);
	  chain->insn_copy = copy;

	  /* If needed, eliminate any eliminable registers.  */
	  if (num_eliminable || num_eliminable_invariants)
	    did_elimination = eliminate_regs_in_insn (copy, 0);

	  if (GET_MODE (copy) == SImode)
	    {
	      chain->will_be_deleted = 1;
	      /* Make sure we call reload_as_needed to delete this insn.  */
	      something_needs_elimination = 1;
	      continue;
	    }

	  /* Analyze the instruction.  */
	  operands_changed = find_reloads (chain, copy, spill_indirect_levels,
					   global);

	  if (num_eliminable)
	    update_eliminable_offsets ();

	  /* Remember for later shortcuts which insns had any reloads or
	     register eliminations.  */
	  chain->need_elim = did_elimination;
	  chain->need_operand_change = operands_changed;

	  something_needs_elimination |= did_elimination;
	  something_needs_operands_changed |= operands_changed;

	  if (chain->n_reloads != 0)
	    {
	      copy_reloads (chain);
	      *pprev_reload = chain;
	      pprev_reload = &chain->next_need_reload;
	    }
	  else
	    scan_rtx (chain, &PATTERN (chain->insn_copy), 0, 0, 0, 1, 0);
	}
    }
  *pprev_reload = 0;
}

void
visualize_chain (struct insn_chain *chain)
{
  
}

/* Comparison function for qsort to decide which of two reloads
   should be handled first.  *P1 and *P2 are the reload numbers.  */

static int
reload_reg_class_lower (const void *r1p, const void *r2p)
{
  int r1 = *(const short *) r1p, r2 = *(const short *) r2p;
  int t;

  /* Consider required reloads before optional ones.  */
  t = rld[r1].optional - rld[r2].optional;
  if (t != 0)
    return t;

  /* Count all solitary classes before non-solitary ones.  */
  t = ((reg_class_size[(int) rld[r2].class] == 1)
       - (reg_class_size[(int) rld[r1].class] == 1));
  if (t != 0)
    return t;

  /* Aside from solitaires, consider all multi-reg groups first.  */
  t = rld[r2].nregs - rld[r1].nregs;
  if (t != 0)
    return t;

  /* Consider reloads in order of increasing reg-class number.  */
  t = (int) rld[r1].class - (int) rld[r2].class;
  if (t != 0)
    return t;

  /* If reloads are equally urgent, sort by reload number,
     so that the results of qsort leave nothing to chance.  */
  return r1 - r2;
}

/* The cost of spilling each hard reg.  */
static int spill_cost[FIRST_PSEUDO_REGISTER];

/* When spilling multiple hard registers, we use SPILL_COST for the first
   spilled hard reg and SPILL_ADD_COST for subsequent regs.  SPILL_ADD_COST
   only the first hard reg for a multi-reg pseudo.  */
static int spill_add_cost[FIRST_PSEUDO_REGISTER];

/* Update the spill cost arrays, considering that pseudo REG is live.  */

static void
count_pseudo (int reg)
{
  int freq = REG_FREQ (reg);
  int r = reg_renumber[reg];
  int nregs;

  if (REGNO_REG_SET_P (&pseudos_counted, reg)
      || REGNO_REG_SET_P (&spilled_pseudos, reg))
    return;

  SET_REGNO_REG_SET (&pseudos_counted, reg);

  gcc_assert (r >= 0);

  spill_add_cost[r] += freq;

  nregs = hard_regno_nregs[r][PSEUDO_REGNO_MODE (reg)];
  while (nregs-- > 0)
    spill_cost[r + nregs] += freq;
}

/* Calculate the SPILL_COST and SPILL_ADD_COST arrays and determine the
   contents of BAD_SPILL_REGS for the insn described by CHAIN.  */

static void
order_regs_for_reload (struct insn_chain *chain)
{
  unsigned i;
  HARD_REG_SET used_by_pseudos;
  HARD_REG_SET used_by_pseudos2;
  reg_set_iterator rsi;

  COPY_HARD_REG_SET (bad_spill_regs, fixed_reg_set);

  memset (spill_cost, 0, sizeof spill_cost);
  memset (spill_add_cost, 0, sizeof spill_add_cost);

  /* Count number of uses of each hard reg by pseudo regs allocated to it
     and then order them by decreasing use.  First exclude hard registers
     that are live in or across this insn.  */

  REG_SET_TO_HARD_REG_SET (used_by_pseudos, &chain->live_before);
  REG_SET_TO_HARD_REG_SET (used_by_pseudos2, &chain->live_after);
  IOR_HARD_REG_SET (bad_spill_regs, used_by_pseudos);
  IOR_HARD_REG_SET (bad_spill_regs, used_by_pseudos2);

  /* Now find out which pseudos are allocated to it, and update
     hard_reg_n_uses.  */
  CLEAR_REG_SET (&pseudos_counted);

  EXECUTE_IF_SET_IN_REG_SET
    (&chain->live_before, FIRST_PSEUDO_REGISTER, i, rsi)
     {
       count_pseudo (i);
     }
  EXECUTE_IF_SET_IN_REG_SET
    (&chain->live_after, FIRST_PSEUDO_REGISTER, i, rsi)
     {
       count_pseudo (i);
     }
  CLEAR_REG_SET (&pseudos_counted);
}

/* Vector of reload-numbers showing the order in which the reloads should
   be processed.  */
static short reload_order[MAX_RELOADS];

/* This is used to keep track of the spill regs used in one insn.  */
static HARD_REG_SET used_spill_regs_local;

/* We decided to spill hard register SPILLED, which has a size of
   SPILLED_NREGS.  Determine how pseudo REG, which is live during the insn,
   is affected.  We will add it to SPILLED_PSEUDOS if necessary, and we will
   update SPILL_COST/SPILL_ADD_COST.  */

static void
count_spilled_pseudo (int spilled, int spilled_nregs, int reg)
{
  int r = reg_renumber[reg];
  int nregs = hard_regno_nregs[r][PSEUDO_REGNO_MODE (reg)];

  if (REGNO_REG_SET_P (&spilled_pseudos, reg)
      || spilled + spilled_nregs <= r || r + nregs <= spilled)
    return;

  SET_REGNO_REG_SET (&spilled_pseudos, reg);

  spill_add_cost[r] -= REG_FREQ (reg);
  while (nregs-- > 0)
    spill_cost[r + nregs] -= REG_FREQ (reg);
}

/* Determine whether pseudo REGNO conflicts with the reload RL.
   We have no information about lifetime of pseudos, but we do know about
   hard register lifetimes.  We only check for hard register references of
   type TYPE (either RLI_INPUTREG or RLI_OUTPUTREG).  */
static int
no_conflict_p (struct insn_chain *chain, int regno, enum rlinsn_type type,
	       struct reload *rl)
{
  int hardreg = reg_renumber[regno];
  int i, nregs;
  int scan_end = chain->n_input_regs + chain->n_output_regs;

  if (hardreg < 0)
    abort ();

  nregs = HARD_REGNO_NREGS (hardreg, PSEUDO_REGNO_MODE (regno));
  for (i = 0; i < nregs; i++)
    {
      /* It's possible that only parts of a pseudo are in
	 pseudo_hard_live_across; this can happen if this insn stores into
	 one reg of a multi-reg pseudo.  There is also code in
	 compute_birth_death to handle that case.  */
      if (hardreg + i >= rl->reginfo.regno
	  && hardreg + i < rl->reginfo.regno + rl->nregs
	  && TEST_HARD_REG_BIT (chain->pseudo_hard_live_across, hardreg + i))
	return 0;
    }
  for (i = 0; i < scan_end; i++)
    {
      int test_reg = chain->reg_usage[i].regno;
      enum rlinsn_type t = chain->rli[i + 2 * chain->n_reloads + 1].type;
      if (test_reg < hardreg || test_reg >= hardreg + nregs)
	continue;
      if (t == type && conflict (chain->reg_usage + i, &rl->reginfo))
	return 0;
    }
  return 1;
}

/* Compute the set of hard registers which can be used for reload R in insn
   CHAIN without spilling a pseudo, even though they are used somewhere in
   the insn.  */
static void
compute_not_conflicting (struct insn_chain *chain, struct reload *rl,
			 HARD_REG_SET *set)
{
  int i;

  CLEAR_HARD_REG_SET (*set);

  /* Two loops are necessary because the same register may occur twice in
     our data structures, and one of them may conflict but not the other.  */
  for (i = 0; i < chain->n_input_regs + chain->n_output_regs; i++)
    SET_HARD_REG_BIT (*set, chain->reg_usage[i].regno);

  for (i = 0; i < chain->n_input_regs + chain->n_output_regs; i++)
    if (conflict (&rl->reginfo, chain->reg_usage + i))
      CLEAR_HARD_REG_BIT (*set, chain->reg_usage[i].regno);

  AND_COMPL_HARD_REG_SET (*set, chain->pseudo_hard_live_across);
}

static int
new_reg_preferred (int old_reg, int new_reg, int old_cost, int new_cost)
{
  if (new_cost < old_cost
      /* Among registers with equal cost, prefer caller-saved ones, or
	 use REG_ALLOC_ORDER if it is defined.  */
      || (new_cost == old_cost
#ifdef REG_ALLOC_ORDER
	  && (inv_reg_alloc_order[new_reg]
	      < inv_reg_alloc_order[old_reg])
#else
	  && call_used_regs[new_reg]
	  && ! call_used_regs[old_reg]
#endif
	  ))
    return 1;
  return 0;
}

/* Compute the set of registers which can't be used for reload RL of insn
   CHAIN.  It consists of explicit hard registers used in the insn whose
   lifetime overlaps the reload, and hard registers already allocated to
   other reloads that conflict with RL.  The set is stored in *SET.  */
static void
compute_not_usable (struct insn_chain *chain, struct reload *rl,
		    HARD_REG_SET *set)
{
  int i;

  COPY_HARD_REG_SET (*set, bad_spill_regs_global);
  IOR_COMPL_HARD_REG_SET (*set, reg_class_contents[rl->class]);
  IOR_HARD_REG_SET (*set, chain->hard_live_across);

  /* Process conflicts with explicit hard registers.  */
  for (i = 0; i < chain->n_input_regs + chain->n_output_regs; i++)
    {
      struct reload_reg_use *ru = chain->reg_usage + i;
      if (ru->hardreg && conflict (&rl->reginfo, ru))
	SET_HARD_REG_BIT (*set, ru->regno);
    }

  /* Process conflicts with other reloads.  */
  for (i = 0; i < chain->n_reloads; i++)
    {
      unsigned int j;
      struct reload *other = chain->rld + i;
      unsigned int regno = other->reginfo.regno;
      unsigned int nregs;

      if (other == rl
	  || ! other->reginfo.allocated
	  || ! conflict (&other->reginfo, &rl->reginfo))
	continue;

      nregs = HARD_REGNO_NREGS (regno, other->mode);
      for (j = 0; j < nregs; j++)
	SET_HARD_REG_BIT (*set, regno + j);
    }

  /* CALL_INSNS need some special handling.  We could allocate proper
     reload_reg_use structures and all that for call-clobbered and
     argument registers, but this is easier.  */
  if (GET_CODE (chain->insn) == CALL_INSN)
    {
      HARD_REG_SET clobbered, used;
      rtx x;
      int call_order = chain->rli[0].order;

      COPY_HARD_REG_SET (clobbered, call_used_reg_set);
      CLEAR_HARD_REG_SET (used);

      for (x = CALL_INSN_FUNCTION_USAGE (chain->insn); x; x = XEXP (x, 1))
	{
	  rtx y = XEXP (x, 0);
	  enum rtx_code code = GET_CODE (y);
	  int regno, nregs;

	  if (code != CLOBBER && code != USE)
	    abort ();
	  y = XEXP (y, 0);
	  if (GET_CODE (y) == MEM)
	    continue;
	  if (GET_CODE (y) != REG)
	    abort ();
	  regno = REGNO (y);
	  nregs = HARD_REGNO_NREGS (regno, GET_MODE (y));
	  while (nregs-- > 0)
	    if (code == CLOBBER)
	      SET_HARD_REG_BIT (clobbered, regno + nregs);
	    else
	      SET_HARD_REG_BIT (used, regno + nregs);
	}
      if (rl->reginfo.birth < call_order)
	{
	  IOR_HARD_REG_SET (*set, used);
	  if (rl->reginfo.death > call_order)
	    IOR_HARD_REG_SET (*set, clobbered);
	}
    }

  /* Process conflicts with inherited registers.  */
  for (i = 0; i < chain->n_reloads; i++)
    {
      unsigned int j;
      struct reload *other = chain->rld + i;
      unsigned int regno, nregs;
      if (other == rl
	  || other->override_in == 0
	  || other->reginfo.birth < rl->reginfo.birth)
	continue;

      regno = REGNO (other->override_in);
      nregs = HARD_REGNO_NREGS (regno, other->mode);
      for (j = 0; j < nregs; j++)
	SET_HARD_REG_BIT (*set, regno + j);
    }
  IOR_HARD_REG_SET (*set, chain->inherited_live);
}

static void
set_reload_reg (struct reload *rl, unsigned regno)
{
  int i;

  rl->nregs = HARD_REGNO_NREGS (regno, rl->mode);
  rl->reginfo.regno = regno;
  rl->reginfo.allocated = 1;
  for (i = 0; i < rl->nregs; i++)
    SET_HARD_REG_BIT (used_spill_regs_local, regno + i);
}

/* Find reload register to use for reload number ORDER.  REG_RTX, if nonnull,
   was suggested by find_reloads.  */

static int
find_reg (struct insn_chain *chain, int order, rtx reg_rtx)
{
  int rnum = reload_order[order];
  struct reload *rl = chain->rld + rnum;
  int best_cost = INT_MAX;
  int best_reg = -1;
  unsigned i;
  HARD_REG_SET not_conflicting;
  HARD_REG_SET not_usable;
  reg_set_iterator rsi;

  compute_not_conflicting (chain, rl, &not_conflicting);
  compute_not_usable (chain, rl, &not_usable);

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      unsigned int regno = i;
      if (! TEST_HARD_REG_BIT (not_usable, regno)
	  && HARD_REGNO_MODE_OK (regno, rl->mode))
	{
	  int exception = TEST_HARD_REG_BIT (not_conflicting, regno);
	  int this_cost = exception ? 0 : spill_cost[regno];
	  int ok = 1;
	  int j, this_nregs = HARD_REGNO_NREGS (regno, rl->mode);

	  for (j = 1; j < this_nregs; j++)
	    {
	      exception = TEST_HARD_REG_BIT (not_conflicting, regno + j);
	      this_cost += exception ? 0 : spill_add_cost[regno + j];
	      if (TEST_HARD_REG_BIT (not_usable, regno + j))
		ok = 0;
	    }
	  if (! ok)
	    continue;
	  if (reg_rtx && REGNO (reg_rtx) == regno)
	    {
	      best_reg = regno;
	      break;
	    }
	  if (rl->in && REG_P (rl->in) && REGNO (rl->in) == regno)
	    this_cost--;
	  if (rl->out && REG_P (rl->out) && REGNO (rl->out) == regno)
	    this_cost--;
	  if (new_reg_preferred (best_reg, regno, best_cost, this_cost))
	    {
	      best_reg = regno;
	      best_cost = this_cost;
	    }
	}
    }

  if (best_reg == -1)
    return 0;

  if (dump_file)
    fprintf (dump_file, "Using reg %d for reload %d\n", best_reg, rnum);

  set_reload_reg (rl, best_reg);

  EXECUTE_IF_SET_IN_REG_SET
    (&chain->live_after, FIRST_PSEUDO_REGISTER, i, rsi)
     {
       if (! no_conflict_p (chain, i, RLI_OUTPUTREG, rl))
	 count_spilled_pseudo (best_reg, rl->nregs, i);
     }

  EXECUTE_IF_SET_IN_REG_SET
    (&chain->live_before, FIRST_PSEUDO_REGISTER, i, rsi)
     {
       if (! no_conflict_p (chain, i, RLI_INPUTREG, rl))
	 count_spilled_pseudo (best_reg, rl->nregs, i);
     }


  return 1;
}

/* Find more reload regs to satisfy the remaining need of an insn, which
   is given by CHAIN.
   Do it by ascending class number, since otherwise a reg
   might be spilled for a big class and might fail to count
   for a smaller class even though it belongs to that class.  */

static void
find_reload_regs (struct insn_chain *chain)
{
  int i;

  memcpy (rld, chain->rld, chain->n_reloads * sizeof (struct reload));

  CLEAR_HARD_REG_SET (used_spill_regs_local);

  if (dump_file)
    fprintf (dump_file, "Spilling for insn %d.\n", INSN_UID (chain->insn));

  /* In order to be certain of getting the registers we need,
     we must sort the reloads into order of increasing register class.  */
  for (i = 0; i < chain->n_reloads; i++)
    reload_order[i] = i;

  qsort (reload_order, chain->n_reloads, sizeof (short),
	 reload_reg_class_lower);

  /* Compute the order of preference for hard registers to spill.  */
  order_regs_for_reload (chain);

  for (i = 0; i < chain->n_reloads; i++)
    {
      struct reload *rl = chain->rld + reload_order[i];

      /* Ignore reloads that got marked inoperative.  */
      if ((rl->out != 0 || rl->in != 0 || rl->secondary_p)
	  && ! rl->optional
	  && ! rl->reginfo.allocated)
	if (! find_reg (chain, i, rl->reg_rtx))
	  {
	    spill_failure (chain->insn, rl->class);
	    failure = 1;
	    return;
	  }
    }

  COPY_HARD_REG_SET (chain->used_spill_regs, used_spill_regs_local);
  IOR_HARD_REG_SET (used_spill_regs, used_spill_regs_local);
}

/* The following functions all deal with reload inheritance.  */

/* Given a reload RL for the insn described by CHAIN, compute the set of
   hard registers which can be used without having to spill a pseudo.  */
static void
compute_unallocated (struct insn_chain *chain, struct reload *rl, HARD_REG_SET *set)
{
  unsigned i;
  HARD_REG_SET tmp;
  reg_set_iterator rsi;

  CLEAR_HARD_REG_SET (tmp);

  EXECUTE_IF_SET_IN_REG_SET
    (&chain->live_before, FIRST_PSEUDO_REGISTER, i, rsi)
    {
      int r = reg_renumber[i];
      int last = r + HARD_REGNO_NREGS (r, PSEUDO_REGNO_MODE (i));

      if (r < 0)
	abort ();

      do 
	SET_HARD_REG_BIT (tmp, r);
      while (++r != last);
    }
  EXECUTE_IF_SET_IN_REG_SET
    (&chain->live_after, FIRST_PSEUDO_REGISTER, i, rsi)
    {
      int r = reg_renumber[i];
      int last = r + HARD_REGNO_NREGS (r, PSEUDO_REGNO_MODE (i));

      if (r < 0)
	abort ();
      do 
	SET_HARD_REG_BIT (tmp, r);
      while (++r != last);
    }
  COMPL_HARD_REG_SET (*set, tmp);

  compute_not_conflicting (chain, rl, &tmp);
  IOR_HARD_REG_SET (*set, tmp);

  compute_not_usable (chain, rl, &tmp);
  AND_COMPL_HARD_REG_SET (*set, tmp);
}

/* Like find_reg, but modified to handle optional reloads during inheritance.
   This function does not need to worry about spilling pseudos, it only ever
   considers regs that are definitely free.  */
static int
find_optional_reg (struct reload *rl, HARD_REG_SET *usable, int suggested,
		   int do_allocate)
{
  int i;
  int best_reg = -1;
  int best_cost = INT_MAX;

  if (suggested >= 0)
    {
      int nregs = HARD_REGNO_NREGS (suggested, rl->mode);
      for (i = 0; i < nregs; i++)
	if (! TEST_HARD_REG_BIT (*usable, suggested + i))
	  break;
      if (i == nregs)
	{
	  if (do_allocate)
	    set_reload_reg (rl, suggested);
	  return 1;
	}
    }

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      int nregs, k, cost;
      if (! HARD_REGNO_MODE_OK (i, rl->mode))
	continue;
      nregs = HARD_REGNO_NREGS (i, rl->mode);
      for (k = 0; k < nregs; k++)
	if (! TEST_HARD_REG_BIT (*usable, i + k))
	  break;

      if (k < nregs)
	continue;
      cost = 0;
      if (new_reg_preferred (best_reg, i, best_cost, cost))
	best_cost = cost, best_reg = i;
    }
  if (best_reg == -1)
    return 0;

  if (do_allocate)
    set_reload_reg (rl, best_reg);

  return 1;
}

/* Subroutine of compute_set_between.  Called via note_stores.  */
static void
note_hard_reg_sets (rtx x, rtx setter ATTRIBUTE_UNUSED, void *data)
{
  enum machine_mode mode = GET_MODE (x);
  HARD_REG_SET *set = (HARD_REG_SET *)data;
  int offset = 0;
  int nregs, regno;

  if (GET_CODE (x) == SUBREG)
    {
      offset = subreg_regno_offset (REGNO (SUBREG_REG (x)),
				  GET_MODE (SUBREG_REG (x)),
				  SUBREG_BYTE (x),
				  GET_MODE (x));
      x = SUBREG_REG (x);
    }

  if (GET_CODE (x) != REG || REGNO (x) >= FIRST_PSEUDO_REGISTER)
    return;

  regno = REGNO (x) + offset;
  nregs = HARD_REGNO_NREGS (regno, mode);

  while (nregs-- > 0)
    SET_HARD_REG_BIT (*set, regno + nregs);
}

/* Compute the set of hard registers which are clobbered by insns between FROM
   and TO (excluding these boundary insns).  */
static void
compute_set_between (HARD_REG_SET *set, struct insn_chain *from,
		     struct insn_chain *to)
{
  CLEAR_HARD_REG_SET (*set);

  while (from->next != to)
    {
      int i;

      from = from->next;

      if (! INSN_P (from->insn) || from->will_be_deleted)
	continue;

      if (from->n_reloads == 0)
	note_stores (PATTERN (from->insn), note_hard_reg_sets, set);
      else
	{
	  for (i = 0; i < from->n_reloads; i++)
	    if (from->rld[i].reginfo.allocated)
	      {
		int nregs = HARD_REGNO_NREGS (from->rld[i].reginfo.regno, from->rld[i].mode);
		while (nregs-- > 0)
		  SET_HARD_REG_BIT (*set, from->rld[i].reginfo.regno + nregs);
	      }
	  for (i = 0; i < from->n_output_regs; i++)
	    SET_HARD_REG_BIT (*set, from->reg_usage[from->n_input_regs + i].regno);
	}
      if (GET_CODE (from->insn) == CALL_INSN)
	IOR_HARD_REG_SET (*set, call_used_reg_set);
    }
}

/* Subroutine of find_clobbered_between_reloads.  This function handles the
   case of two reloads in the same insn.  */
static void
find_clobbered_between_reloads_1 (HARD_REG_SET *total_conflicts,
				  struct insn_chain *chain,
				  struct reload *src_rl, struct reload *rl)
{
  int i;
  int inherit_start = src_rl->reginfo.birth;
  int inherit_end = rl->reginfo.birth;

  CLEAR_HARD_REG_SET (*total_conflicts);

  for (i = 0; i < chain->n_reloads; i++)
    {
      struct reload *rl = chain->rld + i;
      struct reload_reg_use *ru = &rl->reginfo;
      int rl_set_order = chain->rli[0].order;

      if (! ru->allocated || ru->birth >= inherit_end)
	continue;
      if (rl->reginfo.earlyclobber)
	rl_set_order--;
      if (ru->birth > inherit_start
	  || (rl->out && rl_set_order > inherit_start))
	{
	  int nregs = HARD_REGNO_NREGS (ru->regno, chain->rld[i].mode);
	  while (nregs-- > 0)
	    SET_HARD_REG_BIT (*total_conflicts, ru->regno + nregs);
	}
    }
  for (i = 0; i < chain->n_output_regs; i++)
    {
      struct reload_reg_use *ru = chain->reg_usage + chain->n_input_regs + i;
      if (ru->birth > inherit_start && ru->birth < inherit_end)
	SET_HARD_REG_BIT (*total_conflicts, ru->regno);
    }
}

/* When inheriting reloads, we need to find out which hard registers will be
   clobbered between two given reloads.  These reloads are described by
   SRC_RL (the earlier) and RL (the later reload), with SRC_CHAIN and CHAIN
   being the insns they appear in.  Store the conflicts in
   TOTAL_CONFLICTS.  */
static void
find_clobbered_between_reloads (HARD_REG_SET *total_conflicts,
				struct insn_chain *src_chain,
				struct reload *src_rl,
				struct insn_chain *chain, struct reload *rl,
				int src_is_head)
{
  int i;
  int inherit_start = src_rl->reginfo.birth;
  int inherit_end = rl->reginfo.birth;

  HARD_REG_SET in_conflicts;
  HARD_REG_SET out_conflicts;

  if (chain == src_chain)
    {
      find_clobbered_between_reloads_1 (total_conflicts, chain, src_rl, rl);
      return;
    }

  compute_set_between (total_conflicts, src_chain, chain);

  /* Compute the registers which conflict with the outgoing value on
     SRC_CHAIN.  */
  CLEAR_HARD_REG_SET (out_conflicts);
  for (i = 0; i < src_chain->n_reloads; i++)
    {
      struct reload *rli = src_chain->rld + i;
      int nregs;
      /* If the previous reload has an output part, its register will be
	 clobbered, so we must count it as a conflict (unless it's the
	 head reload).  */
      if ((rli == src_rl && src_rl->out != 0 && ! src_is_head)
	  || (rli != src_rl && rli->reginfo.allocated && rli->reginfo.birth >= inherit_start))
	{
	  nregs = HARD_REGNO_NREGS (rli->reginfo.regno, rli->mode);
	  while (nregs-- > 0)
	    SET_HARD_REG_BIT (out_conflicts, rli->reginfo.regno + nregs);
	}
    }
  for (i = 0; i < src_chain->n_output_regs; i++)
    {
      struct reload_reg_use *ru = src_chain->reg_usage + src_chain->n_input_regs + i;
      if (! ru->ignored && ru->birth > inherit_start)
	SET_HARD_REG_BIT (out_conflicts, ru->regno);
    }
  if (GET_CODE (src_chain->insn) == CALL_INSN
      && src_rl->reginfo.birth < src_chain->rli[0].order)
    IOR_HARD_REG_SET (out_conflicts, call_used_reg_set);

  /* Compute the registers which conflict with the incoming value on
     CHAIN.  */
  CLEAR_HARD_REG_SET (in_conflicts);
  for (i = 0; i < chain->n_reloads; i++)
    {
      struct reload *rli = chain->rld + i;
      int nregs;
      if (rli == rl)
	continue;
      if (rli->reginfo.birth > inherit_end || ! rli->reginfo.allocated)
	continue;
      nregs = HARD_REGNO_NREGS (rli->reginfo.regno, rli->mode);
      while (nregs-- > 0)
	SET_HARD_REG_BIT (in_conflicts, rli->reginfo.regno + nregs);
    }
  for (i = 0; i < chain->n_output_regs; i++)
    {
      struct reload_reg_use *ru = chain->reg_usage + chain->n_input_regs + i;
      if (! ru->ignored && ru->birth < inherit_end)
	SET_HARD_REG_BIT (in_conflicts, ru->regno);
    }

  IOR_HARD_REG_SET (*total_conflicts, out_conflicts);
  IOR_HARD_REG_SET (*total_conflicts, in_conflicts);
}

/* Describe an inheritance opportunity.  For all reloaded rtx's, we create
   a list of all places in the current block which reload the same rtx.
   These lists are ordered; earlier uses precede later ones.  */
struct inherit_chain
{
  struct inherit_chain *next_chain;
  /* The next inheritance opportunity on this chain.  */
  struct inherit_chain *next_same;

  /* The set of registers clobbered between the previous reload and this
     one.  */
  HARD_REG_SET new_conflicts;
  /* The place for this inheritance opportunity.  */
  struct insn_chain *chain;
  struct reload_insn *rli;
  unsigned int was_enabled:1;
  unsigned int used_after:1;
};

/* All inheritance chains we found, linked through their NEXT_CHAIN
   fields.  */
static struct inherit_chain *all_inherit_chains;

/* For post-processing, all chains that actually were used for inheritance.  */
static struct inherit_chain *done_inherit_chains;

/* Indexed by pseudo register, gives the last inheritance opportunity we found
   for that pseudo.  */
static struct inherit_chain **pseudo_inherit;

/* Undo actions done in inherit_one_chain: turn off all optional reloads that
   weren't enabled beforehand.  Do that for all places recorded in the chain
   from HEAD until the end of the chain.  */

static void
undo_enables (struct inherit_chain *head, struct inherit_chain *last)
{
  while (head != last)
    {
      int rl_nr = head->rli->nr;
      struct reload *rl = head->chain->rld + rl_nr;

      if (rl->optional && ! head->was_enabled)
	disable_optional (head->chain, rl);

      head = head->next_same;
    }
}

/* Return nonzero if it's not too expensive to use hard reg REGNO, used in
   mode MODE, as an input override for a reload register in class CLASS.  */

static int
suitable_for_copy (int regno, enum machine_mode mode, enum reg_class class)
{
  enum reg_class regno_class = REGNO_REG_CLASS (regno);
  int reg_move_cost = REGISTER_MOVE_COST (mode, regno_class, class);
  int mem_move_cost = MEMORY_MOVE_COST (mode, class, 1);

  /* This used to be a greater or equal comparison.  At least for the i386,
     better code is generated by a greater than comparison, and I'm guessing
     other machines also benefit from this.  */
  if (regno_class != class && reg_move_cost > mem_move_cost)
    return 0;

#ifdef SECONDARY_INPUT_RELOAD_CLASS
  PUT_MODE (permanent_test_reg, mode);
  REGNO (permanent_test_reg) = regno;
    
  if (SECONDARY_INPUT_RELOAD_CLASS (class, mode, permanent_test_reg)
      != NO_REGS)
    return 0;
#endif

#ifdef SECONDARY_MEMORY_NEEDED
  if (SECONDARY_MEMORY_NEEDED (regno_class, class, mode))
    return 0;
#endif

  return 1;
}

enum inherit_type
{
  IT_NONE,
  IT_OVERRIDE_INPUT,
  IT_USE_REG
};

/* We test for two things for each register: whether it can be used as input
   for this reload (if not, we can't use it at all), and whether it can even
   be used as the reload register itself (which gives better code if
   possible).  */
static enum inherit_type
usable_for_inheritance (rtx head_rtx, struct reload *rl, rtx this_rtx,
			int regno, HARD_REG_SET *usable_regs,
			HARD_REG_SET *unallocated)
{
  enum machine_mode mode = rl->mode;
  enum machine_mode inmode = rl->inmode;
  int can_use_inheritance_reg = rl->out_reg == 0;
  int nregs, k;
  int offsetted_regno = regno;

  if (GET_CODE (head_rtx) == REG && GET_CODE (this_rtx) == SUBREG)
    offsetted_regno += subreg_regno_offset (regno,
					    GET_MODE (SUBREG_REG (this_rtx)),
					    SUBREG_BYTE (this_rtx),
					    GET_MODE (this_rtx));

  if (GET_MODE (head_rtx) != inmode)
    {
#ifdef CLASS_CANNOT_CHANGE_MODE
      if (TEST_HARD_REG_BIT (reg_class_contents[CLASS_CANNOT_CHANGE_MODE],
			     regno)
	  && CLASS_CANNOT_CHANGE_MODE_P (regno, mode))
	return IT_NONE;
#endif
    }

  /* To be usable at all, the register must be valid in the mode we are
     going to use for the input.  */
  if (! HARD_REGNO_MODE_OK (offsetted_regno, inmode))
    return IT_NONE;

  /* Check whether it's valid to use the register as reload reg.  For this,
     it must be valid in MODE, not just INMODE.  */

  if (mode != inmode && ! HARD_REGNO_MODE_OK (offsetted_regno, mode))
    {
      nregs = HARD_REGNO_NREGS (offsetted_regno, inmode);

      for (k = 0; k < nregs; k++)
	if (! TEST_HARD_REG_BIT (*usable_regs, offsetted_regno + k))
	  return IT_NONE;

      can_use_inheritance_reg = 0;
    }
  else
    {
      nregs = HARD_REGNO_NREGS (offsetted_regno, mode);

      for (k = 0; k < nregs; k++)
	{
	  if (! TEST_HARD_REG_BIT (*usable_regs, offsetted_regno + k))
	    return IT_NONE;
	  if (! TEST_HARD_REG_BIT (*unallocated, offsetted_regno + k))
	    can_use_inheritance_reg = 0;
	}
    }

  /* Can we inherit for this opportunity?  */
  if (! can_use_inheritance_reg
      && ! suitable_for_copy (offsetted_regno, inmode, rl->class))
    return IT_NONE;

  return can_use_inheritance_reg ? IT_USE_REG : IT_OVERRIDE_INPUT;
}

/* Given the inheritance opportunity THIS, which is for reload THIS_RL,
   try to use BEST_REG for inheritance.  USABLE_REGS contains all the
   registers which have not been clobbered since the head insn.
   Return zero if we could not do inheritance for this insn, nonzero
   otherwise.  */

static int
perform_inheritance (rtx head_rtx, struct inherit_chain *this,
		     struct reload *this_rl, int best_reg,
		     HARD_REG_SET *usable_regs)
{
  rtx this_rtx = (this->rli->type == RLI_OUTPUTRELOAD
		  ? this_rl->out_reg : this_rl->in_reg);
  enum machine_mode mode = this_rl->mode;
  enum inherit_type itype;
  HARD_REG_SET this_usable;
  int offsetted_regno = best_reg;

  if (GET_CODE (head_rtx) == REG && GET_CODE (this_rtx) == SUBREG)
    offsetted_regno += subreg_regno_offset (best_reg,
					    GET_MODE (SUBREG_REG (this_rtx)),
					    SUBREG_BYTE (this_rtx),
					    GET_MODE (this_rtx));

  compute_unallocated (this->chain, this_rl, &this_usable);

  itype = usable_for_inheritance (head_rtx, this_rl, this_rtx, best_reg,
				  usable_regs, &this_usable);
  if (itype == IT_NONE)
    return 0;

  if (this_rl->optional && ! this->was_enabled)
    find_optional_reg (this_rl, &this_usable, offsetted_regno, 1);
  else if (itype == IT_USE_REG)
    {
      this_rl->nregs = HARD_REGNO_NREGS (offsetted_regno, mode);
      this_rl->reginfo.regno = offsetted_regno;
      /* We're overriding - this must already have been allocated earlier.  */
      if (! this_rl->reginfo.allocated)
	abort ();
    }

  this_rl->override_in = gen_rtx_REG (this_rl->inmode, offsetted_regno);

  return 1;
}

/* While processing a chain of inheritance opportunities that starts with
   HEAD, we arrived at position THIS.  Return nonzero if we must stop
   inheritance at this point.  */
static int
must_terminate_inheritance (struct reload *head_rl,
			    struct reload_insn *head_rli,
			    struct reload *this_rl,
			    struct reload_insn *this_rli)
{
  rtx head_rtx = (head_rli->type == RLI_OUTPUTRELOAD
		  ? head_rl->out_reg : head_rl->in_reg);
  rtx this_rtx = (this_rli->type == RLI_OUTPUTRELOAD
		  ? this_rl->out_reg : this_rl->in_reg);

  /* If the head rtx is a subreg, terminate as soon as we see the full reg
     or a different subreg.
     @@@ This may possibly lose a few opportunities, but it probably isn't
     too big a deal.  */
  if (GET_CODE (head_rtx) == SUBREG
      && (GET_CODE (this_rtx) != SUBREG
	  || SUBREG_BYTE (this_rtx) != SUBREG_BYTE (head_rtx)))
    return 1;

  /* Can't inherit from a narrower rtx.  */
  if (GET_MODE_SIZE (GET_MODE (head_rtx))
      < GET_MODE_SIZE (GET_MODE (this_rtx)))
    return 1;

  return 0;
}

/* This is the first pass done when trying to perform inheritance for a given
   chain of opportunities.  It determines which registers are best suited for
   the optimization.  */
static struct inherit_chain *
evaluate_regs_for_inherit (struct inherit_chain *head,
			   HARD_REG_SET *usable_for_head, int *goodness)
{
  unsigned int i;
  struct inherit_chain *this, *prev = head;
  struct reload *head_rl = head->chain->rld + head->rli->nr;
  struct reload *prev_rl = head_rl;
  rtx head_rtx = (head->rli->type == RLI_OUTPUTRELOAD
		  ? head_rl->out_reg : head_rl->in_reg);
  HARD_REG_SET usable_regs;

  prev->was_enabled = prev_rl->enabled;
  enable_optional (prev->chain, prev_rl);

  COPY_HARD_REG_SET (usable_regs, *usable_for_head);

  this = prev->next_same;
  /* In each iteration, try to extend the span during which the reload is
     inherited.  We keep track of which registers we can use in the
     USABLE_FOR_HEAD set.  */
  while (this != 0)
    {
      HARD_REG_SET this_usable;
      struct reload *this_rl = this->chain->rld + this->rli->nr;
      rtx this_rtx = (this->rli->type == RLI_OUTPUTRELOAD
		      ? this_rl->out_reg : this_rl->in_reg);

      if (dump_file)
	fprintf (dump_file, "  at insn %d rl %d: ",
		 INSN_UID (this->chain->insn), this->rli->nr);

      if (must_terminate_inheritance (head_rl, head->rli, this_rl, this->rli))
	{
	  if (dump_file)
	    fprintf (dump_file, "  must terminate\n");
	  break;
	}
      if (this->rli->type == RLI_OUTPUTRELOAD)
	{
	  if (dump_file)
	    fprintf (dump_file, "  is output reload\n");
	  break;
	}

      this->was_enabled = this_rl->enabled;
      enable_optional (this->chain, this_rl);

      /* We can't use a register that's clobbered between these two
	 reloads.  */
      find_clobbered_between_reloads (&this->new_conflicts, prev->chain,
				      prev_rl, this->chain, this_rl, prev == head);
      AND_COMPL_HARD_REG_SET (usable_regs, this->new_conflicts);

      if (dump_file)
	{
	  int i;
	  fprintf (dump_file, "new conflicts: ");
	  for (i = 0; i < FIRST_PSEUDO_REGISTER; ++i)
	    if (TEST_HARD_REG_BIT (this->new_conflicts, i))
	      fprintf (dump_file, "%s ", reg_names[i]);
	  fprintf (dump_file, "\n");
	}
      /* Find out which registers we can use as reload register for this
	 reload.  */
      compute_unallocated (this->chain, this_rl, &this_usable);

      if (this->rli->ignored)
	goto ignore;

      /* No use trying to inherit for an optional reload for which no
	 register is available anymore.  */
      if (this_rl->optional && ! this->was_enabled)
	if (! find_optional_reg (this_rl, &this_usable, -1, 0))
	  goto ignore;

      /* Walk through all hard regs and determine their desirability.  */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	{
	  enum inherit_type itype;

	  itype = usable_for_inheritance (head_rtx, this_rl, this_rtx, i,
					  &usable_regs, &this_usable);
	  if (itype != IT_NONE)
	    goodness[i] += itype == IT_USE_REG ? 2 : 1;
	}

    ignore:
      prev = this;
      prev_rl = this_rl;
      this = prev->next_same;
    }

  return this;
}

static int
allocation_goodness (struct insn_chain *chain, struct reload *rl, unsigned regno)
{
  int goodness = 0;

  if (rl->override_in && REGNO (rl->override_in) == regno)
    goodness++;
#if 0
  if (rl->out && GET_CODE (rl->out) == REG && REGNO (rl->out) == regno)
    goodness++;
  if (chain->n_reloads == 1)
    {
      rtx set = single_set (chain->insn);
      if (set)
	{
	  if (rl->out && ! rl->in)
	    ;
	}
    }
#endif
  return goodness;
}

static bool
sets_full_reg_p (struct inherit_chain *head)
{
  struct reload *head_rl = head->chain->rld + head->rli->nr;
  rtx head_rtx = (head->rli->type == RLI_OUTPUTRELOAD
		  ? head_rl->out_reg : head_rl->in_reg);

  if (head->rli->type != RLI_OUTPUTRELOAD)
    return false;
  if (GET_CODE (head_rtx) != REG)
    return false;
  return true;
}

static bool
jump_insn_between_p (struct insn_chain *first, struct insn_chain *last)
{
  struct insn_chain *chain;
  for (chain = first; chain != last; chain = chain->next)
    if (GET_CODE (chain->insn) == JUMP_INSN)
      return true;
  return false;
}

/* Process one chain of inheritance opportunities.  All entries on the
   chain reload the same rtx (modulo subregs).

   The strategy:
     - The first reload in the chain is the base from which all the others
       inherit.  Its reload register will be used to override the inputs of
       the following reloads.  We choose a suitable register for it here,
       even if we already allocated one.  The goal is to maximize the
       lifetime of the inheritance register, and to make sure it is usable
       as input for the following reloads.
     - For every following reload, test the available possible inheritance
       registers to make sure they can be used as input.  If it's an
       input-only reload, we can also determine whether we can use the
       inheritance register as reload register.  This isn't necessary, but
       cheaper, and we count it that way.

   So, what about output reloads, then?  In inherit_reloads, chains are
   generated for all kinds of reload insns, no matter whether they are for
   input for or output.  If we have an in-out reload which is both loaded
   from a pseudo and stored to the same pseudo, we will see the same reload
   twice in this function, once for the input and once for the output.

   As soon as we find an output reload in the inherit chain, we stop
   processing.  The chain is cut in half: the first half which we have
   already examined is processed, the other half which starts with the
   new output reload is put back into the ALL_INHERIT_CHAINS list so
   that we will process it during the next iteration.
   This means that such an in-out reload may be used for inheritance twice:
   first, inheriting from another reload; and second, as a base for other
   reloads that can inherit from it.  */

static void
inherit_one_chain (struct inherit_chain *head)
{
  struct inherit_chain *last_performed;

  unsigned int i, noninherited_use;
  /* For each hard reg, count the number of inheritances we can perform if
     we select it.  */
  int goodness[FIRST_PSEUDO_REGISTER];
  struct inherit_chain *this, *last, *prev = head;
  struct reload *head_rl = head->chain->rld + head->rli->nr;
  struct reload *prev_rl = head_rl;
  rtx head_rtx = (head->rli->type == RLI_OUTPUTRELOAD
		  ? head_rl->out_reg : head_rl->in_reg);
  int best_reg;
  int best_value;
  /* This keeps track of the last insn we modified.  Each time we find a
     new one that uses the inheritance register, we walk the chains in
     between to update their INHERITED_LIVE sets.  */
  struct insn_chain *previous_chain = head->chain->next;

  HARD_REG_SET usable_for_head, usable_regs, tmp;

  if (dump_file)
    {
      fprintf (dump_file, "*** inherit_one_chain: head insn: %d, rtx: ", INSN_UID (head->chain->insn));
      print_inline_rtx (dump_file, head_rtx, 0);
      fprintf (dump_file, "\n");
    }

  memset (goodness, 0, sizeof goodness);

  /* Find out which registers we can use as reload register for the first
     entry on the chain.  */
  compute_unallocated (prev->chain, prev_rl, &tmp);

  CLEAR_HARD_REG_SET (usable_for_head);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      unsigned int k, nregs;

      if (! HARD_REGNO_MODE_OK (i, head_rl->mode))
	continue;
      nregs = HARD_REGNO_NREGS (i, head_rl->mode);

      for (k = 0; k < nregs; k++)
	if (! TEST_HARD_REG_BIT (tmp, i + k))
	  break;
      if (k == nregs)
	SET_HARD_REG_BIT (usable_for_head, i);
    }

  last = evaluate_regs_for_inherit (head, &usable_for_head, goodness);

  best_reg = -1;
  best_value = INT_MAX;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      /* new_reg_preferred takes cost arguments, so negate the goodness.  */
      int value = - goodness[i];

      /* Re-test against USABLE_FOR_HEAD.  evaluate_regs_for_inherit does
	 sometimes think a partial inheritance is possible, even though
	 that would force the head into an unusable register.  */
      if (! TEST_HARD_REG_BIT (usable_for_head, i) || value == 0)
	continue;

      /* Adjust the value if we know we can save a copy.  We do it here
	 to avoid choosing a register whose value would otherwise be
	 zero.  */
      value -= allocation_goodness (head->chain, head_rl, i);

      if (new_reg_preferred (best_reg, i, best_value, value))
	{
	  best_value = value;
	  best_reg = i;
	}
    }

  if (best_reg == -1)
    {
      /* Now that's very disappointing.  */
      undo_enables (head, last);
      last = head->next_same;
      goto put_back_last;
    }

  if (dump_file)
    {
      int i;
      fprintf (dump_file, " best_reg is %d\n", best_reg);
      for (i = 0; i < FIRST_PSEUDO_REGISTER; ++i)
	if (TEST_HARD_REG_BIT (usable_for_head, i))
	  fprintf (dump_file, "%s ", reg_names[i]);
	  fprintf (dump_file, "\n");
    }

  head->next_chain = done_inherit_chains;
  done_inherit_chains = head;

  /* Now, actually perform all the necessary modifications.  This is a
     second pass over all inheritance opportunites, done in similar fashion
     as above.  */

  /* Change the head reload to use the inheritance register.  */
  set_reload_reg (head_rl, best_reg);

  /* Extend lifetime of original reload.  Set the LIVE_UNTIL_END flag so
     that following calls to compute_birth_death will preserve this
     state.  */
  head_rl->reginfo.death = head->chain->rli[head->chain->last_rlinsn].order + 1;
  head_rl->reginfo.live_until_end = 1;

  COPY_HARD_REG_SET (usable_regs, usable_for_head);

  /* Create a set of the hard registers we use to keep the inherited value.  */
  {
    unsigned int k, nregs = HARD_REGNO_NREGS (best_reg, head_rl->mode);

    CLEAR_HARD_REG_SET (tmp);
    for (k = 0; k < nregs; k++)
      SET_HARD_REG_BIT (tmp, best_reg + k);
  }

  last_performed = head;
  prev = head;
  this = head->next_same;
  noninherited_use = head->used_after;
  while (this != last)
    {
      struct insn_chain *chain;
      struct reload *rl = this->chain->rld + this->rli->nr;

      noninherited_use |= this->used_after;

      if (! noninherited_use)
	noninherited_use |= jump_insn_between_p (prev->chain, this->chain);

      AND_COMPL_HARD_REG_SET (usable_regs, this->new_conflicts);

      if (perform_inheritance (head_rtx, this, rl, best_reg, &usable_regs))
	{
	  /* Update register life information.
	     @@@ This is slightly conservative in rare cases involving
	     inheritance of partial registers.  Fixing it is way too
	     complicated for too little gain.  */
	  if (this->chain != head->chain)
	    while (previous_chain != this->chain)
	      {
		IOR_HARD_REG_SET (previous_chain->inherited_live, tmp);
		previous_chain = previous_chain->next;
	      }
	  if (dump_file)
	    fprintf (dump_file, "    Inherited insn: %d\n", INSN_UID (this->chain->insn));
	  compute_ignored (this->chain);
	  last_performed = this;
	}
      else
	{
	  noninherited_use = 1;
	  if (rl->optional && ! this->was_enabled)
	    disable_optional (this->chain, rl);
	  if (dump_file)
	    fprintf (dump_file, "    Ignored insn: %d\n", INSN_UID (this->chain->insn));
	}

      if (! noninherited_use && this->next_same == last)
	{
	  if ((this->rli->type == RLI_OUTPUTRELOAD
	       && GET_CODE (rl->out_reg) != REG)
	      || (this->rli->type == RLI_INPUTRELOAD
		  && ! find_regno_note (this->chain->insn, REG_DEAD,
					REGNO (head_rtx))
		  && ! (last && sets_full_reg_p (last)
			&& ! jump_insn_between_p (this->chain, last->chain))))
	    noninherited_use = 1;
	}

      prev = this;
      this = this->next_same;
    }
  /* If we have a trail of unperformed opportunities, put them all back for
     another attempt.  */
  if (last_performed->next_same != last)
    {
      last = last_performed->next_same;
    }

  /* If the head is an output, and we changed all the subsequent uses until
     the death or another store into the same register, we can disable the
     head reload.  */
  if (head->rli->type == RLI_OUTPUTRELOAD
      && GET_CODE (head_rtx) == REG
      && ! noninherited_use)
    {
      head->rli->order = head->chain->rli[0].order;
      head->rli->unnecessary = 1;
      head->rli->ignored = 1;
      compute_birth_death (head->chain);
    }

 put_back_last:

  if (last)
    {
      /* The caller has removed the whole chain from ALL_INHERIT_CHAINS.
	 If LAST is nonzero at this point, it means that we stopped half-way
	 through.  The rest of the chain can still be used for inheritance,
	 so put it back.  */
      last->next_chain = all_inherit_chains;
      all_inherit_chains = last;
    }
}

/* Some inherit_chains are not suitable for starting inheritance.  These
   are the ones that refer to the input part of an in-out reload: the
   reload register will be clobbered and thus cannot start inheritance.  */
static int
usable_as_inheritance_head (struct inherit_chain *p)
{
  struct reload *p_rl = p->chain->rld + p->rli->nr;

  if (p->rli->type == RLI_INPUTRELOAD && p_rl->out_reg)
    return 0;
  return 1;
}

static char *inherit_startobj;

/* Called by inherit_reloads when a block boundary (CODE_LABEL, etc.) is
   seen.  Process the collected data about inheritance opportunities.

   Upon return, all state will be reset and the caller can proceed to
   collect data for the remaining insns.  */
static void
inherit_block (void)
{

  /* @@@ Todo:  we might put some code here to select the most promising of
     the chains.  */
  while (all_inherit_chains)
    {
      struct inherit_chain *p = all_inherit_chains;
      all_inherit_chains = p->next_chain;

      while (p && ! usable_as_inheritance_head (p))
	p = p->next_same;

      if (p && p->next_same)
	inherit_one_chain (p);
    }

  memset (pseudo_inherit, 0, (sizeof (struct inherit_chain *)
			      * max_reg_num ()));
  all_inherit_chains = 0;
  done_inherit_chains = 0;
  obstack_free (&reload_obstack, inherit_startobj);
}

/* Try to improve the allocation by inheriting reloads.

   This function is the entry point for the inheritance pass.  It walks all
   insns, recording inheritance opportunties, and hands them off at the end
   of each extended basic block to the function inherit_block.

   Inheritance will modify the data we have gathered about reloads.  It can
   change allocated registers.  It will set the override_in fields for input
   reloads that want to inherit from a register.  Life information will be
   updated.  */
static void
inherit_reloads (void)
{
  struct insn_chain *chain;
  int prev_was_setjmp, this_is_setjmp = 0;
  done_inherit_chains = 0;
  all_inherit_chains = 0;
  pseudo_inherit
    = (struct inherit_chain **) xcalloc (sizeof (struct inherit_chain *),
					 max_reg_num ());

  inherit_startobj = (char *) obstack_alloc (&reload_obstack, 0);

  for (chain = reload_insn_chain; chain; chain = chain->next)
    {
      struct inherit_chain *this_inherit = 0;
      int i;
      unsigned r;
      rtx insn = chain->insn;
      reg_set_iterator rsi;

      prev_was_setjmp = this_is_setjmp;
      this_is_setjmp = (CALL_P (insn)
			 && find_reg_note (insn, REG_SETJMP, NULL));

	  
      if (GET_CODE (insn) == NOTE || chain->will_be_deleted)
	continue;

      if (GET_CODE (insn) == CODE_LABEL || GET_CODE (insn) == BARRIER
	  || prev_was_setjmp
	  || (GET_CODE (insn) == INSN
	      && GET_CODE (PATTERN (insn)) == ASM_OPERANDS
	      && MEM_VOLATILE_P (PATTERN (insn))))
	{
	  inherit_block ();
	  continue;
	}

      EXECUTE_IF_SET_IN_REG_SET (&chain->unreloaded_sets, FIRST_PSEUDO_REGISTER,
				 r, rsi)
	{
	  pseudo_inherit[r] = 0;
	}
      EXECUTE_IF_SET_IN_REG_SET
	(&chain->unreloaded_uses, FIRST_PSEUDO_REGISTER, r, rsi)
	{
	  if (pseudo_inherit[r])
	    pseudo_inherit[r]->used_after = 1;
	}

      if (chain->n_reloads == 0)
	continue;

      /* See which reloads might be inherited.  This loop walks the
	 reload_insns of CHAIN in descending order and produces a list of
	 inheritance opportunties that is sorted in ascending order.  */
      for (i = chain->last_rlinsn; i >= 0;)
	{
	  struct reload_insn *rli = chain->rli + i;

	  if (rli->type == RLI_OUTPUTRELOAD || rli->type == RLI_INPUTRELOAD)
	    {
	      struct reload *rl = chain->rld + rli->nr;
	      rtx which = (rli->type == RLI_OUTPUTRELOAD
			   ? rl->out_reg : rl->in_reg);
	      rtx reg = which;
	      if (GET_CODE (reg) == SUBREG)
		reg = SUBREG_REG (reg);

	      if (GET_CODE (reg) == REG
		  && REGNO (reg) >= FIRST_PSEUDO_REGISTER
		  && ! REGNO_REG_SET_P (&chain->unreloaded_sets, REGNO (reg)))
		{
		  struct inherit_chain *new_inherit;
		  new_inherit
		    = (struct inherit_chain *) obstack_alloc (&reload_obstack,
							      sizeof (struct inherit_chain));
		  new_inherit->next_chain = this_inherit;
		  new_inherit->next_same = 0;
		  new_inherit->used_after = 0;
		  new_inherit->chain = chain;
		  new_inherit->rli = chain->rli + i;
		  this_inherit = new_inherit;
		}
	    }
	  i = rli->prev_order;
	}

      /* Now, walk again over all the new opportunities we found and add them
	 to the global chains.  */
      while (this_inherit)
	{
	  struct inherit_chain *tmp = this_inherit;
	  struct inherit_chain *next = tmp->next_chain;
	  struct reload *rl = chain->rld + this_inherit->rli->nr;
	  int regno;
	  rtx which = (this_inherit->rli->type == RLI_OUTPUTRELOAD
		       ? rl->out_reg : rl->in_reg);
	  rtx reg = which;

	  if (GET_CODE (reg) == SUBREG)
	    reg = SUBREG_REG (reg);
	  regno = REGNO (reg);

	  if (pseudo_inherit[regno] == 0)
	    {
	      tmp->next_chain = all_inherit_chains;
	      all_inherit_chains = tmp;
	    }
	  else
	    pseudo_inherit[regno]->next_same = tmp;
	  pseudo_inherit[regno] = tmp;
	  this_inherit = next;
	}
    }
  inherit_block ();

  free (pseudo_inherit);
}

/* This is called after calculate_needs_all_insns.  We have full information
   about the required reloads at this point, so try to find reload registers
   to satisfy the needs.  */
static void
select_reload_regs (void)
{
  struct insn_chain *chain;

  CLEAR_HARD_REG_SET (used_spill_regs);

  /* Try to satisfy the needs for each insn.  */
  for (chain = insns_need_reload; chain != 0;
       chain = chain->next_need_reload)
    find_reload_regs (chain);

  if (optimize)
    inherit_reloads ();
}

/* Delete all insns that were inserted by emit_caller_save_insns during
   this iteration.  */
static void
delete_caller_save_insns (void)
{
  struct insn_chain *c = reload_insn_chain;

  while (c != 0)
    {
      while (c != 0 && c->is_caller_save_insn)
	{
	  struct insn_chain *next = c->next;
	  rtx insn = c->insn;

	  if (c == reload_insn_chain)
	    reload_insn_chain = next;
	  delete_insn (insn);

	  if (next)
	    next->prev = c->prev;
	  if (c->prev)
	    c->prev->next = next;
	  c->next = unused_insn_chains;
	  unused_insn_chains = c;
	  c = next;
	}
      if (c != 0)
	c = c->next;
    }
}

/* Handle the failure to find a register to spill.
   INSN should be one of the insns which needed this particular spill reg.  */

static void
spill_failure (rtx insn, enum reg_class class)
{
  if (asm_noperands (PATTERN (insn)) >= 0)
    error_for_asm (insn, "can't find a register in class %qs while "
		   "reloading %<asm%>",
		   reg_class_names[class]);
  else
    {
      error ("unable to find a register to spill in class %qs",
	     reg_class_names[class]);
      fatal_insn ("this is the insn:", insn);
    }
}

/* Delete an unneeded INSN and any previous insns who sole purpose is loading
   data that is dead in INSN.  */

static void
delete_dead_insn (rtx insn)
{
  rtx prev = prev_real_insn (insn);
  rtx prev_dest;

  /* If the previous insn sets a register that dies in our insn, delete it
     too.  */
  if (prev && GET_CODE (PATTERN (prev)) == SET
      && (prev_dest = SET_DEST (PATTERN (prev)), REG_P (prev_dest))
      && reg_mentioned_p (prev_dest, PATTERN (insn))
      && find_regno_note (insn, REG_DEAD, REGNO (prev_dest))
      && ! side_effects_p (SET_SRC (PATTERN (prev))))
    delete_dead_insn (prev);

  SET_INSN_DELETED (insn);
}

/* Modify the home of pseudo-reg I.
   The new home is present in reg_renumber[I].

   FROM_REG may be the hard reg that the pseudo-reg is being spilled from;
   or it may be -1, meaning there is none or it is not relevant.
   This is used so that all pseudos spilled from a given hard reg
   can share one stack slot.  */

static void
alter_reg (int i, int from_reg)
{
  /* When outputting an inline function, this can happen
     for a reg that isn't actually used.  */
  if (regno_reg_rtx[i] == 0)
    return;

  /* If the reg got changed to a MEM at rtl-generation time,
     ignore it.  */
  if (!REG_P (regno_reg_rtx[i]))
    return;

  /* Modify the reg-rtx to contain the new hard reg
     number or else to contain its pseudo reg number.  */
  REGNO (regno_reg_rtx[i])
    = reg_renumber[i] >= 0 ? reg_renumber[i] : i;

  /* If we have a pseudo that is needed but has no hard reg or equivalent,
     allocate a stack slot for it.  */

  if (reg_renumber[i] < 0
      && REG_N_REFS (i) > 0
      && reg_equiv_constant[i] == 0
      && reg_equiv_memory_loc[i] == 0)
    {
      rtx x;
      unsigned int inherent_size = PSEUDO_REGNO_BYTES (i);
      unsigned int total_size = MAX (inherent_size, reg_max_ref_width[i]);
      int adjust = 0;

      /* Each pseudo reg has an inherent size which comes from its own mode,
	 and a total size which provides room for paradoxical subregs
	 which refer to the pseudo reg in wider modes.

	 We can use a slot already allocated if it provides both
	 enough inherent space and enough total space.
	 Otherwise, we allocate a new slot, making sure that it has no less
	 inherent space, and no less total space, then the previous slot.  */
      if (from_reg == -1)
	{
	  /* No known place to spill from => no slot to reuse.  */
	  x = assign_stack_local (GET_MODE (regno_reg_rtx[i]), total_size,
				  inherent_size == total_size ? 0 : -1);
	  if (BYTES_BIG_ENDIAN)
	    /* Cancel the  big-endian correction done in assign_stack_local.
	       Get the address of the beginning of the slot.
	       This is so we can do a big-endian correction unconditionally
	       below.  */
	    adjust = inherent_size - total_size;

	  /* Nothing can alias this slot except this pseudo.  */
	  set_mem_alias_set (x, new_alias_set ());
	}

      /* Reuse a stack slot if possible.  */
      else if (spill_stack_slot[from_reg] != 0
	       && spill_stack_slot_width[from_reg] >= total_size
	       && (GET_MODE_SIZE (GET_MODE (spill_stack_slot[from_reg]))
		   >= inherent_size))
	x = spill_stack_slot[from_reg];

      /* Allocate a bigger slot.  */
      else
	{
	  /* Compute maximum size needed, both for inherent size
	     and for total size.  */
	  enum machine_mode mode = GET_MODE (regno_reg_rtx[i]);
	  rtx stack_slot;

	  if (spill_stack_slot[from_reg])
	    {
	      if (GET_MODE_SIZE (GET_MODE (spill_stack_slot[from_reg]))
		  > inherent_size)
		mode = GET_MODE (spill_stack_slot[from_reg]);
	      if (spill_stack_slot_width[from_reg] > total_size)
		total_size = spill_stack_slot_width[from_reg];
	    }

	  /* Make a slot with that size.  */
	  x = assign_stack_local (mode, total_size,
				  inherent_size == total_size ? 0 : -1);
	  stack_slot = x;

	  /* All pseudos mapped to this slot can alias each other.  */
	  if (spill_stack_slot[from_reg])
	    set_mem_alias_set (x, MEM_ALIAS_SET (spill_stack_slot[from_reg]));
	  else
	    set_mem_alias_set (x, new_alias_set ());

	  if (BYTES_BIG_ENDIAN)
	    {
	      /* Cancel the  big-endian correction done in assign_stack_local.
		 Get the address of the beginning of the slot.
		 This is so we can do a big-endian correction unconditionally
		 below.  */
	      adjust = GET_MODE_SIZE (mode) - total_size;
	      if (adjust)
		stack_slot
		  = adjust_address_nv (x, mode_for_size (total_size
							 * BITS_PER_UNIT,
							 MODE_INT, 1),
				       adjust);
	    }

	  spill_stack_slot[from_reg] = stack_slot;
	  spill_stack_slot_width[from_reg] = total_size;
	}

      /* On a big endian machine, the "address" of the slot
	 is the address of the low part that fits its inherent mode.  */
      if (BYTES_BIG_ENDIAN && inherent_size < total_size)
	adjust += (total_size - inherent_size);

      /* If we have any adjustment to make, or if the stack slot is the
	 wrong mode, make a new stack slot.  */
      x = adjust_address_nv (x, GET_MODE (regno_reg_rtx[i]), adjust);

      /* If we have a decl for the original register, set it for the
	 memory.  If this is a shared MEM, make a copy.  */
      if (REG_EXPR (regno_reg_rtx[i])
	  && DECL_P (REG_EXPR (regno_reg_rtx[i])))
	{
	  rtx decl = DECL_RTL_IF_SET (REG_EXPR (regno_reg_rtx[i]));

	  /* We can do this only for the DECLs home pseudo, not for
	     any copies of it, since otherwise when the stack slot
	     is reused, nonoverlapping_memrefs_p might think they
	     cannot overlap.  */
	  if (decl && REG_P (decl) && REGNO (decl) == (unsigned) i)
	    {
	      if (from_reg != -1 && spill_stack_slot[from_reg] == x)
		x = copy_rtx (x);

	      set_mem_attrs_from_reg (x, regno_reg_rtx[i]);
	    }
	}

      /* Save the stack slot for later.  */
      reg_equiv_memory_loc[i] = x;
    }
}

/* Mark the slots in regs_ever_live for the hard regs
   used by pseudo-reg number REGNO.  */

void
mark_home_live (int regno)
{
  int i, lim;

  i = reg_renumber[regno];
  if (i < 0)
    return;
  lim = i + hard_regno_nregs[i][PSEUDO_REGNO_MODE (regno)];
  while (i < lim)
    regs_ever_live[i++] = 1;
}

/* This function handles the tracking of elimination offsets around branches.

   X is a piece of RTL being scanned.

   INSN is the insn that it came from, if any.

   INITIAL_P is nonzero if we are to set the offset to be the initial
   offset and zero if we are setting the offset of the label to be the
   current offset.  */

static void
set_label_offsets (rtx x, rtx insn, int initial_p)
{
  enum rtx_code code = GET_CODE (x);
  rtx tem;
  unsigned int i;
  struct elim_table *p;

  switch (code)
    {
    case LABEL_REF:
      if (LABEL_REF_NONLOCAL_P (x))
	return;

      x = XEXP (x, 0);

      /* ... fall through ...  */

    case CODE_LABEL:
      /* If we know nothing about this label, set the desired offsets.  Note
	 that this sets the offset at a label to be the offset before a label
	 if we don't know anything about the label.  This is not correct for
	 the label after a BARRIER, but is the best guess we can make.  If
	 we guessed wrong, we will suppress an elimination that might have
	 been possible had we been able to guess correctly.  */

      if (! offsets_known_at[CODE_LABEL_NUMBER (x) - first_label_num])
	{
	  for (i = 0; i < NUM_ELIMINABLE_REGS; i++)
	    offsets_at[CODE_LABEL_NUMBER (x) - first_label_num][i]
	      = (initial_p ? reg_eliminate[i].initial_offset
		 : reg_eliminate[i].offset);
	  offsets_known_at[CODE_LABEL_NUMBER (x) - first_label_num] = 1;
	}

      /* Otherwise, if this is the definition of a label and it is
	 preceded by a BARRIER, set our offsets to the known offset of
	 that label.  */

      else if (x == insn
	       && (tem = prev_nonnote_insn (insn)) != 0
	       && BARRIER_P (tem))
	set_offsets_for_label (insn);
      else
	/* If neither of the above cases is true, compare each offset
	   with those previously recorded and suppress any eliminations
	   where the offsets disagree.  */

	for (i = 0; i < NUM_ELIMINABLE_REGS; i++)
	  if (offsets_at[CODE_LABEL_NUMBER (x) - first_label_num][i]
	      != (initial_p ? reg_eliminate[i].initial_offset
		  : reg_eliminate[i].offset))
	    reg_eliminate[i].can_eliminate = 0;

      return;

    case JUMP_INSN:
      set_label_offsets (PATTERN (insn), insn, initial_p);

      /* ... fall through ...  */

    case INSN:
    case CALL_INSN:
      /* Any labels mentioned in REG_LABEL notes can be branched to indirectly
	 and hence must have all eliminations at their initial offsets.  */
      for (tem = REG_NOTES (x); tem; tem = XEXP (tem, 1))
	if (REG_NOTE_KIND (tem) == REG_LABEL)
	  set_label_offsets (XEXP (tem, 0), insn, 1);
      return;

    case PARALLEL:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
      /* Each of the labels in the parallel or address vector must be
	 at their initial offsets.  We want the first field for PARALLEL
	 and ADDR_VEC and the second field for ADDR_DIFF_VEC.  */

      for (i = 0; i < (unsigned) XVECLEN (x, code == ADDR_DIFF_VEC); i++)
	set_label_offsets (XVECEXP (x, code == ADDR_DIFF_VEC, i),
			   insn, initial_p);
      return;

    case SET:
      /* We only care about setting PC.  If the source is not RETURN,
	 IF_THEN_ELSE, or a label, disable any eliminations not at
	 their initial offsets.  Similarly if any arm of the IF_THEN_ELSE
	 isn't one of those possibilities.  For branches to a label,
	 call ourselves recursively.

	 Note that this can disable elimination unnecessarily when we have
	 a non-local goto since it will look like a non-constant jump to
	 someplace in the current function.  This isn't a significant
	 problem since such jumps will normally be when all elimination
	 pairs are back to their initial offsets.  */

      if (SET_DEST (x) != pc_rtx)
	return;

      switch (GET_CODE (SET_SRC (x)))
	{
	case PC:
	case RETURN:
	  return;

	case LABEL_REF:
	  set_label_offsets (SET_SRC (x), insn, initial_p);
	  return;

	case IF_THEN_ELSE:
	  tem = XEXP (SET_SRC (x), 1);
	  if (GET_CODE (tem) == LABEL_REF)
	    set_label_offsets (XEXP (tem, 0), insn, initial_p);
	  else if (GET_CODE (tem) != PC && GET_CODE (tem) != RETURN)
	    break;

	  tem = XEXP (SET_SRC (x), 2);
	  if (GET_CODE (tem) == LABEL_REF)
	    set_label_offsets (XEXP (tem, 0), insn, initial_p);
	  else if (GET_CODE (tem) != PC && GET_CODE (tem) != RETURN)
	    break;
	  return;

	default:
	  break;
	}

      /* If we reach here, all eliminations must be at their initial
	 offset because we are doing a jump to a variable address.  */
      for (p = reg_eliminate; p < &reg_eliminate[NUM_ELIMINABLE_REGS]; p++)
	if (p->offset != p->initial_offset)
	  p->can_eliminate = 0;
      break;

    default:
      break;
    }
}

/* Scan X and replace any eliminable registers (such as fp) with a
   replacement (such as sp), plus an offset.

   MEM_MODE is the mode of an enclosing MEM.  We need this to know how
   much to adjust a register for, e.g., PRE_DEC.  Also, if we are inside a
   MEM, we are allowed to replace a sum of a register and the constant zero
   with the register, which we cannot do outside a MEM.  In addition, we need
   to record the fact that a register is referenced outside a MEM.

   If INSN is an insn, it is the insn containing X.  If we replace a REG
   in a SET_DEST with an equivalent MEM and INSN is nonzero, write a
   CLOBBER of the pseudo after INSN so find_equiv_regs will know that
   the REG is being modified.

   Alternatively, INSN may be a note (an EXPR_LIST or INSN_LIST).
   That's used when we eliminate in expressions stored in notes.
   This means, do not set ref_outside_mem even if the reference
   is outside of MEMs.

   REG_EQUIV_MEM and REG_EQUIV_ADDRESS contain address that have had
   replacements done assuming all offsets are at their initial values.  If
   they are not, or if REG_EQUIV_ADDRESS is nonzero for a pseudo we
   encounter, return the actual location so that find_reloads will do
   the proper thing.  */

rtx
eliminate_regs (rtx x, enum machine_mode mem_mode, rtx insn)
{
  enum rtx_code code = GET_CODE (x);
  struct elim_table *ep;
  int regno;
  rtx new;
  int i, j;
  const char *fmt;
  int copied = 0;

  if (! current_function_decl)
    return x;

  switch (code)
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
    case CONST:
    case SYMBOL_REF:
    case CODE_LABEL:
    case PC:
    case CC0:
    case ASM_INPUT:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
    case RETURN:
      return x;

    case REG:
      regno = REGNO (x);

      /* First handle the case where we encounter a bare register that
	 is eliminable.  Replace it with a PLUS.  */
      if (regno < FIRST_PSEUDO_REGISTER)
	{
	  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS];
	       ep++)
	    if (ep->from_rtx == x && ep->can_eliminate)
	      return plus_constant (ep->to_rtx, ep->previous_offset);

	}
      else if (reg_renumber && reg_renumber[regno] < 0
	       && reg_equiv_constant && reg_equiv_constant[regno]
	       && ! CONSTANT_P (reg_equiv_constant[regno]))
	return eliminate_regs (copy_rtx (reg_equiv_constant[regno]),
			       mem_mode, insn);
      return x;

    /* You might think handling MINUS in a manner similar to PLUS is a
       good idea.  It is not.  It has been tried multiple times and every
       time the change has had to have been reverted.

       Other parts of reload know a PLUS is special (gen_reload for example)
       and require special code to handle code a reloaded PLUS operand.

       Also consider backends where the flags register is clobbered by a
       MINUS, but we can emit a PLUS that does not clobber flags (IA-32,
       lea instruction comes to mind).  If we try to reload a MINUS, we
       may kill the flags register that was holding a useful value.

       So, please before trying to handle MINUS, consider reload as a
       whole instead of this little section as well as the backend issues.  */
    case PLUS:
      /* If this is the sum of an eliminable register and a constant, rework
	 the sum.  */
      if (REG_P (XEXP (x, 0))
	  && REGNO (XEXP (x, 0)) < FIRST_PSEUDO_REGISTER
	  && CONSTANT_P (XEXP (x, 1)))
	{
	  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS];
	       ep++)
	    if (ep->from_rtx == XEXP (x, 0) && ep->can_eliminate)
	      {
		/* The only time we want to replace a PLUS with a REG (this
		   occurs when the constant operand of the PLUS is the negative
		   of the offset) is when we are inside a MEM.  We won't want
		   to do so at other times because that would change the
		   structure of the insn in a way that reload can't handle.
		   We special-case the commonest situation in
		   eliminate_regs_in_insn, so just replace a PLUS with a
		   PLUS here, unless inside a MEM.  */
		if (mem_mode != 0 && GET_CODE (XEXP (x, 1)) == CONST_INT
		    && INTVAL (XEXP (x, 1)) == - ep->previous_offset)
		  return ep->to_rtx;
		else
		  return gen_rtx_PLUS (Pmode, ep->to_rtx,
				       plus_constant (XEXP (x, 1),
						      ep->previous_offset));
	      }

	  /* If the register is not eliminable, we are done since the other
	     operand is a constant.  */
	  return x;
	}

      /* If this is part of an address, we want to bring any constant to the
	 outermost PLUS.  We will do this by doing register replacement in
	 our operands and seeing if a constant shows up in one of them.

	 Note that there is no risk of modifying the structure of the insn,
	 since we only get called for its operands, thus we are either
	 modifying the address inside a MEM, or something like an address
	 operand of a load-address insn.  */

      {
	rtx new0 = eliminate_regs (XEXP (x, 0), mem_mode, insn);
	rtx new1 = eliminate_regs (XEXP (x, 1), mem_mode, insn);

	if (reg_renumber && (new0 != XEXP (x, 0) || new1 != XEXP (x, 1)))
	  {
	    /* If one side is a PLUS and the other side is a pseudo that
	       didn't get a hard register but has a reg_equiv_constant,
	       we must replace the constant here since it may no longer
	       be in the position of any operand.  */
	    if (GET_CODE (new0) == PLUS && REG_P (new1)
		&& REGNO (new1) >= FIRST_PSEUDO_REGISTER
		&& reg_renumber[REGNO (new1)] < 0
		&& reg_equiv_constant != 0
		&& reg_equiv_constant[REGNO (new1)] != 0)
	      new1 = reg_equiv_constant[REGNO (new1)];
	    else if (GET_CODE (new1) == PLUS && REG_P (new0)
		     && REGNO (new0) >= FIRST_PSEUDO_REGISTER
		     && reg_renumber[REGNO (new0)] < 0
		     && reg_equiv_constant[REGNO (new0)] != 0)
	      new0 = reg_equiv_constant[REGNO (new0)];

	    new = form_sum (new0, new1);

	    /* As above, if we are not inside a MEM we do not want to
	       turn a PLUS into something else.  We might try to do so here
	       for an addition of 0 if we aren't optimizing.  */
	    if (! mem_mode && GET_CODE (new) != PLUS)
	      return gen_rtx_PLUS (GET_MODE (x), new, const0_rtx);
	    else
	      return new;
	  }
      }
      return x;

    case MULT:
      /* If this is the product of an eliminable register and a
	 constant, apply the distribute law and move the constant out
	 so that we have (plus (mult ..) ..).  This is needed in order
	 to keep load-address insns valid.   This case is pathological.
	 We ignore the possibility of overflow here.  */
      if (REG_P (XEXP (x, 0))
	  && REGNO (XEXP (x, 0)) < FIRST_PSEUDO_REGISTER
	  && GET_CODE (XEXP (x, 1)) == CONST_INT)
	for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS];
	     ep++)
	  if (ep->from_rtx == XEXP (x, 0) && ep->can_eliminate)
	    {
	      if (! mem_mode
		  /* Refs inside notes don't count for this purpose.  */
		  && ! (insn != 0 && (GET_CODE (insn) == EXPR_LIST
				      || GET_CODE (insn) == INSN_LIST)))
		ep->ref_outside_mem = 1;

	      return
		plus_constant (gen_rtx_MULT (Pmode, ep->to_rtx, XEXP (x, 1)),
			       ep->previous_offset * INTVAL (XEXP (x, 1)));
	    }

      /* ... fall through ...  */

    case CALL:
    case COMPARE:
    /* See comments before PLUS about handling MINUS.  */
    case MINUS:
    case DIV:      case UDIV:
    case MOD:      case UMOD:
    case AND:      case IOR:      case XOR:
    case ROTATERT: case ROTATE:
    case ASHIFTRT: case LSHIFTRT: case ASHIFT:
    case NE:       case EQ:
    case GE:       case GT:       case GEU:    case GTU:
    case LE:       case LT:       case LEU:    case LTU:
      {
	rtx new0 = eliminate_regs (XEXP (x, 0), mem_mode, insn);
	rtx new1
	  = XEXP (x, 1) ? eliminate_regs (XEXP (x, 1), mem_mode, insn) : 0;

	if (new0 != XEXP (x, 0) || new1 != XEXP (x, 1))
	  return gen_rtx_fmt_ee (code, GET_MODE (x), new0, new1);
      }
      return x;

    case EXPR_LIST:
      /* If we have something in XEXP (x, 0), the usual case, eliminate it.  */
      if (XEXP (x, 0))
	{
	  new = eliminate_regs (XEXP (x, 0), mem_mode, insn);
	  if (new != XEXP (x, 0))
	    {
	      /* If this is a REG_DEAD note, it is not valid anymore.
		 Using the eliminated version could result in creating a
		 REG_DEAD note for the stack or frame pointer.  */
	      if (GET_MODE (x) == REG_DEAD)
		return (XEXP (x, 1)
			? eliminate_regs (XEXP (x, 1), mem_mode, insn)
			: NULL_RTX);

	      x = gen_rtx_EXPR_LIST (REG_NOTE_KIND (x), new, XEXP (x, 1));
	    }
	}

      /* ... fall through ...  */

    case INSN_LIST:
      /* Now do eliminations in the rest of the chain.  If this was
	 an EXPR_LIST, this might result in allocating more memory than is
	 strictly needed, but it simplifies the code.  */
      if (XEXP (x, 1))
	{
	  new = eliminate_regs (XEXP (x, 1), mem_mode, insn);
	  if (new != XEXP (x, 1))
	    return
	      gen_rtx_fmt_ee (GET_CODE (x), GET_MODE (x), XEXP (x, 0), new);
	}
      return x;

    case PRE_INC:
    case POST_INC:
    case PRE_DEC:
    case POST_DEC:
    case STRICT_LOW_PART:
    case NEG:          case NOT:
    case SIGN_EXTEND:  case ZERO_EXTEND:
    case TRUNCATE:     case FLOAT_EXTEND: case FLOAT_TRUNCATE:
    case FLOAT:        case FIX:
    case UNSIGNED_FIX: case UNSIGNED_FLOAT:
    case ABS:
    case SQRT:
    case FFS:
    case CLZ:
    case CTZ:
    case POPCOUNT:
    case PARITY:
      new = eliminate_regs (XEXP (x, 0), mem_mode, insn);
      if (new != XEXP (x, 0))
	return gen_rtx_fmt_e (code, GET_MODE (x), new);
      return x;

    case SUBREG:
      /* Similar to above processing, but preserve SUBREG_BYTE.
	 Convert (subreg (mem)) to (mem) if not paradoxical.
	 Also, if we have a non-paradoxical (subreg (pseudo)) and the
	 pseudo didn't get a hard reg, we must replace this with the
	 eliminated version of the memory location because push_reload
	 may do the replacement in certain circumstances.  */
      if (REG_P (SUBREG_REG (x))
	  && (GET_MODE_SIZE (GET_MODE (x))
	      <= GET_MODE_SIZE (GET_MODE (SUBREG_REG (x))))
	  && reg_equiv_memory_loc != 0
	  && reg_equiv_memory_loc[REGNO (SUBREG_REG (x))] != 0)
	{
	  new = SUBREG_REG (x);
	}
      else
	new = eliminate_regs (SUBREG_REG (x), mem_mode, insn);

      if (new != SUBREG_REG (x))
	{
	  int x_size = GET_MODE_SIZE (GET_MODE (x));
	  int new_size = GET_MODE_SIZE (GET_MODE (new));

	  if (MEM_P (new)
	      && ((x_size < new_size
#ifdef WORD_REGISTER_OPERATIONS
		   /* On these machines, combine can create rtl of the form
		      (set (subreg:m1 (reg:m2 R) 0) ...)
		      where m1 < m2, and expects something interesting to
		      happen to the entire word.  Moreover, it will use the
		      (reg:m2 R) later, expecting all bits to be preserved.
		      So if the number of words is the same, preserve the
		      subreg so that push_reload can see it.  */
		   && ! ((x_size - 1) / UNITS_PER_WORD
			 == (new_size -1 ) / UNITS_PER_WORD)
#endif
		   )
		  || x_size == new_size)
	      )
	    return adjust_address_nv (new, GET_MODE (x), SUBREG_BYTE (x));
	  else
	    return gen_rtx_SUBREG (GET_MODE (x), new, SUBREG_BYTE (x));
	}

      return x;

    case MEM:
      /* Our only special processing is to pass the mode of the MEM to our
	 recursive call and copy the flags.  While we are here, handle this
	 case more efficiently.  */
      return
	replace_equiv_address_nv (x,
				  eliminate_regs (XEXP (x, 0),
						  GET_MODE (x), insn));

    case USE:
      /* Handle insn_list USE that a call to a pure function may generate.  */
      new = eliminate_regs (XEXP (x, 0), 0, insn);
      if (new != XEXP (x, 0))
	return gen_rtx_USE (GET_MODE (x), new);
      return x;

    case CLOBBER:
    case ASM_OPERANDS:
    case SET:
      gcc_unreachable ();

    default:
      break;
    }

  /* Process each of our operands recursively.  If any have changed, make a
     copy of the rtx.  */
  fmt = GET_RTX_FORMAT (code);
  for (i = 0; i < GET_RTX_LENGTH (code); i++, fmt++)
    {
      if (*fmt == 'e')
	{
	  new = eliminate_regs (XEXP (x, i), mem_mode, insn);
	  if (new != XEXP (x, i) && ! copied)
	    {
	      rtx new_x = rtx_alloc (code);
	      memcpy (new_x, x, RTX_SIZE (code));
	      x = new_x;
	      copied = 1;
	    }
	  XEXP (x, i) = new;
	}
      else if (*fmt == 'E')
	{
	  int copied_vec = 0;
	  for (j = 0; j < XVECLEN (x, i); j++)
	    {
	      new = eliminate_regs (XVECEXP (x, i, j), mem_mode, insn);
	      if (new != XVECEXP (x, i, j) && ! copied_vec)
		{
		  rtvec new_v = gen_rtvec_v (XVECLEN (x, i),
					     XVEC (x, i)->elem);
		  if (! copied)
		    {
		      rtx new_x = rtx_alloc (code);
		      memcpy (new_x, x, RTX_SIZE (code));
		      x = new_x;
		      copied = 1;
		    }
		  XVEC (x, i) = new_v;
		  copied_vec = 1;
		}
	      XVECEXP (x, i, j) = new;
	    }
	}
    }

  return x;
}

/* Scan rtx X for modifications of elimination target registers.  Update
   the table of eliminables to reflect the changed state.  MEM_MODE is
   the mode of an enclosing MEM rtx, or VOIDmode if not within a MEM.  */

static void
elimination_effects (rtx x, enum machine_mode mem_mode)
{
  enum rtx_code code = GET_CODE (x);
  struct elim_table *ep;
  int regno;
  int i, j;
  const char *fmt;

  switch (code)
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
    case CONST:
    case SYMBOL_REF:
    case CODE_LABEL:
    case PC:
    case CC0:
    case ASM_INPUT:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
    case RETURN:
      return;

    case REG:
      regno = REGNO (x);

      /* First handle the case where we encounter a bare register that
	 is eliminable.  Replace it with a PLUS.  */
      if (regno < FIRST_PSEUDO_REGISTER)
	{
	  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS];
	       ep++)
	    if (ep->from_rtx == x && ep->can_eliminate)
	      {
		if (! mem_mode)
		  ep->ref_outside_mem = 1;
		return;
	      }

	}
      else if (reg_renumber[regno] < 0 && reg_equiv_constant
	       && reg_equiv_constant[regno]
	       && ! function_invariant_p (reg_equiv_constant[regno]))
	elimination_effects (reg_equiv_constant[regno], mem_mode);
      return;

    case PRE_INC:
    case POST_INC:
    case PRE_DEC:
    case POST_DEC:
    case POST_MODIFY:
    case PRE_MODIFY:
      for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
	if (ep->to_rtx == XEXP (x, 0))
	  {
	    int size = GET_MODE_SIZE (mem_mode);

	    /* If more bytes than MEM_MODE are pushed, account for them.  */
#ifdef PUSH_ROUNDING
	    if (ep->to_rtx == stack_pointer_rtx)
	      size = PUSH_ROUNDING (size);
#endif
	    if (code == PRE_DEC || code == POST_DEC)
	      ep->offset += size;
	    else if (code == PRE_INC || code == POST_INC)
	      ep->offset -= size;
	    else if ((code == PRE_MODIFY || code == POST_MODIFY)
		     && GET_CODE (XEXP (x, 1)) == PLUS
		     && XEXP (x, 0) == XEXP (XEXP (x, 1), 0)
		     && CONSTANT_P (XEXP (XEXP (x, 1), 1)))
	      ep->offset -= INTVAL (XEXP (XEXP (x, 1), 1));
	  }

      /* These two aren't unary operators.  */
      if (code == POST_MODIFY || code == PRE_MODIFY)
	break;

      /* Fall through to generic unary operation case.  */
    case STRICT_LOW_PART:
    case NEG:          case NOT:
    case SIGN_EXTEND:  case ZERO_EXTEND:
    case TRUNCATE:     case FLOAT_EXTEND: case FLOAT_TRUNCATE:
    case FLOAT:        case FIX:
    case UNSIGNED_FIX: case UNSIGNED_FLOAT:
    case ABS:
    case SQRT:
    case FFS:
    case CLZ:
    case CTZ:
    case POPCOUNT:
    case PARITY:
      elimination_effects (XEXP (x, 0), mem_mode);
      return;

    case SUBREG:
      if (REG_P (SUBREG_REG (x))
	  && (GET_MODE_SIZE (GET_MODE (x))
	      <= GET_MODE_SIZE (GET_MODE (SUBREG_REG (x))))
	  && reg_equiv_memory_loc != 0
	  && reg_equiv_memory_loc[REGNO (SUBREG_REG (x))] != 0)
	return;

      elimination_effects (SUBREG_REG (x), mem_mode);
      return;

    case USE:
      /* If using a register that is the source of an eliminate we still
	 think can be performed, note it cannot be performed since we don't
	 know how this register is used.  */
      for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
	if (ep->from_rtx == XEXP (x, 0))
	  ep->can_eliminate = 0;

      elimination_effects (XEXP (x, 0), mem_mode);
      return;

    case CLOBBER:
      /* If clobbering a register that is the replacement register for an
	 elimination we still think can be performed, note that it cannot
	 be performed.  Otherwise, we need not be concerned about it.  */
      for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
	if (ep->to_rtx == XEXP (x, 0))
	  ep->can_eliminate = 0;

      elimination_effects (XEXP (x, 0), mem_mode);
      return;

    case SET:
      /* Check for setting a register that we know about.  */
      if (REG_P (SET_DEST (x)))
	{
	  /* See if this is setting the replacement register for an
	     elimination.

	     If DEST is the hard frame pointer, we do nothing because we
	     assume that all assignments to the frame pointer are for
	     non-local gotos and are being done at a time when they are valid
	     and do not disturb anything else.  Some machines want to
	     eliminate a fake argument pointer (or even a fake frame pointer)
	     with either the real frame or the stack pointer.  Assignments to
	     the hard frame pointer must not prevent this elimination.  */

	  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS];
	       ep++)
	    if (ep->to_rtx == SET_DEST (x)
		&& SET_DEST (x) != hard_frame_pointer_rtx)
	      {
		/* If it is being incremented, adjust the offset.  Otherwise,
		   this elimination can't be done.  */
		rtx src = SET_SRC (x);

		if (GET_CODE (src) == PLUS
		    && XEXP (src, 0) == SET_DEST (x)
		    && GET_CODE (XEXP (src, 1)) == CONST_INT)
		  ep->offset -= INTVAL (XEXP (src, 1));
		else
		  ep->can_eliminate = 0;
	      }
	}

      elimination_effects (SET_DEST (x), 0);
      elimination_effects (SET_SRC (x), 0);
      return;

    case MEM:
      /* Our only special processing is to pass the mode of the MEM to our
	 recursive call.  */
      elimination_effects (XEXP (x, 0), GET_MODE (x));
      return;

    default:
      break;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = 0; i < GET_RTX_LENGTH (code); i++, fmt++)
    {
      if (*fmt == 'e')
	elimination_effects (XEXP (x, i), mem_mode);
      else if (*fmt == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  elimination_effects (XVECEXP (x, i, j), mem_mode);
    }
}

/* Descend through rtx X and verify that no references to eliminable registers
   remain.  If any do remain, mark the involved register as not
   eliminable.  */

static void
check_eliminable_occurrences (rtx x)
{
  const char *fmt;
  int i;
  enum rtx_code code;

  if (x == 0)
    return;

  code = GET_CODE (x);

  if (code == REG && REGNO (x) < FIRST_PSEUDO_REGISTER)
    {
      struct elim_table *ep;

      for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
	if (ep->from_rtx == x)
	  ep->can_eliminate = 0;
      return;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = 0; i < GET_RTX_LENGTH (code); i++, fmt++)
    {
      if (*fmt == 'e')
	check_eliminable_occurrences (XEXP (x, i));
      else if (*fmt == 'E')
	{
	  int j;
	  for (j = 0; j < XVECLEN (x, i); j++)
	    check_eliminable_occurrences (XVECEXP (x, i, j));
	}
    }
}

/* Scan INSN and eliminate all eliminable registers in it.

   If REPLACE is nonzero, do the replacement destructively.  Also
   delete the insn as dead it if it is setting an eliminable register.

   If REPLACE is zero, do all our allocations in reload_obstack.

   If no eliminations were done and this insn doesn't require any elimination
   processing (these are not identical conditions: it might be updating sp,
   but not referencing fp; this needs to be seen during reload_as_needed so
   that the offset between fp and sp can be taken into consideration), zero
   is returned.  Otherwise, 1 is returned.  */

static int
eliminate_regs_in_insn (rtx insn, int replace)
{
  int icode = recog_memoized (insn);
  rtx old_body = PATTERN (insn);
  int insn_is_asm = asm_noperands (old_body) >= 0;
  rtx old_set = single_set (insn);
  rtx new_body;
  int val = 0;
  int i;
  rtx substed_operand[MAX_RECOG_OPERANDS];
  rtx orig_operand[MAX_RECOG_OPERANDS];
  struct elim_table *ep;
  rtx plus_src;

  if (! insn_is_asm && icode < 0)
    {
      gcc_assert (GET_CODE (PATTERN (insn)) == USE
		  || GET_CODE (PATTERN (insn)) == CLOBBER
		  || GET_CODE (PATTERN (insn)) == ADDR_VEC
		  || GET_CODE (PATTERN (insn)) == ADDR_DIFF_VEC
		  || GET_CODE (PATTERN (insn)) == ASM_INPUT);
      return 0;
    }

  if (old_set != 0 && REG_P (SET_DEST (old_set))
      && REGNO (SET_DEST (old_set)) < FIRST_PSEUDO_REGISTER)
    {
      /* Check for setting an eliminable register.  */
      for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
	if (ep->from_rtx == SET_DEST (old_set) && ep->can_eliminate)
	  {
#if HARD_FRAME_POINTER_REGNUM != FRAME_POINTER_REGNUM
	    /* If this is setting the frame pointer register to the
	       hardware frame pointer register and this is an elimination
	       that will be done (tested above), this insn is really
	       adjusting the frame pointer downward to compensate for
	       the adjustment done before a nonlocal goto.  */
	    if (ep->from == FRAME_POINTER_REGNUM
		&& ep->to == HARD_FRAME_POINTER_REGNUM)
	      {
		rtx base = SET_SRC (old_set);
		rtx base_insn = insn;
		HOST_WIDE_INT offset = 0;

		while (base != ep->to_rtx)
		  {
		    rtx prev_insn, prev_set;

		    if (GET_CODE (base) == PLUS
		        && GET_CODE (XEXP (base, 1)) == CONST_INT)
		      {
		        offset += INTVAL (XEXP (base, 1));
		        base = XEXP (base, 0);
		      }
		    else if ((prev_insn = prev_nonnote_insn (base_insn)) != 0
			     && (prev_set = single_set (prev_insn)) != 0
			     && rtx_equal_p (SET_DEST (prev_set), base))
		      {
		        base = SET_SRC (prev_set);
		        base_insn = prev_insn;
		      }
		    else
		      break;
		  }

		if (base == ep->to_rtx)
		  {
		    rtx src
		      = plus_constant (ep->to_rtx, offset - ep->offset);

		    new_body = old_body;
		    if (! replace)
		      {
			new_body = copy_insn (old_body);
			if (REG_NOTES (insn))
			  REG_NOTES (insn) = copy_insn_1 (REG_NOTES (insn));
		      }
		    PATTERN (insn) = new_body;
		    old_set = single_set (insn);

		    /* First see if this insn remains valid when we
		       make the change.  If not, keep the INSN_CODE
		       the same and let reload fit it up.  */
		    validate_change (insn, &SET_SRC (old_set), src, 1);
		    validate_change (insn, &SET_DEST (old_set),
				     ep->to_rtx, 1);
		    if (! apply_change_group ())
		      {
			SET_SRC (old_set) = src;
			SET_DEST (old_set) = ep->to_rtx;
		      }

		    val = 1;
		    goto done;
		  }
	      }
#endif

	    /* In this case this insn isn't serving a useful purpose.  We
	       will delete it in reload_as_needed once we know that this
	       elimination is, in fact, being done.  */
	    PUT_MODE (insn, SImode);
	    return 1;
	  }
    }

  /* We allow one special case which happens to work on all machines we
     currently support: a single set with the source or a REG_EQUAL
     note being a PLUS of an eliminable register and a constant.  */
  plus_src = 0;
  if (old_set && REG_P (SET_DEST (old_set)))
    {
      /* First see if the source is of the form (plus (reg) CST).  */
      if (GET_CODE (SET_SRC (old_set)) == PLUS
	  && REG_P (XEXP (SET_SRC (old_set), 0))
	  && GET_CODE (XEXP (SET_SRC (old_set), 1)) == CONST_INT
	  && REGNO (XEXP (SET_SRC (old_set), 0)) < FIRST_PSEUDO_REGISTER)
	plus_src = SET_SRC (old_set);
      else if (REG_P (SET_SRC (old_set)))
	{
	  /* Otherwise, see if we have a REG_EQUAL note of the form
	     (plus (reg) CST).  */
	  rtx links;
	  for (links = REG_NOTES (insn); links; links = XEXP (links, 1))
	    {
	      if (REG_NOTE_KIND (links) == REG_EQUAL
		  && GET_CODE (XEXP (links, 0)) == PLUS
		  && REG_P (XEXP (XEXP (links, 0), 0))
		  && GET_CODE (XEXP (XEXP (links, 0), 1)) == CONST_INT
		  && REGNO (XEXP (XEXP (links, 0), 0)) < FIRST_PSEUDO_REGISTER)
		{
		  plus_src = XEXP (links, 0);
		  break;
		}
	    }
	}
    }
  if (plus_src)
    {
      rtx reg = XEXP (plus_src, 0);
      HOST_WIDE_INT offset = INTVAL (XEXP (plus_src, 1));

      for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
	if (ep->from_rtx == reg && ep->can_eliminate)
	  {
	    offset += ep->offset;

	    if (offset == 0)
	      {
		int num_clobbers;
		/* We assume here that if we need a PARALLEL with
		   CLOBBERs for this assignment, we can do with the
		   MATCH_SCRATCHes that add_clobbers allocates.
		   There's not much we can do if that doesn't work.  */
		PATTERN (insn) = gen_rtx_SET (VOIDmode,
					      SET_DEST (old_set),
					      ep->to_rtx);
		num_clobbers = 0;
		INSN_CODE (insn) = recog (PATTERN (insn), insn, &num_clobbers);
		if (num_clobbers)
		  {
		    rtvec vec = rtvec_alloc (num_clobbers + 1);

		    vec->elem[0] = PATTERN (insn);
		    PATTERN (insn) = gen_rtx_PARALLEL (VOIDmode, vec);
		    add_clobbers (PATTERN (insn), INSN_CODE (insn));
		  }
		gcc_assert (INSN_CODE (insn) >= 0);
	      }
	    /* If we have a nonzero offset, and the source is already
	       a simple REG, the following transformation would
	       increase the cost of the insn by replacing a simple REG
	       with (plus (reg sp) CST).  So try only when plus_src
	       comes from old_set proper, not REG_NOTES.  */
	    else if (SET_SRC (old_set) == plus_src)
	      {
		new_body = old_body;
		if (! replace)
		  {
		    new_body = copy_insn (old_body);
		    if (REG_NOTES (insn))
		      REG_NOTES (insn) = copy_insn_1 (REG_NOTES (insn));
		  }
		PATTERN (insn) = new_body;
		old_set = single_set (insn);

		XEXP (SET_SRC (old_set), 0) = ep->to_rtx;
		XEXP (SET_SRC (old_set), 1) = GEN_INT (offset);
	      }
	    else
	      break;

	    val = 1;
	    /* This can't have an effect on elimination offsets, so skip right
	       to the end.  */
	    goto done;
	  }
    }

  /* Determine the effects of this insn on elimination offsets.  */
  elimination_effects (old_body, 0);

  /* Eliminate all eliminable registers occurring in operands that
     can be handled by reload.  */
  extract_insn (insn);
  for (i = 0; i < recog_data.n_operands; i++)
    {
      orig_operand[i] = recog_data.operand[i];
      substed_operand[i] = recog_data.operand[i];

      /* For an asm statement, every operand is eliminable.  */
      if (insn_is_asm || insn_data[icode].operand[i].eliminable)
	{
	  /* Check for setting a register that we know about.  */
	  if (recog_data.operand_type[i] != OP_IN
	      && REG_P (orig_operand[i]))
	    {
	      /* If we are assigning to a register that can be eliminated, it
		 must be as part of a PARALLEL, since the code above handles
		 single SETs.  We must indicate that we can no longer
		 eliminate this reg.  */
	      for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS];
		   ep++)
		if (ep->from_rtx == orig_operand[i])
		  ep->can_eliminate = 0;
	    }

	  substed_operand[i] = eliminate_regs (recog_data.operand[i], 0,
					       replace ? insn : NULL_RTX);
	  if (substed_operand[i] != orig_operand[i])
	    val = 1;
	  /* Terminate the search in check_eliminable_occurrences at
	     this point.  */
	  *recog_data.operand_loc[i] = 0;

	/* If an output operand changed from a REG to a MEM and INSN is an
	   insn, write a CLOBBER insn.  */
	  if (recog_data.operand_type[i] != OP_IN
	      && REG_P (orig_operand[i])
	      && MEM_P (substed_operand[i])
	      && replace)
	    emit_insn_after (gen_rtx_CLOBBER (VOIDmode, orig_operand[i]),
			     insn);
	}
    }

  for (i = 0; i < recog_data.n_dups; i++)
    *recog_data.dup_loc[i]
      = *recog_data.operand_loc[(int) recog_data.dup_num[i]];

  /* If any eliminable remain, they aren't eliminable anymore.  */
  check_eliminable_occurrences (old_body);

  /* Substitute the operands; the new values are in the substed_operand
     array.  */
  for (i = 0; i < recog_data.n_operands; i++)
    *recog_data.operand_loc[i] = substed_operand[i];
  for (i = 0; i < recog_data.n_dups; i++)
    *recog_data.dup_loc[i] = substed_operand[(int) recog_data.dup_num[i]];

  /* If we are replacing a body that was a (set X (plus Y Z)), try to
     re-recognize the insn.  We do this in case we had a simple addition
     but now can do this as a load-address.  This saves an insn in this
     common case.
     If re-recognition fails, the old insn code number will still be used,
     and some register operands may have changed into PLUS expressions.
     These will be handled by find_reloads by loading them into a register
     again.  */

  if (val)
    {
      /* If we aren't replacing things permanently and we changed something,
	 make another copy to ensure that all the RTL is new.  Otherwise
	 things can go wrong if find_reload swaps commutative operands
	 and one is inside RTL that has been copied while the other is not.  */
      new_body = old_body;
      if (! replace)
	{
	  new_body = copy_insn (old_body);
	  if (REG_NOTES (insn))
	    REG_NOTES (insn) = copy_insn_1 (REG_NOTES (insn));
	}
      PATTERN (insn) = new_body;

      /* If we had a move insn but now we don't, rerecognize it.  This will
	 cause spurious re-recognition if the old move had a PARALLEL since
	 the new one still will, but we can't call single_set without
	 having put NEW_BODY into the insn and the re-recognition won't
	 hurt in this rare case.  */
      /* ??? Why this huge if statement - why don't we just rerecognize the
	 thing always?  */
      if (! insn_is_asm
	  && old_set != 0
	  && ((REG_P (SET_SRC (old_set))
	       && (GET_CODE (new_body) != SET
		   || !REG_P (SET_SRC (new_body))))
	      /* If this was a load from or store to memory, compare
		 the MEM in recog_data.operand to the one in the insn.
		 If they are not equal, then rerecognize the insn.  */
	      || (old_set != 0
		  && ((MEM_P (SET_SRC (old_set))
		       && SET_SRC (old_set) != recog_data.operand[1])
		      || (MEM_P (SET_DEST (old_set))
			  && SET_DEST (old_set) != recog_data.operand[0])))
	      /* If this was an add insn before, rerecognize.  */
	      || GET_CODE (SET_SRC (old_set)) == PLUS))
	{
	  int new_icode = recog (PATTERN (insn), insn, 0);
	  if (new_icode < 0)
	    INSN_CODE (insn) = icode;
	}
    }

  /* Restore the old body.  If there were any changes to it, we made a copy
     of it while the changes were still in place, so we'll correctly return
     a modified insn below.  */
  if (! replace)
    {
      /* Restore the old body.  */
      for (i = 0; i < recog_data.n_operands; i++)
	*recog_data.operand_loc[i] = orig_operand[i];
      for (i = 0; i < recog_data.n_dups; i++)
	*recog_data.dup_loc[i] = orig_operand[(int) recog_data.dup_num[i]];
    }

  /* Update all elimination pairs to reflect the status after the current
     insn.  The changes we make were determined by the earlier call to
     elimination_effects.

     We also detect cases where register elimination cannot be done,
     namely, if a register would be both changed and referenced outside a MEM
     in the resulting insn since such an insn is often undefined and, even if
     not, we cannot know what meaning will be given to it.  Note that it is
     valid to have a register used in an address in an insn that changes it
     (presumably with a pre- or post-increment or decrement).

     If anything changes, return nonzero.  */

  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
    {
      if (ep->previous_offset != ep->offset && ep->ref_outside_mem)
	ep->can_eliminate = 0;

      ep->ref_outside_mem = 0;

      if (ep->previous_offset != ep->offset)
	val = 1;
    }

 done:
  /* If we changed something, perform elimination in REG_NOTES.  This is
     needed even when REPLACE is zero because a REG_DEAD note might refer
     to a register that we eliminate and could cause a different number
     of spill registers to be needed in the final reload pass than in
     the pre-passes.  */
  if (val && REG_NOTES (insn) != 0)
    REG_NOTES (insn) = eliminate_regs (REG_NOTES (insn), 0, REG_NOTES (insn));

  return val;
}

/* Loop through all elimination pairs.
   Recalculate the number not at initial offset.

   Compute the maximum offset (minimum offset if the stack does not
   grow downward) for each elimination pair.  */

static void
update_eliminable_offsets (void)
{
  struct elim_table *ep;

  num_not_at_initial_offset = 0;
  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
    {
      ep->previous_offset = ep->offset;
      if (ep->can_eliminate && ep->offset != ep->initial_offset)
	num_not_at_initial_offset++;
    }
}

/* Given X, a SET or CLOBBER of DEST, if DEST is the target of a register
   replacement we currently believe is valid, mark it as not eliminable if X
   modifies DEST in any way other than by adding a constant integer to it.

   If DEST is the frame pointer, we do nothing because we assume that
   all assignments to the hard frame pointer are nonlocal gotos and are being
   done at a time when they are valid and do not disturb anything else.
   Some machines want to eliminate a fake argument pointer with either the
   frame or stack pointer.  Assignments to the hard frame pointer must not
   prevent this elimination.

   Called via note_stores from reload before starting its passes to scan
   the insns of the function.  */

static void
mark_not_eliminable (rtx dest, rtx x, void *data ATTRIBUTE_UNUSED)
{
  unsigned int i;

  /* A SUBREG of a hard register here is just changing its mode.  We should
     not see a SUBREG of an eliminable hard register, but check just in
     case.  */
  if (GET_CODE (dest) == SUBREG)
    dest = SUBREG_REG (dest);

  if (dest == hard_frame_pointer_rtx)
    return;

  for (i = 0; i < NUM_ELIMINABLE_REGS; i++)
    if (reg_eliminate[i].can_eliminate && dest == reg_eliminate[i].to_rtx
	&& (GET_CODE (x) != SET
	    || GET_CODE (SET_SRC (x)) != PLUS
	    || XEXP (SET_SRC (x), 0) != dest
	    || GET_CODE (XEXP (SET_SRC (x), 1)) != CONST_INT))
      {
	reg_eliminate[i].can_eliminate_previous
	  = reg_eliminate[i].can_eliminate = 0;
	num_eliminable--;
      }
}

/* Verify that the initial elimination offsets did not change since the
   last call to set_initial_elim_offsets.  This is used to catch cases
   where something illegal happened during reload_as_needed that could
   cause incorrect code to be generated if we did not check for it.  */

static void
verify_initial_elim_offsets (void)
{
  HOST_WIDE_INT t;

#ifdef ELIMINABLE_REGS
  struct elim_table *ep;

  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
    {
      INITIAL_ELIMINATION_OFFSET (ep->from, ep->to, t);
      gcc_assert (t == ep->initial_offset);
    }
#else
  INITIAL_FRAME_POINTER_OFFSET (t);
  gcc_assert (t == reg_eliminate[0].initial_offset);
#endif
}

/* Reset all offsets on eliminable registers to their initial values.  */

static void
set_initial_elim_offsets (void)
{
  struct elim_table *ep = reg_eliminate;

#ifdef ELIMINABLE_REGS
  for (; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
    {
      INITIAL_ELIMINATION_OFFSET (ep->from, ep->to, ep->initial_offset);
      ep->previous_offset = ep->offset = ep->initial_offset;
    }
#else
  INITIAL_FRAME_POINTER_OFFSET (ep->initial_offset);
  ep->previous_offset = ep->offset = ep->initial_offset;
#endif

  num_not_at_initial_offset = 0;
}

/* Subroutine of set_initial_label_offsets called via for_each_eh_label.  */

static void
set_initial_eh_label_offset (rtx label)
{
  set_label_offsets (label, NULL_RTX, 1);
}

/* Initialize the known label offsets.
   Set a known offset for each forced label to be at the initial offset
   of each elimination.  We do this because we assume that all
   computed jumps occur from a location where each elimination is
   at its initial offset.
   For all other labels, show that we don't know the offsets.  */

static void
set_initial_label_offsets (void)
{
  rtx x;
  memset (offsets_known_at, 0, num_labels);

  for (x = forced_labels; x; x = XEXP (x, 1))
    if (XEXP (x, 0))
      set_label_offsets (XEXP (x, 0), NULL_RTX, 1);

  for_each_eh_label (set_initial_eh_label_offset);
}

/* Set all elimination offsets to the known values for the code label given
   by INSN.  */

static void
set_offsets_for_label (rtx insn)
{
  unsigned int i;
  int label_nr = CODE_LABEL_NUMBER (insn);
  struct elim_table *ep;

  num_not_at_initial_offset = 0;
  for (i = 0, ep = reg_eliminate; i < NUM_ELIMINABLE_REGS; ep++, i++)
    {
      ep->offset = ep->previous_offset
		 = offsets_at[label_nr - first_label_num][i];
      if (ep->can_eliminate && ep->offset != ep->initial_offset)
	num_not_at_initial_offset++;
    }
}

/* See if anything that happened changes which eliminations are valid.
   For example, on the SPARC, whether or not the frame pointer can
   be eliminated can depend on what registers have been used.  We need
   not check some conditions again (such as flag_omit_frame_pointer)
   since they can't have changed.  */

static void
update_eliminables (HARD_REG_SET *pset)
{
  int previous_frame_pointer_needed = frame_pointer_needed;
  struct elim_table *ep;

  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
    if ((ep->from == HARD_FRAME_POINTER_REGNUM && FRAME_POINTER_REQUIRED)
#ifdef ELIMINABLE_REGS
	|| ! CAN_ELIMINATE (ep->from, ep->to)
#endif
	)
      ep->can_eliminate = 0;

  /* Look for the case where we have discovered that we can't replace
     register A with register B and that means that we will now be
     trying to replace register A with register C.  This means we can
     no longer replace register C with register B and we need to disable
     such an elimination, if it exists.  This occurs often with A == ap,
     B == sp, and C == fp.  */

  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
    {
      struct elim_table *op;
      int new_to = -1;

      if (! ep->can_eliminate && ep->can_eliminate_previous)
	{
	  /* Find the current elimination for ep->from, if there is a
	     new one.  */
	  for (op = reg_eliminate;
	       op < &reg_eliminate[NUM_ELIMINABLE_REGS]; op++)
	    if (op->from == ep->from && op->can_eliminate)
	      {
		new_to = op->to;
		break;
	      }

	  /* See if there is an elimination of NEW_TO -> EP->TO.  If so,
	     disable it.  */
	  for (op = reg_eliminate;
	       op < &reg_eliminate[NUM_ELIMINABLE_REGS]; op++)
	    if (op->from == new_to && op->to == ep->to)
	      op->can_eliminate = 0;
	}
    }

  /* See if any registers that we thought we could eliminate the previous
     time are no longer eliminable.  If so, something has changed and we
     must spill the register.  Also, recompute the number of eliminable
     registers and see if the frame pointer is needed; it is if there is
     no elimination of the frame pointer that we can perform.  */

  frame_pointer_needed = 1;
  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
    {
      if (ep->can_eliminate && ep->from == FRAME_POINTER_REGNUM
	  && ep->to != HARD_FRAME_POINTER_REGNUM)
	frame_pointer_needed = 0;

      if (! ep->can_eliminate && ep->can_eliminate_previous)
	{
	  ep->can_eliminate_previous = 0;
	  SET_HARD_REG_BIT (*pset, ep->from);
	  num_eliminable--;
	}
    }

  /* If we didn't need a frame pointer last time, but we do now, spill
     the hard frame pointer.  */
  if (frame_pointer_needed && ! previous_frame_pointer_needed)
    SET_HARD_REG_BIT (*pset, HARD_FRAME_POINTER_REGNUM);
}

/* Initialize the table of registers to eliminate.  */

static void
init_elim_table (void)
{
  struct elim_table *ep;
#ifdef ELIMINABLE_REGS
  const struct elim_table_1 *ep1;
#endif

  if (!reg_eliminate)
    reg_eliminate = xcalloc (sizeof (struct elim_table), NUM_ELIMINABLE_REGS);

  /* Does this function require a frame pointer?  */

  frame_pointer_needed = (! flag_omit_frame_pointer
			  /* ?? If EXIT_IGNORE_STACK is set, we will not save
			     and restore sp for alloca.  So we can't eliminate
			     the frame pointer in that case.  At some point,
			     we should improve this by emitting the
			     sp-adjusting insns for this case.  */
			  || (current_function_calls_alloca
			      && EXIT_IGNORE_STACK)
			  || FRAME_POINTER_REQUIRED);

  num_eliminable = 0;

#ifdef ELIMINABLE_REGS
  for (ep = reg_eliminate, ep1 = reg_eliminate_1;
       ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++, ep1++)
    {
      ep->from = ep1->from;
      ep->to = ep1->to;
      ep->can_eliminate = ep->can_eliminate_previous
	= (CAN_ELIMINATE (ep->from, ep->to)
	   && ! (ep->to == STACK_POINTER_REGNUM && frame_pointer_needed));
    }
#else
  reg_eliminate[0].from = reg_eliminate_1[0].from;
  reg_eliminate[0].to = reg_eliminate_1[0].to;
  reg_eliminate[0].can_eliminate = reg_eliminate[0].can_eliminate_previous
    = ! frame_pointer_needed;
#endif

  /* Count the number of eliminable registers and build the FROM and TO
     REG rtx's.  Note that code in gen_rtx_REG will cause, e.g.,
     gen_rtx_REG (Pmode, STACK_POINTER_REGNUM) to equal stack_pointer_rtx.
     We depend on this.  */
  for (ep = reg_eliminate; ep < &reg_eliminate[NUM_ELIMINABLE_REGS]; ep++)
    {
      num_eliminable += ep->can_eliminate;
      ep->from_rtx = gen_rtx_REG (Pmode, ep->from);
      ep->to_rtx = gen_rtx_REG (Pmode, ep->to);
    }
}

/* Kick all pseudos out of hard register REGNO.

   If CANT_ELIMINATE is nonzero, it means that we are doing this spill
   because we found we can't eliminate some register.  In the case, no pseudos
   are allowed to be in the register, even if they are only in a block that
   doesn't require spill registers, unlike the case when we are spilling this
   hard reg to produce another spill register.

   Return nonzero if any pseudos needed to be kicked out.  */

static void
spill_hard_reg (unsigned int regno, int cant_eliminate)
{
  int i;

  if (cant_eliminate)
    {
      SET_HARD_REG_BIT (bad_spill_regs_global, regno);
      regs_ever_live[regno] = 1;
    }

  /* Spill every pseudo reg that was allocated to this reg
     or to something that overlaps this reg.  */

  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    if (reg_renumber[i] >= 0
	&& (unsigned int) reg_renumber[i] <= regno
	&& ((unsigned int) reg_renumber[i]
	    + hard_regno_nregs[(unsigned int) reg_renumber[i]]
			      [PSEUDO_REGNO_MODE (i)]
	    > regno))
      SET_REGNO_REG_SET (&spilled_pseudos, i);
}

/* After find_reload_regs has been run for all insn that need reloads,
   and/or spill_hard_regs was called, this function is used to actually
   spill pseudo registers and try to reallocate them.  It also sets up the
   spill_regs array for use by choose_reload_regs.  */

static int
finish_spills (int global)
{
  struct insn_chain *chain;
  int something_changed = 0;
  unsigned i;
  reg_set_iterator rsi;

  /* Build the spill_regs array for the function.  */
  /* If there are some registers still to eliminate and one of the spill regs
     wasn't ever used before, additional stack space may have to be
     allocated to store this register.  Thus, we may have changed the offset
     between the stack and frame pointers, so mark that something has changed.

     One might think that we need only set VAL to 1 if this is a call-used
     register.  However, the set of registers that must be saved by the
     prologue is not identical to the call-used set.  For example, the
     register used by the call insn for the return PC is a call-used register,
     but must be saved by the prologue.  */

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (used_spill_regs, i))
      {
	if (num_eliminable && ! regs_ever_live[i])
	  something_changed = 1;
	regs_ever_live[i] = 1;
      }

  EXECUTE_IF_SET_IN_REG_SET (&spilled_pseudos, FIRST_PSEUDO_REGISTER, i, rsi)
    {
      /* Record the current hard register the pseudo is allocated to in
	 pseudo_previous_regs so we avoid reallocating it to the same
	 hard reg in a later pass.  */
      gcc_assert (reg_renumber[i] >= 0);

      SET_HARD_REG_BIT (pseudo_previous_regs[i], reg_renumber[i]);
      /* Mark it as no longer having a hard register home.  */
      reg_renumber[i] = -1;
      /* We will need to scan everything again.  */
      something_changed = 1;
    }

  /* Retry global register allocation if possible.  */
  if (global)
    {
      memset (pseudo_forbidden_regs, 0, max_regno * sizeof (HARD_REG_SET));
      /* For every insn that needs reloads, set the registers used as spill
	 regs in pseudo_forbidden_regs for every pseudo live across the
	 insn.  */
      for (chain = insns_need_reload; chain; chain = chain->next_need_reload)
	{
	  EXECUTE_IF_SET_IN_REG_SET
	    (&chain->live_before, FIRST_PSEUDO_REGISTER, i, rsi)
	     {
	       IOR_HARD_REG_SET (pseudo_forbidden_regs[i],
				 chain->used_spill_regs);
	     }
	  EXECUTE_IF_SET_IN_REG_SET
	    (&chain->live_after, FIRST_PSEUDO_REGISTER, i, rsi)
	     {
	       IOR_HARD_REG_SET (pseudo_forbidden_regs[i],
				 chain->used_spill_regs);
	     }
	}

      /* Retry allocating the spilled pseudos.  For each reg, merge the
	 various reg sets that indicate which hard regs can't be used,
	 and call retry_global_alloc.
	 We change spill_pseudos here to only contain pseudos that did not
	 get a new hard register.  */
      for (i = FIRST_PSEUDO_REGISTER; i < (unsigned)max_regno; i++)
	if (reg_old_renumber[i] != reg_renumber[i])
	  {
	    HARD_REG_SET forbidden;
	    COPY_HARD_REG_SET (forbidden, bad_spill_regs_global);
	    IOR_HARD_REG_SET (forbidden, pseudo_forbidden_regs[i]);
	    IOR_HARD_REG_SET (forbidden, pseudo_previous_regs[i]);
	    retry_global_alloc (i, forbidden);
	    if (reg_renumber[i] >= 0)
	      CLEAR_REGNO_REG_SET (&spilled_pseudos, i);
	  }
    }

  /* Fix up the register information in the insn chain.
     This involves deleting those of the spilled pseudos which did not get
     a new hard register home from the live_{before,after} sets.  */
  for (chain = reload_insn_chain; chain; chain = chain->next)
    {
      AND_COMPL_REG_SET (&chain->live_before, &spilled_pseudos);
      AND_COMPL_REG_SET (&chain->live_after, &spilled_pseudos);
    }

  /* Let alter_reg modify the reg rtx's for the modified pseudos.  */
  for (i = FIRST_PSEUDO_REGISTER; i < (unsigned)max_regno; i++)
    {
      int regno = reg_renumber[i];
      if (reg_old_renumber[i] == regno)
	continue;

      alter_reg (i, reg_old_renumber[i]);
      reg_old_renumber[i] = regno;
      if (dump_file)
	{
	  if (regno == -1)
	    fprintf (dump_file, " Register %d now on stack.\n\n", i);
	  else
	    fprintf (dump_file, " Register %d now in %d.\n\n",
		     i, reg_renumber[i]);
	}
    }

  return something_changed;
}

/* Find all paradoxical subregs within X and update reg_max_ref_width.  */

static void
scan_paradoxical_subregs (rtx x)
{
  int i;
  const char *fmt;
  enum rtx_code code = GET_CODE (x);

  switch (code)
    {
    case REG:
    case CONST_INT:
    case CONST:
    case SYMBOL_REF:
    case LABEL_REF:
    case CONST_DOUBLE:
    case CONST_VECTOR: /* shouldn't happen, but just in case.  */
    case CC0:
    case PC:
    case USE:
    case CLOBBER:
      return;

    case SUBREG:
      if (REG_P (SUBREG_REG (x))
	  && GET_MODE_SIZE (GET_MODE (x)) > GET_MODE_SIZE (GET_MODE (SUBREG_REG (x))))
	reg_max_ref_width[REGNO (SUBREG_REG (x))]
	  = GET_MODE_SIZE (GET_MODE (x));
      return;

    default:
      break;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	scan_paradoxical_subregs (XEXP (x, i));
      else if (fmt[i] == 'E')
	{
	  int j;
	  for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	    scan_paradoxical_subregs (XVECEXP (x, i, j));
	}
    }
}

/* Reload pseudo-registers into hard regs around each insn as needed.
   Additional register load insns are output before the insn that needs it
   and perhaps store insns after insns that modify the reloaded pseudo reg.  */

static void
reload_as_needed (int live_known)
{
  struct insn_chain *chain;
#if defined (AUTO_INC_DEC)
  int i;
#endif

  memset (spill_reg_rtx, 0, sizeof spill_reg_rtx);
  CLEAR_HARD_REG_SET (reg_reloaded_call_part_clobbered);

  set_initial_elim_offsets ();

  for (chain = reload_insn_chain; chain; chain = chain->next)
    {
      rtx prev = 0;
      rtx insn = chain->insn;

      /* If we pass a label, copy the offsets from the label information
	 into the current offsets of each elimination.  */
      if (LABEL_P (insn))
	set_offsets_for_label (insn);

      else if (INSN_P (insn))
	{
	  /* If this is a USE and CLOBBER of a MEM, ensure that any
	     references to eliminable registers have been removed.  */

	  if ((GET_CODE (PATTERN (insn)) == USE
	       || GET_CODE (PATTERN (insn)) == CLOBBER)
	      && MEM_P (XEXP (PATTERN (insn), 0)))
	    XEXP (XEXP (PATTERN (insn), 0), 0)
	      = eliminate_regs (XEXP (XEXP (PATTERN (insn), 0), 0),
				GET_MODE (XEXP (PATTERN (insn), 0)),
				NULL_RTX);

	  /* If we need to do register elimination processing, do so.
	     This might delete the insn, in which case we are done.  */
	  if ((num_eliminable || num_eliminable_invariants) && chain->need_elim)
	    {
	      eliminate_regs_in_insn (insn, 1);
	      if (NOTE_P (insn))
		{
		  update_eliminable_offsets ();
		  continue;
		}
	    }

	  if (chain->will_be_deleted)
	    {
	      PUT_CODE (insn, NOTE);
	      NOTE_LINE_NUMBER (insn) = NOTE_INSN_DELETED;
	      NOTE_SOURCE_FILE (insn) = 0;
	      update_eliminable_offsets ();
	      continue;
	    }

	  /* If any of these three conditions are true, the actions of
	     find_reloads must be redone here; it may modify the insn.  */

	  if (chain->need_elim || chain->n_reloads > 0
	      || chain->need_operand_change)
	  /* First find the pseudo regs that must be reloaded for this insn.
	     This info is returned in the tables reload_... (see reload.h).
	     Also modify the body of INSN by substituting RELOAD
	     rtx's for those pseudo regs.  */
	    find_reloads (chain, insn, spill_indirect_levels, live_known);

	  if (chain->n_reloads > 0)
	    {
	      rtx next = NEXT_INSN (insn);
	      rtx p;

	      prev = PREV_INSN (insn);

	      /* Now compute which reload regs to reload them into.  Perhaps
		 reusing reload regs from previous insns, or else output
		 load insns to reload them.  Maybe output store insns too.
		 Record the choices of reload reg in reload_reg_rtx.  */
	      choose_reload_regs (chain);

	      /* Generate the insns to reload operands into or out of
		 their reload regs.  */
	      emit_reload_insns (chain);

	      /* Substitute the chosen reload regs from reload_reg_rtx
		 into the insn's body (or perhaps into the bodies of other
		 load and store insn that we just made for reloading
		 and that we moved the structure into).  */
	      subst_reloads (insn);

	      /* If this was an ASM, make sure that all the reload insns
		 we have generated are valid.  If not, give an error
		 and delete them.  */

	      if (asm_noperands (PATTERN (insn)) >= 0)
		for (p = NEXT_INSN (prev); p != next; p = NEXT_INSN (p))
		  if (p != insn && INSN_P (p)
		      && GET_CODE (PATTERN (p)) != USE
		      && (recog_memoized (p) < 0
			  || (extract_insn (p), ! constrain_operands (1))))
		    {
		      error_for_asm (insn,
				     "%<asm%> operand requires "
				     "impossible reload");
		      delete_insn (p);
		    }
	    }

	  if (num_eliminable && chain->need_elim)
	    update_eliminable_offsets ();
	}
    }
}

/* Return nonzero if the rtx X is invariant over the current function.  */
/* ??? Actually, the places where we use this expect exactly what is
   tested here, and not everything that is function invariant.  In
   particular, the frame pointer and arg pointer are special cased;
   pic_offset_table_rtx is not, and we must not spill these things to
   memory.  */

static int
function_invariant_p (rtx x)
{
  if (CONSTANT_P (x))
    return 1;
  if (x == frame_pointer_rtx || x == arg_pointer_rtx)
    return 1;
  if (GET_CODE (x) == PLUS
      && (XEXP (x, 0) == frame_pointer_rtx || XEXP (x, 0) == arg_pointer_rtx)
      && CONSTANT_P (XEXP (x, 1)))
    return 1;
  return 0;
}

/* Assign hard reg targets for the pseudo-registers we must reload
   into hard regs for this insn.  */

static void
choose_reload_regs (struct insn_chain *chain)
{
  int i;

  for (i = 0; i < chain->n_reloads; i++)
    {
      if (rld[i].class != chain->rld[i].class
	  || rld[i].inmode != chain->rld[i].inmode
	  || rld[i].outmode != chain->rld[i].outmode)
	abort ();
    }

  for (i = 0; i < chain->n_reloads; i++)
    {
      rtx new;
      int regno = chain->rld[i].reginfo.regno;
      rld[i].reg_rtx = 0;

      if (! chain->rld[i].reginfo.allocated)
	continue;

      if (chain->rld[i].override_in)
	rld[i].in = chain->rld[i].override_in;
      if (chain->rld[i].override_out)
	rld[i].out = chain->rld[i].override_out;

      new = spill_reg_rtx[regno];

      if (new == 0 || GET_MODE (new) != rld[i].mode)
	new = gen_rtx_REG (rld[i].mode, regno);
      rld[i].reg_rtx = spill_reg_rtx[regno] = new;
    }
}

/* Generate insns to perform reload RL, which is for the insn in CHAIN and
   has the number J.  OLD contains the value to be used as input.  */

static void
emit_input_reload_insns (struct insn_chain *chain, struct reload *rl,
			 rtx inval, int j)
{
  rtx insn = chain->insn;
  rtx reloadreg = rl->reg_rtx;
  enum machine_mode mode;

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
     a float-only register.  In this case INVAL's mode is correct.

     Consider an immediate integer: it has VOIDmode.  Here we need
     to get a mode from something else.

     In some cases, there is a fourth mode, the operand's
     containing mode.  If the insn specifies a containing mode for
     this operand, it overrides all others.

     I am not sure whether the algorithm here is always right,
     but it does the right things in those cases.  */

  mode = GET_MODE (inval);
  if (mode == VOIDmode)
    mode = rl->inmode;

  /* Encapsulate both RELOADREG and INVAL into that mode,
     then load RELOADREG from INVAL.  Note that we cannot use
     gen_lowpart_common since it can do the wrong thing when
     RELOADREG has a multi-word mode.  Note that RELOADREG
     must always be a REG here.  */

  if (GET_MODE (reloadreg) != mode)
    reloadreg = reload_adjust_reg_for_mode (reloadreg, mode);
  while (GET_CODE (inval) == SUBREG && GET_MODE (inval) != mode)
    inval = SUBREG_REG (inval);
  if (GET_MODE (inval) != VOIDmode
      && mode != GET_MODE (inval))
    inval = gen_lowpart_SUBREG (mode, inval);

  /* Auto-increment addresses must be reloaded in a special way.  */
  if (rl->inc)
    {
      /* We are not going to bother supporting the case where a
	 incremented register can't be copied directly from
	 INVAL since this seems highly unlikely.  */
      if (rl->secondary_in_reload >= 0)
	abort ();

      /* Output a special code sequence for this case.  */
      inc_for_reload (reloadreg, inval, inval, rl->inc);
      /* Prevent normal processing of this reload.  */
      goto out;
    }

  /* We can't do that, so output an insn to load RELOADREG.  */

#ifdef SECONDARY_INPUT_RELOAD_CLASS
  if (rl->secondary_in_reload >= 0)
    {
      struct reload *rl2 = rld + rl->secondary_in_reload;
      rtx tmp;
      enum insn_code icode = rl->secondary_in_icode;
      /* See if we need a scratch register to load the
	 intermediate register (a tertiary reload).  */
      enum insn_code tertiary_icode = rl2->secondary_in_icode;

      if (! rl2->reg_rtx)
	abort ();

      if (GET_CODE (inval) == REG && REGNO (inval) >= FIRST_PSEUDO_REGISTER
	  && reg_equiv_mem[REGNO (inval)] != 0)
	inval = reg_equiv_mem[REGNO (inval)];

      if (icode != CODE_FOR_nothing)
	{
	  emit_insn (GEN_FCN (icode) (reloadreg, inval, rl2->reg_rtx));
	  goto out;
	}

      if (tertiary_icode != CODE_FOR_nothing)
	{
	  rtx third_reload_reg = rld[rl2->secondary_in_reload].reg_rtx;

	  emit_insn (GEN_FCN (tertiary_icode) (rl2->reg_rtx, inval,
					       third_reload_reg));
	}
      else
	gen_reload (rl2->reg_rtx, inval, j);

      inval = rl2->reg_rtx;
    }
#endif

  if (! rtx_equal_p (reloadreg, inval))
    gen_reload (reloadreg, inval, j);

  out:
  if (flag_non_call_exceptions)
    copy_eh_notes (insn, get_insns ());
}

/* Generate insns to for the output reload RL, which is for the insn described
   by CHAIN and has the number J.  */
static void
emit_output_reload_insns (struct insn_chain *chain, struct reload *rl,
			  int j)
{
  rtx reloadreg = rl->reg_rtx;
  rtx insn = chain->insn;
  rtx set, old = rl->out;
  enum machine_mode mode = GET_MODE (old);

  /* Determine the mode to reload in.
     See comments above (for input reloading).  */

  if (mode == VOIDmode)
    {
      /* VOIDmode should never happen for an output.  */
      if (asm_noperands (PATTERN (insn)) < 0)
	/* It's the compiler's fault.  */
	fatal_insn ("VOIDmode on an output", insn);
      error_for_asm (insn, "output operand is constant in %<asm%>");
      /* Prevent crash--use something we know is valid.  */
      mode = word_mode;
      old = gen_rtx_REG (mode, REGNO (reloadreg));
    }

  if (GET_MODE (reloadreg) != mode)
    reloadreg = reload_adjust_reg_for_mode (reloadreg, mode);

#ifdef SECONDARY_OUTPUT_RELOAD_CLASS

  /* If we need two reload regs, set RELOADREG to the intermediate
     one, since it will be stored into OLD.  We might need a secondary
     register only for an input reload, so check again here.  */

  if (rl->secondary_out_reload >= 0)
    {
      struct reload *rl2 = rld + rl->secondary_out_reload;
      rtx second_reloadreg = reloadreg;
      rtx real_old = old;
      enum insn_code tertiary_icode = rl2->secondary_out_icode;

      if (GET_CODE (old) == REG && REGNO (old) >= FIRST_PSEUDO_REGISTER
	  && reg_equiv_mem[REGNO (old)] != 0)
	real_old = reg_equiv_mem[REGNO (old)];

      reloadreg = rl2->reg_rtx;

      /* See if RELOADREG is to be used as a scratch register
	 or as an intermediate register.  */
      if (rl->secondary_out_icode != CODE_FOR_nothing)
	{
	  emit_insn ((GEN_FCN (rl->secondary_out_icode)
		      (real_old, second_reloadreg, reloadreg)));
	  goto out;
	}

      /* See if we need both a scratch and intermediate reload
	 register.  */

      if (GET_MODE (reloadreg) != mode)
	reloadreg = gen_rtx_REG (mode, REGNO (reloadreg));

      if (tertiary_icode != CODE_FOR_nothing)
	{
	  rtx third_reloadreg = rld[rl2->secondary_out_reload].reg_rtx;
	  rtx tem;

	  /* Copy primary reload reg to secondary reload reg.
	     (Note that these have been swapped above, then
	     secondary reload reg to OLD using our insn.  */

	  /* If REAL_OLD is a paradoxical SUBREG, remove it
	     and try to put the opposite SUBREG on
	     RELOADREG.  */
	  if (GET_CODE (real_old) == SUBREG
	      && (GET_MODE_SIZE (GET_MODE (real_old))
		  > GET_MODE_SIZE (GET_MODE (SUBREG_REG (real_old))))
	      && 0 != (tem = gen_lowpart_common
		       (GET_MODE (SUBREG_REG (real_old)),
			reloadreg)))
	    real_old = SUBREG_REG (real_old), reloadreg = tem;

	  gen_reload (reloadreg, second_reloadreg, j);
	  emit_insn ((GEN_FCN (tertiary_icode)
		      (real_old, reloadreg, third_reloadreg)));
	  goto out;
	}

      /* Copy between the reload regs here and then to
	 OUT later.  */
      gen_reload (reloadreg, second_reloadreg, j);
    }
#endif

  /* Output the last reload insn.  */

  /* Don't output the last reload if OLD is not the dest of
     INSN and is in the src and is clobbered by INSN. */
  if (! flag_expensive_optimizations
      || GET_CODE (old) != REG
      || ! (set = single_set (insn))
      || rtx_equal_p (old, SET_DEST (set))
      || ! reg_mentioned_p (old, SET_SRC (set))
      || !((REGNO (old) < FIRST_PSEUDO_REGISTER)
	   && regno_clobbered_p (REGNO (old), insn, rl->mode, 0)))
    gen_reload (old, reloadreg, j);

 out:
  if (flag_non_call_exceptions)
    copy_eh_notes (insn, get_insns ());
}

/* Do input reloading for reload RL, which is for the insn described by CHAIN
   and has the number J.  */
static rtx
do_input_reload (struct insn_chain *chain, struct reload *rl, int j)
{
  register rtx retval;
  register rtx reloadreg = rl->reg_rtx;
  rtx old = rl->in;

  if (old == 0
      || reloadreg == 0
      || rtx_equal_p (reloadreg, old))
    return 0;

  start_sequence ();
  emit_input_reload_insns (chain, rl, old, j);
  retval = get_insns ();
  end_sequence ();
  return retval;
}

/* Do output reloading for reload RL, which is for the insn described by
   CHAIN and has the number J.
   ??? At some point we need to support handling output reloads of
   JUMP_INSNs or insns that set cc0.  */
static rtx
do_output_reload (struct insn_chain *chain, struct reload *rl, int j)
{
  rtx insn = chain->insn;
  register rtx old, retval;

  old = rl->out;
  if (old == 0
      || rl->reg_rtx == 0
      || rtx_equal_p (rl->reg_rtx, old))
    return 0;

  /* If is a JUMP_INSN, we can't support output reloads yet.  */
  gcc_assert (!JUMP_P (insn));

  start_sequence ();
  emit_output_reload_insns (chain, rl, j);
  retval = get_insns ();
  end_sequence ();

  return retval;
}

/* Output insns to reload values in and out of the chosen reload regs.  */

static void
emit_reload_insns (struct insn_chain *chain)
{
  rtx insn = chain->insn;
  rtx mark = insn;
  int after = 1;
  int rli_nr = chain->last_rlinsn;

  /* Dump reloads into the dump file.  */
  if (dump_file)
    {
      fprintf (dump_file, "\nReloads for insn # %d\n", INSN_UID (insn));
      debug_reload_to_stream (dump_file);
    }

  while (rli_nr >= 0)
    {
      struct reload_insn *rli = chain->rli + rli_nr;

      if (rli->status != RLIS_SCHEDULED)
	abort ();

      if (rli->type == RLI_INSN)
	after = 0;
      else if (! rli->ignored)
	{
	  rtx seq = 0;
	  if (rli->type == RLI_OUTPUTRELOAD)
	    seq = do_output_reload (chain, rld + rli->nr, rli->nr);
	  else if (rli->type == RLI_INPUTRELOAD)
	    seq = do_input_reload (chain, rld + rli->nr, rli->nr);
	  if (seq)
	    {
	      if (after)
		emit_insn_after (seq, mark);
	      else
		{
		  emit_insn_before (seq, mark);
		  mark = seq;
		}
	    }
	}
      rli_nr = rli->prev_order;
    }
}

/* Emit code to perform a reload from IN (which may be a reload register) to
   OUT (which may also be a reload register).  IN or OUT is from operand
   OPNUM with reload type TYPE.

   Returns first insn emitted.  */

static rtx
gen_reload (rtx out, rtx in, int opnum)
{
  rtx last = get_last_insn ();
  rtx tem;

  /* If IN is a paradoxical SUBREG, remove it and try to put the
     opposite SUBREG on OUT.  Likewise for a paradoxical SUBREG on OUT.  */
  if (GET_CODE (in) == SUBREG
      && (GET_MODE_SIZE (GET_MODE (in))
	  > GET_MODE_SIZE (GET_MODE (SUBREG_REG (in))))
      && (tem = gen_lowpart_common (GET_MODE (SUBREG_REG (in)), out)) != 0)
    in = SUBREG_REG (in), out = tem;
  else if (GET_CODE (out) == SUBREG
	   && (GET_MODE_SIZE (GET_MODE (out))
	       > GET_MODE_SIZE (GET_MODE (SUBREG_REG (out))))
	   && (tem = gen_lowpart_common (GET_MODE (SUBREG_REG (out)), in)) != 0)
    out = SUBREG_REG (out), in = tem;

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
	  || MEM_P (XEXP (in, 0)))
      && (REG_P (XEXP (in, 1))
	  || GET_CODE (XEXP (in, 1)) == SUBREG
	  || CONSTANT_P (XEXP (in, 1))
	  || MEM_P (XEXP (in, 1))))
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

      if (REG_P (XEXP (in, 1))
	  && REGNO (out) == REGNO (XEXP (in, 1)))
	tem = op0, op0 = op1, op1 = tem;

      if (op0 != XEXP (in, 0) || op1 != XEXP (in, 1))
	in = gen_rtx_PLUS (GET_MODE (in), op0, op1);

      insn = emit_insn (gen_rtx_SET (VOIDmode, out, in));
      code = recog_memoized (insn);

      if (code >= 0)
	{
	  extract_insn (insn);
	  /* We want constrain operands to treat this insn strictly in
	     its validity determination, i.e., the way it would after reload
	     has completed.  */
	  if (constrain_operands (1))
	    return insn;
	}

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

      if (CONSTANT_P (op1) || MEM_P (op1) || GET_CODE (op1) == SUBREG
	  || (REG_P (op1)
	      && REGNO (op1) >= FIRST_PSEUDO_REGISTER)
	  || (code != CODE_FOR_nothing
	      && ! ((*insn_data[code].operand[2].predicate)
		    (op1, insn_data[code].operand[2].mode))))
	tem = op0, op0 = op1, op1 = tem;

      gen_reload (out, op0, opnum);

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
	{
	  extract_insn (insn);
	  /* We want constrain operands to treat this insn strictly in
	     its validity determination, i.e., the way it would after reload
	     has completed.  */
	  if (constrain_operands (1))
	    return insn;
	}

      delete_insns_since (last);

      gen_reload (out, op1, opnum);
      insn = emit_insn (gen_add2_insn (out, op0));
      REG_NOTES (insn) = gen_rtx_EXPR_LIST (REG_EQUIV, in, REG_NOTES (insn));
    }

#ifdef SECONDARY_MEMORY_NEEDED
  /* If we need a memory location to do the move, do it that way.  */
  else if ((REG_P (in) || GET_CODE (in) == SUBREG)
	   && reg_or_subregno (in) < FIRST_PSEUDO_REGISTER
	   && (REG_P (out) || GET_CODE (out) == SUBREG)
	   && reg_or_subregno (out) < FIRST_PSEUDO_REGISTER
	   && SECONDARY_MEMORY_NEEDED (REGNO_REG_CLASS (reg_or_subregno (in)),
				       REGNO_REG_CLASS (reg_or_subregno (out)),
				       GET_MODE (out)))
    {
      /* Get the memory to use and rewrite both registers to its mode.  */
      rtx loc = get_secondary_mem (in, GET_MODE (out), opnum,
				   RELOAD_FOR_NONE);

      if (GET_MODE (loc) != GET_MODE (out))
	out = gen_rtx_REG (GET_MODE (loc), REGNO (out));

      if (GET_MODE (loc) != GET_MODE (in))
	in = gen_rtx_REG (GET_MODE (loc), REGNO (in));

      gen_reload (loc, in, opnum);
      gen_reload (out, loc, opnum);
    }
#endif

  /* If IN is a simple operand, use gen_move_insn.  */
  else if (OBJECT_P (in) || GET_CODE (in) == SUBREG)
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
inc_for_reload (rtx reloadreg, rtx in, rtx value, int inc_amount)
{
  /* REG or MEM to be copied and incremented.  */
  rtx incloc = XEXP (value, 0);
  /* Nonzero if increment after copying.  */
  int post = (GET_CODE (value) == POST_DEC || GET_CODE (value) == POST_INC);
  rtx last;
  rtx inc;
  rtx add_insn;
  int code;
  rtx store;
  rtx real_in = in == value ? XEXP (in, 0) : in;

  if (GET_CODE (value) == PRE_DEC || GET_CODE (value) == POST_DEC)
    inc_amount = -inc_amount;

  inc = GEN_INT (inc_amount);

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
	  extract_insn (add_insn);
	  if (constrain_operands (1))
	    {
	      /* If this is a pre-increment and we have incremented the value
		 where it lives, copy the incremented value to RELOADREG to
		 be used as an address.  */

	      if (! post)
		emit_insn (gen_move_insn (reloadreg, incloc));

	      return add_insn;
	    }
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

#ifdef AUTO_INC_DEC
static void
add_auto_inc_notes (rtx insn, rtx x)
{
  enum rtx_code code = GET_CODE (x);
  const char *fmt;
  int i, j;

  if (code == MEM && auto_inc_p (XEXP (x, 0)))
    {
      REG_NOTES (insn)
	= gen_rtx_EXPR_LIST (REG_INC, XEXP (XEXP (x, 0), 0), REG_NOTES (insn));
      return;
    }

  /* Scan all the operand sub-expressions.  */
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	add_auto_inc_notes (insn, XEXP (x, i));
      else if (fmt[i] == 'E')
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  add_auto_inc_notes (insn, XVECEXP (x, i, j));
    }
}
#endif

/* Copy EH notes from an insn to its reloads.  */
static void
copy_eh_notes (rtx insn, rtx x)
{
  rtx eh_note = find_reg_note (insn, REG_EH_REGION, NULL_RTX);
  if (eh_note)
    {
      for (; x != 0; x = NEXT_INSN (x))
	{
	  if (may_trap_p (PATTERN (x)))
	    REG_NOTES (x)
	      = gen_rtx_EXPR_LIST (REG_EH_REGION, XEXP (eh_note, 0),
				   REG_NOTES (x));
	}
    }
}

/* This is used by reload pass, that does emit some instructions after
   abnormal calls moving basic block end, but in fact it wants to emit
   them on the edge.  Looks for abnormal call edges, find backward the
   proper call and fix the damage.

   Similar handle instructions throwing exceptions internally.  */
void
fixup_abnormal_edges (void)
{
  bool inserted = false;
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      edge e;
      edge_iterator ei;

      /* Look for cases we are interested in - calls or instructions causing
         exceptions.  */
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  if (e->flags & EDGE_ABNORMAL_CALL)
	    break;
	  if ((e->flags & (EDGE_ABNORMAL | EDGE_EH))
	      == (EDGE_ABNORMAL | EDGE_EH))
	    break;
	}
      if (e && !CALL_P (BB_END (bb))
	  && !can_throw_internal (BB_END (bb)))
	{
	  rtx insn = BB_END (bb), stop = NEXT_INSN (BB_END (bb));
	  rtx next;
	  FOR_EACH_EDGE (e, ei, bb->succs)
	    if (e->flags & EDGE_FALLTHRU)
	      break;
	  /* Get past the new insns generated. Allow notes, as the insns may
	     be already deleted.  */
	  while ((NONJUMP_INSN_P (insn) || NOTE_P (insn))
		 && !can_throw_internal (insn)
		 && insn != BB_HEAD (bb))
	    insn = PREV_INSN (insn);
	  gcc_assert (CALL_P (insn) || can_throw_internal (insn));
	  BB_END (bb) = insn;
	  inserted = true;
	  insn = NEXT_INSN (insn);
	  while (insn && insn != stop)
	    {
	      next = NEXT_INSN (insn);
	      if (INSN_P (insn))
		{
	          delete_insn (insn);

		  /* Sometimes there's still the return value USE.
		     If it's placed after a trapping call (i.e. that
		     call is the last insn anyway), we have no fallthru
		     edge.  Simply delete this use and don't try to insert
		     on the non-existent edge.  */
		  if (GET_CODE (PATTERN (insn)) != USE)
		    {
		      /* We're not deleting it, we're moving it.  */
		      INSN_DELETED_P (insn) = 0;
		      PREV_INSN (insn) = NULL_RTX;
		      NEXT_INSN (insn) = NULL_RTX;

		      insert_insn_on_edge (insn, e);
		    }
		}
	      insn = next;
	    }
	}
    }
  /* We've possibly turned single trapping insn into multiple ones.  */
  if (flag_non_call_exceptions)
    {
      sbitmap blocks;
      blocks = sbitmap_alloc (last_basic_block);
      sbitmap_ones (blocks);
      find_many_sub_basic_blocks (blocks);
    }
  if (inserted)
    commit_edge_insertions ();
}
