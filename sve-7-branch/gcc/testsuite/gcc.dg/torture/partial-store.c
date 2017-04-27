/* { dg-do run } */

extern void abort (void);

int a[256], b[256], c[256];

void __attribute__((noinline, noclone))
foo (int n)
{
  int i;
  for (i = 0; i < 255; ++i)
    if (a[i] == 0)
      c[i] = b[i];
}

int
main (void)
{
  int i;
  for (i = 0; i < 256; ++i)
    {
      a[i] = (i & 3) == 0;
      b[i] = i;
    }
  foo (255);
  for (i = 0; i < 255; ++i)
    if (c[i] != (i & 3 ? b[i] : 0))
      abort ();
  if (c[i])
    abort ();
  return 0;
}
