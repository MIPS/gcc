/* { dg-do compile { xfail *-*-* } } */

int
main (int argc, char **argv)
{
  float a, b;

  a = 2.0;
  b = 0.0;

  #pragma acc parallel copy (a, b) device_type (host) num_gangs (1) device_type (nvidia) num_gangs (2) /* { dg-message "sorry, unimplemented: device_type clause is not supported yet" } */
  {
  }

  #pragma acc parallel copy (a, b) num_gangs (3) device_type (host) num_gangs (1) device_type (nvidia) num_gangs (2) /* { dg-message "sorry, unimplemented: device_type clause is not supported yet" } */
  {
  }

#pragma acc parallel copy (a, b) device_type (host) num_gangs (1) device_type (nvidia) num_gangs (2) device_type (host) num_gangs (60) /* { dg-message "sorry, unimplemented: device_type clause is not supported yet" } */
  {
  }
  
#pragma acc parallel copy (a, b) num_gangs (3) device_type (nvidia) num_gangs (1) device_type (nvidia) num_gangs (2) /* { dg-message "sorry, unimplemented: device_type clause is not supported yet" } */
  {
  }

  return 0;
}
