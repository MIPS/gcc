/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"

svuint8_t
f1 (svbool_t pg, svuint8_t u8, svint8_t s8, svint64_t s64, svuint64_t u64,
    svfloat32_t f32, svfloat64_t f64)
{
  svcmpeq_wide (pg, s8); /* { dg-error "too few arguments to function 'svcmpeq_wide'" } */
  svcmpeq_wide (pg, s8, s64, s8); /* { dg-error "too many arguments to function 'svcmpeq_wide'" } */
  svcmpeq_wide (s8, s8, s64); /* { dg-error "passing 'svint8_t' to argument 1 of 'svcmpeq_wide', which expects 'svbool_t'" } */
  svcmpeq_wide (pg, 0, s64); /* { dg-error "passing 'int' to argument 2 of 'svcmpeq_wide', which expects an SVE vector type" } */
  svcmpeq_wide (pg, s8, 0); /* { dg-error "passing 'int' to argument 3 of 'svcmpeq_wide', which expects 'svint64_t'" } */
  svcmpeq_wide (pg, s8, s8); /* { dg-error "passing 'svint8_t' to argument 3 of 'svcmpeq_wide', which expects 'svint64_t'" } */
  svcmpeq_wide (pg, s8, u8); /* { dg-error "passing 'svuint8_t' to argument 3 of 'svcmpeq_wide', which expects 'svint64_t'" } */
  svcmpeq_wide (pg, s8, s64);
  svcmpeq_wide (pg, s8, u64); /* { dg-error "passing 'svuint64_t' to argument 3 of 'svcmpeq_wide', which expects 'svint64_t'" } */
  svcmpeq_wide (pg, u8, s8); /* { dg-error "passing 'svint8_t' to argument 3 of 'svcmpeq_wide', which expects 'svuint64_t'" } */
  svcmpeq_wide (pg, u8, u8); /* { dg-error "passing 'svuint8_t' to argument 3 of 'svcmpeq_wide', which expects 'svuint64_t'" } */
  svcmpeq_wide (pg, u8, s64); /* { dg-error "passing 'svint64_t' to argument 3 of 'svcmpeq_wide', which expects 'svuint64_t'" } */
  svcmpeq_wide (pg, u8, u64);
  svcmpeq_wide (pg, s64, s64); /* { dg-error "'svcmpeq_wide' has no form that takes 'svint64_t' arguments" } */
  svcmpeq_wide (pg, f32, f32); /* { dg-error "passing 'svfloat32_t' to argument 2 of 'svcmpeq_wide', which expects a vector of integers" } */
  svcmpeq_wide (pg, f32, f64); /* { dg-error "passing 'svfloat32_t' to argument 2 of 'svcmpeq_wide', which expects a vector of integers" } */
  svcmpeq_wide (pg, f64, f64); /* { dg-error "passing 'svfloat64_t' to argument 2 of 'svcmpeq_wide', which expects a vector of integers" } */
  svcmpeq_wide (pg, pg, pg); /* { dg-error "passing 'svbool_t' to argument 2 of 'svcmpeq_wide', which expects a vector of integers" } */
}
