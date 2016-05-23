/* { dg-do run { target { powerpc*-*-* } } } */
/* { dg-options "-mcpu=power6" } */

/* This test should succeed on both 32- and 64-bit configurations.  */
#include <altivec.h>

int int cmpb_32 (int arg1, int arg2)
{
  return __builtin_cmpb_32 (arg1, arg2);
}

int main (int argc, char *argv[])
{
  int i, j;

  i = 0x01020304;
  j = 0x01050304;

  if (cmpb_32 (i, j) != 0xff00ffff)
    return -2;
  else
    return 0;
}



