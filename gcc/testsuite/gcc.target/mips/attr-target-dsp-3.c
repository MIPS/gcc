/* Test if the built-in function are enabled for DSP per function basis.  */
/* { dg-compile } */
/* { dg-options "-mips32r2 -mno-dsp" } */

/* { dg-final { scan-assembler-times "\.set\tdsp" 2 } } */
/* { dg-final { scan-assembler-times "subu.qb" 1 } } */
/* { dg-final { scan-assembler-times "addsc" 1 } } */

typedef signed char v4i8 __attribute__ ((vector_size (4)));

#pragma GCC push_options
#pragma GCC target("dsp")
v4i8 a, b, c;
void
test_dsp ()
{
  c = __builtin_mips_subu_qb (a, b);
}
#pragma GCC pop_options

__attribute__ ((target("dsp")))
int
test_dsp2 (int a, int b)
{
  return __builtin_mips_addsc (a, b);
}
