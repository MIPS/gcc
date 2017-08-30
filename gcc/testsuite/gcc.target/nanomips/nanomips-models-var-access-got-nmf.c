/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -fpic" } */
/* { dg-skip-if "" { *-*-* } { "-O1"  "-O2" "-O3" "-Os" } { "" } } */

/* { dg-final { scan-assembler "aluipc\t\\\$gp,%pcrel_hi\\(_gp\\)" } } */

/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_page\\(a_ext_int\\)\\(\\\$gp\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_ofst\\(a_ext_int\\)\\(\\\$\[ast0-9\]+\\)" } } */

/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_disp\\(m_ext_int\\)\\(\\\$gp\\)" } } */
/* lw reg, 0(reg) # 1 */

/* { dg-final { scan-assembler "lwpc\t\\\$\[ast0-9\]+,%got_pcrel32\\(l_ext_int\\)" } } */
/* lw reg, 0(reg) # 2 */

/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,0\\(\\\$\[ast0-9\]+\\)" 2 } } */

extern int a_ext_int __attribute__ ((model("auto")));
extern int m_ext_int __attribute__ ((model("medium")));
extern int l_ext_int __attribute__ ((model("large")));

int
foo ()
{
  int a1 = a_ext_int;
  int a2 = m_ext_int;
  int a3 = l_ext_int;
  return 0;
}
