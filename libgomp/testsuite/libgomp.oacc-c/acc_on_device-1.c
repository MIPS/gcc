/* Disable the acc_on_device builtin; we want to test the libgomp library
   function.  */
/* TODO: Remove -DACC_DEVICE_TYPE_host once that is set by the test harness.  */
/* { dg-additional-options "-fno-builtin-acc_on_device -DACC_DEVICE_TYPE_host" } */

#include <stdlib.h>
#include <openacc.h>

int
main (int argc, char *argv[])
{
  /* Host.  */

  {
    if (!acc_on_device (acc_device_none))
      abort ();
    if (!acc_on_device (acc_device_host))
      abort ();
    if (acc_on_device (acc_device_not_host))
      abort ();
  }


  /* Host via offloading fallback mode.  */

#pragma acc parallel if(0)
  {
    if (!acc_on_device (acc_device_none))
      abort ();
    if (!acc_on_device (acc_device_host))
      abort ();
    if (acc_on_device (acc_device_not_host))
      abort ();
  }


#if !ACC_DEVICE_TYPE_host

  /* Offloaded.  */

#pragma acc parallel
  {
    if (acc_on_device (acc_device_none))
      abort ();
    if (acc_on_device (acc_device_host))
      abort ();
    if (!acc_on_device (acc_device_not_host))
      abort ();
  }

#endif

  return 0;
}
