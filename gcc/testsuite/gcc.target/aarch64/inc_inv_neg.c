/* { dg-do compile } */
/* { dg-options "-O3" } */

int inc(float *a, float *b, int c)
{
  int i;
  if (*a!=*b)
      c++;
  /* { dg-final { scan-assembler "csinc\t" } } */
  return c;
}

int inv(float *a, float *b, int c)
{
  int i;
  if (*a==*b)
      c = ~c;
  /* { dg-final { scan-assembler "csinv\t" } } */
  return c;
}

int neg(float *a, float *b, int c)
{
  int i;
  if (*a!=*b)
      c = -c;
  /* { dg-final { scan-assembler "csneg\t" } } */
  return c;
}
