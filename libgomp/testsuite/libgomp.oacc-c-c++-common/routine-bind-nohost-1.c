/* Test the bind and nohost clauses for OpenACC routine directive.  */

/* TODO.  Function inlining and the OpenACC bind clause do not yet get on well
   with one another.
   { dg-additional-options "-fno-inline" } */

/* TODO.  C works, but for C++ we get: "lto1: internal compiler error: in
   ipa_propagate_frequency".
   { dg-xfail-if "TODO" { *-*-* } } */

#include <openacc.h>

/* "MINUS_TWO" is the device variant for function "TWO".  Similar for "THREE",
   and "FOUR".  Exercising different variants for declaring routines.  */

#pragma acc routine nohost
extern int MINUS_TWO(void);

int MINUS_TWO(void)
{
  if (!acc_on_device(acc_device_not_host))
    __builtin_abort();
  return -2;
}

extern int TWO(void);
#pragma acc routine (TWO) bind(MINUS_TWO)

int TWO(void)
{
  if (acc_on_device(acc_device_not_host))
    __builtin_abort();
  return 2;
}


#pragma acc routine nohost
int MINUS_THREE(void)
{
  if (!acc_on_device(acc_device_not_host))
    __builtin_abort();
  return -3;
}

#pragma acc routine bind(MINUS_THREE)
extern int THREE(void);

int THREE(void)
{
  if (acc_on_device(acc_device_not_host))
    __builtin_abort();
  return 3;
}


/* Due to using a string in the bind clause, we don't need "MINUS_FOUR" in
   scope here.  */
#pragma acc routine bind("MINUS_FOUR")
int FOUR(void)
{
  if (acc_on_device(acc_device_not_host))
    __builtin_abort();
  return 4;
}

extern int MINUS_FOUR(void);
#pragma acc routine (MINUS_FOUR) nohost

int MINUS_FOUR(void)
{
  if (!acc_on_device(acc_device_not_host))
    __builtin_abort();
  return -4;
}


int main()
{
  int x2, x3, x4;

#pragma acc parallel copyout(x2, x3, x4) if(0)
  {
    x2 = TWO();
    x3 = THREE();
    x4 = FOUR();
  }
  if (x2 != 2 || x3 != 3 || x4 != 4)
    __builtin_abort();

#pragma acc parallel copyout(x2, x3, x4)
  {
    x2 = TWO();
    x3 = THREE();
    x4 = FOUR();
  }
#ifdef ACC_DEVICE_TYPE_host
  if (x2 != 2 || x3 != 3 || x4 != 4)
    __builtin_abort();
#else
  if (x2 != -2 || x3 != -3 || x4 != -4)
    __builtin_abort();
#endif

  return 0;
}
