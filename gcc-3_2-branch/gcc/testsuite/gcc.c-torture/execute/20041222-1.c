extern void abort (void);
extern void exit (int);

struct S
{
  void *a;
  unsigned int b;
};

void
__attribute__((noinline))
bar (struct S *x)
{
  if (x->b != 2)
    abort ();
}

void
__attribute__((noinline))
foo (struct S *x)
{
  if (! x->a)
    {
      struct S *y, *z;
      y = x;
      if (y)
	++y->b;
      z = x;
      if (z)
	++z->b;
      bar (x);
    }
}

int
main (void)
{
  struct S s = { 0, 0 };
  foo (&s);
  exit (0);
}
