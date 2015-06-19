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
#include "predict.h"
#include "tree.h"
#include "vec.h"
#include "hash-table.h"
#include "basic-block.h"

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
  unsigned directive_offset;

  /* HSA type of the parameter.  */
  BrigType16_t type;

  /* The HSA segment this will eventually end up in.  */
  BrigSegment8_t segment;

  /* The HSA kind of linkage.  */
  BrigLinkage8_t linkage;

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
  unsigned brig_op_offset;

  /* The type of a particular operand.  */
  BrigKind16_t kind;
};

/* Common abstract ancestor for operands which have a type.  */

struct hsa_op_with_type : public hsa_op_base
{
  /* The type.  */
  BrigType16_t type;
};

/* An immediate HSA operand.  */

struct hsa_op_immed : public hsa_op_with_type
{
  /* Value as represented by middle end.  */
  tree value;
};

/* Report whether or not P is a an immediate operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_immed *>::test (hsa_op_base *p)
{
  return p->kind == BRIG_KIND_OPERAND_CONSTANT_BYTES;
}

/* HSA register operand.  */

struct hsa_op_reg : public hsa_op_with_type
{
  /* Destructor.  */
  ~hsa_op_reg ()
  {
  }

  /* Verify register operand.  */
  void verify ();

  /* If NON-NULL, gimple SSA that we come from.  NULL if none.  */
  tree gimple_ssa;

  /* Defining instrution while still in the SSA.  */
  hsa_insn_basic *def_insn;
  /* Uses of the value while still in SSA.  */
  auto_vec <hsa_insn_basic_p> uses;

  /* If the register allocator decides to spill the register, this is the
     appropriate spill symbol.  */
  hsa_symbol *spill_sym;

  /* Number of this register structure in the order in which they were
     allocated.  */
  int order;
  int lr_begin, lr_end;

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
is_a_helper <hsa_op_reg *>::test (hsa_op_base *p)
{
  return p->kind == BRIG_KIND_OPERAND_REGISTER;
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
is_a_helper <hsa_op_address *>::test (hsa_op_base *p)
{
  return p->kind == BRIG_KIND_OPERAND_ADDRESS;
}

/* A reference to code HSA operand. It can be either reference
   to a start of a BB or a start of a function.  */

struct hsa_op_code_ref : public hsa_op_base
{
  /* Offset in the code section that this refers to.  */
  unsigned directive_offset;
};

/* Report whether or not P is a code reference operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_code_ref *>::test (hsa_op_base *p)
{
  return p->kind == BRIG_KIND_OPERAND_CODE_REF;
}

/* Code list HSA operand.  */
struct hsa_op_code_list: public hsa_op_base
{
  /* Destructor.  */
  ~hsa_op_code_list ()
  {
  }

  /* Offset to variable-sized array in hsa_data section, where
     are offsets to entries in the hsa_code section.  */
  auto_vec<unsigned> offsets;
};

/* Report whether or not P is a code list operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_code_list *>::test (hsa_op_base *p)
{
  return p->kind == BRIG_KIND_OPERAND_CODE_LIST;
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

  int number;

  /* Type of the destination of the operations.  */
  BrigType16_t type;

  /* The individual operands, NULL if unused.  */
  struct hsa_op_base *operands[HSA_OPERANDS_PER_INSN];
};

#define HSA_OPCODE_PHI -1
#define HSA_OPCODE_CALL_BLOCK -2

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
is_a_helper <hsa_insn_phi *>::test (hsa_insn_basic *p)
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
is_a_helper <hsa_insn_br *>::test (hsa_insn_basic *p)
{
  return p->opcode == BRIG_OPCODE_BR
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
is_a_helper <hsa_insn_cmp *>::test (hsa_insn_basic *p)
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
  enum BrigMemoryOrder memoryorder;

  /* Scope of the atomic opeeration. */
  enum BrigMemoryScope memoryscope;

  /* TODO:  Add width modifier, perhaps also other things.  */
};

/* Report whether or not P is a memory instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_mem *>::test (hsa_insn_basic *p)
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
is_a_helper <hsa_insn_atomic *>::test (hsa_insn_basic *p)
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
is_a_helper <hsa_insn_addr *>::test (hsa_insn_basic *p)
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
is_a_helper <hsa_insn_seg *>::test (hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_STOF
	  || p->opcode == BRIG_OPCODE_FTOS);
}

/* HSA instruction for function call.  */

struct hsa_insn_call: hsa_insn_basic
{
  /* Destructor.  */
  ~hsa_insn_call ()
  {
  }

  /* Called function */
  tree called_function;

  /* Called function code reference.  */
  struct hsa_op_code_ref func;

  /* Argument symbols.  */
  auto_vec <hsa_symbol *> args_symbols;

  /* Code list for arguments of the function.  */
  hsa_op_code_list *args_code_list;

  /* Result symbol.  */
  hsa_symbol *result_symbol;

  /* Code list for result of the function.  */
  hsa_op_code_list *result_code_list;
};

/* Report whether or not P is a call instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_call *>::test (hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_CALL);
}

/* HSA call instruction block encapsulates definition of arguments,
   result type, corresponding loads and a possible store.
   Moreover, it contains a single call instruction.
   Emission of the instruction will produce multiple
   HSAIL instructions.  */

struct hsa_insn_call_block: hsa_insn_basic
{
  /* Destructor.  */
  ~hsa_insn_call_block ()
  {
  }

  /* Input formal arguments.  */
  auto_vec <hsa_symbol *> input_args;

  /* Input arguments store instructions.  */
  auto_vec <hsa_insn_mem *> input_arg_insns;

  /* Output argument, can be NULL for void functions.  */
  hsa_symbol *output_arg;

  /* Output argument load instruction.  */
  hsa_insn_mem *output_arg_insn;

  /* Call isntruction.  */
  hsa_insn_call *call_insn;
};

/* Report whether or not P is a call block instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_call_block *>::test (hsa_insn_basic *p)
{
  return (p->opcode == HSA_OPCODE_CALL_BLOCK);
}

/* Basic block of HSA instructions.  */

struct hsa_bb
{
  /* The real CFG BB that this HBB belongs to.  */
  basic_block bb;

  /* The operand that referes to the label to this BB.  */
  hsa_op_code_ref label_ref;

  /* The first and last instruction.  */
  struct hsa_insn_basic *first_insn, *last_insn;
  /* The first and last phi node.  */
  struct hsa_insn_phi *first_phi, *last_phi;

  /* Just a number to construct names from.  */
  int index;

  bitmap liveout, livein;
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
  typedef hsa_symbol *value_type;
  typedef hsa_symbol *compare_type;
  static inline hashval_t hash (const value_type);
  static inline bool equal (const value_type, const compare_type);
};

/* Hash hsa_symbol.  */

inline hashval_t
hsa_noop_symbol_hasher::hash (const value_type item)
{
  return DECL_UID (item->decl);
}

/* Return true if the DECL_UIDs of decls both symbols refer to  are equal.  */

inline bool
hsa_noop_symbol_hasher::equal (const value_type a, const compare_type b)
{
  return (DECL_UID (a->decl) == DECL_UID (b->decl));
}

/* Class for hashing global hsa_symbols.  */

struct hsa_free_symbol_hasher : typed_free_remove <hsa_symbol>
{
  typedef hsa_symbol *value_type;
  typedef hsa_symbol *compare_type;
  static inline hashval_t hash (const value_type);
  static inline bool equal (const value_type, const compare_type);
};

/* Hash hsa_symbol.  */

inline hashval_t
hsa_free_symbol_hasher::hash (const value_type item)
{
  return DECL_UID (item->decl);
}

/* Return true if the DECL_UIDs of decls both symbols refer to  are equal.  */

inline bool
hsa_free_symbol_hasher::equal (const value_type a, const compare_type b)
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

  /* Number of allocated register structures.  */
  int reg_count;

  struct hsa_symbol *input_args;
  /* Output argument or NULL if there is none.  */
  struct hsa_symbol *output_arg;
  /* Hash table of local variable symbols.  */
  hash_table <hsa_noop_symbol_hasher> *local_symbols;
  /* Vector of pointers to spill symbols.  */
  vec <struct hsa_symbol *> spill_symbols;

  /* Instructions to be executed before the first BB from gimple.  It's label
   is zero and must not be referenced, of course there are no PHIs.  */
  hsa_bb prologue;

  /* Number of HBB BBs.  */
  int hbb_count;

  /* Whether or not we could check and enforce SSA properties.  */
  bool in_ssa;

  /* True if the function is kernel function.  */
  bool kern_p;
};

/* in hsa.c */
extern struct hsa_function_representation hsa_cfun;
extern hash_table <hsa_free_symbol_hasher> *hsa_global_variable_symbols;
void hsa_init_compilation_unit_data (void);
void hsa_deinit_compilation_unit_data (void);
bool hsa_machine_large_p (void);
bool hsa_full_profile_p (void);
bool hsa_opcode_op_output_p (BrigOpcode16_t, int);
unsigned hsa_type_bit_size (BrigType16_t t);
BrigType16_t hsa_bittype_for_type (BrigType16_t t);
void hsa_add_kern_decl_mapping (tree decl, char *name);
unsigned hsa_get_number_decl_kernel_mappings (void);
tree hsa_get_decl_kernel_mapping_decl (unsigned i);
char *hsa_get_decl_kernel_mapping_name (unsigned i);
void hsa_free_decl_kernel_mapping (void);

/* In hsa-gen.c.  */
void hsa_build_append_simple_mov (hsa_op_reg *, hsa_op_base *, hsa_bb *);
hsa_symbol *hsa_get_spill_symbol (BrigType16_t);
hsa_op_reg *hsa_spill_in (hsa_insn_basic *, hsa_op_reg *, hsa_op_reg **);
hsa_op_reg *hsa_spill_out (hsa_insn_basic *, hsa_op_reg *, hsa_op_reg **);
hsa_bb *hsa_init_new_bb (basic_block);

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
