/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2 -Wall -W -Werror -save-temps" } */

#include <stdio.h>
#include <inttypes.h>

uint64_t x;

int
main ()
{
  printf("x=0x%" PRIx64, x);
  return 0;
}
