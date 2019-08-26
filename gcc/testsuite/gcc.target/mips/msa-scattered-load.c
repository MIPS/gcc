/* { dg-do compile } */
/* { dg-options "-mfp64 -mhard-float -mmsa" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#include <msa.h>
#include <stddef.h>
#include <complex.h>

void pgather2cf1(const float complex* from, v4f32* pv, size_t stride) {
  v4f32 v;
  v[0] = crealf(from[0]);
  v[1] = cimagf(from[0]);
  v[2] = crealf(from[stride]);
  v[3] = cimagf(from[stride]);
  *pv = v;
}

/* { dg-final { scan-assembler-not "mfc1" } } */
/* { dg-final { scan-assembler-not "mtc1" } } */

