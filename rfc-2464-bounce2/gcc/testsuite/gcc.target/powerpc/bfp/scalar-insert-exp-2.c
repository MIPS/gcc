/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power9" } } */
/* { dg-require-effective-target ilp32 } */
/* { dg-require-effective-target powerpc_p9vector_ok } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */
/* { dg-options "-mcpu=power9" } */

/* This test only runs on 32-bit configurations, where a compiler error
   should be issued because this builtin is not available on 
   32-bit configurations.  */

#include <altivec.h>

double get_exponent (unsigned int *exponent_p, 
		     unsigned long long int *signficand_p)
{
  unsigned int exponent = *exponent_p;
  unsigned long long int significand = *significand_p;

  /* kelvin not sure about this error message on big-endian 32 bit */
  return scalar_insert_exp (exponent, significand);	/* { dg-error "Builtin function __builtin_vsx_scalar_insert_exp requires" } */
}
