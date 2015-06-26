/* Test if we generate code for DSP rev. 1.  */
/* { dg-compile } */
/* { dg-options "-mips32r2" } */

/* { dg-final { scan-assembler-times "\.set\tdsp" 3 } } */
/* { dg-final { scan-assembler-times "addq.ph" 3 } } */

typedef short v2i16 __attribute__ ((vector_size (4)));

v2i16 a[10], b[10], c[10];

__attribute__ ((target("dsp")))
void
test_dsp ()
{
  c[0] = a[0] + b[0];
}

__attribute__ ((target("dsp")))
void
test_dsp2 (v2i16 *a, v2i16 *b, v2i16 *c)
{
  *c = *a + *b;
}

__attribute__ ((target("dsp")))
v2i16
test_dsp3 (v2i16 *a, v2i16 *b)
{
  return *a + *b;
}
