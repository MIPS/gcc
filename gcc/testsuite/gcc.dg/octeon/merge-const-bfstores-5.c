/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

void abort (void);

struct s
{
  long long a:12;
  long long b:12;
  long long c:40;
};

struct s ss, *p = &ss;

int a;

__attribute__ ((noinline)) void
g (struct s s)
{
  if (s.b != 3 || s.a != a)
    abort ();
}

__attribute__ ((noinline)) void
f (int i)
{
  struct s s;
  s.a = 2;
  if (i)
    p = &s;
  p->a = 1;
  s.b = 3;

  g (s);
}

int
main ()
{
  a = 2;
  f (0);
  a = 1;
  f (1);

  return 0;
}
