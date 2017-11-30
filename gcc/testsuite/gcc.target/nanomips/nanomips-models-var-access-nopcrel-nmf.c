/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mno-pcrel -mno-gpopt -mno-pid" } */
/* { dg-skip-if "" { *-*-* } { "-O1"  "-O2" "-O3" "-Os" } { "" } } */

/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(a_sta_4Kaligned\\)" } } */
/* lw reg, 0(reg) # 1 */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(a_sta_2aligned\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%lo\\(a_sta_2aligned\\)\\(\\\$\[ast0-9\]+\\)" } } */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(a_sta_char\\)" } } */
/* { dg-final { scan-assembler "lbu\t\\\$\[ast0-9\]+,%lo\\(a_sta_char\\)\\(\\\$\[ast0-9\]+\\)" } } */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(a_sta_short\\)" } } */
/* { dg-final { scan-assembler "lhu\t\\\$\[ast0-9\]+,%lo\\(a_sta_short\\)\\(\\\$\[ast0-9\]+\\)" } } */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(a_sta_int\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%lo\\(a_sta_int\\)\\(\\\$\[ast0-9\]+\\)" } } */

/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(m_sta_4Kaligned\\)" } } */
/* lw reg, 0(reg) # 2 */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(m_sta_2aligned\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%lo\\(m_sta_2aligned\\)\\(\\\$\[ast0-9\]+\\)" } } */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(m_sta_char\\)" } } */
/* { dg-final { scan-assembler "lbu\t\\\$\[ast0-9\]+,%lo\\(m_sta_char\\)\\(\\\$\[ast0-9\]+\\)" } } */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(m_sta_short\\)" } } */
/* { dg-final { scan-assembler "lhu\t\\\$\[ast0-9\]+,%lo\\(m_sta_short\\)\\(\\\$\[ast0-9\]+\\)" } } */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(m_sta_int\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%lo\\(m_sta_int\\)\\(\\\$\[ast0-9\]+\\)" } } */

/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(l_sta_4Kaligned\\)" } } */
/* lw reg, 0(reg) # 3 */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(l_sta_2aligned\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%lo\\(l_sta_2aligned\\)\\(\\\$\[ast0-9\]+\\)" } } */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(l_sta_char\\)" } } */
/* { dg-final { scan-assembler "lbu\t\\\$\[ast0-9\]+,%lo\\(l_sta_char\\)\\(\\\$\[ast0-9\]+\\)" } } */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(l_sta_short\\)" } } */
/* { dg-final { scan-assembler "lhu\t\\\$\[ast0-9\]+,%lo\\(l_sta_short\\)\\(\\\$\[ast0-9\]+\\)" } } */
/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(l_sta_int\\)" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%lo\\(l_sta_int\\)\\(\\\$\[ast0-9\]+\\)" } } */

/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,0\\(\\\$\[ast0-9\]+\\)" 3 } } */

static char a_sta_char __attribute__ ((cmodel("auto"))) = 13;
static short a_sta_short __attribute__ ((cmodel("auto"))) = 14;
static int a_sta_2aligned __attribute__ ((cmodel("auto"), aligned (2))) = 15;
static int a_sta_4Kaligned __attribute__ ((cmodel("auto"), aligned (4096))) = 16;
static int a_sta_int __attribute__ ((cmodel("auto"))) = 17;

static char m_sta_char __attribute__ ((cmodel("medium"))) = 13;
static short m_sta_short __attribute__ ((cmodel("medium"))) = 14;
static int m_sta_2aligned __attribute__ ((cmodel("medium"), aligned (2))) = 15;
static int m_sta_4Kaligned __attribute__ ((cmodel("medium"), aligned (4096))) = 16;
static int m_sta_int __attribute__ ((cmodel("medium"))) = 17;

static char l_sta_char __attribute__ ((cmodel("large"))) = 13;
static short l_sta_short __attribute__ ((cmodel("large"))) = 14;
static int l_sta_2aligned __attribute__ ((cmodel("large"), aligned (2))) = 15;
static int l_sta_4Kaligned __attribute__ ((cmodel("large"), aligned (4096))) = 16;
static int l_sta_int __attribute__ ((cmodel("large"))) = 17;

int
foo ()
{
  int a1 = a_sta_4Kaligned;
  int a2 = a_sta_2aligned;
  char a3 = a_sta_char;
  short a4 = a_sta_short;
  int a5 = a_sta_int;

  int a6 = m_sta_4Kaligned;
  int a7 = m_sta_2aligned;
  char a8 = m_sta_char;
  short a9 = m_sta_short;
  int a10 = m_sta_int;

  int a11 = l_sta_4Kaligned;
  int a12 = l_sta_2aligned;
  char a13 = l_sta_char;
  short a14 = l_sta_short;
  int a15 = l_sta_int;
  return 0;
}
