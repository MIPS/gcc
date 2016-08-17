/* Test the nohost clause for OpenACC routine directive.  Exercising different
   variants for declaring routines.  */

/* { dg-additional-options "-fdump-tree-oaccdevlow" } */

#pragma acc routine nohost
int THREE(void)
{
  return 3;
}

#pragma acc routine (THREE) nohost

#pragma acc routine nohost
extern int THREE(void);


#pragma acc routine nohost
extern void NOTHING(void);

#pragma acc routine (NOTHING) nohost

void NOTHING(void)
{
}

#pragma acc routine nohost
extern void NOTHING(void);

#pragma acc routine (NOTHING) nohost


extern float ADD(float, float);

#pragma acc routine (ADD) nohost

float ADD(float x, float y)
{
  return x + y;
}

#pragma acc routine nohost
extern float ADD(float, float);

#pragma acc routine (ADD) nohost


/* { dg-final { scan-tree-dump-times "Discarding function" 3 "oaccdevlow" } } */
