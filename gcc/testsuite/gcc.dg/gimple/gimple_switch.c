/* { dg-do compile } */

/* Test GIMPLE_SWITCH.  */

int
foo (int a)
{
    switch (a)
      {
        default:
          return 2;
        case 0:
          return a;
        case 1:
          return 1;
      }

}
/* { dg-final { scan-tree-dump-times "gimpleir: switch \\(a\\) <default:, case 0:, case 1:>" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: default:" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: case 0:" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: case 1:" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
