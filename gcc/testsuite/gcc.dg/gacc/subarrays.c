/* { dg-do compile } */
/* { dg-options "-fopenacc" } */
int main() {

	int a[10], b[10][10], c[5], i;

	#pragma acc parallel copy(a[1:4])
	{}
	#pragma acc parallel copy(a[1 + 0 : 5 + 2])
	{}
	#pragma acc parallel copy(a[:3])
	{}
	#pragma acc parallel copy(a[3:])
	{}
	#pragma acc parallel copy(a[:]) /* { dg-error "Syntax error in OpenACC variable list" } */
	#pragma acc parallel copy(a[2:3,2:3]) /* { dg-error "Number of dimensions" } */
	{}
	/* TODO: there must be warning */
	#pragma acc parallel copy (a[:11])
	{}
	#pragma acc parallel copy (a[i:])
	{}

	#pragma acc parallel copy (a[:b]) /* { dg-error "scalar INTEGER expression" } */
	{}

	#pragma acc parallel copy (b[1:3,2:4])
	{}
	#pragma acc parallel copy (b[2:3]) /* { dg-error "Number of dimensions" } */
	{}
	#pragma acc parallel copy (b[1:, 4:6]) /* { dg-warning "whole dimension" } */
	{}

	/* TODO: there must be warning */
	#pragma acc parallel copy (c[7:])
	{}
}