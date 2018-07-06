/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target arm_v8_2a_fp16_scalar_ok } */
/* { dg-add-options arm_v8_2a_fp16_scalar } */

__fp16 f4 ()
{
  __fp16 a = 0.1;

  __fp16 z = a * a;
  return z;
}

/* { dg-final { scan-assembler-times {fmov\th[0-9]+, w[0-9]+} 1 } } */
