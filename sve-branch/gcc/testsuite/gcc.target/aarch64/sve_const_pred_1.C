/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+sve -msve-vector-bits=256" } */

typedef signed char v32qi __attribute__((vector_size(32)));

v32qi
foo (v32qi x, v32qi y)
{
  return (v32qi) { -1, 0, 0, -1, -1, -1, 0, 0,
		   -1, -1, -1, -1, 0, 0, 0, 0,
		   -1, -1, -1, -1, -1, -1, -1, -1,
		   0, 0, 0, 0, 0, 0, 0, 0 } ? x : y;
}

/* { dg-final { scan-assembler {\tldr\tp[0-9]+,} } } */
/* { dg-final { scan-assembler {\t\.byte\t57\n\t\.byte\t15\n\t\.byte\t(255|-1)\n\t\.byte\t0\n} } } */
