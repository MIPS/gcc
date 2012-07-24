/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

void abort (void);

struct s
{
  int a:2;
  int b:21;
};

struct s
f (struct s s, char a)
{
  s.b = (unsigned char) a;
  return s;
}

main ()
{
  struct s s;
  s = f (s, -4);
  if (s.b < 0 || 255 < s.b)
    abort ();
  return 0;
}
