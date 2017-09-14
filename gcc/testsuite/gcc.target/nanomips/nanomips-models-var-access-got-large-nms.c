/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6s -mcmodel=medium -fPIC" } */
/* { dg-skip-if "" { *-*-* } { "-O1"  "-O2" "-O3" "-Os" } { "" } } */

/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%got_pcrel_hi\\(var_extern\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_lo\\(var_extern\\)\\(\\\$\[ast0-9\]+\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,0\\(\\\$\[ast0-9\]+\\)" } } */

extern int var_extern;

int
foo ()
{
  int a1 = var_extern;
  return 0;
}
