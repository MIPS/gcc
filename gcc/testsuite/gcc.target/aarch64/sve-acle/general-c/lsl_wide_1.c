/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"

svuint8_t
f1 (svbool_t pg, svuint8_t x, svint8_t w, svuint64_t y)
{
  svlsl_wide_u8_x (pg, x, x); /* { dg-error "incompatible type for argument 3 of 'svlsl_wide_u8_x'" } */
  svlsl_wide_u8_x (pg, x); /* { dg-error "too few arguments to function 'svlsl_wide_u8_x'" } */
  svlsl_wide_u8_x (pg, x, y, x); /* { dg-error "too many arguments to function 'svlsl_wide_u8_x'" } */
  return svlsl_wide_s8_x (pg, w, y); /* { dg-error "incompatible types when returning type 'svint8_t' but 'svuint8_t' was expected" } */
}
