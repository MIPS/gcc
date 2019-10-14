/* { dg-options "-march=32r6" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-O2" "-O3"} { "" } } */

void
foo(int, ...);

void
bar (char a0, short a1)
{
    foo (2, 2, 2, 1, 14, 1, &a0, 2);
}

/* { dg-final { scan-assembler-times "movep\t" 1} } */
