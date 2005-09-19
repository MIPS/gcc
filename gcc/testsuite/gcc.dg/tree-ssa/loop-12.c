/* A test for loop reversal.  */

/* { dg-do compile } */
/* { dg-options "-O2 -freverse-loops -fdump-tree-revloops-details" } */

int a[1000], b[1000];

void bar(void);
void bla(int);

void foo1 (int n)
{
  int i;

  /* This loop can be reversed, but there is no reason for doing it.  */
  for (i = 0; i < n; i++)
    a[i] = b[n - i - 1];
}

void foo2 (int n)
{
  int i;

  /* This loop can be reversed, and we get a better code if we do it
     (but the array is traversed backwards -- this test may need some
     tweaking if some architecture defines PREFER_PASSING_MEMORY_FORWARDS).  */
  for (i = 0; i < n; i++)
    a[i] = b[i];
}

void foo3 (int n)
{
  int i;

  /* This loop can be reversed, but would make things worse.  */
  for (i = 0; i < n; i++)
    a[n - i - 1] = n - i;
}

void foo4 (int n)
{
  int i;

  /* This loop can be reversed, but it it is doubtful whether it is
     profitable.  */
  for (i = 0; i < n; i++)
    a[n - i - 1] = i;
}

void foo4a (int n)
{
  int i;

  /* This loop can be reversed, and it is profitable.  */
  for (i = 1000; i > n; i--)
    a[i] = i;
}

/* The following three loops cannot be reversed.  */
void foo5 (int n)
{
  int i;

  for (i = 0; i < n; i++)
    a[i + 1] = a[i];
}

void foo6 (int n)
{
  int i;

  for (i = 0; i < n; i++)
    a[i] = a[i + 1];
}

void foo7 (int n)
{
  int i;

  for (i = 0; i < n; i++)
    bla(i);
}

void foo8 (int n)
{
  int i;

  /* The following loop can be reversed, but we fail to detect it.
     Nevertheless, reversing it is not useful anyway, we may just
     create a countdown iv.  */
  for (i = 0; i < n; i++)
    bar ();
}

void foo9 (int n)
{
  int i;

  /* The following loop cannot be reversed.  */
  for (i = 0; i < 100; i++)
    {
      a[7] = i;
      a[10] = i;
      b[i] = i;
    }
}

/* { dg-final { scan-tree-dump-times "SUCCESS: may be reversed" 5 "revloops" } } */
/* { dg-final { scan-tree-dump-times "SUCCESS: profitable to reverse" 2 "revloops" } } */
/* { dg-final { scan-tree-dump-times "FAILED: dependence check failed" 5 "revloops" } } */

/* { dg-final { cleanup-tree-dump "revloops" } } */
