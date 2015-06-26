/* { dg-compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mips32 -mabi=32" } */

__attribute__ ((target("arch=mips32r0")))
unsigned long
test_arch ()
{ /* { dg-error "unrecognized argument in the target attribute arch=" } */
  return 0;
}
