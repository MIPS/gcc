
#pragma acc routine gang
void gang (void) /* { dg-warning "partitioned" 3 } */
{
}

#pragma acc routine worker
void worker (void) /* { dg-warning "partitioned" 2 } */
{
}

#pragma acc routine vector
void vector (void) /* { dg-warning "partitioned" 1 } */
{
}

#pragma acc routine seq
void seq (void)
{
}

int main ()
{
#pragma acc kernels num_gangs (32) num_workers (32) vector_length (32)
  {
    gang ();
    worker ();
    vector ();
    seq ();
  }

#pragma acc parallel num_gangs (32) num_workers (32) vector_length (32)
  {
    gang ();
    worker ();
    vector ();
    seq ();
  }

  return 0;
}
