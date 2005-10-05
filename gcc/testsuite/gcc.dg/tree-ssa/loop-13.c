/* A test for ending condition selection.  */

/* { dg-options "-O2 -fdump-tree-vars" } */

void foo(int);
void bar(void);

void xxx1(void)
{
  int i;

  /* i != 100  */
  for (i = 0; i < 100; i++)
    foo (i);
}

void xxx2(int n)
{
  int i;

  /* n > i  */
  for (i = 0; i < n; i++)
    foo (i);
}

void xxx3(int n)
{
  int i;

  /* i >= 0  */
  for (i = 0; i < n; i++)
    foo (n - i - 1);
}

void xxx4(int n)
{
  int i;

  /* i != 0  */
  for (i = 0; i < n; i++)
    foo (n - i);
}

void xxx5(int n)
{
  int i;

  /* i != 0  */
  for (i = 0; i < n; i++)
    bar ();
}

/* { dg-final { scan-tree-dump-times "!= 100" 1 "vars" } } */
/* { dg-final { scan-tree-dump-times "!= 0" 2 "vars" } } */
/* { dg-final { scan-tree-dump-times "n > i" 1 "vars" } } */
/* { dg-final { scan-tree-dump-times ">= 0" 1 "vars" } } */

/* { dg-final { cleanup-tree-dump "vars" } } */
