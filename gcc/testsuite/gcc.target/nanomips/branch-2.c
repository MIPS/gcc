/* Test BBEQZC and BBNEZC branch instructions. */
/* { dg-options "forbid_cpu=octeon.* -march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void bar (int);
void f1 (int x) { if (x & 4) bar (1); }
void f2 (int x) { if ((x >> 2) & 1) bar (2); }
void f3 (unsigned int x) { if (x & 0x10) bar (3); }
void f4 (unsigned int x) { if ((x >> 4) & 1) bar (4); }
/* { dg-final { scan-assembler-times "\tbbnezc|bbeqzc" 4 } } */
/* { dg-final { scan-assembler-not "\tsrl\t" } } */
/* { dg-final { scan-assembler-not "\tsra\t" } } */
/* { dg-final { scan-assembler-not "\tandi\t" } } */
