/* { dg-options "-mabi=p32 -fpeephole2 isa_rev>=7" } */

int bar (int);
void bar2 (int);

int
foo (void)
{
  int a = 5;
  bar2 (a);
  return bar (2);
}

/* { dg-final { scan-assembler "\tsave\t" } } */
/* { dg-final { scan-assembler "\trestore\t" } } */
