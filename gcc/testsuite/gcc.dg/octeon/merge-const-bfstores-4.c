/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

void abort (void);

struct s
{
  long long a:4;
  long long b:4;
  long long c:56;
};

__attribute__ ((noinline)) void
g (struct s s)
{
  if (s.a != 2 || s.b != -1 || s.c != 4)
    abort ();
}

int
main ()
{
  struct s s;
  s.a = 2;
  s.b = -1;
  s.c = 4;
  g (s);

  return 0;
}
