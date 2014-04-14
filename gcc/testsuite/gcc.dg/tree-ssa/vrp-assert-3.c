/* { dg-do compile } */
/* { dg-options "-O2 -Wall -fdump-tree-optimized" } */

int g(int);
void link_error() __attribute__((noreturn));
int f(int sum)
{
  _Bool c = sum != 0;
  int d = c;
  g(d);
  if (d != 0)
    return 0 + g(0);
  if (c != 0)
    link_error ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "link_error" 0 "optimized"  } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
