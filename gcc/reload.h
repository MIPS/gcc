/* Communication between reload.c and reload1.c.
   Copyright (C) 1987, 1991, 1992, 1993, 1994, 1995, 1997, 1998,
   1999, 2000, 2001, 2003, 2004 Free Software Foundation, Inc.

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


/* If secondary reloads are the same for inputs and outputs, define those
   macros here.  */

#ifdef SECONDARY_RELOAD_CLASS
#define SECONDARY_INPUT_RELOAD_CLASS(CLASS, MODE, X) \
  SECONDARY_RELOAD_CLASS (CLASS, MODE, X)
#define SECONDARY_OUTPUT_RELOAD_CLASS(CLASS, MODE, X) \
  SECONDARY_RELOAD_CLASS (CLASS, MODE, X)
#endif

/* If either macro is defined, show that we need secondary reloads.  */
#if defined(SECONDARY_INPUT_RELOAD_CLASS) || defined(SECONDARY_OUTPUT_RELOAD_CLASS)
#define HAVE_SECONDARY_RELOADS
#endif

/* If MEMORY_MOVE_COST isn't defined, give it a default here.  */
#ifndef MEMORY_MOVE_COST
#ifdef HAVE_SECONDARY_RELOADS
#define MEMORY_MOVE_COST(MODE,CLASS,IN) \
  (4 + memory_move_secondary_cost ((MODE), (CLASS), (IN)))
#else
#define MEMORY_MOVE_COST(MODE,CLASS,IN) 4
#endif
#endif
extern int memory_move_secondary_cost (enum machine_mode, enum reg_class, int);

/* Maximum number of reloads we can need.  */
#define MAX_RELOADS (2 * MAX_RECOG_OPERANDS * (MAX_REGS_PER_ADDRESS + 1))
/* Likewise, the maximum number of replacements we can need.  */
#define MAX_REPLACEMENTS (MAX_RECOG_OPERANDS * ((MAX_REGS_PER_ADDRESS * 2) + 1))

/* Each replacement is recorded with a structure like this.  */
struct replacement
{
  rtx *where;			/* Location to store in */
  rtx *subreg_loc;		/* Location of SUBREG if WHERE is inside
				   a SUBREG; 0 otherwise.  */
  int what;			/* which reload this is for */
  enum machine_mode mode;	/* mode it must have */
  rtx contents;			/* Data stored inside it.  */
};

extern struct replacement replacements[];

/* Number of replacements currently recorded.  */
extern int n_replacements;
/* A dummy rtx that is temporarily substituted into the insn's
   replacements.  */
extern struct rtx_def dummy_replacement_rtx;

/* Encode the usage of a reload.  No longer used, but kept around
   to keep diff sizes small.  */

enum reload_type
{
  RELOAD_FOR_NONE
};

enum rlinsn_type
{
  RLI_INSN,
  RLI_INPUTRELOAD,
  RLI_OUTPUTRELOAD,
  RLI_INPUTREG,
  RLI_OUTPUTREG
};

enum rlinsn_status
{
  RLIS_IGNORED,
  RLIS_NOT_SCHEDULED,
  RLIS_SCHEDULED,
  RLIS_EMITTED
};

/* Describe lifetime information about a register that occurs within a
   reloaded insn.  This structure is used both to describe hard regs (or
   pseudos allocated to hard regs) that occur within the insn, and to
   describe reload regs.  */
struct reload_reg_use
{
  /* The regno of this register.  Unset for not-yet-allocated reloads.  */
  int regno;
  /* After the reload insn positions have been determined, these two fields
     describe the lifetime of the register.  */
  char birth, death;
  /* Nonzero if this is subject to an earlyclobber.  */
  unsigned int earlyclobber:1;
  /* Determines whether this describes a hardreg or an allocated pseudo.  */
  unsigned int hardreg:1;
  /* If this belongs to a reload, ALLOCATED says whether the reload has
     a register.  Optional reloads can remain unallocated until the end.  */
  unsigned int allocated:1;
  /* Nonzero for registers that are live until the end of the insn due to
     being used for inheritance.  */
  unsigned int live_until_end:1;
  unsigned int ignored:1;
};

/* When an insn is reloaded, we allocate one structure for each of the insns
   that reload will generate, as well as another one for the reloaded insn
   itself.  We also make these structures in conjunction with reload_reg_use
   structures to describe register assignments (especially to keep track of
   dependencies through the FEEDS array).  */
struct reload_insn
{
  /* The type of this structure, it says what is being described.  */
  enum rlinsn_type type;
  /* Status, used in compute_reload_order while scheduling these insns.  */
  enum rlinsn_status status;

  /* Describe the position of this insn within the chain we will eventually
     generate.  The first insn will have order 0.  */
  short order;
  /* All reload_insn structures are chained in descending order.  This field
     contains the index of the previous reload_insn.  It is an index into the
     array kept in the corresponding insn_chain structure.  */
  short prev_order;

  /* Information about dependencies between the reload insns.  */
  int feeds_space;
  struct reload_insn **feeds;

  unsigned char n_feeds;
  unsigned char n_feeds_remaining;

  /* Depending on TYPE, this field is either
     - unset (for RLI_INSN)
     - the register number (for RLI_INPUTREG/RLI_OUTPUTREG)
     - the reload number (for RLI_INPUTRELOAD/RLI_OUTPUTRELOAD).  */
  unsigned int nr:14;
  /* Nonzero for a RLI_OUTPUTRELOAD type structure if we do not actually need
     to perform the output reload (i.e. because the destination is unused
     afterwards.  */
  unsigned int unnecessary:1;
  /* A temporary marker that shows that given the current allocation/schedule
     of reloads, this insn is not necessary.  */
  unsigned int ignored:1;
};

#ifdef GCC_INSN_CODES_H
/* Each reload is recorded with a structure like this.  */
struct reload
{
  /* The value to reload from */
  rtx in;
  /* Where to store reload-reg afterward if nec (often the same as
     reload_in)  */
  rtx out;

  rtx override_in, override_out;

  /* The class of registers to reload into.  */
  enum reg_class class;

  /* The mode this operand should have when reloaded, on input.  */
  enum machine_mode inmode;
  /* The mode this operand should have when reloaded, on output.  */
  enum machine_mode outmode;

  /* The mode of the reload register.  */
  enum machine_mode mode;

  /* the largest number of registers this reload will require.  */
  int nregs;

  /* Positive amount to increment or decrement by if
     reload_in is a PRE_DEC, PRE_INC, POST_DEC, POST_INC.
     Otherwise zero.  */
  int inc;
  /* A reg for which reload_in is the equivalent.
     If reload_in is a symbol_ref which came from
     reg_equiv_constant, then this is the pseudo
     which has that symbol_ref as equivalent.  */
  rtx in_reg;
  rtx out_reg;

  /* This is the register to reload into.  If it is zero when `find_reloads'
     returns, you must find a suitable register in the class specified by
     reload_reg_class, and store here an rtx for that register with mode from
     reload_inmode or reload_outmode.  */
  rtx reg_rtx;

  struct reload_reg_use reginfo;

  /* The operand number being reloaded.  This is used to group related reloads
     and need not always be equal to the actual operand number in the insn,
     though it current will be; for in-out operands, it is one of the two
     operand numbers.  */
  int opnum;

  /* Gives the reload number of a secondary input reload, when needed;
     otherwise -1.  */
  int secondary_in_reload;
  /* Gives the reload number of a secondary output reload, when needed;
     otherwise -1.  */
  int secondary_out_reload;
  /* If a secondary input reload is required, gives the INSN_CODE that uses the
     secondary reload as a scratch register, or CODE_FOR_nothing if the
     secondary reload register is to be an intermediate register.  */
  enum insn_code secondary_in_icode;
  /* Likewise, for a secondary output reload.  */
  enum insn_code secondary_out_icode;

  /* Classifies reload as needed either for addressing an input reload,
     addressing an output, for addressing a non-reloaded mem ref, or for
     unspecified purposes (i.e., more than one of the above).  */
  enum reload_type when_needed;

  /* Nonzero for an optional reload.  Optional reloads are ignored unless the
     value is already sitting in a register.  */
  unsigned int optional:1;
  /* Nonzero if this is a secondary register for one or more reloads.  */
  unsigned int secondary_p:1;
  /* Nonzero for an optional reload that has been enabled.  */
  unsigned int enabled:1;
  /* Helper flags used during copy_reloads.  */
  unsigned int scanned_input:1;
  unsigned int scanned_output:1;
  unsigned int earlyclobber:1;
  unsigned int accounted_in:1;
  unsigned int accounted_out:1;  
};

extern struct reload rld[MAX_RELOADS];
#endif

extern GTY (()) struct varray_head_tag *reg_equiv_memory_loc_varray;
extern rtx *reg_equiv_constant;
extern rtx *reg_equiv_memory_loc;
extern rtx *reg_equiv_address;
extern rtx *reg_equiv_mem;

/* Element N is the list of insns that initialized reg N from its equivalent
   constant or memory slot.  */
extern GTY((length("reg_equiv_init_size"))) rtx *reg_equiv_init;

/* The size of the previous array, for GC purposes.  */
extern GTY(()) int reg_equiv_init_size;

/* All the "earlyclobber" operands of the current insn
   are recorded here.  */
extern int n_earlyclobbers;
extern rtx reload_earlyclobbers[MAX_RECOG_OPERANDS];

/* Save the number of operands.  */
extern int reload_n_operands;

/* First uid used by insns created by reload in this function.
   Used in find_equiv_reg.  */
extern int reload_first_uid;

/* Nonzero if indirect addressing is supported when the innermost MEM is
   of the form (MEM (SYMBOL_REF sym)).  It is assumed that the level to
   which these are valid is the same as spill_indirect_levels, above.  */

extern char indirect_symref_ok;

/* Nonzero if an address (plus (reg frame_pointer) (reg ...)) is valid.  */
extern char double_reg_address_ok;

extern int num_not_at_initial_offset;

#if defined SET_HARD_REG_BIT && defined CLEAR_REG_SET
/* This structure describes instructions which are relevant for reload.
   Apart from all regular insns, this also includes CODE_LABELs, since they
   must be examined for register elimination.  */
struct insn_chain
{
  /* Links to the neighbor instructions.  */
  struct insn_chain *next, *prev;

  /* Link through a chains set up by calculate_needs_all_insns, containing
     all insns that need reloading.  */
  struct insn_chain *next_need_reload;

  /* The basic block this insn is in.  */
  int block;
  /* The rtx of the insn, and a scratch copy of that insn to be used by
     find_reloads during the first pass.  */
  rtx insn, insn_copy;

  /* Register life information: record all live hard registers, and all
     live pseudos that have a hard register
     This information is recorded for the point immediately before the insn
     (in live_before), and for the point within the insn at which all
     outputs have just been written to (in live_after).  */
  regset_head live_before;
  regset_head live_after;

  /* find_reloads can make substitutions that obfuscate the fact that a
     pseudo is used or set in the insn.  In such cases, we set the
     corresponding bit in one of these two regsets.  */
  regset_head unreloaded_uses;
  regset_head unreloaded_sets;

  /* Copies of the global variables computed by find_reloads.  */
  struct reload *rld;
  int n_reloads;

  struct reload_insn *rli;
  int last_rlinsn;
  int n_input_regs;
  int n_output_regs;
  struct reload_reg_use *reg_usage;
  HARD_REG_SET hard_live_across;
  HARD_REG_SET pseudo_hard_live_across;

  HARD_REG_SET set_regs, used_regs;

  /* Indicates which registers have already been used for spills.  */
  HARD_REG_SET used_spill_regs;

  /* If any registers are live across this insn due to reload inheritance,
     they are marked here.  */
  HARD_REG_SET inherited_live;

  /* Nonzero if find_reloads needs to be run during reload_as_needed to
     perform modifications on any operands.  */
  unsigned int need_operand_change:1;
  /* Nonzero if eliminate_regs_in_insn said it requires eliminations.  */
  unsigned int need_elim:1;
  /* Nonzero if this insn was inserted by perform_caller_saves.  */
  unsigned int is_caller_save_insn:1;
  /* Nonzero if this insn will be deleted and should therefore be ignored.  */
  unsigned int will_be_deleted:1;
};

/* A chain of insn_chain structures to describe all non-note insns in
   a function.  */
extern struct insn_chain *reload_insn_chain;

/* Allocate a new insn_chain structure.  */
extern struct insn_chain *new_insn_chain (void);

extern void compute_use_by_pseudos (HARD_REG_SET *, regset);

/* Search the body of INSN for values that need reloading and record them
   with push_reload.  REPLACE nonzero means record also where the values occur
   so that subst_reloads can be used.  */
extern int find_reloads (struct insn_chain *, rtx, int, int);

#endif

/* Functions from reload.c:  */

/* Return a memory location that will be used to copy X in mode MODE.
   If we haven't already made a location for this mode in this insn,
   call find_reloads_address on the location being returned.  */
extern rtx get_secondary_mem (rtx, enum machine_mode, int, enum reload_type);

/* Clear any secondary memory locations we've made.  */
extern void clear_secondary_mem (void);

/* Transfer all replacements that used to be in reload FROM to be in
   reload TO.  */
extern void transfer_replacements (int, int);

/* Substitute dummy_replacement_rtx into all replacements.  */
extern void subst_dummy PARAMS ((void));
extern void undo_subst_dummy PARAMS ((void));
/* Find the number of a replacement by its location.  */
extern int replacement_nr PARAMS ((rtx *));

/* Like rtx_equal_p except that it allows a REG and a SUBREG to match
   if they are the same hard reg, and has special hacks for
   autoincrement and autodecrement.  */
extern int operands_match_p (rtx, rtx);

/* Return 1 if altering OP will not modify the value of CLOBBER.  */
extern int safe_from_earlyclobber (rtx, rtx);

/* Compute the sum of X and Y, making canonicalizations assumed in an
   address, namely: sum constant integers, surround the sum of two
   constants with a CONST, put the constant as the second operand, and
   group the constant on the outermost sum.  */
extern rtx form_sum (rtx, rtx);

/* Substitute into the current INSN the registers into which we have reloaded
   the things that need reloading.  */
extern void subst_reloads (rtx);

/* Make a copy of any replacements being done into X and move those copies
   to locations in Y, a copy of X.  We only look at the highest level of
   the RTL.  */
extern void copy_replacements (rtx, rtx);

/* Change any replacements being done to *X to be done to *Y */
extern void move_replacements (rtx *x, rtx *y);

/* If LOC was scheduled to be replaced by something, return the replacement.
   Otherwise, return *LOC.  */
extern rtx find_replacement (rtx *);

/* Nonzero if modifying X will affect IN.  */
extern int reg_overlap_mentioned_for_reload_p (rtx, rtx);

/* Check the insns before INSN to see if there is a suitable register
   containing the same value as GOAL.  */
extern rtx find_equiv_reg (rtx, rtx, enum reg_class, int, short *,
			   int, enum machine_mode);

/* Return 1 if register REGNO is the subject of a clobber in insn INSN.  */
extern int regno_clobbered_p (unsigned int, rtx, enum machine_mode, int);

/* Return 1 if X is an operand of an insn that is being earlyclobbered.  */
extern int earlyclobber_operand_p (rtx);

/* Record one reload that needs to be performed.  */
extern int push_reload (rtx, rtx, rtx *, rtx *, enum reg_class,
			enum machine_mode, enum machine_mode,
			int, int, int, enum reload_type);

/* Functions in postreload.c:  */
extern void reload_cse_regs (rtx);

/* Functions in reload1.c:  */

/* Initialize the reload pass once per compilation.  */
extern void init_reload (void);

/* The reload pass itself.  */
extern int reload (rtx, int);

/* Mark the slots in regs_ever_live for the hard regs
   used by pseudo-reg number REGNO.  */
extern void mark_home_live (int);

/* Scan X and replace any eliminable registers (such as fp) with a
   replacement (such as sp), plus an offset.  */
extern rtx eliminate_regs (rtx, enum machine_mode, rtx);

/* Deallocate the reload register used by reload number R.  */
extern void deallocate_reload_reg (int r);

/* Functions in caller-save.c:  */

/* Initialize for caller-save.  */
extern void init_caller_save (void);

/* Initialize save areas by showing that we haven't allocated any yet.  */
extern void init_save_areas (void);

/* Allocate save areas for any hard registers that might need saving.  */
extern void setup_save_areas (void);

/* Find the places where hard regs are live across calls and save them.  */
extern void save_call_clobbered_regs (void);

/* Replace (subreg (reg)) with the appropriate (reg) for any operands.  */
extern void cleanup_subreg_operands (rtx);

/* Debugging support.  */
extern void debug_reload_to_stream (FILE *);
extern void debug_reload (void);

/* Compute the actual register we should reload to, in case we're
   reloading to/from a register that is wider than a word.  */
extern rtx reload_adjust_reg_for_mode (rtx, enum machine_mode);
