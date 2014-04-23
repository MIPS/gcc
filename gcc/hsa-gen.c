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
#include "hsa.h"
#include "tm.h"
#include "tree.h"
#include "tree-pass.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-expr.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "machmode.h"
#include "output.h"
#include "basic-block.h"
#include "function.h"
#include "vec.h"
#include "dumpfile.h"
#include "gimple-pretty-print.h"
#include "diagnostic-core.h"
#include "alloc-pool.h"
#include "tree-ssa-operands.h"
#include "gimple-ssa.h"
#include "tree-phinodes.h"
#include "stringpool.h"
#include "tree-ssanames.h"
#include "expr.h"
#include "tree-dfa.h"
#include "ssa-iterators.h"
#include "cgraph.h"
#include "stor-layout.h"
#include "gimplify-me.h"
#include "print-tree.h"

/* Structure containing intermediate HSA representation of the generated
   function. */
struct hsa_function_representation hsa_cfun;

/* Alloc pools for allocating basic hsa structures such as operands,
   instructions and other basic entitie.s */
static alloc_pool hsa_allocp_operand_address;
static alloc_pool hsa_allocp_operand_immed;
static alloc_pool hsa_allocp_operand_reg;
static alloc_pool hsa_allocp_inst_basic;
static alloc_pool hsa_allocp_inst_phi;
static alloc_pool hsa_allocp_inst_mem;
static alloc_pool hsa_allocp_inst_atomic;
static alloc_pool hsa_allocp_inst_addr;
static alloc_pool hsa_allocp_inst_seg;
static alloc_pool hsa_allocp_inst_cmp;
static alloc_pool hsa_allocp_inst_br;
static alloc_pool hsa_allocp_bb;
static alloc_pool hsa_allocp_symbols;

/* Hash function to lookup a symbol for a decl.  */
hash_table <hsa_free_symbol_hasher> hsa_global_variable_symbols;

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
  hsa_global_variable_symbols.create (8);
}

/* Free data structures that are used when dealing with different
   functions.  */

void
hsa_deinit_compilation_unit_data (void)
{
  if (!compilation_unit_data_initialized)
    return;

  hsa_global_variable_symbols.dispose ();
}

/* Allocate HSA structures that we need only while generating with this.  */

static void
hsa_init_data_for_cfun ()
{
  int sym_init_len;

  hsa_init_compilation_unit_data ();
  hsa_allocp_operand_address
    = create_alloc_pool ("HSA address operands",
			 sizeof (struct hsa_op_address), 8);
  hsa_allocp_operand_immed
    = create_alloc_pool ("HSA immedate operands",
			 sizeof (struct hsa_op_immed), 32);
  hsa_allocp_operand_reg
    = create_alloc_pool ("HSA register operands",
			 sizeof (struct hsa_op_reg), 64);
  hsa_allocp_inst_basic
    = create_alloc_pool ("HSA basic instructions",
			 sizeof (struct hsa_insn_basic), 64);
  hsa_allocp_inst_phi
    = create_alloc_pool ("HSA phi operands",
			 sizeof (struct hsa_insn_phi), 16);
  hsa_allocp_inst_mem
    = create_alloc_pool ("HSA memory instructions",
			 sizeof (struct hsa_insn_mem), 32);
  hsa_allocp_inst_atomic
    = create_alloc_pool ("HSA atomic instructions",
			 sizeof (struct hsa_insn_atomic), 32);
  hsa_allocp_inst_addr
    = create_alloc_pool ("HSA address instructions",
			 sizeof (struct hsa_insn_addr), 32);
  hsa_allocp_inst_seg
    = create_alloc_pool ("HSA segment conversion instructions",
			 sizeof (struct hsa_insn_seg), 16);
  hsa_allocp_inst_cmp
    = create_alloc_pool ("HSA comparison instructions",
			 sizeof (struct hsa_insn_cmp), 16);
  hsa_allocp_inst_br
    = create_alloc_pool ("HSA branching instructions",
			 sizeof (struct hsa_insn_br), 16);
  hsa_allocp_bb
    = create_alloc_pool ("HSA basic blocks",
			 sizeof (struct hsa_bb), 8);

  sym_init_len = (vec_safe_length (cfun->local_decls) / 2) + 1;
  hsa_allocp_symbols
    = create_alloc_pool ("HSA symbols",
			 sizeof (struct hsa_op_address), sym_init_len);

  memset (&hsa_cfun, 0, sizeof (hsa_cfun));
  hsa_cfun.prologue.label_ref.kind = BRIG_OPERAND_LABEL_REF;
  hsa_cfun.local_symbols.create (sym_init_len);
  hsa_cfun.reg_count = 0;
  hsa_cfun.hbb_count = 1;       /* 0 is for prologue.  */
  hsa_cfun.in_ssa = true;	/* We start in SSA.  */
}

/* Deinitialize HSA subsysterm and free all allocated memory.  */

static void
hsa_deinit_data_for_cfun (void)
{
  free_alloc_pool (hsa_allocp_operand_address);
  free_alloc_pool (hsa_allocp_operand_immed);
  free_alloc_pool (hsa_allocp_operand_reg);
  free_alloc_pool (hsa_allocp_inst_basic);
  free_alloc_pool (hsa_allocp_inst_phi);
  free_alloc_pool (hsa_allocp_inst_atomic);
  free_alloc_pool (hsa_allocp_inst_mem);
  free_alloc_pool (hsa_allocp_inst_addr);
  free_alloc_pool (hsa_allocp_inst_seg);
  free_alloc_pool (hsa_allocp_inst_cmp);
  free_alloc_pool (hsa_allocp_inst_br);
  free_alloc_pool (hsa_allocp_bb);

  free_alloc_pool (hsa_allocp_symbols);
  hsa_cfun.local_symbols.dispose ();
  free (hsa_cfun.input_args);
  free (hsa_cfun.output_arg);
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
	      gcc_unreachable ();
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
	      gcc_unreachable ();
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
	  gcc_unreachable ();
	}
    }

  if (res == BRIG_TYPE_NONE)
    {
      debug_tree (const_cast<tree> (type));
      gcc_unreachable ();
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
	  gcc_unreachable ();
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

  gcc_checking_assert (TREE_CODE (decl) == PARM_DECL
		       || TREE_CODE (decl) == RESULT_DECL
		       || TREE_CODE (decl) == VAR_DECL);

  dummy.decl = decl;

  if (TREE_CODE (decl) == VAR_DECL && is_global_var (decl))
    {
      slot = hsa_global_variable_symbols.find_slot (&dummy, INSERT);
      gcc_checking_assert (slot);
      if (*slot)
	return *slot;
      sym = XCNEW (struct hsa_symbol);
      sym->segment = BRIG_SEGMENT_GLOBAL;
      warning (0, "referring to global symbol %q+D by name from HSA code won't work", decl);
    }
  else
    {
      slot = hsa_cfun.local_symbols.find_slot (&dummy, INSERT);
      gcc_checking_assert (slot);
      if (*slot)
	return *slot;
      gcc_assert (TREE_CODE (decl) == VAR_DECL);
      sym = (struct hsa_symbol *) pool_alloc (hsa_allocp_symbols);
      memset (sym, 0, sizeof (hsa_symbol));
      sym->segment = BRIG_SEGMENT_PRIVATE;
    }

  fillup_sym_for_decl (decl, sym);
  gcc_checking_assert (DECL_NAME (decl));
  sym->name = IDENTIFIER_POINTER (DECL_NAME (decl));
  *slot = sym;
  return sym;
}

/* Create a spill symbol of type TYPE.  */

hsa_symbol *
hsa_get_spill_symbol (BrigType16_t type)
{
  hsa_symbol *sym = (struct hsa_symbol *) pool_alloc (hsa_allocp_symbols);
  memset (sym, 0, sizeof (hsa_symbol));
  sym->segment = BRIG_SEGMENT_SPILL;
  sym->type = type;
  hsa_cfun.spill_symbols.safe_push(sym);
  return sym;
}

/* Allocate and clear a hsa_op_immed and set it to TREE_VAL.  */

static hsa_op_immed *
hsa_alloc_immed_op (tree tree_val)
{
  hsa_op_immed *imm = (hsa_op_immed *) pool_alloc (hsa_allocp_operand_immed);

  gcc_checking_assert (is_gimple_min_invariant (tree_val)
		       && !POINTER_TYPE_P (TREE_TYPE (tree_val)));

  memset (imm, 0 , sizeof (hsa_op_immed));
  imm->kind = BRIG_OPERAND_IMMED;
  imm->type = hsa_type_for_scalar_tree_type (TREE_TYPE (tree_val), true);
  imm->value = tree_val;

  return imm;
}

/* Allocate, clear and return a hsa_op_reg.  */

static hsa_op_reg *
hsa_alloc_reg_op (void)
{
  hsa_op_reg *hreg;

  hreg = (hsa_op_reg *) pool_alloc (hsa_allocp_operand_reg);
  memset (hreg, 0, sizeof (hsa_op_reg));
  hreg->kind = BRIG_OPERAND_REG;
  /* TODO: Try removing later on.  I suppose this is not necessary but I'd
     rather avoid surprises.  */
  hreg->uses = vNULL;
  hreg->order = hsa_cfun.reg_count++;
  return hreg;
}

/* Allocate and set up a new address operand consisintg of base symbol SYM,
   register REG ans immediate OFFSET.  If the machine model is not large and
   offset is 64 bit, the upper, 32 bits have to be zero.  */

static hsa_op_address *
hsa_alloc_addr_op (hsa_symbol *sym, hsa_op_reg *reg, HOST_WIDE_INT offset)
{
  hsa_op_address *addr;

  addr = (hsa_op_address *) pool_alloc (hsa_allocp_operand_address);
  memset (addr, 0, sizeof (hsa_op_address));
  addr->kind = BRIG_OPERAND_ADDRESS;
  addr->symbol = sym;
  addr->reg = reg;
  addr->imm_offset = offset;
  return addr;
}

/* Lookup or create a HSA pseudo register for a given gimple SSA name.  */

static hsa_op_reg *
hsa_reg_for_gimple_ssa (tree ssa, vec <hsa_op_reg_p> ssa_map)
{
  hsa_op_reg *hreg;

  gcc_assert (TREE_CODE (ssa) == SSA_NAME);
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

  hin = (hsa_insn_basic *) pool_alloc (hsa_allocp_inst_basic);
  memset (hin, 0, sizeof (hsa_insn_basic));
  return hin;
}

/* Allocate, clear and return a memory instruction structure.  */

static hsa_insn_mem *
hsa_alloc_mem_insn (void)
{
  hsa_insn_mem *mem;

  mem = (hsa_insn_mem *) pool_alloc (hsa_allocp_inst_mem);
  memset (mem, 0, sizeof (hsa_insn_mem));
  return mem;
}

/* Allocate, clear and return a memory instruction structure.  */

static hsa_insn_atomic *
hsa_alloc_atomic_insn (void)
{
  hsa_insn_atomic *ret;

  ret = (hsa_insn_atomic *) pool_alloc (hsa_allocp_inst_atomic);
  memset (ret, 0, sizeof (hsa_insn_atomic));
  return ret;
}

/* Allocate, clear and return an address instruction structure.  */

static hsa_insn_addr *
hsa_alloc_addr_insn (void)
{
  hsa_insn_addr *addr;

  addr = (hsa_insn_addr *) pool_alloc (hsa_allocp_inst_addr);
  memset (addr, 0, sizeof (hsa_insn_addr));
  return addr;
}

/* Allocate, clear and return a segment conversion instruction structure.  */

static hsa_insn_seg *
hsa_alloc_seg_insn (void)
{
  hsa_insn_seg *seg;

  seg = (hsa_insn_seg *) pool_alloc (hsa_allocp_inst_seg);
  memset (seg, 0, sizeof (hsa_insn_seg));
  return seg;
}

/* Allocate, clear and return a comprison instruction structure.  Also sets the
   opcode.  */

static hsa_insn_cmp *
hsa_alloc_cmp_insn (void)
{
  hsa_insn_cmp *cmp;

  cmp = (hsa_insn_cmp *) pool_alloc (hsa_allocp_inst_cmp);
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

  cbr = (hsa_insn_br *) pool_alloc (hsa_allocp_inst_br);
  memset (cbr, 0, sizeof (hsa_insn_br));
  cbr->opcode = BRIG_OPCODE_CBR;
  cbr->type = BRIG_TYPE_NONE;
  cbr->width =  BRIG_WIDTH_1;
  cbr->operands[0] = ctrl;
  ctrl->uses.safe_push (cbr);
  return cbr;
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

static void gen_hsa_addr_insns (tree val, hsa_op_reg *dest, hsa_bb *hbb,
				vec <hsa_op_reg_p> ssa_map);
static hsa_op_base * hsa_reg_or_immed_for_gimple_op (tree op, hsa_bb *hbb,
						     vec <hsa_op_reg_p> ssa_map,
						     hsa_insn_basic *new_use);

/* Generate HSA address operand for a given tree memory reference REF.  If
   instructions need to be created to calculate the address, they willbe added
   to the end of HBB, SSA_MAP is an array mapping gimple SSA names to HSA
   pseudoregisters.  */

static hsa_op_address *
gen_hsa_addr (tree ref, hsa_bb *hbb, vec <hsa_op_reg_p> ssa_map)
{
  hsa_symbol *symbol = NULL;
  hsa_op_reg *reg = NULL;
  hsa_op_reg *reg2 = NULL;
  HOST_WIDE_INT offset = 0;

  while (true)
    switch (TREE_CODE (ref))
      {
      case PARM_DECL:
      case VAR_DECL:
      case RESULT_DECL:
	gcc_assert (!symbol);
	symbol = get_symbol_for_decl (ref);
	goto done;

      case SSA_NAME:
	gcc_assert (!reg);
	reg = hsa_reg_for_gimple_ssa (ref, ssa_map);
	goto done;

      case MEM_REF:
	{
	  tree t0 = TREE_OPERAND (ref, 0);

	  if (!integer_zerop (TREE_OPERAND (ref, 1)))
	    offset += tree_to_uhwi (TREE_OPERAND (ref, 1));

	  if (TREE_CODE (t0) == SSA_NAME)
	    {
	      gcc_assert (!reg);
	      reg = hsa_reg_for_gimple_ssa (t0, ssa_map);
	      goto done;
	    }
	  ref = t0;
	  if (TREE_CODE (ref) == ADDR_EXPR)
	    ref = TREE_OPERAND (ref, 0);
	  break;
	}

      case TARGET_MEM_REF:
	{
	  hsa_insn_basic *insn;
	  offset += tree_to_uhwi (TMR_OFFSET (ref));
	  if (TMR_INDEX (ref))
	    {
	      gcc_assert (!reg2);
	      reg2 = hsa_alloc_reg_op ();
	      reg2->type = hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT);
	      insn = hsa_alloc_basic_insn ();
	      if (TMR_STEP (ref))
		{
		  insn->opcode = BRIG_OPCODE_MUL;
		  insn->operands[0] = reg2;
		  insn->type = reg2->type;
		  insn->operands[1] = hsa_reg_for_gimple_ssa (TMR_INDEX (ref), ssa_map);
		  insn->operands[2] = hsa_alloc_immed_op (TMR_STEP (ref));
		}
	      else
		{
		  /* XXX shouldn't use MOV, but source is expected of SSA
		     form, so we can't simply use it as reg2 in case TMR_INDEX2
		     is also set.  */
		  insn->opcode = BRIG_OPCODE_MOV;
		  insn->operands[0] = reg2;
		  insn->type = reg2->type;
		  insn->operands[1] = hsa_reg_for_gimple_ssa (TMR_INDEX (ref), ssa_map);
		}
	      hsa_append_insn (hbb, insn);
	    }
	  if (TMR_INDEX2 (ref))
	    {
	      if (reg2)
		{
		  insn = hsa_alloc_basic_insn ();
		  insn->opcode = BRIG_OPCODE_ADD;
		  insn->operands[0] = reg2;
		  insn->type = reg2->type;
		  insn->operands[1] = reg2;
		  insn->operands[2] = hsa_reg_for_gimple_ssa (TMR_INDEX2 (ref), ssa_map);
		  hsa_append_insn (hbb, insn);
		}
	      else
		{
		  reg2 = hsa_alloc_reg_op ();
		  reg2->type = hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT);

		  hsa_build_append_simple_mov (reg2,
					       hsa_reg_for_gimple_ssa (TMR_INDEX2 (ref), ssa_map), hbb);
		}
	    }
	  ref = TMR_BASE (ref);
	  if (TREE_CODE (ref) == SSA_NAME)
	    {
	      gcc_assert (!reg);
	      reg = hsa_reg_for_gimple_ssa (ref, ssa_map);
	      goto done;
	    }
	  if (TREE_CODE (ref) == ADDR_EXPR)
	    ref = TREE_OPERAND (ref, 0);
	  break;
	}

      case COMPONENT_REF:
	{
	  tree fld = TREE_OPERAND (ref, 1);
	  gcc_assert (!DECL_BIT_FIELD (fld));
	  offset += int_byte_position (fld);
	  ref = TREE_OPERAND (ref, 0);
	  break;
	}

      case ARRAY_REF:
      case ARRAY_RANGE_REF:
        {
	  tree base, varoffset;
	  HOST_WIDE_INT bitsize, bitpos;
	  enum machine_mode mode;
	  int unsignedp, volatilep;
	  hsa_op_reg *tmp;

	  /* This is just to make hbb used.  Remove later. */
	  gcc_assert (hbb);
	  base = get_inner_reference(ref, &bitsize, &bitpos, &varoffset, &mode,
				     &unsignedp, &volatilep, false);
	  gcc_assert ((bitpos % BITS_PER_UNIT) == 0);
	  tmp = hsa_alloc_reg_op ();
	  tmp->type = hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT);
	  gen_hsa_addr_insns (base, tmp, hbb, ssa_map);
	  offset = bitpos / BITS_PER_UNIT;
	  if (varoffset)
	    {
	      hsa_insn_basic *insn;
	      if (TREE_CODE (varoffset) == PLUS_EXPR)
		{
		  tree op1 = TREE_OPERAND (varoffset, 0);
		  tree op2 = TREE_OPERAND (varoffset, 1);
		  gcc_assert (TREE_CODE (op2) == INTEGER_CST);
		  offset += tree_to_uhwi (op2);
		  varoffset = op1;
		}
	      /* We support only ssa names scaled by constants.  */
	      if (TREE_CODE (varoffset) == MULT_EXPR)
		{
		  hsa_op_reg *tmp2;
		  hsa_op_base *scale;
		  tree op1 = TREE_OPERAND (varoffset, 0);
		  tree op2 = TREE_OPERAND (varoffset, 1);
		  tree subofs = 0;
		  tmp2 = hsa_alloc_reg_op ();
		  tmp2->type = hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT);
		  if (TREE_CODE (op1) == PLUS_EXPR)
		    {
		      subofs = TREE_OPERAND (op1, 1);
		      op1 = TREE_OPERAND (op1, 0);
		    }
		  if (CONVERT_EXPR_P (op1))
		    {
		      insn = hsa_alloc_basic_insn ();
		      insn->opcode = BRIG_OPCODE_CVT;
		      insn->operands[0] = tmp2;
		      insn->type = tmp2->type;
		      op1 = TREE_OPERAND (op1, 0);
		      insn->operands[1] = hsa_reg_for_gimple_ssa (op1, ssa_map);
		      hsa_append_insn (hbb, insn);
		      reg = tmp2;
		    }
		  gcc_assert (TREE_CODE (op1) == SSA_NAME);
		  if (subofs)
		    {
		      insn = hsa_alloc_basic_insn ();
		      insn->opcode = BRIG_OPCODE_ADD;
		      insn->operands[0] = tmp2;
		      insn->type = tmp2->type;
		      insn->operands[1] = reg;
		      insn->operands[2] = hsa_reg_or_immed_for_gimple_op (subofs, hbb, ssa_map, insn);
		      hsa_append_insn (hbb, insn);
		      reg = tmp2;
		    }
		  insn = hsa_alloc_basic_insn ();
		  scale = hsa_reg_or_immed_for_gimple_op (op2, hbb, ssa_map, insn);

		  gcc_assert (tmp2->type == reg->type);
		  insn->opcode = BRIG_OPCODE_MUL;
		  insn->operands[0] = tmp2;
		  insn->type = reg->type;
		  insn->operands[2] = scale;
		  insn->operands[1] = reg;
		  hsa_append_insn (hbb, insn);
		  reg = tmp2;
		}
	      else
		{
		  if (CONVERT_EXPR_P (varoffset))
		    varoffset = TREE_OPERAND (varoffset, 0);
		  gcc_assert (TREE_CODE (varoffset) == SSA_NAME);
		  reg = hsa_reg_for_gimple_ssa (varoffset, ssa_map);
		}
	      insn = hsa_alloc_basic_insn ();
	      insn->opcode = BRIG_OPCODE_ADD;
	      insn->operands[0] = tmp;
	      insn->type = tmp->type;
	      insn->operands[2] = reg;
	      insn->operands[1] = tmp;
	      hsa_append_insn (hbb, insn);
	    }
	  reg = tmp;
	  goto done;
	}

      case INTEGER_CST:
	offset += tree_to_uhwi (ref);
	goto done;

      default:
	/* This includes TREE_ADDR on purpose.  */
	gcc_unreachable ();
      }

 done:
  if (!reg)
    reg = reg2;
  else if (reg2)
    {
      hsa_insn_basic *insn;
      insn = hsa_alloc_basic_insn ();
      insn->opcode = BRIG_OPCODE_ADD;
      /* reg2 is always a new temp, so writing to it is okay.  */
      insn->operands[0] = reg2;
      insn->type = reg->type;
      insn->operands[1] = reg;
      insn->operands[2] = reg2;
      hsa_append_insn (hbb, insn);
      reg = reg2;
    }
  return hsa_alloc_addr_op (symbol, reg, offset);
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
      gcc_assert (insn->type == dest->type);
      hsa_append_insn (hbb, insn);
    }
}

/* Return an HSA register or HSA immediate value operand corresponding to
   gimple operand OP.  SSA_MAP maps gimple SSA names to HSA pseudo registers.
   If DEF_INSN is non-NULL, a returned register will have its definition
   already set to DEF_INSN.  */

static hsa_op_base *
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
  if (hsa_op_reg *sreg = dyn_cast <hsa_op_reg> (src))
    {
      gcc_assert (bittype_for_type (dest->type) == bittype_for_type (sreg->type));
      sreg->uses.safe_push (insn);
    }
  else
    gcc_assert (bittype_for_type (dest->type)
		== bittype_for_type (as_a <hsa_op_immed> (src)->type));
  set_reg_def (dest, insn);
  hsa_append_insn (hbb, insn);
}

static bool
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
	gen_hsa_addr_insns (rhs, dest, hbb, ssa_map);
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
      mem->type = hsa_type_for_scalar_tree_type (type, false);
      mem->operands[0] = dest;
      mem->operands[1] = addr;
      set_reg_def (dest, mem);
      if (addr->reg)
	addr->reg->uses.safe_push (mem);
      hsa_append_insn (hbb, mem);
    }
  else
    {
      return false;
    }
  return true;
}

static void
gen_hsa_insns_for_store (tree lhs, hsa_op_base *src, hsa_bb *hbb,
			 vec <hsa_op_reg_p> ssa_map)
{
  hsa_insn_mem *mem = hsa_alloc_mem_insn ();
  hsa_op_address *addr;

  addr = gen_hsa_addr (lhs, hbb, ssa_map);
  mem->opcode = BRIG_OPCODE_ST;
  if (hsa_op_reg *reg = dyn_cast <hsa_op_reg> (src))
    reg->uses.safe_push (mem);
  mem->type = hsa_type_for_scalar_tree_type (TREE_TYPE (lhs), false);

  /* XXX The HSAIL disasm has another constraint: if the source
     is an immediate then it must match the destination type.  If
     it's a register the low bits will be used for sub-word stores.
     We're always allocating new operands so we can modify the above
     in place.  */
  if (hsa_op_immed *imm = dyn_cast <hsa_op_immed> (src))
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
      if (!gen_hsa_insns_for_load (dest, rhs, TREE_TYPE (lhs), hbb, ssa_map))
	{
	  debug_gimple_stmt (assign);
	  gcc_unreachable ();
	}
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
      hsa_op_reg *tmp = hsa_alloc_reg_op ();
      tmp->type = hsa_type_for_scalar_tree_type (TREE_TYPE (lhs), true);
      if (!gen_hsa_insns_for_load (tmp, rhs, TREE_TYPE (lhs), hbb, ssa_map))
	{
	  debug_gimple_stmt (assign);
	  gcc_unreachable ();
	}
      gen_hsa_insns_for_store (lhs, tmp, hbb, ssa_map);
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
   its use updaed.  */

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
      fprintf (stderr, "Unsupported comparison tree code %s\n",
	       get_tree_code_name (code));
      gcc_unreachable ();
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
      /* FIXME: Generate an insn with a modifier here.  */
      gcc_unreachable ();
      break;
    case TRUNC_MOD_EXPR:
      opcode = BRIG_OPCODE_REM;
      break;
    case CEIL_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case ROUND_MOD_EXPR:
      /* FIXME: Generate an insn with a modifier here.  */
      gcc_unreachable ();
      break;
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
      /* FIXME: There is no direct HSA equivalent.  Do we need to implement it
	 later?  */
      gcc_unreachable ();
      break;
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
      /* FIXME:  Implement others as we com accorss them.  */
      fprintf (stderr, "\nUnhandled tree code %s in "
	       "gen_hsa_insns_for_operation_assignment.\n",
	       get_tree_code_name (gimple_assign_rhs_code (assign)));
      debug_gimple_stmt (assign);
      gcc_unreachable ();
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
      debug_gimple_stmt (assign);
      gcc_unreachable ();

      /* Fall through */
    case GIMPLE_BINARY_RHS:
      {
	tree top = gimple_assign_rhs2 (assign);
	insn->operands[2] = hsa_reg_or_immed_for_gimple_op (top, hbb, ssa_map,
							    insn);
      }
      /* Fall through */
    case GIMPLE_UNARY_RHS:
      {
	tree top = gimple_assign_rhs1 (assign);
	insn->operands[1] = hsa_reg_or_immed_for_gimple_op (top, hbb, ssa_map,
							    insn);
      }
      break;
    default:
      gcc_unreachable ();
    }

  hsa_append_insn (hbb, insn);
}

/* Generate HSA instructions for a given gimple condition statemet COND.
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
      debug_gimple_stmt (stmt);
      gcc_unreachable ();
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
	    insn->opcode = dest->type == BRIG_TYPE_S32 ? BRIG_OPCODE_MOV : BRIG_OPCODE_CVT;
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
	meminsn->type = hsa_type_for_scalar_tree_type (TREE_TYPE (lhs), false);
	meminsn->operands[0] = dest;
	meminsn->operands[1] = addr;
	meminsn->semantic = BRIG_SEMANTIC_ACQUIRE;

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
	atominsn->type = bittype_for_type (hsa_type_for_scalar_tree_type (TREE_TYPE (lhs), false));
	atominsn->operands[0] = dest;
	atominsn->operands[1] = addr;
	atominsn->operands[2]
	  = hsa_reg_or_immed_for_gimple_op (gimple_call_arg (stmt, 1),
					    hbb, ssa_map, atominsn);
	atominsn->operands[3]
	  = hsa_reg_or_immed_for_gimple_op (gimple_call_arg (stmt, 2),
					    hbb, ssa_map, atominsn);
	atominsn->semantic = BRIG_SEMANTIC_ACQUIRE_RELEASE;
	atominsn->atomicop = BRIG_ATOMIC_CAS;

	set_reg_def (dest, atominsn);
	if (addr->reg)
	  addr->reg->uses.safe_push (atominsn);
	hsa_append_insn (hbb, atominsn);
	break;
      }

    default:
      debug_gimple_stmt (stmt);
      gcc_unreachable ();
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
      {
	hsa_insn_basic *ret = hsa_alloc_basic_insn ();
	ret->opcode = BRIG_OPCODE_RET;
	hsa_append_insn (hbb, ret);
	break;
      }
    case GIMPLE_COND:
      gen_hsa_insns_for_cond_stmt (stmt, hbb, ssa_map);
      break;
    case GIMPLE_CALL:
      gen_hsa_insns_for_call (stmt, hbb, ssa_map);
      break;
    case GIMPLE_DEBUG:
      /* ??? HSA supports some debug facilities.  */
      break;
    default:
      debug_gimple_stmt (stmt);
      gcc_unreachable ();
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

  hphi = (hsa_insn_phi *) pool_alloc (hsa_allocp_inst_phi);
  memset (hphi, 0, sizeof (hsa_insn_phi));
  hphi->opcode = HSA_OPCODE_PHI;
  hphi->bb = hbb->bb;
  hphi->dest = hsa_reg_for_gimple_ssa (gimple_phi_result (gphi), ssa_map);
  set_reg_def (hphi->dest, hphi);

  /* FIXME: Of course we will have to handle more predecesors, but just not
     yet.  */
  gcc_assert (count <= HSA_OPERANDS_PER_INSN);
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
	    /* FIXME: Implement.  Possibly needs (address-load) instructions
	       at predecessor.  */
	    gcc_unreachable ();
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
  hsa_bb *hbb = (struct hsa_bb *) pool_alloc (hsa_allocp_bb);
  memset (hbb, 0, sizeof (hsa_bb));

  gcc_assert (!bb->aux);
  bb->aux = hbb;
  hbb->bb = bb;
  hbb->index = hsa_cfun.hbb_count++;
  hbb->label_ref.kind = BRIG_OPERAND_LABEL_REF;
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
	gen_hsa_insns_for_gimple_stmt (gsi_stmt (gsi), hbb, ssa_map);
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
   function.  Thisd also includes the output parameter representing ther
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
      hsa_cfun.input_args[i].segment = BRIG_SEGMENT_KERNARG;
      gcc_checking_assert (DECL_NAME (parm));
      hsa_cfun.input_args[i].name = IDENTIFIER_POINTER (DECL_NAME (parm));
      slot = hsa_cfun.local_symbols.find_slot (&hsa_cfun.input_args[i],
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
	      mem->type = hsa_type_for_scalar_tree_type (TREE_TYPE (ddef), false);
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
      hsa_cfun.output_arg->name = "output$param";
      slot = hsa_cfun.local_symbols.find_slot (hsa_cfun.output_arg, INSERT);
      gcc_assert (!*slot);
      *slot = hsa_cfun.output_arg;
    }
  else
    hsa_cfun.output_arg = NULL;
}

static void
sanitize_hsa_name (char *p)
{
  for (; *p; p++)
    if (*p == '.')
      *p = '_';
}

/* Genrate HSAIL reprezentation of the current function and write into a
   special section of the output file.  */

static unsigned int
generate_hsa (void)
{
  vec <hsa_op_reg_p> ssa_map = vNULL;

  hsa_init_data_for_cfun ();

  ssa_map.safe_grow_cleared (SSANAMES (cfun)->length ());
  hsa_cfun.name
    = xstrdup (IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (current_function_decl)));
  sanitize_hsa_name (hsa_cfun.name);
  gen_function_parameters (ssa_map);
  gen_body_from_gimple (ssa_map);
  ssa_map.release ();

  hsa_regalloc ();

  hsa_brig_emit_function ();
  hsa_deinit_data_for_cfun ();
  return 0;
}

static GTY(()) tree hsa_launch_fn;
static GTY(()) tree hsa_kernel_desc_type;

static void
init_hsa_functions (void)
{
  tree launch_fn_type;
  tree fields, f;
  tree constcharptr;
  if (hsa_launch_fn)
    return;

  constcharptr = build_pointer_type (build_qualified_type
				     (char_type_node, TYPE_QUAL_CONST));

  hsa_kernel_desc_type = make_node (RECORD_TYPE);
  fields = NULL_TREE;
  f = build_decl (BUILTINS_LOCATION, FIELD_DECL,
		  get_identifier ("filename"), constcharptr);
  DECL_CHAIN (f) = fields;
  fields = f;
  f = build_decl (BUILTINS_LOCATION, FIELD_DECL,
		  get_identifier ("name"), constcharptr);
  DECL_CHAIN (f) = fields;
  fields = f;
  f = build_decl (BUILTINS_LOCATION, FIELD_DECL,
		  get_identifier ("nargs"), uint64_type_node);
  DECL_CHAIN (f) = fields;
  fields = f;
  f = build_decl (BUILTINS_LOCATION, FIELD_DECL,
		  get_identifier ("kernel"), ptr_type_node);
  DECL_CHAIN (f) = fields;
  fields = f;

  finish_builtin_struct (hsa_kernel_desc_type, "__hsa_kernel_desc",
			 fields, NULL_TREE);

  /* __hsa_launch_kernel (__hsa_kernel_desc * kd, void* attr, uint64_t *args) */

  launch_fn_type
    = build_function_type_list (void_type_node,
				build_pointer_type (hsa_kernel_desc_type),
				ptr_type_node,
				build_pointer_type (uint64_type_node),
				NULL_TREE);

  hsa_launch_fn
    = build_fn_decl ("__hsa_launch_kernel",
		     launch_fn_type);
}

static unsigned int
wrap_hsa (void)
{
  bool changed = false;
  basic_block bb;
  init_hsa_functions ();
  FOR_EACH_BB_FN (bb, cfun)
    {
      gimple_stmt_iterator gsi;
      tree fndecl;
      for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi);)
	if (is_gimple_call (gsi_stmt (gsi))
	    && (fndecl = gimple_call_fndecl (gsi_stmt (gsi)))
	    && lookup_attribute ("hsa", DECL_ATTRIBUTES (fndecl)))
	  {
	    char *tmpname;
	    gimple launch;
	    vec<constructor_elt, va_gc> *v = NULL;
	    tree str;
	    str = build_string_literal (1, "");
	    CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, str);

	    int slen = IDENTIFIER_LENGTH (DECL_ASSEMBLER_NAME (fndecl));
	    if (asprintf (&tmpname, "&%s", IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (fndecl))) < 0)
	      gcc_unreachable ();
	    sanitize_hsa_name (tmpname + 1);

	    str = build_string_literal (slen + 2, tmpname);
	    CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, str);
	    CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, size_int (gimple_call_num_args (gsi_stmt (gsi))));
	    CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, null_pointer_node);

	    tree initval = build_constructor (hsa_kernel_desc_type, v);

	    /* Create a new VAR_DECL of type descriptor.  */
	    char tmp_name[32];
	    static unsigned int var_id;
	    ASM_GENERATE_INTERNAL_LABEL (tmp_name, "__hsa_kd", var_id++);
	    tree decl = build_decl (gimple_location (gsi_stmt (gsi)),
				    VAR_DECL, get_identifier (tmp_name),
				    hsa_kernel_desc_type);
	    TREE_STATIC (decl) = 1;
	    TREE_PUBLIC (decl) = 0;
	    DECL_ARTIFICIAL (decl) = 1;
	    DECL_IGNORED_P (decl) = 1;
	    DECL_EXTERNAL (decl) = 0;

	    TREE_CONSTANT (initval) = 1;
	    TREE_STATIC (initval) = 1;
	    DECL_INITIAL (decl) = initval;
	    //rest_of_decl_compilation (decl, 1, 0);
	    /* ??? We'd like to do 
	         varpool_add_new_variable (decl);
	       but that breaks with -O0 (variable not emitted).  */
	    varpool_assemble_decl (varpool_node_for_decl (decl));

	    decl = build_fold_addr_expr (decl);

	    tree args = create_tmp_var (build_array_type_nelts (uint64_type_node,
								gimple_call_num_args (gsi_stmt (gsi))),
					NULL);

	    for (unsigned i = 0; i < gimple_call_num_args (gsi_stmt (gsi)); i++)
	      {
		tree arg = gimple_call_arg (gsi_stmt (gsi), i);
		gimple g;

		tree r = build4 (ARRAY_REF, uint64_type_node, args,
				 size_int (i), NULL_TREE, NULL_TREE);

		arg = force_gimple_operand_gsi (&gsi,
						fold_convert (uint64_type_node,
							      arg),
						true, NULL_TREE,
						true, GSI_SAME_STMT);
		g = gimple_build_assign (r, arg);
		gsi_insert_before (&gsi, g, GSI_SAME_STMT);
	      }

	    args = build_fold_addr_expr (args);

	    /* XXX doesn't handle calls with lhs, doesn't remove EH
	       edges.  */
	    launch = gimple_build_call (hsa_launch_fn, 3, decl, null_pointer_node, args);
	    gsi_insert_before (&gsi, launch, GSI_SAME_STMT);
	    unlink_stmt_vdef (gsi_stmt (gsi));
	    gsi_remove (&gsi, true);
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
  true,					/* has_execute */
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
  return true;
}

unsigned int
pass_gen_hsail::execute (function *)
{
  if (lookup_attribute ("hsa",
			DECL_ATTRIBUTES (current_function_decl)))
    return generate_hsa ();
  else
    return wrap_hsa ();
}

} // anon namespace

/* Create the instance of hsa gen pass.  */

gimple_opt_pass *
make_pass_gen_hsail (gcc::context *ctxt)
{
  return new pass_gen_hsail (ctxt);
}

#include "gt-hsa-gen.h"
