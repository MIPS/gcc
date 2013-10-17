/* { dg-do compile } */
/* { dg-options "-fopenacc -fdump-tree-original" } */

int main() {

	int i;

	#pragma acc host_data use_device(i)
		{}
}
/* { dg-final { scan-tree-dump "pragma acc host_data use_device" "original" } } */
/* { dg-final { cleanup-tree-dump "original" } } */
