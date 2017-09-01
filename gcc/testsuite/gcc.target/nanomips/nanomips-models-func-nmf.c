/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -fpic" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* balc a_sta_func */

/* lapc[48] reg, a_longcall */
/* jalrc reg # 1 */

/* lw reg, %got_call(a_ext_func)($gp) */
/* .reloc 1f, R_NANOMIPS_JALR, a_ext_func */
/* jalrc reg # 2 */

/* lapc reg, a_sta_func */
/* balc a_sta_fptr # 1*/

/* lw reg, %got_disp(a_ext_func)($gp) */
/* balc a_sta_fptr # 2 */

/* { dg-final { scan-assembler "balc\ta_sta_func" } } */
/* { dg-final { scan-assembler "lapc\\\[48\\]\t\\\$\[ast0-9\]+,a_longcall" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_call\\(a_ext_func\\)\\(\\\$gp\\)" } } */
/* { dg-final { scan-assembler "\\\.reloc\t1f,R_NANOMIPS_JALR,a_ext_func" } } */
/* { dg-final { scan-assembler "lapc\t\\\$\[ast0-9\]+,a_sta_func" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_disp\\(a_ext_func\\)\\(\\\$gp\\)" } } */
/* { dg-final { scan-assembler-times "balc\ta_sta_fptr" 2 } } */

/* balc m_sta_func */

/* lapc[48] reg, m_longcall */
/* jalrc reg # 3 */

/* lw reg, %got_disp(m_ext_func)($gp) # 1 */
/* jalrc reg # 4 */

/* lapc[48] reg, m_sta_func */
/* balc m_sta_fptr # 1*/

/* lw reg, %got_disp(m_ext_func)($gp) # 2 */
/* balc m_sta_fptr # 2 */

/* { dg-final { scan-assembler "balc\tm_sta_func" } } */
/* { dg-final { scan-assembler "lapc\\\[48\\]\t\\\$\[ast0-9\]+,m_longcall" } } */
/* { dg-final { scan-assembler "lapc\\\[48\\]\t\\\$\[ast0-9\]+,m_sta_func" } } */
/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,%got_disp\\(m_ext_func\\)\\(\\\$gp\\)" 2 } } */
/* { dg-final { scan-assembler-times "balc\tm_sta_fptr" 2 } } */

/* lapc[48] reg, l_sta_func #1 */
/* jalrc reg # 5 */

/* lapc[48] reg, l_longcall */
/* jalrc reg # 6 */

/* lwpc reg, %got_pcrel32(l_ext_func) # 1 */
/* jalrc reg # 7 */

/* lapc[48] reg, l_sta_func # 2 */
/* lapc[48] reg, l_sta_fptr # 1 */
/* jalrc reg # 8 */

/* lwpc reg, %got_pcrel32(l_ext_func) # 2 */
/* lapc[48] reg, l_sta_fptr # 2 */
/* jalrc reg # 9 */

/* { dg-final { scan-assembler "lapc\\\[48\\]\t\\\$\[ast0-9\]+,l_longcall" } } */
/* { dg-final { scan-assembler-times "lapc\\\[48\\]\t\\\$\[ast0-9\]+,l_sta_func" 2 } } */
/* { dg-final { scan-assembler-times "lapc\\\[48\\]\t\\\$\[ast0-9\]+,l_sta_fptr" 2 } } */
/* { dg-final { scan-assembler-times "lwpc\t\\\$\[ast0-9\]+,%got_pcrel32\\(l_ext_func\\)" 2 } } */

/* { dg-final { scan-assembler-times "jalrc\t\\\$\[ast0-9\]+" 9 } } */
/* { dg-final { scan-assembler "aluipc\t\\\$gp,%pcrel_hi\\(_gp\\)" } } */

__attribute__ ((model("auto"), noclone ,noinline))
static int a_sta_fptr(int t()) { asm (""); return 1; }

__attribute__ ((model("auto"), noclone ,noinline))
static int a_sta_func(void) { asm (""); return 1; }

__attribute__ ((model("auto"), noclone ,noinline, long_call))
static int a_longcall(void) { asm (""); return 1; }

__attribute__ ((model("auto")))
extern int a_ext_func();

__attribute__ ((model("medium"), noclone ,noinline))
static int m_sta_fptr(int t()) { asm (""); return 1; }

__attribute__ ((model("medium"), noclone ,noinline))
static int m_sta_func(void) { asm (""); return 1; }

__attribute__ ((model("medium"), noclone ,noinline, long_call))
static int m_longcall(void) { asm (""); return 1; }

__attribute__ ((model("medium")))
extern int m_ext_func();

__attribute__ ((model("large"), noclone ,noinline))
static int l_sta_fptr(int t()) { asm (""); return 1; }

__attribute__ ((model("large"), noclone ,noinline))
static int l_sta_func(void) { asm (""); return 1; }

__attribute__ ((model("large"), noclone ,noinline, long_call))
static int l_longcall(void) { asm (""); return 1; }

__attribute__ ((model("large")))
extern int l_ext_func();

int
foo ()
{
  a_sta_func();
  a_longcall();
  a_ext_func();
  a_sta_fptr(&a_sta_func);
  a_sta_fptr(&a_ext_func);

  m_sta_func();
  m_longcall();
  m_ext_func();
  m_sta_fptr(&m_sta_func);
  m_sta_fptr(&m_ext_func);

  l_sta_func();
  l_longcall();
  l_ext_func();
  l_sta_fptr(&l_sta_func);
  l_sta_fptr(&l_ext_func);
  return 0;
}
