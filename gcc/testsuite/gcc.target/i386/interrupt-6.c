/* { dg-do compile } */
/* { dg-options "-O2" } */

typedef unsigned int uword_t __attribute__ ((mode (__word__)));

extern int error;

__attribute__((interrupt))
void
fn1 (uword_t error_code, short ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{ /* { dg-error "interrupt service routine should have unsigned \(long long |long |\)int as argument 2" } */
}

__attribute__((interrupt))
void
fn2 (void)
{ /* { dg-error "interrupt service routine can only have 3 to 6 unsigned \(long long |long |\)int arguments" } */
}

extern int fn3 (uword_t error, uword_t ip, uword_t cs, uword_t eflags)
  __attribute__ ((interrupt)); /* { dg-error "interrupt service routine can't have non-void return value" } */

int
fn5 (uword_t error, uword_t ip, uword_t cs, uword_t eflags)
{
  return 0;
}
