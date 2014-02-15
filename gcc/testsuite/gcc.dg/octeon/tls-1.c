/* { dg-do compile { target *-octeon-linux* } } */
/* { dg-options "-march=octeon" } */
/* { dg-final { scan-assembler {\tmove\t\$[^,]*,\$26} } } */
/* { dg-final { scan-assembler-not {\trdhwr\t\$[^,]*,\$29} } } */

__thread int i;
int
main ()
{
  return i;
}
