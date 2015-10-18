/* { dg-do compile } */
/* { dg-additional-options "-fmax-errors=200" } */


void par1 (void)
{
  int i, j;

#pragma acc parallel
  {
#pragma acc loop gang(5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop gang(num:5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop worker(5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop worker(num:5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop vector(5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop vector(length:5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }

   }
}

void k2 (void)
{
  int i, j;

#pragma acc kernels loop gang
  for (i = 0; i < 10; i++)
    {
#pragma acc kernels loop gang // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
    }

#pragma acc kernels loop worker
  for (i = 0; i < 10; i++)
    {
#pragma acc kernels loop worker // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
#pragma acc kernels loop gang // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
    }
  
#pragma acc kernels loop vector
  for (i = 0; i < 10; i++)
    {
#pragma acc kernels loop vector // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
#pragma acc kernels loop worker // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
#pragma acc kernels loop gang // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
    }
}

void p2 (void)
{
  int i, j;

#pragma acc parallel loop gang(5) // { dg-error "no arguments allowed to gang" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "no arguments allowed to gang" "" { target c++ } }
    { }
#pragma acc parallel loop gang(num:5) // { dg-error "no arguments allowed to gang" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "no arguments allowed to gang" "" { target c++ } }
    { }

#pragma acc parallel loop gang
  for (i = 0; i < 10; i++)
    {
#pragma acc parallel loop gang // { dg-error "OpenACC construct inside of non-OpenACC region" }
    for (j = 1; j < 10; j++) 
      { }
    }

#pragma acc parallel loop worker(5) // { dg-error "no arguments allowed to gang" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "no arguments allowed to gang" "" { target c++ } }
    { }
#pragma acc parallel loop worker(num:5) // { dg-error "no arguments allowed to gang" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "no arguments allowed to gang" "" { target c++ } }
    { }
#pragma acc parallel loop worker
  for (i = 0; i < 10; i++)
    {
#pragma acc parallel loop worker // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
#pragma acc parallel loop gang // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
    }

#pragma acc parallel loop vector(5) // { dg-error "no arguments allowed to gang" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "no arguments allowed to gang" "" { target c++ } }
    { }
#pragma acc parallel loop vector(length:5) // { dg-error "no arguments allowed to gang" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "no arguments allowed to gang" "" { target c++ } }
    { }
#pragma acc parallel loop vector
  for (i = 0; i < 10; i++)
    {
#pragma acc parallel loop vector // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
#pragma acc parallel loop worker // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
#pragma acc parallel loop gang // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
    }
}

