/* PR target/70325 */
/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */

typedef char C __attribute((__vector_size__(32)));
typedef int I __attribute((__vector_size__(32)));

void
f(int a,I b)
{
  __builtin_ia32_storedquqi256_mask((char *)f,(C)b,a); /* { dg-error "needs isa option\[^\n\r]*-mavx512vl -mavx512bw" } */
}
