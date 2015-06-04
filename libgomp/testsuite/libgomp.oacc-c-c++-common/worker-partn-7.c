#include <assert.h>

/* Test correct synchronisation between vector-partitioned loops in
   worker-partitioned mode.  */

int
main (int argc, char *argv[])
{
  int n[32 * 32], arr_a[32 * 32 * 32], arr_b[32 * 32 * 32], i;
  int num_workers, num_gangs;

  for (num_workers = 1; num_workers <= 32; num_workers <<= 1)
    for (num_gangs = 1; num_gangs <= 32; num_gangs <<= 1)
      {
	for (i = 0; i < 32 * 32 * 32; i++)
	  arr_a[i] = i;

	for (i = 0; i < 32 * 32; i++)
          n[i] = 0;

	#pragma acc parallel copy (n) copyin(arr_a) copyout(arr_b) \
		num_gangs(num_gangs) num_workers(num_workers) vector_length(32)
	{
	  int j;
	  #pragma acc loop gang
	  for (j = 0; j < 32; j++)
	    {
	      int k;

	      #pragma acc loop worker
	      for (k = 0; k < 32; k++)
		{
		  int m;

		  n[j * 32 + k]++;

		  #pragma acc loop vector
		  for (m = 0; m < 32; m++)
		    {
	              if (((j * 1024 + k * 32 + m) % 2) == 0)
			arr_b[j * 1024 + k * 32 + (31 - m)]
			  = arr_a[j * 1024 + k * 32 + m] * 2;
		      else
			arr_b[j * 1024 + k * 32 + (31 - m)]
			  = arr_a[j * 1024 + k * 32 + m] * 3;
		    }

		  /* Test returning to vector-single mode...  */
		  n[j * 32 + k]++;

		  #pragma acc loop vector
		  for (m = 0; m < 32; m++)
		    {
	              if (((j * 1024 + k * 32 + m) % 3) == 0)
			arr_a[j * 1024 + k * 32 + (31 - m)]
			  = arr_b[j * 1024 + k * 32 + m] * 5;
		      else
			arr_a[j * 1024 + k * 32 + (31 - m)]
			  = arr_b[j * 1024 + k * 32 + m] * 7;
		    }

		  /* ...and back-to-back vector loops.  */

		  #pragma acc loop vector
		  for (m = 0; m < 32; m++)
		    {
	              if (((j * 1024 + k * 32 + m) % 2) == 0)
			arr_b[j * 1024 + k * 32 + (31 - m)]
			  = arr_a[j * 1024 + k * 32 + m] * 3;
		      else
			arr_b[j * 1024 + k * 32 + (31 - m)]
			  = arr_a[j * 1024 + k * 32 + m] * 2;
		    }
		}
	    }
	}

	for (i = 0; i < 32 * 32; i++)
          assert (n[i] == 2);

	for (i = 0; i < 32 * 32 * 32; i++)
          {
	    int m = 6 * ((i % 3) == 0 ? 5 : 7);
	    assert (arr_b[i] == (i ^ 31) * m);
	  }
      }


  return 0;
}
