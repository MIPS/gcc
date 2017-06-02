/* { dg-options "-mabi=p32 -fpeephole2 isa_rev>=6 -mnanomips -mxlp" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void foo (int);

int
bar (int s, int t)
{
  foo(t);
  return s;
}

/* { dg-final { scan-assembler "\tmove.balc\t" } } */
