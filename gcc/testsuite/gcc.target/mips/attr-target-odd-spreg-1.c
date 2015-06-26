/* { dg-compile } */
/* { dg-options "-mabi=32 -mfpxx" } */

/* { dg-final { scan-assembler "\.module\tnooddspreg" } } */
/* { dg-final { scan-assembler "\.set\toddspreg" } } */
/* { dg-final { scan-assembler "add.s\t\\\$f0,\\\$f1,\\\$f0" } } */

float a;

__attribute__ ((target("odd-spreg")))
float
test_oddspreg ()
{
  return a + 1.0f;
}
