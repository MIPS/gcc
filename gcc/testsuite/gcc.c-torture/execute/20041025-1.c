extern void abort (void);

void
foo (int a, int b, int c, int d, int e, void **f)
{
  char g[4096];
  if (f == 0 || *f != 0)
    abort ();
}

int
main (void)
{
  void *x = 0;
  foo (0, 1, 2, 3, 4, &x);
  return 0;
}
