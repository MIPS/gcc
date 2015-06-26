/* Test if we error out on the built-in functions only in function with
   specified target options.  */
/* { dg-compile } */
/* { dg-require-effective-target mips_msa } */
/* { dg-options "-mips32r2 -mno-msa -mfpxx" } */
/* { dg-skip-if "" { *-*-* } { "-fuse-linker-plugin" } { "" } } */
#include <msa.h>

#pragma GCC push_options
#pragma GCC target("msa")
v4i32
test_msa (void *mem)
{
  return __builtin_msa_ld_w (mem, 0);
}
#pragma GCC pop_options

//__attribute__ ((target("msa")))
v4i32
test_nomsa (void *mem)
{
  return __builtin_msa_ld_w (mem, 0); /* { dg-error "requires target pragma or attribute" } */
}

__attribute__ ((target("msa")))
v4i32
test_msa2 (void *mem)
{
  return __builtin_msa_ld_w (mem, 0);
}
