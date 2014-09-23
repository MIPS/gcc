/* Have to enable optimizations, as otherwise builtins won't be expanded.  */
/* { dg-additional-options "-O -fdump-rtl-expand -fno-openacc" } */

typedef enum acc_device_t { acc_device_X = 123 } acc_device_t;
extern int acc_on_device (acc_device_t);

int
f (void)
{
  const int dev = acc_device_X;
  return acc_on_device (dev);
}

/* Without -fopenacc, we're expecting one call.
   { dg-final { scan-rtl-dump-times "\\\(call \[^\\n\]*\\\"acc_on_device" 1 "expand" } } */

/* { dg-final { cleanup-rtl-dump "expand" } } */
