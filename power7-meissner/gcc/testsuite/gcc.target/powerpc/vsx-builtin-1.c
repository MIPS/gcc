/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-options "-O2 -mcpu=power7" } */
/* { dg-final { scan-assembler "xvadddp" } } */
/* { dg-final { scan-assembler "xvsubdp" } } */
/* { dg-final { scan-assembler "xvmuldp" } } */
/* { dg-final { scan-assembler "xvdivdp" } } */
/* { dg-final { scan-assembler "xvmaxdp" } } */
/* { dg-final { scan-assembler "xvmindp" } } */
/* { dg-final { scan-assembler "xvsqrtdp" } } */
/* { dg-final { scan-assembler "xvabsdp" } } */
/* { dg-final { scan-assembler "xvnabsdp" } } */

void use_builtins (__vector double *p, __vector double *q, __vector double *r)
{
  __vector double tmp1 = *q;
  __vector double tmp2 = *r;

  *p++ = __builtin_vsx_xvadddp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvsubdp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvmuldp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvdivdp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvmaxdp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvmindp (tmp1, tmp2);
  *p++ = __builtin_vsx_xvabsdp (tmp1);
  *p++ = __builtin_vsx_xvnabsdp (tmp1);
  *p   = __builtin_vsx_xvsqrtdp (tmp1);
}
