/* { dg-do run { target powerpc*-*-* i?86-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect -maltivec" { target powerpc*-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect -msse" { target i?86-*-* } } */

#include <stdarg.h>
#include <signal.h>

#define N 16

int iadd_results[N] = {0,6,12,18,24,30,36,42,48,54,60,66,72,78,84,90};
float fadd_results[N] = {0.0,6.0,12.0,18.0,24.0,30.0,36.0,42.0,48.0,54.0,60.0,66.0,72.0,78.0,84.0,90.0};
float fmul_results[N] = {0.0,3.0,12.0,27.0,48.0,75.0,108.0,147.0,192.0,243.0,300.0,363.0,432.0,507.0,588.0,675.0};

/****************************************************/
void icheck_results (int *a, int *results)
{
  int i;
  for (i = 0; i < N; i++)
    {
      if (a[i] != results[i])
	abort ();
    }
}

void fcheck_results (float *a, float *results)
{
  int i;
  for (i = 0; i < N; i++)
    {
      if (a[i] != results[i])
	abort ();
    }
}   

void 
fbar_mul (float *a)
{
  fcheck_results (a, fmul_results);
} 

void 
fbar_add (float *a)
{
  fcheck_results (a, fadd_results);
} 

void 
ibar_add (int *a)
{
  icheck_results (a, iadd_results);
} 

int
main1 ()
{
  int i;
  float a[N];
  float e[N];
  float b[N] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45};
  float c[N] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  float d[N] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};
  int ic[N] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45};
  int ib[N] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45};
  int ia[N];

  for (i = 0; i < N; i++){
    a[i] = b[i] * c[i];
  }
  fbar_mul (a);

  for (i = 0; i < N; i++){
    a[i] = b[i] + c[i] + d[i];
    e[i] = b[i] + c[i] + d[i];
    ia[i] = ib[i] + ic[i];
  }
  ibar_add (ia);
  fbar_add (a);
  fbar_add (e);

  return 0;
}

void
sig_ill_handler (int sig)
{
    exit(0);
}
  
int main (void)
{
  /* Exit on systems without altivec.  */
  signal (SIGILL, sig_ill_handler);
  /* Altivec instruction, 'vor %v0,%v0,%v0'.  */
  asm volatile (".long 0x10000484");
  signal (SIGILL, SIG_DFL);

  return main1 ();
}

/* { dg-final { scan-tree-dump-times "vectorized " 3 "vect"} } */
/* { dg-final { scan-tree-dump-times "vectorized 0 loops" 2 "vect"} } */
/* { dg-final { scan-tree-dump-times "vectorized 2 loops" 1 "vect"} } */
