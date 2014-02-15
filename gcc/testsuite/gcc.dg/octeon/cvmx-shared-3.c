/* Test cvmx_shared attribute on small data types.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler ".globl\ti_init\n\t.section\t.cvmx_shared,\"aw\",\@progbits\n" } } */
/* { dg-final { scan-assembler "i_init:\n\t.word\t2" } } */
/* { dg-final { scan-assembler ".local\tx_s_init0\n\t.section\t.cvmx_shared_bss" } } */
/* { dg-final { scan-assembler "x_s_init0:\n\t.space\t4" } } */
/* { dg-final { scan-assembler ".section\t.cvmx_shared\n\[^\n\]*\n\[^\n\]*\n\[^\n\]*\nl_s_init:\n\t.word\t2" } } */
/* { dg-final { scan-assembler ".local\tk_s_uninit\n\t.section\t.cvmx_shared_bss\n\t.align\t2\n\t.type\tk_s_uninit, \@object\n\t.size\tk_s_uninit, 4\nk_s_uninit:\n\t.space\t4" } } */
/* { dg-final { scan-assembler ".globl\tm_c_uninit\n\[^\n\]*\n\[^\n\]*\n\[^\n\]*\nm_c_uninit:\n\t.space\t4\n" } } */
/* Don't use gp_rel for small data with cvmx_shared attribute.  */
/* { dg-final { scan-assembler-not "gp_rel\\\(abc\\\)\n" } } */
/* { dg-final { scan-assembler "hi\\\(abc\\\)|got_disp\\\(abc\\\)" } } */

#define CVMX_SHARED __attribute__ ((cvmx_shared))

CVMX_SHARED int j_uninit;
CVMX_SHARED int i_init = 2;

CVMX_SHARED static int x_s_init0 = 0;
CVMX_SHARED static int l_s_init = 2;
CVMX_SHARED static int k_s_uninit;

CVMX_SHARED const int m_c_uninit;

CVMX_SHARED struct {
 int x;
 int y;
} abc = {.y = 20};

main()
{
  CVMX_SHARED const long b = 10;
  long CVMX_SHARED x[10] = {1};
  CVMX_SHARED long *y[4];
  
  y[1] = &x[3];
  j_uninit = b + x[3] + *y[1] + abc.y;
  return 0;
}
