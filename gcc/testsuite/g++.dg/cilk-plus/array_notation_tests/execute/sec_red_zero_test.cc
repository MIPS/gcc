/* { dg-do run }  */
/* { dg-options "-fcilkplus" } */

#if HAVE_IO
#include <iostream>
#endif
int A[16];

int main () {
      A[:] = 5;
#if HAVE_IO
        std::cout << __sec_reduce_any_zero(A[:]) << "\n";
#else
	if (__sec_reduce_any_zero (A[:]))
	  return 1;
#endif
	A[:] = 0;
#if HAVE_IO
        std::cout << __sec_reduce_any_nonzero(A[:]) << "\n";
#else
	if (__sec_reduce_any_nonzero (A[:]))
	  return 1; 
#endif
      return 0;
}

