/* Test _Float64 type-generic built-in functions.  */
/* { dg-do run } */
/* { dg-require-effective-target float64 } */
/* { dg-options "" } */
/* { dg-add-options float64 } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target float64_runtime } */

#define WIDTH 64
#define EXT 0
#include "floatn-tg.h"
