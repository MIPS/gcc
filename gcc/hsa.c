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
#include "symbol-summary.h"
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
  /* Size of OMP data, if the kernel contains a kernel dispatch.  */
  unsigned omp_data_size;
};

/* Mapping between decls and corresponding HSA kernels in this compilation
   unit.  */

static GTY (()) vec<hsa_decl_kernel_map_element, va_gc> *hsa_decl_kernel_mapping;

/* Mapping between decls and corresponding HSA kernels
   called by the function.  */
hash_map <tree, vec <const char *> *> *hsa_decl_kernel_dependencies;

/* Hash function to lookup a symbol for a decl.  */
hash_table <hsa_free_symbol_hasher> *hsa_global_variable_symbols;

/* HSA summaries.  */
hsa_summary_t *hsa_summaries = NULL;

/* HSA number of threads.  */
hsa_symbol *hsa_num_threads = NULL;

/* HSA function that cannot be expanded to HSAIL.  */
hash_set <tree> *hsa_failed_functions = NULL;

/* True if compilation unit-wide data are already allocated and initialized.  */
static bool compilation_unit_data_initialized;

/* Return true if FNDECL represents an HSA-callable function.  */

bool
hsa_callable_function_p (tree fndecl)
{
  return lookup_attribute ("omp declare target", DECL_ATTRIBUTES (fndecl));
}

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
  if (compilation_unit_data_initialized)
    delete hsa_global_variable_symbols;

  if (hsa_failed_functions)
    delete hsa_failed_functions;
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

/* Return true if a register in operand number OPNUM of instruction with OPCODE
   is output of that instruction.  False if it is an input.  */

bool
hsa_opcode_op_output_p (int opcode, int opnum)
{
  switch (opcode)
    {
    case HSA_OPCODE_PHI:
    case BRIG_OPCODE_CBR:
    case BRIG_OPCODE_SBR:
    case BRIG_OPCODE_ST:
    case BRIG_OPCODE_SIGNALNORET:
      /* FIXME: There are probably missing cases here, double check.  */
      return false;
    default:
     return opnum == 0;
    }
}

/* Return true if OPCODE is an floating-point bit instruction opcode.  */

bool
hsa_opcode_floating_bit_insn_p (BrigOpcode16_t opcode)
{
  switch (opcode)
    {
    case BRIG_OPCODE_NEG:
    case BRIG_OPCODE_ABS:
    case BRIG_OPCODE_CLASS:
    case BRIG_OPCODE_COPYSIGN:
      return true;
    default:
      return false;
    }
}

/* Return the number of destination operands for this INSN.  */

unsigned
hsa_insn_basic::input_count ()
{
  switch (m_opcode)
    {
      default:
	return 1;

      case BRIG_OPCODE_NOP:
	return 0;

      case BRIG_OPCODE_EXPAND:
	return 2;

      case BRIG_OPCODE_LD:
	/* ld_v[234] not yet handled.  */
	return 1;

      case BRIG_OPCODE_ST:
	return 0;

      case BRIG_OPCODE_ATOMICNORET:
	return 0;

      case BRIG_OPCODE_SIGNAL:
	return 1;

      case BRIG_OPCODE_SIGNALNORET:
	return 0;

      case BRIG_OPCODE_MEMFENCE:
	return 0;

      case BRIG_OPCODE_RDIMAGE:
      case BRIG_OPCODE_LDIMAGE:
      case BRIG_OPCODE_STIMAGE:
      case BRIG_OPCODE_QUERYIMAGE:
      case BRIG_OPCODE_QUERYSAMPLER:
	sorry ("HSA image ops not handled");
	return 0;

      case BRIG_OPCODE_CBR:
      case BRIG_OPCODE_BR:
	return 0;

      case BRIG_OPCODE_SBR:
	return 0; /* ??? */

      case BRIG_OPCODE_WAVEBARRIER:
	return 0; /* ??? */

      case BRIG_OPCODE_BARRIER:
      case BRIG_OPCODE_ARRIVEFBAR:
      case BRIG_OPCODE_INITFBAR:
      case BRIG_OPCODE_JOINFBAR:
      case BRIG_OPCODE_LEAVEFBAR:
      case BRIG_OPCODE_RELEASEFBAR:
      case BRIG_OPCODE_WAITFBAR:
	return 0;

      case BRIG_OPCODE_LDF:
	return 1;

      case BRIG_OPCODE_ACTIVELANECOUNT:
      case BRIG_OPCODE_ACTIVELANEID:
      case BRIG_OPCODE_ACTIVELANEMASK:
      case BRIG_OPCODE_ACTIVELANEPERMUTE:
	return 1; /* ??? */

      case BRIG_OPCODE_CALL:
      case BRIG_OPCODE_SCALL:
      case BRIG_OPCODE_ICALL:
	return 0;

      case BRIG_OPCODE_RET:
	return 0;

      case BRIG_OPCODE_ALLOCA:
	return 1;

      case BRIG_OPCODE_CLEARDETECTEXCEPT:
	return 0;

      case BRIG_OPCODE_SETDETECTEXCEPT:
	return 0;

      case BRIG_OPCODE_PACKETCOMPLETIONSIG:
      case BRIG_OPCODE_PACKETID:
      case BRIG_OPCODE_CASQUEUEWRITEINDEX:
      case BRIG_OPCODE_LDQUEUEREADINDEX:
      case BRIG_OPCODE_LDQUEUEWRITEINDEX:
      case BRIG_OPCODE_STQUEUEREADINDEX:
      case BRIG_OPCODE_STQUEUEWRITEINDEX:
	return 1; /* ??? */

      case BRIG_OPCODE_ADDQUEUEWRITEINDEX:
	return 1;

      case BRIG_OPCODE_DEBUGTRAP:
	return 0;

      case BRIG_OPCODE_GROUPBASEPTR:
      case BRIG_OPCODE_KERNARGBASEPTR:
	return 1; /* ??? */

      case HSA_OPCODE_ARG_BLOCK:
	return 0;

      case BRIG_KIND_DIRECTIVE_COMMENT:
	return 0;
    }
}

/* Return the number of source operands for this INSN.  */

unsigned
hsa_insn_basic::num_used_ops ()
{
  gcc_checking_assert (input_count () <= operand_count ());

  return operand_count () - input_count ();
}

/* Set alignment to VALUE.  */

void
hsa_insn_mem::set_align (BrigAlignment8_t value)
{
  /* TODO: Perhaps remove this dump later on:  */
  if (dump_file && (dump_flags & TDF_DETAILS) && value < m_align)
    {
      fprintf (dump_file, "Decreasing alignment to %u in instruction ", value);
      dump_hsa_insn (dump_file, this);
    }
  m_align = value;
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
      gcc_assert (hsa_seen_error ());
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

/* Return true if and only if TYPE is a floating point number type.  */

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

/* Return true if and only if TYPE is an integer number type.  */

bool
hsa_type_integer_p (BrigType16_t type)
{
  switch (type & BRIG_TYPE_BASE_MASK)
    {
    case BRIG_TYPE_U8:
    case BRIG_TYPE_U16:
    case BRIG_TYPE_U32:
    case BRIG_TYPE_U64:
    case BRIG_TYPE_S8:
    case BRIG_TYPE_S16:
    case BRIG_TYPE_S32:
    case BRIG_TYPE_S64:
      return true;
    default:
      return false;
    }
}

/* Return HSA alignment encoding alignment to N bits.  */

BrigAlignment8_t
hsa_alignment_encoding (unsigned n)
{
  gcc_assert (n >= 8 && !(n & (n - 1)));
  if (n >= 256)
    return BRIG_ALIGNMENT_32;

  switch (n)
    {
    case 8:
      return BRIG_ALIGNMENT_1;
    case 16:
      return BRIG_ALIGNMENT_2;
    case 32:
      return BRIG_ALIGNMENT_4;
    case 64:
      return BRIG_ALIGNMENT_8;
    case 128:
      return BRIG_ALIGNMENT_16;
    default:
      gcc_unreachable ();
    }
}

/* Return natural alignment of HSA TYPE.  */

BrigAlignment8_t
hsa_natural_alignment (BrigType16_t type)
{
  return hsa_alignment_encoding (hsa_type_bit_size (type & ~BRIG_TYPE_ARRAY));
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
  if (hsa_insn_arg_block *block = dyn_cast <hsa_insn_arg_block *> (insn))
    {
      block->~hsa_insn_arg_block ();
      return;
    }
  insn->~hsa_insn_basic ();
  return;
}

/* Create a mapping between the original function DECL and kernel name NAME.  */

void
hsa_add_kern_decl_mapping (tree decl, char *name, unsigned omp_data_size)
{
  hsa_decl_kernel_map_element dkm;
  dkm.decl = decl;
  dkm.name = name;
  dkm.omp_data_size = omp_data_size;
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

/* Return maximum OMP size for kernel decl name mapping.  */

unsigned
hsa_get_decl_kernel_mapping_omp_size (unsigned i)
{
  return (*hsa_decl_kernel_mapping)[i].omp_data_size;
}

/* Free the mapping between original decls and kernel names.  */

void
hsa_free_decl_kernel_mapping (void)
{
  if (hsa_decl_kernel_mapping == NULL)
    return;

  for (unsigned i = 0; i < hsa_decl_kernel_mapping->length (); ++i)
    free ((*hsa_decl_kernel_mapping)[i].name);
  ggc_free (hsa_decl_kernel_mapping);
}

/* Add new kernel dependency.  */

void
hsa_add_kernel_dependency (tree caller, const char *called_function)
{
  if (hsa_decl_kernel_dependencies == NULL)
    hsa_decl_kernel_dependencies = new hash_map<tree, vec<const char *> *> ();

  vec <const char *> *s = NULL;
  vec <const char *> **slot = hsa_decl_kernel_dependencies->get (caller);
  if (slot == NULL)
    {
      s = new vec <const char *> ();
      hsa_decl_kernel_dependencies->put (caller, s);
    }
  else
    s = *slot;

  s->safe_push (called_function);
}

/* Modify the name P in-place so that it is a valid HSA identifier.  */

void
hsa_sanitize_name (char *p)
{
  for (; *p; p++)
    if (*p == '.' || *p == '-')
      *p = '_';
}

/* Clone the name P, set trailing ampersand and sanitize the name.  */

char *
hsa_brig_function_name (const char *p)
{
  unsigned len = strlen (p);
  char *buf = XNEWVEC (char, len + 2);

  buf[0] = '&';
  buf[len + 1] = '\0';
  memcpy (buf + 1, p, len);

  hsa_sanitize_name (buf);
  return buf;
}

/* Return declaration name if exists.  */

const char *
hsa_get_declaration_name (tree decl)
{
  if (!DECL_NAME (decl))
    {
      char *b = XNEWVEC (char, 64);
      sprintf (b, "__hsa_anonymous_%i", DECL_UID (decl));
      const char *ggc_str = ggc_alloc_string (b, strlen (b) + 1);
      free (b);
      return ggc_str;
    }
  else if (TREE_CODE (decl) == FUNCTION_DECL)
    return cgraph_node::get_create (decl)->asm_name ();
  else
    return IDENTIFIER_POINTER (DECL_NAME (decl));

  return NULL;
}

/* Add a HOST function to HSA summaries.  */

void
hsa_register_kernel (cgraph_node *host)
{
  if (hsa_summaries == NULL)
    hsa_summaries = new hsa_summary_t (symtab);
  hsa_function_summary *s = hsa_summaries->get (host);
  s->kind = HSA_KERNEL;
}

/* Add a pair of functions to HSA summaries.  GPU is an HSA implementation of
   a HOST function.  */

void
hsa_register_kernel (cgraph_node *gpu, cgraph_node *host)
{
  if (hsa_summaries == NULL)
    hsa_summaries = new hsa_summary_t (symtab);
  hsa_summaries->link_functions (gpu, host, HSA_KERNEL);
}

/* Return true if expansion of the current HSA function has already failed.  */

bool
hsa_seen_error (void)
{
  return hsa_cfun->seen_error;
}

/* Mark current HSA function as failed.  */

void
hsa_fail_cfun (void)
{
  if (hsa_failed_functions == NULL)
    hsa_failed_functions = new hash_set <tree> ();
  hsa_failed_functions->add (hsa_cfun->decl);
  hsa_cfun->seen_error = true;
}

#include "gt-hsa.h"
