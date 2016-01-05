/* { dg-do run } */
/* { dg-additional-options "-O1 -w" } */

#include <stdio.h>
#include <openacc.h>

#define WORK_ID(I,N)						\
  (acc_on_device (acc_device_nvidia)				\
   ? ({unsigned __r;						\
       __asm__ volatile ("mov.u32 %0,%%tid.y;" : "=r" (__r));	\
       __r; }) : (I % N))

#pragma acc routine worker
void Work (int *ptr, int lim, int N)
{
#pragma acc loop worker
  for (int i = 0; i < lim; i++)
    ptr[i] = WORK_ID(i, N);
}

#define LEN 32

int DoWork (int err, int N)
{
  int ary[LEN];

  for (int ix = 0; ix != LEN; ix++)
    ary[ix] = 0xdeadbeef;
  
#pragma acc parallel num_workers(N) copy (ary)
  {
    Work (ary, LEN, N);
  }

  for (int ix = 0; ix != LEN; ix++)
    if (ary[ix] != ix % N)
      {
	printf ("ary[%d] = %d expected %d\n", ix, ary[ix], ix % N);
	err = 1;
      }
  return err;
}


int main ()
{
  int err = 0;

  for (int W = 1; W <= LEN; W <<= 1)
    err = DoWork (err, W);

  return err;
}
