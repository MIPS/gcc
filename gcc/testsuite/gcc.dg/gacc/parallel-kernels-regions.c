/* { dg-do compile } */
/* { dg-options "-fopenacc" } */

int main()
{

#pragma acc parallel
  {
#pragma acc kernels /* { dg-error "parallel/kernels regions" } */
    {}
  }

#pragma acc parallel
  {
#pragma acc parallel /* { dg-error "parallel/kernels regions" } */
    {}
  }

#pragma acc parallel
  {
#pragma acc parallel /* { dg-error "parallel/kernels regions" } */
    {}
#pragma acc kernels /* { dg-error "parallel/kernels regions" } */
    {}
  }

#pragma acc kernels
  {
#pragma acc kernels /* { dg-error "parallel/kernels regions" } */
    {}
  }

#pragma acc kernels
  {
#pragma acc parallel /* { dg-error "parallel/kernels regions" } */
    {}
  }

#pragma acc kernels
  {
#pragma acc parallel /* { dg-error "parallel/kernels regions" } */
    {}
#pragma acc kernels /* { dg-error "parallel/kernels regions" } */
    {}
  }

  /* However, data construct is allowed */

#pragma acc parallel
  {
#pragma acc data
    {}
  }

#pragma acc kernels
  {
#pragma acc data
    {}
  }

}