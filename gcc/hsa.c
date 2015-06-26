/* Implementation of commonly needed HSAIL related functions and methods.
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

/* Structure containing intermediate HSA representation of the generated
   function. */
class hsa_function_representation *hsa_cfun;

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

/* Return true if we are generating large HSA machine model.  */

bool
hsa_machine_large_p (void)
{
  /* FIXME: I suppose this is technically wrong but should work for me now.  */
  return (GET_MODE_BITSIZE (Pmode) == 64);
}

/* Return the HSA profile we are using.  */

bool
hsa_full_profile_p (void)
{
  return true;
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

/* Return size of HSA type T in bits.  */

unsigned
hsa_type_bit_size (BrigType16_t t)
{
  switch (t)
    {
    case BRIG_TYPE_B1:
      return 1;

    case BRIG_TYPE_U8:
    case BRIG_TYPE_S8:
    case BRIG_TYPE_B8:
      return 8;

    case BRIG_TYPE_U16:
    case BRIG_TYPE_S16:
    case BRIG_TYPE_B16:
    case BRIG_TYPE_F16:
      return 16;

    case BRIG_TYPE_U32:
    case BRIG_TYPE_S32:
    case BRIG_TYPE_B32:
    case BRIG_TYPE_F32:
    case BRIG_TYPE_U8X4:
    case BRIG_TYPE_U16X2:
    case BRIG_TYPE_S8X4:
    case BRIG_TYPE_S16X2:
    case BRIG_TYPE_F16X2:
      return 32;

    case BRIG_TYPE_U64:
    case BRIG_TYPE_S64:
    case BRIG_TYPE_F64:
    case BRIG_TYPE_B64:
    case BRIG_TYPE_U8X8:
    case BRIG_TYPE_U16X4:
    case BRIG_TYPE_U32X2:
    case BRIG_TYPE_S8X8:
    case BRIG_TYPE_S16X4:
    case BRIG_TYPE_S32X2:
    case BRIG_TYPE_F16X4:
    case BRIG_TYPE_F32X2:

      return 64;

    case BRIG_TYPE_B128:
    case BRIG_TYPE_U8X16:
    case BRIG_TYPE_U16X8:
    case BRIG_TYPE_U32X4:
    case BRIG_TYPE_U64X2:
    case BRIG_TYPE_S8X16:
    case BRIG_TYPE_S16X8:
    case BRIG_TYPE_S32X4:
    case BRIG_TYPE_S64X2:
    case BRIG_TYPE_F16X8:
    case BRIG_TYPE_F32X4:
    case BRIG_TYPE_F64X2:
      return 128;

    default:
      gcc_assert (seen_error ());
      return t;
    }
}

/* Return HSA bit-type with the same size as the type T.  */

BrigType16_t
hsa_bittype_for_type (BrigType16_t t)
{
  switch (hsa_type_bit_size (t))
    {
    case 1:
      return BRIG_TYPE_B1;
    case 8:
      return BRIG_TYPE_B8;
    case 16:
      return BRIG_TYPE_B16;
    case 32:
      return BRIG_TYPE_B32;
    case 64:
      return BRIG_TYPE_B64;
    case 128:
      return BRIG_TYPE_B128;
    default:
      return t;
    }
}

/* Return true iff TYPE is a floating point number type.  */

bool
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

/* Call the correct destructor on a statement STMT.  */

void
hsa_destroy_insn (hsa_insn_basic *insn)
{
  if (hsa_insn_phi *phi = dyn_cast <hsa_insn_phi *> (insn))
    {
      phi->~hsa_insn_phi ();
      return;
    }
  if (hsa_insn_br *br = dyn_cast <hsa_insn_br *> (insn))
    {
      br->~hsa_insn_br ();
      return;
    }
  if (hsa_insn_cmp *cmp = dyn_cast <hsa_insn_cmp *> (insn))
    {
      cmp->~hsa_insn_cmp ();
      return;
    }
  if (hsa_insn_mem *mem = dyn_cast <hsa_insn_mem *> (insn))
    {
      mem->~hsa_insn_mem ();
      return;
    }
  if (hsa_insn_atomic *atomic = dyn_cast <hsa_insn_atomic *> (insn))
    {
      atomic->~hsa_insn_atomic ();
      return;
    }
  if (hsa_insn_seg *seg = dyn_cast <hsa_insn_seg *> (insn))
    {
      seg->~hsa_insn_seg ();
      return;
    }
  if (hsa_insn_call *call = dyn_cast <hsa_insn_call *> (insn))
    {
      call->~hsa_insn_call ();
      return;
    }
  if (hsa_insn_call_block *block = dyn_cast <hsa_insn_call_block *> (insn))
    {
      block->~hsa_insn_call_block ();
      return;
    }
  insn->~hsa_insn_basic ();
  return;
}

/* Create a mapping between the original function DECL and kernel name NAME.  */

void
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

/* Modify the name P in-place so that it is a valid HSA identifier.  */

void
hsa_sanitize_name (char *p)
{
  for (; *p; p++)
    if (*p == '.')
      *p = '_';
}

#include "gt-hsa.h"
