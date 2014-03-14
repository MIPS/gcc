/* { dg-options "-mgp64" } */
/* Make sure that IV-opts does not produce an unaligned access
   for p[i].s as it can be done using an aligned load. */
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
