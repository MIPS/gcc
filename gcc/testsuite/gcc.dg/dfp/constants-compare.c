/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* Compare floating point constants against each other. */
#include <stdio.h>
extern void abort (void);

int main()
{
  /* Compare like-typed positive constants. */
  if (2.0df != 2.0df)
    abort();
  /* Compare like-typed negative constants. */
  if (-2.0dl < -2.0dl)
    abort();

  /* Compare like-typed constants. */
  if (-2.0dl >= .01dl)
    abort();

  /* Compare like-typed constants. */
  if (0.2dd > 0.02e1dd)
    abort();
  
  /* Compare decimal float constants of different types. */
  if (500e-2dl != 0.05e2df)
    abort();

  /* Compare decimal float constants of different types. */
  if (-.000005dd >= -.0000049DL)
  abort();

  /* Binary floating point introduces errors to decimal values. */
  if (-.01 == -.01dd)
    abort();

  return 0;
}
