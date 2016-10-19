/* Verify that target-side header-foo done in glibc for finite-only math
   functions rewriting doesn't cause offloading-side confusion with newlib,
   such as "unresolved symbol __atanh_finite", etc.  */

/* { dg-additional-options "-lm -foffload=-lm -ffast-math" } */

#ifndef __cplusplus /* C */
# include <stdlib.h>
# include <math.h>
# include <float.h>
# include <complex.h>
#else /* C++ */
# include <cstdlib>
# include <cmath>
# include <cfloat>
# include <complex>
#endif

/* Assign "var <= val", and make sure the compiler can't tell.  */
#define LOAD(var, val) \
  do { \
    (var) = (val); \
    asm volatile ("" : : "g" (&(var)) : "memory"); \
  } while (0)

/* Floating point, you know...  Let's keep it simple.  */
#define EPSILON 0.001f
/* These evaluate macro arguments more than once.  */
#define EQUALSf(a, b) \
  (((a) > (b)) ? (((a) - (b)) < (EPSILON)) : (((b) - (a)) < (EPSILON)))
#define EQUALS(a, b) \
  EQUALSf((a), (b))


int main(int argc, char *argv[])
{
#pragma acc parallel
  {
    int i, i_;
    long l, l_;
    long long ll, ll_;
    float f, f_, f__;
    double d, d_, d__;
    const char *s;
#ifndef __cplusplus /* C */
    div_t div_i;
    ldiv_t div_l;
    lldiv_t div_ll;
#else /* C++ */
    std::div_t div_i;
    std::ldiv_t div_l;
    std::lldiv_t div_ll;
#endif

    LOAD(i, -10); i = abs (i); if (i != 10) __builtin_abort();
    LOAD(l, -9); l = abs (l); if (l != 9) __builtin_abort();
    LOAD(ll, -8); ll = abs (ll); if (ll != 8) __builtin_abort();
    LOAD(f, -7); f = fabsf (f); if (!EQUALSf(f, 7)) __builtin_abort();
    LOAD(d, 6); d = fabs (d); if (!EQUALS(d, 6)) __builtin_abort();

    LOAD(i, -10); LOAD(i_, -3); div_i = div (i, i_); if (div_i.quot != 3 && div_i.rem != -1) __builtin_abort();
    LOAD(l, -11); LOAD(l_, -4); div_l = ldiv (l, l_); if (div_l.quot != 2 && div_l.rem != -3) __builtin_abort();
    LOAD(ll, -12); LOAD(ll_, -5); div_ll = lldiv (ll, ll_); if (div_ll.quot != 2 && div_ll.rem != -2) __builtin_abort();

    LOAD(f, -7); LOAD(f_, -2.5); f = fmodf (f, f_); if (!EQUALSf(f, -2)) __builtin_abort();
    LOAD(d, -8); LOAD(d_, -2.6); d = fmod (d, d_); if (!EQUALS(d, -0.2)) __builtin_abort();

    LOAD(f, -8); LOAD(f_, -2.5); f = remainderf (f, f_); if (!EQUALSf(f, -0.5)) __builtin_abort();
    LOAD(d, -7); LOAD(d_, -2.6); d = remainder (d, d_); if (!EQUALS(d, 0.8)) __builtin_abort();

    LOAD(f, -8); LOAD(f_, -2.5); f = remquof (f, f_, &i); if (!EQUALSf(f, -0.5) || i < 0) __builtin_abort();
    LOAD(d, -7); LOAD(d_, -2.6); d = remquo (d, d_, &i); if (!EQUALS(d, 0.8) || i < 0) __builtin_abort();

    LOAD(f, -8); LOAD(f_, -2.3); LOAD(f__, 2.6); f = fmaf (f, f_, f__); if (!EQUALSf(f, 21)) __builtin_abort();
    LOAD(d, -7); LOAD(d_, -2.6); LOAD(d__, 1.8); d = fma (d, d_, d__); if (!EQUALS(d, 20)) __builtin_abort();

    LOAD(f, -3); LOAD(f_, -2.5); f = fmaxf (f, f_); if (!EQUALSf(f, -2.5)) __builtin_abort();
    LOAD(d, -4); LOAD(d_, 2.6); d = fmax (d, d_); if (!EQUALS(d, 2.6)) __builtin_abort();

    LOAD(f, 3); LOAD(f_, -2.5); f = fminf (f, f_); if (!EQUALSf(f, -2.5)) __builtin_abort();
    LOAD(d, -4); LOAD(d_, 2.6); d = fmin (d, d_); if (!EQUALS(d, -4)) __builtin_abort();

    LOAD(f, 3); LOAD(f_, -2.5); f = fdimf (f, f_); if (!EQUALSf(f, 5.5)) __builtin_abort();
    LOAD(d, -4); LOAD(d_, 2.6); d = fdim (d, d_); if (!EQUALS(d, 0)) __builtin_abort();

    LOAD(f, 3.3); f = expf (f); if (!EQUALSf(f, 27.1126)) __builtin_abort();
    LOAD(d, -0.24); d = exp (d); if (!EQUALS(d, 0.7866)) __builtin_abort();

    LOAD(f, 3.3); f = exp2f (f); if (!EQUALSf(f, 9.8492)) __builtin_abort();
    LOAD(d, -0.24); d = exp2 (d); if (!EQUALS(d, 0.8467)) __builtin_abort();

    LOAD(f, 3.3); f = expm1f (f); if (!EQUALSf(f, 26.1126)) __builtin_abort();
    LOAD(d, -0.24); d = expm1 (d); if (!EQUALS(d, -0.2134)) __builtin_abort();

    LOAD(f, 10.3); f = logf (f); if (!EQUALSf(f, 2.3321)) __builtin_abort();
    LOAD(d, 0.55); d = log (d); if (!EQUALS(d, -0.5978)) __builtin_abort();

    LOAD(f, 1); f = log2f (f); if (!EQUALSf(f, 0)) __builtin_abort();
    LOAD(d, 32768); d = log2 (d); if (!EQUALS(d, 15)) __builtin_abort();

    LOAD(f, 100); f = log10f (f); if (!EQUALSf(f, 2)) __builtin_abort();
    LOAD(d, 0.3162); d = log10 (d); if (!EQUALS(d, -0.5000)) __builtin_abort();

    LOAD(f, 4); f = log1pf (f); if (!EQUALSf(f, 1.6094)) __builtin_abort();
    LOAD(d, -0); d = log1p (d); if (!EQUALS(d, 0)) __builtin_abort();

    LOAD(f, 4); i = ilogbf (f); if (i != 2) __builtin_abort();
    LOAD(d, 987.55); i = ilogb (d); if (i != 9) __builtin_abort();

    LOAD(f, 987.55); f = logbf (f); if (!EQUALSf(f, 9)) __builtin_abort();
    LOAD(d, 4); d = logb (d); if (!EQUALS(d, 2)) __builtin_abort();

    LOAD(f, 987.55); f = sqrtf (f); if (!EQUALSf(f, 31.4253)) __builtin_abort();
    LOAD(d, 4); d = sqrt (d); if (!EQUALS(d, 2)) __builtin_abort();

    LOAD(f, 31034.0387); f = cbrtf (f); if (!EQUALSf(f, 31.4253)) __builtin_abort();
    LOAD(d, 8); d = cbrt (d); if (!EQUALS(d, 2)) __builtin_abort();

    LOAD(f, -8); LOAD(f_, -2.5); f = hypotf (f, f_); if (!EQUALSf(f, 8.3815)) __builtin_abort();
    LOAD(d, -7); LOAD(d_, -2.6); d = hypot (d, d_); if (!EQUALS(d, 7.4673)) __builtin_abort();

    LOAD(f, 8); LOAD(f_, -2.5); f = powf (f, f_); if (!EQUALSf(f, 0.0055)) __builtin_abort();
    LOAD(d, 7); LOAD(d_, -2.6); d = pow (d, d_); if (!EQUALS(d, 0.0063)) __builtin_abort();

    LOAD(f, 8); f = sinf (f); if (!EQUALSf(f, 0.9894)) __builtin_abort();
    LOAD(d, 7); d = sin (d); if (!EQUALS(d, 0.6570)) __builtin_abort();

    LOAD(f, 8); f = cosf (f); if (!EQUALSf(f, -0.1455)) __builtin_abort();
    LOAD(d, 7); d = cos (d); if (!EQUALS(d, 0.7539)) __builtin_abort();

    LOAD(f, 8); f = tanf (f); if (!EQUALSf(f, -6.7997)) __builtin_abort();
    LOAD(d, 7); d = tan (d); if (!EQUALS(d, 0.8714)) __builtin_abort();

    LOAD(f, 0.8); f = asinf (f); if (!EQUALSf(f, 0.9273)) __builtin_abort();
    LOAD(d, 0.7); d = asin (d); if (!EQUALS(d, 0.7754)) __builtin_abort();

    LOAD(f, 0.8); f = acosf (f); if (!EQUALSf(f, 0.6435)) __builtin_abort();
    LOAD(d, 0.7); d = acos (d); if (!EQUALS(d, 0.7954)) __builtin_abort();

    LOAD(f, 0.8); f = atanf (f); if (!EQUALSf(f, 0.6747)) __builtin_abort();
    LOAD(d, 0.7); d = atan (d); if (!EQUALS(d, 0.6107)) __builtin_abort();

    LOAD(f, 0.8); LOAD(f_, -0.7); f = atan2f (f, f_); if (!EQUALSf(f, 2.2896)) __builtin_abort();
    LOAD(d, -0.7); LOAD(d_, 0.8); d = atan2 (d, d_); if (!EQUALS(d, -0.7188)) __builtin_abort();

    LOAD(f, 0.8); f = sinhf (f); if (!EQUALSf(f, 0.8881)) __builtin_abort();
    LOAD(d, 0.7); d = sinh (d); if (!EQUALS(d, 0.7585)) __builtin_abort();

    LOAD(f, 0.8); f = coshf (f); if (!EQUALSf(f, 1.3374)) __builtin_abort();
    LOAD(d, 0.7); d = cosh (d); if (!EQUALS(d, 1.2551)) __builtin_abort();

    LOAD(f, 0.8); f = tanhf (f); if (!EQUALSf(f, 0.6640)) __builtin_abort();
    LOAD(d, 0.7); d = tanh (d); if (!EQUALS(d, 0.6044)) __builtin_abort();

    LOAD(f, 0.8); f = asinhf (f); if (!EQUALSf(f, 0.7327)) __builtin_abort();
    LOAD(d, 0.7); d = asinh (d); if (!EQUALS(d, 0.6527)) __builtin_abort();

    LOAD(f, 1.8); f = acoshf (f); if (!EQUALSf(f, 1.1929)) __builtin_abort();
    LOAD(d, 1.7); d = acosh (d); if (!EQUALS(d, 1.1232)) __builtin_abort();

    LOAD(f, 0.8); f = atanhf (f); if (!EQUALSf(f, 1.0986)) __builtin_abort();
    LOAD(d, 0.7); d = atanh (d); if (!EQUALS(d, 0.8673)) __builtin_abort();

    LOAD(f, 0.8); f = erff (f); if (!EQUALSf(f, 0.7421)) __builtin_abort();
    LOAD(d, 0.7); d = erf (d); if (!EQUALS(d, 0.6778)) __builtin_abort();

    LOAD(f, 0.8); f = erfcf (f); if (!EQUALSf(f, 1 - 0.7421)) __builtin_abort();
    LOAD(d, 0.7); d = erfc (d); if (!EQUALS(d, 1 - 0.6778)) __builtin_abort();

#if 0
    /* TODO: incompatible inline function.  */
    LOAD(f, 0.8); f = lgammaf (f); if (!EQUALSf(f, TODO)) __builtin_abort();
    LOAD(d, 0.7); d = lgamma (d); if (!EQUALS(d, TODO)) __builtin_abort();
#endif

#if 0
    /* TODO: incompatible inline function.  */
    LOAD(f, 0.8); f = tgammaf (f); if (!EQUALSf(f, TODO)) __builtin_abort();
    LOAD(d, 0.7); d = tgamma (d); if (!EQUALS(d, TODO)) __builtin_abort();
#endif

    LOAD(f, -0.8); f = ceilf (f); if (!EQUALSf(f, -0)) __builtin_abort();
    LOAD(d, 0.7); d = ceil (d); if (!EQUALS(d, 1)) __builtin_abort();

    LOAD(f, -0.8); f = floorf (f); if (!EQUALSf(f, -1)) __builtin_abort();
    LOAD(d, 0.7); d = floor (d); if (!EQUALS(d, 0)) __builtin_abort();

    LOAD(f, -0.8); f = truncf (f); if (!EQUALSf(f, -0)) __builtin_abort();
    LOAD(d, 0.7); d = trunc (d); if (!EQUALS(d, 0)) __builtin_abort();

    LOAD(f, -0.8); f = roundf (f); if (!EQUALSf(f, -1)) __builtin_abort();
    LOAD(d, 0.7); d = round (d); if (!EQUALS(d, 1)) __builtin_abort();
    LOAD(f, -0.8); l = lroundf (f); if (l != -1) __builtin_abort();
    LOAD(d, 0.7); l = lround (d); if (l != 1) __builtin_abort();
    LOAD(f, -0.8); ll = llroundf (f); if (ll != -1) __builtin_abort();
    LOAD(d, 0.7); ll = llround (d); if (ll != 1) __builtin_abort();

#if 0
    /* TODO: current rounding mode.  */

    LOAD(f, -0.8); f = nearbyintf (f); if (!EQUALSf(f, TODO)) __builtin_abort();
    LOAD(d, 0.7); d = nearbyint (d); if (!EQUALS(d, TODO)) __builtin_abort();

    LOAD(f, -0.8); f = rintf (f); if (!EQUALSf(f, TODO)) __builtin_abort();
    LOAD(d, 0.7); d = rint (d); if (!EQUALS(d, TODO)) __builtin_abort();
    LOAD(f, -0.8); l = lrintf (f); if (l != TODO) __builtin_abort();
    LOAD(d, 0.7); l = lrint (d); if (l != TODO) __builtin_abort();
    LOAD(f, -0.8); ll = llrintf (f); if (ll != TODO) __builtin_abort();
    LOAD(d, 0.7); ll = llrint (d); if (ll != TODO) __builtin_abort();
#endif

    LOAD(f, -8.88); f = frexpf (f, &i); if (!EQUALSf(f, -0.5550) || i != 4) __builtin_abort();
    LOAD(d, -7.77); d = frexp (d, &i); if (!EQUALS(d, -0.9712) || i != 3) __builtin_abort();

    LOAD(f, -8.88); LOAD(i, 5); f = ldexpf (f, i); if (!EQUALSf(f, -284.16)) __builtin_abort();
    LOAD(d, -7.77); LOAD(i, 6); d = ldexp (d, i); if (!EQUALS(d, -497.28)) __builtin_abort();

    LOAD(f, -8.88); f = modff (f, &f_); if (!EQUALSf(f, -0.88) || !EQUALSf(f_, -8)) __builtin_abort();
    LOAD(d, -7.77); d = modf (d, &d_); if (!EQUALS(d, -0.77) || !EQUALS(d_, -7)) __builtin_abort();

#if FLT_RADIX != 2
# error
#endif
    LOAD(f, -8.88); LOAD(i, 5); f = scalbnf (f, i); if (!EQUALSf(f, -284.16)) __builtin_abort();
    LOAD(d, -7.77); LOAD(i, 6); d = scalbn (d, i); if (!EQUALS(d, -497.28)) __builtin_abort();
    LOAD(f, -8.88); LOAD(l, 5); f = scalblnf (f, l); if (!EQUALSf(f, -284.16)) __builtin_abort();
    LOAD(d, -7.77); LOAD(l, 6); d = scalbln (d, l); if (!EQUALS(d, -497.28)) __builtin_abort();
  }

  return 0;
}
