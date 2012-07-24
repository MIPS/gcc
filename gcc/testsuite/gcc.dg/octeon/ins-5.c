/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

void abort (void);

struct s
{
  int a:2;
  unsigned long long b:34;
};

struct s
f (unsigned int i)
{
  struct s s;
  s.b = (unsigned long long) (i+1);
  return s;
}

main ()
{
  struct s s = f (0x80000000u);
  if (s.b != 0x80000001u)
    abort ();
  return 0;
}
