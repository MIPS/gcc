/* { dg-do compile }  */
/* { dg-options "-fopenacc -fdump-tree-original -fdump-tree-gimple" }  */

int
main()
{
   int i, j, k, m, sum;

    
   {
      #pragma acc kernels loop seq collapse(2)
      for(i = 0; i < 10; i++)
         for(j = 0; j < 10; j++)
            {}

      #pragma acc kernels loop independent gang (3)
      for( i = 0; i < 10; i++)
         {
            #pragma acc loop worker(3)
            for( j = 0; j < 10; j++)
               {
                  #pragma acc loop vector(5)
                  for( k = 0; k < 10; k++)
                     {}
               }
         }
   }

   sum = 0;
   #pragma acc parallel loop private(m) reduction(+:sum)
         for( i = 0; i < 10; i++)
            sum += 1;
      

   return 0;
}

/* { dg-final { scan-tree-dump "pragma acc loop" "original" } }  */

/* { dg-final { scan-tree-dump "seq" "original" } } */
/* { dg-final { scan-tree-dump "collapse" "original" } }  */
/* { dg-final { scan-tree-dump "independent" "original" } }  */
/* { dg-final { scan-tree-dump "gang" "original" } }  */
/* { dg-final { scan-tree-dump "worker" "original" } }  */
/* { dg-final { scan-tree-dump "vector" "original" } }  */

/* { dg-final { scan-tree-dump "private" "original" } }  */
/* { dg-final { scan-tree-dump "reduction" "original" } }  */

/* { dg-final { scan-tree-dump "pragma acc loop" "gimple" } }  */

/* { dg-final { scan-tree-dump "seq" "gimple" } } */
/* { dg-final { scan-tree-dump "collapse" "gimple" } }  */
/* { dg-final { scan-tree-dump "independent" "gimple" } }  */
/* { dg-final { scan-tree-dump "gang" "gimple" } }  */
/* { dg-final { scan-tree-dump "worker" "gimple" } }  */
/* { dg-final { scan-tree-dump "vector" "gimple" } }  */

/* { dg-final { scan-tree-dump "private" "gimple" } }  */
/* { dg-final { scan-tree-dump "reduction" "gimple" } }  */
/* { dg-final { cleanup-tree-dump "gimple" } }  */
/* { dg-final { cleanup-tree-dump "original" } }  */