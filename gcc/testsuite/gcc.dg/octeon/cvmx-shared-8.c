/* Test SECCAT_DATA_REL. */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-fPIC" } */
/* { dg-final { scan-assembler ".globl\tp\n\t.section\t.cvmx_shared," } } */
/* { dg-final { scan-assembler-not "\t.section\t.data.rel" } } */

#define CVMX_SHARED __attribute__ ((cvmx_shared))

extern int f ();
CVMX_SHARED int (*p)() = f;
