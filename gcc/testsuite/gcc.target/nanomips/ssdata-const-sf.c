/* { dg-do compile } */
/* { dg-options "-m32 -mpid -msoft-float" } */
/* { dg-additional-options "-mssdata" } */

/* { dg-final { scan-assembler-not "\\.section\t\\.ssdata,\"aw\",@progbits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sdata,\"aw\",@progbits" } } */

float f32() { return 1.0f; }
double f64() { return 1.0; }