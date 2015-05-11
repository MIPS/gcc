int
main (void)
{
#pragma acc parallel
  {
    while (1)
      ;
  }

  return 0;
}
