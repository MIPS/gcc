/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+sve -msve-vector-bits=256" } */

typedef int v8si __attribute__((vector_size(32)));

v8si
foo (v8si x, v8si y)
{
  return (v8si) { -1, 0, 0, -1, -1, -1, 0, 0 } ? x : y;
}

/* { dg-final { scan-assembler {\tldr\tp[0-9]+,} } } */
/* { dg-final { scan-assembler {\t\.byte\t1\n\t\.byte\t16\n\t\.byte\t17\n\t\.byte\t0\n} } } */
