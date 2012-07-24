/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "bbit\[01\]\t" } } */
/* { dg-final { scan-assembler-not "andi\t" } } */

unsigned long long r;

static inline __attribute__((always_inline)) int
test_bit(unsigned long long nr, const unsigned long long *addr)
{
  return 1UL & (addr[nr >> 6] >> (nr & 63ULL));
}
 
f ()
{
  if (!test_bit(0, &r))
    g ();
}
