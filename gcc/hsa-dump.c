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
#include "hsa.h"
#include "tm.h"
#include "tree.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-expr.h"
#include "gimple.h"
#include "basic-block.h"
#include "vec.h"
#include "gimple-pretty-print.h"

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
    case BRIG_TYPE_RWIMG:
      return "rwimg";
    case BRIG_TYPE_FBAR:
      return "fbar";
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
    case BRIG_OPCODE_LDC:
      return "ldc";
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
    case BRIG_OPCODE_RDIMAGE:
      return "rdimage";
    case BRIG_OPCODE_LDIMAGE:
      return "ldimage";
    case BRIG_OPCODE_STIMAGE:
      return "stimage";
    case BRIG_OPCODE_ATOMICIMAGE:
      return "atomicimage";
    case BRIG_OPCODE_ATOMICIMAGENORET:
      return "atomicimagenoret";
    case BRIG_OPCODE_QUERYIMAGEARRAY:
      return "queryimagearray";
    case BRIG_OPCODE_QUERYIMAGEDEPTH:
      return "queryimagedepth";
    case BRIG_OPCODE_QUERYIMAGEFORMAT:
      return "queryimageformat";
    case BRIG_OPCODE_QUERYIMAGEHEIGHT:
      return "queryimageheight";
    case BRIG_OPCODE_QUERYIMAGEORDER:
      return "queryimageorder";
    case BRIG_OPCODE_QUERYIMAGEWIDTH:
      return "queryimagewidth";
    case BRIG_OPCODE_QUERYSAMPLERCOORD:
      return "querysamplercoord";
    case BRIG_OPCODE_QUERYSAMPLERFILTER:
      return "querysamplerfilter";
    case BRIG_OPCODE_CBR:
      return "cbr";
    case BRIG_OPCODE_BRN:
      return "brn";
    case BRIG_OPCODE_BARRIER:
      return "barrier";
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
    case BRIG_OPCODE_SYNC:
      return "sync";
    case BRIG_OPCODE_COUNTLANE:
      return "countlane";
    case BRIG_OPCODE_COUNTUPLANE:
      return "countuplane";
    case BRIG_OPCODE_MASKLANE:
      return "masklane";
    case BRIG_OPCODE_SENDLANE:
      return "sendlane";
    case BRIG_OPCODE_RECEIVELANE:
      return "receivelane";
    case BRIG_OPCODE_CALL:
      return "call";
    case BRIG_OPCODE_RET:
      return "ret";
    case BRIG_OPCODE_SYSCALL:
      return "syscall";
    case BRIG_OPCODE_ALLOCA:
      return "alloca";
    case BRIG_OPCODE_CLEARDETECTEXCEPT:
      return "cleardetectexcept";
    case BRIG_OPCODE_CLOCK:
      return "clock";
    case BRIG_OPCODE_CUID:
      return "cuid";
    case BRIG_OPCODE_CURRENTWORKGROUPSIZE:
      return "currentworkgroupsize";
    case BRIG_OPCODE_DEBUGTRAP:
      return "debugtrap";
    case BRIG_OPCODE_DIM:
      return "dim";
    case BRIG_OPCODE_DISPATCHID:
      return "dispatchid";
    case BRIG_OPCODE_DISPATCHPTR:
      return "dispatchptr";
    case BRIG_OPCODE_GETDETECTEXCEPT:
      return "getdetectexcept";
    case BRIG_OPCODE_GRIDGROUPS:
      return "gridgroups";
    case BRIG_OPCODE_GRIDSIZE:
      return "gridsize";
    case BRIG_OPCODE_LANEID:
      return "laneid";
    case BRIG_OPCODE_MAXCUID:
      return "maxcuid";
    case BRIG_OPCODE_MAXWAVEID:
      return "maxwaveid";
    case BRIG_OPCODE_NULLPTR:
      return "nullptr";
    case BRIG_OPCODE_QID:
      return "qid";
    case BRIG_OPCODE_QPTR:
      return "qptr";
    case BRIG_OPCODE_SETDETECTEXCEPT:
      return "setdetectexcept";
    case BRIG_OPCODE_WAVEID:
      return "waveid";
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

/* Return textual name for memory semantics.  */

static const char *
hsa_memsem_name (enum BrigMemorySemantic sem)
{
  switch (sem)
    {
    case BRIG_SEMANTIC_NONE:
    case BRIG_SEMANTIC_REGULAR:
      return "";
    case BRIG_SEMANTIC_ACQUIRE:
      return "acq";
    case BRIG_SEMANTIC_RELEASE:
      return "rel";
    case BRIG_SEMANTIC_ACQUIRE_RELEASE:
      return "ar";
    case BRIG_SEMANTIC_PARTIAL_ACQUIRE:
      return "part_acq";
    case BRIG_SEMANTIC_PARTIAL_RELEASE:
      return "part_rel";
    case BRIG_SEMANTIC_PARTIAL_ACQUIRE_RELEASE:
      return "part_ar";
    default:
      return "UNKNOWN_SEMANTIC";
    }

}

static const char *
hsa_atomicop_name (enum BrigAtomicOperation op)
{
  switch (op)
    {
    case BRIG_ATOMIC_AND:
      return "and";
    case BRIG_ATOMIC_OR:
      return "or";
    case BRIG_ATOMIC_XOR:
      return "xor";
    case BRIG_ATOMIC_CAS:
      return "cas";
    case BRIG_ATOMIC_EXCH:
      return "exch";
    case BRIG_ATOMIC_ADD:
      return "add";
    case BRIG_ATOMIC_INC:
      return "inc";
    case BRIG_ATOMIC_DEC:
      return "dec";
    case BRIG_ATOMIC_MIN:
      return "min";
    case BRIG_ATOMIC_MAX:
      return "max";
    case BRIG_ATOMIC_SUB:
      return "sub";
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

/* Dump textual representation of HSA IL register or immedate value OP to file
   F.  */

static void
dump_hsa_imm_or_reg (FILE *f, hsa_op_base *op, bool dump_reg_type = false)
{
  if (is_a <hsa_op_reg *> (op))
    dump_hsa_reg (f, as_a <hsa_op_reg *> (op), dump_reg_type);
  else
    dump_hsa_immed (f, as_a <hsa_op_immed *> (op));
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

/* Dump textual representation of HSA IL instruction INSN to file F.  */

static void
dump_hsa_insn (FILE *f, hsa_insn_basic *insn, int indent)
{
  gcc_checking_assert (insn);
  for (int i = 0; i < indent; i++)
    fputc (' ', f);

  if (is_a <hsa_insn_phi *> (insn))
    {
      hsa_insn_phi *phi = as_a <hsa_insn_phi *> (insn);
      bool first = true;
      dump_hsa_reg (f, phi->dest, true);
      fprintf (f, " = PHI <");
      for (int i = 0; i < HSA_OPERANDS_PER_INSN; i++)
	{
	  if (!phi->operands[i])
	    break;
	  if (!first)
	    fprintf (f, ", ");
	  else
	    first = false;
	  dump_hsa_imm_or_reg (f, phi->operands[i], true);
	}
      fprintf (f, ">\n");
    }
  else if (is_a <hsa_insn_atomic *> (insn))
    {
      hsa_insn_atomic *mem = as_a <hsa_insn_atomic *> (insn);
      hsa_op_address *addr = as_a <hsa_op_address *> (mem->operands[1]);

      fprintf (f, "%s", hsa_opcode_name (mem->opcode));
      fprintf (f, "_%s", hsa_atomicop_name (mem->atomicop));
      if (addr->symbol)
	fprintf (f, "_%s", hsa_seg_name (addr->symbol->segment));
      if (mem->semantic != BRIG_SEMANTIC_NONE
	  && mem->semantic != BRIG_SEMANTIC_REGULAR)
	fprintf (f, "_%s", hsa_memsem_name (mem->semantic));
      fprintf (f, "_%s ", hsa_type_name (mem->type));

      dump_hsa_imm_or_reg (f, mem->operands[0]);
      fprintf (f, ", ");
      dump_hsa_address (f, addr);
      fprintf (f, ", ");
      dump_hsa_imm_or_reg (f, mem->operands[2]);
      if (mem->atomicop == BRIG_ATOMIC_CAS)
	{
	  fprintf (f, ", ");
	  dump_hsa_imm_or_reg (f, mem->operands[3]);
	}
      fprintf (f, "\n");
    }
  else if (is_a <hsa_insn_mem *> (insn))
    {
      hsa_insn_mem *mem = as_a <hsa_insn_mem *> (insn);
      hsa_op_address *addr = as_a <hsa_op_address *> (mem->operands[1]);

      fprintf (f, "%s", hsa_opcode_name (mem->opcode));
      if (addr->symbol)
	fprintf (f, "_%s", hsa_seg_name (addr->symbol->segment));
      if (mem->semantic != BRIG_SEMANTIC_NONE
	  && mem->semantic != BRIG_SEMANTIC_REGULAR)
	fprintf (f, "_%s", hsa_memsem_name (mem->semantic));
      if (mem->equiv_class != 0)
	fprintf (f, "_equiv(%i)", mem->equiv_class);
      fprintf (f, "_%s ", hsa_type_name (mem->type));

      dump_hsa_imm_or_reg (f, mem->operands[0]);
      fprintf (f, ", ");
      dump_hsa_address (f, addr);
      fprintf (f, "\n");
    }
  else if (is_a <hsa_insn_addr *> (insn))
    {
      hsa_insn_addr *ia = as_a <hsa_insn_addr *> (insn);
      hsa_op_address *addr = as_a <hsa_op_address *> (ia->operands[1]);

      fprintf (f, "%s", hsa_opcode_name (ia->opcode));
      if (addr->symbol)
	fprintf (f, "_%s", hsa_seg_name (addr->symbol->segment));
      fprintf (f, "_%s ", hsa_type_name (ia->type));

      dump_hsa_imm_or_reg (f, ia->operands[0]);
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
      dump_hsa_imm_or_reg (f, seg->operands[1]);
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
      dump_hsa_imm_or_reg (f, cmp->operands[1]);
      fprintf (f, ", ");
      dump_hsa_imm_or_reg (f, cmp->operands[2]);
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
  else
    {
      bool first = true;
      fprintf (f, "%s_%s ", hsa_opcode_name (insn->opcode),
	       hsa_type_name (insn->type));

      for (int i = 0; i < HSA_OPERANDS_PER_INSN; i++)
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

  for (insn = hbb->first_phi; insn; insn = insn->next)
    dump_hsa_insn (f, insn, 2);
  for (insn = hbb->first_insn; insn; insn = insn->next)
    dump_hsa_insn (f, insn, 2);

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

  fprintf (f, "\nHSAIL IL for %s\n", hsa_cfun.name);

  dump_hsa_bb (f, &hsa_cfun.prologue);
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
  dump_hsa_insn (stderr, insn, 0);
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
