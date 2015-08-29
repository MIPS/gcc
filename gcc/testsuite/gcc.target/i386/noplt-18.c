/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-pic -fno-plt=bar" } */

extern int bar (void);
static __typeof(bar) __bar __attribute__ ((__weakref__("bar")));

int
active_p (void)
{
  static void *const active_ptr = __extension__ (void *) &__bar;
  return active_ptr != 0;
}

/* { dg-final { scan-assembler "__bar@GOTPCREL" { target { { ! ia32 } && r_x86_64_gotpcrelx } } } } */
/* { dg-final { scan-assembler "__bar@GOT" { target { ia32 && r_386_got32x } } } } */
