/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-tail1-details" } */
int
t(int a)
{
	if (a)
		return t(a-1);
	else
		return 0;
}
/* { dg-final { scan-tree-dump-times "Eliminated tail recursion" 1 "tail1"} } */
