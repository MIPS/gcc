// { dg-options "-fopenmp" }

typedef __sizeless_1 ta;

void
f (void)
{
  ta ta1; // { dg-error {'ta1' has incomplete type} }
#pragma omp for private (ta1)
  // { dg-error {invalid use of sizeless type 'ta'} "" { target *-*-* } .-1 }
  for (int i = 0; i < 10; ++i)
    ;
}
