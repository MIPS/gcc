/* { dg-options "-march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

/* { dg-final { scan-assembler-times {slt	\$a0,\$a1,\$a0} 1 } } */
/* { dg-final { scan-assembler-times {sltu	\$a0,\$a1,\$a0} 1 } } */

/* { dg-final { scan-assembler-times {slt	\$a0,\$zero,\$a0} 1 } } */
/* { dg-final { scan-assembler-times {sltu	\$a0,\$zero,\$a0} 1 } } */

/* { dg-final { scan-assembler-times {slt	\$a0,\$a0,\$a1} 1 } } */
/* { dg-final { scan-assembler-times {sltu	\$a0,\$a0,\$a1} 1 } } */

/* { dg-final { scan-assembler-times {slt	\$a0,\$a0,23}  1 } } */
/* { dg-final { scan-assembler-times {sltu	\$a0,\$a0,23}  1 } } */

#define TEST(N, LHS, REL, RHS) \
  int s##N (int a, int b) { return LHS REL RHS; } \
  int u##N (unsigned a, unsigned b) { return LHS REL RHS; } \

TEST (1, a, >, b);
TEST (2, a, >=, 1);
TEST (3, a, <, b);
TEST (4, a, <=, 22);
