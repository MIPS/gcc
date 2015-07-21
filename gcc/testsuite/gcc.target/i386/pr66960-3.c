/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -mno-accumulate-outgoing-args" { target { lp64 } } } */
/* { dg-options "-O2 -mno-accumulate-outgoing-args -maddress-mode=short" { target { x32 } } } */
/* { dg-options "-O2 -mno-accumulate-outgoing-args -miamcu" { target { ia32 } } } */

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
} /* { dg-message "sorry, unimplemented: .__builtin_stack_top. not supported" } */
