/* { dg-do run } */

#include <stdlib.h>

struct by_lightning {
  int a;
  int b;
  int c;
};

int main (int argc, char* argv[])
{
  int x;
  void *q = NULL, *r = NULL, *p = NULL, *s = NULL, *t = NULL;
  long u;
  struct by_lightning on_the_head = {1, 2, 3};
  int arr[10], *f = NULL;
  _Complex float cf;
  #pragma acc enter data copyin (x, arr, on_the_head, cf)
  #pragma acc host_data use_device (x, arr, on_the_head, cf)
  {
    q = &x;
    {
      f = &arr[5];
      r = f;
      s = &__real__ cf;
      t = &on_the_head.c;
      u = (long) &__imag__ cf;
      #pragma acc parallel copyout(p) present (x, arr, on_the_head, cf)
      {
        /* This will not (and must not) call GOACC_deviceptr, but '&x' will be
	   the address on the device (if appropriate) regardless.  */
	p = &x;
      }
    }
  }
  #pragma acc exit data delete (x)

#if ACC_MEM_SHARED
  if (q != &x || f != &arr[5] || r != f || s != &(__real__ cf)
      || t != &on_the_head.c || u != (long) &(__imag__ cf) || p != &x)
    abort ();
#else
  if (q == &x || f == &arr[5] || r != f || s == &(__real__ cf)
      || t == &on_the_head.c || u == (long) &(__imag__ cf) || p == &x)
    abort ();
#endif

  return 0;
}
