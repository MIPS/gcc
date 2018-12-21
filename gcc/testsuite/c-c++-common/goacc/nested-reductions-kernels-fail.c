/* Test erroneous cases of nested reduction loops in kernels regions,
   corresponding to nested-reductions-parallel-fail.c.  */

/* FIXME:  No diagnostics are produced for these loops because reductions
   in kernels regions are not supported yet.  */

void acc_kernels (void)
{
  int i, j, k, l, sum, diff;

  #pragma acc kernels
  {
    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop collapse(2)
      for (j = 0; j < 10; j++)
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop
      for (j = 0; j < 10; j++)
        #pragma acc loop
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(-:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum)
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum)
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(+:sum) reduction(-:diff)
    for (i = 0; i < 10; i++)
      {
        #pragma acc loop reduction(-:diff)
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(+:sum)
          for (k = 0; k < 10; k++)
            sum = 1;

        #pragma acc loop reduction(+:sum)
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(-:diff)
          for (k = 0; k < 10; k++)
            diff = 1;
      }
  }
}

/* The same tests as above, but using a combined kernels loop construct.  */

void acc_kernels_loop (void)
{
  int i, j, k, l, sum, diff;

  #pragma acc kernels loop
  for (int h = 0; h < 10; ++h)
  {
    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop collapse(2)
      for (j = 0; j < 10; j++)
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop
      for (j = 0; j < 10; j++)
        #pragma acc loop
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(-:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum)
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum)
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(+:sum) reduction(-:diff)
    for (i = 0; i < 10; i++)
      {
        #pragma acc loop reduction(-:diff)
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(+:sum)
          for (k = 0; k < 10; k++)
            sum = 1;

        #pragma acc loop reduction(+:sum)
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(-:diff)
          for (k = 0; k < 10; k++)
            diff = 1;
      }
  }
}

/* The same tests as above, but using a combined kernels loop construct, and
   the outermost reduction clause is on that one, not the outermost loop.  */

void acc_kernels_loop_reduction (void)
{
  int i, j, k, l, sum, diff;

  #pragma acc kernels loop reduction(+:sum)
  for (int h = 0; h < 10; ++h)
  {
    #pragma acc loop
    for (i = 0; i < 10; i++)
      #pragma acc loop
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop
    for (i = 0; i < 10; i++)
      #pragma acc loop collapse(2)
      for (j = 0; j < 10; j++)
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop
    for (i = 0; i < 10; i++)
      #pragma acc loop
      for (j = 0; j < 10; j++)
        #pragma acc loop
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(-:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(max:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum)
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(max:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum)
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum)
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(-:diff)
    for (i = 0; i < 10; i++)
      {
        #pragma acc loop reduction(-:diff)
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(+:sum)
          for (k = 0; k < 10; k++)
            sum = 1;

        #pragma acc loop reduction(+:sum)
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(-:diff)
          for (k = 0; k < 10; k++)
            diff = 1;
      }
  }
}
