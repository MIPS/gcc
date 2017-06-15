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
/* { dg-final { scan-assembler "\tmovep\t\\\$\[4-7\],\\\$\[5-8\],\\\$\[0-7\]|(1\[6-9\])|(2\[0-3\]),\\\$\[0-7\]|(1\[6-9\])|(2\[0-3\])" } } */
/* { dg-final { scan-assembler "\tmovep\t\\\$\[0-7\]|(1\[6-9\])|(2\[0-3\]),\\\$\[0-7\]|(1\[6-9\])|(2\[0-3\]),\\\$\[4-7\],\\\$\[5-8\]" } } */
