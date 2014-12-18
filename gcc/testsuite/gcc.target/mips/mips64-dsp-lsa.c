/* Test MIPS64 DSP instructions - should use LSA instead of LHX */
/* { dg-do compile } */
/* { dg-options "-mgp64 -mdsp (HAS_LSA)" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

/* { dg-final { scan-assembler "\tlsa\t" } } */

NOMIPS16 signed long long test (signed long long *a, int index)
{
  return a[index];
}
