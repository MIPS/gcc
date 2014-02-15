/* { dg-do compile { target *-octeon-linux* } } */
/* { dg-options "-march=octeon2" } */
/* { dg-final { scan-assembler {\trdhwr\t\$[^,]*,\$29} } } */
/* { dg-final { scan-assembler-not {\tmove\t\$[^,]*,\$26} } } */

__thread int i;
int
main ()
{
  return i;
}
