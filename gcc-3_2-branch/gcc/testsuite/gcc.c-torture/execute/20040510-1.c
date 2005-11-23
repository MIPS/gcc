/* PR target/15084 */
extern void abort (void);
extern void exit (int);

struct A
{
  unsigned char a, b;
  unsigned short c;
  unsigned char d, e, f, g;
};

struct A h[255];
int j = 0;

void
foo (void)
{
  struct A a;
  int i = 0;

  a.f = 10;
  for (i = 0; i < 16; i++)
    {
      int k;
      for (k = 0; k < j; k++)
	if (h[k].d == 0 && (h[k].e == i || h[k].g == i))
	  break;

      if (k != j)
	continue;
      a.b = 0;
      a.e = i;
      a.g = i;
      h[j++] = a;
    }
}

int
main ()
{
  foo ();
  if (h[0].f != 10)
    abort ();
  exit (0);
}
