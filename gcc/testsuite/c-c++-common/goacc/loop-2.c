/* { dg-do compile } */
/* { dg-additional-options "-fmax-errors=200" } */

int
main ()
{
  int i, j;

#pragma acc kernels
  {
#pragma acc loop auto
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang(5)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang(num:5)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang(static:5)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang(static:*)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang // { dg-error "gang, worker and vector may occur only once in a loop nest" }
    for (i = 0; i < 10; i++)
      {
#pragma acc loop vector 
	for (j = 0; j < 10; j++)
	  { }
#pragma acc loop worker 
	for (j = 0; j < 10; j++)
	  { }
#pragma acc loop gang
	for (j = 0; j < 10; j++)
	  { }
      }
#pragma acc loop seq gang // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop worker
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker(5)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker(num:5)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker // { dg-error "gang, worker and vector may occur only once in a loop nest" }
    for (i = 0; i < 10; i++)
      {
#pragma acc loop vector 
	for (j = 0; j < 10; j++)
	  { }
#pragma acc loop worker
	for (j = 0; j < 10; j++)
	  { }
#pragma acc loop gang
	for (j = 0; j < 10; j++)
	  { }
      }
#pragma acc loop seq worker // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang worker
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop vector
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector(5)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector(length:5)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector // { dg-error "gang, worker and vector may occur only once in a loop nest" }
    for (i = 0; i < 10; i++)
      {
#pragma acc loop vector
	for (j = 1; j < 10; j++)
	  { }
#pragma acc loop worker
	for (j = 1; j < 10; j++)
	  { }
#pragma acc loop gang
	for (j = 1; j < 10; j++)
	  { }
      }
#pragma acc loop seq vector // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang vector
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker vector
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop auto
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop seq auto // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang auto // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker auto // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector auto // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop tile // { dg-error "expected" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile() // { dg-error "expected" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(1)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(2)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(6-2) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(6+2) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(*, 1) 
    for (i = 0; i < 10; i++)
      {
	for (j = 0; j < 10; i++)
	  { }
      }
#pragma acc loop tile(-1) // { dg-error "tile argument needs positive constant integer" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(i) // { dg-error "tile argument needs positive constant integer" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(2, 2, 1)
    for (i = 2; i < 4; i++)
      for (i = 4; i < 6; i++)
	{ }
#pragma acc loop tile(2, 2)
    for (i = 1; i < 5; i+=2)
      for (j = i+1; j < 7; i++)
	{ }
#pragma acc loop vector tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector gang tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector worker tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang worker tile(*) 
    for (i = 0; i < 10; i++)
      { }
  }


#pragma acc parallel
  {
#pragma acc loop auto
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang(5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang(num:5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang(static:5)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang(static:*)
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang // { dg-error "gang, worker and vector may occur only once in a loop nest" }
    for (i = 0; i < 10; i++)
      {
#pragma acc loop vector
	for (j = 1; j < 10; j++)
	  { }
#pragma acc loop worker 
	for (j = 1; j < 10; j++)
	  { }
#pragma acc loop gang
	for (j = 1; j < 10; j++)
	  { }
      }
#pragma acc loop seq gang // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop worker
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker(5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker(num:5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker // { dg-error "gang, worker and vector may occur only once in a loop" }
    for (i = 0; i < 10; i++)
      {
#pragma acc loop vector 
	for (j = 1; j < 10; j++)
	  { }
#pragma acc loop worker
	for (j = 1; j < 10; j++)
	  { }
#pragma acc loop gang
	for (j = 1; j < 10; j++)
	  { }
      }
#pragma acc loop seq worker // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang worker
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop vector
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector(5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector(length:5) // { dg-error "no arguments allowed to gang" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector // { dg-error "gang, worker and vector may occur only once in a loop nest" }
    for (i = 0; i < 10; i++)
      {
#pragma acc loop vector
	for (j = 1; j < 10; j++)
	  { }
#pragma acc loop worker
	for (j = 1; j < 10; j++)
	  { }
#pragma acc loop gang
	for (j = 1; j < 10; j++)
	  { }
      }
#pragma acc loop seq vector // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang vector
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker vector
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop auto
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop seq auto // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang auto // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker auto // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector auto // { dg-error "incompatible use of clause" }
    for (i = 0; i < 10; i++)
      { }

#pragma acc loop tile // { dg-error "expected" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile() // { dg-error "expected" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(1) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(2) 
    for (i = 0; i < 10; i++)
      {
	for (j = 1; j < 10; j++)
	  { }
      }
#pragma acc loop tile(-1) // { dg-error "tile argument needs positive constant integer expression" }
    for (i = 1; i < 10; i++)
      { }
#pragma acc loop tile(i) // { dg-error "tile argument needs positive constant integer expression" }
    for (i = 1; i < 10; i++)
      { }
#pragma acc loop tile(2, 2, 1)
    for (i = 1; i < 3; i++)
      {
	for (j = 4; j < 6; j++)
	  { }
      } 
#pragma acc loop tile(2, 2)
    for (i = 1; i < 5; i+=2)
      {
	for (j = i + 1; j < 7; j+=i)
	  { }
      }
#pragma acc loop vector tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop worker tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector gang tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop vector worker tile(*) 
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop gang worker tile(*) 
    for (i = 0; i < 10; i++)
      { }
  }

#pragma acc kernels loop auto
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop gang
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop gang(5)
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop gang(num:5)
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop gang(static:5)
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop gang(static:*)
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop gang
  for (i = 0; i < 10; i++)
    {
#pragma acc kernels loop gang // { dg-error "OpenACC construct inside of non-OpenACC region" }
      for (j = 1; j < 10; j++)
	{ }
    }
#pragma acc kernels loop seq gang // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }

#pragma acc kernels loop worker
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop worker(5)
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop worker(num:5)
  for (i = 0; i < 10; i++)
    { }
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
#pragma acc kernels loop seq worker // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }
#pragma acc kernels loop gang worker
  for (i = 0; i < 10; i++)
    { }

#pragma acc kernels loop vector
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop vector(5)
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop vector(length:5)
  for (i = 0; i < 10; i++)
    { }
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
#pragma acc kernels loop seq vector // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }
#pragma acc kernels loop gang vector
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop worker vector
  for (i = 0; i < 10; i++)
    { }

#pragma acc kernels loop auto
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop seq auto // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }
#pragma acc kernels loop gang auto // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }
#pragma acc kernels loop worker auto // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }
#pragma acc kernels loop vector auto // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }

#pragma acc kernels loop tile // { dg-error "expected" }
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop tile() // { dg-error "expected" }
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop tile(1) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop tile(*, 1) 
  for (i = 0; i < 10; i++)
    {
      for (j = 1; j < 10; j++)
	{ }
    }
#pragma acc kernels loop tile(-1) // { dg-error "tile argument needs positive constant integer expression" }
  for (i = 1; i < 10; i++)
    { }
#pragma acc kernels loop tile(i) // { dg-error "tile argument needs positive constant integer expression" }
  for (i = 1; i < 10; i++)
    { }
#pragma acc kernels loop tile(2, 2, 1)
  for (i = 1; i < 3; i++)
    {
      for (j = 4; j < 6; j++)
	{ }
    }    
#pragma acc kernels loop tile(2, 2)
  for (i = 1; i < 5; i++)
    {
      for (j = i + 1; j < 7; j += i)
	{ }
    }
#pragma acc kernels loop vector tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop worker tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop gang tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop vector gang tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop vector worker tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc kernels loop gang worker tile(*) 
  for (i = 0; i < 10; i++)
    { }

#pragma acc parallel loop auto
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop gang
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop gang(5) // { dg-error "no arguments allowed to gang" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "no arguments allowed to gang" "" { target c++ } }
    { }
#pragma acc parallel loop gang(num:5) // { dg-error "no arguments allowed to gang" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "no arguments allowed to gang" "" { target c++ } }
    { }
#pragma acc parallel loop gang(static:5)
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop gang(static:*)
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop gang
  for (i = 0; i < 10; i++)
    {
#pragma acc parallel loop gang // { dg-error "OpenACC construct inside of non-OpenACC region" }
    for (j = 1; j < 10; j++)
      { }
    }
#pragma acc parallel loop seq gang // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }

#pragma acc parallel loop worker
  for (i = 0; i < 10; i++)
    { }
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
#pragma acc parallel loop seq worker // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }
#pragma acc parallel loop gang worker
  for (i = 0; i < 10; i++)
    { }

#pragma acc parallel loop vector
  for (i = 0; i < 10; i++)
    { }
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
#pragma acc parallel loop seq vector // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }
#pragma acc parallel loop gang vector
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop worker vector
  for (i = 0; i < 10; i++)
    { }

#pragma acc parallel loop auto
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop seq auto // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }
#pragma acc parallel loop gang auto // { dg-error "incompatible use of clause"  "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause"  "" { target c++ } }
    { }
#pragma acc parallel loop worker auto // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }
#pragma acc parallel loop vector auto // { dg-error "incompatible use of clause" "" { target c } }
  for (i = 0; i < 10; i++) // { dg-error "incompatible use of clause" "" { target c++ } }
    { }

#pragma acc parallel loop tile // { dg-error "expected" }
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop tile() // { dg-error "expected" }
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop tile(1) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop tile(*, 1) 
  for (i = 0; i < 10; i++)
    {
      for (j = 1; j < 10; j++)
	{ }
    }
#pragma acc parallel loop tile(-1) // { dg-error "tile argument needs positive constant integer expression" }
  for (i = 1; i < 10; i++)
    { }
#pragma acc parallel loop tile(i) // { dg-error "tile argument needs positive constant integer expression" }
  for (i = 1; i < 10; i++)
    { }
#pragma acc parallel loop tile(2, 2, 1)
  for (i = 1; i < 3; i++)
    {
      for (j = 4; j < 6; j++)
        { }
    }    
#pragma acc parallel loop tile(2, 2)
  for (i = 1; i < 5; i+=2)
    {
      for (j = i + 1; j < 7; j++)
        { }
    }
#pragma acc parallel loop vector tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop worker tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop gang tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop vector gang tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop vector worker tile(*) 
  for (i = 0; i < 10; i++)
    { }
#pragma acc parallel loop gang worker tile(*) 
  for (i = 0; i < 10; i++)
    { }

  return 0;
}

