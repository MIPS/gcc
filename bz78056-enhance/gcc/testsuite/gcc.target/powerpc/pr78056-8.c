/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power5" } } */
/* Is desirable to require-effective-target to be p5_ok, but there's
   no such effective target, so we're just assuming that the effective
   target is at minimum power5 ok.  */
/* dfp_hw represents power 6 */
/* { dg-skip-if "" { dfp_hw } } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */
/* { dg-options "-mcpu=power5" } */

/* This test will fail if the effective target is not power 5 ok, but
   there is no dg directive to "unsupport" this test on such a
   platform.  */
/* This test should succeed on both 32- and 64-bit configurations.  */
/* Though the command line specifies power5 target, this function is
   to support power6. Expect an error message here because this target
   does not support power6.  */
__attribute__((target("cpu=power6")))
/* fabs/fnabs/fsel */
double normal1 (double a, double b)
{ /* { dg-warning "lacks power9 support" } */
  return __builtin_copysign (a, b); /* { dg-warning "implicit declaration" } */
}

