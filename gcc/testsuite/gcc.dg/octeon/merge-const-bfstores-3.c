/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

void abort (void);

union u
{
  unsigned long long l;

  struct
  {
    unsigned long long a: 4;
    unsigned long long b: 4;
    unsigned long long c: 4;
    unsigned long long d: 52;
 } s;
};

__attribute__ ((noinline)) void
g (union u u)
{
  if (u.s.a != 1
      || u.s.b != 2
      || u.s.c != 3
      || u.s.d != 0)
    abort ();
}

int
main ()
{
  union u u;

  u.l = 0;
  u.s.b = 2;
  u.s.d = u.s.a;
  u.s.c = 3;
  u.s.a = 1;

  g (u);
  return 0;
}

