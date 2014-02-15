/* Test cvmx_shared attribute variables are loaded properly. */
/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

#define CVMX_SHARED __attribute__ ((cvmx_shared))

void abort (void);

CVMX_SHARED long i[10] = {1};  
CVMX_SHARED long *p = &i[1];
CVMX_SHARED long j;

main()
{
  i[1] = 0x12345678;

  j = *p;

  if (j != 0x12345678)
    abort ();
  if (j != *p)
    abort ();
  return 0;
}
