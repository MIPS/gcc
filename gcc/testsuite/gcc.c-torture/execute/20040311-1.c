/* PR target/12308 */

void *a;
extern void abort (void);

void
foo (const char *x, int y)
{
}

void __attribute__ ((noinline))
bar (void *x, const char *y)
{
  if (__builtin_strcmp (y, "xyz\n"))
    abort ();
}

void __attribute__ ((noinline))
baz (char *x, const char *y, const char *z)
{
}

int
main (void)
{
  struct
  {
    const char *a;
    int b;
  } b;
  const char *c = 0;
  int i;
  char d[80];

  b.b = 5;
  for (i = 0; i < 1; i++)
    {
      double e = b.b;
      if (c)
	{
	  bar (a, "abcd\n");
	  baz (d, "f%s", c);
	}
      else
	{
	  bar (a, "xyz\n");
	  baz (d, "f%s", c);
	}
      foo (d, e);
    }
  return 0;
}
