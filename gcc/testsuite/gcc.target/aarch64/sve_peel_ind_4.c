/* { dg-do compile } */
/* { dg-options "-Ofast -march=armv8-a+sve -msve-vector-bits=256" } */

#define START 1
#define END 505

void __attribute__((weak))
foo (double *x)
{
  double v = 10.0;
  for (unsigned int i = START; i < END; ++i)
    {
      x[i] = v;
      v += 5.0;
    }
}

/* We should operate on aligned vectors.  */
/* { dg-final { scan-assembler {\tubfx\t} } } */
