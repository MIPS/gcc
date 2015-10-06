/* { dg-do compile } */
/* { dg-options "-O2" } */
typedef unsigned int uword_t __attribute__ ((mode (__word__)));

extern void bar (int);

void
f1 (void)
{
  bar (1); 
}

__attribute__((interrupt))
void f2 (uword_t error, uword_t ip, uword_t cs, uword_t eflags, uword_t sp,
	 uword_t ss)
{
  bar (2);
}

void
f3 (void)
{
  bar (3);
}

__attribute__((interrupt))
void
f4 (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
  bar (4);
}

void
f5 (void)
{
  bar (5);
}

/* { dg-final { scan-assembler-times "push.\t%.ax" 2 } } */
/* { dg-final { scan-assembler-times "pop.\t%.ax" 2 } } */
/* { dg-final { scan-assembler-times "iret" 2 } } */
