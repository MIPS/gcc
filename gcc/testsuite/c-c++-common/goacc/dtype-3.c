/* { dg-do compile } */

float b;
#pragma acc declare link (b)

int
main (int argc, char **argv)
{
  float a;

  a = 2.0;

#pragma acc parallel device_type (*) copy (a) /* { dg-error "not valid" } */
  {
  }

#pragma acc parallel device_type (acc_device_nvidia) num_gangs (1)
  {
  }

#pragma acc parallel device_type (acc_device_host, acc_device_nvidia) num_gangs (1)
  {
  }

#pragma acc parallel device_type (acc_device_host) num_gangs (1)
  {
  }

#pragma acc parallel device_type (foo)
  {
  }

  return 0;
}
