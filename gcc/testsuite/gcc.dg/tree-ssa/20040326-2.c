/* { dg-options "-O2 -fno-inline-functions" } */
/* { dg-do run } */

/* Gimplification problem exposed by zsh.  All the side-effects in
   function arguments should happen before the actual function call.  */
int A;

foo (int x)
{
  if (A == x)
    abort ();
}

bar (int x, int y)
{
  if (x == 5 || y != 3)
    abort ();
}

main ()
{
  int b, c;

  /* The gimplifier was emitting A++ after the call.  */
  A = 5;
  foo (A++);

  /* The increment to 'b' and 'c' must happen before the call.  However,
     the first argument to bar() must be the original value of 'b', while
     the second argument must be the new value of 'c'.  */
  b = 4;
  c = 2;
  bar (b++, ++c);

  return 0;
}
