/* PR rtl-optimization/16114 */
void *f1 (void);
int f2 (void *x, int *y);
unsigned long f3 (void *x, const void *y, unsigned long z);
unsigned long *f4 (unsigned long *x);
void f5 (void *x, int y);

long
foo (void *x)
{
  int a = 0, c, g = 0;
  void *b;
  unsigned long *d = 0, e = 172, f;

  b = f1 ();
retry:
  a = f2 (b, &c);
  if (a == -11)
    goto retry;
  else if (a)
    { a = -11; goto out; }

  g = 1;
  if (x)
    {
      if (f3 (&f, x, sizeof (f)))
        { a = -14;  goto out; }
      if ((d = f4 (&f)) && (*d & 4))
        d = 0;
      if (!d)
        { a = -22; goto out; }
    }
  else
    asm volatile ("" : : "r" (e));

out:
  if (a)
    f5 (b, g);

  return a;
}
