/* { dg-do run { target powerpc*-*-* } } */
/* { dg-do compile { target i?86-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -maltivec" { target powerpc*-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -msse2" { target i?86-*-* } } */

#include <stdarg.h>
#include <signal.h>

#define N 16

float b[N] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};
float a[N];
float c[N];

int main1 (int n)
{
  int i=0;

  /* Vectorized: unknown loop bound.  */
  while (n--) {
    a[i] = b[i];
    i++;
  }

  /* check results:  */
  for (i = 0; i < n; i++)
    {
      if (a[i] != b[i])
        abort ();
    }

  return 0;
}

int main2 (unsigned int n)
{
  int i=0;

  /* Vectorized: unknown loop bound.  */
  while (n--) {
    c[i] = b[i];
    i++;
  }

  /* check results:  */
  for (i = 0; i < n; i++)
    {
      if (c[i] != b[i])
        abort ();
    }

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
  
  return main1 (N);
  return main2 (N);
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 2 "vect" } } */
