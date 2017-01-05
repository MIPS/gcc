/* { dg-options "-mno-abicalls -mgpopt -G8 -mabi=32 -mips16 -mmips16e2" } */
/* { dg-skip-if "per-function expected output" { *-*-* } { "-flto" } { "" } } */

/* Generate GP-relative ADDIU.  */

/* { dg-final { scan-assembler "test01:.*\taddiu\t\\\$2,\\\$28.*test01\n" } } */
int arr[2];

int *
test01 (void)
{
  return (&arr[1]);
}

/* ANDI is a two operand instruction.  Hence, it won't be generated if src and
   dest are in different registers.  */

/* { dg-final { scan-assembler "test02:.*\tandi\t.*test02\n" } } */
unsigned int
test02 (unsigned int a)
{
  return ((a + 0x2) & 0x3ff);
}

/* { dg-final { scan-assembler "test03:.*\text\t.*test03\n" } } */
struct
{
  unsigned int a:9;
  unsigned int d:31;
  unsigned int e:9;
  unsigned int f:10;
} t03;

unsigned int
test03 (void)
{
  return t03.f;
}

/* Use EXT when ANDing with low-order bitmasks.  */

/* { dg-final { scan-assembler "test04:.*\text\t.*test04\n" } } */
/* { dg-final { scan-assembler-not "test04.*\tandi?\t.*test04\n" } } */
unsigned int
test04 (unsigned int x)
{
  return (x & 0x1fffffff);
}

/* { dg-final { scan-assembler "test05:.*\tins\t.*test05\n" } } */
struct
{
  unsigned int i : 9;
  unsigned int j : 15;
  unsigned int k : 4;
} s05;

void
test05 (void)
{
  s05.j = 1;
}

/* Use INS with hardcoded $0.  */

/* { dg-final { scan-assembler "test06:.*\tins\t\\\$.*,\\\$0.*test06\n" } } */
struct
{
  unsigned int i : 8;
  unsigned int j : 9;
  unsigned int k : 10;
} __attribute__ ((packed)) s06;

void
test06 (void)
{
  s06.k = 0;
}

/* Use INS when ANDing to clear only one consecutive chunk of bits.  */

/* { dg-final { scan-assembler "test07:.*\tins\t\\\$.*,\\\$0,11,5.*test07\n" } } */
/* { dg-final { scan-assembler-not "test07:.*\tandi?\t.*test07\n" } } */
unsigned int
test07 (unsigned int x)
{
  return (x & 0xffff07ff);
}

/* Test LB[GP].  */

/* { dg-final { scan-assembler "test08:.*\tlb\t.*\\\$28.*test08\n" } } */
signed char c08;

signed char
test08 (void)
{
  return c08;
}

/* Test LBU[GP].  */

/* { dg-final { scan-assembler "test09:.*\tlbu\t.*\\\$28.*test09\n" } } */
unsigned char uc09;

unsigned char
test09 (void)
{
  return uc09;
}

/* Test LH[GP].  */

/* { dg-final { scan-assembler "test10:.*\tlh\t.*\\\$28.*test10\n" } } */
short s10;

short
test10 (void)
{
  return s10;
}

/* Test LHU[GP].  */

/* { dg-final { scan-assembler "test11:.*\tlhu\t.*\\\$28.*test11\n" } } */
unsigned short s11;

unsigned short
test11 (void)
{
  return s11;
}

/* Test LUI.  */

/* { dg-final { scan-assembler "test12:.*\tlui\t.*test12\n" } } */
int
test12 (void)
{
  return 0x44440000;
}

/* Test LW[GP].  */

/* { dg-final { scan-assembler "test13:.*\tlw\t.*\\\$28.*test13\n" } } */
int i13;

int
test13 (void)
{
  return i13;
}

/* ORI is a two operand instruction.  Hence, it won't be generated if src and
   dest are in different registers.  */

/* { dg-final { scan-assembler "test14:.*\tori\t.*test14\n" } } */
unsigned int
test14 (unsigned int a)
{
  return (a + 0x2) | 0x7f0;
}

/* Test SB[GP].  */

/* { dg-final { scan-assembler "test15:.*\tsb\t.*\\\$28.*test15\n" } } */
char c15;

void
test15 (char x)
{
  c15 = x;
}

/* Test SH[GP].  */

/* { dg-final { scan-assembler "test16:.*\tsh\t.*\\\$28.*test16\n" } } */
short s16;

void
test16 (short x)
{
  s16 = x;
}

/* Test SW[GP].  */

/* { dg-final { scan-assembler "test17:.*\tsw\t.*\\\$28.*test17\n" } } */
int i17;

void
test17 (int x)
{
  i17 = x;
}

/* Test 32bit unaligned load.  */

/* { dg-final { scan-assembler "test18:.*\tlwl\t.*test18\n" } } */
/* { dg-final { scan-assembler "test18:.*\tlwr\t.*test18\n" } } */
struct node18
{
  char c;
  int i;
} __attribute__ ((packed)) obj18 __attribute__((aligned(1)));

int
test18 (void)
{
  return obj18.i;
}

/* Test 32bit unaligned load.  */

/* { dg-final { scan-assembler "test19:.*\tlwl\t.*test19\n" } } */
/* { dg-final { scan-assembler "test19:.*\tlwr\t.*test19\n" } } */
struct node19
{
  unsigned int i : 8;
  unsigned int j : 32;
} __attribute__ ((packed)) obj19 __attribute__((aligned(16)));

int
test19 (void)
{
  return obj19.j;
}

/* Test 32bit unaligned store with non-zero constant */

/* { dg-final { scan-assembler "test20:.*\tswl\t.*test20\n" } } */
/* { dg-final { scan-assembler "test20:.*\tswr\t.*test20\n" } } */
struct node20
{
  char c;
  int i;
} __attribute__ ((packed)) obj20 __attribute__((aligned(1)));

void
test20 (void)
{
  obj20.i = 1234;
}

/* Test 32bit unaligned store with zero constant.  */

/* { dg-final { scan-assembler "test21:.*\tswl\t.*test21\n" } } */
/* { dg-final { scan-assembler "test21:.*\tswr\t.*test21\n" } } */
/* { dg-final { scan-assembler-not "test21:.*\tswl\t\\\$0,.*test21\n" } } */
/* { dg-final { scan-assembler-not "test21:.*\tswr\t\\\$0,.*test21\n" } } */
struct node21
{
  char c;
  int i;
} __attribute__ ((packed)) obj21 __attribute__((aligned(1)));

void
test21 (void)
{
  obj21.i = 0;
}

/* Test 32bit unaligned store with non-constant.  */

/* { dg-final { scan-assembler "test22:.*\tswl\t.*test22\n" } } */
/* { dg-final { scan-assembler "test22:.*\tswr\t.*test22\n" } } */
struct node22
{
  char c;
  int i;
} __attribute__ ((packed)) obj22 __attribute__((aligned(1)));

int i22 = 1234;

void
test22 (void)
{
  obj22.i = i22;
}

/* Test 32bit unaligned store with non-constant */

/* { dg-final { scan-assembler "test23:.*\tswl\t.*test23\n" } } */
/* { dg-final { scan-assembler "test23:.*\tswr\t.*test23\n" } } */
struct node23
{
  char c;
  int i;
} __attribute__ ((packed)) obj23 __attribute__((aligned(1)));

void
test23 (int i)
{
  obj23.i = i;
}

/* Test 32bit unaligned store with non-constant.  */

/* { dg-final { scan-assembler "test24:.*\tswl\t.*test24\n" } } */
/* { dg-final { scan-assembler "test24:.*\tswr\t.*test24\n" } } */
struct node24
{
  unsigned int i : 8;
  unsigned int j : 32;
} __attribute__ ((packed)) obj24 __attribute__((aligned(16)));

void
test24 (int i)
{
  obj24.j = i;
}

/* XORI is a two operand instruction.  Hence, it won't be generated if src and
   dest are in different registers.  */

/* { dg-final { scan-assembler "test25:.*\txori\t.*test25\n" } } */
unsigned int
test25 (unsigned int a)
{
  return ((a + 0x2) ^ 0x3f0);
}
