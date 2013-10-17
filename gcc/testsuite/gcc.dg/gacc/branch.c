/* { dg-do compile } */
/* { dg-options "-fopenacc" } */

int main() {
  int  i;

  if (1) {
  #pragma acc parallel
  }  /* { dg-error "Unexpected" } */
  #pragma acc end parallel
  } 

  if (1) {
  #pragma acc kernels
  }  /* { dg-error "Unexpected" } */
  #pragma acc end kernels
  } 

  #pragma acc parallel
  if (1) {
  #pragma acc end parallel /* { dg-error "Unexpected" } */
  } 
  #pragma acc end parallel

  #pragma acc kernels
  if (1) {
  #pragma acc end kernels /* { dg-error "Unexpected" } */
  } 
  #pragma acc end kernels

  #pragma acc parallel
  if (1) {
  } 
  #pragma acc end parallel

  #pragma acc kernels
  if (1) {
  } 
  #pragma acc end kernels

  if (1) {
  #pragma acc parallel
  #pragma acc end parallel
  } 

  if (1) {
  #pragma acc kernels
  #pragma acc end kernels
  } 

  #pragma acc parallel
  /* TODO: implement in lowering pass */
  goto aa /* { dg-error "branch|exit" } */
  #pragma acc end parallel
aa:  
  i = 0

}
