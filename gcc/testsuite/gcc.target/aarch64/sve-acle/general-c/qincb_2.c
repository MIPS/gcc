#include <arm_sve.h>

void
test (svbool_t pg, svint8_t s8, svuint8_t u8,
      svint16_t s16, svuint16_t u16, svint32_t s32, svuint32_t u32,
      svint64_t s64, svuint64_t u64, int16_t sh, uint16_t uh,
      int32_t sw, uint32_t uw, int64_t sd, uint64_t ud,
      float f, int i)
{
  svqincb_n_s32 (sw, -1); /* { dg-error {passing -1 to argument 2 of 'svqincb_n_s32', which expects a value in the range \[1, 16\]} } */
  svqincb_n_s32 (sw, 0); /* { dg-error {passing 0 to argument 2 of 'svqincb_n_s32', which expects a value in the range \[1, 16\]} } */
  svqincb_n_s32 (sw, 1);
  svqincb_n_s32 (sw, 2);
  svqincb_n_s32 (sw, 16);
  svqincb_n_s32 (sw, 17); /* { dg-error {passing 17 to argument 2 of 'svqincb_n_s32', which expects a value in the range \[1, 16\]} } */
}
