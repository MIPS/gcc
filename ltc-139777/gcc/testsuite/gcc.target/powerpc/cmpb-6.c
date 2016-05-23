/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-do run "-mcpu=power6" } */
/* { dg-require-effective-target lp64 } */

/* This test should succeed on only 64-bit configurations.  */
#include <altivec.h>

int int cmpb (int arg1, int arg2)
{
  return __builtin_cmpb (arg1, arg2);
}

int main (int argc, char *argv[])
{
  long long a, b;

  a = 0x0102030405060708L;
  b = 0x0102030405060503L;

  if (cmpb (a, b) != 0xffffffffffff0000)
    return -1;
  else
    return 0;
}

