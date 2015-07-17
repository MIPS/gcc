/* { dg-do compile } */
/* { dg-options "-fopenacc -fdump-tree-omplower" } */
/* { dg-prune-output "sorry, unimplemented: device_type clause is not supported yet" } */

void
test ()
{
  int i1, i2, i3, i4, i5, i6;

  /* ACC PARALLEL DEVICE_TYPE: */

#pragma acc parallel device_type (nvidia) async (1) num_gangs (100) num_workers (100) vector_length (32) wait (1)
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
#pragma acc loop dtype (nvidia) gang tile (1)
  for (i1 = 1; i1 < 10; i1++)
#pragma acc loop device_type (nvidia) worker collapse (1)
    for (i2 = 1; i2 < 10; i2++)
#pragma acc loop device_type (nvidia) vector
      for (i3 = 1; i3 < 10; i3++)
#pragma acc loop dtype (nvidia) auto
	for (i4 = 1; i4 < 10; i4++)
#pragma acc loop dtype (nvidia)
	  for (i5 = 1; i5 < 10; i5++)
#pragma acc loop device_type (nvidia) seq
	    for (i6 = 1; i6 < 10; i6++)
	      {
	      }

#pragma acc parallel
#pragma acc loop device_type (nvidia) gang tile (1) dtype (*) seq
  for (i1 = 1; i1 < 10; i1++)
#pragma acc loop dtype (nvidia) worker collapse (1) device_type (*) seq
    for (i2 = 1; i2 < 10; i2++)
#pragma acc loop device_type (nvidia) vector dtype (*) seq
      for (i3 = 1; i3 < 10; i3++)
#pragma acc loop dtype (nvidia) auto device_type (*) seq
	for (i4 = 1; i4 < 10; i4++)
#pragma acc loop device_type (nvidia) device_type (*) seq
	  for (i5 = 1; i5 < 10; i5++)
#pragma acc loop device_type (nvidia) seq
	    for (i6 = 1; i6 < 10; i6++)
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
#pragma acc routine (foo4) dtype (nvidia) seq
#pragma acc routine (foo5) device_type (nvidia) bind (foo)
#pragma acc routine (foo6) device_type (nvidia) gang device_type (*) seq
#pragma acc routine (foo7) dtype (nvidia) worker dtype (*) seq
#pragma acc routine (foo8) dtype (nvidia) vector device_type (*) seq
#pragma acc routine (foo9) device_type (nvidia) seq device_type (*) worker
#pragma acc routine (foo10) device_type (nvidia) bind (foo) dtype (*) seq
#pragma acc routine (foo11) device_type (gpu) gang device_type (*) seq
#pragma acc routine (foo12) device_type (gpu) worker dtype (*) seq
#pragma acc routine (foo13) device_type (gpu) vector device_type (*) seq
#pragma acc routine (foo14) dtype (gpu) seq dtype (*) worker
#pragma acc routine (foo15) dtype (gpu) bind (foo) dtype (*) seq

/* { dg-final { scan-tree-dump-times "oacc_parallel device_type\\(nvidia\\) \\\[ wait\\(1\\) vector_length\\(32\\) num_workers\\(100\\) num_gangs\\(100\\) async\\(1\\) \\\]" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "oacc_parallel device_type\\(nvidia\\) \\\[ wait\\(2\\) vector_length\\(64\\) num_workers\\(200\\) num_gangs\\(200\\) async\\(2\\) \\\] wait\\(1\\) vector_length\\(1\\) num_workers\\(1\\) num_gangs\\(1\\) async\\(1\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "oacc_parallel device_type\\(\\*\\) \\\[ wait\\(10\\) vector_length\\(10\\) num_workers\\(10\\) num_gangs\\(10\\) async\\(10\\) \\\] device_type\\(nvidia\\) \\\[ wait\\(3\\) vector_length\\(128\\) num_workers\\(300\\) num_gangs\\(300\\) async\\(3\\) \\\] wait\\(1\\) vector_length\\(1\\) num_workers\\(1\\) num_gangs\\(1\\) async\\(1\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "oacc_kernels device_type\\(nvidia\\) \\\[ async\\(-1\\) \\\]" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "oacc_kernels device_type\\(nvidia\\) \\\[ wait\\(1\\) async\\(1\\) \\\] async\\(-1\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "oacc_kernels device_type\\(\\*\\) \\\[ wait\\(0\\) async\\(0\\) \\\] device_type\\(nvidia\\) \\\[ wait\\(2\\) async\\(2\\) \\\] async\\(-1\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(nvidia\\) \\\[ tile\\(1\\) gang \\\] private\\(i1\\.0\\) private\\(i1\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(\\*\\) \\\[ seq \\\] device_type\\(nvidia\\) \\\[ tile\\(1\\) gang \\\] private\\(i1\\.1\\) private\\(i1\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(nvidia\\) \\\[ collapse\\(1\\) worker \\\] private\\(i2\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(nvidia\\) \\\[ vector \\\] private\\(i3\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(nvidia\\) \\\[ auto \\\] private\\(i4\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(nvidia\\) \\\[ \\\] private\\(i5\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(nvidia\\) \\\[ seq \\\] private\\(i6\\)" 2 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(\\*\\) \\\[ seq \\\] device_type\\(nvidia\\) \\\[ collapse\\(1\\) worker \\\] private\\(i2\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(\\*\\) \\\[ seq \\\] device_type\\(nvidia\\) \\\[ vector \\\] private\\(i3\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(\\*\\) \\\[ seq \\\] device_type\\(nvidia\\) \\\[ auto \\\] private\\(i4\\)" 1 "omplower" } } */

/* { dg-final { scan-tree-dump-times "acc loop device_type\\(\\*\\) \\\[ seq \\\] device_type\\(nvidia\\) \\\[ \\\] private\\(i5\\)" 1 "omplower" } } */
