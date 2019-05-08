/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"

void
f1 (svbool_t pg, svuint8_t u8, svint8_t s8, svuint16_t u16, svint16_t s16,
    svfloat16_t f16)
{
  svtbl (u8); /* { dg-error "too few arguments to function 'svtbl'" } */
  svtbl (u8, u8, u8); /* { dg-error "too many arguments to function 'svtbl'" } */
  svtbl (pg, pg); /* { dg-error "'svtbl' has no form that takes 'svbool_t' arguments" } */
  svtbl (pg, u8); /* { dg-error "'svtbl' has no form that takes 'svbool_t' arguments" } */

  svtbl (u8, 0); /* { dg-error "'svtbl_u8' has no '_n' form" } */
  svtbl (u8, u8);
  svtbl (u8, s8); /* { dg-error "passing 'svint8_t' to argument 2 of 'svtbl_u8', which expects 'svuint8_t'" } */
  svtbl (u8, u16); /* { dg-error "passing 'svuint16_t' to argument 2 of 'svtbl_u8', which expects 'svuint8_t'" } */
  svtbl (u8, s16); /* { dg-error "passing 'svint16_t' to argument 2 of 'svtbl_u8', which expects 'svuint8_t'" } */

  svtbl (s8, u8);
  svtbl (s8, s8); /* { dg-error "passing 'svint8_t' to argument 2 of 'svtbl_s8', which expects 'svuint8_t'" } */
  svtbl (s8, u16); /* { dg-error "passing 'svuint16_t' to argument 2 of 'svtbl_s8', which expects 'svuint8_t'" } */
  svtbl (s8, s16); /* { dg-error "passing 'svint16_t' to argument 2 of 'svtbl_s8', which expects 'svuint8_t'" } */

  svtbl (u16, u8); /* { dg-error "passing 'svuint8_t' to argument 2 of 'svtbl_u16', which expects 'svuint16_t'" } */
  svtbl (u16, s8); /* { dg-error "passing 'svint8_t' to argument 2 of 'svtbl_u16', which expects 'svuint16_t'" } */
  svtbl (u16, u16);
  svtbl (u16, s16); /* { dg-error "passing 'svint16_t' to argument 2 of 'svtbl_u16', which expects 'svuint16_t'" } */

  svtbl (s16, u8); /* { dg-error "passing 'svuint8_t' to argument 2 of 'svtbl_s16', which expects 'svuint16_t'" } */
  svtbl (s16, s8); /* { dg-error "passing 'svint8_t' to argument 2 of 'svtbl_s16', which expects 'svuint16_t'" } */
  svtbl (s16, u16);
  svtbl (s16, s16); /* { dg-error "passing 'svint16_t' to argument 2 of 'svtbl_s16', which expects 'svuint16_t'" } */
  svtbl (s16, f16); /* { dg-error "passing 'svfloat16_t' to argument 2 of 'svtbl_s16', which expects 'svuint16_t'" } */

  svtbl (f16, u8); /* { dg-error "passing 'svuint8_t' to argument 2 of 'svtbl_f16', which expects 'svuint16_t'" } */
  svtbl (f16, s8); /* { dg-error "passing 'svint8_t' to argument 2 of 'svtbl_f16', which expects 'svuint16_t'" } */
  svtbl (f16, u16);
  svtbl (f16, s16); /* { dg-error "passing 'svint16_t' to argument 2 of 'svtbl_f16', which expects 'svuint16_t'" } */
  svtbl (f16, f16); /* { dg-error "passing 'svfloat16_t' to argument 2 of 'svtbl_f16', which expects 'svuint16_t'" } */
}
