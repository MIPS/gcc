/* Test if we generate MSA code using #pragma GCC target and if specific
   macros are defined only after #pragma GCC.  */
/* { dg-compile } */
/* { dg-require-effective-target mips_msa } */
/* { dg-options "-mips32r2 -mno-msa -mfpxx" } */

/* { dg-final { scan-assembler "\.set\tmsa" } } */
/* { dg-final { scan-assembler-times "addv.w" 1 } } */
#include <msa.h>

#ifdef __mips_msa
#error "__mips_msa should not be defined here"
#endif

#pragma GCC push_options
#pragma GCC target("msa")
#ifndef __mips_msa
#error "__mips_msa should be defined after #pragma GCC target"
#endif
#pragma GCC pop_options

v4i32 a, b, c;
v4i32
test_nomsa ()
{
  c = a + b;
}

#pragma GCC push_options
#pragma GCC target ("msa")
v4i32 aa, bb, cc;
v4i32
test_msa2 ()
{
  cc = aa + bb;
}
#pragma GCC pop_options
