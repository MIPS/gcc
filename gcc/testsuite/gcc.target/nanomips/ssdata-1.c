/* { dg-do compile } */
/* { dg-options "-fno-pic" } */

/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%gprel\\(var_data\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%gprel\\(var_bss\\)" } } */
/* { dg-final { scan-assembler "\\.section\t\\.ssdata,\"aw\",@progbits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.ssbss,\"aw\",@nobits" } } */


int __attribute__((section(".ssdata"))) var_data;
int __attribute__((section(".ssbss"))) var_bss;

int
foo ()
{
  return var_data + var_bss;
}
