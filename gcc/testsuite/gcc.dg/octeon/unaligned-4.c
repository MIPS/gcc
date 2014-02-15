/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "uld\\t|ldr\\t" 3 } } */

struct s
{
  unsigned long long l;
  unsigned long long z[2];
};

struct r
{
  char c;
  struct s s;
} __attribute__ ((packed));

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
