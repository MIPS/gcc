/* Test _Float16 type-generic built-in functions.  */
/* { dg-do run } */
/* { dg-require-effective-target float16 } */
/* { dg-options "" } */

#define WIDTH 16
#define EXT 0
#include "floatn-tg.h"
