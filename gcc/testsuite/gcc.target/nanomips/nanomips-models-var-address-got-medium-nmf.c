/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mcmodel=large -fpic" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* { dg-final { scan-assembler "lapc\t\\\$gp,_gp" } } */

/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_disp\\(var_extern\\)\\(\\\$gp\\)" } } */

extern int var_extern;

int
foo ()
{
  int a1 = var_extern;
  return 0;
}
