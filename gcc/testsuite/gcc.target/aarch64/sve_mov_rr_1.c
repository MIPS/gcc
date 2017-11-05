/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256 --save-temps" } */

void sve_copy_rr (void)
{
  typedef int v8si __attribute__((vector_size(32)));
  register v8si x asm ("z1");
  register v8si y asm ("z2");
  asm volatile ("#foo" : "=w" (x));
  y = x;
  asm volatile ("#foo" :: "w" (y));
}

/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.d, z[0-9]+\.d\n} 1 } } */
