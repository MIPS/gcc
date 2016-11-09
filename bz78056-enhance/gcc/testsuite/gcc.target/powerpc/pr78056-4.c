/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power7" } } */
/* { dg-require-effective-target powerpc_p8vector_ok } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */
/* { dg-options "-mcpu=power7" } */

/* This test should succeed on both 32- and 64-bit configurations.  */
#include <altivec.h>

/* Though the command line specifies power7 target, this function is
   to support power9.  */
__attribute__((target("cpu=power8")))
__int128
atomic_load_128_relaxed (__int128 *ptr)
{ /* { dg-warning "lacks power8 support" } */
	return __atomic_load_n (ptr, __ATOMIC_RELAXED); /* { dg-warning "implicit declaration" } */
}
