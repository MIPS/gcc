/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mpcrel -mgpopt -mno-pid -G2" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(a_sta_4Kaligned\\)" } } */
/* { dg-final { scan-assembler "lapc.h\t\\\$\[ast0-9\]+,a_sta_2aligned" } } */
/* { dg-final { scan-assembler "addiu\\.b\t\\\$\[ast0-9\]+,\\\$gp,%gprel\\(a_sta_char\\)" } } */
/* { dg-final { scan-assembler "addiu\\.b\t\\\$\[ast0-9\]+,\\\$gp,%gprel\\(a_sta_short\\)" } } */
/* { dg-final { scan-assembler "lapc.h\t\\\$\[ast0-9\]+,a_sta_int" } } */

/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(m_sta_4Kaligned\\)" } } */
/* { dg-final { scan-assembler "lapc.b\t\\\$\[ast0-9\]+,m_sta_2aligned" } } */
/* { dg-final { scan-assembler "addiu\\.b\t\\\$\[ast0-9\]+,\\\$gp,%gprel\\(m_sta_char\\)" } } */
/* { dg-final { scan-assembler "addiu\\.b\t\\\$\[ast0-9\]+,\\\$gp,%gprel\\(m_sta_short\\)" } } */
/* { dg-final { scan-assembler "lapc.b\t\\\$\[ast0-9\]+,m_sta_int" } } */

/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(l_sta_4Kaligned\\)" } } */
/* { dg-final { scan-assembler "lapc.b\t\\\$\[ast0-9\]+,l_sta_2aligned" } } */
/* { dg-final { scan-assembler "addiu\\.b32\t\\\$\[ast0-9\]+,\\\$gp,%gprel32\\(l_sta_char\\)" } } */
/* { dg-final { scan-assembler "addiu\\.b32\t\\\$\[ast0-9\]+,\\\$gp,%gprel32\\(l_sta_short\\)" } } */
/* { dg-final { scan-assembler "lapc.b\t\\\$\[ast0-9\]+,l_sta_int" } } */

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
  int *a1 = &a_sta_4Kaligned;
  int *a2 = &a_sta_2aligned;
  char *a3 = &a_sta_char;
  short *a4 = &a_sta_short;
  int *a5 = &a_sta_int;

  int *a6 = &m_sta_4Kaligned;
  int *a7 = &m_sta_2aligned;
  char *a8 = &m_sta_char;
  short *a9 = &m_sta_short;
  int *a10 = &m_sta_int;

  int *a11 = &l_sta_4Kaligned;
  int *a12 = &l_sta_2aligned;
  char *a13 = &l_sta_char;
  short *a14 = &l_sta_short;
  int *a15 = &l_sta_int;
  return 0;
}
