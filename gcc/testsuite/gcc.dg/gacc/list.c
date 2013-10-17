/* { dg-do compile } */
/* { dg-options "-fopenacc -fmax-errors=100" } */

int main() {


	int i, j, k, l, a[10];
	float* p1 = NULL;
	double c, d[10]

	#pragma acc parallel private(i)
	{}

	#pragma acc parallel private(a)
	{}

	#pragma acc parallel private(c, d)
	{}

	#pragma acc parallel private(i, j, k, l, a)
	{}

	#pragma acc parallel private (i) private (j)
	{}

	#pragma acc parallel private /* { dg-error "Unclassifiable OpenACC directive" } */

	#pragma acc parallel private() /* { dg-error "Syntax error in OpenACC variable list" } */

	#pragma acc parallel private(a[1:2]) /* { dg-error "Syntax error in OpenACC variable list" } */

	#pragma acc parallel private(10) /* { dg-error "Syntax error in OpenACC variable list" } */

	#pragma acc parallel private(i, j, i) /* { dg-warning "present on multiple clauses" } */
	{}

	#pragma acc parallel private(p1) /* { dg-error "POINTER" } */
	{}

	#pragma acc parallel firstprivate(i)
	{}

	#pragma acc parallel firstprivate(c, d)
	{}

	#pragma acc parallel firstprivate(a)
	{}

	#pragma acc parallel firstprivate(i, j, k, l, a)
	{}

	#pragma acc parallel firstprivate (i) firstprivate (j)
	{}

	#pragma acc parallel firstprivate /* { dg-error "Unclassifiable OpenACC directive" } */

	#pragma acc parallel firstprivate() /* { dg-error "Syntax error in OpenACC variable list" } */

	#pragma acc parallel firstprivate(a[1:3]) /* { dg-error "Syntax error in OpenACC variable list" } */

	#pragma acc parallel firstprivate(10) /* { dg-error "Syntax error in OpenACC variable list" } */

	#pragma acc parallel firstprivate (i, j, i) /* { dg-warning "present on multiple clauses" } */
	{}

	#pragma acc parallel firstprivate(p1) /* { dg-error "POINTER" } */
	{}

	#pragma acc parallel private (i) firstprivate (i) /* { dg-warning "present on multiple clauses" } */
	{}

	#pragma acc host_data use_device(i)
	{}

	#pragma acc host_data use_device(c, d)
	{}

	#pragma acc host_data use_device(a)
	{}

	#pragma acc host_data use_device(i, j, k, l, a)
	{}

	#pragma acc host_data use_device (i) use_device (j)
	{}

	#pragma acc host_data use_device /* { dg-error "Unclassifiable OpenACC directive" } */

	#pragma acc host_data use_device() /* { dg-error "Syntax error in OpenACC variable list" } */

	#pragma acc host_data use_device(a[1:3]) /* { dg-error "Syntax error in OpenACC variable list" } */

	#pragma acc host_data use_device(10) /* { dg-error "Syntax error in OpenACC variable list" } */

	#pragma acc host_data use_device(i, j, i) /* { dg-warning "present on multiple clauses" } */
	{}

	#pragma acc host_data use_device(p1) /* { dg-error "POINTER" } */
	{}

}