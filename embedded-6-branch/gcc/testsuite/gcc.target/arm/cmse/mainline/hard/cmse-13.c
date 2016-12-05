/* { dg-do compile } */
/* { dg-require-effective-target arm_arch_v8m_main_ok } */
/* { dg-add-options arm_arch_v8m_main } */
/* { dg-skip-if "Do not combine float-abi= hard | soft | softfp" {*-*-*} {"-mfloat-abi=soft" -mfloat-abi=softfp } {""} } */
/* { dg-skip-if "Skip these if testing single precision" {*-*-*} {"-mfpu=*-sp-*"} {""} } */
/* { dg-options "-mcmse -mfloat-abi=hard -mfpu=fpv5-d16" }  */


int __attribute__ ((cmse_nonsecure_call)) (*bar) (float, double);

int
foo (int a)
{
  return bar (3.0f, 2.0) + a + 1;
}

/* Checks for saving and clearing prior to function call.  */
/* { dg-final { scan-assembler "lsrs\tr4, r4, #1" } } */
/* { dg-final { scan-assembler "lsls\tr4, r4, #1" } } */
/* { dg-final { scan-assembler "mov\tr0, r4" } } */
/* { dg-final { scan-assembler "mov\tr1, r4" } } */
/* { dg-final { scan-assembler "mov\tr2, r4" } } */
/* { dg-final { scan-assembler "mov\tr3, r4" } } */
/* { dg-final { scan-assembler-not "vmov\.f32\ts0, #1\.0" } } */
/* { dg-final { scan-assembler "vmov\.f32\ts1, #1\.0" } } */
/* { dg-final { scan-assembler-not "vmov\.f64\td1, #1\.0" } } */
/* { dg-final { scan-assembler "vmov\.f64\td2, #1\.0" } } */
/* { dg-final { scan-assembler "vmov\.f64\td3, #1\.0" } } */
/* { dg-final { scan-assembler "vmov\.f64\td4, #1\.0" } } */
/* { dg-final { scan-assembler "vmov\.f64\td5, #1\.0" } } */
/* { dg-final { scan-assembler "vmov\.f64\td6, #1\.0" } } */
/* { dg-final { scan-assembler "vmov\.f64\td7, #1\.0" } } */

/* Now we check that we use the correct intrinsic to call.  */
/* { dg-final { scan-assembler "bl\t__gnu_cmse_nonsecure_call" } } */
