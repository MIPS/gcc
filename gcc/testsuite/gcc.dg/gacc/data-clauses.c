/* { dg-do compile } */
/* { dg-options "-fopenacc -fmax-errors=100" } */

/* without suarrays, since they are not implemented yet */
/* todo: some of warnings are not necessary */

struct t {
  int i;
};

void foo (int vi) {
	int i;
	int  ia[10];
	float r;
	float ra[10];
	float *rp = &r;
	float *aa = malloc(sizeof(float)*2);
	struct t ti;
	struct t* tia;
	struct t tit;
	struct t *tip = &tit;

	#pragma acc parallel deviceptr (rp) /* { dg-error "POINTER" } */
		{}
	#pragma acc parallel deviceptr (vi) /* { dg-error "VALUE" } */
		{}
	#pragma acc parallel deviceptr (aa) /* { dg-error "dynamically" } */
		{}

	#pragma acc parallel deviceptr (i, c, r, ia, ra, ti)
		{}
	#pragma acc kernels deviceptr (i, c, r, ia, ra, ti)
		{}
	#pragma acc data deviceptr (i, c, r, ia, ra, ti)
		{}


	#pragma acc parallel copy (tip) /* { dg-error "POINTER" } */
		{}
	#pragma acc parallel copy (tia) /* { dg-error "ALLOCATABLE" } */
		{}
	#pragma acc parallel deviceptr (i) copy (i) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel copy (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc kernels copy (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc data copy (i, c, r, ia, ra, rp, ti, vi, aa)
		{}


	#pragma acc parallel copyin (tip) /* { dg-error "POINTER" } */
		{}
	#pragma acc parallel copyin (tia) /* { dg-error "ALLOCATABLE" } */
		{}
	#pragma acc parallel deviceptr (i) copyin (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (i) copyin (i) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel copyin (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc kernels copyin (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc data copyin (i, c, r, ia, ra, rp, ti, vi, aa)
		{}


	#pragma acc parallel copyout (tip) /* { dg-error "POINTER" } */
		{}
	#pragma acc parallel copyout (tia) /* { dg-error "ALLOCATABLE" } */
		{}
	#pragma acc parallel deviceptr (i) copyout (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (i) copyout (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (i) copyout (i) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel copyout (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc kernels copyout (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc data copyout (i, c, r, ia, ra, rp, ti, vi, aa)
		{}


	#pragma acc parallel create (tip) /* { dg-error "POINTER" } */
		{}
	#pragma acc parallel create (tia) /* { dg-error "ALLOCATABLE" } */
		{}
	#pragma acc parallel deviceptr (i) create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (i) create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (i) create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (i) create (i) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel create (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc kernels create (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc data create (i, c, r, ia, ra, rp, ti, vi, aa)
		{}


	#pragma acc parallel present (tip) /* { dg-error "POINTER" } */
		{}
	#pragma acc parallel present (tia) /* { dg-error "ALLOCATABLE" } */
		{}
	#pragma acc parallel deviceptr (i) present (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (i) present (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (i) present (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (i) present (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel create (i) present (i) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel present (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc kernels present (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc data present (i, c, r, ia, ra, rp, ti, vi, aa)
		{}


	#pragma acc parallel pcopy (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc parallel pcopyin (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc parallel pcopyout (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc parallel pcreate (i, c, r, ia, ra, rp, ti, vi, aa)
		{}


	#pragma acc parallel present_or_copy (tip) /* { dg-error "POINTER" } */
		{}
	#pragma acc parallel present_or_copy (tia) /* { dg-error "ALLOCATABLE" } */
		{}
	#pragma acc parallel deviceptr (i) present_or_copy (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (i) present_or_copy (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (i) present_or_copy (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (i) present_or_copy (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel create (i) present_or_copy (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present (i) present_or_copy (i) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel present_or_copy (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc kernels present_or_copy (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc data present_or_copy (i, c, r, ia, ra, rp, ti, vi, aa)
		{}


	#pragma acc parallel present_or_copyin (tip) /* { dg-error "POINTER" } */
		{}
	#pragma acc parallel present_or_copyin (tia) /* { dg-error "ALLOCATABLE" } */
		{}
	#pragma acc parallel deviceptr (i) present_or_copyin (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (i) present_or_copyin (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (i) present_or_copyin (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (i) present_or_copyin (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel create (i) present_or_copyin (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present (i) present_or_copyin (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copy (i) present_or_copyin (i) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel present_or_copyin (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc kernels present_or_copyin (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc data present_or_copyin (i, c, r, ia, ra, rp, ti, vi, aa)
		{}


	#pragma acc parallel present_or_copyout (tip) /* { dg-error "POINTER" } */
		{}
	#pragma acc parallel present_or_copyout (tia) /* { dg-error "ALLOCATABLE" } */
		{}
	#pragma acc parallel deviceptr (i) present_or_copyout (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (i) present_or_copyout (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (i) present_or_copyout (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (i) present_or_copyout (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel create (i) present_or_copyout (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present (i) present_or_copyout (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copy (i) present_or_copyout (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copyin (i) present_or_copyout (i) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel present_or_copyout (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc kernels present_or_copyout (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc data present_or_copyout (i, c, r, ia, ra, rp, ti, vi, aa)
		{}


	#pragma acc parallel present_or_create (tip) /* { dg-error "POINTER" } */
		{}
	#pragma acc parallel present_or_create (tia) /* { dg-error "ALLOCATABLE" } */
		{}
	#pragma acc parallel deviceptr (i) present_or_create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (i) present_or_create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (i) present_or_create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (i) present_or_create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel create (i) present_or_create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present (i) present_or_create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copy (i) present_or_create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copyin (i) present_or_create (i) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copyout (i) present_or_create (i) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel present_or_create (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc kernels present_or_create (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
	#pragma acc data present_or_create (i, c, r, ia, ra, rp, ti, vi, aa)
		{}
}
