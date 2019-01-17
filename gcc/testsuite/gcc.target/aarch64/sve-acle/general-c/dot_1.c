
/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h" /* { dg-message "note: expected 'svuint8_t' but argument is of type 'svuint32_t'" } */

svint32_t
f1 (svbool_t pg, svuint32_t x, svuint8_t y, svuint8_t z)
{
  svdot_u32 (x, y, x); /* { dg-error "incompatible type for argument 3 of 'svdot_u32'" } */
  svdot_u32 (x); /* { dg-error "too few arguments to function 'svdot_u32'" } */
  svdot_u32 (x, y, z, x); /* { dg-error "too many arguments to function 'svdot_u32'" } */
  return svdot_u32 (x, y, z); /* { dg-error "incompatible types when returning type 'svuint32_t' but 'svint32_t' was expected" } */
}
