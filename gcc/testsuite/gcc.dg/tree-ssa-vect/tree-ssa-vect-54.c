/* APPLE LOCAL file -haifa  */
/* { dg-do run { target powerpc*-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -maltivec" { target powerpc*-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -msse" { target i?86-*-* } } */

#include <stdarg.h>
#include <signal.h>

#define N 256

typedef float afloat __attribute__ ((__aligned__(16)));

void bar (afloat *pa, afloat *pb, afloat *pc)
{
  int i;

  /* check results:  */
  for (i = 0; i < N/2; i++)
    {
      if (pa[i+1] != (pb[i+1] * pc[i+1]))
        printf("%f * %f != %f\n",pb[i],pc[i],pa[i]);
    }

  return;
}


int
main1 (afloat * __restrict__ pa, afloat * __restrict__ pb, afloat * __restrict__ pc)
{
  int i;

  for (i = 0; i < N/2; i++)
    {
      pa[i+1] = pb[i+1] * pc[i+1];
    }

  bar (pa,pb,pc);

  return 0;
}

void
sig_ill_handler (int sig)
{
    exit(0);
}

int main (void)
{
  int i;
  afloat a[N];
  afloat b[N] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,51,54,57};
  afloat c[N] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

  /* Exit on systems without altivec.  */
  signal (SIGILL, sig_ill_handler);
  /* Altivec instruction, 'vor %v0,%v0,%v0'.  */
  asm volatile (".long 0x10000484");
  signal (SIGILL, SIG_DFL);

  main1 (a,b,c);
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { xfail *-*-* } } } */
