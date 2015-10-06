/* { dg-do compile } */
/* { dg-options "-O2" } */

extern int check_int (int i, void *, int align);
typedef int aligned __attribute__((aligned(64)));
typedef unsigned int uword_t __attribute__ ((mode (__word__)));

__attribute__((interrupt))
void
foo (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
  aligned j;
  if (check_int (eflags, &j, __alignof__(j)))
    __builtin_abort ();
}

/* { dg-final { scan-assembler-times "and\[lq\]?\[^\\n\]*-64,\[^\\n\]*sp" 1 } } */
/* { dg-final { scan-assembler "iret" } }*/
