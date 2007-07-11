/* { dg-do compile } */
/* Test generation of default label.  */
int 
foo2 (int a)
{
    switch (a)
      {
        case 0:
          return a;
        case 1:
          return a;
      }
}
/* { dg-final { scan-tree-dump-times "gimpleir: default:" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */

