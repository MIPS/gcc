/* Compact EH unwinding support for MIPS.
   Copyright (C) 2012-2015 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#ifdef MD_HAVE_COMPACT_EH

#define DWARF_SP_REGNO 29

#if defined(_MIPS_SIM) && _MIPS_SIM == _ABIO32
#define MIPS_EH_STACK_ALIGN 8
#else
#define MIPS_EH_STACK_ALIGN 16
#endif

#if defined(_MIPS_SIM) && _MIPS_SIM == _ABIP32
#define VRF_0 -16  /* assume 16-23 are only ones accessed.  */
#else
#define VRF_0 32
#endif

/* Record the push of a register in FrameState FS.  */

static int
record_push (_Unwind_FrameState *fs, int reg, int offset)
{
  int idx = DWARF_REG_TO_UNWIND_COLUMN (reg);

  offset -= dwarf_reg_size_table[idx];
  fs->regs.reg[idx].how = REG_SAVED_OFFSET;
  fs->regs.reg[idx].loc.offset = offset;
  return offset;
}

/* Adjust the cfa_offset by value VAL.  Record the new cfa_offset in
   FrameState FS.  */

static void
record_cfa_adjustment (_Unwind_FrameState *fs, _uleb128_t val)
{
  int i;
  fs->regs.cfa_offset += val;
  /* In case we see an adjustment after pushes, it means that
     the registers aren't saved at the top of the frame, probably
     due to a varargs save area.  Adjust the recorded offsets.  */
  for (i = 0; i < DWARF_FRAME_REGISTERS + 1; i++)
    if (fs->regs.reg[i].how == REG_SAVED_OFFSET)
      fs->regs.reg[i].loc.offset -= val;
}

/* Process the frame unwinding opcodes.  */

static _Unwind_Reason_Code
md_unwind_compact_oabi_nabi (struct _Unwind_Context *context ATTRIBUTE_UNUSED,
		   _Unwind_FrameState *fs, const unsigned char **pp)
{
  unsigned char op;
  _uleb128_t val;
  int push_offset;
  int i;
  int n;
  const unsigned char *p = *pp;

  push_offset = 0;
  fs->regs.cfa_how = CFA_REG_OFFSET;
  fs->regs.cfa_reg = STACK_POINTER_REGNUM;
  fs->regs.cfa_offset = 0;
  fs->retaddr_column = 31;

  while (1)
    {
      op = *(p++);
      switch (op)
	{
	case 0 ... 0x39:
	  /* Increment stack pointer.  */
	  record_cfa_adjustment (fs, (op + 1) * MIPS_EH_STACK_ALIGN);
	  break;

        case 0x40 ... 0x47:
	  /* Push VR[16] to VR[16+x] and VR[31] */
	  push_offset = record_push (fs, 31, push_offset);
	  for (i = op & 7; i >= 0; i--)
	    push_offset = record_push (fs, 16 + i, push_offset);
	  break;

	case 0x48 ... 0x4f:
	  /* Push VR[16] to VR[16+x], VR[30] and VR[31] */
	  push_offset = record_push (fs, 31, push_offset);
	  push_offset = record_push (fs, 30, push_offset);
	  for (i = op & 7; i >= 0; i--)
	    push_offset = record_push (fs, 16 + i, push_offset);
	  break;

	case 0x50 ... 0x57:
	  /* Restore stack pointer from frame pointer */
	  fs->regs.cfa_reg = (op & 7) + 16;
	  fs->regs.cfa_offset = 0;
	  break;

	case 0x58:
	  /* Large SP increment.  */
	  p = read_uleb128 (p, &val);
	  record_cfa_adjustment (fs, (val + 129) * MIPS_EH_STACK_ALIGN);
	  break;

	case 0x59:
	  /* Push VR[x] to VR[x+y] */
	  op = *(p++);
	  n = op >> 3;
	  for (i = op & 7; i >= 0; i--)
	    push_offset = record_push (fs, n + i, push_offset);
	  break;

	case 0x5a:
	  /* Push VRF[x] to VRF[x+y] */
	  op = *(p++);
	  n = (op >> 3) + VRF_0;
	  for (i = op & 7; i >= 0; i--)
	    push_offset = record_push (fs, n + i, push_offset);
	  break;

	case 0x5b:
	  /* Restore the CFA to stack pointer.  */
	  fs->regs.cfa_reg = STACK_POINTER_REGNUM;
	  fs->regs.cfa_offset = 0;
	  break;

	case 0x5c:
	  /* Finish.  */
	  *pp = p;
	  return _URC_NO_REASON;

	case 0x5d:
	  /* No unwind.  */
	  return _URC_END_OF_STACK;

	case 0x5e:
	  /* Restore SP from VR[30] */
	  fs->regs.cfa_reg = 30;
	  fs->regs.cfa_offset = 0;
	  break;

	case 0x5f:
	  /* NOP */
	  break;

	case 0x60 ... 0x6b:
	  /* Push VRF[20] to VRF[20 + x] */
	  for (i = op & 0xf; i >= 0; i--)
	    push_offset = record_push (fs, VRF_0 + 20 + i, push_offset);
	  break;

	case 0x6c ... 0x6f:
	  /* MIPS16 push VR[16], VR[17], VR[18+x]-VR[23], VR[31] */
	  push_offset = record_push (fs, 31, push_offset);
	  for (i = 23; i >= 18 + (op & 3); i--)
	    push_offset = record_push (fs, i, push_offset);
	  push_offset = record_push (fs, 17, push_offset);
	  push_offset = record_push (fs, 16, push_offset);
	  break;

	case 0x70 ... 0x7f:    
	  /* Push VR[16] to VR[16+x], VR[28], VR[31]
	     and optionally VR[30].  */
	  push_offset = record_push (fs, 31, push_offset);
	  if (op & 0x08)
	    push_offset = record_push (fs, 30, push_offset);
	  push_offset = record_push (fs, 28, push_offset);
	  for (i = op & 7; i >= 0; i--)
	    push_offset = record_push (fs, 16 + i, push_offset);
	  break;

	default:
	  return _URC_FATAL_PHASE1_ERROR;
        }
    }
}

static _Unwind_Reason_Code
md_unwind_compact_pabi (struct _Unwind_Context *context ATTRIBUTE_UNUSED,
		   _Unwind_FrameState *fs, const unsigned char **pp)
{
  unsigned char op;
  _uleb128_t val;
  int push_offset;
  int i;
  int n;
  const unsigned char *p = *pp;

  push_offset = 0;
  fs->regs.cfa_how = CFA_REG_OFFSET;
  fs->regs.cfa_reg = STACK_POINTER_REGNUM;
  fs->regs.cfa_offset = 0;
  fs->retaddr_column = 31;

  while (1)
    {
      op = *(p++);
      switch (op)
	{
	case 0 ... 0x39:
	  /* Increment stack pointer.  */
	  record_cfa_adjustment (fs, (op + 1) * MIPS_EH_STACK_ALIGN);
	  break;

        case 0x40 ... 0x47:
	  /* Push VR[16] to VR[16+x] and VR[31] */
	  for (i = op & 7; i >= 0; i--)
	    push_offset = record_push (fs, 16 + i, push_offset);
	  push_offset = record_push (fs, 31, push_offset);
	  break;

	case 0x48 ... 0x4f:
	  /* Push VR[16] to VR[16+x], VR[30] and VR[31] */
	  for (i = op & 7; i >= 0; i--)
	    push_offset = record_push (fs, 16 + i, push_offset);
	  push_offset = record_push (fs, 31, push_offset);
	  push_offset = record_push (fs, 30, push_offset);
	  break;

	case 0x50 ... 0x57:
	  /* Restore stack pointer from frame pointer */
	  fs->regs.cfa_reg = (op & 7) + 16;
	  fs->regs.cfa_offset = 0;
	  break;

	case 0x58:
	  /* Large SP increment.  */
	  p = read_uleb128 (p, &val);
	  record_cfa_adjustment (fs, (val + 129) * MIPS_EH_STACK_ALIGN);
	  break;

        // TODO: not possible, restrict
	case 0x59:
	  /* Push VR[x] to VR[x+y] */
	  op = *(p++);
	  n = op >> 3;
	  for (i = op & 7; i >= 0; i--)
	    push_offset = record_push (fs, n + i, push_offset);
	  break;

        // TODO: not possible, restrict
	case 0x5a:
	  /* Push VRF[x] to VRF[x+y] */
	  op = *(p++);
	  n = (op >> 3) + VRF_0;
	  for (i = op & 7; i >= 0; i--)
	    push_offset = record_push (fs, n + i, push_offset);
	  break;

	case 0x5b:
	  /* Restore the CFA to stack pointer.  */
	  fs->regs.cfa_reg = STACK_POINTER_REGNUM;
	  fs->regs.cfa_offset = 0;
	  break;

	case 0x5c:
	  /* Finish.  */
	  *pp = p;
	  return _URC_NO_REASON;

	case 0x5d:
	  /* No unwind.  */
	  return _URC_END_OF_STACK;

	case 0x5e:
	  /* Restore SP from VR[30] */
	  fs->regs.cfa_reg = 30;
	  fs->regs.cfa_offset = 0;
	  break;

	case 0x5f:
	  /* NOP */
	  break;

	case 0x60 ... 0x6b:
	  /* Push VRF[20] to VRF[20 + x] */
	  for (i = op & 0xf; i >= 0; i--)
	    push_offset = record_push (fs, VRF_0 + 20 + i, push_offset);
	  break;

//        case 0x60 ... 0x67:
//	  /* Push VRF[16] to VRF[16 + x] */
//	  for (i = op & 0xf; i >= 0; i--)
//            {
//	      push_offset = record_push (fs, VRF_0 + 16 + (2*i)+1, push_offset);
//	      push_offset = record_push (fs, VRF_0 + 16 + (2*i), push_offset);
//            }
//	  break;
//
//        case 0x68 ... 0x6b:
//          /* TODO */
//          break;

	case 0x6c ... 0x6f:
	  /* MIPS16 push VR[16], VR[17], VR[18+x]-VR[23], VR[31] */
	  for (i = 23; i >= 18 + (op & 3); i--)
	    push_offset = record_push (fs, i, push_offset);
	  push_offset = record_push (fs, 17, push_offset);
	  push_offset = record_push (fs, 16, push_offset);
	  push_offset = record_push (fs, 31, push_offset);
	  break;

	case 0x70 ... 0x7f:
	  /* Push VR[16] to VR[16+x], VR[28], VR[31]
	     and optionally VR[30].  */
	  push_offset = record_push (fs, 28, push_offset);
	  for (i = op & 7; i >= 0; i--)
	    push_offset = record_push (fs, 16 + i, push_offset);
	  push_offset = record_push (fs, 31, push_offset);
	  if (op & 0x08)
	    push_offset = record_push (fs, 30, push_offset);
	  break;

	default:
	  return _URC_FATAL_PHASE1_ERROR;
        }
    }
}

static _Unwind_Reason_Code
md_unwind_compact (struct _Unwind_Context *context ATTRIBUTE_UNUSED,
		   _Unwind_FrameState *fs, const unsigned char **pp)
{
#if defined(_MIPS_SIM) && _MIPS_SIM == _ABIP32
  return md_unwind_compact_pabi(context, fs, pp);
#else
  return md_unwind_compact_oabi_nabi(context, fs, pp);
#endif
}

#endif /* MD_HAVE_COMPACT_EH */
