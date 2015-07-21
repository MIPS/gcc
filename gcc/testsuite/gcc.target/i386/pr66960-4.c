/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -maccumulate-outgoing-args" { target { lp64 } } } */
/* { dg-options "-O2 -maccumulate-outgoing-args -maddress-mode=short" { target { x32 } } } */
/* { dg-options "-O2 -maccumulate-outgoing-args -miamcu" { target { ia32 } } } */

extern void abort (void);
extern int check_int (int *i, int align);
typedef int aligned __attribute__((aligned(64)));

void *
foo (void)
{
  aligned j;
  if (check_int (&j, __alignof__(j)) != j)
    abort ();
  return __builtin_stack_top ();
}

/* { dg-final { scan-assembler "leaq\[ \t\]8\\(%rbp\\), %rax" { target lp64 } } } */
/* { dg-final { scan-assembler "leal\[ \t\]8\\(%rbp\\), %eax" { target x32 } } } */
/* { dg-final { scan-assembler "leal\[ \t\]4\\(%ebp\\), %eax" { target ia32 } } } */
