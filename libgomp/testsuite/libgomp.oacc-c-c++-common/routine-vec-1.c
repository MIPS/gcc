/* This code uses nvptx inline assembly guarded with acc_on_device, which is
   not optimized away at -O0, and then confuses the target assembler.
   { dg-skip-if "" { *-*-* } { "-O0" } { "" } } */

#include <stdio.h>
#include <openacc.h>

#define VEC_ID(I, N)						\
  (acc_on_device (acc_device_nvidia)				\
   ? ({unsigned __r;						\
       __asm__ volatile ("mov.u32 %0,%%tid.x;" : "=r" (__r));	\
       __r; }) : (I % N))

#pragma acc routine vector
void Vec (int *ptr, int lim, int N)
{
#pragma acc loop vector
  for (int i = 0; i < lim; i++)
    ptr[i] = VEC_ID(i, N);
}

#define LEN 32

int main ()
{
  int ary[LEN];
  int err = 0;

  for (int ix = 0; ix != LEN; ix++)
    ary[ix] = 0xdeadbeef;
  
#pragma acc parallel vector_length(32) copy (ary)
  {
    Vec (ary, LEN, 32);
  }

  for (int ix = 0; ix != LEN; ix++)
    {
      if (ary[ix] != ix % 32)
	{
	  printf ("ary[%d] = %d expected %d\n", ix, ary[ix], ix % 32);
	  err = 1;
	}
    }

  return err;
}
