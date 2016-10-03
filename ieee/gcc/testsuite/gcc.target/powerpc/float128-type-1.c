/* { dg-do compile { target { powerpc64*-*-linux* && lp64 } } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power8" } } */
/* { dg-options "-mcpu=power8 -O2" } */

/* -mfloat128-type is enabled by default on VSX Linux 64-bit systems.  */

typedef double          __attribute__((__mode__(__KF__))) f128_t;
typedef _Complex double __attribute__((__mode__(__KC__))) f128c_t;

f128_t
add_scalar (f128_t a, f128_t b)
{
  return a+b;
}


f128c_t
add_complex (f128c_t a, f128c_t b)
{
  return a+b;
}

/* { dg-final { scan-assembler "bl __addkf3" } } */
