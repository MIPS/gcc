/* { dg-do run } */
/* { dg-options "-fsanitize=shift -w" } */

int
main (void)
{
  int a = 1;
  a <<= 152;
  return 0;
}
/* { dg-output "shift exponent 152 is too large for \[^\n\r]*-bit type int" } */
