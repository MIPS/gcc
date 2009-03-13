/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-options "-O2 -mcpu=power7" } */
/* { dg-final { scan-assembler "xvaddsp" } } */
/* { dg-final { scan-assembler "xvsubsp" } } */
/* { dg-final { scan-assembler "xvmulsp" } } */
/* { dg-final { scan-assembler "xvdivsp" } } */
/* { dg-final { scan-assembler "xvmaxsp" } } */
/* { dg-final { scan-assembler "xvminsp" } } */
/* { dg-final { scan-assembler "xvsqrtsp" } } */
/* { dg-final { scan-assembler "xvabssp" } } */
/* { dg-final { scan-assembler "xvnabssp" } } */

void use_builtins (__vector float *p, __vector float *q, __vector float *r)
{
  __vector float tmp1 = *q;
  __vector float tmp2 = *r;

  *p++ = __builtin_vsx_xvaddsp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvsubsp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvmulsp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvdivsp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvmaxsp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvminsp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvabssp (tmp1);
  *p++ = __builtin_vsx_xvnabssp (tmp1);
  *p   = __builtin_vsx_xvsqrtsp (tmp1);
}
