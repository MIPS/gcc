/* { dg-do compile } */
/* { dg-options "-mgp32 -mhard-float" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "\tmthc1\t" } } */
/* { dg-final { scan-assembler "\tmfhc1\t" } } */

double func1 (long long a)
{
  return a;
}

long long func2 (double b)
{
  return b;
}
