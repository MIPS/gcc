/* { dg-do compile } */

/* Test GIMPLE_ASM.  */

void asm1()
{
  int a=10, b;
  asm ("movl %1, %%eax; "
        "movl %%eax, %0;"
        :"=r"(b)        /* output */
        :"r"(a)         /* input */
        :"%eax"         /* clobbered register */
 );
}
/* { dg-final { scan-tree-dump-times "__asm__" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "movl %1, %%eax; movl %%eax, %0;" 
   1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "b:" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "a:" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "\\\"%eax\\\"" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */

