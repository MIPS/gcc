/* { dg-do run { target { powerpc*-*-linux* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-skip-if "" { powerpc*-*-*spe* } { "*" } { "" } } */
/* { dg-require-effective-target vsx_hw } */
/* { dg-options "-mcpu=power7 -O2 -mfloat128-vsx -static-libgcc" } */

#ifdef DEBUG
#include <stdio.h>
#endif

#if !defined(__FLOAT128_VSX__) && !defined(__FLOAT128_REF__)
static __float128
pass_through (__float128 x)
{
  return x;
}

__float128 (*no_optimize) (__float128) = pass_through;
#endif

void
abort_test (const char *string)
{
#ifdef DEBUG
  printf ("Test %s failed\n", string);
#endif
  __builtin_abort ();
}


int
main (void)
{
  __float128 one = 1.0q;
  __float128 two = 2.0q;
  __float128 three = 3.0q;
  __float128 four = 4.0q;
  __float128 five = 5.0q;
  __float128 add_result = (1.0q + 2.0q);
  __float128 mul_result = ((1.0q + 2.0q) * 3.0q);
  __float128 div_result = (((1.0q + 2.0q) * 3.0q) / 4.0q);
  __float128 sub_result = ((((1.0q + 2.0q) * 3.0q) / 4.0q) - 5.0q);
  __float128 add_xresult;
  __float128 mul_xresult;
  __float128 div_xresult;
  __float128 sub_xresult;

#if defined(__FLOAT128_VSX__)
  __asm__ (" #prevent constant folding, %x0" : "+wa" (one));
  __asm__ (" #prevent constant folding, %x0" : "+wa" (two));
  __asm__ (" #prevent constant folding, %x0" : "+wa" (three));
  __asm__ (" #prevent constant folding, %x0" : "+wa" (four));
  __asm__ (" #prevent constant folding, %x0" : "+wa" (five));

#elif defined(__FLOAT128_REF__)
  __asm__ (" #prevent constant folding, %x0" : "+d" (one));
  __asm__ (" #prevent constant folding, %x0" : "+d" (two));
  __asm__ (" #prevent constant folding, %x0" : "+d" (three));
  __asm__ (" #prevent constant folding, %x0" : "+d" (four));
  __asm__ (" #prevent constant folding, %x0" : "+d" (five));

#else
  one   = no_optimize (one);
  two   = no_optimize (two);
  three = no_optimize (three);
  four  = no_optimize (four);
  five  = no_optimize (five);
#endif

  add_xresult = (one + two);
  if (add_xresult != add_result)
    abort_test ("add");

  mul_xresult = add_xresult * three;
  if (mul_xresult != mul_result)
    abort_test ("multiply");

  div_xresult = mul_xresult / four;
  if (div_xresult != div_result)
    abort_test ("divide");

  sub_xresult = div_xresult - five;
  if (sub_xresult != sub_result)
    abort_test ("subtract");

#ifdef DEBUG
  printf ("Passed\n");
#endif

  return 0;
}
