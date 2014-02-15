/* Test -fno-common.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-fno-common" } */
/* { dg-final { scan-assembler ".globl\ta\n\t.section\t.cvmx_shared_bss,\"aw\",\@nobits\n" } } */
/* { dg-final { scan-assembler-not ".cvmx_shared," } } */

#define CVMX_SHARED __attribute__ ((cvmx_shared))

CVMX_SHARED int a;
