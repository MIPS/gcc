/* { dg-do compile } */
/* { dg-options "-fopenacc -fmax-errors=100" } */

int test1()
{
  int i, j, k, b(10);
  int a[30];
  double d;
  float r;
  i = 0;
#pragma acc loop
  while(1)  /* { dg-error "cannot be a WHILE without loop control" } */
    {
      if (i > 0) break; /* { dg-error "BREAK statement" }{ */
      i = i + 1;
    }
  i = 0;
#pragma acc loop
  for(;;)  /* { dg-error "cannot be a WHILE without loop control" } */
    {
      if (i > 0) break; /* { dg-error "BREAK statement" }{ */
      i = i + 1;
    }
  i = 0;
#pragma acc loop
  do  /* { dg-error "cannot be a WHILE without loop control" } */
    {
      i = i + 1;
    }
  while (i < 4);
#pragma acc loop
  while (i < 8)  /* { dg-error "cannot be a WHILE without loop control" } */
    {
      i = i + 1;
    }
#pragma acc loop
  for (d = 1; d < 30; d+= 6)  /* { dg-error "integer" } */
    {
      i = d;
      a(i) = 1;
    }
#pragma acc loop
  for (d = 1; d < 30; d+= 5)  /* { dg-error "integer" } */
    {
      i = d;
      a(i) = 2;
    }
#pragma acc loop
  for (i = 1; i < 30; i++ )
    if (i == 16) break; /* { dg-error "BREAK statement" }{ */
}
#pragma acc loop
for(i = 1; i < 30; i++)
  {
    for(j = 5; j < 10; j++)
      {
        if (i == 6 && j == 7) break outer; /* { dg-error "BREAK statement" }{ */
      }
  }
outer:

/* different types of loop are allowed */
#pragma acc loop
for (i = 1; i < 10; i++)
  {
  }
#pragma acc loop
for (i = 1; i < 10; i+=2)
  {
    a(i) = i;
  }

/* after loop directive must be loop */
#pragma acc loop
a(1) = 1; /* { dg-error "Unexpected" } */
       for (i = 1; i < 10; i++)
  {
  }
/* combined directives may be used*/
#pragma acc parallel loop
for(i = 1; i < 10; i++)
  {
  }
#pragma acc kernels loop
for(i = 1; i < 10; i++)
  {
  }

/* reduction clause not allowed in kernels region */
#pragma acc kernels loop reduction(max:i) /* { dg-error "KERNELS region" } */
for(i = 1; i < 10; i++)
  {
  }
#pragma acc kernels
{
#pragma acc loop reduction(max:i) /* { dg-error "KERNELS region" } */
  for(i = 1; i < 10; i++)
    {
    }


#pragma acc parallel loop collapse(0) /* { dg-error "constant positive integer" } */
  for(i = 1; i < 10; i++)
    {
    }

#pragma acc parallel loop collapse(-1) /* { dg-error "constant positive integer" } */
  for(i = 1; i < 10; i++)
    {
    }

#pragma acc parallel loop collapse(i) /* { dg-error "Constant expression required" } */
  for(i = 1; i < 10; i++)
    {
    }

#pragma acc parallel loop collapse(4) /* { dg-error "not enough FOR loops for collapsed" } */
  for (i = 1; i < 3; i++)
    {
      for (j = 4; j < 6; j++)
        {
          for (k = 5; k < 7; k++)
            {
              a(i+j-k) = i + j + k;
            }
        }
    }
#pragma acc parallel loop collapse(2)
  for(i = 1; i < 5; i+=2)
    {
      for(j = i + 1; j < 7; j+=i) 	/* { dg-error "collapsed loops don.t form rectangular iteration space" } */
        {
        }
    }
#pragma acc parallel loop collapse(2)
  for(i = 1; i < 3; i++)
    {
      for(j = 4; j < 6; j++)
        {
        }
    }
#pragma acc parallel loop collapse(2)
  for(i = 1; i < 3; i++)
    {
      for(j = 4; j < 6; j++)
        {
        }
      k = 4;
    }
#pragma acc parallel loop collapse(2)
  for (i = 1; i < 3; i++)
    {
      do			/* { dg-error "cannot be a WHILE without loop control" } */
      }
  while(0)
  }
#pragma acc parallel loop collapse(2)
for (i = 1; i < 3; i++)
  {
    for (r = 4, r < 6; r++)		/* { dg-error "integer" } */
      {
      }
  }

/* Both seq and independent are not allowed */
#pragma acc loop independent seq /* { dg-error "not allowed" } */
for(i = 1; i < 10; i++)
  {
  }


#pragma acc cache (a(1)) /* { dg-error "inside of loop" } */

for(i = 1; i < 10; i++)
  {
#pragma acc cache (b) /* { dg-error "single array elements and subarrays" } */
  }

for(i = 1; i < 10; i++)
  {
#pragma acc cache (a) /* { dg-error "single array elements and subarrays" } */
  }

for(i = 1; i < 10; i++)
  {
#pragma acc cache (i) /* { dg-error "single array elements and subarrays" } */
  }

for(i = 1; i < 10; i++)
  {
#pragma acc cache(a(1))
  }

for(i = 1; i < 10; i++)
  {
    a(i) = i
#pragma acc cache(a(1))
  }

}
/* { dg-excess-errors "Deleted" } */
