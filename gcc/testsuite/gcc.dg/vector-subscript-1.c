/* { dg-do compile } */
/* { dg-options "-w" } */

#define vector __attribute__((vector_size(16) ))
/* Check that vector[index] works and index[vector] is rejected.  */

float vf(vector float a)
{
  return 0[a]; /* { dg-error "" } */
}


float fv(vector float a)
{
  return a[0];
}
