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
  /* FIXME: I suppose this is techically wrong but should work for me now.  */
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

#include "gt-hsa.h"
