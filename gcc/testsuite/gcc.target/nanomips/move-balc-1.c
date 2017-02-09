/* { dg-require-effective-target nonpic } */
/* { dg-options "-m32 -fpeephole2 -march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void foo (int);

int
bar (int s, int t)
{
  foo(t);
  return s;
}

/* { dg-final { scan-assembler "\tmove.balc\t" } } */
