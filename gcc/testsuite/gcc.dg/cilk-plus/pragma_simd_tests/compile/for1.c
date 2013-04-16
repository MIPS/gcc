/* { dg-do compile } */
/* { dg-options "-O3 -std=c99 -fcilkplus -fopenmp" } */

int *a, *b;

void foo()
{
  int i, j;

  // The initialization shall declare or initialize a *SINGLE* variable.
#pragma simd
  for (i=0, j=5; i < 1000; i++) // { dg-error "expected ';' before ','" }
    a[i] = b[j];

  // Declaration and initialization is allowed.
#pragma simd
  for (int i=0; i < 1000; i++)
    a[i] = b[j];

  // Empty initialization is not allowed.
#pragma simd
  for (; i < 5; ++i)		// { dg-error "expected iteration decl" }
    a[i] = i;

#pragma simd
  i = 5; /* { dg-error "for statement expected after pragma simd" } */
}
