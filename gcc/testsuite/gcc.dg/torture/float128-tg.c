/* Test _Float128 type-generic built-in functions.  */
/* { dg-do run } */
/* { dg-require-effective-target float128 } */
/* { dg-options "" } */

#define WIDTH 128
#define EXT 0
#include "floatn-tg.h"
