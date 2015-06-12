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

/* TODO: Some of the following includes might be redundand because of ongoing
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

/* Structure containing intermediate HSA representation of the generated
   function. */
struct hsa_function_representation hsa_cfun;

/* Element of the mapping vector between a host decl and an HSA kernel.  */

struct GTY(()) hsa_decl_kernel_map_element
{
  /* The decl of the host function.  */
  tree decl;
  /* Name of the HSA kernel in BRIG.  */
  char * GTY((skip)) name;
};

/* Mapping between decls and corresponding HSA kernels in this compilation
   unit.  */
static GTY (()) vec<hsa_decl_kernel_map_element, va_gc> *hsa_decl_kernel_mapping;

/* Alloc pools for allocating basic hsa structures such as operands,
   instructions and other basic entitie.s */
static pool_allocator<hsa_op_address> *hsa_allocp_operand_address;
static pool_allocator<hsa_op_immed> *hsa_allocp_operand_immed;
static pool_allocator<hsa_op_reg> *hsa_allocp_operand_reg;
static pool_allocator<hsa_op_code_list> *hsa_allocp_operand_code_list;
static pool_allocator<hsa_insn_basic> *hsa_allocp_inst_basic;
static pool_allocator<hsa_insn_phi> *hsa_allocp_inst_phi;
static pool_allocator<hsa_insn_mem> *hsa_allocp_inst_mem;
static pool_allocator<hsa_insn_atomic> *hsa_allocp_inst_atomic;
static pool_allocator<hsa_insn_addr> *hsa_allocp_inst_addr;
static pool_allocator<hsa_insn_seg> *hsa_allocp_inst_seg;
static pool_allocator<hsa_insn_cmp> *hsa_allocp_inst_cmp;
static pool_allocator<hsa_insn_br> *hsa_allocp_inst_br;
static pool_allocator<hsa_insn_call> *hsa_allocp_inst_call;
static pool_allocator<hsa_insn_call_block> *hsa_allocp_inst_call_block;
static pool_allocator<hsa_bb> *hsa_allocp_bb;
static pool_allocator<hsa_symbol> *hsa_allocp_symbols;

/* Vectors with selected instructions and operands that need
   a destruction.  */
static vec <hsa_op_code_list *> hsa_list_operand_code_list;
static vec <hsa_op_reg *> hsa_list_operand_reg;
static vec <hsa_insn_call_block *> hsa_list_insn_call_block;
static vec <hsa_insn_call *> hsa_list_insn_call;

/* Hash function to lookup a symbol for a decl.  */
hash_table <hsa_free_symbol_hasher> *hsa_global_variable_symbols;

/* True if compilation unit-wide data are already allocated and initialized.  */
static bool compilation_unit_data_initialized;

/* Allocate HSA structures that are are used when dealing with different
   functions.  */

void
hsa_init_compilation_unit_data (void)
{
  if (compilation_unit_data_initialized)
    return;

  compilation_unit_data_initialized = true;
  hsa_global_variable_symbols = new hash_table <hsa_free_symbol_hasher> (8);
}

/* Free data structures that are used when dealing with different
   functions.  */

void
hsa_deinit_compilation_unit_data (void)
{
  if (!compilation_unit_data_initialized)
    return;

  delete hsa_global_variable_symbols;
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
    = new pool_allocator<hsa_op_immed> ("HSA immedate operands", 32);
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
  hsa_allocp_inst_addr
    = new pool_allocator<hsa_insn_addr> ("HSA address instructions", 32);
  hsa_allocp_inst_seg
    = new pool_allocator<hsa_insn_seg> ("HSA segment conversion instructions",
					16);
  hsa_allocp_inst_cmp
    = new pool_allocator<hsa_insn_cmp> ("HSA comparison instructions", 16);
  hsa_allocp_inst_br
    = new pool_allocator<hsa_insn_br> ("HSA branching instructions", 16);
  hsa_allocp_inst_call
    = new pool_allocator<hsa_insn_call> ("HSA call instructions", 16);
  hsa_allocp_inst_call_block
    = new pool_allocator<hsa_insn_call_block> ("HSA call block instructions",
					       16);
  hsa_allocp_bb = new pool_allocator<hsa_bb> ("HSA basic blocks", 8);

  sym_init_len = (vec_safe_length (cfun->local_decls) / 2) + 1;
  hsa_allocp_symbols = new pool_allocator<hsa_symbol> ("HSA symbols",
						       sym_init_len);
  memset (&hsa_cfun, 0, sizeof (hsa_cfun));
  hsa_cfun.prologue.label_ref.kind = BRIG_KIND_OPERAND_CODE_REF;
  hsa_cfun.local_symbols
    = new hash_table <hsa_noop_symbol_hasher> (sym_init_len);
  hsa_cfun.reg_count = 0;
  hsa_cfun.hbb_count = 1;       /* 0 is for prologue.  */
  hsa_cfun.in_ssa = true;	/* We start in SSA.  */
}

/* Deinitialize HSA subsysterm and free all allocated memory.  */

static void
hsa_deinit_data_for_cfun (void)
{
  basic_block bb;

  FOR_EACH_BB_FN (bb, cfun)
    if (bb->aux)
      {
	BITMAP_FREE (hsa_bb_for_bb (bb)->livein);
	BITMAP_FREE (hsa_bb_for_bb (bb)->liveout);
	bb->aux = NULL;
      }

  for (unsigned int i = 0; i < hsa_list_operand_code_list.length (); i++)
    hsa_list_operand_code_list[i]->~hsa_op_code_list ();

  for (unsigned int i = 0; i < hsa_list_operand_reg.length (); i++)
    hsa_list_operand_reg[i]->~hsa_op_reg ();

  for (unsigned int i = 0; i < hsa_list_insn_call_block.length (); i++)
    hsa_list_insn_call_block[i]->~hsa_insn_call_block ();

  for (unsigned int i = 0; i < hsa_list_insn_call.length (); i++)
    hsa_list_insn_call[i]->~hsa_insn_call ();

  hsa_list_operand_code_list.release ();
  hsa_list_operand_reg.release ();
  hsa_list_insn_call_block.release ();
  hsa_list_insn_call.release ();

  delete hsa_allocp_operand_address;
  delete hsa_allocp_operand_immed;
  delete hsa_allocp_operand_reg;
  delete hsa_allocp_operand_code_list;
  delete hsa_allocp_inst_basic;
  delete hsa_allocp_inst_phi;
  delete hsa_allocp_inst_atomic;
  delete hsa_allocp_inst_mem;
  delete hsa_allocp_inst_addr;
  delete hsa_allocp_inst_seg;
  delete hsa_allocp_inst_cmp;
  delete hsa_allocp_inst_br;
  delete hsa_allocp_inst_call;
  delete hsa_allocp_inst_call_block;
  delete hsa_allocp_bb;

  delete hsa_allocp_symbols;
  delete hsa_cfun.local_symbols;
  free (hsa_cfun.input_args);
  free (hsa_cfun.output_arg);
  /* Kernel names are deallocated at the end of BRIG output when deallocating
     hsa_decl_kernel_mapping.  */
  if (!hsa_cfun.kern_p)
    free (hsa_cfun.name);
  hsa_cfun.spill_symbols.release();
}

/* Return true if we are generating large HSA machine model.  */

bool
hsa_machine_large_p (void)
{
  /* FIXME: I suppose this is techically wrong but should work for me now.  */
  return (GET_MODE_BITSIZE (Pmode) == 64);
}

/* Return the HSA profile we are using.  */

bool
hsa_full_profile_p (void)
{
  return true;
}

/* Return the type which holds addresses in the given SEGMENT.  */

BrigType16_t
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

/* Return true if operand number OPNUM of instruction with OPCODE is an output.
   False if it is an input.  */

bool
hsa_opcode_op_output_p (BrigOpcode16_t opcode, int opnum)
{
  switch (opcode)
    {
    case BRIG_OPCODE_CBR:
    case BRIG_OPCODE_ST:
      /* FIXME: There are probably missing cases here, double check.  */
      return false;
    default:
     return opnum == 0;
    }
}

/* Return HSA type for tree TYPE, which has to fit into BrigType16_t.  Pointers
   are assumed to use flat addressing.  */

static BrigType16_t
hsa_type_for_scalar_tree_type (const_tree type, bool for_operand)
{
  HOST_WIDE_INT bsize;
  const_tree base;
  BrigType16_t res = BRIG_TYPE_NONE;

  gcc_checking_assert (TYPE_P (type));
  if (POINTER_TYPE_P (type))
    /* Don't use hsa_get_segment_addr_type here, it gives addresses
       as B32/B64, but we need U32/U64.  */
    return hsa_machine_large_p () ? BRIG_TYPE_U64 : BRIG_TYPE_U32;

  /* XXX ARRAY_TYPEs need to decend into pointers, at least for
     global variables.  Same for RECORD_TYPES.  */
  if (TREE_CODE (type) == VECTOR_TYPE || TREE_CODE (type) == ARRAY_TYPE)
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
  else if (for_operand)
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

static bool
hsa_type_float_p (BrigType16_t type)
{
  switch (type & BRIG_TYPE_BASE_MASK)
    {
    case BRIG_TYPE_F16:
    case BRIG_TYPE_F32:
    case BRIG_TYPE_F64:
      return true;
    default:
      return false;
    }
}

/* Returns true if converting from STYPE into DTYPE needs the _CVT
   opcode.  If false a normal _MOV is enough.  */

static bool
hsa_needs_cvt (BrigType16_t dtype, BrigType16_t stype)
{
  /* float <-> int conversions are real converts.  */
  if (hsa_type_float_p (dtype) != hsa_type_float_p (stype))
    return true;
  /* When both types have different size (equivalent to different
     underlying bit types), then we need CVT as well.  */
  if (bittype_for_type (dtype) != bittype_for_type (stype))
    return true;
  return false;
}

/* Return HSA type for tree TYPE.  If it cannot fit into BrigType16_t, some
   kind of array will be generated, setting DIMLO and DIMHI appropriately.
   Otherwise, they will be set to zero.  */

static BrigType16_t
hsa_type_for_tree_type (const_tree type, uint32_t *dimLo, uint32_t *dimHi,
			bool for_operand)
{
  /* FIXME: Not yet implemented for non-scalars.  */
  *dimLo = 0;
  *dimHi = 0;
  return hsa_type_for_scalar_tree_type (type, for_operand);
}

/* Fill in those values into SYM according to DECL, which are determined
   independently from whether it is parameter, result, or a variable, local or
   global.  */

static void
fillup_sym_for_decl (tree decl, struct hsa_symbol *sym)
{
  sym->decl = decl;
  sym->type = hsa_type_for_tree_type (TREE_TYPE (decl), &sym->dimLo,
				      &sym->dimHi, false);
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
      warning (0, "referring to global symbol %q+D by name from HSA code won't work", decl);
    }
  else
    {
      slot = hsa_cfun.local_symbols->find_slot (&dummy, INSERT);
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
  if (!DECL_NAME (decl))
    sorry ("Support for HSA does not implement anonymous declarations");
  else
    sym->name = IDENTIFIER_POINTER (DECL_NAME (decl));
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
  hsa_cfun.spill_symbols.safe_push(sym);
  return sym;
}

/* Allocate and clear a hsa_op_immed and set it to TREE_VAL.  */

static hsa_op_immed *
hsa_alloc_immed_op (tree tree_val)
{
  hsa_op_immed *imm = hsa_allocp_operand_immed->allocate ();

  gcc_checking_assert (is_gimple_min_invariant (tree_val)
		       && !POINTER_TYPE_P (TREE_TYPE (tree_val)));

  memset (imm, 0 , sizeof (hsa_op_immed));
  imm->kind = BRIG_KIND_OPERAND_CONSTANT_BYTES;
  imm->type = hsa_type_for_scalar_tree_type (TREE_TYPE (tree_val), true);
  imm->value = tree_val;

  return imm;
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

/* Allocate, clear and return a hsa_op_reg.  */

static hsa_op_reg *
hsa_alloc_reg_op (void)
{
  hsa_op_reg *hreg;

  hreg = hsa_allocp_operand_reg->allocate ();
  hsa_list_operand_reg.safe_push (hreg);
  memset (hreg, 0, sizeof (hsa_op_reg));
  hreg->kind = BRIG_KIND_OPERAND_REGISTER;
  /* TODO: Try removing later on.  I suppose this is not necessary but I'd
     rather avoid surprises.  */
  hreg->order = hsa_cfun.reg_count++;
  return hreg;
}

/* Allocate and set up a new address operand consisting of base symbol SYM,
   register REG ans immediate OFFSET.  If the machine model is not large and
   offset is 64 bit, the upper, 32 bits have to be zero.  */

static hsa_op_address *
hsa_alloc_addr_op (hsa_symbol *sym, hsa_op_reg *reg, HOST_WIDE_INT offset)
{
  hsa_op_address *addr;

  addr = hsa_allocp_operand_address->allocate ();
  memset (addr, 0, sizeof (hsa_op_address));
  addr->kind = BRIG_KIND_OPERAND_ADDRESS;
  addr->symbol = sym;
  addr->reg = reg;
  addr->imm_offset = offset;
  return addr;
}

/* Allocate and set up a new code list operands with given number
   of ELEMENTS.  */

static hsa_op_code_list *
hsa_alloc_code_list_op (unsigned elements)
{
  hsa_op_code_list *list;
  list = hsa_allocp_operand_code_list->allocate ();
  hsa_list_operand_code_list.safe_push (list);

  memset (list, 0, sizeof (hsa_op_code_list));
  list->kind = BRIG_KIND_OPERAND_CODE_LIST;
  list->offsets.create (1);
  list->offsets.safe_grow_cleared (elements);

  return list;
}

/* Lookup or create a HSA pseudo register for a given gimple SSA name.  */

static hsa_op_reg *
hsa_reg_for_gimple_ssa (tree ssa, vec <hsa_op_reg_p> ssa_map)
{
  hsa_op_reg *hreg;

  gcc_checking_assert (TREE_CODE (ssa) == SSA_NAME);
  if (ssa_map[SSA_NAME_VERSION (ssa)])
    return ssa_map[SSA_NAME_VERSION (ssa)];

  hreg = hsa_alloc_reg_op ();
  hreg->type = hsa_type_for_scalar_tree_type (TREE_TYPE (ssa), true);
  hreg->gimple_ssa = ssa;
  ssa_map[SSA_NAME_VERSION (ssa)] = hreg;

  return hreg;
}

/* Set the defining instruction of REG to be INSN.  When checking, make sure it
   was not set before.  */

static inline void
set_reg_def (hsa_op_reg *reg, hsa_insn_basic *insn)
{
  if (hsa_cfun.in_ssa)
    {
      gcc_checking_assert (!reg->def_insn);
      reg->def_insn = insn;
    }
  else
    reg->def_insn = NULL;
}

/* Allocate, clear and return a new basic instruction.  Note that it cannot set
   any opcode.  */

static hsa_insn_basic *
hsa_alloc_basic_insn (void)
{
  hsa_insn_basic *hin;

  hin = hsa_allocp_inst_basic->allocate ();
  memset (hin, 0, sizeof (hsa_insn_basic));
  return hin;
}

/* Allocate, clear and return a memory instruction structure.  */

static hsa_insn_mem *
hsa_alloc_mem_insn (void)
{
  hsa_insn_mem *mem;

  mem = hsa_allocp_inst_mem->allocate ();
  memset (mem, 0, sizeof (hsa_insn_mem));
  return mem;
}

/* Allocate, clear and return a memory instruction structure.  */

static hsa_insn_atomic *
hsa_alloc_atomic_insn (void)
{
  hsa_insn_atomic *ret;

  ret = hsa_allocp_inst_atomic->allocate ();
  memset (ret, 0, sizeof (hsa_insn_atomic));
  return ret;
}

/* Allocate, clear and return an address instruction structure.  */

static hsa_insn_addr *
hsa_alloc_addr_insn (void)
{
  hsa_insn_addr *addr;

  addr = hsa_allocp_inst_addr->allocate ();
  memset (addr, 0, sizeof (hsa_insn_addr));
  return addr;
}

/* Allocate, clear and return a segment conversion instruction structure.  */

static hsa_insn_seg *
hsa_alloc_seg_insn (void)
{
  hsa_insn_seg *seg;

  seg = hsa_allocp_inst_seg->allocate ();
  memset (seg, 0, sizeof (hsa_insn_seg));
  return seg;
}

/* Allocate, clear and return a comprison instruction structure.  Also sets the
   opcode.  */

static hsa_insn_cmp *
hsa_alloc_cmp_insn (void)
{
  hsa_insn_cmp *cmp;

  cmp = hsa_allocp_inst_cmp->allocate ();
  memset (cmp, 0, sizeof (hsa_insn_cmp));
  cmp->opcode = BRIG_OPCODE_CMP;
  return cmp;
}

/* Allocate, clear, fill in and return a comprison instruction structure.  Also
   sets the opcode.  CTRL is the controlling register.  */

static hsa_insn_br *
hsa_build_cbr_insn (hsa_op_reg *ctrl)
{
  hsa_insn_br *cbr;

  cbr = hsa_allocp_inst_br->allocate ();
  memset (cbr, 0, sizeof (hsa_insn_br));
  cbr->opcode = BRIG_OPCODE_CBR;
  cbr->type = BRIG_TYPE_NONE;
  cbr->width =  BRIG_WIDTH_1;
  cbr->operands[0] = ctrl;
  ctrl->uses.safe_push (cbr);
  return cbr;
}

/* Allocate, clear and return a call instruction structure.  */

static hsa_insn_call *
hsa_alloc_call_insn (void)
{
  hsa_insn_call *call;

  call = hsa_allocp_inst_call->allocate ();
  hsa_list_insn_call.safe_push (call);
  memset (call, 0, sizeof (hsa_insn_call));
  return call;
}

/* Allocate, clear and return a arg block instruction structure.  */

static hsa_insn_call_block *
hsa_alloc_call_block_insn (void)
{
  hsa_insn_call_block *call_block;

  call_block = hsa_allocp_inst_call_block->allocate ();
  hsa_list_insn_call_block.safe_push (call_block);
  memset (call_block, 0, sizeof (hsa_insn_call_block));

  call_block->opcode = HSA_OPCODE_CALL_BLOCK;
  return call_block;
}


/* Append HSA instruction INSN to basic block HBB.  */

static void
hsa_append_insn (hsa_bb *hbb, hsa_insn_basic *insn)
{
  /* Make usre we did not forget to set the kind.  */
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

/* Lookup or create a HSA pseudo register for a given gimple SSA name and if
   necessary, convert it to REQTYPE.  SSA_MAP is a vector mapping gimple
   SSAnames to HSA registers.  Append an new conversion statements to HBB.  */

static hsa_op_reg *
hsa_reg_for_gimple_ssa_reqtype (tree ssa, vec <hsa_op_reg_p> ssa_map,
				hsa_bb *hbb, BrigType16_t reqtype)
{
  hsa_op_reg *reg = hsa_reg_for_gimple_ssa (ssa, ssa_map);
  if (hsa_needs_cvt (reqtype, reg->type))
    {
      hsa_op_reg *converted = hsa_alloc_reg_op ();
      converted->type = reqtype;
      hsa_insn_basic *insn = hsa_alloc_basic_insn ();
      insn->opcode = BRIG_OPCODE_CVT;
      insn->type = reqtype;
      insn->operands[0] = converted;
      insn->operands[1] = reg;
      reg->uses.safe_push (insn);
      hsa_append_insn (hbb, insn);
      return converted;
    }

  return reg;
}

/* Return a register containing the calculated value of EXP which must be an
   expression conisting of PLUS_EXPRs, MULT_EXPRS, NOP_EXPRs, SSA_NAMEs and
   integer constants as returned by get_inner_reference.  SSA_MAP is used to
   lookup HSA equivalent of SSA_NAMEs, newly generated HSA instructions will be
   appended to hbb.  Perform all calculations in ADDRTYPE.  If NEW_USE is
   non-NULL, any register result is going to have it appended to the list of
   uses.  */

static hsa_op_with_type *
gen_address_calculation (tree exp, hsa_bb *hbb, vec <hsa_op_reg_p> ssa_map,
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
       hsa_op_immed *imm = hsa_alloc_immed_op (exp);
       if (addrtype != imm->type)
	 {
	   gcc_assert (bittype_for_type (addrtype)
		       > bittype_for_type (imm->type));
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

  hsa_op_reg *res = hsa_alloc_reg_op ();
  res->type = addrtype;
  hsa_insn_basic *insn = hsa_alloc_basic_insn ();
  insn->opcode = opcode;
  insn->operands[0] = res;
  set_reg_def (res, insn);
  insn->type = addrtype;

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
   to HPP and return the register holding the result.  */

static hsa_op_reg *
add_addr_regs_if_needed (hsa_op_reg *r1, hsa_op_reg *r2, hsa_bb *hbb)
{
  gcc_checking_assert (r2);
  if (!r1)
    return r2;

  hsa_op_reg *res = hsa_alloc_reg_op ();
  gcc_assert (!hsa_needs_cvt (r1->type, r2->type));
  res->type = r1->type;
  hsa_insn_basic *insn = hsa_alloc_basic_insn ();
  insn->opcode = BRIG_OPCODE_ADD;
  insn->operands[0] = res;
  set_reg_def (res, insn);
  insn->type = res->type;
  insn->operands[1] = r1;
  r1->uses.safe_push (insn);
  insn->operands[2] = r2;
  r2->uses.safe_push (insn);
  hsa_append_insn (hbb, insn);
  return res;
}

/* Helper of gen_hsa_addr.  Update *SYMBOL, *ADDRTYPE, *REG and *OFFSET to
   reflect BASE which is the first operand of a MEM_REF or a TARGET_MEM_REF.
   Use SSA_MAP to find registers correspongoing to gimple SSA_NAMEs.  */

static void
process_mem_base (tree base, hsa_symbol **symbol, BrigType16_t *addrtype,
		  hsa_op_reg **reg, offset_int *offset, hsa_bb *hbb,
		  vec <hsa_op_reg_p> ssa_map)
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
   pseudoregisters.  */

static hsa_op_address *
gen_hsa_addr (tree ref, hsa_bb *hbb, vec <hsa_op_reg_p> ssa_map)
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
      /* THe SSA_NAME and ADDR_EXPR cases cannot occur in a valid gimple memory
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
	      disp1 = hsa_alloc_reg_op ();
	      disp1->type = addrtype;
	      hsa_insn_basic *insn = hsa_alloc_basic_insn ();
	      insn->type = addrtype;
	      insn->opcode = BRIG_OPCODE_MUL;
	      insn->operands[0] = disp1;
	      set_reg_def (disp1, insn);
	      insn->operands[1] = idx;
	      idx->uses.safe_push (insn);
	      insn->operands[2] = hsa_alloc_immed_op (TMR_STEP (ref));
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

  return hsa_alloc_addr_op (symbol, reg, hwi_offset);
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

  sym->type = hsa_type_for_tree_type (tree_type, &sym->dimLo,
				      &sym->dimHi, false);

  if (index == -1) /* Function result.  */
    sym->name = "res";
  else /* Function call arguments.  */
    {
      sym->name = NULL;
      sym->name_number = index;
    }

  return hsa_alloc_addr_op (sym, NULL, 0);
}

/* Generate HSA instructions that calculate address of VAL including all
   necessary conversions to flat addressing and place the result into DEST.
   Instructions are appended to HBB.  SSA_MAP maps gimple SSA names to HSA
   pseudo registers.  */

static void
gen_hsa_addr_insns (tree val, hsa_op_reg *dest, hsa_bb *hbb,
		    vec <hsa_op_reg_p> ssa_map)
{
  hsa_op_address *addr;
  hsa_insn_addr *insn = hsa_alloc_addr_insn ();

  gcc_assert (dest->type == hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT));
  if (TREE_CODE (val) == ADDR_EXPR)
    val = TREE_OPERAND (val, 0);
  addr = gen_hsa_addr (val, hbb, ssa_map);
  insn->opcode = BRIG_OPCODE_LDA;
  insn->operands[1] = addr;
  if (addr->reg)
    addr->reg->uses.safe_push (insn);
  if (addr->symbol)
    {
      /* LDA produces segment-relative address, we need to convert
	 it to the flat one.  */
      hsa_op_reg *tmp = hsa_alloc_reg_op ();
      hsa_insn_seg *seg = hsa_alloc_seg_insn ();

      insn->operands[0] = tmp;
      set_reg_def (tmp, insn);
      tmp->type = hsa_get_segment_addr_type (addr->symbol->segment);
      insn->type = tmp->type;
      hsa_append_insn (hbb, insn);

      seg->opcode = BRIG_OPCODE_STOF;
      seg->type = hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT);
      seg->segment = addr->symbol->segment;
      seg->src_type = tmp->type;
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
				vec <hsa_op_reg_p> ssa_map,
				hsa_insn_basic *new_use)
{
  hsa_op_reg *tmp;

  if (TREE_CODE (op) == SSA_NAME)
    tmp = hsa_reg_for_gimple_ssa (op, ssa_map);
  else if (!POINTER_TYPE_P (TREE_TYPE (op)))
    return hsa_alloc_immed_op (op);
  else
    {
      tmp = hsa_alloc_reg_op ();
      tmp->type = hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT);
      gen_hsa_addr_insns (op, tmp, hbb, ssa_map);
    }
  if (new_use)
    tmp->uses.safe_push (new_use);
  return tmp;
}

/* Create a simple movement instruction with register destination dest and
   register or immedate source SRC and append it to the end of HBB.  */

void
hsa_build_append_simple_mov (hsa_op_reg *dest, hsa_op_base *src, hsa_bb *hbb)
{
  hsa_insn_basic *insn = hsa_alloc_basic_insn ();

  insn->opcode = BRIG_OPCODE_MOV;
  insn->type = dest->type;
  insn->operands[0] = dest;
  insn->operands[1] = src;
  if (hsa_op_reg *sreg = dyn_cast <hsa_op_reg *> (src))
    {
      gcc_assert (bittype_for_type (dest->type) == bittype_for_type (sreg->type));
      sreg->uses.safe_push (insn);
    }
  else
    gcc_assert (bittype_for_type (dest->type)
		== bittype_for_type (as_a <hsa_op_immed *> (src)->type));
  set_reg_def (dest, insn);
  hsa_append_insn (hbb, insn);
}


static void
gen_hsa_insns_for_load (hsa_op_reg *dest, tree rhs, tree type, hsa_bb *hbb,
			vec <hsa_op_reg_p> ssa_map)
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
	  hsa_op_immed *imm = hsa_alloc_immed_op (rhs);
	  hsa_build_append_simple_mov (dest, imm, hbb);
	}
    }
  else if (DECL_P (rhs) || TREE_CODE (rhs) == MEM_REF
	   || TREE_CODE (rhs) == TARGET_MEM_REF
	   || handled_component_p (rhs))
    {
      /* Load from memory.  */
      hsa_op_address *addr;
      hsa_insn_mem *mem = hsa_alloc_mem_insn ();

      addr = gen_hsa_addr (rhs, hbb, ssa_map);
      mem->opcode = BRIG_OPCODE_LD;
      /* Not dest->type, that's possibly extended.  */
      mem->type = mem_type_for_type (hsa_type_for_scalar_tree_type (type, false));
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


static void
gen_hsa_insns_for_store (tree lhs, hsa_op_base *src, hsa_bb *hbb,
			 vec <hsa_op_reg_p> ssa_map)
{
  hsa_insn_mem *mem = hsa_alloc_mem_insn ();
  hsa_op_address *addr;

  addr = gen_hsa_addr (lhs, hbb, ssa_map);
  mem->opcode = BRIG_OPCODE_ST;
  if (hsa_op_reg *reg = dyn_cast <hsa_op_reg *> (src))
    reg->uses.safe_push (mem);
  mem->type = mem_type_for_type (hsa_type_for_scalar_tree_type (TREE_TYPE (lhs), false));

  /* XXX The HSAIL disasm has another constraint: if the source
     is an immediate then it must match the destination type.  If
     it's a register the low bits will be used for sub-word stores.
     We're always allocating new operands so we can modify the above
     in place.  */
  if (hsa_op_immed *imm = dyn_cast <hsa_op_immed *> (src))
    imm->type = mem->type;
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
				     vec <hsa_op_reg_p> ssa_map)
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

/* Prepend before INSN a load from SPILL_SYM.  Return the register into which
   we loaded.  We assume we are out of SSA so the returned register does ot
   have its definition set.  */

hsa_op_reg *
hsa_spill_in (hsa_insn_basic *insn, hsa_op_reg *spill_reg, hsa_op_reg **ptmp2)
{
  hsa_symbol *spill_sym = spill_reg->spill_sym;
  hsa_insn_mem *mem = hsa_alloc_mem_insn ();
  hsa_op_reg *reg = hsa_alloc_reg_op ();
  hsa_op_address *addr = hsa_alloc_addr_op (spill_sym, NULL, 0);
  hsa_bb *hbb = hsa_bb_for_bb (insn->bb);

  mem->opcode = BRIG_OPCODE_LD;
  mem->type = spill_sym->type;
  reg->type = spill_sym->type;
  mem->operands[0] = reg;
  mem->operands[1] = addr;

  if (hbb->first_insn == insn)
    hbb->first_insn = mem;
  mem->prev = insn->prev;
  mem->next = insn;
  if (insn->prev)
    insn->prev->next = mem;
  insn->prev = mem;

  *ptmp2 = NULL;
  if (spill_reg->type == BRIG_TYPE_B1)
    {
      hsa_insn_basic *cvtinsn;
      *ptmp2 = reg;
      reg = hsa_alloc_reg_op ();
      reg->type = spill_reg->type;

      cvtinsn = hsa_alloc_basic_insn ();
      cvtinsn->opcode = BRIG_OPCODE_CVT;
      cvtinsn->operands[0] = reg;
      cvtinsn->operands[1] = *ptmp2;
      cvtinsn->type = reg->type;

      cvtinsn->prev = insn->prev;
      cvtinsn->next = insn;
      insn->prev->next = cvtinsn;
      insn->prev = cvtinsn;
    }
  return reg;
}

/* Append after INSN a store to SPILL_SYM.  Return the register from which we
   stored.  We assume we are out of SSA so the returned register does ot have
   its use updated.  */

hsa_op_reg *
hsa_spill_out (hsa_insn_basic *insn, hsa_op_reg *spill_reg, hsa_op_reg **ptmp2)
{
  hsa_symbol *spill_sym = spill_reg->spill_sym;
  hsa_insn_mem *mem = hsa_alloc_mem_insn ();
  hsa_op_reg *reg = hsa_alloc_reg_op ();
  hsa_op_address *addr = hsa_alloc_addr_op (spill_sym, NULL, 0);
  hsa_bb *hbb = hsa_bb_for_bb (insn->bb);
  hsa_op_reg *returnreg;

  *ptmp2 = NULL;
  returnreg = reg;
  if (spill_reg->type == BRIG_TYPE_B1)
    {
      hsa_insn_basic *cvtinsn;
      *ptmp2 = hsa_alloc_reg_op ();
      (*ptmp2)->type = spill_sym->type;
      reg->type = spill_reg->type;

      cvtinsn = hsa_alloc_basic_insn ();
      cvtinsn->opcode = BRIG_OPCODE_CVT;
      cvtinsn->operands[0] = *ptmp2;
      cvtinsn->operands[1] = returnreg;
      cvtinsn->type = (*ptmp2)->type;

      if (hbb->last_insn == insn)
	hbb->last_insn = cvtinsn;
      cvtinsn->prev = insn;
      cvtinsn->next = insn->next;
      if (insn->next)
	insn->next->prev = cvtinsn;
      insn->next = cvtinsn;
      insn = cvtinsn;

      reg = *ptmp2;
    }

  mem->opcode = BRIG_OPCODE_ST;
  mem->type = spill_sym->type;
  reg->type = spill_sym->type;
  mem->operands[0] = reg;
  mem->operands[1] = addr;

  if (hbb->last_insn == insn)
    hbb->last_insn = mem;
  mem->prev = insn;
  mem->next = insn->next;
  if (insn->next)
    insn->next->prev = mem;
  insn->next = mem;

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
			      vec <hsa_op_reg_p> ssa_map)
{
  hsa_insn_cmp *cmp = hsa_alloc_cmp_insn ();

  switch (code)
    {
    case LT_EXPR:
      cmp->compare = BRIG_COMPARE_LT;
      break;
    case LE_EXPR:
      cmp->compare = BRIG_COMPARE_LE;
      break;
    case GT_EXPR:
      cmp->compare = BRIG_COMPARE_GT;
      break;
    case GE_EXPR:
      cmp->compare = BRIG_COMPARE_GE;
      break;
    case EQ_EXPR:
      cmp->compare = BRIG_COMPARE_EQ;
      break;
    case NE_EXPR:
      cmp->compare = BRIG_COMPARE_NE;
      break;
    default:
      sorry ("Support for HSA does not implement comparison tree code %s\n",
	     get_tree_code_name (code));
      return;
    }
  cmp->type = dest->type;
  cmp->operands[0] = dest;
  set_reg_def (dest, cmp);

  cmp->operands[1] = hsa_reg_or_immed_for_gimple_op (lhs, hbb, ssa_map, cmp);
  cmp->operands[2] = hsa_reg_or_immed_for_gimple_op (rhs, hbb, ssa_map, cmp);
  hsa_append_insn (hbb, cmp);
}


/* Generate HSA instruction for an assignment ASSIGN with an operation.
   Instructions will be apended to HBB.  SSA_MAP maps gimple SSA names to HSA
   pseudo registers.  */

static void
gen_hsa_insns_for_operation_assignment (gimple assign, hsa_bb *hbb,
					vec <hsa_op_reg_p> ssa_map)
{
  hsa_insn_basic *insn;
  hsa_op_reg *dest;
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
	dest = hsa_reg_for_gimple_ssa (gimple_assign_lhs (assign), ssa_map);
	gen_hsa_cmp_insn_from_gimple (gimple_assign_rhs_code (assign),
				      gimple_assign_rhs1 (assign),
				      gimple_assign_rhs2 (assign),
				      dest, hbb, ssa_map);
      }
      return;

    default:
      /* Implement others as we com accorss them.  */
      sorry ("Support for HSA does not implement operation %s",
	     get_tree_code_name (gimple_assign_rhs_code (assign)));
      return;
    }

  /* FIXME: Allocate an instruction with modifiers if appropriate.  */
  insn = hsa_alloc_basic_insn ();
  insn->opcode = opcode;
  dest = hsa_reg_for_gimple_ssa (gimple_assign_lhs (assign), ssa_map);
  insn->operands[0] = dest;
  set_reg_def (dest, insn);
  insn->type = dest->type;

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
   Instructions will be apended to HBB, which also needs to be the
   corresponding structure to the basic_block of COND.  SSA_MAP maps gimple SSA
   names to HSA pseudo registers.  */

static void
gen_hsa_insns_for_cond_stmt (gimple cond, hsa_bb *hbb,
			     vec <hsa_op_reg_p> ssa_map)
{
  hsa_op_reg *ctrl = hsa_alloc_reg_op ();
  hsa_insn_br *cbr;

  ctrl->type = BRIG_TYPE_B1;
  gen_hsa_cmp_insn_from_gimple (gimple_cond_code (cond),
				gimple_cond_lhs (cond),
				gimple_cond_rhs (cond),
				ctrl, hbb, ssa_map);

  cbr = hsa_build_cbr_insn (ctrl);
  hsa_append_insn (hbb, cbr);
}

/* Generate HSA instructions for a direct call isntruction.
   Instructions will be apended to HBB, which also needs to be the
   corresponding structure to the basic_block of STMT. SSA_MAP maps gimple SSA
   names to HSA pseudo registers.  */

static void
gen_hsa_insns_for_direct_call (gimple stmt, hsa_bb *hbb,
			       vec <hsa_op_reg_p> ssa_map)
{
  hsa_insn_call *call_insn = hsa_alloc_call_insn ();
  call_insn->opcode = BRIG_OPCODE_CALL;
  call_insn->called_function = gimple_call_fndecl (stmt);
  call_insn->func.kind = BRIG_KIND_OPERAND_CODE_REF;

  hsa_insn_call_block *call_block_insn = hsa_alloc_call_block_insn ();

  /* Preparation of arguments that will be passed to function.  */
  const unsigned args = gimple_call_num_args (stmt);
  for (unsigned i = 0; i < args; ++i)
    {
      tree parm = gimple_call_arg (stmt, (int)i);
      hsa_op_address *addr;
      hsa_insn_mem *mem = hsa_alloc_mem_insn ();
      hsa_op_base *src = hsa_reg_or_immed_for_gimple_op (parm, hbb, ssa_map,
							 mem);

      addr = gen_hsa_addr_for_arg (TREE_TYPE (parm), i);
      mem->opcode = BRIG_OPCODE_ST;
      mem->type = mem_type_for_type (hsa_type_for_scalar_tree_type (TREE_TYPE (parm), false));
      mem->operands[0] = src;
      mem->operands[1] = addr;

      call_block_insn->input_args.safe_push (addr->symbol);
      call_block_insn->input_arg_insns.safe_push (mem);

      call_insn->args_symbols.safe_push (addr->symbol);
    }

  call_insn->args_code_list = hsa_alloc_code_list_op (args);

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
	  result_insn = hsa_alloc_mem_insn ();
	  hsa_op_reg *dst = hsa_reg_for_gimple_ssa (result, ssa_map);

	  result_insn->opcode = BRIG_OPCODE_LD;
	  result_insn->type = mem_type_for_type (hsa_type_for_scalar_tree_type (TREE_TYPE (result), false));
	  result_insn->operands[0] = dst;
	  result_insn->operands[1] = addr;
	  set_reg_def (dst, result_insn);

	  call_block_insn->output_arg_insn = result_insn;
	}

      call_block_insn->output_arg = addr->symbol;
      call_insn->result_symbol = addr->symbol;
      call_insn->result_code_list = hsa_alloc_code_list_op (1);
    }
  else
    call_insn->result_code_list = hsa_alloc_code_list_op (0);

  call_block_insn->call_insn = call_insn;

  if (result_insn)
    call_block_insn->output_arg_insn = result_insn;

  hsa_append_insn (hbb, call_block_insn);
}

/* Generate HSA instructions for a return value isntruction.
   Instructions will be apended to HBB, which also needs to be the
   corresponding structure to the basic_block of STMT. SSA_MAP maps gimple SSA
   names to HSA pseudo registers.  */

static void
gen_hsa_insns_for_return (greturn *stmt, hsa_bb *hbb,
			vec <hsa_op_reg_p> ssa_map)
{
  tree retval = gimple_return_retval (stmt);
  if (retval)
    {
      /* Store of return value.  */
      hsa_insn_mem *mem = hsa_alloc_mem_insn ();
      hsa_op_base *src = hsa_reg_or_immed_for_gimple_op (retval, hbb, ssa_map,
							 mem);

      hsa_op_address *addr = hsa_alloc_addr_op (hsa_cfun.output_arg, NULL, 0);
      mem->opcode = BRIG_OPCODE_ST;
      mem->type = mem_type_for_type (hsa_type_for_scalar_tree_type (TREE_TYPE (retval), false));
      mem->operands[0] = src;
      mem->operands[1] = addr;
      hsa_append_insn (hbb, mem);
    }

  /* HSAIL return instruction emission.  */
  hsa_insn_basic *ret = hsa_alloc_basic_insn ();
  ret->opcode = BRIG_OPCODE_RET;
  hsa_append_insn (hbb, ret);
}

/* If STMT is a call of a known library function, generate code to perform
   it and return true.  */

static bool
gen_hsa_insns_for_known_library_call (gimple stmt, hsa_bb *hbb,
				      vec <hsa_op_reg_p> ssa_map)
{
  tree decl = gimple_call_fndecl (stmt);
  gcc_checking_assert (DECL_NAME (decl));
  const char *name = IDENTIFIER_POINTER (DECL_NAME (decl));

  if (!strcmp (name, "omp_is_initial_device")
      || strcmp (name, "omp_is_initial_device"))
    {
      tree lhs = gimple_call_lhs (stmt);
      if (!lhs)
	return true;

      hsa_op_reg *dest = hsa_reg_for_gimple_ssa (lhs, ssa_map);
      hsa_op_immed *imm = hsa_alloc_immed_op (build_zero_cst (TREE_TYPE (lhs)));

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
			vec <hsa_op_reg_p> ssa_map)
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
	/* We're using just one-dimensional kernels, so hardcode
	   dimension X.  */
	hsa_op_immed *imm = hsa_alloc_immed_op (build_zero_cst (uint32_type_node));
	insn = hsa_alloc_basic_insn ();
	if (dest->type != BRIG_TYPE_U32)
	  {
	    tmp = hsa_alloc_reg_op ();
	    tmp->type = BRIG_TYPE_U32;
	  }
	else
	  tmp = dest;
	insn->opcode = opcode;
	insn->operands[0] = tmp;
	insn->operands[1] = imm;
	insn->type = tmp->type;
	hsa_append_insn (hbb, insn);
	if (dest != tmp)
	  {
	    insn = hsa_alloc_basic_insn ();
	    insn->opcode = dest->type == BRIG_TYPE_S32 ? BRIG_OPCODE_MOV
	      : BRIG_OPCODE_CVT;
	    insn->operands[0] = dest;
	    insn->type = dest->type;
	    insn->operands[1] = tmp;
	    hsa_append_insn (hbb, insn);
	  }
	set_reg_def (dest, insn);
	break;
      }

    case BUILT_IN_SQRT:
    case BUILT_IN_SQRTF:
      /* FIXME: Since calls without a LHS are not removed, souble check that
	 they cannot have side effects.  */
      if (!lhs)
	return;
      insn = hsa_alloc_basic_insn ();
      insn->opcode = BRIG_OPCODE_SQRT;
      dest = hsa_reg_for_gimple_ssa (lhs, ssa_map);
      insn->operands[0] = dest;
      set_reg_def (dest, insn);
      insn->type = dest->type;

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
	hsa_op_address *addr;
	hsa_insn_mem *meminsn = hsa_alloc_mem_insn ();
	addr = gen_hsa_addr (gimple_call_arg (stmt, 0), hbb, ssa_map);
	dest = hsa_reg_for_gimple_ssa (lhs, ssa_map);

	meminsn->opcode = BRIG_OPCODE_LD;
	/* Should check what the memory scope is */
	meminsn->memoryscope = BRIG_MEMORY_SCOPE_WORKGROUP;
	meminsn->type = mem_type_for_type (hsa_type_for_scalar_tree_type (TREE_TYPE (lhs), false));
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
	hsa_op_address *addr;
	hsa_insn_atomic *atominsn = hsa_alloc_atomic_insn ();
	addr = gen_hsa_addr (gimple_call_arg (stmt, 0), hbb, ssa_map);
	dest = hsa_reg_for_gimple_ssa (lhs, ssa_map);

	atominsn->opcode = BRIG_OPCODE_ATOMIC;
	/* Should check what the memory scope is */
	atominsn->memoryscope = BRIG_MEMORY_SCOPE_WORKGROUP;
	atominsn->type = bittype_for_type (hsa_type_for_scalar_tree_type (TREE_TYPE (lhs), false));
	atominsn->operands[0] = dest;
	atominsn->operands[1] = addr;
	atominsn->operands[2]
	  = hsa_reg_or_immed_for_gimple_op (gimple_call_arg (stmt, 1),
					    hbb, ssa_map, atominsn);
	atominsn->operands[3]
	  = hsa_reg_or_immed_for_gimple_op (gimple_call_arg (stmt, 2),
					    hbb, ssa_map, atominsn);
	atominsn->memoryorder = BRIG_MEMORY_ORDER_SC_ACQUIRE_RELEASE;
	atominsn->atomicop = BRIG_ATOMIC_CAS;

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


/* Generate HSA instrutions for a given gimple statement.  Instructions will be
   appended to HBB.  SSA_MAP maps gimple SSA names to HSA pseudo registers.  */

static void
gen_hsa_insns_for_gimple_stmt (gimple stmt, hsa_bb *hbb,
			       vec <hsa_op_reg_p> ssa_map)
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
	sorry ("Support for HSA does not implement gimple label with address taken");

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
			     vec <hsa_op_reg_p> ssa_map)
{
  hsa_insn_phi *hphi;
  unsigned count = gimple_phi_num_args (gphi);

  hphi = hsa_allocp_inst_phi->allocate ();
  memset (hphi, 0, sizeof (hsa_insn_phi));
  hphi->opcode = HSA_OPCODE_PHI;
  hphi->bb = hbb->bb;
  hphi->dest = hsa_reg_for_gimple_ssa (gimple_phi_result (gphi), ssa_map);
  set_reg_def (hphi->dest, hphi);
  /* FIXME: Of course we will have to handle more predecesors, but just not
     yet.  */
  if (count > HSA_OPERANDS_PER_INSN)
    {
      sorry ("Support for HSA does not handle PHI nodes with more than "
	     "%i operands", HSA_OPERANDS_PER_INSN);
      return;
    }

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
	    hphi->operands[i] = hsa_alloc_immed_op (op);
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


/* Create and initialize and return a new hsa_bb structure for a given CFG
   basic block BB.  */

hsa_bb *
hsa_init_new_bb (basic_block bb)
{
  hsa_bb *hbb = hsa_allocp_bb->allocate ();
  memset (hbb, 0, sizeof (hsa_bb));

  gcc_assert (!bb->aux);
  bb->aux = hbb;
  hbb->bb = bb;
  hbb->index = hsa_cfun.hbb_count++;
  hbb->label_ref.kind = BRIG_KIND_OPERAND_CODE_REF;
  hbb->livein = BITMAP_ALLOC (NULL);
  hbb->liveout = BITMAP_ALLOC (NULL);
  return hbb;
}

/* Go over gimple representation and generate our internal HSA one.  SSA_MAP
   maps gimple SSA names to HSA pseudo registers.  */

static void
gen_body_from_gimple (vec <hsa_op_reg_p> ssa_map)
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

/* Generate the vector of parameters of the HSA representation of the current
   function.  This also includes the output parameter representing ther
   result.  */

static void
gen_function_parameters (vec <hsa_op_reg_p> ssa_map)
{
  tree parm;
  int i, count = 0;

  ENTRY_BLOCK_PTR_FOR_FN (cfun)->aux = &hsa_cfun.prologue;
  hsa_cfun.prologue.bb = ENTRY_BLOCK_PTR_FOR_FN (cfun);

  for (parm = DECL_ARGUMENTS (cfun->decl); parm; parm = DECL_CHAIN (parm))
    count++;

  hsa_cfun.input_args_count = count;
  hsa_cfun.input_args = XCNEWVEC (hsa_symbol, count);
  for (parm = DECL_ARGUMENTS (cfun->decl), i = 0;
       parm;
       parm = DECL_CHAIN (parm), i++)
    {
      struct hsa_symbol **slot;

      fillup_sym_for_decl (parm, &hsa_cfun.input_args[i]);
      hsa_cfun.input_args[i].segment = hsa_cfun.kern_p ? BRIG_SEGMENT_KERNARG :
				       BRIG_SEGMENT_ARG;

      hsa_cfun.input_args[i].linkage = BRIG_LINKAGE_FUNCTION;
      if (!DECL_NAME (parm))
	{
	  /* FIXME: Just generate some UID.  */
	  sorry ("Support for HSA does not implement anonymous C++ parameters");
	  return;
	}
      hsa_cfun.input_args[i].name = IDENTIFIER_POINTER (DECL_NAME (parm));
      slot = hsa_cfun.local_symbols->find_slot (&hsa_cfun.input_args[i],
						INSERT);
      gcc_assert (!*slot);
      *slot = &hsa_cfun.input_args[i];

      if (is_gimple_reg (parm))
	{
	  tree ddef = ssa_default_def (cfun, parm);
	  if (ddef && !has_zero_uses (ddef))
	    {
	      hsa_op_address *addr;
	      hsa_insn_mem *mem = hsa_alloc_mem_insn ();
	      hsa_op_reg *dest = hsa_reg_for_gimple_ssa (ddef, ssa_map);

	      addr = gen_hsa_addr (parm, &hsa_cfun.prologue, ssa_map);
	      mem->opcode = BRIG_OPCODE_LD;
	      mem->type = mem_type_for_type (hsa_type_for_scalar_tree_type (TREE_TYPE (ddef), false));
	      mem->operands[0] = dest;
	      mem->operands[1] = addr;
	      set_reg_def (dest, mem);
	      gcc_assert (!addr->reg);
	      hsa_append_insn (&hsa_cfun.prologue, mem);
	    }
	}
    }

  if (!VOID_TYPE_P (TREE_TYPE (TREE_TYPE (cfun->decl))))
    {
      struct hsa_symbol **slot;

      hsa_cfun.output_arg = XCNEW (hsa_symbol);
      fillup_sym_for_decl (DECL_RESULT (cfun->decl), hsa_cfun.output_arg);
      hsa_cfun.output_arg->segment = BRIG_SEGMENT_ARG;
      hsa_cfun.output_arg->linkage = BRIG_LINKAGE_FUNCTION;
      hsa_cfun.output_arg->name = "res";
      slot = hsa_cfun.local_symbols->find_slot (hsa_cfun.output_arg, INSERT);
      gcc_assert (!*slot);
      *slot = hsa_cfun.output_arg;
    }
}

/* Create a mapping between the original function DECL and kernel name NAME.  */

static void
hsa_add_kern_decl_mapping (tree decl, char *name)
{
  hsa_decl_kernel_map_element dkm;
  dkm.decl = decl;
  dkm.name = name;
  vec_safe_push (hsa_decl_kernel_mapping, dkm);
}

/* Return the number of kernel decl name mappings.  */

unsigned
hsa_get_number_decl_kernel_mappings (void)
{
  return vec_safe_length (hsa_decl_kernel_mapping);
}

/* Return the decl in the Ith kernel decl name mapping.  */

tree
hsa_get_decl_kernel_mapping_decl (unsigned i)
{
  return (*hsa_decl_kernel_mapping)[i].decl;
}

/* Return the name in the Ith kernel decl name mapping.  */

char *
hsa_get_decl_kernel_mapping_name (unsigned i)
{
  return (*hsa_decl_kernel_mapping)[i].name;
}

/* Free the mapping between original decls and kernel names.  */

void
hsa_free_decl_kernel_mapping (void)
{
  for (unsigned i = 0; i < hsa_decl_kernel_mapping->length (); ++i)
    free ((*hsa_decl_kernel_mapping)[i].name);
  ggc_free (hsa_decl_kernel_mapping);
}

static void
sanitize_hsa_name (char *p)
{
  for (; *p; p++)
    if (*p == '.')
      *p = '_';
}

/* Genrate HSAIL reprezentation of the current function and write into a
   special section of the output file.  If KERNEL is set, the function will be
   considered an HSA kernel callable from the host, otherwise it will be
   compiled as an HSA function callable from other HSA code.  */

static unsigned int
generate_hsa (bool kernel)
{
  vec <hsa_op_reg_p> ssa_map = vNULL;

  hsa_init_data_for_cfun ();
  hsa_cfun.kern_p = kernel;

  ssa_map.safe_grow_cleared (SSANAMES (cfun)->length ());
  hsa_cfun.name
    = xstrdup (IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (current_function_decl)));
  sanitize_hsa_name (hsa_cfun.name);

  if (hsa_cfun.kern_p)
    hsa_add_kern_decl_mapping (current_function_decl, hsa_cfun.name);

  gen_function_parameters (ssa_map);
  if (seen_error ())
    goto fail;
  gen_body_from_gimple (ssa_map);
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
      group_size_1 = build_int_cst (uint32_type_node, 16);
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

}; // class pass_sra

/* Determine wheter or not to run generation of HSAIL.  */

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
