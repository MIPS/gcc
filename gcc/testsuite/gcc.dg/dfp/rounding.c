/* { dg-do run } */
/* { dg-options "-std=gnu99 -O2" } */

void link_error (void);

int main()
{
  _Decimal32 d32_1,d32_2;
 
  d32_1 = 3.0df;
  d32_2 = 1.0df;
  /*
  if (!__builtin_constant_p( d32_1 + 0.2df ))
    link_error ();
  
  if (!__builtin_constant_p(1.0df / 3.0df ))
    link_error ();
  */
  if (__builtin_constant_p(d32_2/d32_1))
    link_error ();
  /*  
  d32_2 = 2.0df;
  if (!__builtin_constant_p(d32_2/d32_1))
    link_error ();
  */
  return 0;
}
