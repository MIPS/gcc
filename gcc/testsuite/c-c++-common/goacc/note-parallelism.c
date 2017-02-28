/* Test the output of -fopt-info-note-omp.  */

/* { dg-additional-options "-fopt-info-note-omp" } */

int
main ()
{
  int x, y, z;

#pragma acc parallel loop seq
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop gang
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop worker
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop vector
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop gang vector
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop gang worker
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop worker vector
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop gang worker vector
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop
  for (x = 0; x < 10; x++)
#pragma acc loop
    for (y = 0; y < 10; y++)
      ;

#pragma acc parallel loop gang
  for (x = 0; x < 10; x++)
#pragma acc loop worker
    for (y = 0; y < 10; y++)
#pragma acc loop vector
      for (z = 0; z < 10; z++)
	;

  return 0;
}

/* { dg-message "note-parallelism.c:10:9: note: Detected parallelism <acc loop seq>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:14:9: note: Detected parallelism <acc loop gang>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:18:9: note: Detected parallelism <acc loop worker>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:22:9: note: Detected parallelism <acc loop vector>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:26:9: note: Detected parallelism <acc loop gang vector>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:30:9: note: Detected parallelism <acc loop gang worker>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:34:9: note: Detected parallelism <acc loop worker vector>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:38:9: note: Detected parallelism <acc loop gang worker vector>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:42:9: note: Detected parallelism <acc loop gang vector>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:46:9: note: Detected parallelism <acc loop gang worker>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:48:9: note: Detected parallelism <acc loop vector>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:52:9: note: Detected parallelism <acc loop gang>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:54:9: note: Detected parallelism <acc loop worker>" "" { target *-*-* } 0 } */
/* { dg-message "note-parallelism.c:56:9: note: Detected parallelism <acc loop vector>" "" { target *-*-* } 0 } */
