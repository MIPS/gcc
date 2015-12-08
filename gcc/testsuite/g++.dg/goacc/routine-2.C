/* Test invalid use of the routine directive.  */

// { dg-do compile }
// { dg-options "-fopenacc" }

template <typename T>
extern T one_d();
#pragma acc routine (one_d) nohost /* { dg-error "names a set of overloads" } */

template <typename T>
T
one()
{
  return 1;
}
#pragma acc routine (one) bind(one_d) /* { dg-error "names a set of overloads" } */
