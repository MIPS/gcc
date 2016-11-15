/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power5" } } */
/* powerpc_vsx_ok represents power7 */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */
/* There is an "incompatibility" between power6 flags and power7 which
   is not resolved with the __attribute__ directive below, so we'll
   test the __attribute__ directive using a -mcpu=power5 command
   line.  */
/* { dg-options "-mcpu=power5" } */

/* Though the command line specifies power5 target, this function is
   to support power7.  */
__attribute__((target("cpu=power7")))
int
div_we (int a, int b)
{
  return __builtin_divwe (a, b);
}

/* { dg-final { scan-assembler-times "divwe "   1 } } */



