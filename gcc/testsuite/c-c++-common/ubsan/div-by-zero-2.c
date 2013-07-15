/* { dg-do run } */
/* { dg-options "-fsanitize=integer-divide-by-zero -Wno-div-by-zero" } */

int
main (void)
{
  1UL / 0;
  return 0;
}
 /* { dg-output "division by zero" } */
