/* { dg-options "-march=32r6" } */
/* { dg-do compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-O1" } { "" } } */

#define TESTL(N, T) \
  T f##N (T *p, int i) { return p[i]; } \
  unsigned T g##N (unsigned T *p, int i) { return p[i]; }

#define TESTS(N, T) \
  void f##N (T p, int i, T *y) { y[i] = p; } \
  void g##N (unsigned T p, int i, T *y) { y[i] = p; }

TESTL (1, long long)
/* { dg-final { scan-assembler-times "\tlwm\t" 2 } } */

TESTS (4, long long)
/* { dg-final { scan-assembler-times "\tswm\t" 2 } } */
