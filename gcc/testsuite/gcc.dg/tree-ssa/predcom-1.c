/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-pcom-details" } */

void abort (void);

int fib[1000];

void count_fib(void)
{
  int i;

  fib[0] = 0;
  fib[1] = 1;
  for (i = 2; i < 1000; i++)
    fib[i] = fib[i-1] + fib[i - 2];
}

int avg[1000];

void count_averages(void)
{
  int i;

  for (i = 1; i < 999; i++)
    avg[i] = (fib[i - 1] + fib[i] + fib[i + 1]) / 3;
}

int main(void)
{
  count_fib ();
  count_averages ();

  if (fib[19] != 4181 || avg[19] != 4510)
    abort ();

  return 0;
}

/* Verify that both loops were transformed and unrolled.  */
/* { dg-final { scan-tree-dump-times "Unrolling 2 times." 2 "pcom"} } */
/* { dg-final { cleanup-tree-dump "pcom" } } */
