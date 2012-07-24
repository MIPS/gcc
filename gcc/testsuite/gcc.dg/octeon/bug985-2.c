/* { dg-do run } */

#include <ctype.h>

char c = 0xff;

int
main()
{
  return islower (c);
}
