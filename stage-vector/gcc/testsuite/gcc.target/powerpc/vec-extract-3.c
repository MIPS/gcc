/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_vsx_ok } */

#include <altivec.h>

double
add_double0 (vector double *p)
{
  return vec_extract (*p, 0) + 1.0;
}

double
add_double1 (vector double *p)
{
  return vec_extract (*p, 1) + 1.0;
}

long
add_long0 (vector long long *p)
{
  return vec_extract (*p, 0) + 1;
}

long
add_long1 (vector long long *p)
{
  return vec_extract (*p, 1) + 1;
}

/* { dg-final { scan-assembler-not "lxvd2x"   } } */
/* { dg-final { scan-assembler-not "lxvw4x"   } } */
/* { dg-final { scan-assembler-not "lxvx"     } } */
/* { dg-final { scan-assembler-not "lxv"      } } */
/* { dg-final { scan-assembler-not "lvx"      } } */
/* { dg-final { scan-assembler-not "xxpermdi" } } */
