/* { dg-do compile } */
/* { dg-options "-fopenacc -fdump-tree-gimple" } */

/* test lowering 'if' clause to GIMPLE */

int 
main ()
{
  #pragma acc kernels if (1)
  {}
  return 0;
}

/* { dg-final { scan-tree-dump "pragma acc kernels" "gimple" } } */
/* { dg-final { scan-tree-dump "if" "gimple" } } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
