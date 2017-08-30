/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mcmodel=medium -fPIC" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* { dg-final { scan-assembler "lwpc\t\\\$\[ast0-9\]+,%got_pcrel32\\(var_extern\\)" } } */

extern int var_extern;

int
foo ()
{
  int a1 = var_extern;
  return 0;
}
