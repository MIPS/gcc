/* { dg-do compile } */
/* { dg-prune-output "sorry, unimplemented: device_type clause is not supported yet" } */

void
test ()
{
  int i1, i2;

  /* ACC PARALLEL DEVICE_TYPE: */

#pragma acc parallel dtype (nvidia) async (1) num_gangs (100) num_workers (100) vector_length (32) wait (1) copy (i1) /* { dg-error "not valid" } */
  {
  }

  /* ACC KERNELS DEVICE_TYPE: */

#pragma acc kernels device_type (nvidia) async wait copy (i1) /* { dg-error "not valid" } */
  {
  }

  /* ACC LOOP DEVICE_TYPE: */

#pragma acc parallel
#pragma acc loop device_type (nvidia) gang tile (1) private (i2) /* { dg-error "not valid" } */
  for (i1 = 1; i1 < 10; i1++)
    {
    }

  /* ACC UPDATE DEVICE_TYPE: */

#pragma acc update host(i1) dtype (nvidia) async(1) wait (1) self (i2) /* { dg-error "not valid" } */
}
