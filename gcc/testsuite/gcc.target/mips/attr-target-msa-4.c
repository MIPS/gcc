/* Test if the built-in functions are enabled for the target attribute.  */
/* { dg-compile } */
/* { dg-require-effective-target mips_msa } */
/* { dg-options "-mips32r2 -mno-msa -mfpxx" } */
#include <msa.h>

#pragma GCC push_options
#pragma GCC target("msa")
v4i32
test_msa (void *mem)
{
  return __builtin_msa_ld_w (mem, 0);
}
#pragma GCC pop_options

__attribute__ ((target("msa")))
v4i32
test_msa2 (void *mem)
{
  return __builtin_msa_ld_w (mem, 0);
}
