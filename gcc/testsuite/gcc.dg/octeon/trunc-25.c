/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* The zero extend pattern should not need a truncation. */
/* { dg-final { scan-assembler-not "sll\t" } } */
typedef long long ll;
ll f(ll a)
{
  ll c = (unsigned int)a;
  return c;
}
