/* Test invalid calls to routines.  */
/* { dg-do compile } */

#pragma acc routine gang
int
gang ()
{
  return 1;
}

#pragma acc routine worker
int
worker ()
{
  return 1;
}

#pragma acc routine vector
int
vector ()
{
  return 1;
}

#pragma acc routine seq
int
seq ()
{
  return 1;
}

int
main ()
{
  int red = 0;
#pragma acc parallel copy (red)
  {
    /* Independent/seq loop tests.  */
#pragma acc loop reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += gang ();

#pragma acc loop reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += worker ();

#pragma acc loop reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += vector ();

    /* Gang routine tests.  */
#pragma acc loop gang reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += gang (); // { dg-error "incompatible parallelism with acc routine" }

#pragma acc loop worker reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += gang (); // { dg-error "incompatible parallelism with acc routine" }

#pragma acc loop vector reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += gang (); // { dg-error "incompatible parallelism with acc routine" }

    /* Worker routine tests.  */
#pragma acc loop gang reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += worker ();

#pragma acc loop worker reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += worker (); // { dg-error "incompatible parallelism with acc routine" }

#pragma acc loop vector reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += worker (); // { dg-error "incompatible parallelism with acc routine" }

    /* Vector routine tests.  */
#pragma acc loop gang reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += vector ();

#pragma acc loop worker reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += vector ();

#pragma acc loop vector reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += vector (); // { dg-error "incompatible parallelism with acc routine" }

    /* Seq routine tests.  */
#pragma acc loop gang reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += seq ();

#pragma acc loop worker reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += seq ();

#pragma acc loop vector reduction (+:red)
    for (int i = 0; i < 10; i++)
      red += seq ();
  }

  return 0;
}
