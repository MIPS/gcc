/* { dg-do compile } */
/* { dg-additional-options "-fmax-errors=200" } */

void
kern (void)
{
  int i, j;

#pragma acc kernels
  {
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
#pragma acc loop tile(-2) // { dg-warning "'tile' value must be positive" }
    for (i = 0; i < 10; i++)
      { }
#pragma acc loop tile(i)
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
}


void k3 (void)
{
  int i, j;

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
#pragma acc kernels loop tile(-2) // { dg-warning "'tile' value must be positive" }
  for (i = 1; i < 10; i++)
    { }
#pragma acc kernels loop tile(i)
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
}
