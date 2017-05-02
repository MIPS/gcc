/* Test the nohost clause for OpenACC routine directive.  Exercising different
   variants for declaring routines.  */

/* { dg-additional-options "-fdump-tree-oaccdevlow" } */

#pragma acc routine nohost seq
int THREE(void)
{
  return 3;
}

#pragma acc routine (THREE) nohost seq

#pragma acc routine nohost seq
extern int THREE(void);


#pragma acc routine nohost seq
extern void NOTHING(void);

#pragma acc routine (NOTHING) nohost seq

void NOTHING(void)
{
}

#pragma acc routine nohost seq
extern void NOTHING(void);

#pragma acc routine (NOTHING) nohost seq


extern float ADD(float, float);

#pragma acc routine (ADD) nohost seq

float ADD(float x, float y)
{
  return x + y;
}

#pragma acc routine nohost seq
extern float ADD(float, float);

#pragma acc routine (ADD) nohost seq


/* { dg-final { scan-tree-dump-times "Discarding function" 3 "oaccdevlow" } } */
