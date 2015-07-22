/* Expect an error message when shutting down a device different from the one
   that has been initialized.  */
/* { dg-do run { target { ! openacc_host_selected } } } */

#include <openacc.h>

int
main (int argc, char **argv)
{
  acc_init (acc_device_host);
  acc_shutdown (acc_device_default);

  return 0;
}

/* { dg-output "no device initialized" } */
/* { dg-shouldfail "" } */
