/* DWARF2 EH unwinding support for m68k Linux.
   Copyright (C) 2006 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file with other programs, and to distribute
those programs without any restriction coming from the use of this
file.  (The General Public License restrictions do apply in other
respects; for example, they cover modification of the file, and
distribution when not linked into another program.)

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#ifndef inhibit_libc
#include <signal.h>
#include <asm/unistd.h>

#define MD_FALLBACK_FRAME_STATE_FOR m68k_fallback_frame_state

struct sigframe_footer {
  char retcode[8];
  unsigned long extramask[1];
  struct sigcontext sc;
};

struct rt_sigframe_footer {
  char retcode[8];
  struct siginfo info;
  struct ucontext uc;
};

static _Unwind_Reason_Code
m68k_fallback_frame_state (struct _Unwind_Context *context,
			   _Unwind_FrameState *fs)
{
  unsigned short *pc;
  _Unwind_Ptr new_cfa, base;
  int i;

  pc = (unsigned short *) context->ra;

  /* Check for:

	moveq #__NR_sigreturn, %d0	(70xx)
	trap #0				(4e40)
  */
  if (pc[0] == (0x7000 | __NR_sigreturn) && pc[1] == 0x4e40)
    {
      struct sigcontext *sc;

      sc = &((struct sigframe_footer *) context->ra)->sc;

      new_cfa = (_Unwind_Ptr) sc;
      fs->cfa_how = CFA_REG_OFFSET;
      fs->cfa_reg = STACK_POINTER_REGNUM;
      fs->cfa_offset = new_cfa - (_Unwind_Ptr) context->cfa;

      fs->regs.reg[25].how = REG_SAVED_OFFSET;
      fs->regs.reg[25].loc.offset = (_Unwind_Ptr) &sc->sc_pc - new_cfa;
      fs->retaddr_column = 25;

      return _URC_NO_REASON;
    }
  /* Coldfire uses the sequence:

	move.l #__NR_rt_sigreturn, %d0	(203c xxxx xxxx)
	trap #0				(4e40)

     Others use:

	moveq #__NR_rt_sigreturn, %d0	(70xx)
	not.b %d0			(4600)
	trap #0				(4e40)
  */
#ifdef __mcoldfire__
  else if (pc[0] == 0x203c
	   && pc[1] == 0x0000
	   && pc[2] == __NR_rt_sigreturn
	   && pc[3] == 0x4e40)
#else
  else if (pc[0] == (0x7000 | __NR_rt_sigreturn)
	   && pc[1] == 0x4600
	   && pc[2] == 0x4e40)
#endif
    {
      mcontext_t *mc;

      mc = &((struct rt_sigframe_footer *) context->ra)->uc.uc_mcontext;

      new_cfa = (_Unwind_Ptr) mc;
      fs->cfa_how = CFA_REG_OFFSET;
      fs->cfa_reg = STACK_POINTER_REGNUM;
      fs->cfa_offset = new_cfa - (_Unwind_Ptr) context->cfa;

      base = (_Unwind_Ptr) mc->gregs - new_cfa;
      for (i = 0; i < 16; i++)
	{
	  fs->regs.reg[i].how = REG_SAVED_OFFSET;
	  fs->regs.reg[i].loc.offset = base + i * 4;
	}
      fs->regs.reg[25].how = REG_SAVED_OFFSET;
      fs->regs.reg[25].loc.offset = base + R_PC * 4;
      fs->retaddr_column = 25;

      for (i = 0; i < 8; i++)
	{
	  base = (_Unwind_Ptr) &mc->fpregs.f_fpregs[i] - new_cfa;
	  fs->regs.reg[i + 16].how = REG_SAVED_OFFSET;
	  fs->regs.reg[i + 16].loc.offset = base;
	}
      return _URC_NO_REASON;
    }
  else
    return _URC_END_OF_STACK;
}
#endif
