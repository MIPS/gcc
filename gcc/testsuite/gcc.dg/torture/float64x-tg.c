/* Test _Float64x type-generic built-in functions.  */
/* { dg-do run } */
/* { dg-require-effective-target float64x } */
/* { dg-options "" } */

#define WIDTH 64
#define EXT 1
#include "floatn-tg.h"
