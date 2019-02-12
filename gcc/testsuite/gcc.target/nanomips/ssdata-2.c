/* { dg-do compile } */
/* { dg-options "-m32 -mpid -msoft-float -fno-common" } */
/* { dg-additional-options "-mssdata -fdata-sections" } */

/* { dg-final { scan-assembler "\\.section\t\\.ssbss\\.var_i8,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.ssbss\\.var_i16,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_i32,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_i64,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_f32,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_f64,\"aw\",@nobits" } } */

char var_i8;
short var_i16;
int var_i32;
long long var_i64;
float var_f32;
double var_f64;

/* { dg-final { scan-assembler "\\.section\t\\.ssbss\\.var_i8_a,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.ssbss\\.var_i16_a,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_i32_a,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_i64_a,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_f32_a,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_f64_a,\"aw\",@nobits" } } */

char var_i8_a[2];
short var_i16_a[2];
int var_i32_a[2];
long long var_i64_a[2];
float var_f32_a[2];
double var_f64_a[2];

/* { dg-final { scan-assembler "\\.section\t\\.ssbss\\.var_i8_v,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.ssbss\\.var_i16_v,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_i32_v,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_i64_v,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_f32_v,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_f64_v,\"aw\",@nobits" } } */

char __attribute__ ((vector_size (16))) var_i8_v;
short __attribute__ ((vector_size (16))) var_i16_v;
int __attribute__ ((vector_size (16))) var_i32_v;
long long __attribute__ ((vector_size (16))) var_i64_v;
float __attribute__ ((vector_size (16))) var_f32_v;
double __attribute__ ((vector_size (16))) var_f64_v;

/* { dg-final { scan-assembler "\\.section\t\\.ssdata\\.cc_i8,\"aw\",@progbits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.ssdata\\.cc_i16,\"aw\",@progbits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sdata\\.cc_i32,\"aw\",@progbits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sdata\\.cc_i64,\"aw\",@progbits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sdata\\.cc_f32,\"aw\",@progbits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sdata\\.cc_f64,\"aw\",@progbits" } } */

_Complex char  cc_i8 = 1;
_Complex short  cc_i16 = 1;
_Complex int  cc_i32 = 1;
_Complex long long  cc_i64 = 1;
_Complex float  cc_f32 = 1;
_Complex double  cc_f64 = 1;

/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_ptr_void,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_ptr_char,\"aw\",@nobits" } } */

void* var_ptr_void;
char* var_ptr_char;

/* { dg-final { scan-assembler "\\.section\t\\.ssdata\\.var_i8_i,\"aw\",@progbits" } } */

char var_i8_i = 1;

/* { dg-final { scan-assembler "\\.section\t\\.ssdata\\.var_s_misaligned,\"aw\",@progbits" } } */

struct { int val; } var_s_misaligned __attribute__((aligned(1))) = {1};

/* { dg-final { scan-assembler "\\.section\t\\.sdata\\.var_s_not_misaligned,\"aw\",@progbits" } } */

struct { int val; } __attribute__((aligned(1))) var_s_not_misaligned = {1};

/* { dg-final { scan-assembler "\\.section\t\\.ssbss\\.var_s_bitfield,\"aw\",@nobits" } } */

struct {int f: 1; int q: 31; } var_s_bitfield;

/* { dg-final { scan-assembler "\\.section\t\\.ssbss\\.var_s_union,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_s_union1,\"aw\",@nobits" } } */

union { int f; char q; long long s; } var_s_union;
union { int f; float q; long long s; } var_s_union1;

/* { dg-final { scan-assembler "\\.section\t\\.ssbss\\.var_s_pod,\"aw\",@nobits" } } */
/* { dg-final { scan-assembler "\\.section\t\\.sbss\\.var_s_pod1,\"aw\",@nobits" } } */

struct { int f; char q; long long s; } var_s_pod;
struct { int f; float q; long long s; } var_s_pod1;
