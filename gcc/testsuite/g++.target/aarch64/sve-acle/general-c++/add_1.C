/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h" /* { dg-message "initializing argument 3" } */

svuint8_t
f1 (svbool_t pg, svuint8_t x, svint8_t y)
{
  return svadd_u8_x (pg, x, y); /* { dg-error "cannot convert 'svint8_t' to 'svuint8_t'" } */
}
