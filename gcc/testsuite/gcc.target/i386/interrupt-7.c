/* { dg-do compile } */
/* { dg-options "-O2" } */

typedef unsigned int uword_t __attribute__ ((mode (__word__)));

extern void fn (uword_t error, uword_t ip, uword_t cs, uword_t eflags)
  __attribute__((interrupt));

void
foo (void)
{
  fn (1, 2, 3, 4); /* { dg-error "interrupt service routine can't be called directly" } */
}
