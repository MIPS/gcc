/* devidence of two massives */
/* { dg-do compile } */
/* { dg-options "-fopenacc" } */

#include <stdio.h>
#include <stdlib.h>

#define SIZE 64

int main()
{
  float *a, *b, *c;
  size_t i;
  a = (float*) malloc(SIZE * sizeof(float));
  b = (float*) malloc(SIZE * sizeof(float));
  c = (float*) malloc(SIZE * sizeof(float));
  
  for (i = 0; i < SIZE; i++)
    {
      a[i] = (i+1.0f)/10.0f;
      b[i] = (SIZE-i)/10.0f;
    }

  #pragma acc kernels
    for (i = 0; i < SIZE; i++)
      c[i] = a[i]/b[i];

  for (i = 0; i < SIZE; i++) 
    printf("%f ", c[i]);

  return 0;
}
      
