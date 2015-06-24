/* { dg-do run { target openacc_nvidia_accel_selected } } */

int
main (int argc, char **argv)
{
  int a[8] __attribute__((unused));
#pragma acc declare present (a)
}

/* { dg-shouldfail "" } */
