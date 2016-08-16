/* Test _Float64x constant types.  */
/* { dg-do compile } */
/* { dg-require-effective-target float64x } */
/* { dg-options "" } */

#define WIDTH 64
#define EXT 1
#include "floatn-typeof.h"
