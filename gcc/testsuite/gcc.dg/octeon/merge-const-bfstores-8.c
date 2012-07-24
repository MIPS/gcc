/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

void abort (void);

struct s
{
  unsigned long long a:4;
  unsigned long long b:4;
};

int i;

__attribute__ ((noinline)) void g (struct s s)
{
  if (s.a != 5 || s.b != 4)
    abort ();
}
__attribute__ ((noinline)) void h (struct s s)
{
  if (s.a != 2 || s.b != 3)
    abort ();
}

int
main ()
{
  struct s s;

  s.a = 5;
  s.b = 4;
  g (s);
  s.b = 3;
  s.a = 2;
  h (s);

  return 0;
}
