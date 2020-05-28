/* { dg-do compile } */
/* { dg-options "-mabi=64 -march=mips64r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } } */

NOMIPS16 unsigned int
foo (unsigned int x, unsigned int y, unsigned z)
{
  if (x == z)
    y ^= 0xabcd;

  return y;
}

/* { dg-final { scan-assembler-times "seleqz" 1 } } */
/* { dg-final { scan-assembler-not "selnez" } } */
