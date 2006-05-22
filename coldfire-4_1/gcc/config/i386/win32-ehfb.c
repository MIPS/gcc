
/* This file implements the md_fallback_frame_state_for routine for Windows,
   triggered when the GCC table based unwinding process hits a frame for which
   no unwind info has been registered. This typically occurs when raising an
   exception from a signal handler, because the handler is actually called
   from the OS kernel.

   The basic idea is to detect that we are indeed trying to unwind past a
   signal handler and to fill out the GCC internal unwinding structures for
   the OS kernel frame as if it had been directly called from the interrupted
   context.

   This is all assuming that the code to set the handler asked the kernel to
   pass a pointer to such context information.

   There is three main parts.

   1) The first thing to do is to check if we are in a signal context. If not
      we can just return as there is nothing to do. We are probably on some
      foreign code for which no unwind frame can be found. If this is a call
      from the Windows signal handler, then:

   2) We must get the signal context information. 

      * With the standard exception filter:

      This is on Windows pointed to by an EXCEPTION_POINTERS. We know that the
      signal handle will call an UnhandledExceptionFilter with this
      parameter. The spec for this routine is:

         LONG WINAPI UnhandledExceptionFilter(struct _EXCEPTION_POINTERS*);

      So the pointer to struct _EXCEPTION_POINTERS must be somewhere on the
      stack.

      This was found experimentally to always be at offset 0 of the context
      frame in all cases handled by this implementation.

      * With the SEH exception handler:

      In this case the signal context is directly on the stack as the SEH
      exception handler has the following prototype:

         DWORD
         SEH_error_handler (PEXCEPTION_RECORD ExceptionRecord,
                            PVOID EstablisherFrame,
                            PCONTEXT ContextRecord,
                            PVOID DispatcherContext)

      This was found experimentally to always be at offset 56 of the context
      frame in all cases handled by this implementation.

   3) When we have the signal context we just have to save some registers and
      set the return address based on the program counter (Eip).

   Note that this implementation follows closely the same principles as the
   GNU/Linux and OSF ones.  */


/* Patterns found experimentally to be on a Windows signal handler  */

/* In a standard exception filter  */

#define SIG_PAT1 \
      (pc_[-2] == 0xff && pc_[-1] == 0xd0     /* call %eax           */ \
      && pc_[0] == 0x83 && pc_[1] == 0xf8)    /* cmp 0xdepl,%eax     */

#define SIG_PAT2 \
        (pc_[-5] == 0xe8 && pc_[-4] == 0x68   /* call (depl16)       */ \
         && pc_[0] == 0xc3)                   /* ret                 */

/* In a Win32 SEH handler  */

#define SIG_SEH1 \
        (pc_[-5] == 0xe8                      /* call addr           */ \
         && pc_[0] == 0x83 && pc_[1] == 0xc4  /* add 0xval,%esp      */ \
         && pc_[3] == 0xb8)                   /* mov 0xval,%eax      */

#define SIG_SEH2 \
        (pc_[-5] == 0x8b && pc_[-4] == 0x4d   /* mov depl(%ebp),%ecx */ \
         && pc_[0] == 0x64 && pc_[1] == 0x8b) /* mov %fs:(0),<reg>   */ \

/* In the GCC alloca (stack probing)  */

#define SIG_ALLOCA \
          (pc_[-1] == 0x83                    /* orl $0x0,(%ecx)     */ \
	   && pc_[0] == 0x9 && pc_[1] == 0                              \
	   && pc_[2] == 0x2d && pc_[3] == 0   /* subl $0x1000,%eax   */ \
	   && pc_[4] == 0x10 && pc_[5] == 0)

int
md_fallback_frame_state_for (struct _Unwind_Context *context, 
			     _Unwind_FrameState *fs);

int
md_fallback_frame_state_for (struct _Unwind_Context *context, 
			     _Unwind_FrameState *fs)

{									      
  char * eh_debug_env = getenv ("EH_DEBUG");
  int eh_debug = eh_debug_env ? atoi (eh_debug_env) : 0;
 
  void * ctx_ra_  = (void *)(context->ra);  /* return address */
  void * ctx_cfa_ = (void *)(context->cfa); /* context frame address */
  unsigned char * pc_ = (unsigned char *) ctx_ra_;

  if (eh_debug)
    printf ("FALLBACK called for CFA = 0x%p, RA = 0x%p\n", ctx_cfa_, ctx_ra_);

  /* In the test below we look for two specific patterns found
     experimentally to be in the Windows signal handler.  */

  if (SIG_PAT1 || SIG_PAT2 || SIG_SEH1 || SIG_SEH2)
    {
      PEXCEPTION_POINTERS weinfo_;
      PCONTEXT proc_ctx_;
      long new_cfa_;

      if (SIG_SEH1) 
	{
	  proc_ctx_ = (PCONTEXT) (*(int*)(ctx_cfa_ + 56));
	}
      else if (SIG_SEH2)
	{
	  proc_ctx_ = (PCONTEXT) (*(int*)(ctx_cfa_ + 8));
	}
      else
	{
	  weinfo_ = (PEXCEPTION_POINTERS) (*(int*)ctx_cfa_);
	  proc_ctx_ = weinfo_->ContextRecord;
	}

      /* The new context frame address is the stack pointer.  */

      new_cfa_ = proc_ctx_->Esp;
      fs->cfa_how = CFA_REG_OFFSET;
      fs->cfa_reg = __builtin_dwarf_sp_column();
      fs->cfa_offset = new_cfa_ - (long) ctx_cfa_;

      /* Save some registers.  */

      fs->regs.reg[0].how = REG_SAVED_OFFSET;
      fs->regs.reg[0].loc.offset = (long)&proc_ctx_->Eax - new_cfa_;
      fs->regs.reg[3].how = REG_SAVED_OFFSET;
      fs->regs.reg[3].loc.offset = (long)&proc_ctx_->Ebx - new_cfa_;
      fs->regs.reg[1].how = REG_SAVED_OFFSET;
      fs->regs.reg[1].loc.offset = (long)&proc_ctx_->Ecx - new_cfa_;
      fs->regs.reg[2].how = REG_SAVED_OFFSET;
      fs->regs.reg[2].loc.offset = (long)&proc_ctx_->Edx - new_cfa_;
      fs->regs.reg[6].how = REG_SAVED_OFFSET;
      fs->regs.reg[6].loc.offset = (long)&proc_ctx_->Esi - new_cfa_;
      fs->regs.reg[7].how = REG_SAVED_OFFSET;
      fs->regs.reg[7].loc.offset = (long)&proc_ctx_->Edi - new_cfa_;
      fs->regs.reg[9].how = REG_SAVED_OFFSET;
      fs->regs.reg[9].loc.offset = (long)&proc_ctx_->Eip - new_cfa_;
      fs->regs.reg[4].how = REG_SAVED_OFFSET;
      fs->regs.reg[4].loc.offset = (long)&proc_ctx_->Ebp - new_cfa_;

      /* Set the return address to Eip + 1. As we can be called multiple
	 times we use another register for this.  */
      
      proc_ctx_->Dr0 = proc_ctx_->Eip + 1;
      fs->regs.reg[8].how = REG_SAVED_OFFSET;
      fs->regs.reg[8].loc.offset = (long)&proc_ctx_->Dr0 - new_cfa_;
      fs->retaddr_column = 8;
      return 1;
    }

  /* Unwinding through _alloca, propagating from a trap triggered by
     one of it's probes prior to the real SP aadjustment. The only
     operations of interest performed is "pushl %ecx", followed by
     ecx clobbering.  */

  else if (SIG_ALLOCA) 
    {
      /* Only one push between entry in _alloca and the probe trap.  */ 
      long new_cfa_ = (long) ctx_cfa_ + 4;

      fs->cfa_how = CFA_REG_OFFSET;
      fs->cfa_reg = __builtin_dwarf_sp_column();
      fs->cfa_offset = new_cfa_ - (long) ctx_cfa_;

      /* The saved value of %ecx is at CFA - 4 */
      fs->regs.reg[1].how = REG_SAVED_OFFSET;
      fs->regs.reg[1].loc.offset = -4;

      /* and what is stored at the CFA is the return address.  */
      fs->retaddr_column = 8;
      fs->regs.reg[8].how = REG_SAVED_OFFSET;
      fs->regs.reg[8].loc.offset = 0;
 
      return 1;
    }
  else
    return 0;
}
