/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -mno-avx512f -mno-iamcu -mavx -maccumulate-outgoing-args" } */

extern void bar (void);
typedef unsigned int uword_t __attribute__ ((mode (__word__)));

void
 __attribute__ ((interrupt))
foo (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
  bar ();
}

/* { dg-final { scan-assembler-times "movups\[\\t \]*%ymm\[0-9\]+,\[\\t \]*-\[0-9\]*\\(%\[re\]?bp\\)" 16 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "movups\[\\t \]*-\[0-9\]*\\(%\[re\]?bp\\),\[\\t \]*%ymm\[0-9\]+" 16 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "movups\[\\t \]*%ymm\[0-9\]+,\[\\t \]*-\[0-9\]*\\(%\[re\]?bp\\)" 8 { target ia32 } } } */
/* { dg-final { scan-assembler-times "movups\[\\t \]*-\[0-9\]*\\(%\[re\]?bp\\),\[\\t \]*%ymm\[0-9\]+" 8 { target ia32 } } } */
/* { dg-final { scan-assembler-not "pushq\[\\t \]*%rbx" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "pushq\[\\t \]*%r1\[2-5\]+" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "pushl\[\\t \]*%ebx" { target ia32 } } } */
/* { dg-final { scan-assembler-not "pushl\[\\t \]*%e(s|d)i" { target ia32 } } } */
