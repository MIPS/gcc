/* { dg-do run { target { powerpc*-*-linux* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-skip-if "" { powerpc*-*-*spe* } { "*" } { "" } } */
/* { dg-require-effective-target ppc_recip_hw } */
/* { dg-options "-O2 -mfloat128-ref -static-libgcc" } */

/*
 * Test program to make sure we are getting more precision than the 53 bits we
 * get with IEEE double.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stddef.h>

#ifndef TYPE
#define TYPE __float128
#endif

#ifndef NO_INLINE
#define NO_INLINE __attribute__((__noinline__))
#endif

static TYPE power_of_two (ssize_t) NO_INLINE;
static TYPE calc1 (TYPE) NO_INLINE;
static TYPE calc2 (TYPE) NO_INLINE;
static TYPE calc3 (TYPE) NO_INLINE;


/*
 * Print TYPE in hex.
 */


#ifdef DEBUG
static void print_hex (const char *prefix, TYPE, const char *suffix) NO_INLINE;

#if defined (__i386__) || defined (__x86_64__) || defined (__LITTLE_ENDIAN__)
#define ENDIAN_REVERSE(N, MAX)        ((MAX) - 1 - (N))

#else
#define ENDIAN_REVERSE(N, MAX)        (N)
#endif

static void
print_hex (const char *prefix, TYPE value, const char *suffix)
{
  union {
    TYPE f128;
    unsigned char uc[sizeof (TYPE)];
  } u;

  size_t i;

  u.f128 = value;
  printf ("%s0x", prefix);
  for (i = 0; i < sizeof (TYPE); i++)
    printf ("%.2x", u.uc[ ENDIAN_REVERSE (i, sizeof (TYPE)) ]);

  printf (", %24.2Lf%s", (long double)value, suffix);
}
#endif


/*
 * Return a power of two.
 */

static TYPE
power_of_two (ssize_t num)
{
  TYPE ret = (TYPE) 1.0;
  ssize_t i;

  if (num >= 0)
    {
      for (i = 0; i < num; i++)
	ret *= (TYPE) 2.0;
    }
  else
    {
      ssize_t num2 = -num;
      for (i = 0; i < num2; i++)
	ret /= (TYPE) 2.0;
    }

#ifdef DEBUG
  printf ("power_of_two (%2ld)   = ", (long) num);
  print_hex ("", ret, "\n");
#endif

  return ret;
}


/*
 * Various calculations.  Add in 2**60, and subtract 2**59 twice, and we should
 * get the original value.
 */

static TYPE
calc1 (TYPE num)
{
  TYPE num2 = power_of_two (60) + num;
  TYPE ret;

#ifdef DEBUG
  print_hex ("calc1 (before call) = ", num2, "\n");
#endif

  ret = calc2 (num2);

#ifdef DEBUG
  print_hex ("calc1 (after call)  = ", ret, "\n");
#endif

  return ret;
}

static TYPE
calc2 (TYPE num)
{
  TYPE num2 = num - power_of_two (59);
  TYPE ret;

#ifdef DEBUG
  print_hex ("calc2 (before call) = ", num2, "\n");
#endif

  ret = calc3 (num2);

#ifdef DEBUG
  print_hex ("calc2 (after call)  = ", ret, "\n");
#endif

  return ret;
}

static TYPE
calc3 (TYPE num)
{
  TYPE ret = num - (((TYPE) 2.0) * power_of_two (58));

#ifdef DEBUG
  print_hex ("calc3               = ", ret, "\n");
#endif

  return ret;
}


int
main (void)
{
  TYPE input, output;

#ifdef DEBUG
  printf ("Testing, %ld bytes\n", (long) sizeof (TYPE));
#endif

  input = power_of_two (-1);
  output = calc1 (input);

  if (input != (TYPE) 0.5)
    {
#ifdef DEBUG
      print_hex ("Input should be 0.5:  ", output, "\n");
#endif
      __builtin_abort ();
    }

  if (output != (TYPE) 0.5)
    {
#ifdef DEBUG
      print_hex ("Output should be 0.5: ", output, "\n");
#endif
      __builtin_abort ();
    }

  return 0;
}
