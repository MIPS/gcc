/* Check that we can generate the MOVEP and MOVEP[REV] instructions.  */
/* { dg-options "-march=32r6 -fpeephole2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

long long bar (long long, long long);

long long
foo (long long n, long long a)
{
  long long i, j;

  i = bar (n, a);
  j = bar (n, a);
  return i + j;
}
/* { dg-final { scan-assembler "\tmovep\t\\\$a\[0-3\],\\\$\[at0-3\]+,\\\$zero|at|\[av0-3\]+|s\[0-7\],\\\$zero|at|\[av0-3\]+|s\[0-7\]" } } */
/* { dg-final { scan-assembler "\tmovep\t\\\$zero|at|\[av0-3\]+|s\[0-7\],\\\$zero|at|\[av0-3\]+|s\[0-7\],\\\$a\[0-3\],\\\$\[at0-3\]+" } } */
