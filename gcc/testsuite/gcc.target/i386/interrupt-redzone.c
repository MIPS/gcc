/* { dg-do compile { target { ! { ia32 } } } } */
/* { dg-options "-mabi=sysv" } */
extern int a;
void __attribute__((interrupt)) bar ()
  {
    a = 4;
  }

/* { dg-final { scan-assembler "addq\t\\\$-?128, %rsp" } } */
/* { dg-final { scan-assembler "subq\t\\\$-?128, %rsp" } } */
