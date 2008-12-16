/* { dg-do run } */
/* { dg-options "-O2 -std=gnu99 -g -pedantic-errors -mea64" } */

#include <stdlib.h>

__ea char str[] = "abc";
__ea char *p = str;

int
main (void)
{
  if (*p++ != 'a')
    abort ();

  if (*p++ != 'b')
    abort ();

  if (*p++ != 'c')
    abort ();

  if (*p++ != '\0')
    abort ();

  return 0;
}
