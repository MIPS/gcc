/* { dg-do compile } */

/* Test GIMPLE_ASM.  */

void asm2()
{
  int a=10, b;
  asm ("movl %1, %%eax; "
        "movl %%eax, %0;"
        :               /* output */
        :               /* input */
        :"%eax"         /* clobbered register */
 );
}
/* { dg-final { scan-tree-dump-times "__asm__" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "movl %1, %%eax; movl %%eax, %0;"
   1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "\\\"%eax\\\"" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
