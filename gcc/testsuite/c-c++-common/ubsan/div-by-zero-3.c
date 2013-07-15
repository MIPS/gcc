/* { dg-do run } */
/* { dg-options "-fsanitize=integer-divide-by-zero -Wno-overflow" } */

#include <limits.h>

int
main (void)
{
  INT_MIN / -1;
  return 0;
}
 /* { dg-output "division of -2147483648 by -1 cannot be represented in type int" } */
