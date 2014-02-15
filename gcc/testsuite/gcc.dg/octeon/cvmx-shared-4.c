/* Test cvmx_shared attribute types.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-final { scan-assembler ".globl\ta_init0\n\t.section\t.cvmx_shared_bss," } } */
/* { dg-final { scan-assembler "a_init0:\n\t.space" } } */
/* { dg-final { scan-assembler ".globl\ta_init\n\t.section\t.cvmx_shared,\"aw\",\@progbits\n" } } */
/* { dg-final { scan-assembler "a_init:\n\t.dword\t10|a_init:\n\t.word\t10" } } */
/* { dg-final { scan-assembler ".local\tstop\n\[^\n\]*\n\[^\n\]*\n\[^\n\]*\nstop:\n\t.space\t64\n" } } */
/* { dg-final { scan-assembler ".globl\tp_uninit\n\t.section\t.cvmx_shared_bss\[^\n\]*\n\[^\n\]*\n\[^\n\]*\n\[^\n\]*\np_uninit:\n\t.space" } } */
/* { dg-final { scan-assembler ".globl\tx_s_uninit\n\t.section\t.cvmx_shared_bss\[^\n\]*\n\[^\n\]*\n\[^\n\]*\n\[^\n\]*\nx_s_uninit:\n\t.space" } } */

#define CVMX_SHARED __attribute__ ((cvmx_shared))

CVMX_SHARED long a_init0 = 0;
CVMX_SHARED long a_init = 10;

CVMX_SHARED char *p_uninit;
CVMX_SHARED char *ptr = "Hello";

CVMX_SHARED const long x_s_uninit;

static CVMX_SHARED unsigned long long stop[8];

typedef long __attribute__ ((cvmx_shared)) cvmx_shared_attr; /* { dg-warning "attribute only applies to variables" } */

int f() CVMX_SHARED; /* { dg-warning "attribute only applies to variables" } */

