/* { dg-options "-O3 -march=armv8-a+sve -fdump-tree-vect-details" } */

int a[1024];
int b[1024];
int c[1024];

void
f1 (int n)
{
  int i;
  for (i = 0; i < n; i += 2)
    {
      a[i] = 1;
      a[i + 1] = 2;
    }
}

int
f2 (int n)
{
  int i;
  for (i = 0; i < n; ++i)
    a[i] = b[i * 2] + b[i * 2 + 1];
}

int
f3 (int n)
{
  int i;
  for (i = 0; i < n; ++i)
    a[i] = b[i * 2];
}

double
f5 (double *a, int n)
{
  int i;
  double x = 100;
  for (i = 0; i < n; ++i)
    x = __builtin_fmin (x, a[i]);
  return x;
}

/* For f1, which we treat as a contiguous store rather than an ST2.  */
/* { dg-final { scan-tree-dump-times "Can't use a fully-masked loop because an access is grouped" 1 "vect" } } */
/* For f3, where we need peeling for gaps.  */
/* { dg-final { scan-tree-dump-times "Can't use a fully-masked loop because peeling for gaps is required" 1 "vect" } } */
