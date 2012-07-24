/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-march=octeon2 -O" } */
/* { dg-final { scan-assembler-times "\tlbu?x\t" 2 } } */
/* { dg-final { scan-assembler-times "\tlhu?x\t" 2 } } */
/* { dg-final { scan-assembler-times "\tlwx\t" 2 } } */
/* { dg-final { scan-assembler-times "\tldx\t" 2 } } */

#define TEST(N, T) \
  T f##N (T *p, int i) { return p[i]; } \
  unsigned T g##N (unsigned T *p, int i) { return p[i]; }

TEST (1, char)
TEST (2, short)
TEST (3, int)
TEST (4, long long)
