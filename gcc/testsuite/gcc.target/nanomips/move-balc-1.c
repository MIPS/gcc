/* { dg-require-effective-target nonpic } */
/* { dg-options "-mcmodel=auto -m32 -fpeephole2 -march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

/* This test fails only for the large model. Ideally, we would run this test
   for both the automatic and medium model, but we'll have to stick to forcing
   the automatic model for now. The former can be achieved by modifying
   nanomips.exp.  */

void foo (int);

int
bar (int s, int t)
{
  foo(t);
  return s;
}

/* { dg-final { scan-assembler "\tmove.balc\t" } } */
