/* { dg-do compile } */
/* { dg-options "-fcilkplus" } */

#include <complex>

std::complex<double> A[16], a;

void
check ()
{
    a = __sec_reduce_add (A[:]); /* { dg-error "array notations in builtin functions must" } */ 
}

