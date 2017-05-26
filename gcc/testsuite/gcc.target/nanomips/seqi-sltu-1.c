/* Check if we expand seq and sne.  */
/* { dg-do compile } */
/* { dg-final { scan-assembler-times "\tseqi\t" 4 } } */
/* { dg-final { scan-assembler-times "\tsltu\t" 4 } } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#define TEST(N, LHS, REL, RHS) \
  int f##N (int a, int b) { return LHS REL RHS; }

TEST (0, a, ==, 1);
TEST (1, a, ==, 222);
TEST (2, a, ==, 511);
TEST (3, a, ==, 1123);

TEST (10, a, ==, b);
TEST (11, a, !=, b);
TEST (12, a, !=, 1);
TEST (13, a, ==, -200);
