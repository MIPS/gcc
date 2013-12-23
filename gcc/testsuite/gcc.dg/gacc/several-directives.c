/* { dg-do compile } */
/* { dg-options "-fopenacc" } */

int main() {
	/* only one directive-name may appear in directive */
	#pragma acc parallel kernels /* { dg-error "expected" } */
	{}
	return 0;
}