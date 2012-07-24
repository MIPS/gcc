/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

void abort (void);

struct s
{
  long long a:12;
  long long b:12;
  long long c:40;
};

struct s s, *p;

__attribute__ ((noinline)) void
g (struct s s)
{
  if (s.a != 2
      || s.b != 3
      || s.c != 4)
    abort ();
}

__attribute__ ((noinline)) void
f (int i)
{
  struct s s;

  /* If this is not consant we probably need to change this test
     because this is meant to check interaction of varying and
     constant fields.  */
  if (__builtin_constant_p (i))
    abort ();
  s.a = 2;
  s.b = 3;
  s.c = i;

  g (s);
}

int
main ()
{
  f (4);
  return 0;
}
