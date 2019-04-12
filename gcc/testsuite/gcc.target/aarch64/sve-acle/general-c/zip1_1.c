/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"

svuint8_t
f1 (svbool_t pg, svuint8_t x, svint16_t y)
{
  svzip1 (pg); /* { dg-error {too few arguments to function 'svzip1'} } */
  svzip1 (pg, x, x); /* { dg-error {too many arguments to function 'svzip1'} } */
  svzip1 (pg, x); /* { dg-error {passing 'svuint8_t' to argument 2 of 'svzip1', but previous arguments had type 'svbool_t'} } */
  svzip1 (x, pg); /* { dg-error {passing 'svbool_t' to argument 2 of 'svzip1', but previous arguments had type 'svuint8_t'} } */
  svzip1 (x, y); /* { dg-error {passing 'svint16_t' to argument 2 of 'svzip1', but previous arguments had type 'svuint8_t'} } */
  svzip1 (x, 0); /* { dg-error {'svzip1_u8' has no '_n' form} } */
}
