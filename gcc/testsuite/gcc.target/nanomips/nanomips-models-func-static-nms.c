/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6s -mpcrel" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* balc a_static */

/* aluipc reg, %pcrel_hi(a_longcall) # 1 */
/* ori reg, reg, %lo(a_longcall) # 1 */
/* jalrc reg # 1 */

/* lapc reg, a_static # 1 */
/* balc a_takes_fptr # 1 */

/* aluipc reg, %pcrel_hi(a_longcall) # 2 */
/* ori reg, reg, %lo(a_longcall) # 2 */
/* balc a_takes_fptr # 2 */

/* lapc reg, a_static # 2 */
/* aluipc reg, %pcrel_hi(a_long_takes_fptr) # 1 */
/* ori reg, reg, %lo(a_long_takes_fptr) # 1 */
/* jalrc reg # 2 */

/* aluipc reg, %pcrel_hi(a_longcall) # 3 */
/* ori reg, reg, %lo(a_longcall) # 3 */
/* aluipc reg, %pcrel_hi(a_long_takes_fptr) # 2 */
/* ori reg, reg, %lo(a_long_takes_fptr) # 2 */
/* jalrc reg # 3 */

/* { dg-final { scan-assembler "balc\ta_static" } } */
/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(a_longcall\\)" 3 } } */
/* { dg-final { scan-assembler-times "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(a_longcall\\)" 3 } } */
/* { dg-final { scan-assembler-times "lapc\t\\\$\[ast0-9\]+,a_static" 2 } } */
/* { dg-final { scan-assembler-times "balc\ta_takes_fptr" 2 } } */
/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(a_long_takes_fptr\\)" 2 } } */
/* { dg-final { scan-assembler-times "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(a_long_takes_fptr\\)" 2 } } */

/* balc m_static */

/* aluipc reg, %pcrel_hi(m_longcall) # 1 */
/* ori reg, reg, %lo(m_longcall) # 1 */
/* jalrc reg # 4 */

/* aluipc reg, %pcrel_hi(m_static) # 1 */
/* ori reg, reg, %lo(m_static) # 1 */
/* balc m_takes_fptr # 1 */

/* aluipc reg, %pcrel_hi(m_longcall) # 2 */
/* ori reg, reg, %lo(m_longcall) # 2 */
/* balc m_takes_fptr # 2 */

/* aluipc reg, %pcrel_hi(m_static) # 2 */
/* ori reg, reg, %lo(m_static) # 2 */
/* aluipc reg, %pcrel_hi(m_long_takes_fptr) # 1 */
/* ori reg, reg, %lo(m_long_takes_fptr) # 1 */
/* jalrc reg # 5 */

/* aluipc reg, %pcrel_hi(m_longcall) # 3 */
/* ori reg, reg, %lo(m_longcall) # 3 */
/* aluipc reg, %pcrel_hi(m_long_takes_fptr) # 2 */
/* ori reg, reg, %lo(m_long_takes_fptr) # 2 */
/* jalrc reg # 6 */

/* { dg-final { scan-assembler "balc\tm_static" } } */
/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(m_longcall\\)" 3 } } */
/* { dg-final { scan-assembler-times "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(m_longcall\\)" 3 } } */
/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(m_static\\)" 2 } } */
/* { dg-final { scan-assembler-times "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(m_static\\)" 2 } } */
/* { dg-final { scan-assembler-times "balc\tm_takes_fptr" 2 } } */
/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(m_long_takes_fptr\\)" 2 } } */
/* { dg-final { scan-assembler-times "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(m_long_takes_fptr\\)" 2 } } */

/* aluipc reg, %pcrel_hi(l_static) # 1 */
/* ori reg, reg, %lo(l_static) # 1 */
/* jalrc reg # 7 */

/* aluipc reg, %pcrel_hi(l_longcall) # 1 */
/* ori reg, reg, %lo(l_longcall) # 1 */
/* jalrc reg # 8 */

/* aluipc reg, %pcrel_hi(l_static) # 2 */
/* ori reg, reg, %lo(l_static) # 2 */
/* aluipc reg, %pcrel_hi(l_takes_fptr) # 1 */
/* ori reg, reg, %lo(l_takes_fptr) # 1 */
/* jalrc reg # 9 */

/* aluipc reg, %pcrel_hi(l_longcall) # 2 */
/* ori reg, reg, %lo(l_longcall) # 2 */
/* aluipc reg, %pcrel_hi(l_takes_fptr) # 2 */
/* ori reg, reg, %lo(l_takes_fptr) # 2 */
/* jalrc reg # 10 */

/* aluipc reg, %pcrel_hi(l_static) # 3 */
/* ori reg, reg, %lo(l_static) # 3 */
/* aluipc reg, %pcrel_hi(l_long_takes_fptr) # 1 */
/* ori reg, reg, %lo(l_long_takes_fptr) # 1 */
/* jalrc reg # 11 */

/* aluipc reg, %pcrel_hi(l_longcall) # 3 */
/* ori reg, reg, %lo(l_longcall) # 3 */
/* aluipc reg, %pcrel_hi(l_long_takes_fptr) # 2 */
/* ori reg, reg, %lo(l_long_takes_fptr) # 2 */
/* jalrc reg # 12 */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(l_static\\)" 3 } } */
/* { dg-final { scan-assembler-times "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(l_static\\)" 3 } } */
/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(l_longcall\\)" 3 } } */
/* { dg-final { scan-assembler-times "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(l_longcall\\)" 3 } } */
/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(l_takes_fptr\\)" 2 } } */
/* { dg-final { scan-assembler-times "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(l_takes_fptr\\)" 2 } } */
/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(l_long_takes_fptr\\)" 2 } } */
/* { dg-final { scan-assembler-times "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(l_long_takes_fptr\\)" 2 } } */

/* { dg-final { scan-assembler-times "jalrc\t\\\$\[ast0-9\]+" 12 } } */

__attribute__ ((cmodel("auto"), noclone ,noinline))
static int a_takes_fptr(int bar()) { asm (""); return 1; }

__attribute__ ((cmodel("auto"), noclone ,noinline, long_call))
static int a_long_takes_fptr(int bar()) { asm (""); return 1; }

__attribute__ ((cmodel("auto"), noclone ,noinline))
static int a_static(void) { asm (""); return 1; }

__attribute__ ((cmodel("auto"), noclone ,noinline, long_call))
static int a_longcall(void) { asm (""); return 1; }

__attribute__ ((cmodel("medium"), noclone ,noinline))
static int m_takes_fptr(int bar()) { asm (""); return 1; }

__attribute__ ((cmodel("medium"), noclone ,noinline, long_call))
static int m_long_takes_fptr(int bar()) { asm (""); return 1; }

__attribute__ ((cmodel("medium"), noclone ,noinline))
static int m_static(void) { asm (""); return 1; }

__attribute__ ((cmodel("medium"), noclone ,noinline, long_call))
static int m_longcall(void) { asm (""); return 1; }

__attribute__ ((cmodel("large"), noclone ,noinline))
static int l_takes_fptr(int bar()) { asm (""); return 1; }

__attribute__ ((cmodel("large"), noclone ,noinline, long_call))
static int l_long_takes_fptr(int bar()) { asm (""); return 1; }

__attribute__ ((cmodel("large"), noclone ,noinline))
static int l_static(void) { asm (""); return 1; }

__attribute__ ((cmodel("large"), noclone ,noinline, long_call))
static int l_longcall(void) { asm (""); return 1; }

int
foo ()
{
  a_static();
  a_longcall();
  a_takes_fptr(&a_static);
  a_takes_fptr(&a_longcall);
  a_long_takes_fptr(&a_static);
  a_long_takes_fptr(&a_longcall);

  m_static();
  m_longcall();
  m_takes_fptr(&m_static);
  m_takes_fptr(&m_longcall);
  m_long_takes_fptr(&m_static);
  m_long_takes_fptr(&m_longcall);

  l_static();
  l_longcall();
  l_takes_fptr(&l_static);
  l_takes_fptr(&l_longcall);
  l_long_takes_fptr(&l_static);
  l_long_takes_fptr(&l_longcall);

  return 0;
}
