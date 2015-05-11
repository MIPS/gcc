/* Test the nohost clause for OpenACC routine directive.  Exercising different
   variants for declaring routines.  */

/* { dg-additional-options "-fdump-tree-oaccdevlow" } */

#pragma acc routine nohost
int THREE(void)
{
  return 3;
}

/* { dg-final { scan-tree-dump "Discarding function .THREE. with .nohost. clause" "oaccdevlow" } } */


#pragma acc routine nohost
extern void NOTHING(void);

void NOTHING(void)
{
}

/* { dg-final { scan-tree-dump "Discarding function .NOTHING. with .nohost. clause" "oaccdevlow" } } */


extern float ADD(float, float);

#pragma acc routine (ADD) nohost

float ADD(float x, float y)
{
  return x + y;
}

/* { dg-final { scan-tree-dump "Discarding function .ADD. with .nohost. clause" "oaccdevlow" } } */
