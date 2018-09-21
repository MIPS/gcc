/* { dg-options "-m32 -march=32r6 -fno-omit-frame-pointer" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

extern int bar (void*, void*);

int foo(void)
{
 char loc;
 char buff[1024];
 return bar(&loc, buff) + 1;
}

/* { dg-final { scan-assembler-times "\taddiu\t\\\$fp,\\\$sp," 1 } } */
/* { dg-final { scan-assembler-times "\taddiu\t\\\$a0,\\\$sp," 1 } } */
/* { dg-final { scan-assembler-times "\taddiu\t\\\$a1,\\\$sp," 1 } } */
/* { dg-final { scan-assembler-not "\taddiu\t\\\$sp,\\\$fp," } } */
