/* { dg-options "-march=32r6" } */
/* { dg-do compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#define TEST(N, R, T) \
 T fll##N (T j, signed R *b, long long i) { return j + b[i]; } \
 T gll##N (T j, unsigned R *b, long long i) { return j + b[i]; } \
 T fi##N (T j, signed R *b, int i) { return j + b[i]; } \
 T gi##N (T j, unsigned R *b, int i) { return j + b[i]; } \

TEST (1, char, int)
TEST (2, char, long long)
/* { dg-final { scan-assembler-times "\tlbx\t" 4 } } */
/* { dg-final { scan-assembler-times "\tlbux\t" 4 } } */
TEST (3, short, int)
TEST (4, short, long long)
/* { dg-final { scan-assembler-times "\tlhxs\t" 4 } } */
/* { dg-final { scan-assembler-times "\tlhuxs\t" 4 } } */
TEST (5, int, long long)
/* { dg-final { scan-assembler-times "\tlwxs\t" 4 } } */
