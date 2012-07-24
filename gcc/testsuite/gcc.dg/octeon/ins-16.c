/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

void abort (void);

struct s
{
  int a:2;
  int b:21;
};

struct s
f (struct s s, unsigned short a)
{
  s.b = (short) a;
  return s;
}

main ()
{
  struct s s;
  s = f (s, 40000);
  if (s.b < -32768 || 32767 < s.b)
    abort ();
  return 0;
}
