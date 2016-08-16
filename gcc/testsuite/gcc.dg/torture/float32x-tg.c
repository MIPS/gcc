/* Test _Float32x type-generic built-in functions.  */
/* { dg-do run } */
/* { dg-require-effective-target float32x } */
/* { dg-options "" } */

#define WIDTH 32
#define EXT 1
#include "floatn-tg.h"
