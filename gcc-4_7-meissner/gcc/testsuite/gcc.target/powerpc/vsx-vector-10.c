/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-options "-mvsx -Ofast -ftree-vectorize -munaligned-vector -std=gnu99" } */

/* Check whether the compiler does do unaligned vector loads if desired.  */

#include <stddef.h>

/* By default, the compiler will do float and int vectors using unaligned
   loads, make sure they don't under switch control.  */

#ifndef SIZE
#define SIZE 1024
#endif

void
float_add (float * __restrict__ a, float * __restrict__ b, float * __restrict__ c)
{
  size_t i;

  for (i = 0; i < SIZE; i++)
    a[i] = b[i] + c[i];
}

/* { dg-final { scan-assembler "vperm" } } */
/* { dg-final { scan-assembler "lvsr" } } */
