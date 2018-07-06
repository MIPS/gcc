/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"

void
f1 (svbool_t pg, svuint8_t x, svint8_t y)
{
  svadd_x (pg, x); /* { dg-error "too few arguments to function 'svadd_x'" } */
  svadd_x (pg, x, x, x); /* { dg-error "too many arguments to function 'svadd_x'" } */
  svadd_x (x, x, x); /* { dg-error "passing 'svuint8_t' to argument 1 of 'svadd_x', which expects 'svbool_t'" } */
  svadd_x (pg, pg, pg); /* { dg-error "'svadd_x' has no form that takes 'svbool_t' arguments" } */
  svadd_x (pg, 1, x); /* { dg-error "passing 'int' to argument 2 of 'svadd_x', which expects an SVE vector type" } */
  svadd_x (pg, x, y); /* { dg-error "passing 'svint8_t' to argument 3 of 'svadd_x', but previous arguments had type 'svuint8_t'" } */
}
