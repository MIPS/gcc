/* { dg-do compile } */
/* { dg-options "-fopenacc -fmax-errors=100" } */

/* tests async, num_gangs, num_workers, vector_length, gang, worker, vector clauses */

int main()
{
  int i;

#pragma acc parallel async
  {}

#pragma acc parallel async(3)
  {}

#pragma acc parallel async(i)
  {}

#pragma acc parallel async(i+1)
  {}

#pragma acc parallel async(-1) /* { dg-warning "must be positive" } */
  {}

#pragma acc parallel async(0) /* { dg-warning "must be positive" } */
  {}

#pragma acc parallel async() /* { dg-error "expected expression" } */
  {}

#pragma acc parallel async(1.5) /* { dg-error "expected integer expression" } */
  {}

#pragma acc parallel async("1") /* { dg-error "expected integer expression" } */
  {}

#pragma acc kernels async
  {}

#pragma acc kernels async(3)
  {}

#pragma acc kernels async(i)
  {}

#pragma acc kernels async(i+1)
  {}

#pragma acc kernels async(-1) /* { dg-warning "must be positive" } */
  {}

#pragma acc kernels async(0) /* { dg-warning "must be positive" } */
  {}

#pragma acc kernels async() /* { dg-error "expected expression" } */
  {}

#pragma acc kernels async(1.5) /* { dg-error "expected integer expression" } */
  {}

#pragma acc kernels async("1") /* { dg-error "expected integer expression" } */
  {}


#pragma acc parallel num_gangs /* { dg-error "expected" } */
  {}

#pragma acc parallel num_gangs(3)
  {}

#pragma acc parallel num_gangs(i)
  {}

#pragma acc parallel num_gangs(i+1)
  {}

#pragma acc parallel num_gangs(-1) /* { dg-warning "must be positive" } */
  {}

#pragma acc parallel num_gangs(0) /* { dg-warning "must be positive" } */
  {}

#pragma acc parallel num_gangs() /* { dg-error "expected expression" } */
  {}

#pragma acc parallel num_gangs(1.5) /* { dg-error "expected integer expression" } */
  {}

#pragma acc parallel num_gangs("1") /* { dg-error "expected integer expression" } */
  {}


#pragma acc parallel num_workers /* { dg-error "expected" } */
  {}

#pragma acc parallel num_workers(3)
  {}

#pragma acc parallel num_workers(i)
  {}

#pragma acc parallel num_workers(i+1)
  {}

#pragma acc parallel num_workers(-1) /* { dg-warning "must be positive" } */
  {}

#pragma acc parallel num_workers(0) /* { dg-warning "must be positive" } */
  {}

#pragma acc parallel num_workers() /* { dg-error "expected expression" } */
  {}

#pragma acc parallel num_workers(1.5) /* { dg-error "expected integer expression" } */
  {}

#pragma acc parallel num_workers("1") /* { dg-error "expected integer expression" } */
  {}


#pragma acc parallel vector_length /* { dg-error "expected" } */
  {}

#pragma acc parallel vector_length(3)
  {}

#pragma acc parallel vector_length(i)
  {}

#pragma acc parallel vector_length(i+1)
  {}

#pragma acc parallel vector_length(-1) /* { dg-warning "must be positive" } */
  {}

#pragma acc parallel vector_length(0) /* { dg-warning "must be positive" } */
  {}

#pragma acc parallel vector_length() /* { dg-error "expected expression" } */
  {}

#pragma acc parallel vector_length(1.5) /* { dg-error "expected integer expression" } */
  {}

#pragma acc parallel vector_length("1") /* { dg-error "expected integer expression" } */
  {}


#pragma acc loop gang
  for (i = 0; i < 10; i++) {}
#pragma acc loop gang(3)
  for (i = 0; i < 10; i++) {}
#pragma acc loop gang(i)
  for (i = 0; i < 10; i++) {}
#pragma acc loop gang(i+1)
  for (i = 0; i < 10; i++) {}
#pragma acc loop gang(-1) /* { dg-warning "must be positive" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop gang(0) /* { dg-warning "must be positive" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop gang() /* { dg-error "expected expression" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop gang(1.5) /* { dg-error "expected integer expression" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop gang("1") /* { dg-error "expected integer expression" } */
  for (i = 0; i < 10; i++) {}


#pragma acc loop worker
  for (i = 0; i < 10; i++) {}
#pragma acc loop worker(3)
  for (i = 0; i < 10; i++) {}
#pragma acc loop worker(i)
  for (i = 0; i < 10; i++) {}
#pragma acc loop worker(i+1)
  for (i = 0; i < 10; i++) {}
#pragma acc loop worker(-1) /* { dg-warning "must be positive" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop worker(0) /* { dg-warning "must be positive" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop worker() /* { dg-error "expected expression" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop worker(1.5) /* { dg-error "expected integer expression" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop worker("1") /* { dg-error "expected integer expression" } */
  for (i = 0; i < 10; i++) {}


#pragma acc loop vector
  for (i = 0; i < 10; i++) {}
#pragma acc loop vector(3)
  for (i = 0; i < 10; i++) {}
#pragma acc loop vector(i)
  for (i = 0; i < 10; i++) {}
#pragma acc loop vector(i+1)
  for (i = 0; i < 10; i++) {}
#pragma acc loop vector(-1) /* { dg-warning "must be positive" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop vector(0) /* { dg-warning "must be positive" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop vector() /* { dg-error "expected expression" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop vector(1.5) /* { dg-error "expected integer expression" } */
  for (i = 0; i < 10; i++) {}
#pragma acc loop vector("1") /* { dg-error "expected integer expression" } */
  for (i = 0; i < 10; i++) {}

}