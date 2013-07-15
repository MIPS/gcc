/* { dg-do run } */
/* { dg-options "-fsanitize=shift -w" } */

int
main (void)
{
  int a = 1;
  a <<= -3;
  return 0;
}
/* { dg-output "shift exponent -3 is negative" } */
