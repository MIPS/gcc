/* { dg-do compile } */
/* { dg-options "-mfp64 -mhard-float -mmsa" } */
/* { dg-additional-options "-mfuse-vect-init" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

typedef int v4i32 __attribute__ ((vector_size(16)));

void
copy (int* src, v4i32* dst)
{
  v4i32 chunk = (v4i32){src[0], src[1], src[2], src[3]};
  dst[0] = chunk;
}

/* { dg-final { scan-assembler-not "insert" } } */
/* { dg-final { scan-assembler-times "\tld\\\.w" 1 } } */
/* { dg-final { scan-assembler-times "\tst\\\.w" 1 } } */

