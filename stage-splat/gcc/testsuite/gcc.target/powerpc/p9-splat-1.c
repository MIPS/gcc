/* { dg-do compile { target { powerpc64le-*-* } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power9" } } */
/* { dg-options "-mcpu=power9 -O2" } */
/* { dg-require-effective-target powerpc_p9vector_ok } */

#include <altivec.h>

vector int
foo_r (int a)
{
  return (vector int) { a, a, a, a };		/* mtvsrws */
}

vector int
foo_r2 (int a)
{
  return vec_splats (a);			/* mtvsrws */
}

vector int
foo_p (int *a)
{
  return (vector int) { *a, *a, *a, *a };	/* lxvwsx */
}

#if 0
static int a;
int *p_a = &a;

vector int
foo_s (void)
{
  return (vector int) { a, a, a, a };		/* lxvwsx */
}
#endif


/* { dg-final { scan-assembler-times "mtvsrws" 2 } } */
/* { dg-final { scan-assembler-times "lxvwsx"  1 } } */
