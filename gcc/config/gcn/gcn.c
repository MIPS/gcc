/* Copyright (C) 2016-2017 Free Software Foundation, Inc.

   This file is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This file is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "memmodel.h"
#include "rtl.h"
#include "tree.h"
#include "gimple.h"
#include "cfghooks.h"
#include "cfgloop.h"
#include "df.h"
#include "tm_p.h"
#include "stringpool.h"
#include "expmed.h"
#include "optabs.h"
#include "regs.h"
#include "emit-rtl.h"
#include "recog.h"
#include "diagnostic-core.h"
#include "insn-attr.h"
#include "alias.h"
#include "fold-const.h"
#include "stor-layout.h"
#include "calls.h"
#include "varasm.h"
#include "explow.h"
#include "expr.h"
#include "output.h"
#include "cfgrtl.h"
#include "cfgbuild.h"
#include "langhooks.h"
#include "reload.h"
#include "sched-int.h"
#include "params.h"
#include "gimplify.h"
#include "tm-constrs.h"
#include "ddg.h"
#include "dumpfile.h"
#include "builtins.h"
#include "rtl-iter.h"
#include "cfghooks.h"

/* This file should be included last.  */
#include "target-def.h"

static REAL_VALUE_TYPE dconst4, dconst1over2pi;
static bool ext_gcn_constants_init = 0;


/* Register modes.  */

/* Return true if MODE is valid for pair of VGPR registers.  */

static inline bool
vgpr_2reg_mode_p (enum machine_mode mode)
{
  return mode == DImode || mode == DFmode || mode == V64DFmode
	 || mode == V64DImode /*|| mode == V64BImode*/;
}

/* Return true if MODE can be handled directly by VGPR operations.  */

static inline bool
vgpr_vector_mode_p (enum machine_mode mode)
{
  return mode == V64HImode || mode == V64SImode || mode == V64DImode
	 || mode == V64HFmode || mode == V64SFmode || mode == V64DFmode;
}


/* Return true if MODE is valid for pair of SGPR registers.  */

static inline bool
sgpr_2reg_mode_p (enum machine_mode mode)
{
  return mode == DImode || mode == DFmode /*|| mode == V32BImode*/;
}

/* Return number of hard register needed to hold value of MODE in REGNO.  */

int
gcn_hard_regno_nregs (int regno, enum machine_mode mode)
{
  /* Scalar registers are 32bit, vector registers are in fact tuples of
     64 lanes.  */
  if (VGPR_REGNO_P (regno))
    {
      if (vgpr_1reg_mode_p (mode))
	return 1;
      if (vgpr_2reg_mode_p (mode))
	return 2;
    }
  return CEIL (GET_MODE_SIZE (mode), 4);
}

/* Register classes.  */

/* Implement the TARGET_CLASS_MAX_NREGS hook.

   On the 80386, this is the size of MODE in words,
   except in the FP regs, where a single reg is always enough.  */

static unsigned char
gcn_class_max_nregs (reg_class_t rclass, machine_mode mode)
{
  if (rclass == VGPR_REGS)
    {
      if (vgpr_1reg_mode_p (mode))
	return 1;
      if (vgpr_2reg_mode_p (mode))
	return 2;
    }
  return CEIL (GET_MODE_SIZE (mode), 4);
}

/* Return true if REGNO can hold value in MODE.  */

bool
gcn_hard_regno_mode_ok (int regno, machine_mode mode)
{
  switch (regno)
  {
    case FLAT_SCRATCH_LO_REG:
    case XNACK_MASK_LO_REG:
    case TBA_LO_REG:
    case TMA_LO_REG:
      return (mode == SImode || mode == DImode);
    case VCC_LO_REG:
    case EXEC_LO_REG:
      return (mode == CCmode || mode == CCmode
	      || mode == SImode || mode == DImode
	      /*|| mode == V32BImode || mode == V64BImode*/);
    case M0_REG:
    case FLAT_SCRATCH_HI_REG:
    case XNACK_MASK_HI_REG:
    case TBA_HI_REG:
    case TMA_HI_REG:
      return mode == SImode;
    case VCC_HI_REG:
    case EXEC_HI_REG:
      return mode == SImode /*|| mode == V32BImode*/;
    case SCC_REG:
    case VCCZ_REG:
    case EXECZ_REG:
      return mode == BImode;
  }
  /* FIXME: We restrict double register values to aligned registers.  */
  if (SGPR_REGNO_P (regno))
    return (sgpr_1reg_mode_p (mode)
	    || (!((regno - FIRST_VGPR_REG)&1) && sgpr_2reg_mode_p (mode)));
  if (VGPR_REGNO_P (regno))
    return (vgpr_1reg_mode_p (mode) || vgpr_2reg_mode_p (mode));
  return false;
}

/* Return smallest class containing REGNO.  */

enum reg_class
gcn_regno_reg_class (int regno)
{
  switch (regno)
    {
    case SCC_REG:
      return SCC_CONDITIONAL_REG;
    case VCCZ_REG:
      return VCCZ_CONDITIONAL_REG;
    case EXECZ_REG:
      return EXECZ_CONDITIONAL_REG;
    case EXEC_LO_REG:
    case EXEC_HI_REG:
      return EXEC_MASK_REG;
    }
  if (VGPR_REGNO_P (regno))
    return VGPR_REGS;
  if (SGPR_REGNO_P (regno))
    return SGPR_REGS;
  if (regno < FIRST_VGPR_REG)
    return GENERAL_REGS;
  return ALL_REGS;
}

/* GCC assumes that lowpart contains first part of value as stored in memory.
   This is not the case for vector registers.  */

bool gcn_cannot_change_mode_class (machine_mode from, machine_mode to,
				   int regclass)
{
  if (!vgpr_vector_mode_p (from) && !vgpr_vector_mode_p (to))
    return false;
  return gcn_class_max_nregs (regclass, from) != gcn_class_max_nregs (regclass, to);
}

static void
print_reg (FILE *file, rtx x)
{
  enum machine_mode mode = GET_MODE (x);
  if (mode == SImode || mode == BImode || mode == V64SImode
      || mode == HFmode || mode == SFmode || mode == V64SFmode
      || mode == V64SImode)
    fprintf (file, "%s", reg_names[REGNO (x)]);
  else if (mode == DImode || mode == V64DImode
	   || mode == DFmode || mode == V64DFmode)
    {
      if (SGPR_REGNO_P (REGNO (x)))
	fprintf (file, "s[%i:%i]", REGNO (x) - FIRST_SGPR_REG,
		 REGNO (x) - FIRST_SGPR_REG + 1);
      else if (VGPR_REGNO_P (REGNO (x)))
	fprintf (file, "v[%i:%i]", REGNO (x) - FIRST_VGPR_REG,
		 REGNO (x) - FIRST_VGPR_REG + 1);
      else if (REGNO (x) == FLAT_SCRATCH_REG)
	fprintf (file, "flat_scratch");
      else if (REGNO (x) == EXEC_REG)
	fprintf (file, "exec");
      else if (REGNO (x) == VCC_LO_REG)
	fprintf (file, "vcc");
      else
	fprintf (file, "[%s:%s]",
		 reg_names [REGNO (x)], reg_names [REGNO (x)+1]);
    }
  else if (mode == TImode)
    {
      if (SGPR_REGNO_P (REGNO (x)))
	fprintf (file, "s[%i:%i]", REGNO (x) - FIRST_SGPR_REG,
		 REGNO (x) - FIRST_SGPR_REG + 3);
      else if (VGPR_REGNO_P (REGNO (x)))
	fprintf (file, "v[%i:%i]", REGNO (x) - FIRST_VGPR_REG,
		 REGNO (x) - FIRST_VGPR_REG + 3);
      else
	gcc_unreachable ();
    }
  else
   gcc_unreachable ();
}


/* Initialize the table of extra 80387 mathematical constants.  */

static void
init_ext_gcn_constants (void)
{
  real_from_integer (&dconst4, DFmode, 4, SIGNED);

  /* FIXME: this constant probably does not match what hardware really loads.
     Reality check it eventually.  */
  real_from_string (&dconst1over2pi, "0.1591549430918953357663423455968866839");
  real_convert (&dconst1over2pi, SFmode, &dconst1over2pi);

  ext_gcn_constants_init = 1;
}

/* Return non-zero if X is a constant that can appear as inline operation.
   This is 0, 0.5, -0.5, 1, -1, 2, -2, 4,-4, 1/(2*pi)
   Or a vector of those.
   The value returned should be the encoding of this constant.  */

int
gcn_inline_fp_constant_p (rtx x, bool allow_vector)
{
  machine_mode mode = GET_MODE (x);

  if ((mode == V64HFmode || mode == V64SFmode || mode == V64DFmode)
      && allow_vector)
    {
      int n;
      if (GET_CODE (x) != CONST_VECTOR)
	return 0;
      n = gcn_inline_fp_constant_p (CONST_VECTOR_ELT (x, 0), false);
      if (!n)
	 return 0;
      for (int i = 1; i < 64; i++)
	if (CONST_VECTOR_ELT (x, i) != CONST_VECTOR_ELT (x, 0))
	  return 0;
      return 1;
    }

  if (mode != HFmode && mode != SFmode && mode != DFmode)
    return 0;

  const REAL_VALUE_TYPE *r;

  if (x == CONST0_RTX (mode))
    return 128;
  if (x == CONST1_RTX (mode))
    return 242;

  r = CONST_DOUBLE_REAL_VALUE (x);

  if (real_identical (r, &dconstm1))
    return 243;

  if (real_identical (r, &dconsthalf))
    return 240;
  if (real_identical (r, &dconstm1))
    return 243;
  if (real_identical (r, &dconst2))
    return 244;
  if (real_identical (r, &dconst4))
    return 246;
  if (real_identical (r, &dconst1over2pi))
    return 248;
  if (!ext_gcn_constants_init)
    init_ext_gcn_constants ();
  real_value_negate (r);
  if (real_identical (r, &dconsthalf))
    return 241;
  if (real_identical (r, &dconst2))
    return 245;
  if (real_identical (r, &dconst4))
    return 247;

  /* FIXME: add 4, -4 and 1/(2*PI).  */

  return 0;
}

/* Return non-zero if X is a constant that can appear as inline operation.
   This is 0, 0.5, -0.5, 1, -1, 2, -2, 4,-4, 1/(2*pi)
   Or a vector of those.
   The value returned should be the encoding of this constant.  */

bool
gcn_fp_constant_p (rtx x, bool allow_vector)
{
  machine_mode mode = GET_MODE (x);

  if ((mode == V64HFmode || mode == V64SFmode || mode == V64DFmode)
      && allow_vector)
    {
      int n;
      if (GET_CODE (x) != CONST_VECTOR)
	return false;
      n = gcn_fp_constant_p (CONST_VECTOR_ELT (x, 0), false);
      if (!n)
	 return false;
      for (int i = 1; i < 64; i++)
	if (CONST_VECTOR_ELT (x, i) != CONST_VECTOR_ELT (x, 0))
	  return false;
      return true;
    }
  if (mode != HFmode && mode != SFmode && mode != DFmode)
    return false;

  if (gcn_inline_fp_constant_p (x, false))
    return true;
  /* FIXME: It is not clear how 32bit immediates are interpreted here.  */
  return (mode != DFmode);
}

/* Return true if X is constant representable as inline constant.  */

bool
gcn_inline_constant_p (rtx x)
{
  if (GET_CODE (x) == CONST_INT)
    return INTVAL (x) >= -16 && INTVAL (x) < 64;
  if (GET_CODE (x) == CONST_DOUBLE)
    return gcn_inline_fp_constant_p (x, false);
  if (GET_CODE (x) == CONST_VECTOR)
    {
      int n;
      if (!vgpr_vector_mode_p (GET_MODE (x)))
	return false;
      if (GET_CODE (x) != CONST_VECTOR)
	return false;
      n = gcn_inline_constant_p (CONST_VECTOR_ELT (x, 0));
      if (!n)
	 return false;
      for (int i = 1; i < 64; i++)
	if (CONST_VECTOR_ELT (x, i) != CONST_VECTOR_ELT (x, 0))
	  return false;
      return 1;
   }
  return false;
}

/* Return true if X is constant representable as inline constant.  */

bool
gcn_constant_p (rtx x)
{
  if (GET_CODE (x) == CONST_INT)
    return trunc_int_for_mode (INTVAL (x), SImode) == INTVAL (x);
  if (GET_CODE (x) == CONST_DOUBLE)
    return gcn_fp_constant_p (x, false);
  if (GET_CODE (x) == CONST_VECTOR)
    {
      int n;
      if (!vgpr_vector_mode_p (GET_MODE (x)))
	return false;
      if (GET_CODE (x) != CONST_VECTOR)
	return false;
      n = gcn_constant_p (CONST_VECTOR_ELT (x, 0));
      if (!n)
	 return false;
      for (int i = 1; i < 64; i++)
	if (CONST_VECTOR_ELT (x, i) != CONST_VECTOR_ELT (x, 0))
	  return false;
      return 1;
   }
  return false;
}

/* GCN defines multiple ways to address memory.  */

enum gcn_address_type
{
  GCN_ADDR_LOAD,
  GCN_ADDR_STORE,
  GCN_ADDR_FLAT,
  GCN_ADDR_DS,
  GCN_ADDR_BUFFERED,
};

/* Determine instruction used to access memory in ADDRSPACE.  */

enum gcn_address_type
gcn_addr_space_type (addr_space_t addrspace)
{
  switch (addrspace)
    {
    case ADDR_SPACE_SCRATCH:
      return GCN_ADDR_BUFFERED;
    case ADDR_SPACE_FLAT:
      return GCN_ADDR_FLAT;
    case ADDR_SPACE_SCALAR_FLAT:
      return GCN_ADDR_LOAD;
    case ADDR_SPACE_LDS:
    case ADDR_SPACE_GDS:
      return GCN_ADDR_DS;
    default:
      gcc_unreachable ();
    }
}

void
print_operand_address_1 (FILE * file, register rtx addr, addr_space_t as)
{
  rtx reg;
  rtx offset;
  gcn_address_type type = gcn_addr_space_type (as);

  if (type == GCN_ADDR_BUFFERED)
    switch (GET_CODE (addr))
      {
      case REG:
	print_reg (file, addr);
	break;

      case PLUS:
	reg = XEXP (addr, 0);
	offset = XEXP (addr, 1);
	print_reg (file, reg);
	if (GET_CODE (offset) == CONST_INT)
	  fprintf (file, " offset:" HOST_WIDE_INT_PRINT_DEC,
		   INTVAL (offset));
	else
	  abort ();
	break;

      default:
	debug_rtx (addr);
	abort ();
      }
  else if (type == GCN_ADDR_FLAT)
    {
      gcc_assert (GET_CODE (addr) == REG);
      print_reg (file, addr);
    }
  else if (type == GCN_ADDR_DS)
    switch (GET_CODE (addr))
      {
      case REG:
	print_reg (file, addr);
	break;

      case PLUS:
	reg = XEXP (addr, 0);
	print_reg (file, reg);
	break;

      default:
	debug_rtx (addr);
	abort ();
      }
  else
    switch (GET_CODE (addr))
      {
      case REG:
	print_reg (file, addr);
	fprintf (file, ", 0");
	break;

      case PLUS:
	reg = XEXP (addr, 0);
	offset = XEXP (addr, 1);
	print_reg (file, reg);
	fprintf (file, ", ");
	if (GET_CODE (offset) == REG)
	  print_reg (file, reg);
	else if (GET_CODE (offset) == CONST_INT)
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC,
		   INTVAL (offset));
	else
	  abort ();
	break;

      default:
	debug_rtx (addr);
	abort ();
      }
}

void
print_operand_address (FILE * file, register rtx addr)
{
  print_operand_address_1 (file, addr, ADDR_SPACE_FLAT);
}

/* ASM printing.
   b - print operand size as untyped operand (b16/b32/b64)
   i - print operand size as untyped operand (i16/b32/i64)
   u - print operand size as untyped operand (u16/u32/u64)
   s - print operand size as memory access size
       (byte/short/dword/dwordx2/wordx3/dwordx4)
   C - print conditional code for s_cbranch (_sccz/_sccnz/_vccz/_vccnz...)
   D - print conditional code for s_cmp (eq_u64/lg_u64...)
   E - print conditional code for v_cmp (eq_u64/ne_u64...)
   A - print address in formating suitable for given address space.
   O - print offset:n for data share operations.
 */

void
print_operand (FILE * file, rtx x, int code)
{
  int xcode = GET_CODE (x);
  switch (code)
    {
      /* Instructions has following suffixes.
	 If there are two suffixes, first is destination type, second is
	 source type.

	 B32 Bitfield (untyped data) 32-bit
	 B64 Bitfield (untyped data) 64-bit
	 F16 floating-point 16-bit
	 F32 floating-point 32-bit (IEEE 754 single-precision float)
	 F64 floating-point 64-bit (IEEE 754 double-precision float)
	 I16 signed 32-bit integer
	 I32 signed 32-bit integer
	 I64 signed 64-bit integer
	 U16 unsigned 32-bit integer
	 U32 unsigned 32-bit integer
	 U64 unsigned 64-bit integer  */

      /* Print oprande size as untyped suffix.  */
    case 'b':
      {
	const char *s="";
	enum machine_mode mode = GET_MODE (x);
	if (VECTOR_MODE_P (mode))
	  mode = GET_MODE_INNER (mode);
	switch (GET_MODE_SIZE (mode))
	  {
	  case 2:
	    s="_b16";
	    break;
	  case 4:
	    s="_b32";
	    break;
	  case 8:
	    s="_b64";
	    break;
	  default:
	    output_operand_lossage ("invalid operand %%xn code");
	    return;
	  }
	fputs (s, file);
      }
      return;
    case 'i':
      {
	const char *s="";
	enum machine_mode mode = GET_MODE (x);
	if (VECTOR_MODE_P (mode))
	  mode = GET_MODE_INNER (mode);
	if (FLOAT_MODE_P (mode))
	  switch (GET_MODE_SIZE (mode))
	    {
	    case 2:
	      s="_f16";
	      break;
	    case 4:
	      s="_f32";
	      break;
	    case 8:
	      s="_f64";
	      break;
	    default:
	      output_operand_lossage ("invalid operand %%xn code");
	      return;
	    }
	else
	  switch (GET_MODE_SIZE (mode))
	    {
	    case 2:
	      s="_i16";
	      break;
	    case 4:
	      s="_i32";
	      break;
	    case 8:
	      s="_i64";
	      break;
	    default:
	      output_operand_lossage ("invalid operand %%xn code");
	      return;
	    }
	fputs (s, file);
      }
      return;
    case 'u':
      {
	const char *s="";
	enum machine_mode mode = GET_MODE (x);
	if (VECTOR_MODE_P (mode))
	  mode = GET_MODE_INNER (mode);
	switch (GET_MODE_SIZE (mode))
	  {
	  case 2:
	    s="_u16";
	    break;
	  case 4:
	    s="_u32";
	    break;
	  case 8:
	    s="_u64";
	    break;
	  default:
	    output_operand_lossage ("invalid operand %%xn code");
	    return;
	  }
	fputs (s, file);
      }
      return;
      /* Print oprande size as untyped suffix.  */
    case 's':
      {
	const char *s="";
	switch (GET_MODE_SIZE (GET_MODE (x)))
	  {
	  case 1:
	    s="_byte";
	    break;
	  case 2:
	    s="_short";
	    break;
	  case 4:
	    s="_dword";
	    break;
	  case 8:
	    s="_dwordx2";
	    break;
	  case 12:
	    s="_dwordx3";
	    break;
	  case 16:
	    s="_dwordx4";
	    break;
	  case 32:
	    s="_dwordx8";
	    break;
	  case 64:
	    s="_dwordx16";
	    break;
	  case 256:
	    s="_dword";
	    break;
	  default:
	    output_operand_lossage ("invalid operand %%xn code");
	    return;
	  }
	fputs (s, file);
      }
      return;
    case 'A':
      if (xcode != MEM)
	{
	  output_operand_lossage ("invalid %%xn code");
	  return;
	}
      print_operand_address_1 (file, XEXP (x, 0), MEM_ADDR_SPACE (x));
      return;
    case 'O':
      {
	if (xcode != MEM)
	  {
	    output_operand_lossage ("invalid %%xn code");
	    return;
	  }
	if ((unsigned) gcn_addr_space_type (MEM_ADDR_SPACE (x))
	    == ADDR_SPACE_GDS)
	  fprintf (file, " gds");
	if (GET_CODE (XEXP (x, 0)) == REG)
	  return;
	if (GET_CODE (XEXP (x, 0)) != PLUS)
	  {
	    output_operand_lossage ("invalid %%xn code");
	    return;
	  }
	rtx val = XEXP (x, (XEXP (x, 0), 1));
 	if (GET_CODE (val) == CONST_VECTOR)
	  val = CONST_VECTOR_ELT (val, 0);
 	if (GET_CODE (val) != CONST_INT)
	  {
	    output_operand_lossage ("invalid %%xn code");
	    return;
	  }
	fprintf (file, " offset:" HOST_WIDE_INT_PRINT_DEC,
		 INTVAL (val));

      }
      return;
    case 'C':
      {
	const char *s;
	bool num = false;
	if ((xcode != EQ && xcode != NE)
	    || !REG_P (XEXP (x, 0)))
	  {
	    output_operand_lossage ("invalid %%xn code");
	    return;
	  }
	switch (REGNO (XEXP (x, 0)))
	  {
	  case VCCZ_REG:
	    s="_vcc";
	    break;
	  case SCC_REG:
	    /* For some reason llvm as insist on scc0 instead of sccz.  */
	    num = true;
	    s="_scc";
	    break;
	  case EXECZ_REG:
	    s="_exec";
	    break;
	  default:
	    output_operand_lossage ("invalid %%xn code");
	    return;
	  }
	fputs (s, file);
	if (xcode == EQ)
	  fputc (num ? '0' : 'z', file);
	else
	  fputs (num ? "1" : "nz", file);
	return;
      }
    case 'D':
      {
	const char *s;
	bool cmp_signed = false;
	switch (xcode)
	  {
	  case EQ:
	    s="_eq_";
	    break;
	  case NE:
	    s="_lg_";
	    break;
	  case LT:
	    s="_lt_";
	    cmp_signed = true;
	    break;
	  case LE:
	    s="_le_";
	    cmp_signed = true;
	    break;
	  case GT:
	    s="_gt_";
	    cmp_signed = true;
	    break;
	  case GE:
	    s="_ge_";
	    cmp_signed = true;
	    break;
	  case LTU:
	    s="_lt_";
	    break;
	  case LEU:
	    s="_le_";
	    break;
	  case GTU:
	    s="_gt_";
	    break;
	  case GEU:
	    s="_ge_";
	    break;
	  default:
	    output_operand_lossage ("invalid %%xn code");
	    return;
	  }
	fputs (s, file);
	fputc (cmp_signed ? 'i' : 'u', file);

	enum machine_mode mode = GET_MODE (XEXP (x, 0));
	if (mode == VOIDmode)
	  mode = GET_MODE (XEXP (x, 1));
	switch (GET_MODE_SIZE (mode))
	  {
	  case 4:
	    s="32";
	    break;
	  case 8:
	    s="64";
	    break;
	  default:
	    output_operand_lossage ("invalid operand %%xn code");
	    return;
	  }
	fputs (s, file);
	return;
      }
    case 'E':
      {
	const char *s;
	bool cmp_signed = false;
	switch (xcode)
	  {
	  case EQ:
	    s="_eq_";
	    break;
	  case NE:
	    s="_ne_";
	    break;
	  case LT:
	    s="_lt_";
	    cmp_signed = true;
	    break;
	  case LE:
	    s="_le_";
	    cmp_signed = true;
	    break;
	  case GT:
	    s="_gt_";
	    cmp_signed = true;
	    break;
	  case GE:
	    s="_ge_";
	    cmp_signed = true;
	    break;
	  case LTU:
	    s="_lt_";
	    break;
	  case LEU:
	    s="_le_";
	    break;
	  case GTU:
	    s="_gt_";
	    break;
	  case GEU:
	    s="_ge_";
	    break;
	  default:
	    output_operand_lossage ("invalid %%xn code");
	    return;
	  }
	fputs (s, file);
	fputc (cmp_signed ? 'i' : 'u', file);

	enum machine_mode mode = GET_MODE (XEXP (x, 0));
	if (mode == VOIDmode)
	  mode = GET_MODE (XEXP (x, 1));
	switch (GET_MODE_SIZE (mode))
	  {
	  case 4:
	    s="32";
	    break;
	  case 8:
	    s="64";
	    break;
	  default:
	    output_operand_lossage ("invalid operand %%xn code");
	    return;
	  }
	fputs (s, file);
	return;
      }
    case 'L':
      print_operand (file, gcn_operand_part (GET_MODE (x), x, 0), 0);
      return;
    case 'H':
      print_operand (file, gcn_operand_part (GET_MODE (x), x, 1), 0);
      return;
    case 0:
      if (xcode == REG)
	print_reg (file, x);
      else if (xcode == MEM)
	output_address (GET_MODE (x), XEXP (x, 0));
      else if (xcode == CONST_INT)
	fprintf (file, "%i", (int)INTVAL (x));
      else if (xcode == CONST_VECTOR)
	print_operand (file, CONST_VECTOR_ELT (x, 0), code);
      else if (xcode == CONST_DOUBLE)
	{
	  const char *str;
	  switch (gcn_inline_fp_constant_p (x, false))
	    {
	    case 240:
	      str = "0.5";
	      break;
	    case 241:
	      str = "-0.5";
	      break;
	    case 242:
	      str = "1.0";
	      break;
	    case 243:
	      str = "-1.0";
	      break;
	    case 244:
	      str = "2.0";
	      break;
	    case 245:
	      str = "-2.0";
	      break;
	    case 246:
	      str = "4.0";
	      break;
	    case 247:
	      str = "-4.0";
	      break;
	    case 248:
	      str = "1/pi";
	      break;
	    default:
	      rtx ix = simplify_gen_subreg (GET_MODE (x) == DFmode
					    ? DImode : SImode,
					    x, GET_MODE (x), 0);
	      if (x)
		print_operand (file, ix, code);
	      else
		output_operand_lossage ("invlaid fp constant");
	      return;
	      break;
	    }
	  fprintf (file, str);
	  return;
	}
      else
	output_addr_const (file, x);
      return;
    default:
      output_operand_lossage ("invalid %%xn code");
    }
  gcc_unreachable ();
}


/* Addressing.  */

/* Return true if X is CONST_VECTOR of single constant.  */

static bool
single_cst_vector_p (rtx x)
{
  if (GET_CODE (x) != CONST_VECTOR)
    return false;
  for (int i = 1; i < 64; i++)
    if (CONST_VECTOR_ELT (x, i) != CONST_VECTOR_ELT (x, 0))
      return false;
  return true;
}

/* Recognizes RTL expressions that are valid memory addresses for an
   instruction.  The MODE argument is the machine mode for the MEM
   expression that wants to use this address.

   It only recognizes address in canonical form.  LEGITIMIZE_ADDRESS should
   convert common non-canonical forms to canonical form so that they will
   be recognized.  */

static bool
gcn_legitimate_address_p (machine_mode mode, rtx x, bool,
			  enum gcn_address_type type)
{
  enum machine_mode addr_mode;
  switch (type)
    {
    case GCN_ADDR_LOAD:
    case GCN_ADDR_STORE:
      addr_mode = DImode;
      break;
    case GCN_ADDR_BUFFERED:
      addr_mode = SImode;
      break;
    case GCN_ADDR_FLAT:
      if (vgpr_vector_mode_p (mode))
	addr_mode = V64DImode;
      else
	addr_mode = DImode;
      break;
    case GCN_ADDR_DS:
      if (vgpr_vector_mode_p (mode))
	addr_mode = V64SImode;
      else
	addr_mode = SImode;
      break;
    }
  if (GET_MODE (x) != addr_mode)
    return false;
  if (type == GCN_ADDR_LOAD || type == GCN_ADDR_STORE)
    switch (GET_CODE (x))
      {
      case REG:
	return (REGNO (x) >= FIRST_PSEUDO_REGISTER
		|| gcn_sgpr_register_operand (x, DImode));
      /* Addresses are in the form BASE+OFFSET
	 OFFSET is either 20bit unsigned immediate, SGPR or M0.
	 Writes and atomics do not accept SGPR.  */
      case PLUS:
	{
	  rtx x0 = XEXP (x, 0);
	  rtx x1 = XEXP (x, 1);
	  if (GET_CODE (x0) != REG
	      || (REGNO (x0) <= FIRST_PSEUDO_REGISTER
		  && !gcn_sgpr_register_operand (x0, DImode)))
	    return false;
	  if (GET_CODE (x1) == REG)
	    {
	      if (GET_CODE (x1) != REG
		  || (REGNO (x1) == M0_REG
		      /* FIXME: maybe this is about parameter of a store.  */
		      && (type == GCN_ADDR_STORE
			  || (REGNO (x1) <= FIRST_PSEUDO_REGISTER
			      && !gcn_sgpr_register_operand (x1, DImode)))))
		return false;
	    }
	  else if (GET_CODE (x1) == CONST_INT)
	    {
	      if (INTVAL (x1) >= 0 && INTVAL (x1) < (1<<20))
		return true;
	    }
	  return false;
	}

      default:
	break;
      }
  else if (type == GCN_ADDR_BUFFERED)
    {
      return (GET_CODE (x) == REG
	      && (REGNO (x) >= FIRST_PSEUDO_REGISTER
		  || gcn_sgpr_register_operand (x, SImode)));
    }
  else if (type == GCN_ADDR_FLAT)
    {
      return (GET_CODE (x) == REG
	      && (REGNO (x) >= FIRST_PSEUDO_REGISTER
		  || gcn_vgpr_register_operand (x, DImode)));
    }
  else if (type == GCN_ADDR_DS)
    switch (GET_CODE (x))
      {
      case REG:
	return (REGNO (x) >= FIRST_PSEUDO_REGISTER
		|| gcn_vgpr_register_operand (x, DImode));
      /* Addresses are in the form BASE+OFFSET
	 OFFSET is either 20bit unsigned immediate, SGPR or M0.
	 Writes and atomics do not accept SGPR.  */
      case PLUS:
	{
	  rtx x0 = XEXP (x, 0);
	  rtx x1 = XEXP (x, 1);
	  if (GET_CODE (x0) != REG
	      || (REGNO (x0) <= FIRST_PSEUDO_REGISTER
		  && !gcn_vgpr_register_operand (x0, DImode)))
	    return false;
	  if (GET_CODE (x1) == REG)
	    {
	      if (GET_CODE (x1) != REG
		  || (REGNO (x1) == M0_REG
		      /* FIXME: maybe this is about parameter of a store.  */
		      && (type == GCN_ADDR_STORE
			  || (REGNO (x1) <= FIRST_PSEUDO_REGISTER
			      && !gcn_sgpr_register_operand (x1, DImode)))))
		return false;
	    }
	  else if (GET_CODE (x1) == CONST_VECTOR
		   && GET_CODE (CONST_VECTOR_ELT (x1, 0)) == CONST_INT
		   && single_cst_vector_p (x1))
	    {
	      x1 = CONST_VECTOR_ELT (x1, 0);
	      if (INTVAL (x1) >= 0 && INTVAL (x1) < (1<<20))
		return true;
	    }
	  return false;
	}

      default:
	break;
      }
  else
    gcc_unreachable ();
  return false;
}

/* Recognizes RTL expressions that are valid memory addresses for an
   instruction.  The MODE argument is the machine mode for the MEM
   expression that wants to use this address.

   It only recognizes address in canonical form.  LEGITIMIZE_ADDRESS should
   convert common non-canonical forms to canonical form so that they will
   be recognized.  */

static bool
gcn_addr_space_legitimate_address_p (machine_mode mode, rtx x, bool strict,
				     addr_space_t as)
{
  return gcn_legitimate_address_p (mode, x, strict, gcn_addr_space_type (as));
}


/* Return the appropriate mode for a named address pointer.  */
static machine_mode
gcn_addr_space_pointer_mode (addr_space_t addrspace)
{
  switch (addrspace)
    {
    case ADDR_SPACE_SCRATCH:
    case ADDR_SPACE_LDS:
    case ADDR_SPACE_GDS:
      return SImode;
    case ADDR_SPACE_FLAT:
    case ADDR_SPACE_SCALAR_FLAT:
      return DImode;
    default:
      gcc_unreachable ();
    }
}

/* Return the appropriate mode for a named address address.  */
static machine_mode
gcn_addr_space_address_mode (addr_space_t addrspace)
{
  return gcn_addr_space_pointer_mode (addrspace);
}

/* Determine if one named address space is a subset of another.  */

static bool
gcn_addr_space_subset_p (addr_space_t subset, addr_space_t superset)
{
  if (subset == superset)
    return true;
  if (superset == ADDR_SPACE_FLAT || superset == ADDR_SPACE_SCALAR_FLAT)
    return true;
  return false;
}

/* Convert from one address space to another.  */
static rtx
gcn_addr_space_convert (rtx /*op*/, tree /*from_type*/, tree /*to_type*/)
{
  gcc_unreachable ();
}

/* Retun true if REGNO is OK for memory adressing.  */

bool
gcn_regno_mode_code_ok_for_base_p (int regno,
				   machine_mode, addr_space_t, int, int)
{
  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (reg_renumber[regno])
	regno = reg_renumber[regno];
      else
	return true;
    }
  return SGPR_REGNO_P (regno);
}

/* Retun true if REGNO is OK for memory adressing.  */

reg_class
gcn_mode_code_base_reg_class (machine_mode /*mode*/, addr_space_t as, int, int)
{
  switch (gcn_addr_space_type (as))
    {
    case GCN_ADDR_LOAD:
    case GCN_ADDR_STORE:
    case GCN_ADDR_BUFFERED:
      return GENERAL_REGS;
      break;
    case GCN_ADDR_FLAT:
    case GCN_ADDR_DS:
      return VGPR_REGS;
    }
  gcc_unreachable ();
}

/* Return true if REGNO is OK for index of memory addressing.  */

bool
regno_ok_for_index_p (int regno)
{
  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (reg_renumber[regno])
	regno = reg_renumber[regno];
      else
	return true;
    }
  return regno == M0_REG || VGPR_REGNO_P (regno);
}

/* "Function" calling conventions.  */

struct gcn_kernel_arg_type
{
  const char *name;
  const char *header_pseudo;
  int mode;
  int fixed_regno;
};

#define PRIVATE_SEGMENT_BUFFER_ARG 1
#define KERNARG_SEGMENT_PTR 4
#define FLAT_SCRATCH_INIT_ARG 6
#define FLAT_SCRATCH_SEGMENT_SIZE_ARG 7
#define WORKGROUP_ID_X_ARG 11
#define PRIVATE_SEGMENT_WAVE_OFFSET_ARG 15
#define WORK_ITEM_ID_Y_ARG 17
#define WORK_ITEM_ID_Z_ARG 18

static const struct gcn_kernel_arg_type gcn_kernel_arg_types[] =
{
  {"exec", NULL, DImode, EXEC_REG},
  {"private_segment_buffer", "enable_sgpr_private_segment_buffer", TImode, -1},
  {"dispatch_ptr", "enable_sgpr_dispatch_ptr", DImode, -1},
  {"queue_ptr", "enable_sgpr_queue_ptr", DImode, -1},
  {"kernarg_segment_ptr", "enable_sgpr_kernarg_segment_ptr", DImode, -1},
  {"dispatch_id", "enable_sgpr_dispatch_id", DImode, -1},
  {"flat_scratch_init", "enable_sgpr_flat_scratch_init", DImode, -1},
  {"private_segment_size", "enable_sgpr_private_segment_size", SImode, -1},
  {"grid_workgroup_count_X", "enable_sgpr_grid_workgroup_count_x", SImode, -1},
  {"grid_workgroup_count_Y", "enable_sgpr_grid_workgroup_count_y", SImode, -1},
  {"grid_workgroup_count_Z", "enable_sgpr_grid_workgroup_count_z", SImode, -1},
  {"workgroup_id_X", NULL /*"enable_sgpr_workgroup_id_x"*/, SImode, -1},
  {"workgroup_id_Y", NULL /*"enable_sgpr_workgroup_id_y"*/, SImode, -1},
  {"workgroup_id_Z", NULL /*"enable_sgpr_workgroup_id_z"*/, SImode, -1},
  {"workgroup_info", NULL /*"enable_sgpr_workgroup_info"*/, SImode, -1},
  {"private_segment_wave_offset",
   NULL /*"enable_sgpr_private_segment_wave_offset"*/, SImode, -1},
  {"work_item_id_X", NULL, V64SImode, FIRST_VGPR_REG},
  {"work_item_id_Y", NULL, V64SImode, FIRST_VGPR_REG + 1},
  {"work_item_id_Z", NULL, V64SImode, FIRST_VGPR_REG + 2}};

static bool
gcn_parse_amdgpu_hsa_kernel_attribute (struct gcn_kernel_args *args, tree list)
{
  bool err = false;
  args->requested = (1 << PRIVATE_SEGMENT_BUFFER_ARG)
		    | (1 << FLAT_SCRATCH_INIT_ARG)
		    | (1 << KERNARG_SEGMENT_PTR)
		    | (1 << PRIVATE_SEGMENT_WAVE_OFFSET_ARG);
  args->nargs = 0;

  for (int a = 0; a < GCN_KERNEL_ARG_TYPES; a++)
    args->reg[a] = -1;

  for (; list; list = TREE_CHAIN (list))
    {
      const char *str;
      if (TREE_CODE (TREE_VALUE (list)) != STRING_CST)
	{
	  error ("amdgpu_hsa_kernel attribute requires string constant "
		 "arguments");
	  break;
	}
      str = TREE_STRING_POINTER (TREE_VALUE (list));
      int a;
      for (a = 0; a < GCN_KERNEL_ARG_TYPES; a++)
	{
	  if (!strcmp (str, gcn_kernel_arg_types[a].name))
	    break;
	}
      if (a == GCN_KERNEL_ARG_TYPES)
	{
	  error ("unknown specifier %s in amdgpu_hsa_kernel attribute",
		 str);
	  err = true;
	  break;
	}
      if (args->requested & (1<<a))
	{
	  error ("duplicated parameter specifier %s in amdgpu_hsa_kernel "
		 "attribute",
		 str);
	  err = true;
	  break;
	}
      args->requested |= (1<<a);
      args->order[args->nargs++] = a;
    }
  args->requested |= (1 << WORKGROUP_ID_X_ARG);

  int sgpr_regno = FIRST_SGPR_REG;
  for (int a = 0; a < GCN_KERNEL_ARG_TYPES; a++)
    {
      if (!(args->requested & (1<<a)))
	continue;

      if (gcn_kernel_arg_types[a].fixed_regno > 0)
	args->reg[a] = gcn_kernel_arg_types[a].fixed_regno;
      else if (gcn_kernel_arg_types[a].mode == SImode)
	args->reg[a] = sgpr_regno++;
      else if (gcn_kernel_arg_types[a].mode == DImode)
	{
	  args->reg[a] = sgpr_regno;
	  sgpr_regno += 2;
	}
      else if (gcn_kernel_arg_types[a].mode == TImode)
	{
	  args->reg[a] = sgpr_regno;
	  sgpr_regno += 4;
	}
      else
	gcc_unreachable ();
    }
  args->nsgprs = sgpr_regno;
  if (sgpr_regno > FIRST_SGPR_REG + 16)
    {
      error ("too many arguments passed in sgpr registers");
    }
  return err;
}

static tree
gcn_handle_amdgpu_hsa_kernel_attribute (tree *node, tree name,
					tree args, int, bool *no_add_attrs)
{
  if (TREE_CODE (*node) != FUNCTION_TYPE
      && TREE_CODE (*node) != METHOD_TYPE
      && TREE_CODE (*node) != FIELD_DECL
      && TREE_CODE (*node) != TYPE_DECL)
    {
      warning (OPT_Wattributes, "%qE attribute only applies to functions",
	       name);
      *no_add_attrs = true;
      return NULL_TREE;
    }

  /* Can combine regparm with all attributes but fastcall, and thiscall.  */
  if (is_attribute_p ("gcnhsa_kernel", name))
    {
      struct gcn_kernel_args kernelarg;

      if (gcn_parse_amdgpu_hsa_kernel_attribute (&kernelarg, args))
	*no_add_attrs = true;

      return NULL_TREE;
    }

  return NULL_TREE;
}

/* Define how to find the value returned by a function.
   VALTYPE is the data type of the value (as a tree).
   If the precise function being called is known, FUNC is its FUNCTION_DECL;
   otherwise, FUNC is 0.  */

static rtx
gcn_function_value (const_tree valtype, const_tree, bool)
{
  /* FIXME: There is no way to return value.
     The value will be simply ignored.  Perhaps generic code can be told
     to handle NULL in this case.  */
  return gen_rtx_REG (TYPE_MODE (valtype), SGPR_REGNO (0));
}

/* Return true if N is a possible register number of function value.  */

static bool
gcn_function_value_regno_p (const unsigned int)
{
  return false;
}

/* Find a location for the static chain incoming to a nested function.
   This is a register, unless all free registers are used by arguments.  */

static rtx
gcn_static_chain (const_tree, bool)
{
  /* FIXME: there is no function ABI, so we don't support nested
     functions either.  */
  return NULL;
}


static rtx
gcn_function_arg (cumulative_args_t cum_v, machine_mode mode, const_tree type,
		  bool)
{
  CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
  if (cum->num >= cum->args.nargs)
    {
      cum->offset = (cum->offset + TYPE_ALIGN (type) / 8 - 1)
	& -(TYPE_ALIGN (type) / 8);
      cfun->machine->kernarg_segment_alignment
	= MAX ((unsigned) cfun->machine->kernarg_segment_alignment,
	       TYPE_ALIGN (type) / 8);
      rtx addr = gen_rtx_REG (DImode, cum->args.reg [KERNARG_SEGMENT_PTR]);
      if (cum->offset)
	addr = gen_rtx_PLUS (DImode, addr, gen_int_mode (cum->offset, mode));
      rtx mem = gen_rtx_MEM (mode, addr);
      set_mem_attributes (mem, const_cast<tree>(type), 1);
      set_mem_addr_space (mem, ADDR_SPACE_SCALAR_FLAT);
      MEM_READONLY_P (mem) = 1;
      return mem;
    }

  int a = cum->args.order[cum->num];
  if (mode != gcn_kernel_arg_types[a].mode)
    {
      error ("wrong type of argument %s", gcn_kernel_arg_types[a].name);
      return 0;
    }
  return gen_rtx_REG ((machine_mode)gcn_kernel_arg_types[a].mode,
		      cum->args.reg[a]);
}

static void
gcn_function_arg_advance (cumulative_args_t cum_v, machine_mode,
			  const_tree type, bool)
{
  CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
  if (cum->num < cum->args.nargs)
    cum->num++;
  else
    {
      cum->offset += tree_to_uhwi (TYPE_SIZE_UNIT (type));
      cfun->machine->kernarg_segment_byte_size = cum->offset;
    }
}


/* Initialize a variable CUM of type CUMULATIVE_ARGS for a call to a function
   whose data type is FNTYPE.  For a library call, FNTYPE is 0.  */

void
gcn_init_cumulative_args (CUMULATIVE_ARGS *cum /* Argument info to init */,
			  tree fntype /* tree ptr for function decl */,
			  rtx libname /* SYMBOL_REF of library name or 0 */,
			  tree fndecl,
			  int caller)
{
  memset (cum, 0, sizeof (*cum));
  if (libname)
    {
      sorry ("no one decided on calling convention yet: can not expand "
	     "libcall %s0", XSTR (libname, 0));
      return;
    }
  tree attr = NULL;
  if (fndecl)
    attr = lookup_attribute ("amdgpu_hsa_kernel", DECL_ATTRIBUTES (fndecl));
  if (fndecl && !attr)
    attr = lookup_attribute ("amdgpu_hsa_kernel",
			     TYPE_ATTRIBUTES (TREE_TYPE (fndecl)));
  if (!attr && fntype)
    attr = lookup_attribute ("amdgpu_hsa_kernel", TYPE_ATTRIBUTES (fntype));
  /* Handle main () as kernel, so we can run testsuite.  */
  if (!attr && !caller && fndecl && MAIN_NAME_P (DECL_NAME (fndecl)))
    gcn_parse_amdgpu_hsa_kernel_attribute (&cum->args, NULL_TREE);
  else
    {
      if (!attr || caller)
	{
	  sorry ("no one decided on calling convention yet; please just do "
		 "not call functions");
	  return;
	}
      gcn_parse_amdgpu_hsa_kernel_attribute
	 (&cum->args, TREE_VALUE (attr));
    }
  cfun->machine->args = cum->args;
  /* We changed regno of frame pointer.  */
  init_emit_regs ();
  init_regs ();
}

/* Expanders.  */

/* Return N-th part of value occupying multiple registers.  */

rtx
gcn_operand_part (enum machine_mode mode, rtx op, int n)
{
  if (mode == V64DImode || mode == V64SImode)
    {
      if (REG_P (op))
	{
	  gcc_assert (REGNO (op) + n < FIRST_PSEUDO_REGISTER);
	  return gen_rtx_REG (V64SImode, REGNO (op) + n);
	}
      if (GET_CODE (op) == CONST_VECTOR)
	{
	  int units = GET_MODE_NUNITS (mode);
	  rtvec v = rtvec_alloc (units);

	  for (int i = 0; i < units; ++i)
	    RTVEC_ELT (v, i) = gcn_operand_part (GET_MODE_INNER (mode),
						 CONST_VECTOR_ELT (op, i), n);

	  return gen_rtx_CONST_VECTOR (mode, v);
	}
      if (GET_CODE (op) == UNSPEC
	  && XINT (op, 1) == UNSPEC_VECTOR)
	return gcn_gen_undef (V64SImode);
      gcc_unreachable ();
    }
  else
    {
      if (GET_CODE (op) == UNSPEC
	  && XINT (op, 1) == UNSPEC_VECTOR)
	return gcn_gen_undef (SImode);
      return simplify_gen_subreg (SImode, op, mode, n * 4);
    }
}

/* Return N-th part of value occupying multiple registers.  */

rtx
gcn_operand_doublepart (enum machine_mode mode, rtx op, int n)
{
  return simplify_gen_subreg (DImode, op, mode, n * 8);
}

/* Split all operands in OPERANDS into parts so first NOPERANDS corresponds
   to the first part and so on.  */

void
gcn_split_operands (enum machine_mode mode, rtx *operands,
		    int nparts, int noperands)
{
  for (int i = nparts - 1; i >= 0; i--)
    for (int j = 0; j < noperands; j++)
      operands [i*noperands + j] = gcn_operand_part (mode, operands[j], i);
}

/* Return true if OP can be splitted.  */

bool
gcn_can_split_p (enum machine_mode, rtx op)
{
  if (vgpr_vector_mode_p (GET_MODE (op)))
    {
      if (GET_CODE (op) == SUBREG)
	op = SUBREG_REG (op);
      if (!REG_P (op))
	return true;
      return REGNO (op) <= FIRST_PSEUDO_REGISTER;
    }
  return true;
}

/* Return true if all of OPERANDS can be splitted by gcnsplit_operands.  */

bool
gcn_can_split_operands_p (enum machine_mode mode, rtx *operands,
			  int noperands)
{
  for (int i = noperands - 1; i >= 0; i--)
    if (!gcn_can_split_p (mode, operands[i]))
      return false;
  return true;
}

rtx
gcn_gen_undef (enum machine_mode mode)
{
  return gen_rtx_UNSPEC (mode, gen_rtvec (1, const0_rtx), UNSPEC_VECTOR);
}

rtx
gcn_vec_constant (enum machine_mode mode, int a)
{
  /*if (!a)
    return CONST0_RTX (mode);
  if (a == -1)
    return CONSTM1_RTX (mode);
  if (a == 1)
    return CONST1_RTX (mode);
  if (a == 2)
    return CONST2_RTX (mode);*/

  int units = GET_MODE_NUNITS (mode);
  rtx tem = gen_int_mode (a, GET_MODE_INNER (mode));
  rtvec v = rtvec_alloc (units);

  for (int i = 0; i < units; ++i)
    RTVEC_ELT (v, i) = tem;

  return gen_rtx_CONST_VECTOR (mode, v);
}

rtx
gcn_vec_constant (enum machine_mode mode, rtx a)
{
  int units = GET_MODE_NUNITS (mode);
  rtvec v = rtvec_alloc (units);

  for (int i = 0; i < units; ++i)
    RTVEC_ELT (v, i) = a;

  return gen_rtx_CONST_VECTOR (mode, v);
}

static rtx
get_exec (int64_t val)
{
  rtx reg = gen_reg_rtx (DImode);
  emit_insn (gen_rtx_SET (reg, gen_int_mode (val, DImode)));
  return reg;
}

static rtx
get_exec (rtx val)
{
  rtx reg = gen_reg_rtx (DImode);
  emit_insn (gen_rtx_SET (reg, val));
  return reg;
}

/* Generate move which uses the exec flags.  */

rtx
gen_mov_with_exec (rtx op0, rtx op1, rtx exec = NULL)
{
  enum machine_mode mode = GET_MODE (op0);
  if (vgpr_vector_mode_p (mode))
    return (gen_rtx_SET (op0,
			 gen_rtx_VEC_MERGE (mode, op1, gcn_gen_undef (mode),
					    exec)));
  return (gen_rtx_PARALLEL
	   (VOIDmode,
	    gen_rtvec (2, gen_rtx_SET (op0, op1),
		       gen_rtx_USE (VOIDmode, exec ? exec
					      : gcn_scalar_exec ()))));
}

/* Load vector constant where n-th lane contains BASE+n*VAL.  */

static rtx
strided_constant (enum machine_mode mode, int base, int val)
{
  rtx x = gen_reg_rtx (mode);
  emit_insn (gen_mov_with_exec (x, gcn_vec_constant (mode, base),
				gcn_full_exec_reg ()));
  emit_insn (gen_addv64si3_vector (x, x, gcn_vec_constant (mode, val * 32),
				   get_exec (0xffffffff00000000), x));
  emit_insn (gen_addv64si3_vector (x, x, gcn_vec_constant (mode, val * 16),
				   get_exec (0xffff0000ffff0000), x));
  emit_insn (gen_addv64si3_vector (x, x, gcn_vec_constant (mode, val * 8),
				   get_exec (0xff00ff00ff00ff00),  x));
  emit_insn (gen_addv64si3_vector (x, x, gcn_vec_constant (mode, val * 4),
				   get_exec (0xf0f0f0f0f0f0f0f0), x));
  emit_insn (gen_addv64si3_vector (x, x, gcn_vec_constant (mode, val * 2),
				   get_exec (0xcccccccccccccccc), x));
  emit_insn (gen_addv64si3_vector (x, x, gcn_vec_constant (mode, val * 1),
				   get_exec (0xaaaaaaaaaaaaaaaa), x));
  return x;
}

/* Prepare address to load vector of MODE from address OP.  */

static rtx
prepare_flat_address (enum machine_mode mode, rtx op, rtx exec)
{
  rtx addrs = gen_reg_rtx (V64DImode);
  rtx base = force_reg (DImode, XEXP (op, 0));
  rtx offsets = strided_constant (V64SImode, 0, GET_MODE_UNIT_SIZE (mode));

  emit_insn (gen_vec_duplicatev64di
	      (addrs, base, exec, gcn_gen_undef (V64DImode)));

  emit_insn (gen_addv64di3_zext (addrs, offsets, addrs, exec,
				 gcn_gen_undef (V64DImode)));
  return change_address (op, GET_MODE (op), addrs);
}

/* Legitmize address X.  */

static rtx
gcn_addr_space_legitimize_address (rtx x, rtx, machine_mode mode,
				   addr_space_t as)
{
  switch (gcn_addr_space_type (as))
    {
    case GCN_ADDR_LOAD:
    case GCN_ADDR_STORE:
    case GCN_ADDR_BUFFERED:
      return x;
      break;
    case GCN_ADDR_FLAT:
      if (vgpr_vector_mode_p (mode)
	  && GET_MODE (x) != V64DImode)
	{
	  rtx exec = gcn_full_exec_reg ();
	  rtx addrs = gen_reg_rtx (V64DImode);
	  rtx base = force_reg (DImode, x);
	  rtx offsets = strided_constant (V64SImode, 0,
					  GET_MODE_UNIT_SIZE (mode));

	  emit_insn (gen_vec_duplicatev64di
		      (addrs, base, exec, gcn_gen_undef (V64DImode)));

	  emit_insn (gen_addv64di3_zext (addrs, offsets, addrs, exec,
					 gcn_gen_undef (V64DImode)));
	  return addrs;
	}
    case GCN_ADDR_DS:
      /* FIXME: LDS support offsets, handle them!.  */
      if (vgpr_vector_mode_p (mode)
	  && GET_MODE (x) != V64SImode)
	{
	  rtx exec = gcn_full_exec_reg ();
	  rtx addrs = gen_reg_rtx (V64SImode);
	  rtx base = force_reg (SImode, x);
	  rtx offsets = strided_constant (V64SImode, 0,
					  GET_MODE_UNIT_SIZE (mode));

	  emit_insn (gen_vec_duplicatev64si
		      (addrs, base, exec, gcn_gen_undef (V64SImode)));

	  emit_insn (gen_addv64si3_vector (addrs, offsets, addrs, exec,
					   gcn_gen_undef (V64SImode)));
	  return addrs;
	}
      return x;
    }
  gcc_unreachable ();
}

/* Move expander: move op1 to op0 in MODE.
   Only handle special cases that can not be handled directly.
   Return true if RTX was generated.  */

bool
gcn_expand_mov (enum machine_mode mode, rtx op0, rtx op1)
{
  /* At least one of the operands needs to be a register.  */
  if (!register_operand (op0, mode) && !register_operand (op1, mode))
    {
      rtx temp = force_reg (mode, op1);
      emit_move_insn (op0, temp);
      return 1;
    }
  if (GET_MODE (op0) == QImode || GET_MODE (op0) == HImode)
    {
      /* We do not have sub-word register move patterns; just promote to
	 SImode.  */
      if (!memory_operand (op0, mode) && !memory_operand (op1, mode))
	{
	  op0 = simplify_gen_subreg (SImode, op0, mode, 0);
	  op1 = simplify_gen_subreg (SImode, op1, mode, 0);
	}
      /* Scalar flat loads do not support sub-word accesses; emulate.
	 Hardware ignored lower 3 bits of the address, so we need to load
	 aligned address and adjust */
      else if (GET_CODE (op1) == MEM
	       && MEM_ADDR_SPACE (op1) == ADDR_SPACE_SCALAR_FLAT)
	{
	  rtx addr = force_reg (DImode, XEXP (op1, 0));
	  int align = get_mem_align_offset (op1, 32);
	  rtx tmp = gen_reg_rtx (SImode);

	  if (align >= 0)
	    {
	      rtx mem = widen_memory_access (op1, SImode, -align / 8);
	      emit_move_insn (tmp, mem);
	      if (align)
		tmp
		  = expand_simple_binop (SImode, ASHIFTRT, tmp,
					 GEN_INT (align * 8), NULL_RTX, 1,
					 OPTAB_DIRECT);
		emit_move_insn (simplify_gen_subreg (SImode, op0, mode, 0),
				tmp);
	    }
	  else
	    {
	      rtx aligned = gen_reg_rtx (DImode);
	      emit_move_insn (addr, aligned);
	      emit_insn (gen_andsi3 (simplify_gen_subreg (SImode, aligned,
							  DImode, 0),
				     simplify_gen_subreg (SImode, aligned,
							  DImode, 0) ,
			 gen_int_mode (-3, DImode)));
	      rtx mem = change_address (op1, SImode, aligned);
	      set_mem_align (mem, 4) ;
	      emit_move_insn (tmp, mem);
	      rtx offset
		= expand_simple_binop (SImode, AND,
				       simplify_gen_subreg (SImode, addr,
							    DImode, 0),
				     gen_int_mode (3, DImode),
				     NULL_RTX, 1, OPTAB_DIRECT);
	      offset
		= expand_simple_binop (SImode, ASHIFT, offset,
				       gen_int_mode (3, DImode), NULL_RTX, 1,
				       OPTAB_DIRECT);
	      rtx tmp2
		= expand_simple_binop (SImode, ASHIFTRT, tmp, offset, NULL_RTX,
				       1, OPTAB_DIRECT);
	      emit_move_insn (simplify_gen_subreg (SImode, op0, mode, 0), tmp2);
	    }
	  return 1;
	}
    }

  if (GET_CODE (op1) == CONST_VECTOR && !gcn_constant_p (op1))
    {
      gcn_expand_vector_init (op0, op1);
      return 1;
    }
  bool reg_exec = gcn_vgpr_move_p (op0, op1);
  /* Scalar flat load.  */
  if (GET_CODE (op1) == MEM && MEM_ADDR_SPACE (op1) == ADDR_SPACE_SCALAR_FLAT)
    return 0;
  /* Scalar flat store.  */
  if (GET_CODE (op0) == MEM && MEM_ADDR_SPACE (op0) == ADDR_SPACE_SCALAR_FLAT)
    {
      sorry ("scalar flat stores are not supported yet");
      return 0;
    }

  rtx exec
    = vgpr_vector_mode_p (mode) ? gcn_full_exec () : gcn_scalar_exec ();
  if (reg_exec && !REG_P (exec))
    exec = get_exec (exec);

  /* Buffer load/stores for scratch memory segment.  */
  if ((GET_CODE (op0) == MEM
       && MEM_ADDR_SPACE (op0) == ADDR_SPACE_SCRATCH)
      || (GET_CODE (op1) == MEM
	  && MEM_ADDR_SPACE (op1) == ADDR_SPACE_SCRATCH))
    {
      emit_insn (gen_rtx_PARALLEL (VOIDmode,
				   gen_rtvec (3, gen_rtx_SET (op0, op1),
					      gen_rtx_USE (VOIDmode,
							   gen_rtx_REG
							   (TImode, 0)),
					      gen_rtx_USE (VOIDmode, exec))));
      return 1;
    }
  /* Stores to hard registers can be optimized because we know if there
     will be a need for exec or not.  */
  if (gcn_sgpr_register_operand (op0, mode)
      && (gcn_sgpr_register_operand (op1, mode)
	  || !register_operand (op1, mode)))
    return 0;

  /* LRA needs to have memory among the altenratives.  Arrange this by always
     expanidng buffer load/store pattern that also allow reg-reg moves.  */
  if (lra_in_progress && !vgpr_vector_mode_p (mode) && !reg_exec)
    {
      emit_insn (gen_rtx_PARALLEL (VOIDmode,
				   gen_rtvec (4, gen_rtx_SET (op0, op1),
					      gen_rtx_USE (VOIDmode,
							   gen_rtx_REG
							   (TImode, 0)),
					      gen_rtx_USE (VOIDmode, exec),
					      gen_rtx_CLOBBER (VOIDmode,
							       gen_reg_rtx
							       (DImode)))));
      return 1;
    }
  emit_insn (gen_mov_with_exec (op0, op1, exec));
  return 1;
}

/* Generate masked move.  */

rtx
gen_masked_mov (rtx op0, rtx op1, rtx exec)
{
  return (gen_rtx_SET (op0,
		       gen_rtx_VEC_MERGE (GET_MODE (op0),
					  op1, op0, exec)));
}

/* Generate masked move.  */

rtx
gen_masked_scalar_load (rtx op0, rtx op1, rtx op2, rtx exec)
{
  return (gen_rtx_SET (op0,
		       gen_rtx_VEC_MERGE (GET_MODE (op0),
					  gen_rtx_VEC_DUPLICATE (GET_MODE (op0),
								 op1),
					  op2, exec)));
}

/* Expand vector init of OP0 by VEC.  */

void
gcn_expand_vector_init (rtx op0, rtx vec)
{
  int64_t initialized_mask = 0;
  int64_t curr_mask = 1;
  machine_mode mode = GET_MODE (op0);

  rtx val = XVECEXP (vec, 0, 0);

  for (int i = 1; i < 64; i++)
    if (rtx_equal_p (val, XVECEXP (vec, 0, i)))
      curr_mask |= (int64_t)1 << i;

  if (gcn_constant_p (val))
    emit_insn (gen_mov_with_exec (op0, gcn_vec_constant (mode, val),
				  gcn_full_exec_reg ()));
  else
    {
      val = force_reg (GET_MODE_INNER (mode), val);
      emit_insn (gen_masked_scalar_load (op0, val, gcn_gen_undef (mode),
					 gcn_full_exec_reg ()));
    }
  initialized_mask |= curr_mask;
  for (int i = 1; i < 64; i++)
    if (!(initialized_mask & ((int64_t)1 << i)))
      {
	curr_mask = (int64_t)1 << i;
	rtx val = XVECEXP (vec, 0, i);

	for (int j = i + 1; j < 64; j++)
	  if (rtx_equal_p (val, XVECEXP (vec, 0, j)))
	    curr_mask |= (int64_t)1 << j;
	if (gcn_constant_p (val))
	  emit_insn (gen_masked_mov (op0, gcn_vec_constant (mode, val),
				     get_exec (curr_mask)));
	else
	  {
	    val = force_reg (GET_MODE_INNER (mode), val);
	    emit_insn (gen_masked_scalar_load (op0, val, op0,
					       get_exec (curr_mask)));
	  }
	initialized_mask |= curr_mask;
      }
}

/* Return true if move from OP0 to OP1 is known to be executed in vector
   unit.  */

bool
gcn_vgpr_move_p (rtx op0, rtx op1)
{
  if (MEM_P (op0) && MEM_ADDR_SPACE (op0) != ADDR_SPACE_SCALAR_FLAT)
    return true;
  if (MEM_P (op1) && MEM_ADDR_SPACE (op1) != ADDR_SPACE_SCALAR_FLAT)
    return true;
  return ((REG_P (op0) && VGPR_REGNO_P (REGNO (op0)))
	  || (REG_P (op1) && VGPR_REGNO_P (REGNO (op1)))
	  || vgpr_vector_mode_p (GET_MODE (op0)));
}

bool
gcn_sgpr_move_p (rtx op0, rtx op1)
{
  if (MEM_P (op0) && MEM_ADDR_SPACE (op0) == ADDR_SPACE_SCALAR_FLAT)
    return true;
  if (MEM_P (op1) && MEM_ADDR_SPACE (op1) == ADDR_SPACE_SCALAR_FLAT)
    return true;
  if (!REG_P (op0) || REGNO (op0) > FIRST_PSEUDO_REGISTER
      || VGPR_REGNO_P (REGNO (op0)))
    return false;
  if (REG_P (op1)
      && REGNO (op1) < FIRST_PSEUDO_REGISTER
      && !VGPR_REGNO_P (REGNO (op1)))
    return true;
  return immediate_operand (op1, VOIDmode) || memory_operand (op1, VOIDmode);
}

static void
output_file_start (void)
{
  fprintf (asm_out_file, "\t.hsatext\n");
  fprintf (asm_out_file, "\t.hsa_code_object_version 2,0\n");
  fprintf (asm_out_file, "\t.hsa_code_object_isa\n");  /* Autodetect.  */
  fprintf (asm_out_file, "\t.section\t.AMDGPU.config\n");
  fprintf (asm_out_file, "\t.hsatext\n");
}

/* This function produces the initial definition of a function name.  */

void
gcn_hsa_declare_function_name (FILE *file, const char *name, tree)
{
  int sgpr, vgpr;
  bool xnack_enabled = false;
  int extra_regs = 0;

  /* Determine count of sgpr/vgpr registers by looking for last
     one used.  */
  for (sgpr = 101; sgpr >= 0; sgpr--)
    if (df_regs_ever_live_p (FIRST_SGPR_REG + sgpr))
      break;
  sgpr++;
  for (vgpr = 255; vgpr >= 0; vgpr--)
    if (df_regs_ever_live_p (FIRST_VGPR_REG + vgpr))
      break;
  vgpr++;

  if (xnack_enabled)
    extra_regs = 6;
  if (df_regs_ever_live_p (FLAT_SCRATCH_LO_REG)
      || df_regs_ever_live_p (FLAT_SCRATCH_HI_REG))
    extra_regs = 4;
  else if (df_regs_ever_live_p (VCC_LO_REG) || df_regs_ever_live_p (VCC_HI_REG))
    extra_regs = 2;

  fprintf (file, "\t.type\t%s,@function\n", name);
  fprintf (file, "\t.amdgpu_hsa_kernel\t%s\n", name);
  fprintf (file, "%s:\n", name);
  fprintf (file, "\t.amd_kernel_code_t\n"
		 "\t\tkernel_code_version_major = 1\n"
		 "\t\tkernel_code_version_minor = 0\n"
		 "\t\tmachine_kind = 1\n"
		 "\t\tmachine_version_major = 8\n"
		 "\t\tmachine_version_minor = 0\n"
		 "\t\tmachine_version_stepping = 1\n"
		 "\t\tkernel_code_entry_byte_offset = 256\n"
		 "\t\tkernel_code_prefetch_byte_size = 0\n"
		 "\t\tmax_scratch_backing_memory_byte_size = 0\n"
		 "\t\tcompute_pgm_rsrc1_vgprs = %i\n"
		 "\t\tcompute_pgm_rsrc1_sgprs = %i\n"
		 "\t\tcompute_pgm_rsrc1_priority = 0\n"
		 "\t\tcompute_pgm_rsrc1_float_mode = 192\n"
		 "\t\tcompute_pgm_rsrc1_priv = 0\n"
		 "\t\tcompute_pgm_rsrc1_dx10_clamp = 1\n"
		 "\t\tcompute_pgm_rsrc1_debug_mode = 0\n"
		 "\t\tcompute_pgm_rsrc1_ieee_mode = 1\n"
		 /* We enable scratch memory.  */
		 "\t\tcompute_pgm_rsrc2_scratch_en = 1\n"
		 "\t\tcompute_pgm_rsrc2_user_sgpr = %i\n"
		 "\t\tcompute_pgm_rsrc2_tgid_x_en = 1\n"
		 "\t\tcompute_pgm_rsrc2_tgid_y_en = 0\n"
		 "\t\tcompute_pgm_rsrc2_tgid_z_en = 0\n"
		 "\t\tcompute_pgm_rsrc2_tg_size_en = 0\n"
		 "\t\tcompute_pgm_rsrc2_tidig_comp_cnt = 0\n"
		 "\t\tcompute_pgm_rsrc2_excp_en_msb = 0\n"
		 "\t\tcompute_pgm_rsrc2_lds_size = 8\n" /*FIXME */
		 "\t\tcompute_pgm_rsrc2_excp_en = 0\n",
	   (vgpr - 1) / 4,
	   /* Must match wavefront_sgpr_count */
	   (sgpr + extra_regs - 1) / 8,
	   /* The total number of SGPR user data registers requested.  This
 	      number must match the number of user data registers enabled.  */
	   cfun->machine->args.nsgprs - 2);
   for (int a = 0; a < GCN_KERNEL_ARG_TYPES; a++)
     if (gcn_kernel_arg_types[a].header_pseudo)
       fprintf (file, "\t\t%s = %i\n",
		gcn_kernel_arg_types[a].header_pseudo,
		(cfun->machine->args.requested & (1<<a)) != 0);
   /*fprintf (file, "\t\tenable_vgpr_workitem_id = %i\n",
	    (cfun->machine->args.requested & (1 << WORK_ITEM_ID_Z_ARG))
	    ? 2
	    : cfun->machine->args.requested & (1 << WORK_ITEM_ID_Y_ARG)
	    ? 1 : 0);*/
   fprintf (file, "\t\tenable_ordered_append_gds = 0\n"
		  "\t\tprivate_element_size = 1\n"
		  "\t\tis_ptr64 = 1\n"
		  "\t\tis_dynamic_callstack = 0\n"
		  "\t\tis_debug_enabled = 0\n"
		  "\t\tis_xnack_enabled = %i\n"
		  "\t\tworkitem_private_segment_byte_size = %i\n"
		  "\t\tworkgroup_group_segment_byte_size = 0\n"
		  "\t\tgds_segment_byte_size = 0\n"
		  "\t\tkernarg_segment_byte_size = %i\n"
		  "\t\tworkgroup_fbarrier_count = 0\n"
		  "\t\twavefront_sgpr_count = %i\n"
		  "\t\tworkitem_vgpr_count = %i\n"
		  "\t\treserved_vgpr_first = 0\n"
		  "\t\treserved_vgpr_count = 0\n"
		  "\t\treserved_sgpr_first = 0\n"
		  "\t\treserved_sgpr_count = 0\n"
		  "\t\tdebug_wavefront_private_segment_offset_sgpr = 0\n"
		  "\t\tdebug_private_segment_buffer_sgpr = 0\n"
		  "\t\tkernarg_segment_alignment = %i\n"
		  "\t\tgroup_segment_alignment = 4\n"
		  "\t\tprivate_segment_alignment = %i\n"
		  "\t\twavefront_size = 6\n"
		  "\t\tcall_convention = 0\n"
		  "\t\truntime_loader_kernel_symbol = 0\n"
		  "\t.end_amd_kernel_code_t\n",
		  xnack_enabled,
		  (int)get_frame_size (),
		  cfun->machine->kernarg_segment_byte_size,
		/* Number of scalar registers used by a wavefront.  This
		   includes the special SGPRs for VCC, Flat Scratch (Base,
		   Size) and XNACK (for GFX8 (VI)+).  It does not include the
		   16 SGPR added if a trap handler is enabled.  Must match
		   compute_pgm_rsrc1.sgprs.  */
		  sgpr + extra_regs, vgpr,
		  cfun->machine->kernarg_segment_alignment,
		  crtl->stack_alignment_needed / 8);
}

/* Generate prologue.  */

void
gcn_expand_prologue ()
{
  /* Flat access to LDS requires apperture setup and M0 register setup.  */
  emit_insn (gen_rtx_SET (gen_rtx_REG (SImode, M0_REG),
			  gen_int_mode (65536, SImode)));
  /* Flat access also need flat_scratch register initialized.  */
  if (cfun->machine->args.reg[FLAT_SCRATCH_INIT_ARG] > 0)
    {
      rtx arg = gen_rtx_REG (DImode,
			     cfun->machine->args.reg[FLAT_SCRATCH_INIT_ARG]);
      emit_insn (gen_lshrsi3_scalar (gcn_operand_part
				     (DImode, gen_rtx_REG (DImode,
							   FLAT_SCRATCH_REG),
				      1),
				     gcn_operand_part (DImode, arg, 0),
				     gen_int_mode (8, SImode)));
      emit_insn (gen_rtx_SET (gcn_operand_part (DImode,
						gen_rtx_REG (DImode,
							     FLAT_SCRATCH_REG),
						0),
			      gcn_operand_part (DImode, arg, 1)));
    }
  if (df_regs_ever_live_p (HARD_FRAME_POINTER_REGNUM))
    emit_insn (gen_rtx_SET (gen_rtx_REG (SImode, HARD_FRAME_POINTER_REGNUM),
			    gen_rtx_REG (SImode,
					 cfun->machine->args.reg
					 [PRIVATE_SEGMENT_WAVE_OFFSET_ARG])));
  emit_insn (gen_prologue_use (gen_rtx_REG (SImode, M0_REG)));
}

/* Compute a (partial) cost for rtx X.  Return true if the complete
   cost has been computed, and false if subexpressions should be
   scanned.  In either case, *TOTAL contains the cost result.  */

static bool
gcn_rtx_costs (rtx x, machine_mode, int, int, int *total, bool)
{
  enum rtx_code code = GET_CODE (x);
  switch (code)
    {
    case CONST:
    case CONST_DOUBLE:
    case CONST_VECTOR:
    case CONST_INT:
      if (gcn_inline_constant_p (x))
	*total = 0;
      if (code == CONST_INT
	  && ((unsigned HOST_WIDE_INT) INTVAL (x) + 0x8000) < 0x10000)
	*total = 1;
      else if (gcn_constant_p (x))
	*total = 2;
      else
	*total = vgpr_vector_mode_p (GET_MODE (x)) ? 64 : 4;
      return true;

    default:
      *total = 3;
      return false;
    }
}

/* Return the cost of moving data of mode M between a
   register and memory.  A value of 2 is the default; this cost is
   relative to those in `REGISTER_MOVE_COST'.

   This function is used extensively by register_move_cost that is used to
   build tables at startup.  Make it inline in this case.
   When IN is 2, return maximum of in and out move cost.

   If moving between registers and memory is more expensive than
   between two registers, you should define this macro to express the
   relative cost.

   Model also increased moving costs of QImode registers in non
   Q_REGS classes.
 */

#define LOAD_COST  32
#define STORE_COST 32
static int
gcn_memory_move_cost (machine_mode mode, reg_class_t regclass, bool in)
{
  int nregs = CEIL (GET_MODE_SIZE (mode), 4);
  switch (regclass)
    {
    case SCC_CONDITIONAL_REG:
    case VCCZ_CONDITIONAL_REG:
    case VCC_CONDITIONAL_REG:
    case EXECZ_CONDITIONAL_REG:
    case ALL_CONDITIONAL_REGS:
    case SGPR_REGS:
    case SGPR_EXEC_REGS:
    case EXEC_MASK_REG:
    case SGPR_VOP3A_SRC_REGS:
    case SGPR_MEM_SRC_REGS:
    case SGPR_SRC_REGS:
    case SGPR_DST_REGS:
    case GENERAL_REGS:
      if (!in)
	return (STORE_COST+2) * nregs;
      return LOAD_COST * nregs;
    case VGPR_REGS:
      if (in)
	return (LOAD_COST+2) * nregs;
      return STORE_COST * nregs;
    case ALL_REGS:
    case SRCDST_REGS:
      if (in)
	return (LOAD_COST+2) * nregs;
      return (STORE_COST + 2) * nregs;
    default:
      gcc_unreachable ();
    }
}

/* Return the cost of moving data from a register in class CLASS1 to
   one in class CLASS2.  Base value is 2.  */
static int
gcn_register_move_cost (machine_mode, reg_class_t dst,
			reg_class_t src)
{
  /* Increase cost of moving from and to vector registers.  While this is
     fast in hardware (I think), it has hidden cost of setting up the exec
     flags.  */
  if ((src < VGPR_REGS) != (dst < VGPR_REGS))
    return 4;
  return 2;
}

/* Return class of registers which could be used for pseudo of MODE
   and of class RCLASS for spilling instead of memory.  Return NO_REGS
   if it is not possible or non-profitable.  */

static reg_class_t
gcn_spill_class (reg_class_t /*c*/, machine_mode /*mode*/)
{
  return SGPR_REGS;
/*
  return NO_REGS;
  if (mode == DImode)
    return SGPR_REGS;
  return c;*/
  /*return ALL_REGS;*/
}

/* Change allocno class for given pseudo from allocno and best class calculated
   by IRA.

   Be sure we do not try to spill into memory, since this is not supported
   (yet).  */

static reg_class_t
gcn_ira_change_pseudo_allocno_class (int, reg_class_t cl, reg_class_t)
{
  /*if (cl == NO_REGS)
    return VGPR_REGS;*/
  return cl;
}

/* Target hook for scalar_mode_supported_p.  */

static bool
gcn_scalar_mode_supported_p (machine_mode mode)
{
  return default_scalar_mode_supported_p (mode);
}

/* Implements target hook vector_mode_supported_p.  */

static bool
gcn_vector_mode_supported_p (machine_mode mode)
{
  return mode == V64SImode || mode == V64DImode || mode == V64SFmode;
}

/* Initialize machine_function.  */

static struct machine_function *
gcn_init_machine_status (void)
{
  struct machine_function *f;

  f = ggc_cleared_alloc<machine_function> ();
  f->exec_reg = NULL_RTX;
  return f;
}

/* Implement the TARGET_OPTION_OVERRIDE hook.  */

static void
gcn_option_override (void)
{
  init_machine_status = gcn_init_machine_status;
}

/* Return pseudo holding default exec register.  */

rtx
gcn_default_exec ()
{
  if (cfun->machine->exec_reg)
    {
      /* During RTX init we are called with no function CFG built.  */
      if (!cfun->machine->exec_reg_init
	  && cfun->cfg && 0)
	{
	  cfun->machine->exec_reg_init = true;
	}
      return cfun->machine->exec_reg;
    }
  else
    {
      cfun->machine->exec_reg = gen_reg_rtx (DImode);
      if (cfun->cfg && 0)
	{
	  emit_insn_before (gen_rtx_SET (cfun->machine->exec_reg,
			       gen_rtx_REG (DImode, EXEC_REG)),
			    NEXT_INSN (entry_of_function ()));
	  cfun->machine->exec_reg_init = true;
	}
    }
  return cfun->machine->exec_reg;
}

/* Return value of scalar exec register.  */

rtx
gcn_scalar_exec ()
{
  return const1_rtx;
}

/* Return value of full exec register.  */

rtx
gcn_full_exec ()
{
  return constm1_rtx;
}

/* Return pseudo holding full exec register.  */

rtx
gcn_full_exec_reg ()
{
  return get_exec (-1);
}

/* Set live registers passed to the kernel.  */

static void
gcn_live_on_entry (bitmap regs)
{
  /* This register holds the default exec mask.  */
  bitmap_set_bit (regs, EXEC_LO_REG);
  bitmap_set_bit (regs, EXEC_HI_REG);
  /* This register holds vector containing value N at N-th lane
     for every lane where exec mask is set.  */
  bitmap_set_bit (regs, FIRST_VGPR_REG);
}

/* Table of valid machine attributes.  */
static const struct attribute_spec gcn_attribute_table[] =
{
  /* { name, min_len, max_len, decl_req, type_req, fn_type_req, handler,
       affects_type_identity } */
  { "amdgpu_hsa_kernel",   0, GCN_KERNEL_ARG_TYPES, false, true,  true,
    gcn_handle_amdgpu_hsa_kernel_attribute, true },
  /* End element.  */
  { NULL,	0, 0, false, false, false, NULL, false }
};

/* When this hook returns true for MODE, the compiler allows
   registers explicitly used in the rtl to be used as spill registers
   but prevents the compiler from extending the lifetime of these
   registers.  */

bool
gcn_small_register_classes_for_mode_p (machine_mode mode)
{
  /* We allocate into exec and vcc regs.  Those make small register class.  */
  return mode == DImode || mode == SImode;
}

/* Vector registers are wide and we can not directly subreg into word
   sized parts.  */

int
gcn_regmode_natural_size (enum machine_mode mode)
{
  if (vgpr_vector_mode_p (mode))
    return GET_MODE_SIZE (mode);
  return 4;
}

enum gcn_builtin_type_index
{
  GCN_BTI_END_OF_PARAMS,

  GCN_BTI_VOID,
  GCN_BTI_EXEC,

  GCN_BTI_V64SI,
  GCN_BTI_V64SF,
  GCN_BTI_V64PTR,
  GCN_BTI_SIPTR,
  GCN_BTI_SFPTR,

  GCN_BTI_MAX
};

static GTY(()) tree gcn_builtin_types[GCN_BTI_MAX];

#define exec_type_node (gcn_builtin_types[GCN_BTI_EXEC])
#define v64si_type_node (gcn_builtin_types[GCN_BTI_V64SI])
#define v64sf_type_node (gcn_builtin_types[GCN_BTI_V64SF])
#define v64ptr_type_node (gcn_builtin_types[GCN_BTI_V64PTR])
#define siptr_type_node (gcn_builtin_types[GCN_BTI_SIPTR])
#define sfptr_type_node (gcn_builtin_types[GCN_BTI_SFPTR])

static rtx gcn_expand_builtin_1 (tree, rtx, rtx, machine_mode, int,
				 struct gcn_builtin_description *);
static rtx gcn_expand_builtin_binop (tree, rtx, rtx, machine_mode, int,
				     struct gcn_builtin_description *);

struct gcn_builtin_description;
typedef rtx (*gcn_builtin_expander) (tree, rtx, rtx, machine_mode, int,
				     struct gcn_builtin_description *);

enum gcn_builtin_type
{
  B_UNIMPLEMENTED,		/* Sorry out */
  B_INSN,			/* Emit a pattern */
  B_OVERLOAD,			/* Placeholder for an overloaded function */
};

struct gcn_builtin_description
{
  int fcode;
  int icode;
  const char *name;
  enum gcn_builtin_type type;
  /* The first element of parm is always the return type.  The rest
     are a zero terminated list of parameters.  */
  int parm[6];
  gcn_builtin_expander expander;
};


/* Codes for all the GCN builtins.  */

enum gcn_builtin_codes
{
#define DEF_BUILTIN(fcode, icode, name, type, params, expander) \
  GCN_BUILTIN_ ## fcode,
#define DEF_BUILTIN_BINOP_INT_FP(fcode, ic, name)	\
  GCN_BUILTIN_ ## fcode ## _V64SI,			\
  GCN_BUILTIN_ ## fcode ## _V64SI_unspec,
#include "gcn-builtins.def"
#undef DEF_BUILTIN
#undef DEF_BUILTIN_BINOP_INT_FP
   GCN_BUILTIN_MAX
};

extern GTY(()) struct gcn_builtin_description gcn_builtins[GCN_BUILTIN_MAX];

struct gcn_builtin_description gcn_builtins[] = {
#define DEF_BUILTIN(fcode, icode, name, type, params, expander)	\
  {GCN_BUILTIN_ ## fcode, icode, name, type, params, expander},

#define DEF_BUILTIN_BINOP_INT_FP(fcode, ic, name)			\
  {GCN_BUILTIN_ ## fcode ## _V64SI,					\
   CODE_FOR_ ## ic ##v64si3_vector, name "_v64int", B_INSN,		\
   {GCN_BTI_V64SI, GCN_BTI_EXEC, GCN_BTI_V64SI, GCN_BTI_V64SI,		\
    GCN_BTI_V64SI, GCN_BTI_END_OF_PARAMS}, gcn_expand_builtin_binop},	\
  {GCN_BUILTIN_ ## fcode ## _V64SI_unspec,				\
   CODE_FOR_ ## ic ##v64si3_vector, name "_v64int_unspec", B_INSN, 	\
   {GCN_BTI_V64SI, GCN_BTI_EXEC, GCN_BTI_V64SI, GCN_BTI_V64SI,		\
    GCN_BTI_END_OF_PARAMS}, gcn_expand_builtin_binop},

#include "gcn-builtins.def"
#undef DEF_BUILTIN_BINOP_INT_FP
#undef DEF_BUILTIN
};

static GTY(()) tree gcn_builtin_decls[GCN_BUILTIN_MAX];

/* Return the GCN builtin for CODE.  */

static tree
gcn_builtin_decl (unsigned code, bool ARG_UNUSED (initialize_p))
{
  if (code >= GCN_BUILTIN_MAX)
    return error_mark_node;

  return gcn_builtin_decls[code];
}

static void
gcn_init_builtin_types (void)
{
  gcn_builtin_types[GCN_BTI_VOID] = void_type_node;
  exec_type_node =  unsigned_intDI_type_node;
  v64si_type_node = build_vector_type (intSI_type_node, 64);
  v64sf_type_node = build_vector_type (float_type_node, 64);
  v64ptr_type_node
    = build_vector_type (unsigned_intDI_type_node
			 /*build_pointer_type (integer_type_node)*/, 64);
  tree tmp = build_distinct_type_copy (intSI_type_node);
  TYPE_ADDR_SPACE (tmp) = ADDR_SPACE_FLAT;
  siptr_type_node = build_pointer_type (tmp);

  tmp = build_distinct_type_copy (float_type_node);
  TYPE_ADDR_SPACE (tmp) = ADDR_SPACE_FLAT;
  sfptr_type_node = build_pointer_type (tmp);
}

/* Set up all builtin functions for this target.  */

static void
gcn_init_builtins (void)
{
  gcn_init_builtin_types ();

  struct gcn_builtin_description *d;
  unsigned int i;
  for (i = 0, d = gcn_builtins; i < GCN_BUILTIN_MAX; i++, d++)
    {
      tree p;
      char name[64];		/* build_function will make a copy.  */
      int parm;

      /* FIXME: Is this necessary/useful? */
      if (d->name == 0)
	continue;

      /* Find last parm.  */
      for (parm = 1; d->parm[parm] != GCN_BTI_END_OF_PARAMS; parm++)
	;

      p = void_list_node;
      while (parm > 1)
	p = tree_cons (NULL_TREE, gcn_builtin_types[d->parm[--parm]], p);

      p = build_function_type (gcn_builtin_types[d->parm[0]], p);

      sprintf (name, "__builtin_gcn_%s", d->name);
      gcn_builtin_decls[i]
	= add_builtin_function (name, p, i, BUILT_IN_MD, NULL, NULL_TREE);

      /* These builtins don't throw.  */
      TREE_NOTHROW (gcn_builtin_decls[i]) = 1;
    }
}

static rtx
gcn_expand_builtin_1 (tree exp, rtx target, rtx /*subtarget*/,
		      machine_mode /*mode*/, int ignore,
		      struct gcn_builtin_description *)
{
  tree fndecl = TREE_OPERAND (CALL_EXPR_FN (exp), 0);
  switch (DECL_FUNCTION_CODE (fndecl))
    {
    case GCN_BUILTIN_FLAT_LOAD_INT32:
      {
	if (ignore)
	  return target;
	/*rtx exec = */
	  force_reg (DImode,
		     expand_expr (CALL_EXPR_ARG (exp, 0), NULL_RTX, DImode,
				  EXPAND_NORMAL));
	/*rtx ptr =*/
	  force_reg (V64DImode,
		     expand_expr (CALL_EXPR_ARG (exp, 1), NULL_RTX, V64DImode,
				  EXPAND_NORMAL));
	/*emit_insn (gen_vector_flat_loadv64si
		   (target, gcn_gen_undef (V64SImode), ptr, exec));*/
	return target;
      }
    case GCN_BUILTIN_FLAT_LOAD_PTR_INT32:
    case GCN_BUILTIN_FLAT_LOAD_PTR_FLOAT:
      {
	if (ignore)
	  return target;
	rtx exec
	  = force_reg (DImode,
		       expand_expr (CALL_EXPR_ARG (exp, 0), NULL_RTX, DImode,
				    EXPAND_NORMAL));
	rtx ptr
	  = force_reg (DImode,
		       expand_expr (CALL_EXPR_ARG (exp, 1), NULL_RTX, V64DImode,
				    EXPAND_NORMAL));
	rtx offsets = force_reg (V64SImode,
		     expand_expr (CALL_EXPR_ARG (exp, 2), NULL_RTX, V64DImode,
				  EXPAND_NORMAL));
	rtx addrs = gen_reg_rtx (V64DImode);
	rtx tmp = gen_reg_rtx (V64SImode);
	emit_insn (gen_ashlv64si3_vector (tmp, offsets,
					  gcn_vec_constant (V64SImode, 2),
					  exec, gcn_gen_undef (V64SImode)));
	emit_insn (gen_addv64di3_zext_dup2 (addrs, tmp, ptr, exec,
				       gcn_gen_undef (V64DImode)));
	rtx mem = gen_rtx_MEM (GET_MODE (target), addrs);
	set_mem_addr_space (mem, ADDR_SPACE_FLAT);
	/* FIXME: set attributes.  */
	emit_insn (gen_mov_with_exec (target, mem, exec));
	return target;
      }
    case GCN_BUILTIN_FLAT_STORE_PTR_INT32:
    case GCN_BUILTIN_FLAT_STORE_PTR_FLOAT:
      {
	rtx exec
	  = force_reg (DImode,
		       expand_expr (CALL_EXPR_ARG (exp, 0), NULL_RTX, DImode,
				    EXPAND_NORMAL));
	rtx ptr
	  = force_reg (DImode,
		       expand_expr (CALL_EXPR_ARG (exp, 1), NULL_RTX, V64DImode,
				    EXPAND_NORMAL));
	rtx offsets = force_reg (V64SImode,
		     expand_expr (CALL_EXPR_ARG (exp, 2), NULL_RTX, V64DImode,
				  EXPAND_NORMAL));
	enum machine_mode vmode = TYPE_MODE (TREE_TYPE (CALL_EXPR_ARG (exp,
								       3)));
	rtx val = force_reg (vmode,
		     expand_expr (CALL_EXPR_ARG (exp, 3), NULL_RTX, vmode,
				  EXPAND_NORMAL));
	rtx addrs = gen_reg_rtx (V64DImode);
	rtx tmp = gen_reg_rtx (V64SImode);
	emit_insn (gen_ashlv64si3_vector (tmp, offsets,
					  gcn_vec_constant (V64SImode, 2),
					  exec, gcn_gen_undef (V64SImode)));
	emit_insn (gen_addv64di3_zext_dup2 (addrs, tmp, ptr, exec,
					    gcn_gen_undef (V64DImode)));
	rtx mem = gen_rtx_MEM (vmode, addrs);
	set_mem_addr_space (mem, ADDR_SPACE_FLAT);
	/* FIXME: set attributes.  */
	emit_insn (gen_mov_with_exec (mem, val, exec));
	return target;
      }
    default:
      gcc_unreachable ();
    }
}

/* Expansion of simple arithmetic and bit binary operation bultins.  */

static rtx
gcn_expand_builtin_binop (tree exp, rtx target, rtx /*subtarget*/,
			  machine_mode /*mode*/, int ignore,
			  struct gcn_builtin_description *d)
{
  tree fndecl = TREE_OPERAND (CALL_EXPR_FN (exp), 0);
  int icode = d->icode;
  if (ignore)
    return target;

  rtx exec = force_reg (DImode,
			expand_expr (CALL_EXPR_ARG (exp, 0), NULL_RTX, DImode,
				     EXPAND_NORMAL));

  machine_mode m1 = insn_data[icode].operand[1].mode;
  rtx arg1 = expand_expr (CALL_EXPR_ARG (exp, 1), NULL_RTX, m1,
			  EXPAND_NORMAL);
  if (!insn_data[icode].operand[1].predicate (arg1, m1))
    arg1 = force_reg (m1, arg1);

  machine_mode m2 = insn_data[icode].operand[2].mode;
  rtx arg2 = expand_expr (CALL_EXPR_ARG (exp, 2), NULL_RTX, m2,
			  EXPAND_NORMAL);
  if (!insn_data[icode].operand[2].predicate (arg2, m2))
    arg2 = force_reg (m2, arg2);

  rtx arg_prev;
  if (call_expr_nargs (exp) == 4)
    {
      machine_mode m_prev = insn_data[icode].operand[4].mode;
      arg_prev = force_reg (m_prev,
			    expand_expr (CALL_EXPR_ARG (exp, 3), NULL_RTX,
					 m_prev, EXPAND_NORMAL));
    }
  else
    arg_prev = gcn_gen_undef (GET_MODE (target));

  rtx pat = GEN_FCN (icode) (target, arg1, arg2, exec, arg_prev);
  emit_insn (pat);
  return target;
}

/* Expand an expression EXP that calls a buVilt-in function,
   with result going to TARGET if that's convenient
   (and in mode MODE if that's convenient).
   SUBTARGET may be used as the target for computing one of EXP's operands.
   IGNORE is nonzero if the value is to be ignored.  */
rtx
gcn_expand_builtin (tree exp,
		    rtx target,
		    rtx subtarget,
		    machine_mode mode,
		    int ignore)
{
  tree fndecl = TREE_OPERAND (CALL_EXPR_FN (exp), 0);
  unsigned int fcode = DECL_FUNCTION_CODE (fndecl);
  struct gcn_builtin_description *d;

  gcc_assert (fcode < GCN_BUILTIN_MAX);
  d = &gcn_builtins[fcode];

  if (d->type == B_UNIMPLEMENTED)
    sorry ("Builtin not implemented");

  return d->expander (exp, target, subtarget, mode, ignore, d);
}


/* Implement TARGET_CLASS_LIKELY_SPILLED_P.  */

static bool
gcn_class_likely_spilled_p (reg_class_t rclass)
{
  return rclass == EXEC_MASK_REG;
}

/* Implement TARGET_PREFERRED_RELOAD_CLASS.

   Put float CONST_DOUBLE in the constant pool instead of fp regs.
   QImode must go into class Q_REGS.
   Narrow ALL_REGS to GENERAL_REGS.  This supports allowing movsf and
   movdf to do mem-to-mem moves through integer regs.  */

static reg_class_t
gcn_preferred_reload_class (rtx x, reg_class_t regclass)
{
  machine_mode mode = GET_MODE (x);

  /* We're only allowed to return a subclass of CLASS.  Many of the
     following checks fail for NO_REGS, so eliminate that early.  */
  if (regclass == NO_REGS)
    return NO_REGS;

  /* All classes can load zeros.  */
  if (x == CONST0_RTX (mode))
    return regclass;

  if (regclass == EXEC_MASK_REG)
    return SGPR_EXEC_REGS;

  return regclass;
}

unsigned
gcn_frame_pointer_regnum ()
{
  if (cfun && cfun->machine)
    return cfun->machine->args.reg[PRIVATE_SEGMENT_WAVE_OFFSET_ARG];
  return 0;
}

bool
gcn_can_eliminate_p (int /*from_reg*/, int to_reg)
{
  return ((unsigned) to_reg == HARD_FRAME_POINTER_REGNUM);
}

/* Only vector registers can be read/written, be sure to tell IRA about this.
   */

static reg_class_t
gcn_secondary_reload (bool, rtx, reg_class_t rclass,
		      machine_mode, secondary_reload_info *)
{
  if (rclass == VGPR_REGS)
    return NO_REGS;
  return VGPR_REGS;
}

/* Update register usage after having seen the compiler flags.  */

static void
gcn_conditional_register_usage (void)
{
  int i;
  for (i=0;i<16;i++)
    fixed_regs[i] = !cfun || !cfun->machine ? 1 : 0;
  if (!cfun || !cfun->machine)
    return;
  if (cfun->machine->args.reg[PRIVATE_SEGMENT_WAVE_OFFSET_ARG] >= 0)
    fixed_regs[cfun->machine->args.reg[PRIVATE_SEGMENT_WAVE_OFFSET_ARG]] = 1;
  if (cfun->machine->args.reg[PRIVATE_SEGMENT_BUFFER_ARG] >= 0)
    {
      fixed_regs[cfun->machine->args.reg[PRIVATE_SEGMENT_BUFFER_ARG]] = 1;
      fixed_regs[cfun->machine->args.reg[PRIVATE_SEGMENT_BUFFER_ARG] + 1] = 1;
      fixed_regs[cfun->machine->args.reg[PRIVATE_SEGMENT_BUFFER_ARG] + 2] = 1;
      fixed_regs[cfun->machine->args.reg[PRIVATE_SEGMENT_BUFFER_ARG] + 3] = 1;
    }
}
/*  TARGET overrides.  */
#undef TARGET_FUNCTION_VALUE
#define TARGET_FUNCTION_VALUE gcn_function_value
#undef TARGET_STATIC_CHAIN
#define TARGET_STATIC_CHAIN gcn_static_chain
#undef TARGET_FUNCTION_VALUE_REGNO_P
#define TARGET_FUNCTION_VALUE_REGNO_P gcn_function_value_regno_p
#undef TARGET_FUNCTION_ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE gcn_function_arg_advance
#undef TARGET_FUNCTION_ARG
#define TARGET_FUNCTION_ARG gcn_function_arg
#undef TARGET_ASM_FILE_START
#define TARGET_ASM_FILE_START output_file_start
#undef TARGET_LRA_P
#define TARGET_LRA_P hook_bool_void_true
#undef TARGET_REGISTER_MOVE_COST
#define TARGET_REGISTER_MOVE_COST gcn_register_move_cost
#undef TARGET_SMALL_REGISTER_CLASSES_FOR_MODE_P
#define TARGET_SMALL_REGISTER_CLASSES_FOR_MODE_P \
  gcn_small_register_classes_for_mode_p
#undef TARGET_RTX_COSTS
#define TARGET_RTX_COSTS gcn_rtx_costs
#undef TARGET_MEMORY_MOVE_COST
#define TARGET_MEMORY_MOVE_COST gcn_memory_move_cost
#undef TARGET_SPILL_CLASS
#define TARGET_SPILL_CLASS gcn_spill_class
#undef TARGET_IRA_CHANGE_PSEUDO_ALLOCNO_CLASS
#define TARGET_IRA_CHANGE_PSEUDO_ALLOCNO_CLASS \
  gcn_ira_change_pseudo_allocno_class
#undef TARGET_SCALAR_MODE_SUPPORTED_P
#define TARGET_SCALAR_MODE_SUPPORTED_P gcn_scalar_mode_supported_p
#undef TARGET_VECTOR_MODE_SUPPORTED_P
#define TARGET_VECTOR_MODE_SUPPORTED_P gcn_vector_mode_supported_p
#undef TARGET_CLASS_MAX_NREGS
#define TARGET_CLASS_MAX_NREGS gcn_class_max_nregs
#undef TARGET_OPTION_OVERRIDE
#define TARGET_OPTION_OVERRIDE gcn_option_override
#undef TARGET_EXTRA_LIVE_ON_ENTRY
#define TARGET_EXTRA_LIVE_ON_ENTRY gcn_live_on_entry

#undef TARGET_ATTRIBUTE_TABLE
#define TARGET_ATTRIBUTE_TABLE gcn_attribute_table

#undef TARGET_ADDR_SPACE_POINTER_MODE
#define TARGET_ADDR_SPACE_POINTER_MODE gcn_addr_space_pointer_mode

#undef TARGET_ADDR_SPACE_ADDRESS_MODE
#define TARGET_ADDR_SPACE_ADDRESS_MODE gcn_addr_space_address_mode

#undef TARGET_ADDR_SPACE_LEGITIMATE_ADDRESS_P
#define TARGET_ADDR_SPACE_LEGITIMATE_ADDRESS_P \
  gcn_addr_space_legitimate_address_p

#undef TARGET_ADDR_SPACE_LEGITIMIZE_ADDRESS
#define TARGET_ADDR_SPACE_LEGITIMIZE_ADDRESS gcn_addr_space_legitimize_address

#undef TARGET_ADDR_SPACE_SUBSET_P
#define TARGET_ADDR_SPACE_SUBSET_P gcn_addr_space_subset_p

#undef TARGET_ADDR_SPACE_CONVERT
#define TARGET_ADDR_SPACE_CONVERT gcn_addr_space_convert

#undef TARGET_INIT_BUILTINS
#define TARGET_INIT_BUILTINS gcn_init_builtins
#undef TARGET_EXPAND_BUILTIN
#define TARGET_EXPAND_BUILTIN gcn_expand_builtin
#undef  TARGET_BUILTIN_DECL
#define TARGET_BUILTIN_DECL gcn_builtin_decl
/*#undef TARGET_CLASS_LIKELY_SPILLED_P
#define TARGET_CLASS_LIKELY_SPILLED_P gcn_class_likely_spilled_p
#undef TARGET_PREFERRED_RELOAD_CLASS
#define TARGET_PREFERRED_RELOAD_CLASS gcn_preferred_reload_class
#undef TARGET_CAN_ELIMINATE
#define TARGET_CAN_ELIMINATE gcn_can_eliminate_p
#undef TARGET_SECONDARY_RELOAD
#define TARGET_SECONDARY_RELOAD gcn_secondary_reload*/
#undef TARGET_CONDITIONAL_REGISTER_USAGE
#define TARGET_CONDITIONAL_REGISTER_USAGE gcn_conditional_register_usage

struct gcc_target targetm = TARGET_INITIALIZER;

#include "gt-gcn.h"
