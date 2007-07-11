/* { dg-do compile } */

/* Test the when the switch conditional has something complex in it.  */
int
foo2 (int a, int b)
{
    switch (a+a)
      {
        case 0:
          return a;
        case 1:
          return b;
        default:
          return 1;
      }
}
/* { dg-final { scan-tree-dump-times "gimpleir: switch \\(D.*\\)" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
