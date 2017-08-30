/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mno-pcrel -mno-gpopt" } */
/* { dg-skip-if "" { *-*-* } { "-O1"  "-O2" "-O3" "-Os" } { "" } } */

/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(a_sta_4Kaligned\\)" } } */
/* lw reg, 0(reg) # 1 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_2aligned" } } */
/* lw reg, 0(reg) # 2 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_char" } } */
/* lbu reg, 0(reg) # 1 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_short" } } */
/* lhu reg, 0(reg) # 1 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_int" } } */
/* lw reg, 0(reg) # 3 */

/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(a_sta_4Kaligned\\)" } } */
/* lw reg, 0(reg) # 4 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_2aligned" } } */
/* lw reg, 0(reg) # 5 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_char" } } */
/* lbu reg, 0(reg) # 2 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_short" } } */
/* lhu reg, 0(reg) # 2 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_int" } } */
/* lw reg, 0(reg) # 6 */

/* { dg-final { scan-assembler "lui\t\\\$\[ast0-9\]+,%hi\\(a_sta_4Kaligned\\)" } } */
/* lw reg, 0(reg) # 7 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_2aligned" } } */
/* lw reg, 0(reg) # 8 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_char" } } */
/* lbu reg, 0(reg) # 3 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_short" } } */
/* lhu reg, 0(reg) # 3 */
/* { dg-final { scan-assembler "\tli\t\\\$\[ast0-9\]+,a_sta_int" } } */
/* lw reg, 0(reg) # 9 */

/* { dg-final { scan-assembler-times "lbu\t\\\$\[ast0-9\]+,0\\(\\\$\[ast0-9\]+\\)" 3 } } */
/* { dg-final { scan-assembler-times "lhu\t\\\$\[ast0-9\]+,0\\(\\\$\[ast0-9\]+\\)" 3 } } */
/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,0\\(\\\$\[ast0-9\]+\\)" 9 } } */

static char a_sta_char __attribute__ ((model("auto"))) = 13;
static short a_sta_short __attribute__ ((model("auto"))) = 14;
static int a_sta_2aligned __attribute__ ((model("auto"), aligned (2))) = 15;
static int a_sta_4Kaligned __attribute__ ((model("auto"), aligned (4096))) = 16;
static int a_sta_int __attribute__ ((model("auto"))) = 17;

static char m_sta_char __attribute__ ((model("medium"))) = 13;
static short m_sta_short __attribute__ ((model("medium"))) = 14;
static int m_sta_2aligned __attribute__ ((model("medium"), aligned (2))) = 15;
static int m_sta_4Kaligned __attribute__ ((model("medium"), aligned (4096))) = 16;
static int m_sta_int __attribute__ ((model("medium"))) = 17;

static char l_sta_char __attribute__ ((model("large"))) = 13;
static short l_sta_short __attribute__ ((model("large"))) = 14;
static int l_sta_2aligned __attribute__ ((model("large"), aligned (2))) = 15;
static int l_sta_4Kaligned __attribute__ ((model("large"), aligned (4096))) = 16;
static int l_sta_int __attribute__ ((model("large"))) = 17;

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
