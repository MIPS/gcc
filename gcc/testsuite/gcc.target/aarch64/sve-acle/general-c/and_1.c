/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"

void
f1 (svbool_t pg, svuint8_t x, svint8_t y)
{
  svand_x (pg, x); /* { dg-error "too few arguments to function 'svand_x'" } */
  svand_x (pg, x, x, x); /* { dg-error "too many arguments to function 'svand_x'" } */
  svand_x (x, x, x); /* { dg-error "passing 'svuint8_t' to argument 1 of 'svand_x', which expects 'svbool_t'" } */
  svand_x (pg, pg, pg); /* { dg-error "'svand_x' has no form that takes 'svbool_t' arguments" } */
  svand_z (pg, pg, pg);
  svand_z (pg, pg, 0); /* { dg-error "'svand_b_z' has no '_n' form" } */
  svand_m (pg, pg, pg); /* { dg-error "'svand_m' has no form that takes 'svbool_t' arguments" } */
  svand_x (pg, 1, x); /* { dg-error "passing 'int' to argument 2 of 'svand_x', which expects an SVE vector type" } */
  svand_x (pg, x, y); /* { dg-error "passing 'svint8_t' to argument 3 of 'svand_x', but previous arguments had type 'svuint8_t'" } */
}
