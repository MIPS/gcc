/* HSAIL and BRIG related macros and definitions.
   Copyright (C) 2013 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef HSA_H
#define HSA_H

#include "hsa-brig-format.h"
#include "is-a.h"
#include "tree.h"
#include "vec.h"
#include "hash-table.h"
#include "basic-block.h"

#define HSA_C_REGISTER_COUNT 8
#define HSA_S_REGISTER_COUNT 128
#define HSA_D_REGISTER_COUNT 64
#define HSA_Q_REGISTER_COUNT 32

struct hsa_insn_basic;
typedef hsa_insn_basic *hsa_insn_basic_p;

/* Class representing an input argument, output argument (result) or a
   variable, that will eventually end up being a symbol directive.  */
struct hsa_symbol
{
  /* Pointer to the original tree, which is PARM_DECL for input parameters and
     RESULT_DECL for the output parameters.  */
  tree decl;

  /* Name of the symbol, that will be written into output and dumps.  Can be
     NULL, see name_number below.*/
  const char *name;

  /* If name is NULL, artificial name will be formed from the segment name and
     this number.  */
  int name_number;

  /* Once written, this is the offset of the associated symbol directive.  Zero
     means the symbol has not been written yet.  */
  unsigned offset;

  /* HSA type of the parameter.  */
  BrigType16_t type;

  /* The HSA segment this will eventually end up in.  */
  BrigSegment8_t segment;

  /* Array dimensions, if non-zero.  */
  uint32_t dimLo, dimHi;
};

/* Abstract class for HSA instruction operands. */

struct hsa_op_base
{
  /* Next operand scheduled to be written when writing BRIG operand
     section.  */
  hsa_op_base *next;

  /* Offset to which the associated operand structure will be written.  Zero if
     yet not scheduled for writing.  */
  unsigned offset;

  /* The type of a particular operand.  */
  BrigOperandKinds16_t kind;
};

/* An immediate HSA operand.  */

struct hsa_op_immed : public hsa_op_base
{
  /* Type of the. */
  BrigType16_t type;
  /* Value as represented by middle end.  */
  tree value;
};

/* Report whether or not P is a na immediate operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_immed *>::test (struct hsa_op_base *p)
{
  return p->kind == BRIG_OPERAND_IMMED;
}

/* HSA register operand.  */

struct hsa_op_reg : public hsa_op_base
{
  /* If NON-NULL, gimple SSA that we come from.  NULL if none.
     !!? Do we need it? */
  tree gimple_ssa;

  /* Defining instrution while still in the SSA.  */
  hsa_insn_basic *def_insn;
  /* Uses of the value while still in SSA.  */
  vec <hsa_insn_basic_p> uses;

  /* If the register allocator decides to spill the register, this is the
     appropriate spill symbol.  */
  hsa_symbol *spill_sym;

  /* Number of this register structure in the order in which they were
     allocated.  */
  int order;

  /* Type of data in the register.  */
  BrigType16_t type;
  /* Zero if the register is not yet allocated.  After, allocation, this must
     be 'c', 's', 'd' or 'q'.  */
  char reg_class;
  /* If allocated, the number of the hw register (within its HSA register
     class). */
  char hard_num;
};
typedef struct hsa_op_reg *hsa_op_reg_p;

/* Report whether or not P is a register operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_reg *>::test (struct hsa_op_base *p)
{
  return p->kind == BRIG_OPERAND_REG;
}

/* An address HSA operand.  */

struct hsa_op_address : public hsa_op_base
{
  /* Symbol base of the address.  Can be NULL if there is none.  */
  hsa_symbol *symbol;

  /* Register offset.  Can be NULL if there is none.  */
  hsa_op_reg *reg;

  /* Immediate byte offset.  */
  HOST_WIDE_INT imm_offset;
};

/* Report whether or not P is an address operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_address *>::test (struct hsa_op_base *p)
{
  return p->kind == BRIG_OPERAND_ADDRESS;
}

/* A reference-to-label HSA operand.  In reality this is a reference to a start
   of a BB.  */

struct hsa_op_label : public hsa_op_base
{
  /* Offset in the code section that this label refers to.  */
  unsigned directive_offset;
};

/* Report whether or not P is a label reference operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_label *>::test (struct hsa_op_base *p)
{
  return p->kind == BRIG_OPERAND_LABEL_REF;
}

#define HSA_OPERANDS_PER_INSN 5

/* Class representing an HSA instruction.  Unlike typical ancestors for
   specialized classes, this one is also directly used for all instructions
   that are then represented as BrigInstBasic.  */

struct hsa_insn_basic
{
  /* The previous and next instruction in the basic block.  */
  struct hsa_insn_basic *prev, *next;

  /* Basic block this instruction belongs to.  */
  basic_block bb;

  /* Operand code distinguishing different types of instructions.  Eventually
     these should only be BRIG_INST_* values from the BrigOpcode16_t range but
     initially we use negative values for PHI nodes and such.  */
  int opcode;

  /* Type of the destination of the operations.  */
  BrigType16_t type;

  /* The individual operands, NULL if unused.  */
  struct hsa_op_base *operands[HSA_OPERANDS_PER_INSN];
};

#define HSA_OPCODE_PHI -1

/* Structure representing a PHI node of the SSA form of HSA virtual
   registers.  */

struct hsa_insn_phi : public hsa_insn_basic
{
  /* Destination.  */
  struct hsa_op_reg *dest;

  /* FIXME: In order to handle BBs with more than 5 predecesores we will need
     more operands.  */
};

/* Report whether or not P is a PHI node.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_phi *>::test (struct hsa_insn_basic *p)
{
  return p->opcode == HSA_OPCODE_PHI;
}

/* HSA instruction for branches.  */

struct hsa_insn_br : public hsa_insn_basic
{
  /* FIXME: Modifiers are missing but I do not understand what ALU modifiers
     can mean here.  */

  /* Width as described in HSA documentation.  */
  BrigWidth8_t width;
};

/* Report whether P is a branching instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_br *>::test (struct hsa_insn_basic *p)
{
  return p->opcode == BRIG_OPCODE_BRN
    || p->opcode == BRIG_OPCODE_CBR;
}

/* HSA instruction for comparisons.  */

struct hsa_insn_cmp : public hsa_insn_basic
{
  /* Source type should be derived from operand types.  */

  /* The comparison operation.  */
  BrigCompareOperation8_t compare;

  /* TODO: Modifiers are missing but so are everywhere else.  */
};

/* Report whether or not P is a comparison instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_cmp *>::test (struct hsa_insn_basic *p)
{
  return p->opcode == BRIG_OPCODE_CMP;
}

/* HSA instruction for memory operations.  */

struct hsa_insn_mem : public hsa_insn_basic
{
  /* The segment is of the memory access is either the segment of the symbol in
     the address operand or flat address is there is no symbol there.  */

  /* HSA equiv class, basically an alias set number. */
  uint8_t equiv_class;

  /* Things like aquire/release/aligned.  */
  enum BrigMemorySemantic semantic;

  /* TODO:  Add width modifier, perhaps also other things.  */
};

/* Report whether or not P is a memory instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_mem *>::test (struct hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_LD
	  || p->opcode == BRIG_OPCODE_ST);
}

/* HSA instruction for atomic operations.  */

struct hsa_insn_atomic : public hsa_insn_mem
{
  /* The operation itself.  */
  enum BrigAtomicOperation atomicop;
};

/* Report whether or not P is a memory instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_atomic *>::test (struct hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_ATOMIC
	  || p->opcode == BRIG_OPCODE_ATOMICNORET);
}

/* Though the HSA PRM in 19.10.1.7 says that LDA is should be BrigInstMem, the
   verifier insists it is Brig Inst Addr, so provide it here too.  */

struct hsa_insn_addr : public hsa_insn_basic
{
  /* The segment is of the memory access is either the segment of the symbol in
     the address operand or flat address is there is no symbol there.  */
};

/* Report whether or not P is a memory address instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_addr *>::test (struct hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_LDA);
}

/* HSA instruction to convert between flat addressing and segments.  */

struct hsa_insn_seg : hsa_insn_basic
{
  /* Source type.  Depends on the source addressing/segment.  */
  BrigType16_t src_type;
  /* The segment we are converting from or to.  */
  BrigSegment8_t segment;
};

/* Report whether or not P is a segment conversion instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_seg *>::test (struct hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_STOF
	  || p->opcode == BRIG_OPCODE_FTOS);
}

/* Basic block of HSA instructions.  */

struct hsa_bb
{
  /* The real CFG BB that this HBB belongs to.  */
  basic_block bb;

  /* The operand that referes to the label to this BB.  */
  hsa_op_label label_ref;

  /* The first and last instruction.  */
  struct hsa_insn_basic *first_insn, *last_insn;
  /* The first and last phi node.  */
  struct hsa_insn_phi *first_phi, *last_phi;

  /* Just a number to construct names from.  */
  int index;
};

/* Return the corresponding HSA basic block structure for the given control
   flow basic_block BB.  */

static inline hsa_bb *
hsa_bb_for_bb (basic_block bb)
{
  return (struct hsa_bb *) bb->aux;
}

/* Class for hashing local hsa_symbols.  */

struct hsa_noop_symbol_hasher : typed_noop_remove <hsa_symbol>
{
  typedef hsa_symbol value_type;
  typedef hsa_symbol compare_type;
  static inline hashval_t hash (const value_type *);
  static inline bool equal (const value_type *, const compare_type *);
};

/* Hash hsa_symbol.  */

inline hashval_t
hsa_noop_symbol_hasher::hash (const value_type *item)
{
  return DECL_UID (item->decl);
}

/* Return true if the DECL_UIDs of decls both symbols refer to  are equal.  */

inline bool
hsa_noop_symbol_hasher::equal (const value_type *a, const compare_type *b)
{
  return (DECL_UID (a->decl) == DECL_UID (b->decl));
}

/* Class for hashing global hsa_symbols.  */

struct hsa_free_symbol_hasher : typed_free_remove <hsa_symbol>
{
  typedef hsa_symbol value_type;
  typedef hsa_symbol compare_type;
  static inline hashval_t hash (const value_type *);
  static inline bool equal (const value_type *, const compare_type *);
};

/* Hash hsa_symbol.  */

inline hashval_t
hsa_free_symbol_hasher::hash (const value_type *item)
{
  return DECL_UID (item->decl);
}

/* Return true if the DECL_UIDs of decls both symbols refer to  are equal.  */

inline bool
hsa_free_symbol_hasher::equal (const value_type *a, const compare_type *b)
{
  return (DECL_UID (a->decl) == DECL_UID (b->decl));
}

/* Structure that encapsulates intermediate representation of a HSA
   function.  */

struct hsa_function_representation
{
  /* Name of the function.  */
  char *name;

  /* Input arguments of the function.  */
  /* FIXME: Normally we'd use a vector, however our C++ vectors seem to have
     problems with derived classes, so for now we'll use a simple array.  */
  int input_args_count;

  /* Number of allocated register structires.  */
  int reg_count;

  struct hsa_symbol *input_args;
  /* Output argument or NULL if there is none.  */
  struct hsa_symbol *output_arg;
  /* Hash table of local variable symbols.  */
  hash_table <hsa_noop_symbol_hasher> local_symbols;
  /* Vector of pointers to spill symbols.  */
  vec <struct hsa_symbol *> spill_symbols;

  /* Instructions to be executed before the first BB from gimple.  It's label
   is zero and must not be referenced, of course there are no PHIs.  */
  hsa_bb prologue;

  /* Number of HBB BBs.  */
  int hbb_count;

  /* Whether or not we chould check and enforce SSA properties.  */
  bool in_ssa;
};

/* In hsa-gen.c.  */
extern struct hsa_function_representation hsa_cfun;
extern hash_table <hsa_free_symbol_hasher> hsa_global_variable_symbols;
bool hsa_machine_large_p (void);
BrigType16_t hsa_get_segment_addr_type (BrigSegment8_t);
bool hsa_full_profile_p (void);
bool hsa_opcode_op_output_p (BrigOpcode16_t, int);
void hsa_build_append_simple_mov (hsa_op_reg *, hsa_op_base *, hsa_bb *);
hsa_symbol *hsa_get_spill_symbol (BrigType16_t);
hsa_op_reg *hsa_spill_in (hsa_insn_basic *, hsa_op_reg *, hsa_op_reg **);
hsa_op_reg *hsa_spill_out (hsa_insn_basic *, hsa_op_reg *, hsa_op_reg **);
hsa_bb *hsa_init_new_bb (basic_block);
void hsa_deinit_compilation_unit_data (void);

/* In hsa-regalloc.c.  */
void hsa_regalloc (void);

/* In hsa-brig.c.  */
void hsa_brig_emit_function (void);
void hsa_output_brig (void);
BrigType16_t bittype_for_type (BrigType16_t t);

/*  In hsa-dump.c.  */
const char *hsa_seg_name (BrigSegment8_t);
void dump_hsa_bb (FILE *, hsa_bb *);
void dump_hsa_cfun (FILE *);

#endif /* HSA_H */
