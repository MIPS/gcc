/* Communication between reload.c, reload1.c and pre-reload.c.
   Copyright (C) 2002 Free Software Foundation, Inc.
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


  /* Compare two RTX's.  */
#define MATCHES(x, y) \
 (x == y || (x != 0 && (GET_CODE (x) == REG				\
			? GET_CODE (y) == REG && REGNO (x) == REGNO (y)	\
			: rtx_equal_p (x, y) && ! side_effects_p (x))))

  /* Indicates if two reloads purposes are for similar enough things that we
     can merge their reloads.  */
#define MERGABLE_RELOADS(when1, when2, op1, op2) \
  ((when1) == RELOAD_OTHER || (when2) == RELOAD_OTHER	\
   || ((when1) == (when2) && (op1) == (op2))		\
   || ((when1) == RELOAD_FOR_INPUT && (when2) == RELOAD_FOR_INPUT) \
   || ((when1) == RELOAD_FOR_OPERAND_ADDRESS		\
       && (when2) == RELOAD_FOR_OPERAND_ADDRESS)	\
   || ((when1) == RELOAD_FOR_OTHER_ADDRESS		\
       && (when2) == RELOAD_FOR_OTHER_ADDRESS))

  /* Nonzero if these two reload purposes produce RELOAD_OTHER when merged.  */
#define MERGE_TO_OTHER(when1, when2, op1, op2) \
  ((when1) != (when2)					\
   || ! ((op1) == (op2)					\
	 || (when1) == RELOAD_FOR_INPUT			\
	 || (when1) == RELOAD_FOR_OPERAND_ADDRESS	\
	 || (when1) == RELOAD_FOR_OTHER_ADDRESS))

  /* If we are going to reload an address, compute the reload type to
     use.  */
#define ADDR_TYPE(type)					\
  ((type) == RELOAD_FOR_INPUT_ADDRESS			\
   ? RELOAD_FOR_INPADDR_ADDRESS				\
   : ((type) == RELOAD_FOR_OUTPUT_ADDRESS		\
      ? RELOAD_FOR_OUTADDR_ADDRESS			\
      : (type)))


/* Used to track what is modified by an operand.  */
struct decomposition
{
  int reg_flag;		/* Nonzero if referencing a register.  */
  int safe;		/* Nonzero if this can't conflict with anything.  */
  rtx base;		/* Base address for MEM.  */
  HOST_WIDE_INT start;	/* Starting offset or register number.  */
  HOST_WIDE_INT end;	/* Ending offset or register number.  */
};

/* Each replacement is recorded with a structure like this.  */
struct replacement
{
  rtx *where;			/* Location to store in */
  rtx *subreg_loc;		/* Location of SUBREG if WHERE is inside
				   a SUBREG; 0 otherwise.  */
  int what;			/* which reload this is for */
  enum machine_mode mode;	/* mode it must have */
};



/* Replacing reloads.

   If `replace_reloads' is nonzero, then as each reload is recorded
   an entry is made for it in the table `replacements'.
   Then later `subst_reloads' can look through that table and
   perform all the replacements needed.  */

/* Nonzero means record the places to replace.  */
extern int replace_reloads;

extern struct replacement replacements[MAX_RECOG_OPERANDS *
				      ((MAX_REGS_PER_ADDRESS * 2) + 1)];

/* Number of replacements currently recorded.  */
extern int n_replacements;

/* These arrays are filled by emit_reload_insns and its subroutines.  */
extern rtx input_reload_insns[MAX_RECOG_OPERANDS];
extern rtx other_input_address_reload_insns ;
extern rtx other_input_reload_insns;
extern rtx input_address_reload_insns[MAX_RECOG_OPERANDS];
extern rtx inpaddr_address_reload_insns[MAX_RECOG_OPERANDS];
extern rtx output_reload_insns[MAX_RECOG_OPERANDS];
extern rtx output_address_reload_insns[MAX_RECOG_OPERANDS];
extern rtx outaddr_address_reload_insns[MAX_RECOG_OPERANDS];
extern rtx operand_reload_insns;
extern rtx other_operand_reload_insns;
extern rtx other_output_reload_insns[MAX_RECOG_OPERANDS];

/* The instruction we are doing reloads for;
   so we can test whether a register dies in it.  */
extern rtx this_insn;

/* Nonzero if this instruction is a user-specified asm with operands.  */
extern int this_insn_is_asm;

/* On return from push_reload, holds the reload-number for the OUT
   operand, which can be different for that from the input operand.  */
extern int output_reloadnum;



extern void push_replacement (rtx *, int , enum machine_mode);

extern int reload_inner_reg_of_subreg PARAMS ((rtx, enum machine_mode));
extern int find_pre_reloads (rtx, int);
extern enum reg_class find_valid_class PARAMS ((enum machine_mode, int));
extern void copy_eh_notes		PARAMS ((rtx, rtx));
extern int alternative_allows_memconst PARAMS ((const char *, int));
extern void subst_pre_reloads (rtx);
extern void emit_pre_reload_insns (rtx);



