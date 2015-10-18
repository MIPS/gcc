/* { dg-do compile } */

int
main ()
{
  int i, j;
#pragma acc kernels loop gang
  for (i = 0; i < 10; i++)
    {
#pragma acc kernels loop gang // { dg-error "OpenACC construct inside of" "" }
      for (i = 0; i < 10; i++)
	{ }
    }

  return 0;
}
