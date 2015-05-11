int
main (void)
{

#pragma acc parallel
  {
    __builtin_abort ();
  }

  return 0;
}

