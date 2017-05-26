/* { dg-options "-mhard-float -mgp32 -ffixed-f20" } */
register double g __asm__("$f20");

void
test (double a)
{
  g = -a;
}
