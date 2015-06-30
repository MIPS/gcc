/* A pass for lowering gimple to HSAIL
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

/* TODO: Some of the following includes might be redundant because of ongoing
   header cleanups.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "is-a.h"
#include "hash-set.h"
#include "defaults.h"
#include "hard-reg-set.h"
#include "symtab.h"
#include "vec.h"
#include "input.h"
#include "alias.h"
#include "double-int.h"
#include "inchash.h"
#include "tree.h"
#include "tree-pass.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-expr.h"
#include "dominance.h"
#include "cfg.h"
#include "function.h"
#include "predict.h"
#include "basic-block.h"
#include "fold-const.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "machmode.h"
#include "output.h"
#include "function.h"
#include "bitmap.h"
#include "dumpfile.h"
#include "gimple-pretty-print.h"
#include "diagnostic-core.h"
#include "alloc-pool.h"
#include "tree-ssa-operands.h"
#include "gimple-ssa.h"
#include "tree-phinodes.h"
#include "stringpool.h"
#include "tree-ssanames.h"
#include "rtl.h"
#include "expr.h"
#include "tree-dfa.h"
#include "ssa-iterators.h"
#include "ipa-ref.h"
#include "lto-streamer.h"
#include "cgraph.h"
#include "stor-layout.h"
#include "gimplify-me.h"
#include "print-tree.h"
#include "hsa.h"

/* Alloc pools for allocating basic hsa structures such as operands,
   instructions and other basic entities.s */
static pool_allocator<hsa_op_address> *hsa_allocp_operand_address;
static pool_allocator<hsa_op_immed> *hsa_allocp_operand_immed;
static pool_allocator<hsa_op_reg> *hsa_allocp_operand_reg;
static pool_allocator<hsa_op_code_list> *hsa_allocp_operand_code_list;
static pool_allocator<hsa_insn_basic> *hsa_allocp_inst_basic;
static pool_allocator<hsa_insn_phi> *hsa_allocp_inst_phi;
static pool_allocator<hsa_insn_mem> *hsa_allocp_inst_mem;
static pool_allocator<hsa_insn_atomic> *hsa_allocp_inst_atomic;
static pool_allocator<hsa_insn_seg> *hsa_allocp_inst_seg;
static pool_allocator<hsa_insn_cmp> *hsa_allocp_inst_cmp;
static pool_allocator<hsa_insn_br> *hsa_allocp_inst_br;
static pool_allocator<hsa_insn_call> *hsa_allocp_inst_call;
static pool_allocator<hsa_insn_arg_block> *hsa_allocp_inst_arg_block;
static pool_allocator<hsa_bb> *hsa_allocp_bb;
static pool_allocator<hsa_symbol> *hsa_allocp_symbols;

/* Vectors with selected instructions and operands that need
   a destruction.  */
static vec <hsa_op_code_list *> hsa_list_operand_code_list;
static vec <hsa_op_reg *> hsa_list_operand_reg;

/* Constructor of class representing global HSA function/kernel information and
   state.  */

/* TODO: Move more initialization here. */

hsa_function_representation::hsa_function_representation ()
  : prologue (ENTRY_BLOCK_PTR_FOR_FN (cfun), 0)
{
  name = NULL;
  input_args_count = 0;
  reg_count = 0;
  input_args = NULL;
  output_arg = NULL;

  int sym_init_len = (vec_safe_length (cfun->local_decls) / 2) + 1;;
  local_symbols = new hash_table <hsa_noop_symbol_hasher> (sym_init_len);
  spill_symbols = vNULL;
  hbb_count = 1;        /* 0 is for prologue.  */
  in_ssa = true;	/* We start in SSA.  */
  kern_p = false;
  declaration_p = false;
  called_functions = vNULL;
}

/* Destructor of class holding function/kernel-wide informaton and state.  */

hsa_function_representation::~hsa_function_representation ()
{
  delete local_symbols;
  free (input_args);
  free (output_arg);
  /* Kernel names are deallocated at the end of BRIG output when deallocating
     hsa_decl_kernel_mapping.  */
  if (!kern_p)
    free (name);
  spill_symbols.release ();
  called_functions.release ();
}

/* Allocate HSA structures that we need only while generating with this.  */

static void
hsa_init_data_for_cfun ()
{
  int sym_init_len;

  hsa_init_compilation_unit_data ();
  hsa_allocp_operand_address
    = new pool_allocator<hsa_op_address> ("HSA address operands", 8);
  hsa_allocp_operand_immed
    = new pool_allocator<hsa_op_immed> ("HSA immediate operands", 32);
  hsa_allocp_operand_reg
    = new pool_allocator<hsa_op_reg> ("HSA register operands", 64);
  hsa_allocp_operand_code_list
    = new pool_allocator<hsa_op_code_list> ("HSA code list operands", 64);
  hsa_allocp_inst_basic
    = new pool_allocator<hsa_insn_basic> ("HSA basic instructions", 64);
  hsa_allocp_inst_phi
    = new pool_allocator<hsa_insn_phi> ("HSA phi operands", 16);
  hsa_allocp_inst_mem
    = new pool_allocator<hsa_insn_mem> ("HSA memory instructions", 32);
  hsa_allocp_inst_atomic
    = new pool_allocator<hsa_insn_atomic> ("HSA atomic instructions", 32);
  hsa_allocp_inst_seg
    = new pool_allocator<hsa_insn_seg> ("HSA segment conversion instructions",
					16);
  hsa_allocp_inst_cmp
    = new pool_allocator<hsa_insn_cmp> ("HSA comparison instructions", 16);
  hsa_allocp_inst_br
    = new pool_allocator<hsa_insn_br> ("HSA branching instructions", 16);
  hsa_allocp_inst_call
    = new pool_allocator<hsa_insn_call> ("HSA call instructions", 16);
  hsa_allocp_inst_arg_block
    = new pool_allocator<hsa_insn_arg_block> ("HSA arg block instructions",
					       16);
  hsa_allocp_bb = new pool_allocator<hsa_bb> ("HSA basic blocks", 8);

  sym_init_len = (vec_safe_length (cfun->local_decls) / 2) + 1;
  hsa_allocp_symbols = new pool_allocator<hsa_symbol> ("HSA symbols",
						       sym_init_len);

  hsa_cfun = new hsa_function_representation ();
}

/* Deinitialize HSA subsystem and free all allocated memory.  */

static void
hsa_deinit_data_for_cfun (void)
{
  basic_block bb;

  FOR_EACH_BB_FN (bb, cfun)
    if (bb->aux)
      {
	hsa_bb *hbb = hsa_bb_for_bb (bb);
	hsa_insn_phi *phi;
	for (phi = hbb->first_phi;
	     phi;
	     phi = phi->next ? as_a <hsa_insn_phi *> (phi->next): NULL)
	  phi->~hsa_insn_phi ();
	for (hsa_insn_basic *insn = hbb->first_insn; insn; insn = insn->next)
	  hsa_destroy_insn (insn);

	hbb->~hsa_bb ();
	bb->aux = NULL;
      }

  for (unsigned int i = 0; i < hsa_list_operand_code_list.length (); i++)
    hsa_list_operand_code_list[i]->~hsa_op_code_list ();

  for (unsigned int i = 0; i < hsa_list_operand_reg.length (); i++)
    hsa_list_operand_reg[i]->~hsa_op_reg ();

  hsa_list_operand_code_list.release ();
  hsa_list_operand_reg.release ();

  delete hsa_allocp_operand_address;
  delete hsa_allocp_operand_immed;
  delete hsa_allocp_operand_reg;
  delete hsa_allocp_operand_code_list;
  delete hsa_allocp_inst_basic;
  delete hsa_allocp_inst_phi;
  delete hsa_allocp_inst_atomic;
  delete hsa_allocp_inst_mem;
  delete hsa_allocp_inst_seg;
  delete hsa_allocp_inst_cmp;
  delete hsa_allocp_inst_br;
  delete hsa_allocp_inst_call;
  delete hsa_allocp_inst_arg_block;
  delete hsa_allocp_bb;
  delete hsa_allocp_symbols;
  delete hsa_cfun;
}

/* Return the type which holds addresses in the given SEGMENT.  */

static BrigType16_t
hsa_get_segment_addr_type (BrigSegment8_t segment)
{
  switch (segment)
    {
    case BRIG_SEGMENT_NONE:
      gcc_unreachable ();

    case BRIG_SEGMENT_FLAT:
    case BRIG_SEGMENT_GLOBAL:
    case BRIG_SEGMENT_READONLY:
    case BRIG_SEGMENT_KERNARG:
      return hsa_machine_large_p () ? BRIG_TYPE_U64 : BRIG_TYPE_U32;

    case BRIG_SEGMENT_GROUP:
    case BRIG_SEGMENT_PRIVATE:
    case BRIG_SEGMENT_SPILL:
    case BRIG_SEGMENT_ARG:
      return BRIG_TYPE_U32;
    }
  gcc_unreachable ();
}

/* Return HSA type for tree TYPE, which has to fit into BrigType16_t.  Pointers
   are assumed to use flat addressing.  If min32int is true, always expand
   integer types to one that has at least 32 bits.  */

static BrigType16_t
hsa_type_for_scalar_tree_type (const_tree type, bool min32int)
{
  HOST_WIDE_INT bsize;
  const_tree base;
  BrigType16_t res = BRIG_TYPE_NONE;

  gcc_checking_assert (TYPE_P (type));
  gcc_checking_assert (!AGGREGATE_TYPE_P (type));
  if (POINTER_TYPE_P (type))
    return hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT);

  if (TREE_CODE (type) == VECTOR_TYPE)
    base = TREE_TYPE (type);
  else
    base = type;

  if (!tree_fits_uhwi_p (TYPE_SIZE (base)))
    {
      sorry ("Support for HSA does not implement huge or variable-sized type %T",
	     type);
      return res;
    }

  bsize = tree_to_uhwi (TYPE_SIZE (base));
  if (INTEGRAL_TYPE_P (base))
    {
      if (TYPE_UNSIGNED (base))
	{
	  switch (bsize)
	    {
	    case 8:
	      res = BRIG_TYPE_U8;
	      break;
	    case 16:
	      res = BRIG_TYPE_U16;
	      break;
	    case 32:
	      res = BRIG_TYPE_U32;
	      break;
	    case 64:
	      res = BRIG_TYPE_U64;
	      break;
	    default:
	      break;
	    }
	}
      else
	{
	  switch (bsize)
	    {
	    case 8:
	      res = BRIG_TYPE_S8;
	      break;
	    case 16:
	      res = BRIG_TYPE_S16;
	      break;
	    case 32:
	      res = BRIG_TYPE_S32;
	      break;
	    case 64:
	      res = BRIG_TYPE_S64;
	      break;
	    default:
	      break;
	    }
	}
    }
  if (SCALAR_FLOAT_TYPE_P (base))
    {
      switch (bsize)
	{
	case 16:
	  res = BRIG_TYPE_F16;
	  break;
	case 32:
	  res = BRIG_TYPE_F32;
	  break;
	case 64:
	  res = BRIG_TYPE_F64;
	  break;
	default:
	  break;
	}
    }

  if (res == BRIG_TYPE_NONE)
    {
      sorry ("Support for HSA does not implement type %T", type);
      return res;
    }

  if (TREE_CODE (type) == VECTOR_TYPE)
    {
      HOST_WIDE_INT tsize = tree_to_uhwi (TYPE_SIZE (type));
      switch (tsize)
	{
	case 32:
	  res |= BRIG_TYPE_PACK_32;
	  break;
	case 64:
	  res |= BRIG_TYPE_PACK_64;
	  break;
	case 128:
	  res |= BRIG_TYPE_PACK_128;
	  break;
	default:
	  sorry ("Support for HSA does not implement type %T", type);
	}
    }

  if (min32int)
    {
      /* Registers/immediate operands can only be 32bit or more except for
         f16.  */
      if (res == BRIG_TYPE_U8 || res == BRIG_TYPE_U16)
	res = BRIG_TYPE_U32;
      else if (res == BRIG_TYPE_S8 || res == BRIG_TYPE_S16)
	res = BRIG_TYPE_S32;
    }
  return res;
}

/* Returns the BRIG type we need to load/store entities of TYPE.  */

static BrigType16_t
mem_type_for_type (BrigType16_t type)
{
  /* HSA has non-intuitive constraints on load/store types.  If it's
     a bit-type it _must_ be B128, if it's not a bit-type it must be
     64bit max.  So for loading entities of 128 bits (e.g. vectors)
     we have to to B128, while for loading the rest we have to use the
     input type (??? or maybe also flattened to a equally sized non-vector
     unsigned type?).  */
  if ((type & BRIG_TYPE_PACK_MASK) == BRIG_TYPE_PACK_128)
    return BRIG_TYPE_B128;
  return type;
}

/* Return HSA type for tree TYPE.  If it cannot fit into BrigType16_t, some
   kind of array will be generated, setting DIM appropriately.  Otherwise, it
   will be set to zero.  */

static BrigType16_t
hsa_type_for_tree_type (const_tree type, unsigned HOST_WIDE_INT *dim_p)
{
  gcc_checking_assert (TYPE_P (type));
  if (!tree_fits_uhwi_p (TYPE_SIZE_UNIT (type)))
    {
      sorry ("Support for HSA does not implement huge or variable-sized type %T",
	     type);
      return BRIG_TYPE_NONE;
    }

  if (RECORD_OR_UNION_TYPE_P (type))
    {
      *dim_p = tree_to_uhwi (TYPE_SIZE_UNIT (type));
      gcc_assert (*dim_p);
      return BRIG_TYPE_U8 | BRIG_TYPE_ARRAY;
    }

  if (TREE_CODE (type) == ARRAY_TYPE)
    {
      /* We try to be nice and use the real base-type when this is an array of
	 scalars and only resort to an array of bytes if the type is more
	 complex.  */

      unsigned HOST_WIDE_INT dim = 1;

      while (TREE_CODE (type) == ARRAY_TYPE)
	{
	  tree domain = TYPE_DOMAIN (type);
	  if (!TYPE_MIN_VALUE (domain)
	      || !TYPE_MAX_VALUE (domain)
	      || !tree_fits_shwi_p (TYPE_MIN_VALUE (domain))
	      || !tree_fits_shwi_p (TYPE_MAX_VALUE (domain)))
	    {
	      sorry ("Support for HSA does not implement array %T with unknown "
		     "bounds", type);
	      return BRIG_TYPE_NONE;
	    }
	  HOST_WIDE_INT min = tree_to_shwi (TYPE_MIN_VALUE (domain));
	  HOST_WIDE_INT max = tree_to_shwi (TYPE_MAX_VALUE (domain));
	  dim = dim * (unsigned HOST_WIDE_INT) (max - min + 1);
	  type = TREE_TYPE (type);
	}

      BrigType16_t res;
      if (RECORD_OR_UNION_TYPE_P (type))
	{
	  dim = dim * tree_to_uhwi (TYPE_SIZE_UNIT (type));
	  res = BRIG_TYPE_U8;
	}
      else
	res = hsa_type_for_scalar_tree_type (type, false);

      *dim_p = dim;
      return res | BRIG_TYPE_ARRAY;
    }

  /* Scalar case: */
  *dim_p = 0;
  return hsa_type_for_scalar_tree_type (type, false);
}

/* Returns true if converting from STYPE into DTYPE needs the _CVT
   opcode.  If false a normal _MOV is enough.  */

static bool
hsa_needs_cvt (BrigType16_t dtype, BrigType16_t stype)
{
  /* float <-> int conversions are real converts.  */
  if (hsa_type_float_p (dtype) != hsa_type_float_p (stype))
    return true;
  /* When both types have different size, then we need CVT as well.  */
  if (hsa_type_bit_size (dtype) != hsa_type_bit_size (stype))
    return true;
  return false;
}

/* Fill in those values into SYM according to DECL, which are determined
   independently from whether it is parameter, result, or a variable, local or
   global.  */

static void
fillup_sym_for_decl (tree decl, struct hsa_symbol *sym)
{
  sym->decl = decl;
  sym->type = hsa_type_for_tree_type (TREE_TYPE (decl), &sym->dim);
}

/* Lookup or create the associated hsa_symbol structure with a given VAR_DECL
   or lookup the hsa_structure corresponding to a PARM_DECL.  */

static hsa_symbol *
get_symbol_for_decl (tree decl)
{
  struct hsa_symbol **slot;
  struct hsa_symbol dummy, *sym;

  gcc_assert (TREE_CODE (decl) == PARM_DECL
	      || TREE_CODE (decl) == RESULT_DECL
	      || TREE_CODE (decl) == VAR_DECL);

  dummy.decl = decl;

  if (TREE_CODE (decl) == VAR_DECL && is_global_var (decl))
    {
      slot = hsa_global_variable_symbols->find_slot (&dummy, INSERT);
      gcc_checking_assert (slot);
      if (*slot)
	return *slot;
      sym = XCNEW (struct hsa_symbol);
      sym->segment = BRIG_SEGMENT_GLOBAL;
      sym->linkage = BRIG_LINKAGE_FUNCTION;
      warning (0, "referring to global symbol %q+D by name from HSA code "
	       "won't work", decl);
    }
  else
    {
      slot = hsa_cfun->local_symbols->find_slot (&dummy, INSERT);
      gcc_checking_assert (slot);
      if (*slot)
	return *slot;
      gcc_assert (TREE_CODE (decl) == VAR_DECL);
      sym = hsa_allocp_symbols->allocate ();
      memset (sym, 0, sizeof (hsa_symbol));
      sym->segment = BRIG_SEGMENT_PRIVATE;
      sym->linkage = BRIG_LINKAGE_FUNCTION;
    }

  fillup_sym_for_decl (decl, sym);
  sym->name = get_declaration_name (decl);
  *slot = sym;
  return sym;
}

/* Create a spill symbol of type TYPE.  */

hsa_symbol *
hsa_get_spill_symbol (BrigType16_t type)
{
  hsa_symbol *sym = hsa_allocp_symbols->allocate ();
  memset (sym, 0, sizeof (hsa_symbol));
  sym->segment = BRIG_SEGMENT_SPILL;
  sym->linkage = BRIG_LINKAGE_FUNCTION;
  sym->type = type;
  hsa_cfun->spill_symbols.safe_push(sym);
  return sym;
}

/* Constructor of the ancetor if all operands.  K is BRIG kind that identified
   what the operator is.  */

hsa_op_base::hsa_op_base (BrigKind16_t k)
{
  next = NULL;
  brig_op_offset = 0;
  kind = k;
}

/* Constructor of ancestor of all operands which have a type.  K is BRIG kind
   that identified what the operator is.  T is the type of the operator.  */

hsa_op_with_type::hsa_op_with_type (BrigKind16_t k, BrigType16_t t)
  : hsa_op_base (k)
{
  type = t;
}

/* Constructor of class representing HSA immediate values.  TREE_VAL is the
   tree representation of the immediate value. */

hsa_op_immed::hsa_op_immed (tree tree_val)
  : hsa_op_with_type (BRIG_KIND_OPERAND_CONSTANT_BYTES,
		      hsa_type_for_scalar_tree_type (TREE_TYPE (tree_val),
						     true))
{
  gcc_checking_assert (is_gimple_min_invariant (tree_val)
		       && !POINTER_TYPE_P (TREE_TYPE (tree_val)));
  value = tree_val;
}

/* New operator to allocate immediate operands from pool alloc.  */

void *
hsa_op_immed::operator new (size_t)
{
  return hsa_allocp_operand_immed->allocate ();
}

/* Constructor of class representing HSA rgisters and pseudo-registers.  T is
   the BRIG type fo the new register.  */

hsa_op_reg::hsa_op_reg (BrigType16_t t)
  : hsa_op_with_type (BRIG_KIND_OPERAND_REGISTER, t)
{
  gimple_ssa = NULL_TREE;
  def_insn = NULL;
  spill_sym = NULL;
  order = hsa_cfun->reg_count++;
  lr_begin = lr_end = 0;
  reg_class = 0;
  hard_num = 0;

  hsa_list_operand_reg.safe_push (this);
}

/* New operator to allocate immediate operands from pool alloc.  */

void *
hsa_op_reg::operator new (size_t)
{
  return hsa_allocp_operand_reg->allocate ();
}

/* Verify register operand.  */

void
hsa_op_reg::verify ()
{
  /* Verify that each HSA register has a definition assigned.
     Exceptions are VAR_DECL and PARM_DECL that are a default
     definition.  */
  gcc_checking_assert (def_insn
		       || (gimple_ssa != NULL
			   && (!SSA_NAME_VAR (gimple_ssa)
		               || (TREE_CODE (SSA_NAME_VAR (gimple_ssa))
				   != PARM_DECL))
			   && SSA_NAME_IS_DEFAULT_DEF (gimple_ssa)));
}

/* set up a new address operand consisting of base symbol SYM, register R and
   immediate OFFSET.  If the machine model is not large and offset is 64 bit,
   the upper, 32 bits have to be zero.  */

hsa_op_address::hsa_op_address (hsa_symbol *sym, hsa_op_reg *r,
				HOST_WIDE_INT offset)
  : hsa_op_base (BRIG_KIND_OPERAND_ADDRESS)
{
  symbol = sym;
  reg = r;
  imm_offset = offset;
}

/* New operator to allocate address operands from pool alloc.  */

void *
hsa_op_address::operator new (size_t)
{
  return hsa_allocp_operand_address->allocate ();
}

/* Constructor of an operand referring to HSAIL code.  */

hsa_op_code_ref::hsa_op_code_ref () : hsa_op_base (BRIG_KIND_OPERAND_CODE_REF)
{
  directive_offset = 0;
}

/* Constructor of an operand representing a code list.  Set it up so that it
   can contain ELEMENTS number of elements.  */

hsa_op_code_list::hsa_op_code_list (unsigned elements)
  : hsa_op_base (BRIG_KIND_OPERAND_CODE_LIST)
{
  offsets.create (1);
  offsets.safe_grow_cleared (elements);

  hsa_list_operand_code_list.safe_push (this);
}

/* New operator to allocate code list operands.  */

void *
hsa_op_code_list::operator new (size_t)
{
  return hsa_allocp_operand_code_list->allocate ();
}

/* Lookup or create a HSA pseudo register for a given gimple SSA name.  */

static hsa_op_reg *
hsa_reg_for_gimple_ssa (tree ssa, vec <hsa_op_reg_p> *ssa_map)
{
  hsa_op_reg *hreg;

  gcc_checking_assert (TREE_CODE (ssa) == SSA_NAME);
  if ((*ssa_map)[SSA_NAME_VERSION (ssa)])
    return (*ssa_map)[SSA_NAME_VERSION (ssa)];

  hreg = new hsa_op_reg (hsa_type_for_scalar_tree_type (TREE_TYPE (ssa), true));
  hreg->gimple_ssa = ssa;
  (*ssa_map)[SSA_NAME_VERSION (ssa)] = hreg;

  return hreg;
}

/* Set the defining instruction of REG to be INSN.  When checking, make sure it
   was not set before.  */

static inline void
set_reg_def (hsa_op_reg *reg, hsa_insn_basic *insn)
{
  if (hsa_cfun->in_ssa)
    {
      gcc_checking_assert (!reg->def_insn);
      reg->def_insn = insn;
    }
  else
    reg->def_insn = NULL;
}

/* Constructor of the class which is the bases of all instructions and directly
   represents the most basic ones.  NOPS is the number of operands that the
   operand vector will contain (and which will be cleared).  OP is the opcode
   of the instruction.  This constructor does not set type.  */

hsa_insn_basic::hsa_insn_basic (unsigned nops, int opc)
{
  opcode = opc;

  prev = next = NULL;
  bb = NULL;
  number = 0;
  type = BRIG_TYPE_NONE;

  if (nops > 0)
    operands.safe_grow_cleared (nops);
}

/* Constructor of the class which is the bases of all instructions and directly
   represents the most basic ones.  NOPS is the number of operands that the
   operand vector will contain (and which will be cleared).  OPC is the opcode
   of the instruction, T is the type of the instruction.  */

hsa_insn_basic::hsa_insn_basic (unsigned nops, int opc, BrigType16_t t)
{
  opcode = opc;
  type = t;

  prev = next = NULL;
  bb = NULL;
  number = 0;

  if (nops > 0)
    operands.safe_grow_cleared (nops);
}

/* New operator for the most basic instructions.  */

void *
hsa_insn_basic::operator new (size_t)
{
  return hsa_allocp_inst_basic->allocate ();
}

/* Constructor of an instruction representing a PHI node.  NOPS is the number
   of operands (equal to the number of predecessors).  */

hsa_insn_phi::hsa_insn_phi (unsigned nops)
  : hsa_insn_basic (nops, HSA_OPCODE_PHI)
{
  dest = NULL;
}

/* Operator new for an instruction representing an HSA phi node.  */

void *
hsa_insn_phi::operator new (size_t)
{
  return hsa_allocp_inst_phi->allocate ();
}

/* Constructor of clas representing instruction for conditional jump, CTRL is
   the control register deterining whether the jump will be carried out, the
   new instruction is automatically added to its uses list.  */

hsa_insn_br::hsa_insn_br (hsa_op_reg *ctrl) : hsa_insn_basic (1, BRIG_OPCODE_CBR)
{
  width = BRIG_WIDTH_1;
  operands[0] = ctrl;
  ctrl->uses.safe_push (this);
}

/* New operator for jumps and contitional jumps.  */
void *
hsa_insn_br::operator new (size_t)
{
  return hsa_allocp_inst_br->allocate ();
}

/* Constructor of comparison instructin.  CMP is the comparison operation and T
   is the result type.  */

hsa_insn_cmp::hsa_insn_cmp (BrigCompareOperation8_t cmp, BrigType16_t t)
  : hsa_insn_basic (3 , BRIG_OPCODE_CMP, t)
{
  compare = cmp;
}

/* New operator for comparison instruction representation class.  */

void *
hsa_insn_cmp::operator new (size_t)
{
  return hsa_allocp_inst_cmp->allocate ();
}

/* Constructor of classes representing memory accesses.  OPC is the opcode (must
   be BRIG_OPCODE_ST or BRIG_OPCODE_LD) and T is the type.  */

hsa_insn_mem::hsa_insn_mem (int opc, BrigType16_t t)
  : hsa_insn_basic (2, opc, t)
{
  gcc_checking_assert (opc == BRIG_OPCODE_LD || opc == BRIG_OPCODE_ST);
  equiv_class = 0;
  memoryorder = BRIG_MEMORY_ORDER_NONE;
  memoryscope = BRIG_MEMORY_SCOPE_NONE;
}

/* Constructor for descendants allowing different opcodes and number of
   operands, it passes its arguments directly to hsa_insn_basic
   constructor.  */

hsa_insn_mem::hsa_insn_mem (unsigned nops, int opc, BrigType16_t t)
  : hsa_insn_basic (nops, opc, t)
{
  equiv_class = 0;
  memoryorder = BRIG_MEMORY_ORDER_NONE;
  memoryscope = BRIG_MEMORY_SCOPE_NONE;
}

/* New operator of memory access instruction representation.  */

void *
hsa_insn_mem::operator new (size_t)
{
  return hsa_allocp_inst_mem->allocate ();
}

/* Constructor of class representing atomic instructions. OPC is the prinicpa;
   opcode, aop is the specific atomic operation opcode.  T is the type of the
   instruction.  */

hsa_insn_atomic::hsa_insn_atomic (int opc, enum BrigAtomicOperation aop,
				  BrigType16_t t)
  : hsa_insn_mem (4, opc, t)
{
  gcc_checking_assert (opc == BRIG_OPCODE_ATOMICNORET ||
		       opc == BRIG_OPCODE_ATOMIC);
  atomicop = aop;
}

/* New operator for class representing atomic instructions. */

void *
hsa_insn_atomic::operator new (size_t)
{
  return hsa_allocp_inst_atomic->allocate ();
}

/* Constructor of class representing segment conversion instructions.  OPC is
   the opcode which must be either BRIG_OPCODE_STOF or BRIG_OPCODE_FTOS.  DESTT
   and SRCT are destination and source types respectively, SEG is the segment
   we are converting to or from.  */

hsa_insn_seg::hsa_insn_seg (int opc, BrigType16_t destt, BrigType16_t srct,
			    BrigSegment8_t seg)
  : hsa_insn_basic (2, opc, destt)
{
  gcc_checking_assert (opc == BRIG_OPCODE_STOF || opc == BRIG_OPCODE_FTOS);
  src_type = srct;
  segment = seg;
}

/* New operator for segment conversion instructions  */

void *
hsa_insn_seg::operator new (size_t)
{
  return hsa_allocp_inst_seg->allocate ();
}

/* Constructor of class representing a call instruction.  CALLEE is the tree
   representation of the function being called.  */

hsa_insn_call::hsa_insn_call (tree callee) : hsa_insn_basic (0, BRIG_OPCODE_CALL)
{
  called_function = callee;
  args_code_list = NULL;
  result_symbol = NULL;
  result_code_list = NULL;
}

/* Operator new for classes representing a call.  */

void *
hsa_insn_call::operator new (size_t)
{
  return hsa_allocp_inst_call->allocate ();
}

/* Constructor of class representing the argument block required to invoke
   a call in HSAIL.  */
hsa_insn_arg_block::hsa_insn_arg_block (BrigKind brig_kind,
					hsa_insn_call * call)
  : hsa_insn_basic (0, HSA_OPCODE_ARG_BLOCK), kind (brig_kind),
  call_insn (call)
{
}

/* New operator for classes representing HSAIL argument blocks.  */
void *
hsa_insn_arg_block::operator new (size_t)
{
  return hsa_allocp_inst_arg_block->allocate ();
}

/* Append HSA instruction INSN to basic block HBB.  */

static void
hsa_append_insn (hsa_bb *hbb, hsa_insn_basic *insn)
{
  /* Make sure we did not forget to set the kind.  */
  gcc_assert (insn->opcode != 0);
  gcc_assert (!insn->bb);

  insn->bb = hbb->bb;
  insn->prev = hbb->last_insn;
  insn->next = NULL;
  if (hbb->last_insn)
    hbb->last_insn->next = insn;
  hbb->last_insn = insn;
  if (!hbb->first_insn)
    hbb->first_insn = insn;
}

/* Insert HSA instruction NEW_INSN immediately before an existing instruction
   OLD_INSN.  */

static void
hsa_insert_insn_before (hsa_insn_basic *new_insn, hsa_insn_basic *old_insn)
{
  hsa_bb *hbb = hsa_bb_for_bb (old_insn->bb);

  if (hbb->first_insn == old_insn)
    hbb->first_insn = new_insn;
  new_insn->prev = old_insn->prev;
  new_insn->next = old_insn;
  if (old_insn->prev)
    old_insn->prev->next = new_insn;
  old_insn->prev = new_insn;
}

/* Append HSA instruction NEW_INSN immediately after an existing instruction
   OLD_INSN.  */

static void
hsa_append_insn_after (hsa_insn_basic *new_insn, hsa_insn_basic *old_insn)
{
  hsa_bb *hbb = hsa_bb_for_bb (old_insn->bb);

  if (hbb->last_insn == old_insn)
    hbb->last_insn = new_insn;
  new_insn->prev = old_insn;
  new_insn->next = old_insn->next;
  if (old_insn->next)
    old_insn->next->prev = new_insn;
  old_insn->next = new_insn;
}

/* Lookup or create a HSA pseudo register for a given gimple SSA name and if
   necessary, convert it to REQTYPE.  SSA_MAP is a vector mapping gimple
   SSA names to HSA registers.  Append an new conversion statements to HBB.  */

static hsa_op_reg *
hsa_reg_for_gimple_ssa_reqtype (tree ssa, vec <hsa_op_reg_p> *ssa_map,
				hsa_bb *hbb, BrigType16_t reqtype)
{
  hsa_op_reg *reg = hsa_reg_for_gimple_ssa (ssa, ssa_map);
  if (hsa_needs_cvt (reqtype, reg->type))
    {
      hsa_op_reg *converted = new hsa_op_reg (reqtype);
      hsa_insn_basic *insn = new hsa_insn_basic (2, BRIG_OPCODE_CVT, reqtype);
      insn->operands[0] = converted;
      insn->operands[1] = reg;
      reg->uses.safe_push (insn);
      hsa_append_insn (hbb, insn);
      return converted;
    }

  return reg;
}

/* Return a register containing the calculated value of EXP which must be an
   expression consisting of PLUS_EXPRs, MULT_EXPRs, NOP_EXPRs, SSA_NAMEs and
   integer constants as returned by get_inner_reference.  SSA_MAP is used to
   lookup HSA equivalent of SSA_NAMEs, newly generated HSA instructions will be
   appended to HBB.  Perform all calculations in ADDRTYPE.  If NEW_USE is
   non-NULL, any register result is going to have it appended to the list of
   uses.  */

static hsa_op_with_type *
gen_address_calculation (tree exp, hsa_bb *hbb, vec <hsa_op_reg_p> *ssa_map,
			 BrigType16_t addrtype, hsa_insn_basic *new_use)
{
  int opcode;

  if (TREE_CODE (exp) == NOP_EXPR)
    exp =TREE_OPERAND (exp, 0);

  switch (TREE_CODE (exp))
    {
    case SSA_NAME:
      {
	hsa_op_reg *res = hsa_reg_for_gimple_ssa_reqtype (exp, ssa_map, hbb,
							  addrtype);
	if (new_use)
	  res->uses.safe_push (new_use);
	return res;
      }

    case INTEGER_CST:
      {
       hsa_op_immed *imm = new hsa_op_immed (exp);
       if (addrtype != imm->type)
	 {
	   gcc_assert (hsa_type_bit_size (addrtype)
		       > hsa_type_bit_size (imm->type));
	   imm->type = addrtype;
	 }
       return imm;
      }

    case PLUS_EXPR:
      opcode = BRIG_OPCODE_ADD;
      break;

    case MULT_EXPR:
      opcode = BRIG_OPCODE_MUL;
      break;

    default:
      gcc_unreachable ();
    }

  hsa_op_reg *res = new hsa_op_reg (addrtype);
  hsa_insn_basic *insn = new hsa_insn_basic (3, opcode, addrtype);
  insn->operands[0] = res;
  set_reg_def (res, insn);

  hsa_op_with_type *op1 = gen_address_calculation (TREE_OPERAND (exp, 0), hbb,
						   ssa_map, addrtype, insn);
  hsa_op_with_type *op2 = gen_address_calculation (TREE_OPERAND (exp, 1), hbb,
						   ssa_map, addrtype, insn);
  insn->operands[1] = op1;
  insn->operands[2] = op2;

  hsa_append_insn (hbb, insn);
  if (new_use)
    res->uses.safe_push (new_use);
  return res;
}

/* If R1 is NULL, just return R2, otherwise append an instruction adding them
   to HBB and return the register holding the result.  */

static hsa_op_reg *
add_addr_regs_if_needed (hsa_op_reg *r1, hsa_op_reg *r2, hsa_bb *hbb)
{
  gcc_checking_assert (r2);
  if (!r1)
    return r2;

  hsa_op_reg *res = new hsa_op_reg (r1->type);
  gcc_assert (!hsa_needs_cvt (r1->type, r2->type));
  hsa_insn_basic *insn = new hsa_insn_basic (3, BRIG_OPCODE_ADD, res->type);
  insn->operands[0] = res;
  set_reg_def (res, insn);
  insn->operands[1] = r1;
  r1->uses.safe_push (insn);
  insn->operands[2] = r2;
  r2->uses.safe_push (insn);
  hsa_append_insn (hbb, insn);
  return res;
}

/* Helper of gen_hsa_addr.  Update *SYMBOL, *ADDRTYPE, *REG and *OFFSET to
   reflect BASE which is the first operand of a MEM_REF or a TARGET_MEM_REF.
   Use SSA_MAP to find registers corresponding to gimple SSA_NAMEs.  */

static void
process_mem_base (tree base, hsa_symbol **symbol, BrigType16_t *addrtype,
		  hsa_op_reg **reg, offset_int *offset, hsa_bb *hbb,
		  vec <hsa_op_reg_p> *ssa_map)
{
  if (TREE_CODE (base) == SSA_NAME)
    {
      gcc_assert (!*reg);
      *reg = hsa_reg_for_gimple_ssa_reqtype (base, ssa_map, hbb, *addrtype);
    }
  else if (TREE_CODE (base) == ADDR_EXPR)
    {
      tree decl = TREE_OPERAND (base, 0);

      gcc_checking_assert (DECL_P (decl));
      gcc_assert (!*symbol);
      *symbol = get_symbol_for_decl (decl);
      *addrtype = hsa_get_segment_addr_type ((*symbol)->segment);
    }
  else if (TREE_CODE (base) == INTEGER_CST)
    *offset += wi::to_offset (base);
  else
    gcc_unreachable ();
}

/* Generate HSA address operand for a given tree memory reference REF.  If
   instructions need to be created to calculate the address, they will be added
   to the end of HBB, SSA_MAP is an array mapping gimple SSA names to HSA
   pseudo-registers.  */

static hsa_op_address *
gen_hsa_addr (tree ref, hsa_bb *hbb, vec <hsa_op_reg_p> *ssa_map)
{
  hsa_symbol *symbol = NULL;
  hsa_op_reg *reg = NULL;
  offset_int offset = 0;
  tree origref = ref;
  tree varoffset = NULL_TREE;
  BrigType16_t addrtype = hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT);

  if (TREE_CODE (ref) == COMPONENT_REF)
    {
      tree fld = TREE_OPERAND (ref, 1);
      if (DECL_BIT_FIELD (fld))
	{
	  sorry ("Support for HSA does not implement references to "
		 "bit fields such as %D", fld);
	  goto out;
	}
    }
  else if (TREE_CODE (ref) == BIT_FIELD_REF
	   && ((tree_to_uhwi (TREE_OPERAND (ref, 1)) % BITS_PER_UNIT) != 0
	       || (tree_to_uhwi (TREE_OPERAND (ref, 2)) % BITS_PER_UNIT) != 0))
    {
      sorry ("Support for HSA does not implement bit field references "
	     "such as %E", ref);
      goto out;
    }

  if (handled_component_p (ref))
    {
      HOST_WIDE_INT bitsize, bitpos;
      enum machine_mode mode;
      int unsignedp, volatilep;

      ref = get_inner_reference (ref, &bitsize, &bitpos, &varoffset, &mode,
				 &unsignedp, &volatilep, false);

      if ((bitpos % BITS_PER_UNIT) != 0
	  || (bitsize % BITS_PER_UNIT) != 0)
	{
	  sorry ("Support for HSA does not implement bit field references "
		 "such as %E", origref);
	  goto out;
	}

      offset = bitpos;
      offset = wi::rshift (offset, LOG2_BITS_PER_UNIT, SIGNED);
    }

  switch (TREE_CODE (ref))
    {
    case SSA_NAME:
      /* The SSA_NAME and ADDR_EXPR cases cannot occur in a valid gimple memory
	 reference but we also use this function to generate addresses of
	 instructions representing operands of atomic memory access builtins
	 which are just addresses and not references.  */
      gcc_assert (!reg);
      reg = hsa_reg_for_gimple_ssa_reqtype (ref, ssa_map, hbb, addrtype);
      break;

    case ADDR_EXPR:
      ref = TREE_OPERAND (ref, 0);
      gcc_assert (DECL_P (ref));
      /* Fall-through. */
    case PARM_DECL:
    case VAR_DECL:
    case RESULT_DECL:
      gcc_assert (!symbol);
      symbol = get_symbol_for_decl (ref);
      addrtype = hsa_get_segment_addr_type (symbol->segment);
      break;

    case MEM_REF:
      process_mem_base (TREE_OPERAND (ref, 0), &symbol, &addrtype, &reg,
			&offset, hbb, ssa_map);

      if (!integer_zerop (TREE_OPERAND (ref, 1)))
	offset += wi::to_offset (TREE_OPERAND (ref, 1));
      break;

    case TARGET_MEM_REF:
      process_mem_base (TMR_BASE (ref), &symbol, &addrtype, &reg, &offset, hbb,
			ssa_map);
      if (TMR_INDEX (ref))
	{
	  hsa_op_reg *disp1, *idx;
	  idx = hsa_reg_for_gimple_ssa_reqtype (TMR_INDEX (ref), ssa_map, hbb,
						addrtype);
	  if (TMR_STEP (ref) && !integer_onep (TMR_STEP (ref)))
	    {
	      disp1 = new hsa_op_reg (addrtype);
	      hsa_insn_basic *insn = new hsa_insn_basic (3, BRIG_OPCODE_MUL,
							 addrtype);
	      insn->operands[0] = disp1;
	      set_reg_def (disp1, insn);
	      insn->operands[1] = idx;
	      idx->uses.safe_push (insn);
	      insn->operands[2] = new hsa_op_immed (TMR_STEP (ref));
	      hsa_append_insn (hbb, insn);
	    }
	  else
	    disp1 = idx;
	  reg = add_addr_regs_if_needed (reg, disp1, hbb);
	}
      if (TMR_INDEX2 (ref))
	{
	  hsa_op_reg *disp2;
	  disp2 = hsa_reg_for_gimple_ssa_reqtype (TMR_INDEX2 (ref), ssa_map,
						  hbb, addrtype);
	  reg = add_addr_regs_if_needed (reg, disp2, hbb);
	}
      offset += wi::to_offset (TMR_OFFSET (ref));
      break;

    default:
      sorry ("Support for HSA does not implement memory access to %E", origref);
      goto out;
    }

  if (varoffset)
    {
      if (TREE_CODE (varoffset) == INTEGER_CST)
	offset += wi::to_offset (varoffset);
      else
	{
	  hsa_op_base *off_op = gen_address_calculation (varoffset, hbb, ssa_map,
							 addrtype, NULL);
	  reg = add_addr_regs_if_needed (reg, as_a <hsa_op_reg *> (off_op), hbb);
	}
    }

  gcc_checking_assert ((symbol
			&& addrtype
			== hsa_get_segment_addr_type (symbol->segment))
		       || (!symbol
			   && addrtype
			   == hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT)));
out:
  HOST_WIDE_INT hwi_offset = offset.to_shwi ();

  return new hsa_op_address (symbol, reg, hwi_offset);
}

/* Generate HSA address for a function call argument of given TYPE.
   INDEX is used to generate corresponding name of the arguments.
   Special value -1 represents fact that result value is created.  */

static hsa_op_address *
gen_hsa_addr_for_arg (tree tree_type, int index)
{
  hsa_symbol *sym = hsa_allocp_symbols->allocate ();
  memset (sym, 0, sizeof (hsa_symbol));
  sym->segment = BRIG_SEGMENT_ARG;
  sym->linkage = BRIG_LINKAGE_ARG;

  sym->type = hsa_type_for_tree_type (tree_type, &sym->dim);

  if (index == -1) /* Function result.  */
    sym->name = "res";
  else /* Function call arguments.  */
    {
      sym->name = NULL;
      sym->name_number = index;
    }

  return new hsa_op_address (sym, NULL, 0);
}

/* Generate HSA instructions that calculate address of VAL including all
   necessary conversions to flat addressing and place the result into DEST.
   Instructions are appended to HBB.  SSA_MAP maps gimple SSA names to HSA
   pseudo registers.  */

static void
gen_hsa_addr_insns (tree val, hsa_op_reg *dest, hsa_bb *hbb,
		    vec <hsa_op_reg_p> *ssa_map)
{
  hsa_op_address *addr;
  hsa_insn_basic *insn = new hsa_insn_basic (2, BRIG_OPCODE_LDA);

  gcc_assert (dest->type == hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT));
  if (TREE_CODE (val) == ADDR_EXPR)
    val = TREE_OPERAND (val, 0);
  addr = gen_hsa_addr (val, hbb, ssa_map);
  insn->operands[1] = addr;
  if (addr->reg)
    addr->reg->uses.safe_push (insn);
  if (addr->symbol)
    {
      /* LDA produces segment-relative address, we need to convert
	 it to the flat one.  */
      hsa_op_reg *tmp;
      tmp = new hsa_op_reg (hsa_get_segment_addr_type (addr->symbol->segment));
      hsa_insn_seg *seg;
      seg = new hsa_insn_seg (BRIG_OPCODE_STOF,
			      hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT),
			      tmp->type, addr->symbol->segment);

      insn->operands[0] = tmp;
      set_reg_def (tmp, insn);
      insn->type = tmp->type;
      hsa_append_insn (hbb, insn);
      seg->operands[0] = dest;
      seg->operands[1] = tmp;
      set_reg_def (dest, seg);
      tmp->uses.safe_push (seg);
      hsa_append_insn (hbb, seg);
    }
  else
    {
      insn->operands[0] = dest;
      set_reg_def (dest, insn);
      insn->type = hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT);
      hsa_append_insn (hbb, insn);
    }
}

/* Return an HSA register or HSA immediate value operand corresponding to
   gimple operand OP.  SSA_MAP maps gimple SSA names to HSA pseudo registers.
   If DEF_INSN is non-NULL, a returned register will have its definition
   already set to DEF_INSN.  */

static hsa_op_with_type *
hsa_reg_or_immed_for_gimple_op (tree op, hsa_bb *hbb,
				vec <hsa_op_reg_p> *ssa_map,
				hsa_insn_basic *new_use)
{
  hsa_op_reg *tmp;

  if (TREE_CODE (op) == SSA_NAME)
    tmp = hsa_reg_for_gimple_ssa (op, ssa_map);
  else if (!POINTER_TYPE_P (TREE_TYPE (op)))
    return new hsa_op_immed (op);
  else
    {
      tmp = new hsa_op_reg (hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT));
      gen_hsa_addr_insns (op, tmp, hbb, ssa_map);
    }
  if (new_use)
    tmp->uses.safe_push (new_use);
  return tmp;
}

/* Create a simple movement instruction with register destination DEST and
   register or immediate source SRC and append it to the end of HBB.  */

void
hsa_build_append_simple_mov (hsa_op_reg *dest, hsa_op_base *src, hsa_bb *hbb)
{
  hsa_insn_basic *insn = new hsa_insn_basic (2, BRIG_OPCODE_MOV, dest->type);
  insn->operands[0] = dest;
  insn->operands[1] = src;
  if (hsa_op_reg *sreg = dyn_cast <hsa_op_reg *> (src))
    {
      gcc_assert (hsa_type_bit_size (dest->type)
		  == hsa_type_bit_size (sreg->type));
      sreg->uses.safe_push (insn);
    }
  else
    gcc_assert (hsa_type_bit_size (dest->type)
		== hsa_type_bit_size (as_a <hsa_op_immed *> (src)->type));
  set_reg_def (dest, insn);
  hsa_append_insn (hbb, insn);
}

/* Generate HSAIL instructions loading something into register DEST.  RHS is
   tree representation of the loaded data, which are loaded as type TYPE.  Add
   instructions to HBB, use SSA_MAP for HSA SSA lookup.  */

static void
gen_hsa_insns_for_load (hsa_op_reg *dest, tree rhs, tree type, hsa_bb *hbb,
			vec <hsa_op_reg_p> *ssa_map)
{
  /* The destination SSA name will give us the type.  */
  if (TREE_CODE (rhs) == VIEW_CONVERT_EXPR)
    rhs = TREE_OPERAND (rhs, 0);

  if (TREE_CODE (rhs) == SSA_NAME)
    {
      hsa_op_reg *src = hsa_reg_for_gimple_ssa (rhs, ssa_map);
      hsa_build_append_simple_mov (dest, src, hbb);
    }
  else if (is_gimple_min_invariant (rhs)
	   || TREE_CODE (rhs) == ADDR_EXPR)
    {
      if (POINTER_TYPE_P (TREE_TYPE (rhs)))
	{
	  if (dest->type != hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT))
	    {
	      sorry ("Support for HSA does not implement conversion of %E to "
		     "the requested non-pointer type.", rhs);
	      return;
	    }
	  gen_hsa_addr_insns (rhs, dest, hbb, ssa_map);
	}
      else
	{
	  hsa_op_immed *imm = new hsa_op_immed (rhs);
	  hsa_build_append_simple_mov (dest, imm, hbb);
	}
    }
  else if (DECL_P (rhs) || TREE_CODE (rhs) == MEM_REF
	   || TREE_CODE (rhs) == TARGET_MEM_REF
	   || handled_component_p (rhs))
    {
      /* Load from memory.  */
      hsa_op_address *addr;
      BrigType16_t mtype;
      /* Not dest->type, that's possibly extended.  */
      mtype = mem_type_for_type (hsa_type_for_scalar_tree_type (type, false));
      hsa_insn_mem *mem = new hsa_insn_mem (BRIG_OPCODE_LD, mtype);
      addr = gen_hsa_addr (rhs, hbb, ssa_map);
      mem->operands[0] = dest;
      mem->operands[1] = addr;
      set_reg_def (dest, mem);
      if (addr->reg)
	addr->reg->uses.safe_push (mem);
      hsa_append_insn (hbb, mem);
    }
  else
    sorry ("Support for HSA does not implement loading of expression %E",
	   rhs);
}

/* Generate HSAIL instructions storing into memory.  LHS is the destination of
   the store, SRC is the source operand.  Add instructions to HBB, use SSA_MAP
   for HSA SSA lookup.  */

static void
gen_hsa_insns_for_store (tree lhs, hsa_op_base *src, hsa_bb *hbb,
			 vec <hsa_op_reg_p> *ssa_map)
{
  BrigType16_t mtype;
  mtype = mem_type_for_type (hsa_type_for_scalar_tree_type (TREE_TYPE (lhs),
							    false));
  hsa_insn_mem *mem = new hsa_insn_mem (BRIG_OPCODE_ST, mtype);
  hsa_op_address *addr;
  addr = gen_hsa_addr (lhs, hbb, ssa_map);
  if (hsa_op_reg *reg = dyn_cast <hsa_op_reg *> (src))
    reg->uses.safe_push (mem);

  /* XXX The HSAIL disasm has another constraint: if the source
     is an immediate then it must match the destination type.  If
     it's a register the low bits will be used for sub-word stores.
     We're always allocating new operands so we can modify the above
     in place.  */
  if (hsa_op_immed *imm = dyn_cast <hsa_op_immed *> (src))
    {
      if ((imm->type & BRIG_TYPE_PACK_MASK) == BRIG_TYPE_PACK_NONE)
	imm->type = mem->type;
      else
	{
	  /* ...and all vector immediates apparently need to be vectors of
	     unsigned bytes. */
	  unsigned bs = hsa_type_bit_size (imm->type);
	  gcc_assert (bs == hsa_type_bit_size (mem->type));
	  switch (bs)
	    {
	    case 32:
	      imm->type = BRIG_TYPE_U8X4;
	      break;
	    case 64:
	      imm->type = BRIG_TYPE_U8X8;
	      break;
	    case 128:
	      imm->type = BRIG_TYPE_U8X16;
	      break;
	    default:
	      gcc_unreachable ();
	    }
	}
    }

  mem->operands[0] = src;
  mem->operands[1] = addr;
  if (addr->reg)
    addr->reg->uses.safe_push (mem);
  hsa_append_insn (hbb, mem);
}

/* Generate HSA instructions for a single assignment.  HBB is the basic block
   they will be appended to.  SSA_MAP maps gimple SSA names to HSA pseudo
   registers.  */

static void
gen_hsa_insns_for_single_assignment (gimple assign, hsa_bb *hbb,
				     vec <hsa_op_reg_p> *ssa_map)
{
  tree lhs = gimple_assign_lhs (assign);
  tree rhs = gimple_assign_rhs1 (assign);

  if (gimple_clobber_p (assign))
    ;
  else if (TREE_CODE (lhs) == SSA_NAME)
    {
      hsa_op_reg *dest = hsa_reg_for_gimple_ssa (lhs, ssa_map);
      gen_hsa_insns_for_load (dest, rhs, TREE_TYPE (lhs), hbb, ssa_map);
    }
  else if (TREE_CODE (rhs) == SSA_NAME
	   || is_gimple_min_invariant (rhs))
    {
      /* Store to memory.  */
      hsa_op_base *src = hsa_reg_or_immed_for_gimple_op (rhs, hbb, ssa_map,
							 NULL);
      gen_hsa_insns_for_store (lhs, src, hbb, ssa_map);
    }
  else
    {
      gcc_assert (!is_gimple_reg_type (TREE_TYPE (lhs)));
      sorry ("Support for HSA does not implement non-scalar memory moves.");
    }
}

/* Prepend before INSN a load from spill symbol of SPILL_REG.  Return the
   register into which we loaded.  If this required another register to convert
   from a B1 type, return it in *PTMP2, otherwise store NULL into it.  We
   assume we are out of SSA so the returned register does not have its
   definition set.  */

hsa_op_reg *
hsa_spill_in (hsa_insn_basic *insn, hsa_op_reg *spill_reg, hsa_op_reg **ptmp2)
{
  hsa_symbol *spill_sym = spill_reg->spill_sym;
  hsa_insn_mem *mem = new hsa_insn_mem (BRIG_OPCODE_LD, spill_sym->type);
  hsa_op_reg *reg = new hsa_op_reg (spill_sym->type);
  hsa_op_address *addr = new hsa_op_address (spill_sym, NULL, 0);

  mem->operands[0] = reg;
  mem->operands[1] = addr;
  hsa_insert_insn_before (mem, insn);

  *ptmp2 = NULL;
  if (spill_reg->type == BRIG_TYPE_B1)
    {
      hsa_insn_basic *cvtinsn;
      *ptmp2 = reg;
      reg = new hsa_op_reg (spill_reg->type);

      cvtinsn = new hsa_insn_basic (2, BRIG_OPCODE_CVT, reg->type);
      cvtinsn->operands[0] = reg;
      cvtinsn->operands[1] = *ptmp2;

      hsa_insert_insn_before (cvtinsn, insn);
    }
  return reg;
}

/* Append after INSN a store to spill symbol of SPILL_REG.  Return the register
   from which we stored.  If this required another register to convert to a B1
   type, return it in *PTMP2, otherwise store NULL into it.  We assume we are
   out of SSA so the returned register does not have its use updated.  */

hsa_op_reg *
hsa_spill_out (hsa_insn_basic *insn, hsa_op_reg *spill_reg, hsa_op_reg **ptmp2)
{
  hsa_symbol *spill_sym = spill_reg->spill_sym;
  hsa_insn_mem *mem = new hsa_insn_mem (BRIG_OPCODE_ST, spill_sym->type);
  hsa_op_reg *reg = new hsa_op_reg (spill_sym->type);
  hsa_op_address *addr = new hsa_op_address (spill_sym, NULL, 0);
  hsa_op_reg *returnreg;

  *ptmp2 = NULL;
  returnreg = reg;
  if (spill_reg->type == BRIG_TYPE_B1)
    {
      hsa_insn_basic *cvtinsn;
      *ptmp2 = new hsa_op_reg (spill_sym->type);
      reg->type = spill_reg->type;

      cvtinsn = new hsa_insn_basic (2, BRIG_OPCODE_CVT, spill_sym->type);
      cvtinsn->operands[0] = *ptmp2;
      cvtinsn->operands[1] = returnreg;

      hsa_append_insn_after (cvtinsn, insn);
      insn = cvtinsn;
      reg = *ptmp2;
    }

  mem->operands[0] = reg;
  mem->operands[1] = addr;
  hsa_append_insn_after (mem, insn);
  return returnreg;
}

/* Generate a comparison instruction that will compare LHS and RHS with
   comparison specified by CODE and put result into register DEST.  DEST has to
   have its type set already but must not have its definition set yet.
   Generated instructions will be added to HBB, SSA_MAP maps gimple SSA names
   to HSA pseudo registers.  */

static void
gen_hsa_cmp_insn_from_gimple (enum tree_code code, tree lhs, tree rhs,
			      hsa_op_reg *dest, hsa_bb *hbb,
			      vec <hsa_op_reg_p> *ssa_map)
{
  BrigCompareOperation8_t compare;

  switch (code)
    {
    case LT_EXPR:
      compare = BRIG_COMPARE_LT;
      break;
    case LE_EXPR:
      compare = BRIG_COMPARE_LE;
      break;
    case GT_EXPR:
      compare = BRIG_COMPARE_GT;
      break;
    case GE_EXPR:
      compare = BRIG_COMPARE_GE;
      break;
    case EQ_EXPR:
      compare = BRIG_COMPARE_EQ;
      break;
    case NE_EXPR:
      compare = BRIG_COMPARE_NE;
      break;
    default:
      sorry ("Support for HSA does not implement comparison tree code %s\n",
	     get_tree_code_name (code));
      return;
    }

  hsa_insn_cmp *cmp = new hsa_insn_cmp (compare, dest->type);
  cmp->operands[0] = dest;
  set_reg_def (dest, cmp);
  cmp->operands[1] = hsa_reg_or_immed_for_gimple_op (lhs, hbb, ssa_map, cmp);
  cmp->operands[2] = hsa_reg_or_immed_for_gimple_op (rhs, hbb, ssa_map, cmp);
  hsa_append_insn (hbb, cmp);
}

/* Generate HSA instruction for an assignment ASSIGN with an operation.
   Instructions will be appended to HBB.  SSA_MAP maps gimple SSA names to HSA
   pseudo registers.  */

static void
gen_hsa_insns_for_operation_assignment (gimple assign, hsa_bb *hbb,
					vec <hsa_op_reg_p> *ssa_map)
{
  int opcode;

  switch (gimple_assign_rhs_code (assign))
    {
    CASE_CONVERT:
    case FLOAT_EXPR:
      {
	/* HSA is a bit unforgiving with CVT, the types must not be
	   same sized without float/int conversion, otherwise we need
	   to use MOV.  */
	tree tl = TREE_TYPE (gimple_assign_lhs (assign));
	tree tr = TREE_TYPE (gimple_assign_rhs1 (assign));
	/* We don't need to check for float/float conversion of same size,
	   as that wouldn't result in a gimple instruction.  */
	if ((INTEGRAL_TYPE_P (tl) || POINTER_TYPE_P (tl))
	    && (INTEGRAL_TYPE_P (tr) || POINTER_TYPE_P (tr))
	    && TYPE_SIZE (tl) == TYPE_SIZE (tr))
	  opcode = BRIG_OPCODE_MOV;
	else
	  opcode = BRIG_OPCODE_CVT;
      }
      break;

    case PLUS_EXPR:
    case POINTER_PLUS_EXPR:
      opcode = BRIG_OPCODE_ADD;
      break;
    case MINUS_EXPR:
      opcode = BRIG_OPCODE_SUB;
      break;
    case MULT_EXPR:
      opcode = BRIG_OPCODE_MUL;
      break;
    case MULT_HIGHPART_EXPR:
      opcode = BRIG_OPCODE_MULHI;
      break;
    case RDIV_EXPR:
    case TRUNC_DIV_EXPR:
    case EXACT_DIV_EXPR:
      opcode = BRIG_OPCODE_DIV;
      break;
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case ROUND_DIV_EXPR:
      sorry ("Support for HSA does not implement CEIL_DIV_EXPR, FLOOR_DIV_EXPR "
	     "or ROUND_DIV_EXPR");
      return;
    case TRUNC_MOD_EXPR:
      opcode = BRIG_OPCODE_REM;
      break;
    case CEIL_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case ROUND_MOD_EXPR:
      sorry ("Support for HSA does not implement CEIL_MOD_EXPR, FLOOR_MOD_EXPR "
	     "or ROUND_MOD_EXPR");
      return;
    case NEGATE_EXPR:
      opcode = BRIG_OPCODE_NEG;
      break;
    case MIN_EXPR:
      opcode = BRIG_OPCODE_MIN;
      break;
    case MAX_EXPR:
      opcode = BRIG_OPCODE_MAX;
      break;
    case ABS_EXPR:
      opcode = BRIG_OPCODE_ABS;
      break;
    case LSHIFT_EXPR:
      opcode = BRIG_OPCODE_SHL;
      break;
    case RSHIFT_EXPR:
      opcode = BRIG_OPCODE_SHR;
      break;
    case LROTATE_EXPR:
    case RROTATE_EXPR:
      sorry ("Support for HSA does not implement LROTATE_EXPR or RROTATE_EXPR");
      return;
    case BIT_IOR_EXPR:
      opcode = BRIG_OPCODE_OR;
      break;
    case BIT_XOR_EXPR:
      opcode = BRIG_OPCODE_XOR;
      break;
    case BIT_AND_EXPR:
      opcode = BRIG_OPCODE_AND;
      break;
    case BIT_NOT_EXPR:
      opcode = BRIG_OPCODE_NOT;
      break;

    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
      {
	hsa_op_reg *dest = hsa_reg_for_gimple_ssa (gimple_assign_lhs (assign),
						   ssa_map);
	gen_hsa_cmp_insn_from_gimple (gimple_assign_rhs_code (assign),
				      gimple_assign_rhs1 (assign),
				      gimple_assign_rhs2 (assign),
				      dest, hbb, ssa_map);
      }
      return;

    default:
      /* Implement others as we come across them.  */
      sorry ("Support for HSA does not implement operation %s",
	     get_tree_code_name (gimple_assign_rhs_code (assign)));
      return;
    }

  unsigned nops;
  switch (get_gimple_rhs_class (gimple_expr_code (assign)))
    {
    case GIMPLE_TERNARY_RHS:
      nops = 4;
      break;
    case GIMPLE_BINARY_RHS:
      nops = 3;
      break;
    case GIMPLE_UNARY_RHS:
      nops = 2;
      break;
    default:
      gcc_unreachable ();
    }

  hsa_op_reg *dest = hsa_reg_for_gimple_ssa (gimple_assign_lhs (assign),
					     ssa_map);
  /* FIXME: Allocate an instruction with modifiers if appropriate.  */
  hsa_insn_basic *insn = new hsa_insn_basic (nops, opcode, dest->type);
  insn->operands[0] = dest;
  set_reg_def (dest, insn);

  switch (get_gimple_rhs_class (gimple_expr_code (assign)))
    {
    case GIMPLE_TERNARY_RHS:
      /* FIXME: Implement.  */
      sorry ("Support for HSA does not implement ternary operations");
      return;

      /* Fall through */
    case GIMPLE_BINARY_RHS:
      {
	tree top = gimple_assign_rhs2 (assign);
	if ((opcode == BRIG_OPCODE_SHL || opcode == BRIG_OPCODE_SHR)
	    && TREE_CODE (top) != SSA_NAME)
	  top = build_int_cstu (unsigned_type_node, TREE_INT_CST_LOW (top));
	insn->operands[2] = hsa_reg_or_immed_for_gimple_op (top, hbb, ssa_map,
							    insn);
      }
      /* Fall through */
    case GIMPLE_UNARY_RHS:
      {
	tree top = gimple_assign_rhs1 (assign);
	if (opcode == BRIG_OPCODE_ABS || opcode == BRIG_OPCODE_NEG)
	  {
	    /* ABS and NEG only exist in _s form :-/  */
	    if (insn->type == BRIG_TYPE_U32)
	      insn->type = BRIG_TYPE_S32;
	    else if (insn->type == BRIG_TYPE_U64)
	      insn->type = BRIG_TYPE_S64;
	  }
	insn->operands[1] = hsa_reg_or_immed_for_gimple_op (top, hbb, ssa_map,
							    insn);
      }
      break;
    default:
      gcc_unreachable ();
    }

  hsa_append_insn (hbb, insn);
}

/* Generate HSA instructions for a given gimple condition statement COND.
   Instructions will be appended to HBB, which also needs to be the
   corresponding structure to the basic_block of COND.  SSA_MAP maps gimple SSA
   names to HSA pseudo registers.  */

static void
gen_hsa_insns_for_cond_stmt (gimple cond, hsa_bb *hbb,
			     vec <hsa_op_reg_p> *ssa_map)
{
  hsa_op_reg *ctrl = new hsa_op_reg (BRIG_TYPE_B1);
  hsa_insn_br *cbr;

  gen_hsa_cmp_insn_from_gimple (gimple_cond_code (cond),
				gimple_cond_lhs (cond),
				gimple_cond_rhs (cond),
				ctrl, hbb, ssa_map);

  cbr = new hsa_insn_br (ctrl);
  hsa_append_insn (hbb, cbr);
}

/* Generate HSA instructions for a direct call instruction.
   Instructions will be appended to HBB, which also needs to be the
   corresponding structure to the basic_block of STMT. SSA_MAP maps gimple SSA
   names to HSA pseudo registers.  */

static void
gen_hsa_insns_for_direct_call (gimple stmt, hsa_bb *hbb,
			       vec <hsa_op_reg_p> *ssa_map)
{
  hsa_insn_call *call_insn = new hsa_insn_call (gimple_call_fndecl (stmt));
  hsa_cfun->called_functions.safe_push (call_insn->called_function);

  /* Argument block start.  */
  hsa_insn_arg_block *arg_start = new hsa_insn_arg_block
    (BRIG_KIND_DIRECTIVE_ARG_BLOCK_START, call_insn);
  hsa_append_insn (hbb, arg_start);

  /* Preparation of arguments that will be passed to function.  */
  const unsigned args = gimple_call_num_args (stmt);
  for (unsigned i = 0; i < args; ++i)
    {
      tree parm = gimple_call_arg (stmt, (int)i);
      BrigType16_t mtype = mem_type_for_type (hsa_type_for_scalar_tree_type
					      (TREE_TYPE (parm), false));
      hsa_insn_mem *mem = new hsa_insn_mem (BRIG_OPCODE_ST, mtype);
      hsa_op_base *src = hsa_reg_or_immed_for_gimple_op (parm, hbb, ssa_map,
							 mem);
      hsa_op_address *addr;
      addr = gen_hsa_addr_for_arg (TREE_TYPE (parm), i);
      mem->operands[0] = src;
      mem->operands[1] = addr;

      call_insn->input_args.safe_push (addr->symbol);
      hsa_append_insn (hbb, mem);

      call_insn->args_symbols.safe_push (addr->symbol);
    }

  call_insn->args_code_list = new hsa_op_code_list (args);
  hsa_append_insn (hbb, call_insn);

  tree result_type = TREE_TYPE (TREE_TYPE (gimple_call_fndecl (stmt)));

  tree result = gimple_call_lhs (stmt);
  hsa_insn_mem *result_insn = NULL;
  if (!VOID_TYPE_P (result_type))
    {
      hsa_op_address *addr = gen_hsa_addr_for_arg (result_type, -1);

      /* Even if result of a function call is unused, we have to emit
	 declaration for the result.  */
      if (result)
	{
	  BrigType16_t mtype = mem_type_for_type
	    (hsa_type_for_scalar_tree_type (TREE_TYPE (result), false));
	  result_insn = new hsa_insn_mem (BRIG_OPCODE_LD, mtype);
	  hsa_op_reg *dst = hsa_reg_for_gimple_ssa (result, ssa_map);

	  result_insn->operands[0] = dst;
	  result_insn->operands[1] = addr;
	  set_reg_def (dst, result_insn);

	  hsa_append_insn (hbb, result_insn);
	}

      call_insn->output_arg = addr->symbol;
      call_insn->result_symbol = addr->symbol;
      call_insn->result_code_list = new hsa_op_code_list (1);
    }
  else
    call_insn->result_code_list = new hsa_op_code_list (0);

  /* Argument block start.  */
  hsa_insn_arg_block *arg_end = new hsa_insn_arg_block
    (BRIG_KIND_DIRECTIVE_ARG_BLOCK_END, call_insn);
  hsa_append_insn (hbb, arg_end);
}

/* Generate HSA instructions for a return value instruction.
   Instructions will be appended to HBB, which also needs to be the
   corresponding structure to the basic_block of STMT. SSA_MAP maps gimple SSA
   names to HSA pseudo registers.  */

static void
gen_hsa_insns_for_return (greturn *stmt, hsa_bb *hbb,
			vec <hsa_op_reg_p> *ssa_map)
{
  tree retval = gimple_return_retval (stmt);
  if (retval)
    {
      /* Store of return value.  */
      BrigType16_t mtype = mem_type_for_type
	(hsa_type_for_scalar_tree_type (TREE_TYPE (retval), false));
      hsa_insn_mem *mem = new hsa_insn_mem (BRIG_OPCODE_ST, mtype);
      hsa_op_base *src = hsa_reg_or_immed_for_gimple_op (retval, hbb, ssa_map,
							 mem);

      hsa_op_address *addr = new hsa_op_address (hsa_cfun->output_arg, NULL, 0);
      mem->operands[0] = src;
      mem->operands[1] = addr;
      hsa_append_insn (hbb, mem);
    }

  /* HSAIL return instruction emission.  */
  hsa_insn_basic *ret = new hsa_insn_basic (0, BRIG_OPCODE_RET);
  hsa_append_insn (hbb, ret);
}

/* If STMT is a call of a known library function, generate code to perform
   it and return true.  */

static bool
gen_hsa_insns_for_known_library_call (gimple stmt, hsa_bb *hbb,
				      vec <hsa_op_reg_p> *ssa_map)
{
  tree decl = gimple_call_fndecl (stmt);
  const char *name = get_declaration_name (decl);

  if (!strcmp (name, "omp_is_initial_device")
      || strcmp (name, "omp_is_initial_device"))
    {
      tree lhs = gimple_call_lhs (stmt);
      if (!lhs)
	return true;

      hsa_op_reg *dest = hsa_reg_for_gimple_ssa (lhs, ssa_map);
      hsa_op_immed *imm = new hsa_op_immed (build_zero_cst (TREE_TYPE (lhs)));

      hsa_build_append_simple_mov (dest, imm, hbb);
      return true;
    }
  return false;
}

/* Generate HSA instructions for the given call statement STMT.  Instructions
   will be appended to HBB.  SSA_MAP maps gimple SSA names to HSA pseudo
   registers. */

static void
gen_hsa_insns_for_call (gimple stmt, hsa_bb *hbb,
			vec <hsa_op_reg_p> *ssa_map)
{
  tree lhs = gimple_call_lhs (stmt);
  hsa_op_reg *dest;
  hsa_insn_basic *insn;
  int opcode;

  if (!gimple_call_builtin_p (stmt, BUILT_IN_NORMAL))
    {
      if (lookup_attribute ("hsafunc",
			    DECL_ATTRIBUTES (gimple_call_fndecl (stmt))))
        gen_hsa_insns_for_direct_call (stmt, hbb, ssa_map);
      else if (!gen_hsa_insns_for_known_library_call (stmt, hbb, ssa_map))
	sorry ("HSA does support only call for functions with 'hsafunc' "
	       "attribute");
      return;
    }

  switch (DECL_FUNCTION_CODE (gimple_call_fndecl (stmt)))
    {
    case BUILT_IN_OMP_GET_THREAD_NUM:
      opcode = BRIG_OPCODE_WORKITEMABSID;
      goto specialop;

    case BUILT_IN_OMP_GET_NUM_THREADS:
      opcode = BRIG_OPCODE_GRIDSIZE;
      goto specialop;

specialop:
      {
	hsa_op_reg *tmp;
	dest = hsa_reg_for_gimple_ssa (lhs, ssa_map);
	/* We're using just one-dimensional kernels, so hard-coded
	   dimension X.  */
	hsa_op_immed *imm = new hsa_op_immed (build_zero_cst (uint32_type_node));
	insn = new hsa_insn_basic (2, opcode);
	if (dest->type != BRIG_TYPE_U32)
	  tmp = new hsa_op_reg (BRIG_TYPE_U32);
	else
	  tmp = dest;
	insn->operands[0] = tmp;
	insn->operands[1] = imm;
	insn->type = tmp->type;
	hsa_append_insn (hbb, insn);
	if (dest != tmp)
	  {
	    int opc2 = dest->type == BRIG_TYPE_S32 ? BRIG_OPCODE_MOV
	      : BRIG_OPCODE_CVT;
	    insn = new hsa_insn_basic (2, opc2, dest->type);
	    insn->operands[0] = dest;
	    insn->operands[1] = tmp;
	    hsa_append_insn (hbb, insn);
	  }
	set_reg_def (dest, insn);
	break;
      }

    case BUILT_IN_SQRT:
    case BUILT_IN_SQRTF:
      /* FIXME: Since calls without a LHS are not removed, double check that
	 they cannot have side effects.  */
      if (!lhs)
	return;
      dest = hsa_reg_for_gimple_ssa (lhs, ssa_map);
      insn = new hsa_insn_basic (2, BRIG_OPCODE_SQRT, dest->type);
      insn->operands[0] = dest;
      set_reg_def (dest, insn);
      insn->operands[1]
	= hsa_reg_or_immed_for_gimple_op (gimple_call_arg (stmt, 0),
					  hbb, ssa_map, insn);
      hsa_append_insn (hbb, insn);
      break;

    case BUILT_IN_ATOMIC_LOAD_1:
    case BUILT_IN_ATOMIC_LOAD_2:
    case BUILT_IN_ATOMIC_LOAD_4:
    case BUILT_IN_ATOMIC_LOAD_8:
    case BUILT_IN_ATOMIC_LOAD_16:
      {
	/* XXX Ignore mem model for now.  */
	BrigType16_t mtype = mem_type_for_type (hsa_type_for_scalar_tree_type
						(TREE_TYPE (lhs), false));
	hsa_insn_mem *meminsn = new hsa_insn_mem (BRIG_OPCODE_LD, mtype);
	hsa_op_address *addr;
	addr = gen_hsa_addr (gimple_call_arg (stmt, 0), hbb, ssa_map);
	dest = hsa_reg_for_gimple_ssa (lhs, ssa_map);

	/* Should check what the memory scope is */
	meminsn->memoryscope = BRIG_MEMORY_SCOPE_WORKGROUP;
	meminsn->operands[0] = dest;
	meminsn->operands[1] = addr;
	meminsn->memoryorder = BRIG_MEMORY_ORDER_SC_ACQUIRE;

	set_reg_def (dest, meminsn);
	if (addr->reg)
	  addr->reg->uses.safe_push (meminsn);
	hsa_append_insn (hbb, meminsn);
	break;
      }

    case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_1:
    case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_2:
    case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_4:
    case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_8:
    case BUILT_IN_SYNC_VAL_COMPARE_AND_SWAP_16:
      {
	/* XXX Ignore mem model for now.  */
	BrigType16_t atype  = hsa_bittype_for_type
	  (hsa_type_for_scalar_tree_type (TREE_TYPE (lhs), false));
	hsa_insn_atomic *atominsn = new hsa_insn_atomic (BRIG_OPCODE_ATOMIC,
							 BRIG_ATOMIC_CAS, atype);
	hsa_op_address *addr;
	addr = gen_hsa_addr (gimple_call_arg (stmt, 0), hbb, ssa_map);
	dest = hsa_reg_for_gimple_ssa (lhs, ssa_map);

	/* Should check what the memory scope is */
	atominsn->memoryscope = BRIG_MEMORY_SCOPE_WORKGROUP;
	atominsn->operands[0] = dest;
	atominsn->operands[1] = addr;
	atominsn->operands[2]
	  = hsa_reg_or_immed_for_gimple_op (gimple_call_arg (stmt, 1),
					    hbb, ssa_map, atominsn);
	atominsn->operands[3]
	  = hsa_reg_or_immed_for_gimple_op (gimple_call_arg (stmt, 2),
					    hbb, ssa_map, atominsn);
	atominsn->memoryorder = BRIG_MEMORY_ORDER_SC_ACQUIRE_RELEASE;

	set_reg_def (dest, atominsn);
	if (addr->reg)
	  addr->reg->uses.safe_push (atominsn);
	hsa_append_insn (hbb, atominsn);
	break;
      }

    default:
      sorry ("Support for HSA does not implement calls to builtin %D",
	     gimple_call_fndecl (stmt));
      return;
    }
}

/* Generate HSA instructions for a given gimple statement.  Instructions will be
   appended to HBB.  SSA_MAP maps gimple SSA names to HSA pseudo registers.  */

static void
gen_hsa_insns_for_gimple_stmt (gimple stmt, hsa_bb *hbb,
			       vec <hsa_op_reg_p> *ssa_map)
{
  switch (gimple_code (stmt))
    {
    case GIMPLE_ASSIGN:
      if (gimple_assign_single_p (stmt))
	gen_hsa_insns_for_single_assignment (stmt, hbb, ssa_map);
      else
	gen_hsa_insns_for_operation_assignment (stmt, hbb, ssa_map);
      break;
    case GIMPLE_RETURN:
      gen_hsa_insns_for_return (as_a <greturn *> (stmt), hbb, ssa_map);
      break;
    case GIMPLE_COND:
      gen_hsa_insns_for_cond_stmt (stmt, hbb, ssa_map);
      break;
    case GIMPLE_CALL:
      gen_hsa_insns_for_call (stmt, hbb, ssa_map);
      break;
    case GIMPLE_DEBUG:
      /* ??? HSA supports some debug facilities.  */
      break;
    case GIMPLE_LABEL:
    {
      tree label = gimple_label_label (as_a <glabel *> (stmt));
      if (FORCED_LABEL (label))
	sorry ("Support for HSA does not implement gimple label with address "
	       "taken");

      break;
    }
    default:
      sorry ("Support for HSA does not implement gimple statement %s",
	     gimple_code_name[(int) gimple_code (stmt)]);
    }
}

/* Generate a HSA PHI from a gimple PHI.  SSA_MAP maps gimple SSA names to HSA
   pseudo registers.  */

static void
gen_hsa_phi_from_gimple_phi (gimple gphi, hsa_bb *hbb,
			     vec <hsa_op_reg_p> *ssa_map)
{
  hsa_insn_phi *hphi;
  unsigned count = gimple_phi_num_args (gphi);

  hphi = new hsa_insn_phi (count);
  hphi->bb = hbb->bb;
  hphi->dest = hsa_reg_for_gimple_ssa (gimple_phi_result (gphi), ssa_map);
  set_reg_def (hphi->dest, hphi);

  for (unsigned i = 0; i < count; i++)
    {
      tree op = gimple_phi_arg_def (gphi, i);
      if (TREE_CODE (op) == SSA_NAME)
	{
	  hsa_op_reg *hreg = hsa_reg_for_gimple_ssa (op, ssa_map);
	  hphi->operands[i] = hreg;
	  hreg->uses.safe_push (hphi);
	}
      else
	{
	  gcc_assert (is_gimple_min_invariant (op));
	  if (!POINTER_TYPE_P (TREE_TYPE (op)))
	    hphi->operands[i] = new hsa_op_immed (op);
	  else
	    {
	      sorry ("Support for HSA does not handle PHI nodes with constant "
		     "address operands");
	      return;
	    }
	}
    }

  hphi->prev = hbb->last_phi;
  hphi->next = NULL;
  if (hbb->last_phi)
    hbb->last_phi->next = hphi;
  hbb->last_phi = hphi;
  if (!hbb->first_phi)
    hbb->first_phi = hphi;
}

/* Constructor of class containing HSA-specific information about a basic
   block.  CFG_BB is the CFG BB this HSA BB is associated with.  IDX is the new
   index of this BB (so that the constructor does not attempt to use
   hsa_cfun during its construction).  */

hsa_bb::hsa_bb (basic_block cfg_bb, int idx)
{
  gcc_assert (!cfg_bb->aux);
  cfg_bb->aux = this;
  bb = cfg_bb;
  first_insn = last_insn = NULL;
  first_phi = last_phi = NULL;
  index = idx;
  livein = BITMAP_ALLOC (NULL);
  liveout = BITMAP_ALLOC (NULL);
}

/* Constructor of class containing HSA-specific information about a basic
   block.  CFG_BB is the CFG BB this HSA BB is associated with.  */

hsa_bb::hsa_bb (basic_block cfg_bb)
{
  gcc_assert (!cfg_bb->aux);
  cfg_bb->aux = this;
  bb = cfg_bb;
  first_insn = last_insn = NULL;
  first_phi = last_phi = NULL;
  index = hsa_cfun->hbb_count++;
  livein = BITMAP_ALLOC (NULL);
  liveout = BITMAP_ALLOC (NULL);
}

/* New operator of class with HSA information about a basic block.  */
void *
hsa_bb::operator new (size_t)
{
  return hsa_allocp_bb->allocate ();
}

/* Destructor of class representing HSA BB.  */

hsa_bb::~hsa_bb ()
{
  BITMAP_FREE (livein);
  BITMAP_FREE (liveout);
}

/* Create and initialize and return a new hsa_bb structure for a given CFG
   basic block BB.  */

hsa_bb *
hsa_init_new_bb (basic_block bb)
{
  return new hsa_bb (bb);
}

/* Go over gimple representation and generate our internal HSA one.  SSA_MAP
   maps gimple SSA names to HSA pseudo registers.  */

static void
gen_body_from_gimple (vec <hsa_op_reg_p> *ssa_map)
{
  basic_block bb;

  FOR_EACH_BB_FN (bb, cfun)
    {
      gimple_stmt_iterator gsi;
      hsa_bb *hbb = hsa_init_new_bb (bb);

      for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
	{
	  gen_hsa_insns_for_gimple_stmt (gsi_stmt (gsi), hbb, ssa_map);
	  if (seen_error ())
	    return;
	}
    }

  FOR_EACH_BB_FN (bb, cfun)
    {
      gimple_stmt_iterator gsi;
      hsa_bb *hbb = hsa_bb_for_bb (bb);

      for (gsi = gsi_start_phis (bb); !gsi_end_p (gsi); gsi_next (&gsi))
	if (!virtual_operand_p (gimple_phi_result (gsi_stmt (gsi))))
	  gen_hsa_phi_from_gimple_phi (gsi_stmt (gsi), hbb, ssa_map);
    }

  if (dump_file)
    {
      fprintf (dump_file, "------- Generated SSA form -------\n");
      dump_hsa_cfun (dump_file);
    }
}

/* For a function DECL, get number of arguments.  */

static unsigned
get_function_arg_count (tree decl)
{
  unsigned count = 0;

  for (tree parm = TYPE_ARG_TYPES (TREE_TYPE (decl)); parm;
       parm = TREE_CHAIN (parm))
    count++;

  /* Return type is the last element of tree list.  */
  return count - 1;
}

static void
gen_function_decl_parameters (hsa_function_representation *f,
			      tree decl)
{
  tree parm;
  unsigned i;

  f->input_args_count = get_function_arg_count (decl);
  f->input_args = XCNEWVEC (hsa_symbol, f->input_args_count);
  for (parm = TYPE_ARG_TYPES (TREE_TYPE (decl)), i = 0;
       parm;
       parm = TREE_CHAIN (parm), i++)
    {
      /* Result type if last in the tree list.  */
      if (i == f->input_args_count)
	break;

      tree v = TREE_VALUE (parm);
      f->input_args[i].type = hsa_type_for_tree_type (v, &f->input_args[i].dim);
      f->input_args[i].segment = BRIG_SEGMENT_ARG;
      f->input_args[i].linkage = BRIG_LINKAGE_NONE;
      f->input_args[i].name_number = i;
    }

  tree result_type = TREE_TYPE (TREE_TYPE (decl));
  if (!VOID_TYPE_P (result_type))
    {
      f->output_arg = XCNEW (hsa_symbol);
      f->output_arg->type = hsa_type_for_tree_type (result_type,
						   &f->output_arg->dim);
      f->output_arg->segment = BRIG_SEGMENT_ARG;
      f->output_arg->linkage = BRIG_LINKAGE_NONE;
      f->output_arg->name = "res";
    }
}

/* Generate the vector of parameters of the HSA representation of the current
   function.  This also includes the output parameter representing the
   result.  */

static void
gen_function_def_parameters (hsa_function_representation *f,
			     vec <hsa_op_reg_p> *ssa_map)
{
  tree parm;
  int i, count = 0;

  for (parm = DECL_ARGUMENTS (cfun->decl); parm; parm = DECL_CHAIN (parm))
    count++;

  ENTRY_BLOCK_PTR_FOR_FN (cfun)->aux = &f->prologue;
  f->prologue.bb = ENTRY_BLOCK_PTR_FOR_FN (cfun);

  f->input_args_count = count;
  f->input_args = XCNEWVEC (hsa_symbol, f->input_args_count);
  for (parm = DECL_ARGUMENTS (cfun->decl), i = 0;
       parm;
       parm = DECL_CHAIN (parm), i++)
    {
      struct hsa_symbol **slot;

      fillup_sym_for_decl (parm, &f->input_args[i]);
      f->input_args[i].segment = f->kern_p ? BRIG_SEGMENT_KERNARG :
				       BRIG_SEGMENT_ARG;

      f->input_args[i].linkage = BRIG_LINKAGE_FUNCTION;
      if (!DECL_NAME (parm))
	{
	  /* FIXME: Just generate some UID.  */
	  sorry ("Support for HSA does not implement anonymous C++ parameters");
	  return;
	}
      f->input_args[i].name = IDENTIFIER_POINTER (DECL_NAME (parm));
      slot = f->local_symbols->find_slot (&f->input_args[i],
						INSERT);
      gcc_assert (!*slot);
      *slot = &f->input_args[i];

      if (is_gimple_reg (parm))
	{
	  tree ddef = ssa_default_def (cfun, parm);
	  if (ddef && !has_zero_uses (ddef))
	    {
	      BrigType16_t mtype = mem_type_for_type
		(hsa_type_for_scalar_tree_type (TREE_TYPE (ddef), false));
	      hsa_insn_mem *mem = new hsa_insn_mem (BRIG_OPCODE_LD, mtype);
	      hsa_op_reg *dest = hsa_reg_for_gimple_ssa (ddef, ssa_map);
	      hsa_op_address *addr;

	      addr = gen_hsa_addr (parm, &hsa_cfun->prologue, ssa_map);
	      mem->operands[0] = dest;
	      mem->operands[1] = addr;
	      set_reg_def (dest, mem);
	      gcc_assert (!addr->reg);
	      hsa_append_insn (&f->prologue, mem);
	    }
	}
    }

  if (!VOID_TYPE_P (TREE_TYPE (TREE_TYPE (cfun->decl))))
    {
      struct hsa_symbol **slot;

      f->output_arg = XCNEW (hsa_symbol);
      fillup_sym_for_decl (DECL_RESULT (cfun->decl), f->output_arg);
      f->output_arg->segment = BRIG_SEGMENT_ARG;
      f->output_arg->linkage = BRIG_LINKAGE_FUNCTION;
      f->output_arg->name = "res";
      slot = f->local_symbols->find_slot (f->output_arg, INSERT);
      gcc_assert (!*slot);
      *slot = f->output_arg;
    }
}

/* Generate function representation that corresponds to
   a function declaration.  */

hsa_function_representation *
hsa_generate_function_declaration (tree decl)
{
  hsa_function_representation *fun = XCNEW (hsa_function_representation);

  fun->declaration_p = true;
  fun->decl = decl;
  fun->name = xstrdup (get_declaration_name (decl));

  gen_function_decl_parameters (fun, decl);

  return fun;
}

/* Generate HSAIL representation of the current function and write into a
   special section of the output file.  If KERNEL is set, the function will be
   considered an HSA kernel callable from the host, otherwise it will be
   compiled as an HSA function callable from other HSA code.  */

static unsigned int
generate_hsa (bool kernel)
{
  vec <hsa_op_reg_p> ssa_map = vNULL;

  hsa_init_data_for_cfun ();
  hsa_cfun->decl = cfun->decl;
  hsa_cfun->kern_p = kernel;

  ssa_map.safe_grow_cleared (SSANAMES (cfun)->length ());
  hsa_cfun->name
    = xstrdup (get_declaration_name (current_function_decl));
  hsa_sanitize_name (hsa_cfun->name);

  if (hsa_cfun->kern_p)
    hsa_add_kern_decl_mapping (current_function_decl, hsa_cfun->name);

  gen_function_def_parameters (hsa_cfun, &ssa_map);
  if (seen_error ())
    goto fail;
  gen_body_from_gimple (&ssa_map);
  if (seen_error ())
    goto fail;

#ifdef ENABLE_CHECKING
  for (unsigned i = 0; i < ssa_map.length (); i++)
    if (ssa_map[i])
      ssa_map[i]->verify ();
#endif

  ssa_map.release ();

  hsa_regalloc ();

  hsa_brig_emit_function ();

 fail:
  hsa_deinit_data_for_cfun ();
  return 0;
}

static GTY(()) tree hsa_launch_fn;
static GTY(()) tree hsa_dim_array_type;
static GTY(()) tree hsa_lattrs_dimnum_decl;
static GTY(()) tree hsa_lattrs_grid_decl;
static GTY(()) tree hsa_lattrs_group_decl;
static GTY(()) tree hsa_lattrs_nargs_decl;
static GTY(()) tree hsa_launch_attributes_type;

static void
init_hsa_functions (void)
{
  if (hsa_launch_fn)
    return;

  tree dim_arr_index_type;
  dim_arr_index_type = build_index_type (build_int_cst (integer_type_node, 2));
  hsa_dim_array_type = build_array_type (uint32_type_node, dim_arr_index_type);

  hsa_launch_attributes_type = make_node (RECORD_TYPE);
  hsa_lattrs_dimnum_decl = build_decl (BUILTINS_LOCATION, FIELD_DECL,
				       get_identifier ("ndim"),
				       uint32_type_node);
  DECL_CHAIN (hsa_lattrs_dimnum_decl) = NULL_TREE;

  hsa_lattrs_grid_decl = build_decl (BUILTINS_LOCATION, FIELD_DECL,
				    get_identifier ("global_size"),
				    hsa_dim_array_type);
  DECL_CHAIN (hsa_lattrs_grid_decl) = hsa_lattrs_dimnum_decl;
  hsa_lattrs_group_decl = build_decl (BUILTINS_LOCATION, FIELD_DECL,
				     get_identifier ("group_size"),
				     hsa_dim_array_type);
  DECL_CHAIN (hsa_lattrs_group_decl) = hsa_lattrs_grid_decl;
  hsa_lattrs_nargs_decl = build_decl (BUILTINS_LOCATION, FIELD_DECL,
				      get_identifier ("nargs"),
				      uint32_type_node);
  DECL_CHAIN (hsa_lattrs_nargs_decl) = hsa_lattrs_group_decl;
  finish_builtin_struct (hsa_launch_attributes_type, "__hsa_launch_attributes",
			 hsa_lattrs_nargs_decl, NULL_TREE);
  tree launch_fn_type;
  launch_fn_type
    = build_function_type_list (void_type_node, ptr_type_node,
				build_pointer_type (hsa_launch_attributes_type),
				build_pointer_type (uint64_type_node),
				NULL_TREE);

  hsa_launch_fn = build_fn_decl ("__hsa_launch_kernel", launch_fn_type);
}

/* Insert before the current statement in GSI a store of VALUE to INDEX of
   array (of type hsa_dim_array_type) FLD_DECL of RANGE_VAR.  VALUE must be of
   type uint32_type_node.  */

static void
insert_store_range_dim (gimple_stmt_iterator *gsi, tree range_var,
			tree fld_decl, int index, tree value)
{
  tree ref = build4 (ARRAY_REF, uint32_type_node,
		     build3 (COMPONENT_REF, hsa_dim_array_type,
			     range_var, fld_decl, NULL_TREE),
		     build_int_cst (integer_type_node, index),
		     NULL_TREE, NULL_TREE);
  gsi_insert_before (gsi, gimple_build_assign (ref, value), GSI_SAME_STMT);
}

/* Generate call to invoke kernel implementing function FNDECL.  */

static void
wrap_hsa_kernel_call (gimple_stmt_iterator *gsi, tree fndecl)
{
  init_hsa_functions ();

  bool real_kern_p = lookup_attribute ("hsakernel", DECL_ATTRIBUTES (fndecl));
  tree grid_size_1, group_size_1;
  tree u32_one = build_int_cst (uint32_type_node, 1);
  gimple call_stmt = gsi_stmt (*gsi);
  unsigned discard_arguents, num_args = gimple_call_num_args (call_stmt);
  if (real_kern_p)
    {
      discard_arguents = 2;
      if (num_args < 2)
	{
	  error ("Calls to functions with hsakernel attribute must "
		 "have at least two arguments.");
	  grid_size_1 = group_size_1 = u32_one;
	}
      else
	{
	  grid_size_1 = fold_convert (uint32_type_node,
				      gimple_call_arg (call_stmt, num_args - 2));
	  grid_size_1 = force_gimple_operand_gsi (gsi, grid_size_1, true,
						  NULL_TREE, true,
						  GSI_SAME_STMT);
	  group_size_1 = fold_convert (uint32_type_node,
				       gimple_call_arg (call_stmt,
							num_args - 1));
	  group_size_1 = force_gimple_operand_gsi (gsi, group_size_1, true,
						   NULL_TREE, true,
						   GSI_SAME_STMT);
	}
    }
  else
    {
      discard_arguents = 0;
      grid_size_1 = build_int_cst (uint32_type_node, 64);
      group_size_1 = build_int_cst (uint32_type_node, 64);
    }

  tree lattrs = create_tmp_var (hsa_launch_attributes_type,
				"__hsa_launch_attrs");
  tree dimref = build3 (COMPONENT_REF, uint32_type_node,
			lattrs, hsa_lattrs_dimnum_decl, NULL_TREE);
  gsi_insert_before (gsi, gimple_build_assign (dimref, u32_one), GSI_SAME_STMT);
  insert_store_range_dim (gsi, lattrs, hsa_lattrs_grid_decl, 0,
			  grid_size_1);
  insert_store_range_dim (gsi, lattrs, hsa_lattrs_grid_decl, 1,
			  u32_one);
  insert_store_range_dim (gsi, lattrs, hsa_lattrs_grid_decl, 2,
			  u32_one);
  insert_store_range_dim (gsi, lattrs, hsa_lattrs_group_decl, 0,
			  group_size_1);
  insert_store_range_dim (gsi, lattrs, hsa_lattrs_group_decl, 1,
			  u32_one);
  insert_store_range_dim (gsi, lattrs, hsa_lattrs_group_decl, 2,
			  u32_one);
  tree nargsref = build3 (COMPONENT_REF, uint32_type_node,
			 lattrs, hsa_lattrs_nargs_decl, NULL_TREE);
  tree nargsval = build_int_cst (uint32_type_node, num_args - discard_arguents);
  gsi_insert_before (gsi, gimple_build_assign (nargsref, nargsval),
		     GSI_SAME_STMT);
  lattrs = build_fold_addr_expr (lattrs);

  tree args;
  args = create_tmp_var (build_array_type_nelts (uint64_type_node,
						 num_args - discard_arguents),
			 NULL);

  gcc_assert (num_args >= discard_arguents);
  for (unsigned i = 0; i < (num_args - discard_arguents); i++)
    {
      tree arg = gimple_call_arg (call_stmt, i);
      gimple g;

      tree r = build4 (ARRAY_REF, uint64_type_node, args,
		       size_int (i), NULL_TREE, NULL_TREE);

      arg = force_gimple_operand_gsi (gsi, fold_convert (uint64_type_node, arg),
				      true, NULL_TREE, true, GSI_SAME_STMT);
      g = gimple_build_assign (r, arg);
      gsi_insert_before (gsi, g, GSI_SAME_STMT);
    }

  args = build_fold_addr_expr (args);

  /* XXX doesn't handle calls with lhs, doesn't remove EH
     edges.  */
  gimple launch = gimple_build_call (hsa_launch_fn, 3,
				     build_fold_addr_expr (fndecl),
				     lattrs, args);
  gsi_insert_before (gsi, launch, GSI_SAME_STMT);
  unlink_stmt_vdef (call_stmt);
  gsi_remove (gsi, true);
}

/* Replace calls of functions which have been turned into HSA kernels into
   their invocation via HSA run-time.  */

static unsigned int
wrap_all_hsa_calls (void)
{
  bool changed = false;
  basic_block bb;
  FOR_EACH_BB_FN (bb, cfun)
    {
      gimple_stmt_iterator gsi;
      tree fndecl;
      for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi);)
	if (is_gimple_call (gsi_stmt (gsi))
	    && (fndecl = gimple_call_fndecl (gsi_stmt (gsi)))
	    && (lookup_attribute ("hsa", DECL_ATTRIBUTES (fndecl))
		|| lookup_attribute ("hsakernel", DECL_ATTRIBUTES (fndecl))))
	  {
	    wrap_hsa_kernel_call (&gsi, fndecl);
	    changed = true;
	  }
	else
	  gsi_next (&gsi);
    }
  return changed ? TODO_cleanup_cfg | TODO_update_ssa : 0;
}

namespace {

const pass_data pass_data_gen_hsail =
{
  GIMPLE_PASS,
  "hsagen",	 			/* name */
  OPTGROUP_NONE,                        /* optinfo_flags */
  TV_NONE,				/* tv_id */
  PROP_cfg | PROP_ssa,                  /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0					/* todo_flags_finish */
};

class pass_gen_hsail : public gimple_opt_pass
{
public:
  pass_gen_hsail (gcc::context *ctxt)
    : gimple_opt_pass(pass_data_gen_hsail, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *);
  unsigned int execute (function *);

}; // class pass_gen_hsail

/* Determine whether or not to run generation of HSAIL.  */

bool
pass_gen_hsail::gate (function *)
{
#ifdef ENABLE_HSA
  return !flag_disable_hsa;
#else
  return false;
#endif
}

unsigned int
pass_gen_hsail::execute (function *)
{
  if (cgraph_node::get_create (current_function_decl)->offloadable
      || lookup_attribute ("hsa", DECL_ATTRIBUTES (current_function_decl))
      || lookup_attribute ("hsakernel",
			   DECL_ATTRIBUTES (current_function_decl)))
    return generate_hsa (true);
  else if (lookup_attribute ("hsafunc",
			     DECL_ATTRIBUTES (current_function_decl)))
    return generate_hsa (false);
  else
    return wrap_all_hsa_calls ();
}

} // anon namespace

/* Create the instance of hsa gen pass.  */

gimple_opt_pass *
make_pass_gen_hsail (gcc::context *ctxt)
{
  return new pass_gen_hsail (ctxt);
}

#include "gt-hsa-gen.h"
