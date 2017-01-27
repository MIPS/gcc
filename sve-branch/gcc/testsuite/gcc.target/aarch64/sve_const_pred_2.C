/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+sve -msve-vector-bits=256" } */

typedef short v16hi __attribute__((vector_size(32)));

v16hi
foo (v16hi x, v16hi y)
{
  return (v16hi) { -1, 0, 0, -1, -1, -1, 0, 0,
		   -1, -1, -1, -1, 0, 0, 0, 0 } ? x : y;
}

/* { dg-final { scan-assembler {\tldr\tp[0-9]+,} } } */
/* { dg-final { scan-assembler {\t\.byte\t65\n\t\.byte\t5\n\t\.byte\t85\n\t\.byte\t0\n} } } */
