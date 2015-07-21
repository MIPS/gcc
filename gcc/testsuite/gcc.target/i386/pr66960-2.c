/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-omit-frame-pointer" { target { lp64 } } } */
/* { dg-options "-O2 -fno-omit-frame-pointer -maddress-mode=short" { target { x32 } } } */
/* { dg-options "-O2 -fno-omit-frame-pointer -miamcu" { target { ia32 } } } */

extern char **environ;
extern void exit (int status);
extern int main (long argc, char **argv, char **envp);

void
_start (void)
{
  void *argc_p = __builtin_stack_top ();
  char **argv = (char **) (argc_p + sizeof (void *));
  long argc = *(long *) argc_p;
  int status;

  environ = argv + argc + 1;

  status = main (argc, argv, environ);

  exit (status);
}

/* { dg-final { scan-assembler "movq\[ \t\]8\\(%rbp\\), %rdi" { target lp64 } } } */
/* { dg-final { scan-assembler "leaq\[ \t\]16\\(%rbp\\), %rsi" { target lp64 } } } */
/* { dg-final { scan-assembler "leaq\[ \t\]24\\(%rbp,%rdi,8\\), %rdx" { target lp64 } } } */
/* { dg-final { scan-assembler "movl\[ \t\]8\\(%ebp\\), %edi" { target x32 } } } */
/* { dg-final { scan-assembler "leal\[ \t\]12\\(%rbp\\), %esi" { target x32 } } } */
/* { dg-final { scan-assembler "leal\[ \t\]4\\(%rsi,%rdi,4\\), %edx" { target x32 } } } */
/* { dg-final { scan-assembler "movl\[ \t\]4\\(%ebp\\), %eax" { target ia32 } } } */
/* { dg-final { scan-assembler "leal\[ \t\]8\\(%ebp\\), %edx" { target ia32 } } } */
/* { dg-final { scan-assembler "leal\[ \t\]12\\(%ebp,%eax,4\\), %ecx" { target ia32 } } } */
