/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"

void
f1 (svbool_t pg, svuint8_t u8, svint8_t s8, int i, float f)
{
  svdup_lane (u8); /* { dg-error "too few arguments to function 'svdup_lane'" } */
  svdup_lane (u8, 0, 0); /* { dg-error "too many arguments to function 'svdup_lane'" } */
  svdup_lane (u8, 0);
  svdup_lane (u8, -1);
  svdup_lane (u8, i);
  svdup_lane (u8, f);
  svdup_lane (u8, u8); /* { dg-error "incompatible type for argument 2 of 'svdup_lane_u8'" } */
}
