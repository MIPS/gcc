/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-tail2-details" } */
int q(int a);
int *v;
int
t(int a)
{
	int r,r1;
	if (a)
		r1=r = q(a-1);
	else
		return 0;
	/* Dead alloca should not disturb us.  */
	if (r!=r1)
		v=alloca(r);
	return r;
}
/* { dg-final { scan-tree-dump-times "Found tail call" 1 "tail2"} } */
