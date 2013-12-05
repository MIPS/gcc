/* { dg-do compile } */
/* { dg-options "-fopenacc -fdump-tree-gimple" } */

int 
main()
{
  int a, b, c;
  #pragma acc data copyin(a, b) copyout(c)
    {}
  return 0;
}
/* { dg-final { scan-tree-dump "pragma acc data" "gimple" } } */
/* { dg-final { scan-tree-dump "copyin" "gimple" } } */
/* { dg-final { scan-tree-dump "copyout" "gimple" } } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
