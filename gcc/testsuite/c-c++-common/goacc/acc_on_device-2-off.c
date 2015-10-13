/* Have to enable optimizations, as otherwise builtins won't be expanded.  */
/* { dg-additional-options "-O -fdump-rtl-expand -fno-openacc" } */

/* Duplicate parts of libgomp/openacc.h, because we can't include it here.  */

#include "openacc.h"

int
f (void)
{
  const acc_device_t dev = acc_device_X;
  return acc_on_device (dev);
}

/* Without -fopenacc, we're expecting one call.
   { dg-final { scan-rtl-dump-times "\\\(call \[^\\n\]* acc_on_device" 1 "expand" } } */

