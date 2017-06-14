/* Test attribute for clearing hazards while returning.  */
/* { dg-do compile } */
/* { dg-options "isa_rev>=2 -mno-mips16" } */

extern int bar ();

int __attribute__ ((use_hazard_barrier_return))
foo ()
{
  return bar ();
}
/* { dg-final { scan-assembler "\tjr.hb\t$31\n" } } */
/* { dg-final { scan-assembler "\tnop\n" } } */
/* Test attribute for clearing hazards while returning.  */
/* { dg-do compile } */
/* { dg-options "isa_rev>=2 -mno-mips16" } */

extern int bar ();

int __attribute__ ((use_hazard_barrier_return))
foo ()
{
  return bar ();
}
/* { dg-final { scan-assembler "\tjr.hb\t$31\n" } } */
/* { dg-final { scan-assembler "\tnop\n" } } */
