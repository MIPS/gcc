/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6s -fpic -mlong-calls" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* aluipc reg, %pcrel_hi(a_sta_func) # 1 */
/* addiu reg, reg, %lo(a_sta_func) # 1 */
/* jalrc reg # 1 */

/* aluipc reg, %pcrel_hi(a_longcall) */
/* addiu reg, reg, %lo(a_longcall) */
/* jalrc reg # 2 */

/* aluipc reg, %got_pcrel_hi(a_ext_func) # 1 */
/* lw reg, %got_lo(a_ext_func)(reg) # 1 */
/* jalrc reg # 3 */

/* aluipc reg, %pcrel_hi(a_sta_func) # 2 */
/* addiu reg, reg, %lo(a_sta_func) # 2 */
/* aluipc reg, %pcrel_hi(a_sta_fptr) # 1 */
/* addiu reg, reg, %lo(a_sta_fptr) # 1 */
/* jalrc reg # 4 */

/* aluipc reg, %got_pcrel_hi(a_ext_func) # 2 */
/* lw reg, %got_lo(a_ext_func)(reg) # 2 */
/* aluipc reg, %pcrel_hi(a_sta_fptr) # 2 */
/* addiu reg, reg, %lo(a_sta_fptr) # 2 */
/* jalrc reg # 5 */

/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(a_longcall\\)" } } */
/* { dg-final { scan-assembler "addiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(a_longcall\\)" } } */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(a_sta_func\\)" 2 } } */
/* { dg-final { scan-assembler-times "addiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(a_sta_func\\)" 2 } } */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(a_sta_fptr\\)" 2 } } */
/* { dg-final { scan-assembler-times "addiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(a_sta_fptr\\)" 2 } } */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%got_pcrel_hi\\(a_ext_func\\)" 2 } } */
/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,%got_lo\\(a_ext_func\\)\\(\\\$\[ast0-9\]+\\)" 2 } } */


/* aluipc reg, %pcrel_hi(m_sta_func) # 1 */
/* addiu reg, reg, %lo(m_sta_func) # 1 */
/* jalrc reg # 6 */

/* aluipc reg, %pcrel_hi(m_longcall) */
/* addiu reg, reg, %lo(m_longcall) */
/* jalrc reg # 7 */

/* aluipc reg, %got_pcrel_hi(m_ext_func) # 1 */
/* lw reg, %got_lo(m_ext_func)(reg) # 1 */
/* jalrc reg # 8 */

/* aluipc reg, %pcrel_hi(m_sta_func) # 2 */
/* addiu reg, reg, %lo(m_sta_func) # 2 */
/* aluipc reg, %pcrel_hi(m_sta_fptr) # 1 */
/* addiu reg, reg, %lo(m_sta_fptr) # 1 */
/* jalrc reg # 9 */

/* aluipc reg, %got_pcrel_hi(m_ext_func) # 2 */
/* lw reg, %got_lo(m_ext_func)(reg) # 2 */
/* aluipc reg, %pcrel_hi(m_sta_fptr) # 2 */
/* addiu reg, reg, %lo(m_sta_fptr) # 2 */
/* jalrc reg # 10 */

/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(m_longcall\\)" } } */
/* { dg-final { scan-assembler "addiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(m_longcall\\)" } } */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(m_sta_func\\)" 2 } } */
/* { dg-final { scan-assembler-times "addiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(m_sta_func\\)" 2 } } */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(m_sta_fptr\\)" 2 } } */
/* { dg-final { scan-assembler-times "addiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(m_sta_fptr\\)" 2 } } */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%got_pcrel_hi\\(m_ext_func\\)" 2 } } */
/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,%got_lo\\(m_ext_func\\)\\(\\\$\[ast0-9\]+\\)" 2 } } */


/* aluipc reg, %pcrel_hi(l_sta_func) # 1 */
/* addiu reg, reg, %lo(l_sta_func) # 1 */
/* jalrc reg # 11 */

/* aluipc reg, %pcrel_hi(l_longcall) */
/* addiu reg, reg, %lo(l_longcall) */
/* jalrc reg # 12 */

/* aluipc reg, %got_pcrel_hi(l_ext_func) # 1 */
/* lw reg, %got_lo(l_ext_func)(reg) # 1 */
/* jalrc reg # 13 */

/* aluipc reg, %pcrel_hi(l_sta_func) # 2 */
/* addiu reg, reg, %lo(l_sta_func) # 2 */
/* aluipc reg, %pcrel_hi(l_sta_fptr) # 1 */
/* addiu reg, reg, %lo(l_sta_fptr) # 1 */
/* jalrc reg # 14 */

/* aluipc reg, %got_pcrel_hi(l_ext_func) # 2 */
/* lw reg, %got_lo(l_ext_func)(reg) # 2 */
/* aluipc reg, %pcrel_hi(l_sta_fptr) # 2 */
/* addiu reg, reg, %lo(l_sta_fptr) # 2 */
/* jalrc reg # 15 */

/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(l_longcall\\)" } } */
/* { dg-final { scan-assembler "addiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(l_longcall\\)" } } */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(l_sta_func\\)" 2 } } */
/* { dg-final { scan-assembler-times "addiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(l_sta_func\\)" 2 } } */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(l_sta_fptr\\)" 2 } } */
/* { dg-final { scan-assembler-times "addiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(l_sta_fptr\\)" 2 } } */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%got_pcrel_hi\\(l_ext_func\\)" 2 } } */
/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,%got_lo\\(l_ext_func\\)\\(\\\$\[ast0-9\]+\\)" 2 } } */


/* { dg-final { scan-assembler-times "jalrc\t\\\$\[ast0-9\]+" 15 } } */

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
