/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-forwprop1-details" }  */

int abs(int);

int match_1 (int x)
{
  int t1 = abs (x);
  int match_1_val = abs (t1);
  return match_1_val;
}  
/* { dg-final { scan-tree-dump "gimple_simplified to match_1_val_\\d\+ = ABS_EXPR <x_\\d\+\\(D\\)" "forwprop1" } } */
