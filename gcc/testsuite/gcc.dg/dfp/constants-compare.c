/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Compare floating point constants against each other. */

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
  if (1.4 + 1.4 + 1.4 == 4.2)
    abort();

  /* But, this looks more like what one would expect. */
  if (1.4dd + 1.4dd + 1.4dd != 4.2dd)
    abort();

  /* Test use gcc builtins for comparisons. */
  if (__builtin_isless(-2.0dl,-2.0dl))
    abort();

  if (__builtin_isgreaterequal(-2.0dl,.01dl))
    abort();

  if (!(__builtin_islessequal(-2.0dl, -2.0dd)))
    abort();

  if (!(__builtin_isgreater(2.0dl, -2.0dd)))
    abort();

  if (__builtin_islessequal(2.0df, __builtin_nand64("")))
    abort();

  if (__builtin_islessgreater(2.0dd, __builtin_nand64("")))
    abort();

  if (!__builtin_islessgreater(2.0dd, -2.0dd))
    abort();

  if (!__builtin_islessgreater(-3.0dd, 2.0dd))
    abort();

  if (__builtin_isunordered(1.1df, 0.003dd))
    abort();

  if (!__builtin_isunordered(-3.1df, __builtin_nand32("")))
    abort();

  return 0;
}
