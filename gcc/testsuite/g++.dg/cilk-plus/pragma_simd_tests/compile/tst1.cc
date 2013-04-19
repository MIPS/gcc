/* { dg-do compile } */
/* { dg-options "-fcilkplus -O3 -w" } */

#define VLEN 4

/* The clauses really doesn't make sense in this, the main thing this code
   is checking is to see if it can catch the for-loop  correctly after
   the clauses, and compile successfully.  */
int main (void)
{
  int array[1000];
  int i, nphi = 100, w;


#pragma simd vectorlength(VLEN) reduction(+:w) 
for (i = 0; i < nphi; i++) 
{
  w += array[i];
}

#pragma simd reduction(+:w) vectorlength (VLEN)
for (i = 0; i < nphi; i++) 
{
  w += array[i];
}

#pragma simd vectorlength (VLEN) private(array)
for (i = 0; i < nphi; i++) 
{
  w += array[i];
}

#pragma simd reduction(+:w) noassert
for (i = 0; i < nphi; i++) 
{
  w += array[i];
}

#pragma simd vectorlength(VLEN) linear(nphi:1)
for (i = 0; i < nphi; i++) 
{
  w += array[i];
}

#pragma simd linear(nphi) vectorlength(VLEN) linear(nphi:1)
for (i = 0; i < nphi; i++) 
{
  w += array[i];
}
return w;
}

