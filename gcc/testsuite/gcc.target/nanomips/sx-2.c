/* { dg-options "-march=32r6" } */
/* { dg-do compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#define TEST(N, T) \
  void f##N (T *p, int i, T *y) { y[i] = p[i]; } \
  void g##N (unsigned T *p, int i, T *y) { y[i] = p[i]; }

TEST (1, char)
/* { dg-final { scan-assembler-times "\tsbx\t" 2 } } */
TEST (2, short)
/* { dg-final { scan-assembler-times "\tshx\t" 2 } } */
TEST (3, int)
/* { dg-final { scan-assembler-times "\tswxs\t" 2 } } */
