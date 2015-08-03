#include "gomp-constants.h"

/* For when the builtin is explicitly disabled.  */
int acc_on_device (int d)
{
  /* We can't use the builtin itself here, because that only expands
     to device-like things inside offloaded compute regions, which
     this isn't.  Even though it'll be executed on the device --
     unless someone builds a host-side PTX compiler, which would be
     very strange.  */
  return d == GOMP_DEVICE_NOT_HOST || d == GOMP_DEVICE_NVIDIA_PTX;
}

int acc_on_device_h_(int *d)
{
  return acc_on_device(*d);
}
