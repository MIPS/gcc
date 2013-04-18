/* { dg-do run } */
/* { dg-options "-fcilkplus" } */

#if HAVE_IO
#include <iostream>
#endif
#include <limits.h>
int A[16];
int a = 0;

int main () {
      if (__sec_reduce_max(A[0:0:2]) != INT_MIN) {
#if HAVE_IO
	  std::cout << __sec_reduce_max(A[0:0:2]) << std::endl;
#endif
	    a++;
      }
        if (__sec_reduce_min(A[0:0:2]) != INT_MAX) {
#if HAVE_IO
	  std::cout << __sec_reduce_min(A[0:0:2]) << std::endl;
#endif
	      a++;
	}

	  return a;
}


