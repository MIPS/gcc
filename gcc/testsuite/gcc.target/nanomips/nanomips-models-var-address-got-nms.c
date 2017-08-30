/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6s -fpic" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* { dg-final { scan-assembler "aluipc\t\\\$gp,%pcrel_hi\\(_gp\\)" } } */

/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_disp\\(a_ext_int\\)\\(\\\$gp\\)" } } */

/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_disp\\(m_ext_int\\)\\(\\\$gp\\)" } } */

/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%got_pcrel_hi\\(l_ext_int\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_lo\\(l_ext_int\\)\\(\\\$\[ast0-9\]+\\)" } } */

extern int a_ext_int __attribute__ ((model("auto")));
extern int m_ext_int __attribute__ ((model("medium")));
extern int l_ext_int __attribute__ ((model("large")));

int
foo ()
{
  int *a1 = &a_ext_int;
  int *a2 = &m_ext_int;
  int *a3 = &l_ext_int;
  return 0;
}
