/* { dg-options "-m32 isa_rev>=6" } */

#define F 140
#define T 13

int
flt (x, y)
     long long int x;
     long long int y;
{
  if (x < y)
    return T;
  else
    return F;
}

int
fgt (x, y)
     long long int x;
     long long int y;
{
  if (x > y)
    return T;
  else
    return F;
}

/* { dg-final { scan-assembler "\tbltc" } } */
