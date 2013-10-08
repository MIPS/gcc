/* { dg-do compile } */
/* { dg-options "-fopenacc" } */

int main(){
	#pragma acc kernels
	{}
	return 0;
}