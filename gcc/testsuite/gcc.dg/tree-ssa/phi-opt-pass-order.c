/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-optimized" } */

struct arc
{
  int ident;
};
int bea_is_dual_infeasible( struct arc *arc, int red_cost )
{
    return( (red_cost < 0 && arc->ident == 1)
            || (red_cost > 0 && arc->ident == 2) );
}

/* There should be two == phiopt changes */
/* { dg-final { scan-tree-dump-times " _\[0-9\]+ == " 2 "optimized" } } */
/* { dg-final { scan-tree-dump-times "PHI" 1 "optimized" } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */

