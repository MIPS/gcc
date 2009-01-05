/* { dg-do link } */
/* { dg-options "{-fwhopr -shared}" } */
#include "20081224_0.h"

extern struct foo x;

void f(void) {
  x.x = 0;
}
