/* { dg-do compile } */

extern void bar(int);

void f1(){  bar(1); }
__attribute__((interrupt)) 
void f2(){  bar(2); }
void f3(){  bar(3); }
__attribute__((exception)) 
void f4(){  bar(4); }
void f5(){  bar(5); }

/* { dg-final { scan-assembler-times "iret" 2 } } */
