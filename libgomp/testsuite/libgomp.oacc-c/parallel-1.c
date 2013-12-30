/* { dg-do run } */

extern void abort ();

volatile int i;

int main(void)
{
  volatile int j;

  i = -0x42;
  j = -42;
#pragma acc parallel
  {
    if (i != -0x42 || j != -42)
      abort ();
    i = 42;
    j = 0x42;
    if (i != 42 || j != 0x42)
      abort ();
  }
  if (i != 42 || j != 0x42)
    abort ();

  return 0;
}
