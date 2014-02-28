// { dg-do compile }

void foo()
{
  bad1:
  #pragma acc parallel
    goto bad1; // { dg-error "invalid branch to/from OpenACC structured block" }
  #pragma acc kernels
    goto bad1; // { dg-error "invalid branch to/from OpenACC structured block" }
  #pragma acc data
    goto bad1; // { dg-error "invalid branch to/from OpenACC structured block" }

  goto bad2_parallel; // { dg-error "invalid entry to OpenACC structured block" }
  #pragma acc parallel
    {
      bad2_parallel: ;
    }

  goto bad2_kernels; // { dg-error "invalid entry to OpenACC structured block" }
  #pragma acc kernels
    {
      bad2_kernels: ;
    }

  goto bad2_data; // { dg-error "invalid entry to OpenACC structured block" }
  #pragma acc data
    {
      bad2_data: ;
    }

  #pragma acc parallel
    {
      int i;
      goto ok1_parallel;
      for (i = 0; i < 10; ++i)
	{ ok1_parallel: break; }
    }

  #pragma acc kernels
    {
      int i;
      goto ok1_kernels;
      for (i = 0; i < 10; ++i)
	{ ok1_kernels: break; }
    }

  #pragma acc data
    {
      int i;
      goto ok1_data;
      for (i = 0; i < 10; ++i)
	{ ok1_data: break; }
    }
}
