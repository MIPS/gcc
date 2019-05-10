/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"

void
f1 (svbool_t pg, svint32_t s32, svint32_t s64,
    svuint32_t u32, svuint64_t u64, svfloat32_t f32)
{
  svclz_m (u32, pg); /* { dg-error "too few arguments to function 'svclz_m'" } */
  svclz_m (u32, pg, s32, s32); /* { dg-error "too many arguments to function 'svclz_m'" } */
  svclz_m (0, pg, f32); /* { dg-error "passing 'int' to argument 1 of 'svclz_m', which expects an SVE vector type" } */
  svclz_m (u32, u32, f32); /* { dg-error "passing 'svuint32_t' to argument 2 of 'svclz_m', which expects 'svbool_t'" } */
  svclz_m (u32, 0, f32); /* { dg-error "passing 'int' to argument 2 of 'svclz_m', which expects 'svbool_t'" } */
  svclz_m (u32, pg, s32);
  svclz_m (u32, pg, u32);
  svclz_m (u32, pg, f32); /* { dg-error "'svclz_m' has no form that takes 'svfloat32_t' arguments" } */
  svclz_m (u32, pg, pg); /* { dg-error "'svclz_m' has no form that takes 'svbool_t' arguments" } */

  svclz_m (pg, pg, s32); /* { dg-error "incompatible type for argument 1 of 'svclz_s32_m'" } */
  svclz_m (s32, pg, s32); /* { dg-error "incompatible type for argument 1 of 'svclz_s32_m'" } */
  svclz_m (f32, pg, s32); /* { dg-error "incompatible type for argument 1 of 'svclz_s32_m'" } */
}
