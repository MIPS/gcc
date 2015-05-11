/* { dg-do compile } */
/* { dg-options "-fopenacc -fdump-tree-omplower" } */

void
test ()
{
  int i1;

  /* ACC PARALLEL DEVICE_TYPE: */

#pragma acc parallel device_type (nVidia) async (1) num_gangs (100) num_workers (100) vector_length (32) wait (1)
  {
  }

#pragma acc parallel async (1) num_gangs (1) num_workers (1) vector_length (1) wait (1) dtype (nvidia) async (2) num_gangs (200) num_workers (200) vector_length (64) wait (2)
  {
  }

#pragma acc parallel async (1) num_gangs (1) num_workers (1) vector_length (1) wait (1) dtype (nvidia) async (3) num_gangs (300) num_workers (300) vector_length (128) wait (3) device_type (*) async (10) num_gangs (10) num_workers (10) vector_length (10) wait (10)
  {
  }

#pragma acc parallel async (1) num_gangs (1) num_workers (1) vector_length (1) wait (1) device_type (nvidia_ptx) async (3) num_gangs (300) num_workers (300) vector_length (128) wait (3) dtype (*) async (10) num_gangs (10) num_workers (10) vector_length (10) wait (10)
  {
  }

  /* ACC KERNELS DEVICE_TYPE: */

#pragma acc kernels device_type (nvidia) async wait
  {
  }

#pragma acc kernels async wait dtype (nvidia) async (1) wait (1)
  {
  }

#pragma acc kernels async wait dtype (nvidia) async (2) wait (2) device_type (*) async (0) wait (0)
  {
  }

#pragma acc kernels async wait device_type (nvidia_ptx) async (1) wait (1) dtype (*) async (0) wait (0)
  {
  }

  /* ACC LOOP DEVICE_TYPE: */

#pragma acc parallel
#pragma acc loop dtype (nVidia) gang
  for (i1 = 1; i1 < 10; i1++)
    {
    }

#pragma acc parallel
#pragma acc loop device_type (nVidia) gang dtype (*) worker
  for (i1 = 1; i1 < 10; i1++)
    {
    }

#pragma acc parallel
#pragma acc loop dtype (nVidiaGPU) gang device_type (*) vector
  for (i1 = 1; i1 < 10; i1++)
    {
    }

  /* ACC UPDATE DEVICE_TYPE: */

#pragma acc update host(i1) async(1) wait (1)

#pragma acc update host(i1) device_type(nvidia) async(2) wait (2)

#pragma acc update host(i1) async(1) wait (1) device_type(nvidia) async(3) wait (3)

#pragma acc update host(i1) async(4) wait (4) device_type(nvidia) async(5) wait (5) dtype (*) async (6) wait (6)

#pragma acc update host(i1) async(4) wait (4) dtype(nvidia1) async(5) wait (5) dtype (*) async (6) wait (6)
}

/* ACC ROUTINE DEVICE_TYPE: */

#pragma acc routine (foo1) device_type (nvidia) gang
#pragma acc routine (foo2) device_type (nvidia) worker
#pragma acc routine (foo3) dtype (nvidia) vector
#pragma acc routine (foo5) device_type (nvidia) bind (foo)
#pragma acc routine (foo6) device_type (nvidia) gang device_type (*) worker
#pragma acc routine (foo7) dtype (nvidia) worker dtype (*) vector
#pragma acc routine (foo8) dtype (nvidia) vector device_type (*) gang
#pragma acc routine (foo9) device_type (nvidia) vector device_type (*) worker
#pragma acc routine (foo10) device_type (nvidia) bind (foo) dtype (*) gang
#pragma acc routine (foo11) device_type (gpu) gang device_type (*) worker
#pragma acc routine (foo12) device_type (gpu) worker dtype (*) worker
#pragma acc routine (foo13) device_type (gpu) vector device_type (*) worker
#pragma acc routine (foo14) dtype (gpu) worker dtype (*) worker
#pragma acc routine (foo15) dtype (gpu) bind (foo) dtype (*) gang

/* { dg-final { scan-tree-dump-times "oacc_parallel wait\\(1\\) vector_length\\(32\\) num_workers\\(100\\) num_gangs\\(100\\) async\\(1\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "oacc_parallel wait\\(1\\) vector_length\\(1\\) num_workers\\(1\\) num_gangs\\(1\\) async\\(1\\) wait\\(2\\) vector_length\\(64\\) num_workers\\(200\\) num_gangs\\(200\\) async\\(2\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc_parallel wait\\(1\\) vector_length\\(1\\) num_workers\\(1\\) num_gangs\\(1\\) async\\(1\\) wait\\(3\\) vector_length\\(128\\) num_workers\\(300\\) num_gangs\\(300\\) async\\(3" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "oacc_kernels async\\(-1\\)" 4 "omplower" } } */

/* { dg-final { scan-tree-dump-times "oacc_kernels async\\(-1\\) wait\\(2\\) async\\(2\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "oacc_kernels async\\(-1\\) wait\\(0\\) async\\(0\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop gang private\\(i1.0\\) private\\(i1\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop gang private\\(i1.1\\) private\\(i1\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop vector private\\(i1.2\\) private\\(i1\\)" 1 "omplower" } } */

/* { dg-final { cleanup-tree-dump "omplower" } } */
