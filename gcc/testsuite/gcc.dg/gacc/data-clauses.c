/* { dg-do compile } */
/* { dg-options "-fopenacc -fmax-errors=100" } */

/* without suarrays, since they are not implemented yet */
/* todo: some of warnings are not necessary */

#include <stdlib.h>

struct t {
  int i;
};

void foo (int vi) {
	int i;
	int  ia[10];
	float r;
	float ra[10];
	float *rp = &r;
	float *aa = (float*) malloc(sizeof(float)*2);
	struct t ti;
	struct t* tia;
	struct t tit;
	struct t *tip = &tit;

	#pragma acc parallel deviceptr (rp) 
		{}
	#pragma acc parallel deviceptr (vi) /* { dg-error "must be pointer for" } */
		{}
	#pragma acc parallel deviceptr (i) /* { dg-error "must be pointer for" } */
		{}
	#pragma acc parallel deviceptr (r) /* { dg-error "must be pointer for" } */
		{}
	#pragma acc parallel deviceptr (ti) /* { dg-error "must be pointer for" } */
		{}
	#pragma acc parallel deviceptr (aa) 
		{}

	#pragma acc parallel deviceptr (ia) /* { dg-error "must be pointer for" } */
		{}
	#pragma acc data deviceptr (ra) /* { dg-error "must be pointer for" } */
		{}

	#pragma acc parallel deviceptr (rp) copy (rp) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel copy (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc kernels copy (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc data copy (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}

	#pragma acc parallel deviceptr (rp) copyin (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (rp) copyin (rp) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel copyin (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc kernels copyin (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc data copyin (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}

	#pragma acc parallel deviceptr (rp) copyout (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (rp) copyout (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (rp) copyout (rp) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel copyout (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc kernels copyout (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc data copyout (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}

	#pragma acc parallel deviceptr (rp) create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (rp) create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (rp) create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (rp) create (rp) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel create (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc kernels create (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc data create (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}

	#pragma acc parallel deviceptr (rp) present (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (rp) present (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (rp) present (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (rp) present (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel create (rp) present (rp) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel present (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc kernels present (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc data present (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}


	#pragma acc parallel pcopy (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc parallel pcopyin (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc parallel pcopyout (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc parallel pcreate (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}

	#pragma acc parallel deviceptr (rp) present_or_copy (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (rp) present_or_copy (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (rp) present_or_copy (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (rp) present_or_copy (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel create (rp) present_or_copy (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present (rp) present_or_copy (rp) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel present_or_copy (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc kernels present_or_copy (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc data present_or_copy (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}

	#pragma acc parallel deviceptr (rp) present_or_copyin (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (rp) present_or_copyin (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (rp) present_or_copyin (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (rp) present_or_copyin (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel create (rp) present_or_copyin (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present (rp) present_or_copyin (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copy (rp) present_or_copyin (rp) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel present_or_copyin (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc kernels present_or_copyin (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc data present_or_copyin (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}

	#pragma acc parallel deviceptr (rp) present_or_copyout (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (rp) present_or_copyout (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (rp) present_or_copyout (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (rp) present_or_copyout (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel create (rp) present_or_copyout (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present (rp) present_or_copyout (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copy (rp) present_or_copyout (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copyin (rp) present_or_copyout (rp) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel present_or_copyout (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc kernels present_or_copyout (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc data present_or_copyout (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}

	#pragma acc parallel deviceptr (rp) present_or_create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copy (rp) present_or_create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyin (rp) present_or_create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel copyout (rp) present_or_create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel create (rp) present_or_create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present (rp) present_or_create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copy (rp) present_or_create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copyin (rp) present_or_create (rp) /* { dg-warning "multiple clauses" } */
		{}
	#pragma acc parallel present_or_copyout (rp) present_or_create (rp) /* { dg-warning "multiple clauses" } */
		{}

	#pragma acc parallel present_or_create (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc kernels present_or_create (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
	#pragma acc data present_or_create (i, r, ia, ra, rp, ti, vi, aa, tia, tip)
		{}
}
