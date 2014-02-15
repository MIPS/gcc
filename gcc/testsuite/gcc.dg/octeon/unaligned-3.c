/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "uld\\t|ldr\\t" } } */

struct s
{
  unsigned long long l;
  unsigned long long z[2];
};

struct r
{
  char c;
  struct s s;
};

struct s *ss;

f (struct r *p, unsigned long long l)
{
  unsigned long long i;
  for (i = 0; i < l; i++)
    {
      *ss = p[i].s;
      g ();
    }
}
