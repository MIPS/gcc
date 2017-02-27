/* { dg-do run } */
/* { dg-require-effective-target vect_int } */

#define N 100

void __attribute__ ((noinline, noclone))
foo (int *a, int *b, int *c)
{
  for (int i = 0; i < N; ++i)
    {
      a[i * 2] = i;
      a[i * 2 + 1] = i + 100;
      b[i * 2] += 1;
      b[i * 2 + 1] += 2;
      c[i] = a[i * 2] + a[i * 2 + 1];
    }
}

int
main (void)
{
  int a[N * 2], b[N * 2], c[N];

  for (int i = 0; i < N; ++i)
    {
      a[i * 2] = 44;
      a[i * 2 + 1] = 66;
      b[i * 2] = i * 5;
      b[i * 2 + 1] = i * 7;
      c[i] = 77;
      asm volatile ("");
    }
  foo (a, b, c);
  for (int i = 0; i < N; ++i)
    {
      if (a[i * 2] != i
	  || a[i * 2 + 1] != i + 100
	  || b[i * 2] != i * 5 + 1
	  || b[i * 2 + 1] != i * 7 + 2
	  || c[i] != i * 2 + 100)
	__builtin_abort ();
      asm volatile ("");
    }

  for (int i = 0; i < N; ++i)
    {
      a[i * 2] = 44;
      a[i * 2 + 1] = 66;
      asm volatile ("");
    }
  foo (a, a, c);
  for (int i = 0; i < N; ++i)
    {
      if (a[i * 2] != i + 1
	  || a[i * 2 + 1] != i + 102
	  || c[i] != i * 2 + 103)
	__builtin_abort ();
      asm volatile ("");
    }

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
