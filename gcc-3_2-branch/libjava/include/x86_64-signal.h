// x86_64-signal.h - Catch runtime signals and turn them into exceptions
// on an x86_64 based GNU/Linux system.

/* Copyright (C) 2003  Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */


#ifndef JAVA_SIGNAL_H
#define JAVA_SIGNAL_H 1

#include <signal.h>
#include <sys/syscall.h>

#ifdef __x86_64__

#define HANDLE_SEGV 1

#define SIGNAL_HANDLER(_name)	\
static void _Jv_##_name (int, siginfo_t *_sip, void *_p)

extern "C" 
{
  struct kernel_sigaction 
  {
    void (*k_sa_sigaction)(int,siginfo_t *,void *);
    unsigned long k_sa_flags;
    void (*k_sa_restorer) (void);
    sigset_t k_sa_mask;
  };
}

#define MAKE_THROW_FRAME(_exception)					\
do									\
{									\
  /* Advance the program counter so that it is after the start of the	\
     instruction:  the x86_64 exception handler expects			\
     the PC to point to the instruction after a call. */		\
  struct ucontext *_uc = (struct ucontext *)_p;				\
  struct sigcontext *_sc = (struct sigcontext *) &_uc->uc_mcontext;	\
  _sc->rip += 2;							\
}									\
while (0)

#define RESTORE(name, syscall) RESTORE2 (name, syscall)
#define RESTORE2(name, syscall)			\
asm						\
  (						\
   ".byte 0  # Yes, this really is necessary\n" \
   ".align 16\n"				\
   "__" #name ":\n"				\
   "	movq $" #syscall ", %rax\n"		\
   "	syscall\n"				\
   );

/* The return code for realtime-signals.  */
RESTORE (restore_rt, __NR_rt_sigreturn)
static void restore_rt (void) asm ("__restore_rt");

#define INIT_SEGV						\
do								\
  {								\
    nullp = new java::lang::NullPointerException ();		\
    struct kernel_sigaction act;				\
    act.k_sa_sigaction = _Jv_catch_segv;			\
    sigemptyset (&act.k_sa_mask);				\
    act.k_sa_flags = SA_SIGINFO|0x4000000;			\
    act.k_sa_restorer = restore_rt;				\
    syscall (SYS_rt_sigaction, SIGSEGV, &act, NULL, _NSIG / 8);	\
  }								\
while (0)  

/* We use syscall(SYS_rt_sigaction) in INIT_SEGV instead of
 * sigaction() because on some systems the pthreads wrappers for
 * signal handlers are not compiled with unwind information, so it's
 * not possible to unwind through them.  This is a problem that will
 * go away if all systems ever have pthreads libraries that are
 * compiled with unwind info.  */

#else /* i386 */

#define HANDLE_SEGV 1
#define HANDLE_FPE 1

#define SIGNAL_HANDLER(_name)	\
static void _name (int _dummy)

#define MAKE_THROW_FRAME(_exception)					\
do									\
{									\
  void **_p = (void **)&_dummy;						\
  struct sigcontext *_regs = (struct sigcontext *)++_p;		\
									\
  /* Advance the program counter so that it is after the start of the	\
     instruction:  the x86 exception handler expects			\
     the PC to point to the instruction after a call. */		\
  _regs->eip += 2;							\
									\
}									\
while (0)

#define HANDLE_DIVIDE_OVERFLOW						\
do									\
{									\
  void **_p = (void **)&_dummy;						\
  struct sigcontext *_regs = (struct sigcontext *)++_p;		\
									\
  register unsigned char *_eip = (unsigned char *)_regs->eip;		\
									\
  /* According to the JVM spec, "if the dividend is the negative	\
   * integer of the smallest magnitude and the divisor is -1, then	\
   * overflow occurs and the result is equal to the dividend.  Despite	\
   * the overflow, no exception occurs".				\
									\
   * We handle this by inspecting the instruction which generated the	\
   * signal and advancing eip to point to the following instruction.	\
   * As the instructions are variable length it is necessary to do a	\
   * little calculation to figure out where the following instruction	\
   * actually is.							\
									\
   */									\
									\
  if (_eip[0] == 0xf7)							\
    {									\
      unsigned char _modrm = _eip[1];					\
									\
      if (_regs->eax == 0x80000000					\
	  && ((_modrm >> 3) & 7) == 7) /* Signed divide */		\
	{								\
	  _regs->edx = 0; /* the remainder is zero */			\
	  switch (_modrm >> 6)						\
	    {								\
	    case 0:							\
	      if ((_modrm & 7) == 5)					\
		_eip += 4;						\
	      break;							\
	    case 1:							\
	      _eip += 1;						\
	      break;							\
	    case 2:							\
	      _eip += 4;						\
	      break;							\
	    case 3:							\
	      break;							\
	    }								\
	  _eip += 2;							\
	  _regs->eip = (unsigned long)_eip;				\
	  return;							\
	}								\
      else								\
	{								\
	  /* Advance the program counter so that it is after the start	\
	     of the instruction: this is because the x86 exception	\
	     handler expects the PC to point to the instruction after a	\
	     call. */							\
	  _regs->eip += 2;						\
	}								\
    }									\
}									\
while (0)

/* We use old_kernel_sigaction here because we're calling the kernel
   directly rather than via glibc.  The sigaction structure that the
   syscall uses is a different shape from the one in userland and not
   visible to us in a header file so we define it here.  */

struct old_i386_kernel_sigaction {
	void (*k_sa_handler) (int);
	unsigned long k_sa_mask;
	unsigned long k_sa_flags;
	void (*sa_restorer) (void);
};

#define INIT_SEGV						\
do								\
  {								\
    nullp = new java::lang::NullPointerException ();    	\
    struct old_i386_kernel_sigaction kact;		\
    kact.k_sa_handler = catch_segv;			\
    kact.k_sa_mask = 0;					\
    kact.k_sa_flags = 0;				\
    syscall (SYS_sigaction, SIGSEGV, &kact, NULL);	\
  }								\
while (0)  

#define INIT_FPE						\
do								\
  { 								\
    arithexception = new java::lang::ArithmeticException 	\
      (JvNewStringLatin1 ("/ by zero"));			\
    struct old_i386_kernel_sigaction kact;			\
    kact.k_sa_handler = catch_fpe;				\
    kact.k_sa_mask = 0;						\
    kact.k_sa_flags = 0;					\
    syscall (SYS_sigaction, SIGFPE, &kact, NULL);		\
  }								\
while (0)  

/* You might wonder why we use syscall(SYS_sigaction) in INIT_FPE
 * instead of the standard sigaction().  This is necessary because of
 * the shenanigans above where we increment the PC saved in the
 * context and then return.  This trick will only work when we are
 * called _directly_ by the kernel, because linuxthreads wraps signal
 * handlers and its wrappers do not copy the sigcontext struct back
 * when returning from a signal handler.  If we return from our divide
 * handler to a linuxthreads wrapper, we will lose the PC adjustment
 * we made and return to the faulting instruction again.  Using
 * syscall(SYS_sigaction) causes our handler to be called directly
 * by the kernel, bypassing any wrappers.

 * Also, there is at the present time no unwind info in the
 * linuxthreads library's signal handlers and so we can't unwind
 * through them anyway.  

 * Finally, the code that glibc uses to return from a signal handler
 * is subject to change.  */

#endif /* i386 */

#endif /* JAVA_SIGNAL_H */
  
