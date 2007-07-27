// backtrace.h - Fallback backtrace implementation. ARM implementation.

/* Copyright (C) 2005, 2006  Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */

#ifndef __SYSDEP_BACKTRACE_H__
#define __SYSDEP_BACKTRACE_H__

#include <java-stack.h>

extern "C"
{
/* Unwind through the call stack calling TRACE_FN with STATE for every stack
   frame.  Returns the reason why the unwinding was stopped.  */
#ifdef __ARM_EABI_UNWINDER__
#undef _Unwind_GetIPInfo
#define _Unwind_GetIPInfo(ctx,ip_before_insn) \
  (*(ip_before_insn) = 1, (ctx)->ret_addr)

#undef _Unwind_GetRegionStart
#define _Unwind_GetRegionStart(ctx) \
  0

#undef _Unwind_Backtrace
#define _Unwind_Backtrace(trace_fn,state_ptr) \
  (fallback_backtrace (trace_fn, state_ptr))

#define _Unwind_FindEnclosingFunction(PC) \
  (PC)

_Unwind_Reason_Code
fallback_backtrace (_Unwind_Reason_Code (*)(struct _Unwind_Context*, void*), _Jv_UnwindState *)
#else
_Unwind_Reason_Code
fallback_backtrace (_Unwind_Trace_Fn, _Jv_UnwindState *)
#endif
{
  return _URC_NO_REASON;
}
}
#endif
