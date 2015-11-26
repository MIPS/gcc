/* Test invalid intra-routine parallelism.  */
/* { dg-do compile } */

#pragma acc routine gang
int
gang (int red)
{
#pragma acc loop reduction (+:red)
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop gang reduction (+:red)
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop worker reduction (+:red)
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop vector reduction (+:red)
  for (int i = 0; i < 10; i++)
    red ++;

  return 1;
}

#pragma acc routine worker
int
worker (int red)
{
#pragma acc loop reduction (+:red)
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop gang reduction (+:red) // { dg-error "disallowed by containing routine" }
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop worker reduction (+:red)
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop vector reduction (+:red)
  for (int i = 0; i < 10; i++)
    red ++;

  return 1;
}

#pragma acc routine vector
int
vector (int red)
{
#pragma acc loop reduction (+:red)
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop gang reduction (+:red) // { dg-error "disallowed by containing routine" }
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop worker reduction (+:red) // { dg-error "disallowed by containing routine" }
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop vector reduction (+:red)
  for (int i = 0; i < 10; i++)
    red ++;

  return 1;
}

#pragma acc routine seq
int
seq (int red)
{
#pragma acc loop reduction (+:red) // { dg-warning "insufficient partitioning" }
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop gang reduction (+:red) // { dg-error "disallowed by containing routine" }
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop worker reduction (+:red) // { dg-error "disallowed by containing routine" }
  for (int i = 0; i < 10; i++)
    red ++;

#pragma acc loop vector reduction (+:red) // { dg-error "disallowed by containing routine" }
  for (int i = 0; i < 10; i++)
    red ++;

  return 1;
}
