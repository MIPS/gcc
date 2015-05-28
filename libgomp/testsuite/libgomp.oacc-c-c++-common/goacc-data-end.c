/* { dg-do run } */
/* { dg-options "-O2" } */

/* Data directive at end of function.  This is the variant that triggered
   PR65419.  */

static void __attribute__((noinline,noclone))
f (void)
{
  int i;

#pragma acc data copyout (i)
  {

  }
}

/* Data directive in inlined function g_1.  */

static inline void
g_1 (void)
{
  int i;

#pragma acc data copyout (i)
  {

  }
}

static void __attribute__((noinline,noclone))
g (void)
{
  g_1 ();
}

/* Data directive in function h into which a function h_1 is inlined.  */

static inline void
h_1 (void)
{

}

static void __attribute__((noinline,noclone))
h (void)
{
  int i;

  h_1 ();

#pragma acc data copyout (i)
  {

  }
}

/* Main function calling the tests.  */

int
main (void)
{
  f ();
  g ();
  h ();

  return 0;
}
