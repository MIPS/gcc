/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "exts" } } */
/* XFAIL: bug 2851, been failing since at least SDK 2.0.  */
/* { dg-final { scan-assembler-not "sll" { xfail *-*-* } } } */

unsigned f ();

g ()
{
  unsigned y = f ();
  if (y != 680)
    h ();
}
