/* { dg-do compile } */
/* { dg-options "-mabi=64 -march=mips64r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-Os" } } */

NOMIPS16 unsigned int
foo (unsigned int x, unsigned int y, unsigned int z)
{
  if (x == 1)
    y ^= z;

  return y;
}

/* { dg-final { scan-assembler-times "seleqz" 1 } } */
/* { dg-final { scan-assembler-times "selnez" 1 } } */
