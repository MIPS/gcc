/* { dg-do compile } */
/* { dg-options "-O2" } */

#include <arm_sve.h>



/* Make sure that RDFFR comes after the LD1.  */
svint8_t
foo (svbool_t pg, int8_t *restrict ptr, svbool_t *restrict *restrict preds)
{
  svsetffr ();
  svint8_t x = svldff1 (pg, ptr);
  for (int i = 0; i < 100; ++i)
    *preds[i] = svrdffr ();
  return x;
}

/* { dg-final { scan-assembler {\tsetffr\n.*\tldff1b\t.*\trdffr\t} } } */
/* { dg-final { scan-assembler-times {\trdffr\t} 1 } } */
