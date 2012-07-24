/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-march=octeon2 -O" } */
/* { dg-final { scan-assembler-times "\tlbx\t" 2 } } */
/* { dg-final { scan-assembler-times "\tlbux\t" 2 } } */
/* { dg-final { scan-assembler-times "\tlhx\t" 2 } } */
/* { dg-final { scan-assembler-times "\tlhux\t" 2 } } */
/* { dg-final { scan-assembler-times "\tlwx\t" 1 } } */
/* { dg-final { scan-assembler-times "\tlwux\t" 1 } } */

#define TEST(N, R, T) \
  T f##N (T j, R *b, long long i) { return j + b[i]; } \
  T g##N (T j, unsigned R *b, long long i) { return j + b[i]; }

TEST (1, char, int)
TEST (2, char, long long)
TEST (3, short, int)
TEST (4, short, long long)
TEST (5, int, long long)
