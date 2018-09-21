/* { dg-options "-m32 -march=32r6 " } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-Os"} { "" } } */

extern int bar (void*, void*);

int foo (unsigned size)
{
  char loc;
  char* buff = __builtin_alloca(size);
  return bar(&loc, buff) + 1;
}

/* { dg-final { scan-assembler-times "\taddiu\t\\\$fp,\\\$sp," 1 } } */
/* { dg-final { scan-assembler-times "\taddiu\t\\\$a0,\\\$fp," 1 } } */
/* { dg-final { scan-assembler-times "\tmove\t\\\$a1,\\\$sp" 1 } } */
/* { dg-final { scan-assembler-times "\taddiu\t\\\$sp,\\\$fp," 1 } } */
