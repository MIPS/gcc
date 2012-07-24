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
    unsigned long long c: 56;
  } s;
};

__attribute__ ((noinline)) void
g (union u u, int i)
{
  asm ("");
  if (u.s.a != 3
      || u.s.b != 2
      || u.s.c != 0
      || i != 0)
    abort ();
}

unsigned long long l;
int
main ()
{
  union u u;
  int i;

  u.l = l;
  u.s.b = 2;
  i = u.s.a;
  u.s.a = 3;

  g (u, i);
  return 0;
}

