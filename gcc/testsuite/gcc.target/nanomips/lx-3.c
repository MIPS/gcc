/* { dg-do compile } */
/* { dg-options "-march=32r6 -mgp32" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#define TEST(N, T) \
  T f##N (T *p, int i) { return p[i]; } \
  unsigned T g##N (unsigned T *p, int i) { return p[i]; }

TEST (1, char)
/* { dg-final { scan-assembler-times "\tlbu?x\t" 2 } } */
TEST (2, short)
/* { dg-final { scan-assembler-times "\tlhxs\t" 1 } } */
/* { dg-final { scan-assembler-times "\tlhuxs\t" 1 } } */
TEST (3, int)
/* { dg-final { scan-assembler-times "\tlwxs\t" 2 } } */
