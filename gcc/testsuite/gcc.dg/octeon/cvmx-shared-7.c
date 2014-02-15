/* Test -fno-zero-initialized-in-bss.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-fno-zero-initialized-in-bss" } */
/* { dg-final { scan-assembler ".globl\ta\n\t.section\t.cvmx_shared,\"aw\",\@progbits\n" } } */
/* { dg-final { scan-assembler-not ".cvmx_shared_bss" } } */

#define CVMX_SHARED __attribute__ ((cvmx_shared))

CVMX_SHARED int a = 0;
CVMX_SHARED const long b = 0;
CVMX_SHARED static const d = 0;
