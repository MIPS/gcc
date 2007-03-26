/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-lim-details" } */

int x;
int a[100];

struct a
{
  int X;
  int Y;
};

struct a arr[100];

void bla(void);

void foo (struct a *A, unsigned b)
{
  unsigned i;

  /* We should perform store motion here.  */
  for (x = 0; x < 100; x++)
    a[x] = x;

  /* But not here.  */
  for (x = 0; x < 100; x++)
    bla ();

  /* But we should here (using base + offset analysis).  */
  for (i = 0; i < 100; i++)
    {
      A[5].X += i;
      A[5].Y += i;
    }

  /* And here.  */
  for (i = 0; i < 100; i++)
    {
      arr[b+8].X += i;
      arr[b+9].X += i;
    }

  /* And here as well.  */
  for (i = 0; i < 100; i++)
    {
      A[b].X += i;
      A[b+1].Y += i;
    }
}

/* { dg-final { scan-tree-dump-times "Executing store motion of" 7 "lim" } } */

/* { dg-final { cleanup-tree-dump "lim" } } */
