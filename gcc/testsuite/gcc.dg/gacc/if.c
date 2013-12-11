/* { dg-do compile } */
/* { dg-options "-fopenacc" } */

int main() {


	int x;
	int i;

	#pragma acc parallel if /* { dg-error "expected" } */
	{}
	#pragma acc parallel if () /* { dg-error "expected expression" } */
	{}
	#pragma acc parallel if (i) 
	{}
	#pragma acc parallel if (1) 
	{}
	#pragma acc kernels if (i) 
	{}
	#pragma acc kernels if /* { dg-error "expected" } */
	{}
	#pragma acc kernels if () /* { dg-error "expected expression" } */
	{}
	#pragma acc kernels if (1) 
	{}
	#pragma acc data if /* { dg-error "expected" } */
	{}
	#pragma acc data if () /* { dg-error "expected expression" } */
	{}
	#pragma acc data if (i) 
	{}
	#pragma acc data if (1) 
	{}

	/* at most one if clause may appear */
	#pragma acc parallel if (0) if (0) /* { dg-error "too many" } */
	{}
	#pragma acc kernels if (0) if (0) /* { dg-error "too many" } */
	{}
	#pragma acc data if (0) if (0) /* { dg-error "too many" } */
	{}

	#pragma acc parallel if (x)
	{}
	#pragma acc parallel if (1)
	{}
	#pragma acc parallel if (i>1)
	{}
	#pragma acc kernels if (x)
	{}
	#pragma acc kernels if (1)
	{}
	#pragma acc kernels if (i>1)
	{}
	#pragma acc data if (x)
	{}
	#pragma acc data if (1)
	{}
	#pragma acc data if (i>1)
	{}

}