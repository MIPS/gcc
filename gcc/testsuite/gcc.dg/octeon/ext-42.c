/* From cvm_ip_cache_entry_lookup.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "ld\\t" 2 { target { ! mips_n64_abicalls } } } } */
/* n64 with abi calls needs an extra load for the function call */
/* { dg-final { scan-assembler-times "ld\\t" 3 { target mips_n64_abicalls } } } */
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
