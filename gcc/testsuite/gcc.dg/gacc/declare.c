/* { dg-do compile } */
/* { dg-options "-fopenacc -fdump-tree-original" } */
int main() {

int i;

#pragma acc declare copy(i)

}
/* { dg-final { scan-tree-dump "pragma acc declare copy" "original" } } */
/* { dg-final { cleanup-tree-dump "original" } } */