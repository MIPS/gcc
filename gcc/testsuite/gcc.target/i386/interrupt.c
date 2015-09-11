/* { dg-do compile } */

extern void foo (void) __attribute__ ((interrupt));
extern int bar (int);

void foo ()
{
  int a,b,c,d,e,f,i;
  a = bar(5);
  b = bar(a);
  c = bar(b);
  d = bar(c);
  e = bar(d);
  f = bar(e);
  for (i = 1; i < 10; i++)
  {
    a += bar (a + i) + bar(b + i) + bar(c + i) + bar(d + i) + bar(e + i) + bar(f+i);
  }
}
/* { dg-final { scan-assembler "push.\t%.ax" } }*/
/* { dg-final { scan-assembler "pop.\t%.ax" } }*/
/* { dg-final { scan-assembler "push.\t%.dx" } }*/
/* { dg-final { scan-assembler "pop.\t%.dx" } }*/
/* { dg-final { scan-assembler "push.\t%.cx" } }*/
/* { dg-final { scan-assembler "pop.\t%.cx" } }*/
/* { dg-final { scan-assembler "push.\t%.bx" } }*/
/* { dg-final { scan-assembler "pop.\t%.bx" } }*/
/* { dg-final { scan-assembler "push.\t%.si" } }*/
/* { dg-final { scan-assembler "pop.\t%.si" } }*/
/* { dg-final { scan-assembler "push.\t%.di" } }*/
/* { dg-final { scan-assembler "pop.\t%.di" } }*/
/* { dg-final { scan-assembler "iret" } }*/
