/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "dins\t.*,29,3" } } */

void abort (void);

main ()
{
  struct
  {
    signed int s:3;
    unsigned int u:3;
  }
  x = {-1, -1};
  if (x.s != -1)
    abort ();
}
