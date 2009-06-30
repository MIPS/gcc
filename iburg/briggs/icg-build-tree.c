/* Build a side tree for every real INSN, for GNU compiler.
   Copyright (C) 2008  Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "obstack.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "flags.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "icg.h"
#include "icg-opcode.h"
#include "insn-constants.h"
#include "output.h"


/*#define MAX_PARALLEL_KIDS 10*/

bool      *icg_insn2dirty;
icg_node **icg_insn2tree;
int       *icg_insn2goalNT;

/* The works, setting the op field of icg_node to one of
 * icg_opcodes
 */


/* Return true iff REG is dead at INSN.  This implementation is
   imperfect: all defs of registers will get type REGX.  Probably less
   confusing to give them REGX only if the def is unused.  */

static
bool reg_dead_p (rtx reg, rtx insn)
{
  /* Register is not dead if the expression has more than one use despite the
   *   fact that it may be dead after, put in this code.
   */
  if (count_occurrences(PATTERN(insn), reg, 0) > 1)
    return false;

  /* Register is dead if either data flow says so (REG_DEAD) or if
     INSN sets the register. */
  return (find_regno_note (insn, REG_DEAD, REGNO (reg)) != NULL_RTX
	  || reg_set_p (reg, insn));

}

/*
 * Given an rtx expression node,
 * return the type modifier for the burg terminal symbol enumeration.
 *
 * This is where we map gcc types to iburg terminal types.
 * The value returned here relies on the careful enumeration
 * of the terminals in the x86-64.gr file.
 *
 * ICG_TSTEP=14
 * The terminal number%ICG_TSTEP is the type.
 * The terminal number/ICG_TSTEP is the basic operator.
 *
 * For example, this is how the PLUS operators type variants
 * might be enumerated:
 */
static
icg_opcodes get_operator_mode(rtx rtx_expr)
{
  const enum machine_mode expr_mode = GET_MODE(rtx_expr);
  const enum rtx_code     expr_code = GET_CODE(rtx_expr);
  const bool scalar_equiv =
    (expr_code == SET || expr_code == REG || expr_code == MEM);

  int mode;
  switch (expr_mode) {
    case CCmode:	/* basic condition code mode */
    case CCGCmode:	/* garbage in Carry flag */
    case CCGOCmode:	/* garbage in Carry and Overflow flag */
    case CCNOmode:	/* comp with 0 that require Overflow flag to be unset */
    case CCAmode:	/* Above flag is valid */
    case CCCmode:	/* Carry flag is valid */
    case CCOmode:	/* Overflow flag is valid */
    case CCSmode:	/* Sign flag is valid */
    case CCZmode:	/* Zero flag is valid */
      mode = 0;
      break;

    case CCFPmode:
    case CCFPUmode:
      mode = 0;
      break;

    case VOIDmode:
      mode = 0;
      break;
    case DImode :
      mode = 0;	/* eg _DI */
      break;
    case SImode :
      mode = 1;	/* eg _SI */
      break;
    case HImode :
      mode = 2;	/* eg _HI */
      break;
    case QImode :
      mode = 3;	/* eg _QI */
      break;

    case TImode:	/* int128 */
      mode = 4;	/* eg _TI */
      break;
    case OImode:	/* int256 */
      mode = 5;	/* eg _OI */
      break;

    case DFmode :
      mode = 6;	/* eg _DF */
      break;
    case SFmode :
      mode = 7;	/* eg _SF */
      break;

    case V2DImode :	/* vector of  2 double word ints (2*uint64) for SSE */
      mode = scalar_equiv ? 6 : 8;	/* eg _DF or _VDI */
      break;
    case V4SImode :	/* vector of  4 single word ints (4*uint32) for SSE */
      mode = scalar_equiv ? 6 : 9;	/* eg _DF or _VSI */
      break;
    case V8HImode :	/* vector of  8 half   word ints (8*uint16) for SSE */
      mode = scalar_equiv ? 6 : 10;	/* eg _DF or _VHI */
      break;
    case V16QImode :	/* vector of 16 quarter word ints (16*uint8) for SSE */
      mode = scalar_equiv ? 6 : 11;	/* eg _DF or _VQI */
      break;
    case V2DFmode :	/* vector of 2 ieee64 double (2*double) for SSE */
      mode = scalar_equiv ? 6 : 12;	/* eg _DF or _VDF */
      break;
    case V4SFmode :	/* vector of 4 ieee32 float  (4*float) for SSE */
      mode = scalar_equiv ? 6 : 13;	/* eg _DF or _VSF */
      break;

    case BLKmode:	/* block of memory, as for example __builtin_memmove */
      /* TODO? where's the size information */
      mode = 0;		/* only use DI (uint64) grammar encoding */
      break;

    default :
      mode = 0;
      icg_nyi("get_operator_mode: expr_code=%d/%s expr_mode=%d/%s unhandled",
	expr_code, GET_RTX_NAME(expr_code),
	expr_mode, GET_MODE_NAME(expr_mode));
      /*NOTREACHED*/
      break;
  }
  return (icg_opcodes)mode;
}

struct unspec_code_def {
  int unspec_code;
  const char *unspec_name;
  unsigned icg_terminal;
  unsigned char is_type_encoded;
};

static
struct unspec_code_def *lookup_unspec_code(int unspec_code)
{
  static struct unspec_code_def *indexable = 0;
  static int max_unspec_code = 0;
  if (indexable == 0) {
    /*
     * This table is unsorted and dense.
     * We'll lazily copy data in this table
     * to a dynamically allocated directly indexable table
     * the first time we need the table.
     */
    static const struct unspec_code_def unspec_code_def [] = {
      {UNSPEC_GOT, "UNSPEC_GOT", ICG_UNSPEC_GOT, 0},
      {UNSPEC_GOTOFF, "UNSPEC_GOTOFF", ICG_UNSPEC_GOTOFF, 0},
      {UNSPEC_GOTPCREL, "UNSPEC_GOTPCREL", ICG_UNSPEC_GOTPCREL, 0},
      {UNSPEC_GOTTPOFF, "UNSPEC_GOTTPOFF", ICG_UNSPEC_GOTTPOFF, 0},
      {UNSPEC_TPOFF, "UNSPEC_TPOFF", ICG_UNSPEC_TPOFF, 0},
      {UNSPEC_NTPOFF, "UNSPEC_NTPOFF", ICG_UNSPEC_NTPOFF, 0},
      {UNSPEC_DTPOFF, "UNSPEC_DTPOFF", ICG_UNSPEC_DTPOFF, 0},
      {UNSPEC_GOTNTPOFF, "UNSPEC_GOTNTPOFF", ICG_UNSPEC_GOTNTPOFF, 0},
      {UNSPEC_INDNTPOFF, "UNSPEC_INDNTPOFF", ICG_UNSPEC_INDNTPOFF, 0},
      {UNSPEC_PLTOFF, "UNSPEC_PLTOFF", ICG_UNSPEC_PLTOFF, 0},
      {UNSPEC_MACHOPIC_OFFSET, "UNSPEC_MACHOPIC_OFFSET", ICG_UNSPEC_MACHOPIC_OFFSET, 0},
      {UNSPEC_STACK_ALLOC, "UNSPEC_STACK_ALLOC", ICG_UNSPEC_STACK_ALLOC, 0},
      {UNSPEC_SET_GOT, "UNSPEC_SET_GOT", ICG_UNSPEC_SET_GOT, 0},
      {UNSPEC_SSE_PROLOGUE_SAVE, "UNSPEC_SSE_PROLOGUE_SAVE", ICG_UNSPEC_SSE_PROLOGUE_SAVE, 0},
      {UNSPEC_REG_SAVE, "UNSPEC_REG_SAVE", ICG_UNSPEC_REG_SAVE, 0},
      {UNSPEC_DEF_CFA, "UNSPEC_DEF_CFA", ICG_UNSPEC_DEF_CFA, 0},
      {UNSPEC_SET_RIP, "UNSPEC_SET_RIP", ICG_UNSPEC_SET_RIP, 0},
      {UNSPEC_SET_GOT_OFFSET, "UNSPEC_SET_GOT_OFFSET", ICG_UNSPEC_SET_GOT_OFFSET, 0},
      {UNSPEC_TP, "UNSPEC_TP", ICG_UNSPEC_TP, 0},
      {UNSPEC_TLS_GD, "UNSPEC_TLS_GD", ICG_UNSPEC_TLS_GD, 0},
      {UNSPEC_TLS_LD_BASE, "UNSPEC_TLS_LD_BASE", ICG_UNSPEC_TLS_LD_BASE, 0},
      {UNSPEC_TLSDESC, "UNSPEC_TLSDESC", ICG_UNSPEC_TLSDESC, 0},
      {UNSPEC_SCAS, "UNSPEC_SCAS", ICG_UNSPEC_SCAS, 0},
      {UNSPEC_FNSTSW, "UNSPEC_FNSTSW", ICG_UNSPEC_FNSTSW, 0},
      {UNSPEC_SAHF, "UNSPEC_SAHF", ICG_UNSPEC_SAHF, 0},
      {UNSPEC_FSTCW, "UNSPEC_FSTCW", ICG_UNSPEC_FSTCW, 0},
      {UNSPEC_ADD_CARRY, "UNSPEC_ADD_CARRY", ICG_UNSPEC_ADD_CARRY_DI, 1},	/* icg type specific */
      {UNSPEC_FLDCW, "UNSPEC_FLDCW", ICG_UNSPEC_FLDCW, 0},
      {UNSPEC_REP, "UNSPEC_REP", ICG_UNSPEC_REP, 0},
      {UNSPEC_EH_RETURN, "UNSPEC_EH_RETURN", ICG_UNSPEC_EH_RETURN, 0},
      {UNSPEC_LD_MPIC, "UNSPEC_LD_MPIC", ICG_UNSPEC_LD_MPIC, 0},
      {UNSPEC_TRUNC_NOOP, "UNSPEC_TRUNC_NOOP", ICG_UNSPEC_TRUNC_NOOP, 0},
      {UNSPEC_FIX_NOTRUNC, "UNSPEC_FIX_NOTRUNC", ICG_UNSPEC_FIX_NOTRUNC, 0},
      {UNSPEC_PFRCPIT1, "UNSPEC_PFRCPIT1", ICG_UNSPEC_PFRCPIT1, 0},
      {UNSPEC_MASKMOV, "UNSPEC_MASKMOV", ICG_UNSPEC_MASKMOV, 0},
      {UNSPEC_PFRCPIT2, "UNSPEC_PFRCPIT2", ICG_UNSPEC_PFRCPIT2, 0},
      {UNSPEC_MOVMSK, "UNSPEC_MOVMSK", ICG_UNSPEC_MOVMSK, 0},
      {UNSPEC_PFRSQRT, "UNSPEC_PFRSQRT", ICG_UNSPEC_PFRSQRT, 0},
      {UNSPEC_MOVNT, "UNSPEC_MOVNT", ICG_UNSPEC_MOVNT, 0},
      {UNSPEC_PFRSQIT1, "UNSPEC_PFRSQIT1", ICG_UNSPEC_PFRSQIT1, 0},
      {UNSPEC_MFENCE, "UNSPEC_MFENCE", ICG_UNSPEC_MFENCE, 0},
      {UNSPEC_MOVU, "UNSPEC_MOVU", ICG_UNSPEC_MOVU, 0},
      {UNSPEC_LFENCE, "UNSPEC_LFENCE", ICG_UNSPEC_LFENCE, 0},
      {UNSPEC_RCP, "UNSPEC_RCP", ICG_UNSPEC_RCP, 0},
      {UNSPEC_PSADBW, "UNSPEC_PSADBW", ICG_UNSPEC_PSADBW, 0},
      {UNSPEC_RSQRT, "UNSPEC_RSQRT", ICG_UNSPEC_RSQRT, 0},
      {UNSPEC_LDDQU, "UNSPEC_LDDQU", ICG_UNSPEC_LDDQU, 0},
      {UNSPEC_SFENCE, "UNSPEC_SFENCE", ICG_UNSPEC_SFENCE, 0},
      {UNSPEC_PFRCP, "UNSPEC_PFRCP", ICG_UNSPEC_PFRCP, 0},
      {UNSPEC_COPYSIGN, "UNSPEC_COPYSIGN", ICG_UNSPEC_COPYSIGN, 0},
      {UNSPEC_IEEE_MIN, "UNSPEC_IEEE_MIN", ICG_UNSPEC_IEEE_MIN_DI, 1},	/* icg type specific */
      {UNSPEC_IEEE_MAX, "UNSPEC_IEEE_MAX", ICG_UNSPEC_IEEE_MAX_DI, 1},	/* icg type specific */
      {UNSPEC_SIN, "UNSPEC_SIN", ICG_UNSPEC_SIN, 0},
      {UNSPEC_COS, "UNSPEC_COS", ICG_UNSPEC_COS, 0},
      {UNSPEC_FPATAN, "UNSPEC_FPATAN", ICG_UNSPEC_FPATAN, 0},
      {UNSPEC_FYL2X, "UNSPEC_FYL2X", ICG_UNSPEC_FYL2X, 0},
      {UNSPEC_FYL2XP1, "UNSPEC_FYL2XP1", ICG_UNSPEC_FYL2XP1, 0},
      {UNSPEC_FRNDINT, "UNSPEC_FRNDINT", ICG_UNSPEC_FRNDINT, 0},
      {UNSPEC_FIST, "UNSPEC_FIST", ICG_UNSPEC_FIST, 0},
      {UNSPEC_F2XM1, "UNSPEC_F2XM1", ICG_UNSPEC_F2XM1, 0},
      {UNSPEC_TAN, "UNSPEC_TAN", ICG_UNSPEC_TAN, 0},
      {UNSPEC_FXAM, "UNSPEC_FXAM", ICG_UNSPEC_FXAM, 0},
      {UNSPEC_FRNDINT_FLOOR, "UNSPEC_FRNDINT_FLOOR", ICG_UNSPEC_FRNDINT_FLOOR, 0},
      {UNSPEC_FRNDINT_CEIL, "UNSPEC_FRNDINT_CEIL", ICG_UNSPEC_FRNDINT_CEIL, 0},
      {UNSPEC_FRNDINT_TRUNC, "UNSPEC_FRNDINT_TRUNC", ICG_UNSPEC_FRNDINT_TRUNC, 0},
      {UNSPEC_FRNDINT_MASK_PM, "UNSPEC_FRNDINT_MASK_PM", ICG_UNSPEC_FRNDINT_MASK_PM, 0},
      {UNSPEC_FIST_FLOOR, "UNSPEC_FIST_FLOOR", ICG_UNSPEC_FIST_FLOOR, 0},
      {UNSPEC_FIST_CEIL, "UNSPEC_FIST_CEIL", ICG_UNSPEC_FIST_CEIL, 0},
      {UNSPEC_SINCOS_COS, "UNSPEC_SINCOS_COS", ICG_UNSPEC_SINCOS_COS, 0},
      {UNSPEC_SINCOS_SIN, "UNSPEC_SINCOS_SIN", ICG_UNSPEC_SINCOS_SIN, 0},
      {UNSPEC_XTRACT_FRACT, "UNSPEC_XTRACT_FRACT", ICG_UNSPEC_XTRACT_FRACT, 0},
      {UNSPEC_XTRACT_EXP, "UNSPEC_XTRACT_EXP", ICG_UNSPEC_XTRACT_EXP, 0},
      {UNSPEC_FSCALE_FRACT, "UNSPEC_FSCALE_FRACT", ICG_UNSPEC_FSCALE_FRACT, 0},
      {UNSPEC_FSCALE_EXP, "UNSPEC_FSCALE_EXP", ICG_UNSPEC_FSCALE_EXP, 0},
      {UNSPEC_FPREM_F, "UNSPEC_FPREM_F", ICG_UNSPEC_FPREM_F, 0},
      {UNSPEC_FPREM_U, "UNSPEC_FPREM_U", ICG_UNSPEC_FPREM_U, 0},
      {UNSPEC_FPREM1_F, "UNSPEC_FPREM1_F", ICG_UNSPEC_FPREM1_F, 0},
      {UNSPEC_FPREM1_U, "UNSPEC_FPREM1_U", ICG_UNSPEC_FPREM1_U, 0},
      {UNSPEC_C2_FLAG, "UNSPEC_C2_FLAG", ICG_UNSPEC_C2_FLAG, 0},
      {UNSPEC_SP_SET, "UNSPEC_SP_SET", ICG_UNSPEC_SP_SET, 0},
      {UNSPEC_SP_TEST, "UNSPEC_SP_TEST", ICG_UNSPEC_SP_TEST, 0},
      {UNSPEC_SP_TLS_SET, "UNSPEC_SP_TLS_SET", ICG_UNSPEC_SP_TLS_SET, 0},
      {UNSPEC_SP_TLS_TEST, "UNSPEC_SP_TLS_TEST", ICG_UNSPEC_SP_TLS_TEST, 0},
      {UNSPEC_PSHUFB, "UNSPEC_PSHUFB", ICG_UNSPEC_PSHUFB, 0},
      {UNSPEC_PSIGN, "UNSPEC_PSIGN", ICG_UNSPEC_PSIGN, 0},
      {UNSPEC_PALIGNR, "UNSPEC_PALIGNR", ICG_UNSPEC_PALIGNR, 0},
      {UNSPEC_EXTRQI, "UNSPEC_EXTRQI", ICG_UNSPEC_EXTRQI, 0},
      {UNSPEC_EXTRQ, "UNSPEC_EXTRQ", ICG_UNSPEC_EXTRQ, 0},
      {UNSPEC_INSERTQI, "UNSPEC_INSERTQI", ICG_UNSPEC_INSERTQI, 0},
      {UNSPEC_INSERTQ, "UNSPEC_INSERTQ", ICG_UNSPEC_INSERTQ, 0},
      {UNSPEC_BLENDV, "UNSPEC_BLENDV", ICG_UNSPEC_BLENDV, 0},
      {UNSPEC_INSERTPS, "UNSPEC_INSERTPS", ICG_UNSPEC_INSERTPS, 0},
      {UNSPEC_DP, "UNSPEC_DP", ICG_UNSPEC_DP, 0},
      {UNSPEC_MOVNTDQA, "UNSPEC_MOVNTDQA", ICG_UNSPEC_MOVNTDQA, 0},
      {UNSPEC_MPSADBW, "UNSPEC_MPSADBW", ICG_UNSPEC_MPSADBW, 0},
      {UNSPEC_PHMINPOSUW, "UNSPEC_PHMINPOSUW", ICG_UNSPEC_PHMINPOSUW, 0},
      {UNSPEC_PTEST, "UNSPEC_PTEST", ICG_UNSPEC_PTEST, 0},
      {UNSPEC_ROUND, "UNSPEC_ROUND", ICG_UNSPEC_ROUND, 0},
      {UNSPEC_CRC32, "UNSPEC_CRC32", ICG_UNSPEC_CRC32, 0},
      {UNSPEC_PCMPESTR, "UNSPEC_PCMPESTR", ICG_UNSPEC_PCMPESTR, 0},
      {UNSPEC_PCMPISTR, "UNSPEC_PCMPISTR", ICG_UNSPEC_PCMPISTR, 0},
      {UNSPEC_SSE5_INTRINSIC, "UNSPEC_SSE5_INTRINSIC", ICG_UNSPEC_SSE5_INTRINSIC, 0},
      {UNSPEC_SSE5_UNSIGNED_CMP, "UNSPEC_SSE5_UNSIGNED_CMP", ICG_UNSPEC_SSE5_UNSIGNED_CMP, 0},
      {UNSPEC_SSE5_TRUEFALSE, "UNSPEC_SSE5_TRUEFALSE", ICG_UNSPEC_SSE5_TRUEFALSE, 0},
      {UNSPEC_SSE5_PERMUTE, "UNSPEC_SSE5_PERMUTE", ICG_UNSPEC_SSE5_PERMUTE, 0},
      {UNSPEC_FRCZ, "UNSPEC_FRCZ", ICG_UNSPEC_FRCZ, 0},
      {UNSPEC_CVTPH2PS, "UNSPEC_CVTPH2PS", ICG_UNSPEC_CVTPH2PS, 0},
      {UNSPEC_CVTPS2PH, "UNSPEC_CVTPS2PH", ICG_UNSPEC_CVTPS2PH, 0},
      {UNSPEC_AESENC, "UNSPEC_AESENC", ICG_UNSPEC_AESENC, 0},
      {UNSPEC_AESENCLAST, "UNSPEC_AESENCLAST", ICG_UNSPEC_AESENCLAST, 0},
      {UNSPEC_AESDEC, "UNSPEC_AESDEC", ICG_UNSPEC_AESDEC, 0},
      {UNSPEC_AESDECLAST, "UNSPEC_AESDECLAST", ICG_UNSPEC_AESDECLAST, 0},
      {UNSPEC_AESIMC, "UNSPEC_AESIMC", ICG_UNSPEC_AESIMC, 0},
      {UNSPEC_AESKEYGENASSIST, "UNSPEC_AESKEYGENASSIST", ICG_UNSPEC_AESKEYGENASSIST, 0},
      {UNSPEC_PCLMUL, "UNSPEC_PCLMUL", ICG_UNSPEC_PCLMUL, 0},
      {UNSPEC_PCMP, "UNSPEC_PCMP", ICG_UNSPEC_PCMP, 0},
      {UNSPEC_VPERMIL, "UNSPEC_VPERMIL", ICG_UNSPEC_VPERMIL, 0},
      {UNSPEC_VPERMIL2, "UNSPEC_VPERMIL2", ICG_UNSPEC_VPERMIL2, 0},
      {UNSPEC_VPERMIL2F128, "UNSPEC_VPERMIL2F128", ICG_UNSPEC_VPERMIL2F128, 0},
      {UNSPEC_MASKLOAD, "UNSPEC_MASKLOAD", ICG_UNSPEC_MASKLOAD, 0},
      {UNSPEC_MASKSTORE, "UNSPEC_MASKSTORE", ICG_UNSPEC_MASKSTORE, 0},
      {UNSPEC_CAST, "UNSPEC_CAST", ICG_UNSPEC_CAST, 0},
      {UNSPEC_VTESTP, "UNSPEC_VTESTP", ICG_UNSPEC_VTESTP, 0},
    };
    size_t i;
    for (i = 0; i < sizeof(unspec_code_def)/sizeof(unspec_code_def[0]); i++) {
      if (unspec_code_def[i].unspec_code > max_unspec_code) {
	max_unspec_code = unspec_code_def[i].unspec_code;
      }
    }
    indexable = XNEWVEC(struct unspec_code_def, (max_unspec_code + 1));
    for (i = 0; i < sizeof(unspec_code_def)/sizeof(unspec_code_def[0]); i++) {
      indexable[unspec_code_def[i].unspec_code] = unspec_code_def[i];
    }
  }
  gcc_assert(0 <= unspec_code);
  gcc_assert(unspec_code <= max_unspec_code);
  return &indexable[unspec_code];
}

/*
 * This finds the icg_opcode based on a
 * combination of machine mode and rtx code.
 * See icg-opcode.h (in the build directory)
 * for the list of icg_opcodes.
 */
static
icg_opcodes get_operator(rtx rtx_expr, rtx curr_insn)
{
  const enum rtx_code expr_code = GET_CODE(rtx_expr);
  /*
   * The variable op will start off in the range 0..9.
   * For example, DI mode will have op==0; SI mode will have op==1, etc.
   */
  icg_opcodes op = get_operator_mode(rtx_expr);

  /*
   * TODO: Translation of floating point compares requires a bit of context
   * in order to use the cmpsd instruction (double < double) ==> uint64 in xmm
   */
  const enum machine_mode expr_mode = GET_MODE(rtx_expr);
  const enum machine_mode insn_mode = GET_MODE(curr_insn);

  /*
   * perhaps there is a macro in gcc rtl that already determines
   * if a mode is floating point.
   * This has been disabled, as determining whether or not to use FLT_BASE
   * is context sensitive.
   */
  const bool d2 = 0 && ((expr_mode == SFmode && insn_mode == SFmode) ||
    (expr_mode == DFmode && insn_mode == DFmode)
    )
  ;

  switch (expr_code)
  {
    case PARALLEL :
      op = PARALLEL_ALL;
      break;

    case SET :
      op = SET_ALL;
      break;

    case USE :
      op = USE_ALL;
      break;

    case CALL :
      op = CALL_ALL;
      break;

    case CONST:
      op += CONST_DI;
      break;

    case CONST_DOUBLE:
      if (expr_mode == DFmode) {
	op = CONSTIEEE64;
      } else {
	op = CONSTIEEE32;
      }
      break;

    case CONST_INT :
    {
      /*
       * Encode special values into specific terminal symbols.
       */
      const long long number = INTVAL(rtx_expr);
      op = icg_op_of_constant(number);
      break;
    }

    case REG :
    {
      /*
       * Set REGX or REG depending on whether the rtx expression
       * is dead or not
       */
      switch (expr_mode) {
      case CCmode:
      case CCGCmode:
      case CCGOCmode:
      case CCNOmode:
      case CCAmode:
      case CCCmode:
      case CCOmode:
      case CCSmode:
      case CCZmode:
	if (reg_dead_p(rtx_expr, curr_insn)) {
	  op += REGCCX_DI;
	} else {
	  op += REGCC_DI;
	}
        break;
      case CCFPmode:
      case CCFPUmode:
	if (reg_dead_p(rtx_expr, curr_insn)) {
	  op += REGCCFPUX_DI;
	} else {
	  op += REGCCFPU_DI;
	}
        break;
      default:
	if (reg_dead_p(rtx_expr, curr_insn)) {
	  op += REGX_DI;
	} else {
	  op += REG_DI;
	}
	break;
      }
      break;
    }

    case PREFETCH:
      op += PREFETCH_DI;
      break;

    case MEM :
      if (expr_mode == BLKmode) {
        op += MEMB_DI;
      } else {
	op += MEM_DI;
      }
      break;

    case ZERO_EXTEND :
      op += ZERO_EXTEND_DI;
      break;
    case SIGN_EXTEND :
      op += SIGN_EXTEND_DI;
      break;
    case SUBREG :
      op += SUBREG_DI;
      break;
    case PLUS :
      op += PLUS_DI;
      break;
    case MINUS :
      op += MINUS_DI;
      break;
    case NEG :
      op += NEG_DI;
      break;
    case MULT :
      op += MULT_DI;
      break;
    case DIV :
      op += DIV_DI;
      break;
    case UDIV :
      op += UDIV_DI;
      break;
    case MOD :
      op += MOD_DI;
      break;
    case UMOD :
      op += UMOD_DI;
      break;
    case NOT :
      op += NOT_DI;
      break;
    case AND :
      op += AND_DI;
      break;
    case IOR :
      op += IOR_DI;
      break;
    case XOR :
      op += XOR_DI;
      break;
    case ASHIFT :	/* arithmetic left shift */
      op += ASHIFT_DI;
      break;
    case LSHIFTRT :	/* logical right shift */
      op += LSHIFTRT_DI;
      break;
    case ASHIFTRT :	/* arithmetic right shift */
      op += ASHIFTRT_DI;
      break;
    case SMIN :
      op += SMIN_DI;
      break;
    case SMAX :
      op += SMAX_DI;
      break;
    case UMIN :
      op += UMIN_DI;
      break;
    case UMAX :
      op += UMAX_DI;
      break;
    case ROTATE :
      op += ROTATE_DI;
      break;
    case ROTATERT :
      op += ROTATERT_DI;
      break;
    case CLZ :
      op += CLZ_DI;
      break;
    case POPCOUNT :
      op += POPCOUNT_DI;
      break;

    case FIX :
      op += FIX_DI;
      break;
    case TRUNCATE:
      op += TRUNCATE_DI;
      break;
    case FLOAT :
      op += FLOAT_BASE;		/* will get smashed to FLOAT_SF or FLOAT_DF */
      break;
    case FLOAT_TRUNCATE:	/* converting double to float */
      op += FLOAT_TRUNCATE_BASE;
      break;
    case FLOAT_EXTEND:		/* converting float to double */
      op += FLOAT_EXTEND_BASE;
      break;
    case IF_THEN_ELSE :
      /* Specially handled by looking at its 3 kids */
      break;

    case SYMBOL_REF :
      /* From rtl.def:
	 Reference to a named label:
	 Operand 0: label name
	 Operand 1: flags (see SYMBOL_FLAG_* in rtl.h)
	 Operand 2: tree from which this symbol is derived, or null.
	 This is either a DECL node, or some kind of constant.  */
      if (flag_pic) {
	op += SYMBOL_REF_64;
      } else {
	op += SYMBOL_REF_32;
      }
      break;

    case LABEL_REF :
      /* From rtl.def:
         Reference to an assembler label in the code for this function.
	 The operand is a CODE_LABEL found in the insn chain.  */
      op += LABEL_REF_DI;
      break;

    /*
     * scalar and floating point ordered comparisions, yielding condition code
     * floating point ordered comparision, yielding xmm register (cmpsd)
     * This was scaffolded in around 10Oct2008, and still needed work then.
     */
    case EQ :
      if (d2) {
	op += FEQ_BASE;
      } else {
	op = EQ_ALL;
      }
      break;
    case NE :
      if (d2) {
	op += FNEQ_BASE;
      } else {
	op = NE_ALL;
      }
      break;

    case GT :
      if (d2) {
        op += FGT_BASE;	/* will be done as FLE with swapped operands */
      } else {
	const rtx rtx_lchild = XEXP(rtx_expr, 0);
	const enum machine_mode lmode = GET_MODE(rtx_lchild);
	const bool is_ccfpu = (GET_CODE(rtx_lchild) == REG && lmode == CCFPUmode);
	if (is_ccfpu) {
	  op = UNGT_ALL;
	} else {
	  op = GT_ALL;
	}
      }
      break;
    case GTU :		/* > unsigned */
      op = GTU_ALL;
      break;

    case LT :
      if (d2) {
        op += FLT_BASE;
      } else {
	const rtx rtx_lchild = XEXP(rtx_expr, 0);
	const enum machine_mode lmode = GET_MODE(rtx_lchild);
	const bool is_ccfpu = (GET_CODE(rtx_lchild) == REG && lmode == CCFPUmode);
	if (is_ccfpu) {
	  op = UNLT_ALL;
	} else {
	  op = LT_ALL;
	}
	if (expr_mode == CCGOCmode) {
	  op = LTS_ALL; /* TODO: incomplete: added to support uint64->double conversion */
	}
      }
      break;
    case LTU :		/* < unsigned */
      op = LTU_ALL;
      break;

    case GE :
      if (d2) {
	op += FGE_BASE;	/* will be done as FLT with swapped operands */
      } else {
	const rtx rtx_lchild = XEXP(rtx_expr, 0);
	const enum machine_mode lmode = GET_MODE(rtx_lchild);
	const bool is_ccfpu = (GET_CODE(rtx_lchild) == REG && lmode == CCFPUmode);
	if (is_ccfpu) {
	  op = UNGE_ALL;	/* do as an unordered floating compare */
	} else {
	  op = GE_ALL;	/* TODO: in some cases, should be done as a unordered floating compare */
	}
      }
      break;
    case GEU :		/* >= unsigned */
      op = GEU_ALL;
      break;

    case LE :
      if (d2) {
	op += FLE_BASE;
      } else {
	const rtx rtx_lchild = XEXP(rtx_expr, 0);
	const enum machine_mode lmode = GET_MODE(rtx_lchild);
	const bool is_ccfpu = (GET_CODE(rtx_lchild) == REG && lmode == CCFPUmode);
	if (is_ccfpu) {
	  op = UNLE_ALL;
	} else {
	  op = LE_ALL;
	}
      }
      break;
    case LEU :		/* <= unsigned */
      op = LEU_ALL;
      break;

    /*
     * floating point unordered comparisions, yielding condition code
     */
    case UNEQ:		/* == unordered */
      op = UNEQ_ALL;
      break;
    case UNGE:		/* >= unordered */
      op = UNGE_ALL;
      break;
    case UNGT:		/* >  unordered */
      op = UNGT_ALL;
      break;
    case UNLE:		/* <= unordered */
      op = UNLE_ALL;
      break;
    case UNLT:		/* <  unordered */
      op = UNLT_ALL;
      break;
    case LTGT:		/* <> unordered (aka? != unordered) */
      op = LTGT_ALL;
      break;
    case UNORDERED:
      op = UNORDERED_ALL;
      break;
    case ORDERED:
      op = ORDERED_ALL;
      break;

    case PC :
      op = PC_ALL;
      break;

    case COMPARE :
      op = COMPARE_CC;
      break;

    case UNSPEC:
      {
	const int unspec_code = XINT(rtx_expr, 1);
	const struct unspec_code_def *def = lookup_unspec_code(unspec_code);
	switch (unspec_code) {
	case UNSPEC_GOTPCREL:
	  op += flag_pic ? SYMBOL_REF_64 : SYMBOL_REF_32;
	  gcc_assert (XVECLEN (rtx_expr, 0) == 1);
	  gcc_assert (GET_CODE (XVECEXP (rtx_expr, 0, 0)) == SYMBOL_REF);
	  break;
	default:
	  icg_nyi("unhandled unspec_code=%d/%s",
	    unspec_code, def->unspec_name);
	  break;
	}
      }
      break;

    case ASM_OPERANDS:	/* should be rewritten and never seen by now */
    case CONST_VECTOR:
    case CONST_FIXED:
    default :
      if (dump_file) {
        fprintf(dump_file, "\n");
	fprintf(dump_file, "Build-Tree Phase Error: In RTL instruction:\n");
        print_inline_rtx(dump_file, curr_insn, 2);
        fprintf(dump_file, "\n");
	fprintf(dump_file,
	  "%s:%d: RTX Opcode: %s not matched in ICG grammar ... Quitting\n",
	  __FILE__, __LINE__,
	  GET_RTX_NAME(expr_code));
	fflush(dump_file);
      }
      icg_nyi("get_operator: expr_code=%d/%s unhandled",
	expr_code, GET_RTX_NAME(expr_code));
      /*NOTREACHED*/
      break;
  }
  return op;
}

static
bool set_unused_p (rtx set_expr, rtx curr_insn)
{
  rtx notes_rtx;
  rtx reg_expr = XEXP(set_expr, 0);
  if (GET_CODE(reg_expr)!=REG) {
    return false;
  }
  for (notes_rtx=XEXP(curr_insn, 7); notes_rtx; notes_rtx=XEXP(notes_rtx, 1)) {
    gcc_assert(GET_CODE(notes_rtx)==EXPR_LIST);
    {
      rtx reg_rtx = XEXP(notes_rtx, 0);
      if (REG_NOTE_KIND(notes_rtx)==REG_UNUSED && REGNO(reg_rtx)==REGNO(reg_expr)) {
	return true;
      }
    }
  }
  return false;
}

/* Allocate a new icg_node.  If expr is not NULL, then various fields
   are filled in based on the RTL expression. */
static
icg_node * icg_node_alloc (icg_opcodes op, rtx insn, rtx expr)
{
  icg_node *p;
  p = (icg_node *)icg_calloc(sizeof(icg_node));
  p->op = op;
  p->insn = insn;
  if (expr != NULL) {
    const enum rtx_code expr_code = GET_CODE(expr);
    p->rtl = expr;
    p->mode = GET_MODE (expr);	/* TODO: deprecated; only used in one place in icg.c */
    /* Set specific fields for specific node types. */
    switch (expr_code) {
    case REG:
      p->r = REGNO (expr);
      p->rx = REGNO (expr);
      break;

    case CONST_INT:
      p->val = INTVAL (expr);
      p->a.string = NULL;
      break;

    case CONST_DOUBLE:
      {
	/*
	 * This code is likely to be extremely fragile,
	 * and not survive rehosting to a host machine other than x86_64,
	 * or to a host machine with a different endianness than x86_64.
	 */
	const enum machine_mode expr_mode = GET_MODE(expr);
	if (GET_MODE (expr) == DFmode) {
	  long vals[4];
	  typedef unsigned long long uint64;
	  REAL_VALUE_TYPE rv = *CONST_DOUBLE_REAL_VALUE(expr);
	  REAL_VALUE_TO_TARGET_DOUBLE(rv, vals);
	  if (0) {
	    fprintf(stderr, "v[0]=%ld = 0x%016lx\n", vals[0], vals[0]);
	    fprintf(stderr, "v[1]=%ld = 0x%016lx\n", vals[1], vals[1]);
	    fprintf(stderr, "v[2]=%ld = 0x%016lx\n", vals[2], vals[2]);
	    fprintf(stderr, "v[3]=%ld = 0x%016lx\n", vals[3], vals[3]);
	  }
	  p->val = 0ULL
	    | (((uint64)(vals[1]) & 0xffffffffULL) << 32)
	    | (((uint64)(vals[0]) & 0xffffffffULL) << 00)
	  ;
	} else if (GET_MODE (expr) == SFmode) {
	  long val;
	  typedef unsigned long long uint64;
	  REAL_VALUE_TYPE rv = *CONST_DOUBLE_REAL_VALUE(expr);
	  REAL_VALUE_TO_TARGET_SINGLE(rv, val);
	  p->val = val;
	} else {
	  icg_nyi("get_operator: expr_code=%d/%s mode=%d/%s unhandled",
	    expr_code, GET_RTX_NAME(expr_code),
	    expr_mode, GET_MODE_NAME(expr_mode));
	  /*NOTREACHED*/
	}
	p->a.string = NULL;
      }
      break;

    case CONST_VECTOR:
    case CONST_FIXED:
      icg_nyi("get_operator: expr_code=%d/%s",
	expr_code, GET_RTX_NAME(expr_code));
      /*NOTREACHED*/
      break;

    case SYMBOL_REF:
      /* From rtl.def:
	 Reference to a named label:
	 Operand 0: label name
	 Operand 1: flags (see SYMBOL_FLAG_* in rtl.h)
	 Operand 2: tree from which this symbol is derived, or null.
	 This is either a DECL node, or some kind of constant.  */
      if (flag_pic) {
	op += SYMBOL_REF_64;
      } else {
	op += SYMBOL_REF_32;
      }
      p->a.string = XSTR (expr, 0);
      p->val = 0;
      break;

    case LABEL_REF:
      /* From rtl.def:
         Reference to an assembler label in the code for this function.
	 The operand is a CODE_LABEL found in the insn chain.  */
      op += LABEL_REF_DI;
      p->a.string = "???LABEL_REF???";
      p->a.string = 0;
      p->val = CODE_LABEL_NUMBER (XEXP (expr, 0));
      break;

    default:
      break;
    }
  }
  return p;
}

/*
 * Return 1 if ICG can handle this asm somehow.
 */
int icg_analyze_asm(rtx expr, icg_analyze_asm_key how)
{
/* This comment about ASM_OPERANDS accessor functions is lifted from rtl.def
   An assembler instruction with operands.
   1st operand is the instruction template.
   2nd operand is the constraint for the output.
   3rd operand is the number of the output this expression refers to.
     When an insn stores more than one value, a separate ASM_OPERANDS
     is made for each output; this integer distinguishes them.
   4th is a vector of values of input operands.
   5th is a vector of modes and constraints for the input operands.
     Each element is an ASM_INPUT containing a constraint string
     and whose mode indicates the mode of the input operand.
   6th is the source line number.  */
#if 0
  0   ASM_OPERANDS_TEMPLATE
  1   ASM_OPERANDS_OUTPUT_CONSTRAINT
  2   ASM_OPERANDS_OUTPUT_IDX
  3   ASM_OPERANDS_INPUT_VEC
  4   ASM_OPERANDS_INPUT_CONSTRAINT_VEC
  3,N ASM_OPERANDS_INPUT
  3   ASM_OPERANDS_INPUT_LENGTH
  4,N ASM_OPERANDS_INPUT_CONSTRAINT_EXP
  4,N ASM_OPERANDS_INPUT_CONSTRAINT
  4,N ASM_OPERANDS_INPUT_MODE
  5, ASM_OPERANDS_SOURCE_LOCATION
  1, ASM_INPUT_SOURCE_LOCATION
#endif
  const enum rtx_code code = GET_CODE(expr);
  gcc_assert (code == ASM_OPERANDS);
  {
    const char *template             = ASM_OPERANDS_TEMPLATE(expr);
    const char *output_constraint    = ASM_OPERANDS_OUTPUT_CONSTRAINT(expr);
    const int output_idx             = ASM_OPERANDS_OUTPUT_IDX(expr);
    const rtvec input_vec            = ASM_OPERANDS_INPUT_VEC(expr);
    const rtvec input_constraint_vec = ASM_OPERANDS_INPUT_CONSTRAINT_VEC(expr);
    const int ninputs                = ASM_OPERANDS_INPUT_LENGTH(expr);
    const int line_source  = ASM_OPERANDS_SOURCE_LOCATION(expr);
    const int line_input   = ASM_INPUT_SOURCE_LOCATION(expr);
    rtx inputs[20];
    const char *input_constraint[20];
    int i;
    const char *cp;
    unsigned ok = 1;

    (void)line_input;
    (void)line_source;
    (void)input_constraint_vec;
    (void)input_vec;

    memset(inputs, 0, sizeof(inputs));
    memset(input_constraint, 0, sizeof(input_constraint));
    for (i = 0; i < ninputs; i++) {
      inputs[i]           = ASM_OPERANDS_INPUT(expr, i);
      input_constraint[i] = ASM_OPERANDS_INPUT_CONSTRAINT(expr, i);
    }

    if (output_idx != 0) {
      return 0;	/* do not accept */
    }

    if (output_constraint == 0 || output_constraint[0] == 0) {
      if (ninputs == 0) {
	/* example: __asm__ __volatile___("mfence" : : : "memory"); */
	if (how == ICG_ASM_PHASE_DEBUG) {
	  fprintf(dump_file, "\t%s\t; %s\n", template, "crock: ICG_ASM_NULLARY");
	}
	return 1;	/* accept */
      }
      return 0;	/* do not accept */
    }

    /*
     * We expect there to be one output and it should be modifiable
     */
    if (*output_constraint != '=') {
      return 0;
    }

    if (0) {
      /*
       * look for multiple register source operands, single register destination operand
       * Example: 
       *   __asm__("bsr   %1, %0\n\t" "cmovz %2, %0" : "=&r" (bits) : "ro" (v), "r" (neg1) : "cc");
       *
       * We'll treat this as a 2 register src operand , 1 register dst operand instruction
       *   even though the "ro" says the 1st source operand can come from "o"ffsetable memory.
       */
      ok = 1;
      for (cp = output_constraint; *cp; cp++) {
	switch (*cp) {
	case '=':
	case '&':
	case 'r':
	  ok &= 1;
	  break;
	default:
	  ok = 0;
	  break;
	}
      }
      for (i = 0; i < ninputs; i++) {
	unsigned input_ok = 0;
	for (cp = input_constraint[i]; *cp; cp++) {
	  switch (*cp) {
	  case 'r':
	    input_ok = 1;
	    break;
	  default:
	    break;
	  }
	}
	ok &= input_ok;
      }

      if (ok) {
	return 2;	/* accept, using generic binary operator ASM_BINARY_RRR */
      }
    }

  }

  return 0;	/* do not accept */
}

/*
 * Return the number of subexpressions in X.
 * We're peeking inside the structure of rtx (ugh),
 * but there doesn't seem to be a better way.
 */
static
int count_subexpressions (rtx x)
{
  /* Count the number of 'e's in the format string.  */
  const char *format_str = GET_RTX_FORMAT (GET_CODE (x));
  int i, n = 0;
  for (i = 0; format_str[i]; i++) {
    if (format_str[i] == 'e') {
      /*
       * If X has any 'e's, they should be all at the beginning of the
       * string (I think).
       */
      gcc_assert (n == i);
      n++;
    }
  }
  return n;
}

/*
 * Translate rtl subexpression EXPR of INSN and return a newly
 * constructed icg node.
 * This is where the bulk of the work gets done.
 */
static
icg_node *translate_expression (rtx expr, rtx insn)
{
  icg_node *inode = NULL;
  enum rtx_code expr_code;

  gcc_assert (expr != NULL_RTX);
  expr_code = GET_CODE (expr);

  /* Handle special cases explicitly (PARALLEL, CALL, etc) where there
     is not a one-to-one mapping between rtx nodes and icg tree
     nodes.  */

  if (expr_code == SET
      && (GET_CODE (SET_DEST (expr)) == PARALLEL)) {
    /* SET (PARALLEL(expr_list (a, X), expr_list(b, Y)...) , CALL (...)
         => SET (LIST_ALL (DEF_ALL (a), LIST_ALL ...))
                 CALL ( ... )

       This structure appears if the function call returns a
       structure.  In this case the destination of the SET is a
       PARALLEL of expr_list.  Each expr_list has "mode"
       REG_DEP_TRUE. */
    int i;
    icg_node **tail;
    rtx par_expr = SET_DEST (expr);
    inode = icg_node_alloc (SET_ALL, insn, expr);
    inode->right = translate_expression (SET_SRC (expr), insn);
    tail = &(inode->left);
    for (i = 0; i < XVECLEN (par_expr, 0); i ++) {
      rtx vexpr = XVECEXP (par_expr, 0, i);
      /* Verify that element is an expr_list of mode REG_DEP_TRUE. */
      gcc_assert (GET_CODE (vexpr) == EXPR_LIST);
      gcc_assert (GET_MODE (vexpr) == REG_DEP_TRUE);
      gcc_assert (REG_P (XEXP (vexpr, 0)));
      *tail = icg_node_alloc (LIST_ALL, insn, NULL);
      (*tail)->left = icg_node_alloc (DEF_ALL, insn, NULL);
      (*tail)->left->left = translate_expression (XEXP (vexpr, 0), insn);
      tail = &((*tail)->right);
    }
    /* Verify that at least one element is in the LIST_ALL. */
    gcc_assert (tail != &(inode->left));
    /* Terminate list. */
    *tail = icg_node_alloc (END_OF_LIST, insn, NULL);

  } else if (expr_code == PARALLEL) {
    /*
     * PARALLEL(a,b,c,..) => PARALLEL_ALL (a, PARALLEL_ALL(b, ... ))
     * Only the used elements of PARALLEL are translated.
     * If the number of used elements of PARALLEL is one,
     * then just return the single element,
     * and no PARALLEL_ALL node is constructed.
     */
    int i = 0;
    inode = NULL;
    /*
     * Tree is constructed bottom up, so iterate through vector in
     * reverse.  At any point, inode is the top of the partially
     * constructed tree.
     */
    for (i = XVECLEN (expr, 0) - 1; i >= 0; i--) {
      icg_node *parent = NULL;
      rtx vexp = XVECEXP (expr, 0, i);
      if (GET_CODE (vexp) == CLOBBER) {
	/* Ignore clobbers. */
#ifdef OLD_STUFF
      if (GET_CODE (vexp) == CLOBBER)
	  || (GET_CODE (vexp) == SET
	      && set_unused_p (vexp, insn))) {
	/* Ignore clobbers and unused sets. */
#endif
	continue;
      }
      if (inode != NULL) {
	/*
	 * Allocate parent and hang previous icg node on it.
	 */
	parent = icg_node_alloc (PARALLEL_ALL, insn, NULL);
	parent->right = inode;
      }
      inode = translate_expression (vexp, insn);
      if (parent) {
	/*
	 * Hang inode on existing parent (filling it),
	 * and make it the new parent.
	 */
	parent->left = inode;
	inode = parent;
	parent = NULL;
      }
    }
    /*
     * A PARALLEL rtx must have at least one used operand.
     */
    gcc_assert (inode);

  } else if (expr_code == IF_THEN_ELSE) {
    /* IF_THEN_ELSE (a, b, c) => COND_MOVE (a, PAIR_ALL (b, c)) */
    inode = icg_node_alloc (COND_MOVE, insn, expr);
    inode->left = translate_expression (XEXP (expr, 0), insn);
    inode->right = icg_node_alloc (PAIR_ALL, insn, NULL);
    inode->right->left = translate_expression (XEXP (expr, 1), insn);
    inode->right->right = translate_expression (XEXP (expr, 2), insn);

  } else if (expr_code == CALL) {
    /* CALL (f, o) EXPR_LIST (a, b, ...)
       =>  CALL_ALL (PAIR_ALL (f, o), LIST_ALL (USE_ALL (a), LIST_ALL ...))) */
    icg_node *tail;
    rtx link;

    current_function_is_leaf = 0;

    inode = icg_node_alloc (CALL_ALL, insn, expr);
    inode->left = icg_node_alloc (PAIR_ALL, insn, NULL);
    inode->left->left = translate_expression (XEXP (expr, 0), insn);
    inode->left->right = translate_expression (XEXP (expr, 1), insn);

    /* Call parameters are stored in an expression list accessed via
       CALL_INSN_FUNCTION_USAGE.  This macro takes an insn, not an
       expression which will cause problems if for whatever reason
       (forward prop?) the CALL rtx is not the top level rtx (pattern)
       or SET_SRC of the insn, so verify this before continuing
       (although this really shouldn't happen). */
    gcc_assert (PATTERN (insn) == expr
		|| (GET_CODE (PATTERN (insn)) == SET
		    && SET_SRC (PATTERN (insn)) == expr));
    tail = inode;
    for (link = CALL_INSN_FUNCTION_USAGE (insn); link; link = XEXP (link, 1)) {
      /* Extend list. */
      tail->right = icg_node_alloc (LIST_ALL, insn, NULL);
      tail = tail->right;
      /* Add element. */
      tail->left = translate_expression (XEXP (link, 0), insn);
    }
    /* Terminate list. */
    tail->right = icg_node_alloc (END_OF_LIST, insn, NULL);

  } else if (expr_code == ASM_OPERANDS) {

    const char *instr_template = ASM_OPERANDS_TEMPLATE(expr);
    if (0 && strcmp(instr_template, "bsfq %1, %0") == 0) {
      icg_node *inode = icg_node_alloc(BSF_DI, insn, expr);
      const rtvec rtvec   = ASM_OPERANDS_INPUT_VEC(expr);
      const int nelements = ASM_OPERANDS_INPUT_LENGTH(expr);
      gcc_assert(nelements == 1);
      inode->left = translate_expression(RTVEC_ELT(rtvec, 0), insn);
      inode->right = 0;
    } else {
      const int accept_code = icg_analyze_asm(expr, ICG_ASM_PHASE_BUILD);
      if (accept_code) {
	if (accept_code == 2) {
	  const int nelements = ASM_OPERANDS_INPUT_LENGTH(expr);
	  icg_opcodes op;
	  gcc_assert(nelements == 2);
	  op = ASM_BINARY_RRR_DI;		/* base operator */
	  op += get_operator_mode(expr);	/* fold in type info */
	  inode = icg_node_alloc(op, insn, expr);
	  inode->left  = translate_expression(ASM_OPERANDS_INPUT(expr, 0), insn);
	  inode->right = translate_expression(ASM_OPERANDS_INPUT(expr, 1), insn);
	} else {
	  inode = icg_node_alloc(ASM_NULLARY, insn, expr);
	}
      } else {
	icg_nyi("asm template %s unhandled", instr_template);
	/*NOTREACHED*/
	inode = 0;
      }
    }

  } else if ((expr_code == UNSPEC) || (expr_code == UNSPEC_VOLATILE)) {
    const int unspec_code = XINT(expr, 1);
    if (unspec_code == UNSPEC_GOTPCREL) {
      rtx sym = 0;
      gcc_assert (XVECLEN (expr, 0) == 1);
      sym = XVECEXP (expr, 0, 0);
      gcc_assert (GET_CODE (sym) == SYMBOL_REF);
      inode = icg_node_alloc (get_operator (sym, insn), insn, sym);
      if (0) {
	inode->a.string = XSTR (expr, 0);
	inode->val = 0;
      }
    } else {
      /*
       * Unravel the k kids of the UNSPEC node into a right recursive binary tree
       * with k-1 nodes in the icg tree.
       */
      const struct unspec_code_def *def = lookup_unspec_code(unspec_code);
      const rtvec rtvec = XVEC(expr, 0);
      const int num_elem = GET_NUM_ELEM(rtvec);
      int i;
      gcc_assert (num_elem >= 1);
      inode = translate_expression(RTVEC_ELT(rtvec, num_elem-1), insn);
      for (i = num_elem - 2; i >= 0; i--) {
	icg_opcodes op = def->icg_terminal;
	if (def->is_type_encoded) {
	  op += get_operator_mode(expr);
	}
	{
	  icg_node *interior = icg_node_alloc(op, insn, expr);
	  interior->left = translate_expression(RTVEC_ELT(rtvec, i), insn);
	  interior->right = inode;
	  inode = interior;
	}
      }
    }

  } else if (expr_code == PREFETCH) {
    /* From rtl.def:
     * Memory prefetch, with attributes supported on some targets.
     * Operand 1 is the address of the memory to fetch.
     * Operand 2 is 1 for a write access, 0 otherwise.
     * Operand 3 is the level of temporal locality; 0 means there is no
     * temporal locality and 1, 2, and 3 are for increasing levels of temporal
     * locality.
     * The attributes specified by operands 2 and 3 are ignored for targets
     * whose prefetch instructions do not support them.
     *
     * Our strategy is to encode kids 1 and 2 into a single integer
     * and treat prefetch as a binary operator.
     */
    const int k1 = XINT(expr, 1);
    const int k2 = XINT(expr, 2);
    inode = icg_node_alloc (get_operator (expr, insn), insn, expr);
    inode->left = translate_expression(XEXP (expr, 0), insn);
    inode->right = icg_node_alloc (icg_op_of_constant(k1*10+k2), insn, NULL);
    return inode;

  } else {
    /* "Normal" expression. */
    const int nsubs = count_subexpressions (expr);
    gcc_assert (nsubs <= 2);
    inode = icg_node_alloc (get_operator (expr, insn), insn, expr);
    /*
     * These ops should be "normal" binary, unary, or nul-ary
     * expressions with a icg-tree structure which
     * mirrors the RTL structure.
     */
    if (nsubs > 0) {
      inode->left = translate_expression (XEXP (expr, 0), insn);
    }
    if (nsubs > 1) {
      inode->right = translate_expression (XEXP (expr, 1), insn);
    }
  }

  /* Add the offset part for subreg to the tree. */
  if (expr_code == SUBREG) {
    /*
     * (SUBREG (REG XX) OFFSET)
     * REG is already added. OFFSET is an integer that
     * should be separately added.
     */
     const int offset = XINT(expr, 1);
     icg_node *offset_node =
            icg_node_alloc (icg_op_of_constant(offset), insn, NULL);
     offset_node->val = offset;
     offset_node->a.string = NULL;
     inode->right = offset_node;
  }

  if (expr_code == NEG && (GET_MODE(expr)==SFmode || GET_MODE(expr)==DFmode)) {
    /*
     * TODO: somehow we have to register the constant 1LL<<63 or 1LL<<31
     * into the constant pool so that it is retained across icg
     * This comment was added whene exploring floating negates
     * in the context of gcc4.3.1.
     * Note: This may no longer be needed for gcc 4.4
     * Note that the RTL back end can not handle XOR_DF
     *   but has instead rules for NEG_DF,
     *   which it somehow converts to XOR_DF
     */
    const icg_opcodes mode_offset = inode->op - NEG_DI;
    const icg_opcodes new_label = XOR_DI + mode_offset;
    icg_node *msb_node = icg_node_alloc(CONST_0, 0, NULL);	/* TODO BOGUS */
    inode->op = new_label;	/* xor */
    inode->right = msb_node;	/* splice in bogus constant */
  }

  /* Do any post-construction transformations or optimizations */
  if (expr_code == MINUS) {
    /*
     * (MINUS_xx a b) => (PLUS_xx a (NEG_xx b))
     * This increases the scope of reassociation.
     *
     * There are special patterns in the grammar to turn this back into a MINUS.
     */
    const int mode_offset = inode->op - MINUS_DI;

    /* Insert NEG_xx node in between inode and the right child. */
    icg_node *neg_node = icg_node_alloc (NEG_DI + mode_offset, insn, NULL);
    neg_node->left = inode->right;
    neg_node->mode = inode->mode;	/* TODO: deprecaed */
    inode->right = neg_node;

    /* Convert MINUS_xx into PLUS_xx */
    inode->op = PLUS_DI + mode_offset;
  }

  gcc_assert (inode != NULL);
  return inode;
}

/*
 * Returns true if INSN should be translated into an icg tree.
 * Some insns such as (CLOBBER (reg)) are dead wood in icg.
 */
static
bool translate_expression_p (rtx insn)
{
  if (0
  || (GET_CODE (PATTERN (insn)) == CLOBBER)
  ) {
    return false;
  }
  return true;
}

/*
 * This calls the recursive function translate_expression.
 * It populates the op,left,right,val and (r,rx) fields of icg_node
 * Results accumulate in the vector icg_insn2tree and icg_insn2dirty
 */
static
void build_insn_tree(rtx curr_insn)
{
  if (translate_expression_p (curr_insn)) {
    icg_insn2tree[INSN_UID (curr_insn)] =
      translate_expression (PATTERN (curr_insn), curr_insn);
    icg_insn2dirty[INSN_UID (curr_insn)] = true;
  }
}


static int print_icg_rtx_dot(FILE *, rtx);
static int print_icg_tree_dot(FILE *, icg_node *curr_insn);


/*
 * This is the main driver function to build the side-tree for icg
 * Results accumulate in the vector icg_insn2tree and icg_insn2dirty
 */
unsigned icg_create_side_tree(void)
{
  basic_block bb;
  rtx curr_insn;
  static int make_dotfiles_mask = 0;
  FILE *dotfile    = (make_dotfiles_mask & (1<<0))
    ? fopen("icg_orig_rtx.dot", "w") : 0;
  FILE *icgdotfile = (make_dotfiles_mask & (1<<1))
    ? fopen("icg_side_tree.dot", "w") : 0;

  current_function_is_leaf = 1; /* we initialize it to 1, then clear it if we trip across a call */

  icg_insn2dirty  = (bool *)     icg_calloc(get_max_uid() * sizeof(bool));
  icg_insn2tree   = (icg_node **)icg_calloc(get_max_uid() * sizeof(icg_node *));
  icg_insn2goalNT = (int  *)     icg_calloc(get_max_uid() * sizeof(int));
  FOR_EACH_BB(bb)
  {
    FOR_BB_INSNS(bb, curr_insn)
    {
      if (!INSN_P(curr_insn)) {
	/*
	 * includes notes, code_labels, and other non-insns
	 */
        continue;
      }
      build_insn_tree(curr_insn);

      if (dotfile) {
	print_icg_rtx_dot(dotfile, curr_insn);
      }
      if (icgdotfile) {
	print_icg_tree_dot(icgdotfile, icg_insn2tree[INSN_UID(curr_insn)]);
      }
    }
  }

  if (dotfile) {
    fclose(dotfile); dotfile = 0;
  }
  if (icgdotfile) {
    fclose(icgdotfile); icgdotfile = 0;
  }

  return 0;
}


/* The following functions are used to dump the original rtx trees
 * and the side-trees as dot files.
 */

static
int for_each_dot_icg_rtx(FILE *dotfile, rtx curr_insn, int parent)
{
  static int nodenum = 0;
  enum rtx_code this_code;
  int node;
  int i = 0;
  int j = 0;
  const char *format_str;

  if (parent == 0)
    nodenum = 0;
  if (curr_insn == NULL_RTX)
    return 0;
  this_code= GET_CODE(curr_insn);

  nodenum++;
  node = nodenum;
  switch (this_code)
  {
     case REG :
        fprintf(dotfile,"n%d [label=\"%s : %d\"];\n", node,
              GET_RTX_NAME(this_code), XINT(curr_insn, 0));
        break;
     case CONST_INT :
        fprintf(dotfile,"n%d [label=\"%s : %lld\"];\n", node,
              GET_RTX_NAME(this_code), XWINT(curr_insn, 0));
        break;
     case SYMBOL_REF :
        fprintf(dotfile,"n%d [label=\"%s : %s\"];\n", node,
              GET_RTX_NAME(this_code), XSTR(curr_insn, 0));
        break;
     default :
        fprintf(dotfile,"n%d [label=\"%s\"];\n", node, GET_RTX_NAME(this_code));
        break;
  }
  if (parent >0) {
    fprintf(dotfile,"n%d -- n%d [label=\"%d\"];\n", parent, node, node);
  }

  format_str = GET_RTX_FORMAT(this_code);
  if (!format_str) {
    return 0;
  }
  i = 0;
  while (format_str[i]) {
    switch (format_str[i]) {
    case 'E' :
      if (this_code == (enum rtx_code)PARALLEL) {
	for (j = 0; j< XVECLEN (curr_insn, i); j++) {
	   for_each_dot_icg_rtx(dotfile, XVECEXP(curr_insn, i, j),
		 node);
	}
      }
      break;

    case 'e' :
      for_each_dot_icg_rtx(dotfile, XEXP(curr_insn, i),
	   node);
      break;

    default :
      break;
    }
    i++;
  }
  return 0;
}

static
int print_icg_rtx_dot(FILE *dotfile, rtx curr_insn)
{
  if (dotfile == 0)
    dotfile = stdout;
  fprintf(dotfile,"\n" "graph graph_%d {\n", INSN_UID(curr_insn));
  if (curr_insn) {
     for_each_dot_icg_rtx(dotfile, PATTERN(curr_insn), 0);
  }
  fprintf(dotfile,"}\n");
  return 0;
}

static
int for_each_dot_icg_tree(FILE *dotfile, icg_node *insn_node, int parent)
{
  static int nodenum = 0;
  int node;
  if (dotfile == 0)
    dotfile = stdout;

  if (parent == 0)
    nodenum= 0;
  if (!insn_node)
    return 0;
  nodenum++;
  node = nodenum;
  if (insn_node->op >= REG_DI && insn_node->op <= REG_SF) {
    fprintf(dotfile,"n%d [label=\"REG : %d\"];\n", node, insn_node->r);
  } else if (insn_node->op >= REGX_DI && insn_node->op <= REGX_SF) {
    fprintf(dotfile,"n%d [label=\"REGX : %d\"];\n", node, insn_node->r);
  } else {
    fprintf(dotfile,"n%d [label=\"%s\"];\n", node, get_icg_opcode_name(insn_node->op));
  }
  if (parent) {
    fprintf(dotfile,"n%d -- n%d;\n", parent, node);
  }
  for_each_dot_icg_tree(dotfile, insn_node->left, node);
  for_each_dot_icg_tree(dotfile, insn_node->right, node);
  return 0;
}

static
int print_icg_tree_dot(FILE *dotfile, icg_node *insn_node)
{
  static int graphnum = 0;
  if (dotfile == 0)
    dotfile = stdout;
  fprintf(dotfile,"\n" "graph graph_%d {\n", graphnum++);
  if (insn_node) {
    for_each_dot_icg_tree(dotfile, insn_node, 0);
  }
  fprintf(dotfile,"}\n");
  return 0;
}
