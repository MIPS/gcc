/* { dg-do compile } */
/* { dg-options "-fopenacc" } */

int main() {


	int x;
	int i;

	#pragma acc parallel if /* { dg-error "Unclassifiable OpenACC directive" } */
	#pragma acc parallel if () /* { dg-error "Invalid character" } */
	#pragma acc parallel if (i) /* { dg-error "scalar LOGICAL expression" } */
	{}
	#pragma acc parallel if (1) /* { dg-error "scalar LOGICAL expression" } */
	{}
	#pragma acc kernels if (i) /* { dg-error "scalar LOGICAL expression" } */
	{}
	#pragma acc kernels if /* { dg-error "Unclassifiable OpenACC directive" } */
	#pragma acc kernels if () /* { dg-error "Invalid character" } */
	#pragma acc kernels if (1) /* { dg-error "scalar LOGICAL expression" } */
	{}
	#pragma acc data if /* { dg-error "Unclassifiable OpenACC directive" } */
	#pragma acc data if () /* { dg-error "Invalid character" } */
	#pragma acc data if (i) /* { dg-error "scalar LOGICAL expression" } */
	{}
	#pragma acc data if (1) /* { dg-error "scalar LOGICAL expression" } */
	{}

	/* at most one if clause may appear */
	#pragma acc parallel if (0) if (0) { dg-error "Unclassifiable OpenACC directive" }
	#pragma acc kernels if (0) if (0) { dg-error "Unclassifiable OpenACC directive" }
	#pragma acc data if (0) if (0) { dg-error "Unclassifiable OpenACC directive" }

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