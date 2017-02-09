/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mno-pcrel" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* balc a_static */

/* li reg, a_longcall # 1 */
/* jalrc reg # 1 */

/* li reg, a_static # 1 */
/* balc a_takes_fptr # 1 */

/* li reg, a_longcall # 2 */
/* balc a_takes_fptr # 2 */

/* li reg, a_static # 2 */
/* li reg, a_long_takes_fptr # 1 */
/* jalrc reg # 2 */

/* li reg, a_longcall # 3 */
/* li reg, a_long_takes_fptr # 2 */
/* jalrc reg # 3 */

/* { dg-final { scan-assembler "balc\ta_static" } } */
/* { dg-final { scan-assembler-times "li\t\\\$\[ast0-9\]+,a_longcall" 3 } } */
/* { dg-final { scan-assembler-times "li\t\\\$\[ast0-9\]+,a_static" 2 } } */
/* { dg-final { scan-assembler-times "balc\ta_takes_fptr" 2 } } */
/* { dg-final { scan-assembler-times "li\t\\\$\[ast0-9\]+,a_long_takes_fptr" 2 } } */

/* balc m_static */

/* li reg, m_longcall # 1 */
/* jalrc reg # 4 */

/* li $a0, m_static # 1 */
/* balc m_takes_fptr # 1 */

/* li $a0, m_longcall # 2 */
/* balc m_takes_fptr # 2 */

/* li $a0, m_static # 2 */
/* li reg, m_long_takes_fptr # 1 */
/* jalrc reg # 5 */

/* li $a0, m_longcall # 3 */
/* li reg, m_long_takes_fptr # 2 */
/* jalrc reg # 6 */

/* { dg-final { scan-assembler "balc\tm_static" } } */
/* { dg-final { scan-assembler-times "li\t\\\$\[ast0-9\]+,m_longcall" 3 } } */
/* { dg-final { scan-assembler-times "li\t\\\$\[ast0-9\]+,m_static" 2 } } */
/* { dg-final { scan-assembler-times "balc\tm_takes_fptr" 2 } } */
/* { dg-final { scan-assembler-times "li\t\\\$\[ast0-9\]+,m_long_takes_fptr" 2 } } */

/* li reg, l_static # 1 */
/* jalrc reg # 7 */

/* li reg, l_longcall # 1 */
/* jalrc reg # 8 */

/* li $a0, l_static # 2 */
/* li reg, l_takes_fptr # 1 */
/* jalrc reg # 9 */

/* li $a0, l_longcall # 2 */
/* li reg, l_takes_fptr # 2 */
/* jalrc reg # 10 */

/* li $a0, l_static # 3 */
/* li reg, l_long_takes_fptr # 1 */
/* jalrc reg # 11 */

/* li $a0, l_longcall # 3 */
/* li reg, l_long_takes_fptr # 2 */
/* jalrc reg # 12 */

/* { dg-final { scan-assembler-times "li\t\\\$\[ast0-9\]+,l_static" 3 } } */
/* { dg-final { scan-assembler-times "li\t\\\$\[ast0-9\]+,l_longcall" 3 } } */
/* { dg-final { scan-assembler-times "li\t\\\$\[ast0-9\]+,l_takes_fptr" 2 } } */
/* { dg-final { scan-assembler-times "li\t\\\$\[ast0-9\]+,l_long_takes_fptr" 2 } } */

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
