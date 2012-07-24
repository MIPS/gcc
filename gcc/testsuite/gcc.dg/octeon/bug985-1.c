/* { dg-do run } */

#include <string.h>

int
main()
{
  char buf[8];
  buf[0] = 0xFF;
  return strncasecmp(buf, "AAAA", 4) == 0;
}
