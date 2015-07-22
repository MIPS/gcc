/* Expect error message when shutting down a device that has never been
   initialized.  */
/* { dg-do run } */

#include <openacc.h>

int
main (int argc, char **argv)
{
  acc_shutdown (acc_device_default);

  return 0;
}

/* { dg-output "no device initialized" } */
/* { dg-shouldfail "" } */
