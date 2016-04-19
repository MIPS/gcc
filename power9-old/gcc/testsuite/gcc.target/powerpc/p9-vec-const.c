/* { dg-do compile { target { powerpc64*-*-* } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power9" } } */
/* { dg-options "-mcpu=power9 -O2" } */
/* { dg-require-effective-target powerpc_p9vector_ok } */

#include <altivec.h>

vector long ret_long (void) { return vec_splats ((long) 127); }
vector int  ret_int  (void) { return vec_splats ((int)  127); }
vector char ret_char (void) { return vec_splats ((char) 127); }



/* { dg-final { scan-assembler-times "xxspltib" 3 } } */
/* { dg-final { scan-assembler-times "vextsb2d" 1 } } */
/* { dg-final { scan-assembler-times "vextsb2w" 1 } } */
