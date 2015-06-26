/* Test if we error out in functions were built-in function should not be
   available and test if a built-in from DSP rev. 2 is not recognized.  */
/* { dg-compile } */
/* { dg-options "-mips32r2 -mno-dsp" } */
/* { dg-skip-if "" { *-*-* } { "-fuse-linker-plugin" } { "" } } */

typedef signed char v4i8 __attribute__ ((vector_size (4)));

#pragma GCC push_options
#pragma GCC target("dsp")
v4i8 a, b, c;
void
test_dsp ()
{
  c = __builtin_mips_subu_qb (a, b);
}
#pragma GCC pop_options

//__attribute__ ((target("dsp")))
int
test_dsp2 (int a, int b, int *c)
{
  *c = __builtin_mips_adduh_qb (a, b); /* { dg-warning "implicit declaration of function" } */
  return __builtin_mips_addsc (a, b); /* { dg-error "requires target pragma or attribute" } */
}
