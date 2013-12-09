/* { dg-do compile } */
/* { dg-options "-fopenacc -fmax-errors=100" } */

int test1()
{
  int i, j, k, b[10];
  int a[30];
  double d;
  float r;
  i = 0;
#pragma acc loop
  while(1)  /* { dg-error "for statement expected" } */
    {
      if (i > 0) break; 
      i = i + 1;
    }
  i = 0;
#pragma acc loop
  for(;;)  /* { dg-error "expected iteration declaration or initialization" } */
    {
      if (i > 0) break; /* { dg-error "break statement used" }{ */
      i = i + 1;
    }
  i = 0;
#pragma acc loop
  do  /* { dg-error "for statement expected" } */
    {
      i = i + 1;
    }
  while (i < 4);
#pragma acc loop
  while (i < 8)  /* { dg-error "for statement expected" } */
    {
      i = i + 1;
    }
#pragma acc loop
  for (d = 1; d < 30; d+= 6)  /* { dg-error "invalid type for iteration variable" } */
    {
      i = d;
      a[i] = 1;
    }
#pragma acc loop
  for (d = 1; d < 30; d+= 5)  /* { dg-error "invalid type for iteration variable" } */
    {
      i = d;
      a[i] = 2;
    }
#pragma acc loop
  for (i = 1; i < 30; i++ )
    if (i == 16) break; /* { dg-error "break statement used" }{ */

#pragma acc loop
for(i = 1; i < 30; i++)
  {
    for(j = 5; j < 10; j++)
      {
        if (i == 6 && j == 7) goto outer; /* { dg-error "invalid branch" }{ */
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
    a[i] = i;
  }

/* after loop directive must be loop */
#pragma acc loop
a[1] = 1; /* { dg-error "for statement expected" } */
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

#pragma acc parallel loop collapse(0) /* { dg-error "positive constant integer" } */
  for(i = 1; i < 10; i++)
    {
    }

#pragma acc parallel loop collapse(-1) /* { dg-error "positive constant integer" } */
  for(i = 1; i < 10; i++)
    {
    }

#pragma acc parallel loop collapse(i) /* { dg-error "positive constant integer" } */
  for(i = 1; i < 10; i++)
    {
    }

#pragma acc parallel loop collapse(4) 
  for (i = 1; i < 3; i++)
    {
      for (j = 4; j < 6; j++)
        {
          for (k = 5; k < 7; k++)
            {
              a[i+j-k] = i + j + k; /* { dg-error "not enough perfectly nested loops before" } */
            }
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
      k = 4;/* { dg-error "collapsed loops not perfectly nested before" } */
    }
#pragma acc parallel loop collapse(2)
  for (i = 1; i < 3; i++)
    {
      do			/* { dg-error "not enough perfectly nested loops before" } */
      {}
      while(0);
  }
#pragma acc parallel loop collapse(2)
for (i = 1; i < 3; i++)
  {
    for (r = 4; r < 6; r++)		/* { dg-error "invalid type for iteration variable" } */
      {
      }
  }


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
#pragma acc cache(a[1])
  }

for(i = 1; i < 10; i++)
  {
    a[i] = i;
#pragma acc cache(a[1])
  }

  return 0;

}
