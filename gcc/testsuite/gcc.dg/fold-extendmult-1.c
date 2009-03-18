/* { dg-do compile } */
/* { dg-options "-fstrict-overflow -fdump-tree-original" } */

unsigned long foo (int i)
{
    return (unsigned long)((long)(i * 2) * 8);
}

/* { dg-final { scan-tree-dump "i \\\*\/nv 16" "original" } } */
/* { dg-final { cleanup-tree-dump "original" } } */
