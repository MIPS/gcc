/* { dg-options "-m32 isa_rev>=6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int foo(int status)
{
  int s = 0;
  if (status == 1) s=1;
  if (status == 3) s=3;
  if (status == 4) s=4;
  return s;
}

/* { dg-final { scan-assembler "\tbeqic\t\\\$a\[a0-9\]+,\[1|3|4\],\[\\\$.\]L" } } */
