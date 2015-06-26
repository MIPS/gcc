/* Test if DSP code is generated for #pragma GCC target.  */
/* { dg-compile } */
/* { dg-options "-mips32r2 -mno-dsp" } */

/* { dg-final { scan-assembler-times "\.set\tdsp" 2 } } */
/* { dg-final { scan-assembler-times "addq.ph" 2 } } */

typedef short v2i16 __attribute__ ((vector_size (4)));

#pragma GCC push_options
#pragma GCC target("dsp")
v2i16 a, b, c;
__attribute__ ((target("dsp")))
void
test_dsp ()
{
  c = a + b;
}

v2i16
test_dsp2 (v2i16 x)
{
  return a + x;
}
#pragma GCC pop_options
