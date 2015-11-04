/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_p9vector_ok } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power9" } } */
/* { dg-options "-mcpu=power9 -O3" } */

long
do_ext_add (int *p, long a, long b)
{
  long l = *p;
  long l2 = l << 4;
  return l2 + ((l2 == 0) ? a : b);
}

long
do_ext (int *p, long a, long b)
{
  long l = *p;
  long l2 = l << 4;
  return ((l2 == 0) ? a : b);
}

/* { dg-final { scan-assembler "extswsli[.] "} } */
