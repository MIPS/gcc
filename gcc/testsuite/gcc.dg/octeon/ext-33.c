/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "exts" } } */
/* { dg-final { scan-assembler-not "sll" } } */

unsigned f ();

g ()
{
  unsigned y = f ();
  if (y != 680)
    h ();
}
