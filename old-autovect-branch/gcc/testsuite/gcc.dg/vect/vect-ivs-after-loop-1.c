/* { dg-require-effective-target vect_int } */
/* { dg-do compile } */


static int A[1024];
static int B[1024];

int
foo_1 (void)
{
  int i;

  for (i = 16; i < 160; i++)
    {
      A[i] = B[i];
    }

  return i;
}

int
foo_2 (void)
{
  int i;
  int j = 10;

  for (i = 16; i < 116; i++)
    {
      A[i] = B[i];
      j = j + 5;
    }

  return j;
}

int
foo_3 (void)
{
  int i;
  int j = 10;

  for (i = 0; i < 64; i++)
    {
      j = i + 5;
      A[i] = B[i];
    }

  return j;
}

int
foo_4 (unsigned n)
{
  int i;
  int j = 10;

  for (i = 0; i < n; i++)
    {
      j = i + 5;
      A[i] = B[i];
    }

  return j;
}

int
foo_5 (unsigned n, int k)
{
  int i;
  int j = 10;

  for (i = 0; i < n; i++)
    {
      j = i + k;
      A[i] = B[i];
    }

  return j;
}

int
foo_6 (unsigned n, int k)
{
  unsigned i = 0;
  int j = 10;

  do {
      A[i] = B[i];
      j = i + k;
      i = i + 1;
  } while (i < 1024);

  return j + 1;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 6 "vect" } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
