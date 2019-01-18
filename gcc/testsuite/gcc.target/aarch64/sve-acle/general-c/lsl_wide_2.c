/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"

void
f1 (svbool_t pg, svuint8_t x, svuint64_t y)
{
  svlsl_wide_x (pg, x); /* { dg-error "too few arguments to function 'svlsl_wide_x'" } */
  svlsl_wide_x (pg, x, x, x, x); /* { dg-error "too many arguments to function 'svlsl_wide_x'" } */
  svlsl_wide_x (x, x, y); /* { dg-error "passing 'svuint8_t' to argument 1 of 'svlsl_wide_x', which expects 'svbool_t'" } */
  svlsl_wide_x (pg, 1, y); /* { dg-error "passing 'int' to argument 2 of 'svlsl_wide_x', which expects an SVE vector type" } */
  svlsl_wide_x (pg, x, x); /* { dg-error "passing 'svuint8_t' to argument 3 of 'svlsl_wide_x', which expects 'svuint64_t'" } */
  svlsl_wide_x (pg, y, y); /* { dg-error "'svlsl_wide_x' has no form that takes 'svuint64_t' arguments" } */
}
