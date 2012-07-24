/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "0x03e00821" } } */

void h (int (*) (int));

void
f (int i)
{
#if __LONG_MAX__ == 9223372036854775807LL
  int
  g (int j)
  {
    return j + i;
  }

  h (&g);
#endif
}
