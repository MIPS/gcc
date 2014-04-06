/* { dg-do run } */
/* { dg-options "-O2 -Wall -fdump-tree-optimized" } */

void link_failure (void);

void g(int a) __attribute__((noclone, noinline));
void g(int a)
{
  asm("");
}
void f(int ciu_sum) __attribute__((noclone, noinline));
void f(int ciu_sum)
{
   int t1;
  _Bool t = ciu_sum != 0;
  int __r1 = t;
  g(__r1);
  if (__r1)
      t1 = ciu_sum == 0;
  else
      t1 =  0;
  if (t1 != 0)
    link_failure ();
}

int main(void)
{
  f(1);
  return 0;
}

/* The call to link_error should be removed by VRP. */
/* { dg-final { scan-tree-dump-times "link_error" 0 "optimized"  } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
