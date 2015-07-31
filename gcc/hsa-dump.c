/* Infrastructure to dump our HSAIL IL
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "is-a.h"
#include "vec.h"
#include "hash-set.h"
#include "defaults.h"
#include "hard-reg-set.h"
#include "dominance.h"
#include "cfg.h"
#include "input.h"
#include "function.h"
#include "symtab.h"
#include "alias.h"
#include "double-int.h"
#include "inchash.h"
#include "tree.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-expr.h"
#include "predict.h"
#include "basic-block.h"
#include "fold-const.h"
#include "gimple.h"
#include "gimple-pretty-print.h"
#include "hsa.h"

/* Return textual name of TYPE.  */

static const char *
hsa_type_name (BrigType16_t type)
{
  switch (type)
    {
    case BRIG_TYPE_NONE:
      return "none";
    case BRIG_TYPE_U8:
      return "u8";
    case BRIG_TYPE_U16:
      return "u16";
    case BRIG_TYPE_U32:
      return "u32";
    case BRIG_TYPE_U64:
      return "u64";
    case BRIG_TYPE_S8:
      return "s8";
    case BRIG_TYPE_S16:
      return "s16";
    case BRIG_TYPE_S32:
      return "s32";
    case BRIG_TYPE_S64:
      return "s64";
    case BRIG_TYPE_F16:
      return "f16";
    case BRIG_TYPE_F32:
      return "f32";
    case BRIG_TYPE_F64:
      return "f64";
    case BRIG_TYPE_B1:
      return "b1";
    case BRIG_TYPE_B8:
      return "b8";
    case BRIG_TYPE_B16:
      return "b16";
    case BRIG_TYPE_B32:
      return "b32";
    case BRIG_TYPE_B64:
      return "b64";
    case BRIG_TYPE_B128:
      return "b128";
    case BRIG_TYPE_SAMP:
      return "samp";
    case BRIG_TYPE_ROIMG:
      return "roimg";
    case BRIG_TYPE_WOIMG:
      return "woimg";
    case BRIG_TYPE_RWIMG:
      return "rwimg";
    case BRIG_TYPE_SIG32:
      return "sig32";
    case BRIG_TYPE_SIG64:
      return "sig64";
    case BRIG_TYPE_U8X4:
      return "u8x4";
    case BRIG_TYPE_U8X8:
      return "u8x8";
    case BRIG_TYPE_U8X16:
      return "u8x16";
    case BRIG_TYPE_U16X2:
      return "u16x2";
    case BRIG_TYPE_U16X4:
      return "u16x4";
    case BRIG_TYPE_U16X8:
      return "u16x8";
    case BRIG_TYPE_U32X2:
      return "u32x2";
    case BRIG_TYPE_U32X4:
      return "u32x4";
    case BRIG_TYPE_U64X2:
      return "u64x2";
    case BRIG_TYPE_S8X4:
      return "s8x4";
    case BRIG_TYPE_S8X8:
      return "s8x8";
    case BRIG_TYPE_S8X16:
      return "s8x16";
    case BRIG_TYPE_S16X2:
      return "s16x2";
    case BRIG_TYPE_S16X4:
      return "s16x4";
    case BRIG_TYPE_S16X8:
      return "s16x8";
    case BRIG_TYPE_S32X2:
      return "s32x2";
    case BRIG_TYPE_S32X4:
      return "s32x4";
    case BRIG_TYPE_S64X2:
      return "s64x2";
    case BRIG_TYPE_F16X2:
      return "f16x2";
    case BRIG_TYPE_F16X4:
      return "f16x4";
    case BRIG_TYPE_F16X8:
      return "f16x8";
    case BRIG_TYPE_F32X2:
      return "f32x2";
    case BRIG_TYPE_F32X4:
      return "f32x4";
    case BRIG_TYPE_F64X2:
      return "f64x2";
    default:
      return "UNKNOWN_TYPE";
    }
}

/* Return textual name of OPCODE.  */

static const char *
hsa_opcode_name (BrigOpcode16_t opcode)
{
  switch (opcode)
    {
    case BRIG_OPCODE_NOP:
      return "nop";
    case BRIG_OPCODE_ABS:
      return "abs";
    case BRIG_OPCODE_ADD:
      return "add";
    case BRIG_OPCODE_BORROW:
      return "borrow";
    case BRIG_OPCODE_CARRY:
      return "carry";
    case BRIG_OPCODE_CEIL:
      return "ceil";
    case BRIG_OPCODE_COPYSIGN:
      return "copysign";
    case BRIG_OPCODE_DIV:
      return "div";
    case BRIG_OPCODE_FLOOR:
      return "floor";
    case BRIG_OPCODE_FMA:
      return "fma";
    case BRIG_OPCODE_FRACT:
      return "fract";
    case BRIG_OPCODE_MAD:
      return "mad";
    case BRIG_OPCODE_MAX:
      return "max";
    case BRIG_OPCODE_MIN:
      return "min";
    case BRIG_OPCODE_MUL:
      return "mul";
    case BRIG_OPCODE_MULHI:
      return "mulhi";
    case BRIG_OPCODE_NEG:
      return "neg";
    case BRIG_OPCODE_REM:
      return "rem";
    case BRIG_OPCODE_RINT:
      return "rint";
    case BRIG_OPCODE_SQRT:
      return "sqrt";
    case BRIG_OPCODE_SUB:
      return "sub";
    case BRIG_OPCODE_TRUNC:
      return "trunc";
    case BRIG_OPCODE_MAD24:
      return "mad24";
    case BRIG_OPCODE_MAD24HI:
      return "mad24hi";
    case BRIG_OPCODE_MUL24:
      return "mul24";
    case BRIG_OPCODE_MUL24HI:
      return "mul24hi";
    case BRIG_OPCODE_SHL:
      return "shl";
    case BRIG_OPCODE_SHR:
      return "shr";
    case BRIG_OPCODE_AND:
      return "and";
    case BRIG_OPCODE_NOT:
      return "not";
    case BRIG_OPCODE_OR:
      return "or";
    case BRIG_OPCODE_POPCOUNT:
      return "popcount";
    case BRIG_OPCODE_XOR:
      return "xor";
    case BRIG_OPCODE_BITEXTRACT:
      return "bitextract";
    case BRIG_OPCODE_BITINSERT:
      return "bitinsert";
    case BRIG_OPCODE_BITMASK:
      return "bitmask";
    case BRIG_OPCODE_BITREV:
      return "bitrev";
    case BRIG_OPCODE_BITSELECT:
      return "bitselect";
    case BRIG_OPCODE_FIRSTBIT:
      return "firstbit";
    case BRIG_OPCODE_LASTBIT:
      return "lastbit";
    case BRIG_OPCODE_COMBINE:
      return "combine";
    case BRIG_OPCODE_EXPAND:
      return "expand";
    case BRIG_OPCODE_LDA:
      return "lda";
    case BRIG_OPCODE_MOV:
      return "mov";
    case BRIG_OPCODE_SHUFFLE:
      return "shuffle";
    case BRIG_OPCODE_UNPACKHI:
      return "unpackhi";
    case BRIG_OPCODE_UNPACKLO:
      return "unpacklo";
    case BRIG_OPCODE_PACK:
      return "pack";
    case BRIG_OPCODE_UNPACK:
      return "unpack";
    case BRIG_OPCODE_CMOV:
      return "cmov";
    case BRIG_OPCODE_CLASS:
      return "class";
    case BRIG_OPCODE_NCOS:
      return "ncos";
    case BRIG_OPCODE_NEXP2:
      return "nexp2";
    case BRIG_OPCODE_NFMA:
      return "nfma";
    case BRIG_OPCODE_NLOG2:
      return "nlog2";
    case BRIG_OPCODE_NRCP:
      return "nrcp";
    case BRIG_OPCODE_NRSQRT:
      return "nrsqrt";
    case BRIG_OPCODE_NSIN:
      return "nsin";
    case BRIG_OPCODE_NSQRT:
      return "nsqrt";
    case BRIG_OPCODE_BITALIGN:
      return "bitalign";
    case BRIG_OPCODE_BYTEALIGN:
      return "bytealign";
    case BRIG_OPCODE_PACKCVT:
      return "packcvt";
    case BRIG_OPCODE_UNPACKCVT:
      return "unpackcvt";
    case BRIG_OPCODE_LERP:
      return "lerp";
    case BRIG_OPCODE_SAD:
      return "sad";
    case BRIG_OPCODE_SADHI:
      return "sadhi";
    case BRIG_OPCODE_SEGMENTP:
      return "segmentp";
    case BRIG_OPCODE_FTOS:
      return "ftos";
    case BRIG_OPCODE_STOF:
      return "stof";
    case BRIG_OPCODE_CMP:
      return "cmp";
    case BRIG_OPCODE_CVT:
      return "cvt";
    case BRIG_OPCODE_LD:
      return "ld";
    case BRIG_OPCODE_ST:
      return "st";
    case BRIG_OPCODE_ATOMIC:
      return "atomic";
    case BRIG_OPCODE_ATOMICNORET:
      return "atomicnoret";
    case BRIG_OPCODE_SIGNAL:
      return "signal";
    case BRIG_OPCODE_SIGNALNORET:
      return "signalnoret";
    case BRIG_OPCODE_MEMFENCE:
      return "memfence";
    case BRIG_OPCODE_RDIMAGE:
      return "rdimage";
    case BRIG_OPCODE_LDIMAGE:
      return "ldimage";
    case BRIG_OPCODE_STIMAGE:
      return "stimage";
    case BRIG_OPCODE_QUERYIMAGE:
      return "queryimage";
    case BRIG_OPCODE_QUERYSAMPLER:
      return "querysampler";
    case BRIG_OPCODE_CBR:
      return "cbr";
    case BRIG_OPCODE_BR:
      return "br";
    case BRIG_OPCODE_SBR:
      return "sbr";
    case BRIG_OPCODE_BARRIER:
      return "barrier";
    case BRIG_OPCODE_WAVEBARRIER:
      return "wavebarrier";
    case BRIG_OPCODE_ARRIVEFBAR:
      return "arrivefbar";
    case BRIG_OPCODE_INITFBAR:
      return "initfbar";
    case BRIG_OPCODE_JOINFBAR:
      return "joinfbar";
    case BRIG_OPCODE_LEAVEFBAR:
      return "leavefbar";
    case BRIG_OPCODE_RELEASEFBAR:
      return "releasefbar";
    case BRIG_OPCODE_WAITFBAR:
      return "waitfbar";
    case BRIG_OPCODE_LDF:
      return "ldf";
    case BRIG_OPCODE_ACTIVELANECOUNT:
      return "activelanecount";
    case BRIG_OPCODE_ACTIVELANEID:
      return "activelaneid";
    case BRIG_OPCODE_ACTIVELANEMASK:
      return "activelanemask";
    case BRIG_OPCODE_CALL:
      return "call";
    case BRIG_OPCODE_SCALL:
      return "scall";
    case BRIG_OPCODE_ICALL:
      return "icall";
    case BRIG_OPCODE_RET:
      return "ret";
    case BRIG_OPCODE_ALLOCA:
      return "alloca";
    case BRIG_OPCODE_CURRENTWORKGROUPSIZE:
      return "currentworkgroupsize";
    case BRIG_OPCODE_DIM:
      return "dim";
    case BRIG_OPCODE_GRIDGROUPS:
      return "gridgroups";
    case BRIG_OPCODE_GRIDSIZE:
      return "gridsize";
    case BRIG_OPCODE_PACKETCOMPLETIONSIG:
      return "packetcompletionsig";
    case BRIG_OPCODE_PACKETID:
      return "packetid";
    case BRIG_OPCODE_WORKGROUPID:
      return "workgroupid";
    case BRIG_OPCODE_WORKGROUPSIZE:
      return "workgroupsize";
    case BRIG_OPCODE_WORKITEMABSID:
      return "workitemabsid";
    case BRIG_OPCODE_WORKITEMFLATABSID:
      return "workitemflatabsid";
    case BRIG_OPCODE_WORKITEMFLATID:
      return "workitemflatid";
    case BRIG_OPCODE_WORKITEMID:
      return "workitemid";
    case BRIG_OPCODE_CLEARDETECTEXCEPT:
      return "cleardetectexcept";
    case BRIG_OPCODE_GETDETECTEXCEPT:
      return "getdetectexcept";
    case BRIG_OPCODE_SETDETECTEXCEPT:
      return "setdetectexcept";
    case BRIG_OPCODE_ADDQUEUEWRITEINDEX:
      return "addqueuewriteindex";
    case BRIG_OPCODE_CASQUEUEWRITEINDEX:
      return "casqueuewriteindex";
    case BRIG_OPCODE_LDQUEUEREADINDEX:
      return "ldqueuereadindex";
    case BRIG_OPCODE_LDQUEUEWRITEINDEX:
      return "ldqueuewriteindex";
    case BRIG_OPCODE_STQUEUEREADINDEX:
      return "stqueuereadindex";
    case BRIG_OPCODE_STQUEUEWRITEINDEX:
      return "stqueuewriteindex";
    case BRIG_OPCODE_CLOCK:
      return "clock";
    case BRIG_OPCODE_CUID:
      return "cuid";
    case BRIG_OPCODE_DEBUGTRAP:
      return "debugtrap";
    case BRIG_OPCODE_GROUPBASEPTR:
      return "groupbaseptr";
    case BRIG_OPCODE_KERNARGBASEPTR:
      return "kernargbaseptr";
    case BRIG_OPCODE_LANEID:
      return "laneid";
    case BRIG_OPCODE_MAXCUID:
      return "maxcuid";
    case BRIG_OPCODE_MAXWAVEID:
      return "maxwaveid";
    case BRIG_OPCODE_NULLPTR:
      return "nullptr";
    case BRIG_OPCODE_WAVEID:
      return "waveid";
    default:
      return "UNKNOWN_OPCODE";
    }
}

/* Return textual name of SEG.  */

const char *
hsa_seg_name (BrigSegment8_t seg)
{
  switch (seg)
    {
    case BRIG_SEGMENT_NONE:
      return "none";
    case BRIG_SEGMENT_FLAT:
      return "flat";
    case BRIG_SEGMENT_GLOBAL:
      return "global";
    case BRIG_SEGMENT_READONLY:
      return "readonly";
    case BRIG_SEGMENT_KERNARG:
      return "kernarg";
    case BRIG_SEGMENT_GROUP:
      return "group";
    case BRIG_SEGMENT_PRIVATE:
      return "private";
    case BRIG_SEGMENT_SPILL:
      return "spill";
    case BRIG_SEGMENT_ARG:
      return "arg";
    default:
      return "UNKNOWN_SEGMENT";
    }
}

/* Return textual name of CMPOP.  */

static const char *
hsa_cmpop_name (BrigCompareOperation8_t cmpop)
{
  switch (cmpop)
    {
    case BRIG_COMPARE_EQ:
      return "eq";
    case BRIG_COMPARE_NE:
      return "ne";
    case BRIG_COMPARE_LT:
      return "lt";
    case BRIG_COMPARE_LE:
      return "le";
    case BRIG_COMPARE_GT:
      return "gt";
    case BRIG_COMPARE_GE:
      return "ge";
    case BRIG_COMPARE_EQU:
      return "equ";
    case BRIG_COMPARE_NEU:
      return "neu";
    case BRIG_COMPARE_LTU:
      return "ltu";
    case BRIG_COMPARE_LEU:
      return "leu";
    case BRIG_COMPARE_GTU:
      return "gtu";
    case BRIG_COMPARE_GEU:
      return "geu";
    case BRIG_COMPARE_NUM:
      return "num";
    case BRIG_COMPARE_NAN:
      return "nan";
    case BRIG_COMPARE_SEQ:
      return "seq";
    case BRIG_COMPARE_SNE:
      return "sne";
    case BRIG_COMPARE_SLT:
      return "slt";
    case BRIG_COMPARE_SLE:
      return "sle";
    case BRIG_COMPARE_SGT:
      return "sgt";
    case BRIG_COMPARE_SGE:
      return "sge";
    case BRIG_COMPARE_SGEU:
      return "sgeu";
    case BRIG_COMPARE_SEQU:
      return "sequ";
    case BRIG_COMPARE_SNEU:
      return "sneu";
    case BRIG_COMPARE_SLTU:
      return "sltu";
    case BRIG_COMPARE_SLEU:
      return "sleu";
    case BRIG_COMPARE_SNUM:
      return "snum";
    case BRIG_COMPARE_SNAN:
      return "snan";
    case BRIG_COMPARE_SGTU:
      return "sgtu";
    default:
      return "UNKNOWN_COMPARISON";
    }
}

/* Return textual name for memory order.  */

static const char *
hsa_memsem_name (enum BrigMemoryOrder mo)
{
  switch (mo)
    {
    case BRIG_MEMORY_ORDER_NONE:
      return "";
    case BRIG_MEMORY_ORDER_RELAXED:
      return "rlx";
    case BRIG_MEMORY_ORDER_SC_ACQUIRE:
      return "scacq";
    case BRIG_MEMORY_ORDER_SC_RELEASE:
      return "screl";
    case BRIG_MEMORY_ORDER_SC_ACQUIRE_RELEASE:
      return "scar";
    default:
      return "UNKNOWN_MEMORY_ORDER";
    }
}

/* Return textual name for memory scope. */

static const char *
hsa_memscope_name (enum BrigMemoryScope scope)
{
  switch (scope)
    {
    case BRIG_MEMORY_SCOPE_NONE:
      return "";
    case BRIG_MEMORY_SCOPE_WORKITEM:
      return "wi";
    case BRIG_MEMORY_SCOPE_WAVEFRONT:
      return "wave";
    case BRIG_MEMORY_SCOPE_WORKGROUP:
      return "wg";
    case BRIG_MEMORY_SCOPE_AGENT:
      return "agent";
    case BRIG_MEMORY_SCOPE_SYSTEM:
      return "sys";
    default:
      return "UNKNOWN_SCOPE";
    }
}

/* Return textual name for atomic operation.  */

static const char *
hsa_atomicop_name (enum BrigAtomicOperation op)
{
  switch (op)
    {
    case BRIG_ATOMIC_ADD:
      return "add";
    case BRIG_ATOMIC_AND:
      return "and";
    case BRIG_ATOMIC_CAS:
      return "cas";
    case BRIG_ATOMIC_EXCH:
      return "exch";
    case BRIG_ATOMIC_LD:
      return "ld";
    case BRIG_ATOMIC_MAX:
      return "max";
    case BRIG_ATOMIC_MIN:
      return "min";
    case BRIG_ATOMIC_OR:
      return "or";
    case BRIG_ATOMIC_ST:
      return "st";
    case BRIG_ATOMIC_SUB:
      return "sub";
    case BRIG_ATOMIC_WRAPDEC:
      return "wrapdec";
    case BRIG_ATOMIC_WRAPINC:
      return "wrapinc";
    case BRIG_ATOMIC_XOR:
      return "xor";
    case BRIG_ATOMIC_WAIT_EQ:
      return "wait_eq";
    case BRIG_ATOMIC_WAIT_NE:
      return "wait_ne";
    case BRIG_ATOMIC_WAIT_LT:
      return "wait_lt";
    case BRIG_ATOMIC_WAIT_GTE:
      return "wait_gte";
    case BRIG_ATOMIC_WAITTIMEOUT_EQ:
      return "waittimeout_eq";
    case BRIG_ATOMIC_WAITTIMEOUT_NE:
      return "waittimeout_ne";
    case BRIG_ATOMIC_WAITTIMEOUT_LT:
      return "waittimeout_lt";
    case BRIG_ATOMIC_WAITTIMEOUT_GTE:
      return "waittimeout_gte";
    default:
      return "UNKNOWN_ATOMIC_OP";
    }
}

/* Dump textual representation of HSA IL register REG to file F.  */

static void
dump_hsa_reg (FILE *f, hsa_op_reg *reg, bool dump_type = false)
{
  if (reg->reg_class)
    fprintf (f, "$%c%i", reg->reg_class, reg->hard_num);
  else
    fprintf (f, "$_%i", reg->order);
  if (dump_type)
    fprintf (f, " (%s)", hsa_type_name (reg->type));
}

/* Dump textual representation of HSA IL immediate operand IMM to file F.  */

static void
dump_hsa_immed (FILE *f, hsa_op_immed *imm)
{
  print_generic_expr (f, imm->value, 0);
  fprintf (f, " (%s)", hsa_type_name (imm->type));
}

/* Dump textual representation of HSA IL address operand ADDR to file F.  */

static void
dump_hsa_address (FILE *f, hsa_op_address *addr)
{
  bool sth = false;

  if (addr->symbol)
    {
      sth = true;
      if (addr->symbol->name)
	fprintf (f, "[%%%s]", addr->symbol->name);
      else
	fprintf (f, "[%%__%s_%i]", hsa_seg_name(addr->symbol->segment),
		 addr->symbol->name_number);
    }

  if (addr->reg)
    {
      fprintf (f, "[");
      dump_hsa_reg (f, addr->reg);
      if (addr->imm_offset != 0)
	fprintf (f, " + " HOST_WIDE_INT_PRINT_DEC "]", addr->imm_offset);
      else
	fprintf (f, "]");
    }
  else if (!sth || addr->imm_offset != 0)
    fprintf (f, "[" HOST_WIDE_INT_PRINT_DEC "]", addr->imm_offset);
}

/* Dump textual representation of HSA IL operand OP to file F.  */

static void
dump_hsa_operand (FILE *f, hsa_op_base *op, bool dump_reg_type = false)
{
  if (is_a <hsa_op_immed *> (op))
    dump_hsa_immed (f, as_a <hsa_op_immed *> (op));
  else if (is_a <hsa_op_reg *> (op))
    dump_hsa_reg (f, as_a <hsa_op_reg *> (op), dump_reg_type);
  else if (is_a <hsa_op_address *> (op))
    dump_hsa_address (f, as_a <hsa_op_address *> (op));
  else
    fprintf (f, "UNKNOWN_OP_KIND");
}

/* Dump textual representation of HSA IL operands in VEC to file F.  */

static void
dump_hsa_operands (FILE *f, vec <hsa_op_base *> &operands,
		   bool dump_reg_type = false)
{
  for (unsigned i = 0; i < operands.length (); i++)
    {
      dump_hsa_operand (f, operands[i], dump_reg_type);
      if (i != operands.length () - 1)
	fprintf (f, ", ");
    }
}

/* Indent F stream with INDENT spaces.  */

static void indent_stream (FILE *f, int indent)
{
  for (int i = 0; i < indent; i++)
    fputc (' ', f);
}

/* Dump textual representation of HSA IL instruction INSN to file F.  */

static void
dump_hsa_insn (FILE *f, hsa_insn_basic *insn, int *indent)
{
  gcc_checking_assert (insn);
  indent_stream (f, *indent);

  if (insn->number)
    fprintf (f, "%5d: ", insn->number);

  if (is_a <hsa_insn_phi *> (insn))
    {
      hsa_insn_phi *phi = as_a <hsa_insn_phi *> (insn);
      bool first = true;
      dump_hsa_reg (f, phi->dest, true);
      fprintf (f, " = PHI <");
      unsigned count = phi->operands.length ();
      for (unsigned i = 0; i < count; i++)
	{
	  if (!phi->operands[i])
	    break;
	  if (!first)
	    fprintf (f, ", ");
	  else
	    first = false;
	  dump_hsa_operand (f, phi->operands[i], true);
	}
      fprintf (f, ">\n");
    }
  else if (is_a <hsa_insn_signal *> (insn))
    {
      hsa_insn_signal *mem = as_a <hsa_insn_signal *> (insn);

      fprintf (f, "%s", hsa_opcode_name (mem->opcode));
      fprintf (f, "_%s", hsa_atomicop_name (mem->atomicop));
      if (mem->memoryorder != BRIG_MEMORY_ORDER_NONE)
	fprintf (f, "_%s", hsa_memsem_name (mem->memoryorder));
      fprintf (f, "_%s ", hsa_type_name (mem->type));

      dump_hsa_operands (f, mem->operands);
      fprintf (f, "\n");
    }

  else if (is_a <hsa_insn_atomic *> (insn))
    {
      hsa_insn_atomic *mem = as_a <hsa_insn_atomic *> (insn);

      /* Either operand[0] or operand[1] must be an address operand.  */
      hsa_op_address *addr = NULL;
      if (is_a <hsa_op_address *> (mem->operands[0]))
	addr = as_a <hsa_op_address *> (mem->operands[0]);
      else
	addr = as_a <hsa_op_address *> (mem->operands[1]);

      fprintf (f, "%s", hsa_opcode_name (mem->opcode));
      fprintf (f, "_%s", hsa_atomicop_name (mem->atomicop));
      if (addr->symbol)
	fprintf (f, "_%s", hsa_seg_name (addr->symbol->segment));
      if (mem->memoryorder != BRIG_MEMORY_ORDER_NONE)
	fprintf (f, "_%s", hsa_memsem_name (mem->memoryorder));
      if (mem->memoryscope != BRIG_MEMORY_SCOPE_NONE)
	fprintf (f, "_%s", hsa_memscope_name (mem->memoryscope));
      fprintf (f, "_%s ", hsa_type_name (mem->type));

      dump_hsa_operands (f, mem->operands);
      fprintf (f, "\n");
    }
  else if (is_a <hsa_insn_mem *> (insn))
    {
      hsa_insn_mem *mem = as_a <hsa_insn_mem *> (insn);
      hsa_op_address *addr = as_a <hsa_op_address *> (mem->operands[1]);

      fprintf (f, "%s", hsa_opcode_name (mem->opcode));
      if (addr->symbol)
	fprintf (f, "_%s", hsa_seg_name (addr->symbol->segment));
      if (mem->memoryorder != BRIG_MEMORY_ORDER_NONE)
	fprintf (f, "_%s", hsa_memsem_name (mem->memoryorder));
      if (mem->memoryscope != BRIG_MEMORY_SCOPE_NONE)
	fprintf (f, "_%s", hsa_memscope_name (mem->memoryscope));
      if (mem->equiv_class != 0)
	fprintf (f, "_equiv(%i)", mem->equiv_class);
      fprintf (f, "_%s ", hsa_type_name (mem->type));

      dump_hsa_operand (f, mem->operands[0]);
      fprintf (f, ", ");
      dump_hsa_address (f, addr);
      fprintf (f, "\n");
    }
  else if (insn->opcode == BRIG_OPCODE_LDA)
    {
      hsa_op_address *addr = as_a <hsa_op_address *> (insn->operands[1]);

      fprintf (f, "%s", hsa_opcode_name (insn->opcode));
      if (addr->symbol)
	fprintf (f, "_%s", hsa_seg_name (addr->symbol->segment));
      fprintf (f, "_%s ", hsa_type_name (insn->type));

      dump_hsa_operand (f, insn->operands[0]);
      fprintf (f, ", ");
      dump_hsa_address (f, addr);
      fprintf (f, "\n");
    }
  else if (is_a <hsa_insn_seg *> (insn))
    {
      hsa_insn_seg *seg = as_a <hsa_insn_seg *> (insn);
      fprintf (f, "%s_%s_%s_%s ", hsa_opcode_name (seg->opcode),
	       hsa_seg_name (seg->segment),
	       hsa_type_name (seg->type), hsa_type_name (seg->src_type));
      dump_hsa_reg (f, as_a <hsa_op_reg *> (seg->operands[0]));
      fprintf (f, ", ");
      dump_hsa_operand (f, seg->operands[1]);
      fprintf (f, "\n");
    }
  else if (is_a <hsa_insn_cmp *> (insn))
    {
      hsa_insn_cmp *cmp = as_a <hsa_insn_cmp *> (insn);
      BrigType16_t src_type;

      if (is_a <hsa_op_reg *> (cmp->operands[1]))
	src_type = as_a <hsa_op_reg *> (cmp->operands[1])->type;
      else
	src_type = as_a <hsa_op_immed *> (cmp->operands[1])->type;

      fprintf (f, "%s_%s_%s_%s ", hsa_opcode_name (cmp->opcode),
	       hsa_cmpop_name (cmp->compare),
	       hsa_type_name (cmp->type), hsa_type_name (src_type));
      dump_hsa_reg (f, as_a <hsa_op_reg *> (cmp->operands[0]));
      fprintf (f, ", ");
      dump_hsa_operand (f, cmp->operands[1]);
      fprintf (f, ", ");
      dump_hsa_operand (f, cmp->operands[2]);
      fprintf (f, "\n");
    }
  else if (is_a <hsa_insn_br *> (insn))
    {
      hsa_insn_br *br = as_a <hsa_insn_br *> (insn);
      basic_block target = NULL;
      edge_iterator ei;
      edge e;

      /* FIXME: We onlu assume direct jumps now.  */
      fprintf (f, "%s ", hsa_opcode_name (br->opcode));
      if (br->opcode == BRIG_OPCODE_CBR)
	{
	  dump_hsa_reg (f, as_a <hsa_op_reg *> (br->operands[0]));
	  fprintf (f, ", ");
	}

      FOR_EACH_EDGE (e, ei, br->bb->succs)
	if (e->flags & EDGE_TRUE_VALUE)
	  {
	    target = e->dest;
	    break;
	  }
      fprintf (f, "BB %i\n", hsa_bb_for_bb (target)->index);
    }
  else if (is_a <hsa_insn_arg_block *> (insn))
    {
      hsa_insn_arg_block *arg_block = as_a <hsa_insn_arg_block *> (insn);
      bool start_p = arg_block->kind == BRIG_KIND_DIRECTIVE_ARG_BLOCK_START;
      char c = start_p ? '{' : '}';

      if (start_p)
	{
	  *indent += 2;
	  indent_stream (f, 2);
	}

      if (!start_p)
	*indent -= 2;

      fprintf (f, "%c\n", c);
    }
  else if (is_a <hsa_insn_call *> (insn))
    {
      hsa_insn_call *call = as_a <hsa_insn_call *> (insn);
      const char *name = get_declaration_name (call->called_function);

      fprintf (f, "call &%s", name);

      if (call->result_symbol)
	fprintf (f, "(%%res) ");

      fprintf (f, "(");
      for (unsigned i = 0; i < call->args_symbols.length (); i++)
        {
	  fprintf (f, "%%__arg_%u", i);

	  if (i != call->args_symbols.length () - 1)
	    fprintf (f, ", ");
	}
      fprintf (f, ")\n");
    }
  else if (is_a <hsa_insn_comment *> (insn))
    {
      fprintf (f, "%s\n", as_a <hsa_insn_comment *> (insn)->comment);
    }
  else
    {
      bool first = true;
      fprintf (f, "%s_%s ", hsa_opcode_name (insn->opcode),
	       hsa_type_name (insn->type));

      unsigned count = insn->operands.length ();
      for (unsigned i = 0; i < count; i++)
	{
	  hsa_op_base *op = insn->operands[i];

	  if (!op)
	    break;
	  if (!first)
	    fprintf (f, ", ");
	  else
	    first = false;

	  if (is_a <hsa_op_immed *> (op))
	    dump_hsa_immed (f, as_a <hsa_op_immed *> (op));
	  else if (is_a <hsa_op_reg *> (op))
	    dump_hsa_reg (f, as_a <hsa_op_reg *> (op));
	  else if (is_a <hsa_op_address *> (op))
	    dump_hsa_address (f, as_a <hsa_op_address *> (op));
	  else
	    fprintf (f, "UNKNOWN_OP_KIND");
	}
      fprintf (f, "\n");
    }
}

/* Dump textual representation of HSA IL in HBB to file F.  */

void
dump_hsa_bb (FILE *f, hsa_bb *hbb)
{
  hsa_insn_basic *insn;
  edge_iterator ei;
  edge e;
  basic_block true_bb = NULL, other = NULL;

  /* FIXME: Dump a label or something instead.  */
  fprintf (f, "BB %i:\n", hbb->index);

  int indent = 2;
  for (insn = hbb->first_phi; insn; insn = insn->next)
    dump_hsa_insn (f, insn, &indent);

  for (insn = hbb->first_insn; insn; insn = insn->next)
    dump_hsa_insn (f, insn, &indent);

  FOR_EACH_EDGE (e, ei, hbb->bb->succs)
    if (e->flags & EDGE_TRUE_VALUE)
      {
	gcc_assert (!true_bb);
	true_bb = e->dest;
      }
    else
      {
	gcc_assert (!other);
	other = e->dest;
      }

  if (true_bb)
    {
      if (!hbb->last_insn
	  || hbb->last_insn->opcode != BRIG_OPCODE_CBR)
	fprintf (f, "WARNING: No branch insn for a true edge. \n");
    }
  else if (hbb->last_insn
	   && hbb->last_insn->opcode == BRIG_OPCODE_CBR)
    fprintf (f, "WARNING: No true edge for a cbr statement\n");

  if (other && other->aux)
    fprintf (f, "  Fall-through to BB %i\n",
	     hsa_bb_for_bb (other)->index);
  else if (hbb->last_insn
	   && hbb->last_insn->opcode != BRIG_OPCODE_RET)
    fprintf (f, "  WARNING: Fall through to a BB with no aux!\n");

  fprintf (f, "\n");
}

/* Dump textual representation of HSA IL of the current function to file F.  */

void
dump_hsa_cfun (FILE *f)
{
  basic_block bb;

  fprintf (f, "\nHSAIL IL for %s\n", hsa_cfun->name);

  dump_hsa_bb (f, &hsa_cfun->prologue);
  FOR_EACH_BB_FN (bb, cfun)
  {
    hsa_bb *hbb = (struct hsa_bb *) bb->aux;
    dump_hsa_bb (f, hbb);
  }
}

/* Dump textual representation of HSA IL instruction INSN to stderr.  */

DEBUG_FUNCTION void
debug_hsa_insn (hsa_insn_basic *insn)
{
  int indentation = 0;
  dump_hsa_insn (stderr, insn, &indentation);
}

/* Dump textual representation of HSA IL in HBB to stderr.  */

DEBUG_FUNCTION void
debug_hsa_bb (hsa_bb *hbb)
{
  dump_hsa_bb (stderr, hbb);
}

/* Dump textual representation of HSA IL of the current function to stderr.  */

DEBUG_FUNCTION void
debug_hsa_cfun (void)
{
  dump_hsa_cfun (stderr);
}
