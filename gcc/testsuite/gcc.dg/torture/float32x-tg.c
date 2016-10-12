/* Test _Float32x type-generic built-in functions.  */
/* { dg-do run } */
/* { dg-require-effective-target float32x } */
/* { dg-options "" } */
/* { dg-add-options float32x } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target float32x_runtime } */

#define WIDTH 32
#define EXT 1
#include "floatn-tg.h"
