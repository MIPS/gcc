/* Test if we error out on MSA if we compile with -mfp32 as MSA can only be
   build with -mfp64 that is set either implicitly or explicitly.  */
/* { dg-compile } */
/* { dg-require-effective-target mips_msa } */
/* { dg-options "-mips32r2 -mno-msa -mfp32 -ftree-vectorize" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

#define N 4

int a[N];
int b[N];
int c[N];

__attribute__ ((target("msa")))
void
test_msa ()
{ /* { dg-error "unsupported combination" } */
  int i;
  for (i = 0; i < N; i++)
    c[i] = a[i] + b[i];
}
