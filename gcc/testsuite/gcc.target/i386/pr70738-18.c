/* { dg-do run } */
/* { dg-options "-O2" } */

extern void abort ();

__attribute__ ((target("general-regs-only")))
int
cal (int a, int b)
{
  int sum = a + b;
  int sum1 = a * b;
  return (a + b + sum + sum1);
}

int
main (int argc, char **argv)
{
  int ret = cal (1, 2);

  if (ret != 8)
    abort ();

  return 0;
}
