/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+sve -msve-vector-bits=256" } */

typedef long long v4di __attribute__((vector_size(32)));

v4di
foo (v4di x, v4di y)
{
  return (v4di) { -1, 0, 0, -1 } ? x : y;
}

/* { dg-final { scan-assembler {\tldr\tp[0-9]+,} } } */
/* { dg-final { scan-assembler {\t\.byte\t1\n\t\.byte\t0\n\t\.byte\t0\n\t\.byte\t1\n} } } */
