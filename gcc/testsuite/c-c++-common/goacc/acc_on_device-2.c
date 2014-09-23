/* Have to enable optimizations, as otherwise builtins won't be expanded.  */
/* { dg-additional-options "-O -fdump-rtl-expand" } */

typedef enum acc_device_t { acc_device_X = 123 } acc_device_t;
extern int acc_on_device (acc_device_t);

int
f (void)
{
  const int dev = acc_device_X;
  return acc_on_device (dev);
}

/* With -fopenacc, we're expecting the builtin to be expanded, so no calls.
   { dg-final { scan-rtl-dump-times "\\\(call \[^\\n\]*\\\"acc_on_device" 0 "expand" } } */

/* { dg-final { cleanup-rtl-dump "expand" } } */
