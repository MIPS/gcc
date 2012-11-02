/* From cvm_ip_cache_entry_lookup.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* This fails with n64 because the call to g needs a load. */
/* { dg-final { scan-assembler-times "ld\\t" 2 } } */
/* { dg-final { scan-assembler-not "lb" } } */

struct s
{
  unsigned long long a:61;
  unsigned long long b:3;
  unsigned long long c:61;
  unsigned long long d:3;
};

int
f (struct s *s, struct s *d)
{
  *d = *s;
  if (d->b == d->d)
    g ();
}
