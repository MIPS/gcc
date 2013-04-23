#if HAVE_IO
#include <iostream>
#endif
#define N 256
#define B 5
#define L 10
#define S 3
int A[N];

int main () { 
    A[:] = 2; 
    if (__sec_reduce_max_ind (A[B:L:S]) != B + (L - 1) * S) 
      {
#if HAVE_IO
	std::cout << __sec_reduce_max_ind (A[B:L:S]) << std::endl;
#endif	 
	return 1; 
      }
    return 0;
}

