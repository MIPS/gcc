/* Test _Float128x type-generic built-in functions.  */
/* { dg-do run } */
/* { dg-require-effective-target float128x } */
/* { dg-options "" } */

#define WIDTH 128
#define EXT 1
#include "floatn-tg.h"
