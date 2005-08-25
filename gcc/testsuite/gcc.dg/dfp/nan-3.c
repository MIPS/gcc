/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* N1107 8 Arithmetic Operations.  A few simple checks on arithmetic 
   operations.  Based on nan-1.c with the consideration of negative zero.  */

extern void abort (void);

int main()
{
  _Decimal32 d32 = 0.0df;

  if (!__builtin_isnand32(-(d32/-0.0df)))
    abort();
  if (!__builtin_isnand32(-(0.0df/-0.0df)))
    abort();

  return 0;
}
