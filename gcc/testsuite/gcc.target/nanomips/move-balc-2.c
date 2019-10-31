/* { dg-require-effective-target nonpic } */
/* { dg-options "-mcmodel=auto -m32 -march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void foo (int);

int
bar (void)
{
  foo(0);
  return 0;
}

/* { dg-final { scan-assembler "\tmove.balc\t" } } */
