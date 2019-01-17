
/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"

void
f1 (svuint32_t x, svuint8_t y, svint8_t z)
{
  svdot (x); /* { dg-error "too few arguments to function 'svdot'" } */
  svdot (x, y, y, y); /* { dg-error "too many arguments to function 'svdot'" } */
  svdot (x, y, z); /* { dg-error "passing 'svint8_t' to argument 3 of 'svdot', which expects 'svuint8_t'" } */
  svdot (x, x, x); /* { dg-error "passing 'svuint32_t' to argument 2 of 'svdot', which expects 'svuint8_t'" } */
  svdot (1, y, y); /* { dg-error "passing 'int' to argument 1 of 'svdot', which expects an SVE vector type" } */
  svdot (y, y, y); /* { dg-error "'svdot' has no form that takes 'svuint8_t' arguments" } */
}
