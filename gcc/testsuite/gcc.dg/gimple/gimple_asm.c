/* { dg-do compile } */

/* Test GIMPLE_ASM.  */

void asm0()
{
  asm("movl %ecx %eax"); 
}
/* { dg-final { scan-tree-dump-times "__asm__" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "movl %ecx %eax" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */

