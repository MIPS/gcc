/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h" /* { dg-message "note: expected 'svuint8_t' but argument is of type 'svint8_t'" } */

svuint8_t
f1 (svbool_t pg, svuint8_t x, svint8_t y)
{
  svadd_u8_x (pg, x, y); /* { dg-error "incompatible type for argument 3 of 'svadd_u8_x'" } */
  svadd_u8_x (pg, x); /* { dg-error "too few arguments to function 'svadd_u8_x'" } */
  svadd_u8_x (pg, x, x, x); /* { dg-error "too many arguments to function 'svadd_u8_x'" } */
  return svadd_s8_x (pg, y, y); /* { dg-error "incompatible types when returning type 'svint8_t' but 'svuint8_t' was expected" } */
}
