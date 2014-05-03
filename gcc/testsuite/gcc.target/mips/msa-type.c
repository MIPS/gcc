/* Test MIPS MSA ASE instructions */
/* { dg-do compile } */
/* { dg-options "-mfp64 -mhard-float -mmsa" } */
/* { dg-skip-if "madd and msub need combine" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-times "\taddv.b\t" 2 } } */
/* { dg-final { scan-assembler-times "\taddv.h\t" 2 } } */
/* { dg-final { scan-assembler-times "\taddv.w\t" 2 } } */
/* { dg-final { scan-assembler-times "\taddv.d\t" 2 } } */
/* { dg-final { scan-assembler-times "\tfadd.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tfadd.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\tsubv.b\t" 4 } } */
/* { dg-final { scan-assembler-times "\tsubv.h\t" 4 } } */
/* { dg-final { scan-assembler-times "\tsubv.w\t" 4 } } */
/* { dg-final { scan-assembler-times "\tsubv.d\t" 4 } } */
/* { dg-final { scan-assembler-times "\tfsub.w\t" 2 } } */
/* { dg-final { scan-assembler-times "\tfsub.d\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmulv.b\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmulv.h\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmulv.w\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmulv.d\t" 2 } } */
/* { dg-final { scan-assembler-times "\tfmul.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tfmul.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\tdiv_s.b\t" 1 } } */
/* { dg-final { scan-assembler-times "\tdiv_s.h\t" 1 } } */
/* { dg-final { scan-assembler-times "\tdiv_s.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tdiv_s.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\tfdiv.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tfdiv.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\tdiv_u.b\t" 1 } } */
/* { dg-final { scan-assembler-times "\tdiv_u.h\t" 1 } } */
/* { dg-final { scan-assembler-times "\tdiv_u.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tdiv_u.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\tmod_s.b\t" 1 } } */
/* { dg-final { scan-assembler-times "\tmod_s.h\t" 1 } } */
/* { dg-final { scan-assembler-times "\tmod_s.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tmod_s.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\tmod_u.b\t" 1 } } */
/* { dg-final { scan-assembler-times "\tmod_u.h\t" 1 } } */
/* { dg-final { scan-assembler-times "\tmod_u.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tmod_u.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\txor.v\t" 8 } } */
/* { dg-final { scan-assembler-times "\tor.v\t" 8 } } */
/* { dg-final { scan-assembler-times "\tand.v\t" 8 } } */
/* { dg-final { scan-assembler-times "\tsra.b\t" 1 } } */
/* { dg-final { scan-assembler-times "\tsra.h\t" 1 } } */
/* { dg-final { scan-assembler-times "\tsra.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tsra.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\tsrl.b\t" 1 } } */
/* { dg-final { scan-assembler-times "\tsrl.h\t" 1 } } */
/* { dg-final { scan-assembler-times "\tsrl.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tsrl.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\tsll.b\t" 2 } } */
/* { dg-final { scan-assembler-times "\tsll.h\t" 2 } } */
/* { dg-final { scan-assembler-times "\tsll.w\t" 2 } } */
/* { dg-final { scan-assembler-times "\tsll.d\t" 2 } } */
/* { dg-final { scan-assembler-times "\tldi.b\t" 2 } } */
/* { dg-final { scan-assembler-times "\tldi.h\t" 4 } } */
/* { dg-final { scan-assembler-times "\tldi.w\t" 5 } } */
/* { dg-final { scan-assembler-times "\tldi.d\t" 5 } } */
/* { dg-final { scan-assembler-times "\tnor.v\t" 6 } } */
/* { dg-final { scan-assembler-times "\tnori.b\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmaddv.b\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmaddv.h\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmaddv.w\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmaddv.d\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmove.v\t" 40 } } */
/* { dg-final { scan-assembler-times "\tfmadd.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tfmadd.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\tmsubv.b\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmsubv.h\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmsubv.w\t" 2 } } */
/* { dg-final { scan-assembler-times "\tmsubv.d\t" 2 } } */
/* { dg-final { scan-assembler-times "\tfmsub.w\t" 1 } } */
/* { dg-final { scan-assembler-times "\tfmsub.d\t" 1 } } */
/* { dg-final { scan-assembler-times "\tvshf.b\t" 4 } } */
/* { dg-final { scan-assembler-times "\tvshf.h\t" 4 } } */
/* { dg-final { scan-assembler-times "\tvshf.w\t" 6 } } */
/* { dg-final { scan-assembler-times "\tvshf.d\t" 6 } } */

typedef signed char v16i8 __attribute__ ((vector_size(16)));
typedef short v8i16 __attribute__ ((vector_size(16)));
typedef int v4i32 __attribute__ ((vector_size(16)));
typedef long long v2i64 __attribute__ ((vector_size(16)));
typedef unsigned char v16u8 __attribute__ ((vector_size(16)));
typedef unsigned short v8u16 __attribute__ ((vector_size(16)));
typedef unsigned int v4u32 __attribute__ ((vector_size(16)));
typedef unsigned long long v2u64 __attribute__ ((vector_size(16)));
typedef float v4f32 __attribute__ ((vector_size(16)));
typedef double v2f64 __attribute__ ((vector_size(16)));

/*
typedef signed char v8i8 __attribute__ ((vector_size(8)));
typedef short v4i16 __attribute__ ((vector_size(8)));
typedef int v2i32 __attribute__ ((vector_size(8)));
typedef float v2f32 __attribute__ ((vector_size(8)));

typedef signed char v4i8 __attribute__ ((vector_size(4)));
typedef short v2i16 __attribute__ ((vector_size(4)));
*/

typedef long long i64;
typedef int i32;
typedef short i16;
typedef signed char i8;
typedef double f64;
typedef float f32;

#define DECLARE(TYPE) TYPE TYPE ## _0, TYPE ## _1, TYPE ## _2;
#define RETURN(TYPE) NOMIPS16 TYPE test0_ ## TYPE () { return TYPE ## _0; }
#define ASSIGN(TYPE) NOMIPS16 void test1_ ## TYPE (TYPE i) { TYPE ## _1 = i; }
#define ADD(TYPE) NOMIPS16 TYPE test2_ ## TYPE (TYPE i, TYPE j) { return i + j; }
#define SUB(TYPE) NOMIPS16 TYPE test3_ ## TYPE (TYPE i, TYPE j) { return i - j; }
#define MUL(TYPE) NOMIPS16 TYPE test4_ ## TYPE (TYPE i, TYPE j) { return i * j; }
#define DIV(TYPE) TYPE test5_ ## TYPE (TYPE i, TYPE j) { return i / j; }
#define MOD(TYPE) TYPE test6_ ## TYPE (TYPE i, TYPE j) { return i % j; }
#define MINUS(TYPE) TYPE test7_ ## TYPE (TYPE i) { return -i; }
#define XOR(TYPE) TYPE test8_ ## TYPE (TYPE i, TYPE j) { return i ^ j; }
#define OR(TYPE) TYPE test9_ ## TYPE (TYPE i, TYPE j) { return i | j; }
#define AND(TYPE) TYPE test10_ ## TYPE (TYPE i, TYPE j) { return i & j; }
#define BIT_COMPLEMENT(TYPE) TYPE test11_ ## TYPE (TYPE i) { return ~i; }
#define SHIFT_RIGHT(TYPE) TYPE test12_ ## TYPE (TYPE i, TYPE j) { return i >> j; }
#define SHIFT_LEFT(TYPE) TYPE test13_ ## TYPE (TYPE i, TYPE j) { return i << j; }
#define EQ(TYPE) TYPE test14_ ## TYPE (TYPE i, TYPE j) { return i == j; }
#define NEQ(TYPE) TYPE test15_ ## TYPE (TYPE i, TYPE j) { return i != j; }
#define LT(TYPE) TYPE test16_ ## TYPE (TYPE i, TYPE j) { return i < j; }
#define LEQ(TYPE) TYPE test17_ ## TYPE (TYPE i, TYPE j) { return i <= j; }
#define GT(TYPE) TYPE test18_ ## TYPE (TYPE i, TYPE j) { return i > j; }
#define GEQ(TYPE) TYPE test19_ ## TYPE (TYPE i, TYPE j) { return i >= j; }

#define ADD_I(TYPE) TYPE test20_ ## TYPE (TYPE i) { return i + 37; }
#define SUB_I(TYPE) TYPE test21_ ## TYPE (TYPE i) { return i - 37; }
#define MUL_I(TYPE) TYPE test22_ ## TYPE (TYPE i) { return i * 37; }
#define DIV_I(TYPE) TYPE test23_ ## TYPE (TYPE i) { return i / 37; }
#define MOD_I(TYPE) TYPE test24_ ## TYPE (TYPE i) { return i % 37; }
#define XOR_I(TYPE) TYPE test25_ ## TYPE (TYPE i) { return i ^ 37; }
#define OR_I(TYPE) TYPE test26_ ## TYPE (TYPE i) { return i | 37; }
#define AND_I(TYPE) TYPE test27_ ## TYPE (TYPE i) { return i & 37; }
#define SHIFT_RIGHT_I(TYPE) TYPE test28_ ## TYPE (TYPE i) { return i >> 3; }
#define SHIFT_LEFT_I(TYPE) TYPE test29_ ## TYPE (TYPE i) { return i << 3; }

#define ADD_F(TYPE) TYPE test30_ ## TYPE (TYPE i) { return i + 37.0; }
#define SUB_F(TYPE) TYPE test31_ ## TYPE (TYPE i) { return i - 37.0; }
#define MUL_F(TYPE) TYPE test32_ ## TYPE (TYPE i) { return i * 37.0; }
#define DIV_F(TYPE) TYPE test33_ ## TYPE (TYPE i) { return i / 37.0; }

#define SHUFFLE1(TYPE) TYPE test34_ ## TYPE (TYPE i, TYPE mask) { return __builtin_shuffle (i, mask); }
#define SHUFFLE2(TYPE) TYPE test35_ ## TYPE (TYPE i, TYPE j, TYPE mask) { return __builtin_shuffle (i, j, mask); }

#define REAL_SHUFFLE1(TYPE, MASK_TYPE) TYPE test36_ ## TYPE (TYPE i, MASK_TYPE mask) { return __builtin_shuffle (i, mask); }
#define REAL_SHUFFLE2(TYPE, MASK_TYPE) TYPE test37_ ## TYPE (TYPE i, TYPE j, MASK_TYPE mask) { return __builtin_shuffle (i, j, mask); }

#define MADD(TYPE) TYPE test38_ ## TYPE (TYPE i, TYPE j, TYPE k) { return i * j + k; }
#define MSUB(TYPE) TYPE test39_ ## TYPE (TYPE i, TYPE j, TYPE k) { return k - i * j; }

#define ITERATE_FOR_ALL_INT_VECTOR_TYPES(FUNC) \
  FUNC (v16i8) \
  FUNC (v8i16) \
  FUNC (v4i32) \
  FUNC (v2i64) \
  FUNC (v16u8) \
  FUNC (v8u16) \
  FUNC (v4u32) \
  FUNC (v2u64)

/*
  FUNC (v8i8) \
  FUNC (v4i16) \
  FUNC (v2i32) \
  FUNC (v4i8) \
  FUNC (v2i16)
*/

#define ITERATE_FOR_ALL_INT_SCALAR_TYPES(FUNC) \
  FUNC (i64) \
  FUNC (i32) \
  FUNC (i16) \
  FUNC (i8)

#define ITERATE_FOR_ALL_INT_TYPES(FUNC) \
  ITERATE_FOR_ALL_INT_VECTOR_TYPES(FUNC) \

/*
  ITERATE_FOR_ALL_INT_SCALAR_TYPES(FUNC)
*/

#define ITERATE_FOR_ALL_REAL_VECTOR_TYPES(FUNC) \
  FUNC (v4f32) \
  FUNC (v2f64) \

/*
  FUNC (v2f32)
*/

#define ITERATE_FOR_ALL_REAL_SCALAR_TYPES(FUNC) \
  FUNC (f64) \
  FUNC (f32)

#define ITERATE_FOR_ALL_REAL_TYPES(FUNC) \
  ITERATE_FOR_ALL_REAL_VECTOR_TYPES(FUNC) \

/*
  ITERATE_FOR_ALL_REAL_SCALAR_TYPES(FUNC)
*/

#define ITERATE_FOR_ALL_TYPES(FUNC) \
  ITERATE_FOR_ALL_INT_TYPES(FUNC) \
  ITERATE_FOR_ALL_REAL_TYPES(FUNC)

ITERATE_FOR_ALL_TYPES (ADD)
ITERATE_FOR_ALL_TYPES (SUB)
ITERATE_FOR_ALL_TYPES (MUL)
ITERATE_FOR_ALL_TYPES (DIV)
ITERATE_FOR_ALL_INT_TYPES (MOD)
ITERATE_FOR_ALL_INT_TYPES (XOR)
ITERATE_FOR_ALL_INT_TYPES (OR)
ITERATE_FOR_ALL_INT_TYPES (AND)
ITERATE_FOR_ALL_INT_TYPES (SHIFT_RIGHT)
ITERATE_FOR_ALL_INT_TYPES (SHIFT_LEFT)
ITERATE_FOR_ALL_TYPES (MINUS)
ITERATE_FOR_ALL_INT_TYPES (BIT_COMPLEMENT)
ITERATE_FOR_ALL_TYPES (MADD)
ITERATE_FOR_ALL_TYPES (MSUB)
ITERATE_FOR_ALL_INT_VECTOR_TYPES (SHUFFLE1)
ITERATE_FOR_ALL_INT_VECTOR_TYPES (SHUFFLE2)
REAL_SHUFFLE1 (v2f64, v2i64)
REAL_SHUFFLE2 (v2f64, v2i64)
REAL_SHUFFLE1 (v4f32, v4i32)
REAL_SHUFFLE2 (v4f32, v4i32)

/*
ITERATE_FOR_ALL_TYPES (DECLARE)
ITERATE_FOR_ALL_TYPES (RETURN)
ITERATE_FOR_ALL_TYPES (ASSIGN)
ITERATE_FOR_ALL_INT_TYPES (ADD_I)
ITERATE_FOR_ALL_INT_TYPES (SUB_I)
ITERATE_FOR_ALL_INT_TYPES (MUL_I)
ITERATE_FOR_ALL_INT_TYPES (DIV_I)
ITERATE_FOR_ALL_INT_TYPES (MOD_I)
ITERATE_FOR_ALL_INT_TYPES (XOR_I)
ITERATE_FOR_ALL_INT_TYPES (OR_I)
ITERATE_FOR_ALL_INT_TYPES (AND_I)
ITERATE_FOR_ALL_INT_TYPES (SHIFT_RIGHT_I)
ITERATE_FOR_ALL_INT_TYPES (SHIFT_LEFT_I)
ITERATE_FOR_ALL_REAL_TYPES (ADD_F)
ITERATE_FOR_ALL_REAL_TYPES (SUB_F)
ITERATE_FOR_ALL_REAL_TYPES (MUL_F)
ITERATE_FOR_ALL_REAL_TYPES (DIV_F)
ITERATE_FOR_ALL_TYPES (EQ)
ITERATE_FOR_ALL_TYPES (NEQ)
ITERATE_FOR_ALL_TYPES (LT)
ITERATE_FOR_ALL_TYPES (LEQ)
ITERATE_FOR_ALL_TYPES (GT)
ITERATE_FOR_ALL_TYPES (GEQ)
*/
