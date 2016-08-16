/* Test _Float128x constant types.  */
/* { dg-do compile } */
/* { dg-require-effective-target float128x } */
/* { dg-options "" } */

#define WIDTH 128
#define EXT 1
#include "floatn-typeof.h"
