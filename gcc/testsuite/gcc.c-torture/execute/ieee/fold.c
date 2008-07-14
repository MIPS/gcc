/* Test that a folded operation computes the same value as a non-folded
   operation.  Specifically with values that are denorms.  
   The constants used are valid for IEEE formats. */

#include <stdio.h>
extern void abort(void);
extern void exit(int);


union u {
  float f;
  unsigned int i;
  double d;
  unsigned long long l;
};

static const double d_smallest_norm = 0x1p-1022;
static const double d_max_denorm  = 0x0.8p-1022;
static const double d_smallest_denorm = 0x0.0000000000001p-1022;

static const float smallest_norm = 1.1754943508222875079687365372222456778186655567720875215087517062784172594547271728515625e-38f;
static const float max_denorm  = 5.8774717541114375398436826861112283890933277838604376075437585313920862972736358642578125e-39f;
static const float smallest_denorm = 1.40129846432481707092372958328991613128026194187651577175706828388979108268586060148663818836212158203125e-45f;

static const double pinfd  = +1.0 / 0.0;
static const double pinf  = +1.0 / 0.0;

volatile union u a, b;

int failed;

#define TEST_OP(A,B,OP) \
  do { \
    union u u, v; \
    a.f = A; \
    b.f = B; \
    u.f = A OP B; \
    v.f = a.f OP b.f; \
    if (u.i != v.i) \
    { \
      printf( #A #OP #B " = %08x " #OP " %08x = const %08x = volatile %08x\n", a.i, b.i, u.i, v.i ); \
      failed++; \
    } \
  } while (0);

#define TEST_OPD(A,B,OP) \
  do { \
    union u u, v; \
    a.d = A; \
    b.d = B; \
    u.d = A OP B; \
    v.d = a.d OP b.d; \
    if (u.l != v.l) \
    { \
      printf( #A #OP #B " = %016llx " #OP " %016llx = const %016llx = volatile %016llx\n", a.l, b.l, u.l, v.l ); \
      failed++; \
    } \
  } while (0);

#define TEST_FTOD(A) \
  do { \
    union u u, v; \
    a.f = A; \
    u.d = A; \
    v.d = a.f; \
    if (u.l != v.l) \
    { \
      printf( "(double)" #A " = %08x = const %016llx = volatile %016llx\n", a.i, u.l, v.l ); \
      failed++; \
    } \
  } while (0);

#define TEST_DTOF(A) \
  do { \
    union u u, v; \
    a.d = A; \
    u.f = A; \
    v.f = a.d; \
    if (u.i != v.i) \
    { \
      printf( "(float)" #A " = %016llx = const %08x = volatile %08x\n", a.l, u.i, v.i ); \
      failed++; \
    } \
  } while (0);

#define TEST_FUNC(A,OP) \
  do { \
    union u u, v; \
    a.f = A; \
    u.f = OP (A); \
    v.f = OP (a.f); \
    if (u.i != v.i) \
    { \
      printf( #OP "(" #A ") = " #OP " (%08x) = const %08x = volatile %08x\n", a.i, u.i, v.i ); \
      failed++; \
    } \
  } while (0);

int
main()
{
  union u u;
  /* Check constant strings are represented exactly. */
  u.f = smallest_norm;
  if (u.i != 0x00800000)
    {
      printf("00800000  %08x\n", u.i);
      failed++;
    }
  u.f = max_denorm;
  if (u.i != 0x00400000)
    {
      printf("00400000  %08x\n", u.i);
      failed++;
    }
  u.f = smallest_denorm;
  if (u.i != 0x00000001)
    {
      printf("00000001  %08x\n", u.i);
      failed++;
    }

  TEST_OP(smallest_denorm, max_denorm, +);
  TEST_OP(smallest_norm, max_denorm, +);
  TEST_OP(smallest_norm, smallest_denorm, +);

  TEST_OP(max_denorm, 2.0f, *);
  TEST_OP(max_denorm, 0.25f, *);
  TEST_OP(smallest_norm, 2.0f, *);
  TEST_OP(smallest_norm, 0.25f, *);

  TEST_OP(max_denorm, 4.0f, /);
  TEST_OP(max_denorm, 0.5f, /);
  TEST_OP(smallest_norm, 4.0f, /);

  TEST_OP(smallest_norm, 0.5f, /);

  TEST_OP(smallest_denorm, max_denorm, <);
  TEST_OP(smallest_norm, max_denorm, <);
  TEST_OP(smallest_norm, smallest_denorm, <);
  TEST_OP(smallest_denorm, max_denorm, >);
  TEST_OP(smallest_norm, max_denorm, >);
  TEST_OP(smallest_norm, smallest_denorm, >);
  TEST_OP(smallest_denorm, max_denorm, ==);
  TEST_OP(smallest_norm, max_denorm, ==);
  TEST_OP(smallest_norm, smallest_denorm, ==);

  TEST_OP(pinf, pinf, !=);
  TEST_OP(pinf, pinf, ==);

  TEST_OPD(d_smallest_denorm, d_max_denorm, +);
  TEST_OPD(d_smallest_norm, d_max_denorm, +);
  TEST_OPD(d_smallest_norm, d_smallest_denorm, +);

  TEST_OPD(d_max_denorm, 2.0, *);
  TEST_OPD(d_max_denorm, 0.25, *);
  TEST_OPD(d_smallest_norm, 2.0, *);
  TEST_OPD(d_smallest_norm, 0.25, *);

  TEST_OPD(d_max_denorm, 4.0, /);
  TEST_OPD(d_max_denorm, 0.5, /);
  TEST_OPD(d_smallest_norm, 4.0, /);

  TEST_OPD(d_smallest_norm, 0.5, /);

  TEST_OPD(d_smallest_denorm, d_max_denorm, <);
  TEST_OPD(d_smallest_norm, d_max_denorm, <);
  TEST_OPD(d_smallest_norm, d_smallest_denorm, <);
  TEST_OPD(d_smallest_denorm, d_max_denorm, >);
  TEST_OPD(d_smallest_norm, d_max_denorm, >);
  TEST_OPD(d_smallest_norm, d_smallest_denorm, >);
  TEST_OPD(d_smallest_denorm, d_max_denorm, ==);
  TEST_OPD(d_smallest_norm, d_max_denorm, ==);
  TEST_OPD(d_smallest_norm, d_smallest_denorm, ==);

  TEST_OPD(pinfd, pinfd, !=);
  TEST_OPD(pinfd, pinfd, ==);
  TEST_OPD(pinfd, pinf, ==);

  TEST_FUNC(smallest_norm, __builtin_sqrtf); 

  TEST_FUNC(smallest_denorm, __builtin_sqrtf);
  TEST_FUNC(max_denorm, __builtin_sqrtf);
  TEST_FUNC(4.0f, __builtin_sqrtf);
  TEST_FUNC(2.0f, __builtin_sqrtf);
  TEST_FUNC(0.5f, __builtin_sqrtf);

  TEST_FTOD(smallest_norm);
  TEST_FTOD(smallest_denorm);
  TEST_FTOD(max_denorm);
  TEST_FTOD(-0.0f);

  TEST_DTOF(d_smallest_norm);
  TEST_DTOF(d_smallest_denorm);
  TEST_DTOF(d_max_denorm);
  TEST_DTOF(-0.0);

  if (failed > 0)
    abort();
  exit(0);
}
/* Test that a folded operation computes the same value as a non-folded
   operation.  Specifically with values that are denorms.  
   The constants used are valid for IEEE formats. */

#include <stdio.h>
extern void abort(void);
extern void exit(int);


union u {
  float f;
  unsigned int i;
  double d;
  unsigned long long l;
};

static const double d_smallest_norm = 0x1p-1022;
static const double d_max_denorm  = 0x0.8p-1022;
static const double d_smallest_denorm = 0x0.0000000000001p-1022;

static const float smallest_norm = 1.1754943508222875079687365372222456778186655567720875215087517062784172594547271728515625e-38f;
static const float max_denorm  = 5.8774717541114375398436826861112283890933277838604376075437585313920862972736358642578125e-39f;
static const float smallest_denorm = 1.40129846432481707092372958328991613128026194187651577175706828388979108268586060148663818836212158203125e-45f;

static const double pinfd  = +1.0 / 0.0;
static const double pinf  = +1.0 / 0.0;

volatile union u a, b;

int failed;

#define TEST_OP(A,B,OP) \
  do { \
    union u u, v; \
    a.f = A; \
    b.f = B; \
    u.f = A OP B; \
    v.f = a.f OP b.f; \
    if (u.i != v.i) \
    { \
      printf( #A #OP #B " = %08x " #OP " %08x = const %08x = volatile %08x\n", a.i, b.i, u.i, v.i ); \
      failed++; \
    } \
  } while (0);

#define TEST_OPD(A,B,OP) \
  do { \
    union u u, v; \
    a.d = A; \
    b.d = B; \
    u.d = A OP B; \
    v.d = a.d OP b.d; \
    if (u.l != v.l) \
    { \
      printf( #A #OP #B " = %016llx " #OP " %016llx = const %016llx = volatile %016llx\n", a.l, b.l, u.l, v.l ); \
      failed++; \
    } \
  } while (0);

#define TEST_FTOD(A) \
  do { \
    union u u, v; \
    a.f = A; \
    u.d = A; \
    v.d = a.f; \
    if (u.l != v.l) \
    { \
      printf( "(double)" #A " = %08x = const %016llx = volatile %016llx\n", a.i, u.l, v.l ); \
      failed++; \
    } \
  } while (0);

#define TEST_DTOF(A) \
  do { \
    union u u, v; \
    a.d = A; \
    u.f = A; \
    v.f = a.d; \
    if (u.i != v.i) \
    { \
      printf( "(float)" #A " = %016llx = const %08x = volatile %08x\n", a.l, u.i, v.i ); \
      failed++; \
    } \
  } while (0);

#define TEST_FUNC(A,OP) \
  do { \
    union u u, v; \
    a.f = A; \
    u.f = OP (A); \
    v.f = OP (a.f); \
    if (u.i != v.i) \
    { \
      printf( #OP "(" #A ") = " #OP " (%08x) = const %08x = volatile %08x\n", a.i, u.i, v.i ); \
      failed++; \
    } \
  } while (0);

int
main()
{
  union u u;
  /* Check constant strings are represented exactly. */
  u.f = smallest_norm;
  if (u.i != 0x00800000)
    {
      printf("00800000  %08x\n", u.i);
      failed++;
    }
  u.f = max_denorm;
  if (u.i != 0x00400000)
    {
      printf("00400000  %08x\n", u.i);
      failed++;
    }
  u.f = smallest_denorm;
  if (u.i != 0x00000001)
    {
      printf("00000001  %08x\n", u.i);
      failed++;
    }

  TEST_OP(smallest_denorm, max_denorm, +);
  TEST_OP(smallest_norm, max_denorm, +);
  TEST_OP(smallest_norm, smallest_denorm, +);

  TEST_OP(max_denorm, 2.0f, *);
  TEST_OP(max_denorm, 0.25f, *);
  TEST_OP(smallest_norm, 2.0f, *);
  TEST_OP(smallest_norm, 0.25f, *);

  TEST_OP(max_denorm, 4.0f, /);
  TEST_OP(max_denorm, 0.5f, /);
  TEST_OP(smallest_norm, 4.0f, /);

  TEST_OP(smallest_norm, 0.5f, /);

  TEST_OP(smallest_denorm, max_denorm, <);
  TEST_OP(smallest_norm, max_denorm, <);
  TEST_OP(smallest_norm, smallest_denorm, <);
  TEST_OP(smallest_denorm, max_denorm, >);
  TEST_OP(smallest_norm, max_denorm, >);
  TEST_OP(smallest_norm, smallest_denorm, >);
  TEST_OP(smallest_denorm, max_denorm, ==);
  TEST_OP(smallest_norm, max_denorm, ==);
  TEST_OP(smallest_norm, smallest_denorm, ==);

  TEST_OP(pinf, pinf, !=);
  TEST_OP(pinf, pinf, ==);

  TEST_OPD(d_smallest_denorm, d_max_denorm, +);
  TEST_OPD(d_smallest_norm, d_max_denorm, +);
  TEST_OPD(d_smallest_norm, d_smallest_denorm, +);

  TEST_OPD(d_max_denorm, 2.0, *);
  TEST_OPD(d_max_denorm, 0.25, *);
  TEST_OPD(d_smallest_norm, 2.0, *);
  TEST_OPD(d_smallest_norm, 0.25, *);

  TEST_OPD(d_max_denorm, 4.0, /);
  TEST_OPD(d_max_denorm, 0.5, /);
  TEST_OPD(d_smallest_norm, 4.0, /);

  TEST_OPD(d_smallest_norm, 0.5, /);

  TEST_OPD(d_smallest_denorm, d_max_denorm, <);
  TEST_OPD(d_smallest_norm, d_max_denorm, <);
  TEST_OPD(d_smallest_norm, d_smallest_denorm, <);
  TEST_OPD(d_smallest_denorm, d_max_denorm, >);
  TEST_OPD(d_smallest_norm, d_max_denorm, >);
  TEST_OPD(d_smallest_norm, d_smallest_denorm, >);
  TEST_OPD(d_smallest_denorm, d_max_denorm, ==);
  TEST_OPD(d_smallest_norm, d_max_denorm, ==);
  TEST_OPD(d_smallest_norm, d_smallest_denorm, ==);

  TEST_OPD(pinfd, pinfd, !=);
  TEST_OPD(pinfd, pinfd, ==);
  TEST_OPD(pinfd, pinf, ==);

  TEST_FUNC(smallest_norm, __builtin_sqrtf); 

  TEST_FUNC(smallest_denorm, __builtin_sqrtf);
  TEST_FUNC(max_denorm, __builtin_sqrtf);
  TEST_FUNC(4.0f, __builtin_sqrtf);
  TEST_FUNC(2.0f, __builtin_sqrtf);
  TEST_FUNC(0.5f, __builtin_sqrtf);

  TEST_FTOD(smallest_norm);
  TEST_FTOD(smallest_denorm);
  TEST_FTOD(max_denorm);
  TEST_FTOD(-0.0f);

  TEST_DTOF(d_smallest_norm);
  TEST_DTOF(d_smallest_denorm);
  TEST_DTOF(d_max_denorm);
  TEST_DTOF(-0.0);

  if (failed > 0)
    abort();
  exit(0);
}
