/* Have to enable optimizations, as otherwise builtins won't be expanded.  */
/* { dg-additional-options "-O -fdump-rtl-expand" } */

/* Duplicate parts of libgomp/openacc.h, because we can't include it here.  */

#if __cplusplus
extern "C" {
#endif

typedef enum acc_device_t { acc_device_X = 123 } acc_device_t;
extern int acc_on_device (int);

#if __cplusplus
}
#endif

int
f (void)
{
  const acc_device_t dev = acc_device_X;
  return acc_on_device (dev);
}

/* With -fopenacc, we're expecting the builtin to be expanded, so no calls.
   TODO: in C++, the use of enum acc_device_t for acc_on_device's parameter
   perturbs expansion as a builtin, which expects an int parameter.  It's fine
   when changing acc_device_t to plain int, but that's not necessarily what a
   user will be doing.

   { dg-final { scan-rtl-dump-times "\\\(call \[^\\n\]* acc_on_device" 0 "expand" { xfail c++ } } } */
