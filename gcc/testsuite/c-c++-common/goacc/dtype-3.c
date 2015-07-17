/* { dg-do compile { xfail *-*-* } } */
/* { dg-prune-output "sorry, unimplemented: device_type clause is not supported yet" } */

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

#pragma acc parallel device_type (nvidia) num_gangs (1)
  {
  }

#pragma acc parallel device_type (host, nvidia) num_gangs (1)
  {
  }

#pragma acc parallel device_type (host) num_gangs (1)
  {
  }

#pragma acc parallel device_type (foo)
  {
  }

  return 0;
}
