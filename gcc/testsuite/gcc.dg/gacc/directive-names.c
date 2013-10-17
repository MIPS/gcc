/* { dg-do compile } */
/* { dg-options "-fopenacc" } */

int main() {


	#pragma acc parallel
		{}

	#pragma acc kernels
		{}

	#pragma acc data
		{}

	#pragma acc host_data
		{}

}
