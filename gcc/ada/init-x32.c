/****************************************************************************
 *                                                                          *
 *                         GNAT COMPILER COMPONENTS                         *
 *                                                                          *
 *                                 I N I T                                  *
 *                                                                          *
 *                          C Implementation File                           *
 *                                                                          *
 *             Copyright (C) 2015, Free Software Foundation, Inc.           *
 *                                                                          *
 * GNAT is free software;  you can  redistribute it  and/or modify it under *
 * terms of the  GNU General Public License as published  by the Free Soft- *
 * ware  Foundation;  either version 3,  or (at your option) any later ver- *
 * sion.  GNAT is distributed in the hope that it will be useful, but WITH- *
 * OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE.                                     *
 *                                                                          *
 * As a special exception under Section 7 of GPL version 3, you are granted *
 * additional permissions described in the GCC Runtime Library Exception,   *
 * version 3.1, as published by the Free Software Foundation.               *
 *                                                                          *
 * You should have received a copy of the GNU General Public License and    *
 * a copy of the GCC Runtime Library Exception along with this program;     *
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see    *
 * <http://www.gnu.org/licenses/>.                                          *
 *                                                                          *
 * GNAT was originally developed  by the GNAT team at  New York University. *
 * Extensive contributions were provided by Ada Core Technologies Inc.      *
 *                                                                          *
 ****************************************************************************/

/*  This unit contains initialization circuits that are system dependent.
    A major part of the functionality involves stack overflow checking.
    The GCC backend generates probe instructions to test for stack overflow.
    For details on the exact approach used to generate these probes, see the
    "Using and Porting GCC" manual, in particular the "Stack Checking" section
    and the subsection "Specifying How Stack Checking is Done".  The handlers
    installed by this file are used to catch the resulting signals that come
    from these probes failing (i.e. touching protected pages).  */

/* This file should be kept synchronized with s-init.ads, s-init.adb and the
   s-init-*.adb variants. All these files implement the required functionality
   for different targets.  */

#if !defined (__linux__)
#error Unsupported platform.
#endif

/* The following include is here to meet the published VxWorks requirement
   that the __vxworks header appear before any other include.  */
#ifdef __vxworks
#include "vxWorks.h"
#endif

#ifdef __ANDROID__
#undef linux
#endif

#ifdef IN_RTS
#include "tconfig.h"
#include "tsystem.h"
#include <sys/stat.h>

/* We don't have libiberty, so use malloc.  */
#define xmalloc(S) malloc (S)
#else
#include "config.h"
#include "system.h"
#endif

#include "adaint.h"
#include "raise.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void __gnat_raise_program_error (const char *, int);

/* Addresses of exception data blocks for predefined exceptions.  Tasking_Error
   is not used in this unit, and the abort signal is only used on IRIX.
   ??? Revisit this part since IRIX is no longer supported.  */
extern struct Exception_Data constraint_error;
extern struct Exception_Data numeric_error;
extern struct Exception_Data program_error;
extern struct Exception_Data storage_error;

/* For the Cert run time we use the regular raise exception routine because
   Raise_From_Signal_Handler is not available.  */
#ifdef CERT
#define Raise_From_Signal_Handler \
                      __gnat_raise_exception
extern void Raise_From_Signal_Handler (struct Exception_Data *, const char *);
#else
#define Raise_From_Signal_Handler \
                      ada__exceptions__raise_from_signal_handler
extern void Raise_From_Signal_Handler (struct Exception_Data *, const char *);
#endif

/* Global values computed by the binder.  */
int   __gl_main_priority                 = -1;
int   __gl_main_cpu                      = -1;
int   __gl_time_slice_val                = -1;
char  __gl_wc_encoding                   = 'n';
char  __gl_locking_policy                = ' ';
char  __gl_queuing_policy                = ' ';
char  __gl_task_dispatching_policy       = ' ';
char *__gl_priority_specific_dispatching = 0;
int   __gl_num_specific_dispatching      = 0;
char *__gl_interrupt_states              = 0;
int   __gl_num_interrupt_states          = 0;
int   __gl_unreserve_all_interrupts      = 0;
int   __gl_exception_tracebacks          = 0;
int   __gl_detect_blocking               = 0;
int   __gl_default_stack_size            = -1;
int   __gl_leap_seconds_support          = 0;
int   __gl_canonical_streams             = 0;

/* This value is not used anymore, but kept for bootstrapping purpose.  */
int   __gl_zero_cost_exceptions          = 0;

/* Indication of whether synchronous signal handler has already been
   installed by a previous call to adainit.  */
int  __gnat_handler_installed      = 0;

#ifndef IN_RTS
int __gnat_inside_elab_final_code = 0;
/* ??? This variable is obsolete since 2001-08-29 but is kept to allow
   bootstrap from old GNAT versions (< 3.15).  */
#endif

/* HAVE_GNAT_INIT_FLOAT must be set on every targets where a __gnat_init_float
   is defined.  If this is not set then a void implementation will be defined
   at the end of this unit.  */
#undef HAVE_GNAT_INIT_FLOAT

/******************************/
/* __gnat_get_interrupt_state */
/******************************/

char __gnat_get_interrupt_state (int);

/* This routine is called from the runtime as needed to determine the state
   of an interrupt, as set by an Interrupt_State pragma appearing anywhere
   in the current partition.  The input argument is the interrupt number,
   and the result is one of the following:

       'n'   this interrupt not set by any Interrupt_State pragma
       'u'   Interrupt_State pragma set state to User
       'r'   Interrupt_State pragma set state to Runtime
       's'   Interrupt_State pragma set state to System  */

char
__gnat_get_interrupt_state (int intrup)
{
  if (intrup >= __gl_num_interrupt_states)
    return 'n';
  else
    return __gl_interrupt_states [intrup];
}

/***********************************/
/* __gnat_get_specific_dispatching */
/***********************************/

char __gnat_get_specific_dispatching (int);

/* This routine is called from the runtime as needed to determine the
   priority specific dispatching policy, as set by a
   Priority_Specific_Dispatching pragma appearing anywhere in the current
   partition.  The input argument is the priority number, and the result
   is the upper case first character of the policy name, e.g. 'F' for
   FIFO_Within_Priorities. A space ' ' is returned if no
   Priority_Specific_Dispatching pragma is used in the partition.  */

char
__gnat_get_specific_dispatching (int priority)
{
  if (__gl_num_specific_dispatching == 0)
    return ' ';
  else if (priority >= __gl_num_specific_dispatching)
    return 'F';
  else
    return __gl_priority_specific_dispatching [priority];
}

#ifndef IN_RTS

/**********************/
/* __gnat_set_globals */
/**********************/

/* This routine is kept for bootstrapping purposes, since the binder generated
   file now sets the __gl_* variables directly.  */

void
__gnat_set_globals (void)
{
}

#endif

/*********************/
/* GNU/Linux Section */
/*********************/

#if defined (linux)

#include <signal.h>

#define __USE_GNU 1 /* required to get REG_EIP/RIP from glibc's ucontext.h */
#include <sys/ucontext.h>

/* GNU/Linux, which uses glibc, does not define NULL in included
   header files.  */

#if !defined (NULL)
#define NULL ((void *) 0)
#endif

#if defined (MaRTE)

/* MaRTE OS provides its own version of sigaction, sigfillset, and
   sigemptyset (overriding these symbol names).  We want to make sure that
   the versions provided by the underlying C library are used here (these
   versions are renamed by MaRTE to linux_sigaction, fake_linux_sigfillset,
   and fake_linux_sigemptyset, respectively).  The MaRTE library will not
   always be present (it will not be linked if no tasking constructs are
   used), so we use the weak symbol mechanism to point always to the symbols
   defined within the C library.  */

#pragma weak linux_sigaction
int linux_sigaction (int signum, const struct sigaction *act,
		     struct sigaction *oldact)
{
  return sigaction (signum, act, oldact);
}
#define sigaction(signum, act, oldact) linux_sigaction (signum, act, oldact)

#pragma weak fake_linux_sigfillset
void fake_linux_sigfillset (sigset_t *set)
{
  sigfillset (set);
}
#define sigfillset(set) fake_linux_sigfillset (set)

#pragma weak fake_linux_sigemptyset
void fake_linux_sigemptyset (sigset_t *set)
{
  sigemptyset (set);
}
#define sigemptyset(set) fake_linux_sigemptyset (set)

#endif

#if defined (i386) || defined (__x86_64__) || defined (__ia64__) \
    || defined (__ARMEL__)

#define HAVE_GNAT_ADJUST_CONTEXT_FOR_RAISE

void
__gnat_adjust_context_for_raise (int signo ATTRIBUTE_UNUSED, void *ucontext)
{
  mcontext_t *mcontext = &((ucontext_t *) ucontext)->uc_mcontext;

  /* On the i386 and x86-64 architectures, stack checking is performed by
     means of probes with moving stack pointer, that is to say the probed
     address is always the value of the stack pointer.  Upon hitting the
     guard page, the stack pointer therefore points to an inaccessible
     address and an alternate signal stack is needed to run the handler.
     But there is an additional twist: on these architectures, the EH
     return code writes the address of the handler at the target CFA's
     value on the stack before doing the jump.  As a consequence, if
     there is an active handler in the frame whose stack has overflowed,
     the stack pointer must nevertheless point to an accessible address
     by the time the EH return is executed.

     We therefore adjust the saved value of the stack pointer by the size
     of one page + a small dope of 4 words, in order to make sure that it
     points to an accessible address in case it's used as the target CFA.
     The stack checking code guarantees that this address is unused by the
     time this happens.  */

#if defined (i386)
  unsigned long *pc = (unsigned long *)mcontext->gregs[REG_EIP];
  /* The pattern is "orl $0x0,(%esp)" for a probe in 32-bit mode.  */
  if (signo == SIGSEGV && pc && *pc == 0x00240c83)
    mcontext->gregs[REG_ESP] += 4096 + 4 * sizeof (unsigned long);
#elif defined (__x86_64__)
  unsigned long long *pc = (unsigned long long *)mcontext->gregs[REG_RIP];
  if (signo == SIGSEGV && pc
      /* The pattern is "orq $0x0,(%rsp)" for a probe in 64-bit mode.  */
      && ((*pc & 0xffffffffffLL) == 0x00240c8348LL
	  /* The pattern may also be "orl $0x0,(%esp)" for a probe in
	     x32 mode.  */
	  || (*pc & 0xffffffffLL) == 0x00240c83LL))
    mcontext->gregs[REG_RSP] += 4096 + 4 * sizeof (unsigned long);
#elif defined (__ia64__)
  /* ??? The IA-64 unwinder doesn't compensate for signals.  */
  mcontext->sc_ip++;
#elif defined (__ARMEL__)
  /* ARM Bump has to be an even number because of odd/even architecture.  */
  mcontext->arm_pc+=2;
#endif
}

#endif

static void
__gnat_error_handler (int sig, siginfo_t *si ATTRIBUTE_UNUSED, void *ucontext)
{
  struct Exception_Data *exception;
  const char *msg;

  /* Adjusting is required for every fault context, so adjust for this one
     now, before we possibly trigger a recursive fault below.  */
  __gnat_adjust_context_for_raise (sig, ucontext);

  switch (sig)
    {
    case SIGSEGV:
      /* Here we would like a discrimination test to see whether the page
	 before the faulting address is accessible.  Unfortunately, Linux
	 seems to have no way of giving us the faulting address.

	 In old versions of init.c, we had a test of the page before the
	 stack pointer:

	   ((volatile char *)
	    ((long) si->esp_at_signal & - getpagesize ()))[getpagesize ()];

	 but that's wrong since it tests the stack pointer location and the
	 stack probing code may not move it until all probes succeed.

	 For now we simply do not attempt any discrimination at all. Note
	 that this is quite acceptable, since a "real" SIGSEGV can only
	 occur as the result of an erroneous program.  */
      exception = &storage_error;
      msg = "stack overflow or erroneous memory access";
      break;

    case SIGBUS:
      exception = &storage_error;
      msg = "SIGBUS: possible stack overflow";
      break;

    case SIGFPE:
      exception = &constraint_error;
      msg = "SIGFPE";
      break;

    default:
      exception = &program_error;
      msg = "unhandled signal";
    }

  Raise_From_Signal_Handler (exception, msg);
}

#ifndef __ia64__
#define HAVE_GNAT_ALTERNATE_STACK 1
/* This must be in keeping with System.OS_Interface.Alternate_Stack_Size.
   It must be larger than MINSIGSTKSZ and hopefully near 2 * SIGSTKSZ.  */
# if 16 * 1024 < MINSIGSTKSZ
#  error "__gnat_alternate_stack too small"
# endif
char __gnat_alternate_stack[16 * 1024];
#endif

#ifdef __XENO__
#include <sys/mman.h>
#include <native/task.h>

RT_TASK main_task;
#endif

void
__gnat_install_handler (void)
{
  struct sigaction act;

#ifdef __XENO__
  int prio;

  if (__gl_main_priority == -1)
    prio = 49;
  else
    prio = __gl_main_priority;

  /* Avoid memory swapping for this program */

  mlockall (MCL_CURRENT|MCL_FUTURE);

  /* Turn the current Linux task into a native Xenomai task */

  rt_task_shadow (&main_task, "environment_task", prio, T_FPU);
#endif

  /* Set up signal handler to map synchronous signals to appropriate
     exceptions.  Make sure that the handler isn't interrupted by another
     signal that might cause a scheduling event!  Also setup an alternate
     stack region for the handler execution so that stack overflows can be
     handled properly, avoiding a SEGV generation from stack usage by the
     handler itself.  */

  act.sa_sigaction = __gnat_error_handler;
  act.sa_flags = SA_NODEFER | SA_RESTART | SA_SIGINFO;
  sigemptyset (&act.sa_mask);

  /* Do not install handlers if interrupt state is "System".  */
  if (__gnat_get_interrupt_state (SIGABRT) != 's')
    sigaction (SIGABRT, &act, NULL);
  if (__gnat_get_interrupt_state (SIGFPE) != 's')
    sigaction (SIGFPE,  &act, NULL);
  if (__gnat_get_interrupt_state (SIGILL) != 's')
    sigaction (SIGILL,  &act, NULL);
  if (__gnat_get_interrupt_state (SIGBUS) != 's')
    sigaction (SIGBUS,  &act, NULL);
  if (__gnat_get_interrupt_state (SIGSEGV) != 's')
    {
#ifdef HAVE_GNAT_ALTERNATE_STACK
      /* Setup an alternate stack region for the handler execution so that
	 stack overflows can be handled properly, avoiding a SEGV generation
	 from stack usage by the handler itself.  */
      stack_t stack;

      stack.ss_sp = __gnat_alternate_stack;
      stack.ss_size = sizeof (__gnat_alternate_stack);
      stack.ss_flags = 0;
      sigaltstack (&stack, NULL);

      act.sa_flags |= SA_ONSTACK;
#endif
      sigaction (SIGSEGV, &act, NULL);
    }

  __gnat_handler_installed = 1;
}

#elif defined(__ANDROID__)

/*******************/
/* Android Section */
/*******************/

#include <signal.h>
#include "sigtramp.h"

#define HAVE_GNAT_ADJUST_CONTEXT_FOR_RAISE

void
__gnat_adjust_context_for_raise (int signo ATTRIBUTE_UNUSED, void *ucontext)
{
  mcontext_t *mcontext = &((ucontext_t *) ucontext)->uc_mcontext;

  /* ARM Bump has to be an even number because of odd/even architecture.  */
  ((mcontext_t *) mcontext)->arm_pc += 2;
}

static void
__gnat_map_signal (int sig,
		   siginfo_t *si ATTRIBUTE_UNUSED,
		   void *ucontext ATTRIBUTE_UNUSED)
{
  struct Exception_Data *exception;
  const char *msg;

  switch (sig)
    {
    case SIGSEGV:
      exception = &storage_error;
      msg = "stack overflow or erroneous memory access";
      break;

    case SIGBUS:
      exception = &constraint_error;
      msg = "SIGBUS";
      break;

    case SIGFPE:
      exception = &constraint_error;
      msg = "SIGFPE";
      break;

    default:
      exception = &program_error;
      msg = "unhandled signal";
    }

  Raise_From_Signal_Handler (exception, msg);
}

static void
__gnat_error_handler (int sig,
		      siginfo_t *si ATTRIBUTE_UNUSED,
		      void *ucontext ATTRIBUTE_UNUSED)
{
  __gnat_adjust_context_for_raise (sig, ucontext);

  __gnat_sigtramp (sig, (void *) si, (void *) ucontext,
		   (__sigtramphandler_t *)&__gnat_map_signal);
}

/* This must be in keeping with System.OS_Interface.Alternate_Stack_Size.  */
char __gnat_alternate_stack[16 * 1024];

void
__gnat_install_handler (void)
{
  struct sigaction act;

  /* Set up signal handler to map synchronous signals to appropriate
     exceptions.  Make sure that the handler isn't interrupted by another
     signal that might cause a scheduling event!  Also setup an alternate
     stack region for the handler execution so that stack overflows can be
     handled properly, avoiding a SEGV generation from stack usage by the
     handler itself.  */

  stack_t stack;
  stack.ss_sp = __gnat_alternate_stack;
  stack.ss_size = sizeof (__gnat_alternate_stack);
  stack.ss_flags = 0;
  sigaltstack (&stack, NULL);

  act.sa_sigaction = __gnat_error_handler;
  act.sa_flags = SA_NODEFER | SA_RESTART | SA_SIGINFO;
  sigemptyset (&act.sa_mask);

  sigaction (SIGABRT, &act, NULL);
  sigaction (SIGFPE,  &act, NULL);
  sigaction (SIGILL,  &act, NULL);
  sigaction (SIGBUS,  &act, NULL);
  act.sa_flags |= SA_ONSTACK;
  sigaction (SIGSEGV, &act, NULL);

  __gnat_handler_installed = 1;
}

#else

/* For all other versions of GNAT, the handler does nothing.  */

/*******************/
/* Default Section */
/*******************/

void
__gnat_install_handler (void)
{
  __gnat_handler_installed = 1;
}

#endif

/*********************/
/* __gnat_init_float */
/*********************/

/* This routine is called as each process thread is created, for possible
   initialization of the FP processor.  This version is used under INTERIX
   and WIN32.  */

#if defined (_WIN32) || defined (__INTERIX) \
  || defined (__Lynx__) || defined(__NetBSD__) || defined(__FreeBSD__) \
  || defined (__OpenBSD__)

#define HAVE_GNAT_INIT_FLOAT

void
__gnat_init_float (void)
{
#if defined (__i386__) || defined (i386) || defined (__x86_64)

  /* This is used to properly initialize the FPU on an x86 for each
     process thread.  */

  asm ("finit");

#endif  /* Defined __i386__ */
}
#endif

#ifndef HAVE_GNAT_INIT_FLOAT

/* All targets without a specific __gnat_init_float will use an empty one.  */
void
__gnat_init_float (void)
{
}
#endif

/***********************************/
/* __gnat_adjust_context_for_raise */
/***********************************/

#ifndef HAVE_GNAT_ADJUST_CONTEXT_FOR_RAISE

/* All targets without a specific version will use an empty one.  */

/* Given UCONTEXT a pointer to a context structure received by a signal
   handler for SIGNO, perform the necessary adjustments to let the handler
   raise an exception.  Calls to this routine are not conditioned by the
   propagation scheme in use.  */

void
__gnat_adjust_context_for_raise (int signo ATTRIBUTE_UNUSED,
				 void *ucontext ATTRIBUTE_UNUSED)
{
  /* We used to compensate here for the raised from call vs raised from signal
     exception discrepancy with the GCC ZCX scheme, but this now can be dealt
     with generically in the unwinder (see GCC PR other/26208).  This however
     requires the use of the _Unwind_GetIPInfo routine in raise-gcc.c, which
     is predicated on the definition of HAVE_GETIPINFO at compile time.  Only
     the VMS ports still do the compensation described in the few lines below.

     *** Call vs signal exception discrepancy with GCC ZCX scheme ***

     The GCC unwinder expects to be dealing with call return addresses, since
     this is the "nominal" case of what we retrieve while unwinding a regular
     call chain.

     To evaluate if a handler applies at some point identified by a return
     address, the propagation engine needs to determine what region the
     corresponding call instruction pertains to.  Because the return address
     may not be attached to the same region as the call, the unwinder always
     subtracts "some" amount from a return address to search the region
     tables, amount chosen to ensure that the resulting address is inside the
     call instruction.

     When we raise an exception from a signal handler, e.g. to transform a
     SIGSEGV into Storage_Error, things need to appear as if the signal
     handler had been "called" by the instruction which triggered the signal,
     so that exception handlers that apply there are considered.  What the
     unwinder will retrieve as the return address from the signal handler is
     what it will find as the faulting instruction address in the signal
     context pushed by the kernel.  Leaving this address untouched looses, if
     the triggering instruction happens to be the very first of a region, as
     the later adjustments performed by the unwinder would yield an address
     outside that region.  We need to compensate for the unwinder adjustments
     at some point, and this is what this routine is expected to do.

     signo is passed because on some targets for some signals the PC in
     context points to the instruction after the faulting one, in which case
     the unwinder adjustment is still desired.  */
}

#endif

#ifdef __cplusplus
}
#endif
