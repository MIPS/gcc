/* { dg-do compile } */

int
main (int argc, char **argv)
{
  float a, b;

  a = 2.0;
  b = 0.0;

  #pragma acc parallel copy (a, b) device_type (acc_device_host) num_gangs (1) device_type (acc_device_nvidia) num_gangs (2)
  {
  }

  #pragma acc parallel copy (a, b) num_gangs (3) device_type (acc_device_host) num_gangs (1) device_type (acc_device_nvidia) num_gangs (2)
  {
  }

#pragma acc parallel copy (a, b) device_type (acc_device_host) num_gangs (1) device_type (acc_device_nvidia) num_gangs (2) device_type (acc_device_host) num_gangs (60) /* { dg-error "duplicate device_type" } */
  {
  }
  
#pragma acc parallel copy (a, b) num_gangs (3) device_type (nvidia) num_gangs (1) device_type (nvidia) num_gangs (2) /* { dg-error "duplicate device_type" } */
  {
  }

  return 0;
}
