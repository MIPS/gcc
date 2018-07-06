/* { dg-do compile } */

void f1(__SVBool_t) {}
void f2(__SVInt8_t) {}
void f3(__SVInt16_t) {}
void f4(__SVInt32_t) {}
void f5(__SVInt64_t) {}
void f6(__SVUInt8_t) {}
void f7(__SVUInt16_t) {}
void f8(__SVUInt32_t) {}
void f9(__SVUInt64_t) {}
void f10(__SVFloat16_t) {}
void f11(__SVFloat32_t) {}
void f12(__SVFloat64_t) {}

/* { dg-final { scan-assembler "_Z2f110__SVBool_t:" } } */
/* { dg-final { scan-assembler "_Z2f210__SVInt8_t:" } } */
/* { dg-final { scan-assembler "_Z2f311__SVInt16_t:" } } */
/* { dg-final { scan-assembler "_Z2f411__SVInt32_t:" } } */
/* { dg-final { scan-assembler "_Z2f511__SVInt64_t:" } } */
/* { dg-final { scan-assembler "_Z2f611__SVUInt8_t:" } } */
/* { dg-final { scan-assembler "_Z2f712__SVUInt16_t:" } } */
/* { dg-final { scan-assembler "_Z2f812__SVUInt32_t:" } } */
/* { dg-final { scan-assembler "_Z2f912__SVUInt64_t:" } } */
/* { dg-final { scan-assembler "_Z3f1013__SVFloat16_t:" } } */
/* { dg-final { scan-assembler "_Z3f1113__SVFloat32_t:" } } */
/* { dg-final { scan-assembler "_Z3f1213__SVFloat64_t:" } } */
