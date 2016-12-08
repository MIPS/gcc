/* { dg-do compile { target { powerpc64*-*-* && lp64 } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power9" } } */
/* { dg-require-effective-target powerpc_p9vector_ok } */
/* { dg-options "-mcpu=power9 -O2" } */

#include <altivec.h>

double
fpcvt_int_0 (vector int a)
{
  int b = vec_extract (a, 0);
  return (double)b;
}

double
fpcvt_int_3 (vector int a)
{
  int b = vec_extract (a, 3);
  return (double)b;
}

double
fpcvt_uint_0 (vector unsigned int a)
{
  unsigned int b = vec_extract (a, 0);
  return (double)b;
}

double
fpcvt_uint_3 (vector unsigned int a)
{
  unsigned int b = vec_extract (a, 3);
  return (double)b;
}

double
fpcvt_short_0 (vector short a)
{
  short b = vec_extract (a, 0);
  return (double)b;
}

double
fpcvt_short_7 (vector short a)
{
  short b = vec_extract (a, 7);
  return (double)b;
}

double
fpcvt_ushort_0 (vector unsigned short a)
{
  unsigned short b = vec_extract (a, 0);
  return (double)b;
}

double
fpcvt_ushort_7 (vector unsigned short a)
{
  unsigned short b = vec_extract (a, 7);
  return (double)b;
}

double
fpcvt_schar_0 (vector signed char a)
{
  signed char b = vec_extract (a, 0);
  return (double)b;
}

double
fpcvt_schar_15 (vector signed char a)
{
  signed char b = vec_extract (a, 15);
  return (double)b;
}

double
fpcvt_uchar_0 (vector unsigned char a)
{
  unsigned char b = vec_extract (a, 0);
  return (double)b;
}

double
fpcvt_uchar_15 (vector unsigned char a)
{
  signed char b = vec_extract (a, 15);
  return (double)b;
}

/* { dg-final { scan-assembler     "vextractub"    } } */
/* { dg-final { scan-assembler     "vextractuh"    } } */
/* { dg-final { scan-assembler     "vextsb2d"      } } */
/* { dg-final { scan-assembler     "vextsh2d"      } } */
/* { dg-final { scan-assembler     "vspltw"        } } */
/* { dg-final { scan-assembler     "xscvsxddp"     } } */
/* { dg-final { scan-assembler     "xvcvsxwdp"     } } */
/* { dg-final { scan-assembler     "xvcvuxwdp"     } } */
/* { dg-final { scan-assembler-not "exts\[bhw\] "  } } */
/* { dg-final { scan-assembler-not "stxvd2x"       } } */
/* { dg-final { scan-assembler-not "stxv"          } } */
/* { dg-final { scan-assembler-not "mfvsrd"        } } */
/* { dg-final { scan-assembler-not "mfvsrwz"       } } */
/* { dg-final { scan-assembler-not "mtvsrd"        } } */
/* { dg-final { scan-assembler-not "mtvsrwa"       } } */
/* { dg-final { scan-assembler-not "mtvsrwz"       } } */
/* { dg-final { scan-assembler-not "lwa"           } } */
/* { dg-final { scan-assembler-not "lwz"           } } */
/* { dg-final { scan-assembler-not "lha"           } } */
/* { dg-final { scan-assembler-not "lhz"           } } */
