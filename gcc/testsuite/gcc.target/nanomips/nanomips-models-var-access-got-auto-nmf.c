/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -fpic" } */
/* { dg-skip-if "" { *-*-* } { "-O1"  "-O2" "-O3" "-Os" } { "" } } */

/* { dg-final { scan-assembler "aluipc\t\\\$gp,%pcrel_hi\\(_gp\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_page\\(a_var_extern\\)\\(\\\$gp\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_ofst\\(a_var_extern\\)\\(\\\$\[ast0-9\]+\\)" } } */

extern int a_var_extern __attribute__ ((cmodel("auto")));

int
foo ()
{
  int a1 = a_var_extern;
  return 0;
}
