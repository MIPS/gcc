/* { dg-do run } */
/* { dg-require-effective-target arm_cmse_ok } */
/* { dg-options "--save-temps -mcmse -Wl,--section-start,.gnu.sgstubs=0x20400000" } */
#include <limits.h>
#include <stdlib.h>
typedef struct
{
  unsigned char	  a;
  unsigned short  b;
} test_st;

test_st __attribute__ ((cmse_nonsecure_entry)) foo (void)
{
  test_st t;
  t.a = UCHAR_MAX;
  t.b = USHRT_MAX;
  return t;
}

int
main (void)
{
  test_st t;
  t = foo ();
  if (t.a != UCHAR_MAX || t.b != USHRT_MAX)
    abort ();
  return 0;
}

/* { dg-final { scan-assembler "movs\tr1, #255" } } */
/* { dg-final { scan-assembler "movt\tr1, 65535" } } */
/* { dg-final { scan-assembler "ands\tr0(, r0)?, r1" } } */


