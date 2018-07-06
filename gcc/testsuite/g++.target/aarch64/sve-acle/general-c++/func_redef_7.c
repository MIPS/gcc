/* { dg-do compile } */

__SVUInt8_t
svadd_x (__SVBool_t pg, __SVUInt8_t x, __SVUInt8_t y)
{
  return x;
}

#pragma GCC aarch64 "arm_sve.h"

svuint8_t
f (svbool_t pg, svuint8_t x, svuint8_t y)
{
  return svadd_x (pg, x, y);
}
