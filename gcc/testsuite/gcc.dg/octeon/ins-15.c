/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

void abort (void);

struct s
{
  int a:2;
  int b:21;
};

struct s
f (struct s s, unsigned char a)
{
  s.b = (char) a;
  return s;
}

main ()
{
  struct s s;
  s = f (s, 128);
  if (s.b < -128 || 127 < s.b)
    abort ();
  return 0;
}
