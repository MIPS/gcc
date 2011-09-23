/* Local Register Allocator (LRA) intercommunication header file.
   Copyright (C) 2010, 2011
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
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "lra.h"
#include "bitmap.h"
#include "insn-attr.h"

/* The parameter used to prevent infinite reloading for an insn.  Each
   insn operands might require a reload and, if it is a memory, its
   base and index registers might require a reload too.  */
#define LRA_MAX_INSN_RELOADS (MAX_RECOG_OPERANDS * 3)

/* Return the hard register which given pseudo REGNO assigned to.
   Negative value means that the register got memory or we don't know
   allocation yet.  */
static inline int
lra_get_regno_hard_regno (int regno)
{
  resize_reg_info ();
  return reg_renumber[regno];
}

/* Return the allocno reg class of REGNO.  If it is a reload pseudo,
   the pseudo should finally get hard register of the allocno
   class.  */
static inline enum reg_class
lra_get_allocno_class (int regno)
{
  resize_reg_info ();
  return reg_allocno_class (regno);
}

typedef struct lra_live_range *lra_live_range_t;

/* The structure describes program points where a given pseudo lives.
   The live ranges can be used to find conflicts with other pseudos.
   If the live ranges of two pseudos are intersected, the pseudos are
   in conflict.  */
struct lra_live_range
{
  /* Pseudo regno whose live range is described by given
     structure.  */
  int regno;
  /* Program point range.  */
  int start, finish;
  /* Next structure describing program points where the pseudo
     lives.  */
  lra_live_range_t next;
  /* Pointer to structures with the same start/finish.  */
  lra_live_range_t start_next, finish_next;
};

typedef struct lra_copy *lra_copy_t;

/* Copy between pseudos which affects assigning hard registes. */
struct lra_copy
{
  /* True if regno1 is the destination of the copy.  */
  bool regno1_dest_p;
  /* Execution frequency of the copy.  */
  int freq;
  /* Pseudos connected by the copy.  REGNO1 < REGNO2.  */
  int regno1, regno2;
  /* Next copy with correspondingly REGNO1 and REGNO2.  */
  lra_copy_t regno1_next, regno2_next;
};

/* Common info about a register.  */
struct lra_reg
{
  /* Bitmap of UIDs of insns (including debug insns) referring the
     reg.  */
  bitmap_head insn_bitmap;
  /* The following fields are defined only for pseudos.  */
  /* Hard registers with which the pseudo conflicts.  */
  HARD_REG_SET conflict_hard_regs;
  /* We assign hard registers to reload pseudos which can occur in few
     places.  So two hard register preferences are enough for them.
     The following fields define the preferred hard registers.  If
     there are no such hard registers the first field value is
     negative.  If there is only one preferred hard register, the 2nd
     field is negative.  */
  int preferred_hard_regno1, preferred_hard_regno2;
  /* Profits to use the corresponding preferred hard registers.  If
     the both hard registers defined, the first hard register has not
     less profit than the second one.  */
  int preferred_hard_regno_profit1, preferred_hard_regno_profit2;
#ifdef STACK_REGS
  /* True if the pseudo should not be assigned to a stack register.  */
  bool no_stack_p;
#endif
  /* Number of references and execution frequencies of the register in
     *non-debug* insns.  */
  int nrefs, freq;
  int last_reload;
  /* Regno to undo the inheritance.  It can be non-zero only between
     couple of inheritance and undo unheritance passes.  */
  int restore_regno;
  /* Value holding by register.  If the pseudos have the same value
     they do not conflict.  */
  int val;
  /* These members are set up in lra-lives.c and updated in
     lra-coalesce.c.  */
  /* The biggest size mode in which each pseudo reg is referred in
     whole function (possibly via subreg).  */
  enum machine_mode biggest_mode;
  /* Live ranges of the pseudo.  */
  lra_live_range_t live_ranges;
  /* This member is set up in lra-lives.c for subsequent
     assignments.  */
  lra_copy_t copies;
};

/* Reference to the common info about each register.  */
extern struct lra_reg *lra_reg_info;

/* Static info about each insn operand (common for all insns with the
   same ICODE).  Warning: if the structure is changed, the initializer
   for debug_operand_data in lra.c should be changed too.  */
struct lra_operand_data
{
  /* The machine description constraint string.  */
  const char *constraint;
  /* It is only from machine description (which is different from
     recog_data.operand_mode) and can be of VOIDmode.  */
  ENUM_BITFIELD(machine_mode) mode : 16;
  /* The type of the operand (in/out/inout).  */
  ENUM_BITFIELD (op_type) type : 8;
  /* Through if accessed through STRICT_LOW.  */
  unsigned int strict_low : 1;
  /* True if the operand is an operator.  */
  unsigned int is_operator : 1;
  /* True if there is an early clobber alternative for this operand.
     This field is set up every time when operand_alternative is set
     up.  */
  unsigned int early_clobber : 1;
};

/* Info about register in an insn.  */
struct lra_insn_reg
{
  /* The biggest mode through which the insn refers to the register
     (remember the register can be accessed through a subreg in the
     insn).  */
  ENUM_BITFIELD(machine_mode) biggest_mode : 16;
  /* The type of the operand which is the register.  */
  ENUM_BITFIELD (op_type) type : 8;
  /* True of subreg is just a part of the register.  */
  unsigned int subreg_p : 1;
  /* True if there is an early clobber alternative for this
     operand.  */
  unsigned int early_clobber : 1;
  int regno;
  /* Next reg info of the same insn.  */
  struct lra_insn_reg *next;
};

/* Static part (common info for insns with the same ICODE) of LRA
   internal insn info.  It exists in at most one exemplar for each
   non-negative ICODE.  Warning: if the structure is changed, the
   initializer for debug_insn_static_data in lra.c should be changed
   too.  */
struct lra_static_insn_data
{
  /* Static info about each insn operand.  */
  struct lra_operand_data *operand;
  /* Each duplication refers to the number of the corresponding
     operand which is duplicated.  */
  int *dup_num;
  /* The number of operand marked as commutative, -1 otherwise.  */
  int commutative;
  /* Number of operands, duplications, and alternatives of the
     insn.  */
  char n_operands;
  char n_dups;
  char n_alternatives;
  /* Insns in machine description (or clobbers in asm) may contain
     explicit hard regs which are not operands.  The following list
     describes such hard registers.  */
  struct lra_insn_reg *hard_regs;
  /* Array [n_alternatives][n_operand] of static constraint info for
     given operand in given alternative.  This info can be changed if
     the target reg info is changed.  */
  struct operand_alternative *operand_alternative;
};

/* LRA internal info about an insn (LRA internal insn
   representation).  */
struct lra_insn_recog_data
{
  int icode; /* The insn code.  */
  rtx insn; /* The insn itself.  */
  /* Common data for insns with the same ICODE.  */
  struct lra_static_insn_data *insn_static_data;
  rtx **operand_loc; /* The operand locations, NULL if no operands.  */
  rtx **dup_loc; /* The dup locations, NULL if no dups.  */
  /* Number of hard registers implicitly used in given call insn.  The
     value can be NULL or points to the hard registers ending with
     negative value.  */
  int *arg_hard_regs;
#ifdef HAVE_ATTR_enabled
  /* Alternative enabled for the insn.  NULL for debug insns.  */
  bool *alternative_enabled_p;
#endif
  /* The alternative should be used for the insn, -1 if invalid, or we
     should try to use any alternative, or the insn is a debug
     insns.  */
  int used_insn_alternative;
  struct lra_insn_reg *regs;  /* Always NULL for a debug insn.  */
};

typedef struct lra_insn_recog_data *lra_insn_recog_data_t;

/* lra.c: */

extern FILE *lra_dump_file;

extern HARD_REG_SET lra_no_alloc_regs;

extern bitmap_head lra_constraint_insn_stack_bitmap;
extern VEC (rtx, heap) *lra_constraint_insn_stack;

extern int lra_curr_reload_num;

extern void lra_push_insn (rtx);
extern void lra_push_insn_by_uid (unsigned int);
extern void lra_push_insn_and_update_insn_regno_info (rtx);

extern rtx lra_create_new_reg_with_unique_value (enum machine_mode, rtx,
						 enum reg_class, const char *);
extern rtx lra_create_new_reg (enum machine_mode, rtx, enum reg_class,
			       const char *);
extern void lra_set_regno_unique_value (int);
extern void lra_invalidate_insn_data (rtx);
extern void lra_set_insn_deleted (rtx);
extern void lra_delete_dead_insn (rtx);
extern void lra_emit_add (rtx, rtx, rtx);
extern void lra_emit_move (rtx, rtx);
extern void lra_update_dups (lra_insn_recog_data_t, signed char *);

extern void lra_process_new_insns (rtx, rtx, rtx, const char *);

extern lra_insn_recog_data_t lra_get_insn_recog_data (rtx);
extern lra_insn_recog_data_t lra_update_insn_recog_data (rtx);
extern void lra_set_used_insn_alternative (rtx, int);
extern void lra_set_used_insn_alternative_by_uid (int, int);

extern void lra_invalidate_insn_regno_info (rtx);
extern void lra_update_insn_regno_info (rtx);
extern struct lra_insn_reg *lra_get_insn_regs (int);

extern void lra_expand_reg_info (void);
extern void lra_free_copies (void);
extern void lra_create_copy (int, int, int);

extern bool lra_former_scratch_p (int);
extern bool lra_former_scratch_operand_p (rtx, int);

extern int lra_constraint_new_regno_start;
extern bitmap_head lra_inheritance_pseudos;
extern int lra_constraint_new_insn_uid_start;

/* lra-constraints.c: */

extern bitmap_head lra_matched_pseudos;
extern bitmap_head lra_bound_pseudos;

extern rtx lra_secondary_memory[NUM_MACHINE_MODES];

extern int lra_constraint_offset (int, enum machine_mode);

extern int lra_constraint_iter;
extern int lra_inheritance_iter;
extern int lra_undo_inheritance_iter;
extern bool lra_constraints (bool);
extern void lra_contraints_init (void);
extern void lra_contraints_finish (void);
extern void lra_inheritance (void);
extern bool lra_undo_inheritance (void);

/* lra-lives.c: */

extern int lra_live_max_point;
extern lra_live_range_t *lra_start_point_ranges, *lra_finish_point_ranges;
extern int *lra_point_freq;

extern int lra_hard_reg_usage[FIRST_PSEUDO_REGISTER];

extern int lra_live_range_iter;
extern void lra_create_live_ranges (bool);
extern lra_live_range_t lra_copy_live_range_list (lra_live_range_t);
extern lra_live_range_t lra_merge_live_ranges (lra_live_range_t,
					       lra_live_range_t);
extern bool lra_intersected_live_ranges_p (lra_live_range_t,
					   lra_live_range_t);
extern bool lra_live_range_in_p (lra_live_range_t, lra_live_range_t);
extern void lra_print_live_range_list (FILE *, lra_live_range_t);
extern void lra_debug_live_range_list (lra_live_range_t);
extern void lra_debug_pseudo_live_ranges (int);
extern void lra_debug_live_ranges (void);
extern void lra_clear_live_ranges (void);
extern void lra_live_ranges_init (void);
extern void lra_live_ranges_finish (void);
extern void lra_setup_reload_pseudo_preferenced_hard_reg (int, int, int);

/* lra-assigns.c: */

extern void lra_setup_reg_renumber (int, int, bool);
extern bool lra_assign (void);


/* lra-coalesce.c: */

extern int lra_coalesce_iter;
extern void lra_coalesce (void);

/* lra-saves.c: */

extern bool lra_save_restore (void);

/* lra-spills.c:  */

extern bool lra_spill (void);
extern void lra_hard_reg_substitution (void);


/* lra-elimination.c: */

extern void lra_debug_elim_table (void);
extern int lra_get_elimation_hard_regno (int);
extern rtx lra_eliminate_regs_1 (rtx, enum machine_mode, bool, bool, bool);
extern void lra_eliminate (bool);

extern void lra_eliminate_reg_if_possible (rtx *);



/* The function returns TRUE if at least one hard register from ones
   starting with HARD_REGNO and containing value of MODE are in set
   HARD_REGSET.  */
static inline bool
lra_hard_reg_set_intersection_p (int hard_regno, enum machine_mode mode,
				 HARD_REG_SET hard_regset)
{
  int i;

  gcc_assert (hard_regno >= 0);
  for (i = hard_regno_nregs[hard_regno][mode] - 1; i >= 0; i--)
    if (TEST_HARD_REG_BIT (hard_regset, hard_regno + i))
      return true;
  return false;
}

/* Return hard regno and offset of (sub-)register X through HARD_REGNO
   and OFFSET.  If it is not (sub-)register or the hard register is
   unknown, then return -1 and 0 correspondingly.  */
static inline void
lra_get_hard_regno_and_offset (rtx x, int *hard_regno, int *offset)
{
  rtx reg;

  *hard_regno = *offset = -1;
  reg = x;
  if (GET_CODE (x) == SUBREG)
    reg = SUBREG_REG (x);
  if (! REG_P (reg))
    return;
  if ((*hard_regno = REGNO (reg)) >= FIRST_PSEUDO_REGISTER)
    *hard_regno = lra_get_regno_hard_regno (*hard_regno);
  if (*hard_regno < 0)
    return;
  *offset = 0;
  if (GET_CODE (x) == SUBREG)
    *offset += subreg_regno_offset (*hard_regno, GET_MODE (reg),
				   SUBREG_BYTE (x),  GET_MODE (x));
}

/* Add hard registers starting with HARD_REGNO holding value of MODE
   to the set S.  */
static inline void
lra_add_hard_reg_set (int hard_regno, enum machine_mode mode, HARD_REG_SET *s)
{
  int i;

  for (i = hard_regno_nregs[hard_regno][mode] - 1; i >= 0; i--)
    SET_HARD_REG_BIT (*s, hard_regno + i);
}

/* Update insn operands which are duplication of NOP operand.  The
   insn is represented by its LRA internal representation ID.  */
static inline void
lra_update_dup (lra_insn_recog_data_t id, int nop)
{
  int i;
  struct lra_static_insn_data *static_id = id->insn_static_data;

  for (i = 0; i < static_id->n_dups; i++)
    if (static_id->dup_num[i] == nop)
      *id->dup_loc[i] = *id->operand_loc[nop];
}
