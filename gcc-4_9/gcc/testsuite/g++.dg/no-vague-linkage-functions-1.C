/* { dg-do compile } */
/* { dg-options "-fno-vague-linkage-functions -fkeep-inline-functions -ffunction-sections" } */

inline int foo () {
  return 0;
}

/* { dg-final { scan-assembler "_Z3foov" } } */
/* { dg-final { scan-assembler-not "_Z3foov.*comdat" } } */
