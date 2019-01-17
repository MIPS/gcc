/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h" /* { dg-message "initializing argument 2" } */

svuint32_t
f1 (svuint32_t x, svint8_t y, svuint8_t z)
{
  return svdot_u32 (x, y, z); /* { dg-error "cannot convert 'svint8_t' to 'svuint8_t'" } */
}
