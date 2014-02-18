// { dg-do compile }

void foo()
{
  bad1:
  #pragma acc parallel
    goto bad1; // { dg-error "invalid branch to/from OpenACC structured block" }

  goto bad2; // { dg-error "invalid entry to OpenACC structured block" }
  #pragma acc parallel
    {
      bad2: ;
    }

  #pragma acc parallel
    {
      int i;
      goto ok1;
      for (i = 0; i < 10; ++i)
	{ ok1: break; }
    }
}
