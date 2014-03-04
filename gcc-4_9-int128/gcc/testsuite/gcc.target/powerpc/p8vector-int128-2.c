/* { dg-do run { target { powerpc*-*-linux* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-skip-if "" { powerpc*-*-*spe* } { "*" } { "" } } */
/* { dg-require-effective-target p8vector_hw } */
/* { dg-options "-mcpu=power8 -O2" } */

#include <stddef.h>
#include <stdlib.h>
#include <altivec.h>

#ifdef DEBUG
#include <stdio.h>

#ifdef __LITTLE_ENDIAN__
#define HI_WORD 1
#define LO_WORD 0
#else
#define HI_WORD 0
#define LO_WORD 1
#endif
#endif

static int errors = 0;

static void
compare (__uint128_t a, __uint128_t b, size_t n)
{
  union {
    __uint128_t u128;
    unsigned long u64[2];
  } u1, u2;

  if (a == b)
    {
#ifdef DEBUG
      u1.u128 = a;
      u2.u128 = b;
      fprintf (stderr,
	       "Passed on test %d: 0x%.16lx 0x%.16lx == 0x%.16lx 0x%.16lx\n",
	       (int) n,
	       u1.u64[HI_WORD], u1.u64[LO_WORD],
	       u2.u64[HI_WORD], u2.u64[LO_WORD]);
#endif
      return;
    }

#ifdef DEBUG
  u1.u128 = a;
  u2.u128 = b;

  errors++;
  fprintf (stderr,
	   "Failed on test %d: 0x%.16lx 0x%.16lx != 0x%.16lx 0x%.16lx\n",
	   (int) n,
	   u1.u64[HI_WORD], u1.u64[LO_WORD],
	   u2.u64[HI_WORD], u2.u64[LO_WORD]);
#else
  abort ();
#endif
}

/* Check if vadduqm returns the same values as normal 128-bit add.  */

/* Values to add together.  */
const static struct {
  unsigned long hi_1;
  unsigned long lo_1;
  unsigned long hi_2;
  unsigned long lo_2;
} values[] = {
  { 0x0000000000000000UL, 0xfffffffffffffffeUL,
    0x0000000000000000UL, 0x0000000000000002UL },
  { 0x0000000000000000UL, 0x0000000000000002UL,
    0x0000000000000000UL, 0xfffffffffffffffeUL },
  { 0xffffffffffffffffUL, 0xfffffffffffffffeUL,
    0x0000000000000000UL, 0x0000000000000002UL },
  { 0xfffffffffffffff2UL, 0xffffffffffffffffUL,
    0x0000000000000002UL, 0x0000000000000000UL },
  { 0x7fffffffffffffffUL, 0xfffffffffffffffeUL,
    0x0000000000000000UL, 0x0000000000000002UL },
  { 0x7ffffffffffffff2UL, 0xffffffffffffffffUL,
    0x0000000000000002UL, 0x0000000000000000UL },
};

int
main (void)
{
  size_t i;
  __uint128_t res1, res2, inp1, inp2;

  for (i = 0; i < sizeof (values) / sizeof (values[0]); i++)
    {
      inp1 = ((((__uint128_t)values[i].hi_1 << 64)) + ((__uint128_t)values[i].lo_1));
      inp2 = ((((__uint128_t)values[i].hi_2 << 64)) + ((__uint128_t)values[i].lo_2));
      res1 = inp1 + inp2;
      res2 = vec_vadduqm (inp1, inp2);
      compare (res1, res2, i);
    }

  return errors;
}
