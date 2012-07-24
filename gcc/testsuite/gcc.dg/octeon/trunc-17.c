/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "andi" } } */

unsigned short
f (unsigned short a, unsigned short b)
{
  if (a == b)
    g ();
}
