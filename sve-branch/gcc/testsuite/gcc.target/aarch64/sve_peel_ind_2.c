/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256" } */

#define N 512
#define START 7
#define END 22

int x[N] __attribute__((aligned(32)));

void __attribute__((weak))
foo (void)
{
  for (unsigned int i = START; i < END; ++i)
    x[i] = i;
}

/* We should operate on aligned vectors.  */
/* { dg-final { scan-assembler {\tadrp\tx[0-9]+, x\n} } } */
/* We should unroll the loop three times.  */
/* { dg-final { scan-assembler-times "\tst1w\t" 3 } } */
