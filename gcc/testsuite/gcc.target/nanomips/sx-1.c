/* { dg-options "-march=32r6" } */
/* { dg-do compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-Os" } { "" } } */

#define TEST(N, R, T) \
 void fll##N (T j, signed R *b, long long i, T *y) { y[i] = j + b[i]; } \
 void gll##N (T j, unsigned R *b, long long i, T *y) { y[i] = j + b[i]; } \
 void fi##N (T j, signed R *b, int i, T *y) { y[i] = j + b[i]; } \
 void gi##N (T j, unsigned R *b, int i, T *y) { y[i] = j + b[i]; } \

TEST (1, char, char)
TEST (2, char, short)
/* { dg-final { scan-assembler-times "\tsbx\t" 4 } } */
/* { dg-final { scan-assembler-times "\tshxs\t" 4 } } */
TEST (3, short, short)
TEST (4, short, int)
/* { dg-final { scan-assembler-times "\tshx\t" 4 } } */
/* { dg-final { scan-assembler-times "\tswxs\t" 4 } } */
TEST (5, int, long long)
/* { dg-final { scan-assembler-times "\tsw\t" 4 } } */
/* { dg-final { scan-assembler-times "\tswx\t" 4 } } */
