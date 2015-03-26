/* { dg-do run } */
/* { dg-options "-mframe-header-opt" } */
/* Testing -mframe-header-opt optimization option.  */

int __attribute__((noinline))
B (int x)
{
  return x + 3;
}

/* We are sure that B is not using its incoming stack frame so we can skip
   its allocation.  */
int __attribute__((noinline))
A (int x)
{
  return B (x) + 2;
}

int
main (void)
{
  int a;
  void *volatile sp1, *volatile sp2;
  register void *sp asm ("$sp");
  sp1 = sp;
  a = A (5);
  sp2 = sp;
  return !(a == 10 && sp1 == sp2);
}

