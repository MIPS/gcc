/* { dg-do run { target { ! openacc_nvidia_accel_supported } } } */

#include <openacc.h>

int
main (void)
{
  acc_init (acc_device_nvidia);

  acc_shutdown (acc_device_nvidia);

  return 0;
}

/* { dg-output "device type nvidia not supported" } */
/* { dg-shouldfail "" } */
