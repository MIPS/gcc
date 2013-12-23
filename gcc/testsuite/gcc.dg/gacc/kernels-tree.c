/* { dg-do compile } */
/* { dg-options "-fopenacc -fdump-tree-original" } */

/* test for tree-dump-original and spaces-commas */

int main() {

	int q, i, j, k, m, n, o, p, r, s, t, *u, v, w;
	int l;

	#pragma acc kernels if(l) async copy(i), copyin(j), copyout(k), create(m) \
		present(o), pcopy(p), pcopyin(r), pcopyout(s), pcreate(t) \
		deviceptr(u)
			{}

}
/* { dg-final { scan-tree-dump "pragma acc kernels" "original" } } */

/* { dg-final { scan-tree-dump "if" "original" } } */
/* { dg-final { scan-tree-dump "async" "original" } } */
/* { dg-final { scan-tree-dump "copy" "original" } } */
/* { dg-final { scan-tree-dump "copyin" "original" } } */
/* { dg-final { scan-tree-dump "copyout" "original" } } */
/* { dg-final { scan-tree-dump "create" "original" } } */

/* { dg-final { scan-tree-dump "present" "original" } } */
/* { dg-final { scan-tree-dump "present_or_copy" "original" } } */
/* { dg-final { scan-tree-dump "present_or_copyin" "original" } } */
/* { dg-final { scan-tree-dump "present_or_copyout" "original" } } */
/* { dg-final { scan-tree-dump "present_or_create" "original" } } */

/* { dg-final { scan-tree-dump "deviceptr" "original" } } */
/* { dg-final { cleanup-tree-dump "original" } } */
