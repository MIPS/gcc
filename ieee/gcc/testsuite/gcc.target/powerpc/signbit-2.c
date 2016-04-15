/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_p8vector_ok } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power8" } } */
/* { dg-options "-mcpu=power8 -O2 -mfloat128" } */

int do_signbit_sf (float *a) { return __builtin_signbit (*a); }
int do_signbit_df (double *a) { return __builtin_signbit (*a); }
int do_signbit_kf (__float128 *a) { return __builtin_signbit (*a); }

/* { dg-final { scan-assembler-not   "stfs"     } } */
/* { dg-final { scan-assembler-not   "stfd"     } } */
/* { dg-final { scan-assembler-not   "stxvd2x"  } } */
/* { dg-final { scan-assembler-not   "stxvw4x"  } } */
/* { dg-final { scan-assembler-not   "stxsd"    } } */
/* { dg-final { scan-assembler-not   "stxsdx"   } } */
/* { dg-final { scan-assembler-not   "lfs"      } } */
/* { dg-final { scan-assembler-not   "lfd"      } } */
/* { dg-final { scan-assembler-not   "lxvd2x"   } } */
/* { dg-final { scan-assembler-not   "lxvw4x"   } } */
/* { dg-final { scan-assembler-not   "lxsd"     } } */
/* { dg-final { scan-assembler-not   "lxsdx"    } } */
/* { dg-final { scan-assembler-times "lwa"    1 } } */
/* { dg-final { scan-assembler-times "lwd"    2 } } */
/* { dg-final { scan-assembler-times "srdi"   3 } } */
