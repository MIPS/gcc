/* { dg-do compile } */
/* { dg-options "-mno-micromips -mno-mips16 (HAS_LSA)" } */
/* { dg-additional-options "-mfix-i6500" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int
foo (int ***p)
{
  return ***p;
}

float
bar (float *p)
{
  return *p;
}

/* { dg-final { scan-assembler-times "ehb" 2 } } */