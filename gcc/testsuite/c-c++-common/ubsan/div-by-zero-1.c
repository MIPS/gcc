/* { dg-do run } */
/* { dg-options "-fsanitize=integer-divide-by-zero -Wno-div-by-zero" } */

int
main (void)
{
  0 / 0;
  return 0;
}
 /* { dg-output "division by zero" } */
